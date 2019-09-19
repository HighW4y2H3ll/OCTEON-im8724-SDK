/*
 *    Based on code from Cortina Systems, Inc.
 *
 *    Copyright (C) 2011, 2012 by Cortina Systems, Inc.
 *    Copyright (C) 2011 - 2014 Cavium, Inc.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 */
# include <common.h>
# include <command.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/cvmx-mdio.h>
# include <errno.h>
# include <malloc.h>
# include <phy.h>
# include <cortina.h>
# include "cs4224.h"
# include <miiphy.h>

#include "cs4224-regs.h"

struct cs4224_private {
	int slice;
	const uint16_t *slice_off_tbl;
	int max_ports;	/** Maximum number of ports per die */
	int max_dies;	/** Maximum number of dies per device */
	uint16_t sku;	/** Device SKU number */
	bool initialized;
};

#define MII_ADDR_C45			0

#define CS4224_API_VERSION_MAJOR	3
#define CS4224_API_VERSION_MINOR	5
#define CS4224_API_VERSION_UPDATE	44

const uint16_t cs4224_pp_sku_cs4343_offset[8] = {
	0x0000, 0x1000, 0x2000, 0x3000, 0x3000, 0x2000, 0x1000, 0x0000
};
const uint16_t cs4224_mseq_sku_cs4343_offset[8] = {
	0x0000, 0x0000, 0x0100, 0x0100, 0x0100, 0x0100, 0x0000, 0x0000
};


static int debug_phyio = 0;
static const int debug_ledio = 0;

enum phy_state {
	PHY_DOWN = 0,
	PHY_STARTING,
	PHY_READY,
	PHY_PENDING,
	PHY_UP,
	PHY_AN,
	PHY_RUNNING,
	PHY_NOLINK,
	PHY_FORCING,
	PHY_CHANGELINK,
	PHY_HALTED,
	PHY_RESUMING
};


struct cs4224_reg_modify {
	uint16_t reg;
	uint16_t mask_bits;
	uint16_t set_bits;
};

struct cs4224_multi_seq {
	int reg_offset;
	const struct cs4224_reg_modify *seq;
};


uint32_t cs4224_adj_pp(struct phy_device *phydev, uint16_t addr)
{
	struct cs4224_private *p = (struct cs4224_private *)phydev->priv;
	return (addr + cs4224_pp_sku_cs4343_offset[p->slice & 7]);
}

static int cs4224_phy_read_x(struct phy_device *phydev, int off, u16 regnum)
{
	int ret;
	struct phy_device dummy;

	memcpy(&dummy, phydev, sizeof(dummy));
	dummy.addr += off;
	ret = phy_read(&dummy, 0, regnum);
	if (debug_phyio)
		debug("r h: 0x%x, addr: 0x%04x, data: 0x%04x\n",
		      phydev->addr << 8 | off, regnum, ret);
	return ret;
}

static int cs4224_phy_write_x(struct phy_device *phydev, int off, u16 regnum,
			      u16 val)
{
	struct phy_device dummy;

	memcpy(&dummy, phydev, sizeof(dummy));
	dummy.addr += off;
	if (debug_phyio)
		debug("w h: 0x%x, addr: 0x%04x, data: 0x%04x\n",
		      phydev->addr << 8 | off, regnum, val);
	return phy_write(&dummy, 0, regnum, val);
}

static int cs4224_phy_read(struct phy_device *phydev, u16 regnum)
{
	return cs4224_phy_read_x(phydev, 0, regnum);
}

static int cs4224_phy_write(struct phy_device *phydev, u16 regnum, u16 val)
{
	return cs4224_phy_write_x(phydev, 0, regnum, val);
}

static int cs4224_write_seq_x(struct phy_device *phydev, int off,
			      const struct cs4224_reg_modify *seq)
{
	int last_reg = -1;
	int last_val = 0;
	int ret = 0;

	while (seq->reg) {
		if (seq->mask_bits) {
			if (last_reg != seq->reg) {
				ret = cs4224_phy_read_x(phydev, off, seq->reg);
				if (ret < 0)
					goto err;
				last_val = ret;
			}
			last_val &= seq->mask_bits;
		} else {
			last_val = 0;
		}
		last_val |= seq->set_bits;
		ret = cs4224_phy_write_x(phydev, off, seq->reg, last_val);
		if (ret < 0)
			goto err;
		seq++;
	}
err:
	return ret;
}

static int cs4224_write_multi_seq_x(struct phy_device *phydev, int off,
				  const struct cs4224_multi_seq *m)
{
	int ret = 0;

	while (m->seq) {
		debug("// %s(%d, %d, 0x%p)\n", __func__, phydev->addr, off, m->seq);
		ret = cs4224_write_seq_x(phydev, m->reg_offset + off, m->seq);
		if (ret)
			goto err;
		m++;
	}

err:
	return ret;
}

static int cs4224_write_multi_seq(struct phy_device *phydev,
				  const struct cs4224_multi_seq *m)
{
	return cs4224_write_multi_seq_x(phydev, 0, m);
}

static int cs4224_get_hw_revision(struct phy_device *phydev)
{
	static uint32_t revision = 0;

	if (revision == 0) {
		uint16_t data;

		data = cs4224_phy_read(phydev, CS4224_GLOBAL_CHIP_ID_MSB);
		switch (data) {
		case CS4224_ID_MSB_REVA:
			revision = CS4224_HW_REVA;
			break;
		case CS4224_ID_MSB_REVB:
			revision = CS4224_HW_REVB;
			break;
		default:
			revision = -1;
			break;
		}
	}
	return revision;
}

static int cs4224_write_seq(struct phy_device *phydev,
			    const struct cs4224_reg_modify *seq)
{
	return cs4224_write_seq_x(phydev, 0, seq);
}



