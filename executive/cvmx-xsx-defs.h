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
 * cvmx-xsx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon xsx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_XSX_DEFS_H__
#define __CVMX_XSX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_XSX_CONTROL CVMX_XSX_CONTROL_FUNC()
static inline uint64_t CVMX_XSX_CONTROL_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_XSX_CONTROL not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B5002000ull);
}
#else
#define CVMX_XSX_CONTROL (CVMX_ADD_IO_SEG(0x00011800B5002000ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_XSX_ECO CVMX_XSX_ECO_FUNC()
static inline uint64_t CVMX_XSX_ECO_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_XSX_ECO not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B5002010ull);
}
#else
#define CVMX_XSX_ECO (CVMX_ADD_IO_SEG(0x00011800B5002010ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_XSX_ERR_BIST_STATUS CVMX_XSX_ERR_BIST_STATUS_FUNC()
static inline uint64_t CVMX_XSX_ERR_BIST_STATUS_FUNC(void)
{
	if (!(OCTEON_IS_MODEL(OCTEON_CNF75XX)))
		cvmx_warn("CVMX_XSX_ERR_BIST_STATUS not supported on this chip\n");
	return CVMX_ADD_IO_SEG(0x00011800B5002008ull);
}
#else
#define CVMX_XSX_ERR_BIST_STATUS (CVMX_ADD_IO_SEG(0x00011800B5002008ull))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_ARBPRI(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_XSX_SMTX_ARBPRI(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B5000200ull) + ((offset) & 7) * 1024;
}
#else
#define CVMX_XSX_SMTX_ARBPRI(offset) (CVMX_ADD_IO_SEG(0x00011800B5000200ull) + ((offset) & 7) * 1024)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_ERR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_XSX_SMTX_ERR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B5000100ull) + ((offset) & 7) * 1024;
}
#else
#define CVMX_XSX_SMTX_ERR(offset) (CVMX_ADD_IO_SEG(0x00011800B5000100ull) + ((offset) & 7) * 1024)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_ERR_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_XSX_SMTX_ERR_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B5000130ull) + ((offset) & 7) * 1024;
}
#else
#define CVMX_XSX_SMTX_ERR_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B5000130ull) + ((offset) & 7) * 1024)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_ERR_ECC_DISABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_XSX_SMTX_ERR_ECC_DISABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B5000120ull) + ((offset) & 7) * 1024;
}
#else
#define CVMX_XSX_SMTX_ERR_ECC_DISABLE(offset) (CVMX_ADD_IO_SEG(0x00011800B5000120ull) + ((offset) & 7) * 1024)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_ERR_ECC_FLIP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 7)))))
		cvmx_warn("CVMX_XSX_SMTX_ERR_ECC_FLIP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B5000128ull) + ((offset) & 7) * 1024;
}
#else
#define CVMX_XSX_SMTX_ERR_ECC_FLIP(offset) (CVMX_ADD_IO_SEG(0x00011800B5000128ull) + ((offset) & 7) * 1024)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_PX_SRCX_RDWT(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 7)) && ((b <= 1)) && ((c <= 6))))))
		cvmx_warn("CVMX_XSX_SMTX_PX_SRCX_RDWT(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B5000000ull) + ((a) << 10) + ((b) << 6) + ((c) << 3);
}
#else
#define CVMX_XSX_SMTX_PX_SRCX_RDWT(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B5000000ull) + ((a) << 10) + ((b) << 6) + ((c) << 3))
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_XSX_SMTX_PX_SRCX_WRWT(unsigned long a, unsigned long b, unsigned long c)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((a <= 7)) && ((b <= 1)) && ((c <= 6))))))
		cvmx_warn("CVMX_XSX_SMTX_PX_SRCX_WRWT(%lu,%lu,%lu) is invalid on this chip\n", a, b, c);
	return CVMX_ADD_IO_SEG(0x00011800B5000080ull) + ((a) << 10) + ((b) << 6) + ((c) << 3);
}
#else
#define CVMX_XSX_SMTX_PX_SRCX_WRWT(a, b, c) (CVMX_ADD_IO_SEG(0x00011800B5000080ull) + ((a) << 10) + ((b) << 6) + ((c) << 3))
#endif

