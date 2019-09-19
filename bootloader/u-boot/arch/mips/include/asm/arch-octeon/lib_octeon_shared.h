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


#ifndef __LIB_OCTEON_SHARED_H__
#define __LIB_OCTEON_SHARED_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#include "cvmx.h"
#endif
#include "cvmx-lmcx-defs.h"
#include "lib_octeon_board_table_entry.h"

/* Structure for bootloader pci IO buffers */
typedef struct
{
    uint32_t owner;
    uint32_t len;
    char data[0];
} octeon_pci_io_buf_t;
#define BOOTLOADER_PCI_READ_BUFFER_STR_LEN  (BOOTLOADER_PCI_READ_BUFFER_SIZE - 8)
#define BOOTLOADER_PCI_WRITE_BUFFER_STR_LEN  (BOOTLOADER_PCI_WRITE_BUFFER_SIZE - 8)

#define BOOTLOADER_PCI_READ_BUFFER_OWNER_ADDR   (BOOTLOADER_PCI_READ_BUFFER_BASE + 0)
#define BOOTLOADER_PCI_READ_BUFFER_LEN_ADDR   (BOOTLOADER_PCI_READ_BUFFER_BASE + 4)
#define BOOTLOADER_PCI_READ_BUFFER_DATA_ADDR   (BOOTLOADER_PCI_READ_BUFFER_BASE + 8)

enum octeon_pci_io_buf_owner
{
    OCTEON_PCI_IO_BUF_OWNER_INVALID = 0,  /* Must be zero, set when memory cleared */
    OCTEON_PCI_IO_BUF_OWNER_OCTEON = 1,
    OCTEON_PCI_IO_BUF_OWNER_HOST = 2,
};

/* data field addresses in the DDR2 SPD eeprom */
typedef enum ddr2_spd_addrs {
    DDR2_SPD_BYTES_PROGRAMMED	= 0,
    DDR2_SPD_TOTAL_BYTES	= 1,
    DDR2_SPD_MEM_TYPE		= 2,
    DDR2_SPD_NUM_ROW_BITS	= 3,
    DDR2_SPD_NUM_COL_BITS	= 4,
    DDR2_SPD_NUM_RANKS		= 5,
    DDR2_SPD_CYCLE_CLX		= 9,
    DDR2_SPD_CONFIG_TYPE	= 11,
    DDR2_SPD_REFRESH		= 12,
    DDR2_SPD_SDRAM_WIDTH	= 13,
    DDR2_SPD_BURST_LENGTH	= 16,
    DDR2_SPD_NUM_BANKS		= 17,
    DDR2_SPD_CAS_LATENCY	= 18,
    DDR2_SPD_DIMM_TYPE		= 20,
    DDR2_SPD_CYCLE_CLX1		= 23,
    DDR2_SPD_CYCLE_CLX2		= 25,
    DDR2_SPD_TRP		= 27,
    DDR2_SPD_TRRD 		= 28,
    DDR2_SPD_TRCD 		= 29,
    DDR2_SPD_TRAS 		= 30,
    DDR2_SPD_TWR 		= 36,
    DDR2_SPD_TWTR 		= 37,
    DDR2_SPD_TRFC_EXT		= 40,
    DDR2_SPD_TRFC 		= 42,
    DDR2_SPD_CHECKSUM		= 63,
    DDR2_SPD_MFR_ID		= 64
} ddr2_spd_addr_t;

