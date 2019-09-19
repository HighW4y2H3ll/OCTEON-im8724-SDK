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
 * cvmx-mhbwx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon mhbwx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_MHBWX_DEFS_H__
#define __CVMX_MHBWX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_CP_FAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_CP_FAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022000ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_CP_FAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022000ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_CP_NFAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_CP_NFAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022700ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_CP_NFAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022700ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_DEBUG0(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_DEBUG0(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022900ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_DEBUG0(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022900ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1C(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021200ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1C(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021200ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021100ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_DERR_ENA_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021100ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_DERR_INT(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_DERR_INT(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3020E00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_DERR_INT(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3020E00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_DERR_INT_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_DERR_INT_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021300ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_DERR_INT_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021300ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1C(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021C00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1C(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021C00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021B00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_FAT_ERR_ENA_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021B00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021A00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021A00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021D00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_FAT_ERR_INT_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021D00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1C(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1C(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022300ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1C(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022300ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022200ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_NFAT_ERR_ENA_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022200ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022100ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022100ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT_W1S(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT_W1S(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022400ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_NFAT_ERR_INT_W1S(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022400ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_RDERR_OFLOW_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_RDERR_OFLOW_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3020F00ull) + ((a) << 19) + ((b) << 5) + ((c) << 3);
}
#else
#define CVMX_MHBWX_ABX_SLTX_RDERR_OFLOW_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3020F00ull) + ((a) << 19) + ((b) << 5) + ((c) << 3))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_RDERR_UFLOW_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_RDERR_UFLOW_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3020F80ull) + ((a) << 19) + ((b) << 5) + ((c) << 3);
}
#else
#define CVMX_MHBWX_ABX_SLTX_RDERR_UFLOW_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3020F80ull) + ((a) << 19) + ((b) << 5) + ((c) << 3))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_RD_FAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_RD_FAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021E00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_RD_FAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021E00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_RD_NFAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_RD_NFAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022500ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_RD_NFAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022500ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_WDERR_OFLOW_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_WDERR_OFLOW_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021000ull) + ((a) << 19) + ((b) << 5) + ((c) << 3);
}
#else
#define CVMX_MHBWX_ABX_SLTX_WDERR_OFLOW_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021000ull) + ((a) << 19) + ((b) << 5) + ((c) << 3))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_WDERR_UFLOW_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_WDERR_UFLOW_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021080ull) + ((a) << 19) + ((b) << 5) + ((c) << 3);
}
#else
#define CVMX_MHBWX_ABX_SLTX_WDERR_UFLOW_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021080ull) + ((a) << 19) + ((b) << 5) + ((c) << 3))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_WR_FAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_WR_FAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3021F00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_WR_FAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3021F00ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ABX_SLTX_WR_NFAT_JTAG(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 19)) && ((b <= 3)) && ((c <= 2))))))
		cvmx_warn("CVMX_MHBWX_ABX_SLTX_WR_NFAT_JTAG(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B3022600ull) + ((a) << 19) + ((b) << 6) + ((c) << 4);
}
#else
#define CVMX_MHBWX_ABX_SLTX_WR_NFAT_JTAG(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B3022600ull) + ((a) << 19) + ((b) << 6) + ((c) << 4))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020000ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800B3020000ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_DMA_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_DMA_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020028ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_DMA_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020028ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_DMA_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_DMA_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020030ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_DMA_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020030ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B302F000ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B302F000ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ERR_STAT0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_ERR_STAT0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3022818ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_ERR_STAT0(offset) (CVMX_ADD_IO_SEG(0x00011800B3022818ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_ERR_STAT1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_ERR_STAT1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3022820ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_ERR_STAT1(offset) (CVMX_ADD_IO_SEG(0x00011800B3022820ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_EXTX_MEM_BIST_STATUS(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 7)) && ((block_id <= 19))))))
		cvmx_warn("CVMX_MHBWX_EXTX_MEM_BIST_STATUS(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B302D000ull) + (((offset) & 7) + ((block_id) & 31) * 0x8000ull) * 16;
}
#else
#define CVMX_MHBWX_EXTX_MEM_BIST_STATUS(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B302D000ull) + (((offset) & 7) + ((block_id) & 31) * 0x8000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_EXTX_SFUNC(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 19))))))
		cvmx_warn("CVMX_MHBWX_EXTX_SFUNC(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B302D100ull) + (((offset) & 3) + ((block_id) & 31) * 0x8000ull) * 16;
}
#else
#define CVMX_MHBWX_EXTX_SFUNC(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B302D100ull) + (((offset) & 3) + ((block_id) & 31) * 0x8000ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_FATAL_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_FATAL_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020038ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_FATAL_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020038ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_FATAL_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_FATAL_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020040ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_FATAL_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020040ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_FYI(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_FYI(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020100ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_FYI(offset) (CVMX_ADD_IO_SEG(0x00011800B3020100ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_JD_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_JD_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020008ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_JD_CFG(offset) (CVMX_ADD_IO_SEG(0x00011800B3020008ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_JOB_COMPL_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_JOB_COMPL_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3022808ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_JOB_COMPL_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B3022808ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_JOB_DROP_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_JOB_DROP_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3022810ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_JOB_DROP_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B3022810ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_JOB_ENQUEUE_STAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_JOB_ENQUEUE_STAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3022800ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_JOB_ENQUEUE_STAT(offset) (CVMX_ADD_IO_SEG(0x00011800B3022800ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_BIST_STATUS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_BIST_STATUS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020200ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_BIST_STATUS0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020200ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_BIST_STATUS1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_BIST_STATUS1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020300ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_BIST_STATUS1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020300ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_COR_DIS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_COR_DIS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020800ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_COR_DIS0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020800ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_COR_DIS1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_COR_DIS1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020900ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_COR_DIS1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020900ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE0_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE0_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020410ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE0_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B3020410ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE0_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE0_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020408ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE0_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020408ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE0_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE0_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020400ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE0_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B3020400ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE0_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE0_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020418ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE0_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020418ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE1_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE1_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020510ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE1_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B3020510ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE1_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE1_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020508ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE1_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020508ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE1_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE1_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020500ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE1_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B3020500ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_DBE1_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_DBE1_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020518ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_DBE1_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020518ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_FLIP_SYND0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_FLIP_SYND0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020A00ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_FLIP_SYND0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020A00ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_FLIP_SYND1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_FLIP_SYND1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020B00ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_FLIP_SYND1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020B00ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_FLIP_SYND2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_FLIP_SYND2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020C00ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_FLIP_SYND2(offset) (CVMX_ADD_IO_SEG(0x00011800B3020C00ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_FLIP_SYND3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_FLIP_SYND3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020D00ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_FLIP_SYND3(offset) (CVMX_ADD_IO_SEG(0x00011800B3020D00ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE0_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE0_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020610ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE0_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B3020610ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE0_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE0_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020608ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE0_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020608ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE0_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE0_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020600ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE0_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B3020600ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE0_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE0_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020618ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE0_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020618ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE1_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE1_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020710ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE1_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B3020710ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE1_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE1_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020708ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE1_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020708ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE1_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE1_INT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020700ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE1_INT(offset) (CVMX_ADD_IO_SEG(0x00011800B3020700ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_MEM_SBE1_INT_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_MEM_SBE1_INT_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020718ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_MEM_SBE1_INT_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B3020718ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_NON_FATAL_ERROR_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_NON_FATAL_ERROR_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020050ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_NON_FATAL_ERROR_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020050ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_NON_FATAL_ERROR_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_NON_FATAL_ERROR_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020058ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_NON_FATAL_ERROR_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020058ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_SPECIAL_FUNC(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_SPECIAL_FUNC(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020020ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_SPECIAL_FUNC(offset) (CVMX_ADD_IO_SEG(0x00011800B3020020ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_TIMEOUT_JCE_W0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_TIMEOUT_JCE_W0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020068ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_TIMEOUT_JCE_W0(offset) (CVMX_ADD_IO_SEG(0x00011800B3020068ull) + ((offset) & 31) * 0x80000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MHBWX_TIMEOUT_JCE_W1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 19)))))
		cvmx_warn("CVMX_MHBWX_TIMEOUT_JCE_W1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3020070ull) + ((offset) & 31) * 0x80000ull;
}
#else
#define CVMX_MHBWX_TIMEOUT_JCE_W1(offset) (CVMX_ADD_IO_SEG(0x00011800B3020070ull) + ((offset) & 31) * 0x80000ull)
#endif

/**
 * cvmx_mhbw#_ab#_slt#_cp_fat_jtag
 *
 * These registers record the job tag for HAB fatal errors and MHBW control
 * path fatal errors.  Errors are reported for the specific HAB and slot that
 * received the error.
 */
