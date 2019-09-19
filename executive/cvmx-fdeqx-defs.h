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
 * cvmx-fdeqx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon fdeqx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_FDEQX_DEFS_H__
#define __CVMX_FDEQX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_BIST_STATUS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_BIST_STATUS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300060ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_BIST_STATUS0(offset) (CVMX_ADD_IO_SEG(0x00011800B3300060ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300020ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800B3300020ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300000ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3300000ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ECC_CONTROL0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ECC_CONTROL0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300050ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ECC_CONTROL0(offset) (CVMX_ADD_IO_SEG(0x00011800B3300050ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ECC_STATUS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ECC_STATUS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300058ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ECC_STATUS0(offset) (CVMX_ADD_IO_SEG(0x00011800B3300058ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300008ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B3300008ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300040ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3300040ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ERROR_ENABLE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ERROR_ENABLE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300048ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ERROR_ENABLE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3300048ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300030ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3300030ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_ERROR_SOURCE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_ERROR_SOURCE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300038ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_ERROR_SOURCE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3300038ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3302088ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG0(offset) (CVMX_ADD_IO_SEG(0x00011800B3302088ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3302090ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG1(offset) (CVMX_ADD_IO_SEG(0x00011800B3302090ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3302098ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG2(offset) (CVMX_ADD_IO_SEG(0x00011800B3302098ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33020A0ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG3(offset) (CVMX_ADD_IO_SEG(0x00011800B33020A0ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33020A8ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG4(offset) (CVMX_ADD_IO_SEG(0x00011800B33020A8ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_CFG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD0_CFG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33020B0ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD0_CFG5(offset) (CVMX_ADD_IO_SEG(0x00011800B33020B0ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD0_MMSE_CFGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 16)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_FDEQX_JD0_MMSE_CFGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3302000ull) + (((offset) & 31) + ((block_id) & 1) * 0x40000ull) * 8;
}
#else
#define CVMX_FDEQX_JD0_MMSE_CFGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3302000ull) + (((offset) & 31) + ((block_id) & 1) * 0x40000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3304088ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG0(offset) (CVMX_ADD_IO_SEG(0x00011800B3304088ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3304090ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG1(offset) (CVMX_ADD_IO_SEG(0x00011800B3304090ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3304098ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG2(offset) (CVMX_ADD_IO_SEG(0x00011800B3304098ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33040A0ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG3(offset) (CVMX_ADD_IO_SEG(0x00011800B33040A0ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33040A8ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG4(offset) (CVMX_ADD_IO_SEG(0x00011800B33040A8ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_CFG5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_JD1_CFG5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B33040B0ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_JD1_CFG5(offset) (CVMX_ADD_IO_SEG(0x00011800B33040B0ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_JD1_MMSE_CFGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 16)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_FDEQX_JD1_MMSE_CFGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3304000ull) + (((offset) & 31) + ((block_id) & 1) * 0x40000ull) * 8;
}
#else
#define CVMX_FDEQX_JD1_MMSE_CFGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3304000ull) + (((offset) & 31) + ((block_id) & 1) * 0x40000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_PIPELINE_DISABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_PIPELINE_DISABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3301000ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_PIPELINE_DISABLE(offset) (CVMX_ADD_IO_SEG(0x00011800B3301000ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300018ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3300018ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_TEST(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_TEST(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300010ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_TEST(offset) (CVMX_ADD_IO_SEG(0x00011800B3300010ull) + ((offset) & 1) * 0x200000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_FDEQX_TEST2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_FDEQX_TEST2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3300028ull) + ((offset) & 1) * 0x200000ull;
}
#else
#define CVMX_FDEQX_TEST2(offset) (CVMX_ADD_IO_SEG(0x00011800B3300028ull) + ((offset) & 1) * 0x200000ull)
#endif

/**
 * cvmx_fdeq#_bist_status0
 *
 * This register indicates BIST status .Place holder for now
 *
 */
union cvmx_fdeqx_bist_status0 {
	uint64_t u64;
	struct cvmx_fdeqx_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t wrdma_bist_status            : 1;  /**< WRDMA Memory BIST Status */
	uint64_t dft_oif_bist_status          : 1;  /**< DFT IN Memory Bist Status */
	uint64_t dft_ping_bist_status         : 1;  /**< DFT IN Memory Bist Status */
	uint64_t dft_pong_bist_status         : 1;  /**< DFT OUT Memory BIST Status */
	uint64_t buf_bist_status              : 1;  /**< BUF Memory BIST Status */
#else
	uint64_t buf_bist_status              : 1;
	uint64_t dft_pong_bist_status         : 1;
	uint64_t dft_ping_bist_status         : 1;
	uint64_t dft_oif_bist_status          : 1;
	uint64_t wrdma_bist_status            : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_fdeqx_bist_status0_s      cnf75xx;
};
typedef union cvmx_fdeqx_bist_status0 cvmx_fdeqx_bist_status0_t;

/**
 * cvmx_fdeq#_config
 *
 * This Register is used for static configuration settings
 *
 */
