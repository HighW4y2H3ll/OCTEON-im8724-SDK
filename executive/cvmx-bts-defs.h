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
 * cvmx-bts-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon bts.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_BTS_DEFS_H__
#define __CVMX_BTS_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_CG_1PPS_CFG CVMX_BTS_CG_1PPS_CFG_FUNC()
static inline uint64_t CVMX_BTS_CG_1PPS_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_CG_1PPS_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000260ull);
}
#else
#define CVMX_BTS_CG_1PPS_CFG (CVMX_ADD_IO_SEG(0x00011800B6000260ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_CG_CFG CVMX_BTS_CG_CFG_FUNC()
static inline uint64_t CVMX_BTS_CG_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_CG_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000408ull);
}
#else
#define CVMX_BTS_CG_CFG (CVMX_ADD_IO_SEG(0x00011800B6000408ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_CG_CTL CVMX_BTS_CG_CTL_FUNC()
static inline uint64_t CVMX_BTS_CG_CTL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_CG_CTL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000400ull);
}
#else
#define CVMX_BTS_CG_CTL (CVMX_ADD_IO_SEG(0x00011800B6000400ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_ECO CVMX_BTS_ECO_FUNC()
static inline uint64_t CVMX_BTS_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000090ull);
}
#else
#define CVMX_BTS_ECO (CVMX_ADD_IO_SEG(0x00011800B6000090ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_EXT_REF0_DIV_CFG0 CVMX_BTS_EXT_REF0_DIV_CFG0_FUNC()
static inline uint64_t CVMX_BTS_EXT_REF0_DIV_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_EXT_REF0_DIV_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000210ull);
}
#else
#define CVMX_BTS_EXT_REF0_DIV_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000210ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_EXT_REF0_DIV_CFG1 CVMX_BTS_EXT_REF0_DIV_CFG1_FUNC()
static inline uint64_t CVMX_BTS_EXT_REF0_DIV_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_EXT_REF0_DIV_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000218ull);
}
#else
#define CVMX_BTS_EXT_REF0_DIV_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000218ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_EXT_REF1_DIV_CFG0 CVMX_BTS_EXT_REF1_DIV_CFG0_FUNC()
static inline uint64_t CVMX_BTS_EXT_REF1_DIV_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_EXT_REF1_DIV_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000220ull);
}
#else
#define CVMX_BTS_EXT_REF1_DIV_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000220ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_EXT_REF1_DIV_CFG1 CVMX_BTS_EXT_REF1_DIV_CFG1_FUNC()
static inline uint64_t CVMX_BTS_EXT_REF1_DIV_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_EXT_REF1_DIV_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000228ull);
}
#else
#define CVMX_BTS_EXT_REF1_DIV_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000228ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GLOBAL_CTL CVMX_BTS_GLOBAL_CTL_FUNC()
static inline uint64_t CVMX_BTS_GLOBAL_CTL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GLOBAL_CTL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000010ull);
}
#else
#define CVMX_BTS_GLOBAL_CTL (CVMX_ADD_IO_SEG(0x00011800B6000010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GLOBAL_STATUS CVMX_BTS_GLOBAL_STATUS_FUNC()
static inline uint64_t CVMX_BTS_GLOBAL_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GLOBAL_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000018ull);
}
#else
#define CVMX_BTS_GLOBAL_STATUS (CVMX_ADD_IO_SEG(0x00011800B6000018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GPS_1PPS_CFG CVMX_BTS_GPS_1PPS_CFG_FUNC()
static inline uint64_t CVMX_BTS_GPS_1PPS_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GPS_1PPS_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000250ull);
}
#else
#define CVMX_BTS_GPS_1PPS_CFG (CVMX_ADD_IO_SEG(0x00011800B6000250ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_1PPS_CFG CVMX_BTS_GRFE_1PPS_CFG_FUNC()
static inline uint64_t CVMX_BTS_GRFE_1PPS_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_1PPS_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000128ull);
}
#else
#define CVMX_BTS_GRFE_1PPS_CFG (CVMX_ADD_IO_SEG(0x00011800B6000128ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_1PPS_EN CVMX_BTS_GRFE_1PPS_EN_FUNC()
static inline uint64_t CVMX_BTS_GRFE_1PPS_EN_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_1PPS_EN not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000120ull);
}
#else
#define CVMX_BTS_GRFE_1PPS_EN (CVMX_ADD_IO_SEG(0x00011800B6000120ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_CFG0 CVMX_BTS_GRFE_CFG0_FUNC()
static inline uint64_t CVMX_BTS_GRFE_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000100ull);
}
#else
#define CVMX_BTS_GRFE_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_CFG1 CVMX_BTS_GRFE_CFG1_FUNC()
static inline uint64_t CVMX_BTS_GRFE_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000108ull);
}
#else
#define CVMX_BTS_GRFE_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000108ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_CFG2 CVMX_BTS_GRFE_CFG2_FUNC()
static inline uint64_t CVMX_BTS_GRFE_CFG2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_CFG2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000110ull);
}
#else
#define CVMX_BTS_GRFE_CFG2 (CVMX_ADD_IO_SEG(0x00011800B6000110ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_CNTR CVMX_BTS_GRFE_CNTR_FUNC()
static inline uint64_t CVMX_BTS_GRFE_CNTR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_CNTR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000130ull);
}
#else
#define CVMX_BTS_GRFE_CNTR (CVMX_ADD_IO_SEG(0x00011800B6000130ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_GRFE_STA CVMX_BTS_GRFE_STA_FUNC()
static inline uint64_t CVMX_BTS_GRFE_STA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_GRFE_STA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000118ull);
}
#else
#define CVMX_BTS_GRFE_STA (CVMX_ADD_IO_SEG(0x00011800B6000118ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_INT_SUM CVMX_BTS_INT_SUM_FUNC()
static inline uint64_t CVMX_BTS_INT_SUM_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_INT_SUM not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000080ull);
}
#else
#define CVMX_BTS_INT_SUM (CVMX_ADD_IO_SEG(0x00011800B6000080ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_NCB_CFG CVMX_BTS_NCB_CFG_FUNC()
static inline uint64_t CVMX_BTS_NCB_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_NCB_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000020ull);
}
#else
#define CVMX_BTS_NCB_CFG (CVMX_ADD_IO_SEG(0x00011800B6000020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PD1PPS_DIV_CFG0 CVMX_BTS_PD1PPS_DIV_CFG0_FUNC()
static inline uint64_t CVMX_BTS_PD1PPS_DIV_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PD1PPS_DIV_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000240ull);
}
#else
#define CVMX_BTS_PD1PPS_DIV_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000240ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PD1PPS_DIV_CFG1 CVMX_BTS_PD1PPS_DIV_CFG1_FUNC()
static inline uint64_t CVMX_BTS_PD1PPS_DIV_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PD1PPS_DIV_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000248ull);
}
#else
#define CVMX_BTS_PD1PPS_DIV_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000248ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PD3072M_DIV_CFG0 CVMX_BTS_PD3072M_DIV_CFG0_FUNC()
static inline uint64_t CVMX_BTS_PD3072M_DIV_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PD3072M_DIV_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000230ull);
}
#else
#define CVMX_BTS_PD3072M_DIV_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000230ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PD3072M_DIV_CFG1 CVMX_BTS_PD3072M_DIV_CFG1_FUNC()
static inline uint64_t CVMX_BTS_PD3072M_DIV_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PD3072M_DIV_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000238ull);
}
#else
#define CVMX_BTS_PD3072M_DIV_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000238ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PD_HISTORY CVMX_BTS_PD_HISTORY_FUNC()
static inline uint64_t CVMX_BTS_PD_HISTORY_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PD_HISTORY not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000380ull);
}
#else
#define CVMX_BTS_PD_HISTORY (CVMX_ADD_IO_SEG(0x00011800B6000380ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BTS_PD_SLICEX_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_BTS_PD_SLICEX_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B6000300ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_BTS_PD_SLICEX_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800B6000300ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BTS_PD_SLICEX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_BTS_PD_SLICEX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B6000340ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_BTS_PD_SLICEX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B6000340ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PLL_CTL CVMX_BTS_PLL_CTL_FUNC()
static inline uint64_t CVMX_BTS_PLL_CTL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PLL_CTL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000000ull);
}
#else
#define CVMX_BTS_PLL_CTL (CVMX_ADD_IO_SEG(0x00011800B6000000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_PTP_1PPS_CFG CVMX_BTS_PTP_1PPS_CFG_FUNC()
static inline uint64_t CVMX_BTS_PTP_1PPS_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_PTP_1PPS_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000258ull);
}
#else
#define CVMX_BTS_PTP_1PPS_CFG (CVMX_ADD_IO_SEG(0x00011800B6000258ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BTS_PWMX_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_BTS_PWMX_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B6000440ull) + ((offset) & 1) * 8;
}
#else
#define CVMX_BTS_PWMX_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800B6000440ull) + ((offset) & 1) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_SYNCE_25M_DIV_CFG0 CVMX_BTS_SYNCE_25M_DIV_CFG0_FUNC()
static inline uint64_t CVMX_BTS_SYNCE_25M_DIV_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_SYNCE_25M_DIV_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000200ull);
}
#else
#define CVMX_BTS_SYNCE_25M_DIV_CFG0 (CVMX_ADD_IO_SEG(0x00011800B6000200ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_SYNCE_25M_DIV_CFG1 CVMX_BTS_SYNCE_25M_DIV_CFG1_FUNC()
static inline uint64_t CVMX_BTS_SYNCE_25M_DIV_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_SYNCE_25M_DIV_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000208ull);
}
#else
#define CVMX_BTS_SYNCE_25M_DIV_CFG1 (CVMX_ADD_IO_SEG(0x00011800B6000208ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BTS_TP_MUX_SEL CVMX_BTS_TP_MUX_SEL_FUNC()
static inline uint64_t CVMX_BTS_TP_MUX_SEL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BTS_TP_MUX_SEL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B6000480ull);
}
#else
#define CVMX_BTS_TP_MUX_SEL (CVMX_ADD_IO_SEG(0x00011800B6000480ull))
#endif