bool cs4224_is_eeprom_finished(struct phy_device *phydev)
{
	uint16_t efuse_general_status;
	uint16_t global_pin_status;
	uint16_t eeprom_loader_status;

	efuse_general_status = phy_read(phydev, 0, CS4224_EFUSE_GENERAL_STATUS);
	if (efuse_general_status != 0x0001)
		return false;

	global_pin_status = phy_read(phydev, 0, CS4224_GLOBAL_PIN_STATUS);
	if (global_pin_status == 0xf00d || global_pin_status == 0xffff)
		return false;
	else if ((global_pin_status & 0x4000) == 0)
		/* EEPROM not present */
		return true;

	eeprom_loader_status = phy_read(phydev, 0, CS4224_EEPROM_LOADER_STATUS);
	if ((eeprom_loader_status & 0x0007) != 0)
		/* EEPROM load abort, load failed, or load done */
		return true;
	return false;
}

int cs4224_wait_for_eeprom_finished(struct phy_device *phydev,
				    uint32_t max_iterations, uint32_t ms_delay)
{
	int status = 0;
	bool finished;
	int iteration = 0;

	do {
		finished = cs4224_is_eeprom_finished(phydev);
		if (!finished)
			mdelay(max(1, ms_delay));
	} while (iteration++ < max_iterations && !finished);

	if (finished) {
		return 0;
	} else {
		debug("EEPROM never finished loading\n");
		return -1;
	}
}

int cs4224_hard_reset_die(struct phy_device *phydev)
{
	int status = 0;
	uint32_t stride = 0;
	uint16_t data = 0;

	status |= cs4224_wait_for_eeprom_finished(phydev, 2500, 1);
	if (status)
		printf("%s: Error waiting for EEPROM to downlaod\n", __func__);

	/* Fix MDIO_CONFIG register so access to MSEQ registers don't time out */
	phy_write(phydev, CS4224_GLOBAL_MDIO_CONFIG, 0x0060);

	/* Stall all the mseqs */
	for (stride = 0; stride < 0x4000; stride += 0x1000) {
		phy_write(phydev, 0,
			  CS4224_PP_LINE_LINEMISC_MSEQCLKCTRL + stride, 0x8004);
		phy_write(phydev, 0,
			  CS4224_PP_LINE_SDS_DSP_MSEQ_OPTIONS + stride, 0x0008);
		phy_write(phydev, 0,
			  CS4224_PP_HOST_HOSTMISC_MSEQCLKCTRL + stride, 0x8004);
		phy_write(phydev, 0,
			  CS4224_PP_HOST_SDS_DSP_MSEQ_OPTIONS + stride, 0x0008);
	}

	/* Use broadcast to write to all the ports at once, which can't be used
	 * to stall the mseqs
	 */
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x80);

	/* Reset all port-pairs (PP) on the die */
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_CLKEN, 0xffff);
	phy_write(phydev, 0, CS4224_PP_LINE_SDS_DSP_MSEQ_POWER_DOWN_LSB, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_SOFT_RESET, 0x0001);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG, 0x007);
	/* Fix common_tx_sr default */
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_SOFT_RESET, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_MPIF_RESET_DOTREG, 0x0000);
	phy_write(phydev, 0, CS4224_PP_LINE_LINEMISC_CLKEN, 0x0000);

	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x0);

	/* Reset a die of the ASCI.  This write is self clearing */
	phy_write(phydev, 0, CS4224_GLOBAL_MPIF_SOFT_RESET, 0xDEAD);

	/* Wait for the EEPROM to finish downloading to the die */
	status |= cs4224_wait_for_eeprom_finished(phydev, 2500, 1);
	if (status)
		printf("%s: Error waiting for EEPROM to downlaod\n", __func__);

	/* Squelch all transmitters out of reset */
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x80);
	phy_write(phydev, 0, CS4224_PP_LINE_SDS_COMMON_STX0_SQUELCH, 0x1);
	phy_write(phydev, 0, CS4224_GLOBAL_BROADCAST, 0x0);

	if (!status) {
		data = phy_read(phydev, 0, CS4224_GLOBAL_SCRATCH6);
		phy_write(phydev, 0, CS4224_GLOBAL_SCRATCH6, data | 2);
	}
	return status;
}

int cs4224_reset(struct phy_device *phydev)
{
	struct cs4224_private *p = (struct cs4224_private *)phydev->priv;
	struct phy_device other_phy;
	enum cs4224_hw_id id;
	int ret;

	debug("%s: Entry, addr: %d\n", __func__, phydev->addr);

	id = cs4224_hw_id(phydev);
	if (id == CS4224_HW_UNDEV) {
		printf("%s: Unknown Cortina PHY ID\n", __func__);
		return -1;
	}
	ret = cs4224_hard_reset_die(phydev);
	if (ret)
		goto err;

	if (p->max_dies > 1) {
		memcpy(&other_phy, phydev, sizeof(other_phy));
		other_phy.addr ^= 1;
		ret = cs4224_hard_reset_die(&other_phy);
	}
	/* The reset sequence is performed for each of the four phys */
err:
	if (ret)
		printf("%s: Error resetting PHY\n", __func__);
	return ret;
}

int cs4224_slice_enter_operational_state(struct phy_device *phydev)

int cs4224_config_init(struct phy_device *phydev)
{
	int ret;
	struct cs4224_private *p;
	const struct cs4224_multi_seq *init_seq;

	debug("%s: Entry, addr: %d\n", __func__, phydev->addr);

	if (!phydev->priv)
		phydev->priv = calloc(sizeof(*p), 1);

	if (!phydev->priv) {
		printf("%s: Out of memory\n", __func__);
		return -ENOMEM;
	}

	p = (struct cs4224_private *)phydev->priv;
	p->mode = cortina_cs4224_get_host_mode(phydev);
	switch (p->mode) {

	}
	ret = cs4224_reset(phydev);
	if (ret) {
		printf("%s: cs4224_reset returned error\n", __func__);
		goto err;
	}

	p->initialized = true;
err:
	return ret;
}


