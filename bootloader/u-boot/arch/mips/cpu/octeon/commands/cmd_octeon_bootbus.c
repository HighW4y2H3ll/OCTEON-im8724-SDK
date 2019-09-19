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

/**
 * This file provides the octbootbus command which allows one to change
 * the timing parameters for boot bus devices.
 */

#include <common.h>
#include <command.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/octeon_hal.h>
#include <asm/arch/cvmx-mio-defs.h>


static const u8 timing_mult_table[4] = { 4, 1, 2, 8 };
static const u8 page_size_table[4] = { 8, 2, 4, 8 };

static inline unsigned decode_time(u32 value, u32 mult, u32 period)
{
	if (!value)
		return 0;
	return value * mult * period;
}

static void print_boot_bus_config(int bus)
{
	cvmx_mio_boot_reg_cfgx_t reg_cfg;
	cvmx_mio_boot_reg_timx_t reg_tim;
	u32 address;
	u32 size;
	u32 tim_mult;

	reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(bus));
	reg_tim.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_TIMX(bus));
	address = reg_cfg.s.base << 16;
	size = (reg_cfg.s.size + 1) << 16;
	if (!reg_cfg.s.en) {
		printf("bus: %d, disabled\n", bus);
		return;
	}
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		tim_mult = timing_mult_table[reg_cfg.s.tim_mult];
	else
		tim_mult = 4;

	printf("  enabled: %s, address: 0x%08x, size: 0x%x\n",
	       reg_cfg.s.en ? "true" : "false", address, size);
	printf("  width: %d, %smultiplexed", reg_cfg.s.width ? 16 : 8,
		reg_cfg.s.ale ? "" : "non-");
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		printf("  timing multiplier: %d, ", tim_mult);
	printf("read sample delay: %d cycles%s\n",
	       reg_cfg.s.rd_dly,
	       reg_cfg.s.sam ? ", oe and we combined" : "");
	if (reg_cfg.s.we_ext)
		printf("  write enable count extension: %d\n",
		       reg_cfg.s.we_ext);
	if (reg_cfg.s.oe_ext)
		printf("  output enable count extension: %d\n",
		       reg_cfg.s.oe_ext);
	if (reg_cfg.s.orbit)
		printf("  CE enabled when previous region matches.\n");
	if (reg_cfg.s.dmack)
		printf("  DMA acknowlege %d\n", reg_cfg.s.dmack - 1);
	if (reg_tim.s.pagem)
		printf("  Page mode enabled, page size: %u\n",
		       page_size_table[reg_tim.s.pages]);
	if (reg_tim.s.waitm)
		printf("  Wait mode enabled\n");
}

static void print_boot_bus_timing(int bus)
{
	cvmx_mio_boot_reg_timx_t reg_tim;
	cvmx_mio_boot_reg_cfgx_t reg_cfg;
	uint64_t clock_mhz;
	uint64_t clock_period;
	u32 tim_mult;
	u32 val;

	clock_mhz = cvmx_clock_get_rate(CVMX_CLOCK_SCLK) / 1000000;
	clock_period = 1000000 / clock_mhz;	/* clk period (psecs) */

	reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(bus));
	reg_tim.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_TIMX(bus));

	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		tim_mult = timing_mult_table[reg_cfg.s.tim_mult];
	else
		tim_mult = 4;

	printf("  ale: %u ps\n", decode_time(reg_tim.s.ale,
					     tim_mult, clock_period));
	printf("  page: %u ps\n", decode_time(reg_tim.s.page,
					      tim_mult, clock_period));
	printf("  wait: %u ps\n", decode_time(reg_tim.s.wait,
					      1, clock_period));
	printf("  pause: %u ps\n", decode_time(reg_tim.s.pause,
					       tim_mult, clock_period));
	printf("  write hold: %u ps\n", decode_time(reg_tim.s.wr_hld,
						    tim_mult, clock_period));
	printf("  read hold: %u ps\n", decode_time(reg_tim.s.rd_hld,
						    tim_mult, clock_period));
	val = reg_tim.s.we;
	val |= reg_cfg.s.we_ext << 6;
	printf("  write enable: %u ps\n", decode_time(val,
						      tim_mult, clock_period));
	val = reg_tim.s.oe;
	val |= reg_cfg.s.oe_ext << 6;
	printf("  output enable: %u ps\n", decode_time(val,
						       tim_mult, clock_period));
	printf("  chip enable: %u ps\n", decode_time(reg_tim.s.ce,
						     tim_mult, clock_period));
	printf("  address: %u ps\n", decode_time(reg_tim.s.adr,
						 tim_mult, clock_period));
	if (OCTEON_IS_OCTEON2() || OCTEON_IS_OCTEON3() ||
	    OCTEON_IS_MODEL(OCTEON_CN56XX) || OCTEON_IS_MODEL(OCTEON_CN52XX) ||
	    OCTEON_IS_MODEL(OCTEON_CN50XX))
		printf("  read delay: %u ps\n", decode_time(reg_cfg.s.rd_dly, 1,
							    clock_period));
	printf("  wait mode: %s\n", reg_tim.s.waitm ? "enabled" : "disabled");
	printf("  page mode: %s\n", reg_tim.s.pagem ? "enabled" : "disabled");
	printf("  ale mode:  %s\n", reg_cfg.s.ale ? "enabled" : "disabled");
}

