/*
 * (C) Copyright 2004-2014 Cavium, Inc. <support@cavium.com>
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

#include <common.h>
#include <i2c.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mdio.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-board.h>
#include <asm/arch/octeon_eth.h>
#include <cortina_cs4321.h>

DECLARE_GLOBAL_DATA_PTR;

static const struct cs4321_pin_config pin_cfg = {
	.led = {
		.cfg_event_link_active_a = {
			.event_pattern = LED_B_CONTINUOUS,
			.duty_cycle = 31,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_link_host_disable = {
			.event_pattern = LED_B_CONTINUOUS,
			.duty_cycle = 31,
			.blink_on_time = 4,
			.blink_off_time = 0
		},
		.cfg_event_module_boot = {
			.event_pattern = LED_A_B_ALTERNATE,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_beacon = {
			.event_pattern = LED_B_BLINK,
			.duty_cycle = 31,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_tx_fault = {
			.event_pattern = LED_A_BLINK,
			.duty_cycle = 31,
			.blink_on_time = 4,
			.blink_off_time = 0,
		},
		.cfg_event_rx_los = {
			.event_pattern = LED_A_BLINK,
			.duty_cycle = 31,
			.blink_on_time = 1,
			.blink_off_time = 0
		},
		.cfg_event_mod_abs = {
			.event_pattern = LED_A_B_ALTERNATE,
			.duty_cycle = 31,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_generic_gpio = {
			.event_pattern = LED_A_BLINK,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_user_1 = {
			.event_pattern = LED_A_BLINK,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_user_2 = {
			.event_pattern = LED_A_B_ALTERNATE,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_user_3 = {
			.event_pattern = LED_A_B_ALTERNATE,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_link_active_b = {
			.event_pattern = LED_B_CONTINUOUS,
			.duty_cycle = 31,
			.blink_on_time = 1,
			.blink_off_time = 1
		},
		.cfg_event_edc_converged = {
			.event_pattern = LED_A_BLINK,
			.duty_cycle = 7,
			.blink_on_time = 1,
			.blink_off_time = 0
		},
		.auto_link_active_a = {
			.mode = LED_LINKUP_LINK_HOST_LINE_RX,
			.activity_sample_window = 4095,
			.inv_activity_sample_window = 4095,
			.link_mask = CS4321_LINK_LINE_RX | CS4321_LINK_LINE_TX
		},
		.auto_link_active_b = {
			.mode = LED_LINKUP_NO_LINK,
			/* Time LED will be on and off, 250ms = 4 blinks/second */
			.activity_sample_window = 250,
			/* Time LED will be off, must be less than
			 * activity_sample_window
			 */
			.inv_activity_sample_window = 125,
			.link_mask = CS4321_LINK_LINE_RX | CS4321_LINK_LINE_TX
		},
		.priorities = {
			LED_GEN_GPIO,
			LED_EDC_CONVERGED,
			LED_LINK_ACTIVE_A,
			LED_LINK_ACTIVE_B,
			LED_MODULE_BOOT,
			LED_LINK_HOST_LINK_DISABLE,
			LED_TX_FAULT,
			LED_RX_LOS,
			LED_MOD_ABS,
			LED_USER_1,
			LED_USER_2,
			LED_USER_3,
			LED_BEACON
		},
		.led_mode_select = 1,
		.auto_link_active_enable = 1,
		.link_active_a = 0,
		.link_active_b = 0,
		.link_host_disable = 0,
		.module_boot = 0,
		.beacon = 0,
		.user_def1 = 0,
		.user_def2 = 0,
		.user_def3 = 0,
		.sustain_duration = 0
	},
	.gpio0 = {
		.pins = {
			{	/* 0 is not used */
				.pin_config = 0
			},
			{	/* GPIO 1, MPC/RTCD */
				.pin_config = 0,
			},
			{	/* GPIO 2, TX_FAULT */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LOGIC_0,
				.in_fn = CS4321_GPIO_IN_FCN_TX_FAULT,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 0,
				.outen_ovr = 0,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0,
			},
			{	/* GPIO 3, TX_DISABLE */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LOGIC_0,
				.pin_config = 1,
				.invert_output = 0,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pulldn = 0,
				.pullup = 1,
				.toggle_rate = 0,
				.on_delay = 0,
				.value = 0,
			},
			{	/* GPIO 4 RX_LOS */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LOGIC_0,
				.in_fn = CS4321_GPIO_IN_FCN_EXT_LOS,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 0,
				.outen_ovr = 0,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0,
			},
			{	/* GPIO 5, MOD_ABS */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LOGIC_0,
				.in_fn = CS4321_GPIO_IN_FCN_MOD_ABS,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 0,
				.outen_ovr = 0,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0,
			},
			{	/* GPIO 6 LED_EDC_CONVERGED */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_EDC_CONVERGED,
				.in_fn = CS4321_GPIO_IN_FCN_DISABLED,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 1,
				.outen_ovr = 0,
				.oden = 0,
				.pullup = 0,
				.pulldn = 1,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0,
			},
			{	/* GPIO 7, LED0 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
				.src_fn = CS4321_GPIO_OUTPUT_LED0,
				.in_fn = CS4321_GPIO_IN_FCN_DISABLED,
				.pin_config = 1,
				.invert_output = 1,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0
			},
			{	/* GPIO 8 LED1 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
				.src_fn = CS4321_GPIO_OUTPUT_LED1,
				.in_fn = CS4321_GPIO_IN_FCN_DISABLED,
				.pin_config = 1,
				.invert_output = 1,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0
			},
			{	/* GPIO 9 */
				.pin_config = 0
			},
			{	/* GPIO 10 interrupt output */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
				.src_fn = CS4321_GPIO_OUTPUT_GLOBAL_INT,
				.in_fn = CS4321_GPIO_IN_FCN_DISABLED,
				.pin_config = 1,
				.invert_output = 0,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pullup = 0,
				.pulldn = 0,
				.value = 0,
				.toggle_rate = 0,
				.on_delay = 0
			},
			{	/* GPIO 11 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
				.src_fn = CS4321_GPIO_OUTPUT_SFP_SCL,
				.in_fn = CS4321_GPIO_IN_FCN_SFP_SCL,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 0,
				.outen_ovr = 1,
				.oden = 1,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 1,
				.on_delay = 0,
			},
			{	/* GPIO 12 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
				.src_fn = CS4321_GPIO_OUTPUT_SFP_SDA,
				.in_fn = CS4321_GPIO_IN_FCN_SFP_SDA,
				.pin_config = 1,
				.invert_output = 0,
				.outen = 0,
				.outen_ovr = 1,
				.oden = 1,
				.pulldn = 0,
				.pullup = 0,
				.value = 0,
				.toggle_rate = 1,
				.on_delay = 0,
			},
			{	/* GPIO 13 */
				.pin_config = 0
			},
			{	/* GPIO 14 */
				.pin_config = 0
			},
			{	/* GPIO 15 */
				.pin_config = 0
			},
		},
		.vddsel_override = 0,
	},
	.gpio1 = {
		.pins = {
			{	/* 0 is not used */
				.pin_config = 0,
			},
			{	/* GPIO 1 */
				.pin_config = 0,
			},
			{	/* GPIO 2 */
				.pin_config = 0,
			},
			{	/* GPIO 3 */
				.pin_config = 0,
			},
			{	/* GPIO 4 */
				.pin_config = 0,
			},
			{	/* GPIO 5 */
				.pin_config = 0,
			},
			{	/* GPIO 6 */
				.pin_config = 0,
			},
			{	/* GPIO 7 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LED0,
				.pin_config = 1,
				.invert_output = 0,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.toggle_rate = 0,
				.on_delay = 0,
				.value = 1,
			},
			{	/* GPIO 8 */
				.src_sel = CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE,
				.src_fn = CS4321_GPIO_OUTPUT_LOGIC_0,
				.pin_config = 1,
				.invert_output = 0,
				.iovddsel_1v0 = 0,
				.outen_ovr = 0,
				.outen = 1,
				.oden = 0,
				.pulldn = 0,
				.pullup = 0,
				.toggle_rate = 0,
				.on_delay = 0,
				.value = 0,
			},
			{	/* GPIO 9 */
				.pin_config = 0
			},
			{	/* GPIO 10 */
				.pin_config = 0
			},
			{	/* GPIO 11 */
				.pin_config = 0
			},
			{	/* GPIO 12 */
				.pin_config = 0
			},
			{	/* GPIO 13 */
				.pin_config = 0
			},
			{	/* GPIO 14 */
				.pin_config = 0
			},
			{	/* GPIO 15 */
				.pin_config = 0
			},
		},
	},
};

