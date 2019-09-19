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
 * cvmx-tdecx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon tdecx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_TDECX_DEFS_H__
#define __CVMX_TDECX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_BIST_STATUS0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_BIST_STATUS0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600060ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_BIST_STATUS0(offset) (CVMX_ADD_IO_SEG(0x00011800B3600060ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_BIST_STATUS1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_BIST_STATUS1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600068ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_BIST_STATUS1(offset) (CVMX_ADD_IO_SEG(0x00011800B3600068ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3600000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ECC_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ECC_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600050ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ECC_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3600050ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B3600008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3600040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ERROR_ENABLE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ERROR_ENABLE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600048ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ERROR_ENABLE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3600048ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3600030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_ERROR_SOURCE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_ERROR_SOURCE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_ERROR_SOURCE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3600038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_HAB_JCFG0_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_TDECX_HAB_JCFG0_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3602000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_TDECX_HAB_JCFG0_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3602000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_HAB_JCFG1_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_TDECX_HAB_JCFG1_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3604000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_TDECX_HAB_JCFG1_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3604000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_HAB_JCFG2_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_TDECX_HAB_JCFG2_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3606000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_TDECX_HAB_JCFG2_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3606000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_JCFG0_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_JCFG0_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600100ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_JCFG0_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3600100ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_JCFG1_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_JCFG1_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600108ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_JCFG1_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3600108ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_JCFG2_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_JCFG2_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600110ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_JCFG2_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3600110ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_SCRATCH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_SCRATCH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600080ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_SCRATCH(offset) (CVMX_ADD_IO_SEG(0x00011800B3600080ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3600018ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3600018ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_CONFIG_ERR_FLAGS_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_CONFIG_ERR_FLAGS_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_CONFIG_ERR_FLAGS_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_CONFIG_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 12)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_TDECX_TC_CONFIG_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3601400ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_TDECX_TC_CONFIG_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3601400ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_CONTROL_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_CONTROL_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601010ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_CONTROL_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601010ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_ERROR_MASK_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_ERROR_MASK_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_ERROR_MASK_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_ERROR_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_ERROR_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_ERROR_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_MAIN_RESET_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_MAIN_RESET_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_MAIN_RESET_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_MAIN_START_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_MAIN_START_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_MAIN_START_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_MON_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_TDECX_TC_MON_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3601300ull) + (((offset) & 3) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_TDECX_TC_MON_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3601300ull) + (((offset) & 3) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_STATUS0_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_STATUS0_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601020ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_STATUS0_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601020ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TDECX_TC_STATUS1_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_TDECX_TC_STATUS1_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3601028ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_TDECX_TC_STATUS1_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3601028ull) + ((offset) & 1) * 32768)
#endif

/**
 * cvmx_tdec#_bist_status0
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_tdecx_bist_status0 {
	uint64_t u64;
	struct cvmx_tdecx_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t bisr_status                  : 64; /**< BIST status results. */
#else
	uint64_t bisr_status                  : 64;
#endif
	} s;
	struct cvmx_tdecx_bist_status0_s      cnf75xx;
};
typedef union cvmx_tdecx_bist_status0 cvmx_tdecx_bist_status0_t;

/**
 * cvmx_tdec#_bist_status1
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_tdecx_bist_status1 {
	uint64_t u64;
	struct cvmx_tdecx_bist_status1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t bisr_status                  : 22; /**< BIST status results. */
#else
	uint64_t bisr_status                  : 22;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_tdecx_bist_status1_s      cnf75xx;
};
typedef union cvmx_tdecx_bist_status1 cvmx_tdecx_bist_status1_t;

/**
 * cvmx_tdec#_control
 */
union cvmx_tdecx_control {
	uint64_t u64;
	struct cvmx_tdecx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t jobid2                       : 16; /**< Job ID for slot 2. */
	uint64_t jobid1                       : 16; /**< Job ID for slot 1. */
	uint64_t jobid0                       : 16; /**< Job ID for slot 0. */
	uint64_t reserved_3_15                : 13;
	uint64_t start2                       : 1;  /**< Start processing job in slot 2. */
	uint64_t start1                       : 1;  /**< Start processing job in slot 1. */
	uint64_t start0                       : 1;  /**< Start processing job in slot 0. */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t start2                       : 1;
	uint64_t reserved_3_15                : 13;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t jobid2                       : 16;
#endif
	} s;
	struct cvmx_tdecx_control_s           cnf75xx;
};
typedef union cvmx_tdecx_control cvmx_tdecx_control_t;

/**
 * cvmx_tdec#_ecc_control
 */
