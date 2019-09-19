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
 * cvmx-nqm-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon nqm.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_NQM_DEFS_H__
#define __CVMX_NQM_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CFG CVMX_NQM_CFG_FUNC()
static inline uint64_t CVMX_NQM_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006028ull);
}
#else
#define CVMX_NQM_CFG (CVMX_ADD_IO_SEG(0x0001450000006028ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CLKEN CVMX_NQM_CLKEN_FUNC()
static inline uint64_t CVMX_NQM_CLKEN_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CLKEN not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00014500000060F8ull);
}
#else
#define CVMX_NQM_CLKEN (CVMX_ADD_IO_SEG(0x00014500000060F8ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CS_BIST_STATUS0 CVMX_NQM_CS_BIST_STATUS0_FUNC()
static inline uint64_t CVMX_NQM_CS_BIST_STATUS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CS_BIST_STATUS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000004FF0ull);
}
#else
#define CVMX_NQM_CS_BIST_STATUS0 (CVMX_ADD_IO_SEG(0x0001450000004FF0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CS_CMD_DBG0 CVMX_NQM_CS_CMD_DBG0_FUNC()
static inline uint64_t CVMX_NQM_CS_CMD_DBG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CS_CMD_DBG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000FF4078ull);
}
#else
#define CVMX_NQM_CS_CMD_DBG0 (CVMX_ADD_IO_SEG(0x0001450000FF4078ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CS_CPL_DBG0 CVMX_NQM_CS_CPL_DBG0_FUNC()
static inline uint64_t CVMX_NQM_CS_CPL_DBG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CS_CPL_DBG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x000145000FFF4070ull);
}
#else
#define CVMX_NQM_CS_CPL_DBG0 (CVMX_ADD_IO_SEG(0x000145000FFF4070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CS_ECC0_INT CVMX_NQM_CS_ECC0_INT_FUNC()
static inline uint64_t CVMX_NQM_CS_ECC0_INT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CS_ECC0_INT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000004F00ull);
}
#else
#define CVMX_NQM_CS_ECC0_INT (CVMX_ADD_IO_SEG(0x0001450000004F00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_CS_MEM_CTL0 CVMX_NQM_CS_MEM_CTL0_FUNC()
static inline uint64_t CVMX_NQM_CS_MEM_CTL0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_CS_MEM_CTL0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000004F80ull);
}
#else
#define CVMX_NQM_CS_MEM_CTL0 (CVMX_ADD_IO_SEG(0x0001450000004F80ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_FI_FPA_AURA CVMX_NQM_FI_FPA_AURA_FUNC()
static inline uint64_t CVMX_NQM_FI_FPA_AURA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_FI_FPA_AURA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006030ull);
}
#else
#define CVMX_NQM_FI_FPA_AURA (CVMX_ADD_IO_SEG(0x0001450000006030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_FPA_DBG CVMX_NQM_FPA_DBG_FUNC()
static inline uint64_t CVMX_NQM_FPA_DBG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_FPA_DBG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006010ull);
}
#else
#define CVMX_NQM_FPA_DBG (CVMX_ADD_IO_SEG(0x0001450000006010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_GLBL_TAG CVMX_NQM_GLBL_TAG_FUNC()
static inline uint64_t CVMX_NQM_GLBL_TAG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_GLBL_TAG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006020ull);
}
#else
#define CVMX_NQM_GLBL_TAG (CVMX_ADD_IO_SEG(0x0001450000006020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_HS_BIST_STATUS0 CVMX_NQM_HS_BIST_STATUS0_FUNC()
static inline uint64_t CVMX_NQM_HS_BIST_STATUS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_HS_BIST_STATUS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006FF0ull);
}
#else
#define CVMX_NQM_HS_BIST_STATUS0 (CVMX_ADD_IO_SEG(0x0001450000006FF0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_HS_ECC0_INT CVMX_NQM_HS_ECC0_INT_FUNC()
static inline uint64_t CVMX_NQM_HS_ECC0_INT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_HS_ECC0_INT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006FD0ull);
}
#else
#define CVMX_NQM_HS_ECC0_INT (CVMX_ADD_IO_SEG(0x0001450000006FD0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_HS_MEM_CTL0 CVMX_NQM_HS_MEM_CTL0_FUNC()
static inline uint64_t CVMX_NQM_HS_MEM_CTL0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_HS_MEM_CTL0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006FE0ull);
}
#else
#define CVMX_NQM_HS_MEM_CTL0 (CVMX_ADD_IO_SEG(0x0001450000006FE0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_IC_DIV CVMX_NQM_IC_DIV_FUNC()
static inline uint64_t CVMX_NQM_IC_DIV_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_IC_DIV not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006018ull);
}
#else
#define CVMX_NQM_IC_DIV (CVMX_ADD_IO_SEG(0x0001450000006018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_INT CVMX_NQM_INT_FUNC()
static inline uint64_t CVMX_NQM_INT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_INT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006FF8ull);
}
#else
#define CVMX_NQM_INT (CVMX_ADD_IO_SEG(0x0001450000006FF8ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_LWA_SND_DBG CVMX_NQM_LWA_SND_DBG_FUNC()
static inline uint64_t CVMX_NQM_LWA_SND_DBG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_LWA_SND_DBG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000004018ull);
}
#else
#define CVMX_NQM_LWA_SND_DBG (CVMX_ADD_IO_SEG(0x0001450000004018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_MSIX_DBG CVMX_NQM_MSIX_DBG_FUNC()
static inline uint64_t CVMX_NQM_MSIX_DBG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_MSIX_DBG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000010608ull);
}
#else
#define CVMX_NQM_MSIX_DBG (CVMX_ADD_IO_SEG(0x0001450000010608ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_MSIX_DBG_CI_SM CVMX_NQM_MSIX_DBG_CI_SM_FUNC()
static inline uint64_t CVMX_NQM_MSIX_DBG_CI_SM_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_MSIX_DBG_CI_SM not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000010600ull);
}
#else
#define CVMX_NQM_MSIX_DBG_CI_SM (CVMX_ADD_IO_SEG(0x0001450000010600ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_MSIX_DBG_TW_SM CVMX_NQM_MSIX_DBG_TW_SM_FUNC()
static inline uint64_t CVMX_NQM_MSIX_DBG_TW_SM_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_MSIX_DBG_TW_SM not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000010A08ull);
}
#else
#define CVMX_NQM_MSIX_DBG_TW_SM (CVMX_ADD_IO_SEG(0x0001450000010A08ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_NCB_INT CVMX_NQM_NCB_INT_FUNC()
static inline uint64_t CVMX_NQM_NCB_INT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_NCB_INT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000007010ull);
}
#else
#define CVMX_NQM_NCB_INT (CVMX_ADD_IO_SEG(0x0001450000007010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_NCB_TX_ERR_INFO CVMX_NQM_NCB_TX_ERR_INFO_FUNC()
static inline uint64_t CVMX_NQM_NCB_TX_ERR_INFO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_NCB_TX_ERR_INFO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000007008ull);
}
#else
#define CVMX_NQM_NCB_TX_ERR_INFO (CVMX_ADD_IO_SEG(0x0001450000007008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_NCB_TX_ERR_WORD CVMX_NQM_NCB_TX_ERR_WORD_FUNC()
static inline uint64_t CVMX_NQM_NCB_TX_ERR_WORD_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_NCB_TX_ERR_WORD not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000007000ull);
}
#else
#define CVMX_NQM_NCB_TX_ERR_WORD (CVMX_ADD_IO_SEG(0x0001450000007000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_SCRATCH CVMX_NQM_SCRATCH_FUNC()
static inline uint64_t CVMX_NQM_SCRATCH_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_SCRATCH not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006038ull);
}
#else
#define CVMX_NQM_SCRATCH (CVMX_ADD_IO_SEG(0x0001450000006038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_ACQ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_ACQ(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000030ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_ACQ(offset) (0x0000000000000030ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_ACQ_CC(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_ACQ_CC(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000004020ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_ACQ_CC(offset) (CVMX_ADD_IO_SEG(0x0001450000004020ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_AQA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_AQA(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000024ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_AQA(offset) (0x0000000000000024ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_ASQ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_ASQ(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000028ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_ASQ(offset) (0x0000000000000028ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CAP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_CAP(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000000ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_CAP(offset) (0x0000000000000000ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CC(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_CC(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000014ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_CC(offset) (0x0000000000000014ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CPLX_BASE_ADDR_N_SZ(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CPLX_BASE_ADDR_N_SZ(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004050ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CPLX_BASE_ADDR_N_SZ(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004050ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CPLX_H(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CPLX_H(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004058ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CPLX_H(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004058ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CPLX_IFC(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CPLX_IFC(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004060ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CPLX_IFC(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004060ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CPLX_TDB(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CPLX_TDB(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004038ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CPLX_TDB(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004038ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQSM_DBG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_CQSM_DBG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000004040ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_CQSM_DBG(offset) (CVMX_ADD_IO_SEG(0x0001450000004040ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_BASE(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_BASE(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004018ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CQX_BASE(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004018ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_CC(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_CC(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004020ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CQX_CC(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004020ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_ENA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_ENA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004000ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CQX_ENA(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004000ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_HDBL(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_HDBL(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000001004ull + (((offset) & 31) + ((block_id) & 2047) * 0x4000ull) * 8;
}
#else
#define CVMX_NQM_VFX_CQX_HDBL(offset, block_id) (0x0000000000001004ull + (((offset) & 31) + ((block_id) & 2047) * 0x4000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_PRP(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_PRP(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004010ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CQX_PRP(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004010ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CQX_TAIL(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_CQX_TAIL(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004068ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_CQX_TAIL(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004068ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_CSTS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_CSTS(%lu) is invalid on this chip\n", offset);
	return 0x000000000000001Cull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_CSTS(offset) (0x000000000000001Cull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_IC_THR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_IC_THR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000010400ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_IC_THR(offset) (CVMX_ADD_IO_SEG(0x0001450000010400ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_IC_TIME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_IC_TIME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000010408ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_IC_TIME(offset) (CVMX_ADD_IO_SEG(0x0001450000010408ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000005038ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INT(offset) (CVMX_ADD_IO_SEG(0x0001450000005038ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INTMC(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INTMC(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000010ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INTMC(offset) (0x0000000000000010ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INTMS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INTMS(%lu) is invalid on this chip\n", offset);
	return 0x000000000000000Cull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INTMS(offset) (0x000000000000000Cull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INT_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INT_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000005048ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INT_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x0001450000005048ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INT_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INT_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000005050ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INT_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x0001450000005050ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000005040ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x0001450000005040ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_MSIX_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_MSIX_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000010808ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_MSIX_CONFIG(offset) (CVMX_ADD_IO_SEG(0x0001450000010808ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_MSIX_PBA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_MSIX_PBA(%lu) is invalid on this chip\n", offset);
	return 0x0000000000010200ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_MSIX_PBA(offset) (0x0000000000010200ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_NSSR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_NSSR(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000020ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_NSSR(offset) (0x0000000000000020ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQSM_DBG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_SQSM_DBG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001450000002038ull) + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_SQSM_DBG(offset) (CVMX_ADD_IO_SEG(0x0001450000002038ull) + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_BASE(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_BASE(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002020ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_BASE(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002020ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_CC(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_CC(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002008ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_CC(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002008ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_CREDIT(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_CREDIT(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002010ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_CREDIT(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002010ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_ENA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_ENA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002000ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_ENA(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002000ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_HEAD(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_HEAD(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002028ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_HEAD(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002028ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_IFC(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_IFC(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002030ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_IFC(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002030ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_PRP(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((((offset >= 1) && (offset <= 16))) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_PRP(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000002018ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_PRP(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000002018ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_SSO_SETUP(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_SSO_SETUP(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000004048ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128;
}
#else
#define CVMX_NQM_VFX_SQX_SSO_SETUP(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000004048ull) + (((offset) & 31) + ((block_id) & 2047) * 0x400ull) * 128)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_SQX_TDBL(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_SQX_TDBL(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000001000ull + (((offset) & 31) + ((block_id) & 2047) * 0x4000ull) * 8;
}
#else
#define CVMX_NQM_VFX_SQX_TDBL(offset, block_id) (0x0000000000001000ull + (((offset) & 31) + ((block_id) & 2047) * 0x4000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VECX_MSIX_ADDR(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_VECX_MSIX_ADDR(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000010000ull + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16;
}
#else
#define CVMX_NQM_VFX_VECX_MSIX_ADDR(offset, block_id) (0x0000000000010000ull + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VECX_MSIX_CD(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_VECX_MSIX_CD(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000010800ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16;
}
#else
#define CVMX_NQM_VFX_VECX_MSIX_CD(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000010800ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VECX_MSIX_CTL(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_VECX_MSIX_CTL(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000010008ull + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16;
}
#else
#define CVMX_NQM_VFX_VECX_MSIX_CTL(offset, block_id) (0x0000000000010008ull + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VECX_MSIX_INT_FLUSH(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_VECX_MSIX_INT_FLUSH(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000010E08ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16;
}
#else
#define CVMX_NQM_VFX_VECX_MSIX_INT_FLUSH(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000010E08ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VECX_MSIX_INT_ST(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 16)) && ((block_id <= 1027))))))
		cvmx_warn("CVMX_NQM_VFX_VECX_MSIX_INT_ST(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001450000010E00ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16;
}
#else
#define CVMX_NQM_VFX_VECX_MSIX_INT_ST(offset, block_id) (CVMX_ADD_IO_SEG(0x0001450000010E00ull) + (((offset) & 31) + ((block_id) & 2047) * 0x2000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_NQM_VFX_VS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 1027)))))
		cvmx_warn("CVMX_NQM_VFX_VS(%lu) is invalid on this chip\n", offset);
	return 0x0000000000000008ull + ((offset) & 2047) * 0x20000ull;
}
#else
#define CVMX_NQM_VFX_VS(offset) (0x0000000000000008ull + ((offset) & 2047) * 0x20000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_NQM_VF_MODE CVMX_NQM_VF_MODE_FUNC()
static inline uint64_t CVMX_NQM_VF_MODE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CN73XX)))
		cvmx_warn("CVMX_NQM_VF_MODE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001450000006008ull);
}
#else
#define CVMX_NQM_VF_MODE (CVMX_ADD_IO_SEG(0x0001450000006008ull))
#endif

/**
 * cvmx_nqm_cfg
 */
union cvmx_nqm_cfg {
	uint64_t u64;
	struct cvmx_nqm_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mjr                          : 16; /**< Major version number.
                                                         This global value is reflected in all NQM_VF()_VS[MJR] fields. */
	uint64_t mnr                          : 16; /**< Minor version number.
                                                         This global value is reflected in all NQM_VF()_VS[MNR] fields. */
	uint64_t reserved_16_31               : 16;
	uint64_t to                           : 8;  /**< Timeout.  Worst case time that host software shall wait
                                                         for the controller to become ready (NQM_VF()_CSTS[RDY] set) after a
                                                         power-on or reset.  This worst case time may be experienced after
                                                         an unclean shutdown; typical times are expected to be much shorter.
                                                         This field is in 500 millisecond units.  The reset value corresponds
                                                         to 127.5 seconds.
                                                         This global value is reflected in all NQM_VF()_CAP[TO] fields. */
	uint64_t reserved_3_7                 : 5;
	uint64_t ld_type                      : 2;  /**< Load type mode bit. Specifies the CPL entry load type (LDD if 0x1,
                                                         LDI (default) if 0x0, LDWB if 0x2). */
	uint64_t st_type                      : 1;  /**< Store type mode bit. Specifies the work-queue entry store type
                                                         (STT if 1, STF if 0). */
#else
	uint64_t st_type                      : 1;
	uint64_t ld_type                      : 2;
	uint64_t reserved_3_7                 : 5;
	uint64_t to                           : 8;
	uint64_t reserved_16_31               : 16;
	uint64_t mnr                          : 16;
	uint64_t mjr                          : 16;
#endif
	} s;
	struct cvmx_nqm_cfg_s                 cn73xx;
};
typedef union cvmx_nqm_cfg cvmx_nqm_cfg_t;

/**
 * cvmx_nqm_clken
 */
union cvmx_nqm_clken {
	uint64_t u64;
	struct cvmx_nqm_clken_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t clken                        : 1;  /**< Controls the conditional clocking within NQM.
                                                         0 = Allow hardware to control the clocks.
                                                         1 = Force the clocks to be always on. */
#else
	uint64_t clken                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_clken_s               cn73xx;
};
typedef union cvmx_nqm_clken cvmx_nqm_clken_t;

/**
 * cvmx_nqm_cs_bist_status0
 */
union cvmx_nqm_cs_bist_status0 {
	uint64_t u64;
	struct cvmx_nqm_cs_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t bist_status                  : 11; /**< BIST status bit vector for NQM_CS. Bit numbers within this field are enumerated by
                                                         NQM_CS_BIST_STATUS0_E. */
#else
	uint64_t bist_status                  : 11;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_nqm_cs_bist_status0_s     cn73xx;
};
typedef union cvmx_nqm_cs_bist_status0 cvmx_nqm_cs_bist_status0_t;

/**
 * cvmx_nqm_cs_cmd_dbg0
 */
union cvmx_nqm_cs_cmd_dbg0 {
	uint64_t u64;
	struct cvmx_nqm_cs_cmd_dbg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_45_63               : 19;
	uint64_t vals                         : 4;  /**< Valid bits into the SM.
                                                         <41> = r_nqmhs__nqmcs_dval.
                                                         <42> = r_nqmhs__nqmcs_info_val.
                                                         <43> = nfi__cs_cmdc_ptr0_val.
                                                         <44> = csr__cmdc_cmd_val.
                                                         For diagnostic use only. */
	uint64_t reserved_40_40               : 1;
	uint64_t vf                           : 11; /**< Current VF number in SM.
                                                         For diagnostic use only. */
	uint64_t reserved_28_28               : 1;
	uint64_t q                            : 5;  /**< Current queue number in SM.
                                                         For diagnostic use only. */
	uint64_t reserved_18_22               : 5;
	uint64_t cmd_sm_st                    : 4;  /**< CMD state machine state (NOT 1-hot).
                                                         For diagnostic use only. */
	uint64_t reserved_7_13                : 7;
	uint64_t lwa_has_cred                 : 1;  /**< LWA has credit.
                                                         For diagnostic use only. */
	uint64_t ncb_st_ccnt                  : 6;  /**< NCB store credit counter.
                                                         For diagnostic use only. */
#else
	uint64_t ncb_st_ccnt                  : 6;
	uint64_t lwa_has_cred                 : 1;
	uint64_t reserved_7_13                : 7;
	uint64_t cmd_sm_st                    : 4;
	uint64_t reserved_18_22               : 5;
	uint64_t q                            : 5;
	uint64_t reserved_28_28               : 1;
	uint64_t vf                           : 11;
	uint64_t reserved_40_40               : 1;
	uint64_t vals                         : 4;
	uint64_t reserved_45_63               : 19;
#endif
	} s;
	struct cvmx_nqm_cs_cmd_dbg0_s         cn73xx;
};
typedef union cvmx_nqm_cs_cmd_dbg0 cvmx_nqm_cs_cmd_dbg0_t;

/**
 * cvmx_nqm_cs_cpl_dbg0
 */
union cvmx_nqm_cs_cpl_dbg0 {
	uint64_t u64;
	struct cvmx_nqm_cs_cpl_dbg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t csr_op                       : 15; /**< CSR operation into CPL SM (1-hot).
                                                         For diagnostic use only. */
	uint64_t reserved_45_46               : 2;
	uint64_t vals                         : 3;  /**< Valid bits into the SM.
                                                         <42> = csr__cplc_cmd_val.
                                                         <43> = cq_prp_fifo_vld.
                                                         <44> = vf_work_arb_gnt_vld.
                                                         For diagnostic use only. */
	uint64_t reserved_40_41               : 2;
	uint64_t vf                           : 11; /**< Current VF number in SM.
                                                         For diagnostic use only. */
	uint64_t reserved_28_28               : 1;
	uint64_t q                            : 5;  /**< Current queue number in SM.
                                                         For diagnostic use only. */
	uint64_t reserved_22_22               : 1;
	uint64_t cpl_sm_st                    : 8;  /**< CPL state machine state (1-hot).
                                                         For diagnostic use only. */
	uint64_t reserved_10_13               : 4;
	uint64_t cpl_eg_dat_cred              : 1;  /**< CPL egress data fifo has credit.
                                                         For diagnostic use only. */
	uint64_t cpl_eg_inf_cred              : 1;  /**< CPL egress info fifo has credit.
                                                         For diagnostic use only. */
	uint64_t ncb_ccnt                     : 3;  /**< NCB load credit counter.
                                                         For diagnostic use only. */
	uint64_t swi_ccnt                     : 5;  /**< SWI credit counter.
                                                         For diagnostic use only. */
#else
	uint64_t swi_ccnt                     : 5;
	uint64_t ncb_ccnt                     : 3;
	uint64_t cpl_eg_inf_cred              : 1;
	uint64_t cpl_eg_dat_cred              : 1;
	uint64_t reserved_10_13               : 4;
	uint64_t cpl_sm_st                    : 8;
	uint64_t reserved_22_22               : 1;
	uint64_t q                            : 5;
	uint64_t reserved_28_28               : 1;
	uint64_t vf                           : 11;
	uint64_t reserved_40_41               : 2;
	uint64_t vals                         : 3;
	uint64_t reserved_45_46               : 2;
	uint64_t csr_op                       : 15;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_nqm_cs_cpl_dbg0_s         cn73xx;
};
typedef union cvmx_nqm_cs_cpl_dbg0 cvmx_nqm_cs_cpl_dbg0_t;

/**
 * cvmx_nqm_cs_ecc0_int
 */
union cvmx_nqm_cs_ecc0_int {
	uint64_t u64;
	struct cvmx_nqm_cs_ecc0_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_31_63               : 33;
	uint64_t sbe                          : 11; /**< Single-bit error vector for NQM_CS. Throws NQM_INTSN_E::NQM_CS_SBE0().
                                                         Bit numbers within this field are enumerated by NQM_CS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_11_19               : 9;
	uint64_t dbe                          : 11; /**< Double-bit error vector for NQM_CS. Throws NQM_INTSN_E::NQM_CS_DBE0().
                                                         Bit numbers within this field are enumerated by NQM_CS_MEM_CTL0_RAMS_E. */
#else
	uint64_t dbe                          : 11;
	uint64_t reserved_11_19               : 9;
	uint64_t sbe                          : 11;
	uint64_t reserved_31_63               : 33;
#endif
	} s;
	struct cvmx_nqm_cs_ecc0_int_s         cn73xx;
};
typedef union cvmx_nqm_cs_ecc0_int cvmx_nqm_cs_ecc0_int_t;

/**
 * cvmx_nqm_cs_mem_ctl0
 */
union cvmx_nqm_cs_mem_ctl0 {
	uint64_t u64;
	struct cvmx_nqm_cs_mem_ctl0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t flip1                        : 11; /**< Flip[1] syndrome bit 1 vector. Bit numbers within this field are enumerated by
                                                         NQM_CS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_31_39               : 9;
	uint64_t flip0                        : 11; /**< Flip[0] syndrome bit 0 vector. Bit numbers within this field are enumerated by
                                                         NQM_CS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_11_19               : 9;
	uint64_t cdis                         : 11; /**< ECC correction disable vector. Bit numbers within this field are enumerated by
                                                         NQM_CS_MEM_CTL0_RAMS_E. */
#else
	uint64_t cdis                         : 11;
	uint64_t reserved_11_19               : 9;
	uint64_t flip0                        : 11;
	uint64_t reserved_31_39               : 9;
	uint64_t flip1                        : 11;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_nqm_cs_mem_ctl0_s         cn73xx;
};
typedef union cvmx_nqm_cs_mem_ctl0 cvmx_nqm_cs_mem_ctl0_t;

/**
 * cvmx_nqm_fi_fpa_aura
 */
union cvmx_nqm_fi_fpa_aura {
	uint64_t u64;
	struct cvmx_nqm_fi_fpa_aura_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_28_63               : 36;
	uint64_t retry_timer                  : 12; /**< Retry timer. Specifies the time period in roughly 1 us (1024 coprocessor-clocks)
                                                         increments for retrying FPA pointer requests that are not granted.
                                                         The value 0x0 disables retries.  The maximum value for this timer
                                                         is suggested to not exceed 50 us. */
	uint64_t reserved_12_15               : 4;
	uint64_t node                         : 2;  /**< Reserved. */
	uint64_t laura                        : 10; /**< Local aura to use for NQM work-queue entry allocates.
                                                         The FPA aura selected by LAURA must correspond to an FPA pool
                                                         where the buffers (after any FPA_POOL()_CFG[BUF_OFFSET]) are at least 128B. */
#else
	uint64_t laura                        : 10;
	uint64_t node                         : 2;
	uint64_t reserved_12_15               : 4;
	uint64_t retry_timer                  : 12;
	uint64_t reserved_28_63               : 36;
#endif
	} s;
	struct cvmx_nqm_fi_fpa_aura_s         cn73xx;
};
typedef union cvmx_nqm_fi_fpa_aura cvmx_nqm_fi_fpa_aura_t;

/**
 * cvmx_nqm_fpa_dbg
 */
union cvmx_nqm_fpa_dbg {
	uint64_t u64;
	struct cvmx_nqm_fpa_dbg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_42_63               : 22;
	uint64_t ptr                          : 35; /**< FPA pointer.  This value may be returned to FPA via software.
                                                         For diagnostic use only. */
	uint64_t reserved_1_6                 : 6;
	uint64_t ptr_req                      : 1;  /**< FPA pointer request.  A pointer request is sent to FPA when this field
                                                         is written to 1.  Note that the aura and node id of the FPA pointer
                                                         request come from NQM_FI_FPA_AURA.  For diagnostic use only. */
#else
	uint64_t ptr_req                      : 1;
	uint64_t reserved_1_6                 : 6;
	uint64_t ptr                          : 35;
	uint64_t reserved_42_63               : 22;
#endif
	} s;
	struct cvmx_nqm_fpa_dbg_s             cn73xx;
};
typedef union cvmx_nqm_fpa_dbg cvmx_nqm_fpa_dbg_t;

/**
 * cvmx_nqm_glbl_tag
 */
union cvmx_nqm_glbl_tag {
	uint64_t u64;
	struct cvmx_nqm_glbl_tag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_42_63               : 22;
	uint64_t tag_mode                     : 2;  /**< The mode NQM uses to create NQM_WQE_S[TAG] in all WQE's created by NQM.
                                                         Enumerated by NQM_TAG_MODE_E. */
	uint64_t reserved_32_39               : 8;
	uint64_t tag                          : 32; /**< Part of NQM_WQE_S[TAG] for all WQE's created by NQM. See [TAG_MODE] and
                                                         NQM_TAG_MODE_E. */
#else
	uint64_t tag                          : 32;
	uint64_t reserved_32_39               : 8;
	uint64_t tag_mode                     : 2;
	uint64_t reserved_42_63               : 22;
#endif
	} s;
	struct cvmx_nqm_glbl_tag_s            cn73xx;
};
typedef union cvmx_nqm_glbl_tag cvmx_nqm_glbl_tag_t;

/**
 * cvmx_nqm_hs_bist_status0
 */
union cvmx_nqm_hs_bist_status0 {
	uint64_t u64;
	struct cvmx_nqm_hs_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t bist_status                  : 19; /**< BIST status bit vector for NQM_HS. Bit numbers within this field are enumerated by
                                                         NQM_HS_BIST_STATUS0_E. */
#else
	uint64_t bist_status                  : 19;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_nqm_hs_bist_status0_s     cn73xx;
};
typedef union cvmx_nqm_hs_bist_status0 cvmx_nqm_hs_bist_status0_t;

/**
 * cvmx_nqm_hs_ecc0_int
 */
union cvmx_nqm_hs_ecc0_int {
	uint64_t u64;
	struct cvmx_nqm_hs_ecc0_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_36_63               : 28;
	uint64_t sbe                          : 16; /**< Single-bit error vector for NQM_HS. Throws NQM_INTSN_E::NQM_HS_SBE0().
                                                         Bit numbers within this field are enumerated by NQM_HS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_16_19               : 4;
	uint64_t dbe                          : 16; /**< Double-bit error vector for NQM_HS. Throws NQM_INTSN_E::NQM_HS_DBE0().
                                                         Bit numbers within this field are enumerated by NQM_HS_MEM_CTL0_RAMS_E. */
#else
	uint64_t dbe                          : 16;
	uint64_t reserved_16_19               : 4;
	uint64_t sbe                          : 16;
	uint64_t reserved_36_63               : 28;
#endif
	} s;
	struct cvmx_nqm_hs_ecc0_int_s         cn73xx;
};
typedef union cvmx_nqm_hs_ecc0_int cvmx_nqm_hs_ecc0_int_t;

/**
 * cvmx_nqm_hs_mem_ctl0
 */
union cvmx_nqm_hs_mem_ctl0 {
	uint64_t u64;
	struct cvmx_nqm_hs_mem_ctl0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_56_63               : 8;
	uint64_t flip1                        : 16; /**< Flip[1] syndrome bit 1 vector. Bit numbers within this field are enumerated by
                                                         NQM_HS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_36_39               : 4;
	uint64_t flip0                        : 16; /**< Flip[0] syndrome bit 0 vector. Bit numbers within this field are enumerated by
                                                         NQM_HS_MEM_CTL0_RAMS_E. */
	uint64_t reserved_16_19               : 4;
	uint64_t cdis                         : 16; /**< ECC correction disable vector. Bit numbers within this field are enumerated by
                                                         NQM_HS_MEM_CTL0_RAMS_E. */
#else
	uint64_t cdis                         : 16;
	uint64_t reserved_16_19               : 4;
	uint64_t flip0                        : 16;
	uint64_t reserved_36_39               : 4;
	uint64_t flip1                        : 16;
	uint64_t reserved_56_63               : 8;
#endif
	} s;
	struct cvmx_nqm_hs_mem_ctl0_s         cn73xx;
};
typedef union cvmx_nqm_hs_mem_ctl0 cvmx_nqm_hs_mem_ctl0_t;

/**
 * cvmx_nqm_ic_div
 *
 * This register controls the divider for the interrupt coalescing time.
 *
 */
union cvmx_nqm_ic_div {
	uint64_t u64;
	struct cvmx_nqm_ic_div_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t div                          : 18; /**< Coalescing divider (used for MSI-X only). As defined in the NVMe standard,
                                                         Time-based coalescing operates with a 100 microsecond tick. This register
                                                         specifies how many coprocessor-clocks there are in a 25 microsecond period. If
                                                         the coprocessor-clock period is 1.0 ns then the default value of 25,000 is
                                                         correct. The value 66 is a minimum; this is forced if you write below 66.
                                                         _ Timer_tick_period = coprossoror_clock_freq * [DIV] */
#else
	uint64_t div                          : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_nqm_ic_div_s              cn73xx;
};
typedef union cvmx_nqm_ic_div cvmx_nqm_ic_div_t;

/**
 * cvmx_nqm_int
 */
union cvmx_nqm_int {
	uint64_t u64;
	struct cvmx_nqm_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t pcie_mac_reset               : 1;  /**< Asserted when the PCIE core is reset (i.e RST_SOFT_PRST()[SOFT_PRST] is cleared).
                                                         NQM clears all NQM_VF(0..1027)_CC[EN] when it sets [PCIE_MAC_RESET]. */
	uint64_t pcie_vf_enable_clr           : 1;  /**< Asserted when the PCIE SR-IOV VF_ENABLE configuration transitions from
                                                         enabled to disabled.
                                                         NQM clears all VF NQM_VF(1..1027)_CC[EN] when it sets [PCIE_VF_ENABLE_CLR].
                                                         When [PCIE_VF_ENABLE_CLR] asserts at any point during the during a
                                                         complete PF FLR sequence, software should clear [PCIE_VF_ENABLE_CLR]
                                                         and ignore it. NQM asserts NQM_VF(0)_INT[FLR] at the beginning of
                                                         a PF FLR. */
	uint64_t fpa_no_ptrs                  : 1;  /**< NQM recieved a signal for no FPA pointers available. */
#else
	uint64_t fpa_no_ptrs                  : 1;
	uint64_t pcie_vf_enable_clr           : 1;
	uint64_t pcie_mac_reset               : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_nqm_int_s                 cn73xx;
};
typedef union cvmx_nqm_int cvmx_nqm_int_t;

/**
 * cvmx_nqm_lwa_snd_dbg
 */
union cvmx_nqm_lwa_snd_dbg {
	uint64_t u64;
	struct cvmx_nqm_lwa_snd_dbg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t wqp                          : 29; /**< On NQM LWA interface, force send an addwork to SSO. Work queue pointer field.
                                                         For diagnostic use only. */
	uint64_t tag                          : 23; /**< On NQM LWA interface, force send an addwork to SSO. Tag field.
                                                         For diagnostic use only. */
	uint64_t ttype                        : 2;  /**< On NQM LWA interface, force send an addwork to SSO. Tag type field.
                                                         For diagnostic use only. */
	uint64_t grp                          : 10; /**< On NQM LWA interface, force send an addwork to SSO. Group field.
                                                         For diagnostic use only. */
#else
	uint64_t grp                          : 10;
	uint64_t ttype                        : 2;
	uint64_t tag                          : 23;
	uint64_t wqp                          : 29;
#endif
	} s;
	struct cvmx_nqm_lwa_snd_dbg_s         cn73xx;
};
typedef union cvmx_nqm_lwa_snd_dbg cvmx_nqm_lwa_snd_dbg_t;

/**
 * cvmx_nqm_msix_dbg
 *
 * This register gives access to MSIX hardware state for debug.
 *
 */
union cvmx_nqm_msix_dbg {
	uint64_t u64;
	struct cvmx_nqm_msix_dbg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t dcnt                         : 18; /**< Current state of MSIX state machine; divider count. */
	uint64_t reserved_45_45               : 1;
	uint64_t tcnt                         : 12; /**< Current state of MSIX state machine; timer count. */
	uint64_t reserved_27_32               : 6;
	uint64_t stalled                      : 1;  /**< Timewheel is stalled. */
	uint64_t reserved_25_25               : 1;
	uint64_t tw_stl_adr                   : 13; /**< Timewheel stall address. */
	uint64_t reserved_10_11               : 2;
	uint64_t swi_fif_cnt                  : 5;  /**< Timewheel stall address. */
	uint64_t reserved_0_4                 : 5;
#else
	uint64_t reserved_0_4                 : 5;
	uint64_t swi_fif_cnt                  : 5;
	uint64_t reserved_10_11               : 2;
	uint64_t tw_stl_adr                   : 13;
	uint64_t reserved_25_25               : 1;
	uint64_t stalled                      : 1;
	uint64_t reserved_27_32               : 6;
	uint64_t tcnt                         : 12;
	uint64_t reserved_45_45               : 1;
	uint64_t dcnt                         : 18;
#endif
	} s;
	struct cvmx_nqm_msix_dbg_s            cn73xx;
};
typedef union cvmx_nqm_msix_dbg cvmx_nqm_msix_dbg_t;

/**
 * cvmx_nqm_msix_dbg_ci_sm
 *
 * This register gives access to MSIX hardware state for debug.
 *
 */
union cvmx_nqm_msix_dbg_ci_sm {
	uint64_t u64;
	struct cvmx_nqm_msix_dbg_ci_sm_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cur_iv                       : 5;  /**< Current state of MSIX state machine; current IV. */
	uint64_t reserved_25_26               : 2;
	uint64_t cur_vf                       : 11; /**< Current state of MSIX state machine; current VF. */
	uint64_t reserved_8_13                : 6;
	uint64_t irq_state                    : 2;  /**< Current state of MSIX state machine; interrupt state. */
	uint64_t reserved_4_5                 : 2;
	uint64_t cur_state                    : 4;  /**< Current state of MSIX state machine. */
#else
	uint64_t cur_state                    : 4;
	uint64_t reserved_4_5                 : 2;
	uint64_t irq_state                    : 2;
	uint64_t reserved_8_13                : 6;
	uint64_t cur_vf                       : 11;
	uint64_t reserved_25_26               : 2;
	uint64_t cur_iv                       : 5;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_nqm_msix_dbg_ci_sm_s      cn73xx;
};
typedef union cvmx_nqm_msix_dbg_ci_sm cvmx_nqm_msix_dbg_ci_sm_t;

/**
 * cvmx_nqm_msix_dbg_tw_sm
 *
 * This register gives access to MSIX hardware state for debug.
 *
 */
union cvmx_nqm_msix_dbg_tw_sm {
	uint64_t u64;
	struct cvmx_nqm_msix_dbg_tw_sm_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cur_iv                       : 5;  /**< Current state of MSIX state machine; current IV. */
	uint64_t reserved_25_26               : 2;
	uint64_t cur_vf                       : 11; /**< Current state of MSIX state machine; current VF. */
	uint64_t reserved_8_13                : 6;
	uint64_t irq_state                    : 2;  /**< Current state of MSIX state machine; interrupt state. */
	uint64_t reserved_4_5                 : 2;
	uint64_t cur_state                    : 4;  /**< Current state of MSIX state machine. */
#else
	uint64_t cur_state                    : 4;
	uint64_t reserved_4_5                 : 2;
	uint64_t irq_state                    : 2;
	uint64_t reserved_8_13                : 6;
	uint64_t cur_vf                       : 11;
	uint64_t reserved_25_26               : 2;
	uint64_t cur_iv                       : 5;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_nqm_msix_dbg_tw_sm_s      cn73xx;
};
typedef union cvmx_nqm_msix_dbg_tw_sm cvmx_nqm_msix_dbg_tw_sm_t;

/**
 * cvmx_nqm_ncb_int
 */
union cvmx_nqm_ncb_int {
	uint64_t u64;
	struct cvmx_nqm_ncb_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ncb_tx_error                 : 1;  /**< NCB transaction error occurred (error/unpredictable/undefined).
                                                         Throws NQM_INTSN_E::NQM_NCB_TX_ERR. */
#else
	uint64_t ncb_tx_error                 : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_ncb_int_s             cn73xx;
};
typedef union cvmx_nqm_ncb_int cvmx_nqm_ncb_int_t;

/**
 * cvmx_nqm_ncb_tx_err_info
 */
union cvmx_nqm_ncb_tx_err_info {
	uint64_t u64;
	struct cvmx_nqm_ncb_tx_err_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t wcnt                         : 5;  /**< Word count. */
	uint64_t src                          : 12; /**< Source of the IOI outbound transaction. */
	uint64_t dst                          : 8;  /**< Destination of the IOI outbound transaction. */
	uint64_t tag                          : 4;  /**< Tag of the IOI outbound transaction. */
	uint64_t eot                          : 1;  /**< EOT bit of the NCBO transaction. */
	uint64_t sot                          : 1;  /**< SOT bit of the NCBO transaction. */
	uint64_t valid                        : 1;  /**< Valid bit for transaction (should always be 1 on capture). */
#else
	uint64_t valid                        : 1;
	uint64_t sot                          : 1;
	uint64_t eot                          : 1;
	uint64_t tag                          : 4;
	uint64_t dst                          : 8;
	uint64_t src                          : 12;
	uint64_t wcnt                         : 5;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_nqm_ncb_tx_err_info_s     cn73xx;
};
typedef union cvmx_nqm_ncb_tx_err_info cvmx_nqm_ncb_tx_err_info_t;

/**
 * cvmx_nqm_ncb_tx_err_word
 */
union cvmx_nqm_ncb_tx_err_word {
	uint64_t u64;
	struct cvmx_nqm_ncb_tx_err_word_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t err_word                     : 64; /**< NQM NCB error word (first word of erroneous transaction).
                                                         This is only the 64-bit data word; the NCB info that goes with it is
                                                         in NQM_NCB_TX_ERR_INFO. */
#else
	uint64_t err_word                     : 64;
#endif
	} s;
	struct cvmx_nqm_ncb_tx_err_word_s     cn73xx;
};
typedef union cvmx_nqm_ncb_tx_err_word cvmx_nqm_ncb_tx_err_word_t;

/**
 * cvmx_nqm_scratch
 */
union cvmx_nqm_scratch {
	uint64_t u64;
	struct cvmx_nqm_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t scratch                      : 64; /**< Scratch data. */
#else
	uint64_t scratch                      : 64;
#endif
	} s;
	struct cvmx_nqm_scratch_s             cn73xx;
};
typedef union cvmx_nqm_scratch cvmx_nqm_scratch_t;

/**
 * cvmx_nqm_vf#_acq
 *
 * NQM_VF(x)_ACQ is the NVMe-standard admin completion queue base address (ACQ)
 * CSR for function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when the corresponding
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ(0)_ENA[ENABLE] is an enable
 * for the admin CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_acq {
	uint64_t u64;
	struct cvmx_nqm_vfx_acq_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t acqb                         : 52; /**< Admin completion queue base.  Indicates the 64-bit physical address
                                                         for the admin completion queue. This address shall be memory page
                                                         aligned (based upon NQM_VF()_CC[MPS]). All completion queue entries
                                                         for the commands submitted to the admin submission queue shall be
                                                         posted to this completion queue. This queue is always associated with
                                                         interrupt vector 0.  This field is read-only when both NQM_VF()_CC[EN]
                                                         and NQM_VF()_CSTS[RDY] are asserted. */
	uint64_t reserved_0_11                : 12;
#else
	uint64_t reserved_0_11                : 12;
	uint64_t acqb                         : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_acq_s             cn73xx;
};
typedef union cvmx_nqm_vfx_acq cvmx_nqm_vfx_acq_t;

/**
 * cvmx_nqm_vf#_acq_cc
 *
 * NQM_VF(x)_ACQ_CC contains admin completion queue configuration for function x.
 * NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when the corresponding
 * admin CPL/CQ is idle. The corresponding NQM_VF()_CQ(0)_ENA[ENABLE] is an enable
 * for the admin CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_acq_cc {
	uint64_t u64;
	struct cvmx_nqm_vfx_acq_cc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t iv                           : 5;  /**< Interrupt vector.  Interrupt vector associated with this queue.
                                                         Must be zero (i.e. must be NQM_VF_INT_VEC_E::NQM_ADMIN_CQ). */
	uint64_t reserved_3_3                 : 1;
	uint64_t p                            : 1;  /**< Phase tag. Indicates the phase tag that NQM will
                                                         write with the next completion.
                                                         Software should set [P]=1 prior to first enabling a new
                                                         CPL/CQ.
                                                         [P] and corresponding NQM_VF()_CQ(0)_TAIL[PT] are one and the same.
                                                         A write to either [P] or corresponding NQM_VF()_CQ(0)_TAIL[PT] updates
                                                         both. */
	uint64_t ien                          : 1;  /**< Interrupt enable. Interrupts are enabled when asserted.
                                                         [IEN] should always be set since admin CQ interrupts cannot be disabled (according
                                                         to the NVMe standard). */
	uint64_t reserved_0_0                 : 1;
#else
	uint64_t reserved_0_0                 : 1;
	uint64_t ien                          : 1;
	uint64_t p                            : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t iv                           : 5;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_nqm_vfx_acq_cc_s          cn73xx;
};
typedef union cvmx_nqm_vfx_acq_cc cvmx_nqm_vfx_acq_cc_t;

/**
 * cvmx_nqm_vf#_aqa
 *
 * NQM_VF(x)_AQA is the NVMe-standard admin queue attributes (AQA)
 * CSR for function x. NQM_VF_MODE[VF_MODE] selects the functions that
 * are actually present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when the corresponding
 * CPL/CQ and SQ are idle. The corresponding NQM_VF()_CQ(0)_ENA[ENABLE] is
 * an enable for the CPL/CQ.  NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle
 * conditions. The corresponding NQM_VF()_SQ(0)_ENA[ENABLE] is an enable for
 * the SQ. NQM_VF()_SQ(0)_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_aqa {
	uint32_t u32;
	struct cvmx_nqm_vfx_aqa_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t reserved_28_31               : 4;
	uint32_t acqs                         : 12; /**< Admin completion queue size. Defines the size of the admin completion
                                                         queue in entries. The minimum size of the admin completion queue is
                                                         two entries. The maximum size of the admin completion queue is 4096
                                                         entries. This field is read-only when both NQM_VF()_CC[EN] and
                                                         NQM_VF()_CSTS[RDY] are asserted. */
	uint32_t reserved_12_15               : 4;
	uint32_t asqs                         : 12; /**< Admin submission queue size. Defines the size of the admin submission
                                                         queue in entries. The minimum size of the admin submission queue is
                                                         two entries. The maximum size of the admin submission queue is 4096
                                                         entries. This field is read-only when both NQM_VF()_CC[EN] and
                                                         NQM_VF()_CSTS[RDY] are asserted. */
#else
	uint32_t asqs                         : 12;
	uint32_t reserved_12_15               : 4;
	uint32_t acqs                         : 12;
	uint32_t reserved_28_31               : 4;
#endif
	} s;
	struct cvmx_nqm_vfx_aqa_s             cn73xx;
};
typedef union cvmx_nqm_vfx_aqa cvmx_nqm_vfx_aqa_t;

/**
 * cvmx_nqm_vf#_asq
 *
 * NQM_VF(x)_ASQ is the NVMe-standard admin submission queue base address (ASQ)
 * CSR for function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when the corresponding
 * admin SQ is idle. The corresponding NQM_VF()_SQ(0)_ENA[ENABLE] is an enable
 * for the admin SQ. NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_asq {
	uint64_t u64;
	struct cvmx_nqm_vfx_asq_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t asqb                         : 52; /**< Admin submission queue base. Indicates the 64-bit physical address for
                                                         the admin submission queue. This address shall be memory page aligned
                                                         (based upon NQM_VF()_CC[MPS]). All admin commands, including creation
                                                         of I/O submission queues and I/O completions queues shall be submitted
                                                         to this queue.  This field is read-only when both NQM_VF()_CC[EN]
                                                         and NQM_VF()_CSTS[RDY] are asserted. */
	uint64_t reserved_0_11                : 12;
#else
	uint64_t reserved_0_11                : 12;
	uint64_t asqb                         : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_asq_s             cn73xx;
};
typedef union cvmx_nqm_vfx_asq cvmx_nqm_vfx_asq_t;

/**
 * cvmx_nqm_vf#_cap
 *
 * NQM_VF(x)_CAP is the NVMe-standard controller capabilities (CAP) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_cap {
	uint64_t u64;
	struct cvmx_nqm_vfx_cap_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_56_63               : 8;
	uint64_t mpsmax                       : 4;  /**< Memory page size maximum.  The maximum host
                                                         memory page size supported by this controller, specified
                                                         as (2 ^ (12 + MPSMAX)) bytes.  This controller supports the full
                                                         range from 4 KB to 128 MB as denoted by the value of 0xF. */
	uint64_t mpsmin                       : 4;  /**< Memory page size minimum. The minimum host memory
                                                         page size supported by this controller, specified as
                                                         (2 ^ (12 + MPSMIN)) bytes, thus 0x0 corresponds to 4 KB.  The host
                                                         shall not configure a memory page size in NQM_VF()_CC[MPS] that is
                                                         smaller than this value. */
	uint64_t reserved_45_47               : 3;
	uint64_t css                          : 8;  /**< Command set supported. Only the NVM command set is supported. */
	uint64_t nssrs                        : 1;  /**< NVM subsystem reset is not supported. */
	uint64_t dstrd                        : 4;  /**< Doorbell stride.  The stride is specified as (2 ^ (2 + DSTRD)) in
                                                         bytes.  A value of 0x0 indicates a stride of 4 bytes, where the
                                                         doorbell registers are packed without reserved space between each
                                                         register. */
	uint64_t to                           : 8;  /**< Timeout.  Worst case time that host software shall wait
                                                         for the controller to become ready (NQM_VF()_CSTS[RDY] set) after a
                                                         power-on or reset.  This worst case time may be experienced after
                                                         an unclean shutdown; typical times are expected to be much shorter.
                                                         This field is in 500 millisecond units.  The reset value corresponds
                                                         to 127.5 seconds.
                                                         [TO] is a RO copy of NQM_CFG[TO]. [TO] can be modified by software
                                                         via NQM_CFG. */
	uint64_t reserved_19_23               : 5;
	uint64_t ams                          : 2;  /**< Arbitration mechanism supported.  Only round-robin is supported. */
	uint64_t cqr                          : 1;  /**< Contiguous queues required.  The controller supports I/O submission
                                                         queues and I/O completion queues that are not physically contiguous. */
	uint64_t mqes                         : 16; /**< Maximum queue entries supported.  Indicates the maximum
                                                         individual queue size that the controller supports. This value applies
                                                         to each of the I/O submission queues and I/O completion queues that
                                                         host software may create. This is a 0's based value. The value
                                                         0x0FFF indicate 4096 entries. */
#else
	uint64_t mqes                         : 16;
	uint64_t cqr                          : 1;
	uint64_t ams                          : 2;
	uint64_t reserved_19_23               : 5;
	uint64_t to                           : 8;
	uint64_t dstrd                        : 4;
	uint64_t nssrs                        : 1;
	uint64_t css                          : 8;
	uint64_t reserved_45_47               : 3;
	uint64_t mpsmin                       : 4;
	uint64_t mpsmax                       : 4;
	uint64_t reserved_56_63               : 8;
#endif
	} s;
	struct cvmx_nqm_vfx_cap_s             cn73xx;
};
typedef union cvmx_nqm_vfx_cap cvmx_nqm_vfx_cap_t;

/**
 * cvmx_nqm_vf#_cc
 *
 * NQM_VF(x)_CC is the NVMe-standard controller configuration (CC) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_cc {
	uint32_t u32;
	struct cvmx_nqm_vfx_cc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t reserved_24_31               : 8;
	uint32_t iocqes                       : 4;  /**< I/O completion queue entry size.  Defines the I/O
                                                         completion queue entry size in bytes and is formatted as a power of
                                                         two (2^n).  The value 0x4 corresponds to a size of 16 bytes.  The
                                                         value is also reflected in byte 513 of the identify controller data
                                                         structure. */
	uint32_t iosqes                       : 4;  /**< I/O submission queue entry size.  Defines the I/O
                                                         submission queue entry size  in bytes and is formatted as a power of
                                                         two (2^n).  The value 0x6 corresponds to a size of 64 bytes.  The
                                                         value is also reflected in byte 512 of the identify controller data
                                                         structure. */
	uint32_t shn                          : 2;  /**< Shutdown notification.  Initiates shutdown
                                                         processing when a shutdown is occurring (i.e., a power down condition
                                                         is expected). For a normal shutdown notification, it is expected that
                                                         the controller is given time to process the shutdown notification.
                                                         For an abrupt shutdown notification, the host may not wait for
                                                         shutdown processing to complete before power is lost.
                                                         The shutdown notification values are defined as:
                                                              0x0 = No notification; no effect.
                                                              0x1 = Normal shutdown notification.
                                                              0x2 = Abrupt shutdown notification.
                                                         This field should be written by host software prior to any power down
                                                         condition and prior to any change of the PCI power management state.
                                                         It is recommended that this field also be written prior to a warm
                                                         reboot. To determine when shutdown processing is complete, refer to
                                                         NQM_VF()_CSTS[SHST]. */
	uint32_t ams                          : 3;  /**< Arbitration mechanism selected:  Selects the arbitration
                                                         mechanism to be used.  The controller only support round-robin (0x0). */
	uint32_t mps                          : 4;  /**< Memory page size.  Indicates the host memory page size in
                                                         (2 ^ (12 + MPS)) bytes. The minimum host memory page
                                                         size is 4KB and the maximum host memory page size is 128MB as defined
                                                         by NQM_VF()_CAP[MPSMAX] and NQM_VF()_CAP[MPSMIN].
                                                         This field describes the value used for PRP entry size. */
	uint32_t css                          : 3;  /**< I/O command set selected:  Only NVM command set (0x0) is supported. */
	uint32_t reserved_1_3                 : 3;
	uint32_t en                           : 1;  /**< Enable. When set, enables the NVMe controller. [EN] is a queue
                                                         enable for the SQ's and CPL/CQ's together with corresponding
                                                         NQM_VF()_CSTS[RDY] and NQM_VF()_SQ()_ENA[ENABLE]/NQM_VF()_CQ()_ENA[ENABLE].
                                                         See the NQM_VF()_SQ()_ENA[ENABLE] and NQM_VF()_CQ()_ENA[ENABLE]
                                                         descriptions.
                                                         NQM clears [EN] on an FLR to the corresponding function. This
                                                         immediately disables the SQ and CPL/CQ transfers for the function.
                                                         NQM clears all NQM_VF(0..1027)_CC[EN] on a PF FLR or a PCIe
                                                         MAC reset. See NQM_VF(0..1027)_INT[FLR] and NQM_INT[PCIE_MAC_RESET].
                                                         See NQM_VF()_INT[FLR,CCW] and corresponding NQM_INTSN_E::NQM_VF()_INT.
                                                         OCTEON CPUs can receive an interrupt when [EN] transitions,
                                                         perform a controller reset, and eventually clear corresponding
                                                         NQM_VF()_CSTS[RDY].
                                                         NQM clears all NQM_VF(1..1027)_CC[EN] on a VF_ENABLE de-assertion.
                                                         VF_ENABLE has no effect on NQM_VF(0)_CC[EN]. See
                                                         NQM_INT[PCIE_VF_ENABLE_CLR].
                                                         Current NVMe specifications for [EN]:
                                                         _ When set, the controller processes commands based
                                                          on submission queue tail doorbell writes. When clear, the
                                                          controller does not process commands nor post completion queue
                                                          entries to Completion Queues. When this field transitions from set to
                                                          clear, the controller is reset (referred to as a controller reset). The
                                                          reset deletes all I/O submission queues and I/O completion queues,
                                                          resets the admin submission queue and completion queue, and brings
                                                          the hardware to an idle state. The reset does not affect PCI Express
                                                          registers (including MMIO MSI-X registers), nor the admin queue
                                                          registers (AQA, ASQ, or ACQ). All other
                                                          controller registers and internal controller
                                                          state are reset to their default values. The controller ensures
                                                          that there is no data loss for commands that have had corresponding
                                                          completion queue entries posted to an I/O completion queue prior to
                                                          the reset operation.
                                                         _ When [EN] is cleared to 0, NQM_VF()_CSTS[RDY] is cleared to 0 by
                                                          the controller once the controller is ready to be re-enabled.
                                                          When [EN] is set to 1, the controller sets NQM_VF()_CSTS[RDY]
                                                          to 1 when it is ready to process commands. CSTS.RDY may be set
                                                          to 1 before namespace(s) are ready to be accessed.
                                                         _ Setting [EN] from a 0 to a 1 when NQM_VF()_CSTS[RDY] is a 1,
                                                          or clearing [EN] from a '1' to a '0' when NQM_VF()_CSTS[RDY]
                                                          is a '0,' has undefined results. The Admin Queue registers
                                                          (AQA, ASQ, and ACQ) shall only be modified when [EN] is cleared
                                                          to 0. */
#else
	uint32_t en                           : 1;
	uint32_t reserved_1_3                 : 3;
	uint32_t css                          : 3;
	uint32_t mps                          : 4;
	uint32_t ams                          : 3;
	uint32_t shn                          : 2;
	uint32_t iosqes                       : 4;
	uint32_t iocqes                       : 4;
	uint32_t reserved_24_31               : 8;
#endif
	} s;
	struct cvmx_nqm_vfx_cc_s              cn73xx;
};
typedef union cvmx_nqm_vfx_cc cvmx_nqm_vfx_cc_t;

/**
 * cvmx_nqm_vf#_cpl#_base_addr_n_sz
 *
 * NQM_VF(x)_CPL(0..16)_BASE_ADDR_N_SZ are the CPL queue base addresses
 * for function x. Includes both admin and IO queues. NQM_VF_MODE[VF_MODE]
 * selects the queues and functions that are actually present in the
 * hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cplx_base_addr_n_sz {
	uint64_t u64;
	struct cvmx_nqm_vfx_cplx_base_addr_n_sz_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_49_63               : 15;
	uint64_t qsize                        : 14; /**< This value plus one defines the size of the (octeon) CPL queue queue in entries. The
                                                         minimum size of the queue is 2, so 1 should be the minimum programmed value for this
                                                         field. */
	uint64_t base_addr                    : 35; /**< Base address for the (internal/CNXXXX completion) CPL circular queue
                                                         in L2/DRAM memory. The beginning of a CPL queue must be naturally-aligned on
                                                         a 128-byte boundary. ([BASE_ADDR] << 7) is the OCTEON L2/DRAM
                                                         address of the beginning of the beginning of the CPL queue. */
#else
	uint64_t base_addr                    : 35;
	uint64_t qsize                        : 14;
	uint64_t reserved_49_63               : 15;
#endif
	} s;
	struct cvmx_nqm_vfx_cplx_base_addr_n_sz_s cn73xx;
};
typedef union cvmx_nqm_vfx_cplx_base_addr_n_sz cvmx_nqm_vfx_cplx_base_addr_n_sz_t;

/**
 * cvmx_nqm_vf#_cpl#_h
 *
 * NQM_VF(x)_CPL(0..16)_H are the CPL queue heads for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects
 * the queues and functions that are actually present in the hardware.
 * See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cplx_h {
	uint64_t u64;
	struct cvmx_nqm_vfx_cplx_h_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t head                         : 14; /**< CPL queue head pointer. Managed by hardware during normal operation.
                                                         To create a new or clear a CPL queue, [HEAD] should be written to zero
                                                         prior to initially enabling the CPL/CQ.
                                                         For debug and migration. */
#else
	uint64_t head                         : 14;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_nqm_vfx_cplx_h_s          cn73xx;
};
typedef union cvmx_nqm_vfx_cplx_h cvmx_nqm_vfx_cplx_h_t;

/**
 * cvmx_nqm_vf#_cpl#_ifc
 *
 * NQM_VF(x)_CPL(0..16)_IFC are the CPL queue in flight counters for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the queues
 * and functions that are actually present in the hardware. See NQM_VF_MODE_E.
 *
 * The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable for this CPL/CQ.
 * See the NQM_VF()_CQ()_ENA[ENABLE] description.
 */
union cvmx_nqm_vfx_cplx_ifc {
	uint64_t u64;
	struct cvmx_nqm_vfx_cplx_ifc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t count                        : 6;  /**< Returns the number of CPL completion reads in flight. Valid values
                                                         are 0 through 32. */
#else
	uint64_t count                        : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_nqm_vfx_cplx_ifc_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cplx_ifc cvmx_nqm_vfx_cplx_ifc_t;

/**
 * cvmx_nqm_vf#_cpl#_tdb
 *
 * NQM_VF(x)_CPL(0..16)_TDB contains the CPL queue tails for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the
 * queues and functions that are actually present in the hardware.
 * See NQM_VF_MODE_E.
 *
 * This CSR should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cplx_tdb {
	uint64_t u64;
	struct cvmx_nqm_vfx_cplx_tdb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t tail                         : 14; /**< CPL queue tail pointer. Managed by (i.e. written by) software during
                                                         normal operation.
                                                         To create a new or clear a CPL queue, [TAIL] should be written to zero
                                                         prior to initially enabling the CPL/CQ. */
#else
	uint64_t tail                         : 14;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_nqm_vfx_cplx_tdb_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cplx_tdb cvmx_nqm_vfx_cplx_tdb_t;

/**
 * cvmx_nqm_vf#_cq#_base
 *
 * NQM_VF(x)_CQ(1..16)_BASE are the IO CQ base addresses for function x.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 *
 * When configuring a new NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically-contiguous)
 * CQ, then software must clear this entire CSR before initially enabling
 * the CPL/CQ.
 *
 * When configuring a new NQM_VF()_CQ()_CC[PC]=1 (i.e. physically-contiguous) CQ,
 * then software must write this CSR to configure [PBA] prior to
 * initially enabling the CPL/CQ ([PRIP] and [BV] must be written to
 * zero during this write).
 */
union cvmx_nqm_vfx_cqx_base {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqx_base_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t pba                          : 52; /**< Page base address.  If NQM_VF()_CQ()_CC[PC]=1 (i.e. physically-contiguous)
                                                         then [PBA] is a 64-bit memory page aligned pointer (NQM_VF()_CC[MPS]
                                                         selects the page size) to the base of the physically contiguous
                                                         queue. Software should derive [PBA] from the PRP entry 1 (PRP1)
                                                         field (in CDW6+CDW7) of the create I/O completion queue
                                                         command in this case. (If CDW11.PC=0 in the create I/O completion
                                                         queue command, but NQM_VF()_CQ()_CC[PC]=1 due to small
                                                         NQM_VF()_CQ()_CC[QSIZE], then software must read the first
                                                         PRP Entry in the PRP List from host memory, and write it into [PBA].)
                                                         If NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically-contiguous)
                                                         then [PBA] is managed by hardware during normal
                                                         operation. [PBA] can contain a 64-bit memory page aligned
                                                         pointer (NQM_VF()_CC[MPS] selects the page size) to the base
                                                         of the current page being accessed in this case. */
	uint64_t reserved_2_11                : 10;
	uint64_t prip                         : 1;  /**< PRP list read in progress. Must always be written to zero by
                                                         software.
                                                         Managed by hardware during normal operation when
                                                         NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically contiguous).
                                                         When set, NQM hardware has an outstanding read to the PRP list.
                                                         When a PRP list read returns with an error, NQM clears [PRIP,BV]=0,
                                                         clears corresponding NQM_VF()_CQ()_ENA[ENABLE]=0, and sets
                                                         NQM_VF()_INT[CQ_FE] for the function.
                                                         [PRIP] should be clear more often than not, but if a
                                                         function-level reset or VF_ENABLE de-assertion occur during
                                                         a PRP list read, NQM hardware may leave [PRIP] continuously set.
                                                         The CQ will have been disabled (i.e. corresponding
                                                         NQM_VF()_CQ()_ENA[ENABLE] will have been cleared) by hardware
                                                         in this case, and software must clear [PRIP] before re-enabling
                                                         the CQ.
                                                         [PRIP] reads as zero when NQM_VF()_CQ()_CC[PC]=1
                                                         (i.e. physically contiguous). */
	uint64_t bv                           : 1;  /**< When set, [PBA] is valid.
                                                         Managed by hardware during normal operation when NQM_VF()_CQ()_CC[PC]=0
                                                         (i.e. not physically contiguous). [BV] is often the inverse
                                                         of [PRIP] in this case. But [PRIP,BV] may be 0,0 after initialization
                                                         before any completions have been sent and after a read error.
                                                         When NQM_VF()_CQ()_CC[PC]=1 (i.e.physically contiguous),
                                                         the [BV] read value is the corresponding NQM_VF()_CQ()_ENA[ENABLE].
                                                         [BV] should be written to one by software only on a migration when
                                                         NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically contiguous).
                                                         [BV] should be written to zero by software in all other circumstances. */
#else
	uint64_t bv                           : 1;
	uint64_t prip                         : 1;
	uint64_t reserved_2_11                : 10;
	uint64_t pba                          : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_base_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_base cvmx_nqm_vfx_cqx_base_t;

/**
 * cvmx_nqm_vf#_cq#_cc
 *
 * NQM_VF(x)_CQ(1..16)_CC are the IO CQ config and control CSR's for function x.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cqx_cc {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqx_cc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_28_63               : 36;
	uint64_t qsize                        : 12; /**< Queue size. This value plus one defines the size of the submission
                                                         queue in entries. The minimum legal size of 2 entries is [QSIZE]=1.
                                                         The maximum legal size of 4096 entries is [QSIZE]=0xFFF.
                                                         Software should derive [QSIZE] from the queue size field
                                                         (CDW10.QSIZE) of the create I/O Completion Queue command. */
	uint64_t reserved_9_15                : 7;
	uint64_t iv                           : 5;  /**< Interrupt vector.  Interrupt vector associated with this IO queue.
                                                         Valid values are 0..n (a.k.a. NQM_VF_INT_VEC_E::NQM_ADMIN_CQ or
                                                         NQM_VF_INT_VEC_E::NVM_CQ(1..n)), where n+1 is the number of
                                                         MSI-X vectors present in an NQM function. See the NQM_VF_INT_VEC_E
                                                         description.
                                                         If [IEN]=0, [IV] should be zeroed.
                                                         Software should derive [IV] from the interrupt vector field
                                                         and the interrupt enabled field (CDW11.IV and CDW11.IEN) of
                                                         the create I/O Completion Queue command. */
	uint64_t reserved_3_3                 : 1;
	uint64_t p                            : 1;  /**< Phase tag. Indicates the phase tag that NQM will
                                                         write with the next completion.
                                                         Software should set [P]=1 prior to first enabling a new
                                                         CPL/CQ.
                                                         [P] and corresponding NQM_VF()_CQ()_TAIL[PT] are one and the same.
                                                         A write to either [P] or corresponding NQM_VF()_CQ()_TAIL[PT] updates
                                                         both. */
	uint64_t ien                          : 1;  /**< Interrupt enable. Interrupts are enabled when asserted.
                                                         Software should derive [IV] from the interrupt enabled field
                                                         (CDW11.IEN) of the create I/O completion queue command. */
	uint64_t pc                           : 1;  /**< Physically contiguous. When set, indicates that the queue is
                                                         located in physically contiguous host memory pages. When clear,
                                                         indicates that the queue locations are identified by a PRP list.
                                                         [PC] must be set for an enabled queue when all queue entries
                                                         fit within a single page (NQM_VF()_CC[MPS] selects the page size).
                                                         Software should derive [PC] from the physically contiguous field
                                                         (CDW11.PC) and queue size field (CDW10.QSIZE) of the create I/O
                                                         completion queue command. [PC] should be set when CDW11.PC is set.
                                                         [PC] should also be set when CDW11.PC is clear, selecting a
                                                         non-contiguous queue, but [QSIZE] is small enough that all
                                                         entries fit within a single page. */
#else
	uint64_t pc                           : 1;
	uint64_t ien                          : 1;
	uint64_t p                            : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t iv                           : 5;
	uint64_t reserved_9_15                : 7;
	uint64_t qsize                        : 12;
	uint64_t reserved_28_63               : 36;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_cc_s          cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_cc cvmx_nqm_vfx_cqx_cc_t;

/**
 * cvmx_nqm_vf#_cq#_ena
 *
 * NQM_VF(x)_CQ(0..16)_ENA are the completion queue enable CSRs for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the queues
 * and functions that are actually present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_cqx_ena {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqx_ena_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t enable                       : 1;  /**< Enables completion movement from the OCTEON-internal CPL queue to the
                                                         remote host NVMe-standard completion queue.
                                                         This CPL/CQ is enabled when all of [ENABLE], corresponding NQM_VF()_CC[EN],
                                                         and corresponding NQM_VF()_CSTS[RDY] are set.
                                                         The following CSR arrays are related to the host completion queues (CQs):
                                                         <pre>
                                                          NQM_VF()_CQ()_HDBL
                                                          NQM_VF()_CQ()_TAIL
                                                          NQM_VF()_CQ()_PRP
                                                          NQM_VF()_CQ()_BASE
                                                          NQM_VF()_CQ()_CC
                                                          NQM_VF()_ACQ
                                                          NQM_VF()_AQA[ACQS]
                                                          NQM_VF()_ACQ_CC
                                                         </pre>
                                                         The following CSR arrays are related to the OCTEON-internal CPL queues:
                                                         <pre>
                                                          NQM_VF()_CPL()_TDB
                                                          NQM_VF()_CPL()_BASE_ADDR_N_SZ
                                                          NQM_VF()_CPL()_H
                                                          NQM_VF()_CPL()_IFC
                                                          NQM_VF()_SQ()_CREDIT
                                                         </pre>
                                                         The following CSR arrays are related to the the MSI-X interrupts that
                                                         NQM can create when it moves entries from CPL to CQ:
                                                         <pre>
                                                          NQM_VF()_VEC()_MSIX_ADDR
                                                          NQM_VF()_VEC()_MSIX_CTL
                                                          NQM_VF()_MSIX_PBA
                                                          NQM_VF()_VEC()_MSIX_CD
                                                          NQM_VF()_VEC()_MSIX_INT_ST
                                                          NQM_VF()_IC_THR
                                                          NQM_VF()_IC_TIME
                                                          NQM_VF()_MSIX_CONFIG
                                                         </pre>
                                                         When the CPL/CQ is enabled, NQM can move available completion queue
                                                         entries from the corresponding (OCTEON-internal) CPL queue to the
                                                         NVMe-defined admin or IO CQ on the remote host. When the CPL/CQ is
                                                         not enabled or there is no available space in the CQ on the remote host,
                                                         NQM will not initiate new movements.
                                                         When the effective enable for the CPL/CQ transitions 1->0, NQM is
                                                         still moving completion queue entries as long as corresponding
                                                         NQM_VF()_CPL()_IFC[COUNT]!=0. Once NQM_VF()_CPL()_IFC[COUNT] is
                                                         clear after the effective enable transitions 1->0:
                                                         *  NQM CPL/CQ hardware has stopped issuing all writes to the
                                                            host CQ.
                                                         *  After NQM_VF()_CQ()_BASE[PRIP] is also clear, NQM CPL/CQ hardware
                                                            also has no outstanding reads, so the CPL/CQ is idle except
                                                            possibly for MSI-X interrupts.
                                                         *  After the CPL/CQ's are disabled and idle, the MSI-X interrupt
                                                            state can be cleared/migrated. Refer to the NQM_VF()_MSIX_PBA
                                                            description.
                                                         When NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically-contiguous),
                                                         NQM hardware clears [ENABLE] when a PRP list entry read
                                                         returns an error. This would happen when the remote
                                                         host doesn't allow the function to read the entry, for
                                                         example. */
#else
	uint64_t enable                       : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_ena_s         cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_ena cvmx_nqm_vfx_cqx_ena_t;

/**
 * cvmx_nqm_vf#_cq#_hdbl
 *
 * NQM_VF(x)_CQ(y)_HDBL is the NVMe-standard completion queue y head
 * doorbell (CQyHDBL) for function x. Includes both admin and IO queues.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be migrated when the corresponding
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for the CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cqx_hdbl {
	uint32_t u32;
	struct cvmx_nqm_vfx_cqx_hdbl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t reserved_12_31               : 20;
	uint32_t cqh                          : 12; /**< Completion queue head. Indicates the new value of the completion queue
                                                         head entry pointer. This value shall overwrite any previous completion
                                                         queue head value provided. The difference between the last CQH write
                                                         and the current CQH entry pointer write indicates the number of
                                                         entries that are now available for re-use by the controller in the
                                                         completion queue. */
#else
	uint32_t cqh                          : 12;
	uint32_t reserved_12_31               : 20;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_hdbl_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_hdbl cvmx_nqm_vfx_cqx_hdbl_t;

/**
 * cvmx_nqm_vf#_cq#_prp
 *
 * NQM_VF(x)_CQ(1..16)_PRP are the IO CQ PRP list addresses for function x.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 *
 * When configuring a new NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically-contiguous)
 * CQ, software should write this CSR to configure [PRP] prior to
 * initially enabling the CPL/CQ.
 *
 * When configuring a new NQM_VF()_CQ()_CC[PC]=1 (i.e. physically-contiguous) CQ,
 * then software need not write this CSR.
 */
union cvmx_nqm_vfx_cqx_prp {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqx_prp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t prp                          : 52; /**< PRP list address.  If NQM_VF()_CQ()_CC[PC]=0 (i.e. not physically-contiguous)
                                                         then [PRP] is a 64-bit memory page aligned pointer (NQM_VF()_CC[MPS]
                                                         selects the page size) to the base of the (physically contiguous)
                                                         PRP list that constitutes the non-physically-contiguous queue.
                                                         Software should derive [PRP] from the PRP entry 1 (PRP1)
                                                         field (in CDW6+CDW7 of the create I/O completion queue
                                                         command) in this case.
                                                         If NQM_VF()_CQ()_CC[PC]=1 (i.e. physically-contiguous,
                                                         CDW11.PC=1) then [PRP] is not used by hardware. */
	uint64_t reserved_0_11                : 12;
#else
	uint64_t reserved_0_11                : 12;
	uint64_t prp                          : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_prp_s         cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_prp cvmx_nqm_vfx_cqx_prp_t;

/**
 * cvmx_nqm_vf#_cq#_tail
 *
 * NQM_VF(x)_CQ(0..16)_TAIL are the CQ tails for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the
 * queues and functions that are actually present in the hardware. See
 * NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * CPL/CQ is idle. The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable
 * for this CPL/CQ. NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions.
 */
union cvmx_nqm_vfx_cqx_tail {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqx_tail_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t pt                           : 1;  /**< Phase tag. Indicates the phase tag that NQM will
                                                         write with the next completion transferred to the CQ.
                                                         Software should set [PT]=1 prior to first enabling a new
                                                         CPL/CQ.
                                                         [PT] is one and the same as corresponding NQM_VF()_ACQ_CC[P]/
                                                         NQM_VF()_CQ()_CC[P]. A write to either [PT] or corresponding
                                                         NQM_VF()_ACQ_CC[P]/NQM_VF()_CQ()_CC[P] updates both. */
	uint64_t tail                         : 12; /**< CQ tail entry pointer. Managed by hardware during normal operation.
                                                         Indicates the position in the CQ on the remote host that NQM will
                                                         write next.
                                                         Software should clear [TAIL]=0 prior to first enabling
                                                         a new CPL/CQ. */
#else
	uint64_t tail                         : 12;
	uint64_t pt                           : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_nqm_vfx_cqx_tail_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cqx_tail cvmx_nqm_vfx_cqx_tail_t;

/**
 * cvmx_nqm_vf#_cqsm_dbg
 */
union cvmx_nqm_vfx_cqsm_dbg {
	uint64_t u64;
	struct cvmx_nqm_vfx_cqsm_dbg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_21_63               : 43;
	uint64_t cqar                         : 17; /**< CQ arbitration request vector.  For diagnostic use only. */
	uint64_t cqag                         : 4;  /**< CQ arbitration grant index.  For diagnostic use only. */
#else
	uint64_t cqag                         : 4;
	uint64_t cqar                         : 17;
	uint64_t reserved_21_63               : 43;
#endif
	} s;
	struct cvmx_nqm_vfx_cqsm_dbg_s        cn73xx;
};
typedef union cvmx_nqm_vfx_cqsm_dbg cvmx_nqm_vfx_cqsm_dbg_t;

/**
 * cvmx_nqm_vf#_csts
 *
 * NQM_VF(x)_CSTS is the NVMe-standard controller status (CSTS) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_csts {
	uint32_t u32;
	struct cvmx_nqm_vfx_csts_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t reserved_6_31                : 26;
	uint32_t pp                           : 1;  /**< Processing paused: Not supported. (First appeared in NVMe 1.2 specs.)
                                                         [PP] indicates whether the controller is processing commands.  If
                                                         [PP] is cleared to 0, then the controller is processing commands
                                                         normally. If [PP] is set to 1, then the controller has temporarily
                                                         stopped processing commands in order to handle an event (e.g.,
                                                         firmware activation). This bit is only valid when NQM_VF()_CC[EN]=1. */
	uint32_t nssro                        : 1;  /**< NVM subsystem reset is not supported. */
	uint32_t shst                         : 2;  /**< Shutdown status. Indicates the status of shutdown processing that is
                                                         initiated by the host setting NQM_VF()_CC[SHN].
                                                         The shutdown status values are defined as:
                                                         0x0 = Normal operation (no shutdown has been requested).
                                                         0x1 = Shutdown processing occurring.
                                                         0x2 = Shutdown processing complete.
                                                         0x3 = Reserved.
                                                         To start executing commands on the controller after a shutdown
                                                         operation (NQM_VF()_CSTS[SHST] set to 0x2), a controller reset
                                                         (NQM_VF()_CC[EN] cleared) is required. If host software submits
                                                         commands to the controller without issuing a controller reset,
                                                         the behavior is undefined.
                                                         This field is read-only from PCIe (SWI). */
	uint32_t cfs                          : 1;  /**< Controller fatal status. Set when a fatal controller error occurred
                                                         that could not be communicated in the appropriate completion queue.
                                                         Cleared when a fatal controller error has not occurred. The reset
                                                         value of this field is 1 when a fatal controller error is detected
                                                         during controller initialization.
                                                         This field is read-only from PCIe (SWI). */
	uint32_t rdy                          : 1;  /**< Ready. When set, the NVMe controller is ready to process commands
                                                         after the corresponding NQM_VF()_CC[EN] is set. [RDY] is a queue
                                                         enable for the SQ's and CPL/CQ's together with corresponding
                                                         NQM_VF()_CC[EN] and NQM_VF()_SQ()_ENA[ENABLE]/NQM_VF()_CQ()_ENA[ENABLE].
                                                         See the NQM_VF()_SQ()_ENA[ENABLE] and NQM_VF()_CQ()_ENA[ENABLE]
                                                         descriptions.
                                                         [RDY] is RO from PCIe, but internally writable.
                                                         Current NVMe specifications for [RDY]:
                                                         _ [RDY] is set to 1 when the controller is ready to accept Submission
                                                          Queue Tail doorbell writes after NQM_VF()_CC[EN] is set to 1. [RDY]
                                                          shall be cleared to 0 when NQM_VF()_CC[EN] is cleared to 0. Commands
                                                          shall not be submitted to the controller until [RDY] is set
                                                          to 1 after NQM_VF()_CC[EN] is set to 1. Failure to follow this
                                                          requirement produces undefined results. Host software shall wait
                                                          a minimum of NQM_VF()_CAP[TO] seconds for this field to be set
                                                          to 1 after setting NQM_VF()_CC[EN] to 1 from a previous value of 0. */
#else
	uint32_t rdy                          : 1;
	uint32_t cfs                          : 1;
	uint32_t shst                         : 2;
	uint32_t nssro                        : 1;
	uint32_t pp                           : 1;
	uint32_t reserved_6_31                : 26;
#endif
	} s;
	struct cvmx_nqm_vfx_csts_s            cn73xx;
};
typedef union cvmx_nqm_vfx_csts cvmx_nqm_vfx_csts_t;

/**
 * cvmx_nqm_vf#_ic_thr
 *
 * This register controls the interrupt coalescing threshold.
 *
 */
union cvmx_nqm_vfx_ic_thr {
	uint64_t u64;
	struct cvmx_nqm_vfx_ic_thr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t thr                          : 8;  /**< Coalescing threshold.  Specifies the desired minimum number of
                                                         completion queue entries to aggregate per interrupt vector before
                                                         signaling an interrupt to the host. This is a 0's based value.
                                                         [CTIME] should be derived from the Aggregation Threshold (CDW11.THR)
                                                         in a Interrupt Coalescing (Feature Identifier 0x08) Set Features
                                                         admin command. */
	uint64_t reserved_0_15                : 16;
#else
	uint64_t reserved_0_15                : 16;
	uint64_t thr                          : 8;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_nqm_vfx_ic_thr_s          cn73xx;
};
typedef union cvmx_nqm_vfx_ic_thr cvmx_nqm_vfx_ic_thr_t;

/**
 * cvmx_nqm_vf#_ic_time
 *
 * This register controls the interrupt coalescing time.
 *
 */
union cvmx_nqm_vfx_ic_time {
	uint64_t u64;
	struct cvmx_nqm_vfx_ic_time_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ctime                        : 8;  /**< Coalescing time.  Specifies the maximum time in 100 microsecond
                                                         increments that a controller may delay an interrupt due to interrupt
                                                         coalescing.  A value of 0x0 corresponds to no delay (i.e., disabling
                                                         this capability). The controller applies this time across all
                                                         interrupt vectors.
                                                         [CTIME] should be derived from the aggregation time (CDW11.TIME)
                                                         in a interrupt coalescing (feature identifier 0x08) set features
                                                         admin command. */
#else
	uint64_t ctime                        : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_nqm_vfx_ic_time_s         cn73xx;
};
typedef union cvmx_nqm_vfx_ic_time cvmx_nqm_vfx_ic_time_t;

/**
 * cvmx_nqm_vf#_int
 *
 * This register provides status and clears per-VF interrupts sources.  If a bit
 * in this register ANDed with NQM_VF()_INT_ENA_W1C is true then
 * NQM_INTSN_E::NQM_VF()_INT is thrown.
 */
union cvmx_nqm_vfx_int {
	uint64_t u64;
	struct cvmx_nqm_vfx_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t sli_err                      : 1;  /**< A DPI DMA or CPU read to the NQM (i.e. PEM0 PF2) function returned with
                                                         an error. This could be because of a bad pointer, FLR, or other reasons
                                                         related to the function. An FLR when there are inflight reads to the remote host
                                                         function can cause [SLI_ERR] to assert when the reads would normally
                                                         complete successfully. During an FLR, it is possible for [SLI_ERR] to
                                                         continually be set while DPI drains DMA instructions reading the
                                                         function that were queued before the FLR.
                                                         In the DPI DMA case where the instruction that caused [SLI_ERR] to
                                                         be set generates a completion status, DPI_CS_E::ERRRSP is the completion
                                                         status for the DPI DMA instruction.
                                                         When an FLR was not the [SLI_ERR] assertion cause, [SLI_ERR] assertion
                                                         should typically coincide with one or more completions with a data transfer
                                                         error (value 4) status code. When an FLR was the cause, NQM hardware
                                                         will have recently sent [FLR]. */
	uint64_t sq_fe                        : 1;  /**< Submission queue fatal error. NQM asserts [SQ_FE] when a PRP list entry
                                                         read from a SQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [SQ_FE] to assert for a
                                                         normally-operating SQ that is not idle when the FLR is initiated.
                                                         The SQ(s) causing [SQ_FE] to assert must have NQM_VF()_SQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_SQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_SQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [SQ_FE] assertion cause, [SQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t cq_fe                        : 1;  /**< Completion queue fatal error. NQM asserts [CQ_FE] when a PRP list entry
                                                         read from a CQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [CQ_FE] to assert for a
                                                         normally-operating CQ that is not idle when the FLR is initiated.
                                                         The CQ(s) causing [CQ_FE] to assert must have NQM_VF()_CQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_CQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_CQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [CQ_FE] assertion cause, [CQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t asq_cfg                      : 1;  /**< Administration submission queue configuration. Asserted when the host
                                                         changes the configuration of the admin SQ while it is enabled.
                                                         NQM_VF()_SQ()_ENA[ENABLE] describes admin SQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t acq_cfg                      : 1;  /**< Administration completion queue configuration. Asserted when the host
                                                         changes the configuration of the admin CQ while it is enabled.
                                                         NQM_VF()_CQ()_ENA[ENABLE] describes admin CPL/CQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t cq_db_val                    : 1;  /**< Completion queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the CQ, or is the
                                                         same as the previous written value or attempts to remove from an
                                                         empty CQ (underflow) while enabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t cq_db                        : 1;  /**< Completion queue doorbell. Asserted when host software writes the
                                                         doorbell of a CQ that is disabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t sq_db_val                    : 1;  /**< Submission queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the SQ, or is the
                                                         same as the previous written value or attempts to add to a full SQ
                                                         (overflow) while enabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t sq_db                        : 1;  /**< Submission queue doorbell. Asserted when host software writes the
                                                         doorbell of a SQ that is disabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t flr                          : 1;  /**< Function-level reset interrupt.  This bit is asserted when a PCIe
                                                         function-level reset to the function has been initiated. */
	uint64_t ccw                          : 1;  /**< Controller configuration write interrupt.  This bit is asserted anytime
                                                         the NQM_VF()_CC register is written via PCIe.  This can be used to
                                                         notify OCTEON software that the shutdown notification (SHN) and/or
                                                         the enable (EN) fields have changed. */
#else
	uint64_t ccw                          : 1;
	uint64_t flr                          : 1;
	uint64_t sq_db                        : 1;
	uint64_t sq_db_val                    : 1;
	uint64_t cq_db                        : 1;
	uint64_t cq_db_val                    : 1;
	uint64_t acq_cfg                      : 1;
	uint64_t asq_cfg                      : 1;
	uint64_t cq_fe                        : 1;
	uint64_t sq_fe                        : 1;
	uint64_t sli_err                      : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_nqm_vfx_int_s             cn73xx;
};
typedef union cvmx_nqm_vfx_int cvmx_nqm_vfx_int_t;

/**
 * cvmx_nqm_vf#_int_ena_w1c
 */
union cvmx_nqm_vfx_int_ena_w1c {
	uint64_t u64;
	struct cvmx_nqm_vfx_int_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t sli_err                      : 1;  /**< A DPI DMA or CPU read to the NQM (i.e. PEM0 PF2) function returned with
                                                         an error. This could be because of a bad pointer, FLR, or other reasons
                                                         related to the function. An FLR when there are inflight reads to the remote host
                                                         function can cause [SLI_ERR] to assert when the reads would normally
                                                         complete successfully. During an FLR, it is possible for [SLI_ERR] to
                                                         continually be set while DPI drains DMA instructions reading the
                                                         function that were queued before the FLR.
                                                         In the DPI DMA case where the instruction that caused [SLI_ERR] to
                                                         be set generates a completion status, DPI_CS_E::ERRRSP is the completion
                                                         status for the DPI DMA instruction.
                                                         When an FLR was not the [SLI_ERR] assertion cause, [SLI_ERR] assertion
                                                         should typically coincide with one or more completions with a data transfer
                                                         error (value 4) status code. When an FLR was the cause, NQM hardware
                                                         will have recently sent [FLR]. */
	uint64_t sq_fe                        : 1;  /**< Submission queue fatal error. NQM asserts [SQ_FE] when a PRP list entry
                                                         read from a SQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [SQ_FE] to assert for a
                                                         normally-operating SQ that is not idle when the FLR is initiated.
                                                         The SQ(s) causing [SQ_FE] to assert must have NQM_VF()_SQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_SQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_SQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [SQ_FE] assertion cause, [SQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t cq_fe                        : 1;  /**< Completion queue fatal error. NQM asserts [CQ_FE] when a PRP list entry
                                                         read from a CQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [CQ_FE] to assert for a
                                                         normally-operating CQ that is not idle when the FLR is initiated.
                                                         The CQ(s) causing [CQ_FE] to assert must have NQM_VF()_CQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_CQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_CQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [CQ_FE] assertion cause, [CQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t asq_cfg                      : 1;  /**< Administration submission queue configuration. Asserted when the host
                                                         changes the configuration of the admin SQ while it is enabled.
                                                         NQM_VF()_SQ()_ENA[ENABLE] describes admin SQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t acq_cfg                      : 1;  /**< Administration completion queue configuration. Asserted when the host
                                                         changes the configuration of the admin CQ while it is enabled.
                                                         NQM_VF()_CQ()_ENA[ENABLE] describes admin CPL/CQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t cq_db_val                    : 1;  /**< Completion queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the CQ, or is the
                                                         same as the previous written value or attempts to remove from an
                                                         empty CQ (underflow) while enabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t cq_db                        : 1;  /**< Completion queue doorbell. Asserted when host software writes the
                                                         doorbell of a CQ that is disabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t sq_db_val                    : 1;  /**< Submission queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the SQ, or is the
                                                         same as the previous written value or attempts to add to a full SQ
                                                         (overflow) while enabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t sq_db                        : 1;  /**< Submission queue doorbell. Asserted when host software writes the
                                                         doorbell of a SQ that is disabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t flr                          : 1;  /**< Function-level reset interrupt.  This bit is asserted when a PCIe
                                                         function-level reset to the function has been initiated. */
	uint64_t ccw                          : 1;  /**< Controller configuration write interrupt.  This bit is asserted anytime
                                                         the NQM_VF()_CC register is written via PCIe.  This can be used to
                                                         notify OCTEON software that the shutdown notification (SHN) and/or
                                                         the enable (EN) fields have changed. */
#else
	uint64_t ccw                          : 1;
	uint64_t flr                          : 1;
	uint64_t sq_db                        : 1;
	uint64_t sq_db_val                    : 1;
	uint64_t cq_db                        : 1;
	uint64_t cq_db_val                    : 1;
	uint64_t acq_cfg                      : 1;
	uint64_t asq_cfg                      : 1;
	uint64_t cq_fe                        : 1;
	uint64_t sq_fe                        : 1;
	uint64_t sli_err                      : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_nqm_vfx_int_ena_w1c_s     cn73xx;
};
typedef union cvmx_nqm_vfx_int_ena_w1c cvmx_nqm_vfx_int_ena_w1c_t;

/**
 * cvmx_nqm_vf#_int_ena_w1s
 */
union cvmx_nqm_vfx_int_ena_w1s {
	uint64_t u64;
	struct cvmx_nqm_vfx_int_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t sli_err                      : 1;  /**< A DPI DMA or CPU read to the NQM (i.e. PEM0 PF2) function returned with
                                                         an error. This could be because of a bad pointer, FLR, or other reasons
                                                         related to the function. An FLR when there are inflight reads to the remote host
                                                         function can cause [SLI_ERR] to assert when the reads would normally
                                                         complete successfully. During an FLR, it is possible for [SLI_ERR] to
                                                         continually be set while DPI drains DMA instructions reading the
                                                         function that were queued before the FLR.
                                                         In the DPI DMA case where the instruction that caused [SLI_ERR] to
                                                         be set generates a completion status, DPI_CS_E::ERRRSP is the completion
                                                         status for the DPI DMA instruction.
                                                         When an FLR was not the [SLI_ERR] assertion cause, [SLI_ERR] assertion
                                                         should typically coincide with one or more completions with a data transfer
                                                         error (value 4) status code. When an FLR was the cause, NQM hardware
                                                         will have recently sent [FLR]. */
	uint64_t sq_fe                        : 1;  /**< Submission queue fatal error. NQM asserts [SQ_FE] when a PRP list entry
                                                         read from a SQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [SQ_FE] to assert for a
                                                         normally-operating SQ that is not idle when the FLR is initiated.
                                                         The SQ(s) causing [SQ_FE] to assert must have NQM_VF()_SQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_SQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_SQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [SQ_FE] assertion cause, [SQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t cq_fe                        : 1;  /**< Completion queue fatal error. NQM asserts [CQ_FE] when a PRP list entry
                                                         read from a CQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [CQ_FE] to assert for a
                                                         normally-operating CQ that is not idle when the FLR is initiated.
                                                         The CQ(s) causing [CQ_FE] to assert must have NQM_VF()_CQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_CQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_CQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [CQ_FE] assertion cause, [CQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t asq_cfg                      : 1;  /**< Administration submission queue configuration. Asserted when the host
                                                         changes the configuration of the admin SQ while it is enabled.
                                                         NQM_VF()_SQ()_ENA[ENABLE] describes admin SQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t acq_cfg                      : 1;  /**< Administration completion queue configuration. Asserted when the host
                                                         changes the configuration of the admin CQ while it is enabled.
                                                         NQM_VF()_CQ()_ENA[ENABLE] describes admin CPL/CQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t cq_db_val                    : 1;  /**< Completion queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the CQ, or is the
                                                         same as the previous written value or attempts to remove from an
                                                         empty CQ (underflow) while enabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t cq_db                        : 1;  /**< Completion queue doorbell. Asserted when host software writes the
                                                         doorbell of a CQ that is disabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t sq_db_val                    : 1;  /**< Submission queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the SQ, or is the
                                                         same as the previous written value or attempts to add to a full SQ
                                                         (overflow) while enabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t sq_db                        : 1;  /**< Submission queue doorbell. Asserted when host software writes the
                                                         doorbell of a SQ that is disabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t flr                          : 1;  /**< Function-level reset interrupt.  This bit is asserted when a PCIe
                                                         function-level reset to the function has been initiated. */
	uint64_t ccw                          : 1;  /**< Controller configuration write interrupt.  This bit is asserted anytime
                                                         the NQM_VF()_CC register is written via PCIe.  This can be used to
                                                         notify OCTEON software that the shutdown notification (SHN) and/or
                                                         the enable (EN) fields have changed. */
#else
	uint64_t ccw                          : 1;
	uint64_t flr                          : 1;
	uint64_t sq_db                        : 1;
	uint64_t sq_db_val                    : 1;
	uint64_t cq_db                        : 1;
	uint64_t cq_db_val                    : 1;
	uint64_t acq_cfg                      : 1;
	uint64_t asq_cfg                      : 1;
	uint64_t cq_fe                        : 1;
	uint64_t sq_fe                        : 1;
	uint64_t sli_err                      : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_nqm_vfx_int_ena_w1s_s     cn73xx;
};
typedef union cvmx_nqm_vfx_int_ena_w1s cvmx_nqm_vfx_int_ena_w1s_t;

/**
 * cvmx_nqm_vf#_int_w1s
 */
union cvmx_nqm_vfx_int_w1s {
	uint64_t u64;
	struct cvmx_nqm_vfx_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t sli_err                      : 1;  /**< A DPI DMA or CPU read to the NQM (i.e. PEM0 PF2) function returned with
                                                         an error. This could be because of a bad pointer, FLR, or other reasons
                                                         related to the function. An FLR when there are inflight reads to the remote host
                                                         function can cause [SLI_ERR] to assert when the reads would normally
                                                         complete successfully. During an FLR, it is possible for [SLI_ERR] to
                                                         continually be set while DPI drains DMA instructions reading the
                                                         function that were queued before the FLR.
                                                         In the DPI DMA case where the instruction that caused [SLI_ERR] to
                                                         be set generates a completion status, DPI_CS_E::ERRRSP is the completion
                                                         status for the DPI DMA instruction.
                                                         When an FLR was not the [SLI_ERR] assertion cause, [SLI_ERR] assertion
                                                         should typically coincide with one or more completions with a data transfer
                                                         error (value 4) status code. When an FLR was the cause, NQM hardware
                                                         will have recently sent [FLR]. */
	uint64_t sq_fe                        : 1;  /**< Submission queue fatal error. NQM asserts [SQ_FE] when a PRP list entry
                                                         read from a SQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [SQ_FE] to assert for a
                                                         normally-operating SQ that is not idle when the FLR is initiated.
                                                         The SQ(s) causing [SQ_FE] to assert must have NQM_VF()_SQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_SQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_SQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [SQ_FE] assertion cause, [SQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t cq_fe                        : 1;  /**< Completion queue fatal error. NQM asserts [CQ_FE] when a PRP list entry
                                                         read from a CQ in the function completes with an error status.
                                                         This could be because of a bad PRP list pointer, FLR, or other reasons
                                                         related to the function. An FLR can cause [CQ_FE] to assert for a
                                                         normally-operating CQ that is not idle when the FLR is initiated.
                                                         The CQ(s) causing [CQ_FE] to assert must have NQM_VF()_CQ()_CC[PC]=0,
                                                         and NQM clears their NQM_VF()_CQ()_BASE[PRIP,BV]=0,0, and
                                                         NQM_VF()_CQ()_ENA[ENABLE]=0 when the error occurs.
                                                         When an FLR was not the [CQ_FE] assertion cause, [CQ_FE] assertion
                                                         should result in a controller fatal error reported back to the host
                                                         via NQM_VF()_CSTS[CFS]. When an FLR was the cause, NQM will have
                                                         set [FLR]. */
	uint64_t asq_cfg                      : 1;  /**< Administration submission queue configuration. Asserted when the host
                                                         changes the configuration of the admin SQ while it is enabled.
                                                         NQM_VF()_SQ()_ENA[ENABLE] describes admin SQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t acq_cfg                      : 1;  /**< Administration completion queue configuration. Asserted when the host
                                                         changes the configuration of the admin CQ while it is enabled.
                                                         NQM_VF()_CQ()_ENA[ENABLE] describes admin CPL/CQ enable conditions.
                                                         This error should typically result in a controller fatal error
                                                         reported back to the host via NQM_VF()_CSTS[CFS]. */
	uint64_t cq_db_val                    : 1;  /**< Completion queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the CQ, or is the
                                                         same as the previous written value or attempts to remove from an
                                                         empty CQ (underflow) while enabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t cq_db                        : 1;  /**< Completion queue doorbell. Asserted when host software writes the
                                                         doorbell of a CQ that is disabled. NQM_VF()_CQ()_ENA[ENABLE]
                                                         describes CPL/CQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t sq_db_val                    : 1;  /**< Submission queue doorbell value: Asserted when host software writes a
                                                         doorbell value via PCIe that exceeds the size of the SQ, or is the
                                                         same as the previous written value or attempts to add to a full SQ
                                                         (overflow) while enabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell write value (value 1) error status asynchronous event to the
                                                         host. */
	uint64_t sq_db                        : 1;  /**< Submission queue doorbell. Asserted when host software writes the
                                                         doorbell of a SQ that is disabled. NQM_VF()_SQ()_ENA[ENABLE] describes
                                                         SQ enable conditions.
                                                         This error should typically result in software posting an invalid
                                                         doorbell register (value 0) error status asynchronous event to the host. */
	uint64_t flr                          : 1;  /**< Function-level reset interrupt.  This bit is asserted when a PCIe
                                                         function-level reset to the function has been initiated. */
	uint64_t ccw                          : 1;  /**< Controller configuration write interrupt.  This bit is asserted anytime
                                                         the NQM_VF()_CC register is written via PCIe.  This can be used to
                                                         notify OCTEON software that the shutdown notification (SHN) and/or
                                                         the enable (EN) fields have changed. */
#else
	uint64_t ccw                          : 1;
	uint64_t flr                          : 1;
	uint64_t sq_db                        : 1;
	uint64_t sq_db_val                    : 1;
	uint64_t cq_db                        : 1;
	uint64_t cq_db_val                    : 1;
	uint64_t acq_cfg                      : 1;
	uint64_t asq_cfg                      : 1;
	uint64_t cq_fe                        : 1;
	uint64_t sq_fe                        : 1;
	uint64_t sli_err                      : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_nqm_vfx_int_w1s_s         cn73xx;
};
typedef union cvmx_nqm_vfx_int_w1s cvmx_nqm_vfx_int_w1s_t;

/**
 * cvmx_nqm_vf#_intmc
 *
 * NQM_VF(x)_INTMC is the NVMe-standard interrupt mask clear (INTMC) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_intmc {
	uint32_t u32;
	struct cvmx_nqm_vfx_intmc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t ivmc                         : 32; /**< Interrupt vector mask clear.  Not applicable since only MSI-X
                                                         interrupts are supported. */
#else
	uint32_t ivmc                         : 32;
#endif
	} s;
	struct cvmx_nqm_vfx_intmc_s           cn73xx;
};
typedef union cvmx_nqm_vfx_intmc cvmx_nqm_vfx_intmc_t;

/**
 * cvmx_nqm_vf#_intms
 *
 * NQM_VF(x)_INTMS is the NVMe-standard interrupt mask set (INTMS) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_intms {
	uint32_t u32;
	struct cvmx_nqm_vfx_intms_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t ivms                         : 32; /**< Interrupt vector mask set.  Not applicable since only MSI-X
                                                         interrupts are supported. */
#else
	uint32_t ivms                         : 32;
#endif
	} s;
	struct cvmx_nqm_vfx_intms_s           cn73xx;
};
typedef union cvmx_nqm_vfx_intms cvmx_nqm_vfx_intms_t;

/**
 * cvmx_nqm_vf#_msix_config
 *
 * This register can be used to read MSI-X configuration state of a function
 *
 */
union cvmx_nqm_vfx_msix_config {
	uint64_t u64;
	struct cvmx_nqm_vfx_msix_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t en                           : 1;  /**< MSI-X enable. If set, MSI-X is enabled. A RO copy of corresponding PCIEEP_CFG044[MSIXEN]. */
	uint64_t mask                         : 1;  /**< MSI-X function mask. If set, MSI-X interrupts for this function are masked. A RO
                                                         copy of corresponding PCIEEP_CFG044[FUNM]. */
#else
	uint64_t mask                         : 1;
	uint64_t en                           : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_nqm_vfx_msix_config_s     cn73xx;
};
typedef union cvmx_nqm_vfx_msix_config cvmx_nqm_vfx_msix_config_t;

/**
 * cvmx_nqm_vf#_msix_pba
 *
 * NQM_VF(x)_MSIX_PBA contains the PCI-standard MSI-X PBA structure for
 * function x.
 *
 * When migrating a function, the current MSI-X state of the function can
 * be captured via the following sequence:
 *
 * (1) Quiet remote host activity for the function. At this point,
 *     all corresponding NQM_VF()_CQ()_HDBL should be stable in
 *     the function.
 *
 * (2) Save a copy and reset all of the NQM_VF()_VEC()_MSIX_CTL's
 *     in the function.
 *
 *     After this step, NQM stops sending MSIs for the function
 *     to the remote host because all the
 *     NQM_VF()_VEC()_MSIX_CTL[MASK]'s are set.
 *
 * (3) Save a copy of all the NQM_VF()_VEC()_MSIX_ADDR's and
 *     reset them.
 *
 * (4) Read a copy of all corresponding NQM_VF()_CQ()_TAIL in the
 *     function.  Let initTAIL[cq] = the NQM_VF()_CQ(cq)_TAIL[TAIL]
 *     value read.
 *
 * (5) Delay long enough to ensure that any NQM_VF()_CQ()_TAIL changes are
 *     reflected in the NQM_VF()_VEC()_MSIX_INT_ST's read in the next step.
 *
 *     A simple method is to wait for each NQM_VF(0..1027)_CPL(0..16)_IFC[COUNT]
 *     in the function to be zero at least once (independently) before continuing.
 *
 * (6) Read a copy of all corresponding NQM_VF()_VEC()_MSIX_INT_ST.
 *     let initINTCNTi be the NQM_VF()_VEC(i)_MSIX_INT_ST[INTCNT] value
 *     read.
 *
 * (7) Read a copy of corresponding NQM_VF()_MSIX_PBA. Let
 *     PBA = the NQM_VF()_MSIX_PBA[PEND] value read.
 *
 * (8) Ensure that all CPL/CQ's in the function are both disabled and idle. See
 *     the NQM_VF()_CQ()_ENA[ENABLE] description.
 *
 *     The CQ tails should be stable after this step.
 *
 *     Corresponding NQM_VF()_MSIX_PBA and all
 *     NQM_VF()_VEC()_MSIX_INT_ST[INTCNT] in the function
 *     should be zero after this step.
 *
 * (8) For each MSI-X vector used (i), do this:
 *
 *      a_cq_tail_changed = 0;
 *      for each cq
 *        if((cq was enabled before this sequence) and
 *           (cq had interrupts enabled before this sequence) and
 *           (cq used MSI-X vector i before this sequence) and
 *           (initTAIL[cq] does not equal the migrated NQM_VF(0..1027)_CQ(cq)_TAIL[TAIL])) then
 *                a_cq_tail_changed = 1
 *        endif
 *      end for
 *      if(a_cq_tail_changed or (initINTCNT[i] is not zero)) then
 *            PBA<i> = 1
 *      endif
 *
 * (9) Migrate the resultant PBA value together eith the MSIX_ADDR and
 *     MSIX_CTL values captured in (2) and (3) above. PBA
 *     will be written to NQM_VF()_MSIX_PBA[PEND] when the function is
 *     restored.
 *
 *     For MSI-X vectors that are used, the corresponding PBA bit
 *     will be set at this point unless an MSI had already been sent since
 *     the CQ's attached to the interrupt vector became non-empty.
 *     When PBA gets written to NQM_VF()_MSIX_PBA[PEND] during the
 *     restore, the hardware will not allow a PEND bit
 *     to be written to a one unless there is a non-empty CQ
 *     attached to the MSI-X vector.
 *
 * (10) Make a copy and reset corresponding PCIEEP*_CFG044[MSIXEN,FUNM].
 *
 * When restoring a migrated function, the following sequence can
 * restore the MSI-X state:
 *
 * (1) Restore the saved PCIEEP*_CFG044[MSIXEN,FUNM].
 *
 * (2) Restore the saved NQM_VF()_VEC()_MSIX_ADDR's.
 *
 * (3) Restore all other state of the function, enabling all SQ's and
 *     CQ's.
 *
 * (4) Write the saved PBA value to NQM_VF()_MSIX_PBA[PEND].
 *
 *     See the comment in (9) above.
 *
 * (5) After either the remote host is ready to receive MSIs from the
 *     function, or all saved NQM_VF()_VEC()_MSIX_CTL[MASK]'s are
 *     clear, or saved PCIEEP*_CFG044[MSIXEN] is clear, or
 *     saved PCIEEP*_CFG044[FUNM] is set:
 *
 *     Restore all NQM_VF()_VEC()_MSIX_CTL's.
 */
union cvmx_nqm_vfx_msix_pba {
	uint64_t u64;
	struct cvmx_nqm_vfx_msix_pba_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t pend                         : 17; /**< When [PEND<i>] is set, there is a pending message to be sent for the
                                                         MSI-X table entry that is NQM_VF()_VEC(i)_MSIX_ADDR and
                                                         NQM_VF()_VEC(i)_MSIX_CTL. NQM_VF_MODE[VF_MODE] selects the MSI-X
                                                         table entries and functions that are actually present in the hardware.
                                                         There is one [PEND] bit per MSI-X table entry. [PEND] bits whose corresponding
                                                         MSI-X table entry don't exist are RAZ. The NQM_VF_INT_VEC_E enumeration
                                                         describes the individual table entries and [PEND] bits of each function.
                                                         See NQM_VF_INT_VEC_E and NQM_VF_MODE_E.
                                                         This field is read-only when accessed from an NQM BAR0 (as per PCI MSI-X
                                                         standards), writeable internally. Unless there is one or more enabled and
                                                         non-empty CQ that is (1) enabled for interrupts and (2) mapped to the
                                                         MSI-X table entry, NQM forces the [PEND] bit to zero, and the [PEND]
                                                         bit cannot be written to a one. The NQM_VF()_CQ()_ENA[ENABLE] documentation
                                                         describes when a CQ is enabled. */
#else
	uint64_t pend                         : 17;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_nqm_vfx_msix_pba_s        cn73xx;
};
typedef union cvmx_nqm_vfx_msix_pba cvmx_nqm_vfx_msix_pba_t;

/**
 * cvmx_nqm_vf#_nssr
 *
 * NQM_VF(x)_NSSR is the NVMe-standard NVM subsystem reset (NSSR) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_nssr {
	uint32_t u32;
	struct cvmx_nqm_vfx_nssr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t nssrc                        : 32; /**< NVM subsystem reset is not supported. */
#else
	uint32_t nssrc                        : 32;
#endif
	} s;
	struct cvmx_nqm_vfx_nssr_s            cn73xx;
};
typedef union cvmx_nqm_vfx_nssr cvmx_nqm_vfx_nssr_t;

