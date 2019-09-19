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
 * cvmx-ulfe-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon ulfe.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_ULFE_DEFS_H__
#define __CVMX_ULFE_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ANT_STATUS CVMX_ULFE_ANT_STATUS_FUNC()
static inline uint64_t CVMX_ULFE_ANT_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ANT_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081070ull);
}
#else
#define CVMX_ULFE_ANT_STATUS (CVMX_ADD_IO_SEG(0x00011800B3081070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_BIST_STATUS0 CVMX_ULFE_BIST_STATUS0_FUNC()
static inline uint64_t CVMX_ULFE_BIST_STATUS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_BIST_STATUS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080050ull);
}
#else
#define CVMX_ULFE_BIST_STATUS0 (CVMX_ADD_IO_SEG(0x00011800B3080050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_ULFE_CONFIGURATION0X(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 511)))))
		cvmx_warn("CVMX_ULFE_CONFIGURATION0X(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3082000ull) + ((offset) & 511) * 8;
}
#else
#define CVMX_ULFE_CONFIGURATION0X(offset) (CVMX_ADD_IO_SEG(0x00011800B3082000ull) + ((offset) & 511) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_ULFE_CONFIGURATION1X(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 511)))))
		cvmx_warn("CVMX_ULFE_CONFIGURATION1X(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3084000ull) + ((offset) & 511) * 8;
}
#else
#define CVMX_ULFE_CONFIGURATION1X(offset) (CVMX_ADD_IO_SEG(0x00011800B3084000ull) + ((offset) & 511) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_CONTROL CVMX_ULFE_CONTROL_FUNC()
static inline uint64_t CVMX_ULFE_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080000ull);
}
#else
#define CVMX_ULFE_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3080000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ECC_CONTROL0 CVMX_ULFE_ECC_CONTROL0_FUNC()
static inline uint64_t CVMX_ULFE_ECC_CONTROL0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ECC_CONTROL0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080060ull);
}
#else
#define CVMX_ULFE_ECC_CONTROL0 (CVMX_ADD_IO_SEG(0x00011800B3080060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ECC_ENABLE0 CVMX_ULFE_ECC_ENABLE0_FUNC()
static inline uint64_t CVMX_ULFE_ECC_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ECC_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080070ull);
}
#else
#define CVMX_ULFE_ECC_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3080070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ECC_STATUS0 CVMX_ULFE_ECC_STATUS0_FUNC()
static inline uint64_t CVMX_ULFE_ECC_STATUS0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ECC_STATUS0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080068ull);
}
#else
#define CVMX_ULFE_ECC_STATUS0 (CVMX_ADD_IO_SEG(0x00011800B3080068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ECO CVMX_ULFE_ECO_FUNC()
static inline uint64_t CVMX_ULFE_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080008ull);
}
#else
#define CVMX_ULFE_ECO (CVMX_ADD_IO_SEG(0x00011800B3080008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ERROR_ENABLE1 CVMX_ULFE_ERROR_ENABLE1_FUNC()
static inline uint64_t CVMX_ULFE_ERROR_ENABLE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ERROR_ENABLE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080048ull);
}
#else
#define CVMX_ULFE_ERROR_ENABLE1 (CVMX_ADD_IO_SEG(0x00011800B3080048ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ERROR_SOURCE1 CVMX_ULFE_ERROR_SOURCE1_FUNC()
static inline uint64_t CVMX_ULFE_ERROR_SOURCE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ERROR_SOURCE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080038ull);
}
#else
#define CVMX_ULFE_ERROR_SOURCE1 (CVMX_ADD_IO_SEG(0x00011800B3080038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_INDIRECT_ADDR CVMX_ULFE_INDIRECT_ADDR_FUNC()
static inline uint64_t CVMX_ULFE_INDIRECT_ADDR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_INDIRECT_ADDR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081000ull);
}
#else
#define CVMX_ULFE_INDIRECT_ADDR (CVMX_ADD_IO_SEG(0x00011800B3081000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_INDIRECT_DATA CVMX_ULFE_INDIRECT_DATA_FUNC()
static inline uint64_t CVMX_ULFE_INDIRECT_DATA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_INDIRECT_DATA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081008ull);
}
#else
#define CVMX_ULFE_INDIRECT_DATA (CVMX_ADD_IO_SEG(0x00011800B3081008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_INP_CONTROL CVMX_ULFE_INP_CONTROL_FUNC()
static inline uint64_t CVMX_ULFE_INP_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_INP_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081068ull);
}
#else
#define CVMX_ULFE_INP_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3081068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ISS_CNT0 CVMX_ULFE_ISS_CNT0_FUNC()
static inline uint64_t CVMX_ULFE_ISS_CNT0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ISS_CNT0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081078ull);
}
#else
#define CVMX_ULFE_ISS_CNT0 (CVMX_ADD_IO_SEG(0x00011800B3081078ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ISS_CNT1 CVMX_ULFE_ISS_CNT1_FUNC()
static inline uint64_t CVMX_ULFE_ISS_CNT1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ISS_CNT1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081080ull);
}
#else
#define CVMX_ULFE_ISS_CNT1 (CVMX_ADD_IO_SEG(0x00011800B3081080ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_ISS_CNT2 CVMX_ULFE_ISS_CNT2_FUNC()
static inline uint64_t CVMX_ULFE_ISS_CNT2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_ISS_CNT2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081088ull);
}
#else
#define CVMX_ULFE_ISS_CNT2 (CVMX_ADD_IO_SEG(0x00011800B3081088ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_READ_ERR_ENA CVMX_ULFE_READ_ERR_ENA_FUNC()
static inline uint64_t CVMX_ULFE_READ_ERR_ENA_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_READ_ERR_ENA not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080040ull);
}
#else
#define CVMX_ULFE_READ_ERR_ENA (CVMX_ADD_IO_SEG(0x00011800B3080040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_READ_ERR_SRC CVMX_ULFE_READ_ERR_SRC_FUNC()
static inline uint64_t CVMX_ULFE_READ_ERR_SRC_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_READ_ERR_SRC not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080030ull);
}
#else
#define CVMX_ULFE_READ_ERR_SRC (CVMX_ADD_IO_SEG(0x00011800B3080030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_SOS_DOWN_CNT CVMX_ULFE_SOS_DOWN_CNT_FUNC()
static inline uint64_t CVMX_ULFE_SOS_DOWN_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_SOS_DOWN_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081098ull);
}
#else
#define CVMX_ULFE_SOS_DOWN_CNT (CVMX_ADD_IO_SEG(0x00011800B3081098ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_STATUS CVMX_ULFE_STATUS_FUNC()
static inline uint64_t CVMX_ULFE_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3080018ull);
}
#else
#define CVMX_ULFE_STATUS (CVMX_ADD_IO_SEG(0x00011800B3080018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ULFE_TD_RSSI_CNT CVMX_ULFE_TD_RSSI_CNT_FUNC()
static inline uint64_t CVMX_ULFE_TD_RSSI_CNT_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_ULFE_TD_RSSI_CNT not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3081090ull);
}
#else
#define CVMX_ULFE_TD_RSSI_CNT (CVMX_ADD_IO_SEG(0x00011800B3081090ull))
#endif

