/*
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
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

#ifndef __OCTEON_BOARD_PHY_H__
#define __OCTEON_BOARD_PHY_H__

#include <asm/arch/octeon_eth.h>

/**
 * Initializes all of the PHYs on a board using the device tree information.
 *
 * @return 0 for success, error otherwise
 */
int octeon_board_phy_init(void);

/**
 * Weak function called before configuring a PHY device
 *
 * @param	dev		Ethernet device PHY is attached to
 * @param	fdt_blob	FDT pointer
 * @param	fdt_offset	Node offset for phy in device tree
 *
 * @return	0 to continue, otherwise skip initializing the phy
 */
int octeon_board_pre_phy_init(struct eth_device *dev,
			      const void *fdt_blob, int fdt_offset);

/**
 * Weak function called after configuring a PHY device
 *
 * @param	dev		Ethernet device phy is attached to
 * @param	fdt_blob	FDT pointer
 * @param	fdt_offset	Node offset for phy in device tree
 *
 * @return	0 for success, -1 for error
 */
int octeon_board_post_phy_init(struct eth_device *dev,
			       const void *fdt_blob, int fdt_offset);


# ifdef CONFIG_PHY_VITESSE
/**
 * This function is called before and after all of the PHY initialization
 * to control the Vitesse COMA mode.
 */
int octeon_vitesse_coma(bool start);

/**
 * Initializes a Vitesse VSC8490 PHY device
 *
 * @param ethdev	Ethernet device pointer
 *
 * @return 0 for success, -1 for error.
 */
int octeon_fdt_vitesse_vsc8490_config(const struct eth_device *ethdev);

/**
 * Configures the Vitesse vsc8488 for use on boards like the SNIC10xxx based
 * boards using the values in the flat device tree.
 *
 * @param ethdev - ethernet device to initialize PHY for
 *
 * @returns 0 for success, error otherwise
 */
int octeon_fdt_vitesse_config(const struct eth_device *ethdev);

/**
 * This function configures the Vitesse PHY based on information read from the
 * SFP+ module SFF EEPROM
 *
 * @param ethdev - ethernet device to configure
 *
 * @returns 0 for success, error otherwise
 */
int octeon_vitesse_vsc8488_sfp_config(struct eth_device *ethdev);

/**
 * This function configures the Vitesse PHY based on information read from the
 * SFP+ module SFF EEPROM
 *
 * @param ethdev - ethernet device to configure
 *
 * @returns 0 for success, error otherwise
 */
int octeon_vitesse_sfp_config(struct eth_device *ethdev);

/**
 * This function tunes the QLM for use by the Vitesse vsc8488 PHY.
 *
 * @param qlm - qlm number to tune
 */
void octeon_board_vsc8488_qlm_tune(int qlm);

#  ifdef CONFIG_OCTEON_VITESSE_VSC7224
/**
 * Initializes a Vitesse VSC7224 reclocking chip
 *
 * @param[in] fdt	Pointer to flat device tree
 *
 * @return 0 for success, -1 on error
 */
int octeon_vitesse_init_vsc7224(const void *fdt);
#  endif
# endif
#endif /* __OCTEON_BOARD_PHY__ */