/**
 * cvmx_nqm_vf#_sq#_base
 *
 * NQM_VF(x)_SQ(1..16)_BASE are the IO SQ base addresses for function x.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * SQ is idle. The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable
 * for this SQ. NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 *
 * When configuring a new NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically-contiguous)
 * SQ, then software must clear this entire CSR before initially enabling
 * the SQ.
 *
 * When configuring a new NQM_VF()_SQ()_CC[PC]=1 (i.e. physically-contiguous) SQ,
 * then software must write this CSR to configure [PBA] prior to
 * initially enabling the SQ ([PRIP] and [BV] must be written to
 * zero during this write).
 */
union cvmx_nqm_vfx_sqx_base {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_base_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t pba                          : 52; /**< Page base address.  If NQM_VF()_SQ()_CC[PC]=1 (i.e. physically-contiguous)
                                                         then [PBA] is a 64-bit memory page aligned pointer (NQM_VF()_CC[MPS]
                                                         selects the page size) to the base of the physically contiguous
                                                         queue. Software should derive [PBA] from the PRP entry 1 (PRP1)
                                                         field (in CDW6+CDW7) of the create I/O submission queue
                                                         command in this case. (If CDW11.PC=0 in the create I/O submission
                                                         queue command, but NQM_VF()_SQ()_CC[PC]=1 due to small
                                                         NQM_VF()_SQ()_CC[QSIZE], then software must read the first
                                                         PRP Entry in the PRP List from host memory, and write it into [PBA].)
                                                         If NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically-contiguous)
                                                         then [PBA] is managed by hardware during normal
                                                         operation. [PBA] can contain a 64-bit memory page aligned
                                                         pointer (NQM_VF()_CC[MPS] selects the page size) to the base
                                                         of the current page being accessed in this case. */
	uint64_t reserved_2_11                : 10;
	uint64_t prip                         : 1;  /**< PRP list read in progress. Must always be written to zero by
                                                         software.
                                                         Managed by hardware during normal operation when
                                                         NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically contiguous).
                                                         When [PRIP] is set, NQM hardware has an outstanding read to
                                                         the PRP list. When a PRP list read returns an error,
                                                         NQM clears [PRIP,BV]=0,0, clears corresponding
                                                         NQM_VF()_SQ()_ENA[ENABLE]=0, and sets NQM_VF()_INT[SQ_FE]
                                                         for the function.
                                                         [PRIP] reads as zero when NQM_VF()_SQ()_CC[PC]=1
                                                         (i.e. physically contiguous). */
	uint64_t bv                           : 1;  /**< When set, [PBA] is valid.
                                                         Managed by hardware during normal operation when NQM_VF()_SQ()_CC[PC]=0
                                                         (i.e. not physically contiguous). [BV] is often the inverse
                                                         of [PRIP] in this case. But [PRIP,BV] may be 0,0 after initialization
                                                         before any completions have been sent and after a read error.
                                                         When NQM_VF()_SQ()_CC[PC]=1 (i.e.physically contiguous),
                                                         the [BV] read value is the corresponding NQM_VF()_SQ()_ENA[ENABLE].
                                                         [BV] should be written to one by software only on a migration when
                                                         NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically contiguous).
                                                         [BV] should be written to zero by software in all other circumstances. */
#else
	uint64_t bv                           : 1;
	uint64_t prip                         : 1;
	uint64_t reserved_2_11                : 10;
	uint64_t pba                          : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_base_s        cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_base cvmx_nqm_vfx_sqx_base_t;

/**
 * cvmx_nqm_vf#_sq#_cc
 *
 * NQM_VF(x)_SQ(1..16)_CC are the IO submission queue control registers
 * for function x. NQM_VF_MODE[VF_MODE] selects the queues and functions
 * that are actually present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when the SQ is idle.
 * The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable for the SQ.
 * NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_sqx_cc {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_cc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_28_63               : 36;
	uint64_t qsize                        : 12; /**< Queue size. This value plus one defines the size of the submission
                                                         queue in entries. The minimum legal size of 2 entries is configured
                                                         when QSIZE is set to 1.  The maximum size of 4096 entries is
                                                         configured when QSIZE is set to 0xFFF.
                                                         Software should derive [QSIZE] from the queue size field
                                                         (CDW10.QSIZE) of the create I/O submission queue command. */
	uint64_t reserved_1_15                : 15;
	uint64_t pc                           : 1;  /**< Physically contiguous. When set, indicates that the queue is
                                                         located in physically contiguous host memory pages. When clear,
                                                         indicates that the queue locations are identified by a PRP list.
                                                         [PC] must be set for an enabled queue when all queue entries
                                                         fit within a single page (NQM_VF()_CC[MPS] selects the page size).
                                                         Software should derive [PC] from the physically contiguous field
                                                         (CDW11.PC) and the queue size field (CDW10.QSIZE) of the create
                                                         I/O submission queue command. [PC] should be set when CDW11.PC
                                                         is set. [PC] should also be set when CDW11.PC is clear, selecting
                                                         a non-contiguous queue, but [QSIZE] is small enough that all
                                                         entries fit within a single page. */
#else
	uint64_t pc                           : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t qsize                        : 12;
	uint64_t reserved_28_63               : 36;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_cc_s          cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_cc cvmx_nqm_vfx_sqx_cc_t;

/**
 * cvmx_nqm_vf#_sq#_credit
 *
 * NQM_VF(x)_SQ(0..16)_CREDIT are the submission queue credits for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the queues
 * and functions that are actually present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when both the corresponding
 * CPL/CQ and the SQ are idle. There should only be one corresponding CPL/CQ -
 * selected by the Completion Queue Identifier (CQID) specified in the create I/O
 * submission queue command for an I/O SQ, or CPL/CQ zero for an admin SQ.
 * The corresponding NQM_VF()_CQ()_ENA[ENABLE] is an enable for the CPL/CQ.
 * NQM_VF()_CQ()_ENA[ENABLE] describes CPL/CQ idle conditions. The corresponding
 * NQM_VF()_SQ()_ENA[ENABLE] is an enable for the the SQ. NQM_VF()_SQ()_ENA[ENABLE]
 * describes SQ idle conditions.
 */
union cvmx_nqm_vfx_sqx_credit {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_credit_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t cred                         : 12; /**< WQE credit. Flow control credit counter that limits the number
                                                         of commands and completion in-flight to the NVMe controller
                                                         software executing on the CNXXXX CPUs. [CRED] programming
                                                         limits NQM_WQE_S and NQM_CPL_ENTRY_S storage requirements
                                                         on the OCTEON.
                                                         Prior to enabling a new SQ, software should set [CRED] less
                                                         than or equal to the maximum number of inflight NQM_WQE_S's
                                                         allowed from this SQ, and less than or equal to the maximum
                                                         number of NQM_CPL_ENTRY_S's from this SQ that can reside in the
                                                         corresponding CPL queue.
                                                         NQM hardware decrements [CRED] when it creates a WQE from a command
                                                         in the SQ.
                                                         NQM hardware increments [CRED] when it moves a CPL completion
                                                         queue entry to its CQ and NQM_CPL_ENTRY_S[SQID] selects this
                                                         SQ.
                                                         NQM will not move a new command from the SQ into a WQE when [CRED]
                                                         is zero.
                                                         Note that with an admin SQ/CPL/CQ, asynchronous event requests
                                                         will normally remain outstanding for long time periods and
                                                         will consume [CRED]'s while they do. The initially-configured
                                                         admin SQ [CRED] must exceed the asynchronous event request limit
                                                         (AERL) specified in the identify controller data structure provided
                                                         in response to the identify admin command. */
#else
	uint64_t cred                         : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_credit_s      cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_credit cvmx_nqm_vfx_sqx_credit_t;

/**
 * cvmx_nqm_vf#_sq#_ena
 *
 * NQM_VF(x)_SQ(0..16)_ENA are the submission queue enable CSRs for function x.
 * Includes both admin and IO queues. NQM_VF_MODE[VF_MODE] selects the queues
 * and functions that are actually present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_sqx_ena {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_ena_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t enable                       : 1;  /**< Enables entry movement from the remote host NVMe-standard SQ to
                                                         an OCTEON SSO WQE.
                                                         This SQ is enabled when all of [ENABLE], corresponding NQM_VF()_CC[EN],
                                                         and corresponding NQM_VF()_CSTS[RDY] are set.
                                                         The following CSR arrays are related to the submission queues:
                                                         <pre>
                                                          NQM_VF()_SQ()_TDBL
                                                          NQM_VF()_SQ()_CREDIT
                                                          NQM_VF()_SQ()_HEAD
                                                          NQM_VF()_SQ()_IFC
                                                          NQM_VF()_SQ()_CC
                                                          NQM_VF()_SQ()_PRP
                                                          NQM_VF()_SQ()_BASE
                                                          NQM_VF()_SQ()_SSO_SETUP
                                                          NQM_VF()_ASQ
                                                          NQM_VF()_AQA[ASQS]
                                                         </pre>
                                                         When this SQ is enabled, NQM can move available submission queue
                                                         entries from the NVMe-defined admin or IO SQ on the remote host
                                                         to an OCTEON-internal SSO entry. When this SQ is not enabled
                                                         or no credits are available, NQM will not initiate new movements
                                                         on this SQ. (The corresponding NQM_VF()_SQ()_CREDIT[CRED] is the
                                                         credit.)
                                                         When the effective SQ enable transitions 1->0, NQM is still moving
                                                         submission queue entries as long as corresponding
                                                         NQM_VF()_SQ()_IFC[COUNT]!=0. Once NQM_VF()_SQ()_IFC[COUNT] is clear
                                                         and NQM_VF()_SQ()_BASE[PRIP] is also clear after the effective
                                                         enable transitions 1->0, the SQ is idle. SSO will stop receiving
                                                         WQE adds from this SQ soon after the SQ becomes idle.
                                                         NQM hardware clears [ENABLE] when an SQ entry or PRP list
                                                         entry read returns an error. This would happen when the remote
                                                         host doesn't allow the function to read the entry, for
                                                         example. */
#else
	uint64_t enable                       : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_ena_s         cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_ena cvmx_nqm_vfx_sqx_ena_t;

/**
 * cvmx_nqm_vf#_sq#_head
 *
 * NQM_VF(x)_SQ(0..16)_HEAD are the submission queue head pointers (and associated
 * wrap counters) for function x. Includes both admin and IO queues.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and only be migrated when the SQ is idle.
 * The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable for the SQ.
 * NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_sqx_head {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_head_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t head                         : 13; /**< This field contains a combination of a the SQ head pointer and a
                                                         wrap count. Managed by NQM hardware during normal operation. The
                                                         head pointer identifies the next entry to be read out of the SQ
                                                         in host memory.  Whenever the head pointer is used, it
                                                         is either incremented by 1 or wrapped back to zero when it is equal to
                                                         the size of the SQ.  The wrap count increments each time time the SQ
                                                         head pointer wrap and it can be used to disambiguate head pointers
                                                         when the SQ is small in order to simplify fused command processing.
                                                         The wrap counter = HEAD<11:n> while the SQ head pointer = HEAD<n-1:0>
                                                         where n is the number of bits required to specify the SQ size
                                                         n = ROUND_UP( LOG_BASE_2( SQ_SIZE ) ).
                                                         When configuring a new SQ, then software should clear [HEAD]
                                                         before initially enabling the SQ. */
#else
	uint64_t head                         : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_head_s        cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_head cvmx_nqm_vfx_sqx_head_t;

/**
 * cvmx_nqm_vf#_sq#_ifc
 *
 * NQM_VF(x)_SQ(0..16)_IFC are the submission queue inflight counts
 * for function x. Includes both admin and IO queues. NQM_VF_MODE[VF_MODE]
 * selects the queues and functions that are actually present in the
 * hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_sqx_ifc {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_ifc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t count                        : 6;  /**< Returns the number of SQ command reads in flight. Valid values
                                                         are 0 through 32. */
#else
	uint64_t count                        : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_ifc_s         cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_ifc cvmx_nqm_vfx_sqx_ifc_t;

/**
 * cvmx_nqm_vf#_sq#_prp
 *
 * NQM_VF(x)_SQ(1..16)_PRP are the IO SQ PRP list addresses for function x.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be written and should only be migrated when this
 * SQ is idle. The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable
 * for this SQ. NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 *
 * When configuring a new NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically-contiguous)
 * SQ, software should write this CSR to configure [PRP] prior to
 * initially enabling the SQ.
 *
 * When configuring a new NQM_VF()_SQ()_CC[PC]=1 (i.e. physically-contiguous) SQ,
 * then software need not write this CSR.
 */
union cvmx_nqm_vfx_sqx_prp {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_prp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t prp                          : 52; /**< PRP list address.  If NQM_VF()_SQ()_CC[PC]=0 (i.e. not physically-contiguous)
                                                         then [PRP] is a 64-bit memory page aligned pointer (NQM_VF()_CC[MPS]
                                                         selects the page size) to the base of the (physically contiguous)
                                                         PRP list that constitutes the non-physically-contiguous queue.
                                                         Software should derive [PRP] from the PRP entry 1 (PRP1)
                                                         field (in CDW6+CDW7 of the create I/O submission queue
                                                         command) in this case.
                                                         If NQM_VF()_SQ()_CC[PC]=1 (i.e. physically-contiguous)
                                                         then [PRP] is not used by hardware. */
	uint64_t reserved_0_11                : 12;
#else
	uint64_t reserved_0_11                : 12;
	uint64_t prp                          : 52;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_prp_s         cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_prp cvmx_nqm_vfx_sqx_prp_t;

/**
 * cvmx_nqm_vf#_sq#_sso_setup
 *
 * NQM_VF(x)_SQ(0..16)_CC are the submission queue control registers
 * for function x. Includes both admin and IO queues. NQM_VF_MODE[VF_MODE]
 * selects the queues and functions that are actually present in the hardware.
 * See NQM_VF_MODE_E.
 *
 * This CSR should only be written when the SQ is idle.
 * The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable for the SQ.
 * NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_sqx_sso_setup {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqx_sso_setup_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t tag_type                     : 2;  /**< SSO tag type used for all WQE's created from this SQ. Enumerated by
                                                         SSO_TT_E. NQM_TAG_MODE_E describes how NQM generates the associated
                                                         SSO tag. */
	uint64_t reserved_6_45                : 40;
	uint64_t group                        : 6;  /**< SSO group used for all WQE's created from this SQ. */
#else
	uint64_t group                        : 6;
	uint64_t reserved_6_45                : 40;
	uint64_t tag_type                     : 2;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_sso_setup_s   cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_sso_setup cvmx_nqm_vfx_sqx_sso_setup_t;

/**
 * cvmx_nqm_vf#_sq#_tdbl
 *
 * NQM_VF(x)_SQ(y)_TDBL is the NVMe-standard submission queue y tail
 * doorbell (SQyTDBL) for function x. Includes both admin and IO queues.
 * NQM_VF_MODE[VF_MODE] selects the queues and functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 *
 * This CSR should only be migrated when the corresponding
 * SQ is idle. The corresponding NQM_VF()_SQ()_ENA[ENABLE] is an enable
 * for the SQ. NQM_VF()_SQ()_ENA[ENABLE] describes SQ idle conditions.
 */
union cvmx_nqm_vfx_sqx_tdbl {
	uint32_t u32;
	struct cvmx_nqm_vfx_sqx_tdbl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t reserved_12_31               : 20;
	uint32_t sqt                          : 12; /**< Submission queue tail. Indicates the new value of the submission queue
                                                         tail entry pointer. This value shall overwrite any previous submission
                                                         queue tail entry pointer value provided. The difference between the
                                                         last SQT write and the current SQT write indicates the number of
                                                         commands added to the submission queue. */
#else
	uint32_t sqt                          : 12;
	uint32_t reserved_12_31               : 20;
#endif
	} s;
	struct cvmx_nqm_vfx_sqx_tdbl_s        cn73xx;
};
typedef union cvmx_nqm_vfx_sqx_tdbl cvmx_nqm_vfx_sqx_tdbl_t;

/**
 * cvmx_nqm_vf#_sqsm_dbg
 */
union cvmx_nqm_vfx_sqsm_dbg {
	uint64_t u64;
	struct cvmx_nqm_vfx_sqsm_dbg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t sqar                         : 16; /**< SQ arbitration request vector.  For diagnostic use only. */
	uint64_t sqag                         : 4;  /**< SQ arbitration grant index.  For diagnostic use only. */
#else
	uint64_t sqag                         : 4;
	uint64_t sqar                         : 16;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_nqm_vfx_sqsm_dbg_s        cn73xx;
};
typedef union cvmx_nqm_vfx_sqsm_dbg cvmx_nqm_vfx_sqsm_dbg_t;

/**
 * cvmx_nqm_vf#_vec#_msix_addr
 *
 * NQM_VF(x)_VEC(0..16)_MSIX_ADDR contains the address portion of the
 * PCI-standard MSI-X table for function x.
 * NQM_VF_MODE[VF_MODE] selects the MSI-X table entries
 * that are actually present in the hardware. The
 * NQM_VF_INT_VEC_E enumeration describes the table entries available
 * in each function.  See NQM_VF_INT_VEC_E and NQM_VF_MODE_E.
 *
 * For how to migrate these CSRs, see the NQM_VF()_MSIX_PBA description.
 */
union cvmx_nqm_vfx_vecx_msix_addr {
	uint64_t u64;
	struct cvmx_nqm_vfx_vecx_msix_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t addr                         : 62; /**< Address to use for MSI-X delivery of this vector. */
	uint64_t reserved_0_1                 : 2;
#else
	uint64_t reserved_0_1                 : 2;
	uint64_t addr                         : 62;
#endif
	} s;
	struct cvmx_nqm_vfx_vecx_msix_addr_s  cn73xx;
};
typedef union cvmx_nqm_vfx_vecx_msix_addr cvmx_nqm_vfx_vecx_msix_addr_t;

/**
 * cvmx_nqm_vf#_vec#_msix_cd
 *
 * NQM_VF(0..1027)_VEC(0..16)_MSIX_CD contains the vector coalescing disables
 * for function x. NQM_VF_MODE[VF_MODE] selects the MSI-X table entries that are
 * actually present in the hardware. The NQM_VF_INT_VEC_E enumeration describes
 * the table entries available in each function.  See NQM_VF_INT_VEC_E and
 * NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_vecx_msix_cd {
	uint64_t u64;
	struct cvmx_nqm_vfx_vecx_msix_cd_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t cd                           : 1;  /**< Coalescing disable. When set, disables all interrupt aggregation
                                                         for this MSI-X interrupt vector.
                                                         [CD] should be derived from the coalescing disable (CDW11.CD)
                                                         in a interrupt vector Configuration (feature identifier 0x09)
                                                         Set features admin command.
                                                         See also NQM_CPL_ENTRY_S[CD]. */
#else
	uint64_t cd                           : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_vfx_vecx_msix_cd_s    cn73xx;
};
typedef union cvmx_nqm_vfx_vecx_msix_cd cvmx_nqm_vfx_vecx_msix_cd_t;

