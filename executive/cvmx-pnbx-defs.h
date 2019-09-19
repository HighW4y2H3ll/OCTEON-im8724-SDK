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
 * cvmx-pnbx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon pnbx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_PNBX_DEFS_H__
#define __CVMX_PNBX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000210ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B0000210ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000000ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800B0000000ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_CONTROL(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_CONTROL(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900000ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_CONTROL(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900000ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_ECO(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_ECO(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900058ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_ECO(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900058ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_ERR_ENABLE0(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_ERR_ENABLE0(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900040ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_ERR_ENABLE0(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900040ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_ERR_SOURCE0(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_ERR_SOURCE0(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900030ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_ERR_SOURCE0(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900030ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_SCRATCH(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_SCRATCH(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900050ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_SCRATCH(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900050ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMAX_STATUS(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_DMAX_STATUS(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3900018ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768;
}
#else
#define CVMX_PNBX_DMAX_STATUS(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3900018ull) + (((offset) & 1) + ((block_id) & 1) * 0x10ull) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000520ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B0000520ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000528ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B0000528ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000530ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B0000530ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000538ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B0000538ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000540ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG4(offset) (CVMX_ADD_IO_SEG(0x00011800B0000540ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000548ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG5(offset) (CVMX_ADD_IO_SEG(0x00011800B0000548ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG6(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG6(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000550ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG6(offset) (CVMX_ADD_IO_SEG(0x00011800B0000550ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000558ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG7(offset) (CVMX_ADD_IO_SEG(0x00011800B0000558ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG8(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG8(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000560ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG8(offset) (CVMX_ADD_IO_SEG(0x00011800B0000560ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_DMA_DIAG9(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_DMA_DIAG9(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000568ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_DMA_DIAG9(offset) (CVMX_ADD_IO_SEG(0x00011800B0000568ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000300ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B0000300ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHABX_PULL_ARB_WT(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 5)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_PNBX_GHABX_PULL_ARB_WT(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B0000100ull) + (((offset) & 7) + ((block_id) & 1) * 0x200000ull) * 8;
}
#else
#define CVMX_PNBX_GHABX_PULL_ARB_WT(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B0000100ull) + (((offset) & 7) + ((block_id) & 1) * 0x200000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHAB_INB_ARB_WT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHAB_INB_ARB_WT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000140ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHAB_INB_ARB_WT(offset) (CVMX_ADD_IO_SEG(0x00011800B0000140ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHAB_PULL_BUSHOG_MAX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHAB_PULL_BUSHOG_MAX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000010ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHAB_PULL_BUSHOG_MAX(offset) (CVMX_ADD_IO_SEG(0x00011800B0000010ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000400ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B0000400ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000408ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B0000408ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000410ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B0000410ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000418ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B0000418ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000420ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG4(offset) (CVMX_ADD_IO_SEG(0x00011800B0000420ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000428ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG5(offset) (CVMX_ADD_IO_SEG(0x00011800B0000428ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBRD_DIAG6(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBRD_DIAG6(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000430ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBRD_DIAG6(offset) (CVMX_ADD_IO_SEG(0x00011800B0000430ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000440ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B0000440ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000448ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B0000448ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000450ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B0000450ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000458ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B0000458ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000460ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG4(offset) (CVMX_ADD_IO_SEG(0x00011800B0000460ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000468ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG5(offset) (CVMX_ADD_IO_SEG(0x00011800B0000468ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG6(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG6(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000470ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG6(offset) (CVMX_ADD_IO_SEG(0x00011800B0000470ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_GHBWR_DIAG7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_GHBWR_DIAG7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000478ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_GHBWR_DIAG7(offset) (CVMX_ADD_IO_SEG(0x00011800B0000478ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_IARB_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_IARB_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004D0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_IARB_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B00004D0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_IARB_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_IARB_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004D8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_IARB_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B00004D8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_IARB_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_IARB_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004E0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_IARB_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B00004E0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_IARB_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_IARB_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004E8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_IARB_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B00004E8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_INB_ARB_BUSHOG_MAX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_INB_ARB_BUSHOG_MAX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000020ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_INB_ARB_BUSHOG_MAX(offset) (CVMX_ADD_IO_SEG(0x00011800B0000020ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_INT_SUM(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_INT_SUM(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000170ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_INT_SUM(offset) (CVMX_ADD_IO_SEG(0x00011800B0000170ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_MEM_ECC_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_MEM_ECC_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000200ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_MEM_ECC_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800B0000200ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_NCBO_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_NCBO_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000500ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_NCBO_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B0000500ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_NCBO_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_NCBO_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000508ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_NCBO_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B0000508ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_NCBO_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_NCBO_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000510ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_NCBO_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B0000510ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_NCBO_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_NCBO_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000518ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_NCBO_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B0000518ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PPCMD_FF_DBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PPCMD_FF_DBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00001A0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PPCMD_FF_DBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B00001A0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PPCMD_FF_SBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PPCMD_FF_SBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00001A8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PPCMD_FF_SBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B00001A8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PPRSP_FF_DBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PPRSP_FF_DBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00001B0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PPRSP_FF_DBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B00001B0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PPRSP_FF_SBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PPRSP_FF_SBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00001B8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PPRSP_FF_SBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B00001B8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PP_PUSH_ARB_WT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PP_PUSH_ARB_WT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000150ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PP_PUSH_ARB_WT(offset) (CVMX_ADD_IO_SEG(0x00011800B0000150ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PSM_DIAG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PSM_DIAG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004C0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PSM_DIAG(offset) (CVMX_ADD_IO_SEG(0x00011800B00004C0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_PSM_PUSH_ARB_WT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_PSM_PUSH_ARB_WT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000160ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_PSM_PUSH_ARB_WT(offset) (CVMX_ADD_IO_SEG(0x00011800B0000160ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEMRD_DBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEMRD_DBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000180ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEMRD_DBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B0000180ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEMRD_SBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEMRD_SBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000188ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEMRD_SBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B0000188ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEMWR_DBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEMWR_DBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000190ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEMWR_DBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B0000190ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEMWR_SBE_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEMWR_SBE_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000198ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEMWR_SBE_INFO(offset) (CVMX_ADD_IO_SEG(0x00011800B0000198ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000480ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG0(offset) (CVMX_ADD_IO_SEG(0x00011800B0000480ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000488ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG1(offset) (CVMX_ADD_IO_SEG(0x00011800B0000488ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000490ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG2(offset) (CVMX_ADD_IO_SEG(0x00011800B0000490ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000498ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG3(offset) (CVMX_ADD_IO_SEG(0x00011800B0000498ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004A0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG4(offset) (CVMX_ADD_IO_SEG(0x00011800B00004A0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004A8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG5(offset) (CVMX_ADD_IO_SEG(0x00011800B00004A8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG6(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG6(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004B0ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG6(offset) (CVMX_ADD_IO_SEG(0x00011800B00004B0ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_DIAG7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_DIAG7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B00004B8ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_DIAG7(offset) (CVMX_ADD_IO_SEG(0x00011800B00004B8ull) + ((offset) & 1) * 0x1000000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_PNBX_SMEM_PUSH_BUSHOG_MAX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_PNBX_SMEM_PUSH_BUSHOG_MAX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B0000018ull) + ((offset) & 1) * 0x1000000ull;
}
#else
#define CVMX_PNBX_SMEM_PUSH_BUSHOG_MAX(offset) (CVMX_ADD_IO_SEG(0x00011800B0000018ull) + ((offset) & 1) * 0x1000000ull)
#endif