/**
 * cvmx_bts_cg_1pps_cfg
 */
union cvmx_bts_cg_1pps_cfg {
	uint64_t u64;
	struct cvmx_bts_cg_1pps_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t reverse                      : 1;  /**< Set to 1 to invert the polarity of the CG_1PPS signal. */
#else
	uint64_t reverse                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bts_cg_1pps_cfg_s         cnf75xx;
};
typedef union cvmx_bts_cg_1pps_cfg cvmx_bts_cg_1pps_cfg_t;

/**
 * cvmx_bts_cg_cfg
 */
union cvmx_bts_cg_cfg {
	uint64_t u64;
	struct cvmx_bts_cg_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_44_63               : 20;
	uint64_t cnt_max_125us                : 12; /**< Maximum value for the 125 us counter used to time the BFN data signal.
                                                         Set to the number of cycles per 125 us minus 1. Default is 0xEFF
                                                         (3839) for a 30.72 MHz clock. */
	uint64_t reserved_19_31               : 13;
	uint64_t cnt_max_10ms                 : 19; /**< Maximum value for the 10 ms counter used for the RFP signal. Set to
                                                         the number of cycles per 10 ms minus 1. Default is 0x4AFFFF (307199)
                                                         for a 30.72 MHz clock. */
#else
	uint64_t cnt_max_10ms                 : 19;
	uint64_t reserved_19_31               : 13;
	uint64_t cnt_max_125us                : 12;
	uint64_t reserved_44_63               : 20;
#endif
	} s;
	struct cvmx_bts_cg_cfg_s              cnf75xx;
};
typedef union cvmx_bts_cg_cfg cvmx_bts_cg_cfg_t;

/**
 * cvmx_bts_cg_ctl
 *
 * This register controls the BFN and clocks produced by the clock generator.
 *
 */
union cvmx_bts_cg_ctl {
	uint64_t u64;
	struct cvmx_bts_cg_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t sw_1pps_delay                : 32; /**< Delay between PD_1PPS and CG_1PPS in 30.72MHz cycles. */
	uint64_t reserved_28_31               : 4;
	uint64_t sw_bfn                       : 12; /**< BFN sequence start number. This value is only loaded by hardware when
                                                         SW_BFN_2MSB_VLD=1 or SW_BFN_10LSB_VLD=1. */
	uint64_t smcell                       : 1;  /**< Small cell mode:
                                                         0 = Bypass clock generator and output RFP and BFN directly from
                                                         external pins (i.e., BPHY_RFP and BPHY_BFN).
                                                         1 = Output RFP and BFN produced by clock generator. */
	uint64_t reserved_5_14                : 10;
	uint64_t sw_bfn_2msb_vld              : 1;  /**< Set by software to indicate SW_BFN bits [11:10] are valid. The BFN
                                                         value will be updated at the next SW_1PPS pulse. Hardware will
                                                         automatically clear this bit once the value has been updated. */
	uint64_t reserved_1_3                 : 3;
	uint64_t sw_bfn_10lsb_vld             : 1;  /**< Set by software to indicate SW_BFN bits [9:0] are valid. The BFN
                                                         value will be updated at the next SW_1PPS pulse. Hardware will
                                                         automatically clear this bit once the value has been updated. */
#else
	uint64_t sw_bfn_10lsb_vld             : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t sw_bfn_2msb_vld              : 1;
	uint64_t reserved_5_14                : 10;
	uint64_t smcell                       : 1;
	uint64_t sw_bfn                       : 12;
	uint64_t reserved_28_31               : 4;
	uint64_t sw_1pps_delay                : 32;
#endif
	} s;
	struct cvmx_bts_cg_ctl_s              cnf75xx;
};
typedef union cvmx_bts_cg_ctl cvmx_bts_cg_ctl_t;

/**
 * cvmx_bts_eco
 *
 * An ECO CSR.
 *
 */
union cvmx_bts_eco {
	uint64_t u64;
	struct cvmx_bts_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_bts_eco_s                 cnf75xx;
};
typedef union cvmx_bts_eco cvmx_bts_eco_t;

/**
 * cvmx_bts_ext_ref0_div_cfg0
 *
 * This register configures the clock divider used to generate a 1PPS signal
 * from external reference clock 0 (i.e., EXTREF0_CLK).
 */
union cvmx_bts_ext_ref0_div_cfg0 {
	uint64_t u64;
	struct cvmx_bts_ext_ref0_div_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t clkout_bypass                : 1;  /**< Set to 1 to bypass the clock divider (clock out = EXTREF0_CLK). */
	uint64_t clkout_inv                   : 1;  /**< Set to 1 to inverse the clock output polarity. */
	uint64_t clkout_5050_duty             : 1;  /**< Clock divider duty cycle:
                                                         0 = one cycle pulse (i.e., one cycle of external reference clock).
                                                         1 = 50/50 duty-cycle. */
	uint64_t divider_en                   : 1;  /**< Set to 1 to enable the clock divider. The divider counter is reset to 0 when
                                                         [DIVIDER_EN] is set to 0 and the clock output is disabled. */
#else
	uint64_t divider_en                   : 1;
	uint64_t clkout_5050_duty             : 1;
	uint64_t clkout_inv                   : 1;
	uint64_t clkout_bypass                : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_bts_ext_ref0_div_cfg0_s   cnf75xx;
};
typedef union cvmx_bts_ext_ref0_div_cfg0 cvmx_bts_ext_ref0_div_cfg0_t;

