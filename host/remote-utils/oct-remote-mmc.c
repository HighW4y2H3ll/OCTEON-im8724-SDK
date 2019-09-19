/***********************license start************************************
 * Copyright (c) 2014 Cavium Inc. (support@cavium.com).
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 **********************license end**************************************/

/**
 * @file
 *
 * Write a stage 1 or stage 2 bootloader to an eMMC device
 *
 * $Id: oct-remote-mmc.c 75411 2012-07-31 20:00:40Z cchavva $
 */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <byteswap.h>

#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */

#ifdef DEBUG
#define debug(format, args...)	printf(fmt, ##args)
#else
#define debug(format, args...)
#endif

#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "octeon-model.h"
#include <octeon_eeprom_types.h>
#include <lib_octeon_shared.h>
#include <octeon_mem_map.h>
#include "cvmx-mio-defs.h"
#include "cvmx-clock.h"
#include "cvmx-bootloader.h"

#define REF_CLOCK	50000000
#define MMC_MAX_SPEED	20000000
#define MMC_MIN_SPEED	400000
#define FAT_SECTOR_SIZE	512
#define MMC_MAX_WIDTH	8
#define STAGE1_SIZE	8192
#define STAGE2_START	(STAGE1_SIZE / FAT_SECTOR_SIZE)

#define MAX_SD_SPEED		25000000
#define MAX_MMC_SPEED		20000000

#define SD_VERSION_SD		0x20000
#define SD_VERSION_3		(SD_VERSION_SD | 0x300)
#define SD_VERSION_2		(SD_VERSION_SD | 0x200)
#define SD_VERSION_1_0		(SD_VERSION_SD | 0x100)
#define SD_VERSION_1_10		(SD_VERSION_SD | 0x10a)
#define MMC_VERSION_MMC		0x10000
#define MMC_VERSION_UNKNOWN	(MMC_VERSION_MMC)
#define MMC_VERSION_1_2		(MMC_VERSION_MMC | 0x102)
#define MMC_VERSION_1_4		(MMC_VERSION_MMC | 0x104)
#define MMC_VERSION_2_2		(MMC_VERSION_MMC | 0x202)
#define MMC_VERSION_3		(MMC_VERSION_MMC | 0x300)
#define MMC_VERSION_4		(MMC_VERSION_MMC | 0x400)
#define MMC_VERSION_4_1		(MMC_VERSION_MMC | 0x401)
#define MMC_VERSION_4_2		(MMC_VERSION_MMC | 0x402)
#define MMC_VERSION_4_3		(MMC_VERSION_MMC | 0x403)
#define MMC_VERSION_4_41	(MMC_VERSION_MMC | 0x429)
#define MMC_VERSION_4_5		(MMC_VERSION_MMC | 0x405)

#define MMC_MODE_HS		0x001
#define MMC_MODE_HS_52MHz	0x010
#define MMC_MODE_4BIT		0x100
#define MMC_MODE_8BIT		0x20000
#define IS_SD(x)		((x)->version & SD_VERSION_SD)

#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SET_DSR			4
#define MMC_CMD_SWITCH			6
#define MMC_CMD_SELECT_CARD		7
#define MMC_CMD_SEND_EXT_CSD		8
#define MMC_CMD_SEND_CSD		9
#define MMC_CMD_SEND_CID		10
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SEND_STATUS		13
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define MMC_CMD_WRITE_SINGLE_BLOCK	24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK	25
#define MMC_CMD_ERASE_GROUP_START	35
#define MMC_CMD_ERASE_GROUP_END		36
#define MMC_CMD_ERASE			38
#define MMC_CMD_APP_CMD			55

#define SD_CMD_SEND_RELATIVE_ADDR	3
#define SD_CMD_SWITCH_FUNC		6
#define SD_CMD_SEND_IF_COND		8

#define SD_CMD_APP_SET_BUS_WIDTH	6
#define SD_CMD_ERASE_WR_BLK_START	32
#define SD_CMD_ERASE_WR_BLK_END		33
#define SD_CMD_APP_SEND_OP_COND		41
#define SD_CMD_APP_SEND_SCR		51

#define MMC_FLAG_HIGH_CAPACITY		(1 << 0)
#define MMC_FLAG_HIGH_SPEED		(1 << 1)
#define MMC_FLAG_HIGH_SPEED_52MHz	(1 << 2)
#define MMC_FLAG_HIGH_SPEED_DDR		(1 << 3)

#define EXT_CSD_DEVICE_TYPE_26MHz		(1 << 0)
#define EXT_CSD_DEVICE_TYPE_52MHz		(1 << 1)
#define EXT_CSD_DEVICE_TYPE_DDR_52MHz_3V	(1 << 2)
#define EXT_CSD_DEVICE_TYPE_DDR_52MHz_1_2V	(1 << 3)
#define EXT_CSD_DEVICE_TYPE_200MHz_1_8V		(1 << 4)
#define EXT_CSD_DEVICE_TYPE_200MHz_1_2V		(1 << 5)

#define EXT_CSD_BUS_WIDTH_1		0
#define EXT_CSD_BUS_WIDTH_4		1
#define EXT_CSD_BUS_WIDTH_8		2
#define EXT_CSD_DDR_BUS_WIDTH_4		5
#define EXT_CSD_DDR_BUS_WIDTH_8		6

/*
 *  NOTE: This was copied from the Linux kernel.
 *
 *  MMC status in R1, for native mode (SPI bits are different)
 *  Type
 *        e : error bit
 *        s : status bit
 *        r : detected and set for the actual command response
 *        x : detected and set during command execution. the host must poll
 *            the card by sending status command in order to read these bits.
 *  Clear condition
 *        a : according to the card state
 *        b : always related to the previous command. Reception of
 *            a valid command will clear it (with a delay of one command)
 *        c : clear by read
 */

#define R1_OUT_OF_RANGE         (1 << 31)       /* er, c */
#define R1_ADDRESS_ERROR        (1 << 30)       /* erx, c */
#define R1_BLOCK_LEN_ERROR      (1 << 29)       /* er, c */
#define R1_ERASE_SEQ_ERROR      (1 << 28)       /* er, c */
#define R1_ERASE_PARAM          (1 << 27)       /* ex, c */
#define R1_WP_VIOLATION         (1 << 26)       /* erx, c */
#define R1_CARD_IS_LOCKED       (1 << 25)       /* sx, a */
#define R1_LOCK_UNLOCK_FAILED   (1 << 24)       /* erx, c */
#define R1_COM_CRC_ERROR        (1 << 23)       /* er, b */
#define R1_ILLEGAL_COMMAND      (1 << 22)       /* er, b */
#define R1_CARD_ECC_FAILED      (1 << 21)       /* ex, c */
#define R1_CC_ERROR             (1 << 20)       /* erx, c */
#define R1_ERROR                (1 << 19)       /* erx, c */
#define R1_UNDERRUN             (1 << 18)       /* ex, c */
#define R1_OVERRUN              (1 << 17)       /* ex, c */
#define R1_CID_CSD_OVERWRITE    (1 << 16)       /* erx, c, CID/CSD overwrite */
#define R1_WP_ERASE_SKIP        (1 << 15)       /* sx, c */
#define R1_CARD_ECC_DISABLED    (1 << 14)       /* sx, a */
#define R1_ERASE_RESET          (1 << 13)       /* sr, c */
#define R1_STATUS(x)            (x & 0xFFFFE000)
#define R1_CURRENT_STATE(x)     ((x & 0x00001E00) >> 9) /* sx, b (4 bits) */
#define R1_READY_FOR_DATA       (1 << 8)        /* sx, a */
#define R1_SWITCH_ERROR         (1 << 7)        /* sx, c */
#define R1_APP_CMD              (1 << 5)        /* sr, c */

#define R1_BLOCK_READ_MASK	R1_OUT_OF_RANGE |	\
				R1_ADDRESS_ERROR |	\
				R1_BLOCK_LEN_ERROR |	\
				R1_CARD_IS_LOCKED |	\
				R1_COM_CRC_ERROR |	\
				R1_ILLEGAL_COMMAND |	\
				R1_CARD_ECC_FAILED |	\
				R1_CC_ERROR |		\
				R1_ERROR;
#define R1_BLOCK_WRITE_MASK	R1_OUT_OF_RANGE |	\
				R1_ADDRESS_ERROR |	\
				R1_BLOCK_LEN_ERROR |	\
				R1_WP_VIOLATION |	\
				R1_CARD_IS_LOCKED |	\
				R1_COM_CRC_ERROR |	\
				R1_ILLEGAL_COMMAND |	\
				R1_CARD_ECC_FAILED |	\
				R1_CC_ERROR |		\
				R1_ERROR |		\
				R1_UNDERRUN |		\
				R1_OVERRUN

enum mmc_states {
	IDLE,
	READY,
	IDENT,
	STANDBY,
	TRAN,
	DATA,
	RECEIVE,
	PROGRAM,
	DISABLED,
	BTST,
	SLP
};
const char *mmc_state_str[16] = {
	"IDLE",
	"READY",
	"IDENT",
	"STANDBY",
	"TRAN",
	"DATA",
	"RECEIVE",
	"PROGRAM",
	"DISABLED",
	"BTST",
	"SLP",
	"RESERVED 11",
	"RESERVED 12",
	"RESERVED 13",
	"RESERVED 14",
	"RESERVED 15"
};

/**
 * CID register
 */
typedef union {
	struct {
		uint8_t		mid;		/** Manufacturer ID (MID) */
		uint8_t		reserved:6;	/** Reserved */
		uint8_t		device:2;	/** Device/BGA (CBX) */
		uint8_t		oem;		/** OEM/Application ID (OID) */
		char		prod_name[6];	/** Product name (PNM) */
		uint8_t		revision:8;	/** Product revision (PRV) */
		uint32_t	serial_no:32;	/** Product serial number (PSN) */
		uint8_t		manu_date:8;	/** Manufacturer date (MDT)*/
		uint8_t		crc:7;		/** CRC7 checksum (CRC) */
		uint8_t		one:1;		/** Unused, always 1 */
	} __attribute__((packed)) mmc;
	struct {
		uint8_t		mid;		/** Manufacturer ID */
		uint16_t	oem;		/** OEM/Application ID */
		char		prod_name[5];	/** Product name */
		uint8_t		revision;	/** Product revision */
		uint32_t	serial_no;	/** Product serial number */
		uint16_t	reserved:4;
		uint16_t	manu_date_y:8;	/** Manufacturer date (year) */
		uint16_t	manu_date_m:4;	/** Manufacturer date (month) */
		uint8_t		crc:7;		/** CRC7 checksum */
		uint8_t		one:1;		/** Unused, always 1 */
	} __attribute__((packed)) sd;
	uint64_t	u128[2];
} __attribute__((packed)) mmc_cid_t;

