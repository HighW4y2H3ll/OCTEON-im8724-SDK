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
 * cvmx-prch-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon prch.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_PRCH_DEFS_H__
#define __CVMX_PRCH_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ANT_NUM0 CVMX_PRCH_ANT_NUM0_FUNC()
static inline uint64_t CVMX_PRCH_ANT_NUM0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ANT_NUM0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3001000ull);
}
#else
#define CVMX_PRCH_ANT_NUM0 (CVMX_ADD_IO_SEG(0x00011800B3001000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ANT_NUM1 CVMX_PRCH_ANT_NUM1_FUNC()
static inline uint64_t CVMX_PRCH_ANT_NUM1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ANT_NUM1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3001008ull);
}
#else
#define CVMX_PRCH_ANT_NUM1 (CVMX_ADD_IO_SEG(0x00011800B3001008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ANT_NUM2 CVMX_PRCH_ANT_NUM2_FUNC()
static inline uint64_t CVMX_PRCH_ANT_NUM2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ANT_NUM2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3001010ull);
}
#else
#define CVMX_PRCH_ANT_NUM2 (CVMX_ADD_IO_SEG(0x00011800B3001010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_CONTROL CVMX_PRCH_CONTROL_FUNC()
static inline uint64_t CVMX_PRCH_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000000ull);
}
#else
#define CVMX_PRCH_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3000000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ERROR_ENABLE0 CVMX_PRCH_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_PRCH_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000040ull);
}
#else
#define CVMX_PRCH_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3000040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ERROR_ENABLE1 CVMX_PRCH_ERROR_ENABLE1_FUNC()
static inline uint64_t CVMX_PRCH_ERROR_ENABLE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ERROR_ENABLE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000048ull);
}
#else
#define CVMX_PRCH_ERROR_ENABLE1 (CVMX_ADD_IO_SEG(0x00011800B3000048ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ERROR_SOURCE0 CVMX_PRCH_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_PRCH_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000030ull);
}
#else
#define CVMX_PRCH_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3000030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_ERROR_SOURCE1 CVMX_PRCH_ERROR_SOURCE1_FUNC()
static inline uint64_t CVMX_PRCH_ERROR_SOURCE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_ERROR_SOURCE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000038ull);
}
#else
#define CVMX_PRCH_ERROR_SOURCE1 (CVMX_ADD_IO_SEG(0x00011800B3000038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_JOB_CFG0 CVMX_PRCH_JOB_CFG0_FUNC()
static inline uint64_t CVMX_PRCH_JOB_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_JOB_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3002000ull);
}
#else
#define CVMX_PRCH_JOB_CFG0 (CVMX_ADD_IO_SEG(0x00011800B3002000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_JOB_CFG1 CVMX_PRCH_JOB_CFG1_FUNC()
static inline uint64_t CVMX_PRCH_JOB_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_JOB_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3004000ull);
}
#else
#define CVMX_PRCH_JOB_CFG1 (CVMX_ADD_IO_SEG(0x00011800B3004000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_PRCH_STATUS CVMX_PRCH_STATUS_FUNC()
static inline uint64_t CVMX_PRCH_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_PRCH_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3000018ull);
}
#else
#define CVMX_PRCH_STATUS (CVMX_ADD_IO_SEG(0x00011800B3000018ull))
#endif

/**
 * cvmx_prch_ant_num0
 *
 * This register reports which input antennae are mapped to PRCH antennae
 * 0-7.
 *
 * The i^th antenna in the j^th sector configured in PRCH maps is antenna
 * (N*j + i), where N is either 2, 4, or 8, depending on whether PRCH is in 2
 * antenna, 4 antenna, or 8 antenna mode.
 */