/**
 * cvmx_bts_ext_ref0_div_cfg1
 *
 * This register configures the clock divider used to generate a 1PPS signal
 * from external reference clock 0 (i.e., EXTREF0_CLK).
 */
union cvmx_bts_ext_ref0_div_cfg1 {
	uint64_t u64;
	struct cvmx_bts_ext_ref0_div_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t divider_start_pos            : 32; /**< Divider start position. When a new value is written to
                                                         [DIVIDER_START_POS], the counter will reset to 0 the next time the
                                                         counter value is equal to [DIVIDER_START_POS]. Subsequently, the counter
                                                         will reset to 0 when the counter is equal to [DIVIDER_TC].
                                                         Writing [DIVIDER_START_POS] effectively applies a positive phase shift
                                                         of [DIVIDER_START_POS]+1 cycles to the 1PPS signal.
                                                         Note that a phase shift is only applied when this value changes, thus
                                                         it might be necessary to write this field to zero before applying a
                                                         new phase shift. */
	uint64_t divider_tc                   : 32; /**< Divider counter terminal count value.
                                                         When BTS_EXT_REF0_DIV_CFG0[CLKOUT_5050_DUTY]=0, this field should be
                                                         set to the clock frequency minus one.
                                                         When BTS_EXT_REF0_DIV_CFG0[CLKOUT_5050_DUTY]=1, this field should be
                                                         set to half the clock frequency minus one. */
#else
	uint64_t divider_tc                   : 32;
	uint64_t divider_start_pos            : 32;
#endif
	} s;
	struct cvmx_bts_ext_ref0_div_cfg1_s   cnf75xx;
};
typedef union cvmx_bts_ext_ref0_div_cfg1 cvmx_bts_ext_ref0_div_cfg1_t;

/**
 * cvmx_bts_ext_ref1_div_cfg0
 *
 * This register configures the clock divider used to generate a 1PPS signal
 * from external reference clock 1 (i.e., EXTREF1_CLK).
 */
union cvmx_bts_ext_ref1_div_cfg0 {
	uint64_t u64;
	struct cvmx_bts_ext_ref1_div_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t clkout_bypass                : 1;  /**< Set to 1 to bypass the clock divider (clock out = EXTREF1_CLK). */
	uint64_t clkout_inv                   : 1;  /**< Set to 1 to inverse the clock output polarity. */
	uint64_t clkout_5050_duty             : 1;  /**< Clock divider duty cycle:
                                                         0 = one cycle pulse (i.e., one cycle of external reference clock).
                                                         1 = 50/50 duty-cycle. */
	uint64_t divider_en                   : 1;  /**< Set to 1 to enable the clock divider. The divider counter is reset to 0 when
                                                         [DIVIDER_EN] is set to 0 and the clock output is disabled. */
#else
	uint64_t divider_en                   : 1;
	uint64_t clkout_5050_duty             : 1;
	uint64_t clkout_inv                   : 1;
	uint64_t clkout_bypass                : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_bts_ext_ref1_div_cfg0_s   cnf75xx;
};
typedef union cvmx_bts_ext_ref1_div_cfg0 cvmx_bts_ext_ref1_div_cfg0_t;

/**
 * cvmx_bts_ext_ref1_div_cfg1
 *
 * This register configures the clock divider used to generate a 1PPS signal
 * from external reference clock 1 (i.e., EXTREF1_CLK).
 */
union cvmx_bts_ext_ref1_div_cfg1 {
	uint64_t u64;
	struct cvmx_bts_ext_ref1_div_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t divider_start_pos            : 32; /**< Divider start position. When a new value is written to
                                                         [DIVIDER_START_POS], the counter will reset to 0 the next time the
                                                         counter value is equal to [DIVIDER_START_POS]. Subsequently, the counter
                                                         will reset to 0 when the counter is equal to [DIVIDER_TC].
                                                         Writing [DIVIDER_START_POS] effectively applies a positive phase shift
                                                         of [DIVIDER_START_POS]+1 cycles to the 1PPS signal.
                                                         Note that a phase shift is only applied when this value changes, thus
                                                         it might be necessary to write this field to zero before applying a
                                                         new phase shift. */
	uint64_t divider_tc                   : 32; /**< Divider counter terminal count value.
                                                         When BTS_EXT_REF1_DIV_CFG0[CLKOUT_5050_DUTY]=0, this field should be
                                                         set to the clock frequency minus one.
                                                         When BTS_EXT_REF1_DIV_CFG0[CLKOUT_5050_DUTY]=1, this field should be
                                                         set to half the clock minus one. */
#else
	uint64_t divider_tc                   : 32;
	uint64_t divider_start_pos            : 32;
#endif
	} s;
	struct cvmx_bts_ext_ref1_div_cfg1_s   cnf75xx;
};
typedef union cvmx_bts_ext_ref1_div_cfg1 cvmx_bts_ext_ref1_div_cfg1_t;

/**
 * cvmx_bts_global_ctl
 */
union cvmx_bts_global_ctl {
	uint64_t u64;
	struct cvmx_bts_global_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t mon_6144_clk_en              : 1;  /**< Set to 1 to enable 61.44 MHz clock divider and its output (491MHz/8).
                                                         The 61.44 MHz can be output to BPHY_TP2 by setting
                                                         BTS_TP_MUX_SEL[TP2_SEL] = 0x6. */
	uint64_t ptp_1pps_out_en              : 1;  /**< Set to 1 to enable PTP 1 pps output to the PTP_1PPS_OUT external pin. */
	uint64_t gnss_1pps_out_en             : 1;  /**< Set to 1 to enable GNSS 1 pps output to the GNSS_1PPS_OUT external pin. */
	uint64_t dac_clk_en                   : 1;  /**< Set to 1 to enable DAC_CLK external pin output of the (30.72 MHz/14) clock
                                                         divider. */
	uint64_t grfe_en                      : 1;  /**< Set to 1 to enable GRFE. The GNSS_CLK must be stable before setting
                                                         this bit. */
	uint64_t gnss_en                      : 1;  /**< External GNSS receiver enable. This bit controls the value output on the
                                                         GNSS_EN signal. */
#else
	uint64_t gnss_en                      : 1;
	uint64_t grfe_en                      : 1;
	uint64_t dac_clk_en                   : 1;
	uint64_t gnss_1pps_out_en             : 1;
	uint64_t ptp_1pps_out_en              : 1;
	uint64_t mon_6144_clk_en              : 1;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_bts_global_ctl_s          cnf75xx;
};
typedef union cvmx_bts_global_ctl cvmx_bts_global_ctl_t;

/**
 * cvmx_bts_global_status
 */
union cvmx_bts_global_status {
	uint64_t u64;
	struct cvmx_bts_global_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t bts_pll_lock                 : 1;  /**< BTS sets this to one when the BTS PLL is locked. */
#else
	uint64_t bts_pll_lock                 : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bts_global_status_s       cnf75xx;
};
typedef union cvmx_bts_global_status cvmx_bts_global_status_t;

/**
 * cvmx_bts_gps_1pps_cfg
 */
union cvmx_bts_gps_1pps_cfg {
	uint64_t u64;
	struct cvmx_bts_gps_1pps_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t reverse                      : 1;  /**< Set to 1 to invert the polarity of the GNSS_1PPS signal. */
#else
	uint64_t reverse                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bts_gps_1pps_cfg_s        cnf75xx;
};
typedef union cvmx_bts_gps_1pps_cfg cvmx_bts_gps_1pps_cfg_t;

/**
 * cvmx_bts_grfe_1pps_cfg
 *
 * This register configures the BTS GNSS_1PPS timer.
 *
 */
