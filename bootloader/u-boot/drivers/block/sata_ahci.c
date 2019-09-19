/*
 * Copyright (C) Freescale Semiconductor, Inc. 2006.
 * Author: Jason Jin<Jason.jin@freescale.com>
 *         Zhang Wei<wei.zhang@freescale.com>
 *
 * Copyright (C) Cavium, Inc. 2011, 2013 - 2015
 * Author: Aaron Williams <Aaron.Williams@cavium.com>
 * 	Updated to follow SATA drivers
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
 *
 * with the reference on libata and ahci drvier in kernel
 *
 */

#include <common.h>
#include <pci.h>
#include <asm/processor.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/bitops.h>
#include <malloc.h>
#include <libata.h>
#include <ata.h>
#include <linux/ctype.h>
#include <linux/dma-direction.h>
#include <linux/time.h>
#include <ahci.h>
#include <fis.h>

#define WAIT_MS_SPINUP	10000
#define WAIT_MS_DATAIO	5000
#define WAIT_MS_FLUSH	5000
#define WAIT_MS_LINKUP	4
#define ATA_EH_CMD_DFL_TIMEOUT		5000


int ahci_last_port_idx = 0;
int ahci_last_device_idx = 0;

struct ata_port *sata_port_desc[CONFIG_SYS_SATA_MAX_DEVICE];
struct ata_host *sata_host_desc[CONFIG_SYS_SATA_MAX_DEVICE];
struct ata_device *sata_device_desc[CONFIG_SYS_SATA_MAX_DEVICE];
struct ata_link *sata_pmp_link[CONFIG_SYS_SATA_MAX_DEVICE];

hd_driveid_t *ataid[AHCI_MAX_PORTS];

extern block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

/* The following table determines how we sequence resets.  Each entry
 * represents timeout for that try.  The first try can be soft or
 * hardreset.  All others are hardreset if available.  In most cases
 * the first reset w/ 10sec timeout should succeed.  Following entries
 * are mostly for error handling, hotplug and retarded devices.
 */
static const unsigned long ata_eh_reset_timeouts[] = {
	10000,	/* most drives spin up by 10sec */
	10000,	/* > 99% working drives spin up before 20sec */
	35000,	/* give > 30 secs of idleness for retarded devices */
	5000,	/* and sweet one last chance */
	(unsigned long)-1, /* > 1 min has elapsed, give up */
};

static const unsigned long ata_eh_identify_timeouts[] = {
	5000,	/* covers > 99% of successes and not too boring on failures */
	10000,  /* combined time till here is enough even for media access */
	30000,	/* for true idiots */
	(unsigned long)-1,
};

static const unsigned long ata_eh_flush_timeouts[] = {
	15000,	/* be generous with flush */
	15000,  /* ditto */
	30000,	/* and even more generous */
	(unsigned long)-1,
};

static const unsigned long ata_eh_other_timeouts[] = {
	5000,	/* same rationale as identify timeout */
	10000,	/* ditto */
	/* but no merciful 30sec for other commands, it just isn't worth it */
	(unsigned long)-1,
};

struct ata_eh_cmd_timeout_ent {
	const u8		*commands;
	const unsigned long	*timeouts;
};

/* The following table determines timeouts to use for EH internal
 * commands.  Each table entry is a command class and matches the
 * commands the entry applies to and the timeout table to use.
 *
 * On the retry after a command timed out, the next timeout value from
 * the table is used.  If the table doesn't contain further entries,
 * the last value is used.
 *
 * ehc->cmd_timeout_idx keeps track of which timeout to use per
 * command class, so if SET_FEATURES times out on the first try, the
 * next try will use the second timeout value only for that class.
 */
#define CMDS(cmds...)	(const u8 []){ cmds, 0 }

static const struct ata_eh_cmd_timeout_ent ata_eh_cmd_timeout_table[] = {
	{ .commands = CMDS(ATA_CMD_ID_ATA, ATA_CMD_ID_ATAPI),
	  .timeouts = ata_eh_identify_timeouts, },
	{ .commands = CMDS(ATA_CMD_READ_NATIVE_MAX, ATA_CMD_READ_NATIVE_MAX_EXT),
	  .timeouts = ata_eh_other_timeouts, },
	{ .commands = CMDS(ATA_CMD_SET_MAX, ATA_CMD_SET_MAX_EXT),
	  .timeouts = ata_eh_other_timeouts, },
	{ .commands = CMDS(ATA_CMD_SET_FEATURES),
	  .timeouts = ata_eh_other_timeouts, },
	{ .commands = CMDS(ATA_CMD_INIT_DEV_PARAMS),
	  .timeouts = ata_eh_other_timeouts, },
	{ .commands = CMDS(ATA_CMD_FLUSH, ATA_CMD_FLUSH_EXT),
	  .timeouts = ata_eh_flush_timeouts },
};
#undef CMDS
#define ATA_EH_CMD_TIMEOUT_TABLE_SIZE	ARRAY_SIZE(ata_eh_cmd_timeout_table)

static int ahci_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val);
static int ahci_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val);

static void ahci_dev_config(struct ata_device *dev);
static unsigned int ahci_dev_classify(struct ata_port *ap);
static void ahci_dev_config(struct ata_device *dev);

#ifdef CONFIG_SATA_AHCI_PLAT
int ahci_init_platform(void);
#endif

struct ata_port_operations ahci_ops = {
#ifdef CONFIG_SATA_PMP
	.inherits		= &sata_pmp_port_ops,
#endif
	.softreset		= ahci_softreset,
	.hardreset		= ahci_hardreset,
	.pmp_softreset		= ahci_softreset,
	.dev_config		= ahci_dev_config,
	.scr_read		= ahci_scr_read,
	.scr_write		= ahci_scr_write,
	/*.pmp_attach		= ahci_pmp_attach, */
	.port_start		= ahci_port_start,
	.port_stop		= ahci_port_stop,
};

enum board_ids {
	board_ahci
};

static const struct ata_port_info ahci_port_info[] = {
	[board_ahci] = {
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
		.port_ops = &ahci_ops,
	},
};

#ifdef CONFIG_SATA_AHCI_PLAT
u32 ahci_readl(void *addr);
void ahci_writel(u32 value, void *addr);
#else
static u32 ahci_readl(void *addr)
{
	u32 val = readl(addr);
	DPRINTK("R 0x%p => 0x%x\n", addr, val);
	return val;
}

static void ahci_writel(u32 value, void *addr)
{
	DPRINTK("W 0x%p <= 0x%x\n", addr, value);
	writel(value, addr);
}
#endif

#define ahci_writel_with_flush(v, a)	\
	do { ahci_writel((v), (a)); ahci_readl(a); } while (0)

static int ata_lookup_timeout_table(u8 cmd)
{
	int i;

	for (i = 0; i < ATA_EH_CMD_TIMEOUT_TABLE_SIZE; i++) {
		const u8 *cur;

		for (cur = ata_eh_cmd_timeout_table[i].commands; *cur; cur++)
			if (*cur == cmd)
				return i;
	}

	return -1;
}

/**
 *	ata_internal_cmd_timeout - determine timeout for an internal command
 *	@dev: target device
 *	@cmd: internal command to be issued
 *
 *	Determine timeout for internal command @cmd for @dev.
 *
 *	RETURNS:
 *	Determined timeout.
 */
unsigned long ata_internal_cmd_timeout(u8 cmd)
{
	int ent = ata_lookup_timeout_table(cmd);

	if (ent < 0)
		return ATA_EH_CMD_DFL_TIMEOUT;

	return ata_eh_cmd_timeout_table[ent].timeouts[0];
}

static void ahci_sata_dump_fis(const struct sata_fis_d2h *s)
{
	int is_h2d = 0;
	const struct sata_fis_h2d *h2ds = (struct sata_fis_h2d *)s;
	printf("---------------------\n");
	printf("Status FIS dump:\n");
	printf("fis_type:		%02x ", s->fis_type);
	switch (s->fis_type) {
		case SATA_FIS_TYPE_REGISTER_H2D:
			puts("H2D\n");
			is_h2d = 1;
			break;
		case SATA_FIS_TYPE_REGISTER_D2H:
			puts("D2H\n");
			break;
		case SATA_FIS_TYPE_DMA_ACT_D2H:
			puts("ACT D2H\n");
			break;
		case SATA_FIS_TYPE_DMA_SETUP_BI:
			puts("DMA SETUP BI\n");
			break;
		case SATA_FIS_TYPE_DATA_BI:
			puts("DATA BI\n");
			break;
		case SATA_FIS_TYPE_BIST_ACT_BI:
			puts("BIST ACT BI\n");
			break;
		case SATA_FIS_TYPE_PIO_SETUP_D2H:
			puts("PIO SETUP D2H\n");
			break;
		case SATA_FIS_TYPE_SET_DEVICE_BITS_D2H:
			puts("SET DEVICE BITS D2H\n");
			break;
		default:
			puts("UNKNOWN\n");
			break;
	}
	printf("pm_port_i:		%02x\n", s->pm_port_i);
	if (is_h2d) {
		printf("command:		%02x\n", h2ds->command);
	} else {
		printf("status:			%02x", s->status);
		if (s->status & ATA_BUSY)
			puts(" BSY");
		if (s->status & ATA_DRDY)
			puts(" DTDY");
		if (s->status & ATA_DF)
			puts(" DF");
		if (s->status & ATA_DRQ)
			puts(" DRQ");
		if (s->status & ATA_ERR)
			puts(" ERR");
		puts("\n");
	}
	printf("error:			%02x\n", s->error);
	printf("lba_low:		%02x\n", s->lba_low);
	printf("lba_mid:		%02x\n", s->lba_mid);
	printf("lba_high:		%02x\n", s->lba_high);
	printf("device:			%02x\n", s->device);
	printf("lba_low_exp:		%02x\n", s->lba_low_exp);
	printf("lba_mid_exp:		%02x\n", s->lba_mid_exp);
	printf("lba_high_exp:		%02x\n", s->lba_high_exp);
	printf("res1:			%02x\n", s->res1);
	printf("sector_count:		%02x\n", s->sector_count);
	printf("sector_count_exp:	%02x\n", s->sector_count_exp);
	if (is_h2d)
		printf("control:		%02x\n", h2ds->control);
	printf("---------------------\n");
}

