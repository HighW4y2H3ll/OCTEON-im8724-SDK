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
 * cvmx-mdbwx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon mdbwx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_MDBWX_DEFS_H__
#define __CVMX_MDBWX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_CDERR_OFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_CDERR_OFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F40ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_CDERR_OFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F40ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_CDERR_UFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_CDERR_UFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F50ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_CDERR_UFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F50ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_CP_FAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_CP_FAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02900ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_CP_FAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02900ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_CP_NFAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_CP_NFAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F03000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_CP_NFAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F03000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_DEBUG0(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_DEBUG0(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F03230ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_DEBUG0(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F03230ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_DERR_ENA_W1C(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_DERR_ENA_W1C(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02100ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_DERR_ENA_W1C(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02100ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_DERR_ENA_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_DERR_ENA_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_DERR_ENA_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_DERR_INT(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_DERR_INT(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01E00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_DERR_INT(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01E00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_DERR_INT_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_DERR_INT_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02200ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_DERR_INT_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02200ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1C(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1C(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02500ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1C(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02500ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02400ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_FAT_ERR_ENA_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02400ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_FAT_ERR_INT(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_FAT_ERR_INT(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02300ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_FAT_ERR_INT(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02300ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_FAT_ERR_INT_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_FAT_ERR_INT_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02600ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_FAT_ERR_INT_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02600ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1C(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1C(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02C00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1C(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02C00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02B00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_NFAT_ERR_ENA_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02B00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02A00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02A00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT_W1S(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT_W1S(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02D00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_NFAT_ERR_INT_W1S(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02D00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_RDERR_OFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_RDERR_OFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F00ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_RDERR_OFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F00ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_RDERR_UFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_RDERR_UFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F10ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_RDERR_UFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F10ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_RD_FAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_RD_FAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02700ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_RD_FAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02700ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_RD_NFAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_RD_NFAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02E00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_RD_NFAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02E00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_WDERR_OFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_WDERR_OFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F20ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_WDERR_OFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F20ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_WDERR_UFLOW_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_WDERR_UFLOW_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F01F30ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8;
}
#else
#define CVMX_MDBWX_AB_SLTX_WDERR_UFLOW_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F01F30ull) + (((offset) & 1) + ((block_id) & 31) * 0x800ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_WR_FAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_WR_FAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02800ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_WR_FAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02800ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_AB_SLTX_WR_NFAT_JTAG(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDBWX_AB_SLTX_WR_NFAT_JTAG(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F02F00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDBWX_AB_SLTX_WR_NFAT_JTAG(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F02F00ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01000ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01000ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_DEBUG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_DEBUG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03240ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_DEBUG1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03240ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_DMA_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_DMA_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01028ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_DMA_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01028ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_DMA_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_DMA_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01030ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_DMA_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01030ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_DV_SCRATCH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_DV_SCRATCH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03F10ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_DV_SCRATCH(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03F10ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03F00ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03F00ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_ERR_STAT0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_ERR_STAT0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03218ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_ERR_STAT0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03218ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_ERR_STAT1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_ERR_STAT1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03220ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_ERR_STAT1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03220ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_FATAL_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_FATAL_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01038ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_FATAL_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01038ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_FATAL_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_FATAL_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01040ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_FATAL_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01040ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_FYI(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_FYI(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01100ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_FYI(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01100ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_JD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_JD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01008ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_JD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01008ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_JOB_COMPL_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_JOB_COMPL_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03208ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_JOB_COMPL_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03208ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_JOB_DROP_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_JOB_DROP_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03210ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_JOB_DROP_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03210ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_JOB_ENQUEUE_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_JOB_ENQUEUE_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F03100ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_JOB_ENQUEUE_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F03100ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01200ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01200ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_COR_DIS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_COR_DIS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01800ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_COR_DIS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01800ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_DBE_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_DBE_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01410ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_DBE_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01410ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_DBE_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_DBE_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01408ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_DBE_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01408ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_DBE_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_DBE_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01400ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_DBE_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01400ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_DBE_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_DBE_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01418ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_DBE_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01418ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_FLIP_SYND(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_FLIP_SYND(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01A00ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_FLIP_SYND(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01A00ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_SBE_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_SBE_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01610ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_SBE_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01610ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_SBE_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_SBE_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01608ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_SBE_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01608ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_SBE_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_SBE_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01600ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_SBE_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01600ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_MEM_SBE_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_MEM_SBE_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01618ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_MEM_SBE_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01618ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_NON_FATAL_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_NON_FATAL_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01050ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_NON_FATAL_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01050ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_NON_FATAL_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_NON_FATAL_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01058ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_NON_FATAL_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01058ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_TIMEOUT_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_TIMEOUT_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01068ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_TIMEOUT_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01068ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDBWX_TIMEOUT_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDBWX_TIMEOUT_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F01070ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDBWX_TIMEOUT_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F01070ull) + ((offset) & 31) * 16384)
#endif

/**
 * cvmx_mdbw#_ab_slt#_cderr_oflow_jtag
 *
 * This register set, specifies the config DMA overflow error logging of job tag
 * corresponding to the interrupt message per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_cderr_oflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_cderr_oflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_cderr_oflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_cderr_oflow_jtag cvmx_mdbwx_ab_sltx_cderr_oflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_cderr_uflow_jtag
 *
 * This register set, specifies the config DMA underflow error logging of job tag
 * corresponding to the interrupt message per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_cderr_uflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_cderr_uflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_cderr_uflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_cderr_uflow_jtag cvmx_mdbwx_ab_sltx_cderr_uflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_cp_fat_jtag
 *
 * This register set, specifies the control path (JMGR, AB) fatal error event logging of the job
 * tag, corresponding to the interrupt message per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_cp_fat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_cp_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ab                           : 16; /**< This set of bits, specifies the AB (DAC) error job tag. */
	uint64_t jd_fetch                     : 16; /**< This set of bits, specifies the job descriptor fetch, had a GHB response with a fatal
                                                         error. */
	uint64_t job_sd_fetch                 : 16; /**< This set of bits, the job subdescriptor fetch, had a GHB response with a fatal error. */
