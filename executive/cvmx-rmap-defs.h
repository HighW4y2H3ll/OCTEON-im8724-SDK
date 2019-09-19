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
 * cvmx-rmap-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon rmap.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_RMAP_DEFS_H__
#define __CVMX_RMAP_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_BIST_STATUS CVMX_RMAP_BIST_STATUS_FUNC()
static inline uint64_t CVMX_RMAP_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400050ull);
}
#else
#define CVMX_RMAP_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B3400050ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_CONTROL CVMX_RMAP_CONTROL_FUNC()
static inline uint64_t CVMX_RMAP_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400000ull);
}
#else
#define CVMX_RMAP_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3400000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ECC_CTRL CVMX_RMAP_ECC_CTRL_FUNC()
static inline uint64_t CVMX_RMAP_ECC_CTRL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ECC_CTRL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400060ull);
}
#else
#define CVMX_RMAP_ECC_CTRL (CVMX_ADD_IO_SEG(0x00011800B3400060ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ECC_ENABLE CVMX_RMAP_ECC_ENABLE_FUNC()
static inline uint64_t CVMX_RMAP_ECC_ENABLE_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ECC_ENABLE not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400070ull);
}
#else
#define CVMX_RMAP_ECC_ENABLE (CVMX_ADD_IO_SEG(0x00011800B3400070ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ECC_STATUS CVMX_RMAP_ECC_STATUS_FUNC()
static inline uint64_t CVMX_RMAP_ECC_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ECC_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400068ull);
}
#else
#define CVMX_RMAP_ECC_STATUS (CVMX_ADD_IO_SEG(0x00011800B3400068ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ECO CVMX_RMAP_ECO_FUNC()
static inline uint64_t CVMX_RMAP_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400008ull);
}
#else
#define CVMX_RMAP_ECO (CVMX_ADD_IO_SEG(0x00011800B3400008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ERROR_ENABLE0 CVMX_RMAP_ERROR_ENABLE0_FUNC()
static inline uint64_t CVMX_RMAP_ERROR_ENABLE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ERROR_ENABLE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400040ull);
}
#else
#define CVMX_RMAP_ERROR_ENABLE0 (CVMX_ADD_IO_SEG(0x00011800B3400040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ERROR_ENABLE1 CVMX_RMAP_ERROR_ENABLE1_FUNC()
static inline uint64_t CVMX_RMAP_ERROR_ENABLE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ERROR_ENABLE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400048ull);
}
#else
#define CVMX_RMAP_ERROR_ENABLE1 (CVMX_ADD_IO_SEG(0x00011800B3400048ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ERROR_SOURCE0 CVMX_RMAP_ERROR_SOURCE0_FUNC()
static inline uint64_t CVMX_RMAP_ERROR_SOURCE0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ERROR_SOURCE0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400030ull);
}
#else
#define CVMX_RMAP_ERROR_SOURCE0 (CVMX_ADD_IO_SEG(0x00011800B3400030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_ERROR_SOURCE1 CVMX_RMAP_ERROR_SOURCE1_FUNC()
static inline uint64_t CVMX_RMAP_ERROR_SOURCE1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_ERROR_SOURCE1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400038ull);
}
#else
#define CVMX_RMAP_ERROR_SOURCE1 (CVMX_ADD_IO_SEG(0x00011800B3400038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD0_CFG0 CVMX_RMAP_JD0_CFG0_FUNC()
static inline uint64_t CVMX_RMAP_JD0_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD0_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3402000ull);
}
#else
#define CVMX_RMAP_JD0_CFG0 (CVMX_ADD_IO_SEG(0x00011800B3402000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD0_CFG1 CVMX_RMAP_JD0_CFG1_FUNC()
static inline uint64_t CVMX_RMAP_JD0_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD0_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3402008ull);
}
#else
#define CVMX_RMAP_JD0_CFG1 (CVMX_ADD_IO_SEG(0x00011800B3402008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD0_CFG2 CVMX_RMAP_JD0_CFG2_FUNC()
static inline uint64_t CVMX_RMAP_JD0_CFG2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD0_CFG2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3402010ull);
}
#else
#define CVMX_RMAP_JD0_CFG2 (CVMX_ADD_IO_SEG(0x00011800B3402010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD0_CFG3 CVMX_RMAP_JD0_CFG3_FUNC()
static inline uint64_t CVMX_RMAP_JD0_CFG3_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD0_CFG3 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3402018ull);
}
#else
#define CVMX_RMAP_JD0_CFG3 (CVMX_ADD_IO_SEG(0x00011800B3402018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD1_CFG0 CVMX_RMAP_JD1_CFG0_FUNC()
static inline uint64_t CVMX_RMAP_JD1_CFG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD1_CFG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3404000ull);
}
#else
#define CVMX_RMAP_JD1_CFG0 (CVMX_ADD_IO_SEG(0x00011800B3404000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD1_CFG1 CVMX_RMAP_JD1_CFG1_FUNC()
static inline uint64_t CVMX_RMAP_JD1_CFG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD1_CFG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3404008ull);
}
#else
#define CVMX_RMAP_JD1_CFG1 (CVMX_ADD_IO_SEG(0x00011800B3404008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD1_CFG2 CVMX_RMAP_JD1_CFG2_FUNC()
static inline uint64_t CVMX_RMAP_JD1_CFG2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD1_CFG2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3404010ull);
}
#else
#define CVMX_RMAP_JD1_CFG2 (CVMX_ADD_IO_SEG(0x00011800B3404010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_JD1_CFG3 CVMX_RMAP_JD1_CFG3_FUNC()
static inline uint64_t CVMX_RMAP_JD1_CFG3_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_JD1_CFG3 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3404018ull);
}
#else
#define CVMX_RMAP_JD1_CFG3 (CVMX_ADD_IO_SEG(0x00011800B3404018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_SCRATCH CVMX_RMAP_SCRATCH_FUNC()
static inline uint64_t CVMX_RMAP_SCRATCH_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_SCRATCH not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400080ull);
}
#else
#define CVMX_RMAP_SCRATCH (CVMX_ADD_IO_SEG(0x00011800B3400080ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_STATUS CVMX_RMAP_STATUS_FUNC()
static inline uint64_t CVMX_RMAP_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3400018ull);
}
#else
#define CVMX_RMAP_STATUS (CVMX_ADD_IO_SEG(0x00011800B3400018ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_CONFIG0 CVMX_RMAP_TC_CONFIG0_FUNC()
static inline uint64_t CVMX_RMAP_TC_CONFIG0_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_CONFIG0 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401100ull);
}
#else
#define CVMX_RMAP_TC_CONFIG0 (CVMX_ADD_IO_SEG(0x00011800B3401100ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_CONFIG1 CVMX_RMAP_TC_CONFIG1_FUNC()
static inline uint64_t CVMX_RMAP_TC_CONFIG1_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_CONFIG1 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401108ull);
}
#else
#define CVMX_RMAP_TC_CONFIG1 (CVMX_ADD_IO_SEG(0x00011800B3401108ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_CONFIG2 CVMX_RMAP_TC_CONFIG2_FUNC()
static inline uint64_t CVMX_RMAP_TC_CONFIG2_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_CONFIG2 not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401110ull);
}
#else
#define CVMX_RMAP_TC_CONFIG2 (CVMX_ADD_IO_SEG(0x00011800B3401110ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_CONFIG_ERR_FLAGS CVMX_RMAP_TC_CONFIG_ERR_FLAGS_FUNC()
static inline uint64_t CVMX_RMAP_TC_CONFIG_ERR_FLAGS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_CONFIG_ERR_FLAGS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401040ull);
}
#else
#define CVMX_RMAP_TC_CONFIG_ERR_FLAGS (CVMX_ADD_IO_SEG(0x00011800B3401040ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_ERROR CVMX_RMAP_TC_ERROR_FUNC()
static inline uint64_t CVMX_RMAP_TC_ERROR_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_ERROR not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401038ull);
}
#else
#define CVMX_RMAP_TC_ERROR (CVMX_ADD_IO_SEG(0x00011800B3401038ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_ERROR_MASK CVMX_RMAP_TC_ERROR_MASK_FUNC()
static inline uint64_t CVMX_RMAP_TC_ERROR_MASK_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_ERROR_MASK not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401030ull);
}
#else
#define CVMX_RMAP_TC_ERROR_MASK (CVMX_ADD_IO_SEG(0x00011800B3401030ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_MAIN_CONTROL CVMX_RMAP_TC_MAIN_CONTROL_FUNC()
static inline uint64_t CVMX_RMAP_TC_MAIN_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_MAIN_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401010ull);
}
#else
#define CVMX_RMAP_TC_MAIN_CONTROL (CVMX_ADD_IO_SEG(0x00011800B3401010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_MAIN_RESET CVMX_RMAP_TC_MAIN_RESET_FUNC()
static inline uint64_t CVMX_RMAP_TC_MAIN_RESET_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_MAIN_RESET not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401000ull);
}
#else
#define CVMX_RMAP_TC_MAIN_RESET (CVMX_ADD_IO_SEG(0x00011800B3401000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_RMAP_TC_MAIN_START CVMX_RMAP_TC_MAIN_START_FUNC()
static inline uint64_t CVMX_RMAP_TC_MAIN_START_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_RMAP_TC_MAIN_START not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B3401008ull);
}
#else
#define CVMX_RMAP_TC_MAIN_START (CVMX_ADD_IO_SEG(0x00011800B3401008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RMAP_TC_STATUSX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 1)))))
		cvmx_warn("CVMX_RMAP_TC_STATUSX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B3401020ull) + ((offset) & 1) * 8;
}
#else
#define CVMX_RMAP_TC_STATUSX(offset) (CVMX_ADD_IO_SEG(0x00011800B3401020ull) + ((offset) & 1) * 8)
#endif

