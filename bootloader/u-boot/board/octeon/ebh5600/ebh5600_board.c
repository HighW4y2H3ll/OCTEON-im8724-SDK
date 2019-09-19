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
#include <config.h>
#include <common.h>
#include <command.h>
#include <mtd/cfi_flash.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <pci.h>
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	if (octeon_show_info())
		show_pal_info();
	else
		octeon_led_str_write("Boot    ");

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
	enum cvmx_board_types_enum board_type;

	/* NOTE: this is early in the init process, so the serial port is not yet configured */

#ifdef CONFIG_OCTEON_EBH5600
	board_type = CVMX_BOARD_TYPE_EBH5600;
#elif defined(CONFIG_OCTEON_EBH5601)
	board_type = CVMX_BOARD_TYPE_EBH5601;
#else
	board_type = CVMX_BOARD_TYPE_EBT5600;
#endif
	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EBH5600_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(board_type, 1, 0);

	/* CN56XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards
	 */
	cpu_ref = octeon_mcu_read_cpu_ref();

	/* Some sanity checks */
	if (cpu_ref <= 0)
		/* Default if cpu reference clock reading fails. */
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	octeon_led_str_write("Booting.");
	return 0;

}
