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
 * cvmx-cprix-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon cprix.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_CPRIX_DEFS_H__
#define __CVMX_CPRIX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_BFN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_BFN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_BFN(offset) (CVMX_ADD_IO_SEG(0x00011800D5300040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_CM_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_CM_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300078ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_CM_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300078ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_CM_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_CM_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300080ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_CM_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5300080ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300010ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300010ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_CTRL_INDEX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_CTRL_INDEX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300018ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_CTRL_INDEX(offset) (CVMX_ADD_IO_SEG(0x00011800D5300018ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_ADDR_LSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_ADDR_LSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_ADDR_LSB(offset) (CVMX_ADD_IO_SEG(0x00011800D5301060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_ADDR_MSB(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_ADDR_MSB(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301058ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_ADDR_MSB(offset) (CVMX_ADD_IO_SEG(0x00011800D5301058ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CNT_DMAC_MISM(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CNT_DMAC_MISM(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53010A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CNT_DMAC_MISM(offset) (CVMX_ADD_IO_SEG(0x00011800D53010A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CNT_RX_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CNT_RX_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301090ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CNT_RX_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5301090ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CNT_TX_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CNT_TX_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301098ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CNT_TX_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5301098ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CONFIG_1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CONFIG_1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301010ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CONFIG_1(offset) (CVMX_ADD_IO_SEG(0x00011800D5301010ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CONFIG_2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CONFIG_2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301018ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CONFIG_2(offset) (CVMX_ADD_IO_SEG(0x00011800D5301018ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_CONFIG_3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_CONFIG_3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301088ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_CONFIG_3(offset) (CVMX_ADD_IO_SEG(0x00011800D5301088ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_HASH_TABLE(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_HASH_TABLE(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301068ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_HASH_TABLE(offset) (CVMX_ADD_IO_SEG(0x00011800D5301068ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_RX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_RX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_RX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800D5301020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_RX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_RX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301028ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_RX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D5301028ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_RX_DATA_WAIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_RX_DATA_WAIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301030ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_RX_DATA_WAIT(offset) (CVMX_ADD_IO_SEG(0x00011800D5301030ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_RX_EX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_RX_EX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301050ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_RX_EX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301050ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_RX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_RX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_RX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_TX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_TX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301038ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_TX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800D5301038ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_TX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_TX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301040ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_TX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D5301040ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_TX_DATA_WAIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_TX_DATA_WAIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301048ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_TX_DATA_WAIT(offset) (CVMX_ADD_IO_SEG(0x00011800D5301048ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ETH_TX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ETH_TX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301008ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ETH_TX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301008ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_EX_DELAY_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_EX_DELAY_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_EX_DELAY_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D53000A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_EX_DELAY_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_EX_DELAY_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000A8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_EX_DELAY_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D53000A8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_AXC_CONFIG_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_AXC_CONFIG_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300318ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_AXC_CONFIG_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300318ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_CNT_AXC_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_CNT_AXC_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300320ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_CNT_AXC_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300320ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300308ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300308ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_CONFIG_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_CONFIG_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300300ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_CONFIG_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300300ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_FREQ_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_FREQ_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300310ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_FREQ_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300310ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_GRP_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_GRP_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300330ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_GRP_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300330ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_GSM_GRP_CONFIG_SEL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_GSM_GRP_CONFIG_SEL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300328ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_GSM_GRP_CONFIG_SEL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300328ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_CNT_RX_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_CNT_RX_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301890ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_CNT_RX_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5301890ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_CNT_TX_FRAME(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_CNT_TX_FRAME(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301898ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_CNT_TX_FRAME(offset) (CVMX_ADD_IO_SEG(0x00011800D5301898ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301810ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5301810ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_CONFIG_2(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_CONFIG_2(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301818ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_CONFIG_2(offset) (CVMX_ADD_IO_SEG(0x00011800D5301818ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_CONFIG_3(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_CONFIG_3(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301888ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_CONFIG_3(offset) (CVMX_ADD_IO_SEG(0x00011800D5301888ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_RX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_RX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301820ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_RX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800D5301820ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_RX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_RX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301828ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_RX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D5301828ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_RX_DATA_WAIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_RX_DATA_WAIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301830ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_RX_DATA_WAIT(offset) (CVMX_ADD_IO_SEG(0x00011800D5301830ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_RX_EX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_RX_EX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301850ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_RX_EX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301850ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_RX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_RX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301800ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_RX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301800ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_TX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_TX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301838ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_TX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800D5301838ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_TX_DATA(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_TX_DATA(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301840ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_TX_DATA(offset) (CVMX_ADD_IO_SEG(0x00011800D5301840ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_TX_DATA_WAIT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_TX_DATA_WAIT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301848ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_TX_DATA_WAIT(offset) (CVMX_ADD_IO_SEG(0x00011800D5301848ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HDLC_TX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HDLC_TX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5301808ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HDLC_TX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5301808ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HFN(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HFN(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300048ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HFN(offset) (CVMX_ADD_IO_SEG(0x00011800D5300048ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_HW_RESET(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_HW_RESET(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300058ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_HW_RESET(offset) (CVMX_ADD_IO_SEG(0x00011800D5300058ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_INTR(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_INTR(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300000ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_INTR(offset) (CVMX_ADD_IO_SEG(0x00011800D5300000ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_IQ_RX_BUF_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_IQ_RX_BUF_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5300200ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_IQ_RX_BUF_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5300200ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_IQ_RX_BUF_SYNC_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_IQ_RX_BUF_SYNC_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5300220ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_IQ_RX_BUF_SYNC_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5300220ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_IQ_TX_BUF_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_IQ_TX_BUF_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5300240ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_IQ_TX_BUF_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5300240ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_IQ_TX_BUF_SYNC_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_IQ_TX_BUF_SYNC_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D5300260ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_IQ_TX_BUF_SYNC_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D5300260ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_LCV(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_LCV(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300038ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_LCV(offset) (CVMX_ADD_IO_SEG(0x00011800D5300038ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300180ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300180ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_K_SELECT_RXX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_MAP_K_SELECT_RXX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53003A0ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_MAP_K_SELECT_RXX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53003A0ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_K_SELECT_TXX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 3)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_MAP_K_SELECT_TXX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53003C0ull) + (((offset) & 3) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_MAP_K_SELECT_TXX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53003C0ull) + (((offset) & 3) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_OFFSET_RX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_OFFSET_RX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001C8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_OFFSET_RX(offset) (CVMX_ADD_IO_SEG(0x00011800D53001C8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_OFFSET_TX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_OFFSET_TX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001D0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_OFFSET_TX(offset) (CVMX_ADD_IO_SEG(0x00011800D53001D0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_SMPL_CFG_RX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_SMPL_CFG_RX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300380ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_SMPL_CFG_RX(offset) (CVMX_ADD_IO_SEG(0x00011800D5300380ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_SMPL_CFG_TX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_SMPL_CFG_TX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300388ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_SMPL_CFG_TX(offset) (CVMX_ADD_IO_SEG(0x00011800D5300388ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300190ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300190ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_INDEX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_INDEX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300198ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_INDEX(offset) (CVMX_ADD_IO_SEG(0x00011800D5300198ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_RX0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_RX0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001A0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_RX0(offset) (CVMX_ADD_IO_SEG(0x00011800D53001A0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_RX1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_RX1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001A8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_RX1(offset) (CVMX_ADD_IO_SEG(0x00011800D53001A8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_TX0(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_TX0(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001B0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_TX0(offset) (CVMX_ADD_IO_SEG(0x00011800D53001B0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_MAP_TBL_TX1(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_MAP_TBL_TX1(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001B8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_MAP_TBL_TX1(offset) (CVMX_ADD_IO_SEG(0x00011800D53001B8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_PHY_LOOP(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_PHY_LOOP(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300060ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_PHY_LOOP(offset) (CVMX_ADD_IO_SEG(0x00011800D5300060ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_PRBS_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_PRBS_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300280ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_PRBS_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300280ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_PRBS_STATUSX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 1)) && ((block_id <= 5))))))
		cvmx_warn("CVMX_CPRIX_PRBS_STATUSX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x00011800D53002A0ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8;
}
#else
#define CVMX_CPRIX_PRBS_STATUSX(offset, block_id) (CVMX_ADD_IO_SEG(0x00011800D53002A0ull) + (((offset) & 1) + ((block_id) & 7) * 0x1000ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_ROUND_DELAY(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_ROUND_DELAY(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300098ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_ROUND_DELAY(offset) (CVMX_ADD_IO_SEG(0x00011800D5300098ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_BUF_RESYNC_CNT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_BUF_RESYNC_CNT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000E8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_BUF_RESYNC_CNT(offset) (CVMX_ADD_IO_SEG(0x00011800D53000E8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_BUF_THRES(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_BUF_THRES(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_BUF_THRES(offset) (CVMX_ADD_IO_SEG(0x00011800D53000E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300020ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300020ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_DELAY(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_DELAY(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300090ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_DELAY(offset) (CVMX_ADD_IO_SEG(0x00011800D5300090ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_DELAY_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_DELAY_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300088ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_DELAY_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300088ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_RX_SCR_SEED(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_RX_SCR_SEED(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000C0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_RX_SCR_SEED(offset) (CVMX_ADD_IO_SEG(0x00011800D53000C0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_SERDES_CONFIG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_SERDES_CONFIG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300100ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_SERDES_CONFIG(offset) (CVMX_ADD_IO_SEG(0x00011800D5300100ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_START_OFFSET_RX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_START_OFFSET_RX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001D8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_START_OFFSET_RX(offset) (CVMX_ADD_IO_SEG(0x00011800D53001D8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_START_OFFSET_TX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_START_OFFSET_TX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53001E0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_START_OFFSET_TX(offset) (CVMX_ADD_IO_SEG(0x00011800D53001E0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_STATUS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_STATUS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300008ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_STATUS(offset) (CVMX_ADD_IO_SEG(0x00011800D5300008ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_TX_CONTROL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_TX_CONTROL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000D8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_TX_CONTROL(offset) (CVMX_ADD_IO_SEG(0x00011800D53000D8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_TX_CTRL(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_TX_CTRL(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D5300028ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_TX_CTRL(offset) (CVMX_ADD_IO_SEG(0x00011800D5300028ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_TX_PROT_VER(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_TX_PROT_VER(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000B0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_TX_PROT_VER(offset) (CVMX_ADD_IO_SEG(0x00011800D53000B0ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_TX_SCR_SEED(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_TX_SCR_SEED(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000B8ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_TX_SCR_SEED(offset) (CVMX_ADD_IO_SEG(0x00011800D53000B8ull) + ((offset) & 7) * 32768)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_CPRIX_TX_SYNC_DELAY(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 5)))))
		cvmx_warn("CVMX_CPRIX_TX_SYNC_DELAY(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x00011800D53000D0ull) + ((offset) & 7) * 32768;
}
#else
#define CVMX_CPRIX_TX_SYNC_DELAY(offset) (CVMX_ADD_IO_SEG(0x00011800D53000D0ull) + ((offset) & 7) * 32768)
#endif

/**
 * cvmx_cpri#_bfn
 */
union cvmx_cprix_bfn {
	uint64_t u64;
	struct cvmx_cprix_bfn_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t bfn                          : 12; /**< Current BFN from BFN alignment state machine. */
#else
	uint64_t bfn                          : 12;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_cprix_bfn_s               cnf75xx;
};
typedef union cvmx_cprix_bfn cvmx_cprix_bfn_t;

/**
 * cvmx_cpri#_cm_config
 */
union cvmx_cprix_cm_config {
	uint64_t u64;
	struct cvmx_cprix_cm_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t tx_slow_cm_rate              : 3;  /**< Reserved. CNF75XX does not support HDLC. */
	uint64_t reserved_6_7                 : 2;
	uint64_t tx_fast_cm_ptr               : 6;  /**< Pointer to first CPRI control word used for fast C&M. This value will be inserted into
                                                         CPRI
                                                         control byte Z.194.0. */
#else
	uint64_t tx_fast_cm_ptr               : 6;
	uint64_t reserved_6_7                 : 2;
	uint64_t tx_slow_cm_rate              : 3;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_cprix_cm_config_s         cnf75xx;
};
typedef union cvmx_cprix_cm_config cvmx_cprix_cm_config_t;

/**
 * cvmx_cpri#_cm_status
 *
 * This register reports the received C&M channel configuration.
 *
 */
union cvmx_cprix_cm_status {
	uint64_t u64;
	struct cvmx_cprix_cm_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t rx_slow_cm_rate_valid        : 1;  /**< When set, indicates that a valid slow C&M rate has been accepted. Note
                                                         that slow C&M channel is not supported and recevied data will be
                                                         discarded. */
	uint64_t rx_slow_cm_rate              : 3;  /**< Accepted receive slow C&M rate.
                                                         0x0 = No HDLC.
                                                         0x1 = 240 Kbit/s.
                                                         0x2 = 480 Kbit/s.
                                                         0x3 = 960 Kbit/s.
                                                         0x4 = 1920 Kbit/s.
                                                         0x5 = 2400 Kbit/s.
                                                         0x6 = Highest possible rate when line rate is greater than 3072 Mbps.
                                                         0x7 = Reserved. */
	uint64_t reserved_7_7                 : 1;
	uint64_t rx_fast_cm_ptr_valid         : 1;  /**< When set, indicates that a valid fast C&M pointer has been accepted. */
	uint64_t rx_fast_cm_ptr               : 6;  /**< Accepted receive fast C&M pointer. */
#else
	uint64_t rx_fast_cm_ptr               : 6;
	uint64_t rx_fast_cm_ptr_valid         : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t rx_slow_cm_rate              : 3;
	uint64_t rx_slow_cm_rate_valid        : 1;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_cprix_cm_status_s         cnf75xx;
};
typedef union cvmx_cprix_cm_status cvmx_cprix_cm_status_t;

/**
 * cvmx_cpri#_config
 */
