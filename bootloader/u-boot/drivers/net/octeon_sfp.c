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

/**
 *
 * $Id: octeon_sfp.c 144469 2016-08-31 04:51:48Z awilliams $
 *
 */

#include <common.h>
#include <command.h>
#include <exports.h>
#include <net.h>
#include <fdtdec.h>
#include <i2c.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_sfp.h>
#ifdef CONFIG_PCA953X
# include <pca953x.h>
#endif
#ifdef CONFIG_PCA9555
# include <pca9555.h>
#endif
#ifdef CONFIG_PCA9554
# include <pca9554.h>
#endif
#ifdef CONFIG_PCA9698
# include <pca9698.h>
#endif
#include <sfp.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

/** Module match information */
struct sfp_module {
	uint8_t vendor_oui[3];		/** Vendor OID */
	char *product_id;	/** Vendor product ID */
	bool an_enabled;
};

/**
 * Array of SFP modules which require autonegotiation to be disabled.  All of
 * these modules are SFP to CAT5/6 adapters made by Avago/Foxconn.  It's
 * possible I may have missed some.
 */
static const struct sfp_module sfp_modules[] = {
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5700RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5701RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5710RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-571NRZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5712RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-571BRZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5702RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5730GZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5730RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5740RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5730ARZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5740ARZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5740AGZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5731ARZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5741ARZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5741AGZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5731RZ",
		false,
	},
	{
		{ 0x00, 0x17, 0x6a },
		"ABCU-5741RZ",
		false,
	},
};

#ifdef CONFIG_PHY_VITESSE
extern int octeon_vitesse_vsc8488_gpio_set(struct phy_device *phydev,
					   int pin, bool enable);
extern int octeon_vitesse_vsc8488_gpio_get(struct phy_device *phydev,
					   int pin);
#endif

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
bool octeon_check_sfp_autonegotiation(struct eth_device *dev)
	__attribute__((weak, alias("__octeon_check_sfp_autonegotiation")));

bool __octeon_check_sfp_autonegotiation(struct eth_device *dev)
{
	struct octeon_eth_info *ethinfo = dev->priv;
	struct octeon_sfp_info *sfp = &ethinfo->sfp;
	struct phy_sfp_info *sfp_info = &sfp->sfp_info;
	uint8_t buffer[256];
	char env_str[32];
	int rc;
	int i;

	snprintf(env_str, sizeof(env_str), "%s_disable_an", dev->name);

	if (getenv(env_str))
		return false;

	/* Assume autonegotiation if no module present */
	if ((sfp->check_mod_abs &&
	     sfp->check_mod_abs(dev, sfp->mod_abs_data)) ||
	    sfp->i2c_eeprom_addr  <= 0)
		return true;

	rc = phy_sfp_read_i2c_eeprom(buffer, sfp->i2c_bus,
				     sfp->i2c_eeprom_addr);
	if (rc) {
		/* Module eeprom could not be read, return autonegotiation OK */
		debug("%s(%s): Could not read SFP EEPROM at 0x%x:0x%x\n",
		      __func__, dev->name, sfp->i2c_bus,
		      sfp->i2c_eeprom_addr);
		return true;
	}
	rc = phy_sfp_parse_eeprom(&sfp->sfp_info, buffer);
	if (rc) {
		printf("%s(%s): Error: could not parse SFP EEPROM\n",
		       __func__, dev->name);
		return true;
	}
	for (i = 0; i < ARRAY_SIZE(sfp_modules); i++) {
		/* See if OUI and product ID matches, if so, disable AN */
		if (!memcmp(sfp_info->vendor_oui, sfp_modules[i].vendor_oui, 3) &&
		    !memcmp((char *)sfp_info->vendor_pn,
			    sfp_modules[i].product_id,
			    strlen(sfp_modules[i].product_id)))
			return sfp_modules[i].an_enabled;
	}
	return true;
}

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
int octeon_sfp_read_eeprom(struct eth_device *dev)
{
	uint8_t buffer[256];
	struct octeon_eth_info *ethinfo = dev->priv;
	struct octeon_sfp_info *sfp = &ethinfo->sfp;
	int rc;

	debug("%s(%s)\n", __func__, dev->name);
	if (!sfp->valid) {
		printf("%s(%s): No valid SFP info\n", __func__, dev->name);
		return -1;
	}
	if (sfp->i2c_bus < 0 || sfp->i2c_eeprom_addr < 0) {
		printf("%s(%s): No valid eeprom i2c info\n",
		       __func__, dev->name);
		return -1;
	}
	if (sfp->check_mod_abs) {
		rc = sfp->check_mod_abs(dev, sfp->mod_abs_data);
		if (rc) {
			debug("%s(%s): module absent\n", __func__, dev->name);
			return -1;
		}
		if (rc != sfp->last_mod_abs && rc >= 0) {
			mdelay(250);	/* Wait for connection to settle */
			if (sfp->mod_abs_changed)
				sfp->mod_abs_changed(dev, rc,
						     sfp->mod_abs_changed_data);
			sfp->last_mod_abs = rc;
		} else if (rc < 0) {
			printf("%s(%s): Error reading mod_abs/mod_prs\n",
			       __func__, dev->name);
			return -1;
		}
	}
	if (sfp->is_qsfp) {
		if (sfp->select.pin >= 0) {
			/* Select the module by pulling the select pin low */
			debug("%s(%s): Selecting QSFP EEPROM\n",
			      __func__, dev->name);
			octeon_sfp_set_gpio_pin(&sfp->select, false);
			mdelay(10);
		}

	}
	debug("%s: Reading SFP EEPROM at 0x%x:0x%x\n", __func__,
	      sfp->i2c_bus, sfp->i2c_eeprom_addr);
	rc = phy_sfp_read_i2c_eeprom(buffer, sfp->i2c_bus, sfp->i2c_eeprom_addr);
	if (sfp->is_qsfp) {
		if (sfp->select.pin >= 0)
			octeon_sfp_set_gpio_pin(&sfp->select, true);

	}
	if (rc) {
		printf("%s(%s): Error reading sfp/qsfp EEPROM\n",
		       __func__, dev->name);
		return -1;
	}
	rc = phy_sfp_parse_eeprom(&sfp->sfp_info, buffer);
	return rc;
}