/**
 * CSD register
 */
typedef union csd {
	struct {
		uint64_t	csd_structure:2;
		uint64_t	spec_vers:4;
		uint64_t	:2;			/* Reserved */
		uint64_t	taac:8;			/* Data read access-time 1 */
		uint64_t	nsac:8;			/* Data read access-time 2 in CLK cycles (NSAC*100) */
		uint64_t	tran_speed:8;		/* Max bus clock frequency */
		uint64_t	ccc:12;			/* Device command classes */
		uint64_t	read_bl_len:4;		/* Max read data block length */
		uint64_t	read_bl_partial:1;	/* Partial blocks for read allowed */
		uint64_t	write_blk_misalign:1;	/* Write block misalignment */
		uint64_t	read_blk_misalign:1;	/* Read block misalignment */
		uint64_t	dsr_imp:1;		/* DSR implemented */
		uint64_t	:2;			/* Reserved */
		uint64_t	c_size:12;		/* Device size */
		uint64_t	vdd_r_curr_min:3;	/* Max read current @VDD min */
		uint64_t	vdd_r_curr_max:3;	/* Max read current @VDD max */
		uint64_t	vdd_w_curr_min:3;	/* Max write current @VDD min */
		uint64_t	vdd_w_curr_max:3;	/* Max write current @VDD max */
		uint64_t	c_size_mult:3;		/* Device size multiplier */
		uint64_t	erase_grp_size:5;	/* Erase group size */
		uint64_t	erase_grp_mult:5;	/* Erase group size multiplier */
		uint64_t	wp_grp_size:5;		/* Write protect group size */
		uint64_t	wp_grp_enable:1;	/* Write protect group enable */
		uint64_t	default_ecc:2;		/* Manufacturer default ECC */
		uint64_t	r2w_factor:3;		/* Write speed factor */
		uint64_t	write_bl_len:4;		/* Max write data block length */
		uint64_t	write_bl_partial:1;	/* Partial blocks for write allowed */
		uint64_t	:4;			/* Reserved */
		uint64_t	content_prot_app:1;	/* Content protection application */
		uint64_t	file_format_grp:1;	/* File format group */
		uint64_t	copy:1;			/* Copy flag (OTP) */
		uint64_t	perm_write_protect:1;	/* Permanent write protection */
		uint64_t	tmp_write_protect:1;	/* Temporary write protection */
		uint64_t	file_format:2;		/* File format */
		uint64_t	ecc:2;			/* ECC code */
		uint64_t	crc:7;			/* CRC */
		uint64_t	:1;			/* Reserved, always 1 */
	} s __attribute__((packed));
	uint64_t	u128[2];
} mmc_csd_t;

/**
 * Extended CSD register
 */
typedef union {
	struct {
		uint8_t		reserved_0_31[32];	/* 0 Reserved */
		uint8_t		flush_cache;		/* 32 Flushing of the cache */
		uint8_t		cache_ctrl;		/* 33 Control to turn the cache on/off */
		uint8_t		power_off_notification;	/* 34 Power off notification */
		uint8_t		packed_failure_index;	/* 35 Packed command failure index */
		uint8_t		packed_command_status;	/* 36 Packed command status */
		uint8_t		context_conf[15];	/* 37-51 Context configuration */
		uint16_t	ext_partitions_attribute;/* 52-53 Extended partitions attribute */
		uint16_t	exception_events_status;/* 54-55 Exception events status */
		uint16_t	exception_events_ctrl;	/* 56-57 Exception events control */
		uint8_t		dyncap_needed;		/* 58 Number of addressed group to be released */
		uint8_t		class_6_ctrl;		/* 59 Class 6 commands control */
		uint8_t		ini_timeout_emu;	/* 60 1st initialization after disabling sector size emulation */
		uint8_t		data_sector_size;	/* 61 Sector size */
		uint8_t		use_native_sector;	/* 62 Sector size emulation */
		uint8_t		native_sector_size;	/* 63 Native sector size */
		uint8_t		vendor_specific_field[64];/* 64-127 Vendor specific fields */
		uint8_t		reserved_128_129[2];	/* 128-129 Reserved */
		uint8_t		program_cid_csd_ddr_support;/* 130 Program CID/CSD in DDR mode support */
		uint8_t		periodic_wakeup;	/* 131 Periodic wake-up */
		uint8_t 	tcase_support;		/* 132 Package case temperature is controlled */
		uint8_t		reserved_133;		/* 133 Reserved */
		uint8_t		sec_bad_blk_mgmnt;	/* 134 Bad block management mode */
		uint8_t		reserved_135;		/* 135 Reserved */
		uint32_t	enh_start_addr;		/* 136-139 Enhanced user data start address */
		uint8_t		enh_size_mult[3];	/* 140-142 1Enhanced user data area size */
		uint8_t		gp_size_mult[12];	/* 143-154 General purpose partition size */
		uint8_t		partition_setting_completed;	/* 155 Partitioning setting */
		uint8_t		partitions_attribute;	/* 156 Partitions attribute */
		uint8_t		max_enh_size_multi[3];	/* 157-159 Max enhanced area size */
		uint8_t		partitioning_support;	/* 160 Partitioning support */
		uint8_t		hpi_mgmt;		/* 161 HPI management */
		uint8_t		rst_n_function;		/* 162 H/W reset function */
		uint8_t		bkops_en;		/* 163 Enable background operations handshake */
		uint8_t		bkops_start;		/* 164 Manually start background operations handshake */
		uint8_t		sanatize_start;		/* 165 Start sanatize operation */
		uint8_t		wr_rel_param;		/* 166 Write reliability parameter register */
		uint8_t		wr_rel_set;		/* 167 Write reliability setting register */
		uint8_t		rpmb_size_mult;		/* 168 RPMB size */
		uint8_t		fw_config;		/* 169 FW configuration */
		uint8_t		reserved_170;		/* 170 Reserved */
		uint8_t		user_wp;		/* 171 User area write protection register */
		uint8_t		reserved_172;		/* 172 Reserved */
		uint8_t		boot_wp;		/* 173 Boot area write protection register */
		uint8_t		boot_wp_status;		/* 174 Boot write protection status registers */
		uint8_t		erase_group_def;	/* 175 High-density erase group definition */
		uint8_t		reserved_176;		/* 176 Reserved */
		uint8_t		boot_bus_conditions;	/* 177 Boot bus conditions */
		uint8_t		boot_config_prot;	/* 178 Boot config protection */
		uint8_t		partition_config;	/* 179 Partition configuration */
		uint8_t		reserved_180;		/* 180 Reserved */
		uint8_t		erased_mem_cont;	/* 181 Erased memory content */
		uint8_t		reserved_182;		/* 182 Reserved */
		uint8_t		bus_width;		/* 183 Bus width mode */
		uint8_t		reserved_184;		/* 184 Reserved */
		uint8_t		hs_timing;		/* 185 High-speed interface timing */
		uint8_t		reserved_186;		/* 186 Reserved */
		uint8_t		power_class;		/* 187 Power class */
		uint8_t		reserved_188;		/* 188 Reserved */
		uint8_t		cmd_set_rev;		/* 189 Command set revision */
		uint8_t		reserved_190;		/* 190 Reserved */
		uint8_t		cmd_set;		/* 191 Command set */
		uint8_t		ext_csd_rev;		/* 192 Extended CSD revision */
		uint8_t		reserved_193;		/* 193 Reserved */
		uint8_t		csd_struct_ver;		/* 194 CSD structure version */
		uint8_t		reserved_195;		/* 195 Reserved */
		uint8_t		device_type;		/* 196 Device type */
		uint8_t		driver_strength;	/* 197 I/O driver strength */
		uint8_t		out_of_interrupt_time;	/* 198 Out-of-interrupt busy timing */
		uint8_t		partition_switch_time;	/* 199 Partition switching timing */
		uint8_t		pwr_cl_52_195;		/* 200 Power class for 52MHz at 1.95V 1 R */
		uint8_t		pwr_cl_26_195;		/* 201 Power class for 26MHz at 1.95V 1 R */
		uint8_t		pwr_cl_52_360;		/* 202 Power class for 52MHz at 3.6V 1 R */
		uint8_t		pwr_cl_26_360;		/* 203 Power class for 26MHz at 3.6V 1 R */
		uint8_t		reserved_204;		/* 204 Reserved */
		uint8_t		min_perf_r_4_26;	/* 205 Minimum read performance for 4bit at 26MHz */
		uint8_t		min_perf_w_4_26;	/* 206 Minimum write performance for 4bit at 26MHz */
		uint8_t		min_perf_r_8_26_4_52;	/* 207 Minimum read performance for 8bit at 26MHz, 4bit at 52MHz */
		uint8_t		min_perf_w_8_26_4_52;	/* 208 Minimum write performance for 8bit at 26MHz, 4bit at 52MHz */
		uint8_t		min_perf_r_8_52;	/* 209 Minimum read performance for 8bit at 52MHz */
		uint8_t		min_perf_w_8_52;	/* 210 Minimum write performance for 8bit at 52MHz */
		uint8_t		reserved_211;		/* 211 Reserved */
		uint32_t	sec_count;		/* 212-215 Sector count */
		uint8_t		reserved_216;		/* 216 Reserved */
		uint8_t		s_a_timeout;		/* 217 Sleep/awake timeout */
		uint8_t		reserved_218;		/* 218 Reserved */
		uint8_t		s_c_vccq;		/* 219 Sleep current (VCCQ) */
		uint8_t		s_c_vcc;		/* 220 Sleep current (VCC) */
		uint8_t		hc_wp_grp_size;		/* 221 High-capacity write protect group size */
		uint8_t		rel_wr_sec_c;		/* 222 Reliable write sector count */
		uint8_t		erase_timeout_mult;	/* 223 High-capacity erase timeout */
		uint8_t		hc_erase_grp_size;	/* 224 High-capacity erase unit size */
		uint8_t		acc_size;		/* 225 Access size */
		uint8_t		boot_size_multi;	/* 226 Boot partition size */
		uint8_t		reserved_227;		/* 227 Reserved */
		uint8_t		boot_info;		/* 228 Boot information */
		uint8_t		obsolete_229;		/* 229 Obsolete */
		uint8_t		obsolete_230;		/* 230 Obsolete */
		uint8_t		sec_feature_support;	/* 231 Secure feature support */
		uint8_t		trim_mult;		/* 232 TRIM multiplier */
		uint8_t		reserved_233;		/* 233 Reserved */
		uint8_t		min_perf_ddr_r_8_52;	/* 234 Minimum read performance for 8bit at 52MHz in DDR mode */
		uint8_t		min_perf_ddr_w_8_52;	/* 235 Minimum write performance for 8bit at 52MHz in DDR mode */
		uint8_t		pwr_cl_200_195;		/* 236 Power class for 200MHz at 1.95V */
		uint8_t		pwr_cl_200_360;		/* 237 Power class for 200MHz at 3.6V */
		uint8_t		pwr_cl_ddr_52_195;	/* 238 Power class for 52MHz, DDR at 1.95V */
		uint8_t		pwr_cl_ddr_52_360;	/* 239 Power class for 52MHz, DDR at 3.6V */
		uint8_t		reserved240;		/* 240 Reserved */
		uint8_t		ini_timeout_ap;		/* 241 1st initialization time after partitioning */
		uint32_t	correctly_prg_sector_num;/* 242 Number of correctly programmed sectors */
		uint16_t	bkops_status;		/* 246 Background operations status */
		uint8_t		power_off_long_time;	/* 247 Power off notification(long) timeout */
		uint8_t		generic_cmd6_timeout;	/* 248 Generic CMD6 timeout */
		uint32_t	cache_size;		/* 249-252 Cache size */
		uint8_t		reserved253_493[255];	/* 253-493 Reserved */
		uint8_t		ext_support;		/* 494 Extended partitions attribute support */
		uint8_t		large_unit_size_m1;	/* 495 Large unit size */
		uint8_t		context_capabilities;	/* 496 Context management capabilities */
		uint8_t		tag_res_size;		/* 497 Tag resource size */
		uint8_t		tag_unit_size;		/* 498 Tag unit size */
		uint8_t		data_tag_support;	/* 499 Data tag support */
		uint8_t		max_packed_writes;	/* 500 Max packed write commands */
		uint8_t		max_packed_reads;	/* 501 Max packed read commands */
		uint8_t		bkops_support;		/* 502 Background operations support */
		uint8_t		hpi_features;		/* 503 HPI features */
		uint8_t		s_cmd_set;		/* 504 Supported Command Sets */
		uint8_t		reserved505_511[7];	/* 505-511 Reserved */
	} s __attribute__((packed));
	uint64_t buffer[FAT_SECTOR_SIZE/8];
} mmc_ext_csd_t;

