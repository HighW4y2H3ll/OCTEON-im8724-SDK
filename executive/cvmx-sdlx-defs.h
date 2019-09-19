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
 * cvmx-sdlx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon sdlx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_SDLX_DEFS_H__
#define __CVMX_SDLX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_CPUIF_CDC_FIFO_ERR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_CPUIF_CDC_FIFO_ERR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380060ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_CPUIF_CDC_FIFO_ERR(offset) (CVMX_ADD_IO_SEG(0x00011800D5380060ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_AXC_BW_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_DL_AXC_BW_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380208ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_DL_AXC_BW_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380208ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_AXC_MAP_LUTX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 15)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_SDLX_DL_AXC_MAP_LUTX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5380188ull) + (((offset) & 15) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_SDLX_DL_AXC_MAP_LUTX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5380188ull) + (((offset) & 15) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_AXC_MAP_LUT_LOCK(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_DL_AXC_MAP_LUT_LOCK(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380180ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_DL_AXC_MAP_LUT_LOCK(offset) (CVMX_ADD_IO_SEG(0x00011800D5380180ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_BIT_CTRL_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_DL_BIT_CTRL_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380210ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_DL_BIT_CTRL_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380210ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_SYNC_ERR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_DL_SYNC_ERR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380218ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_DL_SYNC_ERR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380218ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_DL_SYNC_GD_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_DL_SYNC_GD_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380220ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_DL_SYNC_GD_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380220ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_LIGHT_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_LIGHT_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380008ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_LIGHT_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380008ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_LN_CHIP_ERR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_LN_CHIP_ERR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380028ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_LN_CHIP_ERR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380028ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_LN_DAT_ERR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_LN_DAT_ERR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380030ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_LN_DAT_ERR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380030ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_LN_FRM_ERR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_LN_FRM_ERR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380020ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_LN_FRM_ERR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380020ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_MAPPER_LOOPBACK(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_MAPPER_LOOPBACK(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380058ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_MAPPER_LOOPBACK(offset) (CVMX_ADD_IO_SEG(0x00011800D5380058ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_MAPPER_VER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_MAPPER_VER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380050ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_MAPPER_VER(offset) (CVMX_ADD_IO_SEG(0x00011800D5380050ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_RX_PAT_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_RX_PAT_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380010ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_RX_PAT_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380010ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_TX_PAT_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_TX_PAT_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380018ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_TX_PAT_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380018ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_AXC_BW_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_AXC_BW_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380108ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_AXC_BW_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380108ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_AXC_MAP_LUTX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 15)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_SDLX_UL_AXC_MAP_LUTX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5380088ull) + (((offset) & 15) + ((block_id) & 3) * 0x2000ull) * 8;
}
#else
#define CVMX_SDLX_UL_AXC_MAP_LUTX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5380088ull) + (((offset) & 15) + ((block_id) & 3) * 0x2000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_AXC_MAP_LUT_LOCK(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_AXC_MAP_LUT_LOCK(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380080ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_AXC_MAP_LUT_LOCK(offset) (CVMX_ADD_IO_SEG(0x00011800D5380080ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_BIT_CTRL_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_BIT_CTRL_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380110ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_BIT_CTRL_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380110ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_LOF_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_LOF_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380128ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_LOF_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5380128ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_SYNC_ERR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_SYNC_ERR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380118ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_SYNC_ERR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380118ull) + ((offset) & 3) * 0x10000ull)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_SDLX_UL_SYNC_GD_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_SDLX_UL_SYNC_GD_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5380120ull) + ((offset) & 3) * 0x10000ull;
}
#else
#define CVMX_SDLX_UL_SYNC_GD_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5380120ull) + ((offset) & 3) * 0x10000ull)
#endif

/**
 * cvmx_sdl#_cpuif_cdc_fifo_err
 *
 * This register reports clock-domain crossing errors for the register
 * interface.
 */
union cvmx_sdlx_cpuif_cdc_fifo_err {
	uint64_t u64;
	struct cvmx_sdlx_cpuif_cdc_fifo_err_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t req_cdc_push_error           : 1;  /**< req_cdc_push_error. */
	uint64_t req_cdc_pop_error            : 1;  /**< req_cdc_pop_error. */
	uint64_t rsp_cdc_push_error           : 1;  /**< rsp_cdc_push_error. */
	uint64_t rsp_cdc_pop_error            : 1;  /**< rsp_cdc_pop_error. */
#else
	uint64_t rsp_cdc_pop_error            : 1;
	uint64_t rsp_cdc_push_error           : 1;
	uint64_t req_cdc_pop_error            : 1;
	uint64_t req_cdc_push_error           : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_sdlx_cpuif_cdc_fifo_err_s cnf75xx;
};
typedef union cvmx_sdlx_cpuif_cdc_fifo_err cvmx_sdlx_cpuif_cdc_fifo_err_t;

