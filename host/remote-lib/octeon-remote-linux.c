/***********************license start************************************
 * Copyright (c) 2003-2009 Cavium Inc. (support@cavium.com). All rights
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
 * @file
 *
 * Interface to Octeon natively under Linux
 *
 * $Id: octeon-remote-linux.c 127530 2015-11-03 22:01:29Z cchavva $
 */
#include <stdint.h>
#include "octeon-remote.h"
#include "octeon-remote-map.h"
#include "octeon-remote-debug-handler.h"
#include "cvmx.h"
#include <errno.h>

/* The linux userspace transport is only supported when we are building on
    Octeon */
#ifdef __mips__
#include <sys/sysmips.h>

#define MIPS_CAVIUM_XKPHYS_WRITE	2011	/* XKPHYS */

/**
 * Called when octeon_remote_open() is called using a remote spec
 * for LINUX.
 *
 * @param remote_spec
 *               String specification of the remote to connect to.
 *
 * @return Zero on success, negative on failure.
 */
static int linux_open(const char *remote_spec __attribute__ ((unused)))
{
    /* There is nothing as we are on the same board. */
    return 0;
}


/**
 * Called when octeon_remote_close() is called.
 */
static void linux_close(void)
{
    octeon_remote_debug_handler_remove();
}


/**
 * Read a 64bit CSR by mmapping the register address
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
static uint64_t linux_read_csr_mmap(uint64_t physical_address)
{
    uint64_t result;
    octeon_remote_map_cookie_t cookie;
    uint64_t *ptr = octeon_remote_map(physical_address, 8, &cookie);
    result = *ptr;
    octeon_remote_unmap(&cookie);
    return result;
}


/**
 * Read a 64bit CSR via the xkphys segment
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 64bit value of the CSR
 */
static uint64_t linux_read_csr_xkphys(uint64_t physical_address)
{
    uint64_t result;
    uint64_t *ptr = (uint64_t *)(physical_address | (1ull << 63));
    result = *ptr;
    return result;
}


/**
 * Write a 64bit CSR by mmapping the register address
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void linux_write_csr_mmap(uint64_t physical_address, uint64_t value)
{
    octeon_remote_map_cookie_t cookie;
    uint64_t *ptr = octeon_remote_map(physical_address, 8, &cookie);
    *ptr = value;
    octeon_remote_unmap(&cookie);
}


/**
 * Write a 64bit CSR via the xkphys segment
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void linux_write_csr_xkphys(uint64_t physical_address, uint64_t value)
{
    uint64_t *ptr = (uint64_t *)(physical_address | (1ull << 63));
    *ptr = value;
}


/**
 * Read a 32bit CSR by mmapping the register address
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 32bit value of the CSR
 */
static uint32_t linux_read_csr32_mmap(uint64_t physical_address)
{
    uint32_t result;
    octeon_remote_map_cookie_t cookie;
    uint32_t *ptr = octeon_remote_map(physical_address, 4, &cookie);
    result = *ptr;
    octeon_remote_unmap(&cookie);
    return result;
}


/**
 * Read a 32bit CSR via the xkphys segment
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 *
 * @return 32bit value of the CSR
 */
static uint32_t linux_read_csr32_xkphys(uint64_t physical_address)
{
    uint32_t result;
    uint32_t *ptr = (uint32_t *)(physical_address | (1ull << 63));
    result = *ptr;
    return result;
}


/**
 * Write a 32bit CSR by mmapping the register address
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void linux_write_csr32_mmap(uint64_t physical_address, uint32_t value)
{
    octeon_remote_map_cookie_t cookie;
    uint32_t *ptr = octeon_remote_map(physical_address, 4, &cookie);
    *ptr = value;
    octeon_remote_unmap(&cookie);
}


/**
 * Write a 32bit CSR via the xkphys segment
 *
 * @param physical_address
 *               Physical address of the CSR. Bits 63-49 should be zero.
 * @param value  Value to write
 */
