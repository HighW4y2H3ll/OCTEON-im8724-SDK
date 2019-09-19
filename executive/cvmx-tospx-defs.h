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
 * cvmx-tospx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon tospx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_TOSPX_DEFS_H__
#define __CVMX_TOSPX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_1PPS_GEN_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_1PPS_GEN_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0198ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_1PPS_GEN_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0198ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_1PPS_SAMPLE_CNT_OFFSET(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_1PPS_SAMPLE_CNT_OFFSET(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0208ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_1PPS_SAMPLE_CNT_OFFSET(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0208ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_1PPS_VERIF_GEN_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_1PPS_VERIF_GEN_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0220ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_1PPS_VERIF_GEN_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0220ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_1PPS_VERIF_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_1PPS_VERIF_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0228ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_1PPS_VERIF_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0228ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_CONF(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_CONF(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0020ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_CONF(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0020ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_CONF2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_CONF2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0038ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_CONF2(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0038ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_CSR_CTL_GPO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_CSR_CTL_GPO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0980ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_CSR_CTL_GPO(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0980ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_ACX0_TRANSFER_SIZE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_ACX0_TRANSFER_SIZE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4A18ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_ACX0_TRANSFER_SIZE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4A18ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_ACX1_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_ACX1_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0018ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_ACX1_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0018ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_FIFO_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_FIFO_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4A00ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_FIFO_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4A00ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_GEN_PURP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_GEN_PURP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4820ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_GEN_PURP(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4820ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_IS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_IS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4818ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_IS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4818ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_ISM(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_ISM(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4800ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_ISM(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4800ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_LOAD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_LOAD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4A10ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_LOAD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4A10ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC0_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC0_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0010ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC0_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0010ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC1_FIFO_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC1_FIFO_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B5200ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC1_FIFO_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B5200ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC1_GEN_PURP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC1_GEN_PURP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B5020ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC1_GEN_PURP(offset) (CVMX_ADD_IO_SEG(0x00011800D53B5020ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC1_LOAD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC1_LOAD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B5210ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC1_LOAD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B5210ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_AXC1_TRANSFER_SIZE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_AXC1_TRANSFER_SIZE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B5218ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_AXC1_TRANSFER_SIZE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B5218ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_CORRECT_ADJ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_CORRECT_ADJ(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01D8ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_CORRECT_ADJ(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01D8ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_IF_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_IF_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0068ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_IF_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0068ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_LEAD_LAG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_LEAD_LAG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0048ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_LEAD_LAG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0048ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_OFFSET(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_OFFSET(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01B0ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_OFFSET(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01B0ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_OFFSET_ADJ_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_OFFSET_ADJ_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0218ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_OFFSET_ADJ_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0218ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_SAMPLE_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_SAMPLE_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4850ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_SAMPLE_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4850ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_SYNC_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_SYNC_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4988ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_SYNC_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4988ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_SYNC_VALUE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_SYNC_VALUE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4980ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_SYNC_VALUE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4980ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_TH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_TH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0828ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_TH(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0828ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_WIN_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_WIN_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4880ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_WIN_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4880ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_WIN_ENDX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_DL_WIN_ENDX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B4908ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_DL_WIN_ENDX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B4908ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_WIN_STARTX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_DL_WIN_STARTX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B4888ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_DL_WIN_STARTX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B4888ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_DL_WIN_UPD_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_DL_WIN_UPD_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4878ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_DL_WIN_UPD_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4878ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_FIRS_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_FIRS_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0988ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_FIRS_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0988ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_FRAME_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_FRAME_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0060ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_FRAME_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0060ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_FRAME_L(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_FRAME_L(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0028ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_FRAME_L(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0028ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_GPOX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_GPOX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B0830ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_GPOX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B0830ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_INT_CTRL_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_INT_CTRL_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8018ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_INT_CTRL_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8018ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_INT_CTRL_STATUS_SHADOW(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_INT_CTRL_STATUS_SHADOW(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8038ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_INT_CTRL_STATUS_SHADOW(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8038ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_MAX_SAMPLE_ADJ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_MAX_SAMPLE_ADJ(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01B8ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_MAX_SAMPLE_ADJ(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01B8ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_MIN_SAMPLE_ADJ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_MIN_SAMPLE_ADJ(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01C0ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_MIN_SAMPLE_ADJ(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01C0ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_NUM_DL_WIN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_NUM_DL_WIN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B4830ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_NUM_DL_WIN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B4830ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_NUM_UL_WIN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_NUM_UL_WIN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0030ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_NUM_UL_WIN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0030ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_PWM_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_PWM_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0300ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_PWM_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0300ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_PWM_HIGH_TIME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_PWM_HIGH_TIME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0308ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_PWM_HIGH_TIME(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0308ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_PWM_LOW_TIME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_PWM_LOW_TIME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0310ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_PWM_LOW_TIME(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0310ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_RD_TIMER64_LSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_RD_TIMER64_LSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0358ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_RD_TIMER64_LSB(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0358ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_RD_TIMER64_MSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_RD_TIMER64_MSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0360ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_RD_TIMER64_MSB(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0360ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_REAL_TIME_TIMER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_REAL_TIME_TIMER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0190ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_REAL_TIME_TIMER(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0190ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_RF_CLK_TIMER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_RF_CLK_TIMER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0328ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_RF_CLK_TIMER(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0328ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_RF_CLK_TIMER_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_RF_CLK_TIMER_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0330ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_RF_CLK_TIMER_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0330ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SAMPLE_ADJ_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SAMPLE_ADJ_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01C8ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SAMPLE_ADJ_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01C8ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SAMPLE_ADJ_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SAMPLE_ADJ_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0200ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SAMPLE_ADJ_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0200ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SAMPLE_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SAMPLE_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0050ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SAMPLE_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0050ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SKIP_FRM_CNT_BITS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SKIP_FRM_CNT_BITS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0888ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SKIP_FRM_CNT_BITS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0888ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_CMDSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 63)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_SPI_CMDSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B1000ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_SPI_CMDSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B1000ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_CMD_ATTRX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 63)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_SPI_CMD_ATTRX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B1400ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_SPI_CMD_ATTRX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B1400ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_CONF0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_CONF0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0850ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_CONF0(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0850ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_CONF1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_CONF1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0858ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_CONF1(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0858ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8010ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8010ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_DINX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 63)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_SPI_DINX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B1200ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_SPI_DINX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B1200ull) + (((offset) & 63) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_LLX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_SPI_LLX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B0860ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_SPI_LLX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B0860ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_RX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_RX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8000ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_RX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8000ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8020ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8020ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_SPI_TX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_SPI_TX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B8008ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_SPI_TX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D53B8008ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TIMER64_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TIMER64_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0340ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TIMER64_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0340ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TIMER64_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TIMER64_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0338ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TIMER64_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0338ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TTI_SCNT_INTX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 7)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_TTI_SCNT_INTX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B0280ull) + (((offset) & 7) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_TTI_SCNT_INTX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B0280ull) + (((offset) & 7) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TTI_SCNT_INT_CLR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TTI_SCNT_INT_CLR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0230ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TTI_SCNT_INT_CLR(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0230ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TTI_SCNT_INT_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TTI_SCNT_INT_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0248ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TTI_SCNT_INT_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0248ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TTI_SCNT_INT_MAP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TTI_SCNT_INT_MAP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0240ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TTI_SCNT_INT_MAP(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0240ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_TTI_SCNT_INT_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_TTI_SCNT_INT_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0238ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_TTI_SCNT_INT_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0238ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC0_FIFO_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC0_FIFO_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0A00ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC0_FIFO_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0A00ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC0_LOAD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC0_LOAD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0A10ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC0_LOAD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0A10ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC0_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC0_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0000ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC0_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0000ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC0_TRANSFER_SIZE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC0_TRANSFER_SIZE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0A18ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC0_TRANSFER_SIZE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0A18ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC1_FIFO_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC1_FIFO_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B3200ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC1_FIFO_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B3200ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC1_GEN_PURP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC1_GEN_PURP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B3020ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC1_GEN_PURP(offset) (CVMX_ADD_IO_SEG(0x00011800D53B3020ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC1_LOAD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC1_LOAD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B3210ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC1_LOAD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B3210ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC1_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC1_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0008ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC1_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0008ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_AXC1_TRANSFER_SIZE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_AXC1_TRANSFER_SIZE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B3218ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_AXC1_TRANSFER_SIZE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B3218ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_CORRECT_ADJ(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_CORRECT_ADJ(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01D0ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_CORRECT_ADJ(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01D0ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_IF_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_IF_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0070ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_IF_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0070ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_IS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_IS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0818ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_IS(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0818ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_ISM(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_ISM(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0800ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_ISM(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0800ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_LEAD_LAG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_LEAD_LAG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0040ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_LEAD_LAG(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0040ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_OFFSET(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_OFFSET(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B01A8ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_OFFSET(offset) (CVMX_ADD_IO_SEG(0x00011800D53B01A8ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_OFFSET_ADJ_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_OFFSET_ADJ_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0210ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_OFFSET_ADJ_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0210ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_SYNC_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_SYNC_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0188ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_SYNC_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0188ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_SYNC_VALUE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_SYNC_VALUE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0180ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_SYNC_VALUE(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0180ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_TH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_TH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0820ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_TH(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0820ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_WIN_EN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_WIN_EN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0080ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_WIN_EN(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0080ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_WIN_ENDX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_UL_WIN_ENDX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B0108ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_UL_WIN_ENDX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B0108ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_WIN_STARTX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_TOSPX_UL_WIN_STARTX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53B0088ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_TOSPX_UL_WIN_STARTX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53B0088ull) + (((offset) & 3) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_UL_WIN_UPD_SCNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_UL_WIN_UPD_SCNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0078ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_UL_WIN_UPD_SCNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0078ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_WR_TIMER64_LSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_WR_TIMER64_LSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0348ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_WR_TIMER64_LSB(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0348ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOSPX_WR_TIMER64_MSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_TOSPX_WR_TIMER64_MSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53B0350ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_TOSPX_WR_TIMER64_MSB(offset) (CVMX_ADD_IO_SEG(0x00011800D53B0350ull) + ((offset) & 3) * 0x10000ull)
#endif

/**
 * cvmx_tosp#_1pps_gen_cfg
 */
union cvmx_tospx_1pps_gen_cfg {
	uint64_t u64;
	struct cvmx_tospx_1pps_gen_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< Enable 1PPS tracking.
                                                         0 = 1PPS signal not tracked.
                                                         1 = 1PPS signal tracked.
                                                         For CNF75XX, 1PPS should be enabled once to perform an initial
                                                         synchronization, and then it should be disabled during normal
                                                         operation to allow RFIF to perform timing alignment. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_1pps_gen_cfg_s      cnf75xx;
};
typedef union cvmx_tospx_1pps_gen_cfg cvmx_tospx_1pps_gen_cfg_t;

/**
 * cvmx_tosp#_1pps_sample_cnt_offset
 */
union cvmx_tospx_1pps_sample_cnt_offset {
	uint64_t u64;
	struct cvmx_tospx_1pps_sample_cnt_offset_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t offset                       : 20; /**< This register holds the sample count at which the 1PPS
                                                         was received.
                                                         Upon reset, the sample counter starts at 0 when the
                                                         first 1PPS is received and then increments to wrap
                                                         around at FRAME_L-1. At each subsequent 1PPS, a
                                                         snapshot of the sample counter is taken and the count
                                                         is made available via this register. This enables
                                                         software to monitor the RF clock drift relative to
                                                         the 1PPS signal. */
#else
	uint64_t offset                       : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_1pps_sample_cnt_offset_s cnf75xx;
};
typedef union cvmx_tospx_1pps_sample_cnt_offset cvmx_tospx_1pps_sample_cnt_offset_t;

/**
 * cvmx_tosp#_1pps_verif_gen_en
 */
union cvmx_tospx_1pps_verif_gen_en {
	uint64_t u64;
	struct cvmx_tospx_1pps_verif_gen_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< 1PPS generation for verification purposes:
                                                         0 = Disabled (default).
                                                         1 = Enabled.
                                                         Note that the BPHY_1PPS signals is ignored when [ENA]=1. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_1pps_verif_gen_en_s cnf75xx;
};
typedef union cvmx_tospx_1pps_verif_gen_en cvmx_tospx_1pps_verif_gen_en_t;

/**
 * cvmx_tosp#_1pps_verif_scnt
 */
union cvmx_tospx_1pps_verif_scnt {
	uint64_t u64;
	struct cvmx_tospx_1pps_verif_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< When TOSP()_1PPS_VERIF_GEN_EN[ENA]=1, the internal 1PPS is generated
                                                         when the sample count is [CNT]. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_1pps_verif_scnt_s   cnf75xx;
};
typedef union cvmx_tospx_1pps_verif_scnt cvmx_tospx_1pps_verif_scnt_t;

/**
 * cvmx_tosp#_conf
 */