/**
 * cvmx_sdl#_dl_axc_bw_sel
 *
 * Select the bandwidth of each downlink antenna-carrier (AxC).
 *
 */
union cvmx_sdlx_dl_axc_bw_sel {
	uint64_t u64;
	struct cvmx_sdlx_dl_axc_bw_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t bw_sel                       : 32; /**< 2-bit selects for each DL AxC. Bits[1:0] control AxC 0, bits[3:2]
                                                         control AxC 1, etc.
                                                         0x0 = AxC disabled.
                                                         0x1 = 5 MHz (i.e., 7.68 Msps).
                                                         0x2 = 10 MHz (i.e., 15.36 Msps).
                                                         0x3 = 20 MHz (i.e., 30.72 Msps). */
#else
	uint64_t bw_sel                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_dl_axc_bw_sel_s      cnf75xx;
};
typedef union cvmx_sdlx_dl_axc_bw_sel cvmx_sdlx_dl_axc_bw_sel_t;

/**
 * cvmx_sdl#_dl_axc_map_lut#
 *
 * These registers program the AxC mapping for the download channels of SDL.
 *
 * Each SDL basic frame (or chip) contains 64 time slots, and each of these
 * 16 regsiters controls which AxC is mapped to each time slot.
 *
 * _ SDL(0..2)_DL_AXC_LUT(0) controls time slots [TS3, TS2, TS1, TS0] with fields 3-0
 * respectively.
 *
 * _ SDL(0..2)_DL_AXC_LUT(1) controls time slots [TS7, TS6, TS5, TS4] with fields 3-0
 * respectively,
 *
 * _ etc.
 *
 * Note that each AxC must be mapped to either 2, 4, or 8 time slots,
 * depending on whether the bandwidth is 5 MHz, 10 MHz, or 20 MHz,
 * respectively.
 *
 * Consecutive time samples for a given antenna should be assigned to strided time
 * slots as follows:
 * 5 MHz: TS x and TS x+32.
 * 10 MHz: TS x, TS x+16, TS x+32, and TS x+48.
 * 20 MHz: TS x, TS x+8, TS x+16, TS x+24, TS x+32, TS x+40, TS x+48, and TS x+56.
 */
union cvmx_sdlx_dl_axc_map_lutx {
	uint64_t u64;
	struct cvmx_sdlx_dl_axc_map_lutx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t enable3                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc3                         : 4;  /**< Chooses which AxC is mapped to this time slot. */
	uint64_t reserved_21_23               : 3;
	uint64_t enable2                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc2                         : 4;  /**< Chooses which AxC is mapped to this time slot. */
	uint64_t reserved_13_15               : 3;
	uint64_t enable1                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc1                         : 4;  /**< Chooses which AxC is mapped to this time slot. */
	uint64_t reserved_5_7                 : 3;
	uint64_t enable0                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc0                         : 4;  /**< Chooses which AxC is mapped to this time slot. */
#else
	uint64_t axc0                         : 4;
	uint64_t enable0                      : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t axc1                         : 4;
	uint64_t enable1                      : 1;
	uint64_t reserved_13_15               : 3;
	uint64_t axc2                         : 4;
	uint64_t enable2                      : 1;
	uint64_t reserved_21_23               : 3;
	uint64_t axc3                         : 4;
	uint64_t enable3                      : 1;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_sdlx_dl_axc_map_lutx_s    cnf75xx;
};
typedef union cvmx_sdlx_dl_axc_map_lutx cvmx_sdlx_dl_axc_map_lutx_t;

/**
 * cvmx_sdl#_dl_axc_map_lut_lock
 *
 * This register needs to be set before updating the SDL DL AxC mapping lookup table or
 * carrier bandwidth select (DL_AXC_BW_SEL).  It then needs to be cleared when update is
 * complete.
 */
union cvmx_sdlx_dl_axc_map_lut_lock {
	uint64_t u64;
	struct cvmx_sdlx_dl_axc_map_lut_lock_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t lock                         : 1;  /**< Active high lock bit. */
#else
	uint64_t lock                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_sdlx_dl_axc_map_lut_lock_s cnf75xx;
};
typedef union cvmx_sdlx_dl_axc_map_lut_lock cvmx_sdlx_dl_axc_map_lut_lock_t;

