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
 * cvmx-rfif-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon rfif.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_RFIF_DEFS_H__
#define __CVMX_RFIF_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_AXC_DL_CONFIGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_RFIF_AXC_DL_CONFIGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000200ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_AXC_DL_CONFIGX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000200ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_AXC_DL_CONFIG_LOCK CVMX_RFIF_AXC_DL_CONFIG_LOCK_FUNC()
static inline uint64_t CVMX_RFIF_AXC_DL_CONFIG_LOCK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_AXC_DL_CONFIG_LOCK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D50001A0ull);
}
#else
#define CVMX_RFIF_AXC_DL_CONFIG_LOCK (CVMX_ADD_IO_SEG(0x00011800D50001A0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_BIST_STATUS CVMX_RFIF_BIST_STATUS_FUNC()
static inline uint64_t CVMX_RFIF_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004600ull);
}
#else
#define CVMX_RFIF_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800D5004600ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_BUF_SIZES CVMX_RFIF_CPRI_BUF_SIZES_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_BUF_SIZES_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_BUF_SIZES not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004030ull);
}
#else
#define CVMX_RFIF_CPRI_BUF_SIZES (CVMX_ADD_IO_SEG(0x00011800D5004030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_ABORT_STS0 CVMX_RFIF_CPRI_ETH_ABORT_STS0_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_ABORT_STS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_ABORT_STS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000A00ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_ABORT_STS0 (CVMX_ADD_IO_SEG(0x00011800D5000A00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_ABORT_STS1 CVMX_RFIF_CPRI_ETH_ABORT_STS1_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_ABORT_STS1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_ABORT_STS1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000A08ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_ABORT_STS1 (CVMX_ADD_IO_SEG(0x00011800D5000A08ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_CONFIG CVMX_RFIF_CPRI_ETH_CONFIG_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_CONFIG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_CONFIG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0000ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_CONFIG (CVMX_ADD_IO_SEG(0x0001D500003F0000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_DL_CFG0 CVMX_RFIF_CPRI_ETH_DL_CFG0_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_DL_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_DL_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000170ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_DL_CFG0 (CVMX_ADD_IO_SEG(0x00011800D5000170ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_DL_CFG1 CVMX_RFIF_CPRI_ETH_DL_CFG1_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_DL_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_DL_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000178ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_DL_CFG1 (CVMX_ADD_IO_SEG(0x00011800D5000178ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_DL_DB CVMX_RFIF_CPRI_ETH_DL_DB_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_DL_DB_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_DL_DB not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F00F0ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_DL_DB (CVMX_ADD_IO_SEG(0x0001D500003F00F0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_DL_STS0 CVMX_RFIF_CPRI_ETH_DL_STS0_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_DL_STS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_DL_STS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000180ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_DL_STS0 (CVMX_ADD_IO_SEG(0x00011800D5000180ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_DL_STS1 CVMX_RFIF_CPRI_ETH_DL_STS1_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_DL_STS1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_DL_STS1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000188ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_DL_STS1 (CVMX_ADD_IO_SEG(0x00011800D5000188ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_LN_CTRL CVMX_RFIF_CPRI_ETH_LN_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_LN_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_LN_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0010ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_LN_CTRL (CVMX_ADD_IO_SEG(0x0001D500003F0010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_LN_STS CVMX_RFIF_CPRI_ETH_LN_STS_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_LN_STS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_LN_STS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0020ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_LN_STS (CVMX_ADD_IO_SEG(0x0001D500003F0020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_SCRATCH CVMX_RFIF_CPRI_ETH_SCRATCH_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_SCRATCH_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_SCRATCH not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003FFFF0ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_SCRATCH (CVMX_ADD_IO_SEG(0x0001D500003FFFF0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_CFG0 CVMX_RFIF_CPRI_ETH_UL_CFG0_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000140ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_CFG0 (CVMX_ADD_IO_SEG(0x00011800D5000140ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_CFG1 CVMX_RFIF_CPRI_ETH_UL_CFG1_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000148ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_CFG1 (CVMX_ADD_IO_SEG(0x00011800D5000148ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_CFG2 CVMX_RFIF_CPRI_ETH_UL_CFG2_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_CFG2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_CFG2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000150ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_CFG2 (CVMX_ADD_IO_SEG(0x00011800D5000150ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_DB CVMX_RFIF_CPRI_ETH_UL_DB_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_DB_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_DB not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F00E0ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_DB (CVMX_ADD_IO_SEG(0x0001D500003F00E0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_STS0 CVMX_RFIF_CPRI_ETH_UL_STS0_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_STS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_STS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000158ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_STS0 (CVMX_ADD_IO_SEG(0x00011800D5000158ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_ETH_UL_STS1 CVMX_RFIF_CPRI_ETH_UL_STS1_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_ETH_UL_STS1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_ETH_UL_STS1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000160ull);
}
#else
#define CVMX_RFIF_CPRI_ETH_UL_STS1 (CVMX_ADD_IO_SEG(0x00011800D5000160ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F1000ull) + ((offset) & 7) * 256;
}
#else
#define CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG0(offset) (CVMX_ADD_IO_SEG(0x0001D500003F1000ull) + ((offset) & 7) * 256)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F1010ull) + ((offset) & 7) * 256;
}
#else
#define CVMX_RFIF_CPRI_LANEX_ETH_DL_CFG1(offset) (CVMX_ADD_IO_SEG(0x0001D500003F1010ull) + ((offset) & 7) * 256)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPRI_LANEX_ETH_DL_DB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_CPRI_LANEX_ETH_DL_DB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F10F0ull) + ((offset) & 7) * 256;
}
#else
#define CVMX_RFIF_CPRI_LANEX_ETH_DL_DB(offset) (CVMX_ADD_IO_SEG(0x0001D500003F10F0ull) + ((offset) & 7) * 256)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPRI_LANEX_ETH_DL_STS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_CPRI_LANEX_ETH_DL_STS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F1020ull) + ((offset) & 7) * 256;
}
#else
#define CVMX_RFIF_CPRI_LANEX_ETH_DL_STS0(offset) (CVMX_ADD_IO_SEG(0x0001D500003F1020ull) + ((offset) & 7) * 256)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPRI_LANEX_ETH_DL_STS1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_CPRI_LANEX_ETH_DL_STS1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F1030ull) + ((offset) & 7) * 256;
}
#else
#define CVMX_RFIF_CPRI_LANEX_ETH_DL_STS1(offset) (CVMX_ADD_IO_SEG(0x0001D500003F1030ull) + ((offset) & 7) * 256)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_CPRI_RESET_CTRL CVMX_RFIF_CPRI_RESET_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_CPRI_RESET_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_CPRI_RESET_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000018ull);
}
#else
#define CVMX_RFIF_CPRI_RESET_CTRL (CVMX_ADD_IO_SEG(0x00011800D5000018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_CPUIFX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 131071)))))
		cvmx_warn("CVMX_RFIF_CPUIFX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5100000ull) + ((offset) & 131071) * 8;
}
#else
#define CVMX_RFIF_CPUIFX(offset) (CVMX_ADD_IO_SEG(0x00011800D5100000ull) + ((offset) & 131071) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_DLFE_OUTPUT_LAT CVMX_RFIF_DLFE_OUTPUT_LAT_FUNC()
static inline uint64_t CVMX_RFIF_DLFE_OUTPUT_LAT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_DLFE_OUTPUT_LAT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004028ull);
}
#else
#define CVMX_RFIF_DLFE_OUTPUT_LAT (CVMX_ADD_IO_SEG(0x00011800D5004028ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_DLFE_PROC_LAT CVMX_RFIF_DLFE_PROC_LAT_FUNC()
static inline uint64_t CVMX_RFIF_DLFE_PROC_LAT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_DLFE_PROC_LAT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004020ull);
}
#else
#define CVMX_RFIF_DLFE_PROC_LAT (CVMX_ADD_IO_SEG(0x00011800D5004020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_DL_CKSUM_CFGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_DL_CKSUM_CFGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5004100ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIF_DL_CKSUM_CFGX(offset) (CVMX_ADD_IO_SEG(0x00011800D5004100ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_DL_CKSUM_RESX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_RFIF_DL_CKSUM_RESX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5004200ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_DL_CKSUM_RESX(offset) (CVMX_ADD_IO_SEG(0x00011800D5004200ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_DL_CKSUM_START CVMX_RFIF_DL_CKSUM_START_FUNC()
static inline uint64_t CVMX_RFIF_DL_CKSUM_START_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_DL_CKSUM_START not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004130ull);
}
#else
#define CVMX_RFIF_DL_CKSUM_START (CVMX_ADD_IO_SEG(0x00011800D5004130ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_DL_RADIO_AXC_ENX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_DL_RADIO_AXC_ENX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000380ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIF_DL_RADIO_AXC_ENX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000380ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_ECO CVMX_RFIF_ECO_FUNC()
static inline uint64_t CVMX_RFIF_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5008000ull);
}
#else
#define CVMX_RFIF_ECO (CVMX_ADD_IO_SEG(0x00011800D5008000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_ETH_ABORT CVMX_RFIF_ETH_ABORT_FUNC()
static inline uint64_t CVMX_RFIF_ETH_ABORT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_ETH_ABORT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0300ull);
}
#else
#define CVMX_RFIF_ETH_ABORT (CVMX_ADD_IO_SEG(0x0001D500003F0300ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_ETH_ECC_CTRL CVMX_RFIF_ETH_ECC_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_ETH_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_ETH_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0100ull);
}
#else
#define CVMX_RFIF_ETH_ECC_CTRL (CVMX_ADD_IO_SEG(0x0001D500003F0100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_ETH_ECC_ERR CVMX_RFIF_ETH_ECC_ERR_FUNC()
static inline uint64_t CVMX_RFIF_ETH_ECC_ERR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_ETH_ECC_ERR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0110ull);
}
#else
#define CVMX_RFIF_ETH_ECC_ERR (CVMX_ADD_IO_SEG(0x0001D500003F0110ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_ETH_RXX_ECC_INFO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_ETH_RXX_ECC_INFO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001D500003F0200ull) + ((offset) & 7) * 16;
}
#else
#define CVMX_RFIF_ETH_RXX_ECC_INFO(offset) (CVMX_ADD_IO_SEG(0x0001D500003F0200ull) + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_ETH_TX_ECC_INFO CVMX_RFIF_ETH_TX_ECC_INFO_FUNC()
static inline uint64_t CVMX_RFIF_ETH_TX_ECC_INFO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_ETH_TX_ECC_INFO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x0001D500003F0260ull);
}
#else
#define CVMX_RFIF_ETH_TX_ECC_INFO (CVMX_ADD_IO_SEG(0x0001D500003F0260ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_INT_SUM CVMX_RFIF_INT_SUM_FUNC()
static inline uint64_t CVMX_RFIF_INT_SUM_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_INT_SUM not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5008008ull);
}
#else
#define CVMX_RFIF_INT_SUM (CVMX_ADD_IO_SEG(0x00011800D5008008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_LANES_ENABLES CVMX_RFIF_LANES_ENABLES_FUNC()
static inline uint64_t CVMX_RFIF_LANES_ENABLES_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_LANES_ENABLES not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000198ull);
}
#else
#define CVMX_RFIF_LANES_ENABLES (CVMX_ADD_IO_SEG(0x00011800D5000198ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_LOF_CNT CVMX_RFIF_LOF_CNT_FUNC()
static inline uint64_t CVMX_RFIF_LOF_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_LOF_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5002A08ull);
}
#else
#define CVMX_RFIF_LOF_CNT (CVMX_ADD_IO_SEG(0x00011800D5002A08ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_LOOP_CFG CVMX_RFIF_LOOP_CFG_FUNC()
static inline uint64_t CVMX_RFIF_LOOP_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_LOOP_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004010ull);
}
#else
#define CVMX_RFIF_LOOP_CFG (CVMX_ADD_IO_SEG(0x00011800D5004010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_LOOP_RFP CVMX_RFIF_LOOP_RFP_FUNC()
static inline uint64_t CVMX_RFIF_LOOP_RFP_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_LOOP_RFP not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004018ull);
}
#else
#define CVMX_RFIF_LOOP_RFP (CVMX_ADD_IO_SEG(0x00011800D5004018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_LOS_CNT CVMX_RFIF_LOS_CNT_FUNC()
static inline uint64_t CVMX_RFIF_LOS_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_LOS_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5002A00ull);
}
#else
#define CVMX_RFIF_LOS_CNT (CVMX_ADD_IO_SEG(0x00011800D5002A00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_MASTER_CFG CVMX_RFIF_MASTER_CFG_FUNC()
static inline uint64_t CVMX_RFIF_MASTER_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_MASTER_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000000ull);
}
#else
#define CVMX_RFIF_MASTER_CFG (CVMX_ADD_IO_SEG(0x00011800D5000000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_OLOS_STAT CVMX_RFIF_OLOS_STAT_FUNC()
static inline uint64_t CVMX_RFIF_OLOS_STAT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_OLOS_STAT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004008ull);
}
#else
#define CVMX_RFIF_OLOS_STAT (CVMX_ADD_IO_SEG(0x00011800D5004008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_PRACH_SOS_CFG CVMX_RFIF_PRACH_SOS_CFG_FUNC()
static inline uint64_t CVMX_RFIF_PRACH_SOS_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_PRACH_SOS_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004408ull);
}
#else
#define CVMX_RFIF_PRACH_SOS_CFG (CVMX_ADD_IO_SEG(0x00011800D5004408ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_PRS_SWC CVMX_RFIF_PRS_SWC_FUNC()
static inline uint64_t CVMX_RFIF_PRS_SWC_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_PRS_SWC not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5004400ull);
}
#else
#define CVMX_RFIF_PRS_SWC (CVMX_ADD_IO_SEG(0x00011800D5004400ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_RESET_CTRL CVMX_RFIF_RESET_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_RESET_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_RESET_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000010ull);
}
#else
#define CVMX_RFIF_RESET_CTRL (CVMX_ADD_IO_SEG(0x00011800D5000010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_RETARD CVMX_RFIF_RETARD_FUNC()
static inline uint64_t CVMX_RFIF_RETARD_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_RETARD not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000040ull);
}
#else
#define CVMX_RFIF_RETARD (CVMX_ADD_IO_SEG(0x00011800D5000040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_RMACX_CLKEN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_RFIF_RMACX_CLKEN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5004500ull) + ((offset) & 3) * 8;
}
#else
#define CVMX_RFIF_RMACX_CLKEN(offset) (CVMX_ADD_IO_SEG(0x00011800D5004500ull) + ((offset) & 3) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_SAMPLE_WIDTH CVMX_RFIF_SAMPLE_WIDTH_FUNC()
static inline uint64_t CVMX_RFIF_SAMPLE_WIDTH_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_SAMPLE_WIDTH not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000050ull);
}
#else
#define CVMX_RFIF_SAMPLE_WIDTH (CVMX_ADD_IO_SEG(0x00011800D5000050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_SDL_RESET_CTRL CVMX_RFIF_SDL_RESET_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_SDL_RESET_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_SDL_RESET_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000028ull);
}
#else
#define CVMX_RFIF_SDL_RESET_CTRL (CVMX_ADD_IO_SEG(0x00011800D5000028ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_BFN_NUM CVMX_RFIF_TIMER_BFN_NUM_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_BFN_NUM_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_BFN_NUM not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F20ull);
}
#else
#define CVMX_RFIF_TIMER_BFN_NUM (CVMX_ADD_IO_SEG(0x00011800D5000F20ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_BFN_NUM_BAD_CNT CVMX_RFIF_TIMER_BFN_NUM_BAD_CNT_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_BFN_NUM_BAD_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_BFN_NUM_BAD_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F30ull);
}
#else
#define CVMX_RFIF_TIMER_BFN_NUM_BAD_CNT (CVMX_ADD_IO_SEG(0x00011800D5000F30ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_BFN_NUM_OK CVMX_RFIF_TIMER_BFN_NUM_OK_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_BFN_NUM_OK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_BFN_NUM_OK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F28ull);
}
#else
#define CVMX_RFIF_TIMER_BFN_NUM_OK (CVMX_ADD_IO_SEG(0x00011800D5000F28ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_BFN_SYNC_FAIL_CNT CVMX_RFIF_TIMER_BFN_SYNC_FAIL_CNT_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_BFN_SYNC_FAIL_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_BFN_SYNC_FAIL_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F18ull);
}
#else
#define CVMX_RFIF_TIMER_BFN_SYNC_FAIL_CNT (CVMX_ADD_IO_SEG(0x00011800D5000F18ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_BFN_SYNC_OK CVMX_RFIF_TIMER_BFN_SYNC_OK_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_BFN_SYNC_OK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_BFN_SYNC_OK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F10ull);
}
#else
#define CVMX_RFIF_TIMER_BFN_SYNC_OK (CVMX_ADD_IO_SEG(0x00011800D5000F10ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_CFG CVMX_RFIF_TIMER_CFG_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000E00ull);
}
#else
#define CVMX_RFIF_TIMER_CFG (CVMX_ADD_IO_SEG(0x00011800D5000E00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_SFC CVMX_RFIF_TIMER_SFC_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_SFC_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_SFC not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F38ull);
}
#else
#define CVMX_RFIF_TIMER_SFC (CVMX_ADD_IO_SEG(0x00011800D5000F38ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_SYNC_CNT CVMX_RFIF_TIMER_SYNC_CNT_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_SYNC_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_SYNC_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F08ull);
}
#else
#define CVMX_RFIF_TIMER_SYNC_CNT (CVMX_ADD_IO_SEG(0x00011800D5000F08ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_SYNC_FAIL_CNT CVMX_RFIF_TIMER_SYNC_FAIL_CNT_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_SYNC_FAIL_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_SYNC_FAIL_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000F00ull);
}
#else
#define CVMX_RFIF_TIMER_SYNC_FAIL_CNT (CVMX_ADD_IO_SEG(0x00011800D5000F00ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIMER_SYNC_OK CVMX_RFIF_TIMER_SYNC_OK_FUNC()
static inline uint64_t CVMX_RFIF_TIMER_SYNC_OK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIMER_SYNC_OK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000E08ull);
}
#else
#define CVMX_RFIF_TIMER_SYNC_OK (CVMX_ADD_IO_SEG(0x00011800D5000E08ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TIM_ADV CVMX_RFIF_TIM_ADV_FUNC()
static inline uint64_t CVMX_RFIF_TIM_ADV_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TIM_ADV not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000038ull);
}
#else
#define CVMX_RFIF_TIM_ADV (CVMX_ADD_IO_SEG(0x00011800D5000038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TOFB_RESET_CTRL CVMX_RFIF_TOFB_RESET_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_TOFB_RESET_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TOFB_RESET_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000020ull);
}
#else
#define CVMX_RFIF_TOFB_RESET_CTRL (CVMX_ADD_IO_SEG(0x00011800D5000020ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TOSP_OUTPUT_ENA CVMX_RFIF_TOSP_OUTPUT_ENA_FUNC()
static inline uint64_t CVMX_RFIF_TOSP_OUTPUT_ENA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TOSP_OUTPUT_ENA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5008010ull);
}
#else
#define CVMX_RFIF_TOSP_OUTPUT_ENA (CVMX_ADD_IO_SEG(0x00011800D5008010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_TOSP_RESET_CTRL CVMX_RFIF_TOSP_RESET_CTRL_FUNC()
static inline uint64_t CVMX_RFIF_TOSP_RESET_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_TOSP_RESET_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000030ull);
}
#else
#define CVMX_RFIF_TOSP_RESET_CTRL (CVMX_ADD_IO_SEG(0x00011800D5000030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_COMB_CONFIGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_RFIF_UL_COMB_CONFIGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000400ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_UL_COMB_CONFIGX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000400ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_UL_COMB_CONFIG_LOCK CVMX_RFIF_UL_COMB_CONFIG_LOCK_FUNC()
static inline uint64_t CVMX_RFIF_UL_COMB_CONFIG_LOCK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_UL_COMB_CONFIG_LOCK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D50003C0ull);
}
#else
#define CVMX_RFIF_UL_COMB_CONFIG_LOCK (CVMX_ADD_IO_SEG(0x00011800D50003C0ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_COMB_P0_CONFIGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_RFIF_UL_COMB_P0_CONFIGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000600ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_UL_COMB_P0_CONFIGX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000600ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_COMB_P1_CONFIGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_RFIF_UL_COMB_P1_CONFIGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000800ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_UL_COMB_P1_CONFIGX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000800ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_UL_COPY CVMX_RFIF_UL_COPY_FUNC()
static inline uint64_t CVMX_RFIF_UL_COPY_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_UL_COPY not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5002970ull);
}
#else
#define CVMX_RFIF_UL_COPY (CVMX_ADD_IO_SEG(0x00011800D5002970ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_PN_INITX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 63)))))
		cvmx_warn("CVMX_RFIF_UL_PN_INITX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5001400ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_RFIF_UL_PN_INITX(offset) (CVMX_ADD_IO_SEG(0x00011800D5001400ull) + ((offset) & 63) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_PN_MAPX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_RFIF_UL_PN_MAPX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5001200ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIF_UL_PN_MAPX(offset) (CVMX_ADD_IO_SEG(0x00011800D5001200ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_UL_PN_START CVMX_RFIF_UL_PN_START_FUNC()
static inline uint64_t CVMX_RFIF_UL_PN_START_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_UL_PN_START not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5001600ull);
}
#else
#define CVMX_RFIF_UL_PN_START (CVMX_ADD_IO_SEG(0x00011800D5001600ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIF_UL_RADIO_AXC_ENX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIF_UL_RADIO_AXC_ENX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000980ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIF_UL_RADIO_AXC_ENX(offset) (CVMX_ADD_IO_SEG(0x00011800D5000980ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_UL_SMPLE_ADJ CVMX_RFIF_UL_SMPLE_ADJ_FUNC()
static inline uint64_t CVMX_RFIF_UL_SMPLE_ADJ_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_UL_SMPLE_ADJ not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D5000048ull);
}
#else
#define CVMX_RFIF_UL_SMPLE_ADJ (CVMX_ADD_IO_SEG(0x00011800D5000048ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RFIF_UL_SYNC_CONFIG CVMX_RFIF_UL_SYNC_CONFIG_FUNC()
static inline uint64_t CVMX_RFIF_UL_SYNC_CONFIG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RFIF_UL_SYNC_CONFIG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800D50003C8ull);
}
#else
#define CVMX_RFIF_UL_SYNC_CONFIG (CVMX_ADD_IO_SEG(0x00011800D50003C8ull))
#endif

