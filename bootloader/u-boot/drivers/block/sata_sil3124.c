/*
 * sata_sil24.c - Driver for Silicon Image 3124/3132 SATA-2 controllers
 *
 * Copyright 2005  Tejun Heo
 *
 * Copyright 2011  Cavium, Inc.
 *
 * Based on preview driver from Silicon Image.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
#include <common.h>

#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <pci.h>
#include <ide.h>
#include <ata.h>
#include <libata.h>
#include <malloc.h>
#include <watchdog.h>

#define DRV_NAME	"sata_sil24"
#define DRV_VERSION	"0.23"

#define DMA_BASE_ADDR  		0
#define SATA_RESET_TIMEOUT	10000	/* 10 sec */

#define ATA_DFLAG_LBA48		(1 << 0)

#define ATA_SHIFT_UDMA		0
#define ATA_SHIFT_MWDMA		8
#define ATA_SHIFT_PIO		11

#define msleep(ms_data)	udelay((ms_data)*1000)
#define mdelay(ms_data)	udelay((ms_data)*1000)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define virt_to_bus(devno, v)	pci_virt_to_mem(devno, (void *) (v))
#define bus_to_phys(devno, a)	pci_mem_to_phys(devno, a)

#define ata_id_has_lba(id)	((id)[49] & (1 << 9))
#define ata_id_has_dma(id)	((id)[49] & (1 << 8))

extern block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

static phys_addr_t sata_cmd_addr[CONFIG_SYS_SATA_MAX_DEVICE];
static phys_addr_t sata_scr_addr[CONFIG_SYS_SATA_MAX_DEVICE];
static unsigned int xfer_modes[CONFIG_SYS_SATA_MAX_DEVICE];
static unsigned int lba64_mode[CONFIG_SYS_SATA_MAX_DEVICE];
static unsigned int sil24_flags[CONFIG_SYS_SATA_MAX_DEVICE];
static struct ata_taskfile sata_tf;

struct sil24_cmd_block *sata_cmd_block;
unsigned char *sata_private_data;
dma_addr_t sge_offset;


#if CONFIG_OCTEON
#include <asm/arch/octeon_pci.h>
#include <mtd/cfi_flash.h>
#endif

static inline void sil24_writel(uint32_t val, uint32_t addr)
{
	writel(val, (void *)addr);
}

static inline void sil24_writew(uint16_t val, uint32_t addr)
{
	writew(val, (void *)addr);
}

static inline u32 sil24_readl(uint32_t addr)
{
	return readl((void *)addr);
}

static inline u16 sil24_readw(uint32_t addr)
{
	return readw((void *)addr);
}

struct sil24_prb {
	u16 ctrl;
	u16 prot;
	u32 rx_cnt;
	u8 fis[6 * 4];
};

/*
 * Scatter gather entry (SGE) 16 bytes
 */
struct sil24_sge {
	u64 addr;
	u32 cnt;
	u32 flags;
};

/*
 * Port multiplier
 */
struct sil24_port_multiplier {
	u32 diag;
	u32 sactive;
};

enum {
	/*
	 * Global controller registers (128 bytes @ BAR0)
	 */
	/* 32 bit regs */
	HOST_SLOT_STAT		= 0x00,	/* 32 bit slot stat * 4 */
	HOST_CTRL		= 0x40,
	HOST_IRQ_STAT		= 0x44,
	HOST_PHY_CFG		= 0x48,
	HOST_BIST_CTRL		= 0x50,
	HOST_BIST_PTRN		= 0x54,
	HOST_BIST_STAT		= 0x58,
	HOST_MEM_BIST_STAT	= 0x5c,
	HOST_FLASH_CMD		= 0x70,
	/* 8 bit regs */
	HOST_FLASH_DATA		= 0x74,
	HOST_TRANSITION_DETECT	= 0x75,
	HOST_GPIO_CTRL		= 0x76,
	HOST_I2C_ADDR		= 0x78,	/* 32 bit */
	HOST_I2C_DATA		= 0x7c,
	HOST_I2C_XFER_CNT	= 0x7e,
	HOST_I2C_CTRL		= 0x7f,

	/* HOST_SLOT_STAT bits */
	HOST_SSTAT_ATTN		= (1 << 31),

	/*
	 * Port registers
	 * (8192 bytes @ +0x0000, +0x2000, +0x4000 and +0x6000 @ BAR2)
	 */
	PORT_REGS_SIZE		= 0x2000,
	PORT_LRAM		= 0x0000, /* 31 LRAM slots and PMP regs */
	PORT_LRAM_SLOT_SZ	= 0x0080, /* 32 bytes PRB + 2 SGE, ACT... */

	PORT_PMP		= 0x0f80, /* 8 bytes PM * 16 (128 bytes) */
	PORT_PMP_STATUS		= 0x0000, /* port device status offset */
	PORT_PMP_QACTIVE 	= 0x0004, /* port device QActive offset */
	PORT_PMP_SIZE		= 0x0008, /* 8 bytes per PMP */

	/* 32 bit regs */
	PORT_CTRL_STAT		= 0x1000, /* write: ctrl-set, read: stat */
	PORT_CTRL_CLR		= 0x1004, /* write: ctrl-clear */
	PORT_IRQ_STAT		= 0x1008, /* high: status, low: interrupt */
	PORT_IRQ_ENABLE_SET	= 0x1010, /* write: enable-set */
	PORT_IRQ_ENABLE_CLR	= 0x1014, /* write: enable-clear */
	PORT_ACTIVATE_UPPER_ADDR= 0x101c,
	PORT_EXEC_FIFO		= 0x1020, /* command execution fifo */
	PORT_CMD_ERR		= 0x1024, /* command error number */
	PORT_FIS_CFG		= 0x1028,
	PORT_FIFO_THRES		= 0x102c,
	/* 16 bit regs */
	PORT_DECODE_ERR_CNT	= 0x1040,
	PORT_DECODE_ERR_THRESH	= 0x1042,
	PORT_CRC_ERR_CNT	= 0x1044,
	PORT_CRC_ERR_THRESH	= 0x1046,
	PORT_HSHK_ERR_CNT	= 0x1048,
	PORT_HSHK_ERR_THRESH	= 0x104a,
	/* 32 bit regs */
	PORT_PHY_CFG		= 0x1050,
	PORT_SLOT_STAT		= 0x1800,
	PORT_CMD_ACTIVATE	= 0x1c00, /* 64 bit cmd activate * 31 (248 bytes) */
	PORT_CONTEXT		= 0x1e04,
	PORT_EXEC_DIAG		= 0x1e00, /* 32bit exec diag * 16 (64 bytes, 0-10 used on 3124) */
	PORT_PSD_DIAG		= 0x1e40, /* 32bit psd diag * 16 (64 bytes, 0-8 used on 3124) */
	PORT_SCONTROL		= 0x1f00,
	PORT_SSTATUS		= 0x1f04,
	PORT_SERROR		= 0x1f08,
	PORT_SACTIVE		= 0x1f0c,

