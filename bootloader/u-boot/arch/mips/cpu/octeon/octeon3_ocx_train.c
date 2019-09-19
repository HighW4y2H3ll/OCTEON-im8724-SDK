/***********************license start************************************
 * Copyright (c) 2017 Cavium, Inc. <support@cavium.com>.  All rights
 * reserved.
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
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#ifdef __U_BOOT__
# include <common.h>
# include <linux/types.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/cvmx-ocx-defs.h>
# include <asm/arch/cvmx-gserx-defs.h>

# ifndef CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2
#  error CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2 MUST be set and U-Boot must execute
#  error out of L2 in order for this to be supported.
# endif
#endif

#define DEBUG_TRAIN_MESSAGES		false
#define DEBUG_STATE			false
#define DEBUG_PARAMETERS		false
#define DEBUG_TUNING_CHANGES		false
#define DEBUG_TIME_RX_EQ		false
#define DEBUG_ONE_LANE_AT_AT_TIME	false

/**
 * During TX Link Trainingthis is the TX POST Tap value that is used when
 * the INITIALIZE coefficients update is received.  It is also the TX SWING Tap
 * value used when the Base-R Link Training begins.
 *
 * The default value is 0xD which is optimal for the ebb780x boards.
 *
 * This is for the opposite node.
 */
#ifndef CONFIG_OCTEON_CCPI_TX_SWING_NO
# define CONFIG_OCTEON_CCPI_TX_SWING_NO		0x0D
#endif

/**
 * During TX Base-R Link Training, this is the TX PRE and Post Tap
 * values that are used when the INITIALIZE coefficients update is
 * received.  It is also the TX PRE and Post Tap value used when the Base-R Link
 * Training begins.  The upper nubble is the post tap and the lower nibble is
 * the pre-tap.
 *
 * The default value is 0xD0
 *
 * This is for the opposite node.
 */
#ifndef CONFIG_OCTEON_CCPI_TX_PREEMP_TAP_NO
# define CONFIG_OCTEON_CCPI_TX_PREEMP_TAP_NO	0xD0
#endif

/**
 * Loss in db for CCPI channel.  Typically this only matters if it is less
 * than 10 or greater than or equal to 10.
 *
 * The default is 0.
 */
#ifndef CONFIG_OCTEON_CCPI_CHANNEL_LOSS
# define CONFIG_OCTEON_CCPI_CHANNEL_LOSS	10
#endif

/**
 * During TX Link Trainingthis is the TX POST Tap value that is used when
 * the INITIALIZE coefficients update is received.  It is also the TX SWING Tap
 * value used when the Base-R Link Training begins.
 *
 * The default value is 0xD.
 *
 * This is for the local node.
 */
#ifndef CONFIG_OCTEON_CCPI_TX_SWING
# define CONFIG_OCTEON_CCPI_TX_SWING		0x0D
#endif

/**
 * During TX Base-R Link Training, this is the TX PRE and Post Tap
 * values that are used when the INITIALIZE coefficients update is
 * received.  It is also the TX PRE and Post Tap value used when the Base-R Link
 * Training begins.  The upper nubble is the post tap and the lower nibble is
 * the pre-tap.
 *
 * The default value is 0xD0
 *
 * This is for the local node.
 */
#ifndef CONFIG_OCTEON_CCPI_TX_PREEMP_TAP
# define CONFIG_OCTEON_CCPI_TX_PREEMP_TAP	0xD0
#endif

/** CPU reference clock speed */
#define CPU_REF_CLK		50000000

/** Number of SERDES lanes used for CCPI */
#define CCPI_LANES		24

/** Number of logical links that must work for success */
#define CCPI_MIN_LINKS		2

/** Total number of logical links */
#define CCPI_MAX_LINKS		3

/** Seconds to wait for CCPI links */
#ifdef DEBUG
# define CCPI_MASTER_TIMEOUT	180
#else
# define CCPI_MASTER_TIMEOUT	60
#endif

/** Maximum number of training steps a lane can take before training ends */
#define CCPI_MAX_STEPS		15

#define REF_CLOCK		50000000	/** 50MHz */

#define TRAIN_INIT_TX_MAIN	13		/** Starting swing/amplitude */
#define TRAIN_INIT_TX_PRE	2		/** Starting pre-emphasis */
#define TRAIN_INIT_TX_POST	11		/** Starting post-emphasis */

/** Lower 6 bits of CU and SR are the data we can use */
#define TRAIN_DATA_MASK		0x3f
#define RXT_ESM_HOLD		0	/** RX equalization request value hold */
#define RXT_ESM_INC		1	/** RX equalization request value increment */
#define RXT_ESM_DEC		2	/** RX equalization request value decrement */

enum cvmx_bgx_spu_br_train_cup_e {
	HOLD = 0,
	DECREMENT = 1,
	INCREMENT = 2,
	RSV_CMD = 3
};

enum cvmx_bgx_spu_br_train_cst_e {
	MAXIMUM = 3,
	MINIMUM = 2,
	NOT_UPDATED = 0,
	UPDATED = 1
};

enum lane_state_e {
	STATE_START,			/** Done nothing, initial start state */
	STATE_WAIT_EIE,			/** Waiting for Eletrical Idle Exit */
	STATE_WAIT_CDR,			/** Waiting for CDR lock */
	/** Waiting for special training message signalling start of message check */
	STATE_WAIT_TRAIN_PRESET,
	/** Sending message to check reliability if training messages */
	STATE_MESSAGE_CHECK,
	STATE_TRAINING_INIT,		/** Start training */
	STATE_TRAINING_IDLE = 0x8,	/** Training, no outstanding CU nor SR */
	STATE_TRAINING_UPDATE = 0x9,	/** Training, no CU, SR in progress */
	STATE_TRAINING_REQ = 0xa,	/** Training, CU in progress, no SR */
	STATE_TRAINING_REQ_UPDATE = 0xb,/** Training, CU in progress, SR in progress */
	/** Waiting for CCPI to mark lane as ready */
	STATE_WAIT_FOR_READY,
	STATE_READY,			/** Lane is ready for use */
};

union cvmx_bgx_spu_br_train_cup_s {
	struct {
#ifdef __BIG_ENDIAN_BITFIELD
		uint16_t	reserved_14_15:2;
		uint16_t	preset:1;
		uint16_t	init:1;
		uint16_t	reserved_6_11:6;
		uint16_t	post_cup:2;
		uint16_t	main_cup:2;
		uint16_t	pre_cup:2;
#else
		uint16_t	pre_cup:2;
		uint16_t	main_cup:2;
		uint16_t	post_cup:2;
		uint16_t	reserved_6_11:6;
		uint16_t	init:1;
		uint16_t	preset:1;
		uint16_t	reserved_14_15:2;
#endif
	} s;
	uint16_t u16;
};

union cvmx_bgx_spu_br_train_rep_s {
	struct {
#ifdef __BIG_ENDIAN_BITFIELD
		uint16_t	rx_ready:1;
		uint16_t	reserved_6_14:9;
		uint16_t	post_cst:2;
		uint16_t	main_cst:2;
		uint16_t	pre_cst:2;
#else
		uint16_t	pre_cst:2;
		uint16_t	main_cst:2;
		uint16_t	post_cst:2;
		uint16_t	reserved_6_14:9;
		uint16_t	rx_ready:1;
#endif
	} s;
	uint16_t u16;
};

struct lane_state {
	/** Last valid received coef update from link partner */
	union cvmx_bgx_spu_br_train_cup_s lp_cu;
	/** Last valid received status response from link partner */
	union cvmx_bgx_spu_br_train_rep_s lp_sr;
	/** Current coef update for local device */
	union cvmx_bgx_spu_br_train_cup_s ld_cu;
	/** Current status response for local device */
	union cvmx_bgx_spu_br_train_rep_s ld_sr;
	/** RX equalization figure of merit */
	uint8_t merit;
	/** Set when lane complete message check */
	uint8_t chk_done:1;
	/** Set when lane is done with training */
	uint8_t rx_ready:1;
	/** RX equalization recommended PRE change (RXT_ESM_*) */
	uint8_t desired_pre:2;
	/** RX equalization recommended MAIN change (RXT_ESM_*) */
	uint8_t desired_main:2;
	/** RX equalization recommended POST change (RXT_ESM_*) */
	uint8_t desired_post:2;
	/** Initial TX MAIN value */
	uint8_t init_main;
	/** Initial TX PRE value */
	uint8_t init_pre;
	/** Initial TX POST value */
	uint8_t init_post;
	/** Current TX MAIN value */
	uint8_t tx_main;
	/** Current TX PRE value */
	uint8_t tx_pre;
	/** Current TX POST value */
	uint8_t tx_post;
	/** Number of training steps for the lane */
	uint8_t steps;
	/** Current state of the lane (enumeration) */
	enum lane_state_e lane_state:8;
};

/** Currently executing node number */
static int node;
static struct lane_state lane_state[CCPI_LANES];

static inline uint32_t ccpi_get_core_num(void)
{
	uint64_t ebase;

	asm("dmfc0 %0, $15,1" : "=r"(ebase));
	return ebase & 0x3ff;
}

static inline uint32_t ccpi_get_node_num(void)
{
	return (ccpi_get_core_num() >> 7) & 7;
}
/**
 * Get the speed of the CCPI lanes
 *
 * @return Speed in Mhz
 */
static int ccpi_get_speed(void)
{
	union cvmx_gserx_spd gserx_spd;
	int qlm = 8;
	static const uint16_t speeds[16] = {
		5000, 2500, 5000, 8000, 8000, 8000, 3125, 5000,
		6250, 8000, 10312, 3125, 5000, 6250, 10312, 0
	};

	/* Use the OCI strapping to find the speed. */
	gserx_spd.u64 = cvmx_read_csr(CVMX_GSERX_SPD(qlm));

	debug("%s: speed: %u\n", __func__, speeds[gserx_spd.s.spd]);
	return speeds[gserx_spd.s.spd];
}

/**
 * Update the per lane state with the current LP messages
 *
 * @param ccpi_lane Lane to update
 */
static void ccpi_update_lp_messages(int ccpi_lane)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	union cvmx_ocx_lnex_trn_lp trn_lp;

	debug("%s(%d)\n", __func__, ccpi_lane);
	trn_lp.u64 = cvmx_read_csr_node(node, CVMX_OCX_LNEX_TRN_LP(ccpi_lane));
	if (trn_lp.s.lp_cu_val) {
		if (DEBUG_TRAIN_MESSAGES &&
			(lstate->lp_cu.u16 != trn_lp.s.lp_cu_dat))
			printf("Lane %2d: LP CU 0x%04x -> 0x%04x\n",
			       ccpi_lane, lstate->lp_cu.u16, trn_lp.s.lp_cu_dat);
		lstate->lp_cu.u16 = trn_lp.s.lp_cu_dat;
	}
	if (trn_lp.s.lp_sr_val) {
		if (DEBUG_TRAIN_MESSAGES &&
			(lstate->lp_sr.u16 != trn_lp.s.lp_sr_dat))
			printf("Lane %2d: LP SR 0x%04x -> 0x%04x\n",
			       ccpi_lane, lstate->lp_sr.u16, trn_lp.s.lp_sr_dat);
		lstate->lp_sr.u16 = trn_lp.s.lp_sr_dat;
	}
}