/**
 * cvmx_rfif_axc_dl_config#
 */
union cvmx_rfif_axc_dl_configx {
	uint64_t u64;
	struct cvmx_rfif_axc_dl_configx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t dl_copy                      : 6;  /**< Bitmask indicating which destination RFIF lane(s) the DLFE antenna
                                                         will be transmitted on. Set bit N to transmit the antenna on one AxC
                                                         on lane N. */
	uint64_t reserved_53_55               : 3;
	uint64_t lane5_axc_id                 : 5;  /**< Lane 5 destination AxC ID. */
	uint64_t reserved_45_47               : 3;
	uint64_t lane4_axc_id                 : 5;  /**< Lane 4 destination AxC ID. */
	uint64_t reserved_37_39               : 3;
	uint64_t lane3_axc_id                 : 5;  /**< Lane 3 destination AxC ID. */
	uint64_t reserved_29_31               : 3;
	uint64_t lane2_axc_id                 : 5;  /**< Lane 2 destination AxC ID. */
	uint64_t reserved_21_23               : 3;
	uint64_t lane1_axc_id                 : 5;  /**< Lane 1 destination AxC ID. */
	uint64_t reserved_13_15               : 3;
	uint64_t lane0_axc_id                 : 5;  /**< Lane 0 destination AxC ID. */
	uint64_t reserved_3_7                 : 5;
	uint64_t bw                           : 3;  /**< Antenna bandwidth:
                                                         0x0 = 1.4 MHz.
                                                         0x1 = 3 MHz.
                                                         0x2 = 5 MHz.
                                                         0x3 = 10 MHz.
                                                         0x4 = 15 MHz.
                                                         0x5 = 20 MHz. */
#else
	uint64_t bw                           : 3;
	uint64_t reserved_3_7                 : 5;
	uint64_t lane0_axc_id                 : 5;
	uint64_t reserved_13_15               : 3;
	uint64_t lane1_axc_id                 : 5;
	uint64_t reserved_21_23               : 3;
	uint64_t lane2_axc_id                 : 5;
	uint64_t reserved_29_31               : 3;
	uint64_t lane3_axc_id                 : 5;
	uint64_t reserved_37_39               : 3;
	uint64_t lane4_axc_id                 : 5;
	uint64_t reserved_45_47               : 3;
	uint64_t lane5_axc_id                 : 5;
	uint64_t reserved_53_55               : 3;
	uint64_t dl_copy                      : 6;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rfif_axc_dl_configx_s     cnf75xx;
};
typedef union cvmx_rfif_axc_dl_configx cvmx_rfif_axc_dl_configx_t;

/**
 * cvmx_rfif_axc_dl_config_lock
 */
union cvmx_rfif_axc_dl_config_lock {
	uint64_t u64;
	struct cvmx_rfif_axc_dl_config_lock_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t lock                         : 1;  /**< Lock bit. Set prior to configuring the AxC DL configuration, then cleared once the
                                                         configuration is completed. */
#else
	uint64_t lock                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_axc_dl_config_lock_s cnf75xx;
};
typedef union cvmx_rfif_axc_dl_config_lock cvmx_rfif_axc_dl_config_lock_t;

/**
 * cvmx_rfif_bist_status
 *
 * This register provides access to internal BIST results. Each bit is the BIST
 * result of an individual memory (per bit, 0 = pass, 1 = fail).
 */
union cvmx_rfif_bist_status {
	uint64_t u64;
	struct cvmx_rfif_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t bist_done                    : 13; /**< BIST done. */
	uint64_t reserved_13_15               : 3;
	uint64_t bist_status                  : 13; /**< BIST status. */
#else
	uint64_t bist_status                  : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t bist_done                    : 13;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_rfif_bist_status_s        cnf75xx;
};
typedef union cvmx_rfif_bist_status cvmx_rfif_bist_status_t;

/**
 * cvmx_rfif_cpri_buf_sizes
 */
union cvmx_rfif_cpri_buf_sizes {
	uint64_t u64;
	struct cvmx_rfif_cpri_buf_sizes_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_25_63               : 39;
	uint64_t dl_buf_size                  : 9;  /**< Receive buffer size per AxC. The legal values are 12 and 16. It is recommanded to use 16. */
	uint64_t reserved_9_15                : 7;
	uint64_t ul_buf_size                  : 9;  /**< Receive buffer size per AxC. The legal values are 12 and 16. It is recommanded to use 16. */
#else
	uint64_t ul_buf_size                  : 9;
	uint64_t reserved_9_15                : 7;
	uint64_t dl_buf_size                  : 9;
	uint64_t reserved_25_63               : 39;
#endif
	} s;
	struct cvmx_rfif_cpri_buf_sizes_s     cnf75xx;
};
typedef union cvmx_rfif_cpri_buf_sizes cvmx_rfif_cpri_buf_sizes_t;

/**
 * cvmx_rfif_cpri_eth_abort_sts0
 */
union cvmx_rfif_cpri_eth_abort_sts0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_abort_sts0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t dl_abort                     : 16; /**< Number of DL packets aborted */
	uint64_t ul_abort                     : 16; /**< Number of UL packets aborted */
	uint64_t ul_abort_crc                 : 16; /**< Number of UL packets aborted, because of CRC error. */
	uint64_t ul_abort_ovfl                : 16; /**< Number of UL packets aborted, because of buffer overflow condition. */
#else
	uint64_t ul_abort_ovfl                : 16;
	uint64_t ul_abort_crc                 : 16;
	uint64_t ul_abort                     : 16;
	uint64_t dl_abort                     : 16;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_abort_sts0_s cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_abort_sts0 cvmx_rfif_cpri_eth_abort_sts0_t;

/**
 * cvmx_rfif_cpri_eth_abort_sts1
 */
union cvmx_rfif_cpri_eth_abort_sts1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_abort_sts1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ul_abort_udfl                : 16; /**< Number of UL packets aborted, because of a buffer underflow condition. */
	uint64_t ul_abort_mii                 : 16; /**< Number of UL packets aborted, because of MII error detection. */
	uint64_t ul_abort_short               : 16; /**< Number of UL packets aborted, because the frame was too short. */
	uint64_t ul_abort_dmac                : 16; /**< Number of UL packets aborted, because of DMAC mismatch. */
#else
	uint64_t ul_abort_dmac                : 16;
	uint64_t ul_abort_short               : 16;
	uint64_t ul_abort_mii                 : 16;
	uint64_t ul_abort_udfl                : 16;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_abort_sts1_s cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_abort_sts1 cvmx_rfif_cpri_eth_abort_sts1_t;

/**
 * cvmx_rfif_cpri_eth_config
 */
