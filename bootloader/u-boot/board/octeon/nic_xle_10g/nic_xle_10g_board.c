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
#include <asm/arch/octeon_eth.h>
#include <asm/arch/cvmx-mdio.h>
#include <asm/gpio.h>
#include <net.h>
#include <phy.h>

DECLARE_GLOBAL_DATA_PTR;

void board_mdio_init(void)
{
	static const char devname[20] = "mdio-octeon0\0";
	char ethname[16];
	uint64_t clk_mod;
	int val;
	int i;

	gpio_direction_output(5, 0);	/* PHY reset active low */
	mdelay(10);
	gpio_set_value(5, 1);
	mdelay(10);

	/* PCSX0 lane swap */
	cvmx_write_csr(CVMX_PCSXX_MISC_CTL_REG(0), 0xc);

	/* Minimum clock period */
	val = (310 * gd->cpu_clk / 1000) & 0xff;
	clk_mod = 0x1001000;	/* set Clause 45 mode */
	clk_mod = clk_mod | val | (val & 0xf) << 8 | 1 << 15
		  | (val >> 4) << 16 | 1 << 24;
	cvmx_write_csr(CVMX_SMIX_CLK(0), clk_mod);

	for (i = 0; i < 2; i++) {
		val = cvmx_mdio_45_read(0, i, 1, 0xe800);
		if (val == 0x8486) {
			val = cvmx_mdio_45_read(0, i, 1, 0xe607);
			/* Check module present */
			printf("Ethernet %d SFP+ module is %spresent\n", i,
			       (val & 0x4000) ? "not " : "");
			/* TXDOUT invert polarity */
			val = cvmx_mdio_45_read(0, i, 1, 0x8000);
			val = val & ~(1 << 7);
			cvmx_mdio_45_write(0, i, 1, 0x8000, val);

			/* DEV_GPIO_CTRL tx/rx led, wis_intb */
			cvmx_mdio_45_write(0, i, 1, 0xe901, 0x283a);
		} else {
			printf("Phy%d does not exist!!! \n", i);
		}
	}
}

int checkboard(void)
{
	gpio_direction_output(0, 1);	/* Turn off run_led */

	gpio_direction_output(15, 0);	/* LP_XGMX = low */

	return 0;
}

int early_board_init(void)
{
	/* Populate global data from eeprom */
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC_XLE_10G, 3, 0);

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

	/* FIXME. This seems to be needed for XAUI to work - not sure why..... */
	{			/* Write Mac address for host use */
		uint8_t *ptr = (uint8_t *) gd->arch.mac_desc.mac_addr_base;
		uint64_t mac = 0;
		int i;
		for (i = 0; i < 6; i++)
			mac = (mac << 8) | (uint64_t) (ptr[i]);
		cvmx_write_csr(CVMX_GMXX_SMACX(0, 0), mac);
	}

	return 0;

}
