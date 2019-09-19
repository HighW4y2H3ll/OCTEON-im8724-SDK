/*
 * (C) Copyright 2004 - 2013
 * Cavium Inc.
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
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>

DECLARE_GLOBAL_DATA_PTR;

/* Device tree */
extern char __dtb_begin;
extern char __dtb_end;
extern char __dtb_68xx_begin;
extern char __dtb_68xx_end;
extern char __dtb_78xx_begin;
extern char __dtb_78xx_end;
extern char __dtb_73xx_begin;
extern char __dtb_73xx_end;
extern char __dtb_75xx_begin;
extern char __dtb_75xx_end;

int checkboard(void)
{
	if (OCTEON_IS_MODEL(OCTEON_CN31XX)
	    || OCTEON_IS_MODEL(OCTEON_CN30XX)
	    || OCTEON_IS_MODEL(OCTEON_CN50XX)) {
		/* Enable RGMII/GMII interface block */
		cvmx_gmxx_inf_mode_t mode;
		mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));
		mode.s.en = 1;
		cvmx_write_csr(CVMX_GMXX_INF_MODE(0), mode.u64);
	}
	/* See if we need to adjust DRAM width for CN3020 simulation */
	if (OCTEON_IS_MODEL(OCTEON_CN3020)) {
		/* We are a 3020, so make the DDR interface 32 bits */
		cvmx_lmc_ctl_t lmc_ctl;
		lmc_ctl.u64 = cvmx_read_csr(CVMX_LMC_CTL);
		lmc_ctl.cn31xx.mode32b = 1;
		cvmx_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);
	}
#ifdef CONFIG_OCTEON_QLM
	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		/* DLM0 to SGMII */
		octeon_configure_qlm(0, 1250, CVMX_QLM_MODE_SGMII_DISABLED, 0, 0, 0, 0);
	/* cn76xx is a subset of cn78xx, check 76xx before checking 78xx */
	} else if (OCTEON_IS_MODEL(OCTEON_CN76XX)) {
		octeon_configure_qlm(2, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 0);
		octeon_configure_qlm(3, 3125, CVMX_QLM_MODE_XAUI, 0, 0, 2, 0);
		octeon_configure_qlm(4, 103125, CVMX_QLM_MODE_XFI, 0, 0, 2, 0);
	} else if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		int node;
		for (node = 0; node < CVMX_MAX_NODES; node++) {
			if (!(gd->arch.node_mask & (1 << node)))
				continue;
			/* Must match device tree definitions. */
			octeon_configure_qlm_cn78xx(node, 0, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 0);
			octeon_configure_qlm_cn78xx(node, 1, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 0);
			octeon_configure_qlm_cn78xx(node, 4, 1250, CVMX_QLM_MODE_ILK, 0, 0, 2, 0);
			octeon_configure_qlm_cn78xx(node, 5, 3125, CVMX_QLM_MODE_XAUI, 0, 0, 2, 0);
			octeon_configure_qlm_cn78xx(node, 6, 103125, CVMX_QLM_MODE_XLAUI, 0, 0, 2, 0);
			octeon_configure_qlm_cn78xx(node, 7, 103125, CVMX_QLM_MODE_XFI, 0, 0, 2, 0);
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		/* GSER0,1 = PCIe, GSER4 (DLM) = SATA, GSER2,3 = BGX{0,1},
		 * GSER5,6 (DLM) = BGX2 */
		octeon_configure_qlm(2, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 0);
		octeon_configure_qlm(3, 3125, CVMX_QLM_MODE_XAUI, 0, 0, 2, 0);
		octeon_configure_qlm(5, 1250, CVMX_QLM_MODE_SGMII_2X1, 0, 0, 2, 0);
		octeon_configure_qlm(6, 103125, CVMX_QLM_MODE_XFI_1X2, 0, 0, 2, 0);
	} else
#endif
	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SRIO_1X4, 0, 0, 2, 0);
		octeon_configure_qlm(3, 3125, CVMX_QLM_MODE_SRIO_1X4, 0, 0, 2, 0);
		octeon_configure_qlm(4, 1250, CVMX_QLM_MODE_SGMII_2X1, 0, 0, 2, 0);
		octeon_configure_qlm(5, 103125, CVMX_QLM_MODE_XFI_1X2, 0, 0, 2, 0);
	}
	return 0;
}