union cvmx_rfif_cpri_eth_config {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t stt                          : 1;  /**< Selects the store operation type used for writing TX packet data to
                                                         memory.
                                                         0 = Allocate in L2C.
                                                         1 = Do not allocate in L2C (write directly to DDR if not already in
                                                         L2C). */
	uint64_t ldwb                         : 1;  /**< Selects the load operation type used for reading RX packet data.
                                                         0 = Normal load.
                                                         1 = LDWB (load and don't write back).
                                                         Using LDWB can reduce the demand for DDR bandwidth, but the RX packet
                                                         data may be lost after being read by RFIF. */
	uint64_t mac_res_en                   : 1;  /**< Enables writing the MAC status into the WQE for RX packet data. */
	uint64_t rx_arb_mode                  : 1;  /**< Selects the mode of arbitration between RX lanes ready to write packet data to memory.
                                                         0 = Fixed priority (lane 0 to lane 5).
                                                         1 = Round-robin. */
	uint64_t tx_mem_arb_mode              : 1;  /**< Selects the mode of arbitration for access from the TX lanes to the shared TX data RAM.
                                                         0 = Fixed priority (lane 0 to lane 5).
                                                         1 = Round-robin. */
	uint64_t tx_ncb_arb_mode              : 1;  /**< Selects the mode of arbitration for NCB access from the TX lanes.
                                                         0 = Fixed priority (lane 0 to lane 5).
                                                         1 = Round-robin. */
	uint64_t ncbi_arb_mode                : 1;  /**< Selects the mode for NCBI arbitration between read responses, eth_tx, and eth_rx.
                                                         0 = Fixed priority (0=rdrsp,1=eth_tx,2=eth_rx).
                                                         1 = Round-robin. */
	uint64_t dl_lane_mode                 : 1;  /**< Reserved. Must be 0. */
#else
	uint64_t dl_lane_mode                 : 1;
	uint64_t ncbi_arb_mode                : 1;
	uint64_t tx_ncb_arb_mode              : 1;
	uint64_t tx_mem_arb_mode              : 1;
	uint64_t rx_arb_mode                  : 1;
	uint64_t mac_res_en                   : 1;
	uint64_t ldwb                         : 1;
	uint64_t stt                          : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_config_s    cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_config cvmx_rfif_cpri_eth_config_t;

/**
 * cvmx_rfif_cpri_eth_dl_cfg0
 *
 * This register configures the location of the common downlink circular
 * buffer when RFIF_CPRI_ETH_CONFIG[DL_LANE_MODE] = 1.
 */
union cvmx_rfif_cpri_eth_dl_cfg0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_dl_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t cbuf_ptr                     : 33; /**< DL circular buffer pointer, cache line aligned (128B). */
	uint64_t reserved_0_6                 : 7;
#else
	uint64_t reserved_0_6                 : 7;
	uint64_t cbuf_ptr                     : 33;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_dl_cfg0_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_dl_cfg0 cvmx_rfif_cpri_eth_dl_cfg0_t;

/**
 * cvmx_rfif_cpri_eth_dl_cfg1
 *
 * This register configures the size of the common downlink circular
 * buffer when RFIF_CPRI_ETH_CONFIG[DL_LANE_MODE] = 1.
 */
union cvmx_rfif_cpri_eth_dl_cfg1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_dl_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cbuf_size                    : 32; /**< Circular buffer size in number of packets. The actual size of the
                                                         buffer in bytes must be CBUF_SIZE*1664 bytes since each packet
                                                         occupies 1664 bytes. */
#else
	uint64_t cbuf_size                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_dl_cfg1_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_dl_cfg1 cvmx_rfif_cpri_eth_dl_cfg1_t;

/**
 * cvmx_rfif_cpri_eth_dl_db
 *
 * This register serves as the CPRI Ethernet downlink common doorbell
 * register when RFIF_CPRI_ETH_CONFIG[DL_LANE_MODE] = 1.
 */
union cvmx_rfif_cpri_eth_dl_db {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_dl_db_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t doorbell                     : 32; /**< Written to indicate the number of packets that have been added
                                                         to the ETH_DL circular buffer by software.  The buffer level and
                                                         write pointer SW_WR_PTR will be updated by this amount.  A read
                                                         from this register will return the current buffer level.  If a
                                                         write occurs with a value larger than the remaining space, a
                                                         doorbell error (RFIF_ETH_DB_ERR) will be flagged, and the
                                                         Ethernet behavior will be unpredictable. */
#else
	uint64_t doorbell                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_dl_db_s     cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_dl_db cvmx_rfif_cpri_eth_dl_db_t;

/**
 * cvmx_rfif_cpri_eth_dl_sts0
 *
 * This register reports the common software packet write pointer
 * when RFIF_CPRI_ETH_CONFIG[DL_LANE_MODE] = 1.
 */
union cvmx_rfif_cpri_eth_dl_sts0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_dl_sts0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t sw_wr_ptr                    : 40; /**< Location where the next packet should be written by software.
                                                         This pointer is updated when software writes the doorbell register
                                                         RFIF_CPRI_ETH_DL_DB, indicating that packets have been added to
                                                         the circular buffer to be transmitted. */
#else
	uint64_t sw_wr_ptr                    : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_dl_sts0_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_dl_sts0 cvmx_rfif_cpri_eth_dl_sts0_t;

/**
 * cvmx_rfif_cpri_eth_dl_sts1
 *
 * This register reports the common next packet read pointer
 * when RFIF_CPRI_ETH_CONFIG[DL_LANE_MODE] = 1.
 */
union cvmx_rfif_cpri_eth_dl_sts1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_dl_sts1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t next_rd_ptr                  : 40; /**< Location where the next packet to be transmitted will be read by
                                                         the RFIF Ethernet hardware.  This pointer is updated as packets
                                                         are sent. */
#else
	uint64_t next_rd_ptr                  : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_dl_sts1_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_dl_sts1 cvmx_rfif_cpri_eth_dl_sts1_t;

/**
 * cvmx_rfif_cpri_eth_ln_ctrl
 *
 * CPRI Ethernet Lane Control register.
 *
 */
union cvmx_rfif_cpri_eth_ln_ctrl {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ln_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_30_63               : 34;
	uint64_t dl_lane_rst                  : 6;  /**< Active low resets for the lane-specific logic in the RFIF DL
                                                         Ethernet block. */
	uint64_t reserved_22_23               : 2;
	uint64_t ul_lane_rst                  : 6;  /**< Active low resets for the lane-specific logic in the RFIF UL
                                                         Ethernet block. */
	uint64_t reserved_15_15               : 1;
	uint64_t dl_lane_dis_mode             : 1;  /**< Controls halting of the DL Ethernet lane when a TX abort is
                                                         signaled.
                                                         0 = Allow the lane to continue after a TX abort.
                                                         1 = Disable the lane on a TX abort. */
	uint64_t dl_lane_dis                  : 6;  /**< Active high disable signals for the lane-specific logic in
                                                         the RFIF DL Ethernet block. */
	uint64_t reserved_6_7                 : 2;
	uint64_t ul_lane_dis                  : 6;  /**< Active high disable signals for the lane-specific logic in
                                                         the RFIF UL Ethernet block. */
#else
	uint64_t ul_lane_dis                  : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t dl_lane_dis                  : 6;
	uint64_t dl_lane_dis_mode             : 1;
	uint64_t reserved_15_15               : 1;
	uint64_t ul_lane_rst                  : 6;
	uint64_t reserved_22_23               : 2;
	uint64_t dl_lane_rst                  : 6;
	uint64_t reserved_30_63               : 34;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ln_ctrl_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ln_ctrl cvmx_rfif_cpri_eth_ln_ctrl_t;

/**
 * cvmx_rfif_cpri_eth_ln_sts
 *
 * CPRI Ethernet Lane Status register.
 *
 */
union cvmx_rfif_cpri_eth_ln_sts {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ln_sts_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t dl_lane_inactive             : 6;  /**< Active high signals indicating that the given DL lane is
                                                         in a safe state to be reset.  Resetting the lane or the
                                                         entire RFIF Ethernet block while these signals are not
                                                         asserted is unsafe. */
	uint64_t reserved_6_7                 : 2;
	uint64_t ul_lane_inactive             : 6;  /**< Active high signals indicating that the given UL lane is
                                                         in a safe state to be reset.  Resetting the lane or the
                                                         entire RFIF Ethernet block while these signals are not
                                                         asserted is unsafe. */
#else
	uint64_t ul_lane_inactive             : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t dl_lane_inactive             : 6;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ln_sts_s    cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ln_sts cvmx_rfif_cpri_eth_ln_sts_t;

/**
 * cvmx_rfif_cpri_eth_scratch
 */
union cvmx_rfif_cpri_eth_scratch {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t data                         : 40; /**< Scratch data. */
#else
	uint64_t data                         : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_scratch_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_scratch cvmx_rfif_cpri_eth_scratch_t;

/**
 * cvmx_rfif_cpri_eth_ul_cfg0
 */
union cvmx_rfif_cpri_eth_ul_cfg0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t eth_port_dest                : 6;  /**< RFIF Ethernet port destination. Each WQE contains a 1 bit PORT field.
                                                         Packets from CPRI lane i set the RFIF_ETH_RX_WQE_S[PORT] = ETH_PORT_DEST<i>. */
	uint64_t int_rep_per                  : 8;  /**< Interrupt coalescing threshold. RFIF triggers RFIF_ETH_PKT_ARR_NOTIF
                                                         after every INT_REP_PER packets arrive. Set to 0 to disable the
                                                         interrupt. */
#else
	uint64_t int_rep_per                  : 8;
	uint64_t eth_port_dest                : 6;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_cfg0_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_cfg0 cvmx_rfif_cpri_eth_ul_cfg0_t;

/**
 * cvmx_rfif_cpri_eth_ul_cfg1
 *
 * This register configures the uplink circular buffer pointer.
 *
 */
union cvmx_rfif_cpri_eth_ul_cfg1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t cbuf_ptr                     : 33; /**< UL circular buffer pointer, cache line aligned (128B). */
	uint64_t reserved_0_6                 : 7;
#else
	uint64_t reserved_0_6                 : 7;
	uint64_t cbuf_ptr                     : 33;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_cfg1_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_cfg1 cvmx_rfif_cpri_eth_ul_cfg1_t;

/**
 * cvmx_rfif_cpri_eth_ul_cfg2
 *
 * This register configures the size of the uplink circular buffer.
 *
 */
union cvmx_rfif_cpri_eth_ul_cfg2 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cbuf_size                    : 32; /**< Circular buffer size in number of packets. The actual size of the
                                                         buffer in bytes must be CBUF_SIZE*1664 bytes since each packet
                                                         occupies 1664 bytes. */
#else
	uint64_t cbuf_size                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_cfg2_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_cfg2 cvmx_rfif_cpri_eth_ul_cfg2_t;

/**
 * cvmx_rfif_cpri_eth_ul_db
 */
union cvmx_rfif_cpri_eth_ul_db {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_db_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t doorbell                     : 32; /**< Written to indicate the number of packets from the ETH_UL
                                                         circular buffer by software.  The buffer level and read pointer
                                                         SW_RD_PTR will be updated by this amount, freeing up the space
                                                         to be reused.  A read from this register will return the current
                                                         buffer level.  If a write occurs with a value larger than the
                                                         current level, a doorbell error (RFIF_ETH_DB_ERR) will be flagged,
                                                         and the Ethernet behavior will be unpredictable. */
#else
	uint64_t doorbell                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_db_s     cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_db cvmx_rfif_cpri_eth_ul_db_t;

/**
 * cvmx_rfif_cpri_eth_ul_sts0
 *
 * Next write packet pointer.
 *
 */
union cvmx_rfif_cpri_eth_ul_sts0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_sts0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t nxt_wr_ptr                   : 40; /**< Location where the next received packet will be written by
                                                         RFIF.  This pointer is updated as each new packet arrives. */
#else
	uint64_t nxt_wr_ptr                   : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_sts0_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_sts0 cvmx_rfif_cpri_eth_ul_sts0_t;

/**
 * cvmx_rfif_cpri_eth_ul_sts1
 *
 * Software packet read pointer.
 *
 */
union cvmx_rfif_cpri_eth_ul_sts1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_eth_ul_sts1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t sw_rd_ptr                    : 40; /**< Location of the next packet to be processed by software.  This
                                                         pointer is updated when software writes the doorbell register
                                                         RFIF_CPRI_ETH_UL_DB, indicating that packets have processed. */
#else
	uint64_t sw_rd_ptr                    : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_eth_ul_sts1_s   cnf75xx;
};
typedef union cvmx_rfif_cpri_eth_ul_sts1 cvmx_rfif_cpri_eth_ul_sts1_t;

/**
 * cvmx_rfif_cpri_lane#_eth_dl_cfg0
 *
 * These registers configure the location of the per-lane downlink circular buffers.
 *
 */
union cvmx_rfif_cpri_lanex_eth_dl_cfg0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_lanex_eth_dl_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t cbuf_ptr                     : 33; /**< DL circular buffer pointer, cache line aligned (128B). */
	uint64_t reserved_0_6                 : 7;
#else
	uint64_t reserved_0_6                 : 7;
	uint64_t cbuf_ptr                     : 33;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_lanex_eth_dl_cfg0_s cnf75xx;
};
typedef union cvmx_rfif_cpri_lanex_eth_dl_cfg0 cvmx_rfif_cpri_lanex_eth_dl_cfg0_t;

/**
 * cvmx_rfif_cpri_lane#_eth_dl_cfg1
 *
 * These registers configure the size of the per-lane downlink circular buffers.
 *
 */
union cvmx_rfif_cpri_lanex_eth_dl_cfg1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_lanex_eth_dl_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cbuf_size                    : 32; /**< Circular buffer size in number of packets. The actual size of the
                                                         buffer in bytes must be CBUF_SIZE*1664 bytes since each packet
                                                         occupies 1664 bytes. */
#else
	uint64_t cbuf_size                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_lanex_eth_dl_cfg1_s cnf75xx;
};
typedef union cvmx_rfif_cpri_lanex_eth_dl_cfg1 cvmx_rfif_cpri_lanex_eth_dl_cfg1_t;

/**
 * cvmx_rfif_cpri_lane#_eth_dl_db
 *
 * These registers provide the Ethernet downlink doorbells for each CPRI
 * lane.
 */
union cvmx_rfif_cpri_lanex_eth_dl_db {
	uint64_t u64;
	struct cvmx_rfif_cpri_lanex_eth_dl_db_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t doorbell                     : 32; /**< Written to indicate the number of packets that have been added
                                                         to the ETH_DL circular buffer by software.  The buffer level and
                                                         write pointer SW_WR_PTR will be updated by this amount.  A read
                                                         from this register will return the current buffer level.  If a
                                                         write occurs with a value larger than the remaining space, a
                                                         doorbell error (RFIF_ETH_DB_ERR) will be flagged, and the
                                                         Ethernet behavior will be unpredictable. */
#else
	uint64_t doorbell                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpri_lanex_eth_dl_db_s cnf75xx;
};
typedef union cvmx_rfif_cpri_lanex_eth_dl_db cvmx_rfif_cpri_lanex_eth_dl_db_t;

/**
 * cvmx_rfif_cpri_lane#_eth_dl_sts0
 *
 * These registers provide the software write pointers of the per-lane downlink circular buffers.
 *
 */
union cvmx_rfif_cpri_lanex_eth_dl_sts0 {
	uint64_t u64;
	struct cvmx_rfif_cpri_lanex_eth_dl_sts0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t sw_wr_ptr                    : 40; /**< Location where the next packet should be written by software.
                                                         This pointer is updated when software writes the doorbell register
                                                         RFIF_CPRI_LANE()_ETH_DL_DB, indicating that packets have been added to
                                                         the circular buffer to be transmitted. */
#else
	uint64_t sw_wr_ptr                    : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_lanex_eth_dl_sts0_s cnf75xx;
};
typedef union cvmx_rfif_cpri_lanex_eth_dl_sts0 cvmx_rfif_cpri_lanex_eth_dl_sts0_t;

/**
 * cvmx_rfif_cpri_lane#_eth_dl_sts1
 *
 * These registers provide the next packet read pointers of the per-lane downlink circular
 * buffers.
 */
union cvmx_rfif_cpri_lanex_eth_dl_sts1 {
	uint64_t u64;
	struct cvmx_rfif_cpri_lanex_eth_dl_sts1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t next_rd_ptr                  : 40; /**< Location where the next packet to be transmitted will be read by
                                                         RFIF.  This pointer is updated as packets
                                                         are read. */
#else
	uint64_t next_rd_ptr                  : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_rfif_cpri_lanex_eth_dl_sts1_s cnf75xx;
};
typedef union cvmx_rfif_cpri_lanex_eth_dl_sts1 cvmx_rfif_cpri_lanex_eth_dl_sts1_t;

/**
 * cvmx_rfif_cpri_reset_ctrl
 */
union cvmx_rfif_cpri_reset_ctrl {
	uint64_t u64;
	struct cvmx_rfif_cpri_reset_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t lane5_reset                  : 8;  /**< See [LANE0_RESET] description. */
	uint64_t lane4_reset                  : 8;  /**< See [LANE0_RESET] description. */
	uint64_t lane3_reset                  : 8;  /**< See [LANE0_RESET] description. */
	uint64_t lane2_reset                  : 8;  /**< See [LANE0_RESET] description. */
	uint64_t lane1_reset                  : 8;  /**< See [LANE0_RESET] description. */
	uint64_t lane0_reset                  : 8;  /**< CPRI internal active low resets:
                                                         Bit 0 = reset_rx_n.
                                                         Bit 1 = reset_tx_n.
                                                         Bit 2 = config_reset_n.
                                                         Bit 3 = cpu_reset_n.
                                                         Bit 4 = cpri_rx_map_reset_n.
                                                         Bit 5 = cpri_tx_map_reset_n.
                                                         Bit 6 = cpri_ex_delay_reset_n.
                                                         Bit 7 = cpri_ipd_disable_reset (active high). */
#else
	uint64_t lane0_reset                  : 8;
	uint64_t lane1_reset                  : 8;
	uint64_t lane2_reset                  : 8;
	uint64_t lane3_reset                  : 8;
	uint64_t lane4_reset                  : 8;
	uint64_t lane5_reset                  : 8;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_rfif_cpri_reset_ctrl_s    cnf75xx;
};
typedef union cvmx_rfif_cpri_reset_ctrl cvmx_rfif_cpri_reset_ctrl_t;

/**
 * cvmx_rfif_cpuif#
 */
union cvmx_rfif_cpuifx {
	uint64_t u64;
	struct cvmx_rfif_cpuifx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cpuif                        : 32; /**< N/A */
#else
	uint64_t cpuif                        : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_cpuifx_s             cnf75xx;
};
typedef union cvmx_rfif_cpuifx cvmx_rfif_cpuifx_t;

/**
 * cvmx_rfif_dl_cksum_cfg#
 *
 * Downlink checksum configuration register:
 *
 * _ CFG0 corresponds to 1.4 MHz antennae.
 *
 * _ CFG1 corresponds to 3 MHz antennae.
 *
 * _ CFG2 corresponds to 5 MHz antennae.
 *
 * _ CFG3 corresponds to 10 MHz antennae.
 *
 * _ CFG4 corresponds to 15 MHz antennae.
 *
 * _ CFG5 corresponds to 20 MHz antennae.
 *
 * The checksum is caluclated within one 1 ms subframe and includes samples
 * in the range [START_SAMPLE, END_SAMPLE].
 */
union cvmx_rfif_dl_cksum_cfgx {
	uint64_t u64;
	struct cvmx_rfif_dl_cksum_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t end_sample                   : 15; /**< Last sample of the checksum. END_SAMPLE+1 must be a multiple of 4. */
	uint64_t reserved_15_31               : 17;
	uint64_t start_sample                 : 15; /**< First sample of the checksum. START_SAMPLE must be a multiple of 4. */
#else
	uint64_t start_sample                 : 15;
	uint64_t reserved_15_31               : 17;
	uint64_t end_sample                   : 15;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_rfif_dl_cksum_cfgx_s      cnf75xx;
};
typedef union cvmx_rfif_dl_cksum_cfgx cvmx_rfif_dl_cksum_cfgx_t;

/**
 * cvmx_rfif_dl_cksum_res#
 */
union cvmx_rfif_dl_cksum_resx {
	uint64_t u64;
	struct cvmx_rfif_dl_cksum_resx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t q_result                     : 16; /**< The checksum result for I samples. */
	uint64_t i_result                     : 16; /**< The checksum result for Q samples. */
#else
	uint64_t i_result                     : 16;
	uint64_t q_result                     : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_dl_cksum_resx_s      cnf75xx;
};
typedef union cvmx_rfif_dl_cksum_resx cvmx_rfif_dl_cksum_resx_t;

/**
 * cvmx_rfif_dl_cksum_start
 */
union cvmx_rfif_dl_cksum_start {
	uint64_t u64;
	struct cvmx_rfif_dl_cksum_start_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< Setting this bit will start the checksum for all antennae of bandwidth
                                                         enabled (i.e., all antennae with BW such that
                                                         RFIF_DL_CKSUM_CFG(0..5).END_SAMPLE!=0). This bit auto clears. */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_dl_cksum_start_s     cnf75xx;
};
typedef union cvmx_rfif_dl_cksum_start cvmx_rfif_dl_cksum_start_t;

/**
 * cvmx_rfif_dl_radio_axc_en#
 *
 * These registers control enables for each downlink lane (0..5).
 *
 */
union cvmx_rfif_dl_radio_axc_enx {
	uint64_t u64;
	struct cvmx_rfif_dl_radio_axc_enx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t enable                       : 32; /**< Enable bit for each of 32 downlink AxCs for the lane. */
#else
	uint64_t enable                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_dl_radio_axc_enx_s   cnf75xx;
};
typedef union cvmx_rfif_dl_radio_axc_enx cvmx_rfif_dl_radio_axc_enx_t;

/**
 * cvmx_rfif_dlfe_output_lat
 */
union cvmx_rfif_dlfe_output_lat {
	uint64_t u64;
	struct cvmx_rfif_dlfe_output_lat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t output_lat                   : 8;  /**< DLFE output latency, corresponding to the time required to fetch a basic frame worth of
                                                         samples, prior to the start of frame. It corresponds to a number of cycles for the 30.72
                                                         MHz clock. */
#else
	uint64_t output_lat                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_rfif_dlfe_output_lat_s    cnf75xx;
};
typedef union cvmx_rfif_dlfe_output_lat cvmx_rfif_dlfe_output_lat_t;

/**
 * cvmx_rfif_dlfe_proc_lat
 */
union cvmx_rfif_dlfe_proc_lat {
	uint64_t u64;
	struct cvmx_rfif_dlfe_proc_lat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t proc_lat                     : 14; /**< DLFE processing latency, corresponding to the time required to prepare a number of
                                                         samples, prior to the start of RFIF frame. It
                                                         corresponds to a number of 30.72 MHz clock cycles of latency. */
#else
	uint64_t proc_lat                     : 14;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_rfif_dlfe_proc_lat_s      cnf75xx;
};
typedef union cvmx_rfif_dlfe_proc_lat cvmx_rfif_dlfe_proc_lat_t;

/**
 * cvmx_rfif_eco
 */
union cvmx_rfif_eco {
	uint64_t u64;
	struct cvmx_rfif_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< N/A */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_rfif_eco_s                cnf75xx;
};
typedef union cvmx_rfif_eco cvmx_rfif_eco_t;

/**
 * cvmx_rfif_eth_abort
 *
 * Ethernet abort register.
 *
 */
union cvmx_rfif_eth_abort {
	uint64_t u64;
	struct cvmx_rfif_eth_abort_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t tx_abort                     : 6;  /**< An abort was raised by the CPRI Ethernet TX logic.
                                                         Bits 21-16 = eth_tx lane [n]. */
	uint64_t reserved_6_15                : 10;
	uint64_t rx_abort                     : 6;  /**< An abort was raised by the CPRI Ethernet RX logic.
                                                         Bits 5-0 = eth_rx lane [n]. */
#else
	uint64_t rx_abort                     : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t tx_abort                     : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_rfif_eth_abort_s          cnf75xx;
};
typedef union cvmx_rfif_eth_abort cvmx_rfif_eth_abort_t;

/**
 * cvmx_rfif_eth_ecc_ctrl
 *
 * Ethernet ECC control register.
 *
 */
union cvmx_rfif_eth_ecc_ctrl {
	uint64_t u64;
	struct cvmx_rfif_eth_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_39_63               : 25;
	uint64_t flip_bit2                    : 7;  /**< This bit flips a second bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_23_31               : 9;
	uint64_t flip_bit1                    : 7;  /**< This bit flips the first bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_7_15                : 9;
	uint64_t cor_dis                      : 7;  /**< This bit disables correction and checking of the respective RAM. */
#else
	uint64_t cor_dis                      : 7;
	uint64_t reserved_7_15                : 9;
	uint64_t flip_bit1                    : 7;
	uint64_t reserved_23_31               : 9;
	uint64_t flip_bit2                    : 7;
	uint64_t reserved_39_63               : 25;
#endif
	} s;
	struct cvmx_rfif_eth_ecc_ctrl_s       cnf75xx;
};
typedef union cvmx_rfif_eth_ecc_ctrl cvmx_rfif_eth_ecc_ctrl_t;

/**
 * cvmx_rfif_eth_ecc_err
 *
 * Ethernet ECC error register.
 *
 */
union cvmx_rfif_eth_ecc_err {
	uint64_t u64;
	struct cvmx_rfif_eth_ecc_err_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t ecc_ded                      : 7;  /**< A double-bit ECC error was detected on the respective RAM. */
	uint64_t reserved_7_15                : 9;
	uint64_t ecc_sec                      : 7;  /**< A single-bit ECC error was detected and corrected on the
                                                         respective RAM. */
#else
	uint64_t ecc_sec                      : 7;
	uint64_t reserved_7_15                : 9;
	uint64_t ecc_ded                      : 7;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_rfif_eth_ecc_err_s        cnf75xx;
};
typedef union cvmx_rfif_eth_ecc_err cvmx_rfif_eth_ecc_err_t;

/**
 * cvmx_rfif_eth_rx#_ecc_info
 *
 * These registers capture details about the first ECC error encountered
 * for the given RX Ethernet RAM.  The values are held until the error bit
 * is cleared in the RFIF_ETH_ECC_ERR register.  When that occurs, the
 * captured info is cleared, and the info register will capture the
 * details about the next ECC error.
 */
union cvmx_rfif_eth_rxx_ecc_info {
	uint64_t u64;
	struct cvmx_rfif_eth_rxx_ecc_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t err_type                     : 2;  /**< Identifies the type of ECC error that was captured in
                                                         this register.  2=DBE.  1=SBE.  0=no error. */
	uint64_t reserved_24_31               : 8;
	uint64_t synd                         : 8;  /**< N/A */
	uint64_t reserved_8_15                : 8;
	uint64_t ecc                          : 8;  /**< N/A */
#else
	uint64_t ecc                          : 8;
	uint64_t reserved_8_15                : 8;
	uint64_t synd                         : 8;
	uint64_t reserved_24_31               : 8;
	uint64_t err_type                     : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_rfif_eth_rxx_ecc_info_s   cnf75xx;
};
typedef union cvmx_rfif_eth_rxx_ecc_info cvmx_rfif_eth_rxx_ecc_info_t;

/**
 * cvmx_rfif_eth_tx_ecc_info
 *
 * These registers capture details about the first ECC error encountered
 * for the TX Ethernet RAM.  The values are held until the error bit
 * is cleared in the RFIF_ETH_ECC_ERR register.  When that occurs, the
 * captured info is cleared, and the info register will capture the
 * details about the next ECC error.
 */
union cvmx_rfif_eth_tx_ecc_info {
	uint64_t u64;
	struct cvmx_rfif_eth_tx_ecc_info_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t err_type                     : 2;  /**< Identifies the type of ECC error that was captured in
                                                         this register.  2=DBE.  1=SBE.  0=no error. */
	uint64_t reserved_24_31               : 8;
	uint64_t synd                         : 8;  /**< N/A */
	uint64_t reserved_8_15                : 8;
	uint64_t ecc                          : 8;  /**< N/A */
#else
	uint64_t ecc                          : 8;
	uint64_t reserved_8_15                : 8;
	uint64_t synd                         : 8;
	uint64_t reserved_24_31               : 8;
	uint64_t err_type                     : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_rfif_eth_tx_ecc_info_s    cnf75xx;
};
typedef union cvmx_rfif_eth_tx_ecc_info cvmx_rfif_eth_tx_ecc_info_t;

/**
 * cvmx_rfif_int_sum
 *
 * This register reports RFIF interrupts. Write one to clear a given interrupt bit.
 * Certain interrupts can only occur or are only meaningful in certain RMAC
 * modes. These should be ignored when the RMAC is operating in a different
 * mode.
 *
 * Available in all modes:
 *
 * _ [DL_SOS_MISS]
 *
 * _ [PP2S]
 *
 * _ [START_SUB_FRAME]
 *
 * _ [START_RADIO_FRAME]
 *
 * CPRI, TOFB, and SDL, but not availble for TOSP:
 *
 * _ CPRI_DL_UFLOW_LANE(0..5)
 *
 * _ LOS_LOF_LANE(0..5)
 *
 * CPRI mode only:
 *
 * _ [ETH_DB_ERR]
 *
 * _ [ETH_SBE]
 *
 * _ [ETH_DBE]
 *
 * _ [ETH_ABORT]
 *
 * _ [ETH_PKT_ARR_NOTIF]
 *
 * _ CPRI_UL_OFLOW_LANE(0..5)
 *
 * TOSP mode only:
 *
 * _ PPS_SYNC_DONE_RMAC(0..2)
 *
 * _ SPI_EVENT3_DONE_RMAC(0..2)
 *
 * _ SPI_EVENT2_DONE_RMAC(0..2)
 *
 * _ SPI_EVENT1_DONE_RMAC(0..2)
 *
 * _ SPI_EVENT0_DONE_RMAC(0..2)
 *
 * _ START_TX_FRAME_RMAC(0..2)
 *
 * _ START_RX_FRAME_RMAC(0..2)
 *
 * _ FIFO_FLAGS_RMAC(0..2)
 *
 * _ SPI_DONE_RMAC(0..2).
 */
union cvmx_rfif_int_sum {
	uint64_t u64;
	struct cvmx_rfif_int_sum_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_61_63               : 3;
	uint64_t dl_sos_miss                  : 1;  /**< SOS_DL was not generated due to an abnormal conditon. */
	uint64_t eth_db_err                   : 1;  /**< Error detected in Ethernet doorbell register programming. */
	uint64_t eth_sbe                      : 1;  /**< Single-bit ECC error detected (and corrected) in an Ethernet RAM. */
	uint64_t eth_dbe                      : 1;  /**< Double-bit ECC error detected in an Ethernet RAM. */
	uint64_t eth_abort                    : 1;  /**< Ethernet abort detected. */
	uint64_t pps_sync_done_rmac2          : 1;  /**< TOSP synchronization to 1PPS done. */
	uint64_t spi_event3_done_rmac2        : 1;  /**< TOSP SPI transfer done for event 3. */
	uint64_t spi_event2_done_rmac2        : 1;  /**< TOSP SPI transfer done for event 2. */
	uint64_t spi_event1_done_rmac2        : 1;  /**< TOSP SPI transfer done for event 1. */
	uint64_t spi_event0_done_rmac2        : 1;  /**< TOSP SPI transfer done for event 0. */
	uint64_t start_tx_frame_rmac2         : 1;  /**< TOSP start of TX frame status. */
	uint64_t start_rx_frame_rmac2         : 1;  /**< TOSP start of RX frame status. */
	uint64_t fifo_flags_rmac2             : 1;  /**< TOSP FIFO flags status. */
	uint64_t pps_sync_done_rmac1          : 1;  /**< TOSP synchronization to 1PPS done. */
	uint64_t spi_event3_done_rmac1        : 1;  /**< TOSP SPI transfer done for event 3. */
	uint64_t spi_event2_done_rmac1        : 1;  /**< TOSP SPI transfer done for event 2. */
	uint64_t spi_event1_done_rmac1        : 1;  /**< TOSP SPI transfer done for event 1. */
	uint64_t spi_event0_done_rmac1        : 1;  /**< TOSP SPI transfer done for event 0. */
	uint64_t start_tx_frame_rmac1         : 1;  /**< TOSP start of TX frame. */
	uint64_t start_rx_frame_rmac1         : 1;  /**< TOSP start of RX frame status. */
	uint64_t fifo_flags_rmac1             : 1;  /**< TOSP FIFO flags status. */
	uint64_t pps_sync_done_rmac0          : 1;  /**< TOSP synchronization to 1PPS done. */
	uint64_t spi_event3_done_rmac0        : 1;  /**< TOSP SPI transfer done for event 3. */
	uint64_t spi_event2_done_rmac0        : 1;  /**< TOSP SPI transfer done for event 2. */
	uint64_t spi_event1_done_rmac0        : 1;  /**< TOSP SPI transfer done for event 1. */
	uint64_t spi_event0_done_rmac0        : 1;  /**< TOSP SPI transfer done for event 0. */
	uint64_t start_tx_frame_rmac0         : 1;  /**< TOSP start of TX frame. */
	uint64_t start_rx_frame_rmac0         : 1;  /**< TOSP start of RX frame. */
	uint64_t fifo_flags_rmac0             : 1;  /**< TOSP FIFO flags. */
	uint64_t reserved_31_31               : 1;
	uint64_t spi_done_rmac2               : 1;  /**< TOSP SPI transaction done for RMAC 2. */
	uint64_t spi_done_rmac1               : 1;  /**< TOSP SPI transaction done for RMAC 1. */
	uint64_t spi_done_rmac0               : 1;  /**< TOSP SPI transaction done for RMAC 0. */
	uint64_t pp2s                         : 1;  /**< Pulse per two seconds. While PP2S is intended for use with GSM over
                                                         CPRI, the interrupt is triggered when the PP2S pulse occurs,
                                                         regardless of the RMAC mode. */
	uint64_t start_sub_frame              : 1;  /**< Start of sub frame (1 ms). */
	uint64_t start_radio_frame            : 1;  /**< Start of radio frame (10 ms). */
	uint64_t eth_pkt_arr_notif            : 1;  /**< Ethernet packet arrival notification. */
	uint64_t reserved_22_23               : 2;
	uint64_t cpri_dl_uflow_lane5          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t cpri_dl_uflow_lane4          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t cpri_dl_uflow_lane3          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t cpri_dl_uflow_lane2          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t cpri_dl_uflow_lane1          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t cpri_dl_uflow_lane0          : 1;  /**< CPRI, SDL, and TOFB  DL buffer overflow status. */
	uint64_t reserved_14_15               : 2;
	uint64_t cpri_ul_oflow_lane5          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t cpri_ul_oflow_lane4          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t cpri_ul_oflow_lane3          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t cpri_ul_oflow_lane2          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t cpri_ul_oflow_lane1          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t cpri_ul_oflow_lane0          : 1;  /**< CPRI map UL buffer overflow status. */
	uint64_t reserved_6_7                 : 2;
	uint64_t los_lof_lane5                : 1;  /**< LOF or LOS interrupt status. */
	uint64_t los_lof_lane4                : 1;  /**< LOF or LOS interrupt status. */
	uint64_t los_lof_lane3                : 1;  /**< LOF or LOS interrupt status. */
	uint64_t los_lof_lane2                : 1;  /**< LOF or LOS interrupt status. */
	uint64_t los_lof_lane1                : 1;  /**< LOF or LOS interrupt status. */
	uint64_t los_lof_lane0                : 1;  /**< LOF or LOS interrupt status. */
#else
	uint64_t los_lof_lane0                : 1;
	uint64_t los_lof_lane1                : 1;
	uint64_t los_lof_lane2                : 1;
	uint64_t los_lof_lane3                : 1;
	uint64_t los_lof_lane4                : 1;
	uint64_t los_lof_lane5                : 1;
	uint64_t reserved_6_7                 : 2;
	uint64_t cpri_ul_oflow_lane0          : 1;
	uint64_t cpri_ul_oflow_lane1          : 1;
	uint64_t cpri_ul_oflow_lane2          : 1;
	uint64_t cpri_ul_oflow_lane3          : 1;
	uint64_t cpri_ul_oflow_lane4          : 1;
	uint64_t cpri_ul_oflow_lane5          : 1;
	uint64_t reserved_14_15               : 2;
	uint64_t cpri_dl_uflow_lane0          : 1;
	uint64_t cpri_dl_uflow_lane1          : 1;
	uint64_t cpri_dl_uflow_lane2          : 1;
	uint64_t cpri_dl_uflow_lane3          : 1;
	uint64_t cpri_dl_uflow_lane4          : 1;
	uint64_t cpri_dl_uflow_lane5          : 1;
	uint64_t reserved_22_23               : 2;
	uint64_t eth_pkt_arr_notif            : 1;
	uint64_t start_radio_frame            : 1;
	uint64_t start_sub_frame              : 1;
	uint64_t pp2s                         : 1;
	uint64_t spi_done_rmac0               : 1;
	uint64_t spi_done_rmac1               : 1;
	uint64_t spi_done_rmac2               : 1;
	uint64_t reserved_31_31               : 1;
	uint64_t fifo_flags_rmac0             : 1;
	uint64_t start_rx_frame_rmac0         : 1;
	uint64_t start_tx_frame_rmac0         : 1;
	uint64_t spi_event0_done_rmac0        : 1;
	uint64_t spi_event1_done_rmac0        : 1;
	uint64_t spi_event2_done_rmac0        : 1;
	uint64_t spi_event3_done_rmac0        : 1;
	uint64_t pps_sync_done_rmac0          : 1;
	uint64_t fifo_flags_rmac1             : 1;
	uint64_t start_rx_frame_rmac1         : 1;
	uint64_t start_tx_frame_rmac1         : 1;
	uint64_t spi_event0_done_rmac1        : 1;
	uint64_t spi_event1_done_rmac1        : 1;
	uint64_t spi_event2_done_rmac1        : 1;
	uint64_t spi_event3_done_rmac1        : 1;
	uint64_t pps_sync_done_rmac1          : 1;
	uint64_t fifo_flags_rmac2             : 1;
	uint64_t start_rx_frame_rmac2         : 1;
	uint64_t start_tx_frame_rmac2         : 1;
	uint64_t spi_event0_done_rmac2        : 1;
	uint64_t spi_event1_done_rmac2        : 1;
	uint64_t spi_event2_done_rmac2        : 1;
	uint64_t spi_event3_done_rmac2        : 1;
	uint64_t pps_sync_done_rmac2          : 1;
	uint64_t eth_abort                    : 1;
	uint64_t eth_dbe                      : 1;
	uint64_t eth_sbe                      : 1;
	uint64_t eth_db_err                   : 1;
	uint64_t dl_sos_miss                  : 1;
	uint64_t reserved_61_63               : 3;
#endif
	} s;
	struct cvmx_rfif_int_sum_s            cnf75xx;
};
typedef union cvmx_rfif_int_sum cvmx_rfif_int_sum_t;

/**
 * cvmx_rfif_lanes_enables
 */
union cvmx_rfif_lanes_enables {
	uint64_t u64;
	struct cvmx_rfif_lanes_enables_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t ul_enable                    : 6;  /**< Enable bit per RFIF UL lane path (bit(i) corresponds to lane i). Set this bit after the
                                                         radio interface is active. */
	uint64_t reserved_6_7                 : 2;
	uint64_t dl_enable                    : 6;  /**< Enable bit per RFIF DL lane path (bit(i) corresponds to lane i). Set this bit after the
                                                         radio interface is active. */
#else
	uint64_t dl_enable                    : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t ul_enable                    : 6;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_rfif_lanes_enables_s      cnf75xx;
};
typedef union cvmx_rfif_lanes_enables cvmx_rfif_lanes_enables_t;

/**
 * cvmx_rfif_lof_cnt
 */
union cvmx_rfif_lof_cnt {
	uint64_t u64;
	struct cvmx_rfif_lof_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t lane5                        : 8;  /**< See LANE0. */
	uint64_t lane4                        : 8;  /**< See LANE0. */
	uint64_t lane3                        : 8;  /**< See LANE0. */
	uint64_t lane2                        : 8;  /**< See LANE0. */
	uint64_t lane1                        : 8;  /**< See LANE0. */
	uint64_t lane0                        : 8;  /**< Count of the number of LOF events that occured. This register count rolls over from 255 to
                                                         0, so no LOF events are missed. */
#else
	uint64_t lane0                        : 8;
	uint64_t lane1                        : 8;
	uint64_t lane2                        : 8;
	uint64_t lane3                        : 8;
	uint64_t lane4                        : 8;
	uint64_t lane5                        : 8;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_rfif_lof_cnt_s            cnf75xx;
};
typedef union cvmx_rfif_lof_cnt cvmx_rfif_lof_cnt_t;

/**
 * cvmx_rfif_loop_cfg
 */
union cvmx_rfif_loop_cfg {
	uint64_t u64;
	struct cvmx_rfif_loop_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_25_63               : 39;
	uint64_t bf_count                     : 5;  /**< Number of basic frames minus one before pausing one extra cycles.
                                                         When SCLK is 800 MHz, set BF_COUNT=2 to add one extra
                                                         cycle every 3 basic frames (to average 208.33 cycles per basic frame). */
	uint64_t reserved_17_19               : 3;
	uint64_t rl_count                     : 9;  /**< The number of clock cycles minus one per basic frame to control the
                                                         rate limiter. The basic frame frequency is 3.84 MHZ.
                                                         When SCLK is 800 MHz, there should be 208.33 cycles
                                                         per BF. Set RL_COUNT=207, and BF_COUNT=2 to have 208 cycles per BF,
                                                         with an extra cycle in every 3rd BF. */
	uint64_t reserved_3_7                 : 5;
	uint64_t rfp_sel                      : 1;  /**< Select source for the radio frame pulse:
                                                         0 = Use RFP from BTS.
                                                         1 = Use RFIF_LOOP_RFP control. */
	uint64_t clk_sel                      : 1;  /**< Reserved. */
	uint64_t loop_en                      : 1;  /**< Enable downlink to uplink loopback from the DLFE to ULFE and PRCH. */
#else
	uint64_t loop_en                      : 1;
	uint64_t clk_sel                      : 1;
	uint64_t rfp_sel                      : 1;
	uint64_t reserved_3_7                 : 5;
	uint64_t rl_count                     : 9;
	uint64_t reserved_17_19               : 3;
	uint64_t bf_count                     : 5;
	uint64_t reserved_25_63               : 39;
#endif
	} s;
	struct cvmx_rfif_loop_cfg_s           cnf75xx;
};
typedef union cvmx_rfif_loop_cfg cvmx_rfif_loop_cfg_t;