union cvmx_tdecx_ecc_control {
	uint64_t u64;
	struct cvmx_tdecx_ecc_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_35_63               : 29;
	uint64_t flip_bit2                    : 3;  /**< This bit flips a second bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_19_31               : 13;
	uint64_t flip_bit1                    : 3;  /**< This bit flips the first bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_3_15                : 13;
	uint64_t jcfg2_cor_dis                : 1;  /**< This bit disables SBE correction of the job configuration 2 RAMs. */
	uint64_t jcfg1_cor_dis                : 1;  /**< This bit disables SBE correction of the job configuration 1 RAMs. */
	uint64_t jcfg0_cor_dis                : 1;  /**< This bit disables SBE correction of the job configuration 0 RAMs. */
#else
	uint64_t jcfg0_cor_dis                : 1;
	uint64_t jcfg1_cor_dis                : 1;
	uint64_t jcfg2_cor_dis                : 1;
	uint64_t reserved_3_15                : 13;
	uint64_t flip_bit1                    : 3;
	uint64_t reserved_19_31               : 13;
	uint64_t flip_bit2                    : 3;
	uint64_t reserved_35_63               : 29;
#endif
	} s;
	struct cvmx_tdecx_ecc_control_s       cnf75xx;
};
typedef union cvmx_tdecx_ecc_control cvmx_tdecx_ecc_control_t;

/**
 * cvmx_tdec#_eco
 */
union cvmx_tdecx_eco {
	uint64_t u64;
	struct cvmx_tdecx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tdecx_eco_s               cnf75xx;
};
typedef union cvmx_tdecx_eco cvmx_tdecx_eco_t;

/**
 * cvmx_tdec#_error_enable0
 *
 * This register enables error reporting for read overflow/underflow errors.
 *
 */
union cvmx_tdecx_error_enable0 {
	uint64_t u64;
	struct cvmx_tdecx_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t rp1_of_en                    : 1;  /**< Read port 1 overflow enable. */
	uint64_t rp0_of_en                    : 1;  /**< Read port 0 overflow enable. */
	uint64_t reserved_2_3                 : 2;
	uint64_t rp1_uf_en                    : 1;  /**< Read Port 1 underflow enable. */
	uint64_t rp0_uf_en                    : 1;  /**< Read Port 0 underflow enable. */
#else
	uint64_t rp0_uf_en                    : 1;
	uint64_t rp1_uf_en                    : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t rp0_of_en                    : 1;
	uint64_t rp1_of_en                    : 1;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_tdecx_error_enable0_s     cnf75xx;
};
typedef union cvmx_tdecx_error_enable0 cvmx_tdecx_error_enable0_t;

/**
 * cvmx_tdec#_error_enable1
 *
 * This register enables error reporting of ECC errors.
 *
 */
union cvmx_tdecx_error_enable1 {
	uint64_t u64;
	struct cvmx_tdecx_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t jcfg2_ded_ecc                : 1;  /**< Job configuration 2 double-bit ECC error enable. */
	uint64_t jcfg1_ded_ecc                : 1;  /**< Job configuration 1 double-bit ECC error enable. */
	uint64_t jcfg0_ded_ecc                : 1;  /**< Job configuration 0 double-bit ECC error enable. */
	uint64_t reserved_3_3                 : 1;
	uint64_t jcfg2_sec_ecc                : 1;  /**< Job configuration 2 single-bit ECC error enable. */
	uint64_t jcfg1_sec_ecc                : 1;  /**< Job configuration 1 single-bit ECC error enable. */
	uint64_t jcfg0_sec_ecc                : 1;  /**< Job configuration 0 single-bit ECC error enable. */
#else
	uint64_t jcfg0_sec_ecc                : 1;
	uint64_t jcfg1_sec_ecc                : 1;
	uint64_t jcfg2_sec_ecc                : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t jcfg0_ded_ecc                : 1;
	uint64_t jcfg1_ded_ecc                : 1;
	uint64_t jcfg2_ded_ecc                : 1;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_tdecx_error_enable1_s     cnf75xx;
};
typedef union cvmx_tdecx_error_enable1 cvmx_tdecx_error_enable1_t;

/**
 * cvmx_tdec#_error_source0
 *
 * This register indicates the source of read overflow/underflow errors.
 *
 */