/**
 * cvmx_rmap_bist_status
 */
union cvmx_rmap_bist_status {
	uint64_t u64;
	struct cvmx_rmap_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_7_63                : 57;
	uint64_t bstatus                      : 7;  /**< Memory BIST status of RMAP memories. */
#else
	uint64_t bstatus                      : 7;
	uint64_t reserved_7_63                : 57;
#endif
	} s;
	struct cvmx_rmap_bist_status_s        cnf75xx;
};
typedef union cvmx_rmap_bist_status cvmx_rmap_bist_status_t;

/**
 * cvmx_rmap_control
 *
 * This register is used to start RMAP HAB processing
 *
 */
union cvmx_rmap_control {
	uint64_t u64;
	struct cvmx_rmap_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_48_63               : 16;
	uint64_t jobid1                       : 16; /**< Job ID1 */
	uint64_t jobid0                       : 16; /**< Job ID0 */
	uint64_t reserved_2_15                : 14;
	uint64_t start1                       : 1;  /**< Write 1 to start processing the job in configuration slot 1. This bit auto-clears.
                                                         Writes are ignored when the configuration slot is busy (RMAP_STATUS[STATUS1] = 1). */
	uint64_t start0                       : 1;  /**< Write 1 to start processing the job in configuration slot 0. This bit auto-clears.
                                                         Writes are ignored when the configuration slot is busy (RMAP_STATUS[STATUS0] = 1). */
#else
	uint64_t start0                       : 1;
	uint64_t start1                       : 1;
	uint64_t reserved_2_15                : 14;
	uint64_t jobid0                       : 16;
	uint64_t jobid1                       : 16;
	uint64_t reserved_48_63               : 16;
#endif
	} s;
	struct cvmx_rmap_control_s            cnf75xx;
};
typedef union cvmx_rmap_control cvmx_rmap_control_t;

/**
 * cvmx_rmap_ecc_ctrl
 */
union cvmx_rmap_ecc_ctrl {
	uint64_t u64;
	struct cvmx_rmap_ecc_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t dft_oif_flip_syn             : 2;  /**< RMAP DFT OIF memory flip syndrome bits. */
	uint64_t dft_oif_cor_dis              : 1;  /**< RMAP DFT OIF memory ECC correction disable. */
	uint64_t dft_pong_flip_syn            : 2;  /**< RMAP DFT PONG memory flip syndrome bits. */
	uint64_t dft_pong_cor_dis             : 1;  /**< RMAP DFT PONG memory ECC correction disable. */
	uint64_t dft_ping_flip_syn            : 2;  /**< RMAP DFT PING memory flip syndrome bits. */
	uint64_t dft_ping_cor_dis             : 1;  /**< RMAP DFT PING memory ECC correction disable. */
	uint64_t mod1_flip_syn                : 2;  /**< RMAP MOD1 memory flip syndrome bits. */
	uint64_t mod1_cor_dis                 : 1;  /**< RMAP MOD1 memory ECC correction disable. */
	uint64_t mod0_flip_syn                : 2;  /**< RMAP MOD0 memory flip syndrome bits. */
	uint64_t mod0_cor_dis                 : 1;  /**< RMAP MOD0 memory ECC correction disable. */
#else
	uint64_t mod0_cor_dis                 : 1;
	uint64_t mod0_flip_syn                : 2;
	uint64_t mod1_cor_dis                 : 1;
	uint64_t mod1_flip_syn                : 2;
	uint64_t dft_ping_cor_dis             : 1;
	uint64_t dft_ping_flip_syn            : 2;
	uint64_t dft_pong_cor_dis             : 1;
	uint64_t dft_pong_flip_syn            : 2;
	uint64_t dft_oif_cor_dis              : 1;
	uint64_t dft_oif_flip_syn             : 2;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_rmap_ecc_ctrl_s           cnf75xx;
};
typedef union cvmx_rmap_ecc_ctrl cvmx_rmap_ecc_ctrl_t;

/**
 * cvmx_rmap_ecc_enable
 *
 * This register enables ECC error reporting.
 *
 */
union cvmx_rmap_ecc_enable {
	uint64_t u64;
	struct cvmx_rmap_ecc_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_37_63               : 27;
	uint64_t dft_oif_dbe                  : 1;  /**< RMAP DFT OIF memory double bit error. */
	uint64_t dft_pong_dbe                 : 1;  /**< RMAP DFT PONG memory double bit error. */
	uint64_t dft_ping_dbe                 : 1;  /**< RMAP DFT PING memory double bit error. */
	uint64_t mod1_dbe                     : 1;  /**< RMAP MOD1 memory double bit error. */
	uint64_t mod0_dbe                     : 1;  /**< RMAP MOD0 memory double bit error. */
	uint64_t reserved_5_31                : 27;
	uint64_t dft_oif_sbe                  : 1;  /**< RMAP DFT OIF memory single bit error. */
	uint64_t dft_pong_sbe                 : 1;  /**< RMAP DFT PONG memory single bit error. */
	uint64_t dft_ping_sbe                 : 1;  /**< RMAP DFT PING memory single bit error. */
	uint64_t mod1_sbe                     : 1;  /**< RMAP MOD1 memory single bit error. */
	uint64_t mod0_sbe                     : 1;  /**< RMAP MOD0 memory single bit error. */
#else
	uint64_t mod0_sbe                     : 1;
	uint64_t mod1_sbe                     : 1;
	uint64_t dft_ping_sbe                 : 1;
	uint64_t dft_pong_sbe                 : 1;
	uint64_t dft_oif_sbe                  : 1;
	uint64_t reserved_5_31                : 27;
	uint64_t mod0_dbe                     : 1;
	uint64_t mod1_dbe                     : 1;
	uint64_t dft_ping_dbe                 : 1;
	uint64_t dft_pong_dbe                 : 1;
	uint64_t dft_oif_dbe                  : 1;
	uint64_t reserved_37_63               : 27;
#endif
	} s;
	struct cvmx_rmap_ecc_enable_s         cnf75xx;
};
typedef union cvmx_rmap_ecc_enable cvmx_rmap_ecc_enable_t;

