/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/cvmx-mdio.h>
#include <i2c.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-cfg.h>
#include <asm/arch/cvmx-pcsx-defs.h>
#include <asm/arch/cvmx-gmxx-defs.h>
#include <asm/arch/cvmx-access-native.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/cvmx-uart.h>
#include <niagara.h>

DECLARE_GLOBAL_DATA_PTR;

#define USB2514B_BUS	0
#define USB2514B_DEV	0x2c

static const uint8_t usb2514b_init[][2] = {
	{0x06, 0x93},		/* CONFIG_BYTE_1 */
	{0x07, 0x30},		/* CONFIG_BYTE_2 */
	{0x08, 0x00},		/* CONFIG_BYTE_3 */
	{0x09, 0x00},		/* NON-REMOVABLE DEVICE */
	{0x0A, 0x00},		/* PORT DISABLE SP */
	{0x0B, 0x00},		/* PORT DISABLE BP */
	{0xFF, 0x01}		/* STATUS/COMMAND */

};

#define TUSB8041_BUS	0
#define TUSB8041_DEV	0x44

/* despite what the datasheet says, this is not an SMBus block write */
static const uint8_t tusb8041_init[][2] = {
	{0x06, 0x00},		/* Battery Charging Support */
	{0x07, 0x8F},		/* Device Removable Configuration */
	{0x08, 0x0F},		/* Port used configuration */
	{0xf8, 0x01},		/* Status/Command */
};

static int cpld_accessible = 0;

#define CPLD_ADDR_MISC_CSR		0x05
#define CPLD_MISC_CSR_PHY_COMA	0x10

#define CPLD_ADDR_RST			0x0C
#define CPLD_RST_MGMT_PHY		0x04

#define N834_CPLD_SIG			0xE1

/**
 * This is another weak function that is called before networking is initialized
 */
void board_net_preinit(void)
{
	uint8_t cpld_reset = cpld_rd(CPLD_ADDR_RST);
	uint16_t cpld_sig =
	    (cpld_rd(CPLD_ADDR_CPLD_SIG_H) << 8) |
	    cpld_rd(CPLD_ADDR_CPLD_SIG_L);

	/* Reset the PHY */
	if (cpld_sig == N834_CPLD_SIG) {
		gpio_direction_output(11, 0);
		mdelay(5);
		gpio_direction_output(11, 1);
	} else {
		cpld_wr(CPLD_ADDR_RST, cpld_reset | CPLD_RST_MGMT_PHY);
		mdelay(5);
		cpld_wr(CPLD_ADDR_RST, cpld_reset);
	}

	mdelay(120);
}

/**
 * Configure the QLMs on the board
 */
void board_configure_qlms(void)
{
	/* Configure QLMs */
	octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_QSGMII_DISABLED, 0, 0, 0,
			     1);
	/* 2x1 PCI-e, gen2, rc mode */
	octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1, 1, 1, 0, 1);
	octeon_configure_qlm(2, 3125, CVMX_QLM_MODE_SATA_2X1, 1, 0, 0, 1);
	debug("DLM 0 refclock: %llu\n", cvmx_qlm_measure_clock(0));
	debug("DLM 1 refclock: %llu\n", cvmx_qlm_measure_clock(1));
	debug("DLM 2 refclock: %llu\n", cvmx_qlm_measure_clock(2));
	return 0;
}

int octeon_add_user_boot_bus_fdt_devices(void)
{
	int rc;
	static const char *cpld_compat[] = { "imt,n83x-cpld-base", NULL };

	debug("In %s\n", __func__);
	rc = octeon_boot_bus_add_fdt_handler("cpld",
					     (void *)cpld_compat,
					     &octeon_boot_bus_generic_init);
	return rc;
}

int octeon_boot_bus_board_post_init(void *fdt_addr)
{
	int rc;
	uint64_t cpld_base = 0;

	debug("In %s\n", __func__);

	rc = octeon_boot_bus_get_dev_info("/soc/bootbus/cpld", NULL,
					  &cpld_base, NULL);
	if (rc || (cpld_base == 0)) {
		printf("%s: Error getting address of CPLD!\n", __func__);
		return rc;
	}

	debug("CPLD address: 0x%llx\n", cpld_base);

	cpld_init(cpld_base);

	cpld_accessible = 1;

	printf("CPLD device %02x%02x rev %02x (PCB rev %c)\n",
	       cpld_rd(CPLD_ADDR_CPLD_SIG_H),
	       cpld_rd(CPLD_ADDR_CPLD_SIG_L),
	       cpld_rd(CPLD_ADDR_CPLD_REV) & 0xf,
	       (cpld_rd(CPLD_ADDR_CPLD_REV) >> 4) + 'A');

	return 0;
}

int octeon_vitesse_coma(bool start)
{
	uint8_t misc_csr =
	    cpld_rd(CPLD_ADDR_MISC_CSR) & ~CPLD_MISC_CSR_PHY_COMA;

	if (start)
		misc_csr |= CPLD_MISC_CSR_PHY_COMA;

	cpld_wr(CPLD_ADDR_MISC_CSR, misc_csr);

	return 0;
}

int early_board_init(void)
{
	debug("%s: ENTER\n", __func__);
	/* Wait at least 10ms */
	mdelay(10);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(OCTEON_NIAGARA83X_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIAGARA83X,
				    CONFIG_OCTEON_NIAGARA83X_MAJOR,
				    CONFIG_OCTEON_NIAGARA83X_MINOR);

	/* CN70XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	return 0;
}

int board_fixup_fdt(void)
{
	const char *fdt_key = "0,n830t6";
	int v;
	union cvmx_smix_en smix_en;

	/* Make sure SMI/MDIO is enabled so we can query PHYs */
	smix_en.u64 = cvmx_read_csr(CVMX_SMIX_EN(0));
	if (!smix_en.s.en) {
		smix_en.s.en = 1;
		cvmx_write_csr(CVMX_SMIX_EN(0), smix_en.u64);
	}

	if ((v = cvmx_mdio_read(0, 0x10, 0x2)) != 0xFFFF) {
		fdt_key = "0,n830t5";
	}

	octeon_fdt_patch(working_fdt, fdt_key, "cavium,board-trim");

	return 0;
}

int late_board_init(void)
{
	int i = 0;

	cpld_wr(0x4, 0xC0);	/* reset both USB hubs */
	mdelay(5);
	cpld_wr(0x4, 0x00);
	mdelay(100);

	i2c_set_bus_num(USB2514B_BUS);

	for (i = 0; i < ARRAY_SIZE(usb2514b_init); i++) {
		uint8_t data[2];
		data[0] = 0x01;	/* smbus block write length */
		data[1] = usb2514b_init[i][1];
		i2c_write(USB2514B_DEV, usb2514b_init[i][0], 1, &data, 2);
	}

	i2c_set_bus_num(TUSB8041_BUS);

	for (i = 0; i < ARRAY_SIZE(tusb8041_init); i++)
		i2c_write(TUSB8041_DEV, tusb8041_init[i][0], 1,
			  &tusb8041_init[i][1], 1);

	return 0;
}
