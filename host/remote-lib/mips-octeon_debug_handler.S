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

    .globl octeon_debug_handler
    .text
    .ent octeon_debug_handler
octeon_debug_handler:
        SETUP_K0_DEBUG_DATA_PER_CORE

        /* Save all the CPU registers */
        /* SAVE_REGISTER(0) */
        SAVE_REGISTER(1)
        SAVE_REGISTER(2)
        SAVE_REGISTER(3)
        SAVE_REGISTER(4)
        SAVE_REGISTER(5)
        SAVE_REGISTER(6)
        SAVE_REGISTER(7)
        SAVE_REGISTER(8)
        SAVE_REGISTER(9)
        SAVE_REGISTER(10)
        SAVE_REGISTER(11)
        SAVE_REGISTER(12)
        SAVE_REGISTER(13)
        SAVE_REGISTER(14)
        SAVE_REGISTER(15)
        SAVE_REGISTER(16)
        SAVE_REGISTER(17)
        SAVE_REGISTER(18)
        SAVE_REGISTER(19)
        SAVE_REGISTER(20)
        SAVE_REGISTER(21)
        SAVE_REGISTER(22)
        SAVE_REGISTER(23)
        SAVE_REGISTER(24)
        SAVE_REGISTER(25)
        dmfc0   $25, $COP0_DESAVE    /* This is really the k0 register */
        sd      $25, 8*26($k0)
        SAVE_REGISTER(27)
        SAVE_REGISTER(28)
        SAVE_REGISTER(29)
        SAVE_REGISTER(30)
        SAVE_REGISTER(31)
        mflo    $k1
        sd      $k1, 8*32($k0)
        mfhi    $k1
        sd      $k1, 8*33($k0)

        /* Save the breakpoint information */
        SAVE_ADDRESS(128 + 0, HW_INSTRUCTION_BREAKPOINT_STATUS)
        SAVE_ADDRESS(128 + 1, HW_DATA_BREAKPOINT_STATUS)
        SAVE_ADDRESS(128 + 8, HW_INSTRUCTION_BREAKPOINT_ADDRESS(0))
        SAVE_ADDRESS(128 + 9, HW_INSTRUCTION_BREAKPOINT_ADDRESS(1))
        SAVE_ADDRESS(128 + 10, HW_INSTRUCTION_BREAKPOINT_ADDRESS(2))
        SAVE_ADDRESS(128 + 11, HW_INSTRUCTION_BREAKPOINT_ADDRESS(3))
        SAVE_ADDRESS(128 + 16, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(0))
        SAVE_ADDRESS(128 + 17, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(1))
        SAVE_ADDRESS(128 + 18, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(2))
        SAVE_ADDRESS(128 + 19, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(3))
        SAVE_ADDRESS(128 + 24, HW_INSTRUCTION_BREAKPOINT_ASID(0))
        SAVE_ADDRESS(128 + 25, HW_INSTRUCTION_BREAKPOINT_ASID(1))
        SAVE_ADDRESS(128 + 26, HW_INSTRUCTION_BREAKPOINT_ASID(2))
        SAVE_ADDRESS(128 + 27, HW_INSTRUCTION_BREAKPOINT_ASID(3))
        SAVE_ADDRESS(128 + 32, HW_INSTRUCTION_BREAKPOINT_CONTROL(0))
        SAVE_ADDRESS(128 + 33, HW_INSTRUCTION_BREAKPOINT_CONTROL(1))
        SAVE_ADDRESS(128 + 34, HW_INSTRUCTION_BREAKPOINT_CONTROL(2))
        SAVE_ADDRESS(128 + 35, HW_INSTRUCTION_BREAKPOINT_CONTROL(3))
        SAVE_ADDRESS(128 + 40, HW_DATA_BREAKPOINT_ADDRESS(0))
        SAVE_ADDRESS(128 + 41, HW_DATA_BREAKPOINT_ADDRESS(1))
        SAVE_ADDRESS(128 + 42, HW_DATA_BREAKPOINT_ADDRESS(2))
        SAVE_ADDRESS(128 + 43, HW_DATA_BREAKPOINT_ADDRESS(3))
        SAVE_ADDRESS(128 + 48, HW_DATA_BREAKPOINT_ADDRESS_MASK(0))
        SAVE_ADDRESS(128 + 49, HW_DATA_BREAKPOINT_ADDRESS_MASK(1))
        SAVE_ADDRESS(128 + 50, HW_DATA_BREAKPOINT_ADDRESS_MASK(2))
        SAVE_ADDRESS(128 + 51, HW_DATA_BREAKPOINT_ADDRESS_MASK(3))
        SAVE_ADDRESS(128 + 56, HW_DATA_BREAKPOINT_ASID(0))
        SAVE_ADDRESS(128 + 57, HW_DATA_BREAKPOINT_ASID(1))
        SAVE_ADDRESS(128 + 58, HW_DATA_BREAKPOINT_ASID(2))
        SAVE_ADDRESS(128 + 59, HW_DATA_BREAKPOINT_ASID(3))
        SAVE_ADDRESS(128 + 64, HW_DATA_BREAKPOINT_CONTROL(0))
        SAVE_ADDRESS(128 + 65, HW_DATA_BREAKPOINT_CONTROL(1))
        SAVE_ADDRESS(128 + 66, HW_DATA_BREAKPOINT_CONTROL(2))
        SAVE_ADDRESS(128 + 67, HW_DATA_BREAKPOINT_CONTROL(3))

        /* Save all of the COP0 registers */
        SAVE_COP0(COP0_INDEX)
        SAVE_COP0(COP0_RANDOM)
        SAVE_COP0(COP0_ENTRYLO0)
        SAVE_COP0(COP0_ENTRYLO1)
        SAVE_COP0(COP0_CONTEXT)
        SAVE_COP0(COP0_PAGEMASK)
        SAVE_COP0(COP0_PAGEGRAIN)
        SAVE_COP0(COP0_WIRED)
        SAVE_COP0(COP0_HWRENA)
        SAVE_COP0(COP0_BADVADDR)
        SAVE_COP0(COP0_COUNT)
        SAVE_COP0(COP0_CVMCOUNT)
        SAVE_COP0(COP0_CVMCTL)
        SAVE_COP0(COP0_ENTRYHI)
        SAVE_COP0(COP0_COMPARE)
        SAVE_COP0(COP0_CVMMEMCTL)
        SAVE_COP0(COP0_STATUS)
        SAVE_COP0(COP0_INTCTL)
        SAVE_COP0(COP0_SRSCTL)
        SAVE_COP0(COP0_CAUSE)
        SAVE_COP0(COP0_EPC)
        SAVE_COP0(COP0_PRID)
        SAVE_COP0(COP0_EBASE)
        SAVE_COP0(COP0_CONFIG)
        SAVE_COP0(COP0_CONFIG1)
        SAVE_COP0(COP0_CONFIG2)
        SAVE_COP0(COP0_CONFIG3)
        SAVE_COP0(COP0_WATCHLO0)
        SAVE_COP0(COP0_WATCHLO1)
        SAVE_COP0(COP0_WATCHHI0)
        SAVE_COP0(COP0_WATCHHI1)
        SAVE_COP0(COP0_XCONTEXT)
        SAVE_COP0(COP0_MULTICOREDEBUG)
        SAVE_COP0(COP0_DEBUG)
        SAVE_COP0(COP0_DEPC)
        SAVE_COP0(COP0_PERFCONTROL0)
        SAVE_COP0(COP0_PERFCONTROL1)
        SAVE_COP0(COP0_PERFVALUE0)
        SAVE_COP0(COP0_PERFVALUE1)
        SAVE_COP0(COP0_CACHEERRI)
        SAVE_COP0(COP0_CACHEERRD)
        SAVE_COP0(COP0_TAGLOI)
        SAVE_COP0(COP0_TAGLOD)
        SAVE_COP0(COP0_DATALOI)
        SAVE_COP0(COP0_DATALOD)
        SAVE_COP0(COP0_TAGHI)
        SAVE_COP0(COP0_DATAHII)
        SAVE_COP0(COP0_DATAHID)
        SAVE_COP0(COP0_ERROREPC)
        SAVE_COP0(COP0_DESAVE)

        /* Read the COP0_CONFIG1 register to find the number of TLB entries.
            Save all the TLB entries (32 for CN3XXX, 64 for CN5XXX). */
        mfc0    $t1, $COP0_CONFIG1
        srl     $t1, $t1, 25
        andi    $t1, $t1, 0x3f
        li      $t0, -1
        daddiu  $a0, $k0, 8*2*256
