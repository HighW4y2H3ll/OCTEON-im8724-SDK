/***********************license start************************************
 * Copyright (c) 2004-2007 Cavium Inc. (support@cavium.com). All rights
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
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
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
 ***********************license end**************************************/


#ifdef __U_BOOT__
#include <common.h>
#include <asm/arch/cvmx.h>
#include <octeon_boot.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/cvmx-gpio.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cvmx.h"
#include "cvmx-gpio.h"
#include "lib_octeon_shared.h"
#endif


/* Define DFM_DEBUG to debug the DFM interface.  This also enables the
** output necessary for review by Cavium Inc., Inc. */
//#define DFM_DEBUG


/* Make a define similar to OCTEON_IS_MODEL that will work in this context.  This is runtime only.
** We can't do the normal runtime here since this code can run on the host.
** All functions using this must have a cpu_id variable set to the correct value */
#define octeon_is_cpuid(x)   (__OCTEON_IS_MODEL_COMPILE__(x, cpu_id))


typedef struct {
    uint8_t odt_ena;
    uint64_t odt_mask;
    cvmx_lmcx_modereg_params1_t odt_mask1;
    uint8_t qs_dic;
    uint64_t rodt_ctl;
    uint8_t dic;
} dfm_odt_config_t;

#if OCTEON_CONFIG_DFM

dfm_odt_config_t dfm_odt_config[2] = { OCTEON_CN63XX_DFM_ODT_CONFIGURATION };

int initialize_dfm_clock(uint32_t cpu_id, uint32_t cpu_hertz, uint64_t dfm_hertz);
uint32_t measure_octeon_dfm_clock(uint32_t cpu_id, uint32_t cpu_hertz, uint64_t dfm_hertz);

#ifdef __U_BOOT__

DECLARE_GLOBAL_DATA_PTR;


static inline int dfm_verbose(void)
{
    return (!!(gd->flags & GD_FLG_DFM_VERBOSE));
}
static inline int dfm_trace_init(void)
{
    return (!!(gd->flags & GD_FLG_DFM_TRACE_INIT));
}


static inline char *dfm_getenv_debug(char *name)
{

    if (gd->flags & GD_FLG_FAILSAFE_MODE)
        return ((char *) 0);

    /* Require dfm_verbose to enable dfm_getenv_debug */
    if (gd->flags & GD_FLG_DFM_VERBOSE)
        return getenv(name);

    return ((char *) 0);
}
#else
static int dfm_verbose(void)
{
    return(getenv("dfm_verbose") != NULL);
}

static int dfm_trace_init(void)
{
    return(getenv("dfm_trace_init") != NULL);
}

static inline char *dfm_getenv_debug(char *name)
{
    return getenv(name);
}
#endif


#ifdef DEBUG
#define DEBUG_DEFINED (1)
#define debug_print  printf
#else
#define DEBUG_DEFINED (0)
#define debug_print(...)
#endif

static void dfm_print(const char *format, ...)
{
    if (dfm_verbose() || (DEBUG_DEFINED))
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

static inline void dfm_config_write_csr(uint64_t csr_addr, uint64_t val)
{
    if (dfm_trace_init()) {
        dfm_print("DFM Config %016llx => %016llx\n", csr_addr, val);
        //cvmx_csr_db_decode(cvmx_get_proc_id(), csr_addr, val);
    }
    cvmx_write_csr(csr_addr, val);
}

#define error_print  printf


extern void octeon_delay_cycles(uint64_t cycles);

#undef min
#define min(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x < __y) ? __x : __y; })

#undef max
#define max(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x > __y) ? __x : __y; })





#ifdef CVMX_BUILD_FOR_UBOOT
static void pulse_gpio_pin(int pin, int usecs)
{
    cvmx_gpio_cfg(pin, 1);
    cvmx_gpio_clear((1ull << pin));
    cvmx_wait_usec(usecs);
    cvmx_gpio_set((1ull << pin));
}
#else
static void pulse_gpio_pin(int pin, int usecs)
{
}
#endif


/* Absolute value of an integer */
static int64_t _abs(int64_t v)
{
    return ((v < 0) ? -v : v);
}

/* Sign of an integer */
static int64_t _sign(int64_t v)
{
    return (v < 0);
}



static int encode_row_lsb_ddr3(uint32_t cpu_id, int row_lsb)
{
    int encoded_row_lsb;
    int row_lsb_start = 14;

    /*  Decoding for row_lsb             */
    /*       000: row_lsb = mem_adr[14]  */
    /*       001: row_lsb = mem_adr[15]  */
    /*       010: row_lsb = mem_adr[16]  */
    /*       011: row_lsb = mem_adr[17]  */
    /*       100: row_lsb = mem_adr[18]  */
    /*       101: row_lsb = mem_adr[19]  */
    /*       110: row_lsb = mem_adr[20]  */
    /*       111: RESERVED               */

    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX))
        row_lsb_start = 11;
    else 
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    encoded_row_lsb      = row_lsb - row_lsb_start ;

    return encoded_row_lsb;
}

static int encode_pbank_lsb_ddr3(uint32_t cpu_id, int pbank_lsb)
{
    int encoded_pbank_lsb;

    /*  Decoding for pbank_lsb        */
    /*  PBANK_LSB                     */                           
    /*       0: rank = mem_adr[24]    */
    /*       1: rank = mem_adr[25]    */
    /*       2: rank = mem_adr[26]    */
    /*       3: rank = mem_adr[27]    */
    /*       4: rank = mem_adr[28]    */
    /*       5: rank = mem_adr[29]    */
    /*       6: rank = mem_adr[30]    */
    /*       7: rank = mem_adr[31]    */
    /*    8-15:  RESERVED             */

    int pbank_lsb_start = 0;
    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX))
        pbank_lsb_start = 25;
    else 
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    encoded_pbank_lsb      = pbank_lsb - pbank_lsb_start;

    return encoded_pbank_lsb;
}


static uint64_t octeon_read_dfm_ddr3_rlevel_dbg(int idx)
{
    cvmx_dfm_rlevel_dbg_t rlevel_dbg;
    cvmx_dfm_rlevel_ctl_t rlevel_ctl;

    rlevel_ctl.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_CTL);
    rlevel_ctl.s.byte = idx;

    dfm_config_write_csr(CVMX_DFM_RLEVEL_CTL, rlevel_ctl.u64);
    cvmx_read_csr(CVMX_DFM_RLEVEL_CTL);

    rlevel_dbg.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_DBG);
    return rlevel_dbg.s.bitmask;
}

static uint64_t octeon_read_dfm_ddr3_wlevel_dbg(int idx)
{
    cvmx_dfm_wlevel_dbg_t wlevel_dbg;

    wlevel_dbg.u64 = 0;
    wlevel_dbg.s.byte = idx;

    dfm_config_write_csr(CVMX_DFM_WLEVEL_DBG, wlevel_dbg.u64);
    cvmx_read_csr(CVMX_DFM_WLEVEL_DBG);

    wlevel_dbg.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_DBG);
    return wlevel_dbg.s.bitmask;
}


#define DEBUG_VALIDATE_BITMASK 0
#if DEBUG_VALIDATE_BITMASK
#define debug_bitmask_print printf
#else
#define debug_bitmask_print(...)
#endif

#define RLEVEL_BITMASK_INVALID_BITS_ERROR       1
#define RLEVEL_BITMASK_BUBBLE_BITS_ERROR        2
#define RLEVEL_BITMASK_BLANK_ERROR              9
#define RLEVEL_NONSEQUENTIAL_DELAY_ERROR       10
#define RLEVEL_UNDEFINED_ERROR            9999999

static int validate_ddr3_rlevel_bitmask(int bitmask, ulong tclk_psecs)
{
    int mask;
    int shifted_bitmask;
    int i, j, leader;
    int errors = 0;

    mask = 0x7f;
    if (tclk_psecs < 1875)
        mask = 0x3f;

    for (leader=0; leader<32; ++leader) {
        shifted_bitmask = (bitmask>>leader);
        if (shifted_bitmask&1)
            break;
    }

    /*
    ** Only accept a mask within 0x7f with contiguous bits set.  Valid
    ** masks are 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01.  Anything else
    ** fails.
    */
    i =0;
    while (mask>>i) {
        debug_bitmask_print("\nbitmask:%05x shifted:%05x leader:%2d", bitmask, shifted_bitmask, leader);

        /* Nothing more to do if bitmask is blank. */
        if (bitmask == 0) {
            errors += RLEVEL_BITMASK_BLANK_ERROR;
            debug_bitmask_print(" %d", errors);
            break;              /* Bail out here */
        }

        /* Detect extra invalid bits at the trailing end of the bitmask. */
        if ((~mask&shifted_bitmask) != 0) {
            errors += RLEVEL_BITMASK_INVALID_BITS_ERROR;
            debug_bitmask_print(" %d", errors);
        }

        /* Detect bubbles in the bitmask */
        j = shifted_bitmask&mask;
        while(j) {
            if ((j&1) == 0) {
                errors += RLEVEL_BITMASK_BUBBLE_BITS_ERROR;
                debug_bitmask_print(" %d", errors);
                break;          /* Bubble detected */
            }
            j>>=1;
        }

        /* Accept bitmask that equals valid mask */
        if ((mask&shifted_bitmask) == mask) {
            break;
        }

        if (errors) {
            break;
        }

        ++i;
    }

    if (errors) {
        debug_bitmask_print(" => invalid");
    }
    debug_bitmask_print("\n");
    return errors;
}


static int test_dfm_byte(uint64_t p, int count, int stride, int byte)
{
    uint64_t p1, p2, d1, d2;
    int errors = 0;
    int i;
    uint64_t v;
    int step = (stride<16) ? 16 : stride; /* Address 3:0 = must be zero */

    p |= 1ull<<63;

    uint64_t p2base = ((((p+count*8)+0x100000)>>20)<<20); /* Round up to next MB boundary */

    p1 = p;
    p2 = p2base;
    for (i=0; i<count; ++i) {
        v = (~((i*i)%256)&0xff)<<(8*byte);
        cvmx_write64_uint64(p1, v);
        cvmx_write64_uint64(p2, ~v);
        p1 += step;
        p2 += step;
    }

    p1 = p;
    p2 = p2base;
    for (i=0; i<count; ++i) {
        v = (~((i*i)%256)&0xff)<<(8*byte);
        d1 = cvmx_read64_uint64(p1) & (0xFFULL << 8*byte);
        d2 = ~cvmx_read64_uint64(p2) & (0xFFULL << 8*byte);

#if 0
        debug_print("%d: [0x%016llX] 0x%016llX expected 0x%016llX xor %016llX\n",
                    errors, p1, d1, v, (d1 ^ v));
        debug_print("%d: [0x%016llX] 0x%016llX expected 0x%016llX xor %016llX\n",
                    errors, p2, d2, v, (d2 ^ v));
#endif

        if (d1 != v) {
            ++errors;
            debug_print("%d: [0x%016llX] 0x%016llX expected 0x%016llX xor %016llX\n",
                        errors, p1, d1, v, (d1 ^ v));
            if (errors > 5)
                return errors;
        }
        if (d2 != v) {
            ++errors;            
            debug_print("%d: [0x%016llX] 0x%016llX expected 0x%016llX xor %016llX\n",
                        errors, p2, d2, v, (d2 ^ v));
            if (errors > 2)
                return errors;
        }
        p1 += step;
        p2 += step;
    }
    return errors;
}

static void update_wlevel_rank_struct(cvmx_dfm_wlevel_rankx_t *dfm_wlevel_rank, int byte, int delay)
{
    switch(byte) {
    case 0:
        dfm_wlevel_rank->cn63xx.byte0 = delay;
        break;
    case 1:
        dfm_wlevel_rank->cn63xx.byte1 = delay;
        break;
    }
}

static int  get_wlevel_rank_struct(cvmx_dfm_wlevel_rankx_t *dfm_wlevel_rank, int byte)
{
    int delay;
    switch(byte) {
    case 0:
        delay = dfm_wlevel_rank->cn63xx.byte0;
        break;
    case 1:
        delay = dfm_wlevel_rank->cn63xx.byte1;
        break;
    }
    return delay;
}

static void update_rlevel_rank_struct(cvmx_dfm_rlevel_rankx_t *dfm_rlevel_rank, int byte, int delay)
{
    switch(byte) {
    case 0:
        dfm_rlevel_rank->cn63xx.byte0 = delay;
        break;
    case 1:
        dfm_rlevel_rank->cn63xx.byte1 = delay;
        break;
    }
}

static int  get_rlevel_rank_struct(cvmx_dfm_rlevel_rankx_t *dfm_rlevel_rank, int byte)
{
    int delay;
    switch(byte) {
    case 0:
        delay = dfm_rlevel_rank->cn63xx.byte0;
        break;
    case 1:
        delay = dfm_rlevel_rank->cn63xx.byte1;
        break;
    }
    return delay;
}

static void rlevel_to_wlevel(cvmx_dfm_rlevel_rankx_t *dfm_rlevel_rank, cvmx_dfm_wlevel_rankx_t *dfm_wlevel_rank, int byte)
{
    int byte_delay = get_rlevel_rank_struct(dfm_rlevel_rank, byte);

    debug_print("Estimating Wlevel delay byte %d: ", byte);
    debug_print("Rlevel=%d => ", byte_delay);
    byte_delay = divide_roundup(byte_delay,2) & 0x1e;
    debug_print("Wlevel=%d\n", byte_delay);
    update_wlevel_rank_struct(dfm_wlevel_rank, byte, byte_delay);
}