	/* PORT_CTRL_STAT bits */
	PORT_CS_PORT_RST	= (1 << 0), /* port reset */
	PORT_CS_DEV_RST		= (1 << 1), /* device reset */
	PORT_CS_INIT		= (1 << 2), /* port initialize */
	PORT_CS_IRQ_WOC		= (1 << 3), /* interrupt write one to clear */
	PORT_CS_CDB16		= (1 << 5), /* 0=12b cdb, 1=16b cdb */
	PORT_CS_PMP_RESUME	= (1 << 6), /* port resume */
	PORT_CS_32BIT_ACTV	= (1 << 10), /* 32-bit activation */
	PORT_CS_PMP_EN		= (1 << 13), /* port multiplier enable */
	PORT_CS_RDY		= (1 << 31), /* port ready to accept commands */

	/* PORT_IRQ_STAT/ENABLE_SET/CLR */
	/* bits[11:0] are masked */
	PORT_IRQ_COMPLETE	= (1 << 0), /* command(s) completed */
	PORT_IRQ_ERROR		= (1 << 1), /* command execution error */
	PORT_IRQ_PORTRDY_CHG	= (1 << 2), /* port ready change */
	PORT_IRQ_PWR_CHG	= (1 << 3), /* power management change */
	PORT_IRQ_PHYRDY_CHG	= (1 << 4), /* PHY ready change */
	PORT_IRQ_COMWAKE	= (1 << 5), /* COMWAKE received */
	PORT_IRQ_UNK_FIS	= (1 << 6), /* Unknown FIS received */
	PORT_IRQ_DEV_XCHG	= (1 << 7), /* device exchanged */
	PORT_IRQ_8B10B		= (1 << 8), /* 8b/10b decode error threshold */
	PORT_IRQ_CRC		= (1 << 9), /* CRC error threshold */
	PORT_IRQ_HANDSHAKE	= (1 << 10), /* handshake error threshold */
	PORT_IRQ_SDB_FIS	= (1 << 11), /* SDB FIS received */

	/* bits[27:16] are unmasked (raw) */
	PORT_IRQ_RAW_SHIFT	= 16,
	PORT_IRQ_MASKED_MASK	= 0x7ff,
	PORT_IRQ_RAW_MASK	= (0x7ff << PORT_IRQ_RAW_SHIFT),

	/* ENABLE_SET/CLR specific, intr steering - 2 bit field */
	PORT_IRQ_STEER_SHIFT	= 30,
	PORT_IRQ_STEER_MASK	= (3 << PORT_IRQ_STEER_SHIFT),

	/* PORT_CMD_ERR constants */
	PORT_CERR_DEV		= 1, /* Error bit in D2H Register FIS */
	PORT_CERR_SDB		= 2, /* Error bit in SDB FIS */
	PORT_CERR_DATA		= 3, /* Error in data FIS not detected by dev */
	PORT_CERR_SEND		= 4, /* Initial cmd FIS transmission failure */
	PORT_CERR_INCONSISTENT	= 5, /* Protocol mismatch */
	PORT_CERR_DIRECTION	= 6, /* Data direction mismatch */
	PORT_CERR_UNDERRUN	= 7, /* Ran out of SGEs while writing */
	PORT_CERR_OVERRUN	= 8, /* Ran out of SGEs while reading */
	PORT_CERR_PKT_PROT	= 11, /* DIR invalid in 1st PIO setup of ATAPI */
	PORT_CERR_SGT_BOUNDARY	= 16, /* PLD ecode 00 - SGT not on qword boundary */
	PORT_CERR_SGT_TGTABRT	= 17, /* PLD ecode 01 - target abort */
	PORT_CERR_SGT_MSTABRT	= 18, /* PLD ecode 10 - master abort */
	PORT_CERR_SGT_PCIPERR	= 19, /* PLD ecode 11 - PCI parity err while fetching SGT */
	PORT_CERR_CMD_BOUNDARY	= 24, /* ctrl[15:13] 001 - PRB not on qword boundary */
	PORT_CERR_CMD_TGTABRT	= 25, /* ctrl[15:13] 010 - target abort */
	PORT_CERR_CMD_MSTABRT	= 26, /* ctrl[15:13] 100 - master abort */
	PORT_CERR_CMD_PCIPERR	= 27, /* ctrl[15:13] 110 - PCI parity err while fetching PRB */
	PORT_CERR_XFR_UNDEF	= 32, /* PSD ecode 00 - undefined */
	PORT_CERR_XFR_TGTABRT	= 33, /* PSD ecode 01 - target abort */
	PORT_CERR_XFR_MSGABRT	= 34, /* PSD ecode 10 - master abort */
	PORT_CERR_XFR_PCIPERR	= 35, /* PSD ecode 11 - PCI prity err during transfer */
	PORT_CERR_SENDSERVICE	= 36, /* FIS received while sending service */

	/* bits of PRB control field */
	PRB_CTRL_PROTOCOL	= (1 << 0), /* override def. ATA protocol */
	PRB_CTRL_PACKET_READ	= (1 << 4), /* PACKET cmd read */
	PRB_CTRL_PACKET_WRITE	= (1 << 5), /* PACKET cmd write */
	PRB_CTRL_NIEN		= (1 << 6), /* Mask completion irq */
	PRB_CTRL_SRST		= (1 << 7), /* Soft reset request (ign BSY?) */

	/* PRB protocol field */
	PRB_PROT_PACKET		= (1 << 0),
	PRB_PROT_TCQ		= (1 << 1),
	PRB_PROT_NCQ		= (1 << 2),
	PRB_PROT_READ		= (1 << 3),
	PRB_PROT_WRITE		= (1 << 4),
	PRB_PROT_TRANSPARENT	= (1 << 5),

