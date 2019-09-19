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
#include <i2c.h>
#include <asm/gpio.h>
#include <miiphy.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_board_phy.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	uchar temp;
	uint8_t val;
	/* Note that the SA56004 will be programmed again later with DTT */
	/* Internal SA56004 temp, approximates ambient/board temp. */
	temp = 80;
	i2c_write(CONFIG_SYS_I2C_DTT_ADDR, 0x20, 1, &temp, 1);
	/* Internal Octeon temp, approximates junction temp. */
	temp = 100;
	i2c_write(CONFIG_SYS_I2C_DTT_ADDR, 0x19, 1, &temp, 1);
	/* Enable the fault queue, requiring three consecutive measurements
	 * before triggering T_CRIT.
	 */
	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 0x03);
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x9, val | 1);

	/* Enable USB supply */
	if (gd->arch.board_desc.rev_major < 3)
		gpio_direction_output(6, 1);

	return 0;
}

int board_fixup_fdt(void)
{
	const char *fdt_key;

	switch (gd->arch.board_desc.rev_major) {
	default:
		fdt_key = "0,revision3";
		break;
	case 2:
	case 1:
		fdt_key = "0,revision1";
		break;
	}
	octeon_fdt_patch(working_fdt, fdt_key, "cavium,qlm-trim");

	return 0;
}

#define BCM8727_GEN_CTRL_STAT	0xCA10
#define BCM8727_MISC_CTRL2	0xCA85
/**
 * Resets the PHY on the NIC10e 66
 *
 * Revision 2 of the NIC10e board requires a special reset sequence for
 * the Broadcom BCM8727 PHY device if the SPI-ROM is installed.  A normal
 * reset sequence will prevent the PHY device from working properly.
 *
 * Revision 1 of the NIC10e board does not have the SPI-ROM and revision 3
 * uses a Vitesse device.  Both the revision 1 and revision 3 boards can
 * use the standard PHY reset code.
 *
 * @param phydev - pointer to phy device data structure
 *
 * @return 0 for success, -1 on error.
 */
static int nic10e_phy_reset(struct phy_device *phydev)
{
	int reg;
	int timeout = 500;
	int devad = MDIO_DEVAD_NONE;

	debug("In %s\n", __func__);
	/* If it's 10G, we need to issue reset through one of the MMDs */
	if (!is_10g_interface(phydev->interface)) {
		printf("%s: Error: phy not 10G interface\n", __func__);
		return -1;
	}

	if (!phydev->mmds)
		gen10g_discover_mmds(phydev);

	devad = ffs(phydev->mmds);

	reg = phy_read(phydev, devad, MII_BMCR);
	if (reg < 0) {
		debug("PHY status read failed\n");
		return -1;
	}
	/* The below sequence comes from the BCM8727 data sheet when the
	 * PMD Adaptive Equalizer is to be configured.
	 *
	 */
	reg |= BMCR_RESET;

	/* The BCM8727 PHY does not recover after a reset command if the SPI-ROM
	 * is installed.
	 */
	if (gd->arch.board_desc.rev_major == 2) {
		debug("Not resetting PHY due to PHY brokeness\n");
		return 0;
		/* For the BCM8727 with the SPI-ROM turn on the SPI interface */
		if (phy_write(phydev, devad, BCM8727_MISC_CTRL2, 1) < 0)
			return -1;
	}

	debug("Resetting phy at %d %d\n", phydev->addr, devad);
	/* Issue soft reset */
	if (phy_write(phydev, devad, MII_BMCR, reg) < 0)
		return -1;
	if (gd->arch.board_desc.rev_major == 2) {
		/* General control status register global reset */
		if (phy_write(phydev, devad, BCM8727_GEN_CTRL_STAT, 1) < 0)
			return -1;
		/* 78 MHz uC clock
		 * Force 8051 SPI port reboot at next reset,
		 * Soft reset the microcontroller
		 */
		if (phy_write(phydev, devad, BCM8727_GEN_CTRL_STAT, 0x18c) < 0)
			return -1;
		/* Enable microcode upload from external SPI-ROM */
		if (phy_write(phydev, devad, BCM8727_MISC_CTRL2, 1) < 0)
			return -1;
		/* refclk derived from REFCLK input
		 * 78MHz
		 * Force an 8051 SPI port reboot at next reset
		 * Soft reset of internal logic (register values retained)
		 */
		if (phy_write(phydev, devad, BCM8727_GEN_CTRL_STAT, 0x018a) < 0)
			return -1;
		/* Take internal logic out of reset */
		if (phy_write(phydev, devad, BCM8727_GEN_CTRL_STAT, 0x0188) < 0)
			return -1;
		/* Give time to load firmware, spec says 100ms */
		mdelay(100);
		/* Disable serial boot and put SPI eeprom in tri-state mode */
		if (phy_write(phydev, devad, BCM8727_MISC_CTRL2, 0) < 0)
			return -1;
	}

	/*
	 * Poll the control register for the reset bit to go to 0 (it is
	 * auto-clearing).  This should happen within 0.5 seconds per the
	 * IEEE spec.
	 */
	while ((reg & BMCR_RESET) && timeout--) {
		reg = phy_read(phydev, devad, MII_BMCR);

		if (reg < 0) {
			debug("PHY status read failed\n");
			return -1;
		}
		mdelay(1);
	}

	if (reg & BMCR_RESET) {
		puts("PHY reset timed out\n");
		return -1;
	}

	return 0;
}

int gen10g_config(struct phy_device *phydev);
int gen10g_startup(struct phy_device *phydev);
int gen10g_shutdown(struct phy_device *phydev);

struct phy_driver nic10e_phy_driver = {
	.uid		= 0x0143bff0,
	.mask		= 0xffffffff,
	.name		= "Broadcom BCM8727 10G PHY",
	.features	= 0,
	.config		= gen10g_config,
	.startup	= gen10g_startup,
	.shutdown	= gen10g_shutdown,
	.reset		= nic10e_phy_reset,
};

int board_phy_init(void)
{
	if (gd->arch.board_desc.rev_major < 3)
		return phy_register(&nic10e_phy_driver);
	else
		return 0;
}

void board_mdio_init(void)
{
	octeon_board_phy_init();
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

	/* Enable PSRAM (take out of low power mode) */
	gpio_direction_output(0, 1);

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(NIC10E_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC10E_66, 3, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	if (gd->arch.board_desc.rev_major >= 3)
		/* Take PHY out of reset.  Do this early to give it time to come up. */
		gpio_direction_output(17, 1);

	return 0;
}

int late_board_init(void)
{
	int qlm;
	for (qlm = 1; qlm <= 2; qlm++)
		octeon_board_vsc8488_qlm_tune(qlm);

	return 0;
}
