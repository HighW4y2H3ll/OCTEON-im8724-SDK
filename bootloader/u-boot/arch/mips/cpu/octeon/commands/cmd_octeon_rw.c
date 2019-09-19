/***********************license start************************************
 * Copyright (c) 2011  Cavium Inc. (support@cavium.com).
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

#include <common.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/octeon_boot.h>

extern uint64_t base_address64;

int do_read64(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr, val;

	if ((argc != 2)) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;

	printf("attempting to read from addr: 0x%llx\n", addr);
	val = cvmx_read_csr(addr);
	printf("0x%llx: 0x%llx\n", addr, val);

	return (0);
}

int do_read64b(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint8_t val;

	if ((argc != 2)) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;

	printf("attempting to read from addr: 0x%llx\n", addr);
	val = octeon_read64_byte(addr);
	printf("0x%llx: 0x%x\n", addr, val);

	return (0);
}

int do_read64l(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint32_t val;

	if ((argc != 2)) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;

	printf("attempting to read from addr: 0x%llx\n", addr);
	val = cvmx_read64_uint32(addr);
	printf("0x%llx: 0x%x\n", addr, val);

	return (0);
}

int do_read64s(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint32_t val;

	if ((argc != 2))
		return CMD_RET_USAGE;

	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;

	printf("attempting to read from addr: 0x%llx\n", addr);
	val = cvmx_read64_uint16(addr);
	printf("0x%llx: 0x%x\n", addr, val);

	return (0);
}

int do_write64(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr, val;

	if (argc != 3)
		return CMD_RET_USAGE;

	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;
	val = simple_strtoull(argv[2], NULL, 16);

	printf("writing 0x%llx to addr: 0x%llx\n", val, addr);

	cvmx_write_csr(addr, val);
	return (0);
}

int do_write64b(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint8_t val;

	if (argc != 3)
		return CMD_RET_USAGE;

	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;
	val = simple_strtoull(argv[2], NULL, 16);

	printf("writing 0x%x to addr: 0x%llx\n", val, addr);

	octeon_write64_byte(addr, val);
	return (0);
}

int do_write64s(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint32_t val;

	if (argc != 3)
		return CMD_RET_USAGE;
	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;
	val = simple_strtoull(argv[2], NULL, 16);

	printf("writing 0x%x to addr: 0x%llx\n", val, addr);

	cvmx_write64_uint16(addr, val);
	return (0);
}

int do_write64l(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	uint64_t addr;
	uint32_t val;

	if (argc != 3)
		return CMD_RET_USAGE;

	addr = simple_strtoull(argv[1], NULL, 16);
	addr += base_address64;
	val = simple_strtoull(argv[2], NULL, 16);

	printf("writing 0x%x to addr: 0x%llx\n", val, addr);

	cvmx_write64_uint32(addr, val);
	return (0);
}

U_BOOT_CMD(read64, 2, 1, do_read64,
	   "read 64 bit word from 64 bit address (deprecated)",
	   "read64 addr\n    - read 64 bit word from 64 bit address\n");
U_BOOT_CMD(write64, 4, 1, do_write64,
	   "write 64 bit word to 64 bit address (deprecated)",
	   "write64 addr val\n    - write 64 bit word to 64 bit address\n");
U_BOOT_CMD(read64b, 2, 1, do_read64b,
	   "read 8 bit word from 64 bit address (deprecated)",
	   "read64b addr\n    - read 8 bit word from 64 bit address\n");
U_BOOT_CMD(read64s, 2, 1, do_read64s,
	   "read 16 bit word from 64 bit address (deprecated)",
	   "read64s addr\n    - read 16 bit word from 64 bit address\n");
U_BOOT_CMD(read64l, 2, 1, do_read64l,
	   "read 32 bit word from 64 bit address (deprecated)",
	   "read64l addr\n    - read 32 bit word from 64 bit address\n");
U_BOOT_CMD(write64b, 4, 1, do_write64b,
	   "write 8 bit word to 64 bit address (deprecated)",
	   "write64b addr val\n    - write 8 bit word to 64 bit address\n");
U_BOOT_CMD(write64s, 4, 1, do_write64s,
	   "write 16 bit word to 64 bit address (deprecated)",
	   "write64s addr val\n    - write 16 bit word to 64 bit address\n");
U_BOOT_CMD(write64l, 4, 1, do_write64l,
	   "write 32 bit word to 64 bit address (deprecated)",
	   "write64l addr val\n    - write 32 bit word to 64 bit address\n");
