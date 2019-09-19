/***********************license start************************************
 * Copyright (c) 2013 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/
#ifndef __OCTEON_DRAM_H__
#define __OCTEON_DRAM_H__

#define OCTEON_DRAM_LATE_ZERO_OFFSET	0x100000ull

/**
 * Initialize system DRAM
 */
int octeon_init_dram(void);

/**
 * Very simple DRAM test to run from flash for cases when bootloader boot
 * can't complete.
 */
int octeon_dram_test(void);

#if OCTEON_CONFIG_DFM && !CONFIG_OCTEON_SIM_NO_DDR
/**
 * Initialize the DFM memory
 */
int octeon_init_dfm(void);
#endif

/**
 * Import any environment variables passed via a remote bootloader.  It is
 * only active if U-Boot was already loaded into RAM.
 */
int octeon_import_ram_env(void);

/**
 * Clears DRAM, while taking care to not overwrite DRAM already in use
 * by u-boot
 */
void octeon_clear_mem_pfs(uint64_t base_addr, uint64_t size);

/**
 * Clear all DRAM ECC errors
 *
 * @return 0 for success
 */
int octeon_dram_clear_ecc_errors(void);

/**
 * This clears any bits set in the l2c_int_reg before exiting U-Boot.
 *
 * @param verbose	Prints out the status information according to which
 *			bits are set in the CVMX_L2C_INT_REG CSR.
 */
void octeon_dram_clear_access_errors(bool verbose);

#endif /* __OCTEON_DRAM_H__ */