	/*
	 * Other constants
	 */
	SGE_TRM			= (1 << 31), /* Last SGE in chain */
	SGE_LNK			= (1 << 30), /* linked list
						Points to SGT, not SGE */
	SGE_DRD			= (1 << 29), /* discard data read (/dev/null)
						data address ignored */

	SIL24_MAX_CMDS		= 31,

	/* board id */
	BID_SIL3124		= 0,
	BID_SIL3132		= 1,
	BID_SIL3131		= 2,

	/* host flags */
	SIL24_COMMON_FLAGS	= 0,
	SIL24_FLAG_PCIX_IRQ_WOC	= (1 << 24), /* IRQ loss errata on PCI-X */

	IRQ_STAT_4PORTS		= 0xf,
};

struct sil24_ata_block {
	struct sil24_prb prb;
	struct sil24_sge sge[ATA_MAX_PRD / 2];
};

struct sil24_atapi_block {
	struct sil24_prb prb;
	u8 cdb[16];
	struct sil24_sge sge[(ATA_MAX_PRD / 2) - 1];
};

struct sil24_cmd_block {
	struct sil24_ata_block ata;
};

/* ap->host_set->private_data */
struct sil24_host_priv {
	unsigned int host_base;	/* global controller control (128 bytes @BAR0) */
	unsigned int port_base;	/* port registers (4 * 8192 bytes @BAR2) */
};

struct sil24_device_id {
	struct pci_device_id	dev_id;
	int			board_id;
};

static const struct sil24_device_id sil24_pci_tbl[] = {
	{ { PCI_VENDOR_ID_CMD, 0x3124 }, BID_SIL3124 },
	{ { PCI_VENDOR_ID_INTEL, 0x3124 }, BID_SIL3124 },
	{ { PCI_VENDOR_ID_CMD, 0x3132 }, BID_SIL3132 },
	{ { PCI_VENDOR_ID_CMD, 0x0242 }, BID_SIL3132 },
	{ { PCI_VENDOR_ID_CMD, 0x0244 }, BID_SIL3132 },
	{ { PCI_VENDOR_ID_CMD, 0x3131 }, BID_SIL3131 },
	{ { PCI_VENDOR_ID_CMD, 0x3531 }, BID_SIL3131 },

	{ } /* terminate list */
};


/*
 * Use bits 30-31 of host_flags to encode available port numbers.
 * Current maxium is 4.
 */
#define SIL24_NPORTS2FLAG(nports)	((((unsigned)(nports) - 1) & 0x3) << 30)
#define SIL24_FLAG2NPORTS(flag)		((((flag) >> 30) & 0x3) + 1)

struct sil24_port_info {
	ulong		flags;
};

static const struct sil24_port_info sil24_port_info[] = {
	/* sil_3124 */
	{
		.flags		= SIL24_COMMON_FLAGS | SIL24_NPORTS2FLAG(4) |
				  SIL24_FLAG_PCIX_IRQ_WOC,
	},
	/* sil_3132 */
	{
		.flags		= SIL24_COMMON_FLAGS | SIL24_NPORTS2FLAG(2),
	},
	/* sil_3131/sil_3531 */
	{
		.flags		= SIL24_COMMON_FLAGS | SIL24_NPORTS2FLAG(1),
	},
};

/**
 *	swap_buf_le16 -
 *	@buf:  Buffer to swap
 *	@buf_words:  Number of 16-bit words in buffer.
 *
 *	Swap halves of 16-bit words if needed to convert from
 *	little-endian byte order to native cpu byte order, or
 *	vice-versa.
 *
 *	LOCKING:
 */
static inline void swap_buf_le16(u16 * buf, unsigned int buf_words)
{
	unsigned int i;

	for (i = 0; i < buf_words; i++)
		buf[i] = le16_to_cpu(buf[i]);
}

/**
 *	ata_tf_to_fis - Convert ATA taskfile to SATA FIS structure
 *	@tf: Taskfile to convert
 *	@fis: Buffer into which data will output
 *	@pmp: Port multiplier port
 *
 *	Converts a standard ATA taskfile to a Serial ATA
 *	FIS structure (Register - Host to Device).
 *
 *	LOCKING:
 *	Inherited from caller.
 */

static void ata_tf_to_fis(struct ata_taskfile *tf, u8 * fis, u8 pmp)
{
	fis[0] = 0x27;		/* Register - Host to Device FIS */
	fis[1] = (pmp & 0xf) | (1 << 7);	/* Port multiplier number,
						   bit 7 indicates Command FIS */
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

	if (tf->command == ATA_CMD_ID_ATA)
		fis[12] = 0;	/* nsec = 0; */
}

static int sil24_scr_map[] = {
	[SCR_CONTROL] = 0,
	[SCR_STATUS] = 1,
	[SCR_ERROR] = 2,
	[SCR_ACTIVE] = 3,
};

#ifdef DEBUG
static void sil24_printprb(void)
{
	int i;
	debug("PRB:\n");
	debug("\tctrl: 0x%04x\n", sata_cmd_block->ata.prb.ctrl);
	debug("\tprot: 0x%04x\n", sata_cmd_block->ata.prb.prot);
	debug("\trx_cnt: 0x%x\n", sata_cmd_block->ata.prb.rx_cnt);
	debug("\tfis:");
	for (i = 0; i < 24; i++)
		debug(" %02x", sata_cmd_block->ata.prb.fis[i]);
	debug("\n");
}
#else
static inline void sil24_printprb(void)
{
	return;
}
#endif

static u32 sil24_scr_read(int dev_no, unsigned sc_reg)
{
	unsigned int scr_addr = sata_scr_addr[dev_no];
	if (sc_reg < ARRAY_SIZE(sil24_scr_map)) {
		unsigned int addr;
		addr = scr_addr + sil24_scr_map[sc_reg] * 4;
		return sil24_readl(scr_addr + sil24_scr_map[sc_reg] * 4);
	}
	return 0xffffffffU;
}

static void sil24_scr_write(int dev_no, unsigned sc_reg, u32 val)
{
	unsigned int scr_addr = sata_scr_addr[dev_no];
	if (sc_reg < ARRAY_SIZE(sil24_scr_map)) {
		unsigned int addr;
		addr = scr_addr + sil24_scr_map[sc_reg] * 4;
		sil24_writel(val, scr_addr + sil24_scr_map[sc_reg] * 4);
	}
}

