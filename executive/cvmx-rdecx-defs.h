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
 * cvmx-rdecx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon rdecx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_RDECX_DEFS_H__
#define __CVMX_RDECX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680050ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3680050ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_CONFIGURATION(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_CONFIGURATION(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3682000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_CONFIGURATION(offset) (CVMX_ADD_IO_SEG(0x00011800B3682000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680000ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800B3680000ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ECC_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ECC_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680060ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ECC_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800B3680060ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ECC_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ECC_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680070ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ECC_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800B3680070ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ECC_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ECC_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680068ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ECC_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3680068ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680008ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B3680008ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ERROR_ENABLE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ERROR_ENABLE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680040ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ERROR_ENABLE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3680040ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_ERROR_SOURCE0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_ERROR_SOURCE0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680030ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_ERROR_SOURCE0(offset) (CVMX_ADD_IO_SEG(0x00011800B3680030ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_SCRATCH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_SCRATCH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680080ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_SCRATCH(offset) (CVMX_ADD_IO_SEG(0x00011800B3680080ull) + ((offset) & 1) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RDECX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RDECX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3680018ull) + ((offset) & 1) * 32768;
}
#else
#define CVMX_RDECX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B3680018ull) + ((offset) & 1) * 32768)
#endif

/**
 * cvmx_rdec#_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_rdecx_bist_status {
	uint64_t u64;
	struct cvmx_rdecx_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t tmgr_status                  : 1;  /**< TMGR memory BIST status. */
#else
	uint64_t tmgr_status                  : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rdecx_bist_status_s       cnf75xx;
};
typedef union cvmx_rdecx_bist_status cvmx_rdecx_bist_status_t;

/**
 * cvmx_rdec#_configuration
 *
 * This register space contains the RDEC job configuration data.
 *
 * Software should not write this register directly, but instead use this
 * format when writing the job configuration section of the job descriptor.
 */
union cvmx_rdecx_configuration {
	uint64_t u64;
	struct cvmx_rdecx_configuration_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_54_63               : 10;
	uint64_t data_byte_order              : 2;  /**< 0: DATA_BYTE_ORDER_MODE_0
                                                         - 1: DATA_BYTE_ORDER_MODE_1
                                                         - 2: DATA_BYTE_ORDER_MODE_2 */
	uint64_t data_bit_order               : 1;  /**< 0: DATA_BIT_MSB_FIRST
                                                         - 1: DATA_BIT_LSB_FIRST */
	uint64_t num_reported_correlations    : 3;  /**< Number of best correlation outputs to be reported. Must be in rage 1-4. */
	uint64_t reserved_46_47               : 2;
	uint64_t ue_dsp_report_size           : 5;  /**< The size of per UE DSP report in 128-bit words. */
	uint64_t rb_dsp_report_size           : 5;  /**< The size of per RB DSP report in 128-bit words. */
	uint64_t num_words_per_task_cnfg      : 4;  /**< Number of 128-bit words per task configuration. Must be set to 1. */
	uint64_t reserved_26_31               : 6;
	uint64_t num_data_words               : 10; /**< Number of 128-bit data words for this job. This should be the total
                                                         size of all DSP reports and input LLRs read into the internal buffer.
                                                         The total number of read DMA words for the job must equal
                                                         NUM_DATA_WORDS + NUM_BUNDLED_TASKS. */
	uint64_t reserved_9_15                : 7;
	uint64_t num_bundled_tasks            : 9;  /**< Number of tasks that are bundled in one job. */
#else
	uint64_t num_bundled_tasks            : 9;
	uint64_t reserved_9_15                : 7;
	uint64_t num_data_words               : 10;
	uint64_t reserved_26_31               : 6;
	uint64_t num_words_per_task_cnfg      : 4;
	uint64_t rb_dsp_report_size           : 5;
	uint64_t ue_dsp_report_size           : 5;
	uint64_t reserved_46_47               : 2;
	uint64_t num_reported_correlations    : 3;
	uint64_t data_bit_order               : 1;
	uint64_t data_byte_order              : 2;
	uint64_t reserved_54_63               : 10;
#endif
	} s;
	struct cvmx_rdecx_configuration_s     cnf75xx;
};
typedef union cvmx_rdecx_configuration cvmx_rdecx_configuration_t;

/**
 * cvmx_rdec#_control
 *
 * This register is used to start RDEC HAB processing
 *
 */
union cvmx_rdecx_control {
	uint64_t u64;
	struct cvmx_rdecx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid                        : 16; /**< Job ID0 */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< A write of 1 will start the HAB using the configuration in
                                                         RDEC()_CONFIGURATION. This bit auto-clears, and it is ignored if the
                                                         HAB status is busy (RDEC()_STATUS[STATUS0] = 1). */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rdecx_control_s           cnf75xx;
};
typedef union cvmx_rdecx_control cvmx_rdecx_control_t;

