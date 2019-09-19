/* AAPL CORE Revision: 2.3.1-beta */

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

/* Core SerDes types and functions.
 */

/** Doxygen File Header
 * * @file
 ** @brief Core SerDes types and functions.
 **/

#ifndef __AVAGO_SERDES_H__
#define __AVAGO_SERDES_H__
struct avago_hdl;

int avago_serdes_get_rx_register_clock(struct avago_hdl *ah, uint32_t addr);

int avago_serdes_get_rx_data(struct avago_hdl *ah, uint32_t sbus_addr,
			     long data[4]);

/**
 * @brief	Gets the SerDes LSB revision.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address number
 *
 * @details	 Reads the revision of SerDes LSB using an AVAGO_LSB_DIRECT read./
 *
 * @return	Revision of the LSB (> 0), or -1 on error.
 *
 * @see avago_firmware_get_rev(), avago_firmware_get_build_id().
 */
int avago_serdes_get_lsb_rev(struct avago_hdl *ah, uint32_t addr);

/**
 * @brief	Selects memory access type.
 * @details	AVAGO_LSB_DIRECT uses SBus transactions to read/write data,
 *		which avoids interrupting the processor, and also works if the
 *		SPICO processor is not running.
 * @details	AVAGO_ESB_DIRECT also works if the processor is not running,
 *		though it is much slower than AVAGO_ESB.
 * @details	The *_PREHALTED versions assume the caller invokes
 *		avago_spico_halt() and avago_spico_resume()
 *		appropriately, and are used to avoid the overhead of halting
 *		and restarting the processor for multiple successive reads.
 */
enum avago_serdes_mem_type {
	AVAGO_ESB,	      /** Electrical Sub-Block access. */
	AVAGO_ESB_DIRECT,     /** ESB access using SBus instead of an interrupt. */
	AVAGO_LSB,	      /** Logical Sub-Block access. */
	AVAGO_LSB_DIRECT,     /** LSB access using SBus instead of an interrupt. */
	AVAGO_DMEM,	      /** Data memory access. */
	AVAGO_DMEM_PREHALTED, /** Data memory access, processor already halted. */
	AVAGO_IMEM,	      /** Instruction memory access. */
	/** Instruction memory access, processor already halted. */
	AVAGO_IMEM_PREHALTED
};

int avago_serdes_mem_rd(struct avago_hdl *ah, uint32_t sbus_addr,
			enum avago_serdes_mem_type type, uint32_t addr);

void avago_serdes_mem_wr(struct avago_hdl *ah, uint32_t sbus_addr,
			 enum avago_serdes_mem_type type, uint32_t addr,
			 uint32_t data);

int avago_serdes_mem_rmw(struct avago_hdl *ah, uint32_t sbus_addr,
			 enum avago_serdes_mem_type type, uint32_t addr,
			 uint32_t data, uint32_t mask);

/**< @brief The TX input data selections. */
enum avago_serdes_tx_data_sel {
	AVAGO_SERDES_TX_DATA_SEL_PRBS7    = 0, /**< PRBS7 (x^7+x^6+1) generator. */
	AVAGO_SERDES_TX_DATA_SEL_PRBS9    = 1, /**< PRBS9 (x^9+x^7+1). */
	AVAGO_SERDES_TX_DATA_SEL_PRBS11   = 2, /**< PRBS11 (x^11+x^9+1). */
	AVAGO_SERDES_TX_DATA_SEL_PRBS15   = 3, /**< PRBS15 (x^15+x^14+1). */
	AVAGO_SERDES_TX_DATA_SEL_PRBS23   = 4, /**< PRBS23 (x^23+x^18+1). */
	AVAGO_SERDES_TX_DATA_SEL_PRBS31   = 5, /**< PRBS31 (x^31+x^28+1). */
	AVAGO_SERDES_TX_DATA_SEL_USER     = 7, /**< User pattern generator. */
	AVAGO_SERDES_TX_DATA_SEL_CORE     = 8, /**< External data. */
	AVAGO_SERDES_TX_DATA_SEL_LOOPBACK = 9  /**< Parallel loopback from receiver. */
};