union mmc_sd_scr {
	struct {
		uint64_t structure:4;		/** SCR Structure */
		uint64_t sd_spec:4;		/** SD Memory Card - Spec. Version */
		uint64_t data_stat_after_erase:1;/** data_status_after erases */
		uint64_t sd_security:3;		/** CPRM Security Support */
		uint64_t sd_bus_widths:4;	/** DAT Bus Widths supported */
		uint64_t sd_spec3:1;		/** Spec. Version 3.00 or higher */
		uint64_t ex_security:4;		/** Extended Security Support */
		uint64_t sd_spec4:1;		/** Spec. Version 4.00 or higher */
		uint64_t reserved:6;		/** Reserved */
		uint64_t cmd_support:4;		/** Command Support bits */
		uint64_t manu_reserved:32;	/** Reserved for manufacturer usage */
	} s;
	uint64_t u64;
};
/**
 * MMC data structure
 */
struct mmc {
	mmc_cid_t cid;		/** CID register */
	mmc_csd_t csd;		/** CSD register */
	mmc_ext_csd_t ext_csd;	/** EXT CSD register */
	uint64_t capacity;	/** MMC capacity in bytes */
	uint64_t sclock;	/** OCTEON I/O Clock */
	uint32_t clock;		/** MMC clock speed */
	uint32_t version;	/** MMC/SD version */
	union mmc_sd_scr scr;	/** SD scr register */
	uint32_t ocr;		/** OCR register */
	uint32_t tran_speed;	/** Transfer speed */
	uint16_t read_bl_len;	/** Read block length (512) */
	uint16_t write_bl_len;	/** Write block length (512) */
	uint16_t rca;		/** RCA address */
	uint16_t flags;
	uint8_t bus_width;	/** Bus width 1, 4 or 8 */
};

enum bootloader_op {
	NONE,			/** Do nothing */
	READ_STAGE1,		/** Read stage 1 bootloader into file */
	WRITE_STAGE1,		/** Write stage 1 bootloader from file */
	UPDATE_STAGE1,		/** Update stage 1 bootloader from file */
	DUMP_STAGE1,		/** Dump stage 1 bootloader to stdout */
	READ_STAGE2,		/** Read stage 2 bootloader into file */
	WRITE_STAGE2,		/** Write stage 2 bootloader from file */
	PRINT_MMC_INFO,		/** Print MMC device information */
};

#ifdef DEBUG
static void mmc_print_csd(struct mmc *mmc)
{
	mmc_csd_t *csd = &mmc->csd;
	printf("structure:..................... %u\n"
	       "spec version:.................. %u\n"
	       "taac:.......................... 0x%x 0x%x\n"
	       "nsac:.......................... %u\n"
	       "tran speed:.................... 0x%x 0x%x\n"
	       "ccc:........................... 0x%x\n"
	       "read block length:............. %u\n"
	       "read block partial:............ %u\n"
	       "write block misalign:.......... %u\n"
	       "read block misalign:........... %u\n"
	       "dsr implemented:............... %u\n"
	       "c_size device capacity (< 2GB): %u\n"
	       "VDD r current minimum:......... %u\n"
	       "VDD r current maximum:......... %u\n"
	       "VDD w current minimum:......... %u\n"
	       "VDD w current maximum:......... %u\n"
	       "C size multiplier:............. %u\n"
	       "Erase group size:.............. %u\n"
	       "Erase group multiplier:........ %u\n"
	       "Write protect group size:...... %u\n"
	       "Write protect group enable:.... %u\n"
	       "Default ecc:................... %u\n"
	       "Read to Write Factor:.......... %u\n"
	       "Write block length:............ %u\n"
	       "Write block partial:........... %u\n"
	       "Content protection app:........ %u\n"
	       "File format group:............. %u\n"
	       "Copy:.......................... %u\n"
	       "Permanent write protect:....... %u\n"
	       "Temporary Write Protect:....... %u\n"
	       "File format:................... %u\n"
	       "ECC:........................... %u\n"
	       "CRC:........................... 0x%x\n",
	       csd->s.csd_structure,
	       csd->s.spec_vers,
	       csd->s.taac >> 3, csd->s.taac & 7,
	       csd->s.nsac,
	       csd->s.tran_speed >> 3, csd->s.tran_speed & 7,
	       (uint32_t)csd->s.ccc,
	       csd->s.read_bl_len, csd->s.read_bl_partial,
	       csd->s.write_blk_misalign, csd->s.read_blk_misalign,
	       csd->s.dsr_imp,
	       (uint32_t)csd->s.c_size,
	       csd->s.vdd_r_curr_min, csd->s.vdd_r_curr_max,
	       csd->s.vdd_w_curr_min, csd->s.vdd_w_curr_max,
	       csd->s.c_size_mult, csd->s.erase_grp_size, csd->s.erase_grp_mult,
	       csd->s.wp_grp_size, csd->s.wp_grp_enable, csd->s.default_ecc,
	       csd->s.r2w_factor, csd->s.write_bl_len, csd->s.write_bl_partial,
	       csd->s.content_prot_app, csd->s.file_format_grp, csd->s.copy,
	       csd->s.perm_write_protect, csd->s.tmp_write_protect,
	       csd->s.file_format, csd->s.ecc, csd->s.crc);
}
#endif /* DEBUG */

/**
 * CRC32 function
 *
 * @param crc	starting CRC
 * @param buf	buffer to calculate CRC on
 * @param len	length of buffer
 *
 * @return crc32 result
 */
uint32_t crc32(uint32_t crc, const uint8_t *buf, uint32_t len);

static inline uint32_t le32_to_cpu(uint32_t val)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	return bswap_32(val);
#else
	return val;
#endif
}

static inline uint32_t be32_to_cpu(uint32_t val)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	return val;
#else
	return bswap_32(val);
#endif
}

static inline uint64_t be64_to_cpu(uint64_t val)
{
#if __BYTE_ORDER == __BIG_ENDIAN
	return val;
#else
	return bswap_64(val);
#endif
}

/**
 * Print data buffer in hex and ascii form to the terminal.
 *
 * data reads are buffered so that each memory address is only read once.
 * Useful when displaying the contents of volatile registers.
 *
 * @param addr		Starting address to display at start of line
 * @param data		pointer to data buffer
 * @param width		data value width.  May be 1, 2, or 4.
 * @param count		number of values to display
 * @param linelen	Number of values to print per line; specify 0 for
 *			default length
 *
 * @return		0
 */
#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)
int print_buffer(ulong addr, const void *data, uint width, uint count,
		 uint linelen)
{
	/* linebuf as a union causes proper alignment */
	union linebuf {
		uint64_t uq[MAX_LINE_LENGTH_BYTES/sizeof(uint64_t) + 1];
		uint32_t ui[MAX_LINE_LENGTH_BYTES/sizeof(uint32_t) + 1];
		uint16_t us[MAX_LINE_LENGTH_BYTES/sizeof(uint16_t) + 1];
		uint8_t  uc[MAX_LINE_LENGTH_BYTES/sizeof(uint8_t) + 1];
	} lb;
	uint i;
	uint64_t x;

	if (linelen*width > MAX_LINE_LENGTH_BYTES)
		linelen = MAX_LINE_LENGTH_BYTES / width;
	if (linelen < 1)
		linelen = DEFAULT_LINE_LENGTH_BYTES / width;

	while (count) {
		uint thislinelen = linelen;
		printf("%08lx:", addr);

		/* check for overflow condition */
		if (count < thislinelen)
			thislinelen = count;

		/* Copy from memory into linebuf and print hex values */
		for (i = 0; i < thislinelen; i++) {
			if (width == 4)
				x = lb.ui[i] = *(volatile uint32_t *)data;
			else if (width == 8)
				x = lb.uq[i] = *(volatile uint64_t *)data;
			else if (width == 2)
				x = lb.us[i] = *(volatile uint16_t *)data;
			else
				x = lb.uc[i] = *(volatile uint8_t *)data;
			printf(" %0*llx", width * 2, (unsigned long long)x);
			data += width;
		}

		while (thislinelen < linelen) {
			/* fill line with whitespace for nice ASCII print */
			for (i=0; i<width*2+1; i++)
				puts(" ");
			linelen--;
		}

		/* Print data in ASCII characters */
		for (i = 0; i < thislinelen * width; i++) {
			if (!isprint(lb.uc[i]) || lb.uc[i] >= 0x80)
				lb.uc[i] = '.';
		}
		lb.uc[i] = '\0';
		printf("    %s\n", lb.uc);

		/* update references */
		addr += thislinelen * width;
		count -= thislinelen;
	}

	return 0;
}