/** Configuration for the SFP+ cache */
static const struct cs4321_sfp_cache_config sfp_cache_config = {
	.disable_mod_abs = 0,		/** Use mod_abs */
	.disable_diag_poll = 0,		/** Poll diag data as well */
	.enable_i2c_fast_mode = 1,	/** Faster = better */
	.diag_poll_interval = 4,	/** 1 second */
	.max_read_burst = 1,		/** Better safe than sorry */
};

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	cvmx_mio_qlmx_cfg_t mio_qlmx;
	int offset;

	debug("%s ENTER\n", __func__);
	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(0));

	if (mio_qlmx.s.qlm_spd == 15) {
		fdt_key = "0,none";	/* Disabled */
	} else if (mio_qlmx.s.qlm_cfg == 7) {
		fdt_key = "0,rxaui";
		debug("QLM 0 in RXAUI mode\n");
	} else if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "0,sgmii";
		debug("QLM 0 in SGMII mode\n");
	} else {
		debug("Unsupported QLM 0 configuration %d\n",
		      mio_qlmx.s.qlm_cfg);
		fdt_key = "0,none";	/* Disabled */
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(3));
	if (mio_qlmx.s.qlm_spd == 15) {
		fdt_key = "3,none";	/* Disabled */
	} else if (mio_qlmx.s.qlm_cfg == 3) {
		debug("QLM 3 in XAUI mode\n");
		fdt_key = "3,xaui";
	} else if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "3,sgmii";
		debug("QLM 3 in SGMII mode\n");
	} else {
		debug("Unsupported QLM 3 configuration %d\n",
		      mio_qlmx.s.qlm_cfg);
		fdt_key = "3,none";
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(4));
	if (mio_qlmx.s.qlm_spd == 15) {
		fdt_key = "4,none";	/* Disabled */
	} else if (mio_qlmx.s.qlm_cfg == 3) {
		fdt_key = "4,xaui";
		debug("QLM 4 in XAUI mode\n");
	} else if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "4,sgmii";
		debug("QLM 4 in SGMII mode\n");
	} else {
		debug("Unsupported QLM 4 configuration %d\n",
		      mio_qlmx.s.qlm_cfg);
		fdt_key = "4,none";
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	/* USB is not present on revision 3 and later of this board. */
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC68_4 &&
	     gd->arch.board_desc.rev_major >= 3)
		fdt_key = "5,none";
	else
		fdt_key = "5,usb";

	debug("Patching USB with key %s\n", fdt_key);
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	offset = -1;
	if (gd->arch.board_desc.rev_major >= 6)
		return 0;

	/* Remove the reset field from earlier boards */
	while ((offset = fdt_node_offset_by_compatible(gd->fdt_blob,
						       offset,
						       "cortina,cs4318")) > 0)
		fdt_nop_property(gd->fdt_blob, offset, "reset");

	debug("%s EXIT\n", __func__);
	return 0;
}

