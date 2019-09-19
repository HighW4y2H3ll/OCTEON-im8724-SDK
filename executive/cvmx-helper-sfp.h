/***********************license start***************
 * Copyright (c) 2016  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Helper functions to abstract SFP and QSFP connectors
 *
 * <hr>$Revision: 146313 $<hr>
 */
#ifndef __CVMX_HELPER_SFP_H__
#define __CVMX_HELPER_SFP_H__

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Maximum size for the SFP EEPROM.  Currently only 96 bytes are used.
 */
#define CVMX_SFP_MAX_EEPROM_SIZE	0x100

/**
 * Default address of sfp EEPROM
 */
#define CVMX_SFP_DEFAULT_I2C_ADDR	0x50

/**
 * Default address of SFP diagnostics chip
 */
#define CVMX_SFP_DEFAULT_DIAG_I2C_ADDR	0x51

struct cvmx_fdt_sfp_info;
struct cvmx_fdt_gpio_info;
/**
 * Connector type for module, usually we only see SFP and QSFPP
 */
enum cvmx_phy_sfp_conn_type {
	CVMX_SFP_CONN_SFP = 0x03,		/** SFP/SFP+/SFP28 */
	CVMX_SFP_CONN_QSFP = 0x0C,		/** 1G QSFP (obsolete) */
	CVMX_SFP_CONN_QSFPP = 0x0D,		/** QSFP+ or later */
	CVMX_SFP_CONN_QSFP28 = 0x11,		/** QSFP28 (100Gbps) */
	CVMX_SFP_CONN_MICRO_QSFP = 0x17,	/** Micro QSFP */
};

/**
 * module type plugged into a SFP/SFP+/QSFP+ port
 */
enum cvmx_phy_sfp_mod_type {
	CVMX_SFP_MOD_UNKNOWN = 0,		/** Unknown or unspecified */
	/** Fiber optic module (LC connector) */
	CVMX_SFP_MOD_OPTICAL_LC = 0x7,
	/** Fiber optic module (pigtail, no connector) */
	CVMX_SFP_MOD_OPTICAL_PIGTAIL = 0xB,
	CVMX_SFP_MOD_COPPER_PIGTAIL = 0x21,	/** copper module */
	CVMX_SFP_MOD_RJ45 = 0x22,		/** RJ45 (i.e. 10GBase-T) */
	/** No separable connector (SFP28/copper) */
	CVMX_SFP_MOD_NO_SEP_CONN = 0x23,
	/** Unknown/other module type */
	CVMX_SFP_MOD_OTHER
};

/** Peak rate supported by SFP cable */
enum cvmx_phy_sfp_rate {
	CVMX_SFP_RATE_UNKNOWN,		/** Unknown rate */
	CVMX_SFP_RATE_1G,		/** 1Gbps */
	CVMX_SFP_RATE_10G,		/** 10Gbps */
	CVMX_SFP_RATE_25G,		/** 25Gbps */
	CVMX_SFP_RATE_40G,		/** 40Gbps */
	CVMX_SFP_RATE_100G		/** 100Gbps */
};

/**
 * Cable compliance specification
 * See table 4-4 from SFF-8024 for the extended specification compliance codes
 */
