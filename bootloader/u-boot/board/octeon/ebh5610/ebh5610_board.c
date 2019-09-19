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
	uint64_t data = cvmx_read_csr(CVMX_PEXP_NPEI_DBG_DATA);

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EBH5610_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBH5610, 1, 0);

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
	if (cpu_ref <= 0) {
		/* Default if cpu reference clock reading fails. */
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
	}

	/* Get and populate the board MAC address from the TLV EEPROM */
	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	data = data >> 18;
	data &= 0x1f;


	gd->cpu_clk = data * cpu_ref;

	/* adjust for 33.33 Mhz clock */
	if (cpu_ref == 33)
		gd->cpu_clk += (data) / 4 + data / 8;

	if (gd->cpu_clk < 100 || gd->cpu_clk > 1100)
		gd->cpu_clk = DEFAULT_ECLK_FREQ_MHZ;

	gd->cpu_clk *= 1000000;

	octeon_led_str_write("Booting.");
	return 0;

}

void octeon_led_str_write(const char *str)
{
	octeon_led_str_write_std(str);
}