static void scr_write_flush(int dev_no, unsigned int reg, u32 val)
{
	sil24_scr_write(dev_no, reg, val);
	sil24_scr_read(dev_no, reg);
}

static unsigned int sata_dev_present(int dev_no)
{
	return ((sil24_scr_read(dev_no, SCR_STATUS) & 0xf) == 0x3) ? 1 : 0;
}

static u32 sil24_wait_register(unsigned int port, unsigned int reg,
			u32 mask, u32 val, u32 interval, u32 timeout)
{
	u32 tmp;
	u32 max_count;

	tmp = sil24_readl(port + reg);
	if (!interval)
		interval = 1;

	max_count = timeout / interval;
	while ((tmp & mask) == val && max_count--) {
		msleep(interval);
		tmp = sil24_readl(port + reg);
	}
	return tmp;
}
static int sil24_init_port(int dev_no)
{
	unsigned int port = sata_cmd_addr[dev_no];
	u32 tmp;

	debug("%s: Initializing device %d\n", __func__, dev_no);
	sil24_writel(0x08ff, port + PORT_IRQ_ENABLE_CLR);
	sil24_writel(PORT_CS_INIT, port + PORT_CTRL_STAT);
	tmp = sil24_wait_register(port, PORT_CTRL_STAT, PORT_CS_RDY, 0, 10, 100);

	if ((tmp & (PORT_CS_INIT | PORT_CS_RDY)) != PORT_CS_RDY) {
		debug("%s: device %d not ready\n", __func__, dev_no);
		return -1;
	}
	return 0;
}

static int sil24_issue_SRST(int dev_no)
{
	u32 irq_enable, irq_stat, cmd_err;
	int cnt;
	unsigned int port = sata_cmd_addr[dev_no];
	struct sil24_prb *prb = &sata_cmd_block->ata.prb;
	dma_addr_t paddr = virt_to_bus(dev_no, prb);

	debug("%s: DMA address = 0x%llx\n", __func__, paddr);
	debug("%s: port status=0x%x\n", __func__,
	      sil24_readl(port + PORT_CTRL_STAT));

	/* temporarily turn off IRQs during SRST */
	irq_enable = sil24_readl(port + PORT_IRQ_ENABLE_SET);
	sil24_writel(irq_enable, port + PORT_IRQ_ENABLE_CLR);
	debug("%s: Wrote 0x%x to IRQ clear register\n", __func__, irq_enable);
	sil24_writel(0x08ff, port + PORT_IRQ_ENABLE_CLR);
	/*
	 * XXX: Not sure whether the following sleep is needed or not.
	 * The original driver had it.  So....
	 */
	sil24_init_port(dev_no);
	msleep(10);

	prb->ctrl = cpu_to_le16(PRB_CTRL_SRST);
	prb->fis[1] = 0;	/* no PM yet */

	msleep(100);

	/* Clear IRQ status */
	irq_stat = sil24_readl(port + PORT_IRQ_STAT);
	sil24_writel(irq_stat, port + PORT_IRQ_STAT);		/* clear irq */
	irq_stat >>= PORT_IRQ_RAW_SHIFT;

	if (sil24_flags[dev_no] & SIL24_FLAG_PCIX_IRQ_WOC)
		sil24_writel(PORT_CS_IRQ_WOC, port + PORT_CTRL_STAT);
	sil24_writel((u32) paddr, port + PORT_CMD_ACTIVATE);
	sil24_writel((u64)paddr >> 32, port + PORT_CMD_ACTIVATE + 4);

	msleep(1);
	/* wait 10 sec for Hitachi SATA II HD */
	for (cnt = 0; cnt < SATA_RESET_TIMEOUT; cnt++) {
		irq_stat = sil24_readl(port + PORT_IRQ_STAT);
		if (irq_stat)
			sil24_writel(irq_stat, port + PORT_IRQ_STAT);	/* clear irq */

		irq_stat >>= PORT_IRQ_RAW_SHIFT;
		if (irq_stat & (PORT_IRQ_COMPLETE | PORT_IRQ_ERROR)) {
			debug("%s: irq complete, status=0x%x\n",
			      __func__, irq_stat);;
			break;
		}
		msleep(1);
	}

	sil24_writel(irq_stat, port + PORT_IRQ_STAT);
	if (irq_stat & PORT_IRQ_ERROR) {
		cmd_err = sil24_readl(port + PORT_CMD_ERR);
		debug("%s: error code: 0x%x\n", __func__, cmd_err);
	}
	if (!(irq_stat & PORT_IRQ_COMPLETE)) {
		debug("%s: No IRQ complete: irq_status=0x%x\n",
		      __func__, irq_stat);

		return -1;
	}
	debug("Reset successful.\n");
	return 0;
}

/**
 *	__sata_phy_reset - Wake/reset a low-level SATA PHY
 *
 *	This function issues commands to standard SATA Sxxx
 *	PHY registers, to wake up the phy (and device), and
 *	clear any reset condition.
 *
 *	LOCKING:
 *	PCI/etc. bus probe sem.
 *
 */
static int __sata_phy_reset(int dev_no)
{
	u32 sstatus;
	int i;

	debug("%s: Resetting phy hda%d\n", __func__, dev_no);
	/* issue phy wake/reset */
	scr_write_flush(dev_no, SCR_CONTROL, 0x301);
	/* Couldn't find anything in SATA I/II specs, but
	 * AHCI-1.1 10.4.2 says at least 1 ms.
	 */
	mdelay(1);
	scr_write_flush(dev_no, SCR_CONTROL, 0x300);	/* phy wake/clear reset */

	/* wait for phy to become ready, if necessary */
	for (i = 0; i < 10; i++) {
		msleep(200);
		sstatus = sil24_scr_read(dev_no, SCR_STATUS);
		if ((sstatus & 0xf) != 1)
			break;
	}

	/* TODO: phy layer with polling, timeouts, etc. */
	sstatus = sil24_scr_read(dev_no, SCR_STATUS);
	if (sata_dev_present(dev_no)) {
		const char *speed;
		u32 tmp;

		tmp = (sstatus >> 4) & 0xf;
		if (tmp & (1 << 0))
			speed = "1.5";
		else if (tmp & (1 << 1))
			speed = "3.0";
		else
			speed = "<unknown>";
		debug("ata%u: SATA link up %s Gbps (SStatus %X)\n",
		      dev_no, speed, sstatus);
		return 0;
	} else {
		debug("ata%u: SATA link down (SStatus %X)\n", dev_no, sstatus);
		return 1;
	}
}

