/*
 * (C) Copyright 2013 Cavium, Inc. <support@cavium.com>
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
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

void board_net_preinit(void)
{
	mdelay(120);
}
/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;

	if (gpio_get_value(3))
		fdt_key = "2,sata";
	else
		fdt_key = "2,pcie";
	octeon_fdt_patch(working_fdt, fdt_key, NULL);


	if (gpio_get_value(1) == 0)
		fdt_key = "0,qsgmii";
	else
		fdt_key = "0,xaui";

	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	if (gpio_get_value(4) == 1) {
		puts("PCM mode detected, disabling MMC and USB power control\n");
		octeon_fdt_patch(working_fdt, "0,pcm", "cavium,pcm-trim");

	}
	return 0;
}

int checkboard(void)
{
	int clk_to_use = 0;	/* Clock used for DLM0 */

	if (gpio_get_value(2) == 0) {	/* DLM1_SEL */
		puts("SW4-2 on: mini-PCIe slots selected\n");
		octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1, 1, 1, 0, 0);
	} else {
		if (gpio_get_value(3) == 1) {
			puts("SW4-2 off, SW4-3 off: PCIe 1x2 mode selected\n");
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X2,
					     1, 1, 0, 0);
		} else {
			int host_mode = cvmx_pcie_is_host_mode(0);
			printf("SW4-2 off, SW4-3 on: PCIe 1x4 %s mode selected\n",
			       host_mode ? "host" : "target");
			/* For PCIe target mode we need to use clock 1 for DLM0
			 * since in this case clock 0 is coming from the PCIe
			 * host.  Also, there's no need to configure the DLM
			 * if we're in PCIe target (endpoint) mode.
			 */
			if (host_mode)
				octeon_configure_qlm(1, 5000,
						     CVMX_QLM_MODE_PCIE,
						     1, 1, 0, 0);
			else
				clk_to_use = 1;

		}
	}

	if (gpio_get_value(1) == 0) {	/* DLM0_SEL */
		puts("SW4-1 on: QSGMII ports selected\n");
		debug("Configuring DLM0 for QSGMII/QSGMII\n");
		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_QSGMII_QSGMII,
				     0, 0, clk_to_use, 0);
		debug("Done.\n");
	} else {
		puts("SW4-1 off: RXAUI (10G) port selected\n");
		octeon_configure_qlm(0, 3125, CVMX_QLM_MODE_RXAUI,
				     0, 0, clk_to_use, 1);
	}


	if (gpio_get_value(3) != 0) {	/* DLM2_SEL */
		puts("SW4-3 off: SATA ports selected\n");
		octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1,
				     0, 0, 0, 0);
	}

	debug("Powering up Vitesse PHYs\n");
	gpio_direction_output(9, 0);	/* COMA mode */
	debug("Powering up USB ports\n");

	return 0;
}

int early_board_init(void)
{
	uint8_t val;

	/* Disable USB power */
	gpio_direction_output(18, 1);
	gpio_direction_output(19, 1);
	/* Enable COMA */
	gpio_direction_output(9, 1);
	gpio_direction_input(1);	/* DLM1_SEL */
	gpio_direction_input(2);	/* DLM2A_SEL */
	gpio_direction_input(3);	/* DLM2B_SEL */
	gpio_direction_input(4);	/* PCM select */

	gpio_direction_input(14);	/* SD/MMC Write Protect */
	gpio_direction_input(15);	/* SD/MMC Card Detect */
	mdelay(10);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EVB7000_SFF_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EVB7000_SFF, 1, 0);

	/* Enable temperature and fan monitoring */
	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);
	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 0);
	/* Invert output, turn on tach, 2-wire analog and enable monitoring */
	val |= 0xd;
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0, val);

	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 1);
	/* Turn off tach 2 and remote 2 temperature sensor */
	val &= ~0x48;
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 1, val);
	/* Enable fan filtering for fan 1 */
	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 0x23);
	val |= 1;
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x23, val);

	/* 800ms spin-up, 93.5KHz PWM, lowest speed 2647RPM */
	val = 0x3b;
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x20, val);

	val = 0x63;	/* CPU fan enable temp 48C, range starts at 40C */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x25, val);

	val = 0x52;
	/* PWM for fan 1 starts at 14%, 33% for fan 2 */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x25, val);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	return 0;
}

void board_net_postinit(void)
{
	/* Try to initialize networking so Vitesse PHYs work for user
	 * programs.
	 */
	eth_init(gd->bd);
}