/**
 * cvmx_nqm_vf#_vec#_msix_ctl
 *
 * NQM_VF(x)_VEC(0..16)_MSIX_CTL contains the data and message
 * control portion of the PCI-standard MSI-X table for function x.
 * NQM_VF_MODE[VF_MODE] selects the MSI-X table entries that are
 * actually present in the hardware. The NQM_VF_INT_VEC_E enumeration
 * describes the table entries available in each function.
 * See NQM_VF_INT_VEC_E and NQM_VF_MODE_E.
 *
 * For how to migrate these CSRs, see the NQM_VF()_MSIX_PBA description.
 */
union cvmx_nqm_vfx_vecx_msix_ctl {
	uint64_t u64;
	struct cvmx_nqm_vfx_vecx_msix_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t mask                         : 1;  /**< When set, no MSI-X interrupts will be sent to this vector.
                                                         NQM clears all NQM_VF(0..1027)_VEC(0..16)_MSIX_CTL[MASK]'s in the
                                                         function on an FLR. NQM clears all NQM_VF(0..1027)_VEC(0..16)_MSIX_CTL[MASK]'s
                                                         in all functions on a PF FLR. NQM clears all
                                                         NQM_VF(1..1027)_VEC(0..16)_MSIX_CTL[MASK]'s on a VF_ENABLE de-assertion.
                                                         (NQM does not modify the NQM_VF(0)_VEC(0..16)_MSIX_CTL[MASK]'s on
                                                         a VF_ENABLE de-assertion.) */
	uint64_t data                         : 32; /**< Data to use for MSI-X delivery of this vector. */
#else
	uint64_t data                         : 32;
	uint64_t mask                         : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_nqm_vfx_vecx_msix_ctl_s   cn73xx;
};
typedef union cvmx_nqm_vfx_vecx_msix_ctl cvmx_nqm_vfx_vecx_msix_ctl_t;

