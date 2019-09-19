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
 * cvmx-dencx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon dencx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_DENCX_DEFS_H__
#define __CVMX_DENCX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480060ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3480060ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3480000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ECC_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ECC_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480050ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ECC_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3480050ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B3480008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3480040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ERROR_ENABLE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ERROR_ENABLE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480048ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ERROR_ENABLE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3480048ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3480030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_ERROR_SOURCE1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_ERROR_SOURCE1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_ERROR_SOURCE1(offset) (CVMX_ADD_IO_SEG(0x00011800B3480038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_HAB_JCFG0_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_HAB_JCFG0_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3482000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_HAB_JCFG0_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3482000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_HAB_JCFG1_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_HAB_JCFG1_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3484000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_HAB_JCFG1_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3484000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_HAB_JCFG2_RAMX_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 255)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_HAB_JCFG2_RAMX_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3486000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_HAB_JCFG2_RAMX_DATA(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3486000ull) + (((offset) & 255) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_JCFG0_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_JCFG0_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480100ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_JCFG0_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3480100ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_JCFG1_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_JCFG1_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480108ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_JCFG1_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3480108ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_JCFG2_ECC_ERROR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_JCFG2_ECC_ERROR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480110ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_JCFG2_ECC_ERROR(offset) (CVMX_ADD_IO_SEG(0x00011800B3480110ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_SCRATCH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_SCRATCH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480080ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_SCRATCH(offset) (CVMX_ADD_IO_SEG(0x00011800B3480080ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3480018ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3480018ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_CONFIG_ERR_FLAGS_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_CONFIG_ERR_FLAGS_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_CONFIG_ERR_FLAGS_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_CONFIG_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 44)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_TC_CONFIG_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3481400ull) + (((offset) & 63) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_TC_CONFIG_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3481400ull) + (((offset) & 63) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_CONTROL_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_CONTROL_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481010ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_CONTROL_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481010ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_ERROR_MASK_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_ERROR_MASK_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_ERROR_MASK_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_ERROR_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_ERROR_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481038ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_ERROR_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481038ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_MAIN_RESET_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_MAIN_RESET_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_MAIN_RESET_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_MAIN_START_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_MAIN_START_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_MAIN_START_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_MON_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_MON_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481300ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_MON_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481300ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_STATIC_EPDCCH_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 65)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_TC_STATIC_EPDCCH_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3481C00ull) + (((offset) & 127) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_TC_STATIC_EPDCCH_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3481C00ull) + (((offset) & 127) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_STATIC_PDCCH_REGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 8)) && ((block_id <= 1))))))
		cvmx_warn("CVMX_DENCX_TC_STATIC_PDCCH_REGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B3481800ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8;
}
#else
#define CVMX_DENCX_TC_STATIC_PDCCH_REGX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B3481800ull) + (((offset) & 15) + ((block_id) & 1) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_STATUS0_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_STATUS0_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481020ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_STATUS0_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481020ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DENCX_TC_STATUS1_REG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_DENCX_TC_STATUS1_REG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3481028ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_DENCX_TC_STATUS1_REG(offset) (CVMX_ADD_IO_SEG(0x00011800B3481028ull) + ((offset) & 1) * 32768)
#endif

/**
 * cvmx_denc#_bist_status
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_dencx_bist_status {
	uint64_t u64;
	struct cvmx_dencx_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_27_63               : 37;
	uint64_t bisr_status                  : 27; /**< BIST status results. */
#else
	uint64_t bisr_status                  : 27;
	uint64_t reserved_27_63               : 37;
#endif
	} s;
	struct cvmx_dencx_bist_status_s       cnf75xx;
};
typedef union cvmx_dencx_bist_status cvmx_dencx_bist_status_t;

/**
 * cvmx_denc#_control
 */