uint64_t get_io_clock(void)
{
	int mult;

	if (OCTEON_IS_MODEL(OCTEON_CN7XXX)) {
		cvmx_rst_boot_t rst_boot;
		rst_boot.u64 = octeon_remote_read_csr(CVMX_RST_BOOT);
		mult = rst_boot.s.c_mul;
	} else {
		cvmx_mio_rst_boot_t mio_rst_boot;
		mio_rst_boot.u64 = octeon_remote_read_csr(CVMX_MIO_RST_BOOT);
		mult = mio_rst_boot.s.c_mul;
	}
	return REF_CLOCK * mult;
}

/**
 * Calculates the CRC of the specified bootloader header
 *
 * @param header pointer to bootloader header
 *
 * @return CRC32 of the header, using zero for the CRC field.
 */
uint32_t calculate_header_crc(const bootloader_header_t * header)
{
	uint32_t crc;
	uint32_t hcrc_p;
	uint8_t *ptr;

	hcrc_p = offsetof(bootloader_header_t, hcrc);

	crc = crc32(0, (void *)header, hcrc_p);
	hcrc_p += sizeof(header->hcrc);	/* step up pass header crc field */
	ptr = (uint8_t *)header + hcrc_p;
	crc = crc32(crc, (void *)ptr, be32_to_cpu(header->hlen) - hcrc_p);

	return crc;
}

/**
 * Calculates the CRC of a bootloader image not including the header.
 *
 * @param header pointer to header in front of the image
 *
 * @return CRC32 of the payload of the U-Boot image.
 */
uint32_t calculate_image_crc(const bootloader_header_t * header)
{
	uint8_t *ptr = (uint8_t *)header + be32_to_cpu(header->hlen);
	return crc32(0, (void *)ptr, be32_to_cpu(header->dlen));
}

/**
 * Validate image header. Intended for actual header discovery.
 * Not NAND or NOR specific
 *
 * @param header     Address of expected image header.
 *
 * @return 0  on success
 *         1  on failure
 */
int validate_bootloader_header(const struct bootloader_header *header)
{
	if (header->magic == BOOTLOADER_HEADER_MAGIC) {
		/* Check the CRC of the header */
		if (calculate_header_crc(header) == header->hcrc)
			return 1;
		else
			return 0;
	}
	return 0;
}

/**
 * Sends a command to the MMC device
 *
 * @param cmd_idx - command index
 * @param arg - argument to pass to command
 * @param flags - various flags to pass, needed for some SD commands to override
 *		  the default OCTEON values for MMC.
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_cmd(int cmd_idx, uint32_t arg, uint32_t flags)
{
	cvmx_mio_emm_cmd_t	emm_cmd;
	cvmx_mio_emm_rsp_sts_t	emm_rsp_sts;

	debug("%s(CMD%u, 0x%x, 0x%x)\n", __func__, cmd_idx, arg, flags);
	emm_cmd.u64 = 0;
	emm_cmd.s.cmd_val = 1;
	emm_cmd.s.cmd_idx = cmd_idx;
	emm_cmd.s.arg = arg;
	emm_cmd.s.ctype_xor = (flags >> 3) & 3;
	emm_cmd.s.rtype_xor = (flags & 7);
	emm_cmd.s.offset = (flags >> 8) & 0x3f;
	octeon_remote_write_csr(CVMX_MIO_EMM_CMD, emm_cmd.u64);

	do {
		emm_rsp_sts.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_STS);
	} while (!emm_rsp_sts.s.cmd_done && !emm_rsp_sts.s.rsp_timeout);

	debug("Response: 0x%llx, command index: %u\n",
	      (unsigned long long)emm_rsp_sts.u64, emm_rsp_sts.s.cmd_idx);

	if (emm_rsp_sts.s.rsp_timeout) {
		debug("%s: cmd: 0x%x, arg: 0x%x timed out\n",
		       __func__, cmd_idx, arg);
		return 2;
	}
	else if (emm_rsp_sts.s.rsp_bad_sts || emm_rsp_sts.s.rsp_crc_err)
	{
		printf("%s: cmd: 0x%x, arg: 0x%x bad status or crc\n",
		       __func__, cmd_idx, arg);
		return 1;
	}
	return 0;
}

/**
 * Sends IDLE commands to force the MMC or SD device to go idle
 *
 * @return 0 for success, error otherwise
 */
static int mmc_go_idle(void)
{
	mmc_send_cmd(0, 0, 0);
	return mmc_send_cmd(0, 0, 0);
}

/**
 * Reads the MMC OCR register
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_op_cond(struct mmc *mmc)
{
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo;
	int rc;
	int timeout = 10;

	mmc_go_idle();
	usleep(10000);

	do {
		rc = mmc_send_cmd(MMC_CMD_SEND_OP_COND, 0x40ff8000, 0);
		emm_rsp_lo.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
		usleep(100000);
	} while ((rc == 0) && !(emm_rsp_lo.u64 & (1ull << (31 + 8))) &&
		 (timeout-- > 0));
	if (rc || !timeout) {
		printf("MMC SEND OP COND failed!\n");
		return 1;
	}
	mmc->ocr = (emm_rsp_lo.u64 >> 8) & 0xffffffff;
	if (mmc->ocr & 0x40000000)
		mmc->flags |= MMC_FLAG_HIGH_CAPACITY;
	return 0;
}

/**
 * Get the extended CSD register from the MMC/SD card
 *
 * @param mmc - MMC data structure
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_ext_csd(struct mmc *mmc)
{
	cvmx_mio_emm_buf_idx_t emm_buf_idx;
	cvmx_mio_emm_buf_dat_t emm_buf_dat;
	int rc;
	int i;

	rc = mmc_send_cmd(MMC_CMD_SEND_EXT_CSD, 0, 0);
	if (rc) {
		printf("EXT CSD failed!\n");
		return rc;
	}
	emm_buf_idx.u64 = 0;
	emm_buf_idx.s.inc = 1;
	octeon_remote_write_csr(CVMX_MIO_EMM_BUF_IDX, emm_buf_idx.u64);
	for (i = 0; i < (int)sizeof(mmc->ext_csd.buffer)/8; i++) {
		emm_buf_dat.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_BUF_DAT);
		mmc->ext_csd.buffer[i] = be64_to_cpu(emm_buf_dat.u64);
	}
	return 0;
}

/**
 * Calculate the clock period based on the clock speed
 *
 * @param[in]	mmc	pointer to mmc data structure
 *
 * @return clock period in cycles for the current clock speed.
 */
static int mmc_calc_clock_period(const struct mmc *mmc)
{
	return (((mmc->sclock + mmc->clock - 1) / mmc->clock) + 1) / 2;
}

/**
 * Set the block length, should be 512
 *
 * @param	mmc	mmc data structure
 *
 * @return	0 for success, error otherwise
 */
int mmc_set_blocklen(struct mmc *mmc)
{
	debug("%s: read block length: %u\n", __func__,
	      1 << mmc->csd.s.read_bl_len);
	debug("%s: CSD: 0x%llx 0x%llx\n", __func__,
	      (unsigned long long)mmc->csd.u128[0],
	      (unsigned long long)mmc->csd.u128[1]);
	return mmc_send_cmd(MMC_CMD_SET_BLOCKLEN,
			    1 << mmc->csd.s.read_bl_len, 0);
}

/**
 * Configures the MMC bus
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success, otherwise error
 */
static int mmc_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_rsp_sts_t emm_sts;
	int clk_period;
	int timeout = 2550;

	emm_switch.u64 = 0;
	emm_switch.s.power_class = 10;

	again:
	switch (mmc->bus_width) {
		case 8:
			if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR)
				emm_switch.s.bus_width = EXT_CSD_DDR_BUS_WIDTH_8;
			else
				emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_8;
			break;
		case 4:
			if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR)
				emm_switch.s.bus_width = EXT_CSD_DDR_BUS_WIDTH_4;
			else
				emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_4;
			break;
		default:
			emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_1;
			break;
	}
	clk_period = mmc_calc_clock_period(mmc);
	emm_switch.s.switch_exe = 1;
	emm_switch.s.clk_hi = clk_period;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	emm_switch.s.hs_timing =
		(mmc->flags & (MMC_FLAG_HIGH_SPEED | MMC_FLAG_HIGH_SPEED_52MHz))
		? 1 : 0;
	octeon_remote_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	do {
		emm_sts.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_STS);
		if (!emm_sts.s.switch_val)
			break;
		usleep(10000);
	} while (timeout-- > 0);
	if (emm_switch.s.switch_err0) {
		if (emm_switch.s.power_class > 0) {
			emm_switch.s.power_class--;
			goto again;
		} else {
			printf("Power class failed\n");
			return 1;
		}
	}
	if (emm_switch.s.switch_err1) {
		if (mmc->flags & MMC_FLAG_HIGH_SPEED_DDR) {
			mmc->flags &= ~MMC_FLAG_HIGH_SPEED_DDR;
			goto again;
		}
		if ((mmc->flags & MMC_FLAG_HIGH_SPEED_52MHz) &&
			(mmc->clock > 26000000)) {
			mmc->flags &= ~MMC_FLAG_HIGH_SPEED_52MHz;
		mmc->flags |= MMC_FLAG_HIGH_SPEED;
		mmc->clock = 26000000;
		goto again;
			}
			if (mmc->flags & MMC_FLAG_HIGH_SPEED) {
				if (MMC_MAX_SPEED < 20000000)
					mmc->clock =  MMC_MAX_SPEED;
				else
					mmc->clock = 20000000;
				mmc->flags &= ~MMC_FLAG_HIGH_SPEED;
				goto again;
			}
			printf("MMC timing failed\n");
			return 2;
	}
	if (emm_switch.s.switch_err2) {
		switch (emm_switch.s.bus_width) {
			case EXT_CSD_BUS_WIDTH_8:
				emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_4;
				break;
			case EXT_CSD_BUS_WIDTH_4:
				emm_switch.s.bus_width = EXT_CSD_BUS_WIDTH_1;
				break;
			default:
				printf("Bus width failed");
				return 3;
		}
		goto again;
	}
	return 0;
}

