/***********************license start************************************
 * Copyright (c) 2011, 2012 Cavium, Inc. (support@cavium.com).  All rights
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

/**
 *
 * $Id: octeon_cf.c 103895 2014-09-04 05:08:09Z awilliams $
 *
 */
/*
 * Compact Flash Boot Bus Support
 */

#include <common.h>
#include <pci.h>
#include <ata.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_hal.h>
#include <asm/arch/octeon_pci.h>
#include <asm/arch/cvmx-compactflash.h>
#include <asm/arch/octeon_cf.h>
#include <asm/gpio.h>
#include <libata.h>
#include <ide.h>

#define IDE_TIME_OUT	5000	/* 5 sec timeout */

#define IDE_DMA_MAX_BLOCK_CHUNK     256

#ifdef CONFIG_IDE_LED
extern void  ide_led   (uchar led, uchar status);
#else
#define ide_led(a,b)	/* dummy */
#endif

extern block_dev_desc_t
	ide_dev_desc[CONFIG_SYS_IDE_MAXBUS][CONFIG_SYS_IDE_MAXDEVICE];

static octeon_block_dev_desc_t
	oct_block_dev_desc[CONFIG_SYS_IDE_MAXBUS][CONFIG_SYS_IDE_MAXDEVICE];

static int ide_dev_desc_register_offset;

extern uchar ide_wait(int dev, ulong t);

extern uint64_t octeon_get_io_clock_rate(void);

static inline octeon_block_dev_desc_t *get_oct_dev_data(block_dev_desc_t *dev)
{
	return (octeon_block_dev_desc_t *)dev->priv;
}

/* From Linux libata.h */
struct ata_timing {
	unsigned short mode;		/* ATA mode */
	unsigned short setup;		/* t1 */
	unsigned short act8b;		/* t2 for 8-bit I/O */
	unsigned short rec8b;		/* t2i for 8-bit I/O */
	unsigned short cyc8b;		/* t0 for 8-bit I/O */
	unsigned short active;		/* t2 or tD */
	unsigned short recover;		/* t2i or tK */
	unsigned short dmack_hold;	/* tj */
	unsigned short cycle;		/* t0 */
	unsigned short udma;		/* t2CYCTYP/2 */
};

/* From Linux libata-core.c */
static const struct ata_timing ata_timing[] = {
/*	{ XFER_PIO_SLOW, 120, 290, 240, 960, 290, 240, 0,  960,   0 }, */
	{ XFER_PIO_0,     70, 290, 240, 600, 165, 150, 0,  600,   0 },
	{ XFER_PIO_1,     50, 290,  93, 383, 125, 100, 0,  383,   0 },
	{ XFER_PIO_2,     30, 290,  40, 330, 100,  90, 0,  240,   0 },
	{ XFER_PIO_3,     30,  80,  70, 180,  80,  70, 0,  180,   0 },
	{ XFER_PIO_4,     25,  70,  25, 120,  70,  25, 0,  120,   0 },
	{ XFER_PIO_5,     15,  65,  25, 100,  65,  25, 0,  100,   0 },
	{ XFER_PIO_6,     10,  55,  20,  80,  55,  20, 0,   80,   0 },

	{ XFER_SW_DMA_0, 120,   0,   0,   0, 480, 480, 50, 960,   0 },
	{ XFER_SW_DMA_1,  90,   0,   0,   0, 240, 240, 30, 480,   0 },
	{ XFER_SW_DMA_2,  60,   0,   0,   0, 120, 120, 20, 240,   0 },

	{ XFER_MW_DMA_0,  60,   0,   0,   0, 215, 215, 20, 480,   0 },
	{ XFER_MW_DMA_1,  45,   0,   0,   0,  80,  50, 5,  150,   0 },
	{ XFER_MW_DMA_2,  25,   0,   0,   0,  70,  25, 5,  120,   0 },
	{ XFER_MW_DMA_3,  25,   0,   0,   0,  65,  25, 5,  100,   0 },
	{ XFER_MW_DMA_4,  25,   0,   0,   0,  55,  20, 5,   80,   0 },
#if 0   /* OCTEON doesn't support UDMA mode */

/*	{ XFER_UDMA_SLOW,  0,   0,   0,   0,   0,   0, 0,    0, 150 }, */
	{ XFER_UDMA_0,     0,   0,   0,   0,   0,   0, 0,    0, 120 },
	{ XFER_UDMA_1,     0,   0,   0,   0,   0,   0, 0,    0,  80 },
	{ XFER_UDMA_2,     0,   0,   0,   0,   0,   0, 0,    0,  60 },
	{ XFER_UDMA_3,     0,   0,   0,   0,   0,   0, 0,    0,  45 },
	{ XFER_UDMA_4,     0,   0,   0,   0,   0,   0, 0,    0,  30 },
	{ XFER_UDMA_5,     0,   0,   0,   0,   0,   0, 0,    0,  20 },
	{ XFER_UDMA_6,     0,   0,   0,   0,   0,   0, 0,    0,  15 },
#endif
	{ 0xFF }
};

