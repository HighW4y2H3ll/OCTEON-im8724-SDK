/***********************license start************************************
 * Copyright (c) 2005-2011 Cavium, Inc. <support@cavium.com>.  All rights
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
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#ifndef __OCTEON_EEPROM_TYPES_H__
#define __OCTEON_EEPROM_TYPES_H__

enum eeprom_types_enum {
	EEPROM_NULL_TYPE = 0,
	EEPROM_CLOCK_DESC_TYPE,
	EEPROM_BOARD_DESC_TYPE,
	EEPROM_CHIP_CAPABILITY_TYPE,
	EEPROM_MAC_ADDR_TYPE,
	EEPROM_VOLT_MULT_TYPE,	/* Not used on 6XXX boards */
	EEPROM_RLDRAM_DESC_TYPE,
	EEPROM_DDR_CLOCK_DESC_TYPE,
	EEPROM_MODULE_CAPABILITY_TYPE,
	EEPROM_VOLTAGE_TYPE,	/* New for 6XXX boards */
	EEPROM_MULTIPLIER_TYPE,	/* New for 6XXX boards */
	EEPROM_DEFAULT_MODULE_CAPABILITY_TYPE,	/* New for 6XXX boards */
	/* TLV marked for deletetion - MCU will skip over, U-BOOT can clean
	 * house
	 */
	EEPROM_DELETE_ME,
	/** Output verbose information during DDR initialization */
	EEPROM_DDR_VERBOSE_TYPE,
	EEPROM_MAX_TYPE,
	/* Start of range (inclusive) for customer use */
	EEPROM_CUSTOMER_RESERVED_START = 0xf000,
	/* End of range (inclusive) for customer use */
	EEPROM_CUSTOMER_RESERVED_END = 0xff00,
	EEPROM_END_TYPE = 0xffff
};

#define ENUM_TYPE_CASE(x)   case x: return(#x + 7);	/* Skip EEPROM_ */
static inline char *eeprom_type_to_string (enum eeprom_types_enum type)
{
	if (type >= EEPROM_CUSTOMER_RESERVED_START
	    && type <= EEPROM_CUSTOMER_RESERVED_END)
		return ("Unknown customer tuple type");
	if (type >= EEPROM_MAX_TYPE)
		return ("Unknown tuple type");

	switch (type) {
		ENUM_TYPE_CASE (EEPROM_NULL_TYPE)
		ENUM_TYPE_CASE (EEPROM_CLOCK_DESC_TYPE)
		ENUM_TYPE_CASE (EEPROM_BOARD_DESC_TYPE)
		ENUM_TYPE_CASE (EEPROM_CHIP_CAPABILITY_TYPE)
		ENUM_TYPE_CASE (EEPROM_MAC_ADDR_TYPE)
		ENUM_TYPE_CASE (EEPROM_VOLT_MULT_TYPE)
		ENUM_TYPE_CASE (EEPROM_RLDRAM_DESC_TYPE)
		ENUM_TYPE_CASE (EEPROM_DDR_CLOCK_DESC_TYPE)
		ENUM_TYPE_CASE (EEPROM_MODULE_CAPABILITY_TYPE)
		ENUM_TYPE_CASE (EEPROM_VOLTAGE_TYPE)
		ENUM_TYPE_CASE (EEPROM_MULTIPLIER_TYPE)
		ENUM_TYPE_CASE (EEPROM_DEFAULT_MODULE_CAPABILITY_TYPE)
		ENUM_TYPE_CASE (EEPROM_DELETE_ME)
		ENUM_TYPE_CASE (EEPROM_DDR_VERBOSE_TYPE)
		ENUM_TYPE_CASE (EEPROM_MAX_TYPE)
		ENUM_TYPE_CASE (EEPROM_CUSTOMER_RESERVED_START)
		ENUM_TYPE_CASE (EEPROM_CUSTOMER_RESERVED_END)
		ENUM_TYPE_CASE (EEPROM_END_TYPE)
	default:
		return ("Unknown tuple type");
	}
}

