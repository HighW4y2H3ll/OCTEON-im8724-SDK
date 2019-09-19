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
 * cvmx-dlfe-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon dlfe.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_DLFE_DEFS_H__
#define __CVMX_DLFE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_BIST_STATUS CVMX_DLFE_BIST_STATUS_FUNC()
static inline uint64_t CVMX_DLFE_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180050ull);
}
#else
#define CVMX_DLFE_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3180050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_BIST_STATUS1 CVMX_DLFE_BIST_STATUS1_FUNC()
static inline uint64_t CVMX_DLFE_BIST_STATUS1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_BIST_STATUS1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180058ull);
}
#else
#define CVMX_DLFE_BIST_STATUS1 (CVMX_ADD_IO_SEG(0x00011800B3180058ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DLFE_CONFIG0X(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 511)))))
		cvmx_warn("CVMX_DLFE_CONFIG0X(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3182000ull) + ((offset) & 511) * 8;
}
#else
#define CVMX_DLFE_CONFIG0X(offset) (CVMX_ADD_IO_SEG(0x00011800B3182000ull) + ((offset) & 511) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DLFE_CONFIG1X(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 511)))))
		cvmx_warn("CVMX_DLFE_CONFIG1X(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3184000ull) + ((offset) & 511) * 8;
}
#else
#define CVMX_DLFE_CONFIG1X(offset) (CVMX_ADD_IO_SEG(0x00011800B3184000ull) + ((offset) & 511) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_CONTROL CVMX_DLFE_CONTROL_FUNC()
static inline uint64_t CVMX_DLFE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180000ull);
}
#else
#define CVMX_DLFE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3180000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_DEBUG_DUMP_ANTENNA CVMX_DLFE_DEBUG_DUMP_ANTENNA_FUNC()
static inline uint64_t CVMX_DLFE_DEBUG_DUMP_ANTENNA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_DEBUG_DUMP_ANTENNA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180410ull);
}
#else
#define CVMX_DLFE_DEBUG_DUMP_ANTENNA (CVMX_ADD_IO_SEG(0x00011800B3180410ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_DEBUG_DUMP_SIZE CVMX_DLFE_DEBUG_DUMP_SIZE_FUNC()
static inline uint64_t CVMX_DLFE_DEBUG_DUMP_SIZE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_DEBUG_DUMP_SIZE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180418ull);
}
#else
#define CVMX_DLFE_DEBUG_DUMP_SIZE (CVMX_ADD_IO_SEG(0x00011800B3180418ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ECC_CTRL CVMX_DLFE_ECC_CTRL_FUNC()
static inline uint64_t CVMX_DLFE_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180060ull);
}
#else
#define CVMX_DLFE_ECC_CTRL (CVMX_ADD_IO_SEG(0x00011800B3180060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ECC_ENABLE CVMX_DLFE_ECC_ENABLE_FUNC()
static inline uint64_t CVMX_DLFE_ECC_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ECC_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180070ull);
}
#else
#define CVMX_DLFE_ECC_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3180070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ECC_STATUS CVMX_DLFE_ECC_STATUS_FUNC()
static inline uint64_t CVMX_DLFE_ECC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ECC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180068ull);
}
#else
#define CVMX_DLFE_ECC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3180068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ECO CVMX_DLFE_ECO_FUNC()
static inline uint64_t CVMX_DLFE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180008ull);
}
#else
#define CVMX_DLFE_ECO (CVMX_ADD_IO_SEG(0x00011800B3180008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ERROR_ENABLE0 CVMX_DLFE_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_DLFE_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180040ull);
}
#else
#define CVMX_DLFE_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3180040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_ERROR_SOURCE0 CVMX_DLFE_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_DLFE_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180030ull);
}
#else
#define CVMX_DLFE_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3180030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_PARITY_CTRL CVMX_DLFE_PARITY_CTRL_FUNC()
static inline uint64_t CVMX_DLFE_PARITY_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_PARITY_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180078ull);
}
#else
#define CVMX_DLFE_PARITY_CTRL (CVMX_ADD_IO_SEG(0x00011800B3180078ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_PARITY_ENABLE CVMX_DLFE_PARITY_ENABLE_FUNC()
static inline uint64_t CVMX_DLFE_PARITY_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_PARITY_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180088ull);
}
#else
#define CVMX_DLFE_PARITY_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3180088ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_PARITY_STATUS CVMX_DLFE_PARITY_STATUS_FUNC()
static inline uint64_t CVMX_DLFE_PARITY_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_PARITY_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180080ull);
}
#else
#define CVMX_DLFE_PARITY_STATUS (CVMX_ADD_IO_SEG(0x00011800B3180080ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_SOS_ADVANCE CVMX_DLFE_SOS_ADVANCE_FUNC()
static inline uint64_t CVMX_DLFE_SOS_ADVANCE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_SOS_ADVANCE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180108ull);
}
#else
#define CVMX_DLFE_SOS_ADVANCE (CVMX_ADD_IO_SEG(0x00011800B3180108ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_SOS_FILTER CVMX_DLFE_SOS_FILTER_FUNC()
static inline uint64_t CVMX_DLFE_SOS_FILTER_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_SOS_FILTER not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180100ull);
}
#else
#define CVMX_DLFE_SOS_FILTER (CVMX_ADD_IO_SEG(0x00011800B3180100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_SSP_ADDR CVMX_DLFE_SSP_ADDR_FUNC()
static inline uint64_t CVMX_DLFE_SSP_ADDR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_SSP_ADDR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180408ull);
}
#else
#define CVMX_DLFE_SSP_ADDR (CVMX_ADD_IO_SEG(0x00011800B3180408ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_SSP_DATA CVMX_DLFE_SSP_DATA_FUNC()
static inline uint64_t CVMX_DLFE_SSP_DATA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_SSP_DATA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180400ull);
}
#else
#define CVMX_DLFE_SSP_DATA (CVMX_ADD_IO_SEG(0x00011800B3180400ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_DLFE_STATUS CVMX_DLFE_STATUS_FUNC()
static inline uint64_t CVMX_DLFE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_DLFE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3180018ull);
}
#else
#define CVMX_DLFE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3180018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_DLFE_TSSIX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 47)))))
		cvmx_warn("CVMX_DLFE_TSSIX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3180200ull) + ((offset) & 63) * 8;
}
#else
#define CVMX_DLFE_TSSIX(offset) (CVMX_ADD_IO_SEG(0x00011800B3180200ull) + ((offset) & 63) * 8)
#endif

