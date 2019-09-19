/***********************license start************************************
 * Copyright (c) 2003-2014 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **********************license end**************************************/

/**
 * @file octeon-remote.c
 *
 * Interface to Octeon remotely using various transports
 *
 * $Id: octeon-remote.c 156174 2017-03-20 22:14:34Z cchavva $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <memory.h>
#include <time.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/time.h>
#include "octeon-remote.h"
#define CVMX_ADD_IO_SEG(a) (a)
#include "cvmx.h"
#include "cvmx-swap.h"
#include "cvmx-bootmem.h"
#include "cvmx-l2c-defs.h"
#include <octeon_mem_map.h>
#include <lib_octeon_shared.h>

#define ULL unsigned long long

typedef struct
{
    uint64_t count;
    uint64_t total;
    uint64_t min;
    uint64_t max;
} time_record_t;

typedef enum
{
    TIME_OPEN,
    TIME_CLOSE,
    TIME_READ_CSR,
    TIME_WRITE_CSR,
    TIME_READ_CSR32,
    TIME_WRITE_CSR32,
    TIME_READ_MEM,
    TIME_WRITE_MEM,
    TIME_GET_MODEL,
    TIME_START_CORES,
    TIME_STOP_CORES,
    TIME_GET_RUNNING_CORES,
    TIME_GET_CORE_STATE,
    TIME_SET_CORE_STATE,
    TIME_LOCK,
    TIME_UNLOCK,
    TIME_RESET,
    TIME_READ_REGISTER,
    TIME_WRITE_REGISTER,
    TIME_GET_SAMPLE,
    TIME_MAX
} time_index_t;

/* Each bit represents a valid COP0 register bit=reg*8+sel VALID_COP0[bit/64] |= 1<<(bit%63) */
uint64_t OCTEON_REMOTE_VALID_COP0[4];

static const int DEBUG_LEVEL_COLORS[] = {
    /*Errors   Normal       Info         Detail        Calls       Detail2        Detail3 */
    31/*red*/, 0/*normal*/, 32/*green*/, 33/*yellow*/, 36/*cyan*/, 35/*magenta*/, 4/*Underline*/};
static const char *TIMING_NAMES[TIME_MAX] = {
    "Open",
    "Close",
    "Read CSR",
    "Write CSR",
    "Read CSR32",
    "Write CSR32",
    "Read memory",
    "Write memory",
    "Get model",
    "Start cores",
    "Stop cores",
    "Get running cores",
    "Get core state",
    "Set core state",
    "Lock",
    "Unlock",
    "Reset",
    "Read CPU register",
    "Write CPU register",
    "Get sample"
};

static octeon_remote_funcs_t remote_funcs;
static char lock_file_name[64];
static int lock_count = 0;
static int lock_fd = -1;
static time_record_t timing_data[TIME_MAX];

static uint64_t timing_get_clock(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000ull + tv.tv_usec;
}

static void timing_update(time_index_t index, uint64_t start_time)
{
    uint64_t delta = timing_get_clock() - start_time;
    timing_data[index].count++;
    timing_data[index].total += delta;
    if (delta < timing_data[index].min)
        timing_data[index].min = delta;
    if (delta > timing_data[index].max)
        timing_data[index].max = delta;
}


/**
 * Default lock implementation using a file under /var/lock to insure
 * exclusivity.
 */
static void default_lock(void)
{
    OCTEON_REMOTE_DEBUG_CALLED();
    lock_fd = open(lock_file_name, O_CREAT|O_WRONLY, 0666);
    if (lock_fd == -1)
    {
        octeon_remote_perror(-1, "%s", lock_file_name);
        exit(1);
    }
    /* Closing the file automatically frees the file lock, so we don't
        need to worry about cleanup after a signal */
    if (lockf(lock_fd, F_LOCK, 0))
    {
        octeon_remote_perror(-1, "%s", lock_file_name);
        exit(1);
    }
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Unlock used with the above lock
 */
static void default_unlock(void)
{
    OCTEON_REMOTE_DEBUG_CALLED();
    /* Closing the file automatically frees the file lock */
    close(lock_fd);
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Default Octeon reset implementation if the remote protocol does
 * supply one.
 *
 * @param stop_core Attempt to stop core 0 from booting, if possible. The default
 *                  implementation does not support this.
 *
 * @return Zero on success, negative on failure.
 */
static int default_reset(int stop_core __attribute__ ((unused)))
{
    OCTEON_REMOTE_DEBUG_CALLED();
    if (!OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2) &&
	!OCTEON_IS_MODEL(OCTEON_CN31XX) &&
	!OCTEON_IS_MODEL(OCTEON_CN6XXX) &&
	!OCTEON_IS_MODEL(OCTEON_CN7XXX))
        octeon_remote_write_csr(CVMX_CIU_SOFT_BIST, 1);
    if (OCTEON_IS_OCTEON3())
        octeon_remote_write_csr(CVMX_RST_SOFT_RST, 1);
    else
        octeon_remote_write_csr(CVMX_CIU_SOFT_RST, 1);
    OCTEON_REMOTE_DEBUG_RETURNED();
    return 0;
}


/**
 * Default Octeon read regsiter implementation if the remote protocol does
 * supply one.
 *
 * @param core   Core to read
 * @param reg    Register to read
 *
 * @return Register value
 */
static uint64_t default_read_register(int core, int reg)
{
    octeon_remote_registers_t registers;
    OCTEON_REMOTE_DEBUG_CALLED("%d, 0x%x", core, reg);
    octeon_remote_get_core_state(core, &registers);
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)registers.regs[reg>>8][reg&255]);
    return registers.regs[reg>>8][reg&255];
}