#define ata_id_u32_t(id,n)	\
	(((u32)(id)[(n) + 1] << 16) | ((u32) (id)[(n)]))

#if 0
static void dump_ataid(hd_driveid_t *ataid)
{
	u8 product[ATA_ID_PROD_LEN+1];
	u8 revision[ATA_ID_FW_REV_LEN+1];
	u8 serial[ATA_ID_SERNO_LEN + 1];
	u8 firmware[ATA_ID_FW_REV_LEN + 1];
	debug("(49)ataid->capability = 0x%x\n", ataid->capability);
	debug("(53)ataid->field_valid =0x%x\n", ataid->field_valid);
	debug("(63)ataid->dma_mword = 0x%x\n", ataid->dma_mword);
	debug("(64)ataid->eide_pio_modes = 0x%x\n", ataid->eide_pio_modes);
	debug("(75)ataid->queue_depth = 0x%x\n", ataid->queue_depth);
	debug("(80)ataid->major_rev_num = 0x%x\n", ataid->major_rev_num);
	debug("(81)ataid->minor_rev_num = 0x%x\n", ataid->minor_rev_num);
	debug("(82)ataid->command_set_1 = 0x%x\n", ataid->command_set_1);
	debug("(83)ataid->command_set_2 = 0x%x\n", ataid->command_set_2);
	debug("(84)ataid->cfsse = 0x%x\n", ataid->cfsse);
	debug("(85)ataid->cfs_enable_1 = 0x%x\n", ataid->cfs_enable_1);
	debug("(86)ataid->cfs_enable_2 = 0x%x\n", ataid->cfs_enable_2);
	debug("(87)ataid->csf_default = 0x%x\n", ataid->csf_default);
	debug("(88)ataid->dma_ultra = 0x%x\n", ataid->dma_ultra);
	debug("(93)ataid->hw_config = 0x%x\n", ataid->hw_config);
	debug("(%d)ataid->capacity = 0x%x\n", ATA_ID_LBA_SECTORS,
	      ata_id_u32_t((u16 *)ataid, ATA_ID_LBA_SECTORS));
	debug("(%d)ataid->capacity_lba48 = 0x%x\n", ATA_ID_LBA48_SECTORS,
	      ata_id_u32_t((u16 *)ataid, ATA_ID_LBA48_SECTORS));
	ata_id_c_string((u16 *)ataid, serial, ATA_ID_SERNO,
			sizeof(serial));
	debug("(%d) serial = %s\n", ATA_ID_SERNO, serial);
	ata_id_c_string((u16 *)ataid, firmware, ATA_ID_FW_REV,
			sizeof(firmware));
	debug("(%d) firmware = %s\n", ATA_ID_FW_REV, firmware);
	ata_id_c_string((u16 *)ataid, product, ATA_ID_PROD,
			sizeof(product));
	debug("(%d) product = %s\n", ATA_ID_PROD, product);
	ata_id_c_string((u16 *)ataid, revision, ATA_ID_FW_REV,
			sizeof(revision));
	debug("(%d) revision = %s\n", ATA_ID_FW_REV, revision);
}

static void ahci_dump_cmd_slot(struct ahci_ioports *pp, unsigned int tag)
{
	DPRINTK("tag: %u\n", tag);
	debug("\topts: 0x%x\n", le32_to_cpu(pp->cmd_slot->opts));
	debug("\tstatus: 0x%x\n", le32_to_cpu(pp->cmd_slot->status));
	debug("\ttable address: 0x%x 0x%x\n",
	      le32_to_cpu(pp->cmd_slot->tbl_addr_hi),
	      le32_to_cpu(pp->cmd_slot->tbl_addr));
}
#endif

static void ahci_enable_ahci(void __iomem *mmio)
{
	int i;
	u32 tmp;

	DPRINTK("ENTER (%p)\n", mmio);
	/* turn on AHCI_EN */
	tmp = ahci_readl(mmio + HOST_CTL);
	if (tmp & HOST_AHCI_EN)
		return;

	/* Some controllers need AHCI_EN to be written multiple times.
	 * Try a few times before giving up.
	 */
	for (i = 0; i < 5; i++) {
		tmp |= HOST_AHCI_EN;
		ahci_writel(tmp, mmio + HOST_CTL);
		tmp = ahci_readl(mmio + HOST_CTL);	/* flush && sanity check */
		if (tmp & HOST_AHCI_EN)
			return;
		mdelay(10);
	}

	printf("%s: Failed to enable AHCI host\n", __func__);
}

/**
 *	ahci_save_initial_config - Save and fixup initial config values
 *	@hpriv: host private area to store config values
 *	@force_port_map: force port map to a specified value
 *	@mask_port_map: mask out particular bits from port map
 *
 *	Some registers containing configuration info might be setup by
 *	BIOS and might be cleared on reset.  This function saves the
 *	initial values of those registers into @hpriv such that they
 *	can be restored after controller reset.
 *
 *	If inconsistent, config values are fixed up by this function.
 *
 */
void ahci_save_initial_config(struct ahci_host_priv *hpriv,
			      unsigned int force_port_map,
			      unsigned int mask_port_map)
{
	void __iomem *mmio = hpriv->mmio;
	u32 cap, cap2, vers, port_map;
	int i;

	DPRINTK("(%p, 0x%x, 0x%x)\n", hpriv, force_port_map, mask_port_map);
	/* make sure AHCI mode is enabled before accessing CAP */
	ahci_enable_ahci(mmio);

	/* Values prefixed with saved_ are written back to host after
	 * reset.  Values without are used for driver operation.
	 */
	hpriv->saved_cap = cap = ahci_readl(mmio + HOST_CAP);
	hpriv->saved_port_map = port_map = ahci_readl(mmio + HOST_PORTS_IMPL);

	/* CAP2 register is only defined for AHCI 1.2 and later */
	vers = ahci_readl(mmio + HOST_VERSION);
	if ((vers >> 16) > 1 ||
	    ((vers >> 16) == 1 && (vers & 0xFFFF) >= 0x200))
		hpriv->saved_cap2 = cap2 = ahci_readl(mmio + HOST_CAP2);
	else
		hpriv->saved_cap2 = cap2 = 0;

	DPRINTK("saved cap: 0x%x, cap2: 0x%x saved port map: 0x%x\n",
		hpriv->saved_cap, hpriv->saved_cap2, hpriv->saved_port_map);

	/* some chips have errata preventing 64bit use */
	if ((cap & HOST_CAP_64) && (hpriv->flags & AHCI_HFLAG_32BIT_ONLY)) {
		printf("controller can't do 64bit DMA, forcing 32bit, cap: 0x%x, flags: 0x%x\n",
		       cap, hpriv->flags);
		cap &= ~HOST_CAP_64;
	}

	if ((cap & HOST_CAP_NCQ) && (hpriv->flags & AHCI_HFLAG_NO_NCQ)) {
		printf("controller can't do NCQ, turning off CAP_NCQ\n");
		cap &= ~HOST_CAP_NCQ;
	}

	if (!(cap & HOST_CAP_NCQ) && (hpriv->flags & AHCI_HFLAG_YES_NCQ)) {
		debug("controller can do NCQ, turning on CAP_NCQ\n");
		cap |= HOST_CAP_NCQ;
	}

	if ((cap & HOST_CAP_PMP) && (hpriv->flags & AHCI_HFLAG_NO_PMP)) {
		debug("controller can't do PMP, turning off CAP_PMP\n");
		cap &= ~HOST_CAP_PMP;
	}

	if ((cap & HOST_CAP_SNTF) && (hpriv->flags & AHCI_HFLAG_NO_SNTF)) {
		debug("controller can't do SNTF, turning off CAP_SNTF\n");
		cap &= ~HOST_CAP_SNTF;
	}

	if (!(cap & HOST_CAP_FBS) && (hpriv->flags & AHCI_HFLAG_YES_FBS)) {
		debug("controller can do FBS, turning on CAP_FBS\n");
		cap |= HOST_CAP_FBS;
	}

	if (force_port_map && port_map != force_port_map) {
		DPRINTK("forcing port_map 0x%x -> 0x%x\n",
			port_map, force_port_map);
		port_map = force_port_map;
	}

	if (mask_port_map) {
		printf("%s: masking port_map 0x%x -> 0x%x\n",
		       __func__, port_map, port_map & mask_port_map);
		port_map &= mask_port_map;
	}

	/* cross check port_map and cap.n_ports */
	if (port_map) {
		int map_ports = 0;

		for (i = 0; i < AHCI_MAX_PORTS; i++)
			if (port_map & (1 << i))
				map_ports++;

		/* If PI has more ports than n_ports, whine, clear
		 * port_map and let it be generated from n_ports.
		 */
		if (map_ports > ahci_nr_ports(cap)) {
			printf("%s: implemented port map (0x%x) contains more ports than nr_ports (%u), using nr_ports\n",
			       __func__, port_map, ahci_nr_ports(cap));
			port_map = 0;
		}
	}

	/* fabricate port_map from cap.nr_ports */
	if (!port_map) {
		port_map = (1 << ahci_nr_ports(cap)) - 1;
		printf("forcing PORTS_IMPL to 0x%x\n", port_map);

		/* write the fixed up value to the PI register */
		hpriv->saved_port_map = port_map;
	}

	/* record values to use during operation */
	hpriv->cap = cap;
	hpriv->cap2 = cap2;
	hpriv->port_map = port_map;
	DPRINTK("EXIT\n");
}