union cvmx_fdeqx_config {
	uint64_t u64;
	struct cvmx_fdeqx_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t mmse_bypass                  : 1;  /**< When MMSE_BYPASS=0, the MHAB_RD DMA provides data to the MMSE Block.
                                                         When the MMSE_BYPASS=1, the MHAB_RD DMA provides data directly to the Engine pipeline */
#else
	uint64_t mmse_bypass                  : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_fdeqx_config_s            cnf75xx;
};
typedef union cvmx_fdeqx_config cvmx_fdeqx_config_t;

/**
 * cvmx_fdeq#_control
 *
 * This register is used to start RDEC HAB processing
 *
 */
union cvmx_fdeqx_control {
	uint64_t u64;
	struct cvmx_fdeqx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID1 */
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (FDEQ_STATUS bit 0 = '1')." */
	uint64_t start0                       : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (FDEQ_STATUS bit 0 = '1')." */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_fdeqx_control_s           cnf75xx;
};
typedef union cvmx_fdeqx_control cvmx_fdeqx_control_t;

/**
 * cvmx_fdeq#_ecc_control0
 *
 * This register controls ECC parameters.
 *
 */
union cvmx_fdeqx_ecc_control0 {
	uint64_t u64;
	struct cvmx_fdeqx_ecc_control0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t wrdma_flip_syn               : 2;  /**< WRDMA Memory Syndrome Flip Bits */
	uint64_t wrdma_cor_dis                : 1;  /**< WRDMA Memory ECC disable */
	uint64_t dft_oif_flip_syn             : 2;  /**< DFT IN Memory Syndrome Flip Bits */
	uint64_t dft_oif_cor_dis              : 1;  /**< DFT IN Memory ECC disable */
	uint64_t dft_ping_flip_syn            : 2;  /**< DFT IN Memory Syndrome Flip Bits */
	uint64_t dft_ping_cor_dis             : 1;  /**< DFT IN Memory ECC disable */
	uint64_t dft_pong_flip_syn            : 2;  /**< DFT OUT Memory Syndrome Flip Bits */
	uint64_t dft_pong_cor_dis             : 1;  /**< DFT OUT Memory ECC disable */
	uint64_t buf_flip_syn                 : 2;  /**< FDEQ BUF Memory Syndrome Flip Bits */
	uint64_t buf_cor_dis                  : 1;  /**< FDEQ BUF Memory ECC disable */
#else
	uint64_t buf_cor_dis                  : 1;
	uint64_t buf_flip_syn                 : 2;
	uint64_t dft_pong_cor_dis             : 1;
	uint64_t dft_pong_flip_syn            : 2;
	uint64_t dft_ping_cor_dis             : 1;
	uint64_t dft_ping_flip_syn            : 2;
	uint64_t dft_oif_cor_dis              : 1;
	uint64_t dft_oif_flip_syn             : 2;
	uint64_t wrdma_cor_dis                : 1;
	uint64_t wrdma_flip_syn               : 2;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_fdeqx_ecc_control0_s      cnf75xx;
};
typedef union cvmx_fdeqx_ecc_control0 cvmx_fdeqx_ecc_control0_t;

/**
 * cvmx_fdeq#_ecc_status0
 *
 * This register indicates ECC status.
 *
 */
union cvmx_fdeqx_ecc_status0 {
	uint64_t u64;
	struct cvmx_fdeqx_ecc_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t wrdma_dbe                    : 1;  /**< WRDMA Memory Double Bit Error */
	uint64_t wrdma_sbe                    : 1;  /**< WRDMA Memory Single Bit Error */
	uint64_t dft_oif_dbe                  : 1;  /**< DFT IN Memory Double Bit Error */
	uint64_t dft_oif_sbe                  : 1;  /**< DFT IN Memory Single Bit Error */
	uint64_t dft_ping_dbe                 : 1;  /**< DFT IN Memory Double Bit Error */
	uint64_t dft_ping_sbe                 : 1;  /**< DFT IN Memory Single Bit Error */
	uint64_t dft_pong_dbe                 : 1;  /**< DFT OUT Memory Double Bit Error */
	uint64_t dft_pong_sbe                 : 1;  /**< DFT OUT Memory Single Bit Error */
	uint64_t buf_dbe                      : 1;  /**< FDEQ BUF Memory Double Bit Error */
	uint64_t buf_sbe                      : 1;  /**< FDEQ BUF Memory Single Bit Error */
#else
	uint64_t buf_sbe                      : 1;
	uint64_t buf_dbe                      : 1;
	uint64_t dft_pong_sbe                 : 1;
	uint64_t dft_pong_dbe                 : 1;
	uint64_t dft_ping_sbe                 : 1;
	uint64_t dft_ping_dbe                 : 1;
	uint64_t dft_oif_sbe                  : 1;
	uint64_t dft_oif_dbe                  : 1;
	uint64_t wrdma_sbe                    : 1;
	uint64_t wrdma_dbe                    : 1;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_fdeqx_ecc_status0_s       cnf75xx;
};
typedef union cvmx_fdeqx_ecc_status0 cvmx_fdeqx_ecc_status0_t;

/**
 * cvmx_fdeq#_eco
 *
 * An ECO CSR
 *
 */
