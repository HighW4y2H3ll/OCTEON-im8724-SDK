/***********************license start************************************
 * Copyright (c) 2011-2012 Cavium, Inc. (support@cavium.com). All rights
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
#ifndef __OCTEON_CF_H__
#define __OCTEON_CF_H__
/**
 *
 * $Id: octeon_cf.h 78679 2012-11-27 08:38:01Z awilliams $
 *
 */

#define BLOCK_QUIRK_IDE_VIA             (1 << 0)
#define BLOCK_QUIRK_IDE_CF              (1 << 1)
#define BLOCK_QUIRK_IDE_4PORT           (1 << 2)
#define BLOCK_QUIRK_IDE_CF_TRUE_IDE     (1 << 3)
#define BLOCK_QUIRK_IDE_CF_TRUE_IDE_DMA (1 << 4)
#define BLOCK_QUIRK_IDE_CF_16BIT_BUS	(1 << 5)

#define CF_DMA_TYPE_NONE    0
#define CF_DMA_TYPE_MDMA    1
#define CF_DMA_TYPE_UDMA    2


typedef struct {
	/* These are really bus based, but we put them here to ease lookup */
	int enabled;
	int chip_sel;
	uint64_t base_addr0;
	uint64_t base_addr1;
	uint64_t base_addr2;
	uint32_t    quirks;  /* bitmask to indicate special handling cases */
	unsigned char dev_on_bus;   /* What position this device is on it's bus */
	unsigned char dma_type;     /* 0 = none, 1 = mdma, 2 = udma */
	unsigned char dma_mode;       /* Which MDMA/UDMA mode in use */
	unsigned char dma_channel;  /* DMA channel number */
	int dma_timing_set;	/* 1 = set, 0 = not set */
	int reset_gpio;		/* -1 if not used */
} octeon_block_dev_desc_t;

/**
 * Returns 1 if the compact flash is present.
 *
 * @param base_addr - register base address for compact flash
 *
 * @return 0 if compact flash at base_addr is not present, 1 if present
 */
int octeon_cf_present(unsigned long base_addr);

/**
 * Registers an IDE device
 *
 * @param if_type - interface type, typically IF_TYPE_IDE
 * @param quirks - quirk flags
 * @param dev_on_bus - device on bus, 0 or 1 (typically 0)
 * @param data_addr - address of data access
 * @param reg_addr - address of IDE registers
 * @param alt_addr - alternate address
 * @param dma_channel - DMA channel number
 * @param reset_gpio - gpio line wired to reset or -1 if none.
 *
 * @return device number or -1 on error
 */
int ide_register_device(int if_type, uint32_t quirks, uint8_t dev_on_bus,
                               uint64_t data_addr, uint64_t reg_addr,
                               uint64_t alt_addr, uint8_t dma_channel,
			       int reset_gpio);

/**
 * Maps the primary chip select to a device number
 *
 * @param cs - chip select to look up
 *
 * @return device number or -1 if not found
 */
int ide_chip_sel_to_devnum(int cs);

/**
 * Reads one or more sectors using DMA
 *
 * @param device - device to read
 * @param blknr - starting block number to read
 * @param blkcnt - number of blocks to read
 * @param[out] buffer - buffer to write to
 *
 * @returns number of blocks read
 */
ulong octeon_ide_read_dma(int device, lbaint_t blknr, ulong blkcnt,
			  ulong *buffer);

/**
 * Writes one or more sectors using DMA
 *
 * @param device - device to write
 * @param blknr - starting block number to write
 * @param blkcnt - number of blocks to write
 * @param[in] buffer - buffer to write from
 *
 * @returns number of blocks read
 */
ulong octeon_ide_write_dma(int device, lbaint_t blknr, ulong blkcnt,
			   const ulong *buffer);

/**
 * Configures DMA timing based on ident IOP data
 * @param device - device to configure
 * @param[in] iop - IOP ident data
 */
void octeon_cf_config_dma_timing(int device, const void *iop);


#endif /* __OCTEON_CF_H__ */