/***********************license start************************************
 * Copyright (c) 2016 Cavium, Inc. <support@cavium.com>.  All rights
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

#include <cvmx.h>
#include <cvmx-clock.h>
#include <cvmx-qlm.h>

/* Indexed by QLM number and lane */
static uint64_t cvmx_prbs_errors[14][4];

typedef enum
{
	CVMX_PRBS_DIRECTION_TX = 1,
	CVMX_PRBS_DIRECTION_RX = 2,
	CVMX_PRBS_DIRECTION_BOTH = 3,
} cvmx_prbs_direction_t;

/**
 * Return the number of PRBS errors since PRBS started running
 *
 * @param node   Node to use in numa setup
 * @param qlm    QLM to use
 * @param lane   Which lane
 * @param clear  Clear counter after return the current value
 *          
 * @return Number of errors
 */
static uint64_t __cvmx_prbs_get_prbs_errors(int node, int qlm, int lane, int clear)
{
	cvmx_gserx_lanex_lbert_cfg_t lbert_cfg;
	cvmx_gserx_lanex_lbert_ecnt_t lbert_ecnt;
	int errors;
	uint64_t result = 0;

	/* Restart synchronization */
	lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
	lbert_cfg.s.lbert_pm_sync_start = 1;
	cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);

	/* This CSR is self clearing per the CSR description, but it doesn't
	   seem to do that. Instead it clears when we trigger sync again */
	lbert_ecnt.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_ECNT(lane, qlm));
	errors = lbert_ecnt.s.lbert_err_cnt;
	if (lbert_ecnt.s.lbert_err_ovbit14)
		errors <<= 7;
	cvmx_prbs_errors[qlm][lane] += errors;
	result = cvmx_prbs_errors[qlm][lane];

	if (clear)
		cvmx_prbs_errors[qlm][lane] = 0;

	return result;
}

static char *__cvmx_prbs_f1(int node, int qlm, int lane, char *buf)
{
	if (lane == 0)
		sprintf(buf, "--- QLM%d ", qlm);
	else
		sprintf(buf, "----------");
	return buf;
}

static char *__cvmx_prbs_f2(int node, int qlm, int lane, char *buf)
{
	sprintf(buf, "Lane%d ", lane);
	return buf;
}

static char *__cvmx_prbs_f3(int node, int qlm, int lane, char *buf)
{
	uint64_t v = __cvmx_prbs_get_prbs_errors(node, qlm, lane, false);
	if (v == -1ull)
		return "No Lock";
	else {
		if (v == 0)
			return "Error";
		else {
			if (v < 1000000) {
				sprintf(buf, "%lld", (long long int)v);
				return buf;
			} else if (v < 1000000000) {
				sprintf(buf, "%lldM", (long long int)v/1000000);
				return buf;
			} else {
				sprintf(buf, "%lldB", (long long int)v/1000000000);
				return buf;
			}
		}
	}
}

static char *__cvmx_prbs_f4(int node, int qlm, int lane, uint64_t run_time, char *buf)
{
	uint64_t baud_mhz = cvmx_qlm_get_gbaud_mhz_node(node, qlm);
	uint64_t v = __cvmx_prbs_get_prbs_errors(node, qlm, lane, false);

	if ((v == -1ull) || (run_time == 0ull))
		return "-";
	else {
		/* Calcuclate the error rate as a fraction of the bits transmitted */
		int rate_exponent = -6;
		int rate;
		uint64_t total_bits = baud_mhz * run_time;
		if (total_bits == 0)
			total_bits = 1;

		if (v == 0)
			v = 1;

		do {
			rate_exponent -= 1;
			v *= 10;
		} while (v < total_bits * 100);

		/* Calculate the error rate */
		rate = v / total_bits;	

		/* Scale the error rate to be three digits */
		do {
			rate_exponent += 1;
			rate /= 10;
		} while (rate >= 1000);

		do {
			rate_exponent -= 1;
			rate *= 10;
		} while (rate < 100);

		/* Convert the three digit error rate into scientific notation */

		sprintf(buf, "%d.%02dE%d", rate/100, rate%100, rate_exponent + 2);
		return buf;
	}
}

static char *__cvmx_prbs_f5(int node, int qlm, int lane)
{
	return "----------";
}

#define GET_VAL(node, qlm, l, function, run_time) 	\
  ({char *result;					\
      result = malloc(20);	\
      switch(function) {				\
      case 1:						\
           result = __cvmx_prbs_f1(node,qlm,l,result);		\
           break;					\
      case 2:						\
           result = __cvmx_prbs_f2(node,qlm,l,result);		\
           break;					\
      case 3:						\
           result = __cvmx_prbs_f3(node,qlm,l,result);		\
           break;					\
      case 4:						\
           result = __cvmx_prbs_f4(node,qlm,l,run_time,result);	\
           break;					\
      case 5:						\
           result = __cvmx_prbs_f5(node,qlm,l);		\
           break;					\
      }							\
   result;})
 

static void __cvmx_prbs_output_line(int node, int qlm, char *label, int function, uint64_t run_time)
{
	int max_lanes = cvmx_qlm_get_lanes(qlm);
	int l;

	printf("%21s", label);
	for (l = 0; l < max_lanes; l++) {
		if (l == 0)
			printf("|%10s", GET_VAL(node,qlm,l,function,run_time));
		else
			printf(" %10s", GET_VAL(node,qlm,l,function,run_time));

	}
	printf("|\n");
}

