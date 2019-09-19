/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
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
#include <pca953x.h>

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
	int val;
	int rc;

	val = pca953x_get_val(0, 0x20);
	if (val & 4)
		fdt_key = "2,sata";
	else
		fdt_key = "2,pcie";
	octeon_fdt_patch(working_fdt, fdt_key, NULL);


	if (val & 1)
		fdt_key = "0,xaui";
	else
		fdt_key = "0,qsgmii";

	debug("%s: Patching DLM 0 for %s\n", __func__, fdt_key);
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	if (val & 8) {
		debug("PCM mode detected, disabling SPI NOR\n");
		octeon_fdt_patch(working_fdt, "0,pcm", "cavium,pcm-trim");
	} else {
		debug("SPI NOR mode selected\n");
		octeon_fdt_patch(working_fdt, "0,not-pcm", "cavium,pcm-trim");
	}

	/* Check if we need to swap the MMC slots or not. */
	if (val & 0x10) {
		int s0_offset, s1_offset, offset;

		debug("%s: Swapping mmc slots 0 and 1\n", __func__);
		/* Swap slot 0 and slot 1 in device tree */
		offset = fdt_path_offset(gd->fdt_blob, "/soc/mmc");
		if (offset < 0) {
			puts("Error accessing /soc/mmc in device tree\n");
			return -1;
		}
		s0_offset = fdt_subnode_offset(gd->fdt_blob, offset,
					       "mmc-slot@0");
		s1_offset = fdt_subnode_offset(gd->fdt_blob, offset,
					       "mmc-slot@1");
		debug("  slot 0 offset: %d, slot 1 offset: %d\n",
		      s0_offset, s1_offset);
		if (s0_offset < 0 || s1_offset < 0) {
			puts("Error accessing MMC in device tree\n");
			return -1;
		}
		rc = fdt_setprop_inplace_u32(gd->fdt_blob, s0_offset, "reg", 1);
		rc |= fdt_setprop_inplace_u32(gd->fdt_blob, s1_offset, "reg", 0);
		if (rc) {
			puts("Error changing reg property in mmc slot\n");
			return -1;
		}

		rc = fdt_set_name(gd->fdt_blob, s0_offset, "mmc-slot@1");
		rc |= fdt_set_name(gd->fdt_blob, s1_offset, "mmc-slot@0");
		if (rc) {
			puts("Error renaming MMC slot names\n");
			return -1;
		}
	}


	return 0;
}

void board_configure_qlms(void)
{
	int clk_to_use = 0;	/* Clock used for DLM0 */
	int val;

	val = pca953x_get_val(0, 0x20);

	/* Print it early so switches are in order */
	if (val & 1) {	/* DLM0_SEL */
		puts("SW4-1 on: RXAUI (10G) port selected\n");
		octeon_configure_qlm(0, 3125, CVMX_QLM_MODE_RXAUI,
				     0, 0, 2, 2);
	} else {
		puts("SW4-1 off: QSGMII ports selected\n");
		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_QSGMII_QSGMII,
				     0, 0, 0, 1);
	}

	if (val & 2) {	/* DLM1_SEL */
		if (val & 4) {
			puts("SW4-2 on, SW4-3 off: PCIe 1x2 mode selected\n");
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X2,
					     1, 1, 0, 0);
		} else {
			int host_mode = cvmx_pcie_is_host_mode(0);
			printf("SW4-2 on, SW4-3 on: PCIe 1x4 %s mode selected\n",
			       host_mode ? "host" : "target");
			/* For PCIe target mode we need to use clock 1 for DLM0
			 * since in this case clock 0 is coming from the PCIe
			 * host.  Also, there's no need to configure the DLM
			 * if we're in PCIe target (endpoint) mode.
			 */
			if (host_mode)
				octeon_configure_qlm(1, 5000,
						     CVMX_QLM_MODE_PCIE,
			 1, 1, clk_to_use, 0);
			else
				clk_to_use = 1;

		}
	} else {
		puts("SW4-2 off: mini-PCIe slots selected\n");
		octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1, 1, 1, 0, 0);
	}

	if (val & 4) {	/* DLM2_SEL */
		puts("SW4-3 on: SATA ports selected\n");
		octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1,
				     0, 0, 0, 0);
	}
	if (val & 8)
		puts("SW4-4 on: PCM mode selected, SPI NOR disabled\n");
	else
		puts("SW4-4 off: SPI NOR enabled\n");

	printf("SW 1-8 is %s, %s selected as slot 1.\n",
	       val & 0x10 ? "on" : "off",
	val & 0x10 ? "internal eMMC flash" : "external SD/MMC slot");

	debug("qlm 0 reference clock: %llu\n", cvmx_qlm_measure_clock(0));
	debug("qlm 1 reference clock: %llu\n", cvmx_qlm_measure_clock(1));
	debug("qlm 2 reference clock: %llu\n", cvmx_qlm_measure_clock(2));

}

int early_board_init(void)
{
	uint8_t val;

	/* Disable USB power */
	gpio_direction_output(1, 1);
	gpio_direction_output(2, 1);

	gpio_direction_input(4);	/* SD/MMC Write Protect */
	gpio_direction_input(3);	/* SD/MMC Card Detect */
	mdelay(10);

	if (PCA953X_DIR_IN != 0)
		val = 0xff;
	else
		val = 0;

	/* Make all pins inputs */
	pca953x_set_dir(0, 0x20, 0xff, val);

	/* Set output GPIOs 0-1, 4-7 */
	val = (val & ~(1 << 0)) | PCA953X_DIR_OUT << 0;
	val = (val & ~(1 << 1)) | PCA953X_DIR_OUT << 1;
	val = (val & ~(1 << 2)) | PCA953X_DIR_IN << 2;
	val = (val & ~(1 << 3)) | PCA953X_DIR_IN << 3;
	val = (val & ~(1 << 4)) | PCA953X_DIR_OUT << 4;
	val = (val & ~(1 << 5)) | PCA953X_DIR_OUT << 5;
	val = (val & ~(1 << 6)) | PCA953X_DIR_OUT << 6;
	val = (val & ~(1 << 7)) | PCA953X_DIR_OUT << 7;
	pca953x_set_dir(0, 0x21, 0xff, val);

	/* Put PHYs in reset */
	pca953x_set_val(0, 0x21, 1 << 4, 0x00);
	mdelay(10);
	/* Take PHY out of reset */
	pca953x_set_val(0, 0x21, 1 << 4, 1 << 4);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(SFF7000_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_SFF7000, 1, 0);

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
