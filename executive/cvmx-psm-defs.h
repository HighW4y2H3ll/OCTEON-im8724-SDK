/***********************license start***************
 * Copyright (c) 2003-2015  Cavium Inc. (support@cavium.com). All rights
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
 * cvmx-psm-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon psm.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_PSM_DEFS_H__
#define __CVMX_PSM_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_BCLK_DLL_STATUS CVMX_PSM_BCLK_DLL_STATUS_FUNC()
static inline uint64_t CVMX_PSM_BCLK_DLL_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_BCLK_DLL_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D4000000FE20ull);
}
#else
#define CVMX_PSM_BCLK_DLL_STATUS (CVMX_ADD_IO_SEG(0x0001D4000000FE20ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_BIST_STATUS CVMX_PSM_BIST_STATUS_FUNC()
static inline uint64_t CVMX_PSM_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000004020ull);
}
#else
#define CVMX_PSM_BIST_STATUS (CVMX_ADD_IO_SEG(0x0001D40000004020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_CTRL CVMX_PSM_CTRL_FUNC()
static inline uint64_t CVMX_PSM_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000004000ull);
}
#else
#define CVMX_PSM_CTRL (CVMX_ADD_IO_SEG(0x0001D40000004000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_DBG_BREAK_CFG CVMX_PSM_DBG_BREAK_CFG_FUNC()
static inline uint64_t CVMX_PSM_DBG_BREAK_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_DBG_BREAK_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007100ull);
}
#else
#define CVMX_PSM_DBG_BREAK_CFG (CVMX_ADD_IO_SEG(0x0001D40000007100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_DJCNT_CFGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset >= 1) && (offset <= 62))))))
		cvmx_warn("CVMX_PSM_DJCNT_CFGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000003410ull) + ((offset) & 63) * 16 - 16*1;
}
#else
#define CVMX_PSM_DJCNT_CFGX(offset) (CVMX_ADD_IO_SEG(0x0001D40000003410ull) + ((offset) & 63) * 16 - 16*1)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_DJCNT_DECR CVMX_PSM_DJCNT_DECR_FUNC()
static inline uint64_t CVMX_PSM_DJCNT_DECR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_DJCNT_DECR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000003800ull);
}
#else
#define CVMX_PSM_DJCNT_DECR (CVMX_ADD_IO_SEG(0x0001D40000003800ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_ECC_CTL CVMX_PSM_ECC_CTL_FUNC()
static inline uint64_t CVMX_PSM_ECC_CTL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_ECC_CTL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007200ull);
}
#else
#define CVMX_PSM_ECC_CTL (CVMX_ADD_IO_SEG(0x0001D40000007200ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_ECO CVMX_PSM_ECO_FUNC()
static inline uint64_t CVMX_PSM_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D4000000FFF8ull);
}
#else
#define CVMX_PSM_ECO (CVMX_ADD_IO_SEG(0x0001D4000000FFF8ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_ERRCAP_MABFIFO_BADCMD CVMX_PSM_ERRCAP_MABFIFO_BADCMD_FUNC()
static inline uint64_t CVMX_PSM_ERRCAP_MABFIFO_BADCMD_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_ERRCAP_MABFIFO_BADCMD not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007320ull);
}
#else
#define CVMX_PSM_ERRCAP_MABFIFO_BADCMD (CVMX_ADD_IO_SEG(0x0001D40000007320ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_ERRCAP_QECC CVMX_PSM_ERRCAP_QECC_FUNC()
static inline uint64_t CVMX_PSM_ERRCAP_QECC_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_ERRCAP_QECC not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007300ull);
}
#else
#define CVMX_PSM_ERRCAP_QECC (CVMX_ADD_IO_SEG(0x0001D40000007300ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_ERRCAP_QUEUE_BADCMD CVMX_PSM_ERRCAP_QUEUE_BADCMD_FUNC()
static inline uint64_t CVMX_PSM_ERRCAP_QUEUE_BADCMD_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_ERRCAP_QUEUE_BADCMD not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007310ull);
}
#else
#define CVMX_PSM_ERRCAP_QUEUE_BADCMD (CVMX_ADD_IO_SEG(0x0001D40000007310ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_GBL_DLL_STATUS CVMX_PSM_GBL_DLL_STATUS_FUNC()
static inline uint64_t CVMX_PSM_GBL_DLL_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_GBL_DLL_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D4000000FE00ull);
}
#else
#define CVMX_PSM_GBL_DLL_STATUS (CVMX_ADD_IO_SEG(0x0001D4000000FE00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_GPINT_SUM_W1C CVMX_PSM_GPINT_SUM_W1C_FUNC()
static inline uint64_t CVMX_PSM_GPINT_SUM_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_GPINT_SUM_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005000ull);
}
#else
#define CVMX_PSM_GPINT_SUM_W1C (CVMX_ADD_IO_SEG(0x0001D40000005000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_GPINT_SUM_W1S CVMX_PSM_GPINT_SUM_W1S_FUNC()
static inline uint64_t CVMX_PSM_GPINT_SUM_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_GPINT_SUM_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005010ull);
}
#else
#define CVMX_PSM_GPINT_SUM_W1S (CVMX_ADD_IO_SEG(0x0001D40000005010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_GRP_CDTX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_PSM_GRP_CDTX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000003100ull) + ((offset) & 7) * 16;
}
#else
#define CVMX_PSM_GRP_CDTX(offset) (CVMX_ADD_IO_SEG(0x0001D40000003100ull) + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_GRP_MASKX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_PSM_GRP_MASKX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000003000ull) + ((offset) & 7) * 16;
}
#else
#define CVMX_PSM_GRP_MASKX(offset) (CVMX_ADD_IO_SEG(0x0001D40000003000ull) + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_DERR_W1C CVMX_PSM_INT_SUM_DERR_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_DERR_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_DERR_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005500ull);
}
#else
#define CVMX_PSM_INT_SUM_DERR_W1C (CVMX_ADD_IO_SEG(0x0001D40000005500ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_DERR_W1S CVMX_PSM_INT_SUM_DERR_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_DERR_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_DERR_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005510ull);
}
#else
#define CVMX_PSM_INT_SUM_DERR_W1S (CVMX_ADD_IO_SEG(0x0001D40000005510ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JERR_W1C CVMX_PSM_INT_SUM_JERR_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JERR_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JERR_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005200ull);
}
#else
#define CVMX_PSM_INT_SUM_JERR_W1C (CVMX_ADD_IO_SEG(0x0001D40000005200ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JERR_W1S CVMX_PSM_INT_SUM_JERR_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JERR_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JERR_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005210ull);
}
#else
#define CVMX_PSM_INT_SUM_JERR_W1S (CVMX_ADD_IO_SEG(0x0001D40000005210ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JNFAT_W1C CVMX_PSM_INT_SUM_JNFAT_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JNFAT_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JNFAT_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005300ull);
}
#else
#define CVMX_PSM_INT_SUM_JNFAT_W1C (CVMX_ADD_IO_SEG(0x0001D40000005300ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JNFAT_W1S CVMX_PSM_INT_SUM_JNFAT_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JNFAT_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JNFAT_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005310ull);
}
#else
#define CVMX_PSM_INT_SUM_JNFAT_W1S (CVMX_ADD_IO_SEG(0x0001D40000005310ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JTO_W1C CVMX_PSM_INT_SUM_JTO_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JTO_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JTO_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005400ull);
}
#else
#define CVMX_PSM_INT_SUM_JTO_W1C (CVMX_ADD_IO_SEG(0x0001D40000005400ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_JTO_W1S CVMX_PSM_INT_SUM_JTO_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_JTO_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_JTO_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005410ull);
}
#else
#define CVMX_PSM_INT_SUM_JTO_W1S (CVMX_ADD_IO_SEG(0x0001D40000005410ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_QOVF_W1C CVMX_PSM_INT_SUM_QOVF_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_QOVF_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_QOVF_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005600ull);
}
#else
#define CVMX_PSM_INT_SUM_QOVF_W1C (CVMX_ADD_IO_SEG(0x0001D40000005600ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_QOVF_W1S CVMX_PSM_INT_SUM_QOVF_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_QOVF_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_QOVF_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005610ull);
}
#else
#define CVMX_PSM_INT_SUM_QOVF_W1S (CVMX_ADD_IO_SEG(0x0001D40000005610ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_QTO_W1C CVMX_PSM_INT_SUM_QTO_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_QTO_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_QTO_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005700ull);
}
#else
#define CVMX_PSM_INT_SUM_QTO_W1C (CVMX_ADD_IO_SEG(0x0001D40000005700ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_QTO_W1S CVMX_PSM_INT_SUM_QTO_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_QTO_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_QTO_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005710ull);
}
#else
#define CVMX_PSM_INT_SUM_QTO_W1S (CVMX_ADD_IO_SEG(0x0001D40000005710ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_W1C CVMX_PSM_INT_SUM_W1C_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005100ull);
}
#else
#define CVMX_PSM_INT_SUM_W1C (CVMX_ADD_IO_SEG(0x0001D40000005100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_INT_SUM_W1S CVMX_PSM_INT_SUM_W1S_FUNC()
static inline uint64_t CVMX_PSM_INT_SUM_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_INT_SUM_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000005110ull);
}
#else
#define CVMX_PSM_INT_SUM_W1S (CVMX_ADD_IO_SEG(0x0001D40000005110ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_JOB_REQX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 3)))))
		cvmx_warn("CVMX_PSM_JOB_REQX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D4000000F080ull) + ((offset) & 3) * 16;
}
#else
#define CVMX_PSM_JOB_REQX(offset) (CVMX_ADD_IO_SEG(0x0001D4000000F080ull) + ((offset) & 3) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_JOB_UNSERVEDX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 3)))))
		cvmx_warn("CVMX_PSM_JOB_UNSERVEDX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D4000000F0C0ull) + ((offset) & 3) * 16;
}
#else
#define CVMX_PSM_JOB_UNSERVEDX(offset) (CVMX_ADD_IO_SEG(0x0001D4000000F0C0ull) + ((offset) & 3) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_LOG_BASE CVMX_PSM_LOG_BASE_FUNC()
static inline uint64_t CVMX_PSM_LOG_BASE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_LOG_BASE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007010ull);
}
#else
#define CVMX_PSM_LOG_BASE (CVMX_ADD_IO_SEG(0x0001D40000007010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_LOG_CFG CVMX_PSM_LOG_CFG_FUNC()
static inline uint64_t CVMX_PSM_LOG_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_LOG_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007000ull);
}
#else
#define CVMX_PSM_LOG_CFG (CVMX_ADD_IO_SEG(0x0001D40000007000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_LOG_PTR CVMX_PSM_LOG_PTR_FUNC()
static inline uint64_t CVMX_PSM_LOG_PTR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_LOG_PTR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000007020ull);
}
#else
#define CVMX_PSM_LOG_PTR (CVMX_ADD_IO_SEG(0x0001D40000007020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_MABFIFO_CTRLX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_MABFIFO_CTRLX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000006000ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_MABFIFO_CTRLX(offset) (CVMX_ADD_IO_SEG(0x0001D40000006000ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_MABFIFO_HEAD_HIX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_MABFIFO_HEAD_HIX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000006800ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_MABFIFO_HEAD_HIX(offset) (CVMX_ADD_IO_SEG(0x0001D40000006800ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_MABFIFO_HEAD_LOX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_MABFIFO_HEAD_LOX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000006400ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_MABFIFO_HEAD_LOX(offset) (CVMX_ADD_IO_SEG(0x0001D40000006400ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_MAB_RES CVMX_PSM_MAB_RES_FUNC()
static inline uint64_t CVMX_PSM_MAB_RES_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_MAB_RES not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D4000000F400ull);
}
#else
#define CVMX_PSM_MAB_RES (CVMX_ADD_IO_SEG(0x0001D4000000F400ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_MAX_JOB_CDTX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 63)))))
		cvmx_warn("CVMX_PSM_MAX_JOB_CDTX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000002400ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_MAX_JOB_CDTX(offset) (CVMX_ADD_IO_SEG(0x0001D40000002400ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_NONJOB_RSRCX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_PSM_NONJOB_RSRCX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D4000000F000ull) + ((offset) & 7) * 16;
}
#else
#define CVMX_PSM_NONJOB_RSRCX(offset) (CVMX_ADD_IO_SEG(0x0001D4000000F000ull) + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_QUEUE_BUSY_STS CVMX_PSM_QUEUE_BUSY_STS_FUNC()
static inline uint64_t CVMX_PSM_QUEUE_BUSY_STS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_QUEUE_BUSY_STS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000001110ull);
}
#else
#define CVMX_PSM_QUEUE_BUSY_STS (CVMX_ADD_IO_SEG(0x0001D40000001110ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_CFGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_CFGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000000400ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_CFGX(offset) (CVMX_ADD_IO_SEG(0x0001D40000000400ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_CMD_HIX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_CMD_HIX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000000008ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_CMD_HIX(offset) (CVMX_ADD_IO_SEG(0x0001D40000000008ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_CMD_LOX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_CMD_LOX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000000000ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_CMD_LOX(offset) (CVMX_ADD_IO_SEG(0x0001D40000000000ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_QUEUE_ENA_W1C CVMX_PSM_QUEUE_ENA_W1C_FUNC()
static inline uint64_t CVMX_PSM_QUEUE_ENA_W1C_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_QUEUE_ENA_W1C not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000001010ull);
}
#else
#define CVMX_PSM_QUEUE_ENA_W1C (CVMX_ADD_IO_SEG(0x0001D40000001010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_QUEUE_ENA_W1S CVMX_PSM_QUEUE_ENA_W1S_FUNC()
static inline uint64_t CVMX_PSM_QUEUE_ENA_W1S_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_QUEUE_ENA_W1S not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000001000ull);
}
#else
#define CVMX_PSM_QUEUE_ENA_W1S (CVMX_ADD_IO_SEG(0x0001D40000001000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_QUEUE_FULL_STS CVMX_PSM_QUEUE_FULL_STS_FUNC()
static inline uint64_t CVMX_PSM_QUEUE_FULL_STS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_QUEUE_FULL_STS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000001100ull);
}
#else
#define CVMX_PSM_QUEUE_FULL_STS (CVMX_ADD_IO_SEG(0x0001D40000001100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_INFOX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_INFOX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000001800ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_INFOX(offset) (CVMX_ADD_IO_SEG(0x0001D40000001800ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_PTRX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_PTRX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000000800ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_PTRX(offset) (CVMX_ADD_IO_SEG(0x0001D40000000800ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_RAMX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 16383)))))
		cvmx_warn("CVMX_PSM_QUEUE_RAMX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000020000ull) + ((offset) & 16383) * 8;
}
#else
#define CVMX_PSM_QUEUE_RAMX(offset) (CVMX_ADD_IO_SEG(0x0001D40000020000ull) + ((offset) & 16383) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_SPACEX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_SPACEX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000000C00ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_SPACEX(offset) (CVMX_ADD_IO_SEG(0x0001D40000000C00ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_QUEUE_TIMEOUT_CFGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_PSM_QUEUE_TIMEOUT_CFGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000003C00ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_QUEUE_TIMEOUT_CFGX(offset) (CVMX_ADD_IO_SEG(0x0001D40000003C00ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PSM_RSRC_TBLX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 63)))))
		cvmx_warn("CVMX_PSM_RSRC_TBLX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D40000002000ull) + ((offset) & 63) * 16;
}
#else
#define CVMX_PSM_RSRC_TBLX(offset) (CVMX_ADD_IO_SEG(0x0001D40000002000ull) + ((offset) & 63) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_RST CVMX_PSM_RST_FUNC()
static inline uint64_t CVMX_PSM_RST_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_RST not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000004010ull);
}
#else
#define CVMX_PSM_RST (CVMX_ADD_IO_SEG(0x0001D40000004010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_SCLK_DLL_STATUS CVMX_PSM_SCLK_DLL_STATUS_FUNC()
static inline uint64_t CVMX_PSM_SCLK_DLL_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_SCLK_DLL_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D4000000FE10ull);
}
#else
#define CVMX_PSM_SCLK_DLL_STATUS (CVMX_ADD_IO_SEG(0x0001D4000000FE10ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_TIMER_CFG CVMX_PSM_TIMER_CFG_FUNC()
static inline uint64_t CVMX_PSM_TIMER_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_TIMER_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000003200ull);
}
#else
#define CVMX_PSM_TIMER_CFG (CVMX_ADD_IO_SEG(0x0001D40000003200ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PSM_TIMER_VAL CVMX_PSM_TIMER_VAL_FUNC()
static inline uint64_t CVMX_PSM_TIMER_VAL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PSM_TIMER_VAL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D40000003210ull);
}
#else
#define CVMX_PSM_TIMER_VAL (CVMX_ADD_IO_SEG(0x0001D40000003210ull))
#endif

/**
 * cvmx_psm_bclk_dll_status
 *
 * Status of the BPHY BCLK DLL.
 *
 */