/**
 * cvmx_rmap_ecc_status
 */
union cvmx_rmap_ecc_status {
	uint64_t u64;
	struct cvmx_rmap_ecc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_37_63               : 27;
	uint64_t dft_oif_dbe                  : 1;  /**< RMAP DFT OIF memory double bit error. */
	uint64_t dft_pong_dbe                 : 1;  /**< RMAP DFT PONG memory double bit error. */
	uint64_t dft_ping_dbe                 : 1;  /**< RMAP DFT PING memory double bit error. */
	uint64_t mod1_dbe                     : 1;  /**< RMAP MOD1 memory double bit error. */
	uint64_t mod0_dbe                     : 1;  /**< RMAP MOD0 memory double bit error. */
	uint64_t reserved_5_31                : 27;
	uint64_t dft_oif_sbe                  : 1;  /**< RMAP DFT OIF memory single bit error. */
	uint64_t dft_pong_sbe                 : 1;  /**< RMAP DFT PONG memory single bit error. */
	uint64_t dft_ping_sbe                 : 1;  /**< RMAP DFT PING memory single bit error. */
	uint64_t mod1_sbe                     : 1;  /**< RMAP MOD1 memory single bit error. */
	uint64_t mod0_sbe                     : 1;  /**< RMAP MOD0 memory single bit error. */
#else
	uint64_t mod0_sbe                     : 1;
	uint64_t mod1_sbe                     : 1;
	uint64_t dft_ping_sbe                 : 1;
	uint64_t dft_pong_sbe                 : 1;
	uint64_t dft_oif_sbe                  : 1;
	uint64_t reserved_5_31                : 27;
	uint64_t mod0_dbe                     : 1;
	uint64_t mod1_dbe                     : 1;
	uint64_t dft_ping_dbe                 : 1;
	uint64_t dft_pong_dbe                 : 1;
	uint64_t dft_oif_dbe                  : 1;
	uint64_t reserved_37_63               : 27;
#endif
	} s;
	struct cvmx_rmap_ecc_status_s         cnf75xx;
};
typedef union cvmx_rmap_ecc_status cvmx_rmap_ecc_status_t;

/**
 * cvmx_rmap_eco
 */
union cvmx_rmap_eco {
	uint64_t u64;
	struct cvmx_rmap_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rmap_eco_s                cnf75xx;
};
typedef union cvmx_rmap_eco cvmx_rmap_eco_t;

/**
 * cvmx_rmap_error_enable0
 *
 * This register enables error reporting for RMAP_ERROR_SOURCE0 register
 *
 */
