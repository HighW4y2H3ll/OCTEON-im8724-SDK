/*
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
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
#include <miiphy.h>
#include <phy.h>
#include <net.h>
#include <libfdt.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_board_vsc8488.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_eth.h>

/**
 * This file contains routines common for the Vitesse vsc8488 PHY device
 * based boards, especially the SNIC10xxx boards.
 */
DECLARE_GLOBAL_DATA_PTR;

/**
 * @INTERNAL
 * Configures the Vitesse vsc8488 for use on boards like the SNIC10xxx based
 * boards using the values in the flat device tree.
 *
 * @param ethdev - ethernet device to initialize PHY for
 *
 * @returns 0 for success, error otherwise
 */
static int octeon_fdt_vsc8488_config(const struct eth_device *ethdev)
{
	const void *fdt = gd->fdt_blob;
	struct phy_device *phydev;
	int phy_offset;
	uint32_t txon_gpio;
	uint32_t rx_eq;
	uint32_t tx_preemph;
	uint32_t tx_out_driver_ctl1;
	uint32_t tx_out_driver_ctl2;
	const uint32_t *nodep;
	const struct octeon_eth_info *octinfo =
				 (struct octeon_eth_info *)ethdev->priv;

	debug("%s: Configuring Vitesse vsc8488 PHY device for %s\n",
	      __func__, ethdev->name);

	phydev = octinfo->phydev;

	if (!phydev) {
		printf("%s: Error: could not get phy device for %s\n",
		       __func__, ethdev->name);
		return -1;
	}

	phy_offset = octeon_fdt_find_phy(phydev->dev);
	if (phy_offset < 0) {
		printf("%s: Error: could not find phy info for phy device\n",
		       __func__);
		return -1;
	}
	if (fdt_node_check_compatible(fdt, phy_offset, "vitesse,vsc8488")) {
		printf("%s: Phy is not compatible with Vitesse VSC8488\n",
		       __func__);
		return -1;
	}
	tx_preemph = getenv_ulong("vsc8488_preemph", 16, 0xffffffff);
	if (tx_preemph != 0xffffffff) {
		printf("Overriding PHY XS XAUI TX Pre-emphasis Control register to 0x%x\n",
		       tx_preemph);
		phy_write(phydev, 4, 0x8011, tx_preemph);
	} else {
		nodep = fdt_getprop(fdt, phy_offset, "vitesse,tx_preemphasis",
				    NULL);
		if (nodep) {
			tx_preemph = fdt32_to_cpu(*nodep);
			debug("%s: Writing 0x%x to TX pre-emphasis\n",
			      __func__, tx_preemph);
			phy_write(phydev, 4, 0x8011, tx_preemph);
		}
	}

	rx_eq = getenv_ulong("vsc8488_rxeq", 16, 0xffffffff);
	if (rx_eq != 0xffffffff) {
		printf("Overriding PHY XS XAUI Rx Equalization Control with 0x%x\n",
		       rx_eq);
		phy_write(phydev, 4, 0x8010, rx_eq);
	} else {
		nodep = fdt_getprop(fdt, phy_offset, "vitesse,rx_equalization",
				    NULL);
		if (nodep) {
			rx_eq = fdt32_to_cpu(*nodep);
			debug("%s: Writing 0x%x to RX equalization\n",
			      __func__, rx_eq);
			phy_write(phydev, 4, 0x8010, rx_eq);
		}
	}

	tx_out_driver_ctl1 = getenv_ulong("vsc8488_txdrvctrl1", 16, 0xffffffff);
	if (tx_out_driver_ctl1 != 0xffffffff) {
		printf("Overriding TX Output Driver Control 1 with 0x%x\n",
		       tx_out_driver_ctl1);
		phy_write(phydev, 1, 0x8014, tx_out_driver_ctl1);
	} else {
		nodep = fdt_getprop(fdt, phy_offset, "vitesse,txout_driver_ctrl1", NULL);
		if (nodep) {
			tx_out_driver_ctl1 = fdt32_to_cpu(*nodep);
			debug("%s: Writing 0x%x to TX Output driver control 1\n",
			      __func__, tx_out_driver_ctl1);
			phy_write(phydev, 1, 0x8013, tx_out_driver_ctl1);
		}
	}

	tx_out_driver_ctl2 = getenv_ulong("vsc8488_txdrvctrl2", 16, 0xffffffff);
	if (tx_out_driver_ctl2 != 0xffffffff) {
		printf("Overriding TX Output Driver Control 2 with 0x%x\n",
		       tx_out_driver_ctl2);
		phy_write(phydev, 1, 0x8014, tx_out_driver_ctl2);
	} else {
		nodep = fdt_getprop(fdt, phy_offset,
				    "vitesse,txout_driver_ctrl2", NULL);
		if (nodep) {
			tx_out_driver_ctl2 = fdt32_to_cpu(*nodep);
			debug("%s: Writing 0x%x to TX Output driver control 2\n",
			      __func__, tx_out_driver_ctl2);
			phy_write(phydev, 1, 0x8014, tx_out_driver_ctl2);
		}
	}

	nodep = fdt_getprop(fdt, phy_offset, "txon", NULL);
	if (nodep) {
		txon_gpio = fdt32_to_cpu(*nodep);
		debug("%s: Turning on transmit with GPIO %d\n",
		      __func__, txon_gpio);
		phy_write(phydev, 0x1e, 0x100 + 2 * txon_gpio, 0);
	}

	return 0;
}

/**
 * Configures all Vitesse vsc8488 devices found.
 *
 * @return 0 for success, error otherwise.
 */
int octeon_vsc8488_config(void)
{
	struct eth_device *start = eth_get_dev();
	struct eth_device *ethdev = start;
	struct octeon_eth_info *oct_eth_info;

	if (!start) {
		debug("%s: No Ethernet devices found\n", __func__);
		return 0;
	}
	do {
		oct_eth_info = (struct octeon_eth_info *)ethdev->priv;
		if (oct_eth_info->phy_device_type == VITESSE) {
			if (octeon_fdt_vsc8488_config(ethdev)) {
				printf("%s: Error configuring Ethernet device %s\n",
				       __func__, ethdev->name);
				return -1;
			}
		}
		ethdev = ethdev->next;
	} while (ethdev && ethdev != start);
	return 0;
}

/**
 * This function tunes the QLM for use by the Vitesse vsc8488 PHY.
 *
 * @param qlm - qlm number to tune
 */
void octeon_board_vsc8488_qlm_tune(int qlm)
{
	if (!OCTEON_IS_MODEL(OCTEON_CN66XX) &&
	    !OCTEON_IS_MODEL(OCTEON_CN61XX) &&
	    !OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		printf("%s: Not supported on this OCTEON model yet\n", __func__);
		return;
	}
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_hf_ls_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_hf_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_lf_ls_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_lf_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_eq_gen1", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_eq_gen2", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_cap_gen1", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_cap_gen2", 0);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_hs_ls_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_hf_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_lf_ls_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_lf_byp", 31);
	/* Assert serdes_tx_byp to force the new settings to override the
	   QLM default. */
	cvmx_qlm_jtag_set(qlm, -1, "serdes_tx_byp", 1);
}