static uint64_t get_ide_port(int dev, int port_arg)
{
	uint64_t port_u64 = port_arg;
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));

	/* In true ide mode, the bus is wired up in 16 bit mode, but Octeon's
	 * addr0 line is not connected.  This allows us to access all of the
	 * 8 bit registers using the low byte lane of the bus.  We need to
	 * shift the register address left by one bit to get the register
	 * we want.
	 */
	if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE) {
		int __attribute__((unused)) orig_port = port_u64;
		port_u64 = (port_u64 & ~0xff) | ((port_u64 & 0x7F) << 1);
		debug("%s: translated port from 0x%x to 0x%llx\n",
                      __func__, orig_port, port_u64);
	}

	if (port_u64 & ATA_IO_DATA_FLAG)
		port_u64 = (port_u64 & 0xFF) + oct_dev->base_addr0;
	else if (port_u64 & ATA_IO_REG_FLAG)
		port_u64 = (port_u64 & 0xFF) + oct_dev->base_addr1;
	else if (port_u64 & ATA_IO_ALT_FLAG)
		port_u64 = (port_u64 & 0xFF) + oct_dev->base_addr2;

	debug("%s: dev %d port_arg 0x%x) = 0x%llx\n",
	      __func__, dev, port_arg, port_u64);

	return port_u64;
}

void ide_outb(int dev, int port, unsigned char val)
{
	uint64_t port_u64 = get_ide_port(dev, port);
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));

	if (!oct_dev->enabled) {
		debug("%s: device %d not enabled\n", __func__, dev);
		return;
	}

	debug("%s: port 0x%x port_u64 0x%llx val 0x%x\n",
	      __func__, port, port_u64, val);

	if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_16BIT_BUS) {
		/* Boot bus CF needs special handling for 16 bit boot bus */
		if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE) {
			debug("%s: 16-bit True IDE\n", __func__);
			/* Only low byte is valid on the bus */
			cvmx_write64_uint16(port_u64, val);
		} else if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF) {
			ushort data, data1;
			debug("%s: 16-bit IDE CF\n", __func__);

			if (port == ATA_DEV_HD ) {
				/* If writing to byte in same short word as the
				 * command byte, we must send a dummy command as
				 * part of the write.
				 * Compact flash expects to be byte addressable,
				 * but in 16 bit mode Octeon only supports 16
				 * bit operations.
				 */
				data = 0x9500;
			} else {
				data = cvmx_read64_uint16(port_u64 & ~0x1ull);
				data = (port & 1) ?
						(data & 0xff) : (data & 0xFF00);
			}
			data1 = val;
			data1 = (port & 1) ? data1 << 8 : data1 & 0xff;
			data = data | data1;
			cvmx_write64_uint16(port_u64 & ~1ull, data);

			/* Wait for dummy command to complete */
			if (port == ATA_DEV_HD )
				/* can't take over 500 ms */
				ide_wait(dev, IDE_TIME_OUT);
		} else {
			debug("%s: Non-CF 16-bit IDE\n", __func__);
			cvmx_write64_uint8(port_u64, val);
		}
	} else {
		debug("%s: 8-bit bus\n", __func__);
		cvmx_write64_uint8(port_u64, val);
	}
}