union cvmx_psm_bclk_dll_status {
	uint64_t u64;
	struct cvmx_psm_bclk_dll_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_60_63               : 4;
	uint64_t max_dll_setting              : 12; /**< Max reported DLL setting. */
	uint64_t min_dll_setting              : 12; /**< Min reported DLL setting. */
	uint64_t pd_pos_bclk_refclk           : 1;  /**< Synchronized pd_pos_bclk_refclk from BPHY BCLK DLL cmb0 phase detectors. */
	uint64_t pdl_bclk_refclk              : 1;  /**< Synchronized pdl_bclk_refclk from BPHY BCLK DLL cmb0 phase detectors. */
	uint64_t pdr_bclk_refclk              : 1;  /**< Synchronized pdr_bclk_refclk from BPHY BCLK DLL cmb0 phase detectors. */
	uint64_t reserved_32_32               : 1;
	uint64_t dll_dly_elem_en              : 16; /**< The BPHY BCLK delay element enable setting, from the negative edge of refclk. */
	uint64_t dll_setting                  : 12; /**< The BPHY BCLK DLL setting, from the negative edge of refclk. */
	uint64_t reserved_1_3                 : 3;
	uint64_t dll_lock                     : 1;  /**< The dll_lock signal from BPHY BCLK DLL, from the positive edge of refclk. */
#else
	uint64_t dll_lock                     : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t dll_setting                  : 12;
	uint64_t dll_dly_elem_en              : 16;
	uint64_t reserved_32_32               : 1;
	uint64_t pdr_bclk_refclk              : 1;
	uint64_t pdl_bclk_refclk              : 1;
	uint64_t pd_pos_bclk_refclk           : 1;
	uint64_t min_dll_setting              : 12;
	uint64_t max_dll_setting              : 12;
	uint64_t reserved_60_63               : 4;
#endif
	} s;
	struct cvmx_psm_bclk_dll_status_s     cnf75xx;
};
typedef union cvmx_psm_bclk_dll_status cvmx_psm_bclk_dll_status_t;

/**
 * cvmx_psm_bist_status
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_psm_bist_status {
	uint64_t u64;
	struct cvmx_psm_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t joblog_bist_status           : 1;  /**< BIST status results for the job log RAM. */
	uint64_t qram_bist_status             : 1;  /**< BIST status results for the queue RAM. */
#else
	uint64_t qram_bist_status             : 1;
	uint64_t joblog_bist_status           : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_psm_bist_status_s         cnf75xx;
};
typedef union cvmx_psm_bist_status cvmx_psm_bist_status_t;

/**
 * cvmx_psm_ctrl
 *
 * This register controls arbitration modes and access to testing and
 * diagnostic registers.
 * For now, these features are marked internal and all users should use the
 * default values.
 */