/**
 * cvmx_rfif_loop_rfp
 */
union cvmx_rfif_loop_rfp {
	uint64_t u64;
	struct cvmx_rfif_loop_rfp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t rfp                          : 1;  /**< Write 1 to generate the first radio frame pulse. The timer block will
                                                         then automatically generate the radio frame pulses,based on the
                                                         30.72 MHz clock. */
#else
	uint64_t rfp                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_loop_rfp_s           cnf75xx;
};
typedef union cvmx_rfif_loop_rfp cvmx_rfif_loop_rfp_t;

/**
 * cvmx_rfif_los_cnt
 */
union cvmx_rfif_los_cnt {
	uint64_t u64;
	struct cvmx_rfif_los_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t lane5                        : 8;  /**< See LANE0. */
	uint64_t lane4                        : 8;  /**< See LANE0. */
	uint64_t lane3                        : 8;  /**< See LANE0. */
	uint64_t lane2                        : 8;  /**< See LANE0. */
	uint64_t lane1                        : 8;  /**< See LANE0. */
	uint64_t lane0                        : 8;  /**< Count of the number of LOS events that occured. This register count rolls over from 255 to
                                                         0, so no LOS events are missed. */
#else
	uint64_t lane0                        : 8;
	uint64_t lane1                        : 8;
	uint64_t lane2                        : 8;
	uint64_t lane3                        : 8;
	uint64_t lane4                        : 8;
	uint64_t lane5                        : 8;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_rfif_los_cnt_s            cnf75xx;
};
typedef union cvmx_rfif_los_cnt cvmx_rfif_los_cnt_t;