/**
 * Returns a pointer to the (Q)SFP EEPROM info.  Note that it  may not be valid.
 *
 * @param[in]	dev	Pointer to Ethernet device
 *
 * @return	Pointer to SFP EEPROM data or NULL if no SFP slot exists.
 */
struct phy_sfp_info *octeon_sfp_get_eeprom_info(const struct eth_device *dev)
{
	struct octeon_eth_info *ethinfo = dev->priv;
	struct octeon_sfp_info *sfp = &ethinfo->sfp;

	if (sfp->valid)
		return &sfp->sfp_info;
	else
		return NULL;
}

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
			     const char *prop_name)
{
	char parent_name[64];
	const uint32_t *pin_info;
	const void *fdt = gd->fdt_blob;
	int length;
	int gpio_node = -1;

	gpio_info->pin = -1;
	gpio_info->type = GPIO_TYPE_UNKNOWN;
	gpio_info->gpio_node = -1;
	gpio_info->i2c_bus = -1;
	gpio_info->i2c_addr = -1;
	gpio_info->inverted = false;

	debug("%s(0x%x, %p, %s)\n", __func__, eth_node, gpio_info, prop_name);
	pin_info = fdt_getprop(fdt, eth_node, prop_name, &length);
	if (!pin_info) {
		debug("%s: Could not find %s in device tree\n",
		      __func__, prop_name);
		return 0;
	}
	if (length == 12) {
		gpio_node = fdt_node_offset_by_phandle(fdt,
					       fdt32_to_cpu(pin_info[0]));
	} else {
		snprintf(parent_name, sizeof(parent_name),
			 "%s-parent", prop_name);
		gpio_node = fdtdec_lookup_phandle(fdt, eth_node, parent_name);
	}
	if (gpio_node < 0) {
		printf("%s(%s): Could not get gpio phandle\n",
		       __func__, prop_name);
		return -1;
	}
	gpio_info->gpio_node = gpio_node;
	switch (length) {
	case 12:
		gpio_info->pin = fdt32_to_cpu(pin_info[1]);
		gpio_info->inverted = !!pin_info[2];
		break;
	case 8:
		gpio_info->inverted = !!pin_info[1];
	case 4:
		gpio_info->pin = fdt32_to_cpu(pin_info[0]);
		break;
	default:
		printf("%s(%s): Error: unexpected parameter length %d\n",
		       __func__, prop_name, length);
		return -1;
	}
	/* GPIO type can be either an i2c device or a PHY */
	if (octeon_fdt_get_gpio_info(gpio_node, &gpio_info->type,
				     &gpio_info->i2c_bus,
				     &gpio_info->i2c_addr)) {
		debug("%s: Unknown i2c GPIO type, trying phy type\n", __func__);
		gpio_info->phydev = octeon_fdt_get_phy_gpio_info(gpio_node,
								 &gpio_info->type);
		if (!gpio_info->phydev) {
			printf("%s(%s): Error: could not get GPIO type info\n",
			       __func__, prop_name);
			return -1;
		}
	}
	debug("%s: Got GPIO info for %s\n", __func__, prop_name);
	return 0;
}

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
 * pins are found.  To see if the SFP/QSFP slot is actually valid, check
 * sfp_info->valid.
 */
