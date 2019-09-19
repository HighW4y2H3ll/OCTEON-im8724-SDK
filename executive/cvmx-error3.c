/***********************license start***************
 * Copyright (c) 2017  Cavium Inc. (support@cavium.com). All rights
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

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

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
 * Interface to the Octeon 78xx extended error status.
 *
 * <hr>$Revision$<hr>
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-error.h>
#include <asm/octeon/cvmx-error-custom.h>
#include <asm/octeon/cvmx-pcie.h>
#include <asm/octeon/cvmx-srio.h>
#include <asm/octeon/cvmx-ciu2-defs.h>
#include <asm/octeon/cvmx-dfm-defs.h>
#include <asm/octeon/cvmx-lmcx-defs.h>
#include <asm/octeon/cvmx-l2c-defs.h>
#include <asm/octeon/cvmx-pexp-defs.h>
#include <asm/octeon/cvmx-helper-cfg.h>
#include <asm/octeon/cvmx-ocx-defs.h>
#include <asm/octeon/cvmx-l2c.h>
#include <asm/octeon/cvmx-atomic.h>
#else
#include "cvmx.h"
#include "cvmx-error.h"
#include "cvmx-error-arrays-cn78xx.h"
#include "cvmx-ciu3-defs.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-lmcx-defs.h"
#include "cvmx-interrupt.h"
#include "cvmx-helper-cfg.h"
#include "cvmx-l2c.h"
#include "cvmx-atomic.h"
#endif

/** Maximum number of ECC error locations to store */
#define MAX_SEC_ERRORS	256

/** Threshold per location to print ECC errors */
#define SEC_ERROR_PRINT_THRESHOLD	2

/** Data structure to record ECC memory errors */
struct lmc_sec_error {
	uint64_t address;	/** 64-bit address of failure */
	uint64_t data;		/** Data of failure */
	uint32_t count;		/** Number of failures at this address */
	uint32_t seq;		/** Sequence number */
	uint32_t row;		/** Row number */
	uint16_t column;	/** Column number */
	uint8_t lmc;		/** LMC number */
	uint8_t sec_err;	/** See LMCX_INT[SEC_ERR] */
	uint8_t ded_err;	/** See LMCX_INT[DED_ERR] */
	uint8_t dimm;		/** DIMM number */
	uint8_t rank;		/** Memory rank */
	uint8_t bank;		/** Memory bank */
	uint8_t bit;		/** Bit for single bit errors */
	uint8_t syndrome;	/** Syndrome reported */
	bool used;		/** True if entry was used */
};

static CVMX_SHARED struct cvmx_error_78xx *octeon_error_array_current = NULL;
static CVMX_SHARED int octeon_error_array_current_size = 0;

/** Log of single-bit ECC errors */
static CVMX_SHARED struct lmc_sec_error sec_errors[MAX_SEC_ERRORS];

/** Number of single-bit ECC errors encountered */
static CVMX_SHARED uint32_t num_sec_errors = 0;

/** Last index of single-bit ECC error recorded */
static CVMX_SHARED int current_sec_error;

/** Single-bit ECC error sequence number (number of errors) */
static CVMX_SHARED uint32_t sec_seq = 0;

int (*cvmx_error3_enable_group_override)(cvmx_error_group_t group, int xipd_port) = NULL;
int (*cvmx_error3_disable_group_override)(cvmx_error_group_t group, int xipd_port) = NULL;
int (*cvmx_error3_handler_override)(int index, struct cvmx_error_78xx *entry) = NULL;

/**
 * Return a pointer to an entry to store the error, choosing a matching
 * error if possible
 *
 * @param	addr	address of error
 * @param	bit	bit of error
 *
 * @return	pointer to entry to store the error
 */
static struct lmc_sec_error *__find_seq_error(uint64_t addr, uint8_t bit)
{
	int i;
	uint32_t min_seq = (uint32_t)-1;
	int min_seq_idx = -1;
	uint32_t min_count = (uint32_t)-1;

	/* See if we've seen this error before and report the minimum number
	 * of errors we fine.
	 */
	for (i = 0; i < MAX_SEC_ERRORS; i++) {
		if (!sec_errors[i].used)
			continue;
		if ((sec_errors[i].address == addr) && (sec_errors[i].bit == bit)) {
			current_sec_error = i;
			return &sec_errors[i];
		}
		/* Keep track of the minimum number of errors we have reported
		 */
		if (sec_errors[i].count < min_count)
			min_count = sec_errors[i].count;
	}