static int nonsequential_delays(int delay0, int delay1, int delay2)
{
    int error = 0;
    int near_diff = _abs (delay1 - delay0);
    int far_diff  = _abs (delay2 - delay0);
    int near_sign = _sign(delay1 - delay0);
    int far_sign  = _sign(delay2 - delay0);
    int sign_miss = ((near_sign != far_sign)&&(near_diff != 0));

    debug_bitmask_print("delay0: %2d, delay1: %2d, delay2: %2d", delay0, delay1, delay2);

    if ((near_diff > far_diff) || sign_miss) {
        error = 1;
        debug_bitmask_print(" => invalid (nonsequential)");
    }
    debug_bitmask_print("\n");
    return error;
}

static int roundup_ddr3_wlevel_bitmask(int bitmask)
{
    int shifted_bitmask;
    int leader;
    int delay;

    for (leader=0; leader<8; ++leader) {
        shifted_bitmask = (bitmask>>leader);
        if ((shifted_bitmask&1) == 0)
            break;
    }

    for (leader=leader; leader<16; ++leader) {
        shifted_bitmask = (bitmask>>(leader%8));
        if (shifted_bitmask&1)
            break;
    }

    delay = (leader&1) ? leader+1 : leader;
    delay = delay % 8;

    return delay;
}

/* Default ODT config must disable ODT */
/* Must be const (read only) so that the structure is in flash */
const dfm_odt_config_t disable_dfm_odt_config[] = {
    /* RANKS   ODT_ENA ODT_MASK ODT_MASK1 QS_DIC RODT_CTL DIC */ \
    /* =====   ======= ======== ========= ====== ======== === */ \
    /*   1 */ {   0,    0x0000,   {.u64 = 0x0000},    0,   0x0000,  0  },  \
    /*   2 */ {   0,    0x0000,   {.u64 = 0x0000},    0,   0x0000,  0  },  \
};

#ifdef CVMX_BUILD_FOR_UBOOT
int octeon_dfm_initialize(void)
#else
int main()
#endif
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

    uint32_t cpu_id = cvmx_get_proc_id();

    ulong tclk_psecs;
    ulong eclk_psecs;

    int row_bits, col_bits, num_banks, num_ranks, dram_width, real_num_ranks;
    int dimm_count = 1;
    int fatal_error = 0;        /* Accumulate and report all the errors before giving up */

    int safe_dfm_flag = 0; /* Flag that indicates safe DDR settings should be used */
    uint32_t mem_size_mbytes = 0;
    int bank_bits = 0;
    int bunk_enable;
    int column_bits_start = 1;
    int row_lsb;
    int pbank_lsb;

    uint64_t dfm_hertz;
    uint64_t cpu_hertz;

    int tAAmin;
    int tCKmin;
    int CL, min_cas_latency = 0, max_cas_latency = 0;
    int i;

    int twr;
    int trcd;
    int trrd;
    int trp;
    int tras;
    int trc;
    int trfc;
    int twtr;
    int trtp;
    int tfaw;

    int addr_mirror;
    int cas_latencies;

    /* These parameters are customized for each board. */
    num_banks             = DFM_CFG_NUM_BANKS    ;
    dram_width            = DFM_CFG_DRAM_WIDTH   ;
    row_bits              = DFM_CFG_ROW_BITS     ;
    col_bits              = DFM_CFG_COL_BITS     ;
    num_ranks             = DFM_CFG_NUM_RANKS    ;
    tCKmin		  = DFM_CFG_TCKMIN	 ;    
    tAAmin		  = DFM_CFG_TAAMIN	 ;    
    twr			  = DFM_CFG_TWR	         ;
    trcd		  = DFM_CFG_TRCD	 ;    
    trrd		  = DFM_CFG_TRRD	 ;    
    trp			  = DFM_CFG_TRP	         ;
    tras		  = DFM_CFG_TRAS	 ;    
    trc			  = DFM_CFG_TRC	         ;
    trfc		  = DFM_CFG_TRFC	 ;    
    twtr		  = DFM_CFG_TWTR	 ;    
    trtp		  = DFM_CFG_TRTP	 ;    
    tfaw		  = DFM_CFG_TFAW	 ;    
    addr_mirror		  = DFM_CFG_ADDR_MIRROR  ;
    cas_latencies	  = DFM_CFG_CAS_LATENCIES;


    int wlevel_bitmask_errors = 0;
    int wlevel_loops = 0;

    if (octeon_has_feature(OCTEON_FEATURE_HFA))
        dfm_print("\nInitializing DFM interface\n");
    else 
    {
        dfm_print("\nHFA disabled, not initializing DFM interface.\n");
        return(0);
    }

    cpu_hertz = cvmx_clock_get_rate(CVMX_CLOCK_SCLK);
    dfm_hertz = divide_nint((uint64_t) 1000*1000*1000*1000, tCKmin);

    if ((s = getenv("dfm_clock_hertz")) != NULL) {
        dfm_hertz = simple_strtoul(s, NULL, 0);
        printf("Parameter found in environment.  "
               "dfm_clock_hertz = %lld\n", dfm_hertz);
    }

    initialize_dfm_clock(cpu_id, cpu_hertz, dfm_hertz);
    dfm_hertz = measure_octeon_dfm_clock(cpu_id, cpu_hertz, dfm_hertz);

    /*
    ** Compute clock rates to the nearest picosecond.
    */
    tclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, dfm_hertz); /* Clock in psecs */
    eclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, cpu_hertz); /* Clock in psecs */

    /* Enable the SCLK source and DDR3 interface. */
    {
        cvmx_dfm_fnt_sclk_t dfm_fnt_sclk;
        cvmx_dfm_fnt_ctl_t dfm_fnt_ctl;

        dfm_fnt_sclk.u64 = cvmx_read_csr(CVMX_DFM_FNT_SCLK);
        dfm_fnt_sclk.s.sclkdis = 0; /* Enable the SCLK source. */
        dfm_config_write_csr(CVMX_DFM_FNT_SCLK, dfm_fnt_sclk.u64);

        dfm_fnt_ctl.u64 = cvmx_read_csr(CVMX_DFM_FNT_CTL);
        dfm_fnt_ctl.s.dfr_ena = 1; /* Enable the DDR3 interface */
        dfm_config_write_csr(CVMX_DFM_FNT_CTL, dfm_fnt_ctl.u64);
    }


#if CONFIG_SOFTWARE_LEVELING
    if ((s = dfm_getenv_debug("dfm_software_leveling")) != NULL) {
        software_leveling = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. dfm_software_leveling = %d\n", software_leveling);
    }
#endif

    if ((s = getenv("dfm_safe")) != NULL) {
        safe_dfm_flag = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. dfm_safe = %d\n", safe_dfm_flag);
    }




    /* FIX
    ** Check that values are within some theoretical limits.
    ** col_bits(min) = row_lsb(min) - bank_bits(max) - bus_bits(max) = 14 - 3 - 4 = 7
    ** col_bits(max) = row_lsb(max) - bank_bits(min) - bus_bits(min) = 18 - 2 - 3 = 13
    */
    if ((col_bits > 13) || (col_bits < 7)) {
        error_print("Unsupported number of Col Bits: %d\n", col_bits);
        ++fatal_error;
    }

    /* FIX
    ** Check that values are within some theoretical limits.
    ** row_bits(min) = pbank_lsb(min) - row_lsb(max) - rank_bits = 26 - 18 - 1 = 7
    ** row_bits(max) = pbank_lsb(max) - row_lsb(min) - rank_bits = 33 - 14 - 1 = 18
    */
    if ((row_bits > 18) || (row_bits < 7)) {
        error_print("Unsupported number of Row Bits: %d\n", row_bits);
        ++fatal_error;
    }

    if (num_banks == 8)
        bank_bits = 3;
    else if (num_banks == 4)
        bank_bits = 2;



#if 0
    /*  Avoid using h/w write-leveling for pass 1.0 and 1.1 silicon.
        This bug is partially fixed in pass 1.2-1.x. (See DFM-14415B) */
    if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_0) || octeon_is_cpuid(OCTEON_CN63XX_PASS1_1))
        wlevel_loops = 0;
    else
        wlevel_loops = 1;
#else
        wlevel_loops = 0;       /* Disable h/w write-leveling.  S/W fast enough for only 2 bytes */
#endif

    if ((s = getenv("dfm_wlevel_loops")) != NULL) {
        wlevel_loops = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. dfm_wlevel_loops = %d\n", wlevel_loops);
    }

    real_num_ranks = num_ranks;

    if ((s = dfm_getenv_debug("dfm_ranks")) != NULL) {
        num_ranks = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. dfm_ranks = %d\n", num_ranks);
    }

    bunk_enable = (num_ranks > 1);

    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX))
        column_bits_start = 1;
    else 
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    row_lsb = column_bits_start + col_bits + bank_bits;
    debug_print("row_lsb = column_bits_start + col_bits + bank_bits = %d\n", row_lsb);

    pbank_lsb = row_lsb + row_bits + bunk_enable;
    debug_print("pbank_lsb = row_lsb + row_bits + bunk_enable = %d\n", pbank_lsb);


    mem_size_mbytes =  dimm_count * ((1ull << pbank_lsb) >> 20);


    /* Mask with 1 bits set for for each active rank, allowing 2 bits per dimm.
    ** This makes later calculations simpler, as a variety of CSRs use this layout.
    ** This init needs to be updated for dual configs (ie non-identical DIMMs).
    ** Bit 0 = dimm0, rank 0
    ** Bit 0 = dimm0, rank 1
    ** Bit 0 = dimm1, rank 0
    ** Bit 0 = dimm1, rank 1
    ** ...
    */
    int rank_mask = 1 | ((num_ranks > 1) << 1);

    for (i = 1; i < dimm_count; i++)
        rank_mask |= ((rank_mask & 0x3) << (2*i));

    dfm_print("row bits: %d, col bits: %d, banks: %d, ranks: %d, dram width: %d, size: %d MB\n",
              row_bits, col_bits, num_banks, num_ranks, dram_width, mem_size_mbytes);

    CL              = divide_roundup(tAAmin, tclk_psecs);

    dfm_print("Desired CAS Latency                           : %6d\n", CL);

    dfm_print("CAS Latencies supported in DFM DRAM           :");
    for (i=0; i<16; ++i) {
        if ((cas_latencies >> i) & 1) {
            dfm_print(" %d", i+4);
            max_cas_latency = i+4;
            if (min_cas_latency == 0)
                min_cas_latency = i+4;
        }
    }
    dfm_print("\n");

    dfm_print("CAS Latency                                   : %6d\n", CL);

    if ((CL * tCKmin) > 20000)
    {
        dfm_print("(CLactual * tCKmin) = %d exceeds 20 ns\n", (CL * tCKmin));
    }

    if (tclk_psecs < (ulong)tCKmin)
        error_print("WARNING!!!!!!: DFM Clock Rate (tCLK: %ld) exceeds DRAM specifications (tCKmin:%ld)!!!!!!!!\n",
                    tclk_psecs, (ulong)tCKmin);

    dfm_print("DDR Clock Rate (tCLK)                         : %6d ps\n", tclk_psecs);
    dfm_print("Core Clock Rate (eCLK)                        : %6d ps\n", eclk_psecs);
    dfm_print("Minimum Cycle Time (tCKmin)                   : %6d ps\n", tCKmin);
    dfm_print("Minimum CAS Latency Time (tAAmin)             : %6d ps\n", tAAmin);
    dfm_print("Write Recovery Time (tWR)                     : %6d ps\n", twr);
    dfm_print("Minimum RAS to CAS delay (tRCD)               : %6d ps\n", trcd);
    dfm_print("Minimum Row Active to Row Active delay (tRRD) : %6d ps\n", trrd);
    dfm_print("Minimum Row Precharge Delay (tRP)             : %6d ps\n", trp);
    dfm_print("Minimum Active to Precharge (tRAS)            : %6d ps\n", tras);
    dfm_print("Minimum Active to Active/Refresh Delay (tRC)  : %6d ps\n", trc);
    dfm_print("Minimum Refresh Recovery Delay (tRFC)         : %6d ps\n", trfc);
    dfm_print("Internal write to read command delay (tWTR)   : %6d ps\n", twtr);
    dfm_print("Min Internal Rd to Precharge Cmd Delay (tRTP) : %6d ps\n", trtp);
    dfm_print("Minimum Four Activate Window Delay (tFAW)     : %6d ps\n", tfaw);


    if ((num_banks != 4) && (num_banks != 8))
    {
        error_print("Unsupported number of banks %d. Must be 4 or 8.\n", num_banks);
        ++fatal_error;
    }

    if ((num_ranks < 1) || (num_ranks > 2))
    {
        error_print("Unsupported number of ranks: %d\n", num_ranks);
        ++fatal_error;
    }

    if ((dram_width != 8) && (dram_width != 16))
    {
        error_print("Unsupported SDRAM Width, %d.  Must be 8 or 16.\n", dram_width);
        ++fatal_error;
    }


    /*
    ** Bail out here if things are not copasetic.
    */
    if (fatal_error)
        return(-1);



    /* Parameters from DDR3 Specifications */