union cvmx_cprix_config {
	uint64_t u64;
	struct cvmx_cprix_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t ori_spec                     : 1;  /**< ORI specification version used for RTWP mapping.
                                                         0 = ORI spec version 1.1.1.
                                                         1 = ORI spec version 4.1.1. */
	uint64_t set_10_acks                  : 1;  /**< N/A */
	uint64_t rtwp_enable_rx               : 1;  /**< When set, enables extraction of RTWP. */
	uint64_t rtwp_enable_tx               : 1;  /**< N/A */
	uint64_t cnt_6_reset                  : 1;  /**< N/A */
	uint64_t sync_pulse_in_re_mode        : 1;  /**< N/A */
	uint64_t rx_enable_buffer             : 1;  /**< 0 = Disable CPRI receiver.
                                                         1 = Enable CPRI receiver. */
	uint64_t ethernet_idle_select         : 1;  /**< Select whether to transmit zeros or ones on Ethernet channel when
                                                         idle. This only applies when [ETHERNET_MODE] = 1.
                                                         0 = Transmit zeros when idle.
                                                         1 = Transmit ones when idle. */
	uint64_t rx_cm_select                 : 1;  /**< Receiver control and management (C & M) channel configuration.
                                                         0 = Use accepted values.
                                                         1 = Use TX configuration. */
	uint64_t ethernet_mode                : 1;  /**< Select mapping of Fast C&M Ethernet channel.
                                                         0 = Standard CPRI v.4.2 mode.
                                                         1 = Alternative IR mapping mode. */
	uint64_t tx_enable                    : 1;  /**< 0 = Disable transmit operations.
                                                         1 = Enable transmit operations. */
	uint64_t loop_mode                    : 3;  /**< N/A */
	uint64_t sync_mode                    : 1;  /**< N/A */
	uint64_t tx_ctrl_insert_en            : 1;  /**< Global enable on CPRI control word insertion. This bit overrides the
                                                         insertion enables configured using the CPRI()_TX_CTRL register. When
                                                         [TX_CTRL_INSER_EN] is set to one, individual bytes must also be enabled
                                                         using CPRI()_TX_CTRL regiser. When cleared, none of the control words
                                                         specified by CPRI()_TX_CTRL are inserted in the transmitted frames. */
#else
	uint64_t tx_ctrl_insert_en            : 1;
	uint64_t sync_mode                    : 1;
	uint64_t loop_mode                    : 3;
	uint64_t tx_enable                    : 1;
	uint64_t ethernet_mode                : 1;
	uint64_t rx_cm_select                 : 1;
	uint64_t ethernet_idle_select         : 1;
	uint64_t rx_enable_buffer             : 1;
	uint64_t sync_pulse_in_re_mode        : 1;
	uint64_t cnt_6_reset                  : 1;
	uint64_t rtwp_enable_tx               : 1;
	uint64_t rtwp_enable_rx               : 1;
	uint64_t set_10_acks                  : 1;
	uint64_t ori_spec                     : 1;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_cprix_config_s            cnf75xx;
};
typedef union cvmx_cprix_config cvmx_cprix_config_t;

/**
 * cvmx_cpri#_ctrl_index
 *
 * This register selects which control word will be read/written by accesses
 * to both the CPRI()_RX_CTRL and CPRI()_TX_CTRL registers.
 */
union cvmx_cprix_ctrl_index {
	uint64_t u64;
	struct cvmx_cprix_ctrl_index_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t cpri_ctrl_sel                : 4;  /**< Specifies the byte y within the control word Z.x.0. */
	uint64_t cpri_ctrl_indx               : 8;  /**< Specifies the basic frame index 'x' for the control word Z.x.0. */
#else
	uint64_t cpri_ctrl_indx               : 8;
	uint64_t cpri_ctrl_sel                : 4;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_cprix_ctrl_index_s        cnf75xx;
};
typedef union cvmx_cprix_ctrl_index cvmx_cprix_ctrl_index_t;

/**
 * cvmx_cpri#_eth_addr_lsb
 */
union cvmx_cprix_eth_addr_lsb {
	uint64_t u64;
	struct cvmx_cprix_eth_addr_lsb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t mac_31_0                     : 32; /**< Least signficant four bytes of local MAC address. */
#else
	uint64_t mac_31_0                     : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_addr_lsb_s      cnf75xx;
};
typedef union cvmx_cprix_eth_addr_lsb cvmx_cprix_eth_addr_lsb_t;

/**
 * cvmx_cpri#_eth_addr_msb
 *
 * Ethernet MAC address (MSB)
 *
 */
union cvmx_cprix_eth_addr_msb {
	uint64_t u64;
	struct cvmx_cprix_eth_addr_msb_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t mac_47_32                    : 16; /**< Most significant two bytes of local MAC address. */
#else
	uint64_t mac_47_32                    : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_cprix_eth_addr_msb_s      cnf75xx;
};
typedef union cvmx_cprix_eth_addr_msb cvmx_cprix_eth_addr_msb_t;

/**
 * cvmx_cpri#_eth_cnt_dmac_mism
 */
union cvmx_cprix_eth_cnt_dmac_mism {
	uint64_t u64;
	struct cvmx_cprix_eth_cnt_dmac_mism_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eth_cnt_dmac_mismatch        : 32; /**< The number of received (and discarded) frames whose destination MAC
                                                         address did not match the local MAC address and didn't pass the
                                                         multicast or broadcast filters. */
#else
	uint64_t eth_cnt_dmac_mismatch        : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_cnt_dmac_mism_s cnf75xx;
};
typedef union cvmx_cprix_eth_cnt_dmac_mism cvmx_cprix_eth_cnt_dmac_mism_t;

/**
 * cvmx_cpri#_eth_cnt_rx_frame
 */
union cvmx_cprix_eth_cnt_rx_frame {
	uint64_t u64;
	struct cvmx_cprix_eth_cnt_rx_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eth_cnt_rx_frame             : 32; /**< Number of received frames. */
#else
	uint64_t eth_cnt_rx_frame             : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_cnt_rx_frame_s  cnf75xx;
};
typedef union cvmx_cprix_eth_cnt_rx_frame cvmx_cprix_eth_cnt_rx_frame_t;

/**
 * cvmx_cpri#_eth_cnt_tx_frame
 */
union cvmx_cprix_eth_cnt_tx_frame {
	uint64_t u64;
	struct cvmx_cprix_eth_cnt_tx_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t eth_cnt_tx_frame             : 32; /**< Number of transmitted frames. */
#else
	uint64_t eth_cnt_tx_frame             : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_cnt_tx_frame_s  cnf75xx;
};
typedef union cvmx_cprix_eth_cnt_tx_frame cvmx_cprix_eth_cnt_tx_frame_t;

/**
 * cvmx_cpri#_eth_config_1
 *
 * Ethernet configuration
 *
 */
union cvmx_cprix_eth_config_1 {
	uint64_t u64;
	struct cvmx_cprix_eth_config_1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t crc_strip_en                 : 1;  /**< When set, the CRC is stripped from incoming packets. */
	uint64_t mac_fail_pass_en             : 1;  /**< Accept incoming packets that fail the unicast, multicast and broadcast
                                                         MAC checks.
                                                         0 = When [MAC_CHECK]=1, packets not passing any filter are discarded.
                                                         1 = When [MAC_CHECK]=1, packets that do not pass any filters are
                                                         accepted, and the failure is recorded by setting
                                                         RFIF_ETH_RX_WQE_S[DMAC_MISMATCH]=1 and
                                                         RFIF_ETH_RX_WQE_S[MAC_RES]=0x7. */
	uint64_t intr_tx_ready_block_en       : 1;  /**< N/A */
	uint64_t intr_tx_abort_en             : 1;  /**< N/A */
	uint64_t intr_tx_ready_en             : 1;  /**< N/A */
	uint64_t intr_rx_ready_block_en       : 1;  /**< N/A */
	uint64_t intr_rx_ready_end_en         : 1;  /**< N/A */
	uint64_t intr_rx_abort_en             : 1;  /**< N/A */
	uint64_t intr_rx_ready_en             : 1;  /**< N/A */
	uint64_t intr_tx_en                   : 1;  /**< N/A */
	uint64_t intr_rx_en                   : 1;  /**< N/A */
	uint64_t intr_en                      : 1;  /**< N/A */
	uint64_t rx_long_frame_en             : 1;  /**< When set, enable reception of Ethernet frame longer than 1536 bytes.
                                                         Note that the RFIF RMAC does not support Ethernet frames longer than
                                                         1536 bytes. */
	uint64_t rx_preamble_abort_en         : 1;  /**< When set, discard incoming frames with illegal preamble nibble for
                                                         receiving SFD. */
	uint64_t broadcast_en                 : 1;  /**< When [MAC_CHECK]=1, allow incoming broadcast packets. */
	uint64_t multicast_flt_en             : 1;  /**< When [MAC_CHECK]=1, enable the multicast filter. Refer to
                                                         CPRI()_ETH_HASH_TABLE for more details. */
	uint64_t mac_check                    : 1;  /**< When set, enable checking of the destination MAC address for received
                                                         packets.
                                                         When MAC_CHECK=1, packets with the DMAC set to the local MAC address
                                                         will be accepted. If [MULTICAST_FLT_EN]=1, then multicast packets that
                                                         are accepted by the mutlicast filter are also accepted. If
                                                         [BROADCAST_EN]=1, then broadcast packets are also accepted. All other
                                                         packets are discarded. */
	uint64_t length_check                 : 1;  /**< When set, packets smaller than 64 bytes will be discarded. */
	uint64_t mac_reset                    : 1;  /**< Ethernet MAC reset. */
	uint64_t reserved_2_2                 : 1;
	uint64_t little_endian_en             : 1;  /**< Select the endianness for Ethernet receive and transmit data:
                                                         0 = Big-endian.
                                                         1 = Little-endian. */
	uint64_t reserved_0_0                 : 1;
#else
	uint64_t reserved_0_0                 : 1;
	uint64_t little_endian_en             : 1;
	uint64_t reserved_2_2                 : 1;
	uint64_t mac_reset                    : 1;
	uint64_t length_check                 : 1;
	uint64_t mac_check                    : 1;
	uint64_t multicast_flt_en             : 1;
	uint64_t broadcast_en                 : 1;
	uint64_t rx_preamble_abort_en         : 1;
	uint64_t rx_long_frame_en             : 1;
	uint64_t intr_en                      : 1;
	uint64_t intr_rx_en                   : 1;
	uint64_t intr_tx_en                   : 1;
	uint64_t intr_rx_ready_en             : 1;
	uint64_t intr_rx_abort_en             : 1;
	uint64_t intr_rx_ready_end_en         : 1;
	uint64_t intr_rx_ready_block_en       : 1;
	uint64_t intr_tx_ready_en             : 1;
	uint64_t intr_tx_abort_en             : 1;
	uint64_t intr_tx_ready_block_en       : 1;
	uint64_t mac_fail_pass_en             : 1;
	uint64_t crc_strip_en                 : 1;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_cprix_eth_config_1_s      cnf75xx;
};
typedef union cvmx_cprix_eth_config_1 cvmx_cprix_eth_config_1_t;

/**
 * cvmx_cpri#_eth_config_2
 */
union cvmx_cprix_eth_config_2 {
	uint64_t u64;
	struct cvmx_cprix_eth_config_2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t crc_enable                   : 1;  /**< When set, enable automatic insertion of Ethernet FCS at the end of
                                                         each frame. When cleared, software must include the FCS in each frame.
                                                         Note that when including the FCS manually, the bit order must be
                                                         reversed for the FCS bytes. */
#else
	uint64_t crc_enable                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_cprix_eth_config_2_s      cnf75xx;
};
typedef union cvmx_cprix_eth_config_2 cvmx_cprix_eth_config_2_t;

/**
 * cvmx_cpri#_eth_config_3
 */
union cvmx_cprix_eth_config_3 {
	uint64_t u64;
	struct cvmx_cprix_eth_config_3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_14_63               : 50;
	uint64_t tx_start_thr                 : 6;  /**< Transmit start threshold. When store-and-forward mode is not enabled, transmission
                                                         will start when the specified number of 32-bit words have been stored in the
                                                         transmit buffer. */
	uint64_t reserved_2_7                 : 6;
	uint64_t rx_crc_en                    : 1;  /**< When set, enables FCS validation of received packets (packets that
                                                         fail the check are discarded). When cleared, no validation is
                                                         performed and all packets are accepted. */
	uint64_t tx_st_fwd                    : 1;  /**< Select between store-and-forward and cut-through mode.
                                                         0 =  Cut-through mode. Transmission stars when the buffer fill-level
                                                         exceeds [TX_START_THR].
                                                         1 = Store-and-forward mode. Store a full packet before start of
                                                         transmission. Packets longer than transmit buffer (256 bytes) will be aborted. */
#else
	uint64_t tx_st_fwd                    : 1;
	uint64_t rx_crc_en                    : 1;
	uint64_t reserved_2_7                 : 6;
	uint64_t tx_start_thr                 : 6;
	uint64_t reserved_14_63               : 50;
#endif
	} s;
	struct cvmx_cprix_eth_config_3_s      cnf75xx;
};
typedef union cvmx_cprix_eth_config_3 cvmx_cprix_eth_config_3_t;

/**
 * cvmx_cpri#_eth_hash_table
 *
 * This register configures the multicast filter hash table.
 *
 * When CPRI()_ETH_CONFIG_1[MAC_CHECK]=1 and
 * CPRI()_ETH_CONFIG_1[MULIT_CAST_FLT_EN]=1, received multicast packets are
 * filtered based on this hash table. Multicast packets must have bit 0 of
 * the least significant byte in the DMAC set to 1. The Ethernet CRC function
 * is then applied to the DMAC address, and the CRC value is used to select
 * one bit from [HASH]. If that bit is set, then the multicast packet is
 * accepted, otherwise the multicast packet is discarded.
 */