static void linux_write_csr32_xkphys(uint64_t physical_address, uint32_t value)
{

    uint32_t *ptr = (uint32_t *)(physical_address | (1ull << 63));
    *ptr = value;
}


/**
 * Read data from a physical memory address by mmapping the memory address
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
static void linux_read_mem_mmap(void *buffer, uint64_t physical_address,
                                int length)
{
    octeon_remote_map_cookie_t cookie;
    uint8_t *ptr = octeon_remote_map(physical_address, length, &cookie);
    /* Special case some common sizes as IO blocks may only allow certain
        size accesses. Unfortunately memcpy() seem to do silly things, like
        byte copies, for the most common operations */
    if (length == 1)
        *(uint8_t *)buffer = *(uint8_t *)ptr;
    else if (length == 2)
        *(uint16_t *)buffer = *(uint16_t *)ptr;
    else if (length == 4)
        *(uint32_t *)buffer = *(uint32_t *)ptr;
    else if (length == 8)
        *(uint64_t *)buffer = *(uint64_t *)ptr;
    else
        memcpy(buffer,  ptr,  length);
    octeon_remote_unmap(&cookie);
}


/**
 * Read data from a physical memory address via the xkphys segment
 *
 * @param buffer_ptr Buffer to put the data in
 * @param physical_address
 *                   Physical address to read from. Bits 63-38 should be zero.
 * @param length     Length to read in bytes
 */
static void linux_read_mem_xkphys(void *buffer, uint64_t physical_address,
                                  int length)
{
    uint8_t *ptr = (uint8_t *)(physical_address | (1ull << 63));
    /* Special case some common sizes as IO blocks may only allow certain
        size accesses. Unfortunately memcpy() seem to do silly things, like
        byte copies, for the most common operations */
    if (length == 1)
        *(uint8_t *)buffer = *(uint8_t *)ptr;
    else if (length == 2)
        *(uint16_t *)buffer = *(uint16_t *)ptr;
    else if (length == 4)
        *(uint32_t *)buffer = *(uint32_t *)ptr;
    else if (length == 8)
        *(uint64_t *)buffer = *(uint64_t *)ptr;
    else
        memcpy(buffer,  ptr,  length);
}


/**
 * Write data to a physical memory address by mmapping the memory address
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
static void linux_write_mem_mmap(uint64_t physical_address, const void *buffer,
                                 int length)
{
    octeon_remote_map_cookie_t cookie;
    uint8_t *ptr = octeon_remote_map(physical_address, length, &cookie);
    /* Special case some common sizes as IO blocks may only allow certain
        size accesses. Unfortunately memcpy() seem to do silly things, like
        byte copies, for the most common operations */
    if (length == 1)
        *(uint8_t *)ptr = *(uint8_t *)buffer;
    else if (length == 2)
        *(uint16_t *)ptr = *(uint16_t *)buffer;
    else if (length == 4)
        *(uint32_t *)ptr = *(uint32_t *)buffer;
    else if (length == 8)
        *(uint64_t *)ptr = *(uint64_t *)buffer;
    else
        memcpy(ptr,  buffer,  length);
    octeon_remote_unmap(&cookie);
}


/**
 * Write data to a physical memory address by via the xkphys segment
 *
 * @param physical_address
 *                   Physical address to write to. Bits 63-38 should be zero.
 * @param buffer_ptr Buffer containing the data to write
 * @param length     Number of bytes to write
 */
static void linux_write_mem_xkphys(uint64_t physical_address, const void *buffer,
                                 int length)
{
    uint8_t *ptr = (uint8_t *)(physical_address | (1ull << 63));
    /* Special case some common sizes as IO blocks may only allow certain
        size accesses. Unfortunately memcpy() seem to do silly things, like
        byte copies, for the most common operations */
    if (length == 1)
        *(uint8_t *)ptr = *(uint8_t *)buffer;
    else if (length == 2)
        *(uint16_t *)ptr = *(uint16_t *)buffer;
    else if (length == 4)
        *(uint32_t *)ptr = *(uint32_t *)buffer;
    else if (length == 8)
        *(uint64_t *)ptr = *(uint64_t *)buffer;
    else
        memcpy(ptr,  buffer,  length);
}


