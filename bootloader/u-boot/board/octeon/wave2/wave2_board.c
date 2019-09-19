/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
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
#include <phy.h>
#include "../common/phy/aquantia.h"
#include <asm/arch/cvmx-mdio.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-gmxx-defs.h>

DECLARE_GLOBAL_DATA_PTR;

enum qlm_mode {
	MODE_SGMII,
	MODE_2500BASEX,
	MODE_RXAUI
};

static enum qlm_mode qlm_mode = MODE_SGMII;	/** QLM 0 mode selected */
static enum qlm_mode mode_2500 = MODE_2500BASEX; /** Mode to use for 2.5G */

/**
 * Perform fixup of the device tree according to the interface type.
 */
int board_fixup_fdt(void)
{
	char fdt_key[16];
	enum cvmx_qlm_mode mode;

	mode = cvmx_qlm_get_mode(0);

	debug("%s: baud: %u\n", __func__, cvmx_qlm_get_gbaud_mhz(0));

	if (mode == CVMX_QLM_MODE_RXAUI_1X2)
		sprintf(fdt_key, "0,rxaui");
	else if (qlm_mode == MODE_SGMII)
		sprintf(fdt_key, "0,sgmii");
	else
		sprintf(fdt_key, "0,2500base-x");

	debug("Patching qlm 0 for %s\n", fdt_key);
	octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL, true,
					NULL, NULL);
	return 0;
}

/**
 * Return the speed of the network interface
 */
int aq_get_link_speed(bool verbose)
{
	ulong start;
	int con_state = -1;
	int val;
	bool link_up = false;

	start = get_timer(0);
	debug("%s: Waiting for link up...\n", __func__);
	do {
		val = cvmx_mdio_45_read(0, 1, 1, 0xe800);
		if (val < 0) {
			printf("%s: Cannot read Aquantia PHY\n", __func__);
			return -1;
		}
		link_up = !!(val & 1);
	} while (!link_up && get_timer(start) < 10000);

	if (link_up) {
		start = get_timer(0);
		/* If we're in autonegotiation or training retry until it
		 * finishes.
		 */
		do {
			val = cvmx_mdio_45_read(0, 1, 7, 0xc810);
			if (val < 0) {
				printf("%s: Cannot read Aquantia PHY\n",
				       __func__);
				return -1;
			}
			/* Get the connection state, poll for a while if the
			 * state is in autonegotiation or calibrating
			 */
			con_state = (val >> 9) & 0x1f;
			if (con_state != 2 && con_state != 3 && con_state != 0xa)
				break;
		} while (get_timer(start) < 2000);

		debug("%s: connection state: 0x%x\n", __func__, con_state);
		debug("%s: Autonegotiation standard status 1: 0x%x, link is %s\n",
		      __func__, val, link_up ? "up" : "down");
	} else {
		debug("%s: link is down, enabling SGMII mode\n",
		      __func__);
	}
	if (!link_up || con_state != 4) {
		if (verbose)
			printf("%s, assuming SGMII (1000Base-T)\n",
			       link_up ? "Ethernet connection state not connected"
				       : "No Ethernet link detected");
		return 1000;
	}

	val = cvmx_mdio_45_read(0, 1, 7, 0xc800);
	debug("%s: Autonegotiation Vendor Status 1: 0x%x\n", __func__, val);
	if (val < 0) {
		printf("%s: Cannot read Aquantia PHY\n", __func__);
		return 10000;
	}

	debug("%s: Connection rate: 0x%x\n", __func__, (val >> 1) & 7);
	switch ((val >> 1) & 0x7) {
	case 0:
		debug("%s: Detected 10Base-T, SGMII mode\n", __func__);
		if (verbose)
			puts("Detected 10Base-T connection.  This speed is not supported.  Defaulting to 100Base-TX/1000Base-T mode\n");
		return 10;
	case 1:
		debug("%s: Detected 100Base-TX, SGMII mode\n", __func__);
		if (verbose)
			puts("Detected 100Base-TX Ethernet connection\n");
		return 100;
	case 2:
		debug("%s: Detected 1000Base-T, SGMII mode\n", __func__);
		if (verbose)
			puts("Detected 1000Base-T Ethernet connection\n");
		return 1000;
	case 3:
		debug("%s: Detected 10GBase-T, RXAUI mode\n", __func__);
		if (verbose)
			puts("Detected 10GBase-T Ethernet connection\n");
		return 10000;
	case 4:
#if AQUANTIA_2500_MODE_RXAUI
		debug("%s: Detected 2.5G, RXAUI mode\n", __func__);
#else
		debug("%s: Detected 2.5G, 2500Base-X mode\n", __func__);
#endif
		if (verbose)
			puts("Detected 2.5G Ethernet connection\n");
		return 2500;
		break;
	case 5:
		if (verbose)
			puts("Detected 5G Ethernet connection\n");
		debug("%s: Detected 5G, RXAUI mode\n", __func__);
		return 5000;
		break;
	default:
		printf("Unknown connection rate, Autonegotiation Vendor Status 1: 0x%x\n",
		       val);
		return 1000;
	}
}