union cvmx_mhbwx_abx_sltx_cp_fat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_cp_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ab                           : 16; /**< The job tag for the job that caused a HAB error. Note, in some
                                                         cases this tag might be incorrect and a HAB-specific register should
                                                         be used to determine the correct tag. */
	uint64_t jd_fetch                     : 16; /**< The job tag for the job that had an error when fetching the job
                                                         descriptor header and subdescriptor pointers. Note this tag is only
                                                         valid for HAB 0 and slot 0. */
	uint64_t job_sd_fetch                 : 16; /**< Job tag for the job that had an error when fetching the job
                                                         subdescriptors. */
#else
	uint64_t job_sd_fetch                 : 16;
	uint64_t jd_fetch                     : 16;
	uint64_t ab                           : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_cp_fat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_cp_fat_jtag cvmx_mhbwx_abx_sltx_cp_fat_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_cp_nfat_jtag
 *
 * These registers record the job tag for HAB nonfatal errors and MHBW control
 * path nonfatal errors.  Errors are reported for the specific HAB and slot that
 * received the error.
 */
union cvmx_mhbwx_abx_sltx_cp_nfat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_cp_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ab                           : 16; /**< The job tag for the job that caused a HAB error. Note, in some
                                                         cases this tag might be incorrect and a HAB-specific register should
                                                         be used to determine the correct tag. */
	uint64_t jd_fetch                     : 16; /**< The job tag for the job that had an error when fetching the job
                                                         descriptor header and subdescriptor pointers. Note this tag is only
                                                         valid for HAB 0 and slot 0. */
	uint64_t job_sd_fetch                 : 16; /**< Job tag for the job that had an error when fetching the job
                                                         subdescriptors. */
#else
	uint64_t job_sd_fetch                 : 16;
	uint64_t jd_fetch                     : 16;
	uint64_t ab                           : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_cp_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_cp_nfat_jtag cvmx_mhbwx_abx_sltx_cp_nfat_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_debug0
 */