union cvmx_psm_ctrl {
	uint64_t u64;
	struct cvmx_psm_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_37_63               : 27;
	uint64_t internal_access_mode         : 1;  /**< Set to one to allow writing to PSM_MAB_RES when reseting individual
                                                         PSM queues. */
	uint64_t nonjob_arb_mode              : 1;  /**< Arbitration scheme when selecting non-job commands from the 48 queues.
                                                         0 = Fixed (Queue 0 = highest priority,  queue 47 = lowest priority).
                                                         1 = Round-robin. */
	uint64_t qram_wr_arb_mode             : 1;  /**< N/A */
	uint64_t qram_rd_arb_mode             : 1;  /**< N/A */
	uint64_t reserved_5_32                : 28;
	uint64_t jca_arb_mode                 : 1;  /**< N/A */
	uint64_t mabenq_arb_mode              : 1;  /**< N/A */
	uint64_t immexec_arb_mode             : 1;  /**< N/A */
	uint64_t mabrsp_arb_mode              : 1;  /**< N/A */
	uint64_t ncbi_arb_mode                : 1;  /**< N/A */
#else
	uint64_t ncbi_arb_mode                : 1;
	uint64_t mabrsp_arb_mode              : 1;
	uint64_t immexec_arb_mode             : 1;
	uint64_t mabenq_arb_mode              : 1;
	uint64_t jca_arb_mode                 : 1;
	uint64_t reserved_5_32                : 28;
	uint64_t qram_rd_arb_mode             : 1;
	uint64_t qram_wr_arb_mode             : 1;
	uint64_t nonjob_arb_mode              : 1;
	uint64_t internal_access_mode         : 1;
	uint64_t reserved_37_63               : 27;
#endif
	} s;
	struct cvmx_psm_ctrl_s                cnf75xx;
};
typedef union cvmx_psm_ctrl cvmx_psm_ctrl_t;

/**
 * cvmx_psm_dbg_break_cfg
 *
 * This register configures the debug breakpoint mechanism. When a breakpoint
 * is triggered, the PSM triggers a PSM_TRACE_BKPT interrupt, and can optionally halt.
 * While halted, the PSM will not schedule any new jobs or execute
 * any commands from the 48 queues, but jobs can still complete, job
 * completion commands can still execute, and new commands may still be added
 * to the queues. The queues can be un-halted by re-enabling the queues
 * (i.e., a write to PSM_QUEUE_ENA_W1S, or a QRUN command received from a
 * MHAB/MDAB).
 */
union cvmx_psm_dbg_break_cfg {
	uint64_t u64;
	struct cvmx_psm_dbg_break_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t start_brk_ena                : 1;  /**< Job start breakpoint enable. When set, a debug breakpoint will be
                                                         triggered when the PSM schedules a job with a matching JOB_TAG to a
                                                         MHAB/MDAB */
	uint64_t done_brk_ena                 : 1;  /**< Enables the debug breakpoint which will be triggered
                                                         when receiving a job completion that matches the specified JOB_TAG.
                                                         Job done breakpoint enable. When set, a debug breakpoint will be
                                                         triggered when the PSM receives a job completion message with matching
                                                         JOB_TAG. */
	uint64_t stop_on_brk                  : 1;  /**< When set, the PSM will halt when a breakpoint is triggered, in
                                                         addition to signalling the PSM_TRACE_BKPT interrupt. Otherwise, the
                                                         PSM will continue execution after signalling the interrupt. */
	uint64_t halted                       : 1;  /**< The PSM sets this bit after halting due to a breakpoint.
                                                         When set, all queues will be halted from advancing. Job completion and job
                                                         queue additions are still enabled. Queues may be resumed by re-enabling
                                                         them using the PSM_QUEUE_ENA_W1S register. */
	uint64_t reserved_16_59               : 44;
	uint64_t job_tag                      : 16; /**< JOBTAG to be matched for the break mechanism. */
#else
	uint64_t job_tag                      : 16;
	uint64_t reserved_16_59               : 44;
	uint64_t halted                       : 1;
	uint64_t stop_on_brk                  : 1;
	uint64_t done_brk_ena                 : 1;
	uint64_t start_brk_ena                : 1;
#endif
	} s;
	struct cvmx_psm_dbg_break_cfg_s       cnf75xx;
};
typedef union cvmx_psm_dbg_break_cfg cvmx_psm_dbg_break_cfg_t;

/**
 * cvmx_psm_djcnt_cfg#
 *
 * These registers allow reading and setting of dependent job counters.
 *
 */
union cvmx_psm_djcnt_cfgx {
	uint64_t u64;
	struct cvmx_psm_djcnt_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t djcnt_sosf                   : 1;  /**< Enable SOS auto-decrement. When set, the DJCNT will decrement by one
                                                         when an RFIF SOS is received. */
	uint64_t djcnt_update                 : 1;  /**< When written 1, the DJCNT will be updated with [DJCNT_VAL]. */
	uint64_t djcnt_val                    : 8;  /**< On read, returns the current value of the DJCNT. On write, the DJCNT will be
                                                         updated with this value if [DJCNT_UPDATE] is set. */
#else
	uint64_t djcnt_val                    : 8;
	uint64_t djcnt_update                 : 1;
	uint64_t djcnt_sosf                   : 1;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_psm_djcnt_cfgx_s          cnf75xx;
};
typedef union cvmx_psm_djcnt_cfgx cvmx_psm_djcnt_cfgx_t;

/**
 * cvmx_psm_djcnt_decr
 *
 * This register selects which DJCNT counters are decremented based on the
 * PSM event inputs from ULFE and PRACH.
 */
union cvmx_psm_djcnt_decr {
	uint64_t u64;
	struct cvmx_psm_djcnt_decr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t djcnt_decr_id_7              : 6;  /**< N/A */
	uint64_t reserved_54_55               : 2;
	uint64_t djcnt_decr_id_6              : 6;  /**< N/A */
	uint64_t reserved_46_47               : 2;
	uint64_t prch_event_1                 : 6;  /**< N/A */
	uint64_t reserved_38_39               : 2;
	uint64_t prch_rpt_done                : 6;  /**< Selects the DJCNT counter that will be decremented by the PSM when it
                                                         receives a RPT completion signal from the PRACH.  Values 1-62
                                                         will select a DJCNT, while 0 will cause the event signal to have no
                                                         effect on any counter. */
	uint64_t reserved_30_31               : 2;
	uint64_t ulfe_event_3                 : 6;  /**< N/A */
	uint64_t reserved_22_23               : 2;
	uint64_t ulfe_rpt_done                : 6;  /**< Selects the DJCNT counter that will be decremented by the PSM when it
                                                         receives a RPT completion signal from the ULFE.  Values 1-62
                                                         will select a DJCNT, while 0 will cause the event signal to have no
                                                         effect on any counter. */
	uint64_t reserved_14_15               : 2;
	uint64_t ulfe_dmrs_done               : 6;  /**< Selects the DJCNT counter that will be decremented by the PSM when it
                                                         receives a DMRS completion signal from the ULFE.  Values 1-62
                                                         will select a DJCNT, while 0 will cause the event signal to have no
                                                         effect on any counter. */
	uint64_t reserved_6_7                 : 2;
	uint64_t ulfe_pstsk_done              : 6;  /**< Selects the DJCNT counter that will be decremented by the PSM when it
                                                         receives a symbol write completion signal from the ULFE.  Values 1-62
                                                         will select a DJCNT, while 0 will cause the event signal to have no
                                                         effect on any counter. */
#else
	uint64_t ulfe_pstsk_done              : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t ulfe_dmrs_done               : 6;
	uint64_t reserved_14_15               : 2;
	uint64_t ulfe_rpt_done                : 6;
	uint64_t reserved_22_23               : 2;
	uint64_t ulfe_event_3                 : 6;
	uint64_t reserved_30_31               : 2;
	uint64_t prch_rpt_done                : 6;
	uint64_t reserved_38_39               : 2;
	uint64_t prch_event_1                 : 6;
	uint64_t reserved_46_47               : 2;
	uint64_t djcnt_decr_id_6              : 6;
	uint64_t reserved_54_55               : 2;
	uint64_t djcnt_decr_id_7              : 6;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_psm_djcnt_decr_s          cnf75xx;
};
typedef union cvmx_psm_djcnt_decr cvmx_psm_djcnt_decr_t;

/**
 * cvmx_psm_ecc_ctl
 *
 * This register is used for controlling ECC and injecting errors
 * (SBE/DBE) into the PSM memories.  When error injection is enabled
 * and a write occurs to the specified address, the syndrome will be
 * flipped as directed.
 */
union cvmx_psm_ecc_ctl {
	uint64_t u64;
	struct cvmx_psm_ecc_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t qram_cor_dis                 : 1;  /**< ECC correction disable for the QRAM. */
	uint64_t reserved_25_31               : 7;
	uint64_t err_inj_en                   : 1;  /**< N/A */
	uint64_t reserved_18_23               : 6;
	uint64_t err_inj_flip_synd            : 2;  /**< N/A */
	uint64_t reserved_13_15               : 3;
	uint64_t err_inj_addr                 : 13; /**< N/A */
#else
	uint64_t err_inj_addr                 : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t err_inj_flip_synd            : 2;
	uint64_t reserved_18_23               : 6;
	uint64_t err_inj_en                   : 1;
	uint64_t reserved_25_31               : 7;
	uint64_t qram_cor_dis                 : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_psm_ecc_ctl_s             cnf75xx;
};
typedef union cvmx_psm_ecc_ctl cvmx_psm_ecc_ctl_t;

/**
 * cvmx_psm_eco
 *
 * An ECO CSR.
 *
 */
union cvmx_psm_eco {
	uint64_t u64;
	struct cvmx_psm_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_psm_eco_s                 cnf75xx;
};
typedef union cvmx_psm_eco cvmx_psm_eco_t;

/**
 * cvmx_psm_errcap_mabfifo_badcmd
 *
 * This register reports the status of bad command errors reported by MHAB/MDAB FIFOs.
 *
 */
