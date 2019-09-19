/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com).  All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * This header defines the CVMX interface to the NAND flash controller. The
 * basic operations common to all NAND devices are supported by this API, but
 * many more advanced functions are not support. The low level hardware supports
 * all types of transactions, but this API only implements the must commonly
 * used operations. This API performs no locking, so it is the responsibility of
 * the caller to make sure only one thread of execution is accessing the NAND
 * controller at a time. Most applications should not use this API directly but
 * instead use a flash logical layer supplied through a secondary system. For
 * example, the Linux MTD layer provides a driver for running JFFS2 on top of
 * NAND flash.
 *
 * <h2>Selecting the NAND Chip</h2>
 *
 * Octeon's NAND controller assumes a single NAND chip is connected to a boot
 * bus chip select. Throughout this API, NAND chips are referred to by the chip
 * select they are connected to (0-7). Chip select 0 will only be a NAND chip
 * when you are booting from NAND flash.
 *
 * <h2>NAND Addressing</h2>
 *
 * Various functions in cvmx-nand use addresses to index into NAND flash. All
 * functions us a uniform address translation scheme to map the passed address
 * into a NAND block, page, and column. In NAND flash a page represents the
 * basic unit of reads and writes. Each page contains a power of two number of
 * bytes and some number of extra out of band (OOB) bytes. A fixed number of
 * pages fit into each NAND block. Here is the mapping of bits in the cvmx-nand
 * address to the NAND hardware:
 * <pre>
 * 63     56      48      40      32      24      16       8      0
 * +-------+-------+-------+-------+-------+-------+-------+------+
 * |                                 64 bit cvmx-nand nand_address|
 * +------------------------------------------------+----+--------+
 * |                                          block |page| column |
 * +-------+-------+-------+-------+-------+--------+----+--------+
 * 63     56      48      40      32      24      16       8      0
 * </pre>
 * Basically the block, page, and column addresses are packet together. Before
 * being sent out the NAND pins for addressing the column is padded out to an
 * even number of bytes. This means that column address are 2 bytes, or 2
 * address cycles, for page sizes between 512 and 65536 bytes. Page sizes
 * between 128KB and 16MB would use 3 column address cycles. NAND device
 * normally either have 32 or 64 pages per block, needing either 5 or 6 address
 * bits respectively. This means you have 10 bits for block address using 4
 * address cycles, or 18 for 5 address cycles. Using the cvmx-nand addressing
 * scheme, it is not possible to directly index the OOB data. Instead you can
 * access it by reading or writing more data than the normal page size would
 * allow. Logically the OOB data is appended onto the the page data. For
 * example, this means that a read of 65 bytes from a column address of 0x7ff
 * would yield byte 2047 of the page and then 64 bytes of OOB data.
 *
 * <hr>$Revision: 35726 $<hr>
 */

#ifndef __CVMX_NAND_H__
#define __CVMX_NAND_H__

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/* Maxium PAGE + OOB size supported.  This is used to size
 * buffers, some that must be statically allocated.
 */
#define CVMX_NAND_MAX_PAGE_AND_OOB_SIZE      (8192 + 744)

/* Block size for boot ECC */
#define CVMX_NAND_BOOT_ECC_BLOCK_SIZE    (256)
/* ECC bytes for each block */
#define CVMX_NAND_BOOT_ECC_ECC_SIZE      (8)

/**
 * Flags to be passed to the initialize function
 */
typedef enum {
	CVMX_NAND_INITIALIZE_FLAGS_DONT_PROBE = 1 << 1,
	CVMX_NAND_INITIALIZE_FLAGS_DEBUG = 1 << 15,
} cvmx_nand_initialize_flags_t;

/**
 * Return codes from NAND functions
 */
typedef enum {
	CVMX_NAND_SUCCESS = 0,
	CVMX_NAND_NO_MEMORY = -1,
	CVMX_NAND_BUSY = -2,
	CVMX_NAND_INVALID_PARAM = -3,
	CVMX_NAND_TIMEOUT = -4,
	CVMX_NAND_ERROR = -5,
	CVMX_NAND_NO_DEVICE = -6,
} cvmx_nand_status_t;

/**
 * NAND NOP command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_4_63:60;
	uint64_t zero:4;
} cvmx_nand_cmd_nop_t;

/**
 * NAND SET_TM_PAR command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t tim_par7:8;
	uint64_t tim_par6:8;
	uint64_t tim_par5:8;
	uint64_t tim_par4:8;
	uint64_t tim_par3:8;
	uint64_t tim_par2:8;
	uint64_t tim_par1:8;
	uint64_t tim_mult:4;
	uint64_t one:4;
} cvmx_nand_cmd_set_tm_par_t;

/**
 * NAND WAIT command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_11_63:53;
	uint64_t n:3;
	uint64_t reserved_5_7:3;
	uint64_t r_b:1;
	uint64_t two:4;
} cvmx_nand_cmd_wait_t;

/**
 * NAND CHIP_EN command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_10_63:54;
	uint64_t width:2;
	uint64_t one:1;
	uint64_t chip:3;
	uint64_t three:4;
} cvmx_nand_cmd_chip_en_t;

/**
 * NAND CHIP_DIS command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_4_63:60;
	uint64_t three:4;
} cvmx_nand_cmd_chip_dis_t;

/**
 * NAND CLE command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_25_63:39;
	uint64_t clen3:3;
	uint64_t clen2:3;
	uint64_t clen1:3;
	uint64_t cmd_data:8;
	uint64_t reserved_4_7:4;
	uint64_t four:4;
} cvmx_nand_cmd_cle_t;

/**
 * NAND ALE command definition
 */
