/***********************license start************************************
 * Copyright (c) 2011 Cavium Inc. (support@cavium.com). All rights
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
 * support@cavium.com
 *
 ***********************license end**************************************/
#ifndef __OCTEON_BCSR_H__
#define __OCTEON_BCSR_H__

#define OCTEON_BCSR_BOARD_ID_OFFSET		0
#define OCTEON_BCSR_CLPD_REV_OFFSET		1
#define OCTEON_BCSR_LED_CTRL_OFFSET		2
#define OCTEON_BCSR_CF_CTRL_STATUS_OFFSET	4
#define OCTEON_BCSR_SGMII_XAUI_STATUS_OFFSET	5
#define OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET	7
#define OCTEON_BCSR_ENET_CTRL_OFFSET		8

typedef enum {
	OCTEON_BCSR_LED_USER0 = 0,
	OCTEON_BCSR_LED_USER1 = 1,
	OCTEON_BCSR_LED_SYSERR = 2
} octeon_bcsr_led_t;

/* These bits enable/disable various LEDs */
#define OCTEON_BCSR_LED_USER0_DISABLE		0x80
#define OCTEON_BCSR_LED_USER1_DISABLE		0x40
#define OCTEON_BCSR_LED_SYSERR_DISABLE		0x20

/* Compact flash control and status bits */
#define OCTEON_BCSR_CF_READY			0x80	/* RO */
#define OCTEON_BCSR_CF_CARD0_NOT_INSERTED	0x40	/* RO */
#define OCTEON_BCSR_CF_CARD1_NOT_INSERTED	0x20	/* RO */
#define OCTEON_BCSR_CF_3_3V			0x10	/* RO */
#define OCTEON_BCSR_CF_WRITE_PROTECTED		0x08	/* RO */
#define OCTEON_BCSR_CF_RESET			0x04

#define OCTEON_BCSR_SGMII_MODE			0x80	/* 0 = XAUI, RO */

#define OCTEON_BCSR_NOR_READY			0x80	/* RO */
#define OCTEON_BCSR_NOR_WRITE_PROTECTED		0x40
#define OCTEON_BCSR_EEPROM_WRITE_PROTECTED	0x20
#define OCTEON_BCSR_NAND_WRITE_PROTECTED	0x08
#define OCTEON_BCSR_POWER_OFF			0x04

#define OCTEON_BCSR_ENET_PHY1_RESET		0x80
#define OCTEON_BCSR_ENET_PHY1_POWER_DOWN	0x40
#define OCTEON_BCSR_ENET_PHY2_RESET		0x20
#define OCTEON_BCSR_ENET_PHY3_RESET		0x10

extern uint64_t bcsr_addr;

static inline uint8_t octeon_bcsr_get_board_id(void)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return 0;
	}
	return cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_BOARD_ID_OFFSET);
}

static inline uint8_t octeon_bcsr_get_cpld_rev(void)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return 0;
	}

	return cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_CLPD_REV_OFFSET);
}

static inline void octeon_bcsr_set_led(octeon_bcsr_led_t led, int set)
{
	uint8_t val;

	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_LED_CTRL_OFFSET);
	if (set)
		val &= ~(0x80 >> led);
	else
		val |= (0x80 >> led);
	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_LED_CTRL_OFFSET, val);
}

static inline uint8_t octeon_bcsr_get_cf_status(void)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return 0;
	}

	return cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_CF_CTRL_STATUS_OFFSET);
}

static inline int octeon_bcsr_get_sgmii_xaui_mode(void)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return 0;
	}

	return cvmx_read64_uint8(bcsr_addr +
				 OCTEON_BCSR_SGMII_XAUI_STATUS_OFFSET)
	       & OCTEON_BCSR_SGMII_MODE;
}

static inline int octeon_bcsr_get_nor_flash_busy(void)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return 0;
	}

	return !!(cvmx_read64_uint8(bcsr_addr +
				    OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET)
		  & OCTEON_BCSR_NOR_READY);
}

static inline void octeon_bcsr_set_nor_flash_wp(int wp)
{
	uint8_t val;

	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr
				+ OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET);

	if (wp)
		val |= OCTEON_BCSR_NOR_WRITE_PROTECTED;
	else
		val &= ~OCTEON_BCSR_NOR_WRITE_PROTECTED;

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET,
			   val);
}

static inline void octeon_bcsr_set_eeprom_wp(int wp)
{
	uint8_t val;

	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr
				+ OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET);

	if (wp)
		val |= OCTEON_BCSR_EEPROM_WRITE_PROTECTED;
	else
		val &= ~OCTEON_BCSR_EEPROM_WRITE_PROTECTED;

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET,
			   val);
}

static inline void octeon_bcsr_set_nand_flash_wp(int wp)
{
	uint8_t val;


	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr
				+ OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET);

	if (wp)
		val |= OCTEON_BCSR_NAND_WRITE_PROTECTED;
	else
		val &= ~OCTEON_BCSR_NAND_WRITE_PROTECTED;

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET,
			   val);
}

static inline void octeon_bcsr_set_power_off(void)
{
	uint8_t val;

	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr
				+ OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET);
	val |= OCTEON_BCSR_POWER_OFF;

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_MEM_PWR_CTRL_STATUS_OFFSET,
			   val);
}

static inline void octeon_bcsr_enet_reset(uint8_t phy_mask)
{
	if (!bcsr_addr) {
		printf("%s: Error: BCSR address unknown\n", __func__);
		return;
	}

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_ENET_CTRL_OFFSET, phy_mask);
}
#endif /* __OCTEON_BCSR_H__ */