union cvmx_fdeqx_eco {
	uint64_t u64;
	struct cvmx_fdeqx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t eco_rw                       : 64; /**< Reserved for ECO usage. */
#else
	uint64_t eco_rw                       : 64;
#endif
	} s;
	struct cvmx_fdeqx_eco_s               cnf75xx;
};
typedef union cvmx_fdeqx_eco cvmx_fdeqx_eco_t;

/**
 * cvmx_fdeq#_error_enable0
 *
 * This register enables error reporting for FDEQ_ERROR_SOURCE0 register
 *
 */
union cvmx_fdeqx_error_enable0 {
	uint64_t u64;
	struct cvmx_fdeqx_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsv1                         : 58; /**< Reserved. */
	uint64_t rp1_of_en                    : 1;  /**< Error enable for Port1 Overflow. */
	uint64_t rp0_of_en                    : 1;  /**< Error enable for Port0 Overflow. */
	uint64_t rsv0                         : 2;  /**< Reserved. */
	uint64_t rp1_uf_en                    : 1;  /**< Error enable for Port1 Underflow. */
	uint64_t rp0_uf_en                    : 1;  /**< Error enable for Port0 Underflow. */
#else
	uint64_t rp0_uf_en                    : 1;
	uint64_t rp1_uf_en                    : 1;
	uint64_t rsv0                         : 2;
	uint64_t rp0_of_en                    : 1;
	uint64_t rp1_of_en                    : 1;
	uint64_t rsv1                         : 58;
#endif
	} s;
	struct cvmx_fdeqx_error_enable0_s     cnf75xx;
};
typedef union cvmx_fdeqx_error_enable0 cvmx_fdeqx_error_enable0_t;

/**
 * cvmx_fdeq#_error_enable1
 *
 * This register enables error reporting for FDEQ_ERROR_SOURCE1 register
 *
 */
union cvmx_fdeqx_error_enable1 {
	uint64_t u64;
	struct cvmx_fdeqx_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rsv                          : 63; /**< Reserved. */
	uint64_t mmse_err_en                  : 1;  /**< Error enable for FDEQ(0..1)_ERROR_SOURCE1 Register. */
#else
	uint64_t mmse_err_en                  : 1;
	uint64_t rsv                          : 63;
#endif
	} s;
	struct cvmx_fdeqx_error_enable1_s     cnf75xx;
};
typedef union cvmx_fdeqx_error_enable1 cvmx_fdeqx_error_enable1_t;

/**
 * cvmx_fdeq#_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_fdeqx_error_source0 {
	uint64_t u64;
	struct cvmx_fdeqx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t rp1_jobid_ufof               : 16; /**< Job ID for Read Port 1 Overflow/Underflow */
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID for Read Port 0 Overflow/Underflow */
	uint64_t reserved_6_15                : 10;
	uint64_t rp1_of                       : 1;  /**< Read Port1 overflow - Need to add H to access (same for other W1C) */
	uint64_t rp0_of                       : 1;  /**< Read Port0 overflow */
	uint64_t reserved_2_3                 : 2;
	uint64_t rp1_uf                       : 1;  /**< Read Port1 underflow */
	uint64_t rp0_uf                       : 1;  /**< Read Port0 underflow */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t rp1_uf                       : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t rp0_of                       : 1;
	uint64_t rp1_of                       : 1;
	uint64_t reserved_6_15                : 10;
	uint64_t rp0_jobid_ufof               : 16;
	uint64_t rp1_jobid_ufof               : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_fdeqx_error_source0_s     cnf75xx;
};
typedef union cvmx_fdeqx_error_source0 cvmx_fdeqx_error_source0_t;

/**
 * cvmx_fdeq#_error_source1
 *
 * This register contains error source information.
 *
 */
union cvmx_fdeqx_error_source1 {
	uint64_t u64;
	struct cvmx_fdeqx_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_58_63               : 6;
	uint64_t subdemap_fifo_err            : 16; /**< Sub DEMAP Fifo Error */
	uint64_t regen_fifo_err               : 8;  /**< REGEN Fifo Error */
	uint64_t fde_fifo_err                 : 2;  /**< FDE Fifo Error */
	uint64_t rp1_jobid_err                : 16; /**< MMSE Job ID for Error */
	uint64_t rp0_jobid_err                : 16; /**< MMSE Job ID for Error */
#else
	uint64_t rp0_jobid_err                : 16;
	uint64_t rp1_jobid_err                : 16;
	uint64_t fde_fifo_err                 : 2;
	uint64_t regen_fifo_err               : 8;
	uint64_t subdemap_fifo_err            : 16;
	uint64_t reserved_58_63               : 6;
#endif
	} s;
	struct cvmx_fdeqx_error_source1_s     cnf75xx;
};
typedef union cvmx_fdeqx_error_source1 cvmx_fdeqx_error_source1_t;

/**
 * cvmx_fdeq#_jd0_cfg0
 *
 * This register space contains the FDEQ job descriptor data.
 *
 */
