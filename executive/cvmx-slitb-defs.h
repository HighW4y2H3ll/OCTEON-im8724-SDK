/***********************license start***************
 * Copyright (c) 2003-2017  Cavium Inc. (support@cavium.com). All rights
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
 * cvmx-slitb-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon slitb.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_SLITB_DEFS_H__
#define __CVMX_SLITB_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_MSIXX_TABLE_ADDR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 127))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 127))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 127)))))
		cvmx_warn("CVMX_SLITB_MSIXX_TABLE_ADDR(%lu) is invalid on this chip\n", offset);
	return 0x0000000000004000ull + ((offset) & 127) * 16;
}
#else
#define CVMX_SLITB_MSIXX_TABLE_ADDR(offset) (0x0000000000004000ull + ((offset) & 127) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_MSIXX_TABLE_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 127))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 127))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 127)))))
		cvmx_warn("CVMX_SLITB_MSIXX_TABLE_DATA(%lu) is invalid on this chip\n", offset);
	return 0x0000000000004008ull + ((offset) & 127) * 16;
}
#else
#define CVMX_SLITB_MSIXX_TABLE_DATA(offset) (0x0000000000004008ull + ((offset) & 127) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_MSIX_MACX_PFX_TABLE_ADDR(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 1)) && ((block_id <= 3)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && (((offset == 0)) && ((block_id <= 3)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset == 0)) && ((block_id <= 3))))))
		cvmx_warn("CVMX_SLITB_MSIX_MACX_PFX_TABLE_ADDR(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000002000ull + ((offset) & 1) * 4096 + ((block_id) & 3) * 0x10ull;
}
#else
#define CVMX_SLITB_MSIX_MACX_PFX_TABLE_ADDR(offset, block_id) (0x0000000000002000ull + ((offset) & 1) * 4096 + ((block_id) & 3) * 0x10ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_MSIX_MACX_PFX_TABLE_DATA(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 1)) && ((block_id <= 3)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && (((offset == 0)) && ((block_id <= 3)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset == 0)) && ((block_id <= 3))))))
		cvmx_warn("CVMX_SLITB_MSIX_MACX_PFX_TABLE_DATA(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000002008ull + ((offset) & 1) * 4096 + ((block_id) & 3) * 0x10ull;
}
#else
#define CVMX_SLITB_MSIX_MACX_PFX_TABLE_DATA(offset, block_id) (0x0000000000002008ull + ((offset) & 1) * 4096 + ((block_id) & 3) * 0x10ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PFX_PKT_CNT_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 4)))))
		cvmx_warn("CVMX_SLITB_PFX_PKT_CNT_INT(%lu) is invalid on this chip\n", offset);
	return 0x0000000000008000ull + ((offset) & 7) * 16;
}
#else
#define CVMX_SLITB_PFX_PKT_CNT_INT(offset) (0x0000000000008000ull + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PFX_PKT_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 4)))))
		cvmx_warn("CVMX_SLITB_PFX_PKT_INT(%lu) is invalid on this chip\n", offset);
	return 0x0000000000008300ull + ((offset) & 7) * 16;
}
#else
#define CVMX_SLITB_PFX_PKT_INT(offset) (0x0000000000008300ull + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PFX_PKT_IN_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 4)))))
		cvmx_warn("CVMX_SLITB_PFX_PKT_IN_INT(%lu) is invalid on this chip\n", offset);
	return 0x0000000000008200ull + ((offset) & 7) * 16;
}
#else
#define CVMX_SLITB_PFX_PKT_IN_INT(offset) (0x0000000000008200ull + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PFX_PKT_RING_RST(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 4)))))
		cvmx_warn("CVMX_SLITB_PFX_PKT_RING_RST(%lu) is invalid on this chip\n", offset);
	return 0x0000000000008400ull + ((offset) & 7) * 16;
}
#else
#define CVMX_SLITB_PFX_PKT_RING_RST(offset) (0x0000000000008400ull + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PFX_PKT_TIME_INT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && ((offset <= 4))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 4)))))
		cvmx_warn("CVMX_SLITB_PFX_PKT_TIME_INT(%lu) is invalid on this chip\n", offset);
	return 0x0000000000008100ull + ((offset) & 7) * 16;
}
#else
#define CVMX_SLITB_PFX_PKT_TIME_INT(offset) (0x0000000000008100ull + ((offset) & 7) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SLITB_PKTX_PF_VF_MBOX_SIGX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CN73XX) && (((offset <= 1)) && ((block_id <= 127)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CN78XX) && (((offset <= 1)) && ((block_id <= 127)))) ||
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 127))))))
		cvmx_warn("CVMX_SLITB_PKTX_PF_VF_MBOX_SIGX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return 0x0000000000011000ull + (((offset) & 1) + ((block_id) & 127) * 0x4000ull) * 8;
}
#else
#define CVMX_SLITB_PKTX_PF_VF_MBOX_SIGX(offset, block_id) (0x0000000000011000ull + (((offset) & 1) + ((block_id) & 127) * 0x4000ull) * 8)
#endif

/**
 * cvmx_slitb_msix#_table_addr
 *
 * See SLI_MSIX()_TABLE_ADDR.
 * These registers shadow the 128 physical MSIX RING entries.
 * Each MAC accesses its RING entries using the PVF and a MSIX TABLE ADDR offset < 65.
 */