static void print_boot_bus_timing_parameters(void)
{
	puts("Boot bus timing parameters:\n"
	     "\tpagemode - page mode - 1 to enable, 0 to disable\n"
	     "\twaitmode - wait mode - 1 to enable, 0 to disable\n"
	     "\tpagesize - page size [2, 4 or 8]\n"
	     "\tale - address latch enable timing\n"
	     "\tpage - page timing\n"
	     "\twait - wait timing\n"
	     "\tpause - pause timing\n"
	     "\twr_hld - write-hold timing\n"
	     "\trd_hld - read-hold timing\n"
	     "\twe - write enable timing\n"
	     "\toe - output enable timing\n"
	     "\tce - chip enable timing\n"
	     "\tadr - address timing\n"
	);
}

static u32 encode_time(u32 value, u32 period, u32 mult)
{
	if (!value)
		return value;
	return ((((value + period) / period) - 1) + mult) / mult;
}

static int set_boot_bus_timing(unsigned bus, const char *param, int value)
{
	cvmx_mio_boot_reg_timx_t reg_tim;
	cvmx_mio_boot_reg_cfgx_t reg_cfg;
	uint64_t clock_mhz;
	uint64_t clock_period;
	u32 tim_mult;

	clock_mhz = cvmx_clock_get_rate(CVMX_CLOCK_SCLK) / 1000000;
	clock_period = 1000000 / clock_mhz;	/* clk period (psecs) */
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		tim_mult = timing_mult_table[reg_cfg.s.tim_mult];
	else
		tim_mult = 4;

	if (bus > 7) {
		printf("bus %d out of range.  Bus must be between 0 and 7.\n",
		       bus);
		return -1;
	}

	debug("Setting bus: %u, parameter: %s to %u\n", bus, param, value);
	reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(bus));
	reg_tim.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_TIMX(bus));

	if (!reg_cfg.s.en) {
		printf("boot bus %u is not enabled\n", bus);
		return -1;
	}

	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		tim_mult = timing_mult_table[reg_cfg.s.tim_mult];
	else
		tim_mult = 4;

	debug("Read 0x%llx from mio_boot_reg_tim%u\n", reg_tim.u64, bus);

	if (!strcmp(param, "pagemode"))
		reg_tim.s.pagem = value;
	else if (!strcmp(param, "waitmode"))
		reg_tim.s.waitm = value;
	else if (!strcmp(param, "pagesize")) {
		switch (value) {
		case 2:	reg_tim.s.pages = 1;	break;
		case 4:	reg_tim.s.pages = 2;	break;
		case 8:	reg_tim.s.pages = 3;	break;
		default:
			printf("Invalid page size %d\n", value);
			return -1;
		}
	} else if (!strcmp(param, "ale"))
		reg_tim.s.ale = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "page"))
		reg_tim.s.page = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "wait"))
		reg_tim.s.wait = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "pause"))
		reg_tim.s.pause = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "wr_hld"))
		reg_tim.s.wr_hld = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "rd_hld"))
		reg_tim.s.rd_hld = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "we"))
		reg_tim.s.we = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "oe"))
		reg_tim.s.oe = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "ce"))
		reg_tim.s.ce = encode_time(value, clock_period, tim_mult);
	else if (!strcmp(param, "adr"))
		reg_tim.s.adr = encode_time(value, clock_period, tim_mult);
	else {
		print_boot_bus_timing_parameters();
		return -1;
	}
	debug("Wrote 0x%llx to mio_boot_reg_tim%u\n", reg_tim.u64, bus);
	cvmx_write_csr(CVMX_MIO_BOOT_REG_TIMX(bus), reg_tim.u64);
	return 0;
}
int do_boot_bus_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 bus;
	const char *param;
	u32 value;

	if (argc >= 4) {
		bus = simple_strtoul(argv[1], NULL, 0);
		if (bus > 7) {
			printf("bus must be between 0 and 7.\n");
			return -1;
		}
		param = argv[2];
		value = simple_strtoul(argv[3], NULL, 0);
		return set_boot_bus_timing(bus, param, value);
	} else {
		printf("Usage: set [param] [value]\n");
		print_boot_bus_timing_parameters();
		return -1;
	}
}

int do_boot_bus_print(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 bus;
	if (argc >= 2) {
		bus = simple_strtoul(argv[1], NULL, 0);
		if (bus > 7) {
			printf("bus must be between 0 and 7.\n");
			return -1;
		}
		print_boot_bus_config(bus);
		print_boot_bus_timing(bus);
		return 0;
	} else {
		printf("Too few arguments.\n");
		cmd_usage(cmdtp);
		return -1;
	}
}

int do_boot_bus(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2) {
		printf("Too few arguments\n");
		cmd_usage(cmdtp);
		return -1;
	}
	if (argv[1][0] == 'p')
		return do_boot_bus_print(cmdtp, flag, argc - 1, &argv[1]);
	if (argv[1][0] == 's')
		return do_boot_bus_set(cmdtp, flag, argc - 1, &argv[1]);

	printf("Unknown argument \"%s\"\n", argv[1]);
	return -1;
}

U_BOOT_CMD(
	octbootbus,    5,    0,     do_boot_bus,
	"print boot bus parameters",
	"\n    print [bus] - print boot bus settings for the specified bus"
	"\n    set [bus] [param] [value] - change a boot bus timing parameter"
	"\n    set - displays all of the available parameters"
	"\n"
);