int cs4224_probe(struct phy_device *phydev)
{
	int ret = 0;
	int id_lsb, id_msb;
	struct cs4224_private *p = (struct cs4224_private *)phydev->priv;

	debug("%s: Entry\n", __func__);
	debug("%s: addr: %d, eth index: %d\n", __func__,
	      phydev->addr, phydev->dev->index);
	debug("%s: bus: 0x%p, drv: 0x%p, dev: 0x%p\n",
	      __func__, phydev->bus, phydev->drv, phydev->dev);
	/*
	 * CS4312 keeps its ID values in non-standard registers, make
	 * sure we are talking to what we think we are.
	 */
	id_lsb = cs4224_phy_read(phydev, CS4224_GLOBAL_CHIP_ID_LSB);
	if (id_lsb < 0) {
		debug("%s: Error reading LSB\n", __func__);
		ret = id_lsb;
		goto err;
	}

	id_msb = cs4224_phy_read(phydev, CS4224_GLOBAL_CHIP_ID_MSB);
	if (id_msb < 0) {
		debug("%s: Error reading MSB\n", __func__);
		ret = id_msb;
		goto err;
	}

	debug("%s: id: %04x:%04x\n", __func__, id_msb, id_lsb);
	if (id_lsb != 0x23E5 || id_msb != 0x1002) {
		debug("%s: Unrecognized lsb 0x%x, msb 0x%x\n",
		      __func__, id_lsb, id_msb);
		ret = -ENODEV;
		goto err;
	}
	if (!p)
		p = (struct cs4224_private *)calloc(sizeof(struct cs4224_private), 1);
	if (!p) {
		ret = -ENOMEM;
		debug("%s: Cannot alloc %u bytes\n",
		      __func__, sizeof(struct cs4224_private));
		goto err;
	}
	p->mode = CS4224_HOST_MODE_UNKNOWN;
	p->fw_loaded = 0;
	phydev->priv = p;

err:
	return ret;
}

int cs4224_config_aneg(struct phy_device *phydev)
{
	return -EINVAL;
}

int cs4224_read_status(struct phy_device *phydev)
{
	int value;

	if (is_10g_interface(phydev->interface)) {
		value = cs4224_phy_read(phydev, CS4224_GPIO_GPIO_INTS);
		phydev->speed = SPEED_10000;
		phydev->duplex = DUPLEX_FULL;
		phydev->link = !!(value & 3);
	} else {
		value = cs4224_phy_read(phydev, CS4224_GIGEPCS_LINE_STATUS);
		phydev->speed = SPEED_1000;
		phydev->duplex = DUPLEX_FULL;
		phydev->link = !!(value & 4);
	}
	return value < 0 ? -1 : 0;
}


struct phy_driver cs4224_driver;

int cortina_cs4224_phy_init(void)
{
	cortina_phy_register(&cs4224_driver);
	return 0;
}

int phy_reset(struct phy_device *phydev)
{
	struct cs4224_private *p = (struct cs4224_private *)phydev->priv;
	if (!p->initialized)
		return 0;

	return cs4224_reset(phydev);
}

int cs4224_config(struct phy_device *phydev)
{
	/* For now assume 10000baseT.  Fill in later */
	if (is_10g_interface(phydev->interface))
		phydev->supported = phydev->advertising =
			 SUPPORTED_10000baseT_Full;
	else
		phydev->supported = phydev->advertising =
			 SUPPORTED_1000baseT_Full |
			 SUPPORTED_Autoneg;
	return 0;
}

int cs4224_startup(struct phy_device *phydev)
{
	int ret;
	phydev->speed = is_10g_interface(phydev->interface) ?
			SPEED_10000 : SPEED_1000;
	phydev->duplex = DUPLEX_FULL;

	debug("%s(%s): Entry\n", __func__, phydev->dev->name);
	ret = cs4224_config_init(phydev);
	if (ret) {
		printf("%s: config init returned %d\n", __func__, ret);
		return ret;
	}
	return cs4224_read_status(phydev);
}

int cs4224_shutdown(struct phy_device *phydev)
{
	/* TODO: shut down the link */
	return 0;
}

int get_phy_id(struct mii_dev *bus, int addr, int devad, uint32_t *phy_id)
{
	int phy_reg;

	phy_reg = bus->read(bus, addr, 0, CS4224_GLOBAL_CHIP_ID_MSB);
	if (phy_reg < 0)
		return -EIO;

	*phy_id = (phy_reg & 0xffff) << 16;

	phy_reg = bus->read(bus, addr, 0, CS4224_GLOBAL_CHIP_ID_LSB);
	if (phy_reg < 0)
		return -EIO;
	*phy_id |= (phy_reg & 0xffff);

	debug("%s: phy_id: 0x%x\n", __func__, *phy_id);

	return 0;
}

struct phy_driver cs4224_driver = {
	.name = "Cortina CS4224",
	.uid = 0x700303e5,
	.mask = 0x0fffffff,	/* Cortina is weird.  chip version is 4 MSBs */
	.features = 0,
	.probe = cs4224_probe,
	.config = cs4224_config,
	.startup = cs4224_startup,
	.shutdown = cs4224_shutdown,
	.reset = NULL
};

struct gpio_addrs {
	u16 cfg;
	u16 output_cfg;
	u16 drive_cfg;
	u16 value;
	u16 toggle;
	u16 delay;
};