#define DDR3_tREFI         7800000    /* 7.8 us */
#define DDR3_ZQCS          80000      /* 80 ns */
#define DDR3_ZQCS_Interval 1280000000 /* 128ms/100 */
#define DDR3_tCKE          5000       /* 5 ns */
#define DDR3_tMRD          4          /* 4 nCK */
#define DDR3_tDLLK         512        /* 512 nCK */
#define DDR3_tMPRR         1          /* 1 nCK */
#define DDR3_tWLMRD        40         /* 40 nCK */
#define DDR3_tWLDQSEN      25         /* 25 nCK */

    /*
     * 4.8.5 Early DFM Initialization
     *
     * All of DDR PLL, DFM CK, and DFM DRESET initializations must
     * be completed prior to starting this DFM initialization
     * sequence.
     *
     * 1. Software must ensure there are no pending DRAM transactions.
     *
     * 2. Write DFM_CONFIG, DFM_CONTROL, DFM_TIMING_PARAMS0,
     *    DFM_TIMING_PARAMS1, DFM_MODEREG_PARAMS0,
     *    DFM_MODEREG_PARAMS1, DFM_DUAL_MEMCFG, DFM_NXM,
     *    DFM_WODT_MASK, DFM_RODT_MASK, DFM_COMP_CTL2,
     *    DFM_PHY_CTL, DFM_DIMM0/1_PARAMS, and DFM_DIMM_CTL
     *    with appropriate values. All sections in this chapter
     *    may be used to derive proper register settings.
     */

    /* DFM_CONFIG */
    {
        cvmx_dfm_config_t dfm_config;
        dfm_config.u64 = cvmx_read_csr(CVMX_DFM_CONFIG);
        dfm_config.s.init_start      = 0;
        dfm_config.s.ecc_ena         = 0; /* Must be zero. */
        dfm_config.s.row_lsb         = encode_row_lsb_ddr3(cpu_id, row_lsb);
        dfm_config.s.pbank_lsb       = encode_pbank_lsb_ddr3(cpu_id, pbank_lsb);

        dfm_config.s.idlepower       = 0; /* Disabled */

        if ((s = dfm_getenv_debug("dfm_idlepower")) != NULL) {
            dfm_config.s.idlepower = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_idlepower = %d\n", dfm_config.s.idlepower);
        }

        dfm_config.s.forcewrite      = 0; /* Disabled */
        dfm_config.s.ecc_adr         = 0; /* Must be zero. */
        dfm_config.s.reset           = 0;

        /*
         *  Program DFM_CONFIG[24:18], ref_zqcs_int(6:0) to
         *  RND-DN(tREFI/clkPeriod/512) Program DFM_CONFIG[36:25],
         *  ref_zqcs_int(18:7) to
         *  RND-DN(ZQCS_Interval/clkPeriod/(512*64)). Note that this
         *  value should always be greater than 32, to account for
         *  resistor calibration delays.
         */
        dfm_config.s.ref_zqcs_int     = ((DDR3_tREFI/tclk_psecs/512) & 0x7f);
        dfm_config.s.ref_zqcs_int    |= ((max(33, (DDR3_ZQCS_Interval/(tclk_psecs/100)/(512*64))) & 0xfff) << 7);

        dfm_config.s.sequence        = 0; /* Set later */

        /* For the rare case when ADD/CMD signals are routed shorter than the
           shortest DQS/DQ. */
        dfm_config.s.early_dqx       = DFM_EARLY_DQX; 

        if ((s = dfm_getenv_debug("dfm_early_dqx")) != NULL) {
            dfm_config.s.early_dqx = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_early_dqx = %d\n", dfm_config.s.early_dqx);
        }

        dfm_config.s.sref_with_dll   = 1; /* enabled for sref */
        dfm_config.s.rank_ena        = bunk_enable;
        dfm_config.s.rankmask        = rank_mask; /* Set later */
        dfm_config.s.mirrmask        = (addr_mirror << 1 | addr_mirror << 3) & rank_mask;
        dfm_config.s.init_status     = 0; /* Set later */


        if ((s = dfm_getenv_debug("dfm_config")) != NULL) {
            dfm_config.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_config = 0x%016llx\n", (unsigned long long)dfm_config.u64);
        }
        dfm_print("DFM_CONFIG                                    : 0x%016llx\n", dfm_config.u64);
        dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
    }

    /* DFM_CONTROL */
    {
        cvmx_dfm_control_t dfm_control;
        dfm_control.u64 = cvmx_read_csr(CVMX_DFM_CONTROL);
        dfm_control.s.rdimm_ena       = 0; /* Must be zero. */
        dfm_control.s.bwcnt           = 0; /* Clear counter later */
        dfm_control.s.ddr2t           = safe_dfm_flag ? 1 : 0;
        dfm_control.s.pocas           = 0;
        dfm_control.s.fprch2          = 1;
        dfm_control.s.throttle_rd     = safe_dfm_flag ? 1 : 0;
        dfm_control.s.throttle_wr     = safe_dfm_flag ? 1 : 0;
        dfm_control.s.inorder_rd      = 0; /* Must be zero. */
        dfm_control.s.inorder_wr      = 0; /* Must be zero. */
        dfm_control.s.elev_prio_dis   = 0; /* Must be zero. */
        dfm_control.s.nxm_write_en    = 0; /* Must be zero. */
        dfm_control.s.max_write_batch = 8; /* Must be set to value 8 */
        dfm_control.s.xor_bank        = 0; /* Must be zero. */
        dfm_control.s.auto_fclkdis    = 1;
        dfm_control.s.int_zqcs_dis    = 1;
        dfm_control.s.ext_zqcs_dis    = 0;
        dfm_control.s.bprch           = 0;
        dfm_control.s.wodt_bprch      = 0;
        dfm_control.s.rodt_bprch      = 0;

        if ((s = dfm_getenv_debug("dfm_xor_bank")) != NULL) {
            dfm_control.s.xor_bank = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_xor_bank = %d\n", dfm_control.s.xor_bank);
        }

        if ((s = dfm_getenv_debug("dfm_2t")) != NULL) {
            dfm_control.s.ddr2t = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_2t = %d\n", dfm_control.s.ddr2t);
        }

        if ((s = dfm_getenv_debug("dfm_fprch2")) != NULL) {
            dfm_control.s.fprch2 = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_fprch2 = %d\n", dfm_control.s.fprch2);
        }

        if ((s = dfm_getenv_debug("dfm_bprch")) != NULL) {
            dfm_control.s.bprch = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_bprch = %d\n", dfm_control.s.bprch);
        }

        if ((s = dfm_getenv_debug("dfm_wodt_bprch")) != NULL) {
            dfm_control.s.wodt_bprch = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_wodt_bprch = %d\n", dfm_control.s.wodt_bprch);
        }

        if ((s = dfm_getenv_debug("dfm_rodt_bprch")) != NULL) {
            dfm_control.s.rodt_bprch = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rodt_bprch = %d\n", dfm_control.s.rodt_bprch);
        }

        if ((s = dfm_getenv_debug("dfm_control")) != NULL) {
            dfm_control.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_control = 0x%016llx\n", (unsigned long long)dfm_control.u64);
        }
        dfm_print("DFM_CONTROL                                   : 0x%016llx\n", dfm_control.u64);
        dfm_config_write_csr(CVMX_DFM_CONTROL, dfm_control.u64);
    }

    /* DFM_TIMING_PARAMS0 */
    {
        unsigned trp_value;
        cvmx_dfm_timing_params0_t dfm_timing_params0;
        dfm_timing_params0.u64 = cvmx_read_csr(CVMX_DFM_TIMING_PARAMS0);

        trp_value = divide_roundup(trp, tclk_psecs) 
            + divide_roundup(max(4*tclk_psecs, 7500), tclk_psecs) - 5;

        if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            /* Reserved. Should be written to zero. */
            dfm_timing_params0.cn63xxp1.tckeon   = 0;
        }
        dfm_timing_params0.s.tzqcs    = divide_roundup(DDR3_ZQCS, (16*tclk_psecs));
        dfm_timing_params0.s.tcke     = divide_roundup(DDR3_tCKE, tclk_psecs) - 1;
        dfm_timing_params0.s.txpr     = divide_roundup(max(5*tclk_psecs, trfc+10000), 16*tclk_psecs);
        dfm_timing_params0.s.tmrd     = divide_roundup((DDR3_tMRD*tclk_psecs), tclk_psecs) - 1;
        dfm_timing_params0.s.tmod     = divide_roundup(max(12*tclk_psecs, 15000), tclk_psecs) - 1;
        dfm_timing_params0.s.tdllk    = divide_roundup(DDR3_tDLLK, 256);
        dfm_timing_params0.s.tzqinit  = divide_roundup(max(512*tclk_psecs, 640000), (256*tclk_psecs));
        dfm_timing_params0.s.trp      = trp_value & 0xf;
        dfm_timing_params0.s.tcksre   = divide_roundup(max(5*tclk_psecs, 10000), tclk_psecs) - 1;

        if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
                dfm_timing_params0.s.trp_ext  = (trp_value >> 4) & 1;
        }

        if ((s = dfm_getenv_debug("dfm_timing_params0")) != NULL) {
            dfm_timing_params0.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_timing_params0 = 0x%016llx\n", (unsigned long long)dfm_timing_params0.u64);
        }
        dfm_print("TIMING_PARAMS0                                : 0x%016llx\n", dfm_timing_params0.u64);
        dfm_config_write_csr(CVMX_DFM_TIMING_PARAMS0, dfm_timing_params0.u64);
    }

    /* DFM_TIMING_PARAMS1 */
    {
        int txp;
        cvmx_dfm_timing_params1_t dfm_timing_params1;
        dfm_timing_params1.u64 = cvmx_read_csr(CVMX_DFM_TIMING_PARAMS1);
        dfm_timing_params1.s.tmprr    = divide_roundup(DDR3_tMPRR*tclk_psecs, tclk_psecs);

        dfm_timing_params1.s.tras     = divide_roundup(tras, tclk_psecs) - 1;
        dfm_timing_params1.s.trcd     = divide_roundup(trcd, tclk_psecs);
        dfm_timing_params1.s.twtr     = divide_roundup(twtr, tclk_psecs) - 1;
        dfm_timing_params1.s.trfc     = divide_roundup(trfc, 8*tclk_psecs);
        dfm_timing_params1.s.trrd     = divide_roundup(trrd, tclk_psecs) - 2;

        /*
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-800   tCLK = 2500 psec
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-1066  tCLK = 1875 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1333  tCLK = 1500 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1600  tCLK = 1250 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1866  tCLK = 1071 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-2133  tCLK =  937 psec
        */
        txp = (tclk_psecs < 1875) ? 6000 : 7500;
        dfm_timing_params1.s.txp      = divide_roundup(max(3*tclk_psecs, txp), tclk_psecs) - 1;

        dfm_timing_params1.s.twlmrd   = divide_roundup(DDR3_tWLMRD*tclk_psecs, 4*tclk_psecs);
        dfm_timing_params1.s.twldqsen = divide_roundup(DDR3_tWLDQSEN*tclk_psecs, 4*tclk_psecs);
        dfm_timing_params1.s.tfaw     = divide_roundup(tfaw, 4*tclk_psecs);
        dfm_timing_params1.s.txpdll   = divide_roundup(max(10*tclk_psecs, 24000), tclk_psecs) - 1;
        if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
                dfm_timing_params1.s.tras_ext = (tras >> 4) & 1;
        }

        if ((s = dfm_getenv_debug("dfm_timing_params1")) != NULL) {
            dfm_timing_params1.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_timing_params1 = 0x%016llx\n", (unsigned long long)dfm_timing_params1.u64);
        }
        dfm_print("TIMING_PARAMS1                                : 0x%016llx\n", dfm_timing_params1.u64);
        dfm_config_write_csr(CVMX_DFM_TIMING_PARAMS1, dfm_timing_params1.u64);
    }

    /* DFM_MODEREG_PARAMS0 */
    {
        cvmx_dfm_modereg_params0_t dfm_modereg_params0;
        int param;

        dfm_modereg_params0.u64 = cvmx_read_csr(CVMX_DFM_MODEREG_PARAMS0);


        /*
        ** CSR   CWL         CAS write Latency
        ** ===   ===   =================================
        **  0      5   (           tCK(avg) >=   2.5 ns)
        **  1      6   (2.5 ns   > tCK(avg) >= 1.875 ns)
        **  2      7   (1.875 ns > tCK(avg) >= 1.5   ns)
        **  3      8   (1.5 ns   > tCK(avg) >= 1.25  ns)
        **  4      9   (1.25 ns  > tCK(avg) >= 1.07  ns)
        **  5     10   (1.07 ns  > tCK(avg) >= 0.935 ns)
        **  6     11   (0.935 ns > tCK(avg) >= 0.833 ns)
        **  7     12   (0.833 ns > tCK(avg) >= 0.75  ns)
        */

        dfm_modereg_params0.s.cwl     = 0;
        if (tclk_psecs < 2500)
            dfm_modereg_params0.s.cwl = 1;
        if (tclk_psecs < 1875)
            dfm_modereg_params0.s.cwl = 2;
        if (tclk_psecs < 1500)
            dfm_modereg_params0.s.cwl = 3;
        if (tclk_psecs < 1250)
            dfm_modereg_params0.s.cwl = 4;
        if (tclk_psecs < 1070)
            dfm_modereg_params0.s.cwl = 5;
        if (tclk_psecs <  935)
            dfm_modereg_params0.s.cwl = 6;
        if (tclk_psecs <  833)
            dfm_modereg_params0.s.cwl = 7;

        if ((s = dfm_getenv_debug("dfm_cwl")) != NULL) {
            dfm_modereg_params0.s.cwl = simple_strtoul(s, NULL, 0) - 5;
            error_print("Parameter found in environment. dfm_cwl = %6d\n", dfm_modereg_params0.s.cwl + 5);
        }

        dfm_print("CAS Write Latency                             : %6d\n", dfm_modereg_params0.s.cwl + 5);

        dfm_modereg_params0.s.mprloc  = 0; /* Must be zero. */
        dfm_modereg_params0.s.mpr     = 0; /* Must be zero. */
        dfm_modereg_params0.s.dll     = 0;
        dfm_modereg_params0.s.al      = 0;
        dfm_modereg_params0.s.wlev    = 0;
        dfm_modereg_params0.s.tdqs    = 0;
        dfm_modereg_params0.s.qoff    = 0; /* Must be zero. */
        dfm_modereg_params0.s.bl      = 0; /* RO. Must be zero. */

        if ((s = dfm_getenv_debug("dfm_cl")) != NULL) {
            CL = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_cl = %d\n", CL);
            dfm_print("CAS Latency                                   : %6d\n", CL);
        }

        dfm_modereg_params0.s.cl      = 0x2;
        if (CL > 5)
            dfm_modereg_params0.s.cl  = 0x4;
        if (CL > 6)
            dfm_modereg_params0.s.cl  = 0x6;
        if (CL > 7)
            dfm_modereg_params0.s.cl  = 0x8;
        if (CL > 8)
            dfm_modereg_params0.s.cl  = 0xA;
        if (CL > 9)
            dfm_modereg_params0.s.cl  = 0xC;
        if (CL > 10)
            dfm_modereg_params0.s.cl  = 0xE;
        if (CL > 11)
            dfm_modereg_params0.s.cl  = 0x1;
        if (CL > 12)
            dfm_modereg_params0.s.cl  = 0x3;
        if (CL > 13)
            dfm_modereg_params0.s.cl  = 0x5;
        if (CL > 14)
            dfm_modereg_params0.s.cl  = 0x7;
        if (CL > 15)
            dfm_modereg_params0.s.cl  = 0x9;

        dfm_modereg_params0.s.rbt     = 1; /* RO. Must be set. */
        dfm_modereg_params0.s.tm      = 0;
        dfm_modereg_params0.s.dllr    = 0;

        param = divide_roundup(twr, tclk_psecs);
        dfm_modereg_params0.s.wrp     = 1;
        if (param > 5)
            dfm_modereg_params0.s.wrp = 2;
        if (param > 6)
            dfm_modereg_params0.s.wrp = 3;
        if (param > 7)
            dfm_modereg_params0.s.wrp = 4;
        if (param > 8)
            dfm_modereg_params0.s.wrp = 5;
        if (param > 10)
            dfm_modereg_params0.s.wrp = 6;
        if (param > 12)
            dfm_modereg_params0.s.wrp = 7;

        dfm_modereg_params0.s.ppd     = 0;

        if ((s = dfm_getenv_debug("dfm_wrp")) != NULL) {
            dfm_modereg_params0.s.wrp = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_wrp = 0x%x\n", dfm_modereg_params0.s.wrp);
        }

        dfm_print("Write recovery for auto precharge (WRP)       : %6d\n", dfm_modereg_params0.s.wrp);

        if ((s = dfm_getenv_debug("dfm_modereg_params0")) != NULL) {
            dfm_modereg_params0.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_modereg_params0 = 0x%016llx\n", (unsigned long long)dfm_modereg_params0.u64);
        }
        dfm_print("MODEREG_PARAMS0                               : 0x%016llx\n", dfm_modereg_params0.u64);
        dfm_config_write_csr(CVMX_DFM_MODEREG_PARAMS0, dfm_modereg_params0.u64);
    }

    /* DFM_MODEREG_PARAMS1 */
    {
        cvmx_dfm_modereg_params1_t dfm_modereg_params1;
        dfm_modereg_params1.u64 = dfm_odt_config[num_ranks-1].odt_mask1.u64;

        for (i=0; i<4; ++i) {
            char buf[10] = {0};
            uint64_t value;
            sprintf(buf, "dfm_rtt_nom_%1d%1d", !!(i&2), !!(i&1));
            if ((s = dfm_getenv_debug(buf)) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. %s = %llx\n", buf, value);
                dfm_modereg_params1.u64 &= ~((uint64_t)0x7 << (i*9));
                dfm_modereg_params1.u64 |=  (        value << (i*9));
            }
        }

        if ((s = dfm_getenv_debug("dfm_modereg_params1")) != NULL) {
            dfm_modereg_params1.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_modereg_params1 = 0x%016llx\n", (unsigned long long)dfm_modereg_params1.u64);
        }
        dfm_print("MODEREG_PARAMS1                               : 0x%016llx\n", dfm_modereg_params1.u64);
        dfm_config_write_csr(CVMX_DFM_MODEREG_PARAMS1, dfm_modereg_params1.u64);
    }