union cvmx_tospx_conf {
	uint64_t u64;
	struct cvmx_tospx_conf_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t loopback                     : 1;  /**< FDD loop back mode:
                                                         0 = Not in loopback mode (default).
                                                         1 = Loops back the DL ouput to the UL input inside the TOSP block.
                                                         [DUPLEX] must be set to TDD mode (0). */
	uint64_t mol                          : 1;  /**< Manual override lock. When locked, writes to [MAN_CTRL], [ENA_CTRL],
                                                         and [TXNRX_CTRL] have no effect.
                                                         0 = Locked.
                                                         1 = Unlock. */
	uint64_t upd_style                    : 1;  /**< DL and UL window update style.
                                                         0 = Update immediately when written to the register.
                                                         1 = Update at the specified time by registers TOSP()_UL_WIN_UPD_SCNT
                                                         and TOSP()_DL_WIN_UPD_SCNT. (recommended) */
	uint64_t diversity                    : 1;  /**< UL AXC1 disable (used to support FDD SISO with CLK
                                                         4x).
                                                         0 = Data gets written to the diversity FIFO in MIMO mode
                                                         (default).
                                                         1 = No data written to the diversity FIFO in MIMO mode. */
	uint64_t duplex                       : 1;  /**< Division duplex mode.
                                                         0 = TDD (default).
                                                         1 = FDD. */
	uint64_t prod_type                    : 1;  /**< Product type.
                                                         0 = UE (not supported).
                                                         1 = eNB, enables using 1PPS synchronization scheme.
                                                         Must be set to 1. CNF75XX only supports eNB mode. */
	uint64_t txnrx_ctrl                   : 1;  /**< RFIC IF TXnRX signal pulse control.
                                                         When [MOL]=1 and [MAN_CTRL]=1, this bit directly controls the output
                                                         on the RF_[a]_TXNRX signal. */
	uint64_t ena_ctrl                     : 1;  /**< RFIC IF ENABLE signal pulse control.
                                                         When [MOL]=1 and [MAN_CTRL]=1, this bit directly controls the output
                                                         on the RF_[a]_ENABLE signal. */
	uint64_t man_ctrl                     : 1;  /**< RF IC manual control enable.
                                                         Setting this bit to 1
                                                         enables manual control of the RF_[a]_TXNRX and RF_[a]_ENABLE signals.
                                                         When set to 0 (default), these
                                                         are automatically controlled when opening and closing
                                                         UL/DL windows. The manual mode is used to initialize
                                                         the RF IC in alert mode.
                                                         This bit can only be written when [MOL]=1. */
	uint64_t ul_int_en                    : 1;  /**< UL AXC0 interrupt mask enable.
                                                         0 = Receives interrupts.
                                                         1 = Doesn't receive interrupts, needs to poll ISRs. */
	uint64_t adi_en                       : 1;  /**< ADI enable signal pulsed or leveled behavior.
                                                         0 = Pulsed.
                                                         1 = Leveled. */
	uint64_t clr_fifo_of                  : 1;  /**< Write 1 to clear the UL AxC 0 FIFO overflow flag
                                                         TOSP()_UL_AXC0_STATUS[FIFO_OF]. This bit auto-clears to zero. */
	uint64_t clr_fifo_ur                  : 1;  /**< Write 1 to clear the UL AxC 0 FIFO under-run flag
                                                         TOSP()_UL_AXC0_STATUS[FIFO_UF]. This bit auto-clears to zero. */
	uint64_t ind_ctrl_mode                : 1;  /**< Indepenedent control mode. When this bit is cleared, TOSP provides
                                                         independent control for the UL and DL data flows to support AD9361 FDD
                                                         independent control mode. The RF_[a]_ENABLE signal acts as RX_CONTROL,
                                                         and the RF_[a]_TXNRX acts as TX_CONTROL.
                                                         0 = Independent control mode.
                                                         1 = Regular mode. */
	uint64_t flush                        : 1;  /**< Flush UL AxC 0 FIFO (auto clear register). */
	uint64_t inv                          : 1;  /**< Invert data bit ordering on the DIQ bus.
                                                         0 = Normal ordering.
                                                         1 = Reversed bit order (i.e., bit 0 -> bit 11, etc.). */
	uint64_t mode                         : 1;  /**< UL MIMO/SISO mode of operation.
                                                         0 = SISO.
                                                         1 = MIMO. */
	uint64_t enable                       : 1;  /**< Enable the interface after the 1PPS synchronization:
                                                         0 = Disable.
                                                         1 = Enable. */
#else
	uint64_t enable                       : 1;
	uint64_t mode                         : 1;
	uint64_t inv                          : 1;
	uint64_t flush                        : 1;
	uint64_t ind_ctrl_mode                : 1;
	uint64_t clr_fifo_ur                  : 1;
	uint64_t clr_fifo_of                  : 1;
	uint64_t adi_en                       : 1;
	uint64_t ul_int_en                    : 1;
	uint64_t man_ctrl                     : 1;
	uint64_t ena_ctrl                     : 1;
	uint64_t txnrx_ctrl                   : 1;
	uint64_t prod_type                    : 1;
	uint64_t duplex                       : 1;
	uint64_t diversity                    : 1;
	uint64_t upd_style                    : 1;
	uint64_t mol                          : 1;
	uint64_t loopback                     : 1;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_tospx_conf_s              cnf75xx;
};
typedef union cvmx_tospx_conf cvmx_tospx_conf_t;

/**
 * cvmx_tosp#_conf2
 */
union cvmx_tospx_conf2 {
	uint64_t u64;
	struct cvmx_tospx_conf2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t latency                      : 1;  /**< RF data variable latency. This feature supports older RF ICs that had
                                                         fixed latency between an ENABLE pulse and data output. For newer RF
                                                         ICs that use separate RXFRAME and TXFRAME signal to support variable
                                                         latency.
                                                         0 = Fixed latency (prior to AD9361).
                                                         1 = Variable latency using RF2_[a]_RXFRAME and RF2_[a]_TXFRAME (recommended). */
	uint64_t iq_cfg                       : 1;  /**< IQ port configuration.
                                                         0 = Single port (10 MHz bandwidth and lower).
                                                         1 = Dual ports (more then 10 MHz bandwidth). */
	uint64_t behavior                     : 1;  /**< UL and DL frame signal behavior. This bit controls the behavior of the
                                                         RF2_x_TXFRAME and RF2_x_RXFRAME signals.
                                                         0 = Pulsed for one cycle every frame (not recommended).
                                                         1 = Level mode, i.e., asserted during active periods, de-asserted
                                                         during idle periods. */
#else
	uint64_t behavior                     : 1;
	uint64_t iq_cfg                       : 1;
	uint64_t latency                      : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tospx_conf2_s             cnf75xx;
};
typedef union cvmx_tospx_conf2 cvmx_tospx_conf2_t;

/**
 * cvmx_tosp#_csr_ctl_gpo
 */
union cvmx_tospx_csr_ctl_gpo {
	uint64_t u64;
	struct cvmx_tospx_csr_ctl_gpo_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t val                          : 4;  /**< Values to output to the RF_[a]_GPO<3:0> ports. */
#else
	uint64_t val                          : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_csr_ctl_gpo_s       cnf75xx;
};
typedef union cvmx_tospx_csr_ctl_gpo cvmx_tospx_csr_ctl_gpo_t;

/**
 * cvmx_tosp#_dl_acx0_transfer_size
 */
union cvmx_tospx_dl_acx0_transfer_size {
	uint64_t u64;
	struct cvmx_tospx_dl_acx0_transfer_size_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t size                         : 18; /**< Indicates the size of the DMA data transfer via the
                                                         HMI IF to the rf_if DL FIFO. */
#else
	uint64_t size                         : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_tospx_dl_acx0_transfer_size_s cnf75xx;
};
typedef union cvmx_tospx_dl_acx0_transfer_size cvmx_tospx_dl_acx0_transfer_size_t;

/**
 * cvmx_tosp#_dl_acx1_status
 */
union cvmx_tospx_dl_acx1_status {
	uint64_t u64;
	struct cvmx_tospx_dl_acx1_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t rfic_ena                     : 1;  /**< RFIC enabled (in alert state). This bit is set when the RF_[a]_ENABLE
                                                         signal was pulsed using the manual overide TOSP()_CONF[ENA_CTRL] bit,
                                                         in order to put the RFIC in alert state. The RFIC must be in this
                                                         state before normal operation can start. */
	uint64_t sync_late                    : 1;  /**< Reserved. */
	uint64_t reserved_19_20               : 2;
	uint64_t thresh_rch                   : 1;  /**< Reserved. */
	uint64_t fifo_of                      : 1;  /**< FIFO overflow for DL AxC 1. This bit can be cleared by writing 1 to
                                                         TOSP()_DL_AXC1_GEN_PURP[OVER_CLR]. */
	uint64_t fifo_ur                      : 1;  /**< FIFO under-run for DL AxC 1. This bit can be cleared by writing 1 to
                                                         TOSP()_DL_AXC1_GEN_PURP[UNDER_CLR]. */
	uint64_t ul_dl_acx1_sm                : 4;  /**< N/A */
	uint64_t hab_req_sm                   : 4;  /**< N/A */
	uint64_t reserved_0_7                 : 8;
#else
	uint64_t reserved_0_7                 : 8;
	uint64_t hab_req_sm                   : 4;
	uint64_t ul_dl_acx1_sm                : 4;
	uint64_t fifo_ur                      : 1;
	uint64_t fifo_of                      : 1;
	uint64_t thresh_rch                   : 1;
	uint64_t reserved_19_20               : 2;
	uint64_t sync_late                    : 1;
	uint64_t rfic_ena                     : 1;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_tospx_dl_acx1_status_s    cnf75xx;
};
typedef union cvmx_tospx_dl_acx1_status cvmx_tospx_dl_acx1_status_t;

/**
 * cvmx_tosp#_dl_axc0_fifo_cnt
 */