/**
 * cvmx_dlfe_bist_status
 *
 * This register reports BIST status for RAMs in the DLFE.
 *
 */
union cvmx_dlfe_bist_status {
	uint64_t u64;
	struct cvmx_dlfe_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ram                          : 64; /**< OR of BIST Status for each group of RAMs. */
#else
	uint64_t ram                          : 64;
#endif
	} s;
	struct cvmx_dlfe_bist_status_s        cnf75xx;
};
typedef union cvmx_dlfe_bist_status cvmx_dlfe_bist_status_t;

/**
 * cvmx_dlfe_bist_status1
 *
 * This register reports BIST status for additional DLFE RAMs.
 *
 */
union cvmx_dlfe_bist_status1 {
	uint64_t u64;
	struct cvmx_dlfe_bist_status1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ram                          : 64; /**< OR of BIST Status for each group of RAMs. */
#else
	uint64_t ram                          : 64;
#endif
	} s;
	struct cvmx_dlfe_bist_status1_s       cnf75xx;
};
typedef union cvmx_dlfe_bist_status1 cvmx_dlfe_bist_status1_t;

/**
 * cvmx_dlfe_config0#
 *
 * These registers store the job configuration for slot 0.
 *
 */
union cvmx_dlfe_config0x {
	uint64_t u64;
	struct cvmx_dlfe_config0x_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< N/A */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_dlfe_config0x_s           cnf75xx;
};
typedef union cvmx_dlfe_config0x cvmx_dlfe_config0x_t;

/**
 * cvmx_dlfe_config1#
 *
 * These registers store the job configuration for slot 1.
 *
 */
union cvmx_dlfe_config1x {
	uint64_t u64;
	struct cvmx_dlfe_config1x_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< N/A */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_dlfe_config1x_s           cnf75xx;
};
typedef union cvmx_dlfe_config1x cvmx_dlfe_config1x_t;

/**
 * cvmx_dlfe_control
 *
 * This register is used to start DLFE HAB processing.
 *
 */
