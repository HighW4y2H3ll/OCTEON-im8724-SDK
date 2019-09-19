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
 * cvmx-mpix-defs.h
 *
 * Configuration and status register (CSR) type definitions for
 * Octeon mpix.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 */
#ifndef __CVMX_MPIX_DEFS_H__
#define __CVMX_MPIX_DEFS_H__

#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_CFG(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_MPIX_CFG(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001070000001000ull) + ((offset) & 3) * 512;
}
#else
#define CVMX_MPIX_CFG(offset) (CVMX_ADD_IO_SEG(0x0001070000001000ull) + ((offset) & 3) * 512)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_DATX(unsigned long offset, unsigned long block_id)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && (((offset <= 8)) && ((block_id <= 2))))))
		cvmx_warn("CVMX_MPIX_DATX(%lu,%lu) is invalid on this chip\n", offset, block_id);
	return CVMX_ADD_IO_SEG(0x0001070000001080ull) + (((offset) & 15) + ((block_id) & 3) * 0x40ull) * 8;
}
#else
#define CVMX_MPIX_DATX(offset, block_id) (CVMX_ADD_IO_SEG(0x0001070000001080ull) + (((offset) & 15) + ((block_id) & 3) * 0x40ull) * 8)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_STS(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_MPIX_STS(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001070000001008ull) + ((offset) & 3) * 512;
}
#else
#define CVMX_MPIX_STS(offset) (CVMX_ADD_IO_SEG(0x0001070000001008ull) + ((offset) & 3) * 512)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_STS_W1S(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_MPIX_STS_W1S(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001070000001018ull) + ((offset) & 3) * 512;
}
#else
#define CVMX_MPIX_STS_W1S(offset) (CVMX_ADD_IO_SEG(0x0001070000001018ull) + ((offset) & 3) * 512)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_TX(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_MPIX_TX(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001070000001010ull) + ((offset) & 3) * 512;
}
#else
#define CVMX_MPIX_TX(offset) (CVMX_ADD_IO_SEG(0x0001070000001010ull) + ((offset) & 3) * 512)
#endif
#if CVMX_ENABLE_CSR_ADDRESS_CHECKING
static inline uint64_t CVMX_MPIX_WIDE_DAT(unsigned long offset)
{
	if (!(
	      (OCTEON_IS_MODEL(OCTEON_CNF75XX) && ((offset <= 2)))))
		cvmx_warn("CVMX_MPIX_WIDE_DAT(%lu) is invalid on this chip\n", offset);
	return CVMX_ADD_IO_SEG(0x0001070000001040ull) + ((offset) & 3) * 512;
}
#else
#define CVMX_MPIX_WIDE_DAT(offset) (CVMX_ADD_IO_SEG(0x0001070000001040ull) + ((offset) & 3) * 512)
#endif

/**
 * cvmx_mpi#_cfg
 *
 * This register provides configuration for the MPI/SPI interface.
 * Note that some of the MPI/SPI0 pins are multiplexed with UART and BOOT pins.
 *
 * MPI/SPI0 Pins:
 *   * SPI0_CK MPI/SPI0 clock; dedicated pin SPI_CK.
 *   * SPI0_DI MPI/SPI0 input; shared with UART0_DCD_L/SPI0_DI; enabled when MPI_CFG[ENABLE]=1.
 *   * SPI0_DO MPI/SPI0 output; shared with UART0_DTR_L/SPI0_DO; enabled when MPI_CFG[ENABLE]=1.
 *   * SPI0_CS0_L MPI/SPI0 chip0 select;
 *     shared with BOOT_CE_N[6], enabled when MPI_CFG[ENABLE]=1 and MPI_CFG[CSENA0]=1.
 *   * SPI0_CS1_L MPI/SPI0 chip1 select;
 *     shared with BOOT_CE_N[7], enabled when MPI_CFG[ENABLE]=1 and MPI_CFG[CSENA1]=1.
 *   * SPI0_CS2_L MPI/SPI0 chip2 select;
 *     shared with UART1_RTS_N, enabled when MPI_CFG[ENABLE]=1 and MPI_CFG[CSENA2]=1.
 *   * SPI0_CS3_L MPI/SPI0 chip3 select;
 *     shared with UART1_CTS_N, enabled when MPI_CFG[ENABLE]=1 and MPI_CFG[CSENA3]=1.
 *
 * MPI/SPI1 Pins:
 *   * SPI1_CK MPI/SPI1 clock; dedicated pin SPI1_CLK.
 *   * SPI1_DI MPI/SPI1 input; dedicated pin SPI1_DI.
 *   * SPI1_DO MPI/SPI1 output; dedicated pin SPI1_DO.
 *   * SPI1_CS_L MPI/SPI1 chip select; dedicated pin SPI1_CS_L (one chip select).
 *
 * MPI/SPI2 Pins:
 *   * SPI2_CK MPI/SPI1 clock; dedicated pin SPI2_CLK.
 *   * SPI2_DI MPI/SPI1 input; dedicated pin SPI2_DI.
 *   * SPI2_DO MPI/SPI1 output; dedicated pin SPI2_DO.
 *   * SPI2_CS_L MPI/SPI1 chip select; dedicated pin SPI2_CS_L (one chip select).
 */
