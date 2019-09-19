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
 * cvmx-rfifx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon rfifx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_RFIFX_DEFS_H__
#define __CVMX_RFIFX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIFX_ALIGN_BF(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIFX_ALIGN_BF(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5000058ull) + ((offset) & 7) * 0x10000ull;
}
#else
#define CVMX_RFIFX_ALIGN_BF(offset) (CVMX_ADD_IO_SEG(0x00011800D5000058ull) + ((offset) & 7) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIFX_DL_BUF_UFL_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIFX_DL_BUF_UFL_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5002AC0ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIFX_DL_BUF_UFL_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5002AC0ull) + ((offset) & 7) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_RFIFX_UL_BUF_OFL_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_RFIFX_UL_BUF_OFL_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5002A80ull) + ((offset) & 7) * 8;
}
#else
#define CVMX_RFIFX_UL_BUF_OFL_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5002A80ull) + ((offset) & 7) * 8)
#endif

/**
 * cvmx_rfif#_align_bf
 *
 * This register reports the first CPRI basic frame number in each radio
 * frame (i.e., the basic frame number at RFP_INT+RFIF_RETARD).
 *
 * Note that this register only reports basic frame numbers when the
 * corresponding RMAC is in CPRI mode.
 */
union cvmx_rfifx_align_bf {
	uint64_t u64;
	struct cvmx_rfifx_align_bf_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t bf_num                       : 16; /**< Designates the first CPRI basic frame number of each radio frame,
                                                         which corresponds to the first basic frame after the RFP_UL pulse.
                                                         This value is updated every radio frame. */
#else
	uint64_t bf_num                       : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_rfifx_align_bf_s          cnf75xx;
};
typedef union cvmx_rfifx_align_bf cvmx_rfifx_align_bf_t;

/**
 * cvmx_rfif#_dl_buf_ufl_cnt
 */
union cvmx_rfifx_dl_buf_ufl_cnt {
	uint64_t u64;
	struct cvmx_rfifx_dl_buf_ufl_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ufl_cnt                      : 8;  /**< Count of the number times a DL buffer overflow condition was detected since the
                                                         last time the register was read. Note that this register clears on
                                                         a read. */
#else
	uint64_t ufl_cnt                      : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_rfifx_dl_buf_ufl_cnt_s    cnf75xx;
};
typedef union cvmx_rfifx_dl_buf_ufl_cnt cvmx_rfifx_dl_buf_ufl_cnt_t;

/**
 * cvmx_rfif#_ul_buf_ofl_cnt
 */
union cvmx_rfifx_ul_buf_ofl_cnt {
	uint64_t u64;
	struct cvmx_rfifx_ul_buf_ofl_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ofl_cnt                      : 8;  /**< Count of the number times a UL buffer overflow condition was detected since the
                                                         last time the register was read. Note that this register clears on
                                                         a read. */
#else
	uint64_t ofl_cnt                      : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_rfifx_ul_buf_ofl_cnt_s    cnf75xx;
};
typedef union cvmx_rfifx_ul_buf_ofl_cnt cvmx_rfifx_ul_buf_ofl_cnt_t;

#endif
