/*
 * (C) Copyright 2012-2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <phy.h>
#include <net.h>
#include <fdt_support.h>
#include <fdtdec.h>
#include <libfdt.h>
#include <i2c.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/octeon_eth.h>
#ifdef CONFIG_PHY_SFP
# include <asm/arch/octeon_sfp.h>
#endif
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-helper-fdt.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/octeon_board_phy.h>
#ifdef CONFIG_PCA953X
# include <pca953x.h>
#endif
#ifdef CONFIG_PCA9698
# include <pca9698.h>
#endif
#ifdef CONFIG_PCA9555
# include <pca9555.h>
#endif
#ifdef CONFIG_PCF857X
# include <pcf857x.h>
#endif
#ifdef CONFIG_PHY_CORTINA
# include <cortina.h>
#endif
#ifdef CONFIG_PHY_SFP
# include <sfp.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_PHYLIB_10G
/**
 * Configure generic clause 45 PHY
 *
 * @param ethdev	- ethernet device to configure
 * @param init_seq_str	- init sequence string, i,e. ti,reg-init
 *
 * @return 0 for success, error otherwise
 */
int octeon_fdt_generic_8023_c45_config(const struct eth_device *ethdev,
				       const char *init_seq_str)
	__attribute__((weak, alias("__octeon_fdt_generic_8023_c45_config")));


# ifdef CONFIG_PHY_VITESSE
/**
 * Configures the Vitesse vsc8488 for use on boards like the SNIC10xxx based
 * boards using the values in the flat device tree.
 *
 * @param ethdev - ethernet device to initialize PHY for
 *
 * @returns 0 for success, error otherwise
 */
int octeon_fdt_vitesse_config(const struct eth_device *ethdev);

#  if defined(CONFIG_PHYLIB_10G) && defined(CONFIG_PHY_SFP)
/**
 * This function configures the Vitesse PHY based on information read from the
 * SFP+ module SFF EEPROM
 *
 * @param ethdev - ethernet device to configure
 *
 * @returns 0 for success, error otherwise
 */
int octeon_vitesse_vsc8488_sfp_config(struct eth_device *ethdev);
#  endif
# endif
#endif

#ifdef CONFIG_PHY_VITESSE
/**
 * This function is called before and after all of the PHY initialization
 * to control the Vitesse COMA mode.
 */
int octeon_vitesse_coma(bool start);
#endif

#ifdef CONFIG_PHY_AQUANTIA
/**
 * @INTERNAL
 * Configures a Aquantia PHY based on the device tree
 *
 * @param ethdev - Ethernet device to configure
 *
 * @return 0 for success, error otherwise
 */
static int octeon_fdt_aquantia_config(const struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info =
		 (struct octeon_eth_info *)ethdev->priv;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s)\n", __func__, ethdev->name);
#ifdef CONFIG_PHYLIB_10G
	if (!fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
				       "ethernet-phy-ieee802.3-c45"))
		return octeon_fdt_generic_8023_c45_config(ethdev,
							  "aquantia,reg-init");
#endif

	/* Check for at least one compatible Aquantia PHY */
	if (fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
				      "aquantia,aqr105")) {
		printf("%s: Warning: unknown Aquantia PHY for %s\n",
		       __func__, ethdev->name);
		return 0;
	}

	return 0;
}
#endif /* CONFIG_PHY_AQUANTIA */

#if defined(CONFIG_PHYLIB_10G)
/**
 * Configure generic clause 45 PHY
 *
 * @param ethdev	- ethernet device to configure
 * @param init_seq_str	- init sequence string, i,e. ti,reg-init
 *
 * @return 0 for success, error otherwise
 */
int __octeon_fdt_generic_8023_c45_config(const struct eth_device *ethdev,
				         const char *init_seq_str)
{
	struct octeon_eth_info *oct_eth_info =
		 (struct octeon_eth_info *)ethdev->priv;
	uint8_t dev;
	uint16_t reg;
	uint16_t mask;
	uint16_t value;
	int len;
	uint32_t *init_seq;
	int seq_len;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s)\n", __func__, ethdev->name);

	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   init_seq_str, &len);
	if (!init_seq)
		return 0;

	if (len % 16 != 0) {
		printf("Invalid init sequence in DT for generic Clause 45 PHY for %s\n",
		       ethdev->name);
		return -1;
	}

	seq_len = len / 16;
	while (seq_len--) {
		dev = fdt32_to_cpu(*init_seq++);
		reg = fdt32_to_cpu(*init_seq++);
		mask = fdt32_to_cpu(*init_seq++);
		if (!mask) {
			value = fdt32_to_cpu(*init_seq++);
		} else {
			value = phy_read(oct_eth_info->phydev, dev, reg);
			value &= mask;
			value |= fdt32_to_cpu(*init_seq++);
		}
		debug("Init for %s addr: 0x%x, dev: 0x%x, reg: 0x%x, mask: 0x%x, value: 0x%x\n",
		      ethdev->name, oct_eth_info->phydev->addr, dev, reg, mask,
		      value);
		phy_write(oct_eth_info->phydev, dev, reg, value);
	}
	return 0;
}
#endif