union cvmx_tospx_dl_axc0_fifo_cnt {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_fifo_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t cnt                          : 13; /**< DL AxC0 FIFO fill level. This register can take values
                                                         between 0 and 2560. */
#else
	uint64_t cnt                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_fifo_cnt_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_fifo_cnt cvmx_tospx_dl_axc0_fifo_cnt_t;

/**
 * cvmx_tosp#_dl_axc0_gen_purp
 */
union cvmx_tospx_dl_axc0_gen_purp {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_gen_purp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t int_mask                     : 1;  /**< DL AXC0 interrupt mask.
                                                         0 = Enables interrupts.
                                                         1 = Disables interrupts, ISR polling available. */
	uint64_t reserved_3_7                 : 5;
	uint64_t over_clr                     : 1;  /**< DL AXC0 FIFO overflow clear, set to 1 to clear the overflow. */
	uint64_t under_clr                    : 1;  /**< DL_AXC0 FIFO under-run clear, set to 1 to clear under-run. */
	uint64_t fifo_flush                   : 1;  /**< DL AXC0 FIFO FLUSH, set to 1 to flush. */
#else
	uint64_t fifo_flush                   : 1;
	uint64_t under_clr                    : 1;
	uint64_t over_clr                     : 1;
	uint64_t reserved_3_7                 : 5;
	uint64_t int_mask                     : 1;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_gen_purp_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_gen_purp cvmx_tospx_dl_axc0_gen_purp_t;

/**
 * cvmx_tosp#_dl_axc0_is
 *
 * This register reports DL interrupt status. The bit mapping matches the
 * mapping used for the interrupt enables in TOSP()_DL_AXC0_ISM.
 *
 * Note that status is only reported for those bits which are enabled in
 * TOSP()_DL_AXC0_ISM.
 *
 * The register contents are cleared on a read.
 */
union cvmx_tospx_dl_axc0_is {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_is_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t isr                          : 12; /**< DL interrupt status register (clear on read). */
#else
	uint64_t isr                          : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_is_s        cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_is cvmx_tospx_dl_axc0_is_t;

/**
 * cvmx_tosp#_dl_axc0_ism
 *
 * This register enables TOSP interrupts. Except for the DL_AXC* interrupts,
 * the other interrupts may be enabled either in TOSP()_UL_ISM or
 * TOSP()_DL_AXC0_ISM, or both. If enabled in both, then it should be cleared in
 * both TOSP()_UL_IS and TOSP()_DL_AXC0_IS.
 *
 * See TOSP()_UL_ISM for more details on the interrupts common to both
 * registers.
 */
union cvmx_tospx_dl_axc0_ism {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_ism_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t pps_sync_done                : 1;  /**< PPS sync done interrupt enable. */
	uint64_t spi_skipped                  : 1;  /**< Enable reporting of SPI event skipped status. */
	uint64_t spi_xfer_done_3              : 1;  /**< SPI transfer done 3 interrupt enable. */
	uint64_t spi_xfer_done_2              : 1;  /**< SPI transfer done 2 interrupt enable. */
	uint64_t spi_xfer_done_1              : 1;  /**< SPI transfer done 1 interrupt enable. */
	uint64_t spi_xfer_done_0              : 1;  /**< SPI transfer done 0 interrupt enable. */
	uint64_t st_dl_frame                  : 1;  /**< Start of TX frame interrupt enable. */
	uint64_t st_ul_frame                  : 1;  /**< Start of UL frame interrupt enable. */
	uint64_t dl_axc1_flags                : 1;  /**< DL AXC1 FIFO flags interrupt enable. */
	uint64_t dl_axc1_thresh               : 1;  /**< Reserved. */
	uint64_t dl_axc0_flags                : 1;  /**< DL AXC0 FIFO flags interrupt enable. */
	uint64_t dl_axc0_thresh               : 1;  /**< Reerved. */
#else
	uint64_t dl_axc0_thresh               : 1;
	uint64_t dl_axc0_flags                : 1;
	uint64_t dl_axc1_thresh               : 1;
	uint64_t dl_axc1_flags                : 1;
	uint64_t st_ul_frame                  : 1;
	uint64_t st_dl_frame                  : 1;
	uint64_t spi_xfer_done_0              : 1;
	uint64_t spi_xfer_done_1              : 1;
	uint64_t spi_xfer_done_2              : 1;
	uint64_t spi_xfer_done_3              : 1;
	uint64_t spi_skipped                  : 1;
	uint64_t pps_sync_done                : 1;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_ism_s       cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_ism cvmx_tospx_dl_axc0_ism_t;

/**
 * cvmx_tosp#_dl_axc0_load_cfg
 */
union cvmx_tospx_dl_axc0_load_cfg {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_load_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t load                         : 1;  /**< Indicates to load and execute the programmed DMA
                                                         transfer size (DL_TRANSFER_SIZE) via the HMI IF to the
                                                         rf_if DL AXC0 FIFO. */
#else
	uint64_t load                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_load_cfg_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_load_cfg cvmx_tospx_dl_axc0_load_cfg_t;

/**
 * cvmx_tosp#_dl_axc0_status
 */
union cvmx_tospx_dl_axc0_status {
	uint64_t u64;
	struct cvmx_tospx_dl_axc0_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t rfic_ena                     : 1;  /**< RFIC enabled (in alert state). This bit is set when the RF_[a]_ENABLE
                                                         signal was pulsed using the manual overide TOSP()_CONF[ENA_CTRL] bit,
                                                         in order to put the RFIC in alert state. The RFIC must be in this
                                                         state before normal operation can start. */
	uint64_t sync_late                    : 1;  /**< Reserved. */
	uint64_t reserved_19_20               : 2;
	uint64_t thresh_rch                   : 1;  /**< Reserved. */
	uint64_t fifo_of                      : 1;  /**< FIFO overflow for DL AxC 0. This bit can be cleared by writing 1 to
                                                         TOSP()_DL_AXC0_GEN_PURP[OVER_CLR]. */
	uint64_t fifo_ur                      : 1;  /**< FIFO under-run for DL AxC 0. This bit can be cleared by writing 1 to
                                                         TOSP()_DL_AXC0_GEN_PURP[UNDER_CLR]. */
	uint64_t ul_dl_axc0_sm                : 4;  /**< N/A */
	uint64_t hab_req_sm                   : 4;  /**< N/A */
	uint64_t reserved_0_7                 : 8;
#else
	uint64_t reserved_0_7                 : 8;
	uint64_t hab_req_sm                   : 4;
	uint64_t ul_dl_axc0_sm                : 4;
	uint64_t fifo_ur                      : 1;
	uint64_t fifo_of                      : 1;
	uint64_t thresh_rch                   : 1;
	uint64_t reserved_19_20               : 2;
	uint64_t sync_late                    : 1;
	uint64_t rfic_ena                     : 1;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_tospx_dl_axc0_status_s    cnf75xx;
};
typedef union cvmx_tospx_dl_axc0_status cvmx_tospx_dl_axc0_status_t;

/**
 * cvmx_tosp#_dl_axc1_fifo_cnt
 */
union cvmx_tospx_dl_axc1_fifo_cnt {
	uint64_t u64;
	struct cvmx_tospx_dl_axc1_fifo_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t cnt                          : 13; /**< DL AXC1 FIFO fill level. This register can take values
                                                         between 0 and 2560. */
#else
	uint64_t cnt                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_dl_axc1_fifo_cnt_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc1_fifo_cnt cvmx_tospx_dl_axc1_fifo_cnt_t;

/**
 * cvmx_tosp#_dl_axc1_gen_purp
 */
union cvmx_tospx_dl_axc1_gen_purp {
	uint64_t u64;
	struct cvmx_tospx_dl_axc1_gen_purp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t over_clr                     : 1;  /**< DL AXC1 FIFO overflow clear, set to 1 to clear */
	uint64_t under_clr                    : 1;  /**< DL AXC1 FIFO under run clear, set to 1 to clear */
	uint64_t fifo_flush                   : 1;  /**< DL AXC1 FIFO FLUSH, set to 1 to flush */
#else
	uint64_t fifo_flush                   : 1;
	uint64_t under_clr                    : 1;
	uint64_t over_clr                     : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tospx_dl_axc1_gen_purp_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc1_gen_purp cvmx_tospx_dl_axc1_gen_purp_t;

/**
 * cvmx_tosp#_dl_axc1_load_cfg
 */
union cvmx_tospx_dl_axc1_load_cfg {
	uint64_t u64;
	struct cvmx_tospx_dl_axc1_load_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t load                         : 1;  /**< Indicates to load and execute the programmed DMA
                                                         transfer size (DL_AXC0_TRANSFER_SIZE) via the HMI IF to
                                                         the rf_if DL AXC1 FIFO. */
#else
	uint64_t load                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_dl_axc1_load_cfg_s  cnf75xx;
};
typedef union cvmx_tospx_dl_axc1_load_cfg cvmx_tospx_dl_axc1_load_cfg_t;

/**
 * cvmx_tosp#_dl_axc1_transfer_size
 */
union cvmx_tospx_dl_axc1_transfer_size {
	uint64_t u64;
	struct cvmx_tospx_dl_axc1_transfer_size_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t size                         : 18; /**< Indicates the size of the DMA data transfer via the
                                                         HMI IF to the rf_if DL AXC1 FIFO. */
#else
	uint64_t size                         : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_tospx_dl_axc1_transfer_size_s cnf75xx;
};
typedef union cvmx_tospx_dl_axc1_transfer_size cvmx_tospx_dl_axc1_transfer_size_t;

/**
 * cvmx_tosp#_dl_correct_adj
 */
union cvmx_tospx_dl_correct_adj {
	uint64_t u64;
	struct cvmx_tospx_dl_correct_adj_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t offset                       : 4;  /**< Indicates the sample counter offset at which the DL
                                                         samples are dropped or added. This register can take
                                                         a values from 0 to 15 and should be configured as
                                                         follow:
                                                         5 : when MIN_SAMPLE_ADJ = 1
                                                         8 : when MIN_SAMPLE_ADJ = 2
                                                         9 : when MIN_SAMPLE_ADJ = 4 */
#else
	uint64_t offset                       : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_dl_correct_adj_s    cnf75xx;
};
typedef union cvmx_tospx_dl_correct_adj cvmx_tospx_dl_correct_adj_t;

/**
 * cvmx_tosp#_dl_if_cfg
 */
union cvmx_tospx_dl_if_cfg {
	uint64_t u64;
	struct cvmx_tospx_dl_if_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t mode                         : 1;  /**< DL communication mode.
                                                         0 = SISO (default).
                                                         1 = MIMO. */
	uint64_t dis_sch                      : 1;  /**< This bit controls the output driven for a disabled DL AxC. This
                                                         feature is only available when using DL SISO and UL MIMO.
                                                         0 = Constant 0 for debugging (default).
                                                         1 = Same as previous cycle to minimize IO switching. */
	uint64_t antenna                      : 2;  /**< Transmit on antenna A and/or B (DL SISO/UL MIMO
                                                         feature only)
                                                         0x0 = Transmit on antenna A (default).
                                                         0x1 = Transmit on antenna B.
                                                         0x2 = Transmit on A and B.
                                                         0x3 = Reserved. */
#else
	uint64_t antenna                      : 2;
	uint64_t dis_sch                      : 1;
	uint64_t mode                         : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_dl_if_cfg_s         cnf75xx;
};
typedef union cvmx_tospx_dl_if_cfg cvmx_tospx_dl_if_cfg_t;

/**
 * cvmx_tosp#_dl_lead_lag
 */
union cvmx_tospx_dl_lead_lag {
	uint64_t u64;
	struct cvmx_tospx_dl_lead_lag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t dl_lag                       : 12; /**< The number of RF clock cycles after the last IQ sample transmission to
                                                         delay before turning off the RF IC downlink chain. This is done to
                                                         prevent truncating the DL samples in process when transmission stops. */
	uint64_t dl_lead                      : 12; /**< The number of RF clock cycles to precondition the RF IC before
                                                         transmitting data. If no lead time is used, the signal will have some initial
                                                         distortion that will impact decoding. */
#else
	uint64_t dl_lead                      : 12;
	uint64_t dl_lag                       : 12;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_dl_lead_lag_s       cnf75xx;
};
typedef union cvmx_tospx_dl_lead_lag cvmx_tospx_dl_lead_lag_t;

/**
 * cvmx_tosp#_dl_offset
 */
union cvmx_tospx_dl_offset {
	uint64_t u64;
	struct cvmx_tospx_dl_offset_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dl_offset                    : 20; /**< Indicates the number of RF clock cycles after the
                                                         BPHY_1PPS pulse is received before the start of the DL
                                                         frame.
                                                         This should be set to 0x4 for CNF75XX, and RFIF should be used to
                                                         perform timing alignment. */
#else
	uint64_t dl_offset                    : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_offset_s         cnf75xx;
};
typedef union cvmx_tospx_dl_offset cvmx_tospx_dl_offset_t;

/**
 * cvmx_tosp#_dl_offset_adj_scnt
 */
union cvmx_tospx_dl_offset_adj_scnt {
	uint64_t u64;
	struct cvmx_tospx_dl_offset_adj_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Indicates the DL sample count at which the 1PPS
                                                         incremental adjustments will be applied to the internal
                                                         sample counter. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_offset_adj_scnt_s cnf75xx;
};
typedef union cvmx_tospx_dl_offset_adj_scnt cvmx_tospx_dl_offset_adj_scnt_t;

/**
 * cvmx_tosp#_dl_sample_cnt
 */
union cvmx_tospx_dl_sample_cnt {
	uint64_t u64;
	struct cvmx_tospx_dl_sample_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Current DL sample count modulo FRAME_L. The start of frame is
                                                         aligned with count 0. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_sample_cnt_s     cnf75xx;
};
typedef union cvmx_tospx_dl_sample_cnt cvmx_tospx_dl_sample_cnt_t;

/**
 * cvmx_tosp#_dl_sync_scnt
 */
union cvmx_tospx_dl_sync_scnt {
	uint64_t u64;
	struct cvmx_tospx_dl_sync_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Sample count at which the DL start of frame reference
                                                         will be modified as described with TOSP()_DL_SYNC_VALUE. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_sync_scnt_s      cnf75xx;
};
typedef union cvmx_tospx_dl_sync_scnt cvmx_tospx_dl_sync_scnt_t;

/**
 * cvmx_tosp#_dl_sync_value
 */
union cvmx_tospx_dl_sync_value {
	uint64_t u64;
	struct cvmx_tospx_dl_sync_value_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t val                          : 20; /**< Value added to the UL sample count to obtain the DL
                                                         sample count. It must be a positive value smaller then
                                                         FRAME_L. (Only used for UE, not eNB products.) */
#else
	uint64_t val                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_sync_value_s     cnf75xx;
};
typedef union cvmx_tospx_dl_sync_value cvmx_tospx_dl_sync_value_t;

/**
 * cvmx_tosp#_dl_th
 */
union cvmx_tospx_dl_th {
	uint64_t u64;
	struct cvmx_tospx_dl_th_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t thr                          : 13; /**< FIFO level reached before granting a DL DMA request.
                                                         This DL FIFO fill level threshold can be used
                                                         in a few ways:
                                                         1- When the FIFO fill level reaches the threshold,
                                                         there is enough data in the FIFO to start the data
                                                         transfer, so it grants a DMA transfer from the DL FIFO
                                                         to the HAB's memory.
                                                         2- It can also be used to generate an interrupt
                                                         when the FIFO threshold is reached.
                                                         3- It can be set to FIFO_DEPTH-1 to permit samples
                                                         transfers as soon as there is room for one sample. */
#else
	uint64_t thr                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_dl_th_s             cnf75xx;
};
typedef union cvmx_tospx_dl_th cvmx_tospx_dl_th_t;

/**
 * cvmx_tosp#_dl_win_en
 */
union cvmx_tospx_dl_win_en {
	uint64_t u64;
	struct cvmx_tospx_dl_win_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t enable                       : 4;  /**< Transmit windows enable. When a bit is set, the corresponding window is
                                                         enabled (e.g., bit 0 enables window 0). The default value enables all
                                                         windows. */
#else
	uint64_t enable                       : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_dl_win_en_s         cnf75xx;
};
typedef union cvmx_tospx_dl_win_en cvmx_tospx_dl_win_en_t;

/**
 * cvmx_tosp#_dl_win_end#
 */
union cvmx_tospx_dl_win_endx {
	uint64_t u64;
	struct cvmx_tospx_dl_win_endx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t end_cnt                      : 20; /**< End sample count plus 1 for DL window [b]. The maximum
                                                         value should be TOSP()_FRAME_L[FRAME_L], unless the window must stay
                                                         opened for ever.
                                                         Note that the end sample is exclusive, i.e,, sample [END_CNT] is not
                                                         included in the window. */
#else
	uint64_t end_cnt                      : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_win_endx_s       cnf75xx;
};
typedef union cvmx_tospx_dl_win_endx cvmx_tospx_dl_win_endx_t;

/**
 * cvmx_tosp#_dl_win_start#
 */
union cvmx_tospx_dl_win_startx {
	uint64_t u64;
	struct cvmx_tospx_dl_win_startx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t start_pnt                    : 20; /**< Start points for each of the four DL windows.
                                                         The start and end values have the following restrictions:
                                                         _ The first DL window must always start at 0.
                                                         _ The other start points must be greater than
                                                         TOSP()_DL_LEAD_LAG[DL_LEAD].
                                                         _ In TDD mode, the start point for each window must be less than the
                                                         end point for that window.
                                                         _ In TDD mode, UL windows have priority over DL windows.
                                                         _ There must a minimum of 7 samples between closing a window and
                                                         opening a new window. A minimum gap of 10 samples is recommended.
                                                         Different RF ICs may require a larger gap. */
#else
	uint64_t start_pnt                    : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_win_startx_s     cnf75xx;
};
typedef union cvmx_tospx_dl_win_startx cvmx_tospx_dl_win_startx_t;

/**
 * cvmx_tosp#_dl_win_upd_scnt
 */
union cvmx_tospx_dl_win_upd_scnt {
	uint64_t u64;
	struct cvmx_tospx_dl_win_upd_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t scnt                         : 20; /**< Downlink window update sample count. When TOSP()_CONF[UPD_STYLE]=1, the
                                                         following registers are updated when the sample count reaches [SCNT]:
                                                         TOSP()_DL_WIN_EN, TOSP()_DL_WIN_START(), TOSP()_DL_WIN_END(),
                                                         TOSP()_NUM_DL_WIN, TOSP()_FRAME_L, TOSP()_DL_LEAD_LAG. */
#else
	uint64_t scnt                         : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_dl_win_upd_scnt_s   cnf75xx;
};
typedef union cvmx_tospx_dl_win_upd_scnt cvmx_tospx_dl_win_upd_scnt_t;

/**
 * cvmx_tosp#_firs_enable
 *
 * The TOSP block does not have any filters.
 *
 */
union cvmx_tospx_firs_enable {
	uint64_t u64;
	struct cvmx_tospx_firs_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t dl_axc1_fil                  : 1;  /**< DL AXC1 filtering control bit.
                                                         0 = DL AXC1 filtering disabled.
                                                         1 = DL AXC1 filtering enabled. */
	uint64_t dl_axc0_fil                  : 1;  /**< DL AXC0 filtering control bit.
                                                         0 = DL AXC0 filtering disabled.
                                                         1 = DL AXC0 filtering enabled. */
	uint64_t ul_axc1_fil                  : 1;  /**< UL AXC1 filtering control bit.
                                                         0 = UL AXC1 filtering disabled.
                                                         1 = UL AXC1 filtering enabled. */
	uint64_t ul_axc0_fil                  : 1;  /**< UL AXC0 filtering control bit.
                                                         0 = UL AXC0 filtering disabled.
                                                         1 = UL AXC0 filtering enabled. */
#else
	uint64_t ul_axc0_fil                  : 1;
	uint64_t ul_axc1_fil                  : 1;
	uint64_t dl_axc0_fil                  : 1;
	uint64_t dl_axc1_fil                  : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_firs_enable_s       cnf75xx;
};
typedef union cvmx_tospx_firs_enable cvmx_tospx_firs_enable_t;

/**
 * cvmx_tosp#_frame_cnt
 */
union cvmx_tospx_frame_cnt {
	uint64_t u64;
	struct cvmx_tospx_frame_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Frame count modulo (1<<16). */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_frame_cnt_s         cnf75xx;
};
typedef union cvmx_tospx_frame_cnt cvmx_tospx_frame_cnt_t;

/**
 * cvmx_tosp#_frame_l
 */
union cvmx_tospx_frame_l {
	uint64_t u64;
	struct cvmx_tospx_frame_l_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t frame_l                      : 20; /**< Frame length in terms of RF clock cycles:
                                                         _ TDD SISO: FRAME_L = num_samples / num_ports
                                                         _ TDD MIMO: FRAME_L = (num_samples*2) / num_ports
                                                         _ FDD SISO: FRAME_L = (num_samples*2) / num_ports
                                                         _ FDD MIMO: FRAME_L = (num_samples*4) / num_ports */
#else
	uint64_t frame_l                      : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_frame_l_s           cnf75xx;
};
typedef union cvmx_tospx_frame_l cvmx_tospx_frame_l_t;

/**
 * cvmx_tosp#_gpo#
 *
 * This register can be used to output specific values on the GPO pins
 * when either DL or UL is active.
 */
union cvmx_tospx_gpox {
	uint64_t u64;
	struct cvmx_tospx_gpox_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t fall_val                     : 11; /**< Fall lag time. The output will be deasserted [FALL_VAL] cycles after
                                                         the UL/DL goes idle. */
	uint64_t rise_val                     : 11; /**< Signed value (lead/lag) on rising edge of level signal. Rise lag time. The output will be
                                                         asserted [RISE_VAL] cycles after the UL/DL goes idle. */
	uint64_t src                          : 2;  /**< Signal active high source:
                                                         0x0 = Idle.
                                                         0x1 = UL.
                                                         0x2 = DL.
                                                         0x3 = Idle.
                                                         When set to "Idle", the GPO outputs are controlled by the
                                                         TOSP()_CSR_CTL_GPO register. */
#else
	uint64_t src                          : 2;
	uint64_t rise_val                     : 11;
	uint64_t fall_val                     : 11;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_gpox_s              cnf75xx;
};
typedef union cvmx_tospx_gpox cvmx_tospx_gpox_t;

/**
 * cvmx_tosp#_int_ctrl_status
 *
 * This register reports SPI status for various conditions.
 *
 * When a given enable bit is set, the corresponding status bit will be set
 * when the condition occurs. All status bits are cleared on a read access.
 *
 * To read the state of the enables without clearing the status bits, the
 * enable states are aliased in the TOSP()_INT_CTRL_STATUS_SHADOW register.
 *
 * Note that no interrupts are generated when these status bits are set.
 */
union cvmx_tospx_int_ctrl_status {
	uint64_t u64;
	struct cvmx_tospx_int_ctrl_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gpint_en_7                   : 1;  /**< Reserved. */
	uint64_t gpint_en_6                   : 1;  /**< Reserved. */
	uint64_t gpint_en_5                   : 1;  /**< Reserved. */
	uint64_t gpint_en_4                   : 1;  /**< Reserved. */
	uint64_t gpint_en_3                   : 1;  /**< Reserved. */
	uint64_t gpint_en_2                   : 1;  /**< Reserved. */
	uint64_t gpint_en_1                   : 1;  /**< Reserved. */
	uint64_t gpint_en_0                   : 1;  /**< Reserved. */
	uint64_t reserved_23_23               : 1;
	uint64_t exwovren                     : 1;  /**< Extended write FIFO overflow reporting enable. */
	uint64_t exrovren                     : 1;  /**< Extended read FIFO overflow reporting enable. */
	uint64_t roen                         : 1;  /**< RX FIFO overflow reporting enable. */
	uint64_t rfen                         : 1;  /**< RX FIFO full reporting enable. */
	uint64_t reen                         : 1;  /**< RX FIFO empty reporting enable. */
	uint64_t tfen                         : 1;  /**< TX FIFO full reporting enable. */
	uint64_t teen                         : 1;  /**< TX FIFO empty reporting enable. */
	uint64_t gp_int7                      : 1;  /**< Reserved. */
	uint64_t gp_int6                      : 1;  /**< Reserved. */
	uint64_t gp_int5                      : 1;  /**< Reserved. */
	uint64_t gp_int4                      : 1;  /**< Reserved. */
	uint64_t gp_int3                      : 1;  /**< Reserved. */
	uint64_t gp_int2                      : 1;  /**< Reserved. */
	uint64_t gp_int1                      : 1;  /**< Reserved. */
	uint64_t gp_int0                      : 1;  /**< Reserved. */
	uint64_t reserved_7_7                 : 1;
	uint64_t exwund_int                   : 1;  /**< SPI extended write FIFO underflow occured (clear-on-read). */
	uint64_t exrovr_int                   : 1;  /**< SPI extended read FIFO overflow occurred (clear-on-read). */
	uint64_t ro                           : 1;  /**< SPI RX FIFO overflow occured (clear-on-read). */
	uint64_t rf                           : 1;  /**< SPI RX FIFO full occured (clear-on-read). */
	uint64_t re                           : 1;  /**< SPI RX FIFO empty occured (clear-on-read). */
	uint64_t tf                           : 1;  /**< SPI TX data FIFO full occured (clear-on-read). */
	uint64_t te                           : 1;  /**< SPI TX data FIFO is full (clear-on-read). */
#else
	uint64_t te                           : 1;
	uint64_t tf                           : 1;
	uint64_t re                           : 1;
	uint64_t rf                           : 1;
	uint64_t ro                           : 1;
	uint64_t exrovr_int                   : 1;
	uint64_t exwund_int                   : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t gp_int0                      : 1;
	uint64_t gp_int1                      : 1;
	uint64_t gp_int2                      : 1;
	uint64_t gp_int3                      : 1;
	uint64_t gp_int4                      : 1;
	uint64_t gp_int5                      : 1;
	uint64_t gp_int6                      : 1;
	uint64_t gp_int7                      : 1;
	uint64_t teen                         : 1;
	uint64_t tfen                         : 1;
	uint64_t reen                         : 1;
	uint64_t rfen                         : 1;
	uint64_t roen                         : 1;
	uint64_t exrovren                     : 1;
	uint64_t exwovren                     : 1;
	uint64_t reserved_23_23               : 1;
	uint64_t gpint_en_0                   : 1;
	uint64_t gpint_en_1                   : 1;
	uint64_t gpint_en_2                   : 1;
	uint64_t gpint_en_3                   : 1;
	uint64_t gpint_en_4                   : 1;
	uint64_t gpint_en_5                   : 1;
	uint64_t gpint_en_6                   : 1;
	uint64_t gpint_en_7                   : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_int_ctrl_status_s   cnf75xx;
};
typedef union cvmx_tospx_int_ctrl_status cvmx_tospx_int_ctrl_status_t;

/**
 * cvmx_tosp#_int_ctrl_status_shadow
 *
 * This register reports the values of the SPI status control enable bits in
 * TOSP()_INT_CTRL_STATUS. Software can use this register to read the
 * interrupt bits without clearing the status bits.
 */
union cvmx_tospx_int_ctrl_status_shadow {
	uint64_t u64;
	struct cvmx_tospx_int_ctrl_status_shadow_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gpint_en_7_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_6_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_5_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_4_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_3_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_2_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_1_stat              : 1;  /**< Reserved. */
	uint64_t gpint_en_0_stat              : 1;  /**< Reserved. */
	uint64_t reserved_23_23               : 1;
	uint64_t exwovren_stat                : 1;  /**< Extended write FIFO overflow reporting enable. */
	uint64_t exrovren_stat                : 1;  /**< Extended read FIFO overflow reporting enable. */
	uint64_t roen_stat                    : 1;  /**< SPI RX FIFO overflow reporting enable. */
	uint64_t rfen_stat                    : 1;  /**< SPI RX FIFO full reporting enable. */
	uint64_t reen_stat                    : 1;  /**< SPI RX FIFO not empty reporting enable. */
	uint64_t tfen_stat                    : 1;  /**< SPI TX FIFO full reporting enable. */
	uint64_t teen_stat                    : 1;  /**< SPI TX FIFO empty reporting enable. */
	uint64_t reserved_0_15                : 16;
#else
	uint64_t reserved_0_15                : 16;
	uint64_t teen_stat                    : 1;
	uint64_t tfen_stat                    : 1;
	uint64_t reen_stat                    : 1;
	uint64_t rfen_stat                    : 1;
	uint64_t roen_stat                    : 1;
	uint64_t exrovren_stat                : 1;
	uint64_t exwovren_stat                : 1;
	uint64_t reserved_23_23               : 1;
	uint64_t gpint_en_0_stat              : 1;
	uint64_t gpint_en_1_stat              : 1;
	uint64_t gpint_en_2_stat              : 1;
	uint64_t gpint_en_3_stat              : 1;
	uint64_t gpint_en_4_stat              : 1;
	uint64_t gpint_en_5_stat              : 1;
	uint64_t gpint_en_6_stat              : 1;
	uint64_t gpint_en_7_stat              : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_int_ctrl_status_shadow_s cnf75xx;
};
typedef union cvmx_tospx_int_ctrl_status_shadow cvmx_tospx_int_ctrl_status_shadow_t;

/**
 * cvmx_tosp#_max_sample_adj
 */
union cvmx_tospx_max_sample_adj {
	uint64_t u64;
	struct cvmx_tospx_max_sample_adj_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t num                          : 10; /**< Indicates the maximum number of samples that can be
                                                         adjusted per frame. Note the value to be programmed
                                                         varies with the mode of operation as follow:
                                                         [NUM] = num_samples*MIMO*FDD / NUM_PORTS
                                                         Where:
                                                         MIMO = 2 in MIMO mode and 1 otherwise.
                                                         FDD = 2 in FDD mode and 1 otherwise.
                                                         NUM_PORTS = 2 in dual port mode, 1 otherwise.
                                                         num_samples must be in the range 1 to 6. */
#else
	uint64_t num                          : 10;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_tospx_max_sample_adj_s    cnf75xx;
};
typedef union cvmx_tospx_max_sample_adj cvmx_tospx_max_sample_adj_t;

/**
 * cvmx_tosp#_min_sample_adj
 */
union cvmx_tospx_min_sample_adj {
	uint64_t u64;
	struct cvmx_tospx_min_sample_adj_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t num                          : 4;  /**< Indicates the minimum number of samples that can be
                                                         adjusted per frame. Note the value to be programmed
                                                         varies with the mode of operation as follow:
                                                         [NUM]  = num_samples*MIMO*FDD / NUM_PORTS
                                                         Where:
                                                         MIMO = 2 in MIMO mode and 1 otherwise.
                                                         FDD = 2 in FDD mode and 1 otherwise.
                                                         NUM_PORTS = 2 in dual port mode, 1 otherwise. */
#else
	uint64_t num                          : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_min_sample_adj_s    cnf75xx;
};
typedef union cvmx_tospx_min_sample_adj cvmx_tospx_min_sample_adj_t;

/**
 * cvmx_tosp#_num_dl_win
 */
union cvmx_tospx_num_dl_win {
	uint64_t u64;
	struct cvmx_tospx_num_dl_win_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t num                          : 3;  /**< Number of DL windows. Must be in the range [0,4], other values are
                                                         reserved. */
#else
	uint64_t num                          : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tospx_num_dl_win_s        cnf75xx;
};
typedef union cvmx_tospx_num_dl_win cvmx_tospx_num_dl_win_t;

/**
 * cvmx_tosp#_num_ul_win
 */
union cvmx_tospx_num_ul_win {
	uint64_t u64;
	struct cvmx_tospx_num_ul_win_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t num                          : 3;  /**< Number of UL windows. Must be in the range 0 to 4, other values are
                                                         reserved. */
#else
	uint64_t num                          : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tospx_num_ul_win_s        cnf75xx;
};
typedef union cvmx_tospx_num_ul_win cvmx_tospx_num_ul_win_t;

/**
 * cvmx_tosp#_pwm_enable
 */
union cvmx_tospx_pwm_enable {
	uint64_t u64;
	struct cvmx_tospx_pwm_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< PWM signal generation enable:
                                                         0 = PWM disabled.
                                                         1 = PWM enabled. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_pwm_enable_s        cnf75xx;
};
typedef union cvmx_tospx_pwm_enable cvmx_tospx_pwm_enable_t;

/**
 * cvmx_tosp#_pwm_high_time
 */
union cvmx_tospx_pwm_high_time {
	uint64_t u64;
	struct cvmx_tospx_pwm_high_time_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t hi_time                      : 24; /**< Number of PWM high cycle minus 1. Cycles are counted in BCLK/2. */
#else
	uint64_t hi_time                      : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_pwm_high_time_s     cnf75xx;
};
typedef union cvmx_tospx_pwm_high_time cvmx_tospx_pwm_high_time_t;

/**
 * cvmx_tosp#_pwm_low_time
 */
union cvmx_tospx_pwm_low_time {
	uint64_t u64;
	struct cvmx_tospx_pwm_low_time_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t lo_time                      : 24; /**< Number of PWM low cycle minus 1. Cycles are counted in BCLK/2. */
#else
	uint64_t lo_time                      : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_pwm_low_time_s      cnf75xx;
};
typedef union cvmx_tospx_pwm_low_time cvmx_tospx_pwm_low_time_t;

/**
 * cvmx_tosp#_rd_timer64_lsb
 */
union cvmx_tospx_rd_timer64_lsb {
	uint64_t u64;
	struct cvmx_tospx_rd_timer64_lsb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t val                          : 32; /**< Least significant 32 bits of the 64-bit timer.
                                                         Note the value written in WR_TIMER64_LSB is not
                                                         propagated until the timer is enabled. */
#else
	uint64_t val                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_rd_timer64_lsb_s    cnf75xx;
};
typedef union cvmx_tospx_rd_timer64_lsb cvmx_tospx_rd_timer64_lsb_t;

/**
 * cvmx_tosp#_rd_timer64_msb
 */
union cvmx_tospx_rd_timer64_msb {
	uint64_t u64;
	struct cvmx_tospx_rd_timer64_msb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t val                          : 32; /**< Most significant 32 bits of the 64-bit timer.
                                                         Note the value written in WR_TIMER64_MSB is not
                                                         propagated until the timer is enabled. */
#else
	uint64_t val                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_rd_timer64_msb_s    cnf75xx;
};
typedef union cvmx_tospx_rd_timer64_msb cvmx_tospx_rd_timer64_msb_t;

/**
 * cvmx_tosp#_real_time_timer
 */
union cvmx_tospx_real_time_timer {
	uint64_t u64;
	struct cvmx_tospx_real_time_timer_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t timer                        : 32; /**< N/A */
#else
	uint64_t timer                        : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_real_time_timer_s   cnf75xx;
};
typedef union cvmx_tospx_real_time_timer cvmx_tospx_real_time_timer_t;

/**
 * cvmx_tosp#_rf_clk_timer
 */
union cvmx_tospx_rf_clk_timer {
	uint64_t u64;
	struct cvmx_tospx_rf_clk_timer_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t timer                        : 32; /**< Timer running off the RF clock. The timer is enabled/disable by
                                                         TOSP()_RF_CLK_TIMER_EN. Once enabled, it is reset to zero and starts
                                                         counting at th first BPHY_1PPS pulse. It is incremented by one ever RF
                                                         clock cycle, and reset to zero every 30 seconds (i.e., every 30th
                                                         pulse of BPHY_1PPS). */
#else
	uint64_t timer                        : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_rf_clk_timer_s      cnf75xx;
};
typedef union cvmx_tospx_rf_clk_timer cvmx_tospx_rf_clk_timer_t;

/**
 * cvmx_tosp#_rf_clk_timer_en
 */
union cvmx_tospx_rf_clk_timer_en {
	uint64_t u64;
	struct cvmx_tospx_rf_clk_timer_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< RF clock 32-bit timer enable:
                                                         0 = Disabled.
                                                         1 = Enabled. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_rf_clk_timer_en_s   cnf75xx;
};
typedef union cvmx_tospx_rf_clk_timer_en cvmx_tospx_rf_clk_timer_en_t;

/**
 * cvmx_tosp#_sample_adj_cfg
 */
union cvmx_tospx_sample_adj_cfg {
	uint64_t u64;
	struct cvmx_tospx_sample_adj_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t adj                          : 1;  /**< Indicates whether samples must be added/removed from the
                                                         beginning or the end of the frame.
                                                         0 = Add/remove samples from the end of the frame
                                                         (default, must be used).
                                                         1 = Add/remove samples from the beginning of the frame. */
#else
	uint64_t adj                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_sample_adj_cfg_s    cnf75xx;
};
typedef union cvmx_tospx_sample_adj_cfg cvmx_tospx_sample_adj_cfg_t;

/**
 * cvmx_tosp#_sample_adj_error
 */
union cvmx_tospx_sample_adj_error {
	uint64_t u64;
	struct cvmx_tospx_sample_adj_error_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t offset                       : 24; /**< Count of the number of times the DL FIFO did not have
                                                         enough IQ samples to be dropped for a DL timing
                                                         adjustment.
                                                         Bits <7:0> = DL AXC0 FIFO sample adjustment error.
                                                         Bits <23:16> = DL AXC1 FIFO sample adjustment error. */
#else
	uint64_t offset                       : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_sample_adj_error_s  cnf75xx;
};
typedef union cvmx_tospx_sample_adj_error cvmx_tospx_sample_adj_error_t;

/**
 * cvmx_tosp#_sample_cnt
 */
union cvmx_tospx_sample_cnt {
	uint64_t u64;
	struct cvmx_tospx_sample_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Current sample count modulo FRAME_L. The start of frame is
                                                         aligned with count 0. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_sample_cnt_s        cnf75xx;
};
typedef union cvmx_tospx_sample_cnt cvmx_tospx_sample_cnt_t;

/**
 * cvmx_tosp#_skip_frm_cnt_bits
 */
union cvmx_tospx_skip_frm_cnt_bits {
	uint64_t u64;
	struct cvmx_tospx_skip_frm_cnt_bits_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t bits                         : 2;  /**< Indicates the number of sample count bits to skip, in
                                                         order to reduce the sample count update frequency and
                                                         permit a reliable clock crossing from the RF to the
                                                         SCLK clock domain. */
#else
	uint64_t bits                         : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_tospx_skip_frm_cnt_bits_s cnf75xx;
};
typedef union cvmx_tospx_skip_frm_cnt_bits cvmx_tospx_skip_frm_cnt_bits_t;

/**
 * cvmx_tosp#_spi_cmd_attr#
 *
 * These registers provide attributes for the SPI commands stored in
 * TOSP()_SPI_CMDS().
 */
union cvmx_tospx_spi_cmd_attrx {
	uint64_t u64;
	struct cvmx_tospx_spi_cmd_attrx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t slave                        : 1;  /**< Slave select (in case there are two RF IC chips). Note that slave
                                                         select 0 uses RF_[a]_SPI_EN, and slave select 1 uses RF_[a]_SPI_EN2.
                                                         0 = Slave 0.
                                                         1 = Slave 1. */
	uint64_t bytes                        : 1;  /**< Number of address bytes.
                                                         0 = 1 byte transfer mode.
                                                         1 = 2 bytes transfer mode. */
	uint64_t gen_int                      : 1;  /**< Generate an interrupt upon the SPI read command completion. This bit
                                                         should be set for the last command in each event.
                                                         0 = No interrupt generated.
                                                         1 = Interrupt generated. */
	uint64_t rw                           : 1;  /**< Read or write command:
                                                         0 = Read.
                                                         1 = Write. */
#else
	uint64_t rw                           : 1;
	uint64_t gen_int                      : 1;
	uint64_t bytes                        : 1;
	uint64_t slave                        : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_spi_cmd_attrx_s     cnf75xx;
};
typedef union cvmx_tospx_spi_cmd_attrx cvmx_tospx_spi_cmd_attrx_t;

/**
 * cvmx_tosp#_spi_cmds#
 */
union cvmx_tospx_spi_cmdsx {
	uint64_t u64;
	struct cvmx_tospx_spi_cmdsx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t word                         : 24; /**< SPI command word. */
#else
	uint64_t word                         : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_spi_cmdsx_s         cnf75xx;
};
typedef union cvmx_tospx_spi_cmdsx cvmx_tospx_spi_cmdsx_t;

/**
 * cvmx_tosp#_spi_conf0
 *
 * This register controls how many SPI commands are executed for each of four
 * events. To disable an event, set the number of commands to zero.
 */
union cvmx_tospx_spi_conf0 {
	uint64_t u64;
	struct cvmx_tospx_spi_conf0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t num_cmds3                    : 6;  /**< Number of SPI commands to transfer for event 3. */
	uint64_t num_cmds2                    : 6;  /**< Number of SPI commands to transfer for event 2. */
	uint64_t num_cmds1                    : 6;  /**< Number of SPI commands to transfer for event 1. */
	uint64_t num_cmds0                    : 6;  /**< Number of SPI commands to transfer for event 0. */
#else
	uint64_t num_cmds0                    : 6;
	uint64_t num_cmds1                    : 6;
	uint64_t num_cmds2                    : 6;
	uint64_t num_cmds3                    : 6;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_spi_conf0_s         cnf75xx;
};
typedef union cvmx_tospx_spi_conf0 cvmx_tospx_spi_conf0_t;

/**
 * cvmx_tosp#_spi_conf1
 *
 * This register controls which SPI commands TOSP executes for each event.
 *
 * For each event i, TOSP executes TOSP()_SPI_CONF0[NUM_CMDSi] consecutive commands
 * starting at command TOSP()_SPI_CONF1[STARTi]. The start value specifies an
 * offset into the command table accessed via TOSP()_SPI_CMDS().
 */
union cvmx_tospx_spi_conf1 {
	uint64_t u64;
	struct cvmx_tospx_spi_conf1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t start3                       : 6;  /**< SPI commands start address for event 3. */
	uint64_t start2                       : 6;  /**< SPI commands start address for event 2. */
	uint64_t start1                       : 6;  /**< SPI commands start address for event 1. */
	uint64_t start0                       : 6;  /**< SPI commands start address for event 0. */
#else
	uint64_t start0                       : 6;
	uint64_t start1                       : 6;
	uint64_t start2                       : 6;
	uint64_t start3                       : 6;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_spi_conf1_s         cnf75xx;
};
typedef union cvmx_tospx_spi_conf1 cvmx_tospx_spi_conf1_t;

/**
 * cvmx_tosp#_spi_ctrl
 */
union cvmx_tospx_spi_ctrl {
	uint64_t u64;
	struct cvmx_tospx_spi_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_28_63               : 36;
	uint64_t phy_act_over_wr              : 1;  /**< Set to 1 for software to issue SPI transactions while TOSP is active. Otherwise software
                                                         access is blocked.
                                                         TOSP event triggered SPI accesses should be disabled to avoid
                                                         collisions between hardware- and sofware- generated SPI commands. To
                                                         disable the TOSP SPI events, set the number of commands for each event
                                                         to 0 in TOSP()_SPI_CONF0. */
	uint64_t phase2                       : 1;  /**< Over-ride [PHASE] setting and instead launch and capture on the rising
                                                         edge. This can lead to improved timing and permit higher operating
                                                         frequency.
                                                         0 = Launch and capture as per [PHASE] setting.
                                                         1 = Launch on rising and capture on the rising edge. */
	uint64_t slave_select                 : 1;  /**< Slave select.
                                                         0 = Select slave 0.
                                                         1 = Select slave 1. */
	uint64_t sspol_1                      : 1;  /**< Sets the polarity for slave select 1 (RF_[a]_SPI_EN2).
                                                         0 = Slave select active low.
                                                         1 = Slave select active high. */
	uint64_t ext_dword_cnt                : 5;  /**< Extended read/write DWORD count (master mode only).
                                                         Must be in the range 0x1 to 0x10. */
	uint64_t extrwen                      : 1;  /**< Extended read/write enable (master mode).
                                                         0 = Disabled.
                                                         1 = Enabled. */
	uint64_t rwpol3w                      : 1;  /**< SPI read/write bit polarity in 3-wire mode.
                                                         0 = Read is 1, and write is 0.
                                                         1 = Read is 0, and write is 1. */
	uint64_t sdbidir                      : 1;  /**< Serial data bi-directional enable for 3-wire mode operation.
                                                         0 = Disabled (4-wire mode).
                                                         1 = Enabled (3-wire mode). */
	uint64_t fss                          : 1;  /**< Force slave select. When set to 1, the slave select signal [SLAVE] is
                                                         asserted. When set to 0, the slave select signals operate normally. */
	uint64_t rate                         : 3;  /**< SPI clock divider. The SPI clock frequency is equal to
                                                         BCLK/(2^([RATE]+2)). Note that TOSP operates at BCLK/2.
                                                         E.g., with a typical BCLK=800 MHz, setting [RATE]=3 results in a SPI
                                                         clock rate of 800 MHz/(2^5) = 25 MHz. */
	uint64_t ms                           : 1;  /**< SPI mode select.
                                                         0 = Slave mode (not supported).
                                                         1 = Master mode.
                                                         Must be set to 1. CN75XX only supports master mode. */
	uint64_t spien                        : 1;  /**< SPI module enable.
                                                         0 = Disable the SPI.
                                                         1 = Enable the SPI. */
	uint64_t lbc                          : 1;  /**< Loop back control.
                                                         0 = Loop back disabled.
                                                         1 = Loop back enabled.
                                                         In loop-back mode, SDO is looped back to SDI, so commands written to
                                                         TOSP()_SPI_TX_DATA can be read back from TOSP()_SPI_RX_DATA. */
	uint64_t sspol_0                      : 1;  /**< Sets the polarity for slave select 1 (RF_[a]_SPI_EN2).
                                                         0 = Slave select active low.
                                                         1 = Slave select active high. */
	uint64_t ssctl                        : 1;  /**< Slave waveform select.
                                                         0 = Slave select stays low between SPI bursts.
                                                         1 = Slave select stays pulses high between SPI bursts. */
	uint64_t phase                        : 1;  /**< Clock/data phase relationship, 4 wire mode only.
                                                         0 = Launch on falling and capture on rising, when POL=0.
                                                         1 = Launch on rising and capture on falling, when POL=0.
                                                         When [POL]=1, the behavior is reversed. */
	uint64_t pol                          : 1;  /**< SPI clock polarity.
                                                         0 = Normal (recommended when using AD9361 RF IC).
                                                         1 = Inverted. */
	uint64_t bit_count                    : 5;  /**< Length of the data transfer, in bits, minus 1. */
#else
	uint64_t bit_count                    : 5;
	uint64_t pol                          : 1;
	uint64_t phase                        : 1;
	uint64_t ssctl                        : 1;
	uint64_t sspol_0                      : 1;
	uint64_t lbc                          : 1;
	uint64_t spien                        : 1;
	uint64_t ms                           : 1;
	uint64_t rate                         : 3;
	uint64_t fss                          : 1;
	uint64_t sdbidir                      : 1;
	uint64_t rwpol3w                      : 1;
	uint64_t extrwen                      : 1;
	uint64_t ext_dword_cnt                : 5;
	uint64_t sspol_1                      : 1;
	uint64_t slave_select                 : 1;
	uint64_t phase2                       : 1;
	uint64_t phy_act_over_wr              : 1;
	uint64_t reserved_28_63               : 36;
#endif
	} s;
	struct cvmx_tospx_spi_ctrl_s          cnf75xx;
};
typedef union cvmx_tospx_spi_ctrl cvmx_tospx_spi_ctrl_t;

/**
 * cvmx_tosp#_spi_din#
 */
union cvmx_tospx_spi_dinx {
	uint64_t u64;
	struct cvmx_tospx_spi_dinx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t data                         : 16; /**< Right-aligned data read back from SPI commands. */
#else
	uint64_t data                         : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_tospx_spi_dinx_s          cnf75xx;
};
typedef union cvmx_tospx_spi_dinx cvmx_tospx_spi_dinx_t;

/**
 * cvmx_tosp#_spi_ll#
 *
 * These registers indicate the sample count when TOSP will start sending SPI
 * commands for SPI event [b].
 */
union cvmx_tospx_spi_llx {
	uint64_t u64;
	struct cvmx_tospx_spi_llx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t num                          : 20; /**< SPI event [a] start sample count. */
#else
	uint64_t num                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_spi_llx_s           cnf75xx;
};
typedef union cvmx_tospx_spi_llx cvmx_tospx_spi_llx_t;

/**
 * cvmx_tosp#_spi_rx_data
 */
union cvmx_tospx_spi_rx_data {
	uint64_t u64;
	struct cvmx_tospx_spi_rx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rd_data                      : 32; /**< SPI read data, right aligned. */
#else
	uint64_t rd_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_spi_rx_data_s       cnf75xx;
};
typedef union cvmx_tospx_spi_rx_data cvmx_tospx_spi_rx_data_t;

/**
 * cvmx_tosp#_spi_status
 */
union cvmx_tospx_spi_status {
	uint64_t u64;
	struct cvmx_tospx_spi_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t sr_state                     : 4;  /**< SPI state machine.
                                                         0x1 = INIT.
                                                         0x2 = IDLE.
                                                         0x3 = WAIT_FIFO.
                                                         0x4 = READ_FIFO.
                                                         0x5 = LOAD_SR.
                                                         0x6 = SHIFT_SR.
                                                         0x7 = WAIT_CLK.
                                                         0x8 = WAIT_FOR_SS. */
	uint64_t ul_fifo_lvl                  : 4;  /**< Level of SPI RX FIFO. Note that a value of 0xF indicates the FIFO
                                                         contains either 15 or 16 entries. */
	uint64_t dl_axc0_fifo_lvl             : 4;  /**< Level of SPI TX FIFO. Note that a value of 0xF indicates the FIFO
                                                         contains either 15 or 16 entries. */
#else
	uint64_t dl_axc0_fifo_lvl             : 4;
	uint64_t ul_fifo_lvl                  : 4;
	uint64_t sr_state                     : 4;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_tospx_spi_status_s        cnf75xx;
};
typedef union cvmx_tospx_spi_status cvmx_tospx_spi_status_t;

/**
 * cvmx_tosp#_spi_tx_data
 */
union cvmx_tospx_spi_tx_data {
	uint64_t u64;
	struct cvmx_tospx_spi_tx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cmd_word                     : 32; /**< SPI read or write command, formatted as follows:
                                                         _ Bit <31>: 1 = Write command, 0 = Read command.
                                                         _ Bits <23:16> = Address.
                                                         _ Bits <15:0> = Data.
                                                         The data is left-aligned, and the length of the transfer is determined
                                                         by TOSP()_SPI_CTRL[BIT_COUNT]. */
#else
	uint64_t cmd_word                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_spi_tx_data_s       cnf75xx;
};
typedef union cvmx_tospx_spi_tx_data cvmx_tospx_spi_tx_data_t;

/**
 * cvmx_tosp#_timer64_cfg
 */
union cvmx_tospx_timer64_cfg {
	uint64_t u64;
	struct cvmx_tospx_timer64_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t clks                         : 8;  /**< The 64-bit timer is incremented every [CLKS]+1 cycles of the RF clock.
                                                         The valid range is [3,255]. */
#else
	uint64_t clks                         : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tospx_timer64_cfg_s       cnf75xx;
};
typedef union cvmx_tospx_timer64_cfg cvmx_tospx_timer64_cfg_t;

/**
 * cvmx_tosp#_timer64_en
 */
union cvmx_tospx_timer64_en {
	uint64_t u64;
	struct cvmx_tospx_timer64_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< Enable for the 64-bit RF clock timer.
                                                         0 = Disabled.
                                                         1 = Enabled. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_timer64_en_s        cnf75xx;
};
typedef union cvmx_tospx_timer64_en cvmx_tospx_timer64_en_t;

/**
 * cvmx_tosp#_tti_scnt_int#
 */
union cvmx_tospx_tti_scnt_intx {
	uint64_t u64;
	struct cvmx_tospx_tti_scnt_intx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t intr                         : 20; /**< TTI Sample Count Interrupt:
                                                         Indicates the sample count of the selected reference
                                                         counter at which to generate an interrupt. */
#else
	uint64_t intr                         : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_tti_scnt_intx_s     cnf75xx;
};
typedef union cvmx_tospx_tti_scnt_intx cvmx_tospx_tti_scnt_intx_t;

/**
 * cvmx_tosp#_tti_scnt_int_clr
 */
union cvmx_tospx_tti_scnt_int_clr {
	uint64_t u64;
	struct cvmx_tospx_tti_scnt_int_clr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t cnt                          : 8;  /**< TTI sample count interrupt status register.
                                                         Writing 0x1 to clear the TTI_SCNT_INT_STAT, writing
                                                         0x2 to clear the TTI_SCNT_INT_STAT(1) and so on. */
#else
	uint64_t cnt                          : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tospx_tti_scnt_int_clr_s  cnf75xx;
};
typedef union cvmx_tospx_tti_scnt_int_clr cvmx_tospx_tti_scnt_int_clr_t;

/**
 * cvmx_tosp#_tti_scnt_int_en
 */
union cvmx_tospx_tti_scnt_int_en {
	uint64_t u64;
	struct cvmx_tospx_tti_scnt_int_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ena                          : 8;  /**< TTI Sample Counter Interrupt Enable:
                                                         Bit 0: 1  Enables TTI_SCNT_INT_0.
                                                         Bit 1: 1 Enables TTI_SCNT_INT_1.
                                                         - ...
                                                         Bit 7: 1  Enables TTI_SCNT_INT_7.
                                                         Note these interrupts are disabled by default (=0x00). */
#else
	uint64_t ena                          : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tospx_tti_scnt_int_en_s   cnf75xx;
};
typedef union cvmx_tospx_tti_scnt_int_en cvmx_tospx_tti_scnt_int_en_t;

/**
 * cvmx_tosp#_tti_scnt_int_map
 */
union cvmx_tospx_tti_scnt_int_map {
	uint64_t u64;
	struct cvmx_tospx_tti_scnt_int_map_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t map                          : 8;  /**< TTI Sample Count Interrupt Mapping to a Reference
                                                         Counter:
                                                         Indicates the reference counter the TTI Sample Count
                                                         Interrupts must be generated from. A value of 0
                                                         indicates the UL reference counter (default) and a
                                                         value of 1 indicates the DL reference counter. The
                                                         bit 0 is associated with TTI_SCNT_INT_0, the bit 1
                                                         is associated with TTI_SCNT_INT_1 and so on.
                                                         Note that This register has not effect in TDD mode,
                                                         only in FDD mode. */
#else
	uint64_t map                          : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tospx_tti_scnt_int_map_s  cnf75xx;
};
typedef union cvmx_tospx_tti_scnt_int_map cvmx_tospx_tti_scnt_int_map_t;

/**
 * cvmx_tosp#_tti_scnt_int_stat
 */
union cvmx_tospx_tti_scnt_int_stat {
	uint64_t u64;
	struct cvmx_tospx_tti_scnt_int_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t cnt                          : 8;  /**< TTI Sample Count Interrupt Status register:
                                                         Indicates if a TTI_SCNT_INT_X occurred (1) or not .
                                                         The bit 0 is associated with TTI_SCNT_INT_0 and so on
                                                         incrementally. */
#else
	uint64_t cnt                          : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tospx_tti_scnt_int_stat_s cnf75xx;
};
typedef union cvmx_tospx_tti_scnt_int_stat cvmx_tospx_tti_scnt_int_stat_t;

/**
 * cvmx_tosp#_ul_axc0_fifo_cnt
 */
union cvmx_tospx_ul_axc0_fifo_cnt {
	uint64_t u64;
	struct cvmx_tospx_ul_axc0_fifo_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t cnt                          : 13; /**< UL FIFO fill level. This register can take values
                                                         between 0 and 2048. */
#else
	uint64_t cnt                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_ul_axc0_fifo_cnt_s  cnf75xx;
};
typedef union cvmx_tospx_ul_axc0_fifo_cnt cvmx_tospx_ul_axc0_fifo_cnt_t;

/**
 * cvmx_tosp#_ul_axc0_load_cfg
 *
 * This register is left-over from 71xx, not used in 75xx.
 *
 */
union cvmx_tospx_ul_axc0_load_cfg {
	uint64_t u64;
	struct cvmx_tospx_ul_axc0_load_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t hidden                       : 1;  /**< Reserved. Must be 0. */
	uint64_t reserved_9_11                : 3;
	uint64_t alt_ant                      : 1;  /**< Send data alternating antenna 0 (first) and antenna 1
                                                         (second) data on the UL HMI interface when set to 1.
                                                         By default, only the data from antenna 0 is sent on
                                                         this interface. */
	uint64_t reserved_3_7                 : 5;
	uint64_t exe3                         : 1;  /**< N/A */
	uint64_t exe2                         : 1;  /**< Setting this bit to 1 indicates the RF_IF to load
                                                         and execute the programmed DMA transfer size (register
                                                         UL_TRANSFER_SIZE) from the FIFO to the ULFE HAB. */
	uint64_t exe1                         : 1;  /**< Setting this bit to 1 indicates the RF_IF to load
                                                         and execute the programmed DMA transfer size (register
                                                         UL_TRANSFER_SIZE) from the FIFO to the SMEM HAB. */
#else
	uint64_t exe1                         : 1;
	uint64_t exe2                         : 1;
	uint64_t exe3                         : 1;
	uint64_t reserved_3_7                 : 5;
	uint64_t alt_ant                      : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t hidden                       : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_ul_axc0_load_cfg_s  cnf75xx;
};
typedef union cvmx_tospx_ul_axc0_load_cfg cvmx_tospx_ul_axc0_load_cfg_t;

/**
 * cvmx_tosp#_ul_axc0_status
 */
union cvmx_tospx_ul_axc0_status {
	uint64_t u64;
	struct cvmx_tospx_ul_axc0_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t rfic_ena                     : 1;  /**< RFIC enabled (in alert state). This bit is set when the RF_[a]_ENABLE
                                                         signal was pulsed using the manual overide TOSP()_CONF[ENA_CTRL] bit,
                                                         in order to put the RFIC in alert state. The RFIC must be in this
                                                         state before normal operation can start. */
	uint64_t sync_late                    : 1;  /**< Reserved. */
	uint64_t reserved_19_20               : 2;
	uint64_t thresh_rch                   : 1;  /**< Reserved. */
	uint64_t fifo_of                      : 1;  /**< FIFO overflow for UL AxC 0. This bit can be cleared by writing 1 to
                                                         TOSP()_CONF[CLR_FIFO_OF]. */
	uint64_t fifo_ur                      : 1;  /**< FIFO under-run for UL AxC 0. This bit can be cleared by writing 1 to
                                                         TOSP()_CONF[CLR_FIFO_UR]. */
	uint64_t ul_dl_axc0_sm                : 4;  /**< N/A */
	uint64_t hab_req_sm                   : 4;  /**< N/A */
	uint64_t reserved_0_7                 : 8;
#else
	uint64_t reserved_0_7                 : 8;
	uint64_t hab_req_sm                   : 4;
	uint64_t ul_dl_axc0_sm                : 4;
	uint64_t fifo_ur                      : 1;
	uint64_t fifo_of                      : 1;
	uint64_t thresh_rch                   : 1;
	uint64_t reserved_19_20               : 2;
	uint64_t sync_late                    : 1;
	uint64_t rfic_ena                     : 1;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_tospx_ul_axc0_status_s    cnf75xx;
};
typedef union cvmx_tospx_ul_axc0_status cvmx_tospx_ul_axc0_status_t;

/**
 * cvmx_tosp#_ul_axc0_transfer_size
 */
union cvmx_tospx_ul_axc0_transfer_size {
	uint64_t u64;
	struct cvmx_tospx_ul_axc0_transfer_size_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t size                         : 18; /**< Indicates the number of IQ samples per antenna to be
                                                         transferred from the rf_if UL FIFO to the SMEM, ULFE
                                                         and/or RACH. The value should not be doubled when
                                                         alternating samples from both antennas to the SMEM,
                                                         ULFE and/or RACH. */
#else
	uint64_t size                         : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_tospx_ul_axc0_transfer_size_s cnf75xx;
};
typedef union cvmx_tospx_ul_axc0_transfer_size cvmx_tospx_ul_axc0_transfer_size_t;

/**
 * cvmx_tosp#_ul_axc1_fifo_cnt
 */
union cvmx_tospx_ul_axc1_fifo_cnt {
	uint64_t u64;
	struct cvmx_tospx_ul_axc1_fifo_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t cnt                          : 13; /**< UL AXC1 FIFO fill level. This register can take values
                                                         between 0 and 2048. */
#else
	uint64_t cnt                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_ul_axc1_fifo_cnt_s  cnf75xx;
};
typedef union cvmx_tospx_ul_axc1_fifo_cnt cvmx_tospx_ul_axc1_fifo_cnt_t;

/**
 * cvmx_tosp#_ul_axc1_gen_purp
 */
union cvmx_tospx_ul_axc1_gen_purp {
	uint64_t u64;
	struct cvmx_tospx_ul_axc1_gen_purp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t clr_ovflw                    : 1;  /**< UL AXC1 FIFO overflow clear.
                                                         0 = Do not clear.
                                                         1 = Clear the the overflow flag. */
	uint64_t clr_underrun                 : 1;  /**< UL AXC1 FIFO underrun clear.
                                                         0 = Do not clear.
                                                         1 = Clear the the underrun flag. */
	uint64_t ff_flush                     : 1;  /**< UL AXC1 FIFO flush.
                                                         0 = Do not flush.
                                                         1 = Flush the FIFO. */
#else
	uint64_t ff_flush                     : 1;
	uint64_t clr_underrun                 : 1;
	uint64_t clr_ovflw                    : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tospx_ul_axc1_gen_purp_s  cnf75xx;
};
typedef union cvmx_tospx_ul_axc1_gen_purp cvmx_tospx_ul_axc1_gen_purp_t;

/**
 * cvmx_tosp#_ul_axc1_load_cfg
 */
union cvmx_tospx_ul_axc1_load_cfg {
	uint64_t u64;
	struct cvmx_tospx_ul_axc1_load_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t exe                          : 1;  /**< Setting this bit to 1 indicates the RF_IF to load
                                                         and execute the programmed DMA transfer size (register
                                                         UL_AXC1_TRANSFER_SIZE) from the FIFO to a destination. */
#else
	uint64_t exe                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tospx_ul_axc1_load_cfg_s  cnf75xx;
};
typedef union cvmx_tospx_ul_axc1_load_cfg cvmx_tospx_ul_axc1_load_cfg_t;

/**
 * cvmx_tosp#_ul_axc1_status
 */
union cvmx_tospx_ul_axc1_status {
	uint64_t u64;
	struct cvmx_tospx_ul_axc1_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t rfic_ena                     : 1;  /**< RFIC enabled (in alert state). This bit is set when the RF_[a]_ENABLE
                                                         signal was pulsed using the manual overide TOSP()_CONF[ENA_CTRL] bit,
                                                         in order to put the RFIC in alert state. The RFIC must be in this
                                                         state before normal operation can start. */
	uint64_t sync_late                    : 1;  /**< Reserved. */
	uint64_t reserved_19_20               : 2;
	uint64_t thresh_rch                   : 1;  /**< Reserved. */
	uint64_t fifo_of                      : 1;  /**< FIFO overflow for UL AxC 1. This bit can be cleared by writing 1 to
                                                         TOSP()_UL_AXC1_GEN_PURP[CLR_OVFLW]. */
	uint64_t fifo_ur                      : 1;  /**< FIFO under-run for UL AxC 1. This bit can be cleared by writing 1 to
                                                         TOSP()_UL_AXC1_GEN_PURP[CLR_UNDERRUN]. */
	uint64_t ul_dl_axc0_sm                : 4;  /**< N/A */
	uint64_t hab_req_sm                   : 4;  /**< N/A */
	uint64_t reserved_0_7                 : 8;
#else
	uint64_t reserved_0_7                 : 8;
	uint64_t hab_req_sm                   : 4;
	uint64_t ul_dl_axc0_sm                : 4;
	uint64_t fifo_ur                      : 1;
	uint64_t fifo_of                      : 1;
	uint64_t thresh_rch                   : 1;
	uint64_t reserved_19_20               : 2;
	uint64_t sync_late                    : 1;
	uint64_t rfic_ena                     : 1;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_tospx_ul_axc1_status_s    cnf75xx;
};
typedef union cvmx_tospx_ul_axc1_status cvmx_tospx_ul_axc1_status_t;

/**
 * cvmx_tosp#_ul_axc1_transfer_size
 */
union cvmx_tospx_ul_axc1_transfer_size {
	uint64_t u64;
	struct cvmx_tospx_ul_axc1_transfer_size_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t size                         : 18; /**< Indicates the size of the DMA data transfer from the
                                                         rf_if UL AXC1 FIFO out via the HMI IF. */
#else
	uint64_t size                         : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_tospx_ul_axc1_transfer_size_s cnf75xx;
};
typedef union cvmx_tospx_ul_axc1_transfer_size cvmx_tospx_ul_axc1_transfer_size_t;

/**
 * cvmx_tosp#_ul_correct_adj
 */
union cvmx_tospx_ul_correct_adj {
	uint64_t u64;
	struct cvmx_tospx_ul_correct_adj_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t offset                       : 4;  /**< Indicates the sample counter offset for the last sample
                                                         flag insertion, which determines when the ul samples
                                                         are dropped or added. This register can take values
                                                         from 0 to 15 and should be configured as follow:
                                                         4 : when MIN_SAMPLE_ADJ = 1
                                                         5 : when MIN_SAMPLE_ADJ = 2
                                                         6 : when MIN_SAMPLE_ADJ = 4 */
#else
	uint64_t offset                       : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_ul_correct_adj_s    cnf75xx;
};
typedef union cvmx_tospx_ul_correct_adj cvmx_tospx_ul_correct_adj_t;

/**
 * cvmx_tosp#_ul_if_cfg
 */
union cvmx_tospx_ul_if_cfg {
	uint64_t u64;
	struct cvmx_tospx_ul_if_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t eorl                         : 1;  /**< Early or late TX_FRAME.
                                                         0 = Late TX_FRAME. The TX_FRAME asserts after the DL lead time and deasserts
                                                         before the DL lag.
                                                         1 = Early TX_FRAME. The TX_FRAME asserts [CAP_LAT] cycles after the
                                                         DL_ON/ENABLE and deasserts [CAP_LAT] cycles after the
                                                         DL_ON/ENABLE signal. */
	uint64_t half_lat                     : 1;  /**< Half cycle latency.
                                                         0 = Capture I and Q on the falling and rising edge of
                                                         the clock respectively.
                                                         1 = Capture I and Q on the rising and falling edge of
                                                         the clock respectively. */
	uint64_t cap_lat                      : 4;  /**< Enable-to-capture latency.
                                                         The data capture starts/stops [CAP_LAT] cycles after the enable pulse.
                                                         Must be greater than 0. */
#else
	uint64_t cap_lat                      : 4;
	uint64_t half_lat                     : 1;
	uint64_t eorl                         : 1;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_tospx_ul_if_cfg_s         cnf75xx;
};
typedef union cvmx_tospx_ul_if_cfg cvmx_tospx_ul_if_cfg_t;

/**
 * cvmx_tosp#_ul_is
 *
 * This register reports UL interrupt status. The bit mapping matches the
 * mapping used for the interrupt enables in TOSP()_UL_ISM.
 *
 * Note that status is only reported for those bits which are enabled in
 * TOSP()_UL_ISM.
 *
 * The register contents are cleared on a read.
 */
union cvmx_tospx_ul_is {
	uint64_t u64;
	struct cvmx_tospx_ul_is_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t int_src                      : 12; /**< UL interrupt status register (clear on read). */
#else
	uint64_t int_src                      : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_tospx_ul_is_s             cnf75xx;
};
typedef union cvmx_tospx_ul_is cvmx_tospx_ul_is_t;

/**
 * cvmx_tosp#_ul_ism
 *
 * This register enables TOSP interrupts and status reporting. Except for the UL_AXC* interrupts,
 * the other interrupts may be enabled either in TOSP()_UL_ISM or
 * TOSP()_DL_AXC0_ISM, or both. If enabled in both, then it should be cleared in
 * both TOSP()_UL_IS and TOSP()_DL_AXC0_IS.
 */
union cvmx_tospx_ul_ism {
	uint64_t u64;
	struct cvmx_tospx_ul_ism_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t pps_sync_done                : 1;  /**< PPS sync done interrupt enable. When enabled, a RFIF_PPS_SYNC_DONE[a]
                                                         is generated when TOSP syncs with BPHY_1PPS. */
	uint64_t spi_skipped                  : 1;  /**< The corresponding bit in TOSP()_UL_IS[INT_SRC] indicates when a SPI event was skipped
                                                         because TOSP was busy issuing SPI commands from an earlier event.
                                                         No interrupt is generated when a SPI event is skipped (regardless of the state of
                                                         [SPI_SKIPPED]), and software must poll TOSP()_UL_IS[INT_SRC] to detect if/when such an
                                                         event has occured. */
	uint64_t spi_xfer_done_3              : 1;  /**< SPI transfer done 3 interrupt enable. When enabled, the
                                                         RFIF_SPI_EVENT3_DONE_RMAC[a] interrupt is triggered by the completion
                                                         of a SPI read command which has set TOSP()_SPI_CMD_ATTR()[GEN_INT]=1
                                                         while executing the event 3 SPI commands. */
	uint64_t spi_xfer_done_2              : 1;  /**< SPI transfer done 2 interrupt enable. When enabled, the
                                                         RFIF_SPI_EVENT2_DONE_RMAC[a] interrupt is triggered by the completion
                                                         of a SPI read command which has set TOSP()_SPI_CMD_ATTR()[GEN_INT]=1
                                                         while executing the event 2 SPI commands. */
	uint64_t spi_xfer_done_1              : 1;  /**< SPI transfer done 1 interrupt enable. When enabled, the
                                                         RFIF_SPI_EVENT1_DONE_RMAC[a] interrupt is triggered by the completion
                                                         of a SPI read command which has set TOSP()_SPI_CMD_ATTR()[GEN_INT]=1
                                                         while executing the event 1 SPI commands. */
	uint64_t spi_xfer_done_0              : 1;  /**< SPI transfer done 0 interrupt enable. When enabled, the
                                                         RFIF_SPI_EVENT0_DONE_RMAC[a] interrupt is triggered by the completion
                                                         of a SPI read command which has set TOSP()_SPI_CMD_ATTR()[GEN_INT]=1
                                                         while executing the event 0 SPI commands. */
	uint64_t st_dl_frame                  : 1;  /**< Start of DL frame interrupt enable. When enabled, the
                                                         RFIF_START_TX_FRAME_RMAC[a] interrupt is triggered at the start of
                                                         each DL frame. */
	uint64_t st_ul_frame                  : 1;  /**< Start of UL frame interrupt enable. When enabled, the
                                                         RFIF_START_RX_FRAME_RMAC[a] interrupt is triggered at the start of
                                                         each UL frame. */
	uint64_t ul_axc1_flags                : 1;  /**< UL AXC1 FIFO flags interrupt enable. When enabled, the
                                                         RFIF_FIFO_FLAGS_RMAC[a] interrupt is triggered when there is a FIFO
                                                         overflow or under-run for UL AxC 1. */
	uint64_t ul_axc1_thresh               : 1;  /**< Reserved. */
	uint64_t ul_axc0_flags                : 1;  /**< UL AXC0 FIFO flags interrupt enable. When enabled, the
                                                         RFIF_FIFO_FLAGS_RMAC[a] interrupt is triggered when there is a FIFO
                                                         overflow or under-run for UL AxC 0. */
	uint64_t ul_axc0_thresh               : 1;  /**< Reserved. */
#else
	uint64_t ul_axc0_thresh               : 1;
	uint64_t ul_axc0_flags                : 1;
	uint64_t ul_axc1_thresh               : 1;
	uint64_t ul_axc1_flags                : 1;
	uint64_t st_ul_frame                  : 1;
	uint64_t st_dl_frame                  : 1;
	uint64_t spi_xfer_done_0              : 1;
	uint64_t spi_xfer_done_1              : 1;
	uint64_t spi_xfer_done_2              : 1;
	uint64_t spi_xfer_done_3              : 1;
	uint64_t spi_skipped                  : 1;
	uint64_t pps_sync_done                : 1;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_tospx_ul_ism_s            cnf75xx;
};
typedef union cvmx_tospx_ul_ism cvmx_tospx_ul_ism_t;

/**
 * cvmx_tosp#_ul_lead_lag
 */
union cvmx_tospx_ul_lead_lag {
	uint64_t u64;
	struct cvmx_tospx_ul_lead_lag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t ul_lag                       : 12; /**< Lag at the end of the UL window. The lag is expressed as an unsigned
                                                         number of RF clock cycles. This value should always be set to 0. */
	uint64_t ul_lead                      : 12; /**< The number of RF clock cycles to precondition the RF IC before receiving
                                                         data. If no lead time is used, the signal will have some initial
                                                         distortion that will impact decoding. */
#else
	uint64_t ul_lead                      : 12;
	uint64_t ul_lag                       : 12;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_tospx_ul_lead_lag_s       cnf75xx;
};
typedef union cvmx_tospx_ul_lead_lag cvmx_tospx_ul_lead_lag_t;

/**
 * cvmx_tosp#_ul_offset
 */
union cvmx_tospx_ul_offset {
	uint64_t u64;
	struct cvmx_tospx_ul_offset_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t ul_offset                    : 20; /**< Indicates the number of RF clock cycles after the
                                                         BPHY_1PPS pulse is received before the start of the UL
                                                         frame.
                                                         This should be set to 0x4 for CNF75XX, and RFIF should be used to
                                                         perform timing alignment. */
#else
	uint64_t ul_offset                    : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_offset_s         cnf75xx;
};
typedef union cvmx_tospx_ul_offset cvmx_tospx_ul_offset_t;

/**
 * cvmx_tosp#_ul_offset_adj_scnt
 */
union cvmx_tospx_ul_offset_adj_scnt {
	uint64_t u64;
	struct cvmx_tospx_ul_offset_adj_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Indicates the UL sample count at which the 1PPS
                                                         incremental adjustments will be applied to the internal
                                                         sample counter. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_offset_adj_scnt_s cnf75xx;
};
typedef union cvmx_tospx_ul_offset_adj_scnt cvmx_tospx_ul_offset_adj_scnt_t;

/**
 * cvmx_tosp#_ul_sync_scnt
 */
union cvmx_tospx_ul_sync_scnt {
	uint64_t u64;
	struct cvmx_tospx_ul_sync_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t cnt                          : 20; /**< Sample count at which the start of frame reference will
                                                         be modified as described with register 0x30. */
#else
	uint64_t cnt                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_sync_scnt_s      cnf75xx;
};
typedef union cvmx_tospx_ul_sync_scnt cvmx_tospx_ul_sync_scnt_t;

/**
 * cvmx_tosp#_ul_sync_value
 */
union cvmx_tospx_ul_sync_value {
	uint64_t u64;
	struct cvmx_tospx_ul_sync_value_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t val                          : 20; /**< UL synchronization offset value. This register
                                                         indicates the sample number at which the start of frame
                                                         must be moved to. This value must be smaller than
                                                         FRAME_L, but it cannot be negative. See below how the
                                                         sample count gets updated based on registers 0x30 and
                                                         0x31 at sample count UL_SYNC_VALUE.
                                                         If UL_SYNC_SCNT >= UL_SYNC_VALUE
                                                         sample_count = UL_SYNC_SCNT ? UL_SYNC_VALUE + 1
                                                         Else
                                                         sample_count = UL_SYNC_SCNT + FRAME_L ?
                                                         UL_SYNC_VALUE + 1
                                                         Note this is not used for eNB products, only for UE
                                                         products.
                                                         Note this register is automatically cleared after the
                                                         correction is applied. */
#else
	uint64_t val                          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_sync_value_s     cnf75xx;
};
typedef union cvmx_tospx_ul_sync_value cvmx_tospx_ul_sync_value_t;

/**
 * cvmx_tosp#_ul_th
 */
union cvmx_tospx_ul_th {
	uint64_t u64;
	struct cvmx_tospx_ul_th_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t thr                          : 13; /**< FIFO level reached before granting a UL DMA request.
                                                         This UL FIFO fill level threshold can be used
                                                         in a few ways:
                                                         1. When the FIFO fill level reaches the threshold,
                                                         there is enough data in the FIFO to start the data
                                                         transfer, so it grants a DMA transfer from the UL FIFO
                                                         to the HAB's memory.
                                                         2- It can also be used to generate an interrupt to
                                                         when the FIFO threshold is reached.
                                                         3- It can be set to 1 to permit samples transfers
                                                         as soon as one sample is available. */
#else
	uint64_t thr                          : 13;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_tospx_ul_th_s             cnf75xx;
};
typedef union cvmx_tospx_ul_th cvmx_tospx_ul_th_t;

/**
 * cvmx_tosp#_ul_win_en
 */
union cvmx_tospx_ul_win_en {
	uint64_t u64;
	struct cvmx_tospx_ul_win_en_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t enable                       : 4;  /**< Receive windows enable. When a bit is set, the corresponding window is
                                                         enabled (e.g., bit 0 enables window 0). The default value enables all
                                                         windows. */
#else
	uint64_t enable                       : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tospx_ul_win_en_s         cnf75xx;
};
typedef union cvmx_tospx_ul_win_en cvmx_tospx_ul_win_en_t;

/**
 * cvmx_tosp#_ul_win_end#
 */
union cvmx_tospx_ul_win_endx {
	uint64_t u64;
	struct cvmx_tospx_ul_win_endx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t end_cnt                      : 20; /**< End sample count plus 1 for UL window [b]. The maximum
                                                         value should be TOSP()_FRAME_L[FRAME_L], unless the window must stay
                                                         opened for ever.
                                                         Note that the end sample is exclusive, i.e,, sample [END_CNT] is not
                                                         included in the window. */
#else
	uint64_t end_cnt                      : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_win_endx_s       cnf75xx;
};
typedef union cvmx_tospx_ul_win_endx cvmx_tospx_ul_win_endx_t;

/**
 * cvmx_tosp#_ul_win_start#
 */
union cvmx_tospx_ul_win_startx {
	uint64_t u64;
	struct cvmx_tospx_ul_win_startx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t start_pnt                    : 20; /**< Start points for each of the four UL windows.
                                                         The start and end values have the following restrictions:
                                                         _ The first UL window must always start at 0.
                                                         _ The other start points must be greater than
                                                         TOSP()_UL_LEAD_LAG[UL_LEAD].
                                                         _ In TDD mode, the start point for each window must be less than the
                                                         end point for that window.
                                                         _ In TDD mode, UL windows have priority over DL windows.
                                                         _ There must a minimum of 7 samples between closing a window and
                                                         opening a new window. A minimum gap of 10 samples is recommended.
                                                         Different RF ICs may require a larger gap. */
#else
	uint64_t start_pnt                    : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_win_startx_s     cnf75xx;
};
typedef union cvmx_tospx_ul_win_startx cvmx_tospx_ul_win_startx_t;

/**
 * cvmx_tosp#_ul_win_upd_scnt
 */
union cvmx_tospx_ul_win_upd_scnt {
	uint64_t u64;
	struct cvmx_tospx_ul_win_upd_scnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t scnt                         : 20; /**< Receive window update sample count. When TOSP()_CONF[UPD_STYLE]=1, the
                                                         following registers are updated when the sample count reaches [SCNT]:
                                                         TOSP()_UL_WIN_EN, TOSP()_UL_WIN_START(), TOSP()_UL_WIN_END(),
                                                         TOSP()_NUM_UL_WIN, TOSP()_FRAME_L, TOSP()_UL_LEAD_LAG. */
#else
	uint64_t scnt                         : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tospx_ul_win_upd_scnt_s   cnf75xx;
};
typedef union cvmx_tospx_ul_win_upd_scnt cvmx_tospx_ul_win_upd_scnt_t;

/**
 * cvmx_tosp#_wr_timer64_lsb
 */
union cvmx_tospx_wr_timer64_lsb {
	uint64_t u64;
	struct cvmx_tospx_wr_timer64_lsb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t val                          : 32; /**< Least significant 32 bits of the initial value of the 64-bit timer. */
#else
	uint64_t val                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_wr_timer64_lsb_s    cnf75xx;
};
typedef union cvmx_tospx_wr_timer64_lsb cvmx_tospx_wr_timer64_lsb_t;

/**
 * cvmx_tosp#_wr_timer64_msb
 */
union cvmx_tospx_wr_timer64_msb {
	uint64_t u64;
	struct cvmx_tospx_wr_timer64_msb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t val                          : 32; /**< Most significant 32 bits of the initial value of the 64-bit timer. */
#else
	uint64_t val                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tospx_wr_timer64_msb_s    cnf75xx;
};
typedef union cvmx_tospx_wr_timer64_msb cvmx_tospx_wr_timer64_msb_t;

#endif
