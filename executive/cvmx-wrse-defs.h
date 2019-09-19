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
 * cvmx-wrse-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon wrse.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_WRSE_DEFS_H__
#define __CVMX_WRSE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_BIST_STATUS CVMX_WRSE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_WRSE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280050ull);
}
#else
#define CVMX_WRSE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3280050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_CONTROL CVMX_WRSE_CONTROL_FUNC()
static inline uint64_t CVMX_WRSE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280000ull);
}
#else
#define CVMX_WRSE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3280000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_ECO CVMX_WRSE_ECO_FUNC()
static inline uint64_t CVMX_WRSE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280008ull);
}
#else
#define CVMX_WRSE_ECO (CVMX_ADD_IO_SEG(0x00011800B3280008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_ERROR_ENABLE0 CVMX_WRSE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_WRSE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280040ull);
}
#else
#define CVMX_WRSE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3280040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_ERROR_SOURCE0 CVMX_WRSE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_WRSE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280030ull);
}
#else
#define CVMX_WRSE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3280030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_WRSE_JCFGX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_WRSE_JCFGX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3282000ull) + ((offset) & 1) * 8;
}
#else
#define CVMX_WRSE_JCFGX(offset) (CVMX_ADD_IO_SEG(0x00011800B3282000ull) + ((offset) & 1) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_WRSE_STATUS CVMX_WRSE_STATUS_FUNC()
static inline uint64_t CVMX_WRSE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_WRSE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3280018ull);
}
#else
#define CVMX_WRSE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3280018ull))
#endif

/**
 * cvmx_wrse_bist_status
 *
 * This register indicates BIST status.
 *
 */
union cvmx_wrse_bist_status {
	uint64_t u64;
	struct cvmx_wrse_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t bist_status                  : 9;  /**< BIST Status:
                                                         - 8:7  ncar_bist_status[1:0],
                                                          6    tps_rslts_bist_status,
                                                         - 5:2  pru_car_bist_status[1:0][1:0],
                                                         - 1:0 smbf_bist_status[1:0] */
#else
	uint64_t bist_status                  : 9;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_wrse_bist_status_s        cnf75xx;
};
typedef union cvmx_wrse_bist_status cvmx_wrse_bist_status_t;

/**
 * cvmx_wrse_control
 *
 * This register is used to start WRSE HAB processing
 *
 */
union cvmx_wrse_control {
	uint64_t u64;
	struct cvmx_wrse_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid                        : 16; /**< Job ID */
	uint64_t reserved_1_15                : 15;
	uint64_t start                        : 1;  /**< "'1' = Start the HAB per config in CONFIGURATION. This bit auto-clears. This start bit is
                                                         ignored if the HAB status is busy (WRSE_STATUS bit 0 = '1')." */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_15                : 15;
	uint64_t jobid                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrse_control_s            cnf75xx;
};
typedef union cvmx_wrse_control cvmx_wrse_control_t;

/**
 * cvmx_wrse_eco
 */
union cvmx_wrse_eco {
	uint64_t u64;
	struct cvmx_wrse_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_wrse_eco_s                cnf75xx;
};
typedef union cvmx_wrse_eco cvmx_wrse_eco_t;

/**
 * cvmx_wrse_error_enable0
 *
 * This register enables error reporting for WRSE_ERROR_SOURCE0 register
 *
 */
union cvmx_wrse_error_enable0 {
	uint64_t u64;
	struct cvmx_wrse_error_enable0_s {
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
	struct cvmx_wrse_error_enable0_s      cnf75xx;
};
typedef union cvmx_wrse_error_enable0 cvmx_wrse_error_enable0_t;

/**
 * cvmx_wrse_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_wrse_error_source0 {
	uint64_t u64;
	struct cvmx_wrse_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID of the job that caused the first read port 0 overflow/underflow.
                                                         If RP0_OF=0 and RP0_UF=0, then the field is the job ID of the last or current
                                                         job. */
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
	struct cvmx_wrse_error_source0_s      cnf75xx;
};
typedef union cvmx_wrse_error_source0 cvmx_wrse_error_source0_t;

/**
 * cvmx_wrse_jcfg#
 *
 * This register space contains the WRSE job configuration data.
 *
 */
union cvmx_wrse_jcfgx {
	uint64_t u64;
	struct cvmx_wrse_jcfgx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t win_size                     : 1;  /**< Window Size:
                                                         - 0:  64 chips
                                                         - 1: 128 chips
                                                          Must be the same for both engines. */
	uint64_t div_combine                  : 1;  /**< Diversity antennta combining
                                                         - 0: no combining
                                                         - 1: combine fht engine reults
                                                          Must be the same for both engines. */
	uint64_t reserved_58_59               : 2;
	uint64_t ca_len                       : 2;  /**< Coherence accumulation length selection.  One hot.  Legal values:
                                                         - 2: 2048 chip accumulation segments
                                                         - 1: 1024 chip accumulation segments
                                                          Must be the same for both engines. */
	uint64_t ca_len_dbg                   : 3;  /**< '"Hidden" debug mode for debug of accumulations
                                                          3'b000 is the only legal value for this register.
                                                         Coherence accumulation length.' */
	uint64_t reserved_51_52               : 2;
	uint64_t base_off                     : 11; /**< Search start offset for hypothesis 0 */
	uint64_t reserved_37_39               : 3;
	uint64_t scr_seed                     : 25; /**< Scrambling code seed value. Both engines must have same seed. */
	uint64_t reserved_9_11                : 3;
	uint64_t dma_idx                      : 1;  /**< Selects input buffer
                                                         - 0: Use input dma0 (ant0)  as source of samples
                                                         - 1: Use input dma1 (ant1)  as source of samples */
	uint64_t reserved_7_7                 : 1;
	uint64_t scale                        : 3;  /**< Input sample scaling.  Right shift samples by SCALE before saturate. */
	uint64_t reserved_1_3                 : 3;
	uint64_t dc_bias_rmv                  : 1;  /**< DC offset
                                                         - 0: 2x
                                                         - 1: 2x+1 */
#else
	uint64_t dc_bias_rmv                  : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t scale                        : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t dma_idx                      : 1;
	uint64_t reserved_9_11                : 3;
	uint64_t scr_seed                     : 25;
	uint64_t reserved_37_39               : 3;
	uint64_t base_off                     : 11;
	uint64_t reserved_51_52               : 2;
	uint64_t ca_len_dbg                   : 3;
	uint64_t ca_len                       : 2;
	uint64_t reserved_58_59               : 2;
	uint64_t div_combine                  : 1;
	uint64_t win_size                     : 1;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_wrse_jcfgx_s              cnf75xx;
};
typedef union cvmx_wrse_jcfgx cvmx_wrse_jcfgx_t;

/**
 * cvmx_wrse_status
 *
 * WRSE Status Register
 *
 */
union cvmx_wrse_status {
	uint64_t u64;
	struct cvmx_wrse_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Ready to receive the next job. */
	uint64_t reserved_1_3                 : 3;
	uint64_t busy                         : 1;  /**< Indicates if the WRSE is busy processing a job. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t busy                         : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_wrse_status_s             cnf75xx;
};
typedef union cvmx_wrse_status cvmx_wrse_status_t;

#endif