union cvmx_mhbwx_abx_sltx_debug0 {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_debug0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ferr                         : 1;  /**< N/A */
	uint64_t nferr                        : 1;  /**< N/A */
	uint64_t derr                         : 1;  /**< N/A */
	uint64_t db1err                       : 1;  /**< N/A */
	uint64_t db0err                       : 1;  /**< N/A */
	uint64_t sb1err                       : 1;  /**< N/A */
	uint64_t sb0err                       : 1;  /**< N/A */
	uint64_t reserved_16_56               : 41;
	uint64_t job_id                       : 16; /**< N/A */
#else
	uint64_t job_id                       : 16;
	uint64_t reserved_16_56               : 41;
	uint64_t sb0err                       : 1;
	uint64_t sb1err                       : 1;
	uint64_t db0err                       : 1;
	uint64_t db1err                       : 1;
	uint64_t derr                         : 1;
	uint64_t nferr                        : 1;
	uint64_t ferr                         : 1;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_debug0_s   cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_debug0 cvmx_mhbwx_abx_sltx_debug0_t;

/**
 * cvmx_mhbw#_ab#_slt#_derr_ena_w1c
 */
union cvmx_mhbwx_abx_sltx_derr_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_derr_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t wr_dma_uflow_port3           : 1;  /**< Write underflow error on write port 3. */
	uint64_t wr_dma_uflow_port2           : 1;  /**< Write underflow error on write port 2. */
	uint64_t wr_dma_uflow_port1           : 1;  /**< Write underflow error on write port 1. */
	uint64_t wr_dma_uflow_port0           : 1;  /**< Write underflow error on write port 0. */
	uint64_t wr_dma_oflow_port3           : 1;  /**< Write overflow error on write port 3. */
	uint64_t wr_dma_oflow_port2           : 1;  /**< Write overflow error on write port 2. */
	uint64_t wr_dma_oflow_port1           : 1;  /**< Write overflow error on write port 1. */
	uint64_t wr_dma_oflow_port0           : 1;  /**< Write overflow error on write port 0. */
	uint64_t rd_dma_uflow_port3           : 1;  /**< Read underflow on read port 3. */
	uint64_t rd_dma_uflow_port2           : 1;  /**< Read underflow on read port 2. */
	uint64_t rd_dma_uflow_port1           : 1;  /**< Read underflow on read port 1. */
	uint64_t rd_dma_uflow_port0           : 1;  /**< Read underflow on read port 0. */
	uint64_t rd_dma_oflow_port3           : 1;  /**< Read overflow on read port 3. */
	uint64_t rd_dma_oflow_port2           : 1;  /**< Read overflow on read port 2. */
	uint64_t rd_dma_oflow_port1           : 1;  /**< Read overflow on read port 1. */
	uint64_t rd_dma_oflow_port0           : 1;  /**< Read overflow on read port 0. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t rd_dma_oflow_port1           : 1;
	uint64_t rd_dma_oflow_port2           : 1;
	uint64_t rd_dma_oflow_port3           : 1;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t rd_dma_uflow_port1           : 1;
	uint64_t rd_dma_uflow_port2           : 1;
	uint64_t rd_dma_uflow_port3           : 1;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t wr_dma_oflow_port1           : 1;
	uint64_t wr_dma_oflow_port2           : 1;
	uint64_t wr_dma_oflow_port3           : 1;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t wr_dma_uflow_port1           : 1;
	uint64_t wr_dma_uflow_port2           : 1;
	uint64_t wr_dma_uflow_port3           : 1;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_derr_ena_w1c_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_derr_ena_w1c cvmx_mhbwx_abx_sltx_derr_ena_w1c_t;

/**
 * cvmx_mhbw#_ab#_slt#_derr_ena_w1s
 */
union cvmx_mhbwx_abx_sltx_derr_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_derr_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t wr_dma_uflow_port3           : 1;  /**< Write underflow error on write port 3. */
	uint64_t wr_dma_uflow_port2           : 1;  /**< Write underflow error on write port 2. */
	uint64_t wr_dma_uflow_port1           : 1;  /**< Write underflow error on write port 1. */
	uint64_t wr_dma_uflow_port0           : 1;  /**< Write underflow error on write port 0. */
	uint64_t wr_dma_oflow_port3           : 1;  /**< Write overflow error on write port 3. */
	uint64_t wr_dma_oflow_port2           : 1;  /**< Write overflow error on write port 2. */
	uint64_t wr_dma_oflow_port1           : 1;  /**< Write overflow error on write port 1. */
	uint64_t wr_dma_oflow_port0           : 1;  /**< Write overflow error on write port 0. */
	uint64_t rd_dma_uflow_port3           : 1;  /**< Read underflow on read port 3. */
	uint64_t rd_dma_uflow_port2           : 1;  /**< Read underflow on read port 2. */
	uint64_t rd_dma_uflow_port1           : 1;  /**< Read underflow on read port 1. */
	uint64_t rd_dma_uflow_port0           : 1;  /**< Read underflow on read port 0. */
	uint64_t rd_dma_oflow_port3           : 1;  /**< Read overflow on read port 3. */
	uint64_t rd_dma_oflow_port2           : 1;  /**< Read overflow on read port 2. */
	uint64_t rd_dma_oflow_port1           : 1;  /**< Read overflow on read port 1. */
	uint64_t rd_dma_oflow_port0           : 1;  /**< Read overflow on read port 0. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t rd_dma_oflow_port1           : 1;
	uint64_t rd_dma_oflow_port2           : 1;
	uint64_t rd_dma_oflow_port3           : 1;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t rd_dma_uflow_port1           : 1;
	uint64_t rd_dma_uflow_port2           : 1;
	uint64_t rd_dma_uflow_port3           : 1;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t wr_dma_oflow_port1           : 1;
	uint64_t wr_dma_oflow_port2           : 1;
	uint64_t wr_dma_oflow_port3           : 1;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t wr_dma_uflow_port1           : 1;
	uint64_t wr_dma_uflow_port2           : 1;
	uint64_t wr_dma_uflow_port3           : 1;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_derr_ena_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_derr_ena_w1s cvmx_mhbwx_abx_sltx_derr_ena_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_derr_int
 *
 * These registers record DMA overflow/underflow errors. There is one
 * register for each job slot (SLT(0..2)) in each HAB (AB(0..3)) in each MHAB
 * (MHBW(0..19)). Note that registers only exist for the number of HABs and
 * slots in each specific MHAB.
 *
 * When an error occurs, the job tag is recorded in one of the following
 * registers:
 *
 * _ Read overflow: MHBW()_AB()_SLT()_RDERR_OFLOW_JTAG
 *
 * _ Read underflow: MHBW()_AB()_SLT()_RDERR_UFLOW_JTAG
 *
 * _ Write overflow: MHBW()_AB()_SLT()_WDERR_OFLOW_JTAG
 *
 * _ Write underflow: MHBW()_AB()_SLT()_WDERR_UFLOW_JTAG
 *
 * In some cases, read overflow/underflow errors are recorded for the wrong
 * slot, and the wrong job tag is recorded.  In these cases, the correct job
 * tag should be available in the HAB_ERROR_SOURCE0 register for the given
 * HAB.
 *
 * When an error occurs, the MHAB sends a DMA error message to PSM which then
 * triggers an interrupt, if enabled. In addition, the MHBW sends the command
 * specified by <MHBW()_DMA_ERROR_JCE_W1,MHBW()_DMA_ERROR_JCE_W0>
 * to the PSM.
 */
union cvmx_mhbwx_abx_sltx_derr_int {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_derr_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t wr_dma_uflow_port3           : 1;  /**< Write underflow error on write port 3. */
	uint64_t wr_dma_uflow_port2           : 1;  /**< Write underflow error on write port 2. */
	uint64_t wr_dma_uflow_port1           : 1;  /**< Write underflow error on write port 1. */
	uint64_t wr_dma_uflow_port0           : 1;  /**< Write underflow error on write port 0. */
	uint64_t wr_dma_oflow_port3           : 1;  /**< Write overflow error on write port 3. */
	uint64_t wr_dma_oflow_port2           : 1;  /**< Write overflow error on write port 2. */
	uint64_t wr_dma_oflow_port1           : 1;  /**< Write overflow error on write port 1. */
	uint64_t wr_dma_oflow_port0           : 1;  /**< Write overflow error on write port 0. */
	uint64_t rd_dma_uflow_port3           : 1;  /**< Read underflow on read port 3. */
	uint64_t rd_dma_uflow_port2           : 1;  /**< Read underflow on read port 2. */
	uint64_t rd_dma_uflow_port1           : 1;  /**< Read underflow on read port 1. */
	uint64_t rd_dma_uflow_port0           : 1;  /**< Read underflow on read port 0. */
	uint64_t rd_dma_oflow_port3           : 1;  /**< Read overflow on read port 3. */
	uint64_t rd_dma_oflow_port2           : 1;  /**< Read overflow on read port 2. */
	uint64_t rd_dma_oflow_port1           : 1;  /**< Read overflow on read port 1. */
	uint64_t rd_dma_oflow_port0           : 1;  /**< Read overflow on read port 0. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t rd_dma_oflow_port1           : 1;
	uint64_t rd_dma_oflow_port2           : 1;
	uint64_t rd_dma_oflow_port3           : 1;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t rd_dma_uflow_port1           : 1;
	uint64_t rd_dma_uflow_port2           : 1;
	uint64_t rd_dma_uflow_port3           : 1;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t wr_dma_oflow_port1           : 1;
	uint64_t wr_dma_oflow_port2           : 1;
	uint64_t wr_dma_oflow_port3           : 1;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t wr_dma_uflow_port1           : 1;
	uint64_t wr_dma_uflow_port2           : 1;
	uint64_t wr_dma_uflow_port3           : 1;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_derr_int_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_derr_int cvmx_mhbwx_abx_sltx_derr_int_t;

/**
 * cvmx_mhbw#_ab#_slt#_derr_int_w1s
 */
union cvmx_mhbwx_abx_sltx_derr_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_derr_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t wr_dma_uflow_port3           : 1;  /**< Write underflow error on write port 3. */
	uint64_t wr_dma_uflow_port2           : 1;  /**< Write underflow error on write port 2. */
	uint64_t wr_dma_uflow_port1           : 1;  /**< Write underflow error on write port 1. */
	uint64_t wr_dma_uflow_port0           : 1;  /**< Write underflow error on write port 0. */
	uint64_t wr_dma_oflow_port3           : 1;  /**< Write overflow error on write port 3. */
	uint64_t wr_dma_oflow_port2           : 1;  /**< Write overflow error on write port 2. */
	uint64_t wr_dma_oflow_port1           : 1;  /**< Write overflow error on write port 1. */
	uint64_t wr_dma_oflow_port0           : 1;  /**< Write overflow error on write port 0. */
	uint64_t rd_dma_uflow_port3           : 1;  /**< Read underflow on read port 3. */
	uint64_t rd_dma_uflow_port2           : 1;  /**< Read underflow on read port 2. */
	uint64_t rd_dma_uflow_port1           : 1;  /**< Read underflow on read port 1. */
	uint64_t rd_dma_uflow_port0           : 1;  /**< Read underflow on read port 0. */
	uint64_t rd_dma_oflow_port3           : 1;  /**< Read overflow on read port 3. */
	uint64_t rd_dma_oflow_port2           : 1;  /**< Read overflow on read port 2. */
	uint64_t rd_dma_oflow_port1           : 1;  /**< Read overflow on read port 1. */
	uint64_t rd_dma_oflow_port0           : 1;  /**< Read overflow on read port 0. */
#else
	uint64_t rd_dma_oflow_port0           : 1;
	uint64_t rd_dma_oflow_port1           : 1;
	uint64_t rd_dma_oflow_port2           : 1;
	uint64_t rd_dma_oflow_port3           : 1;
	uint64_t rd_dma_uflow_port0           : 1;
	uint64_t rd_dma_uflow_port1           : 1;
	uint64_t rd_dma_uflow_port2           : 1;
	uint64_t rd_dma_uflow_port3           : 1;
	uint64_t wr_dma_oflow_port0           : 1;
	uint64_t wr_dma_oflow_port1           : 1;
	uint64_t wr_dma_oflow_port2           : 1;
	uint64_t wr_dma_oflow_port3           : 1;
	uint64_t wr_dma_uflow_port0           : 1;
	uint64_t wr_dma_uflow_port1           : 1;
	uint64_t wr_dma_uflow_port2           : 1;
	uint64_t wr_dma_uflow_port3           : 1;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_derr_int_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_derr_int_w1s cvmx_mhbwx_abx_sltx_derr_int_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_fat_err_ena_w1c
 */
union cvmx_mhbwx_abx_sltx_fat_err_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_fat_err_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a fatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a fatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_fat_err_ena_w1c_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_fat_err_ena_w1c cvmx_mhbwx_abx_sltx_fat_err_ena_w1c_t;

/**
 * cvmx_mhbw#_ab#_slt#_fat_err_ena_w1s
 */
union cvmx_mhbwx_abx_sltx_fat_err_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_fat_err_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a fatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a fatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_fat_err_ena_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_fat_err_ena_w1s cvmx_mhbwx_abx_sltx_fat_err_ena_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_fat_err_int
 *
 * These registers record fatal errors. There is one
 * register for each job slot (SLT(0..2)) in each HAB (AB(0..3)) in each MHAB
 * (MHBW(0..19)). Note that registers only exist for the number of HABs and
 * slots in each specific MHAB.
 *
 * Fatal errors include:
 * * ECC double-bit errors on SMEM accesses.
 * * errors when accessing memory.
 * * ECC double-bit errors in internal HAB memories.
 * * HAB-specific fatal errors.
 *
 * When an error occurs, the job tag is recorded in one of the following
 * registers:
 *
 * _ Error on read DMA: MHBW()_AB()_SLT()_RD_FAT_JTAG
 *
 * _ Error on write DMA: MHBW()_AB()_SLT()_WR_FAT_JTAG
 *
 * _ Other fatal errors: MHBW()_AB()_SLT()_CP_FAT_JTAG
 *
 * When an error occurs, the MHAB sends a fatal error message to PSM which then
 * triggers an interrupt, if enabled. In addition, the MHBW sends the command
 * specified by <MHBW()_FATAL_ERROR_JCE_W1,MHBW()_FATAL_ERROR_JCE_W0>
 * to the PSM.
 */
union cvmx_mhbwx_abx_sltx_fat_err_int {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_fat_err_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a fatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a fatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_fat_err_int_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_fat_err_int cvmx_mhbwx_abx_sltx_fat_err_int_t;

/**
 * cvmx_mhbw#_ab#_slt#_fat_err_int_w1s
 */
union cvmx_mhbwx_abx_sltx_fat_err_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_fat_err_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a fatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a fatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_fat_err_int_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_fat_err_int_w1s cvmx_mhbwx_abx_sltx_fat_err_int_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_nfat_err_ena_w1c
 */
union cvmx_mhbwx_abx_sltx_nfat_err_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_nfat_err_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Non-fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a nonfatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a nonfatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Non-fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Non-fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Non-fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Non-fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Non-fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Non-fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Non-fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Non-fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_nfat_err_ena_w1c_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_nfat_err_ena_w1c cvmx_mhbwx_abx_sltx_nfat_err_ena_w1c_t;

/**
 * cvmx_mhbw#_ab#_slt#_nfat_err_ena_w1s
 */
union cvmx_mhbwx_abx_sltx_nfat_err_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_nfat_err_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Non-fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a nonfatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a nonfatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Non-fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Non-fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Non-fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Non-fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Non-fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Non-fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Non-fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Non-fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_nfat_err_ena_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_nfat_err_ena_w1s cvmx_mhbwx_abx_sltx_nfat_err_ena_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_nfat_err_int
 *
 * These registers record nonfatal errors. There is one
 * register for each job slot (SLT(0..2)) in each HAB (AB(0..3)) in each MHAB
 * (MHBW(0..19)). Note that registers only exist for the number of HABs and
 * slots in each specific MHAB.
 *
 * Fatal errors include:
 * * ECC single-bit errors on SMEM accesses.
 * * ECC single-bit errors in internal HAB memories.
 * * HAB-specific fatal errors.
 *
 * When an error occurs, the job tag is recorded in one of the following
 * registers:
 *
 * _ Error on read DMA: MHBW()_AB()_SLT()_RD_NFAT_JTAG
 *
 * _ Error on write DMA: MHBW()_AB()_SLT()_WR_NFAT_JTAG
 *
 * _ Other nonfatal errors: MHBW()_AB()_SLT()_CP_NFAT_JTAG
 *
 * When an error occurs, the MHAB sends a fatal error message to PSM which then
 * triggers an interrupt, if enabled. In addition, the MHBW sends the command
 * specified by <MHBW()_NON_FATAL_ERROR_JCE_W1,MHBW()_NON_FATAL_ERROR_JCE_W0>
 * to the PSM.
 */
union cvmx_mhbwx_abx_sltx_nfat_err_int {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_nfat_err_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Non-fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a nonfatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a nonfatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Non-fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Non-fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Non-fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Non-fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Non-fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Non-fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Non-fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Non-fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_nfat_err_int_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_nfat_err_int cvmx_mhbwx_abx_sltx_nfat_err_int_t;

/**
 * cvmx_mhbw#_ab#_slt#_nfat_err_int_w1s
 */
union cvmx_mhbwx_abx_sltx_nfat_err_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_nfat_err_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t ab                           : 1;  /**< Non-fatal error reported by the HAB. */
	uint64_t jd_fetch                     : 1;  /**< Job descriptor header or subdescriptor pointer fetch received a
                                                         response with a nonfatal error.  Note that these errors are always
                                                         reported for HAB 0, slot 0. */
	uint64_t job_sd_fetch                 : 1;  /**< Job subdescriptor fetch received a response with a nonfatal error. */
	uint64_t reserved_12_15               : 4;
	uint64_t wr_dma_port3                 : 1;  /**< Non-fatal error on write DMA response for write port 3. */
	uint64_t wr_dma_port2                 : 1;  /**< Non-fatal error on write DMA response for write port 2. */
	uint64_t wr_dma_port1                 : 1;  /**< Non-fatal error on write DMA response for write port 1. */
	uint64_t wr_dma_port0                 : 1;  /**< Non-fatal error on write DMA response for write port 0. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rd_dma_port3                 : 1;  /**< Non-fatal error on read DMA response for read port 3. */
	uint64_t rd_dma_port2                 : 1;  /**< Non-fatal error on read DMA response for read port 2. */
	uint64_t rd_dma_port1                 : 1;  /**< Non-fatal error on read DMA response for read port 1. */
	uint64_t rd_dma_port0                 : 1;  /**< Non-fatal error on read DMA response for read port 0. */
#else
	uint64_t rd_dma_port0                 : 1;
	uint64_t rd_dma_port1                 : 1;
	uint64_t rd_dma_port2                 : 1;
	uint64_t rd_dma_port3                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t wr_dma_port0                 : 1;
	uint64_t wr_dma_port1                 : 1;
	uint64_t wr_dma_port2                 : 1;
	uint64_t wr_dma_port3                 : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_sd_fetch                 : 1;
	uint64_t jd_fetch                     : 1;
	uint64_t ab                           : 1;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_nfat_err_int_w1s_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_nfat_err_int_w1s cvmx_mhbwx_abx_sltx_nfat_err_int_w1s_t;

/**
 * cvmx_mhbw#_ab#_slt#_rd_fat_jtag
 *
 * These registers record the job tag when a read DMA receives a response
 * indicating a fatal error. Errors are reported for the specific HAB and slot
 * that received the error.
 */
union cvmx_mhbwx_abx_sltx_rd_fat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_rd_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_rd_fat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_rd_fat_jtag cvmx_mhbwx_abx_sltx_rd_fat_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_rd_nfat_jtag
 *
 * These registers record the job tag when a read DMA receives a response
 * indicating a nonfatal error. Errors are reported for the specific HAB and slot
 * that received the error.
 */
union cvmx_mhbwx_abx_sltx_rd_nfat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_rd_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_rd_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_rd_nfat_jtag cvmx_mhbwx_abx_sltx_rd_nfat_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_rderr_oflow_jtag
 *
 * These registers record the job tag when a read DMA overflow occurs for a
 * given slot on a given HAB. In some cases, the wrong job tag will be
 * recorded for the wrong slot. In these cases, the HAB_ERROR_SOURCE0
 * register for the given HAB should report the correct job tag.
 */
union cvmx_mhbwx_abx_sltx_rderr_oflow_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_rderr_oflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read overflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read overflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read overflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read overflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_rderr_oflow_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_rderr_oflow_jtag cvmx_mhbwx_abx_sltx_rderr_oflow_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_rderr_uflow_jtag
 *
 * These registers record the job tag when a read DMA underflow occurs for a
 * given slot on a given HAB. In some cases, the wrong job tag will be
 * recorded for the wrong slot. In these cases, the HAB_ERROR_SOURCE0
 * register for the given HAB should report the correct job tag.
 */
union cvmx_mhbwx_abx_sltx_rderr_uflow_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_rderr_uflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_rderr_uflow_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_rderr_uflow_jtag cvmx_mhbwx_abx_sltx_rderr_uflow_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_wderr_oflow_jtag
 *
 * These registers record the job tag when a write DMA overflow occurs for a
 * given slot on a given HAB. Write errors are always recorded correctly.
 */
union cvmx_mhbwx_abx_sltx_wderr_oflow_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_wderr_oflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read overflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read overflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read overflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read overflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_wderr_oflow_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_wderr_oflow_jtag cvmx_mhbwx_abx_sltx_wderr_oflow_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_wderr_uflow_jtag
 *
 * These registers record the job tag when a write DMA overflow occurs for a
 * given slot on a given HAB. Write errors are always recorded correctly.
 */
union cvmx_mhbwx_abx_sltx_wderr_uflow_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_wderr_uflow_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_wderr_uflow_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_wderr_uflow_jtag cvmx_mhbwx_abx_sltx_wderr_uflow_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_wr_fat_jtag
 *
 * These registers record the job tag when a write DMA receives a response
 * indicating a fatal error. Errors are reported for the specific HAB and slot
 * that received the error.
 */
union cvmx_mhbwx_abx_sltx_wr_fat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_wr_fat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_wr_fat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_wr_fat_jtag cvmx_mhbwx_abx_sltx_wr_fat_jtag_t;

/**
 * cvmx_mhbw#_ab#_slt#_wr_nfat_jtag
 *
 * These registers record the job tag when a write DMA receives a response
 * indicating a nonfatal error. Errors are reported for the specific HAB and slot
 * that received the error.
 */
union cvmx_mhbwx_abx_sltx_wr_nfat_jtag {
	uint64_t u64;
	struct cvmx_mhbwx_abx_sltx_wr_nfat_jtag_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t port3                        : 16; /**< The job tag when a read underflow occurred on port 3. */
	uint64_t port2                        : 16; /**< The job tag when a read underflow occurred on port 2. */
	uint64_t port1                        : 16; /**< The job tag when a read underflow occurred on port 1. */
	uint64_t port0                        : 16; /**< The job tag when a read underflow occurred on port 0. */
#else
	uint64_t port0                        : 16;
	uint64_t port1                        : 16;
	uint64_t port2                        : 16;
	uint64_t port3                        : 16;
#endif
	} s;
	struct cvmx_mhbwx_abx_sltx_wr_nfat_jtag_s cnf75xx;
};
typedef union cvmx_mhbwx_abx_sltx_wr_nfat_jtag cvmx_mhbwx_abx_sltx_wr_nfat_jtag_t;

/**
 * cvmx_mhbw#_cfg
 *
 * These registers specify the clock, reset and priority configuration of
 * each MHAB.
 */
union cvmx_mhbwx_cfg {
	uint64_t u64;
	struct cvmx_mhbwx_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t mhbw_done                    : 1;  /**< Indicates the MHBW has been disabled and has finished clearing all
                                                         internal state. When resetting the MHBW, software must wait until this
                                                         bit is set before re-enabling the MHBW via MHBW()_CFG[ENA]. */
	uint64_t ab_done                      : 4;  /**< For each HAB, these bits indicate when it has been disabled and
                                                         all associated state has been cleared. When resetting a HAB, software
                                                         must wait until the corresponding bit is set before re-enabling a HAB
                                                         using MHBW()_CFG[AB_ENA]. */
	uint64_t reserved_29_45               : 17;
	uint64_t mhbw_stopped                 : 1;  /**< N/A */
	uint64_t ab_stopped                   : 4;  /**< N/A */
	uint64_t ab_busy                      : 4;  /**< Each bit, when set to 1, indicates a HAB is busy processing a job. */
	uint64_t ab_ena                       : 4;  /**< Enable or disable each HAB. HABs must be disabled one at a time. When
                                                         a HAB is disabled, software must wait until the corresponding bit in
                                                         MHBW()_CFG[AB_DONE] is set before disabling the next HAB, or before
                                                         re-enabling a HAB. */
	uint64_t ab_clk_gating_ena            : 4;  /**< When set, each bit enables automatic clock gating for one HAB. This
                                                         will automatically turn off the clock to a HAB when it does not have
                                                         any jobs to process. When a bit is cleared to zero, the clock for the corresponding
                                                         HAB will always be cycled, even when the HAB has no jobs assigned. */
	uint64_t ab_clk_toggle_flp_rst_n      : 4;  /**< This set of bits, specifies the active low reset to toggle flop of the integrated clock
                                                         gating (ICG) cell, for clock predictability corresponding to each AB. This is used for
                                                         AB's, which work in a different clock domain than the MHBW. */
	uint64_t ab_clk_ena                   : 4;  /**< When set, each bit enables the conditional clock to one HAB. When a
                                                         bit is cleared to zero, the corresponding HAB will receive no clock. */
	uint64_t reserved_3_3                 : 1;
	uint64_t job_slot_ena                 : 1;  /**< Enable job pipelining.
                                                         0 = Jobs are only scheduled to slot 0 for each HAB.
                                                         1 = Jobs are scheduled to all slots supported by the HAB, allowing
                                                         multiple jobs to be active at once.
                                                         Note that software must ensure the MHBW is in an idle state, i.e, no
                                                         active or pending jobs, before setting this bit. */
	uint64_t ena                          : 1;  /**< When set enables the MHBW. When cleared to zero, disables the MHBW.
                                                         Note that all HABs must be disabled using MHBW()_CFG[AB_ENA] before
                                                         disabling the MHBW.
                                                         While disabled, any jobs scheduled to the MHAB will be dropped.
                                                         After disabling the MHBW, software must wait until
                                                         MHBW()_CFG[MHBW_DONE] is set before re-enabling it. */
	uint64_t clk_ena                      : 1;  /**< When set, enables the MHBW conditional clock. */
#else
	uint64_t clk_ena                      : 1;
	uint64_t ena                          : 1;
	uint64_t job_slot_ena                 : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t ab_clk_ena                   : 4;
	uint64_t ab_clk_toggle_flp_rst_n      : 4;
	uint64_t ab_clk_gating_ena            : 4;
	uint64_t ab_ena                       : 4;
	uint64_t ab_busy                      : 4;
	uint64_t ab_stopped                   : 4;
	uint64_t mhbw_stopped                 : 1;
	uint64_t reserved_29_45               : 17;
	uint64_t ab_done                      : 4;
	uint64_t mhbw_done                    : 1;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_mhbwx_cfg_s               cnf75xx;
};
typedef union cvmx_mhbwx_cfg cvmx_mhbwx_cfg_t;

/**
 * cvmx_mhbw#_dma_error_jce_w0
 *
 * This register specifies the low 64 bits of a PSM command issued when the
 * MHBW has a DMA error.
 */
union cvmx_mhbwx_dma_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mhbwx_dma_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< The first word of the PSM command to send on a DMA error. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_dma_error_jce_w0_s  cnf75xx;
};
typedef union cvmx_mhbwx_dma_error_jce_w0 cvmx_mhbwx_dma_error_jce_w0_t;

/**
 * cvmx_mhbw#_dma_error_jce_w1
 *
 * This register specifies the high 64 bits of a PSM command issued when the
 * MHBW has a DMA error.
 */
union cvmx_mhbwx_dma_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mhbwx_dma_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< The second word of the PSM command to send on a DMA error. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_dma_error_jce_w1_s  cnf75xx;
};
typedef union cvmx_mhbwx_dma_error_jce_w1 cvmx_mhbwx_dma_error_jce_w1_t;

/**
 * cvmx_mhbw#_eco
 */
union cvmx_mhbwx_eco {
	uint64_t u64;
	struct cvmx_mhbwx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_mhbwx_eco_s               cnf75xx;
};
typedef union cvmx_mhbwx_eco cvmx_mhbwx_eco_t;

/**
 * cvmx_mhbw#_err_stat0
 *
 * These registers count fatal and nonfatal errors (excluding ECC errors in
 * MHBW memories). Note that if multiple HABs have errors in the exact same
 * cycle, only one is counted. Both counters wrap to zero at 2^32.
 */
union cvmx_mhbwx_err_stat0 {
	uint64_t u64;
	struct cvmx_mhbwx_err_stat0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nfat_err_stat                : 32; /**< Count of nonfatal errors. */
	uint64_t fat_err_stat                 : 32; /**< Count of fatal errors. */
#else
	uint64_t fat_err_stat                 : 32;
	uint64_t nfat_err_stat                : 32;
#endif
	} s;
	struct cvmx_mhbwx_err_stat0_s         cnf75xx;
};
typedef union cvmx_mhbwx_err_stat0 cvmx_mhbwx_err_stat0_t;