union cvmx_dlfe_control {
	uint64_t u64;
	struct cvmx_dlfe_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID for configuration slot 1. */
	uint64_t jobid0                       : 16; /**< Job ID for configuration slot 0. */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< When one is written, the HAB will start processing the job in
                                                         configuration slot 1. This bit auto-clears once the job has started.
                                                         Writes are ignored when the HAB is already busy processing a job in
                                                         slot 1. */
	uint64_t start0                       : 1;  /**< When one is written, the HAB will start processing the job in
                                                         configuration slot 0. This bit auto-clears once the job has started.
                                                         Writes are ignored when the HAB is already busy processing a job in
                                                         slot 1. */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_dlfe_control_s            cnf75xx;
};
typedef union cvmx_dlfe_control cvmx_dlfe_control_t;

/**
 * cvmx_dlfe_debug_dump_antenna
 */
union cvmx_dlfe_debug_dump_antenna {
	uint64_t u64;
	struct cvmx_dlfe_debug_dump_antenna_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t enable                       : 48; /**< Enable debug dump for each individual antenna. Debugging can be
                                                         enabled for no more than 12 antennae when the sample rate is 30.72 MHz
                                                         or 23.04 MHz, and no more than 24 antennae when the sample rate is
                                                         15.36 MHz or less. */
#else
	uint64_t enable                       : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_dlfe_debug_dump_antenna_s cnf75xx;
};
typedef union cvmx_dlfe_debug_dump_antenna cvmx_dlfe_debug_dump_antenna_t;

/**
 * cvmx_dlfe_debug_dump_size
 */
union cvmx_dlfe_debug_dump_size {
	uint64_t u64;
	struct cvmx_dlfe_debug_dump_size_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t size                         : 18; /**< The total number of 128 bit words in the debug dump output for all
                                                         active antennae with debug dump enabled, minus 1. */
#else
	uint64_t size                         : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_dlfe_debug_dump_size_s    cnf75xx;
};
typedef union cvmx_dlfe_debug_dump_size cvmx_dlfe_debug_dump_size_t;

/**
 * cvmx_dlfe_ecc_ctrl
 *
 * This register enables flipping of ECC for testing.
 *
 */
union cvmx_dlfe_ecc_ctrl {
	uint64_t u64;
	struct cvmx_dlfe_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_36_63               : 28;
	uint64_t cdis                         : 4;  /**< Correction disable. Each bit represents a different RAM as follows:
                                                         <32> = Job configuration.
                                                         <33> = IFFT pass FIFO.
                                                         <34> = RFIF state.
                                                         <35> = RFIF control. */
	uint64_t reserved_20_31               : 12;
	uint64_t flip1                        : 4;  /**< Flip second ECC bit. Each bit represents a different RAM as follows:
                                                         <0> = Job configuration.
                                                         <1> = IFFT pass FIFO.
                                                         <2> = RFIF state.
                                                         <3> = RFIF control. */
	uint64_t reserved_4_15                : 12;
	uint64_t flip0                        : 4;  /**< Flip first ECC bit. Each bit represents a different RAM as follows:
                                                         <0> = Job configuration.
                                                         <1> = IFFT pass FIFO.
                                                         <2> = RFIF state.
                                                         <3> = RFIF control. */
#else
	uint64_t flip0                        : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t flip1                        : 4;
	uint64_t reserved_20_31               : 12;
	uint64_t cdis                         : 4;
	uint64_t reserved_36_63               : 28;
#endif
	} s;
	struct cvmx_dlfe_ecc_ctrl_s           cnf75xx;
};
typedef union cvmx_dlfe_ecc_ctrl cvmx_dlfe_ecc_ctrl_t;

/**
 * cvmx_dlfe_ecc_enable
 *
 * This register enables signaling of ECC errors.
 *
 */
union cvmx_dlfe_ecc_enable {
	uint64_t u64;
	struct cvmx_dlfe_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dbe                          : 4;  /**< Double bit error. Each bit represents a different RAM as follows:
                                                         <16> = Job configuration.
                                                         <17> = IFFT pass FIFO.
                                                         <18> = RFIF state.
                                                         <19> = RFIF control. */
	uint64_t reserved_4_15                : 12;
	uint64_t sbe                          : 4;  /**< Single bit error. Each bit represents a different RAM as follows:
                                                         <0> = Job configuration.
                                                         <1> = IFFT pass FIFO.
                                                         <2> = RFIF state.
                                                         <3> = RFIF control. */
#else
	uint64_t sbe                          : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t dbe                          : 4;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_dlfe_ecc_enable_s         cnf75xx;
};
typedef union cvmx_dlfe_ecc_enable cvmx_dlfe_ecc_enable_t;

/**
 * cvmx_dlfe_ecc_status
 *
 * This register reports which ECC errors have occured. Write one
 * to any bit to clear the error.
 */
