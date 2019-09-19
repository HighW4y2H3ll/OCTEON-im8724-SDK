/***********************license start************************************
 * Copyright (c) 2009 Cavium Inc. (support@cavium.com). All rights
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
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **********************license end**************************************/

/**
 * @file octeon_debugger_handler.S
 *
 * Debug exception handler used with the Octeon EJTAG debug exception
 * to provide access to core registers through memory.
 *
 * $Id: octeon_debugger_handler.S 24122 2007-04-27 16:27:15Z cchavva $
 *
 */
#include "mips-octeon-common.S"

.set noreorder
.set noat

    .globl octeon_profiler_handler
    .text
    .ent octeon_profiler_handler
octeon_profiler_handler:
        SETUP_K0_DEBUG_DATA_PER_CORE

        ld      $0, 0($k0)               /* Perform a load to make sure we don't get a RAW stall */
        sd      $t0, 0($k0)               /* Temporarily store t0 for a working spot */
        dmfc0   $t0, $COP0_DEPC          /* Load the PC */
        sd      $t1, 8($k0)               /* Temporarily store t1 for a working spot */
        dmfc0   $t1, $COP0_PERFVALUE0    /* Load performance counter 0 */
        sd      $t0, 8*(256+24*8+0)($k0)  /* Store the PC into COP0_DEPC spot */
        sd      $t1, 8*(256+25*8+1)($k0)  /* Store the PC into COP0_PERFVALUE0 spot */
        dmfc0   $t0, $COP0_PERFVALUE1    /* Load performance counter 1 */
        sd      $t0, 8*(256+25*8+3)($k0)  /* Store the PC into COP0_PERFVALUE1 spot */
        dmfc0   $t0, $COP0_PRID		  /* OIII has 2 additional performance counters */
        andi	$t0, $t0, 0xff00
        dli	$t1, 0x9500
        slt	$t0, $t0, $t1
        bne	$t0, $zero, 1f
        nop
        dmfc0   $t0, $COP0_PERFVALUE2    /* Load performance counter 2 */
        dmfc0   $t1, $COP0_PERFVALUE3    /* Load performance counter 3 */
        sd      $t0, 8*(256+25*8+5)($k0)  /* Store the PC into COP0_PERFVALUE2 spot */
        sd      $t1, 8*(256+25*8+7)($k0)  /* Store the PC into COP0_PERFVALUE3 spot */
1:
        ld      $t0, 0($k0)               /* Restore t0 */
        ld      $t1, 8($k0)               /* Restore t1 */
        syncw                           /* Make sure all writes go out */
        dmfc0   $k0, $COP0_DESAVE        /* Restore k0 */
        deret                           /* Return to normal execution */
        nop
	.space 1024 - 256

    .end octeon_profiler_handler