static int sil24_phy_reset(int dev_no)
{
	debug("%s: resetting device %d\n", __func__, dev_no);
	if (__sata_phy_reset(dev_no) != 0) {
		debug("ata%u: phy reset failed\n", dev_no);
		return -1;
	}

	if (sil24_issue_SRST(dev_no) < 0) {
		mdelay(2000);
		debug("ata%u: SRST failed, disabling port\n", dev_no);
		return -1;
	}

	return 0;
}

static void sil24_fill_sg(int dev_no, void *buffer)
{
	struct sil24_sge *sge;
	dma_addr_t sge_addr;

	sge_addr = (dma_addr_t)virt_to_bus(dev_no, buffer);
	debug("%s: dev %d, buffer=0x%08x (0x%llx phys)\n", __func__,
	      dev_no, (u32)buffer, (u64)sge_addr);

	sge_offset = 0;

	sge = &sata_cmd_block->ata.sge[0];
	sge->addr = cpu_to_le64(sge_addr);
	sge->cnt = cpu_to_le32(sata_tf.nsect * 512);
	sge->flags = cpu_to_le32(SGE_TRM);
}

void sil24_qc_prep(int dev_no, void *buffer)
{
	struct sil24_prb *prb = &sata_cmd_block->ata.prb;

	switch (sata_tf.protocol) {
	case ATA_PROT_PIO:
	case ATA_PROT_DMA:
	case ATA_PROT_NODATA:
		prb->ctrl = 0;
		break;

	case ATAPI_PROT_PIO:
	case ATAPI_PROT_DMA:
	case ATAPI_PROT_NODATA:
		if (sata_tf.protocol != ATAPI_PROT_NODATA) {
			if (sata_tf.flags & ATA_TFLAG_WRITE)
				prb->ctrl = cpu_to_le16(PRB_CTRL_PACKET_WRITE);
			else
				prb->ctrl = cpu_to_le16(PRB_CTRL_PACKET_READ);
		} else
			prb->ctrl = 0;

		break;

	default:
		break;
	}

	ata_tf_to_fis(&sata_tf, sata_cmd_block->ata.prb.fis, 0);

	sil24_fill_sg(dev_no, buffer);
}

static int sil24_qc_issue(int dev_no)
{
	int cnt;
	unsigned int port = sata_cmd_addr[dev_no];
	dma_addr_t paddr = virt_to_bus(dev_no, &sata_cmd_block->ata.prb);

	udelay(2);
	debug("%s dev: %d, paddr: 0x%llx\n", __func__, dev_no, (u64)paddr);

	sil24_printprb();

	if (sil24_flags[dev_no] & SIL24_FLAG_PCIX_IRQ_WOC)
		sil24_writel(PORT_CS_IRQ_WOC, port + PORT_CTRL_STAT);
	sil24_writel((u32) paddr, port + PORT_CMD_ACTIVATE);
	sil24_writel((u64) paddr >> 32, port + PORT_CMD_ACTIVATE + 4);

	u32 irq_stat;
	for (cnt = 0; cnt < 20 * 1000 * 1000L; cnt++) {
		irq_stat = sil24_readl(port + PORT_IRQ_STAT);
		sil24_writel(irq_stat, port + PORT_IRQ_STAT);	/* clear irq */

		irq_stat >>= PORT_IRQ_RAW_SHIFT;
		if (irq_stat & (PORT_IRQ_COMPLETE | PORT_IRQ_ERROR))
			break;
		udelay(1);
	}

	if (!(irq_stat & PORT_IRQ_COMPLETE)) {
		debug("IRQ Error: irq_stat=%x, irq_enable=%x\n", irq_stat,
		       sil24_readl(port + PORT_IRQ_ENABLE_SET));
		return -1;
	}

	return 0;
}

static int __sil24_restart_controller(unsigned int port)
{
	u32 tmp;
	int cnt;

	debug("%s: restarting controller on port 0x%x\n", __func__, port);
	sil24_writel(PORT_CS_INIT, port + PORT_CTRL_STAT);

	/* Max ~100ms */
	for (cnt = 0; cnt < 10; cnt++) {
		tmp = sil24_readl(port + PORT_CTRL_STAT);
		if (tmp & PORT_CS_RDY) {
			debug("%s: Restarted port 0x%x\n", __func__, port);
			return 0;
		}
		msleep(10);
	}
	debug("%s: Could not restart port 0x%x\n", __func__, port);
	return -1;
}

static int __sil24_reset_controller(unsigned int port)
{
	int cnt;
	u32 tmp;

	debug("%s: Resetting port 0x%x\n", __func__, port);
	/* Reset controller state.  Is this correct? */
	sil24_writel(PORT_CS_DEV_RST, port + PORT_CTRL_STAT);
	sil24_readl(port + PORT_CTRL_STAT);	/* sync */

	/* Max ~100ms */
	for (cnt = 0; cnt < 100; cnt++) {
		udelay(1000);
		tmp = sil24_readl(port + PORT_CTRL_STAT);
		if (!(tmp & PORT_CS_DEV_RST))
			break;
	}

	if (tmp & PORT_CS_DEV_RST)
		return -1;

	if (tmp & PORT_CS_RDY)
		return 0;

	debug("%s: port status: 0x%x, not ready\n", __func__, tmp);
	return __sil24_restart_controller(port);
}

/*-----------------------------------------*/
enum ata_completion_errors {
	AC_ERR_OTHER = (1 << 0),
	AC_ERR_DEV = (1 << 1),
	AC_ERR_ATA_BUS = (1 << 2),
	AC_ERR_HOST_BUS = (1 << 3),
};

static inline unsigned int ac_err_mask(u8 status)
{
	if (status & ATA_BUSY)
		return AC_ERR_ATA_BUS;
	if (status & (ATA_ERR | ATA_DF))
		return AC_ERR_DEV;
	return 0;
}

/*-----------------------------------------*/

