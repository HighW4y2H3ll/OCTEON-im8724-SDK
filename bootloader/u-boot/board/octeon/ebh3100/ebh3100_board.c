/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
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
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_boot_bus.h>

char *isp_voltage_labels[] = {
	NULL,			/* Chan 0 */
	NULL,			/* Chan 1  */
	NULL,			/* Chan 2  */
	"1.2V",			/* Chan 3  */
	"1.8V",			/* Chan 4  */
	"2.5V",			/* Chan 5  */
	NULL,			/* Chan 6  */
	"DDR 1.8V",		/* Chan 7  */
	"Core VDD",		/* Chan 8  */
	"3.3V",			/* Chan 9  */
	"5.0V",			/* Chan 10 */
	"12V / 3",		/* Chan 11 */
	"VCCA",			/* VCCA    */
	"VCCINP",		/* VCCINP  */
	0
};

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
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

/* In order to determine whether the switch is in single-chip or multi-chip mode, we attempt
 * to read the switch identifier register for port 9.
 *
 * This may wind up making some version of the EBH3100 boot up slowly, as it goes through a timeout cycle.
 */
static int marvell_sense_multi_chip_mode(void)
{
	int result;
	uint16_t value;
	const char devname[20] = "mdio-octeon0\0";

	result = miiphy_read(devname, 0x19, 3, &value);
	debug("%s: value: 0x%x, result: %d\n", __func__, value, result);
	return (value != 0x1a52) && !result;
}

int marvel_read(uint16_t device, uint16_t reg, uint16_t * value)
{
	int result;
	smi_cmd smiCmd;
	const char devname[20] = "mdio-octeon0\0";

	smiCmd.u16 = 0;
	smiCmd.s.mode = 1;
	smiCmd.s.op = 2 /* read */ ;
	smiCmd.s.device = device;
	smiCmd.s.reg = reg;
	miiphy_write(devname, 8, 0, smiCmd.u16);

	smiCmd.s.busy = 1;
	miiphy_write(devname, 8, 0, smiCmd.u16);

	/* Wait for the Busy bit to clear, denoting command completion. */
	do {
		miiphy_read(devname, 8, 0, &smiCmd.u16);
	} while (smiCmd.s.busy);

	*value = 0xDEAD;
	result = miiphy_read(devname, 8 /*device */, 1 /*reg */, value);
	return (result);
}

void marvel_write(uint16_t device, uint16_t reg, uint16_t value)
{
	smi_cmd smiCmd;
	const char devname[20] = "mdio-octeon0\0";

	miiphy_write(devname, 8, 1, value);

	smiCmd.u16 = 0;
	smiCmd.s.mode = 1;
	smiCmd.s.op = 1 /* write */ ;
	smiCmd.s.device = device;
	smiCmd.s.reg = reg;
	miiphy_write(devname, 8, 0, smiCmd.u16);

	smiCmd.s.busy = 1;
	miiphy_write(devname, 8, 0, smiCmd.u16);

	/* Wait for the Busy bit to clear, denoting command completion. */
	do {
		miiphy_read(devname, 8, 0, &smiCmd.u16);
	} while (smiCmd.s.busy);
}

void marvel_clear_statistics(void)
{
	uint16_t value;
	marvel_status_cmd statusCmd;

	/* Issue the 'flush all' command */
	statusCmd.s.busy = 1;
	statusCmd.s.op = 1 /*flush all */ ;
	statusCmd.s.mode = 1 /*rcv'd only */ ;
	statusCmd.s.ptr = 0;
	marvel_write(0x1B, 0x1D, statusCmd.u16);

	/* Wait for the Busy bit to clear, denoting command completion. */
	do {
		marvel_read(0x1B, 0x1D, &value);
	} while (value & 0x8000);
}