unsigned char ide_inb(int dev, int port)
{
	uint64_t port_u64 = get_ide_port(dev, port);
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));
	unsigned char retval;

	if (!oct_dev->enabled) {
		debug("ide_inb: device %d not enabled\n", dev);
		return 0xff;
	}

	if ((oct_dev->quirks & BLOCK_QUIRK_IDE_CF_16BIT_BUS) &&
	    (oct_dev->quirks & BLOCK_QUIRK_IDE_CF))
		port_u64 ^= 1;

	retval = cvmx_read64_uint8(port_u64);
	debug("%s: dev %d port 0x%x port_u64 0x%llx retval 0x%x\n",
	      __func__, dev, port, port_u64, (unsigned int)retval);

	return retval;
}

int ide_chip_sel_to_devnum(int cs)
{
	int i;
	for (i = 0; i < ide_dev_desc_register_offset; i++)
		if (oct_block_dev_desc[i][0].chip_sel == cs) {
			debug("%s: Found IDE device %d for chip select %d\n",
			      __func__, i, cs);
			return i;
		}
	debug("%s: No IDE device found for chip select %d\n", __func__, cs);
	return -1;
}

/* Registers an ide (or ide-like: CF, SATA) device, returns the dev number
 * assigned or -1 on error.
 *  This needs to be called for all possibled devices on a bus at the time
 * the bus is detected.
 */
int ide_register_device(int if_type, uint32_t quirks, uint8_t dev_on_bus,
			uint64_t data_addr, uint64_t reg_addr,
			uint64_t alt_addr, uint8_t dma_channel, int reset_gpio)
{
	octeon_block_dev_desc_t *oct_desc;
	block_dev_desc_t *ide_desc;

	if (ide_dev_desc_register_offset >= CONFIG_SYS_IDE_MAXBUS) {
		printf("ERROR: Unable to register IDE device, "
		       "IDE descriptor list full!\n");
		return -1;
	}

	oct_desc = &oct_block_dev_desc[ide_dev_desc_register_offset][dev_on_bus];
	ide_desc = &ide_dev_desc[ide_dev_desc_register_offset][dev_on_bus];

	ide_desc->priv = (void *)oct_desc;
	ide_desc->if_type = if_type;
	oct_desc->base_addr0 = data_addr;
	oct_desc->base_addr1 = reg_addr;
	oct_desc->base_addr2 = alt_addr;
	oct_desc->quirks = quirks;
	oct_desc->dev_on_bus = dev_on_bus;
	oct_desc->dma_channel = dma_channel;
	oct_desc->reset_gpio = reset_gpio;

	debug("%s: dev %d data_addr 0x%llx reg 0x%llx, reset: %d\n",
	      __func__, ide_dev_desc_register_offset, data_addr, reg_addr,
	      reset_gpio);

	ide_desc->type = DEV_TYPE_UNKNOWN;
	ide_desc->dev = IDE_DEVICE(ide_dev_desc_register_offset, 0);
	ide_desc->part_type = PART_TYPE_UNKNOWN;
	ide_desc->blksz = 0;
	ide_desc->lba = 0;
	ide_desc->block_read = ide_read;
	ide_desc->block_write = ide_write;
	oct_desc->enabled = 1;
	printf("Registered IDE device %d from IDE bus:dev %d:%d\n",
	       IDE_DEVICE(ide_dev_desc_register_offset, dev_on_bus),
	       ide_dev_desc_register_offset, dev_on_bus);

	return IDE_DEVICE(ide_dev_desc_register_offset++, dev_on_bus);
}

void __ide_set_reset(int idereset)
{
	int busno, devno;
	octeon_block_dev_desc_t *ide_desc = NULL;

	for (busno = 0; busno < CONFIG_SYS_IDE_MAXBUS; busno++)
		for (devno = 0; devno < CONFIG_SYS_IDE_MAXDEVICE; devno++)
			if (ide_dev_desc[busno][devno].priv != NULL) {
				ide_desc = ide_dev_desc[busno][devno].priv;
				if (ide_desc->reset_gpio >= 0) {
					gpio_direction_output(ide_desc->reset_gpio,
							      !idereset);
					debug("%s: gpio: %d\n",
					      __func__, ide_desc->reset_gpio);
				}
			}
}
void ide_set_reset(int idereset) __attribute__((weak, alias("__ide_set_reset")));

