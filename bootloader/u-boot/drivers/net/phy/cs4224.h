/*    Copyright (C) 2014 Cavium, Inc.
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
#ifndef __CS4224_H__
#define __CS4224_H__

/** LED patterns for blinking */
enum cs4224_led_event_pattern {
	LED_OFF = 0,		/** LEDs are off */
	LED_A_CONTINUOUS = 1,	/** LED A on continuously */
	LED_B_CONTINUOUS = 2,	/** LED B on continuously */
	LED_A_BLINK = 3,	/** LED A blink */
	LED_B_BLINK = 4,	/** LED B blink */
	LED_A_B_ALTERNATE = 5	/** Alternate between LEDs A and B */
};

/** Various events that can be configured for LEDs */
enum cs4224_led_events {
	LED_DISABLED = 0,	/** LED is disabled */
	LED_LINK_ACTIVE_A,	/** LED A indicates link activity */
	LED_LINK_HOST_LINK_DISABLE,
	LED_MODULE_BOOT,	/** Module boot event */
	LED_BEACON,		/** Beacon event */
	LED_TX_FAULT,		/** TX fault event */
	LED_RX_LOS,		/** RX LOS event */
	LED_MOD_ABS,		/** Module Abscent */
	LED_EDC_CONVERGED,	/** EDC converged event */
	LED_GENERIC_GPIO,
	LED_USER_1,
	LED_USER_2,
	LED_USER_3,
	LED_LINK_ACTIVE_B,	/** LED B indicates link activity */
	LED_MAX_EVENT
};

/** Configuration for various LED events */
struct cs4224_led_event_config {
	/** How LEDs should blink */
	enum cs4224_led_event_pattern	event_pattern;
	/** How bright LED should be, 0-31 where 31=100% */
	uint8_t duty_cycle;
	/** How long LED should be on in 250ms increments, 1=250, 2=500, etc. */
	uint8_t blink_on_time;
	/** How long LED should be off in 250ms increments, 1=250, 2=500, etc. */
	uint8_t blink_off_time;
};

#define CS4224_LINK_ACTIVITY_HOST_RX		(1 << 0)
#define CS4224_LINK_ACTIVITY_HOST_TX		(1 << 1)
#define CS4224_LINK_LINE_RX			(1 << 2)
#define CS4224_LINK_LINE_TX			(1 << 3)

/** For linkup events how the LED should behave */
enum cs4224_led_linkup_mode {
	/**
	 * No Link Up, LED is illuminated when a packet event occurs, unlit
	 * otherwise.
	 */
	LED_LINKUP_NO_LINK = 0,
	/**
	 * LED is illuminated when Line Rx PCS is synced with blinking off
	 * pattern proportional to packet events, unlit when no PCS sync.
	 */
	LED_LINKUP_LINK_LINE_RX = 1,
	/**
	 * LED is illuminated when Host RX PCS is synced with blinking off
	 * pattern proportional to packet events, unlit when no PCS sync.
	 */
	LED_LINKUP_LINK_HOST_RX = 2,
	/**
	 * LED is illuminated when Line Rx PCS and Host Rx PCS are synced with
	 * blinking off pattern proportional to packet events, unlit when no PCS
	 * sync.
	 */
	LED_LINKUP_LINK_HOST_LINE_RX = 3
};

/** LED configuration for link activity */
struct cs4224_led_link_activity {
	enum cs4224_led_linkup_mode	mode;	/** LED link behavior */
	/** Activity sample window size in ms, 0-4095ms */
	uint16_t			activity_sample_window;
	/** No activity sample window size in ms, 0-4095ms */
	uint16_t			inv_activity_sample_window;
	/** Mask to contribute to LED activity */
	uint8_t				link_mask;
};

struct cs4224_led_config {
	struct cs4224_led_event_config	cfg_event_link_active_a;
	struct cs4224_led_event_config	cfg_event_link_host_disable;
	struct cs4224_led_event_config	cfg_event_module_boot;
	struct cs4224_led_event_config	cfg_event_beacon;
	struct cs4224_led_event_config	cfg_event_tx_fault;
	struct cs4224_led_event_config	cfg_event_rx_los;
	struct cs4224_led_event_config	cfg_event_mod_abs;
	struct cs4224_led_event_config	cfg_event_edc_converged;
	struct cs4224_led_event_config	cfg_event_generic_gpio;
	struct cs4224_led_event_config	cfg_event_user_1;
	struct cs4224_led_event_config	cfg_event_user_2;
	struct cs4224_led_event_config	cfg_event_user_3;
	struct cs4224_led_event_config	cfg_event_link_active_b;
	struct cs4224_led_link_activity auto_link_active_a;
	struct cs4224_led_link_activity auto_link_active_b;
	/** Assign one event to each priority */
	enum cs4224_led_events priorities[LED_MAX_EVENT];
	/** 0 = bicolor LED, 1 = two single LEDs */
	int led_mode_select;
};