#if 0                           /* Deal with dual mem config later */
    /* DFM_DUAL_MEMCFG */
    {
        cvmx_dfm_dual_memcfg_t dfm_dual_memcfg;
        dfm_dual_memcfg.u64 = cvmx_read_csr(CVMX_DFM_DUAL_MEMCFG);
        dfm_dual_memcfg.s.cs_mask           =
        dfm_dual_memcfg.s.row_lsb           =
            if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
                dfm_dual_memcfg.s.bank8             =
        }
        dfm_config_write_csr(CVMX_DFM_DUAL_MEMCFG, dfm_dual_memcfg.u64);
    }
#endif

    /* DFM_WODT_MASK */
    {
        cvmx_dfm_wodt_mask_t dfm_wodt_mask;
        dfm_wodt_mask.u64 = dfm_odt_config[num_ranks-1].odt_mask;

        if ((s = dfm_getenv_debug("dfm_wodt_mask")) != NULL) {
            dfm_wodt_mask.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_wodt_mask = 0x%016llx\n", (unsigned long long)dfm_wodt_mask.u64);
        }

        dfm_print("WODT_MASK                                     : 0x%016llx\n", dfm_wodt_mask.u64);
        dfm_config_write_csr(CVMX_DFM_WODT_MASK, dfm_wodt_mask.u64);
    }

    /* DFM_RODT_MASK */
    {
        cvmx_dfm_rodt_mask_t dfm_rodt_mask;
        dfm_rodt_mask.u64 = dfm_odt_config[num_ranks-1].rodt_ctl;

        if ((s = dfm_getenv_debug("dfm_rodt_mask")) != NULL) {
            dfm_rodt_mask.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rodt_mask = 0x%016llx\n", (unsigned long long)dfm_rodt_mask.u64);
        }

        dfm_print("RODT_MASK                                     : 0x%016llx\n", dfm_rodt_mask.u64);
        dfm_config_write_csr(CVMX_DFM_RODT_MASK, dfm_rodt_mask.u64);
    }

    /* DFM_COMP_CTL2 */
    {
        cvmx_dfm_comp_ctl2_t dfm_comp_ctl2;
        dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
        dfm_comp_ctl2.s.ntune              = 0;
        dfm_comp_ctl2.s.ptune              = 0;
        dfm_comp_ctl2.s.byp                = 0;
        dfm_comp_ctl2.s.m180               = 0;

        if ((s = dfm_getenv_debug("dfm_comp_ctl2")) != NULL) {
            dfm_comp_ctl2.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_comp_ctl2 = 0x%016llx\n", (unsigned long long)dfm_comp_ctl2.u64);
        }

        dfm_print("COMP_CTL2                                     : 0x%016llx\n", dfm_comp_ctl2.u64);
        dfm_config_write_csr(CVMX_DFM_COMP_CTL2, dfm_comp_ctl2.u64);
    }

    /* DFM_PHY_CTL */
    {
        cvmx_dfm_phy_ctl_t dfm_phy_ctl;
        dfm_phy_ctl.u64 = cvmx_read_csr(CVMX_DFM_PHY_CTL);
        dfm_phy_ctl.s.ts_stagger           = 0;
#if 0                           /* Not in HRM */
        dfm_phy_ctl.s.loopback_pos         = 0;
        dfm_phy_ctl.s.loopback             = 0;
        dfm_phy_ctl.s.ck_dlyout0           = 0;
        dfm_phy_ctl.s.ck_tune0             = 0;
        dfm_phy_ctl.s.ck_dlyout1           = 0;
        dfm_phy_ctl.s.ck_tune1             = 0;
        dfm_phy_ctl.s.lv_mode              = 0;
#endif

        if ((s = dfm_getenv_debug("dfm_phy_ctl")) != NULL) {
            dfm_phy_ctl.u64    = simple_strtoull(s, NULL, 0);
            error_print("Parameter found in environment. dfm_phy_ctl = 0x%016llx\n", (unsigned long long)dfm_phy_ctl.u64);
        }

        dfm_print("PHY_CTL                                       : 0x%016llx\n", dfm_phy_ctl.u64);
        dfm_config_write_csr(CVMX_DFM_PHY_CTL, dfm_phy_ctl.u64);
    }


    /*
     * 3. Without changing any other fields in DFM_CONFIG, write
     *    DFM_CONFIG[RANKMASK,INIT_STATUS,SEQUENCE] while writing
     *    DFM_CONFIG[INIT_START] = 1, all with a single CSR
     *    write. Both DFM_CONFIG[RANKMASK,INIT_STATUS] bits
     *    should be set to the same mask value indicating the
     *    ranks that will participate in the sequence.
     *
     *    The SEQUENCE value should select power-up/init or
     *    self-refresh exit, depending on whether the DRAM parts
     *    are in self-refresh and whether their contents should be
     *    preserved. While DFM performs these sequences, it will
     *    not perform any other DDR3 transactions.
     *
     *    If power-up/init is selected immediately following a
     *    DRESET assertion, DFM executes the sequence described in
     *    the Reset and Initialization Procedure section of the
     *    JEDEC DDR3 specification. This includes activating CKE,
     *    writing all four DDR3 mode registers on all selected
     *    ranks, and issuing the required ZQCL command. The
     *    RANKMASK value should select all ranks with attached
     *    DRAM in this case. If DFM_CONTROL[RDIMM_ENA]=1, DFM
     *    writes the JEDEC standard SSTE32882 control words
     *    selected by DFM_DIMM_CTL [DIMM*_WMASK] between DFM_CKE*
     *    signal assertion and the first DDR3 mode register write.
     */

    {
        int dfm_init_loops = 1;
        char *sequence_str[] = {
            "power-up/init",
            "read-leveling",
            "self-refresh entry",
            "self-refresh exit",
            "precharge power-down entry",
            "precharge power-down exit",
            "write-leveling",
            "illegal"
        };

        if ((s = dfm_getenv_debug("dfm_init_loops")) != NULL) {
            dfm_init_loops = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_init_loops = %d\n", dfm_init_loops);
        }

        while (dfm_init_loops--) {
            cvmx_dfm_config_t dfm_config;
            cvmx_dfm_control_t dfm_control;
            cvmx_dfm_reset_ctl_t dfm_reset_ctl;
            int save_ddr2t;

            dfm_control.u64 = cvmx_read_csr(CVMX_DFM_CONTROL);
            save_ddr2t                    = dfm_control.s.ddr2t;

            /* Some register parts (IDT and TI included) do not like
               the sequence that DFM generates for an MRS register
               write in 1T mode. In this case, the register part does
               not properly forward the MRS register write to the DRAM
               parts.  See errata (DFM-14548) Issues with registered
               DIMMs. */
            dfm_control.s.ddr2t           = 1;

            if ((s = dfm_getenv_debug("dfm_init_2t")) != NULL) {
                dfm_control.s.ddr2t = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. dfm_init_2t = %d\n", dfm_control.s.ddr2t);
            }

            dfm_config_write_csr(CVMX_DFM_CONTROL, dfm_control.u64);

            dfm_config.u64 = cvmx_read_csr(CVMX_DFM_CONFIG);
            dfm_config.s.init_start      = 1;

            dfm_reset_ctl.u64 = cvmx_read_csr(CVMX_DFM_RESET_CTL);
            if (dfm_reset_ctl.cn63xx.ddr3psv) {
                /* Contents are being preserved */
                dfm_print("Exiting Self-Refresh\n");
                dfm_config.s.sequence        = 3; /* self-refresh exit */
                /* Re-initialize flags */
                dfm_reset_ctl.cn63xx.ddr3pwarm = 0;
                dfm_reset_ctl.cn63xx.ddr3psoft = 0;
                dfm_reset_ctl.cn63xx.ddr3psv   = 0;
                cvmx_write_csr(CVMX_DFM_RESET_CTL, dfm_reset_ctl.u64);
            } else {
                /* Contents are not being preserved */
                dfm_config.s.sequence        = 0; /* power-up/init */
            }

            dfm_config.s.rankmask        = rank_mask;
            dfm_config.s.init_status     = rank_mask;

            if ((s = dfm_getenv_debug("dfm_trigger_init")) != NULL) {
                int trigger = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. dfm_trigger_init = %d\n", trigger);
                if (trigger)
                    pulse_gpio_pin(1, 2);
            }

            dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
            cvmx_read_csr(CVMX_DFM_CONFIG);
            cvmx_wait_usec(600); /* Wait a while */

            if ((s = dfm_getenv_debug("dfm_sequence1")) != NULL) {
                int sequence1;
                sequence1 = simple_strtoul(s, NULL, 0);

                dfm_config.s.sequence        = sequence1;
                dfm_config.s.init_start      = 1;

                error_print("Parameter found in environment. dfm_sequence1 = %d (%s)\n",
                            sequence1, sequence_str[dfm_config.s.sequence]);

                if ((s = dfm_getenv_debug("dfm_trigger_sequence1")) != NULL) {
                    int trigger = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. dfm_trigger_sequence1 = %d\n", trigger);
                    if (trigger)
                        pulse_gpio_pin(1, 2);
                }

                dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
                cvmx_read_csr(CVMX_DFM_CONFIG);
                cvmx_wait_usec(600); /* Wait a while */
            }

            if ((s = dfm_getenv_debug("dfm_sequence2")) != NULL) {
                int sequence2;
                sequence2 = simple_strtoul(s, NULL, 0);

                dfm_config.s.sequence        = sequence2;
                dfm_config.s.init_start      = 1;

                error_print("Parameter found in environment. dfm_sequence2 = %d (%s)\n",
                            sequence2, sequence_str[dfm_config.s.sequence]);

                if ((s = dfm_getenv_debug("dfm_trigger_sequence2")) != NULL) {
                    int trigger = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. dfm_trigger_sequence2 = %d\n", trigger);
                    if (trigger)
                        pulse_gpio_pin(1, 2);
                }

                dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
                cvmx_read_csr(CVMX_DFM_CONFIG);
                cvmx_wait_usec(600); /* Wait a while */
            }

            dfm_control.s.ddr2t           = save_ddr2t;
            dfm_config_write_csr(CVMX_DFM_CONTROL, dfm_control.u64);
            cvmx_read_csr(CVMX_DFM_CONTROL);
        }
    }

