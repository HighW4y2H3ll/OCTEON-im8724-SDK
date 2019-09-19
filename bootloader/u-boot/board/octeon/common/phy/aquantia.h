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

#ifndef __OCTEON_PHY_AQUANTIA_H__
#define __OCTEON_PHY_AQUANTIA_H__

/** Image ID string length */
#define AQ_PHY_IMAGE_ID_STR_LEN			62

/** Size of version string in formware */
#define AQ_PHY_VERSION_STRING_SIZE		0x40

/** Byte offset from the top of DRAM to the FW image string */
#define AQ_PHY_VERSION_STRING_BLOCK_OFFSET	0x200

/** The byte address, in processor memory, of the start of the DRAM segment */
#define AQ_PHY_DRAM_BASE_ADDRESS		0x3ffe0000

/** The byte address, in processor memory, of the start of the IRAM segment */
#define AQ_PHY_IRAM_BASE_ADDRESS		0x40000000

/** Image header content offset (28nm only) */
#define AQ_PHY_IMAGE_HEADER_CONTENT_OFFSET	0x300

/* The offset from the start of DRAM where the provisioning block begins */
#define AQ_PHY_IMAGE_PROVTABLE_OFFSET		0x680

/**
 * The offset from the start of DRAM where the provisioning block's ending
 * address is recorded.
 */
#define AQ_PHY_IMAGE_PROVTABLE_TERM_OFFSET	0x28c

/**
 * The size of all space alloted within the PHY image for the provisioning table
 */
#define AQ_PHY_IMAGE_PROVTABLE_MAXSIZE		0x800

/** The maximum size of the debug trace buffer */
#define AQ_PHY_MAX_TRACE_BUFFER_LENGTH		8192

/**
 * Maxumum allowed number of times to poll for debug-trace-freeze
 * acknowledgement
 */
#define AQ_PHY_MAX_FREEZE_CHECKS		2000

/** Time to wait for a UP operation to finish */
#define AQ_PHY_UP_TIMEOUT_MS			5000

/** Time to wait for basic cable diagnostics to finish */
#define AQ_PHY_BASIC_CABLE_DIAG_MS		10000

#define AQ_PHY_SERDESEYE_GRIDSIZE_X 32
#define AQ_PHY_SERDESEYE_GRIDSIZE_Y 64
#define AQ_PHY_MAX_SERDES_EYE_CHECKS	50
#define AQ_PHY_MAX_COMMAND_STATUS_POLLS	700

#define AQ_PHY_FLASH_INTERFACE_MAX_POLL_COUNT	20000

/** The maximum number of ports that can be MDIO bootloaded at once */
#define AQ_PHY_MAX_PHY_IDS		48

enum aq_phy_led_stretch {
	LS_OFF,
	LS_28ms,
	LS_60ms,
	LS_100ms
};

struct aq_phy_led_control {
	bool	led_on;	/** Manually turn LED on */
	bool	led_on_10g_link_up;
	bool	led_on_5g_link_up;
	bool	led_on_2_5g_link_up;
	bool	led_on_1g_link_up;
	bool	led_on_100m_link_up;
	bool	led_on_connecting;
	bool	led_toggle_on_rx;
	bool	led_toggle_on_tx;
	enum aq_phy_led_stretch	led_stretch;
};

enum aq_phy_flash_type {
	/** Atmel 64K flash */
	F_AT25F512A,
	/** Atmel Smart 512K flash */
	F_AT45DB041D,
	/** Numonyx 64K flash */
	F_M25P05A,
	/** Atmel 256K flash */
	F_AT25DF021A,
	/** Atmel 512K flash */
	F_AT25DF041A,
	/** Numonyx 512K flash */
	F_M25P40,
	/** Macronix 512K flash */
	F_MX25V4005C,
	/** No flash, daisy chained slave */
	F_DAISY_CHAIN_SLAVE,
	/** Flash absent or unknown */
	F_INDETERMINATE,
	/** Adesto Smart 4Mbyte flash */
	F_AT45DB321E
};

struct aq_phy_image_info {
	bool programmed;
	char image_id_str[AQ_PHY_IMAGE_ID_STR_LEN];
	uint8_t image_rev_maj;
	uint8_t image_rev_min;
	uint8_t image_rom_id_num;
};

enum aq_phy_serdes_10g_start_mode {
	SSSM_10G_ON,
	SSSM_XSGMII_ON,
	SSSM_1000BASE_X_ON,
	SSSM_OFF,
	SSSM_OTHER
};

enum aq_phy_10g_system_op_mode {
	SOM_10G_XFI,
	SOM_10G_RXAUI_MARVELL,
	SOM_10G_RXAUI_DUNE,
	SOM_10G_XAUI,
	SOM_10G_OTHER
};

enum aq_phy_10g_network_op_mode {
	NOM_10G_10GBASE_T,
	NOM_10G_XFI,
	NOM_10G_OTHER
};

enum aq_phy_1g_network_op_mode {
	NOM_1G_1000BASE_T,
	NOM_1G_1000BASE_X,
	NOM_1G_OTHER
};

enum aq_phy_wol_rate {
	WOL_100BASE_TX,
	WOL_1000BASE_T,
	WOL_NONE
};

enum aq_phy_2_5g_op_mode {
	NOM_2_5G_XAUI,		/** 10G XAUI mode is used */
	NOM_2_5G_RXAUI,		/** 10G RXAUI mode is used */
	NOM_2_5G_XFI,		/** 10G XFI mode is used */
	NOM_2_5G_OTHER,		/** Unsupported/reserved */
	NOM_2_5G_2500BASE_X	/** 2500BASE-X mode is used */
};

