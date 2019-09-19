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
 * cvmx-wtxe-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon wtxe.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_WTXE_DEFS_H__
#define __CVMX_WTXE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_BIST_STATUS CVMX_WTXE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_WTXE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200050ull);
}
#else
#define CVMX_WTXE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3200050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_CONTROL CVMX_WTXE_CONTROL_FUNC()
static inline uint64_t CVMX_WTXE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200000ull);
}
#else
#define CVMX_WTXE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3200000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ECC_CTRL CVMX_WTXE_ECC_CTRL_FUNC()
static inline uint64_t CVMX_WTXE_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200060ull);
}
#else
#define CVMX_WTXE_ECC_CTRL (CVMX_ADD_IO_SEG(0x00011800B3200060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ECC_ENABLE CVMX_WTXE_ECC_ENABLE_FUNC()
static inline uint64_t CVMX_WTXE_ECC_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ECC_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200070ull);
}
#else
#define CVMX_WTXE_ECC_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3200070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ECC_STATUS CVMX_WTXE_ECC_STATUS_FUNC()
static inline uint64_t CVMX_WTXE_ECC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ECC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200068ull);
}
#else
#define CVMX_WTXE_ECC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3200068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ECO CVMX_WTXE_ECO_FUNC()
static inline uint64_t CVMX_WTXE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200008ull);
}
#else
#define CVMX_WTXE_ECO (CVMX_ADD_IO_SEG(0x00011800B3200008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ERROR_ENABLE0 CVMX_WTXE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_WTXE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200040ull);
}
#else
#define CVMX_WTXE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3200040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ERROR_ENABLE1 CVMX_WTXE_ERROR_ENABLE1_FUNC()
static inline uint64_t CVMX_WTXE_ERROR_ENABLE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ERROR_ENABLE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200048ull);
}
#else
#define CVMX_WTXE_ERROR_ENABLE1 (CVMX_ADD_IO_SEG(0x00011800B3200048ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ERROR_SOURCE0 CVMX_WTXE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_WTXE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200030ull);
}
#else
#define CVMX_WTXE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3200030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_ERROR_SOURCE1 CVMX_WTXE_ERROR_SOURCE1_FUNC()
static inline uint64_t CVMX_WTXE_ERROR_SOURCE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_ERROR_SOURCE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200038ull);
}
#else
#define CVMX_WTXE_ERROR_SOURCE1 (CVMX_ADD_IO_SEG(0x00011800B3200038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_JCFG CVMX_WTXE_JCFG_FUNC()
static inline uint64_t CVMX_WTXE_JCFG_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_JCFG not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3202000ull);
}
#else
#define CVMX_WTXE_JCFG (CVMX_ADD_IO_SEG(0x00011800B3202000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WTXE_STATUS CVMX_WTXE_STATUS_FUNC()
static inline uint64_t CVMX_WTXE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WTXE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3200018ull);
}
#else
#define CVMX_WTXE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3200018ull))
#endif

/**
 * cvmx_wtxe_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wtxe_bist_status {
	uint64_t u64;
	struct cvmx_wtxe_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t obuf_data1_status            : 1;  /**< Output Buffer Data1 BIST Status */
	uint64_t obuf_data0_status            : 1;  /**< Output Buffer Data0 BIST Status */
	uint64_t obuf_fifo_status             : 1;  /**< Output Buffer FIFO BIST Status */
	uint64_t scr_seqgen3_status           : 1;  /**< Scrambling Code Sequence Generator Memory3 BIST Status */
	uint64_t scr_seqgen2_status           : 1;  /**< Scrambling Code Sequence Generator Memory2 BIST Status */
	uint64_t scr_seqgen1_status           : 1;  /**< Scrambling Code Sequence Generator Memory1 BIST Status */
	uint64_t scr_seqgen0_status           : 1;  /**< Scrambling Code Sequence Generator Memory0 BIST Status */
	uint64_t offset_status                : 1;  /**< Offset Pointer Memory BIST Status */
	uint64_t ch_data3_status              : 1;  /**< Channel Data Memory3 BIST Status */
	uint64_t ch_data2_status              : 1;  /**< Channel Data Memory2 BIST Status */
	uint64_t ch_data1_status              : 1;  /**< Channel Data Memory1 BIST Status */
	uint64_t ch_data0_status              : 1;  /**< Channel Data Memory0 BIST Status */
	uint64_t rdma_fifo_status             : 1;  /**< RDMA FIFO Memory BIST Status */
	uint64_t ch_cfg_status                : 1;  /**< Channel Config Memory BIST Status */