union cvmx_bts_grfe_1pps_cfg {
	uint64_t u64;
	struct cvmx_bts_grfe_1pps_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t max_cnt                      : 32; /**< The number of GNSS_CLK cycles per second. Typically this should be set
                                                         to 16368000 for a 16.368 MHz GNSS clock. */
	uint64_t reserved_16_31               : 16;
	uint64_t on_length                    : 16; /**< Length of the 1PPS pulse in units of GNSS_CLK cycles. */
#else
	uint64_t on_length                    : 16;
	uint64_t reserved_16_31               : 16;
	uint64_t max_cnt                      : 32;
#endif
	} s;
	struct cvmx_bts_grfe_1pps_cfg_s       cnf75xx;
};
typedef union cvmx_bts_grfe_1pps_cfg cvmx_bts_grfe_1pps_cfg_t;

/**
 * cvmx_bts_grfe_1pps_en
 *
 * This register enables the GNSS_1PPS signal and specifies the frame and
 * sample number of the initial 1PPS pulse.
 */
union cvmx_bts_grfe_1pps_en {
	uint64_t u64;
	struct cvmx_bts_grfe_1pps_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t en                           : 1;  /**< Set to 1 to enable 1PPS. The first 1PPS pulse will be generated 2
                                                         GNSS_CLK cycles after receiving sample [START_SMPL_NUM] in frame
                                                         [START_FRM_NUM]. This bit must be cleared before writing the other
                                                         fields in this register. */
	uint64_t reserved_40_62               : 23;
	uint64_t start_frm_num                : 24; /**< Start frame number for 1PPS generation. The first pulse will be
                                                         generated 2 GNSS clock cycles after [START_FRM_NUM],[START_SMPL_NUM], and
                                                         in some cases this might require setting [START_FRM_NUM] to one less
                                                         than the actual frame number where the first 1PPS pulse will occur. */
	uint64_t start_smpl_num               : 16; /**< Start sample number for 1PPS generation. Note that the first 1PPS
                                                         pulse will actually be generated two GNSS clock cycles later (i.e.,
                                                         at [START_SMPL_NUM]+2). */
#else
	uint64_t start_smpl_num               : 16;
	uint64_t start_frm_num                : 24;
	uint64_t reserved_40_62               : 23;
	uint64_t en                           : 1;
#endif
	} s;
	struct cvmx_bts_grfe_1pps_en_s        cnf75xx;
};
typedef union cvmx_bts_grfe_1pps_en cvmx_bts_grfe_1pps_en_t;

/**
 * cvmx_bts_grfe_cfg0
 *
 * This register configures the RXIQ sampling and weight accumulator for the
 * BTS GRFE.
 */
union cvmx_bts_grfe_cfg0 {
	uint64_t u64;
	struct cvmx_bts_grfe_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t on                           : 1;  /**< Set to 1 to enable the GRFE. The BTS_GRFE_CFG0-2 registers must be
                                                         programmed before setting this bit. Similarly, this bit must be
                                                         cleared before changing any values in the BTS_GRFE_CFG0-2 registers. */
	uint64_t reserved_16_62               : 47;
	uint64_t rs                           : 4;  /**< Right shift amount for the weighted accumulator. */
	uint64_t reserved_10_11               : 2;
	uint64_t ds_factor                    : 2;  /**< Down-sampling factor.
                                                         0x0 = 1 sample down-sampling (i.e., no down-sampling).
                                                         0x1 = 2 sample down-sampling.
                                                         0x2 = 4 sample down-sampling.
                                                         0x3 = 8 sample down-sampling. */
	uint64_t reserved_7_7                 : 1;
	uint64_t iq_mode                      : 3;  /**< Format for incoming I/Q data from GNSS RF front-end:
                                                         0x0 = 1 bit signed 2's complement, complex value.
                                                         0x1 = 2 bit signed 2's complement, complex value.
                                                         0x2 = 3 bit signed 2's complement, real value only.
                                                         0x3 = 1 bit sign/magnitude, complex value.
                                                         0x4 = 2 bit sign/magnitude, complex value.
                                                         0x5 = 3 bit sign/magnitude, real value only.
                                                         0x6 = Reserved.
                                                         0x7 = Reserved. */
	uint64_t reserved_1_3                 : 3;
	uint64_t dlatch_edge                  : 1;  /**< Select whether I/Q values are sampled on rising or falling edge of
                                                         GNSS_CLK:
                                                         0 = Rising edge.
                                                         1 = Falling edge. */
#else
	uint64_t dlatch_edge                  : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t iq_mode                      : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t ds_factor                    : 2;
	uint64_t reserved_10_11               : 2;
	uint64_t rs                           : 4;
	uint64_t reserved_16_62               : 47;
	uint64_t on                           : 1;
#endif
	} s;
	struct cvmx_bts_grfe_cfg0_s           cnf75xx;
};
typedef union cvmx_bts_grfe_cfg0 cvmx_bts_grfe_cfg0_t;

/**
 * cvmx_bts_grfe_cfg1
 *
 * This register sets the weights used in the BTS GRFE weighted accumulator.
 *
 */
union cvmx_bts_grfe_cfg1 {
	uint64_t u64;
	struct cvmx_bts_grfe_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t wht                          : 64; /**< Weight table for the weighted accumulator. The table contains eight
                                                         signed, 8-bit weights. w(0) is stored in bits [7..0], w(1) is stored
                                                         in bits [15..8], etc. */
#else
	uint64_t wht                          : 64;
#endif
	} s;
	struct cvmx_bts_grfe_cfg1_s           cnf75xx;
};
typedef union cvmx_bts_grfe_cfg1 cvmx_bts_grfe_cfg1_t;

/**
 * cvmx_bts_grfe_cfg2
 *
 * This register configures the output format for the BTS GRFE sample framer,
 * and the size and location of the circular buffer used to write the GNSS
 * samples.
 *
 * Frames are written to the circular buffer at increasing addresses starting
 * at [BASE_ADDR]. The number of frames that can be written in the buffer
 * before wrapping around to [BASE_ADDR] is specified via [SIZE_IN_FRM].
 *
 * Note that the size of each frame depends on [IQ_PACK_MODE] and
 * BTS_GRFE_CFG0[DS_FACTOR], as follows:
 *
 * _ For 8-bit samples, each frame is 32768/(2^DS_FACTOR) bytes.
 *
 * _ For 16-bit samples, each frame is 65536/(2^DS_FACTOR) bytes.
 */
union cvmx_bts_grfe_cfg2 {
	uint64_t u64;
	struct cvmx_bts_grfe_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t iq_pack_mode                 : 2;  /**< I/Q packing mode of GPS frame in memory:
                                                         0x0 = 8 bit, little-endian.
                                                         0x1 = 16 bit, little-endian.
                                                         0x2 = 8 bit, big-endian.
                                                         0x3 = 16 bit, big-endian. */
	uint64_t reserved_58_59               : 2;
	uint64_t size_in_frm                  : 10; /**< The size of the frame buffer in number of frames. */
	uint64_t int_threshold                : 8;  /**< Interrupt coalescing threshold. The GRFE_FRMR_RDY interrupt is
                                                         triggered each time the GRFE framer has written INT_THRSESHOLD frames.
                                                         Setting to 0x0 disables the interrupt. */
	uint64_t base_addr                    : 33; /**< Bits[39:7] of the base address of frame buffer. The buffer must be
                                                         128-bit aligned (i.e, bits [6:0] are always 0). */
	uint64_t reserved_0_6                 : 7;
#else
	uint64_t reserved_0_6                 : 7;
	uint64_t base_addr                    : 33;
	uint64_t int_threshold                : 8;
	uint64_t size_in_frm                  : 10;
	uint64_t reserved_58_59               : 2;
	uint64_t iq_pack_mode                 : 2;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_bts_grfe_cfg2_s           cnf75xx;
};
typedef union cvmx_bts_grfe_cfg2 cvmx_bts_grfe_cfg2_t;

/**
 * cvmx_bts_grfe_cntr
 */
