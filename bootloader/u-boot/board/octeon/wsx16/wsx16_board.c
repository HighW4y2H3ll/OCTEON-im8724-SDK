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
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <pci.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/cvmx-spi.h>
#include <asm/arch/octeon_fdt.h>

DECLARE_GLOBAL_DATA_PTR;

int octeon_ebt3000_get_board_major_rev(void)
{
    return(gd->arch.board_desc.rev_major);
}
int octeon_ebt3000_get_board_minor_rev(void)
{
    return(gd->arch.board_desc.rev_minor);
}

/**
 * Callback to add board-specific boot bus devices in the device tree
 */
int octeon_add_user_boot_bus_fdt_devices(void)
{
	int rc;
	static const char *nand_compat[] = { "nand", NULL };
	static const char *unknown_compat[] = { "unknown", NULL };

	debug("In %s\n", __func__);
	rc = octeon_boot_bus_add_fdt_handler("nand",
					     (void *)nand_compat,
					     &octeon_boot_bus_generic_init);
	if (rc)
		printf("Error adding nand FDT handler\n");

	rc = octeon_boot_bus_add_fdt_handler("nand",
					     (void *)unknown_compat,
					     &octeon_boot_bus_generic_init);
	if (rc)
		printf("Error adding unknown FDT handler\n");

	return rc;
}

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
	const int cpu_ref = 33;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

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

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CUST_WSX16, 1, 0);

	if (OCTEON_IS_MODEL(OCTEON_CN58XX))
		gd->mem_clk = 333;
	else
		gd->mem_clk = 266;

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
	if (OCTEON_IS_MODEL(OCTEON_CN38XX))
		gd->arch.ddr_ref_hertz =
			gd->mem_clk * 1000000 / FIXED_DDR_REF_CLOCK_RATIO;
	else
		gd->arch.ddr_ref_hertz = gd->mem_clk * 1000000 / 2;

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_DBG_DATA);
	data = data >> 18;
	data &= 0x1f;

	gd->cpu_clk = data * cpu_ref;

	/* adjust for 33.33 Mhz clock */
	if (cpu_ref == 33)
		gd->cpu_clk += data / 4 + data / 8;

	if (gd->cpu_clk < 100 || gd->cpu_clk > 600)
		gd->cpu_clk = DEFAULT_ECLK_FREQ_MHZ;
	gd->cpu_clk *= 1000000;

	return 0;
}