union cvmx_tdecx_error_source0 {
	uint64_t u64;
	struct cvmx_tdecx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t rp1_jid                      : 16; /**< Job ID of the job which reported the overflow/underflow error on read
                                                         port 1. */
	uint64_t rp0_jid                      : 16; /**< Job ID of the job which reported the overflow/underflow error on read
                                                         port 0. */
	uint64_t reserved_6_15                : 10;
	uint64_t rp1_of                       : 1;  /**< Read port 1 overflow. */
	uint64_t rp0_of                       : 1;  /**< Read port 0 overflow. */
	uint64_t reserved_2_3                 : 2;
	uint64_t rp1_uf                       : 1;  /**< Read port 1 underflow. */
	uint64_t rp0_uf                       : 1;  /**< Read port 0 underflow. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t rp1_uf                       : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t rp0_of                       : 1;
	uint64_t rp1_of                       : 1;
	uint64_t reserved_6_15                : 10;
	uint64_t rp0_jid                      : 16;
	uint64_t rp1_jid                      : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_tdecx_error_source0_s     cnf75xx;
};
typedef union cvmx_tdecx_error_source0 cvmx_tdecx_error_source0_t;

/**
 * cvmx_tdec#_error_source1
 *
 * This register reports the source of ECC errors.
 *
 */
union cvmx_tdecx_error_source1 {
	uint64_t u64;
	struct cvmx_tdecx_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t jcfg2_ded_ecc                : 1;  /**< Job configuration 2 double-bit error detected. */
	uint64_t jcfg1_ded_ecc                : 1;  /**< Job configuration 1 double-bit error detected. */
	uint64_t jcfg0_ded_ecc                : 1;  /**< Job configuration 0 double-bit error detected. */
	uint64_t reserved_3_3                 : 1;
	uint64_t jcfg2_sec_ecc                : 1;  /**< Job configuration 2 single-bit error corrected. */
	uint64_t jcfg1_sec_ecc                : 1;  /**< Job configuration 1 single-bit error corrected. */
	uint64_t jcfg0_sec_ecc                : 1;  /**< Job configuration 0 single-bit error corrected. */
#else
	uint64_t jcfg0_sec_ecc                : 1;
	uint64_t jcfg1_sec_ecc                : 1;
	uint64_t jcfg2_sec_ecc                : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t jcfg0_ded_ecc                : 1;
	uint64_t jcfg1_ded_ecc                : 1;
	uint64_t jcfg2_ded_ecc                : 1;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_tdecx_error_source1_s     cnf75xx;
};
typedef union cvmx_tdecx_error_source1 cvmx_tdecx_error_source1_t;

/**
 * cvmx_tdec#_hab_jcfg0_ram#_data
 *
 * This register range stores the job configuration for slot 0.
 *
 */
union cvmx_tdecx_hab_jcfg0_ramx_data {
	uint64_t u64;
	struct cvmx_tdecx_hab_jcfg0_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_tdecx_hab_jcfg0_ramx_data_s cnf75xx;
};
typedef union cvmx_tdecx_hab_jcfg0_ramx_data cvmx_tdecx_hab_jcfg0_ramx_data_t;

/**
 * cvmx_tdec#_hab_jcfg1_ram#_data
 *
 * This register range stores the job configuration for slot 1.
 *
 */
union cvmx_tdecx_hab_jcfg1_ramx_data {
	uint64_t u64;
	struct cvmx_tdecx_hab_jcfg1_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_tdecx_hab_jcfg1_ramx_data_s cnf75xx;
};
typedef union cvmx_tdecx_hab_jcfg1_ramx_data cvmx_tdecx_hab_jcfg1_ramx_data_t;

/**
 * cvmx_tdec#_hab_jcfg2_ram#_data
 *
 * This register range stores the job configuration for slot 2.
 *
 */
union cvmx_tdecx_hab_jcfg2_ramx_data {
	uint64_t u64;
	struct cvmx_tdecx_hab_jcfg2_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_tdecx_hab_jcfg2_ramx_data_s cnf75xx;
};
typedef union cvmx_tdecx_hab_jcfg2_ramx_data cvmx_tdecx_hab_jcfg2_ramx_data_t;

/**
 * cvmx_tdec#_jcfg0_ecc_error
 *
 * This register contains ECC error information for jcfg0.
 *
 */
union cvmx_tdecx_jcfg0_ecc_error {
	uint64_t u64;
	struct cvmx_tdecx_jcfg0_ecc_error_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t synd                         : 8;  /**< N/A */
	uint64_t reserved_8_31                : 24;
	uint64_t ecc                          : 8;  /**< N/A */
#else
	uint64_t ecc                          : 8;
	uint64_t reserved_8_31                : 24;
	uint64_t synd                         : 8;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_tdecx_jcfg0_ecc_error_s   cnf75xx;
};
typedef union cvmx_tdecx_jcfg0_ecc_error cvmx_tdecx_jcfg0_ecc_error_t;

/**
 * cvmx_tdec#_jcfg1_ecc_error
 *
 * This register contains ECC error information for jcfg1.
 *
 */
union cvmx_tdecx_jcfg1_ecc_error {
	uint64_t u64;
	struct cvmx_tdecx_jcfg1_ecc_error_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t synd                         : 8;  /**< N/A */
	uint64_t reserved_8_31                : 24;
	uint64_t ecc                          : 8;  /**< N/A */
#else
	uint64_t ecc                          : 8;
	uint64_t reserved_8_31                : 24;
	uint64_t synd                         : 8;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_tdecx_jcfg1_ecc_error_s   cnf75xx;
};
typedef union cvmx_tdecx_jcfg1_ecc_error cvmx_tdecx_jcfg1_ecc_error_t;

/**
 * cvmx_tdec#_jcfg2_ecc_error
 *
 * This register contains ECC error information for jcfg2.
 *
 */
union cvmx_tdecx_jcfg2_ecc_error {
	uint64_t u64;
	struct cvmx_tdecx_jcfg2_ecc_error_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t synd                         : 8;  /**< N/A */
	uint64_t reserved_8_31                : 24;
	uint64_t ecc                          : 8;  /**< N/A */
#else
	uint64_t ecc                          : 8;
	uint64_t reserved_8_31                : 24;
	uint64_t synd                         : 8;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_tdecx_jcfg2_ecc_error_s   cnf75xx;
};
typedef union cvmx_tdecx_jcfg2_ecc_error cvmx_tdecx_jcfg2_ecc_error_t;

/**
 * cvmx_tdec#_scratch
 *
 * Scratch register.
 *
 */
union cvmx_tdecx_scratch {
	uint64_t u64;
	struct cvmx_tdecx_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Scratch data. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_tdecx_scratch_s           cnf75xx;
};
typedef union cvmx_tdecx_scratch cvmx_tdecx_scratch_t;

/**
 * cvmx_tdec#_status
 */
union cvmx_tdecx_status {
	uint64_t u64;
	struct cvmx_tdecx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< When set, the TDEC HAB is ready to receive the next job. */
	uint64_t reserved_3_3                 : 1;
	uint64_t cfg2_busy                    : 1;  /**< When set, slot 2 is busy processing a job. */
	uint64_t cfg1_busy                    : 1;  /**< When set, slot 2 is busy processing a job. */
	uint64_t cfg0_busy                    : 1;  /**< When set, slot 2 is busy processing a job. */
#else
	uint64_t cfg0_busy                    : 1;
	uint64_t cfg1_busy                    : 1;
	uint64_t cfg2_busy                    : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tdecx_status_s            cnf75xx;
};
typedef union cvmx_tdecx_status cvmx_tdecx_status_t;

/**
 * cvmx_tdec#_tc_config_err_flags_reg
 *
 * This register reports task configuration errors that occur when a
 * specified parameter value is outside the acceptable range.  Different
 * bit-mappings are used depending on whether the PHY_MODE is LTE or 3G, and
 * whether the TASK_TYPE is E-DCH or DCH.
 */
union cvmx_tdecx_tc_config_err_flags_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_config_err_flags_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_59_63               : 5;
	uint64_t error58                      : 1;  /**< _ LTE mode: invalid CP_MODE.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error57                      : 1;  /**< _ LTE mode: invalid NUM_SYM, valid values are [9, 10, 11, 12].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error56                      : 1;  /**< _ LTE mode: invalid R_PRIME_MUX.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error55                      : 1;  /**< _ LTE mode: invalid Q_PRIME_ACK, valid values are [0,5184].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error54                      : 1;  /**< _ LTE mode: invalid Q_PRIME_RI, valid values are [0,5184].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error53                      : 1;  /**< _ LTE mode: invalid Q_PRIME_CQI, valid values are [0,15552].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error52                      : 1;  /**< Reserved. */
	uint64_t error51                      : 1;  /**< _ LTE mode: invalid BYPS_SCRAM.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error50                      : 1;  /**< _ LTE mode: invalid REENC_BYTE_ORDER, valid values are [0,1,2].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error49                      : 1;  /**< _ LTE mode: invalid REENC_BIT_ORDER.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error48                      : 1;  /**< _ LTE mode: invalid SYMB_BYTE_ALIGN.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error47                      : 1;  /**< _ LTE mode: invalid BYPS_CH_INT.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error46                      : 1;  /**< _ LTE mode: invalid BYPS_RM.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error45                      : 1;  /**< _ LTE mode: invalid REENC_ENA.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error44                      : 1;  /**< _ LTE mode: invalid SO_SEL.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error43                      : 1;  /**< _ LTE mode: invalid HALF_IT_SO, valid range is [1,32].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error42                      : 1;  /**< _ LTE mode: invalid BYPS_BS_SO.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error41                      : 1;  /**< _ LTE mode: invalid SO_ENA.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error40                      : 1;  /**< _ LTE mode: invalid CB_CRC, valid values are [0,1,2].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error39                      : 1;  /**< _ LTE mode: invalid HD_BYTE_ORDER, valid values are [0,1,2].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error38                      : 1;  /**< _ LTE mode: invalid HD_BIT_ORDER.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error37                      : 1;  /**< _ LTE mode: invalid HD_OUT_ENA.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error36                      : 1;  /**< _ LTE mode: invalid ALGO, valid values are [0,1,3].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error35                      : 1;  /**< _ LTE mode: invalid SW_SPEED, valid values are [0,2,4].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error34                      : 1;  /**< _ LTE mode: invalid TBL_SEL, valid values are [0,1,2].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error33                      : 1;  /**< _ LTE mode: invalid THRESH.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error32                      : 1;  /**< _ LTE mode: invalid DYN_STOP, valid values are [0,1,2].
                                                         _ DCH mode: invalid ALGO, valid values are [0,1,3].
                                                         _ EDCH mode: invalid BYPS_CRC. */
	uint64_t error31                      : 1;  /**< _ LTE mode: invalid HALF_IT, valid range is [1,32].
                                                         _ DCH mode: invalid SW_SPEED, valid values are [0,2,4].
                                                         _ EDCH mode: invalid ALGO, valid values are [0,1,3]. */
	uint64_t error30                      : 1;  /**< _ LTE mode: invalid HARQ_RND, valid range is [0,11].
                                                         _ DCH mode: invalid TBL_SEL, valid values are [0,1,2].
                                                         _ EDCH mode: invalid SW_SPEED, valid values are [0,2,4]. */
	uint64_t error29                      : 1;  /**< _ LTE mode: invalid DEC_RND, valid range is [0,2].
                                                         _ DCH mode: invalid THRESH.
                                                         _ EDCH mode: invalid TBL_SEL, valid values are [0,1,2]. */
	uint64_t error28                      : 1;  /**< _ LTE mode: invalid GAIN_H.
                                                         _ DCH mode: invalid DYN_STOP, valid values are [0,1,2].
                                                         _ EDCH mode: invalid THRESH. */
	uint64_t error27                      : 1;  /**< _ LTE mode: invalid GAIN_I.
                                                         _ DCH mode: invalid HALF_IT, valid range is [1,32].
                                                         _ EDCH mode: invalid DYN_STOP, valid values are [0,1,2]. */
	uint64_t error26                      : 1;  /**< _ LTE mode: invalid BYPS_LLR_NEGATE.
                                                         _ DCH mode: invalid BYPS_CRC.
                                                         _ EDCH mode: invalid HALF_IT, valid range is [1:32]. */
	uint64_t error25                      : 1;  /**< _ LTE mode: invalid K0_P, valid range is [0:3K+12-2F-1].
                                                         _ DCH mode: invalid BYPS_LLR_NEGATE.
                                                         _ EDCH mode: invalid HARQ_RND, valid range is [0,11]. */
	uint64_t error24                      : 1;  /**< _ LTE mode: invalid HC_OUT_ENA.
                                                         _ DCH mode: invalid BYPS_TB_PROC.
                                                         _ EDCH mode: invalid DEC_RND, valid range is [0,2]. */
	uint64_t error23                      : 1;  /**< _ LTE mode: invalid BYPS_HC_COMP.
                                                         _ DCH mode: invalid CRC_SIZE, valid range is [0,4].
                                                         _ EDCH mode: invalid GAIN_H. */
	uint64_t error22                      : 1;  /**< _ LTE mode: invalid HC_IN_ENA.
                                                         _ DCH mode: invalid FILLER, valid range is [0,40].
                                                         _ EDCH mode: invalid GAIN_I. */
	uint64_t error21                      : 1;  /**< _ LTE mode: invalid FILLER, valid range is [0,63].
                                                         _ DCH mode: invalid TR_BLK_LEN, valid range is [1,5000].
                                                         _ EDCH mode: invalid HC_OUT_ENA. */
	uint64_t error20                      : 1;  /**< _ LTE mode: invalid BYPS_BIT_SEL.
                                                         _ DCH mode: invalid CD_BLK_LEN, valid range is [40,5114].
                                                         _ EDCH mode: invalid HC_IN_ENA. */
	uint64_t error19                      : 1;  /**< _ LTE mode: invalid BYPS_TB_PROC.
                                                         _ DCH mode: invalid NUM_CB, valid range is [1,3].
                                                         _ EDCH mode: Reserved. */
	uint64_t error18                      : 1;  /**< _ LTE mode: invalid LAYERS, valid values are [1,2].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error17                      : 1;  /**< _ LTE mode: invalid BYPS_DEC.
                                                         _ DCH mode: invalid FEC_MODE, must be 0.
                                                         _ EDCH mode: Reserved. */
	uint64_t error16                      : 1;  /**< _ LTE mode: invalid MOD_ORDER, valid values are [2,4,6].
                                                         _ DCH mode: invalid NUM_TR_BLK, valid range is [1,32].
                                                         _ EDCH mode: Reserved. */
	uint64_t error15                      : 1;  /**< _ LTE mode: invalid BYPS_LAYER_DEMAP.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error14                      : 1;  /**< Reserved. */
	uint64_t error13                      : 1;  /**< _ LTE mode: invalid BYPS_CRC_MSK.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error12                      : 1;  /**< _ LTE mode: invalid RV_IDX, valid range is [0,3].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error11                      : 1;  /**< _ LTE mode: invalid GAMMA, valid range is [0:NUM_CB-1].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error10                      : 1;  /**< _ LTE mode: invalid FIRST_CB_RM, valid range is [2:2^20-1].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error9                       : 1;  /**< _ LTE mode: invalid FIRST_CB_SIZE, valid range is [40,6144].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: invalid PUNCT_REP. */
	uint64_t error8                       : 1;  /**< _ LTE mode: invalid TX_BITS, valid range is [1:2^20-1].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: invalid FILLER, valid range is [0,40]. */
	uint64_t error7                       : 1;  /**< _ LTE mode: invalid PAYLOAD, valid range is [1,777216].
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: invalid CD_BLK_LEN, valid range is [40,5114]. */
	uint64_t error6                       : 1;  /**< _ LTE mode: invalid NUM_CB, valid range is [1,127].
                                                         _ DCH mode: invalid CONV_LLR, valid range is [0,4].
                                                         _ EDCH mode: Reserved. */
	uint64_t error5                       : 1;  /**< _ LTE mode: Reserved.
                                                         _ DCH mode: invalid PUNCT_REP.
                                                         _ EDCH mode: Reserved. */
	uint64_t error4                       : 1;  /**< _ LTE mode: invalid combination of BYPS_DEC, HD_OUT_ENA, REENC_ENA,
                                                         and SO_ENA (decoded and re-encoded outputs not available when
                                                         BYPS_DEC=1).
                                                         _ DCH mode: invalid TTI, valid range is [0,3].
                                                         _ EDCH mode: invalid BYPS_LLR_NEGATE. */
	uint64_t error3                       : 1;  /**< _ LTE mode: invalid combination of BYPS_RM=1 and BYPS_CH_INT=0.
                                                         _ DCH mode: invalid HD_BYTE_ORDER, valid range is [0,1,2].
                                                         _ EDCH mode: invalid HD_BYTE_ORDER, valid range is [0,1,2]. */
	uint64_t error2                       : 1;  /**< _ LTE mode: invalid combination of HC_OUT_ENA=0 and BYPS_DEC=1.
                                                         _ DCH mode: invalid HD_BIT_ORDER.
                                                         _ EDCH mode: invalid HD_BIT_ORDER. */
	uint64_t error1                       : 1;  /**< _ LTE mode: invalid combination of HD_OUT_ENA, SO_ENA, HC_OUT_ENA,
                                                         REENC_ENA, at least one output must be selected.
                                                         _ DCH mode: Reserved.
                                                         _ EDCH mode: Reserved. */
	uint64_t error0                       : 1;  /**< _ LTE mode: Reserved.
                                                         _ DCH mode: invalid PHY_MODE.
                                                         _ EDCH mode: invalid PHY_MODE. */
#else
	uint64_t error0                       : 1;
	uint64_t error1                       : 1;
	uint64_t error2                       : 1;
	uint64_t error3                       : 1;
	uint64_t error4                       : 1;
	uint64_t error5                       : 1;
	uint64_t error6                       : 1;
	uint64_t error7                       : 1;
	uint64_t error8                       : 1;
	uint64_t error9                       : 1;
	uint64_t error10                      : 1;
	uint64_t error11                      : 1;
	uint64_t error12                      : 1;
	uint64_t error13                      : 1;
	uint64_t error14                      : 1;
	uint64_t error15                      : 1;
	uint64_t error16                      : 1;
	uint64_t error17                      : 1;
	uint64_t error18                      : 1;
	uint64_t error19                      : 1;
	uint64_t error20                      : 1;
	uint64_t error21                      : 1;
	uint64_t error22                      : 1;
	uint64_t error23                      : 1;
	uint64_t error24                      : 1;
	uint64_t error25                      : 1;
	uint64_t error26                      : 1;
	uint64_t error27                      : 1;
	uint64_t error28                      : 1;
	uint64_t error29                      : 1;
	uint64_t error30                      : 1;
	uint64_t error31                      : 1;
	uint64_t error32                      : 1;
	uint64_t error33                      : 1;
	uint64_t error34                      : 1;
	uint64_t error35                      : 1;
	uint64_t error36                      : 1;
	uint64_t error37                      : 1;
	uint64_t error38                      : 1;
	uint64_t error39                      : 1;
	uint64_t error40                      : 1;
	uint64_t error41                      : 1;
	uint64_t error42                      : 1;
	uint64_t error43                      : 1;
	uint64_t error44                      : 1;
	uint64_t error45                      : 1;
	uint64_t error46                      : 1;
	uint64_t error47                      : 1;
	uint64_t error48                      : 1;
	uint64_t error49                      : 1;
	uint64_t error50                      : 1;
	uint64_t error51                      : 1;
	uint64_t error52                      : 1;
	uint64_t error53                      : 1;
	uint64_t error54                      : 1;
	uint64_t error55                      : 1;
	uint64_t error56                      : 1;
	uint64_t error57                      : 1;
	uint64_t error58                      : 1;
	uint64_t reserved_59_63               : 5;
#endif
	} s;
	struct cvmx_tdecx_tc_config_err_flags_reg_s cnf75xx;
};
typedef union cvmx_tdecx_tc_config_err_flags_reg cvmx_tdecx_tc_config_err_flags_reg_t;