/**
 * cvmx_ulfe_ant_status
 *
 * This register reports enable status for each antenna.
 *
 */
union cvmx_ulfe_ant_status {
	uint64_t u64;
	struct cvmx_ulfe_ant_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t ant_enable                   : 48; /**< Indicates the enable status for each antenna.
                                                         0 = ULFE drops any data for this antenna.
                                                         1 = The antenna is enabled in the sector-antenna map, and any data
                                                         received for this antenna is written into an input buffer for
                                                         processing. */
#else
	uint64_t ant_enable                   : 48;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_ulfe_ant_status_s         cnf75xx;
};
typedef union cvmx_ulfe_ant_status cvmx_ulfe_ant_status_t;

/**
 * cvmx_ulfe_bist_status0
 *
 * This register provides access to the internal BIST results.  Each bit is
 * the BIST result of an individual memory (per bit, 0 = pass and 1 = fail).
 */
union cvmx_ulfe_bist_status0 {
	uint64_t u64;
	struct cvmx_ulfe_bist_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t tsk_status                   : 1;  /**< Task FIFO memory BIST status. */
	uint64_t fdrssi_status                : 1;  /**< TD RSSI buffer memory BIST status. */
	uint64_t fdxcorr_status               : 1;  /**< TD RSSI buffer memory BIST status. */
	uint64_t fft_status                   : 1;  /**< FFT memory BIST status. */
	uint64_t wdma_status                  : 1;  /**< WDMA buffer memory BIST status. */
	uint64_t xcor_status                  : 1;  /**< Cross-correlation buffer memory BIST status. */
	uint64_t bfib_status                  : 1;  /**< Beam-former input buffer memory BIST status. */
	uint64_t frb_status                   : 1;  /**< FFt reorder buffer memory BIST status. */
	uint64_t inpb_status                  : 1;  /**< Input buffer memory BIST status. */
	uint64_t cfg_status                   : 1;  /**< Configuration memory BIST status. */
	uint64_t gcp_status                   : 1;  /**< General control and parameter memory Bist Status */
	uint64_t bfc_status                   : 1;  /**< Beam-former coefficient memory BIST status. */
	uint64_t antc_status                  : 1;  /**< Antenna calibration coefficient memory BIST status. */
#else
	uint64_t antc_status                  : 1;
	uint64_t bfc_status                   : 1;
	uint64_t gcp_status                   : 1;
	uint64_t cfg_status                   : 1;
	uint64_t inpb_status                  : 1;
	uint64_t frb_status                   : 1;
	uint64_t bfib_status                  : 1;
	uint64_t xcor_status                  : 1;
	uint64_t wdma_status                  : 1;
	uint64_t fft_status                   : 1;
	uint64_t fdxcorr_status               : 1;
	uint64_t fdrssi_status                : 1;
	uint64_t tsk_status                   : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_ulfe_bist_status0_s       cnf75xx;
};
typedef union cvmx_ulfe_bist_status0 cvmx_ulfe_bist_status0_t;

