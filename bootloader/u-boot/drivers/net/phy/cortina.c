/*
 * Cortina PHY Management code
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Copyright 2011 Freescale Semiconductor, Inc.
 * author Andy Fleming
 *
 * Copyright 2014-2016 Caviun, Inc. <support@cavium.com>
 * author Aaron Williams
 *
 * Based off of the generic PHY management code
 *
 * The Cortina/Inphi PHYs differs from other phys in that they do not follow
 * any of the usual PHY standards.  The Cortina PHYs use devid 0 and the chip
 * IDs are stored in registers 0 and 1 instead of 2 and 3.
 *
 * Due to these differences, it is not possible to use the standard PHY
 * identification code, and making changes to the standard code would make it
 * difficult to support both Cortina/Inphi and other more standard devices.
 */

#include <common.h>
#ifdef CONFIG_PHY_CORTINA_CS4223
# include <command.h>
# include <asm/arch/octeon_eth.h>
#endif
#include <malloc.h>
#include <net.h>
#include <command.h>
#include <miiphy.h>
#include <phy.h>
#include <cortina.h>
#include <errno.h>
#include <linux/err.h>
#ifdef CONFIG_PHY_CORTINA_CS4223
# include <cortina_cs4321.h>
#endif
#include <sfp.h>
#ifdef CONFIG_OF_LIBFDT
# include <fdtdec.h>
#endif

/** Default mode for CS4224 */
#ifndef CONFIG_CORTINA_CS4224_DEFAULT_MODE
# define CONFIG_CORTINA_CS4224_DEFAULT_MODE	CORTINA_SLICE_UNKNOWN
#endif

/**
 * LSB of IEEE 1149.1 ID value.  Bits 15-12 are the LSB 4 bits of the 16-bit
 * product ID, 11-1 are the Cortina manufacturer ID, 0x1f2.  Bit 0 is always 0.
 */
#define CORTINA_GLOBAL_CHIP_ID_LSB	0
/**
 * MSB of IEEE 1149.1 ID value.
 */
#define CORTINA_GLOBAL_CHIP_ID_MSB	1

#define CS4224_PP_LINE_LINEMISC_SOFT_RESET		0x1000
#define CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG	0x1001
#define CS4224_PP_LINE_LINEMISC_GIGEPCS_SOFT_RESET	0x1002
#define CS4224_PP_HOST_HOSTMISC_SOFT_RESET		0x1800
#define CS4224_PP_HOST_HOSTMISC_MPIF_RESET_DOTREG	0x1801
#define CS4224_PP_HOST_HOSTMISC_GIGEPCS_SOFT_RESET	0x1802
#define CS4224_EEPROM_LOADER_CONTROL			0x5000
#define CS4224_EEPROM_LOADER_STATUS			0x5001

static LIST_HEAD(cortina_phy_drivers);

static const char *mode_to_str[] = {
	"SR", "CX", "1000Base-X", "Unknown"
};

#ifdef CONFIG_PHY_CORTINA_CS4223
/**
 * Slice offsets for CS4343, note there are 2 dies
 *
 * Die 0 has bit 0 set in the MDIO address whereas die 1 has bit 0 clear,
 * which seems totally backwards.
	 */
static const uint16_t cs4224_pp_sku_cs4343_offset[8] = {
	0x0000, 0x1000, 0x2000, 0x3000, 0x3000, 0x2000, 0x1000, 0x0000
};

static const uint16_t cs4224_mseq_sku_cs4343_offset[8] = {
	0x0000, 0x0000, 0x100, 0x100, 0x100, 0x100, 0x0000, 0x0000
};

static const uint16_t cs4224_pp_sku_cs4224_offset[16] = {
	0x0000, 0x1000, 0x3000, 0x1000, 0x2000, 0x0000, 0x3000, 0x2000,
	0x0000, 0x2000, 0x1000, 0x3000, 0x2000, 0x0000, 0x1000, 0x3000
};

static const uint16_t cs4224_mseq_sku_cs4224_offset[16] = {
	0x0000, 0x0000, 0x0100, 0x0000, 0x0100, 0x0000, 0x0100, 0x0100,
	0x0000, 0x0100, 0x0000, 0x0100, 0x0100, 0x0000, 0x0000, 0x0100
};

static const uint16_t cs4224_pp_sku_cs4221_offset[10] = {
	0x1000, 0x0000, 0x1000, 0x0000, 0x3000, 0x1000, 0x2000, 0x3000,
	0x0000, 0x1000
};

static const uint16_t cs4224_mseq_sku_cs4221_offset[10] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0100, 0x0100,
	0x0000, 0x0000
};


static bool cs4224_is_eeprom_finished(struct mii_dev *bus, int addr);
#endif	/* CONFIG_PHY_CORTINA_CS4223 */

int board_cortina_startup(struct phy_device *phydev) __attribute__((weak));

int board_cortina_startup(struct phy_device *phydev)
{
	return 0;
}

/**
 * Register a Cortina PHY driver.
 *
 * @param drv	Driver to register
 *
 * @return 0 for success
 */
int cortina_phy_register(struct phy_driver *drv)
{
	debug("%s(%s)\n", __func__, drv->name);
	INIT_LIST_HEAD(&drv->list);
	list_add_tail(&drv->list, &cortina_phy_drivers);

	return 0;
}

int cortina_phy_reset(struct phy_device *phydev)
{
	debug("%s(%s)\n", __func__, phydev->dev ? phydev->dev->name : "unknown");
	return 0;
}

int cs4224_mmi_bus(struct mii_dev *bus)
{
	return 1;
}

int cs4224_mmi_bus(struct mii_dev *bus) __attribute__((weak));

/**
 * get_phy_id - reads the specified addr for its ID.
 * @bus: the target MII bus
 * @addr: PHY address on the MII bus
 * @phy_id: where to store the ID retrieved.
 *
 * Description: Reads the ID registers of the PHY at @addr on the
 *   @bus, stores it in @phy_id and returns zero on success.
 */
static int cortina_get_phy_id(struct mii_dev *bus, int addr, uint32_t *phy_id)
{
	int phy_reg;
	bool finished;
#ifdef CONFIG_PHY_CORTINA_CS4223
	if (cs4224_mmi_bus(bus)) {
		int retries = 5000;

		/* Wait for firmware to finish loading */
		do {
			finished = cs4224_is_eeprom_finished(bus, addr);
			if (!finished)
				mdelay(1);
		} while (retries-- > 0 && !finished);
	}
#endif
	/* The Cortina PHY stores the PHY id at addresses 0 and 1 */
	phy_reg = bus->read(bus, addr, 0, CORTINA_GLOBAL_CHIP_ID_MSB);
	if (phy_reg < 0)
		return -EIO;

	*phy_id = (phy_reg & 0xffff) << 16;
	phy_reg = bus->read(bus, addr, 0, CORTINA_GLOBAL_CHIP_ID_LSB);
	if (phy_reg < 0)
		return -EIO;
	*phy_id |= (phy_reg & 0xffff);

	debug("%s(%p, %d, %p): phy id: 0x%x\n", __func__, bus, addr,
	      phy_id, *phy_id);
	return 0;
}

static struct phy_driver *cortina_get_phy_driver(struct phy_device *phydev,
						 phy_interface_t interface)
{
	struct list_head *entry;
	int phy_id = phydev->phy_id;
	struct phy_driver *drv = NULL;

	list_for_each(entry, &cortina_phy_drivers) {
		drv = list_entry(entry, struct phy_driver, list);
		if ((drv->uid & drv->mask) == (phy_id & drv->mask))
			return drv;
	}
	return NULL;
}

static int cortina_phy_probe(struct phy_device *phydev)
{
	int err = 0;

	debug("%s(%s)\n", __func__, phydev->dev->name);
	phydev->advertising = phydev->supported = phydev->drv->features;

	if (phydev->drv->probe)
		err = phydev->drv->probe(phydev);

	return err;
}

/**
 * Create a new Cortina PHY device
 *
 * @param bus		MDIO bus
 * @param addr		MDIO address
 * @param sub_addr	Cortina sub-address used in some cases with Cortina
 *			PHYs such as the CS4223 in XFI mode.
 * @param phy_id	PHY ID reported by the PHY
 * @param interface	Interface type.  Note that this is currently useless
 *			for XLAUI and XFI modes.
 *
 * @return pointer to a new initialized PHY device or NULL if error
 */