static int sil24_port_start(void)
{

	static int do_once = 0;

	if (do_once) {
		memset(sata_private_data, 0, 512);
		memset(sata_cmd_block, 0, sizeof(*sata_cmd_block)*2);
		return 0;
	}

	sata_private_data = memalign(CONFIG_SYS_CACHELINE_SIZE, 512);
	if (sata_private_data == NULL) {
		printf("Memory allocate fail.\n");
		return -1;
	}

	sata_cmd_block =
		(struct sil24_cmd_block *)memalign(CONFIG_SYS_CACHELINE_SIZE,
						   sizeof(struct sil24_cmd_block)*2);
	if (sata_cmd_block == NULL) {
		printf("Memory alllocate fail.\n");
		return -1;
	}
	memset(sata_cmd_block, 0, sizeof(*sata_cmd_block)*2);

	do_once = 1;
	memset(sata_private_data, 0, 512);

	return 0;
}

static int sata_dev_read(int dev_no, lbaint_t blknr, lbaint_t blkcnt,
	                 ulong *buffer)
{
	u8 *dbuf = (u8 *)buffer;
	u32 num_blocks;

	debug("%s: dev: %u, blknr: %llu, blkcnt: %llu, buffer: 0x%x\n",
	      __func__, dev_no, blknr, blkcnt, (u32)buffer);

	if (lba64_mode[dev_no] == 0)
		sata_tf.command = ATA_CMD_READ;
	else
		sata_tf.command = ATA_CMD_READ_EXT;	/* ATA_DFLAG_LBA48  */
	sata_tf.feature = 0;
	sata_tf.flags =
	    ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE | lba64_mode[dev_no];
	sata_tf.protocol = ATAPI_PROT_PIO;
	sata_tf.device = ATA_DEVICE_OBS | ATA_LBA;

	while (blkcnt > 0) {
		sata_tf.lbal = (blknr & 0xFF);
		sata_tf.lbam = ((blknr >> 8) & 0xFF);
		sata_tf.lbah = ((blknr >> 16) & 0xFF);

#ifdef CONFIG_LBA48
		if (lba64_mode[dev_no] == ATA_DFLAG_LBA48) {
			num_blocks = min(blkcnt, 248);
			sata_tf.hob_lbal = ((blknr >> 24) & 0xFF);
			sata_tf.hob_lbam = ((blknr >> 32) & 0xFF);
			sata_tf.hob_lbah = ((blknr >> 40) & 0xFF);
			sata_tf.hob_nsect = (num_blocks >> 8) & 0xFF;

		} else
#endif
		{
			num_blocks = min(blkcnt, 248);
		}

		sata_tf.ctl |= ATA_DRQ;
		sata_tf.nsect = num_blocks & 0xFF;

		debug("%s: Reading %u sectors starting from sector %llu to address %p\n",
		      __func__, num_blocks, blknr, dbuf);
		sil24_qc_prep(dev_no, dbuf);
		if (sil24_qc_issue(dev_no) != 0) {
			debug("%s: sil24_qc_issue returned error\n", __func__);
			return -1;
		}
		blkcnt -= num_blocks;
		blknr += num_blocks;
		dbuf += num_blocks * 512;
		WATCHDOG_RESET();
	}

	return 0;
}
static int sata_dev_write(int dev_no, lbaint_t blknr, lbaint_t blkcnt,
			  ulong * buffer)
{
	u32 num_blocks;
	u8 *dbuf = (u8 *)buffer;

	if (lba64_mode[dev_no] == 0)
		sata_tf.command = ATA_CMD_WRITE;
	else
		sata_tf.command = ATA_CMD_WRITE_EXT;	/* ATA_DFLAG_LBA48 */

	sata_tf.feature = 0;
	sata_tf.flags =
		ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE | ATA_TFLAG_WRITE |
		lba64_mode[dev_no];
	sata_tf.protocol = ATAPI_PROT_PIO;
	sata_tf.device = ATA_DEVICE_OBS | ATA_LBA;

	while (blkcnt > 0) {

		sata_tf.lbal = (blknr >> 0) & 0xFF;
		sata_tf.lbam = (blknr >> 8) & 0xFF;
		sata_tf.lbah = (blknr >> 16) & 0xFF;

#ifdef CONFIG_LBA48
		if (lba64_mode[dev_no] == ATA_DFLAG_LBA48) {
			num_blocks = min(blkcnt, 248);
			sata_tf.hob_nsect = (num_blocks >> 8) & 0xFF;
			sata_tf.hob_lbal = ((blknr >> 24) & 0xFF);
			sata_tf.hob_lbam = ((blknr >> 32) & 0xFF);
			sata_tf.hob_lbah = ((blknr >> 40) & 0xFF);
		} else
#endif
		{
			num_blocks = min(blkcnt, 248);
		}

		sata_tf.nsect = num_blocks & 0xFF;

		sata_tf.ctl |= ATA_DRQ;

		sil24_qc_prep(dev_no, dbuf);
		if (sil24_qc_issue(dev_no) != 0)
			return -1;
		blkcnt -= num_blocks;
		blknr += num_blocks;
		dbuf += 512 * num_blocks;
		WATCHDOG_RESET();
	}

	return (0);
}

/**
 *	ata_dev_set_xfermode - Issue SET FEATURES - XFER MODE command
 *	@dev: Device to which command will be sent
 *
 *	Issue SET FEATURES - XFER MODE command to device @dev
 *	on port @ap.
 *
 *	LOCKING:
 *	PCI/etc. bus probe sem.
 */

static int ata_dev_set_xfermode(int dev_no, int xfer_mode)
{

	/* set up set-features taskfile */

	sata_tf.command = ATA_CMD_SET_FEATURES;
	sata_tf.feature = SETFEATURES_XFER;
	sata_tf.flags = ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE;
	sata_tf.protocol = ATA_PROT_NODATA;
	sata_tf.nsect = xfer_mode;

	sata_tf.device = ATA_DEVICE_OBS;
	sata_tf.ctl |= ATA_DRQ;

	sil24_qc_prep(dev_no, sata_private_data);
	if (sil24_qc_issue(dev_no) != 0) {
		return -1;
	}

	return 0;

}

