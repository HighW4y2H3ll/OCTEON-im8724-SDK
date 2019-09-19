/*
 * (C) Copyright 2004-2014 Cavium Inc.  <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

static const uint8_t tuple_addresses[] = {
	OCTEON_EVB7000_BOARD_EEPROM_TWSI_ADDR,
	OCTEON_SFF7000_BOARD_EEPROM_TWSI_ADDR,
};

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
	const int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
	int i;

	gd->arch.ddr_ref_hertz = 50000000;
	gd->arch.tlv_addr = CONFIG_SYS_I2C_EEPROM_ADDR;

	for (i = 0; i < ARRAY_SIZE(tuple_addresses); i++) {
		gd->arch.tlv_addr = tuple_addresses[i];
		debug("%s: Trying TLV EEPROM at address 0x%x\n",
		      __func__, tuple_addresses[i]);
		octeon_board_get_descriptor(CVMX_BOARD_TYPE_GENERIC, 1, 0);
		if (gd->board_type != CVMX_BOARD_TYPE_GENERIC)
			break;
	}
	if (i == ARRAY_SIZE(tuple_addresses) ||
	    gd->board_type == CVMX_BOARD_TYPE_GENERIC) {
		puts("ERROR: Board TLV descriptor not found!  Cannot continue!\n"
		     "Use the tlv_eeprom command to program the board type.\n");
		return -1;
	}

	octeon_board_get_clock_info(DEFAULT_DDR3_CLOCK_FREQ_MHZ);

	printf("Board type: %s\n",
	       cvmx_board_type_to_string(gd->board_type));

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

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