union cvmx_rmap_error_enable0 {
	uint64_t u64;
	struct cvmx_rmap_error_enable0_s {
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
	struct cvmx_rmap_error_enable0_s      cnf75xx;
};
typedef union cvmx_rmap_error_enable0 cvmx_rmap_error_enable0_t;

/**
 * cvmx_rmap_error_enable1
 *
 * This register enables reporting of job configuration errors.
 *
 */
union cvmx_rmap_error_enable1 {
	uint64_t u64;
	struct cvmx_rmap_error_enable1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t tc_event_en                  : 1;  /**< Enable unexpected event reporting from TC core */
	uint64_t tc_error_en                  : 1;  /**< Enable error reporting from TC core */
	uint64_t reserved_19_31               : 13;
	uint64_t total_re_err_en              : 1;  /**< Enable reporting of inconsistant total RE errors. */
	uint64_t r_prime_mux_err_en           : 1;  /**< Enable reporting of invalid R_PRIME_MUX settings. */
	uint64_t dft_idx_err1_en              : 1;  /**< Enable reporting for invalid combinations of NDFT_INDX and NUM_RB. */
	uint64_t dft_idx_err0_en              : 1;  /**< Enable reporting of invalid NDFT_INDX setting. */
	uint64_t rb_set_err_en                : 1;  /**< Enable reporting of invalid RB_SET0 and RB_SET1 combinations for allocation type 1. */
	uint64_t ri_cat_err_en                : 1;  /**< Enable reporting of invalid RI_ENC_CAT setting. */
	uint64_t ack_cat_err_en               : 1;  /**< Enable reporting of invalid ACK_ENC_CAT setting. */
	uint64_t num_layers_err_en            : 1;  /**< Enable reporting of invalid NUM_LAYERS setting. */
	uint64_t mod_order_err_en             : 1;  /**< Enable reporting of invalid MOD_ORDER setting. */
	uint64_t byte_order_err3_en           : 1;  /**< Enable reporting of invalid ACK_BYTE_ORDER setting. */
	uint64_t byte_order_err2_en           : 1;  /**< Enable reporting of invalid RI_BYTE_ORDER setting. */
	uint64_t byte_order_err1_en           : 1;  /**< Enable reporting of invalid CQI_BYTE_ORDER setting. */
	uint64_t byte_order_err0_en           : 1;  /**< Enable reporting of invalid DATA_BYTE_ORDER setting. */
	uint64_t ack_err1_en                  : 1;  /**< Enable reporting of nonzero NUM_ACK_RE with zero ACK_ENC_CAT. */
	uint64_t ack_err0_en                  : 1;  /**< Enable reporting of zero NUM_ACK_RE with nonzero ACK_ENC_CAT. */
	uint64_t ri_err1_en                   : 1;  /**< Enable reporting of nonzero NUM_RI_RE with zero RI_ENC_CAT. */
	uint64_t ri_err0_en                   : 1;  /**< Enable reporting of zero NUM_RI_RE with nonzero RI_ENC_CAT. */
	uint64_t cqi_err1_en                  : 1;  /**< Enable reporting of nonzero NUM_CQI_RE with zero NUM_ENCODED_CQI_BITS. */
	uint64_t cqi_err0_en                  : 1;  /**< Enable reporting of zero NUM_CQI_RE with nonzero NUM_ENCODED_CQI_BITS. */
#else
	uint64_t cqi_err0_en                  : 1;
	uint64_t cqi_err1_en                  : 1;
	uint64_t ri_err0_en                   : 1;
	uint64_t ri_err1_en                   : 1;
	uint64_t ack_err0_en                  : 1;
	uint64_t ack_err1_en                  : 1;
	uint64_t byte_order_err0_en           : 1;
	uint64_t byte_order_err1_en           : 1;
	uint64_t byte_order_err2_en           : 1;
	uint64_t byte_order_err3_en           : 1;
	uint64_t mod_order_err_en             : 1;
	uint64_t num_layers_err_en            : 1;
	uint64_t ack_cat_err_en               : 1;
	uint64_t ri_cat_err_en                : 1;
	uint64_t rb_set_err_en                : 1;
	uint64_t dft_idx_err0_en              : 1;
	uint64_t dft_idx_err1_en              : 1;
	uint64_t r_prime_mux_err_en           : 1;
	uint64_t total_re_err_en              : 1;
	uint64_t reserved_19_31               : 13;
	uint64_t tc_error_en                  : 1;
	uint64_t tc_event_en                  : 1;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_rmap_error_enable1_s      cnf75xx;
};
typedef union cvmx_rmap_error_enable1 cvmx_rmap_error_enable1_t;

/**
 * cvmx_rmap_error_source0
 *
 * This is register contains information about read port overflows and underflows.
 *
 */
union cvmx_rmap_error_source0 {
	uint64_t u64;
	struct cvmx_rmap_error_source0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rp0_jobid                    : 16; /**< Job ID for the job that caused either an underflow or overflow on port 0. */
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_of                       : 1;  /**< Set to 1 when an overflow occurs on port 0. Cleared by a write to one. */
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_uf                       : 1;  /**< Set to 1 when an underflow occurs on port 0. Cleared by a write to one. */
#else
	uint64_t rp0_uf                       : 1;
	uint64_t reserved_1_3                 : 3;
	uint64_t rp0_of                       : 1;
	uint64_t reserved_5_15                : 11;
	uint64_t rp0_jobid                    : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rmap_error_source0_s      cnf75xx;
};
typedef union cvmx_rmap_error_source0 cvmx_rmap_error_source0_t;

/**
 * cvmx_rmap_error_source1
 *
 * This register contains information about job configuration errors.
 *
 */
union cvmx_rmap_error_source1 {
	uint64_t u64;
	struct cvmx_rmap_error_source1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t tc_event                     : 1;  /**< N/A */
	uint64_t tc_error                     : 1;  /**< Error within multiplexing, interleaving and scambling sub-block. See
                                                         RMAP_TC_ERROR for additional error information. */
	uint64_t reserved_19_31               : 13;
	uint64_t total_re_err                 : 1;  /**< Inconsistant total RE detected - (R_PRIME_MUX x NUM_SYMB_PUSCH) != (G_PRIME + NUM_CQI_RE +
                                                         NUM_RI_RE). */
	uint64_t r_prime_mux_err              : 1;  /**< Invalid R_PRIME_MUX setting. R_PRIME_MUX must be NUM_RB*3. */
	uint64_t dft_idx_err1                 : 1;  /**< Invalid combination of DFT index and NUM_RB detected. The combination of
                                                         NDFT_INDX and NUM_RB, valid combinations are listed in the DFT index table. */
	uint64_t dft_idx_err0                 : 1;  /**< Invalid DFT_INDX setting. */
	uint64_t rb_set_err                   : 1;  /**< Invalid combination of LENGTH_RB_SET0 and LENGTH_RB_SET1 for
                                                         allocation type 1 - they must sum to NUM_RB. */
	uint64_t ri_cat_err                   : 1;  /**< Invalid RI_ENC_CAT setting. */
	uint64_t ack_cat_err                  : 1;  /**< Invalid ACT_ENC_CAT setting. */
	uint64_t num_layers_err               : 1;  /**< Invalid NUM_LAYERS setting. */
	uint64_t mod_order_err                : 1;  /**< Invalid MOD_ORDER setting. */
	uint64_t byte_order_err3              : 1;  /**< Invalid ACK_BYTE_ORDER setting. */
	uint64_t byte_order_err2              : 1;  /**< Invalid RI_BYTE_ORDER setting. */
	uint64_t byte_order_err1              : 1;  /**< Invalid CQI_BYTE_ORDER setting. */
	uint64_t byte_order_err0              : 1;  /**< Invalid DATA_BYTE_ORDER setting. */
	uint64_t ack_err1                     : 1;  /**< Inconsistent ACK parameters - ACK_ENC_CAT was zero but NUM_ACK_RE was
                                                         not zero. */
	uint64_t ack_err0                     : 1;  /**< Inconsistent ACK parameters - NUM_ACK_RE was zero but ACK_ENC_CAT was
                                                         not zero. */
	uint64_t ri_err1                      : 1;  /**< Inconsistent RI parameters - RI_ENC_CAT was zero but NUM_RI_RE was
                                                         not zero. */
	uint64_t ri_err0                      : 1;  /**< Inconsistent RI parameters - NUM_RI_RE was zero but RI_ENC_CAT was not zero. */
	uint64_t cqi_err1                     : 1;  /**< Inconsistent CQI parameters - NUM_ENCODED_CQI_BITS was zero but NUM_CQI_RE was
                                                         not zero. */
	uint64_t cqi_err0                     : 1;  /**< Inconsistent CQI parameters - NUM_CQI_RE was zero but NUM_ENCODED_CQI_BITS was
                                                         not zero. */
#else
	uint64_t cqi_err0                     : 1;
	uint64_t cqi_err1                     : 1;
	uint64_t ri_err0                      : 1;
	uint64_t ri_err1                      : 1;
	uint64_t ack_err0                     : 1;
	uint64_t ack_err1                     : 1;
	uint64_t byte_order_err0              : 1;
	uint64_t byte_order_err1              : 1;
	uint64_t byte_order_err2              : 1;
	uint64_t byte_order_err3              : 1;
	uint64_t mod_order_err                : 1;
	uint64_t num_layers_err               : 1;
	uint64_t ack_cat_err                  : 1;
	uint64_t ri_cat_err                   : 1;
	uint64_t rb_set_err                   : 1;
	uint64_t dft_idx_err0                 : 1;
	uint64_t dft_idx_err1                 : 1;
	uint64_t r_prime_mux_err              : 1;
	uint64_t total_re_err                 : 1;
	uint64_t reserved_19_31               : 13;
	uint64_t tc_error                     : 1;
	uint64_t tc_event                     : 1;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_rmap_error_source1_s      cnf75xx;
};
typedef union cvmx_rmap_error_source1 cvmx_rmap_error_source1_t;

/**
 * cvmx_rmap_jd0_cfg0
 *
 * This register stores the first job configuration word for slot 0.
 *
 */
union cvmx_rmap_jd0_cfg0 {
	uint64_t u64;
	struct cvmx_rmap_jd0_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t bypass_mod                   : 1;  /**< Modulator bypass is not supported. This field must be set to zero. */
	uint64_t bypass_dft                   : 1;  /**< DFT bypass.  If set to 1, DFT is bypassed */
	uint64_t reserved_59_59               : 1;
	uint64_t data_bit_order               : 1;  /**< Select bit order for data input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t data_byte_order              : 2;  /**< Select byte order for data input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_55_55               : 1;
	uint64_t cqi_bit_order                : 1;  /**< Select bit order for CQI input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t cqi_byte_order               : 2;  /**< Select byte order for CQI input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_51_51               : 1;
	uint64_t ri_bit_order                 : 1;  /**< Select bit order for RI input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t ri_byte_order                : 2;  /**< Select byte order for RI input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_47_47               : 1;
	uint64_t ack_bit_order                : 1;  /**< Select bit order for ACK input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t ack_byte_order               : 2;  /**< Select byte order for ACK input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_42_43               : 2;
	uint64_t num_encoded_cqi_bits         : 18; /**< The number of encoded cqi bits.
                                                         When CQI is RM encoded, [NUM_ENCODED_CQI_BITS] must be 32. When
                                                         CQI uses convolutional coding, it may be either partially or fully
                                                         rate-matched, in which case the size should be equal to the
                                                         REENC_RM_OUT_SIZE from the VDEC job configuration that produced the
                                                         CQI bits. Valid range is 0 to 186624. */
	uint64_t reserved_22_23               : 2;
	uint64_t g_prime                      : 14; /**< Total number of coded symbols in the transport block.
                                                         The total number of coded bits is [G_PRIME]*[NUM_LAYERS]*[MOD_ORDER].
                                                         Valid range is 0 to 15552. */
	uint64_t reserved_7_7                 : 1;
	uint64_t mod_order                    : 3;  /**< Modulation order:
                                                         0x2 = QPSK.
                                                         0x4 = 16-QAM.
                                                         0x6 = 64-QAM. */
	uint64_t reserved_2_3                 : 2;
	uint64_t num_layers                   : 2;  /**< Number of layers for PUSCH channel. Must be either 0x1 or 0x2. */
#else
	uint64_t num_layers                   : 2;
	uint64_t reserved_2_3                 : 2;
	uint64_t mod_order                    : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t g_prime                      : 14;
	uint64_t reserved_22_23               : 2;
	uint64_t num_encoded_cqi_bits         : 18;
	uint64_t reserved_42_43               : 2;
	uint64_t ack_byte_order               : 2;
	uint64_t ack_bit_order                : 1;
	uint64_t reserved_47_47               : 1;
	uint64_t ri_byte_order                : 2;
	uint64_t ri_bit_order                 : 1;
	uint64_t reserved_51_51               : 1;
	uint64_t cqi_byte_order               : 2;
	uint64_t cqi_bit_order                : 1;
	uint64_t reserved_55_55               : 1;
	uint64_t data_byte_order              : 2;
	uint64_t data_bit_order               : 1;
	uint64_t reserved_59_59               : 1;
	uint64_t bypass_dft                   : 1;
	uint64_t bypass_mod                   : 1;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rmap_jd0_cfg0_s           cnf75xx;
};
typedef union cvmx_rmap_jd0_cfg0 cvmx_rmap_jd0_cfg0_t;

/**
 * cvmx_rmap_jd0_cfg1
 *
 * This register stores the second job configuration word for slot 0.
 *
 */
union cvmx_rmap_jd0_cfg1 {
	uint64_t u64;
	struct cvmx_rmap_jd0_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re                   : 14; /**< Number of CQI RE per layer. Valid range is 0 to 15552. */
	uint64_t reserved_45_47               : 3;
	uint64_t num_ri_re                    : 13; /**< Number of RI RE per layer. Valid range is 0 to 5184. */
	uint64_t reserved_29_31               : 3;
	uint64_t num_ack_re                   : 13; /**< Number of ACK RE per layer. Valid range is 0 to 5184. */
	uint64_t reserved_11_15               : 5;
	uint64_t r_prime_mux                  : 11; /**< Size R'mux of the channel interleaver matrix. Max size is 1296. */
#else
	uint64_t r_prime_mux                  : 11;
	uint64_t reserved_11_15               : 5;
	uint64_t num_ack_re                   : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t num_ri_re                    : 13;
	uint64_t reserved_45_47               : 3;
	uint64_t num_cqi_re                   : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rmap_jd0_cfg1_s           cnf75xx;
};
typedef union cvmx_rmap_jd0_cfg1 cvmx_rmap_jd0_cfg1_t;

/**
 * cvmx_rmap_jd0_cfg2
 *
 * This register stores the third job configuration word for slot 0.
 *
 */
union cvmx_rmap_jd0_cfg2 {
	uint64_t u64;
	struct cvmx_rmap_jd0_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t ack_enc_cat                  : 3;  /**< ACK encoding category:
                                                         0x0 = No ACK, number of input bits = 0.
                                                         0x1 = Oack = 1, number of input bits = 1.
                                                         0x2 = Oack = 1 ACK bundling, number of input bits = 1.
                                                         0x3 = Oack = 2, number of input bits = 2.
                                                         0x4 = Oack = 2 ACK bundling, number of input bits = 2.
                                                         0x5 = 3 <= Oack <= 11, number of input bits = 32.
                                                         0x6 = 11 < Oack <= 20, number of input bits = 64.
                                                         0x7 = Reserved. */
	uint64_t reserved_43_43               : 1;
	uint64_t ri_enc_cat                   : 3;  /**< RI encoding category.
                                                         0x0 =  No RI, number of input bits = 0.
                                                         0x1 =  Ori = 1, number of input bits = 1.
                                                         0x2 =  Ori = 2, number of input bits = 2.
                                                         0x3 =  3 <= Ori <= 11, number of input bits = 32.
                                                         0x4 =  11 < Ori <= 20, number of input bits = 64.
                                                         0x5 - 0x7: Reserved. */
	uint64_t num_symb_pusch               : 4;  /**< Number of SC-FDMA symbols carrying PUSCH in a subframe.
                                                         * If CP_MODE = 0, must be 11 or 12.
                                                         * If CP_MODE = 1, must be 9 or 10. */
	uint64_t reserved_34_35               : 2;
	uint64_t cp_mode                      : 1;  /**< Either normal or extended cyclic prefix mode.
                                                         0 = Normal CP mode.
                                                         1 = Extended CP mode. */
	uint64_t bypass_scrambler             : 1;  /**< If set to 1, scrambling is bypassed. */
	uint64_t symb_byte_aligned            : 1;  /**< N/A */
	uint64_t scrambling_cinit             : 31; /**< Initial value of the second m-sequence of the scrambler. */
#else
	uint64_t scrambling_cinit             : 31;
	uint64_t symb_byte_aligned            : 1;
	uint64_t bypass_scrambler             : 1;
	uint64_t cp_mode                      : 1;
	uint64_t reserved_34_35               : 2;
	uint64_t num_symb_pusch               : 4;
	uint64_t ri_enc_cat                   : 3;
	uint64_t reserved_43_43               : 1;
	uint64_t ack_enc_cat                  : 3;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_rmap_jd0_cfg2_s           cnf75xx;
};
typedef union cvmx_rmap_jd0_cfg2 cvmx_rmap_jd0_cfg2_t;

/**
 * cvmx_rmap_jd0_cfg3
 *
 * This register stores the fourth job configuration word for slot 0.
 *
 */
union cvmx_rmap_jd0_cfg3 {
	uint64_t u64;
	struct cvmx_rmap_jd0_cfg3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t num_rb                       : 7;  /**< Total number of RBs in a subframe.  If [RES_ALLOC_TYPE] = 1, this must
                                                         equal the sum of [LENGTH_RB_SET0] and [LENGTH_RB_SET1]. */
	uint64_t num_scalar_qam               : 4;  /**< The number of bits the floating point modulation values are left shifted to produce
                                                         the fixed point complex values. Valid range is [1,14]. */
	uint64_t reserved_38_39               : 2;
	uint64_t num_bundled_i                : 2;  /**< Index to the scrambling sequence table for ACK bundling.
                                                         [NUM_BUNDLED_I] = (N_bundled - 1) mod 4. */
	uint64_t reserved_34_35               : 2;
	uint64_t ndft_indx                    : 6;  /**< Index for the NDFT as defined in the list of supported NDFTs. Valid
                                                         range is 0 to 34. */
	uint64_t reserved_25_27               : 3;
	uint64_t rs_bits_last                 : 5;  /**< This parameter controls the output scaling at the last stage of the DFT.
                                                         The DFT output is right shifted by [RS_BITS_LAST]. */
	uint64_t reserved_18_19               : 2;
	uint64_t res_alloc_type               : 1;  /**< The resource allocation type:
                                                         0 = Type 0.
                                                         1 = Type 1. */
	uint64_t freq_hop_type                : 1;  /**< Intra-subframe frequency hopping flag:
                                                         0 = No intra-subframe frequency hopping.
                                                         1 = Intra-subframe frequency hopping.
                                                         This value must be zero if [RES_ALLOC_TYPE] = 1. */
	uint64_t reserved_15_15               : 1;
	uint64_t length_rb_set0               : 7;  /**< Length in terms of contiguously allocated resource blocks for set 0.
                                                         Valid range is 1 to 108. */
	uint64_t reserved_7_7                 : 1;
	uint64_t length_rb_set1               : 7;  /**< Length in terms of contiguously allocated resource blocks for set 1.
                                                         Valid range is 1 to 108. The value is ignored if [RES_ALLOC_TYPE] = 0. */
#else
	uint64_t length_rb_set1               : 7;
	uint64_t reserved_7_7                 : 1;
	uint64_t length_rb_set0               : 7;
	uint64_t reserved_15_15               : 1;
	uint64_t freq_hop_type                : 1;
	uint64_t res_alloc_type               : 1;
	uint64_t reserved_18_19               : 2;
	uint64_t rs_bits_last                 : 5;
	uint64_t reserved_25_27               : 3;
	uint64_t ndft_indx                    : 6;
	uint64_t reserved_34_35               : 2;
	uint64_t num_bundled_i                : 2;
	uint64_t reserved_38_39               : 2;
	uint64_t num_scalar_qam               : 4;
	uint64_t num_rb                       : 7;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_rmap_jd0_cfg3_s           cnf75xx;
};
typedef union cvmx_rmap_jd0_cfg3 cvmx_rmap_jd0_cfg3_t;

/**
 * cvmx_rmap_jd1_cfg0
 *
 * This register stores the first job configuration word for slot 1.
 *
 */
union cvmx_rmap_jd1_cfg0 {
	uint64_t u64;
	struct cvmx_rmap_jd1_cfg0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t bypass_mod                   : 1;  /**< Modulator bypass is not supported. This field must be set to zero. */
	uint64_t bypass_dft                   : 1;  /**< DFT bypass.  If set to 1, DFT is bypassed */
	uint64_t reserved_59_59               : 1;
	uint64_t data_bit_order               : 1;  /**< Select bit order for data input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t data_byte_order              : 2;  /**< Select byte order for data input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_55_55               : 1;
	uint64_t cqi_bit_order                : 1;  /**< Select bit order for CQI input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t cqi_byte_order               : 2;  /**< Select byte order for CQI input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_51_51               : 1;
	uint64_t ri_bit_order                 : 1;  /**< Select bit order for RI input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t ri_byte_order                : 2;  /**< Select byte order for RI input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_47_47               : 1;
	uint64_t ack_bit_order                : 1;  /**< Select bit order for ACK input.
                                                         0 = MSB first within a byte.
                                                         1 = LSB first within a byte. */
	uint64_t ack_byte_order               : 2;  /**< Select byte order for ACK input.
                                                         0x0 = Little endian, byte 0 is bits 7..0.
                                                         0x1 = Byte swapping within 64 bit words from little endian.
                                                         0x2 = Byte swapping within 32 bit words from little endian.
                                                         0x3 = Reserved. */
	uint64_t reserved_42_43               : 2;
	uint64_t num_encoded_cqi_bits         : 18; /**< The number of encoded cqi bits.
                                                         When CQI is RM encoded, [NUM_ENCODED_CQI_BITS] must be 32. When
                                                         CQI uses convolutional coding, it may be either partially or fully
                                                         rate-matched, in which case the size should be equal to the
                                                         REENC_RM_OUT_SIZE from the VDEC job configuration that produced the
                                                         CQI bits. Valid range is 0 to 186624. */
	uint64_t reserved_22_23               : 2;
	uint64_t g_prime                      : 14; /**< Total number of coded symbols in the transport block.
                                                         The total number of coded bits is [G_PRIME]*[NUM_LAYERS]*[MOD_ORDER].
                                                         Valid range is 0 to 15552. */
	uint64_t reserved_7_7                 : 1;
	uint64_t mod_order                    : 3;  /**< Modulation order:
                                                         0x2 = QPSK.
                                                         0x4 = 16-QAM.
                                                         0x6 = 64-QAM. */
	uint64_t reserved_2_3                 : 2;
	uint64_t num_layers                   : 2;  /**< Number of layers for PUSCH channel. Must be either 0x1 or 0x2. */
#else
	uint64_t num_layers                   : 2;
	uint64_t reserved_2_3                 : 2;
	uint64_t mod_order                    : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t g_prime                      : 14;
	uint64_t reserved_22_23               : 2;
	uint64_t num_encoded_cqi_bits         : 18;
	uint64_t reserved_42_43               : 2;
	uint64_t ack_byte_order               : 2;
	uint64_t ack_bit_order                : 1;
	uint64_t reserved_47_47               : 1;
	uint64_t ri_byte_order                : 2;
	uint64_t ri_bit_order                 : 1;
	uint64_t reserved_51_51               : 1;
	uint64_t cqi_byte_order               : 2;
	uint64_t cqi_bit_order                : 1;
	uint64_t reserved_55_55               : 1;
	uint64_t data_byte_order              : 2;
	uint64_t data_bit_order               : 1;
	uint64_t reserved_59_59               : 1;
	uint64_t bypass_dft                   : 1;
	uint64_t bypass_mod                   : 1;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rmap_jd1_cfg0_s           cnf75xx;
};
typedef union cvmx_rmap_jd1_cfg0 cvmx_rmap_jd1_cfg0_t;

/**
 * cvmx_rmap_jd1_cfg1
 *
 * This register stores the second job configuration word for slot 1.
 *
 */
union cvmx_rmap_jd1_cfg1 {
	uint64_t u64;
	struct cvmx_rmap_jd1_cfg1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t num_cqi_re                   : 14; /**< Number of CQI RE per layer. Valid range is 0 to 15552. */
	uint64_t reserved_45_47               : 3;
	uint64_t num_ri_re                    : 13; /**< Number of RI RE per layer. Valid range is 0 to 5184. */
	uint64_t reserved_29_31               : 3;
	uint64_t num_ack_re                   : 13; /**< Number of ACK RE per layer. Valid range is 0 to 5184. */
	uint64_t reserved_11_15               : 5;
	uint64_t r_prime_mux                  : 11; /**< Size R'mux of the channel interleaver matrix. Max size is 1296. */
#else
	uint64_t r_prime_mux                  : 11;
	uint64_t reserved_11_15               : 5;
	uint64_t num_ack_re                   : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t num_ri_re                    : 13;
	uint64_t reserved_45_47               : 3;
	uint64_t num_cqi_re                   : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rmap_jd1_cfg1_s           cnf75xx;
};
typedef union cvmx_rmap_jd1_cfg1 cvmx_rmap_jd1_cfg1_t;

/**
 * cvmx_rmap_jd1_cfg2
 *
 * This register stores the third job configuration word for slot 1.
 *
 */
union cvmx_rmap_jd1_cfg2 {
	uint64_t u64;
	struct cvmx_rmap_jd1_cfg2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t ack_enc_cat                  : 3;  /**< ACK encoding category:
                                                         0x0 = No ACK, number of input bits = 0.
                                                         0x1 = Oack = 1, number of input bits = 1.
                                                         0x2 = Oack = 1 ACK bundling, number of input bits = 1.
                                                         0x3 = Oack = 2, number of input bits = 2.
                                                         0x4 = Oack = 2 ACK bundling, number of input bits = 2.
                                                         0x5 = 3 <= Oack <= 11, number of input bits = 32.
                                                         0x6 = 11 < Oack <= 20, number of input bits = 64.
                                                         0x7 = Reserved. */
	uint64_t reserved_43_43               : 1;
	uint64_t ri_enc_cat                   : 3;  /**< RI encoding category.
                                                         0x0 =  No RI, number of input bits = 0.
                                                         0x1 =  Ori = 1, number of input bits = 1.
                                                         0x2 =  Ori = 2, number of input bits = 2.
                                                         0x3 =  3 <= Ori <= 11, number of input bits = 32.
                                                         0x4 =  11 < Ori <= 20, number of input bits = 64.
                                                         0x5 - 0x7: Reserved. */
	uint64_t num_symb_pusch               : 4;  /**< Number of SC-FDMA symbols carrying PUSCH in a subframe.
                                                         * If CP_MODE = 0, must be 11 or 12.
                                                         * If CP_MODE = 1, must be 9 or 10. */
	uint64_t reserved_34_35               : 2;
	uint64_t cp_mode                      : 1;  /**< Either normal or extended cyclic prefix mode.
                                                         0 = Normal CP mode.
                                                         1 = Extended CP mode. */
	uint64_t bypass_scrambler             : 1;  /**< If set to 1, scrambling is bypassed. */
	uint64_t symb_byte_aligned            : 1;  /**< N/A */
	uint64_t scrambling_cinit             : 31; /**< Initial value of the second m-sequence of the scrambler. */
#else
	uint64_t scrambling_cinit             : 31;
	uint64_t symb_byte_aligned            : 1;
	uint64_t bypass_scrambler             : 1;
	uint64_t cp_mode                      : 1;
	uint64_t reserved_34_35               : 2;
	uint64_t num_symb_pusch               : 4;
	uint64_t ri_enc_cat                   : 3;
	uint64_t reserved_43_43               : 1;
	uint64_t ack_enc_cat                  : 3;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_rmap_jd1_cfg2_s           cnf75xx;
};
typedef union cvmx_rmap_jd1_cfg2 cvmx_rmap_jd1_cfg2_t;

/**
 * cvmx_rmap_jd1_cfg3
 *
 * This register stores the fourth job configuration word for slot 1.
 *
 */
union cvmx_rmap_jd1_cfg3 {
	uint64_t u64;
	struct cvmx_rmap_jd1_cfg3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_51_63               : 13;
	uint64_t num_rb                       : 7;  /**< Total number of RBs in a subframe.  If [RES_ALLOC_TYPE] = 1, this must
                                                         equal the sum of [LENGTH_RB_SET0] and [LENGTH_RB_SET1]. */
	uint64_t num_scalar_qam               : 4;  /**< Num_scalar_qam is to control the effective output bits of the modulator. [TBC] */
	uint64_t reserved_38_39               : 2;
	uint64_t num_bundled_i                : 2;  /**< Index to the scrambling sequence table for ACK bundling.
                                                         [NUM_BUNDLED_I] = (N_bundled - 1) mod 4. */
	uint64_t reserved_34_35               : 2;
	uint64_t ndft_indx                    : 6;  /**< Index for the NDFT as defined in the list of supported NDFTs. Valid
                                                         range is 0 to 34. */
	uint64_t reserved_25_27               : 3;
	uint64_t rs_bits_last                 : 5;  /**< This parameter controls the output scaling at the last stage of the DFT.
                                                         The DFT output is right shifted by [RS_BITS_LAST]. */
	uint64_t reserved_18_19               : 2;
	uint64_t res_alloc_type               : 1;  /**< The resource allocation type:
                                                         0 = Type 0.
                                                         1 = Type 1. */
	uint64_t freq_hop_type                : 1;  /**< Intra-subframe frequency hopping flag:
                                                         0 = No intra-subframe frequency hopping.
                                                         1 = Intra-subframe frequency hopping.
                                                         This value must be zero if [RES_ALLOC_TYPE] = 1. */
	uint64_t reserved_15_15               : 1;
	uint64_t length_rb_set0               : 7;  /**< Length in terms of contiguously allocated resource blocks for set 0.
                                                         Valid range is 1 to 108. */
	uint64_t reserved_7_7                 : 1;
	uint64_t length_rb_set1               : 7;  /**< Length in terms of contiguously allocated resource blocks for set 1.
                                                         Valid range is 1 to 108. The value is ignored if [RES_ALLOC_TYPE] = 0. */
#else
	uint64_t length_rb_set1               : 7;
	uint64_t reserved_7_7                 : 1;
	uint64_t length_rb_set0               : 7;
	uint64_t reserved_15_15               : 1;
	uint64_t freq_hop_type                : 1;
	uint64_t res_alloc_type               : 1;
	uint64_t reserved_18_19               : 2;
	uint64_t rs_bits_last                 : 5;
	uint64_t reserved_25_27               : 3;
	uint64_t ndft_indx                    : 6;
	uint64_t reserved_34_35               : 2;
	uint64_t num_bundled_i                : 2;
	uint64_t reserved_38_39               : 2;
	uint64_t num_scalar_qam               : 4;
	uint64_t num_rb                       : 7;
	uint64_t reserved_51_63               : 13;
#endif
	} s;
	struct cvmx_rmap_jd1_cfg3_s           cnf75xx;
};
typedef union cvmx_rmap_jd1_cfg3 cvmx_rmap_jd1_cfg3_t;