/**
 * cvmx_sdl#_dl_bit_ctrl_sel
 *
 * This register determines how/if the truncation from 16 to 15 bits is done.
 *
 * It is recommended that the 16-15 bit conversion is done by RFIF and that
 * this register is left at its default value.
 */
union cvmx_sdlx_dl_bit_ctrl_sel {
	uint64_t u64;
	struct cvmx_sdlx_dl_bit_ctrl_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t sel                          : 2;  /**< 0x0 = No truncation (recommended).
                                                         0x1 = sat.
                                                         0x2 = round trunc sat.
                                                         0x3 = reserved. */
#else
	uint64_t sel                          : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_sdlx_dl_bit_ctrl_sel_s    cnf75xx;
};
typedef union cvmx_sdlx_dl_bit_ctrl_sel cvmx_sdlx_dl_bit_ctrl_sel_t;

/**
 * cvmx_sdl#_dl_sync_err_cnt
 *
 * This register counts the DL frame synchronization errors.
 *
 */
union cvmx_sdlx_dl_sync_err_cnt {
	uint64_t u64;
	struct cvmx_sdlx_dl_sync_err_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frame_sync_err_cnt           : 16; /**< Frame synchronization error count. */
	uint64_t reserved_0_15                : 16;
#else
	uint64_t reserved_0_15                : 16;
	uint64_t frame_sync_err_cnt           : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_dl_sync_err_cnt_s    cnf75xx;
};
typedef union cvmx_sdlx_dl_sync_err_cnt cvmx_sdlx_dl_sync_err_cnt_t;

/**
 * cvmx_sdl#_dl_sync_gd_cnt
 *
 * This register counts the number of DL good frame synchronizations.
 *
 */
union cvmx_sdlx_dl_sync_gd_cnt {
	uint64_t u64;
	struct cvmx_sdlx_dl_sync_gd_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frame_sync_gd_cnt            : 16; /**< Good frame synchronization count. */
	uint64_t reserved_0_15                : 16;
#else
	uint64_t reserved_0_15                : 16;
	uint64_t frame_sync_gd_cnt            : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_dl_sync_gd_cnt_s     cnf75xx;
};
typedef union cvmx_sdlx_dl_sync_gd_cnt cvmx_sdlx_dl_sync_gd_cnt_t;

/**
 * cvmx_sdl#_light_ctrl
 *
 * This register controls signals on the SerDes-Lite interfaces.
 *
 */
union cvmx_sdlx_light_ctrl {
	uint64_t u64;
	struct cvmx_sdlx_light_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t rx_start_en                  : 1;  /**< Enable SDL RX. */
	uint64_t tx_start_en                  : 1;  /**< Enalbe SDL TX. */
#else
	uint64_t tx_start_en                  : 1;
	uint64_t rx_start_en                  : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_sdlx_light_ctrl_s         cnf75xx;
};
typedef union cvmx_sdlx_light_ctrl cvmx_sdlx_light_ctrl_t;

/**
 * cvmx_sdl#_ln_chip_err_cnt
 *
 * This register counts the number chip sync errors detected by the pattern
 * checker during self-test.
 */
union cvmx_sdlx_ln_chip_err_cnt {
	uint64_t u64;
	struct cvmx_sdlx_ln_chip_err_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t lane1                        : 16; /**< Total number of chip sync errors on lane 1. */
	uint64_t lane0                        : 16; /**< Total number of chip sync errors on lane 0. */
#else
	uint64_t lane0                        : 16;
	uint64_t lane1                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ln_chip_err_cnt_s    cnf75xx;
};
typedef union cvmx_sdlx_ln_chip_err_cnt cvmx_sdlx_ln_chip_err_cnt_t;

/**
 * cvmx_sdl#_ln_dat_err_cnt
 *
 * This register counts the number of data errors detected by the pattern
 * checker during self-test.
 */
union cvmx_sdlx_ln_dat_err_cnt {
	uint64_t u64;
	struct cvmx_sdlx_ln_dat_err_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t lane1                        : 16; /**< Total number of data errors on lane 1. */
	uint64_t lane0                        : 16; /**< Total number of data errors on lane 0. */
#else
	uint64_t lane0                        : 16;
	uint64_t lane1                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ln_dat_err_cnt_s     cnf75xx;
};
typedef union cvmx_sdlx_ln_dat_err_cnt cvmx_sdlx_ln_dat_err_cnt_t;