#ifdef DFM_DEBUG
    /* Not described in HRM. */
    {
        int byte;
        int dll90[9];
        cvmx_dfm_dll_ctl3_t dfm_dll_ctl3;
        dfm_dll_ctl3.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL3);

        dfm_print("DLL90_SETTING                       Bytes 8:0 :");
        for (byte=0; byte<9; ++byte) {
            dfm_dll_ctl3.s.dll90_byte_sel = byte+1;
            dfm_config_write_csr(CVMX_DFM_DLL_CTL3, dfm_dll_ctl3.u64);
            cvmx_read_csr(CVMX_DFM_DLL_CTL3);
            dfm_dll_ctl3.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL3);
            dll90[i] = dfm_dll_ctl3.s.dll90_setting;
            dfm_print(" %d", dll90[i]);
        }
        dfm_print("\n");
    }
#endif

    /*
     *    If self-refresh exit is selected, DFM executes the
     *    required SRX command followed by a refresh and ZQ
     *    calibration. Section 4.5 describes behavior of a REF +
     *    ZQCS.  DFM does not write the DDR3 mode registers as
     *    part of this sequence, and the mode register parameters
     *    must match at self-refresh entry and exit times. The
     *    self-refresh exit sequence does not use DFM_CONFIG
     *    [RANKMASK]. If the self-refresh exit is preceded by a
     *    self-refresh exit SEQUENCE without an intervening DRESET
     *    sequence, then DFM_CONFIG [INIT_STATUS] will already
     *    indicate the ranks that should participate, and software
     *    need not assert any DFM_CONFIG[INIT_STATUS]
     *    bits. Otherwise, the DFM_CONFIG CSR write that
     *    initiates the self-refresh exit must also set
     *    DFM_CONFIG[INIT_STATUS] bits for all ranks with
     *    attached DRAM.
     *
     * 4. Read DFM_CONFIG and wait for the result.
     */

        cvmx_read_csr(CVMX_DFM_CONFIG); /* Read CVMX_DFM_CONFIG */
        cvmx_wait_usec(1000);   /* Wait a while. */

    /*
     * 4.8.6 DFM Write Leveling
     *
     * DFM supports an automatic write-leveling like that
     * described in the JEDEC DDR3 specifications separately per
     * byte-lane.
     *
     * All of DDR PLL, DFM CK, and DFM DRESET, and early DFM
     * initializations must be completed prior to starting this
     * DFM write-leveling sequence.
     *
     * There are many possible procedures that will write-level
     * all the attached DDR3 DRAM parts. One possibility is for
     * software to simply write the desired values into
     * DFM_WLEVEL_RANK(0..3). This section describes one possible
     * sequence that uses DFMs auto-write-leveling capabilities.
     */

    {
        cvmx_dfm_wlevel_ctl_t wlevel_ctl;
        cvmx_dfm_wlevel_rankx_t dfm_wlevel_rank;
        cvmx_dfm_config_t dfm_config;
        int rankx = 0;
        int wlevel_bitmask[2];
        int byte_idx;
        int passx;
        int dfm_wlevel_roundup = 0;

        if (wlevel_loops)
            dfm_print("Performing Write-Leveling\n");
        else
            wlevel_bitmask_errors = 1; /* Force software write-leveling to run */

        while(wlevel_loops--) {
        if ((s = dfm_getenv_debug("dfm_wlevel_roundup")) != NULL) {
            dfm_wlevel_roundup = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_wlevel_roundup = %d\n", dfm_wlevel_roundup);
        }
        for (rankx = 0; rankx < dimm_count * 2;rankx++) {
            if (!(rank_mask & (1 << rankx)))
                continue;

            wlevel_ctl.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_CTL);
            dfm_config.u64 = cvmx_read_csr(CVMX_DFM_CONFIG);
          
            dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), 0); /* Clear write-level delays */
    
            wlevel_bitmask_errors = 0; /* Reset error counter */

            for (byte_idx=0; byte_idx<2; ++byte_idx) {
                wlevel_bitmask[byte_idx] = 0; /* Reset bitmasks */
            }

            /* Make two passes to accomodate some x16 parts that
               require it.  Others don't care. */
            for (passx=0; passx<2; ++passx) {
                wlevel_ctl.s.lanemask = 0x155 >> passx; /* lanemask = 0x155, 0x0AA */
    
                dfm_config_write_csr(CVMX_DFM_WLEVEL_CTL, wlevel_ctl.u64);
    
                /* Read and write values back in order to update the
                   status field. This insurs that we read the updated
                   values after write-leveling has completed. */
                dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx),
                               cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx)));
    
                dfm_config.s.sequence        = 6; /* write-leveling */
                dfm_config.s.rankmask        = 1 << rankx; /* FIXME: hardwire 1 rank for now */
                dfm_config.s.init_start      = 1;

                if ((s = dfm_getenv_debug("dfm_trigger_wleveling")) != NULL) {
                    int trigger = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. dfm_trigger_wleveling = %d\n", trigger);
                    if (trigger)
                        pulse_gpio_pin(1, 2);
                }

                dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
    
                do {
                    dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));
                } while (dfm_wlevel_rank.cn63xx.status != 3);
    
                for (byte_idx=passx; byte_idx<2; byte_idx+=2) {
                    wlevel_bitmask[byte_idx] = octeon_read_dfm_ddr3_wlevel_dbg(byte_idx);
                    if (wlevel_bitmask[byte_idx] == 0)
                        ++wlevel_bitmask_errors;
                }
    
                dfm_print("Rank(%d) Wlevel Debug Results   (lanemask %03X) : %05x %05x\n",
                          rankx,
                          wlevel_ctl.s.lanemask,
                          wlevel_bitmask[1],
                          wlevel_bitmask[0]
                          );
    
                dfm_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d\n",
                          rankx,
                          dfm_wlevel_rank.cn63xx.status,
                          dfm_wlevel_rank.u64,
                          dfm_wlevel_rank.cn63xx.byte1,
                          dfm_wlevel_rank.cn63xx.byte0
                          );
            }
    
            if (dfm_wlevel_roundup) { /* Round up odd bitmask delays */
                for (byte_idx=0; byte_idx<2; ++byte_idx) {
                    update_wlevel_rank_struct(&dfm_wlevel_rank,
                                           byte_idx,
                                           roundup_ddr3_wlevel_bitmask(wlevel_bitmask[byte_idx]));
                }
                dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), dfm_wlevel_rank.u64);
                dfm_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d\n",
                          rankx,
                          dfm_wlevel_rank.cn63xx.status,
                          dfm_wlevel_rank.u64,
                          dfm_wlevel_rank.cn63xx.byte1,
                          dfm_wlevel_rank.cn63xx.byte0
                          );
            }

            if (wlevel_bitmask_errors != 0) {
                dfm_print("Rank(%d) Write-Leveling Failed: %d Bitmask errors\n", rankx, wlevel_bitmask_errors);
            }
        }
        } /* while(wlevel_loops--) */
    }

    /*
     * 1. If the DQS/DQ delays on the board may be more than the
     *    ADD/CMD delays, then ensure that DFM_CONFIG[EARLY_DQX]
     *    is set at this point.
     *
     * Do the remaining steps 2-7 separately for each rank i with
     * attached DRAM.
     *
     * 2. Write DFM_WLEVEL_RANKi = 0.
     *
     * 3. For 8 parts:
     *
     *    Without changing any other fields in DFM_WLEVEL_CTL,
     *    write DFM_WLEVEL_CTL[LANEMASK] to select all byte lanes
     *    with attached DRAM.
     *
     *    For 16 parts:
     *
     *    Without changing any other fields in DFM_WLEVEL_CTL,
     *    write DFM_WLEVEL_CTL[LANEMASK] to select all even byte
     *    lanes with attached DRAM.
     *
     * 4. Without changing any other fields in DFM_CONFIG,
     *
     *    o write DFM_CONFIG[SEQUENCE] to select write-leveling, and
     *
     *    o write DFM_CONFIG[RANKMASK] = (1 << i), and
     *
     *    o write DFM_CONFIG[INIT_START] = 1
     *
     *    DFM will initiate write-leveling at this point. Assuming
     *    DFM_WLEVEL_CTL [SSET] = 0, DFM first enables
     *    write-leveling on the selected DRAM rank via a DDR3 MR1
     *    write, then sequences through and accumulates
     *    write-leveling results for 8 different delay settings
     *    twice, starting at a delay of zero in this case since
     *    DFM_WLEVEL_RANKi[BYTE*<4:3>] = 0, increasing by 1/8 CK
     *    each setting, covering a total distance of one CK, then
     *    disables the write-leveling via another DDR3 MR1 write.
     *
     *    After the sequence through 16 delay settings is complete:
     *
     *    o DFM sets DFM_WLEVEL_RANKi[STATUS]=3
     *
     *    o DFM sets DFM_WLEVEL_RANKi[BYTE*<2:0>] (for all ranks
     *      selected by LANEMASK) to indicate the first write
     *      leveling result of 1 that followed result of 0 during
     *      the sequence, except that DFM always writes
     *      DFM_WLEVEL_RANKi[BYTE*<0>]=0.
     *
     *    o Software can read the 8 write-leveling results from
     *      the first pass through the delay settings by reading
     *      DFM_WLEVEL_DBG[BITMASK] (after writing
     *      DFM_WLEVEL_DBG[BYTE]). (DFM does not retain the
     *      write-leveling results from the second pass through
     *      the 8 delay settings. They should often be identical
     *      to the DFM_WLEVEL_DBG[BITMASK] results, though.)
     *
     * 5. Wait until DFM_WLEVEL_RANKi[STATUS] != 2.
     *
     *    DFM will have updated DFM_WLEVEL_RANKi[BYTE*<2:0>] for
     *    all byte lanes selected by DFM_WLEVEL_CTL[LANEMASK] at
     *    this point.  DFM_WLEVEL_RANKi[BYTE*<4:3>] will still be
     *    the value that software wrote in step 2 above, which is
     *    zero.
     *
     * 6. For 16 parts:
     *
     *    Without changing any other fields in DFM_WLEVEL_CTL,
     *    write DFM_WLEVEL_CTL[LANEMASK] to select all odd byte
     *    lanes with attached DRAM.
     *
     *    Repeat steps 4 and 5 with this new
     *    DFM_WLEVEL_CTL[LANEMASK] setting. Skip to step 7 if
     *    this has already been done.
     *
     *    For 8 parts:
     *
     *    Skip this step. Go to step 7.
     *
     * 7. Calculate DFM_WLEVEL_RANKi[BYTE*<4:3>] settings for all
     *    byte lanes on all ranks with attached DRAM.
     *
     *    At this point, all byte lanes on rank i with attached
     *    DRAM should have been write-leveled, and
     *    DFM_WLEVEL_RANKi[BYTE*<2:0>] has the result for each
     *    byte lane.
     *
     *    But note that the DDR3 write-leveling sequence will only
     *    determine the delay modulo the CK cycle time, and cannot
     *    determine how many additional CKs of delay are
     *    present. Software must calculate the number of CKs, or
     *    equivalently, the DFM_WLEVEL_RANKi[BYTE*<4:3>]
     *    settings.
     *
     *    This BYTE*<4:3> calculation is system/board specific.
     *
     *    Many techniques may be used to calculate write-leveling
     *    BYTE*<4:3> values, including:
     *
     *    o Known values for some byte lanes.
     *
     *    o Relative values for some byte lanes relative to others.
     *
     *    For example, suppose lane X is likely to require a
     *    larger write-leveling delay than lane Y. A BYTEX<2:0>
     *    value that is much smaller than the BYTEY<2:0> value may
     *    then indicate that the required lane X delay wrapped
     *    into the next CK, so BYTEX<4:3> should be set to
     *    BYTEY<4:3>+1.
     *
     * 8. Initialize DFM_WLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    DFM_WLEVEL_RANKj = DFM_WLEVEL_RANKi.
     *
     * 4.8.7 DFM Read Leveling
     *
     * DFM supports an automatic read-leveling separately per
     * byte-lane using the DDR3 multi-purpose register pre-defined
     * pattern for system calibration defined in the JEDEC DDR3
     * specifications.
     *
     * All of DDR PLL, DFM CK, and DFM DRESET, and early DFM
     * initializations must be completed prior to starting this
     * DFM read-leveling sequence.
     *
     * Software could simply write the desired read-leveling
     * values into DFM_RLEVEL_RANK(0..3). This section describes
     * a sequence that uses DFMs auto-read-leveling capabilities.
     *
     * When DFM does the read-leveling sequence for a rank, it
     * first enables the DDR3 multi-purpose register predefined
     * pattern for system calibration on the selected DRAM rank
     * via a DDR3 MR3 write, then executes 64 RD operations at
     * different internal delay settings, then disables the
     * predefined pattern via another DDR3 MR3 write. DFM
     * determines the pass or fail of each of the 64 settings
     * independently for each byte lane, then writes appropriate
     * DFM_RLEVEL_RANK(0..3)[BYTE*] values for the rank.
     *
     * After read-leveling for a rank, software can read the 64
     * pass/fail indications for one byte lane via
     * DFM_RLEVEL_DBG[BITMASK]. Software can observe all
     * pass/fail results for all byte lanes in a rank via separate
     * read-leveling sequences on the rank with different
     * DFM_RLEVEL_CTL[BYTE] values.
     *
     * The 64 pass/fail results will typically have failures for
     * the low delays, followed by a run of some passing settings,
     * followed by more failures in the remaining high delays.
     * DFM sets DFM_RLEVEL_RANK(0..3)[BYTE*] to one of the
     * passing settings. First, DFM selects the longest run of
     * successes in the 64 results. (In the unlikely event that
     * there is more than one longest run, DFM selects the first
     * one.) Then if DFM_RLEVEL_CTL[OFFSET_EN] = 1 and the
     * selected run has more than DFM_RLEVEL_CTL[OFFSET]
     * successes, DFM selects the last passing setting in the run
     * minus DFM_RLEVEL_CTL[OFFSET]. Otherwise DFM selects the
     * middle setting in the run (rounding earlier when
     * necessary). We expect the read-leveling sequence to produce
     * good results with the reset values DFM_RLEVEL_CTL
     * [OFFSET_EN]=1, DFM_RLEVEL_CTL[OFFSET] = 2.
     *
     * The read-leveling sequence:
     *
     * 1. Select desired DFM_RLEVEL_CTL[OFFSET_EN,OFFSET,BYTE]
     *    settings.  Do the remaining steps 2V4 separately for
     *    each rank i with attached DRAM.
     */

    {
        cvmx_dfm_rlevel_rankx_t dfm_rlevel_rank;
        cvmx_dfm_config_t dfm_config;
        cvmx_dfm_comp_ctl2_t dfm_comp_ctl2;
        cvmx_dfm_rlevel_ctl_t rlevel_ctl;
        cvmx_dfm_control_t dfm_control;
        int rodt_ctl;
        int rankx = 0;
        int rlevel_rodt_errors = 0;
        int rodt_ohms[] = { -1, 20, 30, 40, 60, 120 };
        int rlevel_loops = 1;
        int override_rodt_ctl = -1;
        int save_ddr2t;
        int saved_ddr__ptune, saved_ddr__ntune, rlevel_comp_offset = 4;
        struct {
            uint64_t setting;
            int      score;
        } rlevel_scoreboard[sizeof(rodt_ohms)/sizeof(int)][4];

        /*
        ** Part of the recommended approach to using read-leveling is
        ** to try various settings for Read ODT strength selecting the
        ** setting that provides the best results.  This methodology
        ** can be overriden by setting OVERRIDE_RODT_CTL to a fixed
        ** value.
        */
#ifdef OVERRIDE_RODT_CTL
        override_rodt_ctl = OVERRIDE_RODT_CTL;
#endif

        dfm_control.u64 = cvmx_read_csr(CVMX_DFM_CONTROL);
        save_ddr2t                    = dfm_control.s.ddr2t;

        /* During the read-leveling sequence with registered DIMMs
           when 2T mode is enabled (i.e. DFM_CONTROL[DDR2T]=1), some
           register parts do not like the sequence that DFM generates
           for an MRS register write. (14656) */
        dfm_control.s.ddr2t           = 0;

        if ((s = dfm_getenv_debug("dfm_rlevel_2t")) != NULL) {
            dfm_control.s.ddr2t = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rlevel_2t = %d\n", dfm_control.s.ddr2t);
        }

        dfm_config_write_csr(CVMX_DFM_CONTROL, dfm_control.u64);

        dfm_print("Performing Read-Leveling\n");

        rlevel_ctl.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_CTL);

        rlevel_ctl.s.delay_unload_0 = 1; /* should normally be set */
        rlevel_ctl.s.delay_unload_1 = 1; /* should normally be set */
        rlevel_ctl.s.delay_unload_2 = 1; /* should normally be set */
        rlevel_ctl.s.delay_unload_3 = 1; /* should normally be set */

        rlevel_ctl.s.offset    = 2;
        rlevel_ctl.s.offset_en = 1;
        if ((s = dfm_getenv_debug("dfm_rlevel_offset")) != NULL) {
            rlevel_ctl.s.offset   = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rlevel_offset = 0x%x\n", rlevel_ctl.s.offset);
        }
        dfm_config_write_csr(CVMX_DFM_RLEVEL_CTL, rlevel_ctl.u64);

        if ((s = dfm_getenv_debug("dfm_rlevel_loops")) != NULL) {
            rlevel_loops = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rlevel_loops = %d\n", rlevel_loops);
        }

        if ((s = dfm_getenv_debug("dfm_rodt_ctl")) != NULL) {
            override_rodt_ctl    = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rodt_ctl = 0x%x\n", override_rodt_ctl);
        }

        while(rlevel_loops--) {
        /* Initialize the error scoreboard */
        for (rodt_ctl=5; rodt_ctl>0; --rodt_ctl) {
            for (rankx = 0; rankx < dimm_count*2; rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;
                rlevel_scoreboard[rodt_ctl][rankx].score   = RLEVEL_UNDEFINED_ERROR;
                rlevel_scoreboard[rodt_ctl][rankx].setting = 0;
            }
        }

        if ((s = dfm_getenv_debug("dfm_rlevel_comp_offset")) != NULL) {
            rlevel_comp_offset  = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_rlevel_comp_offset = 0x%x\n",
                        rlevel_comp_offset);
        }

        dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
        saved_ddr__ptune = dfm_comp_ctl2.s.ddr__ptune;
        saved_ddr__ntune = dfm_comp_ctl2.s.ddr__ntune;

        /* Disable dynamic compensation settings */
        if (rlevel_comp_offset != 0) {
            dfm_comp_ctl2.s.ptune = saved_ddr__ptune;
            dfm_comp_ctl2.s.ntune = saved_ddr__ntune;

            dfm_comp_ctl2.s.ptune += rlevel_comp_offset;

            dfm_comp_ctl2.s.byp = 1; /* Enable bypass mode */
            dfm_config_write_csr(CVMX_DFM_COMP_CTL2, dfm_comp_ctl2.u64);
            cvmx_read_csr(CVMX_DFM_COMP_CTL2);

            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2); /* Read again */
            dfm_print("DDR__PTUNE/DDR__PTUNE                         : %d/%d\n",
                      dfm_comp_ctl2.s.ddr__ptune, dfm_comp_ctl2.s.ddr__ntune);
        }

        for (rodt_ctl=dfm_odt_config[num_ranks-1].qs_dic; rodt_ctl>0; --rodt_ctl) {
            rlevel_rodt_errors = 0;

            rodt_ctl = (override_rodt_ctl == (-1)) ? rodt_ctl : override_rodt_ctl;

            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
            dfm_comp_ctl2.s.rodt_ctl = rodt_ctl;
            dfm_config_write_csr(CVMX_DFM_COMP_CTL2, dfm_comp_ctl2.u64);
            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
            dfm_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                      dfm_comp_ctl2.s.rodt_ctl, rodt_ohms[rodt_ctl]);
    
            for (rankx = 0; rankx < dimm_count*2; rankx++) {
                int rlevel_rank_errors = 0;
                if (!(rank_mask & (1 << rankx)))
                    continue;

                dfm_config.u64 = cvmx_read_csr(CVMX_DFM_CONFIG);
    
                /* Clear read-level delays */
                dfm_config_write_csr(CVMX_DFM_RLEVEL_RANKX(rankx), 0);
    
                dfm_config.s.sequence        = 1; /* read-leveling */
                dfm_config.s.rankmask        = 1 << rankx;
                dfm_config.s.init_start      = 1;

                if ((s = dfm_getenv_debug("dfm_trigger_rleveling")) != NULL) {
                    int trigger = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. dfm_trigger_rleveling = %d\n", trigger);
                    if (trigger)
                        pulse_gpio_pin(1, 2);
                }

                dfm_config_write_csr(CVMX_DFM_CONFIG, dfm_config.u64);
    
                do {
                    dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));
                } while (dfm_rlevel_rank.cn63xx.status != 3);
    
                {
                    int rlevel_bitmask[9];
                    int byte_idx;
    
                    for (byte_idx=0; byte_idx<2; ++byte_idx) {
                        rlevel_bitmask[byte_idx] = octeon_read_dfm_ddr3_rlevel_dbg(byte_idx);
                        rlevel_rank_errors += validate_ddr3_rlevel_bitmask(rlevel_bitmask[byte_idx], tclk_psecs);
                }

                dfm_print("Rank(%d) Rlevel Debug Test Results  8:0        : %05x %05x\n",
                              rankx,
                              rlevel_bitmask[1],
                              rlevel_bitmask[0] 
                              );
                }
    
                dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));
                rlevel_scoreboard[rodt_ctl][rankx].setting = dfm_rlevel_rank.u64;

                {
                    int rlevel_byte[2];
                    int byte_idx;

                    rlevel_byte[1] = dfm_rlevel_rank.cn63xx.byte1;
                    rlevel_byte[0] = dfm_rlevel_rank.cn63xx.byte0;

                    /* Evaluate delay sequence with a sliding window
                       of three byte delays across the range of
                       bytes. */
                    if (0) {
                        /* Unbuffered dimm topology routes from end to end. */
                        for (byte_idx=0; byte_idx<2; ++byte_idx) {
                            rlevel_rank_errors += (nonsequential_delays(rlevel_byte[byte_idx+0],
                                                                        rlevel_byte[byte_idx+1],
                                                                        rlevel_byte[byte_idx+2])
                                                   ? RLEVEL_NONSEQUENTIAL_DELAY_ERROR : 0);
                        }
                    }
                }

                dfm_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d (%d)\n",
                          rankx,
                          dfm_rlevel_rank.cn63xx.status,
                          dfm_rlevel_rank.u64,
                          dfm_rlevel_rank.cn63xx.byte1,
                          dfm_rlevel_rank.cn63xx.byte0,
                          rlevel_rank_errors
                          );

                /* Save the score for this configuration */
                rlevel_scoreboard[rodt_ctl][rankx].score = rlevel_rank_errors;
                debug_print("rlevel_scoreboard[rodt_ctl=%d][rankx=%d].score:%d\n",
                          rodt_ctl, rankx, rlevel_scoreboard[rodt_ctl][rankx].score);

                rlevel_rodt_errors += rlevel_rank_errors;
            }

            /* If a configuration got a perfect score we're done */
            if (rlevel_rodt_errors == 0)
                break;

            /* Break loop if manual rodt_ctl setting is used. */
            if (override_rodt_ctl != (-1))
                break;
        }

        /* Re-enable dynamic compensation settings. */
        if (rlevel_comp_offset != 0) {
            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);

            dfm_comp_ctl2.s.ptune = 0;
            dfm_comp_ctl2.s.ntune = 0;
            dfm_comp_ctl2.s.byp = 0; /* Disable bypass mode */
            dfm_config_write_csr(CVMX_DFM_COMP_CTL2, dfm_comp_ctl2.u64);
            cvmx_read_csr(CVMX_DFM_COMP_CTL2); /* Read once */

            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2); /* Read again */
            dfm_print("DDR__PTUNE/DDR__PTUNE                         : %d/%d\n",
                      dfm_comp_ctl2.s.ddr__ptune, dfm_comp_ctl2.s.ddr__ntune);
        }
        } /* while(rlevel_loops--) */

        dfm_control.s.ddr2t           = save_ddr2t;
        dfm_config_write_csr(CVMX_DFM_CONTROL, dfm_control.u64);
        dfm_control.u64 = cvmx_read_csr(CVMX_DFM_CONTROL);
        dfm_print("DDR2T                                         : %6d\n", dfm_control.s.ddr2t); /* Display final 2T value */


        /* If no configuration got a perfect score pick the best one. */
        if (rlevel_rodt_errors != 0) {
            dfm_print("Evaluating Read-Leveling Scoreboard.\n");
            uint64_t best_setting[4] = {0};
            int      best_rodt_score = RLEVEL_UNDEFINED_ERROR;
            int      best_rodt_ctl = 0;
            int rodt_score;

            for (rodt_ctl=5; rodt_ctl>0; --rodt_ctl) {
                rodt_score = 0;
                for (rankx = 0; rankx < dimm_count * 2;rankx++) {
                    if (!(rank_mask & (1 << rankx)))
                        continue;
                    debug_print("rlevel_scoreboard[rodt_ctl=%d][rankx=%d].score:%d\n",
                              rodt_ctl, rankx, rlevel_scoreboard[rodt_ctl][rankx].score);
                    rodt_score += rlevel_scoreboard[rodt_ctl][rankx].score;
                }
                debug_print("rodt_score:%d, best_rodt_score:%d\n", rodt_score, best_rodt_score);
                if (rodt_score < best_rodt_score) {
                    best_rodt_score = rodt_score;
                    best_rodt_ctl   = rodt_ctl;

                    for (rankx = 0; rankx < dimm_count * 2;rankx++) {
                        if (!(rank_mask & (1 << rankx)))
                            continue;
                        best_setting[rankx]  = rlevel_scoreboard[rodt_ctl][rankx].setting;
                    }
                }
            }

            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
            dfm_comp_ctl2.s.rodt_ctl = best_rodt_ctl;
            dfm_config_write_csr(CVMX_DFM_COMP_CTL2, dfm_comp_ctl2.u64);
            dfm_comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
            dfm_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                      dfm_comp_ctl2.s.rodt_ctl, rodt_ohms[best_rodt_ctl]);

            for (rankx = 0; rankx < dimm_count * 2;rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;
                dfm_config_write_csr(CVMX_DFM_RLEVEL_RANKX(rankx), rlevel_scoreboard[best_rodt_ctl][rankx].setting);
                dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));
                dfm_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d (%d)\n",
                          rankx,
                          dfm_rlevel_rank.cn63xx.status,
                          dfm_rlevel_rank.u64,
                          dfm_rlevel_rank.cn63xx.byte1,
                          dfm_rlevel_rank.cn63xx.byte0,
                          rlevel_scoreboard[best_rodt_ctl][rankx].score
                          );
            }
        }

        for (rankx = 0; rankx < dimm_count * 2;rankx++) {
            char buf[20]  = {0};
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));

            for (i=0; i<9; ++i) {
                sprintf(buf, "dfm_rlevel_rank%d_byte%d", rankx, i);
                if ((s = dfm_getenv_debug(buf)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. %s = %llu\n", buf, value);

                    update_rlevel_rank_struct(&dfm_rlevel_rank, i, value);
                }
            }

            sprintf(buf, "dfm_rlevel_rank%d", rankx);
            if ((s = dfm_getenv_debug(buf)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                error_print("Parameter found in environment. %s = 0x%016llX\n", buf, (unsigned long long)value);
                dfm_rlevel_rank.u64 = value;
            }

            if (parameter_set) {
                dfm_config_write_csr(CVMX_DFM_RLEVEL_RANKX(rankx), dfm_rlevel_rank.u64);
                dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));
    
                dfm_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d\n",
                          rankx,
                          dfm_rlevel_rank.cn63xx.status,
                          dfm_rlevel_rank.u64,
                          dfm_rlevel_rank.cn63xx.byte1,
                          dfm_rlevel_rank.cn63xx.byte0
                          );
            }
        }
    }

    /*
     * 2. Without changing any other fields in DFM_CONFIG,
     *
     *    o write DFM_CONFIG[SEQUENCE] to select read-leveling, and
     *
     *    o write DFM_CONFIG[RANKMASK] = (1 << i), and
     *
     *    o write DFM_CONFIG[INIT_START] = 1
     *
     *    This initiates the previously-described read-leveling.
     *
     * 3. Wait until DFM_RLEVEL_RANKi[STATUS] != 2
     *
     *    DFM will have updated DFM_RLEVEL_RANKi[BYTE*] for all
     *    byte lanes at this point.
     *
     * 4. If desired, consult DFM_RLEVEL_DBG[BITMASK] and compare
     *    to DFM_RLEVEL_RANKi[BYTE*] for the lane selected by
     *    DFM_RLEVEL_CTL[BYTE]. If desired, modify
     *    DFM_RLEVEL_CTL[BYTE] to a new value and repeat so that
     *    all BITMASKs can be observed.
     *
     * 5. Initialize DFM_RLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    DFM_RLEVEL_RANKj = DFM_RLEVEL_RANKi.
     */

    {
        /* Try to determine/optimize write-level delays experimentally. */
        cvmx_dfm_wlevel_rankx_t dfm_wlevel_rank;
        cvmx_dfm_rlevel_rankx_t dfm_rlevel_rank;
        cvmx_dfm_modereg_params0_t dfm_modereg_params0;
        int byte;
        int delay;
        int rankx = 0;
        int active_rank;
        int sw_wlevel_offset = 1;
        int sw_wlevel_enable = 1;
        typedef enum {
            WL_ESTIMATED = 0,   /* HW/SW wleveling failed. Results
                                   estimated. */
            WL_HARDWARE  = 1,   /* H/W wleveling succeeded */
            WL_SOFTWARE  = 2,   /* S/W wleveling passed 2 contiguous
                                   settings. */
            WL_SOFTWARE1 = 3,   /* S/W wleveling passed 1 marginal
                                   setting. */
        } sw_wl_status_t;

        char *wl_status_strings[] = {
            "(e)",
            "   ",
            "   ",
            "(1)"
        };

        if ((s = dfm_getenv_debug("dfm_software_wlevel")) != NULL) {
            sw_wlevel_enable = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. dfm_software_wlevel = %d\n", sw_wlevel_enable);
        }

        if (sw_wlevel_enable)
            dfm_print("Performing software Write-Leveling\n");
            
        dfm_modereg_params0.u64 = cvmx_read_csr(CVMX_DFM_MODEREG_PARAMS0);

        /* We need to track absolute rank number, as well as how many
        ** active ranks we have.  Two single rank DIMMs show up as
        ** ranks 0 and 2, but only 2 ranks are active. */
        active_rank = 0;

        for (rankx = 0; rankx < dimm_count * 2;rankx++) {
            sw_wl_status_t byte_test_status[9] = {WL_ESTIMATED};
            sw_wl_status_t sw_wl_rank_status = WL_HARDWARE;
            int bytes_failed = 0;

            if (!sw_wlevel_enable)
                break;

            if (!(rank_mask & (1 << rankx)))
                continue;
    

            dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));
            if (wlevel_bitmask_errors == 0) {
                for (byte=0; byte<2; ++byte) {
                    /* If h/w write-leveling had no errors then use
                    ** s/w write-leveling to fixup only the upper bits
                    ** of the delays. */
                    for (delay=get_wlevel_rank_struct(&dfm_wlevel_rank, byte); delay<32; delay+=8) {
                        update_wlevel_rank_struct(&dfm_wlevel_rank, byte, delay);
                        debug_print("Testing byte %d delay %2d\n", byte, delay);
                        dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), dfm_wlevel_rank.u64);
                        dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));

                        /* Determine address of DRAM to test for software write leveling.  Adjust
                        ** address for boot bus hole in memory map. */
                        uint64_t rank_addr = active_rank * ((1ull << pbank_lsb)/num_ranks);
                        if (rank_addr > 0x10000000)
                            rank_addr += 0x10000000;
                        if (test_dfm_byte((uint64_t)0x8001370500000000ULL, 256, 16, byte) == 0) {
                            debug_print("        byte %d(0x%x) delay %2d Passed\n", byte, rank_addr, delay);
                            byte_test_status[byte] = WL_HARDWARE;
                            break;
                        } else {
                            debug_print("        byte %d delay %2d Errors\n", byte, delay);
                        }
                    }
                }
            }

            for (byte=0; byte<2; ++byte) {
                bytes_failed += (byte_test_status[byte]==WL_ESTIMATED);
            }

            if (bytes_failed) {
                sw_wl_rank_status = WL_SOFTWARE;
                /* If s/w fixups failed then retry using s/w write-leveling. */
                if (wlevel_bitmask_errors == 0) {
                    /* h/w succeeded but s/w fixups failed. So retry s/w. */
                    debug_print("Rank(%d) Retrying software Write-Leveling.\n", rankx);
                }
                for (byte=0; byte<2; ++byte) {
                    int passed = 0;
                    int wl_offset;
                    for (wl_offset = sw_wlevel_offset; wl_offset >= 0; --wl_offset) {
                    //for (delay=30; delay>=0; delay-=2) { /* Top-Down */
                    for (delay=0; delay<32; delay+=2) {  /* Bottom-UP */
                        update_wlevel_rank_struct(&dfm_wlevel_rank, byte, delay);
                        debug_print("Testing byte %d delay %2d\n", byte, delay);
                        dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), dfm_wlevel_rank.u64);
                        dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));

                        /* Determine address of DRAM to test for software write leveling.  Adjust
                        ** address for boot bus hole in memory map. */
                        uint64_t rank_addr = active_rank * ((1ull << pbank_lsb)/num_ranks);
                        if (rank_addr > 0x10000000)
                            rank_addr += 0x10000000;
                        if (test_dfm_byte((uint64_t)0x8001370500000000ULL, 256, 16, byte) == 0) {
                            ++passed;
                            if (passed == (1+wl_offset)) { /* Look for consecutive working settings */
                                debug_print("        byte %d(0x%x) delay %2d Passed\n", byte, rank_addr, delay);
                                if (wl_offset == 1) {
                                    byte_test_status[byte] = WL_SOFTWARE;
                                } else if (wl_offset == 0) {
                                    byte_test_status[byte] = WL_SOFTWARE1;
                                }
                                break;
                            }
                        } else {
                            debug_print("        byte %d delay %2d Errors\n", byte, delay);
                            passed = 0;
                        }
                    }
                    if (passed == (1+wl_offset)) { /* Look for consecutive working settings */
                        break;
                    }
                    }
                    if (passed == 0) {
                        /* Last resort. Use Rlevel settings to estimate
                           Wlevel if software write-leveling fails */
                        dfm_rlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_RLEVEL_RANKX(rankx));
                        rlevel_to_wlevel(&dfm_rlevel_rank, &dfm_wlevel_rank, byte);
                    } 
                }
            }
    
            dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), dfm_wlevel_rank.u64);
            dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));
    
            dfm_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %2d%3s %2d%3s %s\n",
                      rankx,
                      dfm_wlevel_rank.cn63xx.status,
                      dfm_wlevel_rank.u64,
                      dfm_wlevel_rank.cn63xx.byte1, wl_status_strings[byte_test_status[1]],
                      dfm_wlevel_rank.cn63xx.byte0, wl_status_strings[byte_test_status[0]],
                      (sw_wl_rank_status == WL_HARDWARE) ? "" : "(s)"
                      );

            active_rank++;
        }


        for (rankx = 0; rankx < dimm_count * 2;rankx++) {
            char buf[20]  = {0};
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));

            for (i=0; i<9; ++i) {
                sprintf(buf, "dfm_wlevel_rank%d_byte%d", rankx, i);
                if ((s = dfm_getenv_debug(buf)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);
                    error_print("Parameter found in environment. %s = %llu\n", buf, value);

                    update_wlevel_rank_struct(&dfm_wlevel_rank, i, value);
                }
            }

            sprintf(buf, "dfm_wlevel_rank%d", rankx);
            if ((s = dfm_getenv_debug(buf)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                error_print("Parameter found in environment. %s = 0x%016llX\n", buf, (unsigned long long)value);
                dfm_wlevel_rank.u64 = value;
            }

            if (parameter_set) {
                dfm_config_write_csr(CVMX_DFM_WLEVEL_RANKX(rankx), dfm_wlevel_rank.u64);
                dfm_wlevel_rank.u64 = cvmx_read_csr(CVMX_DFM_WLEVEL_RANKX(rankx));
    
                dfm_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d\n",
                          rankx,
                          dfm_wlevel_rank.cn63xx.status,
                          dfm_wlevel_rank.u64,
                          dfm_wlevel_rank.cn63xx.byte1,
                          dfm_wlevel_rank.cn63xx.byte0
                          );
            }
        }
    }

    /*
     * 4.8.8 Final DFM Initialization
     *
     * Early DFM initialization, DFM write-leveling, and DFM
     * read-leveling must be completed prior to starting this
     * final DFM initialization.
     *
     * DFM hardware updates the DFM_SLOT_CTL0, DFM_SLOT_CTL1,
     * DFM_SLOT_CTL2 CSRs with minimum values based on the
     * selected read-leveling and write-leveling
     * settings. Software should not write the final
     * DFM_SLOT_CTL0, DFM_SLOT_CTL1, and DFM_SLOT_CTL2 values
     * until after the final readleveling and write-leveling
     * settings are written.
     *
     * Software must ensure the DFM_SLOT_CTL0, DFM_SLOT_CTL1,
     * and DFM_SLOT_CTL2 CSR values are appropriate for this
     * step. These CSRs select the minimum gaps between reads and
     * writes of various types.
     *
     * Software must not reduce the values in these CSR fields
     * below the values previously selected by the DFM hardware
     * (during write-leveling and read-leveling steps above).
     *
     * All sections in this chapter may be used to derive proper
     * settings for these registers.
     */

    if (safe_dfm_flag) {
        cvmx_dfm_slot_ctl0_t dfm_slot_ctl0;
        cvmx_dfm_slot_ctl1_t dfm_slot_ctl1;

        dfm_slot_ctl0.u64 = 0;
        dfm_slot_ctl0.cn63xx.r2r_init = 0x3f;
        dfm_slot_ctl0.cn63xx.r2w_init = 0x3f;
        dfm_slot_ctl0.cn63xx.w2r_init = 0x3f;
        dfm_slot_ctl0.cn63xx.w2w_init = 0x3f;
        dfm_config_write_csr(CVMX_DFM_SLOT_CTL0, dfm_slot_ctl0.u64);

        dfm_slot_ctl1.u64 = 0;
        dfm_slot_ctl1.cn63xx.r2r_xrank_init = 0x3f;
        dfm_slot_ctl1.cn63xx.r2w_xrank_init = 0x3f;
        dfm_slot_ctl1.cn63xx.w2r_xrank_init = 0x3f;
        dfm_slot_ctl1.cn63xx.w2w_xrank_init = 0x3f;
        dfm_config_write_csr(CVMX_DFM_SLOT_CTL1, dfm_slot_ctl1.u64);
    }

    error_print("DFM interface initialized %d MB\n", mem_size_mbytes);
    return(mem_size_mbytes);
}


