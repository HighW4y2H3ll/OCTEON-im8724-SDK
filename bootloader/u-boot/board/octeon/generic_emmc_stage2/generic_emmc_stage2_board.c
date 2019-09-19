/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
	int i;
	static const uint8_t tuple_addresses[] = {
		OCTEON_EBB6100_BOARD_EEPROM_TWSI_ADDR, 0x54,
	};

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */
	for (i = 0; i < ARRAY_SIZE(tuple_addresses); i++) {
		gd->arch.tlv_addr = tuple_addresses[i];
		octeon_board_get_descriptor(CVMX_BOARD_TYPE_GENERIC, 1, 0);
		if (gd->board_type != CVMX_BOARD_TYPE_GENERIC) {
			debug("%s: found TLV EEPROM at address 0x%x\n",
			      __func__, gd->arch.tlv_addr);
			break;
		}
	}
	if (gd->board_type == CVMX_BOARD_TYPE_GENERIC) {
		puts("ERROR: Board TLV descriptor not found!  Cannot continue!\n"
		     "Use the tlv_eeprom command to program the board information into this board.\n");
		return -1;
	}

	octeon_board_get_clock_info(GENERIC_EMMC_DEF_DRAM_FREQ);

	if (gd->mem_clk < 100 || gd->mem_clk > 2000)
		gd->mem_clk = GENERIC_EMMC_DEF_DRAM_FREQ;

	octeon_board_get_mac_addr();

	return 0;
}

void late_board_init(void)
{
	char buf[256];
	char *board_name;
	board_name = getenv("boardname");
	if (board_name) {
		sprintf(buf, "u-boot-octeon_%s.bin", board_name);
		setenv("octeon_stage3_bootloader", buf);
	}
}