#ifdef CONFIG_PHY_MARVELL
const char const *marvell_devices[] = {
	"marvell,88e1118",
	"marvell,88e1149",
	"marvell,88e1149r",
	"marvell,88e1145",
	"marvell,88e1240",
	"marvell,88e1510",
	"marvell,88e1512",
	"marvell,88e1518",
	"marvell,88e1680",
	"marvell,88x3120",
	NULL
};

/**
 * @INTERNAL
 * Configures a Marvell PHY based on the device tree
 *
 * @param ethdev - Ethernet device to configure
 *
 * @return 0 for success, error otherwise
 */
static int octeon_fdt_marvell_config(const struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info =
		 (struct octeon_eth_info *)ethdev->priv;
	uint32_t page = 0, reg, mask, value;
	uint32_t devad = MDIO_DEVAD_NONE;
	int last_page = -1;
	int len;
	uint32_t *init_seq;
	int seq_len;
	bool is_c45 = false;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s)\n", __func__, ethdev->name);
#ifdef CONFIG_PHYLIB_10G
	if (!fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
				       "ethernet-phy-ieee802.3-c45"))
		return octeon_fdt_generic_8023_c45_config(ethdev,
							  "marvell,reg-init");
#endif

	/* Check for at least one compatible Marvell PHY */
	if (octeon_fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
					     marvell_devices)) {
		printf("%s: Warning: unknown Marvell PHY for %s\n",
		       __func__, ethdev->name);
		return 0;
	}

	is_c45 = !fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
					    "marvell,88x3120");

	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   "marvell,reg-init", &len);
	if (!init_seq)
		return 0;

	if (len % 16 != 0) {
		printf("Invalid init sequence in DT for Marvell PHY for %s\n",
		       ethdev->name);
		return -1;
	}


	seq_len = len / 16;
	while (seq_len--) {
		if (is_c45)
			devad = fdt32_to_cpu(*init_seq++);
		else
			page = fdt32_to_cpu(*init_seq++);

		reg = fdt32_to_cpu(*init_seq++);
		mask = fdt32_to_cpu(*init_seq++);
		if (!is_c45 && page != last_page) {
			phy_write(oct_eth_info->phydev, MDIO_DEVAD_NONE,
				  0x16, page);
			last_page = page;
		}

		if (mask) {
			value = phy_read(oct_eth_info->phydev, devad, reg);
			value &= mask;
			value |= fdt32_to_cpu(*init_seq++);
		} else {
			value = fdt32_to_cpu(*init_seq++);
		}
		debug("Marvell init for %s addr: %d: %s: %d, reg: %d, mask: 0x%x, value: 0x%x\n",
		      ethdev->name, oct_eth_info->phydev->addr,
		      is_c45 ? "dev" : "page", is_c45 ? devad : page, reg,
		      mask, value);

		phy_write(oct_eth_info->phydev, devad, reg, value);
	}
	/* Reset to page 0 */
	if (!is_c45 && last_page != 0)
		phy_write(oct_eth_info->phydev, MDIO_DEVAD_NONE, 0x16, 0);

	return 0;
}
#endif /* CONFIG_PHY_MARVELL */

#ifdef CONFIG_PHY_BROADCOM

const char const * broadcom_devices[] = {
	"broadcom,bcm5466",
	"broadcom,bcm5482",
	"broadcom,bcm5464r",
	"broadcom,bcm5464",
	"broadcom,bcm5241",
	"broadcom,bcm7806",
	"broadcom,bcm8706",
	"broadcom,bcm5241",
	"broadcom,bcm8727",
	"broadcom,bcm84856",
	NULL
};

/**
 * @INTERNAL
 * Configures a Broadcom PHY based on the device tree
 *
 * @param ethdev - Ethernet device to configure
 *
 * @return 0 for success, error otherwise
 */
static int octeon_fdt_broadcom_config(const struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info =
		 (struct octeon_eth_info *)ethdev->priv;
	uint32_t devid = MDIO_DEVAD_NONE;
	uint32_t reg, mask;
	int value;
	int c45 = 0;	/* True if clause-45 mode */
	int len;
	uint32_t *init_seq;
	int seq_len;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s)\n", __func__, ethdev->name);

	/* Check for at least one compatible Broadcom PHY */
	if (octeon_fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
					     broadcom_devices)) {
		printf("%s: Unknown Broadcom phy %s\n",
		       __func__, ethdev->name);
		return 0;
	}

	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   "broadcom,c45-reg-init", &len);
	if (init_seq)
		c45 = 1;
	else
		init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob,
						   phy_node_offset,
						   "broadcom,reg-init", &len);
	if (!init_seq)
		return 0;

	if ((c45 && (len % 16 != 0)) || (!c45 && (len % 12 != 0))) {
		printf("Invalid init sequence in DT for Broadcom PHY for %s (c45: %d, len: %d)\n",
		       ethdev->name, c45, len);
		return -1;
	}

	seq_len = len / (c45 ? 16 : 12);
	while (seq_len--) {
		devid = (c45) ? fdt32_to_cpu(*init_seq++) : MDIO_DEVAD_NONE;
		reg = fdt32_to_cpu(*init_seq++);
		mask = fdt32_to_cpu(*init_seq++);

		if (mask) {
			value = phy_read(oct_eth_info->phydev, devid, reg);
			if (value < 0) {
				printf("%s: Error reading from phy for %s at %d:%d\n",
				       __func__, ethdev->name, devid, reg);
				return -1;
			}
			debug("%s: Read: 0x%x, mask: 0x%x\n", __func__, value, mask);
			value &= mask;
			value |= fdt32_to_cpu(*init_seq++);
		} else {
			value = fdt32_to_cpu(*init_seq++);
		}
		debug("Broadcom init for %s addr: %d: dev: %d, reg: 0x%x, mask: 0x%x, value: 0x%x\n",
		      ethdev->name, oct_eth_info->phydev->addr, devid, reg,
		      mask, value);

		if (phy_write(oct_eth_info->phydev, devid, reg, value) < 0) {
			printf("%s: Error writing 0x%x to phy for %s at %d:%d\n",
			       __func__, value, ethdev->name, devid, reg);
			return -1;
		}
	}

	return 0;
}
#endif /* CONFIG_PHY_BROADCOM */