static void __cvmx_prbs_display_status(int node, int qlm, uint64_t run_time)
{
	printf("\n\n");
	printf("Time: %lld seconds\n", (long long int)run_time);
	__cvmx_prbs_output_line(node, qlm, "", 1, run_time);
	__cvmx_prbs_output_line(node, qlm, "", 2, run_time);
	__cvmx_prbs_output_line(node, qlm, "Errors", 3, run_time);
	__cvmx_prbs_output_line(node, qlm, "Error Rate", 4, run_time);
	__cvmx_prbs_output_line(node, qlm, "", 5, run_time);
}

static int __cvmx_prbs_enable(int node, int qlm, int prbs, int direction)
{
	cvmx_gserx_cfg_t gserx_cfg;
	cvmx_gserx_phy_ctl_t phy_ctl;
	cvmx_gserx_lanex_lbert_cfg_t lbert_cfg;

	int max_lanes = cvmx_qlm_get_lanes(qlm);
	int mode;
	int retry, lane;

	switch(prbs) {
	case 31:
		mode = 1;
		break;
	case 23:
		mode = 2;	/* or 3? */
		break;
	case 16:
		mode = 4;
		break;
	case 15:
		mode = 5;
		break;
	case 11:
		mode = 6;
		break;
	case 7:
		mode = 7;
		break;
	default:
		mode = prbs & 0xff;
		for (lane = 0; lane < max_lanes; lane++) {
			cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_PAT_CFG(lane, qlm), prbs >> 8);
		}
		cvmx_printf("Using mode 0x%x with custom pattern 0x%x\n", mode, prbs >> 8);
		break;
	}

	/* For some reason PRBS doesn't work if GSER is configured for PCIe.
	   Disconnect PCIe when we start PRBS */
	gserx_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_CFG(qlm));
	if (gserx_cfg.s.pcie) {
		gserx_cfg.s.pcie = 0;
		cvmx_write_csr_node(node, CVMX_GSERX_CFG(qlm), gserx_cfg.u64);
		printf("WARNING: N%d.QLM%d: Disabling PCIe for PRBS/pattern generation\n", node, qlm);
	}

	/* For some reason PRBS doesn't work if GSER is configured for SATA.
	   Disconnect SATA when we start PRBS */
	if (gserx_cfg.s.sata) {
		gserx_cfg.s.sata = 0;
		cvmx_write_csr_node(node, CVMX_GSERX_CFG(qlm), gserx_cfg.u64);
		printf("WARNING: N%d.QLM%d: Disabling SATA for PRBS/pattern generation\n", node, qlm);
		printf("WARNING: N%d.QLM%d: SATA PRBS/patterns always run at 6G\n", node, qlm);
	}

	phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_GSERX_PHY_CTL(qlm));
	phy_ctl.s.phy_reset = 0;
	cvmx_write_csr_node(node, CVMX_GSERX_PHY_CTL(qlm), phy_ctl.u64);

	if (direction & CVMX_PRBS_DIRECTION_TX) {
		/* Disable first in case already running */
		for (lane = 0; lane < max_lanes; lane++) {
			lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
			lbert_cfg.s.lbert_pg_en = 0;
			cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);
		}
		for (lane = 0; lane < max_lanes; lane++) {
			lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
			lbert_cfg.s.lbert_pg_en = 1;	/* Enable generator */
			lbert_cfg.s.lbert_pg_width = 3;	/* 20 bit */
			lbert_cfg.s.lbert_pg_mode = mode;
			cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);
		}
	}
	if (direction & CVMX_PRBS_DIRECTION_RX) {
		/* Clear the error counter and disable the matcher */
		for (lane = 0; lane < max_lanes; lane++) {
			cvmx_prbs_errors[qlm][lane] = 0;
			lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
			lbert_cfg.s.lbert_pm_en = 0;
			cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);
		}
		for (lane = 0; lane < max_lanes; lane++) {
			lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
			lbert_cfg.s.lbert_pm_en = 1;	/* Enable matcher */
			lbert_cfg.s.lbert_pm_width = 3;	/* 20 bit */
			lbert_cfg.s.lbert_pm_mode = mode;
			cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);
		}

		/* Tell the matcher to start sync */
		for (retry = 0; retry < 4; retry++) {
			for (lane = 0; lane < max_lanes; lane++) {
				lbert_cfg.u64 = cvmx_read_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm));
				lbert_cfg.s.lbert_pm_sync_start = 1;
				cvmx_write_csr_node(node, CVMX_GSERX_LANEX_LBERT_CFG(lane, qlm), lbert_cfg.u64);
			}
			/* Wait 10 ms */
			if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_SIM)
				cvmx_wait_usec(10000);
		}
	}
	return 0;
}

static void __cvmx_prbs_start(int node, int qlm, int mode)
{
	__cvmx_prbs_enable(node, qlm, mode, 1);

	/* Let TX run for 1ms before starting RX */
	if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_SIM)
		cvmx_wait_usec(1000);
	__cvmx_prbs_enable(node, qlm, mode, 2);
}

void cvmx_prbs_process_cmd(int node, int qlm, int mode)
{
	uint64_t start_time, end_time;
	uint64_t run_time = 0;
	uint64_t rate = cvmx_clock_get_rate(CVMX_CLOCK_CORE);
	int count = 0;

	__cvmx_prbs_start(node, qlm, mode);
	start_time =  (cvmx_clock_get_count(CVMX_CLOCK_CORE) * 1024ull) / rate;
	end_time = start_time;
	do {
		if (run_time != 0)
			end_time = (cvmx_clock_get_count(CVMX_CLOCK_CORE) * 1024ull) / rate;
		run_time = (end_time - start_time) + 5;
		start_time = end_time;
		__cvmx_prbs_display_status(node, qlm, run_time);
		count++;
	} while (count < 10);
}
