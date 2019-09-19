/*
 * Copyright (C) Freescale Semiconductor, Inc. 2006.
 * Author: Jason Jin<Jason.jin@freescale.com>
 *         Zhang Wei<wei.zhang@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _AHCI_H_
#define _AHCI_H_

#include <pci.h>

/* Enclosure Management Control */
#define EM_CTRL_MSG_TYPE              0x000f0000

/* Enclosure Management LED Message Type */
#define EM_MSG_LED_HBA_PORT           0x0000000f
#define EM_MSG_LED_PMP_SLOT           0x0000ff00
#define EM_MSG_LED_VALUE              0xffff0000
#define EM_MSG_LED_VALUE_ACTIVITY     0x00070000
#define EM_MSG_LED_VALUE_OFF          0xfff80000
#define EM_MSG_LED_VALUE_ON           0x00010000

#define AHCI_PCI_BAR		0x24
#define AHCI_MAX_PORTS		32
#define AHCI_MAX_SG		168 /* hardware max is 64K */
#define AHCI_MAX_CMDS		32
#define AHCI_CMD_SZ		32
#define AHCI_CMD_SLOT_SZ	AHCI_MAX_CMDS * AHCI_CMD_SZ
#define AHCI_RX_FIS_SZ		256
#define AHCI_CMD_TBL_CDB	0x40
#define AHCI_CMD_TBL_HDR_SZ	0x80
#define AHCI_CMD_TBL_SZ		(AHCI_CMD_TBL_HDR_SZ + (AHCI_MAX_SG * 16))
#define AHCI_CMD_TBL_AR_SZ	(AHCI_CMD_TBL_SZ * AHCI_MAX_CMDS)
#define AHCI_PORT_PRIV_DMA_SZ	(AHCI_CMD_SLOT_SZ + AHCI_CMD_TBL_AR_SZ + \
				 AHCI_RX_FIS_SZ)

#define AHCI_PORT_PRIV_FBS_DMA_SZ	(AHCI_CMD_SLOT_SZ + \
					 AHCI_CMD_TBL_AR_SZ + \
					 (AHCI_RX_FIS_SZ * 16))

#define RX_FIS_PIO_SETUP	0x20	/* offset of PIO setup FIS data */
#define RX_FIS_D2H_REG		0x40	/* offset of D2H Register FIS data */
#define RX_FIS_SDB		0x58	/* offset of SDB FIS data */
#define RX_FIS_UNK		0x60	/* offset of Unknown FIS data */

#define AHCI_CMD_ATAPI		(1 << 5)
#define AHCI_CMD_WRITE		(1 << 6)
#define AHCI_CMD_PREFETCH	(1 << 7)
#define AHCI_CMD_RESET		(1 << 8)
#define AHCI_CMD_CLR_BUSY	(1 << 10)

#define RX_FIS_D2H_REG		0x40	/* offset of D2H Register FIS data */

/* Global controller registers */
#define HOST_CAP		0x00 /* host capabilities */
#define HOST_CTL		0x04 /* global host control */
#define HOST_IRQ_STAT		0x08 /* interrupt status */
#define HOST_PORTS_IMPL		0x0c /* bitmap of implemented ports */
#define HOST_VERSION		0x10 /* AHCI spec. version compliancy */
#define HOST_EM_LOC		0x1C /* Enclosure Management location */
#define HOST_EM_CTL		0x20 /* Enclosure Management Control */
#define HOST_CAP2		0x24 /* host capabilities, extended */

/* HOST_CAP bist */
#define HOST_CAP_SXS		(1 << 5)  /* Supports External SATA */
#define HOST_CAP_EMS		(1 << 6)  /* Enclosure Management Support */
#define HOST_CAP_CCC		(1 << 7)  /* Command Completion Coalescing */
#define HOST_CAP_PART		(1 << 13) /* Partial state capable */
#define HOST_CAP_SSC		(1 << 14) /* Slumber state capable */
#define HOST_CAP_PIO_MULTI	(1 << 15) /* PIO multiple DRQ support */
#define HOST_CAP_FBS		(1 << 16) /* FIS-based switching support */
#define HOST_CAP_PMP		(1 << 17) /* Port Multiplier support */
#define HOST_CAP_ONLY		(1 << 18) /* Supports AHCI mode only */
#define HOST_CAP_CLO		(1 << 24) /* Command List Override support */
#define HOST_CAP_LED		(1 << 25) /* Supports activity LED */
#define HOST_CAP_ALPM		(1 << 26) /* Aggressive Link PM support */
#define HOST_CAP_SSS		(1 << 27) /* Staggered Spin-up */
#define HOST_CAP_MPS		(1 << 28) /* Mechanical presence switch */
#define HOST_CAP_SNTF		(1 << 29) /* SNotification register */
#define HOST_CAP_NCQ		(1 << 30) /* Native Command Queueing */
#define HOST_CAP_64		(1 << 31) /* PCI DAC (64-bit DMA) support */