/**
 * @INTERNAL
 * Configures generic 802.3 C22 PHY
 *
 * @param ethdev - ethernet device to initialize PHY force
 *
 * @return 0 for success, error otherwise
 */
static int octeon_fdt_generic_8023_c22_config(const struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info = ethdev->priv;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;
	int len, seq_len;
	uint32_t *init_seq;
	uint8_t dev;
	uint16_t reg, mask, value;

	debug("%s(%s)\n", __func__, ethdev->name);

	/* Check for compatible Micrel PHY */
	if (fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
				      "ethernet-phy-ieee802.3-c22"))
		return 0;

	/* It is possible for some PHYs to require special "clause 45"
	 * initialization sequences instead of the regular clause 22
	 * sequences.
	 */
	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   "c45-reg-init", &len);
	if (init_seq && len > 0) {
		if (len % 16 != 0) {
			printf("Invalid clause 45 init sequence in DT for generic Clause 22 PHY for %s\n",
			       ethdev->name);
			return -1;
		}
		seq_len = len / 16;
		while (seq_len--) {
			dev = fdt32_to_cpu(*init_seq++);
			reg = fdt32_to_cpu(*init_seq++);
			mask = fdt32_to_cpu(*init_seq++);
			if (!mask) {
				value = fdt32_to_cpu(*init_seq++);
			} else {
				value = phy_read(oct_eth_info->phydev, dev, reg);
				value &= mask;
				value |= fdt32_to_cpu(*init_seq++);
			}
			debug("C45 init for %s addr: 0x%x, dev: 0x%x, reg: 0x%x, mask: 0x%x, value: 0x%x\n",
			     ethdev->name, oct_eth_info->phydev->addr, dev,
			     reg, mask, value);
			phy_write(oct_eth_info->phydev, dev, reg, value);
		}
	}

	/* Look for clause 22 init sequence */
	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   "reg-init", &len);
	if (!init_seq)
		return 0;

	if (len % 12 != 0) {
		printf("Invalid init sequence in DT for generic 802.3 PHY for %s\n",
		       ethdev->name);
		return -1;
	}

	seq_len = len / 12;
	while (seq_len--) {
		reg = fdt32_to_cpu(*init_seq++);
		mask = fdt32_to_cpu(*init_seq++);
		if (mask) {
			value = phy_read(oct_eth_info->phydev, MDIO_DEVAD_NONE,
					 reg);
			value &= mask;
			value |= fdt32_to_cpu(*init_seq++);
		} else {
			value = fdt32_to_cpu(*init_seq++);
		}
		debug("Generic PHY init for %s addr: %d, reg: %d, mask: 0x%x, value: 0x%x\n",
		      ethdev->name, oct_eth_info->phydev->addr, reg, mask, value);
	}

	return 0;
}

#if defined(CONFIG_PHY_TI) && defined(CONFIG_PHYLIB_10G)
/**
 * @INTERNAL
 * Configures a TI PHY based on the device tree
 *
 * @param ethdev - ethernet device PHY is attached to
 *
 * @return 0 for success, error otherwise
 */
static int octeon_fdt_ti_config(const struct eth_device *ethdev)
{
	uint16_t value;
	struct octeon_eth_info *oct_eth_info = ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	int phy_node_offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s)\n", __func__, ethdev->name);

	/* Check for at least one compatible TI PHY */
	if (fdt_node_check_compatible(gd->fdt_blob, phy_node_offset,
				      "ti,tlk10232")) {
		printf("%s: Warning: unknown TI PHY for %s\n",
		       __func__, ethdev->name);
		return 0;
	}

#define	CHANNEL_CONTROL_1 0x1e, 0x0001
	value =  phy_read(phydev, CHANNEL_CONTROL_1);
	value |= 1; /* REFCLK_1_P/N as clock */
	phy_write(phydev, CHANNEL_CONTROL_1, value);

#define HS_CH_CONTROL_1 0x1e, 0x001d
	value =  phy_read(phydev, HS_CH_CONTROL_1);
	value |= 0x1000; /* REFCLK = 312.5MHz */
	phy_write(phydev, HS_CH_CONTROL_1, value);

#define AN_CONTROL 0x07, 0x0000
	value =  phy_read(phydev, AN_CONTROL);
	value &= 0xefff; /* disable autonegotiation */
	phy_write(phydev, AN_CONTROL, value);

#define LT_TRAIN_CONTROL 0x01, 0x0096
	value =  phy_read(phydev, LT_TRAIN_CONTROL);
	value &= 0xfffd; /* turn off LT_TRAINING_ENABLE */
	phy_write(phydev, LT_TRAIN_CONTROL, value);