/**
 * cvmx_rfif_master_cfg
 *
 * This register configures lane rates, clock gating and resets for RFIF.
 *
 */
union cvmx_rfif_master_cfg {
	uint64_t u64;
	struct cvmx_rfif_master_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t one_ms_rf                    : 1;  /**< Reserved. Must be 0. */
	uint64_t rmac2_mode                   : 3;  /**< RMAC mode. See [RMAC0_MODE] for details. */
	uint64_t rmac1_mode                   : 3;  /**< RMAC mode. See [RMAC0_MODE] for details. */
	uint64_t rmac0_mode                   : 3;  /**< Select the RF interface mode for RMAC 0:
                                                         0x0 = CPRI.
                                                         0x1 = JESD 204B (TOFB).
                                                         0x2 = SerDes-Lite (SDL).
                                                         0x3 = JESD 207 (TOSP).
                                                         Note that [RMAC0_MODE], [RMAC1_MODE], and [RMAC2_MODE] must all be set to
                                                         the same value. Mixed-mode operation is not supported. */
	uint64_t rmac2_sw_reset_div2_n        : 1;  /**< Deassertion of this soft reset bit will enable the GSER
                                                         tx_clk (muxed) clock divider. Deassert after DLM_RST_RDY is asserted */
	uint64_t rmac1_sw_reset_div2_n        : 1;  /**< Deassertion of this soft reset bit will enable the GSER
                                                         tx_clk (muxed) clock divider. Deassert after DLM_RST_RDY is asserted */
	uint64_t rmac0_sw_reset_div2_n        : 1;  /**< Deassertion of this soft reset bit will enable the GSER
                                                         tx_clk (muxed) clock divider. Deassert after DLM_RST_RDY is asserted */
	uint64_t rmac2_sw_reset_n             : 1;  /**< RMAC soft reset. Deassert after [RMAC2_SW_RESET_DIV2_N].  Perform soft
                                                         BIST on GSER clock memories between [RMAC2_SW_RESET_DIV2_N] and this
                                                         soft reset. */
	uint64_t rmac1_sw_reset_n             : 1;  /**< RMAC soft reset. Deassert after [RMAC1_SW_RESET_DIV2_N].  Perform soft
                                                         BIST on GSER clock memories between [RMAC1_SW_RESET_DIV2_N] and this
                                                         soft reset. */
	uint64_t rmac0_sw_reset_n             : 1;  /**< RMAC soft reset. Deassert after [RMAC0_SW_RESET_DIV2_N].  Perform soft
                                                         BIST on GSER clock memories between [RMAC0_SW_RESET_DIV2_N] and this
                                                         soft reset. */
	uint64_t rmac_delivers_cpri_tx_sync_rfp : 1;/**< Reserved. Must be 1. */
	uint64_t lane5_dlm_txclk_sel          : 2;  /**< Select TX clock from DLM. See [LANE0_DLM_TXCLK_SEL]. */
	uint64_t lane5_rate                   : 5;  /**< Lane rate selection. See [LANE0_RATE]. */
	uint64_t sim_spup                     : 1;  /**< N/A */
	uint64_t lane4_dlm_txclk_sel          : 2;  /**< Select TX clock from DLM. See [LANE0_DLM_TXCLK_SEL]. */
	uint64_t lane4_rate                   : 5;  /**< Lane rate selection. See [LANE0_RATE]. */
	uint64_t reserved_31_31               : 1;
	uint64_t lane3_dlm_txclk_sel          : 2;  /**< Select TX clock from DLM. See [LANE0_DLM_TXCLK_SEL]. */
	uint64_t lane3_rate                   : 5;  /**< Lane rate selection. See [LANE0_RATE]. */
	uint64_t reserved_23_23               : 1;
	uint64_t lane2_dlm_txclk_sel          : 2;  /**< Select TX clock from DLM. See [LANE0_DLM_TXCLK_SEL]. */
	uint64_t lane2_rate                   : 5;  /**< Lane rate selection. See [LANE0_RATE]. */
	uint64_t reserved_15_15               : 1;
	uint64_t lane1_dlm_txclk_sel          : 2;  /**< Select TX clock from DLM. See [LANE0_DLM_TXCLK_SEL]. */
	uint64_t lane1_rate                   : 5;  /**< Lane rate selection. See [LANE0_RATE]. */
	uint64_t tadv_upd_dlmap_rst           : 1;  /**< Set to enable automatic DL MAP reset when the timing advance is changed. */
	uint64_t lane0_dlm_txclk_sel          : 2;  /**< Select one of 4 TX clocks from SerDes DLM:
                                                         0x0 = tx_clk, used when [LANE0_RATE] = 0xA (9.8 Gbps) or 0x10 (6.1 Gbps).
                                                         0x1 = tx_div2_clk, used when [LANE0_RATE] = 0x5 (3.1 Gbps) or 0x8 (4.9
                                                         Gbps).
                                                         0x2 = tx_div4_clk, used when [LANE0_RATE] = 0x4 (2.5 Gbps).
                                                         0x3 = tx_div8_clk, used when [LANE0_RATE] = 0x2 (1.2 Gbps).
                                                         Note that LANE0_DLM_TXCLK_SEL is not used (i.e., ignored) when
                                                         [RMAC0_MODE]=0x2 (SDL). */
	uint64_t lane0_rate                   : 5;  /**< Lane rate selection (unlisted values are reserved):
                                                         0x2 = 1.2 Gbps.
                                                         0x4 = 2.5 Gbps.
                                                         0x5 = 3.1 Gbps.
                                                         0x8 = 4.9 Gbps.
                                                         0xA = 6.1 Gbps.
                                                         0x10 = 9.8 Gbps.
                                                         0x11 = 10.1 Gbps.
                                                         Note that when [RMAC0_MODE]=0x2 (SDL), the lane rate should be set to
                                                         0x10 (9.8 Gbps). The SDL interface uses two SerDes lanes at 4.9 Gbps,
                                                         and the single RMAC lane must run at 9.8 Gbps (the sum of the rate of
                                                         the two SerDes lanes). */
#else
	uint64_t lane0_rate                   : 5;
	uint64_t lane0_dlm_txclk_sel          : 2;
	uint64_t tadv_upd_dlmap_rst           : 1;
	uint64_t lane1_rate                   : 5;
	uint64_t lane1_dlm_txclk_sel          : 2;
	uint64_t reserved_15_15               : 1;
	uint64_t lane2_rate                   : 5;
	uint64_t lane2_dlm_txclk_sel          : 2;
	uint64_t reserved_23_23               : 1;
	uint64_t lane3_rate                   : 5;
	uint64_t lane3_dlm_txclk_sel          : 2;
	uint64_t reserved_31_31               : 1;
	uint64_t lane4_rate                   : 5;
	uint64_t lane4_dlm_txclk_sel          : 2;
	uint64_t sim_spup                     : 1;
	uint64_t lane5_rate                   : 5;
	uint64_t lane5_dlm_txclk_sel          : 2;
	uint64_t rmac_delivers_cpri_tx_sync_rfp : 1;
	uint64_t rmac0_sw_reset_n             : 1;
	uint64_t rmac1_sw_reset_n             : 1;
	uint64_t rmac2_sw_reset_n             : 1;
	uint64_t rmac0_sw_reset_div2_n        : 1;
	uint64_t rmac1_sw_reset_div2_n        : 1;
	uint64_t rmac2_sw_reset_div2_n        : 1;
	uint64_t rmac0_mode                   : 3;
	uint64_t rmac1_mode                   : 3;
	uint64_t rmac2_mode                   : 3;
	uint64_t one_ms_rf                    : 1;
#endif
	} s;
	struct cvmx_rfif_master_cfg_s         cnf75xx;
};
typedef union cvmx_rfif_master_cfg cvmx_rfif_master_cfg_t;