union cvmx_bts_grfe_cntr {
	uint64_t u64;
	struct cvmx_bts_grfe_cntr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t en                           : 1;  /**< Set to 1 to enable the frame and sample counters. */
	uint64_t reserved_48_62               : 15;
	uint64_t smpl_cntr_max                : 16; /**< The sample counter runs from 0 to [SMPL_CNTR_MAX] every 1 ms. Typically this value
                                                         should be set to the number of cycles per millisecond minus 1. ([SMPL_CNTR_MAX] + 1) must
                                                         be
                                                         multiple of 8 to support GRFE down-sampling. Default value is 0x3FEF
                                                         (16367) for 16.386 MHz clock. */
	uint64_t reserved_24_31               : 8;
	uint64_t frm_num                      : 24; /**< Current value of the frame counter. */
#else
	uint64_t frm_num                      : 24;
	uint64_t reserved_24_31               : 8;
	uint64_t smpl_cntr_max                : 16;
	uint64_t reserved_48_62               : 15;
	uint64_t en                           : 1;
#endif
	} s;
	struct cvmx_bts_grfe_cntr_s           cnf75xx;
};
typedef union cvmx_bts_grfe_cntr cvmx_bts_grfe_cntr_t;

/**
 * cvmx_bts_grfe_sta
 */
union cvmx_bts_grfe_sta {
	uint64_t u64;
	struct cvmx_bts_grfe_sta_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t frames_stored                : 24; /**< The number of frames which have been committed to memory. This number
                                                         resets to zero when it rolls over and when BTS_GRFE_CFG0[ON] is
                                                         cleared.
                                                         If BTS_GRFE_CFG2[SIZE_IN_FRM] is a power of two, then the address of
                                                         the last valid frame can be calculated as:
                                                         BASE_ADDR + FRM_SIZE_IN_BYTES*(([FRAMES_STORED]-1)%SIZE_IN_FRM)
                                                         If BTS_GRFE_CFG2[SIZE_IN_FRM] is NOT a power of two, then software
                                                         must count how many times [FRAMES_STORED] has rolled-over to zero. Then
                                                         software can calculate the address of the last valid frame as:
                                                         BASE_ADDR+FRM_SIZE_IN_BYTES*(([FRAMES_STORED]-1+ROLL_OVERS*((2^24)%SIZE_IN_FRM)%SIZE_IN_FR
                                                         M)
                                                         Note that under continuous operation, [FRAMES_STORED] will roll-over
                                                         approximately every 4.66 hours. */
	uint64_t target_addr                  : 33; /**< Bits [39:7] of the address where the next GNSS frame will be written.
                                                         This value may be updated before the data from previous frames is
                                                         available in memory, so software should rely on the value in
                                                         [FRAMES_STORED] to ensure that it does not read memory before the writes
                                                         complete. This address resets to BTS_GRFE_CFG2[BASE_ADDR] when
                                                         BTS_GRFE_CFG0[ON] is toggled. */
	uint64_t reserved_0_6                 : 7;
#else
	uint64_t reserved_0_6                 : 7;
	uint64_t target_addr                  : 33;
	uint64_t frames_stored                : 24;
#endif
	} s;
	struct cvmx_bts_grfe_sta_s            cnf75xx;
};
typedef union cvmx_bts_grfe_sta cvmx_bts_grfe_sta_t;

/**
 * cvmx_bts_int_sum
 *
 * This register contains the status for various BTS interrupts.
 *
 */
union cvmx_bts_int_sum {
	uint64_t u64;
	struct cvmx_bts_int_sum_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t grfe_frmr_oflow              : 1;  /**< Sample FIFO overflow inside GRFE framer. */
	uint64_t grfe_frmr_rdy                : 1;  /**< GRFE frame buffer ready for software to process. */
	uint64_t pdbank5_done                 : 1;  /**< Phase difference measurement done on PD bank 5 */
	uint64_t pdbank4_done                 : 1;  /**< Phase difference measurement done on PD bank 4 */
	uint64_t pdbank3_done                 : 1;  /**< Phase difference measurement done on PD bank 3 */
	uint64_t pdbank2_done                 : 1;  /**< Phase difference measurement done on PD bank 2 */
	uint64_t pdbank1_done                 : 1;  /**< Phase difference measurement done on PD bank 1 */
	uint64_t pdbank0_done                 : 1;  /**< Phase difference measurement done on PD bank 0 */
#else
	uint64_t pdbank0_done                 : 1;
	uint64_t pdbank1_done                 : 1;
	uint64_t pdbank2_done                 : 1;
	uint64_t pdbank3_done                 : 1;
	uint64_t pdbank4_done                 : 1;
	uint64_t pdbank5_done                 : 1;
	uint64_t grfe_frmr_rdy                : 1;
	uint64_t grfe_frmr_oflow              : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_bts_int_sum_s             cnf75xx;
};
typedef union cvmx_bts_int_sum cvmx_bts_int_sum_t;

/**
 * cvmx_bts_ncb_cfg
 */
union cvmx_bts_ncb_cfg {
	uint64_t u64;
	struct cvmx_bts_ncb_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t cmd_type                     : 2;  /**< Select the command type used when GRFE writes a frame of samples to
                                                         memory. The command types are enumerated with BTS_NCB_WR_CMD_E. */
#else
	uint64_t cmd_type                     : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_bts_ncb_cfg_s             cnf75xx;
};
typedef union cvmx_bts_ncb_cfg cvmx_bts_ncb_cfg_t;

/**
 * cvmx_bts_pd1pps_div_cfg0
 *
 * This register configures the clock divider used to generate the PD_1PPS
 * signal derived from the 30.72 MHz clock (BPHY_BFN_CLK). This signal is
 * used by the clock generation block to control RFP timing.
 *
 * This register configures the clock divder used to generate the
 * PD_3072M_1PPS signal derived from the 30.72 MHz clock (BPHY_BFN_CLK).
 *
 * When [FREE_RUN]=1, the initial alignment depends on when [DIVIDER_EN] is written
 * to 1, and the value in BTS_PD1PPS_DIV_CFG1[DIVIDER_START_POS].
 *
 * When [FREE_RUN]=0, the initial alignment will be
 * BTS_PD1PPS_DIV_CFG1[DIVIDER_START_POS]+4 cycles after the reference clock
 * selected by [REF_IN_SEL] (where cycles are counted in the 30.72 MHz clock).
 *
 * In both cases, after the initial pulse, subsequent 1PPS pulses are
 * generated at a constant period.
 */
union cvmx_bts_pd1pps_div_cfg0 {
	uint64_t u64;
	struct cvmx_bts_pd1pps_div_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t ref_in_sel                   : 3;  /**< Select the reference 1PPS source:
                                                         0x0 = GNSS_1PPS.
                                                         0x1 = PTP_1PPS.
                                                         0x2 = SYNCE25M_1PPS.
                                                         0x3 = EXT_REF0_1PPS.
                                                         0x4 = EXT_REF1_1PPS.
                                                         0x5 = PD_3072_1PPS.
                                                         0x6-0x7 = Reserved. */
	uint64_t free_run                     : 1;  /**< Set to 1 to enable free running mode. In free running mode, all input
                                                         1PPS references are ignored. The initial start position is set by
                                                         BTS_PD1PPS_DIV_CFG1[DIVIDER_START_POS]. */
	uint64_t reserved_2_2                 : 1;
	uint64_t clkout_5050_duty             : 1;  /**< Clock divider duty cycle:
                                                         0 = one cycle pulse (i.e., one 30.72 MHz cycle).
                                                         1 = 50/50 duty-cycle. */
	uint64_t divider_en                   : 1;  /**< Set to 1 to enable the clock divider. The divider counter is reset to 0 when
                                                         [DIVIDER_EN] is set to 0. */
#else
	uint64_t divider_en                   : 1;
	uint64_t clkout_5050_duty             : 1;
	uint64_t reserved_2_2                 : 1;
	uint64_t free_run                     : 1;
	uint64_t ref_in_sel                   : 3;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_bts_pd1pps_div_cfg0_s     cnf75xx;
};
typedef union cvmx_bts_pd1pps_div_cfg0 cvmx_bts_pd1pps_div_cfg0_t;