enum cvmx_phy_sfp_cable_ext_compliance {
	CVMX_SFP_CABLE_UNSPEC = 0,
	CVMX_SFP_CABLE_100G_AOC_HIGH_BER = 0x01,	/** Active optical cable */
	CVMX_SFP_CABLE_100G_SR4 = 0x2,
	CVMX_SFP_CABLE_100G_LR4 = 0x3,
	CVMX_SFP_CABLE_100G_ER4 = 0x4,
	CVMX_SFP_CABLE_100G_SR10 = 0x5,
	CVMX_SFP_CABLE_100G_CWDM4_MSA = 0x6,
	CVMX_SFP_CABLE_100G_PSM4 = 0x7,
	CVMX_SFP_CABLE_100G_ACC_HIGH_BER = 0x8,
	CVMX_SFP_CABLE_100G_CWDM4 = 0x9,
	CVMX_SFP_CABLE_100G_CR4 = 0xA,
	CVMX_SFP_CABLE_25G_CR_CA_S = 0xC,
	CVMX_SFP_CABLE_25G_CR_CA_N = 0xD,
	CVMX_SFP_CABLE_40G_ER4 = 0x10,
	CVMX_SFP_CABLE_4X10G_SR = 0x11,
	CVMX_SFP_CABLE_40G_PSM4= 0x12,
	CVMX_SFP_CABLE_G959_1_P1I1_2D1 = 0x13,
	CVMX_SFP_CABLE_G959_1_P1S1_2D2 = 0x14,
	CVMX_SFP_CABLE_G959_1_P1L1_2D2 = 0x15,
	CVMX_SFP_CABLE_10GBASE_T = 0x16,
	CVMX_SFP_CABLE_100G_CLR4 = 0x17,
	CVMX_SFP_CABLE_100G_AOC_LOW_BER = 0x18,
	CVMX_SFP_CABLE_100G_ACC_LOW_BER = 0x19,
	CVMX_SFP_CABLE_100G_2_LAMBDA_DWDM = 0x1a
};

/** Optical modes module is compliant with */
enum cvmx_phy_sfp_10g_eth_compliance {
	CVMX_SFP_CABLE_10GBASE_ER = 0x80,	/** 10G ER */
	CVMX_SFP_CABLE_10GBASE_LRM = 0x40,	/** 10G LRM */
	CVMX_SFP_CABLE_10GBASE_LR = 0x20,	/** 10G LR */
	CVMX_SFP_CABLE_10GBASE_SR = 0x10	/** 10G SR */
};

/** Diagnostic ASIC compatiblity */
enum cvmx_phy_sfp_sff_8472_diag_rev {
	CVMX_SFP_SFF_8472_NO_DIAG = 0x00,
	CVMX_SFP_SFF_8472_REV_9_3 = 0x01,
	CVMX_SFP_SFF_8472_REV_9_5 = 0x02,
	CVMX_SFP_SFF_8472_REV_10_2 = 0x03,
	CVMX_SFP_SFF_8472_REV_10_4 = 0x04,
	CVMX_SFP_SFF_8472_REV_11_0 = 0x05,
	CVMX_CVMX_SFP_SFF_8472_REV_11_3 = 0x06,
	CVMX_SFP_SFF_8472_REV_11_4 = 0x07,
	CVMX_SFP_SFF_8472_REV_12_0 = 0x08,
	CVMX_SFP_SFF_8472_REV_UNALLOCATED = 0xff
};

/**
 * Data structure describing the current SFP or QSFP EEPROM
 */