int octeon_sfp_get_info(int eth_node, struct octeon_sfp_info *sfp_info)
{
	int rc;
	int eeprom_node;
	int diag_node;
	int sfp_node;

	sfp_info->is_qsfp = false;
	sfp_info->disable_an = false;
	sfp_info->i2c_bus = -1;
	sfp_info->i2c_eeprom_addr = -1;
	sfp_info->valid = false;

	sfp_node = fdtdec_lookup_phandle(gd->fdt_blob, eth_node, "sfp-slot");
	if (sfp_node < 0) {
		debug("%s: sfp-slot not found\n", __func__);
		sfp_node = fdtdec_lookup_phandle(gd->fdt_blob, eth_node,
						 "qsfp-slot");
		sfp_info->is_qsfp = (sfp_node > 0);
	} else {
		debug("%s: Found sfp-slot\n", __func__);
	}
	if (sfp_node < 0)
		return 0;

	debug("%s: Found %ssfp node 0x%x\n", __func__,
	      sfp_info->is_qsfp ? "q" : "", sfp_node);
	if (sfp_info->is_qsfp) {
		debug("%s: Getting QSFP parameters\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->tx_disable,
					      "lp_mode");
		if (rc)
			debug("%s: Error getting lp_mode\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->select,
					      "select");
		if (rc)
			debug("%s: Error getting select\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->mod_abs,
					      "mod_prs");
		if (rc)
			debug("%s: Error getting mod_prs\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->interrupt,
					      "interrupt");
		if (rc)
			debug("%s: Error getting interrupt\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->reset,
					      "reset");
		if (rc)
			debug("%s: Error getting reset\n", __func__);
	} else {
		debug("%s: Getting SFP parameters\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->tx_disable,
					      "tx_disable");
		if (rc)
			debug("%s: Error getting tx_disable\n", __func__);

		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->mod_abs,
					      "mod_abs");
		if (rc)
			debug("%s: Error getting mod_abs\n", __func__);
		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->tx_error,
					      "tx_error");
		if (rc)
			debug("%s: Error getting tx_error\n", __func__);

		rc = octeon_sfp_get_gpio_info(sfp_node, &sfp_info->rx_los,
					      "rx_los");
		if (rc)
			debug("%s: Error getting rx_los\n", __func__);
	}

	eeprom_node = fdtdec_lookup_phandle(gd->fdt_blob, sfp_node, "eeprom");
	if (eeprom_node >= 0) {
		sfp_info->i2c_eeprom_addr =
			fdtdec_get_int(gd->fdt_blob, eeprom_node, "reg", 0x50);
		sfp_info->i2c_bus =
			octeon_fdt_i2c_get_bus(gd->fdt_blob, eeprom_node);
		debug("%s: eeprom at 0x%x:0x%x\n", __func__, sfp_info->i2c_bus,
		      sfp_info->i2c_eeprom_addr);
	} else {
		debug("%s: EEPROM not found for SFP slot\n", __func__);
	}

	sfp_info->valid = sfp_info->i2c_bus >= 0 || sfp_info->mod_abs.pin >= 0;

	diag_node = fdtdec_lookup_phandle(gd->fdt_blob, sfp_node, "diag");
	if (diag_node >= 0)
		sfp_info->i2c_diag_addr =
			fdtdec_get_int(gd->fdt_blob, diag_node, "reg", 0x51);
	else
		sfp_info->i2c_diag_addr = 0x51;

	return 0;
}

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
				       void *data), void *data)
{
	struct octeon_eth_info *eth_info = dev->priv;
	struct octeon_sfp_info *sfp_info = &eth_info->sfp;

	sfp_info->mod_abs_changed = mod_abs_changed;
	sfp_info->mod_abs_changed_data = data;
	return 0;
}