/**
 * cvmx_sdl#_ln_frm_err_cnt
 *
 * This register counts the number frame sync errors detected by the pattern
 * checker during self-test.
 */
union cvmx_sdlx_ln_frm_err_cnt {
	uint64_t u64;
	struct cvmx_sdlx_ln_frm_err_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t lane1                        : 16; /**< Total number of frame sync errors on lane 1. */
	uint64_t lane0                        : 16; /**< Total number of frame sync errors on lane 0. */
#else
	uint64_t lane0                        : 16;
	uint64_t lane1                        : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ln_frm_err_cnt_s     cnf75xx;
};
typedef union cvmx_sdlx_ln_frm_err_cnt cvmx_sdlx_ln_frm_err_cnt_t;

/**
 * cvmx_sdl#_mapper_loopback
 *
 * This register controls the loopback mode at the RFIF-SDL interface boundary.
 *
 */
union cvmx_sdlx_mapper_loopback {
	uint64_t u64;
	struct cvmx_sdlx_mapper_loopback_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t sel                          : 2;  /**< 0x0 = Normal operation (no loopback).
                                                         0x1 = Loopback TX(DL) output to RX(UL) input (DL2UL internal loopback).
                                                         0x2 = Loopback RX(UL) input to TX(DL) output (UL2DL external loopback).
                                                         0x3 = Normal operation (no loopback). */
#else
	uint64_t sel                          : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_sdlx_mapper_loopback_s    cnf75xx;
};
typedef union cvmx_sdlx_mapper_loopback cvmx_sdlx_mapper_loopback_t;

/**
 * cvmx_sdl#_mapper_ver
 *
 * This register contains the SDL version number.
 *
 */
union cvmx_sdlx_mapper_ver {
	uint64_t u64;
	struct cvmx_sdlx_mapper_ver_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t sel                          : 32; /**< yyyymmdd. */
#else
	uint64_t sel                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_mapper_ver_s         cnf75xx;
};
typedef union cvmx_sdlx_mapper_ver cvmx_sdlx_mapper_ver_t;

/**
 * cvmx_sdl#_rx_pat_ctrl
 *
 * This register controls the self-test pattern checker, and the external
 * loopback.
 */
union cvmx_sdlx_rx_pat_ctrl {
	uint64_t u64;
	struct cvmx_sdlx_rx_pat_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_26_63               : 38;
	uint64_t pattern_size                 : 4;  /**< The frame size for the self-test pattern, specified as a number of 122.88 MHz
                                                         reference clock cycles, divided by 8. For a 10 ms frame, set to
                                                         0x25800. */
	uint64_t reserved_5_21                : 17;
	uint64_t pattern_type                 : 2;  /**< Self-test pattern type. Must be set to 0. */
	uint64_t enable                       : 1;  /**< Set to one to enable the self-test pattern checker. */
	uint64_t r2t_far                      : 1;  /**< Set to one to enable the far external (RX-to-TX) loopback path (i.e.,
                                                         loop back that includes SDL formatting/de-formatting, but does not
                                                         including mapping). */
	uint64_t r2t_near                     : 1;  /**< Set to one to enable the near external (RX-to-TX) loopback path (i.e.,
                                                         loop back at the interface between SDL and the SerDes with no SDL
                                                         processing.) */
#else
	uint64_t r2t_near                     : 1;
	uint64_t r2t_far                      : 1;
	uint64_t enable                       : 1;
	uint64_t pattern_type                 : 2;
	uint64_t reserved_5_21                : 17;
	uint64_t pattern_size                 : 4;
	uint64_t reserved_26_63               : 38;
#endif
	} s;
	struct cvmx_sdlx_rx_pat_ctrl_s        cnf75xx;
};
typedef union cvmx_sdlx_rx_pat_ctrl cvmx_sdlx_rx_pat_ctrl_t;

/**
 * cvmx_sdl#_tx_pat_ctrl
 *
 * This register controls the self-test pattern generator, and the internal
 * loopback.
 */