/* Header used by all blocks in the EEPROM.  Checksum covers
** the header (excluding the checksum) and all payload bytes.
*/
typedef struct {
	uint16_t type;
	uint16_t length;
	uint8_t minor_version;	/* Minor version increment indicates backward
				 * compatible change
				 */
	uint8_t major_version;	/* Major version must be changed if incompatible
				 * change made
				 */
	uint16_t checksum;
} octeon_eeprom_header_t;

/*
 * There is a structure defined for each major revision of each tuple type, with
 * the generic (non-versioned) name referring to the current version.
 * The current version (major/minor) for each tuple type is defined for
 * each tuple as well.
 *
 */

/* Maximum length allowed for a tuple.  This is used for simple
 * header validity checks.
 */
#define OCTEON_EEPROM_MAX_TUPLE_LENGTH   256

/********************************************************************/
/*
 * NOTE: The clock descriptor is used by the MCU, so any changes made to the
 * structure/type definitions must be made there as well.
 */

struct octeon_eeprom_clock_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t ddr_clock_mhz;	/* May be overridden by ddr_clock_desc tuple */
	uint16_t cpu_ref_clock_mhz_x_8;	/* Ref clock Mhz * 8 */
	uint16_t spi_clock_mhz;
};

/* Unused fields should be set to 0 */
struct octeon_eeprom_clock_desc_v2 {
	octeon_eeprom_header_t header;
	uint16_t ddr_clock_mhz;
	uint16_t cpu_ref_clock_mhz_x_8;	/* Ref clock Mhz * 8 */
	uint16_t dfa_ref_clock_mhz_x_8;	/* Ref clock Mhz * 8 */
	uint16_t spi_clock_mhz;
};

/* We'll need to change this based on chip type, as Octeon uses version 1, and
 * newer chips use version 2
 */
/* This define always corresponds to the latest structure version, and the
 * latest structure version always has the generic name
 */
#define OCTEON_EEPROM_CLOCK_DESC_MAJOR_VER    2
#define OCTEON_EEPROM_CLOCK_DESC_MINOR_VER    0
typedef struct octeon_eeprom_clock_desc_v2 octeon_eeprom_clock_desc_t;

struct octeon_eeprom_cpu_mul_vol_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t cpu_voltage_millivolts;	/* voltage in millivolts */
	uint16_t cpu_multiplier;		/* Raw value */
};
#define OCTEON_EEPROM_CPU_MUL_VOL_DESC_VER    1
typedef struct octeon_eeprom_cpu_mul_vol_desc_v1
    octeon_eeprom_cpu_mul_vol_desc_t;
/********************************************************************/

/********************************************************************/
#ifndef OCTEON_SERIAL_LEN
#define OCTEON_SERIAL_LEN 20
#endif
struct octeon_eeprom_board_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t board_type;
	uint8_t rev_major;
	uint8_t rev_minor;
	uint16_t chip_type;	/* Deprecated */
	uint8_t chip_rev_major;	/* Deprecated */
	uint8_t chip_rev_minor;	/* Deprecated */
	uint8_t serial_str[OCTEON_SERIAL_LEN];	/* Must be nul terminated */
};
#define OCTEON_EEPROM_BOARD_DESC_MAJOR_VER    1
#define OCTEON_EEPROM_BOARD_DESC_MINOR_VER    0
typedef struct octeon_eeprom_board_desc_v1 octeon_eeprom_board_desc_t;
/********************************************************************/

/********************************************************************/
struct octeon_eeprom_chip_capability_v1 {
	octeon_eeprom_header_t header;
	uint16_t coremask;
	uint16_t voltage_x100;
	uint16_t cpu_freq_mhz;
};
#define OCTEON_EEPROM_CHIP_CAPABILITY_MAJOR_VER    1
#define OCTEON_EEPROM_CHIP_CAPABILITY_MINOR_VER    0
typedef struct octeon_eeprom_chip_capability_v1 octeon_eeprom_chip_capability_t;
/********************************************************************/