void board_mdio_init(void)
{
	static const char devname[20] = "mdio-octeon0\0";
	int device;
	int reg;
	uint16_t value;

	if (marvell_sense_multi_chip_mode()) {
		/* Early boards had the switch (mis)configured, so handle that
		 * case here
		 */
		/* Force a reset of the switch */
		marvel_read(0x1B, 4, &value);
		marvel_write(0x1B, 4, value | 0x8000);
		do {
			marvel_read(0x1B, 4, &value);
		} while (value & 0x8000);

		/* Force Port9 link speed and state. */
		marvel_write(0x19, 1, 0x003E);

		/* Turn off PPU. */
		marvel_read(0x1B, 4, &value);
		marvel_write(0x1B, 4, value & ~0x4000);

		/* Clear MGMII mode */
		marvel_read(0x10, 0x00, &value);
		marvel_write(0x10, 0x00, value & ~0x0040);

		marvel_read(0x11, 0x00, &value);
		marvel_write(0x11, 0x00, value & ~0x0040);

		marvel_read(0x12, 0x00, &value);
		marvel_write(0x12, 0x00, value & ~0x0040);

		marvel_read(0x13, 0x00, &value);
		marvel_write(0x13, 0x00, value & ~0x0040);

		/* Clear all port statistics maintained by the switch. */
		marvel_clear_statistics();

		/* Turn PPU back on. */
		marvel_read(0x1B, 4, &value);
		marvel_write(0x1B, 4, value | 0x4000);
		cvmx_wait(0x40000);

		for (device = 0x10; device < 0x14; device = device + 1) {
			for (reg = 0; reg < 7; reg = reg + 1) {
				marvel_read(device, reg, &value);
			}
		}
		device = 0x19;
		for (reg = 0; reg < 7; reg = reg + 1) {
			marvel_read(device, reg, &value);
		}

	} else {
		/* Force a reset of the switch */
		miiphy_read(devname, 0x1B, 4, &value);
		miiphy_write(devname, 0x1B, 4, value | 0x8000);
		do {
			miiphy_read(devname, 0x1B, 4, &value);
		} while (value & 0x8000);

		/* Force Port9 link speed and state. */
		miiphy_write(devname, 0x19, 1, 0x003E);

		/* Turn off PPU. */
		miiphy_read(devname, 0x1B, 4, &value);
		miiphy_write(devname, 0x1B, 4, value & ~0x4000);

		/* Clear MGMII mode */
		miiphy_read(devname, 0x10, 0x00, &value);
		miiphy_write(devname, 0x10, 0x00, value & ~0x0040);

		miiphy_read(devname, 0x11, 0x00, &value);
		miiphy_write(devname, 0x11, 0x00, value & ~0x0040);

		miiphy_read(devname, 0x12, 0x00, &value);
		miiphy_write(devname, 0x12, 0x00, value & ~0x0040);

		miiphy_read(devname, 0x13, 0x00, &value);
		miiphy_write(devname, 0x13, 0x00, value & ~0x0040);

		/* Clear all port statistics maintained by the switch. */

		/* Turn PPU back on. */
		miiphy_read(devname, 0x1B, 4, &value);
		miiphy_write(devname, 0x1B, 4, value | 0x4000);
		cvmx_wait(0x40000);

	}
}
#endif /* MII */

int octeon_get_board_major_rev(void)
{
	return (gd->arch.board_desc.rev_major);
}

int octeon_get_board_minor_rev(void)
{
	return (gd->arch.board_desc.rev_minor);
}

int checkboard(void)
{
#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	/* Enable MDIO */
	cvmx_write_csr(CVMX_GMXX_INF_MODE(0), 0x3);
#endif
	if (octeon_show_info())
		show_pal_info();
	else
		octeon_led_str_write("Boot    ");

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EBH3100_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBH3100, 1, 0);


	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on ebh5200 boards.
	 */
	cpu_ref = octeon_mcu_read_cpu_ref();

	/* CN52XX has a fixed internally generated 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = cpu_ref * 1000000;

	/* Some sanity checks */
	if (cpu_ref <= 0) {
		/* Default if cpu reference clock reading fails. */
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
	}
	if (gd->mem_clk < 100 || gd->mem_clk > 2000) {
		gd->mem_clk = EBH3100_DEF_DRAM_FREQ;
	}

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_PEXP_NPEI_DBG_DATA);
	data = data >> 18;
	data &= 0x1f;
	gd->cpu_clk = data * cpu_ref;

	/* adjust for 33.33 Mhz clock */
	if (cpu_ref == 33)
		gd->cpu_clk += (data) / 4 + data / 8;

	if (gd->cpu_clk < 100 || gd->cpu_clk > 1100)
		gd->cpu_clk = DEFAULT_ECLK_FREQ_MHZ;

	gd->cpu_clk *= 1000000;

	octeon_led_str_write("Booting.");

	return 0;
}