void octeon_led_str_write(const char *str)
{
	/* Not implemented on simulator */
	return;
}

int board_fdt_init_f(void)
{
	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		gd->fdt_blob = &__dtb_78xx_begin;
	else if (OCTEON_IS_MODEL(OCTEON_CN73XX))
		gd->fdt_blob = &__dtb_73xx_begin;
	else if (OCTEON_IS_MODEL(OCTEON_CNF75XX))
		gd->fdt_blob = &__dtb_75xx_begin;
	else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		gd->fdt_blob = &__dtb_68xx_begin;
	else
		gd->fdt_blob = &__dtb_begin;

	return fdt_check_header(gd->fdt_blob);
}
void board_fdt_get_limits(char **begin, char **end)
{
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		*begin = &__dtb_78xx_begin;
		*end = &__dtb_78xx_end;
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		*begin = &__dtb_73xx_begin;
		*end = &__dtb_73xx_end;
	} else if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		*begin = &__dtb_75xx_begin;
		*end = &__dtb_75xx_end;
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		*begin = &__dtb_68xx_begin;
		*end = &__dtb_68xx_end;
	} else {
		*begin = &__dtb_begin;
		*end = &__dtb_end;
	}
}

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *alias_prop;
	int aliases;
	int i;
	char fdt_key[20];
	int m, node;

	aliases = fdt_path_offset(working_fdt, "/aliases");

	for (i = 0; i < 2; i++) {
		char fdt_key[20];
		sprintf(fdt_key, "mix%d", i);
		/* Some models do not have mgmt port and some have only one. */
		alias_prop = fdt_getprop(working_fdt, aliases, fdt_key, NULL);

		if (alias_prop) {
			int mix = fdt_path_offset(working_fdt, alias_prop);
			int del = 0;
			if (i == 0 && mix >= 0
			    && (OCTEON_IS_MODEL(OCTEON_CN3XXX)
				|| OCTEON_IS_MODEL(OCTEON_CN58XX)
				|| OCTEON_IS_MODEL(OCTEON_CN70XX)))
				del = 1;
			else if (i == 1 && mix >= 0
				 && (OCTEON_IS_MODEL(OCTEON_CN3XXX)
				     || OCTEON_IS_MODEL(OCTEON_CN58XX)
				     || OCTEON_IS_MODEL(OCTEON_CN56XX)
				     || OCTEON_IS_MODEL(OCTEON_CN70XX)))
				del = 1;
			if (del) {
				debug("Deleting %s\n", fdt_key);
				fdt_nop_node(working_fdt, mix);
				fdt_nop_property(working_fdt, aliases, fdt_key);
			}
		}
	}

	/* delete nodes not discovered from oci_init() from device-tree */
	m = gd->arch.node_mask;
	for (node = 0; node < CVMX_MAX_NODES; node++) {
		if (m & (1 << node)) {
			/* keep 'node' in device-tree */
			continue;
		} else {
			sprintf(fdt_key, "%d,none", node);
			octeon_fdt_patch((void *)(gd->fdt_blob), fdt_key,
					 "cavium,node-trim");
		}
	}
	return 0;
}

int board_early_init_f(void)
{
#if 0
	gd->arch.node_mask = 1;
#if CONFIG_OCTEON_SIM_NO_DDR
	/* Get the number of nodes from -numnodes option. */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		int nodes;
		nodes = *((uint32_t *) 0x9ffffffc);
		gd->arch.node_mask = (1 << nodes) - 1;
	}
#endif
	debug("node_mask = 0x%x\n", gd->arch.node_mask);
#endif
	return 0;
}

int early_board_init(void)
{
	gd->arch.board_desc.board_type = CVMX_BOARD_TYPE_SIM;
	gd->board_type = gd->arch.board_desc.board_type;
	gd->arch.board_desc.rev_major = 1;
	gd->arch.board_desc.rev_minor = 0;

	gd->cpu_clk = cvmx_sim_magic_get_cpufreq() * 1000000;

	/* Make up some MAC addresses */
	gd->arch.mac_desc.count = 255;
	gd->arch.mac_desc.mac_addr_base[0] = 0x00;
	gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
	gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
	gd->arch.mac_desc.mac_addr_base[3] = 0xbe;
	gd->arch.mac_desc.mac_addr_base[4] = 0xef;
	gd->arch.mac_desc.mac_addr_base[5] = 0x00;

	return 0;
}