/**
 * Enables or disables software equalization
 *
 * @param	qlm	QLM number to set
 * @param	lane	lane on QLM to set
 * @param	enable	true to enable software equalization, false to disable
 */
static void ccpi_set_sw_equalization(int qlm, int lane, bool enable)
{
	union cvmx_gserx_br_rxx_ctl br_rxx_ctl;

	debug("%s(%d, %d, %s)\n", __func__, qlm, lane, enable ? "true" : "false");
	/* Enable software control of RX equalization */
	br_rxx_ctl.u64 = cvmx_read_csr_node(node,
					    CVMX_GSERX_BR_RXX_CTL(lane, qlm));
	br_rxx_ctl.s.rxt_swm = enable ? 1 : 0;
	cvmx_write_csr_node(node,
			    CVMX_GSERX_BR_RXX_CTL(lane, qlm),
			    br_rxx_ctl.u64);
}

/**
 * Clears the completion flag and initiates a new request
 *
 * @param	qlm	QLM number
 * @param	lane	lane on QLM
 */
static void ccpi_start_rx_request(int qlm, int lane)
{
	union cvmx_gserx_br_rxx_eer br_rxx_eer;

	debug("%s(%d, %d)\n", __func__, qlm, lane);
	/* Clear the completion flag and initiate a
	 * new request
	 */
	br_rxx_eer.u64 = cvmx_read_csr_node(node,
					    CVMX_GSERX_BR_RXX_EER(lane, qlm));
	br_rxx_eer.s.rxt_esv = 0;
	br_rxx_eer.s.rxt_eer = 1;
	cvmx_write_csr_node(node,
			    CVMX_GSERX_BR_RXX_EER(lane, qlm),
			    br_rxx_eer.u64);
}

/**
 * Waits for RX equalization equalization status to become available
 *
 * @param	qlm	qlm number
 * @param	lane	qlm lane number
 *
 * @return	Value of gserx_br_rxx_eer register, check rxt_esv for valid
 *		status.
 */
static union cvmx_gserx_br_rxx_eer ccpi_wait_rx_equalization(int qlm, int lane,
							     uint32_t start_time,
							     uint32_t timeout)
{
	union cvmx_gserx_br_rxx_eer br_rxx_eer;
	int ccpi_lane = (qlm - 8) * 4 + lane;
	uint32_t now;

	do {
		br_rxx_eer.u64 = cvmx_read_csr_node(node,
						    CVMX_GSERX_BR_RXX_EER(lane, qlm));
		/* Update link partner message state */
		ccpi_update_lp_messages(ccpi_lane);
		now = get_timer(start_time);
	} while (now < timeout && !br_rxx_eer.s.rxt_esv);

	if (DEBUG_TIME_RX_EQ)
		printf("QLM %2d, lane %d: RX Equalization: %u ms%s\n",
		       qlm, lane, now,
		       br_rxx_eer.s.rxt_esv ? " (done)" : "");
	return br_rxx_eer;
}

void ccpi_update_rx_equalization(void)
{
	union cvmx_gserx_br_rxx_eer br_rxx_eer;
	struct lane_state *lstate;
	int qlm;
	int lane;
	int ccpi_lane;
	uint32_t start_time;
	const uint32_t timeout = 100;

	debug("%s()\n", __func__);
	for (qlm = 8; qlm < 14; qlm++) {
		for (lane = 0; lane < 4; lane++) {
			ccpi_lane = (qlm - 8) * 4 + lane;
			lstate = &lane_state[ccpi_lane];

			if ((lstate->lane_state >= STATE_WAIT_TRAIN_PRESET) &&
			    (lstate->lane_state < STATE_WAIT_FOR_READY)) {
				ccpi_set_sw_equalization(qlm, lane, true);
				ccpi_start_rx_request(qlm, lane);
			}
		}
	}

	mdelay(10);

	start_time = get_timer(0);

	for (qlm = 8; qlm < 14; qlm++) {
		for (lane = 0; lane < 4; lane++) {
			ccpi_lane = (qlm - 8) * 4 + lane;
			lstate = &lane_state[ccpi_lane];

			if ((lstate->lane_state >= STATE_WAIT_TRAIN_PRESET) &&
			    (lstate->lane_state < STATE_WAIT_FOR_READY)) {
				br_rxx_eer = ccpi_wait_rx_equalization(qlm, lane,
								       start_time,
								       timeout);

				ccpi_set_sw_equalization(qlm, lane, false);

				if (br_rxx_eer.s.rxt_esv) {
					lstate->merit =
						(br_rxx_eer.s.rxt_esm >> 6) & 0xff;
					lstate->desired_pre =
						br_rxx_eer.s.rxt_esm & 3;
					lstate->desired_main =
						(br_rxx_eer.s.rxt_esm >> 2) & 3;
					lstate->desired_post =
						(br_rxx_eer.s.rxt_esm >> 4) & 3;
					if (lstate->desired_pre == 3)
						lstate->desired_pre = RXT_ESM_HOLD;
					if (lstate->desired_main == 3)
						lstate->desired_main = RXT_ESM_HOLD;
					if (lstate->desired_post == 3)
						lstate->desired_post = RXT_ESM_HOLD;
				} else {
					lstate->merit = 0;
					lstate->desired_pre = RXT_ESM_HOLD;
					lstate->desired_main = RXT_ESM_HOLD;
					lstate->desired_post = RXT_ESM_HOLD;
				}
			}
		}
	}
	debug("%s: Done\n", __func__);
}

/**
 * Check if the supplied tuning values are valid and could be applied to a SERDES
 * lane. This function doesn't apply the setting, just range checks.
 *
 * @param tx_swing Desired swing value
 * @param tx_pre   Desired pre-emphasis
 * @param tx_post  Desired post-emphasis
 *
 * @return True if the settings are valid, false otherwise
 */
static bool is_tuning_valid(int tx_swing, int tx_pre, int tx_post)
{
	debug("%s(%d, %d, %d)\n", __func__, tx_swing, tx_pre, tx_post);
	/* SERDES range limits */
	/* 1) (pre + post) - main <= 2 */
	/* 2) pre + post + main <= 35 */
	/* 3) 0 <= pre <= 10 (0=Max, 10=Min) */
	/* 4) 0 <= main <= 25 (0=Max, 25=Min) */
	/* 5) 0 <= post <= 15 (0=Max, 15=Min) */
	if (tx_pre + tx_post - tx_swing > 2)
		return false;
	if (tx_pre + tx_post + tx_swing > 35)
		return false;
	if ((tx_pre < 0) || (tx_pre > 10))
		return false;
	if ((tx_swing < 0) || (tx_swing > 25))
		return false;
	if ((tx_post < 0) || (tx_post > 15))
		return false;
	return true;
}

/**
 * Apply SERDES tuning parameters to a specific CCPI lane
 *
 * @param ccpi_lane Lane to apply settings to
 * @param tx_swing  New swing / amplitude
 * @param tx_pre    New pre-emphasis
 * @param tx_post   New post-emphasis
 */
static void ccpi_tx_tune(int ccpi_lane, int tx_swing, int tx_pre, int tx_post)
{
	union cvmx_gserx_lanex_tx_cfg_0 lanex_tx_cfg_0;
	union cvmx_gserx_lanex_tx_cfg_1 lanex_tx_cfg_1;
	union cvmx_gserx_lanex_tx_pre_emphasis lanex_tx_pre_emp;
	union cvmx_gserx_lanex_pcs_ctlifc_0 lanex_pcs_ctlifc_0;
	union cvmx_gserx_lanex_pcs_ctlifc_2 lanex_pcs_ctlifc_2;

	int qlm = ccpi_lane / 4 + 8;
	int lane = ccpi_lane & 3;

	debug("%s(%d, %d, %d, %d): qlm: %d, lane: %d\n", __func__, ccpi_lane,
	      tx_swing, tx_pre, tx_post, qlm, lane);
	/* Manual tx swing and tx equalization programming steps */

	/* 1) Enable tx swing and tx empasis overrides */
	lanex_tx_cfg_1.u64 = cvmx_read_csr_node(node,
						CVMX_GSERX_LANEX_TX_CFG_1(lane, qlm));
	lanex_tx_cfg_1.s.tx_swing_ovrrd_en = 1;
	lanex_tx_cfg_1.s.tx_premptap_ovrrd_val = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_TX_CFG_1(lane, qlm),
			    lanex_tx_cfg_1.u64);
	/* 2) Program the Tx swing and Tx emphasis pre-cursor and post-cursor
	 *    values
	 */
	lanex_tx_cfg_0.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_TX_CFG_0(lane, qlm));
	lanex_tx_cfg_0.s.cfg_tx_swing = tx_swing;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_TX_CFG_0(lane, qlm),
			    lanex_tx_cfg_0.u64);
	/* 3) Program override for the Tx coefficient request */
	lanex_tx_pre_emp.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_TX_PRE_EMPHASIS(lane, qlm));
	lanex_tx_pre_emp.s.cfg_tx_premptap = (tx_post << 4) | tx_pre;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_TX_PRE_EMPHASIS(lane, qlm),
			    lanex_tx_pre_emp.u64);
	/* 4) Enable the Tx coefficient request override enable */
	lanex_pcs_ctlifc_0.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_0(lane, qlm));
	lanex_pcs_ctlifc_0.s.cfg_tx_coeff_req_ovrrd_val = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_0(lane, qlm),
			    lanex_pcs_ctlifc_0.u64);
	/* 5) Issue a control interface configuration override request to start
	 *    the tx equalizer optimization cycle which applies the new tx swing
	 *    and equalization settings
	 */
	lanex_pcs_ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	lanex_pcs_ctlifc_2.s.ctlifc_ovrrd_req = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    lanex_pcs_ctlifc_2.u64);
	/* 6) To perform a subsequent tx swing and tx equalization adjustment:
	 *  a) Disable the tx coefficient request override enable
	 */
	lanex_pcs_ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	lanex_pcs_ctlifc_2.s.cfg_tx_coeff_req_ovrrd_en = 0;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    lanex_pcs_ctlifc_2.u64);
	/* b) Issue a control interface configuration override request */
	lanex_pcs_ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	lanex_pcs_ctlifc_2.s.ctlifc_ovrrd_req = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    lanex_pcs_ctlifc_2.u64);
}

