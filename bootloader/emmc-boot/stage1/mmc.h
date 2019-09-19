/*
 * Copyright 2008,2010 Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Copyright 2012 Cavium, Inc. <support@cavium.com>
 *
 * Based (loosely) on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
 */
#ifndef __MMC_H__
#define __MMC_H__

#define MAX_SD_SPEED		25000000
#define MAX_MMC_SPEED		20000000

#define SD_VERSION_SD		0x20000
#define SD_VERSION_2		(SD_VERSION_SD | 0x20)
#define SD_VERSION_1_0		(SD_VERSION_SD | 0x10)
#define SD_VERSION_1_1		(SD_VERSION_SD | 0x1a)
#define MMC_VERSION_MMC		0x10000
#define MMC_VERSION_UNKNOWN	(MMC_VERSION_MMC)
#define MMC_VERSION_1_2		(MMC_VERSION_MMC | 0x12)
#define MMC_VERSION_1_4		(MMC_VERSION_MMC | 0x14)
#define MMC_VERSION_2_2		(MMC_VERSION_MMC | 0x22)
#define MMC_VERSION_3		(MMC_VERSION_MMC | 0x30)
#define MMC_VERSION_4		(MMC_VERSION_MMC | 0x40)

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

/**
 * CID register
 */
typedef union {
	struct {
		uint8_t		mid;		/** Manufacturer ID */
		uint8_t		reserved:6;	/** Reserved */
		uint8_t		device:2;	/** Device/BGA */
		uint8_t		oem;		/** OEM/Application ID */
		char		prod_name[6];	/** Product name */
		uint8_t		revision;	/** Product revision */
		uint32_t	serial_no;	/** Product serial number */
		uint8_t		manu_date;	/** Manufacturer date */
		uint8_t		crc:7;		/** CRC7 checksum */
		uint8_t		one:1;		/** Unused, always 1 */
	} __attribute__((packed)) s;
	uint64_t	u128[2];
} __attribute__((packed)) mmc_cid_t;

/**
 * CSD register
 */