union cvmx_psm_errcap_mabfifo_badcmd {
	uint64_t u64;
	struct cvmx_psm_errcap_mabfifo_badcmd_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mabfifo_bad_cmd              : 64; /**< When a bit is set, the corresponding MHAB/MDAB FIFO has reported a bad command.
                                                         Bits 0-19 correspond to the MHABs. Bits 24-41 correspond to MDABs
                                                         0-17.  Write 1 to clear. */
#else
	uint64_t mabfifo_bad_cmd              : 64;
#endif
	} s;
	struct cvmx_psm_errcap_mabfifo_badcmd_s cnf75xx;
};
typedef union cvmx_psm_errcap_mabfifo_badcmd cvmx_psm_errcap_mabfifo_badcmd_t;

/**
 * cvmx_psm_errcap_qecc
 *
 * This register reports the status of Queue RAM errors.  The details
 * of the most recent SBE and DBE are captured.
 */
union cvmx_psm_errcap_qecc {
	uint64_t u64;
	struct cvmx_psm_errcap_qecc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_61_63               : 3;
	uint64_t sbe_addr                     : 13; /**< N/A */
	uint64_t sbe_ecc                      : 8;  /**< N/A */
	uint64_t sbe_synd                     : 8;  /**< N/A */
	uint64_t reserved_29_31               : 3;
	uint64_t dbe_addr                     : 13; /**< N/A */
	uint64_t dbe_ecc                      : 8;  /**< N/A */
	uint64_t dbe_synd                     : 8;  /**< N/A */
#else
	uint64_t dbe_synd                     : 8;
	uint64_t dbe_ecc                      : 8;
	uint64_t dbe_addr                     : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t sbe_synd                     : 8;
	uint64_t sbe_ecc                      : 8;
	uint64_t sbe_addr                     : 13;
	uint64_t reserved_61_63               : 3;
#endif
	} s;
	struct cvmx_psm_errcap_qecc_s         cnf75xx;
};
typedef union cvmx_psm_errcap_qecc cvmx_psm_errcap_qecc_t;

/**
 * cvmx_psm_errcap_queue_badcmd
 *
 * This register reports the status of bad command errors reported by queues.
 *
 */
union cvmx_psm_errcap_queue_badcmd {
	uint64_t u64;
	struct cvmx_psm_errcap_queue_badcmd_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t queue_bad_cmd                : 48; /**< When a bit is set, the corresponding queue has reported a bad command.
                                                         Write 1 to clear. */
#else
	uint64_t queue_bad_cmd                : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_errcap_queue_badcmd_s cnf75xx;
};
typedef union cvmx_psm_errcap_queue_badcmd cvmx_psm_errcap_queue_badcmd_t;

/**
 * cvmx_psm_gbl_dll_status
 *
 * Status of the global SCLK DLL.
 *
 */
union cvmx_psm_gbl_dll_status {
	uint64_t u64;
	struct cvmx_psm_gbl_dll_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t pdr_sclk_refclk              : 1;  /**< Synchronized pdr_sclk_refclk from global SCLK DLL cmb0 phase detectors. */
	uint64_t pdl_sclk_refclk              : 1;  /**< Synchronized pdl_sclk_refclk from global SCLK DLL cmb0 phase detectors. */
	uint64_t pd_pos_sclk_refclk           : 1;  /**< Synchronized pd_pos_sclk_refclk from global SCLK DLL cmb0 phase detectors. */
	uint64_t dll_fsm_state_a              : 3;  /**< State for the global SCLK DLL, from the positive edge of refclk.
                                                         0x0 = TMD_IDLE.
                                                         0x1 = TMD_STATE1.
                                                         0x2 = TMD_STATE2.
                                                         0x3 = TMD_STATE3.
                                                         0x4 = TMD_STATE4.
                                                         0x5 = TMD_LOCKED. */
	uint64_t dll_lock                     : 1;  /**< The dll_lock signal from global SCLK DLL, from the positive edge of refclk. */
	uint64_t dll_clk_invert_out           : 1;  /**< The clk_invert setting from the global SCLK DLL, from the negative edge of refclk. */
	uint64_t dll_setting                  : 12; /**< The global SCLK DLL setting, from the negative edge of refclk. */
#else
	uint64_t dll_setting                  : 12;
	uint64_t dll_clk_invert_out           : 1;
	uint64_t dll_lock                     : 1;
	uint64_t dll_fsm_state_a              : 3;
	uint64_t pd_pos_sclk_refclk           : 1;
	uint64_t pdl_sclk_refclk              : 1;
	uint64_t pdr_sclk_refclk              : 1;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_psm_gbl_dll_status_s      cnf75xx;
};
typedef union cvmx_psm_gbl_dll_status cvmx_psm_gbl_dll_status_t;

/**
 * cvmx_psm_gpint_sum_w1c
 *
 * This register reports the status of the general purpose interrupts. Writing
 * a 1 will clear the selected interupt. Writes of 0 are ignored.
 */
union cvmx_psm_gpint_sum_w1c {
	uint64_t u64;
	struct cvmx_psm_gpint_sum_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t gp                           : 64; /**< Status bit for each general purpose interrupt. */
#else
	uint64_t gp                           : 64;
#endif
	} s;
	struct cvmx_psm_gpint_sum_w1c_s       cnf75xx;
};
typedef union cvmx_psm_gpint_sum_w1c cvmx_psm_gpint_sum_w1c_t;

/**
 * cvmx_psm_gpint_sum_w1s
 */
union cvmx_psm_gpint_sum_w1s {
	uint64_t u64;
	struct cvmx_psm_gpint_sum_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t gp                           : 64; /**< Status bit for each general purpose interrupt. */
#else
	uint64_t gp                           : 64;
#endif
	} s;
	struct cvmx_psm_gpint_sum_w1s_s       cnf75xx;
};
typedef union cvmx_psm_gpint_sum_w1s cvmx_psm_gpint_sum_w1s_t;

/**
 * cvmx_psm_grp_cdt#
 *
 * These registers define the total number of job credits available for
 * the MHAB/MDAB group.  It must match the total of the credits programmed
 * in the PSM_MAX_JOB_CDT() registers for the MHABs/MDABs selected by the
 * corresponding PSM_GRP_MASK[a] register.
 */
union cvmx_psm_grp_cdtx {
	uint64_t u64;
	struct cvmx_psm_grp_cdtx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t grp_cdt                      : 4;  /**< Maximum number of jobs for the MHAB/MDABs selected by the
                                                         corresponding PSM_GROUP_MASK() register. Valid range is [0,15]. */
#else
	uint64_t grp_cdt                      : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_psm_grp_cdtx_s            cnf75xx;
};
typedef union cvmx_psm_grp_cdtx cvmx_psm_grp_cdtx_t;

/**
 * cvmx_psm_grp_mask#
 *
 * These registers define membership of MHABs/MDABs within each of six groups.
 * Along with the PSM_GRP_CDT() registers, these registers can be used to
 * load balance jobs across many homogenous MHABs/MDABs.
 *
 * Under normal circumstances, the six groups should be programmed to match
 * the six GHBs containing the 18 MDABs and ensure MDAB jobs are balanced
 * across GHBs.
 */
union cvmx_psm_grp_maskx {
	uint64_t u64;
	struct cvmx_psm_grp_maskx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mab_map                      : 64; /**< Bit mask indicating which MHAB/MDABs belong to the MHAB/MDAB
                                                         group.  If this register is programmed, the corresponding group
                                                         credit register (PSM_GRP_CDT[a]) must be programmed with the correct
                                                         value. */
#else
	uint64_t mab_map                      : 64;
#endif
	} s;
	struct cvmx_psm_grp_maskx_s           cnf75xx;
};
typedef union cvmx_psm_grp_maskx cvmx_psm_grp_maskx_t;

/**
 * cvmx_psm_int_sum_derr_w1c
 *
 * This register reports the status of the DMA error interrupts,
 * where a MHAB/MDAB has reported a DMA error. Writing a 1 will clear
 * the selected interupt.  Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_derr_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_derr_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t derr                         : 64; /**< Status bit for each DMA error interrupt. */
#else
	uint64_t derr                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_derr_w1c_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_derr_w1c cvmx_psm_int_sum_derr_w1c_t;

/**
 * cvmx_psm_int_sum_derr_w1s
 */
union cvmx_psm_int_sum_derr_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_derr_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t derr                         : 64; /**< Status bit for each DMA error interrupt. */
#else
	uint64_t derr                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_derr_w1s_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_derr_w1s cvmx_psm_int_sum_derr_w1s_t;

/**
 * cvmx_psm_int_sum_jerr_w1c
 *
 * This register reports the status of the job error interrupts for each
 * MHAB/MDAB. Writing a 1 will clear the selected interupt. Writes of 0 are
 * ignored.
 */
union cvmx_psm_int_sum_jerr_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_jerr_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jerr                         : 64; /**< Status bit for each job error interrupt. */
#else
	uint64_t jerr                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jerr_w1c_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_jerr_w1c cvmx_psm_int_sum_jerr_w1c_t;

/**
 * cvmx_psm_int_sum_jerr_w1s
 */
union cvmx_psm_int_sum_jerr_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_jerr_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jerr                         : 64; /**< Status bit for each job error interrupt. */
#else
	uint64_t jerr                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jerr_w1s_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_jerr_w1s cvmx_psm_int_sum_jerr_w1s_t;

/**
 * cvmx_psm_int_sum_jnfat_w1c
 *
 * This register reports the status of the non-fatal job error
 * interrupts for all MHABs/MDABs. Writing a 1 will clear the selected interupt.
 * Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_jnfat_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_jnfat_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jnfat                        : 64; /**< Status bit for each non-fatal job error interrupt. */