/**
 * The definition of a short or long channel is whether the channel loss
 * is less than or equal to 10dB. This function does the range
 * check so it is only in one spot in the CCPI code.
 *
 * @param channel_loss
 *               Channel loss in dB
 *
 * @return True for short channel, false otherwise
 */
static bool is_short_channel(int channel_loss)
{
	/* A value of -1, unknown channel loss, will be treated as short channel.
	 * Short channel is the most common CCPI setup
	 */
	debug("%s(%d)\n", __func__, channel_loss);
	return (channel_loss <= 10);
}

/**
 * Configure the channel for short or long. This function shouldn't be used
 * directly, instead ccpi_channel_loss should be called with the
 * proper channel loss. Due to limited space in training
 * messages, the secondary node uses this function directly.
 *
 * @param ccpi_lane Lane to configure
 * @param short_channel
 *                  True if the channel is short, False if long
 */
static void ccpi_channel_length(int ccpi_lane, bool short_channel)
{
	int qlm = ccpi_lane / 4 + 8;
	int lane = ccpi_lane & 3;
	union cvmx_gserx_lane_vma_fine_ctrl_2 vma_fine_ctrl_2;

	debug("%s(%d, %s)\n", __func__, ccpi_lane, short_channel ? "true" : "false");
	/* The gain setting can only be set for an entire QLM instead of
	 *       per lane. Only set it for lane 0 and ignore lanes 1-3
	 */
	if (lane == 0) {
		vma_fine_ctrl_2.u64 =
			cvmx_read_csr_node(node,
					   CVMX_GSERX_LANE_VMA_FINE_CTRL_2(qlm));
		vma_fine_ctrl_2.s.rx_prectle_gain_min_fine =
						short_channel ? 0x0 : 0x6;
		cvmx_write_csr_node(node, CVMX_GSERX_LANE_VMA_FINE_CTRL_2(qlm),
				    vma_fine_ctrl_2.u64);
	}
}

/**
 * Given a channel loss in dB, configure the ccpi lane
 *
 * @param ccpi_lane Lane to configure
 * @param channel_loss
 *                  Channel loss in dB
 */
static void ccpi_channel_loss(int ccpi_lane, int channel_loss)
{
	ccpi_channel_length(ccpi_lane, is_short_channel(channel_loss));
}

static int64_t ccpi_extract_smag(uint64_t v, int lsb, int msb)
{
	int64_t r;
	r = v >> lsb;
	r &= ((1ll << (msb - lsb)) - 1);
	if (v & (1ull << msb))
		r = -r;
	return r;
}

static void ccpi_display_tuning_state(int ccpi_lane)
{
	const char dir_label[4][5] = {"Hold", "Inc", "Dec", "Hold"};
	int qlm = 8 + ccpi_lane / 4;
	int lane = ccpi_lane % 4;

	uint64_t rx_aeq_out_0 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_AEQ_OUT_0(lane, qlm));
	uint64_t rx_aeq_out_1 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_AEQ_OUT_1(lane, qlm));
	uint64_t rx_aeq_out_2 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_AEQ_OUT_2(lane, qlm));
	uint64_t rx_vma_status_0 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_VMA_STATUS_0(lane, qlm));
	uint64_t rx_vma_status_1 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_RX_VMA_STATUS_1(lane, qlm));
	uint64_t sds_pin_mon_1 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_SDS_PIN_MON_1(lane, qlm));
	uint64_t sds_pin_mon_2 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_SDS_PIN_MON_2(lane, qlm));
	uint64_t br_rxx_eer = cvmx_read_csr_node(node, CVMX_GSERX_BR_RXX_EER(lane, qlm));
	union cvmx_ocx_qlmx_cfg qlmx_cfg;

	qlmx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(qlm - 8));

	printf("Lane %2d:\n", ccpi_lane);
	printf("    DFE Tap 1: %2llu, Tap 2: %2lld, Tap 3: %2lld, Tap 4: %2lld, Tap 5: %2lld\n",
	       (unsigned long long)(rx_aeq_out_1 & 0x1f),
	       (unsigned long long)ccpi_extract_smag(rx_aeq_out_1, 5, 9),
	       (unsigned long long)ccpi_extract_smag(rx_aeq_out_1, 10, 14),
	       (unsigned long long)ccpi_extract_smag(rx_aeq_out_0, 0, 4),
	       (unsigned long long)ccpi_extract_smag(rx_aeq_out_0, 5, 9));
	printf("    Pre-CTLE Gain: %2llu, Post-CTLE Gain: %2llu, CTLE Peak: %2llu, CTLE Pole: %2llu\n",
	       (unsigned long long)(rx_aeq_out_2 >> 4) & 0xf,
	       (unsigned long long)(rx_aeq_out_2 & 0xf),
	       (unsigned long long)((rx_vma_status_0 >> 2) & 0x1f),
	       (unsigned long long)(rx_vma_status_0 & 3));
	printf("    TX Swing: %2llu, Pre-emphasis Pre-cursor: %2llu, Post-cursor: %2llu\n",
	       (unsigned long long)((sds_pin_mon_1 >> 1) & 0x1f),
	       (unsigned long long)(sds_pin_mon_2 & 0xf),
	       (unsigned long long)((sds_pin_mon_2 >> 4) & 0x1f));
	printf("    TX Boost Enable: %2llu, TX Turbo Mode: %2llu\n",
	       (unsigned long long)((sds_pin_mon_2 >> 10) & 1),
	       (unsigned long long)((sds_pin_mon_2 >> 9) & 1));
	printf("    RX Equalization Tx Directions Hints TXPRE: %s, TXMAIN: %s, TXPOST: %s, Figure of Merit: %2llu\n",
	       dir_label[br_rxx_eer & 3],
	       dir_label[(br_rxx_eer >> 2) & 3],
	       dir_label[(br_rxx_eer >> 4) & 3],
	       (unsigned long long)((br_rxx_eer >> 6) & 0xff));
	printf("    Training-done: %llu, Ready: %d, Bad: %d\n",
	       (unsigned long long)(rx_vma_status_1 >> 7) & 1,
	       (qlmx_cfg.s.ser_lane_ready & (1 << lane) ? 1 : 0),
	       (qlmx_cfg.s.ser_lane_bad & (1 << lane) ? 1 : 0));

}

static void ccpi_display_tuning_state_all(void)
{
	int ccpi_lane;

	for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++)
		ccpi_display_tuning_state(ccpi_lane);
}

static void ccpi_display_state(void)
{
	static int count = 0;
	int ccpi_lane;
	int request;

	if ((count & 0xf) == 0) {
		for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++)
			printf(" [%2u]", ccpi_lane);
		printf("\n");
	}
	count++;
	for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
		switch (lane_state[ccpi_lane].lane_state) {
		case STATE_START:
			printf(" Strt");
			break;
		case STATE_WAIT_EIE:
			printf(" Weie");
			break;
		case STATE_WAIT_CDR:
			printf(" Wcdr");
			break;
		case STATE_WAIT_TRAIN_PRESET:
			printf(" W1st");
			break;
		case STATE_MESSAGE_CHECK:
			request = lane_state[ccpi_lane].ld_cu.u16 & TRAIN_DATA_MASK;
			printf(" Ck%2u", request);
			break;
		case STATE_TRAINING_INIT:
			printf(" Init");
			break;
		case STATE_TRAINING_IDLE:
			printf(" Idle");
			break;
		case STATE_TRAINING_UPDATE:
			printf(" Upd ");
			break;
		case STATE_TRAINING_REQ:
			printf(" Req ");
			break;
		case STATE_TRAINING_REQ_UPDATE:
			printf(" R+U ");
			break;
		case STATE_WAIT_FOR_READY:
			printf(" Wrdy");
			break;
		case STATE_READY:
			printf(" Rdy ");
			break;
		}
	}
	printf("\n");
}