/* data field addresses in the DDR2 SPD eeprom */
typedef enum ddr3_spd_addrs {
    DDR3_SPD_BYTES_PROGRAMMED				=  0,
    DDR3_SPD_REVISION					=  1,
    DDR3_SPD_KEY_BYTE_DEVICE_TYPE			=  2,
    DDR3_SPD_KEY_BYTE_MODULE_TYPE			=  3,
    DDR3_SPD_DENSITY_BANKS				=  4,
    DDR3_SPD_ADDRESSING_ROW_COL_BITS			=  5,
    DDR3_SPD_NOMINAL_VOLTAGE				=  6,
    DDR3_SPD_MODULE_ORGANIZATION			=  7,
    DDR3_SPD_MEMORY_BUS_WIDTH				=  8,
    DDR3_SPD_FINE_TIMEBASE_DIVIDEND_DIVISOR		=  9,
    DDR3_SPD_MEDIUM_TIMEBASE_DIVIDEND			= 10,
    DDR3_SPD_MEDIUM_TIMEBASE_DIVISOR			= 11,
    DDR3_SPD_MINIMUM_CYCLE_TIME_TCKMIN			= 12,
    DDR3_SPD_CAS_LATENCIES_LSB				= 14,
    DDR3_SPD_CAS_LATENCIES_MSB				= 15,
    DDR3_SPD_MIN_CAS_LATENCY_TAAMIN			= 16,
    DDR3_SPD_MIN_WRITE_RECOVERY_TWRMIN			= 17,
    DDR3_SPD_MIN_RAS_CAS_DELAY_TRCDMIN			= 18,
    DDR3_SPD_MIN_ROW_ACTIVE_DELAY_TRRDMIN		= 19,
    DDR3_SPD_MIN_ROW_PRECHARGE_DELAY_TRPMIN		= 20,
    DDR3_SPD_UPPER_NIBBLES_TRAS_TRC			= 21,
    DDR3_SPD_MIN_ACTIVE_PRECHARGE_LSB_TRASMIN		= 22,
    DDR3_SPD_MIN_ACTIVE_REFRESH_LSB_TRCMIN		= 23,
    DDR3_SPD_MIN_REFRESH_RECOVERY_LSB_TRFCMIN		= 24,
    DDR3_SPD_MIN_REFRESH_RECOVERY_MSB_TRFCMIN           = 25,
    DDR3_SPD_MIN_INTERNAL_WRITE_READ_CMD_TWTRMIN        = 26,
    DDR3_SPD_MIN_INTERNAL_READ_PRECHARGE_CMD_TRTPMIN    = 27,
    DDR3_SPD_UPPER_NIBBLE_TFAW                          = 28,
    DDR3_SPD_MIN_FOUR_ACTIVE_WINDOW_TFAWMIN             = 29,
    DDR3_SPD_SDRAM_OPTIONAL_FEATURES			= 30,
    DDR3_SPD_SDRAM_THERMAL_REFRESH_OPTIONS		= 31,
    DDR3_SPD_MODULE_THERMAL_SENSOR			= 32,
    DDR3_SPD_SDRAM_DEVICE_TYPE				= 33,
    DDR3_SPD_MINIMUM_CYCLE_TIME_FINE_TCKMIN		= 34,
    DDR3_SPD_MIN_CAS_LATENCY_FINE_TAAMIN		= 35,
    DDR3_SPD_MIN_RAS_CAS_DELAY_FINE_TRCDMIN		= 36,
    DDR3_SPD_MIN_ROW_PRECHARGE_DELAY_FINE_TRPMIN	= 37,
    DDR3_SPD_MIN_ACTIVE_REFRESH_LSB_FINE_TRCMIN		= 38,
    DDR3_SPD_REFERENCE_RAW_CARD                         = 62,
    DDR3_SPD_ADDRESS_MAPPING                            = 63,
    DDR3_SPD_REGISTER_MANUFACTURER_ID_LSB		= 65,
    DDR3_SPD_REGISTER_MANUFACTURER_ID_MSB		= 66,
    DDR3_SPD_REGISTER_REVISION_NUMBER			= 67,
    DDR3_SPD_MODULE_SERIAL_NUMBER                       = 122,
    DDR3_SPD_CYCLICAL_REDUNDANCY_CODE_LOWER_NIBBLE      = 126,
    DDR3_SPD_CYCLICAL_REDUNDANCY_CODE_UPPER_NIBBLE      = 127,
    DDR3_SPD_MODULE_PART_NUMBER                         = 128
} ddr3_spd_addr_t;


