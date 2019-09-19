/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com). All rights
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
 * Prototypes for custom error handler function not handled by the default
 * message display error function.
 *
 * <hr>$Revision: 44252 $<hr>
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-error.h>
#include <asm/octeon/cvmx-error-custom.h>
#include <asm/octeon/cvmx-helper.h>
#include <asm/octeon/cvmx-l2c.h>
#include <asm/octeon/cvmx-pcie.h>
#include <asm/octeon/cvmx-ciu-defs.h>
#include <asm/octeon/cvmx-pexp-defs.h>
#include <asm/octeon/cvmx-dfa-defs.h>
#include <asm/octeon/cvmx-gmxx-defs.h>
#include <asm/octeon/cvmx-lmcx-defs.h>
#include <asm/octeon/cvmx-pemx-defs.h>
#include <asm/octeon/cvmx-sriox-defs.h>
#include <asm/octeon/cvmx-helper-cfg.h>
#define PRINT_ERROR(format, ...) cvmx_safe_printf("ERROR " format, ##__VA_ARGS__)
#else
#include "cvmx.h"
#include "cvmx-ciu-defs.h"
#include "cvmx-error.h"
#include "cvmx-error-custom.h"
#include "cvmx-helper.h"
#include "cvmx-l2c.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-pcie.h"
#include "cvmx-interrupt.h"
#include "cvmx-helper-cfg.h"
#endif


/**
 * @INTERNAL
 * When NPEI_INT_SUM[C0_LDWN] is set, the PCIe block requires a shutdown and
 * initialization to bring the link back up. This handler does this for port 0.
 * Note that config space is not enumerated again, so the devices will still be
 * unusable.
 *
 * @param info
 *
 * @return
 */
static int __cvmx_error_handle_npei_int_sum_c0_ldwn(const struct cvmx_error_info *info)
{
	cvmx_ciu_soft_prst_t ciu_soft_prst;
	PRINT_ERROR("NPEI_INT_SUM[C0_LDWN]: Reset request due to link0 down status.\n");
	ciu_soft_prst.u64 = cvmx_read_csr(CVMX_CIU_SOFT_PRST);
	if (!ciu_soft_prst.s.soft_prst) {
		/* Attempt to automatically bring the link back up */
		cvmx_pcie_rc_shutdown(0);
		cvmx_pcie_rc_initialize(0);
	}
	cvmx_write_csr(CVMX_PEXP_NPEI_INT_SUM, cvmx_read_csr(CVMX_PEXP_NPEI_INT_SUM));
	return 1;
}

/**
 * @INTERNAL
 * When NPEI_INT_SUM[C1_LDWN] is set, the PCIe block requires a shutdown and
 * initialization to bring the link back up. This handler does this for port 1.
 * Note that config space is not enumerated again, so the devices will still be
 * unusable.
 *
 * @param info
 *
 * @return
 */
static int __cvmx_error_handle_npei_int_sum_c1_ldwn(const struct cvmx_error_info *info)
{
	cvmx_ciu_soft_prst_t ciu_soft_prst;
	PRINT_ERROR("NPEI_INT_SUM[C1_LDWN]: Reset request due to link1 down status.\n");
	ciu_soft_prst.u64 = cvmx_read_csr(CVMX_CIU_SOFT_PRST1);
	if (!ciu_soft_prst.s.soft_prst) {
		/* Attempt to automatically bring the link back up */
		cvmx_pcie_rc_shutdown(1);
		cvmx_pcie_rc_initialize(1);
	}
	cvmx_write_csr(CVMX_PEXP_NPEI_INT_SUM, cvmx_read_csr(CVMX_PEXP_NPEI_INT_SUM));
	return 1;
}

#define DECODE_FAILING_ADDRESS
/* #define DECODE_FAILING_BIT */