/**
 * cvmx_xsx_control
 */
union cvmx_xsx_control {
	uint64_t u64;
	struct cvmx_xsx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t smt_clk_disable              : 8;  /**< Disable the main SMT clock. Each bit controls the clock for one SMT. */
	uint64_t reserved_1_7                 : 7;
	uint64_t xor_disable                  : 1;  /**< XOR addressing disable. This bit should not be changed when there are
                                                         any SMEM accesses outstanding. Changing this bit while any request is
                                                         active can cause undefined behavior. */
#else
	uint64_t xor_disable                  : 1;
	uint64_t reserved_1_7                 : 7;
	uint64_t smt_clk_disable              : 8;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_xsx_control_s             cnf75xx;
};
typedef union cvmx_xsx_control cvmx_xsx_control_t;

/**
 * cvmx_xsx_eco
 */
union cvmx_xsx_eco {
	uint64_t u64;
	struct cvmx_xsx_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_xsx_eco_s                 cnf75xx;
};
typedef union cvmx_xsx_eco cvmx_xsx_eco_t;

/**
 * cvmx_xsx_err_bist_status
 *
 * This register provides access to the BIST results.
 *
 */
union cvmx_xsx_err_bist_status {
	uint64_t u64;
	struct cvmx_xsx_err_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t smem_bstat                   : 1;  /**< SMEM bist status (0 = pass and 1 = fail). */
#else
	uint64_t smem_bstat                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_xsx_err_bist_status_s     cnf75xx;
};
typedef union cvmx_xsx_err_bist_status cvmx_xsx_err_bist_status_t;

/**
 * cvmx_xsx_smt#_arbpri
 */
union cvmx_xsx_smtx_arbpri {
	uint64_t u64;
	struct cvmx_xsx_smtx_arbpri_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t hogen                        : 1;  /**< Bus-hog limit enable. When set to 1, the RDPRI bit setting is inverted
                                                         every eighth clock cycle to prevent starvation in the presence of
                                                         persistent conflicts. */
	uint64_t rdpri                        : 1;  /**< When set to 1, reads have priority over writes in bank conflicts. When
                                                         set to 0, writes have priority when resolving bank conflicts. */
	uint64_t p0pri                        : 1;  /**< When set to 1, port 0 (MHAB) requests have priority over port 1 (MDAB)
                                                         requests when resolving bank conflicts. */
#else
	uint64_t p0pri                        : 1;
	uint64_t rdpri                        : 1;
	uint64_t hogen                        : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_xsx_smtx_arbpri_s         cnf75xx;
};
typedef union cvmx_xsx_smtx_arbpri cvmx_xsx_smtx_arbpri_t;

/**
 * cvmx_xsx_smt#_err
 *
 * These registers indicate when an SMT has experienced an ECC error. Writing
 * one to a bit will clear that error.
 */
union cvmx_xsx_smtx_err {
	uint64_t u64;
	struct cvmx_xsx_smtx_err_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t dbe                          : 1;  /**< Uncorrectable double bit errors. */
	uint64_t sbe                          : 1;  /**< Correctable single bit errors. */
#else
	uint64_t sbe                          : 1;
	uint64_t dbe                          : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_xsx_smtx_err_s            cnf75xx;
};
typedef union cvmx_xsx_smtx_err cvmx_xsx_smtx_err_t;

/**
 * cvmx_xsx_smt#_err_bist_status
 *
 * This register provides access to the SMT BIST results.
 *
 */
union cvmx_xsx_smtx_err_bist_status {
	uint64_t u64;
	struct cvmx_xsx_smtx_err_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ram                          : 1;  /**< BIST status (0 = pass and 1 = fail). */
#else
	uint64_t ram                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_xsx_smtx_err_bist_status_s cnf75xx;
};
typedef union cvmx_xsx_smtx_err_bist_status cvmx_xsx_smtx_err_bist_status_t;