/**
 * cvmx_mhbw#_err_stat1
 *
 * These registers count DMA errors for each MHAB. Note that if errors are
 * reported on multiple ports, or for multiple HABs in the same cycle, then
 * only one error is counted. The counter wraps to zero at 2^32.
 */
union cvmx_mhbwx_err_stat1 {
	uint64_t u64;
	struct cvmx_mhbwx_err_stat1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t dma_err_stat                 : 32; /**< Count of DMA errors. */
#else
	uint64_t dma_err_stat                 : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mhbwx_err_stat1_s         cnf75xx;
};
typedef union cvmx_mhbwx_err_stat1 cvmx_mhbwx_err_stat1_t;

/**
 * cvmx_mhbw#_ext#_mem_bist_status
 *
 * These registers provide BIST status for HAB memories for specific
 * HABs that do not have internal BIST registers.  Each bit is the BIST
 * result of an individual memory (per bit, 0 = pass (or never run) and 1 =
 * fail).
 */
union cvmx_mhbwx_extx_mem_bist_status {
	uint64_t u64;
	struct cvmx_mhbwx_extx_mem_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ext                          : 64; /**< N/A */
#else
	uint64_t ext                          : 64;
#endif
	} s;
	struct cvmx_mhbwx_extx_mem_bist_status_s cnf75xx;
};
typedef union cvmx_mhbwx_extx_mem_bist_status cvmx_mhbwx_extx_mem_bist_status_t;