enum cs4224_gpio_output_src_sel {
	/** GPIOx_DRIVE register */
	CS4224_GPIO_OUTPUT_SRC_SEL_DRIVE = 0,
	/** GPIOx_DRIVE after delay */
	CS4224_GPIO_OUTPUT_SRC_DELAY,
	/** Outputs a toggling signal */
	CS4224_GPIO_OUTPUT_SRC_TOGGLE,
	/** Outputs signal from GPIOx_OUTPUT_CFG */
	CS4224_GPIO_OUTPUT_SRC_SEL_EXT,
	/** Outputs signal from GPIOx_OUTPUT_CFG delayed */
	CS4224_GPIO_OUTPUT_SRC_SEL_EXT_DELAY,
};

/** Source function for GPIO output */
enum cs4224_gpio_output_src_fn {
	/** Receive LOS (from DSP) */
	CS4224_GPIO_OUTPUT_RX_LOS		= 0,
	/** Receive LOL (from DSP) */
	CS4224_GPIO_OUTPUT_RX_LOL		= 1,
	/** EDC Converged (from DSP) */
	CS4224_GPIO_OUTPUT_EDC_CONVERGED	= 2,
	/** Receive PRBS sync */
	CS4224_GPIO_OUTPUT_RX_PRBS_SYNC		= 3,
	/** Common - Logic 0 */
	CS4224_GPIO_OUTPUT_LOGIC0		= 0,
	/** Common - GPIO1 input */
	CS4224_GPIO_OUTPUT_GPIO1_INPUT		= 1,
	/** Common - GPIO2 input */
	CS4224_GPIO_OUTPUT_GPIO2_INPUT		= 2,
	/** Common - GPIO3 input */
	CS4224_GPIO_OUTPUT_GPIO3_INPUT		= 3,
	/** Common - GPIO4 input */
	CS4224_GPIO_OUTPUT_GPIO4_INPUT		= 4,
	/** Common - INTERR input */
	CS4224_GPIO_OUTPUT_INTERR_INPUT		= 5,
	/** Common - interrupt output from GLOBAL_INT register */
	CS4224_GPIO_OUTPUT_GLOBAL_INT		= 6,
	/** Common - interrupt output from GPIO_INT register */
	CS4224_GPIO_OUTPUT_GPIO_INT		= 7,
	/** Common - Temp/voltage monitor interrupt */
	CS4224_GPIO_OUTPUT_GPIO_MONITOR_INT	= 8,
	/** Common - Selected clock output of global clock monitor */
	CS4224_GPIO_OUTPUT_GBL_CLKMON_CLK	= 9
};

struct cs4224_gpio_pin_config {
	/** Data source for GPIO output */
	enum cs4224_gpio_output_src_sel src_sel;
	/** Source function for GPIO output */
	enum cs4224_gpio_output_src_fn src_fn;
	/** Set to 1 to configure the pin, 0 to leave it alone */
	u16 pin_config:1;
	/** Invert the output */
	u16 invert_output:1;
	/** Invert the input */
	u16 invert_input:1;
	/**
	 * Pad voltage level selection, 0 = 2.5v/1.8v, 3.3v tolerant,
	 * 1 = GPIO pad 1.0V
	 */
	u16 iovddsel_1v0:1;
	/**
	 * Configures if GPIO output enable controled by 'outen' bit or by
	 * selected output source. 1 = selected output source, 0 = outen.
	 */
	u16 outen_ovr:1;
	/**
	 * Configures pad direction when outen_ovr = 1.
	 * 0  = input,  Pin is "high Z" if pulldn = 0 and pullup = 0.
	 * 1 = output enabled
	 */
	u16 outen:1;
	/**
	 * Open drain output, 0 = standard CMOS output, 1 = open drain output
	 * where a pullup resistor is required.
	 */
	u16 oden:1;
	/** Enable pad pulldown resistor */
	u16 pulldn:1;
	/** Enable pad pullup resistor */
	u16 pullup:1;
	/** Value to output in delay and drive modes */
	u16 value:1;
	/** Set to 0 for a host port (to SFP+), 1 for line port (to CPU) */
	u16 port_line:1;
	/** Padding */
	u16 :5;
	/** Toggle rate in ms (on period and off period) 1 = 2ms */
	u16 toggle_rate;
	/** Delay in ms */
	u16 on_delay;
	/** Port number to use for GPIO pin function */
	uint8_t port_number;
};