union cvmx_fdeqx_jd0_cfg0 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t mmse_idft_bypass             : 1;  /**< MMSE bypass flag
                                                         If set to 1, MMSE and the IDFT are bypassed */
	uint64_t mmse_bypass                  : 1;  /**< MMSE bypass flag
                                                         If set to 1, MMSE is bypassed */
	uint64_t idft_bypass                  : 1;  /**< IDFT bypass flag
                                                         If set to 1, IDFT and all subsequent modules are bypassed */
	uint64_t demapper_bypass              : 1;  /**< Demapper bypass flag
                                                         If set to 1, demapper and all subsequent modules are bypassed */
	uint64_t descrambler_bypass           : 1;  /**< Descrambler bypass flag
                                                         If set to 1, descrambler and all subsequent modules are bypassed */
	uint64_t ack_scomb_bypass             : 1;  /**< ACK soft combining bypass flag
                                                         If set to 1, ACK soft combining is bypassed */
	uint64_t ri_scomb_bypass              : 1;  /**< RI soft combining bypass flag
                                                         If set to 1, RI soft combining is bypassed */
	uint64_t cqi_scomb_bypass             : 1;  /**< CQI soft combining bypass flag
                                                         If set to 1, it indicates CQI is not RM encoded and will be bypassed */
	uint64_t ack_extract_bypass           : 1;  /**< ACK extraction bypass flag
                                                         If set  to 1, bypass
                                                         If set to 0, ACK extraction takes place in addition to soft combining */
	uint64_t ri_extract_bypass            : 1;  /**< RI extraction bypass flag
                                                         If set  to 1, bypass
                                                         If set to 0, RI extraction takes place in addition to soft combining */
	uint64_t report_bypass                : 1;  /**< FDEQ Report bypass flag
                                                         If set to 1, FDEQ Report is not generated */
	uint64_t reserved_33_35               : 3;
	uint64_t cp_type                      : 1;  /**< CP Type
                                                         '0': Normal CP
                                                         '1': Extended CP */
	uint64_t reserved_29_31               : 3;
	uint64_t layer_id                     : 1;  /**< Layer identifier
                                                         '0': layer 0
                                                         '1': layer 1
                                                         layer_id =0 if num_layers=1. */
	uint64_t reserved_26_27               : 2;
	uint64_t num_layers                   : 2;  /**< Number of layers
                                                         '1': 1 layer
                                                         '2': 2 layers */
	uint64_t reserved_23_23               : 1;
	uint64_t mod_order                    : 3;  /**< Modulation order
                                                         '2': QPSK
                                                         '4': 16QAM
                                                         '6': 64QAM */
	uint64_t reserved_18_19               : 2;
	uint64_t slot_id                      : 2;  /**< Slot Identifier
                                                         '0': slot 0
                                                         '1': slot 1
                                                         '2': subframe */
	uint64_t reserved_9_15                : 7;
	uint64_t srs_flag                     : 1;  /**< SRS flag
                                                         If set to 1, the last symbol of the input is SRS sybmol */
	uint64_t reserved_6_7                 : 2;
	uint64_t ndft_indx                    : 6;  /**< Indices for the NDFTs defined in the list of supported NDFTs as given in Table 8. */
#else
	uint64_t ndft_indx                    : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t srs_flag                     : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t slot_id                      : 2;
	uint64_t reserved_18_19               : 2;
	uint64_t mod_order                    : 3;
	uint64_t reserved_23_23               : 1;
	uint64_t num_layers                   : 2;
	uint64_t reserved_26_27               : 2;
	uint64_t layer_id                     : 1;
	uint64_t reserved_29_31               : 3;
	uint64_t cp_type                      : 1;
	uint64_t reserved_33_35               : 3;
	uint64_t report_bypass                : 1;
	uint64_t ri_extract_bypass            : 1;
	uint64_t ack_extract_bypass           : 1;
	uint64_t cqi_scomb_bypass             : 1;
	uint64_t ri_scomb_bypass              : 1;
	uint64_t ack_scomb_bypass             : 1;
	uint64_t descrambler_bypass           : 1;
	uint64_t demapper_bypass              : 1;
	uint64_t idft_bypass                  : 1;
	uint64_t mmse_bypass                  : 1;
	uint64_t mmse_idft_bypass             : 1;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg0_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg0 cvmx_fdeqx_jd0_cfg0_t;

/**
 * cvmx_fdeq#_jd0_cfg1
 *
 * This register space contains the FDEQ job descriptor data for DFT DMP
 *
 */