union cvmx_sdlx_tx_pat_ctrl {
	uint64_t u64;
	struct cvmx_sdlx_tx_pat_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_26_63               : 38;
	uint64_t pattern_size                 : 4;  /**< The frame size for the self-test pattern, specified as a number of 122.88 MHz
                                                         reference clock cycles, divided by 8. For a 10 ms frame, set to
                                                         0x25800. */
	uint64_t reserved_5_21                : 17;
	uint64_t pattern_type                 : 2;  /**< Self-test pattern type. Must be set to 0. */
	uint64_t enable                       : 1;  /**< Set to 1 to enable the self-test pattern generator. */
	uint64_t t2r_far                      : 1;  /**< Set to one to enable the far internal (TX-to-RX) loopback path (i.e.,
                                                         loop back at the interface between SDL and the SerDes, and perform all
                                                         SDL processing). */
	uint64_t t2r_near                     : 1;  /**< far (=SDL mapper IF) loopback enable. Set to one to enable the near internal (TX-to-RX)
                                                         loopback path (i.e., loop back after performing SDL mapping, without performing SDL
                                                         formatting/de-formatting). */
#else
	uint64_t t2r_near                     : 1;
	uint64_t t2r_far                      : 1;
	uint64_t enable                       : 1;
	uint64_t pattern_type                 : 2;
	uint64_t reserved_5_21                : 17;
	uint64_t pattern_size                 : 4;
	uint64_t reserved_26_63               : 38;
#endif
	} s;
	struct cvmx_sdlx_tx_pat_ctrl_s        cnf75xx;
};
typedef union cvmx_sdlx_tx_pat_ctrl cvmx_sdlx_tx_pat_ctrl_t;

/**
 * cvmx_sdl#_ul_axc_bw_sel
 *
 * This register selects the bandwidth of each uplink antenna-carrier (AxC).
 *
 */
union cvmx_sdlx_ul_axc_bw_sel {
	uint64_t u64;
	struct cvmx_sdlx_ul_axc_bw_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t bw_sel                       : 32; /**< 2-bit selects for each UL AxC. Bits[1:0] control AxC 0, bits[3:2]
                                                         control AxC 1, etc.
                                                         0x0 = AxC disabled.
                                                         0x1 = 5 MHz (i.e., 7.68 Msps).
                                                         0x2 = 10 MHz (i.e., 15.36 Msps).
                                                         0x3 = 20 MHz (i.e., 30.72 Msps). */
#else
	uint64_t bw_sel                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ul_axc_bw_sel_s      cnf75xx;
};
typedef union cvmx_sdlx_ul_axc_bw_sel cvmx_sdlx_ul_axc_bw_sel_t;

/**
 * cvmx_sdl#_ul_axc_map_lut#
 *
 * These registers program the AxC mapping for the upload channels of SDL.
 *
 * Each SDL basic frame (or chip) contains 64 time slots, and each of these
 * 16 regsiters controls which AxC is mapped to each time slot.
 *
 * _ SDL(0..2)_UL_AXC_LUT(0) controls time slots [TS3, TS2, TS1, TS0] with fields 3-0
 * respectively.
 *
 * _ SDL(0..2)_UL_AXC_LUT(1) controls time slots [TS7, TS6, TS5, TS4] with fields 3-0
 * respectively,
 *
 * _ etc.
 *
 * Note that each AxC must be mapped to either 2, 4, or 8 time slots,
 * depending on whether the bandwidth is 5 MHz, 10 MHz, or 20 MHz,
 * respectively.
 *
 * Consecutive time samples for a given antenna should be assigned to strided time
 * slots as follows:
 * 5 MHz: TS x and TS x+32.
 * 10 MHz: TS x, TS x+16, TS x+32, and TS x+48.
 * 20 MHz: TS x, TS x+8, TS x+16, TS x+24, TS x+32, TS x+40, TS x+48, and TS x+56.
 */
union cvmx_sdlx_ul_axc_map_lutx {
	uint64_t u64;
	struct cvmx_sdlx_ul_axc_map_lutx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t enable3                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc3                         : 4;  /**< Chooses the target AxC for this time slot. */
	uint64_t reserved_21_23               : 3;
	uint64_t enable2                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc2                         : 4;  /**< Chooses the target AxC for this time slot. */
	uint64_t reserved_13_15               : 3;
	uint64_t enable1                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc1                         : 4;  /**< Chooses the target AxC for this time slot. */
	uint64_t reserved_5_7                 : 3;
	uint64_t enable0                      : 1;  /**< Enables the corresponding time slot to be mapped to an AxC. */
	uint64_t axc0                         : 4;  /**< Chooses the target AxC for this time slot. */
#else
	uint64_t axc0                         : 4;
	uint64_t enable0                      : 1;
	uint64_t reserved_5_7                 : 3;
	uint64_t axc1                         : 4;
	uint64_t enable1                      : 1;
	uint64_t reserved_13_15               : 3;
	uint64_t axc2                         : 4;
	uint64_t enable2                      : 1;
	uint64_t reserved_21_23               : 3;
	uint64_t axc3                         : 4;
	uint64_t enable3                      : 1;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_sdlx_ul_axc_map_lutx_s    cnf75xx;
};
typedef union cvmx_sdlx_ul_axc_map_lutx cvmx_sdlx_ul_axc_map_lutx_t;