#define TI_RESERVED_20 0x1e, 0x8020
	value = 3; /* Magic */
	phy_write(phydev, TI_RESERVED_20, value);

#define RESET_CONTROL 0x1e, 0x000e
	value =  phy_read(phydev, RESET_CONTROL);
	value |= 8; /* Reset channel */
	phy_write(phydev, RESET_CONTROL, value);

	return 0;
}
#endif

#ifdef CONFIG_PHY_CORTINA
static int octeon_fdt_cortina_config(struct eth_device *ethdev,
				     int fdt_node)
{
	struct octeon_eth_info *ethinfo = ethdev->priv;
	struct phy_device *phydev = ethinfo->phydev;
	const void *fdt = gd->fdt_blob;
	int rc;
	cvmx_helper_interface_mode_t mode;
	int phy_nexus_node, slice_node;
	const char *slice_name;
	int slice_num;
#ifdef CONFIG_PHY_SFP
	struct cortina_phy_info *pinfo = phydev->priv;
	struct octeon_sfp_info *sfp = &ethinfo->sfp;
	struct phy_sfp_info sfp_info;
	const uint32_t *eeprom_handle;
	int phandle;
	int eeprom_node;
	int i2c_bus_node;
	bool sfp_valid = false;
#endif

	mode = cvmx_helper_interface_get_mode(ethinfo->interface);

	slice_name = "cortina,cs4343-slice";
	if (fdt_node_check_compatible(fdt, fdt_node, slice_name))
		slice_name = "cortina,cs4223-slice";
	if (fdt_node_check_compatible(fdt, fdt_node, slice_name))
		slice_name = NULL;
	if ((mode == CVMX_HELPER_INTERFACE_MODE_XLAUI ||
	     mode == CVMX_HELPER_INTERFACE_MODE_40G_KR4) && slice_name) {
		phy_nexus_node = fdt_parent_offset(fdt, fdt_node);
		if (phy_nexus_node < 0) {
			printf("%s(%s): Error %d getting slice parent offset\n",
			       __func__, ethdev->name, phy_nexus_node);
			return -1;
		}
		debug("%s(%s): Processing all slices\n", __func__, ethdev->name);
		slice_node = fdt_node_offset_by_compatible(fdt,
							   phy_nexus_node,
							   slice_name);
		do {
			if (!slice_node ||
			    fdt_parent_offset(fdt, slice_node) != phy_nexus_node)
				break;
			slice_num = fdtdec_get_int(fdt, slice_node, "reg", -1);
			if (slice_num < 0) {
				printf("%s(%s): Error: slice reg field in device tree missing\n",
				       __func__, ethdev->name);
				return -1;
			}
			debug("%s: Configuring slice for 40G\n", __func__);
			rc = cortina_phy_config_fdt(phydev, (void *)fdt,
						    slice_node);
			if (rc)
				break;
			slice_node = fdt_node_offset_by_compatible(fdt,
								   slice_node,
								   slice_name);
		} while (!rc && slice_node > 0);

	} else {
		rc = cortina_phy_config_fdt(phydev, (void *)fdt, fdt_node);
	}
	if (rc) {
		printf("%s: Error configuring phy connected to %s\n",
		       __func__, ethdev->name);
		return rc;
	}

	if (fdt_node_check_compatible(fdt, fdt_node, "cortina,cs4343-slice") &&
	    fdt_node_check_compatible(fdt, fdt_node, "cortina,cs4223-slice"))
		return 0;

#if defined(CONFIG_OCTEON_I2C_FDT) && defined(CONFIG_PHY_SFP)
	eeprom_handle = (uint32_t *)fdt_getprop(fdt, fdt_node,
						"sfp_eeprom", NULL);
	if (!eeprom_handle)
		return 0;
	phandle = fdt32_to_cpu(*eeprom_handle);
	eeprom_node = fdt_node_offset_by_phandle(fdt, phandle);
	debug("%s: EEPROM node for %s is %d\n", __func__,
	      ethdev->name, eeprom_node);
	if (eeprom_node < 0) {
		printf("%s: Invalid phandle for sfp EEPROM for phy connected to %s\n",
		       __func__, ethdev->name);
		return -1;
	}
	pinfo->fdt_sfp_eeprom_node = eeprom_node;
	pinfo->sfp_i2c_addr = fdtdec_get_int(fdt, eeprom_node,
					     "reg", -1);
	i2c_bus_node = fdt_parent_offset(fdt, eeprom_node);
	if (i2c_bus_node < 0) {
		printf("%s: Could not get SFP EEPROM parent node\n", __func__);
		return -1;
	}
	debug("%s: i2c bus node: %d\n", __func__, i2c_bus_node);

	pinfo->sfp_i2c_bus = sfp->i2c_bus;
	pinfo->sfp_i2c_addr = sfp->i2c_eeprom_addr;
	debug("%s: i2c bus node for %s is %d\n", __func__, ethdev->name,
	      pinfo->sfp_i2c_bus);

	if (pinfo->sfp_i2c_addr > 0) {
		uint8_t sfp_buffer[SFP_MAX_EEPROM_SIZE];
		sfp_valid = true;
		rc = phy_sfp_read_i2c_eeprom(sfp_buffer,
					     pinfo->sfp_i2c_bus,
					     pinfo->sfp_i2c_addr);
		if (!rc)
			rc = phy_sfp_parse_eeprom(&sfp_info, sfp_buffer);
		sfp_valid = !rc;
	}
	if (sfp_valid) {
	}
#endif

	return 0;
}
#endif

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
			      const void *fdt_blob, int fdt_offset)
{
	return 0;
}
int octeon_board_pre_phy_init(struct eth_device *dev,
			      const void *fdt_blob, int fdt_offset)
	__attribute__((weak));

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
			       const void *fdt_blob, int fdt_offset)
{
	return 0;
}