union cvmx_fdeqx_jd0_cfg1 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_53_63               : 11;
	uint64_t rs_bits_last                 : 5;  /**< Configurable parameter to control the output scaling at the last stage of DFT/IDFT.
                                                         This has to be set as 15 to make overall DFT gain as unity but it is a configurable
                                                         parameter for actual implementation to allow flexibility */
	uint64_t reserved_45_47               : 3;
	uint64_t ack_ri_dmp_treatment         : 1;  /**< 1-bit and 2-bit ACK/RI special treatment flag
                                                         If set to 1, demapper treats 1-bit and 2-bit ACK/RI REs in a special way considering
                                                         only outermost corner constellation points */
	uint64_t q_format                     : 4;  /**< Fixed point Q format for the demapper as given in Table 10. */
	uint64_t scaled_sinr_rsft             : 4;  /**< Right shift factor after the multiplication [SCALE * SINR]. See RS1 in Figure 2 10. */
	uint64_t reserved_34_35               : 2;
	uint64_t mod_rsft                     : 6;  /**< Modulation dependent right shift factor. See RS2 in Figure 2 10. */
	uint64_t llr_bit_width                : 4;  /**< LLR bitwidht */
	uint64_t reserved_23_23               : 1;
	uint64_t llr_offset_input             : 7;  /**< LLR offset value ranging from 0 to 2^(llr_bit_width-1)-1 */
	uint64_t mod_scale                    : 16; /**< Modulation dependent scale value. See SCALE in Figure 2 10. */
#else
	uint64_t mod_scale                    : 16;
	uint64_t llr_offset_input             : 7;
	uint64_t reserved_23_23               : 1;
	uint64_t llr_bit_width                : 4;
	uint64_t mod_rsft                     : 6;
	uint64_t reserved_34_35               : 2;
	uint64_t scaled_sinr_rsft             : 4;
	uint64_t q_format                     : 4;
	uint64_t ack_ri_dmp_treatment         : 1;
	uint64_t reserved_45_47               : 3;
	uint64_t rs_bits_last                 : 5;
	uint64_t reserved_53_63               : 11;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg1_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg1 cvmx_fdeqx_jd0_cfg1_t;

/**
 * cvmx_fdeq#_jd0_cfg2
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd0_cfg2 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t cinit                        : 31; /**< N/A */
	uint64_t reserved_17_31               : 15;
	uint64_t seq_offset                   : 17; /**< Skip offset for the scrambling sequence.
                                                         A part of the scrambling sequence indicated by seq_offset is skipped before descrambling */
#else
	uint64_t seq_offset                   : 17;
	uint64_t reserved_17_31               : 15;
	uint64_t cinit                        : 31;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg2_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg2 cvmx_fdeqx_jd0_cfg2_t;

/**
 * cvmx_fdeq#_jd0_cfg3
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd0_cfg3 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t ack_enc_cat                  : 3;  /**< ACK encoding category
                                                         '0': no ACK
                                                         '1': OACK = 1
                                                         '2': OACK = 1 ACK bundling
                                                         '3': OACK = 2
                                                         '4': OACK = 2 ACK bundling
                                                         '5': 3 ? OACK ? 11
                                                         '6': 11 < OACK ? 20
                                                         '7': reserved */
	uint64_t reserved_35_47               : 13;
	uint64_t ri_enc_cat                   : 3;  /**< RI encoding category
                                                         '0': no RI
                                                         '1': ORI = 1
                                                         '2': ORI = 2
                                                         '3': 3 ? ORI ? 11
                                                         '4': 11 < ORI ? 15
                                                         '5'-'7': -- */
	uint64_t reserved_29_31               : 3;
	uint64_t num_ack_re                   : 13; /**< Number of ACK REs per layer */
	uint64_t reserved_13_15               : 3;
	uint64_t num_ri_re                    : 13; /**< Number of RI REs per layer */
#else
	uint64_t num_ri_re                    : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t num_ack_re                   : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t ri_enc_cat                   : 3;
	uint64_t reserved_35_47               : 13;
	uint64_t ack_enc_cat                  : 3;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg3_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg3 cvmx_fdeqx_jd0_cfg3_t;

/**
 * cvmx_fdeq#_jd0_cfg4
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd0_cfg4 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re0                  : 14; /**< Number of CQI REs per layer for hypothesis 0 */
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re1                  : 14; /**< Number of CQI REs per layer for hypothesis 1 */
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re2                  : 14; /**< Number of CQI REs per layer for hypothesis 2 */
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re3                  : 14; /**< Number of CQI REs per layer for hypothesis 3 */
#else
	uint64_t num_cqi_re3                  : 14;
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re2                  : 14;
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re1                  : 14;
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re0                  : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg4_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg4 cvmx_fdeqx_jd0_cfg4_t;

/**
 * cvmx_fdeq#_jd0_cfg5
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd0_cfg5 {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_cfg5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re4                  : 14; /**< Number of CQI REs per layer for hypothesis 4 */
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re5                  : 14; /**< Number of CQI REs per layer for hypothesis 5 */
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re6                  : 14; /**< Number of CQI REs per layer for hypothesis 6 */
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re7                  : 14; /**< Number of CQI REs per layer for hypothesis 7 */
#else
	uint64_t num_cqi_re7                  : 14;
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re6                  : 14;
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re5                  : 14;
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re4                  : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_fdeqx_jd0_cfg5_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd0_cfg5 cvmx_fdeqx_jd0_cfg5_t;

/**
 * cvmx_fdeq#_jd0_mmse_cfg#
 *
 * This register space contains the MMSE job descriptor data.
 *
 */