enum avago_serdes_tx_data_sel avago_serdes_get_tx_data_sel(struct avago_hdl *ah,
							   uint32_t sbus_addr);

int avago_serdes_set_tx_data_sel(struct avago_hdl *ah, uint32_t sbus_addr,
				 enum avago_serdes_tx_data_sel input);

int avago_serdes_get_tx_user_data(struct avago_hdl *ah, uint32_t sbus_addr,
				  long data[4]);

int avago_serdes_set_tx_user_data(struct avago_hdl *ah, uint32_t sbus_addr,
				  long data[4]);

int avago_serdes_get_tx_rx_ready(struct avago_hdl *ah, uint32_t sbus_addr,
				 bool * tx, bool * rx);

int avago_serdes_set_tx_rx_enable(struct avago_hdl *ah, uint32_t sbus_addr,
				  bool tx, bool rx, bool tx_output);

bool avago_serdes_get_tx_output_enable(struct avago_hdl *ah,
				       uint32_t sbus_addr);

int avago_serdes_set_tx_output_enable(struct avago_hdl *ah, uint32_t sbus_addr,
				      bool enable);

bool avago_serdes_get_tx_invert(struct avago_hdl *ah, uint32_t sbus_addr);

int avago_serdes_set_tx_invert(struct avago_hdl *ah, uint32_t sbus_addr,
			       bool invert);

int avago_serdes_tx_inject_error(struct avago_hdl *ah, uint32_t sbus_addr,
				 uint32_t num_bits);

int avago_serdes_rx_inject_error(struct avago_hdl *ah, uint32_t sbus_addr,
				 uint32_t num_bits);

/** @brief The width mode and line encoding selections */
enum avago_serdes_line_encoding {
	AVAGO_SERDES_NRZ = 0,	  /** Transmit using 2 levels of encoding */
	AVAGO_SERDES_PAM2 = 0,	  /** Transmit using 2 levels of encoding */
	AVAGO_SERDES_PAM4 = 1	  /** Transmit using 4 levels of encoding */
};

int avago_serdes_get_tx_rx_width(struct avago_hdl *ah, uint32_t sbus_addr,
				 int *tx_width, int *rx_width);

int avago_serdes_set_tx_rx_width(struct avago_hdl *ah, uint32_t sbus_addr,
				 int tx_width, int rx_width);

bool avago_serdes_get_tx_line_encoding(struct avago_hdl *ah, uint32_t addr);

bool avago_serdes_get_rx_line_encoding(struct avago_hdl *ah, uint32_t addr);

int avago_serdes_get_tx_rx_line_encoding(struct avago_hdl *ah, uint32_t addr,
					 enum avago_serdes_line_encoding *tx,
					 enum avago_serdes_line_encoding *rx);
int avago_serdes_set_tx_rx_width_pam(struct avago_hdl *ah, uint32_t sbus_addr,
				     int tx_width, int rx_width,
				     enum avago_serdes_line_encoding
				     tx_encoding,
				     enum avago_serdes_line_encoding
				     rx_encoding);

/**< @brief The TX equalization settings. */
struct avago_serdes_tx_eq {
	short pre;            /**< Pre-cursor setting */
	short atten;          /**< Attenuator setting */
	short post;           /**< Post-cursor setting */
	short slew;           /**< Slew rate setting [0..3], 0 is fastest */
	short pre2;           /**< Pre-cursor setting */
	short pre3;           /**< Pre-cursor setting */
	short vert;           /**< Post-cursor setting */
};


void avago_serdes_tx_eq_init(struct avago_serdes_tx_eq *ptr);

int avago_serdes_get_tx_eq(struct avago_hdl *ah, uint32_t sbus_addr,
			   struct avago_serdes_tx_eq *tx_eq);

int avago_serdes_set_tx_eq(struct avago_hdl *ah, uint32_t sbus_addr,
			   struct avago_serdes_tx_eq *tx_eq);