int ahci_host_init(struct ata_host *host)
{
	struct ahci_host_priv *hpriv = host->private_data;
	void __iomem *mmio = (void *)hpriv->mmio;
	u32 tmp, cmd;
	int i;
	ulong start;
#ifndef CONFIG_SATA_AHCI_PLAT
	pci_dev_t pdev = host->pdev;
	u16 tmp16;
	u16 vendor;
#endif

	DPRINTK("ENTER\n");

	/* Global controller reset */
	tmp = ahci_readl(mmio + HOST_CTL);
	if ((tmp & HOST_RESET) == 0)
		ahci_writel_with_flush(tmp | HOST_RESET, mmio + HOST_CTL);

	/* Reset must complete within 1 second or the hardware is fried */
	start = get_timer(0);
	do {
		tmp = ahci_readl(mmio + HOST_CTL);
	} while ((tmp & HOST_RESET) && (get_timer(start) < 1000));
	if (tmp & HOST_RESET) {
		printf("%s: Error: failed to reset host controller\n", __func__);
		return -1;
	}

	/* Enable the controller */
	ahci_writel_with_flush(HOST_AHCI_EN, mmio + HOST_CTL);

#ifndef CONFIG_SATA_AHCI_PLAT
	pci_read_config_word(pdev, PCI_VENDOR_ID, &vendor);

	if (vendor == PCI_VENDOR_ID_INTEL) {
		u16 tmp16;
		pci_read_config_word(pdev, 0x92, &tmp16);
		tmp16 |= 0xf;
		pci_write_config_word(pdev, 0x92, tmp16);
	}
#endif

	hpriv->cap = ahci_readl(mmio + HOST_CAP);
	hpriv->port_map = ahci_readl(mmio + HOST_PORTS_IMPL);
	host->n_ports = (hpriv->cap & 0x1f) + 1;
	DPRINTK("cap: 0x%x, port map: 0x%x, num ports: %d\n",
	      hpriv->cap, hpriv->port_map, host->n_ports);

	for (i = 0; i < host->n_ports; i++) {
		void *port_mmio = __ahci_port_base(host, i);

		/* Make sure port is not active */
		tmp = ahci_readl(port_mmio + PORT_CMD);
		if (tmp & (PORT_CMD_LIST_ON | PORT_CMD_FIS_ON |
			   PORT_CMD_FIS_RX | PORT_CMD_START)) {
			tmp &= ~(PORT_CMD_FIS_RX | PORT_CMD_START);
			ahci_writel_with_flush(tmp, port_mmio + PORT_CMD);
			/* Spec says 500msec for each bit */
			mdelay(1000);
		}

		DPRINTK("Spinning up device on SATA port %d\n", i);
		cmd = ahci_readl(port_mmio + PORT_CMD);
		cmd |= PORT_CMD_FIS_RX | PORT_CMD_SPIN_UP;
		ahci_writel_with_flush(cmd, port_mmio + PORT_CMD);

		/* Bring up SATA link */
		start = get_timer(0);
		do {
			tmp = ahci_readl(port_mmio + PORT_SCR_STAT);
		} while (((tmp & 0xf) != 3) &&
			 (get_timer(start) < WAIT_MS_LINKUP));
		if ((tmp & 0xf) != 3) {
			printf("SATA link %d timeout.\n", i);
			continue;
		}
		/* Clear error status */
		tmp = ahci_readl(port_mmio + PORT_SCR_ERR);
		if (tmp)
			ahci_writel(tmp, port_mmio + PORT_SCR_ERR);


		/* ack any pending irq events port this port */
		tmp = ahci_readl(port_mmio + PORT_IRQ_STAT);
		debug("PORT_IRQ_STAT 0x%x\n", tmp);
		if (tmp)
			ahci_writel(tmp, port_mmio + PORT_IRQ_STAT);
	}

#ifndef CONFIG_SATA_AHCI_PLAT
	pci_read_config_word(pdev, PCI_COMMAND, &tmp16);
	tmp |= PCI_COMMAND_MASTER;
	pci_write_config_word(pdev, PCI_COMMAND, tmp16);
#endif

	DPRINTK("EXIT\n");
	return 0;
}

static unsigned ahci_scr_offset(struct ata_port *ap, unsigned int sc_reg)
{
	static const int offset[] = {
		[SCR_STATUS]		= PORT_SCR_STAT,
		[SCR_CONTROL]		= PORT_SCR_CTL,
		[SCR_ERROR]		= PORT_SCR_ERR,
		[SCR_ACTIVE]		= PORT_SCR_ACT,
		[SCR_NOTIFICATION]	= PORT_SCR_NTF,
	};
	struct ahci_host_priv *hpriv = ap->host->private_data;

	if (sc_reg < ARRAY_SIZE(offset) &&
		(sc_reg != SCR_NOTIFICATION || (hpriv->cap & HOST_CAP_SNTF)))
		return offset[sc_reg];
	return 0;
}

static int ahci_scr_read(struct ata_link *link, unsigned int sc_reg, u32 *val)
{
	void __iomem *port_mmio;
	int offset;

	DPRINTK("(%d, %d, 0x%p)\n", link->ap->port_no, sc_reg, val);
	port_mmio = ahci_port_base(link->ap);
	offset = ahci_scr_offset(link->ap, sc_reg);

	debug("  offset: %d\n", offset);
	if (offset) {
		*val = ahci_readl(port_mmio + offset);
		return 0;
	}
	return -EINVAL;
}

static int ahci_scr_write(struct ata_link *link, unsigned int sc_reg, u32 val)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	int offset = ahci_scr_offset(link->ap, sc_reg);

	if (offset) {
		ahci_writel(val, port_mmio + offset);
		return 0;
	}
	return -EINVAL;
}

void ahci_start_engine(struct ata_port *ap)
{
	void *port_mmio = ahci_port_base(ap);
	u32 tmp;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	tmp = ahci_readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_START;
	ahci_writel_with_flush(tmp, port_mmio + PORT_CMD);
	DPRINTK("EXIT\n");
}

int ahci_stop_engine(struct ata_port *ap)
{
	void *port_mmio = ahci_port_base(ap);
	u32 tmp;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	tmp = ahci_readl(port_mmio + PORT_CMD);

	/* Check if HBA is idle */
	if ((tmp & (PORT_CMD_START | PORT_CMD_LIST_ON)) == 0)
		return 0;

	tmp &= ~PORT_CMD_START;
	ahci_writel(tmp, port_mmio + PORT_CMD);

	/* Wait for engine to stop.  This can take up to 500ms */
	tmp = ata_wait_register(port_mmio + PORT_CMD, PORT_CMD_LIST_ON,
				PORT_CMD_LIST_ON, 500);
	if (tmp & PORT_CMD_LIST_ON)
		return -EIO;

	return 0;
}

static void ahci_power_up(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 cmd;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	cmd = ahci_readl(port_mmio + PORT_CMD) & ~PORT_CMD_ICC_MASK;

	/* spin up device */
	if (hpriv->cap & HOST_CAP_SSS) {
		debug("  Spinning up drive\n");
		cmd |= PORT_CMD_SPIN_UP;
		ahci_writel(cmd, port_mmio + PORT_CMD);
	}

	/* wake up link */
	ahci_writel(cmd | PORT_CMD_ICC_ACTIVE, port_mmio + PORT_CMD);
	DPRINTK("EXIT\n");
}

static void ahci_start_fis_rx(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_host_priv *hpriv = ap->host->private_data;
	struct ahci_port_priv *pp = ap->private_data;
	u32 tmp;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	/* set FIS registers */
	if (hpriv->cap & HOST_CAP_64)
		ahci_writel((pp->cmd_slot_dma >> 16) >> 16,
			    port_mmio + PORT_LST_ADDR_HI);
		ahci_writel(pp->cmd_slot_dma & 0xffffffff, port_mmio + PORT_LST_ADDR);

	if (hpriv->cap & HOST_CAP_64)
		ahci_writel((pp->rx_fis_dma >> 16) >> 16,
			    port_mmio + PORT_FIS_ADDR_HI);
		ahci_writel(pp->rx_fis_dma & 0xffffffff, port_mmio + PORT_FIS_ADDR);

	/* enable FIS reception */
	tmp = ahci_readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_FIS_RX;
	ahci_writel_with_flush(tmp, port_mmio + PORT_CMD);
	DPRINTK("(%d port_cmd: 0x%x, cmd_slot_dma: %llx, rx_fis_dma: %llx)\n",
		ap->port_no, tmp, (u64)pp->cmd_slot_dma, (u64)pp->rx_fis_dma);

	DPRINTK("EXIT\n");
}

static int ahci_stop_fis_rx(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	u32 tmp;
	int rc = 0;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	/* disable FIS reception */
	tmp = ahci_readl(port_mmio + PORT_CMD);
	tmp &= ~PORT_CMD_FIS_RX;
	ahci_writel(tmp, port_mmio + PORT_CMD);

	/* Wait for completion, spec says 500ms */
	tmp = ata_wait_register(port_mmio + PORT_CMD, PORT_CMD_FIS_ON,
				PORT_CMD_FIS_ON, 1000);
	if (tmp & PORT_CMD_FIS_ON)
		rc = -EBUSY;
	DPRINTK("EXIT rc: %d\n", rc);
	return rc;
}