#else
	uint64_t ch_cfg_status                : 1;
	uint64_t rdma_fifo_status             : 1;
	uint64_t ch_data0_status              : 1;
	uint64_t ch_data1_status              : 1;
	uint64_t ch_data2_status              : 1;
	uint64_t ch_data3_status              : 1;
	uint64_t offset_status                : 1;
	uint64_t scr_seqgen0_status           : 1;
	uint64_t scr_seqgen1_status           : 1;
	uint64_t scr_seqgen2_status           : 1;
	uint64_t scr_seqgen3_status           : 1;
	uint64_t obuf_fifo_status             : 1;
	uint64_t obuf_data0_status            : 1;
	uint64_t obuf_data1_status            : 1;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_wtxe_bist_status_s        cnf75xx;
};
typedef union cvmx_wtxe_bist_status cvmx_wtxe_bist_status_t;

/**
 * cvmx_wtxe_control
 *
 * This register is used to start WTXE HAB processing
 *
 */
union cvmx_wtxe_control {
	uint64_t u64;
	struct cvmx_wtxe_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (WTXE_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid0                       : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wtxe_control_s            cnf75xx;
};
typedef union cvmx_wtxe_control cvmx_wtxe_control_t;

/**
 * cvmx_wtxe_ecc_ctrl
 *
 * This register controls ECC parameters.
 *
 */
union cvmx_wtxe_ecc_ctrl {
	uint64_t u64;
	struct cvmx_wtxe_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t ch_cfg_flip_syn              : 2;  /**< Channel Config Memory Syndrome Flip Bits */
	uint64_t ch_cfg_cdis                  : 1;  /**< Channel Config Memory ECC disable */
	uint64_t offset_flip_syn              : 2;  /**< Offset Pointer Memory Syndrome Flip Bits */
	uint64_t offset_cdis                  : 1;  /**< Offset Pointer Memory ECC disable */
#else
	uint64_t offset_cdis                  : 1;
	uint64_t offset_flip_syn              : 2;
	uint64_t ch_cfg_cdis                  : 1;
	uint64_t ch_cfg_flip_syn              : 2;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_wtxe_ecc_ctrl_s           cnf75xx;
};
typedef union cvmx_wtxe_ecc_ctrl cvmx_wtxe_ecc_ctrl_t;

/**
 * cvmx_wtxe_ecc_enable
 *
 * This register enables ECC error reporting.
 *
 */
union cvmx_wtxe_ecc_enable {
	uint64_t u64;
	struct cvmx_wtxe_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t ibuf_chcfg_dbe_enable        : 1;  /**< IBUF Ch Cfg Memory double bit ECC fatal error enable. */
	uint64_t ibuf_chcfg_sbe_enable        : 1;  /**< IBUF Ch Cfg Memory single bit ECC non-fatal error enable. */
	uint64_t ibuf_off_dbe_enable          : 1;  /**< IBUF Offset Memory double bit ECC fatal error enable. */
	uint64_t ibuf_off_sbe_enable          : 1;  /**< IBUF Offset Memory single bit ECC non-fatal error enable. */
#else
	uint64_t ibuf_off_sbe_enable          : 1;
	uint64_t ibuf_off_dbe_enable          : 1;
	uint64_t ibuf_chcfg_sbe_enable        : 1;
	uint64_t ibuf_chcfg_dbe_enable        : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_wtxe_ecc_enable_s         cnf75xx;
};
typedef union cvmx_wtxe_ecc_enable cvmx_wtxe_ecc_enable_t;

/**
 * cvmx_wtxe_ecc_status
 *
 * This register contains ECC error status
 *
 */
union cvmx_wtxe_ecc_status {
	uint64_t u64;
	struct cvmx_wtxe_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t ibuf_chcfg_dbe               : 1;  /**< IBUF Ch Cfg Memory double bit ECC fatal error. */
	uint64_t ibuf_chcfg_sbe               : 1;  /**< IBUF Ch Cfg Memory single bit ECC non-fatal error. */
	uint64_t ibuf_off_dbe                 : 1;  /**< IBUF Offset Memory double bit ECC fatal error. */
	uint64_t ibuf_off_sbe                 : 1;  /**< IBUF Offset Memory single bit ECC non-fatal error. */
#else
	uint64_t ibuf_off_sbe                 : 1;
	uint64_t ibuf_off_dbe                 : 1;
	uint64_t ibuf_chcfg_sbe               : 1;
	uint64_t ibuf_chcfg_dbe               : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_wtxe_ecc_status_s         cnf75xx;
};
typedef union cvmx_wtxe_ecc_status cvmx_wtxe_ecc_status_t;

/**
 * cvmx_wtxe_eco
 */
union cvmx_wtxe_eco {
	uint64_t u64;
	struct cvmx_wtxe_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wtxe_eco_s                cnf75xx;
};
typedef union cvmx_wtxe_eco cvmx_wtxe_eco_t;

/**
 * cvmx_wtxe_error_enable0
 *
 * This register enables error reporting for WTXE_ERROR_SOURCE0 register
 *
 */
union cvmx_wtxe_error_enable0 {
	uint64_t u64;
	struct cvmx_wtxe_error_enable0_s {
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
	struct cvmx_wtxe_error_enable0_s      cnf75xx;
};
typedef union cvmx_wtxe_error_enable0 cvmx_wtxe_error_enable0_t;

/**
 * cvmx_wtxe_error_enable1
 *
 * This register enables error reporting for WTXE_ERROR_SOURCE1 register
 *
 */
union cvmx_wtxe_error_enable1 {
	uint64_t u64;
	struct cvmx_wtxe_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t csr_cdt_err_enable           : 1;  /**< CSR credit fatal error enable. */
#else
	uint64_t csr_cdt_err_enable           : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_wtxe_error_enable1_s      cnf75xx;
};
typedef union cvmx_wtxe_error_enable1 cvmx_wtxe_error_enable1_t;

/**
 * cvmx_wtxe_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_wtxe_error_source0 {
	uint64_t u64;
	struct cvmx_wtxe_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID for the job that caused either an underflow or overflow on port 0. */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Set to 1 when an overflow occurs on port 0. Cleared by a write to one. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Set to 1 when an underflow occurs on port 0. Cleared by a write to one. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid_ufof               : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wtxe_error_source0_s      cnf75xx;
};
typedef union cvmx_wtxe_error_source0 cvmx_wtxe_error_source0_t;

/**
 * cvmx_wtxe_error_source1
 *
 * This register contains error source information.
 *
 */
union cvmx_wtxe_error_source1 {
	uint64_t u64;
	struct cvmx_wtxe_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t csr_cdt                      : 1;  /**< CSR credit fatal error.  MHBW sent more than 8 outstanding requests. */
#else
	uint64_t csr_cdt                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_wtxe_error_source1_s      cnf75xx;
};
typedef union cvmx_wtxe_error_source1 cvmx_wtxe_error_source1_t;

/**
 * cvmx_wtxe_jcfg
 *
 * This register space contains the WTXE job configuration data.
 *
 */
union cvmx_wtxe_jcfg {
	uint64_t u64;
	struct cvmx_wtxe_jcfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t num_carriers                 : 2;  /**< The number of active carriers minus one in current job.
                                                         0 : 1 carrier enabled.  Carrier config sent on read DMA word 0
                                                         1 : 2 carriers enabled.  Carrier config sent on read DMA words 0 and 1
                                                         2 : 3 carriers enabled.  Carrier config sent on read DMA words 0 to 2
                                                         3 : 4 carriers enabled.  Carrier config sent on read DMA words 0 to 3 */
	uint64_t reserved_9_15                : 7;
	uint64_t num_channels                 : 9;  /**< The number of channels for which channel configuration will be received
                                                         through the Read DMA interface.  It does not necessarily indicate the
                                                         number of active channels as a channel configuration may be marked as
                                                         invalid. Legal values are 1 to 384. */
#else
	uint64_t num_channels                 : 9;
	uint64_t reserved_9_15                : 7;
	uint64_t num_carriers                 : 2;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_wtxe_jcfg_s               cnf75xx;
};
typedef union cvmx_wtxe_jcfg cvmx_wtxe_jcfg_t;

/**
 * cvmx_wtxe_status
 *
 * WTXE Status Register
 *
 */
union cvmx_wtxe_status {
	uint64_t u64;
	struct cvmx_wtxe_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t busy                         : 1;  /**< Indicates if the WTXE is busy processing a job with CONFIGURATION 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t busy                         : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wtxe_status_s             cnf75xx;
};
typedef union cvmx_wtxe_status cvmx_wtxe_status_t;

#endif
