/*
 * Copyright 2011 Freescale Semiconductor, Inc.
 *	Andy Fleming <afleming@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * This file pretty much stolen from Linux's mii.h/ethtool.h/phy.h
 */

#ifndef _CORTINA_H
#define _CORTINA_H

#include <linux/list.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/mdio.h>
#include <phy.h>

#define CORTINA_MAX_SLICES		16

struct phy_sfp_info;

/** Mode that the slice is running in */
enum cortina_slice_mode {
	CORTINA_SLICE_SR,	/** Optical */
	CORTINA_SLICE_CX,	/** Passive copper */
	CORTINA_SLICE_1000X,	/** 1000Base-X */
	CORTINA_SLICE_UNKNOWN,	/** Unknown/default mode */
};

/** Hardware ID of ASIC */
enum cortina_hw_id {
	/** Undefined */
	CORTINA_CS4224_HW_UNDEF = 0,
	/** 4 port duplex */
	CORTINA_CS4224_HW_CS4223 = 0x0010,
	/** 16 port simplex */
	CORTINA_CS4224_HW_CS4224 = 0x0011,
	/** 8 port duplex */
	CORTINA_CS4224_HW_CS4343 = 0x0012,
	/** 10 port simplex */
	CORTINA_CS4224_HW_CS4221 = 0x0013,
	/** 2 port duplex */
	CORTINA_CS4224_HW_CS4227 = 0x0014,
	/** 16 port simplex, 15G only */
	CORTINA_CS4224_HW_CS4210 = 0x0015,
	/** 8 port duplex, 10G only */
	CORTINA_CS4224_HW_CS4341 = 0x0016
};

/** Private data for Cortina PHYs */
struct cortina_phy_info {
	/**
	 * Since the MDIO arrays of PHY devices won't work when there are
	 * sub-addresses we use a linked list.
	 */
	struct list_head list;

	/**
	 * PHY device we belong to (for list processing)
	 */
	struct phy_device *phydev;

	/**
	 * Hardware ID of ASIC
	 */
	enum cortina_hw_id hw_id;
	/**
	 * Some Cortina PHYs such as the CS4223 require sub addresses for
	 * each slice when running in XFI mode.  In this case, each Ethernet
	 * interface talks to the same PHY address on the MDIO bus but uses
	 * different register offsets.  This is true when there are multiple
	 * sub-addresses used.  In XLAUI mode sub addresses are not used.
	 */
	bool sub_addr_used;

	/**
	 * Sub-address of this interface, -1 if not used.
	 */
	uint8_t	sub_addr;

	/**
	 * Maximum number of slices
	 */
	int max_slices;

	/**
	 * Slice register offset for this sub-address
	 */
	int slice_offsets[CORTINA_MAX_SLICES];
	/**
	 * The SFP/SFP+ module EEPROM is connected to this i2c bus number
	 */
	int sfp_i2c_bus;

	/**
	 * Address of SFP+ eeprom on i2c bus (usually 0x50/0xA0)
	 */
	int sfp_i2c_addr;

#ifdef CONFIG_OF_LIBFDT
	/**
	 * SFP+ EEPROM node in device tree
	 */
	int fdt_sfp_eeprom_node;

	/**
	 * Flat device-tree node for this instance/slice
	 * Note that usually only entry [sub_addr] is used except in quad_mode
	 * where each slice is indexed and the nexus_node is also used.
	 */
	int fdt_nodes[CORTINA_MAX_SLICES];

	/**
	 * For multiple slices, record the parent node
	 */
	int nexus_node;
#endif

	/**
	 * Mode this port is running in
	 */
	enum cortina_slice_mode	mode;

	/**
	 * PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA indexed by mode
	 */
	uint16_t tx_output_ctrla[CORTINA_MAX_SLICES][CORTINA_SLICE_UNKNOWN];

	/**
	 * PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB indexed by mode
	 */
	uint16_t tx_output_ctrlb[CORTINA_MAX_SLICES][CORTINA_SLICE_UNKNOWN];

	/**
	 * Private data for use outside this driver
	 */
	void *priv;

	/**
	 * Set to true when groups of four slices are used together, for example
	 * for XLAUI mode or for QSFPs.  Note that if XLAUI is spread across
	 * four cables then only the settins of the first cable will be used
	 * for the configuration.
	 */
	bool quad_mode;
};

int cortina_phy_init(void);

/**
 * Connect a new PHY to an Ethernet device
 *
 * @param bus		MDIO bus
 * @param addr		MDIO address of PHY
 * @param sub_addr	Sub-address used for some Cortina PHYs (i.e. cs4223 XFI)
 * @param dev		Ethernet device
 * @param interface	PHY interface type
 *
 * @return pointer to new PHY device or NULL if there is an error
 */
struct phy_device *cortina_phy_connect(struct mii_dev *bus,
				       int addr, char sub_addr,
				       struct eth_device *dev,
				       phy_interface_t interface);
int phy_startup(struct phy_device *phydev);
int phy_config(struct phy_device *phydev);
int phy_shutdown(struct phy_device *phydev);
int cortina_phy_register(struct phy_driver *drv);
#ifdef CONFIG_OF_LIBFDT
int cortina_phy_config_fdt(struct phy_device *phydev, void *fdt_blob, int node);
#endif

/**
 * Changes the mode of a CS4224 Cortina PHY between CX, SR and 1000Base-X modes
 *
 * @param	phydev	phydev to modify
 * @param	mode	New mode for phy
 *
 * @return	0 for success
 */
int cs4224_set_mode(struct phy_device *phydev, enum cortina_slice_mode mode);

/**
 * Checks which SFP module is plugged in and configures the PHY to the proper
 * mode.
 *
 * @param	phydev	phy device to check
 *
 * @return	0 if mode set properly, -1 if sfp not read or if parsing error
 */
int cs4224_port_check(struct phy_device *phydev);
/**
 * Uses the sfp info data to return the proper mode
 *
 * @param[in]	sfp_info	Information on the SFP module plugged in
 *
 * @return	Mode the Cortina PHY should be configured for.  If unknown
 *		it defaults to CX mode.
 */
enum cortina_slice_mode
cs4224_get_mode_from_sfp(const struct phy_sfp_info *sfp_info);

#ifdef CONFIG_PHY_SFP
/**
 * Register the i2c bus and address for a SFP module
 *
 * @param	phydev		PHY device to register SFP with
 * @param	i2c_bus		i2c bus number
 * @param	i2c_addr	i2c address (usually 0x50)
 *
 * @return	0 for success.
 */
int cs4224_register_sfp(struct phy_device *phydev, int i2c_bus, int i2c_addr);
#endif

/**
 * Obtains the ASIC SKU hardware ID
 *
 * @param phydev	phy device to get the SKU from
 *
 * @return	The ASIC SKU number
 */
enum cortina_hw_id cs4224_get_hw_id(struct phy_device *phydev);

/**
 * Weak function called whenever the SFP mode is set.
 *
 * @param	phydev		phy device for SFP port
 * @param	old_mode	old mode switching from
 * @param	new_mode	new mode just switched to
 *
 * @return	0 for success, -1 for error
 */
int board_cs4224_set_mode(struct phy_device *phydev,
			  enum cortina_slice_mode old_mode,
			  enum cortina_slice_mode new_mode);

#endif