union cvmx_cprix_eth_hash_table {
	uint64_t u64;
	struct cvmx_cprix_eth_hash_table_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t hash                         : 32; /**< When bit i is set, then multicast DMAC addresses with a CRC of i
                                                         are accepted by the filter. If the bit is cleared, matching DMAC
                                                         addresses are discarded. */
#else
	uint64_t hash                         : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_hash_table_s    cnf75xx;
};
typedef union cvmx_cprix_eth_hash_table cvmx_cprix_eth_hash_table_t;

/**
 * cvmx_cpri#_eth_rx_control
 *
 * Ethernet receive control
 *
 */
union cvmx_cprix_eth_rx_control {
	uint64_t u64;
	struct cvmx_cprix_eth_rx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t rx_discard                   : 1;  /**< 1 = Discard current Ethernet RX frame.
                                                         0 = Discard not enabled. */
#else
	uint64_t rx_discard                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_cprix_eth_rx_control_s    cnf75xx;
};
typedef union cvmx_cprix_eth_rx_control cvmx_cprix_eth_rx_control_t;

/**
 * cvmx_cpri#_eth_rx_data
 *
 * Ethernet receive data
 *
 */
union cvmx_cprix_eth_rx_data {
	uint64_t u64;
	struct cvmx_cprix_eth_rx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_data                      : 32; /**< Ethernet RX frame data. */
#else
	uint64_t rx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_rx_data_s       cnf75xx;
};
typedef union cvmx_cprix_eth_rx_data cvmx_cprix_eth_rx_data_t;

/**
 * cvmx_cpri#_eth_rx_data_wait
 *
 * Ethernet receive data with wait-state
 *
 */
union cvmx_cprix_eth_rx_data_wait {
	uint64_t u64;
	struct cvmx_cprix_eth_rx_data_wait_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_data                      : 32; /**< Ethernet RX frame data.Wait states will be inserted until data is ready (or
                                                         the CPU time-out the operation). */
#else
	uint64_t rx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_rx_data_wait_s  cnf75xx;
};
typedef union cvmx_cprix_eth_rx_data_wait cvmx_cprix_eth_rx_data_wait_t;

/**
 * cvmx_cpri#_eth_rx_ex_status
 *
 * Ethernet RX Extra Status
 *
 */
union cvmx_cprix_eth_rx_ex_status {
	uint64_t u64;
	struct cvmx_cprix_eth_rx_ex_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t mii_protocol_error           : 1;  /**< 1 = Protocol error. 4B/5B decoding violation */
	uint64_t small_packet                 : 1;  /**< 1 = Frame too short. Aborted. */
	uint64_t rx_ex_overflow               : 1;  /**< 1 = RX buffer overflow */
	uint64_t dmac_mismatch                : 1;  /**< 1 = DMAC mismatch */
	uint64_t crc_error                    : 1;  /**< 1 = CRC error */
	uint64_t reserved_5_5                 : 1;
	uint64_t rx_error                     : 1;  /**< 1 = Abort from CPRI Layer */
	uint64_t long_frame                   : 1;  /**< 1 = Long frame detected */
	uint64_t short_frame                  : 1;  /**< 1 = Short frame detected */
	uint64_t reserved_0_1                 : 2;
#else
	uint64_t reserved_0_1                 : 2;
	uint64_t short_frame                  : 1;
	uint64_t long_frame                   : 1;
	uint64_t rx_error                     : 1;
	uint64_t reserved_5_5                 : 1;
	uint64_t crc_error                    : 1;
	uint64_t dmac_mismatch                : 1;
	uint64_t rx_ex_overflow               : 1;
	uint64_t small_packet                 : 1;
	uint64_t mii_protocol_error           : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_cprix_eth_rx_ex_status_s  cnf75xx;
};
typedef union cvmx_cprix_eth_rx_ex_status cvmx_cprix_eth_rx_ex_status_t;

/**
 * cvmx_cpri#_eth_rx_status
 */
union cvmx_cprix_eth_rx_status {
	uint64_t u64;
	struct cvmx_cprix_eth_rx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_15_63               : 49;
	uint64_t mac_status                   : 2;  /**< '"00" : Packet passed Unicast filter"01" : Packet passed Multicast filter"10"
                                                         : Packet passed Broadcast filter"11" : Forwarded packet did not pass filter.' */
	uint64_t rx_crc_error                 : 1;  /**< 1 = Frame aborted due to CRC error. */
	uint64_t rx_empty                     : 1;  /**< 1 = Read from empty buffer. */
	uint64_t rx_mii_error                 : 1;  /**< 1 = Frame aborted due to mii protocol error. */
	uint64_t rx_small_packet              : 1;  /**< 1 = Frame too short. Aborted. */
	uint64_t rx_overflow                  : 1;  /**< 1 = Frame aborted due to Rx buffer overflow. */
	uint64_t rx_dmac_mismatch             : 1;  /**< 1 = Frame aborted due to DMAC mismatch. */
	uint64_t rx_ready_block               : 1;  /**< 1 : Guaranteed 2^WIDTH_ETH_BLOCK wordsEthernet RX data ready.0 : Not
                                                         ready. */
	uint64_t rx_ready_end                 : 1;  /**< 1 : EOP ready in RX buffer.0 : EOP not ready in RX buffer. */
	uint64_t rx_length                    : 2;  /**< Length of last word in the packet.
                                                         0x0 means 1 valid byte.
                                                         0x1 means 2 valid bytes.
                                                         0x2 means 3 valid bytes.
                                                         0x3 means 4 valid bytes. */
	uint64_t rx_abort                     : 1;  /**< 1 : Ethernet RX packet aborted.0 : Ethernet RX packet not aborted. */
	uint64_t rx_eof                       : 1;  /**< 1 : Ethernet RX end of packet detected. Next word read will contain end
                                                         of packet.0 : Ethernet RX end of packet not detected. */
	uint64_t rx_ready                     : 1;  /**< 1 : Ethernet RX data ready.0 : Ethernet RX data not ready. */
#else
	uint64_t rx_ready                     : 1;
	uint64_t rx_eof                       : 1;
	uint64_t rx_abort                     : 1;
	uint64_t rx_length                    : 2;
	uint64_t rx_ready_end                 : 1;
	uint64_t rx_ready_block               : 1;
	uint64_t rx_dmac_mismatch             : 1;
	uint64_t rx_overflow                  : 1;
	uint64_t rx_small_packet              : 1;
	uint64_t rx_mii_error                 : 1;
	uint64_t rx_empty                     : 1;
	uint64_t rx_crc_error                 : 1;
	uint64_t mac_status                   : 2;
	uint64_t reserved_15_63               : 49;
#endif
	} s;
	struct cvmx_cprix_eth_rx_status_s     cnf75xx;
};
typedef union cvmx_cprix_eth_rx_status cvmx_cprix_eth_rx_status_t;

/**
 * cvmx_cpri#_eth_tx_control
 *
 * Ethernet transmit control
 *
 */
union cvmx_cprix_eth_tx_control {
	uint64_t u64;
	struct cvmx_cprix_eth_tx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t tx_length                    : 2;  /**< Length of last word in the packet.
                                                         0x0 = means 1 valid bytes.
                                                         0x1 = means 2 valid
                                                         bytes.
                                                         0x2 = means 3 valid bytes.
                                                         0x3 = means 4 valid bytes.This field shall be
                                                         set when the tx_eof bit is asserted. */
	uint64_t tx_discard                   : 1;  /**< 1 = Discard TX packet.
                                                         0 = Do not discard TX packet */
	uint64_t tx_eop                       : 1;  /**< 1 = Indicate that next write to TX_DATA or TX_DATA_WAIT registers will be
                                                         last word of the TX packet. Will hereafter be cleared.
                                                         0 = Not last word */
#else
	uint64_t tx_eop                       : 1;
	uint64_t tx_discard                   : 1;
	uint64_t tx_length                    : 2;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_cprix_eth_tx_control_s    cnf75xx;
};
typedef union cvmx_cprix_eth_tx_control cvmx_cprix_eth_tx_control_t;

/**
 * cvmx_cpri#_eth_tx_data
 *
 * Ethernet transmit data
 *
 */
union cvmx_cprix_eth_tx_data {
	uint64_t u64;
	struct cvmx_cprix_eth_tx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t tx_data                      : 32; /**< Ethernet TX frame data."When writing the last word of the frame, data must be
                                                         aligned to bit 31. " */
#else
	uint64_t tx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_tx_data_s       cnf75xx;
};
typedef union cvmx_cprix_eth_tx_data cvmx_cprix_eth_tx_data_t;

/**
 * cvmx_cpri#_eth_tx_data_wait
 *
 * Ethernet transmit data with wait-state
 *
 */
union cvmx_cprix_eth_tx_data_wait {
	uint64_t u64;
	struct cvmx_cprix_eth_tx_data_wait_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t tx_data                      : 32; /**< Ethernet TX frame data.Wait states will be inserted until transmitter is ready
                                                         to get data (or the CPU time-out the operation)."When writing the last word
                                                         of the frame, data must be aligned to bit 31. " */
#else
	uint64_t tx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_eth_tx_data_wait_s  cnf75xx;
};
typedef union cvmx_cprix_eth_tx_data_wait cvmx_cprix_eth_tx_data_wait_t;

/**
 * cvmx_cpri#_eth_tx_status
 *
 * Ethernet transmit status register
 *
 */
union cvmx_cprix_eth_tx_status {
	uint64_t u64;
	struct cvmx_cprix_eth_tx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t tx_ready_block               : 1;  /**< 1 : Guaranteed ready to get2^WIDTH_ETH_BLOCK words of Ethernet TX data.0
                                                         : not ready. */
	uint64_t tx_abort                     : 1;  /**< 1 : Ethernet TX packet aborted.0 : Ethernet TX packet not aborted. */
	uint64_t tx_ready                     : 1;  /**< 1 : Ready to get Ethernet TX data.0 : Not ready to get Ethernet TX data */
#else
	uint64_t tx_ready                     : 1;
	uint64_t tx_abort                     : 1;
	uint64_t tx_ready_block               : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_cprix_eth_tx_status_s     cnf75xx;
};
typedef union cvmx_cprix_eth_tx_status cvmx_cprix_eth_tx_status_t;

/**
 * cvmx_cpri#_ex_delay_config
 */
union cvmx_cprix_ex_delay_config {
	uint64_t u64;
	struct cvmx_cprix_ex_delay_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t tx_ex_delay                  : 6;  /**< Additional transmit delay in units of bit-periods. Valid range is 0 to 39. */
	uint64_t reserved_9_15                : 7;
	uint64_t rx_ex_delay_period           : 9;  /**< Integration period for extended delay measurement. The value programmed must
                                                         be one less than the integration period. */
#else
	uint64_t rx_ex_delay_period           : 9;
	uint64_t reserved_9_15                : 7;
	uint64_t tx_ex_delay                  : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_cprix_ex_delay_config_s   cnf75xx;
};
typedef union cvmx_cprix_ex_delay_config cvmx_cprix_ex_delay_config_t;

/**
 * cvmx_cpri#_ex_delay_status
 */
union cvmx_cprix_ex_delay_status {
	uint64_t u64;
	struct cvmx_cprix_ex_delay_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_ex_buf_delay_valid        : 1;  /**< Set to one each time a new valid measurement is available in
                                                         [RX_EX_BUF_DELAY]. */
	uint64_t reserved_19_30               : 12;
	uint64_t rx_ex_buf_delay              : 19; /**< Result of extended delay measurement in units of 32-bit decoded words
                                                         (or 40 encoded bits at the line rate). */
#else
	uint64_t rx_ex_buf_delay              : 19;
	uint64_t reserved_19_30               : 12;
	uint64_t rx_ex_buf_delay_valid        : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_ex_delay_status_s   cnf75xx;
};
typedef union cvmx_cprix_ex_delay_status cvmx_cprix_ex_delay_status_t;

/**
 * cvmx_cpri#_gsm_axc_config_sel
 */
union cvmx_cprix_gsm_axc_config_sel {
	uint64_t u64;
	struct cvmx_cprix_gsm_axc_config_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t gsm_grp_config_axc           : 5;  /**< AxC to map to the group/counter specified by
                                                         CPRI()_GSM_CNT_AXC_CONFIG[MAP_AXC_GSM]. */
#else
	uint64_t gsm_grp_config_axc           : 5;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_cprix_gsm_axc_config_sel_s cnf75xx;
};
typedef union cvmx_cprix_gsm_axc_config_sel cvmx_cprix_gsm_axc_config_sel_t;

/**
 * cvmx_cpri#_gsm_cnt_axc_config
 *
 * Map AxCs to counter.
 *
 */
union cvmx_cprix_gsm_cnt_axc_config {
	uint64_t u64;
	struct cvmx_cprix_gsm_cnt_axc_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t map_axc_gsm                  : 2;  /**< This field specifies the group/counter mapped to by AxC specified by
                                                         CPRI()_GSM_AXC_CONFIG_SEL.
                                                         When using mapping method 3 (i.e., CPRI()_GSM_CONFIG[GSM_CONFIG] =
                                                         0x2), only the AxC mapped to entry 0 of a group needs to be mapped in
                                                         this register. */
#else
	uint64_t map_axc_gsm                  : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_cprix_gsm_cnt_axc_config_s cnf75xx;
};
typedef union cvmx_cprix_gsm_cnt_axc_config cvmx_cprix_gsm_cnt_axc_config_t;

