/* Copyright 2014-2015 Avago Technologies. All rights reserved.
 *
 * This file is part of the AAPL CORE library.
 *
 * AAPL CORE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AAPL CORE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AAPL CORE.  If not, see http://www.gnu.org/licenses.
 */
/* AAPL Revision: 2.3.1-beta                                        */
/* AAPL (ASIC and ASSP Programming Layer) support for AVSP */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for ASSP functions. */

#ifndef AVSP_H_
#define AVSP_H_

#define AVSP_MAX_SLICE_COUNT 20

/** @brief The AVSP operating mode. */
enum avsp_mode {
	AVSP_PMA = 0,		      /**< Select the AVSP-1104 10:4  operating mode. */
	AVSP_GEARBOX_10_4 = 0,	      /**< Select the AVSP-1104 10:4  operating mode. */
	AVSP_RPT = 1,		      /**< Select the AVSP-1104 10:10 operating mode. */
	AVSP_REPEATER_DUPLEX = 1,     /**< Select the AVSP-1104 10:10 and AVSP-4412 operating mode. */
	AVSP_GEARBOX_10_4_RS_FEC = 2,
	AVSP_GEARBOX_10_4_MLG = 3,
	AVSP_REPEATER_SIMPLEX = 4,    /**< Select the AVSP-8801 mode. */
	AVSP_GEARBOX_4_1 = 5,
	AVSP_ADHOC = 6,
	AVSP_GEARBOX_2_1 = 7,	      /**<Select AVSP-8812 Gearbox 2:1 mode */
	AVSP_GEARBOX_2_1_MOD_HOST = 8,/**<Select AVSP-8812 Gearbox 1:2 mode */
	AVSP_RS_FEC_4x4 = 9,	      /**<Select AVSP-8812 FEC mode */
	AVSP_RS_FEC_528 = 10,	      /**<Select AVSP-7412 FEC mode */
	AVSP_RS_FEC_544 = 11,	      /**<Select AVSP-7412 FEC mode */
	AVSP_RS_FEC = 12	      /**<Select AVSP-5410 FEC mode */
};

/** @brief  Options for auto-negotiation (AN). */
struct avsp_an_config {
	uint32_t host_mask;	/**< Mask that defines which slices on the Host side will AN */
	uint32_t mod_mask;	/**< Mask that defines which slices on the Mod side will AN */
	uint32_t an_speed;	/**< Defines what speed AN will occur at: */
	/**< - 0x00 = 3.125GB/s */
	/**< - 0x01 = 1.56 GB/s */
	/**< - 0x02 = 1.25 GB/s */
	uint32_t an_side;	/**< Boolean that determines whether AN will start on host (false) or mod (true) side */
	uint32_t rx_clk;	/**< Use the ref_clk, instead of the RX recovered clock */
	uint32_t n_ports;	/**< Number of ports plugged into ASSP */
	uint32_t cap_in;	/**< Whether AN will use: */
	/**< - 0 : Default capability list */
	/**< - 1 : User input capability list */
	/**< - 3 : MDIO capability list */
	uint32_t user_cap;	      /**< The user's capability list. (Use 0x000 if default or MDIO) */
	uint32_t kr_training_disable; /**<  Whether or not KR training should be disabled after AN  */
	uint32_t enable_symmetric;    /**<  Whether the KR training should be symmetric   */
};

struct avsp_an_config *avsp_an_config_construct(struct avago_hdl *ah);
void avsp_an_config_destruct(struct avago_hdl *ah,
			     struct avsp_an_config *config);
uint32_t avsp_an_start(struct avago_hdl *ah, uint32_t chip,
		       struct avsp_an_config *config);

int avsp_supervisor_enable(struct avago_hdl *ah, uint32_t chip, bool enable);
int avsp_supervisor_status(struct avago_hdl *ah, uint32_t chip, bool * enabled);
int avsp_supervisor_clear_interr_io(struct avago_hdl *ah, uint32_t chip);
int avsp_supervisor_get_interr_io(struct avago_hdl *ah, uint32_t chip,
				  bool * active);
int avsp_supervisor_set_slices(struct avago_hdl *ah, uint32_t chip, int count,
			       uint32_t * selections);

/** @brief */
enum avsp_supervisor_mode {
	/**<
	 * Start tuning when signal is detected without waiting for frequency
	 * lock.
	 */
	AVSP_SUPERVISOR_MODE_TUNE_IF_SIGNAL,
	/**<
	 * Start tuning only after signal is detected and frequency is locked.
	 */
	AVSP_SUPERVISOR_MODE_TUNE_IF_LOCKED_SIGNAL,
	AVSP_SUPERVISOR_MODE_NO_TUNE		    /**< */
};