/* data field addresses in the DDR4 SPD eeprom */
typedef enum ddr4_spd_addrs {
    DDR4_SPD_BYTES_PROGRAMMED				=  0,
    DDR4_SPD_REVISION					=  1,
    DDR4_SPD_KEY_BYTE_DEVICE_TYPE			=  2,
    DDR4_SPD_KEY_BYTE_MODULE_TYPE			=  3,
    DDR4_SPD_DENSITY_BANKS				=  4,
    DDR4_SPD_ADDRESSING_ROW_COL_BITS			=  5,
    DDR4_SPD_PACKAGE_TYPE				=  6,
    DDR4_SPD_OPTIONAL_FEATURES				=  7,
    DDR4_SPD_THERMAL_REFRESH_OPTIONS			=  8,
    DDR4_SPD_OTHER_OPTIONAL_FEATURES			=  9,
    DDR4_SPD_SECONDARY_PACKAGE_TYPE			= 10,
    DDR4_SPD_MODULE_NOMINAL_VOLTAGE			= 11,
    DDR4_SPD_MODULE_ORGANIZATION			= 12,
    DDR4_SPD_MODULE_MEMORY_BUS_WIDTH			= 13,
    DDR4_SPD_MODULE_THERMAL_SENSOR			= 14,
    DDR4_SPD_RESERVED_BYTE15				= 15,
    DDR4_SPD_RESERVED_BYTE16				= 16,
    DDR4_SPD_TIMEBASES					= 17,
    DDR4_SPD_MINIMUM_CYCLE_TIME_TCKAVGMIN		= 18,
    DDR4_SPD_MAXIMUM_CYCLE_TIME_TCKAVGMAX		= 19,
    DDR4_SPD_CAS_LATENCIES_BYTE0			= 20,
    DDR4_SPD_CAS_LATENCIES_BYTE1			= 21,
    DDR4_SPD_CAS_LATENCIES_BYTE2			= 22,
    DDR4_SPD_CAS_LATENCIES_BYTE3			= 23,
    DDR4_SPD_MIN_CAS_LATENCY_TAAMIN			= 24,
    DDR4_SPD_MIN_RAS_CAS_DELAY_TRCDMIN			= 25,
    DDR4_SPD_MIN_ROW_PRECHARGE_DELAY_TRPMIN		= 26,
    DDR4_SPD_UPPER_NIBBLES_TRAS_TRC			= 27,
    DDR4_SPD_MIN_ACTIVE_PRECHARGE_LSB_TRASMIN		= 28,
    DDR4_SPD_MIN_ACTIVE_REFRESH_LSB_TRCMIN		= 29,
    DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC1MIN		= 30,
    DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC1MIN          = 31,
    DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC2MIN		= 32,
    DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC2MIN          = 33,
    DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC4MIN		= 34,
    DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC4MIN          = 35,
    DDR4_SPD_MIN_FOUR_ACTIVE_WINDOW_MSN_TFAWMIN         = 36,
    DDR4_SPD_MIN_FOUR_ACTIVE_WINDOW_LSB_TFAWMIN         = 37,
    DDR4_SPD_MIN_ROW_ACTIVE_DELAY_SAME_TRRD_SMIN	= 38,
    DDR4_SPD_MIN_ROW_ACTIVE_DELAY_DIFF_TRRD_LMIN	= 39,
    DDR4_SPD_MIN_CAS_TO_CAS_DELAY_TCCD_LMIN		= 40,
    DDR4_SPD_MIN_CAS_TO_CAS_DELAY_FINE_TCCD_LMIN	= 117,
    DDR4_SPD_MIN_ACT_TO_ACT_DELAY_SAME_FINE_TRRD_LMIN	= 118,
    DDR4_SPD_MIN_ACT_TO_ACT_DELAY_DIFF_FINE_TRRD_SMIN	= 119,
    DDR4_SPD_MIN_ACT_TO_ACT_REFRESH_DELAY_FINE_TRCMIN	= 120,
    DDR4_SPD_MIN_ROW_PRECHARGE_DELAY_FINE_TRPMIN	= 121,
    DDR4_SPD_MIN_RAS_TO_CAS_DELAY_FINE_TRCDMIN		= 122,
    DDR4_SPD_MIN_CAS_LATENCY_FINE_TAAMIN		= 123,
    DDR4_SPD_MAX_CYCLE_TIME_FINE_TCKAVGMAX		= 124,
    DDR4_SPD_MIN_CYCLE_TIME_FINE_TCKAVGMIN		= 125,
    DDR4_SPD_CYCLICAL_REDUNDANCY_CODE_LOWER_NIBBLE      = 126,
    DDR4_SPD_CYCLICAL_REDUNDANCY_CODE_UPPER_NIBBLE      = 127,
    DDR4_SPD_REFERENCE_RAW_CARD				= 130,
    DDR4_SPD_UDIMM_ADDR_MAPPING_FROM_EDGE		= 131,
    DDR4_SPD_REGISTER_MANUFACTURER_ID_LSB		= 133,
    DDR4_SPD_REGISTER_MANUFACTURER_ID_MSB		= 134,
    DDR4_SPD_REGISTER_REVISION_NUMBER			= 135,
    DDR4_SPD_RDIMM_ADDR_MAPPING_FROM_REGISTER_TO_DRAM	= 136,
    DDR4_SPD_RDIMM_REGISTER_DRIVE_STRENGTH_CTL		= 137,
    DDR4_SPD_RDIMM_REGISTER_DRIVE_STRENGTH_CK		= 138,
    DDR4_SPD_MODULE_SERIAL_NUMBER			= 325,
    DDR4_SPD_MODULE_PART_NUMBER				= 329
} ddr4_spd_addr_t;


