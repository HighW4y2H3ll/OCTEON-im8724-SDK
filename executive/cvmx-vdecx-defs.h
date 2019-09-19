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
 * cvmx-vdecx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon vdecx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_VDECX_DEFS_H__
#define __CVMX_VDECX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700060ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3700060ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3700000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ECC_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ECC_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700050ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ECC_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3700050ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B3700008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3700040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ERROR_ENABLE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ERROR_ENABLE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700048ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ERROR_ENABLE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3700048ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3700030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_ERROR_SOURCE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_ERROR_SOURCE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_ERROR_SOURCE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3700038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_HAB_JCFG0_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_VDECX_HAB_JCFG0_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3702000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_VDECX_HAB_JCFG0_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3702000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_HAB_JCFG1_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_VDECX_HAB_JCFG1_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3704000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_VDECX_HAB_JCFG1_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3704000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_HAB_JCFG2_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_VDECX_HAB_JCFG2_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3706000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_VDECX_HAB_JCFG2_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3706000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_JCFG0_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_JCFG0_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700100ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_JCFG0_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3700100ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_JCFG1_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_JCFG1_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700108ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_JCFG1_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3700108ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_JCFG2_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_JCFG2_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700110ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_JCFG2_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3700110ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_SCRATCH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_SCRATCH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700080ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_SCRATCH(offset) (CVMX_ADD_IO_SEG(0x00011800B3700080ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3700018ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3700018ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_CONFIG_ERR_FLAGS_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_CONFIG_ERR_FLAGS_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_CONFIG_ERR_FLAGS_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_CONFIG_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 12)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_VDECX_TC_CONFIG_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3701400ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_VDECX_TC_CONFIG_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3701400ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_CONTROL_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_CONTROL_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701010ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_CONTROL_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701010ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_ERROR_MASK_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_ERROR_MASK_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_ERROR_MASK_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_ERROR_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_ERROR_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_ERROR_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_MAIN_RESET_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_MAIN_RESET_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_MAIN_RESET_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_MAIN_START_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_MAIN_START_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_MAIN_START_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_MON_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_VDECX_TC_MON_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3701300ull) + (((offset) & 1) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_VDECX_TC_MON_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3701300ull) + (((offset) & 1) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_STATUS0_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_STATUS0_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701020ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_STATUS0_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701020ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_VDECX_TC_STATUS1_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_VDECX_TC_STATUS1_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3701028ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_VDECX_TC_STATUS1_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3701028ull) + ((offset) & 1) * 32768)
#endif

/**
 * cvmx_vdec#_bist_status
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_vdecx_bist_status {
	uint64_t u64;
	struct cvmx_vdecx_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t bisr_status                  : 14; /**< BIST status results. */
#else
	uint64_t bisr_status                  : 14;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_vdecx_bist_status_s       cnf75xx;
};
typedef union cvmx_vdecx_bist_status cvmx_vdecx_bist_status_t;

/**
 * cvmx_vdec#_control
 */