#ifdef DECODE_FAILING_BIT
#define _Db(x) (x)		/* Data Bit */
#define _Ec(x) (0x100+x)	/* ECC Bit */
#define _Ad(x) (0x200+x)	/* Address Bit */
#define _Bu(x) (0x400+x)	/* Burst */
#define _Un()  (-1)		/* Unused */
/* Use ECC Code as index to lookup corrected bit */
const static short lmc_syndrome_bits[256] = {
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
#endif

/**
 * @INTERNAL
 * This error bit handler clears the status and prints failure infomation.
 *
 * @param info   Error register to check
 *
 * @return
 */
static int __cvmx_cn6xxx_lmc_ecc_error_display(const cvmx_error_info_t * info)
{
#ifdef DECODE_FAILING_ADDRESS
	cvmx_lmcx_config_t lmc_config;
	cvmx_lmcx_control_t lmc_control;
	uint64_t fadr_physical, fadr_data;
	int interface_bits = 0;
	int row_bits;
	int col_bits;
	int rmask;
	int cmask;
	int pbank_lsb;
	int row_lsb;
#endif

	int ddr_controller = info->group_index;
	cvmx_lmcx_int_t lmc_int;
	cvmx_lmcx_fadr_t fadr;
	cvmx_lmcx_ecc_synd_t ecc_synd;
	int sec_err;
	int ded_err;
	int syndrome = -1;
	int phase;
	int i;

	lmc_int.u64 = cvmx_read_csr(CVMX_LMCX_INT(ddr_controller));
	fadr.u64 = cvmx_read_csr(CVMX_LMCX_FADR(ddr_controller));
	ecc_synd.u64 = cvmx_read_csr(CVMX_LMCX_ECC_SYND(ddr_controller));

#ifdef DECODE_FAILING_ADDRESS
	lmc_config.u64 = cvmx_read_csr(CVMX_LMCX_CONFIG(ddr_controller));
	lmc_control.u64 = cvmx_read_csr(CVMX_LMCX_CONTROL(ddr_controller));
#endif

	sec_err = lmc_int.s.sec_err;
	ded_err = lmc_int.s.ded_err;

	phase = ded_err ? ded_err : sec_err;	/* Double bit errors take precedence. */

	/* The err mask could contain multiple failures. Detect the first
	   failing 64b chunk to convert the mask into the phase that
	   corresponds to the information in FADR. */
	for (i = 0; i < 4; ++i) {
		if ((phase >> i) & 1)
			break;
	}
	phase = i;

	switch (phase) {
	case 0:
		syndrome = ecc_synd.cn63xx.mrdsyn0;
		break;
	case 1:
		syndrome = ecc_synd.cn63xx.mrdsyn1;
		break;
	case 2:
		syndrome = ecc_synd.cn63xx.mrdsyn2;
		break;
	case 3:
		syndrome = ecc_synd.cn63xx.mrdsyn3;
		break;
	}

#ifdef DECODE_FAILING_ADDRESS
	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		/* Detect four-lmc mode. */
		cvmx_lmcx_dll_ctl2_t ctl;
		ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(3));
		interface_bits = 1 + ctl.cn68xx.intf_en;
	}

	pbank_lsb = lmc_config.s.pbank_lsb + 28;
	row_lsb = lmc_config.s.row_lsb + 14;

	col_bits = row_lsb - 3 - 3;
	row_bits = pbank_lsb - row_lsb - lmc_config.s.rank_ena;

	cmask = ~(-1 << col_bits);
	rmask = ~(-1 << row_bits);

	fadr_physical = (uint64_t) ddr_controller << 7;	/* Which controller? Always 0 except cn68xx */
	fadr_physical |= (uint64_t) fadr.cn63xx.fdimm << (pbank_lsb + interface_bits);
	fadr_physical |= (uint64_t) fadr.cn63xx.fbunk << (pbank_lsb + interface_bits - 1);
	fadr_physical |= (uint64_t) (fadr.cn63xx.frow & rmask) << (row_lsb + interface_bits);
	fadr_physical |= (uint64_t) (fadr.cn63xx.fcol & 0xf) << 3;
	fadr_physical |= (uint64_t) ((fadr.cn63xx.fcol & cmask) >> 4) << (10 + interface_bits);
	if (lmc_control.s.xor_bank)
		fadr_physical |= (uint64_t) (fadr.cn63xx.fbank ^ ((fadr_physical >> (12 + interface_bits)) & 7)) << (7 + interface_bits);
	else
		fadr_physical |= (uint64_t) fadr.cn63xx.fbank << (7 + interface_bits);
	fadr_physical |= (uint64_t) (phase & 1) << 3;

	fadr_physical += (fadr_physical < (uint64_t) 0x10000000) ? 0 : (uint64_t) 0x10000000;	/* Account for 256MB hole */

	PRINT_ERROR("PBANK_LSB: %d, ROW_LSB: %d, Row bits: %d, Col bits: %d, Row mask: 0x%x, Col mask: 0x%x\n",
		    lmc_config.s.pbank_lsb, lmc_config.s.row_lsb, row_bits, col_bits, rmask, cmask);

	//CVMX_DCACHE_INVALIDATE;
	fadr_data = *(uint64_t *) cvmx_phys_to_ptr(fadr_physical);