	/* Search for the first free entry */
	for (i = 0; i < MAX_SEC_ERRORS; i++)
		if (!sec_errors[i].used) {
			current_sec_error = i;
			return &sec_errors[i];
		}

	/* All entries are in use, look for the oldest entry with the minimum
	 * error count.
	 *
	 * This is guaranteed to return a valid entry except for an extremely
	 * rare corner case where there is only one entry with the minimum
	 * error count and the sequence index is 0xffffffff.
	 */
	for (i = 0; i < MAX_SEC_ERRORS; i++) {
		if (sec_errors[i].count > min_count)
			continue;
		if (sec_errors[i].seq < min_seq) {
			min_seq = sec_errors[i].seq;
			min_seq_idx = i;
		}
	}

	if (min_seq_idx < 0)
		min_seq_idx = 0;
	/* We should probably print out the error we're throwing away */

	memset(&sec_errors[min_seq_idx], 0, sizeof(struct lmc_sec_error));
	current_sec_error = min_seq_idx;

	return &sec_errors[min_seq_idx];
}

#define _Db(x) (x)		/* Data Bit */
#define _Ec(x) (0x100+x)	/* ECC Bit */
#define _Ad(x) (0x200+x)	/* Address Bit */
#define _Bu(x) (0x400+x)	/* Burst */
#define _Un()  (-1)		/* Unused */
/* Use ECC Code as index to lookup corrected bit */
static const short lmc_syndrome_bits[256] = {
	/*        __ 0 __  __ 1 __  __ 2 __  __ 3 __  __ 4 __  __ 5 __  __ 6 __  __ 7 __  __ 8 __  __ 9 __  __ A __  __ B __  __ C __  __ D __  __ E __  __ F __ */
	/* 00: */ _Un(), _Ec(0), _Ec(1), _Un(), _Ec(2), _Un(), _Un(), _Un(), _Ec(3), _Un(), _Un(), _Db(17), _Un(), _Un(), _Db(16), _Un(),
	/* 10: */ _Ec(4), _Un(), _Un(), _Db(18), _Un(), _Db(19), _Db(20), _Un(), _Un(), _Db(21), _Db(22), _Un(), _Db(23), _Un(), _Un(), _Un(),
	/* 20: */ _Ec(5), _Un(), _Un(), _Db(8), _Un(), _Db(9), _Db(10), _Un(), _Un(), _Db(11), _Db(12), _Un(), _Db(13), _Un(), _Un(), _Un(),
	/* 30: */ _Un(), _Db(14), _Un(), _Un(), _Db(15), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Ad(34), _Un(),
	/* 40: */ _Ec(6), _Un(), _Un(), _Un(), _Un(), _Ad(7), _Ad(8), _Un(), _Un(), _Ad(9), _Db(33), _Un(), _Ad(10), _Un(), _Un(), _Db(32),
	/* 50: */ _Un(), _Ad(11), _Db(34), _Un(), _Db(35), _Un(), _Un(), _Db(36), _Db(37), _Un(), _Un(), _Db(38), _Un(), _Db(39), _Ad(12), _Un(),
	/* 60: */ _Un(), _Ad(13), _Db(56), _Un(), _Db(57), _Un(), _Un(), _Db(58), _Db(59), _Un(), _Un(), _Db(60), _Un(), _Db(61), _Ad(14), _Un(),
	/* 70: */ _Db(62), _Un(), _Un(), _Ad(15), _Un(), _Db(63), _Ad(16), _Un(), _Un(), _Ad(17), _Ad(18), _Un(), _Ad(19), _Un(), _Ad(20), _Un(),
	/* 80: */ _Ec(7), _Un(), _Un(), _Ad(21), _Un(), _Ad(22), _Ad(23), _Un(), _Un(), _Ad(24), _Db(49), _Un(), _Ad(25), _Un(), _Un(), _Db(48),
	/* 90: */ _Un(), _Ad(26), _Db(50), _Un(), _Db(51), _Un(), _Un(), _Db(52), _Db(53), _Un(), _Un(), _Db(54), _Un(), _Db(55), _Ad(27), _Un(),
	/* A0: */ _Un(), _Ad(28), _Db(40), _Un(), _Db(41), _Un(), _Un(), _Db(42), _Db(43), _Un(), _Un(), _Db(44), _Un(), _Db(45), _Ad(29), _Un(),
	/* B0: */ _Db(46), _Un(), _Un(), _Ad(30), _Un(), _Db(47), _Ad(31), _Un(), _Un(), _Ad(32), _Ad(33), _Un(), _Un(), _Un(), _Un(), _Un(),
	/* C0: */ _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Db(1), _Un(), _Un(), _Db(0), _Un(),
	/* D0: */ _Un(), _Un(), _Un(), _Db(2), _Un(), _Db(3), _Db(4), _Un(), _Un(), _Db(5), _Db(6), _Un(), _Db(7), _Un(), _Un(), _Un(),
	/* E0: */ _Un(), _Un(), _Un(), _Db(24), _Un(), _Db(25), _Db(26), _Un(), _Un(), _Db(27), _Db(28), _Un(), _Db(29), _Un(), _Un(), _Un(),
	/* F0: */ _Un(), _Db(30), _Un(), _Un(), _Db(31), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un(), _Un()
};