/* Swaps the upper and lower 16-bits of data */
static inline uint32_t u32_short_swap16(uint32_t data)
{
	uint32_t tmp, mask;
	mask = 0xff00ff00;
	tmp = (data & mask) >> 8;
	mask >>= 8;
	tmp |= ((data & mask) << 8);
	return tmp;
}

void ide_input_swap_data(int dev, ulong *sect_buf, int words)
{
	uint64_t port_u64 = get_ide_port(dev, ATA_DATA_REG);
	ulong tmp;
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));

	debug("%s: dev %d sect_buf %p words %d port 0x%llx\n",
	      __func__, dev, sect_buf, words, port_u64);

	if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_16BIT_BUS) {
		if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE)
			while (words--) {
				tmp = cvmx_read64_uint16(port_u64) << 16;
				tmp |= cvmx_read64_uint16(port_u64);
				*sect_buf++ = tmp;
			}
		else if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF)
			while (words--)
				*sect_buf++ = (cvmx_read64_uint32(port_u64));
		else
			while (words--)
				*sect_buf++ = u32_short_swap16(
						cvmx_read64_uint32(port_u64));
	} else
		while (words--)
			*sect_buf++ =
				u32_short_swap16(cvmx_read64_uint32(port_u64));
}

void ide_output_data(int dev, const ulong *sect_buf, int words)
{
	uint64_t port_u64;
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));
	ulong tmp;

	port_u64 = get_ide_port(dev, ATA_DATA_REG);

	debug("%s: dev %d sect_buf %p words %d port_u64 0x%llx\n",
	      __func__, dev, sect_buf, words, port_u64);
	if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_16BIT_BUS) {
		if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF) {
			/* outsw(port_u64, sect_buf, words<<1); */
			while (words--) {
				tmp = u32_short_swap16(*sect_buf++);
				cvmx_write64_uint16(port_u64, tmp >> 16);
				cvmx_write64_uint16(port_u64, tmp);
			}
		} else
			while (words--)
				cvmx_write64_uint32(port_u64, *sect_buf++);
	} else
		while (words--)
			cvmx_write_csr(port_u64, *sect_buf++);
}

void ide_input_data(int dev, ulong *sect_buf, int words)
{
	uint64_t port_u64 = get_ide_port(dev, ATA_DATA_REG);
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(dev));
	uint32_t tmp;

	debug("%s: dev %d sect_buf %p words %d port 0x%llx\n",
	      __func__, dev, sect_buf, words, port_u64);

	if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_16BIT_BUS) {
		if (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE)
			while (words--) {
				tmp = cvmx_read64_uint16(port_u64) << 16;
				tmp |= cvmx_read64_uint16(port_u64);
				*sect_buf++ = u32_short_swap16(tmp);
			}
		else
			while (words--)
				*sect_buf++ = u32_short_swap16(
						cvmx_read64_uint32(port_u64));
	} else
		while (words--)
			*sect_buf++ = cvmx_read64_uint32(port_u64);
}

void octeon_cf_config_dma_timing(int device, const void *iop)
{
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(device));

	if ((oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE) &&
	    (oct_dev->quirks & BLOCK_QUIRK_IDE_CF_TRUE_IDE_DMA) &&
	    (OCTEON_IS_OCTEONPLUS() || OCTEON_IS_OCTEON2())) {

		int mwdma_mode;
		uint64_t tim_val;
		uint64_t dma_limit_mask = 0;
		/* We need to make sure that we're not located above the DMA
		 * address limit or else DMA will not work.
		 */
		if (OCTEON_IS_OCTEONPLUS())
			dma_limit_mask = ~((1ull << 35) - 1);
		else if (OCTEON_IS_OCTEON2())
			dma_limit_mask = ~((1ull << 36) - 1);
		else if (OCTEON_IS_OCTEON3())
			dma_limit_mask = ~((1ull << 42) - 1);

		if (!(cvmx_ptr_to_phys(&tim_val) & dma_limit_mask)) {
			tim_val = cvmx_compactflash_generate_dma_tim(8,
								     (uint16_t *)iop,
								     &mwdma_mode,
								     oct_dev->dma_channel);
			oct_dev->dma_mode = mwdma_mode;
			oct_dev->dma_type = CF_DMA_TYPE_MDMA;
			debug("Using DMA for CF (mode %d)\n",
			      oct_dev->dma_mode);
			cvmx_write_csr(CVMX_MIO_BOOT_DMA_TIMX(oct_dev->dma_channel),
				       tim_val);
		} else {
			debug("Not using DMA for CF\n");
			oct_dev->quirks &= ~BLOCK_QUIRK_IDE_CF_TRUE_IDE_DMA;
			oct_dev->dma_type = CF_DMA_TYPE_NONE;
		}
	} else {
		debug("No DMA quirks\n");
		oct_dev->dma_mode = 0;
		oct_dev->dma_type = CF_DMA_TYPE_NONE;
	}
}