typedef struct {
	uint64_t reserved_96_127:32;
	uint64_t adr_bytes_h:32;
	uint64_t adr_bytes_l:32;
	uint64_t reserved_28_31:4;
	uint64_t alen4:3;
	uint64_t alen3:3;
	uint64_t alen2:3;
	uint64_t alen1:3;
	uint64_t reserved_12_15:4;
	uint64_t adr_byte_num:4;
	uint64_t reserved_4_7:4;
	uint64_t five:4;
} cvmx_nand_cmd_ale_t;

/**
 * NAND WR command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_31_63:34;
	uint64_t wrn2:3;
	uint64_t wrn1:3;
	uint64_t reserved_20_24:4;
	uint64_t data_bytes:16;
	uint64_t eight:4;
} cvmx_nand_cmd_wr_t;

/**
 * NAND RD command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_32_63:32;
	uint64_t rdn4:3;
	uint64_t rdn3:3;
	uint64_t rdn2:3;
	uint64_t rdn1:3;
	uint64_t data_bytes:16;
	uint64_t nine:4;
} cvmx_nand_cmd_rd_t;

/**
 * NAND RD_EDO command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_32_63:32;
	uint64_t rdn4:3;
	uint64_t rdn3:3;
	uint64_t rdn2:3;
	uint64_t rdn1:3;
	uint64_t data_bytes:16;
	uint64_t ten:4;
} cvmx_nand_cmd_rd_edo_t;

/**
 * NAND WAIT_STATUS command definition
 */
typedef struct {
	uint64_t rdn4:3;
	uint64_t rdn3:3;
	uint64_t rdn2:3;
	uint64_t rdn1:3;
	uint64_t comp_byte:8;
	uint64_t and_mask:8;
	uint64_t nine:4;
	uint64_t reserved_28_95:64;
	uint64_t clen4:3;
	uint64_t clen3:3;
	uint64_t clen2:3;
	uint64_t clen1:3;
	uint64_t data:8;
	uint64_t reserved_4_7:4;
	uint64_t eleven:4;
} cvmx_nand_cmd_wait_status_t;

/**
 * NAND WAIT_STATUS_ALE command definition
 */
typedef struct {
	uint64_t rdn4:3;
	uint64_t rdn3:3;
	uint64_t rdn2:3;
	uint64_t rdn1:3;
	uint64_t comp_byte:8;
	uint64_t and_mask:8;
	uint64_t nine:4;
	uint64_t adr_bytes:32;
	uint64_t reserved_60_63:4;
	uint64_t alen4:3;
	uint64_t alen3:3;
	uint64_t alen2:3;
	uint64_t alen1:3;
	uint64_t reserved_44_47:4;
	uint64_t adr_byte_num:4;
	uint64_t five:4;
	uint64_t reserved_25_31:7;
	uint64_t clen3:3;
	uint64_t clen2:3;
	uint64_t clen1:3;
	uint64_t data:8;
	uint64_t reserved_4_7:4;
	uint64_t eleven:4;
} cvmx_nand_cmd_wait_status_ale_t;

/**
 * NAND BUS_ACQ command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_8_63:56;
	uint64_t one:4;
	uint64_t fifteen:4;
} cvmx_nand_cmd_bus_acq_t;

/**
 * NAND BUS_REL command definition
 */
typedef struct {
	uint64_t reserved_64_127:64;
	uint64_t reserved_8_63:56;
	uint64_t zero:4;
	uint64_t fifteen:4;
} cvmx_nand_cmd_bus_rel_t;

/**
 * NAND command union of all possible commands
 */
typedef union {
	uint64_t u64[2];
	cvmx_nand_cmd_nop_t nop;
	cvmx_nand_cmd_set_tm_par_t set_tm_par;
	cvmx_nand_cmd_wait_t wait;
	cvmx_nand_cmd_chip_en_t chip_en;
	cvmx_nand_cmd_chip_dis_t chip_dis;
	cvmx_nand_cmd_cle_t cle;
	cvmx_nand_cmd_ale_t ale;
	cvmx_nand_cmd_rd_t rd;
	cvmx_nand_cmd_rd_edo_t rd_edo;
	cvmx_nand_cmd_wr_t wr;
	cvmx_nand_cmd_wait_status_t wait_status;
	cvmx_nand_cmd_wait_status_ale_t wait_status_ale;
	cvmx_nand_cmd_bus_acq_t bus_acq;
	cvmx_nand_cmd_bus_rel_t bus_rel;
	struct {
		uint64_t reserved_64_127:64;
		uint64_t reserved_4_63:60;
		uint64_t op_code:4;
	} s;
} cvmx_nand_cmd_t;