/**
 * cvmx_pnb#_bist_status
 *
 * BIST test status. 0 = pass, 1 = fail.
 *
 */
union cvmx_pnbx_bist_status {
	uint64_t u64;
	struct cvmx_pnbx_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t status                       : 13; /**< BIST status. */
#else
	uint64_t status                       : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_pnbx_bist_status_s        cnf75xx;
};
typedef union cvmx_pnbx_bist_status cvmx_pnbx_bist_status_t;

/**
 * cvmx_pnb#_config
 */
union cvmx_pnbx_config {
	uint64_t u64;
	struct cvmx_pnbx_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t force_sel_ncb                : 2;  /**< Force the inbound arbiter to always use only one I/O bus segment:
                                                         0x0 = Allow requests to use both bus segments.
                                                         0x1 = Force all requests to system memory on bus 0.
                                                         0x2 = Force all requests to system memory on bus 1.
                                                         0x3 = Reserved. */
	uint64_t diag_en                      : 1;  /**< Enable PNB diagnostics. For internal use only. */
	uint64_t pp_req_priority              : 1;  /**< Select the priority for SMEM requests from cnMIPS cores:
                                                         0 = Low priority.
                                                         1 = High priority. */
#else
	uint64_t pp_req_priority              : 1;
	uint64_t diag_en                      : 1;
	uint64_t force_sel_ncb                : 2;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_pnbx_config_s             cnf75xx;
};
typedef union cvmx_pnbx_config cvmx_pnbx_config_t;

/**
 * cvmx_pnb#_dma#_control
 *
 * AB control register for PNB DMA operation
 *
 */
union cvmx_pnbx_dmax_control {
	uint64_t u64;
	struct cvmx_pnbx_dmax_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (RDEC_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid0                       : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_dmax_control_s       cnf75xx;
};
typedef union cvmx_pnbx_dmax_control cvmx_pnbx_dmax_control_t;

/**
 * cvmx_pnb#_dma#_eco
 *
 * An ECO CSR.
 *
 */
union cvmx_pnbx_dmax_eco {
	uint64_t u64;
	struct cvmx_pnbx_dmax_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_pnbx_dmax_eco_s           cnf75xx;
};
typedef union cvmx_pnbx_dmax_eco cvmx_pnbx_dmax_eco_t;

/**
 * cvmx_pnb#_dma#_err_enable0
 *
 * This register enables error reporting for ERROR_SOURCE0 register
 *
 */
union cvmx_pnbx_dmax_err_enable0 {
	uint64_t u64;
	struct cvmx_pnbx_dmax_err_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t err_en                       : 1;  /**< ERR_ENABLE. */
#else
	uint64_t err_en                       : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_pnbx_dmax_err_enable0_s   cnf75xx;
};
typedef union cvmx_pnbx_dmax_err_enable0 cvmx_pnbx_dmax_err_enable0_t;

/**
 * cvmx_pnb#_dma#_err_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_pnbx_dmax_err_source0 {
	uint64_t u64;
	struct cvmx_pnbx_dmax_err_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID for Read Port 0 Overflow/Underflow */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read Port0 overflow */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read Port0 underflow */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid_ufof               : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_dmax_err_source0_s   cnf75xx;
};
typedef union cvmx_pnbx_dmax_err_source0 cvmx_pnbx_dmax_err_source0_t;

/**
 * cvmx_pnb#_dma#_scratch
 *
 * A scratch CSR.
 *
 */
union cvmx_pnbx_dmax_scratch {
	uint64_t u64;
	struct cvmx_pnbx_dmax_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_0_63                : 64;
#else
	uint64_t reserved_0_63                : 64;
#endif
	} s;
	struct cvmx_pnbx_dmax_scratch_s       cnf75xx;
};
typedef union cvmx_pnbx_dmax_scratch cvmx_pnbx_dmax_scratch_t;

/**
 * cvmx_pnb#_dma#_status
 *
 * AB status register for PNB DMA operation
 *
 */
union cvmx_pnbx_dmax_status {
	uint64_t u64;
	struct cvmx_pnbx_dmax_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job when set */
	uint64_t reserved_1_3                 : 3;
	uint64_t busy                         : 1;  /**< Busy on processing a job when set */
#else
	uint64_t busy                         : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_pnbx_dmax_status_s        cnf75xx;
};
typedef union cvmx_pnbx_dmax_status cvmx_pnbx_dmax_status_t;

/**
 * cvmx_pnb#_dma_diag0
 *
 * GAA SMEM read and write command counters
 *
 */
union cvmx_pnbx_dma_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t cmd_smem_wr                  : 32; /**< total SMEM write commands */
	uint64_t cmd_smem_rd                  : 32; /**< total SMEM read commands */
#else
	uint64_t cmd_smem_rd                  : 32;
	uint64_t cmd_smem_wr                  : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag0_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag0 cvmx_pnbx_dma_diag0_t;

/**
 * cvmx_pnb#_dma_diag1
 *
 * GAA DDR read command counters
 *
 */
union cvmx_pnbx_dma_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ddr_rd_size                  : 32; /**< total DDR read command sizes */
	uint64_t ddr_rd_cnt                   : 32; /**< total DDR read commands */
#else
	uint64_t ddr_rd_cnt                   : 32;
	uint64_t ddr_rd_size                  : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag1_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag1 cvmx_pnbx_dma_diag1_t;

/**
 * cvmx_pnb#_dma_diag2
 *
 * GAA DDR write command counters
 *
 */
union cvmx_pnbx_dma_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ddr_wr_size                  : 32; /**< total DDR write command sizes */
	uint64_t ddr_wr_cnt                   : 32; /**< total DDR write commands */
#else
	uint64_t ddr_wr_cnt                   : 32;
	uint64_t ddr_wr_size                  : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag2_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag2 cvmx_pnbx_dma_diag2_t;

/**
 * cvmx_pnb#_dma_diag3
 *
 * GAA read and write returned credits
 *
 */
union cvmx_pnbx_dma_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t cmd_wr_cdt                   : 32; /**< total write credit returned */
	uint64_t cmd_rd_cdt                   : 32; /**< total read credit returned */
#else
	uint64_t cmd_rd_cdt                   : 32;
	uint64_t cmd_wr_cdt                   : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag3_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag3 cvmx_pnbx_dma_diag3_t;

/**
 * cvmx_pnb#_dma_diag4
 *
 * GAA read and write responses count
 *
 */
union cvmx_pnbx_dma_diag4 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_wr_cnt                   : 32; /**< total write responses */
	uint64_t rsp_rd_cnt                   : 32; /**< total read responses */
#else
	uint64_t rsp_rd_cnt                   : 32;
	uint64_t rsp_wr_cnt                   : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag4_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag4 cvmx_pnbx_dma_diag4_t;

/**
 * cvmx_pnb#_dma_diag5
 *
 * Counts number of DMA reads granted and processed by NCBi
 *
 */
union cvmx_pnbx_dma_diag5 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t req_iarb_pop                 : 32; /**< Request Command popped by inbound arbiter */
	uint64_t req_ncbi_cmd                 : 32; /**< request Command read by NCBI */
