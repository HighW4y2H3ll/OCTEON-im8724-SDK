/*
 * Copyright 2003-2004 Red Hat, Inc.  All rights reserved.
 * Copyright 2003-2004 Jeff Garzik
 * Copyright (C) 2008 Freescale Semiconductor, Inc.
 *		Dave Liu <daveliu@freescale.com>
 *		port from libata of linux kernel
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LIBATA_H__
#define __LIBATA_H__

#include <common.h>
#ifdef CONFIG_PCI
# include <pci.h>
#endif
#include <errno.h>
#include <linux/compat.h>

/*
 * compile-time options: to be removed as soon as all the drivers are
 * converted to the new debugging mechanism
 */
#define ATA_DEBUG		/* debugging output */
#define ATA_VERBOSE_DEBUG	/* yet more debugging output */
#undef ATA_IRQ_TRAP		/* define to ack screaming irqs */
#undef ATA_NDEBUG		/* define to disable quick runtime checks */


/* note: prints function name for you */
#ifdef ATA_DEBUG
#define DPRINTK(fmt, args...) debug("%s: " fmt, __func__, ## args)
#ifdef ATA_VERBOSE_DEBUG
#define VPRINTK(fmt, args...) debug("%s: " fmt, __func__, ## args)
#else
#define VPRINTK(fmt, args...)
#endif	/* ATA_VERBOSE_DEBUG */
#else
#define DPRINTK(fmt, args...)
#define VPRINTK(fmt, args...)
#endif	/* ATA_DEBUG */

#define BPRINTK(fmt, args...) if (ap->flags & ATA_FLAG_DEBUGMSG) printf("%s: " fmt, __func__, ## args)

enum {
	/* various global constants */
	ATA_MAX_DEVICES		= 2,	/* per bus/port */
	ATA_MAX_PRD		= 256,	/* we could make these 256/256 */
	ATA_SECT_SIZE		= 512,
	ATA_MAX_SECTORS_128	= 128,
	ATA_MAX_SECTORS		= 256,
	ATA_MAX_SECTORS_LBA48	= 65535,
	ATA_MAX_SECTORS_TAPE	= 65535,
	ATA_ALL_DEVICES		= (1 << ATA_MAX_DEVICES) - 1,
	ATA_ID_WORDS		= 256,
	ATA_ID_CONFIG		= 0,
	ATA_ID_CYLS		= 1,
	ATA_ID_HEADS		= 3,
	ATA_ID_SECTORS		= 6,
	ATA_ID_SERNO		= 10,
	ATA_ID_BUF_SIZE		= 21,
	ATA_ID_FW_REV		= 23,
	ATA_ID_PROD		= 27,
	ATA_ID_MAX_MULTSECT	= 47,
	ATA_ID_DWORD_IO		= 48,
	ATA_ID_CAPABILITY	= 49,
	ATA_ID_OLD_PIO_MODES	= 51,
	ATA_ID_OLD_DMA_MODES	= 52,
	ATA_ID_FIELD_VALID	= 53,
	ATA_ID_CUR_CYLS		= 54,
	ATA_ID_CUR_HEADS	= 55,
	ATA_ID_CUR_SECTORS	= 56,
	ATA_ID_MULTSECT		= 59,
	ATA_ID_LBA_SECTORS	= 60,
	ATA_ID_SWDMA_MODES	= 62,
	ATA_ID_MWDMA_MODES	= 63,
	ATA_ID_PIO_MODES	= 64,
	ATA_ID_EIDE_DMA_MIN	= 65,
	ATA_ID_EIDE_DMA_TIME	= 66,
	ATA_ID_EIDE_PIO		= 67,
	ATA_ID_EIDE_PIO_IORDY	= 68,
	ATA_ID_ADDITIONAL_SUPP	= 69,
	ATA_ID_QUEUE_DEPTH	= 75,
	ATA_ID_SATA_CAP		= 76,
	ATA_ID_SATA_CAPABILITY_2	= 77,
	ATA_ID_SATA_FEATURES	= 78,
	ATA_ID_SATA_FEATURES_EN	= 79,
	ATA_ID_MAJOR_VER	= 80,
	ATA_ID_MINOR_VER	= 81,
	ATA_ID_COMMAND_SET_1	= 82,
	ATA_ID_COMMAND_SET_2	= 83,
	ATA_ID_CFSSE		= 84,
	ATA_ID_CFS_ENABLE_1	= 85,
	ATA_ID_CFS_ENABLE_2	= 86,
	ATA_ID_CSF_DEFAULT	= 87,
	ATA_ID_UDMA_MODES	= 88,
	ATA_ID_HW_CONFIG	= 93,
	ATA_ID_SPG		= 98,
	ATA_ID_LBA48_SECTORS	= 100,
	ATA_ID_SECTOR_SIZE	= 106,
	ATA_ID_WWN		= 108,
	ATA_ID_LOGICAL_SECTOR_SIZE	= 117,	/* and 118 */
	ATA_ID_LAST_LUN		= 126,
	ATA_ID_DLF		= 128,
	ATA_ID_CSFO		= 129,
	ATA_ID_CFA_POWER	= 160,
	ATA_ID_CFA_KEY_MGMT	= 162,
	ATA_ID_CFA_MODES	= 163,
	ATA_ID_DATA_SET_MGMT	= 169,
	ATA_ID_ROT_SPEED	= 217,
	ATA_ID_PIO4		= (1 << 1),

	ATA_ID_SERNO_LEN	= 20,
	ATA_ID_FW_REV_LEN	= 8,
	ATA_ID_PROD_LEN		= 40,

	ATA_PCI_CTL_OFS		= 2,

	ATA_PIO0		= (1 << 0),
	ATA_PIO1		= ATA_PIO0 | (1 << 1),
	ATA_PIO2		= ATA_PIO1 | (1 << 2),
	ATA_PIO3		= ATA_PIO2 | (1 << 3),
	ATA_PIO4		= ATA_PIO3 | (1 << 4),
	ATA_PIO5		= ATA_PIO4 | (1 << 5),
	ATA_PIO6		= ATA_PIO5 | (1 << 6),

	ATA_SWDMA0		= (1 << 0),
	ATA_SWDMA1		= ATA_SWDMA0 | (1 << 1),
	ATA_SWDMA2		= ATA_SWDMA1 | (1 << 2),

	ATA_SWDMA2_ONLY		= (1 << 2),

	ATA_MWDMA0		= (1 << 0),
	ATA_MWDMA1		= ATA_MWDMA0 | (1 << 1),
	ATA_MWDMA2		= ATA_MWDMA1 | (1 << 2),

	ATA_MWDMA12_ONLY	= (1 << 1) | (1 << 2),
	ATA_MWDMA2_ONLY		= (1 << 2),

	ATA_UDMA0		= (1 << 0),
	ATA_UDMA1		= ATA_UDMA0 | (1 << 1),
	ATA_UDMA2		= ATA_UDMA1 | (1 << 2),
	ATA_UDMA3		= ATA_UDMA2 | (1 << 3),
	ATA_UDMA4		= ATA_UDMA3 | (1 << 4),
	ATA_UDMA5		= ATA_UDMA4 | (1 << 5),
	ATA_UDMA6		= ATA_UDMA5 | (1 << 6),
	ATA_UDMA7		= ATA_UDMA6 | (1 << 7),
	/* ATA_UDMA7 is just for completeness... doesn't exist (yet?).  */

	ATA_UDMA_MASK_40C	= ATA_UDMA2,	/* udma0-2 */

	/* DMA-related */
	ATA_PRD_SZ		= 8,
	ATA_PRD_TBL_SZ		= (ATA_MAX_PRD * ATA_PRD_SZ),
	ATA_PRD_EOT		= (1 << 31),	/* end-of-table flag */

	ATA_DMA_TABLE_OFS	= 4,
	ATA_DMA_STATUS		= 2,
	ATA_DMA_CMD		= 0,
	ATA_DMA_WR		= (1 << 3),
	ATA_DMA_START		= (1 << 0),
	ATA_DMA_INTR		= (1 << 2),
	ATA_DMA_ERR		= (1 << 1),
	ATA_DMA_ACTIVE		= (1 << 0),

