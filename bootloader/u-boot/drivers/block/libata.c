/*
 * Copyright (C) 2008 Freescale Semiconductor, Inc.
 *		Dave Liu <daveliu@freescale.com>
 *		port from the libata of linux kernel
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <libata.h>
#include <linux/dma-direction.h>

#ifndef UINT_MAX
# define UINT_MAX	(~0U)
#endif

/* debounce timing parameters in msecs { interval, duration, timeout } */
const unsigned long sata_deb_timing_normal[]		= {   5,  100, 2000 };
const unsigned long sata_deb_timing_hotplug[]		= {  25,  500, 2000 };
const unsigned long sata_deb_timing_long[]		= { 100, 2000, 5000 };

const struct ata_port_operations ata_base_port_ops = {
	.prereset		= ata_std_prereset,
	.postreset		= ata_std_postreset,
};

const struct ata_port_operations sata_port_ops = {
	.inherits		= &ata_base_port_ops,
	.hardreset		= sata_std_hardreset,
};

int ata_print_id = 0;

static unsigned int ata_dev_init_params(struct ata_device *dev,
					u16 heads, u16 sectors);

u32 __ata_readl(void *addr)
{
	u32 val = readl(addr);
	return val;
}
u32 ata_readl(void *addr) __attribute__((weak, alias("__ata_readl")));

void __ata_writel(u32 val, void *addr)
{
	writel(val, addr);
}
u32 ata_writel(u32 val, void *addr) __attribute__((weak, alias("__ata_writel")));

static int ata_sstatus_online(u32 sstatus)
{
	return (sstatus & 0xf) == 0x3;
}

/**
 *	ata_link_next - link iteration helper
 *	@link: the previous link, NULL to start
 *	@ap: ATA port containing links to iterate
 *	@mode: iteration mode, one of ATA_LITER_*
 *
 *	LOCKING:
 *	Host lock or EH context.
 *
 *	RETURNS:
 *	Pointer to the next link.
 */
struct ata_link *ata_link_next(struct ata_link *link, struct ata_port *ap,
			       enum ata_link_iter_mode mode)
{
	BUG_ON(mode != ATA_LITER_EDGE &&
	       mode != ATA_LITER_PMP_FIRST && mode != ATA_LITER_HOST_FIRST);

	debug("%s(%p, %d, %d)\n", __func__,
	      link, ap->port_no, mode);
	/* NULL link indicates start of iteration */
	if (!link) {
		debug("  Start of iteration, mode: %d\n", mode);
		switch (mode) {
		case ATA_LITER_EDGE:
		case ATA_LITER_PMP_FIRST:
			if (sata_pmp_attached(ap)) {
				debug("  pmp attached to port %d, link: %p\n",
				      ap->port_no, ap->pmp_link);
				return ap->pmp_link;
			}
			/* fall through */
		case ATA_LITER_HOST_FIRST:
			debug("  host first\n");
			return &ap->link;
		}
	}

	/* we just iterated over the host link, what's next? */
	if (link == &ap->link) {
		debug("  Past host link, port: %d, mode: %d\n",
		      ap->port_no, mode);
		switch (mode) {
		case ATA_LITER_HOST_FIRST:
			if (sata_pmp_attached(ap)) {
				debug("  pmp attached to port %d\n",
				      ap->port_no);
				return ap->pmp_link;
			}
			/* fall through */
		case ATA_LITER_PMP_FIRST:
			if (unlikely(ap->slave_link)) {
				debug("  slave link\n");
				return ap->slave_link;
			}
			/* fall through */
		case ATA_LITER_EDGE:
			return NULL;
		}
	}

	/* slave_link excludes PMP */
	if (unlikely(link == ap->slave_link)) {
		debug(" slave link\n");
		return NULL;
	}

	/* we were over a PMP link */
	if (++link < ap->pmp_link + ap->nr_pmp_links) {
		debug("  over a PMP link\n");
		return link;
	}

	if (mode == ATA_LITER_PMP_FIRST) {
		debug("  ATA_LITER_PMP_FIRST\n");
		return &ap->link;
	}

	return NULL;
}

/**
 *	ata_dev_next - device iteration helper
 *	@dev: the previous device, NULL to start
 *	@link: ATA link containing devices to iterate
 *	@mode: iteration mode, one of ATA_DITER_*
 *
 *	LOCKING:
 *	Host lock or EH context.
 *
 *	RETURNS:
 *	Pointer to the next device.
 */
struct ata_device *ata_dev_next(struct ata_device *dev, struct ata_link *link,
				enum ata_dev_iter_mode mode)
{
	BUG_ON(mode != ATA_DITER_ENABLED && mode != ATA_DITER_ENABLED_REVERSE &&
	       mode != ATA_DITER_ALL && mode != ATA_DITER_ALL_REVERSE);

	debug("%s(%p, %p, %d)\n", __func__, dev, link, mode);
	/* NULL dev indicates start of iteration */
	if (!dev)
		switch (mode) {
		case ATA_DITER_ENABLED:
		case ATA_DITER_ALL:
			dev = link->device;
			goto check;
		case ATA_DITER_ENABLED_REVERSE:
		case ATA_DITER_ALL_REVERSE:
			dev = link->device + ata_link_max_devices(link) - 1;
			goto check;
		}

 next:
	/* move to the next one */
	switch (mode) {
	case ATA_DITER_ENABLED:
	case ATA_DITER_ALL:
		if (++dev < link->device + ata_link_max_devices(link))
			goto check;
		return NULL;
	case ATA_DITER_ENABLED_REVERSE:
	case ATA_DITER_ALL_REVERSE:
		if (--dev >= link->device)
			goto check;
		return NULL;
	}

 check:
	if ((mode == ATA_DITER_ENABLED || mode == ATA_DITER_ENABLED_REVERSE) &&
	    !ata_dev_enabled(dev))
		goto next;
	debug("%s: Returning %p\n", __func__, dev);
	return dev;
}

/**
 *	ata_dev_phys_link - find physical link for a device
 *	@dev: ATA device to look up physical link for
 *
 *	Look up physical link which @dev is attached to.  Note that
 *	this is different from @dev->link only when @dev is on slave
 *	link.  For all other cases, it's the same as @dev->link.
 *
 *	RETURNS:
 *	Pointer to the found physical link.
 */
struct ata_link *ata_dev_phys_link(struct ata_device *dev)
{
	struct ata_port *ap = dev->link->ap;

	debug("%s(%p): ap: %p, link: %p, slave_link: %p, devno: %d\n",
	      __func__, dev, ap, dev->link, ap->slave_link, dev->devno);
	if (!ap->slave_link)
		return dev->link;
	if (!dev->devno)
		return &ap->link;
	return ap->slave_link;
}

/**
 *	ata_tf_to_fis - Convert ATA taskfile to SATA FIS structure
 *	@tf: Taskfile to convert
 *	@pmp: Port multiplier port
 *	@is_cmd: This FIS is for command
 *	@fis: Buffer into which data will output
 *
 *	Converts a standard ATA taskfile to a Serial ATA
 *	FIS structure (Register - Host to Device).
 */
void ata_tf_to_fis(const struct ata_taskfile *tf, u8 pmp, int is_cmd, u8 *fis)
{
	fis[0] = 0x27;			/* Register - Host to Device FIS */
	fis[1] = pmp & 0xf;		/* Port multiplier number*/
	if (is_cmd)
		fis[1] |= (1 << 7);	/* bit 7 indicates Command FIS */

	fis[2] = tf->command;
	fis[3] = tf->feature;

	fis[4] = tf->lbal;
	fis[5] = tf->lbam;
	fis[6] = tf->lbah;
	fis[7] = tf->device;

	fis[8] = tf->hob_lbal;
	fis[9] = tf->hob_lbam;
	fis[10] = tf->hob_lbah;
	fis[11] = tf->hob_feature;

	fis[12] = tf->nsect;
	fis[13] = tf->hob_nsect;
	fis[14] = 0;
	fis[15] = tf->ctl;

	fis[16] = 0;
	fis[17] = 0;
	fis[18] = 0;
	fis[19] = 0;
	debug("%s: %02x %02x %02x %02x  %02x %02x %02x %02x - %02x %02x %02x %02x %02x %02x %02x %02x\n",
	      __func__,
	      fis[0], fis[1], fis[2], fis[3], fis[4], fis[5], fis[6], fis[7],
	      fis[8], fis[9], fis[10], fis[11], fis[12], fis[13], fis[14], fis[15]);
}

/**
 *	ata_tf_from_fis - Convert SATA FIS to ATA taskfile
 *	@fis: Buffer from which data will be input
 *	@tf: Taskfile to output
 *
 *	Converts a serial ATA FIS structure to a standard ATA taskfile.
 */

void ata_tf_from_fis(const u8 *fis, struct ata_taskfile *tf)
{
	tf->command	= fis[2];	/* status */
	tf->feature	= fis[3];	/* error */

	tf->lbal	= fis[4];
	tf->lbam	= fis[5];
	tf->lbah	= fis[6];
	tf->device	= fis[7];

	tf->hob_lbal	= fis[8];
	tf->hob_lbam	= fis[9];
	tf->hob_lbah	= fis[10];

	tf->nsect	= fis[12];
	tf->hob_nsect	= fis[13];
	debug("%s: cmd: %02x, feat: %02x, lba l: %02x, m: %02x, h: %02x, dev: %02x\n"
	      "h lbal: %02x, h lbam: %02x, h lbah %02x, n_sect: %02x, h n_sect: %02x\n",
	      __func__, tf->command, tf->feature, tf->lbal, tf->lbam, tf->lbah,
	      tf->device, tf->hob_lbal, tf->hob_lbam, tf->hob_lbah, tf->nsect,
	      tf->hob_nsect);
}

static const u8 ata_rw_cmds[] = {
	/* pio multi */
	ATA_CMD_READ_MULTI,
	ATA_CMD_WRITE_MULTI,
	ATA_CMD_READ_MULTI_EXT,
	ATA_CMD_WRITE_MULTI_EXT,
	0,
	0,
	0,
	ATA_CMD_WRITE_MULTI_FUA_EXT,
	/* pio */
	ATA_CMD_PIO_READ,
	ATA_CMD_PIO_WRITE,
	ATA_CMD_PIO_READ_EXT,
	ATA_CMD_PIO_WRITE_EXT,
	0,
	0,
	0,
	0,
	/* dma */
	ATA_CMD_READ,
	ATA_CMD_WRITE,
	ATA_CMD_READ_EXT,
	ATA_CMD_WRITE_EXT,
	0,
	0,
	0,
	ATA_CMD_WRITE_FUA_EXT
};

/**
 *	ata_rwcmd_protocol - set taskfile r/w commands and protocol
 *	@tf: command to examine and configure
 *	@dev: device tf belongs to
 *
 *	Examine the device configuration and tf->flags to calculate
 *	the proper read/write commands and protocol to use.
 *
 */
static int ata_rwcmd_protocol(struct ata_taskfile *tf, struct ata_device *dev)
{
	u8 cmd;

	int index, fua, lba48, write;

	fua = (tf->flags & ATA_TFLAG_FUA) ? 4 : 0;
	lba48 = (tf->flags & ATA_TFLAG_LBA48) ? 2 : 0;
	write = (tf->flags & ATA_TFLAG_WRITE) ? 1 : 0;

	if (dev->flags & ATA_DFLAG_PIO) {
		tf->protocol = ATA_PROT_PIO;
		index = dev->multi_count ? 0 : 8;
	} else if (lba48 && (dev->link->ap->flags & ATA_FLAG_PIO_LBA48)) {
		/* Unable to use DMA due to host limitation */
		tf->protocol = ATA_PROT_PIO;
		index = dev->multi_count ? 0 : 8;
	} else {
		tf->protocol = ATA_PROT_DMA;
		index = 16;
	}

	cmd = ata_rw_cmds[index + fua + lba48 + write];
	if (cmd) {
		tf->command = cmd;
		return 0;
	}
	return -1;
}