#else
	uint64_t job_sd_fetch                 : 16;
	uint64_t jd_fetch                     : 16;
	uint64_t ab                           : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_cp_fat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_cp_fat_jtag cvmx_mdbwx_ab_sltx_cp_fat_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_cp_nfat_jtag
 *
 * This register set, specifies the control Path (JMGR, AB) non-fatal error event logging of the
 * job tag, corresponding to the interrupt message register per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_cp_nfat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_cp_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ab                           : 16; /**< This set of bits, specifies the AB (DAC) error job tag. */
	uint64_t jd_fetch                     : 16; /**< This set of bits, specifies the job descriptor fetch, had a GHB response with a fatal
                                                         error. */
	uint64_t job_sd_fetch                 : 16; /**< This set of bits, the job subdescriptor fetch, had a GHB response with a fatal error. */
#else
	uint64_t job_sd_fetch                 : 16;
	uint64_t jd_fetch                     : 16;
	uint64_t ab                           : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_cp_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_cp_nfat_jtag cvmx_mdbwx_ab_sltx_cp_nfat_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_debug0
 */
union cvmx_mdbwx_ab_sltx_debug0 {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_debug0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ferr                         : 1;  /**< N/A */
	uint64_t nferr                        : 1;  /**< N/A */
	uint64_t derr                         : 1;  /**< N/A */
	uint64_t dberr                        : 1;  /**< N/A */
	uint64_t reserved_59_59               : 1;
	uint64_t sberr                        : 1;  /**< N/A */
	uint64_t reserved_16_57               : 42;
	uint64_t job_id                       : 16; /**< N/A */
#else
	uint64_t job_id                       : 16;
	uint64_t reserved_16_57               : 42;
	uint64_t sberr                        : 1;
	uint64_t reserved_59_59               : 1;
	uint64_t dberr                        : 1;
	uint64_t derr                         : 1;
	uint64_t nferr                        : 1;
	uint64_t ferr                         : 1;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_debug0_s    cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_debug0 cvmx_mdbwx_ab_sltx_debug0_t;

/**
 * cvmx_mdbw#_ab_slt#_derr_ena_w1c
 */
union cvmx_mdbwx_ab_sltx_derr_ena_w1c {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_derr_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t cfg_dma_uflow                : 1;  /**< This bit specifies the config DMA underflow error. */
	uint64_t cfg_dma_oflow                : 1;  /**< This bit specifies the config DMA overflow error. */
	uint64_t reserved_13_24               : 12;
	uint64_t wr_dma_uflow_port0           : 1;  /**< This bit specifies the write DMA port underflow error. */
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_oflow_port0           : 1;  /**< This bit specifies the write DMA port overflow error. */
	uint64_t reserved_5_7                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;  /**< This bit specifies the read DMA port 0 underflow error. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_oflow_port0           : 1;  /**< This bit specifies the read DMA port overflow error. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t reserved_13_24               : 12;
	uint64_t cfg_dma_oflow                : 1;
	uint64_t cfg_dma_uflow                : 1;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_derr_ena_w1c_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_derr_ena_w1c cvmx_mdbwx_ab_sltx_derr_ena_w1c_t;

/**
 * cvmx_mdbw#_ab_slt#_derr_ena_w1s
 */
union cvmx_mdbwx_ab_sltx_derr_ena_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_derr_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t cfg_dma_uflow                : 1;  /**< This bit specifies the config DMA underflow error. */
	uint64_t cfg_dma_oflow                : 1;  /**< This bit specifies the config DMA overflow error. */
	uint64_t reserved_13_24               : 12;
	uint64_t wr_dma_uflow_port0           : 1;  /**< This bit specifies the write DMA port underflow error. */
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_oflow_port0           : 1;  /**< This bit specifies the write DMA port overflow error. */
	uint64_t reserved_5_7                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;  /**< This bit specifies the read DMA port 0 underflow error. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_oflow_port0           : 1;  /**< This bit specifies the read DMA port overflow error. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t reserved_13_24               : 12;
	uint64_t cfg_dma_oflow                : 1;
	uint64_t cfg_dma_uflow                : 1;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_derr_ena_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_derr_ena_w1s cvmx_mdbwx_ab_sltx_derr_ena_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_derr_int
 *
 * This register set, specifies the DMA overflow/underflow error interrupt vector per MDBW per
 * job slot. These generate DMA error interrupts to the PSM, which drives this signal to the CIU.
 */
union cvmx_mdbwx_ab_sltx_derr_int {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_derr_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t cfg_dma_uflow                : 1;  /**< This bit specifies the config DMA underflow error. */
	uint64_t cfg_dma_oflow                : 1;  /**< This bit specifies the config DMA overflow error. */
	uint64_t reserved_13_24               : 12;
	uint64_t wr_dma_uflow_port0           : 1;  /**< This bit specifies the write DMA port underflow error. */
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_oflow_port0           : 1;  /**< This bit specifies the write DMA port overflow error. */
	uint64_t reserved_5_7                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;  /**< This bit specifies the read DMA port 0 underflow error. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_oflow_port0           : 1;  /**< This bit specifies the read DMA port overflow error. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t reserved_13_24               : 12;
	uint64_t cfg_dma_oflow                : 1;
	uint64_t cfg_dma_uflow                : 1;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_derr_int_s  cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_derr_int cvmx_mdbwx_ab_sltx_derr_int_t;

/**
 * cvmx_mdbw#_ab_slt#_derr_int_w1s
 */
union cvmx_mdbwx_ab_sltx_derr_int_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_derr_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t cfg_dma_uflow                : 1;  /**< This bit specifies the config DMA underflow error. */
	uint64_t cfg_dma_oflow                : 1;  /**< This bit specifies the config DMA overflow error. */
	uint64_t reserved_13_24               : 12;
	uint64_t wr_dma_uflow_port0           : 1;  /**< This bit specifies the write DMA port underflow error. */
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_oflow_port0           : 1;  /**< This bit specifies the write DMA port overflow error. */
	uint64_t reserved_5_7                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;  /**< This bit specifies the read DMA port 0 underflow error. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_oflow_port0           : 1;  /**< This bit specifies the read DMA port overflow error. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t reserved_13_24               : 12;
	uint64_t cfg_dma_oflow                : 1;
	uint64_t cfg_dma_uflow                : 1;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_derr_int_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_derr_int_w1s cvmx_mdbwx_ab_sltx_derr_int_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_fat_err_ena_w1c
 */
union cvmx_mdbwx_ab_sltx_fat_err_ena_w1c {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_fat_err_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies that the AB (DAC) had a fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies the write DMA port 0 has a fatal error. */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies the read DMA port 0 has a fatal error. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_fat_err_ena_w1c_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_fat_err_ena_w1c cvmx_mdbwx_ab_sltx_fat_err_ena_w1c_t;

/**
 * cvmx_mdbw#_ab_slt#_fat_err_ena_w1s
 */
union cvmx_mdbwx_ab_sltx_fat_err_ena_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_fat_err_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies that the AB (DAC) had a fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies the write DMA port 0 has a fatal error. */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies the read DMA port 0 has a fatal error. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_fat_err_ena_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_fat_err_ena_w1s cvmx_mdbwx_ab_sltx_fat_err_ena_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_fat_err_int
 *
 * This register set, specfies the fatal error interrupt vector per MDBW per job slot. These
 * generate fatal error interrupts to the PSM, which drives this signal to the CIU.
 */
union cvmx_mdbwx_ab_sltx_fat_err_int {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_fat_err_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies that the AB (DAC) had a fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies the write DMA port 0 has a fatal error. */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies the read DMA port 0 has a fatal error. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_fat_err_int_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_fat_err_int cvmx_mdbwx_ab_sltx_fat_err_int_t;

/**
 * cvmx_mdbw#_ab_slt#_fat_err_int_w1s
 */
union cvmx_mdbwx_ab_sltx_fat_err_int_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_fat_err_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies that the AB (DAC) had a fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies the write DMA port 0 has a fatal error. */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies the read DMA port 0 has a fatal error. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_fat_err_int_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_fat_err_int_w1s cvmx_mdbwx_ab_sltx_fat_err_int_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_nfat_err_ena_w1c
 */
union cvmx_mdbwx_ab_sltx_nfat_err_ena_w1c {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_nfat_err_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies the AB (DAC) had a non-fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies that the write DMA port 0 has a non-fatal Error . */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies that the read DMA port 0 has a non-fatal error . */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_nfat_err_ena_w1c_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_nfat_err_ena_w1c cvmx_mdbwx_ab_sltx_nfat_err_ena_w1c_t;

/**
 * cvmx_mdbw#_ab_slt#_nfat_err_ena_w1s
 */
union cvmx_mdbwx_ab_sltx_nfat_err_ena_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_nfat_err_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies the AB (DAC) had a non-fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies that the write DMA port 0 has a non-fatal Error . */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies that the read DMA port 0 has a non-fatal error . */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_nfat_err_ena_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_nfat_err_ena_w1s cvmx_mdbwx_ab_sltx_nfat_err_ena_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_nfat_err_int
 *
 * This register set, specifies the non-fatal error interrupt vector per MDBW per job slot. These
 * generate non-fatal error interrupts to the PSM, which drives this signal to the CIU.
 */
union cvmx_mdbwx_ab_sltx_nfat_err_int {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_nfat_err_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies the AB (DAC) had a non-fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies that the write DMA port 0 has a non-fatal Error . */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies that the read DMA port 0 has a non-fatal error . */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_nfat_err_int_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_nfat_err_int cvmx_mdbwx_ab_sltx_nfat_err_int_t;

/**
 * cvmx_mdbw#_ab_slt#_nfat_err_int_w1s
 */
union cvmx_mdbwx_ab_sltx_nfat_err_int_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_nfat_err_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< This bit specifies the AB (DAC) had a non-fatal error. */
	uint64_t jd_fetch                     : 1;  /**< This bit specifies the job descriptor fetch, had a GHB response with a fatal error. */
	uint64_t job_sd_fetch                 : 1;  /**< This bit specifies the job subdescriptor fetch, had a GHB response with a fatal error. */
	uint64_t reserved_9_15                : 7;
	uint64_t wr_dma_port0                 : 1;  /**< This bit specifies that the write DMA port 0 has a non-fatal Error . */
	uint64_t reserved_1_7                 : 7;
	uint64_t rd_dma_port0                 : 1;  /**< This bit specifies that the read DMA port 0 has a non-fatal error . */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t wr_dma_port0                 : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_nfat_err_int_w1s_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_nfat_err_int_w1s cvmx_mdbwx_ab_sltx_nfat_err_int_w1s_t;

/**
 * cvmx_mdbw#_ab_slt#_rd_fat_jtag
 *
 * This register set, specfies the read DMA fatal error event logging of the job tag,
 * corresponding to the interrupt message register per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_rd_fat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_rd_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_rd_fat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_rd_fat_jtag cvmx_mdbwx_ab_sltx_rd_fat_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_rd_nfat_jtag
 *
 * This register set, specifies the read DMA non-fatal error event logging of the job tag,
 * corresponding to the interrupt register per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_rd_nfat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_rd_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_rd_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_rd_nfat_jtag cvmx_mdbwx_ab_sltx_rd_nfat_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_rderr_oflow_jtag
 *
 * This register set, specifies the read DMA overflow error logging of job tag
 * corresponding to the interrupt message per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_rderr_oflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_rderr_oflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_rderr_oflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_rderr_oflow_jtag cvmx_mdbwx_ab_sltx_rderr_oflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_rderr_uflow_jtag
 *
 * This register set, specifies the read DMA underflow error logging of job tag
 * corresponding to the interrupt message per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_rderr_uflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_rderr_uflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_rderr_uflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_rderr_uflow_jtag cvmx_mdbwx_ab_sltx_rderr_uflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_wderr_oflow_jtag
 *
 * This register set, specfies the write DMA overflow error event logging of the job
 * tag, corresponding to the interrupt per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_wderr_oflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_wderr_oflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_wderr_oflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_wderr_oflow_jtag cvmx_mdbwx_ab_sltx_wderr_oflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_wderr_uflow_jtag
 *
 * This register set, specfies the write DMA underflow error event logging of the job
 * tag, corresponding to the interrupt per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_wderr_uflow_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_wderr_uflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_wderr_uflow_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_wderr_uflow_jtag cvmx_mdbwx_ab_sltx_wderr_uflow_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_wr_fat_jtag
 *
 * This register set, specifies the write DMA fatal error event logging of the job tag,
 * corresponding to the interrupt message per MDBW per job slot.
 * register.
 */
union cvmx_mdbwx_ab_sltx_wr_fat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_wr_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_wr_fat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_wr_fat_jtag cvmx_mdbwx_ab_sltx_wr_fat_jtag_t;

/**
 * cvmx_mdbw#_ab_slt#_wr_nfat_jtag
 *
 * This register set, specifies the write DMA non-fatal error logging of the job tag,
 * corresponding to the interrupt message register per MDBW per job slot.
 */
union cvmx_mdbwx_ab_sltx_wr_nfat_jtag {
	uint64_t u64;
	struct cvmx_mdbwx_ab_sltx_wr_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t port0                        : 16; /**< This set of bits, specifies the DMA port 0 job tag. */
#else
	uint64_t port0                        : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdbwx_ab_sltx_wr_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mdbwx_ab_sltx_wr_nfat_jtag cvmx_mdbwx_ab_sltx_wr_nfat_jtag_t;

/**
 * cvmx_mdbw#_cfg
 *
 * This register set, specifies the configuration for each MDBW globally in terms of reset, clock
 * and priority levels.
 */
union cvmx_mdbwx_cfg {
	uint64_t u64;
	struct cvmx_mdbwx_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t mdbw_done                    : 1;  /**< This bit is set, after this register's [ENA] bit = 0, and indicates the MDBW has completed
                                                         clearing internal states corresponding to PSM Jobs. This bit must be set, before software
                                                         reenables the MDBW by setting [ENA] = 1. */
	uint64_t ab_done                      : 1;  /**< This bit is used to indicate the MDBW has completed clearing internal states corresponding
                                                         to
                                                         an AB. This bit is reset by hardware after a MDBW is reenabled. */
	uint64_t reserved_29_48               : 20;
	uint64_t mdbw_stopped                 : 1;  /**< This bit is set, after this register's [ENA] bit = 0, and indicates the MDBW is busy
                                                         clearing internal states corresponding to PSM Jobs. This bit must be equal to 0, before
                                                         software reenables the MDBW by setting [ENA] = 1. */
	uint64_t reserved_25_27               : 3;
	uint64_t ab_stopped                   : 1;  /**< This bit is set indicates the MDBW is busy clearing internal states corresponding to an
                                                         AB. This bit must be equal to 0, before software reenables the AB by setting [AB_ENA] =
                                                         1. */
	uint64_t reserved_21_23               : 3;
	uint64_t ab_busy                      : 1;  /**< This bit is set indicates the AB is busy processing a job. */
	uint64_t reserved_17_19               : 3;
	uint64_t ab_ena                       : 1;  /**< This bit defines the enable corresponding to the AB within the MDBW. */
	uint64_t reserved_13_15               : 3;
	uint64_t ab_clk_gating_ena            : 1;  /**< Enables automatic clock gating to the MDBW data path. When enabled,
                                                         the MDBW clock is disabled when the DSP core is waiting on a WAITI
                                                         instruction and no DMA transfers in progress. When cleared to zero,
                                                         the clock is never gated. */
	uint64_t reserved_5_11                : 7;
	uint64_t ab_clk_ena                   : 1;  /**< This bit specifies the enable for the conditional clock to the AB within the MDBW. */
	uint64_t reserved_3_3                 : 1;
	uint64_t job_slot_ena                 : 1;  /**< This bit enables MDBW job slot scheduling. Setting this bit = 0, all AB's will use slot 0
                                                         for all subsequent
                                                         jobs. Software must ensure the MDBW is in an idle state, i.e., without any active or
                                                         pending jobs enqueued, before setting this bit. */
	uint64_t ena                          : 1;  /**< This bit enables the MDBW. Setting this bit = 0, does not reset any MDBW CSR
                                                         setting or stats; it only resets the jmgr, amm logic. */
	uint64_t clk_ena                      : 1;  /**< This bit specifies the MDBW conditional clock enable. */
#else
	uint64_t clk_ena                      : 1;
	uint64_t ena                          : 1;
	uint64_t job_slot_ena                 : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t ab_clk_ena                   : 1;
	uint64_t reserved_5_11                : 7;
	uint64_t ab_clk_gating_ena            : 1;
	uint64_t reserved_13_15               : 3;
	uint64_t ab_ena                       : 1;
	uint64_t reserved_17_19               : 3;
	uint64_t ab_busy                      : 1;
	uint64_t reserved_21_23               : 3;
	uint64_t ab_stopped                   : 1;
	uint64_t reserved_25_27               : 3;
	uint64_t mdbw_stopped                 : 1;
	uint64_t reserved_29_48               : 20;
	uint64_t ab_done                      : 1;
	uint64_t mdbw_done                    : 1;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_mdbwx_cfg_s               cnf75xx;
};
typedef union cvmx_mdbwx_cfg cvmx_mdbwx_cfg_t;

/**
 * cvmx_mdbw#_debug1
 */
union cvmx_mdbwx_debug1 {
	uint64_t u64;
	struct cvmx_mdbwx_debug1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t master_reset_n               : 1;  /**< N/A */
	uint64_t cfg_arb_sel                  : 1;  /**< N/A */
	uint64_t reserved_1_61                : 61;
	uint64_t amm_dbg_en                   : 1;  /**< Enables the AMM debug mux data on the bus. As this muxs data from SRAM and flp_fif's, this
                                                         bit gates unqualified data. */
#else
	uint64_t amm_dbg_en                   : 1;
	uint64_t reserved_1_61                : 61;
	uint64_t cfg_arb_sel                  : 1;
	uint64_t master_reset_n               : 1;
#endif
	} s;
	struct cvmx_mdbwx_debug1_s            cnf75xx;
};
typedef union cvmx_mdbwx_debug1 cvmx_mdbwx_debug1_t;

/**
 * cvmx_mdbw#_dma_error_jce_w0
 *
 * This register set, specifies the DMA error job completion message word 0 per MDBW.
 *
 */
union cvmx_mdbwx_dma_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mdbwx_dma_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< This set of bits, specifies the word 0 for a DMA error job completion status to send to
                                                         PSM <w1,w0>. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_dma_error_jce_w0_s  cnf75xx;
};
typedef union cvmx_mdbwx_dma_error_jce_w0 cvmx_mdbwx_dma_error_jce_w0_t;

/**
 * cvmx_mdbw#_dma_error_jce_w1
 *
 * This register set, specifies the DMA error job completion message word 1 per MDBW.
 *
 */
union cvmx_mdbwx_dma_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mdbwx_dma_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< This set of bits defines the word 1 for a DMA error job completion status to send to PSM <w1,w0>. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_dma_error_jce_w1_s  cnf75xx;
};
typedef union cvmx_mdbwx_dma_error_jce_w1 cvmx_mdbwx_dma_error_jce_w1_t;

/**
 * cvmx_mdbw#_dv_scratch
 */
union cvmx_mdbwx_dv_scratch {
	uint64_t u64;
	struct cvmx_mdbwx_dv_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t it                           : 64; /**< N/A */
#else
	uint64_t it                           : 64;
#endif
	} s;
	struct cvmx_mdbwx_dv_scratch_s        cnf75xx;
};
typedef union cvmx_mdbwx_dv_scratch cvmx_mdbwx_dv_scratch_t;

/**
 * cvmx_mdbw#_eco
 */
union cvmx_mdbwx_eco {
	uint64_t u64;
	struct cvmx_mdbwx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_mdbwx_eco_s               cnf75xx;
};
typedef union cvmx_mdbwx_eco cvmx_mdbwx_eco_t;

/**
 * cvmx_mdbw#_err_stat0
 *
 * This register set, specifies the statistic for the number of errors (except SBE's and
 * DBE's) per MDBW.
 */
union cvmx_mdbwx_err_stat0 {
	uint64_t u64;
	struct cvmx_mdbwx_err_stat0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nfat_err_stat                : 32; /**< This set of bits, specifies the statistic counter value of non-fatal errors. */
	uint64_t fat_err_stat                 : 32; /**< This set of bits, specifies the statistic counter value of fatal errors. */
#else
	uint64_t fat_err_stat                 : 32;
	uint64_t nfat_err_stat                : 32;
#endif
	} s;
	struct cvmx_mdbwx_err_stat0_s         cnf75xx;
};
typedef union cvmx_mdbwx_err_stat0 cvmx_mdbwx_err_stat0_t;

/**
 * cvmx_mdbw#_err_stat1
 *
 * This register set, specifies the statistic for the number of DMA errors per MDBW.
 *
 */
union cvmx_mdbwx_err_stat1 {
	uint64_t u64;
	struct cvmx_mdbwx_err_stat1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dma_err_stat                 : 32; /**< This set of bits, specifies the number of DMA errors. */
#else
	uint64_t dma_err_stat                 : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdbwx_err_stat1_s         cnf75xx;
};
typedef union cvmx_mdbwx_err_stat1 cvmx_mdbwx_err_stat1_t;

/**
 * cvmx_mdbw#_fatal_error_jce_w0
 *
 * This register set, specifies fatal error job completion message word 0 per MDBW.
 *
 */
union cvmx_mdbwx_fatal_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mdbwx_fatal_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< This set of bits, specifies the word 0 for a fatal error job completion status to send to
                                                         PSM <w1,w0>. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_fatal_error_jce_w0_s cnf75xx;
};
typedef union cvmx_mdbwx_fatal_error_jce_w0 cvmx_mdbwx_fatal_error_jce_w0_t;

/**
 * cvmx_mdbw#_fatal_error_jce_w1
 *
 * This register set, specifies the fatal error job completion message word 1 per MDBW.
 *
 */
union cvmx_mdbwx_fatal_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mdbwx_fatal_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< This set of bits, specifies the word 1 for a fatal error job completion status to send to
                                                         PSM <w1,w0>. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_fatal_error_jce_w1_s cnf75xx;
};
typedef union cvmx_mdbwx_fatal_error_jce_w1 cvmx_mdbwx_fatal_error_jce_w1_t;

/**
 * cvmx_mdbw#_fyi
 *
 * This register set, specifies the hardware specifications per MDBW.
 *
 */
union cvmx_mdbwx_fyi {
	uint64_t u64;
	struct cvmx_mdbwx_fyi_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t psm_did                      : 6;  /**< This set of bits, specifies the PSM bus device ID for the MDBW. */
	uint64_t mdb_did                      : 5;  /**< This set of bits, specifies the Mega-DSP accelerator block device ID for this MDBW. */
	uint64_t clk_type                     : 1;  /**< This bits specifies the clk that is used by the AB interface, 0 corresponds to clk and 1
                                                         correpsonds to clk/2. */
	uint64_t reserved_40_51               : 12;
	uint64_t ab_cores                     : 2;  /**< This set of bits, specifies the number of AB cores present in the MDBW;
                                                         where the number of cores =  MDBW()_FYI[AB_CORES] + 1. */
	uint64_t ab_cfg_credit_ena            : 1;  /**< This bit specifies, if the AB config interface uses credit based access. */
	uint64_t num_ab_rd_ports              : 2;  /**< This set of bits, specifies the number of AB read ports; where the number of RD ports =
                                                         MDBW()_FYI[NUM_AB_RD_PORTS] + 1. */
	uint64_t num_ab_wr_ports              : 2;  /**< This set of bits, specifies the number of AB write ports; where the number of WR ports =
                                                         MDBW()_FYI[NUM_AB_WR_PORTS] + 1. */
	uint64_t num_ab_job_slots             : 2;  /**< This set of bits, specifies the number of AB job slots; where the number of job slots =
                                                         MDBW()_FYI[NUM_AB_JOB_SLOTS] + 1. */
	uint64_t ab_rd_port_0                 : 2;  /**< This set of bits, specifies the read port 0 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_rd_port_1                 : 2;  /**< This set of bits, specifies the read port 1 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_rd_port_2                 : 2;  /**< This set of bits, specifies the read port 2 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_rd_port_3                 : 2;  /**< This set of bits, specifies the read port 3 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_wr_port_0                 : 2;  /**< This set of bits, specifies the write port 0 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_wr_port_1                 : 2;  /**< This set of bits, specifies the write port 1 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_wr_port_2                 : 2;  /**< This set of bits, specifies the write port 2 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t ab_wr_port_3                 : 2;  /**< This set of bits, specifies the write port 3 width if defined for the AB, enumerated with
                                                         MDBW_AB_PORT_WIDTH_E. */
	uint64_t reserved_0_14                : 15;
#else
	uint64_t reserved_0_14                : 15;
	uint64_t ab_wr_port_3                 : 2;
	uint64_t ab_wr_port_2                 : 2;
	uint64_t ab_wr_port_1                 : 2;
	uint64_t ab_wr_port_0                 : 2;
	uint64_t ab_rd_port_3                 : 2;
	uint64_t ab_rd_port_2                 : 2;
	uint64_t ab_rd_port_1                 : 2;
	uint64_t ab_rd_port_0                 : 2;
	uint64_t num_ab_job_slots             : 2;
	uint64_t num_ab_wr_ports              : 2;
	uint64_t num_ab_rd_ports              : 2;
	uint64_t ab_cfg_credit_ena            : 1;
	uint64_t ab_cores                     : 2;
	uint64_t reserved_40_51               : 12;
	uint64_t clk_type                     : 1;
	uint64_t mdb_did                      : 5;
	uint64_t psm_did                      : 6;
#endif
	} s;
	struct cvmx_mdbwx_fyi_s               cnf75xx;
};
typedef union cvmx_mdbwx_fyi cvmx_mdbwx_fyi_t;