/*
** DRAM Module Organization
**
** Octeon:
** Octeon can be configured to use two pairs of DIMM's, lower and
** upper, providing a 128/144-bit interface or one to four DIMM's
** providing a 64/72-bit interface.  This structure contains the TWSI
** addresses used to access the DIMM's Serial Presence Detect (SPD)
** EPROMS and it also implies which DIMM socket organization is used
** on the board.  Software uses this to detect the presence of DIMM's
** plugged into the sockets, compute the total memory capacity, and
** configure DRAM controller.  All DIMM's must be identical.
**
** CN31XX:
** Octeon CN31XX can be configured to use one to four DIMM's providing
** a 64/72-bit interface.  This structure contains the TWSI addresses
** used to access the DIMM's Serial Presence Detect (SPD) EPROMS and
** it also implies which DIMM socket organization is used on the
** board.  Software uses this to detect the presence of DIMM's plugged
** into the sockets, compute the total memory capacity, and configure
** DRAM controller.  All DIMM's must be identical.
*/

/* dimm_config_t value that terminates list */
#define DIMM_CONFIG_TERMINATOR  {{0,0}, {NULL, NULL}}

extern const dimm_odt_config_t disable_odt_config[];
extern const dimm_odt_config_t single_rank_odt_config[];
extern const dimm_odt_config_t dual_rank_odt_config[];

extern const board_table_entry_t octeon_board_ddr_config_table[];
extern int get_ddr_type(const dimm_config_t *dimm_config, int upper_dimm);
extern int get_dimm_module_type(const dimm_config_t *dimm_config, int upper_dimm, int ddr_type);


int validate_dimm(const dimm_config_t *dimm_config, int dimm);
int validate_spd_checksum(int twsi_addr, int silent);

int initialize_ddr_clock(uint32_t cpu_id,
			 const ddr_configuration_t *ddr_configuration,
                         uint32_t cpu_hertz,
                         uint32_t ddr_hertz,
                         uint32_t ddr_ref_hertz,
                         int ddr_interface_num,
                         uint32_t ddr_interface_mask,
			 int node);
uint32_t measure_octeon_ddr_clock(uint32_t cpu_id,
				  const ddr_configuration_t *ddr_configuration,
				  uint32_t cpu_hertz,
                                  uint32_t ddr_hertz,
                                  uint32_t ddr_ref_hertz,
                                  int ddr_interface_num,
                                  uint32_t ddr_interface_mask,
				  int node);

int octeon_ddr_initialize(uint32_t cpu_id,
                          uint32_t cpu_hertz,
                          uint32_t ddr_hertz,
                          uint32_t ddr_ref_hertz,
                          uint32_t ddr_interface_mask,
                          const ddr_configuration_t *ddr_config_array,
                          uint32_t *measured_ddr_hertz,
                          int board_type,
                          int board_rev_maj,
                          int board_rev_min,
			  int node);