/**
 * cvmx_tdec#_tc_config_reg#
 *
 * TC task config registers.
 *
 */
union cvmx_tdecx_tc_config_regx {
	uint64_t u64;
	struct cvmx_tdecx_tc_config_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< Config bits. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_tdecx_tc_config_regx_s    cnf75xx;
};
typedef union cvmx_tdecx_tc_config_regx cvmx_tdecx_tc_config_regx_t;

/**
 * cvmx_tdec#_tc_control_reg
 *
 * This register configures the TDEC internal decoder core.
 *
 */
union cvmx_tdecx_tc_control_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_control_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t single_task_chnl_en          : 1;  /**< When set to 1, the core will wait until idle before accepting a new job with a different TASK_TYPE. */
	uint64_t single_task_phy_en           : 1;  /**< When set to 1, the core will wait until idle before accepting a new job with a different PHY_MODE. */
	uint64_t single_task_en               : 1;  /**< When set to 1, the core will always wait until idle before
                                                         starting the next task. */
	uint64_t cfg_chk_dis                  : 1;  /**< When set to 1, disables config checks. Tasks with
                                                         invalid configurations will still be processed. */
	uint64_t clk_gate_dis                 : 1;  /**< When set to 1, disables clock gating. */
#else
	uint64_t clk_gate_dis                 : 1;
	uint64_t cfg_chk_dis                  : 1;
	uint64_t single_task_en               : 1;
	uint64_t single_task_phy_en           : 1;
	uint64_t single_task_chnl_en          : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tdecx_tc_control_reg_s    cnf75xx;
};
typedef union cvmx_tdecx_tc_control_reg cvmx_tdecx_tc_control_reg_t;