int octeon_board_post_phy_init(struct eth_device *dev,
			       const void *fdt_blob, int fdt_offset)
	__attribute__((weak));

/**
 * Initializes all of the PHYs on a board using the device tree information.
 *
 * @return 0 for success, error otherwise
 */
int octeon_board_phy_init(void)
{
	struct eth_device *ethdev, *next_ethdev, *start;
	struct octeon_eth_info *oct_eth_info;

	int err = 0;
	int rc = 0;
	int phy_off;
#ifdef CONFIG_PHY_VITESSE
	int need_coma = 0;
#endif

	debug("%s()\n", __func__);
	next_ethdev = start = eth_get_dev();
	if (!start) {
		debug("%s: No starting eth device found\n", __func__);
		return 0;
	}

	do {
		ethdev = next_ethdev;
		next_ethdev = ethdev->next;

		oct_eth_info = (struct octeon_eth_info *)ethdev->priv;
		if (!oct_eth_info) {
			printf("%s: ERROR: ethernet private information not set\n",
			       __func__);
		}
		phy_off = oct_eth_info->phy_fdt_offset;

		debug("%s: Initializing %s\n", __func__, ethdev->name);
		if (!oct_eth_info->phydev) {
			debug("No phy device for %s, continuing...\n",
			      ethdev->name);
			ethdev = ethdev->next;
			continue;
		}
#ifdef DEBUG
		switch (oct_eth_info->phy_device_type) {
		case BROADCOM_GENERIC_PHY:
			debug("  Broadcom PHY\n");
			break;
		case MARVELL_GENERIC_PHY:
			debug("  Marvell PHY\n");
			break;
		case CORTINA_PHY:
			debug("  Cortina PHY\n");
			break;
		case AQUANTIA_PHY:
			debug("  Aquantia PHY\n");
			break;
		case GENERIC_8023_C22_PHY:
			debug("  Generic 802.3 C22 PHY\n");
			break;
		case GENERIC_8023_C45_PHY:
			debug("  Generic 802.3 C45 PHY\n");
			break;
		case INBAND_PHY:
			debug("  Inband PHY\n");
			break;
		case QUALCOMM_S17:
			debug("  Qualcomm QCA833X S17 switch\n");
			break;
		default:
			debug("  Unknown PHY type %d\n",
			      oct_eth_info->phy_device_type);
			break;
		}
		debug("Compatible: %s\n", (char *)fdt_getprop(gd->fdt_blob,
							      phy_off,
							      "compatible",
							      NULL));
#endif /* DEBUG */

		if (octeon_board_pre_phy_init(ethdev, gd->fdt_blob, phy_off)) {
			debug("%s: phy pre-init for %s failed, skipping init\n",
			      __func__, ethdev->name);
			continue;
		}

#ifdef CONFIG_PHY_MARVELL
		if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy_off,
					      "marvell")) {
			debug("Configuring Marvell PHY\n");
			if ((rc = octeon_fdt_marvell_config(ethdev)))
				printf("Error configuring Marvell PHY for %s\n",
				       ethdev->name);
		} else
#endif
#ifdef CONFIG_PHY_BROADCOM
		if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy_off,
					      "broadcom")) {
			debug("Configuring Broadcom PHY\n");
			if ((rc = octeon_fdt_broadcom_config(ethdev)))
				printf("Error configuring Broadcom PHY for %s\n",
				       ethdev->name);
		} else
#endif
#ifdef CONFIG_PHY_AQUANTIA
		if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy_off,
					      "aquantia")) {
			debug("Configuring Aquantia PHY\n");
			if ((rc = octeon_fdt_aquantia_config(ethdev)))
				printf("Error configuring Aquantia PHY for %s\n",
				       ethdev->name);
		} else
#endif
#ifdef CONFIG_PHY_VITESSE
		if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy_off,
					      "vitesse")) {
			debug("Configuring Vitesse PHY\n");
			if (!need_coma) octeon_vitesse_coma(true);
			need_coma = 1;

			rc = octeon_fdt_vitesse_config(ethdev);
#if defined(CONFIG_PHYLIB_10G) && defined(CONFIG_PHY_SFP)
			/* Only read SFP+ for 10G parts */
			if (!fdt_node_check_compatible(gd->fdt_blob, phy_off,
						       "vitesse,vsc8488") ||
			    !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						       "vitesse,vsc8486"))
				rc |= octeon_vitesse_vsc8488_sfp_config(ethdev);
#endif
			if (rc)
				printf("Error configuring Vitesse PHY for %s\n",
				       ethdev->name);
		} else
