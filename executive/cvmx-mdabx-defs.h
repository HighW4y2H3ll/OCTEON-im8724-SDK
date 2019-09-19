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
 * cvmx-mdabx-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon mdabx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_MDABX_DEFS_H__
#define __CVMX_MDABX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_CFG_ADDR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_CFG_ADDR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80030ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_CFG_ADDR(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80030ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_CFG_LENGTH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_CFG_LENGTH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80050ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_CFG_LENGTH(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80050ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_CFG_LIMIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_CFG_LIMIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80040ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_CFG_LIMIT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80040ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_CFG_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_CFG_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80020ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_CFG_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80020ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_DAC_BIST_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_DAC_BIST_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80200ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_DAC_BIST_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80200ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_DAC_ECO(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_DAC_ECO(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80300ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_DAC_ECO(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80300ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_DAC_MEMBASE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_DAC_MEMBASE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801D0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_DAC_MEMBASE(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801D0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_DAC_TIMER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_DAC_TIMER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80210ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_DAC_TIMER(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80210ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_DMEM_ARRAYX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 32767)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDABX_DMEM_ARRAYX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4040000ull) + (((offset) & 32767) + ((block_id) & 31) * 0x10000ull) * 8;
}
#else
#define CVMX_MDABX_DMEM_ARRAYX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4040000ull) + (((offset) & 32767) + ((block_id) & 31) * 0x10000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_ERROR_ADDRESS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_ERROR_ADDRESS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80270ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_ERROR_ADDRESS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80270ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_ERROR_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_ERROR_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80260ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_ERROR_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80260ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80350ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP0(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80350ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80360ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP1(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80360ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80370ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP2(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80370ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80380ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP3(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80380ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP4(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP4(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80390ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP4(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80390ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP5(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP5(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F803A0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP5(offset) (CVMX_ADD_IO_SEG(0x00011800B4F803A0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP6(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP6(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F803B0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP6(offset) (CVMX_ADD_IO_SEG(0x00011800B4F803B0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GP7(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GP7(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F803C0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GP7(offset) (CVMX_ADD_IO_SEG(0x00011800B4F803C0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GPIO_IN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GPIO_IN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80320ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GPIO_IN(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80320ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_GPIO_OUT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_GPIO_OUT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80310ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_GPIO_OUT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80310ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_ID(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_ID(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80340ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_ID(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80340ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_IMEM_ARRAYX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 32767)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDABX_IMEM_ARRAYX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4000000ull) + (((offset) & 32767) + ((block_id) & 31) * 0x10000ull) * 8;
}
#else
#define CVMX_MDABX_IMEM_ARRAYX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4000000ull) + (((offset) & 32767) + ((block_id) & 31) * 0x10000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INTERRUPT_ACTIVE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INTERRUPT_ACTIVE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80280ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INTERRUPT_ACTIVE(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80280ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80170ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80170ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80180ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80180ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80190ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80190ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_SRC(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_SRC(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801A0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_SRC(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801A0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_SUM_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_SUM_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80150ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_SUM_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80150ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_INT_SUM_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_INT_SUM_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80160ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_INT_SUM_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80160ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_JOB_STATUS1X(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDABX_JOB_STATUS1X(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F800E0ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDABX_JOB_STATUS1X(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F800E0ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_JOB_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDABX_JOB_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F80000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDABX_JOB_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F80000ull) + (((offset) & 1) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_LD_INT_ENA_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_LD_INT_ENA_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80240ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_LD_INT_ENA_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80240ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_LD_INT_ENA_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_LD_INT_ENA_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80250ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_LD_INT_ENA_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80250ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_LD_INT_SUM_W1C(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_LD_INT_SUM_W1C(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80220ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_LD_INT_SUM_W1C(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80220ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_LD_INT_SUM_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_LD_INT_SUM_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80230ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_LD_INT_SUM_W1S(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80230ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PFIO_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PFIO_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80330ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PFIO_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80330ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PROC_CTL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PROC_CTL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801C0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PROC_CTL(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801C0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PROC_DEBUG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PROC_DEBUG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801E0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PROC_DEBUG(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801E0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PROC_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PROC_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801B0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PROC_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801B0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PSM_CMDX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 17))))))
		cvmx_warn("CVMX_MDABX_PSM_CMDX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800B4F80100ull) + (((offset) & 3) + ((block_id) & 31) * 0x400ull) * 16;
}
#else
#define CVMX_MDABX_PSM_CMDX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800B4F80100ull) + (((offset) & 3) + ((block_id) & 31) * 0x400ull) * 16)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PSM_CMD_PUSH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PSM_CMD_PUSH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80140ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PSM_CMD_PUSH(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80140ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_PSM_TIMER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_PSM_TIMER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F801F0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_PSM_TIMER(offset) (CVMX_ADD_IO_SEG(0x00011800B4F801F0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_RD_ADDR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_RD_ADDR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80070ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_RD_ADDR(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80070ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_RD_LENGTH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_RD_LENGTH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80090ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_RD_LENGTH(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80090ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_RD_LIMIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_RD_LIMIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80080ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_RD_LIMIT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80080ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_RD_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_RD_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F80060ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_RD_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F80060ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_WR_ADDR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_WR_ADDR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F800B0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_WR_ADDR(offset) (CVMX_ADD_IO_SEG(0x00011800B4F800B0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_WR_LENGTH(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_WR_LENGTH(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F800D0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_WR_LENGTH(offset) (CVMX_ADD_IO_SEG(0x00011800B4F800D0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_WR_LIMIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_WR_LIMIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F800C0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_WR_LIMIT(offset) (CVMX_ADD_IO_SEG(0x00011800B4F800C0ull) + ((offset) & 31) * 16384)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MDABX_WR_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 17)))))
		cvmx_warn("CVMX_MDABX_WR_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800B4F800A0ull) + ((offset) & 31) * 16384;
}
#else
#define CVMX_MDABX_WR_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800B4F800A0ull) + ((offset) & 31) * 16384)
#endif

/**
 * cvmx_mdab#_cfg_addr
 *
 * The starting address used by the CFG-DMA engine when writing the job configuration to local
 * DSP memory.
 */
union cvmx_mdabx_cfg_addr {
	uint64_t u64;
	struct cvmx_mdabx_cfg_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_31_63               : 33;
	uint64_t addr                         : 27; /**< The 128b-aligned starting address used by the CFG-DMA engine when writing the job
                                                         configuration to local DSP memory.
                                                         When ADDR[30]=1(IMEM):
                                                            For MBP32 (256KB IMEM): Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4003.FFFF >> 4).
                                                            For SSP16 (64KB IMEM):  Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4000.FFFF >> 4).
                                                         When ADDR[30]=0(DMEM):
                                                            For MBP32 (128KB DMEM): Valid 128b-aligned address range = (0x3FFE.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                            For SSP16 (256KB DMEM): Valid 128b-aligned address range = (0x3FFC.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                         NOTE: In the I/D MEM address decoding scheme, ADDR[30] determines WHICH DSP Memory space
                                                         (0:DMEM/1:IMEM) */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t addr                         : 27;
	uint64_t reserved_31_63               : 33;
#endif
	} s;
	struct cvmx_mdabx_cfg_addr_s          cnf75xx;
};
typedef union cvmx_mdabx_cfg_addr cvmx_mdabx_cfg_addr_t;

/**
 * cvmx_mdab#_cfg_length
 *
 * The actual 128-bit length of the most recent CFG-DMA engine job configuration transfer.
 *
 */
union cvmx_mdabx_cfg_length {
	uint64_t u64;
	struct cvmx_mdabx_cfg_length_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t length                       : 15; /**< The actual 128-bit length of the most recent CFG-DMA engine job configuration transfer.
                                                         NOTE: This field will be updated when the CFG-DMA has been actually write committed to
                                                         local DSP memory,
                                                         which allows software to read (poll) the length field during the CFG-DMA transfer to know
                                                         how many 128-bit chunks are
                                                         now currently available to be read and used. */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t length                       : 15;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdabx_cfg_length_s        cnf75xx;
};
typedef union cvmx_mdabx_cfg_length cvmx_mdabx_cfg_length_t;

/**
 * cvmx_mdab#_cfg_limit
 *
 * The maximum number of 128-bit sized chunks that the CFG-DMA engine may write starting from the
 * 128-bit-aligned MDAB_CFG_ADDR.
 */
union cvmx_mdabx_cfg_limit {
	uint64_t u64;
	struct cvmx_mdabx_cfg_limit_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t last                         : 1;  /**< Indicates the last CFG-DMA slice which may be written to address MDAB_CFG_ADDR.
                                                         DSP software may choose to segment the JD.CFG-DMA data into multiple non-contiguous CFG-
                                                         DMA transfers to DSP local memory.
                                                         Eventually, DSP software will indicate the LAST segment (or slice), to indicate this is
                                                         the final CFG-DMA sub-blk transfer.
                                                         Each CFG-DMA operation's LAST will indicate when the last segment (or slice) CFG-DMA
                                                         operation has been set up to complete the JD.CFG-DMA data transfer.
                                                         SW_NOTE: For every new CFG-DMA slice operation, DSP software *MUST* signal LAST to
                                                         indicate the final JD.CFG-DMA data slice transfer. */
	uint64_t reserved_19_30               : 12;
	uint64_t limit                        : 15; /**< "The maximum number of 128-bit chunks that the CFG-DMA engine may write at address
                                                         MDAB_CFG_ADDR.
                                                         Typically, DSP software will allocate LIMIT buffer space(max) to be larger than the
                                                         JD.JCFG
                                                         data. DSP software is at liberty to break the total CFG-DMA transfer into multiples
                                                         segments (or
                                                         DMA slices) using the LIMIT register. When either LIMIT has been reached (or if the last
                                                         JD.JCFG data is detected), the CFG-DMA is done.
                                                         For MBP DSP:
                                                            IMEM: 0x4000.0000 - 0x4003.FFFF
                                                            DMEM: 0x3FFE.0000 - 0x3FFF.FFFF
                                                         For SSP DSP:
                                                            IMEM: 0x4000.0000 - 0x4000.FFFF
                                                            DMEM: 0x3FFC.0000 - 0x3FFF.FFFF
                                                         Software restriction \#1: software should never program LIMIT < 2. Indeterminate results
                                                         can/will occur.
                                                         Software restriction \#2: software should ensure that ADDR+LIMIT SIZES fall within the I+D
                                                         MEM space, otherwise DMA_ERROR and indeterminate results can/will occur." */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t limit                        : 15;
	uint64_t reserved_19_30               : 12;
	uint64_t last                         : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_cfg_limit_s         cnf75xx;
};
typedef union cvmx_mdabx_cfg_limit cvmx_mdabx_cfg_limit_t;