/**
 * cvmx_rdec#_ecc_ctrl
 *
 * This register controls ECC parameters.
 *
 */
union cvmx_rdecx_ecc_ctrl {
	uint64_t u64;
	struct cvmx_rdecx_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t tmgr_flip_syn                : 2;  /**< TMGR memory syndrome flip bits. */
	uint64_t tmgr_cor_dis                 : 1;  /**< TMGR memory ECC correction disable. */
#else
	uint64_t tmgr_cor_dis                 : 1;
	uint64_t tmgr_flip_syn                : 2;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_rdecx_ecc_ctrl_s          cnf75xx;
};
typedef union cvmx_rdecx_ecc_ctrl cvmx_rdecx_ecc_ctrl_t;

/**
 * cvmx_rdec#_ecc_enable
 *
 * This register enables ECC error reporting.
 *
 */
union cvmx_rdecx_ecc_enable {
	uint64_t u64;
	struct cvmx_rdecx_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t tmgr_dbe                     : 1;  /**< TMGR memory double bit error enable. */
	uint64_t tmgr_sbe                     : 1;  /**< TMGR memory single bit error enable. */
#else
	uint64_t tmgr_sbe                     : 1;
	uint64_t tmgr_dbe                     : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_rdecx_ecc_enable_s        cnf75xx;
};
typedef union cvmx_rdecx_ecc_enable cvmx_rdecx_ecc_enable_t;

/**
 * cvmx_rdec#_ecc_status
 *
 * This register indicates ECC status.
 *
 */
union cvmx_rdecx_ecc_status {
	uint64_t u64;
	struct cvmx_rdecx_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t tmgr_dbe                     : 1;  /**< TMGR memory double bit error */
	uint64_t tmgr_sbe                     : 1;  /**< TMGR memory single bit error */
#else
	uint64_t tmgr_sbe                     : 1;
	uint64_t tmgr_dbe                     : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_rdecx_ecc_status_s        cnf75xx;
};
typedef union cvmx_rdecx_ecc_status cvmx_rdecx_ecc_status_t;

/**
 * cvmx_rdec#_eco
 */
union cvmx_rdecx_eco {
	uint64_t u64;
	struct cvmx_rdecx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rdecx_eco_s               cnf75xx;
};
typedef union cvmx_rdecx_eco cvmx_rdecx_eco_t;

/**
 * cvmx_rdec#_error_enable0
 *
 * This register enables error reporting for RDEC_ERROR_SOURCE0 register.
 *
 */
union cvmx_rdecx_error_enable0 {
	uint64_t u64;
	struct cvmx_rdecx_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t rp0_of_en                    : 1;  /**< Read port 0 overflow enable. */
	uint64_t rp0_uf_en                    : 1;  /**< Read port 0 underflow enable. */
#else
	uint64_t rp0_uf_en                    : 1;
	uint64_t rp0_of_en                    : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_rdecx_error_enable0_s     cnf75xx;
};
typedef union cvmx_rdecx_error_enable0 cvmx_rdecx_error_enable0_t;

/**
 * cvmx_rdec#_error_source0
 *
 * This register reports the source of read DMA overflow/underflow errors.
 *
 */
union cvmx_rdecx_error_source0 {
	uint64_t u64;
	struct cvmx_rdecx_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid                    : 16; /**< Job ID for read port 0 overflow/underflow. */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read port 0 overflow. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read port 0 underflow. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid                    : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rdecx_error_source0_s     cnf75xx;
};
typedef union cvmx_rdecx_error_source0 cvmx_rdecx_error_source0_t;

/**
 * cvmx_rdec#_scratch
 */
union cvmx_rdecx_scratch {
	uint64_t u64;
	struct cvmx_rdecx_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Scratch data */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_rdecx_scratch_s           cnf75xx;
};
typedef union cvmx_rdecx_scratch cvmx_rdecx_scratch_t;

/**
 * cvmx_rdec#_status
 *
 * RDEC Status Register
 *
 */
union cvmx_rdecx_status {
	uint64_t u64;
	struct cvmx_rdecx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< When set, indicates the RDEC HAB is ready to receive next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t status0                      : 1;  /**< When set, indicates the RDEC HAB is busy processing a job. */
#else
	uint64_t status0                      : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_rdecx_status_s            cnf75xx;
};
typedef union cvmx_rdecx_status cvmx_rdecx_status_t;

#endif