/* HOST_CAP2 bits */
#define HOST_CAP2_BOH		(1 << 0)  /* BIOS/OS handoff supported */
#define HOST_CAP2_NVMHCI		(1 << 1)  /* NVMHCI supported */
#define HOST_CAP2_APST		(1 << 2)  /* Automatic partial to slumber */

/* HOST_CTL bits */
#define HOST_RESET		(1 << 0)  /* reset controller; self-clear */
#define HOST_IRQ_EN		(1 << 1)  /* global IRQ enable */
#define HOST_AHCI_EN		(1 << 31) /* AHCI enabled */

/* Registers for each SATA port */
#define PORT_LST_ADDR		0x00 /* command list DMA addr */
#define PORT_LST_ADDR_HI	0x04 /* command list DMA addr hi */
#define PORT_FIS_ADDR		0x08 /* FIS rx buf addr */
#define PORT_FIS_ADDR_HI	0x0c /* FIS rx buf addr hi */
#define PORT_IRQ_STAT		0x10 /* interrupt status */
#define PORT_IRQ_MASK		0x14 /* interrupt enable/disable mask */
#define PORT_CMD		0x18 /* port command */
#define PORT_TFDATA		0x20 /* taskfile data */
#define PORT_SIG		0x24 /* device TF signature */
#define PORT_SCR		0x28 /* SATA phy register block */
#define PORT_SCR_STAT		0x28 /* SATA phy register: SStatus */
#define PORT_SCR_CTL		0x2c /* SATA phy register: SControl */
#define PORT_SCR_ERR		0x30 /* SATA phy register: SError */
#define PORT_SCR_ACT		0x34 /* SATA phy register: SActive */
#define PORT_CMD_ISSUE		0x38 /* command issue */
#define PORT_SCR_NTF		0x3c /* SATA port serial ATA notification */
#define PORT_FBS		0x40 /* SATA port FIS based switching control */
#define PORT_DEVSLP		0x44 /* device sleep */

/* PORT_IRQ_{STAT,MASK} bits */
#define PORT_IRQ_COLD_PRES	(1 << 31) /* cold presence detect */
#define PORT_IRQ_TF_ERR		(1 << 30) /* task file error */
#define PORT_IRQ_HBUS_ERR	(1 << 29) /* host bus fatal error */
#define PORT_IRQ_HBUS_DATA_ERR	(1 << 28) /* host bus data error */
#define PORT_IRQ_IF_ERR		(1 << 27) /* interface fatal error */
#define PORT_IRQ_IF_NONFATAL	(1 << 26) /* interface non-fatal error */
#define PORT_IRQ_OVERFLOW	(1 << 24) /* xfer exhausted available S/G */
#define PORT_IRQ_BAD_PMP	(1 << 23) /* incorrect port multiplier */

#define PORT_IRQ_PHYRDY		(1 << 22) /* PhyRdy changed */
#define PORT_IRQ_DEV_ILCK	(1 << 7) /* device interlock */
#define PORT_IRQ_CONNECT	(1 << 6) /* port connect change status */
#define PORT_IRQ_SG_DONE	(1 << 5) /* descriptor processed */
#define PORT_IRQ_UNK_FIS	(1 << 4) /* unknown FIS rx'd */
#define PORT_IRQ_SDB_FIS	(1 << 3) /* Set Device Bits FIS rx'd */
#define PORT_IRQ_DMAS_FIS	(1 << 2) /* DMA Setup FIS rx'd */
#define PORT_IRQ_PIOS_FIS	(1 << 1) /* PIO Setup FIS rx'd */
#define PORT_IRQ_D2H_REG_FIS	(1 << 0) /* D2H Register FIS rx'd */