static void ahci_start_port(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	/* enable FIS reception */
	ahci_start_fis_rx(ap);

	/* enable DMA */
	if (!(hpriv->flags & AHCI_HFLAG_DELAY_ENGINE))
		ahci_start_engine(ap);
	DPRINTK("EXIT\n");
}

static void ahci_port_resume(struct ata_port *ap)
{
	DPRINTK("ENTER (%d)\n", ap->port_no);

	ahci_power_up(ap);
	ahci_start_port(ap);
#if 0
	/* TODO */
	if (sata_pmp_attached(ap)) {
		ahci_pmp_attach(ap);
	} else {
		ahci_pmp_detach(ap);
	}
#endif
	DPRINTK("EXIT\n");
}

int ahci_port_start(struct ata_port *ap)
{
	struct ahci_host_priv *hpriv = ap->host->private_data;
	struct ahci_port_priv *pp;
	void *mem;
	phys_addr_t mem_dma;
	size_t dma_sz, rx_fis_sz;
	int rc = 0;
	u32 cmd;
	void __iomem *port_mmio;

	DPRINTK("ENTER (%d)\n", ap->port_no);

	if (ap->private_data)
		pp = (struct ahci_port_priv *)ap->private_data;
	else
		pp = calloc(sizeof(*pp), 1);
	if (!pp)
		return -ENOMEM;

	/* Check FBS capability */
	if ((hpriv->cap & HOST_CAP_FBS) && sata_pmp_supported(ap)) {
		DPRINTK("FBS and PMP supported\n");
		port_mmio = ahci_port_base(ap);
		cmd = ahci_readl(port_mmio + PORT_CMD);
		DPRINTK("Port %d capable of FBS and PMP\n", ap->port_no);
		if (cmd & PORT_CMD_FBSCP) {
			pp->fbs_supported = true;
			DPRINTK("port %d can do FBS\n", ap->port_no);
		} else if (hpriv->flags & AHCI_HFLAG_YES_FBS) {
			DPRINTK("port %d can do FBS, forcing FBSCP\n",
			      ap->port_no);
			pp->fbs_supported = true;
		} else
			printf("port %d is not capable of FBS\n", ap->port_no);
	} else {
		DPRINTK("Host does not support FBS (cap: 0x%x, sata pmp supported: %d)\n",
			hpriv->cap & HOST_CAP_FBS, sata_pmp_supported(ap));
	}

	if (pp->fbs_supported) {
		dma_sz = AHCI_PORT_PRIV_FBS_DMA_SZ;
		rx_fis_sz = AHCI_RX_FIS_SZ * 16;
	} else {
		dma_sz = AHCI_PORT_PRIV_DMA_SZ;
		rx_fis_sz = AHCI_RX_FIS_SZ;
	}
	DPRINTK("FBS%s supported, dma size: 0x%x, fis size: 0x%x\n",
		pp->fbs_supported ? "" : " NOT", dma_sz, rx_fis_sz);

	mem = memalign(2048, dma_sz);
	if (!mem)
		return -ENOMEM;
	memset(mem, 0, dma_sz);
	mem_dma = virt_to_phys(mem);

	/*
	 * First item in chunk of DMA memory: 32-slot command table,
	 * 32 bytes each in size
	 */
	pp->cmd_slot = mem;
	pp->cmd_slot_dma = mem_dma;

	mem += AHCI_CMD_SLOT_SZ;
	mem_dma += AHCI_CMD_SLOT_SZ;

	/*
	 * Second item: Received-FIS area
	 */
	pp->rx_fis = mem;
	pp->rx_fis_dma = mem_dma;

	mem += rx_fis_sz;
	mem_dma += rx_fis_sz;

	/*
	 * Third item: data area for storing a single command
	 * and its scatter-gather table
	 */
	DPRINTK("pp: 0x%p, pp->cmd_tbl: %p, pp->cmd_tbl_dma: 0x%llx\n",
		pp, mem, mem_dma);
	pp->cmd_tbl = mem;
	pp->cmd_tbl_dma = mem_dma;

	/*
	 * Save off initial list of interrupts to be enabled.
	 * This could be changed later
	 */
	pp->intr_mask = DEF_PORT_IRQ;

	ap->private_data = pp;

	/* engage engines, captain */
	ahci_port_resume(ap);
	DPRINTK("EXIT %d\n", rc);
	return 0;
}

static int ahci_deinit_port(struct ata_port *ap, const char **emsg)
{
	int rc;

	DPRINTK("ENTER (%d)\n", ap->port_no);

	rc = ahci_stop_engine(ap);
	if (rc) {
		*emsg = "failed to stop engine";
		goto done;
	}

	/* Disable FIS reception */
	rc = ahci_stop_fis_rx(ap);
	if (rc)
		*emsg = "failed to stop FIS RX";
done:
	DPRINTK("EXIT\n");

	return rc;
}

void ahci_port_stop(struct ata_port *ap)
{
	int rc;
	const char *emsg = NULL;
	struct ahci_port_priv *pp = ap->private_data;

	DPRINTK("ENTER (%d)\n", ap->port_no);

	rc = ahci_deinit_port(ap, &emsg);
	if (rc)
		printf("%s (%d)\n", emsg, rc);

	if (!pp)
		return;

	/* Free up the port data structure */
	free(pp->cmd_slot);
	memset(pp, 0xbb, sizeof(*pp));
	free(pp);
	DPRINTK("EXIT\n");
}

/**
 * Returns the port link status after waiting up to 1 second for link up.
 *
 * @param ap	ATA port data structure
 *
 * @return	0 if link is down, 1 if link is up, -1 if other.
 */
static int ahci_port_link_status(struct ata_port *ap)
{
	void __iomem *mmio = ahci_port_base(ap);
	u32 tmp;
	ulong start;

	start = get_timer(0);
	do {
		tmp = ahci_readl(mmio + PORT_SCR_STAT);
	} while (((tmp & 0xd) != 1) && (get_timer(start) < 1000));
	switch (tmp & 0xf) {
	case 0:
		DPRINTK("No device detected for port %d\n", ap->port_no);
		return 0;
	case 1:
		DPRINTK("Device detected on port %d, phy communication NOT established\n",
			ap->port_no);
		return -1;
	case 3:
		DPRINTK("Device detected on port %d, phy communication established\n",
			ap->port_no);
		return 1;
	default:
		DPRINTK("Unexpected SSTATUS.DET value 0x%x on port %d\n",
			tmp & 0xf, ap->port_no);
		return -1;
	}
}

static int ahci_port_reset(struct ata_port *ap)
{
	void __iomem *mmio = ahci_port_base(ap);
	u32 tmp;

	DPRINTK("ENTER (%d)\n", ap->port_no);

	/* AHCI 1.3 spec section 10.4.2. */
	/* Clear PxCMD.ST */
	tmp = ahci_readl(mmio + PORT_CMD);
	tmp &= ~PORT_CMD_START;
	ahci_writel(tmp, mmio + PORT_CMD);
	/* Wait for PxCMD.CR to clear (up to 500ms) */
	DPRINTK("Waiting for PxCMD.CR to clear...\n");
	tmp = ata_wait_register(mmio + PORT_CMD, PORT_CMD_LIST_ON,
				PORT_CMD_LIST_ON, 500);
	if (tmp & PORT_CMD_LIST_ON)
		printf("%s(%d): port is stuck\n", __func__, ap->port_no);

	/* Set PxSCTL.DET to 1 */
	tmp = ahci_readl(mmio + PORT_SCR_CTL);
	tmp &= ~0xf;
	tmp |= 1;
	ahci_writel_with_flush(tmp, mmio + PORT_SCR_CTL);
	/* Wait at least 1ms */
	mdelay(2);
	/* Set PxSCTL.DET to 0 */
	tmp &= ~0xF;
	ahci_writel_with_flush(tmp, mmio + PORT_SCR_CTL);
	tmp = ahci_readl(mmio + PORT_CMD);
	mdelay(10);
	DPRINTK("Reading SSTATUS at 0x%p, PORT_CMD: 0x%x\n",
		mmio + PORT_SCR_STAT, tmp);
	DPRINTK("Waiting for SSTATUS.DET to be 1 or 3, mmio: 0x%p\n", mmio);
	tmp = ahci_port_link_status(ap);
	DPRINTK("SSTATUS: 0x%x\n", tmp);
	/* Wait for PxSSTS.DET to be set to 1 */
	tmp = ata_wait_register(mmio + PORT_SCR_STAT, 0xD, 0, 1000);
	if ((tmp & 0xd) != 1) {
		/* SSTATUS.DET can be 0 if no device is connected. */
		if (tmp == 0)
			DPRINTK("(%d) no device detected.\n", ap->port_no);
		else
			printf("%s: (%d) Error getting device detection status. PORT_SCR_STAT: 0x%x\n",
			       __func__, ap->port_no, tmp);
		return -1;
	}

	/* Write all 1s  to PxSERR */
	ahci_writel_with_flush(0xFFFFFFFF, mmio + PORT_SCR_ERR);

	/* When PxSCTL.DET is set to 1, the HBA shall reset PxTFD.STS to 0x7Fh
	 * and shall reset PxSSTS.DET to 0h.  When PxSCTL.DET is set to 0h,
	 * upon receiving a COMINIT from the attached device, PxTFD.STS.BSY
	 * shall be set to '1' by the HBA.
	 */
	DPRINTK("EXIT 0\n");
	return 0;
}