int ccpi_qlm_errata_gser_27140(int node, int qlm, int baud_mhz, int channel_loss)
{
	union cvmx_gserx_lane_vma_fine_ctrl_0 vma_fine_ctl_0;
	union cvmx_gserx_lane_vma_fine_ctrl_2 vma_fine_ctl_2;
	union cvmx_gserx_rx_txdir_ctrl_0 rx_txdir_ctrl_0;
	union cvmx_gserx_rx_txdir_ctrl_1 rx_txdir_ctrl_1;
	union cvmx_gserx_rx_txdir_ctrl_2 rx_txdir_ctrl_2;
	union cvmx_gserx_lanex_rx_valbbd_ctrl_0 rx_valbbd_ctrl_0;
	union cvmx_gserx_eq_wait_time eq_wait_time;
	int num_lanes = 4;
	int lane;
	/* A channel loss of -1 means the loss is unknown. A short channel is
	 * considered to have loss between 0 and 10 dB
	 */

	bool short_channel = (channel_loss > 0) && (channel_loss <= 10);

	debug("%s(%d, %d, %d, %d)\n", __func__, node, qlm, baud_mhz, channel_loss);
	if (baud_mhz != 10312)
		return 0;

	/* I. For each GSER QLM: */
	/* Workaround GSER-27140: */
	/* (1) GSER-26150 = Applied by the caller */
	/* (2) Write GSER()_LANE_VMA_FINE_CTRL_0[RX_SDLL_IQ_MAX_FINE] = 0xE */
	/* (3) Write GSER()_LANE_VMA_FINE_CTRL_0[RX_SDLL_IQ_MIN_FINE] = 0x8 */
	/* (4) Write GSER()_LANE_VMA_FINE_CTRL_0[RX_SDLL_IQ_STEP_FINE] = 0x2 */
	/* (5) Write GSER()_LANE_VMA_FINE_CTRL_0[VMA_WINDOW_WAIT_FINE] = 0x5 */
	/* (6) Write GSER()_LANE_VMA_FINE_CTRL_0[LMS_WAIT_TIME_FINE] = 0x5 */
	vma_fine_ctl_0.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANE_VMA_FINE_CTRL_0(qlm));
	vma_fine_ctl_0.s.rx_sdll_iq_max_fine = 0xE;
	vma_fine_ctl_0.s.rx_sdll_iq_min_fine = 0x8;
	vma_fine_ctl_0.s.rx_sdll_iq_step_fine = 0x2;
	vma_fine_ctl_0.s.vma_window_wait_fine = 0x5;
	vma_fine_ctl_0.s.lms_wait_time_fine = 0x5;
	cvmx_write_csr_node(node, CVMX_GSERX_LANE_VMA_FINE_CTRL_0(qlm),
			    vma_fine_ctl_0.u64);

	/* (7) Write GSER()_LANE_VMA_FINE_CTRL_2[RX_PRECTLE_GAIN_MAX_FINE] = 0xB */
	/* (8) Write GSER()_LANE_VMA_FINE_CTRL_2[RX_PRECTLE_GAIN_MIN_FINE] =
	 *	 0x6(long) or 0x0(short)
	 */
	vma_fine_ctl_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANE_VMA_FINE_CTRL_2(qlm));
	vma_fine_ctl_2.s.rx_prectle_gain_max_fine = 0xB;
	vma_fine_ctl_2.s.rx_prectle_gain_min_fine = short_channel ? 0x0 : 0x6;
	cvmx_write_csr_node(node, CVMX_GSERX_LANE_VMA_FINE_CTRL_2(qlm),
			    vma_fine_ctl_2.u64);
	/* (9) Write GSER()_RX_TXDIR_CTRL_0[RX_BOOST_LO_THRES] = 0x4 */
	/* (10) Write GSER()_RX_TXDIR_CTRL_0[RX_BOOST_HI_THRES] = 0xB */
	/* (11) Write GSER()_RX_TXDIR_CTRL_0[RX_BOOST_HI_VAL] = 0xF */
	rx_txdir_ctrl_0.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_0(qlm));
	rx_txdir_ctrl_0.s.rx_boost_lo_thrs = 0x4;
	rx_txdir_ctrl_0.s.rx_boost_hi_thrs = 0xB;
	rx_txdir_ctrl_0.s.rx_boost_hi_val = 0xF;
	cvmx_write_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_0(qlm),
			    rx_txdir_ctrl_0.u64);
	/* (12) Write GSER()_RX_TXDIR_CTRL_1[RX_TAP1_LO_THRS] = 0x8 */
	/* (13) Write GSER()_RX_TXDIR_CTRL_1[RX_TAP1_HI_THRS] = 0x17 */
	rx_txdir_ctrl_1.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_1(qlm));
	rx_txdir_ctrl_1.s.rx_tap1_lo_thrs = 0x8;
	rx_txdir_ctrl_1.s.rx_tap1_hi_thrs = 0x17;
	cvmx_write_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_1(qlm),
			    rx_txdir_ctrl_1.u64);
	/* (14) Write GSER()_EQ_WAIT_TIME[RXEQ_WAIT_CNT] = 0x6 */
	eq_wait_time.u64 = cvmx_read_csr_node(node, CVMX_GSERX_EQ_WAIT_TIME(qlm));
	eq_wait_time.s.rxeq_wait_cnt = 0x6;
	cvmx_write_csr_node(node, CVMX_GSERX_EQ_WAIT_TIME(qlm), eq_wait_time.u64);
	/* (15) Write GSER()_RX_TXDIR_CTRL_2[RX_PRECORR_HI_THRS] = 0xC0 */
	/* (16) Write GSER()_RX_TXDIR_CTRL_2[RX_PRECORR_LO_THRS] = 0x40 */
	rx_txdir_ctrl_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_2(qlm));
	rx_txdir_ctrl_2.s.rx_precorr_hi_thrs = 0xC0;
	rx_txdir_ctrl_2.s.rx_precorr_lo_thrs = 0x40;
	cvmx_write_csr_node(node, CVMX_GSERX_RX_TXDIR_CTRL_2(qlm),
			    rx_txdir_ctrl_2.u64);

	/* II. For each GSER QLM SerDes lane: */
	/* Establish typical values, which are already reset values in pass 2: */
	for (lane = 0; lane < num_lanes; lane++) {
		/* (17) For each GSER lane in the 10GBASE-KR link: */
		/*    (a) Write GSER()_LANE()_RX_VALBBD_CTRL_0[AGC_GAIN] = 0x3 */
		/*    (b) Write GSER()_LANE()_RX_VALBBD_CTRL_0[DFE_GAIN] = 0x2 */
		rx_valbbd_ctrl_0.u64 =
			cvmx_read_csr_node(node,
					   CVMX_GSERX_LANEX_RX_VALBBD_CTRL_0(lane, qlm));
		rx_valbbd_ctrl_0.s.agc_gain = 0x3;
		rx_valbbd_ctrl_0.s.dfe_gain = 0x2;
		cvmx_write_csr_node(node,
				    CVMX_GSERX_LANEX_RX_VALBBD_CTRL_0(lane, qlm),
				    rx_valbbd_ctrl_0.u64);
	}
	/* III.  The GSER QLM SerDes Lanes are now ready */
	return 0;
}

/**
 * Errata GSER-27882 -GSER 10GBASE-KR Transmit Equalizer
 * Training may not update PHY Tx Taps. This function is not static
 * so we can share it with BGX KR
 * Applies to:
 *     CN88XX pass 1.x, 2.0, 2.1
 * Fixed in hardware:
 *     CN88XX pass 2.2 and higher
 *     CN81XX
 *     CN83XX
 *
 * @param node   Node to apply errata fix for
 * @param qlm    QLM to apply errata fix to
 * @param lane
 *
 * @return Zero on success, negative on failure
 */
int ccpi_qlm_errata_gser_27882(int node, int qlm, int lane)
{
	union cvmx_gserx_lanex_pcs_ctlifc_0 ctlifc_0;
	union cvmx_gserx_lanex_pcs_ctlifc_2 ctlifc_2;

	debug("%s(%d, %d, %d)\n", __func__, node, qlm, lane);
	/* Toggle TX Coeff Req Override to force an update */
	ctlifc_0.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_0(lane, qlm));
	ctlifc_0.s.cfg_tx_coeff_req_ovrrd_val = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_0(lane, qlm),
			    ctlifc_0.u64);
	ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	ctlifc_2.s.cfg_tx_coeff_req_ovrrd_en = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    ctlifc_2.u64);
	ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	ctlifc_2.s.ctlifc_ovrrd_req = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    ctlifc_2.u64);
	ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	ctlifc_2.s.cfg_tx_coeff_req_ovrrd_en = 0;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    ctlifc_2.u64);
	ctlifc_2.u64 =
		cvmx_read_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm));
	ctlifc_2.s.ctlifc_ovrrd_req = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_PCS_CTLIFC_2(lane, qlm),
			    ctlifc_2.u64);
	return 0;
}

/**
 * Read OCX setup used to determine which QLMs connect to which links
 *
 * @param node   Node to query
 * @param link   Link to query
 *
 * @return QLM mask of CCPI QLMs being used
 */
int ccpi_get_qlm_select(int node, int link)
{
	union cvmx_ocx_qlmx_cfg qlmx_cfg2;
	union cvmx_ocx_qlmx_cfg qlmx_cfg3;
	int qlm_select = 0;

	debug("%s(%d, %d)\n", __func__, node, link);
	/* qlmx_cfg2.ser_local=1 means QLM2 is connected to link 1. */
	qlmx_cfg2.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(2));
	/* qlmx_cfg3.ser_local=1 means QLM3 is connected to link 1. */
	qlmx_cfg3.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(3));

	switch (link) {
	case 0:
		qlm_select = qlmx_cfg2.s.ser_local ? 0x3 : 0x7;
		break;
	case 1:
		qlm_select = qlmx_cfg2.s.ser_local ? 0x4 : 0x0;
		qlm_select |= qlmx_cfg3.s.ser_local ? 0x8 : 0x0;
		break;
	case 2:
		qlm_select = qlmx_cfg3.s.ser_local ? 0x30 : 0x38;
		break;
	default:
		break;
	}
	return qlm_select;
}

static void lane_change_state(int ccpi_lane, enum lane_state_e state)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	int qlm = 8 + ccpi_lane / 4;
	int lane = ccpi_lane % 4;
	union cvmx_gserx_lanex_sds_pin_mon_1 pin1;
	union cvmx_gserx_lanex_sds_pin_mon_2 pin2;


	debug("%s(%d, %d)\n", __func__, ccpi_lane, state);

	if (DEBUG_STATE) {
		char *str = NULL;

		printf("Lane %2d: (Merit %2d Main=", ccpi_lane,
		       lstate->merit);
		switch(lstate->desired_main) {
		case RXT_ESM_INC:
			printf("Incr");
			break;
		case RXT_ESM_DEC:
			printf("Decr");
			break;
		default:
			printf("Hold");
			break;
		}
		printf(" Post=");
		switch(lstate->desired_post) {
		case RXT_ESM_INC:
			printf("Incr");
			break;
		case RXT_ESM_DEC:
			printf("Decr");
			break;
		default:
			printf("Hold");
			break;
		}
		printf(" Pre=");
		switch(lstate->desired_post) {
		case RXT_ESM_INC:
			printf("Incr");
			break;
		case RXT_ESM_DEC:
			printf("Decr");
			break;
		default:
			printf("Hold");
			break;
		}
		pin1.u64 = cvmx_read_csr_node(node,
					      CVMX_GSERX_LANEX_SDS_PIN_MON_1(lane, qlm));
		pin2.u64 = cvmx_read_csr_node(node,
					      CVMX_GSERX_LANEX_SDS_PIN_MON_2(lane, qlm));
		printf(" Tuning Main=%2d, Post=%2d, Pre=%2d, Step=%2d) ",
		       pin1.s.pcs_sds_tx_swing,
		       pin2.s.pcs_sds_premptap >> 4,
		       pin2.s.pcs_sds_premptap & 0xf,
		       lstate->steps);
		switch (state) {
		case STATE_START:
			str = "Starting";
			break;
		case STATE_WAIT_EIE:
			str = "Waiting for EIE";
			break;
		case STATE_WAIT_CDR:
			str = "Waiting for CDR";
			break;
		case STATE_WAIT_TRAIN_PRESET:
			str = "Waiting for first message";
			break;
		case STATE_MESSAGE_CHECK:
			str = "Running message check";
			break;
		case STATE_TRAINING_INIT:
			str = "Training - Starting";
			break;
		case STATE_TRAINING_IDLE:
			str = "Training - Idle";
			break;
		case STATE_TRAINING_UPDATE:
			str = "Training - Updated";
			break;
		case STATE_TRAINING_REQ:
			str = "Training - Request";
			break;
		case STATE_TRAINING_REQ_UPDATE:
			str = "Training - Request, Updated";
			break;
		case STATE_WAIT_FOR_READY:
			str = "Waiting for ready";
			break;
		case STATE_READY:
			str = "Lane ready";
			break;
		default:
			str = "Unknown state";
			break;
		}
		printf("%s\n", str);
	}

	lstate->lane_state = state;
	/* Force RX ready = 0 in states before training */
	if (lstate->lane_state <= STATE_TRAINING_INIT)
		lstate->rx_ready = 0;
	/* Force RX ready = 1 in states after training */
	if (lstate->lane_state >= STATE_WAIT_FOR_READY)
		lstate->rx_ready = 1;

	/* Force message check done bit clear before its state */
	if (lstate->lane_state < STATE_MESSAGE_CHECK)
		lstate->chk_done = 0;
	/* Force message check done bit after its state */
	if (lstate->lane_state > STATE_MESSAGE_CHECK)
		lstate->chk_done = 1;
}