struct cvmx_sfp_mod_info {
	enum cvmx_phy_sfp_conn_type conn_type;	/** Connector type */
	enum cvmx_phy_sfp_mod_type mod_type;	/** Module type */
	enum cvmx_phy_sfp_rate rate;		/** Rate of module */
	/** 10G Ethernet Compliance codes (logical OR) */
	enum cvmx_phy_sfp_10g_eth_compliance eth_comp;
	/** Extended Cable compliance */
	enum cvmx_phy_sfp_cable_ext_compliance cable_comp;
	uint8_t vendor_name[17];		/** Mopdule vendor name */
	uint8_t vendor_oui[3];			/** vendor OUI */
	uint8_t vendor_pn[17];			/** Vendor part number */
	uint8_t vendor_rev[5];			/** Vendor revision */
	uint8_t vendor_sn[17];			/** Vendor serial number */
	uint8_t date_code[9];			/** Date code */
	bool valid;				/** True if module is valid */
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
	enum cvmx_phy_sfp_sff_8472_diag_rev diag_rev;
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
int cvmx_phy_sfp_read_i2c_eeprom(uint8_t *buffer, int i2c_bus, int i2c_addr);

/**
 * Reads the SFP/SFP+/QSFP EEPROM and outputs the type of module or cable
 * plugged in
 *
 * @param[out]	sfp_info	Info about SFP module
 * @param[in]	buffer		SFP EEPROM buffer to parse
 *
 * @return	0 on success, -1 if error reading EEPROM or if EEPROM corrupt
 */
int cvmx_phy_sfp_parse_eeprom(struct cvmx_sfp_mod_info *sfp_info,
			      const uint8_t *buffer);

/**
 * Prints out information about a SFP/QSFP device
 *
 * @param[in]	sfp_info	data structure to print
 */
void cvmx_phy_sfp_print_info(const struct cvmx_sfp_mod_info *sfp_info);

/**
 * Reads and parses SFP/QSFP EEPROM
 *
 * @param	sfp	sfp handle to read
 *
 * @return	0 for success, -1 on error.
 */
int cvmx_sfp_read_i2c_eeprom(struct cvmx_fdt_sfp_info *sfp);

/**
 * Function called to check and return the status of the mod_abs pin or
 * mod_pres pin for QSFPs.
 *
 * @param	sfp	Handle to SFP information.
 * @param	data	User-defined data passed to the function
 *
 * @return	0 if absent, 1 if present, -1 on error
 */
int cvmx_sfp_check_mod_abs(struct cvmx_fdt_sfp_info *sfp, void *data);

/**
 * Registers a function to be called to check mod_abs/mod_pres for a SFP/QSFP
 * slot.
 *
 * @param	sfp		Handle to SFP data structure
 * @param	check_mod_abs	Function to be called or NULL to remove
 * @param	mod_abs_data	User-defined data to be passed to check_mod_abs
 *
 * @return	0 for success
 */
int cvmx_sfp_register_check_mod_abs(struct cvmx_fdt_sfp_info *sfp,
	int (*check_mod_abs)(struct cvmx_fdt_sfp_info *sfp, void *data),
	void *mod_abs_data);

/**
 * Registers a function to be called whenever the mod_abs/mod_pres signal
 * changes.
 *
 * @param	sfp		Handle to SFP data structure
 * @param	mod_abs_changed	Function called whenever mod_abs is changed
 *				or NULL to remove.
 * @param	mod_abs_changed_data	User-defined data passed to
 *					mod_abs_changed
 *
 * @return	0 for success
 */
int cvmx_sfp_register_mod_abs_changed(struct cvmx_fdt_sfp_info *sfp,
	int (*mod_abs_changed)(struct cvmx_fdt_sfp_info *sfp, int val,
			       void *data),
	void *mod_abs_changed_data);

/**
 * Registers a function to be called whenever rx_los changes
 *
 * @param	sfp		Handle to SFP data structure
 * @param	rx_los_changed	Function to be called when rx_los changes
 *				or NULL to remove the function
 * @param	rx_los_changed_data	User-defined data passed to
 *					rx_los_changed
 *
 * @return	0 for success
 */
int cvmx_sfp_register_rx_los_changed(struct cvmx_fdt_sfp_info *sfp,
	int (*rx_los_changed)(struct cvmx_fdt_sfp_info *sfp, int val,
			      void *data),
	void *rx_los_changed_data);

/**
 * Parses the device tree for SFP and QSFP slots
 *
 * @param	fdt_addr	Address of flat device-tree
 *
 * @return	0 for success, -1 on error
 */
int cvmx_sfp_parse_device_tree(void *fdt_addr);

/**
 * Given an IPD port number find the corresponding SFP or QSFP slot
 *
 * @param	ipd_port	IPD port number to search for
 *
 * @return	pointer to SFP data structure or NULL if not found
 */
struct cvmx_fdt_sfp_info *cvmx_sfp_find_slot_by_port(int ipd_port);

/**
 * Given a fdt node offset find the corresponding SFP or QSFP slot
 *
 * @param	of_offset	flat device tree node offset
 *
 * @return	pointer to SFP data structure or NULL if not found
 */
struct cvmx_fdt_sfp_info *cvmx_sfp_find_slot_by_fdt_node(int of_offset);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
#endif

#endif /* __CVMX_HELPER_SFP_H__ */
