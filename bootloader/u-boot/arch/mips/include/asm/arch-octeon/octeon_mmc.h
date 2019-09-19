/***********************license start************************************
 * Copyright (c) 2012 Cavium Inc. (support@cavium.com). All rights
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
 * support@cavium.com
 *
 ***********************license end**************************************/
#ifndef __MMC_H__
#define __MMC_H__

/*
 * Card Command Classes (CCC)
 */
#define CCC_BASIC		(1<<0)	/* (0) Basic protocol functions */
					/* (CMD0,1,2,3,4,7,9,10,12,13,15) */
					/* (and for SPI, CMD58,59) */
#define CCC_STREAM_READ		(1<<1)	/* (1) Stream read commands */
					/* (CMD11) */
#define CCC_BLOCK_READ		(1<<2)	/* (2) Block read commands */
					/* (CMD16,17,18) */
#define CCC_STREAM_WRITE	(1<<3)	/* (3) Stream write commands */
					/* (CMD20) */
#define CCC_BLOCK_WRITE		(1<<4)	/* (4) Block write commands */
					/* (CMD16,24,25,26,27) */
#define CCC_ERASE		(1<<5)	/* (5) Ability to erase blocks */
					/* (CMD32,33,34,35,36,37,38,39) */
#define CCC_WRITE_PROT		(1<<6)	/* (6) Able to write protect blocks */
					/* (CMD28,29,30) */
#define CCC_LOCK_CARD		(1<<7)	/* (7) Able to lock down card */
					/* (CMD16,CMD42) */
#define CCC_APP_SPEC		(1<<8)	/* (8) Application specific */
					/* (CMD55,56,57,ACMD*) */
#define CCC_IO_MODE		(1<<9)	/* (9) I/O mode */
					/* (CMD5,39,40,52,53) */
#define CCC_SWITCH		(1<<10)	/* (10) High speed switch */
					/* (CMD6,34,35,36,37,50) */
					/* (11) Reserved */
					/* (CMD?) */

/*
  NOTE: This was copied from the Linux kernel.

  MMC status in R1, for native mode (SPI bits are different)
  Type
        e : error bit
        s : status bit
        r : detected and set for the actual command response
        x : detected and set during command execution. the host must poll
            the card by sending status command in order to read these bits.
  Clear condition
        a : according to the card state
        b : always related to the previous command. Reception of
            a valid command will clear it (with a delay of one command)
        c : clear by read
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

#define OCTEON_MMC_FLAG_SD_CMD23	1

struct mmc_host {
	struct mmc *mmc;
	uint8_t ext_csd[512];
	int have_ext_csd;
	int bus_id;
	unsigned int sclock;
	int dev_index;
	int sector_mode;
	int max_width;
	int power_gpio;		/** Power/reset GPIO line (usually 8) */
	int power_active_low;	/** Power GPIO active low */
	int cd_gpio;		/** Card detect GPIO */
	int cd_active_low;	/** Card detect GPIO active low */
	int wp_gpio;		/** Write-protect GPIO */
	int wp_active_low;	/** Write-protect active low */
	int clk_period;		/** Clock period */
	int bus_width;		/** Bus width 1, 4 or 8 */
	int power_class;
	int ddr;
	int flags;
	int initialized;
	int cmd_clk_skew;	/** Clock skew for cmd in SCLK */
	int dat_clk_skew;	/** Clock skew for data in SCLK */
	int registered;
	int use_ndf;		/** Use MIO_NDF_DMA.. or MIO_EMM_DMA... */
	int node;		/** OCX node */
	bool non_removable;	/** True if device is not removable */
};

#endif /* __MMC_H__ */