static const struct gpio_addrs gpio_regs[17] = {
	{	/* Error interrupt */
		.cfg		= 0,
		.output_cfg	= 0,
		.drive_cfg	= 0,
		.value		= 0,
		.toggle		= 0,
		.delay		= 0
	},
	{
		.cfg		= CS4224_GPIO_GPIO1,
		.output_cfg	= CS4224_GPIO_GPIO1_OUTPUT_CFG,
		.drive_cfg	= CS4224_GPIO_GPIO1_DRIVE,
		.value		= CS4224_GPIO_GPIO1_VALUE,
		.toggle		= CS4224_GPIO_GPIO1_TOGGLE,
		.delay		= CS4224_GPIO_GPIO1_DELAY
	},
	{
		.cfg		= CS4224_GPIO_GPIO2,
		.output_cfg	= CS4224_GPIO_GPIO2_OUTPUT_CFG,
		.drive_cfg	= CS4224_GPIO_GPIO2_DRIVE,
		.value		= CS4224_GPIO_GPIO2_VALUE,
		.toggle		= CS4224_GPIO_GPIO2_TOGGLE,
		.delay		= CS4224_GPIO_GPIO2_DELAY
	},
	{
		.cfg		= CS4224_GPIO_GPIO3,
		.output_cfg	= CS4224_GPIO_GPIO3_OUTPUT_CFG,
		.drive_cfg	= CS4224_GPIO_GPIO3_DRIVE,
		.value		= CS4224_GPIO_GPIO3_VALUE,
		.toggle		= CS4224_GPIO_GPIO3_TOGGLE,
		.delay		= CS4224_GPIO_GPIO3_DELAY
	},
	{
		.cfg		= CS4224_GPIO_GPIO4,
		.output_cfg	= CS4224_GPIO_GPIO4_OUTPUT_CFG,
		.drive_cfg	= CS4224_GPIO_GPIO4_DRIVE,
		.value		= CS4224_GPIO_GPIO4_VALUE,
		.toggle		= CS4224_GPIO_GPIO4_TOGGLE,
		.delay		= CS4224_GPIO_GPIO4_DELAY
	}
};

/**
 * Configures the GPIO pins for a Cortina PHY
 *
 * @param[in]	phydev	Phy device to configure
 * @param[in]	gpio_config	GPIO configuration to use
 *
 * @return 0 for success, -1 on error
 */
int cs4224_configure_gpios(struct phy_device *phydev,
			   const struct cs4224_gpio_config *gpio_config)
{
	int pin;
	u16 reg;
	const struct cs4224_gpio_pin_config *pin_cfg;
	const struct gpio_addrs *gregs;	/** GPIO register addresses */
	struct cs4224_private *priv = (struct cs4224_private *)phydev->priv;
	debug("%s(%s, %p)\n", __func__, phydev->dev->name, gpio_config);

	for (pin = 1; pin < 4; pin++) {
		pin_cfg = &(gpio_config->pins[pin]);
		if (!pin_cfg->pin_config)
			continue;

		if (debug_ledio)
			debug_phyio = 1;
		gregs = &gpio_regs[pin];
		reg = 0;
		debug("%s: GPIO pin %d\n", __func__, pin);
		if (pin_cfg->src_sel > CS4224_GPIO_OUTPUT_SRC_SEL_EXT_DELAY)
			return -1;
		if (pin_cfg->src_fn > CS4224_GPIO_OUTPUT_SYNCPULSE_ING)
			return -1;
		reg = (pin_cfg->src_sel & 7) << 9;
		reg |= pin_cfg->invert_output << 8;
		reg |= pin_cfg->invert_input << 7;
		reg |= pin_cfg->iovddsel_1v0 << 6;
		reg |= pin_cfg->outen_ovr << 5;
		reg |= pin_cfg->outen << 4;
		reg |= pin_cfg->oden << 3;
		reg |= pin_cfg->pulldn << 1;
		reg |= pin_cfg->pullup << 0;
		debug("%s(%s(%d)): GPIO pin %d\n"
		      "  src_sel:    %d\n"
		      "  invert output: %s\n"
		      "  invert input: %s\n"
		      "  io vdd select: %sv\n"
		      "  output enable override: %s\n"
		      "  output enable: %s\n"
		      "  open drain output: %s\n"
		      "  pull-up: %s\n"
		      "  pull-down: %s\n",
		      __func__, phydev->dev->name, phydev->addr, pin,
		      pin_cfg->src_sel,
		      pin_cfg->invert_output ? "yes" : "no",
		      pin_cfg->invert_input ? "yes" : "no",
		      pin_cfg->iovddsel_1v0 ? "1.0" : "2.5/1.8",
		      pin_cfg->outen_ovr ? "yes" : "no",
		      pin_cfg->outen ? "yes" : "no",
		      pin_cfg->oden ? "yes" : "no",
		      pin_cfg->pulldn ? "yes" : "no",
		      pin_cfg->pullup ? "yes" : "no");
		debug("  reg: 0x%x, value: 0x%x\n", gregs->cfg, reg);
		cs4224_phy_write(phydev, gregs->cfg, reg);

		reg = pin_cfg->src_fn & 0x3f;
		debug("  src fn: 0x%x\n", reg);
		cs4224_phy_write(phydev, gregs->output_cfg, reg);
		reg = pin_cfg->value;
		debug("  drive: %d\n", pin_cfg->value);
		cs4224_phy_write(phydev, gregs->drive_cfg, reg);
		reg = pin_cfg->toggle_rate;
		debug("  toggle rate: %dms\n", reg);
		cs4224_phy_write(phydev, gregs->toggle, reg);
		reg = pin_cfg->on_delay;
		debug("  on delay: %dms\n", reg);
		cs4224_phy_write(phydev, gregs->delay, reg);

		debug("  input function: ");
		switch (pin_cfg->in_fn) {
		case CS4224_GPIO_IN_FCN_DISABLED:
			debug("none\n");
			break;
		case CS4224_GPIO_IN_FCN_EXT_LOS:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT1, pin);
			debug("EXT_LOS\n");
			break;
		case CS4224_GPIO_IN_FCN_MOD_ABS:
			priv->mod_abs_gpio_pin = pin;
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT2, pin);
			debug("MOD_ABS\n");
			break;
		case CS4224_GPIO_IN_FCN_TX_FAULT:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT3, pin);
			debug("TX_FAULT\n");
			break;
		case CS4224_GPIO_IN_FCN_EXT_TRIGGER:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT4, pin);
			debug("External trigger\n");
			break;
		case CS4224_GPIO_IN_FCN_GPIO_MUXED_INPUT1:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT5, pin);
			debug("GPIO muxed input 1\n");
			break;
		case CS4224_GPIO_IN_FCN_GPIO_MUXED_INPUT2:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT6, pin);
			debug("GPIO muxed input 2\n");
			break;
		case CS4224_GPIO_IN_FCN_GPIO_MUXED_INPUT3:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT7, pin);
			debug("GPIO muxed input 3\n");
			break;
		case CS4224_GPIO_IN_FCN_GPIO_MUXED_INPUT4:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT8, pin);
			debug("GPIO muxed input 4\n");
			break;
		case CS4224_GPIO_IN_FCN_SFP_SCL:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT9, pin);
			debug("SFP SCL\n");
			break;
		case CS4224_GPIO_IN_FCN_SFP_SDA:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT10, pin);
			debug("SFP SDA\n");
			break;
		case CS4224_GPIO_IN_FCN_SYNCPULSE:
			cs4224_phy_write(phydev, CS4224_GPIO_GPIO_INPUT11, pin);
			break;
		default:
			printf("Invalid GPIO input selection for GPIO pin %d\n",
			       pin);
			break;
		}
		if (debug_ledio)
			debug_phyio = 0;
	}
	return 0;
}

