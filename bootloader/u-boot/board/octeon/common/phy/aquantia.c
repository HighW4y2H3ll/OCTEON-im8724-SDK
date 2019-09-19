/***********************license start************************************
 * Copyright (c) 2015 Cavium Inc. (support@cavium.com).
 * All rights reserved.
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
 ***********************license end**************************************/

/* This file is based somewhat off of Aquantia's SDK which is freeware.  The
 * code was re-written for use in Linux and U-Boot and possibly other
 * environments.  Code has also been cleaned up and simplified in a number of
 * cases and has been rewritten to be easier to read, eliminating almost all
 * of the macros.  Registers as well as functions have also been renamed.  The
 * register names often will not exactly match the names in the data sheet.
 *
 * Currently most of the configuration code, status code, autonegotiation code,
 * alarm code, interrupt mask code and SERDES code has been ported.
 *
 * Some of the loopback code has been ported.
 *
 * None of the packet generation code, test pattern, cable diagnostics,
 * memory access or MACSEC code has been ported yet.  Only some of the flash
 * programming code has been ported and it is not yet complete enough for use.
 *
 * Support is only for the 28nm devices.  Support for the 40nm devices (APPIA)
 * has not been included.
 *
 * Much of the code has not been tested at this time.
 */

#include <common.h>
#include <command.h>
#include <asm/arch/octeon_eth.h>
#include <miiphy.h>
#include <phy.h>
#include "aquantia.h"
#include "aquantia_regs.h"

#ifdef __U_BOOT__
# define aq_debug(fmt, args...)		\
	debug(fmt, ##args)
# define aq_printf(fmt, args...)	\
	printf(fmt, ##args)
#else
# error	Fill in for your OS
#endif

/** Used to map LED stretching */
static const uint8_t led_stretch[] = { LS_OFF, LS_28ms, LS_60ms, LS_100ms };

/**
 * Delay for delay milliseconds
 *
 * @param	delay	How long to delay in milliseconds
 */
static inline void aq_mdelay(uint32_t delay)
{
#ifdef __U_BOOT__
	mdelay(delay);
#else
# error fill in for Linux
#endif
}

/**
 * Delay for delay microseconds
 *
 * @param	delay	How long to delay in microseconds
 */
static inline void aq_udelay(uint32_t delay)
{
#ifdef __U_BOOT__
	udelay(delay);
#else
# error fill in for Linux
#endif
}

/**
 * Set or clear a bit
 *
 * @param val	Value to modify
 * @param bit	Bit number to set or clear
 * @param set	true to set, false to clear
 *
 * @return	modified value
 */
static inline uint16_t aq_phy_bitset(uint16_t val, int bit, bool set)
{
	if (set)
		return val | (1 << bit);
	else
		return val & ~(1 << bit);
}

/**
 * Reads the value from a PHY register
 *
 * @param phydev	PHY device to read from
 * @param devad		Device (MMD) address
 * @param regnum	register number
 *
 * @return value read from PHY register
 */
static inline int aq_phy_read(struct phy_device *phydev,
			      int devad, int regnum)
{
	return phy_read(phydev, devad, regnum);
}

/**
 * Reads a 32-bit value from a pair of PHY registers
 *
 * @param phydev	PHY device to read from
 * @param devad		Device (MMD) address
 * @param regnum	register number
 *
 * @return 32-bit value from a pair of registers
 */
static int aq_phy_read32(struct phy_device *phydev, int devad, int regnum)
{
	uint32_t value;
	value = phy_read(phydev, devad, regnum + 1) << 16;
	value |= phy_read(phydev, devad, regnum);
	return value;
}

/**
 * Writes a value to a PHY register
 *
 * @param phydev	PHY device to write to
 * @param devad		Device (MMD) address
 * @param regnum	register number
 * @param value		value to write
 *
 * @return	0 for success, otherwise error
 */
static inline int aq_phy_write(struct phy_device *phydev, int devad,
				 int regnum, uint16_t value)
{
	return phy_write(phydev, devad, regnum, value);
}

/**
 * Writes a value to a pair of PHY registers
 *
 * @param phydev	PHY device to write to
 * @param devad		Device (MMD) address
 * @param regnum	register number
 * @param value		value to write
 *
 * @return	0 for success, otherwise error
 */
static int aq_phy_write32(struct phy_device *phydev, int devad,
			  int regnum, uint32_t value)
{
	int rc;

	rc = phy_write(phydev, devad, regnum + 1, (value >> 16) & 0xffff);
	if (rc)
		return rc;
	return phy_write(phydev, devad, regnum, value & 0xffff);
}

/**
 * Reverses the bits within all of the bytes
 *
 * @param val	32-bit value to reverse the bits in each 8-bit byte
 *
 * @return	bit reversed value
 */
static uint32_t bit_reverse32(uint32_t val)
{
	uint64_t val64 = val;
	val64 = (((val64 & 0xaaaaaaaaaaaaaaaa) >> 1) |
		 ((val64 & 0x5555555555555555) << 1));
	val64 = (((val64 & 0xcccccccccccccccc) >> 2) |
		 ((val64 & 0x3333333333333333) << 2));
	val64 = 0xffffffff & ~(((val64 & 0xf0f0f0f0f0f0f0f0) >> 4) |
			       ((val64 & 0x0f0f0f0f0f0f0f0f) << 4));
	return val64 & 0xffffffff;
}

/**
 * Reads the configuration from the specified PHY device
 *
 * @param phydev	Phy device to get configuration from
 * @param[out] config	configuration of the phy
 *
 * @return 0 for success, -1 on error.
 */
int aq_phy_get_config(struct phy_device *phydev, struct aq_phy_config *config)
{
	int temp;
	int i, val;
	uint32_t wakeup_mask_active = 0;
	int wol_frame;
	bool wakeup_frame_enable;

	temp = aq_phy_write(phydev, 0x1e,
			    AQ_REG_GBL_THERM_LOW_TEMP_WARNING_THRESHOLD,
			    config->low_temp_warn_thresh << 8);

	temp = aq_phy_write(phydev, 0x1e,
			    AQ_REG_GBL_THERM_LOW_TEMP_FAILURE_THRESHOLD,
			    config->low_temp_fail_thresh << 8);
	if (temp < 0)
		return temp;

	temp >>= 8;
	if (temp & 0x80)
		temp = temp - 0x100;
	config->low_temp_fail_thresh = temp;

	temp = aq_phy_read(phydev, 0x1e,
			   AQ_REG_GBL_THERM_HI_TEMP_WARNING_THRESHOLD);
	if (temp < 0)
		return temp;

	temp >>= 8;
	if (temp & 0x80)
		temp = temp - 0x100;
	config->hi_temp_warn_thresh = temp;

	temp = aq_phy_read(phydev, 0x1e,
			   AQ_REG_GBL_THERM_HI_TEMP_FAILURE_THRESHOLD);
	if (temp < 0)
		return temp;

	temp >>= 8;
	if (temp & 0x80)
		temp = temp - 0x100;
	config->hi_temp_fail_thresh = temp;

	val = aq_phy_read(phydev, 3, AQ_REG_PCS_RX_VENDOR_PROVISIONING_1);
	config->disable_crc8_frame_trashing = !(val & 1);

	/* Configure the LEDs */
	for (i = 0; i < 6; i++) {
		val = aq_phy_read(phydev, 0x1e,
				  AQ_REG_GBL_LED_PROVISIONING_1 + i);
		config->led_control[i].led_toggle_on_tx = !!(val & 4);
		config->led_control[i].led_toggle_on_rx = !!(val & 8);
		config->led_control[i].led_on_connecting = !!(val & 0x10);
		config->led_control[i].led_on_100m_link_up = !!(val & 0x20);
		config->led_control[i].led_on_1g_link_up = !!(val & 0x40);
		config->led_control[i].led_on_10g_link_up = !!(val & 0x80);
		config->led_control[i].led_on = !!(val & 0x100);
		config->led_control[i].led_on_2_5g_link_up = !!(val & 0x4000);
		config->led_control[i].led_on_5g_link_up = !!(val & 0x8000);
		config->led_control[i].led_stretch = led_stretch[val & 3];
	}

	val = aq_phy_read(phydev, 7,
			  AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1);
	if (!(val & 0x40))
		config->wol_control = WOL_NONE;
	else
		config->wol_control = (val & 0x80) ?
					WOL_1000BASE_T : WOL_100BASE_TX;

	/* Get the wake-up frame filter settings: note that there is a global
	 * enable, but individual frames can be disabled by setting their
	 * 128-bit filter mask to all-zero
	 */

	/* NOTE! The following operations are required in reverse for reading
	 * the WoL filter FCS:
	 *     (1)     take the ones complement of the desired CRC,
	 *     (2)     bit-reverse each byte of the result of step (1), and
	 *     (3)     program the lower 16-bits of the result of step (2) in
	 *             the lower CRC register and the upper 16 bits in the upper
	 *             CRC register.
	 */
	val = aq_phy_read(phydev, 0x1d,
			  AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_DETECT_ENABLE);
	wakeup_frame_enable = !!(val & 1);
	/* frame 0 */
	for (wol_frame = 0; wol_frame < 5; wol_frame++) {
		wakeup_mask_active = false;
		val = aq_phy_read32(phydev, 0x1d,
				    AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_111_96 +
				    wol_frame * 10);
		config->wol_frame[wol_frame].filter[3] = val;
		if (val)
			wakeup_mask_active = true;

		val = aq_phy_read32(phydev, 0x1d,
				    AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_79_64 +
				    wol_frame * 10);
		if (val)
			wakeup_mask_active = true;
		config->wol_frame[wol_frame].filter[2] = val;

		val = aq_phy_read32(phydev, 0x1d,
				    AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_47_32 +
				    wol_frame * 10);
		if (val)
			wakeup_mask_active = true;
		config->wol_frame[wol_frame].filter[1] = val;
		val = aq_phy_read32(phydev, 0x1d,
				    AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_15_0 +
				    wol_frame * 10);
		if (val)
			wakeup_mask_active = true;
		config->wol_frame[wol_frame].filter[0] = val;
		val = aq_phy_read32(phydev, 0x1d,
				    AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_CRC_0_15_0 +
				    wol_frame * 10);
		config->wol_frame[wol_frame].crc32 = bit_reverse32(val);
		config->wol_frame[wol_frame].enable =
				wakeup_frame_enable | wakeup_mask_active;
	}

	config->magic_packet_frame.enable = false;
	/* Byte reverse values */
	val = aq_phy_read(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_15_0);
	config->magic_packet_frame.mac_filter[0] = (val >> 8) & 0xff;
	config->magic_packet_frame.mac_filter[1] = val & 0xff;
	if (val)
		config->magic_packet_frame.enable = true;

	val = aq_phy_read(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_31_16);
	config->magic_packet_frame.mac_filter[2] = (val >> 8) & 0xff;
	config->magic_packet_frame.mac_filter[3] = val & 0xff;
	if (val)
		config->magic_packet_frame.enable = true;

	val = aq_phy_read(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_47_32);
	config->magic_packet_frame.mac_filter[4] = (val >> 8) & 0xff;
	config->magic_packet_frame.mac_filter[5] = val & 0xff;
	if (val)
		config->magic_packet_frame.enable = true;

	return 0;
}

/**
 * Sets the phy configuration
 *
 * @param phydev	Phy device to configure
 * @param config	Configuration to use
 *
 * @return 0 for success, -1 on error.
 */
int aq_phy_set_config(struct phy_device *phydev,
		      const struct aq_phy_config *config)
{
	int i, val;

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_LOW_TEMP_WARNING_THRESHOLD,
		     config->low_temp_warn_thresh << 8);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_LOW_TEMP_FAILURE_THRESHOLD,
		     config->low_temp_fail_thresh << 8);

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_HI_TEMP_WARNING_THRESHOLD,
		     config->hi_temp_warn_thresh << 8);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_HI_TEMP_FAILURE_THRESHOLD,
		     config->hi_temp_fail_thresh << 8);

	val = aq_phy_read(phydev, 3, AQ_REG_PCS_RX_VENDOR_PROVISIONING_1);
	if (config->disable_crc8_frame_trashing)
		val &= ~1;
	else
		val |= 1;
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_PROVISIONING_1, val);

	/* Configure the LEDs */
	for (i = 0; i < 6; i++) {
		val = aq_phy_read(phydev, 0x1e,
				  AQ_REG_GBL_LED_PROVISIONING_1 + i);
		if (config->led_control[i].led_toggle_on_tx)
			val |= 4;
		else
			val &= ~4;
		if (config->led_control[i].led_toggle_on_rx)
			val |= 8;
		else
			val &= ~8;
		if (config->led_control[i].led_on_connecting)
			val |= 0x10;
		else
			val &= ~0x10;

		if (config->led_control[i].led_on_100m_link_up)
			val |= 0x20;
		else
			val &= ~0x20;

		if (config->led_control[i].led_on_1g_link_up)
			val |= 0x40;
		else
			val &= ~0x40;

		if (config->led_control[i].led_on_10g_link_up)
			val |= 0x80;
		else
			val &= ~0x80;

		if (config->led_control[i].led_on)
			val |= 0x100;
		else
			val &= ~0x100;

		if (config->led_control[i].led_on_2_5g_link_up)
			val |= 0x4000;
		else
			val &= ~0x4000;

		if (config->led_control[i].led_on_5g_link_up)
			val |= 0x8000;
		else
			val &= ~0x8000;

		val &= ~3;
		switch (config->led_control[i].led_stretch) {
		default:
			val |= 0;
			break;
		case LS_28ms:
			val |= 1;
			break;
		case LS_60ms:
			val |= 2;
			break;
		case LS_100ms:
			val |= 3;
			break;
		}
		aq_phy_write(phydev, 0x1e,
			     AQ_REG_GBL_LED_PROVISIONING_1 + i, val);
	}

	/* Configure wake on LAN mode */
	val = aq_phy_read(phydev, 7,
			  AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1);
	switch (config->wol_control) {
	case WOL_NONE:	/* disabled */
		val &= ~0x40;
		break;
	case WOL_1000BASE_T:
		val |= 0xc0;
		break;
	case WOL_100BASE_TX:
		val |= 0x80;
		break;
	}
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1,
		     val);

	/* Get the wake-up frame filter settings: note that there is a global
	 * enable, but individual frames can be disabled by setting their
	 * 128-bit filter mask to all-zero
	 */

	/* NOTE! The following operations are required in reverse for reading
	 * the WoL filter FCS:
	 *     (1)     take the ones complement of the desired CRC,
	 *     (2)     bit-reverse each byte of the result of step (1), and
	 *     (3)     program the lower 16-bits of the result of step (2) in
	 *             the lower CRC register and the upper 16 bits in the upper
	 *             CRC register.
	 */
	for (i = 0; i < 5; i++) {
		val = aq_phy_read(phydev, 0x1d,
				  AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_DETECT_ENABLE);
		if (config->wol_frame[i].enable)
			val |= 1;
		else
			val &= ~1;
		aq_phy_write(phydev, 0x1d,
			     AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_DETECT_ENABLE, val);

		aq_phy_write32(phydev, 0x1d,
			       AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_111_96 +
			       i * 10,
			       config->wol_frame[i].enable ?
					config->wol_frame[i].filter[3] : 0);
		aq_phy_write32(phydev, 0x1d,
			       AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_79_64 +
			       i * 10,
			       config->wol_frame[i].enable ?
					config->wol_frame[i].filter[2] : 0);
		aq_phy_write32(phydev, 0x1d,
			       AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_47_32 +
			       i * 10,
			       config->wol_frame[i].enable ?
					config->wol_frame[i].filter[3] : 0);
		aq_phy_write32(phydev, 0x1d,
			       AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_MASK_0_15_0 +
			       i * 10,
			       config->wol_frame[i].enable ?
					config->wol_frame[i].filter[3] : 0);

		aq_phy_write32(phydev, 0x1d,
			       AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_CRC_0_15_0 +
			       i * 10,
			       bit_reverse32(config->wol_frame[i].crc32));
	}

	if (config->magic_packet_frame.enable)
		val = config->magic_packet_frame.mac_filter[0] << 8 |
			config->magic_packet_frame.mac_filter[1];
	else
		val = 0;
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_15_0, val);

	if (config->magic_packet_frame.enable)
		val = config->magic_packet_frame.mac_filter[2] << 8 |
		config->magic_packet_frame.mac_filter[3];
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_31_16, val);

	if (config->magic_packet_frame.enable)
		val = config->magic_packet_frame.mac_filter[4] << 8 |
		config->magic_packet_frame.mac_filter[5];
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_WOL_MAGIC_PACKET_47_32, val);

	return 0;
}

/**
 * Gets the phy operating modes for the system side and network side for 1 and
 * 10G.
 *
 * @param	phydev		phy device to get operating mode from
 * @param[out]	sys_op_mode_10g	10G system interface operating mode
 * @param[out]	net_op_mode_10g	10G network operating mode
 * @param[out]	net_op_mode_1g	1G network operating mode
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_get_op_modes(struct phy_device *phydev,
			enum aq_phy_10g_system_op_mode *sys_op_mode_10g,
			enum aq_phy_10g_network_op_mode *net_op_mode_10g,
			enum aq_phy_1g_network_op_mode *net_op_mode_1g)
{
	uint16_t val;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_OPERATING_MODE);
	switch ((val >> 14) & 3) {
	case 0:
		*sys_op_mode_10g = SOM_10G_XAUI;
		break;
	case 1:
		if (val & 0x20)
			*sys_op_mode_10g = SOM_10G_RXAUI_DUNE;
		else
			*sys_op_mode_10g = SOM_10G_RXAUI_MARVELL;
		break;
	case 2:
		*sys_op_mode_10g = SOM_10G_XFI;
		break;
	case 3:
		*sys_op_mode_10g = SOM_10G_OTHER;
		break;
	}

	switch ((val >> 6) & 7) {
	case 0:	/* Normal mode, 10GBase-T or 1000Base-T */
		*net_op_mode_10g = NOM_10G_10GBASE_T;
		*net_op_mode_1g = NOM_1G_1000BASE_T;
		break;
	case 1:
		/* 1000Base-X or XFI */
		*net_op_mode_10g = NOM_10G_XFI;
		*net_op_mode_1g = NOM_1G_1000BASE_X;
		break;
	default:
		*net_op_mode_10g = NOM_10G_OTHER;
		*net_op_mode_1g = NOM_1G_OTHER;
		break;
	}

	return 0;
}

/**
 * Gets the mode used for 2.5Gbps
 *
 * @param	phydev	Phy device to get mode for
 * @param[out]	mode	Mode used for 2.5Gbps
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_2_5g_mode(struct phy_device *phydev, enum aq_phy_2_5g_op_mode *mode)
{
	uint16_t val;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_OPERATING_MODE);
	if (val & (1 << 0xd)) {
		*mode = NOM_2_5G_2500BASE_X;
	} else {
		switch ((val >> 0xe) & 0x3) {
		case 0:
			*mode = NOM_2_5G_XAUI;
			break;
		case 1:
			*mode = NOM_2_5G_RXAUI;
			break;
		case 2:
			*mode = NOM_2_5G_XFI;
			break;
		default:
			*mode = NOM_2_5G_OTHER;
			break;
		}
	}
	return 0;
}

/**
 * Gets the static configuration of the phy
 *
 * @param	phydev	PHY device
 * @param[out]	config	static configuration
 *
 * @returns 0 for success, -1 for error
 */
int aq_phy_get_static_config (struct phy_device *phydev,
			      struct aq_phy_static_config *config)
{
	int val;
	int i;
	uint32_t dev_id;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_FIRMWARE_ID);
	config->firmware_major_rev = (val >> 8) & 0xff;
	config->firmware_minor_rev = val & 0xff;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESERVED_STATUS_1);
	config->firmware_rom_id = val & 0xff;

	val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_DEVICE_ID_2);
	config->ieee_model_num = (val >> 4) & 0x3f;
	config->ieee_rev_num = val & 0xf;
	dev_id = (val >> 10) | aq_phy_read(phydev, 1,
					   AQ_REG_PMA_STANDARD_DEVICE_ID_1) << 6;
	/* Reverse the bits for the OUI */
	dev_id = (((dev_id & 0xaaaaaaaa) >> 1) | ((dev_id & 0x55555555) << 1));
	dev_id = (((dev_id & 0xcccccccc) >> 2) | ((dev_id & 0x33333333) << 2));
	config->oui = ((dev_id & 0xf0f0f0f0 >> 4) | ((dev_id & 0x0f0f0f0f) << 4));

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_PIN_STATUS);
	if (val & 0x2000)
		config->daisy_chain_setting = DC_SLAVE;
	else
		config->daisy_chain_setting = DC_MASTER;

	config->tx_enable = !!(val & 0x80);
	for (i = 0; i < 6; i++)
		config->led[i] = !!((val >> i) & 1);

	val = aq_phy_get_op_modes(phydev, &config->sys_op_mode_10g,
				  &config->net_op_mode_10g,
				  &config->net_op_mode_1g);
	if (val)
		return val;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_2);
	config->flash_clock_div = val & 0xff;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_2);
	config->mdio_open_drain = !!(val & 0x10);

	config->mdio_preamble_suppression = !!(val & 0x8);
	config->addr_0_broadcast_enable = !!(val & 0x4000);
	config->msw_read_first = !!(val & 0x2000);

	val = aq_phy_read(phydev, 1,
			  AQ_REG_PMA_RESERVED_VENDOR_PROVISIONING_1);
	config->mdi_swap = !!(val & 1);

	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_TX_RESERVED_VENDOR_PROVISIONING_2);
	config->xfi0_tx_invert = !!(val & 0x400);
	config->xfi1_tx_invert = !!(val & 0x800);

	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_4);
	config->xfi0_rx_invert = !!(val & 0x400);
	config->xfi1_rx_invert = !!(val & 0x800);

	/* Get the XAUI TX connectivity */
	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_TX_RESERVED_VENDOR_PROVISIONING_1);
	config->xaui_tx_serdes_lane0 = (val >> 8) & 3;
	config->xaui_tx_serdes_lane1 = (val >> 10) & 3;
	config->xaui_tx_serdes_lane2 = (val >> 12) & 3;
	config->xaui_tx_serdes_lane3 = (val >> 14) & 3;
	config->xaui_tx_lane0_invert = !!(val & 0x10);
	config->xaui_tx_lane1_invert = !!(val & 0x20);
	config->xaui_tx_lane2_invert = !!(val & 0x40);
	config->xaui_tx_lane3_invert = !!(val & 0x80);

	/* The XAUI lane mapping and controls are also used for rxaui */
	config->rxaui_tx_serdes_lane0 = config->xaui_tx_serdes_lane0;
	config->rxaui_tx_serdes_lane1 = config->xaui_tx_serdes_lane1;
	config->rxaui_tx_lane0_invert = config->xaui_tx_lane0_invert;
	config->rxaui_tx_lane1_invert = config->xaui_tx_lane1_invert;

	/* Get the XAUI RX connectivity */
	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_3);

	config->xaui_rx_serdes_lane0 = (val >> 8) & 3;
	config->xaui_rx_serdes_lane1 = (val >> 10) & 3;
	config->xaui_rx_serdes_lane2 = (val >> 12) & 3;
	config->xaui_rx_serdes_lane3 = (val >> 14) & 3;
	config->xaui_rx_lane0_invert = !!(val & 0x10);
	config->xaui_rx_lane1_invert = !!(val & 0x20);
	config->xaui_rx_lane2_invert = !!(val & 0x40);
	config->xaui_rx_lane3_invert = !!(val & 0x80);

	/* The XAUI lane mapping and controls are also used for rxaui */
	config->rxaui_rx_serdes_lane0 = config->xaui_rx_serdes_lane0;
	config->rxaui_rx_serdes_lane1 = config->xaui_rx_serdes_lane1;
	config->rxaui_rx_lane0_invert = config->xaui_rx_lane0_invert;
	config->rxaui_rx_lane1_invert = config->xaui_rx_lane1_invert;

	/* SGMII */
	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_TX_RESERVED_VENDOR_PROVISIONING_1);
	config->sgmii_tx_serdes = (val >> 2) & 3;
	config->sgmii_tx_invert = !!(val & 1);

	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_3);
	config->sgmii_rx_serdes = (val >> 2) & 3;
	config->sgmii_rx_invert = !!(val & 1);

	val = aq_phy_read(phydev, 7,
			  AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1);
	switch ((val >> 13) & 7) {
	case 0:
		config->start_mode_10g = SSSM_10G_ON;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
		config->start_mode_10g = SSSM_OTHER;
		break;
	case 2:
		config->start_mode_10g = SSSM_XSGMII_ON;
		break;
	case 4:
		config->start_mode_10g = SSSM_1000BASE_X_ON;
		break;
	case 6:
		config->start_mode_10g = SSSM_OFF;
	}

	return 0;
}