/**
 * cvmx_bts_pd1pps_div_cfg1
 *
 * This register configures the clock divider used to generate the PD_1PPS
 * signal derived from the 30.72 MHz clock (BPHY_BFN_CLK). This signal is
 * used by the clock generation block to control RFP timing.
 *
 * When enabled, the divider counts from 0 to [DIVIDER_TC], and then resets
 * to 0. When it resets to zero, it either generates a single cycle pulse, or
 * flips the polarity of the output, depending on the setting of
 * BTS_PD1PPS_DIV_CFG0[CLKOUT_5050_DUTY].
 */
union cvmx_bts_pd1pps_div_cfg1 {
	uint64_t u64;
	struct cvmx_bts_pd1pps_div_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t divider_start_pos            : 32; /**< Divider start position. When a new value is written to
                                                         [DIVIDER_START_POS], the counter will reset to 0 the next time the
                                                         counter value is equal to [DIVIDER_START_POS]. Subsequently, the counter
                                                         will reset to 0 when the counter is equal to [DIVIDER_TC].
                                                         Writing [DIVIDER_START_POS] effectively applies a positive phase shift
                                                         of [DIVIDER_START_POS]+1 cycles to the 1PPS signal.
                                                         Note that a phase shift is only applied when this value changes, thus
                                                         it might be necessary to write this field to zero before applying a
                                                         new phase shift. */
	uint64_t divider_tc                   : 32; /**< Divider counter terminal count value.
                                                         When BTS_PD1PPS_DIV_CFG0[CLKOUT_5050_DUTY]=0, this field should be
                                                         set to the clock frequency minus one, i.e., 0x1D4BFFF (307199999 for the 30.72
                                                         MHz clock.
                                                         When BTS_PD1PPS_DIV_CFG0[CLKOUT_5050_DUTY]=1, this field should be
                                                         set to half the clock frequency minus one, i.e., 0xEA5FFF (153599999 for the
                                                         30.72 MHz clock. */
#else
	uint64_t divider_tc                   : 32;
	uint64_t divider_start_pos            : 32;
#endif
	} s;
	struct cvmx_bts_pd1pps_div_cfg1_s     cnf75xx;
};
typedef union cvmx_bts_pd1pps_div_cfg1 cvmx_bts_pd1pps_div_cfg1_t;

/**
 * cvmx_bts_pd3072m_div_cfg0
 *
 * This register configures the clock divider used to generate the
 * PD_3072M_1PPS signal derived from the 30.72 MHz clock (BPHY_BFN_CLK).
 *
 * When [FREE_RUN]=1, the initial alignment depends on when [DIVIDER_EN] is written
 * to 1, and the value in BTS_PD3072M_DIV_CFG1[DIVIDER_START_POS].
 *
 * When [FREE_RUN]=0, the initial alignment will be
 * BTS_PD3072M_DIV_CFG1[DIVIDER_START_POS]+4 cycles after the reference clock
 * selected by [REF_IN_SEL] (where cycles are counted in the 30.72 MHz clock).
 *
 * In both cases, after the initial pulse, subsequent 1PPS pulses are
 * generated at a constant period.
 */
union cvmx_bts_pd3072m_div_cfg0 {
	uint64_t u64;
	struct cvmx_bts_pd3072m_div_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t ref_in_sel                   : 3;  /**< Select the reference 1PPS source:
                                                         0x0 = GNSS_1PPS.
                                                         0x1 = PTP_1PPS.
                                                         0x2 = SYNCE25M_1PPS.
                                                         0x3 = EXT_REF0_1PPS.
                                                         0x4 = EXT_REF1_1PPS.
                                                         0x5 = CG_1PPS.
                                                         0x6-0x7 = Reserved. */
	uint64_t free_run                     : 1;  /**< Set to 1 to enable free running mode. In free running mode, all input
                                                         1PPS references are ignored. The initial start position is set by
                                                         BTS_PD3072M_DIV_CFG1[DIVIDER_START_POS] and the timing of the write to
                                                         set [DIVIDER_EN]=1. */
	uint64_t clkout_inv                   : 1;  /**< Set to 1 to inverse the output polarity for the PD3072_1PPS
                                                         signal. */
	uint64_t clkout_5050_duty             : 1;  /**< Clock divider duty cycle:
                                                         0 = one cycle pulse (i.e., one 30.72 MHz cycle).
                                                         1 = 50/50 duty-cycle. */
	uint64_t divider_en                   : 1;  /**< Set to 1 to enable the clock divider. The divider counter is reset to 0 when
                                                         [DIVIDER_EN] is set to 0 and the clock output is disabled. */
#else
	uint64_t divider_en                   : 1;
	uint64_t clkout_5050_duty             : 1;
	uint64_t clkout_inv                   : 1;
	uint64_t free_run                     : 1;
	uint64_t ref_in_sel                   : 3;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_bts_pd3072m_div_cfg0_s    cnf75xx;
};
typedef union cvmx_bts_pd3072m_div_cfg0 cvmx_bts_pd3072m_div_cfg0_t;

/**
 * cvmx_bts_pd3072m_div_cfg1
 *
 * This register configures the clock divder used to generate the PD3072_1PPS
 * signal derived from the 30.72 MHz clock (BPHY_BFN_CLK).
 *
 * When enabled, the divider counts from 0 to [DIVIDER_TC], and then resets
 * to 0. When it resets to zero, it either generates a single cycle pulse, or
 * flips the polarity of the output, depending on the setting of
 * BTS_PD3072M_DIV_CFG0[CLKOUT_5050_DUTY].
 */
union cvmx_bts_pd3072m_div_cfg1 {
	uint64_t u64;
	struct cvmx_bts_pd3072m_div_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t divider_start_pos            : 32; /**< Divider start position. When a new value is written to
                                                         [DIVIDER_START_POS], the counter will reset to 0 the next time the
                                                         counter value is equal to [DIVIDER_START_POS]. Subsequently, the counter
                                                         will reset to 0 when the counter is equal to [DIVIDER_TC].
                                                         Writing [DIVIDER_START_POS] effectively applies a positive phase shift
                                                         of [DIVIDER_START_POS]+1 cycles to the 1PPS signal.
                                                         Note that a phase shift is only applied when this value changes, thus
                                                         it might be necessary to write this field to zero before applying a
                                                         new phase shift. */
	uint64_t divider_tc                   : 32; /**< Divider counter terminal count value.
                                                         When BTS_PD3072M_DIV_CFG0[CLKOUT_5050_DUTY]=0, this field should be
                                                         set to the clock frequency minus one, i.e., 0x1D4BFFF (30719999) for the 30.72
                                                         MHz clock.
                                                         When BTS_PD3072M_DIV_CFG0[CLKOUT_5050_DUTY]=1, this field should be
                                                         set to half the clock frequency minus one, i.e., 0xEA5FFF (15359999) for the
                                                         30.72 MHz clock. */
#else
	uint64_t divider_tc                   : 32;
	uint64_t divider_start_pos            : 32;
#endif
	} s;
	struct cvmx_bts_pd3072m_div_cfg1_s    cnf75xx;
};
typedef union cvmx_bts_pd3072m_div_cfg1 cvmx_bts_pd3072m_div_cfg1_t;

/**
 * cvmx_bts_pd_history
 *
 * This is a scratch register to facilitate saving the values used for PD
 * bank control registers.
 */