/**
 * Returns the mode used for 2.5Gbps
 *
 * @return	Mode used for 2.5G
 */
enum qlm_mode aq_get_2_5g_mode(void)
{
	uint16_t val;
	val = cvmx_mdio_45_read(0, 1, 4, 0xc441);
	debug("%s() returned 0x%x\n", __func__, val);
	/* Bit 13 = 1 for 2500BASE-X mode, 0 for 10G mode */
	if (val & (1 << 13))
		return MODE_2500BASEX;
	else
		/* Bits 14-15, 0 = XAUI, 1 = RXAUI, 2 = XFI, 3 = reserved */
		return MODE_RXAUI;
}

/**
 * Configure the QLMs on the board
 */
void board_configure_qlms(void)
{
	uint64_t sclock;
	union cvmx_smix_clk smix_clk;
	int speed;
	u16 val;
	ulong start;

	/* Init MDIO early */
	sclock = cvmx_clock_get_rate(CVMX_CLOCK_SCLK);
	smix_clk.u64 = cvmx_read_csr(CVMX_SMIX_CLK(0));
	smix_clk.s.phase = sclock / (2500000 * 2);
	cvmx_write_csr(CVMX_SMIX_CLK(0), smix_clk.u64);

	cvmx_write_csr(CVMX_SMIX_EN(0), 1);
	mdelay(10);

	mode_2500 = aq_get_2_5g_mode();
#ifdef DEBUG
	if (mode_2500 == MODE_2500BASEX)
		debug("%s: 2500BASE-X mode selected for 2.5Gbps\n", __func__);
	else
		debug("%s: RXAUI mode selected for 2.5Gbps\n", __func__);
#endif


	/* Enable all modes */
	/* Enable 10Gbps */
	val = cvmx_mdio_45_read(0, 1, 7, 0x20);
	val |= 1 << 12;
	cvmx_mdio_45_write(0, 1, 7, 0x20, val);
	/* Enable 2.5 and 5Gbps */
	val = cvmx_mdio_45_read(0, 1, 7, 0xc400);
	val |= (1 << 11) | (1 << 10);
	/* Enable 1Gbps */
	val |= (1 << 15);
	cvmx_mdio_45_write(0, 1, 7, 0xc400, val);
	/* Enable 100Mbps */
	val = cvmx_mdio_45_read(0, 1, 7, 0x10);
	val |= (0xc << 5);	/* Turn on 100Base-TX and 100Base-TX FD. */
	cvmx_mdio_45_write(0, 1, 7, 0x10, val);

	/* Restart autonegotiation */
	val = cvmx_mdio_45_read(0, 1, 7, 0);
	val |= (1 << 9);
	cvmx_mdio_45_write(0, 1, 7, 0, val);

	debug("%s: Waiting for autonegotiation to restart\n", __func__);
	start = get_timer(0);
	do {
		val = cvmx_mdio_45_read(0, 1, 0x1e, 0xc831);
	} while ((val & (1 << 15)) && get_timer(start) < 2000);

	if (val & (1 << 15)) {
		puts("Restarting Ethernet autonegotiation times out.  Assuming SGMII (1000Base-T)\n");
		speed = 1000;
	} else {
		speed = aq_get_link_speed(true);
	}

	if (speed < 2500)
		qlm_mode = MODE_SGMII;

	else if ((speed == 2500) && (mode_2500 == MODE_2500BASEX))
		qlm_mode = MODE_2500BASEX;
	else
		qlm_mode = MODE_RXAUI;

	if (qlm_mode == MODE_SGMII) {
		/* Put PHY SERDES into SGMII mode */
		debug("%s: Configuring PHY for SGMII mode\n", __func__);

		val = cvmx_mdio_45_read(0, 1, 7, 0x20);
		/* Disable 10Gbps */
		val &= ~(1 << 12);
		cvmx_mdio_45_write(0, 1, 7, 0x20, val);
		val = cvmx_mdio_45_read(0, 1, 7, 0xc400);
		/* Disable 2.5 and 5Gbps */
		val &= ~((1 << 11) | (1 << 10));
		/* Enable 1Gbps */
		val |= (1 << 15);
		cvmx_mdio_45_write(0, 1, 7, 0xc400, val);
		/* Enable 100Mbps */
		val = cvmx_mdio_45_read(0, 1, 7, 0x10);
		/* Turn on 100Base-TX and 100Base-TX FD. */
		val |= (0xc << 5);
		cvmx_mdio_45_write(0, 1, 7, 0x10, val);

		octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_DISABLED_SGMII,
				     0, 0, 1, 0);
	} else if (qlm_mode == MODE_2500BASEX) {
		debug("%s: Configuring PHY for 2500Base-X mode\n", __func__);
		val = cvmx_mdio_45_read(0, 1, 7, 0x20);
		/* Disable 10Gbps */
		val &= ~(1 << 12);
		cvmx_mdio_45_write(0, 1, 7, 0x20, val);

		val = cvmx_mdio_45_read(0, 1, 7, 0xc400);
		/* Enable 2.5 mode */
		val |= (1 << 10);
		/* Disable 5G and 1G modes */
		val &= ~((1 << 11) | (1 << 15));
		cvmx_mdio_45_write(0, 1, 7, 0xc400, val);
		/* Disable 100Mbps */
		val = cvmx_mdio_45_read(0, 1, 7, 0x10);
		/* Turn off 100Base-TX and 100Base-TX FD. */
		val &= ~(0xf << 5);
		cvmx_mdio_45_write(0, 1, 7, 0x10, val);

		octeon_configure_qlm(0, 3125, CVMX_QLM_MODE_DISABLED_SGMII,
				     0, 0, 1, 0);
	} else {
		/* Enable 10Gbps */
		val = cvmx_mdio_45_read(0, 1, 7, 0x20);
		val |= 1 << 12;
		cvmx_mdio_45_write(0, 1, 7, 0x20, val);
		val = cvmx_mdio_45_read(0, 1, 7, 0xc400);
		/* Enable 5Gbps */
		val |= (1 << 11);
		if (mode_2500 != MODE_2500BASEX)
			/* Enable 2.5Gbps */
			val |= (1 << 10);
		else
			/* Disable 2.5Gbps */
			val &= ~(1 << 10);

		/* Disable 1Gbps */
		val &= ~(1 << 15);
		cvmx_mdio_45_write(0, 1, 7, 0xc400, val);
		/* Disable 10 and 100Mbps */
		val = cvmx_mdio_45_read(0, 1, 7, 0x10);
		val &= ~(0xf << 5);
		cvmx_mdio_45_write(0, 1, 7, 0x10, val);

		debug("%s: Configuring PHY for RXAUI mode\n", __func__);
		octeon_configure_qlm(0, 3125, CVMX_QLM_MODE_RXAUI,
				     0, 0, 1, 0);
	}
	printf("Ethernet interface configured for %s mode.\n",
	       (qlm_mode == MODE_SGMII) ? "SGMII" :
	       (qlm_mode == MODE_RXAUI) ? "RXAUI" : "2500Base-X");
	debug("%s: Configuring DLMs 1 and 2 for PCIe\n", __func__);
	/* Configure QLMs */
	/* We'll configure QLM 0 later */
	/* qlm, speed, mode, rc, pcie_mode, ref_clk_sel, ref_clk_input */

	octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_1X2, 1, 1, 0, 1);
	octeon_configure_qlm(2, 5000, CVMX_QLM_MODE_PCIE_1X2, 1, 1, 0, 1);

	/* Restart autonegotiation */
	val = cvmx_mdio_45_read(0, 1, 7, 0);
	val |= (1 << 9);
	cvmx_mdio_45_write(0, 1, 7, 0, val);

	debug("%s: Waiting for autonegotiation to restart\n", __func__);
	start = get_timer(0);
	do {
		val = cvmx_mdio_45_read(0, 1, 0x1e, 0xc831);
	} while ((val & (1 << 15)) && get_timer(start) < 2000);

	if (val & (1 << 15))
		debug("%s: Restarting autonegotiation timed out\n", __func__);