/**
 * cvmx_mdbw#_jd_cfg
 *
 * This register set, specifies per job configuration parameters per MDBW.
 *
 */
union cvmx_mdbwx_jd_cfg {
	uint64_t u64;
	struct cvmx_mdbwx_jd_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_38_63               : 26;
	uint64_t ghb_rd_wrr_val               : 6;  /**< This set of bits, specifies the MDBW weight used for weighted round-robin arbitration of
                                                         read requests. The weighted round-robin quantum is a 6-bit unsigned value. A value of zero
                                                         will only allow requests from the given MDBW when there are no requests from other
                                                         competing MDBWs in the GHB, or when all
                                                         competing MDBWs have consumed all of their weight in the current quantum. */
	uint64_t reserved_30_31               : 2;
	uint64_t ghb_wr_wrr_val               : 6;  /**< This set of bits, specifies the MDBW weight used for weighted round-robin arbitration of
                                                         write requests. The
                                                         weighted round-robin
                                                         quantum is a 6-bit unsigned value. A value of zero will only allow requests from the
                                                         given MDBW when there are no requests from other competing MDBWs in the GHB, or when all
                                                         competing MDBWs have consumed all of their weight in the current quantum. */
	uint64_t reserved_17_23               : 7;
	uint64_t dac_fetch_cmd_type           : 1;  /**< This set of bits, specifies the command type to use for all DAC orginating read requests
                                                         to L2C/DDR; as enumerated in MDBW_DAC_PNB_RD_CMD_E. */
	uint64_t reserved_10_15               : 6;
	uint64_t ghb_wr_priority              : 1;  /**< This bit specifies the MDBW priority level for GHB write transactions enumerated in
                                                         MDBW_GHB_JOB_PRIORITY_E. */
	uint64_t ghb_rd_priority              : 1;  /**< This bit specifies the MDBW priority level for GHB read transactions enumerated in
                                                         MDBW_GHB_JOB_PRIORITY_E. */
	uint64_t timeout_mult                 : 4;  /**< This set of bits, specifies the timeout multiplier value to be used along with the
                                                         MDBW_HDR_WORD_0_S[TOTH_TICK]
                                                         and MDBW_HDR_WORD_0_S[TOTH] field; to determine the the job timeout value. */
	uint64_t dsp_tto                      : 1;  /**< This bit specifies when the MDBW starts counting for a job timeout specified in
                                                         MDBW_JD_HDR_WORD_0_S[TOTH]. This bit is = 0; indicates that the specified timeout
                                                         commences based on when the dsp is busy. This bit is = 1; indicates that the specified
                                                         timeout commences based on when the job commences internally within the MDBW. Thereby, the
                                                         timeout is an aggregate sum of all the internal system latencies. */
	uint64_t reserved_2_2                 : 1;
	uint64_t jd_fetch_cmd_type            : 2;  /**< This set of bits, specifies the command type to use for all the job descriptor,
                                                         subdescriptor fetches made by the MDBW as enumerated in MDBW_PNB_RD_CMD_E.
                                                          * If the MHBW_PNB_RD_CMD_E=LDWB, then the MHBW will issue an LDT for all non full
                                                         cacheline requests. */
#else
	uint64_t jd_fetch_cmd_type            : 2;
	uint64_t reserved_2_2                 : 1;
	uint64_t dsp_tto                      : 1;
	uint64_t timeout_mult                 : 4;
	uint64_t ghb_rd_priority              : 1;
	uint64_t ghb_wr_priority              : 1;
	uint64_t reserved_10_15               : 6;
	uint64_t dac_fetch_cmd_type           : 1;
	uint64_t reserved_17_23               : 7;
	uint64_t ghb_wr_wrr_val               : 6;
	uint64_t reserved_30_31               : 2;
	uint64_t ghb_rd_wrr_val               : 6;
	uint64_t reserved_38_63               : 26;
#endif
	} s;
	struct cvmx_mdbwx_jd_cfg_s            cnf75xx;
};
typedef union cvmx_mdbwx_jd_cfg cvmx_mdbwx_jd_cfg_t;