/**
 * Sets an output SFP/QSFP GPIO pin
 *
 * @param[in]	gpio	data structure describing the GPIO pin
 * @param	set	Set to true to set the pin, false to clear.  Note that
 *			the outpput may be inverted automatically.
 * @return	0 for success, -1 on error.
 */
int octeon_sfp_set_gpio_pin(struct octeon_sfp_gpio_info *gpio, bool set)
{
	int old_i2c_bus __attribute__((unused));

	if (gpio->inverted)
		set = !set;
	if (gpio->pin < 0) {
		printf("%s: GPIO pin not defined\n", __func__);
		return -1;
	}
	switch (gpio->type) {
	case GPIO_TYPE_OCTEON:
		gpio_set_value(gpio->pin, set);
		break;
#ifdef CONFIG_PCA953X
	case GPIO_TYPE_PCA953X:
		pca953x_set_val(gpio->i2c_bus, gpio->i2c_addr,
				1 << gpio->pin, set << gpio->pin);
		break;
#endif
#ifdef CONFIG_PCA9554
	case GPIO_TYPE_PCA9554:
		pca9554_set_val(gpio->i2c_bus, gpio->i2c_addr,
				1 << gpio->pin, set << gpio->pin);
		break;
#endif
#ifdef CONFIG_PCA9555
	case GPIO_TYPE_PCA9555:
		pca9555_set_val(gpio->i2c_bus, gpio->i2c_addr,
				1 << gpio->pin, set << gpio->pin);
		break;
#endif
#ifdef CONFIG_PCA9698
	case GPIO_TYPE_PCA9698:
		old_i2c_bus = i2c_get_bus_num();
		i2c_set_bus_num(gpio->i2c_bus);
		pca9698_set_value(gpio->i2c_addr, gpio->pin, set);
		i2c_set_bus_num(old_i2c_bus);
		break;
#endif
#ifdef CONFIG_PHY_VITESSE
	case GPIO_TYPE_VSC8488:
		octeon_vitesse_vsc8488_gpio_set(gpio->phydev, gpio->pin, set);
		break;
#endif
	default:
		printf("%s: Unsupported GPIO type for GPIO\n", __func__);
		return -1;
	}
	return 0;
}

/**
 * Default function used to check the status of the mod_abs pin for SFP+ slots
 *
 * @param	dev	Ethernet device
 * @param	data	User-defined data (not used)
 *
 * @return	0 if module present, 1 if module absent, -1 on error
 */
int octeon_sfp_check_mod_abs(struct eth_device *dev, void *data)
	__attribute__((weak, alias("__octeon_sfp_check_mod_abs")));