/**
 * Gets the current phy status
 *
 * @param	phydev	phy device to use
 * @param	status	status of current device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_status(struct phy_device *phydev, struct aq_phy_status *status)
{
	int val;
	enum aq_phy_conn_state conn_state;
	uint8_t version_str[AQ_PHY_VERSION_STRING_SIZE];
	uint32_t start_address;
	uint32_t size;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_THERMAL_STATUS_2);
	if (val & 1) {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_THERMAL_STATUS_1);
		val >>= 8;
		status->temperature = (val & 0x80) ? (val - 0x100) : val;
	} else {
		status->temperature = -100;
	}

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	if (val & 0x8000) {
		conn_state = CONS_RESET;
		/* In soft-reset, uP is not running */
		status->loaded_image_info.image_id_str[0] = '\0';
		status->loaded_image_info.image_rev_maj = 0;
		status->loaded_image_info.image_rev_min = 0;
		status->loaded_image_info.image_rom_id_num = 0;
		status->loaded_image_info.programmed = false;
	} else {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CONTROL_2);
		if ((val & 0x41) == 0x41) {
			/* Run stall override and run stall */
			conn_state = CONS_STALL;
			status->loaded_image_info.image_id_str[0] = '\0';
			status->loaded_image_info.image_rev_maj = 0;
			status->loaded_image_info.image_rev_min = 0;
			status->loaded_image_info.image_rom_id_num = 0;
			status->loaded_image_info.programmed = false;
			aq_debug("%s: Run stall override and run stall: Global Control 2: 0x%x\n",
				 __func__, val);
			return 0;
		} else {
			val = aq_phy_read(phydev, 7,
					  AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_1);
			conn_state = (val >> 9) & 0x1f;
			if (conn_state > CONS_INVALID)
				conn_state = CONS_INVALID;
		}
	}

	aq_debug("%s: Connection state: %d\n", __func__, conn_state);
	if (conn_state != CONS_RESET &&
	    conn_state != CONS_STALL &&
	    conn_state != CONS_INVALID) {
		size = AQ_PHY_VERSION_STRING_SIZE;
		start_address = AQ_PHY_DRAM_BASE_ADDRESS +
				AQ_PHY_VERSION_STRING_BLOCK_OFFSET;
		aq_phy_memory_read(phydev, version_str, size, start_address);
		for (val = 0; val < 61; val++)

		memcpy(status->loaded_image_info.image_id_str, version_str, 61);
		status->loaded_image_info.image_id_str[61] = '\0';
		status->loaded_image_info.image_rev_maj = version_str[62];
		status->loaded_image_info.image_rev_min = version_str[63];
		status->loaded_image_info.image_rom_id_num = version_str[61];
		status->loaded_image_info.programmed = true;
	} else {
		/* The uP is not running */
		status->loaded_image_info.image_id_str[0] = '\0';
		status->loaded_image_info.image_rev_maj = 0;
		status->loaded_image_info.image_rev_min = 0;
		status->loaded_image_info.image_rom_id_num = 0;
		status->loaded_image_info.programmed = false;
	}

	return 0;
}

/**
 * Perform a soft reset on the phy device
 *
 * @param	phydev	phy device to reset
 *
 * @return	0 for success
 */
int aq_phy_soft_reset(struct phy_device *phydev)
{
	int val;

	aq_phy_read(phydev, 0x1e, AQ_REG_GBL_ALARM_1);

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	val |= 0x8000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, val);

	return 0;
}

/**
 * Wait for a processor intensive MDIO operation to complete.
 *
 * @param phydev	PHY device
 * @param time_ms	How long to wait in ms for the operation to complete
 *
 * @return 0 on success or -1 if operation timed out
 */
static int aq_phy_wait_proc_busy(struct phy_device *phydev, ulong time_ms)
{
#ifdef __U_BOOT__
	ulong start = get_timer(0);
	int val;

	aq_mdelay(1);
	do {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_STATUS_2);
		if (!(val & 0x8000))
			return 0;
		aq_udelay(100);
	} while (get_timer(start) < time_ms);
#else
# error	Fill in for your OS
#endif
	return -1;
}

/**
 * Get the current phy connection status
 *
 * @param	phydev		phy device
 * @param[out]	conn_status	connection status
 *
 * @return 0 for success
 */
int aq_phy_get_connection_status(struct phy_device *phydev,
				 struct aq_phy_conn_status *conn_status)
{
	int ang_vendor_status;
	int ang_res_vendor_status;
	int val;

	ang_vendor_status = aq_phy_read(phydev, 7,
					AQ_REG_AUTONEG_VENDOR_STATUS_1);
	ang_res_vendor_status = aq_phy_read(phydev, 7,
					    AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_1);
	conn_status->energy_online = !!(ang_res_vendor_status & 0x8000);
	conn_status->far_device_exists = !!(ang_res_vendor_status & 0x4000);

	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_2);
	conn_status->num_connect_attempts = val & 0xff;
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	if (val & 0x8000) {
		conn_status->state = CONS_RESET;
	} else  {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CONTROL_2);
 		if ((val & 0x41) == 0x41) {
			conn_status->state = CONS_STALL;
		} else {
			switch ((ang_res_vendor_status >> 9) & 0x1f) {
			case 0x0:
				conn_status->state = CONS_INACTIVE;
				break;
			case 0x1:
				conn_status->state = CONS_CABLE_DIAGNOSTICS;
				break;
			case 0x2:
				switch ((ang_vendor_status >> 2) & 3) {
				case 0x1:
					conn_status->state = CONS_100M_AUTONEG;
					break;
				case 0x2:
					conn_status->state = CONS_1G_AUTONEG;
					break;
				case 0x3:
					conn_status->state = CONS_10G_AUTONEG;
					break;
				default:
					conn_status->state = CONS_INVALID;
					break;
				}
				break;
			case 0x3:
				switch ((ang_vendor_status >> 2) & 3) {
				case 0x1:
					conn_status->state = CONS_100M_TRAINING;
					break;
				case 0x2:
					conn_status->state = CONS_1G_TRAINING;
					break;
				case 0x3:
					conn_status->state = CONS_10G_TRAINING;
					break;
				default:
					conn_status->state = CONS_INVALID;
					break;
				}
				break;
			case 0x4:
				switch ((ang_vendor_status >> 2) & 3) {
				case 0x1:
					conn_status->state = CONS_100M_CONNECTED;
					break;
				case 0x2:
					conn_status->state = CONS_1G_CONNECTED;
					break;
				case 0x3:
					conn_status->state = CONS_10G_CONNECTED;
					break;
				default:
					conn_status->state = CONS_INVALID;
					break;
				}
				break;
			case 0x5:
				conn_status->state = CONS_LINK_BREAK;
				break;
			case 0x6:
				conn_status->state = CONS_TEST_MODE;
				break;
			case 0x7:
				conn_status->state = CONS_LOOPBACK_MODE;
				break;
			case 0x8:
				conn_status->state = CONS_LOW_POWER_MODE;
				break;
			case 0x9:
				switch ((ang_vendor_status >> 2) & 3) {
				case 0x1:
					conn_status->state = CONS_100M_WOL_CONNECTED;
					break;
				case 0x2:
					conn_status->state = CONS_1G_WOL_CONNECTED;
					break;
				default:
					conn_status->state = CONS_INVALID;
				}
			case 0x1f:
			default:
				conn_status->state = CONS_INVALID;
			}
		}
	}

	conn_status->full_duplex = !!(ang_vendor_status & 1);

	switch ((ang_res_vendor_status >> 9) & 0x1f) {
	case 3:
	case 4:
		conn_status->mdi_mdi_x =
			(ang_res_vendor_status & 0x100) ? MS_MDI_X : MS_MDI;
		break;
	default:
		conn_status->mdi_mdi_x = MS_NONE;
	}

	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_STATUS);
	conn_status->master_slave_status =
				(val & 0x4000) ? MSS_MASTER : MSS_SLAVE;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_SYS_INTF_CONN_STATUS);
	switch ((val >> 3) & 0x1f) {
	case 0x0:
		conn_status->sys_intf_current_mode = SIOM_BACKPLANE_KR;
		break;
	case 0x1:
		conn_status->sys_intf_current_mode = SIOM_BACKPLANE_KX;
		break;
	case 0x2:
		conn_status->sys_intf_current_mode = SIOM_XFI;
		break;
	case 0x4:
		conn_status->sys_intf_current_mode = SIOM_XAUI;
		break;
	case 0x5:
		conn_status->sys_intf_current_mode = SIOM_XAUI_PAUS_BASED;
		break;
	case 0x6:
		conn_status->sys_intf_current_mode = SIOM_SGMII;
		break;
	case 0x7:
		conn_status->sys_intf_current_mode = SIOM_RXAUI;
		break;
	case 0x8:
		conn_status->sys_intf_current_mode = SIOM_MAC;
		break;
	case 0x9:
		conn_status->sys_intf_current_mode = SIOM_OFF;
		break;
	default:
		conn_status->sys_intf_current_mode = SIOM_OTHER;
		break;
	}

	/* System interface operating mode */
	switch ((val >> 8) & 0xf) {
	case 0x0:
		conn_status->sys_intf_current_rate = SIR_10M;
		break;
	case 0x1:
		conn_status->sys_intf_current_rate = SIR_100M;
		break;
	case 0x2:
		conn_status->sys_intf_current_rate = SIR_1G;
		break;
	case 0x3:
		conn_status->sys_intf_current_rate = SIR_10G;
		break;
	case 0x6:
		conn_status->sys_intf_current_rate = SIR_POWER_DOWN;
		break;
	default:
		conn_status->sys_intf_current_rate = SIR_OTHER;
		break;
	}

	/* System interface autonegotiation status */
	switch ((val >> 14) & 3) {
	case 0x0:
		conn_status->sys_intf_autoneg_status = SIAS_NA;
		break;
	case 0x1:
		conn_status->sys_intf_autoneg_status = SIAS_NOT_COMPLETE;
		break;
	case 0x2:
		conn_status->sys_intf_autoneg_status = SIAS_COMPLETE;
		break;
	default:
		conn_status->sys_intf_autoneg_status = SIAS_OTHER;
		break;
	}

	conn_status->tx_ready = !!(val & 0x1000);

	conn_status->rx_link_up = !!(val & 0x2000);

	return 0;
}

/**
 * Restart the PHY's autonegotiation sequence
 *
 * @param phydev	PHY device
 *
 * @return	0 for success, error otherwise.
 */
int aq_phy_restart_autonegotiation(struct phy_device *phydev)
{
	int val;

	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1);
	val |= 0x200;	/* Restart autonegotation */
	val &= ~0x8000;	/* Don't accidentally trigger a reset */
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1, val);

	return aq_phy_wait_proc_busy(phydev, 100);
}

/**
 * Changes the PHYs status from inactive to active.
 *
 * @param phydev	PHY device
 *
 * @return		0 for success, error otherwise
 */
int aq_phy_set_active(struct phy_device *phydev)
{
	uint16_t val;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CONTROL_2);
	if (val & 1) {
		/* processor is stalled */
		val &= ~1;
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, val);
		val |= 0x40;
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, val);
	}

	/* Clear low power / high impedence states */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	val &= ~(1 << 13);	/* Turn off low-power mode */
	val &= ~(1 << 15);	/* Don't accidentally trigger reset */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, val);

	/* Put PMD transmit into normal operation mode */
	aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_TX_DISABLE_CONTROL, 0);

	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	val = aq_phy_read(phydev, 1, AQ_REG_PMA_10GBASE_T_TEST_MODE);
	val &= ~0xe000;
	aq_phy_write(phydev, 1, AQ_REG_PMA_10GBASE_T_TEST_MODE, val);

	/* Put PMD transmit into normal operation mode again */
	aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_TX_DISABLE_CONTROL, 0);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_STANDARD_XGXS_TEST_CONTROL);
	val &= ~(1 << 2);	/* Disable XAUI Tx test pattern */
	aq_phy_write(phydev, 4, AQ_REG_XS_STANDARD_XGXS_TEST_CONTROL, val);

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_DEBUG_1);
	val &= ~(1 << 10);	/* Disable test pattern checker */
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_DEBUG_1, val);

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_DEBUG_2);
	val &= ~0xf;	/* Disable XAUI channel PRBS test patterns */
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_DEBUG_2, val);

	val = aq_phy_read(phydev, 0x1d, AQ_REG_GBE_PHY_RESERVED_PROVISIONING_2);
	val &= ~0xe000;		/* Turn off test modes */
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_RESERVED_PROVISIONING_2, val);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	/* Put PMD transmit into normal operation mode again */
	aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_TX_DISABLE_CONTROL, 0);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	/* Clear loopbacks */
	/* PMA */
	val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL);
	val &= ~1;		/* Turn off loopback */
	val &= ~(1 << 15);	/* Turn off reset */
	aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL, val);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	/* PCS */
	val = aq_phy_read(phydev, 3, AQ_REG_PCS_STANDARD_CONTROL);
	val &= ~(1 << 14);	/* Turn off loopback */
	val &= ~(1 << 15);	/* Turn off reset */
	aq_phy_write(phydev, 3, AQ_REG_PMA_STANDARD_CONTROL, val);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	val = aq_phy_read(phydev, 3, AQ_REG_PCS_RX_VENDOR_DEBUG_1);
	val &= ~1;	/* Turn off PCS network loopback */
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_DEBUG_1, val);

	/* Phy XS */
	val = aq_phy_read(phydev, 4, AQ_REG_XS_STANDARD_CONTROL);
	val &= ~(1 << 14);	/* Turn off loopback */
	val &= ~(1 << 15);	/* Turn off reset */
	aq_phy_write(phydev, 4, AQ_REG_XS_STANDARD_CONTROL, val);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_TX_VENDOR_DEBUG_3);
	val &= ~(1 << 14);	/* Turn off PHY XS system loopback mode */
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_TX_VENDOR_DEBUG_3, val);
	if (aq_phy_wait_proc_busy(phydev, 100))
		return -1;

	if (aq_phy_set_fw_loopback_control(phydev, AQ_FWLC_NO_LOOPBACK,
					   AQ_FWLRATE_AUTO))
		return -1;

	/* Clear any WoL and disable the filters */
	val = aq_phy_read(phydev, 7,
			  AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1);
	val &= ~(1 << 6);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1,
		     val);

	val = aq_phy_read(phydev, 0x1d,
			  AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_DETECT_ENABLE);
	val &= ~(1 << 0);
	aq_phy_write(phydev, 0x1d,
		     AQ_REG_GBE_PHY_WOL_CONTROL_FRAME_DETECT_ENABLE, val);

	val = aq_phy_read(phydev, 0x1d,
			  AQ_REG_GBE_PHY_WOL_MAGIC_FRAME_DETECT_ENABLE);
	val &= ~(1 << 0);
	aq_phy_write(phydev, 0x1d,
		     AQ_REG_GBE_PHY_WOL_MAGIC_FRAME_DETECT_ENABLE, val);
	return 0;
}

/**
 * Get the current loopback control state of the phy
 * @param	phydev		phy device
 * @param[out]	loopback	loopback mode of phy
 * @param[out]	rate		loopback rate of phy
 *
 * @return 0 for success
 */
int aq_phy_get_fw_loopback_control(struct phy_device *phydev,
				   enum aq_phy_fw_loopback *loopback,
				   enum aq_phy_fw_loopback_rate *rate)
{
	int val;
	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_5);

	switch ((val >> 11) & 0x1f) {
	case 0x0:
		*loopback = AQ_FWLC_NO_LOOPBACK;
		break;
	case 0x1:
		*loopback = AQ_FWLC_SYS_INTF_SYS_LOOPBACK;
		break;
	case 0x2:
		*loopback = AQ_FWLC_SYS_INTF_SYS_LOOPBACK_PASSTHROUGH;
		break;
	case 0x3:
		*loopback = AQ_FWLC_SYS_INTF_NET_LOOPBACK;
		break;
	case 0x4:
		*loopback = AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH;
		break;
	case 0x5:
		*loopback = AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH_MERGE;
		break;
	case 0x9:
		*loopback = AQ_FWLC_NET_INTF_SYS_LOOPBACK;
		break;
	case 0xA:
		*loopback = AQ_FWLC_NET_INTF_SYS_LOOPBACK_PASSTHROUGH;
		break;
	case 0xB:
		*loopback = AQ_FWLC_NET_INTF_NET_LOOPBACK;
		break;
	case 0xC:
		*loopback = AQ_FWLC_NET_INTF_NET_LOOPBACK_PASSTHROUGH;
		break;
	case 0x10:
		*loopback = AQ_FWLC_CROSS_CONNECT_SYS_LOOPBACK;
		break;
	case 0x11:
		*loopback = AQ_FWLC_CROSS_CONNECT_NET_LOOPBACK;
		break;
	case 0x14:
		*loopback = AQ_FWLC_NET_INTF_SYS_PLUG_LOOPBACK;
		break;
	default:
		*loopback = AQ_FWLC_OTHER;
		break;
	}

	switch (val & 7) {
	case 0:
		*rate = AQ_FWLRATE_AUTO;
		break;
	case 3:
		*rate = AQ_FWLRATE_10G;
		break;
	case 2:
		*rate = AQ_FWLRATE_1G;
		break;
	case 1:
		*rate = AQ_FWLRATE_100M;
		break;
	default:
		*rate = AQ_FWLRATE_OTHER;
	}
	return 0;
}

/**
 * Sets the phy loopback mode
 *
 * @param	phydev		phy device
 * @param	loopback	loopback mode to use
 * @param	rate		loopback rate to use
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_set_fw_loopback_control(struct phy_device *phydev,
				   enum aq_phy_fw_loopback loopback,
				   enum aq_phy_fw_loopback_rate rate)
{
	int val;

	val = aq_phy_read(phydev, 4,
			  AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_5);
	val &= 0x03f8;
	switch (loopback) {
	default:
	case AQ_FWLC_NO_LOOPBACK:
		val |= (0 << 11);
		break;
	case AQ_FWLC_SYS_INTF_SYS_LOOPBACK:
		val |= (1 << 11);
		break;
	case AQ_FWLC_SYS_INTF_SYS_LOOPBACK_PASSTHROUGH:
		val |= (2 << 11);
		break;
	case AQ_FWLC_SYS_INTF_NET_LOOPBACK:
		val |= (3 << 11);
		break;
	case AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH:
		val |= (4 << 11);
		break;
	case AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH_MERGE:
		val |= (5 << 11);
		break;
	case AQ_FWLC_NET_INTF_SYS_LOOPBACK:
		val |= (9 << 11);
		break;
	case AQ_FWLC_NET_INTF_SYS_LOOPBACK_PASSTHROUGH:
		val |= (0xa << 11);
		break;
	case AQ_FWLC_NET_INTF_NET_LOOPBACK:
		val |= (0xb << 11);
		break;
	case AQ_FWLC_NET_INTF_NET_LOOPBACK_PASSTHROUGH:
		val |= (0xc << 11);
		break;
	case AQ_FWLC_CROSS_CONNECT_SYS_LOOPBACK:
		val |= (0x10 << 11);
		break;
	case AQ_FWLC_CROSS_CONNECT_NET_LOOPBACK:
		val |= (0x11 << 11);
		break;
	case AQ_FWLC_NET_INTF_SYS_PLUG_LOOPBACK:
		val |= (0x14 << 11);
		break;
	}

	switch (rate) {
	case AQ_FWLRATE_AUTO:
	default:
		val |= 0;
		break;
	case AQ_FWLRATE_10G:
		val |= 3;
		break;
	case AQ_FWLRATE_1G:
		val |= 2;
		break;
	case AQ_FWLRATE_100M:
		val |= 1;
		break;
	}

	aq_phy_write(phydev, 4,
		     AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_5, val);

	return aq_phy_wait_proc_busy(phydev, 100);
}

/**
 * Get the current loopback setting
 *
 * @param	phydev		PHY device to get loopback mode
 * @param[out]	loopback	Current loopback mode
 *
 * @return	0 for success
 */
int aq_phy_get_loopback_control(struct phy_device *phydev,
				enum aq_phy_loopback_control *loopback)
{
	uint16_t pma_val;
	uint16_t xs_val;

	pma_val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL);
	xs_val = aq_phy_read(phydev, 1, AQ_REG_XS_STANDARD_CONTROL);
	if (pma_val & 1)
		*loopback = AQ_PHY_NET_INTF_SYSTEM_LOOPBACK;
	else if (xs_val & 0x4000)
		*loopback = AQ_PHY_SYS_INTF_NETWORK_LOOPBACK;
	else
		*loopback = AQ_PHY_NO_LOOPBACK;

	return 0;
}

/**
 * Sets up loopbacks within the Aquantia PHY.
 *
 * @param phydev	PHY device to configure loopback for
 * @param loopback	Loopback mode to use
 *
 * @return	0 for success, -1 on error
 *
 * NOTE: The link must be established before calling this function.
 */
int aq_phy_set_loopback_control(struct phy_device *phydev,
				enum aq_phy_loopback_control loopback)
{
	struct aq_phy_conn_status conn_status;
	uint16_t val;

	/* Clear any loopback that may already be set */
	/* PMA */
	val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL);
	/* Turn off loopback and reset */
	val &= ~0x8001;
	aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL, val);

	aq_phy_wait_proc_busy(phydev, AQ_PHY_UP_TIMEOUT_MS);

	/* PHY XS */;
	val = aq_phy_read(phydev, 4, AQ_REG_XS_STANDARD_CONTROL);
	val &= ~0xc000;
	aq_phy_write(phydev, 4, AQ_REG_XS_STANDARD_CONTROL, val);

	aq_phy_wait_proc_busy(phydev, AQ_PHY_UP_TIMEOUT_MS);

	aq_phy_get_connection_status(phydev, &conn_status);

	switch (loopback) {
	case AQ_PHY_NO_LOOPBACK:
		break;
	case AQ_PHY_SYS_INTF_NETWORK_LOOPBACK:
		switch (conn_status.state) {
		case CONS_10G_CONNECTED:
		case CONS_1G_CONNECTED:
		case CONS_100M_CONNECTED:
			val = aq_phy_read(phydev, 4, AQ_REG_XS_STANDARD_CONTROL);
			val |= 0x4000;
			val &= ~0x8000;
			aq_phy_write(phydev, 4, AQ_REG_XS_STANDARD_CONTROL, val);
			aq_phy_wait_proc_busy(phydev, AQ_PHY_UP_TIMEOUT_MS);
			break;
		default:
			return -1;
		}
		break;
	case AQ_PHY_NET_INTF_SYSTEM_LOOPBACK:
		switch (conn_status.state) {
		case CONS_10G_CONNECTED:
		case CONS_1G_CONNECTED:
		case CONS_100M_CONNECTED:
			val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL);
			val |= 1;
			val &= ~0x8000;
			aq_phy_write(phydev, 1, AQ_REG_PMA_STANDARD_CONTROL, val);
			aq_phy_wait_proc_busy(phydev, AQ_PHY_UP_TIMEOUT_MS);
		default:
			return -1;
		}
		break;
	}
	return 0;
}

