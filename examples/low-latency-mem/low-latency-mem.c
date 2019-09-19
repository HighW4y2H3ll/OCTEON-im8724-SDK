/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
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


/*
 * File version info: $Id: low-latency-mem.c 94793 2014-03-05 23:52:54Z ddaney $
 *
 */

#include <stdio.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-llm.h"
#include "cvmx-fau.h"
#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"

#define DATA_SET_SIZE       1024

#define REPLICATION_CODE    CVMX_LLM_REPLICATION_2X
#define REPLICATION_AMOUNT  (1<<REPLICATION_CODE)

#define REPLICATION_CODE2   CVMX_LLM_REPLICATION_8X
#define REPLICATION_AMOUNT2  (1<<REPLICATION_CODE2)

#define UNREPLICATED_BASE   0
#define UNREPLICATED_BASE2  (UNREPLICATED_BASE + DATA_SET_SIZE*4)
#define REPLICATED_BASE     (UNREPLICATED_BASE2 + DATA_SET_SIZE*4)
#define REPLICATED_BASE2    (REPLICATED_BASE + DATA_SET_SIZE*4*REPLICATION_AMOUNT)

#define UNREPLICATED_BASE64 (REPLICATED_BASE2 + DATA_SET_SIZE*4*REPLICATION_AMOUNT2)
#define REPLICATED_BASE64   (UNREPLICATED_BASE64 + DATA_SET_SIZE*4)
#define UNREPLICATED_BASE642 (REPLICATED_BASE64 + DATA_SET_SIZE*4*REPLICATION_AMOUNT)
#define REPLICATED_BASE642  (UNREPLICATED_BASE642 + DATA_SET_SIZE*4)

#define FAU_ERRORS (CVMX_FAU_REG_AVAIL_BASE + 0)    /* Used to count test errors */

typedef union
{
    uint64_t u64;
    struct
    {
        uint64_t upper : 32;
        uint64_t lower : 32;
    } s;
} FIT64;


/**
 * Simple utility function to initialize a LLM address
 *
 * @param address Starting location
 * @param repl    Replication type
 * @return Complete LLM address
 */
static inline cvmx_llm_address_t build_llm_addr(uint64_t address, cvmx_llm_replication_t repl)
{
    cvmx_llm_address_t result;

    result.u64 = 0;
    result.s.address = address;
    result.s.repl = repl;

    return result;
}


/**
 * Write incrementing numbers to low latency memory. The 36 bit
 * words in low latency memory starting at the supplied
 * address will be filled with the numbers 0 to size-1. Data
 * will be replicated as necessary by the hardware.
 *
 * @param add    Address to begin writing to
 * @param size   Number of llm bit words to write
 */
static void init_slow(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t        i;
    cvmx_llm_data_t dat;

    /* Loop inserting incrementing numbers */
    for (i = 0; i < size; i++)
    {
        dat.u64 = i;
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            dat.cn31xx_par.par = cvmx_llm_parity(i);
        else
            dat.cn38xx_par.par = cvmx_llm_parity(i);
        cvmx_llm_write36(add, dat.u64, 0);
        add.u64 += 4; /* Increment our address. Each entry is "4 bytes" away */
    }
}


/**
 * Faster function to write incrementing numbers to low
 * latency memory. The 36 bit words in low latency memory
 * starting at the supplied address will be filled with the
 * numbers 0 to size-1. Data will be replicated as necessary
 * by the hardware.
 *
 * This version uses writes to all four sets so may be
 * somewhat faster (perhaps none, though)
 *
 * @param add    Address to begin writing to
 * @param size   Number of llm bit words to write
 */
static void init_fast(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t        i;
    cvmx_llm_data_t dat;

    /* Loop inserting incrementing numbers */
    for (i = 0; i < size; i+=2)
    {
        dat.u64 = i;
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            dat.cn31xx_par.par = cvmx_llm_parity(i);
        else
            dat.cn38xx_par.par = cvmx_llm_parity(i);
        cvmx_llm_write36(add, dat.u64, 0);
        add.u64 += 4;
        dat.u64 = i+1;
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            dat.cn31xx_par.par = cvmx_llm_parity(i+1);
        else
            dat.cn38xx_par.par = cvmx_llm_parity(i+1);
        cvmx_llm_write36(add, dat.u64, 1);
        add.u64 += 4;
    }
}