int ahci_kick_engine(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ahci_host_priv *hpriv = ap->host->private_data;
	u8 status = ahci_readl(port_mmio + PORT_TFDATA) & 0xff;
	u32 tmp;
	int busy, rc;

	DPRINTK("ENTRY (%d)\n", ap->port_no);
	rc = ahci_stop_engine(ap);
	if (rc) {
		DPRINTK("ahci_stop_engine returned %d, restarting\n", rc);
		goto out_restart;
	}

	/* Need to do CLO?  Always do CLO if PMP is attached (AHCI-1.3 9.2) */
	busy = status & (ATA_BUSY | ATA_DRQ);
	if (!busy && !sata_pmp_attached(ap)) {
		rc = 0;
		DPRINTK("PMP not attached\n");
		goto out_restart;
	}

	if (!(hpriv->cap & HOST_CAP_CLO)) {
		DPRINTK("command list override not supported\n");
		rc = -EOPNOTSUPP;
		goto out_restart;
	}

	/* Perform CLO */
	tmp = ahci_readl(port_mmio + PORT_CMD);
	tmp |= PORT_CMD_CLO;
	ahci_writel_with_flush(tmp, port_mmio + PORT_CMD);
	rc = 0;

	tmp = ata_wait_register(port_mmio + PORT_CMD, PORT_CMD_CLO,
				PORT_CMD_CLO, 500);
	if (tmp & PORT_CMD_CLO)
		rc = -EIO;

	/* restart engine */
out_restart:
	DPRINTK("Restarting engine\n");
	ahci_start_engine(ap);
	DPRINTK("EXIT\n");
	return rc;
}

static void ahci_fill_cmd_slot(struct ahci_port_priv *pp,
			       unsigned tag, u32 opts)
{
	phys_addr_t cmd_tbl_dma = pp->cmd_tbl_dma + tag * AHCI_CMD_TBL_SZ;

	DPRINTK("ENTRY (%p, %d, 0x%x)\n", pp, tag, opts);
	pp->cmd_slot[tag].opts = cpu_to_le32(opts);
	pp->cmd_slot[tag].status = 0;
	pp->cmd_slot[tag].tbl_addr = cpu_to_le32(cmd_tbl_dma & 0xffffffff);
	pp->cmd_slot[tag].tbl_addr_hi = cpu_to_le32((cmd_tbl_dma >> 16) >> 16);
	flush_dcache_range((ulong)&pp->cmd_slot[tag],
			   (ulong)(&pp->cmd_slot[tag] +
				sizeof(struct ahci_cmd_hdr)));
	debug("cmd table at 0x%p (0x%llx): 0x%x 0x%x 0x%x 0x%x\n",
	      pp->cmd_tbl, cmd_tbl_dma,
	      le32_to_cpu(pp->cmd_slot[tag].opts),
	      le32_to_cpu(pp->cmd_slot[tag].status),
	      le32_to_cpu(pp->cmd_slot[tag].tbl_addr),
	      le32_to_cpu(pp->cmd_slot[tag].tbl_addr_hi));
}

int ahci_exec_polled_cmd(struct ata_port *ap, int pmp, struct ata_taskfile *tf,
			 bool is_cmd, u16 flags, ulong timeout_msec)
{
	const int cmd_fis_len = 5;
	struct ahci_port_priv *pp = ap->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	u8 *fis = pp->cmd_tbl;
	u32 tmp;

	DPRINTK("ENTER (%d, 0x%x, %p, %d, 0x%x, %lu)\n", ap->port_no,
		pmp, tf, is_cmd, flags, timeout_msec);

	/* Prep command */
	ata_tf_to_fis(tf, pmp, is_cmd ? 1 : 0, fis);
	ahci_fill_cmd_slot(pp, 0, cmd_fis_len | flags | (pmp << 12));
	flush_dcache_range((ulong)fis, (ulong)fis + cmd_fis_len * 4);

	/* Issue and wait */
	ahci_writel(1, port_mmio + PORT_CMD_ISSUE);
	if (!timeout_msec)
		timeout_msec = ata_internal_cmd_timeout(tf->command);

	if (timeout_msec) {
		tmp = ata_wait_register(port_mmio + PORT_CMD_ISSUE, 1, 1,
					timeout_msec);
		if (tmp & 1) {
			printf("%s: timed out\n", __func__);
			ahci_kick_engine(ap);
			DPRINTK("EXIT -EBUSY\n");
			return -EBUSY;
		}
	} else {
		ahci_readl(port_mmio + PORT_CMD_ISSUE);	/* flush */
	}
	DPRINTK("EXIT 0\n");
	return 0;
}

static unsigned int ahci_dev_classify(struct ata_port *ap)
{
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ata_taskfile tf;
	u32 tmp;

	DPRINTK("ENTER (%d)\n", ap->port_no);
	tmp = ahci_readl(port_mmio + PORT_SIG);
	tf.lbah		= (tmp >> 24)	& 0xff;
	tf.lbam		= (tmp >> 16)	& 0xff;
	tf.lbal		= (tmp >> 8)	& 0xff;
	tf.nsect	= (tmp)		& 0xff;
	DPRINTK("  Port signature: 0x%x\n", tmp);

	return ata_dev_classify(&tf);
}

int ahci_do_softreset(struct ata_link *link, unsigned int *cls, int pmp,
		      ulong deadline,
		      int (*check_ready)(struct ata_link *link))
{
	struct ata_port *ap = link->ap;
	struct ahci_host_priv *hpriv = ap->host->private_data;
	const char *reason = NULL;
	ulong msecs = deadline;
	struct ata_taskfile tf;
	int rc;

	DPRINTK("ENTER (%d, %p, 0x%x, %lu, %p)\n", ap->port_no, cls, pmp,
		deadline, check_ready);

	/* Prepare for SRST (AHCI-1.1 10.4.1) */
	rc = ahci_kick_engine(ap);
	if (rc && rc != -EOPNOTSUPP)
		printf("ahci softreset failed to reset engine on port %d (errno=%d)\n",
		       ap->port_no, rc);

	ata_tf_init(link->device, &tf);

	DPRINTK("First D2H register FIS\n");
	/* Issue the first D2H Register FIS */
	tf.ctl |= ATA_SRST;
	rc = ahci_exec_polled_cmd(ap, pmp, &tf, false,
				  AHCI_CMD_RESET | AHCI_CMD_CLR_BUSY, msecs);
	if (rc) {
		rc = -EIO;
		reason = "1st FIS failed";
		DPRINTK("First FIS failed\n");
		goto fail;
	}

	/* Spec says at least 5us, but be generous */
	mdelay(1);

	/* Issue second D2H Register FIS */
	tf.ctl &= ~ATA_SRST;
	rc = ahci_exec_polled_cmd(ap, pmp, &tf, false, 0, 0);

	/* Wait for link to become ready */
	rc = ata_wait_after_reset(link, deadline, check_ready);
	if (rc == -EBUSY && hpriv->flags & AHCI_HFLAG_SRST_TOUT_IS_OFFLINE) {
		printf("ahci device on port %d not ready, treating as offline\n",
		       ap->port_no);
		*cls = ATA_DEV_NONE;
	} else if (rc) {
		/* link occupied, -ENODEV too is an error */
		reason = "device not ready";
		goto fail;
	} else {
		*cls = ahci_dev_classify(ap);
	}

	DPRINTK("EXIT, class=%u\n", *cls);
	return 0;

fail:
	printf("SATA softreset failed for port %d (%s)\n", ap->port_no, reason);
	return rc;
}

static int ahci_check_ready(struct ata_link *link)
{
	void __iomem *port_mmio = ahci_port_base(link->ap);
	u8 status = ahci_readl(port_mmio + PORT_TFDATA) & 0xff;

	return ata_check_ready(status);
}

int ahci_softreset(struct ata_link *link, unsigned int *cls,
		   ulong deadline)
{
	int pmp = sata_srst_pmp(link);

	return ahci_do_softreset(link, cls, pmp, deadline, ahci_check_ready);
}

void ahci_print_info(struct ata_host *host, const char *scc_s)
{
	struct ahci_host_priv *hpriv = host->private_data;
	void __iomem *mmio = hpriv->mmio;
	u32 vers, cap, cap2, impl, speed;
	const char *speed_s;

	vers = ahci_readl(mmio + HOST_VERSION);
	cap = hpriv->cap;
	cap2 = hpriv->cap2;
	impl = hpriv->port_map;

	speed = (cap >> 20) & 0xf;
	if (speed == 1)
		speed_s = "1.5";
	else if (speed == 2)
		speed_s = "3";
	else if (speed == 3)
		speed_s = "6";
	else
		speed_s = "?";

	printf("AHCI %02x%02x.%02x%02x "
	       "%u slots %u ports %s Gbps 0x%x impl %s mode\n"	,
	       (vers >> 24) & 0xff,
	       (vers >> 16) & 0xff,
	       (vers >> 8) & 0xff,
	       vers & 0xff,
	       ((cap >> 8) & 0x1f) + 1,
	       (cap & 0x1f) + 1,
	       speed_s,
	       impl,
	       scc_s);

	printf("flags: "
	       "%s%s%s%s%s%s%s"
	       "%s%s%s%s%s%s%s"
	       "%s%s%s%s%s%s\n",

	       cap & HOST_CAP_64 ? "64bit " : "",
	       cap & HOST_CAP_NCQ ? "ncq " : "",
	       cap & HOST_CAP_SNTF ? "sntf " : "",
	       cap & HOST_CAP_MPS ? "ilck " : "",
	       cap & HOST_CAP_SSS ? "stag " : "",
	       cap & HOST_CAP_ALPM ? "pm " : "",
	       cap & HOST_CAP_LED ? "led " : "",
	       cap & HOST_CAP_CLO ? "clo " : "",
	       cap & HOST_CAP_ONLY ? "only " : "",
	       cap & HOST_CAP_PMP ? "pmp " : "",
	       cap & HOST_CAP_FBS ? "fbs " : "",
	       cap & HOST_CAP_PIO_MULTI ? "pio " : "",
	       cap & HOST_CAP_SSC ? "slum " : "",
	       cap & HOST_CAP_PART ? "part " : "",
	       cap & HOST_CAP_CCC ? "ccc " : "",
	       cap & HOST_CAP_EMS ? "ems " : "",
	       cap & HOST_CAP_SXS ? "sxs " : "",
	       cap2 & HOST_CAP2_APST ? "apst " : "",
	       cap2 & HOST_CAP2_NVMHCI ? "nvmp " : "",
	       cap2 & HOST_CAP2_BOH ? "boh " : ""
	       );
}