/**
 * Read any alarm status information if any alarms are set
 *
 * @param	phydev		phy device to get status from
 * @param	status		current alarm status
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_alarm_status(struct phy_device *phydev,
			    struct aq_phy_alarm_status *status)
{
	uint16_t chip_s, chip_v = 0, val;
	uint16_t vend_gbl_autoneg = 0;
	uint16_t vend_gbl_phyxs = 0;
	uint16_t vend_gbl_pcs = 0, vend_gbl_pcs_3;
	uint16_t vend_gbl_pma = 0;
	uint16_t vend_gbl_gbe = 0;

	memset(status, 0, sizeof(*status));

	status->device_fault_code = 0;

	/* The global chip-wide standard interrupt flag bits are:
	 * f:	Interrupt in PMA standard alarms 1		(see 1.d000)
	 * e:	Interrupt in PMA standard alarms 2		(see 1.d001)
	 * d:	Interrupt in PCS standard alarms 1		(see 3.d000)
	 * c:	Interrupt in PCS standard alarms 2		(see 3.d001)
	 * b:	Interrupt in PCS standard alarms 3		(see 3.d002)
	 * a:	Interrupt in PHY XS standard alarms 1		(see 4.d000)
	 * 9:	Interrupt in PHY XS standard alarms 2		(see 4.d001)
	 * 8:	Interrupt in Autonegotiation standard alarms 1	(see 7.d000)
	 * 7:	Interrupt in Autonegotiation standard alarms 2	(see 7.d001)
	 * 6:	Interrupt in GbE standard alarms
	 * 0:	Interrupt in all vendor alarms (see chip_v, 1e.fc01)
	 */
	chip_s = aq_phy_read(phydev, 0x1e,
			     AQ_REG_GBL_CHIPWIDE_STANDARD_INT_FLAGS);

	/* The global chip-wide vendor interrupt flag bits are:
	 * f:	Interrupt in PMA vendor specific alarm		(see 1.fc00)
	 * e:	Interrupt in PCS vendor specific alarm		(see 3.fc00)
	 * d:	Interrupt in PHY XS vendor specific alarm	(see 4.fc00)
	 * c:	Interrupt in Autonegotiation vendor specific alarm  (see 7.fc00)
	 * b:	Interrupt in GbE vendor specific alarm	(see 1d.fc00)
	 * 2:	Interrupt in Global alarms 1	(see 1e.d400)
	 * 1:	Interrupt in Global alarms 2	(see 1e.d401)
	 * 0:	Interrupt in Global alarms 3	(see 1e.d402)
	 */
	if (chip_s & 1)
		chip_v = aq_phy_read(phydev, 0x1e,
				     AQ_REG_GBL_CHIPWIDE_VENDOR_INT_FLAGS);

	/* Autonegotiation alarms */
	if (chip_s & ((1 << 7) | (1 << 8)))
		vend_gbl_autoneg = aq_phy_read(phydev, 7,
					       AQ_REG_AUTONEG_VENDOR_GLOBAL_INT_FLAGS);

	if (chip_s & (1 << 8)) {
		if (vend_gbl_autoneg & (1 << 15)) {
			val = aq_phy_read(phydev, 7,
					  AQ_REG_AUTONEG_STANDARD_STATUS_1);
			if ((val & (1 << 2)) == 0)	/* Link is down */
				status->alarms_autoneg.link_fail = true;
			if (val & (1 << 4))		/* remote fault */
				status->alarms_autoneg.remote_fault = true;
		}
	}
	if (chip_s & (1 << 7)) {
		if (vend_gbl_autoneg & (1 << 14)) {
			val = aq_phy_read(phydev, 7,
					  AQ_REG_AUTONEG_10GBASE_T_STATUS);

			status->alarms_autoneg.master_slave_config_fault =
							!!(val & (1 << 15));
		}
	}

	/* PHY XS standard alarms */
	if (chip_s & ((1 << 9) | (1 << 10)))
		vend_gbl_phyxs = aq_phy_read(phydev, 4,
					     AQ_REG_XS_VENDOR_GLOBAL_INT_FLAGS);
	if (chip_s & (1 << 10)) {
		if (vend_gbl_phyxs & (1 << 15)) {
			val = aq_phy_read(phydev, 4, AQ_REG_PMA_STANDARD_STATUS);
			if (!(val & (1 << 2)))
				status->alarms_xaui.loss_of_lane_alignment = true;
		}
	}

	if (chip_s & (1 << 9)) {
		if (vend_gbl_phyxs & (1 << 14)) {
			val = aq_phy_read(phydev, 4,
					  AQ_REG_XS_STANDARD_STATUS_2);
			if (val & (1 << 10))
				status->alarms_xaui.phy_xs_rx_fault = true;
			if (val & (1 << 11))
				status->alarms_xaui.phy_xs_tx_fault = true;
		}
	}

	/* PHY PCS standard alarms */
	if (chip_s & ((1 << 11) | (1 << 12) | (1 << 13)))
		vend_gbl_pcs = aq_phy_read(phydev, 3,
					   AQ_REG_PCS_VENDOR_GLOBAL_INT_FLAGS);

	if (chip_s & (1 << 13)) {
		if (vend_gbl_pcs & (1 << 15)) {
			val = aq_phy_read(phydev, 3, AQ_REG_PCS_STANDARD_STATUS);
			if (val & (1 << 2))
				status->alarms_10gbase_t.pcs_link_fail = true;
		}
	}
	if (chip_s & (1 << 12)) {
		if (vend_gbl_pcs & (1 << 14)) {
			val = aq_phy_read(phydev, 3,
					  AQ_REG_PCS_STANDARD_STATUS_2);
			status->alarms_10gbase_t.pcs_rx_fault = !!(val & (1 << 10));
			status->alarms_10gbase_t.pcs_tx_fault = !!(val & (1 << 11));
		}
	}

	if (chip_s & (1 << 11)) {
		if (vend_gbl_pcs & (1 << 13)) {
			val = aq_phy_read(phydev, 3, AQ_REG_PCS_10G_STATUS_2);
			status->alarms_10gbase_t.high_ber = !!(val & (1 << 14));
			status->alarms_10gbase_t.loss_of_block_lock =
							!!(val & (1 << 15));
		}
	}

	/* PMA standard alarms */
	if (chip_s & ((1 << 14) | (1 << 15)))
		vend_gbl_pma = aq_phy_read(phydev, 1,
					   AQ_REG_PMA_VENDOR_GLOBAL_INT_FLAGS);

	if (chip_s & (1 << 15)) {
		if (vend_gbl_pma & (1 << 11)) {
			val = aq_phy_read(phydev, 1, AQ_REG_PMA_STANDARD_STATUS);
			if (val & (1 << 2))
				status->alarms_10gbase_t.pma_link_fail = true;
		}
	}
	if (chip_s & (1 << 14)) {
		if (vend_gbl_pma & (1 << 10)) {
			val = aq_phy_read(phydev, 1,
					  AQ_REG_PMA_STANDARD_STATUS_2);
			if (val & (1 << 10))
				status->alarms_10gbase_t.pma_rx_fault = true;
			if (val & (1 << 11))
				status->alarms_10gbase_t.pma_tx_fault = true;
		}
	}

	/* Vendor interrupts */

	/* Global alarms 1 */
	if (chip_v & (1 << 2)) {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_ALARM_1);
		if (val & (1 << 6))
			status->alarms_global.reset_complete = true;
		if (val & (1 << 4)) {
			status->alarms_global.device_fault = true;
			status->device_fault_code =
				aq_phy_read(phydev, 0x1e,
					    AQ_REG_GBL_FAULT_MESSAGE);
		}
		if (val & (1 << 14))
			status->alarms_global.high_temp_failure = true;
		if (val & (1 << 12))
			status->alarms_global.high_temp_warning = true;
		if (val & (1 << 13))
			status->alarms_global.low_temp_failure = true;
		if (val & (1 << 11))
			status->alarms_global.low_temp_warning = true;
	}
	/* Global alarms 2 */
	if (chip_v & (1 << 1)) {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_ALARM_2);
		if (val & (1 << 7))
			status->alarms_global.mdio_command_overflow = true;
		if (val & (1 << 14))
			status->alarms_global.smart_power_down = true;
		if (val & (1 << 12))
			status->alarms_global.ip_phone = true;
	}
	/* Global alarms 3 */
	if (chip_v & (1 << 0)) {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_ALARM_3);
		if (val & (1 << 1))
			status->alarms_global.mdio_timeout = true;
		if (val & (1 << 0))
			status->alarms_global.watchdog_timer = true;
	}

	/* GbE vendor alarms */
	if (chip_v & (1 << 11)) {
		vend_gbl_gbe = aq_phy_read(phydev, 0x1d,
					   AQ_REG_GBE_VENDOR_GLOBAL_INT_FLAGS);
		if (vend_gbl_gbe & (1 << 4)) {
			val = aq_phy_read(phydev, 0x1d,
					  AQ_REG_GBE_PHY_SGMII_TX_ALARMS);
			if (val & ((1 << 4) | (1 << 0)))
				status->alarms_autoneg.detected_wol_frame = true;
			if (val & ((1 << 5) | (1 << 1)))
				status->alarms_autoneg.detected_wol_packet = true;
		}
		if (vend_gbl_gbe & (1 << 2)) {
			val = aq_phy_read(phydev, 0x1d,
					  AQ_REG_GBE_PHY_SGMII_RX_ALARMS);
			if (val & (1 << 4))
				status->alarms_sgmii0.code_violation_error = true;
			if (val & (1 << 5))
				status->alarms_sgmii0.running_disparity_error = true;
			if (val & (1 << 7))
				status->alarms_sgmii0.loss_of_signal = true;
			if (val & (1 << 0))
				status->alarms_sgmii1.code_violation_error = true;
			if (val & (1 << 1))
				status->alarms_sgmii1.running_disparity_error = true;
			if (val & (1 << 3))
				status->alarms_sgmii1.loss_of_signal = true;
		}
	}
	if (chip_v & (1 << 12)) {
		vend_gbl_autoneg = aq_phy_read(phydev, 7,
					       AQ_REG_AUTONEG_VENDOR_GLOBAL_INT_FLAGS);
		if (vend_gbl_autoneg & (1 << 10)) {
			val = aq_phy_read(phydev, 7,
					  AQ_REG_AUTONEG_TX_VENDOR_ALARMS_1);
			if (val & (1 << 0))
				status->alarms_global.state_change = true;
			if (val & (1 << 1))
				status->alarms_autoneg.auto_downshift = true;
			if (val & (1 << 2))
				status->alarms_autoneg.autonegotation_completed_for_supported_rate = true;
			if (val & (1 << 3))
				status->alarms_autoneg.autoneg_completed_for_unsupported_rate = true;
		}
		if (vend_gbl_autoneg & (1 << 9)) {
			val = aq_phy_read(phydev, 7,
					  AQ_REG_AUTONEG_TX_VENDOR_ALARMS_2);
			if (val & (1 << 0))
				status->alarms_autoneg.link_status_change = true;
			if (val & (1 << 1)) {
				val = aq_phy_read(phydev, 7,
						  AQ_REG_AUTONEG_RX_VENDOR_ALARMS_3);
				if (val & (1 << 2))
					status->alarms_autoneg.detected_10base_t = true;
			}
		}
	}

	/* phyXS vendor alarms */
	if (chip_v & (1 << 13)) {
		vend_gbl_phyxs = aq_phy_read(phydev, 4,
					     AQ_REG_XS_VENDOR_GLOBAL_INT_FLAGS);
		if (vend_gbl_phyxs & (1 << 10)) {
			val = aq_phy_read(phydev, 4,
					  AQ_REG_XS_XAUI_RX_VENDOR_ALARMS_1);
			if (val & (1 << 11))
				status->alarms_test.xaui_prbs_loss_of_sync[0] = true;
			if (val & (1 << 12))
				status->alarms_test.xaui_prbs_loss_of_sync[1] = true;
			if (val & (1 << 13))
				status->alarms_test.xaui_prbs_loss_of_sync[2] = true;
			if (val & (1 << 14))
				status->alarms_test.xaui_prbs_loss_of_sync[3] = true;
			if (val & (1 << 15))
				status->alarms_test.crpat_loss_of_sync = true;
		}
		if (vend_gbl_phyxs & (1 << 9)) {
			val = aq_phy_read(phydev, 4,
					  AQ_REG_XS_XAUI_RX_VENDOR_ALARMS_2);
			if (val & (1 << 0))
				status->alarms_xaui.running_disparity_error[0] = true;
			if (val & (1 << 1))
				status->alarms_xaui.running_disparity_error[1] = true;
			if (val & (1 << 2))
				status->alarms_xaui.running_disparity_error[2] = true;
			if (val & (1 << 3))
				status->alarms_xaui.running_disparity_error[3] = true;
			if (val & (1 << 4))
				status->alarms_xaui.code_violation_error[0] = true;
			if (val & (1 << 5))
				status->alarms_xaui.code_violation_error[1] = true;
			if (val & (1 << 6))
				status->alarms_xaui.code_violation_error[2] = true;
			if (val & (1 << 7))
				status->alarms_xaui.code_violation_error[3] = true;
			if (val & (1 << 13))
				status->alarms_xaui.sequence_ordered_set_deletion = true;
		}
		if (vend_gbl_phyxs & (1 << 8)) {
			val = aq_phy_read(phydev, 4,
					  AQ_REG_XS_XAUI_RX_VENDOR_ALARMS_3);
			if (val & (1 << 12))
				status->alarms_xaui.loss_of_signal[0] = true;
			if (val & (1 << 13))
				status->alarms_xaui.loss_of_signal[1] = true;
			if (val & (1 << 14))
				status->alarms_xaui.loss_of_signal[2] = true;
			if (val & (1 << 15))
				status->alarms_xaui.loss_of_signal[3] = true;
		}
		if (vend_gbl_phyxs & (1 << 1)) {
			val = aq_phy_read(phydev, 4,
					  AQ_REG_XS_XAUI_TX_VENDOR_ALARMS_2);
			if (val & (1 << 14))
				status->alarms_serdes.sys_intf_rx_link_down = true;
			if (val & (1 << 15))
				status->alarms_serdes.sys_intf_rx_link_up = true;
		}
	}
	/* PCS vendor alarms */
	if (chip_v & (1 << 14)) {
		vend_gbl_pcs_3 = aq_phy_read(phydev, 3,
					     AQ_REG_PCS_VENDOR_GLOBAL_INT_FLAGS_3);
		if (vend_gbl_pcs & (1 << 6)) {
			val = aq_phy_read(phydev, 3,
					  AQ_REG_PCS_RX_VENDOR_ALARMS_1);
			if (val & (1 << 8))
				status->alarms_10gbase_t.invalid_65b_block = true;
			if (val & (1 << 14))
				status->alarms_10gbase_t.ldpc_error = true;
			if (val & (1 << 15))
				status->alarms_10gbase_t.crc8_error = true;
		}
		if (vend_gbl_pcs_3 & (1 << 7)) {
			val = aq_phy_read(phydev, 3,
					  AQ_REG_PCS_RX_VENDOR_ALARMS_6);
			if (val & (1 << 3))
				status->alarms_xfi0.invalid_66b_block = true;
			if (val & (1 << 11))
				status->alarms_xfi1.invalid_66b_block = true;
		}
		if (vend_gbl_pcs_3 & (1 << 6)) {
			val = aq_phy_read(phydev, 3,
					  AQ_REG_PCS_RX_VENDOR_ALARMS_7);
			if (val & (1 << 0))
				status->alarms_xfi0.loss_of_block_lock = true;
			if (val & (1 << 1))
				status->alarms_xfi0.high_ber = true;
			if (val & (1 << 4))
				status->alarms_xfi1.loss_of_block_lock = true;
			if (val & (1 << 5))
				status->alarms_xfi1.high_ber = true;
		}
	}

	/* Set the summary alarms */
	if (status->alarms_autoneg.link_fail ||
	    status->alarms_autoneg.remote_fault ||
	    status->alarms_autoneg.master_slave_config_fault ||
	    status->alarms_autoneg.detected_wol_frame ||
	    status->alarms_autoneg.detected_wol_packet ||
	    status->alarms_autoneg.auto_downshift ||
	    status->alarms_autoneg.autonegotation_completed_for_supported_rate ||
	    status->alarms_autoneg.autoneg_completed_for_unsupported_rate ||
	    status->alarms_autoneg.link_status_change ||
	    status->alarms_autoneg.detected_10base_t)
		status->alarm_autoneg_set = true;

	if (status->alarms_10gbase_t.pcs_link_fail ||
	    status->alarms_10gbase_t.pcs_rx_fault ||
	    status->alarms_10gbase_t.pcs_tx_fault ||
	    status->alarms_10gbase_t.high_ber ||
	    status->alarms_10gbase_t.loss_of_block_lock ||
	    status->alarms_10gbase_t.pma_link_fail ||
	    status->alarms_10gbase_t.pma_rx_fault ||
	    status->alarms_10gbase_t.pma_tx_fault ||
	    status->alarms_10gbase_t.invalid_65b_block ||
	    status->alarms_10gbase_t.ldpc_error ||
	    status->alarms_10gbase_t.crc8_error)
		status->alarm_10gbase_t_set = true;

	if (status->alarms_xaui.loss_of_lane_alignment ||
	    status->alarms_xaui.phy_xs_rx_fault ||
	    status->alarms_xaui.phy_xs_tx_fault ||
	    status->alarms_xaui.running_disparity_error[0] ||
	    status->alarms_xaui.running_disparity_error[1] ||
	    status->alarms_xaui.running_disparity_error[2] ||
	    status->alarms_xaui.running_disparity_error[3] ||
	    status->alarms_xaui.code_violation_error[0] ||
	    status->alarms_xaui.code_violation_error[1] ||
	    status->alarms_xaui.code_violation_error[2] ||
	    status->alarms_xaui.code_violation_error[3] ||
	    status->alarms_xaui.sequence_ordered_set_deletion ||
	    status->alarms_xaui.loss_of_signal[0] ||
	    status->alarms_xaui.loss_of_signal[1] ||
	    status->alarms_xaui.loss_of_signal[2] ||
	    status->alarms_xaui.loss_of_signal[3])
		status->alarm_xaui_set = true;

	if (status->alarms_sgmii0.code_violation_error ||
	    status->alarms_sgmii0.running_disparity_error ||
	    status->alarms_sgmii0.loss_of_signal)
		status->alarm_sgmii0_set = true;

	if (status->alarms_sgmii1.code_violation_error ||
	    status->alarms_sgmii1.running_disparity_error ||
	    status->alarms_sgmii1.loss_of_signal)
		status->alarm_sgmii1_set = true;

	if (status->alarms_global.reset_complete ||
	    status->alarms_global.device_fault ||
	    status->alarms_global.ip_phone ||
	    status->alarms_global.mdio_command_overflow ||
	    status->alarms_global.mdio_timeout ||
	    status->alarms_global.smart_power_down ||
	    status->alarms_global.watchdog_timer ||
	    status->alarms_global.high_temp_failure ||
	    status->alarms_global.high_temp_warning ||
	    status->alarms_global.low_temp_failure ||
	    status->alarms_global.low_temp_warning ||
	    status->alarms_global.state_change)
		status->alarm_global_set = true;

	if (status->alarms_xfi0.invalid_66b_block ||
	    status->alarms_xfi0.loss_of_block_lock ||
	    status->alarms_xfi0.high_ber)
		status->alarm_xfi0_set = true;

	if (status->alarms_xfi1.invalid_66b_block ||
	    status->alarms_xfi1.loss_of_block_lock ||
	    status->alarms_xfi1.high_ber)
		status->alarm_xfi1_set = true;

	if (status->alarms_serdes.sys_intf_rx_link_down ||
	    status->alarms_serdes.sys_intf_rx_link_up)
		status->alarm_serdes_set = true;

	return 0;
}