/**
 * cvmx_rfif_olos_stat
 */
union cvmx_rfif_olos_stat {
	uint64_t u64;
	struct cvmx_rfif_olos_stat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t olos_stat                    : 6;  /**< Optical LOS status. Bits 0-5 are associated with Serdes lanes 0-5 respectively.
                                                         When a bit is set, it indicates an optical LOS was detected for the associated Serdes
                                                         lane. */
#else
	uint64_t olos_stat                    : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_rfif_olos_stat_s          cnf75xx;
};
typedef union cvmx_rfif_olos_stat cvmx_rfif_olos_stat_t;

/**
 * cvmx_rfif_prach_sos_cfg
 */
union cvmx_rfif_prach_sos_cfg {
	uint64_t u64;
	struct cvmx_rfif_prach_sos_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t cfg_sel                      : 1;  /**< 0 = No offset relative to SOS_UL sent to ULFE.
                                                         1 = PRCH SOS delayed by SOS_OFFSET basic frames. */
	uint64_t reserved_12_15               : 4;
	uint64_t sos_offset                   : 12; /**< PRCH SOS delay offset relative to the ULFE SOS. The value programmed is in terms of
                                                         number of basic frames. The configuration must meet the constraint
                                                         RFIF_RETARD[RETARD]+SOS_OFFSET <= 999us.
                                                         The SOS_OFFSET must be a multiple of N, where N depends on the lowest
                                                         antenna bandwidth as follows:
                                                         _ If minimum BW >= 10 MHz, N = 1.
                                                         _ If minimum BW = 5 MHz, N = 2.
                                                         _ If minimum BW = 2.5 MHz, N = 4.
                                                         _ If minimum BW = 1.3 MHz, N = 8.
                                                         When adding an new antenna with lower bandwidth than the current
                                                         minimum, software should first ensure that SOS_OFFSET is adjusted to
                                                         meet the above constraint before adding the new antenna. */
#else
	uint64_t sos_offset                   : 12;
	uint64_t reserved_12_15               : 4;
	uint64_t cfg_sel                      : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_rfif_prach_sos_cfg_s      cnf75xx;
};
typedef union cvmx_rfif_prach_sos_cfg cvmx_rfif_prach_sos_cfg_t;

/**
 * cvmx_rfif_prs_swc
 */
union cvmx_rfif_prs_swc {
	uint64_t u64;
	struct cvmx_rfif_prs_swc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t conv_style                   : 2;  /**< 0x0 = SAT15(X).
                                                         0x1 = SAT15(ROUND15(X)).
                                                         0x2 = No sample width conversion.
                                                         0x3 = Reserved.
                                                         Where SAT15() is symmetric saturation to the range [-32767, 32767], and
                                                         ROUND15() is symmetric rounding:
                                                         _ ROUND15(X) = (X + 1) >> 1, if X >= 0.
                                                         _ ROUND15(X) = -((-X + 1) >> 1), if X < 0. */
#else
	uint64_t conv_style                   : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_rfif_prs_swc_s            cnf75xx;
};
typedef union cvmx_rfif_prs_swc cvmx_rfif_prs_swc_t;

/**
 * cvmx_rfif_reset_ctrl
 */
union cvmx_rfif_reset_ctrl {
	uint64_t u64;
	struct cvmx_rfif_reset_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t rfif_timer                   : 1;  /**< Active low reset of the rfif timer. */
	uint64_t rfif_ncb                     : 1;  /**< Active low reset of the rfif NCB interface. */
	uint64_t rfif_eth                     : 1;  /**< Active low reset of the rfif Ethernet data path. */
	uint64_t rfif_dl                      : 1;  /**< Active low reset of the rfif downlink data path. */
	uint64_t rfif_ul                      : 1;  /**< Active low reset of the rfif uplink data path. */
#else
	uint64_t rfif_ul                      : 1;
	uint64_t rfif_dl                      : 1;
	uint64_t rfif_eth                     : 1;
	uint64_t rfif_ncb                     : 1;
	uint64_t rfif_timer                   : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_rfif_reset_ctrl_s         cnf75xx;
};
typedef union cvmx_rfif_reset_ctrl cvmx_rfif_reset_ctrl_t;

/**
 * cvmx_rfif_retard
 *
 * This register specifies the uplink retard value used for over the air synchronization.
 *
 */
union cvmx_rfif_retard {
	uint64_t u64;
	struct cvmx_rfif_retard_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t retard                       : 15; /**< UL retard value, in terms of 30.72 MHz clock cycles. The valid range is 0 to 30689, which
                                                         corresponds to a range of 0 to 999 us. */
#else
	uint64_t retard                       : 15;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_rfif_retard_s             cnf75xx;
};
typedef union cvmx_rfif_retard cvmx_rfif_retard_t;

/**
 * cvmx_rfif_rmac#_clken
 *
 * This register enables clocking of the RMAC and the different RF
 * interfaces.
 *
 * When an RMAC is un-used, set BCLK_EN=0 to disable the conditional clock
 * and save power.
 *
 * To reduce power, clocks to the unused RF interfaces (i.e., all interfaces
 * other than the one selected by the RMAC mode) can be disabled by setting
 * the corresponding clock enables to zero.
 */
union cvmx_rfif_rmacx_clken {
	uint64_t u64;
	struct cvmx_rfif_rmacx_clken_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t bclk_en                      : 1;  /**< Enable conditional clock driver for RMAC BCLK. */
	uint64_t tosp_clk_en                  : 1;  /**< Enable JESD 207 (TOSP) clocking in RMAC. */
	uint64_t sdl_clk_en                   : 1;  /**< Enable SDL clocking in RMAC. */
	uint64_t tofb_clk_en                  : 2;  /**< Enable JESD 204B (TOFB) clocking for lane 1 and lane 0 (one bit per
                                                         lane). */
	uint64_t cpri_clk_en                  : 2;  /**< Enable CPRI clocking for lane 1 and lane 0 (one bit per lane). */
#else
	uint64_t cpri_clk_en                  : 2;
	uint64_t tofb_clk_en                  : 2;
	uint64_t sdl_clk_en                   : 1;
	uint64_t tosp_clk_en                  : 1;
	uint64_t bclk_en                      : 1;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_rfif_rmacx_clken_s        cnf75xx;
};
typedef union cvmx_rfif_rmacx_clken cvmx_rfif_rmacx_clken_t;

/**
 * cvmx_rfif_sample_width
 */