/**
 *	ata_tf_read_block - Read block address from ATA taskfile
 *	@tf: ATA taskfile of interest
 *	@dev: ATA device @tf belongs to
 *
 *	LOCKING:
 *	None.
 *
 *	Read block address from @tf.  This function can handle all
 *	three address formats - LBA, LBA48 and CHS.  tf->protocol and
 *	flags select the address format to use.
 *
 *	RETURNS:
 *	Block address read from @tf.
 */
u64 ata_tf_read_block(struct ata_taskfile *tf, struct ata_device *dev)
{
	u64 block = 0;

	DPRINTK("%p, %p\n", tf, dev);
	if (tf->flags & ATA_TFLAG_LBA) {
		if (tf->flags & ATA_TFLAG_LBA48) {
			block |= (u64)tf->hob_lbah << 40;
			block |= (u64)tf->hob_lbam << 32;
			block |= (u64)tf->hob_lbal << 24;
		} else
			block |= (tf->device & 0xf) << 24;

		block |= tf->lbah << 16;
		block |= tf->lbam << 8;
		block |= tf->lbal;
	} else {
		u32 cyl, head, sect;

		cyl = tf->lbam | (tf->lbah << 8);
		head = tf->device & 0xf;
		sect = tf->lbal;

		if (!sect) {
			ata_dev_warn(dev,
				     "device reported invalid CHS sector 0\n");
			sect = 1; /* oh well */
		}

		block = (cyl * dev->heads + head) * dev->sectors + sect - 1;
	}

	return block;
}

/**
 *	ata_build_rw_tf - Build ATA taskfile for given read/write request
 *	@tf: Target ATA taskfile
 *	@dev: ATA device @tf belongs to
 *	@block: Block address
 *	@n_block: Number of blocks
 *	@tf_flags: RW/FUA etc...
 *	@tag: tag
 *
 *	Build ATA taskfile @tf for read/write request described by
 *	@block, @n_block, @tf_flags and @tag on @dev.
 *
 *	RETURNS:
 *
 *	0 on success, -EINVAL if the request is too large for @dev,
 *	-EINVAL if the request is invalid.
 */
int ata_build_rw_tf(struct ata_taskfile *tf, struct ata_device *dev,
		    u64 block, u32 n_block, unsigned int tf_flags,
		    unsigned int tag)
{
	tf->flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;
	tf->flags |= tf_flags;

	if (dev->flags & ATA_DFLAG_LBA) {
		debug("%s: Using LBA\n", __func__);
		tf->flags |= ATA_TFLAG_LBA;

		if (lba_28_ok(block, n_block)) {
			debug("%s: Using lba 28 block\n", __func__);
			/* use LBA28 */
			tf->device |= (block >> 24) & 0xf;
		} else if (lba_48_ok(block, n_block)) {
			if (!(dev->flags & ATA_DFLAG_LBA48))
				return -EINVAL;

			debug("%s: Using lba48\n", __func__);
			/* use LBA48 */
			tf->flags |= ATA_TFLAG_LBA48;

			tf->hob_nsect = (n_block >> 8) & 0xff;

			tf->hob_lbah = (block >> 40) & 0xff;
			tf->hob_lbam = (block >> 32) & 0xff;
			tf->hob_lbal = (block >> 24) & 0xff;
		} else
			/* request too large even for LBA48 */
			return -EINVAL;

		debug("%s: Getting protocol\n", __func__);
		if (unlikely(ata_rwcmd_protocol(tf, dev) < 0))
			return -EINVAL;

		debug("%s: Got protocol\n", __func__);
		tf->nsect = n_block & 0xff;

		tf->lbah = (block >> 16) & 0xff;
		tf->lbam = (block >> 8) & 0xff;
		tf->lbal = block & 0xff;

		tf->device |= ATA_LBA;
	} else {
		/* CHS */
		u32 sect, head, cyl, track;

		debug("%s: Using CHS\n", __func__);
		/* The request -may- be too large for CHS addressing. */
		if (!lba_28_ok(block, n_block))
			return -EINVAL;

		if (unlikely(ata_rwcmd_protocol(tf, dev) < 0))
			return -EINVAL;

		if (!dev->sectors) {
			printf("%s: Error: sectors = 0!\n", __func__);
			return -EINVAL;
		}
		/* Convert LBA to CHS */
		track = (u32)block / dev->sectors;
		cyl   = track / dev->heads;
		head  = track % dev->heads;
		sect  = (u32)block % dev->sectors + 1;

		debug("block %u track %u cyl %u head %u sect %u\n",
			(u32)block, track, cyl, head, sect);

		/* Check whether the converted CHS can fit.
		   Cylinder: 0-65535
		   Head: 0-15
		   Sector: 1-255*/
		if ((cyl >> 16) || (head >> 4) || (sect >> 8) || (!sect))
			return -EINVAL;

		tf->nsect = n_block & 0xff; /* Sector count 0 means 256 sectors */
		tf->lbal = sect;
		tf->lbam = cyl;
		tf->lbah = cyl >> 8;
		tf->device |= head;
	}

	debug("%s: EXIT\n", __func__);
	return 0;
}

/**
 *	ata_pack_xfermask - Pack pio, mwdma and udma masks into xfer_mask
 *	@pio_mask: pio_mask
 *	@mwdma_mask: mwdma_mask
 *	@udma_mask: udma_mask
 *
 *	Pack @pio_mask, @mwdma_mask and @udma_mask into a single
 *	unsigned int xfer_mask.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Packed xfer_mask.
 */
unsigned long ata_pack_xfermask(unsigned long pio_mask,
				unsigned long mwdma_mask,
				unsigned long udma_mask)
{
	return ((pio_mask << ATA_SHIFT_PIO) & ATA_MASK_PIO) |
		((mwdma_mask << ATA_SHIFT_MWDMA) & ATA_MASK_MWDMA) |
		((udma_mask << ATA_SHIFT_UDMA) & ATA_MASK_UDMA);
}

/**
 *	ata_unpack_xfermask - Unpack xfer_mask into pio, mwdma and udma masks
 *	@xfer_mask: xfer_mask to unpack
 *	@pio_mask: resulting pio_mask
 *	@mwdma_mask: resulting mwdma_mask
 *	@udma_mask: resulting udma_mask
 *
 *	Unpack @xfer_mask into @pio_mask, @mwdma_mask and @udma_mask.
 *	Any NULL distination masks will be ignored.
 */
void ata_unpack_xfermask(unsigned long xfer_mask, unsigned long *pio_mask,
			 unsigned long *mwdma_mask, unsigned long *udma_mask)
{
	if (pio_mask)
		*pio_mask = (xfer_mask & ATA_MASK_PIO) >> ATA_SHIFT_PIO;
	if (mwdma_mask)
		*mwdma_mask = (xfer_mask & ATA_MASK_MWDMA) >> ATA_SHIFT_MWDMA;
	if (udma_mask)
		*udma_mask = (xfer_mask & ATA_MASK_UDMA) >> ATA_SHIFT_UDMA;
}

static const struct ata_xfer_ent {
	int shift, bits;
	u8 base;
} ata_xfer_tbl[] = {
	{ ATA_SHIFT_PIO, ATA_NR_PIO_MODES, XFER_PIO_0 },
	{ ATA_SHIFT_MWDMA, ATA_NR_MWDMA_MODES, XFER_MW_DMA_0 },
	{ ATA_SHIFT_UDMA, ATA_NR_UDMA_MODES, XFER_UDMA_0 },
	{ -1, },
};

/**
 *	ata_xfer_mask2mode - Find matching XFER_* for the given xfer_mask
 *	@xfer_mask: xfer_mask of interest
 *
 *	Return matching XFER_* value for @xfer_mask.  Only the highest
 *	bit of @xfer_mask is considered.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Matching XFER_* value, 0xff if no match found.
 */
u8 ata_xfer_mask2mode(unsigned long xfer_mask)
{
	int highbit = fls(xfer_mask) - 1;
	const struct ata_xfer_ent *ent;

	for (ent = ata_xfer_tbl; ent->shift >= 0; ent++)
		if (highbit >= ent->shift && highbit < ent->shift + ent->bits)
			return ent->base + highbit - ent->shift;
	return 0xff;
}

/**
 *	ata_xfer_mode2mask - Find matching xfer_mask for XFER_*
 *	@xfer_mode: XFER_* of interest
 *
 *	Return matching xfer_mask for @xfer_mode.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Matching xfer_mask, 0 if no match found.
 */
unsigned long ata_xfer_mode2mask(u8 xfer_mode)
{
	const struct ata_xfer_ent *ent;

	for (ent = ata_xfer_tbl; ent->shift >= 0; ent++)
		if (xfer_mode >= ent->base && xfer_mode < ent->base + ent->bits)
			return ((2 << (ent->shift + xfer_mode - ent->base)) - 1)
				& ~((1 << ent->shift) - 1);
	return 0;
}

/**
 *	ata_xfer_mode2shift - Find matching xfer_shift for XFER_*
 *	@xfer_mode: XFER_* of interest
 *
 *	Return matching xfer_shift for @xfer_mode.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Matching xfer_shift, -1 if no match found.
 */
int ata_xfer_mode2shift(unsigned long xfer_mode)
{
	const struct ata_xfer_ent *ent;

	for (ent = ata_xfer_tbl; ent->shift >= 0; ent++)
		if (xfer_mode >= ent->base && xfer_mode < ent->base + ent->bits)
			return ent->shift;
	return -1;
}

/**
 *	ata_mode_string - convert xfer_mask to string
 *	@xfer_mask: mask of bits supported; only highest bit counts.
 *
 *	Determine string which represents the highest speed
 *	(highest bit in @modemask).
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Constant C string representing highest speed listed in
 *	@mode_mask, or the constant C string "<n/a>".
 */
const char *ata_mode_string(unsigned long xfer_mask)
{
	static const char * const xfer_mode_str[] = {
		"PIO0",
		"PIO1",
		"PIO2",
		"PIO3",
		"PIO4",
		"PIO5",
		"PIO6",
		"MWDMA0",
		"MWDMA1",
		"MWDMA2",
		"MWDMA3",
		"MWDMA4",
		"UDMA/16",
		"UDMA/25",
		"UDMA/33",
		"UDMA/44",
		"UDMA/66",
		"UDMA/100",
		"UDMA/133",
		"UDMA7",
	};
	int highbit;

	highbit = fls(xfer_mask) - 1;
	if (highbit >= 0 && highbit < ARRAY_SIZE(xfer_mode_str))
		return xfer_mode_str[highbit];
	return "<n/a>";
}

const char *sata_spd_string(unsigned int spd)
{
	static const char * const spd_str[] = {
		"1.5 Gbps",
		"3.0 Gbps",
		"6.0 Gbps",
	};

	if (spd == 0 || (spd - 1) >= ARRAY_SIZE(spd_str))
		return "<unknown>";
	return spd_str[spd - 1];
}