/**
 * Find out which alarms are enabled
 *
 * @param	phydev	PHY device to get alarms from
 * @param[out]	control	Alarm data
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_alarm_control(struct phy_device *phydev,
			     struct aq_phy_alarm_control *control)
{
#warning TODO
	return 0;
}

/**
 * Enables various alarms to generate interrupts
 *
 * @param	phydev		phy device to configure
 * @param	control		which alarms to enable
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_set_alarm_control(struct phy_device *phydev,
			     const struct aq_phy_alarm_control *control)
{
	uint16_t autoneg_standard_int_mask[2] = {0, 0};
	uint16_t phyxs_tx_xaui_rx_standard_int_mask[2] = {0, 0};
	uint16_t pcs_standard_int_mask[3] = {0, 0, 0};
	uint16_t pma_tx_standard_int_mask[2] = {0, 0};
	uint16_t global_int_mask[3] = {0, 0, 0};
	uint16_t gbe_phy_tx_int_mask = 0;
	uint16_t gbe_phy_rx_int_mask = 0;
	uint16_t autoneg_tx_vendor_int_mask[3] = {0, 0, 0};
	uint16_t autoneg_rx_vendor_int_mask[4] = {0, 0, 0, 0};
	uint16_t phyxs_tx_xaui_rx_vendor_int_mask[3] = {0, 0, 0};
	uint16_t phyxs_rx_xaui_tx_vendor_int_mask[2] = {0, 0};
	uint16_t pcs_tx_vendor_int_mask[4] = {0, 0, 0, 0};
	uint16_t pcs_rx_vendor_int_mask[7] = {0, 0, 0, 0, 0, 0, 0};
	uint16_t gbl_chip_wide_vendor_int_mask = 0;

	if (control->alarm_mask_autoneg.link_fail)
		autoneg_standard_int_mask[0] |= (1 << 2);
	if (control->alarm_mask_autoneg.remote_fault)
		autoneg_standard_int_mask[0] |= (1 << 4);
	if (control->alarm_mask_autoneg.master_slave_config_fault)
		autoneg_standard_int_mask[1] |= (1 << 15);
	if (control->alarm_mask_xaui.loss_of_lane_alignment)
		phyxs_tx_xaui_rx_standard_int_mask[0] |= (1 << 2);
	if (control->alarm_mask_xaui.phy_xs_rx_fault)
		phyxs_tx_xaui_rx_standard_int_mask[1] |= (1 << 10);
	if (control->alarm_mask_xaui.phy_xs_tx_fault)
		phyxs_tx_xaui_rx_standard_int_mask[1] |= (1 << 11);
	if (control->alarm_mask_10gbase_t.pcs_link_fail)
		pcs_standard_int_mask[0] |= (1 << 2);
	if (control->alarm_mask_10gbase_t.pcs_rx_fault)
		pcs_standard_int_mask[1] |= (1 << 10);
	if (control->alarm_mask_10gbase_t.pcs_tx_fault)
		pcs_standard_int_mask[1] |= (1 << 11);
	if (control->alarm_mask_10gbase_t.high_ber)
		pcs_standard_int_mask[2] |= (1 << 14);
	if (control->alarm_mask_10gbase_t.loss_of_block_lock)
		pcs_standard_int_mask[2] |= (1 << 15);
	if (control->alarm_mask_10gbase_t.pma_link_fail)
		pma_tx_standard_int_mask[0] |= (1 << 2);
	if (control->alarm_mask_10gbase_t.pma_rx_fault)
		pma_tx_standard_int_mask[1] |= (1 << 10);
	if (control->alarm_mask_10gbase_t.pma_tx_fault)
		pma_tx_standard_int_mask[1] |= (1 << 11);
	if (control->alarm_mask_global.reset_complete)
		global_int_mask[0] |= (1 << 6);
	if (control->alarm_mask_global.device_fault)
		global_int_mask[0] |= (1 << 4);
	if (control->alarm_mask_global.ip_phone)
		global_int_mask[1] |= (1 << 12);
	if (control->alarm_mask_global.mdio_command_overflow)
		global_int_mask[1] |= (1 << 7);
	if (control->alarm_mask_global.mdio_timeout)
		global_int_mask[2] |= (1 << 1);
	if (control->alarm_mask_global.smart_power_down)
		global_int_mask[1] |= (1 << 14);
	if (control->alarm_mask_global.watchdog_timer)
		global_int_mask[2] |= (1 << 0);
	if (control->alarm_mask_global.high_temp_failure)
		global_int_mask[0] |= (1 << 14);
	if (control->alarm_mask_global.high_temp_warning)
		global_int_mask[0] |= (1 << 12);
	if (control->alarm_mask_global.low_temp_failure)
		global_int_mask[0] |= (1 << 13);
	if (control->alarm_mask_global.low_temp_warning)
		global_int_mask[0] |= (1 << 11);
	if (control->alarm_mask_sgmii0.code_violation_error)
		gbe_phy_rx_int_mask |= (1 << 4);
	if (control->alarm_mask_sgmii0.running_disparity_error)
		gbe_phy_rx_int_mask |= (1 << 5);
	if (control->alarm_mask_sgmii0.loss_of_signal)
		gbe_phy_rx_int_mask |= (1 << 7);
	if (control->alarm_mask_sgmii1.code_violation_error)
		gbe_phy_rx_int_mask |= (1 << 0);
	if (control->alarm_mask_sgmii1.running_disparity_error)
		gbe_phy_rx_int_mask |= (1 << 1);
	if (control->alarm_mask_sgmii1.loss_of_signal)
		gbe_phy_rx_int_mask |= (1 << 3);
	if (control->alarm_mask_autoneg.detected_wol_frame)
		gbe_phy_tx_int_mask |= (1 << 4) | (1 << 0);
	if (control->alarm_mask_autoneg.detected_wol_packet)
		gbe_phy_tx_int_mask |= (1 << 5) | (1 << 1);
	if (control->alarm_mask_global.state_change)
		autoneg_tx_vendor_int_mask[0] |= (1 << 0);
	if (control->alarm_mask_autoneg.auto_downshift)
		autoneg_tx_vendor_int_mask[0] |= (1 << 1);
	if (control->alarm_mask_autoneg.autonegotation_completed_for_supported_rate)
		autoneg_tx_vendor_int_mask[0] |= (1 << 2);
	if (control->alarm_mask_autoneg.autoneg_completed_for_unsupported_rate)
		autoneg_tx_vendor_int_mask[0] |= (1 << 3);
	if (control->alarm_mask_autoneg.link_status_change)
		autoneg_tx_vendor_int_mask[1] |= (1 << 0);
	if (control->alarm_mask_autoneg.detected_10base_t)
		autoneg_rx_vendor_int_mask[2] |= (1 << 2);
	if (control->alarm_mask_test.xaui_prbs_loss_of_sync[0])
		phyxs_tx_xaui_rx_vendor_int_mask[0] |= (1 << 11);
	if (control->alarm_mask_test.xaui_prbs_loss_of_sync[1])
		phyxs_tx_xaui_rx_vendor_int_mask[0] |= (1 << 12);
	if (control->alarm_mask_test.xaui_prbs_loss_of_sync[2])
		phyxs_tx_xaui_rx_vendor_int_mask[0] |= (1 << 13);
	if (control->alarm_mask_test.xaui_prbs_loss_of_sync[3])
		phyxs_tx_xaui_rx_vendor_int_mask[0] |= (1 << 14);
	if (control->alarm_mask_test.crpat_loss_of_sync)
		phyxs_tx_xaui_rx_vendor_int_mask[0] |= (1 << 15);
	if (control->alarm_mask_xaui.running_disparity_error[0])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 0);
	if (control->alarm_mask_xaui.running_disparity_error[1])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 1);
	if (control->alarm_mask_xaui.running_disparity_error[2])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 2);
	if (control->alarm_mask_xaui.running_disparity_error[3])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 3);
	if (control->alarm_mask_xaui.code_violation_error[0])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 4);
	if (control->alarm_mask_xaui.code_violation_error[1])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 5);
	if (control->alarm_mask_xaui.code_violation_error[2])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 6);
	if (control->alarm_mask_xaui.code_violation_error[3])
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 7);
	if (control->alarm_mask_xaui.sequence_ordered_set_deletion)
		phyxs_tx_xaui_rx_vendor_int_mask[1] |= (1 << 14);
	if (control->alarm_mask_xaui.loss_of_signal[0])
		phyxs_tx_xaui_rx_vendor_int_mask[2] |= (1 << 12);
	if (control->alarm_mask_xaui.loss_of_signal[1])
		phyxs_tx_xaui_rx_vendor_int_mask[2] |= (1 << 13);
	if (control->alarm_mask_xaui.loss_of_signal[2])
		phyxs_tx_xaui_rx_vendor_int_mask[2] |= (1 << 14);
	if (control->alarm_mask_xaui.loss_of_signal[3])
		phyxs_tx_xaui_rx_vendor_int_mask[2] |= (1 << 15);
	if (control->alarm_mask_10gbase_t.invalid_65b_block)
		pcs_rx_vendor_int_mask[0] |= (1 << 8);
	if (control->alarm_mask_10gbase_t.ldpc_error)
		pcs_rx_vendor_int_mask[0] |= (1 << 14);
	if (control->alarm_mask_10gbase_t.crc8_error)
		pcs_rx_vendor_int_mask[0] |= (1 << 15);
	if (control->alarm_mask_xfi0.invalid_66b_block)
		pcs_rx_vendor_int_mask[5] |= (1 << 3);
	if (control->alarm_mask_xfi0.loss_of_block_lock)
		pcs_rx_vendor_int_mask[6] |= (1 << 0);
	if (control->alarm_mask_xfi0.high_ber)
		pcs_rx_vendor_int_mask[6] |= (1 << 1);
	if (control->alarm_mask_xfi1.invalid_66b_block)
		pcs_rx_vendor_int_mask[5] |= (1 << 11);
	if (control->alarm_mask_xfi1.loss_of_block_lock)
		pcs_rx_vendor_int_mask[6] |= (1 << 4);
	if (control->alarm_mask_xfi1.high_ber)
		pcs_rx_vendor_int_mask[6] |= (1 << 5);
	if (control->alarm_mask_serdes.sys_intf_rx_link_down)
		phyxs_rx_xaui_tx_vendor_int_mask[1] |= (1 << 14);
	if (control->alarm_mask_serdes.sys_intf_rx_link_up)
		phyxs_rx_xaui_tx_vendor_int_mask[1] |= (1 << 15);
	if (control->alarm_mask_global.reset_complete ||
	    control->alarm_mask_global.device_fault ||
	    control->alarm_mask_global.mdio_timeout ||
	    control->alarm_mask_global.watchdog_timer ||
	    control->alarm_mask_global.high_temp_failure ||
	    control->alarm_mask_global.high_temp_warning ||
	    control->alarm_mask_global.low_temp_failure ||
	    control->alarm_mask_global.low_temp_warning)
		gbl_chip_wide_vendor_int_mask |= (1 << 2);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 2);

	if (control->alarm_mask_global.ip_phone ||
	    control->alarm_mask_global.smart_power_down ||
	    control->alarm_mask_global.mdio_command_overflow)
		gbl_chip_wide_vendor_int_mask |= (1 << 1);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 1);

	if (control->alarm_mask_global.mdio_timeout ||
	    control->alarm_mask_global.watchdog_timer)
		gbl_chip_wide_vendor_int_mask |= (1 << 0);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 0);

	if (control->alarm_mask_sgmii0.code_violation_error ||
	    control->alarm_mask_sgmii0.running_disparity_error ||
	    control->alarm_mask_sgmii0.loss_of_signal ||
	    control->alarm_mask_sgmii1.code_violation_error ||
	    control->alarm_mask_sgmii1.running_disparity_error ||
	    control->alarm_mask_sgmii1.loss_of_signal ||
	    control->alarm_mask_autoneg.detected_wol_frame ||
	    control->alarm_mask_autoneg.detected_wol_packet)
		gbl_chip_wide_vendor_int_mask |= (1 << 11);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 11);

	if (control->alarm_mask_global.state_change ||
	    control->alarm_mask_autoneg.auto_downshift ||
	    control->alarm_mask_autoneg.autonegotation_completed_for_supported_rate ||
	    control->alarm_mask_autoneg.autoneg_completed_for_unsupported_rate ||
	    control->alarm_mask_autoneg.detected_10base_t)
		gbl_chip_wide_vendor_int_mask |= (1 << 12);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 12);

	if (control->alarm_mask_test.xaui_prbs_loss_of_sync[0] ||
	    control->alarm_mask_test.xaui_prbs_loss_of_sync[1] ||
	    control->alarm_mask_test.xaui_prbs_loss_of_sync[2] ||
	    control->alarm_mask_test.xaui_prbs_loss_of_sync[3] ||
	    control->alarm_mask_test.crpat_loss_of_sync ||
	    control->alarm_mask_xaui.running_disparity_error[0] ||
	    control->alarm_mask_xaui.running_disparity_error[1] ||
	    control->alarm_mask_xaui.running_disparity_error[2] ||
	    control->alarm_mask_xaui.running_disparity_error[3] ||
	    control->alarm_mask_xaui.code_violation_error[0] ||
	    control->alarm_mask_xaui.code_violation_error[1] ||
	    control->alarm_mask_xaui.code_violation_error[2] ||
	    control->alarm_mask_xaui.code_violation_error[3] ||
	    control->alarm_mask_xaui.sequence_ordered_set_deletion ||
	    control->alarm_mask_xaui.loss_of_signal[0] ||
	    control->alarm_mask_xaui.loss_of_signal[1] ||
	    control->alarm_mask_xaui.loss_of_signal[2] ||
	    control->alarm_mask_xaui.loss_of_signal[3] ||
	    control->alarm_mask_serdes.sys_intf_rx_link_down ||
	    control->alarm_mask_serdes.sys_intf_rx_link_up)
		gbl_chip_wide_vendor_int_mask |= (1 << 13);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 13);

	if (control->alarm_mask_10gbase_t.invalid_65b_block ||
	    control->alarm_mask_10gbase_t.ldpc_error ||
	    control->alarm_mask_10gbase_t.crc8_error ||
	    control->alarm_mask_xfi0.invalid_66b_block ||
	    control->alarm_mask_xfi0.loss_of_block_lock ||
	    control->alarm_mask_xfi0.high_ber ||
	    control->alarm_mask_xfi1.invalid_66b_block ||
	    control->alarm_mask_xfi1.loss_of_block_lock ||
	    control->alarm_mask_xfi1.high_ber)
		gbl_chip_wide_vendor_int_mask |= (1 << 14);
	else
		gbl_chip_wide_vendor_int_mask &= ~(1 << 14);

	/* Write the masks */
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_STANDARD_INT_MASK_1,
		     autoneg_standard_int_mask[0]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_STANDARD_INT_MASK_2,
		     autoneg_standard_int_mask[1]);
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_STANDARD_INT_MASK_1,
		     phyxs_tx_xaui_rx_standard_int_mask[0]);
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_STANDARD_INT_MASK_2,
		     phyxs_tx_xaui_rx_standard_int_mask[1]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_STANDARD_INT_MASK_1,
		     pcs_standard_int_mask[0]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_STANDARD_INT_MASK_2,
		     pcs_standard_int_mask[1]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_STANDARD_INT_MASK_3,
		     pcs_standard_int_mask[2]);
	aq_phy_write(phydev, 1, AQ_REG_PMA_TX_STANDARD_INT_MASK_1,
		     pma_tx_standard_int_mask[0]);
	aq_phy_write(phydev, 1, AQ_REG_PMA_TX_STANDARD_INT_MASK_2,
		     pma_tx_standard_int_mask[1]);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_INT_MASK_1, global_int_mask[0]);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_INT_MASK_2, global_int_mask[1]);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_INT_MASK_3, global_int_mask[2]);
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_SGMII_TX_INTERRUPT_MASK,
		     gbe_phy_tx_int_mask);
	aq_phy_write(phydev, 0x1d, AQ_REG_GBE_PHY_SGMII_RX_INTERRUPT_MASK,
		     gbe_phy_rx_int_mask);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_TX_VENDOR_INT_MASK_1,
		     autoneg_tx_vendor_int_mask[0]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_TX_VENDOR_INT_MASK_2,
		     autoneg_tx_vendor_int_mask[1]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_TX_VENDOR_INT_MASK_3,
		     autoneg_tx_vendor_int_mask[2]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RX_VENDOR_INT_MASK_1,
		     autoneg_rx_vendor_int_mask[0]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RX_VENDOR_INT_MASK_2,
		     autoneg_rx_vendor_int_mask[1]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RX_VENDOR_INT_MASK_3,
		     autoneg_rx_vendor_int_mask[2]);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_RX_VENDOR_INT_MASK_4,
		     autoneg_rx_vendor_int_mask[3]);
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_INT_MASK_1,
		     phyxs_tx_xaui_rx_vendor_int_mask[0]);
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_INT_MASK_2,
		     phyxs_tx_xaui_rx_vendor_int_mask[1]);
	aq_phy_write(phydev, 4, AQ_REG_XS_XAUI_RX_VENDOR_INT_MASK_3,
		     phyxs_tx_xaui_rx_vendor_int_mask[2]);
	aq_phy_write(phydev, 4, AQ_REG_XS_RX_XAUI_TX_VENDOR_INT_MASK_1,
		     phyxs_rx_xaui_tx_vendor_int_mask[0]);
	aq_phy_write(phydev, 4, AQ_REG_XS_RX_XAUI_TX_VENDOR_INT_MASK_2,
		     phyxs_rx_xaui_tx_vendor_int_mask[1]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_TX_VENDOR_INT_MASK_1,
		     pcs_tx_vendor_int_mask[0]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_TX_VENDOR_INT_MASK_2,
		     pcs_tx_vendor_int_mask[1]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_TX_VENDOR_INT_MASK_3,
		     pcs_tx_vendor_int_mask[2]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_TX_VENDOR_INT_MASK_4,
		     pcs_tx_vendor_int_mask[3]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_1,
		     pcs_rx_vendor_int_mask[0]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_2,
		     pcs_rx_vendor_int_mask[1]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_3,
		     pcs_rx_vendor_int_mask[2]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_4,
		     pcs_rx_vendor_int_mask[3]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_5,
		     pcs_rx_vendor_int_mask[4]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_6,
		     pcs_rx_vendor_int_mask[5]);
	aq_phy_write(phydev, 3, AQ_REG_PCS_RX_VENDOR_INT_MASK_7,
		     pcs_rx_vendor_int_mask[6]);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CHIPWIDE_VENDOR_INT_MASK,
		     gbl_chip_wide_vendor_int_mask);

	return 0;
}

/**
 * Disable all alarm interrupts
 *
 * @param	phydev		phy device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_mask_all_alarms (struct phy_device *phydev)
{
	return aq_phy_write(phydev, 0x1e,
			    AQ_REG_GBL_CHIPWIDE_VENDOR_INT_MASK, 0);
}

/**
 * Enables alarm interrupts
 *
 * @param	phydev		phy device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_unmask_all_alarms (struct phy_device *phydev)
{
	return aq_phy_write(phydev, 0x1e,
			    AQ_REG_GBL_CHIPWIDE_VENDOR_INT_MASK, 0xffff);
}

/**
 * Reads from a PIF mailbox register
 *
 * @param	phydev		phy device
 * @param	mmd		mmd device address
 * @param	address		register address (16-bits)
 * @param[out]	data		data read from register
 *
 * @return	0 for success, -1 for error
 */
int aq_phy_mdio_read_pif_mailbox(struct phy_device *phydev,
				 unsigned int mmd, unsigned int address,
				 unsigned int *data)
{
	uint16_t val;
	int polls;

	/* Clear the command status register */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_CMD_STATUS, 0);
	/* Write the target address */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_ADDRESS, address);
	/* Write the target MMD and "read" command-type */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_ADDR_CMD,
		     mmd | 0x100);
	/* Poll the command-status register until done, error or timeout */
	for (polls = 0; polls < AQ_PHY_MAX_COMMAND_STATUS_POLLS; polls++) {
		val = aq_phy_read(phydev, 0x1e,
				  AQ_REG_GBL_PIF_MAILBOX_CMD_STATUS);
		switch (val & 0xf) {
		case 1:
			*data = aq_phy_read(phydev, 0x1e,
					    AQ_REG_GBL_PIF_MAILBOX_DATA);
			return 0;
		case 2:
			aq_printf("%s: returned error status reading 0x%x.0x%x\n",
				  __func__, mmd, address);
			return -1;
		}
	}
	aq_printf("%s timed out reading 0x%x.0x%x, last value: 0x%x\n",
		  __func__, mmd, address, val);
	return -1;
}

/**
 * Writes to a PIF mailbox register
 *
 * @param	phydev		phy device
 * @param	mmd		mmd device address
 * @param	address		register address (16-bits)
 * @param	data		data to write to register
 *
 * @return	0 for success, -1 for error
 */
int aq_phy_mdio_write_pif_mailbox(struct phy_device *phydev,
				  unsigned int mmd, unsigned int address,
				  unsigned int data)
{
	uint16_t val;
	unsigned polls;

	/* Clear the command status register */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_CMD_STATUS, 0);
	/* Write the target address */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_ADDRESS, address);
	/* Write the data */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_DATA, data);
	/* Write the target MMD and "write" command-type */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_PIF_MAILBOX_ADDR_CMD,
		     mmd | 0x200);
	/* Poll the command-status register until done, error or timeout */
	for (polls = 0; polls < AQ_PHY_MAX_COMMAND_STATUS_POLLS; polls++) {
		val = aq_phy_read(phydev, 0x1e,
				  AQ_REG_GBL_PIF_MAILBOX_CMD_STATUS);
		switch (val & 0xf) {
			case 1:
				return 0;
			case 2:
				debug("%s: returned error status writing 0x%x to 0x%x.0x%x\n",
				      __func__, data, mmd, address);
				return -1;
		}
	}
	aq_printf("%s timed out writing 0x%x to 0x%x.0x%x\n", __func__,
		  data, mmd, address);
	return -1;

}

/**
 * @INTERNAL
 * Return the internal SERDES lane number for the specified lane.
 *
 * @param phydev	PHY device to map
 * @param lane		SERDES lane to look up
 *
 * @return internal SERDES lane
 */
static uint8_t aq_phy_get_serdes_internal_rx_lane(struct phy_device *phydev,
						  uint8_t lane)
{
	uint16_t swap_val;

	swap_val = aq_phy_read(phydev, 3, AQ_REG_PCS_SERDES_MUX_SWAP_TX_RX);
	return ((swap_val >> (lane * 2) & 3) + lane) & 3;
}

/**
 * This function is called before obtaining the EYE data
 *
 * @param	phydev		phy device
 * @param	lane		SERDES lane number
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_begin_serdes_rx_eye_diagram(struct phy_device *phydev, uint8_t lane)
{
	unsigned int lane_addr_offset;
	unsigned int pif_mailbox_read_val;
	uint8_t serdes_reg_val;

	int pif_mailbox_retcode;

	if (lane > 3) {
		aq_printf("%s: Error: lane %u out of range\n", __func__, lane);
		return -1;
	}
	lane_addr_offset = 0x2000 * aq_phy_get_serdes_internal_rx_lane(phydev,
								       lane);
	/* Prepare to make measurements */
	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x57,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x57,
							    serdes_reg_val & ~1);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x56,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x56,
							    serdes_reg_val & ~(1 << 7));
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;

	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x56,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x56,
							    serdes_reg_val & ~(1 << 6));
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;

	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x27,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x27,
							    serdes_reg_val & ~(1 << 2));
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	return 0;
}

/**
 * Obtains SERDES eye information for the specified lane and coordinate
 *
 * @param	phydev		PHY device
 * @param	lane		Lane number to get EYE data on
 * @param	x		X coordinate (0..31)
 * @param	y		Y coordinate (0..63)
 * @param[out]	num_errors	Number of errors at that coordinate
 * @param[out]	max_errors	Maximum errors at that coordinate
 *
 * @return 0 for success, -1 on error
 *
 * NOTE:	This function can take a while.
 *		aq_phy_begin_serdes_rx_eye_diagram must be called before calling
 *		this function and aq_phy_end_serdes_rx_eye_diagram must be
 *		called after obtaining all of the eye information.
 */
int aq_phy_get_serdes_rx_eye_measurement(struct phy_device *phydev,
					 uint8_t lane,
					 uint8_t x, uint8_t y,
					 uint16_t *num_errors,
					 uint16_t *max_errors)
{
	unsigned lane_addr_offset;
	unsigned num_done_checks;
	unsigned pif_mailbox_read_val;
	uint8_t done;
	uint8_t y_adj;
	int8_t x_code_off;
	uint8_t serdes_reg_val;
	uint16_t meas_count;
	uint16_t data_width;
	int pif_mb_retcode;
	struct aq_phy_conn_status conn_status;

	if (lane > 3) {
		aq_printf("%s: Error: lane %d out of range\n", __func__, lane);
		return -1;
	}

	lane_addr_offset = 0x2000 * aq_phy_get_serdes_internal_rx_lane(phydev,
								       lane);
	memset(&conn_status, 0, sizeof(conn_status));
	conn_status.sys_intf_current_mode = SIOM_OTHER;
	aq_phy_get_connection_status(phydev, &conn_status);
	switch (conn_status.sys_intf_current_mode) {
	case SIOM_XFI:
	case SIOM_BACKPLANE_KR:
	case SIOM_BACKPLANE_KX:
		data_width = 32;
		break;
	case SIOM_XAUI:
	case SIOM_XAUI_PAUS_BASED:
	case SIOM_SGMII:
		data_width = 10;
		break;
	case SIOM_RXAUI:
		data_width = 20;
		break;
	default:
		aq_printf("%s: Unsupported SERDES mode %d\n",
			  __func__, conn_status.sys_intf_current_mode);
		return -1;
	}

	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c, 0x8097,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	meas_count = pif_mailbox_read_val & 0xff;
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c, 0x8096,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	meas_count |= (pif_mailbox_read_val & 0xff) << 8;
	if (meas_count == 0) {
		aq_printf("%s: Could not get the bad measure count\n", __func__);
		return -1;
	}
	/* Determine the maximum number of possible errored bits */
	*max_errors = data_width * meas_count;
	/* Set the vertical coordinate of the selected data point */
	if (y < 32)
		y_adj = 63 - y;
	else
		y_adj = y - 32;

	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x22,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	serdes_reg_val = (~0x3f & serdes_reg_val) | y_adj;
	pif_mb_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
						       lane_addr_offset + 0x22,
						       serdes_reg_val);

	/* Read the X code offset. */
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      0x827f,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	x_code_off = (serdes_reg_val & 0x80) ?
				(serdes_reg_val - 0x100) : serdes_reg_val;
	/* Set the horizontal coordinate of the selected data point */
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x21,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	serdes_reg_val = (~0x7f & serdes_reg_val) | (x + 47 - x_code_off);
	pif_mb_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
						       lane_addr_offset + 0x21,
						       serdes_reg_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;

	/* Perform measurements for the selected data point */
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x1f,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mb_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
						       lane_addr_offset + 0x1f,
						       serdes_reg_val | (1 << 2));
	if (pif_mb_retcode)
		return pif_mb_retcode;
	num_done_checks = 0;
	do {
		pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							      lane_addr_offset + 0x20,
							      &pif_mailbox_read_val);
		if (pif_mb_retcode)
			return pif_mb_retcode;
		serdes_reg_val = pif_mailbox_read_val & 0xff;
		num_done_checks++;
		done = (serdes_reg_val >> 2) & 1;
		if (num_done_checks > AQ_PHY_MAX_SERDES_EYE_CHECKS) {
			aq_printf("%s: Timed out waiting for value, last value: 0x%x\n",
				  __func__, serdes_reg_val);
			return -1;
		}
	} while (!done);

	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x1f,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mb_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
						       lane_addr_offset + 0x1f,
							serdes_reg_val & ~(1 << 2));
	if (pif_mb_retcode)
		return pif_mb_retcode;

	/* Read the number of errors for the measurement */
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x26,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	*num_errors = pif_mailbox_read_val & 0xff;
	pif_mb_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
						      lane_addr_offset + 0x25,
						      &pif_mailbox_read_val);
	if (pif_mb_retcode)
		return pif_mb_retcode;
	*num_errors |= (pif_mailbox_read_val & 0xff) << 8;
	aq_debug("%s: x: %d, y: %d, num errors: %d, max errors: %d\n", __func__,
		 x, y, *num_errors, *max_errors);

	return 0;
}

/**
 * This function is called after obtaining the EYE data
 *
 * @param	phydev		phy device
 * @param	lane		SERDES lane number
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_end_serdes_rx_eye_diagram(struct phy_device *phydev, uint8_t lane)
{
	unsigned int lane_addr_offset;
	unsigned int pif_mailbox_read_val;
	uint8_t serdes_reg_val;

	int pif_mailbox_retcode;

	if (lane > 3) {
		aq_printf("%s: Error: lane %u out of range\n", __func__, lane);
		return -1;
	}
	lane_addr_offset = 0x2000 + aq_phy_get_serdes_internal_rx_lane(phydev,
								       lane);
	/* Prepare to make measurements */
	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x57,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x57,
							    1 | serdes_reg_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x56,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x56,
							    (1 << 7) | serdes_reg_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;

	pif_mailbox_retcode = aq_phy_mdio_read_pif_mailbox(phydev, 0x1c,
							   lane_addr_offset + 0x56,
							   &pif_mailbox_read_val);
	if (pif_mailbox_retcode)
		return pif_mailbox_retcode;
	serdes_reg_val = pif_mailbox_read_val & 0xff;
	pif_mailbox_retcode = aq_phy_mdio_write_pif_mailbox(phydev, 0x1c,
							    lane_addr_offset + 0x56,
						            (1 << 6) | serdes_reg_val);
	return pif_mailbox_retcode;
}


/** Table used to compute CRC for loading the flash into the phy */
static const uint16_t aq_phy_crc16_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/**
 * Calculates a CRC16
 *
 * @param crc		starting CRC value
 * @param[in] image	data to calculate CRC over
 * @param size		Size of data in bytes
 *
 * @return		New CRC
 */
uint16_t aq_phy_calc_crc16(uint16_t crc, const uint8_t image[], uint32_t size)
{
	uint32_t i;
	for (i = 0; i < size; i++) {
		crc = ((crc & 0xff) << 8) ^
				aq_phy_crc16_table[(crc >> 8) ^ image[i]];
	}
	return crc;
}

/**
 * Waits for the PHY FLASH interface to become ready.
 *
 * @param	phydev	PHY device to wait for
 *
 * @return	1 if FLASH interface is ready, 0 if not ready
 *
 * NOTE: This waits considerably longer than the free SDK which seems to time
 * out far too soon.
 */
int aq_phy_flash_interface_ready(struct phy_device *phydev)
{
	int i = AQ_PHY_FLASH_INTERFACE_MAX_POLL_COUNT;
	uint16_t val;

	do {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
		/* Wait for nvrBusy to be clear */
		if (!(val & (1 << 8)))
			return 1;
		aq_mdelay(1);
	} while (--i > 0);

	return 0;
}

/**
 * Return if the flash is ready
 *
 * @param phydev		PHY device to check
 * @param[in] fparms		FLASH parameters
 *
 * @return	0 if flash is not ready or timed out, 1 if ready.
 */
int aq_phy_flash_ready(struct phy_device *phydev,
		       const struct aq_flash_parameters *fparms)
{
	uint16_t stat_reg;
	ulong start_time;

	if (!aq_phy_flash_interface_ready(phydev)) {
		aq_printf("%s: flash interface timed out\n", __func__);
		return 0;	/* Timed out */
	}

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
		     fparms->read_status_register_prov);
#ifdef __U_BOOT__
	start_time = get_timer(0);
#else
# error change for Linux
#endif
	do {
		aq_mdelay(1);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms->read_status_register_if);

		if (!aq_phy_flash_interface_ready(phydev)) {
			aq_debug("%s: Timed out waiting for the flash interface to be ready\n",
				 __func__);
			return 0;
		}

		stat_reg = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
		if (fparms->read_status_register_polarity) {
			if ((stat_reg & fparms->read_status_register_mask) != 0)
				break;
		} else {
			if ((stat_reg & fparms->read_status_register_mask) == 0)
				break;
		}
#ifdef __U_BOOT__
		if (get_timer(start_time) >= (fparms->max_operation_time * 1000)) {
			aq_debug("%s: timed out waiting for FLASH to become ready, Global NVR Interface 1: 0x%x\n",
				 __func__, stat_reg);
			return 0;
		}
#else
# error change for Linux
#endif
	} while (1);

	return 1;
}

/**
 * Take control of the FLASH device for programming/verification/etc.
 *
 * @param	phydev	PHY device to take control of
 * @param[out]	state	State that needs to be preserved to return control to
 *			the PHY.
 */
void aq_phy_take_control_of_flash(struct phy_device *phydev,
				  struct aq_flash_control_state *state)
{
	uint16_t val;

	/* Disable the daisy-chain and enable the clock divider override */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_3, 3);
	state->global_control_2 = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CONTROL_2);
	state->global_nvr_provisioning_1 =
			aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1);
	state->global_nvr_provisioning_2 =
			aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_2);
	state->global_nvr_interface_1 =
			aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	state->nvr_crc_freeze = !!(state->global_nvr_interface_1 & (1 << 13));
	/* Freeze NVR CRC */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
		     state->global_nvr_interface_1 | (1 << 13));
	/* Write stall and enable it */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, 0x21);

	/* Force reset of daisy-chain machinery */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESET_CONTROL);
	val |= (1 << 14);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_RESET_CONTROL, val);

	/* Toggle soft reset */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	val &= ~(1 << 15);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, val);
	val |= (1 << 15);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, val);
	val &= ~(1 << 15);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, val);
	/* wait 100us */
	aq_udelay(100);

	/* Re-enable the reset machinery for the register map */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESET_CONTROL);
	val &= ~(1 << 14);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_RESET_CONTROL, val);

	/* Set the clock speed on the SPI to ~10MHz and set the address length
	 * override so we can control the FLASH.
	 */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_2, 0x110);
}