struct aq_phy_wake_up_frame {
	uint32_t filter[4];	/** 128-bit WoL frame filter */
	uint32_t crc32;
	bool enable;
};

struct aq_phy_magic_packet_frame {
	uint8_t	mac_filter[6];
	bool enable;
};

struct aq_phy_config {
	int16_t low_temp_warn_thresh;
	int16_t low_temp_fail_thresh;
	int16_t hi_temp_warn_thresh;
	int16_t hi_temp_fail_thresh;
	bool disable_crc8_frame_trashing;

	struct aq_phy_led_control 		led_control[6];
	enum aq_phy_wol_rate			wol_control;
	struct aq_phy_wake_up_frame		wol_frame[5];
	struct aq_phy_magic_packet_frame	magic_packet_frame;
};

enum aq_phy_daisy_chain_setting {
	DC_SLAVE,
	DC_MASTER
};

struct aq_phy_static_config {
	uint32_t	oui;
	uint8_t		ieee_model_num;
	uint8_t		ieee_rev_num;
	uint8_t		firmware_major_rev;
	uint8_t		firmware_minor_rev;
	uint8_t		firmware_rom_id;
	enum aq_phy_daisy_chain_setting	daisy_chain_setting;
	bool		tx_enable;
	bool		led[6];
	enum aq_phy_10g_system_op_mode	sys_op_mode_10g;
	enum aq_phy_10g_network_op_mode	net_op_mode_10g;
	enum aq_phy_1g_network_op_mode	net_op_mode_1g;
	uint16_t	flash_clock_div;
	bool		mdio_open_drain;
	bool		mdio_preamble_suppression;
	bool		addr_0_broadcast_enable;
	bool		msw_read_first;
	bool		mdi_swap;
	bool		xfi0_tx_invert;
	bool		xfi0_rx_invert;
	bool		xfi1_tx_invert;
	bool		xfi1_rx_invert;
	uint8_t		rxaui_tx_serdes_lane0;
	uint8_t		rxaui_tx_serdes_lane1;
	bool		rxaui_tx_lane0_invert;
	bool		rxaui_tx_lane1_invert;
	uint8_t		rxaui_rx_serdes_lane0;
	uint8_t		rxaui_rx_serdes_lane1;
	bool		rxaui_rx_lane0_invert;
	bool		rxaui_rx_lane1_invert;

	uint8_t		xaui_tx_serdes_lane0;
	uint8_t		xaui_tx_serdes_lane1;
	uint8_t		xaui_tx_serdes_lane2;
	uint8_t		xaui_tx_serdes_lane3;
	bool		xaui_tx_lane0_invert;
	bool		xaui_tx_lane1_invert;
	bool		xaui_tx_lane2_invert;
	bool		xaui_tx_lane3_invert;
	uint8_t		xaui_rx_serdes_lane0;
	uint8_t		xaui_rx_serdes_lane1;
	uint8_t		xaui_rx_serdes_lane2;
	uint8_t		xaui_rx_serdes_lane3;
	bool		xaui_rx_lane0_invert;
	bool		xaui_rx_lane1_invert;
	bool		xaui_rx_lane2_invert;
	bool		xaui_rx_lane3_invert;

	uint8_t		sgmii_tx_serdes;
	uint8_t		sgmii_rx_serdes;
	bool		sgmii_tx_invert;
	bool		sgmii_rx_invert;

	enum aq_phy_serdes_10g_start_mode	start_mode_10g;
};

struct aq_phy_flash_info {
	/** CRC of bytes loaded from the flash interface */
	uint16_t flash_load_crc;
	/** Type of flash device */
	enum aq_phy_flash_type flash_type;
	/** Information on the image */
	struct aq_phy_image_info image_info;
};

enum aq_phy_cable_diag_pair_status {
	/** No problems detected */
	AQ_PHY_BASIC_PAIR_STATUS_OK,
	/** The pair is shorted. */
	AQ_PHY_BASIC_PAIR_STATUS_SHORT,
	/** The pair has lower than expected impedance. */
	AQ_PHY_BASIC_PAIR_STATUS_LOW_MISMATCH,
	/** The pair has higher than expected impedance. */
	AQ_PHY_BASIC_PAIR_STATUS_HIGH_MISMATCH,
	/** the pair is open. */
	AQ_PHY_BASIC_PAIR_STATUS_OPEN,
	/** The pair is crossed-over with pair A. */
	AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_A,
	/** The pair is crossed-over with pair B. */
	AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_B,
	/** The pair is crossed-over with pair C. */
	AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_C,
	/** The pair is crossed-over with pair D. */
	AQ_PHY_BASIC_PAIR_STATUS_CROSSOVER_D
};

struct aq_phy_cable_diag_pair_result {
	/** Pair status reported by diagnostics */
	enum aq_phy_cable_diag_pair_status status;
	/**
	 * Approximate distance, in meters to the first of the four worst
	 * reflections recorded during the basic cable diagnostic routine.
	 */
	uint32_t reflection_1_distance;
	/**
	 * Approximate distance, in meters to the second of the four worst
	 * reflections recorded during the basic cable diagnostic routine.
	 */
	uint32_t reflection_2_distance;
	/**
	 * Approximate distance, in meters to the third of the four worst
	 * reflections recorded during the basic cable diagnostic routine.
	 */
	uint32_t reflection_3_distance;
	/**
	 * Approximate distance, in meters to the fourth of the four worst
	 * reflections recorded during the basic cable diagnostic routine.
	 */
	uint32_t reflection_4_distance;
};