union cvmx_slitb_msixx_table_addr {
	uint64_t u64;
	struct cvmx_slitb_msixx_table_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t addr                         : 64; /**< Message address. */
#else
	uint64_t addr                         : 64;
#endif
	} s;
	struct cvmx_slitb_msixx_table_addr_s  cn73xx;
	struct cvmx_slitb_msixx_table_addr_s  cn78xx;
	struct cvmx_slitb_msixx_table_addr_s  cnf75xx;
};
typedef union cvmx_slitb_msixx_table_addr cvmx_slitb_msixx_table_addr_t;

/**
 * cvmx_slitb_msix#_table_data
 *
 * See SLI_MSIX()_TABLE_DATA.
 * These registers shadow the 128 physical MSIX RING entries.
 * Each MAC accesses its RING entries using the PVF and a MSIX TABLE DATA offset < 65.
 */
union cvmx_slitb_msixx_table_data {
	uint64_t u64;
	struct cvmx_slitb_msixx_table_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t vector_ctl                   : 1;  /**< Message mask. */
	uint64_t data                         : 32; /**< Message data. */
#else
	uint64_t data                         : 32;
	uint64_t vector_ctl                   : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_slitb_msixx_table_data_s  cn73xx;
	struct cvmx_slitb_msixx_table_data_s  cn78xx;
	struct cvmx_slitb_msixx_table_data_s  cnf75xx;
};
typedef union cvmx_slitb_msixx_table_data cvmx_slitb_msixx_table_data_t;

/**
 * cvmx_slitb_msix_mac#_pf#_table_addr
 *
 * These registers shadow the five physical MSIX PF ERR entries.
 * Each MAC sees its ERR entry at its own TRS offset for its PF number.
 */
union cvmx_slitb_msix_macx_pfx_table_addr {
	uint64_t u64;
	struct cvmx_slitb_msix_macx_pfx_table_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t addr                         : 64; /**< Message address. */
#else
	uint64_t addr                         : 64;
#endif
	} s;
	struct cvmx_slitb_msix_macx_pfx_table_addr_s cn73xx;
	struct cvmx_slitb_msix_macx_pfx_table_addr_s cn78xx;
	struct cvmx_slitb_msix_macx_pfx_table_addr_s cnf75xx;
};
typedef union cvmx_slitb_msix_macx_pfx_table_addr cvmx_slitb_msix_macx_pfx_table_addr_t;

/**
 * cvmx_slitb_msix_mac#_pf#_table_data
 *
 * These registers shadow the five physical MSIX PF ERR entries.
 * Each MAC sees its ERR entry at its own TRS offset for its PF number.
 */
union cvmx_slitb_msix_macx_pfx_table_data {
	uint64_t u64;
	struct cvmx_slitb_msix_macx_pfx_table_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_33_63               : 31;
	uint64_t vector_ctl                   : 1;  /**< Message mask. */
	uint64_t data                         : 32; /**< Message data. */
#else
	uint64_t data                         : 32;
	uint64_t vector_ctl                   : 1;
	uint64_t reserved_33_63               : 31;
#endif
	} s;
	struct cvmx_slitb_msix_macx_pfx_table_data_s cn73xx;
	struct cvmx_slitb_msix_macx_pfx_table_data_s cn78xx;
	struct cvmx_slitb_msix_macx_pfx_table_data_s cnf75xx;
};
typedef union cvmx_slitb_msix_macx_pfx_table_data cvmx_slitb_msix_macx_pfx_table_data_t;

