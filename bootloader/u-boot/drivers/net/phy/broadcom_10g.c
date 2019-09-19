/*
 * Broadcom BCM8727 10G dual PHY driver
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
 *
 *
 * Copyright 2012 Cavium Networks, Inc. <support@cavium.com>
 * author: Aaron Williams
 *
 * The Broadcom BCM8727 dual 10G PHY requires a special reset sequence if
 * the PMD adaptive equalizer SPI ROM is installed, otherwise the generic 10G
 * phy can be used.
 *
 * If the normal reset is used the PHY will not work.
 */

#include <config.h>
#include <common.h>
#include <miiphy.h>
#include <phy.h>

int bcm8727_reset(struct phy_device *phydev)
{
	int reg;
	int timeout = 500;
	int devad = MDIO_DEVAD_NONE;

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
	debug("Resetting phy at %d %d\n", phydev->addr, devad);
	/* Issue soft reset */
	if (phy_write(phydev, devad, MII_BMCR, reg) < 0)
		return -1;
	/* General control status register global reset */
	if (phy_write(phydev, devad, 0xca10, 1) < 0)
		return -1;
	/* 78 MHz uC clock
	 * Force 8051 SPI port reboot at next reset,
	 * Soft reset the microcontroller
	 */
	if (phy_write(phydev, devad, 0xca10, 0x8c) < 0)
		return -1;
	/* Enable microcode upload from external SPI-ROM */
	if (phy_write(phydev, devad, 0xca85, 1) < 0)
		return -1;
	/* refclk derived from REFCLK input
	 * 78MHz
	 * Force an 8051 SPI port reboot at next reset
	 * Soft reset of internal logic (register values retained)
	 */
	if (phy_write(phydev, devad, 0xca10, 0x018a) < 0)
		return -1;
	/* Take internal logic out of reset */
	if (phy_write(phydev, devad, 0xca10, 0x0188) < 0)
		return -1;
	/* Give time to load firmware, spec says 100ms */
	mdelay(110);
	/* Disable serial boot and put SPI eeprom in tri-state mode */
	if (phy_write(phydev, devad, 0xca85, 0) < 0)
		return -1;

#ifdef CONFIG_PHY_RESET_DELAY
	udelay(CONFIG_PHY_RESET_DELAY);	/* Intel LXT971A needs this */
#endif
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
		udelay(1000);
	}

	if (reg & BMCR_RESET) {
		puts("PHY reset timed out\n");
		return -1;
	}

	return 0;
}

struct phy_driver bcm8727_driver = {
	.uid		= 0x00206036,
	.mask		= 0xffffffff,
	.name		= "Broadcom BCM8727 10G PHY",
	.features	= 0,
	.config		= gen10g_config,
	.startup	= gen10g_startup,
	.shutdown	= gen10g_shutdown,
	.reset		= bcm8727_reset
};