union cvmx_prch_ant_num0 {
	uint64_t u64;
	struct cvmx_prch_ant_num0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ant7                         : 6;  /**< The RFIF antenna ID for PRCH antenna 7. */
	uint64_t ant6                         : 6;  /**< The RFIF antenna ID for PRCH antenna 6. */
	uint64_t ant5                         : 6;  /**< The RFIF antenna ID for PRCH antenna 5. */
	uint64_t ant4                         : 6;  /**< The RFIF antenna ID for PRCH antenna 4. */
	uint64_t ant3                         : 6;  /**< The RFIF antenna ID for PRCH antenna 3. */
	uint64_t ant2                         : 6;  /**< The RFIF antenna ID for PRCH antenna 2. */
	uint64_t ant1                         : 6;  /**< The RFIF antenna ID for PRCH antenna 1. */
	uint64_t ant0                         : 6;  /**< The RFIF antenna ID for PRCH antenna 0. */
#else
	uint64_t ant0                         : 6;
	uint64_t ant1                         : 6;
	uint64_t ant2                         : 6;
	uint64_t ant3                         : 6;
	uint64_t ant4                         : 6;
	uint64_t ant5                         : 6;
	uint64_t ant6                         : 6;
	uint64_t ant7                         : 6;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_prch_ant_num0_s           cnf75xx;
};
typedef union cvmx_prch_ant_num0 cvmx_prch_ant_num0_t;

/**
 * cvmx_prch_ant_num1
 *
 * This register reports which input antennae are mapped to PRCH antennae
 * 8-15.
 *
 * The i^th antenna in the j^th sector configured in PRCH maps is antenna
 * (N*j + i), where N is either 2, 4, or 8, depending on whether PRCH is in 2
 * antenna, 4 antenna, or 8 antenna mode.
 */
union cvmx_prch_ant_num1 {
	uint64_t u64;
	struct cvmx_prch_ant_num1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ant15                        : 6;  /**< The RFIF antenna ID for PRCH antenna 15. */
	uint64_t ant14                        : 6;  /**< The RFIF antenna ID for PRCH antenna 14. */
	uint64_t ant13                        : 6;  /**< The RFIF antenna ID for PRCH antenna 13. */
	uint64_t ant12                        : 6;  /**< The RFIF antenna ID for PRCH antenna 12. */
	uint64_t ant11                        : 6;  /**< The RFIF antenna ID for PRCH antenna 11. */
	uint64_t ant10                        : 6;  /**< The RFIF antenna ID for PRCH antenna 10. */
	uint64_t ant9                         : 6;  /**< The RFIF antenna ID for PRCH antenna 9. */
	uint64_t ant8                         : 6;  /**< The RFIF antenna ID for PRCH antenna 8. */
#else
	uint64_t ant8                         : 6;
	uint64_t ant9                         : 6;
	uint64_t ant10                        : 6;
	uint64_t ant11                        : 6;
	uint64_t ant12                        : 6;
	uint64_t ant13                        : 6;
	uint64_t ant14                        : 6;
	uint64_t ant15                        : 6;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_prch_ant_num1_s           cnf75xx;
};
typedef union cvmx_prch_ant_num1 cvmx_prch_ant_num1_t;

/**
 * cvmx_prch_ant_num2
 *
 * This register reports which input antennae are mapped to PRCH antennae
 * 16-23.
 *
 * The i^th antenna in the j^th sector configured in PRCH maps is antenna
 * (N*j + i), where N is either 2, 4, or 8, depending on whether PRCH is in 2
 * antenna, 4 antenna, or 8 antenna mode.
 */