/**
 * Initialize SD bus settings
 *
 * @param mmc - pointer to MMC data structure
 *
 * @return 0 for success
 */
int sd_set_ios(struct mmc *mmc)
{
	cvmx_mio_emm_switch_t emm_switch;
	int clk_period;

	clk_period = mmc_calc_clock_period(mmc);
	emm_switch.u64 = 0;
	/* No DDR for now */
	emm_switch.s.bus_width = (mmc->bus_width == 4) ?
	EXT_CSD_BUS_WIDTH_4 : EXT_CSD_BUS_WIDTH_1;
	emm_switch.s.clk_hi = clk_period;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	emm_switch.s.power_class = 10;
	octeon_remote_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	usleep(20000);
	return 0;
}

/**
 * Reads one or more sectors from a MMC or SD device
 *
 * @param mmc - pointer to MMC data structure
 * @param src - starting sector number
 * @param dst - pointer to destination buffer
 * @param count - number of sectors to read
 *
 * @return 0 for success, error otherwise
 */
static int mmc_read(struct mmc *mmc, uint32_t src, void *dst, int count)
{
	cvmx_mio_emm_buf_idx_t buf_idx;
	uint64_t *dst_ptr = (uint64_t *)dst;
	int i;
	int rc;

	if (!(mmc->flags & MMC_FLAG_HIGH_CAPACITY))
		src *= FAT_SECTOR_SIZE;

	while (count-- > 0) {
		debug("%s: reading address 0x%x\n", __func__, src);
		rc = mmc_send_cmd(MMC_CMD_READ_SINGLE_BLOCK, src, 0);
		if (rc) {
			printf("Read failed for block 0x%x\n", src);
			return rc;
		}
		buf_idx.u64 = 0;
		buf_idx.s.inc = 1;
		octeon_remote_write_csr(CVMX_MIO_EMM_BUF_IDX, buf_idx.u64);
		for (i = 0; i < FAT_SECTOR_SIZE / 8; i++)
			*dst_ptr++ = be64_to_cpu(octeon_remote_read_csr(CVMX_MIO_EMM_BUF_DAT));

		if (mmc->flags & MMC_FLAG_HIGH_CAPACITY)
			src++;
		else
			src += FAT_SECTOR_SIZE;
	}
	return 0;
}

/**
 * Writes one or more sectors to a MMC or SD device
 *
 * @param mmc - pointer to MMC data structure
 * @param dest - destination sector number
 * @param buffer - pointer to source buffer
 * @param count - number of sectors to write
 *
 * @return 0 for success, error otherwise
 */
static int mmc_write(struct mmc *mmc, uint32_t dest, const void *buffer,
		     int count)
{
	cvmx_mio_emm_buf_idx_t buf_idx;
	uint64_t *src_ptr = (uint64_t *)buffer;
	int i;
	int rc;
	int timeout = 100;
	cvmx_mio_emm_rsp_lo_t rsp_lo;

	if (!(mmc->flags & MMC_FLAG_HIGH_CAPACITY))
		dest *= FAT_SECTOR_SIZE;

	do {
		rc = mmc_send_cmd(MMC_CMD_SEND_STATUS, mmc->rca << 16, 0);
		if (rc) {
			printf("Error getting status\n");
			return rc;
		}
		rsp_lo.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
		if ((rsp_lo.s.dat >> 8) & R1_READY_FOR_DATA)
			break;

		timeout--;
		usleep(100000);
	} while (timeout--);

	if (!timeout) {
		puts("Write timed out waiting for status");
		return -1;
	}

	debug("MMC status: 0x%x, command index: %u\n",
	      (uint32_t)((rsp_lo.u64 >> 8) & 0xffffffff),
	      (uint32_t)((rsp_lo.u64 >> 40) & 0x3f));
	debug("Current state: %s\n", mmc_state_str[(rsp_lo.u64 >> 17) & 0xF]);
	while (count-- > 0) {
		debug("%s: writing address 0x%x\n", __func__, dest);
		buf_idx.u64 = 0;
		buf_idx.s.inc = 1;
		octeon_remote_write_csr(CVMX_MIO_EMM_BUF_IDX, buf_idx.u64);
		for (i = 0; i < FAT_SECTOR_SIZE / 8; i++)
			octeon_remote_write_csr(CVMX_MIO_EMM_BUF_DAT,
						be64_to_cpu(*src_ptr++));
		rc = mmc_send_cmd(MMC_CMD_WRITE_SINGLE_BLOCK, dest, 0);
		if (rc) {
			printf("Write failed for block 0x%x\n", dest);
			return rc;
		}
		if (mmc->flags & MMC_FLAG_HIGH_CAPACITY)
			dest++;
		else
			dest += FAT_SECTOR_SIZE;
		printf("%u blocks remaining        \r", count);
		fflush(stdout);
	}
	puts("");
	return 0;
}

/**
 * Sends an ACMD to a SD device.
 *
 * @param cmd_idx - command index to send
 * @param arg - command argument
 * @param flags - flags to use (see mmc_send_cmd)
 * @param rca - RCA address of SD device
 *
 * @return 0 for success, error otherwise
 */
static int mmc_send_acmd(int cmd_idx, uint32_t arg, uint32_t flags, uint16_t rca)
{
	int rc;

	rc = mmc_send_cmd(MMC_CMD_APP_CMD, rca << 16, 0);
	if (rc)
		return rc;
	return mmc_send_cmd(cmd_idx, arg, flags);
}

static int mmc_get_scr(struct mmc *mmc)
{
	int rc;
	cvmx_mio_emm_buf_idx_t emm_buf_idx;
	/* response type: MMC_RSP_R1
	 * arg: 0
	 * ctype xor: 1
	 * rtype xor: 1
	 * offset: 63
	 * blocksize 8
	 * blocks 1
	 */
	rc = mmc_send_acmd(SD_CMD_APP_SEND_SCR, 0, (63 << 8) | (1 << 3) | 1, 0);
	if (rc) {
		printf("Error getting SCR\n");
		return rc;
	}
	emm_buf_idx.u64 = 0;
	emm_buf_idx.s.inc = 1;
	octeon_remote_write_csr(CVMX_MIO_EMM_BUF_IDX, emm_buf_idx.u64);
	mmc->scr.u64 = be64_to_cpu(octeon_remote_read_csr(CVMX_MIO_EMM_BUF_DAT));
	debug("%s: scr: 0x%llx\n", __func__, (unsigned long long)mmc->scr.u64);

	return 0;
}