	/* bits in ATA command block registers */
	ATA_HOB			= (1 << 7),	/* LBA48 selector */
	ATA_NIEN		= (1 << 1),	/* disable-irq flag */
	ATA_LBA			= (1 << 6),	/* LBA28 selector */
	ATA_DEV1		= (1 << 4),	/* Select Device 1 (slave) */
	ATA_DEVICE_OBS		= (1 << 7) | (1 << 5), /* obs bits in dev reg */
	ATA_DEVCTL_OBS		= (1 << 3),	/* obsolete bit in devctl reg */
	ATA_BUSY		= (1 << 7),	/* BSY status bit */
	ATA_DRDY		= (1 << 6),	/* device ready */
	ATA_DF			= (1 << 5),	/* device fault */
	ATA_DRQ			= (1 << 3),	/* data request i/o */
	ATA_ERR			= (1 << 0),	/* have an error */
	ATA_SRST		= (1 << 2),	/* software reset */
	ATA_ICRC		= (1 << 7),	/* interface CRC error */
	ATA_UNC			= (1 << 6),	/* uncorrectable media error */
	ATA_IDNF		= (1 << 4),	/* ID not found */
	ATA_ABORTED		= (1 << 2),	/* command aborted */

	/* ATA command block registers */
	ATA_REG_DATA		= 0x00,
	ATA_REG_ERR		= 0x01,
	ATA_REG_NSECT		= 0x02,
	ATA_REG_LBAL		= 0x03,
	ATA_REG_LBAM		= 0x04,
	ATA_REG_LBAH		= 0x05,
	ATA_REG_DEVICE		= 0x06,
	ATA_REG_STATUS		= 0x07,

	ATA_REG_FEATURE		= ATA_REG_ERR, /* and their aliases */
	ATA_REG_CMD		= ATA_REG_STATUS,
	ATA_REG_BYTEL		= ATA_REG_LBAM,
	ATA_REG_BYTEH		= ATA_REG_LBAH,
	ATA_REG_DEVSEL		= ATA_REG_DEVICE,
	ATA_REG_IRQ		= ATA_REG_NSECT,

	/* ATA device commands */
	ATA_CMD_DEV_RESET	= 0x08, /* ATAPI device reset */
	ATA_CMD_CHK_POWER	= 0xE5, /* check power mode */
	ATA_CMD_STANDBY		= 0xE2, /* place in standby power mode */
	ATA_CMD_IDLE		= 0xE3, /* place in idle power mode */
	ATA_CMD_EDD		= 0x90,	/* execute device diagnostic */
	ATA_CMD_FLUSH		= 0xE7,
	ATA_CMD_FLUSH_EXT	= 0xEA,
	ATA_CMD_ID_ATA		= 0xEC,
	ATA_CMD_ID_ATAPI	= 0xA1,
	ATA_CMD_READ		= 0xC8,
	ATA_CMD_READ_EXT	= 0x25,
	ATA_CMD_WRITE		= 0xCA,
	ATA_CMD_WRITE_EXT	= 0x35,
	ATA_CMD_WRITE_FUA_EXT	= 0x3D,
	ATA_CMD_FPDMA_READ	= 0x60,
	ATA_CMD_FPDMA_WRITE	= 0x61,
	ATA_CMD_PIO_READ	= 0x20,
	ATA_CMD_PIO_READ_EXT	= 0x24,
	ATA_CMD_PIO_WRITE	= 0x30,
	ATA_CMD_PIO_WRITE_EXT	= 0x34,
	ATA_CMD_READ_MULTI	= 0xC4,
	ATA_CMD_READ_MULTI_EXT	= 0x29,
	ATA_CMD_WRITE_MULTI	= 0xC5,
	ATA_CMD_WRITE_MULTI_EXT	= 0x39,
	ATA_CMD_WRITE_MULTI_FUA_EXT = 0xCE,
	ATA_CMD_SET_FEATURES	= 0xEF,
	ATA_CMD_SET_MULTI	= 0xC6,
	ATA_CMD_PACKET		= 0xA0,
	ATA_CMD_VERIFY		= 0x40,
	ATA_CMD_VERIFY_EXT	= 0x42,
	ATA_CMD_STANDBYNOW1	= 0xE0,
	ATA_CMD_IDLEIMMEDIATE	= 0xE1,
	ATA_CMD_SLEEP		= 0xE6,
	ATA_CMD_INIT_DEV_PARAMS	= 0x91,
	ATA_CMD_READ_NATIVE_MAX	= 0xF8,
	ATA_CMD_READ_NATIVE_MAX_EXT = 0x27,
	ATA_CMD_SET_MAX		= 0xF9,
	ATA_CMD_SET_MAX_EXT	= 0x37,
	ATA_CMD_READ_LOG_EXT	= 0x2f,
	ATA_CMD_PMP_READ	= 0xE4,
	ATA_CMD_PMP_WRITE	= 0xE8,
	ATA_CMD_CONF_OVERLAY	= 0xB1,
	ATA_CMD_SEC_FREEZE_LOCK	= 0xF5,
	ATA_CMD_SMART		= 0xB0,

	/* READ_LOG_EXT pages */
	ATA_LOG_SATA_NCQ	= 0x10,

	/* READ/WRITE LONG (obsolete) */
	ATA_CMD_READ_LONG	= 0x22,
	ATA_CMD_READ_LONG_ONCE	= 0x23,
	ATA_CMD_WRITE_LONG	= 0x32,
	ATA_CMD_WRITE_LONG_ONCE	= 0x33,

	/* SETFEATURES stuff */
	SETFEATURES_XFER	= 0x03,
	XFER_UDMA_7		= 0x47,
	XFER_UDMA_6		= 0x46,
	XFER_UDMA_5		= 0x45,
	XFER_UDMA_4		= 0x44,
	XFER_UDMA_3		= 0x43,
	XFER_UDMA_2		= 0x42,
	XFER_UDMA_1		= 0x41,
	XFER_UDMA_0		= 0x40,
	XFER_MW_DMA_4		= 0x24,	/* CFA only */
	XFER_MW_DMA_3		= 0x23,	/* CFA only */
	XFER_MW_DMA_2		= 0x22,
	XFER_MW_DMA_1		= 0x21,
	XFER_MW_DMA_0		= 0x20,
	XFER_SW_DMA_2		= 0x12,
	XFER_SW_DMA_1		= 0x11,
	XFER_SW_DMA_0		= 0x10,
	XFER_PIO_6		= 0x0E,	/* CFA only */
	XFER_PIO_5		= 0x0D,	/* CFA only */
	XFER_PIO_4		= 0x0C,
	XFER_PIO_3		= 0x0B,
	XFER_PIO_2		= 0x0A,
	XFER_PIO_1		= 0x09,
	XFER_PIO_0		= 0x08,
	XFER_PIO_SLOW		= 0x00,

	SETFEATURES_WC_ON	= 0x02, /* Enable write cache */
	SETFEATURES_WC_OFF	= 0x82, /* Disable write cache */

	SETFEATURES_SPINUP	= 0x07, /* Spin-up drive */

	SETFEATURES_SATA_ENABLE = 0x10, /* Enable use of SATA feature */
	SETFEATURES_SATA_DISABLE = 0x90, /* Disable use of SATA feature */

	/* SETFEATURE Sector counts for SATA features */
	SATA_AN			= 0x05,  /* Asynchronous Notification */
	SATA_DIPM		= 0x03,  /* Device Initiated Power Management */

	/* feature values for SET_MAX */
	ATA_SET_MAX_ADDR	= 0x00,
	ATA_SET_MAX_PASSWD	= 0x01,
	ATA_SET_MAX_LOCK	= 0x02,
	ATA_SET_MAX_UNLOCK	= 0x03,
	ATA_SET_MAX_FREEZE_LOCK	= 0x04,

	/* feature values for DEVICE CONFIGURATION OVERLAY */
	ATA_DCO_RESTORE		= 0xC0,
	ATA_DCO_FREEZE_LOCK	= 0xC1,
	ATA_DCO_IDENTIFY	= 0xC2,
	ATA_DCO_SET		= 0xC3,

	/* ATAPI stuff */
	ATAPI_PKT_DMA		= (1 << 0),
	ATAPI_DMADIR		= (1 << 2),	/* ATAPI data dir:
						   0=to device, 1=to host */
	ATAPI_CDB_LEN		= 16,

	/* PMP stuff */
	SATA_PMP_MAX_PORTS	= 15,
	SATA_PMP_CTRL_PORT	= 15,

	SATA_PMP_GSCR_DWORDS	= 128,
	SATA_PMP_GSCR_PROD_ID	= 0,
	SATA_PMP_GSCR_REV	= 1,
	SATA_PMP_GSCR_PORT_INFO	= 2,
	SATA_PMP_GSCR_ERROR	= 32,
	SATA_PMP_GSCR_ERROR_EN	= 33,
	SATA_PMP_GSCR_FEAT	= 64,
	SATA_PMP_GSCR_FEAT_EN	= 96,