/**
 * cvmx_rmap_scratch
 */
union cvmx_rmap_scratch {
	uint64_t u64;
	struct cvmx_rmap_scratch_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Scratch data */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_rmap_scratch_s            cnf75xx;
};
typedef union cvmx_rmap_scratch cvmx_rmap_scratch_t;

/**
 * cvmx_rmap_status
 */
union cvmx_rmap_status {
	uint64_t u64;
	struct cvmx_rmap_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ready                        : 1;  /**< Indicates if the RMAP is ready to receive next job. '1' = Ready for next job, '0' = Busy. */
	uint64_t reserved_2_3                 : 2;
	uint64_t status1                      : 1;  /**< Indicates if the RMAP is busy processing a job with CONFIGURATION 1. '0' = Ready for new
                                                         job, '1' = Busy processing. */
	uint64_t status0                      : 1;  /**< Indicates if the RMAP is busy processing a job with CONFIGURATION 0. '0' = Ready for new
                                                         job, '1' = Busy processing. */
#else
	uint64_t status0                      : 1;
	uint64_t status1                      : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t ready                        : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_rmap_status_s             cnf75xx;
};
typedef union cvmx_rmap_status cvmx_rmap_status_t;

/**
 * cvmx_rmap_tc_config0
 *
 * TC1860 task config register 0
 *
 */