#else
	uint64_t jnfat                        : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jnfat_w1c_s   cnf75xx;
};
typedef union cvmx_psm_int_sum_jnfat_w1c cvmx_psm_int_sum_jnfat_w1c_t;

/**
 * cvmx_psm_int_sum_jnfat_w1s
 */
union cvmx_psm_int_sum_jnfat_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_jnfat_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jnfat                        : 64; /**< Status bit for each non-fatal job error interrupt. */
#else
	uint64_t jnfat                        : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jnfat_w1s_s   cnf75xx;
};
typedef union cvmx_psm_int_sum_jnfat_w1s cvmx_psm_int_sum_jnfat_w1s_t;

/**
 * cvmx_psm_int_sum_jto_w1c
 *
 * This register reports the status of the job timeout
 * interrupts for all MHABs/MDABs. Writing a 1 will clear the selected interupt.
 * Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_jto_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_jto_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jto                          : 64; /**< Status bit for each job timeout interrupt. */
#else
	uint64_t jto                          : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jto_w1c_s     cnf75xx;
};
typedef union cvmx_psm_int_sum_jto_w1c cvmx_psm_int_sum_jto_w1c_t;

/**
 * cvmx_psm_int_sum_jto_w1s
 */
union cvmx_psm_int_sum_jto_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_jto_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jto                          : 64; /**< Status bit for each job timeout interrupt. */
#else
	uint64_t jto                          : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_jto_w1s_s     cnf75xx;
};
typedef union cvmx_psm_int_sum_jto_w1s cvmx_psm_int_sum_jto_w1s_t;

/**
 * cvmx_psm_int_sum_qovf_w1c
 *
 * This register reports the status of the queue overflow interrupts,
 * where a command was added to an already full queue. Each bit corresponds
 * to one of the 48 PSM queues.  Writing a 1
 * will clear the selected interupt.  Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_qovf_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_qovf_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qovf                         : 64; /**< Status bit for each queue overflow interrupt. */
#else
	uint64_t qovf                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_qovf_w1c_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_qovf_w1c cvmx_psm_int_sum_qovf_w1c_t;

/**
 * cvmx_psm_int_sum_qovf_w1s
 */
union cvmx_psm_int_sum_qovf_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_qovf_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qovf                         : 64; /**< Status bit for each queue overflow interrupt. */
#else
	uint64_t qovf                         : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_qovf_w1s_s    cnf75xx;
};
typedef union cvmx_psm_int_sum_qovf_w1s cvmx_psm_int_sum_qovf_w1s_t;

/**
 * cvmx_psm_int_sum_qto_w1c
 *
 * This register reports the status of the queue timeout interrupts.
 * Writing a 1 will clear the selected interupt.  Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_qto_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_qto_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qto                          : 64; /**< Status bit for each queue timeout interrupt. */
#else
	uint64_t qto                          : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_qto_w1c_s     cnf75xx;
};
typedef union cvmx_psm_int_sum_qto_w1c cvmx_psm_int_sum_qto_w1c_t;

/**
 * cvmx_psm_int_sum_qto_w1s
 */
union cvmx_psm_int_sum_qto_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_qto_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qto                          : 64; /**< Status bit for each queue timeout interrupt. */
#else
	uint64_t qto                          : 64;
#endif
	} s;
	struct cvmx_psm_int_sum_qto_w1s_s     cnf75xx;
};
typedef union cvmx_psm_int_sum_qto_w1s cvmx_psm_int_sum_qto_w1s_t;

/**
 * cvmx_psm_int_sum_w1c
 *
 * This register returns the interrupt status indicating which errors and
 * events have occurred.  Writing a bit with 1 will clear that bit.
 * Writes of 0 are ignored.
 */
union cvmx_psm_int_sum_w1c {
	uint64_t u64;
	struct cvmx_psm_int_sum_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t qsbe                         : 1;  /**< A correctable ECC error (single-bit error) was detected during
                                                         a read from the queue RAM. */
	uint64_t nerr                         : 1;  /**< An error occurred during an NCB operation.  This can include an
                                                         unsupported operation such as a byte-write, or a STDN response
                                                         with an error. */
	uint64_t bcmd                         : 1;  /**< An undefined or illegal queue command was encountered.  This can
                                                         result from an undefined command opcode, a job (ADDJOB/CONTJOB) with
                                                         an immediate-execution QID, a MAB response command in a queue, or a
                                                         command from a MAB with an invalid QID.
                                                         PSM_ERRCAP_QUEUE_BADCMD and PSM_ERRCAP_MABFIFO_BADCMD report which
                                                         queue or MAB had a bad command. Details on the QID and op-code for the
                                                         bad command can be found in either PSM_QUEUE_INFO() or
                                                         PSM_MABFIFO_CTRL() registers. */
	uint64_t lroll                        : 1;  /**< The job log pointer rolled over within the circular buffer. */
	uint64_t lovf                         : 1;  /**< The job log write buffer overflowed before the log data could be
                                                         written to system memory. */
	uint64_t qecc                         : 1;  /**< An uncorrectable ECC error (double-bit error) was detected during
                                                         a read from the queue RAM. */
	uint64_t bkpt                         : 1;  /**< Trace interrupt from either a job completion or a job dispatch according
                                                         to the debug breakpoint settings. */
	uint64_t sferr                        : 1;  /**< The tick counter reached the maximum programmed value without
                                                         a SOSF being received. */
#else
	uint64_t sferr                        : 1;
	uint64_t bkpt                         : 1;
	uint64_t qecc                         : 1;
	uint64_t lovf                         : 1;
	uint64_t lroll                        : 1;
	uint64_t bcmd                         : 1;
	uint64_t nerr                         : 1;
	uint64_t qsbe                         : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_psm_int_sum_w1c_s         cnf75xx;
};
typedef union cvmx_psm_int_sum_w1c cvmx_psm_int_sum_w1c_t;

/**
 * cvmx_psm_int_sum_w1s
 */
union cvmx_psm_int_sum_w1s {
	uint64_t u64;
	struct cvmx_psm_int_sum_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t qsbe                         : 1;  /**< A correctable ECC error (single-bit error) was detected during
                                                         a read from the queue RAM. */
	uint64_t nerr                         : 1;  /**< An error occurred during an NCB operation.  This can include an
                                                         unsupported operation such as a byte-write, or a STDN response
                                                         with an error. */
	uint64_t bcmd                         : 1;  /**< An undefined or illegal queue command was encountered.  This can
                                                         result from an undefined command opcode, a job (ADDJOB/CONTJOB) with
                                                         an immediate-execution QID, a MAB response command in a queue, or a
                                                         command from a MAB with an invalid QID.
                                                         PSM_ERRCAP_QUEUE_BADCMD and PSM_ERRCAP_MABFIFO_BADCMD report which
                                                         queue or MAB had a bad command. Details on the QID and op-code for the
                                                         bad command can be found in either PSM_QUEUE_INFO() or
                                                         PSM_MABFIFO_CTRL() registers. */
	uint64_t lroll                        : 1;  /**< The job log pointer rolled over within the circular buffer. */
	uint64_t lovf                         : 1;  /**< The job log write buffer overflowed before the log data could be
                                                         written to system memory. */
	uint64_t qecc                         : 1;  /**< An uncorrectable ECC error (double-bit error) was detected during
                                                         a read from the queue RAM. */
	uint64_t bkpt                         : 1;  /**< Trace interrupt from either a job completion or a job dispatch according
                                                         to the debug breakpoint settings. */
	uint64_t sferr                        : 1;  /**< The tick counter reached the maximum programmed value without
                                                         a SOSF being received. */
#else
	uint64_t sferr                        : 1;
	uint64_t bkpt                         : 1;
	uint64_t qecc                         : 1;
	uint64_t lovf                         : 1;
	uint64_t lroll                        : 1;
	uint64_t bcmd                         : 1;
	uint64_t nerr                         : 1;
	uint64_t qsbe                         : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_psm_int_sum_w1s_s         cnf75xx;
};
typedef union cvmx_psm_int_sum_w1s cvmx_psm_int_sum_w1s_t;

/**
 * cvmx_psm_job_req#
 *
 * These registers reports the current value of the job request signals of the queues, at various
 * priority levels.
 */
union cvmx_psm_job_reqx {
	uint64_t u64;
	struct cvmx_psm_job_reqx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t job_req                      : 48; /**< This field indicates the current value of the job request signals of the queues. */
#else
	uint64_t job_req                      : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_job_reqx_s            cnf75xx;
};
typedef union cvmx_psm_job_reqx cvmx_psm_job_reqx_t;

/**
 * cvmx_psm_job_unserved#
 *
 * These registers reports the current value of the job unserved vector of the queues, at various
 * priority levels.
 */
union cvmx_psm_job_unservedx {
	uint64_t u64;
	struct cvmx_psm_job_unservedx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t unserved_vec                 : 48; /**< This register reports the current value of the job unserved vector of the queues, at
                                                         various priority levels. */
#else
	uint64_t unserved_vec                 : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_job_unservedx_s       cnf75xx;
};
typedef union cvmx_psm_job_unservedx cvmx_psm_job_unservedx_t;

/**
 * cvmx_psm_log_base
 *
 * This register sets the location in memory of the PSM log circular buffer.
 * Note that this must point to a main memory address, the log cannot be
 * stored in BPHY SMEM.
 */
union cvmx_psm_log_base {
	uint64_t u64;
	struct cvmx_psm_log_base_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_49_63               : 15;
	uint64_t base_ptr                     : 49; /**< Base address for the job log. Must be cacheline (128 byte) aligned. */
#else
	uint64_t base_ptr                     : 49;
	uint64_t reserved_49_63               : 15;
#endif
	} s;
	struct cvmx_psm_log_base_s            cnf75xx;
};
typedef union cvmx_psm_log_base cvmx_psm_log_base_t;