#endif

	PRINT_ERROR("LMC%d ECC: sec_err:%d ded_err:%d\n"
		    "LMC%d ECC:\tFailing dimm:   %u\n"
		    "LMC%d ECC:\tFailing rank:   %u\n"
		    "LMC%d ECC:\tFailing bank:   %u\n" "LMC%d ECC:\tFailing row:    0x%x\n" "LMC%d ECC:\tFailing column: 0x%x\n" "LMC%d ECC:\tsyndrome: 0x%x"
#ifdef DECODE_FAILING_BIT
		    ", bit: %d"
#endif
		    "\n"
#ifdef DECODE_FAILING_ADDRESS
		    "Failing  Address: 0x%016llx, Data: 0x%016llx\n"
#endif
		    ,		/* Comma */
		    ddr_controller, sec_err, ded_err,
		    ddr_controller, fadr.cn63xx.fdimm,
		    ddr_controller, fadr.cn63xx.fbunk,
		    ddr_controller, fadr.cn63xx.fbank, ddr_controller, fadr.cn63xx.frow, ddr_controller, fadr.cn63xx.fcol, ddr_controller, syndrome
#ifdef DECODE_FAILING_BIT
		    ,		/* Comma */
		    lmc_syndrome_bits[syndrome]
#endif
#ifdef DECODE_FAILING_ADDRESS
		    ,		/* Comma */
		    (unsigned long long)fadr_physical, (unsigned long long)fadr_data
#endif
	    );

	/*
	 * To clear LMC interrupts, first clear interrupt bit in CIU_CIB
	 * followed by the interrupt bit in LMC interrupt CSR.
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		cvmx_write_csr(CVMX_LMCX_INT(ddr_controller), info->status_mask);

	/* This assumes that all bits in the status register are RO or R/W1C */
	cvmx_write_csr(info->status_addr, info->status_mask);
	cvmx_read_csr(info->status_addr);

	if (ded_err) {
		cvmx_safe_printf("\nCannot continue due to corrupted memory!\n"
				 "Please reset or power cycle this system!\n");
		cvmx_octeon_hang();
	}

	return 1;
}

/**
 * @INTERNAL
 * Some of the CIU_CIB error interrupts require clearing first the CIU_CIB
 * interrupt and then clearing the interrupt bit in that block.
 */
static int __cvmx_cn70xx_ciu_cib_error_display(const cvmx_error_info_t * info)
{
	const char *message = (const char *)(long)info->user_info;
	int handled = 0;
	int tad = 0;

	if (info->status_addr == CVMX_CIU_CIB_L2C_RAWX(tad)) {
		cvmx_l2c_tadx_int_t tad_int;
		tad_int.u64 = cvmx_read_csr(CVMX_L2C_TADX_INT(tad));

		/* L2 data single and double bit errors */
		if (tad_int.cn70xx.l2dsbe || tad_int.cn70xx.l2ddbe)
			handled = __cvmx_cn7xxx_l2c_l2d_ecc_error_display(0, tad);
		/* Hardware automatically takes care of this, don't report for now */
		if (tad_int.cn70xx.sbfsbe || tad_int.cn70xx.fbfsbe)
			handled = 0;

		if (tad_int.cn70xx.sbfdbe) {
			cvmx_safe_printf("\nStore buffer double bit error detected, hanging\n");
			cvmx_octeon_hang();
		}

		/* Handle L2C Tag errors */
		if (tad_int.cn70xx.tagsbe || tad_int.cn70xx.tagdbe || tad_int.cn70xx.noway)
			handled = __cvmx_cn7xxx_l2c_tag_error_display(0, tad, false);

		/* should be report this? */
		if (tad_int.cn70xx.bigrd || tad_int.cn70xx.bigwr)
			handled = 0;

		/* Remote tag memory ECC errors (multi-node) */
		if (tad_int.cn70xx.rtgsbe || tad_int.cn70xx.rtgdbe)
			handled = __cvmx_cn7xxx_l2c_tag_error_display(0, tad, true);

		if (!handled)
			cvmx_safe_printf("%s", message);

		/* Clear error interrupt from CIU_CIB before clearing L2C_TADX_INT */
		cvmx_write_csr(info->status_addr, info->status_mask);
		/* Read to make sure write is complete */
		cvmx_read_csr(info->status_addr);
		/* Clear L2C_TADX_INT interrupts */
		cvmx_write_csr(CVMX_L2C_TADX_INT(tad), tad_int.u64);
	} else if (info->status_addr == CVMX_CIU_CIB_RST_RAWX(tad)) {
		cvmx_safe_printf("%s", message);
		/* Clear error interrupt from CIU_CIB before clearing RST_INT */
		cvmx_write_csr(info->status_addr, info->status_mask);
		/* Read to make sure write is complete */
		cvmx_read_csr(info->status_addr);
		cvmx_write_csr(CVMX_RST_INT, info->status_mask);
	}

	return 0;
}