struct cs4224_gpio_config {
	/** Configuration for each GPIO pin, note that there is no GPIO pin 0 */
	struct cs4224_gpio_pin_config pins[16];

	/**
	 * Map GPIO pin 1-15 to GPIO input 1-11 (gpio input 0 not used,
	 * gpio pin 0 = logic 0
	 *
	 * GPIOs are typically mapped as:
	 *	1: MPC/RTCD
	 *	2: TX_FAULT
	 *	3: TX_DISABLED
	 *	4: RX_LOS
	 *	5: MOD_ABS
	 *	6: EDC_CONVERGED
	 *	7: LED0
	 *	8: LED1
	 *	9: GPIO/RTCC
	 *	10: GPIO_INT (general purpose input/output shared between PHYs)
	 *	11: SFP_SCL/MDC2
	 *	12: SFP_SDA/MDIO2
	 *	13: SYNCE_PRI Primary clock output, shared
	 *	14: SYNC_SEC Secondary clock output, shared
	 *	15: SYNCPULSE Sync pulse, shared
	 */
	u8 gpio_input_mapping[12];
	/**
	 * 0 = VDD select configured for each individual pin,
	 * 1 = external IO_VDD_SEL pin
	 */
	u16 vddsel_override:1;
};

/** Data structure used to configure LEDs and GPIO pins */
struct cs4224_pin_config {
	struct cs4224_gpio_config gpio0, gpio1;	/** GPIO configuration */
	struct cs4224_led_config led;		/** LED configuration */
};

/** Hardware ID read from CS4224_EFUSE_PDF_SKU register */
enum cs4224_hw_id {
	/** Undefined */
	CS4224_HW_UNDEF  = 0,

	/** 4 port duplex */
	CS4224_HW_CS4223 = 0x0010,

	/** 16 port simplex */
	CS4224_HW_CS4224 = 0x0011,

	/** 8 port duplex */
	CS4224_HW_CS4343 = 0x0012,

	/** 10 port simplex */
	CS4224_HW_CS4221 = 0x0013,

	/** 2 port duplex */
	CS4224_HW_CS4227 = 0x0014,

	/** 16 port simplex, 15G only */
	CS4224_HW_CS4210 = 0x0015,

	/** 8 port duplex, 10G only */
	CS4224_HW_CS4341 = 0x0016
};

enum cs4224_host_mode {
	CS4224_HOST_MODE_CX1	= 0x02,
	CS4224_HOST_MODE_SR	= 0x04,
	CS4224_HOST_MODE_ZR	= 0x10,
	CS4224_HOST_MODE_DWDM	= 0x20
};

/** The number of available ports or slices in the device   */
#define CS4224_MAX_NUM_CS4223_PORTS   4  /*  4 port duplex  */
#define CS4224_MAX_NUM_CS4224_PORTS  16  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4343_PORTS   8  /*  8 port duplex  */
#define CS4224_MAX_NUM_CS4221_PORTS  10  /* 10 port simplex */
#define CS4224_MAX_NUM_CS4227_PORTS   2  /*  2 port duplex  */
#define CS4224_MAX_NUM_CS4210_PORTS  16  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4341_PORTS   8  /*  8 port duplex */

/** The number of available dies in the device   */
#define CS4224_MAX_NUM_CS4223_DIES    1  /*  4 port duplex  */
#define CS4224_MAX_NUM_CS4224_DIES    2  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4343_DIES    2  /*  8 port duplex  */
#define CS4224_MAX_NUM_CS4221_DIES    2  /* 10 port simplex */
#define CS4224_MAX_NUM_CS4227_DIES    1  /*  2 port duplex  */
#define CS4224_MAX_NUM_CS4210_DIES    2  /* 16 port simplex */
#define CS4224_MAX_NUM_CS4341_DIES    2  /*  8 port duplex */

/** The difference between line and host PP registers */
#define CS4224_LINE_TO_HOST_OFFSET    0x0800

#endif /* __CS4224_H__ */
