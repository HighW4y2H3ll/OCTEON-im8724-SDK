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
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
	uint8_t val;
	int rc;

	/* Enable USB power */
	gpio_direction_output(18, 0);
	gpio_direction_output(19, 0);

	gpio_direction_input(14);	/* SD/MMC Write Protect */
	gpio_direction_input(15);	/* SD/MMC Card Detect */
	mdelay(10);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EVB7000_SFF_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EVB7000_SFF, 1, 0);

	/* Need to enable the PWM of the fan controller */
	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);
	rc = i2c_read(CONFIG_SYS_I2C_DTT_ADDR, 0, 1, &val, 1);
	if (rc) {
		puts("ERROR: Cannot access temperature sensor!!!\n");
		return -1;
	}
	val |= 1;
	rc = i2c_write(CONFIG_SYS_I2C_DTT_ADDR, 0, 1, &val, 1);
	if (rc) {
		puts("ERROR: Cannot access temperature sensor!!!\n");
		return -1;
	}
	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	return 0;
}

void late_board_init(void)
{
	char buf[256];
	char *board_name;
	board_name = getenv("boardname");
	if (board_name) {
		snprintf(buf, sizeof(buf), "u-boot-octeon_%s.bin", board_name);
		setenv("octeon_stage3_bootloader", buf);
	}
}