/**
 * cvmx_mhbw#_ext#_sfunc
 *
 * This register set, specifies any special MHBW direct CSR functionality that is required by a HAB.
 *
 */
union cvmx_mhbwx_extx_sfunc {
	uint64_t u64;
	struct cvmx_mhbwx_extx_sfunc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ext                          : 64; /**< Please see corresponding HAB for special function definitions. */
#else
	uint64_t ext                          : 64;
#endif
	} s;
	struct cvmx_mhbwx_extx_sfunc_s        cnf75xx;
};
typedef union cvmx_mhbwx_extx_sfunc cvmx_mhbwx_extx_sfunc_t;

/**
 * cvmx_mhbw#_fatal_error_jce_w0
 *
 * This register specifies the low 64 bits of a PSM command issued when the
 * MHBW or one of its HABs suffer a fatal error.
 */
union cvmx_mhbwx_fatal_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mhbwx_fatal_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< The first word of the PSM command to send on a fatal error. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_fatal_error_jce_w0_s cnf75xx;
};
typedef union cvmx_mhbwx_fatal_error_jce_w0 cvmx_mhbwx_fatal_error_jce_w0_t;

/**
 * cvmx_mhbw#_fatal_error_jce_w1
 *
 * This register specifies the high 64 bits of a PSM command issued when the
 * MHBW or one of its HABs suffer a fatal error.
 */