int get_dimm_part_number(char *buffer, const dimm_config_t *dimm_config, int dimm_index, int ddr_type);

void board_fixup_dram_config(ddr_configuration_t *cfg);

int read_spd(const dimm_config_t *dimm_config, int dimm_index,
             int spd_field);

int octeon_dfm_initialize(void);

int twsii_mcu_read(uint8_t twsii_addr);
int twsii_mcu_read_node(uint8_t node, uint8_t twsii_addr);


/* Look up the ddr configuration information based on chip and board type */
const ddr_configuration_t *lookup_ddr_config_structure(uint32_t cpu_id, int board_type, int board_rev_maj, int board_rev_min, uint32_t *interface_mask);
const board_table_entry_t *lookup_board_table_entry(int board_type, char *board_name);

/* Allow legacy code to encode bus number in the upper bits of the address
** These are only supported in read_spd() */
#define OCTEON_TWSI_BUS_IN_ADDR_BIT       12
#define OCTEON_TWSI_BUS_IN_ADDR_MASK      (15 << OCTEON_TWSI_BUS_IN_ADDR_BIT)
#define OCTEON_TWSI_GET_BUS(addr)	\
	(((addr) >> OCTEON_TWSI_BUS_IN_ADDR_BIT) & 0xf)

int octeon_tlv_get_tuple_addr(uint8_t dev_addr, uint16_t type, uint8_t major_version, uint8_t *eeprom_buf, uint32_t buf_len);

int  octeon_tlv_eeprom_get_next_tuple(uint8_t dev_addr, uint16_t addr, uint8_t *buf_ptr, uint32_t buf_len);

#ifdef ENABLE_BOARD_DEBUG
void gpio_status(int data);
#endif

/* Divide and round results to the nearest integer. */
static inline uint64_t divide_nint(uint64_t dividend, uint64_t divisor)
{
	uint64_t quotent, remainder;
	quotent   = dividend / divisor;
	remainder = dividend % divisor;
	return (quotent + ((remainder * 2) >= divisor));
}

/* Divide and round results up to the next higher integer. */
/* Divide and round results up to the next higher integer. */
static inline uint64_t divide_roundup(uint64_t dividend, uint64_t divisor)
{
	return ((dividend + divisor - 1) / divisor);
}

typedef enum {
    DDR3_DRAM = 3,
    DDR4_DRAM = 4,
} ddr_type_t;


#define rttnom_none   0         /* Rtt_Nom disabled */
#define rttnom_60ohm  1         /* RZQ/4  = 240/4  =  60 ohms */
#define rttnom_120ohm 2         /* RZQ/2  = 240/2  = 120 ohms */
#define rttnom_40ohm  3         /* RZQ/6  = 240/6  =  40 ohms */
#define rttnom_20ohm  4         /* RZQ/12 = 240/12 =  20 ohms */
#define rttnom_30ohm  5         /* RZQ/8  = 240/8  =  30 ohms */
#define rttnom_rsrv1  6         /* Reserved */
#define rttnom_rsrv2  7         /* Reserved */

#define rttwr_none    0         /* Dynamic ODT off */
#define rttwr_60ohm   1         /* RZQ/4  = 240/4  =  60 ohms */
#define rttwr_120ohm  2         /* RZQ/2  = 240/2  = 120 ohms */
#define rttwr_rsrv1   3         /* Reserved */

#define dic_40ohm     0         /* RZQ/6  = 240/6  =  40 ohms */
#define dic_34ohm     1         /* RZQ/7  = 240/7  =  34 ohms */

#define driver_24_ohm   1
#define driver_27_ohm   2
#define driver_30_ohm   3
#define driver_34_ohm   4
#define driver_40_ohm   5
#define driver_48_ohm   6
#define driver_60_ohm   7

#define rodt_ctl_none     0
#define rodt_ctl_20_ohm   1
#define rodt_ctl_30_ohm   2
#define rodt_ctl_40_ohm   3
#define rodt_ctl_60_ohm   4
#define rodt_ctl_120_ohm  5