union cvmx_prch_ant_num2 {
	uint64_t u64;
	struct cvmx_prch_ant_num2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ant23                        : 6;  /**< The RFIF antenna ID for PRCH antenna 23. */
	uint64_t ant22                        : 6;  /**< The RFIF antenna ID for PRCH antenna 22. */
	uint64_t ant21                        : 6;  /**< The RFIF antenna ID for PRCH antenna 21. */
	uint64_t ant20                        : 6;  /**< The RFIF antenna ID for PRCH antenna 20. */
	uint64_t ant19                        : 6;  /**< The RFIF antenna ID for PRCH antenna 19. */
	uint64_t ant18                        : 6;  /**< The RFIF antenna ID for PRCH antenna 18. */
	uint64_t ant17                        : 6;  /**< The RFIF antenna ID for PRCH antenna 17. */
	uint64_t ant16                        : 6;  /**< The RFIF antenna ID for PRCH antenna 16. */
#else
	uint64_t ant16                        : 6;
	uint64_t ant17                        : 6;
	uint64_t ant18                        : 6;
	uint64_t ant19                        : 6;
	uint64_t ant20                        : 6;
	uint64_t ant21                        : 6;
	uint64_t ant22                        : 6;
	uint64_t ant23                        : 6;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_prch_ant_num2_s           cnf75xx;
};
typedef union cvmx_prch_ant_num2 cvmx_prch_ant_num2_t;

/**
 * cvmx_prch_control
 *
 * This register is used to start PRCH HAB processing
 *
 */
union cvmx_prch_control {
	uint64_t u64;
	struct cvmx_prch_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t jobid0                       : 16; /**< Job ID 0 */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< When written to 1, start PRCH processin using configuration slot 1.
                                                         Auto-clears to 1. Writes are ignores when PRCH_STATUS[STATUS1] = 1. */
	uint64_t start0                       : 1;  /**< When written to 1, start PRCH processin using configuration slot 0.
                                                         Auto-clears to 0. Writes are ignores when PRCH_STATUS[STATUS0] = 1. */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_prch_control_s            cnf75xx;
};
typedef union cvmx_prch_control cvmx_prch_control_t;

/**
 * cvmx_prch_error_enable0
 *
 * This register enables PRCH error reporting for PRCH_ERROR_SOURCE0 register.
 *
 */
union cvmx_prch_error_enable0 {
	uint64_t u64;
	struct cvmx_prch_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t oflow_err_ena                : 1;  /**< Error enable. Set to 1 to enable reporting of read overflow errors.  Reads of this bit
                                                         will
                                                         return underflow error enable. */
	uint64_t reserved_1_3                 : 3;
	uint64_t uflow_err_ena                : 1;  /**< Error enable. Set to 1 to enable reporting of read underflow errors.  Reads of this bit
                                                         will
                                                         return overflow error enable. */
#else
	uint64_t uflow_err_ena                : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t oflow_err_ena                : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_prch_error_enable0_s      cnf75xx;
};
typedef union cvmx_prch_error_enable0 cvmx_prch_error_enable0_t;

/**
 * cvmx_prch_error_enable1
 *
 * This register enables PRCH error reporting for PRCH_ERROR_SOURCE1 register.
 *
 */
union cvmx_prch_error_enable1 {
	uint64_t u64;
	struct cvmx_prch_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t err_ena                      : 1;  /**< Error enable. Set to 1 to enable reporting of SOS errors. */
#else
	uint64_t err_ena                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_prch_error_enable1_s      cnf75xx;
};
typedef union cvmx_prch_error_enable1 cvmx_prch_error_enable1_t;

/**
 * cvmx_prch_error_source0
 *
 * This register contains error source information.
 *
 */
union cvmx_prch_error_source0 {
	uint64_t u64;
	struct cvmx_prch_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t of_ul_jobid0                 : 16; /**< Overflow/Underflow Job ID for Slot0 */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Read port 0 overflow */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Read port 0 underflow */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t of_ul_jobid0                 : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_prch_error_source0_s      cnf75xx;
};
typedef union cvmx_prch_error_source0 cvmx_prch_error_source0_t;

/**
 * cvmx_prch_error_source1
 *
 * This register reports SOS timing errors.
 *
 */
union cvmx_prch_error_source1 {
	uint64_t u64;
	struct cvmx_prch_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t sos_error                    : 24; /**< Reports an SOS error. Indicates an abnormal number of input samples
                                                         fbetween consecutive SOS. One bit per antenna. Use PRCH_ANT_NUMx to
                                                         map the antenna number to the actual input antenna ID. */
#else
	uint64_t sos_error                    : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_prch_error_source1_s      cnf75xx;
};
typedef union cvmx_prch_error_source1 cvmx_prch_error_source1_t;