#endif
#ifdef CONFIG_PHY_TI
		if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy_off, "ti")) {
			debug("Configuring TI PHY\n");
			rc = octeon_fdt_ti_config(ethdev);
			if (rc)
				printf("Error configuring TI PHY for %s\n",
				       ethdev->name);
		} else
#endif
		if (!fdt_node_check_compatible(gd->fdt_blob, phy_off,
					       "ethernet-phy-ieee802.3-c22")) {
			debug("Configuring generic 802.3 C22 PHY\n");
			rc = octeon_fdt_generic_8023_c22_config(ethdev);
			if (rc)
				printf("Error configuring generic 802.3 C22 PHY for %s\n",
				       ethdev->name);
#ifdef CONFIG_PHYLIB_10G
		} else if (!fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "ethernet-phy-ieee802.3-c45")) {
			rc = octeon_fdt_generic_8023_c45_config(ethdev,
								"reg-init");
			if (rc)
				printf("Error configuring generic 802.3 C34 PHY for %s\n",
				       ethdev->name);
#endif
#ifdef CONFIG_PHY_CORTINA
		} else if (!fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "cortina,cs4318") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "cortina,cs4223") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "cortina,cs4343") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "cortina,cs4343-slice") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "cortina,cs4223-slice")) {
			octeon_fdt_cortina_config(ethdev, phy_off);
#endif
#ifdef CONFIG_PHY_ATHEROS
		} else if (!fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "atheros,ar8334") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "atheros,ar8337") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "qualcomm,qca8334") ||
			   !fdt_node_check_compatible(gd->fdt_blob, phy_off,
						      "qualcomm,qca8337")) {
			debug("No config for Qualcomm/Atheros S17 switch\n");
#endif
		} else {
			printf("%s: Unknown PHY type %s for %s\n",
			       __func__,
			       (char *)fdt_getprop(gd->fdt_blob, phy_off,
						   "compatible", NULL),
			       ethdev->name);
			err |= 1;
		}
		err |= rc;

		debug("%s: Calling post phy init for %s\n",
		      __func__, ethdev->name);
		err |= octeon_board_post_phy_init(ethdev, gd->fdt_blob, phy_off);
	} while (next_ethdev != start);

#if defined(CONFIG_PHY_VITESSE)
	if (need_coma)
		octeon_vitesse_coma(false);
#endif
	return err;
}


#if defined(CONFIG_PHY_MARVELL) && defined(CONFIG_PHY_MARVELL_FLASH_PGM)
/* dev 1 */
#define MV_MAX_BUFF_SIZE_OUT_REG	0xc028
#define MV_ACTUAL_BUFF_SIZE_IN_REG	0xc029
#define MV_COMMAND_REG			0xc02a
#define MV_WORDS_WRITTEN_REG		0xc02b
#define MV_LOW_ADDRESS_REG		0xc02c
#define MV_HIGH_ADDRESS_REG		0xc02d
#define MV_DATA_REG			0xc02e
#define MV_CHECKSUM_REG			0xc02f
#define MV_WORDS_RCVD_REG		0xc030
/* dev 3 */
#define MV_REG_RAM_CHECKSUM		0xd05b

/* Host commands */
#define MV_CMD_ERASE_FLASH_PROGRAM_AREA	0x1
#define MV_CMD_FILL_BUFFER		0x2
#define MV_CMD_WRITE_VERIFY_BUFFER	0x6
#define MV_CMD_WRITE_BUFFER		MV_CMD_WRITE_VERIFY_BUFFER
#define MV_CMD_VERIFY_FLASH		0x7

/* Slave responses */
#define MV_RSP_OK			0x100
#define MV_RSP_ERROR_CMD		0x200
#define MV_RSP_FLASH_BUSY		0x300
#define MV_RSP_VERIFY_ERR		0x4000

/* Error codes */
#define MV_ERR_SIZE_NOT_EVEN		0xffe0
#define MV_ERR_ERASING_FLASH		0xffe1
#define MV_ERR_VALUE_READ_BACK		0xffe2
#define MV_ERR_START_WRITE_DATA		0xffe3
#define MV_ERR_SLAVE_FAIL_TO_GET_DATA	0xffe4
#define MV_ERR_ON_SLAVE			0xffe5
#define MV_ERR_CHECKSUM			0xffe6
#define MV_ERR_SLAVE_WRITE_FULL		0xffe7
#define MV_ERR_LAST_TRANSFER		0xffe7
#define MV_ERR_RAM_HW_CHECKSUM_ERR	0xffe8
#define MV_ERR_HW_CHECKSUM_ERR		0xffe9
#define MV_ERR_PHY_NOT_IN_DOWNLOAD_MODE	0xffff

#define MV_POS_ERR_FATAL_BOOT		0
#define MV_POS_BOOT_PROGRESS		1
#define MV_POS_BOOT_COMPLETE		3
#define MV_POS_CODE_STARTED		4
#define MV_POS_UL_CHECKSUM_OK		5
#define MV_POS_CODE_DOWNLOADED		6
#define MV_POS_UL_PROGRESS		7
#define MV_POS_RESET_CAUSE		8