/**
 * cvmx_tdec#_tc_error_mask_reg
 *
 * This register enables internal decoder errors. Errors reported in
 * TDEC()_TC_ERROR_REG will generate an error signal only when the
 * corresponding bit is set in TDEC()_TC_ERROR_REG[ERR_MASK].
 */
union cvmx_tdecx_tc_error_mask_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_error_mask_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t err_mask                     : 4;  /**< Error mask bits. */
#else
	uint64_t err_mask                     : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tdecx_tc_error_mask_reg_s cnf75xx;
};
typedef union cvmx_tdecx_tc_error_mask_reg cvmx_tdecx_tc_error_mask_reg_t;

/**
 * cvmx_tdec#_tc_error_reg
 *
 * This register reports various error conditions.
 * Errors are cleared by writing the specific error bits to zero.
 *
 * All errors reported in this register are reported as FATAL errors, and the
 * MHBW registers can be inspected to determine the job tag(s) associated with
 * the error(s).
 */
union cvmx_tdecx_tc_error_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_error_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t hcout_int_err                : 1;  /**< HARQ output internal error in LTE mode. Insufficient data. */
	uint64_t hcin_input_err               : 1;  /**< HARQ input error in LTE mode. Missing one CB. */
	uint64_t hcin_size_err                : 1;  /**< HARQ input size error in LTE mode. */
	uint64_t reserved_9_16                : 8;
	uint64_t inv_cfg                      : 1;  /**< Invalid task configuration. Task aborted. */
	uint64_t reg_mask_err                 : 4;  /**< Reserved. */
	uint64_t ign_read                     : 1;  /**< Ignored read access error -- one already in progress. */
	uint64_t inv_read                     : 1;  /**< Invalid read access error. */
	uint64_t inv_write                    : 1;  /**< Invalid write access error. */
	uint64_t inv_start                    : 1;  /**< Invalid start task error. */