/**
 * Default Octeon write regsiter implementation if the remote protocol does
 * supply one.
 *
 * @param core   Core to write
 * @param reg    Register to write
 * @param value  Value to write
 */
static void default_write_register(int core, int reg, uint64_t value)
{
    octeon_remote_registers_t registers;
    OCTEON_REMOTE_DEBUG_CALLED("%d, 0x%x, 0x%llx", core, reg, (ULL)value);
    octeon_remote_get_core_state(core, &registers);
    registers.regs[reg>>8][reg&255] = value;
    octeon_remote_set_core_state(core, &registers);
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * All the various remote library print functions behave differently
 * based on the debug level and some special debug flags. This routine
 * is the common implementation of the low level output. Variable
 * argument lists are used so this can be called with unknown
 * argument list gotten from printf style functions.
 *
 * @param level  Debug level to print output at
 * @param show_banner
 *               Non zero if a normal banner should be printed before the supplied
 *               output.
 * @param format Printf style format string
 * @param ap     va_list if printf arguments
 */
static void octeon_remote_output_arg(int level, int show_banner,
                                     const char *format, va_list ap)
{
    if (level <= (0xff & remote_funcs.debug))
    {
        /* Setting bit 8 in the debug level disables color */
        int use_color = !(remote_funcs.debug & 0x100);
        if (use_color)
        {
            int color = 1; /* Index 1 is the normal terminal color */
            if (level < 0)
                color = 0; /* Index 0 is the error color */
            else if (level+1 < (int)(sizeof(DEBUG_LEVEL_COLORS)/sizeof(DEBUG_LEVEL_COLORS[0])))
                color = level+1;
            printf("\033[%dm", DEBUG_LEVEL_COLORS[color]);
        }
        if (show_banner)
        {
            char buf[32];
            time_t t;
            struct tm lt;
            t = time(NULL);
            localtime_r(&t, &lt);
            strftime(buf, sizeof(buf), "%m-%d %H:%M:%S", &lt);
            printf("%s:%*s", buf, remote_funcs.debug_indent*2+1, "");
        }
        vprintf(format, ap);
        if (use_color)
            printf("\033[0m");
        fflush(stdout);
    }
}


/**
 * Utility function to display debug messages. Only messages at
 * or below the current debug level are displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_debug(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    octeon_remote_output_arg(level, 1, format, args);
    va_end(args);
}


/**
 * This function is called by the function entry/exit macros in
 * octeon-remote.h. This function should not be called except by
 * these macros.
 *
 * @param is_return Non zero if this call is the return of a function
 * @param format    printf format to use a args or return value
 */
void octeon_remote_debug_call(int is_return, const char *format, ...)
{
    const int level = 3;
    if (level <= (0xff & remote_funcs.debug))
    {
        va_list args;
        if (is_return)
            remote_funcs.debug_indent--;
        va_start(args, format);
        octeon_remote_output_arg(level, 1, format, args);
        va_end(args);
        if (!is_return)
            remote_funcs.debug_indent++;
    }
}


/**
 * Utility function to display output messages without line
 * prefixes. Only messages at or below the current debug level are
 * displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_output(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    octeon_remote_output_arg(level, 0, format, args);
    va_end(args);
}


/**
 * Utility function to display library error messages similar to
 * perror(). As an extension, this functions takes full printf
 * style arguments instead of the simple string used by perror().
 * Only messages at or below the current debug level are
 * displayed.
 *
 * @param level  Level for this message
 * @param format prinf format
 */
void octeon_remote_perror(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    octeon_remote_output_arg(level, 1, format, args);
    octeon_remote_output(level, ": %s\n", strerror(errno));
    va_end(args);
}

/**
 * Determine the byte order mode of a remote Oceton.
 *
 * @param core
 *              Core to query for byte order mode.
 * @return One if the core is in LE mode.
 */
static int octeon_remote_core_is_le(int core)
{
    uint64_t data;

    if (OCTEON_IS_MODEL(OCTEON_CN61XX) ||
            OCTEON_IS_MODEL(OCTEON_CN63XX) ||
            OCTEON_IS_MODEL(OCTEON_CN66XX) ||
            OCTEON_IS_MODEL(OCTEON_CN68XX) ||
            OCTEON_IS_MODEL(OCTEON_CNF71XX))
    {
        data = octeon_remote_read_csr(CVMX_L2C_COP0_MAPX((core << 8) | (9 << 3) | (7 << 0)));

        return !!(data & 2);
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN70XX) ||
            OCTEON_IS_MODEL(OCTEON_CN73XX) ||
            OCTEON_IS_MODEL(OCTEON_CN78XX) ||
            OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X) ||
            OCTEON_IS_MODEL(OCTEON_CNF75XX))
    {
        octeon_remote_write_csr(CVMX_L2C_COP0_ADR, (core << 16) | (9 << 3) | (7 << 0));
        /* User.CvmCtl
           remote_funcs.write_csr(CVMX_L2C_COP0_ADR, (core << 16) | (1 << 8) | (9 << 3) | (7 << 0));
           */
        data = octeon_remote_read_csr(CVMX_L2C_COP0_DAT);

        return !!(data & 2);
    }
    else
    {
        octeon_remote_debug(-1, "Your Octeon model is not yet fully supported, assuming BE mode\n");
        return 0;
    }
}