/** Basic cable diagnostics results */
struct aq_phy_cable_basic_diag_results {
	/** Result from pair A */
	struct aq_phy_cable_diag_pair_result pair_a_result;
	/** Result from pair B */
	struct aq_phy_cable_diag_pair_result pair_b_result;
	/** Result from pair C */
	struct aq_phy_cable_diag_pair_result pair_c_result;
	/** Result from pair D */
	struct aq_phy_cable_diag_pair_result pair_d_result;
};

enum aq_phy_state {
	AQ_PHY_STATE_LOW_POWER,
	AQ_PHY_STATE_HI_Z,
	AQ_PHY_STATE_RESTART_AUTONEG,
	AQ_PHY_STATE_ACTIVE
};

struct aq_phy_status {
	int16_t	temperature;
	struct aq_phy_image_info	loaded_image_info;
};

/** After autonegotiation the phy can be either a master or slave */
enum aq_phy_autoneg_ms_status {
	MSS_MASTER,
	MSS_SLAVE
};

/** Report pair connection and inversion status for MDI */
enum aq_phy_pair_status {
	PS_PAIR_A,	/** Connection to pair A */
	PS_PAIR_A_INV,	/** Inverted connection to pair A */
	PS_PAIR_B,	/** Connection to pair B */
	PS_PAIR_B_INV,	/** Inverted connection to pair B */
	PS_PAIR_C,	/** Connection to pair C */
	PS_PAIR_C_INV,	/** Inverted connection to pair C */
	PS_PAIR_D,	/** Connection to pair D */
	PS_PAIR_D_INV	/** Inverted connection to pair D */
};

enum aq_phy_conn_state {
	CONS_RESET,
	CONS_STALL,
	CONS_INACTIVE,
	CONS_CABLE_DIAGNOSTICS,
	CONS_10G_AUTONEG,
	CONS_1G_AUTONEG,
	CONS_100M_AUTONEG,
	CONS_10G_TRAINING,
	CONS_1G_TRAINING,
	CONS_100M_TRAINING,
	CONS_10G_CONNECTED,
	CONS_1G_CONNECTED,
	CONS_100M_CONNECTED,
	CONS_100M_WOL_CONNECTED,
	CONS_1G_WOL_CONNECTED,
	CONS_LINK_BREAK,
	CONS_TEST_MODE,
	CONS_LOOPBACK_MODE,
	CONS_LOW_POWER_MODE,
	CONS_INVALID
};

enum aq_phy_mdi_mdix_state {
	MS_NONE,
	MS_MDI,
	MS_MDI_X
};

/** System interface operating mode */
enum aq_phy_system_intf_op_mode {
	SIOM_XFI,
	SIOM_BACKPLANE_KR,
	SIOM_BACKPLANE_KX,
	SIOM_XAUI,
	SIOM_XAUI_PAUS_BASED,
	SIOM_RXAUI,
	SIOM_SGMII,
	SIOM_MAC,
	SIOM_OFF,
	SIOM_OTHER
};

/** System interface rate */
enum aq_phy_system_intf_rate {
	SIR_10M,
	SIR_100M,
	SIR_1G,
	SIR_10G,
	SIR_POWER_DOWN,
	SIR_OTHER
};

/** System interface autonegotiation status */
enum aq_phy_system_intf_autoneg_status {
	SIAS_NA,
	SIAS_NOT_COMPLETE,
	SIAS_COMPLETE,
	SIAS_OTHER
};

/** Connection status */
struct aq_phy_conn_status {
	/** True if far device exists (connection established) */
	bool		far_device_exists;
	/** True if more than -15dBm is present on the line */
	bool		energy_online;
	/**
	 * Number of autonegotiation attempts since power-up or a valid
	 * connection was lost.
	 */
	uint16_t	num_connect_attempts;
	/** Current state of the phy */
	enum aq_phy_conn_state	state;
	/** True if full duplex */
	bool		full_duplex;
	/** MDI/MDI-X state */
	enum aq_phy_mdi_mdix_state	mdi_mdi_x;
	/** Master/slave result from autonegotiation */
	enum aq_phy_autoneg_ms_status	master_slave_status;
	/** Current operating mode of system interface */
	enum aq_phy_system_intf_op_mode	sys_intf_current_mode;
	/** Current data rate of system interface */
	enum aq_phy_system_intf_rate	sys_intf_current_rate;
	/** True if system interface is ready to transmit */
	bool		tx_ready;
	/** True if the system interface is linked and ready to receive */
	bool		rx_link_up;
	/** Autonegotiation status */
	enum aq_phy_system_intf_autoneg_status sys_intf_autoneg_status;
};

