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
 * cvmx-tofb-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon tofb.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_TOFB_DEFS_H__
#define __CVMX_TOFB_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_BITS_PER_SAMPLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_BITS_PER_SAMPLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53340E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_BITS_PER_SAMPLE(offset) (CVMX_ADD_IO_SEG(0x00011800D53340E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_BIT_PER_SMPL_PLS_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_BIT_PER_SMPL_PLS_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334100ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_BIT_PER_SMPL_PLS_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800D5334100ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_CTRL_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_CTRL_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_CTRL_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5334020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_DENSITY_FORMAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_DENSITY_FORMAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_DENSITY_FORMAT(offset) (CVMX_ADD_IO_SEG(0x00011800D5334060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334160ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D5334160ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ERROR_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ERROR_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334500ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ERROR_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5334500ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_FIFO_LEVEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_FIFO_LEVEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334400ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_FIFO_LEVEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5334400ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_FRMS_PER_MULTIFRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_FRMS_PER_MULTIFRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334080ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_FRMS_PER_MULTIFRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5334080ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_0_3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_0_3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5335000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_0_3(offset) (CVMX_ADD_IO_SEG(0x00011800D5335000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_12_13(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_12_13(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5335060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_12_13(offset) (CVMX_ADD_IO_SEG(0x00011800D5335060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_4_7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_4_7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5335020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_4_7(offset) (CVMX_ADD_IO_SEG(0x00011800D5335020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_8_11(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_8_11(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5335040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_8_11(offset) (CVMX_ADD_IO_SEG(0x00011800D5335040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_COUNTER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_COUNTER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334700ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_COUNTER(offset) (CVMX_ADD_IO_SEG(0x00011800D5334700ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_LENGTH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_LENGTH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334240ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_LENGTH(offset) (CVMX_ADD_IO_SEG(0x00011800D5334240ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_ILA_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_ILA_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334200ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_ILA_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5334200ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_INTERLEAVE_MODE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_INTERLEAVE_MODE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334300ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_INTERLEAVE_MODE(offset) (CVMX_ADD_IO_SEG(0x00011800D5334300ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_IP_ID_AND_VERSION(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_IP_ID_AND_VERSION(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_IP_ID_AND_VERSION(offset) (CVMX_ADD_IO_SEG(0x00011800D5334000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_LANE_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_LANE_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334600ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_LANE_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5334600ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_NUMBER_OF_CONVERTERS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_NUMBER_OF_CONVERTERS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53340C0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_NUMBER_OF_CONVERTERS(offset) (CVMX_ADD_IO_SEG(0x00011800D53340C0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_NUMBER_OF_LANES(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_NUMBER_OF_LANES(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53340A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_NUMBER_OF_LANES(offset) (CVMX_ADD_IO_SEG(0x00011800D53340A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_OCTETS_PER_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_OCTETS_PER_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_OCTETS_PER_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5334040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_OVERSAMPLING_RATIO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_OVERSAMPLING_RATIO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334120ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_OVERSAMPLING_RATIO(offset) (CVMX_ADD_IO_SEG(0x00011800D5334120ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_SCRAMBLING_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_SCRAMBLING_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334140ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_SCRAMBLING_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D5334140ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_SUBCLASS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_SUBCLASS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53341E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_SUBCLASS(offset) (CVMX_ADD_IO_SEG(0x00011800D53341E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_SYNC_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_SYNC_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53341C0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_SYNC_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53341C0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_SYSREF_DELAY(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_SYSREF_DELAY(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5334220ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_SYSREF_DELAY(offset) (CVMX_ADD_IO_SEG(0x00011800D5334220ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_RXX_TEST_MODE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_RXX_TEST_MODE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53341A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_RXX_TEST_MODE(offset) (CVMX_ADD_IO_SEG(0x00011800D53341A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_BITS_PER_SAMPLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_BITS_PER_SAMPLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53300E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_BITS_PER_SAMPLE(offset) (CVMX_ADD_IO_SEG(0x00011800D53300E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_BIT_PER_SMPL_PLS_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_BIT_PER_SMPL_PLS_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330100ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_BIT_PER_SMPL_PLS_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800D5330100ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_CTRL_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_CTRL_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_CTRL_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D5330020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_DENSITY_FORMAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_DENSITY_FORMAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_DENSITY_FORMAT(offset) (CVMX_ADD_IO_SEG(0x00011800D5330060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330160ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D5330160ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_FIFO_LEVEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_FIFO_LEVEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330400ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_FIFO_LEVEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5330400ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_FRMS_PER_MULTIFRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_FRMS_PER_MULTIFRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330080ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_FRMS_PER_MULTIFRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5330080ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ILA_0_3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ILA_0_3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5331000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ILA_0_3(offset) (CVMX_ADD_IO_SEG(0x00011800D5331000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ILA_12_13(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ILA_12_13(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5331060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ILA_12_13(offset) (CVMX_ADD_IO_SEG(0x00011800D5331060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ILA_4_7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ILA_4_7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5331020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ILA_4_7(offset) (CVMX_ADD_IO_SEG(0x00011800D5331020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ILA_8_11(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ILA_8_11(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5331040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ILA_8_11(offset) (CVMX_ADD_IO_SEG(0x00011800D5331040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_ILA_LENGTH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_ILA_LENGTH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330240ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_ILA_LENGTH(offset) (CVMX_ADD_IO_SEG(0x00011800D5330240ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_INTERLEAVE_MODE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_INTERLEAVE_MODE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330300ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_INTERLEAVE_MODE(offset) (CVMX_ADD_IO_SEG(0x00011800D5330300ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_IP_ID_AND_VERSION(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_IP_ID_AND_VERSION(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_IP_ID_AND_VERSION(offset) (CVMX_ADD_IO_SEG(0x00011800D5330000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_NUMBER_OF_CONVERTERS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_NUMBER_OF_CONVERTERS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53300C0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_NUMBER_OF_CONVERTERS(offset) (CVMX_ADD_IO_SEG(0x00011800D53300C0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_NUMBER_OF_LANES(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_NUMBER_OF_LANES(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53300A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_NUMBER_OF_LANES(offset) (CVMX_ADD_IO_SEG(0x00011800D53300A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_OCTETS_PER_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_OCTETS_PER_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_OCTETS_PER_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5330040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_OVERSAMPLING_RATIO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_OVERSAMPLING_RATIO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330120ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_OVERSAMPLING_RATIO(offset) (CVMX_ADD_IO_SEG(0x00011800D5330120ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_SCRAMBLING_ENABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_SCRAMBLING_ENABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330140ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_SCRAMBLING_ENABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D5330140ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_SUBCLASS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_SUBCLASS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53301E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_SUBCLASS(offset) (CVMX_ADD_IO_SEG(0x00011800D53301E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_SYNC_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_SYNC_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53301C0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_SYNC_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53301C0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_TAILBITS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_TAILBITS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5330180ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_TAILBITS(offset) (CVMX_ADD_IO_SEG(0x00011800D5330180ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_TOFB_TXX_TEST_MODE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_TOFB_TXX_TEST_MODE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53301A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_TOFB_TXX_TEST_MODE(offset) (CVMX_ADD_IO_SEG(0x00011800D53301A0ull) + ((offset) & 7) * 32768)
#endif

