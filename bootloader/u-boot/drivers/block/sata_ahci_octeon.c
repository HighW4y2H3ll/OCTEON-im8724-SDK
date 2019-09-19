/*
 * Copyright (C) Cavium, Inc. 2014
 * Author: Aaron Williams <Aaron.Williams@cavium.com>
 * 	Updated to follow SATA drivers
 *
 * SATA glue for Cavium Octeon III SOCs
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/cvmx-sata-defs.h>
#include <asm/arch/cvmx-qlm.h>
#include <libata.h>
#include <ata.h>
#include <linux/ctype.h>
#include <ahci.h>
#include <malloc.h>
#include <libfdt.h>
#include <fdt.h>
#include <fdtdec.h>
#include <asm/arch/cvmx-helper-fdt.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/cvmx-gpio-defs.h>
#include <asm/gpio.h>

#define AHCI_FAKE_ADDR		0xC8000000
#define CVMX_SATA_UAHC_GBL_BASE	CVMX_SATA_UAHC_GBL_CAP
#define OCTEON_MAX_SATA_PORTS	2

extern struct ata_port_operations ahci_ops;

struct octeon_sata_port_info {
	int led_gpio;
	int cold_presence_detect_gpio;
	int cold_presence_power_gpio;
	int mech_presence_switch_gpio;
	bool fis_enabled;
	bool external_port;
	bool hot_pluggable;
};

static const struct ata_port_info octeon_ahci_port_info = {
	.flags		= AHCI_FLAG_COMMON,
	.pio_mask	= ATA_PIO4,
	.udma_mask	= ATA_UDMA6,
	.port_ops 	= &ahci_ops,
};

static struct octeon_sata_port_info port_info[OCTEON_MAX_SATA_PORTS];

extern block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

extern struct ata_port *sata_port_desc[CONFIG_SYS_SATA_MAX_DEVICE];
extern struct ata_host *sata_host_desc[CONFIG_SYS_SATA_MAX_DEVICE];
extern struct ata_device *sata_device_desc[CONFIG_SYS_SATA_MAX_DEVICE];
extern hd_driveid_t *ataid[AHCI_MAX_PORTS];
extern struct ata_port_operations ahci_ops;
extern struct ahci_probe_ent *probe_ent;

extern int ahci_last_port_idx;
extern int ahci_last_device_idx;

extern void ahci_save_initial_config(struct ahci_host_priv *hpriv,
				     unsigned int force_port_map,
				     unsigned int mask_port_map);

#ifdef DEBUG
static const char *ahci_reg_to_str(volatile const void *addr)
{
	uint32_t addr32 = (uint32_t)addr;
	if ((addr32 & 0xff00) == 0)
		switch (addr32 & 0xff) {
		case 0:		return "HOST_CAP";
		case 4:		return "HOST_CTL";
		case 8:		return "HOST_IRQ_STAT";
		case 0xc:	return "HOST_PORTS_IMPL";
		case 0x10:	return "HOST_VERSION";
		case 0x1c:	return "HOST_EM_LOC";
		case 0x20:	return "HOST_EM_CTL";
		case 0x24:	return "HOST_CAP2";
		default:	return "Unknown";
		}
	else
		switch(addr32 & 0x7f) {
		case 0x0:	return "PORT_LST_ADDR_LO";
		case 0x4:	return "PORT_LST_ADDR_HI";
		case 0x8:	return "PORT_FIS_ADDR_LO";
		case 0xc:	return "PORT_FIS_ADDR_HI";
		case 0x10:	return "PORT_IRQ_STAT";
		case 0x14:	return "PORT_IRQ_MASK";
		case 0x18:	return "PORT_CMD";
		case 0x20:	return "PORT_TFDATA";
		case 0x24:	return "PORT_SIG";
		case 0x28:	return "PORT_SCR (PORT_SCR_STAT)";
		case 0x2c:	return "PORT_SCR_CTL";
		case 0x30:	return "PORT_SCR_ERR";
		case 0x34:	return "PORT_SCR_ACT";
		case 0x38:	return "PORT_CMD_ISSUE";
		case 0x3c:	return "PORT_SNTF";
		case 0x40:	return "PORT_FBS";
		case 0x44:	return "PORT_DEVSLP";
		default:	return "Unknown";
		}
}
#endif

void ahci_writel(u32 value, volatile void *addr)
{
	u64 addr64 = ((u32)addr - AHCI_FAKE_ADDR) + CVMX_SATA_UAHC_GBL_BASE;
#ifdef DEBUG
	debug("%s: 0x%llx (%s) <= 0x%x\n", __func__, addr64 & 0xffff,
	      ahci_reg_to_str(addr), value);
#endif
	cvmx_write64_uint32(addr64, value);
}

u32 ahci_readl(volatile const void *addr)
{
	u64 addr64 = ((u32)addr - AHCI_FAKE_ADDR) + CVMX_SATA_UAHC_GBL_BASE;
	u32 value;
	value = cvmx_read64_uint32(addr64);
#ifdef DEBUG
	debug("%s(0x%p (0x%llx (%s))) <= 0x%x\n", __func__, addr, addr64 & 0xffff,
	      ahci_reg_to_str(addr), value);
#endif
	return value;
}

void ata_readl(u32 addr) __attribute__((alias("ahci_readl")));
void ata_writel(u32 val, u32 addr) __attribute__((alias("ahci_writel")));

#define ahci_writel_with_flush(v, a)	\
		do { ahci_writel((v), (a)); ahci_readl(a); } while (0)

static int ahci_octeon_configure_ports(void)
{
	int offset = -1;
	int reg;
	int phandle;
	const uint32_t *prop;
	int len;
	union cvmx_gpio_sata_ctl gpio_sata_ctl;
	void *fdt = working_fdt;

	for (offset = 0; offset < OCTEON_MAX_SATA_PORTS; offset++) {
		port_info[offset].cold_presence_detect_gpio = -1;
		port_info[offset].cold_presence_power_gpio = -1;
		port_info[offset].led_gpio = -1;
		port_info[offset].mech_presence_switch_gpio = -1;
	}
	do {
		offset = fdt_node_offset_by_compatible(fdt, offset,
					"cavium,octeon-7130-sata-uahc-port");
		if (offset < 0)
			break;
		reg = fdtdec_get_int(fdt, offset, "reg", -1);
		if (reg < 0) {
			printf("Error getting \"reg\" field from SATA UAHC port in device tree\n");
			continue;
		}
		if (reg >= OCTEON_MAX_SATA_PORTS) {
			printf("%s: Error: SATA port %d out of range in UAHC section of device tree\n",
			       __func__, reg);
			continue;
		}
		port_info[reg].fis_enabled = fdtdec_get_bool(fdt, offset,
							     "fis-switching");
		port_info[reg].external_port =
				fdtdec_get_bool(fdt, offset, "external-port");
		port_info[reg].hot_pluggable =
				fdtdec_get_bool(fdt, offset,
						"hot-pluggable");
		prop = fdt_getprop(fdt, offset,
				   "led-gpio", &len);
		if (!prop || len != 8) {
			port_info[reg].led_gpio = -1;
		} else {
			phandle = fdt32_to_cpu(*prop);
			if (cvmx_fdt_get_gpio_type(fdt, phandle, NULL) !=
			    CVMX_GPIO_PIN_OCTEON) {
				printf("%s: Invalid GPIO type for SATA port %d in device tree\n",
				       __func__, reg);
				continue;
			}
			port_info[reg].led_gpio = fdt32_to_cpu(prop[1]);
		}

		prop = fdt_getprop(fdt, offset,
				   "cold-presence-power", &len);
		if (!prop || len != 8) {
			port_info[reg].cold_presence_power_gpio = -1;
		} else {
			phandle = fdt32_to_cpu(*prop);
			if (cvmx_fdt_get_gpio_type(fdt, phandle, NULL) !=
				CVMX_GPIO_PIN_OCTEON) {
				printf("%s: Invalid GPIO type for SATA port %d in device tree\n",
				       __func__, reg);
				continue;
			}
			port_info[reg].cold_presence_power_gpio =
							fdt32_to_cpu(prop[1]);
		}

		prop = fdt_getprop(fdt, offset,
				   "cold-presence-detect", &len);
		if (!prop || len != 8) {
			port_info[reg].cold_presence_power_gpio = -1;
		} else {
			phandle = fdt32_to_cpu(*prop);
			if (cvmx_fdt_get_gpio_type(fdt, phandle, NULL) !=
							CVMX_GPIO_PIN_OCTEON) {
				printf("%s: Invalid GPIO type for SATA port %d in device tree\n",
				       __func__, reg);
				continue;
			}
			port_info[reg].cold_presence_detect_gpio =
							fdt32_to_cpu(prop[1]);
		}

		prop = fdt_getprop(fdt, offset,
				   "mechanical-presence-switch", &len);
		if (!prop || len != 8) {
			port_info[reg].mech_presence_switch_gpio = -1;
		} else {
			phandle = fdt32_to_cpu(*prop);
			if (cvmx_fdt_get_gpio_type(fdt, phandle, NULL) !=
							CVMX_GPIO_PIN_OCTEON) {
				printf("%s: Invalid GPIO type for SATA port %d in device tree\n",
				       __func__, reg);
				continue;
			}
			port_info[reg].mech_presence_switch_gpio =
							fdt32_to_cpu(prop[1]);
		}

	} while (offset >= 0);

	/* Configure the GPIO inputs for cold presence, mechanical presence
	 * switch, activity LEDs, etc.
	 */
	gpio_sata_ctl.u64 = cvmx_read_csr(CVMX_GPIO_SATA_CTL);

	for (reg = 0; reg < OCTEON_MAX_SATA_PORTS; reg++) {
		/* Configure GPIO for activity */
		if (port_info[reg].led_gpio >= 0)
			cvmx_gpio_cfg_sel(0, port_info[reg].led_gpio,
					  reg == 0 ? 0x17 : 0x18);
		if (port_info[reg].cold_presence_power_gpio >= 0)
			cvmx_gpio_cfg_sel(0,
					  port_info[reg].cold_presence_power_gpio,
					  reg == 0 ? 0x15 : 0x16);

		if (port_info[reg].cold_presence_detect_gpio >= 0) {
			if (reg == 0)
				gpio_sata_ctl.s.sel3 =
					port_info[0].cold_presence_detect_gpio;
			else
				gpio_sata_ctl.s.sel4 =
					port_info[0].cold_presence_detect_gpio;
		}

		if (port_info[reg].mech_presence_switch_gpio >= 0) {
			if (reg == 0)
				gpio_sata_ctl.s.sel1 =
					port_info[reg].mech_presence_switch_gpio;
			else
				gpio_sata_ctl.s.sel2 =
					port_info[reg].mech_presence_switch_gpio;
		}
	}

	cvmx_write_csr(CVMX_GPIO_SATA_CTL, gpio_sata_ctl.u64);
	return 0;
}
/**
 * Initializes the shim for the proper endian swapping
 *
 * @return 0 for success
 */