	SATA_PMP_PSCR_STATUS	= 0,
	SATA_PMP_PSCR_ERROR	= 1,
	SATA_PMP_PSCR_CONTROL	= 2,

	SATA_PMP_FEAT_BIST	= (1 << 0),
	SATA_PMP_FEAT_PMREQ	= (1 << 1),
	SATA_PMP_FEAT_DYNSSC	= (1 << 2),
	SATA_PMP_FEAT_NOTIFY	= (1 << 3),

	/* cable types */
	ATA_CBL_NONE		= 0,
	ATA_CBL_PATA40		= 1,
	ATA_CBL_PATA80		= 2,
	ATA_CBL_PATA40_SHORT	= 3,	/* 40 wire cable to high UDMA spec */
	ATA_CBL_PATA_UNK	= 4,	/* don't know, maybe 80c? */
	ATA_CBL_PATA_IGN	= 5,	/* don't know, ignore cable handling */
	ATA_CBL_SATA		= 6,

	/* SATA Status and Control Registers */
	SCR_STATUS		= 0,
	SCR_ERROR		= 1,
	SCR_CONTROL		= 2,
	SCR_ACTIVE		= 3,
	SCR_NOTIFICATION	= 4,

	/* SError bits */
	SERR_DATA_RECOVERED	= (1 << 0), /* recovered data error */
	SERR_COMM_RECOVERED	= (1 << 1), /* recovered comm failure */
	SERR_DATA		= (1 << 8), /* unrecovered data error */
	SERR_PERSISTENT		= (1 << 9), /* persistent data/comm error */
	SERR_PROTOCOL		= (1 << 10), /* protocol violation */
	SERR_INTERNAL		= (1 << 11), /* host internal error */
	SERR_PHYRDY_CHG		= (1 << 16), /* PHY RDY changed */
	SERR_PHY_INT_ERR	= (1 << 17), /* PHY internal error */
	SERR_COMM_WAKE		= (1 << 18), /* Comm wake */
	SERR_10B_8B_ERR		= (1 << 19), /* 10b to 8b decode error */
	SERR_DISPARITY		= (1 << 20), /* Disparity */
	SERR_CRC		= (1 << 21), /* CRC error */
	SERR_HANDSHAKE		= (1 << 22), /* Handshake error */
	SERR_LINK_SEQ_ERR	= (1 << 23), /* Link sequence error */
	SERR_TRANS_ST_ERROR	= (1 << 24), /* Transport state trans. error */
	SERR_UNRECOG_FIS	= (1 << 25), /* Unrecognized FIS */
	SERR_DEV_XCHG		= (1 << 26), /* device exchanged */

	/* struct ata_taskfile flags */
	ATA_TFLAG_LBA48		= (1 << 0), /* enable 48-bit LBA and "HOB" */
	ATA_TFLAG_ISADDR	= (1 << 1), /* enable r/w to nsect/lba regs */
	ATA_TFLAG_DEVICE	= (1 << 2), /* enable r/w to device reg */
	ATA_TFLAG_WRITE		= (1 << 3), /* data dir: host->dev==1 (write) */
	ATA_TFLAG_LBA		= (1 << 4), /* enable LBA */
	ATA_TFLAG_FUA		= (1 << 5), /* enable FUA */
	ATA_TFLAG_POLLING	= (1 << 6), /* set nIEN to 1 and use polling */

	/* protocol flags */
	ATA_PROT_FLAG_PIO	= (1 << 0), /* is PIO */
	ATA_PROT_FLAG_DMA	= (1 << 1), /* is DMA */
	ATA_PROT_FLAG_DATA	= ATA_PROT_FLAG_PIO | ATA_PROT_FLAG_DMA,
	ATA_PROT_FLAG_NCQ	= (1 << 2), /* is NCQ */
	ATA_PROT_FLAG_ATAPI	= (1 << 3), /* is ATAPI */

	/* Spec mandates to wait for ">= 2ms" before checking status
	 * after reset.  We wait 150ms, because that was the magic
	 * delay used for ATAPI devices in Hale Landis's ATADRVR, for
	 * the period of time between when the ATA command register is
	 * written, and then status is checked.  Because waiting for
	 * "a while" before checking status is fine, post SRST, we
	 * perform this magic delay here as well.
	 *
	 * Old drivers/ide uses the 2mS rule and then waits for ready.
	 */
	ATA_WAIT_AFTER_RESET	=  150,

	/* struct ata_device stuff */
	ATA_DFLAG_LBA		= (1 << 0), /* device supports LBA */
	ATA_DFLAG_LBA48		= (1 << 1), /* device supports LBA48 */
	ATA_DFLAG_CDB_INTR	= (1 << 2), /* device asserts INTRQ when ready for CDB */
	ATA_DFLAG_NCQ		= (1 << 3), /* device supports NCQ */
	ATA_DFLAG_FLUSH_EXT	= (1 << 4), /* do FLUSH_EXT instead of FLUSH */
	ATA_DFLAG_ACPI_PENDING	= (1 << 5), /* ACPI resume action pending */
	ATA_DFLAG_ACPI_FAILED	= (1 << 6), /* ACPI on devcfg has failed */
	ATA_DFLAG_AN		= (1 << 7), /* AN configured */
	ATA_DFLAG_DMADIR	= (1 << 10), /* device requires DMADIR */
	ATA_DFLAG_CFG_MASK	= (1 << 12) - 1,

	ATA_DFLAG_PIO		= (1 << 12), /* device limited to PIO mode */
	ATA_DFLAG_NCQ_OFF	= (1 << 13), /* device limited to non-NCQ mode */
	ATA_DFLAG_SLEEPING	= (1 << 15), /* device is sleeping */
	ATA_DFLAG_DUBIOUS_XFER	= (1 << 16), /* data transfer not verified */
	ATA_DFLAG_NO_UNLOAD	= (1 << 17), /* device doesn't support unload */
	ATA_DFLAG_UNLOCK_HPA	= (1 << 18), /* unlock HPA */
	ATA_DFLAG_INIT_MASK	= (1 << 24) - 1,

	ATA_DFLAG_DETACH	= (1 << 24),
	ATA_DFLAG_DETACHED	= (1 << 25),

	ATA_DFLAG_DA		= (1 << 26), /* device supports Device Attention */
	ATA_DFLAG_DEVSLP	= (1 << 27), /* device supports Device Sleep */
	ATA_DFLAG_ACPI_DISABLED = (1 << 28), /* ACPI for the device is disabled */

	ATA_DEV_UNKNOWN		= 0,	/* unknown device */
	ATA_DEV_ATA		= 1,	/* ATA device */
	ATA_DEV_ATA_UNSUP	= 2,	/* ATA device (unsupported) */
	ATA_DEV_ATAPI		= 3,	/* ATAPI device */
	ATA_DEV_ATAPI_UNSUP	= 4,	/* ATAPI device (unsupported) */
	ATA_DEV_PMP		= 5,	/* SATA port multiplier */
	ATA_DEV_PMP_UNSUP	= 6,	/* SATA port multiplier (unsupported) */
	ATA_DEV_SEMB		= 7,	/* SEMB */
	ATA_DEV_SEMB_UNSUP	= 8,	/* SEMB (unsupported) */
	ATA_DEV_NONE		= 9,	/* no device */

	/* struct ata_link flags */
	ATA_LFLAG_NO_HRST	= (1 << 1), /* avoid hardreset */
	ATA_LFLAG_NO_SRST	= (1 << 2), /* avoid softreset */
	ATA_LFLAG_ASSUME_ATA	= (1 << 3), /* assume ATA class */
	ATA_LFLAG_ASSUME_SEMB	= (1 << 4), /* assume SEMB class */
	ATA_LFLAG_ASSUME_CLASS	= ATA_LFLAG_ASSUME_ATA | ATA_LFLAG_ASSUME_SEMB,
	ATA_LFLAG_NO_RETRY	= (1 << 5), /* don't retry this link */
	ATA_LFLAG_DISABLED	= (1 << 6), /* link is disabled */
	ATA_LFLAG_SW_ACTIVITY	= (1 << 7), /* keep activity stats */
	ATA_LFLAG_NO_LPM	= (1 << 8), /* disable LPM on this link */
	ATA_LFLAG_RST_ONCE	= (1 << 9), /* limit recovery to one reset */

