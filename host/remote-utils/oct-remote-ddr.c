/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
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
 * Configure & test the DDR on an Octeon over a remote interface
 *
 * $Id: oct-remote-ddr.c 89134 2013-10-03 21:39:13Z cchavva $
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CVMX_ADD_IO_SEG(a) (a)  /* There aren't any Mips IO segments when using PCI */
#include "cvmx.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"

#define ULL unsigned long long

uint64_t octeon_read64(uint64_t csr_addr);
void octeon_write64(uint64_t csr_addr, uint64_t val);
void octeon_delay_cycles(uint64_t cycles);

long test_octeon_memory(uint32_t size_mb);

int main(int argc, const char *argv[])
{
    long mbytes_to_test = 8;

    if (argc<2)
        goto usage;

    if (argc>=2)
        mbytes_to_test = atoi(argv[1]);

    if (mbytes_to_test <= 0 ||  mbytes_to_test > 256)
        goto usage;

    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_remote_mem_access_ok())
    {
        octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    printf("Testing %ld Mbytes\n", mbytes_to_test);

    /* Explicitly lock access to the bus, otherwise this is done for every
    ** read/write, which makes this test uselessly slow.
    */
    octeon_remote_lock();
    long failures = test_octeon_memory(mbytes_to_test);
    octeon_remote_unlock();

    octeon_remote_close();
    return -failures;

usage:
    printf("\n"
           "Octeon DDR2 memory test over a remote interface\n"
           "Note: Requires use of oct-*-boot to configure memory controller\n"
           "Usage:\n"
           "    %s  <Mbytes to test (between 1 and 256)>\n"
           "\n", argv[0]);
    return -1;
}


uint64_t octeon_read64(uint64_t csr_addr) {
   return octeon_remote_read_csr(csr_addr);
}

void octeon_write64(uint64_t csr_addr, uint64_t val) {
   return octeon_remote_write_csr(csr_addr, val);
}

void octeon_delay_cycles(uint64_t cycles) {
   usleep((cycles + 499) / 500);
}

int  test_data_bus(uint64_t addr, uint64_t len /* unused */);
int  test_addr_bus(uint64_t addr, uint64_t len);
int  incr_pattern_test(uint64_t addr, uint64_t len);
int  pattern_test(uint64_t addr, uint64_t len);

long test_octeon_memory(uint32_t size_mb) {
    uint64_t len;
    int loops;
    long failures = 0;

    len = size_mb * 1024 * 1024;

    if (octeon_remote_mem_access_ok())
    {
        if (OCTEON_IS_MODEL(OCTEON_CN6XXX) || OCTEON_IS_MODEL(OCTEON_CN70XX))
        {
            uint64_t lmc = octeon_remote_read_csr(CVMX_LMCX_CONFIG(0));
            if (lmc & 0x2) {
                printf("ECC was on. Turning ECC off.\n");
                octeon_remote_write_csr(CVMX_LMCX_CONFIG(0), lmc ^ 2);
            }
            else {
                printf("ECC is off.\n");
            }
        }
        else
        {
            uint64_t lmc = octeon_remote_read_csr(CVMX_LMC_MEM_CFG0);
            if (lmc & 0x2) {
                printf("ECC was on. Turning ECC off.\n");
                octeon_remote_write_csr(CVMX_LMC_MEM_CFG0, lmc ^ 2);
            }
            else {
                printf("ECC is off.\n");
            }
        }
    }
    else
        printf("Skipping ECC check as LMC is disabled.\n");

    printf("OCTEON Remote Memory Tester\n");
    printf("Copyright (C) 2006 Cavium Inc..\n");

    failures += test_data_bus(1 * 1024 * 1024, 0);
    if (failures)
	printf(" --> test data bus %lu\n", failures);

    failures += test_addr_bus(0, len /*4ULL * 1024 * 1024 * 1024*/);
    if (failures)
	printf(" --> test addr bus %lu\n", failures);

#if 1
    if (failures) {
	printf("Basic memory tests found %lu failures, skipping stress testing.\n", failures);
	return failures;
    }
#endif
#if 1
    printf("Basic memory test complete, starting stress test.\n");

    loops = 1;
    failures = 0;
    while (loops--) {	/* Or some other completion condition. */

	failures += incr_pattern_test(0, len);

	if (failures)
	    printf("Number of failures: %lu - TEST FAILED\n", failures);

	printf("Done.\n");
    }
#endif
    return failures;
}

static __inline__ uint64_t phys2map(uint64_t addr) {
    uint32_t selc = (addr & 0x1fffff0000000ULL) >> 28;
    return (selc != 1) ? addr : (0x400000000ULL | addr);
}

#define mem_read(dst, adr)  octeon_remote_read_mem( dst, phys2map(adr), sizeof(*dst))
#define mem_write(adr, src) octeon_remote_write_mem(phys2map(adr), src, sizeof(*src))