/**
 * Callback function used by Cortina PHY
 *
 * @paran[in] phydev - pointer to phy device structure
 *
 * @returns Cortina PHY host mode
 */
enum cortina_cs4321_host_mode
cortina_cs4321_get_host_mode(const struct phy_device *phydev)
{
	const struct octeon_eth_info *oct_eth_info;
	cvmx_phy_host_mode_t mode;
	int ipd_port;

	if (!phydev->dev) {
		printf("%s: Error: eth dev unknown\n", __func__);
		return CS4321_HOST_MODE_UNKNOWN;
	}

	oct_eth_info = (struct octeon_eth_info *)phydev->dev->priv;

	ipd_port = oct_eth_info->port;
	if (ipd_port < 0)
		ipd_port -= CVMX_HELPER_BOARD_MGMT_IPD_PORT;
	mode = cvmx_helper_board_get_phy_host_mode(ipd_port);

	switch (mode) {
	case CVMX_PHY_HOST_MODE_SGMII:
		debug("%s: %s host mode SGMII\n", __func__, phydev->dev->name);
		return CS4321_HOST_MODE_SGMII;
	case CVMX_PHY_HOST_MODE_XAUI:
		debug("%s: %s host mode XAUI\n", __func__, phydev->dev->name);
		return CS4321_HOST_MODE_XAUI;
	case CVMX_PHY_HOST_MODE_RXAUI:
		debug("%s: %s host mode RXAUI\n", __func__, phydev->dev->name);
		return CS4321_HOST_MODE_RXAUI;
	default:
	case CVMX_PHY_HOST_MODE_UNKNOWN:
		printf("%s: Unknown host mode %d\n", __func__, mode);
		return CS4321_HOST_MODE_UNKNOWN;
	}
}