/**
 * cvmx_nqm_vf#_vec#_msix_int_flush
 */
union cvmx_nqm_vfx_vecx_msix_int_flush {
	uint64_t u64;
	struct cvmx_nqm_vfx_vecx_msix_int_flush_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t f                            : 1;  /**< Writing this bit will force the interrupts out for the IV to go out on the
                                                         next time around the time-wheel. (Remember a PBA interrupt depends on
                                                         mask and enable to get out.) Reading this bit will show 1 if interrupt count is > 0.
                                                         This flush is hardware dependent; it may take 1 iteration around the timewheel. */
#else
	uint64_t f                            : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_nqm_vfx_vecx_msix_int_flush_s cn73xx;
};
typedef union cvmx_nqm_vfx_vecx_msix_int_flush cvmx_nqm_vfx_vecx_msix_int_flush_t;

/**
 * cvmx_nqm_vf#_vec#_msix_int_st
 *
 * NQM_VF(x)_VEC(0..16)_MSIX_INT_ST contains MSI-X vector coalescing
 * internal state for function x. NQM_VF_MODE[VF_MODE] selects the
 * MSI-X table entries that are actually present in the hardware.
 * The NQM_VF_INT_VEC_E enumeration describes the table entries available
 * in each function. See NQM_VF_INT_VEC_E and NQM_VF_MODE_E.
 *
 * This CSR can be freely read. It should only ever be written in
 * migration cases, and only ever written with zero. Refer to
 * the migration sequence in the NQM_VF()_MSIX_PBA description.
 */