#define ddr4_rttnom_none   0         /* Rtt_Nom disabled */
#define ddr4_rttnom_60ohm  1         /* RZQ/4  = 240/4  =  60 ohms */
#define ddr4_rttnom_120ohm 2         /* RZQ/2  = 240/2  = 120 ohms */
#define ddr4_rttnom_40ohm  3         /* RZQ/6  = 240/6  =  40 ohms */
#define ddr4_rttnom_240ohm 4         /* RZQ/1  = 240/1  = 240 ohms */
#define ddr4_rttnom_48ohm  5         /* RZQ/5  = 240/5  =  48 ohms */
#define ddr4_rttnom_80ohm  6         /* RZQ/3  = 240/3  =  80 ohms */
#define ddr4_rttnom_34ohm  7         /* RZQ/7  = 240/7  =  34 ohms */

#define ddr4_rttwr_none    0         /* Dynamic ODT off */
#define ddr4_rttwr_120ohm  1         /* RZQ/2  = 240/2  = 120 ohms */
#define ddr4_rttwr_240ohm  2         /* RZQ/1  = 240/1  = 240 ohms */
#define ddr4_rttwr_HiZ     3         /* HiZ */
/* This setting is available for cn78xx pass 2, and cn73xx
   & cnf75xx pass 1. */
#define ddr4_rttwr_80ohm   4         /* RZQ/3  = 240/3  =  80 ohms */

#define ddr4_dic_34ohm     0         /* RZQ/7  = 240/7  =  34 ohms */
#define ddr4_dic_48ohm     1         /* RZQ/5  = 240/5  =  48 ohms */

#define ddr4_rttpark_none   0         /* Rtt_Park disabled */
#define ddr4_rttpark_60ohm  1         /* RZQ/4  = 240/4  =  60 ohms */
#define ddr4_rttpark_120ohm 2         /* RZQ/2  = 240/2  = 120 ohms */
#define ddr4_rttpark_40ohm  3         /* RZQ/6  = 240/6  =  40 ohms */
#define ddr4_rttpark_240ohm 4         /* RZQ/1  = 240/1  = 240 ohms */
#define ddr4_rttpark_48ohm  5         /* RZQ/5  = 240/5  =  48 ohms */
#define ddr4_rttpark_80ohm  6         /* RZQ/3  = 240/3  =  80 ohms */
#define ddr4_rttpark_34ohm  7         /* RZQ/7  = 240/7  =  34 ohms */

#define ddr4_driver_26_ohm   2
#define ddr4_driver_30_ohm   3
#define ddr4_driver_34_ohm   4
#define ddr4_driver_40_ohm   5
#define ddr4_driver_48_ohm   6

#define ddr4_dqx_driver_24_ohm   1
#define ddr4_dqx_driver_27_ohm   2
#define ddr4_dqx_driver_30_ohm   3
#define ddr4_dqx_driver_34_ohm   4
#define ddr4_dqx_driver_40_ohm   5
#define ddr4_dqx_driver_48_ohm   6
#define ddr4_dqx_driver_60_ohm   7

#define ddr4_rodt_ctl_none     0
#define ddr4_rodt_ctl_40_ohm   1
#define ddr4_rodt_ctl_60_ohm   2
#define ddr4_rodt_ctl_80_ohm   3
#define ddr4_rodt_ctl_120_ohm  4
#define ddr4_rodt_ctl_240_ohm  5
#define ddr4_rodt_ctl_34_ohm   6
#define ddr4_rodt_ctl_48_ohm   7

/*
** Generic copy of these parameters. Modify and enable the local
** version in include/configs for board specific customizations.
*/

/* LMC0_MODEREG_PARAMS1 */
#define MODEREG_PARAMS1_1RANK_1SLOT             \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = 0,            \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = 0,            \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = 0,            \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0,            \
        }                                       \
    }

#define MODEREG_PARAMS1_1RANK_2SLOT             \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = rttwr_120ohm, \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = rttwr_120ohm, \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = rttnom_40ohm, \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0             \
        }                                       \
    }