/**
 * cvmx_mdab#_cfg_status
 *
 * Reports the status for the CFG-DMA engine.
 *
 */
union cvmx_mdabx_cfg_status {
	uint64_t u64;
	struct cvmx_mdabx_cfg_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t job_tag                      : 16; /**< The JOB_TAG field from the PSM command for the last completed CFG-DMA operation.
                                                         When DSP software writes [START_BUSY]=0->1, the CFG-DMA engine will wait until the next
                                                         available Job has been enqueued, at which point the [JOB_TAG] will be latched in. */
	uint64_t reserved_10_15               : 6;
	uint64_t pending_slot                 : 1;  /**< The job slot for the pending CFG-DMA operation. Valid only when [START_BUSY] == 0 and
                                                         [DMA_PENDING] == 1. */
	uint64_t job_slot                     : 1;  /**< The job slot for the most recent CFG-DMA operation. */
	uint64_t reserved_7_7                 : 1;
	uint64_t dma_uflow                    : 1;  /**< Set when the last CFG-DMA operation encountered an underflow */
	uint64_t dma_oflow                    : 1;  /**< Set when the last CFG-DMA operation encountered an overflow */
	uint64_t dma_error                    : 1;  /**< Set when the last CFG-DMA operation encountered an error. */
	uint64_t last_dma_done                : 1;  /**< Set when the last CFG-DMA operation transferred the last word of the job configuration
                                                         section from the job descriptor. */
	uint64_t dma_pending                  : 1;  /**< Set when a CFG-DMA operation is pending for the engine and the internal interface needs to
                                                         be programmed. */
	uint64_t post_reset                   : 1;  /**< Set to one on MDAB reset and remains one until the CFG-DMA engine starts its first
                                                         CFG-DMA transfer.
                                                         SWNOTE: DSP software can use this bit to qualify the [JOB_TAG], [JOB_SLOT], [DMA_ERROR],
                                                         [LAST_DMA_DONE]
                                                         bits in this CSR. [see: these status bits remain set and [POST_RESET] is used by software
                                                         to know when they are truly valid].
                                                         SET(hardware): MDAB reset.
                                                         CLR(software): DSP writes [START_BUSY]=0->1. */
	uint64_t start_busy                   : 1;  /**< When software writes a one to this register, hardware will write the job configuration
                                                         section(JD.JCFG) for the next available job into the local memory starting at
                                                         MDAB_CFG_ADDR, up to
                                                         MDAB_CFG_ADDR+MDAB_CFG_LIMIT or until the the last JD.JCFG data has been transferred.
                                                         If the CFG-DMA engine has already transferred part but not all of the JD.JCFG section for
                                                         a job, then it will continue transferring data until it reaches the end of the JD.JCFG
                                                         section, or until it writes
                                                         MDAB_CFG_LIMIT for the LAST CFG-DMA slice, whichever comes first. This bit is reset to
                                                         zero when the
                                                         CFG-DMA engine has completed the transfer and is ready to be re-initialized for another
                                                         transfer. */
#else
	uint64_t start_busy                   : 1;
	uint64_t post_reset                   : 1;
	uint64_t dma_pending                  : 1;
	uint64_t last_dma_done                : 1;
	uint64_t dma_error                    : 1;
	uint64_t dma_oflow                    : 1;
	uint64_t dma_uflow                    : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t job_slot                     : 1;
	uint64_t pending_slot                 : 1;
	uint64_t reserved_10_15               : 6;
	uint64_t job_tag                      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_cfg_status_s        cnf75xx;
};
typedef union cvmx_mdabx_cfg_status cvmx_mdabx_cfg_status_t;

/**
 * cvmx_mdab#_dac_bist_status
 *
 * This register contains the dsp bist status.
 *
 */
union cvmx_mdabx_dac_bist_status {
	uint64_t u64;
	struct cvmx_mdabx_dac_bist_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t dmempar_bist                 : 1;  /**< DSP data memory parity bist status. */
	uint64_t tmem_bist                    : 1;  /**< DSP tmem bist status. */
	uint64_t dmt_bist                     : 1;  /**< DSP DMT bist status. */
	uint64_t imt_bist                     : 1;  /**< DSP IMT bist status. */
#else
	uint64_t imt_bist                     : 1;
	uint64_t dmt_bist                     : 1;
	uint64_t tmem_bist                    : 1;
	uint64_t dmempar_bist                 : 1;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_mdabx_dac_bist_status_s   cnf75xx;
};
typedef union cvmx_mdabx_dac_bist_status cvmx_mdabx_dac_bist_status_t;

/**
 * cvmx_mdab#_dac_eco
 */
union cvmx_mdabx_dac_eco {
	uint64_t u64;
	struct cvmx_mdabx_dac_eco_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eco_rw                       : 32; /**< N/A */
#else
	uint64_t eco_rw                       : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_dac_eco_s           cnf75xx;
};
typedef union cvmx_mdabx_dac_eco cvmx_mdabx_dac_eco_t;

/**
 * cvmx_mdab#_dac_membase
 *
 * This register provides a 1 MB aligned base address that is added to dac-gaa requests before
 * the address in system memory space is issued on the dac->gaa R/W request bus.
 */
union cvmx_mdabx_dac_membase {
	uint64_t u64;
	struct cvmx_mdabx_dac_membase_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t dac_membase                  : 20; /**< When the DSP issues a read or write request to system memory space on the dac->gaa i/f,
                                                         this
                                                         1 MB-aligned offset addr[39:20] is added to the DSP address to provide access to any
                                                         physical
                                                         address in the system memory space. */
#else
	uint64_t dac_membase                  : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_mdabx_dac_membase_s       cnf75xx;
};
typedef union cvmx_mdabx_dac_membase cvmx_mdabx_dac_membase_t;

/**
 * cvmx_mdab#_dac_timer
 *
 * This register contains a local timer that is incremented every tick_div cycles.
 *
 */
union cvmx_mdabx_dac_timer {
	uint64_t u64;
	struct cvmx_mdabx_dac_timer_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t tick_div                     : 8;  /**< Timer divider value.  TICK_DIV == 0 results in a divider value of 256. */
	uint64_t reserved_16_23               : 8;
	uint64_t tick                         : 16; /**< Local timer register.  Increments every tick_div+1 cycles. */
#else
	uint64_t tick                         : 16;
	uint64_t reserved_16_23               : 8;
	uint64_t tick_div                     : 8;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_dac_timer_s         cnf75xx;
};
typedef union cvmx_mdabx_dac_timer cvmx_mdabx_dac_timer_t;

/**
 * cvmx_mdab#_dmem_array#
 *
 * This address range provides cnMIPS cores access to any MDAB data memory array. Intended for
 * debug purposes only since RSL accesses are slow.
 *
 * When generating RSL addresses software should use the following equation:
 *   rsl_byte_address = 0x11800_B400_0000 + (MDAB-ID[4:0] << 19) + (MEMTYP << 18) +
 * (8B_OFFSET[14:0] << 3)
 *
 * where
 *
 * MEMTYP:
 * _  0 = IMEM
 * _  1 = DMEM
 *
 * MDAB-ID:
 * _ [0..13, 15..16] = cnMBP2[0..15]
 * _ [14, 17] = cnSBP2[0,1]
 *
 *     Software must also be sure to restrict the 8B_OFFSET (based on DSPTYPE/RAMTYPE) as
 * follows:
 *     <pre>
 *       DSPTYPE    RAMTYPE   8B_OFFSET[14:0] (range)
 *     ------------------------------------------
 *       cnMBP2       DMEM     [16K..(32K-1)]
 *       cnMBP2       IMEM     [0..(32K-1)]
 *       cnSBP2       DMEM     [0..(32K-1)]
 *       cnSBP2       IMEM     [0..(8K-1)]
 *
 *     </pre>
 */
union cvmx_mdabx_dmem_arrayx {
	uint64_t u64;
	struct cvmx_mdabx_dmem_arrayx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qword                        : 64; /**< Quad-word (8B) data from DSP data memory. */
#else
	uint64_t qword                        : 64;
#endif
	} s;
	struct cvmx_mdabx_dmem_arrayx_s       cnf75xx;
};
typedef union cvmx_mdabx_dmem_arrayx cvmx_mdabx_dmem_arrayx_t;

/**
 * cvmx_mdab#_error_address
 *
 * This register contains the address of the transaction that caused an error signal to be
 * asserted
 */
union cvmx_mdabx_error_address {
	uint64_t u64;
	struct cvmx_mdabx_error_address_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t addr                         : 32; /**< When an RSL access causes the ID_MEM_NXM error bit to be set, this field captures the
                                                         failing address. */
#else
	uint64_t addr                         : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_error_address_s     cnf75xx;
};
typedef union cvmx_mdabx_error_address cvmx_mdabx_error_address_t;

/**
 * cvmx_mdab#_error_status
 *
 * This register contains error status signals.
 *
 */
union cvmx_mdabx_error_status {
	uint64_t u64;
	struct cvmx_mdabx_error_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t error_hilo                   : 1;  /**< This field indicates the state of addr[3] of a CSR transaction that caused the error whose
                                                         state is recorded in
                                                         this register.  (CSR-WR: WBEs/CSR-RD:8B-aligned) */
	uint64_t error_flitsize               : 1;  /**< This field indicates the type of transaction that caused the recorded error. 0=16B, 1=32B */
	uint64_t error_type                   : 1;  /**< This field indicates the type of transaction that caused the recorded error. 0=non-csr, 1=csr. */
	uint64_t error_cmd                    : 1;  /**< This field indicates the command type of the transaction that caused the recorded error.
                                                         0=RD, 1=WR. */
	uint64_t error_src                    : 3;  /**< This field indicates the source of the transaction that caused the recorded error.
                                                         - 000: WDM
                                                         - 001: RDM
                                                         - 010: CDM
                                                         - 011: CSR
                                                         - 100: PFO Write Request Error */
	uint64_t mem_dataerr                  : 1;  /**< This bit will be set when a data error has occurred during a csr or dma engine operation */
	uint64_t mem_addrerr                  : 1;  /**< This bit will be set when a csr or dma engine access is performed to a non existent
                                                         instruction
                                                         or data memory address. */
#else
	uint64_t mem_addrerr                  : 1;
	uint64_t mem_dataerr                  : 1;
	uint64_t error_src                    : 3;
	uint64_t error_cmd                    : 1;
	uint64_t error_type                   : 1;
	uint64_t error_flitsize               : 1;
	uint64_t error_hilo                   : 1;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_mdabx_error_status_s      cnf75xx;
};
typedef union cvmx_mdabx_error_status cvmx_mdabx_error_status_t;

