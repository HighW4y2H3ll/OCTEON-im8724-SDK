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
 * cvmx-bbxc-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon bbxc.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_BBXC_DEFS_H__
#define __CVMX_BBXC_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_CONTROL CVMX_BBXC_CONTROL_FUNC()
static inline uint64_t CVMX_BBXC_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3880000ull);
}
#else
#define CVMX_BBXC_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3880000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_ERROR_ENABLE0 CVMX_BBXC_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_BBXC_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3880040ull);
}
#else
#define CVMX_BBXC_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3880040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_ERROR_SOURCE0 CVMX_BBXC_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_BBXC_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3880030ull);
}
#else
#define CVMX_BBXC_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3880030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_STATUS CVMX_BBXC_STATUS_FUNC()
static inline uint64_t CVMX_BBXC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3880018ull);
}
#else
#define CVMX_BBXC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3880018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_TEST0 CVMX_BBXC_TEST0_FUNC()
static inline uint64_t CVMX_BBXC_TEST0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_TEST0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3882000ull);
}
#else
#define CVMX_BBXC_TEST0 (CVMX_ADD_IO_SEG(0x00011800B3882000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_BBXC_TEST1 CVMX_BBXC_TEST1_FUNC()
static inline uint64_t CVMX_BBXC_TEST1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_BBXC_TEST1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3884000ull);
}
#else
#define CVMX_BBXC_TEST1 (CVMX_ADD_IO_SEG(0x00011800B3884000ull))
#endif

/**
 * cvmx_bbxc_control
 *
 * This register is used to start BBXC HAB processing
 *
 */
union cvmx_bbxc_control {
	uint64_t u64;
	struct cvmx_bbxc_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID1 */
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< "'1' = Start the HAB in Slot 1. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (BBXC_STATUS bit 0 = '1')." */
	uint64_t start0                       : 1;  /**< "'1' = Start the HAB in Slot 0. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (BBXC_STATUS bit 0 = '1')." */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_bbxc_control_s            cnf75xx;
};
typedef union cvmx_bbxc_control cvmx_bbxc_control_t;

/**
 * cvmx_bbxc_error_enable0
 *
 * This register enables BBXC error reporting for BBXC_ERROR_SOURCE0 register
 *
 */
union cvmx_bbxc_error_enable0 {
	uint64_t u64;
	struct cvmx_bbxc_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t err_ena                      : 1;  /**< BBXC Error enable */
#else
	uint64_t err_ena                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_bbxc_error_enable0_s      cnf75xx;
};
typedef union cvmx_bbxc_error_enable0 cvmx_bbxc_error_enable0_t;

/**
 * cvmx_bbxc_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_bbxc_error_source0 {
	uint64_t u64;
	struct cvmx_bbxc_error_source0_s {
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
	struct cvmx_bbxc_error_source0_s      cnf75xx;
};
typedef union cvmx_bbxc_error_source0 cvmx_bbxc_error_source0_t;

/**
 * cvmx_bbxc_status
 *
 * BBXC Status Register
 *
 */
union cvmx_bbxc_status {
	uint64_t u64;
	struct cvmx_bbxc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Indicates if the BBXC is ready to accept a job.   '1' = Ready for new
                                                         job, '0' = Busy processing. */
	uint64_t reserved_2_3                 : 2;
	uint64_t status1                      : 1;  /**< Indicates if the BBXC is busy processing a job in Slot 1. '0' = Ready for new
                                                         job, '1' = Busy processing. */
	uint64_t status0                      : 1;  /**< Indicates if the BBXC is busy processing a job in Slot 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t status1                      : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_bbxc_status_s             cnf75xx;
};
typedef union cvmx_bbxc_status cvmx_bbxc_status_t;

/**
 * cvmx_bbxc_test0
 *
 * This is the BBXC test register 0
 *
 */
union cvmx_bbxc_test0 {
	uint64_t u64;
	struct cvmx_bbxc_test0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t test0_field0                 : 64; /**< BBXC Test1 Field0 */
#else
	uint64_t test0_field0                 : 64;
#endif
	} s;
	struct cvmx_bbxc_test0_s              cnf75xx;
};
typedef union cvmx_bbxc_test0 cvmx_bbxc_test0_t;

/**
 * cvmx_bbxc_test1
 *
 * This is the BBXC test register 1
 *
 */
union cvmx_bbxc_test1 {
	uint64_t u64;
	struct cvmx_bbxc_test1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t test1_field0                 : 64; /**< BBXC Test1 Field0 */
#else
	uint64_t test1_field0                 : 64;
#endif
	} s;
	struct cvmx_bbxc_test1_s              cnf75xx;
};
typedef union cvmx_bbxc_test1 cvmx_bbxc_test1_t;

#endif
