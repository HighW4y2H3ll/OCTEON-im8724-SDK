/*
 * (C) Copyright 2004-2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

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
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIAGARA83X,
				    CONFIG_OCTEON_NIAGARA83X_MAJOR,
				    CONFIG_OCTEON_NIAGARA83X_MINOR);

	debug("%s: Setting clock\n", __func__);
	octeon_board_get_clock_info(OCTEON_NIAGARA83X_DEF_DRAM_FREQ);

	octeon_board_get_mac_addr();

	debug("%s: Done\n", __func__);
	return 0;
}

void late_board_init(void)
{
	char buf[256];
	const char *board_name;
	board_name = getenv("boardname");
	if (board_name) {
		snprintf(buf, sizeof(buf), "u-boot-octeon_%s.bin", board_name);
		setenv("octeon_stage3_bootloader", buf);
	}
}