typedef struct __attribute__ ((packed)) cvmx_nand_onfi_param_page {
	char onfi[4];			/**
					 * Bytes 0-3: The ASCII characters
					 * 'O', 'N', 'F', 'I'
					 */
	uint16_t revision_number;	/**
					 * Bytes 4-5: ONFI revision number
                                         * - 2-15 Reserved (0)
                                         * - 1    1 = supports ONFI version 1.0
                                         * - 0    Reserved (0)
                                         */
	uint16_t features;		/**
					 * Bytes 6-7: Features supported
                                         * - 5-15    Reserved (0)
                                         * - 4       1 = supports odd to even
                                         *		 page Copyback
                                         * - 3       1 = supports interleaved
                                         *		 operations
                                         * - 2       1 = supports non-sequential
                                         *		 page programming
                                         * - 1       1 = supports multiple LUN
                                         *		 operations
                                         * - 0       1 = supports 16-bit data
                                         *		 bus width
                                         */
	uint16_t optional_commands;	/**
					 * Bytes 8-9: Optional commands
					 * supported
					 *   - 6-15   Reserved (0)
					 *   - 5      1 = supports Read Unique
					 *		  ID
					 *   - 4      1 = supports Copyback
					 *   - 3      1 = supports Read Status
					 *		  Enhanced
					 *   - 2      1 = supports Get Features
					 *		  and Set Features
					 *   - 1      1 = supports Read Cache
					 *		  commands
					 *   - 0      1 = supports Page Cache
					 *		  Program command
					 */
	uint8_t reserved_10_11[2];	/** Bytes 10-11: Reserved */
	uint16_t extended_param_page_len;/** Extended parameter page length */
	uint8_t num_param_pages;	/** Number of parameter pages */
	uint8_t reserved_15_31[17]; 	/** Bytes 10-31: Reserved */
	char manufacturer[12];		/**
					 * Bytes 32-43: Device manufacturer
					 * (12 ASCII characters)
					 */
	char model[20];			/**
					 * Bytes 40-63: Device model
					 * (20 ASCII characters)
					 */
	uint8_t jedec_id;	    	/** Byte 64: JEDEC manufacturer ID */
	uint16_t date_code;	    	/** Byte 65-66: Date code */
	uint8_t reserved_67_79[13];	/** Bytes 67-79: Reserved */
	uint32_t page_data_bytes;	/**
					 * Bytes 80-83: Number of data bytes per
					 * page
					 */
	uint16_t page_spare_bytes;	/**
					 * Bytes 84-85: Number of spare bytes
					 * per page
					 */
	uint32_t partial_page_data_bytes;	/**
						* Bytes 86-89: Number of data
						* bytes per partial page
						*/
	uint16_t partial_page_spare_bytes;	/**
						 * Bytes 90-91: Number of spare
						 * bytes per partial page
						 */
	uint32_t pages_per_block;	/**
					 * Bytes 92-95: Number of pages per
					 * block
					 */
	uint32_t blocks_per_lun;	/**
					 * Bytes 96-99: Number of blocks per
					 * logical unit (LUN)
					 */
	uint8_t number_lun;		/**
					 * Byte 100: Number of logical units
					 * (LUNs)
					 */
	uint8_t address_cycles;		/**
					 * Byte 101: Number of address cycles
                                         * - 4-7     Column address cycles
                                         * - 0-3     Row address cycles
                                         */
	uint8_t bits_per_cell;		/** Byte 102: Number of bits per cell */
	uint16_t bad_block_per_lun;	/**
					 *  Bytes 103-104: Bad blocks maximum
					 * per LUN
					 */
	uint16_t block_endurance;	/** Bytes 105-106: Block endurance */
	uint8_t good_blocks;		/**
					 * Byte 107: Guaranteed valid blocks at
					 * beginning of target
					 */
	uint16_t good_block_endurance;	/**
					 * Bytes 108-109: Block endurance for
					 * guaranteed valid blocks
					 */
	uint8_t programs_per_page;	/**
					 * Byte 110: Number of programs per page
					 */
	uint8_t partial_program_attrib;	/**
					 * Byte 111: Partial programming
					 * attributes
					 * - 5-7    Reserved
					 * - 4      1 = partial page layout is
					 *		partial page data
					 *		followed by partial
					 *		page spare
					 * - 1-3    Reserved
					 * - 0      1 = partial page programming
					 *		has constraints
					 */
	uint8_t bits_ecc;		/**
					 * Byte 112: Number of bits ECC
					 * correctability
					 */
	uint8_t interleaved_address_bits;	/**
						 * Byte 113: Number of
						 * interleaved address bits
						 * - 4-7    Reserved (0)
						 * - 0-3    Number of
						 *	    interleaved address
						 *	    bits
						 */
	uint8_t interleaved_attrib;	/**
					 * Byte 114: Interleaved operation
					 * attributes
					 * - 4-7    Reserved (0)
					 * - 3      Address restrictions for
					 *	    program cache
					 * - 2      1 = program cache supported
					 * - 1      1 = no block address
					 *		restrictions
					 * - 0      Overlapped / concurrent
					 *	    interleaving support
					 */
	uint8_t reserved_115_127[13];	/** Bytes 115-127: Reserved (0) */

	uint8_t pin_capacitance;	/** Byte 128: I/O pin capacitance */
	uint16_t timing_mode;		/**
					 * Byte 129-130: Timing mode support
					 * - 6-15   Reserved (0)
					 * - 5      1 = supports timing mode 5
					 * - 4      1 = supports timing mode 4
					 * - 3      1 = supports timing mode 3
					 * - 2      1 = supports timing mode 2
					 * - 1      1 = supports timing mode 1
					 * - 0      1 = supports timing mode 0,
					 *		shall be 1
					 */
	uint16_t cache_timing_mode;	/**
					 * Byte 131-132: Program cache timing
					 * mode support
					 * - 6-15   Reserved (0)
					 * - 5      1 = supports timing mode 5
					 * - 4      1 = supports timing mode 4
					 * - 3      1 = supports timing mode 3
					 * - 2      1 = supports timing mode 2
					 * - 1      1 = supports timing mode 1
					 * - 0      1 = supports timing mode 0
					 */
	uint16_t t_prog;		/**
					 * Byte 133-134: Maximum page program
					 * time (us)
					 */
	uint16_t t_bers;		/**
					 * Byte 135-136: Maximum block erase
					 * time (us)
					 */
	uint16_t t_r;			/**
					 * Byte 137-148: Maximum page read time
					 * (us)
					 */
	uint16_t t_ccs;			/**
					 * Byte 139-140: Minimum change column
					 * setup time (ns)
					 */
	uint8_t reserved_141_163[23];	/** Byte 141-163: Reserved (0) */
	uint16_t vendor_revision;	/**
					 * Byte 164-165: Vendor specific
					 * Revision number
					 */
	uint8_t vendor_specific[88];	/** Byte 166-253: Vendor specific */
	uint16_t crc;			/** Byte 254-255: Integrity CRC */
} cvmx_nand_onfi_param_page_t;