struct aq_phy_10g_connection_status {
	bool		connected;
	uint32_t	num_err_ldpc_frames;
	uint32_t	num_err_crc_frames;
	uint32_t	num_iter1_corrected_ldpc_frames;
	uint32_t	num_iter2_corrected_ldpc_frames;
	uint32_t	num_iter3_corrected_ldpc_frames;
	uint32_t	num_iter4_corrected_ldpc_frames;
	uint32_t	num_iter5_corrected_ldpc_frames;
	uint32_t	num_iter6_corrected_ldpc_frames;
	uint32_t	num_iter7_corrected_ldpc_frames;
	uint32_t	num_iter8_corrected_ldpc_frames;
	uint32_t	num_good_ldpc_frames;
	uint8_t		average_num_iterations;
	int16_t		transmit_power;
	int16_t		link_partner_transmit_power;
	uint32_t	net_intf_good_tx_enet_frames;
	uint32_t	net_intf_good_rx_enet_frames;
	uint32_t	net_intf_bad_tx_enet_frames;
	uint32_t	net_intf_bad_rx_enet_frames;
	uint32_t	sys_intf_good_tx_enet_frames;
	uint32_t	sys_intf_good_rx_enet_frames;
	uint32_t	sys_intf_bad_tx_enet_frames;
	uint32_t	sys_intf_bad_rx_enet_frames;
	uint16_t	pair_a_oper_margin;
	uint16_t	pair_b_oper_margin;
	uint16_t	pair_c_oper_margin;
	uint16_t	pair_d_oper_margin;

	enum aq_phy_pair_status	pair_a_logical_connectivity;
	enum aq_phy_pair_status	pair_b_logical_connectivity;
	enum aq_phy_pair_status	pair_c_logical_connectivity;
	enum aq_phy_pair_status	pair_d_logical_connectivity;

	int16_t pair_b_relative_skew;
	int16_t pair_c_relative_skew;
	int16_t pair_d_relative_skew;
};

struct aq_phy_100m_1g_connection_status {
	bool		connected;
	uint32_t	sys_intf_good_tx_enet_frames;
	uint32_t	sys_intf_good_rx_enet_frames;
	uint32_t	sys_intf_bad_tx_enet_frames;
	uint32_t	sys_intf_bad_rx_enet_frames;
	uint16_t	sgmii_rx_false_carrier_events;
	uint16_t	sgmii_collision_events;
	uint16_t	rx_false_carrier_events;
	uint16_t	collision_events;
	uint32_t	frame_alignment_errors;
	uint32_t	runt_frame_events;
};

enum aq_phy_master_slave_control {
	AN_AUTOMATIC,
	AN_MASTER,
	AN_SLAVE
};

enum aq_phy_port_type {
	AN_SINGLE_PORT,
	AN_MULTI_PORT
};

enum aq_phy_mdi_mdix_control {
	MDC_AUTOMATIC,
	MDC_MDI,
	MDC_MDI_X_
};

enum aq_phy_force_conn_rate {
	FORCE_RATE_AUTONEG,
	FORCE_RATE_100M,
	FORCE_RATE_OTHER
};

struct aq_phy_autoneg_control {
	bool		adv_10gbase_t;
	/** advertise short reach capability */
	bool		adv_short_reach;
	bool		adv_1000base_t_full_dup;
	bool		adv_1000base_t_half_dup;
	bool		adv_100base_t_full_dup;
	bool		adv_100base_t_half_dup;
	bool		adv_10base_t_full_dup;
	bool		adv_10base_t_half_dup;
	bool		adv_symmetric_pause;
	bool		adv_asymmetric_pause;
	bool		enable_auto_downshift;
	uint16_t	auto_downshift_retry_attempts;
	enum aq_phy_master_slave_control	master_slave_control;
	enum aq_phy_port_type			port_type;
	enum aq_phy_mdi_mdix_control		mdi_mdi_x_control;
};

struct aq_phy_link_partner_status {
	bool	has_10gbase_t;
	bool	has_1000base_t_full_duplex;
	bool	has_1000base_t_half_duplex;
	bool	has_100base_t_full_duplex;
	bool	has_100base_t_half_duplex;
	bool	has_10base_t_full_duplex;
	bool	has_10base_t_half_duplex;
	bool	has_short_reach;
	bool	has_symmetric_pause;
	bool	has_asymmertric_pause;
	bool	is_aquantia_device;
	uint8_t	aq_chip_id;
	uint8_t	aq_chip_version;
};

struct aq_phy_autoneg_alarms {
	/** True if the PHY has experienced a loss of link */
	bool	link_fail;
	/** Remote fault indication received from link partner */
	bool	remote_fault;
	/** Master/slave configuration fault */
	bool	master_slave_config_fault;
	/** Autonegotiation completed but not for a mutually supported rate */
	bool	autoneg_completed_for_unsupported_rate;
	/** Autonegotiation completed for a mutually supported link */
	bool	autonegotation_completed_for_supported_rate;
	/** The PHY has downshifted to a lower advertised rate */
	bool	auto_downshift;
	/** The link partner is 10baseT device */
	bool	detected_10base_t;
	/** The PHY received a wake-up frame */
	bool	detected_wol_frame;
	/** The PHY received a wake-up packet */
	bool	detected_wol_packet;
	/** If true, the link has been established or was disconnected */
	bool	link_status_change;
};

struct aq_phy_10gbase_t_alarms {
	/** If true the PHY is operating with a BER > 10^-4 */
	bool	high_ber;
	/** Loss of frame received on 10GBase-T data stream */
	bool	loss_of_block_lock;
	/** PHY experienced a CRC error on the received 10GBase-T frame */
	bool	crc8_error;
	/**
	 * PHY received a 10GBase-T frame that contains uncorrectable
	 * bit errors
	 */
	bool	ldpc_error;
	/** PHY received in 10GBase-T frame with an invalid 65b block */
	bool	invalid_65b_block;
	/** True if PHY experienced loss of link */
	bool	pma_link_fail;
	/**
	 * True if there was a PMA hardware transmit fault.
	 * This should not occur during normal operation.
	 */
	bool	pma_tx_fault;
	/**
	 * True if there was a PMA hardware receive fault.
	 * This should not occur during normal operation.
	 */
	bool	pma_rx_fault;
	/** True if PHY PCS has experienced loss of link */
	bool	pcs_link_fail;
	/**
	 * True if there was a PCS hardware transmit fault.  This should not
	 * occur during normal operation.
	 */
	bool	pcs_tx_fault;
	/**
	 * True if there was a PCS hardware receive fault.  This should not
	 * occur during normal operation.
	 */
	bool	pcs_rx_fault;
};