union cvmx_rmap_tc_config0 {
	uint64_t u64;
	struct cvmx_rmap_tc_config0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t g_prime                      : 14; /**< Parameter G_prime. */
	uint64_t reserved_7_7                 : 1;
	uint64_t qm                           : 3;  /**< Parameter Qm. */
	uint64_t reserved_2_3                 : 2;
	uint64_t nl                           : 2;  /**< Parameter NL. */
#else
	uint64_t nl                           : 2;
	uint64_t reserved_2_3                 : 2;
	uint64_t qm                           : 3;
	uint64_t reserved_7_7                 : 1;
	uint64_t g_prime                      : 14;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_rmap_tc_config0_s         cnf75xx;
};
typedef union cvmx_rmap_tc_config0 cvmx_rmap_tc_config0_t;

/**
 * cvmx_rmap_tc_config1
 *
 * TC1860 task config register 1
 *
 */
union cvmx_rmap_tc_config1 {
	uint64_t u64;
	struct cvmx_rmap_tc_config1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_62_63               : 2;
	uint64_t g_prime_cqi                  : 14; /**< Parameter g_prime_cqi. */
	uint64_t reserved_45_47               : 3;
	uint64_t g_prime_ri                   : 13; /**< Parameter g_prime_ri. */
	uint64_t reserved_29_31               : 3;
	uint64_t q_prime_ack                  : 13; /**< Parameter q_prime_ack. */
	uint64_t reserved_11_15               : 5;
	uint64_t r_prime_mux                  : 11; /**< Parameter r_prime_mux. */
#else
	uint64_t r_prime_mux                  : 11;
	uint64_t reserved_11_15               : 5;
	uint64_t q_prime_ack                  : 13;
	uint64_t reserved_29_31               : 3;
	uint64_t g_prime_ri                   : 13;
	uint64_t reserved_45_47               : 3;
	uint64_t g_prime_cqi                  : 14;
	uint64_t reserved_62_63               : 2;
#endif
	} s;
	struct cvmx_rmap_tc_config1_s         cnf75xx;
};
typedef union cvmx_rmap_tc_config1 cvmx_rmap_tc_config1_t;

