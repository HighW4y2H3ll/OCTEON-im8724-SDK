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
#include <asm/arch/cvmx-pcie.h>

extern void print_isp_volt(const char **labels, uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts(uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts_avg(int loops, uint16_t isp_dev_addr, uint8_t adc_chan);

DECLARE_GLOBAL_DATA_PTR;

void board_net_preinit(void)
{
	mdelay(120);
}

int board_early_init_f(void)
{
	return 0;
}

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key = "0,qsgmii";
	enum cvmx_qlm_mode mode;

	mode = cvmx_qlm_get_mode(0);
	switch (mode) {
	case CVMX_QLM_MODE_SGMII_SGMII:
		debug("DLM 0 mode: sgmii-sgmii\n");
		fdt_key = "0,sgmii-sgmii";
		break;
	case CVMX_QLM_MODE_SGMII_QSGMII:
		debug("DLM 0 mode: sgmii-qsgmii\n");
		fdt_key = "0,sgmii-qsgmii";
		break;
	case CVMX_QLM_MODE_SGMII_DISABLED:
		debug("DLM 0 mode: sgmii-none\n");
		fdt_key = "0,sgmii-none";
		break;
	case CVMX_QLM_MODE_QSGMII_SGMII:
		debug("DLM 0 mode: qsgmii-sgmii\n");
		fdt_key = "0,qsgmii-sgmii";
		break;
	case CVMX_QLM_MODE_QSGMII_QSGMII:
		debug("DLM 0 mode: qsgmii-qsgmii\n");
		fdt_key = "0,qsgmii-qsgmii";
		break;
	case CVMX_QLM_MODE_RXAUI_1X2:
		debug("DLM 0 mode: rxaui\n");
		fdt_key = "0,rxaui";
		break;
	case CVMX_QLM_MODE_DISABLED:
		debug("DLM 0 mode: disabled-disabled (none)\n");
		fdt_key = "0,none";
		break;
	default:
		fdt_key = "0,none";
		break;
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	mode = cvmx_qlm_get_dlm_mode(2, 0);
	switch (mode) {
	case CVMX_QLM_MODE_SATA_2X1:
		debug("dlm 2 mode: sata 2x1\n");
		fdt_key = "2,sata";
		break;
	default:
		debug("dlm 2 mode: disabled (none)\n");
		fdt_key = "2,none";
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	return 0;
}

/* Raise an integer to a power */
static uint64_t ipow(uint64_t base, uint64_t exp)
{
	uint64_t result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}
	return result;
}

static int checkboardinfo(void)
{
        int core_mVolts, dram_mVolts;
	char mcu_ip_msg[64] = { 0 };
	char tmp[10];
	int characters, idx = 0, value = 0;

	debug("In %s\n", __func__);
	if (octeon_show_info()) {

		int mcu_rev_maj = 0;
		int mcu_rev_min = 0;

		if (twsii_mcu_read(0x00) == 0xa5
		    && twsii_mcu_read(0x01) == 0x5a) {
			gd->arch.mcu_rev_maj = mcu_rev_maj = twsii_mcu_read(2);
			gd->arch.mcu_rev_min = mcu_rev_min = twsii_mcu_read(3);
		}

		core_mVolts = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 8);
		dram_mVolts = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 7);

		if (twsii_mcu_read(0x14) == 1)
			sprintf(mcu_ip_msg, "MCU IPaddr: %d.%d.%d.%d, ",
				twsii_mcu_read(0x10),
				twsii_mcu_read(0x11),
				twsii_mcu_read(0x12), twsii_mcu_read(0x13));
		printf("MCU rev: %d.%02d, %sCPU voltage: %d.%03d DDR voltage: %d.%03d\n",
		       gd->arch.mcu_rev_maj, gd->arch.mcu_rev_min, mcu_ip_msg,
		       core_mVolts / 1000, core_mVolts % 1000,
		       dram_mVolts / 1000, dram_mVolts % 1000);

#define LED_CHARACTERS 8
		value = core_mVolts;
		idx = sprintf(tmp, "%lu ", gd->cpu_clk/(1000*1000));
		characters = LED_CHARACTERS - idx;

		if (value / 1000) {
			idx += sprintf(tmp + idx, "%d", value / 1000);
			characters = LED_CHARACTERS - idx;
		}

		characters -= 1;	/* Account for decimal point */

		value %= 1000;
		value = DIV_ROUND_UP(value, ipow(10, max(3 - characters, 0)));

		idx += sprintf(tmp + idx, ".%0*d", min(3, characters), value);

		/* Display CPU speed and voltage on display */
		octeon_led_str_write(tmp);
	} else {
		octeon_led_str_write("Boot    ");
	}

	return 0;
}

