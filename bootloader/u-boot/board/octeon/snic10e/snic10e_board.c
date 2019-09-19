/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
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
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt.h>
#include <pca953x.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * Fixup the device tree based on board version
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	int offset;

	if (gd->arch.board_desc.rev_major < 5) {
		fdt_key = "0,vitesse";
	} else {
		fdt_key = "0,ti";
		offset = -1;
		do {
			offset = fdt_node_offset_by_compatible(gd->fdt_blob,
							       offset,
							       "ethernet,sfp-slot");
			if (offset > 0) {
				fdt_nop_property(gd->fdt_blob, offset,
						 "tx_disable");
				fdt_nop_property(gd->fdt_blob, offset,
						 "tx_error");
				fdt_nop_property(gd->fdt_blob, offset, "rx_los");
			}
		} while (offset > 0);
	}

	debug("SNIC 10e rev %d.%d, using %s for strip key\n",
	      gd->arch.board_desc.rev_major, gd->arch.board_desc.rev_minor,
	      fdt_key);
	octeon_fdt_patch_rename((void *)(gd->fdt_blob), fdt_key,
				"cavium,phy-trim", true, NULL, NULL);

	return 0;
}

void board_mdio_init(void)
{
	static const uint8_t buffer1[] = { 0x1e, 0x01, 0x08, 0x00, 0x00 };
	static const uint8_t buffer2[] = { 0x1e, 0x01, 0x2c, 0x00, 0x00 };
	octeon_board_phy_init();


	if (gd->arch.board_desc.rev_major < 2) {
		/* We only need to use twsi for the small number of rev 1
		 * boards.
		 * For production boards (rev 2 and later) the MDIO writes above
		 * serve the same purpose.
		 */
		i2c_set_bus_num(1);
		i2c_write(0x40, 5, 1, (void *)buffer1, sizeof(buffer1));
		i2c_write(0x40, 5, 1, (void *)buffer2, sizeof(buffer2));
		i2c_set_bus_num(0);
	} else if (gd->arch.board_desc.rev_major >= 5) {
		/* TI PHY, need to set up GPIO */
		/* While ideally this should be handled by the device tree
		 * it's too much of a pain to do this in U-Boot where U-Boot
		 * lacks proper GPIO asbstraction so we'll just do it here.
		 */
		/* All pins are inputs except 1 and 5 which are outputs */
		pca953x_set_dir(CONFIG_SYS_I2C_PCA953X_BUS,
				CONFIG_SYS_I2C_PCA953X_ADDR,
				0, 0xDD);
		/* Enable the transmit */
		pca953x_set_val(CONFIG_SYS_I2C_PCA953X_BUS,
				CONFIG_SYS_I2C_PCA953X_ADDR,
				0x22, 0x00);
	}
}

/** Periodically poll the Ethernet ports for a module change */
void octeon_board_poll(void)
{
	static struct eth_device *dev = NULL;

	if (!dev)
		dev = eth_get_dev_by_index(0);

	if (dev) {
		octeon_phy_port_check(dev);
		dev = dev->next;
	}
}

int early_board_init(void)
{
	/* configure clk_out pin */
	cvmx_mio_fus_pll_t fus_pll;

	fus_pll.u64 = cvmx_read_csr(CVMX_MIO_FUS_PLL);
	fus_pll.cn63xx.c_cout_rst = 1;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* Sel::  0:rclk, 1:pllout 2:psout 3:gnd */
	fus_pll.cn63xx.c_cout_sel = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);
	fus_pll.cn63xx.c_cout_rst = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(SNIC10E_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_SNIC10E, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();


	/* Reset the PHY and set the mode */
	gpio_direction_output(15, 1);
	gpio_direction_output(16, 1);
	gpio_direction_output(17, 0);
	udelay(10);
	gpio_set_value(17, 1);
	return 0;
}

int late_board_init(void)
{
	int qlm;

	for (qlm = 1; qlm <= 2; qlm++)
		octeon_board_vsc8488_qlm_tune(qlm);

	__cvmx_qlm_pcie_cfg_rxd_set_tweak(0, -1);

	return 0;
}