/**
 * cvmx_ulfe_configuration0#
 *
 * This register space contains the ULFE job configuration for slot 0.
 *
 */
union cvmx_ulfe_configuration0x {
	uint64_t u64;
	struct cvmx_ulfe_configuration0x_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_0_63                : 64;
#else
	uint64_t reserved_0_63                : 64;
#endif
	} s;
	struct cvmx_ulfe_configuration0x_cnf75xx {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_0                : 64;
#else
	uint64_t reserved_63_0                : 64;
#endif
	} cnf75xx;
};
typedef union cvmx_ulfe_configuration0x cvmx_ulfe_configuration0x_t;

/**
 * cvmx_ulfe_configuration1#
 *
 * This register space contains the ULFE job configuration for slot 1.
 *
 */
union cvmx_ulfe_configuration1x {
	uint64_t u64;
	struct cvmx_ulfe_configuration1x_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_0_63                : 64;
#else
	uint64_t reserved_0_63                : 64;
#endif
	} s;
	struct cvmx_ulfe_configuration1x_cnf75xx {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_63_0                : 64;
#else
	uint64_t reserved_63_0                : 64;
#endif
	} cnf75xx;
};
typedef union cvmx_ulfe_configuration1x cvmx_ulfe_configuration1x_t;

/**
 * cvmx_ulfe_control
 *
 * This register is used to start ULFE HAB processing
 *
 */
union cvmx_ulfe_control {
	uint64_t u64;
	struct cvmx_ulfe_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID for slot 1. */
	uint64_t jobid0                       : 16; /**< Job ID for slot 0. */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< When written to 1, start ULFE processing using configuration slot 1.
                                                         This bit automatically clears to 0. Writes are ignored when
                                                         configuration slot 1 is already busy (i.e., ULFE_STATUS[STATUS1] = 1). */
	uint64_t start0                       : 1;  /**< When written to 1, start ULFE processing using configuration slot 0.
                                                         This bit automatically clears to 0. Writes are ignored when
                                                         configuration slot 1 is already busy (i.e., ULFE_STATUS[STATUS0] = 1). */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_ulfe_control_s            cnf75xx;
};
typedef union cvmx_ulfe_control cvmx_ulfe_control_t;