int test_data_bus(uint64_t addr, uint64_t len /* unused */) {
    int bit;
    int failures = 0;
    uint64_t w1, w2;
    uint64_t latch1, latch2;

    printf("Data bus test\n");

    for (bit = 0; bit < 128; bit++) {
	w1 = (bit < 64) ? 1ULL << bit : 0;
	w2 = (bit < 64) ?           0 : 1ULL << (bit - 64);

	mem_write(addr  , &w1);
	mem_write(addr+8, &w2);

	mem_read(&latch1, addr  );
	mem_read(&latch2, addr+8);

	if (latch1 != w1) {
	    printf("High-order (bits 64-127) data bus bit test failed, expected 0x%llx, got 0x%llx\n", (ULL)w1, (ULL)latch1);
	    ++failures;
	}
	if (latch2 != w2) {
	    printf("Low-order (bits 0-63) data bus bit test failed, expected 0x%llx, got 0x%llx\n", (ULL)w2, (ULL)latch2);
	    ++failures;
	}
    }

    return failures;
}

int  test_addr_bus(uint64_t addr, uint64_t len) {
    register int failures = 0;
    register uint64_t addr_mask = (len / sizeof(uint64_t)) - 1;
    register uint64_t offset;
    register uint64_t test_offset;
    const uint64_t addr_pattern = 0x5555555555555555ULL;
    const uint64_t c_addr_pattern = ~addr_pattern;
    uint64_t latch;

    printf("Address bus test\n");

    /* Write the pattern at each of the power-of-two offsets. */
    for (offset = 1; (offset & addr_mask) != 0; offset <<= 1) {
	mem_write(addr + (offset << 3), &addr_pattern);
    }

    /* Check for address bits stuck high */
    test_offset = 0;
    mem_write(addr, &c_addr_pattern);

    for (offset = 1; (offset & addr_mask) != 0; offset <<= 1) {
	mem_read(&latch, addr + (offset << 3));

	if (latch != addr_pattern) {
	    printf("Address bit stuck high, offset %llx vs 0, read 0x%llx expected 0x%llx\n", (ULL)offset << 3, (ULL)latch, (ULL)addr_pattern);
	    ++failures;
	}
    }

    mem_write(addr, &addr_pattern);

    /* Check for address bits stuck low / shorted */
    for (test_offset = 1; (test_offset & addr_mask) != 0; test_offset <<= 1) {
	mem_write(addr + (test_offset << 3), &c_addr_pattern);

	mem_read(&latch, addr);

	if (latch != addr_pattern) {
	    printf("Address bit stuck low/shorted, offset %llx vs 0, read 0x%llx expected 0x%llx\n", (ULL)test_offset << 3, (ULL)latch, (ULL)addr_pattern);
	    ++failures;
	}

	for (offset = 1; (offset & addr_mask) != 0; offset <<= 1) {
	    mem_read(&latch, addr + (offset << 3));
	    if ((latch != addr_pattern) && (offset != test_offset)) {
		printf("Address bit stuck low/shorted offset %llx vs %llx read 0x%llx expected 0x%llx\n", (ULL)offset << 3, (ULL)test_offset << 3, (ULL)latch, (ULL)addr_pattern);
		++failures;
	    }
	}

	mem_write(addr + (test_offset << 3), &addr_pattern);
    }

    return failures;
}

int  incr_pattern_test(uint64_t addr, uint64_t len) {
    int failures = 0;
    uint64_t latch;
    uint64_t offset;
    uint64_t temp;

    printf("Increment pattern test addr 0x%llx len 0x%llx\n", (ULL)addr, (ULL)len);

    for (offset = 0; offset < len; offset += 8) {
	if (!(offset & 0xffff)) {
	    printf("\rIncrement pattern fill 0x%08llx ", (ULL)offset);
	    fflush(stdout);
	}
	temp = offset >> 3;
	mem_write(addr + offset, &temp);
    }
    printf("\b.\n");

    for (offset = 0; offset < len; offset += 8) {
	if (!(offset & 0xffff)) {
	    printf("\rIncrement pattern testing 0x%08llx ", (ULL)offset);
	    fflush(stdout);
	}
	mem_read(&latch, addr + offset);
	if (latch != (offset >> 3)) {
	    ++failures;
	    if (failures < 50) {
		printf("Pattern test failure, address 0x%08llx read 0x%08llx expected 0x%08llx xor %08llx\n",
		    (ULL)(addr + offset), (ULL)latch, (ULL)(offset >> 3), (ULL)(latch ^ (offset >> 3)));
	    }
	    else if (failures == 50) {
		printf("... more failures, suppressing further output.\n");
	    }
	}
	temp = ~(offset >> 3);
	mem_write(addr + offset, &temp);
    }
    printf("\b.\n");

    for (offset = 0; offset < len; offset += 8) {
	if (!(offset & 0xffff)) {
	    printf("\rIncrement ~pattern testing 0x%08llx ", (ULL)offset);
	    fflush(stdout);
	}
	mem_read(&latch, addr + offset);
	if (latch != ~(offset >> 3)) {
	    ++failures;
	    if (failures < 50) {
		printf("CPattern test failure, address 0x%08llx read 0x%08llx expected 0x%08llx xor %08llx\n",
		    (ULL)(addr + offset), (ULL)latch, (ULL)~(offset >> 3), (ULL)(latch ^ ~(offset >> 3)));
	    }
	    else if (failures == 50) {
		printf("... more failures, suppressing further output.\n");
	    }
	}
    }
    printf("\b.\n");

    return failures;
}