struct aq_phy_sgmii_alarms {
	/** Set if PHY received an invalid code on the SGMII rx interface */
	bool	code_violation_error;
	/** True if PHY has lost signal on the SGMII rx interface */
	bool	loss_of_signal;
	/**
	 * True if PHY received a running disparity error on the SGMII rx
	 * interface
	 */
	bool	running_disparity_error;
};

struct aq_phy_xaui_alarms {
	/** True if PHY has lost lane alignment on XAUI or RXAUI rx interface */
	bool	loss_of_lane_alignment;
	/** True if PHY has lost signal on the corresponding XAUI or RXAUI lane */
	bool	loss_of_signal[4];
	/**
	 * True if PHY has deleted a sequence ordered set on the XAUI rx
	 * interface.
	 */
	bool	sequence_ordered_set_deletion;
	/**
	 * True if PHY has received a code violation error on the corresponding
	 * XAUI or RXAUI rx lane.
	 */
	bool	code_violation_error[4];
	/**
	 * True if the PHY has received a running disparity error on the
	 * corresponding XAUI or RXAUI rx lane.
	 */
	bool	running_disparity_error[4];
	/** Tx fault in the PHY_XS.  This is a hardware fault */
	bool	phy_xs_tx_fault;
	/** Rx fault in the PHY_XS.  This is a hardware fault */
	bool	phy_xs_rx_fault;
};

struct aq_phy_xfi_alarms {
	/** True if the XFI interface is experiencing high BER on the rx signal */
	bool	high_ber;
	/** True if XFI interface has lost block lock on the 66b framer */
	bool	loss_of_block_lock;
	/** True if XFI interface has received an invalid 66b block */
	bool	invalid_66b_block;
};

struct aq_phy_serdes_alarms {
	/** True if the system interface rx link has gone down */
	bool	sys_intf_rx_link_down;
	/** True if the system interface tx link has gone donw */
	bool	sys_intf_rx_link_up;
};

struct aq_phy_test_alarms {
	/**
	 * True if PHY has experienced loss of synchroniztion on CRPAT test
	 * signal
	 */
	bool	crpat_loss_of_sync;
	/**
	 * True if PHY has experienced loss of synchronization on PRBS pattern
	 * on the corresponding XAUI or RXAUI receive lane.
	 */
	bool	xaui_prbs_loss_of_sync[4];
};

/** Global alarms */
struct aq_phy_global_alarms {
	/** True if temperature exceeded high temperature failure theshold */
	bool	high_temp_failure;
	/** True if temperature fell below the low temperature failure threshold */
	bool	low_temp_failure;
	/** True if temperature exceeded high temperature warning theshold */
	bool	high_temp_warning;
	/** True if temperature fell below t he low temperature warning threshold */
	bool	low_temp_warning;
	/** True if the reset sequence has completed */
	bool	reset_complete;
	/** True if state has changed (see aq_phy_get_connection_status()) */
	bool	state_change;
	/** True if PHY has experienced a device fault */
	bool	device_fault;
	/** True if PHY has detected the presence of an IP phone device */
	bool	ip_phone;
	/** True if MDIO command handling buffer has overflowed */
	bool	mdio_command_overflow;
	/** True if MDIO operation has timed out */
	bool	mdio_timeout;
	/** True if PHY has entered smart power-down state */
	bool	smart_power_down;
	/** True if the PHY's watchdog timer has expired */
	bool	watchdog_timer;
};

/** Status of various alarms */
struct aq_phy_alarm_status {
	/**
	 * If a device fault has occurred this contains the 16-bit diagnostic
	 * code associated with the fault.
	 */
	int	device_fault_code;
	/** True if one of the autonegotiation alarms has been set */
	bool	alarm_autoneg_set;
	/** True if one of the 10GBase-T alarms has been set */
	bool	alarm_10gbase_t_set;
	/** True if one of the XAUI alarms has been set */
	bool	alarm_xaui_set;
	/** True if one of the XFI (lookaside) alarms has been set */
	bool	alarm_xfi0_set;
	/** True if one of the XFI (system) alarms has been set */
	bool	alarm_xfi1_set;
	/** True if one of the SGMII (lookaside) alarms has been set */
	bool	alarm_sgmii0_set;
	/** True if one of the SGMII (system) alarms has been set */
	bool	alarm_sgmii1_set;
	/** True if one of the SERDES alarms has been set */
	bool	alarm_serdes_set;
	/** True if one of the test alarms has been set */
	bool	alarm_test_set;
	/** True if one of the global alarms has been set */
	bool	alarm_global_set;
	/** Autonegotiation related alarms */
	struct aq_phy_autoneg_alarms	alarms_autoneg;
	/** 10GBase-T alarms */
	struct aq_phy_10gbase_t_alarms	alarms_10gbase_t;
	/** XAUI/RXAUI alarms */
	struct aq_phy_xaui_alarms	alarms_xaui;
	/** XFI lookaside port alarms */
	struct aq_phy_xfi_alarms	alarms_xfi0;
	/** XFI system interface alarms */
	struct aq_phy_xfi_alarms	alarms_xfi1;
	/** SGMII lookaside port alarms */
	struct aq_phy_sgmii_alarms	alarms_sgmii0;
	/** SGMII system interface alarms */
	struct aq_phy_sgmii_alarms	alarms_sgmii1;
	/** SERDES related alarms */
	struct aq_phy_serdes_alarms	alarms_serdes;
	/** Test alarms */
	struct aq_phy_test_alarms	alarms_test;
	/** Global alarms */
	struct aq_phy_global_alarms	alarms_global;
};