#define MAX_DATA_BYTE_COUNT  (4*1024*1024)

static int ahci_fill_sg(struct ata_port *ap, unsigned char *buf, int buf_len)
{
	struct ahci_port_priv *pp = ap->private_data;
	struct ahci_sg *ahci_sg = pp->cmd_tbl + AHCI_CMD_TBL_HDR_SZ;
	u32 sg_count;
	int i;
	phys_addr_t paddr = virt_to_phys(buf);

	DPRINTK("(%d, 0x%p, %d)\n", ap->port_no, buf, buf_len);
	debug("pp: 0x%p\n", pp);
	debug("\tsg address: 0x%p (0x%llx)\n", ahci_sg,
	      pp->cmd_tbl_dma + AHCI_CMD_TBL_HDR_SZ);
	sg_count = ((buf_len - 1) / MAX_DATA_BYTE_COUNT) + 1;
	if (sg_count > AHCI_MAX_SG) {
		printf("Error: Too much data!\n");
		return -1;
	}

	for (i = 0; i < sg_count; i++) {
		DPRINTK("sg[%d].paddr: 0x%llx\n", i, paddr);
		ahci_sg->addr = cpu_to_le32(paddr & 0xffffffff);
		ahci_sg->addr_hi = cpu_to_le32((paddr >> 16) >> 16);
		ahci_sg->reserved = 0;
		ahci_sg->flags_size =
		cpu_to_le32(0x3fffff & (buf_len < MAX_DATA_BYTE_COUNT
						? (buf_len - 1)
						: (MAX_DATA_BYTE_COUNT - 1)));
#ifdef DEBUG
		debug("ahci_sg[%d](0x%p) addr=0x%08x, addr_hi=0x%08x, flags/size=0x%08x\n",
		      i, ahci_sg, le32_to_cpu(ahci_sg->addr),
		      le32_to_cpu(ahci_sg->addr_hi),
		      le32_to_cpu(ahci_sg->flags_size));
		print_buffer((ulong)buf, buf, 1,
			   le32_to_cpu(ahci_sg->flags_size) + 1, 0);
#endif
		ahci_sg++;
		buf_len -= MAX_DATA_BYTE_COUNT;
		paddr += MAX_DATA_BYTE_COUNT;
	}

	return sg_count;
}

static int ahci_exec_internal(struct ata_device *dev, struct ata_taskfile *tf,
			      const u8 *cdb, int dma_dir, void *buf,
			      unsigned int buflen, ulong timeout)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	unsigned int n_elem = 0;
	bool is_atapi = !!(tf->protocol & ATA_PROT_FLAG_ATAPI);
	void *cmd_tbl;
	u8 *rx_fis = pp->rx_fis;
	u32 opts;
	const u32 cmd_fis_len = 5;
	u32 tmp;

	DPRINTK("ENTER (%d, %p, %p, %d, %p, 0x%x, %lu)\n", dev->devno, tf, cdb,
		dma_dir, buf, buflen, timeout);

	if (ahci_port_link_status(ap) != 1) {
		DPRINTK("No link on port %d\n", ap->port_no);
		return -1;
	}

	cmd_tbl = pp->cmd_tbl;
	ata_tf_to_fis(tf, link->pmp, 1, cmd_tbl);
	if (is_atapi) {
		if (!cdb) {
			printf("Error: ATAPI requires cdb to be set!\n");
			return -1;
		}
		DPRINTK("Clearing and setting command table\n");
		memset(cmd_tbl + AHCI_CMD_TBL_CDB, 0, 32);
		memcpy(cmd_tbl + AHCI_CMD_TBL_CDB, cdb, ATAPI_CDB_LEN);
	}

	/* Some SATA bridges need us to indicate data xfer direction */
	if (tf->protocol == ATAPI_PROT_DMA && (dev->flags & ATA_DFLAG_DMADIR) &&
		dma_dir == DMA_FROM_DEVICE)
		tf->feature |= ATAPI_DMADIR;

	if (dma_dir != DMA_NONE) {
		n_elem = ahci_fill_sg(ap, buf, buflen);
		if (dma_dir == DMA_TO_DEVICE || dma_dir == DMA_BIDIRECTIONAL)
			flush_dcache_range((ulong)buf,
					   (ulong)((uint8_t *)buf + buflen));
	}

	opts = cmd_fis_len | (link->pmp << 12) | (n_elem << 16);

	if (tf->flags & ATA_TFLAG_WRITE)
		opts |= AHCI_CMD_WRITE;
	if (is_atapi)
		opts |= AHCI_CMD_ATAPI | AHCI_CMD_PREFETCH;

	ahci_fill_cmd_slot(pp, 0, opts);

	if (pp->fbs_enabled && pp->fbs_last_dev != link->pmp) {
		u32 fbs;

		fbs = ahci_readl(port_mmio + PORT_FBS);
		DPRINTK("FBS enabled (0x%x) and last dev not PMP\n", fbs);
		fbs &= ~(PORT_FBS_DEV_MASK | PORT_FBS_DEC);
		fbs |= link->pmp << PORT_FBS_DEV_OFFSET;
		ahci_writel_with_flush(fbs, port_mmio + PORT_FBS);
		pp->fbs_last_dev = link->pmp;
	}

	DPRINTK("Sending command issue\n");
	ahci_writel(1, port_mmio + PORT_CMD_ISSUE);

	tmp = ata_wait_register(port_mmio + PORT_CMD_ISSUE, 1, 1, timeout);
	if (tmp & 1) {
		ahci_kick_engine(ap);
		printf("SATA port %u timed out issuing command\n", ap->port_no);
		return -EBUSY;
	}
	if (dma_dir == DMA_FROM_DEVICE || dma_dir == DMA_BIDIRECTIONAL)
		invalidate_dcache_range((ulong)buf, (ulong)buf + buflen);

	if (tf->protocol == ATA_PROT_PIO && dma_dir == DMA_FROM_DEVICE) {
		ata_tf_from_fis(rx_fis + RX_FIS_PIO_SETUP, tf);
		tf->command = (rx_fis + RX_FIS_PIO_SETUP)[15];
	} else {
		ata_tf_from_fis(rx_fis + RX_FIS_D2H_REG, tf);
	}

	DPRINTK("EXIT 0\n");
	return 0;
}

#ifdef CONFIG_SATA_PMP
int ata_exec_internal(struct ata_device *dev, struct ata_taskfile *tf,
		      const u8 *cdb, int dma_dir, void *buf,
		      unsigned int buflen, ulong timeout)
{
	return ahci_exec_internal(dev, tf, cdb, dma_dir, buf, buflen, timeout);
}
#endif