/**
 * cvmx_mdab#_gp0
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp0 {
	uint64_t u64;
	struct cvmx_mdabx_gp0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp0                          : 32; /**< Scratch register 0. */
#else
	uint64_t gp0                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp0_s               cnf75xx;
};
typedef union cvmx_mdabx_gp0 cvmx_mdabx_gp0_t;

/**
 * cvmx_mdab#_gp1
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp1 {
	uint64_t u64;
	struct cvmx_mdabx_gp1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp1                          : 32; /**< Scratch register 1. */
#else
	uint64_t gp1                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp1_s               cnf75xx;
};
typedef union cvmx_mdabx_gp1 cvmx_mdabx_gp1_t;

/**
 * cvmx_mdab#_gp2
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp2 {
	uint64_t u64;
	struct cvmx_mdabx_gp2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp2                          : 32; /**< Scratch register 2. */
#else
	uint64_t gp2                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp2_s               cnf75xx;
};
typedef union cvmx_mdabx_gp2 cvmx_mdabx_gp2_t;

/**
 * cvmx_mdab#_gp3
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp3 {
	uint64_t u64;
	struct cvmx_mdabx_gp3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp3                          : 32; /**< Scratch register 3. */
#else
	uint64_t gp3                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp3_s               cnf75xx;
};
typedef union cvmx_mdabx_gp3 cvmx_mdabx_gp3_t;

/**
 * cvmx_mdab#_gp4
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp4 {
	uint64_t u64;
	struct cvmx_mdabx_gp4_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp4                          : 32; /**< Scratch register 4. */
#else
	uint64_t gp4                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp4_s               cnf75xx;
};
typedef union cvmx_mdabx_gp4 cvmx_mdabx_gp4_t;

/**
 * cvmx_mdab#_gp5
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp5 {
	uint64_t u64;
	struct cvmx_mdabx_gp5_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp5                          : 32; /**< Scratch register 5. */
#else
	uint64_t gp5                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp5_s               cnf75xx;
};
typedef union cvmx_mdabx_gp5 cvmx_mdabx_gp5_t;

/**
 * cvmx_mdab#_gp6
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp6 {
	uint64_t u64;
	struct cvmx_mdabx_gp6_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp6                          : 32; /**< Scratch register 6. */
#else
	uint64_t gp6                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp6_s               cnf75xx;
};
typedef union cvmx_mdabx_gp6 cvmx_mdabx_gp6_t;

/**
 * cvmx_mdab#_gp7
 *
 * General purpose scratch register.
 *
 */
union cvmx_mdabx_gp7 {
	uint64_t u64;
	struct cvmx_mdabx_gp7_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gp7                          : 32; /**< Scratch register 7. */
#else
	uint64_t gp7                          : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gp7_s               cnf75xx;
};
typedef union cvmx_mdabx_gp7 cvmx_mdabx_gp7_t;

/**
 * cvmx_mdab#_gpio_in
 *
 * DSP GPIO input register, allows DSP to read data from other hosts.
 *
 */
union cvmx_mdabx_gpio_in {
	uint64_t u64;
	struct cvmx_mdabx_gpio_in_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gpio_in                      : 32; /**< DSP GPIO input register, allows DSP to read data from other hosts. */
#else
	uint64_t gpio_in                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gpio_in_s           cnf75xx;
};
typedef union cvmx_mdabx_gpio_in cvmx_mdabx_gpio_in_t;

/**
 * cvmx_mdab#_gpio_out
 *
 * DSP GPIO output register, allows DSP to output data for other hosts.
 *
 */
union cvmx_mdabx_gpio_out {
	uint64_t u64;
	struct cvmx_mdabx_gpio_out_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t gpio_out                     : 32; /**< DSP GPIO output register, allows DSP to output data for other hosts. */
#else
	uint64_t gpio_out                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_gpio_out_s          cnf75xx;
};
typedef union cvmx_mdabx_gpio_out cvmx_mdabx_gpio_out_t;

/**
 * cvmx_mdab#_id
 *
 * MDAB ID Register.
 *
 */
union cvmx_mdabx_id {
	uint64_t u64;
	struct cvmx_mdabx_id_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t mdb_id                       : 5;  /**< MDAB ID register. */
#else
	uint64_t mdb_id                       : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_mdabx_id_s                cnf75xx;
};
typedef union cvmx_mdabx_id cvmx_mdabx_id_t;

/**
 * cvmx_mdab#_imem_array#
 *
 * This address range provides cnMIPS cores access to any MDAB instruction memory array. Intended
 * for debug purposes only since RSL accesses are slow.
 *
 * When generating RSL addresses software should use the following equation:
 *   rsl_byte_address = 0x11800_B400_0000 + (MDAB-ID[4:0] << 19) + (MEMTYP << 18) +
 * (8B_OFFSET[14:0] << 3)
 *
 * where
 *
 * MEMTYP:
 * _ 0 = IMEM
 * _ 1 = DMEM
 *
 * MDAB-ID:
 *
 * _ [0..13, 15..16] = cnMBP2[0..15]
 * _ [14, 17] = cnSBP2[0,1]
 *
 * Software must also be sure to restrict the 8B_OFFSET (based on DSPTYPE/RAMTYPE) as
 * follows:
 *     <pre>
 *       DSPTYPE    RAMTYPE   8B_OFFSET[14:0] (range)
 *     ------------------------------------------
 *       cnMBP2       DMEM     [16K..(32K-1)]
 *       cnMBP2       IMEM     [0..(32K-1)]
 *       cnSBP2       DMEM     [0..(32K-1)]
 *       cnSBP2       IMEM     [0..(8K-1)]
 *
 *     </pre>
 */
union cvmx_mdabx_imem_arrayx {
	uint64_t u64;
	struct cvmx_mdabx_imem_arrayx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t qword                        : 64; /**< Quad-word (8B) data from DSP data memory. */
#else
	uint64_t qword                        : 64;
#endif
	} s;
	struct cvmx_mdabx_imem_arrayx_s       cnf75xx;
};
typedef union cvmx_mdabx_imem_arrayx cvmx_mdabx_imem_arrayx_t;

/**
 * cvmx_mdab#_int_ena_w1c
 *
 * This register is used to clear MDAB progammable interrupt enables for the MDAB_INT_SUM bits.
 *
 */
union cvmx_mdabx_int_ena_w1c {
	uint64_t u64;
	struct cvmx_mdabx_int_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t job_rdy1                     : 1;  /**< Reads or clears the enable for the MDABx_INT_SUM[JOB_RDY1] interrupt bit. */
	uint64_t job_rdy0                     : 1;  /**< Reads or clears the enable for the MDABx_INT_SUM[JOB_RDY0] interrupt bit. */
	uint64_t wr_dma_uflow                 : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[WR_DMA_UFLOW]. */
	uint64_t wr_dma_oflow                 : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[WR_DMA_OFLOW]. */
	uint64_t wr_dma_last_done             : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[WR_DMA_LAST_DONE]. */
	uint64_t wr_dma_slice_done            : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[WR_DMA_SLICE_DONE]. */
	uint64_t wr_dma_pending               : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[WR_DMA_PENDING]. */
	uint64_t rd_dma_last_done             : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[RD_DMA_LAST_DONE]. */
	uint64_t rd_dma_slice_done            : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[RD_DMA_SLICE_DONE]. */
	uint64_t rd_dma_pending               : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[RD_DMA_PENDING]. */
	uint64_t cfg_dma_last_done            : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[CFG_DMA_LAST_DONE]. */
	uint64_t cfg_dma_slice_done           : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[CFG_DMA_SLICE_DONE]. */
	uint64_t cfg_dma_pending              : 1;  /**< Reads or clears enables for the MDABx_INT_SUM[CFG_DMA_PENDING]. */
#else
	uint64_t cfg_dma_pending              : 1;
	uint64_t cfg_dma_slice_done           : 1;
	uint64_t cfg_dma_last_done            : 1;
	uint64_t rd_dma_pending               : 1;
	uint64_t rd_dma_slice_done            : 1;
	uint64_t rd_dma_last_done             : 1;
	uint64_t wr_dma_pending               : 1;
	uint64_t wr_dma_slice_done            : 1;
	uint64_t wr_dma_last_done             : 1;
	uint64_t wr_dma_oflow                 : 1;
	uint64_t wr_dma_uflow                 : 1;
	uint64_t job_rdy0                     : 1;
	uint64_t job_rdy1                     : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mdabx_int_ena_w1c_s       cnf75xx;
};
typedef union cvmx_mdabx_int_ena_w1c cvmx_mdabx_int_ena_w1c_t;

/**
 * cvmx_mdab#_int_ena_w1s
 *
 * This register is used to set MDAB progammable interrupt enables for the MDAB_INT_SUM bits.
 *
 */