#else
	uint64_t req_ncbi_cmd                 : 32;
	uint64_t req_iarb_pop                 : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag5_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag5 cvmx_pnbx_dma_diag5_t;

/**
 * cvmx_pnb#_dma_diag6
 *
 * Counts number of DMA writes granted by NCBi
 *
 */
union cvmx_pnbx_dma_diag6 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag6_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_iarb_pop                 : 32; /**< Request Command popped by inbound arbiter */
#else
	uint64_t req_iarb_pop                 : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag6_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag6 cvmx_pnbx_dma_diag6_t;

/**
 * cvmx_pnb#_dma_diag7
 *
 * Counts number of DMA writes processed by NCBi
 *
 */
union cvmx_pnbx_dma_diag7 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t req_ncbi_dat                 : 32; /**< request data read by NCBI */
	uint64_t req_ncbi_cmd                 : 32; /**< request Command read by NCBI */
#else
	uint64_t req_ncbi_cmd                 : 32;
	uint64_t req_ncbi_dat                 : 32;
#endif
	} s;
	struct cvmx_pnbx_dma_diag7_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag7 cvmx_pnbx_dma_diag7_t;

/**
 * cvmx_pnb#_dma_diag8
 *
 * GAAIF internal states
 *
 */
union cvmx_pnbx_dma_diag8 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag8_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_36_63               : 28;
	uint64_t smem_req_bus                 : 12; /**< diag_dma_smem_rreq_cnt[5:0], diag_dma_smem_wreq_cnt[5:0] */
	uint64_t smem_rsp_bus                 : 24; /**< diag_dma_swrsp_ff_cnt[5:0], diag_dma_srrsp_arb_vld, diag_dma_srrsp_arb_stall,
                                                         diag_dma_srrsp_ff_wptr[4:0], diag_dma_srrsp_ff_rptr[4:0], diag_dma_srrsp_ff_cnt[5:0] */
#else
	uint64_t smem_rsp_bus                 : 24;
	uint64_t smem_req_bus                 : 12;
	uint64_t reserved_36_63               : 28;
#endif
	} s;
	struct cvmx_pnbx_dma_diag8_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag8 cvmx_pnbx_dma_diag8_t;

/**
 * cvmx_pnb#_dma_diag9
 *
 * GAAIF internal states
 *
 */
union cvmx_pnbx_dma_diag9 {
	uint64_t u64;
	struct cvmx_pnbx_dma_diag9_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t ddr_wrreq_bus                : 24; /**< diag_dma_ddr_flitwr_win, diag_dma_ddr_wff_wptr[2:0], diag_dma_ddr_flit_wptr[2:0],
                                                         diag_dma_ddr_arb_wff_rptr[2:0], diag_dma_ddr_ncbi_wff_rptr[2:0],
                                                         diag_dma_ddr_flit_rptr[2:0], diag_dma_ddr_wr_arb_cnt[3:0], diag_dma_ddr_wr_ncbi_cnt[3:0] */
	uint64_t ddr_wrrsp_bus                : 10; /**< diag_dma_wr_stall0, diag_dma_wr_stall1, diag_dma_dwrsp_ff0_vld, diag_dma_dwrsp_ff1_vld,
                                                         diag_dma_dwrsp_cpl_sel, diag_dma_dwrsp_cpl_ff0_vld, diag_dma_dwrsp_cpl_ff1_vld,
                                                         diag_dma_dwrsp_dat_sel, diag_dma_dwrsp_dat_ff0_vld, diag_dma_dwrsp_dat_ff1_vld */
	uint64_t reserved_22_27               : 6;
	uint64_t ddr_rdreq_bus                : 12; /**< diag_dma_ddr_rd_arb_cnt[5:0], diag_dma_ddr_rd_ncbi_cnt[5:0] */
	uint64_t ddr_rdrsp_bus                : 10; /**< [diag_dma_rd_stall0, diag_dma_rd_stall1, diag_dma_drrsp_dat0_win, diag_dma_drrsp_dat1_win,
                                                         diag_dma_drrsp_cpl_sel, diag_dma_drrsp_cpl_ff0_vld, diag_dma_drrsp_cpl_ff1_vld,
                                                         diag_dma_drrsp_dat_sel, diag_dma_drrsp_dat_ff0_vld, diag_dma_drrsp_dat_ff1_vld */
#else
	uint64_t ddr_rdrsp_bus                : 10;
	uint64_t ddr_rdreq_bus                : 12;
	uint64_t reserved_22_27               : 6;
	uint64_t ddr_wrrsp_bus                : 10;
	uint64_t ddr_wrreq_bus                : 24;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_pnbx_dma_diag9_s          cnf75xx;
};
typedef union cvmx_pnbx_dma_diag9 cvmx_pnbx_dma_diag9_t;

/**
 * cvmx_pnb#_eco
 *
 * An ECO CSR.
 *
 */
union cvmx_pnbx_eco {
	uint64_t u64;
	struct cvmx_pnbx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_pnbx_eco_s                cnf75xx;
};
typedef union cvmx_pnbx_eco cvmx_pnbx_eco_t;

/**
 * cvmx_pnb#_ghab#_pull_arb_wt
 *
 * This register specifies the aribtration weights used when accepting GHAB
 * requests to system memory. Each GHAB specifies one weight for high
 * priority reads and writes, and one weight for low priority reads and
 * writes.
 */
union cvmx_pnbx_ghabx_pull_arb_wt {
	uint64_t u64;
	struct cvmx_pnbx_ghabx_pull_arb_wt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t hp_wgt                       : 6;  /**< Arbitration weight for high priority reads and writes. */
	uint64_t reserved_6_15                : 10;
	uint64_t lp_wgt                       : 6;  /**< Arbitration weight for low priority reads and writes. */
#else
	uint64_t lp_wgt                       : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t hp_wgt                       : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_pnbx_ghabx_pull_arb_wt_s  cnf75xx;
};
typedef union cvmx_pnbx_ghabx_pull_arb_wt cvmx_pnbx_ghabx_pull_arb_wt_t;

/**
 * cvmx_pnb#_ghab_inb_arb_wt
 *
 * This register specifies the arbitration weights used when arbitrating
 * between PNBD DMA requests and GHAB requests to system memory.
 */
union cvmx_pnbx_ghab_inb_arb_wt {
	uint64_t u64;
	struct cvmx_pnbx_ghab_inb_arb_wt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_54_63               : 10;
	uint64_t hp_wr_wgt                    : 6;  /**< Arbitration weight for high priority writes. */
	uint64_t reserved_38_47               : 10;
	uint64_t lp_wr_wgt                    : 6;  /**< Arbitration weight for low priority writes. */
	uint64_t reserved_22_31               : 10;
	uint64_t hp_rd_wgt                    : 6;  /**< Arbitration weight for high priority reads. */
	uint64_t reserved_6_15                : 10;
	uint64_t lp_rd_wgt                    : 6;  /**< Arbitration weight for low priority reads. */
#else
	uint64_t lp_rd_wgt                    : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t hp_rd_wgt                    : 6;
	uint64_t reserved_22_31               : 10;
	uint64_t lp_wr_wgt                    : 6;
	uint64_t reserved_38_47               : 10;
	uint64_t hp_wr_wgt                    : 6;
	uint64_t reserved_54_63               : 10;
#endif
	} s;
	struct cvmx_pnbx_ghab_inb_arb_wt_s    cnf75xx;
};
typedef union cvmx_pnbx_ghab_inb_arb_wt cvmx_pnbx_ghab_inb_arb_wt_t;

