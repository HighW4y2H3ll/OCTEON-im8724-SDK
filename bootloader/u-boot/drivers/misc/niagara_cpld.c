/*
 * (C) Copyright 2015 Cavium, Inc. <support@cavium.com>
 *
 * (C) Copyright 2015 Interface Masters
 * Marcel Birthelmer <marcelb@interfacemasters.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <niagara.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-bootmem.h>

static uint64_t cpld_base = 0;

void cpld_init(uint64_t base)
{
	debug("cpld_base: [%016llx]\n", base);
	cpld_base = base;
}

uint8_t cpld_rd(uint8_t offset)
{
	uint8_t val;

	CVMX_SYNC;
	val = cvmx_read64_uint8(cpld_base + offset);
	debug("[%016llx] == %02x\n", cpld_base + offset, val);

	return val;
}

void cpld_wr(uint8_t offset, uint8_t val)
{
	debug("[%016llx] <= %02x\n", cpld_base + offset, val);
	cvmx_write64_uint8(cpld_base + offset, val);
	CVMX_SYNC;
}

#ifdef CONFIG_SYS_NIAGARA_REMOTE

uint8_t cpld_rd_remote(uint8_t target, uint8_t offset)
{
	uint8_t val;
	cpld_wr(CPLD_ADDR_BA_CTL, 0x80 | (target & 0x1f) );
	cpld_wr(CPLD_ADDR_SPI_CFG, 0x80);
	cpld_wr(CPLD_ADDR_SPI_ADDR_0, 0x80 | offset & 0x7f);
	cpld_wr(CPLD_ADDR_SPI_CMD, CPLD_SPI_CMD_OP_R8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST);

	while(cpld_rd(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST);

	val = cpld_rd(CPLD_ADDR_SPI_DATA_0);


	return val;
}

void cpld_wr_remote(uint8_t target, uint8_t offset, uint8_t val)
{
	cpld_wr(CPLD_ADDR_BA_CTL, 0x80 | (target & 0x1f) );
	cpld_wr(CPLD_ADDR_SPI_CFG, 0x80);
	cpld_wr(CPLD_ADDR_SPI_ADDR_0, offset & 0x7f);
	cpld_wr(CPLD_ADDR_SPI_DATA_0, val);
	cpld_wr(CPLD_ADDR_SPI_CMD, CPLD_SPI_CMD_OP_W8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST);

	while(cpld_rd(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST);
}

#endif

int do_cpld_rd(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint16_t addr, val;
	int len = 1, i;

	if ((argc != 3) && (argc != 2)) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtol(argv[1], NULL, 16);
	if(argc == 3) len = simple_strtol(argv[2], NULL, 16);

	for (i = 0; i < len; i++, addr++) {
#ifdef CONFIG_SYS_NIAGARA_REMOTE
		if(addr & 0x8000)
			val = cpld_rd_remote( (addr >> 8) & 0x1f, addr & 0xff);
		else
#endif
			val = cpld_rd(addr);
		printf("0x%x: 0x%x\n", addr, val);
	}

	return (0);
}

int do_cpld_wr(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint16_t addr, val;

	if (argc != 3) {
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtol(argv[1], NULL, 16);
	val = simple_strtol(argv[2], NULL, 16);
#ifdef CONFIG_SYS_NIAGARA_REMOTE
	if(addr & 0x8000)
		cpld_wr_remote( (addr >> 8) & 0x1f, addr & 0xff, val);
	else
#endif
		cpld_wr(addr, val);

	return (0);
}

U_BOOT_CMD(cpld_rd, 3, 1, do_cpld_rd,
	   "cpld_rd 8 bit word from cpld",
	   "\n    - read 8 bit from cpld\n"
	   "cpld_rd  addr [len] \n    - read 8 bit  from cpld\n");

U_BOOT_CMD(cpld_wr, 3, 1, do_cpld_wr,
	   "cpld_wr 8 bit word from cpld",
	   "\n    - write  8 bit from cpld\n"
	   "cpld_wr  addr data \n    - write 8 bit  from cpld\n");