/**
 * Check if a lane as performed Electrical Idle Exit. Lanes are not allowed to
 * proceed until EIE.
 *
 * @param ccpi_lane Lane to check
 */
static void lane_check_eie(int ccpi_lane)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	int qlm = 8 + ccpi_lane / 4;
	union cvmx_ocx_lnex_trn_ctl ocx_lnex_trn_ctl;
	union cvmx_gserx_lane_srst srst;

	debug("%s(%d)\n", __func__, ccpi_lane);
	ocx_lnex_trn_ctl.u64 =
		cvmx_read_csr_node(node, CVMX_OCX_LNEX_TRN_CTL(ccpi_lane));
	if (ocx_lnex_trn_ctl.s.eie_detect) {
		/* If we're waiting for EIE move to the next state */
		if (lstate->lane_state == STATE_WAIT_EIE)
			lane_change_state(ccpi_lane, STATE_WAIT_CDR);
	} else {
		/* Return to the waiting for EIE state */
		if (lstate->lane_state != STATE_WAIT_EIE)
			lane_change_state(ccpi_lane, STATE_WAIT_EIE);
	}
	srst.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANE_SRST(qlm));
	if (srst.s.lsrst)
		cvmx_write_csr_node(node, CVMX_GSERX_LANE_SRST(qlm), 0);
}

/**
 * Check that CDR has occurred for a lane. Lanes are not allowed to continue
 * training until CDR.
 *
 * @param ccpi_lane Lane to check
 */
static void lane_check_cdr(int ccpi_lane)
{
	union cvmx_ocx_qlmx_cfg ocx_qlmx_cfg;
	union cvmx_gserx_rx_eie_detsts eie_detsts;
	struct lane_state *lstate = &lane_state[ccpi_lane];
	int qlm = 8 + ccpi_lane / 4;
	int lane = ccpi_lane & 3;

	debug("%s(%d): state: %d\n", __func__, ccpi_lane, lstate->lane_state);
	if (lstate->lane_state < STATE_WAIT_CDR)
		return;

	eie_detsts.u64 = cvmx_read_csr_node(node, CVMX_GSERX_RX_EIE_DETSTS(qlm));
	if ((1 << lane) & eie_detsts.s.cdrlock) {
		/* If we're waiting for CDR move to the next state */
		if (lstate->lane_state == STATE_WAIT_CDR) {
			ocx_qlmx_cfg.u64 =
				cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(qlm - 8));
			if (ocx_qlmx_cfg.s.trn_ena &&
			    ocx_qlmx_cfg.s.trn_rxeq_only) {
				/* RX equalization only */
				lstate->steps = 0;
				if (DEBUG_TUNING_CHANGES)
					printf("Lane %2d: CDR is locked, RX equalization only\n", ccpi_lane);
				lane_change_state(ccpi_lane, STATE_WAIT_TRAIN_PRESET);
			} else if (ocx_qlmx_cfg.s.trn_ena) {
				/* KR Training */
				if (DEBUG_TUNING_CHANGES)
					printf("Lane %2d: CDR is locked, start KR training\n", ccpi_lane);
				lane_change_state(ccpi_lane, STATE_WAIT_TRAIN_PRESET);
			} else {
				/* No Training, jump directly to wait for lane ready */
				if (DEBUG_TUNING_CHANGES)
					printf("Lane %2d: CDR is locked, no training\n", ccpi_lane);
				lstate->steps = 0;
				lane_change_state(ccpi_lane, STATE_WAIT_FOR_READY);
			}
		}
	} else {
		/* Return to the waiting for CDR state */
		if (lstate->lane_state != STATE_WAIT_CDR)
			lane_change_state(ccpi_lane, STATE_WAIT_CDR);
	}
}

/**
 * Check that training message correctly deliver payload
 *
 * Before starting training, we check that the message protocol is working
 * by sending a sequence of incrementing numbers and verifying we get the same
 * number back. This is unrelated to the actual training, but proves training
 * message can send/receive without error.
 *
 * @param ccpi_lane Lane to check
 * @param is_master True if this node is the master
 */
static void lane_check_messaging(int ccpi_lane, bool is_master)
{
	union cvmx_ocx_lnex_trn_ctl trn_ctl;
	struct lane_state *lstate = &lane_state[ccpi_lane];
	int qlm = 8 + ccpi_lane / 4;
	int request;
	int response;
	int l;
	int finished;

	debug("%s(%d, %s)\n", __func__, ccpi_lane, is_master ? "true" : "false");
	if (lstate->lane_state <= STATE_WAIT_TRAIN_PRESET) {
		lstate->ld_cu.u16 = 0x3f;
		lstate->ld_cu.s.preset = 1;
		lstate->ld_sr.u16 = 0;
	}
	if (lstate->lane_state == STATE_WAIT_TRAIN_PRESET) {
		trn_ctl.u64 = cvmx_read_csr_node(node, CVMX_OCX_LNEX_TRN_CTL(ccpi_lane));
		lstate->steps = 0;
		if (trn_ctl.s.done) {
			if (DEBUG_TRAIN_MESSAGES)
				printf("Lane %2u: Clearing training done\n",
				       ccpi_lane);
			cvmx_write_csr_node(node, CVMX_OCX_LNEX_STATUS(ccpi_lane), 0);
			trn_ctl.s.done = 0;
			cvmx_write_csr_node(node, CVMX_OCX_LNEX_TRN_CTL(ccpi_lane),
					    trn_ctl.u64);
		}
		if (lstate->lp_cu.s.preset)
			lane_change_state(ccpi_lane, STATE_MESSAGE_CHECK);
	}
	if (lstate->lane_state == STATE_MESSAGE_CHECK) {
		/* Respond to the current request */
		lstate->ld_sr.u16 = lstate->lp_cu.u16 & TRAIN_DATA_MASK;

		/* Check the response we got */
		request = lstate->ld_cu.u16 & TRAIN_DATA_MASK;
		response = lstate->lp_sr.u16 & TRAIN_DATA_MASK;
		if (response == request) {
			if (DEBUG_TRAIN_MESSAGES)
				printf("Lane %2u: Message %2u was ACKed\n",
				       ccpi_lane, request);
			if (request == 0) {
				lstate->chk_done = 1;
				finished = 1;
				for (l = 0; l < CCPI_LANES; l++)
					finished &= lane_state[l].chk_done;
				if (finished)
					lane_change_state(ccpi_lane,
							  STATE_TRAINING_INIT);
			} else {
				lstate->ld_cu.u16 -= 3;
			}
		} else if (response - 3 == request) {
			/* Partner is one behind, assume we just haven't gotten
			 * the update yet. Continue in current state
			 */
			lstate->chk_done = 0;
		} else {
			/* Wrong message, start over */
			lstate->ld_cu.u16 = 0x3f;
			lstate->ld_cu.s.preset = 1;
			lstate->chk_done = 0;
			if (request == 63) {
				/* Check if most other lanes are past this step.
				 * If so, this lane is stuck waiting for
				 * something that won't happen.
				 */
				finished = 0;
				for (l = 0; l < CCPI_LANES; l++)
					finished += lane_state[l].chk_done;
				if (finished >= 21) {
					printf("Lane %2d: Training stuck, resetting QLM\n",
					       ccpi_lane);
					cvmx_write_csr_node(node,
							    CVMX_GSERX_LANE_SRST(qlm), 1);
					mdelay(10);
#if 0
					for (l = 0; l < CCPI_LANES; l++)
						lane_state[((qlm - 8) * 4) + l].lane_state = STATE_TRAINING_INIT;
#endif
				}
			}
		}
	}
}

/**
 * Handle a training request from the link partner
 *
 * This function is called periodically to update lane tuning based on training
 * requests from the partner. It applies the settings and responds to the link
 * partner using the SR message.
 *
 * @param ccpi_lane Lane to handle
 */