struct phy_device *cortina_phy_device_create(struct mii_dev *bus,
					     int addr, char sub_addr,
					     u32 phy_id,
					     phy_interface_t interface)
{
	struct phy_device *dev;
	struct cortina_phy_info *pinfo;
	int i;
	bool quad_mode = (interface == PHY_INTERFACE_MODE_XLAUI ||
			  interface == PHY_INTERFACE_MODE_40G_KR4);

	debug("%s(%p, %d, %d, 0x%x, %u)\n", __func__, bus, addr, sub_addr,
	      phy_id, interface);
	/* We allocate the device and initialize the default values
	 * We also kill two birds with one stone, allocating the private data
	 * immediately after the phy device.
	 */

	dev = calloc(sizeof(*dev) + sizeof(*pinfo), 1);
	if (!dev) {
		printf("Failed to allocate PHY device for %s:%d\n",
		       bus->name, addr);
		return NULL;
	}

	pinfo = (struct cortina_phy_info *)(dev + 1);
	debug("%s: dev: %p, pinfo: %p\n", __func__, dev, pinfo);
	dev->priv = pinfo;
	dev->duplex = -1;
	dev->link = 1;
	dev->interface = interface;
	dev->autoneg = AUTONEG_ENABLE;
	dev->addr = addr;
	dev->phy_id = phy_id;
	dev->bus = bus;
	debug("%s: Getting PHY driver\n", __func__);
	dev->drv = cortina_get_phy_driver(dev, interface);

	if (!dev->drv) {
		debug("PHY driver not found for Cortina PHY on %s, address %u:%d, phy id: 0x%08x\n",
		       bus->name, addr, sub_addr, dev->phy_id);
		return NULL;
	}
	debug("%s: phy driver: %s\n", __func__, dev->drv->name);

	debug("Probing PHY driver\n");
	cortina_phy_probe(dev);

	INIT_LIST_HEAD(&pinfo->list);
	pinfo->phydev = dev;
	pinfo->sub_addr_used = sub_addr >= 0;
	pinfo->sub_addr = sub_addr;
	pinfo->sfp_i2c_addr = -1;
	pinfo->sfp_i2c_bus = -1;
#ifdef CONFIG_PHY_CORTINA_CS4223
	pinfo->hw_id = cs4224_get_hw_id(dev);
	switch (pinfo->hw_id) {
	case CORTINA_CS4224_HW_CS4223:	/* 4 port duplex */
		pinfo->max_slices = 4;
		for (i = 0; i < pinfo->max_slices; i++)
			pinfo->slice_offsets[i] =
					cs4224_pp_sku_cs4343_offset[i];
		break;
	case CORTINA_CS4224_HW_CS4224:	/* 16 port simplex */
		pinfo->max_slices = 16;
		for (i = 0; i < pinfo->max_slices; i++)
			pinfo->slice_offsets[i] =
				cs4224_pp_sku_cs4224_offset[i & 0xf];
		break;
	case CORTINA_CS4224_HW_CS4343:	/* 8 port duplex */
	case CORTINA_CS4224_HW_CS4341:	/* 8 port duplex */
		/* This is confusing.  When the LSB address bit is set we're
		 * talking to die 0, when it's clear it's die 1.
		 */
		pinfo->max_slices = 4;	/* 4 slices per device */
		for (i = 0; i < pinfo->max_slices; i++)
			pinfo->slice_offsets[i] = (addr & 1) ?
				cs4224_pp_sku_cs4343_offset[i & 0x3] :
				cs4224_pp_sku_cs4343_offset[(i & 0x3) + 4];
		break;
	case CORTINA_CS4224_HW_CS4221:	/* 10 port simplex */
		pinfo->max_slices = 10;
		for (i = 0; i < pinfo->max_slices; i++)
			pinfo->slice_offsets[i] =
				cs4224_pp_sku_cs4221_offset[i];
		break;
	case CORTINA_CS4224_HW_CS4227:	/* 2 port duplex */
		pinfo->max_slices = 2;
		for (i = 0; i << pinfo->max_slices; i++)
			pinfo->slice_offsets[i] =
				cs4224_pp_sku_cs4343_offset[i];
		break;
	case CORTINA_CS4224_HW_CS4210:	/* 16 port simplex */
		pinfo->max_slices = 16;
		for (i = 0; i < pinfo->max_slices; i++)
			pinfo->slice_offsets[i] =
				cs4224_pp_sku_cs4224_offset[i];
		break;
	default:
		printf("Unknown Cortina SKU for %d.%d\n", addr, sub_addr);
		free(dev);
		return NULL;
	}
#else
	pinfo->slice_offsets[sub_addr] = sub_addr * 0x1000;
#endif
	pinfo->mode = CONFIG_CORTINA_CS4224_DEFAULT_MODE;

	pinfo->quad_mode = quad_mode;

	/* If there's already an entry here, add ourselves to the end of the
	 * list.
	 */
	if (bus->phymap[addr]) {
		struct cortina_phy_info *pi = bus->phymap[addr]->priv;

		if (!pi || !pi->sub_addr_used) {
			printf("Error: Cortina PHY information missing or sub "
			       "address not supported from existing entry at "
			       "address %d:%d\n",
			       addr, sub_addr);
			free(dev);
			return NULL;
		}
		debug("%s: Adding phy driver to tail\n", __func__);
		list_add_tail(&pi->list, &pinfo->list);
	} else {
		debug("%s: Adding phy %d:%d to map\n", __func__, addr, sub_addr);
		bus->phymap[addr] = dev;
	}


	return dev;
}

static struct phy_device *cortina_create_phy_by_addr(struct mii_dev *bus,
				unsigned addr, char sub_addr,
				phy_interface_t interface)
{
	u32 phy_id = 0xffffffff;
	int r = cortina_get_phy_id(bus, addr, &phy_id);

	if (r < 0) {
		debug("%s(%s, 0x%x.%d, %d): could not get PHY id\n", __func__,
		      bus->name, addr, sub_addr, interface);
		return ERR_PTR(r);
	}

	/* If the PHY ID is mostly F's, we didn't find anything */
	if ((phy_id & 0x1fffffff) != 0x1fffffff) {
		debug("%s: addr: %u:%d, creating device\n", __func__,
		      addr, sub_addr);
		return cortina_phy_device_create(bus, addr, sub_addr,
						 phy_id,
						 interface);
	}
	return NULL;
}

static struct phy_device *cortina_search_for_existing_phy(struct mii_dev *bus,
							  unsigned phy_addr,
							  char sub_addr,
							  phy_interface_t interface)
{
	struct phy_device *phydev = bus->phymap[phy_addr];
	struct cortina_phy_info *pinfo = phydev->priv;
	struct list_head *entry;

	if (!phydev || !phydev->priv)
		return NULL;

	if (sub_addr >= 0 && !pinfo->sub_addr_used) {
		printf("%s(%p, %u, %d, %u): found entry but sub address is not used!\n",
		       __func__, bus, phy_addr, sub_addr, interface);
		return NULL;
	}

	if (pinfo->sub_addr_used) {
		if (pinfo->sub_addr_used && pinfo->sub_addr == sub_addr) {
			debug("%s: Found match, addr: %u:%d\n", __func__,
			      phy_addr, sub_addr);
			return phydev;
		}

		list_for_each(entry, &(pinfo->list)) {
			struct cortina_phy_info *pi;
			pi = list_entry(entry, struct cortina_phy_info, list);
			debug("%s: entry: %p, info: %p\n", __func__, entry,
			      pi);
			if (pi->sub_addr_used &&
			    pi->sub_addr == sub_addr) {
				phydev = pi->phydev;
				phydev->interface = interface;
				debug("%s: Found address %u, sub address %d\n",
				      __func__, phy_addr, sub_addr);
				return phydev;
			}
		}
	} else {
		debug("%s: Found match, addr: %u\n", __func__, phy_addr);
		phydev->interface = interface;
		return phydev;
	}
	return NULL;
}

static struct phy_device *cortina_get_phy_device_by_addr(struct mii_dev *bus,
							 unsigned phy_addr,
							 char sub_addr,
							 phy_interface_t interface)
{
	struct phy_device *phydev;

	phydev = cortina_search_for_existing_phy(bus, phy_addr, sub_addr,
						 interface);
	if (phydev)
		return phydev;

	debug("%s: Creating new phy, addr: %u:%d\n", __func__,
	      phy_addr, sub_addr);
	phydev = cortina_create_phy_by_addr(bus, phy_addr, sub_addr, interface);
	if (IS_ERR(phydev))
		return NULL;
	if (phydev)
		return phydev;
	puts("Phy not found\n");
	return cortina_phy_device_create(bus, phy_addr, sub_addr, 0xffffffff,
					 interface);
}

/**
 * Find a Cortina PHY given its address and sub-address
 *
 * @param bus		MDIO bus PHY is connected to
 * @param phy_addr	Address of PHY on MDIO bus
 * @param sub_addr	Sub-address of PHY (i.e. cs4223 in XFI mode)
 * @param interface	Interface mode
 *
 * @return pointer to phy device
 */
struct phy_device *cortina_phy_find_by_addr(struct mii_dev *bus,
					    unsigned phy_addr,
					    char sub_addr,
					    phy_interface_t interface)
{
	if (bus->reset)
		bus->reset(bus);

	/* Wait 15ms to make sure the PHY has come out of hard reset */
	mdelay(15);
	return cortina_get_phy_device_by_addr(bus, phy_addr, sub_addr,
					      interface);
}

/**
 * Connect a PHY device to an Ethernet device
 *
 * @param phydev	PHY device
 * @param dev		Ethernet device
 */
void cortina_phy_connect_dev(struct phy_device *phydev, struct eth_device *dev)
{
	struct cortina_phy_info *pinfo;
	/* Configure the PHY */
	phy_config(phydev);
	/* Soft reset the PHY */
	cortina_phy_reset(phydev);

	if (phydev->dev)
		printf("%s:%d is connected to %s.  Reconnecting to %s\n",
		       phydev->bus->name, phydev->addr,
		       phydev->dev->name, dev->name);
	phydev->dev = dev;
	pinfo = phydev->priv;
	if (pinfo)
		pinfo->mode = CORTINA_SLICE_UNKNOWN;
	debug("%s connected to %s\n", dev->name, phydev->drv->name);
}

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
				       phy_interface_t interface)
{
	struct phy_device *phydev;

	phydev = cortina_phy_find_by_addr(bus, addr, sub_addr, interface);
	if (phydev) {
		debug("%s: Found phy device for %d:%d\n", __func__,
		      addr, sub_addr);
		cortina_phy_connect_dev(phydev, dev);
	} else {
		printf("Could not get PHY for %s: addr %d:%d\n",
		       bus->name, addr, sub_addr);
	}
	return phydev;
}

#ifdef CONFIG_OF_LIBFDT
/**
 * Reads all of the values used for PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA and
 * PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB for a slice.
 *
 * @param	phydev		phy device
 * @param	fdt_blob	pointer to fdt
 * @param	node		offset of FDT slice node in device tree
 * @param	mode		FDT slice mode, can be CORTINA_SLICE_SR,
 *				CORTINA_SLICE_CX or CORTINA_SLICE_1000X.
 *
 * @return	0 if success, -1 if mode is invalid
 */
static int cortina_phy_get_ctrl_values(struct phy_device *phydev,
				       void *fdt_blob, int node,
				       int slice,
				       enum cortina_slice_mode mode)
{
	char parm_name[64];
	struct cortina_phy_info *pinfo = phydev->priv;
	uint16_t ctrla, ctrlb, val;
	const char *prefixes[3] = { "sr", "cx", "basex" };