/**
 *	ata_dev_classify - determine device type based on ATA-spec signature
 *	@tf: ATA taskfile register set for device to be identified
 *
 *	Determine from taskfile register contents whether a device is
 *	ATA or ATAPI, as per "Signature and persistence" section
 *	of ATA/PI spec (volume 1, sect 5.14).
 *
 *	RETURNS:
 *	Device type, %ATA_DEV_ATA, %ATA_DEV_ATAPI, %ATA_DEV_PMP or
 *	%ATA_DEV_UNKNOWN the event of failure.
 */
unsigned int ata_dev_classify(const struct ata_taskfile *tf)
{
	debug("%s(%p)\n", __func__, tf);
	/* Apple's open source Darwin code hints that some devices only
	 * put a proper signature into the LBA mid/high registers,
	 * So, we only check those.  It's sufficient for uniqueness.
	 *
	 * ATA/ATAPI-7 (d1532v1r1: Feb. 19, 2003) specified separate
	 * signatures for ATA and ATAPI devices attached on SerialATA,
	 * 0x3c/0xc3 and 0x69/0x96 respectively.  However, SerialATA
	 * spec has never mentioned about using different signatures
	 * for ATA/ATAPI devices.  Then, Serial ATA II: Port
	 * Multiplier specification began to use 0x69/0x96 to identify
	 * port multpliers and 0x3c/0xc3 to identify SEMB device.
	 * ATA/ATAPI-7 dropped descriptions about 0x3c/0xc3 and
	 * 0x69/0x96 shortly and described them as reserved for
	 * SerialATA.
	 *
	 * We follow the current spec and consider that 0x69/0x96
	 * identifies a port multiplier and 0x3c/0xc3 a SEMB device.
	 * Unfortunately, WDC WD1600JS-62MHB5 (a hard drive) reports
	 * SEMB signature.  This is worked around in
	 * ata_dev_read_id().
	 */
	if ((tf->lbam == 0) && (tf->lbah == 0)) {
		debug("found ATA device by sig\n");
		return ATA_DEV_ATA;
	}

	if ((tf->lbam == 0x14) && (tf->lbah == 0xeb)) {
		debug("found ATAPI device by sig\n");
		return ATA_DEV_ATAPI;
	}

	if ((tf->lbam == 0x69) && (tf->lbah == 0x96)) {
		debug("found PMP device by sig\n");
		return ATA_DEV_PMP;
	}

	if ((tf->lbam == 0x3c) && (tf->lbah == 0xc3)) {
		debug("found SEMB device by sig (could be ATA device)\n");
		return ATA_DEV_SEMB;
	}

	debug("unknown device 0x%x 0x%x\n", tf->lbam, tf->lbah);
	return ATA_DEV_UNKNOWN;
}

/**
 *	ata_id_string - Convert IDENTIFY DEVICE page into string
 *	@id: IDENTIFY DEVICE results we will examine
 *	@s: string into which data is output
 *	@ofs: offset into identify device page
 *	@len: length of string to return. must be an even number.
 *
 *	The strings in the IDENTIFY DEVICE page are broken up into
 *	16-bit chunks.  Run through the string, and output each
 *	8-bit chunk linearly, regardless of platform.
 *
 *	LOCKING:
 *	caller.
 */
void ata_id_string(const u16 *id, unsigned char *s,
		  unsigned int ofs, unsigned int len)
{
	unsigned int c;

	while (len > 0) {
		c = id[ofs] >> 8;
		*s = c;
		s++;

		c = id[ofs] & 0xff;
		*s = c;
		s++;

		ofs++;
		len -= 2;
	}
}

/**
 *	ata_id_c_string - Convert IDENTIFY DEVICE page into C string
 *	@id: IDENTIFY DEVICE results we will examine
 *	@s: string into which data is output
 *	@ofs: offset into identify device page
 *	@len: length of string to return. must be an odd number.
 *
 *	This function is identical to ata_id_string except that it
 *	trims trailing spaces and terminates the resulting string with
 *	null.  @len must be actual maximum length (even number) + 1.
 *
 *	LOCKING:
 *	caller.
 */
void ata_id_c_string(const u16 *id, unsigned char *s,
		     unsigned int ofs, unsigned int len)
{
	unsigned char *p;

	ata_id_string(id, s, ofs, len - 1);

	p = s + strnlen((char *)s, len - 1);
	while (p > s && p[-1] == ' ')
		p--;
	*p = '\0';
}

u64 ata_id_n_sectors(u16 *id)
{
	if (ata_id_has_lba(id)) {
		if (ata_id_has_lba48(id))
			return ata_id_u64(id, ATA_ID_LBA48_SECTORS);
		else
			return ata_id_u32(id, ATA_ID_LBA_SECTORS);
	} else {
		return 0;
	}
}

u64 ata_tf_to_lba48(const struct ata_taskfile *tf)
{
	u64 sectors = 0;

	sectors |= ((u64)(tf->hob_lbah & 0xff)) << 40;
	sectors |= ((u64)(tf->hob_lbam & 0xff)) << 32;
	sectors |= ((u64)(tf->hob_lbal & 0xff)) << 24;
	sectors |= (tf->lbah & 0xff) << 16;
	sectors |= (tf->lbam & 0xff) << 8;
	sectors |= (tf->lbal & 0xff);

	DPRINTK("%p, sectors: 0x%llx\n", tf, sectors);
	return sectors;
}

u64 ata_tf_to_lba(const struct ata_taskfile *tf)
{
	u64 sectors = 0;

	sectors |= (tf->device & 0x0f) << 24;
	sectors |= (tf->lbah & 0xff) << 16;
	sectors |= (tf->lbam & 0xff) << 8;
	sectors |= (tf->lbal & 0xff);

	DPRINTK("%p => 0x%llx\n", tf, sectors);
	return sectors;
}

/**
 *	ata_read_native_max_address - Read native max address
 *	@dev: target device
 *	@max_sectors: out parameter for the result native max address
 *
 *	Perform an LBA48 or LBA28 native size query upon the device in
 *	question.
 *
 *	RETURNS:
 *	0 on success, -EACCES if command is aborted by the drive.
 *	-EIO on other errors.
 */
static int ata_read_native_max_address(struct ata_device *dev, u64 *max_sectors)
{
	unsigned int err_mask;
	struct ata_taskfile tf;
	int lba48 = ata_id_has_lba48(dev->id);

	DPRINTK("ENTER (%d) %p, %p\n", dev->link->ap->port_no, dev, max_sectors);
	ata_tf_init(dev, &tf);

	/* always clear all address registers */
	tf.flags |= ATA_TFLAG_DEVICE | ATA_TFLAG_ISADDR;

	if (lba48) {
		tf.command = ATA_CMD_READ_NATIVE_MAX_EXT;
		tf.flags |= ATA_TFLAG_LBA48;
	} else
		tf.command = ATA_CMD_READ_NATIVE_MAX;

	tf.protocol |= ATA_PROT_NODATA;
	tf.device |= ATA_LBA;

	err_mask = ata_exec_internal(dev, &tf, NULL, DMA_NONE, NULL, 0, 0);
	if (err_mask) {
		ata_dev_warn(dev,
			     "failed to read native max address (err_mask=0x%x)\n",
			     err_mask);
		if (err_mask == AC_ERR_DEV && (tf.feature & ATA_ABORTED))
			return -EACCES;
		return -EIO;
	}

	if (lba48)
		*max_sectors = ata_tf_to_lba48(&tf) + 1;
	else
		*max_sectors = ata_tf_to_lba(&tf) + 1;
	DPRINTK("%p, %p (0x%llx)\n", dev, max_sectors, *max_sectors);
	return 0;
}

/**
 *	ata_set_max_sectors - Set max sectors
 *	@dev: target device
 *	@new_sectors: new max sectors value to set for the device
 *
 *	Set max sectors of @dev to @new_sectors.
 *
 *	RETURNS:
 *	0 on success, -EACCES if command is aborted or denied (due to
 *	previous non-volatile SET_MAX) by the drive.  -EIO on other
 *	errors.
 */
static int ata_set_max_sectors(struct ata_device *dev, u64 new_sectors)
{
	unsigned int err_mask;
	struct ata_taskfile tf;
	int lba48 = ata_id_has_lba48(dev->id);

	DPRINTK("%p, 0x%llx\n", dev, new_sectors);
	new_sectors--;

	ata_tf_init(dev, &tf);

	tf.flags |= ATA_TFLAG_DEVICE | ATA_TFLAG_ISADDR;

	if (lba48) {
		tf.command = ATA_CMD_SET_MAX_EXT;
		tf.flags |= ATA_TFLAG_LBA48;

		tf.hob_lbal = (new_sectors >> 24) & 0xff;
		tf.hob_lbam = (new_sectors >> 32) & 0xff;
		tf.hob_lbah = (new_sectors >> 40) & 0xff;
	} else {
		tf.command = ATA_CMD_SET_MAX;

		tf.device |= (new_sectors >> 24) & 0xf;
	}

	tf.protocol |= ATA_PROT_NODATA;
	tf.device |= ATA_LBA;

	tf.lbal = (new_sectors >> 0) & 0xff;
	tf.lbam = (new_sectors >> 8) & 0xff;
	tf.lbah = (new_sectors >> 16) & 0xff;

	err_mask = ata_exec_internal(dev, &tf, NULL, DMA_NONE, NULL, 0, 0);
	if (err_mask) {
		ata_dev_warn(dev,
			     "failed to set max address (err_mask=0x%x)\n",
			     err_mask);
		if (err_mask == AC_ERR_DEV &&
		    (tf.feature & (ATA_ABORTED | ATA_IDNF)))
			return -EACCES;
		return -EIO;
	}

	return 0;
}

/**
 *	ata_dump_id - IDENTIFY DEVICE info debugging output
 *	@id: IDENTIFY DEVICE page to dump
 *
 *	Dump selected 16-bit words from the given IDENTIFY DEVICE
 *	page.
 *
 *	LOCKING:
 *	caller.
 */
void ata_dump_id(u16 *id)
{
	unsigned char serial[ATA_ID_SERNO_LEN + 1];
	unsigned char firmware[ATA_ID_FW_REV_LEN + 1];
	unsigned char product[ATA_ID_PROD_LEN + 1];
	u64 n_sectors;

	/* Serial number */
	ata_id_c_string(id, serial, ATA_ID_SERNO, sizeof(serial));
	printf("S/N: %s\n\r", serial);

	/* Firmware version */
	ata_id_c_string(id, firmware, ATA_ID_FW_REV, sizeof(firmware));
	printf("Firmware version: %s\n\r", firmware);

	/* Product model */
	ata_id_c_string(id, product, ATA_ID_PROD, sizeof(product));
	printf("Product model number: %s\n\r", product);

	/* Total sectors of device  */
	n_sectors = ata_id_n_sectors(id);
	printf("Capablity: %lld sectors\n\r", n_sectors);

	printf ("id[49]: capabilities = 0x%04x\n"
		"id[53]: field valid = 0x%04x\n"
		"id[63]: mwdma = 0x%04x\n"
		"id[64]: pio = 0x%04x\n"
		"id[75]: queue depth = 0x%04x\n",
		id[49],
		id[53],
		id[63],
		id[64],
		id[75]);

	printf ("id[76]: sata capablity = 0x%04x\n"
		"id[78]: sata features supported = 0x%04x\n"
		"id[79]: sata features enable = 0x%04x\n",
		id[76],
		id[78],
		id[79]);

	printf ("id[80]: major version = 0x%04x\n"
		"id[81]: minor version = 0x%04x\n"
		"id[82]: command set supported 1 = 0x%04x\n"
		"id[83]: command set supported 2 = 0x%04x\n"
		"id[84]: command set extension = 0x%04x\n",
		id[80],
		id[81],
		id[82],
		id[83],
		id[84]);
	printf ("id[85]: command set enable 1 = 0x%04x\n"
		"id[86]: command set enable 2 = 0x%04x\n"
		"id[87]: command set default = 0x%04x\n"
		"id[88]: udma = 0x%04x\n"
		"id[93]: hardware reset result = 0x%04x\n",
		id[85],
		id[86],
		id[87],
		id[88],
		id[93]);
}

