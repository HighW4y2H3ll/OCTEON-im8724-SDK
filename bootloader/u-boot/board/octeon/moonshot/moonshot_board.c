/*
 * (C) Copyright 2004-2013 Cavium, Inc. <support@cavium.com>
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
#include <i2c.h>
#include <mtd/cfi_flash.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <asm/arch/cvmx-helper.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	union cvmx_mio_qlmx_cfg qlm_mode;
	int i;
	char fdt_key[20];

	for (i = 0; i <= 6; i++) { /* 5 & 6 are ILK, real QLMs 1 & 2 */
		qlm_mode.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(i));
		if (qlm_mode.s.qlm_spd == 0xf) {
			sprintf(fdt_key, "%d,none", i); /* Disabled */
		} else {
			switch (qlm_mode.s.qlm_cfg) {
			case 2:
				sprintf(fdt_key, "%d,sgmii", i);
				break;
			case 3:
				sprintf(fdt_key, "%d,xaui", i);
				break;
			case 7:
				sprintf(fdt_key, "%d,rxaui", i);
				break;
			default:
				sprintf(fdt_key, "%d,none", i);
				break;
			}
			debug("%s: Setting QLM %d to %s\n",
			      __func__, i, fdt_key);
		}
		octeon_fdt_patch(working_fdt, fdt_key, NULL);
	}
	return 0;
}

int checkboard(void)
{
	return 0;
}

int board_early_init_f(void)
{
	static const uint8_t pca_cmd_init_bus[] = {0x01, 0x10};

	/* Reset the PCA9541A1 MUX */
	gpio_direction_output(7, 0);
	udelay(10);
	gpio_set_value(7, 1);
	udelay(10);

	i2c_write(0x70, 1, 1, pca_cmd_init_bus, sizeof(pca_cmd_init_bus));
	mdelay(10);

	return 0;
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

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(MOONSHOT_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_MOONSHOT, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	return 0;
}

static int moonshot_override_iface_phy_mode(int interface, int index)
{
	return 1;
}

void board_net_preinit(void)
{
	cvmx_override_iface_phy_mode = moonshot_override_iface_phy_mode;
}