/**
 * @INTERNAL
 *
 * Process LMC single and multi-bit ECC errors
 *
 */
static int __cvmx_cn7xxx_lmc_ecc_error_record(int node, struct cvmx_error_78xx *entry)
{
	struct lmc_sec_error *lmc_sec_err;
	union cvmx_lmcx_config lmc_config;
	union cvmx_lmcx_int lmc_int;
	union cvmx_lmcx_fadr fadr;
	union cvmx_lmcx_ecc_synd ecc_synd;
	union cvmx_l2c_ctl l2c_ctl;
	union cvmx_lmcx_ddr_pll_ctl lmc_ddr_pll_ctl;
	uint64_t fadr_physical, fadr_data;
	uint64_t fadr_c;
	uint64_t fadr_xkphys;
	int interface_bits = 0;
	int row_bits;
	int col_bits;
	int rank_bits;
	int bank_bits;
	int rmask;
	int cmask;
	int int_mask;
	int pbank_lsb;
	int row_lsb;

	int ddr_controller = (entry->intsn >> 12) & 3;
	int sec_err;
	int ded_err;
	int syndrome = -1;
	int phase;
	int i;
	int bit;

	lmc_int.u64 = cvmx_read_csr_node(node, CVMX_LMCX_INT(ddr_controller));
	fadr.u64 = cvmx_read_csr_node(node, CVMX_LMCX_FADR(ddr_controller));
	ecc_synd.u64 = cvmx_read_csr_node(node, CVMX_LMCX_ECC_SYND(ddr_controller));
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_controller));
	lmc_ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(ddr_controller));
	l2c_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);

	sec_err = lmc_int.s.sec_err;
	ded_err = lmc_int.s.ded_err;
	phase = ded_err ? ded_err : sec_err;	/* Double bit errors take precedence. */

	if (lmc_ddr_pll_ctl.s.ddr4_mode && lmc_config.s.bg2_enable)
		bank_bits = 4;
	else
		bank_bits = 3;

	/* The err mask could contain multiple failures. Detect the first
	 * failing 64b chunk to convert the mask into the phase that
	 * corresponds to the information in FADR.
	 */
	for (i = 0; i < 4; ++i) {
		if ((phase >> i) & 1)
			break;
	}
	phase = i;

	switch (phase) {
	case 0:
		syndrome = ecc_synd.cn78xx.mrdsyn0;
		break;
	case 1:
		syndrome = ecc_synd.cn78xx.mrdsyn1;
		break;
	case 2:
		syndrome = ecc_synd.cn78xx.mrdsyn2;
		break;
	case 3:
		syndrome = ecc_synd.cn78xx.mrdsyn3;
		break;
	}

	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		/* Detect four-lmc mode. */
		cvmx_lmcx_dll_ctl2_t ctl;
		ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(3));
		interface_bits = 1 + (ctl.cn78xx.quad_dll_ena | ctl.cn78xx.intf_en);
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		   OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		/* Detect lmc mode. */
		cvmx_lmcx_dll_ctl2_t ctl;
		ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(1));
		interface_bits = (ctl.cn73xx.intf_en | ctl.cn73xx.quad_dll_ena);
	}

	rank_bits = lmc_config.s.rank_ena;
	pbank_lsb = lmc_config.s.pbank_lsb + 28 - rank_bits;

	row_lsb = lmc_config.s.row_lsb + 14;
	row_bits = pbank_lsb - row_lsb;
	col_bits = row_lsb - bank_bits - 3;

	cmask = (1 << col_bits) - 1;
	rmask = (1 << row_bits) - 1;
	int_mask = (1 << interface_bits) - 1;

	fadr_physical = (uint64_t) fadr.cn73xx.fdimm <<
				(rank_bits + pbank_lsb + interface_bits);
	fadr_physical |= (uint64_t) fadr.cn73xx.fbunk << (pbank_lsb + interface_bits);
	fadr_physical |= (uint64_t) (fadr.cn73xx.frow & rmask) << (row_lsb + interface_bits);
	fadr_physical |= (uint64_t) (fadr.cn73xx.fcol & 0xf) << 3;
	fadr_physical |= (uint64_t) fadr.cn73xx.fbank << (7 + interface_bits);
	fadr_physical |= (uint64_t) ((fadr.cn73xx.fcol & cmask) >> 4) <<
						(7 + bank_bits + interface_bits);
	if (!l2c_ctl.s.disidxalias) {
		fadr_c = (ddr_controller ^ (fadr_physical >> 12) ^
			  (fadr_physical >> 20)) & int_mask;
		fadr_physical |= fadr_c << 7;
	}

	/* Account for 256MB hole */
	fadr_physical += (fadr_physical < (uint64_t) 0x10000000) ? 0 : (uint64_t) 0x10000000;

	bit = lmc_syndrome_bits[syndrome];
	if (phase & 1)
		bit += 64;

	lmc_sec_err = __find_seq_error(fadr_physical, bit);
	if (current_sec_error >= MAX_SEC_ERRORS) {
		/* Print all the ECC errors */
		lmc_sec_err = &sec_errors[0];
		current_sec_error = 0;
	}
	if ((lmc_sec_err->address == fadr_physical) && (lmc_sec_err->bit == bit)) {
		cvmx_safe_printf("\nError: single bit ECC error detected %u times at address 0x%016llx\n", 			 (unsigned int)lmc_sec_err->count,
				 (unsigned long long)fadr_physical);
		cvmx_safe_printf("\n  Previous error data: 0x%016llx\n",
				 (unsigned long long)lmc_sec_err->data);
	}

	/* Re-write the data */
	cvmx_safe_printf("Physical address: 0x%016llx\n",
			 (unsigned long long)fadr_physical);
	fadr_xkphys = fadr_physical | (1ull << 63) | ((phase & 1) << 3);
	fadr_data = cvmx_read64_uint64(fadr_xkphys);
	cvmx_atomic_add64_nosync(CASTPTR(int64_t, fadr_xkphys), 0);

	/* L2 cache hit writeback (no invalidate) */
	CVMX_SYNC;
	CVMX_CACHE_L2HWB(fadr_xkphys & ~0x7f, 0);

	lmc_sec_err->address = fadr_physical & ~0x0f;
	lmc_sec_err->data = fadr_data;
	lmc_sec_err->count++;
	lmc_sec_err->seq = sec_seq++;
	lmc_sec_err->lmc = ddr_controller;
	lmc_sec_err->sec_err = sec_err;
	lmc_sec_err->ded_err = ded_err;
	lmc_sec_err->dimm = fadr.cn73xx.fdimm;
	lmc_sec_err->rank = fadr.cn73xx.fbunk;
	lmc_sec_err->bit = bit;
	lmc_sec_err->syndrome = syndrome;
	lmc_sec_err->used = true;
	num_sec_errors++;
	if (ded_err || lmc_sec_err->count >= SEC_ERROR_PRINT_THRESHOLD) {
		cvmx_safe_printf("\n\nERROR: %s memory ECC error detected\n",
				 ded_err ? "uncorrectable" : "single-bit");
		cvmx_safe_printf("\n\nTotal ECC errors: %u\n",
				 (unsigned int)num_sec_errors);
		cvmx_safe_printf("Errors at this address: %u\n",
				 (unsigned int)lmc_sec_err->count);
		cvmx_safe_printf("LMC%d ECC:\tsec_err:%d ded_err:%d\n"
				 "LMC%d ECC:\tFailing dimm:   %u\n"
				 "LMC%d ECC:\tFailing rank:   %u\n"
				 "LMC%d ECC:\tFailing bank:   %u\n"
				 "LMC%d ECC:\tFailing row:    0x%x\n"
				 "LMC%d ECC:\tFailing column: 0x%x\n"
				 "LMC%d ECC:\tsyndrome: 0x%x, bit: %d\n",
				 ddr_controller, sec_err, ded_err,
				 ddr_controller, fadr.cn73xx.fdimm,
				 ddr_controller, fadr.cn73xx.fbunk,
				 ddr_controller, fadr.cn73xx.fbank,
				 ddr_controller, fadr.cn73xx.frow & rmask,
				 ddr_controller, fadr.cn73xx.fcol & cmask,
				 ddr_controller, syndrome, bit);
		cvmx_safe_printf("LMC%d ECC:\tFailing Address: 0x%016llx\n"
				 "LMC%d ECC:\tFailing Data:    0x%016llx\n",
				 ddr_controller, (unsigned long long)fadr_physical,
				 ddr_controller, (unsigned long long)fadr_data);
		if (sec_err) {
			cvmx_safe_printf("%u ECC errors detected at this bit location and address\n",
					 (unsigned int)lmc_sec_err->count);
		}
	}
	if (ded_err)
		cvmx_safe_printf("\nCannot continue due to corrupted memory!\n"
				 "Please reset or power cycle this system!\n");
	if (ded_err)
		cvmx_octeon_hang();

	return 1;
}