union cvmx_bts_pd_history {
	uint64_t u64;
	struct cvmx_bts_pd_history_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t value                        : 64; /**< Scratch data. */
#else
	uint64_t value                        : 64;
#endif
	} s;
	struct cvmx_bts_pd_history_s          cnf75xx;
};
typedef union cvmx_bts_pd_history cvmx_bts_pd_history_t;

/**
 * cvmx_bts_pd_slice#_ctl
 *
 * These registers control the operation of each PD bank.
 *
 * The PD bank must be disabled when changing any of these configuration
 * parameters (i.e., [ENABLE]=0). The PD bank should be enabled with a
 * separate register write after updating the other parameters (i.e., perform
 * one write to change the settings, and use a second write to enable the PD
 * bank).
 */
union cvmx_bts_pd_slicex_ctl {
	uint64_t u64;
	struct cvmx_bts_pd_slicex_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t mea_clk_sel                  : 2;  /**< Measurement clock select:
                                                         0x0 = 491 MHz clock, both edges.
                                                         0x1 = 491 MHz clock, rising edge only.
                                                         0x2, 0x3 = SCLK, rising edge only.
                                                         Refer to BTS_PLL_CTL for how to configure the 491 MHz clock. */
	uint64_t ref_in_sel                   : 3;  /**< Reference 1pps source select.
                                                         0x0 = GNSS_1PPS.
                                                         0x1 = PTP_1PPS.
                                                         0x2 = SyncE 25 MHz 1PPS.
                                                         0x3 = External reference 0 1PPS.
                                                         0x4 = External reference 1 1PPS.
                                                         0x5 = PD_3072_1PPS.
                                                         0x6 = CG_1PPS.
                                                         0x7 = Reserved. */
	uint64_t loop_in_sel                  : 3;  /**< Loop 1pps source select.
                                                         0x0 = GNSS_1PPS.
                                                         0x1 = PTP_1PPS.
                                                         0x2 = SyncE 25 MHz 1PPS.
                                                         0x3 = External reference 0 1PPS.
                                                         0x4 = External reference 1 1PPS.
                                                         0x5 = PD_3072_1PPS.
                                                         0x6 = CG_1PPS.
                                                         0x7 = Reserved. */
	uint64_t enable                       : 1;  /**< Set to 1 to enable the PD bank. */
#else
	uint64_t enable                       : 1;
	uint64_t loop_in_sel                  : 3;
	uint64_t ref_in_sel                   : 3;
	uint64_t mea_clk_sel                  : 2;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_bts_pd_slicex_ctl_s       cnf75xx;
};
typedef union cvmx_bts_pd_slicex_ctl cvmx_bts_pd_slicex_ctl_t;

/**
 * cvmx_bts_pd_slice#_status
 *
 * This register reports the calculated phase difference for each PD bank.
 *
 * BTS triggers a BTS_PDBANK(0..5)_DONE interrupt each time the PD bank
 * completes a measurement and updates this status register.
 */
union cvmx_bts_pd_slicex_status {
	uint64_t u64;
	struct cvmx_bts_pd_slicex_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t pd_result                    : 33; /**< The phase difference as a signed, two's complement value. A positive value indicates that
                                                         the REF clock is ahead of LOOP, and a negative value indicates that LOOP is ahead of REF.
                                                         The phase difference is measured based on the clock selected by
                                                         BTS_PD_SLICE(0..5)_CTL[MEA_CLK_SEL], and can represent either SCLK cycles, 491 MHz cycles,
                                                         or 982 MHz cycles (1.0183 ns). */
#else
	uint64_t pd_result                    : 33;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_bts_pd_slicex_status_s    cnf75xx;
};
typedef union cvmx_bts_pd_slicex_status cvmx_bts_pd_slicex_status_t;

/**
 * cvmx_bts_pll_ctl
 *
 * This register controls the sample clock frequency into PD Bank slices.
 * The following sequence is the PLL-bringup sequence:
 *   1. Write [RESET_N]=0 and [DIV_RESET]=1.
 *   2. Set [CLKF] and [PS_EN].
 *   3. Wait 128 reference-clock cycles.
 *   4. Write [RESET_N]=1.
 *   5. Wait 1152 reference-clock cycles.
 *   6. Write [DIV_RESET]=0.
 *   7. Wait 10 reference-clock cycles before using the PLL clock out.
 */
union cvmx_bts_pll_ctl {
	uint64_t u64;
	struct cvmx_bts_pll_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t alt_ref_clk_sel              : 2;  /**< Alternate reference clock select:
                                                         0x0 = DLM6 ref clock.
                                                         0x1 = DLM7 ref clock.
                                                         0x2 or 0x3 = DLM8 ref clock. */
	uint64_t pll_bypass                   : 1;  /**< Set to 1 to bypass PLL. In PLL bypass mode, the PLL clock out is BPHY_BFN_CLK (30.72 MHz). */
	uint64_t ref_clk_sel                  : 1;  /**< Reference clock select:
                                                         0 = 122.88MHz (alternate reference clock selected by ALT_REF_CLK_SEL).
                                                         1 = 30.72MHz (BPHY_BFN_CLK). */
	uint64_t div_reset                    : 1;  /**< Postscalar divider reset. */
	uint64_t ps_en                        : 4;  /**< PLL postscalar divide ratio. Determines the network clock speed.
                                                         0x0 = divide BTS PLL by 1.
                                                         0x1 = divide BTS PLL by 2.
                                                         0x2 = divide BTS PLL by 3.
                                                         0x3 = divide BTS PLL by 4.
                                                         0x4 = divide BTS PLL by 5.
                                                         0x5 = divide BTS PLL by 6.
                                                         0x6 = divide BTS PLL by 7.
                                                         0x7 = divide BTS PLL by 8.
                                                         0x8 = divide BTS PLL by 10.
                                                         0x9 = divide BTS PLL by 12.
                                                         0xA-0xF = Reserved.
                                                         PS_EN is not used when DIV_RESET = 1. See CLKF for supported settings. */
	uint64_t reserved_8_17                : 10;
	uint64_t reset_n                      : 1;  /**< PLL reset. */
	uint64_t clkf                         : 7;  /**< PLL multiplier. PLL out frequency = PLL in clk(MHz) * ([CLKF]+1) / ([PS_EN]+1)
                                                         To generate a 491MHz clock from  a 30.72 MHz reference (i.e., when
                                                         REF_CLK_SEL=1), set CLKF to 0x7f and PS_EN to 0x7. This results in an
                                                         overall 16x multplier.
                                                         To generate 491MHz clock from a 122.88 MHz reference (i.e., when
                                                         REF_CLK_SEL=0), set CLKF to 0x1f and PS_EN to 0x7. This results in an
                                                         overall 4x multiplier.
                                                         Note that other settings are not supported and may result in undefined
                                                         behavior. */
#else
	uint64_t clkf                         : 7;
	uint64_t reset_n                      : 1;
	uint64_t reserved_8_17                : 10;
	uint64_t ps_en                        : 4;
	uint64_t div_reset                    : 1;
	uint64_t ref_clk_sel                  : 1;
	uint64_t pll_bypass                   : 1;
	uint64_t alt_ref_clk_sel              : 2;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_bts_pll_ctl_s             cnf75xx;
};
typedef union cvmx_bts_pll_ctl cvmx_bts_pll_ctl_t;

/**
 * cvmx_bts_ptp_1pps_cfg
 */
union cvmx_bts_ptp_1pps_cfg {
	uint64_t u64;
	struct cvmx_bts_ptp_1pps_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t reverse                      : 1;  /**< Set to 1 to invert the polarity of the PTP_1PPS signal. */
#else
	uint64_t reverse                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bts_ptp_1pps_cfg_s        cnf75xx;
};
typedef union cvmx_bts_ptp_1pps_cfg cvmx_bts_ptp_1pps_cfg_t;

/**
 * cvmx_bts_pwm#_ctl
 */