/**
 *	ata_id_xfermask - Compute xfermask from the given IDENTIFY data
 *	@id: IDENTIFY data to compute xfer mask from
 *
 *	Compute the xfermask for this device. This is not as trivial
 *	as it seems if we must consider early devices correctly.
 *
 *	FIXME: pre IDE drive timing (do we care ?).
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	Computed xfermask
 */
unsigned long ata_id_xfermask(const u16 *id)
{
	unsigned long pio_mask, mwdma_mask, udma_mask;

	/* Usual case. Word 53 indicates word 64 is valid */
	if (id[ATA_ID_FIELD_VALID] & (1 << 1)) {
		pio_mask = id[ATA_ID_PIO_MODES] & 0x03;
		pio_mask <<= 3;
		pio_mask |= 0x7;
	} else {
		/* If word 64 isn't valid then Word 51 high byte holds
		 * the PIO timing number for the maximum. Turn it into
		 * a mask.
		 */
		u8 mode = (id[ATA_ID_OLD_PIO_MODES] >> 8) & 0xFF;
		if (mode < 5)	/* Valid PIO range */
			pio_mask = (2 << mode) - 1;
		else
			pio_mask = 1;

		/* But wait.. there's more. Design your standards by
		 * committee and you too can get a free iordy field to
		 * process. However its the speeds not the modes that
		 * are supported... Note drivers using the timing API
		 * will get this right anyway
		 */
	}

	mwdma_mask = id[ATA_ID_MWDMA_MODES] & 0x07;

	if (ata_id_is_cfa(id)) {
		/*
		 *	Process compact flash extended modes
		 */
		int pio = (id[ATA_ID_CFA_MODES] >> 0) & 0x7;
		int dma = (id[ATA_ID_CFA_MODES] >> 3) & 0x7;

		if (pio)
			pio_mask |= (1 << 5);
		if (pio > 1)
			pio_mask |= (1 << 6);
		if (dma)
			mwdma_mask |= (1 << 3);
		if (dma > 1)
			mwdma_mask |= (1 << 4);
	}

	udma_mask = 0;
	if (id[ATA_ID_FIELD_VALID] & (1 << 2))
		udma_mask = id[ATA_ID_UDMA_MODES] & 0xff;

	return ata_pack_xfermask(pio_mask, mwdma_mask, udma_mask);
}

void swap_buf_le16(u16 *buf, unsigned int buf_words)
{
	unsigned int i;

	for (i = 0; i < buf_words; i++)
		buf[i] = le16_to_cpu(buf[i]);
}

static int __sata_set_spd_needed(struct ata_link *link, u32 *scontrol)
{
	struct ata_link *host_link = &link->ap->link;
	u32 limit, target, spd;

	limit = link->sata_spd_limit;

	/* Don't configure downstream link faster than upstream link.
	 * It doesn't speed up anything and some PMPs choke on such
	 * configuration.
	 */
	if (!ata_is_host_link(link) && host_link->sata_spd)
		limit &= (1 << host_link->sata_spd) - 1;

	if (limit == UINT_MAX)
		target = 0;
	else
		target = fls(limit);

	spd = (*scontrol >> 4) & 0xf;
	*scontrol = (*scontrol & ~0xf0) | ((target & 0xf) << 4);

	return spd != target;
}

/**
 *	sata_set_spd_needed - is SATA spd configuration needed
 *	@link: Link in question
 *
 *	Test whether the spd limit in SControl matches
 *	@link->sata_spd_limit.  This function is used to determine
 *	whether hardreset is necessary to apply SATA spd
 *	configuration.
 *
 *	LOCKING:
 *	Inherited from caller.
 *
 *	RETURNS:
 *	1 if SATA spd configuration is needed, 0 otherwise.
 */
static int sata_set_spd_needed(struct ata_link *link)
{
	u32 scontrol;

	if (sata_scr_read(link, SCR_CONTROL, &scontrol))
		return 1;

	return __sata_set_spd_needed(link, &scontrol);
}

/**
 *	sata_set_spd - set SATA spd according to spd limit
 *	@link: Link to set SATA spd for
 *
 *	Set SATA spd of @link according to sata_spd_limit.
 *
 *	LOCKING:
 *	Inherited from caller.
 *
 *	RETURNS:
 *	0 if spd doesn't need to be changed, 1 if spd has been
 *	changed.  Negative errno if SCR registers are inaccessible.
 */
int sata_set_spd(struct ata_link *link)
{
	u32 scontrol;
	int rc;

	if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
		return rc;

	if (!__sata_set_spd_needed(link, &scontrol))
		return 0;

	if ((rc = sata_scr_write(link, SCR_CONTROL, scontrol)))
		return rc;

	return 1;
}

/**
 *	ata_do_dev_read_id		-	default ID read method
 *	@dev: device
 *	@tf: proposed taskfile
 *	@id: data buffer
 *
 *	Issue the identify taskfile and hand back the buffer containing
 *	identify data. For some RAID controllers and for pre ATA devices
 *	this function is wrapped or replaced by the driver
 */
unsigned int ata_do_dev_read_id(struct ata_device *dev,
					struct ata_taskfile *tf, u16 *id)
{
	return ata_exec_internal(dev, tf, NULL, DMA_FROM_DEVICE,
				 id, sizeof(id[0]) * ATA_ID_WORDS, 0);
}

/**
 *	ata_dev_read_id - Read ID data from the specified device
 *	@dev: target device
 *	@p_class: pointer to class of the target device (may be changed)
 *	@flags: ATA_READID_* flags
 *	@id: buffer to read IDENTIFY data into
 *
 *	Read ID data from the specified device.  ATA_CMD_ID_ATA is
 *	performed on ATA devices and ATA_CMD_ID_ATAPI on ATAPI
 *	devices.  This function also issues ATA_CMD_INIT_DEV_PARAMS
 *	for pre-ATA4 drives.
 *
 *	FIXME: ATA_CMD_ID_ATA is optional for early drives and right
 *	now we abort if we hit that case.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, -errno otherwise.
 */
int ata_dev_read_id(struct ata_device *dev, unsigned int *p_class,
		    unsigned int flags, u16 *id)
{
	struct ata_port *ap = dev->link->ap;
	unsigned int class = *p_class;
	struct ata_taskfile tf;
	unsigned int err_mask = 0;
	const char *reason;
	bool is_semb = class == ATA_DEV_SEMB;
	int may_fallback = 1, tried_spinup = 0;
	int rc;

	debug("%s: ENTER\n", __func__);

retry:
	ata_tf_init(dev, &tf);

	switch (class) {
	case ATA_DEV_SEMB:
		class = ATA_DEV_ATA;	/* some hard drives report SEMB sig */
	case ATA_DEV_ATA:
		tf.command = ATA_CMD_ID_ATA;
		break;
	case ATA_DEV_ATAPI:
		tf.command = ATA_CMD_ID_ATAPI;
		break;
	default:
		rc = -ENODEV;
		reason = "unsupported class";
		goto err_out;
	}

	tf.protocol = ATA_PROT_PIO;

	/* Some devices choke if TF registers contain garbage.  Make
	 * sure those are properly initialized.
	 */
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;

	/* Device presence detection is unreliable on some
	 * controllers.  Always poll IDENTIFY if available.
	 */
	tf.flags |= ATA_TFLAG_POLLING;

	if (ap->ops->read_id)
		err_mask = ap->ops->read_id(dev, &tf, id);
	else
		err_mask = ata_do_dev_read_id(dev, &tf, id);

	if (err_mask) {
		if (err_mask & AC_ERR_NODEV_HINT) {
			puts("NODEV after polling detection\n");
			return -ENOENT;
		}

		if (is_semb) {
			puts("IDENTIFY failed on device w/ SEMB sig, disabled\n");
			/* SEMB is not supported yet */
			*p_class = ATA_DEV_SEMB_UNSUP;
			return 0;
		}

		if ((err_mask == AC_ERR_DEV) && (tf.feature & ATA_ABORTED)) {
			/* Device or controller might have reported
			 * the wrong device class.  Give a shot at the
			 * other IDENTIFY if the current one is
			 * aborted by the device.
			 */
			if (may_fallback) {
				may_fallback = 0;

				if (class == ATA_DEV_ATA)
					class = ATA_DEV_ATAPI;
				else
					class = ATA_DEV_ATA;
				goto retry;
			}

			/* Control reaches here iff the device aborted
			 * both flavors of IDENTIFYs which happens
			 * sometimes with phantom devices.
			 */
			puts("both IDENTIFYs aborted, assuming NODEV\n");
			return -ENOENT;
		}

		rc = -EIO;
		reason = "I/O error";
		goto err_out;
	}

	/* Falling back doesn't make sense if ID data was read
	 * successfully at least once.
	 */
	may_fallback = 0;

	swap_buf_le16(id, ATA_ID_WORDS);

	/* sanity check */
	rc = -EINVAL;
	reason = "device reports invalid type";

	if (class == ATA_DEV_ATA) {
		if (!ata_id_is_ata(id) && !ata_id_is_cfa(id))
			goto err_out;
		if (ap->host->flags & ATA_HOST_IGNORE_ATA &&
							ata_id_is_ata(id)) {
			printf("host indicates ignore ATA devices, ignored\n");
			return -ENOENT;
		}
	} else {
		if (ata_id_is_ata(id))
			goto err_out;
	}

	if (!tried_spinup && (id[2] == 0x37c8 || id[2] == 0x738c)) {
		tried_spinup = 1;
		/*
		 * Drive powered-up in standby mode, and requires a specific
		 * SET_FEATURES spin-up subcommand before it will accept
		 * anything other than the original IDENTIFY command.
		 */
		err_mask = ata_dev_set_feature(dev, SETFEATURES_SPINUP, 0);
		if (err_mask && id[2] != 0x738c) {
			rc = -EIO;
			reason = "SPINUP failed";
			goto err_out;
		}
		/*
		 * If the drive initially returned incomplete IDENTIFY info,
		 * we now must reissue the IDENTIFY command.
		 */
		if (id[2] == 0x37c8)
			goto retry;
	}

	if ((flags & ATA_READID_POSTRESET) && class == ATA_DEV_ATA) {
		/*
		 * The exact sequence expected by certain pre-ATA4 drives is:
		 * SRST RESET
		 * IDENTIFY (optional in early ATA)
		 * INITIALIZE DEVICE PARAMETERS (later IDE and ATA)
		 * anything else..
		 * Some drives were very specific about that exact sequence.
		 *
		 * Note that ATA4 says lba is mandatory so the second check
		 * should never trigger.
		 */
		if (ata_id_major_version(id) < 4 || !ata_id_has_lba(id)) {
			err_mask = ata_dev_init_params(dev, id[3], id[6]);
			if (err_mask) {
				rc = -EIO;
				reason = "INIT_DEV_PARAMS failed";
				goto err_out;
			}

			/* current CHS translation info (id[53-58]) might be
			 * changed. reread the identify device info.
			 */
			flags &= ~ATA_READID_POSTRESET;
			goto retry;
		}
	}

	*p_class = class;

	return 0;

 err_out:

	printf("failed to IDENTIFY (%s, err_mask=0x%x)\n", reason, err_mask);
	return rc;
}