union cvmx_mdabx_int_ena_w1s {
	uint64_t u64;
	struct cvmx_mdabx_int_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t job_rdy1                     : 1;  /**< Reads or sets the enable for the MDABx_INT_SUM[JOB_RDY1] interrupt bit. */
	uint64_t job_rdy0                     : 1;  /**< Reads or sets the enable for the MDABx_INT_SUM[JOB_RDY0] interrupt bit. */
	uint64_t wr_dma_uflow                 : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[WR_DMA_UFLOW]. */
	uint64_t wr_dma_oflow                 : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[WR_DMA_OFLOW]. */
	uint64_t wr_dma_last_done             : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[WR_DMA_LAST_DONE]. Note that this interrupt
                                                         bit will not get
                                                         set at the end of a store local operation. */
	uint64_t wr_dma_slice_done            : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[WR_DMA_SLICE_DONE]. Note that this interrupt
                                                         bit will not get
                                                         set at the end of a store local operation. */
	uint64_t wr_dma_pending               : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[WR_DMA_PENDING]. */
	uint64_t rd_dma_last_done             : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[RD_DMA_LAST_DONE].  Note that this interrupt
                                                         bit will not get
                                                         set at the end of a load local operation. */
	uint64_t rd_dma_slice_done            : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[RD_DMA_SLICE_DONE].  Note that this interrupt
                                                         bit will not get
                                                         set at the end of a load local operation. */
	uint64_t rd_dma_pending               : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[RD_DMA_PENDING]. */
	uint64_t cfg_dma_last_done            : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[CFG_DMA_LAST_DONE]. */
	uint64_t cfg_dma_slice_done           : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[CFG_DMA_SLICE_DONE]. */
	uint64_t cfg_dma_pending              : 1;  /**< Reads or sets enables for the MDABx_INT_SUM[CFG_DMA_PENDING]. */
#else
	uint64_t cfg_dma_pending              : 1;
	uint64_t cfg_dma_slice_done           : 1;
	uint64_t cfg_dma_last_done            : 1;
	uint64_t rd_dma_pending               : 1;
	uint64_t rd_dma_slice_done            : 1;
	uint64_t rd_dma_last_done             : 1;
	uint64_t wr_dma_pending               : 1;
	uint64_t wr_dma_slice_done            : 1;
	uint64_t wr_dma_last_done             : 1;
	uint64_t wr_dma_oflow                 : 1;
	uint64_t wr_dma_uflow                 : 1;
	uint64_t job_rdy0                     : 1;
	uint64_t job_rdy1                     : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mdabx_int_ena_w1s_s       cnf75xx;
};
typedef union cvmx_mdabx_int_ena_w1s cvmx_mdabx_int_ena_w1s_t;

/**
 * cvmx_mdab#_int_sel
 *
 * This register is used to configure the MDAB interrupts to the DSP cores.
 *
 */
union cvmx_mdabx_int_sel {
	uint64_t u64;
	struct cvmx_mdabx_int_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t ld_lcl_trig_style            : 1;  /**< Determines whether load local start/finish interrupts are level or
                                                         edge triggered:
                                                         0 = Level-sensitive.
                                                         1 = Edge-triggered.
                                                         NOTE: To ensure correct generation of outstanding interrupts, this bit must be programmed
                                                         prior
                                                         to initial configuration of the interrupt enable register. */
	uint64_t dma_int_trig_style           : 1;  /**< Determines whether interrupts reported via MDAB()_INT_SUM are level or
                                                         edge triggered:
                                                         0 = Level-sensitive.
                                                         1 = Edge-triggered.
                                                         NOTE: To ensure correct generation of outstanding interrupts, this bit must be programmed
                                                         prior
                                                         to initial configuration of the interrupt enable register. */
	uint64_t int_level_sel                : 1;  /**< Selects interrupt priority for MDAB()_INT_SUM interrupts.
                                                         0 = Level 1 interrupts.
                                                         1 = Level 2 interrupts.
                                                         NOTE: To ensure correct generation of outstanding interrupts, this bit must be programmed
                                                         prior
                                                         to initial configuration of the interrupt enable register. */
#else
	uint64_t int_level_sel                : 1;
	uint64_t dma_int_trig_style           : 1;
	uint64_t ld_lcl_trig_style            : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_mdabx_int_sel_s           cnf75xx;
};
typedef union cvmx_mdabx_int_sel cvmx_mdabx_int_sel_t;

/**
 * cvmx_mdab#_int_src
 *
 * This register allows cnMIPS software or DSP software to trigger a software
 * generated interrupt.
 */
union cvmx_mdabx_int_src {
	uint64_t u64;
	struct cvmx_mdabx_int_src_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t sw_intr                      : 1;  /**< Generates a software interrupt */
#else
	uint64_t sw_intr                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_mdabx_int_src_s           cnf75xx;
};
typedef union cvmx_mdabx_int_src cvmx_mdabx_int_src_t;

/**
 * cvmx_mdab#_int_sum_w1c
 *
 * This register reports the interrupt status for the MDAB DMA engines.
 * Software clears individual interrupts by writing one to the corresponding bit.
 */
union cvmx_mdabx_int_sum_w1c {
	uint64_t u64;
	struct cvmx_mdabx_int_sum_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t job_rdy1                     : 1;  /**< Indicates a job is ready in slot 1. Writing a 1 to this bit clears the interrupt. */
	uint64_t job_rdy0                     : 1;  /**< Indicates a job is ready in slot 0. Writing a 1 to this bit clears the interrupt. */
	uint64_t wr_dma_uflow                 : 1;  /**< Output data DMA underflow. When set, indicates that the MDAB_WR_LIMIT
                                                         specified a DMA transfer that was smaller thant the amount of output
                                                         data described by the job descriptor and the DSP had data remaining
                                                         in the buffer when the transaction completed. */
	uint64_t wr_dma_oflow                 : 1;  /**< Output data DMA overflow. When set, indicates that the MDAB_WR_LIMIT
                                                         specified a DMA transfer that exceeded the amount of output data
                                                         described by the job descriptor and the DMA engine was unable to
                                                         transfer all of the data between MDAB_WR_ADDR and
                                                         MDAB_WR_ADDR+MDAB_WR_LIMIT. */
	uint64_t wr_dma_last_done             : 1;  /**< When set indicates that the WR-DMA engine completed transferring all
                                                         output data described by the job descriptor (JD.WR-DMA). */
	uint64_t wr_dma_slice_done            : 1;  /**< Output data WR-DMA slice done. When set, indicates that the WR-DMA
                                                         engine completed an output data transfer but reached MDAB_WR_LIMIT
                                                         before transferring all of the output data described by the job descriptor. */
	uint64_t wr_dma_pending               : 1;  /**< Output data WR-DMA pending. When set, software needs to program the
                                                         output data WR-DMA engine to transfer the output data for a new job. */
	uint64_t rd_dma_last_done             : 1;  /**< When set indicates that the input data RD-DMA engine completed
                                                         transferring all input data for a job. */
	uint64_t rd_dma_slice_done            : 1;  /**< RD-DMA slice done. When set, indicates that the RD-DMA engine
                                                         completed an input data transfer but reached MDAB_RD_LIMIT before
                                                         transferring all of the input data for the job. */
	uint64_t rd_dma_pending               : 1;  /**< RD-DMA pending. When set, software needs to program the RD-DMA engine
                                                         to transfer the input data for a new job. */
	uint64_t cfg_dma_last_done            : 1;  /**< CFG-DMA last done detected: When set indicates that the CFG-DMA
                                                         engine detected the last data transfer for the entire job
                                                         configuration data (JD.JCFG) for a job. */
	uint64_t cfg_dma_slice_done           : 1;  /**< CFG-DMA slice done. When set, indicates that the CFG-DMA engine
                                                         completed a job configuration data slice transfer and reached the
                                                         MDAB_CFG_LIMIT before transferring the last data from job configuration
                                                         data for the job (JD.JCFG). */
	uint64_t cfg_dma_pending              : 1;  /**< CFG-DMA pending. When set, software needs to program the CFG-DMA
                                                         engine to transfer the job configuration data for a new job. */
#else
	uint64_t cfg_dma_pending              : 1;
	uint64_t cfg_dma_slice_done           : 1;
	uint64_t cfg_dma_last_done            : 1;
	uint64_t rd_dma_pending               : 1;
	uint64_t rd_dma_slice_done            : 1;
	uint64_t rd_dma_last_done             : 1;
	uint64_t wr_dma_pending               : 1;
	uint64_t wr_dma_slice_done            : 1;
	uint64_t wr_dma_last_done             : 1;
	uint64_t wr_dma_oflow                 : 1;
	uint64_t wr_dma_uflow                 : 1;
	uint64_t job_rdy0                     : 1;
	uint64_t job_rdy1                     : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mdabx_int_sum_w1c_s       cnf75xx;
};
typedef union cvmx_mdabx_int_sum_w1c cvmx_mdabx_int_sum_w1c_t;

/**
 * cvmx_mdab#_int_sum_w1s
 *
 * This register is used to artificially set MDAB interrupts described in
 * MDAB_INT_SUM (for software debug/test), and also provides the hardware input to allow
 * the DMA controllers to set the appropriate interrupt bit.
 */
union cvmx_mdabx_int_sum_w1s {
	uint64_t u64;
	struct cvmx_mdabx_int_sum_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t job_rdy1                     : 1;  /**< Reads or sets the MDABx_INT_SUM[JOB_RDY1] interrupt bit. */
	uint64_t job_rdy0                     : 1;  /**< Reads or sets the MDABx_INT_SUM[JOB_RDY0] interrupt bit. */
	uint64_t wr_dma_uflow                 : 1;  /**< Reads or sets the MDABx_INT_SUM[WR_DMA_UFLOW]. */
	uint64_t wr_dma_oflow                 : 1;  /**< Reads or sets the MDABx_INT_SUM[WR_DMA_OFLOW]. */
	uint64_t wr_dma_last_done             : 1;  /**< Reads or sets the MDABx_INT_SUM[WR_DMA_LAST_DONE]. */
	uint64_t wr_dma_slice_done            : 1;  /**< Reads or sets the MDABx_INT_SUM[WR_DMA_SLICE_DONE]. */
	uint64_t wr_dma_pending               : 1;  /**< Reads or sets the MDABx_INT_SUM[WR_DMA_PENDING]. */
	uint64_t rd_dma_last_done             : 1;  /**< Reads or sets the MDABx_INT_SUM[RD_DMA_LAST_DONE]. */
	uint64_t rd_dma_slice_done            : 1;  /**< Reads or sets the MDABx_INT_SUM[RD_DMA_SLICE_DONE]. */
	uint64_t rd_dma_pending               : 1;  /**< Reads or sets the MDABx_INT_SUM[RD_DMA_PENDING]. */
	uint64_t cfg_dma_last_done            : 1;  /**< Reads or sets the MDABx_INT_SUM[CFG_DMA_LAST_DONE]. */
	uint64_t cfg_dma_slice_done           : 1;  /**< Reads or sets the MDABx_INT_SUM[CFG_DMA_SLICE_DONE]. */
	uint64_t cfg_dma_pending              : 1;  /**< Reads or sets the MDABx_INT_SUM[CFG_DMA_PENDING]. */
#else
	uint64_t cfg_dma_pending              : 1;
	uint64_t cfg_dma_slice_done           : 1;
	uint64_t cfg_dma_last_done            : 1;
	uint64_t rd_dma_pending               : 1;
	uint64_t rd_dma_slice_done            : 1;
	uint64_t rd_dma_last_done             : 1;
	uint64_t wr_dma_pending               : 1;
	uint64_t wr_dma_slice_done            : 1;
	uint64_t wr_dma_last_done             : 1;
	uint64_t wr_dma_oflow                 : 1;
	uint64_t wr_dma_uflow                 : 1;
	uint64_t job_rdy0                     : 1;
	uint64_t job_rdy1                     : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mdabx_int_sum_w1s_s       cnf75xx;
};
typedef union cvmx_mdabx_int_sum_w1s cvmx_mdabx_int_sum_w1s_t;

