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
 * cvmx-wrce-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon wrce.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_WRCE_DEFS_H__
#define __CVMX_WRCE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_BIST_STATUS CVMX_WRCE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_WRCE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380050ull);
}
#else
#define CVMX_WRCE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3380050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_CONTROL CVMX_WRCE_CONTROL_FUNC()
static inline uint64_t CVMX_WRCE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380000ull);
}
#else
#define CVMX_WRCE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3380000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_ECO CVMX_WRCE_ECO_FUNC()
static inline uint64_t CVMX_WRCE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380008ull);
}
#else
#define CVMX_WRCE_ECO (CVMX_ADD_IO_SEG(0x00011800B3380008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_ERROR_ENABLE0 CVMX_WRCE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_WRCE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380040ull);
}
#else
#define CVMX_WRCE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3380040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_ERROR_SOURCE0 CVMX_WRCE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_WRCE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380030ull);
}
#else
#define CVMX_WRCE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3380030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_INIT_CFG CVMX_WRCE_INIT_CFG_FUNC()
static inline uint64_t CVMX_WRCE_INIT_CFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_INIT_CFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3382008ull);
}
#else
#define CVMX_WRCE_INIT_CFG (CVMX_ADD_IO_SEG(0x00011800B3382008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_JCFG CVMX_WRCE_JCFG_FUNC()
static inline uint64_t CVMX_WRCE_JCFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_JCFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3382000ull);
}
#else
#define CVMX_WRCE_JCFG (CVMX_ADD_IO_SEG(0x00011800B3382000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_SCRATCH CVMX_WRCE_SCRATCH_FUNC()
static inline uint64_t CVMX_WRCE_SCRATCH_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_SCRATCH not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380080ull);
}
#else
#define CVMX_WRCE_SCRATCH (CVMX_ADD_IO_SEG(0x00011800B3380080ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_STATUS CVMX_WRCE_STATUS_FUNC()
static inline uint64_t CVMX_WRCE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3380018ull);
}
#else
#define CVMX_WRCE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3380018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRCE_UNEXPECTED_COND CVMX_WRCE_UNEXPECTED_COND_FUNC()
static inline uint64_t CVMX_WRCE_UNEXPECTED_COND_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRCE_UNEXPECTED_COND not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3382010ull);
}
#else
#define CVMX_WRCE_UNEXPECTED_COND (CVMX_ADD_IO_SEG(0x00011800B3382010ull))
#endif

/**
 * cvmx_wrce_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wrce_bist_status {
	uint64_t u64;
	struct cvmx_wrce_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t db1_status                   : 1;  /**< No Memory BIST */
	uint64_t db0_status                   : 1;  /**< No Memory BIST */
	uint64_t cbm_status                   : 1;  /**< No Memory BIST */
	uint64_t cb_status                    : 1;  /**< No Memory BIST */
#else
	uint64_t cb_status                    : 1;
	uint64_t cbm_status                   : 1;
	uint64_t db0_status                   : 1;
	uint64_t db1_status                   : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_wrce_bist_status_s        cnf75xx;
};
typedef union cvmx_wrce_bist_status cvmx_wrce_bist_status_t;

/**
 * cvmx_wrce_control
 *
 * This register is used to start WRCE HAB processing
 *
 */
union cvmx_wrce_control {
	uint64_t u64;
	struct cvmx_wrce_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid                        : 16; /**< Job ID */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (WRCE_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrce_control_s            cnf75xx;
};
typedef union cvmx_wrce_control cvmx_wrce_control_t;

/**
 * cvmx_wrce_eco
 */
union cvmx_wrce_eco {
	uint64_t u64;
	struct cvmx_wrce_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrce_eco_s                cnf75xx;
};
typedef union cvmx_wrce_eco cvmx_wrce_eco_t;

/**
 * cvmx_wrce_error_enable0
 *
 * This register enables error reporting for WRCE_ERROR_SOURCE0 register
 *
 */
union cvmx_wrce_error_enable0 {
	uint64_t u64;
	struct cvmx_wrce_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t rp0_of_err_enable            : 1;  /**< Read Port0 overflow error enable. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf_err_enable            : 1;  /**< Read Port0 underflow error enable. */
#else
	uint64_t rp0_uf_err_enable            : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of_err_enable            : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wrce_error_enable0_s      cnf75xx;
};
typedef union cvmx_wrce_error_enable0 cvmx_wrce_error_enable0_t;

/**
 * cvmx_wrce_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_wrce_error_source0 {
	uint64_t u64;
	struct cvmx_wrce_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID for Read Port 0 Overflow/Underflow */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read Port0 overflow */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read Port0 underflow */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid_ufof               : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrce_error_source0_s      cnf75xx;
};
typedef union cvmx_wrce_error_source0 cvmx_wrce_error_source0_t;