static void lane_handle_training_request(int ccpi_lane)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	const union cvmx_bgx_spu_br_train_cup_s lp_cu = lstate->lp_cu;
	union cvmx_bgx_spu_br_train_rep_s ld_sr = lstate->ld_sr;
	enum lane_state_e new_state = lstate->lane_state;
	int finished;
	int l;
	bool have_request = (lp_cu.u16 != 0); /* True if we have a request from the partner */
	bool lane_stable;

	debug("%s(%d) have request: %s\n", __func__, ccpi_lane,
	      have_request ? "true" : "false");
	if ((lstate->lane_state == STATE_TRAINING_UPDATE) ||
	    (lstate->lane_state == STATE_TRAINING_REQ_UPDATE)) {
		/* We did a coef update and responded to the link partner.
		 * Wait until the link partner acks getting our response
		 * before clearing it.
		 * Response should have hold(0) for update stats (last 6 bits)
		 */
		if (!have_request) {
			/* Clear SR now that the LP has seen it */
			ld_sr.s.main_cst = NOT_UPDATED;
			ld_sr.s.post_cst = NOT_UPDATED;
			ld_sr.s.pre_cst = NOT_UPDATED;
			/* Back to training state as update is complete */
			new_state &= STATE_TRAINING_REQ;
		}
	} else if (have_request) {	/* We got a new request */
		if (lp_cu.s.init && !(lp_cu.u16 & TRAIN_DATA_MASK)) {
			lstate->tx_pre = lstate->init_pre;
			lstate->tx_main = lstate->init_main;
			lstate->tx_post = lstate->init_post;
			ld_sr.s.main_cst = UPDATED;
			ld_sr.s.post_cst = UPDATED;
			ld_sr.s.pre_cst = UPDATED;
			if (DEBUG_PARAMETERS)
				printf("Received INIT\n");
		}

		if (lp_cu.s.preset) {
			if (DEBUG_TRAIN_MESSAGES)
				printf("Received PRESET, restarting training\n");
			lane_change_state(ccpi_lane, STATE_WAIT_TRAIN_PRESET);
		}
		/* INIT set and direction hints is illegal in normal training. We
		 *           use it as a special message to set the init values from the
		 *           remote node. 1-31=tx_swing, 32-47=tx_post, 48-58=tx_pre,
		 *           59-63=reserved */
		if (lp_cu.s.init && (lp_cu.u16 & TRAIN_DATA_MASK)) {
			int v = lp_cu.u16 & TRAIN_DATA_MASK;
			if (v < 32) { /* Request to set init swing */
				lstate->init_main = v - 1;
				ld_sr.s.main_cst = UPDATED;
				ld_sr.s.post_cst = UPDATED;
				ld_sr.s.pre_cst = UPDATED;
				if (DEBUG_PARAMETERS)
					printf("Received param tx_swing=%2u\n",
					       lstate->init_main);
			} else if (v < 48) {	/* Request to set init post */
				lstate->init_post = v - 32;
				ld_sr.s.main_cst = UPDATED;
				ld_sr.s.post_cst = UPDATED;
				ld_sr.s.pre_cst = UPDATED;
				if (DEBUG_PARAMETERS)
					printf("Received param tx_post=%2u\n",
					       lstate->init_post);
			} else if (v < 59) {	/* Request to set init pre */
				lstate->init_pre = v - 48;
				ld_sr.s.main_cst = UPDATED;
				ld_sr.s.post_cst = UPDATED;
				ld_sr.s.pre_cst = UPDATED;
				if (DEBUG_PARAMETERS)
					printf("Received param tx_pre=%2u\n",
					       lstate->init_pre);
			} else if (v == 59) {	/* Request to set long channel */
				ccpi_channel_length(ccpi_lane, false);
				ld_sr.s.main_cst = UPDATED;
				ld_sr.s.post_cst = UPDATED;
				ld_sr.s.pre_cst = UPDATED;
				if (DEBUG_PARAMETERS)
					printf("Received param channel=long\n");
			} else if (v == 60) {/* Request to set short channel */
				ccpi_channel_length(ccpi_lane, true);
				ld_sr.s.main_cst = UPDATED;
				ld_sr.s.post_cst = UPDATED;
				ld_sr.s.pre_cst = UPDATED;
				if (DEBUG_PARAMETERS)
					printf("Received param channel=short\n");
			}
		}

		if (lp_cu.s.main_cup && !lp_cu.s.init) {
			/* 0=max amplitude, 25=min amplitude */
			if (lp_cu.s.main_cup == INCREMENT) {
				if (is_tuning_valid(lstate->tx_main - 1,
						    lstate->tx_pre,
						    lstate->tx_post)) {
					lstate->tx_main--;
					ld_sr.s.main_cst = UPDATED;
				} else {
					ld_sr.s.main_cst = MAXIMUM;
				}
			} else if (lp_cu.s.main_cup == DECREMENT) {
				if (is_tuning_valid(lstate->tx_main + 1,
						    lstate->tx_pre,
						    lstate->tx_post)) {
					lstate->tx_main++;
					ld_sr.s.main_cst = UPDATED;
				} else {
					ld_sr.s.main_cst = MINIMUM;
				}
			}
		}

		if (lp_cu.s.post_cup && !lp_cu.s.init) {
			/* 0=max post emphasis, 15=min post emphasis */
			if (lp_cu.s.post_cup == INCREMENT) {
				if (is_tuning_valid(lstate->tx_main,
						    lstate->tx_pre,
						    lstate->tx_post - 1)) {
					lstate->tx_post--;
					ld_sr.s.post_cst = UPDATED;
				} else {
					ld_sr.s.post_cst = MAXIMUM;
				}
			} else if (lp_cu.s.post_cup == DECREMENT) {
				if (is_tuning_valid(lstate->tx_main,
						    lstate->tx_pre,
						    lstate->tx_post + 1)) {
					lstate->tx_post++;
					ld_sr.s.post_cst = UPDATED;
				} else {
					ld_sr.s.post_cst = MINIMUM;
				}
			}
		}

		if (lp_cu.s.pre_cup && !lp_cu.s.init) {
			/* 0=max pre emphasis, 10=min pre emphasis */
			if (lp_cu.s.pre_cup == INCREMENT) {
				if (is_tuning_valid(lstate->tx_main,
						    lstate->tx_pre - 1,
						    lstate->tx_post)) {
					lstate->tx_pre--;
					ld_sr.s.pre_cst = UPDATED;
				} else {
					ld_sr.s.pre_cst = MAXIMUM;
				}
			} else if (lp_cu.s.pre_cup == DECREMENT) {
				if (is_tuning_valid(lstate->tx_main,
						    lstate->tx_pre + 1,
						    lstate->tx_post)) {
					lstate->tx_pre++;
					ld_sr.s.pre_cst = UPDATED;
				} else {
					ld_sr.s.pre_cst = MINIMUM;
				}
			}
		}

		debug("%s: Lane: %d: Set main: %d, post: %d, pre: %d\n",
		      __func__, ccpi_lane, lstate->tx_main, lstate->tx_post,
		      lstate->tx_pre);
		if (DEBUG_TUNING_CHANGES)
			printf("Lane %2u: Set Main=%2u, Post=%2u, Pre=%2u\n",
			       ccpi_lane, lstate->tx_main, lstate->tx_post,
			       lstate->tx_pre);
		ccpi_tx_tune(ccpi_lane, lstate->tx_main, lstate->tx_pre,
			     lstate->tx_post);
		new_state |= STATE_TRAINING_UPDATE;
	}

	/* Lane is considered stable when RX equalization is requesting all
	 * holds and we don't have a pending request
	 */
	lane_stable =  (lstate->desired_main == RXT_ESM_HOLD);
	lane_stable &= (lstate->desired_post == RXT_ESM_HOLD);
	lane_stable &= (lstate->desired_pre  == RXT_ESM_HOLD);
	if (lstate->steps >= CCPI_MAX_STEPS)
		lane_stable = true;
	lane_stable &= ((lstate->ld_cu.u16 & TRAIN_DATA_MASK) == 0);
	lane_stable &= (lstate->steps >= 4);
	lstate->rx_ready = lane_stable;
	if (DEBUG_ONE_LANE_AT_AT_TIME) {
		ld_sr.s.rx_ready |= lstate->rx_ready;
	} else {
		finished = 1;
		for (l = 0; l < CCPI_LANES; l++)
			finished &= lane_state[l].rx_ready;
		ld_sr.s.rx_ready |= finished;
	}

	lstate->ld_sr = ld_sr;

	if (lstate->lane_state != new_state)
		lane_change_state(ccpi_lane, new_state);
}

static union cvmx_bgx_spu_br_train_cup_s lane_do_param_request(int ccpi_lane)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	union cvmx_bgx_spu_br_train_cup_s ld_cu = {.u16 =0};
	int tx_swing;
	int tx_premptap;
	int channel_loss;
	bool short_channel;

	debug("%s(%d)\n", __func__, ccpi_lane);
	if (lstate->steps == 0) {	/* Send TX Swing */
		ld_cu.u16 = lstate->init_main;
		tx_swing = CONFIG_OCTEON_CCPI_TX_SWING_NO;
		if (tx_swing != -1)
			ld_cu.u16 = tx_swing;
		if (DEBUG_PARAMETERS)
			printf("Sent param tx_swing=%u\n", ld_cu.u16);
		ld_cu.u16 += 1;		/* Encode as 1-31 */
		ld_cu.s.init = 1;
	} else if (lstate->steps == 1) {
		ld_cu.u16 = lstate->init_post;
		tx_premptap = CONFIG_OCTEON_CCPI_TX_PREEMP_TAP_NO;
		if (tx_premptap != -1)
			ld_cu.u16 = tx_premptap >> 4;
		if (DEBUG_PARAMETERS)
			printf("Sent param tx_post=%u\n", ld_cu.u16);
		ld_cu.u16 += 32;	/* Encode as 32 - 47 */
		ld_cu.s.init = 1;
	} else if (lstate->steps == 2) {	/* Send TX Pre */
		ld_cu.u16 = lstate->tx_pre;
		tx_premptap = CONFIG_OCTEON_CCPI_TX_PREEMP_TAP_NO;
		if (tx_premptap != -1)
			ld_cu.u16 = tx_premptap & 0xf;
		if (DEBUG_PARAMETERS)
			printf("Sent param tx_pre=%u\n", ld_cu.u16);
		ld_cu.u16 += 48;		/* Encode as 48 - 58 */
		ld_cu.s.init = 1;
	} else if (lstate->steps == 3) {	/* Set Channel Length */
		channel_loss = CONFIG_OCTEON_CCPI_CHANNEL_LOSS;
		short_channel = is_short_channel(channel_loss);
		if (short_channel)
			ld_cu.u16 = 60;	/* Short channel */
		else
			ld_cu.u16 = 59;	/* Long channel */
			if (DEBUG_PARAMETERS)
				printf("Sent channel=%s\n",
				       short_channel ? "short" : "long");
		ld_cu.s.init = 1;
	} else if (lstate->steps == 4) {	/* Send INIT */
		ld_cu.u16 = 0;
		ld_cu.s.init = 1;
		if (DEBUG_PARAMETERS)
			printf("Sent INIT\n");
	}
	debug("%s: return: 0x%x\n", __func__, ld_cu.u16);
	return ld_cu;
}

/**
 * Perform link training for the local device
 *
 * This function looks at the RX equalization results and sends training
 * requests to the link partner. It doesn't update the local SERDES, just
 * asks the link partner to make updates
 *
 * @param ccpi_lane Lane running training
 * @param is_master True if this node is the master
 */
static void lane_do_training(int ccpi_lane, bool is_master)
{
	struct lane_state *lstate = &lane_state[ccpi_lane];
	const union cvmx_bgx_spu_br_train_rep_s lp_sr = lstate->lp_sr;
	union cvmx_bgx_spu_br_train_cup_s ld_cu = lstate->ld_cu;
	bool have_request = (ld_cu.u16 != 0); /* True if we have a request to the partner that isn't ACKed */
	bool have_response = (lp_sr.u16 & TRAIN_DATA_MASK) != 0; /* True if the partner sent a response (ACK) */

	debug("%s(%d, %s): have request: %s, have response: %s\n",
	      __func__, ccpi_lane, is_master ? "true" : "false",
	      have_request ? "true" : "false", have_response ? "true" : "false");

	if (have_response) {
		if (have_request) {
			ld_cu.s.init = 0;
			ld_cu.s.preset = 0;
			ld_cu.s.main_cup = HOLD;
			ld_cu.s.post_cup = HOLD;
			ld_cu.s.pre_cup = HOLD;
			lstate->ld_cu = ld_cu;
			lane_change_state(ccpi_lane,
					  lstate->lane_state & STATE_TRAINING_UPDATE);
		}
		return;
	}

	if (have_request || (lstate->steps >= CCPI_MAX_STEPS))
		return;

	if ((lstate->steps < 5) && is_master)
		ld_cu = lane_do_param_request(ccpi_lane);
	else if (lstate->ld_sr.s.rx_ready)
		ld_cu.u16 = 0; /* Don't allow any more requests if we already said we're ready */
	else if (lstate->desired_main == RXT_ESM_INC)
		ld_cu.s.main_cup = INCREMENT;
	else if (lstate->desired_main == RXT_ESM_DEC)
		ld_cu.s.main_cup = DECREMENT;
	else if (lstate->desired_post == RXT_ESM_INC)
		ld_cu.s.post_cup = INCREMENT;
	else if (lstate->desired_post == RXT_ESM_DEC)
		ld_cu.s.post_cup = DECREMENT;
	else if (lstate->desired_pre == RXT_ESM_INC)
		ld_cu.s.pre_cup = INCREMENT;
	else if (lstate->desired_pre == RXT_ESM_DEC)
		ld_cu.s.pre_cup = DECREMENT;

	lstate->ld_cu = ld_cu;
	lstate->steps++; /* Increment number of request steps */
	if (ld_cu.u16)
		lane_change_state(ccpi_lane,
				  lstate->lane_state | STATE_TRAINING_REQ);
}