/**
 * cvmx_ulfe_ecc_control0
 */
union cvmx_ulfe_ecc_control0 {
	uint64_t u64;
	struct cvmx_ulfe_ecc_control0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t tsk_flip_syn                 : 2;  /**< Task FIFO memory syndrome flip bits. */
	uint64_t tsk_cor_dis                  : 1;  /**< Task FIFO memory ECC disable. */
	uint64_t bfc1_flip_syn                : 2;  /**< Beam former coefficient 1 memory syndrome flip bits. */
	uint64_t bfc1_cor_dis                 : 1;  /**< Beam former coefficient 1 memory ECC disable. */
	uint64_t bfc0_flip_syn                : 2;  /**< Beam former coefficient 0 memory syndrome flip bits. */
	uint64_t bfc0_cor_dis                 : 1;  /**< Beam former coefficient 0 memory ECC disable. */
	uint64_t cfg1_flip_syn                : 2;  /**< Configuration 1 memory syndrome flip bits. */
	uint64_t cfg1_cor_dis                 : 1;  /**< Configuration 1 memory ECC disable. */
	uint64_t cfg0_flip_syn                : 2;  /**< Configuration 0 memory syndrome flip bits. */
	uint64_t cfg0_cor_dis                 : 1;  /**< Configuration 0 memory ECC disable. */
#else
	uint64_t cfg0_cor_dis                 : 1;
	uint64_t cfg0_flip_syn                : 2;
	uint64_t cfg1_cor_dis                 : 1;
	uint64_t cfg1_flip_syn                : 2;
	uint64_t bfc0_cor_dis                 : 1;
	uint64_t bfc0_flip_syn                : 2;
	uint64_t bfc1_cor_dis                 : 1;
	uint64_t bfc1_flip_syn                : 2;
	uint64_t tsk_cor_dis                  : 1;
	uint64_t tsk_flip_syn                 : 2;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_ulfe_ecc_control0_s       cnf75xx;
};
typedef union cvmx_ulfe_ecc_control0 cvmx_ulfe_ecc_control0_t;

/**
 * cvmx_ulfe_ecc_enable0
 *
 * This register enables ECC status to be detected.
 *
 */
union cvmx_ulfe_ecc_enable0 {
	uint64_t u64;
	struct cvmx_ulfe_ecc_enable0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t tsk_dbe_enable               : 1;  /**< Task FIFO memory double bit error enable. */
	uint64_t tsk_sbe_enable               : 1;  /**< Task FIFO memory single bit error enable. */
	uint64_t bfc1_dbe_enable              : 1;  /**< Beam-former coefficient 1 memory double bit error enable. */
	uint64_t bfc1_sbe_enable              : 1;  /**< Beam-former coefficient 1 memory single bit error enable. */
	uint64_t bfc0_dbe_enable              : 1;  /**< Beam-former coefficient 0 memory double bit error enable. */
	uint64_t bfc0_sbe_enable              : 1;  /**< Beam-former coefficient 0 memory single bit error enable. */
	uint64_t cfg1_dbe_enable              : 1;  /**< Configuration 1 memory double bit error enable. */
	uint64_t cfg1_sbe_enable              : 1;  /**< Configuration 1 memory single bit error enable. */
	uint64_t cfg0_dbe_enable              : 1;  /**< Configuration 0 memory double bit error enable. */
	uint64_t cfg0_sbe_enable              : 1;  /**< Configuration 0 memory single bit error enable. */
#else
	uint64_t cfg0_sbe_enable              : 1;
	uint64_t cfg0_dbe_enable              : 1;
	uint64_t cfg1_sbe_enable              : 1;
	uint64_t cfg1_dbe_enable              : 1;
	uint64_t bfc0_sbe_enable              : 1;
	uint64_t bfc0_dbe_enable              : 1;
	uint64_t bfc1_sbe_enable              : 1;
	uint64_t bfc1_dbe_enable              : 1;
	uint64_t tsk_sbe_enable               : 1;
	uint64_t tsk_dbe_enable               : 1;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_ulfe_ecc_enable0_s        cnf75xx;
};
typedef union cvmx_ulfe_ecc_enable0 cvmx_ulfe_ecc_enable0_t;