/**
 * Returns control of the FLASH to the PHY device
 *
 * @param	phydev	PHY device to return control to
 * @param[in]	state	State saved from aq_phy_take_control_of_flash()
 */
void aq_phy_return_control_of_flash(struct phy_device *phydev,
				    const struct aq_flash_control_state *state)
{
	uint16_t val;

	/* Restore the NVR CRC freeze bit */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	val |= (!!state->nvr_crc_freeze) << 13;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);

	/* Restore the NVR provisioning */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
		     state->global_nvr_interface_1);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_2,
		     state->global_nvr_provisioning_2);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
		     state->global_nvr_provisioning_1);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2,
		     state->global_control_2);

	/* Re-enable the daisy-chain */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_3);
	val &= ~1;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_3, val);
}

/**
 * Automatically detects the type of FLASH device connected to the PHY
 *
 * @param	phydev		PHY device to detect FLASH for
 * @param[out]	flash_type	Type of FLASH device connected to PHY
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_determine_flash_type(struct phy_device *phydev,
				enum aq_phy_flash_type *flash_type)
{
	uint16_t gbl_pin_status;
	uint16_t val;
	uint16_t lsw, msw;
	uint16_t device_id;
	uint16_t manufacturer_id;
	uint16_t boot_load;

	gbl_pin_status = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_PIN_STATUS);
	boot_load = !!(gbl_pin_status & (1 << 13));

	if (boot_load) {
		debug("%s: flash type is DC slave\n", __func__);
		*flash_type = F_DAISY_CHAIN_SLAVE;
		return 1;
	}

	/* Detect flash type */

	/* Atmel AT25F512A */
	val = 0x0200;

	if (!aq_phy_flash_interface_ready(phydev)) {
		aq_debug("%s: flash interface not ready\n", __func__);
		return -1;
	}

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1, 0x200);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, 0x8015);

	if (aq_phy_flash_interface_ready(phydev)) {
		/* Read the device ID */
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
		device_id = val >> 8;
		manufacturer_id = val & 0xff;

		if ((manufacturer_id == 0x1f) && (device_id == 0x65)) {
			*flash_type = F_AT25F512A;
			aq_debug("%s: flash type: Atmel AT25F512A\n", __func__);
			return 0;
		}
	} else {
		aq_debug("%s: flash interface ready timeout while trying to read the FLASH device ID (Atmel AT25F512A style read)\n",
			 __func__);
		*flash_type = F_INDETERMINATE;
		return -1;
	}

	/* Others */

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1, 0x300);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, 0x809f);

	if (!aq_phy_flash_interface_ready(phydev)) {
		aq_debug("%s: flash interface ready timeout while trying to read the FLASH device ID (standard style read)\n",
			 __func__);
		*flash_type = F_INDETERMINATE;
		return -1;
	}

	lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
	msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5);

	device_id = (lsw & 0xff00) | (msw & 0x00ff);
	manufacturer_id = lsw & 0xff;
	*flash_type = F_INDETERMINATE;

	switch (manufacturer_id) {
	case 0x20:
		/* ST / Numonyx - use full 16-bit device ID */
		switch (device_id) {
		case 0x2010:
			*flash_type = F_M25P05A;
			aq_debug("%s: FLASH type: Numonyx M25P05-A\n",
				 __func__);
			return 0;
		case 0x2013:
			*flash_type = F_M25P40;
			aq_debug("%s: FLASH type: Numonyx M25P40\n", __func__);
			return 0;
		default:
			aq_printf("%s: Unknown FLASH manufacturer 0x%x, device 0x%x\n",
				  __func__, manufacturer_id, device_id);
			return -1;
		}
	case 0xc2:
		if (device_id == 0x2013) {
			*flash_type = F_MX25V4005C;
			aq_debug("%s: FLASH type: Macronix MX25V4005C\n",
				 __func__);
			return 0;
		}
		printf("%s: Unknown FLASH manufacturer 0x%x, device 0x%x\n",
		       __func__, manufacturer_id, device_id);
		return -1;
	case 0x1f:
		/* Atmel */
		switch (device_id >> 8) {
		case 0x24:
			*flash_type = F_AT45DB041D;
			aq_debug("%s: FLASH type: Atmel AT45DB041D\n", __func__);
			return 0;
		case 0x27:
			*flash_type = F_AT45DB321E;
			aq_debug("%s: FLASH type: Atmel AT45DB321E\n", __func__);
			return 0;
		case 0x43:
			*flash_type = F_AT25DF021A;
			aq_debug("%s: FLASH type: Atmel AT25DF021A\n", __func__);
			return 0;
		case 0x44:
			*flash_type = F_AT25DF041A;
			aq_debug("%s: FLASH type: Atmel AT25DF041A\n", __func__);
			return 0;
		default:
			printf("%s: Unknown FLASH manufacturer 0x%x, device 0x%x\n",
			       __func__, manufacturer_id, device_id);
			return -1;
		}
	default:
		printf("%s: Unknown FLASH manufacturer 0x%x, device 0x%x\n",
		       __func__, manufacturer_id, device_id);
		return -1;
	}
}

static int aq_phy_determine_flash_type_standalone(struct phy_device *phydev,
						  enum aq_phy_flash_type *flash_type)
{
	struct aq_flash_control_state flash_state;
	int retval;

	memset(&flash_state, 0, sizeof(flash_state));
	aq_phy_take_control_of_flash(phydev, &flash_state);
	retval = aq_phy_determine_flash_type(phydev, flash_type);
	aq_phy_return_control_of_flash(phydev, &flash_state);
	return retval;
}

/**
 * Get all of the parameters needed for various types of FLASH
 *
 * @param	phydev		PHY device
 * @param	flash_type	Type of FLASH
 * @param[out]	param		FLASH parameters to be filled in
 *
 * @return 0 for success, -1 on error
 */
static int
aq_phy_get_flash_parameters_for_known_flash(struct phy_device *phydev,
					    enum aq_phy_flash_type flash_type,
					    struct aq_flash_parameters *param)
{
	memset(param, 0, sizeof(*param));
	param->chip_erase_opcode = 0x62;
	param->write_enable_opcode = 0x06;
	param->write_status_opcode = 0x01;
	param->disable_prot_data = 0x00;
	param->read_status_opcode = 0x05;
	param->read_status_register_mask = 0x01;
	param->read_status_register_polarity = 0;
	param->read_opcode = 0x03;
	param->program_opcode = 0x02;
	param->page_size = 0x80;
	param->max_operation_time = 10;
	param->write_enable_if = 0x00;
	param->write_enable_prov = 0x00;
	param->chip_erase_if = 0x00;
	param->chip_erase_prov = 0x00;
	param->disable_prot_if = 0x00;
	param->disable_prot_prov = 0x00;
	param->read_status_register_if = 0x00;
	param->read_status_register_prov = 0x00;
	param->nonburst_read4_if = 0x00;
	param->nonburst_read4_prov = 0x00;
	param->burst_read4_if = 0x00;
	param->burst_read4_prov = 0x00;
	param->burst_write4_if = 0x00;
	param->burst_write4_prov = 0x00;
	param->read1_if = 0x00;
	param->read1_prov = 0x00;
	param->end_burst_if = 0x00;
	param->exec_op_if = 0x00;
	param->flash_size = 0x00;

	switch (flash_type) {
	case F_AT25F512A:
		param->flash_size = 0x10000;	/* 64K */
		break;
	case F_AT45DB041D:
		param->read_status_opcode = 0xd7;
		param->read_status_register_mask = 0x80;
		param->read_status_register_polarity = 1;
		param->program_opcode =0x82;
		param->page_size = 0x100;
		param->flash_size = 0x80000;	/* 512K */
		break;
	case F_AT45DB321E:
		param->read_status_opcode = 0xd7;
		param->read_status_register_mask = 0x80;
		param->read_status_register_polarity = 1;
		param->program_opcode =0x82;
		param->page_size = 0x200;
		param->flash_size = 0x400000;	/* 4MB */
		break;
	case F_M25P05A:
		param->chip_erase_opcode = 0xc7;
		param->page_size = 0x100;
		param->flash_size = 0x10000;	/* 64K */
		break;
	case F_AT25DF021A:
		/* Quasi-generic 256Kbyte */
		param->chip_erase_opcode = 0xc7;
		param->page_size = 0x100;
		param->flash_size = 0x40000;	/* 256K */
		break;
	case F_AT25DF041A:
	case F_M25P40:
	case F_MX25V4005C:
		param->chip_erase_opcode = 0xc7;
		param->page_size = 0x100;
		param->flash_size = 0x80000;	/* 512K */
		break;
	default:
		aq_printf("Unknown flash type\n");
		return -1;
	}

	/* Set up the command words.
	 *
	 * NOTE: The PHY is controlled primarily through
	 * AQ_REG_GBL_NVR_INTERFACE_1 and AQ_GBL_NVR_PROVISIONING.  To simplify
	 * programming, default values for these two registers for all desired
	 * operations are pre-calculated and stored in the appropriate variables.
	 * To use them, the provisioning word is written first followed by the
	 * interface word.  Then, after the FLASH is ready any data can be read
	 * and a new operation performed.
	 *
	 * DESCRIPTION: This is done to streamline the execution of the various
	 * commands and limits the read-modify-write operations by pre-computing
	 * the two words that always change from command to command.  Note that
	 * the series of bytes that are sent are:  opcode, 0-4 address bytes
	 * (read mode), 1-4 data bytes (write mode).  Thus if you are sending
	 * an opcode followed by 0-4 address bytes you use read mode.  Only
	 * if you have to send 5 or more bytes do you use write mode.
	 */
	if ((flash_type != F_AT45DB041D) && (flash_type != F_AT45DB321E)) {
		/* Disable protection */
		param->disable_prot_if = param->write_status_opcode | 0x8000;
 		param->disable_prot_prov = 0x0001;

		/* Write enable */
		param->write_enable_if = param->write_enable_opcode | 0x8000;
		param->write_enable_prov = 0x0000;

		/* Chip erase */
		param->chip_erase_if = param->chip_erase_opcode | 0x8000;
		param->chip_erase_prov = 0x0000;
	}
	/* Read status register */
	param->read_status_register_if = param->read_status_opcode | 0x8000;
	param->read_status_register_prov = 0x0100;

	/* Burst read 4 bytes */
	param->burst_read4_if = param->read_opcode | 0x8400;
	param->burst_read4_prov = 0x0403;

	/* Non-burst read 4 bytes */
	param->nonburst_read4_if = param->read_opcode | 0x8000;
	param->nonburst_read4_prov = 0x0403;

	/* Burst write 4 bytes */
	param->burst_write4_if = param->program_opcode | 0xC400;
	param->burst_write4_prov = 0x0403;

	/* Read 1 byte */
	param->read1_if = param->read_opcode | 0x8000;
	param->read1_prov = 0x0103;

	/* Turn off burst write mode */
	param->end_burst_if = param->program_opcode | 0x4000;

	/* Execute operation at end of writing a page */
	param->exec_op_if = param->program_opcode | 0xc000;

	/* Set up for 256/512 byte mode programming.  Send the sequence 0x3d,
	 * 0x2a, 0x80, 0xa6 as the opcode, followed by a 3-byte address and
	 * then wait until done.  NOTE: this is done as a read, as the
	 * address is sent first.
	 */
	param->set256_if = 0x803d;
	param->set256_prov = 0x0003;

	aq_debug("%s: Flash parameters:\n"
		 "  chip erase opcode:   0x%x\n"
		 "  write enable opcode: 0x%x\n"
		 "  write status opcode: 0x%x\n"
		 "  disable protection data: 0x%x\n"
		 "  read status opcode:  0x%x\n"
		 "  read status register mask: 0x%x\n"
		 "  read status register polarity: 0x%x\n"
		 "  read opcode: 0x%x\n"
		 "  program opcode: 0x%x\n"
		 "  page size: 0x%x\n"
		 "  max operation time: %d seconds\n"
		 "  write enable if: 0x%x\n"
		 "  write enable prov: 0x%x\n"
		 "  chip erase if: 0x%x\n"
		 "  chip erase prov: 0x%x\n"
		 "  disable prot if: 0x%x\n"
		 "  disable prot prov: 0x%x\n"
		 "  read status reg if: 0x%x\n"
		 "  read status reg prov: 0x%x\n"
		 "  nonburst read4 if: 0x%x\n"
		 "  nonburst read4 prov: 0x%x\n"
		 "  burst read4 if: 0x%x\n"
		 "  burst read4 prov: 0x%x\n"
		 "  burst write4 if: 0x%x\n"
		 "  burst write4 prov: 0x%x\n"
		 "  read1 if: 0x%x\n"
		 "  read1 prov: 0x%x\n"
		 "  end burst if: 0x%x\n"
		 "  exec op if: 0x%x\n"
		 "  set256 if: 0x%x\n"
		 "  set256 prov: 0x%x\n"
		 "  flash size: 0x%x\n",
		 __func__, param->chip_erase_opcode, param->write_enable_opcode,
		 param->write_status_opcode, param->disable_prot_data,
		 param->read_status_opcode, param->read_status_register_mask,
		 param->read_status_register_polarity, param->read_opcode,
		 param->program_opcode, param->page_size,
		 param->max_operation_time,
		 param->write_enable_if, param->write_enable_prov,
		 param->chip_erase_if, param->chip_erase_prov,
		 param->disable_prot_if, param->disable_prot_prov,
		 param->read_status_register_if,
		 param->read_status_register_prov,
		 param->nonburst_read4_if, param->nonburst_read4_prov,
		 param->burst_read4_if, param->burst_read4_prov,
		 param->burst_write4_if, param->burst_write4_prov,
		 param->read1_if, param->read1_prov,
		 param->end_burst_if, param->exec_op_if,
		 param->set256_if, param->set256_prov, param->flash_size);
	return 0;
}

/**
 * Verifies a flash image from a known type of flash
 *
 * @param	phydev		PHY device to veridy
 * @param	flash_type	Type of FLASH device
 * @param	image		pointer to image to verify to FLASH
 * @param	image_size_ptr	Pointer to size of image in bytes
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_verify_flash_image_of_known_flash(struct phy_device *phydev,
					     enum aq_phy_flash_type flash_type,
					     const uint8_t *image,
					     uint32_t *image_size_ptr);

/**
 * Writes a flash image to a known type of flash
 *
 * @param	phydev		PHY device to program
 * @param	flash_type	Type of FLASH device
 * @param	image		pointer to image to write to FLASH
 * @param	image_size_ptr	Pointer to size of image in bytes
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_write_and_verify_flash_image_of_known_flash(struct phy_device *phydev,
						       enum aq_phy_flash_type flash_type,
						       uint8_t *image,
						       uint32_t *image_size_ptr)
{
	struct aq_flash_parameters fparms;
	uint32_t i;
	uint32_t image_size;
	uint32_t address_ptr;
	uint32_t page_ptr;
	uint32_t bytes_left_in_page;
	uint16_t data_word;
	uint16_t data_msw;
	uint16_t data_lsw;
	uint16_t crc16;
	uint16_t crc16_calculated;
	uint16_t status_register;
	uint16_t val;
	int retval;

	aq_debug("%s(0x%p, %d, 0x%p, 0x%p (%d))\n", __func__, phydev, flash_type,
		 image, image_size_ptr, *image_size_ptr);
	image_size = *image_size_ptr;
	crc16 = image[image_size - 2] << 8 | image[image_size - 1];
	crc16_calculated = aq_phy_calc_crc16(0, image, image_size - 2);

	if (crc16_calculated != crc16) {
		aq_printf("%s: Error: Flash image is corrupt, CRC mismatch\n",
			  __func__);
		return -1;
	}

	aq_debug("%s: Getting flash parameters\n", __func__);
	if (aq_phy_get_flash_parameters_for_known_flash(phydev, flash_type,
							&fparms)) {
		aq_printf("%s: Error: could not get flash parameters\n",
			  __func__);
		return -1;
	}

	aq_debug("%s: Checking size\n", __func__);
	/* Check to make sure the image size minus the 2 CRC bytes is no greater
	 * than the size of the FLASH.
	 */
	if (*image_size_ptr > fparms.flash_size + 2) {
		aq_printf("%s: Error: image size (%u bytes) is larger than the flash size (%u bytes)\n",
			  __func__, *image_size_ptr - 2, fparms.flash_size);
		return -1;
	}
	aq_debug("%s: Erase the flash\n", __func__);
	/* Erase the flash */
	if ((flash_type != F_AT45DB041D) && flash_type != F_AT45DB321E) {
		/* DESCRIPTION: Standard FLASH devides incorporate a protection
		 * scheme into their status register.  Typically there is a
		 * protection bit (usually bit 7) and then some sector
		 * protection bits (usually bits 2, 3, etc.).  All of these
		 * must be cleared in order to alter the FLASH.  However, in
		 * order to clear the section protection bits, the protection
		 * bit must first be cleared.  Once it is cleared, then the
		 * sector protection bits can be cleared.  Also, all of these
		 * commands require a WREN prior to execution.
		 */

		/* Check if the interface is ready */
		if (!aq_phy_flash_interface_ready(phydev)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 1);
			return -1;
		}

		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.disable_prot_prov);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
			     fparms.disable_prot_data);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.disable_prot_if);

		if (!aq_phy_flash_ready(phydev, &fparms)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 2);
			return -1;
		}

		/* Perform the WREN command for the second disable protection */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.write_enable_prov);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.write_enable_if);
		if (!aq_phy_flash_ready(phydev, &fparms)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 3);
			return -1;
		}

		/* Perform the second disable protection command to clear any
		 * protection bits.
		 */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.disable_prot_prov);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
			     fparms.disable_prot_data);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.disable_prot_if);
		if (!aq_phy_flash_ready(phydev, &fparms)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 4);
			return -1;
		}

		/* Wait until ready */
		if (!aq_phy_flash_interface_ready(phydev)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 5);
			return -1;
		}

		/* Perform the chip erase command */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.chip_erase_prov);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.chip_erase_if);
		if (!aq_phy_flash_ready(phydev, &fparms)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 6);
			return -1;
		}
	} else {
		/* Atmel AT45DB041D or AT45DB321E */
		/* Check to make sure the device is in 256/512 byte page mode
		 * by looking at the status register.
		 */

		/* Make sure the interface is ready */

		if (!aq_phy_flash_interface_ready(phydev)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 1);
			return -1;
		}

		/* Execute the read status register instruction */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.read_status_register_prov);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.read_status_register_if);
		/* Wait until the interface is ready */
		if (!aq_phy_flash_interface_ready(phydev)) {
			aq_printf("%s: Error: flash readiness timeout %d\n",
				  __func__, 2);
			return -1;
		}

		status_register = aq_phy_read(phydev, 0x1e,
					      AQ_REG_GBL_NVR_INTERFACE_6);
		if  ((status_register & 0x01) == 0) {
			/* Page size is set to 264/528 bytes */
			aq_debug("%s: Device page size is set to 264/528 bytes\n"
				 "  - setting page size to 256/512 bytes\n",
				 __func__);
			/* Execute the command */
			aq_phy_write(phydev, 0x1e,
				     AQ_REG_GBL_NVR_PROVISIONING_1,
				     fparms.set256_prov);
			/* nvr address MSW */
			aq_phy_write(phydev, 0x1e,
				     AQ_REG_GBL_NVR_INTERFACE_3, 0x002a);
			/* nvr address LSW */
			aq_phy_write(phydev, 0x1e,
				     AQ_REG_GBL_NVR_INTERFACE_4, 0x80a6);
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
				     fparms.set256_if);
			/* Wait for the reprogramming to be complete */
			if (!aq_phy_flash_ready(phydev, &fparms)) {
				aq_printf("%s: Error: flash readiness timeout %d\n",
					  __func__, 6);
				return -1;
			}

			aq_printf("Device page size has been changed to 256 / 512 bytes\n"
				  "To complete this operation, power cycle the FLASH device\n");
			return 1;
		}
	}

	aq_debug("%s: Writing the FLASH...\n", __func__);
	/* Write the FLASH */
	page_ptr = 0;
	address_ptr = 0;
	aq_debug("%s: Bytes in file: 0x%x\n", __func__, image_size - 2);

	while (address_ptr < image_size - 2) {
		aq_debug("%s: Writing to address 0x%06x, size: 0x%x\r",
			 __func__, address_ptr, image_size - 2);

		aq_mdelay(10);
		/* Prepare to write the page of data */
		if (!aq_phy_flash_ready(phydev, &fparms)) {
			aq_printf("%s: Timed out waiting to program page\n",
				  __func__);
			return -1;
		}

		/* Write the WREN and Program instruction */
		if ((flash_type != F_AT45DB041D) &&
		    (flash_type != F_AT45DB321E)) {
			aq_phy_write(phydev, 0x1e,
				     AQ_REG_GBL_NVR_PROVISIONING_1,
				     fparms.write_enable_prov);
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
				     fparms.write_enable_if);
		}

		/* Set up for the Program command */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
			     fparms.burst_write4_prov);
		/* Set the page start address.  NOTE: this directly tracks
		 * the bytes in the image, so it is directly usable here.
		 */
		/* nvr Address MSW */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
			     ((address_ptr >> 16) & 0xff));
		/* NVR Address LSW */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
			     (address_ptr & 0xffff));

		/* Write the page of data */
		while ((page_ptr < (fparms.page_size - 4)) &&
		       (address_ptr < (image_size - 4 - 2))) {
			data_msw = (image[address_ptr + 3] << 8) |
						image[address_ptr + 2];
			data_lsw = (image[address_ptr + 1] << 8) |
						image[address_ptr];
			address_ptr += 4;
			page_ptr += 4;

			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5,
				     data_msw);
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6,
				     data_lsw);
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
				     fparms.burst_write4_if);
		}
		/* Prepare to write the last bytes in the page in non-burst
		 * mode.
		 */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.end_burst_if);

		bytes_left_in_page = (page_ptr >= fparms.page_size - 4) ?
			(fparms.page_size - page_ptr) :
			(image_size - 2 - address_ptr);
		data_word = 0;
		switch (bytes_left_in_page) {
		case 4:
			data_word = image[address_ptr + 3] << 8;
			/* fall through */
		case 3:
			data_word |= image[address_ptr + 2];
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5,
				     data_word);
			/* fall through */
		case 2:
			data_word = image[address_ptr + 1] << 8;
			/* fall through */
		case 1:
			data_word |= image[address_ptr];
			aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6,
				     data_word);
		}
		address_ptr += bytes_left_in_page;
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1);
		val &= ~0x0700;
		val |= bytes_left_in_page << 8;
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1, val);

		page_ptr = 0;
		/* Write the data, no need to check for readiness since this is
		 * done at the beginning of the loop.
		 */
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
		val |= 0x8000;
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);
	}

	if (!aq_phy_flash_ready(phydev, &fparms)) {
		aq_printf("%s: Error: flash readiness timeout %d\n",
			  __func__, 7);
		return -1;
	}

	aq_mdelay(250);
	aq_debug("\n\n%s: Verifying FLASH image\n", __func__);
	retval = aq_phy_verify_flash_image_of_known_flash(phydev, flash_type,
							  image,
							  image_size_ptr);
	if (retval)
		return retval;

	aq_debug("%s: Verify successful, resetting processor\n", __func__);
	/* Reset the processor to force a reload */
	/* Set up run stall override and up run stall */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, 0x0041);

	/* Set up reset */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, 0x8041);

	/* Wait 100us */
	for (i = 0; i < 64; i++)
		aq_phy_read(phydev, 0x1e,
			    AQ_REG_GBL_MICROPROCESSOR_SCRATCH_PAD_1);
		/* Clear up reset and up run stall */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, 0x40);

	return 0;
}

/**
 * Writes an image to the PHY's flash and verifies it.
 *
 * @param	phydev		PHY device to program FLASH for
 * @param[in]	image		Image to program into the FLASH
 * @param	image_size_ptr	Size of image in bytes
 *
 * @return	0 if image is successfully programmed and verified, -1 on error
 */
int aq_phy_write_and_verify_flash_image(struct phy_device *phydev,
					uint8_t *image,
					uint32_t *image_size_ptr)
{
	struct aq_flash_control_state flash_state;
	enum aq_phy_flash_type flash_type;
	int retcode;

	aq_debug("%s(0x%p, 0x%p, 0x%p (%u))\n", __func__, phydev, image,
		 image_size_ptr, *image_size_ptr);
	memset(&flash_state, 0, sizeof(flash_state));

	aq_phy_take_control_of_flash(phydev, &flash_state);
	retcode = aq_phy_determine_flash_type(phydev, &flash_type);
	if (retcode) {
		aq_phy_return_control_of_flash(phydev, &flash_state);
		aq_printf("%s: Error: could not determine FLASH type\n",
			  __func__);
		return -1;
	}
	retcode = aq_phy_write_and_verify_flash_image_of_known_flash(phydev,
								     flash_type,
								     image,
								     image_size_ptr);

	aq_debug("%s: retcode: %d\n", __func__, retcode);
	aq_phy_return_control_of_flash(phydev, &flash_state);

	return retcode;
}