union cvmx_dlfe_ecc_status {
	uint64_t u64;
	struct cvmx_dlfe_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dbe                          : 4;  /**< Double bit error. Each bit represents a different RAM as follows:
                                                         <16> = Job configuration.
                                                         <17> = IFFT pass FIFO.
                                                         <18> = RFIF state.
                                                         <19> = RFIF control. */
	uint64_t reserved_4_15                : 12;
	uint64_t sbe                          : 4;  /**< Single bit error. Each bit represents a different RAM as follows:
                                                         <0> = Job configuration.
                                                         <1> = IFFT pass FIFO.
                                                         <2> = RFIF state.
                                                         <3> = RFIF control. */
#else
	uint64_t sbe                          : 4;
	uint64_t reserved_4_15                : 12;
	uint64_t dbe                          : 4;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_dlfe_ecc_status_s         cnf75xx;
};
typedef union cvmx_dlfe_ecc_status cvmx_dlfe_ecc_status_t;

/**
 * cvmx_dlfe_eco
 */
union cvmx_dlfe_eco {
	uint64_t u64;
	struct cvmx_dlfe_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_dlfe_eco_s                cnf75xx;
};
typedef union cvmx_dlfe_eco cvmx_dlfe_eco_t;

/**
 * cvmx_dlfe_error_enable0
 *
 * This register enables reporting of read DMA underflow and overflow.
 *
 */
union cvmx_dlfe_error_enable0 {
	uint64_t u64;
	struct cvmx_dlfe_error_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t rd2_of_en                    : 1;  /**< Read port 2 overflow enable. */
	uint64_t rd1_of_en                    : 1;  /**< Read port 1 overflow enable. */
	uint64_t rd0_of_en                    : 1;  /**< Read port 0 overflow enable. */
	uint64_t reserved_3_3                 : 1;
	uint64_t rd2_uf_en                    : 1;  /**< Read port 2 underflow enable. */
	uint64_t rd1_uf_en                    : 1;  /**< Read port 1 underflow enable. */
	uint64_t rd0_uf_en                    : 1;  /**< Read port 0 underflow enable. */
#else
	uint64_t rd0_uf_en                    : 1;
	uint64_t rd1_uf_en                    : 1;
	uint64_t rd2_uf_en                    : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t rd0_of_en                    : 1;
	uint64_t rd1_of_en                    : 1;
	uint64_t rd2_of_en                    : 1;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_dlfe_error_enable0_s      cnf75xx;
};
typedef union cvmx_dlfe_error_enable0 cvmx_dlfe_error_enable0_t;

/**
 * cvmx_dlfe_error_source0
 *
 * This register contains flags for read DMA underflow and overflow.
 *
 */
union cvmx_dlfe_error_source0 {
	uint64_t u64;
	struct cvmx_dlfe_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rp2_jid                      : 16; /**< Job ID of job that caused overflow/underflow on read port 2. */
	uint64_t rp1_jid                      : 16; /**< Job ID of job that caused overflow/underflow on read port 1. */
	uint64_t rp0_jid                      : 16; /**< Job ID of job that caused overflow/underflow on read port 0. */
	uint64_t reserved_7_15                : 9;
	uint64_t rd2_of                       : 1;  /**< Read port 2 overflow. */
	uint64_t rd1_of                       : 1;  /**< Read port 1 overflow. */
	uint64_t rd0_of                       : 1;  /**< Read port 0 overflow. */
	uint64_t reserved_3_3                 : 1;
	uint64_t rd2_uf                       : 1;  /**< Read port 2 underflow. */
	uint64_t rd1_uf                       : 1;  /**< Read port 1 underflow. */
	uint64_t rd0_uf                       : 1;  /**< Read port 0 underflow. */
#else
	uint64_t rd0_uf                       : 1;
	uint64_t rd1_uf                       : 1;
	uint64_t rd2_uf                       : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t rd0_of                       : 1;
	uint64_t rd1_of                       : 1;
	uint64_t rd2_of                       : 1;
	uint64_t reserved_7_15                : 9;
	uint64_t rp0_jid                      : 16;
	uint64_t rp1_jid                      : 16;
	uint64_t rp2_jid                      : 16;
#endif
	} s;
	struct cvmx_dlfe_error_source0_s      cnf75xx;
};
typedef union cvmx_dlfe_error_source0 cvmx_dlfe_error_source0_t;