/**
 *	sata_link_resume - resume SATA link
 *	@link: ATA link to resume SATA
 *	@params: timing parameters { interval, duratinon, timeout } in msec
 *	@deadline: deadline jiffies for the operation
 *
 *	Resume SATA phy @link and debounce it.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int sata_link_resume(struct ata_link *link, const unsigned long *params,
		     unsigned long deadline)
{
	int tries = ATA_LINK_RESUME_TRIES;
	u32 scontrol, serror;
	int rc;

	if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
		return rc;

	/*
	 * Writes to SControl sometimes get ignored under certain
	 * controllers (ata_piix SIDPR).  Make sure DET actually is
	 * cleared.
	 */
	do {
		scontrol = (scontrol & 0x0f0) | 0x300;
		if ((rc = sata_scr_write(link, SCR_CONTROL, scontrol)))
			return rc;
		/*
		 * Some PHYs react badly if SStatus is pounded
		 * immediately after resuming.  Delay 200ms before
		 * debouncing.
		 */
		ata_msleep(link->ap, 200);

		/* is SControl restored correctly? */
		if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
			return rc;
	} while ((scontrol & 0xf0f) != 0x300 && --tries);

	if ((scontrol & 0xf0f) != 0x300) {
		printf("failed to resume link (SControl %X)\n", scontrol);
		return 0;
	}

	if (tries < ATA_LINK_RESUME_TRIES)
		printf("link resume succeeded after %d retries\n",
		       ATA_LINK_RESUME_TRIES - tries);

	/* clear SError, some PHYs require this even for SRST to work */
	if (!(rc = sata_scr_read(link, SCR_ERROR, &serror)))
		rc = sata_scr_write(link, SCR_ERROR, serror);

	return rc != -EINVAL ? rc : 0;
}

/**
 *	ata_std_prereset - prepare for reset
 *	@link: ATA link to be reset
 *	@deadline: deadline jiffies for the operation
 *
 *	@link is about to be reset.  Initialize it.  Failure from
 *	prereset makes libata abort whole reset sequence and give up
 *	that port, so prereset should be best-effort.  It does its
 *	best to prepare for reset sequence but if things go wrong, it
 *	should just whine, not fail.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, -errno otherwise.
 */
int ata_std_prereset(struct ata_link *link, unsigned long deadline)
{
	struct ata_port *ap = link->ap;
	const unsigned long *timing = sata_ehc_deb_timing();
	int rc;

	debug("%s(%p (%d), %lu)\n", __func__, link, ap->port_no, deadline);
	/* if SATA, resume link */
	if (ap->flags & ATA_FLAG_SATA) {
		rc = sata_link_resume(link, timing, deadline);
		/* whine about phy resume failure but proceed */
		if (rc && rc != -EOPNOTSUPP)
			printf("failed to resume link for reset (errno=%d)\n",
			       rc);
	}

	return 0;
}

/**
 *	sata_link_hardreset - reset link via SATA phy reset
 *	@link: link to reset
 *	@timing: timing parameters { interval, duratinon, timeout } in msec
 *	@deadline: deadline jiffies for the operation
 *	@online: optional out parameter indicating link onlineness
 *	@check_ready: optional callback to check link readiness
 *
 *	SATA phy-reset @link using DET bits of SControl register.
 *	After hardreset, link readiness is waited upon using
 *	ata_wait_ready() if @check_ready is specified.  LLDs are
 *	allowed to not specify @check_ready and wait itself after this
 *	function returns.  Device classification is LLD's
 *	responsibility.
 *
 *	*@online is set to one iff reset succeeded and @link is online
 *	after reset.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, -errno otherwise.
 */
int sata_link_hardreset(struct ata_link *link, const unsigned long *timing,
			unsigned long deadline,
			bool *online, int (*check_ready)(struct ata_link *))
{
	u32 scontrol;
	int rc;
	ulong start = get_timer(0);

	DPRINTK("ENTER\n");

	debug("%s(%p, %p, %lu, %p, %p)\n", __func__, link, timing,
	      deadline, online, check_ready);
	if (online)
		*online = false;

	if (sata_set_spd_needed(link)) {
		/* SATA spec says nothing about how to reconfigure
		 * spd.  To be on the safe side, turn off phy during
		 * reconfiguration.  This works for at least ICH7 AHCI
		 * and Sil3124.
		 */
		if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
			goto out;

		scontrol = (scontrol & 0x0f0) | 0x304;

		if ((rc = sata_scr_write(link, SCR_CONTROL, scontrol)))
			goto out;

		sata_set_spd(link);
	}

	/* issue phy wake/reset */
	if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
		goto out;

	scontrol = (scontrol & 0x0f0) | 0x301;

	if ((rc = sata_scr_write_flush(link, SCR_CONTROL, scontrol)))
		goto out;

	/* Couldn't find anything in SATA I/II specs, but AHCI-1.1
	 * 10.4.2 says at least 1 ms.
	 */
	ata_msleep(link->ap, 1);

	/* bring link back */
	rc = sata_link_resume(link, timing, deadline);
	if (rc)
		goto out;
	/* if link is offline nothing more to do */
	if (ata_phys_link_offline(link))
		goto out;

	/* Link is online.  From this point, -ENODEV too is an error. */
	if (online)
		*online = true;

	if (sata_pmp_supported(link->ap) && ata_is_host_link(link)) {
		/* If PMP is supported, we have to do follow-up SRST.
		 * Some PMPs don't send D2H Reg FIS after hardreset if
		 * the first port is empty.  Wait only for
		 * ATA_TMOUT_PMP_SRST_WAIT.
		 */
		debug("%s: doing follow-up SRST for PMP\n", __func__);
		if (check_ready) {
			unsigned long pmp_deadline =
				ATA_TMOUT_PMP_SRST_WAIT - start;
			if (pmp_deadline > 0) {
				if (pmp_deadline > deadline)
					pmp_deadline = deadline;
				ata_wait_ready(link, pmp_deadline, check_ready);
			}
		}
		rc = -EAGAIN;
		goto out;
	}

	rc = 0;
	if (check_ready)
		rc = ata_wait_ready(link, deadline, check_ready);
 out:
	if (rc && rc != -EAGAIN) {
		/* online is set iff link is online && reset succeeded */
		if (online)
			*online = false;
		printf("COMRESET failed (errno=%d)\n", rc);
	}
	DPRINTK("EXIT, rc=%d\n", rc);
	return rc;
}

/**
 *	sata_std_hardreset - COMRESET w/o waiting or classification
 *	@link: link to reset
 *	@class: resulting class of attached device
 *	@deadline: deadline jiffies for the operation
 *
 *	Standard SATA COMRESET w/o waiting or classification.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 if link offline, -EAGAIN if link online, -errno on errors.
 */
int sata_std_hardreset(struct ata_link *link, unsigned int *class,
		       unsigned long deadline)
{
	const unsigned long *timing = sata_ehc_deb_timing();
	bool online;
	int rc;

	debug("%s(%p, %p, %lu)\n", __func__, link, class, deadline);
	/* do hardreset */
	rc = sata_link_hardreset(link, timing, deadline, &online, NULL);
	return online ? -EAGAIN : rc;
}

/**
 *	ata_std_postreset - standard postreset callback
 *	@link: the target ata_link
 *	@classes: classes of attached devices
 *
 *	This function is invoked after a successful reset.  Note that
 *	the device might have been reset more than once using
 *	different reset methods before postreset is invoked.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 */
void ata_std_postreset(struct ata_link *link, unsigned int *classes)
{
	u32 serror;

	DPRINTK("ENTER\n");

	/* reset complete, clear SError */
	if (!sata_scr_read(link, SCR_ERROR, &serror))
		sata_scr_write(link, SCR_ERROR, serror);

	DPRINTK("EXIT\n");
}

/**
 *	ata_dev_same_device - Determine whether new ID matches configured device
 *	@dev: device to compare against
 *	@new_class: class of the new device
 *	@new_id: IDENTIFY page of the new device
 *
 *	Compare @new_class and @new_id against @dev and determine
 *	whether @dev is the device indicated by @new_class and
 *	@new_id.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	1 if @dev matches @new_class and @new_id, 0 otherwise.
 */
static int ata_dev_same_device(struct ata_device *dev, unsigned int new_class,
			       const u16 *new_id)
{
	const u16 *old_id = dev->id;
	unsigned char model[2][ATA_ID_PROD_LEN + 1];
	unsigned char serial[2][ATA_ID_SERNO_LEN + 1];

	if (dev->class != new_class) {
		ata_dev_info(dev, "class mismatch %d != %d\n",
			     dev->class, new_class);
		return 0;
	}

	ata_id_c_string(old_id, model[0], ATA_ID_PROD, sizeof(model[0]));
	ata_id_c_string(new_id, model[1], ATA_ID_PROD, sizeof(model[1]));
	ata_id_c_string(old_id, serial[0], ATA_ID_SERNO, sizeof(serial[0]));
	ata_id_c_string(new_id, serial[1], ATA_ID_SERNO, sizeof(serial[1]));

	if (strcmp(model[0], model[1])) {
		ata_dev_info(dev, "model number mismatch '%s' != '%s'\n",
			     model[0], model[1]);
		return 0;
	}

	if (strcmp(serial[0], serial[1])) {
		ata_dev_info(dev, "serial number mismatch '%s' != '%s'\n",
			     serial[0], serial[1]);
		return 0;
	}

	return 1;
}

/**
 *	ata_dev_reread_id - Re-read IDENTIFY data
 *	@dev: target ATA device
 *	@readid_flags: read ID flags
 *
 *	Re-read IDENTIFY page and make sure @dev is still attached to
 *	the port.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, negative errno otherwise
 */
int ata_dev_reread_id(struct ata_device *dev, unsigned int readid_flags)
{
	unsigned int class = dev->class;
	u16 *id = (void *)dev->link->ap->sector_buf;
	int rc;

	/* read ID data */
	rc = ata_dev_read_id(dev, &class, readid_flags, id);
	if (rc)
		return rc;

	/* is the device still there? */
	if (!ata_dev_same_device(dev, class, id))
		return -ENODEV;

	memcpy(dev->id, id, sizeof(id[0]) * ATA_ID_WORDS);
	return 0;
}

/**
 *	ata_dev_revalidate - Revalidate ATA device
 *	@dev: device to revalidate
 *	@new_class: new class code
 *	@readid_flags: read ID flags
 *
 *	Re-read IDENTIFY page, make sure @dev is still attached to the
 *	port and reconfigure it according to the new IDENTIFY page.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, negative errno otherwise
 */