/**
 * cvmx_cpri#_gsm_config
 */
union cvmx_cprix_gsm_config {
	uint64_t u64;
	struct cvmx_cprix_gsm_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t gsm_num_mapped_in_last       : 6;  /**< Number of mapped bits in the last basic frame of [GSM_NUM_ZEROS]. */
	uint64_t gsm_num_zeros                : 12; /**< When [GSM_CONFIG]=0x1, the number of initial basic frames (out of K)
                                                         containing zeros. The number of bits actually stuffed depends on the
                                                         number of bits configured for mapping in the map table. */
	uint64_t gsm_max_k                    : 3;  /**< Maximum K value for the GSM counter as specified by table 5D of CPRI v5.0:
                                                         0x0 = 0x4.
                                                         0x1 = 0x180.
                                                         0x2 = 0x300.
                                                         0x3 = 0x1200.
                                                         0x4-0x7 = Reserved */
	uint64_t gsm_config                   : 2;  /**< GSM counter mapping method:
                                                         0x0 = No GSM mapping. Map table works in normal mode.
                                                         0x1 = GSM mapping method 1.
                                                         0x2 = GSM mapping method 3.
                                                         0x3 = Reserved. */
#else
	uint64_t gsm_config                   : 2;
	uint64_t gsm_max_k                    : 3;
	uint64_t gsm_num_zeros                : 12;
	uint64_t gsm_num_mapped_in_last       : 6;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_cprix_gsm_config_s        cnf75xx;
};
typedef union cvmx_cprix_gsm_config cvmx_cprix_gsm_config_t;

/**
 * cvmx_cpri#_gsm_config_sel
 */
union cvmx_cprix_gsm_config_sel {
	uint64_t u64;
	struct cvmx_cprix_gsm_config_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_6_63                : 58;
	uint64_t gsm_cnt                      : 6;  /**< GSM counter to be configured in registers CPRI()_GSM_CONFIG and CPRI()_GSM_FREQ_CONFIG. */
#else
	uint64_t gsm_cnt                      : 6;
	uint64_t reserved_6_63                : 58;
#endif
	} s;
	struct cvmx_cprix_gsm_config_sel_s    cnf75xx;
};
typedef union cvmx_cprix_gsm_config_sel cvmx_cprix_gsm_config_sel_t;

/**
 * cvmx_cpri#_gsm_freq_config
 *
 * This register configures the GSM counter used for mapping method 3 (i.e.,
 * when CPRI()_GSM_CONFIG[GSM_CONFIG]=0x2).
 */
union cvmx_cprix_gsm_freq_config {
	uint64_t u64;
	struct cvmx_cprix_gsm_freq_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t gsm_fract                    : 22; /**< Fractional part of counter for mapping method 3. Resolution is 2^(-22). */
	uint64_t gsm_integer                  : 7;  /**< Integer part of counter for mapping method 3. */
#else
	uint64_t gsm_integer                  : 7;
	uint64_t gsm_fract                    : 22;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_cprix_gsm_freq_config_s   cnf75xx;
};
typedef union cvmx_cprix_gsm_freq_config cvmx_cprix_gsm_freq_config_t;

/**
 * cvmx_cpri#_gsm_grp_config
 *
 * These registers configure the GSM groups when using mapping method 3.
 *
 */
union cvmx_cprix_gsm_grp_config {
	uint64_t u64;
	struct cvmx_cprix_gsm_grp_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t map_gsm_axc_grp_3_7_11       : 7;  /**< Map AxC to group gsm_grp_config, entry 3+4*gsm_grp_no. */
	uint64_t map_gsm_axc_grp_en_3_7_11    : 1;  /**< Enable AxC in field below in group. */
	uint64_t map_gsm_axc_grp_2_6_10       : 7;  /**< Map AxC to group gsm_grp_config, entry 2+4*gsm_grp_no. */
	uint64_t map_gsm_axc_grp_en_2_6_10    : 1;  /**< Enable AxC in field below in group. */
	uint64_t map_gsm_axc_grp_1_5_9_13     : 7;  /**< Map AxC to group gsm_grp_config, entry 1+4*gsm_grp_no. */
	uint64_t map_gsm_axc_grp_en_1_5_9_13  : 1;  /**< Enable AxC in field below in group. */
	uint64_t map_gsm_axc_grp_0_4_8_12     : 7;  /**< Map AxC to group gsm_grp_config, entry 0+4*gsm_grp_no. */
	uint64_t map_gsm_axc_grp_en_0_4_8_12  : 1;  /**< Enable AxC in field below in group. */
#else
	uint64_t map_gsm_axc_grp_en_0_4_8_12  : 1;
	uint64_t map_gsm_axc_grp_0_4_8_12     : 7;
	uint64_t map_gsm_axc_grp_en_1_5_9_13  : 1;
	uint64_t map_gsm_axc_grp_1_5_9_13     : 7;
	uint64_t map_gsm_axc_grp_en_2_6_10    : 1;
	uint64_t map_gsm_axc_grp_2_6_10       : 7;
	uint64_t map_gsm_axc_grp_en_3_7_11    : 1;
	uint64_t map_gsm_axc_grp_3_7_11       : 7;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_gsm_grp_config_s    cnf75xx;
};
typedef union cvmx_cprix_gsm_grp_config cvmx_cprix_gsm_grp_config_t;

/**
 * cvmx_cpri#_gsm_grp_config_sel
 */
union cvmx_cprix_gsm_grp_config_sel {
	uint64_t u64;
	struct cvmx_cprix_gsm_grp_config_sel_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t gsm_grp_no                   : 2;  /**< Sub-group of [GSM_GRP_CONFIG] to configure. Each group consists of 4 sub-groups
                                                         with 4, 4, 4, and 2 groups to configure. Up to 14 AxCs may be configured to
                                                         belong to a group. */
	uint64_t gsm_grp_config               : 3;  /**< Mapping method 3 group (out of 6) to configure. */
#else
	uint64_t gsm_grp_config               : 3;
	uint64_t gsm_grp_no                   : 2;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_cprix_gsm_grp_config_sel_s cnf75xx;
};
typedef union cvmx_cprix_gsm_grp_config_sel cvmx_cprix_gsm_grp_config_sel_t;

/**
 * cvmx_cpri#_hdlc_cnt_rx_frame
 *
 * HDLC received frames
 *
 */
union cvmx_cprix_hdlc_cnt_rx_frame {
	uint64_t u64;
	struct cvmx_cprix_hdlc_cnt_rx_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t hdlc_cnt_rx_frame            : 32; /**< Number of received frames. */
#else
	uint64_t hdlc_cnt_rx_frame            : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_cnt_rx_frame_s cnf75xx;
};
typedef union cvmx_cprix_hdlc_cnt_rx_frame cvmx_cprix_hdlc_cnt_rx_frame_t;

/**
 * cvmx_cpri#_hdlc_cnt_tx_frame
 *
 * HDLC transmitted frames
 *
 */
union cvmx_cprix_hdlc_cnt_tx_frame {
	uint64_t u64;
	struct cvmx_cprix_hdlc_cnt_tx_frame_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t hdlc_cnt_tx_frame            : 32; /**< Number of transmitted frames. */
#else
	uint64_t hdlc_cnt_tx_frame            : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_cnt_tx_frame_s cnf75xx;
};
typedef union cvmx_cprix_hdlc_cnt_tx_frame cvmx_cprix_hdlc_cnt_tx_frame_t;

/**
 * cvmx_cpri#_hdlc_config
 *
 * HDLC configuration
 *
 */
union cvmx_cprix_hdlc_config {
	uint64_t u64;
	struct cvmx_cprix_hdlc_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t intr_tx_ready_block_en       : 1;  /**< 1 = Enable interrupt for TX ready block.
                                                         0 = Not enabled. */
	uint64_t intr_tx_abort_en             : 1;  /**< 1 = Enable interrupt for TX abort.
                                                         0 = Not enabled. */
	uint64_t intr_tx_ready_en             : 1;  /**< 1 = Enable interrupt for TX ready.
                                                         0 = Not enabled. */
	uint64_t intr_rx_ready_block_en       : 1;  /**< 1 = Enable interrupt for RX ready block.
                                                         0 = Not enabled. */
	uint64_t intr_rx_ready_end_en         : 1;  /**< 1 = Enable interrupt for RX ready end.
                                                         0 = Not enabled. */
	uint64_t intr_rx_abort_en             : 1;  /**< 1 = Enable interrupt for RX abort.
                                                         0 = Not enabled. */
	uint64_t intr_rx_ready_en             : 1;  /**< 1 = Enable interrupt for RX ready.
                                                         0 = Not enabled. */
	uint64_t intr_tx_en                   : 1;  /**< 1 = HDLC TX interrupt enable.
                                                         0 = Not enabled. */
	uint64_t intr_rx_en                   : 1;  /**< 1 : HDLC RX interrupt enable.
                                                         0 = Not enabled. */
	uint64_t intr_en                      : 1;  /**< 1 = HDLC global interrupt enable.
                                                         0 = Not enabled. */
	uint64_t reserved_5_9                 : 5;
	uint64_t length_check                 : 1;  /**< 1 = Enable discard of RX packet fragments (packets with length less than
                                                         64 bytes).
                                                         0 = Not enabled. */
	uint64_t reserved_2_3                 : 2;
	uint64_t little_endian_en             : 1;  /**< 1 = Use little endian byte data representation for HDLC receive and transmit
                                                         data.
                                                         0 = Use big endian byte data representation for HDLC receive and transmit
                                                         data. */
	uint64_t reserved_0_0                 : 1;
#else
	uint64_t reserved_0_0                 : 1;
	uint64_t little_endian_en             : 1;
	uint64_t reserved_2_3                 : 2;
	uint64_t length_check                 : 1;
	uint64_t reserved_5_9                 : 5;
	uint64_t intr_en                      : 1;
	uint64_t intr_rx_en                   : 1;
	uint64_t intr_tx_en                   : 1;
	uint64_t intr_rx_ready_en             : 1;
	uint64_t intr_rx_abort_en             : 1;
	uint64_t intr_rx_ready_end_en         : 1;
	uint64_t intr_rx_ready_block_en       : 1;
	uint64_t intr_tx_ready_en             : 1;
	uint64_t intr_tx_abort_en             : 1;
	uint64_t intr_tx_ready_block_en       : 1;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_cprix_hdlc_config_s       cnf75xx;
};
typedef union cvmx_cprix_hdlc_config cvmx_cprix_hdlc_config_t;

/**
 * cvmx_cpri#_hdlc_config_2
 *
 * HDLC configuration 2
 *
 */
union cvmx_cprix_hdlc_config_2 {
	uint64_t u64;
	struct cvmx_cprix_hdlc_config_2_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t crc_enable                   : 1;  /**< '"1 = Enable insertion of HDLC CRC at the end of the frame. If the bit is
                                                         not set, the CRC must be included in the injected frame. "
                                                         0 = not enabled.' */
#else
	uint64_t crc_enable                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_cprix_hdlc_config_2_s     cnf75xx;
};
typedef union cvmx_cprix_hdlc_config_2 cvmx_cprix_hdlc_config_2_t;

/**
 * cvmx_cpri#_hdlc_config_3
 *
 * HDLC configuration 3
 *
 */
union cvmx_cprix_hdlc_config_3 {
	uint64_t u64;
	struct cvmx_cprix_hdlc_config_3_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t tx_start_thr                 : 3;  /**< Transmit start threshold.When store-and-forward mode is not enabled transmission
                                                         will start when the specified number of 32-bit words have been stored in the
                                                         transmit buffer.X is equal to generic WIDTH_HDLC_BUFFor our use, WIDTH_HDLC_BUF       :
                                                         in integer := 2; */
	uint64_t reserved_2_7                 : 6;
	uint64_t rx_crc_en                    : 1;  /**< '"1 = Enable FCS validation of received packets. If check fails, packet will
                                                         be discarded. "
                                                         0 = No FCS validation. All packets are accepted.' */
	uint64_t tx_st_fwd                    : 1;  /**< 1 = Transmit store-and-forward mode. Store a full packet before start of
                                                         transmission. Packets longer than transmit buffer will be aborted.
                                                         0 = Cut-through mode. Transmission will start when buffer fill level exceeds tx_start_thr. */
#else
	uint64_t tx_st_fwd                    : 1;
	uint64_t rx_crc_en                    : 1;
	uint64_t reserved_2_7                 : 6;
	uint64_t tx_start_thr                 : 3;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_cprix_hdlc_config_3_s     cnf75xx;
};
typedef union cvmx_cprix_hdlc_config_3 cvmx_cprix_hdlc_config_3_t;

/**
 * cvmx_cpri#_hdlc_rx_control
 *
 * HDLC receive control
 *
 */
union cvmx_cprix_hdlc_rx_control {
	uint64_t u64;
	struct cvmx_cprix_hdlc_rx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t rx_discard                   : 1;  /**< 1 = Discard current HDLC RX frame.
                                                         0 = Do not discard. */
#else
	uint64_t rx_discard                   : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_cprix_hdlc_rx_control_s   cnf75xx;
};
typedef union cvmx_cprix_hdlc_rx_control cvmx_cprix_hdlc_rx_control_t;

/**
 * cvmx_cpri#_hdlc_rx_data
 *
 * HDLC receive data
 *
 */
union cvmx_cprix_hdlc_rx_data {
	uint64_t u64;
	struct cvmx_cprix_hdlc_rx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_data                      : 32; /**< HDLC RX frame data. */
#else
	uint64_t rx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_rx_data_s      cnf75xx;
};
typedef union cvmx_cprix_hdlc_rx_data cvmx_cprix_hdlc_rx_data_t;