/**
 * Similar to init_slow, only with two 32-bit values rather
 * than one 64-bit value. Note that ECC is provided so no
 * parity tricks required
 *
 * @param add    Address to begin writing to
 * @param size   Number of llm bit words to write
 */
static void init_slow64(cvmx_llm_address_t add, uint64_t size)
{
    FIT64       dat;
    uint64_t    i;

    /* Loop inserting incrementing numbers */
    for (i = 0; i < size; i += 2)
    {
        dat.s.upper = i;   /* The upper section contains the first data */
        dat.s.lower = i+1; /* The lower section contains the second data */
        cvmx_llm_write64(add, dat.u64, 0);
        add.u64 += 8;
    }
}


/**
 * Similar to init_fast, only with two 32-bit values rather
 * than one 64-bit value. Note that ECC is provided so no
 * parity tricks required
 *
 * @param add    Address to begin writing to
 * @param size   Number of llm bit words to write
 */
static void init_fast64(cvmx_llm_address_t add, uint64_t size)
{
    FIT64       dat;
    uint64_t    i;

    /* Loop inserting incrementing numbers */
    for (i = 0; i < size; i += 4)
    {
        dat.s.upper = i;   /* The upper section contains the first data */
        dat.s.lower = i+1; /* The lower section contains the second data */
        cvmx_llm_write64(add, dat.u64, 0);
        add.u64 += 8;
        dat.s.upper = i+2; /* The upper section contains the first data */
        dat.s.lower = i+3; /* The lower section contains the second data */
        cvmx_llm_write64(add, dat.u64, 1);
        add.u64 += 8;
    }
}


/**
 * Simple function to add all the (35-bit) memory locations
 * in a range.
 *
 * @param add    Address to begin adding at
 * @param size   Number of llm bit words to add
 * @return The result
 */
static inline uint64_t add_slow(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t        total = 0;
    uint64_t        i;
    cvmx_llm_data_t dat;

    /* Loop adding numbers */
    for (i = 0; i < size; i++)
    {
        CVMX_MT_LLM_READ_ADDR(0,add.u64);   /* This launches the fetch to read the data */
        add.u64 += 4;                       /* Increment our address. Each entry is "4 bytes" away */
        CVMX_MF_LLM_DATA(0,dat.u64);        /* Get the data into a register */
        if (dat.s.xxor)
        {
            printf("Parity error: addr=0x%016llx data=0x%016llx\n", (unsigned long long)add.u64, (unsigned long long)dat.u64);
            return 0;
        }
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            total += dat.cn31xx_par.dat;    /* 31 bits of useful data here */
        else
            total += dat.cn38xx_par.dat;    /* 35 bits of useful data here */
    }

    return total;
}


/**
 * Simple program to add all the (32-bit) memory locations in
 * a range
 *
 * @param add    Address to begin adding at
 * @param size   Number of llm bit words to add
 * @return The result
 */
static inline uint64_t add_slow64(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t    total = 0;
    uint64_t    i;
    FIT64       dat;

    /* Loop adding numbers */
    for (i = 0; i < size; i += 2)
    {
        CVMX_MT_LLM_READ64_ADDR(0,add.u64); /* this launches the fetch to read the data */
        add.u64 += 8;                       /* Increment our address. Each entry is "8 bytes" away */
        CVMX_MF_LLM_DATA(0,dat.u64);        /* get the data into a register */
        total += dat.s.upper;               /* Add the first result returned */
        total += dat.s.lower;               /* Add the second result returned */
    }

    return total;
}


/**
 * Fast version of add_slow to add all the (35-bit) memory
 * locations in a range. Should be ~2x faster than the slow
 * version.
 *
 * @param add    Address to begin adding at
 * @param size   Number of llm bit words to add
 * @return The result
 */