union cvmx_fdeqx_jd0_mmse_cfgx {
	uint64_t u64;
	struct cvmx_fdeqx_jd0_mmse_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mmse_job                     : 64; /**< MMSE Job descriptors */
#else
	uint64_t mmse_job                     : 64;
#endif
	} s;
	struct cvmx_fdeqx_jd0_mmse_cfgx_s     cnf75xx;
};
typedef union cvmx_fdeqx_jd0_mmse_cfgx cvmx_fdeqx_jd0_mmse_cfgx_t;

/**
 * cvmx_fdeq#_jd1_cfg0
 *
 * This register space contains the FDEQ job descriptor data.
 *
 */
union cvmx_fdeqx_jd1_cfg0 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t mmse_idft_bypass             : 1;  /**< MMSE bypass flag
                                                         If set to 1, MMSE and the IDFT are bypassed */
	uint64_t mmse_bypass                  : 1;  /**< MMSE bypass flag
                                                         If set to 1, MMSE is bypassed */
	uint64_t idft_bypass                  : 1;  /**< IDFT bypass flag
                                                         If set to 1, IDFT and all subsequent modules are bypassed */
	uint64_t demapper_bypass              : 1;  /**< Demapper bypass flag
                                                         If set to 1, demapper and all subsequent modules are bypassed */
	uint64_t descrambler_bypass           : 1;  /**< Descrambler bypass flag
                                                         If set to 1, descrambler and all subsequent modules are bypassed */
	uint64_t ack_scomb_bypass             : 1;  /**< ACK soft combining bypass flag
                                                         If set to 1, ACK soft combining is bypassed */
	uint64_t ri_scomb_bypass              : 1;  /**< RI soft combining bypass flag
                                                         If set to 1, RI soft combining is bypassed */
	uint64_t cqi_scomb_bypass             : 1;  /**< CQI soft combining bypass flag
                                                         If set to 1, it indicates CQI is not RM encoded and will be bypassed */
	uint64_t ack_extract_bypass           : 1;  /**< ACK extraction bypass flag
                                                         If set  to 1, bypass
                                                         If set to 0, ACK extraction takes place in addition to soft combining */
	uint64_t ri_extract_bypass            : 1;  /**< RI extraction bypass flag
                                                         If set  to 1, bypass
                                                         If set to 0, RI extraction takes place in addition to soft combining */
	uint64_t report_bypass                : 1;  /**< FDEQ Report bypass flag
                                                         If set to 1, FDEQ Report is not generated */
	uint64_t reserved_33_35               : 3;
	uint64_t cp_type                      : 1;  /**< CP Type
                                                         '0': Normal CP
                                                         '1': Extended CP */
	uint64_t reserved_29_31               : 3;
	uint64_t layer_id                     : 1;  /**< Layer identifier
                                                         '0': layer 0
                                                         '1': layer 1
                                                         layer_id =0 if num_layers=1. */
	uint64_t reserved_26_27               : 2;
	uint64_t num_layers                   : 2;  /**< Number of layers
                                                         '1': 1 layer
                                                         '2': 2 layers */
	uint64_t reserved_23_23               : 1;
	uint64_t mod_order                    : 3;  /**< Modulation order
                                                         '2': QPSK
                                                         '4': 16QAM
                                                         '6': 64QAM */
	uint64_t reserved_18_19               : 2;
	uint64_t slot_id                      : 2;  /**< Slot Identifier
                                                         '0': slot 0
                                                         '1': slot 1
                                                         '2': subframe */
	uint64_t reserved_9_15                : 7;
	uint64_t srs_flag                     : 1;  /**< SRS flag
                                                         If set to 1, the last symbol of the input is SRS sybmol */
	uint64_t reserved_6_7                 : 2;
	uint64_t ndft_indx                    : 6;  /**< Indices for the NDFTs defined in the list of supported NDFTs as given in Table 8. */
#else
	uint64_t ndft_indx                    : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t srs_flag                     : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t slot_id                      : 2;
	uint64_t reserved_18_19               : 2;
	uint64_t mod_order                    : 3;
	uint64_t reserved_23_23               : 1;
	uint64_t num_layers                   : 2;
	uint64_t reserved_26_27               : 2;
	uint64_t layer_id                     : 1;
	uint64_t reserved_29_31               : 3;
	uint64_t cp_type                      : 1;
	uint64_t reserved_33_35               : 3;
	uint64_t report_bypass                : 1;
	uint64_t ri_extract_bypass            : 1;
	uint64_t ack_extract_bypass           : 1;
	uint64_t cqi_scomb_bypass             : 1;
	uint64_t ri_scomb_bypass              : 1;
	uint64_t ack_scomb_bypass             : 1;
	uint64_t descrambler_bypass           : 1;
	uint64_t demapper_bypass              : 1;
	uint64_t idft_bypass                  : 1;
	uint64_t mmse_bypass                  : 1;
	uint64_t mmse_idft_bypass             : 1;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg0_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg0 cvmx_fdeqx_jd1_cfg0_t;

/**
 * cvmx_fdeq#_jd1_cfg1
 *
 * This register space contains the FDEQ job descriptor data for DFT DMP
 *
 */
