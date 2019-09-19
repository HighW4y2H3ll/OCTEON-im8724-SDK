/*
 * (C) Copyright 2004-2014 Cavium Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/octeon-model.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * Perform early board initialization
 */
int early_board_init(void)
{
	const int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EAP7000_REF, 1, 0);

	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_clock_info(EAP7000_REF_DEF_DRAM_FREQ);

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}

/**
 * Perform initialization late in the init process
 */
void late_board_init(void)
{
	char buf[256];
	char *board_name;
	board_name = getenv("boardname");
	if (board_name) {
		sprintf(buf, "u-boot-octeon_%s.bin", board_name);
		setenv("octeon_stage3_bootloader", buf);
		debug("Detected board type %s\n", board_name);
		sprintf(buf, "%s (SPI stage 2)", board_name);
		setenv("prompt", buf);
	}
}