union cvmx_mpix_cfg {
	uint64_t u64;
	struct cvmx_mpix_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_29_63               : 35;
	uint64_t clkdiv                       : 13; /**< Clock divisor.
                                                         _ SPI_CK = coprocessor clock / (2 * CLKDIV)
                                                         _ CLKDIV = coprocessor clock / (2 * SPI_CK) */
	uint64_t csena3                       : 1;  /**< SPI0_CS3_L pin enable, valid for SPI/MPI0 only:
                                                         0 = UART1_RTS_L/SPI_CS3_L pin is UART pin.
                                                         1 = UART1_RTS_L/SPI_CS3_L pin is MPI/SPI pin.
                                                             SPI_CS3_L drives UART1_RTS_L/SPI_CS3_L. */
	uint64_t csena2                       : 1;  /**< SPI0_CS2_L pin enable, valid for SPI/MPI0 only:
                                                         0 = UART1_CTS_L/SPI_CS2_L pin is UART pin.
                                                         1 = UART1_CTS_L/SPI_CS2_L pin is MPI/SPI pin.
                                                             SPI_CS2_L drives UART1_CTS_L/SPI_CS2_L. */
	uint64_t csena1                       : 1;  /**< SPI0_CS1_L pin enable, valid for SPI/MPI0 only:
                                                         0 = BOOT_CE_N<7>/SPI_CS1_L pin is boot-bus pin.
                                                         1 = BOOT_CE_N<7>/SPI_CS1_L pin is MPI/SPI pin.
                                                             SPI_CS1_L drives BOOT_CE_N<7>/SPI_CS1_L. */
	uint64_t csena0                       : 1;  /**< SPI0_CS0_L pin enable, valid for SPI/MPI0 only:
                                                         0 = BOOT_CE_N<6>/SPI_CS0_L pin is boot-bus pin.
                                                         1 = BOOT_CE_N<6>/SPI_CS0_L pin is MPI/SPI pin.
                                                             SPI_CS0_L drives BOOT_CE_N<6>/SPI_CS0_L. */
	uint64_t cslate                       : 1;  /**< SPIx_CSn_L late.
                                                         0 = SPIx_CSn_L asserts 1/2 SPI_CK cycle before the transaction.
                                                         1 = SPIx_CSn_L asserts coincident with the transaction. */
	uint64_t tritx                        : 1;  /**< Tristate TX. Used only when WIREOR = 1
                                                         0 = SPIx_DO port is driven when slave is not expected to be driving.
                                                         1 = SPIx_DO port is tristated when not transmitting. */
	uint64_t idleclks                     : 2;  /**< Idle clocks. When set, guarantees idle SPI_CK cycles between commands. */
	uint64_t cshi                         : 1;  /**< SPI_CSn_L high: 1 = SPI_CSn_L is asserted high, 0 = SPI_CSn_L is asserted low. */
	uint64_t reserved_5_6                 : 2;
	uint64_t lsbfirst                     : 1;  /**< Shift LSB first: 0 = shift MSB first, 1 = shift LSB first. */
	uint64_t wireor                       : 1;  /**< Wire-OR DO and DI.
                                                         0 = SPIx_DO and SPIx_DI are separate wires (SPI). SPIx_DO port is always driven.
                                                         1 = SPIx_DO/DI is all from SPIx_DO port (MPI). SPIx_DO pin is tristated when not
                                                         transmitting.
                                                         If WIREOR = 1, SPIx_DI port is not used by the MPI/SPI engine. */
	uint64_t clk_cont                     : 1;  /**< Clock control.
                                                         0 = clock idles to value given by IDLELO after completion of MPI/SPI transaction.
                                                         1 = clock never idles, requires SPIx_CSn_L deassertion/assertion between commands. */
	uint64_t idlelo                       : 1;  /**< Clock idle low/clock invert.
                                                         0 = SPIx_CK idles high, first transition is high-to-low. This mode corresponds to SPI
                                                         Block
                                                         Guide options CPOL = 1, CPHA = 1.
                                                         1 = SPIx_CK idles low, first transition is low-to-high. This mode corresponds to SPI Block
                                                         Guide options CPOL = 0, CPHA = 0. */
	uint64_t enable                       : 1;  /**< MPI/SPI core enable, this bit needs to be set before any SPI/MPI transactions.
                                                         This bit is valid for SPI/MPI 0-2. */
#else
	uint64_t enable                       : 1;
	uint64_t idlelo                       : 1;
	uint64_t clk_cont                     : 1;
	uint64_t wireor                       : 1;
	uint64_t lsbfirst                     : 1;
	uint64_t reserved_5_6                 : 2;
	uint64_t cshi                         : 1;
	uint64_t idleclks                     : 2;
	uint64_t tritx                        : 1;
	uint64_t cslate                       : 1;
	uint64_t csena0                       : 1;
	uint64_t csena1                       : 1;
	uint64_t csena2                       : 1;
	uint64_t csena3                       : 1;
	uint64_t clkdiv                       : 13;
	uint64_t reserved_29_63               : 35;
#endif
	} s;
	struct cvmx_mpix_cfg_s                cnf75xx;
};
typedef union cvmx_mpix_cfg cvmx_mpix_cfg_t;