/**
 * cvmx_pnb#_ghab_pull_bushog_max
 *
 * This register controls the bus-hog limit when arbitrating for system
 * memory requests coming from the GHABs.
 */
union cvmx_pnbx_ghab_pull_bushog_max {
	uint64_t u64;
	struct cvmx_pnbx_ghab_pull_bushog_max_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t bushog_max                   : 8;  /**< The maximum number of consecutive high priority requests that can win
                                                         arbitration over a waiting low-priority request. If set to zero, no
                                                         limit is enforced and low priority requests might be starved. */
#else
	uint64_t bushog_max                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_pnbx_ghab_pull_bushog_max_s cnf75xx;
};
typedef union cvmx_pnbx_ghab_pull_bushog_max cvmx_pnbx_ghab_pull_bushog_max_t;

/**
 * cvmx_pnb#_ghbrd_diag0
 *
 * Counts number of GHAB read requests received
 *
 */
union cvmx_pnbx_ghbrd_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_cmd                      : 32; /**< Request Command received */
#else
	uint64_t req_cmd                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag0_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag0 cvmx_pnbx_ghbrd_diag0_t;

/**
 * cvmx_pnb#_ghbrd_diag1
 *
 * Counts number of GHAB read granted
 *
 */
union cvmx_pnbx_ghbrd_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_gnt                      : 32; /**< Request Command granted */
#else
	uint64_t req_gnt                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag1_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag1 cvmx_pnbx_ghbrd_diag1_t;

/**
 * cvmx_pnb#_ghbrd_diag2
 *
 * Counts number of GHAB read processed
 *
 */
union cvmx_pnbx_ghbrd_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t obuf_dat_rptr                : 2;  /**< data buffer read pointer */
	uint64_t obuf_dat_wptr                : 2;  /**< data buffer write pointer */
	uint64_t obuf_cmd_rptr                : 2;  /**< command buffer read pointer */
	uint64_t obuf_cmd_wptr                : 2;  /**< command buffer write pointer */
	uint64_t req_ff_full                  : 1;  /**< request FIFO full */
	uint64_t req_ff_empty                 : 1;  /**< request FIFO empty */
	uint64_t cpl_ff_cnt                   : 12; /**< total request completed count */
	uint64_t cmd_cdt_cnt                  : 8;  /**< credit count for outstanding NCBi grants */
	uint64_t req_ncbi_rd                  : 32; /**< request Command read by NCBI */
#else
	uint64_t req_ncbi_rd                  : 32;
	uint64_t cmd_cdt_cnt                  : 8;
	uint64_t cpl_ff_cnt                   : 12;
	uint64_t req_ff_empty                 : 1;
	uint64_t req_ff_full                  : 1;
	uint64_t obuf_cmd_wptr                : 2;
	uint64_t obuf_cmd_rptr                : 2;
	uint64_t obuf_dat_wptr                : 2;
	uint64_t obuf_dat_rptr                : 2;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag2_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag2 cvmx_pnbx_ghbrd_diag2_t;

/**
 * cvmx_pnb#_ghbrd_diag3
 *
 * Counts number of GHAB read response
 *
 */
union cvmx_pnbx_ghbrd_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rsp_cmd                      : 32; /**< read response sent in 128b */
#else
	uint64_t rsp_cmd                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag3_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag3 cvmx_pnbx_ghbrd_diag3_t;

/**
 * cvmx_pnb#_ghbrd_diag4
 *
 * read responses from NCBO segment 0
 *
 */
union cvmx_pnbx_ghbrd_diag4 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rsp_pkt_cnt                  : 32; /**< per transaction count */
#else
	uint64_t rsp_pkt_cnt                  : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag4_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag4 cvmx_pnbx_ghbrd_diag4_t;

/**
 * cvmx_pnb#_ghbrd_diag5
 *
 * read responses from NCBO segment 1
 *
 */
union cvmx_pnbx_ghbrd_diag5 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rsp_pkt_cnt                  : 32; /**< per transaction count */
#else
	uint64_t rsp_pkt_cnt                  : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag5_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag5 cvmx_pnbx_ghbrd_diag5_t;

/**
 * cvmx_pnb#_ghbrd_diag6
 *
 * read response completion count
 *
 */