/**
 * cvmx_slitb_pf#_pkt_cnt_int
 *
 * These registers shadow the five per-PF srn-based interrupt bits.
 * Each PF sees its own rings where ring[srn] is bit[0]. PF(4) means MAC0:PF1.
 */
union cvmx_slitb_pfx_pkt_cnt_int {
	uint64_t u64;
	struct cvmx_slitb_pfx_pkt_cnt_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ring                         : 64; /**< Multi-ring output ring packet counter interrupt bits. RING<i> is one
                                                         whenever SLI_PKT(i)_CNTS[CNT] > SLI_PKT(i)_INT_LEVELS[CNT].
                                                         RING<i> is the CNT component of SLI_PKT(i)_CNTS[PO_INT]
                                                         (and SLI_PKT_IN_DONE(i)_CNTS[PO_INT]), and one of the components
                                                         of SLI_PKT_INT[RING<i>]. Hardware may not update RING<i> when
                                                         software modifies SLI_PKT(i)_INT_LEVELS[CNT] - refer to the
                                                         description of SLI_PKT()_INT_LEVELS[CNT].
                                                         SLI_PKT(i)_OUTPUT_CONTROL[CENB] does not affect RING<i>. */
#else
	uint64_t ring                         : 64;
#endif
	} s;
	struct cvmx_slitb_pfx_pkt_cnt_int_s   cn73xx;
	struct cvmx_slitb_pfx_pkt_cnt_int_s   cn78xx;
	struct cvmx_slitb_pfx_pkt_cnt_int_s   cnf75xx;
};
typedef union cvmx_slitb_pfx_pkt_cnt_int cvmx_slitb_pfx_pkt_cnt_int_t;

/**
 * cvmx_slitb_pf#_pkt_in_int
 *
 * These registers shadow the five per-PF srn-based interrupt bits.
 * Each PF sees its own rings where ring[srn] is bit[0]
 */
union cvmx_slitb_pfx_pkt_in_int {
	uint64_t u64;
	struct cvmx_slitb_pfx_pkt_in_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ring                         : 64; /**< Multi-ring packet input interrupt register. Each RING<i> is a read-only copy of
                                                         SLI_PKT_IN_DONE(i)_CNTS[PI_INT]. */
#else
	uint64_t ring                         : 64;
#endif
	} s;
	struct cvmx_slitb_pfx_pkt_in_int_s    cn73xx;
	struct cvmx_slitb_pfx_pkt_in_int_s    cn78xx;
	struct cvmx_slitb_pfx_pkt_in_int_s    cnf75xx;
};
typedef union cvmx_slitb_pfx_pkt_in_int cvmx_slitb_pfx_pkt_in_int_t;

/**
 * cvmx_slitb_pf#_pkt_int
 *
 * These registers shadow the five per-PF srn-based interrupt bits.
 * Each PF sees its own rings where ring[srn] is bit[0]
 */
union cvmx_slitb_pfx_pkt_int {
	uint64_t u64;
	struct cvmx_slitb_pfx_pkt_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ring                         : 64; /**< Multi-ring packet interrupt register. Each RING<i> is set whenever any
                                                         of these three conditions are true:
                                                          * SLI_PKT(i)_CNTS[CNT] > SLI_PKT(i)_INT_LEVELS[CNT] (i.e. SLI_PKT_CNT_INT<i>
                                                            is set),
                                                          * Or, SLI_PKT(i)_CNTS[TIMER] > SLI_PKT(i)_INT_LEVELS[TIME] (i.e. SLI_PKT_TIME_INT<i>
                                                            is set).
                                                          * Or, SLI_PKT_IN_DONE(i)_CNTS[PI_INT] (i.e. SLI_PKT_IN_INT<i>) is set.
                                                         Any of these three conditions can cause an MSI-X interrupt, but only
                                                         the first two (i.e. SLI_PKT_CNT_INT and SLI_PKT_TIME_INT) can cause
                                                         INTA/B/C/D and MSI interrupts.
                                                         SLI_PKT(i)_OUTPUT_CONTROL[CENB,TENB] have no effect on RING<i>. */
#else
	uint64_t ring                         : 64;
#endif
	} s;
	struct cvmx_slitb_pfx_pkt_int_s       cn73xx;
	struct cvmx_slitb_pfx_pkt_int_s       cn78xx;
	struct cvmx_slitb_pfx_pkt_int_s       cnf75xx;
};
typedef union cvmx_slitb_pfx_pkt_int cvmx_slitb_pfx_pkt_int_t;