/**
 * Handle L2C TAD interrupts
 *
 * @param	node	CCPI node
 * @param	entry	Error information
 *
 * @return	0 if not handled, 1 if handled
 *
 * TODO: Handle other non-L2D cases
 */
static int __cvmx_cn7xxx_l2c_tad_error_display(int node, struct cvmx_error_78xx *entry)
{
	switch (entry->block_csr_bitpos) {
	case 0:		/* L2DSBE */
	case 1:		/* L2DDBE */
		/* L2 data single and double bit errors */
		return __cvmx_cn7xxx_l2c_l2d_ecc_error_display(node, entry->intsn);
	case 2:		/* SBFSBE store-buffer single-bit error */
	case 4:		/* FBFSBE fill-buffer single-bit error */
		/* Hardware automatically takes care of this, don't report
		 * for now
		 */
		return 0;
	case 3:		/* SBFDBE store-buffer double-bit error */
		cvmx_safe_printf("\nStore buffer double bit error detected, hanging\n");
		cvmx_octeon_hang();
	case 5:		/* FBFDBE fill-buffer double-bit error */
		cvmx_safe_printf("\nFBF double bit error detected, hanging\n");
		cvmx_octeon_hang();
	case 8:		/* TAGSBE tag memory single-bit error */
	case 9:		/* TAGDBE tag memory double-bit error */
	case 10:	/* NOWAY */
		return __cvmx_cn7xxx_l2c_tag_error_display(node, entry->intsn, false);
	case 13:	/* BIGWR */
	case 14:	/* BIGRD */
		/* Should we report this? */
		return 0;
	case 32:	/* RTGSBE Remote-tag single-bit error */
	case 33:	/* RTGDBE Remote-tag double-bit error */
		/* Remote tag memory ECC errors (multi-node) */
		return __cvmx_cn7xxx_l2c_tag_error_display(node, entry->intsn, true);
	default:
		return 0;
	}
}