/**
 * Structure to store the parameters that we care about that
 * describe the ONFI speed modes.  This is used to configure
 * the flash timing to match what is reported in the
 * parameter page of the ONFI flash chip.
 */
typedef struct onfi_speed_mode_desc {
	char twp;
	char twh;
	char twc;
	char tclh;
	char tals;
} onfi_speed_mode_desc_t;

typedef enum {
	CVMX_NAND_MICRON_HW_ECC = 1 << 1,
	CVMX_NAND_NUMONYX = 1 << 2,
} cvmx_nand_state_flags_t;

/**
 * Structure used to store data about the NAND devices hooked
 * to the bootbus.
 */
typedef struct cvmx_nand_state {
	int page_size;
	int oob_size;
	int pages_per_block;
	int blocks;
	int tim_mult;
	int tim_par[8];
	int clen[4];
	int alen[4];
	int rdn[4];
	int wrn[2];
	int onfi_timing;
	cvmx_nand_state_flags_t flags;
} cvmx_nand_state_t;

/* The following are taken from U-Boot for higher-level NAND support for ECC
 * layout and whatnot.
 */

/*
 * NAND Flash Manufacturer ID Codes
 */
#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01
#define NAND_MFR_MACRONIX	0xc2
#define NAND_MFR_EON		0x92

/* Options for the bad block table descriptors */

/* The number of bits used per block in the bbt on the device */
#define NAND_BBT_NRBITS_MSK	0x0000000F
#define NAND_BBT_1BIT		0x00000001
#define NAND_BBT_2BIT		0x00000002
#define NAND_BBT_4BIT		0x00000004
#define NAND_BBT_8BIT		0x00000008
/* The bad block table is in the last good block of the device */
#define NAND_BBT_LASTBLOCK	0x00000010
/* The bbt is at the given page, else we must scan for the bbt */
#define NAND_BBT_ABSPAGE	0x00000020
/* bbt is stored per chip on multichip devices */
#define NAND_BBT_PERCHIP	0x00000080
/* bbt has a version counter at offset veroffs */
#define NAND_BBT_VERSION	0x00000100
/* Create a bbt if none exists */
#define NAND_BBT_CREATE		0x00000200
/*
 * Create an empty BBT with no vendor information. Vendor's information may be
 * unavailable, for example, if the NAND controller has a different data and OOB
 * layout or if this information is already purged. Must be used in conjunction
 * with NAND_BBT_CREATE.
 */
#define NAND_BBT_CREATE_EMPTY	0x00000400
/* Search good / bad pattern through all pages of a block */
#define NAND_BBT_SCANALLPAGES	0x00000800
/* Scan block empty during good / bad block scan */
#define NAND_BBT_SCANEMPTY	0x00001000
/* Write bbt if neccecary */
#define NAND_BBT_WRITE		0x00002000
/* Read and write back block contents when writing bbt */
#define NAND_BBT_SAVECONTENT	0x00004000
/* Search good / bad pattern on the first and the second page */
#define NAND_BBT_SCAN2NDPAGE	0x00008000
/* Search good / bad pattern on the last page of the eraseblock */
#define NAND_BBT_SCANLASTPAGE	0x00010000
/*
 * Use a flash based bad block table. By default, OOB identifier is saved in
 * OOB area. This option is passed to the default bad block table function.
 */
