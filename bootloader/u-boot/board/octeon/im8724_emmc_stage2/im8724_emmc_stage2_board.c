/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
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

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
        int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

	puts("IM8724 eMMC Stage\n");
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CUST_IM8724,
			CONFIG_OCTEON_IM8724_MAJOR,
			CONFIG_OCTEON_IM8724_MINOR);

	octeon_board_get_clock_info(IM8724_DEF_DRAM_FREQ);

        /* Set for CN78XX alternative 100 MHz reference clock,
         * instead of 50 Hz: (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) */
        gd->arch.ddr_ref_hertz = 100000000ull;

	octeon_board_get_mac_addr();

        /* Read CPU clock multiplier */
        gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}

void late_board_init(void)
{
	setenv("octeon_stage3_bootloader", "u-boot-octeon_im8724.bin");
	setenv("octeon_stage3_failsafe_bootloader", "u-boot-octeon_im8724.bak");
}