/**
 * cvmx_mpi#_dat#
 */
union cvmx_mpix_datx {
	uint64_t u64;
	struct cvmx_mpix_datx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_8_63                : 56;
	uint64_t data                         : 8;  /**< Data to transmit/receive. */
#else
	uint64_t data                         : 8;
	uint64_t reserved_8_63                : 56;
#endif
	} s;
	struct cvmx_mpix_datx_s               cnf75xx;
};
typedef union cvmx_mpix_datx cvmx_mpix_datx_t;

/**
 * cvmx_mpi#_sts
 */
union cvmx_mpix_sts {
	uint64_t u64;
	struct cvmx_mpix_sts_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_13_63               : 51;
	uint64_t rxnum                        : 5;  /**< Number of bytes written for the transaction. */
	uint64_t reserved_2_7                 : 6;
	uint64_t mpi_intr                     : 1;  /**< MPI interrupt on transaction done. Throws MPI_INTSN_E::MPI_STS_INTR. */
	uint64_t busy                         : 1;  /**< Busy.
                                                         0 = No MPI/SPI transaction in progress.
                                                         1 = MPI/SPI engine is processing a transaction. */
#else
	uint64_t busy                         : 1;
	uint64_t mpi_intr                     : 1;
	uint64_t reserved_2_7                 : 6;
	uint64_t rxnum                        : 5;
	uint64_t reserved_13_63               : 51;
#endif
	} s;
	struct cvmx_mpix_sts_s                cnf75xx;
};
typedef union cvmx_mpix_sts cvmx_mpix_sts_t;

/**
 * cvmx_mpi#_sts_w1s
 *
 * This register sets MPI_STS() interrupts.
 *
 */
union cvmx_mpix_sts_w1s {
	uint64_t u64;
	struct cvmx_mpix_sts_w1s_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_2_63                : 62;
	uint64_t mpi_intr_w1s                 : 1;  /**< Reads or sets MPI_STS[MPI_INTR]. */
	uint64_t reserved_0_0                 : 1;
#else
	uint64_t reserved_0_0                 : 1;
	uint64_t mpi_intr_w1s                 : 1;
	uint64_t reserved_2_63                : 62;
#endif
	} s;
	struct cvmx_mpix_sts_w1s_s            cnf75xx;
};
typedef union cvmx_mpix_sts_w1s cvmx_mpix_sts_w1s_t;

/**
 * cvmx_mpi#_tx
 */
union cvmx_mpix_tx {
	uint64_t u64;
	struct cvmx_mpix_tx_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_22_63               : 42;
	uint64_t csid                         : 2;  /**< Which CS to assert for this transaction */
	uint64_t reserved_17_19               : 3;
	uint64_t leavecs                      : 1;  /**< Leave SPIx_CSn_L asserted.
                                                         0 = deassert SPIx_CSn_L after the transaction is done.
                                                         1 = leave SPI_CSn_L asserted after the transaction is done. */
	uint64_t reserved_13_15               : 3;
	uint64_t txnum                        : 5;  /**< Number of bytes to transmit. */
	uint64_t reserved_5_7                 : 3;
	uint64_t totnum                       : 5;  /**< Total number of bytes to shift (transmit and receive). */
#else
	uint64_t totnum                       : 5;
	uint64_t reserved_5_7                 : 3;
	uint64_t txnum                        : 5;
	uint64_t reserved_13_15               : 3;
	uint64_t leavecs                      : 1;
	uint64_t reserved_17_19               : 3;
	uint64_t csid                         : 2;
	uint64_t reserved_22_63               : 42;
#endif
	} s;
	struct cvmx_mpix_tx_s                 cnf75xx;
};
typedef union cvmx_mpix_tx cvmx_mpix_tx_t;

/**
 * cvmx_mpi#_wide_dat
 */
union cvmx_mpix_wide_dat {
	uint64_t u64;
	struct cvmx_mpix_wide_dat_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t data                         : 64; /**< Data to transmit/receive. */
#else
	uint64_t data                         : 64;
#endif
	} s;
	struct cvmx_mpix_wide_dat_s           cnf75xx;
};
typedef union cvmx_mpix_wide_dat cvmx_mpix_wide_dat_t;

#endif