	if (mode > CORTINA_SLICE_1000X) {
		printf("%s: Unsupported mode %d\n", __func__, mode);
		return -1;
	}
	snprintf(parm_name, sizeof(parm_name), "%s-stx-cmode-res",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 3);
	ctrla = (val & 7) << 12;
	snprintf(parm_name, sizeof(parm_name), "%s-stx-drv-lower-cm",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 8);
	ctrla |= (val & 0xf) << 8;
	snprintf(parm_name, sizeof(parm_name), "%s-stx-level",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 0x14);
	ctrla |= (val & 0x3f);
	snprintf(parm_name, sizeof(parm_name), "%s-stx-pre-peak",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 1);
	ctrlb = (val & 0x1f) << 8;
	snprintf(parm_name, sizeof(parm_name), "%s-stx-muxsubrate-sel",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 0);
	ctrlb |= (val & 0x1) << 7;
	snprintf(parm_name, sizeof(parm_name), "%s-stx-post-peak",
		 prefixes[mode]);
	val = fdtdec_get_int(fdt_blob, node, parm_name, 4);
	ctrlb |= (val & 0x3f);

	pinfo->tx_output_ctrla[slice][mode] = ctrla;
	pinfo->tx_output_ctrlb[slice][mode] = ctrlb;

	debug("%s: mode: %s, PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA: 0x%x\n",
	      __func__, prefixes[mode], ctrla);
	debug("%s: mode: %s, PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB: 0x%x\n",
	      __func__, prefixes[mode], ctrlb);

	return 0;
}

/**
 * This function is called by the host to parse some of the FDT fields for the
 * Cortina PHY.
 *
 * @param	phydev		PHY device for Cortina slice
 * @param	fdt_blob	Pointer to FDT
 * @param	node		node in FDT for the slice
 *
 * @return	0 for success.
 */
int cortina_phy_config_fdt(struct phy_device *phydev, void *fdt_blob, int node)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	int rc = 0;
	const char *compat;
	int reg;
	int i;

	debug("%s(%s, %p, 0x%x)\n", __func__, phydev->dev->name,
	      fdt_blob, node);
	for (i = 0; i < CORTINA_MAX_SLICES; i++)
		pinfo->fdt_nodes[i] = -1;
	pinfo->nexus_node = -1;

	if (!fdt_node_check_compatible(fdt_blob, node, "cortina,cs4343-slice") ||
	    !fdt_node_check_compatible(fdt_blob, node, "cortina,cs4223-slice")) {
		debug("%s(%s): old slice offset: 0x%x, sub addr: 0x%x\n",
		      __func__, phydev->dev->name,
		      pinfo->slice_offsets[pinfo->sub_addr],
		      pinfo->sub_addr);
		pinfo->sub_addr = fdtdec_get_int(fdt_blob, node, "reg",
						 pinfo->sub_addr);
		pinfo->fdt_nodes[pinfo->sub_addr] = node;
		pinfo->slice_offsets[pinfo->sub_addr] =
			fdtdec_get_int(fdt_blob, node, "slice_offset",
				        pinfo->slice_offsets[pinfo->sub_addr]);

		debug("%s(%s): slice offset: 0x%x, sub addr: 0x%x\n", __func__,
		      phydev->dev->name, pinfo->slice_offsets[pinfo->sub_addr],
		      pinfo->sub_addr);

		rc |= cortina_phy_get_ctrl_values(phydev, fdt_blob, node,
						  pinfo->sub_addr,
						  CORTINA_SLICE_SR);
		rc |= cortina_phy_get_ctrl_values(phydev, fdt_blob, node,
						  pinfo->sub_addr,
						  CORTINA_SLICE_CX);

		rc |= cortina_phy_get_ctrl_values(phydev, fdt_blob, node,
						  pinfo->sub_addr,
						  CORTINA_SLICE_1000X);
		if (rc)
			return rc;
	} else {
		int slice_node = node;

		if (!fdt_node_check_compatible(fdt_blob, node,
					       "cortina,cs4343")) {
			compat = "cortina,cs4343-slice";
		} else if (!!fdt_node_check_compatible(fdt_blob, node,
						       "cortina,cs4223")) {
			compat = "cortina,cs4223-slice";
		} else {
			printf("Unknown/unsupported Inphy/Cortina phy\n");
			return -1;
		}
		debug("%s: slice compat: %s\n", __func__, compat);
		/* We get here for XLAUI mode or 40G KR4 mode */
		pinfo->nexus_node = node;
		pinfo->quad_mode = true;

		/* Walk through all slices and get the values from the device
		 * tree.
		 */
		do {
			slice_node = fdt_node_offset_by_compatible(fdt_blob,
								   slice_node,
								   compat);
			debug("%s: slice node: 0x%x\n", __func__, slice_node);
			if (slice_node < 0 ||
			    fdt_parent_offset(fdt_blob, slice_node) != node) {
				debug("%s: mismatch parent offset\n", __func__);
				break;
			}

			reg = fdtdec_get_int(fdt_blob, slice_node, "reg", -1);
			debug("%s: getting control values for slice %d\n",
			      __func__, reg);
			if (reg < 0) {
				printf("%s(%s): Error: reg field missing in device tree\n",
				       __func__, phydev->dev->name);
				return -1;
			}
			if (reg >= pinfo->max_slices) {
				printf("%s(%s): Error: reg %d out of range\n",
				       __func__, phydev->dev->name, reg);
				return -1;
			}
			pinfo->fdt_nodes[reg] = slice_node;
			pinfo->slice_offsets[reg] =
				fdtdec_get_int(fdt_blob, slice_node,
					       "slice_offset",
					       pinfo->slice_offsets[reg]);
			rc = cortina_phy_get_ctrl_values(phydev, fdt_blob,
							 slice_node,
							 reg,
							 CORTINA_SLICE_SR);
			rc |= cortina_phy_get_ctrl_values(phydev, fdt_blob,
							  slice_node,
							  reg,
							  CORTINA_SLICE_CX);
			rc |= cortina_phy_get_ctrl_values(phydev, fdt_blob,
							  slice_node,
							  reg,
							  CORTINA_SLICE_1000X);
		} while (!rc);
	}
	return 0;
}
#endif
/**
 * Probe for a Cortina CS4223 PHY device
 *
 * @param phydev	Pointer to phy device data structure
 *
 * @return		0 if probe was successful, error otherwise.
 */
static int cs4223_probe(struct phy_device *phydev)
{
	int id_lsb, id_msb;

	id_lsb = phy_read(phydev, 0, CORTINA_GLOBAL_CHIP_ID_LSB);
	if (id_lsb < 0)
		return id_lsb;
	id_msb = phy_read(phydev, 0, CORTINA_GLOBAL_CHIP_ID_MSB);
	if (id_msb < 0)
		return -1;

	debug("%s(%u): ID: 0x%x\n", __func__, phydev->addr,
	      (id_msb << 16) | id_lsb);
	if ((id_lsb != 0x3e5) || ((id_msb & 0x0fff) != 0x0003))
		return -ENODEV;
	return 0;
}

static int cs4223_config(struct phy_device *phydev)
{
	/* This PHY supports both 10G and 40G and can run as XLAUI or XFI.
	 * Currently the U-Boot infrastructure (and Linux for that matter)
	 * doesn't really provide a way to detect these higher speed interfaces.
	 */
	phydev->supported = phydev->advertising =
		SUPPORTED_40000baseKR4_Full |
		ADVERTISED_40000baseKR4_Full;
	phydev->duplex = DUPLEX_FULL;
	phydev->speed = 10000;
	return 0;
}

/**
 * Resets the PHY
 *
 * @param phydev	Phy device to reset
 *
 * @return 0 for success.
 */
int cs4223_reset(struct phy_device *phydev)
{
	u16 reg;
	u16 value;
	ulong start;

	/* First check that the EEPROM has been properly loaded */
	value = phy_read(phydev, 0, CS4224_EEPROM_LOADER_STATUS);
	if ((value & 7) != 1) {
		/* Start downloading the firmware */
		reg = CS4224_EEPROM_LOADER_CONTROL;
		phy_write(phydev, 0, reg, 1);

		reg = CS4224_EEPROM_LOADER_STATUS;
		start = get_timer(0);
		do {
			value = phy_read(phydev, 0, reg);
		} while ((get_timer(start) < 500) && !(value & 1));
		if (value & 7) {
			printf("Error: %s firmware download timeout\n",
			       phydev->dev->name);
			return -1;
		}
	}

	/* Reset both the host and line sides */
	reg = CS4224_PP_HOST_HOSTMISC_SOFT_RESET;
	value = 0xb;
	phy_write(phydev, 0, reg, value);
	phy_write(phydev, 0, reg + 1000, value);
	phy_write(phydev, 0, reg + 2000, value);
	phy_write(phydev, 0, reg + 3000, value);

	reg = CS4224_PP_LINE_LINEMISC_SOFT_RESET;
	value = 0xb;
	phy_write(phydev, 0, reg, value);
	phy_write(phydev, 0, reg + 1000, value);
	phy_write(phydev, 0, reg + 2000, value);
	phy_write(phydev, 0, reg + 3000, value);

	mdelay(100);

	reg = CS4224_PP_HOST_HOSTMISC_SOFT_RESET;
	value = 0;
	phy_write(phydev, 0, reg, value);
	phy_write(phydev, 0, reg + 1000, value);
	phy_write(phydev, 0, reg + 2000, value);
	phy_write(phydev, 0, reg + 3000, value);

	reg = CS4224_PP_LINE_LINEMISC_SOFT_RESET;
	value = 0;
	phy_write(phydev, 0, reg, value);
	phy_write(phydev, 0, reg + 1000, value);
	phy_write(phydev, 0, reg + 2000, value);
	phy_write(phydev, 0, reg + 3000, value);

	return 0;
}

static struct phy_driver cortina_cs4223 = {
	.uid = 0x000303e5,
	.mask = 0x0fffffff,
	.name = "Cortina CS4223",
	.features = PHY_10G_FEATURES,
	.config = cs4223_config,
	.startup = board_cortina_startup,
	.shutdown = NULL,
	.probe = cs4223_probe,
	.reset = cs4223_reset,
};

int cortina_phy_init(void)
{
	int rc = 0;
#ifdef CONFIG_PHY_CORTINA_CS4321
	rc |= cortina_cs4321_phy_init();
#endif
#ifdef CONFIG_PHY_CORTINA_CS4318
	rc |= cortina_cs4318_phy_init();
#endif
	cortina_phy_register(&cortina_cs4223);

	return rc;
}

#ifdef CONFIG_PHY_CORTINA_CS4223