/**
 * Controls for enabling and disabling alarm interrupts
 */
struct aq_phy_alarm_control {
	/** Autonegotiation related alarms */
	struct aq_phy_autoneg_alarms	alarm_mask_autoneg;
	/** 10GBase-T related alarms */
	struct aq_phy_10gbase_t_alarms	alarm_mask_10gbase_t;
	/** XAUI/RXAUI related alarms */
	struct aq_phy_xaui_alarms	alarm_mask_xaui;
	/** XFI 0 related alarms */
	struct aq_phy_xfi_alarms	alarm_mask_xfi0;
	/** XFI 1 related alarms */
	struct aq_phy_xfi_alarms	alarm_mask_xfi1;
	/** SGMII 0 related alarms */
	struct aq_phy_sgmii_alarms	alarm_mask_sgmii0;
	/** SGMII 1 related alarms */
	struct aq_phy_sgmii_alarms	alarm_mask_sgmii1;
	/** SERDES related alarms */
	struct aq_phy_serdes_alarms	alarm_mask_serdes;
	/** Test related alarms */
	struct aq_phy_test_alarms	alarm_mask_test;
	/** Global alarms */
	struct aq_phy_global_alarms	alarm_mask_global;
};

/** Loopback more for the Aquantia PHY (not managed by the processor) */
enum aq_phy_loopback_control {
	/** No phy loopback */
	AQ_PHY_NO_LOOPBACK,
	/** Network loopback on the system interface */
	AQ_PHY_SYS_INTF_NETWORK_LOOPBACK,
	/** System loopback on the network interface */
	AQ_PHY_NET_INTF_SYSTEM_LOOPBACK
};

/** Loopback mode managed by the processor */
enum aq_phy_fw_loopback {
	/** No loopback mode */
	AQ_FWLC_NO_LOOPBACK,
	/** System loopback test mode with system interface */
	AQ_FWLC_SYS_INTF_SYS_LOOPBACK,
	/** System loopback test mode with system interface and passthrough */
	AQ_FWLC_SYS_INTF_SYS_LOOPBACK_PASSTHROUGH,
	/** Network loopback with system interface */
	AQ_FWLC_SYS_INTF_NET_LOOPBACK,
	/** Network loopback with system interface and passthrough */
	AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH,
	/**
	 * Network loopback with system interface and passthrough.  Redirected
	 * traffic will be non-destructively merged with system traffic.
	 */
	AQ_FWLC_SYS_INTF_NET_LOOPBACK_PASSTHROUGH_MERGE,
	/**
	 * Loopback test with network interface.  System traffic will be
	 * redirected back to the system.
	 */
	AQ_FWLC_NET_INTF_SYS_LOOPBACK,
	/**
	 * Loopback test with network interface.  System traffic will be
	 * redirected back to the system.  Traffic will also be passed through
	 * to the network.
	 */
	AQ_FWLC_NET_INTF_SYS_LOOPBACK_PASSTHROUGH,
	/**
	 * System loopback test within the network interface.  Traffic from the
	 * system will be redirected towards the system.  A loopback plug must
	 * be inserted into the MDI port.
	 */
	AQ_FWLC_NET_INTF_SYS_PLUG_LOOPBACK,
	/**
	 * Network loopback test mode within the network interface.  Traffic
	 * from the network will be redirected towards the network.
	 */
	AQ_FWLC_NET_INTF_NET_LOOPBACK,
	/**
	 * Network loopback test mode within the network interface.  Traffic
	 * from the network will be redirected towards the network as well as
	 * passed through to the system.
	 */
	AQ_FWLC_NET_INTF_NET_LOOPBACK_PASSTHROUGH,
	/**
	 * System loopback test mode between the network and system interfaces.
	 * Traffic from the network will be redirected back to the network.
	 */
	AQ_FWLC_CROSS_CONNECT_SYS_LOOPBACK,
	/**
	 * Network loopback within the cross connect between the network and
	 * system interfaces.  Traffic from the network will be redirected back
	 * towards the network.
	 */
	AQ_FWLC_CROSS_CONNECT_NET_LOOPBACK,
	/** Unknown/unsupported loopback */
	AQ_FWLC_OTHER
};

enum aq_phy_fw_loopback_rate {
	/** Loopback rate chosen by currently active system/network interface */
	AQ_FWLRATE_AUTO,
	/** Loopback 10G data path */
	AQ_FWLRATE_10G,
	/** Loopback 1G data path */
	AQ_FWLRATE_1G,
	/** Loopback 100M data path */
	AQ_FWLRATE_100M,
	/** Unsupported rate */
	AQ_FWLRATE_OTHER
};

