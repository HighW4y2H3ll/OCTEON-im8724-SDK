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
 * cvmx-wpse-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon wpse.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_WPSE_DEFS_H__
#define __CVMX_WPSE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_BIST_STATUS CVMX_WPSE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_WPSE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100050ull);
}
#else
#define CVMX_WPSE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3100050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_CONTROL CVMX_WPSE_CONTROL_FUNC()
static inline uint64_t CVMX_WPSE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100000ull);
}
#else
#define CVMX_WPSE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3100000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ECC_CTRL CVMX_WPSE_ECC_CTRL_FUNC()
static inline uint64_t CVMX_WPSE_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100060ull);
}
#else
#define CVMX_WPSE_ECC_CTRL (CVMX_ADD_IO_SEG(0x00011800B3100060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ECC_ENABLE CVMX_WPSE_ECC_ENABLE_FUNC()
static inline uint64_t CVMX_WPSE_ECC_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ECC_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100070ull);
}
#else
#define CVMX_WPSE_ECC_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3100070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ECC_STATUS CVMX_WPSE_ECC_STATUS_FUNC()
static inline uint64_t CVMX_WPSE_ECC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ECC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100068ull);
}
#else
#define CVMX_WPSE_ECC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3100068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ECO CVMX_WPSE_ECO_FUNC()
static inline uint64_t CVMX_WPSE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100008ull);
}
#else
#define CVMX_WPSE_ECO (CVMX_ADD_IO_SEG(0x00011800B3100008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ERROR_ENABLE0 CVMX_WPSE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_WPSE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100040ull);
}
#else
#define CVMX_WPSE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3100040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_ERROR_SOURCE0 CVMX_WPSE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_WPSE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100030ull);
}
#else
#define CVMX_WPSE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3100030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_JCFG CVMX_WPSE_JCFG_FUNC()
static inline uint64_t CVMX_WPSE_JCFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_JCFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3102000ull);
}
#else
#define CVMX_WPSE_JCFG (CVMX_ADD_IO_SEG(0x00011800B3102000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WPSE_STATUS CVMX_WPSE_STATUS_FUNC()
static inline uint64_t CVMX_WPSE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WPSE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3100018ull);
}
#else
#define CVMX_WPSE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3100018ull))
#endif

/**
 * cvmx_wpse_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wpse_bist_status {
	uint64_t u64;
	struct cvmx_wpse_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t bist_status                  : 5;  /**< BIST Status:
                                                         5..1 ncar_bist_status, // 5:1
                                                         0    smbf_bist_status  // 0 */
#else
	uint64_t bist_status                  : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wpse_bist_status_s        cnf75xx;
};
typedef union cvmx_wpse_bist_status cvmx_wpse_bist_status_t;

/**
 * cvmx_wpse_control
 *
 * This register is used to start WPSE HAB processing
 *
 */
union cvmx_wpse_control {
	uint64_t u64;
	struct cvmx_wpse_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid                        : 16; /**< Job ID */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (WPSE_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wpse_control_s            cnf75xx;
};
typedef union cvmx_wpse_control cvmx_wpse_control_t;

/**
 * cvmx_wpse_ecc_ctrl
 *
 * This register controls ECC parameters.
 *
 */
union cvmx_wpse_ecc_ctrl {
	uint64_t u64;
	struct cvmx_wpse_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_36_63               : 28;
	uint64_t cdis                         : 4;  /**< NCAR Memory ECC disable for NCAR[eng3..eng0] */
	uint64_t reserved_20_31               : 12;
	uint64_t flip1                        : 4;  /**< NCAR Flip Syndrome [1] for NCAR[eng3..eng0] */
	uint64_t reserved_4_15                : 12;
	uint64_t flip0                        : 4;  /**< NCAR Flip Syndrome [0] for NCAR[eng3..eng0] */
#else
	uint64_t flip0                        : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t flip1                        : 4;
	uint64_t reserved_20_31               : 12;
	uint64_t cdis                         : 4;
	uint64_t reserved_36_63               : 28;
#endif
	} s;
	struct cvmx_wpse_ecc_ctrl_s           cnf75xx;
};
typedef union cvmx_wpse_ecc_ctrl cvmx_wpse_ecc_ctrl_t;

/**
 * cvmx_wpse_ecc_enable
 *
 * This register enables error reporting for WPSE_ECC_STATUS register
 *
 */
