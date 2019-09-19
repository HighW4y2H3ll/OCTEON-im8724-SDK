/*
 * (C) Copyright 2015-2016 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SFP_H__
#define __SFP_H__

/**
 * This file contains support for parsing a SFP/SFP+/QSFP+ eeprom for Ethernet
 * SFP adapters.
 */

/**
 * Maximum size for the SFP EEPROM.  Currently only 96 bytes are used.
 */
#define SFP_MAX_EEPROM_SIZE		0x100

/**
 * Default address of sfp EEPROM
 */
#define SFP_DEFAULT_I2C_ADDR		0x50

/**
 * Default address of SFP diagnostics chip
 */
#define SFP_DEFAULT_DIAG_I2C_ADDR	0x51

/**
 * Connector type for module, usually we only see SFP and QSFPP
 */
enum phy_sfp_conn_type {
	SFP_CONN_SFP = 0x03,		/** SFP/SFP+/SFP28 */
	SFP_CONN_QSFP = 0x0C,		/** 1G QSFP (obsolete) */
	SFP_CONN_QSFPP = 0x0D,		/** QSFP+ or later */
	SFP_CONN_QSFP28 = 0x11,		/** QSFP28 (100Gbps) */
	SFP_CONN_MICRO_QSFP = 0x17,	/** Micro QSFP */
};

/**
 * module type plugged into a SFP/SFP+/QSFP+ port
 */
enum phy_sfp_mod_type {
	SFP_MOD_UNKNOWN = 0,	/** Unknown or unspecified */
	/** Fiber optic module (LC connector) */
	SFP_MOD_OPTICAL_LC = 0x7,
	/** Fiber optic module (pigtail, no connector) */
	SFP_MOD_OPTICAL_PIGTAIL = 0xB,
	SFP_MOD_COPPER_PIGTAIL = 0x21,	/** copper module */
	SFP_MOD_RJ45 = 0x22,	/** RJ45 (i.e. 10GBase-T) */
	SFP_MOD_NO_SEP_CONN = 0x23,/** No separable connector (SFP28/copper) */
	SFP_MOD_OTHER		/** Unknown/other module type */
};

/** Peak rate supported by SFP cable */
enum phy_sfp_rate {
	SFP_RATE_UNKNOWN,	/** Unknown rate */
	SFP_RATE_1G,		/** 1Gbps */
	SFP_RATE_10G,		/** 10Gbps */
	SFP_RATE_25G,		/** 25Gbps */
	SFP_RATE_40G,		/** 40Gbps */
	SFP_RATE_100G		/** 100Gbps */
};

/**
 * Cable compliance specification
 * See table 4-4 from SFF-8024 for the extended specification compliance codes
 */
enum phy_sfp_cable_ext_compliance {
	SFP_CABLE_UNSPEC = 0,
	SFP_CABLE_100G_AOC_HIGH_BER = 0x01,	/** Active optical cable */
	SFP_CABLE_100G_SR4 = 0x2,
	SFP_CABLE_100G_LR4 = 0x3,
	SFP_CABLE_100G_ER4 = 0x4,
	SFP_CABLE_100G_SR10 = 0x5,
	SFP_CABLE_100G_CWDM4_MSA = 0x6,
	SFP_CABLE_100G_PSM4 = 0x7,
	SFP_CABLE_100G_ACC_HIGH_BER = 0x8,
	SFP_CABLE_100G_CWDM4 = 0x9,
	SFP_CABLE_100G_CR4 = 0xA,
	SFP_CABLE_25G_CR_CA_S = 0xC,
	SFP_CABLE_25G_CR_CA_N = 0xD,
	SFP_CABLE_40G_ER4 = 0x10,
	SFP_CABLE_4X10G_SR = 0x11,
	SFP_CABLE_40G_PSM4= 0x12,
	SFP_CABLE_G959_1_P1I1_2D1 = 0x13,
	SFP_CABLE_G959_1_P1S1_2D2 = 0x14,
	SFP_CABLE_G959_1_P1L1_2D2 = 0x15,
	SFP_CABLE_10GBASE_T = 0x16,
	SFP_CABLE_100G_CLR4 = 0x17,
	SFP_CABLE_100G_AOC_LOW_BER = 0x18,
	SFP_CABLE_100G_ACC_LOW_BER = 0x19,
	SFP_CABLE_100G_2_LAMBDA_DWDM = 0x1a
};

/** Optical modes module is compliant with */
enum phy_sfp_10g_eth_compliance {
	SFP_CABLE_10GBASE_ER = 0x80,	/** 10G ER */
	SFP_CABLE_10GBASE_LRM = 0x40,	/** 10G LRM */
	SFP_CABLE_10GBASE_LR = 0x20,	/** 10G LR */
	SFP_CABLE_10GBASE_SR = 0x10	/** 10G SR */
};

/** Diagnostic ASIC compatiblity */
enum phy_sfp_sff_8472_diag_rev {
	SFP_SFF_8472_NO_DIAG = 0x00,
	SFP_SFF_8472_REV_9_3 = 0x01,
	SFP_SFF_8472_REV_9_5 = 0x02,
	SFP_SFF_8472_REV_10_2 = 0x03,
	SFP_SFF_8472_REV_10_4 = 0x04,
	SFP_SFF_8472_REV_11_0 = 0x05,
	SFP_SFF_8472_REV_11_3 = 0x06,
	SFP_SFF_8472_REV_11_4 = 0x07,
	SFP_SFF_8472_REV_12_0 = 0x08,
	SFP_SFF_8472_REV_UNALLOCATED = 0xff
};

