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
 * Display the state of an Octeon core
 *
 * $Id: oct-remote-save.c 40311 2009-01-28 20:03:12Z kreese $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cvmx.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "octeon-model.h"

#define ULL unsigned long long

static const char *CPU_REGS[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
                                 "a4", "a5", "a6", "a7", "t0", "t1", "t2", "t3",
                                 "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
                                 "t8", "t9", "k0", "k1", "gp", "sp", "s8/fp", "ra"};

static const char *EXCEPTION_CODES[] = {
    "Interrupt",
    "TLB modification",
    "TLB (load or fetch)",
    "TLB store",
    "Address error (load or fetch)",
    "Address error (store)",
    "Bus error (fetch)",
    "Bus error (data reference, load or store)",
    "Syscall",
    "Breakpoint",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap",
    "Reserved",
    "Floating point",
    "Implementation dependent",
    "Implementation dependent",
    "Precise COP2",
    "Reserved",
    "Reserved",
    "Reserved",
    "MDMX unusable",
    "Reference to WatchHi/WatchLo addr",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Cache Error",
    "Reserved"
};
static struct
{
    int reg;
    int sel;
    const char *name;
} COP0_REGS[] = {
    {0, 0, "Index"},	// 0
    {1, 0, "Random"},
    {2, 0, "EntryLo0"},
    {3, 0, "EntryLo1"},
    {4, 0, "Context"},
    {5, 0, "PageMask"},
    {5, 1, "PageGrain"},
    {6, 0, "Wired"},
    {7, 0, "HWREna"},
    {8, 0, "BadVAddr"}, 
    {9, 0, "Count"},	// 10
    {10, 0, "EntryHi"},
    {11, 0, "Compare"},
    {12, 0, "Status"},
    {12, 1, "IntCtl"},
    {12, 2, "SRSCtl"},
    {13, 0, "Cause"},
    {14, 0, "EPC"},
    {15, 0, "PRId"},
    {15, 1, "Ebase"},
    {16, 0, "Config"},	// 20
    {16, 1, "Config1"},
    {16, 2, "Config2"},
    {16, 3, "Config3"},
    {18, 0, "WatchLo0"},
    {18, 1, "WatchLo1"},
    {19, 0, "WatchHi0"},
    {19, 1, "WatchHi1"},
    {20, 0, "XContext"},
    {23, 0, "Debug"},
    {25, 0, "PerfControl0"},	// 30
    {25, 2, "PerfControl1"},
    {24, 0, "DEPC"},
    {25, 1, "PerfCount0"},
    {25, 3, "PerfCount1"},
    {27, 0, "IcacheError"},
    {27, 1, "DcacheError"},
    {28, 0, "IcacheTagLo"},
    {28, 1, "IcacheDataLo"},
    {28, 2, "DcacheTagLo"},
    {28, 3, "DcacheDataLo"},	// 40
    {29, 1, "IcacheDataHi"},
    {29, 2, "DcacheTagHi"},
    {29, 3, "DcacheDataHi"},
    {30, 0, "ErrorEPC"},
    {31, 0, "DESAVE"},
    {9, 7, "CvmCtl"},
    {11, 7, "CvmMemCtl"},
    {9, 6, "CvmCount"},
    {22, 0, "MultiCoreDebug"},
    {4, 2, "UserLocal"}, /* Added in cn63XX */ // 50
    {16, 4, "Config4"},
    {11, 6, "PowThr"},
    {23, 6, "Debug2"},
    {31, 2, "Kscratch1"},
    {31, 3, "Kscratch2"},
    {31, 4, "Kscratch3"},
    {31, 5, "Kscratch4"}, /* Added in cn70XX */
    {8, 1, "BadInstr"},
    {8, 2, "BadInstrP"},
    {10, 5, "GuestCtl2"},	// 60
    {12, 6, "GuestCtl0"},
    {12, 7, "GTOffset"},
    {16, 5, "Config5"},
    {16, 6, "VMMemCtl2"},
    {16, 7, "CvmVMConfig"},
    {22, 2, "CvmCountOffset"},
    {25, 4, "PerfControl2"},
    {25, 6, "PerfControl3"},
    {25, 5, "PerfCount2"},
    {25, 7, "PerfCount3"},	// 70
    {26, 0, "ErrCtl"},
    {27, 2, "IcacheDebug"},
    {28, 4, "TagLo1"},
    {0, 0, NULL}
};