/**
 * cvmx_mdab#_interrupt_active
 *
 * This register indicates what interrupts are enabled and active.
 *
 */
union cvmx_mdabx_interrupt_active {
	uint64_t u64;
	struct cvmx_mdabx_interrupt_active_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t job_rdy1                     : 1;  /**< Indicates the JOB_RDY1 interrupt is active. */
	uint64_t job_rdy0                     : 1;  /**< Indicates the JOB_RDY0 interrupt is active. */
	uint64_t wr_dma_uflow                 : 1;  /**< Indicates the WR_DMA_UFLOW interrupt is active. */
	uint64_t wr_dma_oflow                 : 1;  /**< Indicates the WR_DMA_OFLOW interrupt is active. */
	uint64_t wr_dma_last_done             : 1;  /**< Indicates the WR_DMA_LAST_DONE interrupt is active. */
	uint64_t wr_dma_slice_done            : 1;  /**< Indicates the WR_DMA_SLICE_DONE interrupt is active. */
	uint64_t wr_dma_pending               : 1;  /**< Indicates the WR_DMA_PENDING interrupt is active. */
	uint64_t rd_dma_last_done             : 1;  /**< Indicates the RD_DMA_LAST_DONE interrupt is active. */
	uint64_t rd_dma_slice_done            : 1;  /**< Indicates the RD_DMA_SLICE_DONE interrupt is active. */
	uint64_t rd_dma_pending               : 1;  /**< Indicates the RD_DMA_PENDING interrupt is active. */
	uint64_t cfg_dma_last_done            : 1;  /**< Indicates the CFG_DMA_LAST_DONE interrupt is active. */
	uint64_t cfg_dma_slice_done           : 1;  /**< Indicates the CFG_DMA_SLICE_DONE interrupt is active. */
	uint64_t cfg_dma_pending              : 1;  /**< Indicates the CFG_DMA_PENDING interrupt is active. */
#else
	uint64_t cfg_dma_pending              : 1;
	uint64_t cfg_dma_slice_done           : 1;
	uint64_t cfg_dma_last_done            : 1;
	uint64_t rd_dma_pending               : 1;
	uint64_t rd_dma_slice_done            : 1;
	uint64_t rd_dma_last_done             : 1;
	uint64_t wr_dma_pending               : 1;
	uint64_t wr_dma_slice_done            : 1;
	uint64_t wr_dma_last_done             : 1;
	uint64_t wr_dma_oflow                 : 1;
	uint64_t wr_dma_uflow                 : 1;
	uint64_t job_rdy0                     : 1;
	uint64_t job_rdy1                     : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mdabx_interrupt_active_s  cnf75xx;
};
typedef union cvmx_mdabx_interrupt_active cvmx_mdabx_interrupt_active_t;

/**
 * cvmx_mdab#_job_status#
 *
 * Reports the status for the job in each slot. When a job is received from the PSM, its status
 * will always be reflected in one of these two status registers until the job completes. The job
 * will not complete until
 * bits 0..5 have all been set.
 */
union cvmx_mdabx_job_statusx {
	uint64_t u64;
	struct cvmx_mdabx_job_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t job_tag                      : 16; /**< The JOB_TAG field from the PSM command that created the job.
                                                         Since MDAB hardware updates this register, its contents are unpredictable in software. */
	uint64_t job_type                     : 2;  /**< The JOB_TYPE field from the PSM command that created the job.
                                                             [ 0: NML OP / 1: ST_LCL / 2: LD_LCL (w/o RST) / 3: LD_LCL (w/ RST) ]
                                                         Since MDAB hardware updates this register, its contents are unpredictable in software. */
	uint64_t reserved_6_13                : 8;
	uint64_t wr_dma_done                  : 1;  /**< Set when hardware completes all WR-DMA transfers and all writes have been committed to
                                                         SMEM or L2C/DDR memory.
                                                         Since MDAB hardware updates this register, its contents are unpredictable in software. */
	uint64_t rd_dma_done                  : 1;  /**< Set when hardware completes all RD-DMA transfers and the data has been committed to local
                                                         DSP I/D memory.
                                                         Since MDAB hardware updates this register, its contents are unpredictable in software. */
	uint64_t cfg_dma_done                 : 1;  /**< Set when hardware completes all CFG-DMA transfers for the job configuration and the
                                                         configuration has been committed to local DSP I/D memory. */
	uint64_t sw_done                      : 1;  /**< Software must set this bit to signal when it has finished all processing for a job. */
	uint64_t sw_started                   : 1;  /**< Software can set this bit to record when it began processing the job. The time when this
                                                         bit is set will be used by the job watchdog timer and will be used to determine the job
                                                         runtime
                                                         recorded in the job log. */
	uint64_t valid                        : 1;  /**< Set to one when the status reflects a valid job has been received from the PSM. Set to
                                                         zero
                                                         when there is no job in the given slot. */
#else
	uint64_t valid                        : 1;
	uint64_t sw_started                   : 1;
	uint64_t sw_done                      : 1;
	uint64_t cfg_dma_done                 : 1;
	uint64_t rd_dma_done                  : 1;
	uint64_t wr_dma_done                  : 1;
	uint64_t reserved_6_13                : 8;
	uint64_t job_type                     : 2;
	uint64_t job_tag                      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_job_statusx_s       cnf75xx;
};
typedef union cvmx_mdabx_job_statusx cvmx_mdabx_job_statusx_t;

/**
 * cvmx_mdab#_job_status1#
 *
 * Additional job status information for software.
 *
 */
union cvmx_mdabx_job_status1x {
	uint64_t u64;
	struct cvmx_mdabx_job_status1x_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t cfg_cnt                      : 9;  /**< Contains the total CFG-DMA transfer count in 128B chunks. */
#else
	uint64_t cfg_cnt                      : 9;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_mdabx_job_status1x_s      cnf75xx;
};
typedef union cvmx_mdabx_job_status1x cvmx_mdabx_job_status1x_t;

/**
 * cvmx_mdab#_ld_int_ena_w1c
 *
 * This is the INT_ENA clear register for the load local start and done interrupts.
 *
 */
union cvmx_mdabx_ld_int_ena_w1c {
	uint64_t u64;
	struct cvmx_mdabx_ld_int_ena_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t ld_lcl_done_int              : 1;  /**< Writing a 1 to this bit disables the load local done interrupt. */
	uint64_t ld_lcl_start_int             : 1;  /**< Writing a 1 to this bit disables the load local start interrupt. */
#else
	uint64_t ld_lcl_start_int             : 1;
	uint64_t ld_lcl_done_int              : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_ld_int_ena_w1c_s    cnf75xx;
};
typedef union cvmx_mdabx_ld_int_ena_w1c cvmx_mdabx_ld_int_ena_w1c_t;

/**
 * cvmx_mdab#_ld_int_ena_w1s
 *
 * This is the INT_ENA set register for the load local start and done interrupts.
 *
 */
union cvmx_mdabx_ld_int_ena_w1s {
	uint64_t u64;
	struct cvmx_mdabx_ld_int_ena_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t ld_lcl_done_int              : 1;  /**< Writing a 1 to this bit enables the load local done interrupt. */
	uint64_t ld_lcl_start_int             : 1;  /**< Writing a 1 to this bit enables the load local start interrupt. */
#else
	uint64_t ld_lcl_start_int             : 1;
	uint64_t ld_lcl_done_int              : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_ld_int_ena_w1s_s    cnf75xx;
};
typedef union cvmx_mdabx_ld_int_ena_w1s cvmx_mdabx_ld_int_ena_w1s_t;

/**
 * cvmx_mdab#_ld_int_sum_w1c
 *
 * This is the INT_SUM clear register for the load local start and done interrupts.
 *
 */
union cvmx_mdabx_ld_int_sum_w1c {
	uint64_t u64;
	struct cvmx_mdabx_ld_int_sum_w1c_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t ld_lcl_done_int              : 1;  /**< Writing a 1 to this bit clears the INT_SUM bit for the load local done interrupt. */
	uint64_t ld_lcl_start_int             : 1;  /**< Writing a 1 to this bit clears the INT_SUM bit for the load local start interrupt. */
#else
	uint64_t ld_lcl_start_int             : 1;
	uint64_t ld_lcl_done_int              : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_ld_int_sum_w1c_s    cnf75xx;
};
typedef union cvmx_mdabx_ld_int_sum_w1c cvmx_mdabx_ld_int_sum_w1c_t;

/**
 * cvmx_mdab#_ld_int_sum_w1s
 *
 * This is the INT_SUM set register for the load local start and done interrupts.
 *
 */