/** @brief Specify the pattern to use in validating the received data. */
enum  avago_serdes_rx_cmp_data {
	AVAGO_SERDES_RX_CMP_DATA_PRBS7    = 0, /**< PRBS7 (x^7+x^6+1) generator. */
	AVAGO_SERDES_RX_CMP_DATA_PRBS9    = 1, /**< PRBS9 (x^9+x^7+1). */
	AVAGO_SERDES_RX_CMP_DATA_PRBS11   = 2, /**< PRBS11 (x^11+x^9+1). */
	AVAGO_SERDES_RX_CMP_DATA_PRBS15   = 3, /**< PRBS15 (x^15+x^14+1). */
	AVAGO_SERDES_RX_CMP_DATA_PRBS23   = 4, /**< PRBS23 (x^23+x^18+1). */
	AVAGO_SERDES_RX_CMP_DATA_PRBS31   = 5, /**< PRBS31 (x^31+x^28+1). */
	AVAGO_SERDES_RX_CMP_DATA_SELF_SEED= 7, /**< Auto-seed to received 40 bit repeating pattern. */
	/**< NOTE: This is USER mode in firmware. */
	AVAGO_SERDES_RX_CMP_DATA_OFF      = 8  /**< Disable cmp data generator */
};

enum avago_serdes_rx_cmp_data avago_serdes_get_rx_cmp_data(struct avago_hdl *ah,
							   uint32_t sbus_addr);

int avago_serdes_set_rx_cmp_data(struct avago_hdl *ah, uint32_t sbus_addr,
				 enum avago_serdes_rx_cmp_data cmp_data);

bool avago_serdes_get_rx_input_loopback(struct avago_hdl *ah,
					uint32_t sbus_addr);

int avago_serdes_set_rx_input_loopback(struct avago_hdl *ah, uint32_t sbus_addr,
				       bool select_internal);

bool avago_serdes_get_rx_invert(struct avago_hdl *ah, uint32_t sbus_addr);

int avago_serdes_set_rx_invert(struct avago_hdl *ah, uint32_t sbus_addr,
			       bool invert);

int avago_serdes_get_rx_live_data(struct avago_hdl *ah, uint32_t sbus_addr);

/** @brief Receiver data comparison mode. */
enum avago_serdes_rx_cmp_mode {
	AVAGO_SERDES_RX_CMP_MODE_OFF         = 0x0000, /**< Turn Off */
	AVAGO_SERDES_RX_CMP_MODE_XOR         = 0x0100, /**< Test^Mission */
	AVAGO_SERDES_RX_CMP_MODE_TEST_PATGEN = 0x0120, /**< Test^PatGen. */
	AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN = 0x0200  /**< Mission^PatGen */
};

enum avago_serdes_rx_cmp_mode avago_serdes_get_rx_cmp_mode(struct avago_hdl *ah,
							   uint32_t sbus);
int avago_serdes_set_rx_cmp_mode(struct avago_hdl *ah, uint32_t sbus,
				 enum avago_serdes_rx_cmp_mode mode);

/** @brief Specify the RX termination value. */
enum avago_serdes_rx_term {
	AVAGO_SERDES_RX_TERM_AGND = 0,    /**< Resistor to ground */
	AVAGO_SERDES_RX_TERM_AVDD = 1,    /**< Resistor to AVDD */
	AVAGO_SERDES_RX_TERM_FLOAT = 2    /**< No resistor. */
};

enum avago_serdes_rx_term avago_serdes_get_rx_term(struct avago_hdl *ah,
						   uint32_t sbus_addr);

int avago_serdes_set_rx_term(struct avago_hdl *ah, uint32_t sbus_addr,
			     enum avago_serdes_rx_term term);