	/* struct ata_port flags */
	ATA_FLAG_SLAVE_POSS	= (1 << 0), /* host supports slave dev */
					    /* (doesn't imply presence) */
	ATA_FLAG_SATA		= (1 << 1),
	ATA_FLAG_NO_ATAPI	= (1 << 6), /* No ATAPI support */
	ATA_FLAG_PIO_DMA	= (1 << 7), /* PIO cmds via DMA */
	ATA_FLAG_PIO_LBA48	= (1 << 8), /* Host DMA engine is LBA28 only */
	ATA_FLAG_PIO_POLLING	= (1 << 9), /* use polling PIO if LLD
					     * doesn't handle PIO interrupts */
	ATA_FLAG_NCQ		= (1 << 10), /* host supports NCQ */
	ATA_FLAG_NO_POWEROFF_SPINDOWN = (1 << 11), /* don't spindown before poweroff */
	ATA_FLAG_NO_HIBERNATE_SPINDOWN = (1 << 12), /* don't spindown before hibernation */
	ATA_FLAG_DEBUGMSG	= (1 << 13),
	ATA_FLAG_FPDMA_AA		= (1 << 14), /* driver supports Auto-Activate */
	ATA_FLAG_IGN_SIMPLEX	= (1 << 15), /* ignore SIMPLEX */
	ATA_FLAG_NO_IORDY	= (1 << 16), /* controller lacks iordy */
	ATA_FLAG_ACPI_SATA	= (1 << 17), /* need native SATA ACPI layout */
	ATA_FLAG_AN		= (1 << 18), /* controller supports AN */
	ATA_FLAG_PMP		= (1 << 19), /* controller supports PMP */
	ATA_FLAG_EM		= (1 << 21), /* driver supports enclosure
					      * management */
	ATA_FLAG_SW_ACTIVITY	= (1 << 22), /* driver supports sw activity
					      * led */
	ATA_FLAG_NO_DIPM	= (1 << 23), /* host not happy with DIPM */

	/* bits 24:31 of ap->flags are reserved for LLD specific flags */


	/* struct ata_port pflags */
	ATA_PFLAG_EH_PENDING	= (1 << 0), /* EH pending */
	ATA_PFLAG_EH_IN_PROGRESS = (1 << 1), /* EH in progress */
	ATA_PFLAG_FROZEN	= (1 << 2), /* port is frozen */
	ATA_PFLAG_RECOVERED	= (1 << 3), /* recovery action performed */
	ATA_PFLAG_LOADING	= (1 << 4), /* boot/loading probe */
	ATA_PFLAG_SCSI_HOTPLUG	= (1 << 6), /* SCSI hotplug scheduled */
	ATA_PFLAG_INITIALIZING	= (1 << 7), /* being initialized, don't touch */
	ATA_PFLAG_RESETTING	= (1 << 8), /* reset in progress */
	ATA_PFLAG_UNLOADING	= (1 << 9), /* driver is being unloaded */
	ATA_PFLAG_UNLOADED	= (1 << 10), /* driver is unloaded */

	ATA_PFLAG_SUSPENDED	= (1 << 17), /* port is suspended (power) */
	ATA_PFLAG_PM_PENDING	= (1 << 18), /* PM operation pending */
	ATA_PFLAG_INIT_GTM_VALID = (1 << 19), /* initial gtm data valid */

	ATA_PFLAG_PIO32		= (1 << 20),  /* 32bit PIO */
	ATA_PFLAG_PIO32CHANGE	= (1 << 21),  /* 32bit PIO can be turned on/off */

	/* struct ata_queued_cmd flags */
	ATA_QCFLAG_ACTIVE	= (1 << 0), /* cmd not yet ack'd to scsi lyer */
	ATA_QCFLAG_DMAMAP	= (1 << 1), /* SG table is DMA mapped */
	ATA_QCFLAG_IO		= (1 << 3), /* standard IO command */
	ATA_QCFLAG_RESULT_TF	= (1 << 4), /* result TF requested */
	ATA_QCFLAG_CLEAR_EXCL	= (1 << 5), /* clear excl_link on completion */
	ATA_QCFLAG_QUIET	= (1 << 6), /* don't report device error */
	ATA_QCFLAG_RETRY	= (1 << 7), /* retry after failure */

	ATA_QCFLAG_FAILED	= (1 << 16), /* cmd failed and is owned by EH */
	ATA_QCFLAG_SENSE_VALID	= (1 << 17), /* sense data valid */
	ATA_QCFLAG_EH_SCHEDULED = (1 << 18), /* EH scheduled (obsolete) */

	/* host set flags */
	ATA_HOST_SIMPLEX	= (1 << 0),	/* Host is simplex, one DMA channel per host only */
	ATA_HOST_STARTED	= (1 << 1),	/* Host started */
	ATA_HOST_IGNORE_ATA	= (1 << 3),	/* Ignore ATA devices on this host. */
	ATA_TMOUT_INTERNAL_QUICK = 5000,

	/* If PMP is supported, we have to do follow-up SRST.  As some
	 * PMPs don't send D2H Reg FIS after hardreset, LLDs are
	 * advised to wait only for the following duration before
	 * doing SRST.
	 */
	ATA_TMOUT_PMP_SRST_WAIT	= 5000,

	/* SATA port states */
	PORT_UNKNOWN		= 0,
	PORT_ENABLED		= 1,
	PORT_DISABLED		= 2,

	/* encoding various smaller bitmaps into a single
	 * unsigned long bitmap
	 */
	ATA_NR_PIO_MODES	= 7,
	ATA_NR_MWDMA_MODES	= 5,
	ATA_NR_UDMA_MODES	= 8,

	ATA_SHIFT_PIO		= 0,
	ATA_SHIFT_MWDMA		= ATA_SHIFT_PIO + ATA_NR_PIO_MODES,
	ATA_SHIFT_UDMA		= ATA_SHIFT_MWDMA + ATA_NR_MWDMA_MODES,

	/* size of buffer to pad xfers ending on unaligned boundaries */
	ATA_DMA_PAD_SZ		= 4,

	/* ering size */
	ATA_ERING_SIZE		= 32,

	/* return values for ->qc_defer */
	ATA_DEFER_LINK		= 1,
	ATA_DEFER_PORT		= 2,

	/* desc_len for ata_eh_info and context */
	ATA_EH_DESC_LEN		= 80,

	/* reset / recovery action types */
	ATA_EH_REVALIDATE	= (1 << 0),
	ATA_EH_SOFTRESET	= (1 << 1), /* meaningful only in ->prereset */
	ATA_EH_HARDRESET	= (1 << 2), /* meaningful only in ->prereset */
	ATA_EH_RESET		= ATA_EH_SOFTRESET | ATA_EH_HARDRESET,
	ATA_EH_ENABLE_LINK	= (1 << 3),
	ATA_EH_PARK		= (1 << 5), /* unload heads and stop I/O */

	ATA_EH_PERDEV_MASK	= ATA_EH_REVALIDATE | ATA_EH_PARK,
	ATA_EH_ALL_ACTIONS	= ATA_EH_REVALIDATE | ATA_EH_RESET |
				  ATA_EH_ENABLE_LINK,

	/* ata_eh_info->flags */
	ATA_EHI_HOTPLUGGED	= (1 << 0),  /* could have been hotplugged */
	ATA_EHI_NO_AUTOPSY	= (1 << 2),  /* no autopsy */
	ATA_EHI_QUIET		= (1 << 3),  /* be quiet */
	ATA_EHI_NO_RECOVERY	= (1 << 4),  /* no recovery */

	ATA_EHI_DID_SOFTRESET	= (1 << 16), /* already soft-reset this port */
	ATA_EHI_DID_HARDRESET	= (1 << 17), /* already soft-reset this port */
	ATA_EHI_PRINTINFO	= (1 << 18), /* print configuration info */
	ATA_EHI_SETMODE		= (1 << 19), /* configure transfer mode */
	ATA_EHI_POST_SETMODE	= (1 << 20), /* revalidating after setmode */

	ATA_EHI_DID_RESET	= ATA_EHI_DID_SOFTRESET | ATA_EHI_DID_HARDRESET,

	/* mask of flags to transfer *to* the slave link */
	ATA_EHI_TO_SLAVE_MASK	= ATA_EHI_NO_AUTOPSY | ATA_EHI_QUIET,

	/* max tries if error condition is still set after ->error_handler */
	ATA_EH_MAX_TRIES	= 5,

	/* sometimes resuming a link requires several retries */
	ATA_LINK_RESUME_TRIES	= 5,

	/* how hard are we gonna try to probe/recover devices */
	ATA_PROBE_MAX_TRIES	= 3,
	ATA_EH_DEV_TRIES	= 3,
	ATA_EH_PMP_TRIES	= 5,
	ATA_EH_PMP_LINK_TRIES	= 3,

	SATA_PMP_RW_TIMEOUT	= 3000,		/* PMP read/write timeout */