ulong octeon_ide_read_dma(int device, lbaint_t blknr, ulong blkcnt, ulong *buffer)
{
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(device));
	cvmx_mio_boot_dma_cfgx_t dma_cfg;
	cvmx_mio_boot_dma_intx_t dma_int;
	cvmx_mio_boot_dma_adrx_t dma_adr;
	ulong n = 0;
	unsigned char c;
#ifdef CONFIG_LBA48
	unsigned char lba48 = 0;

	if (blknr & 0x0000fffff0000000ULL)
		/* More than 28 bits used, use 48bit mode */
		lba48 = 1;
#endif
	debug("%s: dev %d start %llx, blocks %lX buffer at 0x%p\n",
	      __func__, device, blknr, blkcnt, buffer);

	cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel), 0);

	while (blkcnt > 0) {
		int blocks;
		int to;

		c = ide_wait(device, IDE_TIME_OUT);

		if (c & ATA_STAT_BUSY) {
			printf("IDE read: device %d not idle\n", device);
			break;
		}
		/* Sleep of 20 required here or data is corrupted.... */
		udelay(20);

		if (blkcnt >= IDE_DMA_MAX_BLOCK_CHUNK)
			blocks = IDE_DMA_MAX_BLOCK_CHUNK;
		else
			blocks = blkcnt;

		/* Clear DMA interrupt */
		dma_int.u64 = 0;
		dma_int.s.done = 1;
		cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(oct_dev->dma_channel),
			       dma_int.u64);

		dma_cfg.u64 = 0;
		dma_cfg.s.en = 1;
		dma_cfg.s.swap8 = 1;
		/* Note that 78xx moved the address to a new register to handle
		 * the larger address space.
		 */
		if (OCTEON_IS_OCTEON3()) {
			dma_adr.u64 = 0;
			dma_adr.s.adr = cvmx_ptr_to_phys(buffer);
			cvmx_write_csr(CVMX_MIO_BOOT_DMA_ADRX(oct_dev->dma_channel),
				       dma_adr.u64);
		} else {
			dma_cfg.s.adr = cvmx_ptr_to_phys(buffer);
		}
		/* TODO: fix use of addresses from bootmem_alloc */
		/* printf("ptr: %p, address: 0x%llx\n", buffer,
		 *        (unsigned long long)dma_cfg.s.adr);
		 */
		dma_cfg.s.size = blocks * (512 / 2) - 1;
		debug("%s: DMA channel: %d, address: 0x%llx, size: 0x%llx\n",
		      __func__, oct_dev->dma_channel, (u64)dma_cfg.s.adr,
		      (u64)dma_cfg.s.size);
		cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel),
			       dma_cfg.u64);

#ifdef CONFIG_LBA48
		if (lba48) {
			/* write high bits */
			ide_outb(device, ATA_SECT_CNT, 0);
			ide_outb(device, ATA_LBA_LOW, (blknr >> 24) & 0xFF);
#ifdef CONFIG_SYS_64BIT_LBA
			ide_outb(device, ATA_LBA_MID, (blknr >> 32) & 0xFF);
			ide_outb(device, ATA_LBA_HIGH, (blknr >> 40) & 0xFF);
#else
			ide_outb(device, ATA_LBA_MID, 0);
			ide_outb(device, ATA_LBA_HIGH, 0);
#endif
		}
#endif
		ide_outb(device, ATA_SECT_CNT, blocks);
		ide_outb(device, ATA_LBA_LOW,  (blknr >>  0) & 0xFF);
		ide_outb(device, ATA_LBA_MID,  (blknr >>  8) & 0xFF);
		ide_outb(device, ATA_LBA_HIGH, (blknr >> 16) & 0xFF);

