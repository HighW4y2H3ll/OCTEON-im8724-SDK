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
 * @file
 *
 * Common defines for writing Octeon assembly
 *
 * $Id: octeon_debugger_handler.S 24122 2007-04-27 16:27:15Z cchavva $
 *
 */
#define OCTEON_DEBUG_HANDLER_BASE 0xdeadbeefdeadbeef
#define OFFSET_14_0  ((OCTEON_DEBUG_HANDLER_BASE>>0) & 0x7fff)
#define OFFSET_29_15 ((OCTEON_DEBUG_HANDLER_BASE>>15) & 0x7fff)
#define OFFSET_44_30 ((OCTEON_DEBUG_HANDLER_BASE>>30) & 0x7fff)
#define OFFSET_59_45 ((OCTEON_DEBUG_HANDLER_BASE>>45) & 0x7fff)
#define OFFSET_63_60 ((OCTEON_DEBUG_HANDLER_BASE>>60) & 0xf)

#define COP0_INDEX      0,0     /* TLB read/write index */
#define COP0_RANDOM     1,0     /* TLB random index */
#define COP0_ENTRYLO0   2,0     /* TLB entryLo0 */
#define COP0_ENTRYLO1   3,0     /* TLB entryLo1 */
#define COP0_CONTEXT    4,0     /* Context */
#define COP0_USERLOCAL  4,2     /* Octeon2 - User Local */
#define COP0_PAGEMASK   5,0     /* TLB pagemask */
#define COP0_PAGEGRAIN  5,1     /* TLB config for max page sizes */
#define COP0_WIRED      6,0     /* TLB number of wired entries */
#define COP0_HWRENA     7,0     /* rdhw instruction enable per register */
#define COP0_BADVADDR   8,0     /* Bad virtual address */
#define COP0_BADINSTR   8,1     /* Bad instruction */
#define COP0_BADINSTRP  8,2     /* Bad prior instruction */
#define COP0_COUNT      9,0     /* Mips count register */
#define COP0_CVMCOUNT   9,6     /* Cavium count register */
#define COP0_CVMCTL     9,7     /* Cavium control */
#define COP0_ENTRYHI    10,0    /* TLB entryHi */
#define COP0_GUESTCTL2  10,5    /* Octeon3 - Guest control register */
#define COP0_COMPARE    11,0    /* Mips compare register */
#define COP0_POWTHR     11,6    /* Octeon2 - Power Throttle Register */
#define COP0_CVMMEMCTL  11,7    /* Cavium memory control */
#define COP0_STATUS     12,0    /* Mips status register */
#define COP0_INTCTL     12,1    /* Useless (Vectored interrupts) */
#define COP0_SRSCTL     12,2    /* Useless (Shadow registers) */
#define COP0_GUESTCTL0  12,6    /* Octeon3 - Guest control register */
#define COP0_GTOFFSET   12,7    /* Octeon3 - 2's compli offset from Count */
#define COP0_CAUSE      13,0    /* Mips cause register */
#define COP0_EPC        14,0    /* Exception program counter */
#define COP0_PRID       15,0    /* Processor ID */
#define COP0_EBASE      15,1    /* Exception base */
#define COP0_CONFIG     16,0    /* Misc config options */
#define COP0_CONFIG1    16,1    /* Misc config options */
#define COP0_CONFIG2    16,2    /* Misc config options */
#define COP0_CONFIG3    16,3    /* Misc config options */
#define COP0_CONFIG4    16,4    /* Octeon2 - Misc config options */
#define COP0_CONFIG5    16,5    /* Octeon3 - Misc config options */
#define COP0_VMMEMCTL2  16,6    /* Octeon3 - VM memory control register */
#define COP0_CVMVMCONFIG  16,7  /* Octeon3 - VM config regoster */
#define COP0_WATCHLO0   18,0    /* Address watch registers */
#define COP0_WATCHLO1   18,1    /* Address watch registers */
#define COP0_WATCHHI0   19,0    /* Address watch registers */
#define COP0_WATCHHI1   19,1    /* Address watch registers */
#define COP0_XCONTEXT   20,0    /* OS context */
#define COP0_MULTICOREDEBUG 22,0 /* Cavium debug */
#define COP0_CVMCOUNTOFFSET 22,2 /* Offset added to cvmcount to create guest */
#define COP0_DEBUG      23,0    /* Debug status */
#define COP0_DEBUG2     23,6    /* Octeon2 - Debug2 Complex breakpoints */
#define COP0_DEPC       24,0    /* Debug PC */
#define COP0_PERFCONTROL0 25,0  /* Performance counter control */
#define COP0_PERFCONTROL1 25,2  /* Performance counter control */
#define COP0_PERFCONTROL2 25,4  /* Octeon3 - Performance counter control */
#define COP0_PERFCONTROL3 25,6  /* Octeon3 - Performance counter control */
#define COP0_PERFVALUE0 25,1    /* Performance counter */
#define COP0_PERFVALUE1 25,3    /* Performance counter */
#define COP0_PERFVALUE2 25,5    /* Octeon3 - Performance counter */
#define COP0_PERFVALUE3 25,7    /* Octeon3 - Performance counter */
#define COP0_ERRCTL     26,0    /* Octeon3 - I cache errors */ 
#define COP0_CACHEERRI  27,0    /* I cache error status */
#define COP0_CACHEERRD  27,1    /* D cache error status */
#define COP0_ICACHEDEBUG  27,2    /* cache debug status */
#define COP0_TAGLOI     28,0    /* I cache tagLo */
#define COP0_TAGLOD     28,2    /* D cache tagLo */
#define COP0_DATALOI    28,1    /* I cache dataLo */
#define COP0_DATALOD    28,3    /* D cahce dataLo */
#define COP0_TAGLO1     28,4    /* D cahce taglo */
#define COP0_TAGHI      29,2    /* ? */
#define COP0_DATAHII    29,1    /* ? */
#define COP0_DATAHID    29,3    /* ? */
#define COP0_ERROREPC   30,0    /* Error PC */
#define COP0_DESAVE     31,0    /* Debug scratch area */
#define COP0_KSCRATCH1  31,2    /* Octeon2 - Scratch area */
#define COP0_KSCRATCH2  31,3    /* Octeon2 - Scratch area */
#define COP0_KSCRATCH3  31,4    /* Octeon2 - Scratch area */
#define COP0_KSCRATCH4  31,5    /* Octeon3 - Scratch area */

