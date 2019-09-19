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
 * cvmx-bbxbx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon bbxbx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_BBXBX_DEFS_H__
#define __CVMX_BBXBX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3800000ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3800000ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3800040ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3800040ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3800030ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3800030ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3800018ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3800018ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_TEST0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_TEST0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3801000ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_TEST0(offset) (CVMX_ADD_IO_SEG(0x00011800B3801000ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_TEST1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_TEST1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3802200ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_TEST1(offset) (CVMX_ADD_IO_SEG(0x00011800B3802200ull) + ((offset) & 3) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_BBXBX_TEST2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_BBXBX_TEST2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3802208ull) + ((offset) & 3) * 32768;
}
#else
#define CVMX_BBXBX_TEST2(offset) (CVMX_ADD_IO_SEG(0x00011800B3802208ull) + ((offset) & 3) * 32768)
#endif

/**
 * cvmx_bbxb#_control
 *
 * This register is used to start BBXB HAB processing
 *
 */
union cvmx_bbxbx_control {
	uint64_t u64;
	struct cvmx_bbxbx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID1 */
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< "'1' = Start the HAB in Slot 1. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (BBXB_STATUS bit 0 = '1')." */
	uint64_t start0                       : 1;  /**< "'1' = Start the HAB in Slot 0. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (BBXB_STATUS bit 0 = '1')." */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_bbxbx_control_s           cnf75xx;
};
typedef union cvmx_bbxbx_control cvmx_bbxbx_control_t;

/**
 * cvmx_bbxb#_error_enable0
 *
 * This register enables BBXB error reporting for BBXB_ERROR_SOURCE0 register
 *
 */
union cvmx_bbxbx_error_enable0 {
	uint64_t u64;
	struct cvmx_bbxbx_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t err_ena                      : 1;  /**< BBXB Error enable */
#else
	uint64_t err_ena                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bbxbx_error_enable0_s     cnf75xx;
};
typedef union cvmx_bbxbx_error_enable0 cvmx_bbxbx_error_enable0_t;

/**
 * cvmx_bbxb#_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_bbxbx_error_source0 {
	uint64_t u64;
	struct cvmx_bbxbx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t of_ul_jobid1                 : 16; /**< Overflow/Underflow Job ID for Slot1 */
	uint64_t of_ul_jobid0                 : 16; /**< Overflow/Underflow Job ID for Slot0 */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read Port0 overflow */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read Port0 underflow */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t of_ul_jobid0                 : 16;
	uint64_t of_ul_jobid1                 : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_bbxbx_error_source0_s     cnf75xx;
};
typedef union cvmx_bbxbx_error_source0 cvmx_bbxbx_error_source0_t;

/**
 * cvmx_bbxb#_status
 *
 * BBXB Status Register
 *
 */
union cvmx_bbxbx_status {
	uint64_t u64;
	struct cvmx_bbxbx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Indicates if the BBXB is ready to accept a job.   '1' = Ready for new
                                                         job, '0' = Busy processing. */
	uint64_t reserved_2_3                 : 2;
	uint64_t ignore_field                 : 1;  /**< BBXB Ignore Field */
	uint64_t status0                      : 1;  /**< Indicates if the BBXB is busy processing a job in Slot 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t ignore_field                 : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_bbxbx_status_s            cnf75xx;
};
typedef union cvmx_bbxbx_status cvmx_bbxbx_status_t;

/**
 * cvmx_bbxb#_test0
 *
 * This is the BBXB test regster 0
 *
 */
union cvmx_bbxbx_test0 {
	uint64_t u64;
	struct cvmx_bbxbx_test0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t test0_field0                 : 64; /**< BBXB Test0 Field0 */
#else
	uint64_t test0_field0                 : 64;
#endif
	} s;
	struct cvmx_bbxbx_test0_s             cnf75xx;
};
typedef union cvmx_bbxbx_test0 cvmx_bbxbx_test0_t;

/**
 * cvmx_bbxb#_test1
 *
 * This is the BBXB test register 1
 *
 */
union cvmx_bbxbx_test1 {
	uint64_t u64;
	struct cvmx_bbxbx_test1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t test1_field0                 : 40; /**< BBXB Test1 Field0 */
#else
	uint64_t test1_field0                 : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_bbxbx_test1_s             cnf75xx;
};
typedef union cvmx_bbxbx_test1 cvmx_bbxbx_test1_t;

/**
 * cvmx_bbxb#_test2
 *
 * This is the BBXB test register 2
 *
 */
union cvmx_bbxbx_test2 {
	uint64_t u64;
	struct cvmx_bbxbx_test2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_40_63               : 24;
	uint64_t test2_field0                 : 40; /**< BBXB Test2 Field0 */
#else
	uint64_t test2_field0                 : 40;
	uint64_t reserved_40_63               : 24;
#endif
	} s;
	struct cvmx_bbxbx_test2_s             cnf75xx;
};
typedef union cvmx_bbxbx_test2 cvmx_bbxbx_test2_t;

#endif