/*
 * @INTERNAL
 * Handle L2C L2DSBE and L2DDBE error interrupts
 *
 * @param info: Error information
 *
 * @return      0 is not handled, 1 if handled
 */
static int __cvmx_error_handle_6xxx_l2c_l2d_ecc_error_display(const struct cvmx_error_info *info)
{
	cvmx_l2c_err_tdtx_t tdtx;
	cvmx_l2c_tadx_int_t tadx_int;
	bool dbe = false;

	tadx_int.u64 = cvmx_read_csr(CVMX_L2C_TADX_INT(info->group_index));
	tdtx.u64 = cvmx_read_csr(CVMX_L2C_ERR_TDTX(info->group_index));

	if (tdtx.s.dbe || tdtx.s.vdbe)
		dbe = true;
	else if (!tdtx.s.sbe) {
		cvmx_safe_printf("Unknown L2 cache error detected\n"
				 "L2D double-bit L2D error: %d\n"
				 "L2D double-bit VBF error: %d\n"
				 "L2D single-bit L2D error: %d\n"
				 "L2D single-bit VBF error: %d\n",
				 tdtx.s.dbe, tdtx.s.vdbe, tdtx.s.sbe, tdtx.s.vsbe);
	}

	cvmx_safe_printf("L2 cache %s-bit ECC error detected\n",
			 dbe ? "double" : "single");

	cvmx_safe_printf("TAD: %d, L2D wayindex: 0x%x, error syndrome: 0x%x\n",
			 info->group_index, tdtx.s.wayidx, tdtx.s.syn);

	if (tdtx.s.vsbe || tdtx.s.vdbe) {
		cvmx_l2c_err_vbfx_t vbfx;
		vbfx.u64 = cvmx_read_csr(CVMX_L2C_ERR_VBFX(info->group_index));
		cvmx_safe_printf("VBF error syndrome: 0x%x\n", vbfx.s.vsyn);
	}

	if (dbe) {
		cvmx_safe_printf("\nCannot continue due to uncorrectable cache line!\n"
				 "Please reset or power cycle this system!\n");
		cvmx_octeon_hang();
	}

	/* Write back and invalidate the cache line */
	CVMX_SYNCW;
	CVMX_CACHE_WBIL2I((tdtx.s.wayidx) | (1ULL << 63), 0);

	/* Now clear the interrupt */
	cvmx_write_csr(CVMX_L2C_TADX_INT(info->group_index), tadx_int.u64);
	/* Read back to make sure the write happened */
	cvmx_read_csr(CVMX_L2C_TADX_INT(info->group_index));

	return 1;
}

/**
 * @INTERNAL
 * Handle L2C TAG SBE/DBE error interrupts
 *
 * @param info: Error information
 *
 * @return      0 is not handled, 1 if handled
 */