/** @brief Structure for retrieving supervisor status, and for controlling */
/**        supervisor operation. */
struct avsp_supervisor_config {
	short assp_control_0;		    /**< Control flags.  Change only if you know what you are doing. */
	short assp_control_1;
	short assp_control_2;
	short assp_control_3;
	int serdes_count;		    /**< Informational */
	bool enabled;			    /**< Supervisor run status. */
	bool interr_io;			    /**< interr_io pin status. */
	int status[AVSP_MAX_SLICE_COUNT];   /**< Per slice status bits. */
};

bool avsp_supervisor_config_init(struct avago_hdl *ah,
				 enum avsp_supervisor_mode mode,
				 bool squelch_tx, bool reuse_ical,
				 bool adaptive,
				 struct avsp_supervisor_config *config);
struct avsp_supervisor_config *avsp_supervisor_config_construct(struct avago_hdl
								*ah, enum
								avsp_supervisor_mode
								mode,
								bool squelch_tx,
								bool reuse_ical,
								bool adaptive);
void avsp_supervisor_config_destruct(struct avago_hdl *ah,
				     struct avsp_supervisor_config *config);
int avsp_supervisor_get_config(struct avago_hdl *ah, uint32_t chip,
			       struct avsp_supervisor_config *config);
int avsp_supervisor_set_config(struct avago_hdl *ah, uint32_t chip,
			       struct avsp_supervisor_config *config);
int avsp_supervisor_set_signal_ok(struct avago_hdl *ah, uint32_t chip,
				  uint32_t select, int threshold);

/** @Deprecated */
enum avsp_setup {
	AVSP_CUST,
	AVSP_BERT,
	AVSP_SOLO
};
/** @Deprecated */
enum avsp_lb_mode {
	AVSP_LB_NEAR,
	AVSP_LB_FAR,
	AVSP_LB_OFF,
	AVSP_LB_STATE
};

/** @Deprecated */
enum avsp_lb_side {
	AVSP_LB_HOST,
	AVSP_LB_MOD
};

enum avsp_addr_list {
	AVSP_ALL,
	AVSP_HOST,
	AVSP_MOD,
	AVSP_HOST_MOD4,
	AVSP_MOD_HOST4,
	AVSP_MOD4,
	AVSP_MODX,
	AVSP_GB01_HOST_MOD,
	AVSP_GB23_HOST_MOD,
	AVSP_GB01_MOD_HOST,
	AVSP_GB23_MOD_HOST,
	AVSP_HALF0,
	AVSP_HALF1,
	AVSP_MOD6 = AVSP_MODX
};

bool avago_get_addr_list(struct avago_hdl *ah, uint32_t prtad,
			 const char *avsp_name, enum avsp_addr_list list,
			 uint32_t * len, uint32_t ** addrs);
bool avsp_get_name_list(struct avago_hdl *ah, uint32_t prtad,
			const char *avsp_name, uint32_t * len,
			const char ***names);

/* General AVSP functions: */
int avsp_upload_firmware(struct avago_hdl *ah, uint32_t prtad,
			 bool refclk_halve, int serdes_rom_size,
			 const int *serdes_rom, int sbm_rom_size,
			 const int *sbm_rom, int swap_rom_size,
			 const int *swap_rom);
int avsp_upload_firmware_file(struct avago_hdl *ah, uint32_t prtad,
			      bool refclk_halve, const char *serdes_filename,
			      const char *sbm_filename);
int avsp_get_refclk_divisor(struct avago_hdl *ah, uint32_t prtad);

int avsp_enable_signal_ok(struct avago_hdl *ah, uint32_t prtad,
			  enum avsp_addr_list list, int threshold);

bool avsp_control_logic_reset(struct avago_hdl *ah, uint32_t prtad);
bool avsp_control_logic_reset_host_to_mod(struct avago_hdl *ah, uint32_t prtad,
					  uint32_t slice_mask);
bool avsp_control_logic_reset_mod_to_host(struct avago_hdl *ah, uint32_t prtad,
					  uint32_t slice_mask);

bool avsp_control_logic_reset_direct(struct avago_hdl *ah, uint32_t prtad,
				     enum avsp_mode mode);
bool avsp_control_logic_reset_host_to_mod_direct(struct avago_hdl *ah,
						 uint32_t prtad,
						 enum avsp_mode mode,
						 uint32_t slice_mask);