/**
 * @INTERNAL
 *
 * @param intsn interrupt source number to display
 *
 * @return Zero on success, -1 on error
 */
int cvmx_error_intsn_display_v3(int node, uint32_t intsn) {
	struct cvmx_error_78xx *entry;
	int offs, idx, prev_low, prev_high;
	char msg[128];
	int handled = 0;

	prev_low = 0;
	prev_high = octeon_error_array_current_size - 1;

	idx = prev_high / 2;

	/* Try to do a binary search */
	while (prev_low < prev_high && octeon_error_array_current[idx].intsn != intsn) {
		if (octeon_error_array_current[idx].intsn < intsn) {
			prev_low = idx + 1;
			idx += (prev_high - idx) / 2;
			if (idx < prev_low)
				idx = prev_low;
		} else {
			prev_high = idx - 1;
			idx -= (idx - prev_low) / 2;
			if (idx > prev_high)
				idx = prev_high;
		}
	}
	if (intsn != octeon_error_array_current[idx].intsn) {
		fprintf(stderr, "ERROR: Unknown intsn 0x%x\n", (unsigned int)intsn);
		cvmx_error_intsn_disable_v3(node, intsn);
		return -1;
	}
	entry  = &octeon_error_array_current[idx];
	offs = sprintf(msg, "%d:", node);
	snprintf(msg + offs, sizeof(msg), entry->err_mesg,
		entry->block_csr | ((uint64_t)node << CVMX_NODE_IO_SHIFT), entry->block_csr_bitpos);
	/* LMCX_INT.sec_err is a recoverable error, don't report */
	if ((entry->error_group == CVMX_ERROR_GROUP_LMC) &&	/* LMC ECC errors */
	    (((1ull << entry->block_csr_bitpos) & 0x1fe) == (1ull << entry->block_csr_bitpos)))
		handled = __cvmx_cn7xxx_lmc_ecc_error_record(node, entry);
	else if ((entry->error_group == CVMX_ERROR_GROUP_L2C) && /* L2C ECC errors */
		 (entry->intsn >= 0x80000) && (entry->intsn < 0x80800))
		handled = __cvmx_cn7xxx_l2c_tad_error_display(node, entry);

	if (cvmx_error3_handler_override)
		return cvmx_error3_handler_override(idx, entry);

	if (!handled)
		cvmx_safe_printf("%s\n", msg);

	if (entry->block_csr) {
		uint64_t clear_addr;
		clear_addr = 0x8000000000000000ull | entry->block_csr;
		cvmx_write_csr_node(node, clear_addr,
				    1ull << entry->block_csr_bitpos);
	}
	return 0;
}