/**
 * Determine the byte order mode of a remote Oceton.
 *
 * @return One if Octeon is in LE mode.
 */
int octeon_remote_is_le(void)
{
    return remote_funcs.is_le;
}


/**
 * Determine the byte order mode of a remote Oceton memory model.
 *
 * @return One if Octeon memory should be accessed is in LE mode.
 */
int octeon_remote_is_mem_le(void)
{
    return remote_funcs.is_le && remote_funcs.memory_le;
}

/**
 * Determine the byte order mode of a remote Oceton memory model.
 *
 * @return One if Octeon memory should be accessed is in LE mode.
 */
int octeon_remote_protocol(void)
{
    return remote_funcs.memory_le;
}

/**
 * Open a connection to a remote Octeon.
 *
 * @param remote_spec
 *               Remote protocol spec to use. If NULL, it will be taken from the
 *               OCTEON_REMOTE_PROTOCOL environment variable.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_open(const char *remote_spec, int debug)
{
    int result;
    time_index_t t;

    if (remote_spec == NULL)
    {
        remote_spec = getenv("OCTEON_REMOTE_PROTOCOL");
        if (remote_spec == NULL)
            remote_spec = "UNKNOWN";
    }
    memset(&remote_funcs, 0, sizeof(remote_funcs));

    if (debug)
        remote_funcs.debug = debug;
    else
    {
        const char *debugenv = getenv("OCTEON_REMOTE_DEBUG");
        if (debugenv)
            remote_funcs.debug = atoi(debugenv);
    }

    OCTEON_REMOTE_DEBUG_CALLED("\"%s\"", remote_spec);

    for (t=0; t<TIME_MAX; t++)
    {
        timing_data[t].count = 0;
        timing_data[t].total = 0;
        timing_data[t].min = -1;
        timing_data[t].max = 0;
    }

    remote_funcs.lock = default_lock;
    remote_funcs.unlock = default_unlock;
    remote_funcs.reset = default_reset;
    remote_funcs.read_register = default_read_register;
    remote_funcs.write_register = default_write_register;
    remote_funcs.read_csr = octeon_remote_read_mem64;
    remote_funcs.write_csr = octeon_remote_write_mem64;
    remote_funcs.read_csr32 = octeon_remote_read_mem32;
    remote_funcs.write_csr32 = octeon_remote_write_mem32;

    octeon_remote_debug(1, "Remote protocol is %s\n", remote_spec);
    octeon_remote_debug(1, "Remote debug is %d\n", remote_funcs.debug);
    octeon_remote_debug(1, "Color can be disabled by adding 256 to the debug level\n");

    /* Create the default lockfile name based on the remote spec */
    strcpy(lock_file_name, "/var/lock/octeon-remote-lock-");
    strcat(lock_file_name, remote_spec);
    char *ptr = lock_file_name;
    while (*ptr)
    {
        *ptr = tolower(*ptr);
        ptr++;
    }

    if (strncasecmp(remote_spec, "PCIHOST", 7) == 0)
    {
        extern int octeon_remote_pcihost(octeon_remote_funcs_t *remote_funcs);
        result = octeon_remote_pcihost(&remote_funcs);
    }
    else if (strncasecmp(remote_spec, "PCI", 3) == 0)
    {
        extern int octeon_remote_pci(octeon_remote_funcs_t *remote_funcs);
        result = octeon_remote_pci(&remote_funcs);
    }
    else if (strcasecmp(remote_spec, "LINUX") == 0)
    {
        extern int octeon_remote_linux(octeon_remote_funcs_t *remote_funcs);
        result = octeon_remote_linux(&remote_funcs);
    }
    else if (strncasecmp(remote_spec, "GDB:", 4) == 0)
    {
        extern int octeon_remote_gdbremote(octeon_remote_funcs_t *remote_funcs);
        result = octeon_remote_gdbremote(&remote_funcs);
    }
    else
    {
        result = -1;
        octeon_remote_debug(-1, "Illegal Octeon remote protocol\n");
    }

    memset(OCTEON_REMOTE_VALID_COP0, 0, sizeof(OCTEON_REMOTE_VALID_COP0));
    if (result == 0)
    {
        #define UPDATE_VALID_COP0(r,s) OCTEON_REMOTE_VALID_COP0[(r*8+s)/64] |= 1ull << ((r*8+s)%64)

        UPDATE_VALID_COP0(0, 0); /* COP0_INDEX              TLB read/write index */
        UPDATE_VALID_COP0(1, 0); /* COP0_RANDOM             TLB random index */
        UPDATE_VALID_COP0(2, 0); /* COP0_ENTRYLO0           TLB entryLo0 */
        UPDATE_VALID_COP0(3, 0); /* COP0_ENTRYLO1           TLB entryLo1 */
        UPDATE_VALID_COP0(4, 0); /* COP0_CONTEXT            Context */
        UPDATE_VALID_COP0(5, 0); /* COP0_PAGEMASK           TLB pagemask */
        UPDATE_VALID_COP0(5, 1); /* COP0_PAGEGRAIN          TLB config for max page sizes */
        UPDATE_VALID_COP0(6, 0); /* COP0_WIRED              TLB number of wired entries */
        UPDATE_VALID_COP0(7, 0); /* COP0_HWRENA             rdhw instruction enable per register */
        UPDATE_VALID_COP0(8, 0); /* COP0_BADVADDR           Bad virtual address */
        UPDATE_VALID_COP0(9, 0); /* COP0_COUNT              Mips count register */
        UPDATE_VALID_COP0(9, 6); /* COP0_CVMCOUNT           Cavium count register */
        UPDATE_VALID_COP0(9, 7); /* COP0_CVMCTL             Cavium control */
        UPDATE_VALID_COP0(10, 0); /* COP0_ENTRYHI           TLB entryHi */
        UPDATE_VALID_COP0(11, 0); /* COP0_COMPARE           Mips compare register */
        UPDATE_VALID_COP0(11, 7); /* COP0_CVMMEMCTL         Cavium memory control */
        UPDATE_VALID_COP0(12, 0); /* COP0_STATUS            Mips status register */
        UPDATE_VALID_COP0(12, 1); /* COP0_INTCTL            Useless (Vectored interrupts) */
        UPDATE_VALID_COP0(12, 2); /* COP0_SRSCTL            Useless (Shadow registers) */
        UPDATE_VALID_COP0(13, 0); /* COP0_CAUSE             Mips cause register */
        UPDATE_VALID_COP0(14, 0); /* COP0_EPC               Exception program counter */
        UPDATE_VALID_COP0(15, 0); /* COP0_PRID              Processor ID */
        UPDATE_VALID_COP0(15, 1); /* COP0_EBASE             Exception base */
        UPDATE_VALID_COP0(16, 0); /* COP0_CONFIG            Misc config options */
        UPDATE_VALID_COP0(16, 1); /* COP0_CONFIG1           Misc config options */
        UPDATE_VALID_COP0(16, 2); /* COP0_CONFIG2           Misc config options */
        UPDATE_VALID_COP0(16, 3); /* COP0_CONFIG3           Misc config options */
        UPDATE_VALID_COP0(18, 0); /* COP0_WATCHLO0          Address watch registers */
        UPDATE_VALID_COP0(18, 1); /* COP0_WATCHLO1          Address watch registers */
        UPDATE_VALID_COP0(19, 0); /* COP0_WATCHHI0          Address watch registers */
        UPDATE_VALID_COP0(19, 1); /* COP0_WATCHHI1          Address watch registers */
        UPDATE_VALID_COP0(20, 0); /* COP0_XCONTEXT          OS context */
        UPDATE_VALID_COP0(22, 0); /* COP0_MULTICOREDEBUG    Cavium debug */
        UPDATE_VALID_COP0(23, 0); /* COP0_DEBUG             Debug status */
        UPDATE_VALID_COP0(24, 0); /* COP0_DEPC              Debug PC */
        UPDATE_VALID_COP0(25, 0); /* COP0_PERFCONTROL0      Performance counter control */
        UPDATE_VALID_COP0(25, 2); /* COP0_PERFCONTROL1      Performance counter control */
        UPDATE_VALID_COP0(25, 1); /* COP0_PERFVALUE0        Performance counter */
        UPDATE_VALID_COP0(25, 3); /* COP0_PERFVALUE1        Performance counter */
        UPDATE_VALID_COP0(27, 0); /* COP0_CACHEERRI         I cache error status */
        UPDATE_VALID_COP0(27, 1); /* COP0_CACHEERRD         D cache error status */
        UPDATE_VALID_COP0(28, 0); /* COP0_TAGLOI            I cache tagLo */
        UPDATE_VALID_COP0(28, 2); /* COP0_TAGLOD            D cache tagLo */
        UPDATE_VALID_COP0(28, 1); /* COP0_DATALOI           I cache dataLo */
        UPDATE_VALID_COP0(28, 3); /* COP0_DATALOD           D cahce dataLo */
        UPDATE_VALID_COP0(29, 2); /* COP0_TAGHI */
        UPDATE_VALID_COP0(29, 1); /* COP0_DATAHII */
        UPDATE_VALID_COP0(29, 3); /* COP0_DATAHID */
        UPDATE_VALID_COP0(30, 0); /* COP0_ERROREPC          Error PC */
        UPDATE_VALID_COP0(31, 0); /* COP0_DESAVE            Debug scratch area */
    }

    if (result)
    {
        octeon_remote_debug(0, "    Valid protocols for OCTEON_REMOTE_PROTOCOL are:\n");
        octeon_remote_debug(0, "        PCI:<device> - Use PCI/PCIe for <device> (0 is the first Octeon)\n");
        octeon_remote_debug(0, "        PCIHOST - Use PCI/PCIe to connect to the host Octeon from a target\n");
        octeon_remote_debug(0, "        LINUX - Assume Linux userspace native with direct access\n");
        octeon_remote_debug(0, "        GDB:<name>,<tcp_port> - Use GDB remote protocol to communicate with Octeon.\n");
    }
    else
    {
        uint64_t start_time = timing_get_clock();
        result = remote_funcs.open(remote_spec);
        timing_update(TIME_OPEN, start_time);
        if ((result == 0) &&
	    (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_OCTEON3()))
        {
            UPDATE_VALID_COP0(4, 2); /* COP0_USERLOCAL      Octeon2 - User Local */
            UPDATE_VALID_COP0(16, 4); /* COP0_CONFIG4       Octeon2 - Misc config options */
            UPDATE_VALID_COP0(11, 6); /* COP0_POWTHR        Octeon2 - Power Throttle Register */
            UPDATE_VALID_COP0(23, 6); /* COP0_DEBUG2        Octeon2 - Debug2 Complex breakpoints */
            UPDATE_VALID_COP0(31, 2); /* COP0_KSCRATCH1     Octeon2 - Scratch area */
            UPDATE_VALID_COP0(31, 3); /* COP0_KSCRATCH2     Octeon2 - Scratch area */
            UPDATE_VALID_COP0(31, 4); /* COP0_KSCRATCH3     Octeon2 - Scratch area */
        }
        if ((result == 0) &&
	    (OCTEON_IS_OCTEON3()))
        {
            UPDATE_VALID_COP0(31, 4); /* COP0_KSCRATCH4     Octeon3 - Scratch area */
            UPDATE_VALID_COP0(8, 1);  /* COP0_BADINSTR */
            UPDATE_VALID_COP0(8, 2);  /* COP0_BADINSTR0 */
            UPDATE_VALID_COP0(16, 5); /* COP0_CONFIG5 */
            UPDATE_VALID_COP0(22, 2); /* COP0_CVMCOUNTOFFSET */
            UPDATE_VALID_COP0(25, 4); /* COP0_PERFCONTROL2 */
            UPDATE_VALID_COP0(25, 6); /* COP0_PERFCONTROL3 */
            UPDATE_VALID_COP0(25, 5); /* COP0_PERFCOUNT2 */
            UPDATE_VALID_COP0(25, 7); /* COP0_PERFCOUNT3 */
            UPDATE_VALID_COP0(26, 0); /* COP0_ERRCTL */
            UPDATE_VALID_COP0(27, 2); /* COP0_ICACHEDEBUG */
            UPDATE_VALID_COP0(28, 4); /* COP0_TAGLO1 */
	}
    }

    if (result == 0)
    {
        remote_funcs.is_le = octeon_remote_core_is_le(0);
        octeon_remote_debug(1, "Remote host is %s\n", octeon_remote_is_le() ? "LE" : "BE");
        octeon_remote_debug(1, "Remote host memory access is %s\n", octeon_remote_is_mem_le() ? "LE" : "BE");
    }

    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}