/** This stores some state information needed when programming flash */
struct aq_flash_control_state {
	/** Store the state of freeze_nvr_crc (0x1e.0x0100 */
	uint16_t nvr_crc_freeze;
	/** 0x1e.0x0100 */
	uint16_t global_nvr_interface_1;
	/** 0x1e.0xc450 */
	uint16_t global_nvr_provisioning_1;
	/** 0x1e.0xc451 */
	uint16_t global_nvr_provisioning_2;
	/** 0x1e.0xc001 */
	uint16_t global_control_2;
};

/** Parameters used internally for reading and writing to the FLASH */
struct aq_flash_parameters {
	/** FLASH opcode to erase the chip */
	uint16_t chip_erase_opcode;
	/** FLASH opcode to enable writing */
	uint16_t write_enable_opcode;
	/** FLASH opcode to get the write status */
	uint16_t write_status_opcode;
	/** FLASH opcode to disable write protection */
	uint16_t disable_prot_data;
	/** FLASH opcode to read the status */
	uint16_t read_status_opcode;
	/** FLASH status mask bit we're interested in */
	uint16_t read_status_register_mask;
	/** FLASH status bit polarity we want */
	uint16_t read_status_register_polarity;
	/** FLASH opcode to read */
	uint16_t read_opcode;
	/** FLASH opcode to program */
	uint16_t program_opcode;
	/** FLASH page size */
	uint16_t page_size;
	/** Maximum time in seconds for an operation to complete */
	uint16_t max_operation_time;
	/** Write enable interface register value */
	uint16_t write_enable_if;
	/** Write enable provisioning register value */
	uint16_t write_enable_prov;
	uint16_t chip_erase_if;
	uint16_t chip_erase_prov;
	uint16_t disable_prot_if;
	uint16_t disable_prot_prov;
	uint16_t read_status_register_if;
	uint16_t read_status_register_prov;
	uint16_t nonburst_read4_if;
	uint16_t nonburst_read4_prov;
	uint16_t burst_read4_if;
	uint16_t burst_read4_prov;
	uint16_t burst_write4_if;
	uint16_t burst_write4_prov;
	uint16_t read1_if;
	uint16_t read1_prov;
	uint16_t end_burst_if;
	uint16_t exec_op_if;
	uint16_t set256_if;
	uint16_t set256_prov;
	/** Size of FLASH device in bytes */
	uint32_t flash_size;
};

/**
 * Reads the configuration from the specified PHY device
 *
 * @param phydev	Phy device to get configuration from
 * @param[out] config	configuration of the phy
 *
 * @return 0 for success, -1 on error.
 */
int aq_phy_get_config(struct phy_device *phydev, struct aq_phy_config *config);

/**
 * Sets the phy configuration
 *
 * @param phydev	Phy device to configure
 * @param config	Configuration to use
 *
 * @return 0 for success, -1 on error.
 */
int aq_phy_set_config(struct phy_device *phydev,
		      const struct aq_phy_config *config);

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
			enum aq_phy_1g_network_op_mode *net_op_mode_1g);

/**
 * Gets the mode used for 2.5Gbps
 *
 * @param	phydev	Phy device to get mode for
 * @param[out]	mode	Mode used for 2.5Gbps
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_get_2_5g_mode(struct phy_device *phydev,
			 enum aq_phy_2_5g_op_mode *mode);

/**
 * Gets the static configuration of the phy
 *
 * @param	phydev	PHY device
 * @param[out]	config	static configuration
 *
 * @returns 0 for success, -1 for error
 */
int aq_phy_get_static_config(struct phy_device *phydev,
			     struct aq_phy_static_config *config);

/**
 * Gets the current phy status
 *
 * @param	phydev	phy device to use
 * @param	status	status of current device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_status(struct phy_device *phydev, struct aq_phy_status *status);

/**
 * Perform a soft reset on the phy device
 *
 * @param	phydev	phy device to reset
 *
 * @return	0 for success
 */
int aq_phy_soft_reset(struct phy_device *phydev);

/**
 * Get the current phy connection status
 *
 * @param	phydev		phy device
 * @param[out]	conn_status	connection status
 *
 * @return 0 for success
 */
int aq_phy_get_connection_status(struct phy_device *phydev,
				 struct aq_phy_conn_status *conn_status);


/**
 * Restart the PHY's autonegotiation sequence
 *
 * @param phydev	PHY device
 *
 * @return	0 for success, error otherwise.
 */
int aq_phy_restart_autonegotiation(struct phy_device *phydev);


/**
 * Changes the PHYs status from inactive to active.
 *
 * @param phydev	PHY device
 *
 * @return		0 for success, error otherwise
 */
int aq_phy_set_active(struct phy_device *phydev);

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
				   enum aq_phy_fw_loopback_rate *rate);

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
				   enum aq_phy_fw_loopback_rate rate);

/**
 * Get the current loopback setting
 *
 * @param	phydev		PHY device to get loopback mode
 * @param[out]	loopback	Current loopback mode
 *
 * @return	0 for success
 */
int aq_phy_get_loopback_control(struct phy_device *phydev,
				enum aq_phy_loopback_control *loopback);

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
				enum aq_phy_loopback_control loopback);

