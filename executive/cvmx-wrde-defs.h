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
 * cvmx-wrde-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon wrde.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_WRDE_DEFS_H__
#define __CVMX_WRDE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_BIST_STATUS CVMX_WRDE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_WRDE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580050ull);
}
#else
#define CVMX_WRDE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3580050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_CONTROL CVMX_WRDE_CONTROL_FUNC()
static inline uint64_t CVMX_WRDE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580000ull);
}
#else
#define CVMX_WRDE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3580000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ECC_CTRL CVMX_WRDE_ECC_CTRL_FUNC()
static inline uint64_t CVMX_WRDE_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580060ull);
}
#else
#define CVMX_WRDE_ECC_CTRL (CVMX_ADD_IO_SEG(0x00011800B3580060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ECC_ENABLE CVMX_WRDE_ECC_ENABLE_FUNC()
static inline uint64_t CVMX_WRDE_ECC_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ECC_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580070ull);
}
#else
#define CVMX_WRDE_ECC_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3580070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ECC_STATUS CVMX_WRDE_ECC_STATUS_FUNC()
static inline uint64_t CVMX_WRDE_ECC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ECC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580068ull);
}
#else
#define CVMX_WRDE_ECC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3580068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ECO CVMX_WRDE_ECO_FUNC()
static inline uint64_t CVMX_WRDE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580008ull);
}
#else
#define CVMX_WRDE_ECO (CVMX_ADD_IO_SEG(0x00011800B3580008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ERROR_ENABLE0 CVMX_WRDE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_WRDE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580040ull);
}
#else
#define CVMX_WRDE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3580040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_ERROR_SOURCE0 CVMX_WRDE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_WRDE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580030ull);
}
#else
#define CVMX_WRDE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3580030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_JCFG CVMX_WRDE_JCFG_FUNC()
static inline uint64_t CVMX_WRDE_JCFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_JCFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3582000ull);
}
#else
#define CVMX_WRDE_JCFG (CVMX_ADD_IO_SEG(0x00011800B3582000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRDE_STATUS CVMX_WRDE_STATUS_FUNC()
static inline uint64_t CVMX_WRDE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRDE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3580018ull);
}
#else
#define CVMX_WRDE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3580018ull))
#endif

/**
 * cvmx_wrde_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wrde_bist_status {
	uint64_t u64;
	struct cvmx_wrde_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t bist_status                  : 10; /**< BIST Status:
                                                         ucbf_status, // 9..8  [uppr:lowr]
                                                         sabf_status, // 7
                                                         chbf_status, // 6
                                                         smbf_status // 5..0 [2:0][1:0] */
#else
	uint64_t bist_status                  : 10;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_wrde_bist_status_s        cnf75xx;
};
typedef union cvmx_wrde_bist_status cvmx_wrde_bist_status_t;

/**
 * cvmx_wrde_control
 *
 * This register is used to start WRDE HAB processing
 *
 */
union cvmx_wrde_control {
	uint64_t u64;
	struct cvmx_wrde_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid                        : 16; /**< Job ID0 */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (WRDE_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrde_control_s            cnf75xx;
};
typedef union cvmx_wrde_control cvmx_wrde_control_t;

/**
 * cvmx_wrde_ecc_ctrl
 *
 * This register controls ECC parameters.
 *
 */
union cvmx_wrde_ecc_ctrl {
	uint64_t u64;
	struct cvmx_wrde_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t cdis                         : 2;  /**< UCBF[1:0] Memory ECC disable */
	uint64_t reserved_18_31               : 14;
	uint64_t flip1                        : 2;  /**< UCBF[1:0] Memory Syndrome Flip Bit 1 */
	uint64_t reserved_2_15                : 14;
	uint64_t flip0                        : 2;  /**< UCBF[1:0] Memory Syndrome Flip Bit 0 */
#else
	uint64_t flip0                        : 2;
	uint64_t reserved_2_15                : 14;
	uint64_t flip1                        : 2;
	uint64_t reserved_18_31               : 14;
	uint64_t cdis                         : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_wrde_ecc_ctrl_s           cnf75xx;
};
typedef union cvmx_wrde_ecc_ctrl cvmx_wrde_ecc_ctrl_t;

/**
 * cvmx_wrde_ecc_enable
 *
 * This register enables error reporting for WRDE_ECC_STATUS register
 *
 */
union cvmx_wrde_ecc_enable {
	uint64_t u64;
	struct cvmx_wrde_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t dbe_enable                   : 2;  /**< UCBF Uncorrectable Memory Error Enable for UCBF[uppr..lowr] */
	uint64_t reserved_2_15                : 14;
	uint64_t sbe_enable                   : 2;  /**< UCBF Single Bit Memory Error Enable for UCBF[uppr..lowr] */
#else
	uint64_t sbe_enable                   : 2;
	uint64_t reserved_2_15                : 14;
	uint64_t dbe_enable                   : 2;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_wrde_ecc_enable_s         cnf75xx;
};
typedef union cvmx_wrde_ecc_enable cvmx_wrde_ecc_enable_t;

/**
 * cvmx_wrde_ecc_status
 *
 * This register indicates ECC status.
 *
 */
union cvmx_wrde_ecc_status {
	uint64_t u64;
	struct cvmx_wrde_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t dbe                          : 2;  /**< UCBF uncorrectable Memory Error for UCBF[uppr..lowr] */
	uint64_t reserved_2_15                : 14;
	uint64_t sbe                          : 2;  /**< UCBF  Single Bit Memory Error for UCBF[uppr..lowr] */
#else
	uint64_t sbe                          : 2;
	uint64_t reserved_2_15                : 14;
	uint64_t dbe                          : 2;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_wrde_ecc_status_s         cnf75xx;
};
typedef union cvmx_wrde_ecc_status cvmx_wrde_ecc_status_t;

/**
 * cvmx_wrde_eco
 */
union cvmx_wrde_eco {
	uint64_t u64;
	struct cvmx_wrde_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrde_eco_s                cnf75xx;
};
typedef union cvmx_wrde_eco cvmx_wrde_eco_t;

/**
 * cvmx_wrde_error_enable0
 *
 * This register enables error reporting for WRDE_ERROR_SOURCE0 register.
 *
 */
union cvmx_wrde_error_enable0 {
	uint64_t u64;
	struct cvmx_wrde_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t rp0_of_err_enable            : 1;  /**< Read port 0 overflow error enable. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf_err_enable            : 1;  /**< Read port 0 underflow error enable. */
#else
	uint64_t rp0_uf_err_enable            : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of_err_enable            : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wrde_error_enable0_s      cnf75xx;
};
typedef union cvmx_wrde_error_enable0 cvmx_wrde_error_enable0_t;

/**
 * cvmx_wrde_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_wrde_error_source0 {
	uint64_t u64;
	struct cvmx_wrde_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID of the job that caused the first read port 0 overflow/underflow.
                                                         If RP0_OF=0 and RP0_UF=0, then the field is the job ID of the last or current
                                                         job. */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read port 0 overflow. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read port 0 underflow. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid_ufof               : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrde_error_source0_s      cnf75xx;
};
typedef union cvmx_wrde_error_source0 cvmx_wrde_error_source0_t;