/** @brief Select the SPICO clock source. */
enum avago_serdes_spico_clk {
	AVAGO_SERDES_SPICO_REFCLK = 0,		      /** default */
	AVAGO_SERDES_SPICO_PCIE_CORE_CLK = 1,
	AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED = 2,
	AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED = 3,
	AVAGO_SERDES_SPICO_REFCLK_DIV2 = 4,
	AVAGO_SERDES_SPICO_PCIE_CORE_CLK_DIV2 = 5,
	AVAGO_SERDES_SPICO_TX_F10_CLK_FIXED_DIV2 = 6,
	AVAGO_SERDES_SPICO_TX_F40_CLK_FIXED_DIV2 = 7,
	AVAGO_SERDES_SPICO_TX_DATA_CLK           = 2,   /**< P1 only */
	AVAGO_SERDES_SPICO_TX_DATA_CLK_DIV2      = 6,   /**< P1 only */
	AVAGO_SERDES_SPICO_TX_F20_CLK_FIXED = 0xe,
	AVAGO_SERDES_SPICO_TX_F80_CLK_FIXED = 0xf
};

enum avago_serdes_spico_clk avago_serdes_get_spico_clk_src(struct avago_hdl *ah,
							   uint32_t sbus_addr);
int avago_serdes_set_spico_clk_src(struct avago_hdl *ah, uint32_t sbus_addr,
				   enum avago_serdes_spico_clk src);

/** @brief Select the Tx PLL clock source. */
enum avago_serdes_tx_pll_clk {
	AVAGO_SERDES_TX_PLL_REFCLK             = 0x00,    /**< Use REF clock; default. */
	AVAGO_SERDES_TX_PLL_RX_DIVX            = 0x10,    /**< Use the recovered clock. */
	AVAGO_SERDES_TX_PLL_OFF                = 0x30,    /**< Use no clock. */
	AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK      = 0x70,    /**< Use core clock. */
	AVAGO_SERDES_TX_PLL_PCIE_CORE_CLK_DIV2 = 0xf0     /**< Use core clock / 2. */
};

enum avago_serdes_tx_pll_clk avago_serdes_get_tx_pll_clk_src(struct avago_hdl
							     *ah,
							     uint32_t
							     sbus_addr);
int avago_serdes_set_tx_pll_clk_src(struct avago_hdl *ah, uint32_t sbus_addr,
				    enum avago_serdes_tx_pll_clk src);

bool avago_serdes_get_electrical_idle(struct avago_hdl *ah, uint32_t sbus_addr);
bool avago_serdes_get_signal_ok_enable(struct avago_hdl *ah,
				       uint32_t sbus_addr);
int avago_serdes_get_signal_ok_threshold(struct avago_hdl *ah,
					 uint32_t sbus_addr);

bool avago_serdes_get_signal_ok(struct avago_hdl *ah, uint32_t sbus_addr,
				bool reset);
int avago_serdes_initialize_signal_ok(struct avago_hdl *ah, uint32_t sbus_addr,
				      int threshold);

bool avago_serdes_get_frequency_lock(struct avago_hdl *ah, uint32_t sbus_addr);
bool avago_serdes_get_signal_ok_live(struct avago_hdl *ah, uint32_t sbus_addr);

uint32_t avago_serdes_get_errors(struct avago_hdl *ah, uint32_t sbus_addr,
				 enum avago_serdes_mem_type type,
				 bool reset_count_after_get);
bool avago_serdes_get_error_flag(struct avago_hdl *ah, uint32_t addr,
				 bool reset);
void avago_serdes_error_flag_reset(struct avago_hdl *ah, uint32_t addr);
bool avago_serdes_error_reset(struct avago_hdl *ah, uint32_t addr);
bool avago_serdes_enable_core_to_cntl(struct avago_hdl *ah, uint32_t addr,
				      bool enable_tx_rx_en,
				      bool enable_low_power);
bool avago_serdes_slip_bits(struct avago_hdl *ah, uint32_t sbus_addr,
			    uint32_t bits);
bool avago_serdes_slip_rx_phase(struct avago_hdl *ah, uint32_t sbus_addr,
				uint32_t bits, bool apply_at_init);
bool avago_serdes_slip_tx_phase(struct avago_hdl *ah, uint32_t sbus_addr,
				uint32_t bits, bool apply_at_init);