union cvmx_mhbwx_fatal_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mhbwx_fatal_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< The second word of the PSM command to send on a fatal error. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_fatal_error_jce_w1_s cnf75xx;
};
typedef union cvmx_mhbwx_fatal_error_jce_w1 cvmx_mhbwx_fatal_error_jce_w1_t;

/**
 * cvmx_mhbw#_fyi
 *
 * These registers detail the specification of each MHBW.
 *
 */
union cvmx_mhbwx_fyi {
	uint64_t u64;
	struct cvmx_mhbwx_fyi_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t psm_did                      : 6;  /**< The PSM ID used to schedule jobs on this MHAB (i.e., the bit to set in
                                                         PSM_RSRC_TBL()[MAB_MAP] to map a job type to this MHAB). */
	uint64_t mhb_did                      : 5;  /**< The MHAB DID. */
	uint64_t clk_type                     : 1;  /**< The clock type for the HABs in this MHAB.
                                                         0 = SCLK.
                                                         1 = SCLK/2. */
	uint64_t reserved_40_51               : 12;
	uint64_t ab_cores                     : 2;  /**< The number of HABs in the MHAB, minus 1. */
	uint64_t ab_cfg_credit_ena            : 1;  /**< N/A */
	uint64_t num_ab_rd_ports              : 2;  /**< The number of read ports for each HAB, minus 1. */
	uint64_t num_ab_wr_ports              : 2;  /**< The number of write ports for each HAB, minus 1. */
	uint64_t num_ab_job_slots             : 2;  /**< The number of job slots available in each HAB, minus 1. */
	uint64_t ab_rd_port_0                 : 2;  /**< The width of read port 0, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. */
	uint64_t ab_rd_port_1                 : 2;  /**< The width of read port 1, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_RD_PORTS] < 1. */
	uint64_t ab_rd_port_2                 : 2;  /**< The width of read port 2, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_RD_PORTS] < 2. */
	uint64_t ab_rd_port_3                 : 2;  /**< The width of read port 3, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_RD_PORTS] < 3. */
	uint64_t ab_wr_port_0                 : 2;  /**< The width of write port 0, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. */
	uint64_t ab_wr_port_1                 : 2;  /**< The width of write port 1, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_WR_PORTS] < 1. */
	uint64_t ab_wr_port_2                 : 2;  /**< The width of write port 2, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_WR_PORTS] < 2. */
	uint64_t ab_wr_port_3                 : 2;  /**< The width of write port 3, specified using the enumeration
                                                         MHBW_AB_PORT_WIDTH_E. Value is undefined if
                                                         MHBW()_FYI[NUM_AB_WR_PORTS] < 3. */
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
	uint64_t mhb_did                      : 5;
	uint64_t psm_did                      : 6;
#endif
	} s;
	struct cvmx_mhbwx_fyi_s               cnf75xx;
};
typedef union cvmx_mhbwx_fyi cvmx_mhbwx_fyi_t;

/**
 * cvmx_mhbw#_jd_cfg
 *
 * These registers specify additional MHBW configuration parameters.
 *
 */
union cvmx_mhbwx_jd_cfg {
	uint64_t u64;
	struct cvmx_mhbwx_jd_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_38_63               : 26;
	uint64_t ghb_rd_wrr_val               : 6;  /**< The weighted round-robin arbitration weight used in the GHB when
                                                         arbitrating for read requests from this MHAB. A value of zero will
                                                         only allow requests when there are no competing requests from other
                                                         MHABs in the same GHB, or when all competing MHABs have an effective
                                                         weight of zero. A higher weight guarantees a greater share of the GHB
                                                         bandwidth. */
	uint64_t reserved_30_31               : 2;
	uint64_t ghb_wr_wrr_val               : 6;  /**< The weighted round-robin arbitration weight used in the GHB when
                                                         arbitrating for read requests from this MHAB. A value of zero will
                                                         only allow requests when there are no competing requests from other
                                                         MHABs in the same GHB, or when all competing MHABs have an effective
                                                         weight of zero. A higher weight guarantees a greater share of the GHB
                                                         bandwidth. */
	uint64_t reserved_10_23               : 14;
	uint64_t ghb_wr_priority              : 1;  /**< Specifies the priority of write requests from this MHAB. Priority levels
                                                         are enumerated in MHBW_GHB_JOB_PRIORITY_E. */
	uint64_t ghb_rd_priority              : 1;  /**< Specifies the priority of read requests from this MHAB. Priority levels
                                                         are enumerated in MHBW_GHB_JOB_PRIORITY_E. */
	uint64_t timeout_mult                 : 4;  /**< This set of bits, specifies the timeout multiplier used when
                                                         calculating job timeout thresholds. See MHBW_JD_HDR_WORD_0_S[TOTH] for
                                                         details. */
	uint64_t reserved_2_3                 : 2;
	uint64_t jd_fetch_cmd_type            : 2;  /**< Specifies the command type used when reading a job descriptor
                                                         (including subdescriptor sections) from main memory. Command types
                                                         are enumerated in MHBW_PNB_RD_CMD_E.  Note that if
                                                         MHBW_PNB_RD_CMD_E::LDWB is specified, then any requests for less than
                                                         128 bytes will be automatically converted to type LDT to avoid
                                                         accidental loss of data. */
#else
	uint64_t jd_fetch_cmd_type            : 2;
	uint64_t reserved_2_3                 : 2;
	uint64_t timeout_mult                 : 4;
	uint64_t ghb_rd_priority              : 1;
	uint64_t ghb_wr_priority              : 1;
	uint64_t reserved_10_23               : 14;
	uint64_t ghb_wr_wrr_val               : 6;
	uint64_t reserved_30_31               : 2;
	uint64_t ghb_rd_wrr_val               : 6;
	uint64_t reserved_38_63               : 26;
#endif
	} s;
	struct cvmx_mhbwx_jd_cfg_s            cnf75xx;
};
typedef union cvmx_mhbwx_jd_cfg cvmx_mhbwx_jd_cfg_t;

/**
 * cvmx_mhbw#_job_compl_stat
 *
 * These registers count the number of jobs completed by each MHAB.
 *
 */
union cvmx_mhbwx_job_compl_stat {
	uint64_t u64;
	struct cvmx_mhbwx_job_compl_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t stat                         : 48; /**< The counter value. Wraps to zero at 2^48. */
#else
	uint64_t stat                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mhbwx_job_compl_stat_s    cnf75xx;
};
typedef union cvmx_mhbwx_job_compl_stat cvmx_mhbwx_job_compl_stat_t;

/**
 * cvmx_mhbw#_job_drop_stat
 *
 * These registers count the number of jobs dropped by each MHAB. Dropped
 * jobs include any jobs received when the MHBW is disabled or when all HABs
 * are disabled, and any jobs active on a HAB when it is disabled.
 */
