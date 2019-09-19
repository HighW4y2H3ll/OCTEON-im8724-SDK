/*
 * (C) Copyright 2004-2011 Cavium, Inc. <support@cavium.com>
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
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

void board_mdio_init(void)
{
	static const char devname[20]="mdio-octeon0\0";
	int i;
	unsigned short val = 0;

	gpio_direction_output(5, 0);	/* GPIO 5 phy #reset */
	mdelay(10);
	gpio_set_value(5, 1);
	mdelay(10);

	for (i = 1; i <= 4; i++) {
		miiphy_read(devname, i, 3, &val);
		if ((val & 0x000f) == 4) {	/* revision C1 *//* Revision C1 workaround */
			miiphy_write(devname, i, 22, 0x00ff);
			miiphy_write(devname, i, 24, 0x1111);
			miiphy_write(devname, i, 23, 0x2012);
		}
	}
	octeon_board_phy_init();
}

int checkboard(void)
{
	gpio_direction_output(0, 1);	/* turn off #run_led */

	return 0;
}

int early_board_init(void)
{
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC_XLE_4G, 3, 0);

	gd->mem_clk = 200;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.clock_desc), ee_buf,
		       sizeof(octeon_eeprom_clock_desc_t));
		gd->mem_clk = gd->arch.clock_desc.ddr_clock_mhz;
	}

	octeon_board_get_mac_addr();

	return 0;
}