#define CS4224_GLOBAL_CHIP_ID_LSB                 0x0
#define CS4224_GLOBAL_CHIP_ID_LSB_dft             0x3E5
#define CS4224_GLOBAL_CHIP_ID_MSB                 0x1
#define CS4224_GLOBAL_CHIP_ID_MSB_dft             0x7003
#define CS4224_GLOBAL_SCRATCH                     0x2
#define CS4224_GLOBAL_SCRATCH_dft                 0x0
#define CS4224_GLOBAL_UCODE_VERSION_SR            0x3
#define CS4224_GLOBAL_UCODE_VERSION_SR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_CX1           0x4
#define CS4224_GLOBAL_UCODE_VERSION_CX1_dft       0x0
#define CS4224_GLOBAL_UCODE_VERSION_KR            0x5
#define CS4224_GLOBAL_UCODE_VERSION_KR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_ZR            0x6
#define CS4224_GLOBAL_UCODE_VERSION_ZR_dft        0x0
#define CS4224_GLOBAL_UCODE_VERSION_FC            0x7
#define CS4224_GLOBAL_UCODE_VERSION_FC_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP0            0x8
#define CS4224_GLOBAL_UCODE_TIMESTAMP0_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP1            0x9
#define CS4224_GLOBAL_UCODE_TIMESTAMP1_dft        0x0
#define CS4224_GLOBAL_UCODE_TIMESTAMP2            0xA
#define CS4224_GLOBAL_UCODE_TIMESTAMP2_dft        0x0
#define CS4224_GLOBAL_MPIF_SOFT_RESET             0xD

#define CS4224_GLOBAL_MDIO_CONFIG                 0xF

#define CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL         0x11
#define CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL_dft     0x0
#define CS4224_GLOBAL_DWNLD_CHECKSUM_STATUS       0x12
#define CS4224_GLOBAL_DWNLD_CHECKSUM_STATUS_dft   0x0
#define CS4224_GLOBAL_DWNLD_CHECKSUM_HW           0x13
#define CS4224_GLOBAL_DWNLD_CHECKSUM_HW_dft       0xFFFF
#define CS4224_GLOBAL_DWNLD_CHECKSUM_SW           0x14
#define CS4224_GLOBAL_DWNLD_CHECKSUM_SW_dft       0x0
#define CS4224_GLOBAL_PIN_STATUS                  0x16
#define CS4224_GLOBAL_BROADCAST                   0x17

#define CS4224_GLOBAL_SCRATCH0                    0x1D
#define CS4224_GLOBAL_SCRATCH6                    0x23
#define CS4224_GLOBAL_SCRATCH7                    0x24
#define CS4224_EFUSE_GENERAL_STATUS               0x181

#define CS4224_EFUSE_PDF_SKU                      0x19F

#define CS4224_EFUSE_PDF_MON_GAIN_DATA            0x1A5
#define CS4224_EFUSE_PDF_MON_LUT15                0x1A6

#define CS4224_MONITOR_CONTROL0                   0x200
#define CS4224_MONITOR_CONFIG_MASK                0x204
#define CS4224_MONITOR_LUT_SELECT                 0x20B
#define CS4224_MONITOR_LUT_LOCAL_SELECT           0x20C
#define CS4224_MONITOR_LUT_APPLY                  0x20D
#define CS4224_MONITOR_CAL_CONST_OVERRIDE_ENA     0x20E
#define CS4224_MONITOR_CAL_OVERRIDE               0x20F
#define CS4224_MONITOR_CAL_CONST_APPLY            0x210

#define CS4224_MONITOR_STATUS_FINAL0              0x25A
#define CS4224_MONITOR_STATUS_FINAL2              0x25C
#define CS4224_MONITOR_STATUS_FINAL6              0x260
#define CS4224_MONITOR_LUT_RANGE0                 0x27B
#define CS4224_MONITOR_LUT_VALUE0                 0x28B

#define CS4224_PP_LINE_LINEMISC_SOFT_RESET        0x1000
#define CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG 0x1001

#define CS4224_PP_LINE_LINEMISC_CLKEN             0x1004
#define CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL       0x1007

#define CS4224_PP_LINE_LINEMISC_DATAPATH_CTRL     0x100B
#define CS4224_PP_LINE_LINEMISC_DATAPATH_CTRL_dft 0xF180

#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_CPA     0x1027
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_SPARE   0x102F
#define CS4224_PP_LINE_SDS_COMMON_SRX0_RX_SPARE_dft 0xE0F0

#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL00 0x1047
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL01 0x1048
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL02 0x1049
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL03 0x104A
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL10 0x104B
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL11 0x104C
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL12 0x104D
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_BIASVAL1	0x104E
#define CS4224_PP_LINE_SDS_COMMON_RXVCO0_LC_VCOBIAS 0x104F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA	0x108F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB	0x1090

#define CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH    0x1091


#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_SHADOW 0x1224
#define CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS       0x1225
#define CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_LSB 0x1235
#define CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_PHSEL  0x1265

#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_LSB		0x1280
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE0_MSB		0x1281
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_LSB		0x1282
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE1_MSB		0x1283
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_LSB		0x1284
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE2_MSB		0x1285
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_LSB		0x1286
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE3_MSB		0x1287
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_LSB		0x1288
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE4_MSB		0x1289
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_LSB		0x128A
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE5_MSB		0x128B
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_LSB		0x128C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE6_MSB		0x128D
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_LSB		0x128E
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE7_MSB		0x128F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_LSB		0x1290
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE8_MSB		0x1291
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_LSB		0x1292
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE9_MSB		0x1293
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_LSB		0x1294
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE10_MSB		0x1295
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_LSB		0x1296
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE11_MSB		0x1297
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_LSB		0x1298
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE12_MSB		0x1299
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_LSB		0x129A
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE13_MSB		0x129B
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE14_LSB		0x129C
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE14_MSB		0x129D
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE15_LSB		0x129E
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE15_MSB		0x129F
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE16_LSB		0x12A0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE16_MSB		0x12A1
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE17_LSB		0x12A2
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE17_MSB		0x12A3
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_LSB		0x12A4
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE18_MSB		0x12A5
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE19_LSB		0x12A6
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE19_MSB		0x12A7
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_LSB		0x12A8
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE20_MSB		0x12A9
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_LSB		0x12AA
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE21_MSB		0x12AB
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB		0x12AC
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB		0x12AD
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_LSB		0x12AE
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE23_MSB		0x12AF
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB		0x12B0
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_MSB		0x12B1
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_LSB		0x12B2
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE25_MSB		0x12B3
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_LSB		0x12B4
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_MSB		0x12B5
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_LSB		0x12B6
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE27_MSB		0x12B7
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_LSB		0x12B8
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE28_MSB		0x12B9

#define CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL       0x1807
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLA	0x188F
#define CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLB	0x1890
#define CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS       0x1A25
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE0_LSB		0x1A80
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE0_MSB		0x1A81
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE1_LSB		0x1A82
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE1_MSB		0x1A83
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE2_LSB		0x1A84
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE2_MSB		0x1A85
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE3_LSB		0x1A86
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE3_MSB		0x1A87
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE4_LSB		0x1A88
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE4_MSB		0x1A89
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE5_LSB		0x1A8A
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE5_MSB		0x1A8B
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE6_LSB		0x1A8C
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE6_MSB		0x1A8D
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE7_LSB		0x1A8E
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE7_MSB		0x1A8F
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE8_LSB		0x1A90
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE8_MSB		0x1A91
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE9_LSB		0x1A92
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE9_MSB		0x1A93
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE10_LSB		0x1A94
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE10_MSB		0x1A95
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE11_LSB		0x1A96
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE11_MSB		0x1A97
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_LSB		0x1A98
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE12_MSB		0x1A99
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE13_LSB		0x1A9A
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE13_MSB		0x1A9B
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE14_LSB		0x1A9C
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE14_MSB		0x1A9D
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE15_LSB		0x1A9E
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE15_MSB		0x1A9F
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE16_LSB		0x1AA0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE16_MSB		0x1AA1
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE17_LSB		0x1AA2
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE17_MSB		0x1AA3
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE18_LSB		0x1AA4
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE18_MSB		0x1AA5
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE19_LSB		0x1AA6
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE19_MSB		0x1AA7
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE20_LSB		0x1AA8
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE20_MSB		0x1AA9
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE21_LSB		0x1AAA
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE21_MSB		0x1AAB
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB		0x1AAC
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB		0x1AAD
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE23_LSB		0x1AAE
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE23_MSB		0x1AAF
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB		0x1AB0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_MSB		0x1AB1
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_LSB		0x1AB2
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE25_MSB		0x1AB3
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE26_LSB		0x1AB4
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE26_MSB		0x1AB5
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE27_LSB		0x1AB6
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE27_MSB		0x1AB7
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE28_LSB		0x1AB8
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE28_MSB		0x1AB9

#define CS4224_EEPROM_LOADER_CONTROL              0x5000
#define CS4224_EEPROM_LOADER_STATUS               0x5001
#define CS4224_EEPROM_LOADER_ERROR_STATUS         0x5002
#define CS4224_EEPROM_LOADER_VERIFY_RESULT        0x5003
#define CS4224_EEPROM_UNSTICKER_CONTROL           0x5004
#define CS4224_EEPROM_MICRO_ACCESS_CONTROL        0x5005
#define CS4224_EEPROM_MICRO_ACCESS_STATUS         0x5006
#define CS4224_EEPROM_MICRO_ACCESS_ADDRESS        0x5007
#define CS4224_EEPROM_MICRO_ACCESS_READ_DATA      0x5008
#define CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA     0x5009
#define CS4224_EEPROM_EXCESSIVE_COLLISION_THRESHOLD 0x500b
#define CS4224_EEPROM_TOTAL_COLLISION_COUNT       0x500c

/**
 * Obtains the ASIC SKU hardware ID
 *
 * @param phydev	phy device to get the SKU from
 *
 * @return	The ASIC SKU number
 */
enum cortina_hw_id cs4224_get_hw_id(struct phy_device *phydev)
{
	uint16_t sku;
	struct cortina_phy_info *cinfo = phydev->priv;

	if (cinfo->hw_id != CORTINA_CS4224_HW_UNDEF)
		return cinfo->hw_id;

	sku = phy_read(phydev, 0, CS4224_EFUSE_PDF_SKU);
	sku &= 0x0017;