/********************************************************************/
struct octeon_eeprom_module_capability_v1 {
	octeon_eeprom_header_t header;
	uint16_t protocol;
	uint16_t speed;
	uint16_t cfg_flags;	/* Reserved for future use (added in rev 1.1) */
/************************************************************************/
/*  Flags Definition                                                    */
/*  bit 0 - Reverse Lanes (added in 1.1)                                */
/*  bit 1 - Use Common Ref Clock (added in 1.1)                         */
/*  bit 2 - SRIO End Point Mode (SRIO is normally host) (added in 1.2)  */
/*  bit 3 - Board End Point Mode (for board to board)   (added in 1.4)  */
/*  bits [4-13] - Reserved for future use, set to 0 when programming    */
/*  bit 14 - invalid config (used internally by MCU only)(added in 1.3) */
/*  bit 15 - ignore TLV if set to 1 (added in 1.1)                      */
/************************************************************************/
};
#define OCTEON_EEPROM_MODULE_CAPABILITY_MAJOR_VER    1
#define OCTEON_EEPROM_MODULE_CAPABILITY_MINOR_VER    4
typedef struct octeon_eeprom_module_capability_v1
    octeon_eeprom_module_capability_t;
/********************************************************************/

/********************************************************************/
struct octeon_eeprom_default_module_capability_v1 {
	octeon_eeprom_header_t header;
	uint16_t qlm_number;
	uint16_t protocol;
	uint16_t speed;
	uint16_t cfg_flags;	/* Reserved for future use (added in rev 1.0) */
/************************************************************************/
/*  Flags Definition                                                    */
/*  bit 0 - Reverse Lanes (added in 1.0)                                */
/*  bit 1 - Use Common Ref Clock (added in 1.0)                         */
/*  bit 2 - SRIO End Point Mode (SRIO is normally host) (added in 1.1)  */
/*  bit 3 - Board End Point Mode (for board to board)   (added in 1.3)  */
/*  bits [4-13] - Reserved for future use, set to 0 when programming    */
/*  bit 14 - invalid config (used internally by MCU only)(added in 1.2) */
/*  bit 15 - ignore TLV if set to 1                                     */
/************************************************************************/
	uint16_t raw_cfg;	/* used to set a specific setting on the cfg
				 * switches - keep 0xFFFF to not use
				 */
	uint16_t raw_speed;	/* used to set a specific setting on the speed
				 * switches - keep 0xFFFF to not use
				 */
	uint16_t raw_fsel;  	/* used to set a specific setting on the speed
				 * switches - keep 0xFFFF to not use.
				 */
};
#define OCTEON_EEPROM_DEFAULT_MODULE_CAPABILITY_MAJOR_VER    1
#define OCTEON_EEPROM_DEFAULT_MODULE_CAPABILITY_MINOR_VER    4
typedef struct octeon_eeprom_default_module_capability_v1
    octeon_eeprom_default_module_capability_t;
/********************************************************************/

/********************************************************************/
struct octeon_eeprom_mac_addr_v1 {
	octeon_eeprom_header_t header;
	uint8_t mac_addr_base[6];
	uint8_t count;
};
#define OCTEON_EEPROM_MAC_ADDR_MAJOR_VER    1
#define OCTEON_EEPROM_MAC_ADDR_MINOR_VER    0
typedef struct octeon_eeprom_mac_addr_v1 octeon_eeprom_mac_addr_t;
/********************************************************************/