#define HW_INSTRUCTION_BREAKPOINT_STATUS            (0xFFFFFFFFFF301000)
#define HW_INSTRUCTION_BREAKPOINT_ADDRESS(num)      (0xFFFFFFFFFF301100 + 0x100 * (num))
#define HW_INSTRUCTION_BREAKPOINT_ADDRESS_MASK(num) (0xFFFFFFFFFF301108 + 0x100 * (num))
#define HW_INSTRUCTION_BREAKPOINT_ASID(num)         (0xFFFFFFFFFF301110 + 0x100 * (num))
#define HW_INSTRUCTION_BREAKPOINT_CONTROL(num)      (0xFFFFFFFFFF301118 + 0x100 * (num))

#define HW_DATA_BREAKPOINT_STATUS                   (0xFFFFFFFFFF302000)
#define HW_DATA_BREAKPOINT_ADDRESS(num)             (0xFFFFFFFFFF302100 + 0x100 * (num))
#define HW_DATA_BREAKPOINT_ADDRESS_MASK(num)        (0xFFFFFFFFFF302108 + 0x100 * (num))
#define HW_DATA_BREAKPOINT_ASID(num)                (0xFFFFFFFFFF302110 + 0x100 * (num))
#define HW_DATA_BREAKPOINT_CONTROL(num)             (0xFFFFFFFFFF302118 + 0x100 * (num))

#define SAVE_REGISTER(reg)      sd $##reg, 8*reg($k0);
#define RESTORE_REGISTER(reg)   ld $##reg, 8*reg($k0);
#define SAVE_FP_REGISTER(reg, offset)    sdc1 $##reg, 8*offset($a0)
#define RESTORE_FP_REGISTER(reg, offset) ldc1 $##reg, 8*offset($a0)

#define SAVE_COP0(a)    SAVE_COP0_ARGS(a)
#define SAVE_COP0_ARGS(reg, sel)        \
        dmfc0 $k1, $##reg, sel;          \
        sd $k1, 8*(256+reg*8+sel)($k0);

#define RESTORE_COP0(a) RESTORE_COP0_ARGS(a)
#define RESTORE_COP0_ARGS(reg, sel)     \
        ld $k1, 8*(256+reg*8+sel)($k0);   \
        dmtc0 $k1, $##reg, sel;

#define SAVE_ADDRESS(offset, addr)      \
        dli $k1, addr;                   \
        ld $k1, 0($k1);                   \
        sd $k1, 8*(offset)($k0);

#define RESTORE_ADDRESS(offset, addr)   \
        dli $t0, addr;                   \
        ld $k1, 8*(offset)($k0);          \
        sd $k1, 0($t0);

.macro  SETUP_K0_DEBUG_DATA_PER_CORE
        dmtc0   $k0, $COP0_DESAVE    /* Save k0 */
        mfc0    $k0, $15, 1          /* Read exception base register */
        andi    $k0, 0xff            /* Mask off core ID */
        sll     $k0, 14              /* Multiply by 16384. This is the bytes per core */
        daddiu  $k0, $k0, OFFSET_14_0 /* We use 15 bits instead of 16 to avoid sign extend of the immediate */
        drotr   $k0, $k0, 15
        daddiu  $k0, $k0, OFFSET_29_15
        drotr   $k0, $k0, 15
        daddiu  $k0, $k0, OFFSET_44_30
        drotr   $k0, $k0, 15
        daddiu  $k0, $k0, OFFSET_59_45
        drotr   $k0, $k0, 15
        daddiu  $k0, $k0, OFFSET_63_60
        drotr   $k0, $k0, 4
.endm