/**
 * cvmx_ulfe_ecc_status0
 */
union cvmx_ulfe_ecc_status0 {
	uint64_t u64;
	struct cvmx_ulfe_ecc_status0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t tsk_dbe                      : 1;  /**< Task FIFO memory double bit error. */
	uint64_t tsk_sbe                      : 1;  /**< Task FIFO memory single bit error. */
	uint64_t bfc1_dbe                     : 1;  /**< Beam-former coefficient 1 memory double bit error. */
	uint64_t bfc1_sbe                     : 1;  /**< Beam-former coefficient 1 memory single bit error. */
	uint64_t bfc0_dbe                     : 1;  /**< Beam-former coefficient 0 memory double bit error. */
	uint64_t bfc0_sbe                     : 1;  /**< Beam-former coefficient 0 memory single bit error. */
	uint64_t cfg1_dbe                     : 1;  /**< Configuration 1 memory double bit error. */
	uint64_t cfg1_sbe                     : 1;  /**< Configuration 1 memory single bit error. */
	uint64_t cfg0_dbe                     : 1;  /**< Configuration 0 memory double bit error. */
	uint64_t cfg0_sbe                     : 1;  /**< Configuration 0 memory single bit error. */
#else
	uint64_t cfg0_sbe                     : 1;
	uint64_t cfg0_dbe                     : 1;
	uint64_t cfg1_sbe                     : 1;
	uint64_t cfg1_dbe                     : 1;
	uint64_t bfc0_sbe                     : 1;
	uint64_t bfc0_dbe                     : 1;
	uint64_t bfc1_sbe                     : 1;
	uint64_t bfc1_dbe                     : 1;
	uint64_t tsk_sbe                      : 1;
	uint64_t tsk_dbe                      : 1;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_ulfe_ecc_status0_s        cnf75xx;
};
typedef union cvmx_ulfe_ecc_status0 cvmx_ulfe_ecc_status0_t;

/**
 * cvmx_ulfe_eco
 */
union cvmx_ulfe_eco {
	uint64_t u64;
	struct cvmx_ulfe_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_ulfe_eco_s                cnf75xx;
};
typedef union cvmx_ulfe_eco cvmx_ulfe_eco_t;

/**
 * cvmx_ulfe_error_enable1
 *
 * This register enables error reporting for errors reported by the ULFE_ERROR_SOURCE1 register.
 *
 */
union cvmx_ulfe_error_enable1 {
	uint64_t u64;
	struct cvmx_ulfe_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t extra_samp_nfat_ena          : 1;  /**< EXTRA_SAMP_NFAT error enable. */
	uint64_t rdma_sos_nfat_ena            : 1;  /**< RDMA_SOS_NFAT error enable. */
	uint64_t ibrd_nfat_ena                : 1;  /**< IBRD_NFAT error enable. */
	uint64_t wtick_sos_nfat_ena           : 1;  /**< WTICK_SOS_NFAT error enable. */
	uint64_t wr_fifo_ov_fat_ena           : 1;  /**< WR_FIFO_OV_FAT error enable. */
#else
	uint64_t wr_fifo_ov_fat_ena           : 1;
	uint64_t wtick_sos_nfat_ena           : 1;
	uint64_t ibrd_nfat_ena                : 1;
	uint64_t rdma_sos_nfat_ena            : 1;
	uint64_t extra_samp_nfat_ena          : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_ulfe_error_enable1_s      cnf75xx;
};
typedef union cvmx_ulfe_error_enable1 cvmx_ulfe_error_enable1_t;

/**
 * cvmx_ulfe_error_source1
 *
 * This register contains error source information.
 * Any write to this register will clear all contents.
 *
 * Fields named X_NFAT indicate nonfatal errors, and those name X_FAT
 * indicate fatal errors. Software must reset ULFE after a fatal error. After
 * a nonfatal error, ULFE can continue to operate correctly, and software
 * should simply record the error and clear the error state.
 */