static inline uint64_t add_fast(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t        total = 0;
    uint64_t        i;
    cvmx_llm_data_t dat;

    /* Launch the first two fetches to read the data, each to an independent set */
    CVMX_MT_LLM_READ_ADDR(0,add.u64);
    add.u64 += 4;
    CVMX_MT_LLM_READ_ADDR(1,add.u64);
    add.u64 += 4;
    size -= 2;

    /* Loop adding numbers */
    for (i = 0; i < size; i += 2)
    {
        CVMX_MF_LLM_DATA(0,dat.u64);        /* get the previously fetched data into a register */
        CVMX_MT_LLM_READ_ADDR(0,add.u64);   /* launch the fetch to read the next piece of data */
        add.u64 += 4;                       /* Increment our address. Each entry is "4 bytes" away */
        if (dat.s.xxor)
        {
            printf("Parity error: addr=0x%016llx data=0x%016llx\n", (unsigned long long)add.u64, (unsigned long long)dat.u64);
            return 0;
        }
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            total += dat.cn31xx_par.dat;    /* 31 bits of useful data here */
        else
            total += dat.cn38xx_par.dat;    /* 35 bits of useful data here */
        CVMX_MF_LLM_DATA(1,dat.u64);        /* get the previously fetched data into a register */
        CVMX_MT_LLM_READ_ADDR(1,add.u64);   /* launch the fetch to read the next piece of data */
        add.u64 += 4;                       /* Increment our address. Each entry is "4 bytes" away */
        if (dat.s.xxor)
        {
            printf("Parity error: addr=0x%016llx data=0x%016llx\n", (unsigned long long)add.u64, (unsigned long long)dat.u64);
            return 0;
        }
        if (OCTEON_IS_MODEL(OCTEON_CN31XX))
            total += dat.cn31xx_par.dat;    /* 31 bits of useful data here */
        else
            total += dat.cn38xx_par.dat;    /* 35 bits of useful data here */
    }

    CVMX_MF_LLM_DATA(0,dat.u64);            /* get the previously fetched data into a register */
    if (dat.s.xxor)
    {
        printf("Parity error: addr=0x%016llx data=0x%016llx\n", (unsigned long long)add.u64, (unsigned long long)dat.u64);
        return 0;
    }
    if (OCTEON_IS_MODEL(OCTEON_CN31XX))
        total += dat.cn31xx_par.dat;        /* 31 bits of useful data here */
    else
        total += dat.cn38xx_par.dat;        /* 35 bits of useful data here */
    CVMX_MF_LLM_DATA(1,dat.u64);            /* get the previously fetched data into a register */
    if (dat.s.xxor)
    {
        printf("Parity error: addr=0x%016llx data=0x%016llx\n", (unsigned long long)add.u64, (unsigned long long)dat.u64);
        return 0;
    }
    if (OCTEON_IS_MODEL(OCTEON_CN31XX))
        total += dat.cn31xx_par.dat;        /* 31 bits of useful data here */
    else
        total += dat.cn38xx_par.dat;        /* 35 bits of useful data here */

    return total;
}


/**
 * Faster version of add_slow64 to add all the (32-bit) memory
 * locations in a range. Should be ~2x faster than the slow
 * version
 *
 * @param add    Address to begin adding at
 * @param size   Number of llm bit words to add
 * @return The result
 */