/**
 * cvmx_tofb_rx#_bit_per_smpl_pls_ctl
 *
 * This register sets the total number of bits per sample,
 * including tail bits and control bits.
 */
union cvmx_tofb_rxx_bit_per_smpl_pls_ctl {
	uint64_t u64;
	struct cvmx_tofb_rxx_bit_per_smpl_pls_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ntotal                       : 5;  /**< The total number of bits per sample minus 1. This includes data bits,
                                                         control bits, and tail bits. Legal values are 11, 15, and 19 (i.e.,
                                                         12, 16, or 20 bits per sample).
                                                         The number of tail bits added to each sample will be
                                                         _ [NTOTAL] - TOFB_RX()_BITS_PER_SAMPLE[N] - TOFB_RX()_CTRL_CNT */
#else
	uint64_t ntotal                       : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_bit_per_smpl_pls_ctl_s cnf75xx;
};
typedef union cvmx_tofb_rxx_bit_per_smpl_pls_ctl cvmx_tofb_rxx_bit_per_smpl_pls_ctl_t;

/**
 * cvmx_tofb_rx#_bits_per_sample
 */
union cvmx_tofb_rxx_bits_per_sample {
	uint64_t u64;
	struct cvmx_tofb_rxx_bits_per_sample_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t n                            : 5;  /**< The number of bits per sample minus 1. Legal values are 9, 11, 13, and
                                                         15 to produce 10, 12, 14, or 16 bits per sample. All other values are
                                                         reserved. */
#else
	uint64_t n                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_bits_per_sample_s cnf75xx;
};
typedef union cvmx_tofb_rxx_bits_per_sample cvmx_tofb_rxx_bits_per_sample_t;

/**
 * cvmx_tofb_rx#_ctrl_cnt
 *
 * Number of control bits per sample.
 *
 */
union cvmx_tofb_rxx_ctrl_cnt {
	uint64_t u64;
	struct cvmx_tofb_rxx_ctrl_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t cs                           : 2;  /**< Number of control bits per sample. Any control bits will be ignored. */
#else
	uint64_t cs                           : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_tofb_rxx_ctrl_cnt_s       cnf75xx;
};
typedef union cvmx_tofb_rxx_ctrl_cnt cvmx_tofb_rxx_ctrl_cnt_t;

/**
 * cvmx_tofb_rx#_density_format
 */
union cvmx_tofb_rxx_density_format {
	uint64_t u64;
	struct cvmx_tofb_rxx_density_format_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t hd                           : 1;  /**< Selects the density format.
                                                         0 = Normal Density.
                                                         1 = High Density.
                                                         High density format must be used when
                                                         TOFB_RX()_BIT_PER_SMPL_PLS_CTL[NTOTAL] is set to 11 or 19 (i.e., 12 or
                                                         20 bits total bits per sample). */
#else
	uint64_t hd                           : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_rxx_density_format_s cnf75xx;
};
typedef union cvmx_tofb_rxx_density_format cvmx_tofb_rxx_density_format_t;