/**
 * Handles the state machine for KR training
 *
 * @param ccpi_lane Lane to train
 * @param is_master True if this node is the master
 */
static void lane_check_training(int ccpi_lane, bool is_master)
{
	union cvmx_ocx_lnex_trn_ctl trn_ctl;
	struct lane_state *lstate = &lane_state[ccpi_lane];

	debug("%s(%d, %s)\n", __func__, ccpi_lane, is_master ? "true" : "false");
	if (lstate->lane_state == STATE_TRAINING_INIT) {
		/* Show link partner we are in INIT */
		lstate->ld_cu.u16 = 0;
		lstate->ld_cu.s.init = 1;
		lane_change_state(ccpi_lane, STATE_TRAINING_REQ);
		return;
	}

	if ((lstate->lane_state < STATE_TRAINING_IDLE) ||
	    (lstate->lane_state > STATE_TRAINING_REQ_UPDATE))
		return;

	if (lstate->lp_cu.s.preset) {
		/* Respond to the current request, other guy is still in STATE_MESSAGE_CHECK */
		lstate->ld_sr.u16 = lstate->lp_cu.u16 & TRAIN_DATA_MASK;
	} else {
		/* Handle link partner request */
		lane_handle_training_request(ccpi_lane);

		/* Make request to link partner for better tuning */
		lane_do_training(ccpi_lane, is_master);

		trn_ctl.u64 =
			cvmx_read_csr_node(node,
					   CVMX_OCX_LNEX_TRN_CTL(ccpi_lane));
		if (trn_ctl.s.done) {
			lstate->steps = 0;
			lane_change_state(ccpi_lane, STATE_WAIT_FOR_READY);
		}
	}
}

/**
 * Checks if lane is ready for use
 *
 * After a lane finishes training, we wait for CCPI to make the lane good and
 * ready for use. If it is marked bad, we clear the flag and resart the lane.
 *
 * @param ccpi_lane Lane to check
 */
static void lane_check_ready(int ccpi_lane)
{
	union cvmx_ocx_qlmx_cfg qlmx_cfg;
	struct lane_state *lstate = &lane_state[ccpi_lane];
	int ccpi_qlm = ccpi_lane / 4;
	int lane = ccpi_lane & 3;
	bool is_ready;

	debug("%s(%d)\n", __func__, ccpi_lane);
	/* Don't check for ready until all other training is complete */
	if (lstate->lane_state < STATE_WAIT_FOR_READY)
		return;

	/* Lane is ready if SER_LANE_READY is set and SER_LANE_BAD is clear */
	qlmx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(ccpi_qlm));
	is_ready = ((qlmx_cfg.s.ser_lane_ready & (1 << lane)) &&
		    !(qlmx_cfg.s.ser_lane_bad & (1 << lane)));

	if (is_ready) {
		/* Count how many times we are ready in row. Limit to 5 */
		if (lstate->steps < 5)
			lstate->steps++;
		/* Transition to ready if we've been ready 10 loops in a row */
		if ((lstate->steps >= 5) && (lstate->lane_state != STATE_READY))
			lane_change_state(ccpi_lane, STATE_READY);
	} else {
		/* Not ready, retry. In rare cases it seems necessary to toggle
		 *           SER_LANE_BAD to restart a lane, which is why there
		 *           is a write/read/write sequence below
		 */
		qlmx_cfg.s.ser_lane_ready = 0xf;
		qlmx_cfg.s.ser_lane_bad |= 1 << lane;
		cvmx_write_csr_node(node, CVMX_OCX_QLMX_CFG(ccpi_qlm),
				    qlmx_cfg.u64);
		cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(ccpi_qlm));
		mdelay(10); /* 1ms proved to be too short in rare conditions */
		qlmx_cfg.s.ser_lane_bad = 0;
		cvmx_write_csr_node(node, CVMX_OCX_QLMX_CFG(ccpi_qlm), qlmx_cfg.u64);
		mdelay(10); /* 1ms proved to be too short in rare conditions */
		/* Set count to zero */
		lstate->steps = 0;
		/* Change our state if needed */
		if (lstate->lane_state > STATE_WAIT_FOR_READY)
			lane_change_state(ccpi_lane, STATE_WAIT_FOR_READY);
	}
}

/**
 * Step the lane state machine
 *
 * @param ccpi_lane Lane to update
 * @param is_master True if this node is the master
 */
static void lane_state_update(int ccpi_lane, bool is_master)
{
	union cvmx_ocx_lnex_trn_ld trn_ld;
	struct lane_state *lstate = &lane_state[ccpi_lane];

	debug("%s(%d, %s)\n", __func__, ccpi_lane, is_master ? "true" : "false");
	/* Unconditionally move from the start state to EIE detect */
	if (lstate->lane_state == STATE_START)
		lane_change_state(ccpi_lane, STATE_WAIT_EIE);

	/* Update Link Partner message state */
	ccpi_update_lp_messages(ccpi_lane);

	lane_check_eie(ccpi_lane);
	lane_check_cdr(ccpi_lane);
	lane_check_messaging(ccpi_lane, is_master);
	lane_check_training(ccpi_lane, is_master);
	lane_check_ready(ccpi_lane);

	trn_ld.u64 = cvmx_read_csr_node(node, CVMX_OCX_LNEX_TRN_LD(ccpi_lane));
	if (DEBUG_TRAIN_MESSAGES && (trn_ld.s.ld_cu_dat != lstate->ld_cu.u16))
		printf("Lane %2u: LD CU 0x%04x -> 0x%04x\n", ccpi_lane,
		       trn_ld.s.ld_cu_dat, lstate->ld_cu.u16);
	trn_ld.s.ld_cu_dat = lstate->ld_cu.u16;
	if (DEBUG_TRAIN_MESSAGES && (trn_ld.s.ld_sr_dat != lstate->ld_sr.u16))
		printf("Lane %2u: LD SR 0x%04x -> 0x%04x\n", ccpi_lane,
		       trn_ld.s.ld_sr_dat, lstate->ld_sr.u16);
	trn_ld.s.ld_sr_dat = lstate->ld_sr.u16;
	cvmx_write_csr_node(node, CVMX_OCX_LNEX_TRN_LD(ccpi_lane), trn_ld.u64);
}

void init_ccpi_early(bool is_master)
{
	union cvmx_gserx_phy_ctl gserx_phy_ctl;
	union cvmx_ocx_lnkx_cfg ocx_lnkx_cfg;
	union cvmx_ocx_qlmx_cfg ocx_qlmx_cfg;
	union cvmx_ocx_lnex_trn_ld trn_ld;
	union cvmx_ocx_lnex_trn_ctl trn_ctl;
	union cvmx_gserx_br_rxx_ctl br_rxx_ctl;
#if 0
	union cvmx_l2c_oci_ctl l2c_oci_ctl;
#endif
	int link;
	int ccpi_lane;
	int ccpi_qlm;
	int lane;
	int baud_mhz;

	debug("%s(%s)\n", __func__, is_master ? "true" : "false");

	node = ccpi_get_node_num();

	gserx_phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_GSERX_PHY_CTL(8));

	debug("%s: phy reset: %d\n", __func__, gserx_phy_ctl.s.phy_reset);
	if (gserx_phy_ctl.s.phy_reset) {
		return;
	}

	for (link = 0; link < CCPI_MAX_LINKS; link++) {
		ocx_lnkx_cfg.u64 =
			cvmx_read_csr_node(node, CVMX_OCX_LNKX_CFG(link));
		ocx_lnkx_cfg.s.lane_align_dis = 1;
		cvmx_write_csr_node(node, CVMX_OCX_LNKX_CFG(link),
				    ocx_lnkx_cfg.u64);
	}
	ocx_qlmx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(0));
	if (ocx_qlmx_cfg.s.trn_ena) {
		for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
			trn_ctl.u64 = 0;
			trn_ctl.s.done = 1;
			cvmx_write_csr_node(node, CVMX_OCX_LNEX_TRN_CTL(ccpi_lane),
					    trn_ctl.u64);
		}
		mdelay(100);
		/* Force training into manual mode so we can control it */
		for (ccpi_lane  = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
			trn_ld.u64 = 0;
			trn_ld.s.lp_manual = 1;
			cvmx_write_csr_node(node, CVMX_OCX_LNEX_TRN_LD(ccpi_lane),
					   trn_ld.u64);
		}

		for (ccpi_qlm = 0; ccpi_qlm < 6; ccpi_qlm++) {
			for (lane = 0; lane < 4; lane++) {
				/* Disable each GSER Lane KR Training time-out
				 * timer (330msec)
				 */
				br_rxx_ctl.u64 =
					cvmx_read_csr_node(node,
							   CVMX_GSERX_BR_RXX_CTL(ccpi_qlm + 8, lane));
				/* Disable timer */
				br_rxx_ctl.s.rxt_adtmout_disable = 1;
				/* Software control of RX equalization */
				br_rxx_ctl.s.rxt_swm = 1;
				br_rxx_ctl.s.rxt_initialize = 1;
				cvmx_write_csr_node(node, CVMX_GSERX_BR_RXX_CTL(ccpi_qlm + 8, lane),
						    br_rxx_ctl.u64);
			}
			baud_mhz = ccpi_get_speed();
			/* Erratas 25992, 26150 and 26636 only apply to pass 1.X */
			ccpi_qlm_errata_gser_27140(node, ccpi_qlm + 8, baud_mhz, -1);
		}

		/* Training may not update update PHY Tx TAPS */
		for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
			int qlm = ccpi_lane / 4 + 8;
			int lane = ccpi_lane % 4;

			ccpi_qlm_errata_gser_27882(node, qlm, lane);
		}

	}
#if 0	/* This causes node 1 to hang and it's done later */
	/* We use L2C_OCI_CTL[iofrcl] to remember if this code has been run
	 * when we pass through multiple images
	 */
	l2c_oci_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
	l2c_oci_ctl.s.iofrcl = 0;
	cvmx_write_csr_node(node, CVMX_L2C_OCI_CTL, l2c_oci_ctl.u64);