#define NAND_BBT_USE_FLASH	0x00020000
/*
 * Do not store flash based bad block table marker in the OOB area; store it
 * in-band.
 */
#define NAND_BBT_NO_OOB		0x00040000
/*
 * Do not write new bad block markers to OOB; useful, e.g., when ECC covers
 * entire spare area. Must be used with NAND_BBT_USE_FLASH.
 */
#define NAND_BBT_NO_OOB_BBM	0x00080000

/*
 * Flag set by nand_create_default_bbt_descr(), marking that the nand_bbt_descr
 * was allocated dynamicaly and must be freed in nand_release(). Has no meaning
 * in nand_chip.bbt_options.
 */
#define NAND_BBT_DYNAMICSTRUCT	0x80000000

/* The maximum number of blocks to scan for a bbt */
#define NAND_BBT_SCAN_MAXBLOCKS	4


#define MTD_MAX_OOBFREE_ENTRIES	8

/*
 * This constant declares the max. oobsize / page, which
 * is supported now. If you add a chip with bigger oobsize/page
 * adjust this accordingly.
 */
#define NAND_MAX_OOBSIZE	640
#define NAND_MAX_PAGESIZE	8192

/*
 * Constants for hardware specific CLE/ALE/NCE function
 *
 * These are bits which can be or'ed to set/clear multiple
 * bits in one go.
 */
/* Select the chip by setting nCE to low */
#define NAND_NCE		0x01
/* Select the command latch by setting CLE to high */
#define NAND_CLE		0x02
/* Select the address latch by setting ALE to high */
#define NAND_ALE		0x04

#define NAND_CTRL_CLE		(NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE		(NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE	0x80

/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_GET_FEATURES	0xee
#define NAND_CMD_SET_FEATURES	0xef
#define NAND_CMD_RESET		0xff

#define NAND_CMD_LOCK		0x2a
#define NAND_CMD_LOCK_TIGHT	0x2c
#define NAND_CMD_UNLOCK1	0x23
#define NAND_CMD_UNLOCK2	0x24
#define NAND_CMD_LOCK_STATUS	0x7a

/* Extended commands for large page devices */
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

/* Extended commands for AG-AND device */
/*
 * Note: the command for NAND_CMD_DEPLETE1 is really 0x00 but
 *       there is no way to distinguish that from NAND_CMD_READ0
 *       until the remaining sequence of commands has been completed
 *       so add a high order bit and mask it off in the command.
 */
#define NAND_CMD_DEPLETE1	0x100
#define NAND_CMD_DEPLETE2	0x38
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_STATUS_ERROR	0x72
/* multi-bank error status (banks 0-3) */
#define NAND_CMD_STATUS_ERROR0	0x73
#define NAND_CMD_STATUS_ERROR1	0x74
#define NAND_CMD_STATUS_ERROR2	0x75
#define NAND_CMD_STATUS_ERROR3	0x76
#define NAND_CMD_STATUS_RESET	0x7f
#define NAND_CMD_STATUS_CLEAR	0xff

#define NAND_CMD_NONE		-1

/* Status bits */
#define NAND_STATUS_FAIL	0x01
#define NAND_STATUS_FAIL_N1	0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY	0x40
#define NAND_STATUS_WP		0x80

/** Flags used in octeon_nand_priv */
#define OCTEON_NAND_GET_SET_FEATURE		1

/*
 * Constants for ECC_MODES
 */
typedef enum {
	NAND_ECC_NONE,
	NAND_ECC_SOFT,
	NAND_ECC_HW,
	NAND_ECC_HW_SYNDROME,
	NAND_ECC_HW_OOB_FIRST,
	NAND_ECC_SOFT_BCH,
} nand_ecc_modes_t;

/* Cell info constants */
#define NAND_CI_CHIPNR_MSK	0x03
#define NAND_CI_CELLTYPE_MSK	0x0C

/* ONFI timing mode, used in both asynchronous and synchronous mode */
#define ONFI_TIMING_MODE_0		(1 << 0)
#define ONFI_TIMING_MODE_1		(1 << 1)
#define ONFI_TIMING_MODE_2		(1 << 2)
#define ONFI_TIMING_MODE_3		(1 << 3)
#define ONFI_TIMING_MODE_4		(1 << 4)
#define ONFI_TIMING_MODE_5		(1 << 5)
#define ONFI_TIMING_MODE_UNKNOWN	(1 << 6)

/* ONFI feature address */
#define ONFI_FEATURE_ADDR_TIMING_MODE	0x1

/* ONFI subfeature parameters length */
#define ONFI_SUBFEATURE_PARAM_LEN	4

/*
 * Constants for oob configuration
 */
#define NAND_SMALL_BADBLOCK_POS		5
#define NAND_LARGE_BADBLOCK_POS		0

struct nand_onfi_params {
	/* rev info and features block */
	/* 'O' 'N' 'F' 'I'  */
	u8 sig[4];
	__le16 revision;
	__le16 features;
	__le16 opt_cmd;
	u8 jedec_jtg_primary_adv_cmd;
	u8 reserved;
	__le16 ext_param_page_len;
	u8 num_param_pages;
	u8 reserved2[17];