union cvmx_ulfe_error_source1 {
	uint64_t u64;
	struct cvmx_ulfe_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t err_jobid                    : 16; /**< Job ID for the active job when the error occurred. */
	uint64_t reserved_5_15                : 11;
	uint64_t extra_samp_nfat              : 1;  /**< Extra samples received for subframe. */
	uint64_t rdma_sos_nfat                : 1;  /**< RDMA active when SOS occurs. */
	uint64_t ibrd_nfat                    : 1;  /**< ULFE input buffer does not have sufficient data when PSTSK begins execution. */
	uint64_t wtick_sos_nfat               : 1;  /**< WTICK wait command active when SOS occurs. */
	uint64_t wr_fifo_ov_fat               : 1;  /**< ULFE internal write FIFO overflow. This is not the MHBW FIFO overflow,
                                                         but an internal buffer within the ULFE HAB. */
#else
	uint64_t wr_fifo_ov_fat               : 1;
	uint64_t wtick_sos_nfat               : 1;
	uint64_t ibrd_nfat                    : 1;
	uint64_t rdma_sos_nfat                : 1;
	uint64_t extra_samp_nfat              : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t err_jobid                    : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_ulfe_error_source1_s      cnf75xx;
};
typedef union cvmx_ulfe_error_source1 cvmx_ulfe_error_source1_t;

/**
 * cvmx_ulfe_indirect_addr
 *
 * This register provides the address for indirect accesses to ULFE's
 * internal memories. To read/write an internal memory, software first
 * writes an address to ULFE_INDIRECT_ADDR, and then reads/writes the data
 * register ULFE_INDIRECT_DATA.
 *
 * To access the beam-former coefficient semi-static parameters, use the
 * addresses definied by the ULFE_IND_ADR_BFC0_E enumeration.
 *
 * These registers should only be used for debug and diagnostic use.
 * Reading/writing memories while ULFE is active might result in undefined
 * behavior.
 */
union cvmx_ulfe_indirect_addr {
	uint64_t u64;
	struct cvmx_ulfe_indirect_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t indirect_addr                : 17; /**< This field provides the address for indirect accesses. */
#else
	uint64_t indirect_addr                : 17;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_ulfe_indirect_addr_s      cnf75xx;
};
typedef union cvmx_ulfe_indirect_addr cvmx_ulfe_indirect_addr_t;

/**
 * cvmx_ulfe_indirect_data
 *
 * This register provides the data for indirect accesses.
 *
 */
union cvmx_ulfe_indirect_data {
	uint64_t u64;
	struct cvmx_ulfe_indirect_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t indirect_data                : 64; /**< This field provides the data for indirect accesses. */
#else
	uint64_t indirect_data                : 64;
#endif
	} s;
	struct cvmx_ulfe_indirect_data_s      cnf75xx;
};
typedef union cvmx_ulfe_indirect_data cvmx_ulfe_indirect_data_t;

/**
 * cvmx_ulfe_inp_control
 *
 * This register provides input control parameters.
 *
 */
union cvmx_ulfe_inp_control {
	uint64_t u64;
	struct cvmx_ulfe_inp_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t sos_rst_delay                : 17; /**< Specifies the number of cycles to delay after receiving SOS_UL before
                                                         reseting the internal tick counter. Note that ULFE receives SOS_UL
                                                         at the same time as it receives the first sample of the subframe. */
#else
	uint64_t sos_rst_delay                : 17;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_ulfe_inp_control_s        cnf75xx;
};
typedef union cvmx_ulfe_inp_control cvmx_ulfe_inp_control_t;

/**
 * cvmx_ulfe_iss_cnt0
 *
 * Back up trigger count for ISS gain report 0.
 *
 */
union cvmx_ulfe_iss_cnt0 {
	uint64_t u64;
	struct cvmx_ulfe_iss_cnt0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< ULFE will write the first ISS gain report COUNT cycles after SOS, if
                                                         it has not already written it by that time. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_ulfe_iss_cnt0_s           cnf75xx;
};
typedef union cvmx_ulfe_iss_cnt0 cvmx_ulfe_iss_cnt0_t;

/**
 * cvmx_ulfe_iss_cnt1
 *
 * Back up trigger count for ISS gain report 1.
 *
 */