/**
 * cvmx_dlfe_parity_ctrl
 *
 * This register enables flipping of parity for testing.
 *
 */
union cvmx_dlfe_parity_ctrl {
	uint64_t u64;
	struct cvmx_dlfe_parity_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t flip                         : 5;  /**< Flip parity. One bit per RAM, with each bit mapped as follows:
                                                         _ <0> = Beam-forming coefficients.
                                                         _ <1> = Calibration coefficeints.
                                                         _ <2> = Windowing coefficients.
                                                         _ <3> = CDD values.
                                                         _ <4> = Antenna gain. */
#else
	uint64_t flip                         : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_dlfe_parity_ctrl_s        cnf75xx;
};
typedef union cvmx_dlfe_parity_ctrl cvmx_dlfe_parity_ctrl_t;

/**
 * cvmx_dlfe_parity_enable
 *
 * This register enables signaling of parity errors.
 *
 */
union cvmx_dlfe_parity_enable {
	uint64_t u64;
	struct cvmx_dlfe_parity_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t parity                       : 5;  /**< Parity error. One bit per RAM, with each bit mapped as follows:
                                                         _ <0> = Beam-forming coefficients.
                                                         _ <1> = Calibration coefficeints.
                                                         _ <2> = Windowing coefficients.
                                                         _ <3> = CDD values.
                                                         _ <4> = Antenna gain. */
#else
	uint64_t parity                       : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_dlfe_parity_enable_s      cnf75xx;
};
typedef union cvmx_dlfe_parity_enable cvmx_dlfe_parity_enable_t;

/**
 * cvmx_dlfe_parity_status
 *
 * This register reports parity errors.
 *
 */
union cvmx_dlfe_parity_status {
	uint64_t u64;
	struct cvmx_dlfe_parity_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t parity                       : 5;  /**< Parity error. One bit per RAM, with each bit mapped as follows:
                                                         _ <0> = Beam-forming coefficients.
                                                         _ <1> = Calibration coefficeints.
                                                         _ <2> = Windowing coefficients.
                                                         _ <3> = CDD values.
                                                         _ <4> = Antenna gain. */
#else
	uint64_t parity                       : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_dlfe_parity_status_s      cnf75xx;
};
typedef union cvmx_dlfe_parity_status cvmx_dlfe_parity_status_t;

/**
 * cvmx_dlfe_sos_advance
 *
 * This register determines when DLFE will reset its internal tick counter.
 * The counter is reset when the SoS filter counter is equal to
 * DLFE_SOS_ADVANCE.
 */
union cvmx_dlfe_sos_advance {
	uint64_t u64;
	struct cvmx_dlfe_sos_advance_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< When the SoS filter counter reaches COUNT, the internal tick count is
                                                         reset to zero. This should be approximately the number of cycles
                                                         required for a single symbol transmission (e.g., about 57000 when
                                                         running at 800 MHz). */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_dlfe_sos_advance_s        cnf75xx;
};
typedef union cvmx_dlfe_sos_advance cvmx_dlfe_sos_advance_t;

/**
 * cvmx_dlfe_sos_filter
 *
 * Initial value of the SoS filter counter loaded when the SOS_DL is received
 * from RFIF. Ths SoS filter counter decrments by one each clock cycle. If
 * the next SOS_DL arrives before the counter reaches zero, then the SOS_DL
 * is ignored.
 */
union cvmx_dlfe_sos_filter {
	uint64_t u64;
	struct cvmx_dlfe_sos_filter_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< The cycle count used to initialize the SoS filter counter. The value
                                                         should be set to the expected number of clock cycles in a 1 ms
                                                         subframe, minus a small winow (10-20 cycles) to allow for SOS jitter. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_dlfe_sos_filter_s         cnf75xx;
};
typedef union cvmx_dlfe_sos_filter cvmx_dlfe_sos_filter_t;

/**
 * cvmx_dlfe_ssp_addr
 *
 * This register is used to select a portion of a semi-static parameter to be
 * read or written through the DLFE_SSP_DATA registers. See DLFE_SSP_DATA for
 * more details.
 */