/**
 * cvmx_cpri#_hdlc_rx_data_wait
 *
 * HDLC receive data with wait-state
 *
 */
union cvmx_cprix_hdlc_rx_data_wait {
	uint64_t u64;
	struct cvmx_cprix_hdlc_rx_data_wait_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_data                      : 32; /**< HDLC RX frame data.Wait states will be inserted until data is ready (or the
                                                         CPU time-out the operation). */
#else
	uint64_t rx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_rx_data_wait_s cnf75xx;
};
typedef union cvmx_cprix_hdlc_rx_data_wait cvmx_cprix_hdlc_rx_data_wait_t;

/**
 * cvmx_cpri#_hdlc_rx_ex_status
 *
 * HDLC extra receive status.
 *
 */
union cvmx_cprix_hdlc_rx_ex_status {
	uint64_t u64;
	struct cvmx_cprix_hdlc_rx_ex_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t mii_protocol_error           : 1;  /**< 1 = Protocol error. */
	uint64_t reserved_9_9                 : 1;
	uint64_t rx_ex_overflow               : 1;  /**< 1 = RX buffer overflow. */
	uint64_t reserved_7_7                 : 1;
	uint64_t crc_error                    : 1;  /**< 1 = CRC error. */
	uint64_t reserved_5_5                 : 1;
	uint64_t rx_error                     : 1;  /**< 1 = Abort from CPRI Layer. */
	uint64_t reserved_0_3                 : 4;
#else
	uint64_t reserved_0_3                 : 4;
	uint64_t rx_error                     : 1;
	uint64_t reserved_5_5                 : 1;
	uint64_t crc_error                    : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t rx_ex_overflow               : 1;
	uint64_t reserved_9_9                 : 1;
	uint64_t mii_protocol_error           : 1;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_cprix_hdlc_rx_ex_status_s cnf75xx;
};
typedef union cvmx_cprix_hdlc_rx_ex_status cvmx_cprix_hdlc_rx_ex_status_t;

/**
 * cvmx_cpri#_hdlc_rx_status
 *
 * HDLC receive status
 *
 */
union cvmx_cprix_hdlc_rx_status {
	uint64_t u64;
	struct cvmx_cprix_hdlc_rx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t rx_crc_error                 : 1;  /**< 1 = Frame aborted due to CRC error. */
	uint64_t rx_empty                     : 1;  /**< 1 = Read from empty buffer. */
	uint64_t rx_mii_error                 : 1;  /**< 1 = Frame aborted due to mii protocol error. */
	uint64_t reserved_9_9                 : 1;
	uint64_t rx_overflow                  : 1;  /**< 1 = Frame aborted due to Rx buffer overflow. */
	uint64_t reserved_7_7                 : 1;
	uint64_t rx_ready_block               : 1;  /**< 1 = Guaranteed 2^WIDTH_HDLC_BLOCK words HDLC RX data ready.
                                                         0 = Not ready. */
	uint64_t rx_ready_end                 : 1;  /**< 1 = EOP ready in RX buffer.
                                                         0 = Not ready. */
	uint64_t rx_length                    : 2;  /**< Length of last word in the packet.
                                                         0x0 means 1 valid byte.
                                                         0x1 means 2 valid bytes.
                                                         0x2 means 3 valid bytes.
                                                         0x3 means 4 valid bytes. */
	uint64_t rx_abort                     : 1;  /**< 1 = HDLC RX packet aborted.
                                                         0 = Not aborted. */
	uint64_t rx_eof                       : 1;  /**< 1 = HDLC RX end of packet.
                                                         0 = Not EOP. */
	uint64_t rx_ready                     : 1;  /**< 1 = HDLC RX data ready.
                                                         0 = Not ready . */
#else
	uint64_t rx_ready                     : 1;
	uint64_t rx_eof                       : 1;
	uint64_t rx_abort                     : 1;
	uint64_t rx_length                    : 2;
	uint64_t rx_ready_end                 : 1;
	uint64_t rx_ready_block               : 1;
	uint64_t reserved_7_7                 : 1;
	uint64_t rx_overflow                  : 1;
	uint64_t reserved_9_9                 : 1;
	uint64_t rx_mii_error                 : 1;
	uint64_t rx_empty                     : 1;
	uint64_t rx_crc_error                 : 1;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_cprix_hdlc_rx_status_s    cnf75xx;
};
typedef union cvmx_cprix_hdlc_rx_status cvmx_cprix_hdlc_rx_status_t;

/**
 * cvmx_cpri#_hdlc_tx_control
 *
 * HDLC transmit control
 *
 */
union cvmx_cprix_hdlc_tx_control {
	uint64_t u64;
	struct cvmx_cprix_hdlc_tx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_4_63                : 60;
	uint64_t tx_length                    : 2;  /**< Length of last word in the packet.
                                                         0x0 = means 1 valid bytes.
                                                         0x1 = means 2 valid
                                                         bytes.
                                                         0x2 = means 3 valid bytes.
                                                         0x3 = means 4 valid bytes.This field shall be
                                                         set when the tx_eof bit is asserted. */
	uint64_t tx_discard                   : 1;  /**< 1 = Discard TX packet.
                                                         0 = Do not discard. */
	uint64_t tx_eop                       : 1;  /**< 1 = Indicate that next write to TX_DATA or TX_DATA_WAIT registers will be
                                                         last word of the TX packet. Will hereafter be cleared.
                                                         0 = not last word. */
#else
	uint64_t tx_eop                       : 1;
	uint64_t tx_discard                   : 1;
	uint64_t tx_length                    : 2;
	uint64_t reserved_4_63                : 60;
#endif
	} s;
	struct cvmx_cprix_hdlc_tx_control_s   cnf75xx;
};
typedef union cvmx_cprix_hdlc_tx_control cvmx_cprix_hdlc_tx_control_t;

/**
 * cvmx_cpri#_hdlc_tx_data
 *
 * HDLC transmit data
 *
 */
union cvmx_cprix_hdlc_tx_data {
	uint64_t u64;
	struct cvmx_cprix_hdlc_tx_data_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t tx_data                      : 32; /**< HDLC TX frame data."When writing the last word of the frame, data must be aligned
                                                         to bit 31 in big endian  mode and bit 0 in little endian. " */
#else
	uint64_t tx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_tx_data_s      cnf75xx;
};
typedef union cvmx_cprix_hdlc_tx_data cvmx_cprix_hdlc_tx_data_t;

/**
 * cvmx_cpri#_hdlc_tx_data_wait
 *
 * HDLC transmit data with wait-state
 *
 */
union cvmx_cprix_hdlc_tx_data_wait {
	uint64_t u64;
	struct cvmx_cprix_hdlc_tx_data_wait_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t tx_data                      : 32; /**< HDLC TX frame data.Wait states will be inserted until transmitter is ready to
                                                         get data (or the CPU time-out the operation)."When writing the last word of
                                                         the frame, data must be aligned to bit 31 in big-endian  mode and bit 0 in little-
                                                         endian. " */
#else
	uint64_t tx_data                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_hdlc_tx_data_wait_s cnf75xx;
};
typedef union cvmx_cprix_hdlc_tx_data_wait cvmx_cprix_hdlc_tx_data_wait_t;

/**
 * cvmx_cpri#_hdlc_tx_status
 *
 * HDLC transmit status
 *
 */
union cvmx_cprix_hdlc_tx_status {
	uint64_t u64;
	struct cvmx_cprix_hdlc_tx_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t tx_ready_block               : 1;  /**< 1 = Guaranteed to get 2^WIDTH_HDLC_BLOCK words of HDLC TX data.
                                                         0 = Not ready. */
	uint64_t tx_abort                     : 1;  /**< HDLC TX packet aborted.
                                                         0 = Not aborted. */
	uint64_t tx_ready                     : 1;  /**< 1 = Ready to get HDLC TX data.
                                                         0 = Not ready. */
#else
	uint64_t tx_ready                     : 1;
	uint64_t tx_abort                     : 1;
	uint64_t tx_ready_block               : 1;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_cprix_hdlc_tx_status_s    cnf75xx;
};
typedef union cvmx_cprix_hdlc_tx_status cvmx_cprix_hdlc_tx_status_t;

/**
 * cvmx_cpri#_hfn
 */
union cvmx_cprix_hfn {
	uint64_t u64;
	struct cvmx_cprix_hfn_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t hfn                          : 8;  /**< Current hyperframe number (HFN) from alignment receive state machine. */
#else
	uint64_t hfn                          : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_cprix_hfn_s               cnf75xx;
};
typedef union cvmx_cprix_hfn cvmx_cprix_hfn_t;

/**
 * cvmx_cpri#_hw_reset
 *
 * This register is used to transmit a reset request to the RE. A reset
 * request will be generated when both [RESET_GEN_EN] and [RESET_GEN_FORCE]
 * are set to 1. The reset request will be held as long as [RESET_GEN_FORCE]
 * is set and until a reset acknowledgement has been received from the RE.
 *
 * The reset request can be aborted by clearing [RESET_GEN_EN].
 */
union cvmx_cprix_hw_reset {
	uint64_t u64;
	struct cvmx_cprix_hw_reset_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t reset_gen_done_hold          : 1;  /**< Latched value of [RESET_GEN_DONE].
                                                         0 : Reset requested not completed.
                                                         1 : Reset request completed since last read. The request completes
                                                         when the acknowledgement has been received. */
	uint64_t reset_gen_done               : 1;  /**< Indicates that transmitter has finished the reset request.
                                                         0 = Reset request not done.
                                                         1 = Reset request completed.
                                                         Note that the reset request completes when the reset acknowledgement
                                                         has been received. This bit will remain asserted until bit 0 of
                                                         Z.130.0 is cleared for three of the last five received hyperframes.
                                                         The reset completion state will be latched in [RESET_GEN_DONE_HOLD] in
                                                         case this bit is cleared before software reads the completion state. */
	uint64_t reset_detect_hold            : 1;  /**< N/A */
	uint64_t reset_detect                 : 1;  /**< N/A */
	uint64_t reserved_3_3                 : 1;
	uint64_t reset_out_en                 : 1;  /**< N/A */
	uint64_t reset_gen_force              : 1;  /**< Force generation of a reset request on the CPRI transmitter.
                                                         0 = Disable reset request.
                                                         1 = Force reset request. */
	uint64_t reset_gen_en                 : 1;  /**< Enable generation of reset requests on the CPRI transmitter using
                                                         dedicated control word Z.130.0. When enabled, [RESET_GEN_FORCE] can be
                                                         used to generate reset requests.
                                                         0 = Disable reset request.
                                                         1 = Enable reset request. */
#else
	uint64_t reset_gen_en                 : 1;
	uint64_t reset_gen_force              : 1;
	uint64_t reset_out_en                 : 1;
	uint64_t reserved_3_3                 : 1;
	uint64_t reset_detect                 : 1;
	uint64_t reset_detect_hold            : 1;
	uint64_t reset_gen_done               : 1;
	uint64_t reset_gen_done_hold          : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_cprix_hw_reset_s          cnf75xx;
};
typedef union cvmx_cprix_hw_reset cvmx_cprix_hw_reset_t;

/**
 * cvmx_cpri#_intr
 */
union cvmx_cprix_intr {
	uint64_t u64;
	struct cvmx_cprix_intr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_1_63                : 63;
	uint64_t intr_en                      : 1;  /**< CPRI module interrupt assert.1 : Assert Global Interrupt.0 :  De-assert
                                                         Global Interrupt.Remark that there are separate interrupt enables for Ethernet
                                                         and HDLC modules. */
#else
	uint64_t intr_en                      : 1;
	uint64_t reserved_1_63                : 63;
#endif
	} s;
	struct cvmx_cprix_intr_s              cnf75xx;
};
typedef union cvmx_cprix_intr cvmx_cprix_intr_t;

/**
 * cvmx_cpri#_iq_rx_buf_status#
 */
union cvmx_cprix_iq_rx_buf_statusx {
	uint64_t u64;
	struct cvmx_cprix_iq_rx_buf_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t buffer_rx_underflow          : 16; /**< One bit per AxC interface, with set bits indicating buffer underflow. */
	uint64_t buffer_rx_overflow           : 16; /**< One bit per AxC interface, with set bits indicating buffer overflow. */
#else
	uint64_t buffer_rx_overflow           : 16;
	uint64_t buffer_rx_underflow          : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_iq_rx_buf_statusx_s cnf75xx;
};
typedef union cvmx_cprix_iq_rx_buf_statusx cvmx_cprix_iq_rx_buf_statusx_t;

/**
 * cvmx_cpri#_iq_rx_buf_sync_status#
 */
union cvmx_cprix_iq_rx_buf_sync_statusx {
	uint64_t u64;
	struct cvmx_cprix_iq_rx_buf_sync_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t buffer_rx_wr_sync_error      : 16; /**< One bit per AxC interface, with set bits indicating buffer write
                                                         synchronization errors. */
	uint64_t buffer_rx_rd_sync_error      : 16; /**< One bit per AxC interface, with set bits indicating buffer read
                                                         synchronization errors. */
#else
	uint64_t buffer_rx_rd_sync_error      : 16;
	uint64_t buffer_rx_wr_sync_error      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_iq_rx_buf_sync_statusx_s cnf75xx;
};
typedef union cvmx_cprix_iq_rx_buf_sync_statusx cvmx_cprix_iq_rx_buf_sync_statusx_t;

/**
 * cvmx_cpri#_iq_tx_buf_status#
 */