int ahci_octeon_config(void)
{
	cvmx_sata_uctl_shim_cfg_t shim_cfg;
	/* cvmx_sata_uctl_ctl_t uctl_ctl; */
	/* u64 core_clock; */
	int qlm;

	DPRINTK("ENTER\n");
	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		qlm = 2;
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		qlm = 4;
	} else {
		printf("SATA not supported on this Octeon model\n");
		return -1;
	}

	if (cvmx_qlm_get_mode(qlm) != CVMX_QLM_MODE_SATA_2X1) {
		DPRINTK("QLM %d not in SATA mode.\n", qlm);
		return -1;
	}
	shim_cfg.u64 = cvmx_read_csr(CVMX_SATA_UCTL_SHIM_CFG);
	shim_cfg.s.dma_endian_mode = 1;
	/* Use 1 for LE mode when running BE, or 3 for BE mode running BE */
	shim_cfg.s.csr_endian_mode = 3;	/* Don't byte swap */
	shim_cfg.s.dma_read_cmd = 1; /* No allocate L2C */
	cvmx_write_csr(CVMX_SATA_UCTL_SHIM_CFG, shim_cfg.u64);

	return 0;
}

int ahci_init_platform(void)
{
	int rc = -1;
	struct ata_port *port = NULL;
	struct ata_port *ap;
	struct ata_link *link = NULL;
	struct ahci_host_priv *hpriv;
	struct ata_host *host = NULL;
	struct ata_port_info pi = octeon_ahci_port_info;
	const struct ata_port_info *ppi[] = { &pi, NULL };
	int port_no;
	int host_idx = -1;
	int n_ports;

	debug("%s() ENTER\n", __func__);

	if (ahci_octeon_config()) {
		DPRINTK("SATA not enabled\n");
		return 0;
	}

	pi.flags = AHCI_FLAG_COMMON;
	pi.pio_mask = ATA_PIO4;
	pi.udma_mask = ATA_UDMA7;
	pi.port_ops = &ahci_ops;

	hpriv = calloc(sizeof(*hpriv), 1);
	if (!hpriv)
		return -ENOMEM;

	hpriv->mmio = (void *)AHCI_FAKE_ADDR;
	ahci_save_initial_config(hpriv, 0, 0);
	hpriv->flags = 0;

	if (hpriv->cap & HOST_CAP_PMP)
		pi.flags |= ATA_FLAG_PMP;

	n_ports = max(ahci_nr_ports(hpriv->cap), fls(hpriv->port_map));

	debug("%s: Allocating pinfo for %d ports\n", __func__, n_ports);
	host = ata_host_alloc_pinfo(ppi, n_ports);
	if (!host)
		return -ENOMEM;

	host->private_data = hpriv;

	for (host_idx = 0; host_idx < CONFIG_SYS_SATA_MAX_DEVICE; host_idx++) {
		if (sata_host_desc[host_idx] == NULL)  {
			if (host_idx + host->n_ports > CONFIG_SYS_SATA_MAX_DEVICE) {
				printf("Too many SATA devices found, increase CONFIG_SYS_SATA_MAX_DEVICE, host index: %d, num ports: %d\n",
				       host_idx, host->n_ports);
				goto err_out;
			}
			host->index = host_idx;
			sata_host_desc[host_idx] = host;
			break;
		}
	}

	for (port_no = 0; port_no < host->n_ports; port_no++) {
		DPRINTK("Initializing port %d\n", port_no);
		port = host->ports[port_no];
		if (!(hpriv->port_map & (1 << port_no)))
			port->link.ap->ops = &ata_dummy_port_ops;

		sata_port_desc[ahci_last_port_idx++] = port;
		link = &port->link;

		DPRINTK("port %d, ahci_last_device_idx: %d, nr_pmp_links: %d\n",
			port_no, ahci_last_device_idx, port->nr_pmp_links);
		if (ahci_last_device_idx + port->nr_pmp_links >= CONFIG_SYS_SATA_MAX_DEVICE) {
			printf("Too many SATA devices found on port, increase CONFIG_SYS_SATA_MAX_DEVICE (currently %d)\n",
			       CONFIG_SYS_SATA_MAX_DEVICE);
			goto err_out;

		}
		DPRINTK("Configuring PxCMD register for Octeon\n");
		ap = port->link.ap;
		ahci_port_start(ap);
		DPRINTK("Stopping engine %d %d\n", port_no, ap->port_no);
		ahci_stop_engine(link->ap);
		sata_device_desc[ahci_last_device_idx] = &link->device[0];
		DPRINTK("Assigning port %d device number %d\n",
			port_no, ahci_last_device_idx);
		link->device[0].devno = ahci_last_device_idx;
		DPRINTK("Scanning engine %d %d, device: %d\n",
			port_no, ap->port_no, link->device[0].devno);
		rc = ahci_scan_device(&link->device[0], false);
		if (rc)
					sata_device_desc[ahci_last_device_idx] = NULL;
					ahci_last_device_idx++;
				}
	ahci_octeon_configure_ports();

	DPRINTK("EXIT (0)\n");
	return 0;

err_out:
	debug("%s: err_out\n", __func__);
	if (host)
		free(host);
	if (hpriv)
		free(hpriv);

	return rc;
}

void ahci_shutdown_platform(struct ata_host *host)
{
	int i;

	debug("%s: ENTRY\n", __func__);
	for (i = 0; i < host->n_ports; i++) {
		if (host->ports[i])
			free(host->ports[i]);
	}
	memset(host, 0, sizeof(*host));
	if (host->private_data)
		free(host->private_data);
	free(host);
}