static int __cvmx_error_handle_6xxx_l2c_tag_error_display(const struct cvmx_error_info *info)
{
	cvmx_l2c_err_ttgx_t ttgx;
	cvmx_l2c_tadx_int_t tadx_int;
	bool dbe = false;

	tadx_int.u64 = cvmx_read_csr(CVMX_L2C_TADX_INT(info->group_index));
	ttgx.u64 = cvmx_read_csr(CVMX_L2C_ERR_TTGX(info->group_index));

	if (ttgx.s.dbe)
		dbe = true;

	cvmx_safe_printf("L2 cache %s-bit ECC error detected\n",
			 dbe ? "double" : "single");
	cvmx_safe_printf("TAD: %d, L2D index: 0x%x, way: 0x%x, error syndrome: 0x%x\n",
			 info->group_index, (ttgx.s.wayidx >> 7),
			 (ttgx.s.wayidx & 0x7f), ttgx.s.syn);

	if (dbe) {
		cvmx_safe_printf("\nCannot continue due to uncorrectable cache line!\n"
				 "Please reset or power cycle this system!\n");
		cvmx_octeon_hang();
	}

	CVMX_SYNCW;
	CVMX_CACHE_WBIL2I((ttgx.s.wayidx) | (1ULL << 63), 0);
	CVMX_SYNC;

	/* Clear error interrupts */
	cvmx_write_csr(CVMX_L2C_TADX_INT(info->group_index), tadx_int.u64);
	/* Read back to make sure the write happened */
	cvmx_read_csr(CVMX_L2C_TADX_INT(info->group_index));

	return 1;
}

/**
 * @INTERNAL
 * Some errors require more complicated error handing functions than the
 * automatically generated functions in cvmx-error-init-*.c. This function
 * replaces these handers with hand coded functions for these special cases.
 *
 * @return Zero on success, negative on failure.
 */