/**
 * cvmx_psm_log_cfg
 *
 * This register configures the job log circular buffer and enables the
 * job logging feature. The progress of written log entries can be tracked
 * usign the PSM_LOG_PTR register. When the pointer wraps around to the
 * beginning of the circular buffer, the PSM_LOG_ROLLOVER interrupt is
 * triggered.
 *
 * Each log entry is 128 bits. Entries for job starts use the format
 * specified by PSM_START_LOG_S, and job completions and errors are logged
 * using the format specified by PSM_RESP_LOG_S.
 *
 * Log entries are buffered in an internal buffer before being written 128
 * bytes at a time (i.e., 8 entries at a time). If PSM_LOG_CFG[FLUSH] is
 * written and the number of entries in the internal buffer is not a multiple
 * of 8, then the log will be padded with zeros to account for missing
 * entries. The internal buffer can store up to 32 entries. If a new job
 * starts or a job completes while the buffer is full and waiting for a write
 * to complete, then a PSM_LOG_WRBUF_OVERFLOW interrupt is triggered.
 */
union cvmx_psm_log_cfg {
	uint64_t u64;
	struct cvmx_psm_log_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t log_thold                    : 3;  /**< Job log write buffer threshhold for initiating a write operation
                                                         for dumping log entries to memory.  The threshhold value is in
                                                         units of 8 log entry chunks (128 bytes). The valid range is 1-4, and a
                                                         threshold of 4 will likely result in frequent overflow errors. */
	uint64_t reserved_41_47               : 7;
	uint64_t flush                        : 1;  /**< When written 1, flushes any pending log writes remaining
                                                         in the internal buffer. */
	uint64_t reserved_34_39               : 6;
	uint64_t log_done_en                  : 1;  /**< Enable output log of all job completions and errors. */
	uint64_t log_start_en                 : 1;  /**< Enable output log of all job starts. */
	uint64_t reserved_16_31               : 16;
	uint64_t log_size                     : 16; /**< Size in cache lines (128 byte units) of the circular buffer log. */
#else
	uint64_t log_size                     : 16;
	uint64_t reserved_16_31               : 16;
	uint64_t log_start_en                 : 1;
	uint64_t log_done_en                  : 1;
	uint64_t reserved_34_39               : 6;
	uint64_t flush                        : 1;
	uint64_t reserved_41_47               : 7;
	uint64_t log_thold                    : 3;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_psm_log_cfg_s             cnf75xx;
};
typedef union cvmx_psm_log_cfg cvmx_psm_log_cfg_t;

/**
 * cvmx_psm_log_ptr
 *
 * This register indicates the current job log pointer, where the next
 * job log entry will be written. When the pointer wraps around to the
 * beginning of the circular buffer, the PSM_LOG_ROLLOVER interrupt is
 * triggered.
 */
union cvmx_psm_log_ptr {
	uint64_t u64;
	struct cvmx_psm_log_ptr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_49_63               : 15;
	uint64_t log_ptr                      : 49; /**< Address of the current output pointer for the job trace log. Note that
                                                         PSM always writes the log 128 bytes at a time. */
#else
	uint64_t log_ptr                      : 49;
	uint64_t reserved_49_63               : 15;
#endif
	} s;
	struct cvmx_psm_log_ptr_s             cnf75xx;
};
typedef union cvmx_psm_log_ptr cvmx_psm_log_ptr_t;

/**
 * cvmx_psm_mab_res
 *
 * This register reports the current value of the per-MegaAB reservation vector, for CONT_JOB
 * commands.
 */
union cvmx_psm_mab_res {
	uint64_t u64;
	struct cvmx_psm_mab_res_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t mabres                       : 48; /**< This field reports the current value of the per-MegaAB reservation vector, for CONT_JOB
                                                         commands. */
#else
	uint64_t mabres                       : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_mab_res_s             cnf75xx;
};
typedef union cvmx_psm_mab_res cvmx_psm_mab_res_t;

/**
 * cvmx_psm_mabfifo_ctrl#
 *
 * These registers contain internal information about the MHAB/MDAB FIFOs.
 * I.e., the FIFOs of commands receieved from MHABs/MDABs.
 */
union cvmx_psm_mabfifo_ctrlx {
	uint64_t u64;
	struct cvmx_psm_mabfifo_ctrlx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_61_63               : 3;
	uint64_t errcap_qid                   : 5;  /**< Contains the QID for the most recent bad command encountered by the MHAB/MDAB FIFO.  Write
                                                         any nonzero value to clear. */
	uint64_t reserved_53_55               : 3;
	uint64_t errcap_opc                   : 5;  /**< Contains the opcode for the most recent bad command encountered by the MHAB/MDAB FIFO.
                                                         Write any nonzero value to clear. */
	uint64_t reserved_35_47               : 13;
	uint64_t req_cnt                      : 3;  /**< Contains the number of pending JCA requests made by the MHAB/MDAB that have not yet been
                                                         granted. */
	uint64_t reserved_27_31               : 5;
	uint64_t fifo_level                   : 3;  /**< Contains the number of entries held in the MHAB/MDAB FIFO. */
	uint64_t reserved_11_23               : 13;
	uint64_t fifo_cdt                     : 3;  /**< Contains the number of credits held by the MHAB/MDAB for making JCA requests. */
	uint64_t reserved_1_7                 : 7;
	uint64_t mabfifo_dis                  : 1;  /**< N/A */
#else
	uint64_t mabfifo_dis                  : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t fifo_cdt                     : 3;
	uint64_t reserved_11_23               : 13;
	uint64_t fifo_level                   : 3;
	uint64_t reserved_27_31               : 5;
	uint64_t req_cnt                      : 3;
	uint64_t reserved_35_47               : 13;
	uint64_t errcap_opc                   : 5;
	uint64_t reserved_53_55               : 3;
	uint64_t errcap_qid                   : 5;
	uint64_t reserved_61_63               : 3;
#endif
	} s;
	struct cvmx_psm_mabfifo_ctrlx_s       cnf75xx;
};
typedef union cvmx_psm_mabfifo_ctrlx cvmx_psm_mabfifo_ctrlx_t;

/**
 * cvmx_psm_mabfifo_head_hi#
 *
 * These registers contains the high 64-bits of the MAB FIFO head command.
 *
 */
union cvmx_psm_mabfifo_head_hix {
	uint64_t u64;
	struct cvmx_psm_mabfifo_head_hix_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t head_hi                      : 64; /**< Contains the upper 64-bits of the MAB FIFO head command. */
#else
	uint64_t head_hi                      : 64;
#endif
	} s;
	struct cvmx_psm_mabfifo_head_hix_s    cnf75xx;
};
typedef union cvmx_psm_mabfifo_head_hix cvmx_psm_mabfifo_head_hix_t;

/**
 * cvmx_psm_mabfifo_head_lo#
 *
 * These registers contains the low 64-bits of the MAB FIFO head command.
 *
 */
union cvmx_psm_mabfifo_head_lox {
	uint64_t u64;
	struct cvmx_psm_mabfifo_head_lox_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t head_lo                      : 64; /**< Contains the low 64-bits of the MAB FIFO head command. */
#else
	uint64_t head_lo                      : 64;
#endif
	} s;
	struct cvmx_psm_mabfifo_head_lox_s    cnf75xx;
};
typedef union cvmx_psm_mabfifo_head_lox cvmx_psm_mabfifo_head_lox_t;

/**
 * cvmx_psm_max_job_cdt#
 *
 * These registers define the maximum number of jobs that PSM can submit to
 * each MHAB/MDAB at a time.
 */
union cvmx_psm_max_job_cdtx {
	uint64_t u64;
	struct cvmx_psm_max_job_cdtx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t max                          : 4;  /**< Maximum number of jobs for MHAB/MDAB [a]. Valid range is [0,11]. */
#else
	uint64_t max                          : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_psm_max_job_cdtx_s        cnf75xx;
};
typedef union cvmx_psm_max_job_cdtx cvmx_psm_max_job_cdtx_t;

/**
 * cvmx_psm_nonjob_rsrc#
 *
 * These registers reports the status of nonjob resources.
 *
 */
union cvmx_psm_nonjob_rsrcx {
	uint64_t u64;
	struct cvmx_psm_nonjob_rsrcx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t rsrc_in_use                  : 1;  /**< This bit indicates if the nonjob resource is in use. */
	uint64_t reserved_7_15                : 9;
	uint64_t rsrc_owner_type              : 1;  /**< This bit indicates if the nonjob resource is currently owned by a MAB FIFO (1) or a
                                                         command queue (0). */
	uint64_t rsrc_owner                   : 6;  /**< This field shows the identity of nonjob resource owner. */
#else
	uint64_t rsrc_owner                   : 6;
	uint64_t rsrc_owner_type              : 1;
	uint64_t reserved_7_15                : 9;
	uint64_t rsrc_in_use                  : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_psm_nonjob_rsrcx_s        cnf75xx;
};
typedef union cvmx_psm_nonjob_rsrcx cvmx_psm_nonjob_rsrcx_t;

/**
 * cvmx_psm_queue_busy_sts
 *
 * This register contains busy status bits for each queue.
 *
 */
union cvmx_psm_queue_busy_sts {
	uint64_t u64;
	struct cvmx_psm_queue_busy_sts_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t busy                         : 48; /**< Busy status.
                                                         0 = No commands queued or in-flight.
                                                         1 = Queue busy. The queue has at least one command waiting to execute
                                                         or actively being executed by a MHAB/MDAB. */
#else
	uint64_t busy                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_queue_busy_sts_s      cnf75xx;
};
typedef union cvmx_psm_queue_busy_sts cvmx_psm_queue_busy_sts_t;