/**
 * cvmx_mdbw#_job_compl_stat
 *
 * This register set, specifies the statistic for the number of jobs that have been completed per
 * MDBW.
 */
union cvmx_mdbwx_job_compl_stat {
	uint64_t u64;
	struct cvmx_mdbwx_job_compl_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t stat                         : 48; /**< This set of bits, specifies the statistic counter value. */
#else
	uint64_t stat                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mdbwx_job_compl_stat_s    cnf75xx;
};
typedef union cvmx_mdbwx_job_compl_stat cvmx_mdbwx_job_compl_stat_t;

/**
 * cvmx_mdbw#_job_drop_stat
 *
 * This register set, specifies the statistic for the number of jobs that have been
 * dropped per MDBW. This
 * indicates jobs that are dropped after the MDBW is disabled.
 */
union cvmx_mdbwx_job_drop_stat {
	uint64_t u64;
	struct cvmx_mdbwx_job_drop_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t stat                         : 32; /**< This set of bits, specifies the statistic counter value. */
#else
	uint64_t stat                         : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdbwx_job_drop_stat_s     cnf75xx;
};
typedef union cvmx_mdbwx_job_drop_stat cvmx_mdbwx_job_drop_stat_t;

/**
 * cvmx_mdbw#_job_enqueue_stat
 *
 * This register set, specifies the statistic for the number of jobs that have been enqueued per MDBW.
 *
 */
