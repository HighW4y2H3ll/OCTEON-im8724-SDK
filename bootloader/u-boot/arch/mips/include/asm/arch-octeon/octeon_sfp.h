/***********************license start************************************
 * Copyright (c) 2016 Cavium, Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#ifndef __OCTEON_SFP_H__
#define __OCTEON_SFP_H__

#include <sfp.h>

enum octeon_gpio_type;
struct eth_device;
struct phy_device;

/** GPIO pin information for SFP/QSFP pins */
struct octeon_sfp_gpio_info {
	int			pin;		/** Pin number */
	enum octeon_gpio_type	type;		/** Type of GPIO device */
	int			gpio_node;	/** GPIO node in device tree */
	int			i2c_bus;	/** I2C bus of device */
	int			i2c_addr;	/** I2C address of device */
	/** True if input/output inverted */
	bool			inverted;
	/** Some GPIOs can be connected directly to PHY devices */
	struct phy_device	*phydev;
};

/** Information about 1/10/25/40/50 SFP/SFP+/QSFP+/SFP28/QSFP28 modules */
struct octeon_sfp_info {
	int i2c_bus;		/** I2C bus number for module */
	int i2c_eeprom_addr;	/** EEPROM address (usually 0x50) */
	int i2c_diag_addr;	/** Diagnostics address (usually 0x51) */

	/**
	 * SFP/SFP+/SFP28 optical transmit disable (active high)
	 *
	 * For QSFP+/QSFP28 this is tied to lp_mode.
	 */
	struct octeon_sfp_gpio_info	tx_disable;
	/** mod_abs/mod_prs GPIO pin */
	struct octeon_sfp_gpio_info	mod_abs;
	/** SFP/SFP+/SFP28 transmit error pin */
	struct octeon_sfp_gpio_info	tx_error;
	/** SFP/SFP+/SFP28 receive loss of signal pin */
	struct octeon_sfp_gpio_info	rx_los;
	/** QSFP+/QSFP28 module select for i2c */
	struct octeon_sfp_gpio_info	select;
	/** QSFP+/QSFP28 module reset */
	struct octeon_sfp_gpio_info	reset;
	/** QSFP+/QSFP28 interrupt input */
	struct octeon_sfp_gpio_info	interrupt;
	/** True if module is QSFP+/QSFP28 */
	bool is_qsfp;
	/** True if module requires autonegotiation to be disabled */
	bool disable_an;
	/** True if we successfully parsed SFP info for this slot */
	bool valid;
	/** Function called to return the status of the mod_abs/mod_prs pin */
	int (*check_mod_abs)(struct eth_device *dev, void *data);
	/** User-defined data passed to the check_mod_abs function above */
	void *mod_abs_data;
	/** Function called whenever mod_abs has changed state */
	int (*mod_abs_changed)(struct eth_device *dev, bool present, void *data);
	/** User-defined data passed to mod_abs_changed function */
	void *mod_abs_changed_data;

	/** Last mod_abs value to check for change */
	bool last_mod_abs;

	/** SFP/SFP+/SFP28/QSFP+/QSFP28 module info decoded from EEPROM */
	struct phy_sfp_info		sfp_info;
};

/**
 * Parses the device tree for all data relavent to a
 * SFP/SFP+/SFP28/QSFP+/QSFP28 slot.
 *
 * @param	eth_node	Node in device tree of Ethernet port or
 *				interface
 * @param[out]	sfp_info	SFP slot information read from device tree
 *
 * @return	0 for success (even if no data found) or -1 on error
 *
 * NOTE: This does not actually read the EEPROM, only the address of it and
 * the GPIO pins.  It tries to guess if it's a QSFP or not based on what GPIO
 * pins are found.
 */
int octeon_sfp_get_info(int eth_node, struct octeon_sfp_info *sfp_info);

/**
 * Registers a function to be called whenever mod_abs changes state
 *
 * @param	dev		Ethernet device
 * @param[in]	mod_abs_changed	Function to be called
 * @param[in]	data		data to be passed to user-defined function
 *
 * @return	0 for success
 */
int octeon_sfp_register_mod_abs_changed(struct eth_device *dev,
		int (*mod_abs_changed)(struct eth_device *dev, bool present,
				       void *data), void *data);

/**
 * Registers a function to be called to report the status of the SFP+ MOD_ABS
 * signal.
 *
 * @param	dev		Ethernet device to register this for
 * @param	check_mod_abs	Function to be called, should return 1 if
 *				module is absent.
 * @param	data		Pointer to user-defined data to pass to
 *				check_mod_abs()
 *
 * NOTE: If check_mod_abs is NULL and the mod_abs GPIO data is configured then
 * the default function will be used.
 */
void octeon_sfp_register_mod_abs_func(struct eth_device *dev,
				      int (*check_mod_abs)(struct eth_device *dev, void *data),
				      void *data);