static inline uint64_t add_fast64(cvmx_llm_address_t add, uint64_t size)
{
    uint64_t    total = 0;
    uint64_t    i;
    FIT64       dat;

    /* Launch the first two fetches to read the data, each to an independent set */
    CVMX_MT_LLM_READ64_ADDR(0,add.u64);
    add.u64 += 8;
    CVMX_MT_LLM_READ64_ADDR(1,add.u64);
    add.u64 += 8;
    size -= 4;

    /* Loop adding numbers */
    for (i = 0; i < size; i += 4)
    {
        CVMX_MF_LLM_DATA(0,dat.u64);        /* get the previous data into a register */
        CVMX_MT_LLM_READ64_ADDR(0,add.u64); /* this launches the fetch to read the next data */
        add.u64 += 8;                       /* Increment our address. Each entry is "8 bytes" away */
        total += dat.s.upper;               /* Add the first result returned */
        total += dat.s.lower;               /* Add the second result returned */
        CVMX_MF_LLM_DATA(1,dat.u64);        /* get the previous data into a register */
        CVMX_MT_LLM_READ64_ADDR(1,add.u64); /* this launches the fetch to read the next data */
        add.u64 += 8;                       /* Increment our address. Each entry is "8 bytes" away */
        total += dat.s.upper;               /* Add the first result returned */
        total += dat.s.lower;               /* Add the second result returned */
    }

    CVMX_MF_LLM_DATA(0,dat.u64);            /* get the previous data into a register */
    total += dat.s.upper;                   /* Add the first result returned */
    total += dat.s.lower;                   /* Add the second result returned */
    CVMX_MF_LLM_DATA(1,dat.u64);            /* get the previous data into a register */
    total += dat.s.upper;                   /* Add the first result returned */
    total += dat.s.lower;                   /* Add the second result returned */

    return total;
}


/**
 * Initialize LLM
 */