typedef union {
    uint64_t val;
    float flt;
    double dbl;
} fpu_reg_t;

static void dump_core(int core, octeon_remote_registers_t *registers)
{
    int r;
    printf("Core %d\n", core);
    printf("CPU registers:\n");
    for (r=0; r<16; r++)
    {
        printf("  %5s (r%d)\t0x%016llx\t%5s (r%d)\t0x%016llx\n",
            CPU_REGS[r], r, (ULL)registers->regs[0][r],
            CPU_REGS[r+16], r+16, (ULL)registers->regs[0][r+16]);
    }
    printf("  %5s\t\t0x%016llx\t%5s\t\t0x%016llx\n", "lo", (ULL)registers->regs[0][32], "hi", (ULL)registers->regs[0][33]);
    /* Print FP registers only when Config1[FP] is set */
    if ((ULL)registers->regs[1][COP0_REGS[21].reg*8 + COP0_REGS[21].sel] & 1)
    {
        fpu_reg_t fp;
        printf("FPU registers:\n");
        for (r=0; r<32; r++)
        {
            fp.val = registers->regs[0][r+64];
            printf("   f%d:0x%llx\tflt:%-17.9g\tdbl:%-24.17g\n",
                r, (ULL)fp.val, fp.flt, fp.dbl);
        }
        /* fir and fcsr are at offset 96 (64+32). */ 
        fp.dbl = registers->regs[0][96];
        printf("   fir:%-24.17g\n", fp.dbl);
        fp.dbl = registers->regs[0][97];
        printf("   fcsr:%-24.17g\n", fp.dbl);
    }
    printf("Coprocessor 0 registers:\n");
    int num_cop0 = 0;
    while (COP0_REGS[num_cop0].name)
        num_cop0++;
    /* The last 18 are added in Octeon3 and 7 are added in Octeon 2 */
    if (OCTEON_IS_OCTEON1PLUS())
        num_cop0 -= 24;
    else if (OCTEON_IS_OCTEON2())
        num_cop0 -= 17;
    int half = (num_cop0+1)/2;
    /* Skip printing COP0 registers when in GUEST mode */
    if (OCTEON_IS_OCTEON3() &&
       ((ULL)registers->regs[1][COP0_REGS[61].reg*8 + COP0_REGS[61].sel] >> 31))
    {
            printf("Core %d is in Guest mode, skipping \n", core);
            goto Lprint_tlb_entries;

    }
    for (r=0; r<half; r++)
    {
        printf("  ($%2d,%d) %-9s 0x%016llx", COP0_REGS[r].reg, COP0_REGS[r].sel,
            COP0_REGS[r].name, (ULL)registers->regs[1][COP0_REGS[r].reg*8 + COP0_REGS[r].sel]);
        if (r + half < num_cop0)
            printf("  ($%2d,%d) %-14s 0x%016llx\n", COP0_REGS[r+half].reg, COP0_REGS[r+half].sel,
                COP0_REGS[r+half].name, (ULL)registers->regs[1][COP0_REGS[r+half].reg*8 + COP0_REGS[r+half].sel]);
    }
    if (num_cop0 & 1)
        printf("\n");
    /* Decode the cause register, if in exception context (Status reg bit 1) */
    if ((ULL)registers->regs[1][COP0_REGS[13].reg*8 + COP0_REGS[13].sel] & 0x2)
    {
        int exc_code = ((ULL)registers->regs[1][COP0_REGS[16].reg*8 + COP0_REGS[13].sel] >> 2) & 0x1f;
        printf("Exc code: 0x%x, %s\n", exc_code, EXCEPTION_CODES[exc_code]);
    }

Lprint_tlb_entries:

    /* Skip printing the TLB if the first two entries have the same virtual
        address. The underlying protocol probably can't get the TLB */
    if (registers->tlb[0][0] == registers->tlb[1][0])
        return;

    printf("TLB:\n");
    int tlb_num;
    if (OCTEON_IS_MODEL(OCTEON_CN3XXX))
        tlb_num = 32;
    else if (OCTEON_IS_MODEL(OCTEON_CN5XXX))
        tlb_num = 64;
    else if (OCTEON_IS_OCTEON2())
        tlb_num = 128;
    else
        /* Read the tlb entries from CvmVMConfig COP0 register */
        tlb_num = (registers->regs[1][COP0_REGS[65].reg*8 + COP0_REGS[65].sel] & 0xff) + 1;
    for (r=0; r<tlb_num; r++)
    {
        uint64_t va = registers->tlb[r][0]>>12<<12;
        if ((va>>62) == 3)
            va |= 0x3ffe000000000000ull;
        uint8_t  asid       = registers->tlb[r][0] & 0xff;
        uint64_t pagemask   = (registers->tlb[r][1] | 0x1fff) >> 1;
        int      pagesize   = 1<<(cvmx_pop(pagemask)-10);
        uint64_t page0      = registers->tlb[r][2]>>6<<12;
        int      cache0     = (registers->tlb[r][2]>>3) & 7;
        int      dirty0     = (registers->tlb[r][2]>>2) & 1;
        int      valid0     = (registers->tlb[r][2]>>1) & 1;
        int      glbl0      = registers->tlb[r][2] & 1;
        int      ri0        = (registers->tlb[r][2]>>63) & 1;
        int      xi0        = (registers->tlb[r][2]>>62) & 1;
        uint64_t page1      = registers->tlb[r][3]>>6<<12;
        int      cache1     = (registers->tlb[r][3]>>3) & 7;
        int      dirty1     = (registers->tlb[r][3]>>2) & 1;
        int      valid1     = (registers->tlb[r][3]>>1) & 1;
        int      glbl1      = registers->tlb[r][3] & 1;
        int      ri1        = (registers->tlb[r][3]>>63) & 1;
        int      xi1        = (registers->tlb[r][3]>>62) & 1;
        printf("%2d: Virtual=0x%016llx Page0=0x%09llx,C=%d,D=%d,V=%d,G=%d,RI=%d,XI=%d Page1=0x%09llx,C=%d,D=%d,V=%d,G=%d,RI=%d,XI=%d ASID=%3d Size=%dKB\n",
            r, (ULL)va, (ULL)page0, cache0, dirty0, valid0, glbl0, ri0, xi0, (ULL)page1, cache1, dirty1, valid1, glbl1, ri1, xi1, asid, pagesize);
    }
}

void usage(const char *argv[])
{
    printf("\n"
           "Usage: %s <core>\n"
           "    Display the state of an Octeon core.\n"
           "\n"
           "    core - Octeon core to display.\n"
           "\n", argv[0]);
}

int main(int argc, const char *argv[])
{
    int num_cores;
    if (print_help_message(usage, argv, argc) == -1)
        return -1;

    if (argc != 2)
    {
        usage(argv);
        return -1;
    }

    int core = atoi(argv[1]);

    int status = octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0);
    if (status)
        return status;

    num_cores = cvmx_octeon_num_cores();
    if (core >= num_cores)
    {
        printf("Requested core(%d) is not available, available cores are [0-%d]\n", core, (num_cores-1)); 
        octeon_remote_close();
        return -1;
    }
    octeon_remote_registers_t registers;
    octeon_remote_stop_cores(1ull<<core);
    status = octeon_remote_get_core_state(core, &registers);
    octeon_remote_start_cores(1ull<<core);
    if (status)
        printf("Getting the Core state failed with %d\n",  status);
    else
        dump_core(core, &registers);
    octeon_remote_close();
    return status;
}