union cvmx_cprix_iq_tx_buf_statusx {
	uint64_t u64;
	struct cvmx_cprix_iq_tx_buf_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t buffer_tx_underflow          : 16; /**< One bit per AxC interface, with set bits indicating buffer underflow. */
	uint64_t buffer_tx_overflow           : 16; /**< One bit per AxC interface, with set bits indicating buffer overflow. */
#else
	uint64_t buffer_tx_overflow           : 16;
	uint64_t buffer_tx_underflow          : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_iq_tx_buf_statusx_s cnf75xx;
};
typedef union cvmx_cprix_iq_tx_buf_statusx cvmx_cprix_iq_tx_buf_statusx_t;

/**
 * cvmx_cpri#_iq_tx_buf_sync_status#
 *
 * TX MAP transmitter FIFO sync buffering status.
 *
 */
union cvmx_cprix_iq_tx_buf_sync_statusx {
	uint64_t u64;
	struct cvmx_cprix_iq_tx_buf_sync_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t buffer_tx_wr_sync_error      : 16; /**< One bit per AxC interface, with set bits indicating buffer write
                                                         synchronization errors. */
	uint64_t buffer_tx_rd_sync_error      : 16; /**< One bit per AxC interface, with set bits indicating buffer read
                                                         synchronization errors. */
#else
	uint64_t buffer_tx_rd_sync_error      : 16;
	uint64_t buffer_tx_wr_sync_error      : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_iq_tx_buf_sync_statusx_s cnf75xx;
};
typedef union cvmx_cprix_iq_tx_buf_sync_statusx cvmx_cprix_iq_tx_buf_sync_statusx_t;

/**
 * cvmx_cpri#_lcv
 */
union cvmx_cprix_lcv {
	uint64_t u64;
	struct cvmx_cprix_lcv_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t cnt_lcv                      : 32; /**< Detected number of line code violation indications from the 8b/10b
                                                         decoding block. The counter will saturate at 0xFFFFFFFF. */
#else
	uint64_t cnt_lcv                      : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_lcv_s               cnf75xx;
};
typedef union cvmx_cprix_lcv cvmx_cprix_lcv_t;

/**
 * cvmx_cpri#_map_config
 */
union cvmx_cprix_map_config {
	uint64_t u64;
	struct cvmx_cprix_map_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t cpri_map_tx_tr_mode          : 1;  /**< N/A */
	uint64_t cpri_map_rx_tr_mode          : 1;  /**< N/A */
	uint64_t map_interleave_mode          : 1;  /**< 0 = I/Q data is bit-interleaved on the map interface. CPRI block does
                                                         not perform interleaving.
                                                         1 = I and Q data are separated on the map interface, and CPRI block
                                                         performs interleaving.
                                                         Must be set to 1 for correct operation, the RFIF RMAC does not perform
                                                         interleaving. */
	uint64_t reserved_2_4                 : 3;
	uint64_t map_mode                     : 2;  /**< Select the AxC mapping mode. Set to 0x0 while changing the AxC
                                                         mapping. Once the entire mapping has been programmed, set to 0x1 to
                                                         start using the new mapping. The values 0x2 and 0x3 are reserved. */
#else
	uint64_t map_mode                     : 2;
	uint64_t reserved_2_4                 : 3;
	uint64_t map_interleave_mode          : 1;
	uint64_t cpri_map_rx_tr_mode          : 1;
	uint64_t cpri_map_tx_tr_mode          : 1;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_cprix_map_config_s        cnf75xx;
};
typedef union cvmx_cprix_map_config cvmx_cprix_map_config_t;

/**
 * cvmx_cpri#_map_k_select_rx#
 *
 * This register selects which K value is used for each 32-bit segment in the
 * recieve AxC mapping table. Each register selects the K value for 32
 * segments from 32*[b] to 32*([b]+1)-1.
 *
 * Note that the K value indicates the number of basic frames spanned by each
 * AxC container block.
 */
union cvmx_cprix_map_k_select_rxx {
	uint64_t u64;
	struct cvmx_cprix_map_k_select_rxx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t map_k_select_rx              : 32; /**< Each bit i selects the K value use for the AxC(s) mapped in mapping table segment
                                                         i+b*[32].
                                                         0 = Use CPRI()_MAP_TBL_CONFIG[K0].
                                                         1 = Use CPRI()_MAP_TBL_CONFIG[K1]. */
#else
	uint64_t map_k_select_rx              : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_map_k_select_rxx_s  cnf75xx;
};
typedef union cvmx_cprix_map_k_select_rxx cvmx_cprix_map_k_select_rxx_t;

/**
 * cvmx_cpri#_map_k_select_tx#
 *
 * This register selects which K value is used for each 32-bit segment in the
 * recieve AxC mapping table. Each register selects the K value for 32
 * segments from 32*[b] to 32*([b]+1)-1.
 *
 * Note that the K value indicates the number of basic frames spanned by each
 * AxC container block.
 */
union cvmx_cprix_map_k_select_txx {
	uint64_t u64;
	struct cvmx_cprix_map_k_select_txx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t map_k_select_tx              : 32; /**< Each bit i selects the K value use for the AxC(s) mapped in mapping table segment
                                                         i+b*[32].
                                                         0 = Use CPRI()_MAP_TBL_CONFIG[K0].
                                                         1 = Use CPRI()_MAP_TBL_CONFIG[K1]. */
#else
	uint64_t map_k_select_tx              : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_map_k_select_txx_s  cnf75xx;
};
typedef union cvmx_cprix_map_k_select_txx cvmx_cprix_map_k_select_txx_t;

/**
 * cvmx_cpri#_map_offset_rx
 *
 * This register controls the receive map offset. The RFIF register
 * RFIF_RETARD should be used to control receiver synchronization and this
 * register should be left at its default value.
 */
union cvmx_cprix_map_offset_rx {
	uint64_t u64;
	struct cvmx_cprix_map_offset_rx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t map_rx_hf_resync             : 1;  /**< This bit enables synchronization every hyperframe instead of every radio frame.
                                                         When this bit is set, [MAP_RX_OFFSET_Z] is ignored. */
	uint64_t map_rx_offset_z              : 8;  /**< Hyperframe number for start of RX side AxC container block. */
	uint64_t map_rx_offset_x              : 8;  /**< Basic frame number for start of RX side AxC container block. */
#else
	uint64_t map_rx_offset_x              : 8;
	uint64_t map_rx_offset_z              : 8;
	uint64_t map_rx_hf_resync             : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_cprix_map_offset_rx_s     cnf75xx;
};
typedef union cvmx_cprix_map_offset_rx cvmx_cprix_map_offset_rx_t;

/**
 * cvmx_cpri#_map_offset_tx
 *
 * This register determines the alignment of the transmitter RFP relative to
 * the CPRI hyperframe and basic frame numbers.
 */
union cvmx_cprix_map_offset_tx {
	uint64_t u64;
	struct cvmx_cprix_map_offset_tx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t map_tx_hf_resync             : 1;  /**< This bit enables synchronization every hyperframe instead of every radio frame.
                                                         When this bit is set [MAP_TX_OFFSET_Z] will be ignored. */
	uint64_t map_tx_offset_z              : 8;  /**< Hyperframe number for start of TX side A&C container block. */
	uint64_t map_tx_offset_x              : 8;  /**< Basic frame number for start of TX side A&C container block. */
#else
	uint64_t map_tx_offset_x              : 8;
	uint64_t map_tx_offset_z              : 8;
	uint64_t map_tx_hf_resync             : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_cprix_map_offset_tx_s     cnf75xx;
};
typedef union cvmx_cprix_map_offset_tx cvmx_cprix_map_offset_tx_t;

/**
 * cvmx_cpri#_map_smpl_cfg_rx
 *
 * This register configures AxC sample with used for the reciever AxC
 * mapping.
 *
 * To read the current sample width for an AxC, first write this register
 * with [AXC] set to the desired AxC, and [CFG] cleared to zero. Then read
 * back the [WIDTH] value.
 */
union cvmx_cprix_map_smpl_cfg_rx {
	uint64_t u64;
	struct cvmx_cprix_map_smpl_cfg_rx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t axc                          : 5;  /**< Select the AxC. */
	uint64_t cfg                          : 1;  /**< When this bit is written, the sample width for [AXC] will be updated
                                                         to [WIDTH]. The bit is cleared after the write. When this bit is
                                                         clear, no action is taken on the write. */
	uint64_t reserved_5_6                 : 2;
	uint64_t width                        : 5;  /**< AxC sample width. */
#else
	uint64_t width                        : 5;
	uint64_t reserved_5_6                 : 2;
	uint64_t cfg                          : 1;
	uint64_t axc                          : 5;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_cprix_map_smpl_cfg_rx_s   cnf75xx;
};
typedef union cvmx_cprix_map_smpl_cfg_rx cvmx_cprix_map_smpl_cfg_rx_t;

/**
 * cvmx_cpri#_map_smpl_cfg_tx
 *
 * This register configures AxC sample with used for the transmit AxC
 * mapping.
 *
 * To read the current sample width for an AxC, first write this register
 * with [AXC] set to the desired AxC, and [CFG] cleared to zero. Then read
 * back the [WIDTH] value.
 */
union cvmx_cprix_map_smpl_cfg_tx {
	uint64_t u64;
	struct cvmx_cprix_map_smpl_cfg_tx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t axc                          : 5;  /**< Select the AxC. */
	uint64_t cfg                          : 1;  /**< When this bit is written, the sample width for [AXC] will be updated
                                                         to [WIDTH]. The bit is cleared after the write. When this bit is
                                                         clear, no action is taken on the write. */
	uint64_t reserved_5_6                 : 2;
	uint64_t width                        : 5;  /**< AxC sample width. */
#else
	uint64_t width                        : 5;
	uint64_t reserved_5_6                 : 2;
	uint64_t cfg                          : 1;
	uint64_t axc                          : 5;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_cprix_map_smpl_cfg_tx_s   cnf75xx;
};
typedef union cvmx_cprix_map_smpl_cfg_tx cvmx_cprix_map_smpl_cfg_tx_t;

/**
 * cvmx_cpri#_map_tbl_config
 *
 * This register stores two possible K values that can be used for mapping
 * AxCs. The K parameter specifies the number of basic frames spanned by an
 * AxC container block.
 *
 * The AxC mapping selects between using [K0] or [K1] using the
 * CPRI()_MAP_K_SELECT_RX and CPRI()_MAP_K_SELECT_TX register.
 */
union cvmx_cprix_map_tbl_config {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t k1                           : 6;  /**< Number of basic frames in an AxC container block. */
	uint64_t reserved_6_15                : 10;
	uint64_t k0                           : 6;  /**< Number of basic frames in an AxC container block. */
#else
	uint64_t k0                           : 6;
	uint64_t reserved_6_15                : 10;
	uint64_t k1                           : 6;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_cprix_map_tbl_config_s    cnf75xx;
};
typedef union cvmx_cprix_map_tbl_config cvmx_cprix_map_tbl_config_t;

/**
 * cvmx_cpri#_map_tbl_index
 *
 * This register specifies which mapping table segment will be read/written by
 * accesses to any of the registers:
 * * CPRI()_MAP_TBL_RX0
 * * CPRI()_MAP_TBL_RX1
 * * CPRI()_MAP_TBL_TX0
 * * CPRI()_MAP_TBL_TX1
 *
 * Note that a write to CPRI()_MAP_TBL_RX1 or CPRI()_MAP_TBL_TX1 will
 * automatically increment this register to one.
 */
union cvmx_cprix_map_tbl_index {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_index_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_11_63               : 53;
	uint64_t map_conf_index               : 11; /**< Mapping table index. */
#else
	uint64_t map_conf_index               : 11;
	uint64_t reserved_11_63               : 53;
#endif
	} s;
	struct cvmx_cprix_map_tbl_index_s     cnf75xx;
};
typedef union cvmx_cprix_map_tbl_index cvmx_cprix_map_tbl_index_t;

/**
 * cvmx_cpri#_map_tbl_rx0
 */
union cvmx_cprix_map_tbl_rx0 {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_rx0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t width1                       : 5;  /**< Bit width of entry 1 divided by two. */
	uint64_t reserved_25_26               : 2;
	uint64_t position1                    : 3;  /**< Start position of entry 1 divided by two. The start position is
                                                         relative to bit 0xA (e.g., [POSITION1] = 1 results in entry 1 starting
                                                         at bit 0xC). */
	uint64_t axc1                         : 5;  /**< AxC number mapped by entry 1. */
	uint64_t enable1                      : 1;  /**< Enable entry 1. */
	uint64_t width0                       : 5;  /**< Bit width of entry 0 divided by two. */
	uint64_t reserved_9_10                : 2;
	uint64_t position0                    : 3;  /**< Start position of entry 0 divided by two. The start position is
                                                         relative to bit 0x0 (e.g., [POSITION0] = 1 results in entry 0 starting
                                                         at bit 0x2). */
	uint64_t axc0                         : 5;  /**< AxC number mapped by entry 0. */
	uint64_t enable0                      : 1;  /**< Enable entry 0. */
#else
	uint64_t enable0                      : 1;
	uint64_t axc0                         : 5;
	uint64_t position0                    : 3;
	uint64_t reserved_9_10                : 2;
	uint64_t width0                       : 5;
	uint64_t enable1                      : 1;
	uint64_t axc1                         : 5;
	uint64_t position1                    : 3;
	uint64_t reserved_25_26               : 2;
	uint64_t width1                       : 5;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_map_tbl_rx0_s       cnf75xx;
};
typedef union cvmx_cprix_map_tbl_rx0 cvmx_cprix_map_tbl_rx0_t;

/**
 * cvmx_cpri#_map_tbl_rx1
 *
 * The receive mapping table configuration for entry 2 in the segment selected by
 * CPRI()_MAP_TBL_INDEX.
 *
 * When this register is written, CPRI()_MAP_TBL_INDEX automatically
 * increments by one.
 */