static void set_dfm_clock_initialized(int inited_flag)
{
#ifdef __U_BOOT__
    if (inited_flag)
        gd->flags |= (GD_FLG_DFM_CLK_INITIALIZED);
    else
        gd->flags &= ~(GD_FLG_DFM_CLK_INITIALIZED);
#else
    if (inited_flag)
        global_dfm_clock_initialized |= 0x1;
    else
        global_dfm_clock_initialized &= ~0x1;
#endif

}
static int dfm_clock_initialized(void)
{
#ifdef __U_BOOT__
    return (!!(gd->flags & (GD_FLG_DFM_CLK_INITIALIZED)));
#else
    return (!!(global_dfm_clock_initialized & 0x1));
#endif
}


int initialize_dfm_clock(uint32_t cpu_id,
                         uint32_t cpu_hertz,
                         uint64_t dfm_hertz
                         )
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

#ifdef __U_BOOT__
    {
        if (getenv("dfm_verbose") != NULL)
        {
            gd->flags |= GD_FLG_DFM_VERBOSE;
        }
        if (getenv("dfm_trace_init") != NULL)
        {
            gd->flags |= GD_FLG_DFM_TRACE_INIT;
            gd->flags |= GD_FLG_DFM_VERBOSE;
        }

        /* Force dfm_verbose for failsafe debugger */
        if (gd->flags & GD_FLG_FAILSAFE_MODE)
            gd->flags |= GD_FLG_DFM_VERBOSE;

#ifdef DFM_DEBUG
        /* Keep verbose on while we are still debugging. */
        gd->flags |= GD_FLG_DFM_VERBOSE;
#endif

    }