	/* If the EFUSE is not programmed then default to CS4343.
	 * Bit 5 will be set to 1 if the EFUSE is programmed.  If it is not
	 * programmed it will be 0.
	 */
	if ((sku & 0x10) != 0x10) {
		debug("%s(%s): ASIC SKU not programmed\n",
		      __func__, phydev->dev->name);
		return CORTINA_CS4224_HW_UNDEF;
		}

	cinfo->hw_id = sku;

	return sku;
}

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
			  enum cortina_slice_mode new_mode)
{
	return 0;
}
int board_cs4224_set_mode(struct phy_device *phydev,
			  enum cortina_slice_mode old_mode,
			  enum cortina_slice_mode new_mode)
	__attribute__((weak));

/**
 * Return the mod_abs pin status for the specified phy.
 *
 * @param	phydev	phy device to get mod_abs pin status
 *
 * @return	0 if module is present, 1 if absent, -1 on error.
 */
int board_cs4224_get_mod_abs(struct phy_device *phydev) __attribute__((weak));

int board_cs4224_get_mod_abs(struct phy_device *phydev)
{
	return 0;
}

static int cs4224_set_mode_slice(struct phy_device *phydev,
				 enum cortina_slice_mode mode,
				 int sub_addr, int off)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	uint16_t val;

	debug("%s(%s, %s, %d, 0x%x)\n", __func__, phydev->dev->name,
	      mode_to_str[mode], sub_addr, off);

	switch (pinfo->mode) {
	case CORTINA_SLICE_SR: {
		switch (mode) {
		case CORTINA_SLICE_SR:
			debug("%s: Changing mode from SR to SR\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			break;
		case CORTINA_SLICE_CX:
			debug("%s: Changing mode from SR to CX\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			break;
		case CORTINA_SLICE_1000X:
			debug("%s: Changing from SR to 1000Base-X\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			/* use 0x0005 for copper, 0x0009 for optical */
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			/* use 0x0005 for copper, 0x0009 for optical */
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			break;
		default:
			printf("%s: Error: Unsupported mode %d\n",
			       __func__, mode);
			break;
		}
			break;
	}
	case CORTINA_SLICE_CX: {
		switch (mode) {
		case CORTINA_SLICE_SR:
			debug("%s: Changing from CX to SR\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			break;
		case CORTINA_SLICE_CX:
			debug("%s: Changing from CX to CX\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			break;
		case CORTINA_SLICE_1000X:
			debug("%s: Changing from CX to 1000Base-X\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			/* use 0x0005 for copper, 0x0009 for optical */
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			/* use 0x0005 for copper, 0x0009 for optical */
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: Read Line 0x%04x from 0x%x\n", __func__, val,
			      off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: Read Host 0x%04x from 0x%x\n", __func__, val,
			      off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			break;
	default:
			printf("%s: Error: Unsupported mode %d\n",
			       __func__, mode);
			break;
	}
		break;
	}
	case CORTINA_SLICE_1000X:
		switch (mode) {
		case CORTINA_SLICE_SR:
			debug("%s: Changing from 1000Base-X to SR\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			break;
		case CORTINA_SLICE_CX:
			debug("%s Changing from 1000Base-X to CX\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			break;
		case CORTINA_SLICE_1000X:
			debug("%s: Changing from 1000Base-X to 1000Base-X\n",
			      __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(10);
			break;
		default:
			printf("%s: Error: Unsupported mode %d\n",
			       __func__, mode);
			break;
		}
	case CORTINA_SLICE_UNKNOWN:
		switch (mode) {
		case CORTINA_SLICE_SR:
			debug("%s: Changing from Unknown to SR\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0009);
			mdelay(10);
			break;
		case CORTINA_SLICE_CX:
			debug("%s Changing from Unknown to CX\n", __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0005);
			mdelay(10);
			break;
		case CORTINA_SLICE_1000X:
			debug("%s: Changing from Unknown to 1000Base-X\n",
			      __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: val: 0x%x\n", __func__, val);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: val: 0x%x\n", __func__, val);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(20);
			debug("%s: Changing from Unknown to 1000Base-X again\n",
			      __func__);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: val: 0x%x\n", __func__, val);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			val = phy_read(phydev, 0,
				       off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB);
			debug("%s: val: 0x%x\n", __func__, val);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  val);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB,
				  0x8000);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB,
				  0x0009);
			mdelay(20);
			phy_write(phydev, 0,
				  off + CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB,
				  0x0000);
			mdelay(20);

			break;
		default:
			printf("%s: Error: Unsupported mode %d\n",
			       __func__, mode);
			break;
		}
	}
	debug("%s(%s): 0x1023: 0x%x\n", __func__, phydev->dev->name,
	      phy_read(phydev, 0, 0x1023));

	debug("%s: ctrla: 0x%x, ctrlb: 0x%x\n", __func__,
	      pinfo->tx_output_ctrla[sub_addr][mode],
	      pinfo->tx_output_ctrlb[sub_addr][mode]);
	phy_write(phydev, 0,
		  off + CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA,
		  pinfo->tx_output_ctrla[sub_addr][mode]);
	phy_write(phydev, 0,
		  off + CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB,
		  pinfo->tx_output_ctrlb[sub_addr][mode]);
	debug("%s(%s): 0x1023: 0x%x\n", __func__, phydev->dev->name,
	      phy_read(phydev, 0, 0x1023));

	return 0;
}

/**
 * Changes the mode of a CS4224 Cortina PHY between CX, SR and 1000Base-X modes
 *
 * @param	phydev	phydev to modify
 * @param	mode	New mode for phy
 *
 * @return	0 for success
 */
int cs4224_set_mode(struct phy_device *phydev, enum cortina_slice_mode mode)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	int off = pinfo->slice_offsets[pinfo->sub_addr];
	enum cortina_slice_mode old_mode;
	int rc;
	int i;

	debug("%s(%s: 0x%x.0x%x, %s) offset: %x\n", __func__, phydev->dev->name,
	      phydev->addr, pinfo->sub_addr, mode_to_str[mode], off);
	old_mode = pinfo->mode;
	if (mode == old_mode) {
		debug("%s(%s, %s): not changing mode\n", __func__,
		      phydev->dev->name, mode_to_str[mode]);
		return 0;
	}

	if (pinfo->quad_mode) {
		debug("%s: quad mode, setting all slices\n", __func__);
		rc = 0;
		for (i = 0; i < pinfo->max_slices; i++)
			rc |= cs4224_set_mode_slice(phydev, mode, i,
						    pinfo->slice_offsets[i]);
	} else {
		rc = cs4224_set_mode_slice(phydev, mode, pinfo->sub_addr, off);
		}

	if (rc) {
		printf("%s(%s, %s): Error setting slice mode\n", __func__,
		       phydev->dev->name, mode_to_str[mode]);
		return rc;
	}
	pinfo->mode = mode;
	mdelay(10);
	return board_cs4224_set_mode(phydev, old_mode, mode);
}

/**
 * Uses the sfp info data to return the proper mode
 *
 * @param[in]	sfp_info	Information on the SFP module plugged in
 *
 * @return	Mode the Cortina PHY should be configured for.  If unknown
 *		it defaults to CX mode.
 */
enum cortina_slice_mode
cs4224_get_mode_from_sfp(const struct phy_sfp_info *sfp_info)
{
	if (sfp_info->rate >= SFP_RATE_10G) {
		if (sfp_info->limiting) {
			return CORTINA_SLICE_SR;
	} else {
			return CORTINA_SLICE_CX;
		}
	} else if (sfp_info->rate == SFP_RATE_1G) {
		return CORTINA_SLICE_1000X;
	} else {
		return CORTINA_SLICE_CX;
	}
}

/**
 * Board specific funtion to set the PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA
 * and PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB.
 *
 * @param	phydev	PHY device
 * @param	slice	phy slice number
 * @param	mode	PHY mode to configure values for
 * @param[out]	ctrla	Value for PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA
 * @param[out]	ctrlb	Value for PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB
 */
int board_cs4224_get_ctrl(struct phy_device *phydev,
			  int slice,
			  enum cortina_slice_mode mode,
			  uint16_t *ctrla, uint16_t *ctrlb)
	__attribute__((weak));

int board_cs4224_get_ctrl(struct phy_device *phydev,
			  int slice,
			  enum cortina_slice_mode mode,
			  uint16_t *ctrla, uint16_t *ctrlb)
{
	return 0;
}

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
int cs4224_register_sfp(struct phy_device *phydev, int i2c_bus, int i2c_addr)
{
	struct cortina_phy_info *pinfo = phydev->priv;

	debug("%s(%s, 0x%x, 0x%x)\n", __func__, phydev->dev->name,
	      i2c_bus, i2c_addr);
	pinfo->sfp_i2c_bus = i2c_bus;
	pinfo->sfp_i2c_addr = i2c_addr;

	return 0;
}
#endif

/**
 * Checks which SFP module is plugged in and configures the PHY to the proper
 * mode.
 *
 * @param	phydev	phy device to check
 *
 * @return	0 if mode set properly, -1 if sfp not read or if parsing error
 */
int cs4224_port_check(struct phy_device *phydev)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	struct phy_sfp_info sfp_info;
	uint8_t buffer[SFP_MAX_EEPROM_SIZE];
	int rc;
	int i;
	enum cortina_slice_mode mode;

	debug("%s: Checking port %s, i2c %x:%2x\n", __func__, phydev->dev->name,
	      pinfo->sfp_i2c_bus, pinfo->sfp_i2c_addr);
	rc = board_cs4224_get_mod_abs(phydev);
	if (rc) {
		if (rc > 0)
			debug("  module is absent\n");
		else
			debug("  error reading mod_abs pin\n");
		return -1;
	}
#ifdef CONFIG_PHY_SFP
	if (pinfo->sfp_i2c_bus < 0 || pinfo->sfp_i2c_addr <= 0) {
		printf("%s(%s): Error: SFP I2C bus and/or address undefined\n",
		       __func__, phydev->dev->name);
		return -1;
	}

	rc = phy_sfp_read_i2c_eeprom(buffer, pinfo->sfp_i2c_bus,
				     pinfo->sfp_i2c_addr);
	if (rc)
		return -1;
	rc = phy_sfp_parse_eeprom(&sfp_info, buffer);
	if (rc) {
		printf("Invalid SFP EEPROM detected for %s (%02x:%02x)\n",
		       phydev->dev->name,
		       pinfo->sfp_i2c_bus, pinfo->sfp_i2c_addr);
		return -1;
	}
	mode = cs4224_get_mode_from_sfp(&sfp_info);
	debug("%s: SFP mode: %s, old mode: %s\n", __func__, mode_to_str[mode],
	      mode_to_str[pinfo->mode]);
	if (mode != pinfo->mode) {
		if (pinfo->quad_mode) {
			for (i = 0; i < pinfo->max_slices; i++)
				board_cs4224_get_ctrl(phydev, i,
						      mode,
						      &pinfo->tx_output_ctrla[i][mode],
						      &pinfo->tx_output_ctrlb[i][mode]);
		} else {
			board_cs4224_get_ctrl(phydev, pinfo->sub_addr,
					      mode,
					      &pinfo->tx_output_ctrla[pinfo->sub_addr][mode],
					      &pinfo->tx_output_ctrlb[pinfo->sub_addr][mode]);

		}
	}
#endif
	debug("%s: mode: %d, pinfo->mode: %d\n", __func__, mode, pinfo->mode);
	/* if (mode != pinfo->mode) */
		cs4224_set_mode(phydev, mode);

	return 0;
}

static bool cs4224_is_eeprom_finished(struct mii_dev *bus, int addr)
{
	uint16_t efuse_general_status;
	uint16_t global_pin_status;
	uint16_t eeprom_loader_status;

	efuse_general_status = bus->read(bus, addr, 0,
					 CS4224_EFUSE_GENERAL_STATUS);
	if (efuse_general_status != 1)
		return false;

	global_pin_status = bus->read(bus, addr, 0, CS4224_GLOBAL_PIN_STATUS);
	if (global_pin_status == 0xf00d || global_pin_status == 0xffff)
		return false;
	else if ((global_pin_status & 0x4000) == 0)
		/* EEPROM not present */
		return true;

	eeprom_loader_status = bus->read(bus, addr, 0,
					 CS4224_EEPROM_LOADER_STATUS);
	if ((eeprom_loader_status & 0x0007) != 0)
		/* EEPROM load abort, load failed, or load done */
		return true;
	return false;
}

static int cs4224_enable_monitor_sense_points(struct phy_device *phydev, bool wait)
{
	uint16_t val;
	int i;
	uint16_t last = 0;

	val = phy_read(phydev, 0, CS4224_MONITOR_CONTROL0);
	if (val != 0x630f) {
		phy_write(phydev, 0, CS4224_MONITOR_CONFIG_MASK, 0x45);
		phy_write(phydev, 0, CS4224_MONITOR_CONTROL0, 0x630f);

		if (wait) {
			val = phy_read(phydev, 0, CS4224_MONITOR_STATUS_FINAL6);
			for (i = 0; i < 100; i++) {
				mdelay(10);
				if (last > val)
					break;
				last = val;
				val = phy_read(phydev, 0,
					       CS4224_MONITOR_STATUS_FINAL6);
			}
			if (i == 100)
				return -1;
		}
	}
	return 0;
}

static int cs4224_apply_workarounds(struct phy_device *phydev)
{
	uint16_t val;

	val = phy_read(phydev, 0, CS4224_GLOBAL_SCRATCH6);
	if (val & 1)	/* See if already applied */
		return 0;
	val = phy_read(phydev, 0, CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS_SHADOW);
	if (!(val & 8)) {
		printf("ERROR: MSEQs on die %d are not stalled.\n",
		       phydev->addr & 1);
		return -1;
	}
	cs4224_enable_monitor_sense_points(phydev, false);

	phy_write(phydev, 0, CS4224_GLOBAL_MDIO_CONFIG, 0x60);

	return 0;
}

bool cs4223_is_eeprom_finished(struct phy_device *phydev)
{
	uint16_t efuse_general_status;
	uint16_t global_pin_status;
	uint16_t eeprom_loader_status;

	efuse_general_status = phy_read(phydev, 0, CS4224_EFUSE_GENERAL_STATUS);
	if (efuse_general_status != 0x0001) {
		debug("%s: EFUSE has not finished loading\n", __func__);
		return false;
	}

	global_pin_status = phy_read(phydev, 0, CS4224_GLOBAL_PIN_STATUS);
	/* 0xf00d means the EEPROM loader is still running.
	 * 0xffff means the refclk is not stable.
	 */
	if ((global_pin_status == 0xf00d) || (global_pin_status == 0xffff))
		return false;
	else if (!(global_pin_status & 0x4000)) {
		/* EEPROM not present */
		return true;
	}

	eeprom_loader_status = phy_read(phydev, 0, CS4224_EEPROM_LOADER_STATUS);
	if ((eeprom_loader_status & 0x0007) != 0)
		return true;
	return false;
}

int cs4223_wait_for_eeprom_finished(struct phy_device *phydev,
				    uint32_t max_iterations,
				    uint32_t ms_delay)
{
	bool finished = false;
	uint32_t attempt = 0;

	do {
		finished = cs4223_is_eeprom_finished(phydev);
		mdelay(ms_delay ? ms_delay : 1);
	} while (!finished && attempt++ < max_iterations);

	if (finished)
		return 0;
	else
		return -1;
}

static int cs4224_hard_reset(struct phy_device *phydev)
{
	uint16_t val;
	uint32_t stride;
	if (cs4223_wait_for_eeprom_finished(phydev, 2500, 1)) {
		printf("%s: Timeout waiting for EEPROM\n", __func__);
		return -1;
	}

	/* Fix the MDIO_CONFIG register so accesses to MSEQ registers don't
	 * time out.
	 */
	phy_write(phydev, 0, CS4224_GLOBAL_MDIO_CONFIG, 0x60);

	/* Blindly stall all mseqs */
	for (stride = 0; stride < 0x4000; stride += 0x1000) {
		phy_write(phydev, 0,
			  CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL + stride, 0x8004);
		phy_write(phydev, 0,
			  CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS + stride,  0x0008);
		phy_write(phydev, 0,
			  CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL + stride,  0x8004);
		phy_write(phydev, 0,
			  CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS + stride,  0x0008);
	}
	/* Use broadcast to write to all the ports at once, which
	 * can't be used to stall the mseqs.
	 */
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x80);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_CLKEN, 0xffff);
	phy_write(phydev, 0, CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_LSB, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_SOFT_RESET, 0x0001);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG, 0x0007);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_SOFT_RESET, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_CLKEN, 0x0000);
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x0000);
	phy_write(phydev, 0, CS4224_GLOBAL_MPIF_SOFT_RESET, 0xDEAD);
	/* Wait for EEPROM to finish downloading the die */
	cs4223_wait_for_eeprom_finished(phydev, 2500, 1);
	/* Squelch all transmitters out of reset */
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x80);
	phy_write(phydev, 0, CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH, 0x1);
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x0);
	val = phy_read(phydev, 0, CS4224_GLOBAL_SCRATCH6);
	val |= 2;
	phy_write(phydev, 0, CS4224_GLOBAL_SCRATCH6, val);

	return 0;

}

