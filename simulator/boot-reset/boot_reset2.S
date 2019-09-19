/*************************************************************************
Copyright (c) 2003-2004, Cavium Inc.. All rights reserved.
 
This Software is the property of Cavium Inc..  The Software and all 
accompanying documentation are copyrighted.  The Software made available 
here constitutes the proprietary information of Cavium Inc..  You 
agree to take reasonable steps to prevent the disclosure, unauthorized use 
or unauthorized distribution of the Software.  You shall use this Software 
solely with Cavium hardware.  
 
Except as expressly permitted in a separate Software License Agreement 
between You and Cavium Inc., you shall not modify, decompile, 
disassemble, extract, or otherwise reverse engineer this Software.  You 
shall not make any copy of the Software or its accompanying documentation, 
except for copying incident to the ordinary and intended use of the 
Software and the Underlying Program and except for the making of a single 
archival copy.  
 
This Software, including technical data, may be subject to U.S.  export 
control laws, including the U.S.  Export Administration Act and its 
associated regulations, and may be subject to export or import regulations 
in other countries.  You warrant that You will comply strictly in all 
respects with all such regulations and acknowledge that you have the 
responsibility to obtain licenses to export, re-export or import the 
Software.  
 
TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
AND WITH ALL FAULTS AND CAVIUM MAKES NO PROMISES, REPRESENTATIONS OR 
WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT 
TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY 
REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT 
DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES 
OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR 
PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET 
POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT 
OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.  

*************************************************************************/
#include "reset.h"
.set noreorder


/*  This is a very basic reset vector to allow support booting of 
various operating systems on Octeon.  Change START_ADDR to be the address
to execute after the reset init. has completed.

*/


#ifndef START_ADDR
#define START_ADDR 0xffffffff80580000
#endif


	
// general exception handler, break,trap,tlb,syscall,iov
.=0x180
LTrapEntry:
	mfc0    k1, COP0_EPC_REG		  // this is here for compare point
	mfc0	k1, COP0_CAUSE_REG
	dext	k1, k1, 2, 5	                  // extract cause
    eret
    nop
    break   1
	break   1

.=PHASE_2_RESET_LOC
__reset2:
	/// initialize the TLB
        dla     v0,0x20000000
        mtc0    v0, COP0_PAGEGRAIN_REG

InitTLB:
        dmtc0   zero, COP0_ENTRYLO0_REG
        mfc0    a0, COP0_CONFIG1_REG
        dmtc0   zero, COP0_ENTRYLO1_REG
        srl     a0, a0, 25
        mtc0    zero, COP0_PAGEMASK_REG
        dla     v0, 0xFFFFFFFF80000000 // kseg0 is unmapped, so use this
        and     a0, a0, 0x3F // a0 now has the max mmu entry index
        dmtc0   zero, COP0_CONTEXT_REG
        dmtc0   zero, COP0_XCONTEXT_REG
        mtc0    zero, COP0_WIRED_REG
InitTLBloop:
        dmtc0   v0, COP0_ENTRYHI_REG
        ehb
        tlbp
        ehb
        mfc0    v1, COP0_INDEX_REG
        daddiu  v0, v0, 1<<13
        bgez    v1, InitTLBloop
        nop

        mtc0    a0, COP0_INDEX_REG
        ehb
        tlbwi
        ehb
        bne     a0, zero, InitTLBloop
        addiu   a0, -1

	/// initialize the registers
	move	$2,zero
	move	$3,zero
	move	$4,zero
	move	$5,zero
	move	$6,zero
	move	$7,zero
	move	$8,zero
	move	$9,zero
	move	$10,zero
	move	$11,zero
	move	$12,zero
	move	$13,zero
	move	$14,zero
	move	$15,zero
	move	$16,zero
	move	$17,zero
	move	$18,zero
	move	$19,zero
	move	$20,zero
	move	$21,zero
	move	$22,zero
	move	$23,zero
	move	$24,zero
	move	$25,zero
	move	$26,zero
	move	$27,zero
	move	$28,zero
	move	$29,zero
	move	$30,zero
	move	$31,zero
	mthi	zero
	mtlo	zero
	mtm0	zero
	mtm1	zero
	mtm2	zero
	mtp0	zero
	mtp1	zero
	mtp2	zero

	mtc0    zero, COP0_COUNT_REG
        mtc0    zero, COP0_COMPARE_REG // try to make the timer interrupt happen a long time from now
        // dmtc0   zero, COP0_CVMCOUNT_REG
        mtc0    zero, COP0_CAUSE_REG
        dla     v0,0x5080FFE5
        mtc0    v0, COP0_STATUS_REG

#define CIU_PP_RST                     0x8001070000000700 // use XKPHYS address
        sd      zero, CIU_PP_RST  // enable all the other cores, if not already

        dla     v0,0xC000000F // enable all
        mtc0    v0, COP0_HWRENA_REG
        
#ifdef USE_CAVIUM_UNALIGNED_LOADS
        dla     v0,0x5BFD
#else
        dla     v0,0x0BFD
#endif
        dmtc0   v0, COP0_CVMCTL_REG
        dla     v0,0x6104
        dmtc0   v0, COP0_CVMMEMCTL_REG

	dla	v0, START_ADDR
	dmtc0	v0, COP0_ERROREPC_REG


    // clear these to avoid immediate interrupt in noperf mode
	mtc0	zero, COP0_COMPARE_REG            // clear timer interrupt
	mtc0	zero, COP0_PERF_CNT0              // clear perfCnt0
	mtc0	zero, COP0_PERF_CNT1              // clear perfCnt1
    
    
	eret
	nop

    break   1
	break   1