/**
 * cvmx_wrde_jcfg
 *
 * This register space contains the WRDE job configuration data.
 *
 */
union cvmx_wrde_jcfg {
	uint64_t u64;
	struct cvmx_wrde_jcfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t num_ant                      : 4;  /**< Number of input antennas.  Range 1..8 */
	uint64_t reserved_58_59               : 2;
	uint64_t n_shift_bit                  : 2;  /**< Number of shift bit for CHEST symbol before saving into internal memory, i.e., n bits in
                                                         Sat14(.>>n) block, range: 0, 1, 2 */
	uint64_t reserved_53_55               : 3;
	uint64_t num_slot                     : 5;  /**< Number of slots received from RDMA.  Legal: 3..6 (2ms) or 16..18 (10ms) */
	uint64_t reserved_45_47               : 3;
	uint64_t num_valid_user               : 5;  /**< Total number of users with user_valid = 1 AND ch_enable > 0, i.e., N,  N  K; Defines size
                                                         of each CHEST segment to be read in RD DMA setup (N x 8 symbols) */
	uint64_t reserved_32_39               : 8;
	uint64_t scale                        : 16; /**< Scale input sample data: Sat6(. >>> scale[i][1:0]) for antennas 7..0
                                                         17..16: ant0 scale
                                                         19..18: ant1 scale, etc. */
	uint64_t dc_bias_rmv                  : 8;  /**< For the 2x+1 unit on antenna 0 to 7, add ?1? to the LSB of product;
                                                         - 0: no addition required;
                                                          b8: ant0, b9: ant1,  b10: ant2, b11: ant3, b12: ant4, b13: ant5, b14: ant6, b15: ant7 */
	uint64_t reserved_5_7                 : 3;
	uint64_t num_user_entry               : 5;  /**< Total number of user entries to be read in the configuration table,
                                                         i.e., K. Maximum 16 users;
                                                         Defines number of bytes to be read in configuration RD DMA setup;
                                                         range: 1..16 */
#else
	uint64_t num_user_entry               : 5;
	uint64_t reserved_5_7                 : 3;
	uint64_t dc_bias_rmv                  : 8;
	uint64_t scale                        : 16;
	uint64_t reserved_32_39               : 8;
	uint64_t num_valid_user               : 5;
	uint64_t reserved_45_47               : 3;
	uint64_t num_slot                     : 5;
	uint64_t reserved_53_55               : 3;
	uint64_t n_shift_bit                  : 2;
	uint64_t reserved_58_59               : 2;
	uint64_t num_ant                      : 4;
#endif
	} s;
	struct cvmx_wrde_jcfg_s               cnf75xx;
};
typedef union cvmx_wrde_jcfg cvmx_wrde_jcfg_t;

/**
 * cvmx_wrde_status
 *
 * WRDE Status Register
 *
 */
union cvmx_wrde_status {
	uint64_t u64;
	struct cvmx_wrde_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t busy                         : 1;  /**< Indicates if the WRDE is busy processing a job with CONFIGURATION 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t busy                         : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wrde_status_s             cnf75xx;
};
typedef union cvmx_wrde_status cvmx_wrde_status_t;

#endif
