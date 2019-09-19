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
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-board.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

void board_net_preinit(void)
{
	mdelay(120);
}

int checkboard(void)
{
	int clk_to_use = 1;	/* Clock used for DLM0 */
	octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_QSGMII_QSGMII,
			     0, 0, clk_to_use, 0);
	return 0;
}

int early_board_init(void)
{
	/* Populate global data from eeprom */
	octeon_board_get_clock_info(CUST_CLARK_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CUST_CLARK, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();
	return 0;
}