#ifdef CONFIG_LBA48
		if (lba48) {
			ide_outb(device, ATA_DEV_HD,
				 ATA_LBA | ATA_DEVICE(device));
			ide_outb(device, ATA_COMMAND,  ATA_CMD_RD_DMA_EXT);
		} else
#endif
		{
			ide_outb(device, ATA_DEV_HD,
				 ATA_LBA | ATA_DEVICE(device) | ((blknr >> 24) & 0xF) );
			ide_outb(device, ATA_COMMAND, ATA_CMD_RD_DMA);
		}
		/* Poll for DMA completion */
		to = 10000 + 1000 * blkcnt;
		while (--to) {
			dma_int.u64 = cvmx_read_csr(CVMX_MIO_BOOT_DMA_INTX(
							oct_dev->dma_channel));
			if (dma_int.s.done)
				break;
			udelay(1);
		}
		debug("dma_cfg: 0x%llx, dma_int: 0x%llx, to: %d\n",
		      dma_cfg.u64, dma_int.u64, to);
		if (!dma_int.s.done)
			printf("ERROR: timeout waiting for DMA completion\n");
#ifdef DEBUG
		print_buffer(blknr * 512, buffer, 1, blocks * 512, 0);
#endif
		cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel), 0ull);

		n += blocks;
		blknr += blocks;
		buffer += ATA_SECTORWORDS * blocks;
		blkcnt -= blocks;
	}

	(void)ide_inb(device, ATA_STATUS);	/* clear IRQ */

	/* This seems to be required to get some CF devices to not interfere
	 * with the boot bus on reset.....
	 */
	ide_outb(device, ATA_COMMAND,  ATA_CMD_IDLE);
	udelay(20);
	debug("%s: Read %lu blocks\n", __func__, n);
#ifdef CONFIG_IDE_LED
	ide_led(DEVICE_LED(device), 0);
#endif
	return n;
}