#else
	uint64_t inv_start                    : 1;
	uint64_t inv_write                    : 1;
	uint64_t inv_read                     : 1;
	uint64_t ign_read                     : 1;
	uint64_t reg_mask_err                 : 4;
	uint64_t inv_cfg                      : 1;
	uint64_t reserved_9_16                : 8;
	uint64_t hcin_size_err                : 1;
	uint64_t hcin_input_err               : 1;
	uint64_t hcout_int_err                : 1;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_tdecx_tc_error_reg_s      cnf75xx;
};
typedef union cvmx_tdecx_tc_error_reg cvmx_tdecx_tc_error_reg_t;

/**
 * cvmx_tdec#_tc_main_reset_reg
 *
 * Internal decoder core reset register.
 *
 */
union cvmx_tdecx_tc_main_reset_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_main_reset_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t soft_reset                   : 1;  /**< Reset bit. */
#else
	uint64_t soft_reset                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tdecx_tc_main_reset_reg_s cnf75xx;
};
typedef union cvmx_tdecx_tc_main_reset_reg cvmx_tdecx_tc_main_reset_reg_t;

/**
 * cvmx_tdec#_tc_main_start_reg
 *
 * TC start register.
 *
 */
union cvmx_tdecx_tc_main_start_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_main_start_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< Start bit. */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tdecx_tc_main_start_reg_s cnf75xx;
};
typedef union cvmx_tdecx_tc_main_start_reg cvmx_tdecx_tc_main_start_reg_t;

