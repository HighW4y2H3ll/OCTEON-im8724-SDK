/*
 * Aquantia PHY drivers
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Copyright 2014-2015 Cavium, Inc. <support@cavium.com>
 */

#include <common.h>
#include <phy.h>
#include <miiphy.h>

/* Aquantia AQR105 */


static int aqr105_parse_status(struct phy_device *phydev)
{
/*
	int mii_reg;

	mii_reg = phy_read(phydev, MDIO_DEVAD_NONE, MIIM_DP83865_LANR);

	switch (mii_reg & MIIM_DP83865_SPD_MASK) {

	case MIIM_DP83865_SPD_1000:
		phydev->speed = SPEED_1000;
		break;

	case MIIM_DP83865_SPD_100:
		phydev->speed = SPEED_100;
		break;

	default:
		phydev->speed = SPEED_10;
		break;

	}

	if (mii_reg & MIIM_DP83865_DPX_FULL)
		phydev->duplex = DUPLEX_FULL;
	else
		phydev->duplex = DUPLEX_HALF;
*/
	return 0;
}


static int aqr105_config(struct phy_device *phydev)
{
	return 0;
}

static int aqr105_startup(struct phy_device *phydev)
{
	printf("ENTER aqr105_starup(struct phy_device *phydev=%p)...\n", phydev);
	genphy_update_link(phydev);
	aqr105_parse_status(phydev);

	return 0;
}

static int aqr105_shutdown(struct phy_device *phydev)
{
	return 0;
}

static int aqr105_reset(struct phy_device *phydev)
{
	int reg;
	int timeout = 500;
	int devad;

	if (!phydev->mmds)
		gen10g_discover_mmds(phydev);
	devad = ffs(phydev->mmds) - 1;

	reg = phy_read(phydev, devad, MII_BMCR);

	debug("%s: read device 0x%d BMCR value 0x%x\n", __func__, devad, reg);
	/* TODO */
	return 0;
}

static struct phy_driver AQR105_driver = {
	.name = "Aquantia AQR105",
	.uid = 0x03a1b4a3,
	.mask = 0xfffffff0,
	.features = PHY_10G_FEATURES,
	.config = &aqr105_config,
	.startup = &aqr105_startup,
	.shutdown = &aqr105_shutdown,
	.reset = &aqr105_reset,
};

int aquantia_phy_init(void)
{
	phy_register(&AQR105_driver);

	return 0;
}