/**
 * cvmx_psm_queue_cfg#
 *
 * These registers configure the start and end of each circular command
 * queue within the queue RAM. The queue RAM contains 8192 entries, and
 * software must allocate the queues so that they do not overlap. These
 * registers should only be written during initial PSM setup when the PSM is
 * idle.
 *
 * Note that writing to a PSM_QUEUE_CFG() register will automatically reset
 * the head and tail pointers of the corresponding queue. Any commands in the
 * queue before the write will be dropped and the queue will be reset to an
 * empty state.
 *
 * Also, note that at reset, all queues are effectively uninitialized, and
 * PSM_QUEUE_SPACE() will return 0. Queues must be initializd by writing to
 * PSM_QUEUE_CFG() before they can be used. This is true even in the case of
 * a single-entry queue that starts and ends at entry 0.
 */
union cvmx_psm_queue_cfgx {
	uint64_t u64;
	struct cvmx_psm_queue_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t prio                         : 2;  /**< Queue priority. 0x0 is the highest priority and 0x3 is the lowest. */
	uint64_t reserved_29_31               : 3;
	uint64_t top                          : 13; /**< Indicates the highest queue RAM address allocated for the
                                                         queue. Must be greater than the BASE address pointer. */
	uint64_t reserved_13_15               : 3;
	uint64_t base                         : 13; /**< Indicates the lowest queue RAM address allocated for the
                                                         queue. Must be lower than the TOP address pointer. */
#else
	uint64_t base                         : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t top                          : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t prio                         : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_psm_queue_cfgx_s          cnf75xx;
};
typedef union cvmx_psm_queue_cfgx cvmx_psm_queue_cfgx_t;

/**
 * cvmx_psm_queue_cmd_hi#
 *
 * These registers are aliased to the 48 command queues. When writing this aliased
 * register alone, the 64-bit value will be combined with the previously written
 * PSM_QUEUE_CMD_LO() value that is held in a temporary register to produce the 128-bit PSM
 * command.  Upon writing the PSM_QUEUE_CMD_HI(), the PSM command will be enqueued.
 * If the queue is full, a PSM_QUEUE_OVERFLOW interrupt is triggered, and the
 * command is dropped.
 */
union cvmx_psm_queue_cmd_hix {
	uint64_t u64;
	struct cvmx_psm_queue_cmd_hix_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t command_hi                   : 64; /**< The high 64 bits of a PSM command. */
#else
	uint64_t command_hi                   : 64;
#endif
	} s;
	struct cvmx_psm_queue_cmd_hix_s       cnf75xx;
};
typedef union cvmx_psm_queue_cmd_hix cvmx_psm_queue_cmd_hix_t;

/**
 * cvmx_psm_queue_cmd_lo#
 *
 * These registers are aliased to the 48 command queues. When writing the
 * aliased register PSM_QUEUE_CMD_LO(), the 64-bit value will be held in a per-queue
 * temporary register.  The command will only be added to a queue when the corresponding
 * PSM_QUEUE_CMD_HI() register is written. If the queue is full, a
 * PSM_QUEUE_OVERFLOW interrupt is triggered and the command is dropped.
 */
union cvmx_psm_queue_cmd_lox {
	uint64_t u64;
	struct cvmx_psm_queue_cmd_lox_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t command_lo                   : 64; /**< The low 64 bits of a PSM command. */
#else
	uint64_t command_lo                   : 64;
#endif
	} s;
	struct cvmx_psm_queue_cmd_lox_s       cnf75xx;
};
typedef union cvmx_psm_queue_cmd_lox cvmx_psm_queue_cmd_lox_t;

/**
 * cvmx_psm_queue_ena_w1c
 *
 * This register contains disable bits for each queue.
 *
 */
union cvmx_psm_queue_ena_w1c {
	uint64_t u64;
	struct cvmx_psm_queue_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t dis                          : 48; /**< Disable bit for each queue. If written 1, the queue will be disabled.
                                                         If written 0, the queue enable status will be unmodified.  Queues may be
                                                         enabled using the PSM_QUEUE_ENA_W1S register.  When disabled,
                                                         jobs may be added to the queue, but those jobs will not be scheduled for execution. */
#else
	uint64_t dis                          : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_queue_ena_w1c_s       cnf75xx;
};
typedef union cvmx_psm_queue_ena_w1c cvmx_psm_queue_ena_w1c_t;

/**
 * cvmx_psm_queue_ena_w1s
 *
 * This register contains enable bits for each queue.
 *
 */
union cvmx_psm_queue_ena_w1s {
	uint64_t u64;
	struct cvmx_psm_queue_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ena                          : 48; /**< Enable bit for each queue. If written 1, the queue will be enabled.
                                                         If written 0, the queue enable will be unmodified.  Queues may be
                                                         disabled using the PSM_QUEUE_ENA_W1C register.  When disabled, jobs
                                                         may be added to the queue, but those jobs will not be scheduled for
                                                         execution until the queue is enabled. */
#else
	uint64_t ena                          : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_queue_ena_w1s_s       cnf75xx;
};
typedef union cvmx_psm_queue_ena_w1s cvmx_psm_queue_ena_w1s_t;

/**
 * cvmx_psm_queue_full_sts
 *
 * This register contains full indicators for each queue.
 *
 */
union cvmx_psm_queue_full_sts {
	uint64_t u64;
	struct cvmx_psm_queue_full_sts_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t full                         : 48; /**< For each queue, a 1 bit indicates the queue is full, a 0 indicates
                                                         the queue is not full and a cnMIPS core can enqueue a job. */
#else
	uint64_t full                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_psm_queue_full_sts_s      cnf75xx;
};
typedef union cvmx_psm_queue_full_sts cvmx_psm_queue_full_sts_t;

/**
 * cvmx_psm_queue_info#
 *
 * These registers contain internal information about the queue.
 *
 */
union cvmx_psm_queue_infox {
	uint64_t u64;
	struct cvmx_psm_queue_infox_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t cur_cmd_lo                   : 24; /**< N/A */
	uint64_t reserved_38_39               : 2;
	uint64_t cont_mab_id                  : 6;  /**< IN_CONT_SEQ is set to one, this field provides the MAB ID of the
                                                         MHAB/MDAB reserved by the CONTJOB. */
	uint64_t reserved_29_31               : 3;
	uint64_t badcmd_opc                   : 5;  /**< Contains the opcode for the most recent bad command.  Write any nonzero value to clear. */
	uint64_t runjob_ctr                   : 8;  /**< Contains the current count of running jobs issued by the queue. */
	uint64_t reserved_15_15               : 1;
	uint64_t queue_njreq                  : 3;  /**< N/A */
	uint64_t state                        : 4;  /**< N/A */
	uint64_t queue_jobreq                 : 1;  /**< N/A */
	uint64_t cont_job_wait_cdt            : 1;  /**< N/A */
	uint64_t cont_job_wait_done           : 1;  /**< N/A */
	uint64_t rdy_for_followup             : 1;  /**< When set, indicates the queue is ready to schedule a follow-up job
                                                         after completing a continued job. */
	uint64_t in_cont_seq                  : 1;  /**< When set, indicates that the queue holds a reservation from a
                                                         continued job. */
	uint64_t cur_cmd_vld                  : 1;  /**< N/A */
	uint64_t new_cmdlo_vld                : 1;  /**< N/A */
	uint64_t new_cmd_vld                  : 1;  /**< N/A */
#else
	uint64_t new_cmd_vld                  : 1;
	uint64_t new_cmdlo_vld                : 1;
	uint64_t cur_cmd_vld                  : 1;
	uint64_t in_cont_seq                  : 1;
	uint64_t rdy_for_followup             : 1;
	uint64_t cont_job_wait_done           : 1;
	uint64_t cont_job_wait_cdt            : 1;
	uint64_t queue_jobreq                 : 1;
	uint64_t state                        : 4;
	uint64_t queue_njreq                  : 3;
	uint64_t reserved_15_15               : 1;
	uint64_t runjob_ctr                   : 8;
	uint64_t badcmd_opc                   : 5;
	uint64_t reserved_29_31               : 3;
	uint64_t cont_mab_id                  : 6;
	uint64_t reserved_38_39               : 2;
	uint64_t cur_cmd_lo                   : 24;
#endif
	} s;
	struct cvmx_psm_queue_infox_s         cnf75xx;
};
typedef union cvmx_psm_queue_infox cvmx_psm_queue_infox_t;

/**
 * cvmx_psm_queue_ptr#
 *
 * These registers return the status of each queue's head and tail pointers.
 *
 */
union cvmx_psm_queue_ptrx {
	uint64_t u64;
	struct cvmx_psm_queue_ptrx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t full                         : 1;  /**< Indicates the queue is full. */
	uint64_t notempty                     : 1;  /**< Indicates there are commands in the queue. */
	uint64_t reserved_29_31               : 3;
	uint64_t tail                         : 13; /**< Tail pointer in the queue. This index points to the location where the
                                                         next item will be added to the queue. */
	uint64_t reserved_13_15               : 3;
	uint64_t head                         : 13; /**< The head pointer in the queue. Points to the next entry to be executed
                                                         from the queue. */
#else
	uint64_t head                         : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t tail                         : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t notempty                     : 1;
	uint64_t full                         : 1;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_psm_queue_ptrx_s          cnf75xx;
};
typedef union cvmx_psm_queue_ptrx cvmx_psm_queue_ptrx_t;

/**
 * cvmx_psm_queue_ram#
 *
 * These addresses provide a debug mechanism into the queue RAM. Both read
 * and write accesses are allowed into individual queue RAM entries.  The QRAM
 * entries are 128-bit entities, and must be written in a proper sequence due
 * to ECC logic.  The lower 64-bit word must be written first, followed by the
 * upper 64-bit word.  No other QRAM write may occur between writing the two
 * 64-bit words.
 */
union cvmx_psm_queue_ramx {
	uint64_t u64;
	struct cvmx_psm_queue_ramx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< The queue RAM entry data. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_psm_queue_ramx_s          cnf75xx;
};
typedef union cvmx_psm_queue_ramx cvmx_psm_queue_ramx_t;

/**
 * cvmx_psm_queue_space#
 *
 * These registers report the number of free entries in each queue.
 *
 */