/**
 * cvmx_tdec#_tc_mon_reg#
 *
 * TC mon registers.
 *
 */
union cvmx_tdecx_tc_mon_regx {
	uint64_t u64;
	struct cvmx_tdecx_tc_mon_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t bus_val                      : 16; /**< Output bus monitoring values. */
#else
	uint64_t bus_val                      : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_tdecx_tc_mon_regx_s       cnf75xx;
};
typedef union cvmx_tdecx_tc_mon_regx cvmx_tdecx_tc_mon_regx_t;

/**
 * cvmx_tdec#_tc_status0_reg
 *
 * This register reports status from the internal decoder core.
 *
 */
union cvmx_tdecx_tc_status0_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_status0_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t core_stat                    : 32; /**< N/A */
	uint64_t reserved_1_31                : 31;
	uint64_t idle                         : 1;  /**< Idle status bit. */
#else
	uint64_t idle                         : 1;
	uint64_t reserved_1_31                : 31;
	uint64_t core_stat                    : 32;
#endif
	} s;
	struct cvmx_tdecx_tc_status0_reg_s    cnf75xx;
};
typedef union cvmx_tdecx_tc_status0_reg cvmx_tdecx_tc_status0_reg_t;

/**
 * cvmx_tdec#_tc_status1_reg
 *
 * TC status 1 register.
 *
 */
union cvmx_tdecx_tc_status1_reg {
	uint64_t u64;
	struct cvmx_tdecx_tc_status1_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t core_stat                    : 64; /**< Core-dependent bits. */
#else
	uint64_t core_stat                    : 64;
#endif
	} s;
	struct cvmx_tdecx_tc_status1_reg_s    cnf75xx;
};
typedef union cvmx_tdecx_tc_status1_reg cvmx_tdecx_tc_status1_reg_t;

#endif
