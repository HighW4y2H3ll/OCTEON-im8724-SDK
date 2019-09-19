/***********************license start************************************
 * Copyright (c) 2010 Cavium Inc. (support@cavium.com).
 * All rights reserved.
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
 ***********************license end**************************************/

#include <config.h>
#include <command.h>
#include <common.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-swap.h>
#include <asm/arch/cvmx-l2c.h>

/* This file contains support for OCTEON specific memory commands for handling
 * named blocks and cache support.
 */

int do_bootmem_named_alloc(cmd_tbl_t * cmdtp, int flag,
			   int argc, char * const argv[])
{
	char *name;
	int64_t addr = 0;
	uint64_t max_addr = (1ull << 32) - 1;
	uint64_t size;
	char buf[20];

	setenv("named_block_addr", NULL);
	setenv("named_block_size", NULL);

	if (argc < 3) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	name = argv[1];
	size = simple_strtoull(argv[2], NULL, 16);
	if (size & 0x7f) {
		size = (size + 0x7f) & ~0x7f;
		printf("Rounding size up to 0x%llx bytes\n", size);
	}
	if (argc == 4) {
		addr = simple_strtoull(argv[3], NULL, 16);
		max_addr = addr + size;
	}

	debug("Name: %s, size: 0x%llx, addr: 0x%llx\n", name, size, addr);

	/* Align to a cache line if the size is bigger than a cache line */
	addr = cvmx_bootmem_phy_named_block_alloc(size, addr, max_addr,
						  size >= 128 ? 128 : 0,
						  name, 0);
	if (addr < 0) {
		printf("Named allocation failed!\n");
		return 1;
	}
	printf("Allocated 0x%llx bytes at address: 0x%llx, name: %s\n", size,
	       addr, name);

	sprintf(buf, "0x%llx", addr);
	setenv("named_block_addr", buf);
	sprintf(buf, "0x%llx", size);
	setenv("named_block_size", buf);

#ifdef DEBUG
	debug("\n\n===========================================\n");
	debug("Dump of named blocks:\n");
	cvmx_bootmem_phy_named_block_print();
	cvmx_bootmem_phy_list_print();
#endif

	return 0;
}

int do_bootmem_named_free(cmd_tbl_t * cmdtp, int flag,
			  int argc, char * const argv[])
{
	if (argc < 2) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	if (!cvmx_bootmem_phy_named_block_free(argv[1], 0)) {
		printf("Error freeing block: %s\n", argv[1]);
		return 1;
	}
	return 0;
}

int do_bootmem_named_print(cmd_tbl_t * cmdtp, int flag,
			   int argc, char * const argv[])
{
	cvmx_bootmem_phy_named_block_print();
	return 0;
}

int do_bootmem_free_print(cmd_tbl_t * cmdtp, int flag,
			  int argc, char * const argv[])
{
	cvmx_bootmem_phy_list_print();
	return 0;
}

int do_flush_l2c(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	cvmx_l2c_flush();
	return 0;
}

int do_icache_invalidate(cmd_tbl_t *cmdtp, int flag,
			 int argc, char * const argv[])
{
	CVMX_SYNC;
	CVMX_ICACHE_INVALIDATE;
	return 0;
}

int do_dcache_flush(cmd_tbl_t *cmdtp, int flag,
		    int argc, char * const argv[])
{
	CVMX_SYNC;
	CVMX_DCACHE_INVALIDATE;
	return 0;
}

U_BOOT_CMD(namedalloc, 4, 0, do_bootmem_named_alloc,
	   "Allocate a named bootmem block",
	   "\n    - Allocate a named bootmem block\n"
	   "namedalloc name size [address]\n"
	   "    - Allocate a named bootmem block with a given name and size at an\n"
	   "      optional fixed address.  Sets environment variables named_block_addr,\n"
	   "      named_block_size to address and size of block allocated.\n");

U_BOOT_CMD(namedfree, 2, 0, do_bootmem_named_free,
	   "Free a named bootmem block",
	   "name\n    - Free a named bootmem block\n");

U_BOOT_CMD(namedprint, 1, 0, do_bootmem_named_print,
	   "Print list of named bootmem blocks",
	   "\n    - Print list of named bootmem blocks\n");

U_BOOT_CMD(freeprint, 1, 0, do_bootmem_free_print,
	   "Print list of free bootmem blocks",
	   "\n    - Print list of free bootmem blocks\n");

U_BOOT_CMD(flush_l2c, 1, 1, do_flush_l2c,
	   "Flushes the L2 cache",
	   "Flushes the L2 cache");

U_BOOT_CMD(inv_icache, 1, 1, do_icache_invalidate,
	   "Invalidates the instruction cache",
	   "Invalidates the instruction cache");

U_BOOT_CMD(flush_dcache, 1, 1, do_dcache_flush,
	   "Flushes and invalidates the data cache",
	   "Flushes and invalidates the data cache");