/********************************************************************/
struct octeon_eeprom_voltage_multiplier_v1 {
	octeon_eeprom_header_t header;
	uint16_t voltage_millivolts;
	uint16_t cpu_multiplier;
};
#define OCTEON_EEPROM_VOLT_MULT_MAJOR_VER    1
#define OCTEON_EEPROM_VOLT_MULT_MINOR_VER    0
typedef struct octeon_eeprom_voltage_multiplier_v1
    octeon_eeprom_voltage_multiplier_t;
/********************************************************************/

/********************************************************************/
/* This tuple specifies options for RLDRAM on CN38XX/CN58XX boards.
** Only stuffing related options are stored here - the address swizzle
** is specified in the initialization code for each board type. */
struct octeon_eeprom_rldram_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t rld0_mbytes;
	uint16_t rld1_mbytes;
	uint8_t rld0_bunks;
	uint8_t rld1_bunks;
	uint16_t max_rld_clock_mhz;
};
#define OCTEON_EEPROM_RLDRAM_DESC_MAJOR_VER    1
#define OCTEON_EEPROM_RLDRAM_DESC_MINOR_VER    0
typedef struct octeon_eeprom_rldram_desc_v1 octeon_eeprom_rldram_desc_t;
/********************************************************************/

/********************************************************************/
/* If present, this tuple overrides any ddr clock specified in the
** the clock descriptor tuple.
** If ddr_ref_hz is 0, then the CPU reference frequency is used.
*/
struct octeon_eeprom_ddr_clock_desc_v1 {
	octeon_eeprom_header_t header;
	uint32_t ddr_ref_hz;	/* Reference clock input in Hz */
	/* NOTE: ddr_ref_hz is often ignored,
	 ** as many Octeon chips only support a 50 MHz
	 ** reference clock. */
	uint32_t ddr_clock_hz;	/* Desired DDR clock output in Hz */
};
#define OCTEON_EEPROM_DDR_CLOCK_DESC_MAJOR_VER    1
#define OCTEON_EEPROM_DDR_CLOCK_DESC_MINOR_VER    0
typedef struct octeon_eeprom_ddr_clock_desc_v1 octeon_eeprom_ddr_clock_desc_t;
/********************************************************************/

/********************************************************************/
/* Specifies the voltage for Octeon (New for 6XXX)
*/
struct octeon_eeprom_voltage_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t octeon_core_voltage_mv; /* Octeon core voltage in millivolts */
};
#define OCTEON_EEPROM_VOLTAGE_MAJOR_VER    1
#define OCTEON_EEPROM_VOLTAGE_MINOR_VER    0
typedef struct octeon_eeprom_voltage_desc_v1 octeon_eeprom_voltage_desc_t;
/********************************************************************/

/********************************************************************/
/* Specifies the multipliers for Octeon. (New for 6XXX)
*/
struct octeon_eeprom_multiplier_desc_v1 {
	octeon_eeprom_header_t header;
	uint16_t octeon_core_mult;	/* Octeon core multiplier */
	uint16_t octeon_io_mult;	/* Octeon core multiplier */
};
#define OCTEON_EEPROM_MULTIPLIER_MAJOR_VER    1
#define OCTEON_EEPROM_MULTIPLIER_MINOR_VER    0
typedef struct octeon_eeprom_multiplier_desc_v1 octeon_eeprom_multiplier_desc_t;
/********************************************************************/

/********************************************************************/
/**
 * Specifies that verbose debugging information should be displayed during
 * DDR initialization.
 */
struct octeon_eeprom_ddr_verbose_v1 {
	octeon_eeprom_header_t header;
	uint32_t verbosity;	/** Bitmask of DDR verbosity */
};
#define OCTEON_EEPROM_DDR_VERBOSE_MAJOR_VER	1
#define OCTEON_EEPROM_DDR_VERBOSE_MINOR_VER	0
typedef struct octeon_eeprom_ddr_verbose_v1 octeon_eeprom_ddr_verbose_t;
/********************************************************************/

#endif /* __OCTEON_EEPROM_TYPES_H__ */