union cvmx_nqm_vfx_vecx_msix_int_st {
	uint64_t u64;
	struct cvmx_nqm_vfx_vecx_msix_int_st_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_28_63               : 36;
	uint64_t intcnt                       : 8;  /**< Interrupt count field. When nonzero, there is a pending PBA bit assertion
                                                         (i.e. NQM is actively coalescing interrupts) for the vector.
                                                         Unless there is one or more enabled and non-empty CQ that is (1) enabled
                                                         for interrupts and (2) mapped to the MSI-X table entry, [INTCNT] is zero.
                                                         The NQM_VF()_CQ()_ENA[ENABLE] documentation describes when a CQ is enabled. */
	uint64_t reserved_12_19               : 8;
	uint64_t ts                           : 12; /**< Timer stamp field. */
#else
	uint64_t ts                           : 12;
	uint64_t reserved_12_19               : 8;
	uint64_t intcnt                       : 8;
	uint64_t reserved_28_63               : 36;
#endif
	} s;
	struct cvmx_nqm_vfx_vecx_msix_int_st_s cn73xx;
};
typedef union cvmx_nqm_vfx_vecx_msix_int_st cvmx_nqm_vfx_vecx_msix_int_st_t;

/**
 * cvmx_nqm_vf#_vs
 *
 * NQM_VF(x)_VS is the NVMe-standard version (VS) CSR for
 * function x. NQM_VF_MODE[VF_MODE] selects the functions that are actually
 * present in the hardware. See NQM_VF_MODE_E.
 */