#define MODEREG_PARAMS1_2RANK_1SLOT             \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = 0,            \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = 0,            \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = 0,            \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0,            \
        }                                       \
    }

#define MODEREG_PARAMS1_2RANK_2SLOT                     \
    { .cn63xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00    = 0,                    \
                  .dic_00       = 1,                    \
                  .rtt_nom_00   = rttnom_120ohm,        \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01    = 0,                    \
                  .dic_01       = 1,                    \
                  .rtt_nom_01   = rttnom_40ohm,         \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10    = 0,                    \
                  .dic_10       = 1,                    \
                  .rtt_nom_10   = rttnom_120ohm,        \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = 0,                    \
                  .dic_11       = 1,                    \
                  .rtt_nom_11   = rttnom_40ohm,         \
        }                                               \
    }

#define CN31XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   ODT_ENA  WODT_CTL0       WODT_CTL1      reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======= ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   0,    0x00000100, {.u64=0x00000000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   0,    0x01000400, {.u64=0x00000000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   3 */ {   0,    0x01000400, {.u64=0x00000400}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   4 */ {   0,    0x01000400, {.u64=0x04000400}, {.u64=0x0000},    2,   0x0000,  0  }

#define CN31XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   ODT_ENA  WODT_CTL0     WODT_CTL1        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======= ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   0,    0x00000101, {.u64=0x00000000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   0,    0x01010404, {.u64=0x00000000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   3 */ {   0,    0x01010404, {.u64=0x00000404}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   4 */ {   0,    0x01010404, {.u64=0x04040404}, {.u64=0x0000},    2,   0x0000,  0  }

/* CN30xx is the same as CN31xx */
#define CN30XX_DRAM_ODT_1RANK_CONFIGURATION CN31XX_DRAM_ODT_1RANK_CONFIGURATION
#define CN30XX_DRAM_ODT_2RANK_CONFIGURATION CN31XX_DRAM_ODT_2RANK_CONFIGURATION

#define CN38XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   ODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======= ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   0,    0x00000001, {.u64=0x0000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   0,    0x00010001, {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   3 */ {   0,    0x01040104, {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   4 */ {   0,    0x01040104, {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  }

#define CN38XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   ODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======= ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   0,    0x00000011, {.u64=0x0000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   0,    0x00110011, {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   3 */ {   0,    0x11441144, {.u64=0x0000}, {.u64=0x0000},    3,   0x0000,  0  },  \
    /*   4 */ {   0,    0x11441144, {.u64=0x0000}, {.u64=0x0000},    3,   0x0000,  0  }

/* Note: CN58XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN58XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000001,  {.u64=0x0000}, {.u64=0x0000},    2,   0x00000001,  0  },  \
    /*   2 */ {   2,    0x00010001,  {.u64=0x0000}, {.u64=0x0000},    2,   0x00010001,  0  },  \
    /*   3 */ {   2,    0x01040104,  {.u64=0x0000}, {.u64=0x0000},    2,   0x01040104,  0  },  \
    /*   4 */ {   2,    0x01040104,  {.u64=0x0000}, {.u64=0x0000},    2,   0x01040104,  0  }

/* Note: CN58XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN58XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000011,  {.u64=0x0000}, {.u64=0x0000},    2,   0x00000011,  0  },  \
    /*   2 */ {   2,    0x00110011,  {.u64=0x0000}, {.u64=0x0000},    2,   0x00110011,  0  },  \
    /*   3 */ {   2,    0x11441144,  {.u64=0x0000}, {.u64=0x0000},    2,   0x11441144,  0  },  \
    /*   4 */ {   2,    0x11441144,  {.u64=0x0000}, {.u64=0x0000},    2,   0x11441144,  0  }