/**
 * Data structure describing the current SFP or QSFP EEPROM
 */
struct phy_sfp_info {
	enum phy_sfp_conn_type conn_type;	/** Connector type */
	enum phy_sfp_mod_type mod_type;		/** Module type */
	enum phy_sfp_rate rate;			/** Rate of module */
	/** 10G Ethernet Compliance codes (logical OR) */
	enum phy_sfp_10g_eth_compliance eth_comp;
	/** Extended Cable compliance */
	enum phy_sfp_cable_ext_compliance cable_comp;
	uint8_t vendor_name[17];		/** Mopdule vendor name */
	uint8_t vendor_oui[3];			/** vendor OUI */
	uint8_t vendor_pn[17];			/** Vendor part number */
	uint8_t vendor_rev[5];			/** Vendor revision */
	uint8_t vendor_sn[17];			/** Vendor serial number */
	uint8_t date_code[9];			/** Date code */
	bool active_cable;			/** False for passive copper */
	bool copper_cable;			/** True if cable is copper */
	/** True if module is limiting (i.e. not passive copper) */
	bool limiting;
	/** Maximum length of copper cable in meters */
	int max_copper_cable_len;
	/** Max single mode cable length in meters */
	int max_single_mode_cable_length;
	/** Max 50um OM2 cable length */
	int max_50um_om2_cable_length;
	/** Max 62.5um OM1 cable length */
	int max_62_5um_om1_cable_length;
	/** Max 50um OM4 cable length */
	int max_50um_om4_cable_length;
	/** Max 50um OM3 cable length */
	int max_50um_om3_cable_length;
	/** Minimum bitrate in Mbps */
	int bitrate_min;
	/** Maximum bitrate in Mbps */
	int bitrate_max;
	/**
	 * Set to true if forward error correction is required,
	 * for example, a 25GBase-CR CA-S cable.
	 *
	 * FEC should only be disabled at 25G with CA-N cables.  FEC is required
	 * with 5M and longer cables.
	 */
	bool fec_required;
	/** True if RX output is linear */
	bool linear_rx_output;
	/** Power level, can be 1, 2 or 3 */
	int power_level;
	/** False if conventional cooling is used, true for active cooling */
	bool cooled_laser;
	/** True if internal retimer or clock and data recovery circuit */
	bool internal_cdr;
	/** True if LoS is implemented */
	bool los_implemented;
	/** True if LoS is inverted from the standard */
	bool los_inverted;
	/** True if TX_FAULT is implemented */
	bool tx_fault_implemented;
	/** True if TX_DISABLE is implemented */
	bool tx_disable_implemented;
	/** True if RATE_SELECT is implemented */
	bool rate_select_implemented;
	/** True if tuneable transmitter technology is used */
	bool tuneable_transmitter;
	/** True if receiver decision threshold is implemented */
	bool rx_decision_threshold_implemented;
	/** True if diagnostic monitoring present */
	bool diag_monitoring;
	/** True if diagnostic address 0x7f is used for selecting the page */
	bool diag_paging;
	/** Diagnostic feature revision */
	enum phy_sfp_sff_8472_diag_rev diag_rev;
	/** True if an address change sequence is required for diagnostics */
	bool diag_addr_change_required;
	/** True if RX power is averaged, false if OMA */
	bool diag_rx_power_averaged;
	/** True if diagnostics are externally calibrated */
	bool diag_externally_calibrated;
	/** True if diagnostics are internally calibrated */
	bool diag_internally_calibrated;
	/** True of soft rate select control implemented per SFF-8431 */
	bool diag_soft_rate_select_control;
	/** True if application select control implemented per SFF-8079 */
	bool diag_app_select_control;
	/** True if soft RATE_SELECT control and moonitoring implemented */
	bool diag_soft_rate_select_implemented;
	/** True if soft RX_LOS monitoring implemented */
	bool diag_soft_rx_los_implemented;
	/** True if soft TX_FAULT monitoring implemented */
	bool diag_soft_tx_fault_implemented;
	/** True if soft TX_DISABLE control and monitoring implemented */
	bool diag_soft_tx_disable_implemented;
	/** True if alarm/warning flags implemented */
	bool diag_alarm_warning_flags_implemented;
};

/**
 * Reads the SFP EEPROM using the i2c bus
 *
 * @param[out]	buffer		Buffer to store SFP EEPROM data in
 *				The buffer should be SFP_MAX_EEPROM_SIZE bytes.
 * @param	i2c_bus		i2c bus number to read from for SFP port
 * @param	i2c_addr	i2c address to use, 0 for default
 *
 * @return	-1 if invalid bus or i2c read error, 0 for success
 */
int phy_sfp_read_i2c_eeprom(uint8_t *buffer, int i2c_bus, int i2c_addr);

/**
 * Reads the SFP/SFP+/QSFP EEPROM and outputs the type of module or cable
 * plugged in
 *
 * @param[out]	sfp_info	Info about SFP module
 * @param[in]	buffer		SFP EEPROM buffer to parse
 *
 * @return	0 on success, -1 if error reading EEPROM or if EEPROM corrupt
 */
int phy_sfp_parse_eeprom(struct phy_sfp_info *sfp_info, const uint8_t *buffer);

/**
 * Prints out information about a SFP/QSFP device
 *
 * @param[in]	sfp_info	data structure to print
 */
void phy_sfp_print_info(const struct phy_sfp_info *sfp_info);

#endif /* __SFP_H__ */