union cvmx_mdbwx_job_enqueue_stat {
	uint64_t u64;
	struct cvmx_mdbwx_job_enqueue_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t stat                         : 48; /**< This set of bits, specifies the statistic counter value. */
#else
	uint64_t stat                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mdbwx_job_enqueue_stat_s  cnf75xx;
};
typedef union cvmx_mdbwx_job_enqueue_stat cvmx_mdbwx_job_enqueue_stat_t;

/**
 * cvmx_mdbw#_mem_bist_status
 *
 * This register set, specifies the bist status for all memories; where a value 0
 * = pass or never run and 1 = fail per MDBW.
 */
union cvmx_mdbwx_mem_bist_status {
	uint64_t u64;
	struct cvmx_mdbwx_mem_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mdbwx_mem_bist_status_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_bist_status cvmx_mdbwx_mem_bist_status_t;

/**
 * cvmx_mdbw#_mem_cor_dis
 *
 * This register set, is used to to test the internal memory by disabling ECC correction, by
 * setting the corresponding bit per MDBW.
 */
union cvmx_mdbwx_mem_cor_dis {
	uint64_t u64;
	struct cvmx_mdbwx_mem_cor_dis_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mdbwx_mem_cor_dis_s       cnf75xx;
};
typedef union cvmx_mdbwx_mem_cor_dis cvmx_mdbwx_mem_cor_dis_t;

/**
 * cvmx_mdbw#_mem_dbe_ena_w1c
 */
union cvmx_mdbwx_mem_dbe_ena_w1c {
	uint64_t u64;
	struct cvmx_mdbwx_mem_dbe_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_dbe_ena_w1c_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_dbe_ena_w1c cvmx_mdbwx_mem_dbe_ena_w1c_t;

/**
 * cvmx_mdbw#_mem_dbe_ena_w1s
 */
union cvmx_mdbwx_mem_dbe_ena_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_mem_dbe_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_dbe_ena_w1s_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_dbe_ena_w1s cvmx_mdbwx_mem_dbe_ena_w1s_t;

/**
 * cvmx_mdbw#_mem_dbe_int
 *
 * This register set, specifies the double-bit error status for all memories;
 * where a value 0 = pass or never run and 1 = fail per MDBW. These generate fatal error
 * interrupts to the PSM, which drives this signal to the CIU.
 */
union cvmx_mdbwx_mem_dbe_int {
	uint64_t u64;
	struct cvmx_mdbwx_mem_dbe_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_dbe_int_s       cnf75xx;
};
typedef union cvmx_mdbwx_mem_dbe_int cvmx_mdbwx_mem_dbe_int_t;

/**
 * cvmx_mdbw#_mem_dbe_int_w1s
 */
union cvmx_mdbwx_mem_dbe_int_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_mem_dbe_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_dbe_int_w1s_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_dbe_int_w1s cvmx_mdbwx_mem_dbe_int_w1s_t;

/**
 * cvmx_mdbw#_mem_flip_synd
 *
 * This register set, is used to to test the internal memory for single-bit or double-bit error
 * handling per MDBW.
 */
union cvmx_mdbwx_mem_flip_synd {
	uint64_t u64;
	struct cvmx_mdbwx_mem_flip_synd_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mdbwx_mem_flip_synd_s     cnf75xx;
};
typedef union cvmx_mdbwx_mem_flip_synd cvmx_mdbwx_mem_flip_synd_t;

/**
 * cvmx_mdbw#_mem_sbe_ena_w1c
 */
union cvmx_mdbwx_mem_sbe_ena_w1c {
	uint64_t u64;
	struct cvmx_mdbwx_mem_sbe_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_sbe_ena_w1c_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_sbe_ena_w1c cvmx_mdbwx_mem_sbe_ena_w1c_t;

/**
 * cvmx_mdbw#_mem_sbe_ena_w1s
 */
union cvmx_mdbwx_mem_sbe_ena_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_mem_sbe_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_sbe_ena_w1s_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_sbe_ena_w1s cvmx_mdbwx_mem_sbe_ena_w1s_t;

/**
 * cvmx_mdbw#_mem_sbe_int
 *
 * This register set, handles the single-bit error status for all memories;
 * where a value 0 = pass or never run and 1 = fail per MDBW. These generate non-fatal error
 * interrupts to the PSM, which drives this signal to the CIU.
 */
union cvmx_mdbwx_mem_sbe_int {
	uint64_t u64;
	struct cvmx_mdbwx_mem_sbe_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_sbe_int_s       cnf75xx;
};
typedef union cvmx_mdbwx_mem_sbe_int cvmx_mdbwx_mem_sbe_int_t;

/**
 * cvmx_mdbw#_mem_sbe_int_w1s
 */
union cvmx_mdbwx_mem_sbe_int_w1s {
	uint64_t u64;
	struct cvmx_mdbwx_mem_sbe_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mdbwx_mem_sbe_int_w1s_s   cnf75xx;
};
typedef union cvmx_mdbwx_mem_sbe_int_w1s cvmx_mdbwx_mem_sbe_int_w1s_t;

/**
 * cvmx_mdbw#_non_fatal_error_jce_w0
 *
 * This register set, specifies the non-fatal error job completion message word 0 per MDBW.
 *
 */
union cvmx_mdbwx_non_fatal_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mdbwx_non_fatal_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< This set of bits, specifies the word 0 for a non-fatal error job completion status to send
                                                         to
                                                         PSM <w1,w0>. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_non_fatal_error_jce_w0_s cnf75xx;
};
typedef union cvmx_mdbwx_non_fatal_error_jce_w0 cvmx_mdbwx_non_fatal_error_jce_w0_t;

/**
 * cvmx_mdbw#_non_fatal_error_jce_w1
 *
 * This register set, specifies the non-fatal error job completion message word 1 per MDBW.
 *
 */
union cvmx_mdbwx_non_fatal_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mdbwx_non_fatal_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< This set of bits, specifies the word 1 for a non-fatal error job completion status to send
                                                         to
                                                         PSM <w1,w0>. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_non_fatal_error_jce_w1_s cnf75xx;
};
typedef union cvmx_mdbwx_non_fatal_error_jce_w1 cvmx_mdbwx_non_fatal_error_jce_w1_t;

/**
 * cvmx_mdbw#_timeout_jce_w0
 *
 * This register set, specifies the timeout job completion message word 0 per MDBW.
 *
 */
union cvmx_mdbwx_timeout_jce_w0 {
	uint64_t u64;
	struct cvmx_mdbwx_timeout_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< This set of bits, specifies the word 0 for a timeout job completion status to send to PSM<w1,w0>. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_timeout_jce_w0_s    cnf75xx;
};
typedef union cvmx_mdbwx_timeout_jce_w0 cvmx_mdbwx_timeout_jce_w0_t;

/**
 * cvmx_mdbw#_timeout_jce_w1
 *
 * This register set, specifies the timeout job completion message word 1 per MDBW.
 *
 */
union cvmx_mdbwx_timeout_jce_w1 {
	uint64_t u64;
	struct cvmx_mdbwx_timeout_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< This set of bits, specifies the word 1 for a timeout job completion status to send to PSM<w1,w0>. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mdbwx_timeout_jce_w1_s    cnf75xx;
};
typedef union cvmx_mdbwx_timeout_jce_w1 cvmx_mdbwx_timeout_jce_w1_t;

#endif