union cvmx_rfif_sample_width {
	uint64_t u64;
	struct cvmx_rfif_sample_width_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t dl_sample_width_sat_bypass   : 1;  /**< RFIF downlink saturation bypass.
                                                         0 = Perform symmetric saturation to [DL_SAMPLE_WIDTH].
                                                         1 = Bypass saturation. If [DL_SAMPLE_WIDTH_OPTION]=0, no conversion is
                                                         performed (as if [DL_SAMPLE_WIDTH_BYPASS]=1). If
                                                         [DL_SAMPLE_WIDTH_OPTION]=1, the rounded value is truncated instead of
                                                         saturating. */
	uint64_t dl_sample_width_bypass       : 1;  /**< RFIF downlink sample width conversion bypass.
                                                         0 = Perform conversion specified by [DL_SAMPLE_WIDTH_OPTION] and
                                                         [DL_SAMPLE_WIDTH_SAT_BYPASS].
                                                         1 = Bypass conversion. This is equivalent to selecting the n most
                                                         significant bits, where n is determined by the RF interface. */
	uint64_t dl_sample_width_option       : 1;  /**< RFIF downlink sample width conversion:
                                                         - 0: Y = SATn(X).
                                                         - 1: Y = SATn(ROUND(X)).
                                                          SATn(X) is saturation to the range [-2^(n-1)+1, 2^(n-1)-1].
                                                          ROUND(X) is symmetric rounding, defined as:
                                                          _ ROUND(X) = (X + 2^(m-1)) >> m, if X >= 0.
                                                          _ ROUND(X) = -((-X + 2^(m-1)) >> m), if X < 0.
                                                          _ where m = 16 - n, and when m=0, 2^(0-1) = 0.
                                                          n is the sample bit width specified by [DL_SAMPLE_WIDTH].
                                                          Note that the saturation operation can be bypassed by setting
                                                          [DL_SAMPLE_WIDTH_SAT_BYPASS]=1. */
	uint64_t dl_sample_width              : 3;  /**< RFIF downlink sample bit width:
                                                         0x0 = 16 bits.
                                                         0x1 = 15 bits.
                                                         0x2 = 14 bits.
                                                         0x3 = 12 bits.
                                                         0x4 = 10 bits. */
	uint64_t reserved_7_7                 : 1;
	uint64_t ul_sample_width_bypass       : 1;  /**< RFIF UL sample width bypass.
                                                         0 = Perform sample width conversion specified by
                                                         [UL_SAMPLE_WIDTH_OPTION].
                                                         1 = Bypass sample width conversion. The left-justified value from the
                                                         RF interface is used, with no guarantee about the value of the
                                                         least-significant bits. */
	uint64_t ul_sample_width_option       : 1;  /**< RFIF UL sample width conversion method.
                                                         0 = Sign-extend to the left.
                                                         1 = Zero pad on the right. */
	uint64_t ul_sample_width              : 3;  /**< RFIF UL sample width:
                                                         0x0 = 16 bits.
                                                         0x1 = 15 bits.
                                                         0x2 = 14 bits.
                                                         0x3 = 12 bits. */
	uint64_t reserved_0_1                 : 2;
#else
	uint64_t reserved_0_1                 : 2;
	uint64_t ul_sample_width              : 3;
	uint64_t ul_sample_width_option       : 1;
	uint64_t ul_sample_width_bypass       : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t dl_sample_width              : 3;
	uint64_t dl_sample_width_option       : 1;
	uint64_t dl_sample_width_bypass       : 1;
	uint64_t dl_sample_width_sat_bypass   : 1;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_rfif_sample_width_s       cnf75xx;
};
typedef union cvmx_rfif_sample_width cvmx_rfif_sample_width_t;

/**
 * cvmx_rfif_sdl_reset_ctrl
 */
union cvmx_rfif_sdl_reset_ctrl {
	uint64_t u64;
	struct cvmx_rfif_sdl_reset_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t lane2_reset                  : 6;  /**< SDL lane 2 (RFIF lane 4) resets.  See [LANE0_RESET] description. */
	uint64_t reserved_14_15               : 2;
	uint64_t lane1_reset                  : 6;  /**< SDL lane 1 (RFIF lane 2) resets.  See [LANE0_RESET] description. */
	uint64_t reserved_6_7                 : 2;
	uint64_t lane0_reset                  : 6;  /**< SDL active low resets:
                                                         Bit 0 = core_reset_n.
                                                         Bit 1 = reserved.
                                                         Bit 2 = reserved.
                                                         Bit 3 = cpu_reset_n.
                                                         Bit 4 = rx_map_reset_n.
                                                         Bit 5 = tx_map_reset_n. */
#else
	uint64_t lane0_reset                  : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t lane1_reset                  : 6;
	uint64_t reserved_14_15               : 2;
	uint64_t lane2_reset                  : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_rfif_sdl_reset_ctrl_s     cnf75xx;
};
typedef union cvmx_rfif_sdl_reset_ctrl cvmx_rfif_sdl_reset_ctrl_t;

/**
 * cvmx_rfif_tim_adv
 *
 * This register specifies the downlink timing advance value used for over the air synchronization.
 *
 */
union cvmx_rfif_tim_adv {
	uint64_t u64;
	struct cvmx_rfif_tim_adv_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t tim_adv                      : 15; /**< Downlink timing advance value, in terms of 30.72 MHz clock cycles. The
                                                         valid range is 0 to 17726, which corresponds to a range of 0 to 577 us. */
#else
	uint64_t tim_adv                      : 15;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_rfif_tim_adv_s            cnf75xx;
};
typedef union cvmx_rfif_tim_adv cvmx_rfif_tim_adv_t;

/**
 * cvmx_rfif_timer_bfn_num
 */
union cvmx_rfif_timer_bfn_num {
	uint64_t u64;
	struct cvmx_rfif_timer_bfn_num_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t bfn_num                      : 32; /**< Reports the last received BFN. If the last BFN SYNC failed, the last BFN is incremented by one. */
#else
	uint64_t bfn_num                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_bfn_num_s      cnf75xx;
};
typedef union cvmx_rfif_timer_bfn_num cvmx_rfif_timer_bfn_num_t;

/**
 * cvmx_rfif_timer_bfn_num_bad_cnt
 */
union cvmx_rfif_timer_bfn_num_bad_cnt {
	uint64_t u64;
	struct cvmx_rfif_timer_bfn_num_bad_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t bad_cnt                      : 32; /**< Reports the number of times the received BFN did not correspond to the previous BFN+1. */
#else
	uint64_t bad_cnt                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_bfn_num_bad_cnt_s cnf75xx;
};
typedef union cvmx_rfif_timer_bfn_num_bad_cnt cvmx_rfif_timer_bfn_num_bad_cnt_t;

/**
 * cvmx_rfif_timer_bfn_num_ok
 */
union cvmx_rfif_timer_bfn_num_ok {
	uint64_t u64;
	struct cvmx_rfif_timer_bfn_num_ok_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t status                       : 1;  /**< Reports if the last received BFN corresponded to the previous BFN+1:
                                                         0 = BFN was not incremented by one.
                                                         1 = BFN was incremented by one. */
#else
	uint64_t status                       : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_timer_bfn_num_ok_s   cnf75xx;
};
typedef union cvmx_rfif_timer_bfn_num_ok cvmx_rfif_timer_bfn_num_ok_t;

/**
 * cvmx_rfif_timer_bfn_sync_fail_cnt
 */
union cvmx_rfif_timer_bfn_sync_fail_cnt {
	uint64_t u64;
	struct cvmx_rfif_timer_bfn_sync_fail_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t fail_count                   : 32; /**< Reports the number of times the BFN SYNC bit was missing. */
#else
	uint64_t fail_count                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_bfn_sync_fail_cnt_s cnf75xx;
};
typedef union cvmx_rfif_timer_bfn_sync_fail_cnt cvmx_rfif_timer_bfn_sync_fail_cnt_t;

/**
 * cvmx_rfif_timer_bfn_sync_ok
 */
union cvmx_rfif_timer_bfn_sync_ok {
	uint64_t u64;
	struct cvmx_rfif_timer_bfn_sync_ok_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t status                       : 1;  /**< The BFN input is composed by one SYNC bit and 12 serial BFN bits. This
                                                         bit indicates whether the SYNC bit was missing from the last received BFN (0 = missing, 1
                                                         =
                                                         present). */
#else
	uint64_t status                       : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_timer_bfn_sync_ok_s  cnf75xx;
};
typedef union cvmx_rfif_timer_bfn_sync_ok cvmx_rfif_timer_bfn_sync_ok_t;

/**
 * cvmx_rfif_timer_cfg
 */
union cvmx_rfif_timer_cfg {
	uint64_t u64;
	struct cvmx_rfif_timer_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t gen_clk_hp                   : 4;  /**< Reserved. */
	uint64_t clk_sel                      : 1;  /**< Reserved. Must be zero. */
	uint64_t bfn_per                      : 16; /**< BFN period in number of 30.72 MHz clock cycles. The BFN sync pulse if sampled BFN_PER/2
                                                         cycle after the RFP detection. The 12 BFN bits are then sampled with a period of BFN_PER
                                                         cycles. */
	uint64_t reserved_3_7                 : 5;
	uint64_t pp2s_pol                     : 1;  /**< Polarity of the pulse per 2 second device input:
                                                         0 = PP2S detected on a falling edge.
                                                         1 = PP2s detected on a rising edge. */
	uint64_t bfn_insert                   : 1;  /**< This bit is set to enable inserting the BFN number captured at the chip IO sync interface.
                                                         When cleared, the BFN starts at 0 and increments every radio frame to wrap around at 2047
                                                         to 0. */
	uint64_t rfp_pol                      : 1;  /**< Polarity of the radio frame pulse:
                                                         0 = RFP detected on a falling edge.
                                                         1 = RFP detected on a rising edge. */
#else
	uint64_t rfp_pol                      : 1;
	uint64_t bfn_insert                   : 1;
	uint64_t pp2s_pol                     : 1;
	uint64_t reserved_3_7                 : 5;
	uint64_t bfn_per                      : 16;
	uint64_t clk_sel                      : 1;
	uint64_t gen_clk_hp                   : 4;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_rfif_timer_cfg_s          cnf75xx;
};
typedef union cvmx_rfif_timer_cfg cvmx_rfif_timer_cfg_t;

/**
 * cvmx_rfif_timer_sfc
 */
union cvmx_rfif_timer_sfc {
	uint64_t u64;
	struct cvmx_rfif_timer_sfc_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t sfc_cnt                      : 32; /**< Reports the current sub-frame count (0-9). The sub-frame count is reset at the 10 ms
                                                         synchronization pulse. */
#else
	uint64_t sfc_cnt                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_sfc_s          cnf75xx;
};
typedef union cvmx_rfif_timer_sfc cvmx_rfif_timer_sfc_t;

/**
 * cvmx_rfif_timer_sync_cnt
 */
union cvmx_rfif_timer_sync_cnt {
	uint64_t u64;
	struct cvmx_rfif_timer_sync_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t sync_count                   : 32; /**< Reports the internal count at which the most recent 10 ms pulse was received.
                                                         SYNC_COUNT is the combination of two counters:
                                                         _ SYNC_COUNT<14:0> = cycle count in the range [0,30179].
                                                         _ SYNC_COUNT<19:16> = subframe count in the range [0,9].
                                                         _ SYNC_COUNT<15> is always 0.
                                                         The counter should indicate cycle 30179 in subframe 9 when the 10 ms
                                                         RFP pulse is received. A different counter value means the
                                                         syncronization was either early or late.  Software can interpret this
                                                         value as desired.  One possible interpretation is as follow:
                                                         _ 153600 <= x < 307199 = Early.
                                                         _ x > 307199 or 0 <= x < 153600 = Late.
                                                         Where x = 30720*SYNC_COUNT[19:16] + SYNC_COUNT[14:0]. */
#else
	uint64_t sync_count                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_sync_cnt_s     cnf75xx;
};
typedef union cvmx_rfif_timer_sync_cnt cvmx_rfif_timer_sync_cnt_t;

/**
 * cvmx_rfif_timer_sync_fail_cnt
 */
union cvmx_rfif_timer_sync_fail_cnt {
	uint64_t u64;
	struct cvmx_rfif_timer_sync_fail_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t fail_count                   : 32; /**< The number of times the count between two 10 ms scynchronization pulses was not 307200. */
#else
	uint64_t fail_count                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_timer_sync_fail_cnt_s cnf75xx;
};
typedef union cvmx_rfif_timer_sync_fail_cnt cvmx_rfif_timer_sync_fail_cnt_t;

/**
 * cvmx_rfif_timer_sync_ok
 */
union cvmx_rfif_timer_sync_ok {
	uint64_t u64;
	struct cvmx_rfif_timer_sync_ok_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t sync_status                  : 1;  /**< Reports if the exact number of clock cycles (307 200) were counted between the last two
                                                         10 ms synchronization pulses. */
#else
	uint64_t sync_status                  : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_timer_sync_ok_s      cnf75xx;
};
typedef union cvmx_rfif_timer_sync_ok cvmx_rfif_timer_sync_ok_t;

/**
 * cvmx_rfif_tofb_reset_ctrl
 *
 * This register controls resets for the JESD204B logic.
 *
 */
union cvmx_rfif_tofb_reset_ctrl {
	uint64_t u64;
	struct cvmx_rfif_tofb_reset_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_46_63               : 18;
	uint64_t lane5_reset                  : 6;  /**< See [LANE0_RESET] description. */
	uint64_t reserved_38_39               : 2;
	uint64_t lane4_reset                  : 6;  /**< See [LANE0_RESET] description. */
	uint64_t reserved_30_31               : 2;
	uint64_t lane3_reset                  : 6;  /**< See [LANE0_RESET] description. */
	uint64_t reserved_22_23               : 2;
	uint64_t lane2_reset                  : 6;  /**< See [LANE0_RESET] description. */
	uint64_t reserved_14_15               : 2;
	uint64_t lane1_reset                  : 6;  /**< See [LANE0_RESET] description. */
	uint64_t reserved_6_7                 : 2;
	uint64_t lane0_reset                  : 6;  /**< TOFB active low resets:
                                                         Bit 0 =  reset_rx_n.
                                                         Bit 1 = reset_tx_n.
                                                         Bit 2 = config_reset_n.
                                                         Bit 3 = cpu_reset_n.
                                                         Bit 4 = rx_map_reset_n.
                                                         Bit 5 = tx_map_reset_n. */
#else
	uint64_t lane0_reset                  : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t lane1_reset                  : 6;
	uint64_t reserved_14_15               : 2;
	uint64_t lane2_reset                  : 6;
	uint64_t reserved_22_23               : 2;
	uint64_t lane3_reset                  : 6;
	uint64_t reserved_30_31               : 2;
	uint64_t lane4_reset                  : 6;
	uint64_t reserved_38_39               : 2;
	uint64_t lane5_reset                  : 6;
	uint64_t reserved_46_63               : 18;
#endif
	} s;
	struct cvmx_rfif_tofb_reset_ctrl_s    cnf75xx;
};
typedef union cvmx_rfif_tofb_reset_ctrl cvmx_rfif_tofb_reset_ctrl_t;

/**
 * cvmx_rfif_tosp_output_ena
 */
union cvmx_rfif_tosp_output_ena {
	uint64_t u64;
	struct cvmx_rfif_tosp_output_ena_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ena                          : 1;  /**< Enable output for JESD 207. When TOSP is not in use, this bit should
                                                         be cleared to save power. */
#else
	uint64_t ena                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_tosp_output_ena_s    cnf75xx;
};
typedef union cvmx_rfif_tosp_output_ena cvmx_rfif_tosp_output_ena_t;