union cvmx_pnbx_ghbrd_diag6 {
	uint64_t u64;
	struct cvmx_pnbx_ghbrd_diag6_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_cpl_cnt1                 : 32; /**< completion count from NCBO segment 1 */
	uint64_t rsp_cpl_cnt0                 : 32; /**< completion count from NCBO segment 0 */
#else
	uint64_t rsp_cpl_cnt0                 : 32;
	uint64_t rsp_cpl_cnt1                 : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbrd_diag6_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbrd_diag6 cvmx_pnbx_ghbrd_diag6_t;

/**
 * cvmx_pnb#_ghbwr_diag0
 *
 * Counts number of GHAB write requests received
 *
 */
union cvmx_pnbx_ghbwr_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_pull_last                : 32; /**< Request Pull with last flag set */
#else
	uint64_t req_pull_last                : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag0_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag0 cvmx_pnbx_ghbwr_diag0_t;

/**
 * cvmx_pnb#_ghbwr_diag1
 *
 * Counts number of GHAB write commands received
 *
 */
union cvmx_pnbx_ghbwr_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t req_cmd                      : 32; /**< Request Command received */
	uint64_t req_cmd_last                 : 32; /**< Request Command with last flag set */
#else
	uint64_t req_cmd_last                 : 32;
	uint64_t req_cmd                      : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag1_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag1 cvmx_pnbx_ghbwr_diag1_t;

/**
 * cvmx_pnb#_ghbwr_diag2
 *
 * Counts number of GHAB write granted
 *
 */
union cvmx_pnbx_ghbwr_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_gnt                      : 32; /**< Request Command granted */
#else
	uint64_t req_gnt                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag2_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag2 cvmx_pnbx_ghbwr_diag2_t;

/**
 * cvmx_pnb#_ghbwr_diag3
 *
 * Counts number of GHAB writes processed by NCBi
 *
 */
union cvmx_pnbx_ghbwr_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t req_ncbi_cmd                 : 32; /**< request Command read by NCBI */
#else
	uint64_t req_ncbi_cmd                 : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag3_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag3 cvmx_pnbx_ghbwr_diag3_t;

/**
 * cvmx_pnb#_ghbwr_diag4
 *
 * Counts number of GHAB writes processed by NCBi
 *
 */
union cvmx_pnbx_ghbwr_diag4 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_41_63               : 23;
	uint64_t ff_pkt_cpl_rptr              : 5;  /**< FIFO packet write pointer (completion) */
	uint64_t ff_pkt_req_rptr              : 5;  /**< FIFO packet write pointer (request) */
	uint64_t ff_flit_rptr                 : 3;  /**< FIFO flit read pointer */
	uint64_t ff_pkt_wptr                  : 5;  /**< FIFO packet write pointer */
	uint64_t ff_flit_wptr                 : 3;  /**< FIFO flit write pointer */
	uint64_t reserved_18_19               : 2;
	uint64_t req_ff_cnt                   : 6;  /**< total request outstanding count */
	uint64_t cpl_ff_cnt                   : 6;  /**< total request completed count */
	uint64_t cmd_cdt_cnt                  : 6;  /**< credit count for outstanding NCBi grants */
#else
	uint64_t cmd_cdt_cnt                  : 6;
	uint64_t cpl_ff_cnt                   : 6;
	uint64_t req_ff_cnt                   : 6;
	uint64_t reserved_18_19               : 2;
	uint64_t ff_flit_wptr                 : 3;
	uint64_t ff_pkt_wptr                  : 5;
	uint64_t ff_flit_rptr                 : 3;
	uint64_t ff_pkt_req_rptr              : 5;
	uint64_t ff_pkt_cpl_rptr              : 5;
	uint64_t reserved_41_63               : 23;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag4_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag4 cvmx_pnbx_ghbwr_diag4_t;

/**
 * cvmx_pnb#_ghbwr_diag5
 *
 * Counts number of GHAB write response
 *
 */
union cvmx_pnbx_ghbwr_diag5 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rsp_cmd                      : 32; /**< write response sent */
#else
	uint64_t rsp_cmd                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag5_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag5 cvmx_pnbx_ghbwr_diag5_t;

/**
 * cvmx_pnb#_ghbwr_diag6
 *
 * write responses from NCBO segment 0 and 1
 *
 */
union cvmx_pnbx_ghbwr_diag6 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag6_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_pkt_cnt1                 : 32; /**< per transaction count from segment 1 */
	uint64_t rsp_pkt_cnt0                 : 32; /**< per transaction count from segment 0 */
#else
	uint64_t rsp_pkt_cnt0                 : 32;
	uint64_t rsp_pkt_cnt1                 : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag6_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag6 cvmx_pnbx_ghbwr_diag6_t;

/**
 * cvmx_pnb#_ghbwr_diag7
 *
 * write response completion count
 *
 */
union cvmx_pnbx_ghbwr_diag7 {
	uint64_t u64;
	struct cvmx_pnbx_ghbwr_diag7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_cpl_cnt1                 : 32; /**< completion count from NCBO segment 1 */
	uint64_t rsp_cpl_cnt0                 : 32; /**< completion count from NCBO segment 0 */
#else
	uint64_t rsp_cpl_cnt0                 : 32;
	uint64_t rsp_cpl_cnt1                 : 32;
#endif
	} s;
	struct cvmx_pnbx_ghbwr_diag7_s        cnf75xx;
};
typedef union cvmx_pnbx_ghbwr_diag7 cvmx_pnbx_ghbwr_diag7_t;

/**
 * cvmx_pnb#_iarb_diag0
 *
 * Counts number of PSM status write requests
 *
 */
union cvmx_pnbx_iarb_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_iarb_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dev0_compl                   : 32; /**< Number of NCBi device 0 L2C/DDR requests completed */
#else
	uint64_t dev0_compl                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_iarb_diag0_s         cnf75xx;
};
typedef union cvmx_pnbx_iarb_diag0 cvmx_pnbx_iarb_diag0_t;

/**
 * cvmx_pnb#_iarb_diag1
 *
 * Counts number of PSM status write requests
 *
 */
union cvmx_pnbx_iarb_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_iarb_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dev1_compl                   : 32; /**< Number of NCBi device 1 L2C/DDR requests completed */
#else
	uint64_t dev1_compl                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_iarb_diag1_s         cnf75xx;
};
typedef union cvmx_pnbx_iarb_diag1 cvmx_pnbx_iarb_diag1_t;

/**
 * cvmx_pnb#_iarb_diag2
 *
 * Counts number of PSM status write requests
 *
 */
union cvmx_pnbx_iarb_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_iarb_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dev2_compl                   : 32; /**< Number of NCBi device 2 L2C/DDR requests completed */
#else
	uint64_t dev2_compl                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_iarb_diag2_s         cnf75xx;
};
typedef union cvmx_pnbx_iarb_diag2 cvmx_pnbx_iarb_diag2_t;

/**
 * cvmx_pnb#_iarb_diag3
 *
 * Counts number of PSM status write requests
 *
 */
union cvmx_pnbx_iarb_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_iarb_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dev3_compl                   : 32; /**< Number of NCBi device 3 L2C/DDR requests completed */
#else
	uint64_t dev3_compl                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_iarb_diag3_s         cnf75xx;
};
typedef union cvmx_pnbx_iarb_diag3 cvmx_pnbx_iarb_diag3_t;

/**
 * cvmx_pnb#_inb_arb_bushog_max
 *
 * This regisers specifies the bus-hog limit when arbitrating between PNBD
 * DMA engine requests and GHAB requests to system memory.
 */
union cvmx_pnbx_inb_arb_bushog_max {
	uint64_t u64;
	struct cvmx_pnbx_inb_arb_bushog_max_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t bushog_max                   : 8;  /**< The maximum number of consecutive high priority requests. */
#else
	uint64_t bushog_max                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_pnbx_inb_arb_bushog_max_s cnf75xx;
};
typedef union cvmx_pnbx_inb_arb_bushog_max cvmx_pnbx_inb_arb_bushog_max_t;

/**
 * cvmx_pnb#_int_sum
 *
 * This register contains records a summary of outstanding PNB interrupts.
 *
 */