static int mmc_init(struct mmc *mmc)
{
	cvmx_mio_emm_cfg_t emm_cfg;
	cvmx_mio_emm_switch_t emm_switch;
	cvmx_mio_emm_wdog_t emm_wdog;
	cvmx_mio_emm_rsp_lo_t emm_rsp_lo;
	int timeout;
	int rc;
	int freq;
	int mult;
	uint64_t val;
	/* frequency bases */
	/* divided by 10 to be nice to platforms without floating point */
	static const int fbase[] = {
		10000,
		100000,
		1000000,
		10000000,
	};

	/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
	 * to platforms without floating point.
	 */
	static const uint8_t multipliers[] = {
		0,	/* reserved */
		10, 12, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, };

	memset(mmc, 0, sizeof(*mmc));
	mmc->sclock = get_io_clock();
	mmc->clock = MMC_MIN_SPEED;
	mmc->version = MMC_VERSION_UNKNOWN;

	/* Reset MMC bus */
	emm_cfg.u64 = 0;
	octeon_remote_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);
	usleep(10000);
	/* Enable MMC */
	emm_cfg.s.bus_ena = 1;
	octeon_remote_write_csr(CVMX_MIO_EMM_CFG, emm_cfg.u64);

	emm_wdog.u64 = 0;
	emm_wdog.s.clk_cnt = 256000 + (8 * MMC_MIN_SPEED) / 10;
	octeon_remote_write_csr(CVMX_MIO_EMM_WDOG, emm_wdog.u64);

	/* Configure power class 10, speed 400KHz */
	emm_switch.u64 = 0;
	emm_switch.s.power_class = 10;
	emm_switch.s.clk_hi = ((mmc->sclock / mmc->clock) + 1) / 2;
	emm_switch.s.clk_lo = emm_switch.s.clk_hi;
	octeon_remote_write_csr(CVMX_MIO_EMM_SWITCH, emm_switch.u64);
	usleep(2000);	/* Wait 1ms + 74 clock cycles */

	/* Force MMC device to go idle */
	mmc_go_idle();

	/* Check if SD version 2.0 or later */
	rc = mmc_send_cmd(SD_CMD_SEND_IF_COND, 0x1aa, (1 << 3) | 2);
	emm_rsp_lo.u64 = 0;
	if (rc == 0) {
		emm_rsp_lo.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
		if ((emm_rsp_lo.u64 & 0xff00) == 0xaa00)
			mmc->version = SD_VERSION_2;
		else
			mmc->version = SD_VERSION_1_0;
		debug("Found SD card\n");
	}
	if (IS_SD(mmc)) {
		debug("SD sending op cond\n");
		timeout = 1000;
		do {
			uint32_t args = 0xff8000;
			if (mmc->version == SD_VERSION_2)
				args |= 0x40000000;
			rc = mmc_send_acmd(SD_CMD_APP_SEND_OP_COND, args, 3, 0);
			if (rc)
				break;
			emm_rsp_lo.u64 = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
			usleep(1000);
		} while (!(emm_rsp_lo.u64 & (1ull << (31 + 8))) && timeout--);
		if (rc == 0 && timeout > 0) {
			if (mmc->version != SD_VERSION_2)
				mmc->version = SD_VERSION_1_0;
			mmc->ocr = emm_rsp_lo.u64 >> 8;
			if (mmc->ocr & 0x40000000)
				mmc->flags |= MMC_FLAG_HIGH_CAPACITY;
		} else {
			mmc_send_op_cond(mmc);
		}
	} else {
		debug("MMC: going idle again\n");
		mmc_go_idle();
		debug("MMC sending op cond\n");
		mmc_send_op_cond(mmc);
	}
	debug("MMC sending CID\n");
	rc = mmc_send_cmd(MMC_CMD_ALL_SEND_CID, 0, 0);
	val = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_HI);
	mmc->cid.u128[0] = 0;
	mmc->cid.u128[1] = 0;
	if (IS_SD(mmc)) {
		mmc->cid.sd.mid = (val >> 56) & 0xff;
		mmc->cid.sd.oem = (val >> 40) & 0xffff;
		mmc->cid.sd.prod_name[4] = (val >> 32) & 0xff;
		mmc->cid.sd.prod_name[3] = (val >> 24) & 0xff;
		mmc->cid.sd.prod_name[2] = (val >> 16) & 0xff;
		mmc->cid.sd.prod_name[1] = (val >> 8) & 0xff;
		mmc->cid.sd.prod_name[0] = val & 0xff;
		val = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
		mmc->cid.sd.revision = (val >> 56) & 0xff;
		mmc->cid.sd.serial_no = (val >> 24) & 0xffffffff;
		mmc->cid.sd.manu_date_y = (val >> 12) & 0xff;
		mmc->cid.sd.manu_date_m = (val >> 8) & 0xf;
		mmc->cid.sd.crc = (val >> 1) & 0x7f;
		mmc->cid.sd.one = val & 1;
	} else {
		mmc->cid.mmc.mid = (val >> 56) & 0xff;
		mmc->cid.mmc.device = (val >> 48) & 3;
		mmc->cid.mmc.oem = (val >> 40) & 0xff;
		mmc->cid.mmc.prod_name[5] = (val >> 32) & 0xff;
		mmc->cid.mmc.prod_name[4] = (val >> 24) & 0xff;
		mmc->cid.mmc.prod_name[3] = (val >> 16) & 0xff;
		mmc->cid.mmc.prod_name[2] = (val >> 8) & 0xff;
		mmc->cid.mmc.prod_name[1] = (val & 0xff);
		val = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
		mmc->cid.mmc.prod_name[0] = (val >> 56) & 0xff;
		mmc->cid.mmc.revision = (val >> 48) & 0xff;
		mmc->cid.mmc.serial_no = (val >> 16) & 0xffffffff;
		mmc->cid.mmc.manu_date = (val >> 8) & 0xff;
		mmc->cid.mmc.crc = (val >> 1) & 0x7f;
		mmc->cid.mmc.one = val & 1;
	}
	debug("CID: 0x%llx 0x%llx\n",
	      (unsigned long long)mmc->cid.u128[0],
	      (unsigned long long)mmc->cid.u128[1]);
	if (IS_SD(mmc)) {
		rc = mmc_send_cmd(SD_CMD_SEND_RELATIVE_ADDR, 0, 2);
		if (!rc) {
			mmc->rca = (octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO) >> 24) & 0xffff;
		}
	} else {
		mmc->rca = 16;	/* Must be greater than 1 */
		rc = mmc_send_cmd(MMC_CMD_SET_RELATIVE_ADDR, mmc->rca << 16, 0);
	}
	debug("Sending CSD\n");
	rc = mmc_send_cmd(MMC_CMD_SEND_CSD, mmc->rca << 16, 0);

	if (rc)
		goto error2;

	val = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_HI);
	debug("CSD: 0x%llx ", (unsigned long long)val);
	mmc->csd.u128[0] = 0;
	mmc->csd.u128[1] = 0;
	mmc->csd.s.csd_structure = (val >> 62) & 3;
	mmc->csd.s.spec_vers = (val >> 58) & 0xf;
	mmc->csd.s.taac = (val >> 48) & 0xff;
	mmc->csd.s.nsac = (val >> 40) & 0xff;
	mmc->csd.s.tran_speed = (val >> 32) & 0xff;
	mmc->csd.s.ccc = (val >> 20) & 0x3ff;
	mmc->csd.s.read_bl_len = (val >> 16) & 0xf;
	mmc->csd.s.read_bl_partial = (val >> 15) & 1;
	mmc->csd.s.write_blk_misalign = (val >> 14) & 1;
	mmc->csd.s.read_blk_misalign = (val >> 13) & 1;
	mmc->csd.s.dsr_imp = (val >> 12) & 1;
	mmc->csd.s.c_size = (val & 0x3ff) << 2;
	val = octeon_remote_read_csr(CVMX_MIO_EMM_RSP_LO);
	debug("0x%llx\n", (unsigned long long)val);
	mmc->csd.s.c_size |= (val >> 62) & 3;
	mmc->csd.s.vdd_r_curr_min = (val >> 59) & 7;
	mmc->csd.s.vdd_r_curr_max = (val >> 56) & 7;
	mmc->csd.s.vdd_w_curr_min = (val >> 53) & 7;
	mmc->csd.s.vdd_w_curr_max = (val >> 50) & 7;
	mmc->csd.s.c_size_mult = (val >> 47) & 7;
	mmc->csd.s.erase_grp_size = (val >> 42) & 0x1f;
	mmc->csd.s.erase_grp_mult = (val >> 37) & 0x1f;
	mmc->csd.s.wp_grp_size = (val >> 32) & 0x1f;
	mmc->csd.s.wp_grp_enable = (val >> 31) & 1;
	mmc->csd.s.default_ecc = (val >> 29) & 3;
	mmc->csd.s.r2w_factor = (val >> 26) & 7;
	mmc->csd.s.write_bl_len = (val >> 22) & 0xf;
	mmc->csd.s.write_bl_partial = (val >> 21) & 1;
	mmc->csd.s.content_prot_app = (val >> 16) & 1;
	mmc->csd.s.file_format_grp = (val >> 15) & 1;
	mmc->csd.s.copy = (val >> 14) & 1;
	mmc->csd.s.perm_write_protect = (val >> 13) & 1;
	mmc->csd.s.tmp_write_protect = (val >> 12) & 1;
	mmc->csd.s.file_format = (val >> 10) & 3;
	mmc->csd.s.ecc = (val >> 8) & 3;
	mmc->csd.s.crc = (val >> 1) & 0x7f;
#ifdef DEBUG
	mmc_print_csd(mmc);
#endif
	if (mmc->version == MMC_VERSION_UNKNOWN) {
		switch (mmc->csd.s.spec_vers) {
		default:
		case 0:
			mmc->version = MMC_VERSION_1_2;
			break;
		case 1:
			mmc->version = MMC_VERSION_1_4;
			break;
		case 2:
			mmc->version = MMC_VERSION_2_2;
			break;
		case 3:
			mmc->version = MMC_VERSION_3;
			break;
		case 4:
			mmc->version = MMC_VERSION_4;
			break;
		}
	}
	debug("MMC version: 0x%x\n", mmc->version);
	freq = fbase[mmc->csd.s.tran_speed & 7];
	mult = multipliers[(mmc->csd.s.tran_speed >> 3) & 0xf];
	mmc->tran_speed = freq * mult;
	if (mmc->tran_speed > MMC_MAX_SPEED)
		mmc->tran_speed = MMC_MAX_SPEED;
	mmc->read_bl_len = 1 << mmc->csd.s.read_bl_len;
	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << mmc->csd.s.write_bl_len;

	if (mmc->flags & MMC_FLAG_HIGH_CAPACITY)
		mmc->capacity = ((uint64_t)mmc->csd.s.c_size + 1) << 10;
	else
		mmc->capacity = ((uint64_t)mmc->csd.s.c_size + 1)
					<< (mmc->csd.s.c_size_mult + 2);
	if (mmc->read_bl_len > FAT_SECTOR_SIZE)
		mmc->read_bl_len = FAT_SECTOR_SIZE;
	if (mmc->write_bl_len > FAT_SECTOR_SIZE)
		mmc->write_bl_len = FAT_SECTOR_SIZE;

	debug("Selecting card 0x%x\n", mmc->rca);
	/* Select the MMC device */
	rc = mmc_send_cmd(MMC_CMD_SELECT_CARD, mmc->rca << 16, 0);
	if (rc)
		goto error2;
	debug("Setting RCA to 0x%x\n", mmc->rca);
	octeon_remote_write_csr(CVMX_MIO_EMM_RCA, mmc->rca);

	if (!IS_SD(mmc)) {
		if (mmc->bus_width == 0)
			mmc->bus_width = MMC_MAX_WIDTH;
		mmc->clock = 20000000;

		if (mmc->version >= MMC_VERSION_4) {
			rc = mmc_send_ext_csd(mmc);
			if (rc)
				goto error2;
			if ((mmc->ext_csd.s.ext_csd_rev >= 2) &&
			    (mmc->flags & MMC_FLAG_HIGH_CAPACITY)) {
				mmc->capacity = (uint64_t)le32_to_cpu(mmc->ext_csd.s.sec_count) * (uint64_t)FAT_SECTOR_SIZE;
				debug("Sector count: 0x%x\n",
				      le32_to_cpu(mmc->ext_csd.s.sec_count));
			}
			if ((MMC_MAX_SPEED > 20000000) &&
			    (mmc->ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_26MHz)) {
				mmc->flags |= MMC_FLAG_HIGH_SPEED;
				mmc->clock = 26000000;
			}
			if ((MMC_MAX_SPEED > 26000000) &&
			    (mmc->ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_52MHz)) {
				mmc->flags |= MMC_FLAG_HIGH_SPEED_52MHz;
				mmc->clock = 52000000;
			}
			if ((MMC_MAX_SPEED > 26000000) &&
			    (mmc->ext_csd.s.device_type & EXT_CSD_DEVICE_TYPE_DDR_52MHz_3V)) {
				mmc->flags |= MMC_FLAG_HIGH_SPEED_DDR;
				mmc->clock = 52000000;
			}
		}
		if (mmc->clock > MMC_MAX_SPEED)
			mmc->clock = MMC_MAX_SPEED;
		debug("MMC setting IOS\n");
		rc = mmc_set_ios(mmc);
		if (rc)
			printf("mmc_set_ios failed\n");
		usleep(10000);
	} else {
		rc = mmc_get_scr(mmc);
		if (rc) {
			printf("Error getting SD SCR\n");
			mmc->scr.u64 = 0;
		}

		if (MMC_MAX_WIDTH < 4)
			mmc->bus_width = MMC_MAX_WIDTH;
		else
			mmc->bus_width = 4;
		if (MMC_MAX_SPEED < 25000000)
			mmc->clock = MMC_MAX_SPEED;
		else
			mmc->clock = 25000000;
		debug("Setting bus width to %d\n", mmc->bus_width);
		rc = mmc_send_acmd(SD_CMD_APP_SET_BUS_WIDTH,
				   mmc->bus_width == 4 ? 2 : 0, 0, mmc->rca);
	}

	rc = mmc_set_blocklen(mmc);
	if (rc) {
		puts("Error setting block length");
		return rc;
	}

	/* Set watchdog timeout again */
	emm_wdog.s.clk_cnt = 256000 + (10 * mmc->clock) / 8;
	debug("Setting watchdog to %u\n", emm_wdog.s.clk_cnt);
	octeon_remote_write_csr(CVMX_MIO_EMM_WDOG, emm_wdog.u64);

	return 0;