	/* This should match the actual table size of
	 * ata_eh_cmd_timeout_table in libata-eh.c.
	 */
	ATA_EH_CMD_TIMEOUT_TABLE_SIZE = 6,
};

enum ata_xfer_mask {
	ATA_MASK_PIO		= ((1LU << ATA_NR_PIO_MODES) - 1)
					<< ATA_SHIFT_PIO,
	ATA_MASK_MWDMA		= ((1LU << ATA_NR_MWDMA_MODES) - 1)
					<< ATA_SHIFT_MWDMA,
	ATA_MASK_UDMA		= ((1LU << ATA_NR_UDMA_MODES) - 1)
					<< ATA_SHIFT_UDMA,
};

enum ata_completion_errors {
	AC_ERR_DEV		= (1 << 0), /* device reported error */
	AC_ERR_HSM		= (1 << 1), /* host state machine violation */
	AC_ERR_TIMEOUT		= (1 << 2), /* timeout */
	AC_ERR_MEDIA		= (1 << 3), /* media error */
	AC_ERR_ATA_BUS		= (1 << 4), /* ATA bus error */
	AC_ERR_HOST_BUS		= (1 << 5), /* host bus error */
	AC_ERR_SYSTEM		= (1 << 6), /* system error */
	AC_ERR_INVALID		= (1 << 7), /* invalid argument */
	AC_ERR_OTHER		= (1 << 8), /* unknown */
	AC_ERR_NODEV_HINT	= (1 << 9), /* polling device detection hint */
	AC_ERR_NCQ		= (1 << 10), /* marker for offending NCQ qc */
};

enum ata_tf_protocols {
	/* ATA taskfile protocols */
	ATA_PROT_UNKNOWN,	/* unknown/invalid */
	ATA_PROT_NODATA,	/* no data */
	ATA_PROT_PIO,		/* PIO data xfer */
	ATA_PROT_DMA,		/* DMA */
	ATA_PROT_NCQ,		/* NCQ */
	ATAPI_PROT_NODATA,	/* packet command, no data */
	ATAPI_PROT_PIO,		/* packet command, PIO data xfer*/
	ATAPI_PROT_DMA,		/* packet command with special DMA sauce */
};

enum ata_ioctls {
	ATA_IOC_GET_IO32	= 0x309,
	ATA_IOC_SET_IO32	= 0x324,
};

enum {
	/* flags for ata_dev_read_id() */
	ATA_READID_POSTRESET	= (1 << 0), /* reading ID after reset */
};

struct ata_link;
struct ata_queued_cmd;

/* typedefs */
typedef void (*ata_qc_cb_t) (struct ata_queued_cmd *qc);
typedef int (*ata_prereset_fn_t)(struct ata_link *link, unsigned long deadline);
typedef int (*ata_reset_fn_t)(struct ata_link *link, unsigned int *classes,
			      unsigned long deadline);
typedef void (*ata_postreset_fn_t)(struct ata_link *link, unsigned int *classes);


struct ata_taskfile {
	unsigned long		flags;		/* ATA_TFLAG_xxx */
	u8			protocol;	/* ATA_PROT_xxx */

	u8			ctl;		/* control reg */

	u8			hob_feature;	/* additional data */
	u8			hob_nsect;	/* to support LBA48 */
	u8			hob_lbal;
	u8			hob_lbam;
	u8			hob_lbah;

	u8			feature;
	u8			nsect;
	u8			lbal;
	u8			lbam;
	u8			lbah;

	u8			device;

	u8			command;	/* IO operation */
};

struct ata_port;
struct ata_port_operations;

struct ata_device {
	struct ata_link		*link;
	struct ata_device	*next, *prev;
	unsigned int		devno;		/* 0 or 1 */
	unsigned int		horkage;	/* List of broken features */
	unsigned long		flags;		/* ATA_DFLAG_xxx */
	void			*private_data;
	u64			n_sectors;	/* size of device, if ATA */
	u64			n_native_sectors; /* native size, if ATA */
	unsigned int		class;		/* ATA_DEV_xxx */

	u8			pio_mode;
	u8			dma_mode;
	u8			xfer_mode;
	unsigned int		xfer_shift;	/* ATA_SHIFT_xxx */

	unsigned int		multi_count;	/* sector count for
						   READ/WRITE MULTIPLE */
	unsigned int		max_sectors;	/* per-device max sectors */
	unsigned int		cdb_len;

	/* per-dev xfer mask */
	unsigned long		pio_mask;
	unsigned long		mwdma_mask;
	unsigned long		udma_mask;

	/* for CHS addressing */
	u16			cylinders;	/* Number of cylinders */
	u16			heads;		/* Number of heads */
	u16			sectors;	/* Number of sectors per track */

	union {
		u16		id[ATA_ID_WORDS]; /* IDENTIFY xxx DEVICE data */
		u32		gscr[SATA_PMP_GSCR_DWORDS]; /* PMP GSCR block */
	};
	char			end;
};

/* Fields between ATA_DEVICE_CLEAR_BEGIN and ATA_DEVICE_CLEAR_END are
 * cleared to zero on ata_dev_init().
 */
#define ATA_DEVICE_CLEAR_BEGIN		offsetof(struct ata_device, n_sectors)
#define ATA_DEVICE_CLEAR_END		offsetof(struct ata_device, end)

struct ata_host {
	unsigned int		n_ports;
	int			index;
	void			*private_data;
	struct ata_port_operations *ops;
	unsigned long		flags;
#ifdef CONFIG_PCI
	pci_dev_t		pdev;
#endif
	struct ata_port		*ports[0];
};

struct ata_queued_cmd {
	struct ata_port		*ap;
	struct ata_device	*dev;
	struct ata_taskfile	tf;
	u8			cdb[ATAPI_CDB_LEN];

	unsigned long		flags;
	unsigned int		n_elem;
	unsigned int		orig_n_elem;

	int			dma_dir;

	unsigned int		sect_size;
	unsigned int		nbytes;
	unsigned int		extrabytes;
	unsigned int		curbytes;

	void			*dataptr;	/* Instead of scatter list */

	struct ata_taskfile	result_tf;
	void			*private_data;
};

struct ata_eh_info {
	struct ata_device	*dev;		/* offending device */
	u32			serror;		/* SError from LLDD */
	unsigned int		err_mask;	/* port-wide err_mask */
	unsigned int		action;		/* ATA_EH_* action mask */
	unsigned int		dev_action[ATA_MAX_DEVICES]; /* dev EH action */
	unsigned int		flags;		/* ATA_EHI_* flags */

	unsigned int		probe_mask;

	char			desc[ATA_EH_DESC_LEN];
	int			desc_len;
};

struct ata_eh_context {
	struct ata_eh_info	i;
	int			tries[ATA_MAX_DEVICES];
	int			cmd_timeout_idx[ATA_MAX_DEVICES]
					       [ATA_EH_CMD_TIMEOUT_TABLE_SIZE];
	unsigned int		classes[ATA_MAX_DEVICES];
	unsigned int		did_probe_mask;
	unsigned int		unloaded_mask;
	unsigned int		saved_ncq_enabled;
	u8			saved_xfer_mode[ATA_MAX_DEVICES];
	/* timestamp for the last reset attempt or success */
	unsigned long		last_reset;
};

struct ata_link {
	struct ata_port		*ap;
	int			pmp;		/* port multiplier port # */
	u32			sactive;	/* active NCQ commands */
	unsigned int		flags;		/* ATA_LFLAG_xxx */
	u32			saved_scontrol;	/* SControl on probe */
	unsigned int		hw_sata_spd_limit;
	unsigned int		sata_spd_limit;
	unsigned int		sata_spd;

	/* record runtime error info, protected by host_set lock */
	struct ata_eh_info	eh_info;
	/* EH context */
	struct ata_eh_context	eh_context;

	struct ata_device	device[ATA_MAX_DEVICES];
};
#define ATA_LINK_CLEAR_BEGIN		offsetof(struct ata_link, sactive)
#define ATA_LINK_CLEAR_END		offsetof(struct ata_link, device[0])

struct ata_port {
	struct ata_port_operations *ops;
	unsigned long		flags;	/* ATA_FLAG_xxx */
	unsigned int		pflags;	/* ATA_PFLAG_xxx */
	unsigned int		print_id; /* user visible unique port ID */
	unsigned int		local_port_no; /* host local port num */
	unsigned int		port_no; /* 0 base port no. inside the host */
	unsigned int		pio_mask;
	unsigned int		mwdma_mask;
	unsigned int		udma_mask;
	unsigned int		cbl;	/* cable type; ATA_CBL_xxx */