union cvmx_mdabx_ld_int_sum_w1s {
	uint64_t u64;
	struct cvmx_mdabx_ld_int_sum_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t ld_lcl_done_int              : 1;  /**< This bit is set by the read dma load local logic when a load_local_without_reset slice
                                                         completes.  Writing a 1 to this bit sets the INT_SUM bit for the load local done
                                                         interrupt.
                                                         This allows software to generate the interrupt if the corresponding enable bit is set. */
	uint64_t ld_lcl_start_int             : 1;  /**< This bit is set by the read dma load local logic when a load_local_without_reset slice
                                                         operation
                                                         starts.  Writing a 1 to this bit sets the INT_SUM bit for the load local start interrupt.
                                                         This allows software to generate the interrupt if the corresponding enable bit is set. */
#else
	uint64_t ld_lcl_start_int             : 1;
	uint64_t ld_lcl_done_int              : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_ld_int_sum_w1s_s    cnf75xx;
};
typedef union cvmx_mdabx_ld_int_sum_w1s cvmx_mdabx_ld_int_sum_w1s_t;

/**
 * cvmx_mdab#_pfio_ctl
 *
 * PFI and PFO control register
 *
 */
union cvmx_mdabx_pfio_ctl {
	uint64_t u64;
	struct cvmx_mdabx_pfio_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_10_63               : 54;
	uint64_t pfo_rd_dswap                 : 3;  /**< Determines the swap data field on pfo read requests to system memory (not SMEM).
                                                         0 = no swapping.
                                                         1 = byte swapping (0x0123456789abcdef -> 0xefcdab8967452301).
                                                         2 = 16-bit swapping (0x0123456789abcdef -> 0xcdef89ab45670123).
                                                         3 = 32-bit swapping (0x0123456789abcdef -> 0x89abcdef01234567).
                                                         4-7 = Reserved. */
	uint64_t pfo_wr_dswap                 : 3;  /**< Determines the swap data field on pfo write requests to system memory (not SMEM).
                                                         0 = no swapping/
                                                         1 = byte swapping (0x0123456789abcdef -> 0xefcdab8967452301)/
                                                         2 = 16-bit swapping (0x0123456789abcdef -> 0xcdef89ab45670123)/
                                                         3 = 32-bit swapping (0x0123456789abcdef -> 0x89abcdef01234567)/
                                                         4-7 = Reserved */
	uint64_t pfo_rsp_arbmode              : 1;  /**< Determines the PFO-RSP arbitration policy for the several
                                                         different RSP types which can be serviced/returned to the DSP core ?out of order?.
                                                         When set to one, the PFO-RSP arbitration will be round robin amongst the several
                                                         pending PFO-RSP types = [WR-RSP, RD-RSP, ILL-RSP].  When set to zero, the PFO-RSP
                                                         arbitration uses a two stage arbitration where the first stage uses round robin
                                                         amongst the pending WR-RSPs and RD-RSPs.  The second stage uses fixed priority, with
                                                         ILL-RSPs(illegal responses) having higher Priority over the RD|WR-RSPs.
                                                         NOTE: ILL-RSP = ?Illegal responses? and include:
                                                         1)      For non-existent memory (NXM): DSP will receive RSP=AErrL.
                                                         2)      For illegal Command (ILL-CMD): DSP will receive RSP=ADErrL. */
	uint64_t req_priority                 : 2;  /**< Programmable control of the PFI-REQ Priority field for incoming requests. */
	uint64_t pfi_req_arbmode              : 1;  /**< Determines the PFI-REQ arbitration policy for the several different inbound PIF request
                                                         types which are serviced/returned by the DSP in strict order.
                                                         When set to one, the PFO-REQ arbitration will be round robin amongst the several
                                                         pending PFI-REQ types = [CSR(I/D MEM), CFG-DMA, RD-DMA, WR-DMA].
                                                         When set to zero, the PFI-REQ arbitration uses a fixed priority arbitration scheme:
                                                         [CSR(I/D MEM)=LP, CFG-DMA, RD-DMA, WR-DMA=HP]. */
#else
	uint64_t pfi_req_arbmode              : 1;
	uint64_t req_priority                 : 2;
	uint64_t pfo_rsp_arbmode              : 1;
	uint64_t pfo_wr_dswap                 : 3;
	uint64_t pfo_rd_dswap                 : 3;
	uint64_t reserved_10_63               : 54;
#endif
	} s;
	struct cvmx_mdabx_pfio_ctl_s          cnf75xx;
};
typedef union cvmx_mdabx_pfio_ctl cvmx_mdabx_pfio_ctl_t;

/**
 * cvmx_mdab#_proc_ctl
 *
 * This register contains controls for certain DSP operating modes
 *
 */
union cvmx_mdabx_proc_ctl {
	uint64_t u64;
	struct cvmx_mdabx_proc_ctl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t cya_ld_lcl_rst_rs_clr        : 1;  /**< N/A */
	uint64_t stlcl_stall                  : 1;  /**< Local stall bit.  During a ST_LCL transaction, if this
                                                         bit is set, the DAC hardware will assert the DSP RunStall input. If the bit
                                                         is CLEAR, the DAC hardware will not assert the DSP RunStall input. */
	uint64_t ldlcl_stall                  : 1;  /**< Local stall bit.  During a LD_LCL (without reset) transaction, if this
                                                         bit is set, the DAC hardware will assert the DSP RunStall input. If the bit
                                                         is CLEAR, the DAC hardware will not assert the DSP RunStall input.
                                                         NOTE: */
	uint64_t breset                       : 1;  /**< When software sets this bit the entire dsp will be forced in the reset state. When the
                                                         software clears this bit, the dsp will come out of reset and start execution at the reset
                                                         interrupt vector (if run_stall is not asserted).  This bit is cleared at the start of
                                                         a load local with reset operation so that the dsp reset input can be correctly controlled
                                                         by the load local with reset logic. */
	uint64_t ocd_halt_on_reset            : 1;  /**< Force the DSP to enter OCDHaltMode after reset. */
	uint64_t dreset                       : 1;  /**< Setting this bit asserts reset to the DSP debug logic. */
	uint64_t stat_vector_sel              : 1;  /**< Setting this bit selects the alternative stationary vector base address. */
	uint64_t run_stall                    : 1;  /**< When this bit is set the DSP's RUN_STALL input will be asserted, forcing the DSP to
                                                         "stall" until the bit is cleared.  This bit is cleared at the start of a load local
                                                         with reset operation to enable the load local logic to control the dsp RunStall input.
                                                         At the end of the load local with reset operation when the last block of instruction/
                                                         data has been loaded into the dsp memory, the dsp RunStall input will be deasserted
                                                         so the dsp can start running. */
#else
	uint64_t run_stall                    : 1;
	uint64_t stat_vector_sel              : 1;
	uint64_t dreset                       : 1;
	uint64_t ocd_halt_on_reset            : 1;
	uint64_t breset                       : 1;
	uint64_t ldlcl_stall                  : 1;
	uint64_t stlcl_stall                  : 1;
	uint64_t cya_ld_lcl_rst_rs_clr        : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_mdabx_proc_ctl_s          cnf75xx;
};
typedef union cvmx_mdabx_proc_ctl cvmx_mdabx_proc_ctl_t;

/**
 * cvmx_mdab#_proc_debug
 *
 * This register contains iram data store and dram conditional store status bits.
 *
 */
union cvmx_mdabx_proc_debug {
	uint64_t u64;
	struct cvmx_mdabx_proc_debug_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t iram1loadstore               : 1;  /**< Data load/store to iram 1. */
	uint64_t iram0loadstore               : 1;  /**< Data load/store to iram 0. */
	uint64_t dram1lockb1                  : 1;  /**< Conditional store has caused dram1 bank1 to be locked. */
	uint64_t dram1lockb0                  : 1;  /**< Conditional store has caused dram1 bank0 to be locked. */
	uint64_t dram0lockb1                  : 1;  /**< Conditional store has caused dram0 bank1 to be locked. */
	uint64_t dram0lockb0                  : 1;  /**< Conditional store has caused dram0 bank0 to be locked. */
#else
	uint64_t dram0lockb0                  : 1;
	uint64_t dram0lockb1                  : 1;
	uint64_t dram1lockb0                  : 1;
	uint64_t dram1lockb1                  : 1;
	uint64_t iram0loadstore               : 1;
	uint64_t iram1loadstore               : 1;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_mdabx_proc_debug_s        cnf75xx;
};
typedef union cvmx_mdabx_proc_debug cvmx_mdabx_proc_debug_t;

/**
 * cvmx_mdab#_proc_status
 *
 * This register indicates if the DSP is halted due to a halt-on-reset or waiti instruction
 *
 */
union cvmx_mdabx_proc_status {
	uint64_t u64;
	struct cvmx_mdabx_proc_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t xocd_state                   : 1;  /**< Indicates the DSP has entered the post reset OCD halt mode. */
	uint64_t pwait_state                  : 1;  /**< Indicates that the DSP is in the sleep mode as a result of executing the
                                                         WAITI instruction.  Any enabled interrupt will wake the DSP up. */
#else
	uint64_t pwait_state                  : 1;
	uint64_t xocd_state                   : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_proc_status_s       cnf75xx;
};
typedef union cvmx_mdabx_proc_status cvmx_mdabx_proc_status_t;

/**
 * cvmx_mdab#_psm_cmd#
 *
 * These four 32b registers comprise a full 128b job command which allows DSP software to spawn
 * new
 * 128-bit job command to the PHY
 * scheduler module(PSM) by:
 *    1) Writes to the 4x PSM_CMD(0..3) registers: (where the 128-bit job command =
 * MDABx_PSM_CMD[3:0]).
 *    2) Write to the MDAB_PSM_CMD_PUSH[PUSH_BUSY]=1 which will send a 128b job command to the
 * PSM.
 *    3) Wait(poll) until hardware clears the DAB_PSM_CMD_PUSH[PUSH_BUSY]=0 to ensure the job
 * command
 * has been
 *       fully transferred to the PSM before the DSP software can re-use the PSM_CMD[3:0]
 * registers.
 */
union cvmx_mdabx_psm_cmdx {
	uint64_t u64;
	struct cvmx_mdabx_psm_cmdx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t word                         : 32; /**< The full 128-bit job command is comprised of the WORD[3:0] from EACH 1(of 4) 32b CSRs =
                                                         [MDABx_PSM_CMD[3:0]]. */
#else
	uint64_t word                         : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_psm_cmdx_s          cnf75xx;
};
typedef union cvmx_mdabx_psm_cmdx cvmx_mdabx_psm_cmdx_t;