union cvmx_dencx_control {
	uint64_t u64;
	struct cvmx_dencx_control_s {
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
	struct cvmx_dencx_control_s           cnf75xx;
};
typedef union cvmx_dencx_control cvmx_dencx_control_t;

/**
 * cvmx_denc#_ecc_control
 */
union cvmx_dencx_ecc_control {
	uint64_t u64;
	struct cvmx_dencx_ecc_control_s {
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
	struct cvmx_dencx_ecc_control_s       cnf75xx;
};
typedef union cvmx_dencx_ecc_control cvmx_dencx_ecc_control_t;

/**
 * cvmx_denc#_eco
 */
union cvmx_dencx_eco {
	uint64_t u64;
	struct cvmx_dencx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_dencx_eco_s               cnf75xx;
};
typedef union cvmx_dencx_eco cvmx_dencx_eco_t;

/**
 * cvmx_denc#_error_enable0
 *
 * This register enables error reporting for read overflow/underflow errors.
 *
 */
union cvmx_dencx_error_enable0 {
	uint64_t u64;
	struct cvmx_dencx_error_enable0_s {
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
	struct cvmx_dencx_error_enable0_s     cnf75xx;
};
typedef union cvmx_dencx_error_enable0 cvmx_dencx_error_enable0_t;

/**
 * cvmx_denc#_error_enable1
 *
 * This register enables error reporting for ECC errors.
 *
 */
union cvmx_dencx_error_enable1 {
	uint64_t u64;
	struct cvmx_dencx_error_enable1_s {
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
	struct cvmx_dencx_error_enable1_s     cnf75xx;
};
typedef union cvmx_dencx_error_enable1 cvmx_dencx_error_enable1_t;

/**
 * cvmx_denc#_error_source0
 *
 * This register indicates the source of read overflow/underflow errors.
 *
 */
union cvmx_dencx_error_source0 {
	uint64_t u64;
	struct cvmx_dencx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jid                      : 16; /**< Job ID of the job which reported the overflow/underflow error on read
                                                         port 1. */
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
	struct cvmx_dencx_error_source0_s     cnf75xx;
};
typedef union cvmx_dencx_error_source0 cvmx_dencx_error_source0_t;

/**
 * cvmx_denc#_error_source1
 *
 * This register reports the source of ECC errors.
 *
 */
union cvmx_dencx_error_source1 {
	uint64_t u64;
	struct cvmx_dencx_error_source1_s {
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
	struct cvmx_dencx_error_source1_s     cnf75xx;
};
typedef union cvmx_dencx_error_source1 cvmx_dencx_error_source1_t;

/**
 * cvmx_denc#_hab_jcfg0_ram#_data
 *
 * This register range stores the job configuration for slot 0.
 *
 */
union cvmx_dencx_hab_jcfg0_ramx_data {
	uint64_t u64;
	struct cvmx_dencx_hab_jcfg0_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_hab_jcfg0_ramx_data_s cnf75xx;
};
typedef union cvmx_dencx_hab_jcfg0_ramx_data cvmx_dencx_hab_jcfg0_ramx_data_t;

/**
 * cvmx_denc#_hab_jcfg1_ram#_data
 *
 * This register range stores the job configuration for slot 1.
 *
 */
union cvmx_dencx_hab_jcfg1_ramx_data {
	uint64_t u64;
	struct cvmx_dencx_hab_jcfg1_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_hab_jcfg1_ramx_data_s cnf75xx;
};
typedef union cvmx_dencx_hab_jcfg1_ramx_data cvmx_dencx_hab_jcfg1_ramx_data_t;

/**
 * cvmx_denc#_hab_jcfg2_ram#_data
 *
 * This register range stores the job configuration for slot 2.
 *
 */
union cvmx_dencx_hab_jcfg2_ramx_data {
	uint64_t u64;
	struct cvmx_dencx_hab_jcfg2_ramx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< N/A */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_hab_jcfg2_ramx_data_s cnf75xx;
};
typedef union cvmx_dencx_hab_jcfg2_ramx_data cvmx_dencx_hab_jcfg2_ramx_data_t;

/**
 * cvmx_denc#_jcfg0_ecc_error
 *
 * This register contains ECC error information for jcfg0.
 *
 */
union cvmx_dencx_jcfg0_ecc_error {
	uint64_t u64;
	struct cvmx_dencx_jcfg0_ecc_error_s {
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
	struct cvmx_dencx_jcfg0_ecc_error_s   cnf75xx;
};
typedef union cvmx_dencx_jcfg0_ecc_error cvmx_dencx_jcfg0_ecc_error_t;

/**
 * cvmx_denc#_jcfg1_ecc_error
 *
 * This register contains ECC error information for jcfg1.
 *
 */
union cvmx_dencx_jcfg1_ecc_error {
	uint64_t u64;
	struct cvmx_dencx_jcfg1_ecc_error_s {
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
	struct cvmx_dencx_jcfg1_ecc_error_s   cnf75xx;
};
typedef union cvmx_dencx_jcfg1_ecc_error cvmx_dencx_jcfg1_ecc_error_t;

/**
 * cvmx_denc#_jcfg2_ecc_error
 *
 * This register contains ECC error information for jcfg2.
 *
 */
union cvmx_dencx_jcfg2_ecc_error {
	uint64_t u64;
	struct cvmx_dencx_jcfg2_ecc_error_s {
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
	struct cvmx_dencx_jcfg2_ecc_error_s   cnf75xx;
};
typedef union cvmx_dencx_jcfg2_ecc_error cvmx_dencx_jcfg2_ecc_error_t;

/**
 * cvmx_denc#_scratch
 *
 * Scratch register.
 *
 */
union cvmx_dencx_scratch {
	uint64_t u64;
	struct cvmx_dencx_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Scratch data. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_dencx_scratch_s           cnf75xx;
};
typedef union cvmx_dencx_scratch cvmx_dencx_scratch_t;

/**
 * cvmx_denc#_status
 */
union cvmx_dencx_status {
	uint64_t u64;
	struct cvmx_dencx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< When set, the DENC HAB is ready to receive the next job. */
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
	struct cvmx_dencx_status_s            cnf75xx;
};
typedef union cvmx_dencx_status cvmx_dencx_status_t;

/**
 * cvmx_denc#_tc_config_err_flags_reg
 *
 * This register reports task configuration errors that occur when a
 * specified parameter value is outside the acceptable range.  Different
 * bit-mappings are used depending on whether the PHY_MODE and TASK_TYPE, as
 * follows:
 * * PHY_MODE = 0: LTE mode.
 * * PHY_MODE = 1 and TASK_TYPE=8: HS-DSCH mode.
 * * PHY_MODE = 1 and TASK_TYPE=9 or 10: CCTrCH mode.
 * * PHY_MODE = 1 and TASK_TYPE=11: HS-SCCH mode.
 */
union cvmx_dencx_tc_config_err_flags_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_config_err_flags_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_61_63               : 3;
	uint64_t error60                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_15 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error59                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_14 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error58                      : 1;  /**< _ CCTrCH mode: invalid PUNCT_REP.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_13 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error57                      : 1;  /**< _ CCTrCH mode: invalid FILLER, valid range is [0,40].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_12 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error56                      : 1;  /**< _ CCTrCH mode: invalid K, valid range is [0,5114].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_11 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error55                      : 1;  /**< _ CCTrCH mode: invalid NUM_CB, valid range is [0,6].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_10 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error54                      : 1;  /**< _ CCTrCH mode: invalid CRC, valid range is [0,4].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_9 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error53                      : 1;  /**< _ CCTrCH mode: invalid RF_IDX, valid range is [0,7].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_8 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error52                      : 1;  /**< _ CCTrCH mode: invalid TTI, valid range is [0,3].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_7 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error51                      : 1;  /**< _ CCTrCH mode: invalid TB_BITS, valid range is [0,5000].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_6 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error50                      : 1;  /**< _ CCTrCH mode: invalid RATE.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_5 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error49                      : 1;  /**< _ CCTrCH mode: invalid FEC.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_4 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error48                      : 1;  /**< _ CCTrCH mode: invalid M, valid range is [0,96].
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_3 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error47                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_2 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error46                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_1 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error45                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: invalid PATTERN_0 for E-PDCCH task, valid range is [0,4]. */
	uint64_t error44                      : 1;  /**< Reserved. */
	uint64_t error43                      : 1;  /**< Reserved. */
	uint64_t error42                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: E-PDCCH task, invalid IN_BY_O. */
	uint64_t error41                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: E-PDCCH task, invalid IN_BI_O. */
	uint64_t error40                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: E-PDCCH task, invalid NRB, valid range is [1,16]. */
	uint64_t error39                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: E-PDCCH task, invalid EDCI, valid range is [1,64]. */
	uint64_t error38                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error37                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid SYMB_BYTE_ALIGN.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid IN_BY_O. */
	uint64_t error36                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid CONSTELL, valid range is [0,3].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid IN_BI_O. */
	uint64_t error35                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid QM, valid values are [2,4,6].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error34                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid P, valid range is [1,15].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid NRB, valid range is [6,100]. */
	uint64_t error33                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid BYPS_SEG.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid REG_PAT, valid range is [0,8]. */
	uint64_t error32                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid NDATA, valid range is [960,43200].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid BYPS_MAP. */
	uint64_t error31                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 15.
                                                         _ HSDCCH mode: invalid NR, valid range is [0,6].
                                                         _ HSSSCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error30                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 14.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid QUAD, valid range is [9,800]. */
	uint64_t error29                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 13.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid PCELL, valid range is [0,791]. */
	uint64_t error28                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 12.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error27                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 11.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid BYPS_PDCCH */
	uint64_t error26                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 10.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDCCH task, invalid DCI, valid range is [1,88]. */
	uint64_t error25                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 9.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error24                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 8.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error23                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 7.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid FILLER, valid range is [0,min(K-1,63)]. */
	uint64_t error22                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 6.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error21                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 5.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid BYPS_SCRM. */
	uint64_t error20                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 4.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid IN_BY_O. */
	uint64_t error19                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 3.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid IN_BI_O. */
	uint64_t error18                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 2.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid SYMB_BYTE_ALIGN. */
	uint64_t error17                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 1.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid QM, valid values are [2,4,6,8]. */
	uint64_t error16                      : 1;  /**< _ CCTrCH mode: error in parameters for TrCH 0.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid NL, valid range is [1,4]. */
	uint64_t error15                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid GAMMA, valid range is [0,NUM_CB-1]. */
	uint64_t error14                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid PUNCT_REP.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid RM_E. */
	uint64_t error13                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid FILLER, valid range is [0,40].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid G. */
	uint64_t error12                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid K, valid range is [40,5114].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: Reserved. */
	uint64_t error11                      : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: invalid NUM_CB, valid range is [1,9].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid RV_IDX. */
	uint64_t error10                      : 1;  /**< _ CCTrCH mode: invalid BYPS_SEG.
                                                         _ HS-DSCH mode: invalid TB_BITS, valid range is [1,42192].
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid BYPS_RV. */
	uint64_t error9                       : 1;  /**< _ CCTrCH mode: Reserved.
                                                         _ HS-DSCH mode: Reserved.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid BYPS_RM. */
	uint64_t error8                       : 1;  /**< _ CCTrCH mode: invalid U, valid range is [30,19080].
                                                         _ HS-DSCH mode: invalid CRC_METHOD.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid BYPS_CB_CRC. */
	uint64_t error7                       : 1;  /**< _ CCTrCH mode: invalid P, valid range is [1,3].
                                                         _ HS-DSCH mode: invalid CB_MODE.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid CB_MODE. */
	uint64_t error6                       : 1;  /**< _ CCTrCH mode: invalid FLEX.
                                                         _ HS-DSCH mode: invalid BYPS_SCRM.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid BYPS_CRC. */
	uint64_t error5                       : 1;  /**< _ CCTrCH mode: invalid OUT_BY_O.
                                                         _ HS-DSCH mode: invalid BYPS_CRC.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid K, valid range is [40,6144]. */
	uint64_t error4                       : 1;  /**< _ CCTrCH mode: invalid OUT_BI_O.
                                                         _ HS-DSCH mode: invalid OUT_BY_O.
                                                         _ HS-SCCH mode: Reserved.
                                                         _ LTE mode: PDSCH task, invalid PAYLOAD, valid range is [1,1048552]. */
	uint64_t error3                       : 1;  /**< _ CCTrCH mode: invalid IN_BY_O.
                                                         _ HS-DSCH mode: invalid OUT_BI_O.
                                                         _ HS-SCCH mode: invalid OUT_BY_O.
                                                         _ LTE mode: PDSCH task, invalid NUM_CB, valid range is [1,171]. */
	uint64_t error2                       : 1;  /**< _ CCTrCH mode: invalid IN_BI_O.
                                                         _ HS-DSCH mode: invalid IN_BY_O.
                                                         _ HS-SCCH mode: invalid OUT_BI_O.
                                                         _ LTE mode: Reserved. */
	uint64_t error1                       : 1;  /**< _ CCTrCH mode: invalid NUM_TRCH, valid range is [1,16].
                                                         _ HS-DSCH mode: invalid IN_BI_O.
                                                         _ HS-SCCH mode: invalid FORMAT, valid range is [0,7].
                                                         _ LTE mode: invalid payload block size. */
	uint64_t error0                       : 1;  /**< Invalid TASK_TYPE, valid range is [0x0,0xB]. */
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
	uint64_t error59                      : 1;
	uint64_t error60                      : 1;
	uint64_t reserved_61_63               : 3;
#endif
	} s;
	struct cvmx_dencx_tc_config_err_flags_reg_s cnf75xx;
};
typedef union cvmx_dencx_tc_config_err_flags_reg cvmx_dencx_tc_config_err_flags_reg_t;

/**
 * cvmx_denc#_tc_config_reg#
 *
 * TC task config registers.
 *
 */
union cvmx_dencx_tc_config_regx {
	uint64_t u64;
	struct cvmx_dencx_tc_config_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< Config bits. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_tc_config_regx_s    cnf75xx;
};
typedef union cvmx_dencx_tc_config_regx cvmx_dencx_tc_config_regx_t;

/**
 * cvmx_denc#_tc_control_reg
 *
 * This register configures the internal DENC encoder core.
 *
 */
union cvmx_dencx_tc_control_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_control_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t single_task_chnl_en          : 1;  /**< When set to 1, the core will wait until idle before accepting a new job with a different TASK_TYPE. */
	uint64_t single_task_phy_en           : 1;  /**< When set to 1, the core will wait until idle before accepting a new job with a different PHY_MODE. */
	uint64_t single_task_en               : 1;  /**< When set to 1, the core will always wait until idle before
                                                         starting the next task. */
	uint64_t cfg_chk_dis                  : 1;  /**< When set to 1, disables configuration checks. Tasks with
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
	struct cvmx_dencx_tc_control_reg_s    cnf75xx;
};
typedef union cvmx_dencx_tc_control_reg cvmx_dencx_tc_control_reg_t;

/**
 * cvmx_denc#_tc_error_mask_reg
 *
 * This register enables internal encoder errors. Errors reported in
 * DENC()_TC_ERROR_REG will generate an error signal only when the
 * corresponding bit is set in DENC()_TC_ERROR_REG.
 */
union cvmx_dencx_tc_error_mask_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_error_mask_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t err_mask1                    : 4;  /**< Error mask bits. */
	uint64_t reserved_4_4                 : 1;
	uint64_t err_mask0                    : 4;  /**< Error mask bits. */
#else
	uint64_t err_mask0                    : 4;
	uint64_t reserved_4_4                 : 1;
	uint64_t err_mask1                    : 4;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_dencx_tc_error_mask_reg_s cnf75xx;
};
typedef union cvmx_dencx_tc_error_mask_reg cvmx_dencx_tc_error_mask_reg_t;

/**
 * cvmx_denc#_tc_error_reg
 *
 * This register reports various error conditions.
 * Errors are cleared by writing the specific error bits to zero.
 *
 * All errors reported in this register are reported as FATAL errors, and the
 * MHBW registers can be inspected to determine the job tag(s) associated with
 * the error(s).
 */
union cvmx_dencx_tc_error_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_error_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t pdcch_err                    : 1;  /**< PDCCH REG_MASK mapping algorithm error. */
	uint64_t ramf_ra_err                  : 1;  /**< Read access error in REG_MASK memory F - empty. */
	uint64_t ramf_err                     : 1;  /**< Error in REG_MASK memory F. */
	uint64_t inv_cfg                      : 1;  /**< Invalid task configuration. Task aborted. */
	uint64_t reg_mask_err                 : 1;  /**< Error in REG_MASK memory G. */
	uint64_t inv_wr_mask_mem              : 1;  /**< Invalid write to register mask config memory. */
	uint64_t static_wr_act                : 1;  /**< Write to static register when core is active. */
	uint64_t reserved_4_4                 : 1;
	uint64_t ign_read                     : 1;  /**< Ignored read access error -- one already in progress. */
	uint64_t inv_read                     : 1;  /**< Invalid read access error. */
	uint64_t inv_write                    : 1;  /**< Invalid write access error. */
	uint64_t inv_start                    : 1;  /**< Invalid start task error. */
#else
	uint64_t inv_start                    : 1;
	uint64_t inv_write                    : 1;
	uint64_t inv_read                     : 1;
	uint64_t ign_read                     : 1;
	uint64_t reserved_4_4                 : 1;
	uint64_t static_wr_act                : 1;
	uint64_t inv_wr_mask_mem              : 1;
	uint64_t reg_mask_err                 : 1;
	uint64_t inv_cfg                      : 1;
	uint64_t ramf_err                     : 1;
	uint64_t ramf_ra_err                  : 1;
	uint64_t pdcch_err                    : 1;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_dencx_tc_error_reg_s      cnf75xx;
};
typedef union cvmx_dencx_tc_error_reg cvmx_dencx_tc_error_reg_t;

/**
 * cvmx_denc#_tc_main_reset_reg
 *
 * This register provides a reset for the internal encoder core.
 *
 */
union cvmx_dencx_tc_main_reset_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_main_reset_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t soft_reset                   : 1;  /**< Reset bit. */
#else
	uint64_t soft_reset                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_dencx_tc_main_reset_reg_s cnf75xx;
};
typedef union cvmx_dencx_tc_main_reset_reg cvmx_dencx_tc_main_reset_reg_t;

/**
 * cvmx_denc#_tc_main_start_reg
 *
 * TC start register.
 *
 */
union cvmx_dencx_tc_main_start_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_main_start_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< Start bit. */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_dencx_tc_main_start_reg_s cnf75xx;
};
typedef union cvmx_dencx_tc_main_start_reg cvmx_dencx_tc_main_start_reg_t;

/**
 * cvmx_denc#_tc_mon_reg
 *
 * TC mon registers.
 *
 */
union cvmx_dencx_tc_mon_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_mon_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t bus_val                      : 16; /**< Output bus monitoring values. */
#else
	uint64_t bus_val                      : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_dencx_tc_mon_reg_s        cnf75xx;
};
typedef union cvmx_dencx_tc_mon_reg cvmx_dencx_tc_mon_reg_t;

/**
 * cvmx_denc#_tc_static_epdcch_reg#
 *
 * These registers store the EREG pattern table used for E-PDCCH symbol
 * mapping. The default values of these registers are configured for normal
 * usage in accordance with the 3GPP specifications release 11, and there
 * should be no reason to change the default values in these registers.
 *
 * There are 5 EREG patterns corresponding to different CP modes. Each
 * pattern consists of 840 bits, and the 5 patterns are concatenated to form
 * a single vector of 4200 bits, stored in 66 x 64-bit registers.
 *
 * The combined pattern vector can be indexed as follows:
 *
 * _ vector[(840*i + 60*t + 5*(s+1) - 1):(840*i + 60*t + 5*s)] = P(i,t,s)
 *
 * Where i is the CP mode index [0,4], t is the OFDM symbol [0,13], and s is
 * the sub-carrier within the RB [0,11].
 *
 * The 5-bit pattern value selects between one of the 16 EREGS when set to a
 * value in the range [0,15], and values > 15 indicate that no EREG is mapped
 * to that RE.
 *
 * At reset, the E-PDCCH EREG pattern table is loaded with the appropriate
 * patterns for 5 CP modes:
 * * 0x0 = Normal CP.
 * * 0x1 = Special sub-frame 3, 4, 8, 9 with normal CP.
 * * 0x2 = Special sub-frame 1, 2, 6, 7 with normal CP.
 * * 0x3 = Extended sub-frame.
 * * 0x4 = Special sub-frame 1, 2, 3, 5, 6 with extended CP.
 *
 * The DENC_EPDCCH_GEN_CFG_S[PATTERN_[0..15]] fields select one of the 5 CP
 * modes to use for each RB.
 *
 * These registers can only be written when DENC()_TC_STATUS0_REG[IDLE] = 1.
 *
 * Under normal usage, there should be no reason to
 * write these registers, either at start-up or at any other time.
 */
union cvmx_dencx_tc_static_epdcch_regx {
	uint64_t u64;
	struct cvmx_dencx_tc_static_epdcch_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< EREG pattern vector bits. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_tc_static_epdcch_regx_s cnf75xx;
};
typedef union cvmx_dencx_tc_static_epdcch_regx cvmx_dencx_tc_static_epdcch_regx_t;

/**
 * cvmx_denc#_tc_static_pdcch_reg#
 *
 * These regsiters store information used for PDCCH resource mapping.
 *
 * Each entry contains one REG order sequence. Bits <7:0> store the number of
 * REG per RB. Bits <47:8> store the REG order sequence as an array of 20 x
 * 2-bit values (i.e., bits<9:8> stores the control symbol for the first REG,
 * bits<11:10> store the control symbol for the second REG, etc.).
 *
 * These registers can only be written when DENC()_TC_STATUS0_REG[IDLE] = 1.
 *
 * At reset, the REG order table is loaded with 9 default sequences which
 * are sufficient for supporting all functionality specified in 3GPP TS
 * 36.212 release 11. Under normal usage, there should be no reason to
 * write these registers, either at start-up or at any other time.
 */
union cvmx_dencx_tc_static_pdcch_regx {
	uint64_t u64;
	struct cvmx_dencx_tc_static_pdcch_regx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t entry                        : 64; /**< REG order sequence [b]. */
#else
	uint64_t entry                        : 64;
#endif
	} s;
	struct cvmx_dencx_tc_static_pdcch_regx_s cnf75xx;
};
typedef union cvmx_dencx_tc_static_pdcch_regx cvmx_dencx_tc_static_pdcch_regx_t;

/**
 * cvmx_denc#_tc_status0_reg
 *
 * This register reports status from the internal encoder core.
 *
 */
union cvmx_dencx_tc_status0_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_status0_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t core_stat                    : 32; /**< N/A */
	uint64_t reserved_1_31                : 31;
	uint64_t idle                         : 1;  /**< When set to 1, the encoder is idle. */
#else
	uint64_t idle                         : 1;
	uint64_t reserved_1_31                : 31;
	uint64_t core_stat                    : 32;
#endif
	} s;
	struct cvmx_dencx_tc_status0_reg_s    cnf75xx;
};
typedef union cvmx_dencx_tc_status0_reg cvmx_dencx_tc_status0_reg_t;

/**
 * cvmx_denc#_tc_status1_reg
 *
 * TC status 1 register.
 *
 */
union cvmx_dencx_tc_status1_reg {
	uint64_t u64;
	struct cvmx_dencx_tc_status1_reg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t core_stat                    : 64; /**< Core-dependent bits. */
#else
	uint64_t core_stat                    : 64;
#endif
	} s;
	struct cvmx_dencx_tc_status1_reg_s    cnf75xx;
};
typedef union cvmx_dencx_tc_status1_reg cvmx_dencx_tc_status1_reg_t;

#endif