union cvmx_bts_pwmx_ctl {
	uint64_t u64;
	struct cvmx_bts_pwmx_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_56_63               : 8;
	uint64_t fall_pos                     : 20; /**< Pulse low duration minus one, measured in ticks, as determined by
                                                         [TICK_SEL]. */
	uint64_t reserved_28_35               : 8;
	uint64_t rise_pos                     : 20; /**< Pulse high duration minus one, measured in ticks, as determined by
                                                         [TICK_SEL]. */
	uint64_t reserved_3_7                 : 5;
	uint64_t tick_sel                     : 2;  /**< PWM tick clock select:
                                                         0x0 = SCLK / 4.
                                                         0x1 = SCLK / 8.
                                                         0x2 = SCLK / 16.
                                                         0x3 = SCLK / 32. */
	uint64_t enable                       : 1;  /**< Set to 1 to enable PWM clock and output. */
#else
	uint64_t enable                       : 1;
	uint64_t tick_sel                     : 2;
	uint64_t reserved_3_7                 : 5;
	uint64_t rise_pos                     : 20;
	uint64_t reserved_28_35               : 8;
	uint64_t fall_pos                     : 20;
	uint64_t reserved_56_63               : 8;
#endif
	} s;
	struct cvmx_bts_pwmx_ctl_s            cnf75xx;
};
typedef union cvmx_bts_pwmx_ctl cvmx_bts_pwmx_ctl_t;

/**
 * cvmx_bts_synce_25m_div_cfg0
 *
 * This register configures the clock divider for the 25 MHz SyncE clock.
 *
 */
union cvmx_bts_synce_25m_div_cfg0 {
	uint64_t u64;
	struct cvmx_bts_synce_25m_div_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t clkout_inv                   : 1;  /**< Set to 1 to inverse the clock output polarity. */
	uint64_t clkout_5050_duty             : 1;  /**< Clock divider duty cycle:
                                                         0 = one cycle pulse (i.e., one cycle of 25 MHz clock).
                                                         1 = 50/50 duty-cycle. */
	uint64_t divider_en                   : 1;  /**< Set to 1 to enable the clock divider. The divider counter is reset to 0 when
                                                         [DIVIDER_EN] is set to 0 and the clock output is disabled. */
#else
	uint64_t divider_en                   : 1;
	uint64_t clkout_5050_duty             : 1;
	uint64_t clkout_inv                   : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_bts_synce_25m_div_cfg0_s  cnf75xx;
};
typedef union cvmx_bts_synce_25m_div_cfg0 cvmx_bts_synce_25m_div_cfg0_t;

/**
 * cvmx_bts_synce_25m_div_cfg1
 */
union cvmx_bts_synce_25m_div_cfg1 {
	uint64_t u64;
	struct cvmx_bts_synce_25m_div_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t divider_start_pos            : 32; /**< Divider start position. When a new value is written to
                                                         [DIVIDER_START_POS], the counter will reset to 0 the next time the
                                                         counter value is equal to [DIVIDER_START_POS]. Subsequently, the counter
                                                         will reset to 0 when the counter is equal to [DIVIDER_TC].
                                                         Writing [DIVIDER_START_POS] effectively applies a positive phase shift
                                                         of [DIVIDER_START_POS]+1 cycles to the 1PPS signal.
                                                         Note that a phase shift is only applied when this value changes, thus
                                                         it might be necessary to write this field to zero before applying a
                                                         new phase shift. */
	uint64_t divider_tc                   : 32; /**< Divider counter terminal count value.
                                                         When BTS_SYNCE_25M_DIV_CFG0[CLKOUT_5050_DUTY]=0, this field should be
                                                         set to the clock frequency minus one, i.e., 0x17D783F (24999999) for the 25
                                                         MHz clock.
                                                         When BTS_SYNCE_25M_DIV_CFG0[CLKOUT_5050_DUTY]=1, this field should be
                                                         set to half of the clock frequency minus one, i.e., 0xBEBC1F (12499999) for the 25
                                                         MHz clock. */
#else
	uint64_t divider_tc                   : 32;
	uint64_t divider_start_pos            : 32;
#endif
	} s;
	struct cvmx_bts_synce_25m_div_cfg1_s  cnf75xx;
};
typedef union cvmx_bts_synce_25m_div_cfg1 cvmx_bts_synce_25m_div_cfg1_t;

/**
 * cvmx_bts_tp_mux_sel
 */
union cvmx_bts_tp_mux_sel {
	uint64_t u64;
	struct cvmx_bts_tp_mux_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t tp4_sel                      : 4;  /**< Select the source for the BPHY_TP4 output signal:
                                                         0x0 = PD bank 0 PD positive done.
                                                         0x1 = PD bank 1 PD positive done.
                                                         0x2 = PD bank 2 PD positive done.
                                                         0x3 = PD bank 3 PD positive done.
                                                         0x4 = PD bank 4 PD positive done.
                                                         0x5 = PD bank 5 PD positive done.
                                                         0x6 = PD_1PPS.
                                                         0x7 = Reserved.
                                                         0x8 = PD bank 0 PD negative clear.
                                                         0x9 = PD bank 1 PD negative clear.
                                                         0xA = PD bank 2 PD negative clear.
                                                         0xB = PD bank 3 PD negative clear.
                                                         0xC = PD bank 4 PD negative clear.
                                                         0xD = PD bank 5 PD negative clear.
                                                         0xE-0xF = Reserved. */
	uint64_t tp3_sel                      : 4;  /**< Select the source for the BPHY_TP3 output signal:
                                                         0x0 = PD bank 0 PD positive clear.
                                                         0x1 = PD bank 1 PD positive clear.
                                                         0x2 = PD bank 2 PD positive clear.
                                                         0x3 = PD bank 3 PD positive clear.
                                                         0x4 = PD bank 4 PD positive clear.
                                                         0x5 = PD bank 5 PD positive clear.
                                                         0x6 = CG_1pps.
                                                         0x7 = Reserved.
                                                         0x8 = PD bank 0 PD negative done.
                                                         0x9 = PD bank 1 PD negative done.
                                                         0xA = PD bank 2 PD negative done.
                                                         0xB = PD bank 3 PD negative done.
                                                         0xC = PD bank 4 PD negative done.
                                                         0xD = PD bank 5 PD negative done.
                                                         0xE-0xF = Reserved. */
	uint64_t tp2_sel                      : 4;  /**< Select the source for the BPHY_TP2 output signal:
                                                         0x0 = PD bank 0 LOOP_1pps.
                                                         0x1 = PD bank 1 LOOP_1pps.
                                                         0x2 = PD bank 2 LOOP_1pps.
                                                         0x3 = PD bank 3 LOOP_1pps.
                                                         0x4 = PD bank 4 LOOP_1pps.
                                                         0x5 = PD bank 5 LOOP_1pps.
                                                         0x6 = 491MHz/8.
                                                         0x7-0xF = Reserved. */
	uint64_t tp1_sel                      : 4;  /**< Select the source for the BPHY_TP1 output signal:
                                                         0x0 = PD bank 0 REF_1pps.
                                                         0x1 = PD bank 1 REF_1pps.
                                                         0x2 = PD bank 2 REF_1pps.
                                                         0x3 = PD bank 3 REF_1pps.
                                                         0x4 = PD bank 4 REF_1pps.
                                                         0x5 = PD bank 5 REF_1pps.
                                                         0x6-0xF = Reserved. */
#else
	uint64_t tp1_sel                      : 4;
	uint64_t tp2_sel                      : 4;
	uint64_t tp3_sel                      : 4;
	uint64_t tp4_sel                      : 4;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_bts_tp_mux_sel_s          cnf75xx;
};
typedef union cvmx_bts_tp_mux_sel cvmx_bts_tp_mux_sel_t;

#endif
