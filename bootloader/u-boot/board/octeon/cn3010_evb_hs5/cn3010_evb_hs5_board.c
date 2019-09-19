/*
 * (C) Copyright 2004 - 2012 Cavium Inc. <support@cavium.com>
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
#include <pci.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <miiphy.h>
#include <mtd/cfi_flash.h>
#include <asm/arch/octeon_boot_bus.h>

DECLARE_GLOBAL_DATA_PTR;

typedef union {
	uint16_t u16;
	struct {
		uint16_t busy:1, rsv:2, mode:1, op:2, device:5, reg:5;
	} s;
} smi_cmd;

typedef union {
	uint16_t u16;
	struct {
		uint16_t busy:1, op:3, mode:2, rsv:4, ptr:6;
	} s;
} marvel_status_cmd;

static const char devname[20] = "mdio-octeon0\0";

#if defined(CONFIG_CMD_MII)
#define mdio_write(w,x,y,z) miiphy_write(w,x,y,z)
#define mdio_read(w,x,y,z) miiphy_read(w,x,y,z)

/* In order to determine whether the switch is in single-chip or multi-chip mode, we attempt
 * to read the switch identifier register for port 9.
 *
 * This may wind up making some version of the EBH3100 boot up slowly, as it goes through a timeout cycle.
 */
static int marvell_sense_multi_chip_mode(void)
{
	uint16_t value;

	mdio_read(devname, 0x19, 3, &value);
	return (value != 0x1a52);
}

void board_mdio_init(void)
{
	uint16_t value;

	if (marvell_sense_multi_chip_mode()) {
		printf("\n\nERROR: marvell switch in multi-chip mode!!!!!\n\n");

	} else {
		/* Force a reset of the switch */
		mdio_read(devname, 0x1B, 4, &value);
		mdio_write(devname, 0x1B, 4, value | 0x8000);
		do {
			mdio_read(devname, 0x1B, 4, &value);
		} while (value & 0x8000);

		/* Force Port9 link speed and state. */
		mdio_write(devname, 0x19, 1, 0x003E);

		/* Turn off PPU. */
		mdio_read(devname, 0x1B, 4, &value);
		mdio_write(devname, 0x1B, 4, value & ~0x4000);

		/* Clear MGMII mode */
		mdio_read(devname, 0x10, 0x00, &value);
		mdio_write(devname, 0x10, 0x00, value & ~0x0040);

		mdio_read(devname, 0x11, 0x00, &value);
		mdio_write(devname, 0x11, 0x00, value & ~0x0040);

		mdio_read(devname, 0x12, 0x00, &value);
		mdio_write(devname, 0x12, 0x00, value & ~0x0040);

		mdio_read(devname, 0x13, 0x00, &value);
		mdio_write(devname, 0x13, 0x00, value & ~0x0040);

		/* Turn PPU back on. */
		mdio_read(devname, 0x1B, 4, &value);
		mdio_write(devname, 0x1B, 4, value | 0x4000);
		cvmx_wait(0x40000);

		if (miiphy_read(devname, 0x04, 27, &value) >= 0) {
			if ((value & 0xf) == 0xf) {
				printf("!!! Configuring Port 0 for MII mode !!!\n");
				cvmx_write_csr(CVMX_GMXX_INF_MODE(0), 0x7);
			}
		}
	}
}

#endif /* MII */


int checkboard(void)
{
	if (octeon_show_info())
		show_pal_info();
	else
		octeon_led_str_write("Boot    ");

	/* Set up switch complex */

	/* Force us into GMII mode, enable interface */
	/* NOTE:  Applications expect this to be set appropriately
	 ** by the bootloader, and will configure the interface accordingly */
	cvmx_write_csr(CVMX_GMXX_INF_MODE(0), 0x3);

	/* Enable SMI to talk with the GMII switch */
	cvmx_write_csr(CVMX_SMI_EN, 0x1);
	cvmx_read_csr(CVMX_SMI_EN);


	return 0;
}

int early_board_init(void)
{
	int cpu_ref = 50;
	union cvmx_dbg_data dbg_data;
	int mul;

	/* NOTE: this is early in the init process, so the serial port is not yet configured */

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

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CN3010_EVB_HS5, 2, 0);

	/* Even though the CPU ref freq is stored in the clock descriptor, we don't read it here.  The MCU
	 * reads it and configures the clock, and we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for normal operation on
	 * ebh3100 boards
	 */
	cpu_ref = octeon_mcu_read_cpu_ref();
	gd->mem_clk = octeon_mcu_read_ddr_clock();

	/* CN50XX has a fixed internally generated 50 MHz reference clock */
	if (OCTEON_IS_MODEL(OCTEON_CN50XX)) {
		gd->arch.ddr_ref_hertz = 50000000;
		gd->mem_clk = gd->arch.clock_desc.ddr_clock_mhz;
	}

	/* Some sanity checks */
	if (cpu_ref <= 0) {
		/* Default to 50 */
		cpu_ref = 50;
	}
	if (gd->mem_clk <= 0) {
		/* Default to 200 */
		gd->mem_clk = 200;
	}

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	dbg_data.u64 = cvmx_read_csr(CVMX_DBG_DATA);
	mul = dbg_data.cn30xx.c_mul;
	gd->cpu_clk = mul * cpu_ref;

	/* adjust for 33.33 Mhz clock */
	if (cpu_ref == 33)
		gd->cpu_clk += (mul) / 4 + mul / 8;

	if (gd->cpu_clk < 100 || gd->cpu_clk > 900)
		gd->cpu_clk = DEFAULT_ECLK_FREQ_MHZ;
	gd->cpu_clk *= 1000000;

	return 0;
}

int late_board_init(void)
{
	char prompt[32];
	snprintf(prompt, sizeof(prompt), "Octeon cn%c010_evb_hs5",
		 OCTEON_IS_MODEL(OCTEON_CN3XXX) ? '3' : '5');
	setenv("prompt", prompt);

	return 0;
}
