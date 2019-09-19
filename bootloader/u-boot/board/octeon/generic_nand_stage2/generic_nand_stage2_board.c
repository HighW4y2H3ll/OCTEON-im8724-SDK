/*
 * (C) Copyright 2004-2011
 * Cavium Inc.
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
#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <pci.h>
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>

DECLARE_GLOBAL_DATA_PTR;


int board_early_init_f(void)
{
	/* 63XX TODO: Signal to MCU that we are running */
	twsii_mcu_read(2);
	twsii_mcu_read(3);

	return 0;
}

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	union cvmx_gmxx_inf_mode gmx_mode;

	gmx_mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));
	if (gmx_mode.cn63xx.speed == 0xf) {
		fdt_key = "0,none";	/* Disabled */
	} else if (gmx_mode.cn63xx.mode) {
		fdt_key = "0,xaui";
	} else {
		fdt_key = "0,sgmii";
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	return 0;
}

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
/* Configure the PHYS for the proper LED mode */
void marvellphy_led_mode(void)
{
	int i;
	unsigned short phy_identifier;
	static const char devname[20] = "mdio-octeon0\0";

	miiphy_read(devname, 0x81, 2, &phy_identifier);

	if (phy_identifier != 0x0141)	/* Is it a Marvell PHY? */
		return;

	for (i = 0x81; i <= 0x84; i++) {
		miiphy_write(devname, i, 0x16, 3);	/* set register page */
		/* Write LED modes */
		miiphy_write(devname, i, 0x10, 0x5777);
		miiphy_write(devname, i, 0x13, 0x0070);
		miiphy_write(devname, i, 0x16, 0);	/* set register page */
	}
}

void board_mdio_init(void)
{
	marvellphy_led_mode();
}
#endif

int checkboard(void)
{
	/* Enable MDIO */
	cvmx_write_csr(CVMX_SMIX_EN(0), 1);
	cvmx_write_csr(CVMX_SMIX_EN(1), 1);

	return 0;
}

int early_board_init(void)
{
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

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
	}

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_DDR_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		octeon_eeprom_ddr_clock_desc_t *ddr_clock_ptr = (void *)ee_buf;
		gd->mem_clk = ddr_clock_ptr->ddr_clock_hz / 1000000;
	} else {
		gd->mem_clk = 533;
		gd->flags |= GD_FLG_CLOCK_DESC_MISSING;
	}

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	/* Determine board type/rev */
	strncpy((char *)(gd->arch.board_desc.serial_str), "unknown", SERIAL_LEN);
	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_BOARD_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.board_desc), ee_buf,
		       sizeof(octeon_eeprom_board_desc_t));
	} else {
		gd->flags |= GD_FLG_BOARD_DESC_MISSING;
		gd->arch.board_desc.rev_minor = 0;
		gd->board_type = gd->arch.board_desc.board_type
						= CVMX_BOARD_TYPE_EBB6300;
		/* Try to determine board rev by looking at PAL */
		gd->arch.board_desc.rev_major = 1;
	}

	if (gd->mem_clk < 100 || gd->mem_clk > 2000)
		gd->mem_clk = 533;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_MAC_ADDR_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.mac_desc), ee_buf,
		       sizeof(octeon_eeprom_mac_addr_t));
	} else {
		/* Make up some MAC addresses */
		gd->arch.mac_desc.count = 255;
		gd->arch.mac_desc.mac_addr_base[0] = 0x00;
		gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
		gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
		gd->arch.mac_desc.mac_addr_base[3] =
		    (gd->arch.board_desc.rev_major << 4) | gd->arch.board_desc.
		    rev_minor;
		gd->arch.mac_desc.mac_addr_base[4] =
		    gd->arch.board_desc.serial_str[0];
		gd->arch.mac_desc.mac_addr_base[5] = 0x00;
	}

	octeon_led_str_write("Booting.");

	return 0;
}
