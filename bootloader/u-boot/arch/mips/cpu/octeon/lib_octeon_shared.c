/***********************license start************************************
 * Copyright (c) 2004-2013 Cavium Inc. (support@cavium.com). All rights
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

/* $Revision: 167267 $ */

#if defined(__U_BOOT__)
/* If necessary define DDR_VERBOSE to enable verbose output at compile
** time. The ddr_verbose environment variable can also be used at
** runtime to debug the DDR interface.  This also enables the output
** necessary for review by Cavium Inc., Inc.
*/

#include <common.h>
#include <i2c.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-l2c.h>
#include <asm/arch/cvmx-swap.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/gpio.h>
#include <linux/ctype.h>
#include <cvmx-rng.h>
#else
#ifdef __unix__
# include <ctype.h>
#endif
/* Building on host for PCI boot
** Always use the pass 2 definitions for this module.
** Assume that pass 2 is a superset of pass 1 and
** determine actual chip revision at run time. */
#undef  OCTEON_MODEL
#define OCTEON_MODEL OCTEON_CN38XX_PASS2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef BDK
#include "bdk_lib_octeon_shared.h"
extern int isspace(int);
#endif
#include "cvmx.h"
#include "cvmx-twsi.h"
#include "cvmx-l2c.h"
#include "cvmx-gpio.h"
#include "cvmx-swap.h"
#include "cvmx-rng.h"
#include "lib_octeon_shared.h"


#define simple_strtoul  strtoul
#define simple_strtoull strtoul
#define simple_strtol  strtol
#endif

#ifndef BDK
#include "cvmx-version.h"
#endif

#ifdef __U_BOOT__
# ifdef CONFIG_OCTEON_OCX
#  define cvmx_read_csr(c) cvmx_read_csr_node(gd->arch.dram_init_node, c)
#  define cvmx_write_csr(c, v) cvmx_write_csr_node(gd->arch.dram_init_node, c, v)
# endif
#else
# define cvmx_read_csr(c) cvmx_read_csr_node(cvmx_get_node_num(), c)
# define cvmx_write_csr(c, v) cvmx_write_csr_node(cvmx_get_node_num(), c, v)
#endif

#include "configs/octeon_ebt3000_shared.h"
#include "configs/octeon_ebh3100_shared.h"
#include "configs/octeon_ebh5200_shared.h"
#include "configs/octeon_ebh5600_shared.h"
#include "configs/octeon_ebb5600_shared.h"
#include "configs/octeon_ebb6300_shared.h"
#include "configs/octeon_ebb6100_shared.h"
#include "configs/octeon_sff6100_shared.h"
#include "configs/octeon_ep6300c_shared.h"
#include "configs/octeon_ebb6800_shared.h"
#include "configs/octeon_ebb6600_shared.h"
#include "configs/octeon_evb7100_shared.h"
#include "configs/octeon_evb7000_shared.h"
#include "configs/octeon_evb7000_sff_shared.h"
#include "configs/octeon_ebb7800_shared.h"
#include "configs/octeon_ebb7304_shared.h"
#include "configs/octeon_ebb7504_shared.h"
#include "configs/octeon_ebb7500_shared.h"
#include "configs/octeon_cnf7300_shared.h"
#include "configs/octeon_nic73_shared.h"
#include "configs/octeon_nic25e_shared.h"
#include "configs/octeon_nic225e_shared.h"
#include "configs/octeon_ebb7804_shared.h"
#include "configs/octeon_tb7000_shared.h"
#include "configs/octeon_sff7000_shared.h"
#include "configs/octeon_tb7600_shared.h"
#include "configs/octeon_thunder_shared.h"
#include "configs/octeon_ebt5800_shared.h"
#include "configs/octeon_nicpro2_shared.h"
#include "configs/octeon_nic_xle_4g_shared.h"
#include "configs/octeon_bbgw_ref_shared.h"
#include "configs/octeon_cn3010_evb_hs5_shared.h"
#include "configs/octeon_nic10e_shared.h"
#include "configs/octeon_nic10e_66_shared.h"
#include "configs/octeon_nic2e_shared.h"
#include "configs/octeon_nic68_4_shared.h"
#include "configs/octeon_swordfish_shared.h"
#include "configs/octeon_nac38_shared.h"
#include "configs/octeon_wsx16_shared.h"
#include "configs/octeon_snic10e_shared.h"
#include "configs/octeon_snic10e_61_shared.h"
#include "configs/octeon_iw_evb_shared.h"
#include "configs/octeon_cnf71xx_ref_shared.h"
#include "configs/octeon_moonshot_shared.h"
#include "configs/octeon_clark_shared.h"
#include "configs/octeon_eap7000_ref_shared.h"
#include "configs/octeon_wave2_shared.h"
#include "configs/octeon_nas7000_ref_shared.h"
#include "configs/octeon_router7000_ref_shared.h"
#include "configs/octeon_nic210nvg_shared.h"
#include "configs/octeon_nic401nvg_shared.h"
#include "configs/octeon_sff7800_shared.h"
#include "configs/octeon_niagara83x_shared.h"
#include "configs/octeon_rainier_shared.h"
#include "configs/octeon_nicx40e_shared.h"
#include "configs/octeon_copperhead_shared.h"
#ifdef __U_BOOT__
# include <asm/arch/octeon_eeprom_types.h>
#else
# include "octeon_eeprom_types.h"
#endif

#ifdef __U_BOOT__
DECLARE_GLOBAL_DATA_PTR;
#endif

#define ENABLE_PRINT_CURSP 0
#if ENABLE_PRINT_CURSP
static inline void *curSP(void)
{
    uint64_t ret;

    __asm__ __volatile__(
        "move  %0,$sp"
        : "=r" (ret) : );

    return ret;
}
#define PRINT_CURSP(label) printf("CURSP @ %s: %p\n", label, curSP())
#else
#define PRINT_CURSP(label) do {} while (0)
#endif

#ifndef __U_BOOT__
int global_ddr_clock_initialized = 0;
int global_ddr_memory_preserved  = 0;
#endif

/* Make a define similar to OCTEON_IS_MODEL that will work in this context.  This is runtime only.
** We can't do the normal runtime here since this code can run on the host.
** All functions using this must have a cpu_id variable set to the correct value */
#define octeon_is_cpuid(x)   (__OCTEON_IS_MODEL_COMPILE__(x, cpu_id))

/**
 * Disable debugging support for the generic bootloaders in order to make
 * the bootloader fit into L2 cache.
 */
#if defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)		|| \
    defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
# define DDR_NO_DEBUG
#endif
#ifndef CONFIG_OCTEON_DISABLE_DDR2
static int init_octeon_ddr2_interface(uint32_t cpu_id,
                                      const ddr_configuration_t *ddr_configuration,
                                      uint32_t ddr_hertz,
                                      uint32_t cpu_hertz,
                                      uint32_t ddr_ref_hertz,
                                      int board_type,
                                      int board_rev_maj,
                                      int board_rev_min,
                                      int ddr_interface_num,
                                      uint32_t ddr_interface_mask
                                      );
#endif
#ifndef CONFIG_OCTEON_DISABLE_DDR3
static int init_octeon_ddr3_interface(uint32_t cpu_id,
				      const ddr_configuration_t *ddr_configuration,
				      uint32_t ddr_hertz,
				      uint32_t cpu_hertz,
				      uint32_t ddr_ref_hertz,
				      int board_type,
				      int board_rev_maj,
				      int board_rev_min,
				      int ddr_interface_num,
				      uint32_t ddr_interface_mask);

static int init_octeon3_ddr3_interface(uint32_t cpu_id,
                                       const ddr_configuration_t *ddr_configuration,
                                       uint32_t ddr_hertz,
                                       uint32_t cpu_hertz,
                                       uint32_t ddr_ref_hertz,
                                       int board_type,
                                       int board_rev_maj,
                                       int board_rev_min,
                                       int ddr_interface_num,
                                       uint32_t ddr_interface_mask,
                                       int node);
#endif

#ifdef __U_BOOT__
# ifndef DDR_NO_DEBUG
static inline int ddr_verbose(void)
{
    return !!(gd->flags & GD_FLG_DDR_VERBOSE);
}

static inline int ddr_trace_init(void)
{
    return !!(gd->flags & GD_FLG_DDR_TRACE_INIT);
}
#else
static inline int ddr_verbose(void)
{
    return 0;
}

static inline int ddr_trace_init(void)
{
    return 0;
}
#endif

# ifndef CONFIG_OCTEON_DISABLE_DDR2
static char *getenv_wrapper(const char *name)
{
    int i = 0;

    /* This buffer must be in scratch, as DRAM is not initialized yet. */
    char *input = (char *)(gd->arch.tmp_str);
    memset(input, 0, GD_TMP_STR_SIZE);

    if (gd->flags & GD_FLG_DDR_PROMPT) {
        printf("Please enter value for %s DDR param(enter for default):",
               name);
        while (i < GD_TMP_STR_SIZE) {
            input[i] = getc();

            switch (input[i]) {
                /* Enter ends the input */
            case '\r':
            case '\n':
                input[i] = '\0';
                goto gw_done;
                break;

                /* Handle backspace for interactive use */
            case 0x08:
            case 0x7F:
                putc(0x08);
                putc(' ');
                putc(0x08);
                if (i > 0)
                    i--;
                continue;
                break;
            }
            putc(input[i]);
            i++;
        }
        gw_done:
        if (i == 0) {
            printf("\nNo value entered, using default value for parameter %s\n",
                   name);
            return NULL;
        } else {
            printf("\nUsing value %s for parameter %s\n",
                   input, name);
            return input;
        }
    } else {
        return getenv(name);
    }
}
# endif

/* The ddr_verbose environment variable no longer enables environment
 * checking which slows down the boot process.  The ddr_debug
 * environment variable is now required to enable checking for the
 * various environment variables used for testing and debug.
 */
static inline char *ddr_getenv_debug(char *name)
{
#ifndef DDR_NO_DEBUG
    if (gd->flags & GD_FLG_FAILSAFE_MODE)
        return NULL;

    if (gd->flags & GD_FLG_DDR_DEBUG)
        return getenv(name);
#endif
    return NULL;
}

#else
static inline char *getenv_wrapper(char *name)
{
    return getenv(name);
}

int ddr_verbose(void)
{
    return getenv("ddr_verbose") != NULL;
}

static int ddr_trace_init(void)
{
    return getenv("ddr_trace_init") != NULL;
}

static inline char *ddr_getenv_debug(char *name)
{
    return getenv(name);
}
#endif  /* __U_BOOT__ */

#define DEBUG_DEFINED (0)
#define debug_print(fmt, args...) do { } while (0)



#ifdef DEBUG_SPD
static const char *ddr2_spd_strings[];
#endif

#ifdef DDR_NO_DEBUG
# define ddr_print(fmt, args...) do { } while (0)
#else
static void ddr_print(const char *format, ...)
{
    if (ddr_verbose() || (DEBUG_DEFINED)) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
#endif

static void ddr_config_write_csr(uint64_t csr_addr, uint64_t val)
{
    if (ddr_trace_init())
        ddr_print("DDR Config %016llx => %016llx\n", csr_addr, val);

    cvmx_write_csr(csr_addr, val);
}

static void ddr_config_write_csr_node(int node,
					     uint64_t csr_addr, uint64_t val)
{
    if (ddr_trace_init())
        ddr_print("DDR Config %016llx => %016llx\n", csr_addr, val);

    cvmx_write_csr_node(node, csr_addr, val);
}

#define error_print  printf


extern void octeon_delay_cycles(uint64_t cycles);

#undef min
#define min(X, Y)				\
    ({ typeof (X) __x = (X);                    \
        typeof (Y) __y = (Y);                   \
        (__x < __y) ? __x : __y; })

#undef max
#define max(X, Y)                                       \
    ({ typeof (X) __x = (X); typeof(Y) __y = (Y);	\
        (__x > __y) ? __x : __y; })

/*
 *
 * +---------+----+----------------------+---------------+--------+------+-----+
 * |  Dimm   |Rank|         Row          |      Col      |  Bank  |  Col | Bus |
 * +---------+----+----------------------+---------------+--------+------+-----+
 *          |   |                       |                    |
 *          |   |                       |                    |
 *          |  1 bit           LMC_MEM_CFG0[ROW_LSB]         |
 *          |                                                |
 *   LMC_MEM_CFG0[PBANK_LSB]                       (2 + LMC_DDR2_CTL[BANK8]) bits
 *
 *    Bus     = Selects the byte on the 72/144-bit DDR2 bus
 *         CN38XX: Bus = (3 + LMC_CTL[MODE128b]) bits
 *         CN31XX: Bus = (3 - LMC_CTL[MODE32b]) bits
 *         CN30XX: Bus = (1 + LMC_CTL[MODE32b]) bits
 *         CN58XX: Bus =
 *         CN56XX: Bus =
 *         CN50XX: Bus =
 *    Col     = Column Address for the DDR2 part
 *    Bank    = Bank Address for the DDR2 part
 *    Row     = Row Address for the DDR2 part
 *    Rank    = Optional Rank Address for dual-rank DIMM's
 *              (present when LMC_CTL[BUNK_ENABLE] is set)
 *    DIMM    = Optional DIMM address - 0, 1,or 2 bits
 */

#ifdef STATIC_DRAM_CONFIG_TABLE
extern const unsigned char STATIC_DRAM_CONFIG_TABLE [];
#endif

#ifdef CVMX_BUILD_FOR_UBOOT
static inline void config_gpio_output_pin(int pin)
{
    gpio_direction_output(pin, 1);
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR3
# ifdef CVMX_BUILD_FOR_UBOOT
static void pulse_gpio_pin(int pin, int usecs)
{
    static int configured_pin = 0;

    if (!(configured_pin & 1<<pin)) {
        configured_pin |= 1<<pin;
        gpio_direction_output(pin, 0);
    } else {
        gpio_set_value(pin, 0);
    }
    udelay(usecs);
    gpio_set_value(pin, 1);
}

# else
static inline void pulse_gpio_pin(int pin, int usecs)
{
}
# endif
#endif

/**
 * Wait for the specified number of DRAM clock cycles
 *
 * @param cycles
 */
static inline void cvmx_wait_dclk(uint64_t cycles)
{
    uint64_t done = cvmx_clock_get_count(CVMX_CLOCK_DDR) + cycles;

    while (cvmx_clock_get_count(CVMX_CLOCK_DDR) < done)
        ; /* Spin */
}

#ifndef DDR_NO_DEBUG
static char * lookup_env_parameter(const char *format, ...)
{
    char *s;
    unsigned long value;
    va_list args;
    char buffer[64];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if ((s = ddr_getenv_debug(buffer)) != NULL) {
        value = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. %s = \"%s\" 0x%lx (%ld)\n", buffer, s, value, value);
    }
    return s;
}

#ifndef CONFIG_OCTEON_DISABLE_DDR3
static char * lookup_env_parameter_ull(const char *format, ...)
{
    char *s;
    unsigned long long value;
    va_list args;
    char buffer[64];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if ((s = ddr_getenv_debug(buffer)) != NULL) {
        value = simple_strtoull(s, NULL, 0);
        error_print("Parameter found in environment. %s = 0x%016llx\n",
                    buffer, value);
    }
    return s;
}
#endif
#else
static inline char *lookup_env_parameter(const char *format, ...)
{
    return NULL;
}

static inline char * lookup_env_parameter_ull(const char *format, ...)
{
    return NULL;
}
#endif

/* Read an DIMM SPD value, either using TWSI to read it from the DIMM, or
 * from a provided array.
 */
int read_spd(const dimm_config_t *dimm_config, int dimm_index,
		    int spd_field)
{
    dimm_index = !!dimm_index;
    /* If pointer to data is provided, use it, otherwise read from SPD over twsi */
    if (dimm_config->spd_ptrs[dimm_index]) {
        return dimm_config->spd_ptrs[dimm_index][spd_field];
    } else if (dimm_config->spd_addrs[dimm_index]) {
	uint8_t busno = OCTEON_TWSI_GET_BUS(dimm_config->spd_addrs[dimm_index]);
        uint8_t cmdno = dimm_config->spd_addrs[dimm_index];
#ifdef __U_BOOT__
        uint8_t dummy[2];
	int data;
#else
	uint8_t data;
#endif
        int bank = 0;

        /* for DDR4, the part number/serial number are in the bank 1 data
	 * (offsets > 255)
	 */
        if (OCTEON_IS_OCTEON3() && spd_field >= 256) { /* DDR4 bank 1!!! */
            bank = 1;
            spd_field %= 256;
        }
#ifdef __U_BOOT__
        if (i2c_set_bus_num(busno)) {
            error_print("TWSI bus for dimm %d not available\n", dimm_index);
            return -1;
        }
        if (bank == 1)
            i2c_write(0x37, 0, 2, dummy, 1); // this command should switch to bank 1
        if (i2c_read(cmdno & 0x7f, spd_field, 1, (uint8_t *) &dummy[0], 1))
            data = -1;
	else
	    data = dummy[0];
        if (bank == 1)
            i2c_write(0x36, 0, 2, dummy, 1); // this command should switch back to bank 0
        return data;
#else
        if (bank == 1)
            cvmx_twsix_write_ia(busno, 0x37, 0, 2, 1, 0); // SBA1 - set bank address to bank 1
        data = cvmx_twsix_read_ia8(busno, cmdno, spd_field, 1);
        if (bank == 1)
            cvmx_twsix_write_ia(busno, 0x36, 0, 2, 1, 0); // SBA0 - set bank address to bank 0
        return data;
#endif
    }
    return -1;
}

#ifndef CONFIG_OCTEON_DISABLE_DDR2
static int validate_spd_checksum_ddr2(int twsi_addr, int silent)
{
    int i;
    uint16_t sum = 0;
    uint8_t csum;
    int ff_cnt = 0;
#ifdef __U_BOOT__
    uint8_t buffer[64];
    i2c_set_bus_num(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK));
    if (i2c_read(twsi_addr, 0, 1, buffer, sizeof(buffer)))
        /* TWSI read error */
        return 0;
    for (i = 0; i < 63; i++) {
        sum += buffer[i];
        /* Sometimes the tuple eeprom gets a correct SPD checksum, even
         * though most of it reads back 0xff due to different IA size.
         */
        if (buffer[i] == 0xff && ++ff_cnt > 20)
            return 0;

        /* Bail out early if it looks blank, as this is slow over ejtag.
         * If first 5 values are all 0xff, it is not a valid ddr2 SPD.
         */
        if (sum == (5*0xff) && i == 4)
            return 0;
    }
    csum = buffer[63];
    if (sum == 0xff * 63) {
        if (!silent)
            printf("ERROR: DIMM SPD at addr 0x%x is all 0xff..., looks like SPD is blank\n",
                   twsi_addr);
        return 0;
    }
    if (csum != (sum & 0xff)) {
        if (buffer[0] == 0 && ((sum + 0x80) & 0xff) == csum) {
            ddr_print("WARNING: DIMM SPD at addr 0x%x has incorrect value at address 0\n",
                      twsi_addr);
            /* checksum is bad, but we know why */
            return 1;
        }
        if (!silent)
            printf("ERROR: DIMM SPD at addr 0x%x: checksum mismatch(read: 0x%x, computed: 0x%x)\n",
                   twsi_addr, csum, sum & 0xFF);
        return 0;
    }
#else
    int rv;
    for (i = 0; i < 63; i++) {
        rv = cvmx_twsix_read_ia8(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK),
                                 twsi_addr, i, 1);
        if (rv < 0)
            return 0;   /* TWSI read error */
        sum  += (uint8_t)rv;

        /* Sometimes the tuple eeprom gets a correct SPD checksum, even
         * though most of it reads back 0xff due to different IA size.
         */
        if (rv == 0xff && ++ff_cnt > 20)
            return 0;
        /* Bail out early if it looks blank, as this is slow over ejtag.
         * If first 5 values are all 0xff, it is not a valid ddr2 SPD.
         */
        if (sum == (5*0xff) && i == 4)
            return 0;
    }
    csum = cvmx_twsix_read_ia8(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK),
                               twsi_addr, 63, 1);
    if (sum == 0xff*63) {
        if (!silent)
            printf("ERROR: DIMM SPD at addr 0x%x is all 0xff..., looks like SPD is blank\n",
                   twsi_addr);
        return 0;
    }
    if (csum != (sum & 0xff)) {
        /* Special handling for dimms that have byte 0 value of 0
         * instead of 0x80
         */
        if (cvmx_twsix_read_ia8(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK),
                                twsi_addr, 0, 1) == 0 && ((sum + 0x80)&0xFF) == csum) {
            ddr_print("WARNING: DIMM SPD at addr 0x%x has incorrect value at address 0\n",
                      twsi_addr);
            return 1;  /* Checksum is bad, but we know why */
        }

        if (!silent)
            printf("ERROR: DIMM SPD at addr 0x%x: checksum mismatch(read: 0x%x, computed: 0x%x)\n",
                   twsi_addr, csum, sum & 0xFF);
        return 0;
    }
#endif
    if (sum == 0) {
        if (!silent)
            printf("ERROR: DIMM SPD at addr 0x%x is all zeros....\n",
                   twsi_addr);
        return 0;
    }
    return 1;
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR3
static uint16_t ddr3_crc16(uint8_t *ptr, int count)
{
    /* From DDR3 spd specification */
    int crc, i;
    crc = 0;
    while (--count >= 0) {
        crc = crc ^ (int)*ptr++ << 8;
        for (i = 0; i < 8; ++i)
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
    }
    return (crc & 0xFFFF);
}

static int validate_spd_checksum_ddr3(int twsi_addr, int silent)
{
    uint8_t spd_data[128];
    int crc_bytes = 126;
    uint16_t crc_comp;

#ifdef __U_BOOT__
# ifdef CONFIG_OCTEON_OCX
    if (i2c_set_bus_num((!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) |
                        gd->arch.dram_init_node << 1) < 0) {
        printf("Error setting TWSI bus to %d\n",
               (!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) |
               gd->arch.dram_init_node << 1);
        return 0;
    }
# else
    if (i2c_set_bus_num(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) < 0) {
        printf("Error setting TWSI bus to %d\n",
               !!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK));
        return 0;
    }
# endif
    if (i2c_read((twsi_addr & 0x7f), 0, 1, spd_data, 128)) {
        printf("Error reading SPD EEPROM at 0x%x\n", twsi_addr);
        return 0;
    }

#else
    int i;
    int rv;
    for (i = 0; i < 128; i++) {
        rv = cvmx_twsix_read_ia8(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK),
                                 (twsi_addr & 0x7f), i, 1);
        if (rv < 0)
            return 0;   /* TWSI read error */
        spd_data[i]  = (uint8_t)rv;
    }
#endif
    /* Check byte 0 to see how many bytes checksum is over */
    if (spd_data[0] & 0x80)
        crc_bytes = 117;

    crc_comp = ddr3_crc16(spd_data, crc_bytes);

    if (spd_data[126] == (crc_comp & 0xff) &&
        spd_data[127] == (crc_comp >> 8)) {
        return 1;
    } else {
        if (!silent)
            printf("DDR3 SPD CRC error, spd addr: 0x%x, calculated crc: 0x%04x, read crc: 0x%02x%02x\n",
                   twsi_addr, crc_comp, spd_data[127], spd_data[126]);
        return 0;
    }
}
#endif

int validate_spd_checksum(int twsi_addr, int silent)
{
#ifdef __U_BOOT__
    uint8_t rv;
#else
    int rv;
#endif

    if (ddr_verbose())
        debug_print("Validating DIMM at address 0x%x\n", twsi_addr);

#ifdef __U_BOOT__
# ifdef CONFIG_OCTEON_OCX
    if (i2c_set_bus_num((!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) |
                        gd->arch.dram_init_node << 1) < 0) {
        printf("Error setting TWSI bus to %d\n",
               (!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) |
               gd->arch.dram_init_node << 1);
        return 0;
    }
# else
    if (i2c_set_bus_num(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK)) < 0) {
        printf("Error setting TWSI bus to %d\n",
               !!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK));
        return 0;
    }
# endif
    if (i2c_read(twsi_addr, 2, 1, &rv, 1))
        return 0;
#else
    /* Look up module type to determine if DDR2 or DDR3 */
    rv = cvmx_twsix_read_ia8(!!(twsi_addr & OCTEON_TWSI_BUS_IN_ADDR_MASK),
                             twsi_addr, 2, 1);
    if (rv < 0)
        return 0;   /* TWSI read error */
#endif
    if (rv >= 0x8 && rv <= 0xA)
#ifndef CONFIG_OCTEON_DISABLE_DDR2
        return validate_spd_checksum_ddr2(twsi_addr, silent);
#else
    printf("%s: Error: DDR2 support disabled\n", __func__);
#endif
    if (rv >= 0xB && rv <= 0xB)
#ifndef CONFIG_OCTEON_DISABLE_DDR3
        return validate_spd_checksum_ddr3(twsi_addr, silent);
#else
    printf("%s: Error: DDR3 support disabled\n", __func__);
#endif
    if (rv >= 0xC && rv <= 0xC)
#ifndef CONFIG_OCTEON_DISABLE_DDR3
        return validate_spd_checksum_ddr3(twsi_addr, silent);
#else
    printf("%s: Error: DDR4 support disabled\n", __func__);
#endif
    if (!silent)
        printf("Unrecognized DIMM type: 0x%x at spd address: 0x%x\n",
               rv, twsi_addr);
    return 0;
}


int validate_dimm(const dimm_config_t *dimm_config, int dimm_index)
{
    int spd_addr;
    dimm_index = !!dimm_index;  /* Normalize to 0/1 */
    spd_addr = dimm_config->spd_addrs[dimm_index];

    debug_print("Validating dimm %d, spd addr: 0x%02x spd ptr: %p\n",
                dimm_index,
                dimm_config->spd_addrs[dimm_index],
                dimm_config->spd_ptrs[dimm_index]);
    /* Only validate 'real' dimms, assume compiled in values are OK */
    if (!dimm_config->spd_ptrs[dimm_index]) {
        int val0, val1;
        int dimm_type = read_spd(dimm_config, dimm_index,
                                 DDR2_SPD_MEM_TYPE) & 0xff;
        switch (dimm_type) {
#ifndef CONFIG_OCTEON_DISABLE_DDR2
        case 0x08:              /* DDR2 */
            if (ddr_verbose())
                printf("Validating DDR2 DIMM %d\n", dimm_index);
            val0 = read_spd(dimm_config, dimm_index,
                            DDR2_SPD_NUM_ROW_BITS);
            val1 = read_spd(dimm_config, dimm_index,
                            DDR2_SPD_NUM_COL_BITS);
            if (val0 < 0 && val1 < 0)
                return 0; /* Failed to read dimm */
            if (val0 == 0xff && val1 == 0xff)
                return 0; /* Blank SPD or otherwise unreadable device */

            /* Don't treat bad checksums as fatal. */
            validate_spd_checksum(spd_addr, 0);
            break;
#endif
#ifndef CONFIG_OCTEON_DISABLE_DDR3
        case 0x0B:              /* DDR3 */
            if (ddr_verbose())
                printf("Validating DDR3 DIMM %d\n", dimm_index);
            val0 = read_spd(dimm_config, dimm_index,
                            DDR3_SPD_DENSITY_BANKS);
            val1 = read_spd(dimm_config, dimm_index,
                            DDR3_SPD_ADDRESSING_ROW_COL_BITS);
            if (val0 < 0 && val1 < 0) {
                if (ddr_verbose())
                    printf("Error reading SPD for DIMM %d\n",
                           dimm_index);
                return 0; /* Failed to read dimm */
            }
            if (val0 == 0xff && val1 == 0xff) {
                if (ddr_verbose())
                    printf("Blank or unreadable SPD for DIMM %d\n",
                           dimm_index);
                return 0; /* Blank SPD or otherwise unreadable device */
            }

            /* Don't treat bad checksums as fatal. */
            validate_spd_checksum(spd_addr, 0);
            break;
        case 0x0C:              /* DDR4 */
            if (ddr_verbose())
                printf("Validating DDR4 DIMM %d\n", dimm_index);
            val0 = read_spd(dimm_config, dimm_index,
                            DDR4_SPD_DENSITY_BANKS);
            val1 = read_spd(dimm_config, dimm_index,
                            DDR4_SPD_ADDRESSING_ROW_COL_BITS);
            if (val0 < 0 && val1 < 0) {
                if (ddr_verbose())
                    printf("Error reading SPD for DIMM %d\n",
                           dimm_index);
                return 0; /* Failed to read dimm */
            }
            if (val0 == 0xff && val1 == 0xff) {
                if (ddr_verbose())
                    printf("Blank or unreadable SPD for DIMM %d\n",
                           dimm_index);
                return 0; /* Blank SPD or otherwise unreadable device */
            }

            /* Don't treat bad checksums as fatal. */
            validate_spd_checksum(spd_addr, 0);
            break;
#endif
        case 0x00:              /* Terminator detected. Fail silently. */
            return 0;

        default:
            debug_print("Unknown DIMM type 0x%x for DIMM %d @ 0x%x\n",
                        dimm_type, dimm_index,
                        dimm_config->spd_addrs[dimm_index]);
            return 0;      /* Failed to read dimm */
        }
    }

    return 1;
}

/* Absolute value of an integer */
static inline int64_t _abs(int64_t v)
{
    return ((v < 0) ? -v : v);
}

#ifndef CONFIG_OCTEON_DISABLE_DDR3
/* Sign of an integer */
static inline int64_t _sign(int64_t v)
{
    return (v < 0);
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR2
static void report_ddr2_dimm(const dimm_config_t *dimm_config, int upper_dimm,
			     int dimm)
{
    int spd_ecc;
    int spd_rdimm;
    int i;
    int strlen;
    uint32_t serial_number;
    char part_number[19] = {0}; /* 18 bytes plus string terminator */
    upper_dimm = !!upper_dimm;  /* Normalize to 0/1 */

    spd_ecc   = !!(read_spd(dimm_config, upper_dimm, DDR2_SPD_CONFIG_TYPE) & 2);
    spd_rdimm = !!(read_spd(dimm_config, upper_dimm, DDR2_SPD_DIMM_TYPE) & 0x11);

    printf("DIMM %d: DDR2 %s, %s", dimm,
           (spd_rdimm ? "Registered" : "Unbuffered"),
           (spd_ecc ? "ECC" : "non-ECC"));

    strlen = 0;
    for (i = 0; i < 18; ++i) {
        part_number[i] = (read_spd(dimm_config, upper_dimm, i+73) & 0xff);
        if (part_number[i])
            ++strlen;
        /* debug_print("spd[%d]: 0x%02x\n", i+128, part_number[i]); */
    }
    if (strlen)
        printf("  %s", part_number);

    serial_number = 0;
    for (i = 0; i<4; ++i) {
        serial_number |= ((read_spd(dimm_config, upper_dimm, 95+i) & 0xff) << ((3-i)*8));
    }
    if ((serial_number!=0) && (serial_number!=0xffffffff)) {
        printf("  s/n %u", serial_number);
    } else {
        int spd_chksum;
        spd_chksum  =   0xff & read_spd(dimm_config, upper_dimm,
                                        DDR2_SPD_CHECKSUM);
        printf("  chksum: %u (0x%02x)", spd_chksum, spd_chksum);
    }
    printf("\n");
}
#endif /* ! CONFIG_OCTEON_DISABLE_DDR2 */

#ifndef CONFIG_OCTEON_DISABLE_DDR3

int get_ddr_type(const dimm_config_t *dimm_config, int upper_dimm)
{
    int spd_ddr_type;

#define DEVICE_TYPE DDR4_SPD_KEY_BYTE_DEVICE_TYPE // same for DDR3 and DDR4
    spd_ddr_type = read_spd(dimm_config, upper_dimm, DEVICE_TYPE);

    debug_print("%s:%d spd_ddr_type=0x%02x\n", __FUNCTION__, __LINE__, spd_ddr_type);

    /* we return only DDR4 or DDR3 */
    return (spd_ddr_type == 0x0C) ? DDR4_DRAM : DDR3_DRAM;
}

int get_dimm_ecc(const dimm_config_t *dimm_config, int upper_dimm, int ddr_type)
{
#define BUS_WIDTH(t)   (((t) == DDR4_DRAM) ? DDR4_SPD_MODULE_MEMORY_BUS_WIDTH : DDR3_SPD_MEMORY_BUS_WIDTH)

    return !!(read_spd(dimm_config, upper_dimm, BUS_WIDTH(ddr_type)) & 8);
}

int get_dimm_module_type(const dimm_config_t *dimm_config, int upper_dimm, int ddr_type)
{
#define MODULE_TYPE DDR4_SPD_KEY_BYTE_MODULE_TYPE // same for DDR3 and DDR4

    return (read_spd(dimm_config, upper_dimm, MODULE_TYPE) & 0x0F);
}

#define myisspace(c) (((c) == ' ') || (((c) >= '\t') && ((c) <= '\r')))

int get_dimm_part_number(char *buffer, const dimm_config_t *dimm_config,
                         int dimm_index, int ddr_type)
{
    int i;
    int c;
    int skipping = 1;
    int strlen   = 0;

#define PART_LIMIT(t)  (((t) == DDR4_DRAM) ? 19 : 18)
#define PART_NUMBER(t) (((t) == DDR4_DRAM) ? DDR4_SPD_MODULE_PART_NUMBER : DDR3_SPD_MODULE_PART_NUMBER)

    int limit  = PART_LIMIT(ddr_type);
    int offset = PART_NUMBER(ddr_type);

    for (i = 0; i < limit; ++i) {

	c = (read_spd(dimm_config, dimm_index, offset+i) & 0xff);
	if (c == 0) // any null, we are done
	    break;

	/* Skip leading spaces. */
	if (skipping) {
	    if (myisspace(c))
		continue;
	    else
		skipping = 0;
	}

	/* Put non-null non-leading-space-skipped char into buffer */
	buffer[strlen] = c;
	++strlen;
    }

    if (strlen > 0) {
	i = strlen - 1; // last char put into buf
	while (i >= 0 && myisspace((int)buffer[i])) { // still in buf and a space
	    --i;
	    --strlen;
	}
    }
    buffer[strlen] = 0;       /* Insure that the string is terminated */

    return strlen;
}

uint32_t get_dimm_serial_number(const dimm_config_t *dimm_config,
                                int dimm_index, int ddr_type)
{
    uint32_t serial_number = 0;
    int offset;
    int i, j;

#define SERIAL_NUMBER(t) (((t) == DDR4_DRAM) ? DDR4_SPD_MODULE_SERIAL_NUMBER : DDR3_SPD_MODULE_SERIAL_NUMBER)

    offset = SERIAL_NUMBER(ddr_type);

    for (i = 0, j = 24; i < 4; ++i, j -= 8) {
        serial_number |= ((read_spd(dimm_config, dimm_index, offset + i) & 0xff) << j);
    }

    return serial_number;
}

static uint32_t get_dimm_checksum(const dimm_config_t *dimm_config,
                                  int dimm_index, int ddr_type)
{
    uint32_t spd_chksum;

#define LOWER_NIBBLE(t) (((t) == DDR4_DRAM) ? DDR4_SPD_CYCLICAL_REDUNDANCY_CODE_LOWER_NIBBLE : DDR3_SPD_CYCLICAL_REDUNDANCY_CODE_LOWER_NIBBLE)
#define UPPER_NIBBLE(t) (((t) == DDR4_DRAM) ? DDR4_SPD_CYCLICAL_REDUNDANCY_CODE_UPPER_NIBBLE : DDR3_SPD_CYCLICAL_REDUNDANCY_CODE_UPPER_NIBBLE)

    spd_chksum  =   0xff & read_spd(dimm_config, dimm_index, LOWER_NIBBLE(ddr_type));
    spd_chksum |= ((0xff & read_spd(dimm_config, dimm_index, UPPER_NIBBLE(ddr_type))) << 8);

    return spd_chksum;
}

static
void report_common_dimm(const dimm_config_t *dimm_config, int upper_dimm, int dimm,
			const char **dimm_types, int ddr_type, char *volt_str, int ddr_interface_num)
{
    int spd_ecc;
    unsigned spd_module_type;
    uint32_t serial_number;
    char part_number[21]; /* 20 bytes plus string terminator is big enough for either */
    char *sn_str;

    spd_module_type = get_dimm_module_type(dimm_config, upper_dimm, ddr_type);
    spd_ecc = get_dimm_ecc(dimm_config, upper_dimm, ddr_type);

    (void) get_dimm_part_number(part_number, dimm_config, upper_dimm, ddr_type);

    serial_number = get_dimm_serial_number(dimm_config, upper_dimm, ddr_type);
    if ((serial_number != 0) && (serial_number != 0xffffffff)) {
        sn_str = "s/n";
    } else {
        serial_number = get_dimm_checksum(dimm_config, upper_dimm, ddr_type);
        sn_str = "chksum";
    }

    // FIXME: add output of DIMM rank/width, as in: 2Rx4, 1Rx8, etc
    printf("LMC%d DIMM %d: DDR%d %s, %s  %s  %s: %u   %s\n",
           ddr_interface_num, dimm, ddr_type, dimm_types[spd_module_type],
	   (spd_ecc ? "ECC" : "non-ECC"), part_number, sn_str, serial_number, volt_str);
}

static const char *ddr3_dimm_types[] = {
    /* 0000 */ "Undefined",
    /* 0001 */ "RDIMM",
    /* 0010 */ "UDIMM",
    /* 0011 */ "SO-DIMM",
    /* 0100 */ "Micro-DIMM",
    /* 0101 */ "Mini-RDIMM",
    /* 0110 */ "Mini-UDIMM",
    /* 0111 */ "Mini-CDIMM",
    /* 1000 */ "72b-SO-UDIMM",
    /* 1001 */ "72b-SO-RDIMM",
    /* 1010 */ "72b-SO-CDIMM"
    /* 1011 */ "LRDIMM",
    /* 1100 */ "16b-SO-DIMM",
    /* 1101 */ "32b-SO-DIMM",
    /* 1110 */ "Reserved",
    /* 1111 */ "Reserved"
};

static void
report_ddr3_dimm(const dimm_config_t *dimm_config, int upper_dimm,
                 int dimm, int ddr_interface_num)
{
    int spd_voltage;
    char *volt_str;

    spd_voltage = read_spd(dimm_config, upper_dimm, DDR3_SPD_NOMINAL_VOLTAGE);
    if ((spd_voltage == 0) || (spd_voltage & 3))
        volt_str = "1.5V";
    if (spd_voltage & 2)
        volt_str = "1.35V";
    if (spd_voltage & 4)
        volt_str = "1.2xV";

    report_common_dimm(dimm_config, upper_dimm, dimm, ddr3_dimm_types,
                       DDR3_DRAM, volt_str, ddr_interface_num);
}

static const char *ddr4_dimm_types[] = {
    /* 0000 */ "Extended",
    /* 0001 */ "RDIMM",
    /* 0010 */ "UDIMM",
    /* 0011 */ "SO-DIMM",
    /* 0100 */ "LRDIMM",
    /* 0101 */ "Mini-RDIMM",
    /* 0110 */ "Mini-UDIMM",
    /* 0111 */ "Reserved",
    /* 1000 */ "72b-SO-RDIMM",
    /* 1001 */ "72b-SO-UDIMM",
    /* 1010 */ "Reserved",
    /* 1011 */ "Reserved",
    /* 1100 */ "16b-SO-DIMM",
    /* 1101 */ "32b-SO-DIMM",
    /* 1110 */ "Reserved",
    /* 1111 */ "Reserved"
};

static void report_ddr4_dimm(const dimm_config_t *dimm_config, int upper_dimm,
			     int dimm, int ddr_interface_num)
{
    int spd_voltage;
    char *volt_str;

    spd_voltage = read_spd(dimm_config, upper_dimm, DDR4_SPD_MODULE_NOMINAL_VOLTAGE);
    if ((spd_voltage == 0x01) || (spd_voltage & 0x02))
	volt_str = "1.2V";
    if ((spd_voltage == 0x04) || (spd_voltage & 0x08))
	volt_str = "TBD1 V";
    if ((spd_voltage == 0x10) || (spd_voltage & 0x20))
	volt_str = "TBD2 V";

    report_common_dimm(dimm_config, upper_dimm, dimm, ddr4_dimm_types,
                       DDR4_DRAM, volt_str, ddr_interface_num);
}

static void report_dimm(const dimm_config_t *dimm_config, int upper_dimm,
                        int dimm, int ddr_interface_num)
{
    int ddr_type;

    /* ddr_type only indicates DDR4 or DDR3 */
    ddr_type = get_ddr_type(dimm_config, upper_dimm);

    if (ddr_type == DDR4_DRAM)
        report_ddr4_dimm(dimm_config, 0, dimm, ddr_interface_num);
    else
        report_ddr3_dimm(dimm_config, 0, dimm, ddr_interface_num);
}

#endif /* CONFIG_OCTEON_DISABLE_DDR3 */

#ifndef CONFIG_OCTEON_DISABLE_DDR2
static inline int lookup_cycle_time_psec (int spd_byte)
{
    static const char _subfield_b[] = { 00, 10, 20, 30, 40, 50, 60, 70,
                                        80, 90, 25, 33, 66, 75, 00, 00 };

    return (((spd_byte >> 4) & 0xf) * 1000) + _subfield_b[spd_byte & 0xf] * 10;
}

static int lookup_delay_psec (int spd_byte)
{
    static const char _subfield_b[] =  { 0, 25, 50, 75 };

    return (((spd_byte >> 2) & 0x3f) * 1000) + _subfield_b[spd_byte & 0x3] * 10;
}


static int lookup_refresh_rate_nsec (int spd_byte)
{
#define BASE_REFRESH 15625
    static const int _refresh[] = {
        (BASE_REFRESH),     /* 15.625 us */
        (BASE_REFRESH/4),   /* Reduced (.25x)..3.9us */
        (BASE_REFRESH/2),   /* Reduced (.5x)...7.8us */
        (BASE_REFRESH*2),   /* Extended (2x)...31.3us */
        (BASE_REFRESH*4),   /* Extended (4x)...62.5us */
        (BASE_REFRESH*8),   /* Extended (8x)...125us */
    };

    if ((spd_byte & 0x7f) > 5) {
        error_print("Unsupported refresh rate: %#04x\n", spd_byte);
        return -1;
    }

    return _refresh[spd_byte & 0x7f];
}

static int lookup_rfc_psec (int spd_trfc, int spd_trfc_ext)
{
    static const char _subfield_b[] =  { 0, 25, 33, 50, 66, 75 };
    int trfc = (spd_trfc * 1000) + _subfield_b[(spd_trfc_ext >> 1) & 0x7] * 10;
    return ((spd_trfc_ext & 1) ? (trfc + (256 * 1000)) : trfc);
}

static int select_cas_latency (ulong tclk_psecs, int spd_cas_latency,
			       unsigned *cycle_clx)
{
    int i;
    int max_cl = 7;

    while ( (((spd_cas_latency>>max_cl)&1) == 0) && (max_cl > 0) )
        --max_cl;

    if (max_cl == 0) {
        error_print("CAS Latency was not specified by SPD: %#04x\n",
                    spd_cas_latency);
        return -1;
    }

    /* Pick that fastest CL that is within spec. */
    for (i = 2; i >= 0; --i) {
        if ((spd_cas_latency>>(max_cl-i))&1) {
            ddr_print("CL%d Minimum Clock Rate                        : %6d ps\n",
                      max_cl - i, cycle_clx[i]);

            if (tclk_psecs >= cycle_clx[i])
                return max_cl-i;
        }
    }

    error_print("WARNING!!!!!!: DDR3 Clock Rate (tCLK: %ld) exceeds DIMM specifications (tCKmin:%ld)!!!!!!!!\n",
                tclk_psecs, (ulong)cycle_clx[0]);
    return max_cl;
}


/**
 * Calculate the board delay in quarter-cycles.  That value is used as
 * an index into the delay parameter table.
 */
static ulong compute_delay_params(ulong board_skew, ulong tclk_psecs,
				  int silo_qc_unsupported)
{
    int idx;
    static const ulong _params[] = {
        /* idx   board delay (cycles) tskw  silo_hc  silo_qc */
        /* ====  ==================== ====  =======  ======= */
        /* [ 0]     0.00 - 0.25 */     0 |   1<<8 |   0<<16,
        /* [ 1]     0.25 - 0.50 */     0 |   1<<8 |   1<<16,
        /* [ 2]     0.50 - 0.75 */     1 |   0<<8 |   0<<16,
        /* [ 3]     0.75 - 1.00 */     1 |   0<<8 |   1<<16,
        /* [ 4]     1.00 - 1.25 */     1 |   1<<8 |   0<<16,
        /* [ 5]     1.25 - 1.50 */     1 |   1<<8 |   1<<16,
        /* [ 6]     1.50 - 1.75 */     2 |   0<<8 |   0<<16,
        /* [ 7]     1.75 - 2.00 */     2 |   0<<8 |   1<<16,
        /* [ 8]     2.00 - 2.25 */     2 |   1<<8 |   0<<16,
        /* [ 9]     2.25 - 2.50 */     2 |   1<<8 |   1<<16,
        /* [10]     2.50 - 2.75 */     3 |   0<<8 |   0<<16,
        /* [11]     2.75 - 3.00 */     3 |   0<<8 |   1<<16,
        /* [12]     3.00 - 3.25 */     3 |   1<<8 |   0<<16,
        /* [13]     3.25 - 3.50 */     3 |   1<<8 |   1<<16,
        /* [14]     3.50 - 3.75 */     4 |   0<<8 |   0<<16,
    };


    /*
     * Octeon pass 1 did not support the quarter-cycle delay setting.
     * In that case round to the settings for the nearest half-cycle.
     * This is effectively done by rounding up to the next
     * quarter-cycle setting and ignoring the silo_qc bit when the
     * quarter-cycle setting is not supported.
     */
    idx = ((board_skew*4) + (tclk_psecs*silo_qc_unsupported)) / tclk_psecs;
    return _params[min(idx, 14)];
}

static int encode_row_lsb(uint32_t cpu_id, int row_lsb, int ddr_interface_wide)
{
    int encoded_row_lsb;
    static const int row_lsb_table[] = {
        /* 110: row_lsb = mem_adr[12] */ 6,
        /* 111: row_lsb = mem_adr[13] */ 7,
        /* 000: row_lsb = mem_adr[14] */ 0,
        /* 001: row_lsb = mem_adr[15] */ 1,
        /* 010: row_lsb = mem_adr[16] */ 2,
        /* 011: row_lsb = mem_adr[17] */ 3,
        /* 100: row_lsb = mem_adr[18] */ 4
        /* 101: RESERVED              */
    };

    int row_lsb_start = 12;
    if (octeon_is_cpuid(OCTEON_CN30XX))
        row_lsb_start = 12;
    else if (octeon_is_cpuid(OCTEON_CN31XX))
        row_lsb_start = 12;
    else if (octeon_is_cpuid(OCTEON_CN38XX))
        row_lsb_start = 14;
    else if (octeon_is_cpuid(OCTEON_CN58XX))
        row_lsb_start = 14;
    else if (octeon_is_cpuid(OCTEON_CN56XX))
        row_lsb_start = 12;
    else if (octeon_is_cpuid(OCTEON_CN52XX))
        row_lsb_start = 12;
    else if (octeon_is_cpuid(OCTEON_CN50XX))
        row_lsb_start = 12;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    if (octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN31XX)
        || octeon_is_cpuid(OCTEON_CN56XX) || octeon_is_cpuid(OCTEON_CN50XX)
        || octeon_is_cpuid(OCTEON_CN52XX)) {
        encoded_row_lsb = row_lsb_table[row_lsb  + ddr_interface_wide - row_lsb_start];
        debug_print("row_lsb_table[%d] = %d\n",
                    (row_lsb  + ddr_interface_wide - row_lsb_start),
                    encoded_row_lsb);
    } else {
        encoded_row_lsb = row_lsb + ddr_interface_wide - row_lsb_start;
    }
    return encoded_row_lsb;
}


static int encode_pbank_lsb(uint32_t cpu_id, int pbank_lsb,
			    int ddr_interface_wide)
{
    int encoded_pbank_lsb;

    static const int pbank_lsb_table[] = {
        14, /* 1110:pbank[1:0] = mem_adr[26:25]    / rank = mem_adr[24] (if bunk_ena) */
        15, /* 1111:pbank[1:0] = mem_adr[27:26]    / rank = mem_adr[25] (if bunk_ena) */
        0, /* 0000:pbank[1:0] = mem_adr[28:27]    / rank = mem_adr[26] (if bunk_ena) */
        1, /* 0001:pbank[1:0] = mem_adr[29:28]    / rank = mem_adr[27]      "        */
        2, /* 0010:pbank[1:0] = mem_adr[30:29]    / rank = mem_adr[28]      "        */
        3, /* 0011:pbank[1:0] = mem_adr[31:30]    / rank = mem_adr[29]      "        */
        4, /* 0100:pbank[1:0] = mem_adr[32:31]    / rank = mem_adr[30]      "        */
        5, /* 0101:pbank[1:0] = mem_adr[33:32]    / rank = mem_adr[31]      "        */
        6, /* 0110:pbank[1:0] ={1'b0,mem_adr[33]} / rank = mem_adr[32]      "        */
        7, /* 0111:pbank[1:0] ={2'b0}             / rank = mem_adr[33]      "        */
        /* 1000-1101: RESERVED                                                    */
    };

    int pbank_lsb_start = 0;
    if (octeon_is_cpuid(OCTEON_CN30XX))
        pbank_lsb_start = 25;
    else if (octeon_is_cpuid(OCTEON_CN31XX))
        pbank_lsb_start = 25;
    else if (octeon_is_cpuid(OCTEON_CN38XX))
        pbank_lsb_start = 27;
    else if (octeon_is_cpuid(OCTEON_CN58XX))
        pbank_lsb_start = 27;
    else if (octeon_is_cpuid(OCTEON_CN56XX))
        pbank_lsb_start = 25;
    else if (octeon_is_cpuid(OCTEON_CN52XX))
        pbank_lsb_start = 25;
    else if (octeon_is_cpuid(OCTEON_CN50XX))
        pbank_lsb_start = 25;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);
    if (octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN31XX)
        || octeon_is_cpuid(OCTEON_CN56XX) || octeon_is_cpuid(OCTEON_CN50XX)
        || octeon_is_cpuid(OCTEON_CN52XX)) {
        encoded_pbank_lsb = pbank_lsb_table[pbank_lsb + ddr_interface_wide - pbank_lsb_start];
        debug_print("pbank_lsb_table[%d] = %d\n",
                    (pbank_lsb + ddr_interface_wide - pbank_lsb_start),
                    encoded_pbank_lsb);
    } else {
        encoded_pbank_lsb = pbank_lsb + ddr_interface_wide - pbank_lsb_start;
    }
    return encoded_pbank_lsb;
}
#endif /* CONFIG_OCTEON_DISABLE_DDR2 */

#ifndef CONFIG_OCTEON_DISABLE_DDR3
static int encode_row_lsb_ddr3(uint32_t cpu_id, int row_lsb, int ddr_interface_wide)
{
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

    if (octeon_is_cpuid(OCTEON_CN6XXX) || octeon_is_cpuid(OCTEON_CNF7XXX) || octeon_is_cpuid(OCTEON_CN7XXX))
        row_lsb_start = 14;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    return row_lsb - row_lsb_start ;
}

static int encode_pbank_lsb_ddr3(uint32_t cpu_id, int pbank_lsb, int ddr_interface_wide)
{
    /*  Decoding for pbank_lsb                                             */
    /*       0000:DIMM = mem_adr[28]    / rank = mem_adr[27] (if RANK_ENA) */
    /*       0001:DIMM = mem_adr[29]    / rank = mem_adr[28]      "        */
    /*       0010:DIMM = mem_adr[30]    / rank = mem_adr[29]      "        */
    /*       0011:DIMM = mem_adr[31]    / rank = mem_adr[30]      "        */
    /*       0100:DIMM = mem_adr[32]    / rank = mem_adr[31]      "        */
    /*       0101:DIMM = mem_adr[33]    / rank = mem_adr[32]      "        */
    /*       0110:DIMM = mem_adr[34]    / rank = mem_adr[33]      "        */
    /*       0111:DIMM = 0              / rank = mem_adr[34]      "        */
    /*       1000-1111: RESERVED                                           */

    int pbank_lsb_start = 0;

    if (octeon_is_cpuid(OCTEON_CN6XXX) || octeon_is_cpuid(OCTEON_CNF7XXX) || octeon_is_cpuid(OCTEON_CN7XXX))
        pbank_lsb_start = 28;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    return pbank_lsb - pbank_lsb_start;
}
#endif

/* #define PRINT_LMC_REGS */
#ifdef PRINT_LMC_REGS
void print_addr_name(uint64_t addr, char *name)
{
    printf("%34s(0x%llx): 0x%llx\n", name, (unsigned long long)addr,
           (unsigned long long)cvmx_read_csr(addr));
}

#define PRINT_REG(X)     print_addr_name(X,#X)
static void octeon_print_lmc_regs(uint32_t cpu_id, int int_num)
{
    PRINT_REG(CVMX_LMCX_CTL(int_num));
    PRINT_REG(CVMX_LMCX_CTL1(int_num));
    PRINT_REG(CVMX_LMCX_DDR2_CTL(int_num));
    PRINT_REG(CVMX_LMC_RODT_CTL);

    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX)) {
        PRINT_REG(CVMX_LMCX_WODT_CTL0(int_num));
        PRINT_REG(CVMX_LMCX_WODT_CTL1(int_num));
    } else {
        PRINT_REG(CVMX_LMC_WODT_CTL);
    }
    PRINT_REG(CVMX_LMCX_DELAY_CFG(int_num));
    PRINT_REG(CVMX_LMCX_MEM_CFG0(int_num));
    PRINT_REG(CVMX_LMCX_MEM_CFG1(int_num));
    if (octeon_is_cpuid(OCTEON_CN58XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX)
        || octeon_is_cpuid(OCTEON_CN52XX)) {
        PRINT_REG(CVMX_LMCX_RODT_COMP_CTL(int_num));
        PRINT_REG(CVMX_LMC_PLL_CTL);
    }
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR2
static uint64_t octeon_read_lmcx_read_level_dbg(int ddr_interface_num, int idx)
{
    cvmx_lmcx_read_level_dbg_t read_level_dbg;
    read_level_dbg.u64 = 0;
    read_level_dbg.s.byte = idx;
    ddr_config_write_csr(CVMX_LMCX_READ_LEVEL_DBG(ddr_interface_num),
                         read_level_dbg.u64);
    read_level_dbg.u64 = cvmx_read_csr(CVMX_LMCX_READ_LEVEL_DBG(ddr_interface_num));
    return read_level_dbg.s.bitmask;
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR3
static uint64_t octeon_read_lmcx_ddr3_rlevel_dbg(int node, int ddr_interface_num, int idx)
{
    cvmx_lmcx_rlevel_dbg_t rlevel_dbg;
    cvmx_lmcx_rlevel_ctl_t rlevel_ctl;

    rlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));
    rlevel_ctl.s.byte = idx;

    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num),
                              rlevel_ctl.u64);
    cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));

    rlevel_dbg.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_DBG(ddr_interface_num));
    return rlevel_dbg.s.bitmask;
}

static uint64_t octeon_read_lmcx_ddr3_wlevel_dbg(int node, int ddr_interface_num, int idx)
{
    cvmx_lmcx_wlevel_dbg_t wlevel_dbg;

    wlevel_dbg.u64 = 0;
    wlevel_dbg.s.byte = idx;

    ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_DBG(ddr_interface_num),
                              wlevel_dbg.u64);
    cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_DBG(ddr_interface_num));

    wlevel_dbg.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_DBG(ddr_interface_num));
    return wlevel_dbg.s.bitmask;
}
#endif

#define DEBUG_VALIDATE_BITMASK 0
#if DEBUG_VALIDATE_BITMASK
#define debug_bitmask_print printf
#else
#define debug_bitmask_print(...)
#endif

#define RLEVEL_BITMASK_TRAILING_BITS_ERROR      5
#define RLEVEL_BITMASK_BUBBLE_BITS_ERROR        11 // FIXME? now less than TOOLONG
#define RLEVEL_BITMASK_NARROW_ERROR             6
#define RLEVEL_BITMASK_BLANK_ERROR              100
#define RLEVEL_BITMASK_TOOLONG_ERROR            12
#define RLEVEL_NONSEQUENTIAL_DELAY_ERROR        50
#define RLEVEL_ADJACENT_DELAY_ERROR             30

/*
 * Apply a filter to the BITMASK results returned from Octeon
 * read-leveling to determine the most likely delay result.  This
 * computed delay may be used to qualify the delay result returned by
 * Octeon. Accumulate an error penalty for invalid characteristics of
 * the bitmask so that they can be used to select the most reliable
 * results.
 *
 * The algorithm searches for the largest contiguous MASK within a
 * maximum RANGE of bits beginning with the MSB.
 *
 * 1. a MASK with a WIDTH less than 4 will be penalized
 * 2. Bubbles in the bitmask that occur before or after the MASK
 *    will be penalized
 * 3. If there are no trailing bubbles then extra bits that occur
 *    beyond the maximum RANGE will be penalized.
 *
 *   +++++++++++++++++++++++++++++++++++++++++++++++++++
 *   +                                                 +
 *   +   e.g. bitmask = 27B00                          +
 *   +                                                 +
 *   +   63                  +--- mstart           0   +
 *   +   |                   |                     |   +
 *   +   |         +---------+     +--- fb         |   +
 *   +   |         |  range  |     |               |   +
 *   +   V         V         V     V               V   +
 *   +                                                 +
 *   +   0 0 ... 1 0 0 1 1 1 1 0 1 1 0 0 0 0 0 0 0 0   +
 *   +                                                 +
 *   +           ^     ^     ^                         +
 *   +           |     | mask|                         +
 *   +     lb ---+     +-----+                         +
 *   +                  width                          +
 *   +                                                 +
 *   +++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#ifndef CONFIG_OCTEON_DISABLE_DDR3

typedef struct {
    uint64_t bm;
    uint8_t mstart;
    uint8_t width;
    int errs;
} rlevel_bitmask_t;

#define MASKRANGE_BITS  6
#define MASKRANGE       ((1 << MASKRANGE_BITS) - 1)

static int
validate_ddr3_rlevel_bitmask(rlevel_bitmask_t *rlevel_bitmask_p, int ddr_type)
{
    int i;
    int errors  = 0;
    uint64_t mask = 0;      /* Used in 64-bit comparisons */
    int8_t  mstart = 0;
    uint8_t width = 0;
    uint8_t firstbit = 0;
    uint8_t lastbit = 0;
    uint8_t bubble = 0;
    uint8_t tbubble = 0;
    uint8_t blank = 0;
    uint8_t narrow = 0;
    uint8_t trailing = 0;
    uint64_t bitmask = rlevel_bitmask_p->bm;
    uint8_t extras = 0;
    uint8_t toolong = 0;
    uint64_t temp;

    if (bitmask == 0) {
	blank += RLEVEL_BITMASK_BLANK_ERROR;
    } else {

	/* Look for fb, the first bit */
        temp = bitmask;
        while (!(temp & 1)) {
            firstbit++;
            temp >>= 1;
        }

	/* Look for lb, the last bit */
        lastbit = firstbit;
        while ((temp >>= 1))
            lastbit++;

	/* Start with the max range to try to find the largest mask within the bitmask data */
        width = MASKRANGE_BITS;
        for (mask = MASKRANGE; mask > 0; mask >>= 1, --width) {
	    for (mstart = lastbit - width + 1; mstart >= firstbit; --mstart) {
                temp = mask << mstart;
		if ((bitmask & temp) == temp)
                    goto done_now;
	    }
        }
    done_now:
        /* look for any more contiguous 1's to the right of mstart */
        if (width == MASKRANGE_BITS) { // only when maximum mask
            while ((bitmask >> (mstart - 1)) & 1) { // slide right over more 1's
                --mstart;
                if (ddr_type == DDR4_DRAM) // only for DDR4
                    extras++; // count the number of extra bits
            }
        }

        /* Penalize any extra 1's beyond the maximum desired mask */
        if (extras > 0)
            toolong = RLEVEL_BITMASK_TOOLONG_ERROR * ((1 << extras) - 1);

	/* Detect if bitmask is too narrow. */
	if (width < 4)
	    narrow = (4 - width) * RLEVEL_BITMASK_NARROW_ERROR;

	/* detect leading bubble bits, that is, any 0's between first and mstart */
        temp = bitmask >> (firstbit + 1);
        i = mstart - firstbit - 1;
        while (--i >= 0) {
	    if ((temp & 1) == 0)
		bubble += RLEVEL_BITMASK_BUBBLE_BITS_ERROR;
            temp >>= 1;
        }

        temp = bitmask >> (mstart + width + extras);
        i = lastbit - (mstart + width + extras - 1);
        while (--i >= 0) {
            if (temp & 1) { /* Detect 1 bits after the trailing end of the mask, including last. */
                trailing += RLEVEL_BITMASK_TRAILING_BITS_ERROR;
            } else { /* Detect trailing bubble bits, that is, any 0's between end-of-mask and last */
		tbubble  += RLEVEL_BITMASK_BUBBLE_BITS_ERROR;
            }
            temp >>= 1;
        }
    }

    errors = bubble + tbubble + blank + narrow + trailing + toolong;

    /* Pass out useful statistics */
    rlevel_bitmask_p->mstart = mstart;
    rlevel_bitmask_p->width  = width;

    debug_bitmask_print("bm:%08lx mask:%02lx, width:%2u, mstart:%2d, fb:%2u, lb:%2u"
                        " (bu:%2d, tb:%2d, bl:%2d, n:%2d, t:%2d, x:%2d) errors:%3d %s\n",
                        (unsigned long) bitmask, mask, width, mstart,
                        firstbit, lastbit, bubble, tbubble, blank, narrow,
                        trailing, toolong, errors, (errors) ? "=> invalid" : "");

    return errors;
}

static int compute_ddr3_rlevel_delay(uint8_t mstart, uint8_t width,
				     cvmx_lmcx_rlevel_ctl_t rlevel_ctl)
{
    int delay;

    debug_bitmask_print("  offset_en:%d", rlevel_ctl.s.offset_en);

    if (rlevel_ctl.s.offset_en) {
        delay = max(mstart, mstart + width - 1 - rlevel_ctl.s.offset);
    } else {
        /* if (rlevel_ctl.s.offset) { */ /* Experimental */
        if (0) {
            delay = max(mstart + rlevel_ctl.s.offset, mstart + 1);
            /* Insure that the offset delay falls within the bitmask */
            delay = min(delay, mstart + width-1);
        } else {
            delay = (width - 1) / 2 + mstart; /* Round down */
            /* delay = (width/2) + mstart; */  /* Round up */
        }
    }

    return delay;
}
#endif /* CONFIG_OCTEON_DISABLE_DDR3 */

/* Sometimes the pass/fail results for all possible delay settings
 * determined by the read-leveling sequence is too forgiving.  This
 * usually occurs for DCLK speeds below 300 MHz. As a result the
 * passing range is exaggerated. This function accepts the bitmask
 * results from the sequence and truncates the passing range to a
 * reasonable range and recomputes the proper deskew setting.
 */
#ifndef CONFIG_OCTEON_DISABLE_DDR2
static int adjust_read_level_byte(int bitmask)
{
    int mask;
    int shift;
    int leader;
    int adjusted = 0;
    int adjusted_byte_delay = 0;

    for (mask=0xf, leader=3; mask>0; mask=mask>>1, --leader) {
        for (shift=15-leader; shift>=0; --shift) {

#if DEBUG_DEFINED
            ddr_print("bitmask:%04x, mask:%x, shift:%d, leader:%d\n",
                      bitmask, mask, shift, leader);
#endif

            if (((bitmask >> shift) & mask) == mask) {
                adjusted = 1;
                break;
            }
        }

        if (adjusted)
            break;
    }

    if (adjusted)
        adjusted_byte_delay = shift + leader / 2;

    return adjusted_byte_delay;
}
#endif

/* Default ODT config must disable ODT */
/* Must be const (read only) so that the structure is in flash */
const dimm_odt_config_t disable_odt_config[] = {
    /*   1 */ {   0,    0x0000,   {.u64 = 0x0000},    {.u64 = 0x0000},    0,   0x0000,  0  },
    /*   2 */ {   0,    0x0000,   {.u64 = 0x0000},    {.u64 = 0x0000},    0,   0x0000,  0  },
    /*   3 */ {   0,    0x0000,   {.u64 = 0x0000},    {.u64 = 0x0000},    0,   0x0000,  0  },
    /*   4 */ {   0,    0x0000,   {.u64 = 0x0000},    {.u64 = 0x0000},    0,   0x0000,  0  },
};
/* Memory controller setup function */
static int init_octeon_dram_interface(uint32_t cpu_id,
				      const ddr_configuration_t *ddr_configuration,
				      uint32_t ddr_hertz,
				      uint32_t cpu_hertz,
				      uint32_t ddr_ref_hertz,
				      int board_type,
				      int board_rev_maj,
				      int board_rev_min,
				      int ddr_interface_num,
				      uint32_t ddr_interface_mask,
				      int node)
{
    uint32_t mem_size_mbytes = 0;

#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif
#if 1
    PRINT_CURSP("init_octeon_dram_interface");
#endif
#if defined(__U_BOOT__) && !defined(CONFIG_OCTEON_OCX)
    node = 0;
#elif defined(__U_BOOT__)
    gd->arch.dram_init_node = node;
#endif
    if ((s = lookup_env_parameter("ddr_timing_hertz")) != NULL)
        ddr_hertz = simple_strtoul(s, NULL, 0);

#ifndef CONFIG_OCTEON_DISABLE_DDR3
    if (OCTEON_IS_OCTEON2())
        mem_size_mbytes = init_octeon_ddr3_interface(cpu_id,
                                                     ddr_configuration,
                                                     ddr_hertz,
                                                     cpu_hertz,
                                                     ddr_ref_hertz,
                                                     board_type,
                                                     board_rev_maj,
                                                     board_rev_min,
                                                     ddr_interface_num,
                                                     ddr_interface_mask);
    else if (OCTEON_IS_OCTEON3()) {
        int lmc_restart_retries = 0;

    restart_lmc_init:
        mem_size_mbytes = init_octeon3_ddr3_interface(cpu_id,
                                                      ddr_configuration,
                                                      ddr_hertz,
                                                      cpu_hertz,
                                                      ddr_ref_hertz,
                                                      board_type,
                                                      board_rev_maj,
                                                      board_rev_min,
                                                      ddr_interface_num,
                                                      ddr_interface_mask,
                                                      node);
#define DEFAULT_RESTART_RETRIES 3
        if (mem_size_mbytes == 0) { // 0 means restart is possible
            if (lmc_restart_retries < DEFAULT_RESTART_RETRIES) {
                lmc_restart_retries++;
                ddr_print("N%d.LMC%d Configuration problem: attempting LMC reset and init restart %d\n",
                            node, ddr_interface_num, lmc_restart_retries);
                goto restart_lmc_init;
            } else {
                error_print("INFO: N%d.LMC%d Configuration: fatal problem remains after %d LMC init retries - Resetting node...\n",
                            node, ddr_interface_num, lmc_restart_retries);
#if defined(BDK) || defined(CVMX_BUILD_FOR_UBOOT)
                cvmx_wait_usec(500000);
                cvmx_reset_octeon_node(node);
                while (1);
#else
                mem_size_mbytes = -1; // return an error
#endif
            }
        }
    }
# ifdef CONFIG_OCTEON_DISABLE_DDR2
    else
        ddr_print("%s: DDR3 memory not supported\n", __func__);
# endif
#endif
#ifndef CONFIG_OCTEON_DISABLE_DDR2
    if (octeon_is_cpuid(OCTEON_CN3XXX) || octeon_is_cpuid(OCTEON_CN5XXX))
        mem_size_mbytes = init_octeon_ddr2_interface(cpu_id,
                                                     ddr_configuration,
                                                     ddr_hertz,
                                                     cpu_hertz,
                                                     ddr_ref_hertz,
                                                     board_type,
                                                     board_rev_maj,
                                                     board_rev_min,
                                                     ddr_interface_num,
                                                     ddr_interface_mask);
# ifdef CONFIG_OCTEON_DISABLE_DDR3
    else
        ddr_print("%s: DDR2 memory not supported\n", __func__);
# endif
#endif
    error_print("N%d.LMC%d Configuration Completed: %d MB\n",
              node, ddr_interface_num, mem_size_mbytes);
    return mem_size_mbytes;
}

#ifndef CONFIG_OCTEON_DISABLE_DDR2
static int init_octeon_ddr2_interface(uint32_t cpu_id,
				      const ddr_configuration_t *ddr_configuration,
				      uint32_t ddr_hertz,
				      uint32_t cpu_hertz,
				      uint32_t ddr_ref_hertz,
				      int board_type,
				      int board_rev_maj,
				      int board_rev_min,
				      int ddr_interface_num,
				      uint32_t ddr_interface_mask)
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

    uint32_t board_delay;

    const dimm_odt_config_t *odt_1rank_config = ddr_configuration->odt_1rank_config;
    const dimm_odt_config_t *odt_2rank_config = ddr_configuration->odt_2rank_config;
    const dimm_config_t *dimm_config_table = ddr_configuration->dimm_config_table;

    /*
    ** Compute clock rates in picoseconds.
    */
    ulong tclk_psecs = (ulong) divide_nint((1000*1000*1000), (ddr_hertz/1000)); /* Clock in psecs */
    ulong eclk_psecs = (ulong) divide_nint((1000*1000*1000), (cpu_hertz/1000)); /* Clock in psecs */

    cvmx_lmc_ctl_t lmc_ctl;
    cvmx_lmc_ddr2_ctl_t ddr2_ctl;
    cvmx_lmc_mem_cfg0_t mem_cfg0;
    cvmx_lmc_mem_cfg1_t mem_cfg1;
    cvmx_lmc_delay_cfg_t lmc_delay;
    cvmx_lmc_rodt_comp_ctl_t lmc_rodt_comp_ctl;

    int row_bits, col_bits, num_banks, num_ranks, dram_width;
    int dimm_count = 0;
    int fatal_error = 0;        /* Accumulate and report all the errors before giving up */

    int safe_ddr_flag = 0; /* Flag that indicates safe DDR settings should be used */
    int ddr_interface_wide = 0;
    int ddr_interface_width;
    uint32_t mem_size_mbytes = 0;
    unsigned int didx;
    int bank_bits = 0;
    int bunk_enable;
    int column_bits_start = 1;
    int row_lsb;
    int pbank_lsb;
    int use_ecc = 1;
    int spd_cycle_clx;
    int spd_refresh;
    int spd_cas_latency;
    int spd_cycle_clx1;
    int spd_cycle_clx2;
    int spd_trp;
    int spd_trrd;
    int spd_trcd;
    int spd_tras;
    int spd_twr;
    int spd_twtr;
    int spd_trfc_ext;
    int spd_trfc;
    int spd_ecc;
    int spd_rdimm;
    int spd_burst_lngth;
    int cas_latency;
    int refresh;
    int trp;
    int trrd;
    int trcd;
    int tras;
    int twr;
    int twtr;
    int trfc;
    unsigned cycle_clx[3];
    int ctl_silo_hc = 0, ctl_silo_qc = 0, ctl_tskw = 0, ctl_fprch2;
    int ctl_qs_dic, ctl_dic, ctl_odt_ena;
    unsigned int ctl_odt_mask, ctl_rodt_ctl;
    unsigned int ctl_odt_mask1 = 0;
    unsigned long delay_params;
    int odt_idx;
    const dimm_odt_config_t *odt_config;
    int ctl_ddr_eof;
    int ctl_twr;
    int cfg1_tras;
    int cfg1_trcd;
    int cfg1_twtr;
    int cfg1_trp;
    int cfg1_trfc;
    int cfg1_tmrd;
    int cfg1_trrd;
    int ref_int;

    ddr_print("\nInitializing DDR interface %d, DDR Clock %d, DDR Reference Clock %d, CPUID 0x%08x\n",
              ddr_interface_num, ddr_hertz, ddr_ref_hertz, cpu_id);

    if (dimm_config_table[0].spd_addrs[0] == 0 && !dimm_config_table[0].spd_ptrs[0]) {
        error_print("ERROR: No dimms specified in the dimm_config_table.\n");
        return (-1);
    }

    if (ddr_verbose()) {
        printf("DDR SPD Table:");
        for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx) {
            if (dimm_config_table[didx].spd_addrs[0] == 0) break;
            printf(" --ddr%dspd=0x%02x", ddr_interface_num, dimm_config_table[didx].spd_addrs[0]);
            if (dimm_config_table[didx].spd_addrs[1] != 0)
                printf(",0x%02x", dimm_config_table[didx].spd_addrs[1]);
        }
        printf("\n");
    }

    /*
    ** Walk the DRAM Socket Configuration Table to see what is installed.
    */
    for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx)
    {
        /* Check for lower DIMM socket populated */
        if (validate_dimm(&dimm_config_table[didx], 0)) {
            if (ddr_verbose())
                report_ddr2_dimm(&dimm_config_table[didx], 0, dimm_count);
            ++dimm_count;
        } else { break; }       /* Finished when there is no lower DIMM */

        /* Now check the upper dimm for chips with the 128-bit interface. */
        if (octeon_is_cpuid(OCTEON_CN38XX) || octeon_is_cpuid(OCTEON_CN58XX))
        {
            /* Check for upper DIMM socket populated */
            if (validate_dimm(&dimm_config_table[didx], 1)) {
                ddr_interface_wide = 1;
                if (ddr_verbose())
                    report_ddr2_dimm(&dimm_config_table[didx], 1, dimm_count);
                ++dimm_count;
            }

            /* Check for odd number of DIMMs when 128-bit expected */
            if ((ddr_interface_wide)  && (dimm_count&1)) {
                error_print("ERROR: Install DIMMs in pairs for 128-bit interface\n");
                ++fatal_error;
            }
        }
        else
        {
            ddr_interface_wide = 1;
            continue;               /* Wide means 1 DIMM width for O2P */
        }
    }


    initialize_ddr_clock(cpu_id,
			 ddr_configuration,
			 cpu_hertz,
			 ddr_hertz,
			 ddr_ref_hertz,
			 ddr_interface_num,
			 ddr_interface_mask,
			 0
                         );

    if (!odt_1rank_config)
        odt_1rank_config = disable_odt_config;
    if (!odt_2rank_config)
        odt_2rank_config = disable_odt_config;

    if (((s = getenv("ddr_safe")) != NULL) && (strcmp(s,"yes") == 0)) {
        safe_ddr_flag = 1;
        error_print("Parameter found in environment. ddr_safe = %d\n", safe_ddr_flag);
    }


    if (dimm_count == 0) {
        error_print("ERROR: DIMM 0 not detected.\n");
        return(-1);
    }

    /* Force narrow DDR for chips that do not support wide */
    if (octeon_is_cpuid(OCTEON_CN3020) || octeon_is_cpuid(OCTEON_CN3005))
        ddr_interface_wide = 0;

    /* Temporary workaround.   Some sample 3020 and 3010 parts are mismarked as 3100. */
    if (octeon_is_cpuid(OCTEON_CN31XX)
        && ((board_type == CVMX_BOARD_TYPE_CN3020_EVB_HS5) || (board_type == CVMX_BOARD_TYPE_CN3010_EVB_HS5))) {
        ddr_interface_wide = 0;
    }

    if (ddr_interface_wide && ((s = getenv("ddr_narrow")) != NULL) && (strcmp(s,"yes") == 0)) {
        error_print("Parameter found in environment: ddr_narrow, forcing narrow ddr interface\n");
        ddr_interface_wide = 0;
    }


    row_bits = read_spd(&dimm_config_table[0], 0, DDR2_SPD_NUM_ROW_BITS);
    col_bits = read_spd(&dimm_config_table[0], 0, DDR2_SPD_NUM_COL_BITS);
    num_banks = read_spd(&dimm_config_table[0], 0, DDR2_SPD_NUM_BANKS);
    num_ranks = 1 + (0x7 & read_spd(&dimm_config_table[0], 0, DDR2_SPD_NUM_RANKS));
    dram_width = read_spd(&dimm_config_table[0], 0, DDR2_SPD_SDRAM_WIDTH);



    /*
    ** Check that values are within some theoretical limits.
    ** col_bits(min) = row_lsb(min) - bank_bits(max) - bus_bits(max) = 14 - 3 - 4 = 7
    ** col_bits(max) = row_lsb(max) - bank_bits(min) - bus_bits(min) = 18 - 2 - 3 = 13
    */
    if ((col_bits > 13) || (col_bits < 7)) {
        error_print("Unsupported number of Col Bits: %d\n", col_bits);
        ++fatal_error;
    }

    /*
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


    bunk_enable = (num_ranks > 1);

#if defined(__U_BOOT__)
    if (octeon_is_cpuid(OCTEON_CN31XX))
    {
        /*
        ** For EBH3100 pass 1 we have screwy chip select mappings.  As a
        ** result only the first rank of each DIMM is selectable.
        ** Furthermore, the first rank of the second DIMM appears as the
        ** second rank of the first DIMM.  Therefore, when dual-rank DIMMs
        ** are present report half the memory size.  When single-rank
        ** DIMMs are present report them as one dual-rank DIMM.  The ODT
        ** masks will be adjusted appropriately.
        */
        {
            if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_EBH3100
                && gd->arch.board_desc.rev_major == 1
                && gd->arch.board_desc.rev_minor == 0)
            {
                bunk_enable = (dimm_count > 1) || (num_ranks > 1);
            }
        }
    }
#endif
    if (octeon_is_cpuid(OCTEON_CN30XX))
        column_bits_start = 1;
    else if (octeon_is_cpuid(OCTEON_CN31XX))
        column_bits_start = 2;
    else if (octeon_is_cpuid(OCTEON_CN38XX))
        column_bits_start = 3;
    else if (octeon_is_cpuid(OCTEON_CN58XX))
        column_bits_start = 3;
    else if (octeon_is_cpuid(OCTEON_CN56XX))
        column_bits_start = 2;
    else if (octeon_is_cpuid(OCTEON_CN52XX))
        column_bits_start = 2;
    else if (octeon_is_cpuid(OCTEON_CN50XX))
        column_bits_start = 1;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    row_lsb = column_bits_start + col_bits + bank_bits;
    debug_print("row_lsb = column_bits_start + col_bits + bank_bits = %d\n", row_lsb);

    pbank_lsb = row_lsb + row_bits + bunk_enable;
    debug_print("pbank_lsb = row_lsb + row_bits + bunk_enable = %d\n", pbank_lsb);


    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)
        )
    {
        /* Interface width is not a function of dimm_count so add it in
           for this calculation. */
        mem_size_mbytes =  dimm_count * ((1ull << (pbank_lsb+ddr_interface_wide)) >> 20);
#if defined(__U_BOOT__)
        {
            if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_EBH3100
                && gd->arch.board_desc.rev_major == 1
                && gd->arch.board_desc.rev_minor == 0
                && bunk_enable)
            {
                /* Reduce memory size by half on rev 1.0 ebh3100s */
                printf("NOTICE: Memory size reduced by half on 2 rank memory configs.\n");
                mem_size_mbytes /= 2;
            }
        }
#endif
    }
    else
        mem_size_mbytes =  dimm_count * ((1ull << pbank_lsb) >> 20);


    ddr_print("row bits: %d, col bits: %d, banks: %d, ranks: %d, dram width: %d, size: %d MB\n",
              row_bits, col_bits, num_banks, num_ranks, dram_width, mem_size_mbytes);

#if defined(__U_BOOT__)
    {
        /* If we are booting from RAM, the DRAM controller is already set up.  Just return the
        ** memory size */
        if (gd->flags & GD_FLG_RAM_RESIDENT)
            return mem_size_mbytes;
    }
#endif


    spd_cycle_clx   = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_CYCLE_CLX);
    spd_refresh     = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_REFRESH);
    spd_cas_latency = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_CAS_LATENCY);
    spd_cycle_clx1  = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_CYCLE_CLX1);
    spd_cycle_clx2  = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_CYCLE_CLX2);
    spd_trp         = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRP);
    spd_trrd        = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRRD);
    spd_trcd        = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRCD);
    spd_tras        = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRAS);
    spd_twr         = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TWR);
    spd_twtr        = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TWTR);
    spd_trfc_ext    = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRFC_EXT);
    spd_trfc        = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_TRFC);
    spd_ecc         = !!(read_spd(&dimm_config_table[0], 0, DDR2_SPD_CONFIG_TYPE) & 2);
    spd_rdimm       = !!(read_spd(&dimm_config_table[0], 0, DDR2_SPD_DIMM_TYPE) & 0x11);
    spd_burst_lngth = 0xff & read_spd(&dimm_config_table[0], 0, DDR2_SPD_BURST_LENGTH);

    /* Determine board delay based on registered or unbuffered dimms */
    board_delay = spd_rdimm ? ddr_configuration->registered.ddr_board_delay : ddr_configuration->unbuffered.ddr_board_delay;

    if (octeon_is_cpuid(OCTEON_CN3020) || octeon_is_cpuid(OCTEON_CN3005))
        use_ecc = 0;

    /* Temporary to handle mismarked 3020 parts */
    if (board_type == CVMX_BOARD_TYPE_CN3020_EVB_HS5)
        use_ecc = 0;

    use_ecc = use_ecc && spd_ecc;

    if (octeon_is_cpuid(OCTEON_CN31XX))
        ddr_interface_width = ddr_interface_wide ? 64 : 32;
    else if (octeon_is_cpuid(OCTEON_CN30XX))
        ddr_interface_width = ddr_interface_wide ? 32 : 16;
    else if (octeon_is_cpuid(OCTEON_CN58XX))
        ddr_interface_width = ddr_interface_wide ? 128 : 64;
    else if (octeon_is_cpuid(OCTEON_CN56XX))
        ddr_interface_width = ddr_interface_wide ? 64 : 32;
    else if (octeon_is_cpuid(OCTEON_CN52XX))
        ddr_interface_width = ddr_interface_wide ? 64 : 32;
    else if (octeon_is_cpuid(OCTEON_CN50XX))
        ddr_interface_width = ddr_interface_wide ? 32 : 16;
    else
        ddr_interface_width = ddr_interface_wide ? 128 : 64;

    ddr_print("DRAM Interface width: %d bits %s\n", ddr_interface_width, use_ecc ? "+ECC" : "");



    debug_print("spd_cycle_clx   : %#04x\n", spd_cycle_clx);
    debug_print("spd_refresh     : %#04x\n", spd_refresh);
    debug_print("spd_cas_latency : %#04x\n", spd_cas_latency);
    debug_print("spd_cycle_clx1  : %#04x\n", spd_cycle_clx1);
    debug_print("spd_cycle_clx2  : %#04x\n", spd_cycle_clx2);
    debug_print("spd_trp         : %#04x\n", spd_trp);
    debug_print("spd_trrd        : %#04x\n", spd_trrd);
    debug_print("spd_trcd        : %#04x\n", spd_trcd);
    debug_print("spd_tras        : %#04x\n", spd_tras);
    debug_print("spd_twr         : %#04x\n", spd_twr);
    debug_print("spd_twtr        : %#04x\n", spd_twtr);
    debug_print("spd_trfc_ext    : %#04x\n", spd_trfc_ext);
    debug_print("spd_trfc        : %#04x\n", spd_trfc);

    cycle_clx[0] = lookup_cycle_time_psec (spd_cycle_clx);
    cycle_clx[1] =  lookup_cycle_time_psec (spd_cycle_clx1);
    cycle_clx[2] =  lookup_cycle_time_psec (spd_cycle_clx2);

    cas_latency     = select_cas_latency (tclk_psecs, spd_cas_latency, cycle_clx);
    refresh         = lookup_refresh_rate_nsec (spd_refresh);
    trp             = lookup_delay_psec (spd_trp);
    trrd            = lookup_delay_psec (spd_trrd);
    trcd            = lookup_delay_psec (spd_trcd);
    tras            = spd_tras * 1000;
    twr             = lookup_delay_psec (spd_twr);
    twtr            = lookup_delay_psec (spd_twtr);
    trfc            = lookup_rfc_psec (spd_trfc, spd_trfc_ext);

    ddr_print("DDR Clock Rate (tCLK)                         : %6d ps\n", tclk_psecs);
    ddr_print("Core Clock Rate (eCLK)                        : %6d ps\n", eclk_psecs);
    ddr_print("CAS Latency                                   : %6d\n",    cas_latency);
    ddr_print("Refresh Rate (tREFI)                          : %6d ns\n", refresh);
    ddr_print("Minimum Row Precharge Time (tRP)              : %6d ps\n", trp);
    ddr_print("Minimum Row Active to Row Active delay (tRRD) : %6d ps\n", trrd);
    ddr_print("Minimum RAS to CAS delay (tRCD)               : %6d ps\n", trcd);
    ddr_print("Minimum Active to Precharge Time (tRAS)       : %6d ps\n", tras);
    ddr_print("Write Recovery Time (tWR)                     : %6d ps\n", twr);
    ddr_print("Internal write to read command delay (tWTR)   : %6d ps\n", twtr);
    ddr_print("Device Min Auto-refresh Active/Command (tRFC) : %6d ps\n", trfc);


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





    if ((s = getenv_wrapper("ddr_board_delay")) != NULL) {
        unsigned env_delay;
        env_delay = (unsigned) simple_strtoul(s, NULL, 0);
        if ((board_delay) && (board_delay != env_delay)) {
            error_print("Overriding internal board delay (%d ps).\n", board_delay);
            error_print("Parameter found in environment. ddr_board_delay = %d ps\n",
                        env_delay);
            board_delay = env_delay;
        }
    }

    ddr_print("Board delay                                   : %6d ps\n", board_delay);
    if (board_delay == 0 && (!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        /* Board delay of 0 uses read leveling for cn52xx/cn56xx */
        error_print("Error!!!  Board delay was not specified!!!\n");
        ++fatal_error;
    }

    /*
    ** Bail out here if things are not copasetic.
    */
    if (fatal_error)
        return(-1);


    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        delay_params = compute_delay_params(board_delay, tclk_psecs, 0);

        ctl_tskw    = (delay_params>>0) & 0xff;
        ctl_silo_hc = (delay_params>>8) & 1;
        ctl_silo_qc = (delay_params>>16) & 1;


        if (ddr_verbose())
        {
            if ((s = getenv_wrapper("ddr_tskw")) != NULL) {
                ctl_tskw    = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_tskw = %d\n", ctl_tskw);
            }

            if ((s = getenv_wrapper("ddr_silo_hc")) != NULL) {
                ctl_silo_hc    = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_silo_hc = %d\n", ctl_silo_hc);
            }

            if ((s = getenv_wrapper("ddr_silo_qc")) != NULL) {
                ctl_silo_qc    = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_silo_qc = %d\n", ctl_silo_qc);
            }
        }
    }


    ctl_fprch2  = 1;

    odt_idx = min(dimm_count - 1, 3);
    odt_config = bunk_enable ? odt_2rank_config : odt_1rank_config;

    ctl_qs_dic		= odt_config[odt_idx].qs_dic;
    /* Note: We don't use OCTEON_IS_MODEL here since we must use the cpu_id variable passed us - we
    ** may be running on a PCI host and not be able to ready the CPU ID directly */
    if ((octeon_is_cpuid(OCTEON_CN31XX) || (octeon_is_cpuid(OCTEON_CN30XX)))
        && (tclk_psecs <= 3750)
        && (num_ranks > 1)) {
        /* O2P requires 50 ohm ODT for dual-rank 533 MHz and higher  */
        ctl_qs_dic  = 3;
    }

    ctl_dic		= odt_config[odt_idx].dic;
    ctl_odt_ena		= odt_config[odt_idx].odt_ena;
    ctl_odt_mask	= odt_config[odt_idx].odt_mask;
    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)
        )
        ctl_odt_mask1	= odt_config[odt_idx].modereg_params1.u64;
    ctl_rodt_ctl	= odt_config[odt_idx].rodt_ctl;

    if ((s = getenv_wrapper("ddr_dic")) != NULL) {
        ctl_dic    = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_dic = %d\n", ctl_dic);
    }

    if ((s = getenv_wrapper("ddr_qs_dic")) != NULL) {
        ctl_qs_dic    = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_qs_dic = %d\n", ctl_qs_dic);
    }

    if ((s = getenv_wrapper("ddr_odt_ena")) != NULL) {
        ctl_odt_ena    = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_odt_ena = %d\n", ctl_odt_ena);
    }

    if ((s = getenv_wrapper("ddr_odt_mask")) != NULL) {
        ctl_odt_mask    = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_odt_mask = 0x%x\n", ctl_odt_mask);
    }

    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX))
    {
        if ((s = getenv_wrapper("ddr_odt_mask1")) != NULL) {
            ctl_odt_mask1    = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. ddr_odt_mask1 = 0x%x\n", ctl_odt_mask1);
        }
    }

    if ((s = getenv_wrapper("ddr_rodt_ctl")) != NULL) {
        ctl_rodt_ctl    = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_rodt_ctl = 0x%x\n", ctl_rodt_ctl);
    }

    /* Work around RODT disable errata.  In order to disable remote read ODT, both odt_ena and rodt_ctl must be 0 */
    if ((ctl_odt_ena == 0) || (ctl_rodt_ctl == 0)) {
	ctl_odt_ena = 0;
	ctl_rodt_ctl = 0;
    }


    if (board_type == CVMX_BOARD_TYPE_EBT3000 && board_rev_maj == 1)
    {
        /* Hack to support old rev 1 ebt3000 boards.  These boards
        ** are not supported by the pci utils */
        ctl_fprch2  = 0;
        ctl_qs_dic  = 2;  /* 0 may also work */
    }
    /* ----------------------------------------------------------------------------- */

    /*
     * DRAM Controller Initialization
     * The reference-clock inputs to the LMC (DDR2_REF_CLK_*) should be stable
     * when DCOK asserts (refer to Section 26.3). DDR_CK_* should be stable from that
     * point, which is more than 200 us before software can bring up the main memory
     * DRAM interface. The generated DDR_CK_* frequency is four times the
     * DDR2_REF_CLK_* frequency.
     * To initialize the main memory and controller, software must perform the following
     * steps in this order:
     *
     * 1. Write LMC_CTL with [DRESET] = 1, [PLL_BYPASS] = user_value, and
     * [PLL_DIV2] = user_value.
     */

    lmc_ctl.u64               = 0;

    lmc_ctl.s.dic               = ctl_dic;
    lmc_ctl.s.qs_dic            = ctl_qs_dic;
    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        lmc_ctl.s.tskw              = ctl_tskw;
        lmc_ctl.s.sil_lat           = safe_ddr_flag ? 2 : 1;
    }
    lmc_ctl.s.bprch             = ctl_silo_hc & ctl_silo_qc;
    lmc_ctl.s.fprch2            = ctl_fprch2;
    if (octeon_is_cpuid(OCTEON_CN31XX))
        lmc_ctl.cn31xx.mode32b           = ! ddr_interface_wide; /* 32-bit == Not Wide */
    else if (octeon_is_cpuid(OCTEON_CN30XX))
        lmc_ctl.cn30xx.mode32b           = ddr_interface_wide; /* 32-bit == Wide */
    else if (octeon_is_cpuid(OCTEON_CN56XX))
        lmc_ctl.cn56xx.mode32b           = ! ddr_interface_wide; /* 32-bit == Not Wide */
    else if (octeon_is_cpuid(OCTEON_CN52XX))
        lmc_ctl.cn52xx.mode32b           = ! ddr_interface_wide; /* 32-bit == Not Wide */
    else if (octeon_is_cpuid(OCTEON_CN50XX))
        lmc_ctl.cn50xx.mode32b           = ddr_interface_wide; /* 32-bit == Wide */
    else
        lmc_ctl.cn38xx.mode128b          = ddr_interface_wide; /* 128-bit == Wide */
    lmc_ctl.s.inorder_mrf       = 0;
    lmc_ctl.s.inorder_mwf       = 0;
    lmc_ctl.s.r2r_slot          = safe_ddr_flag ? 1 : 0;
    lmc_ctl.s.rdimm_ena         = spd_rdimm;
    lmc_ctl.s.max_write_batch   = 0xf;
    lmc_ctl.s.xor_bank          = 1;
    lmc_ctl.s.ddr__pctl         = 0;
    lmc_ctl.s.ddr__nctl         = 0;
    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX))
        lmc_ctl.cn31xx.dreset            = 1;

    ddr_config_write_csr(CVMX_LMCX_CTL(ddr_interface_num), lmc_ctl.u64);

    /* 2. Read L2D_BST0 and wait for the result. */
    cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

    /* 3. Wait 10 us (LMC_CTL[PLL_BYPASS] and LMC_CTL[PLL_DIV2] must not
     * transition after this) */
    cvmx_wait_usec(10);


    /* ----------------------------------------------------------------------------- */

    ctl_twr = divide_roundup(twr, tclk_psecs) - 1;
    ctl_twr = min(ctl_twr, 5);
    ctl_twr = max(ctl_twr, 1);

    if ((octeon_is_cpuid(OCTEON_CN58XX)) || (octeon_is_cpuid(OCTEON_CN56XX))
        || (octeon_is_cpuid(OCTEON_CN50XX)) || (octeon_is_cpuid(OCTEON_CN52XX))
        ) {
        int ctl_ddr_eof_ratio;
        ctl_ddr_eof_ratio = tclk_psecs*10/eclk_psecs;
        ctl_ddr_eof = 3;
        if (ctl_ddr_eof_ratio >= 15)
            ctl_ddr_eof = 2;
        if (ctl_ddr_eof_ratio >= 20)
            ctl_ddr_eof = 1;
        if (ctl_ddr_eof_ratio >= 30)
            ctl_ddr_eof = 0;
    }
    else {
        ctl_ddr_eof = min( 3ull, max( 1ull, divide_roundup(tclk_psecs, eclk_psecs)));
    }

    ddr2_ctl.u64 = 0;
    ddr2_ctl.s.ddr2       = 1;
    if (octeon_is_cpuid(OCTEON_CN38XX))
        ddr2_ctl.cn38xx.rdqs       = 0;
    if (octeon_is_cpuid(OCTEON_CN58XX))
        ddr2_ctl.cn58xx.rdqs       = 0;
    if (octeon_is_cpuid(OCTEON_CN56XX))
        ddr2_ctl.cn56xx.rdqs       = 0;
    if (octeon_is_cpuid(OCTEON_CN52XX))
        ddr2_ctl.cn52xx.rdqs       = 0;
    if (octeon_is_cpuid(OCTEON_CN50XX))
        ddr2_ctl.cn50xx.rdqs       = 0;
    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        ddr2_ctl.s.dll90_byp  = 0;
        ddr2_ctl.s.dll90_vlu  = 0;
        ddr2_ctl.s.qdll_ena   = 0;
    }
    ddr2_ctl.s.crip_mode  = 0;

    ddr2_ctl.s.ddr2t  = safe_ddr_flag ? 1 : (spd_rdimm == 0);

    if (ddr_verbose() && ((s = getenv("ddr_2t")) != NULL)) {
        ddr2_ctl.s.ddr2t  = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_2t = %d\n", ddr2_ctl.s.ddr2t);
    }


    ddr2_ctl.s.ddr_eof    = ctl_ddr_eof;
    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        ddr2_ctl.s.silo_hc    = ctl_silo_hc;
    }
    ddr2_ctl.s.twr        = ctl_twr;
    ddr2_ctl.s.bwcnt      = 0;
    ddr2_ctl.s.pocas      = 0;
    ddr2_ctl.s.addlat     = 0;
    if (cpu_id == OCTEON_CN38XX_PASS2) {
        ddr2_ctl.s.odt_ena    = 0; /* Signal aliased to cripple_mode. */
    }
    else {
        ddr2_ctl.s.odt_ena    = (ctl_odt_ena != 0);
    }
    ddr2_ctl.s.burst8     = (spd_burst_lngth & 8) ? 1 : 0;


    if (ddr_verbose() && (s = getenv("ddr_burst8")) != NULL) {
        ddr2_ctl.s.burst8  = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_burst8 = %d\n", ddr2_ctl.s.burst8);
    }


    ddr2_ctl.s.bank8      = (num_banks == 8);

    if (num_banks == 8) {
        int dram_page_size;
        int tfaw;

        /*
        ** Page size is the number of bytes of data delivered from the
        ** array to the internal sense amplifiers when an ACTIVE command
        ** is registered. Page size is per bank, calculated as follows:
        **
        ** page size = 2 COLBITS x ORG ÷ 8
        **
        ** where:
        ** COLBITS = the number of column address bits
        ** ORG = the number of I/O (DQ) bits
        */

        dram_page_size = (1 << col_bits) * dram_width / 8;

        /*
        **                       tFAW ns (JESD79-2C)
        **              -----------------------------------
        **   page size  DDR2-800 DDR2-667 DDR2-533 DDR2-400
        **   ---------  -------- -------- -------- --------
        **      1KB       35       37.5     37.5     37.5
        **      2KB       45       50       50       50
        */

        if (dram_page_size < 2048)
            tfaw = (tclk_psecs > 2500) ? 37500 : 35000; /* 1KB Page Size */
        else
            tfaw = (tclk_psecs > 2500) ? 50000 : 45000; /* 2KB Page Size */

        /*
        **  tFAW - Cycles = RNDUP[tFAW(ns)/tcyc(ns)] - 1
        **  Four Access Window time. Only relevent to 8-bank parts.
        */

        ddr2_ctl.s.tfaw = max(0ull, divide_roundup(tfaw, tclk_psecs) - 1);

        ddr_print("Four Activate Window/Page Size (Tfaw=%5d ps): %d/%dK\n",
                  tfaw, ddr2_ctl.s.tfaw, dram_page_size/1024);
    }


    /* 4. Write LMC_DDR2_CTL[QDLL_ENA] = 1. */ /* Is it OK to write 0 first? */
    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        cvmx_read_csr(CVMX_LMCX_DDR2_CTL(ddr_interface_num));
        ddr2_ctl.s.qdll_ena   = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR2_CTL(ddr_interface_num), ddr2_ctl.u64);
        cvmx_read_csr(CVMX_LMCX_DDR2_CTL(ddr_interface_num));

        octeon_delay_cycles(2000);   /* must be 200 dclocks */

        /* 5. Read L2D_BST0 and wait for the result. */
        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /* 6. Wait 10 us (LMC_DDR2_CTL[QDLL_ENA] must not transition after this) */
	cvmx_wait_usec(10);
    }


    /* ----------------------------------------------------------------------------- */

    /*
     * 7. Write LMC_CTL[DRESET] = 0 (at this point, the DCLK is running and the
     * memory controller is out of reset)
     */

    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX))
    {
        lmc_ctl.cn31xx.dreset            = 0;
        ddr_config_write_csr(CVMX_LMCX_CTL(ddr_interface_num), lmc_ctl.u64);
        octeon_delay_cycles(2000);   /* must be 200 dclocks */
    }

    /* ----------------------------------------------------------------------------- */
    /*
     * Next, boot software must re-initialize the LMC_MEM_CFG1, LMC_CTL, and
     * LMC_DDR2_CTL CSRs, and also the LMC_WODT_CTL and LMC_RODT_CTL
     * CSRs. Refer to Sections 2.3.4, 2.3.5, and 2.3.7 regarding these CSRs (and
     * LMC_MEM_CFG0).
     */

    /* Configure ODT */
    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)
        )
    {
        ddr_config_write_csr(CVMX_LMCX_WODT_CTL0(ddr_interface_num), ctl_odt_mask);
    }
    else
        ddr_config_write_csr(CVMX_LMC_WODT_CTL, ctl_odt_mask);

    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN52XX)
        )
    {
        ddr_config_write_csr(CVMX_LMCX_WODT_CTL1(ddr_interface_num), ctl_odt_mask1);
    }

    ddr_config_write_csr(CVMX_LMCX_RODT_CTL(ddr_interface_num), ctl_rodt_ctl);

    ddr_config_write_csr(CVMX_LMCX_DDR2_CTL(ddr_interface_num), ddr2_ctl.u64);

    /* ----------------------------------------------------------------------------- */

    if ((octeon_is_cpuid(OCTEON_CN30XX))
        || (cpu_id == OCTEON_CN38XX_PASS3)
        || (octeon_is_cpuid(OCTEON_CN58XX))
        || (octeon_is_cpuid(OCTEON_CN56XX))
        || (octeon_is_cpuid(OCTEON_CN52XX))
        || (octeon_is_cpuid(OCTEON_CN50XX)))
    {
        lmc_delay.u64 = 0;

        if (spd_rdimm == 1) {
            lmc_delay.s.clk  = ddr_configuration->registered.lmc_delay_clk;
            lmc_delay.s.cmd  = ddr_configuration->registered.lmc_delay_cmd;
            lmc_delay.s.dq   = ddr_configuration->registered.lmc_delay_dq;
        } else {
            lmc_delay.s.clk  = ddr_configuration->unbuffered.lmc_delay_clk;
            lmc_delay.s.cmd  = ddr_configuration->unbuffered.lmc_delay_cmd;
            lmc_delay.s.dq   = ddr_configuration->unbuffered.lmc_delay_dq;
        }

        if (ddr_verbose())
        {
            if ((s = getenv_wrapper("ddr_delay_clk")) != NULL) {
                lmc_delay.s.clk  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_delay_clk = %d\n", lmc_delay.s.clk);
            }

            if ((s = getenv_wrapper("ddr_delay_cmd")) != NULL) {
                lmc_delay.s.cmd  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_delay_cmd = %d\n", lmc_delay.s.cmd);
            }

            if ((s = getenv_wrapper("ddr_delay_dq")) != NULL) {
                lmc_delay.s.dq  = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_delay_dq = %d\n", lmc_delay.s.dq);
            }
        }

        ddr_config_write_csr(CVMX_LMCX_DELAY_CFG(ddr_interface_num), lmc_delay.u64);

        ddr_print("delay_clk                                     : %6d\n", lmc_delay.s.clk);
        ddr_print("delay_cmd                                     : %6d\n", lmc_delay.s.cmd);
        ddr_print("delay_dq                                      : %6d\n", lmc_delay.s.dq);
    }

    /* ----------------------------------------------------------------------------- */

    if (ddr_verbose() && (octeon_is_cpuid(OCTEON_CN58XX) || octeon_is_cpuid(OCTEON_CN56XX)
                          || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)
                          ))
    {
        cvmx_lmc_ctl1_t lmc_ctl1;
        cvmx_lmc_pll_ctl_t pll_ctl;
        uint64_t clkf, clkr, pll_MHz, calculated_ddr_hertz;
        int _en = 0;

        lmc_ctl1.u64 = cvmx_read_csr(CVMX_LMCX_CTL1(ddr_interface_num));
        ddr_print("sil_mode                                      : %6d\n", lmc_ctl1.s.sil_mode);

        pll_ctl.u64 = cvmx_read_csr(CVMX_LMC_PLL_CTL);  /* Only one LMC_PLL_CTL */
        clkf = pll_ctl.s.clkf;
        clkr = pll_ctl.s.clkr;

        ddr_print("DDR Fixed Reference Clock Hertz               : %8d\n", ddr_ref_hertz);
        ddr_print("clkf                                          : %6d\n", clkf);
        ddr_print("clkr                                          : %6d\n", clkr);

        if (pll_ctl.s.en2 == 1) {
            ddr_print("EN2                                           : %6d\n", 1); _en = 2;
        }
        if (pll_ctl.s.en4 == 1) {
            ddr_print("EN4                                           : %6d\n", 1); _en = 4;
        }
        if (pll_ctl.s.en6 == 1) {
            ddr_print("EN6                                           : %6d\n", 1); _en = 6;
        }
        if (pll_ctl.s.en8 == 1) {
            ddr_print("EN8                                           : %6d\n", 1); _en = 8;
        }
        if (pll_ctl.s.en12 == 1) {
            ddr_print("EN12                                          : %6d\n", 1); _en = 12;
        }
        if (pll_ctl.s.en16 == 1) {
            ddr_print("EN16                                          : %6d\n", 1); _en = 16;
        }

        pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;

        ddr_print("LMC PLL Frequency                             : %6d MHz\n", pll_MHz);

        calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * _en);
        ddr_print("Calculated DClk Frequency                     : %8d Hz\n", calculated_ddr_hertz);
    }

    /* ----------------------------------------------------------------------------- */

    cfg1_tras =  divide_roundup(tras, tclk_psecs);

    cfg1_trcd = divide_roundup(trcd, tclk_psecs);

    cfg1_twtr = divide_roundup(twtr, tclk_psecs);

    /* Plus 1 for 8-bank parts */
    cfg1_trp = divide_roundup(trp, tclk_psecs) + (num_banks == 8);

    cfg1_trfc =  divide_roundup(trfc, (4*tclk_psecs));

    cfg1_tmrd = 3; /* Always at least 2. 3 supports higher speeds */

    cfg1_trrd = divide_roundup(trrd, tclk_psecs);



    mem_cfg1.u64 = 0;
    mem_cfg1.s.tras     = cfg1_tras;
    /* In 2T mode, make this register TRCD-1, not going below 2. */
    mem_cfg1.s.trcd     = max(2, (ddr2_ctl.s.ddr2t ? (cfg1_trcd - 1) : cfg1_trcd));
    mem_cfg1.s.twtr     = cfg1_twtr;
    mem_cfg1.s.trp      = cfg1_trp;
    mem_cfg1.s.trfc     = cfg1_trfc;
    mem_cfg1.s.tmrd     = cfg1_tmrd;
    mem_cfg1.s.caslat   = cas_latency;
    mem_cfg1.s.trrd     = cfg1_trrd;



    ddr_config_write_csr(CVMX_LMCX_MEM_CFG1(ddr_interface_num), mem_cfg1.u64);

    /* ----------------------------------------------------------------------------- */


    {
        cvmx_lmc_comp_ctl_t lmc_comp_ctl;

        lmc_comp_ctl.u64 = 0;
        lmc_comp_ctl.s.pctl_clk = 0x0;
        lmc_comp_ctl.s.pctl_csr = 0xf;
        lmc_comp_ctl.s.nctl_dat = 0x0;
        lmc_comp_ctl.s.nctl_clk = 0x0;
        lmc_comp_ctl.s.nctl_csr = 0xf;


#if 0
        if (octeon_is_cpuid(OCTEON_CN52XX)) {
            cvmx_lmcx_pll_status_t tmp_pll_status;

            /*
             * Override compensation circuit with hardcoded settings.
             */

            tmp_pll_status.u64 = cvmx_read_csr(CVMX_LMCX_PLL_STATUS(ddr_interface_num));

#if 0
#define CN52XX_P1_NCTL_DAT_STRENGTH       11                      /* 15 ohm Output Impedance (Strength) setting */
#define CN52XX_P1_PCTL_DAT_STRENGTH       31                      /* 15 ohm Output Impedance (Strength) setting */
#endif
#if 1
#define CN52XX_P1_NCTL_DAT_STRENGTH        3                      /* 50 ohm Output Impedance (Strength) setting */
#define CN52XX_P1_PCTL_DAT_STRENGTH       10                      /* 50 ohm Output Impedance (Strength) setting */
#endif
#if 0
#define CN52XX_P1_NCTL_DAT_STRENGTH        2                      /* 65 ohm Output Impedance (Strength) setting */
#define CN52XX_P1_PCTL_DAT_STRENGTH        7                      /* 65 ohm Output Impedance (Strength) setting */
#endif


            lmc_comp_ctl.cn52xx.pctl_dat =  (((CN52XX_P1_PCTL_DAT_STRENGTH) + 32) - tmp_pll_status.cn52xx.ddr__pctl) & 0x1f;
            lmc_comp_ctl.cn52xx.nctl_dat =  (((CN52XX_P1_NCTL_DAT_STRENGTH) + 32) - tmp_pll_status.cn52xx.ddr__nctl) & 0xf;
            ddr_print("DDR PMOS dat                                  : %6d\n", lmc_comp_ctl.u64 & 0x1full);
            ddr_print("DDR NMOS dat                                  : %6d\n", lmc_comp_ctl.cn52xx.nctl_dat);
        }
#endif

        if (board_type == CVMX_BOARD_TYPE_EBT3000 && board_rev_maj == 1)
        {
            /* Hack to support old rev 1 ebt3000 boards.  These boards
            ** are not supported by the pci utils */
            cvmx_lmc_ctl_t tmp_ctl;

#define NCTL_CMD_STRENGTH       14                      /* Strength setting */
#define PCTL_CMD_STRENGTH       14                      /* Strength setting */
#define NCTL_CLK_STRENGTH       2                       /* Strength setting */
#define PCTL_CLK_STRENGTH       1                       /* Strength setting */

            tmp_ctl.u64 = cvmx_read_csr(CVMX_LMCX_CTL(ddr_interface_num));

            lmc_comp_ctl.cn38xx.pctl_cmd =  (((PCTL_CMD_STRENGTH) + 16) - tmp_ctl.s.ddr__pctl) & 0xf;
            lmc_comp_ctl.s.nctl_cmd =  (((NCTL_CMD_STRENGTH) + 16) - tmp_ctl.s.ddr__nctl) & 0xf;

            if (ddr_hertz > 250000000)
            {
                lmc_comp_ctl.s.pctl_clk =  (((PCTL_CLK_STRENGTH) + 16) - tmp_ctl.s.ddr__pctl) & 0xf;
                lmc_comp_ctl.s.nctl_clk =  (((NCTL_CLK_STRENGTH) + 16) - tmp_ctl.s.ddr__nctl) & 0xf;
            }
        }


        if (cpu_id == OCTEON_CN56XX_PASS1) {
            cvmx_lmcx_pll_status_t tmp_pll_status;

            /*
             * CN56XX Compensation circuit is not working on pass 1.
             * Override with hardcoded settings.
             */

            tmp_pll_status.u64 = cvmx_read_csr(CVMX_LMCX_PLL_STATUS(ddr_interface_num));

#define CN56XX_P1_NCTL_DAT_STRENGTH        6                      /* 25 ohm Output Impedance (Strength) setting */
#define CN56XX_P1_PCTL_DAT_STRENGTH       19                      /* 25 ohm Output Impedance (Strength) setting */

            lmc_comp_ctl.cn56xx.pctl_dat =  (((CN56XX_P1_PCTL_DAT_STRENGTH) + 32) - tmp_pll_status.cn56xx.ddr__pctl) & 0x1f;
            lmc_comp_ctl.cn56xx.nctl_dat =  (((CN56XX_P1_NCTL_DAT_STRENGTH) + 32) - tmp_pll_status.cn56xx.ddr__nctl) & 0xf;
            ddr_print("DDR PMOS dat                                  : %6d\n", lmc_comp_ctl.u64 & 0x1full);
            ddr_print("DDR NMOS dat                                  : %6d\n", lmc_comp_ctl.cn56xx.nctl_dat);
        }

        ddr_config_write_csr(CVMX_LMCX_COMP_CTL(ddr_interface_num), lmc_comp_ctl.u64);
    }

    /* ----------------------------------------------------------------------------- */

    /*
     * Finally, software must write the LMC_MEM_CFG0 register with
     * LMC_MEM_CFG0[INIT_START] = 1. At that point, CN31XX hardware initiates
     * the standard DDR2 initialization sequence shown in Figure 2.
     */

    ref_int = (refresh*1000) / (tclk_psecs*512);

    mem_cfg0.u64 = 0;


    mem_cfg0.s.init_start   = 0;
    ddr_config_write_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64); /* Reset one shot */

    mem_cfg0.s.ecc_ena      = use_ecc && spd_ecc;
    mem_cfg0.s.row_lsb      = encode_row_lsb(cpu_id, row_lsb, ddr_interface_wide);
    mem_cfg0.s.bunk_ena     = bunk_enable;

    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        mem_cfg0.s.silo_qc  = ctl_silo_qc;
    }

    mem_cfg0.s.pbank_lsb    = encode_pbank_lsb(cpu_id, pbank_lsb, ddr_interface_wide);
    mem_cfg0.s.ref_int      = ref_int;
    mem_cfg0.s.tcl          = 0; /* Has no effect on the controller's behavior */
    mem_cfg0.s.intr_sec_ena = 0;
    mem_cfg0.s.intr_ded_ena = 0;
    mem_cfg0.s.sec_err      = ~0;
    mem_cfg0.s.ded_err      = ~0;
    mem_cfg0.s.reset        = 0;



    ddr_print("bunk_enable                                   : %6d\n", mem_cfg0.s.bunk_ena);
    ddr_print("burst8                                        : %6d\n", ddr2_ctl.s.burst8);
    ddr_print("ddr2t                                         : %6d\n", ddr2_ctl.s.ddr2t);

    if ((!octeon_is_cpuid(OCTEON_CN56XX)) && (!octeon_is_cpuid(OCTEON_CN52XX))) {
        ddr_print("tskw                                          : %6d\n", lmc_ctl.s.tskw);
        ddr_print("silo_hc                                       : %6d\n", ddr2_ctl.s.silo_hc);

        ddr_print("silo_qc                                       : %6d\n", mem_cfg0.s.silo_qc);

        ddr_print("sil_lat                                       : %6d\n", lmc_ctl.s.sil_lat);
    }

    ddr_print("r2r_slot                                      : %6d\n", lmc_ctl.s.r2r_slot);

    ddr_print("odt_ena                                       : %6d\n", ddr2_ctl.s.odt_ena);
    ddr_print("qs_dic                                        : %6d\n", lmc_ctl.s.qs_dic);
    ddr_print("dic                                           : %6d\n", lmc_ctl.s.dic);
    ddr_print("ctl_odt_mask                                  : %08x\n", ctl_odt_mask);
    if (octeon_is_cpuid(OCTEON_CN31XX) || octeon_is_cpuid(OCTEON_CN30XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX))
        ddr_print("ctl_odt_mask1                                 : %08x\n", ctl_odt_mask1);
    ddr_print("ctl_rodt_ctl                                  : %08x\n", ctl_rodt_ctl);

    if (octeon_is_cpuid(OCTEON_CN58XX) || octeon_is_cpuid(OCTEON_CN56XX)
        || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)
        ) {
        uint32_t rodt_ena = odt_config[odt_idx].odt_ena;
        cvmx_lmc_ctl_t tmp_ctl;
        lmc_rodt_comp_ctl.u64 = 0;

        if ((s = getenv("ddr_rodt_ena")) != NULL) {
            rodt_ena  = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. ddr_rodt_ena = %d\n", rodt_ena);
        }

        if (rodt_ena == 1) { /* Weak Read ODT */
            lmc_rodt_comp_ctl.s.enable = 1;
            lmc_rodt_comp_ctl.s.pctl = 3;
            lmc_rodt_comp_ctl.s.nctl = 1;
        }

        if (rodt_ena == 2) { /* Strong Read ODT */
            lmc_rodt_comp_ctl.s.enable = 1;
            lmc_rodt_comp_ctl.s.pctl = 7;
            lmc_rodt_comp_ctl.s.nctl = 2;
        }

        if (rodt_ena == 3) { /* Dynamic Read ODT */
            cvmx_lmcx_pll_status_t pll_status;
            pll_status.u64 = cvmx_read_csr(CVMX_LMCX_PLL_STATUS(ddr_interface_num));

            lmc_rodt_comp_ctl.s.enable = 1;
            lmc_rodt_comp_ctl.s.pctl = pll_status.s.ddr__pctl;
            lmc_rodt_comp_ctl.s.nctl = pll_status.s.ddr__nctl;
        }


        ddr_config_write_csr(CVMX_LMCX_RODT_COMP_CTL(ddr_interface_num), lmc_rodt_comp_ctl.u64);

        ddr_print("RODT enable                                   : %6d\n", lmc_rodt_comp_ctl.s.enable);
        ddr_print("RODT pctl                                     : %6d\n", lmc_rodt_comp_ctl.s.pctl);
        ddr_print("RODT nctl                                     : %6d\n", lmc_rodt_comp_ctl.s.nctl);

        if (octeon_is_cpuid(OCTEON_CN56XX) || octeon_is_cpuid(OCTEON_CN50XX) || octeon_is_cpuid(OCTEON_CN52XX)) {
            /* CN56XX has a 5-bit copy of the compensation values */
            cvmx_lmcx_pll_status_t pll_status;
            pll_status.u64 = cvmx_read_csr(CVMX_LMCX_PLL_STATUS(ddr_interface_num));
            ddr_print("DDR PMOS control                              : %6d\n", pll_status.s.ddr__pctl);
            ddr_print("DDR NMOS control                              : %6d\n", pll_status.s.ddr__nctl);
        } else {
            tmp_ctl.u64 = cvmx_read_csr(CVMX_LMCX_CTL(ddr_interface_num));
            ddr_print("DDR PMOS control                              : %6d\n", tmp_ctl.s.ddr__pctl);
            ddr_print("DDR NMOS control                              : %6d\n", tmp_ctl.s.ddr__nctl);
        }
    }

    ddr_config_write_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64);
    mem_cfg0.u64 = cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num));

    mem_cfg0.s.init_start   = 1;
    ddr_config_write_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64);
    cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num));
    /* ----------------------------------------------------------------------------- */


    if ((octeon_is_cpuid(OCTEON_CN56XX)) || (octeon_is_cpuid(OCTEON_CN52XX))) {
        cvmx_lmcx_read_level_ctl_t read_level_ctl;
        cvmx_lmcx_read_level_rankx_t read_level_rankx;
        cvmx_lmc_ctl1_t lmc_ctl1;
        uint32_t rankmask;
        int sequence = 0; /* Default to not using read leveling to establish timing */
        int global_delay;
        int idx, rankidx;


        /* Use read levelling if sequence is 0 */
        if (board_delay == 0)
            sequence = 1;

        if ((s = getenv_wrapper("ddr_read_level")) != NULL) {
            sequence = (simple_strtoul(s, NULL, 0) == 0) ? 0 : 1;
            error_print("Parameter found in environment. ddr_read_level = %d\n", sequence);
        }

        read_level_ctl.u64 = cvmx_read_csr(CVMX_LMCX_READ_LEVEL_CTL(ddr_interface_num));
        lmc_ctl1.u64 = cvmx_read_csr(CVMX_LMCX_CTL1(ddr_interface_num));
        read_level_rankx.u64 = 0;

        rankmask = (num_ranks == 1) ? 0x1 : 0x3; /* mask for ranks present */
        /* Replicate dimm0 mask for the number of dimms present */
        for (idx=1; idx<dimm_count; ++idx) {
            rankmask |= (rankmask & 0x3) << (idx*2);
        }

        for (rankidx = 0; rankidx < 4; ++rankidx) {
            if ((rankmask >> rankidx) & 1) {
                if (sequence == 1) {
                    read_level_ctl.s.rankmask = 1 << rankidx;
                    ddr_config_write_csr(CVMX_LMCX_READ_LEVEL_CTL(ddr_interface_num), read_level_ctl.u64);
                    /* ddr_print("Read Level Rankmask                           : %#6x\n", read_level_ctl.s.rankmask); */

                    if (OCTEON_IS_OCTEON2())
                        lmc_ctl1.s.sequence = 1;
                    ddr_config_write_csr(CVMX_LMCX_CTL1(ddr_interface_num), lmc_ctl1.u64);
                    cvmx_read_csr(CVMX_LMCX_CTL1(ddr_interface_num));

                    /* Trigger init_start to allow read leveling to take effect */
                    mem_cfg0.u64 = cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num));
                    mem_cfg0.s.init_start   = 0; /* Insure that one shot is armed */
                    ddr_config_write_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64);
                    cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num));
                    mem_cfg0.s.init_start   = 1;
                    ddr_config_write_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64);
                    cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_interface_num));
                }

                if (sequence == 0) {
                    global_delay = board_delay * 4 / tclk_psecs;
                    read_level_rankx.s.byte0 = global_delay;
                    read_level_rankx.s.byte1 = global_delay;
                    read_level_rankx.s.byte2 = global_delay;
                    read_level_rankx.s.byte3 = global_delay;
                    read_level_rankx.s.byte4 = global_delay;
                    read_level_rankx.s.byte5 = global_delay;
                    read_level_rankx.s.byte6 = global_delay;
                    read_level_rankx.s.byte7 = global_delay;
                    read_level_rankx.s.byte8 = global_delay;

                    ddr_config_write_csr(CVMX_LMCX_READ_LEVEL_RANKX(rankidx, ddr_interface_num), read_level_rankx.u64);
                }

                if (sequence == 1) {
                    int read_level_bitmask[9];
                    int byte_idx;

                    /*
                     * A given read of LMC0/1_READ_LEVEL_DBG returns
                     * the read-leveling pass/fail results for all
                     * possible delay settings (i.e. the BITMASK) for
                     * only one byte in the last rank that the
                     * hardware ran read-leveling
                     * on. LMC0/1_READ_LEVEL_DBG[BYTE] selects the
                     * particular byte. To get these pass/fail results
                     * for a different rank, you must run the hardware
                     * read-leveling again. For example, it is
                     * possible to get the BITMASK results for every
                     * byte of every rank if you run read-leveling
                     * separately for each rank, probing
                     * LMC0/1_READ_LEVEL_DBG between each
                     * read-leveling.
                     */

                    for (byte_idx=0; byte_idx<9; ++byte_idx)
                        read_level_bitmask[byte_idx] = octeon_read_lmcx_read_level_dbg(ddr_interface_num, byte_idx);

                    ddr_print("Rank(%d) Read Level Debug Test Results 8:0     : %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
                              rankidx,
                              read_level_bitmask[8],
                              read_level_bitmask[7],
                              read_level_bitmask[6],
                              read_level_bitmask[5],
                              read_level_bitmask[4],
                              read_level_bitmask[3],
                              read_level_bitmask[2],
                              read_level_bitmask[1],
                              read_level_bitmask[0]
                              );

                    read_level_rankx.s.byte8 = adjust_read_level_byte(read_level_bitmask[8]);
                    read_level_rankx.s.byte7 = adjust_read_level_byte(read_level_bitmask[7]);
                    read_level_rankx.s.byte6 = adjust_read_level_byte(read_level_bitmask[6]);
                    read_level_rankx.s.byte5 = adjust_read_level_byte(read_level_bitmask[5]);
                    read_level_rankx.s.byte4 = adjust_read_level_byte(read_level_bitmask[4]);
                    read_level_rankx.s.byte3 = adjust_read_level_byte(read_level_bitmask[3]);
                    read_level_rankx.s.byte2 = adjust_read_level_byte(read_level_bitmask[2]);
                    read_level_rankx.s.byte1 = adjust_read_level_byte(read_level_bitmask[1]);
                    read_level_rankx.s.byte0 = adjust_read_level_byte(read_level_bitmask[0]);
                    ddr_config_write_csr(CVMX_LMCX_READ_LEVEL_RANKX(rankidx, ddr_interface_num), read_level_rankx.u64);
                }

                read_level_rankx.u64 = cvmx_read_csr(CVMX_LMCX_READ_LEVEL_RANKX(rankidx, ddr_interface_num));
                ddr_print("Rank(%d) Status / Deskew Bytes 8:0      %#6x : %4d %4d %4d %4d %4d %4d %4d %4d %4d\n",
                          rankidx,
                          read_level_rankx.s.status,
                          read_level_rankx.s.byte8,
                          read_level_rankx.s.byte7,
                          read_level_rankx.s.byte6,
                          read_level_rankx.s.byte5,
                          read_level_rankx.s.byte4,
                          read_level_rankx.s.byte3,
                          read_level_rankx.s.byte2,
                          read_level_rankx.s.byte1,
                          read_level_rankx.s.byte0
                          );
                if ((sequence == 1) && DEBUG_DEFINED) {
                    uint32_t avg_board_delay;
                    avg_board_delay = (( read_level_rankx.s.byte8 + read_level_rankx.s.byte7 + read_level_rankx.s.byte6 +
                                         read_level_rankx.s.byte5 + read_level_rankx.s.byte4 + read_level_rankx.s.byte3 +
                                         read_level_rankx.s.byte2 + read_level_rankx.s.byte1 + read_level_rankx.s.byte0 ) / 9 ) * tclk_psecs / 4;
                    ddr_print("Equivalent Board Delay                        : %4d:%4d ps\n", avg_board_delay, avg_board_delay + tclk_psecs / 4);
                }
            }
        }
    }

    if ((octeon_is_cpuid(OCTEON_CN56XX)) || (octeon_is_cpuid(OCTEON_CN52XX)) || (octeon_is_cpuid(OCTEON_CN58XX))) {
        cvmx_lmcx_nxm_t lmc_nxm;
        static const unsigned char _cs_mask_interface_wide  [] = {(unsigned char)~0x33, (unsigned char)~0xFF};
        static const unsigned char _cs_mask_interface_narrow[] = {(unsigned char)~0x03, (unsigned char)~0x0F, (unsigned char)~0x3F, (unsigned char)~0xFF};
        lmc_nxm.u64 = 0;

        /*
        **  128 bit interface
        **  dimm 0,1 cs_0,cs_4 Rank 0,0 mask 0x11
        **  dimm 0,1 cs_1,cs_5 Rank 1,1 mask 0x22
        **
        **  dimm 2,3 cs_2,cs_6 Rank 0,0 mask 0x44
        **  dimm 2,3 cs_3,cs_7 Rank 1,1 mask 0x88
        **
        **  64 bit interface
        **  dimm 0   cs_0      Rank 0,0 mask 0x01
        **  dimm 0   cs_1      Rank 1,1 mask 0x02
        **
        **  dimm 1   cs_2      Rank 0,0 mask 0x04
        **  dimm 1   cs_3      Rank 1,1 mask 0x08
        **
        **  dimm 2   cs_4      Rank 0,0 mask 0x10
        **  dimm 2   cs_5      Rank 1,1 mask 0x20
        **
        **  dimm 3   cs_6      Rank 0,0 mask 0x40
        **  dimm 3   cs_7      Rank 1,1 mask 0x80
        */

        if (ddr_interface_width > 64)
            lmc_nxm.s.cs_mask = _cs_mask_interface_wide  [(dimm_count/2)-1]; /* Get mask from table */
        else
            lmc_nxm.s.cs_mask = _cs_mask_interface_narrow[dimm_count-1]; /* Get mask from table */

        ddr_print("lmc_nxm                                       :     %02x\n", lmc_nxm.u64);
        ddr_config_write_csr(CVMX_LMCX_NXM(ddr_interface_num), lmc_nxm.u64);
    }


#ifdef PRINT_LMC_REGS
    printf("##################################################################\n"
           "LMC register dump for interface %d\n",ddr_interface_num);
    octeon_print_lmc_regs(cpu_id, ddr_interface_num);
    puts("##################################################################\n");
#endif

    return mem_size_mbytes;
}
#endif /* CONFIG_OCTEON_DISABLE_DDR2 */
#ifndef CONFIG_OCTEON_OCX
static int limit_l2_ways(int ways, int verbose)
{
    uint32_t valid_mask = (0x1 << cvmx_l2c_get_num_assoc()) - 1;
    int ways_max = cvmx_l2c_get_num_assoc();
    int ways_min;
    uint32_t mask = (valid_mask << ways) & valid_mask;
    int num_cores = cvmx_octeon_num_cores();
    int errors = 0;

    if (OCTEON_IS_OCTEON2() || OCTEON_IS_OCTEON3())
        ways_min = 0;
    else
        ways_min = 1;

    if (ways >= ways_min && ways <= ways_max) {
        int i;
        if (verbose)
            printf("Limiting L2 to %d ways\n", ways);
        for (i = 0; i < num_cores; i++)
            errors += cvmx_l2c_set_core_way_partition(i, mask);
        errors += cvmx_l2c_set_hw_way_partition(mask);
    } else {
        errors++;
        printf("ERROR: invalid limit_l2_ways %d, must be between %d and %d\n",
               ways, ways_min, ways_max);
    }
    if (errors)
        puts("ERROR limiting L2 cache ways\n");

    return errors;
}
#endif
#if !defined(CONFIG_OCTEON_DISABLE_DDR3) && !defined(__U_BOOT__)
static int limit_l2_mask(uint32_t mask)
{
    int num_assoc;
    uint32_t valid_mask;
    int errors = 0;
    int num_cores = cvmx_octeon_num_cores();

    num_assoc = cvmx_l2c_get_num_assoc();

    valid_mask = (0x1 << num_assoc) - 1;
    if ((mask & valid_mask) == mask) {
        int i;
        for (i = 0; i < num_cores; i++)
            errors += cvmx_l2c_set_core_way_partition(i, mask);
        errors += cvmx_l2c_set_hw_way_partition(mask);
    } else {
        errors++;
        printf("ERROR: invalid limit_l2_mask 0x%x, valid mask = 0x%x\n",
               mask, valid_mask);
    }
    if (errors)
        puts("ERROR limiting L2 cache ways\n");

    return errors;
}
#endif

/*
 * Suggested testing patterns.
 *
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0xAAAA_AAAA_AAAA_AAAA
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0xAAAA_AAAA_AAAA_AAAA
 *  0x5555_5555_5555_5555
 *  0xAAAA_AAAA_AAAA_AAAA
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0xAAAA_AAAA_AAAA_AAAA
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0x5555_5555_5555_5555
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0x5555_5555_5555_5555
 *  0xAAAA_AAAA_AAAA_AAAA
 *  0x5555_5555_5555_5555
 *  0xFFFF_FFFF_FFFF_FFFF
 *  0x5555_5555_5555_5555
 *
 *  or possibly
 *
 *  0xFDFD_FDFD_FDFD_FDFD
 *  0x8787_8787_8787_8787
 *  0xFEFE_FEFE_FEFE_FEFE
 *  0xC3C3_C3C3_C3C3_C3C3
 *  0x7F7F_7F7F_7F7F_7F7F
 *  0xE1E1_E1E1_E1E1_E1E1
 *  0xBFBF_BFBF_BFBF_BFBF
 *  0xF0F0_F0F0_F0F0_F0F0
 *  0xDFDF_DFDF_DFDF_DFDF
 *  0x7878_7878_7878_7878
 *  0xEFEF_EFEF_EFEF_EFEF
 *  0x3C3C_3C3C_3C3C_3C3C
 *  0xF7F7_F7F7_F7F7_F7F7
 *  0x1E1E_1E1E_1E1E_1E1E
 *  0xFBFB_FBFB_FBFB_FBFB
 *  0x0F0F_0F0F_0F0F_0F0F
 */

static const uint64_t test_pattern[] = {
    0x327B23C66B8B4567ULL,
    0x66334873643C9869ULL,
    0x19495CFF74B0DC51ULL,
    0x625558EC2AE8944AULL,
    0x46E87CCD238E1F29ULL,
    0x507ED7AB3D1B58BAULL,
    0x41B71EFB2EB141F2ULL,
    0x7545E14679E2A9E3ULL,
    0x5BD062C2515F007CULL,
    0x4DB127F812200854ULL,
    0x1F16E9E80216231BULL,
    0x66EF438D1190CDE7ULL,
    0x3352255A140E0F76ULL,
    0x0DED7263109CF92EULL,
    0x1BEFD79F7FDCC233ULL,
    0x6B68079A41A7C4C9ULL,
    0x25E45D324E6AFB66ULL,
    0x431BD7B7519B500DULL,
    0x7C83E4583F2DBA31ULL,
    0x62BBD95A257130A3ULL,
    0x628C895D436C6125ULL,
    0x721DA317333AB105ULL,
    0x2D1D5AE92443A858ULL,
    0x75A2A8D46763845EULL,
    0x79838CB208EDBDABULL,
    0x0B03E0C64353D0CDULL,
    0x54E49EB4189A769BULL,
    0x2CA8861171F32454ULL,
    0x02901D820836C40EULL,
    0x081386413A95F874ULL,
    0x7C3DBD3D1E7FF521ULL,
    0x6CEAF087737B8DDCULL,
};

#define test_dram_byte_print ddr_print

static int test_dram_byte(uint64_t p, int count, int byte, uint64_t bitmask)
{
    uint64_t p1, p2, d1, d2;
    uint64_t v, v1;
    uint64_t p2offset = 0x4000000;
    uint64_t datamask;
    int i, j, k;
    int errors = 0;
    int counter;
    int index;
    uint32_t cpu_id = cvmx_get_proc_id();
    int bitno = (OCTEON_IS_OCTEON3()) ? 20: 18;

    if ((octeon_is_cpuid(OCTEON_CN73XX)) || (octeon_is_cpuid(OCTEON_CNF75XX)))
        bitno = 18;

    datamask = bitmask << (8*byte);

    p |= 1ull<<63;

    /* Add offset to both test regions to not clobber u-boot stuff
     * when running from L2 for NAND boot.
     */
    p += 0x4000000;

    counter = 0;
    /* Store zeros into each location first */
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
        for (j = 0; j < (1 << 12); j += (1 << 9)) {
            for (i = 0; i < (1 << 7); i += 8) {
                index = i + j + k;
                p1 = p + index;
                p2 = p1 + p2offset;

                cvmx_write64_uint64(p1, 0ULL);
                cvmx_write64_uint64(p2, 0ULL);
#ifdef __U_BOOT__
            /* Write back and invalidate the cache lines
             *
             * For OCX we cannot limit the number of L2 ways
             * so instead we just write back and invalidate
             * the L2 cache lines.  This is not possible
             * when booting remotely, however so this is
             * only enabled for U-Boot right now.
             * Potentially the BDK can also take advantage
             * of this.
             */
            CVMX_CACHE_WBIL2(p1, 0);
            CVMX_CACHE_WBIL2(p2, 0);
#endif
                ++counter;
            }
        }
    }
#ifdef __U_BOOT__
    CVMX_DCACHE_INVALIDATE;
#endif
    counter = 0;
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
        for (j = 0; j < (1 << 12); j += (1 << 9)) {
            for (i = 0; i < (1 << 7); i += 8) {
                index = i + j + k;
                p1 = p + index;
                p2 = p1 + p2offset;
                v = test_pattern[(index/sizeof(uint64_t))%(sizeof(test_pattern)/sizeof(uint64_t))];
                v &= datamask;
                v1 = ~v;

                /* test_dram_byte_print("[0x%016llX]: 0x%016llX, [0x%016llX]: 0x%016llX\n",
                 *            p1, v, p2, v1);
                 */

                cvmx_write64_uint64(p1, v);
                cvmx_write64_uint64(p2, v1);
#ifdef __U_BOOT__
            /* Write back and invalidate the cache lines
             *
             * For OCX we cannot limit the number of L2 ways
             * so instead we just write back and invalidate
             * the L2 cache lines.  This is not possible
             * when booting remotely, however so this is
             * only enabled for U-Boot right now.
             * Potentially the BDK can also take advantage
             * of this.
             */
            CVMX_CACHE_WBIL2(p1, 0);
            CVMX_CACHE_WBIL2(p2, 0);
#endif
                ++counter;
            }
        }
    }
#ifdef __U_BOOT__
    CVMX_DCACHE_INVALIDATE;
#endif
    counter = 0;

    /* Walk through a range of addresses avoiding bits that alias
     * interfaces on the CN68XX.
     */
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
        for (i = 0; i < (1 << 7); i += 8) {
            for (j = 0; j < (1 << 12); j += (1 << 9)) {
                index = i + j + k;
                p1 = p + index;
                p2 = p1 + p2offset;
                v = test_pattern[(index/sizeof(uint64_t))%(sizeof(test_pattern)/sizeof(uint64_t))];
                v &= datamask;
                d1 = cvmx_read64_uint64(p1);
                d1 &= datamask;
                d2 = ~cvmx_read64_uint64(p2);
                d2 &= datamask;

                /* test_dram_byte_print("[0x%016llX]: 0x%016llX, [0x%016llX]: 0x%016llX\n",
                 *             p1, d1, p2, d2);
                 */

                if (d1 != v) {
                    ++errors;
                    debug_print("%d: [0x%016llX] 0x%016llX expected 0x%016llX xor %016llX\n",
                                errors, p1, d1, v, (d1 ^ v));
                    return errors;      /* Quit on first error */
                }
                if (d2 != v) {
                    ++errors;
                    debug_print("%d: [0x%016llX] 0x%016llX  expected 0x%016llX xor %016llX\n",
                                errors, p2, d2, v, (d2 ^ v));
                    return errors;      /* Quit on first error */
                }
                ++counter;
            }
        }
    }
    return errors;
}

#ifndef CONFIG_OCTEON_DISABLE_DDR3
#define WLEVEL_BYTE_BITS 5
#define WLEVEL_BYTE_MSK  ((1ULL << 5) - 1)

static void update_wlevel_rank_struct(cvmx_lmcx_wlevel_rankx_t *lmc_wlevel_rank,
				      int byte, int delay)
{
    cvmx_lmcx_wlevel_rankx_t temp_wlevel_rank;
    if (byte >= 0 && byte <= 8) {
	temp_wlevel_rank.u64 = lmc_wlevel_rank->u64;
	temp_wlevel_rank.u64 &= ~(WLEVEL_BYTE_MSK << (WLEVEL_BYTE_BITS * byte));
	temp_wlevel_rank.u64 |= ((delay & WLEVEL_BYTE_MSK) << (WLEVEL_BYTE_BITS * byte));
	lmc_wlevel_rank->u64 = temp_wlevel_rank.u64;
    }
}

static int  get_wlevel_rank_struct(cvmx_lmcx_wlevel_rankx_t *lmc_wlevel_rank,
				   int byte)
{
    int delay = 0;
    if (byte >= 0 && byte <= 8) {
	delay = ((lmc_wlevel_rank->u64) >> (WLEVEL_BYTE_BITS * byte)) & WLEVEL_BYTE_MSK;
    }
    return delay;
}

#define RLEVEL_BYTE_BITS 6
#define RLEVEL_BYTE_MSK  ((1ULL << 6) - 1)

static void update_rlevel_rank_struct(cvmx_lmcx_rlevel_rankx_t *lmc_rlevel_rank,
				      int byte, int delay)
{
    cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank;
    if (byte >= 0 && byte <= 8) {
	temp_rlevel_rank.u64 = lmc_rlevel_rank->u64 & ~(RLEVEL_BYTE_MSK << (RLEVEL_BYTE_BITS * byte));
	temp_rlevel_rank.u64 |= ((delay & RLEVEL_BYTE_MSK) << (RLEVEL_BYTE_BITS * byte));
	lmc_rlevel_rank->u64 = temp_rlevel_rank.u64;
    }
}

static int  get_rlevel_rank_struct(cvmx_lmcx_rlevel_rankx_t *lmc_rlevel_rank,
				   int byte)
{
    int delay = 0;
    if (byte >= 0 && byte <= 8) {
	delay = ((lmc_rlevel_rank->u64) >> (RLEVEL_BYTE_BITS * byte)) & RLEVEL_BYTE_MSK;
    }
    return delay;
}

static void rlevel_to_wlevel(cvmx_lmcx_rlevel_rankx_t *lmc_rlevel_rank,
			     cvmx_lmcx_wlevel_rankx_t *lmc_wlevel_rank, int byte)
{
    int byte_delay = get_rlevel_rank_struct(lmc_rlevel_rank, byte);

    debug_print("Estimating Wlevel delay byte %d: ", byte);
    debug_print("Rlevel=%d => ", byte_delay);
    byte_delay = divide_roundup(byte_delay,2) & 0x1e;
    debug_print("Wlevel=%d\n", byte_delay);
    update_wlevel_rank_struct(lmc_wlevel_rank, byte, byte_delay);
}

/* Delay trend: constant=0, decreasing=-1, increasing=1 */
static int64_t calc_delay_trend(int64_t v)
{
    if (v == 0)
        return (0);
    if (v < 0)
        return (-1);
    return 1;
}

typedef struct {
    int delay;
    int loop_total;
    int loop_count;
    int best;
    uint64_t bm;
    int bmerrs;
    int sqerrs;
    int bestsq;
} rlevel_byte_data_t;

/* Evaluate delay sequence across the whole range of byte delays while
** keeping track of the overall delay trend, increasing or decreasing.
** If the trend changes charge an error amount to the score.
*/

// NOTE: "max_adj_delay_inc" argument is, by default, 1 for DDR3 and 2 for DDR4

static int nonsequential_delays(rlevel_byte_data_t *rlevel_byte,
				int start, int end, int max_adj_delay_inc)
{
    int64_t error = 0;
    int64_t delay_trend, prev_trend = 0;
    int byte_idx;
    int64_t seq_err;
    int64_t adj_err;
    int64_t delay_inc;
    int64_t delay_diff;

    for (byte_idx = start; byte_idx < end; ++byte_idx) {

        delay_diff = rlevel_byte[byte_idx+1].delay - rlevel_byte[byte_idx].delay;
	delay_trend = calc_delay_trend(delay_diff);

        /* Increment error each time the trend changes to the opposite direction.
         */
	if ((prev_trend != 0) && (delay_trend != 0) && (prev_trend != delay_trend)) {
            seq_err = RLEVEL_NONSEQUENTIAL_DELAY_ERROR;
	} else {
            seq_err = 0;
        }

	delay_inc = _abs(delay_diff); // how big was the delay change, if any

        /* Even if the trend did not change to the opposite direction, check for
           the magnitude of the change, and scale the penalty by the amount that
           the size is larger than the provided limit.
         */
	if ((max_adj_delay_inc != 0) && (delay_inc > max_adj_delay_inc)) {
	    adj_err = (delay_inc - max_adj_delay_inc) * RLEVEL_ADJACENT_DELAY_ERROR;
	} else {
            adj_err = 0;
        }

        rlevel_byte[byte_idx+1].sqerrs = seq_err + adj_err;
        error += seq_err + adj_err;

        debug_bitmask_print("Byte %d: %d, Byte %d: %d, delay_trend: %ld, prev_trend: %ld, [%ld/%ld]%s%s\n",
                            byte_idx+0, rlevel_byte[byte_idx+0].delay,
                            byte_idx+1, rlevel_byte[byte_idx+1].delay,
                            delay_trend, prev_trend, seq_err, adj_err,
                            (seq_err) ? " => Nonsequential byte delay" : "",
                            (adj_err) ? " => Adjacent delay error" : ""
                            );

	if (delay_trend != 0) {
	    prev_trend = delay_trend;
        }
    }
    return (int)error;
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

    delay = (leader & 1) ? leader + 1 : leader;
    delay = delay % 8;

    return delay;
}

void perform_ddr3_sequence(uint32_t cpu_id, int rank_mask,
			   int ddr_interface_num, int sequence)
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
    static const char *sequence_str[] = {
        "power-up/init",
        "read-leveling",
        "self-refresh entry",
        "self-refresh exit",
        "precharge power-down entry",
        "precharge power-down exit",
        "write-leveling",
        "illegal"
    };
#endif

    cvmx_lmcx_control_t lmc_control;
    cvmx_lmcx_config_t lmc_config;
    int save_ddr2t;

    lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
    save_ddr2t                    = lmc_control.s.ddr2t;

    if ((save_ddr2t == 0) && octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
        /* Some register parts (IDT and TI included) do not like
         * the sequence that LMC generates for an MRS register
         * write in 1T mode. In this case, the register part does
         * not properly forward the MRS register write to the DRAM
         * parts.  See errata (LMC-14548) Issues with registered
         * DIMMs.
         */
        ddr_print("Forcing DDR 2T during init seq. Re: Pass 1 LMC-14548\n");
        lmc_control.s.ddr2t           = 1;
    }

    if ((s = lookup_env_parameter("ddr_init_2t")) != NULL)
        lmc_control.s.ddr2t = simple_strtoul(s, NULL, 0);

    ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num),
                         lmc_control.u64);


    lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));

    lmc_config.s.init_start      = 1;
    if (OCTEON_IS_OCTEON2())
        lmc_config.cn63xx.sequence        = sequence;
    lmc_config.s.rankmask        = rank_mask;

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
    ddr_print("Performing LMC sequence: rank_mask=0x%02x, sequence=%d, %s\n",
              rank_mask, sequence, sequence_str[sequence]);
#endif

    if ((s = lookup_env_parameter("ddr_trigger_sequence%d", sequence)) != NULL) {
        int trigger = simple_strtoul(s, NULL, 0);
        if (trigger)
            pulse_gpio_pin(1, 2);
    }

    ddr_config_write_csr(CVMX_LMCX_CONFIG(ddr_interface_num),
                         lmc_config.u64);
    cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
    cvmx_wait_usec(600); /* Wait a while */

    lmc_control.s.ddr2t           = save_ddr2t;
    ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num),
                         lmc_control.u64);
    cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
}

#define SET_DDR_DLL_CTL3(field, expr)                   \
    do {                                                \
        if (octeon_is_cpuid(OCTEON_CN66XX) ||           \
    	    octeon_is_cpuid(OCTEON_CN63XX))             \
    	    ddr_dll_ctl3.cn63xx.field = (expr);         \
        else if (octeon_is_cpuid(OCTEON_CN68XX) ||      \
                 octeon_is_cpuid(OCTEON_CN61XX) ||      \
                 octeon_is_cpuid(OCTEON_CNF71XX))       \
    	    ddr_dll_ctl3.cn61xx.field = (expr);         \
        else if (octeon_is_cpuid(OCTEON_CN70XX) ||	\
                 octeon_is_cpuid(OCTEON_CN78XX))        \
    	    ddr_dll_ctl3.cn70xx.field = (expr);         \
        else if (octeon_is_cpuid(OCTEON_CN73XX) ||	\
                 octeon_is_cpuid(OCTEON_CNF75XX))       \
    	    ddr_dll_ctl3.cn73xx.field = (expr);         \
        else                                            \
    	    ddr_print("%s(): " #field                   \
                      "not set for unknown chip\n",     \
                      __func__);                        \
    } while (0)

#define ENCODE_DLL90_BYTE_SEL(byte_sel) (octeon_is_cpuid(OCTEON_CN70XX) ? ((9+7-(byte_sel))%9) : ((byte_sel)+1))

#ifdef DDR_NO_DEBUG
/**
 * If debugging is disabled the ddr_print macro is not compatible
 * with this macro.
 */
# define GET_DDR_DLL_CTL3(field)                         \
    ((octeon_is_cpuid(OCTEON_CN66XX) ||                  \
      octeon_is_cpuid(OCTEON_CN63XX)) ?                  \
        ddr_dll_ctl3.cn63xx.field :                      \
        (octeon_is_cpuid(OCTEON_CN68XX) ||               \
         octeon_is_cpuid(OCTEON_CN61XX) ||               \
         octeon_is_cpuid(OCTEON_CNF71XX)) ?              \
            ddr_dll_ctl3.cn61xx.field :                  \
            (octeon_is_cpuid(OCTEON_CN70XX) ||           \
             octeon_is_cpuid(OCTEON_CN78XX)) ?           \
                ddr_dll_ctl3.cn70xx.field :              \
                (octeon_is_cpuid(OCTEON_CN73XX) ||       \
                 octeon_is_cpuid(OCTEON_CNF75XX)) ?      \
		    ddr_dll_ctl3.cn73xx.field : 0)       \

#else
# define GET_DDR_DLL_CTL3(field)                        \
    ((octeon_is_cpuid(OCTEON_CN66XX) ||                 \
      octeon_is_cpuid(OCTEON_CN63XX)) ?                 \
        ddr_dll_ctl3.cn63xx.field :                     \
        ((octeon_is_cpuid(OCTEON_CN68XX) ||             \
          octeon_is_cpuid(OCTEON_CN61XX) ||             \
          octeon_is_cpuid(OCTEON_CNF71XX)) ?            \
            ddr_dll_ctl3.cn61xx.field :                 \
            ((octeon_is_cpuid(OCTEON_CN70XX) ||         \
              octeon_is_cpuid(OCTEON_CN78XX)) ?         \
               ddr_dll_ctl3.cn70xx.field :              \
               ((octeon_is_cpuid(OCTEON_CN73XX) ||      \
                 octeon_is_cpuid(OCTEON_CNF75XX)) ?     \
                   ddr_dll_ctl3.cn73xx.field :         \
                   (ddr_print("%s(): " #field " not retrieved for unknown chip\n", __func__), 0)))))
#endif

/* Check to see if any custom offset values are used */
static int is_dll_offset_provided(const int8_t *dll_offset_table)
{
    int i;

    if (dll_offset_table == NULL) /* Check for pointer to table. */
        return (0);

    for (i=0; i<9; ++i) {
        if (dll_offset_table[i] != 0)
            return (1);
    }
    return (0);
}

void
change_dll_offset_enable(uint32_t cpu_id, int node, int ddr_interface_num, int change)
{
    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;

    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));
    SET_DDR_DLL_CTL3(offset_ena, !!change);
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));
}

unsigned short
load_dll_offset(int node, int ddr_interface_num,
                uint32_t cpu_id, int dll_offset_mode,
                int byte_offset, int byte)
{
    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
    int field_width = 6;
    /* byte_sel:
       0x1 = byte 0, ..., 0x9 = byte 8
       0xA = all bytes */
    int byte_sel = (byte == 10) ? byte : byte + 1;

    if (octeon_is_cpuid(OCTEON_CN6XXX))
        field_width = 5;

    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));
    SET_DDR_DLL_CTL3(load_offset, 0);
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    SET_DDR_DLL_CTL3(mode_sel, dll_offset_mode);
    SET_DDR_DLL_CTL3(offset, (_abs(byte_offset)&(~(-1<<field_width))) | (_sign(byte_offset)<<field_width));
    SET_DDR_DLL_CTL3(byte_sel, byte_sel);
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    SET_DDR_DLL_CTL3(load_offset, 1);
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    return ((unsigned short) GET_DDR_DLL_CTL3(offset));
}

static void
process_custom_dll_offsets(int node, int ddr_interface_num, uint32_t cpu_id, const char *enable_str,
                           const int8_t *offsets, const char *byte_str, int mode)
{
    const char *s;
    int enabled;
    int provided;
    int byte_offset;
    unsigned short offset[9] = {0};
    int byte;


    if ((s = lookup_env_parameter(enable_str)) != NULL) {
	enabled = !!simple_strtol(s, NULL, 0);
    } else
	enabled = -1;

    // enabled == -1: no override, do only configured offsets if provided
    // enabled ==  0: override OFF, do NOT do it even if configured offsets provided
    // enabled ==  1: override ON, do it for overrides plus configured offsets

    if (enabled == 0)
	return;

    provided = is_dll_offset_provided(offsets);

    if (enabled < 0 && !provided)
	return;

    change_dll_offset_enable(cpu_id, node, ddr_interface_num, 0);

    for (byte = 0; byte < 9; ++byte) {

	// always take the provided, if available
	byte_offset = (provided) ? offsets[byte] : 0;

	// then, if enabled, use any overrides present
	if (enabled > 0) {
	    if ((s = lookup_env_parameter(byte_str, ddr_interface_num, byte)) != NULL) {
                byte_offset = simple_strtol(s, NULL, 0);
            }
	}

	offset[byte] = load_dll_offset(node, ddr_interface_num, cpu_id, mode, byte_offset, byte);
    }

    change_dll_offset_enable(cpu_id, node, ddr_interface_num, 1);

    ddr_print("N%d.LMC%d: DLL %s Offset 8:0       :"
	      "  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x\n",
	      node, ddr_interface_num, (mode == 2) ? "Read " : "Write",
	      offset[8], offset[7], offset[6], offset[5], offset[4],
	      offset[3], offset[2], offset[1], offset[0]);
}

#ifdef CONFIG_OCTEON_CN70XX
#include "octeon3_cn70xx_lmc.h"
#else
#include "octeon3_lmc.h"
#endif

static void set_ddr_memory_preserved(void)
{
#ifdef __U_BOOT__
    gd->flags |=  GD_FLG_MEMORY_PRESERVED;
#else
    global_ddr_memory_preserved |= 0x1;
#endif

}
static int ddr_memory_preserved(void)
{
#ifdef __U_BOOT__
    return (!!(gd->flags & GD_FLG_MEMORY_PRESERVED));
#else
    return (!!(global_ddr_memory_preserved));
#endif
}

static void perform_ddr_init_sequence(uint32_t cpu_id, int node, int rank_mask,
				       int ddr_interface_num)
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif
    int ddr_init_loops = 1;
    int rankx;

#if defined(__U_BOOT__) && !defined(CONFIG_OCTEON_OCX)
    node = 0;
#endif
    if ((s = lookup_env_parameter("ddr%d_init_loops", ddr_interface_num)) != NULL)
        ddr_init_loops = simple_strtoul(s, NULL, 0);

    while (ddr_init_loops--) {
        for (rankx = 0; rankx < 8;rankx++) {
            if (!(rank_mask & (1 << rankx)))
                continue;

            if (OCTEON_IS_OCTEON3())
                perform_octeon3_ddr3_sequence(cpu_id, node, (1 << rankx),
                                              ddr_interface_num, 0); /* power-up/init */
            else
                perform_ddr3_sequence(cpu_id, (1 << rankx),
                                      ddr_interface_num, 0); /* power-up/init */

            cvmx_wait_usec(1000);   /* Wait a while. */

            if ((s = lookup_env_parameter("ddr_sequence1")) != NULL) {
                int sequence1;
                sequence1 = simple_strtoul(s, NULL, 0);

                if (OCTEON_IS_OCTEON3())
                    perform_octeon3_ddr3_sequence(cpu_id, node, (1 << rankx),
                                                  ddr_interface_num, sequence1);
                else
                    perform_ddr3_sequence(cpu_id, (1 << rankx),
                                          ddr_interface_num, sequence1);
            }

            if ((s = lookup_env_parameter("ddr_sequence2")) != NULL) {
                int sequence2;
                sequence2 = simple_strtoul(s, NULL, 0);

                if (OCTEON_IS_OCTEON3())
                    perform_octeon3_ddr3_sequence(cpu_id, node, (1 << rankx),
                                                  ddr_interface_num, sequence2);
                else
                    perform_ddr3_sequence(cpu_id, (1 << rankx),
                                          ddr_interface_num, sequence2);
            }
	}
    }
}

static int init_octeon_ddr3_interface(uint32_t cpu_id,
				      const ddr_configuration_t *ddr_configuration,
				      uint32_t ddr_hertz,
				      uint32_t cpu_hertz,
				      uint32_t ddr_ref_hertz,
				      int board_type,
				      int board_rev_maj,
				      int board_rev_min,
				      int ddr_interface_num,
				      uint32_t ddr_interface_mask)
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

    const dimm_odt_config_t *odt_1rank_config = ddr_configuration->odt_1rank_config;
    const dimm_odt_config_t *odt_2rank_config = ddr_configuration->odt_2rank_config;
    const dimm_odt_config_t *odt_4rank_config = ddr_configuration->odt_4rank_config;
    const dimm_config_t *dimm_config_table = ddr_configuration->dimm_config_table;
    const dimm_odt_config_t *odt_config;
    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;
    int odt_idx;

    /*
    ** Compute clock rates to the nearest picosecond.
    */
    ulong tclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, ddr_hertz); /* Clock in psecs */
    ulong eclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, cpu_hertz); /* Clock in psecs */

    int row_bits, col_bits, num_banks, num_ranks, dram_width, real_num_ranks;
    int dimm_count = 0;
    int fatal_error = 0;        /* Accumulate and report all the errors before giving up */

    int safe_ddr_flag = 0; /* Flag that indicates safe DDR settings should be used */
    int ddr_interface_64b = 1;  /* Octeon II Default: 64bit interface width */
    int ddr_interface_bytemask;
    uint32_t mem_size_mbytes = 0;
    unsigned int didx;
    int bank_bits = 0;
    int bunk_enable;
    int rank_mask;
    int column_bits_start = 1;
    int row_lsb;
    int pbank_lsb;
    int use_ecc = 1;
    int mtb_psec;
    short ftb_Dividend;
    short ftb_Divisor;
    int tAAmin;
    int tCKmin;
    int CL, min_cas_latency = 0, max_cas_latency = 0, override_cas_latency = 0;
    int ddr_rtt_nom_auto, ddr_rodt_ctl_auto;
    int i;

    int spd_addr;
    int spd_org;
    int spd_rdimm;
    int spd_dimm_type;
    int spd_ecc;
    int spd_cas_latency;
    int spd_mtb_dividend;
    int spd_mtb_divisor;
    int spd_tck_min;
    int spd_taa_min;
    int spd_twr;
    int spd_trcd;
    int spd_trrd;
    int spd_trp;
    int spd_tras;
    int spd_trc;
    int spd_trfc;
    int spd_twtr;
    int spd_trtp;
    int spd_tfaw;
    int spd_addr_mirror;

    /* FTB values are two's complement ranging from +127 to -128. */
    typedef signed char SC_t;

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

    int wlevel_bitmask_errors = 0;
    int wlevel_loops = 0;
    int default_rtt_nom[4];
    int dyn_rtt_nom_mask;
    static const int node = 0;

    ddr_print("\nInitializing DDR interface %d, DDR Clock %d, DDR Reference Clock %d, CPUID 0x%08x\n",
              ddr_interface_num, ddr_hertz, ddr_ref_hertz, cpu_id);

    if (dimm_config_table[0].spd_addrs[0] == 0 && !dimm_config_table[0].spd_ptrs[0]) {
        error_print("ERROR: No dimms specified in the dimm_config_table.\n");
        return (-1);
    }

    if (ddr_verbose()) {
        printf("DDR SPD Table:");
        for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx) {
            if (dimm_config_table[didx].spd_addrs[0] == 0) break;
            printf(" --ddr%dspd=0x%02x", ddr_interface_num, dimm_config_table[didx].spd_addrs[0]);
            if (dimm_config_table[didx].spd_addrs[1] != 0)
                printf(",0x%02x", dimm_config_table[didx].spd_addrs[1]);
        }
        printf("\n");
    }

    /*
    ** Walk the DRAM Socket Configuration Table to see what is installed.
    */
    for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx)
    {
        /* Check for lower DIMM socket populated */
        if (validate_dimm(&dimm_config_table[didx], 0)) {
            if (ddr_verbose())
                report_ddr3_dimm(&dimm_config_table[didx], 0, dimm_count, ddr_interface_num);
            ++dimm_count;
        } else { break; }       /* Finished when there is no lower DIMM */
    }


    initialize_ddr_clock(cpu_id,
			 ddr_configuration,
			 cpu_hertz,
			 ddr_hertz,
			 ddr_ref_hertz,
			 ddr_interface_num,
			 ddr_interface_mask,
			 0);

    if (!odt_1rank_config)
        odt_1rank_config = disable_odt_config;
    if (!odt_2rank_config)
        odt_2rank_config = disable_odt_config;
    if (!odt_4rank_config)
        odt_4rank_config = disable_odt_config;

    if ((s = getenv("ddr_safe")) != NULL) {
        safe_ddr_flag = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_safe = %d\n", safe_ddr_flag);
    }


    if (dimm_count == 0) {
        error_print("ERROR: DIMM 0 not detected.\n");
        return(-1);
    }

    if (custom_lmc_config->mode32b)
        ddr_interface_64b = 0;

    if ((s = lookup_env_parameter("ddr_interface_64b")) != NULL) {
        ddr_interface_64b = simple_strtoul(s, NULL, 0);
    }

    if (ddr_interface_64b == 0) {
        if ( !(octeon_is_cpuid(OCTEON_CN61XX) || octeon_is_cpuid(OCTEON_CNF71XX))) {
            error_print("32-bit interface width is not supported for this Octeon model\n");
            ++fatal_error;
        }
    }

    spd_addr = read_spd(&dimm_config_table[0], 0, DDR3_SPD_ADDRESSING_ROW_COL_BITS);
    debug_print("spd_addr        : %#06x\n", spd_addr );
    row_bits = 12 + ((spd_addr >> 3) & 0x7);
    col_bits =  9 + ((spd_addr >> 0) & 0x7);

    num_banks = 1 << (3+((read_spd(&dimm_config_table[0], 0, DDR3_SPD_DENSITY_BANKS) >> 4) & 0x7));

    spd_org = read_spd(&dimm_config_table[0], 0, DDR3_SPD_MODULE_ORGANIZATION);
    debug_print("spd_org         : %#06x\n", spd_org );
    num_ranks =  1 + ((spd_org >> 3) & 0x7);
    dram_width = 4 << ((spd_org >> 0) & 0x7);



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



    spd_dimm_type   = read_spd(&dimm_config_table[0], 0, DDR3_SPD_KEY_BYTE_MODULE_TYPE);
    spd_rdimm       = (spd_dimm_type == 1) || (spd_dimm_type == 5) || (spd_dimm_type == 9);

    if ((s = lookup_env_parameter("ddr_rdimm_ena")) != NULL) {
        spd_rdimm = simple_strtoul(s, NULL, 0);
    }

    /*  Avoid using h/w write-leveling for pass 1.0 and 1.1 silicon.
        This bug is partially fixed in pass 1.2-1.x. (See LMC-14415B) */
    if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_0) || octeon_is_cpuid(OCTEON_CN63XX_PASS1_1)) {
        ddr_print("Disabling H/W Write-leveling. Re: Pass 1.0-1.1 LMC-14415B\n");
        wlevel_loops = 0;
    } else
        wlevel_loops = 1;

    /* Avoid using h/w write-leveling for pass 1.X silicon with dual
       ranked, registered dimms.  See errata (LMC-14548) Issues with
       registered DIMMs. */
    if (spd_rdimm && (num_ranks==2) && octeon_is_cpuid(OCTEON_CN63XX_PASS1_X))
        wlevel_loops = 0;

    if ((s = lookup_env_parameter("ddr%d_wlevel_loops", ddr_interface_num)) != NULL) {
        wlevel_loops = simple_strtoul(s, NULL, 0);
    }

    real_num_ranks = num_ranks;

    if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
        /* NOTICE: Ignoring second rank of registered DIMMs if using
           h/w write-leveling for pass 1. See errata (LMC-14548)
           Issues with registered DIMMs. */
        num_ranks = (spd_rdimm && wlevel_loops) ? 1 : real_num_ranks;
    }

    if ((s = lookup_env_parameter("ddr_ranks")) != NULL) {
        num_ranks = simple_strtoul(s, NULL, 0);
    }

    if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
        /* NOTICE: Ignoring second rank of registered DIMMs if using
           h/w write-leveling for pass 1. See errata (LMC-14548)
           Issues with registered DIMMs. */
        if ((spd_rdimm) && (num_ranks != real_num_ranks))  {
            ddr_print("Ignoring second rank of Registered DIMMs. Re: Pass 1 LMC-14548\n");
        }
    }

    bunk_enable = (num_ranks > 1);

    if (octeon_is_cpuid(OCTEON_CN6XXX) || octeon_is_cpuid(OCTEON_CNF7XXX))
        column_bits_start = 3;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    row_lsb = column_bits_start + col_bits + bank_bits - (! ddr_interface_64b);
    debug_print("row_lsb = column_bits_start + col_bits + bank_bits = %d\n", row_lsb);

    pbank_lsb = row_lsb + row_bits + bunk_enable;
    debug_print("pbank_lsb = row_lsb + row_bits + bunk_enable = %d\n", pbank_lsb);


    mem_size_mbytes =  dimm_count * ((1ull << pbank_lsb) >> 20);
    if (num_ranks == 4) {
        /* Quad rank dimm capacity is equivalent to two dual-rank dimms. */
        mem_size_mbytes *= 2;
    }

    /* Mask with 1 bits set for for each active rank, allowing 2 bits per dimm.
    ** This makes later calculations simpler, as a variety of CSRs use this layout.
    ** This init needs to be updated for dual configs (ie non-identical DIMMs).
    ** Bit 0 = dimm0, rank 0
    ** Bit 1 = dimm0, rank 1
    ** Bit 2 = dimm1, rank 0
    ** Bit 3 = dimm1, rank 1
    ** ...
    */
    rank_mask = 0x1;
    if (num_ranks > 1)
        rank_mask = 0x3;
    if (num_ranks > 2)
        rank_mask = 0xf;

    for (i = 1; i < dimm_count; i++)
        rank_mask |= ((rank_mask & 0x3) << (2*i));



    ddr_print("row bits: %d, col bits: %d, banks: %d, ranks: %d, dram width: %d, size: %d MB\n",
              row_bits, col_bits, num_banks, num_ranks, dram_width, mem_size_mbytes);

#if defined(__U_BOOT__)
    {
        /* If we are booting from RAM, the DRAM controller is already set up.  Just return the
        ** memory size */
        if (gd->flags & GD_FLG_RAM_RESIDENT) {
            ddr_print("Ram Boot: Skipping LMC config\n");
            return mem_size_mbytes;
        }
    }
#endif


    spd_ecc         = !!(read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEMORY_BUS_WIDTH) & 8);

    spd_cas_latency  = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_CAS_LATENCIES_LSB);
    spd_cas_latency |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_CAS_LATENCIES_MSB)) << 8);

    spd_mtb_dividend = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEDIUM_TIMEBASE_DIVIDEND);
    spd_mtb_divisor  = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEDIUM_TIMEBASE_DIVISOR);
    spd_tck_min      = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MINIMUM_CYCLE_TIME_TCKMIN);
    spd_taa_min      = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_CAS_LATENCY_TAAMIN);

    spd_twr          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_WRITE_RECOVERY_TWRMIN);
    spd_trcd         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_RAS_CAS_DELAY_TRCDMIN);
    spd_trrd         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ROW_ACTIVE_DELAY_TRRDMIN);
    spd_trp          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ROW_PRECHARGE_DELAY_TRPMIN);
    spd_tras         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ACTIVE_PRECHARGE_LSB_TRASMIN);
    spd_tras        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLES_TRAS_TRC)&0xf) << 8);
    spd_trc          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ACTIVE_REFRESH_LSB_TRCMIN);
    spd_trc         |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLES_TRAS_TRC)&0xf0) << 4);
    spd_trfc         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_REFRESH_RECOVERY_LSB_TRFCMIN);
    spd_trfc        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_REFRESH_RECOVERY_MSB_TRFCMIN)) << 8);
    spd_twtr         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_INTERNAL_WRITE_READ_CMD_TWTRMIN);
    spd_trtp         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_INTERNAL_READ_PRECHARGE_CMD_TRTPMIN);
    spd_tfaw         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_FOUR_ACTIVE_WINDOW_TFAWMIN);
    spd_tfaw        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLE_TFAW)&0xf) << 8);
    spd_addr_mirror  = 0xff & read_spd(&dimm_config_table[0], 0,DDR3_SPD_ADDRESS_MAPPING) & 0x1;
    spd_addr_mirror  = spd_addr_mirror && !spd_rdimm; /* Only address mirror unbuffered dimms.  */
    ftb_Dividend     = read_spd(&dimm_config_table[0], 0, DDR3_SPD_FINE_TIMEBASE_DIVIDEND_DIVISOR) >> 4;
    ftb_Divisor      = read_spd(&dimm_config_table[0], 0, DDR3_SPD_FINE_TIMEBASE_DIVIDEND_DIVISOR) & 0xf;
    ftb_Divisor      = (ftb_Divisor == 0) ? 1 : ftb_Divisor; /* Make sure that it is not 0 */

    if ((s = getenv("ddr_use_ecc")) != NULL) {
        use_ecc = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_use_ecc = %d\n", use_ecc);
    }
    use_ecc = use_ecc && spd_ecc;

    ddr_interface_bytemask = ddr_interface_64b
        ? (use_ecc ? 0x1ff : 0xff)
        : (use_ecc ? 0x01f : 0x0f);

    ddr_print("DRAM Interface width: %d bits %s\n",
              ddr_interface_64b ? 64 : 32, use_ecc ? "+ECC" : "");


    debug_print("spd_cas_latency : %#06x\n", spd_cas_latency );
    debug_print("spd_twr         : %#06x\n", spd_twr );
    debug_print("spd_trcd        : %#06x\n", spd_trcd);
    debug_print("spd_trrd        : %#06x\n", spd_trrd);
    debug_print("spd_trp         : %#06x\n", spd_trp );
    debug_print("spd_tras        : %#06x\n", spd_tras);
    debug_print("spd_trc         : %#06x\n", spd_trc );
    debug_print("spd_trfc        : %#06x\n", spd_trfc);
    debug_print("spd_twtr        : %#06x\n", spd_twtr);
    debug_print("spd_trtp        : %#06x\n", spd_trtp);
    debug_print("spd_tfaw        : %#06x\n", spd_tfaw);
    debug_print("spd_addr_mirror : %#06x\n", spd_addr_mirror);

    ddr_print("\n------ Board Custom Configuration Settings ------\n");
    ddr_print("%-45s : %d\n", "MIN_RTT_NOM_IDX   ", custom_lmc_config->min_rtt_nom_idx);
    ddr_print("%-45s : %d\n", "MAX_RTT_NOM_IDX   ", custom_lmc_config->max_rtt_nom_idx);
    ddr_print("%-45s : %d\n", "MIN_RODT_CTL      ", custom_lmc_config->min_rodt_ctl);
    ddr_print("%-45s : %d\n", "MAX_RODT_CTL      ", custom_lmc_config->max_rodt_ctl);
    ddr_print("%-45s : %d\n", "DQX_CTL           ", custom_lmc_config->dqx_ctl);
    ddr_print("%-45s : %d\n", "CK_CTL            ", custom_lmc_config->ck_ctl);
    ddr_print("%-45s : %d\n", "CMD_CTL           ", custom_lmc_config->cmd_ctl);
    ddr_print("%-45s : %d\n", "MIN_CAS_LATENCY   ", custom_lmc_config->min_cas_latency);
    ddr_print("%-45s : %d\n", "OFFSET_EN         ", custom_lmc_config->offset_en);
    ddr_print("%-45s : %d\n", "OFFSET_UDIMM      ", custom_lmc_config->offset_udimm);
    ddr_print("%-45s : %d\n", "OFFSET_RDIMM      ", custom_lmc_config->offset_rdimm);
    ddr_print("%-45s : %d\n", "DDR_RTT_NOM_AUTO  ", custom_lmc_config->ddr_rtt_nom_auto);
    ddr_print("%-45s : %d\n", "DDR_RODT_CTL_AUTO ", custom_lmc_config->ddr_rodt_ctl_auto);
    if (spd_rdimm)
        ddr_print("%-45s : %d\n", "RLEVEL_COMP_OFFSET", custom_lmc_config->rlevel_comp_offset_rdimm);
    else
        ddr_print("%-45s : %d\n", "RLEVEL_COMP_OFFSET", custom_lmc_config->rlevel_comp_offset_udimm);
    ddr_print("%-45s : %d\n", "RLEVEL_COMPUTE    ", custom_lmc_config->rlevel_compute);
    ddr_print("%-45s : %d\n", "DDR2T_UDIMM       ", custom_lmc_config->ddr2t_udimm);
    ddr_print("%-45s : %d\n", "DDR2T_RDIMM       ", custom_lmc_config->ddr2t_rdimm);
    ddr_print("%-45s : %d\n", "FPRCH2            ", custom_lmc_config->fprch2);
    ddr_print("-------------------------------------------------\n");

    mtb_psec        = spd_mtb_dividend * 1000 / spd_mtb_divisor;
    tAAmin          = mtb_psec * spd_taa_min;
    tAAmin         += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_CAS_LATENCY_FINE_TAAMIN) / ftb_Divisor;
    tCKmin          = mtb_psec * spd_tck_min;
    tCKmin         += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MINIMUM_CYCLE_TIME_FINE_TCKMIN) / ftb_Divisor;

    CL              = divide_roundup(tAAmin, tclk_psecs);

    ddr_print("Desired CAS Latency                           : %6d\n", CL);

    min_cas_latency = custom_lmc_config->min_cas_latency;


    if ((s = lookup_env_parameter("ddr_min_cas_latency")) != NULL) {
        min_cas_latency = simple_strtoul(s, NULL, 0);
    }

    ddr_print("CAS Latencies supported in DIMM               :");
    for (i=0; i<16; ++i) {
        if ((spd_cas_latency >> i) & 1) {
            ddr_print(" %d", i+4);
            max_cas_latency = i+4;
            if (min_cas_latency == 0)
                min_cas_latency = i+4;
        }
    }
    ddr_print("\n");


    /* Use relaxed timing when running slower than the minimum
       supported speed.  Adjust timing to match the smallest supported
       CAS Latency. */
    if (CL < min_cas_latency) {
        ulong adjusted_tclk = tAAmin / min_cas_latency;
        CL = min_cas_latency;
        ddr_print("Slow clock speed. Adjusting timing: tClk = %d, Adjusted tClk = %d\n",
                  tclk_psecs, adjusted_tclk);
        tclk_psecs = adjusted_tclk;
    }

    if ((s = getenv("ddr_cas_latency")) != NULL) {
        override_cas_latency = simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_cas_latency = %d\n", override_cas_latency);
    }

    /* Make sure that the selected cas latency is legal */
    for (i=(CL-4); i<16; ++i) {
        if ((spd_cas_latency >> i) & 1) {
            CL = i+4;
            break;
        }
    }

    if (CL > max_cas_latency)
        CL = max_cas_latency;

    if (override_cas_latency != 0) {
        CL = override_cas_latency;
    }

    ddr_print("CAS Latency                                   : %6d\n", CL);

    if ((CL * tCKmin) > 20000)
    {
        ddr_print("(CLactual * tCKmin) = %d exceeds 20 ns\n", (CL * tCKmin));
    }

    if (tclk_psecs < (ulong)tCKmin)
        error_print("WARNING!!!!!!: DDR3 Clock Rate (tCLK: %ld) exceeds DIMM specifications (tCKmin:%ld)!!!!!!!!\n",
                    tclk_psecs, (ulong)tCKmin);

    twr             = spd_twr  * mtb_psec;
    trcd            = spd_trcd * mtb_psec;
    trcd           += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_RAS_CAS_DELAY_FINE_TRCDMIN) / ftb_Divisor;
    trrd            = spd_trrd * mtb_psec;
    trp             = spd_trp  * mtb_psec;
    trp            += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ROW_PRECHARGE_DELAY_FINE_TRPMIN) / ftb_Divisor;
    tras            = spd_tras * mtb_psec;
    trc             = spd_trc  * mtb_psec;
    trc            += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ACTIVE_REFRESH_LSB_FINE_TRCMIN) / ftb_Divisor;
    trfc            = spd_trfc * mtb_psec;
    twtr            = spd_twtr * mtb_psec;
    trtp            = spd_trtp * mtb_psec;
    tfaw            = spd_tfaw * mtb_psec;


    ddr_print("DDR Clock Rate (tCLK)                         : %6d ps\n", tclk_psecs);
    ddr_print("Core Clock Rate (eCLK)                        : %6d ps\n", eclk_psecs);
    ddr_print("Medium Timebase (MTB)                         : %6d ps\n", mtb_psec);
    ddr_print("Minimum Cycle Time (tCKmin)                   : %6d ps\n", tCKmin);
    ddr_print("Minimum CAS Latency Time (tAAmin)             : %6d ps\n", tAAmin);
    ddr_print("Write Recovery Time (tWR)                     : %6d ps\n", twr);
    ddr_print("Minimum RAS to CAS delay (tRCD)               : %6d ps\n", trcd);
    ddr_print("Minimum Row Active to Row Active delay (tRRD) : %6d ps\n", trrd);
    ddr_print("Minimum Row Precharge Delay (tRP)             : %6d ps\n", trp);
    ddr_print("Minimum Active to Precharge (tRAS)            : %6d ps\n", tras);
    ddr_print("Minimum Active to Active/Refresh Delay (tRC)  : %6d ps\n", trc);
    ddr_print("Minimum Refresh Recovery Delay (tRFC)         : %6d ps\n", trfc);
    ddr_print("Internal write to read command delay (tWTR)   : %6d ps\n", twtr);
    ddr_print("Min Internal Rd to Precharge Cmd Delay (tRTP) : %6d ps\n", trtp);
    ddr_print("Minimum Four Activate Window Delay (tFAW)     : %6d ps\n", tfaw);


    if ((num_banks != 4) && (num_banks != 8))
    {
        error_print("Unsupported number of banks %d. Must be 4 or 8.\n", num_banks);
        ++fatal_error;
    }

    if ((num_ranks != 1) && (num_ranks != 2) && (num_ranks != 4))
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

    odt_idx = min(dimm_count - 1, 3);

    switch (num_ranks) {
    case 1:
        odt_config = odt_1rank_config;
        break;
    case 2:
        odt_config = odt_2rank_config;
        break;
    case 4:
        odt_config = odt_4rank_config;
        break;
    default:
        odt_config = disable_odt_config;
        error_print("Unsupported number of ranks: %d\n", num_ranks);
        ++fatal_error;
    }


    /* Parameters from DDR3 Specifications */
#define DDR3_tREFI         7800000    /* 7.8 us */
#define DDR3_ZQCS          80000ull   /* 80 ns */
#define DDR3_ZQCS_Interval 1280000000 /* 128ms/100 */
#define DDR3_tCKE          5000       /* 5 ns */
#define DDR3_tMRD          4          /* 4 nCK */
#define DDR3_tDLLK         512        /* 512 nCK */
#define DDR3_tMPRR         1          /* 1 nCK */
#define DDR3_tWLMRD        40         /* 40 nCK */
#define DDR3_tWLDQSEN      25         /* 25 nCK */

    /*
     * 4.8.5 Early LMC Initialization
     *
     * All of DDR PLL, LMC CK, and LMC DRESET initializations must
     * be completed prior to starting this LMC initialization
     * sequence.
     *
     * 1. Software must ensure there are no pending DRAM transactions.
     *
     * 2. Write LMC0_CONFIG, LMC0_CONTROL, LMC0_TIMING_PARAMS0,
     *    LMC0_TIMING_PARAMS1, LMC0_MODEREG_PARAMS0,
     *    LMC0_MODEREG_PARAMS1, LMC0_DUAL_MEMCFG, LMC0_NXM,
     *    LMC0_WODT_MASK, LMC0_RODT_MASK, LMC0_COMP_CTL2,
     *    LMC0_PHY_CTL, LMC0_DIMM0/1_PARAMS, and LMC0_DIMM_CTL
     *    with appropriate values. All sections in this chapter
     *    may be used to derive proper register settings.
     */

    /* LMC0_CONFIG */
    {
        cvmx_lmcx_config_t lmc_config;
        lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
        lmc_config.s.init_start      = 0;
        lmc_config.s.ecc_ena         = use_ecc;
        lmc_config.s.row_lsb         = encode_row_lsb_ddr3(cpu_id, row_lsb, ddr_interface_64b);
        lmc_config.s.pbank_lsb       = encode_pbank_lsb_ddr3(cpu_id, pbank_lsb, ddr_interface_64b);

        lmc_config.s.idlepower       = 0; /* Disabled */

        if ((s = lookup_env_parameter("ddr_idlepower")) != NULL) {
            lmc_config.s.idlepower = simple_strtoul(s, NULL, 0);
        }

        lmc_config.s.forcewrite      = 0; /* Disabled */
        lmc_config.s.ecc_adr         = 1; /* Include memory reference address in the ECC */

        if ((s = lookup_env_parameter("ddr_ecc_adr")) != NULL) {
            lmc_config.s.ecc_adr = simple_strtoul(s, NULL, 0);
        }

        lmc_config.s.reset           = 0;

        /*
         *  Program LMC0_CONFIG[24:18], ref_zqcs_int(6:0) to
         *  RND-DN(tREFI/clkPeriod/512) Program LMC0_CONFIG[36:25],
         *  ref_zqcs_int(18:7) to
         *  RND-DN(ZQCS_Interval/clkPeriod/(512*128)). Note that this
         *  value should always be greater than 32, to account for
         *  resistor calibration delays.
         */

        lmc_config.cn63xx.ref_zqcs_int     = ((DDR3_tREFI/tclk_psecs/512) & 0x7f);
        lmc_config.cn63xx.ref_zqcs_int    |= ((max(33ull, (DDR3_ZQCS_Interval/(tclk_psecs/100)/(512*128))) & 0xfff) << 7);
        lmc_config.cn63xx.sequence        = 0; /* Set later */

        lmc_config.s.early_dqx       = 0; /* disabled */

        if ((s = lookup_env_parameter("ddr_early_dqx")) == NULL)
            s = lookup_env_parameter("ddr%d_early_dqx", ddr_interface_num);
        if (s != NULL) {
            lmc_config.s.early_dqx = simple_strtoul(s, NULL, 0);
        }

        lmc_config.s.sref_with_dll   = 0;
        lmc_config.s.rank_ena        = bunk_enable;
        lmc_config.s.rankmask        = rank_mask; /* Set later */
        lmc_config.s.mirrmask        = (spd_addr_mirror << 1 | spd_addr_mirror << 3) & rank_mask;
        lmc_config.s.init_status     = rank_mask; /* Set once and don't change it. */


        if ((s = lookup_env_parameter_ull("ddr_config")) != NULL) {
            lmc_config.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_CONFIG                                    : 0x%016llx\n",
                  (unsigned long long)lmc_config.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
    }

    /* LMC0_CONTROL */
    {
        cvmx_lmcx_control_t lmc_control;
        lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
        lmc_control.s.rdimm_ena       = spd_rdimm;
        lmc_control.s.bwcnt           = 0; /* Clear counter later */
        if (spd_rdimm)
            lmc_control.s.ddr2t       = (safe_ddr_flag ? 1 : custom_lmc_config->ddr2t_rdimm );
        else
            lmc_control.s.ddr2t       = (safe_ddr_flag ? 1 : custom_lmc_config->ddr2t_udimm );
        lmc_control.s.pocas           = 0;
        lmc_control.s.fprch2          = (safe_ddr_flag ? 2 : custom_lmc_config->fprch2 );
        lmc_control.s.throttle_rd     = safe_ddr_flag ? 1 : 0;
        lmc_control.s.throttle_wr     = safe_ddr_flag ? 1 : 0;
        lmc_control.s.inorder_rd      = safe_ddr_flag ? 1 : 0;
        lmc_control.s.inorder_wr      = safe_ddr_flag ? 1 : 0;
        lmc_control.s.elev_prio_dis   = safe_ddr_flag ? 1 : 0;
        lmc_control.s.nxm_write_en    = 0; /* discards writes to
                                              addresses that don't exist
                                              in the DRAM */
        lmc_control.s.max_write_batch = 8;
        lmc_control.s.xor_bank        = 1;
        lmc_control.s.auto_dclkdis    = 1;
        lmc_control.s.int_zqcs_dis    = 0;
        lmc_control.s.ext_zqcs_dis    = 0;
        lmc_control.s.bprch           = 0;
        lmc_control.s.wodt_bprch      = 0;
        lmc_control.s.rodt_bprch      = 0;

        if ((s = lookup_env_parameter("ddr_xor_bank")) != NULL) {
            lmc_control.s.xor_bank = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_2t")) != NULL) {
            lmc_control.s.ddr2t = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_fprch2")) != NULL) {
            lmc_control.s.fprch2 = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_bprch")) != NULL) {
            lmc_control.s.bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_wodt_bprch")) != NULL) {
            lmc_control.s.wodt_bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rodt_bprch")) != NULL) {
            lmc_control.s.rodt_bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_int_zqcs_dis")) != NULL) {
            lmc_control.s.int_zqcs_dis = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_ext_zqcs_dis")) != NULL) {
            lmc_control.s.ext_zqcs_dis = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter_ull("ddr_control")) != NULL) {
            lmc_control.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_CONTROL                                   : 0x%016llx\n",
                  (unsigned long long)lmc_control.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);
    }

    /* LMC0_TIMING_PARAMS0 */
    {
        unsigned trp_value;
        cvmx_lmcx_timing_params0_t lmc_timing_params0;
        lmc_timing_params0.u64 = cvmx_read_csr(CVMX_LMCX_TIMING_PARAMS0(ddr_interface_num));

        trp_value = divide_roundup(trp, tclk_psecs)
            + divide_roundup(max(4*tclk_psecs, 7500ull), tclk_psecs) - 5;

        if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            /* Reserved. Should be written to zero. */
            lmc_timing_params0.cn63xxp1.tckeon   = 0;
        }
        lmc_timing_params0.cn61xx.tzqcs    = divide_roundup(max(64*tclk_psecs, DDR3_ZQCS), (16*tclk_psecs));
        lmc_timing_params0.cn61xx.tcke     = divide_roundup(DDR3_tCKE, tclk_psecs) - 1;
        lmc_timing_params0.cn61xx.txpr     = divide_roundup(max(5*tclk_psecs, trfc+10000ull), 16*tclk_psecs);
        lmc_timing_params0.cn61xx.tmrd     = divide_roundup((DDR3_tMRD*tclk_psecs), tclk_psecs) - 1;
        lmc_timing_params0.cn61xx.tmod     = divide_roundup(max(12*tclk_psecs, 15000ull), tclk_psecs) - 1;
        lmc_timing_params0.cn61xx.tdllk    = divide_roundup(DDR3_tDLLK, 256);
        lmc_timing_params0.cn61xx.tzqinit  = divide_roundup(max(512*tclk_psecs, 640000ull), (256*tclk_psecs));
        lmc_timing_params0.cn61xx.trp      = trp_value & 0xf;
        lmc_timing_params0.cn61xx.tcksre   = divide_roundup(max(5*tclk_psecs, 10000ull), tclk_psecs) - 1;

        if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            lmc_timing_params0.cn61xx.trp_ext  = (trp_value >> 4) & 1;
        }

        if ((s = lookup_env_parameter_ull("ddr_timing_params0")) != NULL) {
            lmc_timing_params0.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("TIMING_PARAMS0                                : 0x%016llx\n",
                  (unsigned long long)lmc_timing_params0.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_TIMING_PARAMS0(ddr_interface_num),
				  lmc_timing_params0.u64);
    }

    /* LMC0_TIMING_PARAMS1 */
    {
        int txp;
        cvmx_lmcx_timing_params1_t lmc_timing_params1;
        lmc_timing_params1.u64 = cvmx_read_csr_node(node,
						    CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));
        lmc_timing_params1.s.tmprr    = divide_roundup(DDR3_tMPRR*tclk_psecs, tclk_psecs) - 1;

        lmc_timing_params1.cn61xx.tras     = divide_roundup(tras, tclk_psecs) - 1;
        lmc_timing_params1.cn61xx.trcd     = divide_roundup(trcd, tclk_psecs);
        lmc_timing_params1.cn61xx.twtr     = divide_roundup(twtr, tclk_psecs) - 1;
        lmc_timing_params1.cn61xx.trfc     = divide_roundup(trfc, 8*tclk_psecs);
        lmc_timing_params1.cn61xx.trrd     = divide_roundup(trrd, tclk_psecs) - 2;

        /*
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-800   tCLK = 2500 psec
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-1066  tCLK = 1875 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1333  tCLK = 1500 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1600  tCLK = 1250 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1866  tCLK = 1071 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-2133  tCLK =  937 psec
        */
        txp = (tclk_psecs < 1875) ? 6000 : 7500;
        lmc_timing_params1.cn61xx.txp      = divide_roundup(max(3*tclk_psecs, (unsigned)txp), tclk_psecs) - 1;

        lmc_timing_params1.cn61xx.twlmrd   = divide_roundup(DDR3_tWLMRD*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn61xx.twldqsen = divide_roundup(DDR3_tWLDQSEN*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn61xx.tfaw     = divide_roundup(tfaw, 4*tclk_psecs);
        lmc_timing_params1.cn61xx.txpdll   = divide_roundup(max(10*tclk_psecs, 24000ull), tclk_psecs) - 1;
        if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            lmc_timing_params1.cn61xx.tras_ext = ((divide_roundup(tras, tclk_psecs) - 1) >> 5) & 1;
        }

        if ((s = lookup_env_parameter_ull("ddr_timing_params1")) != NULL) {
            lmc_timing_params1.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("TIMING_PARAMS1                                : 0x%016llx\n",
                  (unsigned long long)lmc_timing_params1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num),
				  lmc_timing_params1.u64);
    }

    /* LMC0_MODEREG_PARAMS0 */
    {
        cvmx_lmcx_modereg_params0_t lmc_modereg_params0;
        int param;

        lmc_modereg_params0.u64 = cvmx_read_csr(CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));


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

        lmc_modereg_params0.s.cwl     = 0;
        if (tclk_psecs < 2500)
            lmc_modereg_params0.s.cwl = 1;
        if (tclk_psecs < 1875)
            lmc_modereg_params0.s.cwl = 2;
        if (tclk_psecs < 1500)
            lmc_modereg_params0.s.cwl = 3;
        if (tclk_psecs < 1250)
            lmc_modereg_params0.s.cwl = 4;
        if (tclk_psecs < 1070)
            lmc_modereg_params0.s.cwl = 5;
        if (tclk_psecs <  935)
            lmc_modereg_params0.s.cwl = 6;
        if (tclk_psecs <  833)
            lmc_modereg_params0.s.cwl = 7;

        if ((s = lookup_env_parameter("ddr_cwl")) != NULL) {
            lmc_modereg_params0.s.cwl = simple_strtoul(s, NULL, 0) - 5;
        }

        ddr_print("CAS Write Latency                             : %6d\n", lmc_modereg_params0.s.cwl + 5);

        lmc_modereg_params0.s.mprloc  = 0;
        lmc_modereg_params0.s.mpr     = 0;
        lmc_modereg_params0.s.dll     = 0;
        lmc_modereg_params0.s.al      = 0;
        lmc_modereg_params0.s.wlev    = 0; /* Read Only */
        lmc_modereg_params0.s.tdqs    = 0;
        lmc_modereg_params0.s.qoff    = 0;
        //lmc_modereg_params0.s.bl      = 0; /* Don't touch block dirty logic */

        if ((s = lookup_env_parameter("ddr_cl")) != NULL) {
            CL = simple_strtoul(s, NULL, 0);
            ddr_print("CAS Latency                                   : %6d\n", CL);
        }

        lmc_modereg_params0.s.cl      = 0x2;
        if (CL > 5)
            lmc_modereg_params0.s.cl  = 0x4;
        if (CL > 6)
            lmc_modereg_params0.s.cl  = 0x6;
        if (CL > 7)
            lmc_modereg_params0.s.cl  = 0x8;
        if (CL > 8)
            lmc_modereg_params0.s.cl  = 0xA;
        if (CL > 9)
            lmc_modereg_params0.s.cl  = 0xC;
        if (CL > 10)
            lmc_modereg_params0.s.cl  = 0xE;
        if (CL > 11)
            lmc_modereg_params0.s.cl  = 0x1;
        if (CL > 12)
            lmc_modereg_params0.s.cl  = 0x3;
        if (CL > 13)
            lmc_modereg_params0.s.cl  = 0x5;
        if (CL > 14)
            lmc_modereg_params0.s.cl  = 0x7;
        if (CL > 15)
            lmc_modereg_params0.s.cl  = 0x9;

        lmc_modereg_params0.s.rbt     = 0; /* Read Only. */
        lmc_modereg_params0.s.tm      = 0;
        lmc_modereg_params0.s.dllr    = 0;

        param = divide_roundup(twr, tclk_psecs);
        lmc_modereg_params0.s.wrp     = 1;
        if (param > 5)
            lmc_modereg_params0.s.wrp = 2;
        if (param > 6)
            lmc_modereg_params0.s.wrp = 3;
        if (param > 7)
            lmc_modereg_params0.s.wrp = 4;
        if (param > 8)
            lmc_modereg_params0.s.wrp = 5;
        if (param > 10)
            lmc_modereg_params0.s.wrp = 6;
        if (param > 12)
            lmc_modereg_params0.s.wrp = 7;

        lmc_modereg_params0.s.ppd     = 0;

        if ((s = lookup_env_parameter("ddr_wrp")) != NULL) {
            lmc_modereg_params0.s.wrp = simple_strtoul(s, NULL, 0);
        }

        ddr_print("Write recovery for auto precharge (WRP)       : %6d\n", lmc_modereg_params0.s.wrp);

        if ((s = lookup_env_parameter_ull("ddr_modereg_params0")) != NULL) {
            lmc_modereg_params0.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("MODEREG_PARAMS0                               : 0x%016llx\n",
                  (unsigned long long)lmc_modereg_params0.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
				  lmc_modereg_params0.u64);
    }

    /* LMC0_WODT_MASK */
    {
        cvmx_lmcx_wodt_mask_t lmc_wodt_mask;
        lmc_wodt_mask.u64 = odt_config[odt_idx].odt_mask;

        if ((s = lookup_env_parameter_ull("ddr_wodt_mask")) != NULL) {
            lmc_wodt_mask.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("WODT_MASK                                     : 0x%016llx\n",
                  (unsigned long long)lmc_wodt_mask.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_WODT_MASK(ddr_interface_num),
				  lmc_wodt_mask.u64);
    }

    /* LMC0_RODT_MASK */
    {
        int rankx;
        cvmx_lmcx_rodt_mask_t lmc_rodt_mask;
        lmc_rodt_mask.u64 = odt_config[odt_idx].rodt_ctl;

        if ((s = lookup_env_parameter_ull("ddr_rodt_mask")) != NULL) {
            lmc_rodt_mask.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("%-45s : 0x%016llx\n", "RODT_MASK",
                  (unsigned long long)lmc_rodt_mask.u64);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_RODT_MASK(ddr_interface_num),
                                  lmc_rodt_mask.u64);

        dyn_rtt_nom_mask = 0;
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            if (!(rank_mask & (1 << rankx)))
                continue;
            dyn_rtt_nom_mask |= ((lmc_rodt_mask.u64 >> (8*rankx)) & 0xff);
        }
        if (num_ranks == 4) {
            /* Normally ODT1 is wired to rank 1. For quad-ranked DIMMs
               ODT1 is wired to the third rank (rank 2).  The mask,
               dyn_rtt_nom_mask, is used to indicate for which ranks
               to sweep RTT_NOM during read-leveling. Shift the bit
               from the ODT1 position over to the "ODT2" position so
               that the read-leveling analysis comes out right. */
            int odt1_bit = dyn_rtt_nom_mask & 2;
            dyn_rtt_nom_mask &= ~2;
            dyn_rtt_nom_mask |= odt1_bit<<1;
        }
        ddr_print("%-45s : 0x%02x\n", "DYN_RTT_NOM_MASK", dyn_rtt_nom_mask);
    }

    /* LMC0_MODEREG_PARAMS1 */
    {
        cvmx_lmcx_modereg_params1_t lmc_modereg_params1;
        static const unsigned char rtt_nom_ohms[] = { 0, 60, 120, 40, 20, 30, 0, 0};
        static const unsigned char rtt_wr_ohms[]  = { 0, 60, 120, 0};
        static const unsigned char dic_ohms[]     = { 40, 34, 0, 0 };

        lmc_modereg_params1.u64 = odt_config[odt_idx].modereg_params1.u64;


        if ((s = lookup_env_parameter("ddr_rtt_nom_mask")) != NULL) {
            dyn_rtt_nom_mask    = simple_strtoul(s, NULL, 0);
        }


        /* Save the original rtt_nom settings before sweeping through settings. */
        default_rtt_nom[0] = lmc_modereg_params1.s.rtt_nom_00;
        default_rtt_nom[1] = lmc_modereg_params1.s.rtt_nom_01;
        default_rtt_nom[2] = lmc_modereg_params1.s.rtt_nom_10;
        default_rtt_nom[3] = lmc_modereg_params1.s.rtt_nom_11;

        ddr_rtt_nom_auto = custom_lmc_config->ddr_rtt_nom_auto;

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_nom_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_nom_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x7  << (i*12+9));
                lmc_modereg_params1.u64 |=  ( (value & 0x7) << (i*12+9));
                default_rtt_nom[i] = value;
                ddr_rtt_nom_auto = 0;
            }
        }


        if ((s = lookup_env_parameter("ddr_rtt_nom")) == NULL)
            s = lookup_env_parameter("ddr%d_rtt_nom", ddr_interface_num);
        if (s != NULL) {
            uint64_t value;
            value = simple_strtoul(s, NULL, 0);


            if (dyn_rtt_nom_mask & 1)
                default_rtt_nom[0] = lmc_modereg_params1.s.rtt_nom_00 = value;
            if (dyn_rtt_nom_mask & 2)
                default_rtt_nom[1] = lmc_modereg_params1.s.rtt_nom_01 = value;
            if (dyn_rtt_nom_mask & 4)
                default_rtt_nom[2] = lmc_modereg_params1.s.rtt_nom_10 = value;
            if (dyn_rtt_nom_mask & 8)
                default_rtt_nom[3] = lmc_modereg_params1.s.rtt_nom_11 = value;

            ddr_rtt_nom_auto = 0;
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_nom_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_nom_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+9));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+9));

                ddr_rtt_nom_auto = 0;
            }
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_wr_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_wr_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+5));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+5));
            }
        }

        if ((s = lookup_env_parameter("ddr_dic")) != NULL) {
            uint64_t value = simple_strtoul(s, NULL, 0);
            for (i=0; i<4; ++i) {
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+7));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+7));
            }
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_dic_%1d%1d", !!(i&2), !!(i&1))) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+7));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+7));
            }
        }

        if ((s = lookup_env_parameter_ull("ddr_modereg_params1")) != NULL) {
            lmc_modereg_params1.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
                  rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
                  rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
                  rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
                  lmc_modereg_params1.s.rtt_nom_11,
                  lmc_modereg_params1.s.rtt_nom_10,
                  lmc_modereg_params1.s.rtt_nom_01,
                  lmc_modereg_params1.s.rtt_nom_00);

        ddr_print("RTT_WR      %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_11],
                  rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_10],
                  rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_01],
                  rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_00],
                  lmc_modereg_params1.s.rtt_wr_11,
                  lmc_modereg_params1.s.rtt_wr_10,
                  lmc_modereg_params1.s.rtt_wr_01,
                  lmc_modereg_params1.s.rtt_wr_00);

        ddr_print("DIC         %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  dic_ohms[lmc_modereg_params1.s.dic_11],
                  dic_ohms[lmc_modereg_params1.s.dic_10],
                  dic_ohms[lmc_modereg_params1.s.dic_01],
                  dic_ohms[lmc_modereg_params1.s.dic_00],
                  lmc_modereg_params1.s.dic_11,
                  lmc_modereg_params1.s.dic_10,
                  lmc_modereg_params1.s.dic_01,
                  lmc_modereg_params1.s.dic_00);

        ddr_print("MODEREG_PARAMS1                               : 0x%016llx\n",
                  (unsigned long long)lmc_modereg_params1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num),
				  lmc_modereg_params1.u64);
    }


#if 0                           /* Deal with dual mem config later */
    /* LMC0_DUAL_MEMCFG */
    {
        cvmx_lmcx_dual_memcfg_t lmc_dual_memcfg;
        lmc_dual_memcfg.u64 = cvmx_read_csr(CVMX_LMCX_DUAL_MEMCFG(ddr_interface_num));
        lmc_dual_memcfg.s.cs_mask           =
            lmc_dual_memcfg.s.row_lsb           =
            if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
                lmc_dual_memcfg.s.bank8             =
            }
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_DUAL_MEMCFG(ddr_interface_num),
				  lmc_dual_memcfg.u64);
    }
#endif

    /* LMC0_NXM */
    {
        cvmx_lmcx_nxm_t lmc_nxm;
        lmc_nxm.u64 = cvmx_read_csr(CVMX_LMCX_NXM(ddr_interface_num));
        if (rank_mask & 0x1)
            lmc_nxm.cn68xx.mem_msb_d0_r0 = row_lsb + row_bits - 26;
        if (rank_mask & 0x2)
            lmc_nxm.cn68xx.mem_msb_d0_r1 = row_lsb + row_bits - 26;
        if (rank_mask & 0x4)
            lmc_nxm.cn68xx.mem_msb_d1_r0 = row_lsb + row_bits - 26;
        if (rank_mask & 0x8)
            lmc_nxm.cn68xx.mem_msb_d1_r1 = row_lsb + row_bits - 26;

        lmc_nxm.cn68xx.cs_mask = ~rank_mask & 0xff; /* Set the mask for non-existant ranks. */

        if ((s = lookup_env_parameter_ull("ddr_nxm")) != NULL) {
            lmc_nxm.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_NXM                                       : 0x%016llx\n",
                  (unsigned long long)lmc_nxm.u64);
        ddr_config_write_csr_node(node, CVMX_LMCX_NXM(ddr_interface_num),
				  lmc_nxm.u64);
    }

    /* LMC0_COMP_CTL2 */
    {
        cvmx_lmcx_comp_ctl2_t lmc_comp_ctl2;
        static const char *strength_str[] = {"none","24","26.67","30","34.3","40","48","60"};
        lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
        lmc_comp_ctl2.cn61xx.ntune              = 0;
        lmc_comp_ctl2.cn61xx.ptune              = 0;
        lmc_comp_ctl2.cn61xx.byp                = 0;
        lmc_comp_ctl2.cn61xx.m180               = 0;

        if ((s = lookup_env_parameter_ull("ddr_comp_ctl2")) != NULL) {
            lmc_comp_ctl2.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("COMP_CTL2                                     : 0x%016llx\n",
                  (unsigned long long)lmc_comp_ctl2.u64);

        ddr_print("CMD_CTL                                       : 0x%x (%s ohms)\n",
                  lmc_comp_ctl2.s.cmd_ctl, strength_str[lmc_comp_ctl2.s.cmd_ctl]);
        ddr_print("CK_CTL                                        : 0x%x (%s ohms)\n",
                  lmc_comp_ctl2.s.ck_ctl, strength_str[lmc_comp_ctl2.s.ck_ctl]);
        ddr_print("DQX_CTL                                       : 0x%x (%s ohms)\n",
                  lmc_comp_ctl2.s.dqx_ctl, strength_str[lmc_comp_ctl2.s.dqx_ctl]);
        ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                  lmc_comp_ctl2.cn61xx.ddr__ptune, lmc_comp_ctl2.cn61xx.ddr__ntune);


        ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				  lmc_comp_ctl2.u64);
    }

    /* LMC0_PHY_CTL */
    {
        cvmx_lmcx_phy_ctl_t lmc_phy_ctl;
        lmc_phy_ctl.u64 = cvmx_read_csr(CVMX_LMCX_PHY_CTL(ddr_interface_num));
        lmc_phy_ctl.s.ts_stagger           = 0;

        if ((s = lookup_env_parameter_ull("ddr_phy_ctl")) != NULL) {
            lmc_phy_ctl.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("PHY_CTL                                       : 0x%016llx\n",
                  (unsigned long long)lmc_phy_ctl.u64);
        ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num),
				  lmc_phy_ctl.u64);
    }

    /* LMC0_DIMM0_PARAMS */
    if (spd_rdimm) {
        for (didx=0; didx<(unsigned)dimm_count; ++didx) {
            cvmx_lmcx_dimmx_params_t lmc_dimmx_params;
            cvmx_lmcx_dimm_ctl_t lmc_dimm_ctl;
            int dimm = didx;
            int rc;

            lmc_dimmx_params.u64 = cvmx_read_csr(CVMX_LMCX_DIMMX_PARAMS(dimm, ddr_interface_num));

            rc = read_spd(&dimm_config_table[didx], 0, 69);
            lmc_dimmx_params.s.rc0         = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc1         = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 70);
            lmc_dimmx_params.s.rc2         = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc3         = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 71);
            lmc_dimmx_params.s.rc4         = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc5         = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 72);
            lmc_dimmx_params.s.rc6         = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc7         = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 73);
            lmc_dimmx_params.s.rc8         = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc9         = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 74);
            lmc_dimmx_params.s.rc10        = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc11        = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 75);
            lmc_dimmx_params.s.rc12        = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc13        = (rc >> 4) & 0xf;

            rc = read_spd(&dimm_config_table[didx], 0, 76);
            lmc_dimmx_params.s.rc14        = (rc >> 0) & 0xf;
            lmc_dimmx_params.s.rc15        = (rc >> 4) & 0xf;


            if ((s = ddr_getenv_debug("ddr_clk_drive")) != NULL) {
                if (strcmp(s,"light") == 0) {
                    lmc_dimmx_params.s.rc5         = 0x0; /* Light Drive */
                }
                if (strcmp(s,"moderate") == 0) {
                    lmc_dimmx_params.s.rc5         = 0x5; /* Moderate Drive */
                }
                if (strcmp(s,"strong") == 0) {
                    lmc_dimmx_params.s.rc5         = 0xA; /* Strong Drive */
                }
                error_print("Parameter found in environment. ddr_clk_drive = %s\n", s);
            }

            if ((s = ddr_getenv_debug("ddr_cmd_drive")) != NULL) {
                if (strcmp(s,"light") == 0) {
                    lmc_dimmx_params.s.rc3         = 0x0; /* Light Drive */
                }
                if (strcmp(s,"moderate") == 0) {
                    lmc_dimmx_params.s.rc3         = 0x5; /* Moderate Drive */
                }
                if (strcmp(s,"strong") == 0) {
                    lmc_dimmx_params.s.rc3         = 0xA; /* Strong Drive */
                }
                error_print("Parameter found in environment. ddr_cmd_drive = %s\n", s);
            }

            if ((s = ddr_getenv_debug("ddr_ctl_drive")) != NULL) {
                if (strcmp(s,"light") == 0) {
                    lmc_dimmx_params.s.rc4         = 0x0; /* Light Drive */
                }
                if (strcmp(s,"moderate") == 0) {
                    lmc_dimmx_params.s.rc4         = 0x5; /* Moderate Drive */
                }
                error_print("Parameter found in environment. ddr_ctl_drive = %s\n", s);
            }


            /*
            ** rc10               RDIMM Operating Speed
            ** ====   =========================================================
            **  0                 tclk_psecs > 2500 psec DDR3/DDR3L-800 (default)
            **  1     2500 psec > tclk_psecs > 1875 psec DDR3/DDR3L-1066
            **  2     1875 psec > tclk_psecs > 1500 psec DDR3/DDR3L-1333
            **  3     1500 psec > tclk_psecs > 1250 psec DDR3/DDR3L-1600
            */
            lmc_dimmx_params.s.rc10        = 3;
            if (tclk_psecs >= 1250)
                lmc_dimmx_params.s.rc10    = 3;
            if (tclk_psecs >= 1500)
                lmc_dimmx_params.s.rc10    = 2;
            if (tclk_psecs >= 1875)
                lmc_dimmx_params.s.rc10    = 1;
            if (tclk_psecs >= 2500)
                lmc_dimmx_params.s.rc10    = 0;

            for (i=0; i<16; ++i) {
                uint64_t value;
                if ((s = lookup_env_parameter("ddr_rc%d", i)) != NULL) {
                    value = simple_strtoul(s, NULL, 0);
                    lmc_dimmx_params.u64 &= ~((uint64_t)0xf << (i*4));
                    lmc_dimmx_params.u64 |=           (  value << (i*4));
                }
            }

            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_DIMMX_PARAMS(dimm, ddr_interface_num),
                                      lmc_dimmx_params.u64);

            ddr_print("DIMM%d Register Control Words         RC15:RC0 : %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
                      dimm,
                      lmc_dimmx_params.s.rc15,
                      lmc_dimmx_params.s.rc14,
                      lmc_dimmx_params.s.rc13,
                      lmc_dimmx_params.s.rc12,
                      lmc_dimmx_params.s.rc11,
                      lmc_dimmx_params.s.rc10,
                      lmc_dimmx_params.s.rc9 ,
                      lmc_dimmx_params.s.rc8 ,
                      lmc_dimmx_params.s.rc7 ,
                      lmc_dimmx_params.s.rc6 ,
                      lmc_dimmx_params.s.rc5 ,
                      lmc_dimmx_params.s.rc4 ,
                      lmc_dimmx_params.s.rc3 ,
                      lmc_dimmx_params.s.rc2 ,
                      lmc_dimmx_params.s.rc1 ,
                      lmc_dimmx_params.s.rc0 );


            /* LMC0_DIMM_CTL */
            lmc_dimm_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DIMM_CTL(ddr_interface_num));
            lmc_dimm_ctl.s.dimm0_wmask         = 0xffff;
            lmc_dimm_ctl.s.dimm1_wmask         = (dimm_count > 1) ? 0xffff : 0x0000;
            lmc_dimm_ctl.s.tcws                = 0x4e0;
            lmc_dimm_ctl.s.parity              = custom_lmc_config->parity;

            if ((s = lookup_env_parameter("ddr_dimm0_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm0_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm1_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm1_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_parity")) != NULL) {
                lmc_dimm_ctl.s.parity = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_tcws")) != NULL) {
                lmc_dimm_ctl.s.tcws = simple_strtoul(s, NULL, 0);
            }

            ddr_print("DIMM%d DIMM_CTL                                : 0x%016llx\n",
                      dimm, (unsigned long long)lmc_dimm_ctl.u64);
            ddr_config_write_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num),
                                      lmc_dimm_ctl.u64);
        }
    } else {
        /* Disable register control writes for unbuffered */
        cvmx_lmcx_dimm_ctl_t lmc_dimm_ctl;
        lmc_dimm_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DIMM_CTL(ddr_interface_num));
        lmc_dimm_ctl.s.dimm0_wmask         = 0;
        lmc_dimm_ctl.s.dimm1_wmask         = 0;
        ddr_config_write_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num),
				  lmc_dimm_ctl.u64);
    }


    /*
     * 3. Without changing any other fields in LMC0_CONFIG, write
     *    LMC0_CONFIG[RANKMASK,INIT_STATUS,SEQUENCE] while writing
     *    LMC0_CONFIG[INIT_START] = 1, all with a single CSR
     *    write. Both LMC0_CONFIG[RANKMASK,INIT_STATUS] bits
     *    should be set to the same mask value indicating the
     *    ranks that will participate in the sequence.
     *
     *    The SEQUENCE value should select power-up/init or
     *    self-refresh exit, depending on whether the DRAM parts
     *    are in self-refresh and whether their contents should be
     *    preserved. While LMC performs these sequences, it will
     *    not perform any other DDR3 transactions.
     *
     *    If power-up/init is selected immediately following a
     *    DRESET assertion, LMC executes the sequence described in
     *    the Reset and Initialization Procedure section of the
     *    JEDEC DDR3 specification. This includes activating CKE,
     *    writing all four DDR3 mode registers on all selected
     *    ranks, and issuing the required ZQCL command. The
     *    RANKMASK value should select all ranks with attached
     *    DRAM in this case. If LMC0_CONTROL[RDIMM_ENA]=1, LMC
     *    writes the JEDEC standard SSTE32882 control words
     *    selected by LMC0_DIMM_CTL [DIMM*_WMASK] between DDR_CKE*
     *    signal assertion and the first DDR3 mode register write.
     */

    if (ddr_memory_preserved()) {
        /* Contents are being preserved. Take DRAM out of self-refresh
           first. Then init steps can procede normally */
        perform_ddr3_sequence(cpu_id, rank_mask, ddr_interface_num, 3); /* self-refresh exit */
    }

    {
        cvmx_lmcx_modereg_params0_t lmc_modereg_params0;
        lmc_modereg_params0.u64 = cvmx_read_csr(CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
        lmc_modereg_params0.s.dllr    = 1; /* Set during first init sequence */
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);

        perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        lmc_modereg_params0.s.dllr    = 0; /* Clear for normal operation */
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);
    }

    /*
     *    If self-refresh exit is selected, LMC executes the
     *    required SRX command followed by a refresh and ZQ
     *    calibration. Section 4.5 describes behavior of a REF +
     *    ZQCS.  LMC does not write the DDR3 mode registers as
     *    part of this sequence, and the mode register parameters
     *    must match at self-refresh entry and exit times. The
     *    self-refresh exit sequence does not use LMC0_CONFIG
     *    [RANKMASK]. If the self-refresh exit is preceded by a
     *    self-refresh exit SEQUENCE without an intervening DRESET
     *    sequence, then LMC0_CONFIG [INIT_STATUS] will already
     *    indicate the ranks that should participate, and software
     *    need not assert any LMC0_CONFIG[INIT_STATUS]
     *    bits. Otherwise, the LMC0_CONFIG CSR write that
     *    initiates the self-refresh exit must also set
     *    LMC0_CONFIG[INIT_STATUS] bits for all ranks with
     *    attached DRAM.
     *
     * 4. Read LMC0_CONFIG and wait for the result.
     */

    cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num)); /* Read CVMX_LMCX_CONFIG */

    /*
     * 4.8.6 LMC Write Leveling
     *
     * LMC supports an automatic write-leveling like that
     * described in the JEDEC DDR3 specifications separately per
     * byte-lane.
     *
     * All of DDR PLL, LMC CK, and LMC DRESET, and early LMC
     * initializations must be completed prior to starting this
     * LMC write-leveling sequence.
     *
     * There are many possible procedures that will write-level
     * all the attached DDR3 DRAM parts. One possibility is for
     * software to simply write the desired values into
     * LMC0_WLEVEL_RANK(0..3). This section describes one possible
     * sequence that uses LMCs auto-write-leveling capabilities.
     */

    {
        cvmx_lmcx_wlevel_ctl_t wlevel_ctl;
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        int rankx = 0;
        int wlevel_bitmask[9];
        int byte_idx;
        int passx;
        int ecc_ena;
        int ddr_wlevel_roundup = 0;
        int save_mode32b;

        if (wlevel_loops)
            ddr_print("Performing Write-Leveling\n");
        else
            wlevel_bitmask_errors = 1; /* Force software write-leveling to run */

        lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
        save_mode32b = lmc_config.s.mode32b;
        lmc_config.s.mode32b         = (! ddr_interface_64b);
        ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.s.mode32b);

        while(wlevel_loops--) {
            if ((s = lookup_env_parameter("ddr_wlevel_roundup")) != NULL) {
                ddr_wlevel_roundup = simple_strtoul(s, NULL, 0);
            }
            for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;

                wlevel_ctl.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_CTL(ddr_interface_num));
                lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
                ecc_ena = lmc_config.s.ecc_ena;

                if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
                    if ((s = lookup_env_parameter("ddr_wlevel_rtt_nom")) != NULL) {
                        wlevel_ctl.s.rtt_nom   = simple_strtoul(s, NULL, 0);
                    }
                }

                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                          0); /* Clear write-level delays */

                wlevel_bitmask_errors = 0; /* Reset error counter */

                for (byte_idx=0; byte_idx<9; ++byte_idx) {
                    wlevel_bitmask[byte_idx] = 0; /* Reset bitmasks */
                }

                /* Make separate passes for each byte to reduce power. */
                for (passx=0; passx<(8+ecc_ena); ++passx) {

                    if (!(ddr_interface_bytemask&(1<<passx)))
                        continue;

                    wlevel_ctl.s.lanemask = (1<<passx);

                    ddr_config_write_csr_node(node,
                                              CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
                                              wlevel_ctl.u64);

                    /* Read and write values back in order to update the
                       status field. This insurs that we read the updated
                       values after write-leveling has completed. */
                    ddr_config_write_csr_node(node,
                                              CVMX_LMCX_WLEVEL_RANKX(rankx,
                                                                     ddr_interface_num),
                                              cvmx_read_csr_node(node,
                                                                 CVMX_LMCX_WLEVEL_RANKX(rankx,
                                                                                        ddr_interface_num)));

                    perform_ddr3_sequence(cpu_id, 1 << rankx, ddr_interface_num, 6); /* write-leveling */

                    do {
                        lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));
                    } while (lmc_wlevel_rank.cn68xx.status != 3);

                    wlevel_bitmask[passx] = octeon_read_lmcx_ddr3_wlevel_dbg(node, ddr_interface_num, passx);
                    if (wlevel_bitmask[passx] == 0)
                        ++wlevel_bitmask_errors;
                } /* for (passx=0; passx<(8+ecc_ena); ++passx) */

                ddr_print("Rank(%d) Wlevel Debug Results                  : %05x %05x %05x %05x %05x %05x %05x %05x %05x\n",
                          rankx,
                          wlevel_bitmask[8],
                          wlevel_bitmask[7],
                          wlevel_bitmask[6],
                          wlevel_bitmask[5],
                          wlevel_bitmask[4],
                          wlevel_bitmask[3],
                          wlevel_bitmask[2],
                          wlevel_bitmask[1],
                          wlevel_bitmask[0]
                          );

                ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_wlevel_rank.cn68xx.status,
                          lmc_wlevel_rank.u64,
                          lmc_wlevel_rank.cn68xx.byte8,
                          lmc_wlevel_rank.cn68xx.byte7,
                          lmc_wlevel_rank.cn68xx.byte6,
                          lmc_wlevel_rank.cn68xx.byte5,
                          lmc_wlevel_rank.cn68xx.byte4,
                          lmc_wlevel_rank.cn68xx.byte3,
                          lmc_wlevel_rank.cn68xx.byte2,
                          lmc_wlevel_rank.cn68xx.byte1,
                          lmc_wlevel_rank.cn68xx.byte0
                          );

                if (ddr_wlevel_roundup) { /* Round up odd bitmask delays */
                    for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                        if (!(ddr_interface_bytemask&(1<<byte_idx)))
                            continue;
                        update_wlevel_rank_struct(&lmc_wlevel_rank,
                                                  byte_idx,
                                                  roundup_ddr3_wlevel_bitmask(wlevel_bitmask[byte_idx]));
                    }
                    ddr_config_write_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                    ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                              rankx,
                              lmc_wlevel_rank.cn68xx.status,
                              lmc_wlevel_rank.u64,
                              lmc_wlevel_rank.cn68xx.byte8,
                              lmc_wlevel_rank.cn68xx.byte7,
                              lmc_wlevel_rank.cn68xx.byte6,
                              lmc_wlevel_rank.cn68xx.byte5,
                              lmc_wlevel_rank.cn68xx.byte4,
                              lmc_wlevel_rank.cn68xx.byte3,
                              lmc_wlevel_rank.cn68xx.byte2,
                              lmc_wlevel_rank.cn68xx.byte1,
                              lmc_wlevel_rank.cn68xx.byte0
                              );
                }

                if (wlevel_bitmask_errors != 0) {
                    ddr_print("Rank(%d) Write-Leveling Failed: %d Bitmask errors\n", rankx, wlevel_bitmask_errors);
                }
            } /* for (rankx = 0; rankx < dimm_count * 4;rankx++) */
        } /* while(wlevel_loops--) */

        lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
        lmc_config.s.mode32b         = save_mode32b;
        ddr_config_write_csr(CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.s.mode32b);
    }

    /*
     * 1. If the DQS/DQ delays on the board may be more than the
     *    ADD/CMD delays, then ensure that LMC0_CONFIG[EARLY_DQX]
     *    is set at this point.
     *
     * Do the remaining steps 2-7 separately for each rank i with
     * attached DRAM.
     *
     * 2. Write LMC0_WLEVEL_RANKi = 0.
     *
     * 3. For 8 parts:
     *
     *    Without changing any other fields in LMC0_WLEVEL_CTL,
     *    write LMC0_WLEVEL_CTL[LANEMASK] to select all byte lanes
     *    with attached DRAM.
     *
     *    For 16 parts:
     *
     *    Without changing any other fields in LMC0_WLEVEL_CTL,
     *    write LMC0_WLEVEL_CTL[LANEMASK] to select all even byte
     *    lanes with attached DRAM.
     *
     * 4. Without changing any other fields in LMC0_CONFIG,
     *
     *    o write LMC0_CONFIG[SEQUENCE] to select write-leveling, and
     *
     *    o write LMC0_CONFIG[RANKMASK] = (1 << i), and
     *
     *    o write LMC0_CONFIG[INIT_START] = 1
     *
     *    LMC will initiate write-leveling at this point. Assuming
     *    LMC0_WLEVEL_CTL [SSET] = 0, LMC first enables
     *    write-leveling on the selected DRAM rank via a DDR3 MR1
     *    write, then sequences through and accumulates
     *    write-leveling results for 8 different delay settings
     *    twice, starting at a delay of zero in this case since
     *    LMC0_WLEVEL_RANKi[BYTE*<4:3>] = 0, increasing by 1/8 CK
     *    each setting, covering a total distance of one CK, then
     *    disables the write-leveling via another DDR3 MR1 write.
     *
     *    After the sequence through 16 delay settings is complete:
     *
     *    o LMC sets LMC0_WLEVEL_RANKi[STATUS]=3
     *
     *    o LMC sets LMC0_WLEVEL_RANKi[BYTE*<2:0>] (for all ranks
     *      selected by LANEMASK) to indicate the first write
     *      leveling result of 1 that followed result of 0 during
     *      the sequence, except that LMC always writes
     *      LMC0_WLEVEL_RANKi[BYTE*<0>]=0.
     *
     *    o Software can read the 8 write-leveling results from
     *      the first pass through the delay settings by reading
     *      LMC0_WLEVEL_DBG[BITMASK] (after writing
     *      LMC0_WLEVEL_DBG[BYTE]). (LMC does not retain the
     *      write-leveling results from the second pass through
     *      the 8 delay settings. They should often be identical
     *      to the LMC0_WLEVEL_DBG[BITMASK] results, though.)
     *
     * 5. Wait until LMC0_WLEVEL_RANKi[STATUS] != 2.
     *
     *    LMC will have updated LMC0_WLEVEL_RANKi[BYTE*<2:0>] for
     *    all byte lanes selected by LMC0_WLEVEL_CTL[LANEMASK] at
     *    this point.  LMC0_WLEVEL_RANKi[BYTE*<4:3>] will still be
     *    the value that software wrote in step 2 above, which is
     *    zero.
     *
     * 6. For 16 parts:
     *
     *    Without changing any other fields in LMC0_WLEVEL_CTL,
     *    write LMC0_WLEVEL_CTL[LANEMASK] to select all odd byte
     *    lanes with attached DRAM.
     *
     *    Repeat steps 4 and 5 with this new
     *    LMC0_WLEVEL_CTL[LANEMASK] setting. Skip to step 7 if
     *    this has already been done.
     *
     *    For 8 parts:
     *
     *    Skip this step. Go to step 7.
     *
     * 7. Calculate LMC0_WLEVEL_RANKi[BYTE*<4:3>] settings for all
     *    byte lanes on all ranks with attached DRAM.
     *
     *    At this point, all byte lanes on rank i with attached
     *    DRAM should have been write-leveled, and
     *    LMC0_WLEVEL_RANKi[BYTE*<2:0>] has the result for each
     *    byte lane.
     *
     *    But note that the DDR3 write-leveling sequence will only
     *    determine the delay modulo the CK cycle time, and cannot
     *    determine how many additional CKs of delay are
     *    present. Software must calculate the number of CKs, or
     *    equivalently, the LMC0_WLEVEL_RANKi[BYTE*<4:3>]
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
     * 8. Initialize LMC0_WLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    LMC0_WLEVEL_RANKj = LMC0_WLEVEL_RANKi.
     *
     * 4.8.7 LMC Read Leveling
     *
     * LMC supports an automatic read-leveling separately per
     * byte-lane using the DDR3 multi-purpose register pre-defined
     * pattern for system calibration defined in the JEDEC DDR3
     * specifications.
     *
     * All of DDR PLL, LMC CK, and LMC DRESET, and early LMC
     * initializations must be completed prior to starting this
     * LMC read-leveling sequence.
     *
     * Software could simply write the desired read-leveling
     * values into LMC0_RLEVEL_RANK(0..3). This section describes
     * a sequence that uses LMCs auto-read-leveling capabilities.
     *
     * When LMC does the read-leveling sequence for a rank, it
     * first enables the DDR3 multi-purpose register predefined
     * pattern for system calibration on the selected DRAM rank
     * via a DDR3 MR3 write, then executes 64 RD operations at
     * different internal delay settings, then disables the
     * predefined pattern via another DDR3 MR3 write. LMC
     * determines the pass or fail of each of the 64 settings
     * independently for each byte lane, then writes appropriate
     * LMC0_RLEVEL_RANK(0..3)[BYTE*] values for the rank.
     *
     * After read-leveling for a rank, software can read the 64
     * pass/fail indications for one byte lane via
     * LMC0_RLEVEL_DBG[BITMASK]. Software can observe all
     * pass/fail results for all byte lanes in a rank via separate
     * read-leveling sequences on the rank with different
     * LMC0_RLEVEL_CTL[BYTE] values.
     *
     * The 64 pass/fail results will typically have failures for
     * the low delays, followed by a run of some passing settings,
     * followed by more failures in the remaining high delays.
     * LMC sets LMC0_RLEVEL_RANK(0..3)[BYTE*] to one of the
     * passing settings. First, LMC selects the longest run of
     * successes in the 64 results. (In the unlikely event that
     * there is more than one longest run, LMC selects the first
     * one.) Then if LMC0_RLEVEL_CTL[OFFSET_EN] = 1 and the
     * selected run has more than LMC0_RLEVEL_CTL[OFFSET]
     * successes, LMC selects the last passing setting in the run
     * minus LMC0_RLEVEL_CTL[OFFSET]. Otherwise LMC selects the
     * middle setting in the run (rounding earlier when
     * necessary). We expect the read-leveling sequence to produce
     * good results with the reset values LMC0_RLEVEL_CTL
     * [OFFSET_EN]=1, LMC0_RLEVEL_CTL[OFFSET] = 2.
     *
     * The read-leveling sequence:
     *
     * 1. Select desired LMC0_RLEVEL_CTL[OFFSET_EN,OFFSET,BYTE]
     *    settings.  Do the remaining steps 2V4 separately for
     *    each rank i with attached DRAM.
     */

    {
#pragma pack(push,4)
        cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        cvmx_lmcx_comp_ctl2_t lmc_comp_ctl2;
        cvmx_lmcx_rlevel_ctl_t rlevel_ctl;
        cvmx_lmcx_control_t lmc_control;
        cvmx_lmcx_modereg_params1_t lmc_modereg_params1;
        unsigned char rodt_ctl;
        unsigned char rankx = 0;
        int rlevel_rodt_errors = 0;
        unsigned char ecc_ena;
        unsigned char rtt_nom;
        unsigned char rtt_idx;
        int min_rtt_nom_idx;
        int max_rtt_nom_idx;
        int min_rodt_ctl;
        int max_rodt_ctl;
        static const unsigned char rodt_ohms[] = { 0, 20, 30, 40, 60, 120 };
        static const unsigned char rtt_nom_ohms[] = { 0, 60, 120, 40, 20, 30 };
        static const unsigned char rtt_nom_table[] = { 0, 2, 1, 3, 5, 4 };
        static const unsigned char rtt_wr_ohms[]  = { 0, 60, 120 };
        static const unsigned char dic_ohms[]     = { 40, 34 };
        int rlevel_debug_loops = 1;
        int default_rodt_ctl;
        unsigned char save_ddr2t;
        int rlevel_avg_loops;
        int ddr_rlevel_compute;
        int saved_ddr__ptune, saved_ddr__ntune, rlevel_comp_offset;
        int saved_int_zqcs_dis = 0;
        int disable_sequential_delay_check = 0;
        int maximum_adjacent_rlevel_delay_increment = 0;
        struct {
            uint64_t setting;
            int      score;
        } rlevel_scoreboard[sizeof(rtt_nom_ohms)][sizeof(rodt_ohms)][4];
#pragma pack(pop)

        default_rodt_ctl = odt_config[odt_idx].qs_dic;

        lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
        save_ddr2t                    = lmc_control.s.ddr2t;

        lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
        ecc_ena = lmc_config.s.ecc_ena;

        {
            int save_ref_zqcs_int;
            uint64_t temp_delay_usecs;

            /* Temporarily select the minimum ZQCS interval and wait
               long enough for a few ZQCS calibrations to occur.  This
               should ensure that the calibration circuitry is
               stabilized before read-leveling occurs. */
            if (OCTEON_IS_OCTEON3()) {
                save_ref_zqcs_int         = lmc_config.cn78xx.ref_zqcs_int;
                lmc_config.cn78xx.ref_zqcs_int = 1 | (32<<7); /* set smallest interval */
            } else {
                save_ref_zqcs_int         = lmc_config.cn63xx.ref_zqcs_int;
                lmc_config.cn63xx.ref_zqcs_int = 1 | (32<<7); /* set smallest interval */
            }
            ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));

            /* Compute an appropriate delay based on the current ZQCS
               interval. The delay should be long enough for the
               current ZQCS delay counter to expire plus ten of the
               minimum intarvals to ensure that some calibrations
               occur. */
            temp_delay_usecs = (((uint64_t)save_ref_zqcs_int >> 7)
                                * tclk_psecs * 100 * 512 * 128) / (10000*10000)
                + 10 * ((uint64_t)32 * tclk_psecs * 100 * 512 * 128) / (10000*10000);

            ddr_print ("Waiting %d usecs for ZQCS calibrations to start\n",
                       temp_delay_usecs);
            cvmx_wait_usec(temp_delay_usecs);

            if (OCTEON_IS_OCTEON3())
                lmc_config.cn78xx.ref_zqcs_int = save_ref_zqcs_int; /* Restore computed interval */
            else
                lmc_config.cn63xx.ref_zqcs_int = save_ref_zqcs_int; /* Restore computed interval */
            ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        }

        if ((save_ddr2t == 1) && octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            /* During the read-leveling sequence with registered DIMMs
               when 2T mode is enabled (i.e. LMC0_CONTROL[DDR2T]=1), some
               register parts do not like the sequence that LMC generates
               for an MRS register write. (14656) */
            ddr_print("Disabling DDR 2T during read-leveling seq. Re: Pass 1 LMC-14656\n");
            lmc_control.s.ddr2t           = 0;
        }

        if ((s = lookup_env_parameter("ddr_rlevel_2t")) != NULL) {
            lmc_control.s.ddr2t = simple_strtoul(s, NULL, 0);
        }

        ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

        ddr_print("Performing Read-Leveling\n");

        rlevel_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));

        rlevel_avg_loops = custom_lmc_config->rlevel_average_loops;
        if (rlevel_avg_loops == 0) rlevel_avg_loops = 1;

        ddr_rlevel_compute = custom_lmc_config->rlevel_compute;
        rlevel_ctl.s.offset_en = custom_lmc_config->offset_en;
        rlevel_ctl.s.offset    = spd_rdimm
            ? custom_lmc_config->offset_rdimm
            : custom_lmc_config->offset_udimm;

        if (! octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            rlevel_ctl.s.delay_unload_0 = 1; /* should normally be set */
            rlevel_ctl.s.delay_unload_1 = 1; /* should normally be set */
            rlevel_ctl.s.delay_unload_2 = 1; /* should normally be set */
            rlevel_ctl.s.delay_unload_3 = 1; /* should normally be set */
        }

        {
            int byte_bitmask = 0xff;

            /* If we will be switching to 32bit mode level based on only
               four bits because there are only 4 ECC bits. */
            if (! ddr_interface_64b)
                byte_bitmask = 0x0f;

            rlevel_ctl.s.or_dis   = (! ddr_interface_64b);
            rlevel_ctl.s.bitmask  = byte_bitmask;
        }

        ddr_rodt_ctl_auto = custom_lmc_config->ddr_rodt_ctl_auto;
        ddr_rlevel_compute = custom_lmc_config->rlevel_compute;
        rlevel_ctl.cn78xx.offset_en = custom_lmc_config->offset_en;
        rlevel_ctl.cn78xx.offset    = spd_rdimm
            ? custom_lmc_config->offset_rdimm
            : custom_lmc_config->offset_udimm;

        rlevel_comp_offset = spd_rdimm
            ? custom_lmc_config->rlevel_comp_offset_rdimm
            : custom_lmc_config->rlevel_comp_offset_udimm;

        /* Default compensation offset is 2. */
        rlevel_comp_offset = (rlevel_comp_offset == 0) ? 2 : rlevel_comp_offset;

        if ((s = lookup_env_parameter("ddr_rlevel_offset")) != NULL) {
            rlevel_ctl.s.offset   = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_offset_en")) != NULL) {
            rlevel_ctl.s.offset_en   = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_rlevel_ctl")) != NULL) {
            rlevel_ctl.u64   = simple_strtoul(s, NULL, 0);
        }

        ddr_config_write_csr(CVMX_LMCX_RLEVEL_CTL(ddr_interface_num), rlevel_ctl.u64);

        if ((s = lookup_env_parameter("ddr%d_rlevel_debug_loops", ddr_interface_num)) != NULL) {
            rlevel_debug_loops = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rtt_nom_auto")) != NULL) {
            ddr_rtt_nom_auto = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rodt_ctl")) == NULL)
            s = lookup_env_parameter("ddr%d_rodt_ctl", ddr_interface_num);
        if (s != NULL) {
            default_rodt_ctl    = simple_strtoul(s, NULL, 0);
            ddr_rodt_ctl_auto = 0;
        }

        if ((s = lookup_env_parameter("ddr_rodt_ctl_auto")) != NULL) {
            ddr_rodt_ctl_auto = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_average")) != NULL) {
            rlevel_avg_loops = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_compute")) != NULL) {
            ddr_rlevel_compute = simple_strtoul(s, NULL, 0);
        }

        ddr_print("RLEVEL_CTL                                    : 0x%016llx\n",
                  (unsigned long long)rlevel_ctl.u64);
        ddr_print("RLEVEL_OFFSET                                 : %6d\n", rlevel_ctl.s.offset);
        ddr_print("RLEVEL_OFFSET_EN                              : %6d\n", rlevel_ctl.s.offset_en);

        /* The purpose for the indexed table is to sort the settings
        ** by the ohm value to simplify the testing when incrementing
        ** through the settings.  (index => ohms) 1=120, 2=60, 3=40,
        ** 4=30, 5=20 */
        min_rtt_nom_idx = (custom_lmc_config->min_rtt_nom_idx == 0) ? 1 : custom_lmc_config->min_rtt_nom_idx;
        max_rtt_nom_idx = (custom_lmc_config->max_rtt_nom_idx == 0) ? 5 : custom_lmc_config->max_rtt_nom_idx;

        min_rodt_ctl = (custom_lmc_config->min_rodt_ctl == 0) ? 1 : custom_lmc_config->min_rodt_ctl;
        max_rodt_ctl = (custom_lmc_config->max_rodt_ctl == 0) ? 5 : custom_lmc_config->max_rodt_ctl;

        if ((s = lookup_env_parameter("ddr_min_rodt_ctl")) != NULL) {
            min_rodt_ctl = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_max_rodt_ctl")) != NULL) {
            max_rodt_ctl = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_min_rtt_nom_idx")) != NULL) {
            min_rtt_nom_idx = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_max_rtt_nom_idx")) != NULL) {
            max_rtt_nom_idx = simple_strtoul(s, NULL, 0);
        }

        while(rlevel_debug_loops--) {
            /* Initialize the error scoreboard */
            for ( rtt_idx=1; rtt_idx<6; ++rtt_idx) {
                rtt_nom = rtt_nom_table[rtt_idx];
                for (rodt_ctl=5; rodt_ctl>0; --rodt_ctl) {
                    for (rankx = 0; rankx < dimm_count*4; rankx++) {
                        if (!(rank_mask & (1 << rankx)))
                            continue;
                        rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score   = 0;
                        rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting = 0;
                    }
                }
            }

            if ((s = lookup_env_parameter("ddr_rlevel_comp_offset")) != NULL) {
                rlevel_comp_offset = simple_strtoul(s, NULL, 0);
            }

            disable_sequential_delay_check = custom_lmc_config->disable_sequential_delay_check;

            if ((s = lookup_env_parameter("ddr_disable_sequential_delay_check")) != NULL) {
                disable_sequential_delay_check = simple_strtoul(s, NULL, 0);
            }

            maximum_adjacent_rlevel_delay_increment = custom_lmc_config->maximum_adjacent_rlevel_delay_increment;

            if ((s = lookup_env_parameter("ddr_maximum_adjacent_rlevel_delay_increment")) != NULL) {
                maximum_adjacent_rlevel_delay_increment = simple_strtoul(s, NULL, 0);
            }

            lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            saved_ddr__ptune = lmc_comp_ctl2.cn61xx.ddr__ptune;
            saved_ddr__ntune = lmc_comp_ctl2.cn61xx.ddr__ntune;

            /* Disable dynamic compensation settings */
            if (rlevel_comp_offset != 0) {
                lmc_comp_ctl2.cn61xx.ptune = saved_ddr__ptune;
                lmc_comp_ctl2.cn61xx.ntune = saved_ddr__ntune;

                lmc_comp_ctl2.cn61xx.ptune += rlevel_comp_offset;

                lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
                saved_int_zqcs_dis = lmc_control.s.int_zqcs_dis;
                lmc_control.s.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
                ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

                lmc_comp_ctl2.cn61xx.byp = 1; /* Enable bypass mode */
                ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            }

            lmc_modereg_params1.u64 = cvmx_read_csr(CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

            for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) {
                rtt_nom = rtt_nom_table[rtt_idx];

                /* When the read ODT mask is zero the dyn_rtt_nom_mask is
                   zero than RTT_NOM will not be changing during
                   read-leveling.  Since the value is fixed we only need
                   to test it once. */
                if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
                    continue;

                if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.s.rtt_nom_00 = rtt_nom;
                if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.s.rtt_nom_01 = rtt_nom;
                if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.s.rtt_nom_10 = rtt_nom;
                if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.s.rtt_nom_11 = rtt_nom;

                ddr_config_write_csr(CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num), lmc_modereg_params1.u64);
                ddr_print("\n");
                ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
                          lmc_modereg_params1.s.rtt_nom_11,
                          lmc_modereg_params1.s.rtt_nom_10,
                          lmc_modereg_params1.s.rtt_nom_01,
                          lmc_modereg_params1.s.rtt_nom_00);

                perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

                for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) {
                    rlevel_rodt_errors = 0;
                    lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                    lmc_comp_ctl2.cn61xx.rodt_ctl = rodt_ctl;
                    ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                    lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                    ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                              lmc_comp_ctl2.cn61xx.rodt_ctl, rodt_ohms[lmc_comp_ctl2.cn61xx.rodt_ctl]);

                    for (rankx = 0; rankx < dimm_count*4; rankx++) {
                        int byte_idx;
                        rlevel_byte_data_t rlevel_byte[9];
                        int average_loops;
                        int rlevel_rank_errors;

                        if (!(rank_mask & (1 << rankx)))
                            continue;

                        memset(rlevel_byte, 0, sizeof(rlevel_byte));

                        average_loops = rlevel_avg_loops;
                        while (average_loops--) {
                            rlevel_rank_errors = 0;

                            /* Clear read-level delays */
                            ddr_config_write_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), 0);

                            perform_ddr3_sequence(cpu_id, 1 << rankx, ddr_interface_num, 1); /* read-leveling */

                            do {
                                lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                            } while (lmc_rlevel_rank.cn68xx.status != 3);

                            {
                                rlevel_bitmask_t rlevel_bitmask[9];
                                memset(rlevel_bitmask, 0, sizeof(rlevel_bitmask));

                                /* Evaluate the quality of the read-leveling
                                   delays. Also save off a software computed
                                   read-leveling mask that may be used later
                                   to qualify the delay results from Octeon. */
                                for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                                    if (!(ddr_interface_bytemask&(1<<byte_idx)))
                                        continue;
                                    rlevel_bitmask[byte_idx].bm =
                                        octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, byte_idx);
                                    rlevel_rank_errors +=
                                        validate_ddr3_rlevel_bitmask(&rlevel_bitmask[byte_idx], DDR3_DRAM);
                                }

                                /* Set delays for unused bytes to match byte 0. */
                                for (byte_idx=0; byte_idx<9; ++byte_idx) {
                                    if (ddr_interface_bytemask&(1<<byte_idx))
                                        continue;
                                    update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx, lmc_rlevel_rank.cn68xx.byte0);
                                }

                                /* Save a copy of the byte delays in physical
                                   order for sequential evaluation. */
                                if (ecc_ena) {
                                    rlevel_byte[8].delay = lmc_rlevel_rank.cn68xx.byte7;
                                    rlevel_byte[7].delay = lmc_rlevel_rank.cn68xx.byte6;
                                    rlevel_byte[6].delay = lmc_rlevel_rank.cn68xx.byte5;
                                    rlevel_byte[5].delay = lmc_rlevel_rank.cn68xx.byte4;
                                    rlevel_byte[4].delay = lmc_rlevel_rank.cn68xx.byte8; /* ECC */
                                } else {
                                    rlevel_byte[7].delay = lmc_rlevel_rank.cn68xx.byte7;
                                    rlevel_byte[6].delay = lmc_rlevel_rank.cn68xx.byte6;
                                    rlevel_byte[5].delay = lmc_rlevel_rank.cn68xx.byte5;
                                    rlevel_byte[4].delay = lmc_rlevel_rank.cn68xx.byte4;
                                }
                                rlevel_byte[3].delay = lmc_rlevel_rank.cn68xx.byte3;
                                rlevel_byte[2].delay = lmc_rlevel_rank.cn68xx.byte2;
                                rlevel_byte[1].delay = lmc_rlevel_rank.cn68xx.byte1;
                                rlevel_byte[0].delay = lmc_rlevel_rank.cn68xx.byte0;

                                if (! disable_sequential_delay_check) {
                                    /* Evaluate delay sequence across the whole range of bytes for stantard dimms. */
                                    if ((spd_dimm_type == 1) || (spd_dimm_type == 5)) { /* 1=RDIMM, 5=Mini-RDIMM */
                                        /* Registerd dimm topology routes from the center. */
                                        rlevel_rank_errors += nonsequential_delays(rlevel_byte, 0, 3+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                        rlevel_rank_errors += nonsequential_delays(rlevel_byte, 4, 7+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                    }
                                    if ((spd_dimm_type == 2) || (spd_dimm_type == 6)) { /* 1=UDIMM, 5=Mini-UDIMM */
                                        /* Unbuffered dimm topology routes from end to end. */
                                        rlevel_rank_errors += nonsequential_delays(rlevel_byte, 0, 7+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                    }
                                }

                                ddr_print("Rank(%d) Rlevel Debug Test Results  8:0        : %05llx %05llx %05llx %05llx %05llx %05llx %05llx %05llx %05llx\n",
                                          rankx,
                                          rlevel_bitmask[8].bm,
                                          rlevel_bitmask[7].bm,
                                          rlevel_bitmask[6].bm,
                                          rlevel_bitmask[5].bm,
                                          rlevel_bitmask[4].bm,
                                          rlevel_bitmask[3].bm,
                                          rlevel_bitmask[2].bm,
                                          rlevel_bitmask[1].bm,
                                          rlevel_bitmask[0].bm
                                          );

                                ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d)\n",
                                          rankx,
                                          lmc_rlevel_rank.cn68xx.status,
                                          lmc_rlevel_rank.u64,
                                          lmc_rlevel_rank.cn68xx.byte8,
                                          lmc_rlevel_rank.cn68xx.byte7,
                                          lmc_rlevel_rank.cn68xx.byte6,
                                          lmc_rlevel_rank.cn68xx.byte5,
                                          lmc_rlevel_rank.cn68xx.byte4,
                                          lmc_rlevel_rank.cn68xx.byte3,
                                          lmc_rlevel_rank.cn68xx.byte2,
                                          lmc_rlevel_rank.cn68xx.byte1,
                                          lmc_rlevel_rank.cn68xx.byte0,
                                          rlevel_rank_errors
                                          );

                                if (ddr_rlevel_compute) {
                                    /* Recompute the delays based on the bitmask */
                                    for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                                        if (!(ddr_interface_bytemask&(1<<byte_idx)))
                                            continue;
                                        update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx,
                                                                  compute_ddr3_rlevel_delay(rlevel_bitmask[byte_idx].mstart,
                                                                                            rlevel_bitmask[byte_idx].width,
                                                                                            rlevel_ctl));
                                    }

                                    /* Override the copy of byte delays with the computed results. */
                                    if (ecc_ena) {
                                        rlevel_byte[8].delay = lmc_rlevel_rank.cn68xx.byte7;
                                        rlevel_byte[7].delay = lmc_rlevel_rank.cn68xx.byte6;
                                        rlevel_byte[6].delay = lmc_rlevel_rank.cn68xx.byte5;
                                        rlevel_byte[5].delay = lmc_rlevel_rank.cn68xx.byte4;
                                        rlevel_byte[4].delay = lmc_rlevel_rank.cn68xx.byte8; /* ECC */
                                    } else {
                                        rlevel_byte[7].delay = lmc_rlevel_rank.cn68xx.byte7;
                                        rlevel_byte[6].delay = lmc_rlevel_rank.cn68xx.byte6;
                                        rlevel_byte[5].delay = lmc_rlevel_rank.cn68xx.byte5;
                                        rlevel_byte[4].delay = lmc_rlevel_rank.cn68xx.byte4;
                                    }
                                    rlevel_byte[3].delay = lmc_rlevel_rank.cn68xx.byte3;
                                    rlevel_byte[2].delay = lmc_rlevel_rank.cn68xx.byte2;
                                    rlevel_byte[1].delay = lmc_rlevel_rank.cn68xx.byte1;
                                    rlevel_byte[0].delay = lmc_rlevel_rank.cn68xx.byte0;

                                    ddr_print("Rank(%d) Computed delays                       : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                                              rankx,
                                              lmc_rlevel_rank.cn68xx.byte8,
                                              lmc_rlevel_rank.cn68xx.byte7,
                                              lmc_rlevel_rank.cn68xx.byte6,
                                              lmc_rlevel_rank.cn68xx.byte5,
                                              lmc_rlevel_rank.cn68xx.byte4,
                                              lmc_rlevel_rank.cn68xx.byte3,
                                              lmc_rlevel_rank.cn68xx.byte2,
                                              lmc_rlevel_rank.cn68xx.byte1,
                                              lmc_rlevel_rank.cn68xx.byte0
                                              );
                                }
                            }

                            /* Accumulate the total score across averaging loops for this setting */
                            rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score += rlevel_rank_errors;
                            debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d\n",
                                        rtt_nom, rodt_ctl, rankx,
                                        rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);

                            /* Accumulate the delay totals and loop counts
                               necessary to compute average delay results */
                            for (byte_idx=0; byte_idx<9; ++byte_idx) {
                                rlevel_byte[byte_idx].loop_total += rlevel_byte[byte_idx].delay;
                                if (rlevel_byte[byte_idx].delay != 0) /* Don't include delay=0 in the average */
                                    ++rlevel_byte[byte_idx].loop_count;
                            }

                            rlevel_rodt_errors += rlevel_rank_errors;
                        } /* while (rlevel_avg_loops--) */

                        /* Compute the average score across averaging loops */
                        rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score =
                            divide_nint(rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score, rlevel_avg_loops);

                        /* Compute the average delay results */
                        for (byte_idx=0; byte_idx<9; ++byte_idx) {
                            if (rlevel_byte[byte_idx].loop_count == 0)
                                rlevel_byte[byte_idx].loop_count = 1;
                            rlevel_byte[byte_idx].delay = divide_nint(rlevel_byte[byte_idx].loop_total,
                                                                      rlevel_byte[byte_idx].loop_count);
                        }

                        lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

                        if (ecc_ena) {
                            lmc_rlevel_rank.cn68xx.byte7 = rlevel_byte[8].delay;
                            lmc_rlevel_rank.cn68xx.byte6 = rlevel_byte[7].delay;
                            lmc_rlevel_rank.cn68xx.byte5 = rlevel_byte[6].delay;
                            lmc_rlevel_rank.cn68xx.byte4 = rlevel_byte[5].delay;
                            lmc_rlevel_rank.cn68xx.byte8 = rlevel_byte[4].delay; /* ECC */
                        } else {
                            lmc_rlevel_rank.cn68xx.byte7 = rlevel_byte[7].delay;
                            lmc_rlevel_rank.cn68xx.byte6 = rlevel_byte[6].delay;
                            lmc_rlevel_rank.cn68xx.byte5 = rlevel_byte[5].delay;
                            lmc_rlevel_rank.cn68xx.byte4 = rlevel_byte[4].delay;
                        }
                        lmc_rlevel_rank.cn68xx.byte3 = rlevel_byte[3].delay;
                        lmc_rlevel_rank.cn68xx.byte2 = rlevel_byte[2].delay;
                        lmc_rlevel_rank.cn68xx.byte1 = rlevel_byte[1].delay;
                        lmc_rlevel_rank.cn68xx.byte0 = rlevel_byte[0].delay;

                        if (rlevel_avg_loops > 1) {
                            ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d) Average\n\n",
                                      rankx,
                                      lmc_rlevel_rank.cn68xx.status,
                                      lmc_rlevel_rank.u64,
                                      lmc_rlevel_rank.cn68xx.byte8,
                                      lmc_rlevel_rank.cn68xx.byte7,
                                      lmc_rlevel_rank.cn68xx.byte6,
                                      lmc_rlevel_rank.cn68xx.byte5,
                                      lmc_rlevel_rank.cn68xx.byte4,
                                      lmc_rlevel_rank.cn68xx.byte3,
                                      lmc_rlevel_rank.cn68xx.byte2,
                                      lmc_rlevel_rank.cn68xx.byte1,
                                      lmc_rlevel_rank.cn68xx.byte0,
                                      rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score
                                      );
                        }

                        rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting = lmc_rlevel_rank.u64;
                    } /* for (rankx = 0; rankx < dimm_count*4; rankx++) */
                } /* for (rodt_ctl=odt_config[odt_idx].qs_dic; rodt_ctl>0; --rodt_ctl) */
            } /*  for ( rtt_idx=1; rtt_idx<6; ++rtt_idx) */


            /* Re-enable dynamic compensation settings. */
            if (rlevel_comp_offset != 0) {
                lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));

                lmc_comp_ctl2.cn61xx.ptune = 0;
                lmc_comp_ctl2.cn61xx.ntune = 0;
                lmc_comp_ctl2.cn61xx.byp = 0; /* Disable bypass mode */
                ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read once */

                lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
                ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                          lmc_comp_ctl2.cn61xx.ddr__ptune, lmc_comp_ctl2.cn61xx.ddr__ntune);

                lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
                lmc_control.s.int_zqcs_dis    = saved_int_zqcs_dis; /* Restore original setting */
                ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

            }


            {
                int override_compensation = 0;
                if ((s = lookup_env_parameter("ddr__ptune")) != NULL) {
                    saved_ddr__ptune = simple_strtoul(s, NULL, 0);
                    override_compensation = 1;
                }
                if ((s = lookup_env_parameter("ddr__ntune")) != NULL) {
                    saved_ddr__ntune = simple_strtoul(s, NULL, 0);
                    override_compensation = 1;
                }
                if (override_compensation) {
                    lmc_comp_ctl2.cn61xx.ptune = saved_ddr__ptune;
                    lmc_comp_ctl2.cn61xx.ntune = saved_ddr__ntune;

                    lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
                    saved_int_zqcs_dis = lmc_control.s.int_zqcs_dis;
                    lmc_control.s.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
                    ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

                    lmc_comp_ctl2.cn61xx.byp = 1; /* Enable bypass mode */
                    ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                    cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));

                    ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                              lmc_comp_ctl2.cn61xx.ptune, lmc_comp_ctl2.cn61xx.ntune);
                }
            }

            if (octeon_is_cpuid(OCTEON_CN61XX_PASS1_0)) {
                /* cn63xx p1.0 requires bypass mode */
                int ptune_offset = +0;
                int ntune_offset = +2;

                if ((s = lookup_env_parameter("ddr_ptune_offset")) != NULL) {
                    ptune_offset = simple_strtoul(s, NULL, 0);
                }
                if ((s = lookup_env_parameter("ddr_ntune_offset")) != NULL) {
                    ntune_offset = simple_strtoul(s, NULL, 0);
                }

                lmc_comp_ctl2.cn61xx.ptune = saved_ddr__ptune;
                lmc_comp_ctl2.cn61xx.ntune = saved_ddr__ntune;

                lmc_comp_ctl2.cn61xx.ptune += ptune_offset;
                lmc_comp_ctl2.cn61xx.ntune += ntune_offset;

                /* Disable ZQCS while in bypass. */
                lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
                saved_int_zqcs_dis = lmc_control.s.int_zqcs_dis;
                lmc_control.s.int_zqcs_dis    = 1;
                ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

                lmc_comp_ctl2.cn61xx.byp = 1; /* Enable bypass mode */
                ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                ddr_print("Bypass mode: DDR__PTUNE/DDR__NTUNE            : %d/%d\n",
                          lmc_comp_ctl2.cn61xx.ptune, lmc_comp_ctl2.cn61xx.ntune);
            }

            {
                int      best_rodt_score = 9999999; /* Start with an arbitrarily high score */
                int      auto_rodt_ctl = 0;
                int      auto_rtt_nom  = 0;
                int rodt_score;

                ddr_print("Evaluating Read-Leveling Scoreboard.\n");
                for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) {
                    rtt_nom = rtt_nom_table[rtt_idx];

                    /* When the read ODT mask is zero the dyn_rtt_nom_mask is
                       zero than RTT_NOM will not be changing during
                       read-leveling.  Since the value is fixed we only need
                       to test it once. */
                    if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
                        continue;

                    for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) {
                        rodt_score = 0;
                        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                            if (!(rank_mask & (1 << rankx)))
                                continue;
                            debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d\n",
                                        rtt_nom, rodt_ctl, rankx, rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);
                            rodt_score += rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score;
                        }
                        debug_print("rodt_score:%d, best_rodt_score:%d\n", rodt_score, best_rodt_score);
                        if (rodt_score < best_rodt_score) {
                            best_rodt_score = rodt_score;
                            auto_rodt_ctl   = rodt_ctl;
                            auto_rtt_nom    = rtt_nom;
                        }
                    }
                }

                lmc_modereg_params1.u64 = cvmx_read_csr(CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

                if (ddr_rtt_nom_auto) {
                    /* Store the automatically set RTT_NOM value */
                    if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.s.rtt_nom_00 = auto_rtt_nom;
                    if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.s.rtt_nom_01 = auto_rtt_nom;
                    if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.s.rtt_nom_10 = auto_rtt_nom;
                    if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.s.rtt_nom_11 = auto_rtt_nom;
                } else {
                    /* restore the manual settings to the register */
                    lmc_modereg_params1.s.rtt_nom_00 = default_rtt_nom[0];
                    lmc_modereg_params1.s.rtt_nom_01 = default_rtt_nom[1];
                    lmc_modereg_params1.s.rtt_nom_10 = default_rtt_nom[2];
                    lmc_modereg_params1.s.rtt_nom_11 = default_rtt_nom[3];
                }

                ddr_config_write_csr(CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num), lmc_modereg_params1.u64);
                ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
                          rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
                          lmc_modereg_params1.s.rtt_nom_11,
                          lmc_modereg_params1.s.rtt_nom_10,
                          lmc_modereg_params1.s.rtt_nom_01,
                          lmc_modereg_params1.s.rtt_nom_00);

                ddr_print("RTT_WR      %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                          rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_11],
                          rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_10],
                          rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_01],
                          rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_00],
                          lmc_modereg_params1.s.rtt_wr_11,
                          lmc_modereg_params1.s.rtt_wr_10,
                          lmc_modereg_params1.s.rtt_wr_01,
                          lmc_modereg_params1.s.rtt_wr_00);

                ddr_print("DIC         %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                          dic_ohms[lmc_modereg_params1.s.dic_11],
                          dic_ohms[lmc_modereg_params1.s.dic_10],
                          dic_ohms[lmc_modereg_params1.s.dic_01],
                          dic_ohms[lmc_modereg_params1.s.dic_00],
                          lmc_modereg_params1.s.dic_11,
                          lmc_modereg_params1.s.dic_10,
                          lmc_modereg_params1.s.dic_01,
                          lmc_modereg_params1.s.dic_00);

                lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                if (ddr_rodt_ctl_auto)
                    lmc_comp_ctl2.cn61xx.rodt_ctl = auto_rodt_ctl;
                else
                    lmc_comp_ctl2.cn61xx.rodt_ctl = default_rodt_ctl;
                ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                lmc_comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                          lmc_comp_ctl2.cn61xx.rodt_ctl, rodt_ohms[lmc_comp_ctl2.cn61xx.rodt_ctl]);

                for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                    if (!(rank_mask & (1 << rankx)))
                        continue;
                    lmc_rlevel_rank.u64 = rlevel_scoreboard[auto_rtt_nom][auto_rodt_ctl][rankx].setting;
                    if (!ecc_ena){
                        lmc_rlevel_rank.cn68xx.byte8 = lmc_rlevel_rank.cn68xx.byte0; /* ECC is not used */
                    }
                    ddr_config_write_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), lmc_rlevel_rank.u64);
                    lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                    ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d)\n",
                              rankx,
                              lmc_rlevel_rank.cn68xx.status,
                              lmc_rlevel_rank.u64,
                              lmc_rlevel_rank.cn68xx.byte8,
                              lmc_rlevel_rank.cn68xx.byte7,
                              lmc_rlevel_rank.cn68xx.byte6,
                              lmc_rlevel_rank.cn68xx.byte5,
                              lmc_rlevel_rank.cn68xx.byte4,
                              lmc_rlevel_rank.cn68xx.byte3,
                              lmc_rlevel_rank.cn68xx.byte2,
                              lmc_rlevel_rank.cn68xx.byte1,
                              lmc_rlevel_rank.cn68xx.byte0,
                              rlevel_scoreboard[auto_rtt_nom][auto_rodt_ctl][rankx].score
                              );
                }
            }
        } /* while(rlevel_debug_loops--) */

        lmc_control.s.ddr2t           = save_ddr2t;
        ddr_config_write_csr(CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);
        lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_interface_num));
        ddr_print("DDR2T                                         : %6d\n", lmc_control.s.ddr2t); /* Display final 2T value */



        perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

            for (i=0; i<9; ++i) {
                if ((s = lookup_env_parameter("ddr%d_rlevel_rank%d_byte%d", ddr_interface_num, rankx, i)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);

                    update_rlevel_rank_struct(&lmc_rlevel_rank, i, value);
                }
            }

            if ((s = lookup_env_parameter_ull("ddr%d_rlevel_rank%d", ddr_interface_num, rankx)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                lmc_rlevel_rank.u64 = value;
            }

            if (parameter_set) {
                ddr_config_write_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), lmc_rlevel_rank.u64);
                lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

                ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_rlevel_rank.cn68xx.status,
                          lmc_rlevel_rank.u64,
                          lmc_rlevel_rank.cn68xx.byte8,
                          lmc_rlevel_rank.cn68xx.byte7,
                          lmc_rlevel_rank.cn68xx.byte6,
                          lmc_rlevel_rank.cn68xx.byte5,
                          lmc_rlevel_rank.cn68xx.byte4,
                          lmc_rlevel_rank.cn68xx.byte3,
                          lmc_rlevel_rank.cn68xx.byte2,
                          lmc_rlevel_rank.cn68xx.byte1,
                          lmc_rlevel_rank.cn68xx.byte0
                          );
            }
        }
    }

    /*
     * 2. Without changing any other fields in LMC0_CONFIG,
     *
     *    o write LMC0_CONFIG[SEQUENCE] to select read-leveling, and
     *
     *    o write LMC0_CONFIG[RANKMASK] = (1 << i), and
     *
     *    o write LMC0_CONFIG[INIT_START] = 1
     *
     *    This initiates the previously-described read-leveling.
     *
     * 3. Wait until LMC0_RLEVEL_RANKi[STATUS] != 2
     *
     *    LMC will have updated LMC0_RLEVEL_RANKi[BYTE*] for all
     *    byte lanes at this point.
     *
     * 4. If desired, consult LMC0_RLEVEL_DBG[BITMASK] and compare
     *    to LMC0_RLEVEL_RANKi[BYTE*] for the lane selected by
     *    LMC0_RLEVEL_CTL[BYTE]. If desired, modify
     *    LMC0_RLEVEL_CTL[BYTE] to a new value and repeat so that
     *    all BITMASKs can be observed.
     *
     * 5. Initialize LMC0_RLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    LMC0_RLEVEL_RANKj = LMC0_RLEVEL_RANKi.
     */

    {
        /* Try to determine/optimize write-level delays experimentally. */
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        int byte;
        int delay;
        int rankx = 0;
        int save_ecc_ena;
        int active_rank;
        int sw_wlevel_offset = 1;
        int sw_wlevel_enable = 1;
        int interfaces;
        typedef enum {
            WL_ESTIMATED = 0,   /* HW/SW wleveling failed. Results
                                   estimated. */
            WL_HARDWARE  = 1,   /* H/W wleveling succeeded */
            WL_SOFTWARE  = 2,   /* S/W wleveling passed 2 contiguous
                                   settings. */
            WL_SOFTWARE1 = 3,   /* S/W wleveling passed 1 marginal
                                   setting. */
        } sw_wl_status_t;

        static const char *wl_status_strings[] = {
            "(e)",
            "   ",
            "   ",
            "(1)"
        };

        if ((s = lookup_env_parameter("ddr_software_wlevel")) != NULL) {
            sw_wlevel_enable = simple_strtoul(s, NULL, 0);
        }

        if (sw_wlevel_enable)
            ddr_print("Performing software Write-Leveling\n");

        /* Disable ECC for DRAM tests */
        lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_interface_num));
        save_ecc_ena = lmc_config.s.ecc_ena;
        lmc_config.s.ecc_ena = 0;
        ddr_config_write_csr(CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
#ifndef __U_BOOT__
        limit_l2_ways(0, 0);       /* Disable l2 sets for DRAM testing */
#endif
        /* We need to track absolute rank number, as well as how many
        ** active ranks we have.  Two single rank DIMMs show up as
        ** ranks 0 and 2, but only 2 ranks are active. */
        active_rank = 0;

	interfaces = __builtin_popcount(ddr_interface_mask);

        for (rankx = 0; rankx < dimm_count * 4; rankx++) {
            uint64_t rank_addr;

            sw_wl_status_t byte_test_status[9] = {WL_ESTIMATED};
            sw_wl_status_t sw_wl_rank_status = WL_HARDWARE;
            int bytes_failed = 0;

            if (!sw_wlevel_enable)
                break;

            if (!(rank_mask & (1 << rankx)))
                continue;


            lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));
            if (wlevel_bitmask_errors == 0) {
                /* Determine address of DRAM to test for software write leveling. */
                rank_addr  = active_rank * ((1ull << (pbank_lsb+interfaces/2))/(1+bunk_enable));
                rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */

                /* Adjust address for boot bus hole in memory map. */
                if (rank_addr > 0x10000000)
                    rank_addr += 0x10000000;
                debug_print("Rank Address: 0x%llx\n", rank_addr);

                for (byte=0; byte<8; ++byte) {
                    uint64_t byte_bitmask = 0xff;
                    if (!(ddr_interface_bytemask&(1<<byte)))
                        continue;

                    /* If we will be switching to 32bit mode only test 4 ECC bits.  */
                    if ((! ddr_interface_64b) && (byte == 4))
                        byte_bitmask = 0x0f;

                    /* If h/w write-leveling had no errors then use
                    ** s/w write-leveling to fixup only the upper bits
                    ** of the delays. */
                    for (delay=get_wlevel_rank_struct(&lmc_wlevel_rank, byte); delay<32; delay+=8) {
                        update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
                        debug_print("Testing byte %d delay %2d\n", byte, delay);
                        ddr_config_write_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                        lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                        if (!test_dram_byte(rank_addr, 2048, byte, byte_bitmask)) {
                            debug_print("        byte %d(0x%llx) delay %2d Passed\n", byte, rank_addr, delay);
                            byte_test_status[byte] = WL_HARDWARE;
                            break;
                        } else {
                            debug_print("        byte %d delay %2d Errors\n", byte, delay);
                        }
                    }
                }

                if (save_ecc_ena) {
                    int save_byte8 = lmc_wlevel_rank.cn68xx.byte8;
                    byte_test_status[8] = WL_HARDWARE; /* H/W delay value */

                    if ((save_byte8 != lmc_wlevel_rank.cn68xx.byte3) &&
                        (save_byte8 != lmc_wlevel_rank.cn68xx.byte4))
                    {
                        int test_byte8 = divide_nint(lmc_wlevel_rank.cn68xx.byte3
                                                     + lmc_wlevel_rank.cn68xx.byte4
                                                     + 2 /* round-up*/ , 2);
                        lmc_wlevel_rank.cn68xx.byte8 = test_byte8 & ~1; /* Use only even settings */
                    }

                    if (lmc_wlevel_rank.cn68xx.byte8 != save_byte8) {
                        /* Change the status if s/w adjusted the delay */
                        byte_test_status[8] = WL_SOFTWARE; /* Estimated delay */
                    }
                } else {
                    lmc_wlevel_rank.cn68xx.byte8 = lmc_wlevel_rank.cn68xx.byte0; /* ECC is not used */
                }
            }

            for (byte=0; byte<8; ++byte) {
                bytes_failed += (byte_test_status[byte]==WL_ESTIMATED);
            }

            if (bytes_failed) {
                uint64_t rank_addr;
                sw_wl_rank_status = WL_SOFTWARE;
                /* If s/w fixups failed then retry using s/w write-leveling. */
                if (wlevel_bitmask_errors == 0) {
                    /* h/w succeeded but s/w fixups failed. So retry s/w. */
                    debug_print("Rank(%d) Retrying software Write-Leveling.\n", rankx);
                }
                for (byte=0; byte<8; ++byte) {
                    int passed = 0;
                    int wl_offset;
                    uint64_t byte_bitmask = 0xff;

                    if (!(ddr_interface_bytemask&(1<<byte)))
                        continue;

                    /* If we will be switching to 32bit mode only test 4 ECC bits.  */
                    if ((! ddr_interface_64b) && (byte == 4))
                        byte_bitmask = 0x0f;

                    /* Determine address of DRAM to test for software write leveling. */
                    rank_addr  = active_rank * ((1ull << (pbank_lsb+interfaces/2))/(1+bunk_enable));
                    rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */

                    /* Adjust address for boot bus hole in memory map. */
                    if (rank_addr > 0x10000000)
                        rank_addr += 0x10000000;
                    debug_print("Rank Address: 0x%llx\n", rank_addr);

                    for (wl_offset = sw_wlevel_offset; wl_offset >= 0; --wl_offset) {
                        /* for (delay=30; delay>=0; delay-=2) { */ /* Top-Down */
                        for (delay=0; delay<32; delay+=2) {  /* Bottom-UP */
                            update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
                            debug_print("Testing byte %d delay %2d\n", byte, delay);
                            ddr_config_write_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                            lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                            if (!test_dram_byte(rank_addr, 2048, byte, byte_bitmask)) {
                                ++passed;
                                if (passed == (1+wl_offset)) { /* Look for consecutive working settings */
                                    debug_print("        byte %d(0x%llx) delay %2d Passed\n", byte, rank_addr, delay);
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
                        lmc_rlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                        rlevel_to_wlevel(&lmc_rlevel_rank, &lmc_wlevel_rank, byte);
                    }
                }

                if (save_ecc_ena) {
                    /* ECC byte has to be estimated. Take the average of the two surrounding bytes. */
                    int test_byte8 = divide_nint(lmc_wlevel_rank.cn68xx.byte3
                                                 + lmc_wlevel_rank.cn68xx.byte4
                                                 + 2 /* round-up*/ , 2);
                    lmc_wlevel_rank.cn68xx.byte8 = test_byte8 & ~1; /* Use only even settings */
                    byte_test_status[8] = WL_ESTIMATED; /* Estimated delay */
                } else {
                    lmc_wlevel_rank.cn68xx.byte8 = lmc_wlevel_rank.cn68xx.byte0; /* ECC is not used */
                }

                /* Set delays for unused bytes to match byte 0. */
                for (byte=0; byte<8; ++byte) {
                    if ((ddr_interface_bytemask&(1<<byte)))
                        continue;
                    update_wlevel_rank_struct(&lmc_wlevel_rank, byte, lmc_wlevel_rank.cn68xx.byte0);
                    byte_test_status[byte] = WL_SOFTWARE;
                }

            }

            ddr_config_write_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
            lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %s\n",
                      rankx,
                      lmc_wlevel_rank.cn68xx.status,
                      lmc_wlevel_rank.u64,
                      lmc_wlevel_rank.cn68xx.byte8, wl_status_strings[byte_test_status[8]],
                      lmc_wlevel_rank.cn68xx.byte7, wl_status_strings[byte_test_status[7]],
                      lmc_wlevel_rank.cn68xx.byte6, wl_status_strings[byte_test_status[6]],
                      lmc_wlevel_rank.cn68xx.byte5, wl_status_strings[byte_test_status[5]],
                      lmc_wlevel_rank.cn68xx.byte4, wl_status_strings[byte_test_status[4]],
                      lmc_wlevel_rank.cn68xx.byte3, wl_status_strings[byte_test_status[3]],
                      lmc_wlevel_rank.cn68xx.byte2, wl_status_strings[byte_test_status[2]],
                      lmc_wlevel_rank.cn68xx.byte1, wl_status_strings[byte_test_status[1]],
                      lmc_wlevel_rank.cn68xx.byte0, wl_status_strings[byte_test_status[0]],
                      (sw_wl_rank_status == WL_HARDWARE) ? "" : "(s)"
                      );

            active_rank++;
        }


        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            for (i=0; i<9; ++i) {
                if ((s = lookup_env_parameter("ddr%d_wlevel_rank%d_byte%d", ddr_interface_num, rankx, i)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);

                    update_wlevel_rank_struct(&lmc_wlevel_rank, i, value);
                }
            }

            if ((s = lookup_env_parameter_ull("ddr%d_wlevel_rank%d", ddr_interface_num, rankx)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                lmc_wlevel_rank.u64 = value;
            }

            if (parameter_set) {
                ddr_config_write_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                lmc_wlevel_rank.u64 = cvmx_read_csr(CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_wlevel_rank.cn68xx.status,
                          lmc_wlevel_rank.u64,
                          lmc_wlevel_rank.cn68xx.byte8,
                          lmc_wlevel_rank.cn68xx.byte7,
                          lmc_wlevel_rank.cn68xx.byte6,
                          lmc_wlevel_rank.cn68xx.byte5,
                          lmc_wlevel_rank.cn68xx.byte4,
                          lmc_wlevel_rank.cn68xx.byte3,
                          lmc_wlevel_rank.cn68xx.byte2,
                          lmc_wlevel_rank.cn68xx.byte1,
                          lmc_wlevel_rank.cn68xx.byte0
                          );
            }
        }

        /* Enable 32-bit mode if required. */
        lmc_config.s.mode32b         = (! ddr_interface_64b);

        /* Restore the ECC configuration */
        lmc_config.s.ecc_ena = save_ecc_ena;
        ddr_config_write_csr(CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.s.mode32b);
#ifndef __U_BOOT__
        /* Restore the l2 set configuration */
	{
	    const char *eptr;
            eptr = getenv("limit_l2_ways");
            if (eptr) {
                int ways = simple_strtoul(eptr, NULL, 10);
                limit_l2_ways(ways, 1);
            } else {
                limit_l2_ways(cvmx_l2c_get_num_assoc(), 0);
            }
            eptr = getenv("limit_l2_mask");
            if (eptr) {
                int mask = simple_strtoul(eptr, NULL, 10);
                limit_l2_mask(mask);
            }
	}
#endif
#ifndef CONFIG_OCTEON_OCX
	{
	    int num_assoc = cvmx_l2c_get_num_assoc();

	    /* When Authentik is used the number of ways is limited by the
	     * Octeon ROM.  We restore it here.
	     */
	    if (cvmx_l2c_get_core_way_partition(0) != num_assoc) {
		ddr_print("Restoring number of L2 ways to %d\n", num_assoc);
		limit_l2_ways(num_assoc, 0);
	    }
	}
#endif
    }


#if 1
    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_write_offset",
			       custom_lmc_config->dll_write_offset, "ddr%d_dll_write_offset_byte%d", 1);
    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_read_offset",
			       custom_lmc_config->dll_read_offset,  "ddr%d_dll_read_offset_byte%d",  2);
#else
    if (((s = lookup_env_parameter("ddr_dll_write_offset")) != NULL) ||
        is_dll_offset_provided(custom_lmc_config->dll_write_offset)) {
        cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
        int byte_offset;
        unsigned short offset[9] = {0};
        int i;
        char *s;

        for (i=0; i<9; ++i) {
            byte_offset = custom_lmc_config->dll_write_offset[i];

            if ((s = lookup_env_parameter("ddr%d_dll_write_offset_byte%d", ddr_interface_num, i)) != NULL) {
                byte_offset = simple_strtol(s, NULL, 0);
            }

            offset[i] = load_dll_offset(node, ddr_interface_num, cpu_id,
					1 /* write */, byte_offset, i);
        }

        ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));
        SET_DDR_DLL_CTL3(offset_ena, 1);
        ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);

        ddr_print("%-45s :  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x\n",
                  "DLL Write Offset 8:0",
                  offset[8], offset[7], offset[6], offset[5], offset[4],
                  offset[3], offset[2], offset[1], offset[0]);
    }

    if (((s = lookup_env_parameter("ddr_dll_read_offset")) != NULL) ||
        is_dll_offset_provided(custom_lmc_config->dll_read_offset)) {
        cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
        int byte_offset;
        unsigned short offset[9] = {0};
        int i;
        char *s;

        for (i=0; i<9; ++i) {
            byte_offset = custom_lmc_config->dll_read_offset[i];

            if ((s = lookup_env_parameter("ddr%d_dll_read_offset_byte%d", ddr_interface_num, i)) != NULL) {
                byte_offset = simple_strtol(s, NULL, 0);
            }

            offset[i] = load_dll_offset(node, ddr_interface_num, cpu_id,
					2 /* read */, byte_offset, i);
        }

        ddr_dll_ctl3.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL3(ddr_interface_num));
        SET_DDR_DLL_CTL3(offset_ena, 1);
        ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);

        ddr_print("%-45s :  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x  0x%02x\n",
                  "DLL Read Offset 8:0",
                  offset[8], offset[7], offset[6], offset[5], offset[4],
                  offset[3], offset[2], offset[1], offset[0]);
    }
#endif

    /*
     * 4.8.8 Final LMC Initialization
     *
     * Early LMC initialization, LMC write-leveling, and LMC
     * read-leveling must be completed prior to starting this
     * final LMC initialization.
     *
     * LMC hardware updates the LMC0_SLOT_CTL0, LMC0_SLOT_CTL1,
     * LMC0_SLOT_CTL2 CSRs with minimum values based on the
     * selected read-leveling and write-leveling
     * settings. Software should not write the final
     * LMC0_SLOT_CTL0, LMC0_SLOT_CTL1, and LMC0_SLOT_CTL2 values
     * until after the final readleveling and write-leveling
     * settings are written.
     *
     * Software must ensure the LMC0_SLOT_CTL0, LMC0_SLOT_CTL1,
     * and LMC0_SLOT_CTL2 CSR values are appropriate for this
     * step. These CSRs select the minimum gaps between reads and
     * writes of various types.
     *
     * Software must not reduce the values in these CSR fields
     * below the values previously selected by the LMC hardware
     * (during write-leveling and read-leveling steps above).
     *
     * All sections in this chapter may be used to derive proper
     * settings for these registers.
     */

    if (safe_ddr_flag) {
        cvmx_lmcx_slot_ctl0_t lmc_slot_ctl0;
        cvmx_lmcx_slot_ctl1_t lmc_slot_ctl1;
        cvmx_lmcx_slot_ctl2_t lmc_slot_ctl2;

        lmc_slot_ctl0.u64 = 0;
        lmc_slot_ctl0.cn68xx.r2r_init = 0x3f;
        lmc_slot_ctl0.cn68xx.r2w_init = 0x3f;
        lmc_slot_ctl0.cn68xx.w2r_init = 0x3f;
        lmc_slot_ctl0.cn68xx.w2w_init = 0x3f;
        ddr_config_write_csr(CVMX_LMCX_SLOT_CTL0(ddr_interface_num), lmc_slot_ctl0.u64);

        lmc_slot_ctl1.u64 = 0;
        lmc_slot_ctl1.cn68xx.r2r_xrank_init = 0x3f;
        lmc_slot_ctl1.cn68xx.r2w_xrank_init = 0x3f;
        lmc_slot_ctl1.cn68xx.w2r_xrank_init = 0x3f;
        lmc_slot_ctl1.cn68xx.w2w_xrank_init = 0x3f;
        ddr_config_write_csr(CVMX_LMCX_SLOT_CTL1(ddr_interface_num), lmc_slot_ctl1.u64);

        lmc_slot_ctl2.u64 = 0;
        lmc_slot_ctl2.cn68xx.r2r_xdimm_init = 0x3f;
        lmc_slot_ctl2.cn68xx.r2w_xdimm_init = 0x3f;
        lmc_slot_ctl2.cn68xx.w2r_xdimm_init = 0x3f;
        lmc_slot_ctl2.cn68xx.w2w_xdimm_init = 0x3f;
        ddr_config_write_csr(CVMX_LMCX_SLOT_CTL2(ddr_interface_num), lmc_slot_ctl2.u64);
    }

    {
        /* Clear any residual ECC errors */
        int num_tads = 1;
        int tad;

        ddr_config_write_csr(CVMX_LMCX_INT(ddr_interface_num), -1ULL);
        if (octeon_is_cpuid(OCTEON_CN63XX_PASS1_X))
            num_tads = 0; /* (not present in pass 1 O63) */
        if (octeon_is_cpuid(OCTEON_CN68XX) || octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX))
            num_tads = 4;
	else if (octeon_is_cpuid(OCTEON_CN78XX))
	    num_tads = 8;
        /* Initialize "Disabled LMC Error" status */
        for (tad=0; tad<num_tads; tad++)
            cvmx_write_csr(CVMX_L2C_TADX_INT(tad), cvmx_read_csr(CVMX_L2C_TADX_INT(tad)));
    }

    return(mem_size_mbytes);
}
#endif /* CONFIG_OCTEON_DISABLE_DDR3 */

int twsii_mcu_read_node(uint8_t node, uint8_t twsii_addr)
{
    /* Copied from  bootloader/u-boot/drivers/i2c/octeon_i2c.c */
#define node_bus_to_i2c_bus(node,bus)   ((node << 1) | bus)
    int bus = node_bus_to_i2c_bus(node, 0);

#ifndef BOARD_MCU_TWSI_ADDR
    (void) bus;
    return 0;
#else
# ifdef __U_BOOT__
    i2c_set_bus_num(bus);
    return i2c_reg_read(BOARD_MCU_TWSI_ADDR, twsii_addr);
# else
    return cvmx_twsix_read_ia8(bus, BOARD_MCU_TWSI_ADDR, twsii_addr, 1);
# endif /* U_BOOT */
#endif
}

int twsii_mcu_read(uint8_t twsii_addr)
{
#ifdef BOARD_MCU_TWSI_ADDR
# ifdef __U_BOOT__
    i2c_set_bus_num(0);
    return i2c_reg_read(BOARD_MCU_TWSI_ADDR, twsii_addr);
# else
    return cvmx_twsix_read_ia8(0, BOARD_MCU_TWSI_ADDR, twsii_addr, 1);
# endif
#else
    return 0;
#endif /* BOARD_MCU_TWSI_ADDR */
}

static void set_ddr_clock_initialized(int node, int ddr_interface,
				      bool inited_flag)
{
#ifdef __U_BOOT__
# ifndef CONFIG_OCTEON_OCX
    node = 0;
# endif
    if (inited_flag)
        gd->arch.ddr_int_initialized |= 1 << (ddr_interface + (node * 4));
    else
        gd->arch.ddr_int_initialized &=
            ~(1 << (ddr_interface + (node * 4)));
#else
    if (inited_flag)
        global_ddr_clock_initialized |=
            (0x1 << (ddr_interface + node * 4));
    else
        global_ddr_clock_initialized &=
            ~(0x1 << (ddr_interface + node * 4));
#endif

}

static int ddr_clock_initialized(int node, int ddr_interface)
{
#ifdef __U_BOOT__
# ifndef CONFIG_OCTEON_OCX
    node = 0;
# endif
    return !!(gd->arch.ddr_int_initialized & (1 << (ddr_interface + node * 4)));
#else
    return !!(global_ddr_clock_initialized & (1 << (ddr_interface + node * 4)));
#endif
}


static void cn78xx_lmc_dreset_init (int node, int ddr_interface_num)
{
    cvmx_lmcx_dll_ctl2_t	dll_ctl2;

#if defined(__U_BOOT__) && !defined(CONFIG_OCTEON_OCX)
    node = 0;	/* Optimize */
#endif
    /*
     * The remainder of this section describes the sequence for LMCn.
     *
     * 1. If not done already, write LMC(0..3)_DLL_CTL2 to its reset value
     * (except without changing the LMC(0..3)_DLL_CTL2[INTF_EN] value from
     * that set in the prior Step 3), including LMC(0..3)_DLL_CTL2[DRESET] = 1.
     *
     * 2. Without changing any other LMC(0..3)_DLL_CTL2 fields, write
     * LMC(0..3)_DLL_CTL2[DLL_BRINGUP] = 1.
     */

    dll_ctl2.u64 = cvmx_read_csr_node(node,
                                      CVMX_LMCX_DLL_CTL2(ddr_interface_num));
    dll_ctl2.cn78xx.dll_bringup = 1;
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_DLL_CTL2(ddr_interface_num),
                              dll_ctl2.u64);

    /*
     * 3. Read LMC(0..3)_DLL_CTL2 and wait for the result.
     */

    cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num));

    /*
     * 4. Wait for a minimum of 10 LMC CK cycles.
     */

    cvmx_wait_usec(1);

    /*
     * 5. Without changing any other fields in LMC(0..3)_DLL_CTL2, write
     * LMC(0..3)_DLL_CTL2[QUAD_DLL_ENA] = 1.
     * LMC(0..3)_DLL_CTL2[QUAD_DLL_ENA] must not change after this point
     * without restarting the LMCn DRESET initialization sequence.
     */

    dll_ctl2.u64 = cvmx_read_csr_node(node,
                                      CVMX_LMCX_DLL_CTL2(ddr_interface_num));
    dll_ctl2.cn78xx.quad_dll_ena = 1;
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_DLL_CTL2(ddr_interface_num),
                              dll_ctl2.u64);

    /*
     * 6. Read LMC(0..3)_DLL_CTL2 and wait for the result.
     */

    cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num));

    /*
     * 7. Wait a minimum of 10 us.
     */

    cvmx_wait_usec(10);

    /*
     * 8. Without changing any other fields in LMC(0..3)_DLL_CTL2, write
     * LMC(0..3)_DLL_CTL2[DLL_BRINGUP] = 0.
     * LMC(0..3)_DLL_CTL2[DLL_BRINGUP] must not change after this point
     * without restarting the LMCn DRESET initialization sequence.
     */

    dll_ctl2.u64 = cvmx_read_csr_node(node,
                                      CVMX_LMCX_DLL_CTL2(ddr_interface_num));
    dll_ctl2.cn78xx.dll_bringup = 0;
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

    /*
     * 9. Read LMC(0..3)_DLL_CTL2 and wait for the result.
     */

    cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num));

    /*
     * 10. Without changing any other fields in LMC(0..3)_DLL_CTL2, write
     * LMC(0..3)_DLL_CTL2[DRESET] = 0.
     * LMC(0..3)_DLL_CTL2[DRESET] must not change after this point without
     * restarting the LMCn DRESET initialization sequence.
     *
     * After completing LMCn DRESET initialization, all LMC CSRs may be
     * accessed.  Prior to completing LMC DRESET initialization, only
     * LMC(0..3)_DDR_PLL_CTL, LMC(0..3)_DLL_CTL2, LMC(0..3)_RESET_CTL, and
     * LMC(0..3)_COMP_CTL2 LMC CSRs can be accessed.
     */

    dll_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num));
    dll_ctl2.cn78xx.dreset= 0;
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);
}


int initialize_ddr_clock(uint32_t cpu_id,
			 const ddr_configuration_t *ddr_configuration,
                         uint32_t cpu_hertz,
                         uint32_t ddr_hertz,
                         uint32_t ddr_ref_hertz,
                         int ddr_interface_num,
                         uint32_t ddr_interface_mask,
			 int node)
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

    if (ddr_clock_initialized(node, ddr_interface_num))
        return 0;

#ifndef CONFIG_OCTEON_DISABLE_DDR2
    if ((octeon_is_cpuid(OCTEON_CN31XX)) || (octeon_is_cpuid(OCTEON_CN30XX))) {
        cvmx_lmc_ctl_t lmc_ctl;
        cvmx_lmc_ddr2_ctl_t ddr2_ctl;

        /*
         * DRAM Controller Initialization
         * The reference-clock inputs to the LMC (DDR2_REF_CLK_*) should be stable
         * when DCOK asserts (refer to Section 26.3). DDR_CK_* should be stable from that
         * point, which is more than 200 us before software can bring up the main memory
         * DRAM interface. The generated DDR_CK_* frequency is four times the
         * DDR2_REF_CLK_* frequency.
         * To initialize the main memory and controller, software must perform the following
         * steps in this order:
         *
         * 1. Write LMC_CTL with [DRESET] = 1, [PLL_BYPASS] = user_value, and
         * [PLL_DIV2] = user_value.
         */

        lmc_ctl.u64                 = cvmx_read_csr(CVMX_LMC_CTL);

        lmc_ctl.s.pll_bypass        = 0;
        lmc_ctl.s.pll_div2          = 0;
        lmc_ctl.cn31xx.dreset            = 1;
        ddr_config_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);

        /* 2. Read L2D_BST0 and wait for the result. */
        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /* 3. Wait 10 us (LMC_CTL[PLL_BYPASS] and LMC_CTL[PLL_DIV2] must not
         * transition after this) */
	cvmx_wait_usec(10);

        /* 4. Write LMC_DDR2_CTL[QDLL_ENA] = 1. */
        ddr2_ctl.u64 = cvmx_read_csr(CVMX_LMC_DDR2_CTL);
        ddr2_ctl.s.qdll_ena   = 1;
        ddr_config_write_csr(CVMX_LMC_DDR2_CTL, ddr2_ctl.u64);
        cvmx_read_csr(CVMX_LMC_DDR2_CTL);

        octeon_delay_cycles(2000);   /* must be 200 dclocks */

        /* 5. Read L2D_BST0 and wait for the result. */
        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /* 6. Wait 10 us (LMC_DDR2_CTL[QDLL_ENA] must not transition after this) */
	cvmx_wait_usec(10);

	/* 7. Write LMC_CTL[DRESET] = 0 (at this point, the DCLK is running and the
	 * memory controller is out of reset)
	 */
	lmc_ctl.cn31xx.dreset = 0;
	ddr_config_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);
	octeon_delay_cycles(2000);	/* must be 200 dclocks */
    }

    if (octeon_is_cpuid(OCTEON_CN58XX) || octeon_is_cpuid(OCTEON_CN50XX)) {
        cvmx_lmc_ctl_t lmc_ctl;
        cvmx_lmc_ddr2_ctl_t ddr2_ctl;
        cvmx_lmc_pll_ctl_t pll_ctl;
        cvmx_lmc_ctl1_t lmc_ctl1;

        if (ddr_ref_hertz == 0) {
            error_print("ERROR: DDR Reference Clock not specified.\n");
            return(-1);
        }

        /*
         * DCLK Initialization Sequence
         *
         * When the reference-clock inputs to the LMC (DDR2_REF_CLK_P/N) are
         * stable, perform the following steps to initialize the DCLK.
         *
         * 1. Write LMC_CTL[DRESET]=1, LMC_DDR2_CTL[QDLL_ENA]=0.
         */

        lmc_ctl.u64                 = cvmx_read_csr(CVMX_LMC_CTL);
        lmc_ctl.cn50xx.dreset       = 1;
        ddr_config_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);

        ddr2_ctl.u64                 = cvmx_read_csr(CVMX_LMC_DDR2_CTL);
        ddr2_ctl.s.qdll_ena     = 0;
        ddr_config_write_csr(CVMX_LMC_DDR2_CTL, ddr2_ctl.u64);

        /*
         * 2. Write LMC_PLL_CTL[CLKR, CLKF, EN*] with the appropriate values,
         *    while writing LMC_PLL_CTL[RESET_N] = 0, LMC_PLL_CTL[DIV_RESET] = 1.
         *    LMC_PLL_CTL[CLKR, CLKF, EN*] values must not change after this
         *    point without restarting the DCLK initialization sequence.
         */

        /* CLKF = (DCLK * (CLKR+1) * EN(2, 4, 6, 8, 12, 16))/DREF - 1 */
        {
            int en_idx, save_en_idx, best_en_idx=0;
            uint64_t clkf, clkr, max_clkf = 127;
            uint64_t best_clkf=0, best_clkr=0;
            uint64_t best_pll_MHz = 0, pll_MHz, min_pll_MHz = 1200, max_pll_MHz = 2500;
            uint64_t error = ddr_hertz, best_error = ddr_hertz; /* Init to max error */
            uint64_t lowest_clkf = max_clkf;
            uint64_t best_calculated_ddr_hertz = 0, calculated_ddr_hertz = 0;
            uint64_t orig_ddr_hertz = ddr_hertz;
            static const int _en[] = {2, 4, 6, 8, 12, 16};

            pll_ctl.u64 = 0;

            while (best_error == ddr_hertz)
            {

                for (clkr = 0; clkr < 64; ++clkr) {
                    for (en_idx=sizeof(_en)/sizeof(int)-1; en_idx>=0; --en_idx) {
                        save_en_idx = en_idx;
                        clkf = ((ddr_hertz) * (clkr+1) * _en[save_en_idx]);
                        clkf = divide_nint(clkf, ddr_ref_hertz) - 1;
                        pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;
                        calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * _en[save_en_idx]);
                        error = ddr_hertz - calculated_ddr_hertz;

                        if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz)) continue;
                        if (clkf > max_clkf) continue; /* PLL requires clkf to be limited values less than 128 */
                        if (_abs(error) > _abs(best_error)) continue;

#if defined(__U_BOOT__)
                        ddr_print("clkr: %2lu, en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d\n",
                                  clkr, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#else
                        ddr_print("clkr: %2llu, en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld\n",
                                  clkr, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#endif

                        if ((_abs(error) < _abs(best_error)) || (clkf < lowest_clkf)) {
                            lowest_clkf = clkf;
                            best_pll_MHz = pll_MHz;
                            best_calculated_ddr_hertz = calculated_ddr_hertz;
                            best_error = error;
                            best_clkr = clkr;
                            best_clkf = clkf;
                            best_en_idx = save_en_idx;
                        }
                    }
                }

#if defined(__U_BOOT__)
                ddr_print("clkr: %2lu, en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d <==\n",
                          best_clkr, _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#else
                ddr_print("clkr: %2llu, en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld <==\n",
                          best_clkr, _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#endif

                /* Try lowering the frequency if we can't get a working configuration */
                if (best_error == ddr_hertz) {
                    if (ddr_hertz < orig_ddr_hertz - 10000000)
                        break;
                    ddr_hertz -= 1000000;
                    best_error = ddr_hertz;
                }

            }
            if (best_error == ddr_hertz) {
                error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                return(-1);
            }

            pll_ctl.cn50xx.en2  = (best_en_idx == 0);
            pll_ctl.cn50xx.en4  = (best_en_idx == 1);
            pll_ctl.cn50xx.en6  = (best_en_idx == 2);
            pll_ctl.cn50xx.en8  = (best_en_idx == 3);
            pll_ctl.cn50xx.en12 = (best_en_idx == 4);
            pll_ctl.cn50xx.en16 = (best_en_idx == 5);

            pll_ctl.cn50xx.clkf = best_clkf;
            pll_ctl.cn50xx.clkr = best_clkr;
            pll_ctl.cn50xx.reset_n = 0;
            pll_ctl.cn50xx.fasten_n = 1;
            pll_ctl.cn50xx.div_reset = 1;

            ddr_config_write_csr(CVMX_LMC_PLL_CTL, pll_ctl.u64);
        }


        /*
         * 3. Read L2D_BST0 and wait for the result.
         */

        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /*
         * 4. Wait 5 usec.
         */
	cvmx_wait_usec(10);


        /*
         * 5. Write LMC_PLL_CTL[RESET_N] = 1 while keeping LMC_PLL_CTL[DIV_RESET]
         *    = 1. LMC_PLL_CTL[RESET_N] must not change after this point without
         *    restarting the DCLK initialization sequence.
         */

        pll_ctl.u64 = cvmx_read_csr(CVMX_LMC_PLL_CTL);
        pll_ctl.cn50xx.reset_n = 1;
        ddr_config_write_csr(CVMX_LMC_PLL_CTL, pll_ctl.u64);

        /*
         * 6. Read L2D_BST0 and wait for the result.
         */

        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /*
         * 7. Wait 500  (LMC_PLL_CTL[CLKR] + 1) reference-clock cycles.
         */

	cvmx_wait_usec(1000);

        /*
         * 8. Write LMC_PLL_CTL[DIV_RESET] = 0. LMC_PLL_CTL[DIV_RESET] must not
         *    change after this point without restarting the DCLK initialization
         *    sequence.
         */

        pll_ctl.u64 = cvmx_read_csr(CVMX_LMC_PLL_CTL);
        pll_ctl.cn50xx.div_reset = 0;
        ddr_config_write_csr(CVMX_LMC_PLL_CTL, pll_ctl.u64);

        /*
         * 9. Read L2D_BST0 and wait for the result.
         *
         * The DDR address clock frequency (DDR_CK_<5:0>_P/N) should be stable at
         * that point. Section 2.3.9 describes the DDR_CK frequencies resulting
         * from different reference-clock values and programmings.
         */

        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */


        /*
         * DRESET Initialization Sequence
         *
         * The DRESET initialization sequence cannot start unless DCLK is stable
         * due to a prior DCLK initialization sequence. Perform the following
         * steps to initialize DRESET.
         *
         * 1. Write LMC_CTL[DRESET] = 1 and LMC_DDR2_CTL[QDLL_ENA] = 0.
         */

        lmc_ctl.u64                 = cvmx_read_csr(CVMX_LMC_CTL);
        lmc_ctl.cn50xx.dreset       = 1;
        ddr_config_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);

        ddr2_ctl.u64                 = cvmx_read_csr(CVMX_LMC_DDR2_CTL);
        ddr2_ctl.s.qdll_ena     = 0;
        ddr_config_write_csr(CVMX_LMC_DDR2_CTL, ddr2_ctl.u64);

        /*
         * 2. Write LMC_DDR2_CTL[QDLL_ENA] = 1. LMC_DDR2_CTL[QDLL_ENA] must not
         *    change after this point without restarting the LMC and/or DRESET
         *    initialization sequence.
         */

        ddr2_ctl.u64 = cvmx_read_csr(CVMX_LMC_DDR2_CTL);
        ddr2_ctl.s.qdll_ena   = 1;
        ddr_config_write_csr(CVMX_LMC_DDR2_CTL, ddr2_ctl.u64);
        cvmx_read_csr(CVMX_LMC_DDR2_CTL);

        /*
         * 3. Read L2D_BST0 and wait for the result.
         */

        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

        /*
         * 4. Wait 10 usec.
         */
	cvmx_wait_usec(10);


        /*
         * 5. Write LMC_CTL[DRESET] = 0. LMC_CTL[DRESET] must not change after
         *    this point without restarting the DRAM-controller and/or DRESET
         *    initialization sequence.
         */

        lmc_ctl.u64                 = cvmx_read_csr(CVMX_LMC_CTL);
        lmc_ctl.cn50xx.dreset       = 0;
        ddr_config_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);


        /*
         * 6. Read L2D_BST0 and wait for the result.
         */

        cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */



        /*
         * LMC Initialization Sequence
         *
         * The LMC initialization sequence must be preceded by a DCLK and DRESET
         * initialization sequence.
         *
         * 1. Software must ensure there are no pending DRAM transactions.
         */

        /*
         * 2. Write LMC_CTL, LMC_CTL1, LMC_MEM_CFG1, LMC_DDR2_CTL,
         * LMC_RODT_CTL, LMC_DUAL_MEMCFG, and LMC_WODT_CTL with appropriate
         * values, if necessary. Refer to Sections 2.3.4, 2.3.5, and 2.3.7 regarding
         * these registers (and LMC_MEM_CFG0).
         */

        lmc_ctl1.u64 = cvmx_read_csr(CVMX_LMC_CTL1);
        lmc_ctl1.cn50xx.sil_mode = 1;

	if (ddr_verbose() && (s = getenv("ddr_sil_mode")) != NULL) {
	    lmc_ctl1.cn50xx.sil_mode = simple_strtoul(s, NULL, 0);
	    error_print("Parameter found in environment. ddr_sil_mode = %d\n", lmc_ctl1.cn50xx.sil_mode);
	}

        ddr_config_write_csr(CVMX_LMC_CTL1, lmc_ctl1.u64);

        /*
         * 3. Write LMC_MEM_CFG0 with appropriate values and
         * LMC_MEM_CFG0[INIT_START] = 0.
         */

        /*
         * 4. Write LMC_MEM_CFG0 with appropriate values and
         *    LMC_MEM_CFG0[INIT_START] = 1. At that point, CN58XX hardware
         *    initiates the standard DDR2 init sequence shown in Figure 29.
         *
         *    CN58XX activates DDR_CKE (if it has not already been
         *    activated). DDR_CKE remains activated from that point until a
         *    subsequent DRESET.
         *
         *    CN58XX then follows with the standard DDR2 initialization sequence,
         *    not using OCD. While CN58XX performs the initialization sequence,
         *    it cannot perform other DDR2 transactions.
         *
         *    Note that if there is not a DRESET between two LMC initialization
         *    sequences, DDR_CKE remains asserted through the second
         *    initialization sequence. The hardware initiates the same DDR2
         *    initialization sequence as the first, except that DDR_CKE does not
         *    deactivate. If DDR_CKE deactivation and reactivation is desired for
         *    a second controller reset, a DRESET sequence is required.
         */

        /*
         * 5. Read L2D_BST0 and wait for the result.
         *
         * After this point, the LMC is fully functional.
         * LMC_MEM_CFG0[INIT_START] must not transition from 0.1 during normal
         * operation.
         */

    }


    if ((octeon_is_cpuid(OCTEON_CN56XX)) || (octeon_is_cpuid(OCTEON_CN52XX))) {
        cvmx_l2c_cfg_t l2c_cfg;
        cvmx_lmc_pll_ctl_t pll_ctl;
        cvmx_lmcx_dll_ctl_t lmc_dll_ctl;
        cvmx_lmc_bist_ctl_t lmc_bist_ctl;
        cvmx_lmc_bist_result_t lmc_bist_result;
        cvmx_lmc_ctl1_t lmc_ctl1;
        cvmx_lmc_dclk_ctl_t lmc_dclk_ctl;
        int ddr_interface_enabled = 0;

        ddr_ref_hertz = 50000000;

        /*
         * 2.3.8 DRAM Controller Initialization
         *
         *       There are five parts to the LMC-initialization procedure:
         *
         *       1. Global DCLK initialization
         *       2. LMC0 DRESET initialization
         *       3. LMC1 DCLK offsetting
         *       4. LMC1 DRESET initialization
         *       5. LMC0/LMC1 initialization
         *
         *       During a cold reset, all five initializations should be executed
         *       in sequence. The global DCLK and LMC1 DCLK initialization
         *       (i.e. steps 1 and 3) need only be performed once if the DCLK
         *       speed and parameters do not change. Subsequently, it is possible
         *       to execute only DRESET and LMC initializations, or to execute
         *       only the LMC0/LMC1 initialization.
         *
         *
         * 2.3.8.1 Global DCLK Initialization Sequence
         *
         *       Perform the following steps to initialize the DCLK.
         *
         *       1.  First, L2C_CFG[DPRES0,DPRES1] must be set to enable the LMC
         *           controllers that have DRAM attached. L2C_CFG[DPRES0,DPRES1]
         *           must not change after this point without restarting the
         *           global DCLK initialization sequence. Refer to Section 2.2.5.
         */

	if (octeon_is_cpuid(OCTEON_CN56XX)) {
	    l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);

	    if ((ddr_interface_num == 0) && (ddr_interface_mask & 1)) {
		l2c_cfg.cn56xx.dpres0 = 1; /* Enable Interface */
		ddr_interface_enabled = 1;
	    }

	    if ((ddr_interface_num == 1) && (ddr_interface_mask & 2)) {
		l2c_cfg.cn56xx.dpres1 = 1; /* Enable Interface */
		ddr_interface_enabled = 1;
	    }

	    ddr_config_write_csr(CVMX_L2C_CFG, l2c_cfg.u64);
	} else if (octeon_is_cpuid(OCTEON_CN52XX)) {
            /* Unlike the bits for the CN56XX, this bit does not do anything.
            ** We set it here to indicate to the host the the LMC is setup up, and this
            ** allows the same host code to work for the CN52XX CN56XX */
	    l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);
            l2c_cfg.cn56xx.dpres0 = 1; /* Use spare bit to indiate interface is enabled */
	    ddr_config_write_csr(CVMX_L2C_CFG, l2c_cfg.u64);
            ddr_interface_enabled = 1;
        } else {
	    ddr_interface_enabled = 1; /* Interface is always enabled */
	}

        ddr_print("DDR Interface %d: %s,  Mask 0x%x\n",
                  ddr_interface_num,
                  ddr_interface_enabled ? "Enabled" : "Disabled",
                  ddr_interface_mask);

        /*       2. If not done already, set LMC0/1_DLL_CTL=0, except that LMC0/
         *          1_DLL_CTL[DRESET]=1, for each LMC controller that is enabled.
         */


        lmc_dll_ctl.u64 = 0;
        lmc_dll_ctl.cn56xx.dreset = 1;

        ddr_config_write_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num), lmc_dll_ctl.u64);


        /*       3. Write LMC1_DCLK_CTL=0. */

	if (octeon_is_cpuid(OCTEON_CN56XX)) {
            ddr_config_write_csr(CVMX_LMCX_DCLK_CTL(1), 0);
        }


        /*       Note that the remaining global DCLK initialization steps
         *       reference only LMC0 CSRs, not LMC1. This is because there is
         *       only one global DCLK PLL, and it is attached only to LMC0.
         */

        /* Since there is only one global DCLK PLL this only needs to
         * be performed once.  Therefore, if LMC0_PLL_CTL[RESET_N] = 1
         * then this sequence has already been performed. */

        pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_PLL_CTL(0));
        if (pll_ctl.cn56xx.reset_n == 0) {
            /*       4.  Write LMC0_PLL_CTL[CLKF,CLKR,EN*] with the appropriate values, while
             *           writing LMC0_PLL_CTL[RESET_N] = 0, LMC0_PLL_CTL[DIV_RESET] = 1.
             *           LMC0_PLL_CTL[CLKF,CLKR,EN*] values must not change after
             *           this point without restarting the DCLK initialization
             *           sequence.
             */

            /* CLKF = (DCLK * (CLKR+1) * EN(2, 4, 6, 8, 12, 16))/DREF - 1 */
            int en_idx, save_en_idx, best_en_idx=0;
            uint64_t clkf, clkr, max_clkf = 127;
            uint64_t best_clkf=0, best_clkr=0;
            uint64_t best_pll_MHz = 0, pll_MHz, min_pll_MHz = 1200, max_pll_MHz = 2500;
            uint64_t error = ddr_hertz, best_error = ddr_hertz; /* Init to max error */
            uint64_t lowest_clkf = max_clkf;
            uint64_t best_calculated_ddr_hertz = 0, calculated_ddr_hertz = 0;
            uint64_t orig_ddr_hertz = ddr_hertz;
            static const int _en[] = {2, 4, 6, 8, 12, 16};

            ddr_print("DDR Interface %d: %s, Target DCLK: %d\n", ddr_interface_num,
                      "Enabling DCLK PLL", ddr_hertz);
            pll_ctl.u64 = 0;

            while (best_error == ddr_hertz)
            {

                for (clkr = 0; clkr < 64; ++clkr) {
                    for (en_idx=sizeof(_en)/sizeof(int)-1; en_idx>=0; --en_idx) {
                        save_en_idx = en_idx;
                        clkf = ((ddr_hertz) * (clkr+1) * _en[save_en_idx]);
                        clkf = divide_nint(clkf, ddr_ref_hertz) - 1;
                        pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;
                        calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * _en[save_en_idx]);
                        error = ddr_hertz - calculated_ddr_hertz;

                        if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz)) continue;
                        if (clkf > max_clkf) continue; /* PLL requires clkf to be limited values less than 128 */
                        if (_abs(error) > _abs(best_error)) continue;

#if defined(__U_BOOT__)
                        ddr_print("clkr: %2lu, en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d\n",
                                  clkr, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#else
                        ddr_print("clkr: %2llu, en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld\n",
                                  clkr, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#endif

                        if ((_abs(error) < _abs(best_error)) || (clkf < lowest_clkf)) {
                            lowest_clkf = clkf;
                            best_pll_MHz = pll_MHz;
                            best_calculated_ddr_hertz = calculated_ddr_hertz;
                            best_error = error;
                            best_clkr = clkr;
                            best_clkf = clkf;
                            best_en_idx = save_en_idx;
                        }
                    }
                }

#if defined(__U_BOOT__)
                ddr_print("clkr: %2lu, en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d <==\n",
                          best_clkr, _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#else
                ddr_print("clkr: %2llu, en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld <==\n",
                          best_clkr, _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#endif

                /* Try lowering the frequency if we can't get a working configuration */
                if (best_error == ddr_hertz) {
                    if (ddr_hertz < orig_ddr_hertz - 10000000)
                        break;
                    ddr_hertz -= 1000000;
                    best_error = ddr_hertz;
                }

            }


            if (best_error == ddr_hertz) {
                error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                return(-1);
            }

            pll_ctl.cn56xx.en2  = (best_en_idx == 0);
            pll_ctl.cn56xx.en4  = (best_en_idx == 1);
            pll_ctl.cn56xx.en6  = (best_en_idx == 2);
            pll_ctl.cn56xx.en8  = (best_en_idx == 3);
            pll_ctl.cn56xx.en12 = (best_en_idx == 4);
            pll_ctl.cn56xx.en16 = (best_en_idx == 5);

            pll_ctl.cn56xx.clkf = best_clkf;
            pll_ctl.cn56xx.clkr = best_clkr;
            pll_ctl.cn56xx.reset_n = 0;
            pll_ctl.cn56xx.fasten_n = 1;
            pll_ctl.cn56xx.div_reset = 1;
            pll_ctl.cn56xx.fasten_n = 1; /* Must always be set due to G-800 */

            ddr_config_write_csr(CVMX_LMCX_PLL_CTL(0), pll_ctl.u64);


            /*       5.  Read L2D_BST0 and wait for the result. */
            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

            /*       6.  Wait 5 usec. */
            cvmx_wait_usec(10);


            /*       7.  Write LMC0_PLL_CTL[RESET_N] = 1 while keeping
             *           LMC0_PLL_CTL[DIV_RESET] = 1. LMC0_PLL_CTL[RESET_N] must not
             *           change after this point without restarting the DCLK
             *           initialization sequence.
             */

            pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_PLL_CTL(0));
            pll_ctl.cn56xx.reset_n = 1;
            ddr_config_write_csr(CVMX_LMCX_PLL_CTL(0), pll_ctl.u64);

            /*       8.  Read L2D_BST0 and wait for the result. */
            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */


            /*       9.  Wait 500 ~ (LMC0_PLL_CTL[CLKR] + 1) reference-clock cycles. */
            cvmx_wait_usec(1000);


            /*       10. Write LMC0_PLL_CTL[DIV_RESET] = 0. LMC0_PLL_CTL[DIV_RESET]
             *           must not change after this point without restarting the DCLK
             *           initialization sequence.
             */

            pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_PLL_CTL(0));
            pll_ctl.cn56xx.div_reset = 0;
            ddr_config_write_csr(CVMX_LMCX_PLL_CTL(0), pll_ctl.u64);

            /*       11. Read L2D_BST0 and wait for the result.
             *
             *       The DDR address clock frequency (DDR_CK_<5:0>_P/N) should be
             *       stable at that point. Section 2.3.9 describes the DDR_CK
             *       frequencies resulting from different reference-clock values and
             *       programmings.
             */

            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */
        }


        /*
         * Only perform the LMC1 DCLK Offsetting Sequence if both
         * interfaces are enabled.  This can only happen on the second
         * pass of this procedure.
         */
        if (octeon_is_cpuid(OCTEON_CN56XX) && (l2c_cfg.cn56xx.dpres0 && l2c_cfg.cn56xx.dpres1)) {
            lmc_dclk_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DCLK_CTL(1));
            if (lmc_dclk_ctl.cn56xx.off90_ena == 0) {
                ddr_print("DDR Interface %d: %s\n", ddr_interface_num,
                          "Performing LMC1 DCLK Offsetting Sequence");
                /* 2.3.8.3 LMC1 DCLK Offsetting Sequence
                 *
                 *       The LMC1 DCLK Offsetting sequence offsets the LMC1 DCLK relative
                 *       to the LMC0 DCLK (by approximately 90 degrees). Note that DCLK
                 *       offsetting only applies to LMC1, so the sequence only references
                 *       LMC1 CSRs.
                 *
                 *       The LMC1 DCLK offsetting sequence should be skipped if either
                 *       LMC0 or LMC1 are not being used (i.e. if either
                 *       L2C_CFG[DPRES0]=0 or L2C_CFG[DPRES1]=0).
                 *
                 *       1.  Write LMC1_DCLK_CTL[DCLK90_LD]=1 to cause the LMC1
                 *           offsetting DLL to sample the delay setting from
                 *           LMC0. (LMC1_DCLK_CTL should be zero prior to this.)
                 */

                lmc_dclk_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DCLK_CTL(1));
                lmc_dclk_ctl.cn56xx.dclk90_ld = 1;
                ddr_config_write_csr(CVMX_LMCX_DCLK_CTL(1), lmc_dclk_ctl.u64);

                /*       2.  Write LMC1_DCLK_CTL[DCLK90_LD]=0 and
                 *           LMC1_DCLK_CTL[OFF90_ENA]=1 to enable the offsetting DLL.
                 *           LMC1_DCLK_CTL must not change after this point without
                 *           restarting the LMC1 DCLK offsetting initialization sequence
                 *           and redoing LMC1 DRESET sequence.
                 */

                lmc_dclk_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DCLK_CTL(1));
                lmc_dclk_ctl.cn56xx.dclk90_ld = 0;
                lmc_dclk_ctl.cn56xx.off90_ena = 1;
                ddr_config_write_csr(CVMX_LMCX_DCLK_CTL(1), lmc_dclk_ctl.u64);

                /*       3.  Read L2D_BST0 and wait for the result. */

                cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */
            }
        }


        /*
         *  This section handles the DRESET Initialization Sequence
         *  for both interfaces.  The comments taken from the HRM
         *  describe the sequence for interface 0.  Therefore, this
         *  same section is used on a subsequent call to perform the
         *  sequence described in section "2.3.8.4 LMC1 DRESET
         *  Initialization Sequence".
         */

        if (ddr_interface_enabled) {
            ddr_print("DDR Interface %d: %s\n", ddr_interface_num,
                      "Performing DRESET Initialization Sequence");
            /*
             *
             * 2.3.8.2 LMC0 DRESET Initialization Sequence
             *
             *       The LMC0 DRESET initialization should be skipped if LMC0 is not
             *       being used (i.e. if L2C_CFG[DPRES0]=0).
             *
             *       The LMC0 DRESET initialization sequence cannot start unless DCLK
             *       is stable due to a prior DCLK initialization sequence. Perform
             *       the following steps to initialize LMC0 DRESET.
             *
             * 2.3.8.3 LMC1 DCLK Offsetting Sequence (See above)
             *
             * 2.3.8.4 LMC1 DRESET Initialization Sequence
             *
             *       The LMC1 DRESET initialization should be skipped if LMC1 is not being used (i.e. if
             *       L2C_CFG[DPRES1]=0).
             *
             *       The LMC1 DRESET initialization sequence cannot start unless
             *       LMC0's DCLK is stable due to a prior global DCLK initialization
             *       sequence and, if necessary, a prior LMC1 DCLK
             *       offsetting. Perform the following steps to initialize DRESET.
             *
             *       The LMC1 DRESET sequence steps are identical to the LMC0 DRESET
             *       steps described in Section 2.3.8.2 above, except that LMC1 CSRs
             *       are used rather than LMC0.
             */


            /*       1.  If not done already, set LMC0_DLL_CTL=0, except that
             *           LMC0_DLL_CTL[DRESET]=1.
             */
#if 1
            /* Already done during step 2 of 2.3.8.1 Global DCLK Initialization Sequence.
               Why is it prescribed again? */
            lmc_dll_ctl.u64 = 0;
            lmc_dll_ctl.cn56xx.dreset = 1;
            ddr_config_write_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num), lmc_dll_ctl.u64);
#endif


            /*       2.  Write LMC0_DLL_CTL[DLL90_ENA] = 1.
             *           LMC0_DLL_CTL[DLL90_ENA,DLL90_BYP] must not change after this
             *           point without restarting the LMC and/or DRESET
             *           initialization sequence.
             */

            lmc_dll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num));
            lmc_dll_ctl.cn56xx.dll90_ena = 1;
            ddr_config_write_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num), lmc_dll_ctl.u64);


            /*       3.  Read L2D_BST0 and wait for the result. */

            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

            /*       4.  Wait 10 usec. */
            cvmx_wait_usec(10);


            /*       5.  Write LMC0_BIST_CTL[START]=1 and back to zero to start BIST
             *           of the DCLK memories in LMC. (LMC0_BIST_CTL[START] should
             *           have previously been zero.)
             */

            lmc_bist_ctl.u64 = cvmx_read_csr(CVMX_LMCX_BIST_CTL(ddr_interface_num));
            lmc_bist_ctl.cn56xx.start = 0; /* Incase it wasn't already 0 */
            ddr_config_write_csr(CVMX_LMCX_BIST_CTL(ddr_interface_num), lmc_bist_ctl.u64);
            lmc_bist_ctl.cn56xx.start = 1; /* Start BIST */
            ddr_config_write_csr(CVMX_LMCX_BIST_CTL(ddr_interface_num), lmc_bist_ctl.u64);
            lmc_bist_ctl.cn56xx.start = 0; /* Leave it zeroed */
            ddr_config_write_csr(CVMX_LMCX_BIST_CTL(ddr_interface_num), lmc_bist_ctl.u64);


            /*       6.  Read L2D_BST0 and wait for the result. */

            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */

            /*       7.  Wait 3000 core-clock cycles + 3000 memory-clock cycles for
             *           BIST to complete. (25 us should cover the longest possible
             *           case.)
             */
            cvmx_wait_usec(1000);

            /*       8.  Read LMC0_BIST_RESULT. If LMC0_BIST_RESULT is not zero, it
             *           is a fatal error.
             */

            lmc_bist_result.u64 = cvmx_read_csr(CVMX_LMCX_BIST_RESULT(ddr_interface_num));
            if (lmc_bist_result.u64 != 0) {
                error_print("BIST ERROR: 0x%llx\n", (unsigned long long)lmc_bist_result.u64);
                error_print("CSRD2E = 0x%x\n", lmc_bist_result.cn56xx.csrd2e);
                error_print("CSRE2D = 0x%x\n", lmc_bist_result.cn56xx.csre2d);
                error_print("MWF    = 0x%x\n", lmc_bist_result.cn56xx.mwf);
                error_print("MWD    = 0x%x\n", lmc_bist_result.cn56xx.mwd);
                error_print("MWC    = 0x%x\n", lmc_bist_result.cn56xx.mwc);
                error_print("MRF    = 0x%x\n", lmc_bist_result.cn56xx.mrf);
                error_print("MRD    = 0x%x\n", lmc_bist_result.cn56xx.mrd);
                return(-1);
            }

            /*       9.  Write LMC0_DLL_CTL[DRESET] = 0. LMC0_DLL_CTL[DRESET] must
             *           not change after this point without restarting the
             *           DRAM-controller and/or DRESET initialization sequence.
             */

            lmc_dll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num));
            lmc_dll_ctl.cn56xx.dreset = 0;
            ddr_config_write_csr(CVMX_LMCX_DLL_CTL(ddr_interface_num), lmc_dll_ctl.u64);

            /*       10. Read L2D_BST0 and wait for the result. */

            cvmx_read_csr(CVMX_L2D_BST0); /* Read CVMX_L2D_BST0 */
        }


        /*
         * 2.3.8.5 LMC0/LMC1 Initialization Sequence
         *
         *       The LMC initialization sequence must be preceded by a DCLK and DRESET
         *       initialization sequence.
         *
         *       Perform these steps for all enabled LMCs (i.e. perform these
         *       steps for LMC0 if L2C_CFG[DPRES0]=1, and separately perform
         *       these steps for LMC1 if L2C_CFG[DPRES0]=1).
         */

        if (ddr_interface_enabled) {
            ddr_print("DDR Interface %d: %s\n", ddr_interface_num,
                      "Performing LMCX Initialization Sequence");
            /*
             *       1.  Software must ensure there are no pending DRAM transactions.
             *
             *       2.  Write LMC0/1_CTL, LMC0/1_CTL1, LMC0/1_MEM_CFG1, LMC0/
             *           1_DDR2_CTL, LMC0/1_RODT_COMP_CTL, LMC0/1_DUAL_MEMCFG, LMC0/
             *           1_WODT_CTL, LMC0/1_READ_LEVEL_RANKn, and LMC0/
             *           1_READ_LEVEL_CTL with appropriate values, if
             *           necessary. Refer to Sections 2.3.4, 2.3.5, and 2.3.7
             *           regarding these registers (and LMC0/1_MEM_CFG0).
             */

            lmc_ctl1.u64 = cvmx_read_csr(CVMX_LMCX_CTL1(ddr_interface_num));

            lmc_ctl1.cn56xx.sil_mode = 1;
            lmc_ctl1.cn56xx.ecc_adr = 1; /* Include the address in ECC to pickup incorrect line loads */
            lmc_ctl1.cn56xx.sequence = 0; /* Required at startup */

            if (ddr_verbose() && (s = getenv("ddr_sil_mode")) != NULL) {
                lmc_ctl1.cn56xx.sil_mode = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_sil_mode = %d\n", lmc_ctl1.cn56xx.sil_mode);
            }

            ddr_config_write_csr(CVMX_LMCX_CTL1(ddr_interface_num), lmc_ctl1.u64);


            /*       3.  Write LMC0/1_MEM_CFG0 with appropriate values and
             *           LMC_MEM_CFG0[INIT_START] = 0.
             */


            /*       4.  If not done already, set LMC0/1_CTL1[SEQUENCE] to the
             *           desired initialization sequence.  This will normally be
             *           LMC0/1_CTL1[SEQUENCE]=0, selecting a power-up/init.
             */

            /*       5.  Write LMC0/1_MEM_CFG0 with appropriate values and
             *           LMC0/1_MEM_CFG0[INIT_START] = 1. At that point, CN56/57XX
             *           hardware initiates the initialization sequence selected by
             *           LMC0/1_CTL1[SEQUENCE].
             *
             *           Figure 2]12 shows the power-up/init sequence. CN56/57XX
             *           activates DDR_CKE as part of the power-up/init sequence (if
             *           it has not already been activated).  DDR_CKE remains
             *           activated from that point until a subsequent DRESET, except
             *           during power-down (refer to Section 2.3.11 below) or
             *           self-refresh entry.
             *
             *           After DDR_CKE asserts to start the power-up/init sequence,
             *           CN56/57XX then follows with the standard DDR2 initialization
             *           sequence, not using OCD. While CN56/57XX performs this
             *           initialization sequence, it cannot perform other DDR2
             *           transactions.
             *
             *           Note that if there is not a DRESET between two LMC
             *           power-up/init sequences, DDR_CKE remains asserted through
             *           the second sequence. The hardware initiates the same DDR2
             *           initialization sequence as the first, except that DDR_CKE
             *           does not deactivate. If DDR_CKE deactivation and
             *           reactivation is desired for a second controller reset, a
             *           DRESET sequence is required.
             *
             *       6.  Read L2D_BST0 and wait for the result.
             *
             *       After this point, the LMC is fully
             *       functional. LMC0/1_MEM_CFG0[INIT_START] should not transition
             *       from 0¨1 during normal operation.
             */
        } else {
            /* If this interface is not enabled don't try start it. */
            return(-1);
        }
    }
#endif /* CONFIG_OCTEON_DISABLE_DDR2 */
#ifndef CONFIG_OCTEON_DISABLE_DDR3

    if (!ddr_clock_initialized(node, 0)) { /* Do this once */
        int i;
        cvmx_lmcx_reset_ctl_t reset_ctl;
        /* Check to see if memory is to be preserved and set global flag */
        for (i = 3; i >= 0; --i) {
            if ((ddr_interface_mask & (1 << i)) == 0)
                continue;
            reset_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RESET_CTL(i));
            if (reset_ctl.s.ddr3psv == 1) {
                ddr_print("LMC%d Preserving memory\n", i);
                set_ddr_memory_preserved();

                /* Re-initialize flags */
                reset_ctl.s.ddr3pwarm = 0;
                reset_ctl.s.ddr3psoft = 0;
                reset_ctl.s.ddr3psv   = 0;
                cvmx_write_csr_node(node, CVMX_LMCX_RESET_CTL(i), reset_ctl.u64);
            }
        }
    }

    if (octeon_is_cpuid(OCTEON_CN63XX) || octeon_is_cpuid(OCTEON_CN66XX) ||
	octeon_is_cpuid(OCTEON_CN61XX) || octeon_is_cpuid(OCTEON_CNF71XX)) {
        /* Initialization Sequence
         *
         * There are seven parts to the LMC initialization procedure:
         *
         * 1. DDR PLL initialization
         *
         * 2. LMC CK initialization
         *
         * 3. LMC DRESET initialization
         *
         * 4. LMC RESET initialization
         *
         * 5. Early LMC initialization
         *
         * 6. LMC write leveling
         *
         * 7. LMC read leveling
         *
         * 8. Final LMC initialization
         *
         * During a cold reset, all seven initializations should be
         * executed in sequence. The DDR PLL and LMC CK
         * initializations need only be performed once if the
         * SDRAMclock speed and parameters do not
         * change. Subsequently, it is possible to execute only DRESET
         * and subsequent steps, or to execute only the final 3 steps.
         *
         * The remainder of this section covers these parts in
         * sequence.
         */

        cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;
        cvmx_lmcx_dll_ctl2_t dll_ctl2;
        cvmx_lmcx_reset_ctl_t lmc_reset_ctl;
        cvmx_dfm_dll_ctl2_t dfm_dll_ctl2;

        /*
         * 4.8.1 DDR PLL Initialization
         *
         * The same PLL is used by both LMC and the HFA memory
         * controller (HMC), so its initialization affects
         * both. Chapter 11 describes more about HMC.
         *
         * Perform the following steps to initialize the DDR PLL:
         *
         * 1. If not done already, write all fields in
         *    LMC0_DDR_PLL_CTL, LMC0_DLL_CTL2 to their reset value,
         *    including LMC0_DDR_PLL_CTL[DDR_DIV_RESET, DFM_DIV_RESET]
         *    = 1 and LMC0_DLL_CTL2[DRESET] = 1.
         */

        ddr_pll_ctl.u64 = 0;
        ddr_pll_ctl.cn63xx.dfm_div_reset = 1;
        ddr_pll_ctl.cn63xx.dfm_ps_en = 2;
        ddr_pll_ctl.cn63xx.ddr_div_reset = 1;
        ddr_pll_ctl.cn63xx.ddr_ps_en = 2;
        ddr_pll_ctl.cn63xx.clkf = 0x30;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        dll_ctl2.u64 = 0;
        dll_ctl2.cn63xx.dreset = 1;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         *    If the HMC is being used, also write all fields in
         *    DFM_DLL_CTL2 to their reset value, including
         *    DFM_DLL_CTL2[DRESET] = 1.
         */

	if (octeon_is_cpuid(OCTEON_CN63XX)) {
            dfm_dll_ctl2.u64 = 0;
            dfm_dll_ctl2.cn63xx.dreset = 1;
            ddr_config_write_csr(CVMX_DFM_DLL_CTL2, dfm_dll_ctl2.u64);
	}

        /*
         *    The above actions of this step are not necessary after a
         *    chip cold reset / power up.
         *
         *    If the current DRAM contents are not preserved (See
         *    LMC0_RESET_CTL [DDR3PSV]), this is also an appropriate
         *    time to assert the RESET# pin of the DDR3 DRAM parts. If
         *    desired, write LMC0_RESET_CTL[DDR3RST]=0 without
         *    modifying any other LMC0_RESET_CTL fields to assert the
         *    DDR_RESET_L pin.  No action is required here to assert
         *    DDR_RESET_L following a chip cold reset/ power up. Refer
         *    to Section 4.8.4.
         */

        if ( !ddr_memory_preserved()) {
            cvmx_lmcx_reset_ctl_t reset_ctl;
            reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
            reset_ctl.cn63xx.ddr3rst = 0; /* Reset asserted */
            ddr_print("LMC%d Asserting DDR_RESET_L\n", ddr_interface_num);
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_RESET_CTL(ddr_interface_num),
				      reset_ctl.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_RESET_CTL(ddr_interface_num));
        }

        /*
         * 2. Without changing any other LMC0_DDR_PLL_CTL values,
         *    write LMC0_DDR_PLL_CTL[CLKF] with a value that gives a
         *    desired DDR PLL speed.  The LMC0_DDR_PLL_CTL[CLKF] value
         *    should be selected in conjunction with both post-scalar
         *    divider values for LMC (LMC0_DDR_PLL_CTL[DDR_PS_EN]) and
         *    HMC (LMC0_DDR_PLL_CTL[DFM_PS_EN]) so that the desired
         *    LMC and HMC CK speeds are produced. Section 4.13
         *    describes LMC0_DDR_PLL_CTL[CLKF] and
         *    LMC0_DDR_PLL_CTL[DDR_PS_EN] programmings that produce
         *    the desired LMC CK speed. Section 4.8.2 describes LMC CK
         *    initialization, which can be done separately from the
         *    DDR PLL initialization described in this section.
         *
         *    The LMC0_DDR_PLL_CTL[CLKF] value must not change after
         *    this point without restarting this SDRAM PLL
         *    initialization sequence.
         */

        /* CLKF = (LMC CLK SPEED(MHz) * DDR_PS_EN(1, 2, 3, 4, 6, 8, 12)) / 50 (Ref CLK MHz) */
        {
            unsigned en_idx, save_en_idx, best_en_idx=0;
            uint64_t clkf, min_clkf = 32, max_clkf = 64;
            uint64_t best_clkf=0;
            uint64_t best_pll_MHz = 0, pll_MHz, highest_pll_MHz = 0;
            uint64_t min_pll_MHz = 1600, max_pll_MHz = 3000;
            int64_t  error = ddr_hertz, best_error = ddr_hertz; /* Init to max error */
            uint64_t best_calculated_ddr_hertz = 0, calculated_ddr_hertz = 0;
            uint64_t orig_ddr_hertz = ddr_hertz;
            static const unsigned _en[] = {1, 2, 3, 4, 6, 8, 12};

            ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

            while (best_error == ddr_hertz)
            {
                for (en_idx=0; en_idx < sizeof(_en)/sizeof(int); ++en_idx) {
                    save_en_idx = en_idx;
                    clkf = ((ddr_hertz) * _en[save_en_idx]);
                    clkf = divide_nint(clkf, 50 * 1000000);
                    pll_MHz = clkf * 50;
                    calculated_ddr_hertz = (pll_MHz * 1000000) / _en[save_en_idx];
                    error = ddr_hertz - calculated_ddr_hertz;

#if defined(__U_BOOT__)
                    ddr_print("en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %9lu, error: %10d, best_error: %8d\n",
                              _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error, best_error);
#else
                    ddr_print("en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %9llu, error: %10lld, best_error: %8d\n",
                              _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error, best_error);
#endif


                    if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz)) continue;
                    if ((clkf < min_clkf) || (clkf > max_clkf)) continue;
                    if (_abs(error) > _abs(best_error)) continue;

                    if ((_abs(error) < _abs(best_error)) || (pll_MHz > highest_pll_MHz)) {
                        highest_pll_MHz = pll_MHz;
                        best_pll_MHz = pll_MHz;
                        best_calculated_ddr_hertz = calculated_ddr_hertz;
                        best_error = error;
                        best_clkf = clkf;
                        best_en_idx = save_en_idx;
                    }
                }

#if defined(__U_BOOT__)
                ddr_print("en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %10d <==\n",
                          _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#else
                ddr_print("en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %10lld <==\n",
                          _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#endif

                /* Try lowering the frequency if we can't get a working configuration */
                if (best_error == ddr_hertz) {
                    if (ddr_hertz < orig_ddr_hertz - 10000000)
                        break;
                    ddr_hertz -= 1000000;
                    best_error = ddr_hertz;
                }
            }
            if (best_error == ddr_hertz) {
                error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                return(-1);
            }

            ddr_pll_ctl.cn63xx.ddr_ps_en = best_en_idx;
            ddr_pll_ctl.cn63xx.dfm_ps_en = best_en_idx; /* Set DFM to the DDR speed. */
            ddr_pll_ctl.cn63xx.clkf = best_clkf;

            ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);
        }

        /*
         * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 4. Wait 3 usec.
         */

        cvmx_wait_usec(3);          /* Wait 3 us */

        /*
         * 5. Write LMC0_DDR_PLL_CTL[RESET_N] = 1 without changing any
         *    other LMC0_DDR_PLL_CTL values.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn63xx.reset_n = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 7. Wait 25 usec.
         */

        cvmx_wait_usec(25);          /* Wait 25 us */

        /*
         * 4.8.2 LMC CK Initialization
         *
         * DDR PLL initialization must be completed prior to starting
         * LMC CK initialization.
         *
         * 1. Without changing any other LMC0_DDR_PLL_CTL values,
         *    write LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 1 and
         *    LMC0_DDR_PLL_CTL [DDR_PS_EN] with the appropriate value
         *    to get the desired LMC CK speed. Section Section 4.13
         *    discusses CLKF and DDR_PS_EN programmings.
         *
         *    The LMC0_DDR_PLL_CTL[DDR_PS_EN] must not change after
         *    this point without restarting this LMC CK initialization
         *    sequence.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn63xx.ddr_div_reset = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);


        /*
         * 2. Without changing any other LMC0_COMP_CTL2 values, write
         *    LMC0_COMP_CTL2[CK_CTL] to the desired DDR_CK_*_P/
         *    DDR_DIMM*_CS*_L/DDR_DIMM*_ODT_* drive strength.
         */

        {
            cvmx_lmcx_comp_ctl2_t comp_ctl2;
	    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;

            comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));

	    comp_ctl2.s.dqx_ctl  = (custom_lmc_config->dqx_ctl == 0) ? 4 : custom_lmc_config->dqx_ctl; /* Default 4=34.3 ohm */
	    comp_ctl2.s.ck_ctl   = (custom_lmc_config->ck_ctl  == 0) ? 4 : custom_lmc_config->ck_ctl;  /* Default 4=34.3 ohm */
	    comp_ctl2.s.cmd_ctl  = (custom_lmc_config->cmd_ctl == 0) ? 4 : custom_lmc_config->cmd_ctl; /* Default 4=34.3 ohm */

            comp_ctl2.cn61xx.rodt_ctl           = 0x4; /* 60 ohm */

            if ((s = lookup_env_parameter("ddr_clk_ctl")) != NULL) {
                comp_ctl2.s.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_ck_ctl")) != NULL) {
                comp_ctl2.s.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_cmd_ctl")) != NULL) {
                comp_ctl2.s.cmd_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dqx_ctl")) != NULL) {
                comp_ctl2.s.dqx_ctl  = simple_strtoul(s, NULL, 0);
            }

            ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), comp_ctl2.u64);
        }

        /*
         * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 4. Wait 200 ns.
         */

        cvmx_wait_usec(1);          /* Wait 200 ns minimum */

        /*
         * 5. Without changing any other LMC0_DDR_PLL_CTL values,
         *    write LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
         */

        ddr_pll_ctl.cn63xx.ddr_div_reset = 0;

        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 7. Wait 200 ns.
         */

        cvmx_wait_usec(1);      /* Wait 200 ns minimum */

        /*
         * 4.8.3 LMC DRESET Initialization
         *
         * Both DDR PLL and LMC CK initializations must be completed
         * prior to starting this LMC DRESET initialization.
         *
         * 1. If not done already, write LMC0_DLL_CTL2 to its reset
         *    value, including LMC0_DLL_CTL2[DRESET] = 1.
         */

        /* Already done. */

        /*
         * 2. Without changing any other LMC0_DLL_CTL2 fields, write
         *    LMC0_DLL_CTL2 [DLL_BRINGUP] = 1.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn63xx.dll_bringup = 1;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 3. Read LMC0_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 4. Wait for 10 LMC CK cycles.
         */

        cvmx_wait_usec(1);

        /*
         * 5. Without changing any other fields in LMC0_DLL_CTL2, write
         *
         *    LMC0_DLL_CTL2[QUAD_DLL_ENA] = 1.
         *
         *    LMC0_DLL_CTL2[QUAD_DLL_ENA] must not change after this
         *    point without restarting the LMC DRESET initialization
         *    sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn63xx.quad_dll_ena = 1;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 6. Read LMC0_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 7. Wait 10 usec.
         */

        cvmx_wait_usec(10);

        /*
         * 8. Without changing any other fields in LMC0_DLL_CTL2, write
         *
         *    LMC0_DLL_CTL2[DLL_BRINGUP] = 0.
         *
         *    LMC0_DLL_CTL2[DLL_BRINGUP] must not change after this
         *    point without restarting the LMC DRESET initialization
         *    sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn63xx.dll_bringup = 0;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 9. Read LMC0_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 10. Without changing any other fields in LMC0_DLL_CTL2, write
         *
         *     LMC0_DLL_CTL2[DRESET] = 0.
         *
         *     LMC0_DLL_CTL2[DRESET] must not change after this point
         *     without restarting the LMC DRESET initialization
         *     sequence.
         *
         *     After completing LMC DRESET initialization, all LMC
         *     CSRs may be accessed. Prior to completing LMC DRESET
         *     initialization, only LMC0_DDR_PLL_CTL, LMC0_DLL_CTL2,
         *     LMC0_RESET_CTL, and LMC0_COMP_CTL2 LMC CSRs can be
         *     accessed.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn63xx.dreset = 0;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 4.8.4 LMC RESET Initialization
         *
         * The purpose of this step is to assert/deassert the RESET#
         * pin at the DDR3 parts.
         *
         * It may be appropriate to skip this step if the DDR3 DRAM
         * parts are in self refresh and are currently preserving
         * their contents. (Software can determine this via
         * LMC0_RESET_CTL[DDR3PSV] in some circumstances.)  The
         * remainder of this section assumes that the DRAM contents
         * need not be preserved.
         *
         * The remainder of this section assumes that the CN63XX
         * DDR_RESET_L pin is attached to the RESET# pin of the
         * attached DDR3 parts, as will be appropriate in many
         * systems.
         *
         * (In other systems, such as ones that can preserve DDR3 part
         * contents while CN63XX is powered down, it will not be
         * appropriate to directly attach the CN63XX DDR_RESET_L pin
         * to DRESET# of the DDR3 parts, and this section may not
         * apply.)
         *
         * DRAM Controller (LMC): LMC Initialization Sequence
         *
         * 1. If not done already, assert DDR_RESET_L pin by writing
         *    LMC0_RESET_CTL[DDR3RST] = 0 without modifying any other
         *    LMC0_RESET_CTL fields.
         */

        lmc_reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
        if ( !ddr_memory_preserved()) {
            /*
             * 2. Read LMC0_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));

            /*
             * 3. Wait until RESET# assertion time requirement from JEDEC
             *    DDR3 specification is satisfied (200 usec during a
             *    power-on ramp, 100ns when power is already stable).
             */

            cvmx_wait_usec(200);

            /*
             * 4. Deassert DDR_RESET_L pin by writing
             *    LMC0_RESET_CTL[DDR3RST] = 1 without modifying any other
             *    LMC0_RESET_CTL fields.
             */

            lmc_reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
            lmc_reset_ctl.cn63xx.ddr3rst = 1;
            ddr_print("LMC%d De-asserting DDR_RESET_L\n", ddr_interface_num);
            ddr_config_write_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num), lmc_reset_ctl.u64);

            /*
             * 5. Read LMC0_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));

            /* 6. Wait a minimum of 500 us */

            cvmx_wait_usec(500);
        }

        /*
         * (Subsequent RESET# deassertion to CKE assertion requirement
         * can be satisfied during LMC initialization via
         * LMC0_TIMING_PARAMS0[TCKEON] timing.)
         */

        if (!octeon_is_cpuid(OCTEON_CN63XX_PASS1_X)) {
            cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
            ddr_dll_ctl3.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL3(ddr_interface_num));
            ddr_dll_ctl3.cn63xx.fine_tune_mode = 1; /* Enable Fine Tune Mode */
            ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);
        }
    }

    if (octeon_is_cpuid(OCTEON_CN68XX)) {
        cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;

        /*
         * 4.8 LMC Initialization Sequence
         * There are ten parts to the LMC initialization procedure:
         *
         * 1. DDR PLL initialization
         *
         * 2. LMC CK global initialization
         *
         * 3. LMC interface enable initialization
         *
         * 4. LMC DRESET initialization
         *
         * 5. LMC CK local initialization
         *
         * 6. LMC RESET initialization
         *
         * 7. Early LMC initialization
         *
         * 8. LMC write leveling
         *
         * 9. LMC read leveling
         *
         * 10.Final LMC initialization
         *
         * CN68XX supports two modes:
         *
         * . two-LMC mode: both LMCs 2/3 must not be enabled
         * (LMC2/3_DLL_CTL2[DRESET] must be set to 1 and
         * LMC2/3_DLL_CTL2[INTF_EN] must be set to 0) and both LMCs 0/1 must be
         * enabled).
         *
         * . four-LMC mode: all four LMCs 0-3 must be enabled.
         * Steps 4 and 6-10 should each be performed for each enabled LMC (either twice or
         * four times). Steps 1-3 and 5 are more global in nature and each must be executed
         * exactly once (not once per LMC) each time the DDR PLL changes or is first brought
         * up. Steps 1-3 and 5 need not be performed if the DDR PLL is stable.
         *
         * Generally, the steps are performed in order. The exception is that the CK local
         * initialization (step 5) must be performed after some DRESET initializations (step 4)
         * and before other DRESET initializations when the DDR PLL is brought up or
         *
         *
         * DRAM Controller (LMC): LMC Initialization Sequence
         *
         * changed. (The CK local initialization uses information from some LMCs to bring up
         * the other local CKs.) The following text describes these ordering requirements in
         * more detail.
         *
         * Following any chip reset, the DDR PLL must be brought up, and all ten steps should
         * be executed. Subsequently, it is possible to execute only steps 4 and 6.10, or to
         * execute only steps 8.10.
         *
         * The remainder of this section covers these initialization steps in sequence.
         */


        /* 4.8.1 DDR PLL Initialization
         * =============================================================================
         * DDR PLL initialization (Step 1) must be performed for each chip reset and whenever
         * the DDR clock speed changes. This step needs to be performed only once, not once per
         * LMC.
         *
         * Perform the following eight substeps to initialize the DDR PLL:
         *
         * 1. If not done already, write all fields in LMC(0..3)_DDR_PLL_CTL and
         * LMC(0..3)_DLL_CTL2 to their reset values, including:
         * . LMC(0..3)_DDR_PLL_CTL[DDR_DIV_RESET, DFM_DIV_RESET] = 1
         * . LMC(0..3)_DLL_CTL2[DRESET] = 1
         * This substep is not necessary after a chip reset.
         */


        /* 2. If the current DRAM contents are not preserved (see
         * LMC(0..3)_RESET_CTL[DDR3PSV]), this is also an appropriate time to assert
         * the RESET# pin of the DDR3 DRAM parts. If desired, write
         * LMC(0..3)_RESET_CTL[DDR3RST] = 0 without modifying any other
         * LMC(0..3)_RESET_CTL fields to assert the DDR_RESET_L pin. No action is
         * required here to assert DDR_RESET_L following a chip reset. Refer to Section
         * 4.8.6. Do this for all enabled LMCs.
         *
         * 3. Without changing any other LMC0_DDR_PLL_CTL values, write
         * LMC0_DDR_PLL_CTL[CLKF] with a value that gives a desired DDR PLL speed.
         * The LMC0_DDR_PLL_CTL[CLKF] value should be selected in conjunction with
         * the post-scalar divider values for LMC (LMC0_DDR_PLL_CTL[DDR_PS_EN]) so
         * that the desired LMC CK speeds are is produced (all enabled LMCs must run the
         * same speed). Section 4.13 describes LMC0_DDR_PLL_CTL[CLKF] and
         * LMC0_DDR_PLL_CTL[DDR_PS_EN] programmings that produce the desired
         * LMC CK speed. Section 4.8.2 describes LMC CK initialization, which can be done
         * separately from the DDR PLL initialization described in this section.
         * The LMC0_DDR_PLL_CTL[CLKF] value must not change after this point
         *
         * without restarting this SDRAM PLL initialization sequence.
         *
         * 4. Read LMC0_DDR_PLL_CTL and wait for the result.
         *
         * 5. Wait a minimum of 3 us.
         *
         * 6. Write LMC0_DDR_PLL_CTL[RESET_N] = 1 without changing any other
         * LMC0_DDR_PLL_CTL values.
         *
         * 7. Read LMC0_DDR_PLL_CTL and wait for the result.
         *
         * 8. Wait a minimum of 25 us.
         *
         *
         * 4.8.2 LMC CK Global Initialization
         * =============================================================================
         * DDR PLL initialization must be completed prior to starting LMC CK global
         * initialization.
         * LMC CK global initialization (Step 2) must be performed for each chip reset and
         * whenever the DDR clock speed changes. This step needs to be performed only once
         * (for LMC0), not once per LMC.
         *
         *
         * Perform the following seven substeps to initialize the LMC CK:
         *
         *
         * 1. Without changing any other LMC0_DDR_PLL_CTL values, write
         * LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 1 and LMC0_DDR_PLL_CTL
         * [DDR_PS_EN] with the appropriate value to get the desired LMC CK speed. Section
         * Section 4.13 discusses CLKF and DDR_PS_EN programmings.
         * The LMC0_DDR_PLL_CTL[DDR_PS_EN] must not change after this point
         * without restarting this LMC CK global initialization sequence.
         *
         * 2. Without changing any other LMC0_COMP_CTL2 values, write
         * LMC0_COMP_CTL2[CK_CTL] to the desired DDR_CK_*_P/
         * DDR_DIMM*_CS*_L/DDR_DIMM*_ODT_* drive strength.
         *
         * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
         *
         * 4. Wait a minimum of 200 ns.
         *
         * 5. Without changing any other LMC0_DDR_PLL_CTL values, write
         * LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
         *
         * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
         *
         * 7. Wait a minimum of 200 ns.
         *
         * 4.8.3 LMC Interface-Enable Initialization
         * =============================================================================
         * LMC interface-enable initialization (Step 3) must be performed after Step 2 for each
         * chip reset and whenever the DDR clock speed changes. This step needs to be
         * performed only once, not once per LMC.
         *
         * Perform the following three substeps for the LMC interface-enable initialization:
         *
         * 1. Without changing any other LMC2_DLL_CTL2 fields (LMC(0..3)_DLL_CTL2
         * should be at their reset values after Step 1), write LMC2_DLL_CTL2[INTF_EN]
         * = 1 if four-LMC mode is desired.
         *
         * 2. Without changing any other LMC3_DLL_CTL2 fields, write
         * LMC3_DLL_CTL2[INTF_EN] = 1 if four-LMC mode is desired.
         *
         * 3. Read LMC2_DLL_CTL2 and wait for the result.
         * The LMC2_DLL_CTL2[INTF_EN] and LMC3_DLL_CTL2[INTF_EN] values
         * should not be changed by software from this point.
         *
         * No action is required in this substep 3 if four-LMC mode is desired.
         *
         * 4.8.4 LMC DRESET Initialization
         * =============================================================================
         * All of the DDR PLL, LMC global CK, and LMC interface enable initializations must
         * be completed prior to starting this LMC DRESET initialization (Step 4).
         * This LMC DRESET step is done for all enabled LMCs.
         *
         * There are special constraints on the ordering of DRESET initialization (Steps 4) and
         * CK local initialization (Step 5) whenever CK local initialization must be executed.
         * CK local initialization must be executed whenever the DDR PLL is being brought up
         * (for each chip reset and whenever the DDR clock speed changes).
         *
         * When Step 5 must be executed in the two-LMC mode case:
         *
         * o LMC0 DRESET initialization must occur before Step 5.
         *
         * o LMC1 DRESET initialization must occur after Step 5.
         *
         * When Step 5 must be executed in the four-LMC mode case:
         *
         * o LMC2 and LMC3 DRESET initialization must occur before Step 5.
         *
         * o LMC0 and LMC1 DRESET initialization must occur after Step 5.
         *
         * The remainder of this section describes the sequence for LMCn.
         *
         * 1. If not done already, write LMCn_DLL_CTL2 to its reset value (except without
         * changing the LMC(0..3)_DLL_CTL2[INTF_EN] value from that set in the prior
         * Step 3), including LMCn_DLL_CTL2[DRESET] = 1.
         *
         * 2. Without changing any other LMCn_DLL_CTL2 fields, write LMCn_DLL_CTL2
         * [DLL_BRINGUP] = 1.
         *
         * 3. Read LMCn_DLL_CTL2 and wait for the result.
         *
         * 4. Wait for a minimum of 10 LMC CK cycles.
         *
         * 5. Without changing any other fields in LMCn_DLL_CTL2, write
         * LMCn_DLL_CTL2[QUAD_DLL_ENA] = 1.
         * LMCn_DLL_CTL2[QUAD_DLL_ENA] must not change after this point without
         * restarting the LMCn DRESET initialization sequence.
         *
         * 6. Read LMCn_DLL_CTL2 and wait for the result.
         *
         * 7. Wait a minimum of 10 us.
         *
         * 8. Without changing any other fields in LMCn_DLL_CTL2, write
         * LMCn_DLL_CTL2[DLL_BRINGUP] = 0.
         * LMCn_DLL_CTL2[DLL_BRINGUP] must not change after this point without
         * restarting the LMCn DRESET initialization sequence.
         *
         *
         * 9. Read LMCn_DLL_CTL2 and wait for the result.
         *
         * 10. Without changing any other fields in LMCn_DLL_CTL2, write
         * LMCn_DLL_CTL2[DRESET] = 0.
         * LMCn_DLL_CTL2[DRESET] must not change after this point without restarting
         * the LMCn DRESET initialization sequence.
         *
         *
         * After completing LMCn DRESET initialization, all LMC CSRs may be accessed.
         * Prior to completing LMC DRESET initialization, only LMCn_DDR_PLL_CTL,
         * LMCn_DLL_CTL2, LMCn_RESET_CTL, and LMCn_COMP_CTL2 LMC CSRs can
         * be accessed.
         *
         * 4.8.5 LMC CK Local Initialization
         * =============================================================================
         * All of DDR PLL, LMC global CK, and LMC interface-enable initializations must be
         * completed prior to starting this LMC CK local initialization (Step 5).
         *
         * LMC CK Local initialization must be performed for each chip reset and whenever the
         * DDR clock speed changes. This step needs to be performed only once, not once per
         * LMC.
         *
         * DRAM Controller (LMC): LMC Initialization Sequence
         *
         * There are special constraints on the ordering of DRESET initialization (Steps 4) and
         * CK local initialization (Step 5) whenever CK local initialization must be executed.
         * CK local initialization must be executed whenever the DDR PLL is being brought up
         * (for each chip reset and whenever the DDR clock speed changes).
         *
         * When Step 5 must be executed in the two-LMC mode case:
         *
         * . LMC0 DRESET initialization must occur before Step 5
         *
         * . LMC1 DRESET initialization must occur after Step 5
         * When Step 5 must be executed in the four-LMC mode case:
         *
         * . LMC2 and LMC3 DRESET initialization must occur before Step 5
         *
         * . LMC0 and LMC1 DRESET initialization must occur after Step 5
         * LMC CK local initialization is different depending on whether two-LMC or four-LMC
         * modes are desired.
         *
         * 4.8.5.1 LMC CK Local Initialization for Two-LMC Mode
         *
         * 1. Write LMC0_DLL_CTL3 to its reset value. (Note that
         * LMC0_DLL_CTL3[DLL_90_BYTE_SEL] = 0x2 . 0x8 should also work.)
         *
         * 2. Read LMC0_DLL_CTL3 and wait for the result.
         *
         * 3. Without changing any other fields in LMC0_DLL_CTL3, write
         * LMC0_DLL_CTL3[DCLK90_FWD] = 1.
         * Writing LMC0_DLL_CTL3[DCLK90_FWD] = 1 causes clock-delay information to
         * be forwarded from LMC0 to LMC1.
         *
         * 4. Read LMC0_DLL_CTL3 and wait for the result.
         * 4.8.5.2 LMC CK Local Initialization for Four-LMC Mode
         *
         * 1. Write LMC2_DLL_CTL3 to its reset value except
         * LMC2_DLL_CTL3[DLL90_BYTE_SEL] = 0x7.
         *
         * 2. Write LMC3_DLL_CTL3 to its reset value except
         * LMC3_DLL_CTL3[DLL90_BYTE_SEL] = 0x2.
         *
         * 3. Read LMC3_DLL_CTL3 and wait for the result.
         *
         * 4. Without changing any other fields in LMC2_DLL_CTL3, write
         * LMC2_DLL_CTL3[DCLK90_FWD] = 1 and
         * LMC2_DLL_CTL3[DCLK90_RECAL_DIS] = 1.
         * Writing LMC2_DLL_CTL3[DCLK90_FWD] = 1 causes LMC 2 to forward clock-
         * delay information to LMC0. Setting LMC2_DLL_CTL3[DCLK90_RECAL_DIS]
         * to 1 prevents LMC2 from periodically recalibrating this delay information.
         *
         * 5. Without changing any other fields in LMC3_DLL_CTL3, write
         * LMC3_DLL_CTL3[DCLK90_FWD] = 1 and
         * LMC3_DLL_CTL3[DCLK90_RECAL_DIS] = 1.
         * Writing LMC3_DLL_CTL3[DCLK90_FWD] = 1 causes LMC3 to forward clock-
         * delay information to LMC1. Setting LMC3_DLL_CTL3[DCLK90_RECAL_DIS]
         * to 1 prevents LMC3 from periodically recalibrating this delay information.
         *
         * 6. Read LMC3_DLL_CTL3 and wait for the result.
         * Cavium Inc. Proprietary and Confidential DO NOT COPY
         *
         *
         * 4.8.6 LMC RESET Initialization
         * =============================================================================
         * The purpose of this step is to assert/deassert the RESET# pin at the DDR3 parts.
         *
         * This LMC RESET step is done for all enabled LMCs.
         *
         * It may be appropriate to skip this step if the DDR3 DRAM parts are in self refresh
         * and are currently preserving their contents. (Software can determine this via
         * LMC(0..3)_RESET_CTL[DDR3PSV] in some circumstances.) The remainder of this
         * section assumes that the DRAM contents need not be preserved.
         *
         * The remainder of this section assumes that the CN68XX DDRn_RESET_L pin is
         * attached to the RESET# pin of the attached DDR3 parts, as will be appropriate in
         * many systems.
         *
         * (In other systems, such as ones that can preserve DDR3 part contents while CN68XX
         * is powered down, it will not be appropriate to directly attach the CN68XX
         * DDRn_RESET_L pin to DRESET# of the DDR3 parts, and this section may not
         * apply.)
         *
         * The remainder of this section describes the sequence for LMCn.
         * Perform the following six substeps for LMC reset initialization:
         *
         * 1. If not done already, assert DDRn_RESET_L pin by writing
         * LMCn_RESET_CTL[DDR3RST] = 0 without modifying any other
         * LMCn_RESET_CTL fields.
         *
         * 2. Read LMCn_RESET_CTL and wait for the result.
         *
         * 3. Wait until RESET# assertion-time requirement from JEDEC DDR3 specification
         * is satisfied (200 us during a power-on ramp, 100ns when power is already stable).
         *
         * 4. Deassert DDRn_RESET_L pin by writing LMCn_RESET_CTL[DDR3RST] = 1
         * without modifying any other LMCn_RESET_CTL fields.
         *
         * 5. Read LMCn_RESET_CTL and wait for the result.
         *
         * 6. Wait a minimum of 500us. This guarantees the necessary T = 500us delay
         * between DDRn_RESET_L deassertion and DDRn_DIMM*_CKE* assertion.
         *
         * 4.8.7 Early LMC Initialization
         * =============================================================================
         * All of DDR PLL and LMC global and local CK initializations must be completed prior
         * to starting this LMC initialization sequence. LMC DRESET initialization must also
         * have been completed for LMCn prior to starting this LMC initialization sequence.
         *
         * This early LMC initialization step is done for all enabled LMCs.
         * The remainder of this section describes the sequence for LMCn.
         * Perform the following five substeps for early LMC initialization:
         *
         * 1. Software must ensure there are no pending DRAM transactions.
         *
         * 2. Write LMCn_CONFIG, LMCn_CONTROL, LMCn_TIMING_PARAMS0,
         * LMCn_TIMING_PARAMS1, LMCn_MODEREG_PARAMS0,
         * LMCn_MODEREG_PARAMS1, LMCn_DUAL_MEMCFG, LMCn_NXM,
         * LMCn_WODT_MASK, LMCn_RODT_MASK, LMCn_COMP_CTL2,
         * LMCn_PHY_CTL, LMCn_DIMM0/1_PARAMS, and LMCn_DIMM_CTL with
         * appropriate values. All sections in this chapter may be used to derive proper
         * register settings.
         *
         * 3. Without changing any other fields in LMCn_CONFIG, write
         * LMCn_CONFIG[RANKMASK,INIT_STATUS,SEQUENCE] while writing
         *
         *
         * DRAM Controller (LMC): LMC Initialization Sequence
         *
         * LMCn_CONFIG[INIT_START] = 1, all with a single CSR write operation. Both
         * LMCn_CONFIG[RANKMASK,INIT_STATUS] bits should be set to the same
         * mask value indicating the ranks that will participate in the sequence.
         *
         * The LMCn_CONFIG[SEQUENCE] value should select power-up/init or self-
         * refresh exit, depending on whether the DRAM parts are in self-refresh and
         * whether their contents should be preserved. While LMC performs these
         * sequences, it will not perform any other DDR3 transactions.
         *
         * If power-up/init is selected immediately following a DRESET assertion for the
         * LMC, LMC executes the sequence described in the Reset and Initialization
         * Procedure section of the JEDEC DDR3 specification. This includes activating
         * CKE, writing all four DDR3 mode registers on all selected ranks, and issuing the
         * required ZQCL command. The LMCn_CONFIG[RANKMASK] value should
         * select all ranks with attached DRAM in this case. If
         * LMCn_CONTROL[RDIMM_ENA] = 1, LMCn writes the JEDEC standard
         * SSTE32882 control words selected by LMCn_DIMM_CTL[DIMM*_WMASK]
         * between DDRn_DIMM*_CKE* signal assertion and the first DDR3 mode register
         * write operation. LMCn_DIMM_CTL[DIMM*_WMASK] should be cleared to 0 if
         * the corresponding DIMM is not present.
         *
         * If self-refresh exit is selected, LMCn executes the required SRX command
         * followed by a refresh and ZQ calibration. Section 4.5 describes behavior of a REF
         *
         * + ZQCS. LMC does not write the DDR3 mode registers as part of this sequence,
         * and the mode register parameters must match at self-refresh entry and exit
         * times. The self-refresh exit sequence does not use LMCn_CONFIG
         * [RANKMASK]. If the self-refresh exit is preceded by a self-refresh exit
         * SEQUENCE without an intervening DRESET sequence, then LMCn_CONFIG
         * [INIT_STATUS] will already indicate the ranks that should participate, and
         * software need not assert any LMCn_CONFIG[INIT_STATUS] bits. Otherwise,
         * the LMCn_CONFIG CSR write that initiates the self-refresh exit must also set
         * LMCn_CONFIG[INIT_STATUS] bits for all ranks with attached DRAM.
         *
         * 4. Read LMCn_CONFIG and wait for the result.
         *
         * 5. Wait until the sequence is complete. There are two delays, depending on whether
         * registered or unregistered DIMMs are used (specified in
         * LMCn_CONTROL[RDIMM_ENA]). The worst case is with registered DIMMs,
         * where the power-up/init or self-refresh exit sequence may take as many as
         * (num_rank x 1 million) CK cycles, where num_rank is the number of bits set in
         * LMCn_CONFIG[RANKMASK]. With unregistered DIMMs, the delay is
         * (num_rank x 32K) CK cycles.
         */

        if (!ddr_clock_initialized(0, 0)) { /* Do this once */
            int i;
            cvmx_lmcx_reset_ctl_t reset_ctl;
            for (i=3; i>=0; --i) {
                if ((ddr_interface_mask & (1 << i)) == 0)
                    continue;

                if ( !ddr_memory_preserved()) {
                    reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(i));
                    /* 1. Assert DDRn_RESET_L by writing LMCn_RESET_CTL[ddr3rst]=0 */
                    reset_ctl.cn68xx.ddr3rst = 0; /* Reset asserted */
                    ddr_print("LMC%d Asserting DDR_RESET_L\n", i);
                    ddr_config_write_csr(CVMX_LMCX_RESET_CTL(i), reset_ctl.u64);
                    /* 2. Read LMCn_RESET_CTL and wait for the result */
                    cvmx_read_csr(CVMX_LMCX_RESET_CTL(i));
                }
            }

            if (ddr_interface_mask & (1 << 2)) { /* four-LMC Mode */
		/* 0x800118008b0001c8 LMC3_DLL_CTL2 0x000000000000a032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(3), 0x000000000000a032ULL);
		/* 0x800118008a0001c8 LMC2_DLL_CTL2 0x000000000000a032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(2), 0x000000000000a032ULL);
            }

            {
                unsigned en_idx, save_en_idx, best_en_idx=0;
                uint64_t clkf, min_clkf = 32, max_clkf = 64;
                uint64_t best_clkf=0;
                uint64_t best_pll_MHz = 0, pll_MHz, highest_pll_MHz = 0;
                uint64_t min_pll_MHz = 1600, max_pll_MHz = 3000;
                int64_t  error = ddr_hertz, best_error = ddr_hertz; /* Init to max error */
                uint64_t best_calculated_ddr_hertz = 0, calculated_ddr_hertz = 0;
                uint64_t orig_ddr_hertz = ddr_hertz;
                static const unsigned _en[] = {1, 2, 3, 4, 6, 8, 12};

                ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

                while (best_error == ddr_hertz)
                {
                    for (en_idx=0; en_idx < sizeof(_en)/sizeof(int); ++en_idx) {
                        save_en_idx = en_idx;
                        clkf = ((ddr_hertz) * _en[save_en_idx]);
                        clkf = divide_nint(clkf, 50 * 1000000);
                        pll_MHz = clkf * 50;
                        calculated_ddr_hertz = (pll_MHz * 1000000) / _en[save_en_idx];
                        error = ddr_hertz - calculated_ddr_hertz;

#if defined(__U_BOOT__)
                        ddr_print("en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %9lu, error: %10d, best_error: %8d\n",
                                  _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error, best_error);
#else
                        ddr_print("en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %9llu, error: %10lld, best_error: %8d\n",
                                  _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error, best_error);
#endif


                        if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz)) continue;
                        if ((clkf < min_clkf) || (clkf > max_clkf)) continue;
                        if (_abs(error) > _abs(best_error)) continue;

                        if ((_abs(error) < _abs(best_error)) || (pll_MHz > highest_pll_MHz)) {
                            highest_pll_MHz = pll_MHz;
                            best_pll_MHz = pll_MHz;
                            best_calculated_ddr_hertz = calculated_ddr_hertz;
                            best_error = error;
                            best_clkf = clkf;
                            best_en_idx = save_en_idx;
                        }
                    }

#if defined(__U_BOOT__)
                    ddr_print("en: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %10d <==\n",
                              _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#else
                    ddr_print("en: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %10lld <==\n",
                              _en[best_en_idx], best_clkf, best_pll_MHz, best_calculated_ddr_hertz, best_error);
#endif

                    /* Try lowering the frequency if we can't get a working configuration */
                    if (best_error == ddr_hertz) {
                        if (ddr_hertz < orig_ddr_hertz - 10000000)
                            break;
                        ddr_hertz -= 1000000;
                        best_error = ddr_hertz;
                    }
                }
                if (best_error == ddr_hertz) {
                    error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                    return(-1);
                }

                ddr_pll_ctl.cn63xx.ddr_ps_en = best_en_idx;
                ddr_pll_ctl.cn63xx.dfm_ps_en = best_en_idx; /* Set DFM to the DDR speed. */
                ddr_pll_ctl.cn63xx.clkf = best_clkf;

                ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);
            }

            /*
             * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

            /*
             * 4. Wait 3 usec.
             */

            cvmx_wait_usec(3);          /* Wait 3 us */

            /*
             * 5. Write LMC0_DDR_PLL_CTL[RESET_N] = 1 without changing any
             *    other LMC0_DDR_PLL_CTL values.
             */

            ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));
            ddr_pll_ctl.cn63xx.reset_n = 1;
            ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);

            /*
             * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

            /*
             * 7. Wait 25 usec.
             */

            cvmx_wait_usec(25);          /* Wait 25 us */


            /*
             * 4.8.2 LMC CK Initialization
             *
             * DDR PLL initialization must be completed prior to starting
             * LMC CK initialization.
             *
             * 1. Without changing any other LMC0_DDR_PLL_CTL values,
             *    write LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 1 and
             *    LMC0_DDR_PLL_CTL [DDR_PS_EN] with the appropriate value
             *    to get the desired LMC CK speed. Section Section 4.13
             *    discusses CLKF and DDR_PS_EN programmings.
             *
             *    The LMC0_DDR_PLL_CTL[DDR_PS_EN] must not change after
             *    this point without restarting this LMC CK initialization
             *    sequence.
             */

            ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));
            ddr_pll_ctl.cn63xx.ddr_div_reset = 1;
            ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);

            /*
             * 3. Read LMC0_DDR_PLL_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

            /*
             * 4. Wait 200 ns.
             */

            cvmx_wait_usec(1);          /* Wait 200 ns minimum */

            /*
             * 5. Without changing any other LMC0_DDR_PLL_CTL values,
             *    write LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
             */

            ddr_pll_ctl.cn63xx.ddr_div_reset = 0;

            ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);

            /*
             * 6. Read LMC0_DDR_PLL_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

            /*
             * 7. Wait 200 ns.
             */

            cvmx_wait_usec(1);      /* Wait 200 ns minimum */


            if (ddr_interface_mask & (1 << 2)) { /* four-LMC Mode */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(3),	0x000000000000e032ULL); /* 0x800118008b0001c8 LMC3_DLL_CTL2 0x000000000000e032 */
                cvmx_wait_io(20*4); /*add a small delay here( 20 dclks) */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(3),	0x000000000000f032ULL); /* 0x800118008b0001c8 LMC3_DLL_CTL2 0x000000000000f032 */
                cvmx_wait(                                      0x0000000000001cd6ULL); /* Delay for 7382 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(3),	0x000000000000b032ULL); /* 0x800118008b0001c8 LMC3_DLL_CTL2 0x000000000000b032 */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(3),	0x0000000000009032ULL); /* 0x800118008b0001c8 LMC3_DLL_CTL2 0x0000000000009032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(2),	0x000000000000e032ULL); /* 0x800118008a0001c8 LMC2_DLL_CTL2 0x000000000000e032 */
                cvmx_wait_io(20*4); /*add a small delay here( 20 dclks) */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(2),	0x000000000000f032ULL); /* 0x800118008a0001c8 LMC2_DLL_CTL2 0x000000000000f032 */
                cvmx_wait(                                      0x0000000000001cd7ULL); /* Delay for 7383 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(2),	0x000000000000b032ULL); /* 0x800118008a0001c8 LMC2_DLL_CTL2 0x000000000000b032 */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(2),	0x0000000000009032ULL); /* 0x800118008a0001c8 LMC2_DLL_CTL2 0x0000000000009032 */
                cvmx_wait_io(20*4); /*add a small delay here( 20 dclks) */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(2),	0x000000000001c000ULL); /* 0x800118008a000218 LMC2_DLL_CTL3 0x000000000001c000 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(3),	0x0000000000008000ULL); /* 0x800118008b000218 LMC3_DLL_CTL3 0x0000000000008000 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(2),	0x000001400001c000ULL); /* 0x800118008a000218 LMC2_DLL_CTL3 0x000001400001c000 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(3),	0x0000014000008000ULL); /* 0x800118008b000218 LMC3_DLL_CTL3 0x0000014000008000 */
                cvmx_wait(                                      0x00000000000002bcULL); /* Delay for 700 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000e032ULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000e032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000f032ULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000f032 */
                cvmx_wait(                                      0x0000000000001cd7ULL); /* Delay for 7383 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000b032ULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000b032 */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x0000000000009032ULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x0000000000009032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(0),	0xcb6c10000d490000ULL); /* 0x8001180088000218 LMC0_DLL_CTL3 0xcb6c10000d49aadc */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(0),	0x0000010000018000ULL); /* 0x8001180088000218 LMC0_DLL_CTL3 0x000001000009aadc */
                cvmx_wait(                                      0x00000000000002bcULL); /* Delay for 700 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x967c330b3857e032ULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x967c330b3857e032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x000000000000f032ULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x000000000000f032 */
                cvmx_wait(                                      0x0000000000001cd7ULL); /* Delay for 7383 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x000000000000b032ULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x000000000000b032 */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x0000000000009032ULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x0000000000009032 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(1),	0x7e215c001cf04000ULL); /* 0x8001180089000218 LMC1_DLL_CTL3 0x7e215c001cf866aa */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(1),	0x0000010000004000ULL); /* 0x8001180089000218 LMC1_DLL_CTL3 0x00000100000866aa */
                cvmx_wait(                                      0x00000000000002bcULL); /* Delay for 700 cycles */
                {
                    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
                    ddr_dll_ctl3.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL3(2));
                    ddr_dll_ctl3.cn68xx.dclk90_recal_dis = 0;
                    ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(2),	ddr_dll_ctl3.u64);

                    ddr_dll_ctl3.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL3(3));
                    ddr_dll_ctl3.cn68xx.dclk90_recal_dis = 0;
                    ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(3),	ddr_dll_ctl3.u64);
                }
            } else {
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000e04bULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000e04b */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000f04bULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000f04b */
                cvmx_wait(                                      0x0000000000002669ULL); /* Delay for 9833 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000b04bULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000b04b */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0),	0x000000000000904bULL); /* 0x80011800880001c8 LMC0_DLL_CTL2 0x000000000000904b */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(0),	0x30d56a0608c10000ULL); /* 0x8001180088000218 LMC0_DLL_CTL3 0x30d56a0608c10502 */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(0),	0x0000010600010000ULL); /* 0x8001180088000218 LMC0_DLL_CTL3 0x0000010600010502 */
                cvmx_wait(                                      0x00000000000002bcULL); /* Delay for 700 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x8680a02d98a1e04bULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x8680a02d98a1e04b */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x000000000000f04bULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x000000000000f04b */
                cvmx_wait(                                      0x000000000000266aULL); /* Delay for 9834 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x000000000000b04bULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x000000000000b04b */
                cvmx_wait(                                      0x0000000000000047ULL); /* Delay for 71 cycles */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(1),	0x000000000000904bULL); /* 0x80011800890001c8 LMC1_DLL_CTL2 0x000000000000904b */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(1),	0x11fb860635a0000fULL); /* 0x8001180089000218 LMC1_DLL_CTL3 0x11fb860635a1608f */
                ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(1),	0x0000010620014000ULL); /* 0x8001180089000218 LMC1_DLL_CTL3 0x000001062001608f */
                cvmx_wait(                                      0x00000000000002bcULL); /* Delay for 700 cycles */
            }

            for (i=3; i>=0; --i) {
                if ((ddr_interface_mask & (1 << i)) == 0)
                    continue;

                if ( !ddr_memory_preserved()) {
                    reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(i));

                    /* 4. Deassert DDRn_RESET_L by writing LMCn_RESET_CTL[ddr3rst]=1 */
                    reset_ctl.cn68xx.ddr3rst = 1; /* Reset de-asserted */
                    ddr_print("LMC%d De-asserting DDR_RESET_L\n", i);
                    ddr_config_write_csr(CVMX_LMCX_RESET_CTL(i), reset_ctl.u64);
                    /* 5. Read LMCn_RESET_CTL and wait for the result */
                    cvmx_read_csr(CVMX_LMCX_RESET_CTL(i));
                    /* 6. Wait a minimum of 500 us */
                    cvmx_wait_usec(500);
                }
            }
            /* Delay for 5022 cycles */
            cvmx_wait(0x000000000000139eULL);
            cvmx_wait_usec(10000); /* Wait a long time. */
        }
        {
            cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
            ddr_dll_ctl3.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL3(ddr_interface_num));
            ddr_dll_ctl3.cn68xx.fine_tune_mode = 1; /* Enable Fine Tune Mode */
            ddr_config_write_csr(CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);
        }
        {
            cvmx_lmcx_comp_ctl2_t comp_ctl2;
            const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;

            comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));

            comp_ctl2.s.dqx_ctl  = (custom_lmc_config->dqx_ctl == 0) ? 4 : custom_lmc_config->dqx_ctl; /* Default 4=34.3 ohm */
            comp_ctl2.s.ck_ctl   = (custom_lmc_config->ck_ctl  == 0) ? 4 : custom_lmc_config->ck_ctl;  /* Default 4=34.3 ohm */
            comp_ctl2.s.cmd_ctl  = (custom_lmc_config->cmd_ctl == 0) ? 4 : custom_lmc_config->cmd_ctl; /* Default 4=34.3 ohm */

            comp_ctl2.cn61xx.rodt_ctl           = 0x4; /* 60 ohm */

            if ((s = lookup_env_parameter("ddr_clk_ctl")) != NULL) {
                comp_ctl2.s.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_ck_ctl")) != NULL) {
                comp_ctl2.s.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_cmd_ctl")) != NULL) {
                comp_ctl2.s.cmd_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dqx_ctl")) != NULL) {
                comp_ctl2.s.dqx_ctl  = simple_strtoul(s, NULL, 0);
            }

            ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), comp_ctl2.u64);
        }
    }

    if (octeon_is_cpuid(OCTEON_CN70XX)) {

        cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;
        cvmx_lmcx_dll_ctl2_t	dll_ctl2;
        cvmx_lmcx_reset_ctl_t lmc_reset_ctl;
        const dimm_config_t *dimm_config_table = ddr_configuration->dimm_config_table;
        int ddr_type;

#ifndef CONFIG_OCTEON_CN70XX
        error_print("\nERROR: Software is not properly configured for the CN70XX processor. ");
        error_print("CONFIG_OCTEON_CN70XX must be defined for this board.\n\n");
#endif

        /* ddr_type only indicates DDR4 or DDR3 */
        ddr_type = (read_spd(&dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_DEVICE_TYPE) == 0x0C) ? DDR4_DRAM : DDR3_DRAM;

        /*
         * 4.8 LMC Initialization Sequence
         *
         * There are eleven parts to the LMC initialization procedure:
         *
         * 1. DDR PLL initialization
         *
         * 2. LMC CK initialization
         *
         * 3. LMC DRESET initialization
         *
         * 4. LMC RESET initialization
         *
         * 5. Early LMC initialization
         *
         * 6. LMC offset training
         *
         * 7. LMC internal Vref training
         *
         * 8. LMC deskew training
         *
         * 9. LMC write leveling
         *
         * 10. LMC read leveling
         *
         * 11. Final LMC initialization
         *
         * During a cold reset, all eight initializations should be executed in
         * sequence. The DDR PLL and LMC CK initializations need only be
         * performed once if the SDRAM-clock speed and parameters do not
         * change. Subsequently, it is possible to execute only DRESET and
         * subsequent steps, or to execute only the final three steps.
         *
         * The remainder of this section covers these initialization steps in
         * sequence.
         *
         * 4.8.1 DDR PLL Initialization
         *
         * Perform the following eight substeps to initialize the DDR PLL:
         *
         * 1. If not done already, write all fields in LMC(0)_DDR_PLL_CTL and
         *    LMC(0)_DLL_CTL2 to their reset values, including:
         *
         *    - LMC(0)_DDR_PLL_CTL[DDR_DIV_RESET] = 1
         *
         *    - LMC(0)_DLL_CTL2[DRESET] = 1
         *
         *    This substep is not necessary after a chip cold reset/power up.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0));

        ddr_pll_ctl.cn70xx.reset_n           = 0;
        ddr_pll_ctl.cn70xx.ddr_div_reset     = 1;
        ddr_pll_ctl.cn70xx.phy_dcok          = 0;

        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);


        dll_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(0));

        dll_ctl2.cn70xx.byp_setting          = 0;
        dll_ctl2.cn70xx.byp_sel              = 0;
        dll_ctl2.cn70xx.quad_dll_ena         = 0;
        dll_ctl2.cn70xx.dreset               = 1;
        dll_ctl2.cn70xx.dll_bringup          = 0;

        /* Set this to notify external software. It doesn't do
           anything in the chip. */
        dll_ctl2.cn70xx.intf_en              = 1;

        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(0), dll_ctl2.u64);

        /*
         * 2. If the current DRAM contents are not preserved (see
         *    LMC(0)_RESET_CTL[DDR3PSV]), this is also an appropriate time to
         *    assert the RESET# pin of the DDR3 DRAM parts. If desired, write
         *    LMC(0)_RESET_CTL[DDR3RST] = 0 without modifying any other
         *    LMC(0)_RESET_CTL fields to assert the DDR_RESET_L pin. No action is
         *    required here to assert DDR_RESET_L following a chip cold
         *    reset/power up.  Refer to Section 4.8.4.
         */

        if ( !ddr_memory_preserved()) {
            cvmx_lmcx_reset_ctl_t reset_ctl;
            reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
            reset_ctl.cn70xx.ddr3rst = 0; /* Reset asserted */
            ddr_print("LMC%d Asserting DDR_RESET_L\n", ddr_interface_num);
            ddr_config_write_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num), reset_ctl.u64);
            cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
        }

        /*
         * 3. Without changing any other LMC(0)_DDR_PLL_CTL values, write
         *    LMC(0)_DDR_PLL_CTL[CLKF] with a value that gives a desired DDR PLL
         *    speed. The LMC(0)_DDR_PLL_CTL[CLKF] value should be selected in
         *    conjunction with the post-scalar divider values for LMC
         *    (LMC(0)_DDR_PLL_CTL[DDR_PS_EN]) so that the desired LMC CK speeds
         *    are is produced. Section 4.13 describes LMC(0)_DDR_PLL_CTL[CLKF]
         *    and LMC(0)_DDR_PLL_CTL[DDR_PS_EN] programmings that produce the
         *    desired LMC CK speed. Section 4.8.2 describes LMC CK
         *    initialization, which can be done separately from the DDR PLL
         *    initialization described in this section.
         *
         *    The LMC(0)_DDR_PLL_CTL[CLKF] value must not change after this point
         *    without restarting this SDRAM PLL initialization sequence.
         */

        {
            /* CLKF = (DCLK * (CLKR+1) * EN(1, 2, 3, 4, 5, 6, 7, 8, 10, 12))/DREF - 1 */
            int en_idx, save_en_idx, best_en_idx=0;
            uint64_t clkf, clkr, max_clkf = 127;
            uint64_t best_clkf=0, best_clkr=0;
            uint64_t best_pll_MHz = 0;
            uint64_t pll_MHz;
            uint64_t min_pll_MHz = 800;
            uint64_t max_pll_MHz = 4300;
            uint64_t error;
            uint64_t best_error;
            uint64_t best_calculated_ddr_hertz = 0;
            uint64_t calculated_ddr_hertz = 0;
            uint64_t orig_ddr_hertz = ddr_hertz;
            static const int _en[] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12};
            int override_pll_settings;

            error = best_error = ddr_hertz;  /* Init to max error */

            ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0));

            ddr_print("DDR Reference Hertz = %d\n", ddr_ref_hertz);

            while (best_error == ddr_hertz)
            {

                for (clkr = 0; clkr < 4; ++clkr) {
                    for (en_idx=sizeof(_en)/sizeof(int)-1; en_idx>=0; --en_idx) {
                        save_en_idx = en_idx;
                        clkf = ((ddr_hertz) * (clkr+1) * (_en[save_en_idx]));
                        clkf = divide_nint(clkf, ddr_ref_hertz) - 1;
                        pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;
                        calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * (_en[save_en_idx]));
                        error = ddr_hertz - calculated_ddr_hertz;

                        if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz)) continue;
                        if (clkf > max_clkf) continue; /* PLL requires clkf to be limited */
                        if (_abs(error) > _abs(best_error)) continue;

#if defined(__U_BOOT__)
                        ddr_print("clkr: %2lu, en[%d]: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d\n",
                                  clkr, save_en_idx, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#else
                        ddr_print("clkr: %2llu, en[%d]: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld\n",
                                  clkr, save_en_idx, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#endif

                        /* Favor the highest PLL frequency. */
                        if ((_abs(error) < _abs(best_error)) || (pll_MHz > best_pll_MHz)) {
                            best_pll_MHz = pll_MHz;
                            best_calculated_ddr_hertz = calculated_ddr_hertz;
                            best_error = error;
                            best_clkr = clkr;
                            best_clkf = clkf;
                            best_en_idx = save_en_idx;
                        }
                    }
                }

                override_pll_settings = 0;

		if ((s = lookup_env_parameter("ddr_pll_clkr")) != NULL) {
                    best_clkr = simple_strtoul(s, NULL, 0);
                    override_pll_settings = 1;
                }
		if ((s = lookup_env_parameter("ddr_pll_clkf")) != NULL) {
                    best_clkf = simple_strtoul(s, NULL, 0);
                    override_pll_settings = 1;
                }
		if ((s = lookup_env_parameter("ddr_pll_en_idx")) != NULL) {
                    best_en_idx = simple_strtoul(s, NULL, 0);
                    override_pll_settings = 1;
                }

                if (override_pll_settings) {
                    best_pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;
                    best_calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * (_en[best_en_idx]));
                    best_error = ddr_hertz - calculated_ddr_hertz;
                }

#if defined(__U_BOOT__)
                ddr_print("clkr: %2lu, en[%d]: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d <==\n",
                          best_clkr, best_en_idx, _en[best_en_idx], best_clkf, best_pll_MHz,
                          best_calculated_ddr_hertz, best_error);
#else
                ddr_print("clkr: %2llu, en[%d]: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld <==\n",
                          best_clkr, best_en_idx, _en[best_en_idx], best_clkf, best_pll_MHz,
                          best_calculated_ddr_hertz, best_error);
#endif

                /* Try lowering the frequency if we can't get a working configuration */
                if (best_error == ddr_hertz) {
                    if (ddr_hertz < orig_ddr_hertz - 10000000)
                        break;
                    ddr_hertz -= 1000000;
                    best_error = ddr_hertz;
                }

            }


            if (best_error == ddr_hertz) {
                error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                return(-1);
            }

            ddr_pll_ctl.cn70xx.ddr_ps_en = best_en_idx;
            ddr_pll_ctl.cn70xx.clkf = best_clkf;
            ddr_pll_ctl.cn70xx.clkr = best_clkr;
            ddr_pll_ctl.cn70xx.reset_n = 0;

            ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);
        }


        /*
         * 4. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 5. Wait a minimum of 3 us.
         */

        cvmx_wait_usec(3);          /* Wait 3 us */

        /*
         * 6. Write LMC(0)_DDR_PLL_CTL[RESET_N] = 1 without changing any other
         *    LMC(0)_DDR_PLL_CTL values.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn70xx.reset_n = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 7. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 8. Wait a minimum of 25 us.
         */

        cvmx_wait_usec(25);          /* Wait 25 us */

        /*
         * 4.8.2 LMC CK Initialization
         *
         * DDR PLL initialization must be completed prior to starting LMC CK
         * initialization.
         *
         * Perform the following seven substeps to initialize the LMC CK:
         *
         * 1. Without changing any other LMC(0)_DDR_PLL_CTL values, write
         *    LMC(0)_DDR_PLL_CTL[DDR_DIV_RESET] = 1 and
         *    LMC(0)_DDR_PLL_CTL[DDR_PS_EN] with the appropriate value to get the
         *    desired LMC CK speed. Section Section 4.13 discusses CLKF and
         *    DDR_PS_EN programmings.
         *
         *    The LMC(0)_DDR_PLL_CTL[DDR_PS_EN] must not change after this point
         *    without restarting this LMC CK initialization sequence.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn70xx.ddr_div_reset = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 2. Without changing any other fields in LMC(0)_DDR_PLL_CTL, write
         *    LMC(0)_DDR_PLL_CTL[DDR4_MODE] = 0.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn70xx.ddr4_mode = (ddr_type == DDR4_DRAM) ? 1 : 0;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 3. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 4. Wait a minimum of 1 us.
         */

        cvmx_wait_usec(1);          /* Wait 1 us */

        /*
         * 5. Without changing any other fields in LMC(0)_DDR_PLL_CTL, write
         *    LMC(0)_DDR_PLL_CTL[PHY_DCOK] = 1.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn70xx.phy_dcok = 1;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 6. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 7. Wait a minimum of 20 us.
         */

        cvmx_wait_usec(20);          /* Wait 20 us */

        /*
         * 8. Without changing any other LMC(0)_COMP_CTL2 values, write
         *    LMC(0)_COMP_CTL2[CK_CTL] to the desired DDR_CK_*_P/
         *    DDR_DIMM*_CS*_L/DDR_DIMM*_ODT_* drive strength.
         */

        {
            cvmx_lmcx_comp_ctl2_t comp_ctl2;
	    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;

            comp_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num));

	    comp_ctl2.cn70xx.dqx_ctl  =
                (custom_lmc_config->dqx_ctl == 0) ? 4 : custom_lmc_config->dqx_ctl; /* Default 4=34.3 ohm */
	    comp_ctl2.cn70xx.ck_ctl   =
                (custom_lmc_config->ck_ctl  == 0) ? 4 : custom_lmc_config->ck_ctl;  /* Default 4=34.3 ohm */
	    comp_ctl2.cn70xx.cmd_ctl  =
                (custom_lmc_config->cmd_ctl == 0) ? 4 : custom_lmc_config->cmd_ctl; /* Default 4=34.3 ohm */

            comp_ctl2.cn70xx.rodt_ctl           = 0x4; /* 60 ohm */

	    comp_ctl2.cn70xx.ptune_offset  = (_abs(custom_lmc_config->ptune_offset) & 0x7) | (_sign(custom_lmc_config->ptune_offset) << 3);
	    comp_ctl2.cn70xx.ntune_offset  = (_abs(custom_lmc_config->ntune_offset) & 0x7) | (_sign(custom_lmc_config->ntune_offset) << 3);

            if ((s = lookup_env_parameter("ddr_clk_ctl")) != NULL) {
                comp_ctl2.cn70xx.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_ck_ctl")) != NULL) {
                comp_ctl2.cn70xx.ck_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_cmd_ctl")) != NULL) {
                comp_ctl2.cn70xx.cmd_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dqx_ctl")) != NULL) {
                comp_ctl2.cn70xx.dqx_ctl  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_ptune_offset")) != NULL) {
                comp_ctl2.cn70xx.ptune_offset  = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_ntune_offset")) != NULL) {
                comp_ctl2.cn70xx.ntune_offset  = simple_strtoul(s, NULL, 0);
            }

            ddr_config_write_csr(CVMX_LMCX_COMP_CTL2(ddr_interface_num), comp_ctl2.u64);
        }

        /*
         * 9. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 10. Wait a minimum of 200 ns.
         */

        cvmx_wait_usec(1);          /* Wait 1 us */

        /*
         * 11. Without changing any other LMC(0)_DDR_PLL_CTL values, write
         *     LMC(0)_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
         */

        ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));
        ddr_pll_ctl.cn70xx.ddr_div_reset = 0;
        ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num), ddr_pll_ctl.u64);

        /*
         * 12. Read LMC(0)_DDR_PLL_CTL and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(ddr_interface_num));

        /*
         * 13. Wait a minimum of 200 ns.
         */

        cvmx_wait_usec(1);          /* Wait 1 us */

        /*
         * 4.8.3 LMC DRESET Initialization
         *
         * Both the DDR PLL and LMC CK initializations must be completed prior to
         * starting this LMC DRESET initialization.
         *
         * 1. If not done already, write LMC(0)_DLL_CTL2 to its reset value,
         *    including LMC(0)_DLL_CTL2[DRESET] = 1.
         */

        /* Already done. */

        /*
         * 2. Without changing any other LMC(0)_DLL_CTL2 fields, write
         *    LMC(0)_DLL_CTL2 [DLL_BRINGUP] = 1.
         */


        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn70xx.dll_bringup = 1;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 3. Read LMC(0)_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 4. Wait for a minimum of 10 LMC CK cycles.
         */

        cvmx_wait_usec(1);

        /*
         * 5. Without changing any other fields in LMC(0)_DLL_CTL2, write
         *    LMC(0)_DLL_CTL2[QUAD_DLL_ENA] = 1.  LMC(0)_DLL_CTL2[QUAD_DLL_ENA]
         *    must not change after this point without restarting the LMC DRESET
         *    initialization sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn70xx.quad_dll_ena = 1;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 6. Read LMC(0)_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 7. Wait a minimum of 10 us.
         */

        cvmx_wait_usec(10);

        /*
         * 8. Without changing any other fields in LMC(0)_DLL_CTL2, write
         *    LMC(0)_DLL_CTL2[DLL_BRINGUP] = 0.  LMC(0)_DLL_CTL2[DLL_BRINGUP]
         *    must not change after this point without restarting the LMC DRESET
         *    initialization sequence.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn70xx.dll_bringup = 0;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 9. Read LMC(0)_DLL_CTL2 and wait for the result.
         */

        cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));

        /*
         * 10. Without changing any other fields in LMC(0)_DLL_CTL2, write
         *     LMC(0)_DLL_CTL2[DRESET] = 0.
         *
         *     LMC(0)_DLL_CTL2[DRESET] must not change after this point without
         *     restarting the LMC DRESET initialization sequence.
         *
         * After completing LMC DRESET initialization, all LMC CSRs may be
         * accessed. Prior to completing LMC DRESET initialization, only
         * LMC(0)_DDR_PLL_CTL, LMC(0)_DLL_CTL2, LMC(0)_RESET_CTL, and
         * LMC(0)_COMP_CTL2 LMC CSRs can be accessed.
         */

        dll_ctl2.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num));
        dll_ctl2.cn70xx.dreset= 0;
        ddr_config_write_csr(CVMX_LMCX_DLL_CTL2(ddr_interface_num), dll_ctl2.u64);

        /*
         * 4.8.4 LMC RESET Initialization
         *
         * The purpose of this step is to assert/deassert the RESET# pin at the
         * DDR3 parts.
         *
         * It may be appropriate to skip this step if the DDR3 DRAM parts are in
         * self refresh and are currently preserving their contents. (Software
         * can determine this via LMC(0)_RESET_CTL[DDR3PSV] in some
         * circumstances.) The remainder of this section assumes that the DRAM
         * contents need not be preserved.
         *
         * The remainder of this section assumes that the CN70XX DDR_RESET_L pin
         * is attached to the RESET# pin of the attached DDR3 parts, as will be
         * appropriate in many systems.
         *
         * (In other systems, such as ones that can preserve DDR3 part contents
         * while CN70XX is powered down, it will not be appropriate to directly
         * attach the CN70XX DDR_RESET_L pin to DRESET# of the DDR3 parts, and
         * this section may not apply.)
         *
         * Perform the following six substeps for LMC reset initialization:
         *
         * 1. If not done already, assert DDR_RESET_L pin by writing
         *    LMC(0)_RESET_CTL[DDR3RST] = 0 without modifying any other
         *    LMC(0)_RESET_CTL fields.
         */

        if ( !ddr_memory_preserved()) {
            /*
             * 2. Read LMC(0)_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));

            /*
             * 3. Wait until RESET# assertion-time requirement from JEDEC DDR3
             *    specification is satisfied (200 us during a power-on ramp, 100ns
             *    when power is already stable).
             */

            cvmx_wait_usec(200);

            /*
             * 4. Deassert DDR_RESET_L pin by writing LMC(0)_RESET_CTL[DDR3RST] = 1
             *    without modifying any other LMC(0)_RESET_CTL fields.
             */

            lmc_reset_ctl.u64 = cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));
            lmc_reset_ctl.cn70xx.ddr3rst = 1;
            ddr_print("LMC%d De-asserting DDR_RESET_L\n", ddr_interface_num);
            ddr_config_write_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num), lmc_reset_ctl.u64);

            /*
             * 5. Read LMC(0)_RESET_CTL and wait for the result.
             */

            cvmx_read_csr(CVMX_LMCX_RESET_CTL(ddr_interface_num));

            /*
             * 6. Wait a minimum of 500us. This guarantees the necessary T = 500us
             *    delay between DDR_RESET_L deassertion and DDR_CKE* assertion.
             */

            cvmx_wait_usec(500);
        }
    }

    if (octeon_is_cpuid(OCTEON_CN78XX) || octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX)) {

        cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;
        const dimm_config_t *dimm_config_table = ddr_configuration->dimm_config_table;
        int ddr_type;

        /* ddr_type only indicates DDR4 or DDR3 */
        ddr_type = (read_spd(&dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_DEVICE_TYPE) == 0x0C) ? DDR4_DRAM : DDR3_DRAM;

        /*
         * 5.9 LMC Initialization Sequence
         *
         * There are 13 parts to the LMC initialization procedure:
         *
         * 1. DDR PLL initialization
         *
         * 2. LMC CK initialization
         *
         * 3. LMC interface enable initialization
         *
         * 4. LMC DRESET initialization
         *
         * 5. LMC CK local initialization
         *
         * 6. LMC RESET initialization
         *
         * 7. Early LMC initialization
         *
         * 8. LMC offset training
         *
         * 9. LMC internal Vref training
         *
         * 10. LMC deskew training
         *
         * 11. LMC write leveling
         *
         * 12. LMC read leveling
         *
         * 13. Final LMC initialization
         *
         * CN78XX supports two modes:
         *
         * ? two-LMC mode: both LMCs 2/3 must not be enabled
         * (LMC2/3_DLL_CTL2[DRESET] must be set to 1 and LMC2/3_DLL_CTL2[INTF_EN]
         * must be set to 0) and both LMCs 0/1 must be enabled).
         *
         * ? four-LMC mode: all four LMCs 0..3 must be enabled.
         *
         * Steps 4 and 6..13 should each be performed for each enabled LMC (either
         * twice or four times). Steps 1..3 and 5 are more global in nature and
         * each must be executed exactly once (not once per LMC) each time the
         *  DDR PLL changes or is first brought up. Steps 1..3 and 5 need not be
         * performed if the DDR PLL is stable.
         *
         * Generally, the steps are performed in order. The exception is that the
         * CK local initialization (step 5) must be performed after some DRESET
         * initializations (step 4) and before other DRESET initializations when
         * the DDR PLL is brought up or changed. (The CK local initialization
         * uses information from some LMCs to bring up the other local CKs.) The
         * following text describes these ordering requirements in more detail.
         *
         * Following any chip reset, the DDR PLL must be brought up, and all 13
         * steps should be executed. Subsequently, it is possible to execute only
         * steps 4 and 6..13, or to execute only steps 8..13.
         *
         * The remainder of this section covers these initialization steps in
         * sequence.
         */

        if (ddr_interface_num == 0) { /* Do this once */
            cvmx_lmcx_dll_ctl2_t	dll_ctl2;
            int loop_interface_num;

            /*
             * 5.9.3 LMC Interface-Enable Initialization
             *
             * LMC interface-enable initialization (Step 3) must be performed after
             * Step 2 for each chip reset and whenever the DDR clock speed
             * changes. This step needs to be performed only once, not once per LMC.
             * Perform the following three substeps for the LMC interface-enable
             * initialization:
             *
             * 1. Without changing any other LMC2_DLL_CTL2 fields (LMC(0..3)_DLL_CTL2
             * should be at their reset values after Step 1), write
             * LMC2_DLL_CTL2[INTF_EN] = 1 if four-LMC mode is desired.
             *
             * 2. Without changing any other LMC3_DLL_CTL2 fields, write
             * LMC3_DLL_CTL2[INTF_EN] = 1 if four-LMC mode is desired.
             *
             * 3. Read LMC2_DLL_CTL2 and wait for the result.
             *
             * The LMC2_DLL_CTL2[INTF_EN] and LMC3_DLL_CTL2[INTF_EN] values should
             * not be changed by software from this point.
             *
             */

            for (loop_interface_num=0; loop_interface_num<4; ++loop_interface_num) {
                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;

                dll_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(loop_interface_num));

                dll_ctl2.cn78xx.byp_setting          = 0;
                dll_ctl2.cn78xx.byp_sel              = 0;
                dll_ctl2.cn78xx.quad_dll_ena         = 0;
                dll_ctl2.cn78xx.dreset               = 1;
                dll_ctl2.cn78xx.dll_bringup          = 0;
                dll_ctl2.cn78xx.intf_en              = 0;

                ddr_config_write_csr_node(node,
					  CVMX_LMCX_DLL_CTL2(loop_interface_num),
					  dll_ctl2.u64);
            }

            /* ###### Interface enable (intf_en)deferred until after DDR_DIV_RESET=0 ####### */

            /*
             * 5.9.1 DDR PLL Initialization
             *
             * DDR PLL initialization (Step 1) must be performed for each chip reset
             * and whenever the DDR clock speed changes. This step needs to be
             * performed only once, not once per LMC.
             *
             * Perform the following eight substeps to initialize the DDR PLL:
             *
             * 1. If not done already, write all fields in LMC(0..3)_DDR_PLL_CTL and
             * LMC(0..1)_DLL_CTL2 to their reset values, including:
             *
             * .. LMC0_DDR_PLL_CTL[DDR_DIV_RESET] = 1
             * .. LMC0_DLL_CTL2[DRESET] = 1
             *
             * This substep is not necessary after a chip reset.
             *
             */

            ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0));

            ddr_pll_ctl.cn78xx.reset_n           = 0;
            ddr_pll_ctl.cn78xx.ddr_div_reset     = 1;
            ddr_pll_ctl.cn78xx.phy_dcok          = 0;

            // 73XX pass 1.3 has LMC0 DCLK_INVERT tied to 1; earlier 73xx passes are tied to 0
            // 75XX needs LMC0 DCLK_INVERT set to 1 to minimize duty cycle falling points
            // and we default all other chips LMC0 to DCLK_INVERT=0
            ddr_pll_ctl.cn78xx.dclk_invert = !!(octeon_is_cpuid(OCTEON_CN73XX_PASS1_3) ||
                                                octeon_is_cpuid(OCTEON_CNF75XX));

            // allow override of LMC0 desired setting for DCLK_INVERT, but not on 73XX;
            // we cannot change LMC0 DCLK_INVERT on 73XX any pass
            if (! (octeon_is_cpuid(OCTEON_CN73XX))) {
                if ((s = lookup_env_parameter("ddr0_set_dclk_invert")) != NULL) {
                    ddr_pll_ctl.cn78xx.dclk_invert = !!simple_strtoul(s, NULL, 0);
                    ddr_print("LMC0: override DDR_PLL_CTL[dclk_invert] to %d\n",
                              ddr_pll_ctl.cn78xx.dclk_invert);
                }
            }

            ddr_config_write_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);
            ddr_print("%-45s : 0x%016llx\n", "LMC0: DDR_PLL_CTL",
                      (unsigned long long)ddr_pll_ctl.u64);

            // only when LMC1 is active
            if (ddr_interface_mask & 0x2) {

                /* For CNF75XX, both LMC0 and LMC1 use the same PLL,
                 * so we use the LMC0 setting of DCLK_INVERT for LMC1.
                 */
                if (! octeon_is_cpuid(OCTEON_CNF75XX)) {
                    int override = 0;

                    // by default, for non-CNF75XX, we want LMC1 toggled LMC0
                    int lmc0_dclk_invert = ddr_pll_ctl.cn78xx.dclk_invert;

                    // FIXME: work-around for DDR3 UDIMM problems is to use LMC0 setting on LMC1
                    // and if 73xx pass 1.3, we want to default LMC1 DCLK_INVERT to LMC0, not the invert of LMC0
                    int lmc1_dclk_invert = ((ddr_type == DDR4_DRAM) && !octeon_is_cpuid(OCTEON_CN73XX_PASS1_3))
                        		   ? lmc0_dclk_invert ^ 1
                        		   : lmc0_dclk_invert;

                    // allow override of LMC1 desired setting for DCLK_INVERT
                    if ((s = lookup_env_parameter("ddr1_set_dclk_invert")) != NULL) {
                        lmc1_dclk_invert = !!simple_strtoul(s, NULL, 0);
                        override = 1;
                    }
                    ddr_print("LMC1: %s DDR_PLL_CTL[dclk_invert] to %d (LMC0 %d)\n",
                              (override) ? "override" : "default",
                              lmc1_dclk_invert, lmc0_dclk_invert);

                    ddr_pll_ctl.cn78xx.dclk_invert = lmc1_dclk_invert;
                }

                // but always write LMC1 CSR if it is active
                ddr_config_write_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(1), ddr_pll_ctl.u64);
                ddr_print("%-45s : 0x%016llx\n", "LMC1: DDR_PLL_CTL",
                          (unsigned long long)ddr_pll_ctl.u64);
            }

            /*
             * 2. If the current DRAM contents are not preserved (see
             * LMC(0..3)_RESET_ CTL[DDR3PSV]), this is also an appropriate time to
             * assert the RESET# pin of the DDR3/DDR4 DRAM parts. If desired, write
             * LMC0_RESET_ CTL[DDR3RST] = 0 without modifying any other
             * LMC0_RESET_CTL fields to assert the DDR_RESET_L pin. No action is
             * required here to assert DDR_RESET_L following a chip reset. Refer to
             * Section 5.9.6. Do this for all enabled LMCs.
             */

            for (loop_interface_num = 0;
                 ( !ddr_memory_preserved()) && loop_interface_num<4;
                 ++loop_interface_num) {
                cvmx_lmcx_reset_ctl_t reset_ctl;

                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;

                reset_ctl.u64 = cvmx_read_csr_node(node,
						   CVMX_LMCX_RESET_CTL(loop_interface_num));
                reset_ctl.cn78xx.ddr3rst = 0; /* Reset asserted */
                ddr_print("LMC%d Asserting DDR_RESET_L\n", loop_interface_num);
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_RESET_CTL(loop_interface_num),
					  reset_ctl.u64);
                cvmx_read_csr_node(node,
				   CVMX_LMCX_RESET_CTL(loop_interface_num));
            }

            /*
             * 3. Without changing any other LMC0_DDR_PLL_CTL values, write LMC0_DDR_
             * PLL_CTL[CLKF] with a value that gives a desired DDR PLL speed. The
             * LMC0_DDR_PLL_CTL[CLKF] value should be selected in conjunction with
             * the post-scalar divider values for LMC (LMC0_DDR_PLL_CTL[DDR_PS_EN])
             * so that the desired LMC CK speeds are is produced (all enabled LMCs
             * must run the same speed). Section 5.14 describes
             * LMC0_DDR_PLL_CTL[CLKF] and LMC0_DDR_PLL_CTL[DDR_PS_EN] programmings
             * that produce the desired LMC CK speed. Section 5.9.2 describes LMC CK
             * initialization, which can be done separately from the DDR PLL
             * initialization described in this section.
             *
             * The LMC0_DDR_PLL_CTL[CLKF] value must not change after this point
             * without restarting this SDRAM PLL initialization sequence.
             */

            {
                /* CLKF = (DCLK * (CLKR+1) * EN(1, 2, 3, 4, 5, 6, 7, 8, 10, 12))/DREF - 1 */
                int en_idx, save_en_idx, best_en_idx=0;
                uint64_t clkf, clkr, max_clkf = 127;
                uint64_t best_clkf=0, best_clkr=0;
                uint64_t best_pll_MHz = 0;
                uint64_t pll_MHz;
                uint64_t min_pll_MHz = 800;
                uint64_t max_pll_MHz = 5000;
                uint64_t error;
                uint64_t best_error;
                uint64_t best_calculated_ddr_hertz = 0;
                uint64_t calculated_ddr_hertz = 0;
                uint64_t orig_ddr_hertz = ddr_hertz;
                static const int _en[] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12};
                int override_pll_settings;
                int new_bwadj;

                error = best_error = ddr_hertz;  /* Init to max error */

                ddr_print("DDR Reference Hertz = %d\n", ddr_ref_hertz);

                while (best_error == ddr_hertz)
                {

                    for (clkr = 0; clkr < 4; ++clkr) {
                        for (en_idx=sizeof(_en)/sizeof(int)-1; en_idx >= 0; --en_idx) {
                            save_en_idx = en_idx;
                            clkf = ((ddr_hertz) * (clkr+1) * (_en[save_en_idx]));
                            clkf = divide_nint(clkf, ddr_ref_hertz) - 1;
                            pll_MHz = ddr_ref_hertz * (clkf+1) / (clkr+1) / 1000000;
                            calculated_ddr_hertz = ddr_ref_hertz * (clkf + 1) / ((clkr + 1) * (_en[save_en_idx]));
                            error = ddr_hertz - calculated_ddr_hertz;

                            if ((pll_MHz < min_pll_MHz) || (pll_MHz > max_pll_MHz))
                                continue;
                            if (clkf > max_clkf)
                                continue; /* PLL requires clkf to be limited */
                            if (_abs(error) > _abs(best_error))
                                continue;

#if defined(__U_BOOT__)
                            ddr_print("clkr: %2lu, en[%d]: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d\n",
                                      clkr, save_en_idx, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#else
                            ddr_print("clkr: %2llu, en[%d]: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld\n",
                                      clkr, save_en_idx, _en[save_en_idx], clkf, pll_MHz, calculated_ddr_hertz, error);
#endif

                            /* Favor the highest PLL frequency. */
                            if ((_abs(error) < _abs(best_error)) || (pll_MHz > best_pll_MHz)) {
                                best_pll_MHz = pll_MHz;
                                best_calculated_ddr_hertz = calculated_ddr_hertz;
                                best_error = error;
                                best_clkr = clkr;
                                best_clkf = clkf;
                                best_en_idx = save_en_idx;
                            }
                        }
                    }

                    override_pll_settings = 0;

                    if ((s = lookup_env_parameter("ddr_pll_clkr")) != NULL) {
			best_clkr = simple_strtoul(s, NULL, 0);
                        override_pll_settings = 1;
                    }
                    if ((s = lookup_env_parameter("ddr_pll_clkf")) != NULL) {
			best_clkf = simple_strtoul(s, NULL, 0);
                        override_pll_settings = 1;
                    }
                    if ((s = lookup_env_parameter("ddr_pll_en_idx")) != NULL) {
			best_en_idx = simple_strtoul(s, NULL, 0);
                        override_pll_settings = 1;
                    }

                    if (override_pll_settings) {
                        best_pll_MHz = ddr_ref_hertz * (best_clkf+1) / (best_clkr+1) / 1000000;
                        best_calculated_ddr_hertz = ddr_ref_hertz * (best_clkf + 1) / ((best_clkr + 1) * (_en[best_en_idx]));
                        best_error = ddr_hertz - best_calculated_ddr_hertz;
                    }

#if defined(__U_BOOT__)
                    ddr_print("clkr: %2lu, en[%d]: %2d, clkf: %4lu, pll_MHz: %4lu, ddr_hertz: %8lu, error: %8d <==\n",
                              best_clkr, best_en_idx, _en[best_en_idx], best_clkf, best_pll_MHz,
                              best_calculated_ddr_hertz, best_error);
#else
                    ddr_print("clkr: %2llu, en[%d]: %2d, clkf: %4llu, pll_MHz: %4llu, ddr_hertz: %8llu, error: %8lld <==\n",
                              best_clkr, best_en_idx, _en[best_en_idx], best_clkf, best_pll_MHz,
                              best_calculated_ddr_hertz, best_error);
#endif

                    /* Try lowering the frequency if we can't get a working configuration */
                    if (best_error == ddr_hertz) {
                        if (ddr_hertz < orig_ddr_hertz - 10000000)
                            break;
                        ddr_hertz -= 1000000;
                        best_error = ddr_hertz;
                    }

                }


                if (best_error == ddr_hertz) {
                    error_print("ERROR: Can not compute a legal DDR clock speed configuration.\n");
                    return(-1);
                }

                new_bwadj = (best_clkf + 1) / 10;
                ddr_print("bwadj: %2d\n", new_bwadj);

                if ((s = lookup_env_parameter("ddr_pll_bwadj")) != NULL) {
                    new_bwadj = strtoul(s, NULL, 0);
                    ddr_print("bwadj: %2d\n", new_bwadj);
                }

                for (loop_interface_num = 0; loop_interface_num<2; ++loop_interface_num) {
                    if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                        continue;

                    ddr_pll_ctl.u64 =
                        cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                    ddr_print("LMC%d: DDR_PLL_CTL                             : 0x%016llx\n",
                              loop_interface_num, (unsigned long long)ddr_pll_ctl.u64);

                    ddr_pll_ctl.cn78xx.ddr_ps_en = best_en_idx;
                    ddr_pll_ctl.cn78xx.clkf = best_clkf;
                    ddr_pll_ctl.cn78xx.clkr = best_clkr;
                    ddr_pll_ctl.cn78xx.reset_n = 0;
                    ddr_pll_ctl.cn78xx.bwadj = new_bwadj;

                    ddr_config_write_csr_node(node,
					      CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
					      ddr_pll_ctl.u64);
                    ddr_print("LMC%d: DDR_PLL_CTL                             : 0x%016llx\n",
                              loop_interface_num, (unsigned long long)ddr_pll_ctl.u64);

                    /* For cnf75xx LMC0 and LMC1 use the same PLL so
                       only program LMC0 PLL. */
                    if (octeon_is_cpuid(OCTEON_CNF75XX)) break;
                }
            }


            for (loop_interface_num = 0; loop_interface_num<4; ++loop_interface_num) {
                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;
                /*
                 * 4. Read LMC0_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 5. Wait a minimum of 3 us.
                 */

                cvmx_wait_usec(3);          /* Wait 3 us */

                /*
                 * 6. Write LMC0_DDR_PLL_CTL[RESET_N] = 1 without changing any other
                 * LMC0_DDR_PLL_CTL values.
                 */

                ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                ddr_pll_ctl.cn78xx.reset_n = 1;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
                                          ddr_pll_ctl.u64);

                /*
                 * 7. Read LMC0_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 8. Wait a minimum of 25 us.
                 */

                cvmx_wait_usec(25);          /* Wait 25 us */

                /* For cnf75xx LMC0 and LMC1 use the same PLL so
                   only program LMC0 PLL. */
                if (octeon_is_cpuid(OCTEON_CNF75XX)) break;
            }

            for (loop_interface_num = 0; loop_interface_num<4; ++loop_interface_num) {
                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;
                /*
                 * 5.9.2 LMC CK Initialization
                 *
                 * DDR PLL initialization must be completed prior to starting LMC CK
                 * initialization.
                 *
                 * Perform the following substeps to initialize the LMC CK:
                 *
                 * 1. Without changing any other LMC(0..3)_DDR_PLL_CTL values, write
                 * LMC(0..3)_DDR_PLL_CTL[DDR_DIV_RESET] = 1 and
                 * LMC(0..3)_DDR_PLL_CTL[DDR_PS_EN] with the appropriate value to get the
                 * desired LMC CK speed. Section 5.14 discusses CLKF and DDR_PS_EN
                 * programmings.  The LMC(0..3)_DDR_PLL_CTL[DDR_PS_EN] must not change
                 * after this point without restarting this LMC CK initialization
                 * sequence.
                 */

                ddr_pll_ctl.u64 = cvmx_read_csr_node(node,
                                                     CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                ddr_pll_ctl.cn78xx.ddr_div_reset = 1;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
                                          ddr_pll_ctl.u64);

                /*
                 * 2. Without changing any other fields in LMC(0..3)_DDR_PLL_CTL, write
                 * LMC(0..3)_DDR_PLL_CTL[DDR4_MODE] = 0.
                 */

                ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                ddr_pll_ctl.cn78xx.ddr4_mode = (ddr_type == DDR4_DRAM) ? 1 : 0;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
                                          ddr_pll_ctl.u64);

                /*
                 * 3. Read LMC(0..3)_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node,
                                   CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 4. Wait a minimum of 1 us.
                 */

                cvmx_wait_usec(1);          /* Wait 1 us */

                /* ###### Steps 5 through 7 deferred until after DDR_DIV_RESET=0 ####### */

                /*
                 * 8. Without changing any other LMC(0..3)_COMP_CTL2 values, write
                 * LMC(0..3)_COMP_CTL2[CK_CTL,CONTROL_CTL,CMD_CTL] to the desired
                 * DDR*_CK_*_P control and command signals drive strength.
                 */

                {
                    cvmx_lmcx_comp_ctl2_t comp_ctl2;
                    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;

                    comp_ctl2.u64 = cvmx_read_csr_node(node,
                                                       CVMX_LMCX_COMP_CTL2(loop_interface_num));

                    comp_ctl2.cn78xx.dqx_ctl  =
                        (custom_lmc_config->dqx_ctl == 0) ? 4 : custom_lmc_config->dqx_ctl; /* Default 4=34.3 ohm */
                    comp_ctl2.cn78xx.ck_ctl   =
                        (custom_lmc_config->ck_ctl  == 0) ? 4 : custom_lmc_config->ck_ctl;  /* Default 4=34.3 ohm */
                    comp_ctl2.cn78xx.cmd_ctl  =
                        (custom_lmc_config->cmd_ctl == 0) ? 4 : custom_lmc_config->cmd_ctl; /* Default 4=34.3 ohm */

                    comp_ctl2.cn78xx.rodt_ctl           = 0x4; /* 60 ohm */

                    comp_ctl2.cn70xx.ptune_offset  = (_abs(custom_lmc_config->ptune_offset) & 0x7) | (_sign(custom_lmc_config->ptune_offset) << 3);
                    comp_ctl2.cn70xx.ntune_offset  = (_abs(custom_lmc_config->ntune_offset) & 0x7) | (_sign(custom_lmc_config->ntune_offset) << 3);

                    if ((s = lookup_env_parameter("ddr_clk_ctl")) != NULL) {
                        comp_ctl2.cn78xx.ck_ctl  = simple_strtoul(s, NULL, 0);
                    }

                    if ((s = lookup_env_parameter("ddr_ck_ctl")) != NULL) {
                        comp_ctl2.cn78xx.ck_ctl  = simple_strtoul(s, NULL, 0);
                    }

                    if ((s = lookup_env_parameter("ddr_cmd_ctl")) != NULL) {
                        comp_ctl2.cn78xx.cmd_ctl  = simple_strtoul(s, NULL, 0);
                    }

                    if ((s = lookup_env_parameter("ddr_dqx_ctl")) != NULL) {
                        comp_ctl2.cn78xx.dqx_ctl  = simple_strtoul(s, NULL, 0);
                    }

                    if ((s = lookup_env_parameter("ddr_ptune_offset")) != NULL) {
                        comp_ctl2.cn78xx.ptune_offset  = simple_strtoul(s, NULL, 0);
                    }

                    if ((s = lookup_env_parameter("ddr_ntune_offset")) != NULL) {
                        comp_ctl2.cn78xx.ntune_offset  = simple_strtoul(s, NULL, 0);
                    }

                    ddr_config_write_csr_node(node,
                                              CVMX_LMCX_COMP_CTL2(loop_interface_num),
                                              comp_ctl2.u64);
                }

                /*
                 * 9. Read LMC(0..3)_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node,
                                   CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 10. Wait a minimum of 200 ns.
                 */

                cvmx_wait_usec(1);          /* Wait 1 us */

                /*
                 * 11. Without changing any other LMC(0..3)_DDR_PLL_CTL values, write
                 * LMC(0..3)_DDR_PLL_CTL[DDR_DIV_RESET] = 0.
                 */

                ddr_pll_ctl.u64 = cvmx_read_csr_node(node,
                                                     CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                ddr_pll_ctl.cn78xx.ddr_div_reset = 0;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
                                          ddr_pll_ctl.u64);

                /*
                 * 12. Read LMC(0..3)_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 13. Wait a minimum of 200 ns.
                 */

                cvmx_wait_usec(1);          /* Wait 1 us */
            }

            /*
             * Relocated Interface Enable (intf_en) Step
             */
            for (loop_interface_num = (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX)) ? 1 : 2;
                 loop_interface_num < 4; ++loop_interface_num) {
                /* This step is only necessary for LMC 2 and 3 in
                   4-LMC mode. The mask will cause the unpopulated
                   interfaces to be skipped. */
                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;

                dll_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(loop_interface_num));
                dll_ctl2.cn78xx.intf_en              = 1;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL2(loop_interface_num), dll_ctl2.u64);
                cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(loop_interface_num));
            }


            /*
             * Relocated PHY_DCOK Step
             */
            for (loop_interface_num = 0; loop_interface_num<4; ++loop_interface_num) {
                if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                    continue;
                /*
                 * 5. Without changing any other fields in LMC(0..3)_DDR_PLL_CTL, write
                 * LMC(0..3)_DDR_PLL_CTL[PHY_DCOK] = 1.
                 */

                ddr_pll_ctl.u64 = cvmx_read_csr_node(node,
                                                     CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));
                ddr_pll_ctl.cn78xx.phy_dcok = 1;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_DDR_PLL_CTL(loop_interface_num),
                                          ddr_pll_ctl.u64);
                /*
                 * 6. Read LMC(0..3)_DDR_PLL_CTL and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(loop_interface_num));

                /*
                 * 7. Wait a minimum of 20 us.
                 */

                cvmx_wait_usec(20);          /* Wait 20 us */
            }
        } /* Do this once */

        if (ddr_interface_num == 0) { /* Do this once */
            cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;

            /*
             * 5.9.4 LMC DRESET Initialization
             *
             * All of the DDR PLL, LMC global CK, and LMC interface enable
             * initializations must be completed prior to starting this LMC DRESET
             * initialization (Step 4).
             *
             * This LMC DRESET step is done for all enabled LMCs.
             *
             * There are special constraints on the ordering of DRESET initialization
             * (Steps 4) and CK local initialization (Step 5) whenever CK local
             * initialization must be executed.  CK local initialization must be
             * executed whenever the DDR PLL is being brought up (for each chip reset
             * and whenever the DDR clock speed changes).
             *
             * When Step 5 must be executed in the two-LMC mode case:
             * ? LMC0 DRESET initialization must occur before Step 5.
             * ? LMC1 DRESET initialization must occur after Step 5.
             *
             * When Step 5 must be executed in the four-LMC mode case:
             * ? LMC2 and LMC3 DRESET initialization must occur before Step 5.
             * ? LMC0 and LMC1 DRESET initialization must occur after Step 5.
             */

            if (octeon_is_cpuid(OCTEON_CN73XX)) {
                /* ONE-LMC or TWO-LMC MODE BEFORE STEP 5 for cn73xx */
                    cn78xx_lmc_dreset_init(node, 0);
            } else if (octeon_is_cpuid(OCTEON_CNF75XX)) {
                if (ddr_interface_mask == 0x3) {
                    /* 2-LMC Mode: LMC1 DRESET must occur before Step 5 */
                    cn78xx_lmc_dreset_init(node, 1);
                }
            } else {
                /* TWO-LMC MODE DRESET BEFORE STEP 5 */
                if (ddr_interface_mask == 0x3) {
                    cn78xx_lmc_dreset_init(node, 0);
                }

                /* FOUR-LMC MODE BEFORE STEP 5 */
                if (ddr_interface_mask == 0xf) {
                    cn78xx_lmc_dreset_init(node, 2);
                    cn78xx_lmc_dreset_init(node, 3);
                }
            }

            /*
             * 5.9.5 LMC CK Local Initialization
             *
             * All of DDR PLL, LMC global CK, and LMC interface-enable
             * initializations must be completed prior to starting this LMC CK local
             * initialization (Step 5).
             *
             * LMC CK Local initialization must be performed for each chip reset and
             * whenever the DDR clock speed changes. This step needs to be performed
             * only once, not once per LMC.
             *
             * There are special constraints on the ordering of DRESET initialization
             * (Steps 4) and CK local initialization (Step 5) whenever CK local
             * initialization must be executed.  CK local initialization must be
             * executed whenever the DDR PLL is being brought up (for each chip reset
             * and whenever the DDR clock speed changes).
             *
             * When Step 5 must be executed in the two-LMC mode case:
             * ? LMC0 DRESET initialization must occur before Step 5.
             * ? LMC1 DRESET initialization must occur after Step 5.
             *
             * When Step 5 must be executed in the four-LMC mode case:
             * ? LMC2 and LMC3 DRESET initialization must occur before Step 5.
             * ? LMC0 and LMC1 DRESET initialization must occur after Step 5.
             *
             * LMC CK local initialization is different depending on whether two-LMC
             * or four-LMC modes are desired.
             */

            if (ddr_interface_mask == 0x3) {
                int temp_lmc_interface_num = octeon_is_cpuid(OCTEON_CNF75XX) ? 1 : 0;

                /*
                 * 5.9.5.1 LMC CK Local Initialization for Two-LMC Mode
                 *
                 * 1. Write LMC0_DLL_CTL3 to its reset value. (Note that
                 * LMC0_DLL_CTL3[DLL_90_BYTE_SEL] = 0x2 .. 0x8 should also work.)
                 */

                ddr_dll_ctl3.u64 = 0;
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 1;

                if (octeon_is_cpuid(OCTEON_CNF75XX)) {
                    ddr_dll_ctl3.cn78xx.dll90_byte_sel = 7;
                } else {
                    ddr_dll_ctl3.cn78xx.dll90_byte_sel = 1;
                }

                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(temp_lmc_interface_num),
                                          ddr_dll_ctl3.u64);

                /*
                 * 2. Read LMC0_DLL_CTL3 and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(temp_lmc_interface_num));

                /*
                 * 3. Without changing any other fields in LMC0_DLL_CTL3, write
                 * LMC0_DLL_CTL3[DCLK90_FWD] = 1.  Writing LMC0_DLL_CTL3[DCLK90_FWD] = 1
                 * causes clock-delay information to be forwarded from LMC0 to LMC1.
                 */

                ddr_dll_ctl3.cn78xx.dclk90_fwd = 1;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(temp_lmc_interface_num),
                                          ddr_dll_ctl3.u64);

                /*
                 * 4. Read LMC0_DLL_CTL3 and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(temp_lmc_interface_num));
            }

            if (ddr_interface_mask == 0xf) {
                /*
                 * 5.9.5.2 LMC CK Local Initialization for Four-LMC Mode
                 *
                 * 1. Write LMC2_DLL_CTL3 to its reset value except LMC2_DLL_CTL3[DLL90_-
                 * BYTE_SEL] = 0x7.
                 */

                ddr_dll_ctl3.u64 = 0;
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 1;
                ddr_dll_ctl3.cn78xx.dll90_byte_sel = 7;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(2),
                                          ddr_dll_ctl3.u64);

                /*
                 * 2. Write LMC3_DLL_CTL3 to its reset value except LMC3_DLL_CTL3[DLL90_-
                 * BYTE_SEL] = 0x2.
                 */

                ddr_dll_ctl3.u64 = 0;
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 1;
                ddr_dll_ctl3.cn78xx.dll90_byte_sel = 2;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(3),
                                          ddr_dll_ctl3.u64);

                /*
                 * 3. Read LMC3_DLL_CTL3 and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(3));

                /*
                 * 4. Without changing any other fields in LMC2_DLL_CTL3, write
                 * LMC2_DLL_CTL3[DCLK90_FWD] = 1 and LMC2_DLL_CTL3[DCLK90_RECAL_ DIS] = 1.
                 * Writing LMC2_DLL_CTL3[DCLK90_FWD] = 1 causes LMC 2 to forward
                 * clockdelay information to LMC0. Setting
                 * LMC2_DLL_CTL3[DCLK90_RECAL_DIS] to 1 prevents LMC2 from periodically
                 * recalibrating this delay information.
                 */

                ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(2));
                ddr_dll_ctl3.cn78xx.dclk90_fwd = 1;
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 1;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(2),
                                          ddr_dll_ctl3.u64);

                /*
                 * 5. Without changing any other fields in LMC3_DLL_CTL3, write
                 * LMC3_DLL_CTL3[DCLK90_FWD] = 1 and LMC3_DLL_CTL3[DCLK90_RECAL_ DIS] = 1.
                 * Writing LMC3_DLL_CTL3[DCLK90_FWD] = 1 causes LMC3 to forward
                 * clockdelay information to LMC1. Setting
                 * LMC3_DLL_CTL3[DCLK90_RECAL_DIS] to 1 prevents LMC3 from periodically
                 * recalibrating this delay information.
                 */

                ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(3));
                ddr_dll_ctl3.cn78xx.dclk90_fwd = 1;
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 1;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(3),
                                          ddr_dll_ctl3.u64);

                /*
                 * 6. Read LMC3_DLL_CTL3 and wait for the result.
                 */

                cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(3));
            }


            if (octeon_is_cpuid(OCTEON_CNF75XX)) {
                /* cnf75xx 2-LMC Mode: LMC0 DRESET must occur after
                   Step 5, Do LMC0 for 1-LMC Mode here too */
                cn78xx_lmc_dreset_init(node, 0);
            }

            /* TWO-LMC MODE AFTER STEP 5 */
            if (ddr_interface_mask == 0x3) {
                if (octeon_is_cpuid(OCTEON_CNF75XX)) {
                    /* cnf75xx 2-LMC Mode: LMC0 DRESET must occur after Step 5 */
                    cn78xx_lmc_dreset_init(node, 0);
                } else {
                    cn78xx_lmc_dreset_init(node, 1);
                }
            }

            /* FOUR-LMC MODE AFTER STEP 5 */
            if (ddr_interface_mask == 0xf) {
                cn78xx_lmc_dreset_init(node, 0);
                cn78xx_lmc_dreset_init(node, 1);

                /* Enable periodic recalibration of DDR90 delay line in. */
                ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(0));
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 0;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(0),
                                          ddr_dll_ctl3.u64);
                ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(1));
                ddr_dll_ctl3.cn78xx.dclk90_recal_dis = 0;
                ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(1),
                                          ddr_dll_ctl3.u64);
            }

            /* Enable fine tune mode for all LMCs */
            {
                int lmc;
                for (lmc = 0; lmc < 4; ++lmc) {
                    if ((ddr_interface_mask & (1 << lmc)) == 0)
                        continue;
                    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(lmc));
                    ddr_dll_ctl3.cn78xx.fine_tune_mode = 1;
                    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(lmc),
                                              ddr_dll_ctl3.u64);
                }
            }

            /* Enable the trim circuit on the appropriate channels to
               adjust the DDR clock duty cycle for chips that support
               it. */
            if (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X) ||
                octeon_is_cpuid(OCTEON_CN73XX) ||
                octeon_is_cpuid(OCTEON_CNF75XX)) {
                cvmx_lmcx_phy_ctl_t lmc_phy_ctl;
                int loop_interface_num;

                for (loop_interface_num = 0; loop_interface_num<4; ++loop_interface_num) {
                    if ((ddr_interface_mask & (1 << loop_interface_num)) == 0)
                        continue;

                    lmc_phy_ctl.u64 = cvmx_read_csr(CVMX_LMCX_PHY_CTL(loop_interface_num));

                    if (octeon_is_cpuid(OCTEON_CNF75XX) || octeon_is_cpuid(OCTEON_CN73XX_PASS1_3)) {
                        lmc_phy_ctl.s.lv_mode = 0; /* Both LMCs */
                    } else {
                        lmc_phy_ctl.s.lv_mode = (~loop_interface_num) & 1; /* Odd LMCs = 0, Even LMCs = 1 */
                    }

                    ddr_print("LMC%d: PHY_CTL                                 : 0x%016llx\n",
                              loop_interface_num, (unsigned long long)lmc_phy_ctl.u64);
                    ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(loop_interface_num),
                                              lmc_phy_ctl.u64);
                }
            }
        } /* Do this once */

        /*
         * 5.9.6 LMC RESET Initialization
         *
         * NOTE: this is now done as the first step in init_octeon3_ddr3_interface,
         *       rather than the last step in clock init. This reorg allows restarting
         *       per-LMC initialization should problems be encountered, rather than
         *       being forced to resort to resetting the chip and starting all over.
         *
         *       Look for the code in octeon3_lmc.h: perform_lmc_reset().
         */
    }
#endif /* CONFIG_OCTEON_DISABLE_DDR3 */


    /* Start the DDR clock so that its frequency can be measured.  For
    ** some chips we must activate the memory controller with
    ** init_start to make the DDR clock start to run. */
    if ((!octeon_is_cpuid(OCTEON_CN6XXX))
        && (!octeon_is_cpuid(OCTEON_CN56XX))
        && (!octeon_is_cpuid(OCTEON_CNF7XXX))
        && (!octeon_is_cpuid(OCTEON_CN7XXX))) {
        cvmx_lmc_mem_cfg0_t mem_cfg0;
        mem_cfg0.u64 = 0;
        mem_cfg0.s.init_start   = 1;
        ddr_config_write_csr_node(node, CVMX_LMCX_MEM_CFG0(ddr_interface_num), mem_cfg0.u64);
        cvmx_read_csr_node(node, CVMX_LMCX_MEM_CFG0(ddr_interface_num));
    }

    set_ddr_clock_initialized(node, ddr_interface_num, 1);
    return(0);
}

static void octeon_ipd_delay_cycles(uint64_t cycles)
{
    uint64_t start = cvmx_read_csr(CVMX_IPD_CLK_COUNT);
    while (start + cycles > cvmx_read_csr(CVMX_IPD_CLK_COUNT))
        ;
}

static void octeon_ipd_delay_cycles_o3(uint64_t cycles)
{
    uint64_t start = cvmx_read_csr(CVMX_FPA_CLK_COUNT);
    while (start + cycles > cvmx_read_csr(CVMX_FPA_CLK_COUNT))
        ;
}

uint32_t measure_octeon_ddr_clock(uint32_t cpu_id,
				  const ddr_configuration_t *ddr_configuration,
				  uint32_t cpu_hertz,
				  uint32_t ddr_hertz,
				  uint32_t ddr_ref_hertz,
				  int ddr_interface_num,
				  uint32_t ddr_interface_mask,
				  int node)
{
    uint64_t core_clocks;
    uint64_t ddr_clocks;
    uint64_t calc_ddr_hertz;

    if (ddr_configuration) {
        if (initialize_ddr_clock(cpu_id,
                                 ddr_configuration,
                                 cpu_hertz,
                                 ddr_hertz,
                                 ddr_ref_hertz,
                                 ddr_interface_num,
                                 ddr_interface_mask,
                                 node) != 0)
            return 0;
    }

    /* Dynamically determine the DDR clock speed */
    if (OCTEON_IS_OCTEON2() || octeon_is_cpuid(OCTEON_CN70XX)) {
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT);
        ddr_clocks = cvmx_read_csr(CVMX_LMCX_DCLK_CNT(ddr_interface_num));
        /* How many cpu cycles to measure over */
        octeon_ipd_delay_cycles(100000000);
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT) - core_clocks;
        ddr_clocks = cvmx_read_csr(CVMX_LMCX_DCLK_CNT(ddr_interface_num)) - ddr_clocks;
        calc_ddr_hertz = ddr_clocks * cvmx_clock_get_rate(CVMX_CLOCK_IPD) / core_clocks;
    } else if (octeon_is_cpuid(OCTEON_CN7XXX)) {
        core_clocks = cvmx_read_csr_node(node, CVMX_FPA_CLK_COUNT);
        ddr_clocks = cvmx_read_csr_node(node, CVMX_LMCX_DCLK_CNT(ddr_interface_num));
        octeon_ipd_delay_cycles_o3(100000000); /* How many cpu cycles to measure over */
        core_clocks = cvmx_read_csr_node(node, CVMX_FPA_CLK_COUNT) - core_clocks;
        ddr_clocks = cvmx_read_csr_node(node, CVMX_LMCX_DCLK_CNT(ddr_interface_num)) - ddr_clocks;
        calc_ddr_hertz = ddr_clocks * cvmx_clock_get_rate_node(node, CVMX_CLOCK_IPD) / core_clocks;
    } else {
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT);
        /* ignore overflow, starts counting when we enable the controller */
        ddr_clocks = cvmx_read_csr(CVMX_LMCX_DCLK_CNT_LO(ddr_interface_num));
        octeon_ipd_delay_cycles(100000000); /* How many cpu cycles to measure over */
        core_clocks = cvmx_read_csr(CVMX_IPD_CLK_COUNT) - core_clocks;
        ddr_clocks = cvmx_read_csr(CVMX_LMCX_DCLK_CNT_LO(ddr_interface_num)) - ddr_clocks;
        calc_ddr_hertz = ddr_clocks * cpu_hertz / core_clocks;
    }

    debug_print("core clocks: %llu, ddr clocks: %llu, calc rate: %llu\n",
                core_clocks, ddr_clocks, calc_ddr_hertz);
    ddr_print("LMC%d: Measured DDR clock: %d, cpu clock: %u, ddr clocks: %llu\n",
              ddr_interface_num, calc_ddr_hertz, cpu_hertz, ddr_clocks);

#ifdef CVMX_BUILD_FOR_UBOOT
    /* Check for unreasonable settings. */
    if (calc_ddr_hertz < 10000) {
# if defined(CONFIG_OCTEON_EBB6300)
        error_print("DDR clock misconfigured. Asking MCU to power cycle Octeon.\n");
        twsii_mcu_read(8);  /* MCU from 5/10 required */
# endif
        cvmx_wait(8000000 * 100);
        error_print("DDR clock misconfigured on interface %d.  Resetting...\n",
		    ddr_interface_num);
        do_reset (NULL, 0, 0, NULL);
    }

    {
        char tmp[32];
        sprintf(tmp, "DDR%5lld", divide_nint(calc_ddr_hertz*2, 1000000));
        octeon_led_str_write(tmp);
    }
#else
    /* Check for unreasonable settings. */
    if (calc_ddr_hertz == 0) {
        error_print("DDR clock misconfigured. Exiting.\n");
        exit(1);
    }
#endif
    return calc_ddr_hertz;
}

#define tlv_debug_print(...) /* Careful... This could be too early to print */

#if (defined(CONFIG_OCTEON_EEPROM_TUPLES) || defined(CONFIG_CMD_EEPROM)) \
    || (!defined(__U_BOOT__) && !defined(BDK))
/***************** EEPROM TLV support functions ******************/

int  octeon_tlv_eeprom_get_next_tuple(uint8_t dev_addr, uint16_t addr,
				      uint8_t *buf_ptr, uint32_t buf_len)
{
    octeon_eeprom_header_t *tlv_hdr_ptr = (void *)buf_ptr;
    uint16_t checksum = 0;
    unsigned int i;


    if (buf_len < sizeof(octeon_eeprom_header_t)) {
        tlv_debug_print("ERROR: buf_len too small: %d, must be at least %d\n",
                        buf_len, sizeof(octeon_eeprom_header_t));
        return -1;
    }
#ifdef __U_BOOT__
    /* TLV EEPROM is always on Bus# 0, node 0. Other node EEPROM unused */
    i2c_set_bus_num( node_bus_to_i2c_bus(0, 0));
    if (eeprom_read(dev_addr, addr, buf_ptr, sizeof(octeon_eeprom_header_t))) {
        tlv_debug_print("ERROR: could not read TLV header starting at offset %d\n",
                        addr);
        return -1;
    }
    for (i = 0; i < sizeof(octeon_eeprom_header_t); i++)
        checksum += buf_ptr[i];
    buf_ptr += sizeof(octeon_eeprom_header_t);
    addr += sizeof(octeon_eeprom_header_t);
#else
    for (i = 0; i < sizeof(octeon_eeprom_header_t); i++) {
        *buf_ptr = (uint8_t)cvmx_twsix_read_ia16(0, dev_addr, addr++, 1);
        tlv_debug_print("Read: 0x%02x\n", *buf_ptr);
        checksum += *buf_ptr++;
    }
#endif
    /* Fix endian issues - data structure in EEPROM is big endian */
    tlv_hdr_ptr->length = cvmx_be16_to_cpu(tlv_hdr_ptr->length);
    tlv_hdr_ptr->type = cvmx_be16_to_cpu(tlv_hdr_ptr->type);
    tlv_hdr_ptr->checksum = cvmx_be16_to_cpu(tlv_hdr_ptr->checksum);

    if (tlv_hdr_ptr->type == EEPROM_END_TYPE)
        return -1;

    tlv_debug_print("TLV header at addr 0x%x: type: 0x%x, len: 0x%x, maj_version: %d, min_version: %d, checksum: 0x%x\n",
                    addr, tlv_hdr_ptr->type, tlv_hdr_ptr->length,
                    tlv_hdr_ptr->major_version, tlv_hdr_ptr->minor_version,
                    tlv_hdr_ptr->checksum);

    /* Do basic header check to see if we should continue */
    if (tlv_hdr_ptr->length > OCTEON_EEPROM_MAX_TUPLE_LENGTH ||
        tlv_hdr_ptr->length < sizeof(octeon_eeprom_header_t)) {
        tlv_debug_print("Invalid tuple type/length: type: 0x%x, len: 0x%x\n",
                        tlv_hdr_ptr->type, tlv_hdr_ptr->length);
        return -1;
    }
    if (buf_len < tlv_hdr_ptr->length) {
        tlv_debug_print("Error: buffer length too small.\n");
        return -1;
    }

    /* Read rest of tuple into buffer */
#ifdef __U_BOOT__
    eeprom_read(dev_addr, addr, buf_ptr,
                (int)tlv_hdr_ptr->length - sizeof(octeon_eeprom_header_t));
    for (i = 0;
         i < (int)tlv_hdr_ptr->length - sizeof(octeon_eeprom_header_t);
         i++)
        checksum += *buf_ptr++;
#else
    for (i = 0;
         i < (int)tlv_hdr_ptr->length - sizeof(octeon_eeprom_header_t);
         i++) {
        *buf_ptr = (uint8_t)cvmx_twsix_read_ia16(0, dev_addr, addr++, 1);
        checksum += *buf_ptr++;
    }
#endif
    checksum -= (tlv_hdr_ptr->checksum & 0xff) + (tlv_hdr_ptr->checksum >> 8);

    if (checksum != tlv_hdr_ptr->checksum) {
        tlv_debug_print("Checksum mismatch: computed 0x%x, found 0x%x\n",
                        checksum, tlv_hdr_ptr->checksum);
        return -1;
    }

    return tlv_hdr_ptr->length;
}

/* Find tuple based on type and major version.  if not found, returns next
 * available address with bit 31 set (negative).
 * If positive value is returned the tuple was found
 * if supplied version is zero, matches any version
 */
int octeon_tlv_get_tuple_addr(uint8_t dev_addr, uint16_t type,
			      uint8_t major_version, uint8_t *eeprom_buf,
			      uint32_t buf_len)
{

    octeon_eeprom_header_t *tlv_hdr_ptr = (void *)eeprom_buf;
    int cur_addr = 0;
    int len;

#ifdef CONFIG_EEPROM_TLV_BASE_ADDRESS
    cur_addr = CONFIG_EEPROM_TLV_BASE_ADDRESS; /* Skip to the beginning of the TLV region. */
#endif

    while ((len = octeon_tlv_eeprom_get_next_tuple(dev_addr, cur_addr,
                                                   eeprom_buf, buf_len)) > 0) {
        /* Checksum already validated here, endian swapping of header
         * done in  octeon_tlv_eeprom_get_next_tuple
         */
        /* Check to see if we found matching */
        if (type == tlv_hdr_ptr->type &&
            (!major_version ||
             (major_version == tlv_hdr_ptr->major_version)))
            return cur_addr;

        cur_addr += len;
    }
    /* Return next available address with bit 31 set */
    cur_addr |= (1 << 31);

    return cur_addr;
}
#endif


#if 0
# if !defined(CVMX_BUILD_FOR_UBOOT)		||      \
    defined(CONFIG_OCTEON_EBB7804)		||      \
    defined(CONFIG_OCTEON_EBB7800)		||      \
    defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	||      \
    defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)

/////////////////////////////////////
// DRAM FIXUPS
//
// #1 is for RDIMM 2Rx4 1-slot only, at this time...

static void ebb780x_fixup_1_dram_odt_2rank_configuration(dimm_odt_config_t odt[4])
{
    int dimm = 0;
    cvmx_lmcx_modereg_params1_t *modereg1;
    cvmx_lmcx_modereg_params2_t *modereg2;

    //setup_modereg_params1_2rank_1slot(&odt[dimm].odt_mask1); /* LMCX_MODEREG_PARAMS1 */
    modereg1 = &odt[dimm].modereg_params1;
    modereg1->cn78xx.rtt_wr_00       = ddr4_rttwr_240ohm/* default currently ddr4_rttwr_240ohm */;
    modereg1->cn78xx.rtt_wr_01       = ddr4_rttwr_240ohm/* default currently ddr4_rttwr_240ohm */;

    //setup_modereg_params2_2rank_1slot(&odt[dimm].odt_mask2); /* LMCX_MODEREG_PARAMS2 */
    modereg2 = &odt[dimm].modereg_params2;

    modereg2->cn78xx.rtt_park_00    = ddr4_rttpark_40ohm/* default currently ddr4_rttpark_120ohm */;
    modereg2->cn78xx.rtt_park_01    = ddr4_rttpark_40ohm/* default currently ddr4_rttpark_120ohm */;

    odt[dimm].qs_dic = ddr4_rodt_ctl_120_ohm; /* RODT_CTL */
}


// DLL read Offset table for the #122 INPHI DIMMs
static const int8_t ebb780x_inphi_122_dll_read_offset [4][9] =
    { {-5, 0, 5, 5, 0,  0, 5, 0, 0}, // LMC 0
      {-5, 5, 0, 5, 0,  0, 5, 0, 0}, // LMC 1
      {-5, 5,10, 5, 0, -5, 5, 0, 0}, // LMC 2
      { 5, 5, 5, 5, 0,-10, 5, 0, 0}  // LMC 3
};

static void ebb780x_fixup_dram_config(ddr_configuration_t *cfg)
{
    int lmc;
    int spd_dimm_type = 0xff & read_spd(&cfg->dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_MODULE_TYPE);
    int spd_rdimm = (spd_dimm_type == 1);

    /* FIXME: FIXUPS
     * Here is where we can fix up configuration values dependent on the SPD contents.
     * For example, we may want to change MODEREG_PARAMSx fields like rtt_wr dependent on the DIMM.
     */
    int spd_package = 0xff & read_spd(&cfg->dimm_config_table[0], 0, DDR4_SPD_PACKAGE_TYPE);
    int spd_org     = 0xff & read_spd(&cfg->dimm_config_table[0], 0, DDR4_SPD_MODULE_ORGANIZATION);
    int num_ranks   = 1 +  ((spd_org >> 3) & 0x7);
    int dram_width  = 4 << ((spd_org >> 0) & 0x7);
    char part_number[21] = {0};

    // look for RDIMM && 2Rx4 && stacked die
    if (spd_rdimm && (num_ranks == 2) && (dram_width == 4) && ((spd_package & 0xf3) == 0x91))
    {
	// fixup ONLY the items that need to be different
	//printf("CONFIG_EBB7804: fixups for RDIMM 2Rx4 stacked die\n");

	// do each LMC
	for (lmc = 0; lmc < 4; lmc++) {
	    //fixup_dram_custom_lmc_config(&cfg[lmc].custom_lmc_config);
	    //fixup_dram_odt_1rank_configuration(cfg[lmc].odt_1rank_config);
	    ebb780x_fixup_1_dram_odt_2rank_configuration(cfg[lmc].odt_2rank_config);
	    //fixup_dram_odt_4rank_configuration(cfg[lmc].odt_4rank_config);
	}
    }

    // Check for specific DIMM part number and set a different DLL Read Offset table
    get_dimm_part_number(part_number, &cfg->dimm_config_table[0], 0);
    if (strcmp(part_number, "36ASF2G72PZ-2G1A2") == 0)
    {
	//printf("CONFIG_EBB7804: fixup for RDIMM #122 & #121 part number '%s'\n", part_number);

	// do each LMC
	for (lmc = 0; lmc < 4; lmc++) {
	    cfg[lmc].custom_lmc_config.dll_read_offset = &ebb780x_inphi_122_dll_read_offset[lmc][0];
	}
    }
}
#endif
#endif

# if !defined(CVMX_BUILD_FOR_UBOOT)		||      \
    defined(CONFIG_OCTEON_EBB6100)		||      \
    defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	||      \
    defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)

// DLL read Offset table for the #122 INPHI DIMMs
static const int8_t ebb6100_dll_read_offset [9] =
    {-13,-13,-13,-13,-13,-13,-13,-13,-13};  // LMC 0
#endif

int octeon_ddr_initialize(uint32_t cpu_id,
			  uint32_t cpu_hertz,
			  uint32_t ddr_hertz,
			  uint32_t ddr_ref_hertz,
			  uint32_t ddr_interface_mask,
			  const ddr_configuration_t *ddr_configuration,
			  uint32_t *measured_ddr_hertz,
			  int board_type,
			  int board_rev_maj,
			  int board_rev_min,
			  int node)
{
    uint32_t ddr_config_valid_mask = 0;
    int memsize_mbytes = 0;
    char *eptr;
    int retval;
    int interface_index;
    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;
    uint32_t ddr_max_speed = 667000000;
    uint32_t calc_ddr_hertz = -1;

#if 1
    PRINT_CURSP("octeon_ddr_initialize");
#endif

#ifdef __U_BOOT__
#ifndef CONFIG_OCTEON_OCX
    node = 0;
#endif
    if ((getenv("ddr_verbose") != NULL) || (getenv("ddr_prompt") != NULL))
        gd->flags |= GD_FLG_DDR_VERBOSE;

#ifdef DDR_VERBOSE
    gd->flags |= GD_FLG_DDR_VERBOSE;
#endif

    if (getenv("ddr_trace_init") != NULL) {
        error_print("Parameter ddr_trace_init found in environment.\n");
        gd->flags |= GD_FLG_DDR_TRACE_INIT;
        gd->flags |= GD_FLG_DDR_VERBOSE;
    }

# ifdef CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2
    /* If CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2 is defined then this
     * code can be running out of L2 cache.  In that case the overhead
     * for checking environment variables is hardly detectable.
     * Therefore, default ddr_debug mode to enabled.
     */
    gd->flags |= GD_FLG_DDR_DEBUG;
# endif

    switch (getenv_ulong("ddr_debug", 10, (u32)-1)) {
    case 0:
        gd->flags &= ~GD_FLG_DDR_DEBUG;
        error_print("Parameter ddr_debug clear in environment\n");
        break;
    case (u32)-1:
        break;
    default:
        error_print("Parameter ddr_debug set in environment\n");
        gd->flags |= GD_FLG_DDR_DEBUG;
        gd->flags |= GD_FLG_DDR_VERBOSE;
        break;
    }
    if (getenv("ddr_prompt"))
        gd->flags |= GD_FLG_DDR_PROMPT;

    /* Force ddr_verbose for failsafe debugger */
    if (gd->flags & GD_FLG_FAILSAFE_MODE)
        gd->flags |= GD_FLG_DDR_VERBOSE;

# ifdef DDR_DEBUG
    gd->flags |= GD_FLG_DDR_DEBUG;
    /* Keep verbose on while we are still debugging. */
    gd->flags |= GD_FLG_DDR_VERBOSE;
# endif
#endif  /* __U_BOOT__ */

#ifndef OCTEON_SDK_VERSION_STRING
# define OCTEON_SDK_VERSION_STRING "Development Build"
#endif

#if defined(unix) || defined(BDK)
    /* For the Moonshot board we need to take the I2C MUX out of reset to
     * talk to the SPDs.
     */
    if (board_type == CVMX_BOARD_TYPE_MOONSHOT) {
        if (ddr_verbose())
            printf("Taking i2c multi-master mux out of reset\n");
        cvmx_gpio_cfg(7, 1);
        cvmx_gpio_clear(1 << 7);
        cvmx_wait_usec(1000);
        cvmx_gpio_set(1 << 7);
        cvmx_wait_usec(10000);
# ifndef BDK
        cvmx_twsix_write_ia(0, 0x70, 1, 1, 1, 0x10);
        cvmx_wait_usec(1000);
        cvmx_twsix_write_ia(0, 0x70, 1, 1, 1, 0x04);
# endif
    }
#endif

    ddr_print(OCTEON_SDK_VERSION_STRING": $Revision: 167267 $\n");

    if ((octeon_is_cpuid(OCTEON_CN61XX) || octeon_is_cpuid(OCTEON_CNF71XX))
        && ddr_max_speed > 533333333)
        ddr_max_speed = 533333333; // FIXME? was 553333333???
    else if (octeon_is_cpuid(OCTEON_CN7XXX)) {
        /* Override speed restrictions to support internal testing. */
        ddr_max_speed = 1210000000;
    }

    if (ddr_hertz > ddr_max_speed) {
        error_print("DDR clock speed %u exceeds maximum supported DDR speed,"
                    " reducing to %uHz\n",
                    ddr_hertz, ddr_max_speed);
        ddr_hertz = ddr_max_speed;
    }

    if (OCTEON_IS_OCTEON3()) { // restrict check
        if (ddr_hertz > cpu_hertz) {
            error_print("\nFATAL ERROR: DDR speed %u exceeds CPU speed %u, exiting...\n\n",
                        ddr_hertz, cpu_hertz);
            return -1;
        }
    }

    /* Enable L2 ECC */
    eptr = getenv("disable_l2_ecc");
    if (eptr) {
        puts("Disabling L2 ECC based on disable_l2_ecc environment variable\n");
        if (OCTEON_IS_OCTEON2() || OCTEON_IS_OCTEON3()) {
            cvmx_l2c_ctl_t l2c_val;
            l2c_val.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
            l2c_val.s.disecc = 1;
            ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_val.u64);
        } else {
            ddr_config_write_csr(CVMX_L2D_ERR, 0);
            ddr_config_write_csr(CVMX_L2T_ERR, 0);
        }
    } else {
        if (OCTEON_IS_OCTEON2() || OCTEON_IS_OCTEON3()) {
            cvmx_l2c_ctl_t l2c_val;
            l2c_val.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
            l2c_val.s.disecc = 0;
            ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_val.u64);
        }  else {
            ddr_config_write_csr(CVMX_L2D_ERR, 1);
            ddr_config_write_csr(CVMX_L2T_ERR, 1);
        }
    }

#if !defined(CONFIG_OCTEON_NAND_STAGE2)		&&      \
    !defined(CONFIG_OCTEON_EMMC_STAGE2)		&&      \
    !defined(CONFIG_OCTEON_AUTHENTIK_STAGE2)	&&      \
    !defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)&&      \
    !defined(CONFIG_OCTEON_SPI_BOOT)
    {
        /* Init the L2C, must be done before DRAM access so that we
         * know L2 is empty
         */
#if defined(__U_BOOT__)
        if (!(gd->flags & GD_FLG_RAM_RESIDENT) &&
            !(gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE))
#endif
        {
            eptr = getenv("disable_l2_index_aliasing");
            if (eptr) {
                puts("L2 index aliasing disabled.\n");
                if (!OCTEON_IS_OCTEON1PLUS()) {
                    cvmx_l2c_ctl_t l2c_val;
                    l2c_val.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
                    l2c_val.s.disidxalias = 1;
                    ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_val.u64);
                } else {
                    cvmx_l2c_cfg_t l2c_cfg;
                    l2c_cfg.u64 = cvmx_read_csr_node(node, CVMX_L2C_CFG);
                    l2c_cfg.s.idxalias = 0;
                    ddr_config_write_csr_node(node, CVMX_L2C_CFG,  l2c_cfg.u64);
                }
            } else {
                /* Enable L2C index aliasing */
                if (!OCTEON_IS_OCTEON1PLUS()) {
                    cvmx_l2c_ctl_t l2c_val;
                    l2c_val.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
                    l2c_val.s.disidxalias = 0;
                    ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_val.u64);
                } else {
                    cvmx_l2c_cfg_t l2c_cfg;
                    l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);
                    l2c_cfg.s.idxalias = 1;
                    ddr_config_write_csr(CVMX_L2C_CFG,  l2c_cfg.u64);
                }
            }
        }
    }
#endif

    /* Enable LMC to L2 early fill */
    if (OCTEON_IS_OCTEON2()) {
        int ef_cnt;
        cvmx_l2c_ctl_t l2c_val;
        int ef_cnt_base = 7;
        char *s;
        int ddr_interface_64b = 1;  /* Octeon II Default: 64bit interface width */

        if (custom_lmc_config->mode32b)
            ddr_interface_64b = 0;

        if ((s = lookup_env_parameter("ddr_interface_64b")) != NULL)
            ddr_interface_64b = simple_strtoul(s, NULL, 0);

        ef_cnt_base = ddr_interface_64b ? 7 : 14;

        /* LMC early fill count Specifies the number of cycles after the
         * first LMC fill cycle to wait before requesting a fill on the
         * RSC/RSD bus.
         */
        /* 7 dclks (we've received 1st out of 8 by the time we start counting) */
        ef_cnt = ef_cnt_base * cvmx_clock_get_rate(CVMX_CLOCK_RCLK) / ddr_hertz;
        /* + 1 rclk if the dclk and rclk edges don't stay in the same position */
        ef_cnt++; /* Always add one so we don't have a special case where the clock line up, which is unlikely */
        /* + 2 rclk synchronization uncertainty */
        ef_cnt += 2;
        /* - 3 rclks to recognize first write */
        ef_cnt -= 3;
        /* + 3 rclks to perform first write */
        ef_cnt += 3;
        /* - 9 rclks minimum latency from counter expire to final fbf read */
        ef_cnt -= 9;
        if ((ef_cnt >=  0) && (ef_cnt < 127)) {
            l2c_val.u64 = cvmx_read_csr(CVMX_L2C_CTL);
            l2c_val.cn63xx.ef_ena = 1;
            l2c_val.cn63xx.ef_cnt = ef_cnt;
            ddr_config_write_csr(CVMX_L2C_CTL, l2c_val.u64);
        } else {
            printf("Disabling L2 to LMC early fill due to range violation (ef_cnt=%d)\n",
                   ef_cnt);
        }
    }

#ifndef CONFIG_OCTEON_DISABLE_DDR3
    if (OCTEON_IS_OCTEON3()) {
        /*

        rdf_cnt: Defines the sample point of the LMC response data in
        the DDR-clock/core-clock crossing.  For optimal
        performance set to 10 * (DDR-clock period/core-clock
        period) - 1.  To disable set to 0. All other values
        are reserved.
        */

        char *s;
        cvmx_l2c_ctl_t l2c_ctl;
        uint64_t rdf_cnt;

        l2c_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
        /* It is more convenient to compute the ratio using clock
           frequencies rather than clock periods. */
        rdf_cnt = (((uint64_t) 10 * cpu_hertz) / ddr_hertz) - 1;
        rdf_cnt = rdf_cnt<256 ? rdf_cnt : 255;
        l2c_ctl.cn78xx.rdf_cnt = rdf_cnt;

        if ((s = lookup_env_parameter("early_fill_count")) != NULL)
            l2c_ctl.cn78xx.rdf_cnt = simple_strtoul(s, NULL, 0);

        ddr_print("%-45s : %d, cpu_hertz:%d, ddr_hertz:%d\n", "EARLY FILL COUNT  ",
                  l2c_ctl.cn78xx.rdf_cnt, cpu_hertz, ddr_hertz);
        ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_ctl.u64);
    }

#endif /* CONFIG_OCTEON_DISABLE_DDR3 */
#ifndef __U_BOOT__
    /* Check to see if we should limit the number of L2 ways. */
    eptr = getenv("limit_l2_ways");
    if (eptr) {
        int ways = simple_strtoul(eptr, NULL, 10);
        limit_l2_ways(ways, 1);
    }
#endif
    /* Check for lower DIMM socket populated */
    for (interface_index = 0; interface_index < 4; ++interface_index) {

        if ((ddr_interface_mask & (1<<interface_index))
            && validate_dimm(&ddr_configuration[(int)interface_index].dimm_config_table[0], 0))
            ddr_config_valid_mask |= (1 << interface_index);
    }

    if (octeon_is_cpuid(OCTEON_CN68XX) || octeon_is_cpuid(OCTEON_CN78XX)) {
        int four_lmc_mode = 1;

        char *s;

#if defined(__U_BOOT__)
        if (gd->flags & GD_FLG_FAILSAFE_MODE)
            four_lmc_mode = 0;
#endif

        /* Pass 1.0 disable four LMC mode.
         *  See errata (LMC-15811)
         */
        if (octeon_is_cpuid(OCTEON_CN68XX_PASS1_0))
            four_lmc_mode = 0;

        if ((s = getenv("ddr_four_lmc")) != NULL) {
            four_lmc_mode = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. ddr_four_lmc = %d\n",
                        four_lmc_mode);
        }

        if (!four_lmc_mode) {
            puts("Forcing two-LMC Mode.\n");
            ddr_config_valid_mask &= ~(3<<2); /* Invalidate LMC[2:3] */
        }
    } else if (octeon_is_cpuid(OCTEON_CN73XX)) {
        int one_lmc_mode = 0;
	char *s;

        if ((s = getenv("ddr_one_lmc")) != NULL) {
            one_lmc_mode = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. ddr_one_lmc = %d\n",
                        one_lmc_mode);
        }

        if (one_lmc_mode) {
            puts("Forcing one-LMC Mode.\n");
            ddr_config_valid_mask &= ~(1<<1); /* Invalidate LMC[1:3] */
        }
    }

    if (!ddr_config_valid_mask) {
        puts("ERROR: No valid DIMMs detected on any DDR interface.\n");
        return -1;
    }

#if 0
#if !defined(CVMX_BUILD_FOR_UBOOT)		||      \
    defined(CONFIG_OCTEON_EBB7804)		||      \
    defined(CONFIG_OCTEON_EBB7800)		||      \
    defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	||      \
    defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)

    if ((board_type == CVMX_BOARD_TYPE_EBB7800)  ||
        (board_type == CVMX_BOARD_TYPE_EBB7804)) {
        ebb780x_fixup_dram_config(ddr_configuration);
    }
#endif
#endif

    /* We measure the DDR frequency by counting DDR clocks.  We can
     * confirm or adjust the expected frequency as necessary.  We use
     * the measured frequency to make accurate timing calculations
     * used to configure the controller.
     */
    for (interface_index = 0; interface_index < 4; ++interface_index) {
        uint32_t tmp_hertz;

        if (! (ddr_config_valid_mask & (1 << interface_index)))
            continue;

        try_again:

	// only check for alternate refclk wanted on chips that support it
        if ((octeon_is_cpuid(OCTEON_CN73XX)) ||
	    (octeon_is_cpuid(OCTEON_CNF75XX)) ||
	    (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)))
	{
	    // only need do this if we are LMC0
            if (interface_index == 0) {
                cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;

                ddr_pll_ctl.u64 = cvmx_read_csr(CVMX_LMCX_DDR_PLL_CTL(0));

                // if we are asking for 100 MHz refclk, we can only get it via alternate, so switch to it
                if (ddr_ref_hertz == 100000000) {
                    ddr_pll_ctl.cn78xx.dclk_alt_refclk_sel = 1;
                    ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);
                    cvmx_wait_usec(1000); // wait 1 msec
                } else {
                    // if we are NOT asking for 100MHz, then reset to (assumed) 50MHz and go on
                    ddr_pll_ctl.cn78xx.dclk_alt_refclk_sel = 0;
                    ddr_config_write_csr(CVMX_LMCX_DDR_PLL_CTL(0), ddr_pll_ctl.u64);
                    cvmx_wait_usec(1000); // wait 1 msec
                }
            }
        } else {
            if (ddr_ref_hertz == 100000000) {
                ddr_print("N%d: DRAM init: requested 100 MHz refclk NOT SUPPORTED\n", node);
                ddr_ref_hertz = 50000000; /* FIXME: Need a place to get this from. */
            }
        }

        tmp_hertz = measure_octeon_ddr_clock(cpu_id,
                                             &ddr_configuration[interface_index],
                                             cpu_hertz,
                                             ddr_hertz,
                                             ddr_ref_hertz,
                                             interface_index,
                                             ddr_config_valid_mask,
                                             node);

        // only check for alternate refclk acquired on chips that support it
        if ((octeon_is_cpuid(OCTEON_CN73XX))  ||
            (octeon_is_cpuid(OCTEON_CNF75XX)) ||
            (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)))
        {
            // if we are LMC0 and we are asked for 100 MHz refclk,
            // we must be sure it is available
            // If not, we print an error message, set to 50MHz, and go on...
            if ((interface_index == 0) && (ddr_ref_hertz == 100000000)) {
                // validate that the clock returned is close enough to the clock desired
                // FIXME: is 5% close enough?
                int hertz_diff = _abs((int)tmp_hertz - (int)ddr_hertz);
                if (hertz_diff > ((int)ddr_hertz * 5 / 100)) { // nope, diff is greater than than 5%
                    ddr_print("N%d: DRAM init: requested 100 MHz refclk NOT FOUND\n", node);
#ifdef __U_BOOT__
                    ddr_ref_hertz = gd->arch.ddr_ref_hertz;
#else
                    ddr_ref_hertz = 50000000; /* FIXME: Need a place to get this from. */
#endif
                    set_ddr_clock_initialized(node, 0, 0); // clear the flag before trying again!!
                    goto try_again;
                } else {
                    ddr_print("N%d: DRAM Init: requested 100 MHz refclk FOUND and SELECTED.\n", node);
                }
            }
        }

        if (tmp_hertz > 0)
            calc_ddr_hertz = tmp_hertz;
        ddr_print("LMC%d: measured speed: %u hz\n",
                  interface_index, tmp_hertz);
    }

    if (measured_ddr_hertz)
        *measured_ddr_hertz = calc_ddr_hertz;

    memsize_mbytes = 0;
    for (interface_index = 0; interface_index < 4; ++interface_index) {
        if (! (ddr_config_valid_mask & (1 << interface_index)))
            continue;

        retval = init_octeon_dram_interface(cpu_id,
                                            &ddr_configuration[interface_index],
                                            calc_ddr_hertz, /* Configure using measured value */
                                            cpu_hertz,
                                            ddr_ref_hertz,
                                            board_type,
                                            board_rev_maj,
                                            board_rev_min,
                                            interface_index,
                                            ddr_config_valid_mask,
                                            node);
        if (retval > 0)
            memsize_mbytes += retval;
    }

    if (memsize_mbytes == 0)
        /* All interfaces failed to initialize, so return error */
        return -1;

#ifndef CONFIG_OCTEON_DISABLE_DDR3
    // switch over to DBI mode only for chips that support it, and enabled by envvar
    if ((octeon_is_cpuid(OCTEON_CN73XX))  ||
        (octeon_is_cpuid(OCTEON_CNF75XX)) ||
        (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)))
    {
        eptr = getenv("ddr_dbi_switchover");
        if (eptr) {
            ddr_print("DBI Switchover starting...\n");
            cvmx_dbi_switchover(node, cpu_id);
            printf("DBI Switchover finished.\n");
        }
    }
#endif
#ifndef CONFIG_OCTEON_DISABLE_DDR3
    // call HW-assist tuning here on chips that support it...
    // static int cvmx_maybe_tune_node(int node, uint32_t cpuid, uint32_t ddr_speed);
    if ((octeon_is_cpuid(OCTEON_CN73XX))  ||
        (octeon_is_cpuid(OCTEON_CNF75XX)) ||
        (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)))
    {
        cvmx_maybe_tune_node(node, cpu_id, calc_ddr_hertz);
    }
#endif

    eptr = getenv("limit_dram_mbytes");
    if (eptr) {
        unsigned int mbytes = simple_strtoul(eptr, NULL, 10);
        if (mbytes > 0) {
            memsize_mbytes = mbytes;
            printf("Limiting DRAM size to %d MBytes based on limit_dram_mbytes env. variable\n",
                   mbytes);
        }
    }

    ddr_print("LMC Initialization complete. Total DRAM %d MB\n", memsize_mbytes);
    return memsize_mbytes;
}

#if defined(DEBUG) && !defined(CONFIG_OCTEON_DISABLE_DDR2)
/* Serial Presence Detect (SPD) for DDR2 SDRAM - JEDEC Standard No. 21-C */
/* ===================================================================== */

const char *ddr2_spd_strings[] = {
    /* Byte                                                                   */
    /* Number  Function                                                       */
    /* ======  ============================================================== */
    /*  0     */ "Number of Serial PD Bytes written during module production",
    /*  1     */ "Total number of Bytes in Serial PD device",
    /*  2     */ "Fundamental Memory Type (FPM, EDO, SDRAM, DDR, DDR2)",
    /*  3     */ "Number of Row Addresses on this assembly",
    /*  4     */ "Number of Column Addresses on this assembly",
    /*  5     */ "Number of DIMM Ranks",
    /*  6     */ "Data Width of this assembly",
    /*  7     */ "Reserved",
    /*  8     */ "Voltage Interface Level of this assembly",
    /*  9     */ "SDRAM Cycle time at Maximum Supported CAS Latency (CL), CL=X",
    /* 10     */ "SDRAM Access from Clock (tAC)",
    /* 11     */ "DIMM configuration type (Non-parity, Parity or ECC)",
    /* 12     */ "Refresh Rate/Type (tREFI)",
    /* 13     */ "Primary SDRAM Width",
    /* 14     */ "Error Checking SDRAM Width",
    /* 15     */ "Reserved",
    /* 16     */ "SDRAM Device Attributes: Burst Lengths Supported",
    /* 17     */ "SDRAM Device Attributes: Number of Banks on SDRAM Device",
    /* 18     */ "SDRAM Device Attributes: CAS Latency",
    /* 19     */ "Reserved",
    /* 20     */ "DIMM Type Information",
    /* 21     */ "SDRAM Module Attributes",
    /* 22     */ "SDRAM Device Attributes: General",
    /* 23     */ "Minimum Clock Cycle at CLX-1",
    /* 24     */ "Maximum Data Access Time (tAC) from Clock at CLX-1",
    /* 25     */ "Minimum Clock Cycle at CLX-2",
    /* 26     */ "Maximum Data Access Time (tAC) from Clock at CLX-2",
    /* 27     */ "Minimum Row Precharge Time (tRP)",
    /* 28     */ "Minimum Row Active to Row Active delay (tRRD)",
    /* 29     */ "Minimum RAS to CAS delay (tRCD)",
    /* 30     */ "Minimum Active to Precharge Time (tRAS)",
    /* 31     */ "Module Rank Density",
    /* 32     */ "Address and Command Input Setup Time Before Clock (tIS)",
    /* 33     */ "Address and Command Input Hold Time After Clock (tIH)",
    /* 34     */ "Data Input Setup Time Before Clock (tDS)",
    /* 35     */ "Data Input Hold Time After Clock (tDH)",
    /* 36     */ "Write recovery time (tWR)",
    /* 37     */ "Internal write to read command delay (tWTR)",
    /* 38     */ "Internal read to precharge command delay (tRTP)",
    /* 39     */ "Memory Analysis Probe Characteristics",
    /* 40     */ "Extension of Byte 41 tRC and Byte 42 tRFC",
    /* 41     */ "SDRAM Device Minimum Active to Active/Auto Refresh Time (tRC)",
    /* 42     */ "SDRAM Min Auto-Ref to Active/Auto-Ref Command Period (tRFC)",
    /* 43     */ "SDRAM Device Maximum device cycle time (tCKmax)",
    /* 44     */ "SDRAM Device maximum skew between DQS and DQ signals (tDQSQ)",
    /* 45     */ "SDRAM Device Maximum Read DataHold Skew Factor (tQHS)",
    /* 46     */ "PLL Relock Time",
    /* 47-61  */ "IDD in SPD - To be defined",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /* 62     */ "SPD Revision",
    /* 63     */ "Checksum for Bytes 0-62",
    /* 64-71  */ "Manufacturers JEDEC ID Code",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /* 72     */ "Module Manufacturing Location",
    /* 73-90  */ "Module Part Number",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /* 91-92  */ "Module Revision Code",
    /*        */ "",
    /* 93-94  */ "Module Manufacturing Date",
    /*        */ "",
    /* 95-98  */ "Module Serial Number",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /* 99-127 */ "Manufacturers Specific Data",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
    /*        */ "",
};
#endif /* DEBUG */








/* Arrays of compiled in SPD values.  For flash based bootloaders, only the
** array(s) for the specific board is compiled in.  For oct-remote-boot, support
** for all boards is compiled in.
*/
#if !defined(CONFIG_OCTEON_DISABLE_DDR2)
# if defined(EBH5200_SPD_VALUES)
static uint8_t ebh5200_spd_values[128] = { EBH5200_SPD_VALUES };
# endif


# ifndef _MIPS_ARCH_OCTEON2
#  if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_BBGW_REF
static uint8_t bbgw_ref_spd_values[128] = { BBGW_REF_SPD_VALUES };
#  endif
# endif /* _MIPS_ARCH_OCTEON2 */
#endif /* CONFIG_OCTEON_DISABLE_DDR2 */
/* Disabled in the BDK to save space */
# if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_NIC2E)
static uint8_t octeon_nic2e_spd_values[128] = { OCTEON_NIC2E_SPD_VALUES };
# endif
# if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_SNIC10E)
static uint8_t octeon_snic10e_spd_values[128] = { OCTEON_SNIC10E_SPD_VALUES };
# endif
# if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_SNIC10E_61)
static uint8_t octeon_snic10e_61_spd_values[128] = { OCTEON_SNIC10E_61_SPD_VALUES };
# endif

#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB6800)
static uint8_t __attribute__((unused)) ebb6800_spd_values[0x92] = { WD3UE02GX818_1333L_CT_spd };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB6300)
static uint8_t __attribute__((unused)) ebb6300_spd_values[0x92] = { WD3RE02GX818V_1333L_PEI_spd };
#endif
/* Disabled in the BDK to save space */
#if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) \
	|| defined(CONFIG_OCTEON_EP6300C))
static uint8_t __attribute__((unused)) ep6300c_spd_values[0x92] = { WD3RE02GX818V_1333L_PEI_spd };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SFF6100)
static uint8_t __attribute__((unused)) sff6100_spd_values[0x92] = { SG572568FH8SZRN1_1600_spd };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_EVB7000)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
static uint8_t __attribute__((unused)) octeon_evb7000_spd_values[176] =
					{ OCTEON_EVB7000_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_TB7000)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
static uint8_t __attribute__((unused)) octeon_tb7000_spd_values[176] =
					{ OCTEON_TB7000_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_EVB7000_SFF)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_EVB7000_SFF_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
static uint8_t __attribute__((unused)) octeon_evb7000_sff_spd_values[128] =
					{ OCTEON_EVB7000_SFF_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_SFF7000)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)		|| \
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
static uint8_t __attribute__((unused)) octeon_sff7000_4gb_spd_values[128] =
					{ OCTEON_SFF7000_4GB_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)
static uint8_t __attribute__((unused)) octeon_clark_spd_values[256] =
					{ OCTEON_CLARK_SPD_VALUES };
#endif

#if !defined(CVMX_BUILD_FOR_UBOOT) ||			\
    defined(CONFIG_OCTEON_NAS7000_REF_EMMC_STAGE2) ||	\
    (defined(CONFIG_OCTEON_NAS7000_REF) && defined(unix))
static uint8_t __attribute__((unused)) octeon_nas7000_ref_spd_values[256] =
					{ OCTEON_NAS7000_REF_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) ||			\
    defined(CONFIG_OCTEON_EAP7000_REF_EMMC_STAGE2) ||	\
    defined(CONFIG_OCTEON_EAP7000_REF_SPI_STAGE2) ||	\
    (defined(CONFIG_OCTEON_EAP7000_REF) && defined(unix))
static uint8_t __attribute__((unused)) octeon_eap7000_ref_spd_values[256] =
					{ OCTEON_EAP7000_REF_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) ||			\
    defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2) ||	\
    defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2) ||	\
    (defined(CONFIG_OCTEON_WAVE2) && defined(unix))
static uint8_t __attribute__((unused)) octeon_wave2_spd_values[256] =
	{ OCTEON_WAVE2_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) ||				\
    defined(CONFIG_OCTEON_ROUTER7000_REF_EMMC_STAGE2) ||	\
    defined(CONFIG_OCTEON_ROUTER7000_REF_SPI_STAGE2)  ||	\
    (defined(CONFIG_OCTEON_ROUTER7000_REF) && defined(unix))
static uint8_t __attribute__((unused)) octeon_router7000_ref_spd_values[256] =
					{ OCTEON_ROUTER7000_REF_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_EBB7800)		|| \
    defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	|| \
    defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)
static uint8_t __attribute__((unused)) octeon_ebb7800_cfg0_spd_values[176] =
					{ OCTEON_EBB7800_CFG0_SPD_VALUES };
static uint8_t __attribute__((unused)) octeon_ebb7800_cfg1_spd_values[176] =
					{ OCTEON_EBB7800_CFG1_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SFF7800)
static uint8_t __attribute__((unused)) octeon_sff7800_cfg_spd_values[176] =
					{ OCTEON_SFF7800_CFG_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_TB7600)
# ifdef CONFIG_DISABLE_TB7600_DDR_TWSI
static uint8_t __attribute__((unused)) octeon_tb7600_cfg0_spd_values[176] =
					{ OCTEON_TB7600_CFG0_SPD_VALUES };
static uint8_t __attribute__((unused)) octeon_tb7600_cfg1_spd_values[176] =
					{ OCTEON_TB7600_CFG1_SPD_VALUES };
# endif
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
    defined(CONFIG_OCTEON_EBB7804)
static uint8_t __attribute__((unused)) octeon_ebb7804_cfg0_spd_values[256] =
					{ OCTEON_EBB7804_CFG0_SPD_VALUES };
static uint8_t __attribute__((unused)) octeon_ebb7804_cfg1_spd_values[256] =
					{ OCTEON_EBB7804_CFG1_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC73) || \
    defined(CONFIG_OCTEON_NIC23)
static uint8_t __attribute__((unused)) octeon_nic73_cfg0_spd_values[512] =
					{ OCTEON_NIC73_CFG0_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_COPPERHEAD)
static uint8_t __attribute__((unused)) octeon_copperhead_cfg0_spd_values[512] =
					{ OCTEON_COPPERHEAD_CFG0_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC25E)
static uint8_t __attribute__((unused)) octeon_nic25e_cfg0_spd_values[512] =
					{ OCTEON_NIC25E_CFG0_SPD_VALUES };
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC225E)
static uint8_t __attribute__((unused)) octeon_nic225e_cfg0_spd_values[512] =
					{ OCTEON_NIC225E_CFG0_SPD_VALUES };
#endif

/* Table where all the DDR configurations are defined.  This is shared between
 * u-boot and oct-remote-boot.  oct-remote-boot has all boards compiled in, while
 * u-boot only includes the board for which it is built.
 */
const board_table_entry_t octeon_board_ddr_config_table[] =
{
#if !defined(_MIPS_ARCH_OCTEON2) && !defined(CONFIG_OCTEON_DISABLE_DDR2)
    /* DDR 2 boards */
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_THUNDER)
    {
        .board_type = CVMX_BOARD_TYPE_THUNDER,
        .default_ddr_clock_hz = 266000000,
        .eeprom_addr = OCTEON_THUNDER_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_THUNDER_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NICPRO2)
    {
        .board_type = CVMX_BOARD_TYPE_NICPRO2,
        .default_ddr_ref_hz = 100000000,
        .eeprom_addr = OCTEON_NICPRO2_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NICPRO2_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBT5800)
    {
        .board_type = CVMX_BOARD_TYPE_EBT5800,
        .eeprom_addr = OCTEON_EBT5800_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .chip_model = OCTEON_CN58XX,
                .ddr_config = {
                    OCTEON_EBT5800_DDR_CONFIGURATION
                }
            },
            {
                .chip_model = OCTEON_CN38XX,
                .ddr_config = {
                    OCTEON_EBT5800_CN38XX_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBT5810
    {
        /* The ebt5810 board shares its DDR configuration with the ebt5800 */
        .board_type = CVMX_BOARD_TYPE_EBT5810,
        .eeprom_addr = OCTEON_EBT5800_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .chip_model = OCTEON_CN58XX,
                .ddr_config = {
                    OCTEON_EBT5800_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBT3000
    {
        .board_type = CVMX_BOARD_TYPE_EBT3000,
        .eeprom_addr = OCTEON_EBT3000_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBT3000_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBT5200
    {
        .board_type = CVMX_BOARD_TYPE_EBT5200,
        .eeprom_addr = OCTEON_EBH5200_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5200_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_CN3010_EVB_HS5
    {
        .board_type = CVMX_BOARD_TYPE_CN3010_EVB_HS5,
        .eeprom_addr = CN3010_EVB_HS5_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .chip_model = OCTEON_CN50XX,
                .ddr_config = {
                    OCTEON_CN50XX_CN3010_EVB_HS5_DDR_CONFIGURATION
                }
            },
            {
                .chip_model = OCTEON_CN3XXX,
                .ddr_config = {
                    OCTEON_CN30XX_CN3010_EVB_HS5_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBH3100
    {
        .board_type = CVMX_BOARD_TYPE_EBH3100,
        .eeprom_addr = OCTEON_EBH3100_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .max_maj_rev = 1,
                .max_min_rev = 0,
                .ddr_config = {
                    OCTEON_EBH3100_DDR_CONFIGURATION_REV1
                }
            },
            {
                .max_maj_rev = 0,  /* All other revisions */
                .max_min_rev = 0,
                .ddr_config = {
                    OCTEON_EBH3100_DDR_CONFIGURATION_REV2
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBH5200
    {
        .board_type = CVMX_BOARD_TYPE_EBH5200,
        .eeprom_addr = OCTEON_EBH5200_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5200_DDR_CONFIGURATION
                }
            }
        }
    },
    /* The same configuration also supports the ebh5201 board */
    {
        .board_type = CVMX_BOARD_TYPE_EBH5201,
        .eeprom_addr = OCTEON_EBH5200_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5200_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_BBGW_REF
    {
        .board_type = CVMX_BOARD_TYPE_BBGW_REF,
        .default_ddr_clock_hz = 266000000,
        .eeprom_addr = BBGW_REF_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_BBGW_REF_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBB5600
    {
        .board_type = CVMX_BOARD_TYPE_EBB5600,
        .eeprom_addr = OCTEON_EBB5600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBB5600_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
#endif /* !_MIPS_ARCH_OCTEON2 && !CONFIG_OCTEON_DISABLE_DDR2 */
#if !defined(CVMX_BUILD_FOR_UBOOT) || CONFIG_OCTEON_EBB6300
    {
        .board_type = CVMX_BOARD_TYPE_EBB6300,
        .eeprom_addr = OCTEON_EBB6300_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBB6300_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SFF6100) || \
	defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)
    {
        .board_type = CVMX_BOARD_TYPE_SFF6100,
        .eeprom_addr = OCTEON_SFF6100_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_SFF6100_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB6100) || \
	defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)
    {
        .board_type = CVMX_BOARD_TYPE_EBB6100,
        .eeprom_addr = OCTEON_EBB6100_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBB6100_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EVB7100) \
	|| defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)
    {
        .board_type = CVMX_BOARD_TYPE_EVB7100,
        .eeprom_addr = OCTEON_EVB7100_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EVB7100_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_IW_EVB)
    {
        .board_type = CVMX_BOARD_TYPE_IW_EVB,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_IW_EVB_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_CNF71XX_REF)
    {
        .board_type = CVMX_BOARD_TYPE_CNF71XX_REF,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_CNF71XX_REF_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB6800)
    {
        .board_type = CVMX_BOARD_TYPE_EBB6800,
        .eeprom_addr = OCTEON_EBB6800_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBB6800_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_EBB7800)		|| \
     defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_EBB7800,
	.eeprom_addr = OCTEON_EBB7800_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7800_DDR_CONFIGURATION(OCTEON_EBB7800_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
    {
	.board_type = CVMX_BOARD_TYPE_EBB7800_CFG0,
	.eeprom_addr = OCTEON_EBB7800_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7800_DDR_CONFIGURATION(OCTEON_EBB7800_CFG0_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
    {
	.board_type = CVMX_BOARD_TYPE_EBB7800_CFG1,
	.eeprom_addr = OCTEON_EBB7800_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7800_DDR_CONFIGURATION(OCTEON_EBB7800_CFG1_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SFF7800)
    {
	.board_type = CVMX_BOARD_TYPE_SFF7800,
	.eeprom_addr = 0,
	.chip_ddr_config = {
		{ .ddr_config = { OCTEON_SFF7800_DDR_CONFIGURATION } }
	}
    },
#endif
#if defined(CONFIG_OCTEON_RAINIER) || !defined(CVMX_BUILD_FOR_UBOOT)
    {
	.board_type = CVMX_BOARD_TYPE_RAINIER,
	.eeprom_addr = OCTEON_RAINIER_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config = {
	    {
		.ddr_config = {
			OCTEON_RAINIER_DDR_CONFIGURATION
		}
	    }
	}
    },
#endif

#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_TB7600)		|| \
     defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_TB7600,
	.eeprom_addr = OCTEON_TB7600_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_TB7600_DDR_CONFIGURATION(OCTEON_TB7600_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB7804)
    {
	.board_type = CVMX_BOARD_TYPE_EBB7804,
	.eeprom_addr = OCTEON_EBB7804_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7804_DDR_CONFIGURATION(OCTEON_EBB7804_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
    {
	.board_type = CVMX_BOARD_TYPE_EBB7804_CFG0,
	.eeprom_addr = OCTEON_EBB7804_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7804_DDR_CONFIGURATION(OCTEON_EBB7804_CFG0_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
    {
	.board_type = CVMX_BOARD_TYPE_EBB7804_CFG1,
	.eeprom_addr = OCTEON_EBB7804_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7804_DDR_CONFIGURATION(OCTEON_EBB7804_CFG1_DRAM_SOCKET_CONFIGURATION) }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB7304)
    {
	.board_type = CVMX_BOARD_TYPE_EBB7304,
	.eeprom_addr = OCTEON_EBB7304_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7304_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_CNF7300)
    {
	.board_type = CVMX_BOARD_TYPE_CNF7300,
	.eeprom_addr = OCTEON_CNF7300_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_CNF7300_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB7504)
    {
	.board_type = CVMX_BOARD_TYPE_EBB7504,
	.eeprom_addr = OCTEON_EBB7504_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7504_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB7500)
    {
	.board_type = CVMX_BOARD_TYPE_EBB7500,
	.eeprom_addr = OCTEON_EBB7500_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EBB7500_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if 0 /*!defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_CNF7300)*/
    {
        .board_type = CVMX_BOARD_TYPE_CNF7300,
        .eeprom_addr = OCTEON_CNF7300_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = { OCTEON_CNF7300_DDR_CONFIGURATION }
            }
        }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC73) || \
     defined(CONFIG_OCTEON_NIC23)
    {
	.board_type = CVMX_BOARD_TYPE_NIC73,
	.eeprom_addr = OCTEON_NIC73_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_NIC73_DDR_CONFIGURATION }
	    }
	}
    },
    {	/* Same as NIC73 */
	.board_type = CVMX_BOARD_TYPE_NIC23,
	.eeprom_addr = OCTEON_NIC73_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_NIC73_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC25E)
    {
	.board_type = CVMX_BOARD_TYPE_NIC25E,
	.eeprom_addr = OCTEON_NIC25E_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_NIC25E_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC225E)
    {
	    .board_type = CVMX_BOARD_TYPE_NIC225E,
	    .eeprom_addr = OCTEON_NIC225E_BOARD_EEPROM_TWSI_ADDR,
	    .chip_ddr_config =
	    {
		{
		    .ddr_config = { OCTEON_NIC225E_DDR_CONFIGURATION }
		}
	    }
    },
    {
	    .board_type = CVMX_BOARD_TYPE_NIC25E,
	    .eeprom_addr = OCTEON_NIC225E_BOARD_EEPROM_TWSI_ADDR,
	    .chip_ddr_config =
	    {
		    {
			    .ddr_config = { OCTEON_NIC225E_DDR_CONFIGURATION }
		    }
	    }
    },
    # endif
# if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_EVB7000) 		|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_EVB7000,
	.eeprom_addr = OCTEON_EVB7000_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EVB7000_DDR_CONFIGURATION }
	    }
	}
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NICX40E)
    {
	    .board_type = CVMX_BOARD_TYPE_NICX40E,
	    .eeprom_addr = OCTEON_NICX40E_BOARD_EEPROM_TWSI_ADDR,
	    .chip_ddr_config = {
		    {
			    .ddr_config = { OCTEON_NICX40E_DDR_CONFIGURATION }
		    }
	    }
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_TB7000) 		|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_TB7000,
	.eeprom_addr = OCTEON_TB7000_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_TB7000_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_EVB7000_SFF)		|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_EVB7000_SFF_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_EVB7000_SFF,
	.eeprom_addr = OCTEON_EVB7000_SFF_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EVB7000_SFF_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if !defined(CVMX_BUILD_FOR_UBOOT)		|| \
     defined(CONFIG_OCTEON_SFF7000)		|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)	|| \
     defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)
     {
	.board_type = CVMX_BOARD_TYPE_SFF7000,
	.eeprom_addr = OCTEON_SFF7000_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_SFF7000_4GB_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if (!defined(BDK) && !defined(CVMX_BUILD_FOR_UBOOT)) ||	\
     defined(CONFIG_OCTEON_NAS7000_REF_EMMC_STAGE2) ||		\
     (defined(CONFIG_OCTEON_NAS7000_REF) && defined(unix))
    {
	.board_type = CVMX_BOARD_TYPE_NAS7000_REF,
	.eeprom_addr = 0,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_NAS7000_REF_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if (!defined(BDK) && !defined(CVMX_BUILD_FOR_UBOOT)) ||	\
     defined(CONFIG_OCTEON_EAP7000_REF_EMMC_STAGE2) ||		\
     defined(CONFIG_OCTEON_EAP7000_REF_SPI_STAGE2) ||		\
     (defined(CONFIG_OCTEON_EAP7000_REF) && defined(unix))
    {
	.board_type = CVMX_BOARD_TYPE_EAP7000_REF,
	.eeprom_addr = 0,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_EAP7000_REF_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if (!defined(BDK) && !defined(CVMX_BUILD_FOR_UBOOT)) ||	\
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2) ||		\
     defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2) ||	\
     (defined(CONFIG_OCTEON_WAVE2) && defined(unix))
    {
	    .board_type = CVMX_BOARD_TYPE_WAVE2,
	    .eeprom_addr = 0,
	    .chip_ddr_config =
	    {
		    {
			    .ddr_config = { OCTEON_WAVE2_DDR_CONFIGURATION }
		    }
	    }
    },
#endif
#if (!defined(BDK) && !defined(CVMX_BUILD_FOR_UBOOT)) ||	\
     defined(CONFIG_OCTEON_ROUTER7000_REF_EMMC_STAGE2) ||	\
     defined(CONFIG_OCTEON_ROUTER7000_REF_SPI_STAGE2)  ||	\
     (defined(CONFIG_OCTEON_ROUTER7000_REF) && defined(unix))
    {
	.board_type = CVMX_BOARD_TYPE_ROUTER7000_REF,
	.eeprom_addr = 0,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_ROUTER7000_REF_DDR_CONFIGURATION }
	    }
	}
    },
#endif
#if (!defined(BDK) && !defined(CVMX_BUILD_FOR_UBOOT))	||	\
     defined(CONFIG_OCTEON_NIAGARA83X_EMMC_STAGE2)	||	\
     defined(CONFIG_OCTEON_NIAGARA83X_SPI_STAGE2)	||	\
     defined(CONFIG_OCTEON_NIAGARA83X)			||	\
     defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)
    {
	.board_type = CVMX_BOARD_TYPE_NIAGARA83X,
	.eeprom_addr = OCTEON_NIAGARA83X_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
	        .ddr_config = { OCTEON_NIAGARA83X_DDR_CONFIGURATION }
	    }
        }
    },
#endif

/* Disabled in the BDK to save space */
#if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) || \
		      defined(CONFIG_OCTEON_NIC68_4))
    {
        .board_type = CVMX_BOARD_TYPE_NIC68_4,
        .eeprom_addr = OCTEON_NIC68_4_BOARD_EEPROM_TWSI_ADDR,
        .default_ddr_clock_hz = 667000000,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC68_4_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(BDK) && (!defined(CVMX_BUILD_FOR_UBOOT) || \
		      defined(CONFIG_OCTEON_SWORDFISH))
    {
        .board_type = CVMX_BOARD_TYPE_SWORDFISH,
        .eeprom_addr = OCTEON_SWORDFISH_BOARD_EEPROM_TWSI_ADDR,
        .default_ddr_clock_hz = 667000000,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_SWORDFISH_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#ifndef CVMX_BUILD_FOR_UBOOT
    {
	.board_type = CVMX_BOARD_TYPE_CUST_CLARK,
	.eeprom_addr = 0,
	.chip_ddr_config =
	{
	    {
		.ddr_config = { OCTEON_CLARK_DDR_CONFIGURATION }
	    }
	}
    },

#endif
#if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBB6600)
    {
        .board_type = CVMX_BOARD_TYPE_EBB6600,
        .eeprom_addr = OCTEON_EBB6600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBB6600_DDR_CONFIGURATION
                }
            }
        }
    },
#endif
#if !defined(BDK)
# if !defined(CONFIG_OCTEON_DISABLE_DDR2)
#  if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NAC38)
    {
        .board_type = CVMX_BOARD_TYPE_NAC38,
        .eeprom_addr = OCTEON_NAC38_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .chip_model = OCTEON_CN38XX,
                .ddr_config = {
                    OCTEON_CN38XX_NAC38_DDR_CONFIGURATION
                }
            },
            {
                .chip_model = OCTEON_CN58XX,
                .ddr_config = {
                    OCTEON_CN58XX_NAC38_DDR_CONFIGURATION
                }
            }
        }
    },
#  endif
#  if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_WSX16)
    {
        .board_type = CVMX_BOARD_TYPE_CUST_WSX16,
        .eeprom_addr = OCTEON_WSX16_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .chip_model = OCTEON_CN38XX,
                .ddr_config = {
                    OCTEON_CN38XX_WSX16_DDR_CONFIGURATION
                }
            },
            {
                .chip_model = OCTEON_CN58XX,
                .ddr_config = {
                    OCTEON_CN58XX_WSX16_DDR_CONFIGURATION
                }
            }
        }
    },
#  endif
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC10E)
    {
        .board_type = CVMX_BOARD_TYPE_NIC10E,
        .eeprom_addr = OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC10E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC210NVG)
    {
        .board_type = CVMX_BOARD_TYPE_NIC210NVG,
        .eeprom_addr = OCTEON_NIC210NVG_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC210NVG_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_COPPERHEAD)
    {
	.board_type = CVMX_BOARD_TYPE_COPPERHEAD,
	.eeprom_addr = OCTEON_COPPERHEAD_BOARD_EEPROM_TWSI_ADDR,
	.chip_ddr_config =
	{
	    {
		.ddr_config = {
		    OCTEON_COPPERHEAD_DDR_CONFIGURATION
		}
	    }
	}
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SNIC10E)
    {
        .board_type = CVMX_BOARD_TYPE_SNIC10E,
        .eeprom_addr = OCTEON_SNIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_SNIC10E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_SNIC10E_61)
    {
        .board_type = CVMX_BOARD_TYPE_SNIC10E_61,
        .eeprom_addr = OCTEON_SNIC10E_61_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_SNIC10E_61_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC10E_66)
/* The nic10e and nic10e_66 share the same DDR3 layout and configuration,
 * so they share the same configuration
 */
    {
        .board_type = CVMX_BOARD_TYPE_NIC10E_66,
        .eeprom_addr = OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                /* For revs 1 and 2 */
                .max_maj_rev = 2,
                .max_min_rev = 255,
                .ddr_config = {
                    OCTEON_NIC10E_DDR_CONFIGURATION
                }
            },
            {
                /* Revs 3.0 and higher.  The SPD address changed,
                ** but otherwise the DDR settings are the same. */
                .max_maj_rev = 0,
                .max_min_rev = 0,
                .ddr_config = {
                    OCTEON_NIC10E_66_REV3_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC4E_66)
/* The nic10e and nic4e_66 share the same DDR3 layout and configuration,
** so they share the same configuration */
    {
        .board_type = CVMX_BOARD_TYPE_NIC4E_66,
        .eeprom_addr = OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC10E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC401NVG)
/* The nic10e and nic401nvg share the same DDR3 layout and configuration,
 * so they share the same configuration
 */
    {
        .board_type = CVMX_BOARD_TYPE_NIC401NVG,
        .eeprom_addr = OCTEON_NIC401NVG_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC401NVG_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC4E)
/* The nic10e and nic4e share the same DDR3 layout and configuration,
** so they share the same configuration */
    {
        .board_type = CVMX_BOARD_TYPE_NIC4E,
        .eeprom_addr = OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC10E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_REDWING)
/* The nic10e and Redwing share the same DDR3 layout and configuration,
** so they share the same configuration */
    {
        .board_type = CVMX_BOARD_TYPE_REDWING,
        .eeprom_addr = OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC10E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC2E)
/* The nic10e and nic4e share the same DDR3 layout and configuration,
** so they share the same configuration */
    {
        .board_type = CVMX_BOARD_TYPE_NIC2E,
        .eeprom_addr = OCTEON_NIC2E_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC2E_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EP6300C)
    {
	    .board_type = CVMX_BOARD_TYPE_EP6300C,
	    .eeprom_addr = OCTEON_EP6300C_BOARD_EEPROM_TWSI_ADDR,
	    .chip_ddr_config =
	    {
		    {
			    .ddr_config = {
				    OCTEON_EP6300C_DDR_CONFIGURATION
			    }
		    }
	    }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_MOONSHOT)
    {
        .board_type = CVMX_BOARD_TYPE_MOONSHOT,
        .eeprom_addr = OCTEON_MOONSHOT_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_MOONSHOT_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
#endif /* BDK - Disabled in the BDK to save space */
#if !defined(_MIPS_ARCH_OCTEON2) && !defined(CONFIG_OCTEON_DISABLE_DDR2)
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBT5600)
    {
        /* The EBT5600 board uses the same configuration as the ebh5600 board */
        .board_type = CVMX_BOARD_TYPE_EBT5600,
        .eeprom_addr = OCTEON_EBH5600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5600_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBH5601)
    {
        /* the EBH5601 board uses the same config as the ebh5600 board */
        .board_type = CVMX_BOARD_TYPE_EBH5601,
        .eeprom_addr = OCTEON_EBH5600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5600_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBH5600)
    {
        .board_type = CVMX_BOARD_TYPE_EBH5600,
        .eeprom_addr = OCTEON_EBH5600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5600_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_EBH5610)
    {
        /* Uses the same DRAM config as ebh5600 board */
        .board_type = CVMX_BOARD_TYPE_EBH5610,
        .eeprom_addr = OCTEON_EBH5600_BOARD_EEPROM_TWSI_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_EBH5600_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC_XLE_4G)
    {
        .board_type = CVMX_BOARD_TYPE_NIC_XLE_4G,
        .eeprom_addr = OCTEON_NIC_XLE_4G_EEPROM_ADDR,
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC_XLE_4G_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
# if !defined(CVMX_BUILD_FOR_UBOOT) || defined(CONFIG_OCTEON_NIC_XLE_10G)
    {
        .board_type = CVMX_BOARD_TYPE_NIC_XLE_10G,
        .eeprom_addr = OCTEON_NIC_XLE_4G_EEPROM_ADDR,
        /* This board shares the DDR configuration with the nic_xle_4g */
        .chip_ddr_config =
        {
            {
                .ddr_config = {
                    OCTEON_NIC_XLE_4G_DDR_CONFIGURATION
                }
            }
        }
    },
# endif
#endif /* !_MIPS_ARCH_OCTEON2 && !CONFIG_OCTEON_DISABLE_DDR2 */
#if !defined(CVMX_BUILD_FOR_UBOOT)	|| \
    (defined(CONFIG_OCTEON_GENERIC)			&& \
     !defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)	&& \
     !defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2))
    {
        .board_type = CVMX_BOARD_TYPE_GENERIC,
        .default_ddr_ref_hz = 50000000,
        .default_ddr_clock_hz = 200000000,
        .bootloader_name = "u-boot-octeon_generic_ram.bin",
    },
#endif
    {
        .board_type = 0,  /* Mark the end of the board list */
    },

};




const ddr_configuration_t *lookup_ddr_config_structure(uint32_t cpu_id,
						       int board_type,
						       int board_rev_maj,
						       int board_rev_min,
						       uint32_t *interface_mask)
{
	/* Look up board type and get ddr_configuration from table */
	int i,j;
	int interface_index;

	if (!interface_mask)
		return NULL;
	*interface_mask = 0;

	for (i = 0; octeon_board_ddr_config_table[i].board_type; i++) {
		if (octeon_board_ddr_config_table[i].board_type == board_type) {
			/* Now look up the configuration matching the current
			 * chip type
			 */
			for (j = 0; j < MAX_DDR_CONFIGS_PER_BOARD; j++) {
				if ((!octeon_board_ddr_config_table[i].chip_ddr_config[j].chip_model
				     || octeon_is_cpuid(octeon_board_ddr_config_table[i].chip_ddr_config[j].chip_model))
			            && (!octeon_board_ddr_config_table[i].chip_ddr_config[j].max_maj_rev
					|| (board_rev_maj <= octeon_board_ddr_config_table[i].chip_ddr_config[j].max_maj_rev
				    && board_rev_min <= octeon_board_ddr_config_table[i].chip_ddr_config[j].max_min_rev))) {
					for (interface_index= 0 ; interface_index < 4;
					     ++interface_index) {
						if (octeon_board_ddr_config_table[i].chip_ddr_config[j].ddr_config[interface_index].dimm_config_table[0].spd_addrs[0]
						    || octeon_board_ddr_config_table[i].chip_ddr_config[j].ddr_config[interface_index].dimm_config_table[0].spd_ptrs[0])
							*interface_mask |= 1 << interface_index;
					}
					ddr_print("Found DDR configuration for %s\n",
						  cvmx_board_type_to_string(board_type));
					return octeon_board_ddr_config_table[i].chip_ddr_config[j].ddr_config;
				}
			}
		}
	}
	return NULL;
}

/* Look up the board table entry for a board, given a board type or a board name */
const board_table_entry_t *lookup_board_table_entry(int board_type,
						    char *board_name)
{

	int i;
	for (i = 0; octeon_board_ddr_config_table[i].board_type; i++) {
		if (octeon_board_ddr_config_table[i].board_type == board_type)
			return &octeon_board_ddr_config_table[i];
#if !defined(__U_BOOT__)
		if (board_name &&
		    !strcasecmp(board_name,
				cvmx_board_type_to_string(octeon_board_ddr_config_table[i].board_type)))
			return &octeon_board_ddr_config_table[i];
#endif
	}

	return NULL;
}