static int cs4224_configure_led_event(struct phy_device *phydev, uint16_t addr,
				      const struct cs4224_led_event_config *event)
{
	uint16_t reg;

	reg = 0;
	reg = event->blink_off_time << 12;
	reg |= event->blink_on_time << 8;
	reg |= event->duty_cycle << 3;
	reg |= event->event_pattern;
	cs4224_phy_write(phydev, addr, reg);
	return 0;
}

/**
 * Configures the LEDs of a revision B Cortina PHY
 *
 * @param[in]	phydev	Phy device to configure
 * @param[in]	led_config	Configuration to apply for LEDs
 *
 * @return 0 for success, -1 for error
 */
int cs4224_configure_leds(struct phy_device *phydev,
			  const struct cs4224_led_config *led_config)
{
	struct phy_device dummy;
	int reg;

	if (debug_ledio) {
		debug("%s(%s, %p) ENTER\n", __func__, phydev->dev->name,
		      led_config);
	}
	memcpy(&dummy, phydev, sizeof(dummy));
	dummy.addr += 1;

	/* LED configuration does not work for revision A */
	if (cs4224_get_hw_revision(phydev) == CS4224_HW_REVA) {
		if (debug_ledio) {
			debug("  PHY rev A, not configuring LEDs\n");
		}
		return -1;
	}

	debug_phyio = debug_ledio;
	cs4224_phy_write(phydev, CS4224_LED_RESET, 1);
	mdelay(10);
	cs4224_phy_write(phydev, CS4224_LED_RESET, 0);
	/* Turn everything off */
	cs4224_phy_write(phydev, CS4224_LED_LED_CTRL, 0);

	/* Disable all LED priorities */
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF0, 0);
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF1, 0);
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF2, 0);
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF3, 0);

	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_LINK_ACTIVE,
				   &led_config->cfg_event_link_active_a);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_LINK_HOST_DIS,
				   &led_config->cfg_event_link_host_disable);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_MODULE_BOOT,
				   &led_config->cfg_event_module_boot);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_BEACON,
				   &led_config->cfg_event_beacon);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_TX_FAULT,
				   &led_config->cfg_event_tx_fault);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_RX_LOS,
				   &led_config->cfg_event_rx_los);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_MOD_ABS,
				   &led_config->cfg_event_mod_abs);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_EDC_CONVERGED,
				   &led_config->cfg_event_edc_converged);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_GEN_GPIO,
				   &led_config->cfg_event_generic_gpio);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_USER_DEF1,
				   &led_config->cfg_event_user_1);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_USER_DEF2,
				   &led_config->cfg_event_user_2);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_USER_DEF3,
				   &led_config->cfg_event_user_3);
	cs4224_configure_led_event(phydev, CS4224_LED_EVENT_CFG_LINK_ACTIVE_B,
				   &led_config->cfg_event_link_active_b);

	reg = led_config->auto_link_active_a.mode & 3;
	reg |= (led_config->auto_link_active_a.link_mask & 0xf) << 4;
	reg |= (led_config->auto_link_active_b.mode & 3) << 8;
	reg |= (led_config->auto_link_active_b.link_mask & 0xf) << 12;
	cs4224_phy_write(phydev, CS4224_LED_LED_AUTO_LINK_ACTIVE_CFG, reg);
	cs4224_phy_write(phydev, CS4224_LED_LED_AUTO_LINK_ACTIVE_WIN_CFG_A,
			 led_config->auto_link_active_a.activity_sample_window);
	cs4224_phy_write(phydev, CS4224_LED_LED_AUTO_LINK_ACTIVE_INV_CFG_A,
			 led_config->auto_link_active_a.inv_activity_sample_window);
	cs4224_phy_write(phydev, CS4224_LED_LED_AUTO_LINK_ACTIVE_WIN_CFG_B,
			 led_config->auto_link_active_b.activity_sample_window);
	cs4224_phy_write(phydev, CS4224_LED_LED_AUTO_LINK_ACTIVE_INV_CFG_B,
			 led_config->auto_link_active_b.inv_activity_sample_window);

	reg = led_config->priorities[0] & 0xF;
	reg |= (led_config->priorities[1] & 0xF) << 4;
	reg |= (led_config->priorities[2] & 0xF) << 8;
	reg |= (led_config->priorities[3] & 0xF) << 12;
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF0, reg);
	reg = led_config->priorities[4] & 0xF;
	reg |= (led_config->priorities[5] & 0xF) << 4;
	reg |= (led_config->priorities[6] & 0xF) << 8;
	reg |= (led_config->priorities[7] & 0xF) << 12;
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF1, reg);
	reg = led_config->priorities[8] & 0xF;
	reg |= (led_config->priorities[9] & 0xF) << 4;
	reg |= (led_config->priorities[10] & 0xF) << 8;
	reg |= (led_config->priorities[11] & 0xF) << 12;
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF2, reg);
	cs4224_phy_write(phydev, CS4224_LED_PRIORITY_DEF3,
			 led_config->priorities[12] & 0xF);
	cs4224_phy_write(phydev, CS4224_LED_LED_MODE_SEL,
			 led_config->led_mode_select);

	reg = led_config->auto_link_active_enable << 8;
	reg |= led_config->link_active_b << 7;
	reg |= led_config->link_active_a << 0;
	reg |= led_config->link_host_disable << 1;
	reg |= led_config->module_boot << 2;
	reg |= led_config->beacon << 3;
	reg |= led_config->user_def1 << 4;
	reg |= led_config->user_def2 << 5;
	reg |= led_config->user_def3 << 6;
	cs4224_phy_write(phydev, CS4224_LED_LED_CTRL, reg);
	reg = led_config->sustain_duration & 7;
	cs4224_phy_write(phydev, CS4224_LED_CFG_SUSTAIN_DUR, reg);

	if (debug_ledio)
		debug_phyio = 0;

	return 0;
}