union cvmx_vdecx_control {
	uint64_t u64;
	struct cvmx_vdecx_control_s {
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
	struct cvmx_vdecx_control_s           cnf75xx;
};
typedef union cvmx_vdecx_control cvmx_vdecx_control_t;

/**
 * cvmx_vdec#_ecc_control
 */
union cvmx_vdecx_ecc_control {
	uint64_t u64;
	struct cvmx_vdecx_ecc_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_35_63               : 29;
	uint64_t flip_bit2                    : 3;  /**< This bit flips a second bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_19_31               : 13;
	uint64_t flip_bit1                    : 3;  /**< This bit flips the first bit of syndrome of the write port paths to the respective RAM. */
	uint64_t reserved_3_15                : 13;
	uint64_t jcfg2_cor_dis                : 1;  /**< This bit disables single-bit error correction of the job configuration 2 RAMs. */
	uint64_t jcfg1_cor_dis                : 1;  /**< This bit disables single-bit error correction of the job configuration 1 RAMs. */
	uint64_t jcfg0_cor_dis                : 1;  /**< This bit disables single-bit error correction of the job configuration 0 RAMs. */
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
	struct cvmx_vdecx_ecc_control_s       cnf75xx;
};
typedef union cvmx_vdecx_ecc_control cvmx_vdecx_ecc_control_t;

/**
 * cvmx_vdec#_eco
 */
union cvmx_vdecx_eco {
	uint64_t u64;
	struct cvmx_vdecx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_vdecx_eco_s               cnf75xx;
};
typedef union cvmx_vdecx_eco cvmx_vdecx_eco_t;

/**
 * cvmx_vdec#_error_enable0
 *
 * This register enables error reporting for read overflow/underflow errors.
 *
 */
union cvmx_vdecx_error_enable0 {
	uint64_t u64;
	struct cvmx_vdecx_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t rp0_of_en                    : 1;  /**< Read port 0 overflow enable. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf_en                    : 1;  /**< Read Port 0 underflow enable. */
#else
	uint64_t rp0_uf_en                    : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of_en                    : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_vdecx_error_enable0_s     cnf75xx;
};
typedef union cvmx_vdecx_error_enable0 cvmx_vdecx_error_enable0_t;

/**
 * cvmx_vdec#_error_enable1
 *
 * This register enables error reporting of ECC errors.
 *
 */
union cvmx_vdecx_error_enable1 {
	uint64_t u64;
	struct cvmx_vdecx_error_enable1_s {
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
	struct cvmx_vdecx_error_enable1_s     cnf75xx;
};
typedef union cvmx_vdecx_error_enable1 cvmx_vdecx_error_enable1_t;

/**
 * cvmx_vdec#_error_source0
 *
 * This register indicates the source of read overflow/underflow errors.
 *
 */
union cvmx_vdecx_error_source0 {
	uint64_t u64;
	struct cvmx_vdecx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jid                      : 16; /**< Job ID of the job which reported the overflow/underflow error on read
                                                         port 0. */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read port 0 overflow. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read port 0 underflow. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jid                      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_vdecx_error_source0_s     cnf75xx;
};
typedef union cvmx_vdecx_error_source0 cvmx_vdecx_error_source0_t;

/**
 * cvmx_vdec#_error_source1
 *
 * This register reports the source of ECC errors.
 *
 */
union cvmx_vdecx_error_source1 {
	uint64_t u64;
	struct cvmx_vdecx_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t jcfg2_ded_ecc                : 1;  /**< Job configuration 2 double-bit ECC error detected. */
	uint64_t jcfg1_ded_ecc                : 1;  /**< Job configuration 1 double-bit ECC error detected. */
	uint64_t jcfg0_ded_ecc                : 1;  /**< Job configuration 0 double-bit ECC error detected. */
	uint64_t reserved_3_3                 : 1;
	uint64_t jcfg2_sec_ecc                : 1;  /**< Job configuration 2 single-bit ECC error corrected. */
	uint64_t jcfg1_sec_ecc                : 1;  /**< Job configuration 1 single-bit ECC error corrected. */
	uint64_t jcfg0_sec_ecc                : 1;  /**< Job configuration 0 single-bit ECC error corrected. */
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
	struct cvmx_vdecx_error_source1_s     cnf75xx;
};
typedef union cvmx_vdecx_error_source1 cvmx_vdecx_error_source1_t;

/**
 * cvmx_vdec#_hab_jcfg0_ram#_data
 *
 * This register range stores the job configuration for slot 0.
 *
 */
union cvmx_vdecx_hab_jcfg0_ramx_data {
	uint64_t u64;
	struct cvmx_vdecx_hab_jcfg0_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_vdecx_hab_jcfg0_ramx_data_s cnf75xx;
};
typedef union cvmx_vdecx_hab_jcfg0_ramx_data cvmx_vdecx_hab_jcfg0_ramx_data_t;

/**
 * cvmx_vdec#_hab_jcfg1_ram#_data
 *
 * This register range stores the job configuration for slot 1.
 *
 */
union cvmx_vdecx_hab_jcfg1_ramx_data {
	uint64_t u64;
	struct cvmx_vdecx_hab_jcfg1_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_vdecx_hab_jcfg1_ramx_data_s cnf75xx;
};
typedef union cvmx_vdecx_hab_jcfg1_ramx_data cvmx_vdecx_hab_jcfg1_ramx_data_t;

/**
 * cvmx_vdec#_hab_jcfg2_ram#_data
 *
 * This register range stores the job configuration for slot 2.
 *
 */
union cvmx_vdecx_hab_jcfg2_ramx_data {
	uint64_t u64;
	struct cvmx_vdecx_hab_jcfg2_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_vdecx_hab_jcfg2_ramx_data_s cnf75xx;
};
typedef union cvmx_vdecx_hab_jcfg2_ramx_data cvmx_vdecx_hab_jcfg2_ramx_data_t;

/**
 * cvmx_vdec#_jcfg0_ecc_error
 *
 * This register contains ECC error information for jcfg0.
 *
 */
union cvmx_vdecx_jcfg0_ecc_error {
	uint64_t u64;
	struct cvmx_vdecx_jcfg0_ecc_error_s {
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
	struct cvmx_vdecx_jcfg0_ecc_error_s   cnf75xx;
};
typedef union cvmx_vdecx_jcfg0_ecc_error cvmx_vdecx_jcfg0_ecc_error_t;

/**
 * cvmx_vdec#_jcfg1_ecc_error
 *
 * This register contains ECC error information for jcfg1.
 *
 */
union cvmx_vdecx_jcfg1_ecc_error {
	uint64_t u64;
	struct cvmx_vdecx_jcfg1_ecc_error_s {
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
	struct cvmx_vdecx_jcfg1_ecc_error_s   cnf75xx;
};
typedef union cvmx_vdecx_jcfg1_ecc_error cvmx_vdecx_jcfg1_ecc_error_t;

/**
 * cvmx_vdec#_jcfg2_ecc_error
 *
 * This register contains ECC error information for jcfg2.
 *
 */
union cvmx_vdecx_jcfg2_ecc_error {
	uint64_t u64;
	struct cvmx_vdecx_jcfg2_ecc_error_s {
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
	struct cvmx_vdecx_jcfg2_ecc_error_s   cnf75xx;
};
typedef union cvmx_vdecx_jcfg2_ecc_error cvmx_vdecx_jcfg2_ecc_error_t;

/**
 * cvmx_vdec#_scratch
 *
 * Scratch register.
 *
 */
union cvmx_vdecx_scratch {
	uint64_t u64;
	struct cvmx_vdecx_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Scratch data. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_vdecx_scratch_s           cnf75xx;
};
typedef union cvmx_vdecx_scratch cvmx_vdecx_scratch_t;

/**
 * cvmx_vdec#_status
 */
union cvmx_vdecx_status {
	uint64_t u64;
	struct cvmx_vdecx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< When set, VDEC is ready to receive the next job. */
	uint64_t reserved_3_3                 : 1;
	uint64_t cfg2_busy                    : 1;  /**< When set, slot 2 is busy processing a job. */
	uint64_t cfg1_busy                    : 1;  /**< When set, slot 1 is busy processing a job. */
	uint64_t cfg0_busy                    : 1;  /**< When set, slot 0 is busy processing a job. */
#else
	uint64_t cfg0_busy                    : 1;
	uint64_t cfg1_busy                    : 1;
	uint64_t cfg2_busy                    : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_vdecx_status_s            cnf75xx;
};
typedef union cvmx_vdecx_status cvmx_vdecx_status_t;

/**
 * cvmx_vdec#_tc_config_err_flags_reg
 *
 * This register reports task configuration errors, that occur when a
 * specified parameter value is outside the acceptable range.  Different
 * bit mappings are used depending on whether the PHY_MODE for the job is set
 * to LTE, GSM or 3G mode.
 */
union cvmx_vdecx_tc_config_err_flags_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_config_err_flags_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t error33                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid TRACEBACK_LEN [20:TB_MAX]. */
	uint64_t error32                      : 1;  /**< Reserved. */
	uint64_t error31                      : 1;  /**< Reserved. */
	uint64_t error30                      : 1;  /**< Reserved. */
	uint64_t error29                      : 1;  /**< Reserved. */
	uint64_t error28                      : 1;  /**< Reserved. */
	uint64_t error27                      : 1;  /**< Reserved. */
	uint64_t error26                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid BYPASS_CRC (0,1). */
	uint64_t error25                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid BYPS_LLR_NEGATE (0,1). */
	uint64_t error24                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid BPYS_TB_PROC (0,1). */
	uint64_t error23                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid CRC_SIZE [0:4]. */
	uint64_t error22                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid FILLER [0:40]. */
	uint64_t error21                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid TB_LEN [1:5000]. */
	uint64_t error20                      : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid CB_LEN [1:5114]. */
	uint64_t error19                      : 1;  /**< _ LTE mode: invalid REENC_BYTE_ORDER (0,1,2).
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid NUM_CB [1:3]. */
	uint64_t error18                      : 1;  /**< _ LTE mode: invalid REENC_BIT_ORDER (0,1).
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid CODE_RATE (0,1). */
	uint64_t error17                      : 1;  /**< _ LTE mode: invalid DEC_BYTE_ORDER (0,1,2).
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid FEC_MODE (0,1). */
	uint64_t error16                      : 1;  /**< _ LTE mode: invalid DEC_BIT_ORDER (0,1).
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: invalid NUM_TB [1:32]. */
	uint64_t error15                      : 1;  /**< _ LTE mode: invalid SYMB_BYTE_ALIGN (0,1).
                                                         _ GSM mode: invalid START_METRIC1 [0:2048].
                                                         _ 3G mode: Reserved. */
	uint64_t error14                      : 1;  /**< _ LTE mode: invalid REENC_SIZE [1:2^18-1].
                                                         _ GSM mode: invalid START_METRIC0 [0:2048].
                                                         _ 3G mode: Reserved. */
	uint64_t error13                      : 1;  /**< _ LTE mode: invalid BYPS_REENC_RM.
                                                         _ GSM mode: invalid DEC_BIT_ORDER (0,1).
                                                         _ 3G mode: Reserved. */
	uint64_t error12                      : 1;  /**< _ LTE mode: invalid REENC_ENA (0,1).
                                                         _ GSM mode: invalid DEC_BYTE_ORDER (0,1,2).
                                                         _ 3G mode: Reserved. */
	uint64_t error11                      : 1;  /**< _ LTE mode: invalid LLR_LVL [0:6].
                                                         _ GSM mode: invalid TAILBITING (0,1).
                                                         _ 3G mode: Reserved. */
	uint64_t error10                      : 1;  /**< _ LTE mode: invalid LLR_MODE (0,1,2).
                                                         _ GSM mode: invalid TAILBITS (0,1).
                                                         _ 3G mode: Reserved. */
	uint64_t error9                       : 1;  /**< _ LTE mode: invalid BYPS_LLR_NEGATE (0,1).
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: Reserved. */
	uint64_t error8                       : 1;  /**< _ LTE mode: invalid BYPS_CIRC_EXT (0,1).
                                                         _ GSM mode: invalid RECURSIVE (0,1).
                                                         _ 3G mode: Reserved. */
	uint64_t error7                       : 1;  /**< _ LTE mode: invalid BYPS_CRC (0,1).
                                                         _ GSM mode: invalid RATE_INDEX [2:10].
                                                         _ 3G mode: Reserved. */
	uint64_t error6                       : 1;  /**< _ LTE mode: invalid MOD_ORDER (2,4,6).
                                                         _ GSM mode: invalid CODED_BITS [36:1938].
                                                         _ 3G mode: invalid LLR_LVL [0: I_WIDTH + REP_WIDTH-D_WIDTH]. */
	uint64_t error5                       : 1;  /**< _ LTE mode: invalid BYPS_LAYER_DEMAP (0,1).
                                                         _ GSM mode: invalid DEC_BITS [18:876].
                                                         _ 3G mode: invalid PINCT_REP [0:255]. */
	uint64_t error4                       : 1;  /**< _ LTE mode: invalid NUM_LLR [1:2^18-1].
                                                         _ GSM mode: invalid TRACEBACK_LEN [20:212].
                                                         _ 3G mode: invalid TTI [0:3]. */
	uint64_t error3                       : 1;  /**< _ LTE mode: invalid BYPS_RM (0,1).
                                                         _ GSM mode: invalid CHANNEL [1:189].
                                                         _ 3G mode: invalid DEC_BYTE_ORDER (0,1). */
	uint64_t error2                       : 1;  /**< _ LTE mode: invalid TRACEBACK_LEN [20:96].
                                                         _ GSM mode: invalid BYPS_FRONTEND (0,1).
                                                         _ 3G mode: invalid DEC_BIT_ORDER (0,1). */
	uint64_t error1                       : 1;  /**< _ LTE mode: invalid DEC_OUT_SIZE [20:8192].
                                                         _ GSM mode: Reserved.
                                                         _ 3G mode: Reserved. */
	uint64_t error0                       : 1;  /**< _ LTE mode: Reserved.
                                                         _ GSM mode: invalid PHY_MODE (0,1,2).
                                                         _ 3G mode: invalid PHY_MODE (0,1,2). */
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
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_vdecx_tc_config_err_flags_reg_s cnf75xx;
};
typedef union cvmx_vdecx_tc_config_err_flags_reg cvmx_vdecx_tc_config_err_flags_reg_t;

/**
 * cvmx_vdec#_tc_config_reg#
 *
 * TC task config registers.
 *
 */
union cvmx_vdecx_tc_config_regx {
	uint64_t u64;
	struct cvmx_vdecx_tc_config_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< Config bits. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_vdecx_tc_config_regx_s    cnf75xx;
};
typedef union cvmx_vdecx_tc_config_regx cvmx_vdecx_tc_config_regx_t;

/**
 * cvmx_vdec#_tc_control_reg
 *
 * This register controls the internal VDEC decoder core.
 *
 */
union cvmx_vdecx_tc_control_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_control_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t single_task_chnl_en          : 1;  /**< When set to 1, the core will wait until idle before starting the next
                                                         task when changing channel mode. */
	uint64_t single_task_phy_en           : 1;  /**< When set to 1, the core will wait until idle before starting the next
                                                         task when changing PHY mode. */
	uint64_t single_task_en               : 1;  /**< When set to 1, the core will always wait until idle before starting
                                                         the next task. */
	uint64_t cfg_chk_dis                  : 1;  /**< When set to 1, disables config checks. Tasks with invalid
                                                         configurations will still be processed. */
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
	struct cvmx_vdecx_tc_control_reg_s    cnf75xx;
};
typedef union cvmx_vdecx_tc_control_reg cvmx_vdecx_tc_control_reg_t;

/**
 * cvmx_vdec#_tc_error_mask_reg
 *
 * This register enables internal decoder errors. Errors reported in
 * VDEC()_TC_ERROR_REG will generate an error signal only when the
 * corresponding bit is set in VDEC()_TC_ERROR_REG[ERR_MASK].
 */
union cvmx_vdecx_tc_error_mask_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_error_mask_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t err_mask                     : 4;  /**< Error mask bits. */
#else
	uint64_t err_mask                     : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_vdecx_tc_error_mask_reg_s cnf75xx;
};
typedef union cvmx_vdecx_tc_error_mask_reg cvmx_vdecx_tc_error_mask_reg_t;

/**
 * cvmx_vdec#_tc_error_reg
 *
 * This register reports decoder errors.
 *
 */
union cvmx_vdecx_tc_error_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_error_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t inv_cfg                      : 1;  /**< Invalid task configuration -- aborted job. */
	uint64_t reg_mask_err                 : 4;  /**< Reserved. */
	uint64_t ign_read                     : 1;  /**< Ignored a read access while another read was in process. */
	uint64_t inv_read                     : 1;  /**< Invalid read access to an out-of-range address. */
	uint64_t inv_write                    : 1;  /**< Invalid write access to an out-of-range address. */
	uint64_t inv_start                    : 1;  /**< Invalid task start. */
#else
	uint64_t inv_start                    : 1;
	uint64_t inv_write                    : 1;
	uint64_t inv_read                     : 1;
	uint64_t ign_read                     : 1;
	uint64_t reg_mask_err                 : 4;
	uint64_t inv_cfg                      : 1;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_vdecx_tc_error_reg_s      cnf75xx;
};
typedef union cvmx_vdecx_tc_error_reg cvmx_vdecx_tc_error_reg_t;

/**
 * cvmx_vdec#_tc_main_reset_reg
 */
union cvmx_vdecx_tc_main_reset_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_main_reset_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t soft_reset                   : 1;  /**< Reset bit. */
#else
	uint64_t soft_reset                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_vdecx_tc_main_reset_reg_s cnf75xx;
};
typedef union cvmx_vdecx_tc_main_reset_reg cvmx_vdecx_tc_main_reset_reg_t;