/**
 * cvmx_rmap_tc_config2
 *
 * TC1860 task config register 2
 *
 */
union cvmx_rmap_tc_config2 {
	uint64_t u64;
	struct cvmx_rmap_tc_config2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_47_63               : 17;
	uint64_t ack_enc_cat                  : 3;  /**< Parameter ack_enc_cat. */
	uint64_t reserved_43_43               : 1;
	uint64_t ri_enc_cat                   : 3;  /**< Parameter ri_enc_cat. */
	uint64_t n_symb_pusch                 : 4;  /**< Parameter n_symb_pusch. */
	uint64_t reserved_34_35               : 2;
	uint64_t cp_mode                      : 1;  /**< Parameter cp_mode. */
	uint64_t bypass_scrambling            : 1;  /**< Parameter bypass_scrambling. */
	uint64_t symb_byte_aligned            : 1;  /**< Parameter symb_byte_aligned. */
	uint64_t scrambling_init              : 31; /**< Parameter scrambling_init. */
#else
	uint64_t scrambling_init              : 31;
	uint64_t symb_byte_aligned            : 1;
	uint64_t bypass_scrambling            : 1;
	uint64_t cp_mode                      : 1;
	uint64_t reserved_34_35               : 2;
	uint64_t n_symb_pusch                 : 4;
	uint64_t ri_enc_cat                   : 3;
	uint64_t reserved_43_43               : 1;
	uint64_t ack_enc_cat                  : 3;
	uint64_t reserved_47_63               : 17;
#endif
	} s;
	struct cvmx_rmap_tc_config2_s         cnf75xx;
};
typedef union cvmx_rmap_tc_config2 cvmx_rmap_tc_config2_t;