union cvmx_cprix_map_tbl_rx1 {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_rx1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t width2                       : 5;  /**< Bit width of entry 2 divided by two. */
	uint64_t reserved_9_10                : 2;
	uint64_t position2                    : 3;  /**< Start position of entry 2 divided by two. The start position is
                                                         relative to bit 0x14 (e.g., [POSITION2] = 1 results in entry 2 starting
                                                         at bit 0x16). */
	uint64_t axc2                         : 5;  /**< AxC number mapped by entry 2. */
	uint64_t enable2                      : 1;  /**< Enable entry 2. */
#else
	uint64_t enable2                      : 1;
	uint64_t axc2                         : 5;
	uint64_t position2                    : 3;
	uint64_t reserved_9_10                : 2;
	uint64_t width2                       : 5;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_cprix_map_tbl_rx1_s       cnf75xx;
};
typedef union cvmx_cprix_map_tbl_rx1 cvmx_cprix_map_tbl_rx1_t;

/**
 * cvmx_cpri#_map_tbl_tx0
 */
union cvmx_cprix_map_tbl_tx0 {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_tx0_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t width1                       : 5;  /**< Bit width of entry 1 divided by two. */
	uint64_t position1                    : 5;  /**< Start position of entry 1 divided by two. The start position is
                                                         relative to bit 0xA (e.g., [POSITION1] = 1 results in entry 1 starting
                                                         at bit 0xC). */
	uint64_t axc1                         : 5;  /**< AxC number mapped by entry 1. */
	uint64_t enable1                      : 1;  /**< Enable entry 1. */
	uint64_t width0                       : 5;  /**< Bit width of entry 0 divided by two. */
	uint64_t position0                    : 5;  /**< Start position of entry 0 divided by two. The start position is
                                                         relative to bit 0x0 (e.g., [POSITION0] = 1 results in entry 0 starting
                                                         at bit 0x2). */
	uint64_t axc0                         : 5;  /**< AxC number mapped by entry 0. */
	uint64_t enable0                      : 1;  /**< Enable entry 0. */
#else
	uint64_t enable0                      : 1;
	uint64_t axc0                         : 5;
	uint64_t position0                    : 5;
	uint64_t width0                       : 5;
	uint64_t enable1                      : 1;
	uint64_t axc1                         : 5;
	uint64_t position1                    : 5;
	uint64_t width1                       : 5;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_map_tbl_tx0_s       cnf75xx;
};
typedef union cvmx_cprix_map_tbl_tx0 cvmx_cprix_map_tbl_tx0_t;

/**
 * cvmx_cpri#_map_tbl_tx1
 *
 * The transmit mapping table configuration for entry 2 in the segment selected by
 * CPRI()_MAP_TBL_INDEX.
 *
 * When this register is written, CPRI()_MAP_TBL_INDEX automatically
 * increments by one.
 */
union cvmx_cprix_map_tbl_tx1 {
	uint64_t u64;
	struct cvmx_cprix_map_tbl_tx1_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t width2                       : 5;  /**< Bit width of entry 2 divided by two. */
	uint64_t position2                    : 5;  /**< Start position of entry 2 divided by two. The start position is
                                                         relative to bit 0x14 (e.g., [POSITION2] = 1 results in entry 2 starting
                                                         at bit 0x16). */
	uint64_t axc2                         : 5;  /**< AxC number mapped by entry 2. */
	uint64_t enable2                      : 1;  /**< Enable entry 2. */
#else
	uint64_t enable2                      : 1;
	uint64_t axc2                         : 5;
	uint64_t position2                    : 5;
	uint64_t width2                       : 5;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_cprix_map_tbl_tx1_s       cnf75xx;
};
typedef union cvmx_cprix_map_tbl_tx1 cvmx_cprix_map_tbl_tx1_t;

/**
 * cvmx_cpri#_phy_loop
 *
 * Loopback is only available in slave port mode (not supported in CNF75XX).
 *
 */
union cvmx_cprix_phy_loop {
	uint64_t u64;
	struct cvmx_cprix_phy_loop_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_5_63                : 59;
	uint64_t loop_resync                  : 1;  /**< Reset resynchronization  detected. This will typically happen when receive clock
                                                         and system clock do not have exact same frequency.1 : Resynchronization
                                                         detected.0 : Resynchronization not detected. */
	uint64_t reserved_2_3                 : 2;
	uint64_t loop_mode                    : 2;  /**< PHY loop mode configuration:"00" : No loop."01" : Parallel 10bits Loop before
                                                         8b10b decoding."10" : Parallel 8bits Loop after 8b10b decoding."11" : not used. */
#else
	uint64_t loop_mode                    : 2;
	uint64_t reserved_2_3                 : 2;
	uint64_t loop_resync                  : 1;
	uint64_t reserved_5_63                : 59;
#endif
	} s;
	struct cvmx_cprix_phy_loop_s          cnf75xx;
};
typedef union cvmx_cprix_phy_loop cvmx_cprix_phy_loop_t;

/**
 * cvmx_cpri#_prbs_config
 *
 * This register configures transmission and validation of PRBS patterns. The
 * PRBS patterns can be used for diagnostics with external loop-backs, or to
 * support manual receiver tunning if necessary.
 *
 * When [PRBS_MODE] is set to any value other than 0x0, the transmitted IQ
 * samples for all AxCs will be replaced with a test pattern. When configured, the AxC
 * sample width must be set to 8-bits, and the AxC mapping must be configured
 * accordingly.
 *
 * On the receive side, CPRI will validate the received test pattern and
 * record the validation status in CPRI()_PRBS_STATUS.
 */
union cvmx_cprix_prbs_config {
	uint64_t u64;
	struct cvmx_cprix_prbs_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t prbs_mode                    : 2;  /**< Select the PRBS test pattern:
                                                         0x0 : Normal mode (I/Q samples).
                                                         0x1 : Incrementing pattern from 0 through 2559, transmitted as a
                                                         16-bit value using both the I and Q part.
                                                         0x2 : PRBS 2**23-1 inverted. Separate PRBS sequences are mapped into I
                                                         and Q samples.
                                                         0x3 : All zeros.
                                                         Note that the PRBS mode is common for all AxC interfaces. */
#else
	uint64_t prbs_mode                    : 2;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_cprix_prbs_config_s       cnf75xx;
};
typedef union cvmx_cprix_prbs_config cvmx_cprix_prbs_config_t;

/**
 * cvmx_cpri#_prbs_status#
 *
 * This register reports whether the receive PRBS pattern was valid or had any errors.
 * CPRI()_PRBS_STATUS(0) reports status for AxCs 0-15, and CPRI()_PRBS_STATUS(1) reports status
 * for AxCs 16-31.
 * [PRBS_ERROR] indicates which AxCs had errors since the last read of the register, and
 * [PRBS_VALID] indicats which AxCs have activity on the receive interface.
 * Correct reception of the PRBS pattern is indicated for AxCs with the [PRBS_VALID] bit set and
 * the [PRBS_ERROR] bit cleared.
 * The valid and error bits are latched in between reads. Thus, it possible for both the error
 * and valid bits to be set for the same AxC (when there were old errors for an AxC that is now
 * valid, or vice-versa). In such cases, software should re-read the register to determine the
 * latest status.
 */
union cvmx_cprix_prbs_statusx {
	uint64_t u64;
	struct cvmx_cprix_prbs_statusx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t prbs_error                   : 16; /**< One bit per AxC indicates PRBS error detected on the receive map interface.
                                                         0 : No PRBS errors since last register read.
                                                         1 : PRBS error since last regiser read. */
	uint64_t prbs_valid                   : 16; /**< One bit per AxC indicates activity on the receive map interface.
                                                         0 : Not valid indication.
                                                         1 : Valid indication. */
#else
	uint64_t prbs_valid                   : 16;
	uint64_t prbs_error                   : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_prbs_statusx_s      cnf75xx;
};
typedef union cvmx_cprix_prbs_statusx cvmx_cprix_prbs_statusx_t;

/**
 * cvmx_cpri#_round_delay
 */
union cvmx_cprix_round_delay {
	uint64_t u64;
	struct cvmx_cprix_round_delay_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_20_63               : 44;
	uint64_t rx_round_trip_delay          : 20; /**< Measured round trip delay in clock periods of the main clock between
                                                         the CPRI TX RFP and the CPRI RX RFP. The delay value has a resolution
                                                         of 40 encoded bits (i.e., with 8b/10b encoding). */
#else
	uint64_t rx_round_trip_delay          : 20;
	uint64_t reserved_20_63               : 44;
#endif
	} s;
	struct cvmx_cprix_round_delay_s       cnf75xx;
};
typedef union cvmx_cprix_round_delay cvmx_cprix_round_delay_t;

/**
 * cvmx_cpri#_rx_buf_resync_cnt
 */
union cvmx_cprix_rx_buf_resync_cnt {
	uint64_t u64;
	struct cvmx_cprix_rx_buf_resync_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t resync_max_cnt               : 16; /**< Number of times the buffer has reset due to fill level
                                                         becoming more than CPRI()_RX_BUF_THRESHOLDS[MAX_THRESHOLD]. */
	uint64_t resync_min_cnt               : 16; /**< Number of times the buffer has reset due to fill level
                                                         becoming less than CPRI()_RX_BUF_THRESHOLDS[MIN_THRESHOLD]. */
#else
	uint64_t resync_min_cnt               : 16;
	uint64_t resync_max_cnt               : 16;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_rx_buf_resync_cnt_s cnf75xx;
};
typedef union cvmx_cprix_rx_buf_resync_cnt cvmx_cprix_rx_buf_resync_cnt_t;

/**
 * cvmx_cpri#_rx_buf_thres
 *
 * This register specifies thresholds for the receiver buffer fill level.
 * When the buffer level goes beyond either fill level, it will re-align to
 * the level specified by CPRI()_RX_DELAY_CTRL[RX_BUF_INT_DELAY].
 *
 * Automatic resynchronization events are counted in CPRI(0..5)_RX_BUF_RESYNC_CNT.
 *
 * Buffer levels are specified in 32-bit decoded words (i.e., after 8b/10b
 * decoding).
 */
union cvmx_cprix_rx_buf_thres {
	uint64_t u64;
	struct cvmx_cprix_rx_buf_thres_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_26_63               : 38;
	uint64_t max_threshold                : 10; /**< The maximum fill level at which the buffer can operate.
                                                         If the fill level goes above this, the buffer will resync. */
	uint64_t reserved_10_15               : 6;
	uint64_t min_threshold                : 10; /**< The minimum fill level at which the buffer can operate.
                                                         If the fill level goes below this, the buffer will resync */
#else
	uint64_t min_threshold                : 10;
	uint64_t reserved_10_15               : 6;
	uint64_t max_threshold                : 10;
	uint64_t reserved_26_63               : 38;
#endif
	} s;
	struct cvmx_cprix_rx_buf_thres_s      cnf75xx;
};
typedef union cvmx_cprix_rx_buf_thres cvmx_cprix_rx_buf_thres_t;

/**
 * cvmx_cpri#_rx_ctrl
 */
union cvmx_cprix_rx_ctrl {
	uint64_t u64;
	struct cvmx_cprix_rx_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t rx_control_data              : 8;  /**< Reads the last received CPRI control byte from byte y in CPRI
                                                         hyper frame position Z.x.0, where x is specified by
                                                         CPRI()_CTRL_INDEX[CPRI_CTRL_INDEX], and y is specified by
                                                         CPRI()_CTRL_INDEX[CPRI_CTRL_SEL]. */
#else
	uint64_t rx_control_data              : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_cprix_rx_ctrl_s           cnf75xx;
};
typedef union cvmx_cprix_rx_ctrl cvmx_cprix_rx_ctrl_t;

/**
 * cvmx_cpri#_rx_delay
 */
union cvmx_cprix_rx_delay {
	uint64_t u64;
	struct cvmx_cprix_rx_delay_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_23_63               : 41;
	uint64_t rx_align_delay               : 7;  /**< Bit delay in 8b10b comma alignment. The resolution is bit periods
                                                         before 8b/10b decoding. The maximum value is 39. */
	uint64_t reserved_12_15               : 4;
	uint64_t rx_buf_delay                 : 10; /**< Current receive buffer delay, with resolution of 32-bit words after 8b/10b
                                                         decoding. */
	uint64_t rx_byte_delay                : 2;  /**< Byte alignment delay, with a resolution of 8-bit words after 8b/10b decoding. */
#else
	uint64_t rx_byte_delay                : 2;
	uint64_t rx_buf_delay                 : 10;
	uint64_t reserved_12_15               : 4;
	uint64_t rx_align_delay               : 7;
	uint64_t reserved_23_63               : 41;
#endif
	} s;
	struct cvmx_cprix_rx_delay_s          cnf75xx;
};
typedef union cvmx_cprix_rx_delay cvmx_cprix_rx_delay_t;

/**
 * cvmx_cpri#_rx_delay_ctrl
 */