LTLBSave:
        addu    $t0, 1
        mtc0    $t0, $COP0_INDEX
        tlbr
        dmfc0   $k1, $COP0_ENTRYHI
        sd      $k1, 0($a0)
        dmfc0   $k1, $COP0_PAGEMASK
        sd      $k1, 8($a0)
        dmfc0   $k1, $COP0_ENTRYLO0
        sd      $k1, 16($a0)
        dmfc0   $k1, $COP0_ENTRYLO1
        sd      $k1, 24($a0)
        bne     $t0, $t1, LTLBSave
         daddiu $a0, 4*8

LBeginWaitForDebugger:
        ld      $k1, 0($k0)
        bbit1   $k1, 1, LSkipWait
         syncw  /* Syncw is needed to make sure all of the processor state has made it to memory. */

        /* Signal that we've stopped. The syncw gets the waiting flag out. */
        li      $k1, 1
        sd      $k1, 0($k0)
        syncw

        /* Spin here until the Debugger tells us to start running again */
LWaitForDebugger:
        ld      $k1, 0($k0)
        bne     $k1, $0, LWaitForDebugger
         nop

LSkipWait:
        /* Restore the TLB entries */
        li      $t0, -1
        daddiu  $a0, $k0, 8*2*256
LTLBRestore:
        addu    $t0, 1
        mtc0    $t0, $COP0_INDEX
        ld      $k1, 0($a0)
        dmtc0   $k1, $COP0_ENTRYHI
        ld      $k1, 8($a0)
        dmtc0   $k1, $COP0_PAGEMASK
        ld      $k1, 16($a0)
        dmtc0   $k1, $COP0_ENTRYLO0
        ld      $k1, 24($a0)
        dmtc0   $k1, $COP0_ENTRYLO1
        tlbwi
        bne     $t0, $t1, LTLBRestore
         daddiu $a0, 4*8

        /* Restore the breakpoint information */
        RESTORE_ADDRESS(128 + 0, HW_INSTRUCTION_BREAKPOINT_STATUS)
        RESTORE_ADDRESS(128 + 1, HW_DATA_BREAKPOINT_STATUS)
        RESTORE_ADDRESS(128 + 8, HW_INSTRUCTION_BREAKPOINT_ADDRESS(0))
        RESTORE_ADDRESS(128 + 9, HW_INSTRUCTION_BREAKPOINT_ADDRESS(1))
        RESTORE_ADDRESS(128 + 10, HW_INSTRUCTION_BREAKPOINT_ADDRESS(2))
        RESTORE_ADDRESS(128 + 11, HW_INSTRUCTION_BREAKPOINT_ADDRESS(3))
        RESTORE_ADDRESS(128 + 16, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(0))
        RESTORE_ADDRESS(128 + 17, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(1))
        RESTORE_ADDRESS(128 + 18, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(2))
        RESTORE_ADDRESS(128 + 19, HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(3))
        RESTORE_ADDRESS(128 + 24, HW_INSTRUCTION_BREAKPOINT_ASID(0))
        RESTORE_ADDRESS(128 + 25, HW_INSTRUCTION_BREAKPOINT_ASID(1))
        RESTORE_ADDRESS(128 + 26, HW_INSTRUCTION_BREAKPOINT_ASID(2))
        RESTORE_ADDRESS(128 + 27, HW_INSTRUCTION_BREAKPOINT_ASID(3))
        RESTORE_ADDRESS(128 + 32, HW_INSTRUCTION_BREAKPOINT_CONTROL(0))
        RESTORE_ADDRESS(128 + 33, HW_INSTRUCTION_BREAKPOINT_CONTROL(1))
        RESTORE_ADDRESS(128 + 34, HW_INSTRUCTION_BREAKPOINT_CONTROL(2))
        RESTORE_ADDRESS(128 + 35, HW_INSTRUCTION_BREAKPOINT_CONTROL(3))
        RESTORE_ADDRESS(128 + 40, HW_DATA_BREAKPOINT_ADDRESS(0))
        RESTORE_ADDRESS(128 + 41, HW_DATA_BREAKPOINT_ADDRESS(1))
        RESTORE_ADDRESS(128 + 42, HW_DATA_BREAKPOINT_ADDRESS(2))
        RESTORE_ADDRESS(128 + 43, HW_DATA_BREAKPOINT_ADDRESS(3))
        RESTORE_ADDRESS(128 + 48, HW_DATA_BREAKPOINT_ADDRESS_MASK(0))
        RESTORE_ADDRESS(128 + 49, HW_DATA_BREAKPOINT_ADDRESS_MASK(1))
        RESTORE_ADDRESS(128 + 50, HW_DATA_BREAKPOINT_ADDRESS_MASK(2))
        RESTORE_ADDRESS(128 + 51, HW_DATA_BREAKPOINT_ADDRESS_MASK(3))
        RESTORE_ADDRESS(128 + 56, HW_DATA_BREAKPOINT_ASID(0))
        RESTORE_ADDRESS(128 + 57, HW_DATA_BREAKPOINT_ASID(1))
        RESTORE_ADDRESS(128 + 58, HW_DATA_BREAKPOINT_ASID(2))
        RESTORE_ADDRESS(128 + 59, HW_DATA_BREAKPOINT_ASID(3))
        RESTORE_ADDRESS(128 + 64, HW_DATA_BREAKPOINT_CONTROL(0))
        RESTORE_ADDRESS(128 + 65, HW_DATA_BREAKPOINT_CONTROL(1))
        RESTORE_ADDRESS(128 + 66, HW_DATA_BREAKPOINT_CONTROL(2))
        RESTORE_ADDRESS(128 + 67, HW_DATA_BREAKPOINT_CONTROL(3))

        /* Restore all the COP0 registers */
        RESTORE_COP0(COP0_INDEX)
        RESTORE_COP0(COP0_RANDOM)
        RESTORE_COP0(COP0_ENTRYLO0)
        RESTORE_COP0(COP0_ENTRYLO1)
        RESTORE_COP0(COP0_CONTEXT)
        RESTORE_COP0(COP0_PAGEMASK)
        RESTORE_COP0(COP0_PAGEGRAIN)
        RESTORE_COP0(COP0_WIRED)
        RESTORE_COP0(COP0_HWRENA)
        RESTORE_COP0(COP0_BADVADDR)
        RESTORE_COP0(COP0_COUNT)
        RESTORE_COP0(COP0_CVMCOUNT)
        RESTORE_COP0(COP0_CVMCTL)
        RESTORE_COP0(COP0_ENTRYHI)
        RESTORE_COP0(COP0_COMPARE)
        RESTORE_COP0(COP0_CVMMEMCTL)
        RESTORE_COP0(COP0_STATUS)
        RESTORE_COP0(COP0_INTCTL)
        RESTORE_COP0(COP0_SRSCTL)
        RESTORE_COP0(COP0_CAUSE)
        RESTORE_COP0(COP0_EPC)
        RESTORE_COP0(COP0_PRID)
        RESTORE_COP0(COP0_EBASE)
        RESTORE_COP0(COP0_CONFIG)
        RESTORE_COP0(COP0_CONFIG1)
        RESTORE_COP0(COP0_CONFIG2)
        RESTORE_COP0(COP0_CONFIG3)
        RESTORE_COP0(COP0_WATCHLO0)
        RESTORE_COP0(COP0_WATCHLO1)
        RESTORE_COP0(COP0_WATCHHI0)
        RESTORE_COP0(COP0_WATCHHI1)
        RESTORE_COP0(COP0_XCONTEXT)
        RESTORE_COP0(COP0_MULTICOREDEBUG)
        RESTORE_COP0(COP0_DEBUG)
        RESTORE_COP0(COP0_DEPC)
        RESTORE_COP0(COP0_PERFCONTROL0)
        RESTORE_COP0(COP0_PERFCONTROL1)
        RESTORE_COP0(COP0_PERFVALUE0)
        RESTORE_COP0(COP0_PERFVALUE1)
        RESTORE_COP0(COP0_CACHEERRI)
        RESTORE_COP0(COP0_CACHEERRD)
        RESTORE_COP0(COP0_TAGLOI)
        RESTORE_COP0(COP0_TAGLOD)
        RESTORE_COP0(COP0_DATALOI)
        RESTORE_COP0(COP0_DATALOD)
        RESTORE_COP0(COP0_TAGHI)
        RESTORE_COP0(COP0_DATAHII)
        RESTORE_COP0(COP0_DATAHID)
        RESTORE_COP0(COP0_ERROREPC)
        RESTORE_COP0(COP0_DESAVE)


        ld      $k1, 8*32($k0)
        mtlo    $k1
        ld      $k1, 8*33($k0)
        mthi    $k1
        /* RESTORE_REGISTER(0) */
        RESTORE_REGISTER(1)
        RESTORE_REGISTER(2)
        RESTORE_REGISTER(3)
        RESTORE_REGISTER(4)
        RESTORE_REGISTER(5)
        RESTORE_REGISTER(6)
        RESTORE_REGISTER(7)
        RESTORE_REGISTER(8)
        RESTORE_REGISTER(9)
        RESTORE_REGISTER(10)
        RESTORE_REGISTER(11)
        RESTORE_REGISTER(12)
        RESTORE_REGISTER(13)
        RESTORE_REGISTER(14)
        RESTORE_REGISTER(15)
        RESTORE_REGISTER(16)
        RESTORE_REGISTER(17)
        RESTORE_REGISTER(18)
        RESTORE_REGISTER(19)
        RESTORE_REGISTER(20)
        RESTORE_REGISTER(21)
        RESTORE_REGISTER(22)
        RESTORE_REGISTER(23)
        RESTORE_REGISTER(24)
        RESTORE_REGISTER(25)
        /* Skip k0, we'll get it last */
        RESTORE_REGISTER(27)
        RESTORE_REGISTER(28)
        RESTORE_REGISTER(29)
        RESTORE_REGISTER(30)
        RESTORE_REGISTER(31)
        RESTORE_REGISTER(26)    /* Restore k0 */
        synci   0($0)           /* Flush the icache and return */
        deret
        nop

    .end octeon_debug_handler