union cvmx_pnbx_int_sum {
	uint64_t u64;
	struct cvmx_pnbx_int_sum_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_30_63               : 34;
	uint64_t mem12_dbe                    : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem11_dbe                    : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem10_dbe                    : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem9_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem8_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem7_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem6_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem5_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem4_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem3_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem2_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem1_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem0_dbe                     : 1;  /**< Double-bit ECC error in internal PNB SRAM. */
	uint64_t mem12_sbe                    : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem11_sbe                    : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem10_sbe                    : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem9_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem8_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem7_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem6_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem5_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem4_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem3_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem2_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem1_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t mem0_sbe                     : 1;  /**< Single-bit ECC error in internal PNB SRAM. */
	uint64_t smemrd_sbe                   : 1;  /**< A single-bit ECC error occured during an SMEM read. */
	uint64_t smemrd_dbe                   : 1;  /**< A double-bit ECC error occured during an SMEM read. */
	uint64_t smemwr_sbe                   : 1;  /**< A single-bit ECC error occured during an SMEM write. */
	uint64_t smemwr_dbe                   : 1;  /**< A double-bit ECC error occured during an SMEM write. */
#else
	uint64_t smemwr_dbe                   : 1;
	uint64_t smemwr_sbe                   : 1;
	uint64_t smemrd_dbe                   : 1;
	uint64_t smemrd_sbe                   : 1;
	uint64_t mem0_sbe                     : 1;
	uint64_t mem1_sbe                     : 1;
	uint64_t mem2_sbe                     : 1;
	uint64_t mem3_sbe                     : 1;
	uint64_t mem4_sbe                     : 1;
	uint64_t mem5_sbe                     : 1;
	uint64_t mem6_sbe                     : 1;
	uint64_t mem7_sbe                     : 1;
	uint64_t mem8_sbe                     : 1;
	uint64_t mem9_sbe                     : 1;
	uint64_t mem10_sbe                    : 1;
	uint64_t mem11_sbe                    : 1;
	uint64_t mem12_sbe                    : 1;
	uint64_t mem0_dbe                     : 1;
	uint64_t mem1_dbe                     : 1;
	uint64_t mem2_dbe                     : 1;
	uint64_t mem3_dbe                     : 1;
	uint64_t mem4_dbe                     : 1;
	uint64_t mem5_dbe                     : 1;
	uint64_t mem6_dbe                     : 1;
	uint64_t mem7_dbe                     : 1;
	uint64_t mem8_dbe                     : 1;
	uint64_t mem9_dbe                     : 1;
	uint64_t mem10_dbe                    : 1;
	uint64_t mem11_dbe                    : 1;
	uint64_t mem12_dbe                    : 1;
	uint64_t reserved_30_63               : 34;
#endif
	} s;
	struct cvmx_pnbx_int_sum_s            cnf75xx;
};
typedef union cvmx_pnbx_int_sum cvmx_pnbx_int_sum_t;

/**
 * cvmx_pnb#_mem_ecc_ctrl
 *
 * This register set can disable ECC correction and trigger single/double bit errors by flipping
 * the syndrome.
 */
union cvmx_pnbx_mem_ecc_ctrl {
	uint64_t u64;
	struct cvmx_pnbx_mem_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_39_63               : 25;
	uint64_t cdis                         : 13; /**< ECC correction disable for internal PNB memories. */
	uint64_t flip1                        : 13; /**< Flip syndrome bit <1> on access to one of the internal PNB memories */
	uint64_t flip0                        : 13; /**< Flip syndrome bit <0> on access to one of the internal PNB memories */
#else
	uint64_t flip0                        : 13;
	uint64_t flip1                        : 13;
	uint64_t cdis                         : 13;
	uint64_t reserved_39_63               : 25;
#endif
	} s;
	struct cvmx_pnbx_mem_ecc_ctrl_s       cnf75xx;
};
typedef union cvmx_pnbx_mem_ecc_ctrl cvmx_pnbx_mem_ecc_ctrl_t;

/**
 * cvmx_pnb#_ncbo_diag0
 *
 * Counts number of NCBO PP commands
 *
 */
union cvmx_pnbx_ncbo_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_ncbo_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t req_cdt                      : 32; /**< total returned credits */
	uint64_t req_cmd                      : 32; /**< total NCBo request cycles */
#else
	uint64_t req_cmd                      : 32;
	uint64_t req_cdt                      : 32;
#endif
	} s;
	struct cvmx_pnbx_ncbo_diag0_s         cnf75xx;
};
typedef union cvmx_pnbx_ncbo_diag0 cvmx_pnbx_ncbo_diag0_t;

/**
 * cvmx_pnb#_ncbo_diag1
 *
 * Counts number of NCBO PP responses sent
 *
 */
union cvmx_pnbx_ncbo_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_ncbo_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rsp_compl                    : 32; /**< Number of NCBo PP read responses completed */
#else
	uint64_t rsp_compl                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_pnbx_ncbo_diag1_s         cnf75xx;
};
typedef union cvmx_pnbx_ncbo_diag1 cvmx_pnbx_ncbo_diag1_t;

/**
 * cvmx_pnb#_ncbo_diag2
 *
 * NCBO scoreboard internal states
 *
 */
union cvmx_pnbx_ncbo_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_ncbo_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t cam_vector                   : 32; /**< CAM read/write reponse vector */
	uint64_t cam_valid                    : 32; /**< CAM entry valid bits */
#else
	uint64_t cam_valid                    : 32;
	uint64_t cam_vector                   : 32;
#endif
	} s;
	struct cvmx_pnbx_ncbo_diag2_s         cnf75xx;
};
typedef union cvmx_pnbx_ncbo_diag2 cvmx_pnbx_ncbo_diag2_t;

/**
 * cvmx_pnb#_ncbo_diag3
 *
 * NCBO scoreboard internal states
 *
 */
union cvmx_pnbx_ncbo_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_ncbo_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t sb_states                    : 6;  /**< sb_buf_stall, flist_empty, rrsp_ret_ff_vld, wrsp_ret_ff_vld, smem_cmd_ff_vld, cam_match_vec */
#else
	uint64_t sb_states                    : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_pnbx_ncbo_diag3_s         cnf75xx;
};
typedef union cvmx_pnbx_ncbo_diag3 cvmx_pnbx_ncbo_diag3_t;

/**
 * cvmx_pnb#_pp_push_arb_wt
 *
 * This register specifies the arbitration weights for cnMIPS requests to
 * SMEM. The same weight is used for both reads and writes. All requests use
 * either the high or low priority weight depending on the setting of
 * PNB()_CONFIG[PP_REQ_PRIORITY].
 */
union cvmx_pnbx_pp_push_arb_wt {
	uint64_t u64;
	struct cvmx_pnbx_pp_push_arb_wt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t hp_wgt                       : 6;  /**< Arbitration weight when cnMIPS requests have high priority. */
	uint64_t reserved_6_15                : 10;
	uint64_t lp_wgt                       : 6;  /**< Arbitration weight when cnMIPS requests have low priority. */
#else
	uint64_t lp_wgt                       : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t hp_wgt                       : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_pnbx_pp_push_arb_wt_s     cnf75xx;
};
typedef union cvmx_pnbx_pp_push_arb_wt cvmx_pnbx_pp_push_arb_wt_t;

/**
 * cvmx_pnb#_ppcmd_ff_dbe_info
 *
 * This register contains information on double-bit ECC errors that occured
 * in the command FIFO for cnMIPS requests to SMEM. These correspond to
 * PNB(0..1)_INT_SUM[MEM11_DBE] interrupts.
 */
union cvmx_pnbx_ppcmd_ff_dbe_info {
	uint64_t u64;
	struct cvmx_pnbx_ppcmd_ff_dbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t ppcmd_dbe_cmdtype            : 1;  /**< Command type:
                                                         1 = Read.
                                                         0 = Write. */
	uint64_t ppcmd_dbe_cmdsrcid           : 12; /**< PP source ID. */
	uint64_t ppcmd_dbe_cmdaddr            : 20; /**< SMEM address (bits <23:4> of the byte address). */
#else
	uint64_t ppcmd_dbe_cmdaddr            : 20;
	uint64_t ppcmd_dbe_cmdsrcid           : 12;
	uint64_t ppcmd_dbe_cmdtype            : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_pnbx_ppcmd_ff_dbe_info_s  cnf75xx;
};
typedef union cvmx_pnbx_ppcmd_ff_dbe_info cvmx_pnbx_ppcmd_ff_dbe_info_t;