static int ahci_identify_device(struct ata_device *dev, unsigned int *p_cls,
				unsigned int flags, u16 *id)
{
	struct ata_port *ap = dev->link->ap;
	struct ata_taskfile tf;
	unsigned int cls = *p_cls;
	int rc;

	DPRINTK("ENTER (%u, 0x%x, 0x%x, %p)\n", ap->port_no, cls, flags, id);

	memset(&tf, 0, sizeof(tf));
	ata_tf_init(dev, &tf);

	switch (cls) {
	case ATA_DEV_SEMB:
		cls = ATA_DEV_ATA;
		/* fall-through */
	case ATA_DEV_ATA:
		tf.command = ATA_CMD_ID_ATA;
		break;
	case ATA_DEV_ATAPI:
		tf.command = ATA_CMD_ID_ATAPI;
		break;
	default:
		printf("%s: Unsupported class 0x%x\n", __func__, cls);
		return -1;
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

	DPRINTK("Execing TF\n");
	rc = ahci_exec_internal(dev, &tf, NULL, DMA_FROM_DEVICE, id,
				sizeof(id[0]) * ATA_ID_WORDS, 5000);

	swap_buf_le16((u16 *)id, ATA_ID_WORDS);
#ifdef DEBUG
	DPRINTK("SATA port %d id (0x%p):\n", ap->port_no, id);
	print_buffer(0, id, 2,
		     sizeof(struct hd_driveid) / 2, 0);
#endif
	DPRINTK("EXIT %d\n", rc);
	return rc;
}

int ahci_scan_device(struct ata_device *dev, bool verbose)
{
	int rc;
	u16 *id;
	u8 serial[ATA_ID_SERNO_LEN + 1];
	u8 firmware[ATA_ID_FW_REV_LEN + 1];
	u8 product[ATA_ID_PROD_LEN + 1];
	u32 cls = ATA_DEV_ATA;
	struct ata_port *ap;
	void __iomem *port_mmio;
	u32 port_cmd;

	DPRINTK("ENTER (%d)\n", dev->devno);

	ap = dev->link->ap;
	port_mmio  = ahci_port_base(ap);
	ahci_stop_engine(ap);
	ahci_port_resume(ap);

	DPRINTK("port %d mmio: 0x%p\n", ap->port_no, port_mmio);
	port_cmd = ahci_readl(port_mmio + PORT_CMD);
	if ((port_cmd & PORT_CMD_CPD) & !(port_cmd & PORT_CMD_CPS)) {
		DPRINTK("Port %d disabled, port cmd: 0x%x\n", ap->port_no,
			port_cmd);
		return 1;
	}

	if (verbose)
		printf("SATA#%d\n", dev->devno);

	DPRINTK("Resetting port %d\n", ap->port_no);
	rc = ahci_port_reset(ap);
	if (rc) {
		DPRINTK("Error resetting SATA#%d port %d\n",
			dev->devno, ap->port_no);
		return rc;
	}

	DPRINTK("Performing softreset\n");
	rc = ahci_softreset(dev->link, &cls, 5000);
	if (!rc) {
		DPRINTK("Setting device %d class to %d\n", dev->devno, cls);
		dev->class = cls;
		if (cls == ATA_DEV_PMP) {
			DPRINTK("Attaching PMP\n");
			rc = sata_pmp_attach(dev);
			if (rc) {
				printf("%s: dev: %d, error attaching PMP\n",
				       __func__, dev->devno);
			}
			DPRINTK("EXIT (pmp)\n");
			return rc;
		}
	} else {
		DPRINTK("Class: %d\n", cls);
		DPRINTK("EXIT rc: %d\n", rc);
		return rc;
	}
	id = dev->id;
	rc = ahci_identify_device(dev, &cls, 0, id);
	dev->n_sectors = dev->n_native_sectors = ata_id_n_sectors(id);

	if (ata_id_has_lba(id)) {
		dev->flags |= ATA_DFLAG_LBA;
#ifdef CONFIG_LBA48
		if (ata_id_has_lba48(id)) {
			dev->flags |= ATA_DFLAG_LBA48;
			dev->max_sectors = ATA_MAX_SECTORS_LBA48;
			if (dev->n_sectors >= (1UL << 28) &&
			    ata_id_has_flush_ext(id))
				dev->flags |= ATA_DFLAG_FLUSH_EXT;
			sata_dev_desc[dev->devno].lba48 = true;
		} else
#endif
			dev->max_sectors = ATA_MAX_SECTORS;
	}

	DPRINTK("Updating SATA device %d\n", dev->devno);
	/* Serial number */
	ata_id_c_string(id, serial, ATA_ID_SERNO, sizeof(serial));
	ata_id_c_string(id, firmware, ATA_ID_FW_REV, sizeof(firmware));
	ata_id_c_string(id, product, ATA_ID_PROD, sizeof(product));
	strncpy((char *)sata_dev_desc[dev->devno].product, (char *)serial,
		sizeof(sata_dev_desc[dev->devno].product));
	strncpy(sata_dev_desc[dev->devno].revision, (char *)firmware,
		sizeof(sata_dev_desc[dev->devno].revision));
	strncpy(sata_dev_desc[dev->devno].vendor, (char *)product,
		sizeof(sata_dev_desc[dev->devno].vendor));
	sata_dev_desc[dev->devno].lba = ata_id_n_sectors(id);
	sata_dev_desc[dev->devno].type = DEV_TYPE_HARDDISK;
	sata_dev_desc[dev->devno].blksz = ATA_BLOCKSIZE;
	sata_dev_desc[dev->devno].lun = 0;
	sata_dev_desc[dev->devno].priv = dev;
#ifdef DEBUG
	ata_dump_id(id);
#endif

#ifdef CONFIG_AHCI_SETFEATURE_XFER
	ahci_set_feature(dev->link->ap);
#endif

	DPRINTK("EXIT (dev: %d, port: %d, rc: 0)\n", dev->devno, ap->port_no);
	return 0;
}

int ahci_hardreset(struct ata_link *link, unsigned int *cls,
		   unsigned long deadline)
{
	const unsigned long timing[] = { 100, 2000, 5000};
	struct ata_port *ap = link->ap;
	struct ahci_port_priv *pp = ap->private_data;
	u8 *d2h_fis = pp->rx_fis + RX_FIS_D2H_REG;
	struct ata_taskfile tf;
	bool online;
	int rc;

	DPRINTK("ENTER (%d, %p, %lu)\n", ap->port_no, cls, deadline);

	ahci_stop_engine(ap);

	/* Clear D2H reception area to properly wait for D2H FIS */
	ata_tf_init(link->device, &tf);
	tf.command = 0x80;
	ata_tf_to_fis(&tf, 0, 0, d2h_fis);

	rc = sata_link_hardreset(link, timing, deadline, &online,
				 ahci_check_ready);

	ahci_start_engine(ap);

	if (online)
		*cls = ahci_dev_classify(ap);

	DPRINTK("EXIT, rc: %d, class: 0x%x\n", rc, *cls);
	return rc;
}

static void ahci_dev_config(struct ata_device *dev)
{
	struct ahci_host_priv *hpriv = dev->link->ap->host->private_data;

	if (hpriv->flags & AHCI_HFLAG_SECT255) {
		dev->max_sectors = 255;
		printf("SB600 AHCI: Limiting to 255 sectors per command\n");
	}
}

#ifndef CONFIG_SATA_AHCI_PLAT
static int ahci_init_one(pci_dev_t pdev)
{
	u16 vendor;
	struct ata_host *host;
	struct ata_port *port;
	struct ata_link *link;
	struct ahci_host_priv *hpriv;
	u32 mmio;
	int port_no;
	int n_ports;
	int host_idx = -1;
	int rc;
	struct ata_port_info pi = ahci_port_info[board_ahci];
	const struct ata_port_info *ppi[] = { &pi, NULL };

	mmio = (u32)pci_map_bar(pdev, AHCI_PCI_BAR, PCI_REGION_MEM);
	DPRINTK("pdev: 0x%x, ahci mmio: 0x%x\n", pdev, mmio);

	/* JMicron-specific fixup, put in AHCI mode */
	pci_read_config_word(pdev, PCI_VENDOR_ID, &vendor);
	if (vendor == 0x197b) {
		u32 conf1, conf5;
		u16 id;

		pci_write_config_byte(pdev, 0x41, 0xa1);
		conf1 = pci_read_config_dword(pdev, 0x40, &conf1);
		conf5 = pci_read_config_dword(pdev, 0x80, &conf5);
		conf1 &= ~0x00CFF302;	/* Clear bit 1, 8, 9, 12-19, 22, 23 */
		conf5 &= ~(1 << 24);	/* Clear bit 24 */
		pci_read_config_word(pdev, PCI_DEVICE_ID, &id);
		switch (id) {
		case 0x2360:	/* Single SATA port */
		case 0x2362:	/* Dual SATA port */
			/* The controller should be in single function ahci mode */
			conf1 |= 0x002A100;	/* Set 8, 13, 15, 17 */
			break;
		case 0x2361:
		case 0x2363:
			/* Redirect IDE second PATA port to the right spot */
			conf1 |= 0x00C2A1B3;	/* Set 0, 1, 4, 5, 7, 8, 13, 15, 17, 22, 23 */
			break;
		}
		pci_write_config_dword(pdev, 0x40, conf1);
		pci_write_config_dword(pdev, 0x80, conf5);
	}

	hpriv = calloc(sizeof(*hpriv), 1);
	if (!hpriv)
		return -ENOMEM;
	hpriv->mmio = (void *)mmio;

	ahci_save_initial_config(hpriv, 0, 0);

	if (hpriv->cap & HOST_CAP_PMP)
		pi.flags |= ATA_FLAG_PMP;

	n_ports = max(ahci_nr_ports(hpriv->cap), fls(hpriv->port_map));

	host = ata_host_alloc_pinfo(ppi, n_ports);
	if (!host)
		return -ENOMEM;

	host->private_data = hpriv;

	for (host_idx = 0; host_idx < CONFIG_SYS_SATA_MAX_DEVICE; host_idx++) {
		if (sata_host_desc[host_idx] == NULL)  {
			if (host_idx + host->n_ports >= CONFIG_SYS_SATA_MAX_DEVICE) {
				printf("Too many SATA devices found, increase CONFIG_SYS_SATA_MAX_DEVICE, host_idx: %d, n_ports: %d\n",
				       host_idx, host->n_ports);
				goto err_out;
			}
			host->index = host_idx;
			sata_host_desc[host_idx] = host;
			break;
		}
	}
	DPRINTK("last port index: %d, number of ports for host %d: %d\n",
		ahci_last_port_idx, host_idx, host->n_ports);

	for (port_no = 0; port_no < host->n_ports; port_no++) {
		port = host->ports[port_no];
		if (!(hpriv->port_map & (1 << port_no)))
			port->link.ap->ops = &ata_dummy_port_ops;
		sata_port_desc[ahci_last_port_idx++] = port;
		link = &port->link;
		DPRINTK("port: %d, ahci_last_device_idx: %d, nr_pmp_links: %d\n",
			port_no, ahci_last_device_idx, port->nr_pmp_links);
		if (ahci_last_device_idx + port->nr_pmp_links >= CONFIG_SYS_SATA_MAX_DEVICE) {
			printf("Too many SATA devices found, increase CONFIG_SYS_SATA_MAX_DEVICE, last device index: %d, num PMP links: %d\n",
			       ahci_last_device_idx, port->nr_pmp_links);
			goto err_out;
		}
		DPRINTK("Starting port %d\n", link->ap->port_no);
		ahci_port_start(link->ap);
		DPRINTK("Stopping engine %d\n", link->ap->port_no);
		ahci_stop_engine(link->ap);
		sata_device_desc[ahci_last_device_idx] = &link->device[0];
		link->device[0].devno = ahci_last_device_idx;
		DPRINTK("Scanning device %d\n", link->device[0].devno);
		rc = ahci_scan_device(&link->device[0], false);
		if (rc)
			sata_device_desc[ahci_last_device_idx] = NULL;

		ahci_last_device_idx++;
	}
	DPRINTK("EXIT (0)\n");
	return 0;

err_out:
	DPRINTK("EXIT (-1)\n");
	return -1;
}
#endif

static int ahci_sata_rw_cmd(struct ata_device *dev, lbaint_t start,
			    lbaint_t blkcnt, void *buffer, bool is_wr,
			    bool is_lba48)
{
	struct ata_taskfile tf;
	u32 flags = 0;
	int rc;

	DPRINTK("ENTER (%d, 0x%llx, 0x%llx, %p, %d, %d)\n",
		dev->devno, start, blkcnt, buffer, is_wr, is_lba48);
	if (!blkcnt)
		return 0;

	ata_tf_init(dev, &tf);
	if (is_wr)
		flags |= ATA_TFLAG_WRITE;

	if (ata_build_rw_tf(&tf, dev, start, blkcnt, flags, 0)) {
		printf("Error building rw taskfile\n");
		return -1;
	}

	rc = ahci_exec_internal(dev, &tf, NULL,
				is_wr ? DMA_TO_DEVICE : DMA_FROM_DEVICE,
				buffer, blkcnt * ATA_SECT_SIZE,
				5000 + 10 * blkcnt);
	DPRINTK("EXIT (%d)\n", rc);
	return rc;
}

static lbaint_t ahci_sata_read_write(struct ata_device *dev, lbaint_t blknr,
				     lbaint_t blkcnt, void *buffer, bool is_wr)
{
	lbaint_t start, blks, max_blks;
	void *addr;
	bool is_lba48;
	u32 num_blks;
	int rc = 0;

	start = blknr;
	blks = blkcnt;
	addr = buffer;

	if (!dev->link) {
		printf("SATA device %d not initialized\n", dev->devno);
		return 0;
	}
	is_lba48 = !!(dev->flags & ATA_DFLAG_LBA48);

	max_blks = is_lba48 ? ATA_MAX_SECTORS_LBA48 : ATA_MAX_SECTORS;

	do {
		num_blks = min(max_blks, blks);
		rc = ahci_sata_rw_cmd(dev, start, num_blks, addr, is_wr,
				      is_lba48);
		if (rc)
			return blkcnt - blks;

		start += num_blks;
		blks -= num_blks;
		addr += ATA_SECT_SIZE * num_blks;
	} while (blks != 0);

	DPRINTK("Read 0x%llx blocks\n", (u64)blkcnt);

	return blkcnt;
}

int scan_sata(int dev)
{
	struct ata_device *ahci_dev;
	int rc;

	DPRINTK("(%d)\n", dev);
	ahci_dev = sata_device_desc[dev];

	rc = ahci_scan_device(ahci_dev, true);

	DPRINTK("EXIT (%d)\n", rc);
	return rc;
}

lbaint_t __sata_read(int dev, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
{
	lbaint_t count;
	DPRINTK("ENTER (%d, 0x%llx, 0x%llx, %p)\n", dev, (u64)blknr,
		(u64)blkcnt, buffer);
	count = ahci_sata_read_write(sata_device_desc[dev], blknr, blkcnt,
				     buffer, false);
	DPRINTK("EXIT (0x%llx)\n", (u64)count);
	return count;
}
lbaint_t sata_read(int dev, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
	__attribute__((weak, alias("__sata_read")));

lbaint_t __sata_write(int dev, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
{
	lbaint_t count;
	DPRINTK("ENTER (%d, 0x%llx, 0x%llx, %p)\n", dev, (u64)blknr,
		(u64)blkcnt, buffer);
	count = ahci_sata_read_write(sata_device_desc[dev], blknr, blkcnt,
				     buffer, true);
	DPRINTK("EXIT (0x%llx)\n", (u64)count);
	return count;
}
lbaint_t sata_write(int dev, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
	__attribute__((weak, alias("__sata_write")));

#ifdef CONFIG_AHCI_SETFEATURES_XFER
int ahci_set_feature(struct ata_port *ap)
{
	volatile struct ahci_port_priv *pp = ap->private_data;
	void __iomem *port_mmio = ahci_port_base(ap);
	struct ata_taskfile tf;
	int pmp;
	int port = ap->port_no;
	int rc;

	/*set feature */
	DPRINTK("(%d)\n", port);
	pmp = sata_srst_pmp(&ap->link);
	ata_tf_init(ap->link.device, &tf);
	tf.feature = SETFEATURES_XFER;
	tf.command = ATA_CMD_SET_FEATURES;
	tf.flags = ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE | ATA_TFLAG_POLLING;
	tf.protocol = ATA_PROT_NODATA;

	switch (ap->udma_mask) {
	case ATA_UDMA7:
		tf.nsect = XFER_UDMA_7;
		DPRINTK("(%d), UDMA7\n", port);
		break;
	case ATA_UDMA6:
		tf.nsect = XFER_UDMA_6;
		DPRINTK("(%d), UDMA6\n", port);
		break;
	case ATA_UDMA5:
		tf.nsect = XFER_UDMA_5;
		DPRINTK("(%d), UDMA5\n", port);
		break;
	case ATA_UDMA4:
		tf.nsect = XFER_UDMA_4;
		DPRINTK("(%d), UDMA4\n", port);
		break;
	case ATA_UDMA3:
		tf.nsect = XFER_UDMA_3;
		DPRINTK("(%d), UDMA3\n", port);
		break;
	default:
		printf("%s: Unknown UDMA mask 0x%x for port %d\n",
		       __func__, ap->udma_mask, port);
	}

	DPRINTK("exec polled command\n");
	rc = ahci_exec_polled_cmd(ap, pmp, &tf, 1, 0, 0);
	if (rc) {
		printf("%s port %d setfeature failed\n", __func__, port);
		printf("set feature error (port %d)!\n", port);
		DPRINTK("port interrupt status: 0x%x, cmd issue: 0x%x\n",
			ahci_readl(port_mmio + PORT_IRQ_STAT),
			ahci_readl(port_mmio + PORT_CMD_ISSUE));
		DPRINTK("phy error: 0x%x\n",
			ahci_readl(port_mmio + PORT_SCR_ERR));
		DPRINTK("udma mask: 0x%x\n", ap->udma_mask);
		ahci_sata_dump_fis((struct sata_fis_d2h *)pp->cmd_tbl);
		DPRINTK("EXIT -1\n");
		return -1;
	}
	DPRINTK("EXIT 0\n");
	return 0;
}
#endif


int __init_sata(int dev)
{
#ifndef CONFIG_SATA_AHCI_PLAT
	pci_dev_t pcidev;
	int host_num = 0;
#endif
	int rc = 0;
	static bool initted = false;
	int port_num = 0;
	int i;

	DPRINTK("ENTRY (%d)\n", dev);
	if (initted) {
		DPRINTK("Already initialized\n");
		if (dev < CONFIG_SYS_SATA_MAX_DEVICE &&
		    sata_device_desc[dev] != NULL) {
			return 0;
		} else {
			DPRINTK("dev %d not present\n", dev);
			sata_dev_desc[dev].type = DEV_TYPE_UNKNOWN;
			return -1;
		}
	}

	for (i = 0; i < CONFIG_SYS_SATA_MAX_DEVICE; i++)
		sata_dev_desc[i].type = DEV_TYPE_UNKNOWN;
#ifndef CONFIG_SATA_AHCI_PLAT
	do {
		pcidev = pci_find_class(1	/* storage */,
					6	/* SATA */,
					1	/* AHCI */,
					host_num++);

		if (pcidev == -1)
			break;

		DPRINTK("Calling ahci_init_one(0x%x)\n", pcidev);
		rc = ahci_init_one(pcidev);
		if (rc) {
			DPRINTK("ahci_init_one(0x%x) returned %d\n", pcidev, rc);
			return rc;
		}
	} while (pcidev >= 0 && !rc);
#else
	rc = ahci_init_platform();
	if (rc) {
		DPRINTK("ahci_init_platform() returned %d\n", rc);
		return rc;
	}
#endif
	/* Now that we've initialized all of the hosts it's time to register
	 * them.  In Linux this triggers an asynchronous port probe but
	 * we  have to do it synchronously.
	 */

	DPRINTK("ahci_last_port_idx: %d\n", ahci_last_port_idx);
	for (port_num = 0; port_num < ahci_last_port_idx; port_num++) {
		struct ata_port *ap = sata_port_desc[port_num];
		if (!ap) {
			DPRINTK("Port id %d does not exist, skipping\n",
				port_num);
			continue;
		}

		/* set SATA cable type if still unset */
		if (ap->cbl == ATA_CBL_NONE && (ap->flags & ATA_FLAG_SATA))
			ap->cbl = ATA_CBL_SATA;

		/* init sata_spd_limit to the current value */
		sata_link_init_spd(&(ap->link));
		if (ap->slave_link)
			sata_link_init_spd(ap->slave_link);

		DPRINTK("Starting port %d\n", ap->port_no);
		if (ahci_port_start(ap)) {
			printf("Can not start port %d\n", ap->port_no);
			continue;
		}
	}

	initted = true;
	DPRINTK("(%d) finished successfully\n", dev);

	return 0;
}

int init_sata(int dev)	__attribute__((weak, alias("__init_sata")));