	int			nr_active_links; /* #links with active qcs */

	struct ata_link		link;		/* host default link */
	struct ata_link		*slave_link;

	int			nr_pmp_links;	/* nr of available PMP links */
	struct ata_link		*pmp_link;	/* array of PMP links */
	struct ata_link		*excl_link;	/* For PMP qc exclusion */

	struct ata_host		*host;
	int			em_message_type;
	void			*private_data;

	u8			sector_buf[ATA_SECT_SIZE]
			__attribute__((aligned(CONFIG_SYS_CACHELINE_SIZE)));
};

struct ata_port_operations {
	unsigned int (*read_id)(struct ata_device *dev, struct ata_taskfile *tf,
				u16 *id);
	void (*dev_config)(struct ata_device *dev);
	ata_prereset_fn_t	prereset;
	ata_reset_fn_t		softreset;
	ata_reset_fn_t		hardreset;
	ata_postreset_fn_t	postreset;
	ata_prereset_fn_t	pmp_prereset;
	ata_reset_fn_t		pmp_softreset;
	ata_reset_fn_t		pmp_hardreset;
	ata_postreset_fn_t	pmp_postreset;
	void (*error_handler)(struct ata_port *ap);

	/*
	 * Optional features
	 */
	int (*scr_read)(struct ata_link *link, unsigned int sc_reg, u32 *val);
	int (*scr_write)(struct ata_link *link, unsigned int sc_reg, u32 val);
	void (*pmp_attach)(struct ata_port *ap);
	void (*pmp_detach)(struct ata_port *ap);

	/*
	 * Start and stop
	 */
	int (*port_start)(struct ata_port *ap);
	void (*port_stop)(struct ata_port *ap);
	void (*host_stop)(struct ata_host *host);

	void (*phy_reset)(struct ata_port *ap);
	int (*add_pmp_attached_dev)(struct ata_link *link);

	const struct ata_port_operations	*inherits;
};

struct ata_port_info {
	unsigned long		flags;
	unsigned long		link_flags;
	unsigned long		pio_mask;
	unsigned long		mwdma_mask;
	unsigned long		udma_mask;
	struct ata_port_operations *port_ops;
	void 			*private_data;
};

extern const unsigned long sata_deb_timing_normal[];
extern const unsigned long sata_deb_timing_hotplug[];
extern const unsigned long sata_deb_timing_long[];

extern struct ata_port_operations ata_dummy_port_ops;
extern const struct ata_port_info ata_dummy_port_info;

static inline const unsigned long *
sata_ehc_deb_timing(void)
{
	return sata_deb_timing_normal;
}

static inline int ata_port_is_dummy(struct ata_port *ap)
{
	return ap->ops == &ata_dummy_port_ops;
}

extern int sata_set_spd(struct ata_link *link);
extern int ata_std_prereset(struct ata_link *link, unsigned long deadline);
extern int ata_wait_after_reset(struct ata_link *link, unsigned long deadline,
				int (*check_ready)(struct ata_link *link));
extern int sata_link_resume(struct ata_link *link, const unsigned long *params,
			    unsigned long deadline);
extern int sata_link_hardreset(struct ata_link *link,
			const unsigned long *timing, unsigned long deadline,
			bool *online, int (*check_ready)(struct ata_link *));
extern int sata_std_hardreset(struct ata_link *link, unsigned int *class,
			      unsigned long deadline);
extern void ata_std_postreset(struct ata_link *link, unsigned int *classes);
extern int sata_scr_valid(struct ata_link *link);
extern int sata_scr_read(struct ata_link *link, int reg, u32 *val);
extern int sata_scr_write(struct ata_link *link, int reg, u32 val);
extern int sata_scr_write_flush(struct ata_link *link, int reg, u32 val);

/*
 * device helpers
 */
static inline unsigned int ata_class_enabled(unsigned int class)
{
	return class == ATA_DEV_ATA || class == ATA_DEV_ATAPI ||
		class == ATA_DEV_PMP || class == ATA_DEV_SEMB;
}

static inline unsigned int ata_class_disabled(unsigned int class)
{
	return class == ATA_DEV_ATA_UNSUP || class == ATA_DEV_ATAPI_UNSUP ||
		class == ATA_DEV_PMP_UNSUP || class == ATA_DEV_SEMB_UNSUP;
}

static inline unsigned int ata_class_absent(unsigned int class)
{
	return !ata_class_enabled(class) && !ata_class_disabled(class);
}

static inline unsigned int ata_dev_enabled(const struct ata_device *dev)
{
	return ata_class_enabled(dev->class);
}

static inline unsigned int ata_dev_disabled(const struct ata_device *dev)
{
	return ata_class_disabled(dev->class);
}

static inline unsigned int ata_dev_absent(const struct ata_device *dev)
{
	return ata_class_absent(dev->class);
}

/*
 * protocol tests
 */
static inline unsigned int ata_prot_flags(u8 prot)
{
	switch (prot) {
	case ATA_PROT_NODATA:
		return 0;
	case ATA_PROT_PIO:
		return ATA_PROT_FLAG_PIO;
	case ATA_PROT_DMA:
		return ATA_PROT_FLAG_DMA;
	case ATA_PROT_NCQ:
		return ATA_PROT_FLAG_DMA | ATA_PROT_FLAG_NCQ;
	case ATAPI_PROT_NODATA:
		return ATA_PROT_FLAG_ATAPI;
	case ATAPI_PROT_PIO:
		return ATA_PROT_FLAG_ATAPI | ATA_PROT_FLAG_PIO;
	case ATAPI_PROT_DMA:
		return ATA_PROT_FLAG_ATAPI | ATA_PROT_FLAG_DMA;
	}
	return 0;
}

static inline bool ata_is_atapi(u8 prot)
{
	return ata_prot_flags(prot) & ATA_PROT_FLAG_ATAPI;
}

static inline bool ata_is_nodata(u8 prot)
{
	return !(ata_prot_flags(prot) & ATA_PROT_FLAG_DATA);
}

static inline bool ata_is_pio(u8 prot)
{
	return ata_prot_flags(prot) & ATA_PROT_FLAG_PIO;
}

static inline bool ata_is_dma(u8 prot)
{
	return ata_prot_flags(prot) & ATA_PROT_FLAG_DMA;
}

static inline bool ata_is_ncq(u8 prot)
{
	return ata_prot_flags(prot) & ATA_PROT_FLAG_NCQ;
}

static inline bool ata_is_data(u8 prot)
{
	return ata_prot_flags(prot) & ATA_PROT_FLAG_DATA;
}

/*
 * id tests
 */
#define ata_id_is_ata(id)		(((id)[0] & (1 << 15)) == 0)
#define ata_id_has_lba(id)		((id)[49] & (1 << 9))
#define ata_id_has_dma(id)		((id)[49] & (1 << 8))
#define ata_id_has_ncq(id)		((id)[76] & (1 << 8))
#define ata_id_queue_depth(id)		(((id)[75] & 0x1f) + 1)
#define ata_id_removeable(id)		((id)[0] & (1 << 7))
#define ata_id_iordy_disable(id)	((id)[49] & (1 << 10))
#define ata_id_has_iordy(id)		((id)[49] & (1 << 11))

#define ata_id_u32(id,n)	\
	(((u32) (id)[(n) + 1] << 16) | ((u32) (id)[(n)]))
#define ata_id_u64(id,n)	\
	( ((u64) (id)[(n) + 3] << 48) | \
	  ((u64) (id)[(n) + 2] << 32) | \
	  ((u64) (id)[(n) + 1] << 16) | \
	  ((u64) (id)[(n) + 0]) )

#define ata_id_cdb_intr(id)		(((id)[0] & 0x60) == 0x20)

static inline bool ata_id_has_fua(const u16 *id)
{
	if ((id[84] & 0xC000) != 0x4000)
		return 0;
	return id[84] & (1 << 6);
}

static inline bool ata_id_has_flush(const u16 *id)
{
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	return id[83] & (1 << 12);
}

static inline bool ata_id_has_flush_ext(const u16 *id)
{
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	return id[83] & (1 << 13);
}

static inline bool ata_id_has_lba48(const u16 *id)
{
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	if (!ata_id_u64(id, 100))
		return 0;
	return id[83] & (1 << 10);
}

static inline bool ata_id_lba48_enabled(const u16 *id)
{
	if (ata_id_has_lba48(id) == 0)
		return 0;
	if ((id[87] & 0xC000) != 0x4000)
		return 0;
	return id[86] & (1 << 10);
}