#endif

	/* Disable the bad lane timer and clear all bad bits */
	for (ccpi_qlm = 0; ccpi_qlm < 6; ccpi_qlm++) {
		ocx_qlmx_cfg.u64 = cvmx_read_csr_node(node,
						      CVMX_OCX_QLMX_CFG(ccpi_qlm));
		ocx_qlmx_cfg.s.timer_dis = 1;
		ocx_qlmx_cfg.s.ser_lane_bad = 0;
		cvmx_write_csr_node(node, CVMX_OCX_QLMX_CFG(ccpi_lane),
				    ocx_qlmx_cfg.u64);
	}
}

static int init_ccpi_master_connection(void)
{
	union cvmx_gserx_rx_eie_detsts eie_detsts;
	struct lane_state *lstate;
	int ccpi_lane;
	int tx_swing;
	int tx_premptap;
	int channel_loss;

	eie_detsts.u64 = cvmx_read_csr_node(node, CVMX_GSERX_RX_EIE_DETSTS(8));
	if (eie_detsts.s.cdrlock == 0) {
		debug("No CDR lock on QLM8, assuming multi-node is not available\n");
		return -1;
	}
	for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
		lstate = &lane_state[ccpi_lane];

		/* Master has access to config data, read defaults */
		tx_swing = CONFIG_OCTEON_CCPI_TX_SWING;
		if (tx_swing != -1)
			lstate->init_main = tx_swing;

		tx_premptap = CONFIG_OCTEON_CCPI_TX_PREEMP_TAP;
		if (tx_premptap != -1) {
			lstate->init_post = tx_premptap >> 4;
			lstate->init_pre = tx_premptap & 0xf;
		}
		channel_loss = CONFIG_OCTEON_CCPI_CHANNEL_LOSS;
		ccpi_channel_loss(ccpi_lane, channel_loss);
	}
	return 0;
}

static void ccpi_disable_link_layer(int node)
{
	union cvmx_ocx_lnkx_cfg ocx_lnkx_cfg;
	int link;

	ocx_lnkx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_LNKX_CFG(0));
	if (ocx_lnkx_cfg.s.lane_align_dis == 0) {
		debug("%s: Lanes not ready, disabling alignment\n", __func__);

		/* Make sure the link layer is down using lane alignment */
		for (link = 0; link < CCPI_MAX_LINKS; link++) {
			ocx_lnkx_cfg.u64 = cvmx_read_csr_node(node,
							      CVMX_OCX_LNKX_CFG(link));
			ocx_lnkx_cfg.s.lane_align_dis = 1;
			cvmx_write_csr_node(node, CVMX_OCX_LNKX_CFG(link),
					    ocx_lnkx_cfg.u64);
		}
	}
}

static void ccpi_enable_link(int node, int link, uint32_t *valid_links)
{
	union cvmx_ocx_lnkx_cfg ocx_lnkx_cfg;
	union cvmx_ocx_com_linkx_ctl ocx_com_linkx_ctl;
	union cvmx_ocx_tlkx_lnk_vcx_cnt ocx_tlkx_lnk_vcx_cnt;

	ocx_lnkx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_LNKX_CFG(link));
	ocx_com_linkx_ctl.u64 = cvmx_read_csr_node(node, CVMX_OCX_COM_LINKX_CTL(link));

	/* If lane alignment is disabled, enable it */
	if (ocx_lnkx_cfg.s.lane_align_dis) {
		debug("%s: link %2d is disabled, enabling\n", __func__, link);
		ocx_lnkx_cfg.s.lane_align_dis = 0;
		cvmx_write_csr_node(node, CVMX_OCX_LNKX_CFG(link),
				    ocx_lnkx_cfg.u64);
	} else if (ocx_com_linkx_ctl.s.auto_clr ||
		   ocx_com_linkx_ctl.s.drop ||
		   ocx_com_linkx_ctl.s.reinit) {
		/* Clear bits that can get set by
		 * hardware/software on error
		 */
		debug("%s: link %2d error bits being reset\n", __func__, link);
		ocx_com_linkx_ctl.s.auto_clr = 0;
		ocx_com_linkx_ctl.s.drop = 0;
		ocx_com_linkx_ctl.s.reinit = 0;
		cvmx_write_csr_node(node, CVMX_OCX_COM_LINKX_CTL(link),
				    ocx_com_linkx_ctl.u64);
		/* Give link 10ms to update */
		mdelay(10);
	}  else if (ocx_com_linkx_ctl.s.up &&
		    ocx_com_linkx_ctl.s.valid) {
		/* If the link is up and valid, check the
		 * credits
		 */
		debug("%s: link %2d is up\n", __func__, link);
		ocx_tlkx_lnk_vcx_cnt.u64 = cvmx_read_csr_node(node,
							      CVMX_OCX_TLKX_LNK_VCX_CNT(link, 0));
		if (ocx_tlkx_lnk_vcx_cnt.s.count == 0) {
			/* No credits, perform a reinit */
			ocx_com_linkx_ctl.s.reinit = 1;
			cvmx_write_csr_node(node, CVMX_OCX_COM_LINKX_CTL(link),
					    ocx_com_linkx_ctl.u64);
			debug("%s: Link %d: Re-init\n", __func__, link);
			/* Give link 10ms to update */
			mdelay(10);
		} else {
			*valid_links |= 1 << link;
		}
	}
}

int init_ccpi_connection2(int is_master, uint64_t gbaud, bool trace)
{
	union cvmx_gserx_phy_ctl gserx_phy_ctl;
	union cvmx_ocx_qlmx_cfg ocx_qlmx_cfg;
	union cvmx_rst_ocx rst_ocx;
	struct lane_state *lstate;
	int baud_mhz;
	int ccpi_lane;
	uint32_t start_time;
	uint32_t master_timeout;
	const uint32_t one_second = 1000;
	int good_lanes;
	int link;
	uint32_t valid_links = 0;

	debug("%s(%d, %llu)\n", __func__, is_master,
	      (unsigned long long)gbaud);
	/* Bail early if CCPI is disabled */
	node = ccpi_get_node_num();

	gserx_phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_GSERX_PHY_CTL(8));
	if (gserx_phy_ctl.s.phy_reset) {
		debug("%s: CCPI for node %d is disabled\n", __func__, node);
		return -1;
	}
	baud_mhz = ccpi_get_speed();

	memset(lane_state, 0, sizeof(lane_state));

	for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
		lstate = &lane_state[ccpi_lane];
		lstate->init_main = TRAIN_INIT_TX_MAIN;
		lstate->init_post = TRAIN_INIT_TX_POST;
		lstate->init_pre = TRAIN_INIT_TX_PRE;
	}

	if (is_master && init_ccpi_master_connection()) {
		debug("No CDR lock on QLM8, assuming multi-node is not available\n");
		return -1;
	}

	/* Configure the starting tuning settings */
	for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
		struct lane_state *lstate = &lane_state[ccpi_lane];
		lstate->tx_main = lstate->init_main;
		lstate->tx_post = lstate->init_post;
		lstate->tx_pre = lstate->init_pre;
		ccpi_tx_tune(ccpi_lane, lstate->tx_main, lstate->tx_pre,
			     lstate->tx_post);
	}

	/* Wait 100ms after making changes before continuing */
	mdelay(100);

	if (1) {
		printf("Node %d, CCPI Speed: %2d.%02d", node, baud_mhz / 1000,
		       baud_mhz % 1000);
		ocx_qlmx_cfg.u64 = cvmx_read_csr_node(node, CVMX_OCX_QLMX_CFG(0));
		if (ocx_qlmx_cfg.s.trn_ena && ocx_qlmx_cfg.s.trn_rxeq_only)
			printf(", RX equalization only\n");
		else if (ocx_qlmx_cfg.s.trn_ena)
			printf(", KR Training\n");
		else
			printf(", No Training\n");
	}
	start_time = get_timer(0);
	master_timeout = one_second * CCPI_MASTER_TIMEOUT;
	good_lanes = 0;

	/* Loop forever trying to get CCPI links up */
	while (1) {
		if (is_master &&
		    (get_timer(start_time) >= master_timeout)) {
			ccpi_display_tuning_state_all();
			printf("OCI/CCPI timed out, %d valid links (%d lanes) out of %d\n",
			       valid_links, good_lanes, CCPI_MIN_LINKS);
			return -1;
		}

		/* The node ID may change while we're running */
		node = ccpi_get_node_num();
		debug("%s: node: %d\n", __func__, node);

		/* Perform RX equalization on all lanes, updating the training
		 * direction hints
		 */
		ccpi_update_rx_equalization();

		if (trace)
			ccpi_display_state();
		/* Loop updating the state of each lane. Count how many lanes
		 *           are up and running */
		good_lanes = 0;
		for (ccpi_lane = 0; ccpi_lane < CCPI_LANES; ccpi_lane++) {
			lane_state_update(ccpi_lane, is_master);
			/* For debug, we only do one lane at a time so it is
			 * easy to follow.  We don't check a lane until all
			 * lanes before it are done with training.  Note that
			 * lanes bounce in and out of ready while lanes next
			 * to them come up.  Too slow for production
			 */
			if (lane_state[ccpi_lane].lane_state == STATE_READY) {
				good_lanes++;
				debug("%s: Lane %2d state is READY\n",
				      __func__, ccpi_lane);
			} else {
				debug("%s: Lane %2d state is %d NOT READY\n",
				      __func__, ccpi_lane, lane_state[ccpi_lane].lane_state);
			}
		}

		debug("%s: %d lanes are ready (good)\n", __func__, good_lanes);
		/* Don't check links until all lanes are up */
		if (good_lanes < CCPI_LANES) {
			debug("%s: Not all lanes are up (%d of %d), disabling link layer\n",
			     __func__, good_lanes, CCPI_LANES);
			ccpi_disable_link_layer(node);
			/* Skip the rest of the loop body until lanes are all
			 * good
			 */
			continue;
		}

		valid_links = 0;
		/* Check that the three CCPI links are operating */
		for (link = 0; link < CCPI_MAX_LINKS; link++)
			ccpi_enable_link(node, link, &valid_links);

		/* Require the links to be valid before marking everything good */
		if (__builtin_popcount(valid_links) >= CCPI_MIN_LINKS) {
			/* Reset if we loose a link */
			rst_ocx.u64 = 0;
			rst_ocx.s.rst_link = valid_links;
			cvmx_write_csr_node(node, CVMX_RST_OCX, rst_ocx.u64);
			cvmx_read_csr_node(node, CVMX_RST_OCX);
			if (DEBUG_STATE)
				printf("%s: %d CCPI links up\n",
				       __func__, __builtin_popcount(valid_links));
			return 0;
		}
	}
	printf("OCX initialization failed, %d out of at least %d links up\n",
	       __builtin_popcount(valid_links), CCPI_MIN_LINKS);
	return -1;
}