	/* manufacturer information block */
	char manufacturer[12];
	char model[20];
	u8 jedec_id;
	__le16 date_code;
	u8 reserved3[13];

	/* memory organization block */
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	__le32 data_bytes_per_ppage;
	__le16 spare_bytes_per_ppage;
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	u8 programs_per_page;
	u8 ppage_attr;
	u8 ecc_bits;
	u8 interleaved_bits;
	u8 interleaved_ops;
	u8 reserved4[13];

	/* electrical parameter block */
	u8 io_pin_capacitance_max;
	__le16 async_timing_mode;
	__le16 program_cache_timing_mode;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_ccs;
	__le16 src_sync_timing_mode;
	__le16 src_ssync_features;
	__le16 clk_pin_capacitance_typ;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	u8 input_pin_capacitance_max;
	u8 driver_strenght_support;
	__le16 t_int_r;
	__le16 t_ald;
	u8 reserved5[7];

	/* vendor */
	u8 reserved6[90];

	__le16 crc;
} __attribute__((packed));

enum nand_extended_section_type {
	NAND_EXTENDED_UNUSED = 0,
	NAND_EXTENDED_SECTION_TYPE = 1,
	NAND_EXTENDED_ECC = 2,
};

struct nand_extended_section_info {
	uint8_t type;
	uint8_t length;
};

struct nand_extended_ecc_info {
	uint8_t ecc_bits;	/** Number of bits ECC correctability */
	uint8_t ecc_size;	/** 1 << ecc_size */
	uint16_t max_lun_bad_blocks;	/** Max bad blocks per LUN */
	uint16_t block_endurance;
	uint16_t reserved;
};

struct nand_extended_param_page_hdr {
	uint16_t crc;
	char sig[4];	/* Should be 'EPPS' */
	uint8_t reserved0[10];
	struct nand_extended_section_info section_types[8];
};

#define ONFI_CRC_BASE	0x4F4E

/**
 * struct nand_buffers - buffer structure for read/write
 * @ecccalc:	buffer for calculated ECC
 * @ecccode:	buffer for ECC read from flash
 * @databuf:	buffer for data - dynamically sized
 *
 * Do not change the order of buffers. databuf and oobrbuf must be in
 * consecutive order.
 */
struct nand_buffers {
	uint8_t	ecccalc[ALIGN(NAND_MAX_OOBSIZE, ARCH_DMA_MINALIGN)];
	uint8_t	ecccode[ALIGN(NAND_MAX_OOBSIZE, ARCH_DMA_MINALIGN)];
	uint8_t databuf[ALIGN(NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE,
			      ARCH_DMA_MINALIGN)];
};

/**
 * struct nand_ecc_ctrl - Control structure for ECC
 * @mode:	ECC mode
 * @steps:	number of ECC steps per page
 * @size:	data bytes per ECC step
 * @bytes:	ECC bytes per step
 * @strength:	max number of correctible bits per ECC step
 * @total:	total number of ECC bytes per page
 * @prepad:	padding information for syndrome based ECC generators
 * @postpad:	padding information for syndrome based ECC generators
 * @layout:	ECC layout control struct pointer
 * @priv:	pointer to private ECC control data
 * @hwctl:	function to control hardware ECC generator. Must only
 *		be provided if an hardware ECC is available
 * @calculate:	function for ECC calculation or readback from ECC hardware
 * @correct:	function for ECC correction, matching to ECC generator (sw/hw)
 * @read_page_raw:	function to read a raw page without ECC
 * @write_page_raw:	function to write a raw page without ECC
 * @read_page:	function to read a page according to the ECC generator
 *		requirements; returns maximum number of bitflips corrected in
 *		any single ECC step, 0 if bitflips uncorrectable, -EIO hw error
 * @read_subpage:	function to read parts of the page covered by ECC;
 *			returns same as read_page()
 * @write_page:	function to write a page according to the ECC generator
 *		requirements.
 * @write_oob_raw:	function to write chip OOB data without ECC
 * @read_oob_raw:	function to read chip OOB data without ECC
 * @read_oob:	function to read chip OOB data
 * @write_oob:	function to write chip OOB data
 */
struct nand_ecc_ctrl {
	nand_ecc_modes_t mode;
	int steps;
	int size;
	int bytes;
	int total;
	int strength;
	int prepad;
	int postpad;
	struct nand_ecclayout	*layout;
	void *priv;
};

struct nand_oobfree {
	uint16_t offset;
	uint16_t length;
};

struct nand_ecclayout {
	uint16_t eccbytes;
	uint16_t oobavail;
	uint32_t eccpos[NAND_MAX_OOBSIZE];
	struct nand_oobfree oobfree[MTD_MAX_OOBFREE_ENTRIES];
};