int ata_dev_revalidate(struct ata_device *dev, unsigned int new_class,
		       unsigned int readid_flags)
{
	u64 n_sectors = dev->n_sectors;
	u64 n_native_sectors = dev->n_native_sectors;
	int rc;

	if (!ata_dev_enabled(dev))
		return -ENODEV;

	/* fail early if !ATA && !ATAPI to avoid issuing [P]IDENTIFY to PMP */
	if (ata_class_enabled(new_class) &&
	    new_class != ATA_DEV_ATA &&
	    new_class != ATA_DEV_ATAPI &&
	    new_class != ATA_DEV_SEMB) {
		ata_dev_info(dev, "class mismatch %u != %u\n",
			     dev->class, new_class);
		rc = -ENODEV;
		goto fail;
	}

	/* re-read ID */
	rc = ata_dev_reread_id(dev, readid_flags);
	if (rc)
		goto fail;

	/* configure device according to the new ID */
	rc = ata_dev_configure(dev);
	if (rc)
		goto fail;

	/* verify n_sectors hasn't changed */
	if (dev->class != ATA_DEV_ATA || !n_sectors ||
	    dev->n_sectors == n_sectors)
		return 0;

	/* n_sectors has changed */
	ata_dev_warn(dev, "n_sectors mismatch %llu != %llu\n",
		     (unsigned long long)n_sectors,
		     (unsigned long long)dev->n_sectors);

	/*
	 * Something could have caused HPA to be unlocked
	 * involuntarily.  If n_native_sectors hasn't changed and the
	 * new size matches it, keep the device.
	 */
	if (dev->n_native_sectors == n_native_sectors &&
	    dev->n_sectors > n_sectors && dev->n_sectors == n_native_sectors) {
		ata_dev_warn(dev,
			     "new n_sectors matches native, probably "
			     "late HPA unlock, n_sectors updated\n");
		/* use the larger n_sectors */
		return 0;
	}

	/*
	 * Some BIOSes boot w/o HPA but resume w/ HPA locked.  Try
	 * unlocking HPA in those cases.
	 *
	 * https://bugzilla.kernel.org/show_bug.cgi?id=15396
	 */
	if (dev->n_native_sectors == n_native_sectors &&
	    dev->n_sectors < n_sectors && n_sectors == n_native_sectors) {
		puts("old n_sectors matches native, probably late HPA lock, will try to unlock HPA\n");
		/* try unlocking HPA */
		dev->flags |= ATA_DFLAG_UNLOCK_HPA;
		rc = -EIO;
	} else
		rc = -ENODEV;

	/* restore original n_[native_]sectors and fail */
	dev->n_native_sectors = n_native_sectors;
	dev->n_sectors = n_sectors;
 fail:
	printf("revalidation failed (errno=%d)\n", rc);
	return rc;
}

/**
 *	ata_dev_set_feature - Issue SET FEATURES - SATA FEATURES
 *	@dev: Device to which command will be sent
 *	@enable: Whether to enable or disable the feature
 *	@feature: The sector count represents the feature to set
 *
 *	Issue SET FEATURES - SATA FEATURES command to device @dev
 *	on port @ap with sector count
 *
 *	LOCKING:
 *	PCI/etc. bus probe sem.
 *
 *	RETURNS:
 *	0 on success, AC_ERR_* mask otherwise.
 */
unsigned int ata_dev_set_feature(struct ata_device *dev, u8 enable, u8 feature)
{
	struct ata_taskfile tf;
	unsigned int err_mask;

	/* set up set-features taskfile */
	DPRINTK("set features - SATA features\n");
	debug("%s(%p, %d, 0x%x)\n", __func__, dev, enable, feature);

	ata_tf_init(dev, &tf);
	tf.command = ATA_CMD_SET_FEATURES;
	tf.feature = enable;
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;
	tf.protocol = ATA_PROT_NODATA;
	tf.nsect = feature;

	err_mask = ata_exec_internal(dev, &tf, NULL, DMA_NONE, NULL, 0, 0);

	DPRINTK("EXIT, err_mask=%x\n", err_mask);
	return err_mask;
}

/**
 *	ata_dev_init_params - Issue INIT DEV PARAMS command
 *	@dev: Device to which command will be sent
 *	@heads: Number of heads (taskfile parameter)
 *	@sectors: Number of sectors (taskfile parameter)
 *
 *	LOCKING:
 *	Kernel thread context (may sleep)
 *
 *	RETURNS:
 *	0 on success, AC_ERR_* mask otherwise.
 */
static unsigned int ata_dev_init_params(struct ata_device *dev,
					u16 heads, u16 sectors)
{
	struct ata_taskfile tf;
	unsigned int err_mask;

	debug("%s(%p, 0x%x, 0x%x)\n", __func__, dev, heads, sectors);
	/* Number of sectors per track 1-255. Number of heads 1-16 */
	if (sectors < 1 || sectors > 255 || heads < 1 || heads > 16)
		return AC_ERR_INVALID;

	/* set up init dev params taskfile */
	DPRINTK("init dev params\n");

	ata_tf_init(dev, &tf);
	tf.command = ATA_CMD_INIT_DEV_PARAMS;
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;
	tf.protocol = ATA_PROT_NODATA;
	tf.nsect = sectors;
	tf.device |= (heads - 1) & 0x0f; /* max head = num. of heads - 1 */

	err_mask = ata_exec_internal(dev, &tf, NULL, DMA_NONE, NULL, 0, 0);
	/* A clean abort indicates an original or just out of spec drive
	   and we should continue as we issue the setup based on the
	   drive reported working geometry */
	if (err_mask == AC_ERR_DEV && (tf.feature & ATA_ABORTED))
		err_mask = 0;

	DPRINTK("EXIT, err_mask=%x\n", err_mask);
	return err_mask;
}

/**
 *	ata_wait_after_reset - wait for link to become ready after reset
 *	@link: link to be waited on
 *	@deadline: deadline jiffies for the operation
 *	@check_ready: callback to check link readiness
 *
 *	Wait for @link to become ready after reset.
 *
 *	RETURNS:
 *	0 if @linke is ready before @deadline; otherwise, -errno.
 */
int ata_wait_after_reset(struct ata_link *link, unsigned long deadline,
				int (*check_ready)(struct ata_link *link))
{
	mdelay(ATA_WAIT_AFTER_RESET);

	return ata_wait_ready(link, deadline, check_ready);
}

/**
 *	ata_dev_init - Initialize an ata_device structure
 *	@dev: Device structure to initialize
 *
 *	Initialize @dev in preparation for probing.
 */
void ata_dev_init(struct ata_device *dev)
{
	struct ata_link *link = ata_dev_phys_link(dev);

	debug("%s(%p) link: %p\n", __func__, dev, link);
	/* SATA spd limit is bound to the attached device, reset together */
	link->sata_spd_limit = link->hw_sata_spd_limit;
	link->sata_spd = 0;

	dev->flags &= ~ATA_DFLAG_INIT_MASK;
	dev->horkage = 0;

	memset((void *)dev + ATA_DEVICE_CLEAR_BEGIN, 0,
	       ATA_DEVICE_CLEAR_END - ATA_DEVICE_CLEAR_BEGIN);
	dev->pio_mask = 0xFFFFFFFF;
	dev->mwdma_mask = 0xFFFFFFFF;
	dev->udma_mask = 0xFFFFFFFF;
}

/**
 *	ata_phys_link_online - test whether the given link is online
 *	@link: ATA link to test
 *
 *	Test whether @link is online.  Note that this function returns
 *	0 if online status of @link cannot be obtained, so
 *	ata_link_online(link) != !ata_link_offline(link).
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	True if the port online status is available and online.
 */
bool ata_phys_link_online(struct ata_link *link)
{
	u32 sstatus;

	if (sata_scr_read(link, SCR_STATUS, &sstatus) == 0 &&
	    ata_sstatus_online(sstatus))
		return true;
	return false;
}

/**
 *	ata_phys_link_offline - test whether the given link is offline
 *	@link: ATA link to test
 *
 *	Test whether @link is offline.  Note that this function
 *	returns 0 if offline status of @link cannot be obtained, so
 *	ata_link_online(link) != !ata_link_offline(link).
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	True if the port offline status is available and offline.
 */
bool ata_phys_link_offline(struct ata_link *link)
{
	u32 sstatus;

	if (sata_scr_read(link, SCR_STATUS, &sstatus) == 0 &&
	    !ata_sstatus_online(sstatus))
		return true;
	return false;
}

/**
 *	ata_link_online - test whether the given link is online
 *	@link: ATA link to test
 *
 *	Test whether @link is online.  This is identical to
 *	ata_phys_link_online() when there's no slave link.  When
 *	there's a slave link, this function should only be called on
 *	the master link and will return true if any of M/S links is
 *	online.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	True if the port online status is available and online.
 */
bool ata_link_online(struct ata_link *link)
{
	struct ata_link *slave = link->ap->slave_link;

	if (link == slave);	/* shouldn't be called on slave link */
		printf("%s: Warning: Should not be called on slave link\n",
		       __func__);

	return ata_phys_link_online(link) ||
		(slave && ata_phys_link_online(slave));
}

/**
 *	ata_link_offline - test whether the given link is offline
 *	@link: ATA link to test
 *
 *	Test whether @link is offline.  This is identical to
 *	ata_phys_link_offline() when there's no slave link.  When
 *	there's a slave link, this function should only be called on
 *	the master link and will return true if both M/S links are
 *	offline.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	True if the port offline status is available and offline.
 */
bool ata_link_offline(struct ata_link *link)
{
	struct ata_link *slave = link->ap->slave_link;

	if (link == slave);	/* shouldn't be called on slave link */
		printf("%s: Warning: should not be called on slave link.\n",
		       __func__);

	return ata_phys_link_offline(link) &&
		(!slave || ata_phys_link_offline(slave));
}

/**
 *	sata_scr_valid - test whether SCRs are accessible
 *	@link: ATA link to test SCR accessibility for
 *
 *	Test whether SCRs are accessible for @link.
 *
 *	LOCKING:
 *	None.
 *
 *	RETURNS:
 *	1 if SCRs are accessible, 0 otherwise.
 */
int sata_scr_valid(struct ata_link *link)
{
	struct ata_port *ap = link->ap;

	debug("%s(%d), checking if valid, flags: 0x%x, scr_read: %p\n",
	      __func__, ap->port_no, ap->flags, ap->ops->scr_read);
	return (ap->flags & ATA_FLAG_SATA) && ap->ops->scr_read;
}

/**
 *	sata_scr_read - read SCR register of the specified port
 *	@link: ATA link to read SCR for
 *	@reg: SCR to read
 *	@val: Place to store read value
 *
 *	Read SCR register @reg of @link into *@val.  This function is
 *	guaranteed to succeed if @link is ap->link, the cable type of
 *	the port is SATA and the port implements ->scr_read.
 *
 *	LOCKING:
 *	None if @link is ap->link.  Kernel thread context otherwise.
 *
 *	RETURNS:
 *	0 on success, negative errno on failure.
 */
int sata_scr_read(struct ata_link *link, int reg, u32 *val)
{
	debug("%s(%d, %d, 0x%p)\n", __func__, link->ap->port_no, reg, val);
	if (ata_is_host_link(link)) {
		debug("%s: is host link\n", __func__);
		if (sata_scr_valid(link)) {
			debug("%s: scr %p is valid\n", __func__,
			      link->ap->ops->scr_read);
			return link->ap->ops->scr_read(link, reg, val);
		}
	}

	debug("%s: Calling sata_pmp_scr_read()\n", __func__);
	return sata_pmp_scr_read(link, reg, val);
}

/**
 *	sata_scr_write - write SCR register of the specified port
 *	@link: ATA link to write SCR for
 *	@reg: SCR to write
 *	@val: value to write
 *
 *	Write @val to SCR register @reg of @link.  This function is
 *	guaranteed to succeed if @link is ap->link, the cable type of
 *	the port is SATA and the port implements ->scr_read.
 *
 *	LOCKING:
 *	None if @link is ap->link.  Kernel thread context otherwise.
 *
 *	RETURNS:
 *	0 on success, negative errno on failure.
 */
int sata_scr_write(struct ata_link *link, int reg, u32 val)
{
	if (ata_is_host_link(link)) {
		if (sata_scr_valid(link))
			return link->ap->ops->scr_write(link, reg, val);
	}

	return sata_pmp_scr_write(link, reg, val);
}