union cvmx_fdeqx_jd1_cfg1 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_53_63               : 11;
	uint64_t rs_bits_last                 : 5;  /**< Configurable parameter to control the output scaling at the last stage of DFT/IDFT.
                                                         This has to be set as 15 to make overall DFT gain as unity but it is a configurable
                                                         parameter for actual implementation to allow flexibility */
	uint64_t reserved_45_47               : 3;
	uint64_t ack_ri_dmp_treatment         : 1;  /**< 1-bit and 2-bit ACK/RI special treatment flag
                                                         If set to 1, demapper treats 1-bit and 2-bit ACK/RI REs in a special way considering
                                                         only outermost corner constellation points */
	uint64_t q_format                     : 4;  /**< Fixed point Q format for the demapper as given in Table 10. */
	uint64_t scaled_sinr_rsft             : 4;  /**< Right shift factor after the multiplication [SCALE * SINR]. See RS1 in Figure 2 10. */
	uint64_t reserved_34_35               : 2;
	uint64_t mod_rsft                     : 6;  /**< Modulation dependent right shift factor. See RS2 in Figure 2 10. */
	uint64_t llr_bit_width                : 4;  /**< LLR bitwidht */
	uint64_t reserved_23_23               : 1;
	uint64_t llr_offset_input             : 7;  /**< LLR offset value ranging from 0 to 2^(llr_bit_width-1)-1 */
	uint64_t mod_scale                    : 16; /**< Modulation dependent scale value. See SCALE in Figure 2 10. */
#else
	uint64_t mod_scale                    : 16;
	uint64_t llr_offset_input             : 7;
	uint64_t reserved_23_23               : 1;
	uint64_t llr_bit_width                : 4;
	uint64_t mod_rsft                     : 6;
	uint64_t reserved_34_35               : 2;
	uint64_t scaled_sinr_rsft             : 4;
	uint64_t q_format                     : 4;
	uint64_t ack_ri_dmp_treatment         : 1;
	uint64_t reserved_45_47               : 3;
	uint64_t rs_bits_last                 : 5;
	uint64_t reserved_53_63               : 11;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg1_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg1 cvmx_fdeqx_jd1_cfg1_t;

/**
 * cvmx_fdeq#_jd1_cfg2
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd1_cfg2 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_63               : 1;
	uint64_t cinit                        : 31; /**< N/A */
	uint64_t reserved_17_31               : 15;
	uint64_t seq_offset                   : 17; /**< Skip offset for the scrambling sequence.
                                                         A part of the scrambling sequence indicated by seq_offset is skipped before descrambling */
#else
	uint64_t seq_offset                   : 17;
	uint64_t reserved_17_31               : 15;
	uint64_t cinit                        : 31;
	uint64_t reserved_63_63               : 1;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg2_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg2 cvmx_fdeqx_jd1_cfg2_t;

/**
 * cvmx_fdeq#_jd1_cfg3
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd1_cfg3 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t ack_enc_cat                  : 3;  /**< ACK encoding category
                                                         '0': no ACK
                                                         '1': OACK = 1
                                                         '2': OACK = 1 ACK bundling
                                                         '3': OACK = 2
                                                         '4': OACK = 2 ACK bundling
                                                         '5': 3 ? OACK ? 11
                                                         '6': 11 < OACK ? 20
                                                         '7': reserved */
	uint64_t reserved_35_47               : 13;
	uint64_t ri_enc_cat                   : 3;  /**< RI encoding category
                                                         '0': no RI
                                                         '1': ORI = 1
                                                         '2': ORI = 2
                                                         '3': 3 ? ORI ? 11
                                                         '4': 11 < ORI ? 15
                                                         '5'-'7': -- */
	uint64_t reserved_29_31               : 3;
	uint64_t num_ack_re                   : 13; /**< Number of ACK REs per layer */
	uint64_t reserved_13_15               : 3;
	uint64_t num_ri_re                    : 13; /**< Number of RI REs per layer */
#else
	uint64_t num_ri_re                    : 13;
	uint64_t reserved_13_15               : 3;
	uint64_t num_ack_re                   : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t ri_enc_cat                   : 3;
	uint64_t reserved_35_47               : 13;
	uint64_t ack_enc_cat                  : 3;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg3_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg3 cvmx_fdeqx_jd1_cfg3_t;

/**
 * cvmx_fdeq#_jd1_cfg4
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd1_cfg4 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re0                  : 14; /**< Number of CQI REs per layer for hypothesis 0 */
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re1                  : 14; /**< Number of CQI REs per layer for hypothesis 1 */
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re2                  : 14; /**< Number of CQI REs per layer for hypothesis 2 */
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re3                  : 14; /**< Number of CQI REs per layer for hypothesis 3 */
#else
	uint64_t num_cqi_re3                  : 14;
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re2                  : 14;
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re1                  : 14;
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re0                  : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg4_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg4 cvmx_fdeqx_jd1_cfg4_t;

/**
 * cvmx_fdeq#_jd1_cfg5
 *
 * This register space contains the FDEQ job descriptor data
 *
 */