union cvmx_nqm_vfx_vs {
	uint32_t u32;
	struct cvmx_nqm_vfx_vs_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint32_t mjr                          : 16; /**< Major version number.
                                                         [MJR] is a RO copy of NQM_CFG[MJR]. [MJR] can be modified by software
                                                         via NQM_CFG. */
	uint32_t mnr                          : 16; /**< Minor version number.
                                                         [MNR] is a RO copy of NQM_CFG[MNR]. [MNR] can be modified by software
                                                         via NQM_CFG. */
#else
	uint32_t mnr                          : 16;
	uint32_t mjr                          : 16;
#endif
	} s;
	struct cvmx_nqm_vfx_vs_s              cn73xx;
};
typedef union cvmx_nqm_vfx_vs cvmx_nqm_vfx_vs_t;

/**
 * cvmx_nqm_vf_mode
 */
union cvmx_nqm_vf_mode {
	uint64_t u64;
	struct cvmx_nqm_vf_mode_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t vf_mode                      : 2;  /**< Selects the number of functions, the number of queues per function, and
                                                         the number of MSI-X vectors per function that are actually supported by
                                                         the hardware. Enumerated by NQM_VF_MODE_E. */
#else
	uint64_t vf_mode                      : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_nqm_vf_mode_s             cn73xx;
};
typedef union cvmx_nqm_vf_mode cvmx_nqm_vf_mode_t;

#endif