/**
 * cvmx_mdab#_psm_cmd_push
 *
 * This register causes the hardware to push a new job command currently in MDAB_PSM_CMD_(0..3)
 * to the PSM.
 */
union cvmx_mdabx_psm_cmd_push {
	uint64_t u64;
	struct cvmx_mdabx_psm_cmd_push_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t psm_idle                     : 1;  /**< When high, indicates that there are no outstanding psm commands in the jcmd logic. */
	uint64_t push_busy                    : 1;  /**< When software writes a one, hardware will start sending the PSM command stored in
                                                         MDABx_PSM_CMD(0..3) to the PSM. Hardware will clear this bit when the 128b job command has
                                                         been sent to the PSM, thus allowing software to reuse the MDABx_PSM_CMD(0..3) for the next
                                                         job command (if required). */
#else
	uint64_t push_busy                    : 1;
	uint64_t psm_idle                     : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mdabx_psm_cmd_push_s      cnf75xx;
};
typedef union cvmx_mdabx_psm_cmd_push cvmx_mdabx_psm_cmd_push_t;

/**
 * cvmx_mdab#_psm_timer
 *
 * This register contains the latest value of the PSM timer.
 *
 */
union cvmx_mdabx_psm_timer {
	uint64_t u64;
	struct cvmx_mdabx_psm_timer_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t frame                        : 12; /**< PSM timer frame field. */
	uint64_t sub_frame                    : 4;  /**< PSM timer sub_frame field. */
#else
	uint64_t sub_frame                    : 4;
	uint64_t frame                        : 12;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_mdabx_psm_timer_s         cnf75xx;
};
typedef union cvmx_mdabx_psm_timer cvmx_mdabx_psm_timer_t;

/**
 * cvmx_mdab#_rd_addr
 *
 * The starting address used by the RD-DMA engine when writing RD-DMA data to local DSP memory.
 * During Job_Type = LD_LCL commands, hardware loads the RD_ADDR/RD_LIMIT registers directly.
 */
union cvmx_mdabx_rd_addr {
	uint64_t u64;
	struct cvmx_mdabx_rd_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_31_63               : 33;
	uint64_t addr                         : 27; /**< The 128b-aligned starting address used by the RD-DMA engine when writing RD-DMA data to
                                                         local DSP memory.
                                                         When ADDR[30]=1(IMEM):
                                                            For MBP32 (256KB IMEM): Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4003.FFFF >> 4).
                                                            For SSP16 (64KB IMEM):  Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4000.FFFF >> 4).
                                                         When ADDR[30]=0(DMEM):
                                                            For MBP32 (128KB DMEM): Valid 128b-aligned address range = (0x3FFE.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                            For SSP16 (256KB DMEM): Valid 128b-aligned address range = (0x3FFC.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                         NOTE: In the I/D MEM address decoding scheme, ADDR[30] determines WHICH DSP Memory space
                                                         (0:DMEM/1:IMEM) */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t addr                         : 27;
	uint64_t reserved_31_63               : 33;
#endif
	} s;
	struct cvmx_mdabx_rd_addr_s           cnf75xx;
};
typedef union cvmx_mdabx_rd_addr cvmx_mdabx_rd_addr_t;

/**
 * cvmx_mdab#_rd_length
 *
 * The actual 128-bit length of the most recent RD-DMA engine job configuration transfer.
 *
 */
union cvmx_mdabx_rd_length {
	uint64_t u64;
	struct cvmx_mdabx_rd_length_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t length                       : 15; /**< The actual 128-bit length of the most recent RD-DMA engine read DMA transfer.
                                                         This field will be updated when the RD-DMA has been actually write committed to local DSP
                                                         memory,
                                                         which allows software to read(poll) the length field during the RD-DMA transfer to know
                                                         how many 128-bit chunks are now currently available to be read and used. */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t length                       : 15;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdabx_rd_length_s         cnf75xx;
};
typedef union cvmx_mdabx_rd_length cvmx_mdabx_rd_length_t;

/**
 * cvmx_mdab#_rd_limit
 *
 * The maximum number of 128-bit sized chunks that the RD-DMA engine may write starting from the
 * 128-bit-aligned MDAB_RD_ADDR. During Job_Type = LD_LCL* commands, hardware loads the
 * RD_ADDR/RD_LIMIT
 * registers directly from the Job Descriptor's DMA section(s).
 */
union cvmx_mdabx_rd_limit {
	uint64_t u64;
	struct cvmx_mdabx_rd_limit_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t last                         : 1;  /**< Indicates the last RD-DMA slice which may be written to address MDAB_RD_ADDR.
                                                         DSP software may choose to segment the JD.RD-DMA data into multiple non-contiguous RD-DMA
                                                         transfers to DSP local memory.
                                                         Eventually, DSP software will indicate the LAST segment (or slice), to indicate this is
                                                         the final RD-DMA sub-blk transfer.
                                                         Each RD-DMA operation's LAST will indicate when the last segment (or slice) RD-DMA
                                                         operation has been setup, to complete the JD.RD-DMA data transfer.
                                                         SW_NOTE: For every new RD-DMA slice operation, DSP software *MUST* signal LAST to indicate
                                                         the final JD.RD-DMA data slice transfer. */
	uint64_t reserved_19_30               : 12;
	uint64_t limit                        : 15; /**< "The maximum number of 128-bit chunks that the RD-DMA engine may write at address
                                                         MDAB_RD_ADDR.
                                                         The JD.JCFG data typically will contain the total RD-DMA length for each new job. The
                                                         DSP software is at liberty to break the total RD-DMA transfer into multiples RD-DMA
                                                         segments (or
                                                         slices) using the LIMIT register. When either LIMIT has been reached (or if the last
                                                         JD.RD-DMA data is detected), the RD-DMA is done.
                                                         For MBP DSP:
                                                            IMEM: 0x4000.0000 - 0x4003.FFFF
                                                            DMEM: 0x3FFE.0000 - 0x3FFF.FFFF
                                                         For SSP DSP:
                                                            IMEM: 0x4000.0000 - 0x4000.FFFF
                                                            DMEM: 0x3FFC.0000 - 0x3FFF.FFFF
                                                         Software restriction \#1: Software should never program LIMIT < 2. Indeterminate results
                                                         can/will occur.
                                                         Software restriction \#2: Software should ensure that ADDR+LIMIT SIZES fall within the I+D
                                                         MEM space, otherwise DMA_ERROR and indeterminate results can/will occur." */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t limit                        : 15;
	uint64_t reserved_19_30               : 12;
	uint64_t last                         : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_rd_limit_s          cnf75xx;
};
typedef union cvmx_mdabx_rd_limit cvmx_mdabx_rd_limit_t;

/**
 * cvmx_mdab#_rd_status
 *
 * Reports the status for the RD-DMA engine.
 *
 */
union cvmx_mdabx_rd_status {
	uint64_t u64;
	struct cvmx_mdabx_rd_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t job_tag                      : 16; /**< The JOB_TAG field from the PSM command for the last completed RD-DMA operation.
                                                         When DSP software writes [START_BUSY]=0->1, the RD-DMA engine will wait until the next
                                                         available job has been enqueued, at which point the [JOB_TAG] will be latched in. */
	uint64_t reserved_12_15               : 4;
	uint64_t post_ld_lcl                  : 1;  /**< Set when a load-local job has completed(last LD-LCL micro-op completes).
                                                         Cleared when a normal job has completed. */
	uint64_t ld_lcl_busy                  : 1;  /**< Set when a load-local operation is active. */
	uint64_t pending_slot                 : 1;  /**< The job slot for the pending RD-DMA operation. Valid only when [START_BUSY] == 0 and
                                                         [DMA_PENDING] == 1. */
	uint64_t job_slot                     : 1;  /**< The job slot for the most recent RD-DMA operation. */
	uint64_t reserved_7_7                 : 1;
	uint64_t dma_uflow                    : 1;  /**< Set when the last RD-DMA operation encountered an underflow */
	uint64_t dma_oflow                    : 1;  /**< Set when the last RD-DMA operation encountered an overflow */
	uint64_t dma_error                    : 1;  /**< Set when the last RD-DMA operation encountered an error. */
	uint64_t last_dma_done                : 1;  /**< Set when the last RD-DMA operation transferred the last word of the RD-DMA data from the
                                                         job descriptor (JD.RD-DMA). */
	uint64_t dma_pending                  : 1;  /**< Set when a RD-DMA operation is pending for the engine and the internal interface needs to
                                                         be programmed. */
	uint64_t post_reset                   : 1;  /**< Set to one on MDAB reset and remains one until the RD-DMA engine starts its first
                                                         RD-DMA transfer.
                                                         SWNOTE: DSP software can use this bit to qualify the [JOB_TAG], [JOB_SLOT], [DMA_ERROR],
                                                         [LAST_DMA_DONE]
                                                         bits in this CSR. [see: these status bits remain set and [POST_RESET] is used
                                                         by software to know when they are truly valid].
                                                         SET(hardware): MDAB reset
                                                         CLR(software): DSP writes START_BUSY=0->1 */
	uint64_t start_busy                   : 1;  /**< When software writes a one to this register, hardware will write the job descriptor's read
                                                         dma section (JD.RD-DMA) for the next available job into the local memory starting at
                                                         MDAB_RD_ADDR, up to MDAB_RD_ADDR+MDAB_RD_LIMIT or when the last 128-bit word of the JD.RD-
                                                         DMA section is
                                                         transferred (whichever comes first).
                                                         If the RD-DMA engine has already transferred part but not all of the JD.RD-DMA section for
                                                         a job, then it will continue transferring data until it reaches the end of the JD.RD-DMA
                                                         section, or until it
                                                         writes MDAB_CFG_LIMIT for the LAST RD-DMA slice, whichever comes first. This bit is reset
                                                         to zero when the
                                                         RD-DMA engine has completed the transfer and is ready to be re-initialized for another
                                                         transfer. */
#else
	uint64_t start_busy                   : 1;
	uint64_t post_reset                   : 1;
	uint64_t dma_pending                  : 1;
	uint64_t last_dma_done                : 1;
	uint64_t dma_error                    : 1;
	uint64_t dma_oflow                    : 1;
	uint64_t dma_uflow                    : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t job_slot                     : 1;
	uint64_t pending_slot                 : 1;
	uint64_t ld_lcl_busy                  : 1;
	uint64_t post_ld_lcl                  : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_tag                      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_rd_status_s         cnf75xx;
};
typedef union cvmx_mdabx_rd_status cvmx_mdabx_rd_status_t;