static int cs4223_wait_micro_access_status_done(struct phy_device *phydev,
						unsigned delay_us,
						int count)
{
	uint16_t val;

	do {
		val = phy_read(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_STATUS);
		if (val & 1)
			break;
		udelay(delay_us);
	} while (count-- > 0);
	return (count > 0) ? val : -1;
}

int cs4223_program_flash(struct phy_device *phydev,
			 int argc, char * const argv[])
{
	uint32_t addr, size;
	const char *script;
	const char *end;
	const char *start_ptr;
	int reg, val;
	uint16_t access_status = 0;
	int max_iterations;
	struct phy_device phydev2;
	int byte_count = 0;
	int current_byte;
	uint16_t data = 0;
	int mismatches = 0;
	uint16_t eeprom_addr = 0;
	ulong start_time;
	ulong write_time;

	memcpy(&phydev2, phydev, sizeof(phydev2));
	phydev2.addr ^= 1;

	cs4224_apply_workarounds(phydev);
	cs4224_apply_workarounds(&phydev2);

	if (argc > 1) {
		addr = simple_strtoul(argv[1], NULL, 16);
	} else {
		addr = getenv_ulong("fileaddr", 16,
				    getenv_ulong("loadaddr", 16,
						 CONFIG_SYS_LOAD_ADDR));
	}
	if (argc > 2) {
		size = simple_strtoul(argv[1], NULL, 16);
	} else {
		size = getenv_ulong("filesize", 16, 0);
	}
	printf("Programming the Cortina PHY EEPROM for %s, please wait...\n",
	       phydev->dev->name);
	script = (char *)addr;
	/* Skip comments */
	while (*script == '#' || ((script[0] == '/') && (script[1] == '/'))) {
		while (*script++ != '\n')
			;
	}
	if (strncmp(script, "write ", 6)) {
		printf("Unknown script type\n");
		return -1;
	}
	start_ptr = script;
	if (size)
		end = script + size;
	else
		end = NULL;

	/* Toggle checksum calculator */
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x01);
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x00);
	do {
		while (*script == ' ' || *script == '\n')
			script++;
		/* Skip comments */
		if (*script == '#' || ((script[0] == '/') && (script[1] == '/'))) {
			while (*script++ != '\n')
				;
			continue;
		}

		if (*script == '\0')
			break;
		if (*script == '\r') {
			script++;
			continue;
		}
		if (strncmp(script, "write ", 6)) {
			printf("Error: expected write in string, got \"%c%c%c%c%c%c\"\n",
			       script[0], script[1], script[2], script[3],
			       script[4], script[5]);
			return -1;
		}
		script += 6;
		while (*script && *script == ' ')
			script++;
		reg = simple_strtoul(script, (char **)&script, 16);
		while (*script && *script == ' ')
			script++;
		val = simple_strtoul(script, (char **)&script, 16);
		while (*script && *script == ' ')
			script++;
		if (*script == '\0')
			break;
		if (*script == '\r')
			script++;
		if (*script != '\n') {
			printf("Excepted end of line, got \'%c\' instead.\n",
			       *script);
			return -1;
		}
		script++;
		debug("%s: Writing %s reg: 0x%04x, val: 0x%04x\n", __func__,
		      phydev->dev->name, reg, val);
		switch (reg) {
		case CS4224_EEPROM_MICRO_ACCESS_CONTROL:
			if (val != 2) {
				printf("Unknown value for EEPROM micro access control register: 0x%x\n",
				       val);
				return -1;
			}
			start_time = get_timer(0);
			phy_write(phydev, 0, reg, val);
			val = cs4223_wait_micro_access_status_done(phydev, 0,
								   100);
			if (val < 0) {
				puts("\nERROR: Timed out waiting for an EEPROM i2c write to occur\n");
				return -1;
			}
			/* Now poll for the ACK */
			do {
				phy_write(phydev, 0,
					  CS4224_EEPROM_MICRO_ACCESS_CONTROL, 4);
				val = cs4223_wait_micro_access_status_done(phydev,
									   1, 100);
				write_time = get_timer(start_time);
			} while ((val >= 0) && (val & 2) &&
				 (write_time <= 10));
			if (val < 0) {
				puts("ERROR: Timed out polling for ACK\n");
				return -1;
			} else if (val & 2) {
				printf("\nERROR: Timed out waiting for an EEPROM write to occur, time: %lu, address: 0x%x, value: 0x%x\n",
				       get_timer(start_time), eeprom_addr, val);
			}

			byte_count++;
			if ((byte_count & 0xff) == 0)
				printf("\rProgrammed 0x%x bytes", byte_count);
			break;
		case CS4224_EEPROM_MICRO_ACCESS_ADDRESS:
			eeprom_addr = val;
		case CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA:
		default:
			phy_write(phydev, 0, reg, val);
			break;
		}
	} while (end && (script < end) && (*script != '\0'));
	printf("\nProgrammed %d bytes total\n", byte_count);
	printf("Verifying image...\n");
	script = start_ptr;
	current_byte = 0;
	/* Toggle checksum calculator */
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x01);
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x00);
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x01);
	do {
		while (*script == ' ' || *script == '\n')
			script++;
		/* Skip comments */
		if (*script == '#' || ((script[0] == '/') && (script[1] == '/'))) {
			while (*script++ != '\n')
				;
			continue;
		}

		if (*script == '\0')
			break;
		if (strncmp(script, "write ", 6)) {
			printf("Error: expected write in string, got \"%c%c%c%c%c%c\"\n",
			       script[0], script[1], script[2], script[3],
			       script[4], script[5]);
			return -1;
		}
		script += 6;
		while (*script && *script == ' ')
			script++;
		reg = simple_strtoul(script, (char **)&script, 16);
		while (*script && *script == ' ')
			script++;
		val = simple_strtoul(script, (char **)&script, 16);
		while (*script && *script == ' ')
			script++;
		if (*script == '\0')
			break;
		if (*script != '\n') {
			printf("Excepted end of line, got \'%c\' instead.",
			       *script);
			return -1;
		}
		script++;
		debug("%s: Verifying %s reg: 0x%04x, val: 0x%04x\n", __func__,
		      phydev->dev->name, reg, val);
		switch (reg) {
		case CS4224_EEPROM_MICRO_ACCESS_ADDRESS:
			addr = val;
			phy_write(phydev, 0, reg, val);
			break;
		case CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA:
			/* Save the data we expect to read */
			data = val;
			break;
		case CS4224_EEPROM_MICRO_ACCESS_CONTROL:
			/* Perform a read instead of a write */
			phy_write(phydev, 0,
				  CS4224_EEPROM_MICRO_ACCESS_CONTROL, 1);
			/* Wait for the read to finish */
			max_iterations = 100;
			do {
				access_status = phy_read(phydev,
							 0, CS4224_EEPROM_MICRO_ACCESS_STATUS);
			} while (!(access_status & 1) && (--max_iterations > 0));
			if (max_iterations <= 0) {
				puts("\nERROR: Timed out waiting for an EEPROM write to occur\n");
				return -1;
			}
			/* read the data back */
			val = phy_read(phydev, 0,
				       CS4224_EEPROM_MICRO_ACCESS_READ_DATA);
			/* Compare it */
			if (val != data) {
				printf("Data mismatch at address 0x%x, read 0x%x, expected 0x%x\n",
				       addr, val, data);
				mismatches++;
			}
			current_byte++;
			break;
		default:
			break;
		}
	} while (end && (script < end) && (*script != '\0'));
	phy_write(phydev, 0, CS4224_GLOBAL_DWNLD_CHECKSUM_CTRL, 0x00);
	if (mismatches)
		printf("Error: %d mismatches found\n", mismatches);
	else
		printf("EEPROM verified successfully.\n");
	return CMD_RET_SUCCESS;
}