union cvmx_mhbwx_job_drop_stat {
	uint64_t u64;
	struct cvmx_mhbwx_job_drop_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t stat                         : 32; /**< The counter value. Wraps to zero at 2^32. */
#else
	uint64_t stat                         : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mhbwx_job_drop_stat_s     cnf75xx;
};
typedef union cvmx_mhbwx_job_drop_stat cvmx_mhbwx_job_drop_stat_t;

/**
 * cvmx_mhbw#_job_enqueue_stat
 *
 * These registers count the number of job en-queued per MHAB.
 *
 */
union cvmx_mhbwx_job_enqueue_stat {
	uint64_t u64;
	struct cvmx_mhbwx_job_enqueue_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t stat                         : 48; /**< The counter value. Wraps to zero at 2^48. */
#else
	uint64_t stat                         : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_mhbwx_job_enqueue_stat_s  cnf75xx;
};
typedef union cvmx_mhbwx_job_enqueue_stat cvmx_mhbwx_job_enqueue_stat_t;

/**
 * cvmx_mhbw#_mem_bist_status0
 *
 * These registers provide access to the internal BIST results for MHBW
 * memories.  Each bit is the BIST result of an individual memory (per bit, 0
 * = pass (or never run) and 1 = fail).
 */
union cvmx_mhbwx_mem_bist_status0 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< N/A */
	uint64_t blk1                         : 8;  /**< N/A */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mhbwx_mem_bist_status0_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_bist_status0 cvmx_mhbwx_mem_bist_status0_t;

/**
 * cvmx_mhbw#_mem_bist_status1
 *
 * These registers provide access to the internal BIST results for MHBW
 * memories.  Each bit is the BIST result of an individual memory (per bit, 0
 * = pass (or never run) and 1 = fail).
 */
union cvmx_mhbwx_mem_bist_status1 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_bist_status1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk3                         : 64; /**< N/A */
#else
	uint64_t blk3                         : 64;
#endif
	} s;
	struct cvmx_mhbwx_mem_bist_status1_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_bist_status1 cvmx_mhbwx_mem_bist_status1_t;

/**
 * cvmx_mhbw#_mem_cor_dis0
 *
 * These registers disable ECC correction for single-bit errors in internal
 * MHBW memories. Each bit corresponds to a different internal memory, and
 * setting a bit disable correction for that memory.
 */
union cvmx_mhbwx_mem_cor_dis0 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_cor_dis0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< Disable correction in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< Disable correction in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mhbwx_mem_cor_dis0_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_cor_dis0 cvmx_mhbwx_mem_cor_dis0_t;

/**
 * cvmx_mhbw#_mem_cor_dis1
 *
 * These registers disable ECC correction for single-bit errors in internal
 * MHBW memories. Each bit corresponds to a different internal memory, and
 * setting a bit disable correction for that memory.
 */
union cvmx_mhbwx_mem_cor_dis1 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_cor_dis1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk3                         : 64; /**< Disable correction in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 64;
#endif
	} s;
	struct cvmx_mhbwx_mem_cor_dis1_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_cor_dis1 cvmx_mhbwx_mem_cor_dis1_t;

/**
 * cvmx_mhbw#_mem_dbe0_ena_w1c
 */
union cvmx_mhbwx_mem_dbe0_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe0_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents DBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents DBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe0_ena_w1c_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe0_ena_w1c cvmx_mhbwx_mem_dbe0_ena_w1c_t;

/**
 * cvmx_mhbw#_mem_dbe0_ena_w1s
 */
union cvmx_mhbwx_mem_dbe0_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe0_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents DBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents DBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe0_ena_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe0_ena_w1s cvmx_mhbwx_mem_dbe0_ena_w1s_t;

/**
 * cvmx_mhbw#_mem_dbe0_int
 *
 * These registers report double-bit ECC errors for internal MHBW memories.
 * On a double-bit ECC error, the MHBW will report a fatal error to the
 * PSM which then triggers an interrupt, if enable. In addition, the MHBW sends
 * the command specified by
 * <MHBW()_FATAL_ERROR_JCE_W1,MHBW()_FATAL_ERROR_JCE_W0> to the PSM.
 *
 * Write one to each bit to clear it and allow reporting of future errors.
 */
union cvmx_mhbwx_mem_dbe0_int {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe0_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents DBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents DBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe0_int_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe0_int cvmx_mhbwx_mem_dbe0_int_t;

/**
 * cvmx_mhbw#_mem_dbe0_int_w1s
 */
union cvmx_mhbwx_mem_dbe0_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe0_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents DBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents DBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe0_int_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe0_int_w1s cvmx_mhbwx_mem_dbe0_int_w1s_t;

/**
 * cvmx_mhbw#_mem_dbe1_ena_w1c
 */
union cvmx_mhbwx_mem_dbe1_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe1_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents DBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe1_ena_w1c_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe1_ena_w1c cvmx_mhbwx_mem_dbe1_ena_w1c_t;

/**
 * cvmx_mhbw#_mem_dbe1_ena_w1s
 */
union cvmx_mhbwx_mem_dbe1_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe1_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents DBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe1_ena_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe1_ena_w1s cvmx_mhbwx_mem_dbe1_ena_w1s_t;

/**
 * cvmx_mhbw#_mem_dbe1_int
 *
 * These registers report double-bit ECC errors for internal MHBW memories.
 * On a double-bit ECC error, the MHBW will report a fatal error to the
 * PSM which then triggers an interrupt, if enable. In addition, the MHBW sends
 * the command specified by
 * <MHBW()_FATAL_ERROR_JCE_W1,MHBW()_FATAL_ERROR_JCE_W0> to the PSM.
 *
 * Write one to each bit to clear it and allow reporting of future errors.
 */
union cvmx_mhbwx_mem_dbe1_int {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe1_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents DBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe1_int_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe1_int cvmx_mhbwx_mem_dbe1_int_t;

/**
 * cvmx_mhbw#_mem_dbe1_int_w1s
 */
union cvmx_mhbwx_mem_dbe1_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_dbe1_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents DBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_dbe1_int_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_dbe1_int_w1s cvmx_mhbwx_mem_dbe1_int_w1s_t;

/**
 * cvmx_mhbw#_mem_flip_synd0
 *
 * These registers flip the ECC syndrome for internal MHBW memories to allow
 * for testing of the ECC mechanisms.
 *
 * Each internal memory has two consecutive bits. Setting one of the two bits will cause
 * a single-bit error on the next access, and setting both bits will cause a
 * double-bit error on the next access.
 */
union cvmx_mhbwx_mem_flip_synd0 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_flip_synd0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk2                         : 56; /**< Flip syndromes for DMA related memories for HAB 0. */
	uint64_t blk1                         : 8;  /**< Flip syndromes for job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 56;
#endif
	} s;
	struct cvmx_mhbwx_mem_flip_synd0_s    cnf75xx;
};
typedef union cvmx_mhbwx_mem_flip_synd0 cvmx_mhbwx_mem_flip_synd0_t;

/**
 * cvmx_mhbw#_mem_flip_synd1
 *
 * These registers flip the ECC syndrome for internal MHBW memories to allow
 * for testing of the ECC mechanisms.
 *
 * Each internal memory has two consecutive bits. Setting one of the two bits will cause
 * a single-bit error on the next access, and setting both bits will cause a
 * double-bit error on the next access.
 */
union cvmx_mhbwx_mem_flip_synd1 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_flip_synd1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk3                         : 64; /**< Flip syndromes for DMA related memories for HAB 1. */
#else
	uint64_t blk3                         : 64;
#endif
	} s;
	struct cvmx_mhbwx_mem_flip_synd1_s    cnf75xx;
};
typedef union cvmx_mhbwx_mem_flip_synd1 cvmx_mhbwx_mem_flip_synd1_t;

/**
 * cvmx_mhbw#_mem_flip_synd2
 *
 * These registers flip the ECC syndrome for internal MHBW memories to allow
 * for testing of the ECC mechanisms.
 *
 * Each internal memory has two consecutive bits. Setting one of the two bits will cause
 * a single-bit error on the next access, and setting both bits will cause a
 * double-bit error on the next access.
 */
union cvmx_mhbwx_mem_flip_synd2 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_flip_synd2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk4                         : 64; /**< Flip syndromes for DMA related memories for HAB 2. */
#else
	uint64_t blk4                         : 64;