struct octeon_nand_chip {
	uint64_t chipsize;
	int selected_page;
	uint32_t erasesize;
	uint32_t pagemask;
	uint32_t writesize;
	uint32_t pages_per_block;
	uint16_t pagesize;
	uint16_t oobsize;
	uint16_t badblockpos;
	uint16_t data_len;
	uint16_t index;
	uint8_t badblockbits;
	uint8_t cellinfo;
	uint8_t bbt_options;
	uint8_t status;
	uint8_t use_status;
	uint8_t selected_chip;
	uint8_t page_shift;
	uint8_t onfi_version;
	uint8_t flags;
	uint8_t num_onfi_pages;
	struct nand_onfi_params onfi_params;
	struct nand_ecclayout layout;
	struct nand_ecc_ctrl ecc;
	uint8_t eccmask[NAND_MAX_OOBSIZE];
	uint8_t oob_poi[NAND_MAX_OOBSIZE];
	struct nand_buffers buffers;
};
/**
 * Array indexed by bootbus chip select with information
 * about NAND devices.
 */
extern struct cvmx_nand_state cvmx_nand_state[8];
/**
 * Called to initialize the NAND controller for use.  Note that you must be
 * running out of L2 or memory and not NAND before calling this function.
 * When probing for NAND chips, this function attempts to autoconfigure based
 * on the NAND parts detected.  It currently supports autodetection for ONFI
 * parts (with valid parameter pages), and some Samsung NAND parts (decoding ID
 * bits).  If autoconfiguration fails, the defaults set with
 * __set_chip_defaults() prior to calling cvmx_nand_initialize() are used.
 * If defaults are set and the CVMX_NAND_INITIALIZE_FLAGS_DONT_PROBE flag is
 * provided, the defaults are used for all chips in the active_chips mask.
 *
 * @param flags  Optional initialization flags
 *               If the CVMX_NAND_INITIALIZE_FLAGS_DONT_PROBE flag is passed,
 *		 chips are not probed and the default parameters, if set with
 *		 cvmx_nand_set_defaults, are used for all chips
 *               in the active_chips mask.
 * @param active_chips
 *               Each bit in this parameter represents a chip select that might
 *               contain NAND flash. Any chip select present in this bitmask may
 *               be connected to NAND. It is normally safe to pass 0xff here and
 *               let the API probe all 8 chip selects.
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_initialize(cvmx_nand_initialize_flags_t flags,
					int active_chips);

/**
 * This function may be called before cvmx_nand_initialize to set default values
 * that will be used for NAND chips that do not identify themselves in a way
 * that allows autoconfiguration. (ONFI chip with missing parameter page, for
 * example).
 * The parameters set by this function will be used by _all_ non-autoconfigured
 * NAND chips.
 *
 *
 *   NOTE:  This function signature is _NOT_ stable, and will change in the
 *          future as required to support various NAND chips.
 *
 * @param page_size page size in bytes
 * @param oob_size  Out of band size in bytes (per page)
 * @param pages_per_block
 *                  number of pages per block
 * @param blocks    Total number of blocks in device
 * @param onfi_timing_mode
 *                  ONFI timing mode
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_set_defaults(int page_size, int oob_size,
					  int pages_per_block,
					  int blocks,
					  int onfi_timing_mode);

/**
 * Call to shutdown the NAND controller after all transactions
 * are done. In most setups this will never be called.
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_shutdown(void);

/**
 * Returns a bitmask representing the chip selects that are connected to NAND
 * chips.  This can be called after initializing to determine the actual
 * number of NAND chips found.  Each bit in the response coresponds to a chip
 * select.
 *
 * @return Zero if no NAND chips were found. Otherwise a bit is set for
 *         each chip select (1<<chip).
 */
int cvmx_nand_get_active_chips(void);

/**
 * Get the page size, excluding out of band data. This  function
 * will return zero for chip selects not connected to NAND.
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Page size in bytes or a negative cvmx_nand_status_t error code
 *	   on failure
 */
static int cvmx_nand_get_page_size(int chip);

/**
 * Override the timing parameters for a NAND chip
 *
 * @param chip     Chip select to override
 * @param tim_mult
 * @param tim_par
 * @param clen
 * @param alen
 * @param rdn
 * @param wrn
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_set_timing(int chip, int tim_mult,
					int tim_par[7], int clen[4],
					int alen[4], int rdn[4],
					int wrn[2]);

/**
 * Submit a command to the NAND command queue. Generally this
 * will not be used directly. Instead most programs will use the other
 * higher level NAND functions.
 *
 * @param cmd    Command to submit
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_submit(cvmx_nand_cmd_t cmd);

/**
 * Read a page from NAND. If the buffer has room, the out of band
 * data will be included.
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               Location in NAND to read. See description in file comment
 * @param buffer_address
 *               Physical address to store the result at
 * @param buffer_length
 *               Number of bytes to read
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *	   failure
 */
int cvmx_nand_page_read(int chip, uint64_t nand_address,
			uint64_t buffer_address, int buffer_length);

/**
 * Read random data from NAND.  This adjusts the column address before starting
 * a read operation.
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               Location in NAND to read.  NOTE: only the column bits are used.
 * @param buffer_address
 *               Physical address to store the result at
 * @param buffer_length
 *               Number of bytes to read
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *	   failure
 */