union cvmx_cprix_rx_delay_ctrl {
	uint64_t u64;
	struct cvmx_cprix_rx_delay_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_17_63               : 47;
	uint64_t rx_buf_resync                : 1;  /**< Write one to force re-alignment of the receive buffer to the delay
                                                         specified by [RX_BUF_INT_DELAY]. */
	uint64_t reserved_10_15               : 6;
	uint64_t rx_buf_int_delay             : 10; /**< Receive buffer will always be re-aligned with this initial buffer
                                                         delay. The delay is specified in number of 32-bit words (after 8b/10b
                                                         decoding). If the buffer fill level reaches the upper or lower
                                                         thresholds specified in CPRI(0..5)_RX_BUF_THRES, then the receive
                                                         buffer will automatically force re-alignment to [RX_BUF_INT_DELAY]. */
#else
	uint64_t rx_buf_int_delay             : 10;
	uint64_t reserved_10_15               : 6;
	uint64_t rx_buf_resync                : 1;
	uint64_t reserved_17_63               : 47;
#endif
	} s;
	struct cvmx_cprix_rx_delay_ctrl_s     cnf75xx;
};
typedef union cvmx_cprix_rx_delay_ctrl cvmx_cprix_rx_delay_ctrl_t;

/**
 * cvmx_cpri#_rx_scr_seed
 */
union cvmx_cprix_rx_scr_seed {
	uint64_t u64;
	struct cvmx_cprix_rx_scr_seed_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t rx_scr_ena                   : 1;  /**< When set, indicates that scrambling is enabled in the received CPRI
                                                         stream. The setting is detected from the recevied protocol version. */
	uint64_t rx_scr_seed                  : 31; /**< The receiver scrambler seed value, as detected from incoming data.
                                                         This seed is only valid when [RX_SCR_ENA]=1. */
#else
	uint64_t rx_scr_seed                  : 31;
	uint64_t rx_scr_ena                   : 1;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
	struct cvmx_cprix_rx_scr_seed_s       cnf75xx;
};
typedef union cvmx_cprix_rx_scr_seed cvmx_cprix_rx_scr_seed_t;

/**
 * cvmx_cpri#_serdes_config
 */
union cvmx_cprix_serdes_config {
	uint64_t u64;
	struct cvmx_cprix_serdes_config_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t swap_serdes_rx               : 1;  /**< When set, swap the bit order on the SerDes receive interface. */
	uint64_t swap_serdes_tx               : 1;  /**< When set, swap the bit order on the SerDes transmit interface. */
#else
	uint64_t swap_serdes_tx               : 1;
	uint64_t swap_serdes_rx               : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_cprix_serdes_config_s     cnf75xx;
};
typedef union cvmx_cprix_serdes_config cvmx_cprix_serdes_config_t;

/**
 * cvmx_cpri#_start_offset_rx
 *
 * This register controls when the received CPRI samples are sent to the RFIF
 * RMAC. Receive timing alignment should be controlled using the RFIF_RETARD
 * register, and this register should be left at its default value.
 */
union cvmx_cprix_start_offset_rx {
	uint64_t u64;
	struct cvmx_cprix_start_offset_rx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_25_63               : 39;
	uint64_t start_rx_hf_resync           : 1;  /**< This bit enables synchronization every hyperframe instead of every radio frame.
                                                         When this bit is set, [START_RX_OFFSET_Z] will be ignored. */
	uint64_t reserved_22_23               : 2;
	uint64_t start_rx_offset_seq          : 6;  /**< Sequence number for start of cpri_rx_start synchronization output. Valid values
                                                         are 0 to 4*RATE-1. */
	uint64_t start_rx_offset_z            : 8;  /**< Hyperframe  number  for  start  of  cpri_rx_start synchronization output. */
	uint64_t start_rx_offset_x            : 8;  /**< Basic frame number for start of cpri_rx_start synchronization output. */
#else
	uint64_t start_rx_offset_x            : 8;
	uint64_t start_rx_offset_z            : 8;
	uint64_t start_rx_offset_seq          : 6;
	uint64_t reserved_22_23               : 2;
	uint64_t start_rx_hf_resync           : 1;
	uint64_t reserved_25_63               : 39;
#endif
	} s;
	struct cvmx_cprix_start_offset_rx_s   cnf75xx;
};
typedef union cvmx_cprix_start_offset_rx cvmx_cprix_start_offset_rx_t;

/**
 * cvmx_cpri#_start_offset_tx
 *
 * This register controls when CPRI requests TX samples from the RFIF RMAC
 * relative to the TX RFP. This register should be programmed to request
 * samples 4 basic frames before the time specified in CPRI()_MAP_OFFSET_TX.
 *
 * For example, if CPRI()_MAP_OFFSET_TX[MAP_TX_OFFSET_Z]=0 and
 * CPRI()_MAP_OFFSET_TX[MAP_TX_OFFSET_X]=0, then software should program
 * [START_TX_OFFSET_Z]=149 and [START_TX_OFFSET_X]=251.
 */
union cvmx_cprix_start_offset_tx {
	uint64_t u64;
	struct cvmx_cprix_start_offset_tx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_25_63               : 39;
	uint64_t start_tx_hf_resync           : 1;  /**< This bit enables synchronization every hyperframe instead of every radio frame.
                                                         When this bit is set, [START_TX_OFFSET_Z] will be ignored. */
	uint64_t reserved_22_23               : 2;
	uint64_t start_tx_offset_seq          : 6;  /**< Sequence  number  when CPRI should start requesting TX samples from
                                                         RFIF RMAC.  Valid values are 0 to 4*RATE-1. */
	uint64_t start_tx_offset_z            : 8;  /**< Hyperframe  number  when CPRI should start requesting TX samples from
                                                         RFIF RMAC. */
	uint64_t start_tx_offset_x            : 8;  /**< Basic frame  number  when CPRI should start requesting TX samples from
                                                         RFIF RMAC. */
#else
	uint64_t start_tx_offset_x            : 8;
	uint64_t start_tx_offset_z            : 8;
	uint64_t start_tx_offset_seq          : 6;
	uint64_t reserved_22_23               : 2;
	uint64_t start_tx_hf_resync           : 1;
	uint64_t reserved_25_63               : 39;
#endif
	} s;
	struct cvmx_cprix_start_offset_tx_s   cnf75xx;
};
typedef union cvmx_cprix_start_offset_tx cvmx_cprix_start_offset_tx_t;

/**
 * cvmx_cpri#_status
 */
union cvmx_cprix_status {
	uint64_t u64;
	struct cvmx_cprix_status_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_12_63               : 52;
	uint64_t rx_rfp_hold                  : 1;  /**< Radio frame pulse received. This bit will be asserted every 10 ms.
                                                         0 : RFP has not been detected since last read.
                                                         1 : RFP has been detected since last read. */
	uint64_t rx_freq_alarm_hold           : 1;  /**< When this bit is set, it indicates that the CPRI receive clock is not
                                                         synchronous with the CPRI clock (which runs at SerDes lane rate / 40).
                                                         This bit is cleared on a read, and re-asserted each time the phase of
                                                         the receive clock has changed more than 4 clock periods of the CPRI
                                                         clock in a 10 ms period. */
	uint64_t rx_state_hold                : 1;  /**< Receive state hold register.
                                                         0 = Loss of SYNC has not occurred.
                                                         1 = Loss of SYNC has occurred (i.e., [RX_STATE] has been 0 since last
                                                         read of CPRI()_STATUS). */
	uint64_t rx_los_hold                  : 1;  /**< Receive loss-of-signal (LOS) hold register.
                                                         0 = LOS has not occured.
                                                         1 = LOS has occured since last read of CPRI()_STATUS. */
	uint64_t reserved_4_7                 : 4;
	uint64_t rx_bfn_state                 : 1;  /**< Radio frame alignment state:
                                                         0 = BFN alignment not detected.
                                                         1 = BFN alignment detected. */
	uint64_t rx_hfn_state                 : 1;  /**< Hyper-frame number alignment state:
                                                         0 = HFN alignment not detected.
                                                         1 = HFN alignment detected. */
	uint64_t rx_state                     : 1;  /**< Overal receive synchronization state. When set, indicates that the
                                                         receive state, HFN state, and BFN state are all okay. This bit is
                                                         updated continuously, and any loss of synchronization is latched in
                                                         [RX_STATE_HOLD].
                                                         0 = No global receive SYNC.
                                                         1 = Global receive state is SYNC. */
	uint64_t rx_los                       : 1;  /**< Receive LOS (loss of signal) indication. Indicates either excessive 8B/10B violations
                                                         (>15) or external indication of loss of signal from optical module
                                                         0 = LOS not detected.
                                                         1 = LOS detected. */
#else
	uint64_t rx_los                       : 1;
	uint64_t rx_state                     : 1;
	uint64_t rx_hfn_state                 : 1;
	uint64_t rx_bfn_state                 : 1;
	uint64_t reserved_4_7                 : 4;
	uint64_t rx_los_hold                  : 1;
	uint64_t rx_state_hold                : 1;
	uint64_t rx_freq_alarm_hold           : 1;
	uint64_t rx_rfp_hold                  : 1;
	uint64_t reserved_12_63               : 52;
#endif
	} s;
	struct cvmx_cprix_status_s            cnf75xx;
};
typedef union cvmx_cprix_status cvmx_cprix_status_t;

/**
 * cvmx_cpri#_tx_control
 */
union cvmx_cprix_tx_control {
	uint64_t u64;
	struct cvmx_cprix_tx_control_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_3_63                : 61;
	uint64_t tx_rfp_tolerance             : 2;  /**< Set tolerance for the transmit RFP pulse. If the synchronization pulse
                                                         does not coincide with the internal frame counters within the allowed
                                                         tolerance, then the internal frame counters are updated automatically.
                                                         0x0 = No tolerance. Syncrhonization pulse must coincide with internal
                                                         counters.
                                                         0x1 = Tolerance of +/- 1 cycle.
                                                         0x2 = Tolerance of +/- 2 cycle.
                                                         0x3 = Tolerance of +/- 3 cycle. */
	uint64_t tx_reset_bfn                 : 1;  /**< Set CPRI TX BFN (radio frame number) value to 0 starting at the next radio frame boundary.
                                                         This bit will clear after each write. */
#else
	uint64_t tx_reset_bfn                 : 1;
	uint64_t tx_rfp_tolerance             : 2;
	uint64_t reserved_3_63                : 61;
#endif
	} s;
	struct cvmx_cprix_tx_control_s        cnf75xx;
};
typedef union cvmx_cprix_tx_control cvmx_cprix_tx_control_t;

/**
 * cvmx_cpri#_tx_ctrl
 *
 * CPRI transmit control words
 *
 */
union cvmx_cprix_tx_ctrl {
	uint64_t u64;
	struct cvmx_cprix_tx_ctrl_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_9_63                : 55;
	uint64_t tx_control_insert            : 1;  /**< Enable insertion of this control byte in subsequent transmissions.
                                                         Note that CPRI()_CONFIG[TX_CTRL_INSERT_EN] must also be set to 1
                                                         before this control word will be transmitted.
                                                         0 = Disable control word transmission.
                                                         1 = Enable control word transmission. */
	uint64_t tx_control_data              : 8;  /**< Specifies the CPRI control byte to by transmitted at byte y in CPRI
                                                         hyper frame position Z.x.0, where x is specified by
                                                         CPRI()_CTRL_INDEX[CPRI_CTRL_INDEX], and y is specified by
                                                         CPRI()_CTRL_INDEX[CPRI_CTRL_SEL]. */
#else
	uint64_t tx_control_data              : 8;
	uint64_t tx_control_insert            : 1;
	uint64_t reserved_9_63                : 55;
#endif
	} s;
	struct cvmx_cprix_tx_ctrl_s           cnf75xx;
};
typedef union cvmx_cprix_tx_ctrl cvmx_cprix_tx_ctrl_t;

/**
 * cvmx_cpri#_tx_prot_ver
 */
union cvmx_cprix_tx_prot_ver {
	uint64_t u64;
	struct cvmx_cprix_tx_prot_ver_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t tx_prot_ver                  : 8;  /**< Transmit protocol and version mapped into Z.2.0. Note that scrambling is disabled for
                                                         values
                                                         less than 0x2, and scrambling can never be enabled for line rates of 3.072 Gbps or less. */
#else
	uint64_t tx_prot_ver                  : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_cprix_tx_prot_ver_s       cnf75xx;
};
typedef union cvmx_cprix_tx_prot_ver cvmx_cprix_tx_prot_ver_t;

/**
 * cvmx_cpri#_tx_scr_seed
 *
 * Transmit scrambler seed
 *
 */
union cvmx_cprix_tx_scr_seed {
	uint64_t u64;
	struct cvmx_cprix_tx_scr_seed_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_31_63               : 33;
	uint64_t tx_scr_seed                  : 31; /**< Seed for TX scrambler. Setting the seed to 0x0 disables the scrambler.
                                                         Scrambling is automatically disabled when
                                                         CPRI()_TX_PROT_VER[TX_PROT_VER]=1, and scrambling must be disabled if
                                                         the line rate is lower than 4.915 Gbps. */
#else
	uint64_t tx_scr_seed                  : 31;
	uint64_t reserved_31_63               : 33;
#endif
	} s;
	struct cvmx_cprix_tx_scr_seed_s       cnf75xx;
};
typedef union cvmx_cprix_tx_scr_seed cvmx_cprix_tx_scr_seed_t;

/**
 * cvmx_cpri#_tx_sync_delay
 */
union cvmx_cprix_tx_sync_delay {
	uint64_t u64;
	struct cvmx_cprix_tx_sync_delay_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_16_63               : 48;
	uint64_t tx_rfp_sync_delay            : 16; /**< Internal delay for radio frame synchronization pulse. Delay is programmed in
                                                         CPRI clock cycles (the CPRI clock runs at 1/40 of the SerDes lane rate). */
#else
	uint64_t tx_rfp_sync_delay            : 16;
	uint64_t reserved_16_63               : 48;
#endif
	} s;
	struct cvmx_cprix_tx_sync_delay_s     cnf75xx;
};
typedef union cvmx_cprix_tx_sync_delay cvmx_cprix_tx_sync_delay_t;

#endif