/**
 * cvmx_tofb_rx#_enable
 */
union cvmx_tofb_rxx_enable {
	uint64_t u64;
	struct cvmx_tofb_rxx_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t enablemodule                 : 1;  /**< Enable operation of the receive module:
                                                         0 = Disabled.
                                                         1 = Enabled.
                                                         If the system is re-synchronized by disabling and
                                                         enabling the module, insert a delay of at least 20 SerDes
                                                         clock cycles between the two register writes to allow the
                                                         FIFOs to be purged. */
#else
	uint64_t enablemodule                 : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_rxx_enable_s         cnf75xx;
};
typedef union cvmx_tofb_rxx_enable cvmx_tofb_rxx_enable_t;

/**
 * cvmx_tofb_rx#_error_cnt
 */
union cvmx_tofb_rxx_error_cnt {
	uint64_t u64;
	struct cvmx_tofb_rxx_error_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t fifo_level                   : 16; /**< The number of detected 8b10b decoding errors is counted for
                                                         each lane. Counter will saturate on maximum value. */
#else
	uint64_t fifo_level                   : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_tofb_rxx_error_cnt_s      cnf75xx;
};
typedef union cvmx_tofb_rxx_error_cnt cvmx_tofb_rxx_error_cnt_t;

/**
 * cvmx_tofb_rx#_fifo_level
 *
 * FIFO level.
 *
 */
union cvmx_tofb_rxx_fifo_level {
	uint64_t u64;
	struct cvmx_tofb_rxx_fifo_level_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t fifo_level                   : 5;  /**< Number of elements in the receive FIFO. */
#else
	uint64_t fifo_level                   : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_fifo_level_s     cnf75xx;
};
typedef union cvmx_tofb_rxx_fifo_level cvmx_tofb_rxx_fifo_level_t;

/**
 * cvmx_tofb_rx#_frms_per_multiframe
 */
union cvmx_tofb_rxx_frms_per_multiframe {
	uint64_t u64;
	struct cvmx_tofb_rxx_frms_per_multiframe_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t k                            : 5;  /**< Number of frames per multi frame minus 1. */
#else
	uint64_t k                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_frms_per_multiframe_s cnf75xx;
};
typedef union cvmx_tofb_rxx_frms_per_multiframe cvmx_tofb_rxx_frms_per_multiframe_t;

/**
 * cvmx_tofb_rx#_ila_0_3
 *
 * This register reports octets 0 to 3 of the received ILA sequence. The
 * data is valid once TOFB_RX()_ILA_STATUS[VALID] = 1.
 *
 * The octets are stored in big-endian order, i.e., octet 0 is stored in bits
 * <31:24>, octet 1 in bits <23:16>, etc.
 */