void board_configure_qlms(void)
{
	int clksel_to_use = 0;  /* Reference-clock selection to use */
	int iclk_to_use = 1;    /* Reference-clock input to use */
	int host_mode;
	const char *dlm_mode;

	/* Take PHYs out of reset */
	gpio_direction_output(4, 1);

	/* For PCIe target mode we need to use clock 1 for DLM0
	 * since in this case clock 0 is coming from the PCIe
	 * host.  Also, there's no need to configure the DLM
	 * if we're in PCIe target (endpoint) mode.
	 */
	host_mode = cvmx_pcie_is_host_mode(0);

	if (!host_mode)
		clksel_to_use = 1;

	debug("%s: PCIE %s mode, clock select: %d\n",
	      __func__, host_mode ? "host" : "target", clksel_to_use);

	dlm_mode = getenv("dlm0_mode");
	if (!dlm_mode)
		dlm_mode = getenv("qlm0_mode");
	debug("%s: DLM0 mode: %s\n", dlm_mode ? dlm_mode : "(not set)");
	if (!dlm_mode || !strcmp(dlm_mode, "disabled_disabled") ||
	    !strcmp(dlm_mode, "disabled")) {
		puts("DLM 0 is disabled\n");
		octeon_configure_qlm(0, 0, CVMX_QLM_MODE_DISABLED,
				     0, 0, clksel_to_use, 0);
	} else if (!strcmp(dlm_mode, "sgmii_disabled")) {
		puts("Configuring DLM0 for SGMII/DISABLED\n");
		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_SGMII_DISABLED, 0, 0,
				     clksel_to_use, iclk_to_use);
	} else if (!strcmp(dlm_mode, "sgmii_sgmii") ||
		   !strcmp(dlm_mode, "sgmii")) {
		/* Enable the code based on MCU configuration */

		/* DLM0: SGMII/SGMII */
		puts("Configuring DLM0 for SGMII/SGMII\n");
		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_SGMII_SGMII, 0, 0,
				     clksel_to_use, iclk_to_use);
	} else if (!strcmp(dlm_mode, "qsgmii_qsgmii") ||
		   !strcmp(dlm_mode, "qsgmii")) {
		/* DLM0: QSGMII/QSGMII */
		puts("Configuring DLM0 for QSGMII/QSGMII\n");
		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_QSGMII_QSGMII, 0, 0,
				     clksel_to_use, iclk_to_use);
	} else if (!strcmp(dlm_mode, "rxaui") || !strcmp(dlm_mode, "xaui")) {
		puts("Configuring DLM0 for RXAUI\n");
		/* DLM0: RXAUI/DISABLED */
		/* Use external reference clock 156 MHz for RXAUI interface */
		octeon_configure_qlm(0, 3125, CVMX_QLM_MODE_RXAUI, 0, 0,
				     2, 2);
	} else {
		printf("Unknown DLM 0 configuration \"%s\"\n", dlm_mode);
		octeon_configure_qlm(0, 0, CVMX_QLM_MODE_DISABLED,
				     0, 0, clksel_to_use, 0);
	}

	dlm_mode = getenv("dlm1_mode");
	if (!dlm_mode)
		dlm_mode = getenv("qlm1_mode");
	debug("%s: DLM 1/2 mode: %s\n",
	      __func__, dlm_mode ? dlm_mode : "(not set)");
	if (!dlm_mode || !strcmp(dlm_mode, "disabled")) {
		puts("Configuring DLM1 as disabled\n");
		octeon_configure_qlm(1, 0, CVMX_QLM_MODE_DISABLED,
				     0, 0, 0, 0);
	} else if (!strcmp(dlm_mode, "pcie1x4") && host_mode) {
		/* Enable the following configuration for PCIe */
		puts("Configuring DLM1 for PCIe 1x4 host mode\n");

		/* PCIe0 1x4 */
		octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE, 1, 1, 0, 0);
	} else if (!strcmp(dlm_mode, "pcie2x2")) {
		puts("Configuring DLM1/2 to PCIe 2x2\n");
		if (host_mode)
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X2,
					     1, 1, 0, 0);
		octeon_configure_qlm(2, 5000, CVMX_QLM_MODE_PCIE_1X2, 1, 1, 0, 0);
	} else if (!strcmp(dlm_mode, "pcie3x1")) {
		/* PCIe0/PCIe1/PCIe2 3x1 */
		puts("Configuring DLM1/2 to PCIe 3x1\n");
		if (host_mode)
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X1,
					     1, 1, 0, 0);
		octeon_configure_qlm(2, 5000, CVMX_QLM_MODE_PCIE_2X1,
				     1, 1, 0, 0);
	} else if (!strcmp(dlm_mode, "pcie1x2_sata")) {
		puts("Configuring DLM1/2 to PCIe 1x2 + SATA\n");
		/* PCIe0/PCIe1 1x2 (L2) and SATA (L4/L5) */
		if (host_mode)
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X2,
					     1, 1, 0, 0);
		/* SATA0/SATA1 3.0 */
		octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1, 1, 0, 0, 0);
	} else if (!strcmp(dlm_mode, "pcie2x1_sata")) {
		puts("Configuring DLM1/2 to PCIe 2x1 + SATA\n");
		/* PCIe0/PCIe1 2x1 (L2/L3) and SATA (L4/L5) */
		if (host_mode)
			octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1,
					     1, 1, 0, 0);
		/* SATA0/SATA1 3.0 */
		octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1, 1, 0, 0, 0);
	}
}

int checkboard(void)
{
        checkboardinfo();

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

	/* Turn off power to USB ports */
	gpio_direction_output(2, 0);
	gpio_direction_output(3, 0);

	/* Populate global data from eeprom */

#ifdef CONFIG_OCTEON_TB7000
	octeon_board_get_clock_info(TB7000_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_TB7000, 1, 0);
#else
	octeon_board_get_clock_info(EVB7000_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EVB7000, 1, 0);
#endif

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull);

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}

void board_net_postinit(void)
{
	/* Try to initialize networking so Vitesse PHYs work for user
	 * programs.
	 */
	eth_init(gd->bd);
}