/**
 * cvmx_rmap_tc_config_err_flags
 */
union cvmx_rmap_tc_config_err_flags {
	uint64_t u64;
	struct cvmx_rmap_tc_config_err_flags_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_18_63               : 46;
	uint64_t error                        : 18; /**< When RMAP_TC_ERROR[CORE_ERR] is set, this field indicatest which
                                                         configuration parameter was set incorrectly. Each bit maps to a different error:
                                                         Bit 0 = NUM_RI_RE <= R_PRIME_MUX * 4.
                                                         Bit 1 = NUM_ACK_RE <= R_PRIME_MUX * 4.
                                                         Bit 2 = Invalid combination of N_SYMB_PUSCH and CP_MODE.
                                                         Bit 3 = R_PRIME_MUX * N_SYMB_PUSCH <= (G_PRIME + ri_prime) [TBC].
                                                         Bit 4 = Invalid range for G_PRIME.
                                                         Bit 5 = Invalid range for MOD_ORDER.
                                                         Bit 6 = Invalid range for NUM_LAYERS.
                                                         Bit 7 = Invalid range for NUM_CQI_RE.
                                                         Bit 8 = Invalid range for NUM_RI_RE.
                                                         Bit 9 = Invalid range for NUM_ACK_RE.
                                                         Bit 10 = Invalid range for R_PRIME_MUX.
                                                         Bit 11 = Invalid range for N_SYMB_PUSCH.
                                                         Bit 12 = Invalid range for ACK_ENC_CAT.
                                                         Bit 13 = Invalid range for RI_ENC_CAT.
                                                         Bit 14 = Invalid range for CP_MODE.
                                                         Bit 15 = Invalid range for BYPASS_SCRAMBLING.
                                                         Bit 16 = Reserved.
                                                         Bit 17 = Reserved. */
#else
	uint64_t error                        : 18;
	uint64_t reserved_18_63               : 46;
#endif
	} s;
	struct cvmx_rmap_tc_config_err_flags_s cnf75xx;
};
typedef union cvmx_rmap_tc_config_err_flags cvmx_rmap_tc_config_err_flags_t;

/**
 * cvmx_rmap_tc_error
 */
union cvmx_rmap_tc_error {
	uint64_t u64;
	struct cvmx_rmap_tc_error_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t core_err                     : 1;  /**< Invalid configuration, the block is aborted. See RMAP_TC_CONFIG_ERR_FLAGS. */
	uint64_t reserved_4_7                 : 4;
	uint64_t ign_rd_acc                   : 1;  /**< Ignored read access, already one read in progress. */
	uint64_t inv_rd_acc                   : 1;  /**< Invalid read access (out of range address). */
	uint64_t inv_wr_acc                   : 1;  /**< Invalid write access (out of range address). */
	uint64_t inv_start_task               : 1;  /**< Invalid start task (core is already busy processing a task). Error bits,  a bit of "1"
                                                         indicates an error, write "0" to clear. */
#else
	uint64_t inv_start_task               : 1;
	uint64_t inv_wr_acc                   : 1;
	uint64_t inv_rd_acc                   : 1;
	uint64_t ign_rd_acc                   : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t core_err                     : 1;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_rmap_tc_error_s           cnf75xx;
};
typedef union cvmx_rmap_tc_error cvmx_rmap_tc_error_t;

/**
 * cvmx_rmap_tc_error_mask
 */
union cvmx_rmap_tc_error_mask {
	uint64_t u64;
	struct cvmx_rmap_tc_error_mask_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t error_mask                   : 32; /**< The contents of RMAP_TC_ERROR are masked with this register. When an
                                                         error occurs, it is reported in RMAP_ERROR_SOURCE1[TC_ERROR] only when
                                                         the corresponding bit in ERROR_MASK is set to one. */
#else
	uint64_t error_mask                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_rmap_tc_error_mask_s      cnf75xx;
};
typedef union cvmx_rmap_tc_error_mask cvmx_rmap_tc_error_mask_t;

/**
 * cvmx_rmap_tc_main_control
 *
 * This register controls pipelining, error checks and clock
 * gating within the multiplexing, interleaving and scrambling sub-block.
 */
union cvmx_rmap_tc_main_control {
	uint64_t u64;
	struct cvmx_rmap_tc_main_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t chn_single_task_en           : 1;  /**< N/A */
	uint64_t phy_single_task_en           : 1;  /**< N/A */
	uint64_t single_task_en               : 1;  /**< Write 1 to disable all pipelining within the multiplexing,
                                                         interleaving and scrambling sub-block. Note that this degrades throughput. */
	uint64_t dis_conf_chk                 : 1;  /**< Write 1 to disable job configuration parameter validation. When
                                                         parameter validation is disabled, output is unpredictable for invalid
                                                         configurations. */
	uint64_t clk_en                       : 1;  /**< Disables internal automatic clock gating when set to 1. */
#else
	uint64_t clk_en                       : 1;
	uint64_t dis_conf_chk                 : 1;
	uint64_t single_task_en               : 1;
	uint64_t phy_single_task_en           : 1;
	uint64_t chn_single_task_en           : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_rmap_tc_main_control_s    cnf75xx;
};
typedef union cvmx_rmap_tc_main_control cvmx_rmap_tc_main_control_t;

/**
 * cvmx_rmap_tc_main_reset
 */
union cvmx_rmap_tc_main_reset {
	uint64_t u64;
	struct cvmx_rmap_tc_main_reset_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t soft_reset                   : 1;  /**< Any write to this bit (either 0 or 1) causes a soft reset of the
                                                         multiplexing, channel interleaving, and scrambling sub-block. */
#else
	uint64_t soft_reset                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rmap_tc_main_reset_s      cnf75xx;
};
typedef union cvmx_rmap_tc_main_reset cvmx_rmap_tc_main_reset_t;

/**
 * cvmx_rmap_tc_main_start
 *
 * TC1860 Main Start Register
 *
 */
union cvmx_rmap_tc_main_start {
	uint64_t u64;
	struct cvmx_rmap_tc_main_start_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t start                        : 1;  /**< N/A */
#else
	uint64_t start                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_rmap_tc_main_start_s      cnf75xx;
};
typedef union cvmx_rmap_tc_main_start cvmx_rmap_tc_main_start_t;

/**
 * cvmx_rmap_tc_status#
 *
 * TC1860 status register
 *
 */
union cvmx_rmap_tc_statusx {
	uint64_t u64;
	struct cvmx_rmap_tc_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t status                       : 64; /**< Status bits */
#else
	uint64_t status                       : 64;
#endif
	} s;
	struct cvmx_rmap_tc_statusx_s         cnf75xx;
};
typedef union cvmx_rmap_tc_statusx cvmx_rmap_tc_statusx_t;

#endif