/**
 * Initalize the error status system for cn78xx. This should be called once
 * before any other functions are called. This function enables the interrupts
 * described in the array.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_initialize_cn78xx(int node) {
	int i;
	int arrsize;
	int procid = cvmx_get_proc_id();
	struct cvmx_error_array *array = octeon_error_arrays;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;
	for (i = 0; array->array != NULL; i++) {
		if ((procid & array->prid_mask) == array->prid_val) {
			arrsize = cvmx_error_78xx_array_sizes[i];
			break;
		}
		array++;
	}

	if (!array->array) {
		fprintf(stderr, "ERROR: No array found\n");
		return -1;
	}

	octeon_error_array_current = array->array;
	octeon_error_array_current_size = arrsize;

	/* Enable all interrupts described in the array */
	for (i = 0; octeon_error_array_current[i].block_csr != 0; i++)
		if (octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ETHERNET
		    && octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ILA
		    && octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ILK)
			cvmx_error_intsn_enable_v3(node, octeon_error_array_current[i].intsn);

	return 0;
}

/**
 * Enable interrupt for a specific INTSN.
 *
 * @param intsn Interrupt source number
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_intsn_enable_v3(int node, uint32_t intsn) {
	uint64_t addr;
	uint8_t idt = cvmx_get_local_core_num() * 4 + 3;
	cvmx_ciu3_iscx_ctl_t isc_ctl;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	addr = CVMX_CIU3_ISCX_CTL(intsn);
	isc_ctl.u64 = cvmx_read_csr_node(node, addr);
	if (!isc_ctl.s.imp) {
		return -1;
	}
	//if (isc_ctl.s.en)
		/* First disable the entry, in order to be able to write IDT */
	cvmx_error_intsn_disable_v3(node, intsn);
	isc_ctl.u64 = 0;
	isc_ctl.s.idt = idt;
	isc_ctl.s.en = 1;
	cvmx_write_csr_node(node, addr, isc_ctl.u64);
	/* Read back to ensure it was written */
	cvmx_read_csr_node(node, addr);

	return 0;
}

