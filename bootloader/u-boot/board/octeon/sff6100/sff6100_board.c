/*
 * (C) Copyright 2012 Cavium, Inc. <support@cavium.com>
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
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
	return 0;
}

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	cvmx_mio_qlmx_cfg_t mio_qlmx;
	const char *fdt_key;

	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(0));
	debug("%s: Fixing up QLM 0 to %s mode\n", __func__,
	      mio_qlmx.s.qlm_cfg == 2 ? "SGMII" : "PCI");
	if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "1,sgmii";
	} else {
		fdt_key = "1,none";	/* Disabled */
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	return 0;
}

int checkboard(void)
{
	if (gpio_get_value(12))
		printf("SW1-6 off: QLM 0 configured for SGMII\n");
	else
		printf("SW1-6  on: QLM 0 configured for PCIe\n");

	if (gpio_get_value(14)) {
		printf("SW1-7 off: QLM 1 configured for SATA\n");
	} else {
		if (gpio_get_value(12) == 0)
			puts("SW1-7  on: ERROR: QLM 1 cannot be configured for 2x1 PCIe mode\n"
			     "if QLM 0 is also configured for PCIe.\n");
		else
			puts("SW1-7  on: QLM 1 configured for 2x1 PCIe\n");
	}

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	gpio_direction_input(1);
	gpio_direction_input(3);
	gpio_direction_input(7);
	gpio_direction_output(10, 1);
	gpio_direction_input(12);
	gpio_direction_input(13);
	gpio_direction_input(14);

	/* Configure the QLMs */
#ifdef CONFIG_OCTEON_QLM
	octeon_configure_qlm(2, 1250, 2, 0, 0, 0, 0);

	if (gpio_get_value(12))
		octeon_configure_qlm(0, 1250, 2, 0, 0, 0, 0);
	else
		octeon_configure_qlm(0, 5000, 0, 1, 3, 0, 0);

	if (gpio_get_value(14))
		octeon_configure_qlm(1, 5000, 0, 1, 0, 0, 0);
	else
		octeon_configure_qlm(1, 5000, 1, 1, 3, 0, 0);
#endif

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(SFF6100_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_SFF6100, 2, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	return 0;
}
