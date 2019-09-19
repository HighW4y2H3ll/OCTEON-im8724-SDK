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
#ifndef __CS4321_H__
#define __CS4321_H__

/** LED patterns for blinking */
enum cs4321_led_event_pattern {
	LED_OFF = 0,		/** LEDs are off */
	LED_A_CONTINUOUS = 1,	/** LED A on continuously */
	LED_B_CONTINUOUS = 2,	/** LED B on continuously */
	LED_A_BLINK = 3,	/** LED A blink */
	LED_B_BLINK = 4,	/** LED B blink */
	LED_A_B_ALTERNATE = 5	/** Alternate between LEDs A and B */
};

/** Various events that can be configured for LEDs */
enum cs4321_led_events {
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
struct cs4321_led_event_config {
	/** How LEDs should blink */
	enum cs4321_led_event_pattern	event_pattern;
	/** How bright LED should be, 0-31 where 31=100% */
	uint8_t duty_cycle;
	/** How long LED should be on in 250ms increments, 1=250, 2=500, etc. */
	uint8_t blink_on_time;
	/** How long LED should be off in 250ms increments, 1=250, 2=500, etc. */
	uint8_t blink_off_time;
};

#define CS4321_LINK_ACTIVITY_HOST_RX		(1 << 0)
#define CS4321_LINK_ACTIVITY_HOST_TX		(1 << 1)
#define CS4321_LINK_LINE_RX			(1 << 2)
#define CS4321_LINK_LINE_TX			(1 << 3)

/** For linkup events how the LED should behave */
enum cs4321_led_linkup_mode {
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
struct cs4321_led_link_activity {
	enum cs4321_led_linkup_mode	mode;	/** LED link behavior */
	/** Activity sample window size in ms, 0-4095ms */
	uint16_t			activity_sample_window;
	/** No activity sample window size in ms, 0-4095ms */
	uint16_t			inv_activity_sample_window;
	/** Mask to contribute to LED activity */
	uint8_t				link_mask;
};

struct cs4321_led_config {
	struct cs4321_led_event_config	cfg_event_link_active_a;
	struct cs4321_led_event_config	cfg_event_link_host_disable;
	struct cs4321_led_event_config	cfg_event_module_boot;
	struct cs4321_led_event_config	cfg_event_beacon;
	struct cs4321_led_event_config	cfg_event_tx_fault;
	struct cs4321_led_event_config	cfg_event_rx_los;
	struct cs4321_led_event_config	cfg_event_mod_abs;
	struct cs4321_led_event_config	cfg_event_edc_converged;
	struct cs4321_led_event_config	cfg_event_generic_gpio;
	struct cs4321_led_event_config	cfg_event_user_1;
	struct cs4321_led_event_config	cfg_event_user_2;
	struct cs4321_led_event_config	cfg_event_user_3;
	struct cs4321_led_event_config	cfg_event_link_active_b;
	struct cs4321_led_link_activity auto_link_active_a;
	struct cs4321_led_link_activity auto_link_active_b;
	/** Assign one event to each priority */
	enum cs4321_led_events priorities[LED_MAX_EVENT];
	/** 0 = bicolor LED, 1 = two single LEDs */
	int led_mode_select;
};

enum cs4321_gpio_output_src_sel {
	/** GPIOx_DRIVE register */
	CS4321_GPIO_OUTPUT_SRC_SEL_DRIVE = 0,
	/** GPIOx_DRIVE after delay */
	CS4321_GPIO_OUTPUT_SRC_DELAY,
	/** Outputs a toggling signal */
	CS4321_GPIO_OUTPUT_SRC_TOGGLE,
	/** Outputs signal from GPIOx_OUTPUT_CFG */
	CS4321_GPIO_OUTPUT_SRC_SEL_EXT,
	/** Outputs signal from GPIOx_OUTPUT_CFG delayed */
	CS4321_GPIO_OUTPUT_SRC_SEL_EXT_DELAY,
};

/** Source function for GPIO output */
enum cs4321_gpio_output_src_fn {
	/** Logic 0 */
	CS4321_GPIO_OUTPUT_LOGIC_0		= 0,
	/** Line side receive LOS (from DSP) */
	CS4321_GPIO_OUTPUT_RX_LOS		= 1,
	/** EDC Converged (from DSP) */
	CS4321_GPIO_OUTPUT_EDC_CONVERGED	= 2,	/* Default GPIO 6 */
	/** Drive for LED0 */
	CS4321_GPIO_OUTPUT_LED0			= 3,	/* Default GPIO 7 */
	/** Drive for LED1 */
	CS4321_GPIO_OUTPUT_LED1			= 4,	/* Default GPIO 8 */
	/** Interrupt output from GLOBAL_INT register */
	CS4321_GPIO_OUTPUT_GLOBAL_INT		= 5,
	/** Interrupt output from GPIO_INT register */
	CS4321_GPIO_OUTPUT_GPIO_INT		= 6,	/* Default GPIO 10 */
	/** SFP Cache I2C clock */
	CS4321_GPIO_OUTPUT_SFP_SCL		= 7,	/* Default GPIO 11 */
	/** SFP Cache I2C data */
	CS4321_GPIO_OUTPUT_SFP_SDA		= 8,	/* Default GPIO 12 */
	/** SYNCPULSE Output from Egress PTP */
	CS4321_GPIO_OUTPUT_SYNCPULSE		= 9,	/* Default GPIO 15 */
	/** SYNCE Clock Primary output */
	CS4321_GPIO_OUTPUT_SYNCE_PRI		= 10,	/* Default GPIO 13 */
	/** SYNCE Clock Secondary output */
	CS4321_GPIO_OUTPUT_SYNCE_SEC		= 11,	/* Default GPIO 14 */
	/** Line side receive LOL */
	CS4321_GPIO_OUTPUT_RX_LOL		= 12,
	/** Host side receive LOL */
	CS4321_GPIO_OUTPUT_TX_LOL		= 13,
	/** Line side receive PRBS sync */
	CS4321_GPIO_OUTPUT_RX_PRBS_SYNC		= 15,
	/* Host side receive PRBS sync */
	CS4321_GPIO_OUTPUT_TX_PRBS_SYNC		= 16,
	/** Microsequencer watchdog timeout */
	CS4321_GPIO_OUTPUT_WATCHDOG_TIMEOUT	= 17,
	/** GPIO input mapped to GPPI_MUXED_INPUT_1 */
	CS4321_GPIO_OUTPUT_GPIO_MUXED_INPUT1	= 23,
	/** GPIO input mapped to GPPI_MUXED_INPUT_2 */
	CS4321_GPIO_OUTPUT_GPIO_MUXED_INPUT2	= 24,
	/** GPIO input mapped to GPPI_MUXED_INPUT_3 */
	CS4321_GPIO_OUTPUT_GPIO_MUXED_INPUT3	= 25,
	/** GPIO input mapped to GPPI_MUXED_INPUT_4 */
	CS4321_GPIO_OUTPUT_GPIO_MUXED_INPUT4	= 26,
	/** PTP Memory valid indication */
	CS4321_GPIO_OUTPUT_PTP_MEM_VAL		= 27,
	/** Host side ML receive PRBS sync */
	CS4321_GPIO_OUTPUT_ML_PRBS_SYNC		= 28,
	/** Host side receive LOS (TX LOS) */
	CS4321_GPIO_OUTPUT_HOST_RX_LOS		= 29,	/* Default GPIO 4 */
	/** Temperature monitor interrupt */
	CS4321_GPIO_OUTPUT_TEMPMON_INT		= 30,
	/** Voltage monitor interrupt */
	CS4321_GPIO_OUTPUT_VOLTMON_INT		= 31,
	/** SFP Cache alarm interrupt */
	CS4321_GPIO_OUTPUT_SFP_CACHE_ALARM	= 32,
	/** SYNCPULSE Out from Ingress PTP */
	CS4321_GPIO_OUTPUT_SYNCPULSE_ING	= 33	/* Default GPIO 15 */
};

enum cs4321_gpio_input_function {
	/** Input disabled */
	CS4321_GPIO_IN_FCN_DISABLED		= 0,
	/** Maps input to external LOS signal */
	CS4321_GPIO_IN_FCN_EXT_LOS		= 1,
	/** Maps input to external module absent signal */
	CS4321_GPIO_IN_FCN_MOD_ABS		= 2,
	/** Map the input to the TX fault signal */
	CS4321_GPIO_IN_FCN_TX_FAULT		= 3,
	/** Map the input to the external trigger signal */
	CS4321_GPIO_IN_FCN_EXT_TRIGGER		= 4,
	/** Map the input to muxed input 1 */
	CS4321_GPIO_IN_FCN_GPIO_MUXED_INPUT1	= 5,
	/** Map the input to muxed input 2 */
	CS4321_GPIO_IN_FCN_GPIO_MUXED_INPUT2	= 6,
	/** Map the input to muxed input 3 */
	CS4321_GPIO_IN_FCN_GPIO_MUXED_INPUT3	= 7,
	/** Map the input to muxed input 4 */
	CS4321_GPIO_IN_FCN_GPIO_MUXED_INPUT4	= 8,
	/** Map the input to the SFP SCL signal */
	CS4321_GPIO_IN_FCN_SFP_SCL		= 9,
	/** Map the input to the SFP SDA signal */
	CS4321_GPIO_IN_FCN_SFP_SDA		= 10,
	/** Map the input to the SYNCPULSE signal */
	CS4321_GPIO_IN_FCN_SYNCPULSE		= 11
};

struct cs4321_gpio_pin_config {
	/** Data source for GPIO output */
	enum cs4321_gpio_output_src_sel src_sel;
	/** Source function for GPIO output */
	enum cs4321_gpio_output_src_fn src_fn;
	/** Input function */
	enum cs4321_gpio_input_function in_fn;
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
	/** Padding */
	u16 :6;
	/** Toggle rate in ms (on period and off period) 1 = 2ms */
	u16 toggle_rate;
	/** Delay in ms */
	u16 on_delay;
};

struct cs4321_gpio_config {
	/** Configuration for each GPIO pin, note that there is no GPIO pin 0 */
	struct cs4321_gpio_pin_config pins[16];

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
struct cs4321_pin_config {
	struct cs4321_gpio_config gpio0, gpio1;	/** GPIO configuration */
	struct cs4321_led_config led;		/** LED configuration */
};

#endif /* __CS4321_H__ */
