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


/*
 * File version info: $Id: reset.h 69916 2012-02-14 14:47:03Z bprakash $
 */
 

#include <machine/asm.h>
#include <machine/regdef.h>

//#ifndef NO_HACK_FOR_NOW
//#define HACK_FOR_NOW // legacy
//#endif
#define HAVE_TLB_MISSES

#define COP0_HWRENA_REG $7,0
#define COP0_COUNT_REG $9,0
#define COP0_CVMCOUNT_REG $9,6
#define COP0_CVMCTL_REG $9,7
#define COP0_COMPARE_REG $11,0
#define COP0_CVMMEMCTL_REG $11,7
#define COP0_STATUS_REG $12,0
#define COP0_CAUSE_REG $13,0
#define COP0_PAGEGRAIN_REG $5,1
#define COP0_ENTRYLO0_REG $2,0
#define COP0_ENTRYLO1_REG $3,0
#define COP0_PAGEMASK_REG $5,0
#define COP0_ENTRYHI_REG $10,0
#define COP0_INDEX_REG $0,0
#define COP0_CONFIG1_REG $16,1
#define COP0_CONTEXT_REG $4,0
#define COP0_XCONTEXT_REG $20,0
#define COP0_WIRED_REG $6,0
#define COP0_EPC_REG $14,0
#define COP0_ERROREPC_REG $30,0
#define COP0_BADVADDR_REG $8,0
#define COP0_DEPC_REG $24,0
#define COP0_DEBUG_REG $23,0
#define COP0_MDEBUG_REG $22,0
#define COP0_DESAVE_REG $31,0
#define COP0_PERF_CNT0 $25,1
#define COP0_PERF_CNT1 $25,3

#ifdef HACK_FOR_NOW
#define PHASE_2_RESET_LOC 0
#else
#define PHASE_2_RESET_LOC 0x800 // choose an unused spot in DRAM
#endif

#define DEBUG_INST_LOC 0x1800
#define PHASE_2_DEBUG_LOC (DEBUG_INST_LOC + 128)

#define REGSIZE 8
#define SIZEOFSTACK 400

#define CVMX_DEBUG_STACK_ADDR			(0xb000000000201400)	
