/***********************license start************************************
 * Copyright (c) 2013-2016 Cavium, Inc. <support@cavium.com>.  All rights
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
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#ifndef __OCTEON_QLM_H__
#define __OCTEON_QLM_H__

#if defined (CONFIG_OCTEON_QLM) || defined(CVMX_BUILD_FOR_STANDALONE)

/* Reference clock selector values for ref_clk_sel */
#define OCTEON_QLM_REF_CLK_100MHZ		0	/** 100 MHz */
#define OCTEON_QLM_REF_CLK_125MHZ		1	/** 125 MHz */
#define OCTEON_QLM_REF_CLK_156MHZ		2	/** 156.25 MHz */
#define OCTEON_QLM_REF_CLK_161MHZ		3	/** 161.1328125 MHz */

/**
 * Configure qlm/dlm speed and mode.
 * @param qlm     The QLM or DLM to configure
 * @param speed   The speed the QLM needs to be configured in Mhz.
 * @param mode    The QLM to be configured as SGMII/XAUI/PCIe.
 * @param rc      Only used for PCIe, rc = 1 for root complex mode, 0 for EP
 *		  mode.
 * @param pcie_mode Only used when qlm/dlm are in pcie mode.
 * @param ref_clk_sel Reference clock to use for 70XX where:
 *			0: 100MHz
 *			1: 125MHz
 *			2: 156.25MHz
 *			3: 161.1328125MHz (CN73XX and CN78XX only)
 * @param ref_clk_input	This selects which reference clock input to use.  For
 *			cn70xx:
 *				0: DLMC_REF_CLK0
 *				1: DLMC_REF_CLK1
 *				2: DLM0_REF_CLK
 *			cn61xx: (not used)
 *			cn78xx/cn76xx/cn73xx:
 *				0: Internal clock (QLM[0-7]_REF_CLK)
 *				1: QLMC_REF_CLK0
 *				2: QLMC_REF_CLK1
 *
 * @return	Return 0 on success or -1.
 *
 * @note	When the 161MHz clock is used it can only be used for
 *		XLAUI mode with a 6316 speed or XFI mode with a 103125 speed.
 *		This rate is also only supported for CN73XX and CN78XX.
 */
extern int octeon_configure_qlm(int qlm, int speed, int mode, int rc,
				int pcie_mode, int ref_clk_sel,
				int ref_clk_input);

extern int octeon_configure_qlm_cn78xx(int node, int qlm, int speed, int mode,
				       int rc, int pcie_mode, int ref_clk_sel,
				       int ref_clk_input);

/**
 * Some QLM speeds need to override the default tuning parameters
 *
 * @param node     Node to configure
 * @param qlm      QLM to configure
 * @param baud_mhz Desired speed in MHz
 * @param lane     Lane the apply the tunning parameters
 * @param tx_swing Voltage swing.  The higher the value the lower the voltage,
 *		   the default value is 7.
 * @param tx_pre   pre-cursor pre-emphasis
 * @param tx_post  post-cursor pre-emphasis.
 * @param tx_gain   Transmit gain. Range 0-7
 * @param tx_vboost Transmit voltage boost. Range 0-1
 */
void octeon_qlm_tune_per_lane_v3(int node, int qlm, int baud_mhz, int lane,
			int tx_swing, int tx_pre, int tx_post, int tx_gain,
			int tx_vboost);

/**
 * Some QLM speeds need to override the default tuning parameters
 *
 * @param node     Node to configure
 * @param qlm      QLM to configure
 * @param baud_mhz Desired speed in MHz
 * @param tx_swing Voltage swing.  The higher the value the lower the voltage,
 *		   the default value is 7.
 * @param tx_premptap bits [0:3] pre-cursor pre-emphasis, bits[4:8] post-cursor
 *		      pre-emphasis.
 * @param tx_gain   Transmit gain. Range 0-7
 * @param tx_vboost Transmit voltage boost. Range 0-1
 */
extern void octeon_qlm_tune_v3(int node, int qlm, int baud_mhz, int tx_swing,
			       int tx_premptap, int tx_gain, int tx_vboost);

/**
 * Disables DFE for the specified QLM lane(s).
 * This function should only be called for low-loss channels.
 *
 * @param node     Node to configure
 * @param qlm      QLM to configure
 * @param lane     Lane to configure, or -1 all lanes
 * @param baud_mhz The speed the QLM needs to be configured in Mhz.
 * @param mode     The QLM to be configured as SGMII/XAUI/PCIe.
 */
void octeon_qlm_dfe_disable(int node, int qlm, int lane, int baud_mhz, int mode);

/**
 * Some QLMs need to override the default pre-ctle for low loss channels.
 *
 * @param node     Node to configure
 * @param qlm      QLM to configure
 * @param pre_ctle pre-ctle settings for low loss channels
 */
extern void octeon_qlm_set_channel_v3(int node, int qlm, int pre_ctle);

extern void octeon_init_qlm(int node);

extern int octeon_mcu_probe(int node);

#else
# error CONFIG_OCTEON_QLM is not defined!
#endif	/* CONFIG_OCTEON_QLM */

#endif	/* __OCTEON_QLM_H__ */