/**
 * Disable interrupt for a specific INTSN.
 *
 * @param intsn Interrupt source number
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_intsn_disable_v3(int node, uint32_t intsn) {
	cvmx_ciu3_iscx_w1c_t w1c;
	uint64_t isc_w1c_addr = CVMX_CIU3_ISCX_W1C(intsn);

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	w1c.u64 = 0;
	w1c.s.en = 1;
	cvmx_write_csr_node(node, isc_w1c_addr, w1c.u64);
	cvmx_read_csr_node(node, isc_w1c_addr);
	return 0;
}

/**
 * Clear interrupt for a specific INTSN.
 *
 * @param intsn Interrupt source number
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_intsn_clear_v3(int node, uint32_t intsn)
{
	uint64_t addr;
	uint8_t idt = cvmx_get_local_core_num() * 4 + 3;
	cvmx_ciu3_iscx_ctl_t isc_ctl;
	cvmx_ciu3_iscx_w1c_t w1c;
	uint64_t isc_w1c_addr = CVMX_CIU3_ISCX_W1C(intsn);

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	addr = CVMX_CIU3_ISCX_CTL(intsn);
	isc_ctl.u64 = cvmx_read_csr_node(node, addr);
	if (!isc_ctl.s.imp) {
		return -1;
	}
	w1c.u64 = 0;
	w1c.s.raw = 1;
	cvmx_write_csr_node(node, isc_w1c_addr, w1c.u64);
	cvmx_read_csr_node(node, isc_w1c_addr);

	isc_ctl.u64 = 0;
	isc_ctl.s.idt = idt;
	isc_ctl.s.en = 1;
	cvmx_write_csr_node(node, addr, isc_ctl.u64);
	/* Read back to ensure it was written */
	cvmx_read_csr_node(node, addr);

	return 0;
}

/**
 * Enable interrupts for a specific CSR(all the bits/intsn in the csr).
 *
 * @param csr_address CSR address
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_csr_enable_v3(int node, uint64_t csr_address)
{
	int i, ret, flag = 0;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (csr_address != octeon_error_array_current[i].block_csr) {
			if (flag) /* If we pass that CSR address - break */
				break;
			continue;
		}

		flag = 1;
		ret = cvmx_error_intsn_enable_v3(node, octeon_error_array_current[i].intsn);
		if (0 != ret)
			return ret;
	}

	return 0;
}

/**
 * Disable interrupts for a specific CSR (all the bits/intsn in the csr).
 *
 * @param csr_address CSR address
 *
 * @return Zero
 */
int cvmx_error_csr_disable_v3(int node, uint64_t csr_address)
{
	int i, flag = 0;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (csr_address != octeon_error_array_current[i].block_csr) {
			if (flag) /* If we pass that CSR address - break */
				break;
			continue;
		}

		flag = 1;
		cvmx_error_intsn_disable_v3(node, octeon_error_array_current[i].intsn);
	}

	return 0;
}

/* Wrapper function to enable/disable/clear error interrupts
 *
 * @param index  Index of the error array
 * @param group  Logical group to enable
 * @param xipd_port  The IPD port value
 * @param mode   the operation to perform
 *               0 - enable
 *               1 - disable
 *               2 - clear
 */
static int __cvmx_error_wrapper(int index, cvmx_error_group_t group, int xipd_port, int mode)
{
	int node;
	cvmx_xport_t r = cvmx_helper_ipd_port_to_xport(xipd_port);
	node = r.node;
	if (octeon_error_array_current[index].error_group == CVMX_ERROR_GROUP_ETHERNET) {
		int xiface = cvmx_helper_get_interface_num(xipd_port);
		int index = cvmx_helper_get_interface_index_num(xipd_port);
		struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);
		unsigned long long int match_ifindex = xi.interface*0x1000000 + index*0x100000;
		node = xi.node;

		/* Skip the loopback interface */
		if (octeon_error_array_current[index].block_csr == (CVMX_LBK_INT & ~(1ull << 63)))
			return 0;
		if ((octeon_error_array_current[index].block_csr & 0xff00000ull) != match_ifindex)
			return 0;
	}
	if (octeon_error_array_current[index].error_group == CVMX_ERROR_GROUP_ILK) {
		int intf = (xipd_port >> 2) & 0x3;
		int lane_mask = (xipd_port >> 0x4) & 0xffff;
		int intsn = octeon_error_array_current[index].intsn;
		int range = ((intsn >> 0x8) & 0xf);
		node = xipd_port & 0x3;
		/* Mask off intf bit from intsn */
		intsn &= ~(0xf << 0x8);
		/* For ILK_[RXx|TXx]_INT, enable interrupts per interface */
		if ((intsn >= 0x14040) && (intsn < 0x140c0) && (range != intf))
			return 0;

		/* For ILK_RX_LNEx_INT, enable interrupts for lanes used on an interface */
		else if (!(lane_mask & (1 << range)) && (intsn >= 0x140c0) && (intsn <= 0x140c9))
			return 0;
	}
	if (mode == 0)
		cvmx_error_intsn_enable_v3(node, octeon_error_array_current[index].intsn);
	else if (mode == 1)
		cvmx_error_intsn_disable_v3(node, octeon_error_array_current[index].intsn);
	else
		cvmx_error_intsn_clear_v3(node, octeon_error_array_current[index].intsn);

	return 0;
}