/**
 * Read the current value of a GPIO pin
 *
 * @param phydev	Phy device to read from
 * @param gpio_pin	pin number to read [1..11]
 *
 * @return -1 if GPIO pin out of range, otherwise 0 or 1 depending on value.
 */
int cs4224_gpio_read(struct phy_device *phydev, int gpio_pin)
{
	static const int gpio_reg[] = {
		0,
		CS4224_GPIO_GPIO1_VALUE,
		CS4224_GPIO_GPIO2_VALUE,
		CS4224_GPIO_GPIO3_VALUE,
		CS4224_GPIO_GPIO4_VALUE,
		CS4224_GPIO_GPIO5_VALUE,
		CS4224_GPIO_GPIO6_VALUE,
		CS4224_GPIO_GPIO7_VALUE,
		CS4224_GPIO_GPIO8_VALUE,
		CS4224_GPIO_GPIO9_VALUE,
		CS4224_GPIO_GPIO10_VALUE,
		CS4224_GPIO_GPIO11_VALUE
	};
	int reg;
	if (!gpio_pin || gpio_pin >= ARRAY_SIZE(gpio_reg)) {
		printf("Error: GPIO pin %d out of range for %s\n", gpio_pin,
		       phydev->dev->name);
		return -1;
	}
	reg = gpio_reg[gpio_pin];
	return cs4224_phy_read(phydev, reg);
}

int cs4224_phy_read(struct phy_device *phydev, int slice, uint16_t reg)
{
	return phy_read(phydev, 0, reg + slice * 0x1000);
}

int cs4224_phy_write(struct phy_device *phydev, int slice, uint16_t reg,
		     uint16_t data)
{
	return phy_write(phydev, 0, reg + slice * 0x1000, data);
}

/**
 * Returns the hardware ID of the specified Inphi PHY.  Also initializes
 * the number of ports and dies of the device.
 *
 * @param	phydev	PHY device to get the ID of
 *
 * @return	The Inphy PHY ID
 *
 * NOTE: This will cache the value in the phy device private data as well as
 *	 record the number of ports and dies.
 */
enum cs4224_hw_id cs4224_hw_id(struct phy_device *phydev)
{
	struct cs4224_private *p = (struct cs4224_private *)phydev->priv;
	uint16_t sku;

	if (priv->sku)
		return priv->sku;

	sku = cs4224_phy_read(phydev, 0, CS4224_EFUSE_PDF_SKU);
	/* Clear SKU bits [15:5] and [3] */
	sku &= 0x0017;