/**
 * Writes an image to the PHY's flash and verifies it.
 *
 * @param	phydev		PHY device to program FLASH for
 * @param	flash_type	Type of flash device
 * @param[in]	image		Image to program into the FLASH
 * @param	image_size_ptr	Size of image in bytes
 *
 * @return	0 if image is successfully programmed and verified, -1 on error
 */
int aq_phy_verify_flash_image_of_known_flash(struct phy_device *phydev,
					     enum aq_phy_flash_type flash_type,
					     const uint8_t *image,
					     uint32_t *image_size_ptr)
{
	struct aq_flash_parameters fparms;
	uint32_t i;
	uint32_t image_size = *image_size_ptr;
	uint32_t image_dword_size;
	uint32_t image_readback_index;
	uint16_t crc16;
	uint16_t crc16_calculated = 0x0000;
	uint16_t msw;
	uint16_t lsw;
	uint16_t val;

	aq_debug("%s(0x%p, %d, 0x%p, 0x%p (%u))\n", __func__, phydev,
		 flash_type, image, image_size_ptr, *image_size_ptr);
	crc16 = (image[image_size - 2] << 8) | image[image_size - 1];
	crc16_calculated = aq_phy_calc_crc16(0, image, image_size - 2);
	if (crc16_calculated != crc16) {
		aq_printf("%s: Error: calculated CRC does not match image CRC\n",
			  __func__);
		return -1;
	}
	aq_debug("%s: Getting flash parameters\n", __func__);
	memset(&fparms, 0, sizeof(fparms));
	if (aq_phy_get_flash_parameters_for_known_flash(phydev,
						        flash_type, &fparms)) {
		aq_printf("%s: Error: could not get flash parameters\n",
			  __func__);
		return -1;
	}

	if (image_size > fparms.flash_size + 2) {
		aq_printf("%s: Error: image size is too large\n", __func__);
		return -1;
	}

	if (!aq_phy_flash_ready(phydev, &fparms)) {
		aq_printf("%s: Error: flash not ready\n", __func__);
		return -1;
	}

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
		     fparms.nonburst_read4_prov);

	/* Set the starting read address */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3, 0x0000);	/* MSW */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4, 0x0000); /* LSW */

	/* Verify the written image */
	crc16_calculated = 0x0000;

	/* Set the number of DWords to read prior to the last block of 4 bytes
	 * so the burst write can be ended.  The -1 is to knock off the last
	 * even block of 4 if there are a multiple of 4 bytes: i.e. we want
	 * 1 -> 4 bytes left, not 0 -> 3 bytes.  The -2 is for the CRC16.
	 */
	image_dword_size = (image_size - 1 - 2) >> 2;

	image_readback_index = 0;

	aq_debug("%s: Verifying %u bytes\n", __func__, image_dword_size * 4);
	for (i = 0; i < image_dword_size; i++) {
		/* non-burst read method */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
			     (i * 4) >> 16);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
			     (i * 4) & 0xffff);

		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.nonburst_read4_if);

		/* Read LSW */
		lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
		/* Read MSW */
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5);

		if (image[image_readback_index] != (lsw & 0xff)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, lsw & 0xff,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw & 0xff)];
		image_readback_index++;

		if (image[image_readback_index] != lsw >> 8) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, lsw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		image_readback_index++;
		if (image[image_readback_index] != (msw & 0xff)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, msw & 0xff,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw & 0xff)];
		image_readback_index++;

		if (image[image_readback_index] != msw >> 8) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, msw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw >> 8)];
		image_readback_index++;
	}

	aq_debug("%s: Verifying last few bytes...\n", __func__);
	/* Set the end of the burst write and read the last bytes (NOTE: just
	 * read 4 bytes regardless and sort out which are valid afterwards).
	 */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	val &= ~0x400;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
		     (image_dword_size * 4) >> 16);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
		     (image_dword_size * 4) & 0xffff);
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	val |= 0x8000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);

	if (!aq_phy_flash_interface_ready(phydev)) {
		aq_printf("%s: Error: FLASH not ready\n", __func__);
		return -1;
	}

	i = image_dword_size;
	aq_debug("%s: Verifying last %d bytes\n",
		 __func__, (image_size - 2) & 3);
	/* We have at least one more byte so handle the first byte */
	lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
	if (image[image_readback_index] != (lsw & 0xff)) {
		aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
			  __func__, image_readback_index, lsw & 0xff,
			  image[image_readback_index]);
		return -1;
	}
	crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
		aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw & 0xff)];
	image_readback_index++;

	switch ((image_size - 2) & 3) {
	case 0:	/* This is actually 4 */
		if (image[image_readback_index] != (lsw >> 8)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, lsw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		image_readback_index++;
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
		if (image[image_readback_index] != (msw & 0xff)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, msw & 0xff,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw & 0xff)];
		image_readback_index++;
		if (image[image_readback_index] != (msw >> 8)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, msw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw >> 8)];
		break;
	case 1:
		/* Handled above */
		break;
	case 2:
		if (image[image_readback_index] != (lsw >> 8)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, lsw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		break;
	case 3:
		if (image[image_readback_index] != (lsw >> 8)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, lsw >> 8,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		image_readback_index++;
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
		if (image[image_readback_index] != (msw & 0xff)) {
			aq_printf("%s:  Mismatch on byte 0x%x: Read 0x%x, expected 0x%x\n",
				  __func__, image_readback_index, msw & 0xff,
				  image[image_readback_index]);
			return -1;
		}
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw & 0xff)];
		image_readback_index++;
		break;
	}
	if (crc16_calculated != crc16) {
		aq_printf("%s: Error: calculated CRC 0x%x does not match expected CRC 0x%x\n",
			  __func__, crc16_calculated, crc16);
		return -1;
	}
	return 0;
}

/**
 * Reads a FLASH image from the PHY
 *
 * @param	phydev		PHY device to read FLASH from
 * @param	flash_type	Type of FLASH connected to the PHY
 * @param	start_address	Starting address in the PHY FLASH
 * @param[out]	image		Data read from the FLASH
 * @param[in,out] image_size_ptr	Size of buffer and size of image
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_read_flash_image_of_known_flash(struct phy_device *phydev,
					   enum aq_phy_flash_type flash_type,
					   uint32_t *start_address,
					   uint8_t *image,
					   uint32_t *image_size_ptr)
{
	struct aq_flash_parameters fparms;
	uint32_t i;
	uint32_t image_size = *image_size_ptr;
	uint32_t image_dword_size;
	uint32_t image_readback_index;
	uint16_t msw;
	uint16_t lsw;
	uint16_t val;

	memset(&fparms, 0, sizeof(fparms));

	aq_debug("%s(0x%p, %d, 0x%x, 0x%p, 0x%p (0x%x))\n",
		 __func__, phydev, flash_type, *start_address, image,
		 image_size_ptr, *image_size_ptr);

	if (aq_phy_get_flash_parameters_for_known_flash(phydev, flash_type,
							&fparms)) {
		aq_printf("%s: Error getting FLASH parameters\n", __func__);
		return -1;
	}

	/* Read the flash */
	/* Poll the FLASH for readiness to start the read */
	if (!aq_phy_flash_ready(phydev, &fparms)) {
		aq_printf("%s: Error waiting for flash to become ready\n",
			  __func__);
		return -1;
	}

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_1,
		     fparms.nonburst_read4_prov);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
		     (*start_address) >> 16);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
		     (*start_address) & 0xffff);
	image_dword_size = (image_size - 1) >> 2;
	image_readback_index = 0;

	for (i = 0; i < image_dword_size; i++) {
		/* Non-burst read method */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
			     ((*start_address) + (i * 4)) >> 16);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
			     ((*start_address) + (i * 4)) & 0xffff);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1,
			     fparms.nonburst_read4_if);

		/* Read LSW */
		lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5);
		image[image_readback_index++] = lsw & 0xff;
		image[image_readback_index++] = lsw >> 8;
		image[image_readback_index++] = msw & 0xff;
		image[image_readback_index++] = msw >> 8;
	}

	/* Set the end of the burst write and read the last bytes */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	val &= ~0x400;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_3,
		     ((*start_address) + (image_dword_size * 4)) >> 16);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_4,
		     ((*start_address) + (image_dword_size * 4)) & 0xffff);
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1);
	val |= 0x8000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_1, val);
	if (!aq_phy_flash_interface_ready(phydev)) {
		aq_printf("%s: Error waiting for FLASH to be ready\n", __func__);
		return -1;
	}
	i = image_dword_size;

	lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_6);
	image[image_readback_index++] = lsw & 0xff;
	switch (image_size & 3) {
	case 1:
		/* Handled above */
		break;
	case 2:
		image[image_readback_index++] = lsw >> 8;
		break;
	case 3:
		image[image_readback_index++] = lsw >> 8;
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5);
		image[image_readback_index++] = msw & 0xff;
		break;
	case 0: /* 4 */
		image[image_readback_index++] = lsw >> 8;
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_INTERFACE_5);
		image[image_readback_index++] = msw & 0xff;
		image[image_readback_index++] = msw >> 8;
		break;
	}

	return 0;
}

/**
 * Reads a FLASH image from the PHY
 *
 * @param	phydev		PHY device to read FLASH from
 * @param	start_address	Starting address in the PHY FLASH
 * @param[out]	image		Data read from the FLASH
 * @param[in,out] image_size_ptr	Size of buffer and size of image
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_read_flash_image(struct phy_device *phydev, uint32_t *start_address,
			    uint8_t *image, uint32_t *image_size_ptr)
{
	struct aq_flash_control_state flash_state;
	enum aq_phy_flash_type flash_type;
	int retcode;

	memset(&flash_state, 0, sizeof(flash_state));
	aq_phy_take_control_of_flash(phydev, &flash_state);
	retcode = aq_phy_determine_flash_type(phydev, &flash_type);
	if (retcode) {
		aq_phy_return_control_of_flash(phydev, &flash_state);
		aq_printf("%s: Error detecting flash type\n", __func__);
		return retcode;
	}
	retcode = aq_phy_read_flash_image_of_known_flash(phydev, flash_type,
							 start_address, image,
							 image_size_ptr);
	aq_phy_return_control_of_flash(phydev, &flash_state);

	return retcode;
}

/**
 * Gets information about the PHY FLASH image
 *
 * @param	phydev		PHY device
 * @param[out]	flash_info	FLASH image information
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_get_flash_information(struct phy_device *phydev,
				 struct aq_phy_flash_info *flash_info)
{
	uint32_t start_address;
	uint32_t size;
	uint32_t primary_header_ptr = 0x00000000;
	uint32_t primary_dram_ptr = 0x00000000;
	uint32_t pointer = 0x00000000;
	struct aq_flash_control_state fcontrol;
	uint16_t i;
	uint8_t image[256];
	uint8_t null_terminator_found = 0;
	uint8_t version_string[AQ_PHY_VERSION_STRING_SIZE];
	int retval;

	aq_debug("%s(0x%p, 0x%p) ENTRY\n", __func__, phydev, flash_info);
	memset(&fcontrol, 0, sizeof(fcontrol));
	memset(flash_info, 0, sizeof(*flash_info));

	flash_info->flash_load_crc = aq_phy_read(phydev, 0x1e,
						 AQ_REG_GBL_DAISY_CHAIN_STATUS_2);
	aq_debug("%s: Taking control of flash\n", __func__);
	aq_phy_take_control_of_flash(phydev, &fcontrol);

	aq_debug("%s: Determining FLASH type\n", __func__);
	retval = aq_phy_determine_flash_type_standalone(phydev,
							&flash_info->flash_type);
	if (retval) {
		aq_debug("%s: Could not determine flash type\n", __func__);
		goto done;
	}

	aq_debug("%s: flash type: %d\n", __func__, flash_info->flash_type);
	if ((flash_info->flash_type != F_DAISY_CHAIN_SLAVE) &&
	    (flash_info->flash_type != F_INDETERMINATE)) {
		size = 0x20;
		start_address = 0x0;
		aq_debug("%s: Reading first 32 bytes of image\n", __func__);
		retval = aq_phy_read_flash_image_of_known_flash(phydev,
								flash_info->flash_type,
								&start_address,
								image,
								&size);
		if (retval)
			goto done;
		primary_header_ptr = (((image[0x9] & 0x0F) << 8) | image[0x8]) << 12;
		size = 0x20;
		start_address = primary_header_ptr + AQ_PHY_IMAGE_HEADER_CONTENT_OFFSET;
		aq_debug("%s: Primary header start: 0x%x\n",
			 __func__, start_address);
		retval = aq_phy_read_flash_image_of_known_flash(phydev,
								flash_info->flash_type,
								&start_address,
								image,
								&size);
		if (retval)
			goto done;

		primary_dram_ptr =
			(image[0xC] << 16) | (image[0xB] << 8) | image[0xA];
		primary_dram_ptr += primary_header_ptr;
		size = 64;
		pointer = primary_dram_ptr + AQ_PHY_VERSION_STRING_BLOCK_OFFSET;
		aq_debug("%s: Primary DRAM: 0x%x, version ptr: 0x%x\n",
			 __func__, primary_dram_ptr, pointer);
		retval = aq_phy_read_flash_image_of_known_flash(phydev,
								flash_info->flash_type,
								&pointer,
								version_string,
								&size);
		if (retval)
			goto done;

		for (i = 0; i < 61; i++) {
			flash_info->image_info.image_id_str[i] =
						(char)version_string[i];
			if (version_string[i] == 0x0)
				null_terminator_found = true;
		}
		if (!null_terminator_found)
			flash_info->image_info.image_id_str[61] = 0x0;
		flash_info->image_info.image_rev_maj = version_string[62];
		flash_info->image_info.image_rev_min = version_string[63];
		flash_info->image_info.image_rom_id_num = version_string[61];
		flash_info->image_info.programmed = true;
		aq_debug("%s: Got image info\n", __func__);
	}

done:
	aq_phy_return_control_of_flash(phydev, &fcontrol);

	aq_debug("%s: Returning %d\n", __func__, retval);
	return retval;
}

void aq_phy_enable_mdio_boot_load_mode(struct phy_device *phydev,
				       unsigned int provisioning_address)
{
	uint16_t val;

	/* Disable the daisy-chain */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_3);
	val |= 1;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_NVR_PROVISIONING_3, val);

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_2);
	val &= ~0x003f;
	val |= provisioning_address;
	val |= 0x40;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_2, val);
}

void aq_phy_enable_gang_load_mode(struct phy_device *phydev,
				  unsigned int provisioning_address,
				  unsigned int gang_load_address)
{
	uint16_t val;

	aq_phy_enable_mdio_boot_load_mode(phydev, provisioning_address);

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_8);
	val &= ~0x1f;
	val |= gang_load_address;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_8, val);

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_2);
	val |= 0x4000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_2, val);
}

void aq_phy_disable_gang_load_mode(struct phy_device *phydev,
				   uint16_t orig_val_ggp1)
{
	uint16_t val;

	val = orig_val_ggp1;
	val &= ~0x4000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_GENERAL_PROVISIONING_2, val);
}

int aq_phy_write_boot_load_image_with_prov_table(struct phy_device **phydevs,
						 int num_ports,
						 const uint32_t *provisioning_addresses,
						 int *result_codes,
						 uint8_t *image,
						 uint32_t *image_size_ptr,
						 uint8_t gangload_mdio_address,
						 struct phy_device *gangload_port,
						 const uint8_t *prov_table_image,
						 uint32_t *prov_table_size_ptr)
{
	struct phy_device *phydev;
	uint32_t primary_header_ptr = 0;
	uint32_t primary_iram_ptr = 0;
	uint32_t primary_dram_ptr = 0;
	uint32_t primary_iram_size = 0;
	uint32_t primary_dram_size = 0;
	uint32_t terminator_ptr = 0;
	uint32_t i, j;
	uint32_t image_size;
	uint32_t prov_table_image_size = 0;
	uint32_t byte_ptr;
	uint32_t byte_size;
	uint32_t dword_size;
	uint16_t msw, lsw;
	uint16_t crc16_calculated;
	uint16_t file_crc;
	uint16_t prov_table_file_crc;
	uint16_t mailbox_crc;
	uint16_t mailbox_write;
	uint16_t recorded_ggp1_values[AQ_PHY_MAX_PHY_IDS];
	int offset;

	image_size = *image_size_ptr;
	file_crc = (image[image_size - 2] << 8) | image[image_size - 1];

	crc16_calculated = aq_phy_calc_crc16(0, image, image_size - 2);

	if (crc16_calculated != file_crc) {
		aq_printf("%s: Error: CRC check on image file failed.  Expected 0x%x, found 0x%x\n",
			  __func__, file_crc, crc16_calculated);
		for (i = 0; i < num_ports; i++)
			result_codes[i] = -1;
		return -1;
	}

	aq_debug("%s: Image CRC OK\n", __func__);
	if (prov_table_size_ptr != NULL && prov_table_image != NULL) {
		prov_table_image_size = (*prov_table_size_ptr) - 2;
		prov_table_file_crc =
			(prov_table_image[prov_table_image_size + 1] << 8) |
			prov_table_image[prov_table_image_size];

		crc16_calculated = aq_phy_calc_crc16(0, prov_table_image,
						     prov_table_image_size);
		if (prov_table_file_crc != crc16_calculated) {
			aq_printf("%s: Error: provision table CRC was 0x%x, expected 0x%x\n",
				  __func__, prov_table_file_crc,
				  crc16_calculated);
			for (i = 0; i < num_ports; i++)
				result_codes[i] = -1;
			return -1;
		}
	}
	aq_debug("%s: Provisioning CRC OK\n", __func__);
	for (j = 0; j < num_ports; j++) {
		if (provisioning_addresses[j] > 47) {
			aq_printf("%s: Error: provisioning addresses out of range 0-47 (index: %u: %d)\n",
				  __func__, j, provisioning_addresses[j]);
			for (i = 0; i < num_ports; i++)
				result_codes[i] = -1;
			return -1;
		}
		phydev = phydevs[j];
	}

	for (j = 0; j < num_ports; j++) {
		phydev = phydevs[j];
		recorded_ggp1_values[j] = aq_phy_read(phydev, 0x1e,
						      AQ_REG_GBL_GENERAL_PROVISIONING_2);
		if (recorded_ggp1_values[j] != recorded_ggp1_values[0]) {
			aq_printf("%s: Non-uniform value of 0x1e.0xc441 found (expected 0x%x, found 0x%x\n",
				  __func__, recorded_ggp1_values[0],
				  recorded_ggp1_values[j]);
			for (i = 0; i < num_ports; i++)
				result_codes[i] = -1;
			return -1;
		}
	}

	for (j = 0; j < num_ports; j++)
		aq_phy_enable_gang_load_mode(phydevs[j],
					     provisioning_addresses[j],
					     gangload_mdio_address);
	aq_debug("%s: gang load mode enabled\n", __func__);
	phydev = gangload_port;

	/** Set UP run stall override and UP run stall */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_CONTROL_2, 0x41);

	/** Initialize the mailbox write command */
	mailbox_write = 0xc000;	/* Write + execute mode */

	primary_header_ptr = (((image[0x9] & 0xf) << 8) | image[0x8]) << 12;

	offset = primary_header_ptr + AQ_PHY_IMAGE_HEADER_CONTENT_OFFSET;
	primary_iram_ptr = (image[offset + 0x4 + 2] << 16) |
			   (image[offset + 0x4 + 1] << 8) |
			   (image[offset + 0x4]);
	primary_iram_size = (image[offset + 0x7 + 2] << 16) |
			    (image[offset + 0x7 + 1] << 8) |
			    (image[offset + 0x7]);
	primary_dram_ptr = (image[offset + 0xa + 2] << 16) |
			   (image[offset + 0xa + 1] << 8) |
			   (image[offset + 0xa]);
	primary_dram_size = (image[offset + 0xd + 2] << 16) |
			    (image[offset + 0xd + 1] << 8) |
			    (image[offset + 0xd]);
	primary_iram_ptr += primary_header_ptr;
	primary_dram_ptr += primary_header_ptr;

	aq_printf("\nSegment Addresses and Sizes as read from the PHY ROM image header:\n\n");
	aq_printf("Primary Iram Address: 0x%x\n", primary_iram_ptr);
	aq_printf("Primary Iram Size:    0x%x\n", primary_iram_size);
	aq_printf("Primary Dram Address: 0x%x\n", primary_dram_ptr);
	aq_printf("Primary Dram Size:    0x%x\n", primary_dram_size);

	/* Merge the provisioning table into the main image */
	if (prov_table_size_ptr && prov_table_image) {
		aq_debug("%s: Merging provisioning image\n", __func__);
		terminator_ptr = primary_dram_ptr +
			((image[primary_dram_ptr + AQ_PHY_IMAGE_PROVTABLE_TERM_OFFSET + 1] << 8) |
			 image[primary_dram_ptr + AQ_PHY_IMAGE_PROVTABLE_TERM_OFFSET]);

		aq_printf("Supplied Provisioning Table at Address: 0x%x\n", terminator_ptr);

		/* Check that the supplied table will fit within the alloted
		 * space.
		 */
		if (terminator_ptr -
		    (primary_dram_ptr + AQ_PHY_IMAGE_PROVTABLE_OFFSET) +
		    prov_table_image_size > AQ_PHY_IMAGE_PROVTABLE_MAXSIZE) {
			for (j = 0; j < num_ports; j++)
				result_codes[j] = -1;
			return -1;
		}
		/* Write the supplied provisioning table into the image,
		 * starting at the terminator address.
		 */
		for (i = 0; i < prov_table_image_size; i++)
			image[terminator_ptr + i] = prov_table_image[i];
	}

	/* Load IRAM and DRAM */
	/* Clear the mailbox CRC */
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1, 0x1000);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1, 0x0000);
	crc16_calculated = 0;

	/* Load the IRAM */
	aq_printf("\nLoading IRAM:\n\n");
	msw = (uint16_t)(AQ_PHY_IRAM_BASE_ADDRESS >> 16);
	lsw = (uint16_t)(AQ_PHY_IRAM_BASE_ADDRESS & 0xfffc);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3, msw);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw);

	/* Set the block size so that there are from 0-3 bytes remaining */
	byte_size = primary_iram_size;
	dword_size = byte_size >> 2;
	byte_ptr = primary_iram_ptr;

	for (i = 0; i < dword_size; i++) {
		lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
		msw = (image[byte_ptr + 3] << 8) | image[byte_ptr + 2];

		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);

		/* Update the CRC */
		crc16_calculated = aq_phy_calc_crc16(crc16_calculated,
						     &image[byte_ptr], 4);
		byte_ptr += 4;
		if ((byte_ptr % 0x100) == 0)
			aq_printf("Address: 0x%x\n", byte_ptr);
	}

	msw = 0;
	lsw = 0;
	switch (byte_size & 3) {
	case 1:
		lsw = image[byte_ptr++];
		break;
	case 2:
		lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
		byte_ptr += 2;
		break;
	case 3:
		lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
		byte_ptr += 2;
		msw = image[byte_ptr++];
		break;
	}
	if (byte_size & 3) {
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw >> 8)];
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw & 0xff)];
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		crc16_calculated =
			((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw & 0xff)];
	}
	aq_printf("\nCRC-16 after loading IRAM: 0x%x\n", crc16_calculated);
	aq_printf("\nLoading DRAM:\n\n");

	/* dword align the address. */
	msw = (uint16_t)(AQ_PHY_DRAM_BASE_ADDRESS >> 16);
	lsw = (uint16_t)(AQ_PHY_DRAM_BASE_ADDRESS & 0xfffc);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3, msw);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw);

	byte_size = primary_dram_size;
	dword_size = byte_size >> 2;
	byte_ptr = primary_dram_ptr;

	for (i = 0; i < dword_size; i++) {
		lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
		msw = (image[byte_ptr + 3] << 8) | image[byte_ptr + 2];

		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);

		/* Update the CRC */
		crc16_calculated = aq_phy_calc_crc16(crc16_calculated,
						     &image[byte_ptr], 4);
		byte_ptr += 4;
	}
	msw = 0;
	lsw = 0;
	switch (byte_size & 3) {
		case 1:
			lsw = image[byte_ptr++];
			break;
		case 2:
			lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
			byte_ptr += 2;
			break;
		case 3:
			lsw = (image[byte_ptr + 1] << 8) | image[byte_ptr];
			byte_ptr += 2;
			msw = image[byte_ptr++];
			break;
	}
	if (byte_size & 3) {
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw >> 8)];
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (msw & 0xff)];
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw >> 8)];
		crc16_calculated = ((crc16_calculated & 0xff) << 8) ^
			aq_phy_crc16_table[(crc16_calculated >> 8) ^ (lsw & 0xff)];
	}

	/* Exit gangland load */
	aq_debug("%s: Disabling gang load mode\n", __func__);
	aq_phy_disable_gang_load_mode(phydev, recorded_ggp1_values[0]);

	/* Check to make sure the mailbox CRC matches the calculated CRC */
	for (j = 0; j < num_ports; j++) {
		mailbox_crc = aq_phy_read(phydevs[j], 0x1e,
					  AQ_REG_GBL_MAILBOX_INTERFACE_2);
		if (mailbox_crc != crc16_calculated) {
			aq_printf("\n%uth port: Mailbox CRC-16 (0x%x) does not match calculated CRC-16 (0x%x)\n",
				  j, mailbox_crc, crc16_calculated);
			result_codes[j] = -1;
		} else {
			result_codes[j] = 0;
		}
	}

	aq_debug("%s: Clearing up resets\n", __func__);
	/* Clear any resets */
	for (j = 0; j < num_ports; j++)
		aq_phy_write(phydevs[j], 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1, 0);

	aq_debug("%s: Releasing PHY processors\n", __func__);
	/* Release the UP */
	for (j = 0; j < num_ports; j++) {
		aq_phy_write(phydevs[j], 0x1e, AQ_REG_GBL_CONTROL_2, 1);
		aq_phy_write(phydevs[j], 0x1e, AQ_REG_GBL_CONTROL_2, 0x8001);
	}
	/* wait at least 100us */
	aq_udelay(100);
	for (j = 0; j < num_ports; j++)
		aq_phy_write(phydevs[j], 0x1e, AQ_REG_GBL_CONTROL_2, 0x41);

	for (j = 0; j < num_ports; j++)
		if (result_codes[j])
			return -1;
	return 0;
}