int checkboard(void)
{
	cvmx_mio_qlmx_cfg_t mio_qlmx;

	debug("%s: ENTER\n", __func__);
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC68_4) {
		mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(0));

		printf("NIC SFP+ ports configured for %s\n",
		       mio_qlmx.s.qlm_cfg == 7 ? "XAUI" : "SGMII");
	}

	return 0;
}

int early_board_init(void)
{
	cvmx_mio_fus_pll_t fus_pll;
	uint8_t val;

	octeon_board_get_clock_info(NIC68_4_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC68_4, 3, 0);

	octeon_board_get_mac_addr();

	/* configure clk_out pin */

	fus_pll.u64 = cvmx_read_csr(CVMX_MIO_FUS_PLL);
	fus_pll.cn63xx.c_cout_rst = 1;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* Sel::  0:rclk, 1:pllout 2:psout 3:gnd */
	fus_pll.cn63xx.c_cout_sel = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);
	fus_pll.cn63xx.c_cout_rst = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* The NIC68_4 and the NIC68_410NV are virtually identical as far as
	 * software is concerned.
	 */
	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	/* Note that the SA56004 will be programmed again later with DTT */
	/* Internal SA56004 temp, approximates ambient/board temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x20, 80);

	/* Internal Octeon temp, approximates junction temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x19, 100);
	/* Enable the fault queue, requiring three consecutive measurements
	 * before triggering T_CRIT.
	 */
	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 0x03);
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x9, val | 1);

	/* Enable PSRAM (take out of low power mode) */
	if ((gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC68_4) &&
	    (gd->arch.board_desc.rev_major > 2)) {
		gpio_direction_output(0, 1);

		/* Enable USB supply */
		gpio_direction_output(6, 1);
	}
	return 0;
}

int board_phy_init(void)
{
	int i;
	debug("%s: ENTER\n", __func__);
	/* We need to initialize MDIO early on for the Cortina PHY */
	/* Put Cortina PHYs into reset */
	gpio_direction_output(10, 0);
	/* Enable MDIO */
	for (i = 0; i < 4; i++)
		cvmx_write_csr(CVMX_SMIX_EN(i), 1);
	/* Wait 10ms */
	mdelay(10);
	/* Take Cortina PHYs out of reset */
	gpio_set_value(10, 1);
	/* Wait before we start issuing commands */
	mdelay(10);

	return 0 /* cortina_phy_init() */;
}

/**
 * This function configures all of the LEDs, GPIOs and SFP cache for a Cortina
 * PHY device
 *
 * @param phydev pointer to phy information
 *
 * @return 0 for success, error otherwise.
 */
int board_configure_cs4321_leds(struct phy_device *phydev)
{
	int ret;
	struct phy_device dummy;

	debug("%s(%s(%d))\n", __func__, phydev->dev->name, phydev->addr);
	if (gd->arch.board_desc.rev_major < 4)
		return 0;

	if ((phy_read(phydev, 0, 0) !=  0x23e5) &&
	    (phy_read(phydev, 0, 1) != 0x2002)) {
		printf("ERROR: %s Revision B0 Cortina PHY not found, board revision improperly set or hardware needs rework.\n",
		       phydev->dev->name);
		return -1;
	}
	debug("  Configuring LEDs\n");
	ret = cs4321_configure_leds(phydev, &pin_cfg.led);
	if (ret) {
		puts("Error configuring Cortina PHY LEDs\n");
		return ret;
	}
	debug("%s: Configuring GPIOs 0\n", __func__);
	ret = cs4321_configure_gpios(phydev, &pin_cfg.gpio0);
	if (ret) {
		puts("Error configuring Cortina GPIOs 0\n");
		return ret;
	}
	memcpy(&dummy, phydev, sizeof(*phydev));
	dummy.addr++;
	debug("%s: Configuring GPIOs 1\n", __func__);
	ret = cs4321_configure_gpios(&dummy, &pin_cfg.gpio1);
	if (ret) {
		puts("Error configuring Cortina GPIOs\n");
		return ret;
	}
	ret = cs4321_configure_sfp_cache(phydev, &sfp_cache_config);
	if (ret)
		puts("Error configuring Cortina SFP+ cache\n");

	return ret;
}

/**
 * Dynamically adjust the board name, used for prompt generation
 * @param name - name string to be adjusted
 * @param max_len - maximum length of name
 *
 * This function can overwrite the name of a board, for example based on
 * the processor installed.
 */
void octeon_adjust_board_name(char *name, size_t max_len)
{
	if (gd->arch.board_desc.rev_major >= 4)
		strcpy(name, "nic410nvg");
}