union cvmx_fdeqx_jd1_cfg5 {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_cfg5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re4                  : 14; /**< Number of CQI REs per layer for hypothesis 4 */
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re5                  : 14; /**< Number of CQI REs per layer for hypothesis 5 */
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re6                  : 14; /**< Number of CQI REs per layer for hypothesis 6 */
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re7                  : 14; /**< Number of CQI REs per layer for hypothesis 7 */
#else
	uint64_t num_cqi_re7                  : 14;
	uint64_t reserved_14_15               : 2;
	uint64_t num_cqi_re6                  : 14;
	uint64_t reserved_30_31               : 2;
	uint64_t num_cqi_re5                  : 14;
	uint64_t reserved_46_47               : 2;
	uint64_t num_cqi_re4                  : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_fdeqx_jd1_cfg5_s          cnf75xx;
};
typedef union cvmx_fdeqx_jd1_cfg5 cvmx_fdeqx_jd1_cfg5_t;

/**
 * cvmx_fdeq#_jd1_mmse_cfg#
 *
 * This register space contains the MMSE job descriptor data.
 *
 */
union cvmx_fdeqx_jd1_mmse_cfgx {
	uint64_t u64;
	struct cvmx_fdeqx_jd1_mmse_cfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t mmse_job                     : 64; /**< MMSE Job descriptors */
#else
	uint64_t mmse_job                     : 64;
#endif
	} s;
	struct cvmx_fdeqx_jd1_mmse_cfgx_s     cnf75xx;
};
typedef union cvmx_fdeqx_jd1_mmse_cfgx cvmx_fdeqx_jd1_mmse_cfgx_t;

/**
 * cvmx_fdeq#_pipeline_disable
 *
 * This register is used to disable pipelining in the MMSE
 *
 */
union cvmx_fdeqx_pipeline_disable {
	uint64_t u64;
	struct cvmx_fdeqx_pipeline_disable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t dis_pipeline                 : 1;  /**< '1' = Disables the pipelining of Jobs in the MMSE */
#else
	uint64_t dis_pipeline                 : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_fdeqx_pipeline_disable_s  cnf75xx;
};
typedef union cvmx_fdeqx_pipeline_disable cvmx_fdeqx_pipeline_disable_t;

/**
 * cvmx_fdeq#_status
 *
 * FDEQ Status Register
 *
 */
union cvmx_fdeqx_status {
	uint64_t u64;
	struct cvmx_fdeqx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t status1                      : 1;  /**< Indicates if the FDEQ is busy processing a job with CONFIGURATION 1. '0' = Ready for new
                                                         job, '1' = Busy processing. */
	uint64_t status0                      : 1;  /**< Indicates if the FDEQ is busy processing a job with CONFIGURATION 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t status1                      : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_fdeqx_status_s            cnf75xx;
};
typedef union cvmx_fdeqx_status cvmx_fdeqx_status_t;

/**
 * cvmx_fdeq#_test
 *
 * This Register is used to stress internal Hardware backpressure points for Testing
 *
 */
union cvmx_fdeqx_test {
	uint64_t u64;
	struct cvmx_fdeqx_test_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t enable                       : 4;  /**< Enable test mode. For diagnostic use only. */
	uint64_t reserved_24_59               : 36;
	uint64_t bp_cfg                       : 8;  /**< Enable backpressure. For diagnostic use only. */
	uint64_t reserved_12_15               : 4;
	uint64_t lfsr_freq                    : 12; /**< Test LFSR update frequency in coprocessor-clocks minus one. */
#else
	uint64_t lfsr_freq                    : 12;
	uint64_t reserved_12_15               : 4;
	uint64_t bp_cfg                       : 8;
	uint64_t reserved_24_59               : 36;
	uint64_t enable                       : 4;
#endif
	} s;
	struct cvmx_fdeqx_test_s              cnf75xx;
};
typedef union cvmx_fdeqx_test cvmx_fdeqx_test_t;

/**
 * cvmx_fdeq#_test2
 *
 * This Register is used to stress internal Hardware backpressure points for Testing
 *
 */
union cvmx_fdeqx_test2 {
	uint64_t u64;
	struct cvmx_fdeqx_test2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t enable                       : 4;  /**< Enable test mode. For diagnostic use only. */
	uint64_t reserved_24_59               : 36;
	uint64_t bp_cfg                       : 8;  /**< Enable backpressure. For diagnostic use only. */
	uint64_t reserved_12_15               : 4;
	uint64_t lfsr_freq                    : 12; /**< Test LFSR update frequency in coprocessor-clocks minus one. */
#else
	uint64_t lfsr_freq                    : 12;
	uint64_t reserved_12_15               : 4;
	uint64_t bp_cfg                       : 8;
	uint64_t reserved_24_59               : 36;
	uint64_t enable                       : 4;
#endif
	} s;
	struct cvmx_fdeqx_test2_s             cnf75xx;
};
typedef union cvmx_fdeqx_test2 cvmx_fdeqx_test2_t;

#endif