/**
 * cvmx_xsx_smt#_err_ecc_disable
 */
union cvmx_xsx_smtx_err_ecc_disable {
	uint64_t u64;
	struct cvmx_xsx_smtx_err_ecc_disable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t ram                          : 1;  /**< When set, this bit disables ECC correction and checking of the SMT RAM. */
#else
	uint64_t ram                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_xsx_smtx_err_ecc_disable_s cnf75xx;
};
typedef union cvmx_xsx_smtx_err_ecc_disable cvmx_xsx_smtx_err_ecc_disable_t;

/**
 * cvmx_xsx_smt#_err_ecc_flip
 */
union cvmx_xsx_smtx_err_ecc_flip {
	uint64_t u64;
	struct cvmx_xsx_smtx_err_ecc_flip_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_34_63               : 30;
	uint64_t ramx                         : 2;  /**< This bit flips the second bit of syndrome of the write port paths to the
                                                         respective SMT. Bit 1 controls write port 1, and bit 0 controls write
                                                         port 0. */
	uint64_t reserved_2_31                : 30;
	uint64_t ram                          : 2;  /**< This bit flips the first bit of syndrome of the write port paths to
                                                         the respective SMT. Bit 1 controls write port 1, and bit 0 controls
                                                         write port 0. */
#else
	uint64_t ram                          : 2;
	uint64_t reserved_2_31                : 30;
	uint64_t ramx                         : 2;
	uint64_t reserved_34_63               : 30;
#endif
	} s;
	struct cvmx_xsx_smtx_err_ecc_flip_s   cnf75xx;
};
typedef union cvmx_xsx_smtx_err_ecc_flip cvmx_xsx_smtx_err_ecc_flip_t;

/**
 * cvmx_xsx_smt#_p#_src#_rdwt
 *
 * These registers specify the arbitration weights used when accepting read
 * requests. Each input source is assigned an arbitration weight, and
 * weights are programmed separately for each of the eight memory tiles
 * (SMT(0..7)).
 *
 * The MHABs are connected via port 0 (P(0)). Sources 0-5 represent the 6
 * GHABs, and source 6 is PNB0.
 *
 * The MDABs are connected via port 1 (P(1)). Sources 0-5 are the 6 GDABs,
 * and source 6 is PNB1.
 */
union cvmx_xsx_smtx_px_srcx_rdwt {
	uint64_t u64;
	struct cvmx_xsx_smtx_px_srcx_rdwt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t wgt                          : 6;  /**< Weighted round-robin arbitration weight. */
#else
	uint64_t wgt                          : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_xsx_smtx_px_srcx_rdwt_s   cnf75xx;
};
typedef union cvmx_xsx_smtx_px_srcx_rdwt cvmx_xsx_smtx_px_srcx_rdwt_t;

/**
 * cvmx_xsx_smt#_p#_src#_wrwt
 *
 * These registers specify the arbitration weights used when accepting write
 * requests. Each input source is assigned an arbitration weight, and
 * weights are programmed separately for each of the eight memory tiles
 * (SMT(0..7)).
 *
 * The MHABs are connected via port 0 (P(0)). Sources 0-5 represent the 6
 * GHABs, and source 6 is PNB0.
 *
 * The MDABs are connected via port 1 (P(1)). Sources 0-5 are the 6 GDABs,
 * and source 6 is PNB1.
 */
union cvmx_xsx_smtx_px_srcx_wrwt {
	uint64_t u64;
	struct cvmx_xsx_smtx_px_srcx_wrwt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t wgt                          : 6;  /**< Weighted round-robin arbitration weight. */
#else
	uint64_t wgt                          : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_xsx_smtx_px_srcx_wrwt_s   cnf75xx;
};
typedef union cvmx_xsx_smtx_px_srcx_wrwt cvmx_xsx_smtx_px_srcx_wrwt_t;

#endif
