/*
 * (C) Copyright 2004-2011
 * Cavium Inc.
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
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-ixf18201.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	if (octeon_show_info())
		show_pal_info();
	else
		octeon_led_str_write("Boot    ");

	/* Initialize the Cortina IXF18201 SPI ->XAUI MAX */
	cvmx_ixf18201_init();

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBT5810, 1, 0);
	/* Populate global data from eeprom */
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.clock_desc), ee_buf,
		       sizeof(octeon_eeprom_clock_desc_t));
	}

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards
	 */
	cpu_ref = octeon_mcu_read_cpu_ref();
	gd->mem_clk = octeon_mcu_read_ddr_clock();

	/* Some sanity checks */
	if (cpu_ref <= 0) {
		/* Default if cpu reference clock reading fails. */
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
	}
	if (gd->mem_clk <= 0) {
		gd->mem_clk = EBT5800_REV1_DEF_DRAM_FREQ;
	}

	/*
	 * For the cn58xx the DDR reference clock frequency is used to
	 * configure the appropriate internal DDR_CK/DDR2_REF_CLK ratio in
	 * order to generate the ddr clock frequency specified by
	 * ddr_clock_mhz.  For cn38xx this setting should match the state
	 * of the DDR2 output clock divide by 2 pin DDR2_PLL_DIV2.
	 *
	 * For DDR@_PLL_DIV2 = 0 the DDR_CK/DDR2_REF_CLK ratio is 4
	 * For DDR@_PLL_DIV2 = 1 the DDR_CK/DDR2_REF_CLK ratio is 2
	 */
#define FIXED_DDR_REF_CLOCK_RATIO	4

	/*
	 * More details about DDR clock configuration used for LMC
	 * configuration for the CN58XX.  Not used for CN38XX.  Since the
	 * reference clock frequency is not known it is inferred from the
	 * specified DCLK frequency divided by the DDR_CK/DDR2_REF_CLK
	 * ratio.
	 */
	gd->arch.ddr_ref_hertz =
	    gd->mem_clk * 1000000 / FIXED_DDR_REF_CLOCK_RATIO;

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_DBG_DATA);
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