#define PORT_IRQ_FATAL		PORT_IRQ_TF_ERR | PORT_IRQ_HBUS_ERR	\
				| PORT_IRQ_HBUS_DATA_ERR | PORT_IRQ_IF_ERR

#define DEF_PORT_IRQ		PORT_IRQ_FATAL | PORT_IRQ_PHYRDY	\
				| PORT_IRQ_CONNECT | PORT_IRQ_SG_DONE	\
				| PORT_IRQ_UNK_FIS | PORT_IRQ_SDB_FIS	\
				| PORT_IRQ_DMAS_FIS | PORT_IRQ_PIOS_FIS	\
				| PORT_IRQ_D2H_REG_FIS

/* PORT_SCR_STAT bits */
#define PORT_SCR_STAT_DET_MASK	0x3
#define PORT_SCR_STAT_DET_COMINIT 0x1
#define PORT_SCR_STAT_DET_PHYRDY 0x3

/* PORT_CMD bits */
#define PORT_CMD_ASP		(1 << 27) /* Aggressive Slumber/Partial */
#define PORT_CMD_ALPE		(1 << 26) /* Aggressive Link PM enable */
#define PORT_CMD_ALED		(1 << 25) /* Assert LED on activity */
#define PORT_CMD_ATAPI		(1 << 24) /* Device is ATAPI */
#define PORT_CMD_FBSCP		(1 << 22) /* FBS Capable Port */
#define PORT_CMD_ESP		(1 << 21) /* External SATA Port */
#define PORT_CMD_CPD		(1 << 20) /* Cold Presence Detection */
#define PORT_CMD_PMP		(1 << 17) /* PMP attached */
#define PORT_CMD_CPS		(1 << 16) /* Cold Presence State */
#define PORT_CMD_LIST_ON	(1 << 15) /* cmd list DMA engine running */
#define PORT_CMD_FIS_ON		(1 << 14) /* FIS DMA engine running */
#define PORT_CMD_FIS_RX		(1 << 4) /* Enable FIS receive DMA engine */
#define PORT_CMD_CLO		(1 << 3) /* Command list override */
#define PORT_CMD_POWER_ON	(1 << 2) /* Power up device */
#define PORT_CMD_SPIN_UP	(1 << 1) /* Spin up device */
#define PORT_CMD_START		(1 << 0) /* Enable port DMA engine */

#define PORT_CMD_ICC_MASK	(0xf << 28) /* i/f ICC state mask */
#define PORT_CMD_ICC_ACTIVE	(0x1 << 28) /* Put i/f in active state */
#define PORT_CMD_ICC_PARTIAL	(0x2 << 28) /* Put i/f in partial state */
#define PORT_CMD_ICC_SLUMBER	(0x6 << 28) /* Put i/f in slumber state */

#define PORT_FBS_DWE_OFFSET	16	/* FBS device with error offset */
#define PORT_FBS_ADO_OFFSET	12	/* FBS active dev optimization offset */
#define PORT_FBS_DEV_OFFSET	8	/* FBS device to issue offset */
#define PORT_FBS_DEV_MASK	(0xf << PORT_FBS_DEV_OFFSET) /* FBS.DEV */
#define PORT_FBS_SDE		(1 << 2) /* FBS single device error */
#define PORT_FBS_DEC		(1 << 1) /* FBS device error clear */
#define PORT_FBS_EN		(1 << 0) /* Enable FBS */

#define AHCI_MAX_PORTS		32

/* SETFEATURES stuff */
#define SETFEATURES_XFER	0x03
#define XFER_UDMA_7		0x47
#define XFER_UDMA_6		0x46
#define XFER_UDMA_5		0x45
#define XFER_UDMA_4		0x44
#define XFER_UDMA_3		0x43
#define XFER_UDMA_2		0x42
#define XFER_UDMA_1		0x41
#define XFER_UDMA_0		0x40
#define XFER_MW_DMA_2		0x22
#define XFER_MW_DMA_1		0x21
#define XFER_MW_DMA_0		0x20
#define XFER_SW_DMA_2		0x12
#define XFER_SW_DMA_1		0x11
#define XFER_SW_DMA_0		0x10
#define XFER_PIO_4		0x0C
#define XFER_PIO_3		0x0B
#define XFER_PIO_2		0x0A
#define XFER_PIO_1		0x09
#define XFER_PIO_0		0x08
#define XFER_PIO_SLOW		0x00