int __octeon_sfp_check_mod_abs(struct eth_device *dev, void *data)
{
	struct octeon_eth_info *eth_info = dev->priv;
	struct octeon_sfp_info *sfp_info = &eth_info->sfp;
	struct octeon_sfp_gpio_info *mod_abs_info = &sfp_info->mod_abs;
	int result = -1;
	int old_i2c_bus __attribute__((unused));

	debug("%s(%s, %p) pin: %d\n", __func__, dev->name, data,
	      mod_abs_info->pin);
	if (mod_abs_info->pin < 0) {
		debug("%s: mod_abs/mod_prs not set\n", __func__);
		return 0;
	}
	switch (mod_abs_info->type) {
	case GPIO_TYPE_OCTEON:
		result = gpio_get_value(mod_abs_info->pin);
		break;
#ifdef CONFIG_PCA953X
	case GPIO_TYPE_PCA953X:
		debug("%s: PCA953X reading 0x%x:0x%x\n", __func__,
		      mod_abs_info->i2c_bus, mod_abs_info->i2c_addr);
		old_i2c_bus = i2c_get_bus_num();
		result = pca953x_get_val(mod_abs_info->i2c_bus,
					 mod_abs_info->i2c_addr);
		debug("%s: result: 0x%x\n", __func__, result);
		if (result >= 0)
			result = (result >> mod_abs_info->pin) & 1;
		i2c_set_bus_num(old_i2c_bus);
		break;
#endif
#ifdef CONFIG_PCA9554
	case GPIO_TYPE_PCA9554:
		debug("%s: PCA9554 reading 0x%x:0x%x\n", __func__,
		      mod_abs_info->i2c_bus, mod_abs_info->i2c_addr);
		old_i2c_bus = i2c_get_bus_num();
		result = pca9554_get_val(mod_abs_info->i2c_bus,
					 mod_abs_info->i2c_addr);
		debug("%s: result: 0x%x\n", __func__, result);
		if (result >= 0)
			result = (result >> mod_abs_info->pin) & 1;
		i2c_set_bus_num(old_i2c_bus);
		break;
#endif
#ifdef CONFIG_PCA9555
	case GPIO_TYPE_PCA9555:
		debug("%s: PCA9555 reading 0x%x:0x%x\n", __func__,
		      mod_abs_info->i2c_bus, mod_abs_info->i2c_addr);
		old_i2c_bus = i2c_get_bus_num();
		result = pca9555_get_val(mod_abs_info->i2c_bus,
					 mod_abs_info->i2c_addr);
		debug("%s: result: 0x%x\n", __func__, result);
		if (result >= 0)
			result = (result >> mod_abs_info->pin) & 1;
		i2c_set_bus_num(old_i2c_bus);
		break;
#endif
#ifdef CONFIG_PCA9698
	case GPIO_TYPE_PCA9698:
		debug("%s: PCA9698 reading 0x%x:0x%x\n", __func__,
		      mod_abs_info->i2c_bus, mod_abs_info->i2c_addr);
		old_i2c_bus = i2c_get_bus_num();
		i2c_set_bus_num(mod_abs_info->i2c_bus);
		result = pca9698_get_value(mod_abs_info->i2c_addr,
					   mod_abs_info->pin);
		debug("%s: result: 0x%x\n", __func__, result);
		i2c_set_bus_num(old_i2c_bus);
		break;
#endif
#ifdef CONFIG_PHY_VITESSE
	case GPIO_TYPE_VSC8488:
		debug("%s: VSC8488 phy reading %s, pin %d\n",
		      __func__, mod_abs_info->phydev->dev->name,
		      mod_abs_info->pin);
		result = octeon_vitesse_vsc8488_gpio_get(eth_info->phydev,
							 mod_abs_info->pin);
		break;
#endif
	default:
		printf("Unknown mod_abs GPIO type %d for %s\n",
		       mod_abs_info->type, dev->name);
		break;
	}

	if (result >= 0 && mod_abs_info->inverted)
		result = !result;

	debug("%s(%s, %p) returning %d\n", __func__, dev->name, data, result);
	return result;
}

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
				 bool inverted)
	__attribute__((weak, alias("__octeon_sfp_register_mod_abs")));

void __octeon_sfp_register_mod_abs(struct eth_device *dev,
				   enum octeon_gpio_type type,
				   int i2c_bus, int i2c_addr, int pin,
				   bool inverted)
{
	struct octeon_eth_info *eth_info = dev->priv;
	struct octeon_sfp_info *sfp_info = &eth_info->sfp;
	struct octeon_sfp_gpio_info *mod_abs = &sfp_info->mod_abs;

	debug("%s(%s, %d, %x:%x, %d, %sinverted)\n", __func__, dev->name,
	      type, i2c_bus, i2c_addr, pin, inverted ? "" : "not ");
	mod_abs->type = type;
	mod_abs->i2c_bus = i2c_bus;
	mod_abs->i2c_addr = i2c_addr;
	mod_abs->pin = pin;
	mod_abs->inverted = inverted;
}

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
				      void *data)
	__attribute__((weak, alias("__octeon_sfp_register_mod_abs_func")));