error2:
	return -1;
}

void print_mmc_device_info(struct mmc *mmc)
{
	const char *type;
	const char *version;
	uint32_t card_type;
	static const char *cbx_str[4] = {
		"Card (removable)",
		"BGA (Discrete embedded)",
		"POP",
		"Reserved"
	};
	char product[8];

	debug("CID: 0x%08llx 0x%08llx\n",
	      (unsigned long long)mmc->cid.u128[0],
	      (unsigned long long)mmc->cid.u128[1]);
	card_type = mmc->ext_csd.s.device_type;
	if (IS_SD(mmc)) {
		if (mmc->flags & MMC_FLAG_HIGH_CAPACITY)
			type = "SDHC or SDXC";
		else
			type = "SD";
	} else {
		type = "MMC";
	}

	switch (mmc->version) {
	case SD_VERSION_2:
	case SD_VERSION_3:
		if (mmc->scr.s.sd_spec3) {
			if (mmc->scr.s.sd_spec4)
				version = "SD 4.XX";
			else
				version = "SD 3.0X";
		} else {
			version = "SD 2.00";
		}
		break;
	case SD_VERSION_1_10:	version = "SD 1.10";	break;
	case SD_VERSION_1_0:	version = "SD 1.0";	break;
	case MMC_VERSION_4:
		switch (mmc->ext_csd.s.ext_csd_rev) {
		case 0:		version = "MMC v4.0";	break;
		case 1:		version = "MMC v4.1";	break;
		case 2:		version = "MMC v4.3";	break;
		case 4:		version = "MMC v4.4 (obsolete)";	break;
		case 5:		version = "MMC v4.41";	break;
		case 6:		version = "MMC v4.5";	break;
		default:	version = "MMC > v4.5";	break;
		}
		break;
	case MMC_VERSION_3:	version = "MMC v3";	break;
	case MMC_VERSION_2_2:	version = "MMC v2.2";	break;
	case MMC_VERSION_1_4:	version = "MMC v1.4";	break;
	case MMC_VERSION_1_2:	version = "MMC v1.2";	break;
	case MMC_VERSION_UNKNOWN:	version = "MMC Unknown";	break;
	default:		version = "Unknown";	break;
	}
	printf("Type:                  %s\n", type);
	printf("Version:               %s\n", version);
	if (IS_SD(mmc))
		printf("Manufacturer ID:       0x%02x\n", mmc->cid.sd.mid);
	else
		printf("Manufacturer ID:       0x%02x\n", mmc->cid.mmc.mid);

	if (IS_SD(mmc)) {
		printf("OEM ID:                %c%c\n",
		       (mmc->cid.sd.oem >> 8) & 0xff, mmc->cid.sd.oem & 0xff);
	} else {
		printf("Device Type:           %s\n",
		       cbx_str[mmc->cid.mmc.device]);
		printf("OEM ID:                0x%02x\n", mmc->cid.mmc.oem);
	}
	if (IS_SD(mmc)) {
		memcpy(product, mmc->cid.sd.prod_name, 5);
		product[5] = '\0';
	} else {
		memcpy(product, mmc->cid.mmc.prod_name, 6);
		product[6] = '\0';
	}
	printf("Product:               %s\n", product);
	if (IS_SD(mmc))
		printf("Revision:              %d.%d\n",
		       mmc->cid.sd.revision >> 4, mmc->cid.sd.revision & 0xF);
	else
		printf("Revision:              %d.%d\n",
		       mmc->cid.mmc.revision >> 4, mmc->cid.mmc.revision & 0xF);
	if (IS_SD(mmc))
		printf("Serial Number:         %u\n", mmc->cid.sd.serial_no);
	else
		printf("Serial Number:         %u\n", mmc->cid.mmc.serial_no);
	if (IS_SD(mmc)) {
		printf("Manufacturing Date:    %d/%d\n",
		       mmc->cid.sd.manu_date_m,
		       mmc->cid.sd.manu_date_y + 2000);
	} else {
		int start_year;
		if ((mmc->ext_csd.s.ext_csd_rev > 4) &&
		    ((mmc->cid.mmc.manu_date) & 0xf) <= 12)
			start_year = 2013;
		else
			start_year = 1997;
		printf("Manufacturing Date:    %d/%d\n",
		       (mmc->cid.mmc.manu_date >> 4) & 0xf,
		       (mmc->cid.mmc.manu_date & 0xf) + start_year);
	}
	printf("Capacity:              %llu bytes (%llu sectors)\n",
	       (unsigned long long)mmc->capacity,
	       (unsigned long long)(mmc->capacity / mmc->read_bl_len));
	printf("Read block length:     %u\n", mmc->read_bl_len);
	printf("Write block length:    %u\n", mmc->write_bl_len);
	printf("High capacity:         %s\n",
	       mmc->flags & MMC_FLAG_HIGH_CAPACITY ? "yes" : "no");
	printf("Bus width:             %u bits\n", mmc->bus_width);
	printf("Bus frequency:         %u\n", mmc->clock);
	printf("Relative Card Address: 0x%x\n", mmc->rca);
	if (IS_SD(mmc)) {
		const char *sd_security;
		switch (mmc->scr.s.sd_security) {
		case 0:
			sd_security = "None";
			break;
		case 1:
			sd_security = "Not Used";
			break;
		case 2:
			sd_security = "SDSC Card (Security Version 1.01)";
			break;
		case 3:
			sd_security = "SDHC Card (Security Version 2.00)";
			break;
		case 4:
			sd_security = "SDXC Card (Security Version 3.xx)";
			break;
		default:
			sd_security = "Reserved/Unknown";
			break;
		}
		printf("SD Security:           %s\n", sd_security);
		if (mmc->scr.s.ex_security)
			printf("SD Extended Security supported\n");

		if (mmc->scr.s.cmd_support & 2)
			printf("SD Set Block Count (CMD23) supported\n");
		if (mmc->scr.s.cmd_support & 1)
			printf("SDXC Speed Class Control supported\n");
	}
	if (card_type != 0) {
		int prev = 0;
		fputs("Supported bus speeds:    ", stdout);
		if (card_type & EXT_CSD_DEVICE_TYPE_26MHz) {
			fputs(" 26MHz", stdout);
			prev = 1;
		}
		if (card_type & EXT_CSD_DEVICE_TYPE_52MHz) {
			if (prev)
				putchar(',');
			fputs(" 52MHz", stdout);
			prev = 1;
		}
		if (card_type & EXT_CSD_DEVICE_TYPE_DDR_52MHz_3V) {
			if (prev)
				putchar(',');
			fputs(" DDR 52MHz 3V", stdout);
			prev = 1;
		}
		if (card_type & EXT_CSD_DEVICE_TYPE_DDR_52MHz_1_2V) {
			if (prev)
				putchar(',');
			fputs(" DDR 52MHz 1.2V", stdout);
			prev = 1;
		}
		if (card_type & EXT_CSD_DEVICE_TYPE_200MHz_1_8V) {
			if (prev)
				putchar(',');
			fputs(" 200MHz 1.8V", stdout);
			prev = 1;
		}
		if (card_type & EXT_CSD_DEVICE_TYPE_200MHz_1_2V) {
			if (prev)
				putchar(',');
			fputs(" 200MHz 1.2V", stdout);
			prev = 1;
		}
	}
	puts("");
	if (!IS_SD(mmc)) {
		int i;
		uint8_t *buf = (uint8_t *)mmc->ext_csd.buffer;
		printf("\nExtended CSD Register:");
		for (i = 0; i < 512; i++) {
			if (i % 16 == 0)
				printf("\n0x%03x: ", i);
			if (i % 16 == 8)
				fputs("- ", stdout);
			printf("%02x ", (uint32_t)buf[i]);
		}
		puts("\n");
	}
}

/**
 * Get the size of the stage 2 image on the MMC device
 *
 * @param[in]	mmc	mmc data structure
 * @param[out]	size	size of stage 2 image following the stage 1 image
 *
 * @return	0 for success, -1 on error
 *
 * size will be output as zero if the header is invalid
 */
int get_stage2_size(struct mmc *mmc, size_t *size)
{
	uint8_t sector[FAT_SECTOR_SIZE];
	struct bootloader_header *header = (struct bootloader_header *)sector;
	int rc;

	rc = mmc_read(mmc, STAGE2_START, sector, 1);
	if (rc) {
		printf("Cannot read stage 2 first sector\n");
		return rc;
	}
	if (!size)
		return -1;

	if (validate_bootloader_header(header)) {
		*size = be32_to_cpu(header->hlen) + be32_to_cpu(header->dlen);
		return 0;
	} else {
		*size = 0;
		return -1;
	}
}

/**
 * Reads a stage 2 bootloader image from the MMC device
 *
 * @param[in]	mmc		mmc data structure
 * @param[out]	buffer		buffer to hold the stage 2 image
 * @param[in]	buffer_size	size of buffer
 * @param[out]	size		size of stage 2 image
 *
 * @return	0 for success, error otherwise
 */
int read_stage2(struct mmc *mmc, uint8_t *buffer, size_t buffer_size,
		size_t *size)
{
	size_t image_size;
	int rc;

	if (size)
		*size = 0;

	rc = get_stage2_size(mmc, &image_size);
	if (rc) {
		printf("Error reading stage 2 image size\n");
		return -1;
	}
	if (image_size > buffer_size) {
		printf("Error: image size of %zu bytes is too large for buffer\n",
		       image_size);
		if (size)
			*size = image_size;
		return -1;
	}
	rc = mmc_read(mmc, STAGE2_START, buffer,
		      (image_size + FAT_SECTOR_SIZE - 1) / FAT_SECTOR_SIZE);
	if (rc) {
		printf("Could not read stage 2 boot loader\n");
		return -1;
	}
	return 0;
}

/**
 * Write a new stage 2 bootloader to the eMMC device.  Any data at the end of
 * the last sector beyond the end of the stage 2 bootloader is zeroed.
 *
 * @param mmc		mmc device
 * @param stage2_image	pointer to image of new stage 2 bootloader
 * @param stage2_size	size of new stage 2 bootloader
 *
 * @return 0 for success, error otherwise
 */