/**
 * cvmx_vdec#_tc_main_start_reg
 *
 * TC start register.
 *
 */
union cvmx_vdecx_tc_main_start_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_main_start_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< Start bit. */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_vdecx_tc_main_start_reg_s cnf75xx;
};
typedef union cvmx_vdecx_tc_main_start_reg cvmx_vdecx_tc_main_start_reg_t;

/**
 * cvmx_vdec#_tc_mon_reg#
 */
union cvmx_vdecx_tc_mon_regx {
	uint64_t u64;
	struct cvmx_vdecx_tc_mon_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t bus_val                      : 16; /**< Output bus monitoring values. */
#else
	uint64_t bus_val                      : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_vdecx_tc_mon_regx_s       cnf75xx;
};
typedef union cvmx_vdecx_tc_mon_regx cvmx_vdecx_tc_mon_regx_t;

/**
 * cvmx_vdec#_tc_status0_reg
 *
 * This register reports status from the internal decoder core.
 *
 */
union cvmx_vdecx_tc_status0_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_status0_reg_s {
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
	struct cvmx_vdecx_tc_status0_reg_s    cnf75xx;
};
typedef union cvmx_vdecx_tc_status0_reg cvmx_vdecx_tc_status0_reg_t;

/**
 * cvmx_vdec#_tc_status1_reg
 *
 * This register reports additional status of the internal decoder core.
 *
 */
union cvmx_vdecx_tc_status1_reg {
	uint64_t u64;
	struct cvmx_vdecx_tc_status1_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t core_stat                    : 64; /**< N/A */
#else
	uint64_t core_stat                    : 64;
#endif
	} s;
	struct cvmx_vdecx_tc_status1_reg_s    cnf75xx;
};
typedef union cvmx_vdecx_tc_status1_reg cvmx_vdecx_tc_status1_reg_t;

#endif