ulong octeon_ide_write_dma(int device, lbaint_t blknr, ulong blkcnt,
			   const ulong *buffer)
{
	octeon_block_dev_desc_t *oct_dev = get_oct_dev_data(ide_get_dev(device));
	cvmx_mio_boot_dma_cfgx_t dma_cfg;
	cvmx_mio_boot_dma_adrx_t dma_adr;
	cvmx_mio_boot_dma_intx_t dma_int;
	ulong n = 0;
	unsigned char c;
#ifdef CONFIG_LBA48
	unsigned char lba48 = 0;

	if (blknr & 0x0000fffff0000000ULL)
		/* More than 28 bits used, use 48bit mode */
		lba48 = 1;
#endif

	debug("%s: dev %d start %llx, blocks %lX buffer at %p\n",
	      __func__, device, blknr, blkcnt, buffer);

	/* Clear any interrupts */
	dma_int.u64 = cvmx_read_csr(CVMX_MIO_BOOT_DMA_INTX(oct_dev->dma_channel));
	cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(oct_dev->dma_channel), dma_int.u64);

	while (blkcnt > 0) {
		int blocks;
		int to;

		c = ide_wait(device, IDE_TIME_OUT);

		if (c & ATA_STAT_BUSY) {
			printf("IDE write: device %d not ready\n", device);
			break;
		}

		/* Clear DMA interrupt */
		dma_int.u64 = 0;
		dma_int.s.done = 1;
		cvmx_write_csr(CVMX_MIO_BOOT_DMA_INTX(oct_dev->dma_channel),
			       dma_int.u64);

		/* Sleep of 20 required here or data is corrupted... */
		udelay(20);

		if (blkcnt >= IDE_DMA_MAX_BLOCK_CHUNK)
			blocks = IDE_DMA_MAX_BLOCK_CHUNK;
		else
			blocks = blkcnt;

		dma_cfg.u64 = 0;
		dma_cfg.s.en = 1;
		dma_cfg.s.swap8 = 1;
		dma_cfg.s.rw = 1;
		if (OCTEON_IS_OCTEON3()) {
			dma_adr.u64 = 0;
			dma_adr.s.adr = cvmx_ptr_to_phys((void *)buffer);
			cvmx_write_csr(CVMX_MIO_BOOT_DMA_ADRX(oct_dev->dma_channel),
				       dma_adr.u64);
		} else {
			dma_cfg.s.adr = cvmx_ptr_to_phys((void *)buffer);
		}

		dma_cfg.s.size = blocks * (512 / 2) - 1;
#ifdef CONFIG_LBA48
		if (lba48) {
			/* Write high bits */
			ide_outb(device, ATA_SECT_CNT, 0);
			ide_outb(device, ATA_LBA_LOW, (blknr >> 24) & 0xFF);
#ifdef CONFIG_SYS_64BIT_LBA
			ide_outb(device, ATA_LBA_MID, (blknr >> 32) & 0xFF);
			ide_outb(device, ATA_LBA_HIGH, (blknr >> 40) & 0xFF);
#else
			ide_outb(device, ATA_LBA_MID, 0);
			ide_outb(device, ATA_LBA_HIGH, 0);
#endif
		}
#endif
		ide_outb(device, ATA_SECT_CNT, blocks);
		ide_outb(device, ATA_LBA_LOW,  (blknr >>  0) & 0xFF);
		ide_outb(device, ATA_LBA_MID,  (blknr >>  8) & 0xFF);
		ide_outb(device, ATA_LBA_HIGH, (blknr >> 16) & 0xFF);

#ifdef CONFIG_LBA48
		if (lba48) {
			ide_outb(device, ATA_DEV_HD,
				 ATA_LBA | ATA_DEVICE(device));
			ide_outb(device, ATA_COMMAND, ATA_CMD_WR_DMA_EXT);
		} else
#endif
		{
			ide_outb(device, ATA_DEV_HD, ATA_LBA		|
						     ATA_DEVICE(device)	|
						     ((blknr >> 24) & 0xF));
			ide_outb(device, ATA_COMMAND, ATA_CMD_WR_DMA);
		}
		udelay(50);
		c = ide_wait(device, IDE_TIME_OUT * 2);	/* Can't take over 500ms */
		if ((c & (ATA_STAT_DRQ|ATA_STAT_BUSY|ATA_STAT_ERR)) != ATA_STAT_DRQ) {
#if defined(CONFIG_SYS_64BIT_LBA)
			printf("Error (no IRQ) dev %d blk %Ld: status 0x%02x\n",
			       device, blknr, c);
#else
			printf("Error (no IRQ) dev %d blk %ld: status 0x%02x\n",
			       device, (ulong)blknr, c);
#endif
			printf("%s: dma_cfg%d: 0x%llx, dma_int: 0x%llx, dma_tim: 0x%llx\n",
			       __func__,
			       oct_dev->dma_channel,
			       cvmx_read_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel)),
			       cvmx_read_csr(CVMX_MIO_BOOT_DMA_INTX(oct_dev->dma_channel)),
			       cvmx_read_csr(CVMX_MIO_BOOT_DMA_TIMX(oct_dev->dma_channel)));
			goto WR_OUT;
		}
		cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel),
			       dma_cfg.u64);

		/* Poll for DMA completion */
		to = 100000 + 5000 * blkcnt;
		while (--to) {
			dma_int.u64 = cvmx_read_csr(CVMX_MIO_BOOT_DMA_INTX(
							oct_dev->dma_channel));
			if (dma_int.s.done)
				break;
			udelay(1);
		}
		if (!dma_int.s.done)
			printf("ERROR: timeout waiting for DMA completion\n");
		(void)ide_inb(device, ATA_STATUS);	/* clear IRQ */
		debug("dma_cfg: 0x%llx, dma_int: 0x%llx, to: %d\n",
		      dma_cfg.u64, dma_int.u64, to);

		cvmx_write_csr(CVMX_MIO_BOOT_DMA_CFGX(oct_dev->dma_channel),
			       0ull);

		n += blocks;
		blknr += blocks;
		buffer += ATA_SECTORWORDS * blocks;
		blkcnt -= blocks;
	}

WR_OUT:
	/* This seems to be required to get some CF devices to not interfere
	 * with the boot bus on reset.....
	 */
	ide_outb(device, ATA_COMMAND,  ATA_CMD_IDLE);
#ifdef CONFIG_IDE_LED
	ide_led(DEVICE_LED(device), 0);
#endif

	return n;

}