/* Note: CN50XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN50XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000001,  {.u64=0x0000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   2,    0x00010001,  {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   3 */ {   2,    0x01040104,  {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   4 */ {   2,    0x01040104,  {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  }

/* Note: CN50XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN50XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved       reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ============== ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000011,  {.u64=0x0000}, {.u64=0x0000},    1,   0x0000,  0  },  \
    /*   2 */ {   2,    0x00110011,  {.u64=0x0000}, {.u64=0x0000},    2,   0x0000,  0  },  \
    /*   3 */ {   2,    0x11441144,  {.u64=0x0000}, {.u64=0x0000},    3,   0x0000,  0  },  \
    /*   4 */ {   2,    0x11441144,  {.u64=0x0000}, {.u64=0x0000},    3,   0x0000,  0  }

/* Note: CN56XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN56XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA  WODT_CTL0     WODT_CTL1        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   2,     0x00000100, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0001,  0  },  \
    /*   2 */ {   2,     0x01000400, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0104,  0  },  \
    /*   3 */ {   2,     0x01000400, {.u64=0x00000400}, {.u64=0x0000},    3,   0x0104,  0  },  \
    /*   4 */ {   2,     0x01000400, {.u64=0x04000400}, {.u64=0x0000},    3,   0x0104,  0  }

/* Note: CN56XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN56XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA  WODT_CTL0     WODT_CTL1        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   2,     0x00000101, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0011,  0  },  \
    /*   2 */ {   2,     0x01010404, {.u64=0x00000000}, {.u64=0x0000},    3,   0x1144,  0  },  \
    /*   3 */ {   2,     0x01010404, {.u64=0x00000404}, {.u64=0x0000},    3,   0x1144,  0  },  \
    /*   4 */ {   2,     0x01010404, {.u64=0x04040404}, {.u64=0x0000},    3,   0x1144,  0  }

/* Note: CN52XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN52XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA  WODT_CTL0     WODT_CTL1        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   2,     0x00000100, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0001,  0  },  \
    /*   2 */ {   2,     0x01000400, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0104,  0  },  \
    /*   3 */ {   2,     0x01000400, {.u64=0x00000400}, {.u64=0x0000},    3,   0x0104,  0  },  \
    /*   4 */ {   2,     0x01000400, {.u64=0x04000400}, {.u64=0x0000},    3,   0x0104,  0  }

/* Note: CN52XX RODT_ENA 0 = disabled, 1 = Weak Read ODT, 2 = Strong Read ODT */
#define CN52XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA  WODT_CTL0     WODT_CTL1        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ================== ============== ====== ======== === */ \
    /*   1 */ {   2,     0x00000101, {.u64=0x00000000}, {.u64=0x0000},    3,   0x0011,  0  },  \
    /*   2 */ {   2,     0x01010404, {.u64=0x00000000}, {.u64=0x0000},    3,   0x1144,  0  },  \
    /*   3 */ {   2,     0x01010404, {.u64=0x00000404}, {.u64=0x0000},    3,   0x1144,  0  },  \
    /*   4 */ {   2,     0x01010404, {.u64=0x04040404}, {.u64=0x0000},    3,   0x1144,  0  }

#define CN63XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   reserved  WODT_MASK     LMCX_MODEREG_PARAMS1         reserved       RODT_CTL    RODT_MASK    reserved */ \
    /* =====   ======== ============== ============================ ============== ========= ============== ======== */ \
    /*   1 */ {   0,    0x00000001ULL, MODEREG_PARAMS1_1RANK_1SLOT, {.u64=0x0000},    3,     0x00000000ULL,  0  },      \
    /*   2 */ {   0,    0x00050005ULL, MODEREG_PARAMS1_1RANK_2SLOT, {.u64=0x0000},    3,     0x00010004ULL,  0  }

#define CN63XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   reserved  WODT_MASK     LMCX_MODEREG_PARAMS1         reserved       RODT_CTL    RODT_MASK    reserved */ \
    /* =====   ======== ============== ============================ ============== ========= ============== ======== */ \
    /*   1 */ {   0,    0x00000101ULL, MODEREG_PARAMS1_2RANK_1SLOT, {.u64=0x0000},    3,     0x00000000ULL,    0  },    \
    /*   2 */ {   0,    0x06060909ULL, MODEREG_PARAMS1_2RANK_2SLOT, {.u64=0x0000},    3,     0x02020808ULL,    0  }

#endif  /*  __LIB_OCTEON_SHARED_H__  */