typedef union {
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
		uint8_t		reserved505_511[7];	/* 505-511 Reserved */
		uint8_t		s_cmd_set;		/* 504 Supported Command Sets */
		uint8_t		hpi_features;		/* 503 HPI features */
		uint8_t		bkops_support;		/* 502 Background operations support */
		uint8_t		max_packed_reads;	/* 501 Max packed read commands */
		uint8_t		max_packed_writes;	/* 500 Max packed write commands */
		uint8_t		data_tag_support;	/* 499 Data tag support */
		uint8_t		tag_unit_size;		/* 498 Tag unit size */
		uint8_t		tag_res_size;		/* 497 Tag resource size */
		uint8_t		context_capabilities;	/* 496 Context management capabilities */
		uint8_t		large_unit_size_m1;	/* 495 Large unit size */
		uint8_t		ext_support;		/* 494 Extended partitions attribute support */
		uint8_t		reserved253_493[255];	/* 253-493 Reserved */
		uint32_t	cache_size;		/* 249-252 Cache size */
		uint8_t		generic_cmd6_timeout;	/* 248 Generic CMD6 timeout */
		uint8_t		power_off_long_time;	/* 247 Power off notification(long) timeout */
		uint16_t	bkops_status;		/* 246 Background operations status */
		uint32_t	correctly_prg_sector_num;/* 242 Number of correctly programmed sectors */
		uint8_t		ini_timeout_ap;		/* 241 1st initialization time after partitioning */
		uint8_t		reserved240;		/* 240 Reserved */
		uint8_t		pwr_cl_ddr_52_360;	/* 239 Power class for 52MHz, DDR at 3.6V */
		uint8_t		pwr_cl_ddr_52_195;	/* 238 Power class for 52MHz, DDR at 1.95V */
		uint8_t		pwr_cl_200_360;		/* 237 Power class for 200MHz at 3.6V */
		uint8_t		pwr_cl_200_195;		/* 236 Power class for 200MHz at 1.95V */
		uint8_t		min_perf_ddr_w_8_52;	/* 235 Minimum write performance for 8bit at 52MHz in DDR mode */
		uint8_t		min_perf_ddr_r_8_52;	/* 234 Minimum read performance for 8bit at 52MHz in DDR mode */
		uint8_t		reserved_233;		/* 233 Reserved */
		uint8_t		trim_mult;		/* 232 TRIM multiplier */
		uint8_t		sec_feature_support;	/* 231 Secure feature support */
		uint8_t		obsolete_230;		/* 230 Obsolete */
		uint8_t		obsolete_229;		/* 229 Obsolete */
		uint8_t		boot_info;		/* 228 Boot information */
		uint8_t		reserved_227;		/* 227 Reserved */
		uint8_t		boot_size_multi;	/* 226 Boot partition size */
		uint8_t		acc_size;		/* 225 Access size */
		uint8_t		hc_erase_grp_size;	/* 224 High-capacity erase unit size */
		uint8_t		erase_timeout_mult;	/* 223 High-capacity erase timeout */
		uint8_t		rel_wr_sec_c;		/* 222 Reliable write sector count */
		uint8_t		hc_wp_grp_size;		/* 221 High-capacity write protect group size */
		uint8_t		s_c_vcc;		/* 220 Sleep current (VCC) */
		uint8_t		s_c_vccq;		/* 219 Sleep current (VCCQ) */
		uint8_t		reserved_218;		/* 218 Reserved */
		uint8_t		s_a_timeout;		/* 217 Sleep/awake timeout */
		uint8_t		reserved_216;		/* 216 Reserved */
		uint32_t	sec_count;		/* 212-215 Sector count */
		uint8_t		reserved_211;		/* 211 Reserved */
		uint8_t		min_perf_w_8_52;	/* 210 Minimum write performance for 8bit at 52MHz */
		uint8_t		min_perf_r_8_52;	/* 209 Minimum read performance for 8bit at 52MHz */
		uint8_t		min_perf_w_8_26_4_52;	/* 208 Minimum write performance for 8bit at 26MHz, 4bit at 52MHz */
		uint8_t		min_perf_r_8_26_4_52;	/* 207 Minimum read performance for 8bit at 26MHz, 4bit at 52MHz */
		uint8_t		min_perf_w_4_26;	/* 206 Minimum write performance for 4bit at 26MHz */
		uint8_t		min_perf_r_4_26;	/* 205 Minimum read performance for 4bit at 26MHz */
		uint8_t		reserved_204;		/* 204 Reserved */
		uint8_t		pwr_cl_26_360;		/* 203 Power class for 26MHz at 3.6V 1 R */
		uint8_t		pwr_cl_52_360;		/* 202 Power class for 52MHz at 3.6V 1 R */
		uint8_t		pwr_cl_26_195;		/* 201 Power class for 26MHz at 1.95V 1 R */
		uint8_t		pwr_cl_52_195;		/* 200 Power class for 52MHz at 1.95V 1 R */
		uint8_t		partition_switch_time;	/* 199 Partition switching timing */
		uint8_t		out_of_interrupt_time;	/* 198 Out-of-interrupt busy timing */
		uint8_t		driver_strength;	/* 197 I/O driver strength */
		uint8_t		device_type;		/* 196 Device type */
		uint8_t		reserved_195;		/* 195 Reserved */
		uint8_t		csd_struct_ver;		/* 194 CSD structure version */
		uint8_t		reserved_193;		/* 193 Reserved */
		uint8_t		ext_csd_rev;		/* 192 Extended CSD revision */
		/* modes_segment */
		uint8_t		cmd_set;		/* 191 Command set */
		uint8_t		reserved_190;		/* 190 Reserved */
		uint8_t		cmd_set_rev;		/* 189 Command set revision */
		uint8_t		reserved_188;		/* 188 Reserved */
		uint8_t		power_class;		/* 187 Power class */
		uint8_t		reserved_186;		/* 186 Reserved */
		uint8_t		hs_timing;		/* 185 High-speed interface timing */
		uint8_t		reserved_184;		/* 184 Reserved */
		uint8_t		bus_width;		/* 183 Bus width mode */
		uint8_t		reserved_182;		/* 182 Reserved */
		uint8_t		erased_mem_cont;	/* 181 Erased memory content */
		uint8_t		reserved_180;		/* 180 Reserved */
		uint8_t		partition_config;	/* 179 Partition configuration */
		uint8_t		boot_config_prot;	/* 178 Boot config protection */
		uint8_t		boot_bus_conditions;	/* 177 Boot bus conditions */
		uint8_t		reserved_176;		/* 176 Reserved */
		uint8_t		erase_group_def;	/* 175 High-density erase group definition */
		uint8_t		boot_wp_status;		/* 174 Boot write protection status registers */
		uint8_t		boot_wp;		/* 173 Boot area write protection register */
		uint8_t		reserved_172;		/* 172 Reserved */
		uint8_t		user_wp;		/* 171 User area write protection register */
		uint8_t		reserved_170;		/* 170 Reserved */
		uint8_t		fw_config;		/* 169 FW configuration */
		uint8_t		rpmb_size_mult;		/* 168 RPMB size */
		uint8_t		wr_rel_set;		/* 167 Write reliability setting register */
		uint8_t		wr_rel_param;		/* 166 Write reliability parameter register */
		uint8_t		sanatize_start;		/* 165 Start sanatize operation */
		uint8_t		bkops_start;		/* 164 Manually start background operations handshake */
		uint8_t		bkops_en;		/* 163 Enable background operations handshake */
		uint8_t		rst_n_function;		/* 162 H/W reset function */
		uint8_t		hpi_mgmt;		/* 161 HPI management */
		uint8_t		partitioning_support;	/* 160 Partitioning support */
		uint8_t		max_enh_size_multi[3];	/* 157-159 Max enhanced area size */
		uint8_t		partitions_attribute;	/* 156 Partitions attribute */
		uint8_t		partition_setting_completed;	/* 155 Partitioning setting */
		uint8_t		gp_size_mult[12];	/* 143-154 General purpose partition size */
		uint8_t		enh_size_mult[3];	/* 140-142 1Enhanced user data area size */
		uint32_t	enh_start_addr;		/* 136-139 Enhanced user data start address */
		uint8_t		reserved_135;		/* 135 Reserved */
		uint8_t		sec_bad_blk_mgmnt;	/* 134 Bad block management mode */
		uint8_t		reserved_133;		/* 133 Reserved */
		uint8_t 	tcase_support;		/* 132 Package case temperature is controlled */
		uint8_t		periodic_wakeup;	/* 131 Periodic wake-up */
		uint8_t		program_cid_csd_ddr_support;/* 130 Program CID/CSD in DDR mode support */
		uint8_t		reserved_128_129[2];	/* 128-129 Reserved */
		uint8_t		vendor_specific_field[64];/* 64-127 Vendor specific fields */
		uint8_t		native_sector_size;	/* 63 Native sector size */
		uint8_t		use_native_sector;	/* 62 Sector size emulation */
		uint8_t		data_sector_size;	/* 61 Sector size */
		uint8_t		ini_timeout_emu;	/* 60 1st initialization after disabling sector size emulation */
		uint8_t		class_6_ctrl;		/* 59 Class 6 commands control */
		uint8_t		dyncap_needed;		/* 58 Number of addressed group to be released */
		uint16_t	exception_events_ctrl;	/* 56-57 Exception events control */
		uint16_t	exception_events_status;/* 54-55 Exception events status */
		uint16_t	ext_partitions_attribute;/* 52-53 Extended partitions attribute */
		uint8_t		context_conf[15];	/* 37-51 Context configuration */
		uint8_t		packed_command_status;	/* 36 Packed command status */
		uint8_t		packed_failure_index;	/* 35 Packed command failure index */
		uint8_t		power_off_notification;	/* 34 Power off notification */
		uint8_t		cache_ctrl;		/* 33 Control to turn the cache on/off */
		uint8_t		flush_cache;		/* 32 Flushing of the cache */
		uint8_t		reserved_0_31[32];	/* 0 Reserved */
	} s __attribute__((packed));
	uint64_t buffer[512/8];
} mmc_ext_csd_t;

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
	uint32_t ocr;		/** OCR register */
	uint32_t tran_speed;	/** Transfer speed */
	uint16_t read_bl_len;	/** Read block length (512) */
	uint16_t write_bl_len;	/** Write block length (512) */
	uint16_t rca;		/** RCA address */
	uint16_t flags;
	uint8_t bus_width;	/** Bus width 1, 4 or 8 */
};
#endif /* __MMC_H__ */
