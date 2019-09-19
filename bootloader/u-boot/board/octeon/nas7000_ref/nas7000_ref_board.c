/*
 * (C) Copyright 2013 Cavium, Inc. <support@cavium.com>
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
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	/* Take PHYs out of reset */
	gpio_direction_output(9, 1);

	/* Configure QLMs */
	octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_SGMII_SGMII, 0, 0, 1, 1);
	octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1, 1, 2, 1, 1);
	octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1, 0, 0, 1, 0);
	return 0;
}

int early_board_init(void)
{
	uint8_t val;

	/* Disable USB power */
	gpio_direction_output(1, 0);
	gpio_direction_output(2, 0);

	/* Put the PHYs in reset */
	gpio_direction_output(9, 0);

	mdelay(10);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(NAS7000_REF_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NAS7000_REF,
				    CONFIG_OCTEON_NAS7000_REF_MAJOR,
				    CONFIG_OCTEON_NAS7000_REF_MINOR);


	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	return 0;
}