/**
 * cvmx_mdab#_wr_addr
 *
 * The starting address used by the WR-DMA engine when reading WR-DMA data from local DSP memory.
 * During Job_Type = ST_LCL* commands, hardware loads the WR_ADDR/WR_LIMIT registers directly.
 */
union cvmx_mdabx_wr_addr {
	uint64_t u64;
	struct cvmx_mdabx_wr_addr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_31_63               : 33;
	uint64_t addr                         : 27; /**< The 128b-aligned starting address used by the WR-DMA engine when reading WR-DMA data from
                                                         local DSP memory.
                                                         When ADDR[30]=1(IMEM):
                                                            For MBP32 (256KB IMEM): Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4003.FFFF >> 4).
                                                            For SSP16 (64KB IMEM):  Valid 128b-aligned address range = (0x4000.0000 >> 4) -
                                                         (0x4000.FFFF >> 4).
                                                         When ADDR[30]=0(DMEM):
                                                            For MBP32 (128KB DMEM): Valid 128b-aligned address range = (0x3FFE.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                            For SSP16 (256KB DMEM): Valid 128b-aligned address range = (0x3FFC.0000 >> 4) -
                                                         (0x3FFF.FFFF >> 4).
                                                         NOTE: In the I/D MEM address decoding scheme, ADDR[30] determines WHICH DSP Memory space
                                                         (0:DMEM/1:IMEM) */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t addr                         : 27;
	uint64_t reserved_31_63               : 33;
#endif
	} s;
	struct cvmx_mdabx_wr_addr_s           cnf75xx;
};
typedef union cvmx_mdabx_wr_addr cvmx_mdabx_wr_addr_t;

/**
 * cvmx_mdab#_wr_length
 *
 * The actual 128-bit length of the most recent WR-DMA engine job configuration transfer.
 *
 */
union cvmx_mdabx_wr_length {
	uint64_t u64;
	struct cvmx_mdabx_wr_length_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_19_63               : 45;
	uint64_t length                       : 15; /**< The actual 128-bit length of the most recent WR-DMA engine write dma transfer.
                                                         NOTE: This field will be updated when the WR-DMA has been actually write-committed to SMEM
                                                         or L2C/DDR Memory,
                                                         which allows software to read(poll) the length field during the WR-DMA transfer to know
                                                         how many 128-bit chunks are
                                                         now currently available at their intended memory destinations before use by software or
                                                         other hardware
                                                         blocks. */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t length                       : 15;
	uint64_t reserved_19_63               : 45;
#endif
	} s;
	struct cvmx_mdabx_wr_length_s         cnf75xx;
};
typedef union cvmx_mdabx_wr_length cvmx_mdabx_wr_length_t;

/**
 * cvmx_mdab#_wr_limit
 *
 * The maximum number of 128-bit sized chunks that the WR-DMA engine may read starting from the
 * 128-bit-aligned MDAB_WR_ADDR. NOTE: During Job_Type = ST_LCL* commands, hardware loads the
 * WR_ADDR/WR_LIMIT registers
 * directly.
 */
union cvmx_mdabx_wr_limit {
	uint64_t u64;
	struct cvmx_mdabx_wr_limit_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t last                         : 1;  /**< Indicates the last WR-DMA slice which may be read from address MDAB_WR_ADDR.
                                                         DSP software may choose to segment the JD.WR-DMA data into multiple non-contiguous WR-DMA
                                                         transfers from DSP local memory.
                                                         Eventually, DSP software will indicate the LAST segment (or slice), to indicate this is
                                                         the final WR-DMA sub-blk transfer.
                                                         Each WR-DMA operation's LAST will indicate when the last segment (or slice) WR-DMA
                                                         operation has been setup, to complete the JD.WR-DMA data transfer.
                                                         SW_NOTE: For every new WR-DMA slice operation, DSP software *MUST* signal LAST to indicate
                                                         the final JD.WR-DMA data slice transfer. */
	uint64_t reserved_19_30               : 12;
	uint64_t limit                        : 15; /**< "The maximum number of 128-bit chunks that the WR-DMA engine may read from address
                                                         MDAB_WR_ADDR.
                                                         The JD.JCFG data typically will contain the total WR-DMA Length for each new job. The
                                                         DSP software is at liberty to break the total WR-DMA transfer into multiple WR-DMA
                                                         segments (or
                                                         slices) using the LIMIT register.
                                                         DSP software will set the LAST flag and a LIMIT for the final WR-DMA slice.
                                                         When LIMIT has been reached during the LAST WR-DMA slice (or if the last JD.WR-DMA's write
                                                         commit is detected), the WR-DMA is done.
                                                         For MBP DSP:
                                                            IMEM: 0x4000.0000 - 0x4003.FFFF
                                                            DMEM: 0x3FFE.0000 - 0x3FFF.FFFF
                                                         For SSP DSP:
                                                            IMEM: 0x4000.0000 - 0x4000.FFFF
                                                            DMEM: 0x3FFC.0000 - 0x3FFF.FFFF
                                                         Software restriction \#1: software should never program LIMIT < 2. Indeterminate results
                                                         can/will occur.
                                                         Software restriction \#2: software should ensure that ADDR+LIMIT SIZES fall within the I+D
                                                         MEM space, otherwise DMA_ERROR and indeterminate results can/will occur." */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t limit                        : 15;
	uint64_t reserved_19_30               : 12;
	uint64_t last                         : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_wr_limit_s          cnf75xx;
};
typedef union cvmx_mdabx_wr_limit cvmx_mdabx_wr_limit_t;

/**
 * cvmx_mdab#_wr_status
 *
 * Reports the status for the WR-DMA engine.
 *
 */
union cvmx_mdabx_wr_status {
	uint64_t u64;
	struct cvmx_mdabx_wr_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t job_tag                      : 16; /**< The JOB_TAG field from the PSM command for the last completed WR-DMA operation.
                                                         When DSP software writes START_BUSY=0->1, the WR-DMA engine will wait until the next
                                                         available job has been enqueued, at which point the JOB_TAG will be latched in. */
	uint64_t reserved_12_15               : 4;
	uint64_t post_st_lcl                  : 1;  /**< Set when a store-local job has completed(last ST-LCL micro-op completes).
                                                         Cleared when a normal job has completed. */
	uint64_t st_lcl_busy                  : 1;  /**< Set when a store-local operation is active. */
	uint64_t pending_slot                 : 1;  /**< The job slot for the pending WR-DMA operation. Valid only when [START_BUSY] == 0 and
                                                         [DMA_PENDING] == 1. */
	uint64_t job_slot                     : 1;  /**< The job slot for the most recent WR-DMA operation. */
	uint64_t reserved_7_7                 : 1;
	uint64_t dma_uflow                    : 1;  /**< Set when the last WR-DMA operation encountered an underflow. */
	uint64_t dma_oflow                    : 1;  /**< Set when the last WR-DMA operation encountered an overflow. */
	uint64_t dma_error                    : 1;  /**< Set when the last WR-DMA operation encountered an error. */
	uint64_t last_dma_done                : 1;  /**< Set when the last WR-DMA operation received the last write-commit for the WR-DMA data from
                                                         the job descriptor (JD.WR-DMA). WR-DMAs are issued to either SMEM or L2C/DDR, and the WR-
                                                         DMA engine will receive
                                                         ordered write responses from the MHBW-AMM to indicate which writes have been committed. */
	uint64_t dma_pending                  : 1;  /**< Set when a WR-DMA operation is pending for the engine and the internal interface needs to
                                                         be programmed. */
	uint64_t post_reset                   : 1;  /**< Set to one on MDAB reset and remains one until the WR-DMA engine starts its first
                                                         WR-DMA transfer.
                                                         SWNOTE: DSP software can use this bit to qualify the [JOB_TAG], [JOB_SLOT], [DMA_ERROR],
                                                         [LAST_DMA_DONE]
                                                         bits in this CSR. [see: these status bits remain set and POST_RESET is used
                                                         by software to know when they are truly valid].
                                                         SET(hardware): MDAB reset
                                                         CLR(software): DSP writes [START_BUSY]=0->1 */
	uint64_t start_busy                   : 1;  /**< When software writes a one to this register, hardware will copy the data for the completed
                                                         job from the local memory starting at MDAB_WD_ADDR to the mdbw, up to
                                                         MDAB_WR_ADDR+MDAB_WR_LIMIT
                                                         or when the last 128-bit word of the JD.WR-DMA section is transferred (whichever comes
                                                         first).
                                                         If the WR-DMA engine has already transferred part but not all of the JD.WR-DMA section for
                                                         a job, then it will continue transferring data until it reaches the end of the JD.WR-DMA
                                                         section,
                                                         or until it writes MDAB_CFG_LIMIT for the LAST WR-DMA slice, whichever comes first.
                                                         This bit is reset to zero when the WR-DMA engine has completed the transfer and is ready
                                                         to be reinitialized for another transfer. */
#else
	uint64_t start_busy                   : 1;
	uint64_t post_reset                   : 1;
	uint64_t dma_pending                  : 1;
	uint64_t last_dma_done                : 1;
	uint64_t dma_error                    : 1;
	uint64_t dma_oflow                    : 1;
	uint64_t dma_uflow                    : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t job_slot                     : 1;
	uint64_t pending_slot                 : 1;
	uint64_t st_lcl_busy                  : 1;
	uint64_t post_st_lcl                  : 1;
	uint64_t reserved_12_15               : 4;
	uint64_t job_tag                      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_mdabx_wr_status_s         cnf75xx;
};
typedef union cvmx_mdabx_wr_status cvmx_mdabx_wr_status_t;

#endif