	/* If EFUSE not programmed, default to CS4343 */
	if ((sku & 0x10) != 0x10) {
		uint16_t chip_id;

		chip_id = cs4224_phy_read(phydev, 0, CS4224_GLOBAL_CHIP_ID_MSB);
		if (chip_id == 0 || chip_id == 0x3e5) {
			p->max_ports = CS4224_MAXNUM_CS4343_PORTS;
			p->max_dies = CS4224_MAX_NUM_CS4343_DIES;
		} else {
			p->max_ports = CS4224_MAXNUM_CS4223_PORTS;
			p->max_dies = CS4224_MAX_NUM_CS4223_DIES;

		}
	} else {
		switch (sku) {
		case  CS4224_HW_CS4223:
			p->max_ports = CS4224_MAX_NUM_CS4223_PORTS;
			p->max_dies = CS4224_MAX_NUM_CS4223_DIES;
			break;
		case CS4224_HW_CS4224:
			p->max_ports = CS4224_MAX_NUM_CS4224_PORTS;
			p->max_dies = CS4224_MAX_NUM_CS4224_DIES;
		case CS4224_HW_CS4221:
			/* 10 port simplex */
			p->max_ports = CS4224_MAX_NUM_CS4221_PORTS;
			p->max_dies = CS4224_MAX_NUM_CS4221_DIES;
			break;
		case CS4224_HW_CS4227:
			/* 2 port duplex */
			p->max_ports = CS4224_MAX_NUM_CS4227_PORTS;
			p->max_dies  = CS4224_MAX_NUM_CS4227_DIES;
			break;
		case CS4224_HW_CS4210:
			/* 16 port simplex */
			p->max_ports = CS4224_MAX_NUM_CS4210_PORTS;
			p->max_dies  = CS4224_MAX_NUM_CS4210_DIES;
			break;
		case CS4224_HW_CS4341:
			/* 8 port duplex */
			p->max_ports = CS4224_MAX_NUM_CS4341_PORTS;
			p->max_dies  = CS4224_MAX_NUM_CS4341_DIES;
			break;
		default:
			/* SKU must be cs4343 or if undefined we default to 8 port duplex */
			p->max_ports = CS4224_MAX_NUM_CS4343_PORTS;
			p->max_dies  = CS4224_MAX_NUM_CS4343_DIES;
			break;
		}
	}
	p->sku = sku;
	return sku;
}

/**
 * Configures the host and line tx and rx trace loss in dB.
 *
 * @param phydev	PHY device (including slice)
 * @param host_tx_loss	Host transmit trace loss in dB, valid range 0..6
 * @param host_rx_loss	Host receive trace loss in dB, valid range 0..6
 * @param line_tx_loss	Line transmit loss in dB, valid range 0..6
 * @param line_rx_loss	Line receive loss in dB, valid range 0..6
 *
 * NOTE: Line is between the MAC and the PHY,
 *	 host is between PHY and SFP+/backplane
 *
 * @return	0 for success
 */
int cs4224_configure_trace_loss(struct phy_device *phydev,
				uint8_t host_tx_loss, uint8_t host_rx_loss,
				uint8_t line_tx_loss, uint8_t line_rx_loss)
{
	uint16_t addr;
	uint16_t data;
	static const uint16_t tx_loss_data_ctrla[7] = {
		0x0814, 0x0814, 0x3810, 0x4818, 0x4848, 0x4848, 0x4818
	};
	static const uint16_t tx_loss_data_ctrlb[7] = {
		0x0104, 0x0104, 0x0105, 0x0109, 0x010b, 0x010d, 0x010e
	};
	static const uint16_t rx_loss_tbl[7] = {
		1, 0, 1, 4, 5, 6, 7
	};
	static const uint16_t lim_eqadj_tbl[7] = {
		0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100
	};
	static const uint16_t pass_eqadj1_tbl[7] = {
		0x0001, 0x0000, 0x0001, 0x0004, 0x0005, 0x0006, 0x0007
	};
	static const uint16_t pass_eqadj2_tbl[7] = {
		0x070f, 0x070f, 0x070f, 0x070f, 0x070f, 0x070f, 0x070f
	};

	addr = cs4224_adj_pp(phydev, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE26_LSB);
	data = phy_read(phydev, 0, addr);
	data = data & 0xfff8;
	data |= rx_loss_tbl[host_rx_loss];
	phy_write(phydev, 0, addr, data);
	addr = cs4224_adj_pp(phydev,
			     CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLA);
	phy_write(phydev, 0, addr, tx_loss_data_ctrla[host_tx_loss]);
	addr = cs4224_adj_pp(phydev,
			     CS4224_PP_HOST_SDS_COMMON_STX0_TX_OUTPUT_CTRLB);
	phy_write(phydev, 0, addr, tx_loss_data_ctrlb[host_tx_loss]);

	addr = cs4224_adj_pp(phydev, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE26_LSB);
	data = phy_read(phydev, 0, addr);
	data = data & 0xfff8;
	data |= rx_loss_tbl[line_rx_loss];
	phy_write(phydev, 0, addr, data);
	addr = cs4224_adj_pp(phydev,
			     CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA);
	phy_write(phydev, 0, addr, tx_loss_data_ctrla[line_tx_loss]);
	addr = cs4224_adj_pp(phydev,
			     CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB);
	phy_write(phydev, 0, addr, tx_loss_data_ctrlb[line_tx_loss]);

	/* Update the pre-equalizer settings */
	addr = cs4224_adj_pp(phydev, CS4224_PP_LINE_SDS_COMMON_SRX0_RX_MISC);
	phy_write(phydev, 0, addr, lim_eqadj_tbl[line_rx_loss]);

	addr = cs4224_adj_pp(phydev, CS4224_PP_LINE_SDS_DSP_MSEQ_CAL_RX_EQADJ1);
	phy_write(phydev, 0, addr, pass_eqadj1_tbl[line_rx_loss]);

	addr = cs4224_adj_pp(phydev, CS4224_PP_LONE_SDS_DSP_MSEQ_CAL_RX_EQADJ2);
	phy_write(phydev, 0, addr, pass_eqadj2_tbl[line_rx_loss]);
	return 0;
}


/**
 * Programs the internal temperature LUT
 */