/**
 * Close a remote connection created by octeon_remote_open()
 */
void octeon_remote_close(void)
{
    time_index_t t;
    OCTEON_REMOTE_DEBUG_CALLED();
    uint64_t start_time = timing_get_clock();
    remote_funcs.close();
    timing_update(TIME_CLOSE, start_time);

    octeon_remote_debug(2, "%-20s %8s %10s %9s %9s %9s\n", "Function Timing",
                        "Calls", "Total(us)", "Avg(us)", "Min(us)", "Max(us)");
    for (t=0; t<TIME_MAX; t++)
        octeon_remote_debug(2, "%-20s %8llu %10llu %9llu %9llu %9llu\n",
            TIMING_NAMES[t], (ULL)timing_data[t].count, (ULL)timing_data[t].total,
            (ULL)timing_data[t].total / (timing_data[t].count ? (ULL)timing_data[t].count : 1),
            (ULL)(timing_data[t].min != -1ull ? timing_data[t].min : 0), (ULL)timing_data[t].max);

    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Read a CSR from a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
uint64_t octeon_remote_read_csr(uint64_t physical_address)
{
    uint64_t result;
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.read_csr(physical_address);
    timing_update(TIME_READ_CSR, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}


/**
 * Write a CSR on a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
void octeon_remote_write_csr(uint64_t physical_address, uint64_t value)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, 0x%llx", (ULL)physical_address, (ULL)value);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_csr(physical_address, value);
    timing_update(TIME_WRITE_CSR, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Read a 32bit CSR from a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 32bit value of the CSR
 */
uint32_t octeon_remote_read_csr32(uint64_t physical_address)
{
    uint32_t result;
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.read_csr32(physical_address);
    timing_update(TIME_READ_CSR32, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%x", result);
    return result;
}


/**
 * Write a 32bit CSR on a remote Octeon
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
void octeon_remote_write_csr32(uint64_t physical_address, uint32_t value)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, 0x%x", (ULL)physical_address, value);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_csr32(physical_address, value);
    timing_update(TIME_WRITE_CSR32, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Read data from a physical memory address.
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
void octeon_remote_read_mem(void *buffer, uint64_t physical_address, int length)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, %d", (ULL)physical_address, length);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.read_mem(buffer, physical_address, length);
    timing_update(TIME_READ_MEM, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Write data to a physical memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
void octeon_remote_write_mem(uint64_t physical_address, const void *buffer, int length)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, %d", (ULL)physical_address, length);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_mem(physical_address, buffer, length);
    timing_update(TIME_WRITE_MEM, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Lock the remote protocol for exclusive access. This is needed
 * if multiple programs might be accessing the same Octeon at the
 * same time.
 */
void octeon_remote_lock(void)
{
    OCTEON_REMOTE_DEBUG_CALLED();
    if (!lock_count)
    {
        uint64_t start_time = timing_get_clock();
        if (remote_funcs.lock)
            remote_funcs.lock();
        timing_update(TIME_LOCK, start_time);
    }
    lock_count++;
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Unlock the remote protocol for exclusive access. This is needed if multiple
 * programs might be accessing the same Octeon at the same time.
 */
void octeon_remote_unlock(void)
{
    OCTEON_REMOTE_DEBUG_CALLED();
    if (lock_count)
    {
        lock_count--;
        if (lock_count == 0)
        {
            uint64_t start_time = timing_get_clock();
            if (remote_funcs.unlock)
                remote_funcs.unlock();
            timing_update(TIME_UNLOCK, start_time);
        }
    }
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Return the Processor ID of the Octeon. This should be identical
 * to the value found in the COP0 PRID register
 *
 * @return Processor ID
 */
uint32_t octeon_remote_get_model(void)
{
    if (!remote_funcs.model)
    {
        uint64_t start_time = timing_get_clock();
        remote_funcs.model = remote_funcs.get_model();
        timing_update(TIME_GET_MODEL, start_time);
        octeon_remote_debug(1, "Octeon model is 0x%x\n", remote_funcs.model);
    }
    return remote_funcs.model;
}


/**
 * Take the cores in the bit vector out of the debug exception.
 *
 * @param start_mask Cores to start
 */
void octeon_remote_start_cores(uint64_t start_mask)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)start_mask);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.start_cores(start_mask);
    timing_update(TIME_START_CORES, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Cause the cores in the stop mask to take a debug exception
 *
 * @param stop_mask Cores to stop
 */
void octeon_remote_stop_cores(uint64_t stop_mask)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)stop_mask);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.stop_cores(stop_mask);
    timing_update(TIME_STOP_CORES, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Get a mask where each bit represents a running core. If a core
 * is in the debug exception handler, it's bit will be zero.
 *
 * @return Bit set for every running core
 */
uint64_t octeon_remote_get_running_cores(void)
{
    uint64_t result;
    OCTEON_REMOTE_DEBUG_CALLED();
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.get_running_cores();
    timing_update(TIME_GET_RUNNING_CORES, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}

/**
 * Return the number of cores available in the chip
 *
 * @return num of cores on chip
 */
uint32_t octeon_remote_get_num_cores(void)
{

    uint32_t num_cores;
    OCTEON_REMOTE_DEBUG_CALLED();
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    num_cores = remote_funcs.get_num_cores();
    timing_update(TIME_GET_RUNNING_CORES, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("%d", num_cores);
    return num_cores;
}


/**
 * Get all registers, COP0, TLB, etc for a core
 *
 * @param core      Core to get state info for
 * @param registers All of the core's internal state
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_get_core_state(int core, octeon_remote_registers_t *registers)
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED("core=%d, registers=%p", core, registers);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.get_core_state(core, registers);
    timing_update(TIME_GET_CORE_STATE, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}


/**
 * Set all registers, COP0, TLB, etc for a given core
 *
 * @param core      Core to set state for
 * @param registers All date for the core
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_set_core_state(int core, const octeon_remote_registers_t *registers)
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED("core=%d, registers=%p", core, registers);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.set_core_state(core, registers);
    timing_update(TIME_SET_CORE_STATE, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}


/**
 * Read a single Octeon register
 *
 * @param core   Core to read the register from
 * @param reg    Register to read
 *
 * @return Register value
 */
uint64_t octeon_remote_read_register(int core, int reg)
{
    uint64_t result;
    OCTEON_REMOTE_DEBUG_CALLED("core=%d, reg=0x%x", core, reg);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    if (reg == 0)
        result = 0;
    else if ((reg>0) && (reg<34))
        result = remote_funcs.read_register(core, reg);
    else if ((reg>=128) && (reg<256))
        result = remote_funcs.read_register(core, reg);
    else if ((reg>=0x100) && (reg<0x200) && (OCTEON_REMOTE_VALID_COP0[(reg-0x100)/64] & (1ull<<(reg&63))))
        result = remote_funcs.read_register(core, reg);
    else
        result = 0x0bad0bad0bad0badull;
    timing_update(TIME_READ_REGISTER, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}


/**
 * Write a single Octeon register.
 *
 * @param core   Core to write the register on
 * @param reg    Register to write
 * @param value  Value to write
 */
void octeon_remote_write_register(int core, int reg, uint64_t value)
{
    OCTEON_REMOTE_DEBUG_CALLED("core=%d, reg=0x%x, value=0x%llx", core, reg, (ULL)value);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    if ((reg>0) && (reg<34))
        remote_funcs.write_register(core, reg, value);
    else if ((reg>=128) && (reg<256))
        remote_funcs.write_register(core, reg, value);
    else if ((reg>=0x100) && (reg<0x200) && (OCTEON_REMOTE_VALID_COP0[(reg-0x100)/64] & (1ull<<(reg&63))))
        remote_funcs.write_register(core, reg, value);
    timing_update(TIME_WRITE_REGISTER, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Reset a remote Octeon. This performs a full chip soft reset.
 *
 * @param stop_core Attempt to stop core 0 from booting, if possible. If the remote
 *                  protocol supports it, core 0 is stopped before executing
 *                  instructions from flash.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_reset(int stop_core)
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED();
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.reset(stop_core);
    timing_update(TIME_RESET, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
    return result;
}


/**
 * Sample performance / profiling information for a set of cores
 * minimizing the performance impact of taking the sample. The
 * amount of data returned may vary depending on the remote
 * protocol used, but at least the program counter for each core
 * will normally be supported. Values that could not be sampled
 * will be zero.
 *
 * @param coremask Each set bit represent a core that should be sampled.
 * @param sample   Array of samples to populate. Note that the sample array must
 *                 contain elements for cores not set in the coremask. These may
 *                 be filled with sample data even if the coremask bit is clear.
 *                 Some transports may sample all core efficiently and yield
 *                 unwanted core samples.
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_get_sample(uint64_t coremask, octeon_remote_sample_t sample[64])
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED("coremask=0x%llx, sample=%p", (ULL)coremask, sample);
    memset(sample, 0, 64*sizeof(octeon_remote_sample_t));
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    result = remote_funcs.get_sample(coremask, sample);
    timing_update(TIME_GET_SAMPLE, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}


/**
 * Write a 16 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem16(uint64_t physical_address, uint16_t data)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, 0x%x", (ULL)physical_address, (uint32_t)data);
    octeon_remote_lock();
    if (octeon_remote_is_mem_le())
        data = cvmx_cpu_to_le16(data);
    else
        data = cvmx_cpu_to_be16(data);
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_mem(physical_address, &data, sizeof(data));
    timing_update(TIME_WRITE_MEM, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem32(uint64_t physical_address, uint32_t data)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, 0x%x", (ULL)physical_address, data);
    octeon_remote_lock();
    if (octeon_remote_is_mem_le())
        data = cvmx_cpu_to_le32(data);
    else
        data = cvmx_cpu_to_be32(data);
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_mem(physical_address, &data, sizeof(data));
    timing_update(TIME_WRITE_MEM, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Write a 64 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 * @param data   Data to write
 */
void octeon_remote_write_mem64(uint64_t physical_address, uint64_t data)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, 0x%llx", (ULL)physical_address, (ULL)data);
    octeon_remote_lock();
    if (octeon_remote_is_mem_le())
        data = cvmx_cpu_to_le64(data);
    else
        data = cvmx_cpu_to_be64(data);
    uint64_t start_time = timing_get_clock();
    remote_funcs.write_mem(physical_address, &data, sizeof(data));
    timing_update(TIME_WRITE_MEM, start_time);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED();
}


/**
 * Read a 16 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint16_t octeon_remote_read_mem16(uint64_t physical_address)
{
    uint16_t result;
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.read_mem(&result, physical_address, sizeof(result));
    timing_update(TIME_READ_MEM, start_time);
    if (octeon_remote_is_mem_le())
        result = cvmx_le16_to_cpu(result);
    else
        result = cvmx_be16_to_cpu(result);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%x", result);
    return result;
}


/**
 * Read a 32 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint32_t octeon_remote_read_mem32(uint64_t physical_address)
{
    uint32_t result;
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.read_mem(&result, physical_address, sizeof(result));
    timing_update(TIME_READ_MEM, start_time);
    if (octeon_remote_is_mem_le())
        result = cvmx_le32_to_cpu(result);
    else
        result = cvmx_be32_to_cpu(result);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%x", result);
    return result;
}


/**
 * Read a 64 bit value from Octeon's memory. This code automatically correct for
 * differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to read from. Bits 63-38 should be zero.
 *
 * @return Data read
 */
uint64_t octeon_remote_read_mem64(uint64_t physical_address)
{
    uint64_t result;
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
    octeon_remote_lock();
    uint64_t start_time = timing_get_clock();
    remote_funcs.read_mem(&result, physical_address, sizeof(result));
    timing_update(TIME_READ_MEM, start_time);
    if (octeon_remote_is_mem_le())
        result = cvmx_le64_to_cpu(result);
    else
        result = cvmx_be64_to_cpu(result);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}


/**
 * Send a string to u-boot as a command.
 *
 * @param cmd_str String to send
 * @param wait_hundredths
 *                Time to wait for u-boot to accept the command.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_send_bootcmd(const char *cmd_str, int wait_hundredths)
{
    OCTEON_REMOTE_DEBUG_CALLED("\"%s\", %d", cmd_str, wait_hundredths);
    int len = strlen(cmd_str);
    if (len > BOOTLOADER_PCI_WRITE_BUFFER_STR_LEN - 1)
    {
        octeon_remote_debug(-1, "Command string too long, max length is: %d\n", BOOTLOADER_PCI_WRITE_BUFFER_STR_LEN - 1);
        OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
        return -1;
    }

    if (octeon_remote_wait_for_bootloader(wait_hundredths) != 0)
    {
        octeon_remote_debug(-1, "Bootloader not ready for command.\n");
        OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
        return -1;
    }

    /* Write command to bootloader */
    octeon_remote_lock();
    octeon_remote_write_mem(BOOTLOADER_PCI_READ_BUFFER_DATA_ADDR, cmd_str, len);
    octeon_remote_write_mem32(BOOTLOADER_PCI_READ_BUFFER_LEN_ADDR, len);
    octeon_remote_write_mem32(BOOTLOADER_PCI_READ_BUFFER_OWNER_ADDR, OCTEON_PCI_IO_BUF_OWNER_OCTEON);

    /* Bootloader should accept command very quickly if it really was ready */
    if (octeon_remote_wait_for_bootloader(200) != 0)
    {
        octeon_remote_unlock();
        octeon_remote_debug(-1, "Bootloader did not accept command.\n");
        OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
        return -1;
    }
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("%d", 0);
    return 0;
}


/**
 * Wait for u-boot to boot and be waiting for a command.
 *
 * @param wait_time_hundredths
 *               Maximum time to wait
 *
 * @return Zero on success, negative on failure.
 */
int octeon_remote_wait_for_bootloader(int wait_time_hundredths)
{
    OCTEON_REMOTE_DEBUG_CALLED("%d", wait_time_hundredths);
    if (!octeon_remote_mem_access_ok())
    {
        OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
        return -1;
    }

    while (wait_time_hundredths > 0 && octeon_remote_read_mem32(BOOTLOADER_PCI_READ_BUFFER_OWNER_ADDR) != OCTEON_PCI_IO_BUF_OWNER_HOST)
    {
	if (--wait_time_hundredths <= 0)
        {
            OCTEON_REMOTE_DEBUG_RETURNED("%d", -1);
            return -1;
        }
	usleep(500000);
    }
    OCTEON_REMOTE_DEBUG_RETURNED("%d", 0);
    return 0;
}


/**
 * Find a named block on the remote Octeon
 *
 * @param name      Name of block to find
 * @param base_addr Address the block is at (OUTPUT)
 * @param size      The size of the block (OUTPUT)
 *
 * @return One on success, zero on failure.
 */
int octeon_remote_named_block_find(const char *name, uint64_t *base_addr, uint64_t *size)
{
    OCTEON_REMOTE_DEBUG_CALLED("\"%s\"", name);

    octeon_remote_lock();
    const cvmx_bootmem_named_block_desc_t *named_block = cvmx_bootmem_find_named_block(name);
    octeon_remote_unlock();
    if (named_block)
    {
        *base_addr = named_block->base_addr;
        *size = named_block->size;
        OCTEON_REMOTE_DEBUG_RETURNED("%d - success, address=0x%llx, size=0x%llx", 1, (ULL)named_block->base_addr, (ULL)named_block->size);
        return 1;
    }
    OCTEON_REMOTE_DEBUG_RETURNED("%d - fail", 0);
    return 0;
}

/**
 * Allocate a named block on the remote Octeon
 *
 * @param size      Size of the block in bytes
 * @param min_addr  Minimum address the block can be located at. Zero is a good default.
 * @param max_addr  Maximum upper address of the block. Use 0 if you don't care.
 * @param alignment Byte alignment for the block. Must be 0 or a power of two.
 * @param name      Name to give the new block
 *
 * @return Address of the named block or 0xffffffffffffffff on failure.
 */
int64_t octeon_remote_named_block_alloc(uint64_t size, uint64_t min_addr, uint64_t max_addr, uint64_t alignment, const char *name)
{
    int64_t result;
    OCTEON_REMOTE_DEBUG_CALLED("size=0x%llx, min_addr=0x%llx, max_addr=0x%llx, alignment=0x%llx, name=\"%s\"",
        (ULL)size, (ULL)min_addr, (ULL)max_addr, (ULL)alignment, name);
    octeon_remote_lock();
    result = cvmx_bootmem_phy_named_block_alloc(size, min_addr, max_addr, alignment, name, 0);
    octeon_remote_unlock();
    OCTEON_REMOTE_DEBUG_RETURNED("0x%llx", (ULL)result);
    return result;
}


/**
 * Return non-zero of Octeon's memory can be accessed.
 *
 * @return True if memory is configured.
 */
int octeon_remote_mem_access_ok(void)
{
    int result;
    OCTEON_REMOTE_DEBUG_CALLED();
    if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN58XX) || OCTEON_IS_MODEL(OCTEON_CN50XX))
    {
        cvmx_lmcx_mem_cfg0_t lmcx_mem_cfg0;
        lmcx_mem_cfg0.u64 = octeon_remote_read_csr(CVMX_LMCX_MEM_CFG0(0));
        /* This code assumes that we will never program the refresh interval
            to 512 cycles (the reset value). All setups should use a higher
            value */
        result = (lmcx_mem_cfg0.s.ref_int != 1);
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN56XX) || OCTEON_IS_MODEL(OCTEON_CN52XX))
    {
        /* Check to make sure a DDR interface is enabled */
        cvmx_l2c_cfg_t l2c_cfg;
        l2c_cfg.u64 = octeon_remote_read_csr(CVMX_L2C_CFG);
        result = (l2c_cfg.s.dpres0 || l2c_cfg.s.dpres1);
    }
    else
    {
        /* Check to make sure a DDR interface is enabled */
        cvmx_lmcx_reset_ctl_t lmcx_reset_ctl;
        lmcx_reset_ctl.u64 = octeon_remote_read_csr(CVMX_LMCX_RESET_CTL(0));
        result = lmcx_reset_ctl.s.ddr3rst;
    }
    OCTEON_REMOTE_DEBUG_RETURNED("%d", result);
    return result;
}


/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 */
void octeon_remote_spinlock_lock(uint64_t physical_address)
{
	uint64_t start_time;

	OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
	octeon_remote_lock();
	start_time = timing_get_clock();

	pci_spinlock_lock(physical_address);

	timing_update(TIME_WRITE_MEM, start_time);
	octeon_remote_unlock();
	OCTEON_REMOTE_DEBUG_RETURNED();
}

/**
 * Write a 32 bit value into Octeon's memory. This code automatically correct
 * for differences between Octeon's endianness and where we are running.
 *
 * @param physical_address
 *               Physical address to write to. Bits 63-38 should be zero.
 */
void octeon_remote_spinlock_unlock(uint64_t physical_address)
{
	uint64_t start_time;

	OCTEON_REMOTE_DEBUG_CALLED("0x%llx", (ULL)physical_address);
	octeon_remote_lock();
	start_time = timing_get_clock();

	pci_spinlock_unlock(physical_address);

	timing_update(TIME_WRITE_MEM, start_time);
	octeon_remote_unlock();
	OCTEON_REMOTE_DEBUG_RETURNED();
}

