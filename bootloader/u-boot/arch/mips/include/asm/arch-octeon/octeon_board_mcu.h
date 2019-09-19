/***********************license start************************************
 * Copyright (c) 2012 Cavium Inc. (support@cavium.com). All rights
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
#ifndef __OCTEON_BOARD_MCU_H__
#define __OCTEON_BOARD_MCU_H__

//#include <asm/arch/cvmx-app-init.h>

/* NOTE: All of these functions are weak and may be overwritten */

#ifdef CONFIG_OCTEON_MCU

/**
 * Query all nodes MCUs for QLM params (number_lanes, mode, speed, clk_ref)
 * Return 0 on success, -1 on fail
 */
int get_mcu_qlm_params(void);

/**
 * Return char * mode_str to mode as retrieved from MCU.
 * @param node - octeon node number starting from 0
 * @param qlm  - interface number staring from 0
 */
const char * octeon_board_mcu_get_qlm_mode(uint node, uint qlm);

/**
 * Return PCIe mode  0=>EP, 1=>RC as retrieved from MCU.
 * @param node - octeon node number starting from 0
 * @param qlm  - interface number staring from 0
 */
int octeon_board_mcu_get_pcie_mode(uint node, uint qlm);

/**
 * Return PCIe gen  0=>gen1, 1=>gen2, 2=>gen3 as retrieved from MCU.
 * @param node - octeon node number starting from 0
 * @param qlm  - interface number staring from 0
 */
int octeon_board_mcu_get_pcie_gen(uint node, uint qlm);

/**
 * Return PCIe number of lanes as retrieved from MCU.
 * @param node - octeon node number starting from 0
 * @param qlm  - interface number staring from 0
 */
uint octeon_board_mcu_get_pcie_lanes(uint node, uint qlm);


#endif

#endif /* __OCTEON_BOARD_MCU_H__ */