/**
 * cvmx_sdl#_ul_axc_map_lut_lock
 *
 * This register needs to be set before updating the SDL UL AxC mapping lookup table or
 * carrier bandwidth select (UL_AXC_BW_SEL).  It then needs to be cleared when update is
 * complete.
 */
union cvmx_sdlx_ul_axc_map_lut_lock {
	uint64_t u64;
	struct cvmx_sdlx_ul_axc_map_lut_lock_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t lock                         : 1;  /**< Active high lock bit. */
#else
	uint64_t lock                         : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_sdlx_ul_axc_map_lut_lock_s cnf75xx;
};
typedef union cvmx_sdlx_ul_axc_map_lut_lock cvmx_sdlx_ul_axc_map_lut_lock_t;

/**
 * cvmx_sdl#_ul_bit_ctrl_sel
 */
union cvmx_sdlx_ul_bit_ctrl_sel {
	uint64_t u64;
	struct cvmx_sdlx_ul_bit_ctrl_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t sel                          : 2;  /**< 0x0 = No expansion (recommended).
                                                         0x1 = shift.
                                                         0x2 = sign ext.
                                                         0x3 = reserved. */
#else
	uint64_t sel                          : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_sdlx_ul_bit_ctrl_sel_s    cnf75xx;
};
typedef union cvmx_sdlx_ul_bit_ctrl_sel cvmx_sdlx_ul_bit_ctrl_sel_t;

/**
 * cvmx_sdl#_ul_lof_sel
 *
 * This register selects when SDL reports a loss of frame sync to RFIF.
 *
 */
union cvmx_sdlx_ul_lof_sel {
	uint64_t u64;
	struct cvmx_sdlx_ul_lof_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t sel                          : 2;  /**< 0x0 = Never report LOF.
                                                         0x1 = Report LOF only for chip LOF.
                                                         0x2 = Report LOF only for frame LOF.
                                                         0x3 = Report LOF when either chip or frame LOF occur. */
#else
	uint64_t sel                          : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_sdlx_ul_lof_sel_s         cnf75xx;
};
typedef union cvmx_sdlx_ul_lof_sel cvmx_sdlx_ul_lof_sel_t;

/**
 * cvmx_sdl#_ul_sync_err_cnt
 *
 * This register counts the frame and chip synchronization errors.
 *
 */
union cvmx_sdlx_ul_sync_err_cnt {
	uint64_t u64;
	struct cvmx_sdlx_ul_sync_err_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frame_sync_err_cnt           : 16; /**< Frame synchronization error count. */
	uint64_t chip_sync_err_cnt            : 16; /**< Chip synchronization error count. */
#else
	uint64_t chip_sync_err_cnt            : 16;
	uint64_t frame_sync_err_cnt           : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ul_sync_err_cnt_s    cnf75xx;
};
typedef union cvmx_sdlx_ul_sync_err_cnt cvmx_sdlx_ul_sync_err_cnt_t;

/**
 * cvmx_sdl#_ul_sync_gd_cnt
 *
 * This register counts good frames and chip synchronizations.
 *
 */
union cvmx_sdlx_ul_sync_gd_cnt {
	uint64_t u64;
	struct cvmx_sdlx_ul_sync_gd_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frame_sync_gd_cnt            : 16; /**< Good frame synchronization count. */
	uint64_t chip_sync_gd_cnt             : 16; /**< Good chip synchronization count. */
#else
	uint64_t chip_sync_gd_cnt             : 16;
	uint64_t frame_sync_gd_cnt            : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_sdlx_ul_sync_gd_cnt_s     cnf75xx;
};
typedef union cvmx_sdlx_ul_sync_gd_cnt cvmx_sdlx_ul_sync_gd_cnt_t;

#endif