/**
 * Enable all error registers for a logical group. This should be
 * called whenever a logical group is brought online.
 *
 * @param group  Logical group to enable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_enable_group_v3(cvmx_error_group_t group, int xipd_port)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	if (cvmx_error3_enable_group_override)
		return cvmx_error3_enable_group_override(group, xipd_port);

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].error_group == group) {
			__cvmx_error_wrapper(i, group, xipd_port, 0);
		}
	}

	return 0;
}

/**
 * Disable all error registers for a logical group.
 *
 * @param group  Logical group to disable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_disable_group_v3(cvmx_error_group_t group, int xipd_port)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	if (cvmx_error3_disable_group_override)
		return cvmx_error3_disable_group_override(group, xipd_port);

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].error_group == group) {
			__cvmx_error_wrapper(i, group, xipd_port, 1);
		}
	}

	return 0;
}

/**
 * Clear all error registers for a logical group.
 *
 * @param group  Logical group to disable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_clear_group_v3(cvmx_error_group_t group, int xipd_port)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].error_group == group) {
			__cvmx_error_wrapper(i, group, xipd_port, 2);
		}
	}

	return 0;
}

/**
 * Enable all error registers for a specific category in a logical group.
 * This should be called whenever a logical group is brought online.
 *
 * @param group  Logical group to enable
 * @param type   Category in a logical group to enable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_enable_group_type_v3(cvmx_error_group_t group,
				    cvmx_error_type_t type, int xipd_port)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	/* If requested to enable all errors, then call standard APIs. */
	if (type == 0)
		return cvmx_error_enable_group_v3(group, xipd_port);

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if ((octeon_error_array_current[i].error_group == group)
		    && (octeon_error_array_current[i].flags == type)) {
			__cvmx_error_wrapper(i, group, xipd_port, 0);
		}
	}

	return 0;
}

/**
 * Disable all error registers for a specific category in a logical group.
 * This should be called whenever a logical group is brought online.
 *
 * @param group  Logical group to disable
 * @param type   Category in a logical group to disable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_disable_group_type_v3(cvmx_error_group_t group,
				     cvmx_error_type_t type, int xipd_port)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	/* If requested to enable all errors, then call standard APIs. */
	if (type == 0)
		return cvmx_error_disable_group_v3(group, xipd_port);

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if ((octeon_error_array_current[i].error_group == group)
		    && (octeon_error_array_current[i].flags == type)) {
			__cvmx_error_wrapper(i, group, xipd_port, 1);
		}
	}

	return 0;
}

/**
 * Enable all error registers for a particular category.
 *
 * @param node  CCPI node
 * @param type  category to enable
 *
 * @return Zero.
 */
int cvmx_error_enable_type_v3(int node, cvmx_error_type_t type)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].flags == type) {
			cvmx_error_intsn_enable_v3(node, octeon_error_array_current[i].intsn);
		}
	}

	return 0;
}

/**
 * Disable all error registers for a particular category.
 *
 * @param node  CCPI node
 * @param type  category to enable
 *
 * @return Zero.
 */
int cvmx_error_disable_type_v3(int node, cvmx_error_type_t type)
{
	int i;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].flags == type) {
			cvmx_error_intsn_disable_v3(node, octeon_error_array_current[i].intsn);
		}
	}

	return 0;
}
