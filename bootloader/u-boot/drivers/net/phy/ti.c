/*
 * TI PHY drivers
 *
 * Copyright 2014 Cavium, Inc.
 * Author: Aaron Williams
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <phy.h>

#ifdef CONFIG_PHYLIB_10G
static int tlk10232_config(struct phy_device *phydev)
{
	int rc;

	debug("%s(0x%x) ENTRY\n", __func__, phydev->addr);

	rc = gen10g_config(phydev);

	phydev->mmds &= ~1;	/* Turn of clause 22 since we don't use it */
	debug("%s: mmds: 0x%x\n", __func__, phydev->mmds);

	return rc;
}

static struct phy_driver tlk10232_driver = {
	.name		= "TI TLK10232",
	.uid		= 0x40005100,
	.mask		= 0xffffffff,
	.features	= 0,
	.config		= tlk10232_config,
	.startup	= gen10g_startup,
	.shutdown	= gen10g_shutdown,
};
#endif

int phy_ti_init(void)
{
#ifdef CONFIG_PHYLIB_10G
	phy_register(&tlk10232_driver);
#endif
	return 0;
}