int sata_dev_identify(int dev_no, unsigned long *buffer, int init_type)
{
	u8 *dbuf = (u8 *)buffer;

	if (!sata_dev_present(dev_no)) {
		return -1;
	}

	debug("In %s for device %d\n", __func__, dev_no);
	sata_tf.feature = 0;
	sata_tf.device = ATA_DEVICE_OBS;
	sata_tf.ctl |= ATA_DRQ;
	sata_tf.command = ATA_CMD_ID_ATA;
	sata_tf.protocol = ATA_PROT_PIO;
	sata_tf.nsect = 1;

	sil24_qc_prep(dev_no, buffer);
	if (sil24_qc_issue(dev_no) != 0) {
		debug("%s: sil24_qc_issue(%d) failed\n", __func__, dev_no);
		return -1;
	}

	swap_buf_le16((u16 *)dbuf, ATA_ID_WORDS);

	if (!ata_id_has_dma((u16 *)dbuf) || !ata_id_has_lba((u16 *)dbuf)) {
		debug("%s: dbuf doesn't have DMA or LBA for device %d\n",
		      __func__, dev_no);
		return -1;
	}

	xfer_modes[dev_no] = dbuf[ATA_ID_UDMA_MODES];
	if (!xfer_modes[dev_no])
		xfer_modes[dev_no] =
		    (dbuf[ATA_ID_MWDMA_MODES]) << ATA_SHIFT_MWDMA;
	if (!xfer_modes[dev_no]) {
		xfer_modes[dev_no] =
		    (dbuf[ATA_ID_PIO_MODES]) << (ATA_SHIFT_PIO + 3);
		xfer_modes[dev_no] |= (0x7 << ATA_SHIFT_PIO);
	}

	if (ata_id_has_lba48(dbuf))
		lba64_mode[dev_no] = ATA_DFLAG_LBA48;
	else
		lba64_mode[dev_no] = 0;

	/* print device info to dmesg */
	if (init_type != 0)
		debug("ata%u: lba%s mode\n         ",
		       dev_no,
		       (lba64_mode[dev_no] & ATA_DFLAG_LBA48) ? " 48" : "");

	return 0;
}

int sata_device_add(int dev_no)
{
	unsigned char buf[512];

	if (sil24_phy_reset(dev_no) != 0)
		return -1;

	memset(buf, 0, sizeof(buf));

	if (sata_dev_identify(dev_no, (void *)buf, 0) != 0) {
		debug("%s: sata_dev_identify failed\n", __func__);
		return -1;
	}

	return 0;

}

int sil24_init_one(int dev_no)
{
	int ret_val = 0;
	uint32_t host_base;
	uint32_t port_base;
	uint16_t pci_command;
        static int base_initialized = 0;
	pci_dev_t dev = -1;
	int max_ports;
	int i;
	int board_id;

	i = 0;
	while (sil24_pci_tbl[i].dev_id.vendor != 0) {
		dev = pci_find_device(sil24_pci_tbl[i].dev_id.vendor,
				      sil24_pci_tbl[i].dev_id.device, 0);
		if (dev >= 0) {
			board_id = sil24_pci_tbl[i].board_id;
			sil24_flags[dev_no] = sil24_port_info[board_id].flags;
			debug("%s: Found device %04x:%04x (%d)\n",
			      __func__, sil24_pci_tbl[i].dev_id.vendor,
			      sil24_pci_tbl[i].dev_id.device, dev_no);
			break;
		}
		i++;
	}
	if (dev < 0) {
		debug("Silicon Image device not found for dev %d\n", dev_no);
		return -1;
	}
	max_ports = SIL24_FLAG2NPORTS(sil24_flags[dev_no]);
	if (dev_no >= max_ports)
		return -1;

	debug("%s: Found board dev %d\n", __func__, dev);
	pci_read_config_dword(dev, PCI_BASE_ADDRESS_0, &host_base);
	host_base = host_base & 0xfffffff0;

	pci_read_config_dword(dev, PCI_BASE_ADDRESS_2, &port_base);
	port_base = port_base & 0xfffffff0;

	pci_read_config_word(dev, PCI_COMMAND, &pci_command);

	debug("host base: 0x%x, port base: 0x%x, command: 0x%x\n",
	      host_base, port_base, pci_command);
	/*
	 * Configure the device
	 */
	if (!base_initialized) {
		/* GPIO off */
		sil24_writel(0, host_base + HOST_FLASH_CMD);

		/* Mask interrupts during initialization */
		/* 1. Clear port reset
		 */
#if 0
		sil24_writel(0x80000000, host_base + HOST_CTRL);
		mdelay(10);
#endif
		sil24_writel(0, host_base + HOST_CTRL);
		udelay(100);
		debug("%s: Global control reg=0x%x\n", __func__,
		      sil24_readl(host_base + HOST_CTRL));
		base_initialized = 1;
	}
	unsigned long port = port_base + dev_no * PORT_REGS_SIZE;
	unsigned long portu = (unsigned long)port;
	int cnt;
	uint32_t tmp;

	sata_cmd_addr[dev_no] = portu + PORT_LRAM;
	sata_scr_addr[dev_no] = portu + PORT_SCONTROL;

	debug("device %d port address 0x%lx\n", dev_no, port);
	sil24_writel(0x20c, port + PORT_PHY_CFG);

	/* 2 a) Clear port reset */
	sil24_writel(1, port + PORT_CTRL_CLR);

	/* 2 d) check if device is present */
	for (cnt = 0; cnt < 10; cnt++) {
		tmp = sil24_readl(port + PORT_SSTATUS);
		if ((tmp & 0x0000000F) == 3)
			break;
		msleep(10);
	}
	if ((tmp & 0x0000000F) != 3) {
		debug("Device not present on port %d\n", dev_no);
	} else {
			/* 2 e) Wait for port ready */
		for (cnt = 0; cnt < 10; cnt++) {
			tmp = sil24_readl(port + PORT_CTRL_STAT);
			if (tmp & PORT_CS_RDY)
				break;
			msleep(10);
		}
	}
	if ((tmp & (PORT_CS_RDY | PORT_CS_INIT)) != PORT_CS_RDY) {
		debug("Port %d not ready!\n", dev_no);
	} else {

		/* Initial PHY setting */
		if (sil24_flags[dev_no] & SIL24_FLAG_PCIX_IRQ_WOC)
			sil24_writel(PORT_CS_IRQ_WOC, port + PORT_CTRL_STAT);
		else
			sil24_writel(PORT_CS_IRQ_WOC, port + PORT_CTRL_CLR);

		/* Zero error counters. */
		sil24_writew(0x8000, port + PORT_DECODE_ERR_THRESH);
		sil24_writew(0x8000, port + PORT_CRC_ERR_THRESH);
		sil24_writew(0x8000, port + PORT_HSHK_ERR_THRESH);
		sil24_writew(0x0000, port + PORT_DECODE_ERR_CNT);
		sil24_writew(0x0000, port + PORT_CRC_ERR_CNT);
		sil24_writew(0x0000, port + PORT_HSHK_ERR_CNT);

		/*sil24_writel(0, port + PORT_ACTIVATE_UPPER_ADDR);*/
		/*sil24_writel(PORT_CS_32BIT_ACTV, port + PORT_CTRL_STAT);*/
		/* always use 64bit activation */
		sil24_writel(PORT_CS_32BIT_ACTV, port + PORT_CTRL_CLR);

		/* clear port multiplier enable and resume bits */
		sil24_writel(PORT_CS_PMP_EN | PORT_CS_PMP_RESUME, port + PORT_CTRL_CLR);

		/* Configure interrupts */
		sil24_writel(0x08ff, port + PORT_IRQ_ENABLE_CLR);

		/* Clear interrupts */
		sil24_writel(0x0fff0fff, port + PORT_IRQ_STAT);

		/* Reset itself */
		if (__sil24_reset_controller(port))
			printf("%s: failed to reset controller\n", __func__);
	}

	msleep(10);		/* *** East add */

	sil24_port_start();

	ret_val = sata_device_add(dev_no);

	if (!ret_val)
		debug("%s returned SUCCESS\n", __func__);

	return ret_val;
}