union cvmx_dlfe_ssp_addr {
	uint64_t u64;
	struct cvmx_dlfe_ssp_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_21_63               : 43;
	uint64_t ssp_type                     : 3;  /**< Selects the semi-static parameter to read or write:
                                                         0x0 = BF coefficients.
                                                         0x1 = BF coefficient exponents.
                                                         0x2 = Calibration coefficients.
                                                         0x3 = Calibration coefficient exponents.
                                                         0x4 = Windowing coefficients.
                                                         0x5 = CDD.
                                                         0x6 = Antenna gain.
                                                         0x7 = Sector-antenna map. */
	uint64_t antenna                      : 6;  /**< When readinging or writing the calibration coefficients, this field
                                                         selects the antenna. Otherwise this field is ignored. */
	uint64_t index                        : 10; /**< The index into the semi-static parameter data structure. Data is
                                                         organized as 32-bit words stored in little-endian format.  Except for
                                                         the sector-antenna map, all data structures match the descriptions
                                                         provided in the HRM. Sector-antenna map uses a packed data structure
                                                         with only 4 bits per sector instead of the sparse data structure used
                                                         when programming the SSPUPs. */
	uint64_t reserved_0_1                 : 2;
#else
	uint64_t reserved_0_1                 : 2;
	uint64_t index                        : 10;
	uint64_t antenna                      : 6;
	uint64_t ssp_type                     : 3;
	uint64_t reserved_21_63               : 43;
#endif
	} s;
	struct cvmx_dlfe_ssp_addr_s           cnf75xx;
};
typedef union cvmx_dlfe_ssp_addr cvmx_dlfe_ssp_addr_t;

/**
 * cvmx_dlfe_ssp_data
 *
 * DLFE_SSP_ADDR and DLFE_SSP_DATA provide direct access to the DLFE semi-static parameters.
 * Software must first write DLFE_SSP_ADDR to select a portion of a semi-static parameter to read
 * or
 * write. Once DLFE_SSP_ADDR has been set, a read/write to DLFE_SSP_DATA will
 * read/write a portion of a semi-static parameter data structure.
 * In general, accesses will read/write only the bottom 32-bits. However,
 * when reading/writing the sector antenna map, each access reads/writes
 * 64-bits of data. Sector antenna map information is 4 bits per
 * sector, with log(num antenna) in the low two bits, and (log(fft size)-9)
 * in the high two bits.
 *
 * These registers are intended for debugging use when the DLFE pipeline is
 * idle. When the DLFE pipeline is busy, read and write operations may fail
 * due to contention with normal processing. Software must peform a read
 * after any write to determine whether the write succeeded.Failed reads will
 * set DATA bit 63 to one.
 */
union cvmx_dlfe_ssp_data {
	uint64_t u64;
	struct cvmx_dlfe_ssp_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< For most acceses, bits (31..0) read/write the semi-static parameter value. On most reads,
                                                         bit 63 is the RETRY bit which is set to one if the read access fails and needs to be
                                                         retried. When accessing the sector antenna map, accesses use all 64 bits and there is no
                                                         retry bit. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_dlfe_ssp_data_s           cnf75xx;
};
typedef union cvmx_dlfe_ssp_data cvmx_dlfe_ssp_data_t;

/**
 * cvmx_dlfe_status
 *
 * This register is used to monitor DLFE processing.
 *
 */
union cvmx_dlfe_status {
	uint64_t u64;
	struct cvmx_dlfe_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< The DLFE is ready for the next job. */
	uint64_t reserved_2_3                 : 2;
	uint64_t busy1                        : 1;  /**< The DLFE is busy processing the job in configuration slot 1. */
	uint64_t busy0                        : 1;  /**< The DLFE is busy processing the job in configuration slot 0. */
#else
	uint64_t busy0                        : 1;
	uint64_t busy1                        : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_dlfe_status_s             cnf75xx;
};
typedef union cvmx_dlfe_status cvmx_dlfe_status_t;

/**
 * cvmx_dlfe_tssi#
 *
 * This register reports the transmit signal strength indicator (TSSI) for
 * each antenna that transmitted during the last subframe.
 */
union cvmx_dlfe_tssix {
	uint64_t u64;
	struct cvmx_dlfe_tssix_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t retry                        : 1;  /**< Indicates that a read access occured when the DLFE was busy updating
                                                         the TSSI value. A subsequent read access should succeed. */
	uint64_t reserved_48_62               : 15;
	uint64_t data                         : 48; /**< The TSSI value for antenna [a]. */
#else
	uint64_t data                         : 48;
	uint64_t reserved_48_62               : 15;
	uint64_t retry                        : 1;
#endif
	} s;
	struct cvmx_dlfe_tssix_s              cnf75xx;
};
typedef union cvmx_dlfe_tssix cvmx_dlfe_tssix_t;

#endif