/**
 * Return the Processor ID of the Octeon. This should be identical
 * to the value found in the COP0 PRID register
 *
 * @return Processor ID
 */
uint32_t octeon_remote_linux_get_model(void)
{
    uint32_t result = 0;

    FILE *infile = fopen("/proc/octeon_info", "r");
    if (!infile)
    {
        octeon_remote_debug(-1, "Error opening /proc/octeon_info");
        return 0;
    }

    while (!feof(infile))
    {
        char buffer[80];
        if (fgets(buffer, sizeof(buffer), infile))
        {
            octeon_remote_debug(2, "/proc/octeon_info: %s", buffer);
            const char *field = strtok(buffer, " ");
            const char *valueS = strtok(NULL, " ");
            if (!field || !valueS)
                continue;
            if (strcmp(field, "processor_id:") == 0)
            {
                sscanf(valueS, "%i", &result);
                break;
            }
        }
    }
    fclose(infile);
    return result;
}


/**
 * Take the cores in the bit vector out of the debug exception.
 *
 * @param start_mask Cores to start
 */
static void linux_start_cores(uint64_t start_mask __attribute__ ((unused)))
{
    /* We only support the profiling debug handler, so cores automatically
        restart */
}


/**
 * Cause the cores in the stop mask to take a debug exception
 *
 * @param stop_mask Cores to stop
 */
static void linux_stop_cores(uint64_t stop_mask)
{
    if (octeon_remote_debug_handler_install(OCTEON_REMOTE_SAVE_HANDLER, 0))
        return;
    octeon_remote_write_csr(CVMX_CIU_DINT, stop_mask);
}


/**
 * Get a mask where each bit represents a running core. If a core
 * is in the debug exception handler, it's bit will be zero.
 *
 * @return Bit set for every running core
 */
static uint64_t linux_get_running_cores(void)
{
    return ~octeon_remote_read_csr(CVMX_CIU_PP_DBG);
}


/**
 * Get all registers, COP0, TLB, etc for a core
 *
 * @param core      Core to get state info for
 * @param registers All of the core's internal state
 *
 * @return Zero on success, negative on failure
 */
static int linux_get_core_state(int core __attribute__ ((unused)), octeon_remote_registers_t *registers __attribute__ ((unused)))
{
    uint64_t base = octeon_remote_debug_handler_get_base(core);
    if (!base)
        return -1;
    octeon_remote_read_mem(registers, base, sizeof(*registers));
    registers->regs[0][0] = 0;
    return 0;
}


/**
 * Set all registers, COP0, TLB, etc for a given core
 *
 * @param core      Core to set state for
 * @param registers All date for the core
 *
 * @return Zero on success, negative on failure
 */
static int linux_set_core_state(int core __attribute__ ((unused)), const octeon_remote_registers_t *registers __attribute__ ((unused)))
{
    /* The profile debug handler doesn't support setting any registers */
    return -1;
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
static int linux_get_sample(uint64_t coremask, octeon_remote_sample_t sample[64])
{
    int core;

    if (octeon_remote_debug_handler_install(OCTEON_REMOTE_PROFILE_HANDLER, 0))
        return -1;

    /* Pulse the debug interrupt */
    octeon_remote_write_csr(CVMX_CIU_DINT, coremask);
    /* Give the cores time to do their sample */
    usleep(1);
    /* Read the samples */
    for (core=0; core<64; core++)
    {
        if ((1ull<<core) & coremask)
        {
            uint64_t base = octeon_remote_debug_handler_get_base(core);
            if (!base)
                return -1;
            sample[core].pc = octeon_remote_read_mem64(base + 8*(256 + 24*8 + 0));
            sample[core].perf_count[0] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 1));
            sample[core].perf_count[1] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 3));
            /* Octeon III has 4 performance counters */
            if (octeon_remote_get_model() >= OCTEON_CN78XX_PASS1_0)
            {
                sample[core].perf_count[2] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 5));
                sample[core].perf_count[3] = octeon_remote_read_mem64(base + 8*(256 + 25*8 + 7));
            }
        }
    }
    return 0;
}