static void do_init(void)
{
    cvmx_llm_initialize();
    cvmx_hwfau_atomic_write64(FAU_ERRORS, 0);

    printf("Initializing Low latency Memory\n");

    init_slow(build_llm_addr(UNREPLICATED_BASE, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    init_fast(build_llm_addr(UNREPLICATED_BASE2, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    init_slow(build_llm_addr(REPLICATED_BASE, REPLICATION_CODE), DATA_SET_SIZE);
    init_fast(build_llm_addr(REPLICATED_BASE2, REPLICATION_CODE2), DATA_SET_SIZE);
    init_slow64(build_llm_addr(UNREPLICATED_BASE64, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    init_fast64(build_llm_addr(UNREPLICATED_BASE642, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    init_slow64(build_llm_addr(REPLICATED_BASE64, REPLICATION_CODE), DATA_SET_SIZE);
    init_fast64(build_llm_addr(REPLICATED_BASE642, REPLICATION_CODE2), DATA_SET_SIZE);

    printf("Initialization Complete\n");
}


/**
 * Called between test to insure synchronization and no bus
 * contention
 *
 * @param core_mask Mask showing the cores running the test
 */
static void do_wait(cvmx_coremask_t *core_mask)
{
    uint64_t end_time = cvmx_get_cycle() + 50000;

    while (cvmx_get_cycle() < end_time)
    {
        /* Spin so there is no extra bus contention affecting the test */
    }

    cvmx_coremask_barrier_sync(core_mask);

    end_time = cvmx_get_cycle() + 50000;
    while (cvmx_get_cycle() < end_time)
    {
        /* Spin so there is no extra bus contention affecting the test */
    }
}


/**
 * Run all of the LLM tests
 *
 * @param core_mask Cores running the test.
 * @param totals    Results from the tests
 * @param cycles    Test time in cycles for each test
 */
static void do_tests(cvmx_coremask_t *core_mask, uint64_t totals[], uint64_t cycles[])
{
    uint64_t start_time;

    CVMX_SYNC;

    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[0] = add_slow(build_llm_addr(UNREPLICATED_BASE, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    cycles[0] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[1] = add_fast(build_llm_addr(UNREPLICATED_BASE2, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    cycles[1] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[2] = add_slow(build_llm_addr(REPLICATED_BASE, REPLICATION_CODE), DATA_SET_SIZE);
    cycles[2] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[3] = add_fast(build_llm_addr(REPLICATED_BASE2, REPLICATION_CODE2), DATA_SET_SIZE);
    cycles[3] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[4] = add_slow64(build_llm_addr(UNREPLICATED_BASE64, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    cycles[4] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[5] = add_fast64(build_llm_addr(UNREPLICATED_BASE642, CVMX_LLM_REPLICATION_NONE), DATA_SET_SIZE);
    cycles[5] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[6] = add_slow64(build_llm_addr(REPLICATED_BASE64, REPLICATION_CODE), DATA_SET_SIZE);
    cycles[6] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
    start_time = cvmx_get_cycle();
    totals[7] = add_fast64(build_llm_addr(REPLICATED_BASE642, REPLICATION_CODE2), DATA_SET_SIZE);
    cycles[7] = cvmx_get_cycle() - start_time;
    do_wait(core_mask);
}


/**
 * Display the results of a test for the current processor.
 * This must be called for every processor.
 *
 * @param data_set Test to display
 * @param correct_cycles
 *                 Expect run time of the test in cycles
 */
static void do_display(const char *name, int replication, uint64_t total, uint64_t cycles)
{
    uint64_t correct_result = (uint64_t)DATA_SET_SIZE * (DATA_SET_SIZE - 1) / 2;
    int is_result_error = (total != correct_result);
    char sum_msg[100] = {0};

    if (is_result_error)
    {
        snprintf(sum_msg, 100, " [FAIL: Incorrect Sum, correct is: %6llu]", (unsigned long long)correct_result);
        cvmx_hwfau_atomic_add64(FAU_ERRORS, 1);
    }

    printf("Core %2d: %-10s %dx sum = %6llu in %6llu cycles%s\n",
           (int)cvmx_get_core_num(),
           name,
           1<<replication,
           (unsigned long long)total,
           (unsigned long long)cycles,
           sum_msg);
}


/**
 * Main entry point
 *
 * @return
 */
int main()
{
    static CVMX_SHARED uint64_t average_timing[8];      /* Average timing for all processors on each test */
    const char *names[] = {"add_slow", "add_fast", "add_slow", "add_fast",
                           "add_slow64", "add_fast64", "add_slow64", "add_fast64"};
    const int   replication[] = {CVMX_LLM_REPLICATION_NONE, CVMX_LLM_REPLICATION_NONE, REPLICATION_CODE, REPLICATION_CODE2,
                                 CVMX_LLM_REPLICATION_NONE, CVMX_LLM_REPLICATION_NONE, REPLICATION_CODE, REPLICATION_CODE2};
    cvmx_sysinfo_t *sysinfo;
    cvmx_coremask_t *core_mask;
    uint64_t        test;
    uint64_t        core;
    uint64_t        totals[8];
    uint64_t        cycles[8];
    uint64_t        num_cores;

    cvmx_user_app_init();

    sysinfo = cvmx_sysinfo_get();
    core_mask = &sysinfo->core_mask;
    
    num_cores = cvmx_coremask_get_core_count(core_mask);


    /* First core does initialization for all the cores */
    if (cvmx_is_init_core())
        do_init();

    /* All cores needs to finish llm initialization before refering LLM regs.*/
    cvmx_coremask_barrier_sync(core_mask);

    do_tests(core_mask, totals, cycles);

    /* Display the results of each test */
    for (test=0; test<8; test++)
    {
        /* Show each core in incrementing order */
        for (core=0; core < CVMX_MAX_CORES; core++)
        {
            if (core == cvmx_get_core_num())
            {
                do_display(names[test], replication[test], totals[test], cycles[test]);
                average_timing[test] += cycles[test];
            }
            /* Other cores wait here for the display to complete */
            cvmx_coremask_barrier_sync(core_mask);
        }
    }

    /* The first core needs to display some additional info */
    if (cvmx_is_init_core())
    {
        printf("\n");
        for (test=0; test<8; test++)
        {
            average_timing[test] /= num_cores;
            printf("Average: %-10s %dx = %6llu cycles\n",
                   names[test],
                   1<<replication[test],
                   (unsigned long long)average_timing[test]);
        }

        /* Uncomment the following printf if you modify this test and need
            to regenerate the expected cycles table. */
        // iprintf("    {%lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu}, /* %u */\n",
        //        average_timing[0], average_timing[1], average_timing[2], average_timing[3],
        //        average_timing[4], average_timing[5], average_timing[6], average_timing[7],
        //        num_cores);

        if (cvmx_hwfau_fetch_and_add64(FAU_ERRORS, 0))
            printf("\nTEST FAILED\n");
        else
            printf("\nTEST PASSED\n");
    }

    return 0;
}