int aq_phy_write_boot_load_image(struct phy_device **phydevs,
				 uint32_t num_ports,
				 const uint32_t *provisioning_addresses,
				 int *result_codes,
				 uint8_t *image,
				 uint32_t *image_size_ptr,
				 uint8_t gangload_mdio_address,
				 struct phy_device *gangload_port)
{
	return aq_phy_write_boot_load_image_with_prov_table(phydevs,
							    num_ports,
							    provisioning_addresses,
							    result_codes,
							    image,
							    image_size_ptr,
							    gangload_mdio_address,
							    gangload_port,
							    NULL, NULL);
}

static void aq_phy_print_reg(struct phy_device *phydev, int mmd, int reg)
{
	aq_printf("%2x.%04x, 0x%04x\n", mmd, reg, aq_phy_read(phydev, mmd, reg));
}

/**
 * Prints diagnostics registers for Aquantia
 *
 * @param phydev	Phy device to print
 */
void aq_phy_print_debug_data(struct phy_device *phydev)
{
	int i;

	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_STANDARD_CONTROL);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_STANDARD_STATUS);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_STANDARD_STATUS_2);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_STANDARD_TX_DISABLE_CONTROL);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_10GBASE_T_PAIR_SWAP_POLARITY);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_TX_POWER_BACKOFF_SHORT_REACH);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_A);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_B);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_C);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_D);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_A);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_B);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_C);
	aq_phy_print_reg(phydev, 0x1,
			 AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_D);
	/* It's unknown what these registers display */
	for (i = 0xc021; i <= 0xc028; i++)
		aq_phy_print_reg(phydev, 0x1, i);
	aq_phy_print_reg(phydev, 0x1, 0xc100);
	aq_phy_print_reg(phydev, 0x1, 0xc112);
	aq_phy_print_reg(phydev, 0x1, 0xc113);
	aq_phy_print_reg(phydev, 0x1, 0xc116);
	aq_phy_print_reg(phydev, 0x1, 0xc124);
	aq_phy_print_reg(phydev, 0x1, 0xc125);
	aq_phy_print_reg(phydev, 0x1, 0xc126);
	aq_phy_print_reg(phydev, 0x1, 0xc127);
	aq_phy_print_reg(phydev, 0x1, 0xc128);
	aq_phy_print_reg(phydev, 0x1, 0xc129);
	aq_phy_print_reg(phydev, 0x1, 0xc134);
	aq_phy_print_reg(phydev, 0x1, 0xc135);
	aq_phy_print_reg(phydev, 0x1, 0xc136);
	aq_phy_print_reg(phydev, 0x1, 0xc137);
	aq_phy_print_reg(phydev, 0x1, 0xc138);
	aq_phy_print_reg(phydev, 0x1, 0xc141);
	aq_phy_print_reg(phydev, 0x1, 0xc142);
	aq_phy_print_reg(phydev, 0x1, 0xc152);
	aq_phy_print_reg(phydev, 0x1, 0xc153);
	aq_phy_print_reg(phydev, 0x1, 0xc154);
	aq_phy_print_reg(phydev, 0x1, 0xc155);
	aq_phy_print_reg(phydev, 0x1, 0xc410);
	aq_phy_print_reg(phydev, 0x1, 0xc810);
	aq_phy_print_reg(phydev, 0x1, 0xc811);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_TX_VENDOR_ALARMS_1);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_TX_VENDOR_DEBUG_1);
	aq_phy_print_reg(phydev, 0x1, 0xe400);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_RX_VENDOR_STATE_1);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_RX_RESERVED_VENDOR_STATE_1);
	aq_phy_print_reg(phydev, 0x1, AQ_REG_PMA_RX_RESERVED_VENDOR_STATE_2);
	for (i = 0xe920; i <= 0xe927; i++)
		aq_phy_print_reg(phydev, 0x1, i);
	for (i = 0xec01; i <= 0xec07; i++)
		aq_phy_print_reg(phydev, 0x1, i);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_STANDARD_CONTROL_1);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_STANDARD_STATUS_1);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_ADVERTISEMENT_REG);
	aq_phy_print_reg(phydev, 0x7,
			 AQ_REG_AUTONEG_LINK_PARTNER_BASE_PAGE_ABILITY);
	aq_phy_print_reg(phydev, 0x7,
			 AQ_REG_AUTONEG_EXTENDED_NEXT_PAGE_UNFORMATTED_CODE_1);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_10GBASE_T_CONTROL);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_10GBASE_T_STATUS);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
	aq_phy_print_reg(phydev, 0x7,
			 AQ_REG_AUTONEG_RESERVED_VENDOR_PRIVISIONING_1);
	aq_phy_print_reg(phydev, 0x7, 0xc411);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_VENDOR_STATUS_1);

	for (i = 0; i < 70; i++) {
		aq_mdelay(100);
		aq_phy_print_reg(phydev, 0x7,
				 AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_1);
	}
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_2);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_TX_VENDOR_ALARMS_1);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_RX_LINK_PARTNER_STATUS_1);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_RX_LINK_PARTNER_STATUS_2);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_RX_LINK_PARTNER_STATUS_3);
	aq_phy_print_reg(phydev, 0x7, AQ_REG_AUTONEG_RX_LINK_PARTNER_STATUS_4);
	aq_phy_print_reg(phydev, 0x7,
			 AQ_REG_AUTONEG_RX_RESERVED_VENDOR_STATUS_1);
	aq_phy_print_reg(phydev, 0x7,
			 AQ_REG_AUTONEG_RX_RESERVED_VENDOR_STATUS_3);

	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_STANDARD_CONTROL);
	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_STANDARD_STATUS_2);
	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_10G_STATUS_1);
	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_10G_STATUS_2);
	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_TX_VENDOR_DEBUG_1);
	aq_phy_print_reg(phydev, 0x3, 0xd801);
	aq_phy_print_reg(phydev, 0x3, 0xe810);
	aq_phy_print_reg(phydev, 0x3, 0xe811);
	aq_phy_print_reg(phydev, 0x3,
			 AQ_REG_PCS_RX_VENDOR_FCS_NO_ERROR_FRAME_COUNT_1);
	aq_phy_print_reg(phydev, 0x3,
			 AQ_REG_PCS_RX_VENDOR_FCS_NO_ERROR_FRAME_COUNT_2);
	for (i = 0xe820; i <= 0xe823; i++)
		aq_phy_print_reg(phydev, 0x3, i);
	for (i = 0xe840; i <= 0xe852; i++)
		aq_phy_print_reg(phydev, 0x3, i);
	aq_phy_print_reg(phydev, 0x3,
			 AQ_REG_PCS_USX1_UNIDATA_SM_STATUS_REGISTER);
	aq_phy_print_reg(phydev, 0x3, AQ_REG_PCS_RX_VENDOR_DEBUG_1);

	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_STANDARD_CONTROL);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_STANDARD_STATUS);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_STANDARD_STATUS_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_STANDARD_XGXS_LANE_STATUS);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_STANDARD_XGXS_LANE_STATUS);
	aq_phy_print_reg(phydev, 0x4,
			 AQ_REG_XS_XAUI_RX_RESERVED_VENDOR_PROVISIONING_4);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_TX_XAUI_RX_PCS_STATUS_1);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_TX_XAUI_RX_PCS_STATUS_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_TX_XAUI_RX_PCS_STATUS_3);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_TX_XAUI_RX_PCS_STATUS_4);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_RX_VENDOR_ALARMS_2);
	aq_phy_print_reg(phydev, 0x4, 0xcc03);
	aq_phy_print_reg(phydev, 0x4, 0xcc04);
	for (i = 0xe402; i <= 0xe405; i++)
		aq_phy_print_reg(phydev, 0x4, i);
	aq_phy_print_reg(phydev, 0x4,
			 AQ_REG_XS_XAUI_TX_RESERVED_VENDOR_PROVISIONING_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_PCS_STATUS_1);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_PCS_STATUS_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_PCS_STATUS_3);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_PCS_STATUS_4);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_SYS_INTF_CONN_STATUS);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_VENDOR_ALARMS_1);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_VENDOR_ALARMS_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_VENDOR_DEBUG_2);
	aq_phy_print_reg(phydev, 0x4, AQ_REG_XS_XAUI_TX_VENDOR_DEBUG_3);

	aq_phy_print_reg(phydev, 0x1d, 0xc280);
	aq_phy_print_reg(phydev, 0x1d, AQ_REG_GBE_PHY_RESERVED_PROVISIONING_1);
	for (i = AQ_REG_GBE_SGMII1_RX_STATUS_1;
	     i <= AQ_REG_GBE_SGMII1_RX_STATUS_9; i++)
		aq_phy_print_reg(phydev, 0x1d, i);
	for (i = AQ_REG_GBE_SGMII0_RX_STATUS_1;
	     i <= AQ_REG_GBE_SGMII0_RX_STATUS_9; i++)
		aq_phy_print_reg(phydev, 0x1d, i);
	for (i = AQ_REG_GBE_SGMII1_TX_STATUS_1;
	     i <= AQ_REG_GBE_SGMII1_TX_STATUS_10; i++)
		aq_phy_print_reg(phydev, 0x1d, i);
	for (i = AQ_REG_GBE_SGMII0_TX_STATUS_1;
	     i <= AQ_REG_GBE_SGMII0_TX_STATUS_10; i++)
		aq_phy_print_reg(phydev, 0x1d, i);
	aq_phy_print_reg(phydev, 0x1d, 0xe010);
	aq_phy_print_reg(phydev, 0x1d, 0xe030);

	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_STANDARD_CONTROL_1);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_FIRMWARE_ID);
	for (i = 0x1000; i <= 0x1005; i++)
		aq_phy_print_reg(phydev, 0x1e, i);
	for (i = 0x1200; i <= 0x1205; i++)
		aq_phy_print_reg(phydev, 0x1e, i);
	aq_phy_print_reg(phydev, 0x1e, 0x2D50);
	aq_phy_print_reg(phydev, 0x1e, 0x2D60);
	aq_phy_print_reg(phydev, 0x1e, 0x2D70);
	aq_phy_print_reg(phydev, 0x1e, 0x2D80);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_CONTROL_2);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_3);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_4);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_5);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_6);
	aq_phy_print_reg(phydev, 0x1e, 0xc476);
	aq_phy_print_reg(phydev, 0x1e, 0xc477);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_THERMAL_STATUS_1);
	aq_phy_print_reg(phydev, 0x1e, AQ_REG_GBL_THERMAL_STATUS_2);
	for (i = 0xc840; i <= 0xc851; i++)
		aq_phy_print_reg(phydev, 0x1e, i);
	aq_phy_print_reg(phydev, 0x1e, 0xc870);
	for (i = 0xc880; i<= 0xc885; i++)
		aq_phy_print_reg(phydev, 0x1e, i);
}

/**
 * Write to a memory location in the UP
 *
 * @param phydev	PHY device to write to
 * @param[in] data	Data to write
 * @param length	Length of data to write
 * @param start_address	Starting address to write to
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_memory_write(struct phy_device *phydev, const uint8_t *data,
			uint32_t length, uint32_t start_address)
{
	uint32_t i;
	uint32_t byte_pointer;
	uint32_t remaining_bytes;
	uint32_t dword_size;
	uint16_t msw, lsw;
	uint16_t msw_address, lsw_address;
	const uint16_t mailbox_write = 0xc000;
	const uint16_t mailbox_read = 0x8000;

	/* Write the data */

	/* Initialize the mailbox read and write commands */
	msw_address = start_address >> 16;
	lsw_address  = start_address & 0xfffc;

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3, msw_address);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw_address);

	/* Check for non-dword aligned write */
	byte_pointer = 0;
	if (start_address & 3) {
		/* We have to do a read-modify-write */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_read);
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5);
		lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6);
		/* re-write the address as it advanced during the read */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3, msw_address);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw_address);

		for (i = start_address & 3; (i < 4) && (byte_pointer < length);
		     i++) {
			switch (i) {
			case 0x1:
				lsw = (data[byte_pointer++] << 8) | (lsw & 0xff);
				break;
			case 0x2:
				msw = (msw & 0xff00) | data[byte_pointer++];
				break;
			case 0x3:
				msw = (data[byte_pointer++] << 8) | (msw & 0x00ff);
				break;
			}
		}
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);
	}

	/* We are now dword aligned - set the block size so that there are
	 * 0-3 bytes remaining.
	 */
	dword_size = (length - byte_pointer) >> 2;
	remaining_bytes = (length - byte_pointer) & 3;

	for (i = 0; i < dword_size; i++) {
		/* Write 4 bytes of data */
		lsw = (data[byte_pointer + 1] << 8) | data[byte_pointer];
		msw = (data[byte_pointer + 3] << 8) | data[byte_pointer + 2];
		byte_pointer += 4;

		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);
	}

	if (remaining_bytes) {
		msw_address = aq_phy_read(phydev, 0x1e,
					  AQ_REG_GBL_MAILBOX_INTERFACE_3);
		lsw_address = aq_phy_read(phydev, 0x1e,
					  AQ_REG_GBL_MAILBOX_INTERFACE_4);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_read);
		msw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5);
		lsw = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6);
		/* re-write the address as it advanced during the read */
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3,
			     msw_address);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4,
			     lsw_address);

		switch (remaining_bytes) {
		case 0x1:
			lsw = (lsw & 0xff00) | data[byte_pointer];
			break;
		case 0x2:
			lsw = (data[byte_pointer + 1] << 8) | data[byte_pointer];
			break;
		case 0x3:
			lsw = (data[byte_pointer + 1] << 8) | data[byte_pointer];
			msw = (msw & 0xff00) | data[byte_pointer + 2];
			break;
		}
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_5, msw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_6, lsw);
		aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_1,
			     mailbox_write);
	}

	return 0;
}

/**
 * Read from a memory location in the UP
 *
 * @param phydev	PHY device to write to
 * @param[out] data	Data to write
 * @param length	Length of data to write
 * @param start_address	Starting address to write to
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_memory_read(struct phy_device *phydev, uint8_t *data,
		       uint32_t length, uint32_t start_address)
{
	uint32_t byte_pointer;
	uint32_t current_address;
	uint16_t msw, lsw;
	const uint16_t mailbox_read = 0x8000;

	msw = start_address >> 16;
	lsw = start_address & 0xfffc;

	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_3, msw);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_MAILBOX_INTERFACE_4, lsw);

	byte_pointer = 0;
	/* Check for non-dword aligned read */
	for (byte_pointer = 0; byte_pointer < length; byte_pointer++) {
		current_address = byte_pointer + start_address;
		if (((current_address & 3) == 0) || (byte_pointer == 0)) {
			aq_phy_write(phydev, 0x1e,
				     AQ_REG_GBL_MAILBOX_INTERFACE_1,
				     mailbox_read);
			msw = aq_phy_read(phydev, 0x1e,
					  AQ_REG_GBL_MAILBOX_INTERFACE_5);
			lsw = aq_phy_read(phydev, 0x1e,
					  AQ_REG_GBL_MAILBOX_INTERFACE_6);
		}

		switch (current_address & 3) {
		case 0x0:
			data[byte_pointer] = lsw & 0xff;
			break;
		case 0x1:
			data[byte_pointer] = (lsw >> 8) & 0xff;
			break;
		case 0x2:
			data[byte_pointer] = msw & 0xff;
			break;
		case 0x3:
			data[byte_pointer] = (msw >> 8) & 0xff;
			break;
		}
	}
	return 0;
}

/**
 * Print a debug trace output for Aquantia
 *
 * @param	phydev	Phy device to print trace from
 *
 * @return 0 for success, -1 on error (can't freeze/unfreeze debug trace)
 */
int aq_phy_print_debug_trace(struct phy_device *phydev)
{
	unsigned num_freeze_checks;
	unsigned num_unfreeze_checks;
	unsigned i;
	uint32_t target_address;
	uint16_t trace_length;
	uint16_t trace_depth;
	uint16_t trace_read_cursor;
	uint16_t trace_buf_addr_lsw, trace_buf_addr_msw;
	/** Target byte buffer used when reading trace parameters from memory */
	uint8_t reg_data[] = {0, 0};
	/** Byte buffer written to memory to request trace freeze. */
	uint8_t freeze_req_buf[] = {0x4, 0x0};
	/** Byte buffer written to memory to request trace unfreeze */
	uint8_t unfreeze_req_buffer[] = {0x5, 0x0};
	/**
	 * Target byte buffer used when reading the trace buffer from the
	 * PHY's memory
	 */
	uint8_t trace_buffer[AQ_PHY_MAX_TRACE_BUFFER_LENGTH];

	/* Send request to FW to freeze debug trace */
	target_address = 0x3ffe0480 + (0x43 << 1);

	aq_phy_memory_write(phydev, freeze_req_buf, 2, target_address);
	/* Poll until we see the request granted */
	for (num_freeze_checks = 0;
	     num_freeze_checks < AQ_PHY_MAX_FREEZE_CHECKS;
	     num_freeze_checks++) {
		/* Wait 1ms */
		aq_mdelay(1);
		aq_phy_memory_read(phydev, reg_data, 2, target_address);
		if (reg_data[1] == 0x4)
			break;
	}

	if (reg_data[1] != 0x4) {
		aq_printf("Trace freeze request was not acknowledged.\n");
		return -1;
	}

	/* Read trace parameters from memory */
	target_address = 0x3ffe0480 + (0x45 << 1);
	aq_phy_memory_read(phydev, reg_data, 2, target_address);
	trace_buf_addr_lsw = (reg_data[1] << 8) | reg_data[0];

	target_address = 0x3ffe0480 + (0x46 << 1);
	aq_phy_memory_read(phydev, reg_data, 2, target_address);
	trace_buf_addr_msw = (reg_data[1] << 8) | reg_data[0];

	target_address = 0x3ffe0480 + (0x47 << 1);
	aq_phy_memory_read(phydev, reg_data, 2, target_address);
	trace_length = (reg_data[1] << 8) | reg_data[0];

	target_address = 0x3ffe0480 + (0x48 << 1);
	aq_phy_memory_read(phydev, reg_data, 2, target_address);
	trace_depth = (reg_data[1] << 8) | reg_data[0];

	target_address = 0x3ffe0480 + (0x49 << 1);
	aq_phy_memory_read(phydev, reg_data, 2, target_address);
	trace_read_cursor = (reg_data[1] << 8) | reg_data[0];

	/* Read trace buffer from memory */
	target_address = (trace_buf_addr_msw << 16) | trace_buf_addr_lsw;
	aq_phy_memory_read(phydev, trace_buffer, trace_length, target_address);

	/* Print the trace parameters and buffer to traceDump string */
	aq_printf("\n\n0xA0047, 0x%04x\n", trace_length);
	aq_printf("0xA0048, 0x%04x\n", trace_depth);
	aq_printf("0xA0049, 0x%04x\n\n\n", trace_read_cursor);

	for (i = 0; i < (trace_length / 4); i++) {
		aq_printf("%02x%02x%02x%02x\n",
			  trace_buffer[i * 4 + 3],
			  trace_buffer[i * 4 + 2],
			  trace_buffer[i * 4 + 1],
			  trace_buffer[i * 4 + 0]);
	}

	/* Send the request to FW to un-freeze debug trace */
	target_address = 0x3ffe0480 + (0x43 << 1);
	aq_phy_memory_write(phydev, unfreeze_req_buffer, 2, target_address);
	for (num_unfreeze_checks = 0;
	     num_unfreeze_checks < AQ_PHY_MAX_FREEZE_CHECKS;
	     num_unfreeze_checks++) {
		/* Wait 1ms */
		aq_mdelay(1);
		aq_phy_memory_read(phydev, reg_data, 2, target_address);
		if (reg_data[1] == 0x5)
			break;
	}

	if (reg_data[1] != 0x5) {
		aq_printf("Trace unfreeze request was not acknowledged.\n");
		return -1;
	}
	return 0;
}

/**
 * Print Ethernet cable signal information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_print_signal(struct phy_device *phydev)
{
	int a, b, c, d;

	a = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_A);
	a = a - 0x8000;

	b = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_B);
	b = b - 0x8000;

	c = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_C);
	c = c - 0x8000;

	d = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_OPERATING_MARGIN_CHAN_D);
	d = d - 0x8000;

	aq_printf("PMA 10GBase-T SNR Operating Margin:\n"
		  "  Channel A: %d.%ddB\n"
		  "  Channel B: %d.%ddB\n"
		  "  Channel C: %d.%ddB\n"
		  "  Channel D: %d.%ddB\n",
		  a / 10, a % 10, b / 10, b % 10, c / 10, c % 10, d / 10, d % 10);

	a = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_A);
	a = a - 0x8000;

	b = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_B);
	b = b - 0x8000;

	c = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_C);
	c = c - 0x8000;

	d = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_SNR_MIN_OPERATING_MARGIN_CHAN_D);
	d = d - 0x8000;

	aq_printf("PMA 10GBase-T SNR Minimum Operating Margin:\n"
		  "  Channel A: %d.%ddB\n"
		  "  Channel B: %d.%ddB\n"
		  "  Channel C: %d.%ddB\n"
		  "  Channel D: %d.%ddB\n",
		  a / 10, a % 10, b / 10, b % 10, c / 10, c % 10, d / 10, d % 10);

	a = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_RX_SIGNAL_POWER_CHAN_A);
	a = a - 0x8000;

	b = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_RX_SIGNAL_POWER_CHAN_B);
	b = b - 0x8000;

	c = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_RX_SIGNAL_POWER_CHAN_C);
	c = c - 0x8000;

	d = aq_phy_read(phydev, 1,
			AQ_REG_PMA_10GBASE_T_RX_SIGNAL_POWER_CHAN_D);
	d = d - 0x8000;

	aq_printf("PMA 10GBase-T Receive Signal Power:\n"
		  "  Channel A: %d.%ddB\n"
		  "  Channel B: %d.%ddB\n"
		  "  Channel C: %d.%ddB\n"
		  "  Channel D: %d.%ddB\n",
		  a / 10, a % 10, b / 10, b % 10, c / 10, c % 10, d / 10, d % 10);

	return 0;
}

/**************** Cable diagnostics ********************/

/**
 * Starts basic cable diagnostics
 *
 * @param phydev	PHY device to test
 *
 * @return 0 for success, -1 on error (UP timeout)
 */
int aq_phy_run_basic_cable_diags(struct phy_device *phydev)
{
	uint16_t val;

	/* Select "normal" diagnostics */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_1);
	val &= ~0x8000;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_1, val);

	aq_phy_wait_proc_busy(phydev, AQ_PHY_BASIC_CABLE_DIAG_MS);

	/* Set the start bit for diagnostics */
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_1);
	val |= 0x0010;
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_RESERVED_PROVISIONING_1, val);

	return aq_phy_wait_proc_busy(phydev, AQ_PHY_BASIC_CABLE_DIAG_MS);
}

/**
 * Get the results from basic cable diagnostics
 *
 * @param phydev	PHY device
 * @param[out] results	Diagnostic results
 *
 * @return -1 on error, 0 on success, 1 if the diagnostics are still in progress
 */
int aq_phy_get_basic_cable_diags_results(struct phy_device *phydev,
					 struct aq_phy_cable_basic_diag_results *results)
{
	uint16_t diags_running;
	uint16_t pair_status;
	uint16_t val;

	diags_running = aq_phy_read(phydev, 0x1e,
				    AQ_REG_GBL_RESERVED_PROVISIONING_1);
	if (diags_running & 0x10)
		return 1;