/**
 * cvmx_pnb#_ppcmd_ff_sbe_info
 *
 * This register contains information on single-bit ECC errors that occured
 * in the command FIFO for cnMIPS requests to SMEM. These correspond to
 * PNB(0..1)_INT_SUM[MEM11_SBE] interrupts.
 */
union cvmx_pnbx_ppcmd_ff_sbe_info {
	uint64_t u64;
	struct cvmx_pnbx_ppcmd_ff_sbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t ppcmd_sbe_cmdtype            : 1;  /**< PP command type:
                                                         1 = Read.
                                                         0 = Write. */
	uint64_t ppcmd_sbe_cmdsrcid           : 12; /**< PP source ID. */
	uint64_t ppcmd_sbe_cmdaddr            : 20; /**< SMEM address in 64b. */
#else
	uint64_t ppcmd_sbe_cmdaddr            : 20;
	uint64_t ppcmd_sbe_cmdsrcid           : 12;
	uint64_t ppcmd_sbe_cmdtype            : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_pnbx_ppcmd_ff_sbe_info_s  cnf75xx;
};
typedef union cvmx_pnbx_ppcmd_ff_sbe_info cvmx_pnbx_ppcmd_ff_sbe_info_t;

/**
 * cvmx_pnb#_pprsp_ff_dbe_info
 *
 * This register contains information on double-bit ECC errors that occured
 * in the response FIFO for cnMIPS requests to SMEM. These correspond to
 * PNB(0..1)_INT_SUM[MEM12_DBE] interrupts.
 */
union cvmx_pnbx_pprsp_ff_dbe_info {
	uint64_t u64;
	struct cvmx_pnbx_pprsp_ff_dbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t pprsp_dbe_cmdsrcid           : 12; /**< PP source ID. */
#else
	uint64_t pprsp_dbe_cmdsrcid           : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_pnbx_pprsp_ff_dbe_info_s  cnf75xx;
};
typedef union cvmx_pnbx_pprsp_ff_dbe_info cvmx_pnbx_pprsp_ff_dbe_info_t;

/**
 * cvmx_pnb#_pprsp_ff_sbe_info
 *
 * This register contains information on single-bit ECC errors that occured
 * in the response FIFO for cnMIPS requests to SMEM. These correspond to
 * PNB(0..1)_INT_SUM[MEM12_SBE] interrupts.
 */
union cvmx_pnbx_pprsp_ff_sbe_info {
	uint64_t u64;
	struct cvmx_pnbx_pprsp_ff_sbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t pprsp_sbe_cmdsrcid           : 12; /**< PP source ID. */
#else
	uint64_t pprsp_sbe_cmdsrcid           : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_pnbx_pprsp_ff_sbe_info_s  cnf75xx;
};
typedef union cvmx_pnbx_pprsp_ff_sbe_info cvmx_pnbx_pprsp_ff_sbe_info_t;

/**
 * cvmx_pnb#_psm_diag
 *
 * Counts number of PSM status write requests
 *
 */
union cvmx_pnbx_psm_diag {
	uint64_t u64;
	struct cvmx_pnbx_psm_diag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t req_cdt                      : 16; /**< number of committed credit returned */
	uint64_t req_cmd_cmt                  : 16; /**< number of PSM write requests with committed credit return */
	uint64_t req_cmd                      : 32; /**< number of PSM write cycles (3 cycles per request command) */
#else
	uint64_t req_cmd                      : 32;
	uint64_t req_cmd_cmt                  : 16;
	uint64_t req_cdt                      : 16;
#endif
	} s;
	struct cvmx_pnbx_psm_diag_s           cnf75xx;
};
typedef union cvmx_pnbx_psm_diag cvmx_pnbx_psm_diag_t;

/**
 * cvmx_pnb#_psm_push_arb_wt
 *
 * This register specifies the arbitration weight for PSM writes to SMEM
 * (from WRSTS commands). Note that PSM writes always have LOW priority.
 * All PSM requests are sered by PNB0, and the value in
 * PNB(1)_PSM_PUSH_ARB_WT has no significance.
 */
union cvmx_pnbx_psm_push_arb_wt {
	uint64_t u64;
	struct cvmx_pnbx_psm_push_arb_wt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t hp_wgt                       : 6;  /**< N/A */
	uint64_t reserved_6_15                : 10;
	uint64_t lp_wgt                       : 6;  /**< Arbitration weight for PSM writes to SMEM. */
#else
	uint64_t lp_wgt                       : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t hp_wgt                       : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_pnbx_psm_push_arb_wt_s    cnf75xx;
};
typedef union cvmx_pnbx_psm_push_arb_wt cvmx_pnbx_psm_push_arb_wt_t;

/**
 * cvmx_pnb#_smem_diag0
 *
 * Counts number of SMEM push requests
 *
 */
union cvmx_pnbx_smem_diag0 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsh_cdt_rd                   : 32; /**< number of SMEM read returned credits received */
	uint64_t psh_cmd_rd                   : 32; /**< number of SMEM read commands sent */
#else
	uint64_t psh_cmd_rd                   : 32;
	uint64_t rsh_cdt_rd                   : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag0_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag0 cvmx_pnbx_smem_diag0_t;

/**
 * cvmx_pnb#_smem_diag1
 *
 * Counts number of SMEM push requests
 *
 */
union cvmx_pnbx_smem_diag1 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsh_cdt_wr                   : 32; /**< number of SMEM write returned credits received */
	uint64_t psh_cmd_wr                   : 32; /**< number of SMEM write commands sent */
#else
	uint64_t psh_cmd_wr                   : 32;
	uint64_t rsh_cdt_wr                   : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag1_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag1 cvmx_pnbx_smem_diag1_t;

/**
 * cvmx_pnb#_smem_diag2
 *
 * Counts number of SMEM push requests
 *
 */
union cvmx_pnbx_smem_diag2 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsh_pp_wr                    : 32; /**< number of PP writes */
	uint64_t psh_pp_rd                    : 32; /**< number of PP reads */
#else
	uint64_t psh_pp_rd                    : 32;
	uint64_t rsh_pp_wr                    : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag2_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag2 cvmx_pnbx_smem_diag2_t;

/**
 * cvmx_pnb#_smem_diag3
 *
 * Counts number of SMEM push requests
 *
 */
union cvmx_pnbx_smem_diag3 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsh_dma_wr                   : 32; /**< number of DMA writes */
	uint64_t psh_dma_rd                   : 32; /**< number of DMA reads */
#else
	uint64_t psh_dma_rd                   : 32;
	uint64_t rsh_dma_wr                   : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag3_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag3 cvmx_pnbx_smem_diag3_t;

/**
 * cvmx_pnb#_smem_diag4
 *
 * Counts number of SMEM push requests
 *
 */
union cvmx_pnbx_smem_diag4 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_psm_wr                   : 32; /**< number of PSM write responses */
	uint64_t psh_psm_wr                   : 32; /**< number of PSM status writes */
#else
	uint64_t psh_psm_wr                   : 32;
	uint64_t rsp_psm_wr                   : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag4_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag4 cvmx_pnbx_smem_diag4_t;