#define ATA_FLAG_NO_LEGACY	(1 << 4) /* no legacy mode check */
#define ATA_FLAG_MMIO		(1 << 6) /* use MMIO, not PIO */
#define ATA_FLAG_SATA_RESET	(1 << 7) /* (obsolete) use COMRESET */
#define ATA_FLAG_NO_ATAPI	(1 << 11) /* No ATAPI support */

enum {
	/* hpriv->flags bits */

#define AHCI_HFLAGS(flags)		.private_data	= (void *)(flags)

	AHCI_HFLAG_NO_NCQ		= (1 << 0),
	AHCI_HFLAG_IGN_IRQ_IF_ERR	= (1 << 1), /* ignore IRQ_IF_ERR */
	AHCI_HFLAG_IGN_SERR_INTERNAL	= (1 << 2), /* ignore SERR_INTERNAL */
	AHCI_HFLAG_32BIT_ONLY		= (1 << 3), /* force 32bit */
	AHCI_HFLAG_MV_PATA		= (1 << 4), /* PATA port */
	AHCI_HFLAG_NO_MSI		= (1 << 5), /* no PCI MSI */
	AHCI_HFLAG_NO_PMP		= (1 << 6), /* no PMP */
	AHCI_HFLAG_SECT255		= (1 << 8), /* max 255 sectors */
	AHCI_HFLAG_YES_NCQ		= (1 << 9), /* force NCQ cap on */
	AHCI_HFLAG_NO_SUSPEND		= (1 << 10), /* don't suspend */
	AHCI_HFLAG_SRST_TOUT_IS_OFFLINE	= (1 << 11), /* treat SRST timeout as
							link offline */
	AHCI_HFLAG_NO_SNTF		= (1 << 12), /* no sntf */
	AHCI_HFLAG_NO_FPDMA_AA		= (1 << 13), /* no FPDMA AA */
	AHCI_HFLAG_YES_FBS		= (1 << 14), /* force FBS cap on */
	AHCI_HFLAG_DELAY_ENGINE		= (1 << 15), /* do not start engine on
						        port start (wait until
						        error-handling stage) */

	/* ap->flags bits */

	AHCI_FLAG_COMMON		= ATA_FLAG_SATA | ATA_FLAG_PIO_DMA |
					  ATA_FLAG_ACPI_SATA | ATA_FLAG_AN,

	/* em constants */
	EM_MAX_SLOTS			= 8,
	EM_MAX_RETRY			= 5,

	/* em_ctl bits */
	EM_CTL_RST			= (1 << 9), /* Reset */
	EM_CTL_TM			= (1 << 8), /* Transmit Message */
	EM_CTL_MR			= (1 << 0), /* Message Received */
	EM_CTL_ALHD			= (1 << 26), /* Activity LED */
	EM_CTL_XMT			= (1 << 25), /* Transmit Only */
	EM_CTL_SMB			= (1 << 24), /* Single Message Buffer */
	EM_CTL_SGPIO			= (1 << 19), /* SGPIO messages supported */
	EM_CTL_SES			= (1 << 18), /* SES-2 messages supported */
	EM_CTL_SAFTE			= (1 << 17), /* SAF-TE messages supported */
	EM_CTL_LED			= (1 << 16), /* LED messages supported */

	/* em message type */
	EM_MSG_TYPE_LED			= (1 << 0), /* LED */
	EM_MSG_TYPE_SAFTE		= (1 << 1), /* SAF-TE */
	EM_MSG_TYPE_SES2		= (1 << 2), /* SES-2 */
	EM_MSG_TYPE_SGPIO		= (1 << 3), /* SGPIO */
};

struct ahci_cmd_hdr {
	u32	opts;
	u32	status;
	u32	tbl_addr;
	u32	tbl_addr_hi;
	u32	reserved[4];
};

struct ahci_sg {
	u32	addr;
	u32	addr_hi;
	u32	reserved;
	u32	flags_size;
};