/**
 * cvmx_rfif_tosp_reset_ctrl
 *
 * This register controls resets for the TOSP (JESD207 interface) logic.
 *
 */
union cvmx_rfif_tosp_reset_ctrl {
	uint64_t u64;
	struct cvmx_rfif_tosp_reset_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t lane2_reset                  : 6;  /**< TOSP lane 2 (RFIF lane 4) resets. See LANE0_RESET description. */
	uint64_t reserved_14_15               : 2;
	uint64_t lane1_reset                  : 6;  /**< TOSP lane 1 (RFIF lane 2) resets. See LANE0_RESET description. */
	uint64_t reserved_6_7                 : 2;
	uint64_t lane0_reset                  : 6;  /**< TOSP active low resets:
                                                         Bit 0 = core_reset_n.
                                                         Bit 1 = reserved.
                                                         Bit 2 = reserved.
                                                         Bit 3 = cpu_reset_n.
                                                         Bit 4 = rx_map_reset_n.
                                                         Bit 5 = tx_map_reset_n. */
#else
	uint64_t lane0_reset                  : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t lane1_reset                  : 6;
	uint64_t reserved_14_15               : 2;
	uint64_t lane2_reset                  : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_rfif_tosp_reset_ctrl_s    cnf75xx;
};
typedef union cvmx_rfif_tosp_reset_ctrl cvmx_rfif_tosp_reset_ctrl_t;

/**
 * cvmx_rfif_ul_comb_config#
 *
 * These registers configure the uplink combining configuration for antennae (0..47).
 *
 */
union cvmx_rfif_ul_comb_configx {
	uint64_t u64;
	struct cvmx_rfif_ul_comb_configx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rfif_src_en                  : 6;  /**< Enable for each RFIF lane to be combined. Set bit N to include one AxC
                                                         from lane N in the sum/average for the antenna. Clear bit N to exclude
                                                         all AxC on lane N from the sum/average. */
	uint64_t reserved_56_57               : 2;
	uint64_t rfif_sum_avg                 : 1;  /**< Select between sum or average combining:
                                                         0 = Sum.
                                                         1 = Average. */
	uint64_t reserved_51_54               : 4;
	uint64_t rfif_axc_bw                  : 3;  /**< Bandwidth of the AxCs being combined:
                                                         0x0 = 1.4 MHz.
                                                         0x1 = 3.0 MHz.
                                                         0x2 = 5.0 MHz.
                                                         0x3 = 10 MHz.
                                                         0x4 = 15 MHz.
                                                         0x5 = 20 MHz. */
	uint64_t reserved_45_47               : 3;
	uint64_t lane5_axc_id                 : 5;  /**< Select the AxC from lane 5 used in the sum/average. The AxC is only
                                                         included if bit 5 in RFIF_SRC_EN is set. */
	uint64_t reserved_37_39               : 3;
	uint64_t lane4_axc_id                 : 5;  /**< Select the AxC from lane 4 used in the sum/average. The AxC is only
                                                         included if bit 4 in RFIF_SRC_EN is set. */
	uint64_t reserved_29_31               : 3;
	uint64_t lane3_axc_id                 : 5;  /**< Select the AxC from lane 3 used in the sum/average. The AxC is only
                                                         included if bit 3 in RFIF_SRC_EN is set. */
	uint64_t reserved_21_23               : 3;
	uint64_t lane2_axc_id                 : 5;  /**< Select the AxC from lane 2 used in the sum/average. The AxC is only
                                                         included if bit 2 in RFIF_SRC_EN is set. */
	uint64_t reserved_13_15               : 3;
	uint64_t lane1_axc_id                 : 5;  /**< Select the AxC from lane 1 used in the sum/average. The AxC is only
                                                         included if bit 1 in RFIF_SRC_EN is set. */
	uint64_t reserved_5_7                 : 3;
	uint64_t lane0_axc_id                 : 5;  /**< Select the AxC from lane 0 used in the sum/average. The AxC is only
                                                         included if bit 0 in RFIF_SRC_EN is set. */
#else
	uint64_t lane0_axc_id                 : 5;
	uint64_t reserved_5_7                 : 3;
	uint64_t lane1_axc_id                 : 5;
	uint64_t reserved_13_15               : 3;
	uint64_t lane2_axc_id                 : 5;
	uint64_t reserved_21_23               : 3;
	uint64_t lane3_axc_id                 : 5;
	uint64_t reserved_29_31               : 3;
	uint64_t lane4_axc_id                 : 5;
	uint64_t reserved_37_39               : 3;
	uint64_t lane5_axc_id                 : 5;
	uint64_t reserved_45_47               : 3;
	uint64_t rfif_axc_bw                  : 3;
	uint64_t reserved_51_54               : 4;
	uint64_t rfif_sum_avg                 : 1;
	uint64_t reserved_56_57               : 2;
	uint64_t rfif_src_en                  : 6;
#endif
	} s;
	struct cvmx_rfif_ul_comb_configx_s    cnf75xx;
};
typedef union cvmx_rfif_ul_comb_configx cvmx_rfif_ul_comb_configx_t;

/**
 * cvmx_rfif_ul_comb_config_lock
 */
union cvmx_rfif_ul_comb_config_lock {
	uint64_t u64;
	struct cvmx_rfif_ul_comb_config_lock_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t lock                         : 1;  /**< Lock bit. Set prior to configuring the uplink configuration, then cleared once the
                                                         configuration is completed. */
#else
	uint64_t lock                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_ul_comb_config_lock_s cnf75xx;
};
typedef union cvmx_rfif_ul_comb_config_lock cvmx_rfif_ul_comb_config_lock_t;

/**
 * cvmx_rfif_ul_comb_p0_config#
 *
 * These registers specify the AxC gain for lanes 0-3 antennae 0-47.
 *
 */
union cvmx_rfif_ul_comb_p0_configx {
	uint64_t u64;
	struct cvmx_rfif_ul_comb_p0_configx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_60_63               : 4;
	uint64_t gain_0                       : 12; /**< Gain applied to AxC samples from LANE 0 */
	uint64_t reserved_44_47               : 4;
	uint64_t gain_1                       : 12; /**< Gain applied to AxC samples from LANE 1 */
	uint64_t reserved_28_31               : 4;
	uint64_t gain_2                       : 12; /**< Gain applied to AxC samples from LANE 2 */
	uint64_t reserved_12_15               : 4;
	uint64_t gain_3                       : 12; /**< Gain applied to AxC samples from LANE 3 */
#else
	uint64_t gain_3                       : 12;
	uint64_t reserved_12_15               : 4;
	uint64_t gain_2                       : 12;
	uint64_t reserved_28_31               : 4;
	uint64_t gain_1                       : 12;
	uint64_t reserved_44_47               : 4;
	uint64_t gain_0                       : 12;
	uint64_t reserved_60_63               : 4;
#endif
	} s;
	struct cvmx_rfif_ul_comb_p0_configx_s cnf75xx;
};
typedef union cvmx_rfif_ul_comb_p0_configx cvmx_rfif_ul_comb_p0_configx_t;

/**
 * cvmx_rfif_ul_comb_p1_config#
 *
 * These registers specifiy additional uplink configuration parameters for antennae 0-47.
 *
 */
union cvmx_rfif_ul_comb_p1_configx {
	uint64_t u64;
	struct cvmx_rfif_ul_comb_p1_configx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_60_63               : 4;
	uint64_t gain_4                       : 12; /**< Gain applied to AxC samples from LANE 4 */
	uint64_t reserved_44_47               : 4;
	uint64_t gain_5                       : 12; /**< Gain applied to AxC samples from LANE 5 */
	uint64_t reserved_19_31               : 13;
	uint64_t shift_val                    : 3;  /**< Number of bits of right shift applied after the gain.
                                                         0x0 = 10 bits.
                                                         0x1 = 11 bits.
                                                         0x2 = 12 bits.
                                                         0x3 = 13 bits.
                                                         0x4 = 14 bits.
                                                         0x5 = 15 bits.
                                                         0x6 = Reserved.
                                                         0x7 = Reserved. */
	uint64_t reserved_4_15                : 12;
	uint64_t sat_bypass                   : 1;  /**< Bypass RX combiner saturation:
                                                         0 = Enable saturation.
                                                         1 = Bypass saturation and instead truncate the sum/average. */
	uint64_t sat_val                      : 3;  /**< Reserved. */
#else
	uint64_t sat_val                      : 3;
	uint64_t sat_bypass                   : 1;
	uint64_t reserved_4_15                : 12;
	uint64_t shift_val                    : 3;
	uint64_t reserved_19_31               : 13;
	uint64_t gain_5                       : 12;
	uint64_t reserved_44_47               : 4;
	uint64_t gain_4                       : 12;
	uint64_t reserved_60_63               : 4;
#endif
	} s;
	struct cvmx_rfif_ul_comb_p1_configx_s cnf75xx;
};
typedef union cvmx_rfif_ul_comb_p1_configx cvmx_rfif_ul_comb_p1_configx_t;

/**
 * cvmx_rfif_ul_copy
 */
union cvmx_rfif_ul_copy {
	uint64_t u64;
	struct cvmx_rfif_ul_copy_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t copy_enables                 : 24; /**< Enable bits for antenna copy. Set bit N to 1 to enable copying of antenna N to antenna
                                                         N+24.
                                                         Note that RFIF supports a maximum of 24 antennae at 20 MHz, or 48
                                                         antennae at <= 10 MHz, including all the duplicate antennae streams. */
#else
	uint64_t copy_enables                 : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_rfif_ul_copy_s            cnf75xx;
};
typedef union cvmx_rfif_ul_copy cvmx_rfif_ul_copy_t;

/**
 * cvmx_rfif_ul_pn_init#
 *
 * These registers specifiy the initial values used by the pseudo-random
 * sequence generators.
 *
 * Each of the 8 sequence generators uses 8 LFSRs to produce 8 x 16-bit
 * values at a time. In interation i, LFSR0 produces I(4i), LFSR1 produces
 * Q(4i), LFSR2 produces I(4i+1), ..., LFSR7 produces Q(4i+3).
 *
 * The initial value for LFSRx in PN generator y is specified by
 * RFIF_UL_PN_INIT(8y+x).
 *
 * The polynomial used is x(n+31) = x(n) + x(n+3).
 *
 * Each 16-bit I/Q sample is composed of 16 bits shifted from bit 0 of the
 * LFSR, starting at the most-significant bit of the sample.
 *
 * i.e., sample i can be determined as I(i)[15:0] = [x(16i),...,x(16i+15)].
 */
union cvmx_rfif_ul_pn_initx {
	uint64_t u64;
	struct cvmx_rfif_ul_pn_initx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t init_val                     : 32; /**< The LFSR initial value. */
#else
	uint64_t init_val                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_ul_pn_initx_s        cnf75xx;
};
typedef union cvmx_rfif_ul_pn_initx cvmx_rfif_ul_pn_initx_t;

/**
 * cvmx_rfif_ul_pn_map#
 *
 * These registers maps the eight pseudo-random sequence generators to
 * specific ULFE/PRCH antennae.
 */
union cvmx_rfif_ul_pn_mapx {
	uint64_t u64;
	struct cvmx_rfif_ul_pn_mapx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t ant_id                       : 6;  /**< The antenna ID that the polynomial sequence will be sent to. The valid
                                                         range is 0x0-0x2F. Set to 0x3F to disable the sequence generator. */
#else
	uint64_t ant_id                       : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_rfif_ul_pn_mapx_s         cnf75xx;
};
typedef union cvmx_rfif_ul_pn_mapx cvmx_rfif_ul_pn_mapx_t;

/**
 * cvmx_rfif_ul_pn_start
 */
union cvmx_rfif_ul_pn_start {
	uint64_t u64;
	struct cvmx_rfif_ul_pn_start_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< Start bit set to replace samples using PN sequence generated samples, starting the next sub-frame. */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rfif_ul_pn_start_s        cnf75xx;
};
typedef union cvmx_rfif_ul_pn_start cvmx_rfif_ul_pn_start_t;

/**
 * cvmx_rfif_ul_radio_axc_en#
 */
union cvmx_rfif_ul_radio_axc_enx {
	uint64_t u64;
	struct cvmx_rfif_ul_radio_axc_enx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t enable                       : 32; /**< Enable bit for each of 32 uplink AxCs for the lane. */
#else
	uint64_t enable                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rfif_ul_radio_axc_enx_s   cnf75xx;
};
typedef union cvmx_rfif_ul_radio_axc_enx cvmx_rfif_ul_radio_axc_enx_t;

/**
 * cvmx_rfif_ul_smple_adj
 *
 * This register designates the first RFIF uplink sample of a radio frame within the first basic
 * frame.
 */
union cvmx_rfif_ul_smple_adj {
	uint64_t u64;
	struct cvmx_rfif_ul_smple_adj_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t adj                          : 3;  /**< UL sample level adjustment register. This selects which sample within
                                                         the first BF serves as the first sample for the radio frame. The valid
                                                         range is 0 to 7. Note that the same adjustment is used for all
                                                         antennae.
                                                         The legal values depend on the BW of the lowest BW antenna as follows:
                                                         20 MHz Antennas only: 0 to 7.
                                                         20 MHz and 10 MHz: 0, 2, 4, 6.
                                                         20 MHz, 10 MHz and 5 MHz: 0, 4.
                                                         The sample selected for each antenna is scaled by the number of
                                                         samples per BF as follows:
                                                         _ For 20 MHz antennae the alignment is as programmed by ADJ.
                                                         _ For 10 MHz antennae it is ADJ>>1.
                                                         _ For  5 MHz antennae it is ADJ>>2. */
#else
	uint64_t adj                          : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_rfif_ul_smple_adj_s       cnf75xx;
};
typedef union cvmx_rfif_ul_smple_adj cvmx_rfif_ul_smple_adj_t;

/**
 * cvmx_rfif_ul_sync_config
 */
union cvmx_rfif_ul_sync_config {
	uint64_t u64;
	struct cvmx_rfif_ul_sync_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t stop_ulfe_sos                : 1;  /**< Controls whether RFIF sends an SOS_UL to ULFE when all lanes are down
                                                         due to LOS/LOF condition, or when the UL path is under reset.
                                                         0 = Always generate SOS_UL to ULFE.
                                                         1 = Stop sending SOS_UL to ULFE during reset or when all lanes are
                                                         down. */
	uint64_t sync_style                   : 1;  /**< Reserved. Must be 0. */
#else
	uint64_t sync_style                   : 1;
	uint64_t stop_ulfe_sos                : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_rfif_ul_sync_config_s     cnf75xx;
};
typedef union cvmx_rfif_ul_sync_config cvmx_rfif_ul_sync_config_t;

#endif