/**
 * Converts a 2-digit string hex value to a byte.
 *
 * @param	s	String containing 2 hex digits
 *
 * @returns	Byte value of the hex string
 */
static uint8_t str2hex(const char *s)
{
	uint8_t byte = 0;
	int i;
	for (i = 0; i < 2; i++) {
		byte <<= 4;
		if ((*s >= '0') && (*s <= '9'))
			byte |= *s - '0';
		else if ((*s >= 'a') && (*s <= 'f'))
			byte |= *s - 'a' + 0xa;
		else if ((*s >= 'A') && (*s <= 'F'))
			byte |= *s - 'A' + 0xa;
		s++;
	}
	return byte;
}

/**
 * Reads a S-Record
 *
 * @param[in]	str	string to parse
 * @param[out]	addr	Address read from S-Record
 * @param[out]	val	Value read from S-Record
 * @param[out]	start	Set true if this is a start record (S0)
 * @param[out]	end	Set true  if this is an end record (S9)
 *
 * @return	Pointer to next S-Record or NULL if error.
 */
static const char *cs4223_read_srecord(const char *str,
				       uint16_t *addr, uint8_t *val,
				       bool *start, bool *end)
{
	int len;
	uint8_t csum;

	*start = false;
	*end = false;
	if (*str++ != 'S')
		return NULL;
	/* Check for start or end */
	if ((*str == '0') ||(*str == '9')) {
		if (*str == '0')
			*start = true;
		else
			*end = true;
		str++;
		len = str2hex(str);
		csum = len;
		str += 2;
		*addr = str2hex(str);
		csum += *addr;
		*addr <<= 8;
		str += 2;
		*addr |= str2hex(str);
		csum += (*addr & 0xff);
		str += 2;
		csum += str2hex(str);
		if (csum != 0xff) {
			printf("Invalid S-Record %s checksum\n",
			       *end ? "end" : "start");
			return NULL;
		}
		str += 2;
		if (*str == '\r')
			str++;
		if (*str != '\n') {
			printf("Invalid S-Record %s EOL, got 0x%02x instead of 0x%02x\n",
			       *end ? "end" : "start", *str, '\n');
			return NULL;
		}
		str++;
		return str;
	}
	if (*str != '1') {
		printf("Unknown S record type %c\n", *str);
		return NULL;
	}
	str++;
	len = str2hex(str);
	if (len != 4) {
		printf("Unknown length 0x%x\n", len);
		return NULL;
	}
	csum = 4;
	str += 2;
	*addr = str2hex(str);
	csum += *addr;
	str += 2;
	*addr = (*addr << 8) | str2hex(str);
	csum += (*addr & 0xff);
	str += 2;
	*val = str2hex(str);
	csum += *val;
	str += 2;
	csum += str2hex(str);
	if (csum != 0xff) {
		printf("Bad S-Record checksum\n");
		return NULL;
	}
	str += 2;

	while (*str == '\n' || *str == '\r')
		str++;

	if (*str == 'S')
		return str;
	printf("Expected EOL in S-Record, got 0x%x\n", *str);
	return NULL;
}

/**
 * Programs the EEPROM by parsing an S-Record file.
 *
 * @param phydev	PHY device to program EEPROM for
 * @param argc		Number of arguments
 * @param argv		Array of arguments
 *
 * @return 0 for success, -1 on error
 */
int cs4223_program_flash_srec(struct phy_device *phydev,
			      int argc, char * const argv[])
{
	uint32_t addr;
	const char *script;
	const char *start_ptr;
	uint16_t access_status = 0;
	int max_iterations;
	struct phy_device phydev2;
	int byte_count = 0;
	int current_byte;
	int mismatches = 0;
	uint16_t eeprom_addr = 0;
	uint8_t eeprom_val;
	bool start, end;
	ulong start_time;
	ulong write_time;
	int val;

	memcpy(&phydev2, phydev, sizeof(phydev2));
	phydev2.addr ^= 1;

	cs4224_apply_workarounds(phydev);
	cs4224_apply_workarounds(&phydev2);

	if (argc > 1) {
		addr = simple_strtoul(argv[1], NULL, 16);
	} else {
		addr = getenv_ulong("fileaddr", 16,
				    getenv_ulong("loadaddr", 16,
						 CONFIG_SYS_LOAD_ADDR));
	}
	printf("Programming the Cortina PHY EEPROM for %s, please wait...\n",
	       phydev->dev->name);
	script = (char *)addr;
	script = cs4223_read_srecord(script, &eeprom_addr, &eeprom_val,
				     &start, &end);
	if (!script || !start) {
		printf("Invalid S-Record start header\n");
		return -1;
	}
	start_ptr = script;

	do {
		script = cs4223_read_srecord(script, &eeprom_addr, &eeprom_val,
					     &start, &end);
		if (!script) {
			printf("Error reading S-Record\n");
			return -1;
		}
		if (end)
			break;

		if (*script == '\0')
			break;

		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_ADDRESS,
			  eeprom_addr);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA,
			  eeprom_val);


		debug("%s: Writing %s reg: 0x%04x, val: 0x%04x\n", __func__,
		      phydev->dev->name, eeprom_addr, eeprom_val);
		start_time = get_timer(0);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_CONTROL, 2);
		val = cs4223_wait_micro_access_status_done(phydev, 0, 100);
		if (val < 0) {
			puts("\nERROR: Timed out waiting for an EEPROM i2c write to occur\n");
			return -1;
		}
		/* Now poll for the ACK */
		do {
			phy_write(phydev, 0,
				  CS4224_EEPROM_MICRO_ACCESS_CONTROL, 4);
			val = cs4223_wait_micro_access_status_done(phydev,
								   1, 100);
			write_time = get_timer(start_time);
		} while ((val >= 0) && (val & 2) &&
			 (write_time <= 10));
		if (val < 0) {
			puts("ERROR: Timed out polling for ACK\n");
			return -1;
		} else if (val & 2) {
			printf("\nERROR: Timed out waiting for an EEPROM write to occur, time: %lu, address: 0x%x, value: 0x%x\n",
			       get_timer(start_time), eeprom_addr, val);
		}

		byte_count++;
		if ((byte_count & 0xff) == 0)
			printf("\rProgrammed 0x%x bytes", byte_count);
	} while (!end && (*script != '\0'));
	printf("\nProgrammed %d bytes total\n", byte_count);
	printf("Verifying image...\n");
	script = start_ptr;
	current_byte = 0;

	script = start_ptr;

	do {
		script = cs4223_read_srecord(script, &eeprom_addr, &eeprom_val,
					     &start, &end);
		if (!script) {
			printf("Error reading S-Record\n");
			return -1;
		}
		if (end)
			break;

		if (*script == '\0')
			break;

		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_ADDRESS,
			  eeprom_addr);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_CONTROL, 1);
		debug("%s: Verifying %s reg: 0x%04x, val: 0x%04x\n", __func__,
		      phydev->dev->name, eeprom_addr, eeprom_val);

		/* Wait for the read to finish */
		max_iterations = 100;
		do {
			access_status = phy_read(phydev,
						 0, CS4224_EEPROM_MICRO_ACCESS_STATUS);
		} while (!(access_status & 1) && (--max_iterations > 0));
		if (max_iterations <= 0) {
			puts("\nERROR: Timed out waiting for an EEPROM read to occur\n");
			return -1;
		}
		/* read the data back */
		val = phy_read(phydev, 0,
			       CS4224_EEPROM_MICRO_ACCESS_READ_DATA);
		/* Compare it */
		if (val != eeprom_val) {
			printf("Data mismatch at address 0x%x, read 0x%x, expected 0x%x\n",
			       addr, val, eeprom_val);
			mismatches++;
		}
		current_byte++;
	} while (!end && (*script != '\0'));
	if (mismatches)
		printf("Error: %d mismatches found\n", mismatches);
	else
		printf("EEPROM verified successfully.\n");
	return CMD_RET_SUCCESS;
}