/**
 * cvmx_slitb_pf#_pkt_ring_rst
 *
 * These registers shadow the five per-PF srn-based ring rst bits.
 * Each PF sees its own rings where ring[srn] is bit[0]
 */
union cvmx_slitb_pfx_pkt_ring_rst {
	uint64_t u64;
	struct cvmx_slitb_pfx_pkt_ring_rst_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t rst                          : 64; /**< RST<i> is a read-only copy of SLI_PKT(i)_INPUT_CONTROL[RST]. */
#else
	uint64_t rst                          : 64;
#endif
	} s;
	struct cvmx_slitb_pfx_pkt_ring_rst_s  cn73xx;
	struct cvmx_slitb_pfx_pkt_ring_rst_s  cn78xx;
	struct cvmx_slitb_pfx_pkt_ring_rst_s  cnf75xx;
};
typedef union cvmx_slitb_pfx_pkt_ring_rst cvmx_slitb_pfx_pkt_ring_rst_t;

/**
 * cvmx_slitb_pf#_pkt_time_int
 *
 * These registers shadow the five per-PF srn-based interrupt bits.
 * Each PF sees its own rings where ring[srn] is bit[0]
 */
union cvmx_slitb_pfx_pkt_time_int {
	uint64_t u64;
	struct cvmx_slitb_pfx_pkt_time_int_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t ring                         : 64; /**< Multi-ring output ring packet time interrupt bits. RING<i> reads as one
                                                         whenever SLI_PKT(i)_CNTS[TIMER] > SLI_PKT(i)_INT_LEVELS[TIME].
                                                         RING<i> is the TIMER component of SLI_PKT(i)_CNTS[PO_INT]
                                                         (and SLI_PKT_IN_DONE(i)_CNTS[PO_INT]), and one of the components
                                                         of SLI_PKT_INT[RING<i>]. Hardware may not update RING<i> when
                                                         software modifies SLI_PKT(i)_INT_LEVELS[TIME] - refer to the
                                                         description of SLI_PKT()_INT_LEVELS[TIME].
                                                         SLI_PKT(i)_OUTPUT_CONTROL[TENB] does not affect RING<i>. */
#else
	uint64_t ring                         : 64;
#endif
	} s;
	struct cvmx_slitb_pfx_pkt_time_int_s  cn73xx;
	struct cvmx_slitb_pfx_pkt_time_int_s  cn78xx;
	struct cvmx_slitb_pfx_pkt_time_int_s  cnf75xx;
};
typedef union cvmx_slitb_pfx_pkt_time_int cvmx_slitb_pfx_pkt_time_int_t;

/**
 * cvmx_slitb_pkt#_pf_vf_mbox_sig#
 *
 * see SLI_PKT(0..63)_PF_VF_MBOX_SIG(0..1)
 * These registers index MBOX_SIG vector RTL registers by pf-pkt-ring and direction for
 * Verification.
 */
union cvmx_slitb_pktx_pf_vf_mbox_sigx {
	uint64_t u64;
	struct cvmx_slitb_pktx_pf_vf_mbox_sigx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Communication data from PF to VF. Writes to SLI_PKT()_PF_VF_MBOX_SIG(0)
                                                         in the corresponding VF. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_slitb_pktx_pf_vf_mbox_sigx_s cn73xx;
	struct cvmx_slitb_pktx_pf_vf_mbox_sigx_s cn78xx;
	struct cvmx_slitb_pktx_pf_vf_mbox_sigx_s cnf75xx;
};
typedef union cvmx_slitb_pktx_pf_vf_mbox_sigx cvmx_slitb_pktx_pf_vf_mbox_sigx_t;

#endif