int write_stage2(struct mmc *mmc, const uint8_t *stage2_image,
		 size_t stage2_size)
{
	int count = stage2_size / FAT_SECTOR_SIZE;
	uint8_t last_sector[FAT_SECTOR_SIZE];
	int last_size = stage2_size % FAT_SECTOR_SIZE;
	int rc;

	if (last_size) {
		memset(last_sector + last_size,  0, FAT_SECTOR_SIZE - last_size);
		memcpy(last_sector, stage2_image + count * FAT_SECTOR_SIZE,
		       last_size);
	}

	rc = mmc_write(mmc, STAGE2_START, stage2_image, count);
	if (!rc && last_size)
		rc = mmc_write(mmc, count, last_sector, 1);

	if (rc)
		printf("Error writing stage 2 image to MMC device\n");

	return rc;
}

/**
 * Update the stage 1 bootloader
 *
 * @param mmc		mmc data structure
 * @param new_stage1	new stage1 bootloader
 * @param keep_ptable	set to 1 to preserve the partition table already on
 *                      the device or 0 to overwrite it with whatever is encoded
 *			in the new stage 1 bootloader.
 * @return		0 for success, error otherwise
 */
int update_stage1(struct mmc *mmc, const uint8_t new_stage1[STAGE1_SIZE],
		  int keep_ptable)
{
	uint8_t old_stage1[FAT_SECTOR_SIZE];	/* Old boot sector */
	uint8_t new_image[STAGE1_SIZE];
	int rc;

	if (keep_ptable) {
		debug("Reading boot sector\n");
		rc = mmc_read(mmc, 0, old_stage1, 1);
		if (rc) {
			printf("Could not read old stage 1 boot sector from MMC device\n");
			goto out;
		}
		memcpy(new_image, new_stage1, 0x1be);
		/* Copy partition table from old stage 1 bootloader */
		memcpy(&new_image[0x1be], &old_stage1[0x1be], 0x40);
		/* Copy rest of new image */
		memcpy(&new_image[0x1fe], &new_stage1[0x1fe],
		       STAGE1_SIZE - 0x1fe);

		debug("Writing new stage 1\n");
		rc = mmc_write(mmc, 0, new_image, STAGE1_SIZE / FAT_SECTOR_SIZE);
	} else {
		debug("Writing new stage 1\n");
		rc = mmc_write(mmc, 0, new_stage1, STAGE1_SIZE / FAT_SECTOR_SIZE);
	}
out:
	if (rc)
		printf("Error writing new stage 1 bootloader to MMC device\n");

	return rc;
}

void usage(const char *name, const char *error, int retcode)
{
	if (error)
		puts(error);
	printf("Usage: %s [-r1 | -w1 | -u1 | -r2 | -w2 | -I] [-i | -o [filename | -]] [-f]\n"
	       "Where:\n"
	       "-r1    Read the stage 1 bootloader and output to a file specified with -o\n"
	       "-w1    Write the stage 1 bootloader from the file specified with -i.\n"
	       "       The partition table will not be preserved.\n"
	       "-u1    Update the stage 1 bootloader from the file specified with -i.\n"
	       "       The partition table will be preserved.\n"
	       "-r2    Read the stage 2 bootloader and output to a file specified with -o\n"
	       "-w2    Write the stage 2 bootloader from the file specified with -i\n"
	       "-I     Print out information about the MMC or SD device\n"
	       "-d1    Dump the stage 1 bootloader to stdout as hex/ASCII\n"
	       "\n"
	       "-i filename    Specify the file to read from.  If - then stdin is used.\n"
	       "               Required for -w1, -u1 and -w2\n"
	       "-o filename    Specify the file to write to.  If - then stdout is used.\n"
	       "               Required for -r1 and -r2.\n"
	       "-f      Enable fast mode, puts the cores in reset.\n",
	       name);
	exit(retcode);
}

int main(int argc, char *argv[])
{
	int rc;
	struct mmc mmc;
	uint8_t	*buffer = NULL;
	int i;
	const char *filename = NULL;
	FILE *file = NULL;
	int file_input = 0;
	int file_output = 0;
	enum bootloader_op op = NONE;
	size_t size;
	bool fast_mode = false;

	if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0)) {
		puts("Could not open remote connection");
		return 1;
	}
	octeon_remote_lock();

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-i")) {
			if (i >= argc - 1) {
				usage(argv[0], "Missing input filename", 1);
			}
			if (filename)
				usage(argv[0], "Filename already specified", 1);

			i++;
			filename = argv[i];
			file_input = 1;
		} else if (!strcmp(argv[i], "-o")) {
			if (i >= argc - 1)
				usage(argv[0], "Missing output filename", 1);

			if (filename)
				usage(argv[0], "Filename already specified", 1);

			i++;
			filename = argv[i];
			file_output = 1;
		} else if (!strcmp(argv[i], "-r1")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = READ_STAGE1;
		} else if (!strcmp(argv[i], "-r2")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = READ_STAGE2;
		} else if (!strcmp(argv[i], "-u1")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = UPDATE_STAGE1;
		} else if (!strcmp(argv[i], "-w1")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = WRITE_STAGE1;
		} else if (!strcmp(argv[i], "-r2")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = READ_STAGE2;
		} else if (!strcmp(argv[i], "-w2")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = WRITE_STAGE2;
			debug("OP: write stage2\n");
		} else if (!strcmp(argv[i], "-I")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = PRINT_MMC_INFO;
		} else if (!strcmp(argv[i], "-d1")) {
			if (op != NONE)
				usage(argv[0],
				      "Cannot specify multiple operations", 1);
			op = DUMP_STAGE1;
		} else if (!strcmp(argv[i], "-h")) {
			usage(argv[0], "", 0);
		} else if (!strcmp(argv[i], "-f")) {
			fast_mode = true;
		}
	}

	if (fast_mode) {
		debug("Fast mode enabled, stopping core 0\n");
		octeon_remote_stop_cores(1);
	}
	debug("Initializing MMC\n");
	rc = mmc_init(&mmc);
	debug("mmc_init returned %d\n", rc);
	if (rc) {
		printf("Error initializing MMC\n");
		goto cleanup;
	}
#if 0
	if (IS_SD(&mmc)) {
		rc = mmc_get_scr(&mmc);
		if (rc) {
			printf("Error getting SD SCR\n");
			mmc.scr.u64 = 0;
		}
	}
#endif
	if (filename && filename[0] == '-' && filename[1] != '\0')
		usage(argv[0], "Invalid or missing filename", 1);

	if (op == READ_STAGE1 || op == READ_STAGE2) {
		if (!filename && !file_output)
			usage(argv[0], "No output file specified", 1);

	} else if (op == WRITE_STAGE1 || op == UPDATE_STAGE1 || op == WRITE_STAGE2) {
		if (!filename && !file_input)
			usage(argv[0], "No input file specified", 1);
	}

	if (filename && file_input) {
		if (!strcmp(filename, "-"))
			file = stdin;
		else
			file = fopen(filename, "r");

	} else if (filename && file_output) {
		if (!strcmp(filename, "-"))
			file = stdout;
		else
			file = fopen(filename, "w");
	}
	if (filename && !file) {
		printf("%s: Error: could not open %s for %s\n",
		       argv[0], filename, file_input ? "reading" : "writing");
		return 2;
	}

	switch (op) {
	case PRINT_MMC_INFO:
		print_mmc_device_info(&mmc);
		break;
	case READ_STAGE1:
	case DUMP_STAGE1:
		buffer = calloc(1, STAGE1_SIZE);
		if (!buffer) {
			printf("Out of memory\n");
			return 3;
		}
		rc = mmc_read(&mmc, 0, buffer, STAGE1_SIZE / FAT_SECTOR_SIZE);
		if (rc) {
			printf("Error reading stage 1 bootloader from MMC\n");
			return 4;
		}
		if (op == READ_STAGE1) {
			size = fwrite(buffer, STAGE1_SIZE, 1, file);
			if (size != STAGE1_SIZE) {
				perror("write");
				return 2;
			}
		} else if (op == DUMP_STAGE1) {
			print_buffer(0, buffer, 1, STAGE1_SIZE, 0);
		}
		break;
	case WRITE_STAGE1:
	case UPDATE_STAGE1:
		debug("Writing/Updating stage 1\n");
		buffer = calloc(2, STAGE1_SIZE);
		if (!buffer) {
			puts("Out of memory");
			rc = 3;
			goto cleanup;
		}
		size = fread(buffer, 1, 2 * STAGE1_SIZE, file);
		if (!feof(file) || size > STAGE1_SIZE) {
			puts("Error: stage 1 bootloader file is too large");
			rc = 2;
			goto cleanup;
		}
		if (ferror(file)) {
			perror("read");
			rc = 2;
			goto cleanup;
		}
		debug("%s stage 1 bootloader size %zu bytes\n",
		      op == UPDATE_STAGE1 ? "Updating" : "Writing", size);
		rc = update_stage1(&mmc, buffer, op == UPDATE_STAGE1);
		if (rc) {
			printf("Error updating stage 1 bootloader\n");
			goto cleanup;
		}
		break;
	case READ_STAGE2:
		printf("Read stage 2 not yet implemented\n");
		break;
	case WRITE_STAGE2:
		debug("Writing stage 2\n");
		buffer = calloc(2, 1 << 20);
		if (!buffer) {
			puts("Out of memory\n");
			rc = 3;
			goto cleanup;
		}
		size = fread(buffer, 1, 2 * (1 << 20), file);
		if (!feof(file)) {
			puts("Error: stage 2 bootloader is too large\n");
			rc = 2;
			goto cleanup;
		}
		if (ferror(file)) {
			perror("read");
			rc = 2;
			goto cleanup;
		}
		debug("Stage 2 bootloader size: %zu bytes\n", size);
		rc = mmc_write(&mmc, 16, buffer,
			       (size + FAT_SECTOR_SIZE - 1) / FAT_SECTOR_SIZE);
		if (rc) {
			printf("Error writing stage 2 bootloader\n");
			rc = 3;
			goto cleanup;
		}
		puts("Done writing stage 2 bootloader to MMC");
		break;
	default:
		printf("Unknown operation %d\n", op);
		rc = 10;
		goto cleanup;
	}

cleanup:
	if (file)
		fclose(file);
	if (buffer)
		free(buffer);
	octeon_remote_unlock();
	octeon_remote_close();
	return rc;
}