/**
 * Programs the EEPROM by parsing an S-Record file.
 *
 * @param phydev	PHY device to program EEPROM for
 * @param argc		Number of arguments
 * @param argv		Array of arguments
 *
 * @return CMD_RET_SUCCESS for success, CMD_RET_FAILURE on error
 */
int cs4223_program_flash_bin(struct phy_device *phydev, int argc,
			     char * const argv[])
{
	uint32_t addr;
	const uint8_t *image_ptr;
	size_t image_size;
	int val;
	uint16_t access_status = 0;
	struct phy_device phydev2;
	int byte_count = 0;
	int mismatches = 0;
	uint16_t eeprom_addr = 0;
	uint8_t eeprom_val;
	ulong start_time;
	ulong write_time;
	int max_iterations;

	memcpy(&phydev2, phydev, sizeof(phydev2));
	phydev2.addr ^= 1;

	cs4224_apply_workarounds(phydev);
	cs4224_apply_workarounds(&phydev2);

	if (argc > 1) {
		addr = simple_strtoul(argv[1], NULL, 16);
	} else {
		addr = getenv_ulong("fileaddr", 16,
				    getenv_ulong("loadaddr", 16,
						 CONFIG_SYS_LOAD_ADDR));
	}
	if (argc > 2)
		image_size = simple_strtoul(argv[2], NULL, 16);
	else
		image_size = getenv_ulong("filesize", 16, 0);

	if (image_size == 0) {
		puts("Error: image size unknown\n");
		return CMD_RET_FAILURE;
	}

	printf("Programming the Cortina PHY EEPROM for %s, please wait...\n",
	       phydev->dev->name);

	image_ptr = (const uint8_t *)addr;
	do {
		eeprom_val = *image_ptr++;
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_ADDRESS,
			  eeprom_addr++);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA,
			  eeprom_val);
		start_time = get_timer(0);
		debug("%s: Writing %s reg: 0x%04x, val: 0x%04x\n", __func__,
		      phydev->dev->name, eeprom_addr, eeprom_val);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_CONTROL, 2);
		val = cs4223_wait_micro_access_status_done(phydev, 0, 100);
		if (val < 0) {
			puts("\nERROR: Timed out waiting for an EEPROM i2c write to occur\n");
			return CMD_RET_FAILURE;
		}
		/* Now poll for the ACK */
		do {
			phy_write(phydev, 0,
				  CS4224_EEPROM_MICRO_ACCESS_CONTROL, 4);
			val = cs4223_wait_micro_access_status_done(phydev,
								   1, 100);
			write_time = get_timer(start_time);
		} while ((val >= 0) && (val & 2) &&
		(write_time <= 10));
		if (val < 0) {
			puts("ERROR: Timed out polling for ACK\n");
			return CMD_RET_FAILURE;
		} else if (val & 2) {
			printf("\nERROR: Timed out waiting for an EEPROM write to occur, time: %lu, address: 0x%x, value: 0x%x\n",
			       get_timer(start_time), eeprom_addr, val);
		}
		if ((byte_count & 0xff) == 0)
			printf("\rProgrammed 0x%x bytes", byte_count);
		byte_count++;
	} while (byte_count < image_size);
	printf("\nProgrammed %d bytes total\nVerifying image...\n", byte_count);
	eeprom_addr = 0;
	byte_count = 0;
	image_ptr = (const uint8_t *)addr;

	do {
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_ADDRESS,
			  eeprom_addr);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_CONTROL, 1);
		/* Wait for the read to finish */
		max_iterations = 100;
		do {
			access_status = phy_read(phydev, 0,
						 CS4224_EEPROM_MICRO_ACCESS_STATUS);
		} while (!(access_status & 1) && (--max_iterations > 0));
		if (max_iterations <= 0) {
			puts("\nERROR: Timed out waiting for an EEPROM read to occur\n");
			return CMD_RET_FAILURE;
		}
		/* read the data back */
		val = phy_read(phydev, 0,
			       CS4224_EEPROM_MICRO_ACCESS_READ_DATA);
		if (val != *image_ptr) {
			printf("Data mismatch at address 0x%x, read 0x%x, expected 0x%x\n",
			       eeprom_addr, val, *image_ptr);
			mismatches++;
		}
		image_ptr++;
		eeprom_addr++;
		byte_count++;
	} while (byte_count < image_size);
	if (mismatches)
		printf("Error: %d mismatches found\n", mismatches);
	else
		puts("EEPROM verified successfully.\n");
	return mismatches ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}

/**
 * Erases the EEPROM for the specified PHY
 *
 * @param phydev	PHY for EEPROM to erase
 *
 * @return 0 for success, -1 on error
 */
int cs4223_erase(struct phy_device *phydev)
{
	int i;
	int status;
	int max_iterations;
	ulong start;

	puts("\n");
	phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_WRITE_DATA, 0xff);
	for (i = 0; i < 0x10000; i++) {
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_ADDRESS, i);
		start = get_timer(0);
		phy_write(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_CONTROL, 2);
		max_iterations = 100;
		do {
			status = phy_read(phydev, 0, CS4224_EEPROM_MICRO_ACCESS_STATUS);
		} while (((status & 1) == 0) && --max_iterations > 0);
		if (max_iterations <= 0) {
			printf("Timeout waiting for I2C write\n");
			return -1;
		}
		/* Wait for write to complete */
		do {
			phy_write(phydev, 0,
				  CS4224_EEPROM_MICRO_ACCESS_CONTROL, 4);
			status = cs4223_wait_micro_access_status_done(phydev,
								      1, 100);
		} while ((status >= 0) && (status & 2) && (get_timer(start) < 10));
		if ((status < 0) || (status & 2)) {
			printf("\nError erasing address %d\n", i);
			return -1;
		}
		if ((i & 0xff) == 0)
			printf("\rErased offset 0x%04x", i);
		if (ctrlc()) {
			printf("\nInterrupted by user\n");
			return -1;
		}
	}
	puts("\nDone.\n");
	return 0;
}

/**
 * Performs command for the cs422x PHY
 */
enum command_ret_t do_cs4223(cmd_tbl_t *cmdtp, int flag, int argc,
			     char * const argv[])
{
	struct phy_device *phydev;
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;

	if (argc < 3)
		return CMD_RET_USAGE;

	ethdev = eth_get_dev_by_name(argv[2]);
	if (!ethdev) {
		printf("Could not find %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}
	ethinfo = ethdev->priv;
	phydev = ethinfo->phydev;
	if (!phydev) {
		printf("No phy associated with %s\n", ethdev->name);
		return CMD_RET_FAILURE;
	}

	if (!strcmp(argv[1], "programscr")) {
		if ((cs4223_program_flash(phydev, argc - 2, argv + 2)))
			return CMD_RET_FAILURE;
		else
			return CMD_RET_SUCCESS;
	} else if (!strcmp(argv[1], "programsrec")) {
		if ((cs4223_program_flash_srec(phydev, argc - 2, argv + 2)))
			return CMD_RET_FAILURE;
		else
			return CMD_RET_SUCCESS;
	} else if (!strcmp(argv[1], "program")) {
		if ((cs4223_program_flash_bin(phydev, argc - 2, argv + 2)))
			return CMD_RET_FAILURE;
		else
			return CMD_RET_SUCCESS;
	} else if (!strcmp(argv[1], "erase")) {
		if (cs4223_erase(phydev))
			return CMD_RET_FAILURE;
		else
			return CMD_RET_SUCCESS;
	} else {
		return CMD_RET_USAGE;
	}
}

U_BOOT_CMD(cs4223, 5, 1, do_cs4223,
	   "Cortina CS422X PHY system",
	   "program [eth device] [address] [size] - Program the PHY EEPROM using the binary image at the address provided\n"
	   "programscr [eth device] [address] [size] - Program the PHY EEPROM using the script at the address provided\n"
	   "programsrec [eth device] [address] - Program the PHY EEPROM using S-Records\n"
	   "erase [eth device] - Erases the EEPROM");
#endif /* CONFIG_PHY_CORTINA_CS4223 */