static inline bool ata_id_hpa_enabled(const u16 *id)
{
	/* Yes children, word 83 valid bits cover word 82 data */
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	/* And 87 covers 85-87 */
	if ((id[87] & 0xC000) != 0x4000)
		return 0;
	/* Check command sets enabled as well as supported */
	if ((id[85] & ( 1 << 10)) == 0)
		return 0;
	return id[82] & (1 << 10);
}

static inline bool ata_id_has_wcache(const u16 *id)
{
	/* Yes children, word 83 valid bits cover word 82 data */
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	return id[82] & (1 << 5);
}

static inline bool ata_id_has_pm(const u16 *id)
{
	if ((id[83] & 0xC000) != 0x4000)
		return 0;
	return id[82] & (1 << 3);
}

static inline bool ata_id_rahead_enabled(const u16 *id)
{
	if ((id[87] & 0xC000) != 0x4000)
		return 0;
	return id[85] & (1 << 6);
}

static inline bool ata_id_wcache_enabled(const u16 *id)
{
	if ((id[87] & 0xC000) != 0x4000)
		return 0;
	return id[85] & (1 << 5);
}

static inline unsigned int ata_id_major_version(const u16 *id)
{
	unsigned int mver;

	if (id[ATA_ID_MAJOR_VER] == 0xFFFF)
		return 0;

	for (mver = 14; mver >= 1; mver--)
		if (id[ATA_ID_MAJOR_VER] & (1 << mver))
			break;
	return mver;
}

static inline bool ata_id_is_sata(const u16 *id)
{
	return ata_id_major_version(id) >= 5 && id[93] == 0;
}

static inline bool ata_id_has_tpm(const u16 *id)
{
	/* The TPM bits are only valid on ATA8 */
	if (ata_id_major_version(id) < 8)
		return 0;
	if ((id[48] & 0xC000) != 0x4000)
		return 0;
	return id[48] & (1 << 0);
}

static inline bool ata_id_has_dword_io(const u16 *id)
{
	/* ATA 8 reuses this flag for "trusted" computing */
	if (ata_id_major_version(id) > 7)
		return 0;
	if (id[48] & (1 << 0))
		return 1;
	return 0;
}

static inline bool ata_id_current_chs_valid(const u16 *id)
{
	/* For ATA-1 devices, if the INITIALIZE DEVICE PARAMETERS command
	   has not been issued to the device then the values of
	   id[54] to id[56] are vendor specific. */
	return (id[53] & 0x01) && /* Current translation valid */
		id[54] &&  /* cylinders in current translation */
		id[55] &&  /* heads in current translation */
		id[55] <= 16 &&
		id[56];    /* sectors in current translation */
}

static inline bool ata_id_is_cfa(const u16 *id)
{
	u16 v = id[0];
	if (v == 0x848A)	/* Standard CF */
		return 1;
	/* Could be CF hiding as standard ATA */
	if (ata_id_major_version(id) >= 3 &&  id[82] != 0xFFFF &&
			(id[82] & ( 1 << 2)))
		return 1;
	return 0;
}

static inline bool ata_drive_40wire(const u16 *dev_id)
{
	if (ata_id_is_sata(dev_id))
		return 0;	/* SATA */
	if ((dev_id[93] & 0xE000) == 0x6000)
		return 0;	/* 80 wire */
	return 1;
}

static inline bool ata_drive_40wire_relaxed(const u16 *dev_id)
{
	if ((dev_id[93] & 0x2000) == 0x2000)
		return 0;	/* 80 wire */
	return 1;
}

static inline int atapi_cdb_len(const u16 *dev_id)
{
	u16 tmp = dev_id[0] & 0x3;
	switch (tmp) {
	case 0:		return 12;
	case 1:		return 16;
	default:	return -1;
	}
}

static inline int atapi_command_packet_set(const u16 *dev_id)
{
	return (dev_id[0] >> 8) & 0x1f;
}

static inline int atapi_id_dmadir(const u16 *dev_id)
{
	return ata_id_major_version(dev_id) >= 7 && (dev_id[62] & 0x8000);
}

static inline int is_multi_taskfile(struct ata_taskfile *tf)
{
	return (tf->command == ATA_CMD_READ_MULTI) ||
	       (tf->command == ATA_CMD_WRITE_MULTI) ||
	       (tf->command == ATA_CMD_READ_MULTI_EXT) ||
	       (tf->command == ATA_CMD_WRITE_MULTI_EXT) ||
	       (tf->command == ATA_CMD_WRITE_MULTI_FUA_EXT);
}

static inline bool ata_ok(u8 status)
{
	return ((status & (ATA_BUSY | ATA_DRDY | ATA_DF | ATA_DRQ | ATA_ERR))
			== ATA_DRDY);
}

static inline bool lba_28_ok(u64 block, u32 n_block)
{
	/* check the ending block number */
	return ((block + n_block - 1) < ((u64)1 << 28)) && (n_block <= 256);
}

static inline bool lba_48_ok(u64 block, u32 n_block)
{
	/* check the ending block number */
	return ((block + n_block - 1) < ((u64)1 << 48)) && (n_block <= 65536);
}

#ifdef CONFIG_SATA_PMP
static inline bool sata_pmp_supported(struct ata_port *ap)
{
	return ap->flags & ATA_FLAG_PMP;
}

static inline bool sata_pmp_attached(struct ata_port *ap)
{
	return ap->nr_pmp_links != 0;
}

static inline bool ata_is_host_link(const struct ata_link *link)
{
	return link == &link->ap->link || link == link->ap->slave_link;
}
#else /* CONFIG_SATA_PMP */
static inline bool sata_pmp_supported(struct ata_port *ap)
{
	return 0;
}

static inline bool sata_pmp_attached(struct ata_port *ap)
{
	return 0;
}

static inline bool ata_is_host_link(const struct ata_link *link)
{
	return 1;
}
#endif /* CONFIG_SATA_PMP */

static inline int sata_srst_pmp(struct ata_link *link)
{
	if (sata_pmp_supported(link->ap) && ata_is_host_link(link))
		return SATA_PMP_CTRL_PORT;
	return link->pmp;
}

/*
 * printk helpers
 */
__printf(3, 4)
int ata_port_printk(const struct ata_port *ap, const char *level,
		    const char *fmt, ...);
__printf(3, 4)
int ata_link_printk(const struct ata_link *link, const char *level,
		    const char *fmt, ...);
__printf(3, 4)
int ata_dev_printk(const struct ata_device *dev, const char *level,
		   const char *fmt, ...);