/**
 * cvmx_pnb#_smem_diag5
 *
 * Counts number of SMEM pull responses
 *
 */
union cvmx_pnbx_smem_diag5 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_wr                       : 32; /**< number of write responses in general */
	uint64_t rsp_rd                       : 32; /**< number of read responses in general */
#else
	uint64_t rsp_rd                       : 32;
	uint64_t rsp_wr                       : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag5_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag5 cvmx_pnbx_smem_diag5_t;

/**
 * cvmx_pnb#_smem_diag6
 *
 * Counts number of SMEM pull responses
 *
 */
union cvmx_pnbx_smem_diag6 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag6_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_pp_wr                    : 32; /**< number of PP write responses */
	uint64_t rsp_pp_rd                    : 32; /**< number of PP read responses */
#else
	uint64_t rsp_pp_rd                    : 32;
	uint64_t rsp_pp_wr                    : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag6_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag6 cvmx_pnbx_smem_diag6_t;

/**
 * cvmx_pnb#_smem_diag7
 *
 * Counts number of SMEM pull responses
 *
 */
union cvmx_pnbx_smem_diag7 {
	uint64_t u64;
	struct cvmx_pnbx_smem_diag7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsp_dma_wr                   : 32; /**< number of DMA write responses */
	uint64_t rsp_dma_rd                   : 32; /**< number of DMA read responses */
#else
	uint64_t rsp_dma_rd                   : 32;
	uint64_t rsp_dma_wr                   : 32;
#endif
	} s;
	struct cvmx_pnbx_smem_diag7_s         cnf75xx;
};
typedef union cvmx_pnbx_smem_diag7 cvmx_pnbx_smem_diag7_t;

/**
 * cvmx_pnb#_smem_push_bushog_max
 *
 * This register specifies the bus-hog limit when arbitrating for requests to
 * SMEM from the cnMIPS cores, the PNBD DMA engines, and PSM WRSTS requests.
 */
union cvmx_pnbx_smem_push_bushog_max {
	uint64_t u64;
	struct cvmx_pnbx_smem_push_bushog_max_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t bushog_max                   : 8;  /**< The maximum number of consecutive high priority requests. */
#else
	uint64_t bushog_max                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_pnbx_smem_push_bushog_max_s cnf75xx;
};
typedef union cvmx_pnbx_smem_push_bushog_max cvmx_pnbx_smem_push_bushog_max_t;

/**
 * cvmx_pnb#_smemrd_dbe_info
 *
 * This register contains details of the source of an ECC DBE during an SMEM
 * read request.
 */
union cvmx_pnbx_smemrd_dbe_info {
	uint64_t u64;
	struct cvmx_pnbx_smemrd_dbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_21_63               : 43;
	uint64_t smemrd_ncbo_rdrsp_dbe        : 1;  /**< DBE occured in PNB response FIFO. */
	uint64_t smemrd_pnb_dbe               : 1;  /**< DBE occured in SMEM RAM. */
	uint64_t smemrd_dbe_src               : 3;  /**< Source of the SMEM read request:
                                                         0x0 = PNBD MHAB.
                                                         0x4 = cnMIPS request. */
	uint64_t smemrd_dbe_ctx               : 16; /**< N/A */
#else
	uint64_t smemrd_dbe_ctx               : 16;
	uint64_t smemrd_dbe_src               : 3;
	uint64_t smemrd_pnb_dbe               : 1;
	uint64_t smemrd_ncbo_rdrsp_dbe        : 1;
	uint64_t reserved_21_63               : 43;
#endif
	} s;
	struct cvmx_pnbx_smemrd_dbe_info_s    cnf75xx;
};
typedef union cvmx_pnbx_smemrd_dbe_info cvmx_pnbx_smemrd_dbe_info_t;

/**
 * cvmx_pnb#_smemrd_sbe_info
 *
 * This register contains details of the source of an ECC SBE during an SMEM
 * read request.
 */
union cvmx_pnbx_smemrd_sbe_info {
	uint64_t u64;
	struct cvmx_pnbx_smemrd_sbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_21_63               : 43;
	uint64_t smemrd_ncbo_rdrsp_sbe        : 1;  /**< SBE occured in PNB response FIFO. */
	uint64_t smemrd_pnb_sbe               : 1;  /**< SBE occured in SMEM RAM. */
	uint64_t smemrd_sbe_src               : 3;  /**< Source of the SMEM read request:
                                                         0x0 = PNBD MHAB.
                                                         0x4 = cnMIPS request. */
	uint64_t smemrd_sbe_ctx               : 16; /**< N/A */
#else
	uint64_t smemrd_sbe_ctx               : 16;
	uint64_t smemrd_sbe_src               : 3;
	uint64_t smemrd_pnb_sbe               : 1;
	uint64_t smemrd_ncbo_rdrsp_sbe        : 1;
	uint64_t reserved_21_63               : 43;
#endif
	} s;
	struct cvmx_pnbx_smemrd_sbe_info_s    cnf75xx;
};
typedef union cvmx_pnbx_smemrd_sbe_info cvmx_pnbx_smemrd_sbe_info_t;

/**
 * cvmx_pnb#_smemwr_dbe_info
 *
 * This register contains details of the source of an ECC DBE during an SMEM
 * write request.
 */
union cvmx_pnbx_smemwr_dbe_info {
	uint64_t u64;
	struct cvmx_pnbx_smemwr_dbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t smemwr_pnb_dbe               : 1;  /**< DBE occured in SMEM RAM. */
	uint64_t smemwr_dbe_src               : 3;  /**< Source of the SMEM write request:
                                                         0x0 = PNBD MHAB.
                                                         0x4 = cnMIPS request. */
	uint64_t smemwr_dbe_ctx               : 16; /**< N/A */
#else
	uint64_t smemwr_dbe_ctx               : 16;
	uint64_t smemwr_dbe_src               : 3;
	uint64_t smemwr_pnb_dbe               : 1;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_pnbx_smemwr_dbe_info_s    cnf75xx;
};
typedef union cvmx_pnbx_smemwr_dbe_info cvmx_pnbx_smemwr_dbe_info_t;

/**
 * cvmx_pnb#_smemwr_sbe_info
 *
 * This register contains details of the source of an ECC SBE during an SMEM
 * write request.
 */
union cvmx_pnbx_smemwr_sbe_info {
	uint64_t u64;
	struct cvmx_pnbx_smemwr_sbe_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t smemwr_pnb_sbe               : 1;  /**< DBE occured in SMEM RAM. */
	uint64_t smemwr_sbe_src               : 3;  /**< Source of the SMEM write request:
                                                         0x0 = PNBD MHAB.
                                                         0x4 = cnMIPS request. */
	uint64_t smemwr_sbe_ctx               : 16; /**< N/A */
#else
	uint64_t smemwr_sbe_ctx               : 16;
	uint64_t smemwr_sbe_src               : 3;
	uint64_t smemwr_pnb_sbe               : 1;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_pnbx_smemwr_sbe_info_s    cnf75xx;
};
typedef union cvmx_pnbx_smemwr_sbe_info cvmx_pnbx_smemwr_sbe_info_t;

#endif