/* ------------------------------------------------------------------------- */

/*
 * copy src to dest, skipping leading and trailing blanks and null
 * terminate the string
 * "len" is the size of available memory including the terminating '\0'
 */
static void ident_cpy(unsigned char *dst, unsigned char *src, unsigned int len)
{
	unsigned char *end, *last;

	last = dst;
	end = src + len - 1;

	/* reserve space for '\0' */
	if (len < 2)
		goto OUT;

	/* skip leading white space */
	while ((*src) && (src < end) && (*src == ' '))
		++src;

	/* copy string, omitting trailing white space */
	while ((*src) && (src < end)) {
		*dst++ = *src;
		if (*src++ != ' ')
			last = dst;
	}
OUT:
	*last = '\0';
}

void sata_ident(block_dev_desc_t * dev_desc)
{
	static int do_first = 0;
	int device = dev_desc->dev;;
	ulong iobuf[ATA_SECTORWORDS];
	hd_driveid_t *iop = (hd_driveid_t *) iobuf;

	memset(iobuf, 0, sizeof(iobuf));

	if (sata_dev_identify(device, iobuf, 1) != 0)
		return;

	if (do_first == 0) {
		ata_dev_set_xfermode(device, xfer_modes[device]);
		do_first = 1;
	}

	dev_desc->if_type = IF_TYPE_SATA;

	ident_cpy((unsigned char *)dev_desc->revision, iop->fw_rev,
		  sizeof(dev_desc->revision));
	ident_cpy((unsigned char *)dev_desc->vendor, iop->model,
		  sizeof(dev_desc->vendor));
	ident_cpy((unsigned char *)dev_desc->product, iop->serial_no,
		  sizeof(dev_desc->product));
#ifdef __LITTLE_ENDIAN
	/*
	 * firmware revision and model number have Big Endian Byte
	 * order in Word. Convert both to little endian.
	 *
	 * See CF+ and CompactFlash Specification Revision 2.0:
	 * 6.2.1.6: Identfy Drive, Table 39 for more details
	 */

	strswab(dev_desc->revision);
	strswab(dev_desc->vendor);
#endif /* __LITTLE_ENDIAN */

	if ((iop->config & 0x0080) == 0x0080)
		dev_desc->removable = 1;
	else
		dev_desc->removable = 0;

#ifdef __BIG_ENDIAN
	/* swap shorts */
	dev_desc->lba = (iop->lba_capacity << 16) | (iop->lba_capacity >> 16);
#else /* ! __BIG_ENDIAN */
	/*
	 * do not swap shorts on little endian
	 *
	 * See CF+ and CompactFlash Specification Revision 2.0:
	 * 6.2.1.6: Identfy Drive, Table 39, Word Address 57-58 for details.
	 */
	dev_desc->lba = iop->lba_capacity;
#endif /* __BIG_ENDIAN */

#ifdef CONFIG_LBA48
	if (iop->command_set_2 & 0x0400) {	/* LBA 48 support */
		dev_desc->lba48 = 1;
		dev_desc->lba = (unsigned long long)iop->lba48_capacity[0] |
		    ((unsigned long long)iop->lba48_capacity[1] << 16) |
		    ((unsigned long long)iop->lba48_capacity[2] << 32) |
		    ((unsigned long long)iop->lba48_capacity[3] << 48);
	} else {
		dev_desc->lba48 = 0;
	}
#endif /* CONFIG_LBA48 */
	/* assuming HD */
	dev_desc->type = DEV_TYPE_HARDDISK;
	dev_desc->blksz = ATA_BLOCKSIZE;
	dev_desc->lun = 0;	/* just to fill something in... */

}

ulong sata_read(int device, lbaint_t blknr, ulong blkcnt, void * buffer)
{
	int ret;

	ret = sata_dev_read(sata_get_dev(device)->dev, blknr, blkcnt,
			    (ulong *)buffer);
	if (ret != 0)
		return 0;
	else
		return blkcnt;
}

ulong sata_write(int device, lbaint_t blknr, ulong blkcnt, void * buffer)
{
	int ret, n;
	u32 *buf = (u32 *)buffer;

	n = 0;
	while (blkcnt-- > 0) {
		ret =
		    sata_dev_write(sata_get_dev(device)->dev, blknr, 1,
				   (ulong *)buffer);
		if (ret != 0)
			return n;
		++n;
		++blknr;
		buf += ATA_SECTORWORDS;
	}

	return (n);
}

void sil24_sata_reset(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	sil24_init_one(0);
}

int init_sata(int dev_no)
{
	return sil24_init_one(dev_no);
}

int scan_sata(int dev_no)
{
	int rc;
	rc = sil24_init_port(dev_no);
	if (rc) {
		debug("%s: sil24_init_port(%d) failed\n", __func__, dev_no);
		return rc;
	}
	sata_ident(&sata_dev_desc[dev_no]);
	return rc;
}