#endif
    if (dfm_clock_initialized())
        return 0;

    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX)) {
        cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;
        cvmx_dfm_dll_ctl2_t dll_ctl2;
        cvmx_dfm_reset_ctl_t dfm_reset_ctl;


        {

            /* Scale the DFM clock to meet the DFM DRAM
               specifications.  Since the configuration is constrained
               by the DDR configuration the DFM clock could be slower
               than optimal. */

            uint64_t clkf;
            uint64_t pll_MHz;
            uint64_t calculated_dfm_hertz;
            unsigned en, ps;
            static const unsigned _en[] = {1, 2, 3, 4, 6, 8, 12};

            dfm_print("Setting DFM Clock %ld hertz\n", dfm_hertz);

            ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

            clkf = ddr_pll_ctl.cn63xx.clkf;
            pll_MHz = clkf * 50;

            ps = divide_roundup((pll_MHz * 1000000), dfm_hertz);
            for (en=0; en<sizeof(_en)/sizeof(unsigned); ++en) {
                if (_en[en] >= ps)
                    break;
            }
            en = min(en, sizeof(_en)/sizeof(unsigned));
            ddr_pll_ctl.cn63xx.dfm_ps_en = en;

            cvmx_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);

            calculated_dfm_hertz = (pll_MHz * 1000000) / _en[en];

#if defined(__U_BOOT__)
            dfm_print("en: %2d, clkf: %4lu, pll_MHz: %4lu, dfm_hertz: %8lu\n",
                      _en[en], clkf, pll_MHz, calculated_dfm_hertz);
#else
            dfm_print("en: %2d, clkf: %4llu, pll_MHz: %4llu, dfm_hertz: %8llu\n",
                      _en[en], clkf, pll_MHz, calculated_dfm_hertz);
#endif
        }


        /*
         * 4.8.2 DFM CK Initialization
         *
         * DDR PLL initialization must be completed prior to starting
         * DFM CK initialization.
         *
         * 1. Without changing any other DFM_DFM_PLL_CTL values,
         *    write DFM_DFM_PLL_CTL[DFM_DIV_RESET] = 1 and
         *    DFM_DFM_PLL_CTL [DFM_PS_EN] with the appropriate value
         *    to get the desired DFM CK speed. Section Section 4.13
         *    discusses CLKF and DFM_PS_EN programmings.
         *
         *    The DFM_DFM_PLL_CTL[DFM_PS_EN] must not change after
         *    this point without restarting this DFM CK initialization
         *    sequence.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));
        ddr_pll_ctl.cn63xx.dfm_div_reset = 1;
        dfm_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);


        /*
         * 2. Without changing any other DFM_COMP_CTL2 values, write
         *    DFM_COMP_CTL2[CK_CTL] to the desired DFM_CK_*_P/
         *    DFM_DIMM*_CS*_L/DFM_DIMM*_ODT_* drive strength.
         */

        {
            cvmx_dfm_comp_ctl2_t comp_ctl2;
            comp_ctl2.u64 = cvmx_read_csr(CVMX_DFM_COMP_CTL2);
            comp_ctl2.s.dqx_ctl            = 0x4; /* 34.3 ohm */
            comp_ctl2.s.ck_ctl             = 0x4; /* 34.3 ohm */
            comp_ctl2.s.cmd_ctl            = 0x4; /* 34.3 ohm */
            comp_ctl2.s.rodt_ctl           = 0x4; /* 60 ohm */

            if (((s = dfm_getenv_debug("dfm_clk_ctl")) != NULL) || ((s = dfm_getenv_debug("dfm_ck_ctl")) != NULL)) {
                comp_ctl2.s.ck_ctl  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. dfm_clk_ctl = 0x%x\n", comp_ctl2.s.ck_ctl);
            }

            if ((s = dfm_getenv_debug("dfm_cmd_ctl")) != NULL) {
                comp_ctl2.s.cmd_ctl  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. dfm_cmd_ctl = 0x%x\n", comp_ctl2.s.cmd_ctl);
            }

            if ((s = dfm_getenv_debug("dfm_dqx_ctl")) != NULL) {
                comp_ctl2.s.dqx_ctl  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. dfm_dqx_ctl = 0x%x\n", comp_ctl2.s.dqx_ctl);
            }

            dfm_config_write_csr(CVMX_DFM_COMP_CTL2, comp_ctl2.u64);
        }

        /*
         * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

        /*
         * 4. Wait 200 ns.
         */

        cvmx_wait_usec(200000);          /* Wait 200 ns */

        /*
         * 5. Without changing any other LMC0_DDR_PLL_CTL values,
         *    write LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
         */

        ddr_pll_ctl.cn63xx.dfm_div_reset = 0;

        dfm_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);

        /*
         * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

        /*
         * 7. Wait 200 ns.
         */

        cvmx_wait_usec(1);      /* Wait 200 ns */

        /*
         * 4.8.3 DFM DRESET Initialization
         *
         * Both DDR PLL and DFM CK initializations must be completed
         * prior to starting this DFM DRESET initialization.
         *
         * 1. If not done already, write DFM_DLL_CTL2 to its reset
         *    value, including DFM_DLL_CTL2[DRESET] = 1.
         */

        /* Already done. */

        /*
         * 2. Without changing any other DFM_DLL_CTL2 fields, write
         *    DFM_DLL_CTL2 [DLL_BRINGUP] = 1.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL2);
        dll_ctl2.cn63xx.dll_bringup = 1;
        dfm_config_write_csr(CVMX_DFM_DLL_CTL2, dll_ctl2.u64);

        /*
         * 3. Read DFM_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_DFM_DLL_CTL2);

        /*
         * 4. Wait for 10 DFM CK cycles.
         */

        cvmx_wait_usec(1);

        /*
         * 5. Without changing any other fields in DFM_DLL_CTL2, write
         *
         *    DFM_DLL_CTL2[QUAD_DLL_ENA] = 1.
         *
         *    DFM_DLL_CTL2[QUAD_DLL_ENA] must not change after this
         *    point without restarting the DFM DRESET initialization
         *    sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL2);
        dll_ctl2.cn63xx.quad_dll_ena = 1;
        dfm_config_write_csr(CVMX_DFM_DLL_CTL2, dll_ctl2.u64);

        /*
         * 6. Read DFM_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_DFM_DLL_CTL2);

        /*
         * 7. Wait 10 usec.
         */

        cvmx_wait_usec(10);

        /*
         * 8. Without changing any other fields in DFM_DLL_CTL2, write
         *
         *    DFM_DLL_CTL2[DLL_BRINGUP] = 0.
         *
         *    DFM_DLL_CTL2[DLL_BRINGUP] must not change after this
         *    point without restarting the DFM DRESET initialization
         *    sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL2);
        dll_ctl2.cn63xx.dll_bringup = 0;
        dfm_config_write_csr(CVMX_DFM_DLL_CTL2, dll_ctl2.u64);

        /*
         * 9. Read DFM_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_DFM_DLL_CTL2);

        /*
         * 10. Without changing any other fields in DFM_DLL_CTL2, write
         *
         *     DFM_DLL_CTL2[DRESET] = 0.
         *
         *     DFM_DLL_CTL2[DRESET] must not change after this point
         *     without restarting the DFM DRESET initialization
         *     sequence.
         *
         *     After completing DFM DRESET initialization, all DFM
         *     CSRs may be accessed. Prior to completing DFM DRESET
         *     initialization, only DFM_DFM_PLL_CTL, DFM_DLL_CTL2,
         *     DFM_RESET_CTL, and DFM_COMP_CTL2 DFM CSRs can be
         *     accessed.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_DFM_DLL_CTL2);
        dll_ctl2.cn63xx.dreset = 0;
        dfm_config_write_csr(CVMX_DFM_DLL_CTL2, dll_ctl2.u64);

        /*
         * 4.8.4 DFM RESET Initialization
         *
         * The purpose of this step is to assert/deassert the RESET#
         * pin at the DDR3 parts.
         *
         * It may be appropriate to skip this step if the DDR3 DRAM
         * parts are in self refresh and are currently preserving
         * their contents. (Software can determine this via
         * DFM_RESET_CTL[DDR3PSV] in some circumstances.)  The
         * remainder of this section assumes that the DRAM contents
         * need not be preserved.
         *
         * The remainder of this section assumes that the CN63XX
         * DFM_RESET_L pin is attached to the RESET# pin of the
         * attached DDR3 parts, as will be appropriate in many
         * systems.
         *
         * (In other systems, such as ones that can preserve DDR3 part
         * contents while CN63XX is powered down, it will not be
         * appropriate to directly attach the CN63XX DFM_RESET_L pin
         * to DRESET# of the DDR3 parts, and this section may not
         * apply.)
         *
         * DRAM Controller (DFM): DFM Initialization Sequence
         *
         * 1. If not done already, assert DFM_RESET_L pin by writing
         *    DFM_RESET_CTL[DDR3RST] = 0 without modifying any other
         *    DFM_RESET_CTL fields.
         */

        dfm_reset_ctl.u64 = cvmx_read_csr(CVMX_DFM_RESET_CTL);
        if (dfm_reset_ctl.cn63xx.ddr3psv == 0) {
            /* Contents are not being preserved */
            dfm_reset_ctl.cn63xx.ddr3rst = 0;
            dfm_config_write_csr(CVMX_DFM_RESET_CTL, dfm_reset_ctl.u64);

            /*
             * 2. Read DFM_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_DFM_RESET_CTL);

            /*
             * 3. Wait until RESET# assertion time requirement from JEDEC
             *    DDR3 specification is satisfied (200 usec during a
             *    power-on ramp, 100ns when power is already stable).
             */

            cvmx_wait_usec(200);

            /*
             * 4. Deassert DFM_RESET_L pin by writing
             *    DFM_RESET_CTL[DDR3RST] = 1 without modifying any other
             *    DFM_RESET_CTL fields.
             */

            dfm_reset_ctl.u64 = cvmx_read_csr(CVMX_DFM_RESET_CTL);
            dfm_reset_ctl.cn63xx.ddr3rst = 1;
            dfm_config_write_csr(CVMX_DFM_RESET_CTL, dfm_reset_ctl.u64);

            /*
             * 5. Read DFM_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_DFM_RESET_CTL);
        }

        /*
         * (Subsequent RESET# deassertion to CKE assertion requirement
         * can be satisfied during DFM initialization via
         * DFM_TIMING_PARAMS0[TCKEON] timing.)
         */

    }

    set_dfm_clock_initialized(1);
    return(0);
}