#endif
	} s;
	struct cvmx_mhbwx_mem_flip_synd2_s    cnf75xx;
};
typedef union cvmx_mhbwx_mem_flip_synd2 cvmx_mhbwx_mem_flip_synd2_t;

/**
 * cvmx_mhbw#_mem_flip_synd3
 *
 * These registers flip the ECC syndrome for internal MHBW memories to allow
 * for testing of the ECC mechanisms.
 *
 * Each internal memory has two consecutive bits. Setting one of the two bits will cause
 * a single-bit error on the next access, and setting both bits will cause a
 * double-bit error on the next access.
 */
union cvmx_mhbwx_mem_flip_synd3 {
	uint64_t u64;
	struct cvmx_mhbwx_mem_flip_synd3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t blk5                         : 64; /**< Flip syndromes for DMA related memories for HAB 3. */
#else
	uint64_t blk5                         : 64;
#endif
	} s;
	struct cvmx_mhbwx_mem_flip_synd3_s    cnf75xx;
};
typedef union cvmx_mhbwx_mem_flip_synd3 cvmx_mhbwx_mem_flip_synd3_t;

/**
 * cvmx_mhbw#_mem_sbe0_ena_w1c
 */
union cvmx_mhbwx_mem_sbe0_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe0_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents SBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents SBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe0_ena_w1c_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe0_ena_w1c cvmx_mhbwx_mem_sbe0_ena_w1c_t;

/**
 * cvmx_mhbw#_mem_sbe0_ena_w1s
 */
union cvmx_mhbwx_mem_sbe0_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe0_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents SBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents SBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe0_ena_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe0_ena_w1s cvmx_mhbwx_mem_sbe0_ena_w1s_t;

/**
 * cvmx_mhbw#_mem_sbe0_int
 *
 * These registers report single-bit ECC errors for internal MHBW memories.
 * On a single-bit ECC error, the MHBW will report a nonfatal error to the
 * PSM which then triggers an interrupt, if enable. In addition, the MHBW sends
 * the command specified by
 * <MHBW()_NON_FATAL_ERROR_JCE_W1,MHBW()_NON_FATAL_ERROR_JCE_W0> to the PSM.
 *
 * Write one to each bit to clear it and allow reporting of future errors.
 *
 * Note that single-bit ECC errors will be automatically corrected, unless
 * MHBW()_MEM_COR_DIS0 is set to disable correction.
 */
union cvmx_mhbwx_mem_sbe0_int {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe0_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents SBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents SBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe0_int_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe0_int cvmx_mhbwx_mem_sbe0_int_t;

/**
 * cvmx_mhbw#_mem_sbe0_int_w1s
 */
union cvmx_mhbwx_mem_sbe0_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe0_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk2                         : 55; /**< This field represents SBEs in DMA related memories for HABs 0 and 1. */
	uint64_t blk1                         : 8;  /**< This field represents SBEs in job control related memories. */
#else
	uint64_t blk1                         : 8;
	uint64_t blk2                         : 55;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe0_int_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe0_int_w1s cvmx_mhbwx_mem_sbe0_int_w1s_t;

/**
 * cvmx_mhbw#_mem_sbe1_ena_w1c
 */
union cvmx_mhbwx_mem_sbe1_ena_w1c {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe1_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents SBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe1_ena_w1c_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe1_ena_w1c cvmx_mhbwx_mem_sbe1_ena_w1c_t;

/**
 * cvmx_mhbw#_mem_sbe1_ena_w1s
 */
union cvmx_mhbwx_mem_sbe1_ena_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe1_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents SBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe1_ena_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe1_ena_w1s cvmx_mhbwx_mem_sbe1_ena_w1s_t;

/**
 * cvmx_mhbw#_mem_sbe1_int
 *
 * These registers report single-bit ECC errors for internal MHBW memories.
 * On a single-bit ECC error, the MHBW will report a nonfatal error to the
 * PSM which then triggers an interrupt, if enable. In addition, the MHBW sends
 * the command specified by
 * <MHBW()_NON_FATAL_ERROR_JCE_W1,MHBW()_NON_FATAL_ERROR_JCE_W0> to the PSM.
 *
 * Write one to each bit to clear it and allow reporting of future errors.
 *
 * Note that single-bit ECC errors will be automatically corrected, unless
 * MHBW()_MEM_COR_DIS0 is set to disable correction.
 */
union cvmx_mhbwx_mem_sbe1_int {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe1_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents SBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe1_int_s      cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe1_int cvmx_mhbwx_mem_sbe1_int_t;

/**
 * cvmx_mhbw#_mem_sbe1_int_w1s
 */
union cvmx_mhbwx_mem_sbe1_int_w1s {
	uint64_t u64;
	struct cvmx_mhbwx_mem_sbe1_int_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t blk3                         : 63; /**< This field represents SBEs in DMA related memories for HABs 2 and 3. */
#else
	uint64_t blk3                         : 63;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_mhbwx_mem_sbe1_int_w1s_s  cnf75xx;
};
typedef union cvmx_mhbwx_mem_sbe1_int_w1s cvmx_mhbwx_mem_sbe1_int_w1s_t;

/**
 * cvmx_mhbw#_non_fatal_error_jce_w0
 *
 * This register specifies the low 64 bits of a PSM command issued when the
 * MHBW or one of its HABs suffer a nonfatal error.
 */
union cvmx_mhbwx_non_fatal_error_jce_w0 {
	uint64_t u64;
	struct cvmx_mhbwx_non_fatal_error_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< The first word of the PSM command to send on a nonfatal error. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_non_fatal_error_jce_w0_s cnf75xx;
};
typedef union cvmx_mhbwx_non_fatal_error_jce_w0 cvmx_mhbwx_non_fatal_error_jce_w0_t;

/**
 * cvmx_mhbw#_non_fatal_error_jce_w1
 *
 * This register specifies the high 64 bits of a PSM command issued when the
 * MHBW or one of its HABs suffer a nonfatal error.
 */
union cvmx_mhbwx_non_fatal_error_jce_w1 {
	uint64_t u64;
	struct cvmx_mhbwx_non_fatal_error_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< The second word of the PSM command to send on a nonfatal error. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_non_fatal_error_jce_w1_s cnf75xx;
};
typedef union cvmx_mhbwx_non_fatal_error_jce_w1 cvmx_mhbwx_non_fatal_error_jce_w1_t;

/**
 * cvmx_mhbw#_special_func
 *
 * These registers control special functions MHBW functions for the FDEQ MHABs.
 *
 */
union cvmx_mhbwx_special_func {
	uint64_t u64;
	struct cvmx_mhbwx_special_func_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ab_hr256_pack_dis            : 1;  /**< Disable 256 bit data packing on HAB read ports. When set to one, read
                                                         data is only transferred using bits [127:0] of the read DMA ports.
                                                         Note this bit is only used by FDEQ MHABs. */
#else
	uint64_t ab_hr256_pack_dis            : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_mhbwx_special_func_s      cnf75xx;
};
typedef union cvmx_mhbwx_special_func cvmx_mhbwx_special_func_t;

/**
 * cvmx_mhbw#_timeout_jce_w0
 *
 * This register specifies the low 64 bits of a PSM command issued when a
 * job timeout occurs.
 */
union cvmx_mhbwx_timeout_jce_w0 {
	uint64_t u64;
	struct cvmx_mhbwx_timeout_jce_w0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ls_word                      : 64; /**< The first word of the PSM command to send on a job timeout. */
#else
	uint64_t ls_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_timeout_jce_w0_s    cnf75xx;
};
typedef union cvmx_mhbwx_timeout_jce_w0 cvmx_mhbwx_timeout_jce_w0_t;

/**
 * cvmx_mhbw#_timeout_jce_w1
 *
 * This register specifies the high 64 bits of a PSM command issued when a
 * job timeout occurs.
 */
union cvmx_mhbwx_timeout_jce_w1 {
	uint64_t u64;
	struct cvmx_mhbwx_timeout_jce_w1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ms_word                      : 64; /**< The second word of the PSM command to send on a job timeout. */
#else
	uint64_t ms_word                      : 64;
#endif
	} s;
	struct cvmx_mhbwx_timeout_jce_w1_s    cnf75xx;
};
typedef union cvmx_mhbwx_timeout_jce_w1 cvmx_mhbwx_timeout_jce_w1_t;

#endif