/**
 * cvmx_wrce_init_cfg
 *
 * This register space contains the WRCE initial configuration data.
 *
 */
union cvmx_wrce_init_cfg {
	uint64_t u64;
	struct cvmx_wrce_init_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t delta_phase                  : 3;  /**< Phase shift between Early, On-Time and On-Time, Late samples expressed in multiples of
                                                         1/8th of a chip, */
#else
	uint64_t delta_phase                  : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_wrce_init_cfg_s           cnf75xx;
};
typedef union cvmx_wrce_init_cfg cvmx_wrce_init_cfg_t;

/**
 * cvmx_wrce_jcfg
 *
 * This register space contains the WRCE job configuration data.
 *
 */
union cvmx_wrce_jcfg {
	uint64_t u64;
	struct cvmx_wrce_jcfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t scale_1                      : 2;  /**< Number of bits to right shift antenna 1 data before storing in DB1. Valid values are 0, 1, 2 */
	uint64_t reserved_26_31               : 6;
	uint64_t scale_0                      : 2;  /**< Number of bits to right shift antenna 0 data before storing in DB0. Valid values are 0, 1, 2 */
	uint64_t reserved_17_23               : 7;
	uint64_t dc_bias_rmv_1                : 1;  /**< For the output of 2x+1 unit of antenna 1. Set output LSB if bit is set */
	uint64_t reserved_9_15                : 7;
	uint64_t dc_bias_rmv_0                : 1;  /**< For the output of 2x+1 unit of antenna 0. Set output LSB if bit is set */
	uint64_t reserved_6_7                 : 2;
	uint64_t num_user_entry               : 6;  /**< Total number of user entries to be read in the configuration table. */
#else
	uint64_t num_user_entry               : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t dc_bias_rmv_0                : 1;
	uint64_t reserved_9_15                : 7;
	uint64_t dc_bias_rmv_1                : 1;
	uint64_t reserved_17_23               : 7;
	uint64_t scale_0                      : 2;
	uint64_t reserved_26_31               : 6;
	uint64_t scale_1                      : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_wrce_jcfg_s               cnf75xx;
};
typedef union cvmx_wrce_jcfg cvmx_wrce_jcfg_t;

/**
 * cvmx_wrce_scratch
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wrce_scratch {
	uint64_t u64;
	struct cvmx_wrce_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_0_63                : 64;
#else
	uint64_t reserved_0_63                : 64;
#endif
	} s;
	struct cvmx_wrce_scratch_s            cnf75xx;
};
typedef union cvmx_wrce_scratch cvmx_wrce_scratch_t;

/**
 * cvmx_wrce_status
 *
 * WRCE Status Register
 *
 */
union cvmx_wrce_status {
	uint64_t u64;
	struct cvmx_wrce_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t status0                      : 1;  /**< Indicates if the WRCE is busy processing a job. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wrce_status_s             cnf75xx;
};
typedef union cvmx_wrce_status cvmx_wrce_status_t;

/**
 * cvmx_wrce_unexpected_cond
 *
 * This register space contains counters for each unexpected condition
 *
 */
union cvmx_wrce_unexpected_cond {
	uint64_t u64;
	struct cvmx_wrce_unexpected_cond_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t zero_user_count              : 4;  /**< Number of jobs submitted with a zero user count specification.
                                                         Counter saturates at 15 and is readable and re-settable by software. */
#else
	uint64_t zero_user_count              : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_wrce_unexpected_cond_s    cnf75xx;
};
typedef union cvmx_wrce_unexpected_cond cvmx_wrce_unexpected_cond_t;

#endif