void __octeon_sfp_register_mod_abs_func(struct eth_device *dev,
			int (*check_mod_abs)(struct eth_device *dev, void *data),
			void *data)
{
	struct octeon_eth_info *eth_info = dev->priv;
	struct octeon_sfp_info *sfp_info = &eth_info->sfp;

	debug("%s(%s): Registering %p(%p)\n",
	      __func__, dev->name, check_mod_abs, data);
	if (check_mod_abs == NULL && sfp_info->valid) {
		debug("%s: Registering default function\n", __func__);
		sfp_info->check_mod_abs = octeon_sfp_check_mod_abs;
		sfp_info->mod_abs_data = NULL;
	} else {
		sfp_info->check_mod_abs = check_mod_abs;
		sfp_info->mod_abs_data = data;
	}
	sfp_info->last_mod_abs = true;
}

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
void octeon_eth_get_mod_abs(const struct eth_device *dev,
			    enum octeon_gpio_type *type, int *i2c_bus,
			    int *i2c_addr, int *pin, bool *inverted)
	__attribute__((weak, alias("__octeon_eth_get_mod_abs")));

void __octeon_eth_get_mod_abs(const struct eth_device *dev,
			      enum octeon_gpio_type *type, int *i2c_bus,
			      int *i2c_addr, int *pin, bool *inverted)
{
	const struct octeon_eth_info *eth_info = dev->priv;
	const struct octeon_sfp_info *sfp = &eth_info->sfp;
	const struct octeon_sfp_gpio_info *mod_abs = &sfp->mod_abs;

	if (type)
		*type = mod_abs->type;
	if (i2c_bus)
		*i2c_bus = mod_abs->i2c_bus;
	if (i2c_addr)
		*i2c_addr = mod_abs->i2c_addr;
	if (pin)
		*pin = mod_abs->pin;
	if (inverted)
		*inverted = mod_abs->inverted;
}

/**
 * Command to print out SFP information based on the Ethernet device name
 */
static enum command_ret_t do_octsfpinfo(struct cmd_tbl_s *cmd, int flag,
					int argc, char * const argv[])
{
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;
	struct octeon_sfp_info *sfp;
	struct phy_sfp_info sfp_info;
	uint8_t buffer[256];
	int rc;

	if (argc < 2)
		return CMD_RET_USAGE;

	memset(buffer, 0, sizeof(buffer));
	ethdev = eth_get_dev_by_name(argv[1]);

	if (!ethdev) {
		printf("Ethernet device %s not found.\n", argv[1]);
		return CMD_RET_FAILURE;
	}
	ethinfo = (struct octeon_eth_info *)ethdev->priv;
	if (!ethinfo) {
		printf("%s: ethinfo not found for %s\n", __func__, argv[1]);
		return CMD_RET_FAILURE;
	}
	sfp = &ethinfo->sfp;
	debug("sfp at %p\n", sfp);
	if (!sfp) {
		printf("No SFP information is associated with %s\n", argv[1]);
		return CMD_RET_USAGE;
	}
	if (sfp->i2c_bus < 0) {
		printf("No SFP i2c bus for %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}

	if (!sfp->valid) {
		printf("%s: No valid SFP configuration for %s\n", __func__,
		       ethdev->name);
		return CMD_RET_FAILURE;
	}
	if (!sfp->check_mod_abs) {
		printf("%s: No check_mod_abs function registered with %s\n",
		       __func__, ethdev->name);
		return CMD_RET_FAILURE;
	}
	debug("%s: checking mod_abs, func at %p\n", __func__, sfp->check_mod_abs);
	if (sfp->check_mod_abs(ethdev, sfp->mod_abs_data) ||
		sfp->i2c_eeprom_addr <= 0) {
		printf("No SFP/SFP+ module attached to %s\n", argv[1]);
		return CMD_RET_SUCCESS;
	}

	printf("Reading SFP/SFP+ EEPROM at 0x%x:0x%x\n",
	       sfp->i2c_bus, sfp->i2c_eeprom_addr);
	rc = phy_sfp_read_i2c_eeprom(buffer, sfp->i2c_bus,
				     sfp->i2c_eeprom_addr);
	if (rc) {
		printf("Error reading SFP+ EEPROM for %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}
	debug("%s: parsing eeprom\n", __func__);
	rc = phy_sfp_parse_eeprom(&sfp_info, buffer);
	if (rc) {
		printf("Error parsing SFP+ EEPROM for %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}
	phy_sfp_print_info(&sfp_info);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(octsfpinfo, 2, 1, do_octsfpinfo,
	   "Prints out SFP/SFP+ module information",
	   "[ethdevice] - Ethernet device to print module data from");