union cvmx_ulfe_iss_cnt1 {
	uint64_t u64;
	struct cvmx_ulfe_iss_cnt1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< ULFE will write the second ISS gain report COUNT cycles after SOS, if
                                                         it has not already written it by that time. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_ulfe_iss_cnt1_s           cnf75xx;
};
typedef union cvmx_ulfe_iss_cnt1 cvmx_ulfe_iss_cnt1_t;

/**
 * cvmx_ulfe_iss_cnt2
 *
 * Back up trigger count for ISS gain report 2.
 *
 */
union cvmx_ulfe_iss_cnt2 {
	uint64_t u64;
	struct cvmx_ulfe_iss_cnt2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< ULFE will write the third ISS gain report COUNT cycles after SOS, if
                                                         it has not already written it by that time. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_ulfe_iss_cnt2_s           cnf75xx;
};
typedef union cvmx_ulfe_iss_cnt2 cvmx_ulfe_iss_cnt2_t;

/**
 * cvmx_ulfe_read_err_ena
 *
 * This register enables error reporting for read overflow/underflow errors.
 *
 */
union cvmx_ulfe_read_err_ena {
	uint64_t u64;
	struct cvmx_ulfe_read_err_ena_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t rp0_of_en                    : 1;  /**< Read port 0 overflow enable. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf_en                    : 1;  /**< Read port 0 underflow enable. */
#else
	uint64_t rp0_uf_en                    : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of_en                    : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_ulfe_read_err_ena_s       cnf75xx;
};
typedef union cvmx_ulfe_read_err_ena cvmx_ulfe_read_err_ena_t;

/**
 * cvmx_ulfe_read_err_src
 *
 * This register contains read port error source information.
 * Any write to this register will clear all contents.
 */
union cvmx_ulfe_read_err_src {
	uint64_t u64;
	struct cvmx_ulfe_read_err_src_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid_ufof               : 16; /**< Job ID of the job which reported the overflow/underflow error on read
                                                         port 1. */
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
	struct cvmx_ulfe_read_err_src_s       cnf75xx;
};
typedef union cvmx_ulfe_read_err_src cvmx_ulfe_read_err_src_t;

/**
 * cvmx_ulfe_sos_down_cnt
 *
 * Back up count for SOS window function.
 *
 */
union cvmx_ulfe_sos_down_cnt {
	uint64_t u64;
	struct cvmx_ulfe_sos_down_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< Indicates the number of clock ticks after SOS to open the SOS window. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_ulfe_sos_down_cnt_s       cnf75xx;
};
typedef union cvmx_ulfe_sos_down_cnt cvmx_ulfe_sos_down_cnt_t;

/**
 * cvmx_ulfe_status
 *
 * ULFE Status Register
 *
 */
union cvmx_ulfe_status {
	uint64_t u64;
	struct cvmx_ulfe_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< When set and when at least one job slot is not busy,
                                                         the ULFE is ready to start a new job. */
	uint64_t reserved_2_3                 : 2;
	uint64_t status1                      : 1;  /**< When set, slot 1 is busy processing a job. */
	uint64_t status0                      : 1;  /**< When set, slot 0 is busy processing a job. */
#else
	uint64_t status0                      : 1;
	uint64_t status1                      : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_ulfe_status_s             cnf75xx;
};
typedef union cvmx_ulfe_status cvmx_ulfe_status_t;

/**
 * cvmx_ulfe_td_rssi_cnt
 *
 * Back up trigger count for TD RSSI report
 *
 */
union cvmx_ulfe_td_rssi_cnt {
	uint64_t u64;
	struct cvmx_ulfe_td_rssi_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_24_63               : 40;
	uint64_t count                        : 24; /**< ULFE will write the TD RSSI report COUNT cycles after SOS, if
                                                         it has not already written it by that time. */
#else
	uint64_t count                        : 24;
	uint64_t reserved_24_63               : 40;
#endif
	} s;
	struct cvmx_ulfe_td_rssi_cnt_s        cnf75xx;
};
typedef union cvmx_ulfe_td_rssi_cnt cvmx_ulfe_td_rssi_cnt_t;

#endif