/*
 * Enable access to XKPHYS segments. Warning message is printed in case of
 * error.
 */
static inline int cvmx_linux_enable_xkphys_access(void)
{
	int	ret;
	ret = sysmips(MIPS_CAVIUM_XKPHYS_WRITE, getpid(), 3, 0);
	if (ret) {
		switch (errno) {
		case EINVAL:
			perror("sysmips(MIPS_CAVIUM_XKPHYS_WRITE) failed.\n"
			       "  Did you configure your kernel with both:\n"
			       "     CONFIG_CAVIUM_OCTEON_USER_MEM_PER_PROCESS *and*\n"
			       "     CONFIG_CAVIUM_OCTEON_USER_IO_PER_PROCESS?");
			break;
		case EPERM:
			perror("sysmips(MIPS_CAVIUM_XKPHYS_WRITE) failed.\n"
			       "  Are you running as root?");
			break;
		default:
			perror("sysmips(MIPS_CAVIUM_XKPHYS_WRITE) failed");
			break;
		}
	}

	return ret;
}

#endif


/**
 * Initialize the pointers needed for octeon_remote to work over
 * LINUX. This doesn't actually create a connection, just setup
 * internal data structures.
 *
 * @param remote_funcs
 *               Function pointers to be populated
 *
 * @return Zero on success, negative on failure
 */
int octeon_remote_linux(octeon_remote_funcs_t *remote_funcs)
{
#ifdef __mips__
    int ret = 0;
    int use_xkphys = 0;

    /*
     * Applications running in 64-bit mode can access the register space
     * and/or memory directly via the xkphys segment, thus improving
     * performance.
     */
    if (sizeof(void *) == 8) {
        ret = cvmx_linux_enable_xkphys_access();
        use_xkphys = !ret;
    }

    if (use_xkphys) {
        remote_funcs->read_csr = linux_read_csr_xkphys;
        remote_funcs->write_csr = linux_write_csr_xkphys;
        remote_funcs->read_csr32 = linux_read_csr32_xkphys;
        remote_funcs->write_csr32 = linux_write_csr32_xkphys;
        remote_funcs->read_mem = linux_read_mem_xkphys;
        remote_funcs->write_mem = linux_write_mem_xkphys;
    }
    else {
        remote_funcs->read_csr = linux_read_csr_mmap;
        remote_funcs->write_csr = linux_write_csr_mmap;
        remote_funcs->read_csr32 = linux_read_csr32_mmap;
        remote_funcs->write_csr32 = linux_write_csr32_mmap;
        remote_funcs->read_mem = linux_read_mem_mmap;
        remote_funcs->write_mem = linux_write_mem_mmap;
    }

    remote_funcs->open = linux_open;
    remote_funcs->close = linux_close;
    remote_funcs->get_model = octeon_remote_linux_get_model;
    remote_funcs->start_cores = linux_start_cores;
    remote_funcs->stop_cores = linux_stop_cores;
    remote_funcs->get_running_cores = linux_get_running_cores;
    remote_funcs->get_core_state = linux_get_core_state;
    remote_funcs->set_core_state = linux_set_core_state;
    remote_funcs->get_sample = linux_get_sample;
    remote_funcs->lock = NULL;
    remote_funcs->unlock = NULL;
    remote_funcs->memory_le = 1;
    return 0;
#else
    octeon_remote_debug(-1, "LINUX remote protocol is only supported when running natively on Octeon\n");
    remote_funcs->open = NULL;
    return -1;
#endif
}