/**
 * cvmx_prch_job_cfg0
 *
 * This register stores the last job configuration.
 *
 */
union cvmx_prch_job_cfg0 {
	uint64_t u64;
	struct cvmx_prch_job_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t num_sectors                  : 4;  /**< The number of sectors minus 1. Sectors are numbered from 0 to
                                                         NUM_SECTORS.
                                                         This field only needs to be set during an initial setup job. */
	uint64_t ant_mode                     : 2;  /**< The number of antennae per sector:
                                                         0x0 = 2 Antennae per sector.
                                                         0x1 = 4 Antennae per sector.
                                                         0x2 = 3 Antennae per sector.
                                                         0x3 = Reserved.
                                                         This field only needs to be set during an initial setup job. */
	uint64_t job_type                     : 4;  /**< Specifies the job type:
                                                         0x0 = Initial setup job. Includes setting calibration and BF
                                                         parameters.
                                                         0x1 = Static job. Normal PRCH processing with no parameter updates.
                                                         0x2 = Parameter update job. The parameters will be updated, but no
                                                         other processing is performed.
                                                         0x3 = Parameter check job.
                                                         0x4 - 0xF = Reserved. */
#else
	uint64_t job_type                     : 4;
	uint64_t ant_mode                     : 2;
	uint64_t num_sectors                  : 4;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_prch_job_cfg0_s           cnf75xx;
};
typedef union cvmx_prch_job_cfg0 cvmx_prch_job_cfg0_t;

/**
 * cvmx_prch_job_cfg1
 *
 * This register stores the last job configuration.
 *
 * This is an alias for PRCH_JOB_CFG0 and will report the job configuration
 * for the last job run on either PRCH slot 0 or 1.
 */
union cvmx_prch_job_cfg1 {
	uint64_t u64;
	struct cvmx_prch_job_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t num_sectors                  : 4;  /**< The number of sectors minus 1. Sectors are numbered from 0 to
                                                         NUM_SECTORS.
                                                         This field only needs to be set during an initial setup job. */
	uint64_t ant_mode                     : 2;  /**< The number of antennae per sector:
                                                         0x0 = 2 Antennae per sector.
                                                         0x1 = 4 Antennae per sector.
                                                         0x2 = 3 Antennae per sector.
                                                         0x3 = Reserved.
                                                         This field only needs to be set during an initial setup job. */
	uint64_t job_type                     : 4;  /**< Specifies the job type:
                                                         0x0 = Initial setup job. Includes setting calibration and BF
                                                         parameters.
                                                         0x1 = Static job. Normal PRCH processing with no parameter updates.
                                                         0x2 = Parameter update job. The parameters will be updated, but no
                                                         other processing is performed.
                                                         0x3 = Parameter check job.
                                                         0x4 - 0xF = Reserved. */
#else
	uint64_t job_type                     : 4;
	uint64_t ant_mode                     : 2;
	uint64_t num_sectors                  : 4;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_prch_job_cfg1_s           cnf75xx;
};
typedef union cvmx_prch_job_cfg1 cvmx_prch_job_cfg1_t;

/**
 * cvmx_prch_status
 */
union cvmx_prch_status {
	uint64_t u64;
	struct cvmx_prch_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Indicates if the PRCH is ready to accept a job.   '1' = Ready for new
                                                         job, '0' = Busy processing. */
	uint64_t reserved_2_3                 : 2;
	uint64_t status1                      : 1;  /**< Indicates if the PRCH is busy processing a job in Slot 1. '0' = Ready for new
                                                         job, '1' = Busy processing. */
	uint64_t status0                      : 1;  /**< Indicates if the PRCH is busy processing a job in Slot 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t status1                      : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_prch_status_s             cnf75xx;
};
typedef union cvmx_prch_status cvmx_prch_status_t;

#endif