bool avsp_control_logic_reset_mod_to_host_direct(struct avago_hdl *ah,
						 uint32_t prtad,
						 enum avsp_mode mode,
						 uint32_t slice_mask);

enum avsp_fec_register {
	AVSP_FEC_ERROR_INDICATION_DISABLE_ABILITY,   /**< Returns 1. */
	AVSP_FEC_ERROR_CORRECTION_BYPASS_ABILITY,    /**< Returns 1. */
	AVSP_FEC_ERROR_INDICATION_DISABLE,	     /**< Returns TRUE if disabled. */
	AVSP_FEC_ERROR_CORRECTION_BYPASS_ENABLE,     /**< Returns TRUE if enabled. */
	AVSP_FEC_BLOCK_LOCK,	    /**< Bit map [19..0] 8812 only */
	AVSP_FEC_LANE_ALIGNMENT,    /**< Bit map [3..0] */
	AVSP_FEC_LANE_MAPPING,	    /**< Returns mapping for all 4 lanes in one hex value. */
	/**< Each mapping is 4 bits, ordered 0x3210. */

	AVSP_PCS_BLOCK_LOCK,	    /**< Bit map [19..0] */
	AVSP_PCS_LANE_ALIGNMENT,    /**< Bit map [19..0] */
	AVSP_PCS_LANE_MAPPING,	    /**< Select lane using lane field: range [0-19]. */

	AVSP_FEC_CORRECTED,	    /**< Returns corrected FEC errors. */
	AVSP_FEC_UNCORRECTED,	    /**< Returns uncorrected FEC errors. */
	AVSP_FEC_SYMBOL_ERRORS,	    /**< Return error count for one lane: range [0-3]. */
	AVSP_BIP_ERRORS,	    /**< Return errors for lane [0..19] and mod(0)/host(1) selection */
	AVSP_ALL_ERRORS		    /**< Returns sum of all above errors. */
};

enum avsp_mode avsp_5410_get_mode(struct avago_hdl *ah, uint32_t prtad);
int avsp_5410_set_mode(struct avago_hdl *ah, uint32_t prtad,
		       enum avsp_mode mode);
int avsp_5410_bring_up(struct avago_hdl *ah, uint32_t prtad,
		       const char *slices);
bool avsp_5410_control_logic_reset_direct(struct avago_hdl *ah, uint32_t prtad,
					  const char *direction);

struct avsp_state_options {
	enum avsp_mode mode;
	int divider;
	/** 8812 specific variables */
	enum avsp_mode half_mode[2];
	int half0_div;
	int half1_div;
	/** Valid only for 9104 AVSP_GEARBOX_10_4_RS_FEC mode: */
	bool disable_fec;
	bool enable_fec_bip_error_workaround;
	bool disable_mod_during_fec_setup;
	bool disable_host_during_fec_setup;
	bool run_an_before_fec_setup;	/** seq 1 */
	/** seq 2, Ignored if run_an_before_fec_setup is set. */
	bool run_an_after_fec_setup;
	bool run_kr_after_an;	/** If AN disabled, KR still runs (when?) */
	uint32_t extra_opts;	/** For testing/debugging purposes. */

};

int avagi_sign_mag(int value, int mag_bits);
int avago_twos_comp(int bits, int mag_bits);

/**
 * Uploads firmware stored in u32 format.
 *
 * @param	ah	Avago handle
 * @param	prtad	port address to upload firmware to
 * @param	refclk_halve	Set true to cut reference clock in half
 * @param	serdes_rom_size	Size of SerDes ROM image in 32-bit words
 * @param[in]	serdes_rom	Pointer to array of uint32_t containing SerDes
 *				ROM image
 * @param	sbm_rom_size	Size of SBM ROM in 32-bit words
 * @param[in]	sbm_rom		Pointer to array of uint32_t containing SBM
 *				ROM image
 *
 * @return	0 for success, otherwise error
 */
int avsp_upload_firmware_u32(struct avago_hdl *ah, uint32_t prtad,
			     bool refclk_halve, int serdes_rom_size,
			     const uint32_t *serdes_rom, int sbm_rom_size,
			     const uint32_t *sbm_rom);

/**
 * @brief   Allows the user to enable/disable self healing
 *
 * @param	ah	Avago handle
 * @param	prtad	Port address
 * @param	self_heal_enable	Set true to enable self-healing, false
 *					to disable.
 */
void avsp_set_self_healing(struct avago_hdl *ah, uint32_t prtad,
			   bool self_heal_enable);

#endif /* AVSP_H_ */