	memset(results, 0, sizeof(*results));
	pair_status = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CABLE_DIAG_STATUS_1);
	switch ((pair_status >> 12) & 7) {
	case 0:
		results->pair_a_result.status = AQ_PHY_BASIC_PAIR_STATUS_OK;
		break;
	case 1:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_B;
		break;
	case 2:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_C;
		break;
	case 3:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_D;
		break;
	case 4:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_SHORT;
		break;
	case 5:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_LOW_MISMATCH;
		break;
	case 6:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_HIGH_MISMATCH;
		break;
	case 7:
		results->pair_a_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_OPEN;
		break;
	}
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CABLE_DIAG_STATUS_2);
	results->pair_a_result.reflection_1_distance = val >> 8;
	results->pair_a_result.reflection_2_distance = val & 0xff;

	switch ((pair_status >> 8) & 7) {
	case 0:
		results->pair_b_result.status = AQ_PHY_BASIC_PAIR_STATUS_OK;
		break;
	case 1:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_C;
		break;
	case 2:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_D;
		break;
	case 3:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_A;
		break;
	case 4:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_SHORT;
		break;
	case 5:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_LOW_MISMATCH;
		break;
	case 6:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_HIGH_MISMATCH;
		break;
	case 7:
		results->pair_b_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_OPEN;
		break;
	}
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CABLE_DIAG_STATUS_4);
	results->pair_b_result.reflection_1_distance = val >> 8;
	results->pair_b_result.reflection_2_distance = val & 0xff;

	switch ((pair_status >> 4) & 7) {
	case 0:
		results->pair_c_result.status = AQ_PHY_BASIC_PAIR_STATUS_OK;
		break;
	case 1:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_D;
		break;
	case 2:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_A;
		break;
	case 3:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_B;
		break;
	case 4:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_SHORT;
		break;
	case 5:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_LOW_MISMATCH;
		break;
	case 6:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_HIGH_MISMATCH;
		break;
	case 7:
		results->pair_c_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_OPEN;
		break;
	}
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CABLE_DIAG_STATUS_6);
	results->pair_c_result.reflection_1_distance = val >> 8;
	results->pair_c_result.reflection_2_distance = val & 0xff;

	switch (pair_status & 7) {
	case 0:
		results->pair_d_result.status = AQ_PHY_BASIC_PAIR_STATUS_OK;
		break;
	case 1:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_C;
		break;
	case 2:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_D;
		break;
	case 3:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_A;
		break;
	case 4:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_SHORT;
		break;
	case 5:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_LOW_MISMATCH;
		break;
	case 6:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_HIGH_MISMATCH;
		break;
	case 7:
		results->pair_d_result.status =
				AQ_PHY_BASIC_PAIR_STATUS_OPEN;
		break;
	}
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_CABLE_DIAG_STATUS_8);
	results->pair_d_result.reflection_1_distance = val >> 8;
	results->pair_d_result.reflection_2_distance = val & 0xff;

	return 0;
}

/**
 * Gets the current temperature of the PHY and if there have been any
 * failures or warnings.
 *
 * @param	phydev		PHY device to measure
 * @param[out]	temperature	Current temperature in 1/256 degrees C.
 *				The value is in 2s complement.  For example,
 *				-40C = 0xd800.
 * @param[out]	fail_high	True if the high temp failure threshold has
 *				been exceeded.  Can be NULL.
 * @param[out]	warn_high	True if the high temp warning threshold has
 *				been exceeded.  Can be NULL
 * @param[out]	fail_low	True if the low temp failure threshold has
 *				been exceeded.  Can be NULL.
 * @param[out]	warn_low	True if the low temp warning threshold has
 *				been exceeded.  Can be NULL
 *
 * @return	0 for success, -1 if the temperature is not ready.
 */
int aq_phy_get_temperature(struct phy_device *phydev, int *temperature,
			   bool *fail_high, bool *warn_high,
			   bool *fail_low, bool *warn_low)
{
	uint16_t val;

	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_THERMAL_STATUS_2);
	if (!(val & 1))	/* Make sure temp is ready */
		return -1;

	*temperature = (int16_t)aq_phy_read(phydev, 0x1e,
					    AQ_REG_GBL_THERMAL_STATUS_1);
	val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_STATUS_1);
	if (*fail_high)
		*fail_high = !!(val & 0x4000);
	if (*warn_high)
		*warn_high = !!(val & 0x1000);
	if (*fail_low)
		*fail_low = !!(val & 0x2000);
	if (*warn_low)
		*warn_low = !!(val & 0x800);
	return 0;
}

/**
 * Set the temperature failure and warning thresholds
 *
 * @param phydev		PHY device to configure
 * @param high_fail_threshold	High temperature failure threshold in 2s
 *				complement in 1/256C.  Default is 70C (0x4600).
 * @param high_warn_threshold	High temperature failure threshold in 2s
 *				complement in 1/256C.  Default is 60C (0x3c00).
 * @param low_fail_threshold	Low temperature failure threshold in 2s
 *				complement in 1/256C.  Default is 0C (0x0000).
 * @param low_warn_threshold	Low temperature failure threshold in 2s
 *				complement in 1/256C.  Default is 10C (0x0a00).
 * @return	0 for success
 */
int aq_phy_set_temperature_thresholds(struct phy_device *phydev,
				      int16_t high_fail_threshold,
				      int16_t high_warn_threshold,
				      int16_t low_fail_threshold,
				      int16_t low_warn_threshold)
{
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_HI_TEMP_FAILURE_THRESHOLD,
		     high_fail_threshold);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_HI_TEMP_WARNING_THRESHOLD,
		     high_warn_threshold);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_LOW_TEMP_FAILURE_THRESHOLD,
		     low_fail_threshold);
	aq_phy_write(phydev, 0x1e, AQ_REG_GBL_THERM_LOW_TEMP_WARNING_THRESHOLD,
		     low_warn_threshold);

	return 0;
}

/**
 * Returns the speed of the interface
 *
 * @param phydev	PHY device to get the link speed from
 * @param timeout_ms	How long to wait for the link to up, also used for
 *			how long to wait for the state to reach connected.
 *			This is the delay for the two waits combined.
 *
 * @return		speed in Mbps, 0 if no link, -1 if invalid state
 */
int aq_phy_get_link_speed(struct phy_device *phydev, unsigned int timeout_ms)
{
	ulong start;
	int con_state = -1;
	uint16_t val;
	bool link_up = false;
	const int speed_tbl[] = { 10, 100, 1000, 10000, 2500, 5000, -1, -1 };

	/* Wait for link up */
	start = get_timer(0);
	do {
		val = aq_phy_read(phydev, 1, AQ_REG_PMA_RX_VENDOR_STATE_1);
		link_up = !!(val & 1);
	} while (!link_up && get_timer(start < timeout_ms));

	if (!link_up)
		return 0;
	/* Wait for connected state */
	do {
		val = aq_phy_read(phydev, 7,
				  AQ_REG_AUTONEG_RESERVED_VENDOR_STATUS_1);
		con_state = (val >> 9) & 0x1f;
		if (con_state != 2 && con_state != 3 && con_state != 0xa)
			break;
	} while (get_timer(start) < timeout_ms);
	/* See if connected */
	if (con_state != 4)
		return 0;

	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_STATUS_1);
	return speed_tbl[(val >> 1) & 7];
}

/**
 * Restarts autonegotiation for the specified speeds
 *
 * @param phydev	PHY device to restart autonegotiation on
 * @param enable_1g	Enable 100Mbps and 1Gbps speeds
 * @param enable_2_5g	Enable 2.5Gbps speed
 * @param enable_5g	Enable 5Gbps speed
 * @param enable_10g	Enable 10Gbps speed
 * @param[out] speed	Speed that was negotiated, 0 if link down
 *
 * @return 0 on success, 1 if link is down, -1 on error
 */
int aq_phy_restart_an(struct phy_device *phydev, bool enable_1g,
		      bool enable_2_5g, bool enable_5g, bool enable_10g,
		      int *speed)
{
	ulong start;
	uint16_t val;
	bool __attribute__((unused)) rxaui_2500;
	bool __attribute__((unused)) sgmii_1g;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_OPERATING_MODE);
	rxaui_2500 = !(val & 0x2000);
	sgmii_1g = (val & 0x1800) == 0;

	if (enable_10g) {
		val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL);
		val |= 1 << 12;
		aq_phy_write(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL, val);
	} else {
		val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL);
		val &= ~(1 << 12);
		aq_phy_write(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL, val);
	}
	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
	if (enable_5g)
		val |= (1 << 11);
	else
		val &= ~(1 << 11);
	if (enable_2_5g)
		val |= (1 << 10);
	else
		val &= ~(1 << 10);
	if (enable_1g)
		val |= (1 << 15);
	else
		val &= ~(1 << 15);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING, val);
	/* Restart autonegotiation */
	val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1);
	val |= (1 << 9);
	aq_phy_write(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1, val);

	start = get_timer(0);
	do {
		val = aq_phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_STATUS_2);
	} while ((val & 1 << 15) && get_timer(start) < 2000);
	if (val & (1 << 15)) {
		if (speed)
			*speed = 0;
		return 1;
	}
	if (speed) {
		*speed = aq_phy_get_link_speed(phydev, 10000);

		if (*speed > 0)
			return 0;
		else if (*speed == 0)
			return 1;
		else
			return -1;
	}
	return 0;
}

/**
 * Outputs the mode used for 2.5G
 *
 * @param[in]	phydev		Phy device to read
 * @param[out]	mode_2500basex	Set to true if 2.5G mode uses 2500Base-X, false
 *				if it uses the 10G mode.
 *
 * @return	0 on success, -1 if mode_2500basex is NULL;
 */
int aq_phy_get_2_5g_mode(struct phy_device *phydev,
			 enum aq_phy_2_5g_op_mode *mode)
{
	uint16_t val;
	static const enum aq_phy_2_5g_op_mode modes[] = {
		NOM_2_5G_XAUI, NOM_2_5G_RXAUI, NOM_2_5G_XFI, NOM_2_5G_OTHER
	};

	if (!mode)
		return -1;

	val = aq_phy_read(phydev, 4, AQ_REG_XS_XAUI_RX_OPERATING_MODE);

	if (val & (1 << 13))
		*mode = NOM_2_5G_2500BASE_X;
	else
		*mode = modes[(val >> 14) & 3];

	return 0;
}

static void
__aq_phy_print_basic_cable_pair_result(const char pair_name,
				       const struct aq_phy_cable_diag_pair_result *result)
{
	static const char *pair_status_str[] = {
		"Success",
		"Pair shorted",
		"Lower than expected impedance",
		"Higher than expected impedance",
		"Pair open",
		"Pair is crossed-over with pair A",
		"Pair is crossed-over with pair B",
		"Pair is crossed-over with pair C",
		"Pair is crossed-over with pair D"
	};

	aq_printf("Pair %c result: %s\n", pair_name,
		  pair_status_str[result->status]);
	aq_printf("  First reflection distance in meters: %u (+/- 1m)\n",
		  result->reflection_1_distance);
	aq_printf("  Second reflection distance in meters: %u (+/- 1m)\n",
		  result->reflection_2_distance);
	/* Reflections 3 and 4 are not yet valid */
}

/**
 * Performs basic cable diagnostics and prints the results
 *
 * @param phydev	PHY device to test cable on
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_print_basic_cable_diags(struct phy_device *phydev)
{
	struct aq_phy_cable_basic_diag_results results;
	int retcode;

	aq_printf("Starting basic cable diagnostics, please wait...\n");
	retcode = aq_phy_run_basic_cable_diags(phydev);
	if (retcode) {
		aq_printf("Error starting basic cable diagnostics\n");
		return -1;
	}

	do {
		retcode = aq_phy_get_basic_cable_diags_results(phydev, &results);
		if (retcode < 0) {
			aq_printf("Error getting basic cable diagnostic results\n");
			return -1;
		}
	} while (retcode != 0);

	__aq_phy_print_basic_cable_pair_result('A', &results.pair_a_result);
	__aq_phy_print_basic_cable_pair_result('B', &results.pair_b_result);
	__aq_phy_print_basic_cable_pair_result('C', &results.pair_c_result);
	__aq_phy_print_basic_cable_pair_result('D', &results.pair_d_result);

	return 0;
}

/**
 * Print out temperature information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 if not ready
 */
int aq_phy_print_temperature(struct phy_device *phydev)
{
	int temp;
	bool high_fail, high_warn, low_fail, low_warn;

	if (aq_phy_get_temperature(phydev, &temp,
				   &high_fail, &high_warn,
				   &low_fail, &low_warn)) {
		aq_printf("Error: temperature not ready\n");
		return -1;
	}

	aq_printf("Current temperature: %dC\n", temp / 256);
	if (high_fail)
		aq_printf("Error: high temperature failure\n");
	if (high_warn)
		aq_printf("Warning: high temperature\n");
	if (low_fail)
		aq_printf("Error: low temperature failure\n");
	if (low_warn)
		aq_printf("Warning: low temperature\n");
	return 0;
}

/**
 * Print out PHY FLASH information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 if error
 */
int aq_phy_print_flash_information(struct phy_device *phydev)
{
	struct aq_phy_flash_info flash_info;
	int retcode;

	retcode = aq_phy_get_flash_information(phydev, &flash_info);
	if (retcode) {
		aq_debug("%s: failed to get FLASH information\n", __func__);
		return retcode;
	}

	aq_printf("CRC16: 0x%04x\n", flash_info.flash_load_crc);
	aq_printf("Flash type: ");
	switch (flash_info.flash_type) {
	case F_AT25F512A:
		aq_printf("Atmel 64K AT25F512A\n");
		break;
	case F_AT45DB041D:
		aq_printf("Atmel Smart 512K AT45DB041D\n");
		break;
	case F_M25P05A:
		aq_printf("Numonyx 64K M25P05\n");
		break;
	case F_AT25DF021A:
		aq_printf("Atmel 256K AT25DF021A\n");
		break;
	case F_AT25DF041A:
		aq_printf("Atmel 512K AT25DF041A\n");
		break;
	case F_M25P40:
		aq_printf("Numonyx 512K M25P40\n");
		break;
	case F_MX25V4005C:
		aq_printf("Macronix 512K MX25V4005C\n");
		break;
	case F_AT45DB321E:
		aq_printf("Adesto Smart 4MB AT45DB321E\n");
		break;
	case F_INDETERMINATE:
		aq_printf("Absent or unknown FLASH type\n");
		return 0;
	default:
		aq_printf("Unknown flash type %d\n", flash_info.flash_type);
		return 0;

	}
	if (!flash_info.image_info.programmed) {
		aq_printf("Flash device is not programmed\n");
		return 0;
	}

	aq_printf("Image revision %d.%d\n", flash_info.image_info.image_rev_maj,
		  flash_info.image_info.image_rev_min);
	aq_printf("Image ID: \"%s\"\n", flash_info.image_info.image_id_str);
	aq_printf("ROM ID number: 0x%x\n",
		  flash_info.image_info.image_rom_id_num);
	return 0;
}

#ifdef __U_BOOT__
/**
 * Gets the EYE information from the PHY device
 */
enum command_ret_t do_aq_get_eye(cmd_tbl_t *cmdtp, int flag, int argc,
				 char * const argv[])
{
	struct phy_device *phydev;
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;
	uint32_t addr = 0;
	char *str = NULL;
	char *start;
	int rc;
	int lane;
	int x, y;
	uint16_t num_errors = 0, max_errors = 0;

	ethdev = eth_get_dev_by_name(argv[1]);
	if (!ethdev) {
		aq_printf("Could not find %s\n", argv[1]);
		return CMD_RET_FAILURE;
	}
	ethinfo = ethdev->priv;
	phydev = ethinfo->phydev;
	if (!phydev) {
		aq_printf("No phy associated with %s\n", ethdev->name);
		return CMD_RET_FAILURE;
	}

	if (argc == 4)
		addr = simple_strtoul(argv[2], NULL, 16);
	str = (char *)addr;
	start = str;

	lane = simple_strtoul(argv[argc - 1], NULL, 0);

	rc = aq_phy_begin_serdes_rx_eye_diagram(phydev, lane);
	if (rc) {
		aq_printf("Could not begin getting the EYE data for lane %d\n",
			  lane);
		return CMD_RET_FAILURE;
	}
	if (addr)
		aq_printf("Writing eye data starting at address %p\n", str);

	aq_printf("EYE data:\n");
	for (y = 0; y < 64; y++) {
		for (x = 0; x < 32; x++) {
			rc = aq_phy_get_serdes_rx_eye_measurement(phydev, lane,
								  x, y,
					     &num_errors,
					     &max_errors);
			if (rc) {
				aq_printf("Error getting eye measurement\n");
				return CMD_RET_FAILURE;
			}
			if (addr) {
				str += sprintf(str, "%u %u 0.%06llu\n", x, y,
					       ((uint64_t)num_errors * 1000000) / (uint64_t)max_errors);
			} else {
				aq_printf("%u %u 0.%06llu\n", x, y,
					  ((uint64_t)num_errors * 1000000) / (uint64_t)max_errors);
			}
		}
	}
	rc = aq_phy_end_serdes_rx_eye_diagram(phydev, lane);
	if (rc) {
		aq_printf("Could not end getting the EYE data for lane %d\n",
			  lane);
		return CMD_RET_FAILURE;
	}
	aq_printf("Done.\n");
	if (addr) {
		setenv_hex("fileaddr", addr);
		setenv_hex("filesize", str - start);

		aq_printf("%u (0x%x) bytes of EYE data located at 0x%p\n",
			  str - start, str - start, start);
	}
	return CMD_RET_SUCCESS;
}

/**
 * Load an image into the PHY's FLASH
 *
 * @param	phydev	PHY device to program
 * @param	argc	Argument count for address and size
 * @param[in]	argv	Array of arguments
 *
 * @return CMD_RET_SUCCESS or CMD_RET_FAILURE
 */
enum command_ret_t do_load_image(struct phy_device *phydev,
				 int argc, char * const argv[])
{
	int retcode;
	uint32_t addr;
	uint32_t size;
	uint8_t *image_ptr;

	aq_debug("%s(0x%p, %d, 0x%p)\n", __func__, phydev, argc, argv);
	if (argc > 1) {
		addr = simple_strtoul(argv[1], NULL, 16);
		aq_debug("%s: Address 0x%x (%s)\n", __func__, addr, argv[1]);
	} else {
		addr = getenv_ulong("fileaddr", 16,
				    getenv_ulong("loadaddr", 16,
						 CONFIG_SYS_LOAD_ADDR));
		aq_debug("%s: Address %u loaded from environment\n",
			 __func__, addr);
	}
	if (argc > 2) {
		size = simple_strtoul(argv[2], NULL, 16);
		aq_debug("%s: Size: %u (%s)\n", __func__, size, argv[2]);
	} else {
		size = getenv_ulong("filesize", 16, 0);
		aq_debug("%s: size %u loaded from environment\n",
			 __func__, size);
	}
	image_ptr = (uint8_t *)addr;

	printf("Loading %u byte image from 0x%p\n", size, image_ptr);
	puts("Please wait while the Aquantia PHY FLASH is programmed...\n");
	retcode = aq_phy_write_and_verify_flash_image(phydev, image_ptr, &size);
	if (retcode) {
		printf("Error loading firmware image for %s\n",
		       phydev->dev->name);
		return CMD_RET_FAILURE;
	}
	puts("Programming successful.\n");
	return CMD_RET_SUCCESS;
}

static enum command_ret_t do_rate(struct phy_device *phydev,
				  int argc, char * const argv[])
{
	bool enable;
	u16 val;
	ulong start;

	if (argc < 2) {
		/* Print out the rates supported */
		bool first = true;
		aq_printf("Rates supported: ");
		val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL);
		if (val & 1 << 12) {
			printf("10GBase-T");
			first = false;
		}
		val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
		if (val & (1 << 11)) {
			if (!first)
				aq_printf(", ");
			aq_printf("5GBase-T");
			first = false;
		}
		if (val & (1 << 10)) {
			if (!first)
				aq_printf(", ");
			aq_printf("2.5GBase-T");
			first = false;
		}
		if (val & (1 << 15)) {
			if (!first)
				aq_printf(", ");
			aq_printf("1000Base-T");
		}
		val = aq_phy_read(phydev, 7, AQ_REG_AUTONEG_ADVERTISEMENT_REG);
		if (val & (8 << 5)) {
			if (!first)
				aq_printf(", ");
			aq_printf("100Base-TX (FD)");
			first = false;
		}
		if (val & (4 << 5)) {
			if (!first)
				aq_printf(", ");
			aq_printf("100Base-TX (HD)");
			first = false;
		}
		aq_printf("\n");
		return CMD_RET_SUCCESS;
	}

	if (*argv[2] == 'e') {
		enable = true;
	} else if (*argv[2] == 'd') {
		enable = false;
	} else {
		printf("Use \"enable\" or \"disable\" for each rate.\n");
		return CMD_RET_USAGE;
	}

	if (!strcmp(argv[1], "10g")) {
		val = phy_read(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL);
		if (enable)
			val |= (1 << 12);
		else
			val &= ~(1 << 12);
		phy_write(phydev, 7, AQ_REG_AUTONEG_10GBASE_T_CONTROL, val);
	} else if (!strcmp(argv[1], "5g")) {
		val = phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
		if (enable)
			val |= (1 << 11);
		else
			val &= ~(1 << 11);
		phy_write(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING, val);
	} else if (!strcmp(argv[1], "2.5g")) {
		val = phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
		if (enable)
			val |= (1 << 10);
		else
			val &= ~(1 << 10);
		phy_write(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING, val);
	} else if (!strcmp(argv[1], "1g")) {
		val = phy_read(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING);
		if (enable)
			val |= (1 << 15);
		else
			val &= ~(1 << 15);
		phy_write(phydev, 7, AQ_REG_AUTONEG_VENDOR_PROVISIONING, val);
	} else if (!strcmp(argv[1], "100m")) {
		val = phy_read(phydev, 7, AQ_REG_AUTONEG_ADVERTISEMENT_REG);
		if (enable)
			val |= (0xc << 5);
		else
			val &= ~(0xf << 5);
		phy_write(phydev, 7, AQ_REG_AUTONEG_ADVERTISEMENT_REG, val);
	} else {
		printf("Unknown rate %s\n", argv[1]);
		return CMD_RET_USAGE;
	}

	aq_debug("Restarting autonegotiation\n");
	/* Restart autonegotiation */
	val = phy_read(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1);
	val |= (1 << 9);
	phy_write(phydev, 7, AQ_REG_AUTONEG_STANDARD_CONTROL_1, val);

	aq_debug("Waiting for autonegotiation to restart\n");
	start = get_timer(0);
	do {
		val = phy_read(phydev, 0x1e, AQ_REG_GBL_GENERAL_STATUS_2);
	} while ((val & (1 << 15)) && get_timer(start) < 10000);

	if (val & (1 << 15)) {
		printf("Error: Restarting autonegotiation timed out\n");
		return CMD_RET_FAILURE;
	}
	aq_debug("Restart took %lu ms\n", get_timer(start));

	return CMD_RET_SUCCESS;
}

/**
 * Process aqr105 commands
 *
 * @param	cmdtp	Command data structure, not used
 * @param	flag	Command flags
 * @param	argc	Argument count
 * @param[in]	argv	Array of arguments
 *
 * @return	CMD_RET_USAGE, CMD_RET_FAILURE or CMD_RET_SUCCESS
 */
enum command_ret_t do_aqr105(cmd_tbl_t *cmdtp, int flag, int argc,
				 char * const argv[])
{
	struct phy_device *phydev;
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;

	if (argc < 3)
		return CMD_RET_USAGE;

	ethdev = eth_get_dev_by_name(argv[2]);
	if (!ethdev) {
		aq_printf("Could not find %s\n", argv[2]);
		return CMD_RET_FAILURE;
	}
	ethinfo = ethdev->priv;
	phydev = ethinfo->phydev;
	if (!phydev) {
		aq_printf("No phy associated with %s\n", ethdev->name);
		return CMD_RET_FAILURE;
	}

	if (!strcmp(argv[1], "eye")) {
		return do_aq_get_eye(cmdtp, flag, argc - 1, &argv[1]);
	} else if (!strcmp(argv[1], "signal")) {
		aq_phy_print_signal(phydev);
	} else if (!strcmp(argv[1], "debugdump")) {
		aq_phy_print_debug_data(phydev);
	} else if (!strcmp(argv[1], "tracedump")) {
		if (aq_phy_print_debug_trace(phydev))
			return CMD_RET_FAILURE;
	} else if (!strcmp(argv[1], "basiccablediag")) {
		if (aq_phy_print_basic_cable_diags(phydev))
			return CMD_RET_FAILURE;
	} else if (!strcmp(argv[1], "temp")) {
		if (aq_phy_print_temperature(phydev))
			return CMD_RET_FAILURE;
	} else if (!strcmp(argv[1], "flashinfo")) {
		aq_phy_print_flash_information(phydev);
	} else if (!strcmp(argv[1], "programflash")) {
		if (do_load_image(phydev, argc - 2, argv + 2))
			return CMD_RET_FAILURE;
	} else if (!strcmp(argv[1], "rate")) {
		if (do_rate(phydev, argc - 2, argv + 2))
			return CMD_RET_FAILURE;
	} else {
		return CMD_RET_USAGE;
	}
	return CMD_RET_SUCCESS;
}


U_BOOT_CMD(aqr105, 5, 0, do_aqr105,
	   "Aquantia PHY tool",
	   "eye <eth device> <address> <lane> - reads eye information to the specified address\n"
	   "aqr105 signal <eth device> - Display signal information\n"
	   "aqr105 debugdump <ethdevice> - Dump debugging register information\n"
	   "aqr105 tracedump <ethdevice> - Dump trace information for Aquantia\n"
	   "aqr105 basiccablediag <ethdevice> - Perform basic cable diagnostics\n"
	   "aqr105 temp <ethdevice> - Print temperature information\n"
	   "aqr105 flashinfo <ethdevice> - Print FLASH information\n"
	   "aqr105 programflash <ethdevice> <image address> <image size>\n"
	   "            - Program a new FLASH image into the flash\n"
	   "aqr105 rate <ethdevice> [ 10g | 5g | 2.5g | 1g | 100m] [enable|disable]\n"
	   "            - enables or disables the specified rate\n"
	   "              If no rate is specified then the currently supported rates\n"
	   "              will be displayed.\n");

#endif	/* __U_BOOT__ */