#define MV_MASK_FATAL_BOOT_ERROR	(1 << MV_POS_ERR_FATAL_BOOT)
#define MV_MASK_BOOT_PROGRESS		(3 << MV_POS_BOOT_PROGRESS)
#define MV_MASK_BOOT_COMPLETE		(1 << MV_POS_BOOT_COMPLETE)
#define MV_MASK_CODE_STARTED		(1 << MV_POS_CODE_STARTED)
#define MV_MASK_UL_CHECKSUM_OK		(1 << MV_POS_UL_CHECKSUM_OK)
#define MV_MASK_CODE_DOWNLOADED		(1 << MV_POS_CODE_DOWNLOADED)
#define MV_MASK_UL_PROGRESS		(1 << MV_POS_UL_PROGRESS)
#define MV_MASK_RESET_CAUSE		(3 << MV_POS_RESET_CAUSE)

/* Boot progress */
#define MV_BOOT_INIT			0x00
#define MV_BOOT_WAIT_MDIO		0x01
#define MV_BOOT_CHECKSUM_STARTED	0x02
#define MV_BOOT_JUMPING_TO_APP		0x03

static void marvell_88x3140_special_software_reset(struct phy_device *phydev)
{
	uint16_t val;
	val = phy_read(phydev, 1, 0xc000);
	phy_write(phydev, 1, 0xc000, val | 0x8000);
}

/**
 * Download a new firmware image into the PHY's SPI flash
 *
 * @param[in]	phydev	PHY device to program
 * @param[in]	data	Pointer to PHY image data
 * @param	size	Size of image in bytes
 *
 * @return	0 for success, -1 on error
 */
int marvell_88x3140_flash_download(struct phy_device *phydev,
				   const uint8_t *data, size_t size)
{
	uint16_t val;
	int max_buf_size;
	int size_left = size;
	int write_size;
	int byte_index = 0;
	uint16_t buf_checksum, tmp_checksum;
	int words_rcvd;
	int stop_index;

	debug("%s(%s, %p, %u)\n", __func__, phydev->dev->name, data, size);
	/* Erase the flash program area */
	printf("Erasing flash program area...");
	phy_write(phydev, 1, MV_COMMAND_REG, MV_CMD_ERASE_FLASH_PROGRAM_AREA);

	do {
		val = phy_read(phydev, 1, MV_COMMAND_REG);
	} while (val == MV_CMD_ERASE_FLASH_PROGRAM_AREA ||
		 val == MV_RSP_FLASH_BUSY);

	if (val == MV_RSP_ERROR_CMD) {
		printf("\nSlave encountered error while erasing flash.\n");
		return -1;
	}
	if (val == MV_RSP_OK) {
		puts("  Done.\n");
	} else {
		printf("Unexpected response 0x%x from phy.  Exiting.\n", val);
		return -1;
	}

	max_buf_size = phy_read(phydev, 1, MV_MAX_BUFF_SIZE_OUT_REG);
	max_buf_size *= 2;	/* convert to bytes */

	debug("%s: max buffer size: %u bytes\n", __func__, max_buf_size);
	printf("Writing %u bytes to flash...", size_left);

	while (size_left > 0) {
		if (size_left > max_buf_size)
			write_size = max_buf_size;
		else
			write_size = size_left;
		stop_index = byte_index + write_size;
		/* Set the flash start index */
		phy_write(phydev, 1, MV_LOW_ADDRESS_REG, byte_index & 0xffff);
		phy_write(phydev, 1, MV_HIGH_ADDRESS_REG, byte_index >> 16);
		/* Set the size of the buffer being sent */
		phy_write(phydev, 1, MV_ACTUAL_BUFF_SIZE_IN_REG, write_size / 2);
		/* Tell the slave we've written the start address and size and
		 * now we're going to start writing data
		 */
		phy_write(phydev, 1, MV_COMMAND_REG, MV_CMD_FILL_BUFFER);
		do {
			val = phy_read(phydev, 1, MV_COMMAND_REG);
		} while (val == MV_CMD_FILL_BUFFER);

		if (val != MV_RSP_OK) {
			printf("\nExpected OK response for fill buffer command, got 0x%x\n", val);
			return -1;
		}

		buf_checksum = 0;
		phy_read(phydev, 1, MV_CHECKSUM_REG);
		while (byte_index < stop_index) {
			val = data[byte_index++];
			val |= (((uint16_t)data[byte_index++]) << 8);
			buf_checksum += val;
			phy_write(phydev, 1, MV_DATA_REG, val);
		}
		tmp_checksum = phy_read(phydev, 1, MV_CHECKSUM_REG);
		words_rcvd = phy_read(phydev, 1, MV_WORDS_RCVD_REG);
		if (tmp_checksum != buf_checksum ||
		    words_rcvd * 2 != write_size) {
			printf("\nError: slave failed to get all the data correctly.  Slave received %u bytes, expected %u bytes, checksum: 0x%x, expected checksum: 0x%x\n",
			       words_rcvd * 2, write_size, tmp_checksum,
			       buf_checksum);
			return -1;
		}

		/* One full RAM buffer inside DSP is ready to write to FLASH now */
		phy_write(phydev, 1, MV_COMMAND_REG, MV_CMD_WRITE_BUFFER);
		do {
			mdelay(250);
			val = phy_read(phydev, 1, MV_COMMAND_REG);
		} while (val == MV_CMD_WRITE_BUFFER || val == MV_RSP_FLASH_BUSY);
		if (val == MV_RSP_ERROR_CMD) {
			printf("\nSome kind of error occurred on slave.\n");
			return -1;
		} else if (val != MV_RSP_OK) {
			printf("\nExpected OK from slave, got 0x%x\n", val);
			return -1;
		} else {
			tmp_checksum = phy_read(phydev, 1, MV_CHECKSUM_REG);
			if (tmp_checksum != buf_checksum) {
				printf("\nExpected checksum 0x%x, got 0x%x\n",
				       buf_checksum, tmp_checksum);
				return -1;
			}
		}

		val = phy_read(phydev, 1, MV_WORDS_WRITTEN_REG);
		if (val != write_size / 2) {
			printf("\nSlave did not write enough words to flash.\n");
			return -1;
		}
		size_left -= write_size;
	}
	puts("  Done.\n");
	return 0;
}