int __cvmx_error_custom_initialize(void)
{
	if (OCTEON_IS_OCTEON2()) {
		int lmc;
		for (lmc = 0; lmc < CVMX_L2C_TADS; lmc++) {
			if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
				cvmx_lmcx_dll_ctl2_t ctl;
				ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(lmc));
				if (ctl.cn68xx.intf_en == 0)
					continue;
			}
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64, CVMX_LMCX_INT(lmc), 0xfull << 1 /* sec_err */ ,
						  __cvmx_cn6xxx_lmc_ecc_error_display, 0, NULL, NULL);
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64, CVMX_LMCX_INT(lmc), 0xfull << 5 /* ded_err */ ,
						  __cvmx_cn6xxx_lmc_ecc_error_display, 0, NULL, NULL);
			if (!OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X)) {
				int i;

				for (i = 0; i < 6; i++) {
					switch (i) {
					case 0:    /* L2DSBE */
					case 1:    /* L2DDBE */
					case 4: /* VBFSBE */
					case 5: /* VBFDBE */
						cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
							CVMX_L2C_TADX_INT(lmc), (1ull << i) ,
				                	__cvmx_error_handle_6xxx_l2c_l2d_ecc_error_display,
					        	0, NULL, NULL);
						break;
					case 2: /* TAGSBE */
					case 3: /* TAGDBE */
						cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
							CVMX_L2C_TADX_INT(lmc), (1ull << i),
				                	__cvmx_error_handle_6xxx_l2c_tag_error_display,
					        	0, NULL, NULL);
						break;
					default:
						break;
					}
				}
			}
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
				CVMX_CIU_CIB_LMCX_RAWX(0, 0), 0xfull << 1 /* sec_err */ ,
				__cvmx_cn6xxx_lmc_ecc_error_display, 0, NULL, NULL);
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
				CVMX_CIU_CIB_LMCX_RAWX(0, 0), 0xfull << 5 /* ded_err */ ,
				__cvmx_cn6xxx_lmc_ecc_error_display, 0, NULL, NULL);
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
				CVMX_CIU_CIB_L2C_RAWX(0), 0x7fffff /* TAD/MCI/CBC */ ,
				__cvmx_cn70xx_ciu_cib_error_display, 0, NULL, NULL);
			cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64,
				CVMX_CIU_CIB_RST_RAWX(0), 0x3f /* RST */ ,
				__cvmx_cn70xx_ciu_cib_error_display, 0, NULL, NULL);
	} else if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64, CVMX_PEXP_NPEI_INT_SUM, 1ull << 59 /* c0_ldwn */ ,
					  __cvmx_error_handle_npei_int_sum_c0_ldwn, 0, NULL, NULL);
		cvmx_error_change_handler(CVMX_ERROR_REGISTER_IO64, CVMX_PEXP_NPEI_INT_SUM, 1ull << 60 /* c1_ldwn */ ,
					  __cvmx_error_handle_npei_int_sum_c1_ldwn, 0, NULL, NULL);
	}

	/* CN63XX pass 1.x has a bug where the PCIe config CRS counter does not
	   stop. Disable reporting errors from CRS */
	if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X)) {
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_PEMX_INT_SUM(0), 1ull << 12);
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_PEMX_INT_SUM(0), 1ull << 13);
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_PEMX_INT_SUM(1), 1ull << 12);
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_PEMX_INT_SUM(1), 1ull << 13);
	}
	/* According to the workaround for errata SRIO-15282, clearing
	   SRIOx_INT_ENABLE[MAC_BUF]. */
	if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS2_0) && OCTEON_IS_MODEL(OCTEON_CN63XX_PASS2_1)) {
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_SRIOX_INT_ENABLE(0), 1ull << 22);
		cvmx_error_disable(CVMX_ERROR_REGISTER_IO64, CVMX_SRIOX_INT_ENABLE(1), 1ull << 22);
	}

	return 0;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_cp2dbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[CP2DBE]: DFA PP-CP2 Double Bit Error Detected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_cp2perr(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[CP2PERR]: PP-CP2 Parity Error Detected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_cp2sbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[CP2SBE]: DFA PP-CP2 Single Bit Error Corrected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_dblovf(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[DBLOVF]: Doorbell Overflow detected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_dtedbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[DTEDBE]: DFA DTE 29b Double Bit Error Detected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_dteperr(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[DTEPERR]: DTE Parity Error Detected\n");
	return 1;
}

/**
 * @INTERNAL
 * DFA_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_dfa_err_dtesbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_DFA_ERR, cvmx_read_csr(CVMX_DFA_ERR));
	PRINT_ERROR("DFA_ERR[DTESBE]: DFA DTE 29b Single Bit Error Corrected\n");
	return 1;
}

/**
 * @INTERNAL
 * L2D_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2d_err_ded_err(const struct cvmx_error_info *info)
{
	cvmx_l2d_err_t derr;
	cvmx_l2d_fadr_t fadr;
	uint64_t syn0 = cvmx_read_csr(CVMX_L2D_FSYN0);
	uint64_t syn1 = cvmx_read_csr(CVMX_L2D_FSYN1);
	derr.u64 = cvmx_read_csr(CVMX_L2D_ERR);
	fadr.u64 = cvmx_read_csr(CVMX_L2D_FADR);

	PRINT_ERROR("L2D_ERR[DED_ERR] ECC double: fadr: 0x%llx, syn0:0x%llx, syn1: 0x%llx\n", (unsigned long long)fadr.u64, (unsigned long long)syn0, (unsigned long long)syn1);
	/* Flush the line that had the error */
	cvmx_l2c_flush_line(fadr.s.fset, fadr.s.fadr >> 1);
	cvmx_write_csr(CVMX_L2D_ERR, derr.u64);
	return 1;
}

/**
 * @INTERNAL
 * L2D_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2d_err_sec_err(const struct cvmx_error_info *info)
{
	cvmx_l2d_err_t derr;
	cvmx_l2d_fadr_t fadr;
	uint64_t syn0 = cvmx_read_csr(CVMX_L2D_FSYN0);
	uint64_t syn1 = cvmx_read_csr(CVMX_L2D_FSYN1);
	derr.u64 = cvmx_read_csr(CVMX_L2D_ERR);
	fadr.u64 = cvmx_read_csr(CVMX_L2D_FADR);

	PRINT_ERROR("L2D_ERR[SEC_ERR] ECC single: fadr: 0x%llx, syn0:0x%llx, syn1: 0x%llx\n", (unsigned long long)fadr.u64, (unsigned long long)syn0, (unsigned long long)syn1);
	/* Flush the line that had the error */
	cvmx_l2c_flush_line(fadr.s.fset, fadr.s.fadr >> 1);
	cvmx_write_csr(CVMX_L2D_ERR, derr.u64);
	return 1;
}

/**
 * @INTERNAL
 * L2T_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2t_err_ded_err(const struct cvmx_error_info *info)
{
	cvmx_l2t_err_t terr;
	terr.u64 = cvmx_read_csr(CVMX_L2T_ERR);
	cvmx_write_csr(CVMX_L2T_ERR, terr.u64);
	PRINT_ERROR("L2T_ERR[DED_ERR]: double bit:\tfadr: 0x%x, fset: 0x%x, fsyn: 0x%x\n", terr.s.fadr, terr.s.fset, terr.s.fsyn);
	if (!terr.s.fsyn) {
		/* Syndrome is zero, which means error was in non-hit line,
		   so flush all associations */
		int i;
		int l2_assoc = cvmx_l2c_get_num_assoc();

		for (i = 0; i < l2_assoc; i++)
			cvmx_l2c_flush_line(i, terr.s.fadr);
	} else
		cvmx_l2c_flush_line(terr.s.fset, terr.s.fadr);
	return 1;
}