static void octeon_ipd_delay_cycles(uint64_t cycles)
{
    uint64_t start = cvmx_read_csr(CVMX_IPD_CLK_COUNT);
    while (start + cycles > cvmx_read_csr(CVMX_IPD_CLK_COUNT))
        ;
}

uint32_t measure_octeon_dfm_clock(uint32_t cpu_id,
                                  uint32_t cpu_hertz,
                                  uint64_t dfm_hertz
                                  )
{
    uint64_t core_clocks;
    uint64_t dfm_clocks;
    uint64_t calc_dfm_hertz = 0;

    if (initialize_dfm_clock(cpu_id, cpu_hertz, dfm_hertz) != 0)
        return (0);

    /* Dynamically determine the DDR clock speed */
    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX)) {
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT);
        dfm_clocks = cvmx_read_csr(CVMX_DFM_FCLK_CNT);
        octeon_ipd_delay_cycles(100000000); /* How many cpu cycles to measure over */
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT) - core_clocks;
        dfm_clocks = cvmx_read_csr(CVMX_DFM_FCLK_CNT) - dfm_clocks;
        calc_dfm_hertz = dfm_clocks * cvmx_clock_get_rate(CVMX_CLOCK_IPD) / core_clocks;
    }

    dfm_print("Measured DFM clock %d\n", calc_dfm_hertz);

#ifdef CVMX_BUILD_FOR_UBOOT
    /* Check for unreasonable settings. */
    if (calc_dfm_hertz < 10000) {
        error_print("DFM clock misconfigured.\n");
    }
#else
    /* Check for unreasonable settings. */
    if (calc_dfm_hertz == 0) {
        error_print("DFM clock misconfigured. Exiting.\n");
        exit(1);
    }
#endif
    return (calc_dfm_hertz);
}

#endif  /* OCTEON_CONFIG_DFM */