union cvmx_tofb_rxx_ila_0_3 {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_0_3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 0 to 3 of the received ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_0_3_s        cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_0_3 cvmx_tofb_rxx_ila_0_3_t;

/**
 * cvmx_tofb_rx#_ila_12_13
 *
 * This register reports octets 12 and 13 of the received ILA sequence. The
 * data is valid once TOFB_RX()_ILA_STATUS[VALID] = 1.
 *
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_rxx_ila_12_13 {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_12_13_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 16; /**< Octets 12 and 13 of the received ILA sequence. */
	uint64_t reserved_0_15                : 16;
#else
	uint64_t reserved_0_15                : 16;
	uint64_t ila_config                   : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_12_13_s      cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_12_13 cvmx_tofb_rxx_ila_12_13_t;

/**
 * cvmx_tofb_rx#_ila_4_7
 *
 * This register reports octets 4 to 7 of the received ILA sequence. The
 * data is valid once TOFB_RX()_ILA_STATUS[VALID] = 1.
 *
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_rxx_ila_4_7 {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_4_7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 4 to 7 of the received ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_4_7_s        cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_4_7 cvmx_tofb_rxx_ila_4_7_t;

/**
 * cvmx_tofb_rx#_ila_8_11
 *
 * This register reports octets 8 to 11 of the received ILA sequence. The
 * data is valid once TOFB_RX()_ILA_STATUS[VALID] = 1.
 *
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_rxx_ila_8_11 {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_8_11_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 8 to 11 of the received ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_8_11_s       cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_8_11 cvmx_tofb_rxx_ila_8_11_t;

/**
 * cvmx_tofb_rx#_ila_counter
 */
union cvmx_tofb_rxx_ila_counter {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_counter_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t len_det_done                 : 1;  /**< ILA length detection done. */
	uint64_t reserved_9_15                : 7;
	uint64_t lane_sync_done               : 9;  /**< The number of detected multi-frames in ILA sequence.  This counter
                                                         will count number of multi-frames with K28.0 as first character. The
                                                         counter will stop counting and [LEN_DET_DONE will be set when the
                                                         first multi-frame without K28.0 as first character is detected.  The
                                                         counter will saturate on maximum value.  A new measurement will be
                                                         initiated after resynchronization given by sync_b cleared. */
#else
	uint64_t lane_sync_done               : 9;
	uint64_t reserved_9_15                : 7;
	uint64_t len_det_done                 : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_counter_s    cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_counter cvmx_tofb_rxx_ila_counter_t;

/**
 * cvmx_tofb_rx#_ila_length
 */
union cvmx_tofb_rxx_ila_length {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_length_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ila_length                   : 8;  /**< Expected number of multi-frames in the ILA sequence minus 1.
                                                         Default setting is 3 for 4 multi-frames in the ILA sequence.
                                                         For subclass = 0 length can optionally be set to a larger value.
                                                         This setting is used to calculate the position for start of
                                                         sample decoding.  It is also used to determine expected
                                                         positions of K control symbols. */
#else
	uint64_t ila_length                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_length_s     cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_length cvmx_tofb_rxx_ila_length_t;

/**
 * cvmx_tofb_rx#_ila_status
 *
 * ILA valid.
 *
 */
union cvmx_tofb_rxx_ila_status {
	uint64_t u64;
	struct cvmx_tofb_rxx_ila_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t valid                        : 1;  /**< 0 = ILA not available.
                                                         1 = Recovered ILA available.
                                                         Note that TOFB does not validate the content of the received ILA sequence. */
#else
	uint64_t valid                        : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_rxx_ila_status_s     cnf75xx;
};
typedef union cvmx_tofb_rxx_ila_status cvmx_tofb_rxx_ila_status_t;

/**
 * cvmx_tofb_rx#_interleave_mode
 */
union cvmx_tofb_rxx_interleave_mode {
	uint64_t u64;
	struct cvmx_tofb_rxx_interleave_mode_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t intrlv_mode                  : 1;  /**< For the interface between RFIF RMAC and TOFB, specifies whether data
                                                         is interleaved:
                                                         0 = Interleaved.
                                                         1 = Non-interleaved.
                                                         Must be set to 1 since the RFIF RMAC does not interleave AxC data. */
#else
	uint64_t intrlv_mode                  : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_rxx_interleave_mode_s cnf75xx;
};
typedef union cvmx_tofb_rxx_interleave_mode cvmx_tofb_rxx_interleave_mode_t;

/**
 * cvmx_tofb_rx#_ip_id_and_version
 */
union cvmx_tofb_rxx_ip_id_and_version {
	uint64_t u64;
	struct cvmx_tofb_rxx_ip_id_and_version_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t ip_version                   : 8;  /**< Version number. */
	uint64_t id                           : 8;  /**< ID number. */
#else
	uint64_t id                           : 8;
	uint64_t ip_version                   : 8;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_tofb_rxx_ip_id_and_version_s cnf75xx;
};
typedef union cvmx_tofb_rxx_ip_id_and_version cvmx_tofb_rxx_ip_id_and_version_t;

/**
 * cvmx_tofb_rx#_lane_status
 */
union cvmx_tofb_rxx_lane_status {
	uint64_t u64;
	struct cvmx_tofb_rxx_lane_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t lane_sync_done               : 1;  /**< Lane resynchronization done. */
	uint64_t unexpected_k_rcvd            : 1;  /**< Unexpected K symbol received:
                                                         * K28.3 is accepted at last symbol of multi-frame.
                                                         * K28.7 is accepted at last symbol of frame.
                                                         * K28.0 is accepted at first symbol of multi-frame part of ILA sequence.
                                                         * K28.4 is accepted at second symbol of second multi-frame of ILA sequence.
                                                         * All other K symbols will generate error. */
	uint64_t frame_sync_state             : 2;  /**< State of frame synchronization state machine:
                                                         0x0 = FS_INIT.
                                                         0x1 = FS_CHECK.
                                                         0x2 = FS_DATA.
                                                         0x3 = Reserved. */
	uint64_t coding_group_state           : 2;  /**< State of coding group synchronization state machine:
                                                         0x0 = CS_INIT.
                                                         0x1 = CS_CHECK.
                                                         0x2 = CS_DATA. */
#else
	uint64_t coding_group_state           : 2;
	uint64_t frame_sync_state             : 2;
	uint64_t unexpected_k_rcvd            : 1;
	uint64_t lane_sync_done               : 1;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_tofb_rxx_lane_status_s    cnf75xx;
};
typedef union cvmx_tofb_rxx_lane_status cvmx_tofb_rxx_lane_status_t;

/**
 * cvmx_tofb_rx#_number_of_converters
 */
union cvmx_tofb_rxx_number_of_converters {
	uint64_t u64;
	struct cvmx_tofb_rxx_number_of_converters_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t m                            : 4;  /**< The number of converters minus 1. The number of converters should be
                                                         twice the number of AxCs. */
#else
	uint64_t m                            : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tofb_rxx_number_of_converters_s cnf75xx;
};
typedef union cvmx_tofb_rxx_number_of_converters cvmx_tofb_rxx_number_of_converters_t;

/**
 * cvmx_tofb_rx#_number_of_lanes
 */
union cvmx_tofb_rxx_number_of_lanes {
	uint64_t u64;
	struct cvmx_tofb_rxx_number_of_lanes_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t l                            : 3;  /**< The number of RX lanes minus 1. Must be set to 0. */
#else
	uint64_t l                            : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tofb_rxx_number_of_lanes_s cnf75xx;
};
typedef union cvmx_tofb_rxx_number_of_lanes cvmx_tofb_rxx_number_of_lanes_t;

/**
 * cvmx_tofb_rx#_octets_per_frame
 */
union cvmx_tofb_rxx_octets_per_frame {
	uint64_t u64;
	struct cvmx_tofb_rxx_octets_per_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t f                            : 8;  /**< The number octets (bytes) per frame. Legal values are 1, 2, 4, 8, and
                                                         16. All other values are reserved. */
#else
	uint64_t f                            : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tofb_rxx_octets_per_frame_s cnf75xx;
};
typedef union cvmx_tofb_rxx_octets_per_frame cvmx_tofb_rxx_octets_per_frame_t;

/**
 * cvmx_tofb_rx#_oversampling_ratio
 *
 * Number of samples per converter per frame.
 *
 */
union cvmx_tofb_rxx_oversampling_ratio {
	uint64_t u64;
	struct cvmx_tofb_rxx_oversampling_ratio_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t s                            : 5;  /**< Currently this value is ignored, and IP only
                                                         supports S=1 (i.e. [S] = 0). */
#else
	uint64_t s                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_oversampling_ratio_s cnf75xx;
};
typedef union cvmx_tofb_rxx_oversampling_ratio cvmx_tofb_rxx_oversampling_ratio_t;

/**
 * cvmx_tofb_rx#_scrambling_enable
 */
union cvmx_tofb_rxx_scrambling_enable {
	uint64_t u64;
	struct cvmx_tofb_rxx_scrambling_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t scr                          : 1;  /**< 0 = Scrambling disabled.
                                                         1 = Scrambling enabled. */
#else
	uint64_t scr                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_rxx_scrambling_enable_s cnf75xx;
};
typedef union cvmx_tofb_rxx_scrambling_enable cvmx_tofb_rxx_scrambling_enable_t;

/**
 * cvmx_tofb_rx#_subclass
 */
union cvmx_tofb_rxx_subclass {
	uint64_t u64;
	struct cvmx_tofb_rxx_subclass_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t subclass                     : 2;  /**< Select the operation subclass of the transmitter.
                                                         0 = Subclass 0.
                                                         1 = Subclass 1. */
#else
	uint64_t subclass                     : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_tofb_rxx_subclass_s       cnf75xx;
};
typedef union cvmx_tofb_rxx_subclass cvmx_tofb_rxx_subclass_t;

/**
 * cvmx_tofb_rx#_sync_status
 *
 * Synchronization status.
 *
 */
union cvmx_tofb_rxx_sync_status {
	uint64_t u64;
	struct cvmx_tofb_rxx_sync_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t sync_b_hold_low              : 1;  /**< Value of sync_b output signal with hold of low. */
	uint64_t sync_b                       : 1;  /**< Value of sync_b output signal */
	uint64_t sysref_sync_err              : 1;  /**< SYSREF synchronization error (subclass 1). */
	uint64_t sysref_sync_status           : 1;  /**< SYSREF synchronization status of the module:
                                                         0 = Not synchronized.
                                                         1 = Synchronized. */
	uint64_t rcv_active                   : 1;  /**< Receiver active. */
#else
	uint64_t rcv_active                   : 1;
	uint64_t sysref_sync_status           : 1;
	uint64_t sysref_sync_err              : 1;
	uint64_t sync_b                       : 1;
	uint64_t sync_b_hold_low              : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_rxx_sync_status_s    cnf75xx;
};
typedef union cvmx_tofb_rxx_sync_status cvmx_tofb_rxx_sync_status_t;

/**
 * cvmx_tofb_rx#_sysref_delay
 *
 * ILA valid.
 *
 */
union cvmx_tofb_rxx_sysref_delay {
	uint64_t u64;
	struct cvmx_tofb_rxx_sysref_delay_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t sysref_delay                 : 6;  /**< For subclass 1 operation, this register defines delay from LMFC edge
                                                         to start of data decodiing. This value should be set to the total
                                                         delay from the transmitter to the receiver. */
#else
	uint64_t sysref_delay                 : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_tofb_rxx_sysref_delay_s   cnf75xx;
};
typedef union cvmx_tofb_rxx_sysref_delay cvmx_tofb_rxx_sysref_delay_t;

/**
 * cvmx_tofb_rx#_test_mode
 */
union cvmx_tofb_rxx_test_mode {
	uint64_t u64;
	struct cvmx_tofb_rxx_test_mode_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t enable_ila                   : 1;  /**< Enable ILA test mode. When set to 1, the TOFB RX expects to receive the code group
                                                         sequence
                                                         followed by repeated ILA sequences as input. The TOFB_RX()_ILA_x_y
                                                         registers will be updated with each received ILA sequence. */
	uint64_t enable_short_sync            : 1;  /**< Enable short 1 LMFC repeated lane synchronization. */
	uint64_t replace_char                 : 1;  /**< Enable character replacement. */
#else
	uint64_t replace_char                 : 1;
	uint64_t enable_short_sync            : 1;
	uint64_t enable_ila                   : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tofb_rxx_test_mode_s      cnf75xx;
};
typedef union cvmx_tofb_rxx_test_mode cvmx_tofb_rxx_test_mode_t;

/**
 * cvmx_tofb_tx#_bit_per_smpl_pls_ctl
 *
 * This register sets the total number of bits per sample,
 * including tail bits.
 */
union cvmx_tofb_txx_bit_per_smpl_pls_ctl {
	uint64_t u64;
	struct cvmx_tofb_txx_bit_per_smpl_pls_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t ntotal                       : 5;  /**< The total number of bits per sample minus 1. This includes data bits,
                                                         control bits, and tail bits. Legal values are 11, 15, and 19 (i.e.,
                                                         12, 16, or 20 bits per sample). The number of control bits per sample
                                                         should be set to 0, in which case, the number of tail bits added to
                                                         each sample will be [NTOTAL] - TOFB_TX()_BITS_PER_SAMPLE[N]. */
#else
	uint64_t ntotal                       : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_bit_per_smpl_pls_ctl_s cnf75xx;
};
typedef union cvmx_tofb_txx_bit_per_smpl_pls_ctl cvmx_tofb_txx_bit_per_smpl_pls_ctl_t;

/**
 * cvmx_tofb_tx#_bits_per_sample
 */
union cvmx_tofb_txx_bits_per_sample {
	uint64_t u64;
	struct cvmx_tofb_txx_bits_per_sample_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t n                            : 5;  /**< The number of bits per sample minus 1. Legal values are 9, 11, 13, and
                                                         15 to produce 10, 12, 14, or 16 bits per sample. All other values are
                                                         reserved. */
#else
	uint64_t n                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_bits_per_sample_s cnf75xx;
};
typedef union cvmx_tofb_txx_bits_per_sample cvmx_tofb_txx_bits_per_sample_t;

/**
 * cvmx_tofb_tx#_ctrl_cnt
 */
union cvmx_tofb_txx_ctrl_cnt {
	uint64_t u64;
	struct cvmx_tofb_txx_ctrl_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t cs                           : 2;  /**< Number of control bits per sample. Must be set to 0. */
#else
	uint64_t cs                           : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_tofb_txx_ctrl_cnt_s       cnf75xx;
};
typedef union cvmx_tofb_txx_ctrl_cnt cvmx_tofb_txx_ctrl_cnt_t;

/**
 * cvmx_tofb_tx#_density_format
 */
union cvmx_tofb_txx_density_format {
	uint64_t u64;
	struct cvmx_tofb_txx_density_format_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t hd                           : 1;  /**< Selects the density format.
                                                         0 = Normal Density.
                                                         1 = High Density.
                                                         High density format must be used when
                                                         TOFB_TX()_BIT_PER_SMPL_PLS_CTL[NTOTAL] is set to 11 or 19 (i.e., 12 or
                                                         20 bits total bits per sample). */
#else
	uint64_t hd                           : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_txx_density_format_s cnf75xx;
};
typedef union cvmx_tofb_txx_density_format cvmx_tofb_txx_density_format_t;

/**
 * cvmx_tofb_tx#_enable
 */
union cvmx_tofb_txx_enable {
	uint64_t u64;
	struct cvmx_tofb_txx_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t enablemodule                 : 1;  /**< Enable operation of the transmit module:
                                                         0 = Disabled.
                                                         1 = Enabled.
                                                         If the system is re-synchronized by disabling and
                                                         enabling the module, insert a delay of at least 20 SerDes
                                                         clock cycles between the two register writes to allow time for the
                                                         FIFOs to drain. */
#else
	uint64_t enablemodule                 : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_txx_enable_s         cnf75xx;
};
typedef union cvmx_tofb_txx_enable cvmx_tofb_txx_enable_t;

/**
 * cvmx_tofb_tx#_fifo_level
 */
union cvmx_tofb_txx_fifo_level {
	uint64_t u64;
	struct cvmx_tofb_txx_fifo_level_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t fifo_level                   : 5;  /**< Number of elements in the TX FIFO. */
#else
	uint64_t fifo_level                   : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_fifo_level_s     cnf75xx;
};
typedef union cvmx_tofb_txx_fifo_level cvmx_tofb_txx_fifo_level_t;

/**
 * cvmx_tofb_tx#_frms_per_multiframe
 */
union cvmx_tofb_txx_frms_per_multiframe {
	uint64_t u64;
	struct cvmx_tofb_txx_frms_per_multiframe_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t k                            : 5;  /**< The number of frames per multi-frame minus 1. */
#else
	uint64_t k                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_frms_per_multiframe_s cnf75xx;
};
typedef union cvmx_tofb_txx_frms_per_multiframe cvmx_tofb_txx_frms_per_multiframe_t;

/**
 * cvmx_tofb_tx#_ila_0_3
 *
 * This register stores octets 0 to 3 of the ILA sequence to transmit.
 * The octets are stored in big-endian order, i.e., octet 0 is stored in bits
 * <31:24>, octet 1 in bits <23:16>, etc.
 */
union cvmx_tofb_txx_ila_0_3 {
	uint64_t u64;
	struct cvmx_tofb_txx_ila_0_3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 0 to 3 of the ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_txx_ila_0_3_s        cnf75xx;
};
typedef union cvmx_tofb_txx_ila_0_3 cvmx_tofb_txx_ila_0_3_t;

/**
 * cvmx_tofb_tx#_ila_12_13
 *
 * This register stores octets 12 to 13 of the ILA sequence to transmit.
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_txx_ila_12_13 {
	uint64_t u64;
	struct cvmx_tofb_txx_ila_12_13_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 16; /**< Octets 12 and 13 of the ILA sequence. */
	uint64_t resd                         : 16; /**< Reserved. */
#else
	uint64_t resd                         : 16;
	uint64_t ila_config                   : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_txx_ila_12_13_s      cnf75xx;
};
typedef union cvmx_tofb_txx_ila_12_13 cvmx_tofb_txx_ila_12_13_t;

/**
 * cvmx_tofb_tx#_ila_4_7
 *
 * This register stores octets 4 to 7 of the ILA sequence to transmit.
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_txx_ila_4_7 {
	uint64_t u64;
	struct cvmx_tofb_txx_ila_4_7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 4 to 7 of the ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_txx_ila_4_7_s        cnf75xx;
};
typedef union cvmx_tofb_txx_ila_4_7 cvmx_tofb_txx_ila_4_7_t;

/**
 * cvmx_tofb_tx#_ila_8_11
 *
 * This register stores octets 8 to 11 of the ILA sequence to transmit.
 * The octets are stored in big-endian order.
 */
union cvmx_tofb_txx_ila_8_11 {
	uint64_t u64;
	struct cvmx_tofb_txx_ila_8_11_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t ila_config                   : 32; /**< Octets 8 to 11 of the ILA sequence. */
#else
	uint64_t ila_config                   : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_tofb_txx_ila_8_11_s       cnf75xx;
};
typedef union cvmx_tofb_txx_ila_8_11 cvmx_tofb_txx_ila_8_11_t;

/**
 * cvmx_tofb_tx#_ila_length
 */
union cvmx_tofb_txx_ila_length {
	uint64_t u64;
	struct cvmx_tofb_txx_ila_length_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t ila_length                   : 8;  /**< Number of multi-frames in the transmitted ILA sequence minus 1.
                                                         Default setting is 3 for 4 multi-frames in the ILA sequence.
                                                         For subclass = 0 length can optionally be set to larger value. */
#else
	uint64_t ila_length                   : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tofb_txx_ila_length_s     cnf75xx;
};
typedef union cvmx_tofb_txx_ila_length cvmx_tofb_txx_ila_length_t;

/**
 * cvmx_tofb_tx#_interleave_mode
 */
union cvmx_tofb_txx_interleave_mode {
	uint64_t u64;
	struct cvmx_tofb_txx_interleave_mode_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t intrlv_mode                  : 1;  /**< For the interface between RFIF RMAC and TOFB, specifies whether data
                                                         is interleaved:
                                                         0 = Interleaved.
                                                         1 = Non-interleaved.
                                                         Must be set to 1 since the RFIF RMAC does not interleave AxC data. */
#else
	uint64_t intrlv_mode                  : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_txx_interleave_mode_s cnf75xx;
};
typedef union cvmx_tofb_txx_interleave_mode cvmx_tofb_txx_interleave_mode_t;

/**
 * cvmx_tofb_tx#_ip_id_and_version
 */
union cvmx_tofb_txx_ip_id_and_version {
	uint64_t u64;
	struct cvmx_tofb_txx_ip_id_and_version_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t ip_version                   : 8;  /**< Version number. */
	uint64_t id                           : 8;  /**< ID number. */
#else
	uint64_t id                           : 8;
	uint64_t ip_version                   : 8;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_tofb_txx_ip_id_and_version_s cnf75xx;
};
typedef union cvmx_tofb_txx_ip_id_and_version cvmx_tofb_txx_ip_id_and_version_t;

/**
 * cvmx_tofb_tx#_number_of_converters
 */
union cvmx_tofb_txx_number_of_converters {
	uint64_t u64;
	struct cvmx_tofb_txx_number_of_converters_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t m                            : 4;  /**< The number of converters minus 1. The number of converters should be
                                                         twice the number of AxCs. */
#else
	uint64_t m                            : 4;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_tofb_txx_number_of_converters_s cnf75xx;
};
typedef union cvmx_tofb_txx_number_of_converters cvmx_tofb_txx_number_of_converters_t;

/**
 * cvmx_tofb_tx#_number_of_lanes
 */
union cvmx_tofb_txx_number_of_lanes {
	uint64_t u64;
	struct cvmx_tofb_txx_number_of_lanes_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t l                            : 3;  /**< The number of TX lanes minus 1. Must be set to 0. */
#else
	uint64_t l                            : 3;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tofb_txx_number_of_lanes_s cnf75xx;
};
typedef union cvmx_tofb_txx_number_of_lanes cvmx_tofb_txx_number_of_lanes_t;

/**
 * cvmx_tofb_tx#_octets_per_frame
 */
union cvmx_tofb_txx_octets_per_frame {
	uint64_t u64;
	struct cvmx_tofb_txx_octets_per_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t f                            : 8;  /**< The number octets (bytes) per frame. Legal values are 1, 2, 4, 8, and
                                                         16. All other values are reserved. */
#else
	uint64_t f                            : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_tofb_txx_octets_per_frame_s cnf75xx;
};
typedef union cvmx_tofb_txx_octets_per_frame cvmx_tofb_txx_octets_per_frame_t;

/**
 * cvmx_tofb_tx#_oversampling_ratio
 *
 * Number of samples per converter per frame.
 *
 */
union cvmx_tofb_txx_oversampling_ratio {
	uint64_t u64;
	struct cvmx_tofb_txx_oversampling_ratio_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t s                            : 5;  /**< Reserved. Must be 0. */
#else
	uint64_t s                            : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_oversampling_ratio_s cnf75xx;
};
typedef union cvmx_tofb_txx_oversampling_ratio cvmx_tofb_txx_oversampling_ratio_t;

/**
 * cvmx_tofb_tx#_scrambling_enable
 */
union cvmx_tofb_txx_scrambling_enable {
	uint64_t u64;
	struct cvmx_tofb_txx_scrambling_enable_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t scr                          : 1;  /**< 0 = Scrambling disabled.
                                                         1 = Scrambling enabled. */
#else
	uint64_t scr                          : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_txx_scrambling_enable_s cnf75xx;
};
typedef union cvmx_tofb_txx_scrambling_enable cvmx_tofb_txx_scrambling_enable_t;

/**
 * cvmx_tofb_tx#_subclass
 */
union cvmx_tofb_txx_subclass {
	uint64_t u64;
	struct cvmx_tofb_txx_subclass_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t subclass                     : 2;  /**< Select the operation subclass of the transmitter.
                                                         0 = Subclass 0.
                                                         1 = Subclass 1. */
#else
	uint64_t subclass                     : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_tofb_txx_subclass_s       cnf75xx;
};
typedef union cvmx_tofb_txx_subclass cvmx_tofb_txx_subclass_t;

/**
 * cvmx_tofb_tx#_sync_status
 */
union cvmx_tofb_txx_sync_status {
	uint64_t u64;
	struct cvmx_tofb_txx_sync_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t sync_b_hold_low              : 1;  /**< Value of sync_b input signal with hold of low value. */
	uint64_t sync_b                       : 1;  /**< Value of sync_b input signal. */
	uint64_t sysref_sync_err              : 1;  /**< SYSREF synchronization error (subclass 1). */
	uint64_t sysref_sync_status           : 1;  /**< SYSREF synchronization status of the module:
                                                         0 = Not synchronized.
                                                         1 = Synchronized. */
	uint64_t tx_active                    : 1;  /**< Transmitter active. */
#else
	uint64_t tx_active                    : 1;
	uint64_t sysref_sync_status           : 1;
	uint64_t sysref_sync_err              : 1;
	uint64_t sync_b                       : 1;
	uint64_t sync_b_hold_low              : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_tofb_txx_sync_status_s    cnf75xx;
};
typedef union cvmx_tofb_txx_sync_status cvmx_tofb_txx_sync_status_t;

/**
 * cvmx_tofb_tx#_tailbits
 */
union cvmx_tofb_txx_tailbits {
	uint64_t u64;
	struct cvmx_tofb_txx_tailbits_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t tailbits                     : 1;  /**< Controls the value of the tail bits.
                                                         0 = Insert 0s.
                                                         1 = Insert 1s. */
#else
	uint64_t tailbits                     : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_tofb_txx_tailbits_s       cnf75xx;
};
typedef union cvmx_tofb_txx_tailbits cvmx_tofb_txx_tailbits_t;

/**
 * cvmx_tofb_tx#_test_mode
 */
union cvmx_tofb_txx_test_mode {
	uint64_t u64;
	struct cvmx_tofb_txx_test_mode_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t enable_char_repl             : 1;  /**< Enable character replacement. */
	uint64_t test_mode                    : 2;  /**< Transmit one of the standard test sequences.
                                                         0x0 = Normal mode, no test sequence tranmitted.
                                                         0x1 = Continuous sequence of D21.5 characters.
                                                         0x2 = Continuous sequence of K28.5 characters.
                                                         0x3 = Code group sequence followed by repeated
                                                              lane alignment (ILA) sequence. */
#else
	uint64_t test_mode                    : 2;
	uint64_t enable_char_repl             : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_tofb_txx_test_mode_s      cnf75xx;
};
typedef union cvmx_tofb_txx_test_mode cvmx_tofb_txx_test_mode_t;

#endif