#ifdef DEBUG
	debug("%s: DLM 0 rate: %d, DLM 1 rate: %d, DLM 2 rate: %d\n",
	      __func__, cvmx_qlm_measure_clock(0),
	      cvmx_qlm_measure_clock(1), cvmx_qlm_measure_clock(2));
	debug("%s: gbaud DLM 0: %d, DLM 1: %d, DLM 2: %d\n", __func__,
	      cvmx_qlm_get_gbaud_mhz(0), cvmx_qlm_get_gbaud_mhz(1),
	      cvmx_qlm_get_gbaud_mhz(2));
#endif
}

int octeon_eth_board_post_init(struct eth_device *dev)
{
	union cvmx_gmxx_rxx_frm_ctl frm_ctl;
	cvmx_helper_interface_mode_t if_mode;

	if_mode = cvmx_helper_interface_get_mode(0);
	if (if_mode == CVMX_HELPER_INTERFACE_MODE_RXAUI) {
		/* We need to turn on backpressure for 2.5 and 5g to work
		 * properly
		 */
		frm_ctl.u64 = cvmx_read_csr(CVMX_GMXX_RXX_FRM_CTL(0, 0));
		frm_ctl.s.ctl_bck = 1;
		cvmx_write_csr(CVMX_GMXX_RXX_FRM_CTL(0, 0), frm_ctl.u64);
	}

	return 0;
}
/**
 * Perform early board initialization
 */
int early_board_init(void)
{
	debug("%s: ENTER\n", __func__);
	/* Enable USB power */
	gpio_direction_output(1, 1);

	/* Put the PHYs in reset */
	gpio_direction_output(9, 0);

	/* Wait at least 10ms */
	mdelay(11);

	/* Enable PCIe wireless cards */
	gpio_direction_output(5, 1);
	gpio_direction_output(12, 1);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(WAVE2_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_WAVE2,
				    CONFIG_OCTEON_WAVE2_MAJOR,
				    CONFIG_OCTEON_WAVE2_MINOR);


	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	return 0;
}

int checkboard(void)
{
	debug("%s: Entry\n", __func__);
	/* Take PHYs out of reset */
	gpio_direction_output(9, 1);
	mdelay(10);

	return 0;
}