/**
 * Read any alarm status information if any alarms are set
 *
 * @param	phydev		phy device to get status from
 * @param	status		current alarm status
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_alarm_status(struct phy_device *phydev,
			    struct aq_phy_alarm_status *status);

/**
 * Find out which alarms are enabled
 *
 * @param	phydev	PHY device to get alarms from
 * @param[out]	control	Alarm data
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_get_alarm_control(struct phy_device *phydev,
			     struct aq_phy_alarm_control *control);

/**
 * Enables various alarms to generate interrupts
 *
 * @param	phydev		phy device to configure
 * @param	control		which alarms to enable
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_set_alarm_control(struct phy_device *phydev,
			     const struct aq_phy_alarm_control *control);

/**
 * Disable all alarm interrupts
 *
 * @param	phydev		phy device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_mask_all_alarms (struct phy_device *phydev);

/**
 * Enables alarm interrupts
 *
 * @param	phydev		phy device
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_unmask_all_alarms (struct phy_device *phydev);

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
				 unsigned int *data);

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
				  unsigned int data);

/**
 * This function is called before obtaining the EYE data
 *
 * @param	phydev		phy device
 * @param	lane		SERDES lane number
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_begin_serdes_rx_eye_diagram(struct phy_device *phydev, uint8_t lane);

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
					 uint16_t *max_errors);

/**
 * This function is called after obtaining the EYE data
 *
 * @param	phydev		phy device
 * @param	lane		SERDES lane number
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_end_serdes_rx_eye_diagram(struct phy_device *phydev, uint8_t lane);

/**
 * Calculates a CRC16
 *
 * @param crc		starting CRC value
 * @param[in] image	data to calculate CRC over
 * @param size		Size of data in bytes
 *
 * @return		New CRC
 */
uint16_t aq_phy_calc_crc16(uint16_t crc, const uint8_t image[], uint32_t size);

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
int aq_phy_flash_interface_ready(struct phy_device *phydev);

/**
 * Return if the flash is ready
 *
 * @param phydev		PHY device to check
 * @param[in] fparms		FLASH parameters
 *
 * @return	0 if flash is not ready or timed out, 1 if ready.
 */
int aq_phy_flash_ready(struct phy_device *phydev,
		       const struct aq_flash_parameters *fparms);

/**
 * Take control of the FLASH device for programming/verification/etc.
 *
 * @param	phydev	PHY device to take control of
 * @param[out]	state	State that needs to be preserved to return control to
 *			the PHY.
 */
void aq_phy_take_control_of_flash(struct phy_device *phydev,
				  struct aq_flash_control_state *state);

/**
 * Returns control of the FLASH to the PHY device
 *
 * @param	phydev	PHY device to return control to
 * @param[in]	state	State saved from aq_phy_take_control_of_flash()
 */
void aq_phy_return_control_of_flash(struct phy_device *phydev,
				    const struct aq_flash_control_state *state);

/**
 * Automatically detects the type of FLASH device connected to the PHY
 *
 * @param	phydev		PHY device to detect FLASH for
 * @param[out]	flash_type	Type of FLASH device connected to PHY
 *
 * @return	0 for success, -1 on error
 */
int aq_phy_determine_flash_type(struct phy_device *phydev,
				enum aq_phy_flash_type *flash_type);

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
						       uint32_t *image_size_ptr);

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
					uint32_t *image_size_ptr);

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
					     uint32_t *image_size_ptr);

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
					   uint32_t *image_size_ptr);

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
			    uint8_t *image, uint32_t *image_size_ptr);

/**
 * Gets information about the PHY FLASH image
 *
 * @param	phydev		PHY device
 * @param[out]	flash_info	FLASH image information
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_get_flash_information(struct phy_device *phydev,
				 struct aq_phy_flash_info *flash_info);


/**
 * Prints diagnostics registers for Aquantia
 *
 * @param phydev	Phy device to print
 */
void aq_phy_print_debug_data(struct phy_device *phydev);

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
			uint32_t length, uint32_t start_address);

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
		       uint32_t length, uint32_t start_address);

/**
 * Print a debug trace output for Aquantia
 *
 * @param	phydev	Phy device to print trace from
 *
 * @return 0 for success, -1 on error (can't freeze/unfreeze debug trace)
 */
int aq_phy_print_debug_trace(struct phy_device *phydev);

/**
 * Print Ethernet cable signal information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_print_signal(struct phy_device *phydev);

/**
 * Starts basic cable diagnostics
 *
 * @param phydev	PHY device to test
 *
 * @return 0 for success, -1 on error (UP timeout)
 */
int aq_phy_run_basic_cable_diags(struct phy_device *phydev);

/**
 * Get the results from basic cable diagnostics
 *
 * @param phydev	PHY device
 * @param[out] results	Diagnostic results
 *
 * @return -1 on error, 0 on success, 1 if the diagnostics are still in progress
 */
int aq_phy_get_basic_cable_diags_results(struct phy_device *phydev,
					 struct aq_phy_cable_basic_diag_results *results);

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
			   bool *fail_low, bool *warn_low);

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
				      int16_t low_warn_threshold);


/**
 * Performs basic cable diagnostics and prints the results
 *
 * @param phydev	PHY device to test cable on
 *
 * @return 0 for success, -1 on error
 */
int aq_phy_print_basic_cable_diags(struct phy_device *phydev);

/**
 * Print out temperature information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 if not ready
 */
int aq_phy_print_temperature(struct phy_device *phydev);

/**
 * Print out PHY FLASH information
 *
 * @param phydev	PHY device to print
 *
 * @return 0 for success, -1 if error
 */
int aq_phy_print_flash_information(struct phy_device *phydev);

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
int aq_phy_get_link_speed(struct phy_device *phydev, unsigned int timeout_ms);

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
		      int *speed);

#endif /* __OCTEON_PHY_AQUANTIA_H__ */