static int cs4224_hsif_mon_temp_program_lut(struct phy_device *phydev,
					    uint16_t reclaim_offset)
{
	int i;
	static const uint16_t cs4224_lut_mon_temp[] = {
	    0x6059, 0x81FD, /* Index 0 -> DSP_SDS_TEMPMON_MON_LUT_RANGE0,VALUE0 */
	    0x685E, 0x82E2, /* Index 1 -> DSP_SDS_TEMPMON_MON_LUT_RANGE1,VALUE1 */
	    0x6E9A, 0x826F, /* Index 2 */
	    0x74F9, 0x81FC, /* Index 3 */
	    0x7B79, 0x81CD, /* Index 4 */
	    0x8235, 0x819F, /* Index 5 */
	    0x892D, 0x8114, /* Index 6 */
	    0x9026, 0x808A, /* Index 7 */
	    0xFFFF, 0x0000, /* Index 8 */
	    0x0000, 0x0000, /* Index 9 */
	    0x0000, 0x0000, /* Index 10 */
	    0x0000, 0x0000, /* Index 11 */
	    0x0000, 0x0000, /* Index 12 */
	    0x0000, 0x0000, /* Index 13 */
	    0x0000, 0x0000, /* Index 14 */
	    0x0000, 0x0000, /* Index 15 -> DSP_SDS_TEMPMON_MON_LUT_RANGE15,VALUE15*/
	};
	for (i = 0; i < 32; i += 2) {
		uint16_t range = cs4224_lut_mon_temp[i];
		uint16_t value =
			 cs4224_twos_compliment(cs4224_lut_mon_temp[i + 1])
			 + reclaim_offset;
		int index = i / 2;
		cs4224_phy_write(phydev,
				 CS4224_DSP_SDS_TEMPMON_MON_LUT_RANGE0 + index,
				 range);
		cs4224_phy_write(phydev,
				 CS4224_DSP_SDS_TEMPMON_MON_LUT_VALUE0 + index,
				 value);
	}
	return 0;
}

/**
 * Outputs the current PHY temperature in C
 *
 * @param[in] phydev - phy device
 * @param[out] temp - temperature
 *
 * @return 0 for success, -1 on error
 */
static int cs4224_hsif_min_temp_read(struct phy_device *phydev, int *temp)
{
	int data;
	int is_reclaimed_part ;
	int use_register_lut = 1;

	/* Read misc config to check for reclaimed part */
	data = cs4224_phy_read(phydev, CS4224_EFUSE_PDF_MISC_CHIP_CONFIG);
	if (data < 0)
		goto err;
	is_reclaimed_part = ((data & 0x8000) == 0x8000);

	/* Power on voltage monitor */
	data = cs4224_phy_read(phydev, CS4224_LINE_SDS_COMMON_SRX0_RX_CONFIG);
	if (data < 0)
		goto err;
	data &= ~0x2000;
	cs4224_phy_write(phydev, CS4224_LINE_SDS_COMMON_SRX0_RX_CONFIG, data);
	udelay(10);

	if (use_register_lut) {
		uint16_t cal_constant = 0;
		if (is_reclaimed_part) {
			data = cs4224_phy_read(phydev,
					       CS4224_EFUSE_PDF_VOL_MON_LUT7);
			if (data < 0)
				goto err;
			cal_constant = (uint16_t)data;
			cs4224_phy_write(phydev,
					 CS4224_DSP_SDS_TEMPMON_MON_CONTROL0,
					 0x323);
		} else {
			cs4224_phy_write(phydev,
					 CS4224_DSP_SDS_TEMPMON_MON_CONTROL0,
					 0x333);
		}
		cs4224_phy_write(phydev,
				 CS4224_DSP_SDS_TEMPMON_MON_CONTROL1, 0x0010);
		cs4224_hsif_mon_temp_program_lut(phydev, cal_constant);
		mdelay(1);
	} else {
		cs4224_phy_write(phydev, CS4224_DSP_SDS_TEMPMON_MON_CONTROL0,
				 0x0033);
		cs4224_phy_write(phydev, CS4224_DSP_SDS_TEMPMON_MON_CONTROL1,
				 0x0010);
	}
	data = cs4224_phy_read(phydev, CS4224_DSP_SDS_TEMPMON_MON_STATUS2);
	if (data < 0)
		goto err;
	/* This is the equivelent of ((data / 256.0) * -1.122) + 210.37 */
	*temp = ((data * -1122) + (256 * 210370)) / 256000;
	return 0;

err:
	return -1;
}

int do_cs4224(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct phy_device *phydev;
	int temp;
	uint8_t buffer[256];
	int rc;

	if (argc < 2) {
		printf("Too few args, argc=%d\n", argc);
		return CMD_RET_USAGE;
	}

	if (argc < 3) {
		puts("Ethernet device not specified\n");
		return CMD_RET_USAGE;
	}
	phydev = mdio_phydev_for_ethname(argv[2]);
	if (!phydev) {
		printf("Could not find phy device for %s\n", argv[2]);
		return -1;
	}

	if (argv[1][0] == 't') {
		if (cs4224_hsif_min_temp_read(phydev, &temp))
			printf("Error reading temperature\n");
		else
			printf("%s: %d C\n", argv[2], temp);
		return 0;
	} else if (argv[1][0] == 'e') {
		rc = cs4224_sfp_cache_read_data(phydev, false, 0, buffer,
						sizeof(buffer));
		if (rc < 0) {
			puts("Could not read EEPROM data\n");
			return -1;
		} else if (rc > 0) {
			puts("SFP+ Module not present\n");
		} else {
			print_buffer(0, buffer, 1, sizeof(buffer), 0);
		}
	} else if (argv[1][0] == 'd') {
		if (cs4224_sfp_cache_read_data(phydev, true, 0, buffer,
					       sizeof(buffer)) < 0) {
			puts("Could not read EEPROM data\n");
			return -1;
		}
		print_buffer(0, buffer, 1, sizeof(buffer), 0);
	} else {
		printf("Unknown arg %s\n", argv[1]);
		return CMD_RET_USAGE;
	}
	return 0;
}

U_BOOT_CMD(
	cs4224, 3, 1, do_cs4224,
	"Cortina CS43XX PHY system",
	"temp [eth device] - display temperature\n"
	"cs4224 eeprom [eth device] - display EEPROM data (NIC410NVG only)\n"
	"cs4224 diag   [eth device] - display DIAG data (NIC410NVG only)\n");