/**
 *	sata_scr_write_flush - write SCR register of the specified port and flush
 *	@link: ATA link to write SCR for
 *	@reg: SCR to write
 *	@val: value to write
 *
 *	This function is identical to sata_scr_write() except that this
 *	function performs flush after writing to the register.
 *
 *	LOCKING:
 *	None if @link is ap->link.  Kernel thread context otherwise.
 *
 *	RETURNS:
 *	0 on success, negative errno on failure.
 */
int sata_scr_write_flush(struct ata_link *link, int reg, u32 val)
{
	if (ata_is_host_link(link)) {
		int rc;

		if (sata_scr_valid(link)) {
			rc = link->ap->ops->scr_write(link, reg, val);
			if (rc == 0)
				rc = link->ap->ops->scr_read(link, reg, &val);
			return rc;
		}
		return -EOPNOTSUPP;
	}

	return sata_pmp_scr_write(link, reg, val);
}

/**
 *	ata_link_init - Initialize an ata_link structure
 *	@ap: ATA port link is attached to
 *	@link: Link structure to initialize
 *	@pmp: Port multiplier port number
 *
 *	Initialize @link.
 */
void ata_link_init(struct ata_port *ap, struct ata_link *link, int pmp)
{
	int i;

	debug("%s(0x%p(%d), 0x%p, %d)\n", __func__, ap, ap->port_no, link, pmp);
	/* clear everything except for devices */
	memset((void *)link + ATA_LINK_CLEAR_BEGIN, 0,
	       ATA_LINK_CLEAR_END - ATA_LINK_CLEAR_BEGIN);

	link->ap = ap;
	link->pmp = pmp;
	link->hw_sata_spd_limit = UINT_MAX;

	/* can't use iterator, ap isn't initialized yet */
	for (i = 0; i < ATA_MAX_DEVICES; i++) {
		struct ata_device *dev = &(link->device[i]);
		debug("%s: dev(%d): %p, link: %p\n", __func__, i, dev, link);
		dev->link = link;
		dev->devno = dev - &(link->device[0]);
		ata_dev_init(dev);
		debug("%s: dev %p(%d) link: %p\n", __func__, dev, dev->devno, link);
	}
	DPRINTK("EXIT\n");
}

/**
 *	sata_link_init_spd - Initialize link->sata_spd_limit
 *	@link: Link to configure sata_spd_limit for
 *
 *	Initialize @link->[hw_]sata_spd_limit to the currently
 *	configured value.
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int sata_link_init_spd(struct ata_link *link)
{
	u8 spd;
	int rc;

	debug("%s(%d)\n", __func__, link->ap->port_no);
	rc = sata_scr_read(link, SCR_CONTROL, &link->saved_scontrol);
	if (rc) {
		debug("%s: sata_scr_read returned %d\n", __func__, rc);
		return rc;
	}

	spd = (link->saved_scontrol >> 4) & 0xf;
	if (spd)
		link->hw_sata_spd_limit &= (1 << spd) - 1;

	link->sata_spd_limit = link->hw_sata_spd_limit;
	debug("%s: sata speed limit: %d\n", __func__, link->sata_spd_limit);

	return 0;
}

/**
 *	ata_port_alloc - allocate and initialize basic ATA port resources
 *	@host: ATA host this allocated port belongs to
 *
 *	Allocate and initialize basic ATA port resources.
 *
 *	RETURNS:
 *	Allocate ATA port on success, NULL on failure.
 */
struct ata_port *ata_port_alloc(struct ata_host *host)
{
	struct ata_port *ap;

	debug("%s(0x%p)\n", __func__, host);

	ap = calloc(sizeof(*ap), 1);
	if (!ap) {
		debug("%s: Out of memory\n", __func__);
		return NULL;
	}

	ap->pflags = ATA_PFLAG_INITIALIZING | ATA_PFLAG_FROZEN;
	ap->local_port_no = -1;
	ap->print_id = -1;
	ap->host = host;

	debug("%s: ap: %p, initializing link\n", __func__, ap);
	ata_link_init(ap, &(ap->link), 0);

	return ap;
}

/**
 *	ata_host_alloc - allocate and init basic ATA host resources
 *	@dev: generic device this host is associated with
 *	@max_ports: maximum number of ATA ports associated with this host
 *
 *	Allocate and initialize basic ATA host resources.  LLD calls
 *	this function to allocate a host, initializes it fully and
 *	attaches it using ata_host_register().
 *
 *	@max_ports ports are allocated and host->n_ports is
 *	initialized to @max_ports.  The caller is allowed to decrease
 *	host->n_ports before calling ata_host_register().  The unused
 *	ports will be automatically freed on registration.
 *
 *	RETURNS:
 *	Allocate ATA host on success, NULL on failure.
 */
struct ata_host *ata_host_alloc(int max_ports)
{
	struct ata_host *host;
	size_t sz;
	int i;

	debug("%s(%d)\n", __func__, max_ports);

	/* alloc a container for our list of ATA ports (buses) */
	sz = sizeof(struct ata_host) + (max_ports + 1) * sizeof(void *);
	/* alloc a container for our list of ATA ports (buses) */
	debug("Allocating %d bytes\n", sz);
	host = calloc(sz, 1);
	if (!host)
		goto err_out;

	host->n_ports = max_ports;

	/* allocate ports bound to this host */
	for (i = 0; i < max_ports; i++) {
		struct ata_port *ap;

		ap = ata_port_alloc(host);
		if (!ap)
			goto err_out;

		ap->port_no = i;
		host->ports[i] = ap;
	}

	return host;

 err_out:
	free(host);
	return NULL;
}

/**
 *	ata_host_alloc_pinfo - alloc host and init with port_info array
 *	@dev: generic device this host is associated with
 *	@ppi: array of ATA port_info to initialize host with
 *	@n_ports: number of ATA ports attached to this host
 *
 *	Allocate ATA host and initialize with info from @ppi.  If NULL
 *	terminated, @ppi may contain fewer entries than @n_ports.  The
 *	last entry will be used for the remaining ports.
 *
 *	RETURNS:
 *	Allocate ATA host on success, NULL on failure.
 *
 *	LOCKING:
 *	Inherited from calling layer (may sleep).
 */
struct ata_host *ata_host_alloc_pinfo(const struct ata_port_info * const * ppi,
				      int n_ports)
{
	const struct ata_port_info *pi;
	struct ata_host *host;
	int i, j;

	debug("%s(%p(%p), %d)\n", __func__, ppi, *ppi, n_ports);
	host = ata_host_alloc(n_ports);
	if (!host)
		return NULL;

	for (i = 0, j = 0, pi = NULL; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		if (ppi[j])
			pi = ppi[j++];

		ap->pio_mask = pi->pio_mask;
		ap->mwdma_mask = pi->mwdma_mask;
		ap->udma_mask = pi->udma_mask;
		ap->flags |= pi->flags;
		debug("%s: port %d flags: 0x%x, pi flags: 0x%x\n",
		      __func__, i, ap->flags, pi->flags);
		ap->link.flags |= pi->link_flags;
		ap->ops = pi->port_ops;
		ap->port_no = i;
	}

	return host;
}

/**
 *	ata_slave_link_init - initialize slave link
 *	@ap: port to initialize slave link for
 *
 *	Create and initialize slave link for @ap.  This enables slave
 *	link handling on the port.
 *
 *	In libata, a port contains links and a link contains devices.
 *	There is single host link but if a PMP is attached to it,
 *	there can be multiple fan-out links.  On SATA, there's usually
 *	a single device connected to a link but PATA and SATA
 *	controllers emulating TF based interface can have two - master
 *	and slave.
 *
 *	However, there are a few controllers which don't fit into this
 *	abstraction too well - SATA controllers which emulate TF
 *	interface with both master and slave devices but also have
 *	separate SCR register sets for each device.  These controllers
 *	need separate links for physical link handling
 *	(e.g. onlineness, link speed) but should be treated like a
 *	traditional M/S controller for everything else (e.g. command
 *	issue, softreset).
 *
 *	slave_link is libata's way of handling this class of
 *	controllers without impacting core layer too much.  For
 *	anything other than physical link handling, the default host
 *	link is used for both master and slave.  For physical link
 *	handling, separate @ap->slave_link is used.  All dirty details
 *	are implemented inside libata core layer.  From LLD's POV, the
 *	only difference is that prereset, hardreset and postreset are
 *	called once more for the slave link, so the reset sequence
 *	looks like the following.
 *
 *	prereset(M) -> prereset(S) -> hardreset(M) -> hardreset(S) ->
 *	softreset(M) -> postreset(M) -> postreset(S)
 *
 *	Note that softreset is called only for the master.  Softreset
 *	resets both M/S by definition, so SRST on master should handle
 *	both (the standard method will work just fine).
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int ata_slave_link_init(struct ata_port *ap)
{
	struct ata_link *link;

	debug("%s(%p(%d))\n", __func__, ap, ap->port_no);
	link = calloc(sizeof(*link), 1);
	if (!link)
		return -ENOMEM;

	ata_link_init(ap, link, 1);
	ap->slave_link = link;
	return 0;
}

static void ata_host_stop(struct ata_host *host, void *res)
{
	int i;

	debug("%s(%p, %p)\n", __func__, host, res);
	if (!(host->flags & ATA_HOST_STARTED))
		printf("%s: Warning: host not started\n", __func__);

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		if (ap->ops->port_stop)
			ap->ops->port_stop(ap);
	}

	if (host->ops->host_stop)
		host->ops->host_stop(host);
}

/**
 *	ata_finalize_port_ops - finalize ata_port_operations
 *	@ops: ata_port_operations to finalize
 *
 *	An ata_port_operations can inherit from another ops and that
 *	ops can again inherit from another.  This can go on as many
 *	times as necessary as long as there is no loop in the
 *	inheritance chain.
 *
 *	Ops tables are finalized when the host is started.  NULL or
 *	unspecified entries are inherited from the closet ancestor
 *	which has the method and the entry is populated with it.
 *	After finalization, the ops table directly points to all the
 *	methods and ->inherits is no longer necessary and cleared.
 *
 *	Using ATA_OP_NULL, inheriting ops can force a method to NULL.
 *
 *	LOCKING:
 *	None.
 */
static void ata_finalize_port_ops(struct ata_port_operations *ops)
{
	const struct ata_port_operations *cur;
	void **begin = (void **)ops;
	void **end = (void **)&ops->inherits;
	void **pp;

	debug("%s(%p)\n", __func__, ops);
	if (!ops || !ops->inherits)
		return;

	for (cur = ops->inherits; cur; cur = cur->inherits) {
		void **inherit = (void **)cur;

		for (pp = begin; pp < end; pp++, inherit++)
			if (!*pp)
				*pp = *inherit;
	}

	ops->inherits = NULL;
}

/**
 *	ata_host_start - start and freeze ports of an ATA host
 *	@host: ATA host to start ports for
 *
 *	Start and then freeze ports of @host.  Started status is
 *	recorded in host->flags, so this function can be called
 *	multiple times.  Ports are guaranteed to get started only
 *	once.  If host->ops isn't initialized yet, its set to the
 *	first non-dummy port ops.
 *
 *	LOCKING:
 *	Inherited from calling layer (may sleep).
 *
 *	RETURNS:
 *	0 if all ports are started successfully, -errno otherwise.
 */