int cvmx_nand_random_data_out(int chip, uint64_t nand_address,
			      uint64_t buffer_address,
			      int buffer_length);

/**
 * Read the NAND ID information
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               NAND address to read ID from. Usually this is either 0x0 or
 *		 0x20.
 * @param buffer_address
 *               Physical address to store data in
 * @param buffer_length
 *               Length of the buffer. Usually this is 4 bytes
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *         failure
 */
int cvmx_nand_read_id(int chip, uint64_t nand_address,
		      uint64_t buffer_address, int buffer_length);

/**
 * Read the NAND parameter page
 *
 * @param chip   Chip select for NAND flash
 * @param buffer_address
 *               Physical address to store data in
 * @param buffer_length
 *               Length of the buffer. Usually this is 4 bytes
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *         failure
 */
int cvmx_nand_read_param_page(int chip, uint64_t buffer_address,
			      int buffer_length);

/**
 * Validate the ONFI parameter page and return a pointer to the config values.
 *
 * @param param_page Pointer to the raw NAND data returned after a parameter
 *                   page read.  It will contain at least 3 copies of the
 *                   parameter structure.
 *
 * @return Pointer to a validated paramter page, or NULL if one couldn't be
 *         found.
 */
cvmx_nand_onfi_param_page_t *
cvmx_nand_onfi_process(cvmx_nand_onfi_param_page_t param_page[3]);

/**
 * Get the status of the NAND flash
 *
 * @param chip   Chip select for NAND flash
 *
 * @return NAND status or a negative cvmx_nand_status_t error code on failure
 */
static int cvmx_nand_get_status(int chip);

/**
 * Gets the specified feature number
 *
 * @param chip     Chip select for NAND flash
 * @param feat_num Feature number to get
 * @param feature  P1 - P4 of the feature data
 *
 * @return cvmx_nand_status_t error code
 */
cvmx_nand_status_t cvmx_nand_get_feature(int chip, uint8_t feat_num,
					 uint8_t feature[4]);

/**
 * Sets the specified feature number
 *
 * @param chip     Chip select for NAND flash
 * @param feat_num Feature number to get
 * @param feature  P1 - P4 of the feature data
 *
 * @return cvmx_nand_status_t error code
 */
static cvmx_nand_status_t cvmx_nand_set_feature(int chip, uint8_t feat_num,
						const uint8_t feature[4]);

/**
 * Reset the NAND flash
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_reset(int chip);

/**
 * Get the OOB size.
 *
 * @param chip   Chip select for NAND flash
 *
 * @return OOB in bytes or a negative cvmx_nand_status_t error code on failure
 */
static inline int cvmx_nand_get_oob_size(int chip)
{
	return(cvmx_nand_state[chip].oob_size);
}

/**
 * Get the number of pages per NAND block
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Number of pages in each block or a negative cvmx_nand_status_t error
 *         code on failure
 */
static inline int cvmx_nand_get_pages_per_block(int chip)
{
	return(cvmx_nand_state[chip].pages_per_block);
}

/**
 * Get the number of blocks in the NAND flash
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Number of blocks or a negative cvmx_nand_status_t error code
 *	   on failure
 */
static inline int cvmx_nand_get_blocks(int chip)
{
	return(cvmx_nand_state[chip].blocks);
}

/**
 * Sets the buffer to use for low-level NAND read/write operations
 *
 * @param buffer	pointer to buffer
 */
void cvmx_nand_set_buffer(void *buffer);

/**
 * Return the number of pages in an erase block.
 *
 * @param[in] chip	Chip data structure
 *
 * @return Number of pages in an erase block.
 */
static inline int
octeon_nand_get_pages_per_block(const struct octeon_nand_chip *chip)
{
	return chip->pages_per_block;
}

/**
 * Returns if the current block is bad or not.
 * @param[in] chip	chip to check
 *
 * @return 0 if current block is good, otherwise it's bad.
 */
static inline int octeon_nand_block_is_bad(const struct octeon_nand_chip *chip)
{
	unsigned page_in_block = chip->selected_page % chip->pages_per_block;
	if ((page_in_block == 0) || (page_in_block == chip->pages_per_block - 1))
		return (chip->buffers.databuf[chip->writesize] == 0);
	else
		return 0;
}

/**
 * Returns the page size for the specified chip
 *
 * @param[in] chip	NAND chip data structure
 *
 * @return		page size in bytes not including OOB.
 */
static inline int octeon_nand_get_page_size(const struct octeon_nand_chip *chip)
{
	return cvmx_nand_get_page_size(chip->selected_chip);
}

/**
 * Reads an entire page from the current chip into the buffer after performing
 * error correction.
 *
 * @param chip		Chip data structure
 * @param buf		Pointer to buffer to read page into
 * @param oob_required	Currently not used, ignored.
 * @param page		Page number to read
 *
 * @return 0 for success, -1 if ECC failed, -2 if block is bad.
 *
 * NOTE: Bad block checking only occurrs for the first page of each erase block.
 */
int octeon_nand_hw_bch_read_page(struct octeon_nand_chip *chip, uint8_t *buf,
				 int oob_required, int page);
#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /* __CVMX_NAND_H__ */
