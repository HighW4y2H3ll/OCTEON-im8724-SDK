/*
 * (C) Copyright 2004-2012 Cavium, Inc.  <support@cavium.com>
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
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_bcsr.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/cvmx-mio-defs.h>

DECLARE_GLOBAL_DATA_PTR;

uint64_t bcsr_addr = 0;

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	union cvmx_gmxx_inf_mode gmx_mode;

	gmx_mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));
	if (gmx_mode.cn63xx.speed == 0xf) {
		/* Remove all PHYs */
		fdt_key = "0,none";	/* Disabled */
	} else if (gmx_mode.cn63xx.mode) {
		/* Remove all non-XAUI PHYs */
		fdt_key = "0,xaui";
	} else {
		/* Remove all non-SGMII PHYs */
		fdt_key = "0,sgmii";
	}
	/* Fixup the device tree */
	return octeon_fdt_patch(working_fdt, fdt_key, NULL);
}

/* The BCSR are the Board Control and Status Registers defined for the
 * FPGA in the Embedded Planet EP6300C board.  They are used to control
 * LEDs, reset the CF card (and read if the card is inserted or write protected),
 * detect XAUI or SGMII mode, write protect the NAND, NOR and EEPROM, turn
 * off the board and enable or disable Ethernet.
 */

/**
 * Callback to add board-specific boot bus devices in the device tree
 */
int octeon_add_user_boot_bus_fdt_devices(void)
{
	int rc;
	static const char *bcsr_compat[] = { "embedded-planet,bcsr-ep6300c",
					     NULL };

	debug("In %s\n", __func__);
	rc = octeon_boot_bus_add_fdt_handler("bcsr-fpga",
					     (void *)bcsr_compat,
					     &octeon_boot_bus_generic_init);
	return rc;
}

/**
 * Board-specific function which is called after the boot bus has finished
 * initializing all devices in the device tree.
 */
int octeon_boot_bus_board_post_init(void *fdt_addr)
{
	int rc;
	uint8_t val;

	debug("In %s\n", __func__);

	rc = octeon_boot_bus_get_dev_info("/soc/bootbus/bcsr-fpga",
					  "embedded-planet,bcsr-ep6300c",
					  &bcsr_addr, NULL);
	if (rc || (bcsr_addr == 0)) {
		printf("%s: Error getting address of BCSR FPGA!\n", __func__);
		return rc;
	}

	debug("BCSR address: 0x%llx\n", bcsr_addr);

	/* Reset the PHYs */
	val = cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_ENET_CTRL_OFFSET);
	val &= ~(OCTEON_BCSR_ENET_PHY1_RESET | OCTEON_BCSR_ENET_PHY2_RESET |
		 OCTEON_BCSR_ENET_PHY3_RESET);
	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_ENET_CTRL_OFFSET, val);

	return 0;
}

extern int __octeon_cf_present(unsigned long base_addr);
/**
 * Reads the BCSR to determine if the CF card is present or not.
 */
int octeon_cf_present(unsigned long base_addr)
{
	if (!bcsr_addr)
		return __octeon_cf_present(base_addr);

	return (!(octeon_bcsr_get_cf_status() &
		 (OCTEON_BCSR_CF_CARD0_NOT_INSERTED |
		  OCTEON_BCSR_CF_CARD1_NOT_INSERTED)));
}

/**
 * Sets the CF reset line using the BCSR.
 */
void ide_set_reset(int reset)
{
	int val;

	debug("In %s\n", __func__);
	if (bcsr_addr == 0) {
		printf("%s: Error: unknown BCSR address\n", __func__);
		return;
	}

	val = cvmx_read64_uint8(bcsr_addr + OCTEON_BCSR_CF_CTRL_STATUS_OFFSET);
	if (reset)
		val |= OCTEON_BCSR_CF_RESET;
	else
		val &= ~OCTEON_BCSR_CF_RESET;

	cvmx_write64_uint8(bcsr_addr + OCTEON_BCSR_CF_CTRL_STATUS_OFFSET, val);
	CVMX_SYNC;
}

#if defined(CONFIG_IDE_LED)
/* We use one of the IDE LEDs to indicate CF accesses */
void ide_led(uchar led, uchar status)
{
	debug("In %s\n", __func__);
	octeon_bcsr_set_led(OCTEON_BCSR_LED_USER1, status);
}
#endif

/* Check if the board is OK and do any late initialization. */
int checkboard(void)
{
	return 0;
}

/* Early board initialization */
int early_board_init(void)
{
	/* configure clk_out pin */
	cvmx_mio_fus_pll_t fus_pll;

	fus_pll.u64 = cvmx_read_csr(CVMX_MIO_FUS_PLL);
	fus_pll.cn63xx.c_cout_rst = 1;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* Sel::  0:rclk, 1:pllout 2:psout 3:gnd */
	fus_pll.cn63xx.c_cout_sel = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);
	fus_pll.cn63xx.c_cout_rst = 0;
	cvmx_write_csr(CVMX_MIO_FUS_PLL, fus_pll.u64);

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EP6300C_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EP6300C, 1, 0);

	octeon_board_get_mac_addr();

	return 0;
}

/**
 * Dynamically adjust the board name, used for prompt generation
 * @param name - name string to be adjusted
 * @param max_len - maximum length of name
 *
 * This function can overwrite the name of a board, for example based on
 * the processor installed.
 */
void octeon_adjust_board_name(char *name, size_t max_len)
{
	if (!strcasecmp(name, "ep6300c") && OCTEON_IS_MODEL(OCTEON_CN66XX))
		name[3] = '6';
}