/** @brief Initialize a SerDes slice. */
enum avago_serdes_init_mode {
	AVAGO_PRBS31_ILB,    /** Run a PRBS check in ILB; leave SerDes in ILB running PRBS 31 data. */
	AVAGO_PRBS31_ELB,    /** Run a PRBS check in ILB; leave SerDes in ELB running PRBS 31 data. */
	AVAGO_CORE_DATA_ILB, /** Run a PRBS check in ILB; leave SerDes in ILB sending data from the ASIC core. */
	AVAGO_CORE_DATA_ELB, /** Run a PRBS check in ILB; leave SerDes in ELB sending data from the ASIC core. */
	AVAGO_INIT_ONLY	     /** Don't check any errors during init, just init the serdes. */
	/** SerDes will be left in ELB mode sending data from the ASIC core. */
};

struct avago_serdes_init_config {
	bool sbus_reset;
	bool spico_reset;
	bool init_tx;
	bool init_rx;
	enum avago_serdes_init_mode init_mode;

	uint32_t tx_divider;
	enum avago_serdes_line_encoding tx_encoding;
	bool tx_output_en;
	uint32_t tx_width;	/** TX width mode: 10, 20, 40, 80, 16, 32, or 64.  Valid values are SerDes dependent. */
	bool tx_phase_cal;
	bool refclk_sync_master;
	bool tx_inversion;  /** Enable TX polarity inversion if ELB selected */

	uint32_t rx_divider;
	enum avago_serdes_line_encoding rx_encoding;
	uint32_t rx_width;	/** RX width mode: 10, 20, 40, 80, 16, 32, or 64.  Valid values are SerDes dependent. */
	bool signal_ok_en;
	int signal_ok_threshold;    /** [0-15] */
	bool rx_inversion;  /** Enable RX polarity inversion if ELB selected */
};

struct avago_serdes_init_config *avago_serdes_init_config_construct(struct
								    avago_hdl
								    *ah);

void avago_serdes_init_config_destruct(struct avago_hdl *ah,
				       struct avago_serdes_init_config *config);
int avago_serdes_init(struct avago_hdl *ah, uint32_t sbus_addr,
		      struct avago_serdes_init_config *config);
int avago_serdes_init_quick(struct avago_hdl *ah, uint32_t sbus_addr,
			    uint32_t divider);

/** Controls what type of tuning to execute */
enum avago_serdes_dfe_tune_mode {
	/** Initial calibration, coarse+fine tuning. (Default) */
	AVAGO_DFE_ICAL,
	/** Periodic calibration, fine tuning, no LF,HF adjustments */
	AVAGO_DFE_PCAL,
	/** Initial calibration, coarse tuning without PCAL */
	AVAGO_DFE_ICAL_ONLY,
	/** Launch continuous pCal */
	AVAGO_DFE_START_ADAPTIVE,
	/** Stop continuous pCal */
	AVAGO_DFE_STOP_ADAPTIVE,
	/** Enable SerDes to participate in Round-Robin pCal */
	AVAGO_DFE_ENABLE_RR,
	/** Disable SerDes from participating in Round-Robin pCal */
	AVAGO_DFE_DISABLE_RR
};

struct avago_serdes_dfe_tune {
	bool fixed[4];
	int value[4];
	uint32_t dfe_disable;	/** When set, skip tuning of dfeTaps */
	/** Select tuning mode to execute */
	enum avago_serdes_dfe_tune_mode tune_mode;
};


bool avago_serdes_dfe_running(struct avago_hdl *ah, uint32_t sbus_addr);

int avago_serdes_dfe_pause(struct avago_hdl *ah, uint32_t sbus_addr,
			   uint32_t * rr_enabled);

int avago_serdes_dfe_wait(struct avago_hdl *ah, uint32_t sbus_addr);

int avago_serdes_dfe_resume(struct avago_hdl *ah, uint32_t sbus_addr,
			    uint32_t rr_enable);
#endif /* __AVAGO_SERDES_H__ */