/**
 * @INTERNAL
 * L2T_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2t_err_lckerr2(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_L2T_ERR, cvmx_read_csr(CVMX_L2T_ERR));
	PRINT_ERROR("L2T_ERR[LCKERR2]: HW detected a case where a Rd/Wr Miss from PP#n could not find an available/unlocked set (for replacement).\n");
	return 1;
}

/**
 * @INTERNAL
 * L2T_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2t_err_lckerr(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_L2T_ERR, cvmx_read_csr(CVMX_L2T_ERR));
	PRINT_ERROR("L2T_ERR[LCKERR]: SW attempted to LOCK DOWN the last available set of the INDEX (which is ignored by HW - but reported to SW).\n");
	return 1;
}

/**
 * @INTERNAL
 * L2T_ERR contains R/W1C bits along with R/W bits. This means that it requires
 * special handling instead of the normal __cvmx_error_display() function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_l2t_err_sec_err(const struct cvmx_error_info *info)
{
	cvmx_l2t_err_t terr;
	terr.u64 = cvmx_read_csr(CVMX_L2T_ERR);
	cvmx_write_csr(CVMX_L2T_ERR, terr.u64);
	PRINT_ERROR("L2T_ERR[SEC_ERR]: single bit:\tfadr: 0x%x, fset: 0x%x, fsyn: 0x%x\n", terr.s.fadr, terr.s.fset, terr.s.fsyn);
	if (!terr.s.fsyn) {
		/* Syndrome is zero, which means error was in non-hit line,
		   so flush all associations */
		int i;
		int l2_assoc = cvmx_l2c_get_num_assoc();

		for (i = 0; i < l2_assoc; i++)
			cvmx_l2c_flush_line(i, terr.s.fadr);
	} else
		cvmx_l2c_flush_line(terr.s.fset, terr.s.fadr);
	return 1;
}