#define ata_port_err(ap, fmt, ...)				\
	ata_port_printk(ap, "\001" "3", fmt, ##__VA_ARGS__)
#define ata_port_warn(ap, fmt, ...)				\
	ata_port_printk(ap, "\001" "4", fmt, ##__VA_ARGS__)
#define ata_port_notice(ap, fmt, ...)				\
	ata_port_printk(ap, "\001" "5", fmt, ##__VA_ARGS__)
#define ata_port_info(ap, fmt, ...)				\
	ata_port_printk(ap, "\001" "6", fmt, ##__VA_ARGS__)
#define ata_port_dbg(ap, fmt, ...)				\
	ata_port_printk(ap, "\001" "7", fmt, ##__VA_ARGS__)

#define ata_link_err(link, fmt, ...)				\
	ata_link_printk(link, "\001" "3", fmt, ##__VA_ARGS__)
#define ata_link_warn(link, fmt, ...)				\
	ata_link_printk(link, "\001" "4", fmt, ##__VA_ARGS__)
#define ata_link_notice(link, fmt, ...)				\
	ata_link_printk(link, "\001" "5", fmt, ##__VA_ARGS__)
#define ata_link_info(link, fmt, ...)				\
	ata_link_printk(link, "\001" "6", fmt, ##__VA_ARGS__)
#define ata_link_dbg(link, fmt, ...)				\
	ata_link_printk(link, "\001" "7", fmt, ##__VA_ARGS__)

#define ata_dev_err(dev, fmt, ...)				\
	ata_dev_printk(dev, "\001" "3", fmt, ##__VA_ARGS__)
#define ata_dev_warn(dev, fmt, ...)				\
	ata_dev_printk(dev, "\001" "4", fmt, ##__VA_ARGS__)
#define ata_dev_notice(dev, fmt, ...)				\
	ata_dev_printk(dev, "\001" "5", fmt, ##__VA_ARGS__)
#define ata_dev_info(dev, fmt, ...)				\
	ata_dev_printk(dev, "\001" "6", fmt, ##__VA_ARGS__)
#define ata_dev_dbg(dev, fmt, ...)				\
	ata_dev_printk(dev, "\001" "7", fmt, ##__VA_ARGS__)

/*
 * ata_eh_info helpers
 */
extern __printf(2, 3)
void __ata_ehi_push_desc(struct ata_eh_info *ehi, const char *fmt, ...);
extern __printf(2, 3)
void ata_ehi_push_desc(struct ata_eh_info *ehi, const char *fmt, ...);
extern void ata_ehi_clear_desc(struct ata_eh_info *ehi);

static inline void ata_ehi_hotplugged(struct ata_eh_info *ehi)
{
	ehi->probe_mask |= (1 << ATA_MAX_DEVICES) - 1;
	ehi->flags |= ATA_EHI_HOTPLUGGED;
	ehi->action |= ATA_EH_RESET | ATA_EH_ENABLE_LINK;
	ehi->err_mask |= AC_ERR_ATA_BUS;
}

/*
 * link helpers
 */
static inline int ata_link_max_devices(const struct ata_link *link)
{
	if (ata_is_host_link(link) && link->ap->flags & ATA_FLAG_SLAVE_POSS)
		return 2;
	return 1;
}

/*
 * Iterators
 *
 * ATA_LITER_* constants are used to select link iteration mode and
 * ATA_DITER_* device iteration mode.
 *
 * For a custom iteration directly using ata_{link|dev}_next(), if
 * @link or @dev, respectively, is NULL, the first element is
 * returned.  @dev and @link can be any valid device or link and the
 * next element according to the iteration mode will be returned.
 * After the last element, NULL is returned.
 */
enum ata_link_iter_mode {
	ATA_LITER_EDGE,		/* if present, PMP links only; otherwise,
				 * host link.  no slave link */
	ATA_LITER_HOST_FIRST,	/* host link followed by PMP or slave links */
	ATA_LITER_PMP_FIRST,	/* PMP links followed by host link,
				 * slave link still comes after host link */
};

enum ata_dev_iter_mode {
	ATA_DITER_ENABLED,
	ATA_DITER_ENABLED_REVERSE,
	ATA_DITER_ALL,
	ATA_DITER_ALL_REVERSE,
};

extern struct ata_link *ata_link_next(struct ata_link *link,
				      struct ata_port *ap,
				      enum ata_link_iter_mode mode);

extern struct ata_device *ata_dev_next(struct ata_device *dev,
				       struct ata_link *link,
				       enum ata_dev_iter_mode mode);

/*
 * Shortcut notation for iterations
 *
 * ata_for_each_link() iterates over each link of @ap according to
 * @mode.  @link points to the current link in the loop.  @link is
 * NULL after loop termination.  ata_for_each_dev() works the same way
 * except that it iterates over each device of @link.
 *
 * Note that the mode prefixes ATA_{L|D}ITER_ shouldn't need to be
 * specified when using the following shorthand notations.  Only the
 * mode itself (EDGE, HOST_FIRST, ENABLED, etc...) should be
 * specified.  This not only increases brevity but also makes it
 * impossible to use ATA_LITER_* for device iteration or vice-versa.
 */
#define ata_for_each_link(link, ap, mode) \
	for ((link) = ata_link_next(NULL, (ap), ATA_LITER_##mode); (link); \
	     (link) = ata_link_next((link), (ap), ATA_LITER_##mode))

#define ata_for_each_dev(dev, link, mode) \
	for ((dev) = ata_dev_next(NULL, (link), ATA_DITER_##mode); (dev); \
	     (dev) = ata_dev_next((dev), (link), ATA_DITER_##mode))

static inline void ata_tf_init(struct ata_device *dev, struct ata_taskfile *tf)
{
	memset(tf, 0, sizeof(*tf));

#ifdef CONFIG_ATA_SFF
	tf->ctl = dev->link->ap->ctl;
#else
	tf->ctl = ATA_DEVCTL_OBS;
#endif
	if (dev->devno == 0)
		tf->device = ATA_DEVICE_OBS;
	else
		tf->device = ATA_DEVICE_OBS | ATA_DEV1;
}

static inline int ata_check_ready(u8 status)
{
	if (!(status & ATA_BUSY))
		return 1;

	/* 0xff indicates either no device or device not ready */
	if (status == 0xff)
		return -1;

	return 0;
}

#define sata_pmp_gscr_vendor(gscr)	((gscr)[SATA_PMP_GSCR_PROD_ID] & 0xffff)
#define sata_pmp_gscr_devid(gscr)	((gscr)[SATA_PMP_GSCR_PROD_ID] >> 16)
#define sata_pmp_gscr_rev(gscr)		(((gscr)[SATA_PMP_GSCR_REV] >> 8) & 0xff)
#define sata_pmp_gscr_ports(gscr)	((gscr)[SATA_PMP_GSCR_PORT_INFO] & 0xf)

u64 ata_id_n_sectors(u16 *id);
unsigned int ata_dev_classify(const struct ata_taskfile *tf);
void ata_id_c_string(const u16 *id, unsigned char *s,
			 unsigned int ofs, unsigned int len);
extern unsigned int ata_dev_set_feature(struct ata_device *dev,
					u8 enable, u8 feature);
void ata_dump_id(u16 *id);
void swap_buf_le16(u16 *buf, unsigned int buf_words);
extern bool ata_phys_link_online(struct ata_link *link);
extern bool ata_phys_link_offline(struct ata_link *link);
extern void ata_msleep(struct ata_port *ap, unsigned int msecs);
u32 ata_wait_register(void *reg, u32 mask, u32 val, unsigned long timeout);
int atapi_cmd_type(u8 opcode);
void ata_tf_to_fis(const struct ata_taskfile *tf, u8 pmp, int is_cmd, u8 *fis);
void ata_tf_from_fis(const u8 *fis, struct ata_taskfile *tf);
int ata_wait_after_reset(struct ata_link *link, unsigned long deadline,
			 int (*check_ready)(struct ata_link *link));
extern void ata_link_init(struct ata_port *ap, struct ata_link *link, int pmp);
extern int sata_link_init_spd(struct ata_link *link);
struct ata_host *ata_host_alloc(int max_ports);
struct ata_host *ata_host_alloc_pinfo(const struct ata_port_info * const * ppi,
				      int n_ports);
extern struct ata_link *ata_dev_phys_link(struct ata_device *dev);
extern int ata_build_rw_tf(struct ata_taskfile *tf, struct ata_device *dev,
			   u64 block, u32 n_block, unsigned int tf_flags,
			   unsigned int tag);
int ata_exec_internal(struct ata_device *dev, struct ata_taskfile *tf,
		      const u8 *cdb, int dma_dir,
		      void *buf, unsigned int buflen,
		      unsigned long timeout);
extern int ata_wait_ready(struct ata_link *link, unsigned long deadline,
			  int (*check_ready)(struct ata_link *link));
extern int ata_dev_read_id(struct ata_device *dev, unsigned int *p_class,
			   unsigned int flags, u16 *id);
extern void ata_dev_init(struct ata_device *dev);
extern void ata_link_init(struct ata_port *ap, struct ata_link *link, int pmp);
extern void ata_dev_disable(struct ata_device *dev);


#ifdef CONFIG_SATA_PMP
extern int sata_pmp_scr_read(struct ata_link *link, int reg, u32 *val);
extern int sata_pmp_scr_write(struct ata_link *link, int reg, u32 val);
extern int sata_pmp_attach(struct ata_device *dev);

#else
static inline int sata_pmp_scr_read(struct ata_link *link, int reg, u32 *val)
{
	return -EINVAL;
}
static inline int sata_pmp_scr_write(struct ata_link *link, int reg, u32 val)
{
	return -EINVAL;
}
static inline int sata_pmp_attach(struct ata_device *dev)
{
	return -EINVAL;
}
#endif

int ata_eh_reset(struct ata_link *link, int classify,
		 ata_prereset_fn_t prereset, ata_reset_fn_t softreset,
		 ata_reset_fn_t hardreset, ata_postreset_fn_t postreset);

/**************************************************************************
 * PMP - drivers/ata/libata-pmp.c
 */
#ifdef CONFIG_SATA_PMP

extern const struct ata_port_operations sata_pmp_port_ops;

extern int sata_pmp_qc_defer_cmd_switch(struct ata_queued_cmd *qc);
extern void sata_pmp_error_handler(struct ata_port *ap);

#else /* CONFIG_SATA_PMP */

#define sata_pmp_port_ops		sata_port_ops
#define sata_pmp_qc_defer_cmd_switch	ata_std_qc_defer
#define sata_pmp_error_handler		ata_std_error_handler

#endif /* CONFIG_SATA_PMP */

#endif /* __LIBATA_H__ */