union cvmx_psm_queue_spacex {
	uint64_t u64;
	struct cvmx_psm_queue_spacex_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t space                        : 14; /**< The number of commands that can be added to the queue. */
#else
	uint64_t space                        : 14;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_psm_queue_spacex_s        cnf75xx;
};
typedef union cvmx_psm_queue_spacex cvmx_psm_queue_spacex_t;

/**
 * cvmx_psm_queue_timeout_cfg#
 *
 * These registers program the queue watchdog timers. Each queue has a
 * watchdog timer that increments by one on every tick of the PSM timer if
 * the queue is enabled and not empty. The watchdog timer is reset each time
 * the queue launches a job or becomes idle (i.e., empty). If the watchdog
 * timer reaches the maximum timeout, a PSM_QUEUE_TIMEOUT() interrupt is
 * signalled.
 */
union cvmx_psm_queue_timeout_cfgx {
	uint64_t u64;
	struct cvmx_psm_queue_timeout_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_52_63               : 12;
	uint64_t wdog_timer                   : 20; /**< Current value of the queue's watchdog timer. */
	uint64_t reserved_20_31               : 12;
	uint64_t qtime_max                    : 20; /**< Maxium value of the watchdog timer.  A value of 0 will disable the
                                                         timeout check. */
#else
	uint64_t qtime_max                    : 20;
	uint64_t reserved_20_31               : 12;
	uint64_t wdog_timer                   : 20;
	uint64_t reserved_52_63               : 12;
#endif
	} s;
	struct cvmx_psm_queue_timeout_cfgx_s  cnf75xx;
};
typedef union cvmx_psm_queue_timeout_cfgx cvmx_psm_queue_timeout_cfgx_t;

/**
 * cvmx_psm_rsrc_tbl#
 *
 * These registers define a mapping table of job types to MHAB(s)/MDAB(s).
 * Multiple MHAB/MDABs may be assigned to service jobs of each type, and a
 * given MHAB/MDAB can be assigned to any number of job types.
 */
union cvmx_psm_rsrc_tblx {
	uint64_t u64;
	struct cvmx_psm_rsrc_tblx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mab_map                      : 64; /**< Bit mask indicating which MHAB/MDABs can accept jobs of type [a].
                                                         Bits 0-19 represent the MHABs, and bits 24-41 represent MDABs 0-17. */
#else
	uint64_t mab_map                      : 64;
#endif
	} s;
	struct cvmx_psm_rsrc_tblx_s           cnf75xx;
};
typedef union cvmx_psm_rsrc_tblx cvmx_psm_rsrc_tblx_t;

/**
 * cvmx_psm_rst
 *
 * This register provides a mechanism to reset the entire PSM or
 * individual PSM queues.
 */
union cvmx_psm_rst {
	uint64_t u64;
	struct cvmx_psm_rst_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t psm_reset                    : 1;  /**< When set, will reset entire PSM to hardware reset state. */
	uint64_t reserved_48_62               : 15;
	uint64_t queue_reset                  : 48; /**< Each bit written 1 will reset an individual queue.  Reset will empty the queue
                                                         but not reset any in-flight jobs dispatched from the given queue, and
                                                         will not change the size of the queue. */
#else
	uint64_t queue_reset                  : 48;
	uint64_t reserved_48_62               : 15;
	uint64_t psm_reset                    : 1;
#endif
	} s;
	struct cvmx_psm_rst_s                 cnf75xx;
};
typedef union cvmx_psm_rst cvmx_psm_rst_t;

/**
 * cvmx_psm_sclk_dll_status
 *
 * Status of the ROC SCLK DLL.
 *
 */
union cvmx_psm_sclk_dll_status {
	uint64_t u64;
	struct cvmx_psm_sclk_dll_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_60_63               : 4;
	uint64_t max_dll_setting              : 12; /**< Max reported DLL setting. */
	uint64_t min_dll_setting              : 12; /**< Min reported DLL setting. */
	uint64_t pd_pos_sclk_refclk           : 1;  /**< Synchronized pd_pos_sclk_refclk from ROC SCLK DLL cmb0 phase detectors. */
	uint64_t pdl_sclk_refclk              : 1;  /**< Synchronized pdl_sclk_refclk from ROC SCLK DLL cmb0 phase detectors. */
	uint64_t pdr_sclk_refclk              : 1;  /**< Synchronized pdr_sclk_refclk from ROC SCLK DLL cmb0 phase detectors. */
	uint64_t reserved_32_32               : 1;
	uint64_t dll_dly_elem_en              : 16; /**< The ROC SCLK delay element enable setting, from the negative edge of refclk. */
	uint64_t dll_setting                  : 12; /**< The ROC SCLK DLL setting, from the negative edge of refclk. */
	uint64_t reserved_1_3                 : 3;
	uint64_t dll_lock                     : 1;  /**< The dll_lock signal from ROC SCLK DLL, from the positive edge of refclk. */
#else
	uint64_t dll_lock                     : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t dll_setting                  : 12;
	uint64_t dll_dly_elem_en              : 16;
	uint64_t reserved_32_32               : 1;
	uint64_t pdr_sclk_refclk              : 1;
	uint64_t pdl_sclk_refclk              : 1;
	uint64_t pd_pos_sclk_refclk           : 1;
	uint64_t min_dll_setting              : 12;
	uint64_t max_dll_setting              : 12;
	uint64_t reserved_60_63               : 4;
#endif
	} s;
	struct cvmx_psm_sclk_dll_status_s     cnf75xx;
};
typedef union cvmx_psm_sclk_dll_status cvmx_psm_sclk_dll_status_t;

/**
 * cvmx_psm_timer_cfg
 *
 * This register configures the PSM timer.
 *
 */
union cvmx_psm_timer_cfg {
	uint64_t u64;
	struct cvmx_psm_timer_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ena                          : 1;  /**< Timer enable. */
	uint64_t reserved_49_62               : 14;
	uint64_t bcast_mode                   : 1;  /**< Enables the timer broadcast mode. When set, the PSM will send a
                                                         message to the MDABs each time it receives an SOS from the RFIF. */
	uint64_t reserved_41_47               : 7;
	uint64_t frame_mode                   : 1;  /**< Selects the mode for counting frames and sub-frames.
                                                         0 = PSM mode. PSM advances the subframe/frame upon receiving SOS from
                                                         RFIF.
                                                         1 = RFIF mode. PSM uses the subframe/frame values supplied by RFIF on
                                                         each SOS. */
	uint64_t reserved_32_39               : 8;
	uint64_t tick_max                     : 16; /**< Sets the maximum tick count value.  If the tick counter exceeds this
                                                         maximum value without receiving a SOS from RFIF, a PSM_SOSF_ERROR
                                                         interrupt will be raised. */
	uint64_t reserved_8_15                : 8;
	uint64_t tick_div                     : 8;  /**< Divider value for generating the tick clock from the SCLK.
                                                         PSM_TIMER_VAL[TICK] is incremented by one every TICK_DIV cycles. */
#else
	uint64_t tick_div                     : 8;
	uint64_t reserved_8_15                : 8;
	uint64_t tick_max                     : 16;
	uint64_t reserved_32_39               : 8;
	uint64_t frame_mode                   : 1;
	uint64_t reserved_41_47               : 7;
	uint64_t bcast_mode                   : 1;
	uint64_t reserved_49_62               : 14;
	uint64_t ena                          : 1;
#endif
	} s;
	struct cvmx_psm_timer_cfg_s           cnf75xx;
};
typedef union cvmx_psm_timer_cfg cvmx_psm_timer_cfg_t;

/**
 * cvmx_psm_timer_val
 *
 * This register holds the frame/subframe count and timer tick within the
 * subframe. [TICK] is incremented on the divided clock (specified
 * by PSM_TIMER_CFG[TICK_DIV]) and reset on every SOS. Based on PSM_TIMER_CFG[FRAME_MODE], the
 * frame and subframe counts are either captured from the RFIF
 * (PSM_TIMER_CFG[FRAME_MODE] = 1), or automatically incremented on each SOS
 * received from RFIF.
 * (PSM_TIMER_CFG[FRAME_MODE] = 0). This PSM timer is used for timestamps in
 * job log entries, when writing status for WRSTS commands, and for WAIT
 * commands.
 *
 * If no SOS is received from RFIF, then [TICK] will continue to increment,
 * eventually wrapping around to zero.
 */
union cvmx_psm_timer_val {
	uint64_t u64;
	struct cvmx_psm_timer_val_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_59_63               : 5;
	uint64_t frame_update                 : 1;  /**< If written 1, update [FRAME] count.  If written 0, the [FRAME]
                                                         count remains unchanged. */
	uint64_t sf_update                    : 1;  /**< If written 1, update [SUBFRAME] count.  If written 0, the [SUBFRAME]
                                                         count remains unchanged. */
	uint64_t tick_update                  : 1;  /**< If written 1, update [TICK] count.  If written 0, the [TICK]
                                                         count remains unchanged. */
	uint64_t reserved_32_55               : 24;
	uint64_t frame                        : 12; /**< Frame count (i.e., BFN). */
	uint64_t subframe                     : 4;  /**< Subframe count. */
	uint64_t tick                         : 16; /**< Current timer count. When enabled, a SOS signal from RFIF will reset
                                                         [TICK] to 0.  Otherwise [TICK] increments by one every
                                                         PSM_TIMER_CFG[TICK_DIV] cycles. */
#else
	uint64_t tick                         : 16;
	uint64_t subframe                     : 4;
	uint64_t frame                        : 12;
	uint64_t reserved_32_55               : 24;
	uint64_t tick_update                  : 1;
	uint64_t sf_update                    : 1;
	uint64_t frame_update                 : 1;
	uint64_t reserved_59_63               : 5;
#endif
	} s;
	struct cvmx_psm_timer_val_s           cnf75xx;
};
typedef union cvmx_psm_timer_val cvmx_psm_timer_val_t;

#endif
