/*
 * (C) Copyright 2004-2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
//#define DEBUG
#include <common.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

int early_board_init(void)
{
	debug("%s: Initializing descriptor\n", __func__);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_ROUTER7000_REF,
				    CONFIG_OCTEON_ROUTER7000_REF_MAJOR,
				    CONFIG_OCTEON_ROUTER7000_REF_MINOR);

	debug("%s: Setting clock\n", __func__);
	octeon_board_get_clock_info(OCTEON_ROUTER7000_REF_DEF_DRAM_FREQ);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	debug("%s: Done\n", __func__);
	return 0;
}

void late_board_init(void)
{
	char buf[256];
	char *board_name;
	board_name = getenv("boardname");
	if (board_name) {
		sprintf(buf, "u-boot-octeon_%s.bin", board_name);
		setenv("octeon_stage3_bootloader", buf);
	}
}