int ata_host_start(struct ata_host *host)
{
	int have_stop = 0;
	void *start_dr = NULL;
	int i, rc;

	debug("%s: ENTRY\n", __func__);
	if (host->flags & ATA_HOST_STARTED) {
		debug("%s: Host already started\n", __func__);
		return 0;
	}

	ata_finalize_port_ops(host->ops);

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		ata_finalize_port_ops(ap->ops);

		if (!host->ops && !ata_port_is_dummy(ap))
			host->ops = ap->ops;

		if (ap->ops->port_stop)
			have_stop = 1;
	}

	if (host->ops->host_stop)
		have_stop = 1;

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		if (ap->ops->port_start) {
			rc = ap->ops->port_start(ap);
			if (rc) {
				if (rc != -ENODEV)
					printf("failed to start port %d (errno=%d)\n",
					       i, rc);
				goto err_out;
			}
		}
	}

	host->flags |= ATA_HOST_STARTED;
	debug("%s: EXIT\n", __func__);
	return 0;

 err_out:
	while (--i >= 0) {
		struct ata_port *ap = host->ports[i];

		if (ap->ops->port_stop)
			ap->ops->port_stop(ap);
	}
	debug("%s: EXIT rc=%d\n", __func__, rc);
	return rc;
}

/**
 *	ata_sas_host_init - Initialize a host struct for sas (ipr, libsas)
 *	@host:	host to initialize
 *	@ops:	port_ops
 *
 */
void ata_host_init(struct ata_host *host, struct ata_port_operations *ops)
{
	DPRINTK("ENTER: %p, %p\n", host, ops);
	host->ops = ops;
}

void __ata_port_probe(struct ata_port *ap)
{
	struct ata_eh_info *ehi = &ap->link.eh_info;
	unsigned long flags;

	/* kick EH for boot probing */
	DPRINTK("%p (%d)\n", ap, ap->port_no);
	ehi->probe_mask |= ATA_ALL_DEVICES;
	ehi->action |= ATA_EH_RESET;
	ehi->flags |= ATA_EHI_NO_AUTOPSY | ATA_EHI_QUIET;

	ap->pflags &= ~ATA_PFLAG_INITIALIZING;
	ap->pflags |= ATA_PFLAG_LOADING;
	ata_port_schedule_eh(ap);
}

int ata_port_probe(struct ata_port *ap)
{
	int rc = 0;

	DPRINTK("%p (%d)\n", ap, ap->port_no);
	if (ap->ops->error_handler) {
		__ata_port_probe(ap);
		ata_port_wait_eh(ap);
	} else {
		DPRINTK("ata%u: bus probe begin\n", ap->print_id);
		rc = ata_bus_probe(ap);
		DPRINTK("ata%u: bus probe end\n", ap->print_id);
	}
	return rc;
}


/**
 *	ata_host_register - register initialized ATA host
 *	@host: ATA host to register
 *	@sht: template for SCSI host
 *
 *	Register initialized ATA host.  @host is allocated using
 *	ata_host_alloc() and fully initialized by LLD.  This function
 *	starts ports, registers @host with ATA and SCSI layers and
 *	probe registered devices.
 *
 *	LOCKING:
 *	Inherited from calling layer (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno otherwise.
 */
int ata_host_register(struct ata_host *host)
{
	int i, rc;

	DPRINTK("%p\n", host);
	/* host must have been started */
	if (!(host->flags & ATA_HOST_STARTED)) {
		printf("%s: BUG: trying to register unstarted host\n",
		       __func__);
		return -EINVAL;
	}

	/* Blow away unused ports.  This happens when LLD can't
	 * determine the exact number of ports to allocate at
	 * allocation time.
	 */
	for (i = host->n_ports; host->ports[i]; i++)
		if (host->ports[i])
			free(host->ports[i]);

	/* give ports names and add SCSI hosts */
	for (i = 0; i < host->n_ports; i++)
		host->ports[i]->print_id = ata_print_id++;


	/* set cable, sata_spd_limit and report */
	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];
		unsigned long xfer_mask;

		/* set SATA cable type if still unset */
		if (ap->cbl == ATA_CBL_NONE && (ap->flags & ATA_FLAG_SATA))
			ap->cbl = ATA_CBL_SATA;

		/* init sata_spd_limit to the current value */
		sata_link_init_spd(&ap->link);
		if (ap->slave_link)
			sata_link_init_spd(ap->slave_link);

		/* print per-port info to dmesg */
		xfer_mask = ata_pack_xfermask(ap->pio_mask, ap->mwdma_mask,
					      ap->udma_mask);

		if (!ata_port_is_dummy(ap)) {
			ata_port_info(ap, "%cATA max %s %s\n",
				      (ap->flags & ATA_FLAG_SATA) ? 'S' : 'P',
				      ata_mode_string(xfer_mask),
				      ap->link.eh_info.desc);
			ata_ehi_clear_desc(&ap->link.eh_info);
		} else
			ata_port_info(ap, "DUMMY\n");
	}

	DPRINTK("EXIT (0)\n");
	return 0;

 err_tadd:
	DPRINTK("EXIT (%d)\n", rc);
	return rc;

}

/**
 *	ata_wait_register - wait until register value changes
 *	@reg: IO-mapped register
 *	@mask: Mask to apply to read register value
 *	@val: Wait condition
 *	@timeout: timeout in milliseconds
 *
 *	Waiting for some bits of register to change is a common
 *	operation for ATA controllers.  This function reads 32bit LE
 *	IO-mapped register @reg and tests for the following condition.
 *
 *	(*@reg & mask) != val
 *
 *	If the condition is met, it returns; otherwise, the process is
 *	repeated after 1 msec until timeout.
 *
 *	RETURNS:
 *	The final register value.
 */
u32 ata_wait_register(void *reg, u32 mask, u32 val, unsigned long timeout)
{
	u32 tmp;
	u32 start;

	debug("%s(%p, 0x%x, 0x%x, %lu) ENTER\n", __func__, reg, mask, val,
	      timeout);
	tmp = ata_readl(reg);
	start = get_timer(0);
	while ((tmp & mask) == val && get_timer(start) < timeout) {
		udelay(10);
		tmp = ata_readl(reg);
	}

	debug("%s: EXIT 0x%x, time: %lu\n", __func__, tmp, get_timer(start));
	return tmp;
}

/**
 *	ata_msleep - ATA EH owner aware msleep
 *	@ap: ATA port to attribute the sleep to
 *	@msecs: duration to sleep in milliseconds
 *
 *	Sleeps @msecs.  If the current task is owner of @ap's EH, the
 *	ownership is released before going to sleep and reacquired
 *	after the sleep is complete.  IOW, other ports sharing the
 *	@ap->host will be allowed to own the EH while this task is
 *	sleeping.
 *
 *	LOCKING:
 *	Might sleep.
 */
void ata_msleep(struct ata_port *ap, unsigned int msecs)
{
	mdelay(msecs);
}

/**
 *	ata_wait_ready - wait for link to become ready
 *	@link: link to be waited on
 *	@deadline: deadline jiffies for the operation
 *	@check_ready: callback to check link readiness
 *
 *	Wait for @link to become ready.  @check_ready should return
 *	positive number if @link is ready, 0 if it isn't, -ENODEV if
 *	link doesn't seem to be occupied, other errno for other error
 *	conditions.
 *
 *	Transient -ENODEV conditions are allowed for
 *	ATA_TMOUT_FF_WAIT.
 *
 *	RETURNS:
 *	0 if @linke is ready before @deadline; otherwise, -errno.
 */
int ata_wait_ready(struct ata_link *link, unsigned long deadline,
		   int (*check_ready)(struct ata_link *link))
{
	unsigned long start = get_timer(0);
	unsigned long nodev_deadline;
	int warned = 0;

	nodev_deadline = 800;

	debug("%s(%p(%d), %lu, %p)\n", __func__, link, link->ap->port_no,
	      deadline, check_ready);
	/* Slave readiness can't be tested separately from master.  On
	 * M/S emulation configuration, this function should be called
	 * only on the master and it will handle both master and slave.
	 */

	if (((long)deadline - (long)nodev_deadline) < 0)
		nodev_deadline = deadline;

	while (1) {
		unsigned long now = get_timer(start);
		int ready, tmp;

		ready = tmp = check_ready(link);
		if (ready > 0)
			return 0;

		if (ready == -ENODEV) {
			if (ata_link_online(link))
				ready = 0;
			else if ((link->ap->flags & ATA_FLAG_SATA) &&
				 !ata_link_offline(link) &&
				 (now - start) < nodev_deadline * CONFIG_SYS_HZ)
				ready = 0;
		}
		if (ready)
			return ready;

		if (((long)deadline - (long)now) < 0) {
			debug("%s: timed out\n", __func__);
			return -EBUSY;
		}

		if (!warned &&
		    ((((long)start + 5 * CONFIG_SYS_HZ) - (long)now) < 0) &&
		    (deadline - now > 3 * CONFIG_SYS_HZ)) {
			printf("link is slow to respond, please be patient "
			       "(ready=%d)\n", tmp);
			warned = 1;
		}

		mdelay(50);
	}
}

int ata_tlink_add(struct ata_link *link)
{
	struct ata_port *ap = link->ap;
	struct ata_device *ata_dev;
	int error;

	debug("%s(%p(%d))\n", __func__, link, ap->port_no);
#if 0
	ata_for_each_dev(ata_dev, link, ALL) {
		error = ata_tdev_add(ata_dev);
		if (error) {
			goto tlink_dev_err;
		}
	}
#endif
	return 0;

tlink_dev_err:
	return error;
}

struct ata_port_operations ata_dummy_port_ops = {
};

const struct ata_port_info ata_dummy_port_info = {
	.port_ops		= &ata_dummy_port_ops,
};

/*
 * Utility print functions
 */
int ata_port_printk(const struct ata_port *ap, const char *level,
		    const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int r;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	r = printf("%sata%u: %pV", level, ap->print_id, &vaf);

	va_end(args);

	return r;
}

int ata_link_printk(const struct ata_link *link, const char *level,
		    const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int r;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	if (sata_pmp_attached(link->ap) || link->ap->slave_link)
		r = printf("%sata%u.%02u: %pV",
			   level, link->ap->print_id, link->pmp, &vaf);
	else
		r = printf("%sata%u: %pV",
			   level, link->ap->print_id, &vaf);

	va_end(args);

	return r;
}

int ata_dev_printk(const struct ata_device *dev, const char *level,
		    const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int r;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	r = printf("%sata%u.%02u: %pV",
		   level, dev->link->ap->print_id, dev->link->pmp + dev->devno,
		   &vaf);

	va_end(args);

	return r;
}

/**
 *	__ata_ehi_push_desc - push error description without adding separator
 *	@ehi: target EHI
 *	@fmt: printf format string
 *
 *	Format string according to @fmt and append it to @ehi->desc.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
void __ata_ehi_push_desc(struct ata_eh_info *ehi, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	__ata_ehi_pushv_desc(ehi, fmt, args);
	va_end(args);
}

/**
 *	ata_ehi_push_desc - push error description with separator
 *	@ehi: target EHI
 *	@fmt: printf format string
 *
 *	Format string according to @fmt and append it to @ehi->desc.
 *	If @ehi->desc is not empty, ", " is added in-between.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
void ata_ehi_push_desc(struct ata_eh_info *ehi, const char *fmt, ...)
{
	va_list args;

	if (ehi->desc_len)
		__ata_ehi_push_desc(ehi, ", ");

	va_start(args, fmt);
	__ata_ehi_pushv_desc(ehi, fmt, args);
	va_end(args);
}

/**
 *	ata_ehi_clear_desc - clean error description
 *	@ehi: target EHI
 *
 *	Clear @ehi->desc.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */
void ata_ehi_clear_desc(struct ata_eh_info *ehi)
{
	ehi->desc[0] = '\0';
	ehi->desc_len = 0;
}