/**
 * Downloads an image into the PHY DSP RAM and begins executing it
 *
 * @param[in]	phydev		Phy device to load image into
 * @param[in]	data		Pointer to PHY image to load
 * @param	size		Size of image in bytes
 * @param	use_ram_checksum	Validate the checksum of image in RAM
 *
 * @return	0 for success, -1 on error
 *
 * NOTE:	This is used to load a special image used for downloading
 *		new firmware into the SPI flash.
 */
int marvell_88x3140_mdio_ram_download(struct phy_device *phydev,
				      const uint8_t *data,
				      size_t size, bool use_ram_checksum)
{
	uint16_t val;
	uint16_t ram_checksum, expected_checksum;
	size_t size_left = size;
	uint8_t low_byte, high_byte;
	int buf_count;

	marvell_88x3140_special_software_reset(phydev);
	mdelay(250);
	/* Make sure we can access the DSP and it's in the correct mode
	 * (waiting for download)
	 */
	val = phy_read(phydev, 3, 0xd000);
	if (val != 0x000a) {
		printf("PHY DSP is not waiting in download mode, read 0x%x, expected 0x000A\n",
		       val);
		return -1;
	}
	if (use_ram_checksum)
		/* Read register to clear it */
		ram_checksum = phy_read(phydev, 3, MV_REG_RAM_CHECKSUM);

	/* Set starting RAM address to 0x0000 */
	phy_write(phydev, 3, 0xd004, 0);
	phy_write(phydev, 3, 0xd005, 0);

	buf_count = 0;
	expected_checksum = 0;
	while (size_left > 0) {
		low_byte = data[buf_count++];
		high_byte = data[buf_count++];
		expected_checksum += (low_byte + high_byte);
		phy_write(phydev, 3, 0xd006,
			  (uint16_t)((uint16_t)high_byte) << 8 | low_byte);
		size_left -= 2;
	}

	if (use_ram_checksum) {
		ram_checksum = phy_read(phydev, 3, MV_REG_RAM_CHECKSUM);
		if (expected_checksum != ram_checksum) {
			printf("Error downloading code.  Expected RAM hw checksum to be 0x%x but was 0x%x\n",
			       expected_checksum, ram_checksum);
			return -1;
		}
	}
	/* Now start the code */
	val = phy_read(phydev, 3, 0xd000);
	phy_write(phydev, 3, 0xd000, val | (1 << 6));
	mdelay(500);

	return 0;
}

/**
 * Binary image used to download firmware into flash for the Marvell 88x3140 PHY.
 */
#include "phy/mv88x3140_flash_download_slave.h"

int do_marvell88x3140(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct phy_device *phydev;
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;
	uint32_t addr;
	size_t size;
	int rc;

	if (argc < 4) {
		printf("Wrong number of arguments (%d)\n", argc);
		return CMD_RET_USAGE;
	}

	if (argv[1][0] != 'p') {
		printf("Error: only program supported\n");
		return CMD_RET_USAGE;
	}

	ethdev = eth_get_dev_by_name(argv[2]);
	if (!ethdev) {
		printf("Could not find %s\n", argv[2]);
		return -1;
	}
	ethinfo = ethdev->priv;
	phydev = ethinfo->phydev;
	if (!phydev) {
		printf("No phy associated with %s\n", ethdev->name);
		return -1;
	}

	rc = marvell_88x3140_mdio_ram_download(phydev,
					       mv_88x3140_flashdownloadslave,
					       sizeof(mv_88x3140_flashdownloadslave),
					       true);
	if (rc) {
		printf("Could not load slave download image\n");
		return CMD_RET_FAILURE;
	}
	addr = simple_strtoul(argv[3], NULL, 16);
	size = simple_strtoul(argv[4], NULL, 16);
	debug("address: 0x%x, size: %u bytes\n", addr, size);
	printf("Please remove the Marvell PHY flash programming jumper and press ENTER\n");
	getc();
	rc = marvell_88x3140_flash_download(phydev, (uint8_t *)addr, size);
	puts("Please replace the Marvell PHY flash programming jumper now and press ENTER\n");
	getc();
	marvell_88x3140_special_software_reset(phydev);
	if (!rc)
		return CMD_RET_SUCCESS;
	else
		return CMD_RET_FAILURE;
}

U_BOOT_CMD(marvell88x3140, 5, 0, do_marvell88x3140,
	   "Marvell 88x3140 tool",
	   "program <eth device> <address> <size> - program the image at <address> of size <size> into the PHY flash\n"
	   "                                        of the specified device (i.e. octeth0)\n"
	   "                                        All values are in hex.\n");

#endif