union cvmx_wpse_ecc_enable {
	uint64_t u64;
	struct cvmx_wpse_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dbe_enable                   : 4;  /**< NCAR Uncorrrecctable Memory Error Enable for NCAR[engine] */
	uint64_t reserved_4_15                : 12;
	uint64_t sbe_enable                   : 4;  /**< NCAR Single Bit Memory Error Enable for NCAR[engine] */
#else
	uint64_t sbe_enable                   : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t dbe_enable                   : 4;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_wpse_ecc_enable_s         cnf75xx;
};
typedef union cvmx_wpse_ecc_enable cvmx_wpse_ecc_enable_t;

/**
 * cvmx_wpse_ecc_status
 *
 * This register indicates ECC status.
 *
 */
union cvmx_wpse_ecc_status {
	uint64_t u64;
	struct cvmx_wpse_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dbe                          : 4;  /**< NCAR uncorrrecctable Memory Error for NCAR[engine] */
	uint64_t reserved_4_15                : 12;
	uint64_t sbe                          : 4;  /**< NCAR Single  Bit Memory Error for NCAR[engine] */
#else
	uint64_t sbe                          : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t dbe                          : 4;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_wpse_ecc_status_s         cnf75xx;
};
typedef union cvmx_wpse_ecc_status cvmx_wpse_ecc_status_t;

/**
 * cvmx_wpse_eco
 */
union cvmx_wpse_eco {
	uint64_t u64;
	struct cvmx_wpse_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wpse_eco_s                cnf75xx;
};
typedef union cvmx_wpse_eco cvmx_wpse_eco_t;

/**
 * cvmx_wpse_error_enable0
 *
 * This register enables error reporting for WPSE_ERROR_SOURCE0 register
 *
 */
union cvmx_wpse_error_enable0 {
	uint64_t u64;
	struct cvmx_wpse_error_enable0_s {
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
	struct cvmx_wpse_error_enable0_s      cnf75xx;
};
typedef union cvmx_wpse_error_enable0 cvmx_wpse_error_enable0_t;

/**
 * cvmx_wpse_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_wpse_error_source0 {
	uint64_t u64;
	struct cvmx_wpse_error_source0_s {
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
	struct cvmx_wpse_error_source0_s      cnf75xx;
};
typedef union cvmx_wpse_error_source0 cvmx_wpse_error_source0_t;

/**
 * cvmx_wpse_jcfg
 *
 * This register space contains the WPSE job configuration data.
 *
 */
union cvmx_wpse_jcfg {
	uint64_t u64;
	struct cvmx_wpse_jcfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_38_63               : 26;
	uint64_t num_links                    : 6;  /**< Number of Links in current job */
	uint64_t reserved_15_31               : 17;
	uint64_t ant1_scale                   : 3;  /**< Input sample scaling.  Right shift samples by SCALE before saturate. */
	uint64_t reserved_9_11                : 3;
	uint64_t ant1_dc_bias_rmv             : 1;  /**< Buffer0 DC offset
                                                         - 0: 2x
                                                         - 1: 2x+1 */
	uint64_t reserved_7_7                 : 1;
	uint64_t ant0_scale                   : 3;  /**< Input sample scaling.  Right shift samples by SCALE before saturate. */
	uint64_t reserved_1_3                 : 3;
	uint64_t ant0_dc_bias_rmv             : 1;  /**< Buffer0 DC offset
                                                         - 0: 2x
                                                         - 1: 2x+1 */
#else
	uint64_t ant0_dc_bias_rmv             : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ant0_scale                   : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t ant1_dc_bias_rmv             : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t ant1_scale                   : 3;
	uint64_t reserved_15_31               : 17;
	uint64_t num_links                    : 6;
	uint64_t reserved_38_63               : 26;
#endif
	} s;
	struct cvmx_wpse_jcfg_s               cnf75xx;
};
typedef union cvmx_wpse_jcfg cvmx_wpse_jcfg_t;

/**
 * cvmx_wpse_status
 *
 * WPSE Status Register
 *
 */
union cvmx_wpse_status {
	uint64_t u64;
	struct cvmx_wpse_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t busy                         : 1;  /**< Indicates if the WPSE is busy processing a job. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t busy                         : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wpse_status_s             cnf75xx;
};
typedef union cvmx_wpse_status cvmx_wpse_status_t;

#endif