struct ahci_ioports {
	u32	cmd_addr;
	u32	scr_addr;
	u32	port_mmio;
	struct ahci_cmd_hdr	*cmd_slot;
	struct ahci_sg		*cmd_tbl_sg;
	void 			*cmd_tbl;
	void			*rx_fis;
	phys_addr_t	cmd_addr_dma;
	phys_addr_t	scr_addr_dma;
	phys_addr_t	cmd_slot_dma;
	phys_addr_t	cmd_tbl_dma;
	phys_addr_t	cmd_tbl_sg_dma;
	phys_addr_t	rx_fis_dma;
	int		is_lba48;
	int		fbs_supported;
};

struct ahci_probe_ent {
	pci_dev_t	dev;
	struct ahci_ioports	port[AHCI_MAX_PORTS];
	u32	n_ports;
	u32	hard_port_no;
	u32	host_flags;
	u32	host_set_flags;
	u32	mmio_base;
	u32	pio_mask;
	u32	udma_mask;
	u32	flags;
	u32	cap;	/* cache of HOST_CAP register */
	u32	port_map; /* cache of HOST_PORTS_IMPL reg */
	u32	link_port_map; /*linkup port map*/
};

struct ahci_port_priv {
	struct ata_link		*active_link;
	struct ahci_cmd_hdr	*cmd_slot;
	phys_addr_t		cmd_slot_dma;
	void			*cmd_tbl;
	phys_addr_t		cmd_tbl_dma;
	void			*rx_fis;
	phys_addr_t		rx_fis_dma;
	/* for NCQ spurious interrupt analysis */
	unsigned int		ncq_saw_d2h:1;
	unsigned int		ncq_saw_dmas:1;
	unsigned int		ncq_saw_sdb:1;
	u32 			intr_mask;	/* interrupts to enable */
	bool			fbs_supported;	/* set iff FBS is supported */
	bool			fbs_enabled;	/* set iff FBS is enabled */
	int			fbs_last_dev;	/* save FBS.DEV of last FIS */
};

#ifdef CONFIG_SATA_AHCI_PLAT
struct ahci_platform_data {
	int (*init)(struct ata_host *host);
	void (*exit)(struct ata_host *host);
	const struct ata_port_info *ata_port_info;
	unsigned int force_port_map;
	unsigned int mask_port_map;
	void *priv;
};
#endif

struct ahci_host_priv {
	void		*mmio;		/* bus-independent mem map */
	unsigned int	flags;		/* AHCI_HFLAG_ */
	u32		cap;		/* cap to use */
	u32		cap2;		/* cap2 to use */
	u32		port_map;	/* port map to use */
	u32		saved_cap;	/* saved initial cap */
	u32		saved_cap2;	/* saved initial cap2 */
	u32		saved_port_map;	/* saved initial port map */
	u32 		em_loc;		/* enclosure management location */
	u32		em_buf_sz;	/* EM buffer size in byte */
	u32		em_msg_type;	/* EM message type */
	pci_dev_t	pdev;		/* PCI device */
#ifdef CONFIG_SATA_AHCI_PLAT
	struct ahci_platform_data *pdata;
#endif
};

int ahci_init(u32 base);

static inline void *__ahci_port_base(struct ata_host *host,
				     unsigned int port_no)
{
	struct ahci_host_priv *hpriv = host->private_data;
	void *mmio = hpriv->mmio;

	return mmio + 0x100 + (port_no * 0x80);
}

static inline void *ahci_port_base(struct ata_port *ap)
{
	return __ahci_port_base(ap->host, ap->port_no);
}

static inline int ahci_nr_ports(u32 cap)
{
	return (cap & 0x1f) + 1;
}

int ahci_port_start(struct ata_port *ap);
void ahci_port_stop(struct ata_port *ap);
void ahci_enable_fbs(struct ata_port *ap);
void ahci_disable_fbs(struct ata_port *ap);
void ahci_pmp_attach(struct ata_port *ap);
void ahci_pmp_detach(struct ata_port *ap);
int ahci_softreset(struct ata_link *link, unsigned int *cls,
		   unsigned long deadline);
int ahci_hardreset(struct ata_link *link, unsigned int *cls,
		   unsigned long deadline);
void ahci_start_engine(struct ata_port *ap);
int ahci_stop_engine(struct ata_port *ap);

int ahci_scan_device(struct ata_device *dev, bool verbose);

#endif /* _AHCI_H_ */