/**
 * @INTERNAL
 * LMCX_MEM_CFG0 contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
static int __cvmx_error_handle_lmcx_mem_cfg0(const struct cvmx_error_info *info)
{
	int ddr_controller = info->group_index;
	cvmx_lmcx_mem_cfg0_t mem_cfg0;
	cvmx_lmcx_fadr_t fadr;
	int sec_err;
	int ded_err;

	mem_cfg0.u64 = cvmx_read_csr(CVMX_LMCX_MEM_CFG0(ddr_controller));
	fadr.u64 = cvmx_read_csr(CVMX_LMCX_FADR(ddr_controller));
	cvmx_write_csr(CVMX_LMCX_MEM_CFG0(ddr_controller), mem_cfg0.u64);

	sec_err = cvmx_dpop(mem_cfg0.s.sec_err);
	ded_err = cvmx_dpop(mem_cfg0.s.ded_err);

	if (ded_err || sec_err) {
		PRINT_ERROR("DDR%d ECC: %d Single bit corrections, %d Double bit errors\n"
			    "DDR%d ECC:\tFailing dimm:   %u\n"
			    "DDR%d ECC:\tFailing rank:   %u\n"
			    "DDR%d ECC:\tFailing bank:   %u\n"
			    "DDR%d ECC:\tFailing row:    0x%x\n"
			    "DDR%d ECC:\tFailing column: 0x%x\n",
			    ddr_controller, sec_err, ded_err,
			    ddr_controller, fadr.cn38xx.fdimm,
			    ddr_controller, fadr.cn38xx.fbunk, ddr_controller, fadr.cn38xx.fbank, ddr_controller, fadr.cn38xx.frow, ddr_controller, fadr.cn38xx.fcol);
	}
	return 1;
}

/**
 * @INTERNAL
 * LMCX_MEM_CFG0 contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_lmcx_mem_cfg0_ded_err(const struct cvmx_error_info *info)
{
	return __cvmx_error_handle_lmcx_mem_cfg0(info);
}

/**
 * @INTERNAL
 * LMCX_MEM_CFG0 contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_lmcx_mem_cfg0_sec_err(const struct cvmx_error_info *info)
{
	return __cvmx_error_handle_lmcx_mem_cfg0(info);
}

/**
 * @INTERNAL
 * POW_ECC_ERR contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_pow_ecc_err_dbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_POW_ECC_ERR, cvmx_read_csr(CVMX_POW_ECC_ERR));
	PRINT_ERROR("POW_ECC_ERR[DBE]: POW double bit error\n");
	return 1;
}

/**
 * @INTERNAL
 * POW_ECC_ERR contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_pow_ecc_err_iop(const struct cvmx_error_info *info)
{
	cvmx_pow_ecc_err_t err;
	err.u64 = cvmx_read_csr(CVMX_POW_ECC_ERR);
	cvmx_write_csr(CVMX_POW_ECC_ERR, err.u64);
	if (err.s.iop & (1 << 0))
		PRINT_ERROR("POW_ECC_ERR[IOP0]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH/DESCH/UPD_WQP from PP in NULL_NULL state\n");
	if (err.s.iop & (1 << 1))
		PRINT_ERROR("POW_ECC_ERR[IOP1]: Received SWTAG/SWTAG_DESCH/DESCH/UPD_WQP from PP in NULL state\n");
	if (err.s.iop & (1 << 2))
		PRINT_ERROR("POW_ECC_ERR[IOP2]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH/GET_WORK from PP with pending tag switch to ORDERED or ATOMIC\n");
	if (err.s.iop & (1 << 3))
		PRINT_ERROR("POW_ECC_ERR[IOP3]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH from PP with tag specified as NULL_NULL\n");
	if (err.s.iop & (1 << 4))
		PRINT_ERROR("POW_ECC_ERR[IOP4]: Received SWTAG_FULL/SWTAG_DESCH from PP with tag specified as NULL\n");
	if (err.s.iop & (1 << 5))
		PRINT_ERROR("POW_ECC_ERR[IOP5]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH/DESCH/UPD_WQP/GET_WORK/NULL_RD from PP with GET_WORK pending\n");
	if (err.s.iop & (1 << 6))
		PRINT_ERROR("POW_ECC_ERR[IOP6]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH/DESCH/UPD_WQP/GET_WORK/NULL_RD from PP with NULL_RD pending\n");
	if (err.s.iop & (1 << 7))
		PRINT_ERROR("POW_ECC_ERR[IOP7]: Received CLR_NSCHED from PP with SWTAG_DESCH/DESCH/CLR_NSCHED pending\n");
	if (err.s.iop & (1 << 8))
		PRINT_ERROR("POW_ECC_ERR[IOP8]: Received SWTAG/SWTAG_FULL/SWTAG_DESCH/DESCH/UPD_WQP/GET_WORK/NULL_RD from PP with CLR_NSCHED pending\n");
	if (err.s.iop & (1 << 9))
		PRINT_ERROR("POW_ECC_ERR[IOP9]: Received illegal opcode\n");
	if (err.s.iop & (1 << 10))
		PRINT_ERROR("POW_ECC_ERR[IOP10]: Received ADD_WORK with tag specified as NULL_NULL\n");
	if (err.s.iop & (1 << 11))
		PRINT_ERROR("POW_ECC_ERR[IOP11]: Received DBG load from PP with DBG load pending\n");
	if (err.s.iop & (1 << 12))
		PRINT_ERROR("POW_ECC_ERR[IOP12]: Received CSR load from PP with CSR load pending\n");
	return 1;
}

/**
 * @INTERNAL
 * POW_ECC_ERR contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_pow_ecc_err_rpe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_POW_ECC_ERR, cvmx_read_csr(CVMX_POW_ECC_ERR));
	PRINT_ERROR("POW_ECC_ERR[RPE]: Remote pointer error\n");
	return 1;
}

/**
 * @INTERNAL
 * POW_ECC_ERR contains R/W1C bits along with R/W bits. This means that it
 * requires special handling instead of the normal __cvmx_error_display()
 * function.
 *
 * @param info
 *
 * @return
 */
int __cvmx_error_handle_pow_ecc_err_sbe(const struct cvmx_error_info *info)
{
	cvmx_write_csr(CVMX_POW_ECC_ERR, cvmx_read_csr(CVMX_POW_ECC_ERR));
	PRINT_ERROR("POW_ECC_ERR[SBE]: POW single bit error\n");
	return 1;
}