/**
 * Register the GPIO pin used to read the mod_abs pin for some SFP+ ports
 *
 * @param	dev		Ethernet device
 * @param	type		Type of GPIO device
 * @param	i2c_bus		Optional i2c bus used for GPIO pin
 * @param	i2c_addr	i2c address of GPIO device if not native
 * @param	pin		GPIO pin number of device, set to -1 for no
 *				GPIO device.
 * @param	inverted	Set true if mod_abs is high when device present
 */
void octeon_sfp_register_mod_abs(struct eth_device *dev,
				 enum octeon_gpio_type type,
				 int i2c_bus, int i2c_addr, int pin,
				 bool inverted);

/**
 * Registers a function to be called to report the status of the SFP+ MOD_ABS
 * signal.
 *
 * @param	dev		Ethernet device to register this for
 * @param	check_mod_abs	Function to be called, should return 1 if
 *				module is absent.
 * @param	data		Pointer to user-defined data to pass to
 *				check_mod_abs()
 */
void octeon_sfp_register_mod_abs_func(struct eth_device *dev,
		      int (*check_mod_abs)(struct eth_device *dev, void *data),
		      void *data);

/**
 * Gets information about the GPIO device connected to the mod_abs pin of a
 * SFP+ port.
 *
 * @param[in]	dev		Ethernet device
 * @param[out]	type		Type of GPIO device, can be NULL
 * @param[out]	i2c_bus		Optional i2c bus used for GPIO pin, can be NULL
 * @param[out]	i2c_addr	i2c address of GPIO device if not native,
 *				can be NULL.
 * @param[out]	pin		GPIO pin number of device, can be NULL
 * @param[out]	inverted	Set true if mod_abs is high when device present,
 *				can be NULL
 */
void octeon_sfp_get_mod_abs(const struct eth_device *dev,
			    enum octeon_gpio_type *type, int *i2c_bus,
			    int *i2c_addr, int *pin, bool *inverted);

/**
 * Default function used to check the status of the mod_abs pin for SFP+ slots
 *
 * @param	dev	Ethernet device
 * @param	data	User-defined data (not used)
 *
 * @return	0 if module present, 1 if module absent, -1 on error
 */
int octeon_sfp_check_mod_abs(struct eth_device *dev, void *data);

/**
 * Check if the SFP module supports autonegotiation being enabled.
 *
 * @param	dev	Ethernet device of port
 *
 * @return	true if autonegotiation is allowed, false if it is not allowed
 *
 * With some modules such as the Avago/Foxconn gigabit copper modules
 * autonegotiation must be disabled.
 *
 * The autonegotiation can also be disabled by setting the environment variable
 * devname_disable_an where devname is the Ethernet interface name such as
 * octeth2.
 */
bool octeon_check_sfp_autonegotiation(struct eth_device *dev);

/**
 * @brief Get the i2c address and bus of the SFP EEPROM and ASIC for a given Ethernet
 *        port.
 *
 * @param[in,out]	dev		Ethernet device
 * @param		eth_node	Node of Ethernet port in device tree
 *
 * @return	0 for success
 */
int octeon_sfp_get_address(struct eth_device *dev, int eth_node);

/**
 * Output the GPIO info for a SFP/QSFP pin
 *
 * @param	eth_node	Node of Ethernet interface (QSFP) or port (SFP)
 * @param[out]	gpio_info	Information about the GPIO pin
 * @param[in]	prop_name	Name of GPIO pin property
 *
 * @return	0 if success or if GPIO pin data is missing, -1 on error
 *
 * NOTE: In all cases the gpio_info data structure will be initialized.  If
 * no entry is found then the type is set to GPIO_TYPE_UNKNOWN and the pin
 * is set to -1.
 */
int octeon_sfp_get_gpio_info(int eth_node,
			     struct octeon_sfp_gpio_info *gpio_info,
			     const char *prop_name);

/**
 * Reads the EEPROM data structure from a SFP/QSFP device into the sfp data
 * structure.
 *
 * @param	dev	Ethernet device to read
 *
 * @return	0 for success, -1 on error
 *
 * NOTE: This function also checks mod_abs/mod_prs and activates select for
 * QSFP modules.
 */
int octeon_sfp_read_eeprom(struct eth_device *dev);

/**
 * Returns a pointer to the (Q)SFP EEPROM info.  Note that it  may not be valid.
 *
 * @param[in]	dev	Pointer to Ethernet device
 *
 * @return	Pointer to SFP EEPROM data or NULL if no SFP slot exists.
 */
struct phy_sfp_info *octeon_sfp_get_eeprom_info(const struct eth_device *dev);

/**
 * Sets an output SFP/QSFP GPIO pin
 *
 * @param[in]	gpio	data structure describing the GPIO pin
 * @param	set	Set to true to set the pin, false to clear.  Note that
 *			the outpput may be inverted automatically.
 * @return	0 for success, -1 on error.
 */
int octeon_sfp_set_gpio_pin(struct octeon_sfp_gpio_info *gpio, bool set);

#endif /* __OCTEON_SFP_H__ */
