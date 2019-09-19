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
#include <i2c.h>
#include <mtd/cfi_flash.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <miiphy.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/arch/cvmx-qlm.h>

DECLARE_GLOBAL_DATA_PTR;

static const char *isp_voltage_labels[] = {
	NULL      ,		/* Chan 0  */
	"DCOK_QLM",		/* Chan 1  */
	NULL,			/* Chan 2  */
	"1.2V",			/* Chan 3  */
	NULL,			/* Chan 4  */
	"2.5V",			/* Chan 5  */
	"DDR1 1.5V",		/* Chan 6  */
	"DDR0 1.5V",		/* Chan 7  */
	"Core VDD",		/* Chan 8  */
	"3.3V",			/* Chan 9  */
	"5.0V",			/* Chan 10 */
	"12V / 3",		/* Chan 11 */
	"VCCA",			/* VCCA    */
	"VCCINP",		/* VCCINP  */
	0
};

extern void print_isp_volt(const char **labels, uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts(uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts_avg(int loops, uint16_t isp_dev_addr, uint8_t adc_chan);

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	union cvmx_mio_qlmx_cfg qlm_mode;
	int i;
	char fdt_key[20];

	for (i = 0; i < cvmx_qlm_get_num(); i++) { /* 5 & 6 are ILK, real QLMs 1 & 2 */
		qlm_mode.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(i));
		if (qlm_mode.s.qlm_spd == 0xf) {
			sprintf(fdt_key, "%d,none", i); /* Disabled */
		} else {
			switch (qlm_mode.s.qlm_cfg) {
			case 2:
				sprintf(fdt_key, "%d,sgmii", i);
				break;
			case 3:
				sprintf(fdt_key, "%d,xaui", i);
				break;
			case 7:
				sprintf(fdt_key, "%d,rxaui", i);
				break;
			default:
				sprintf(fdt_key, "%d,none", i);
				break;
			}
			debug("%s: Setting QLM %d to %s\n",
			      __func__, i, fdt_key);
		}
		octeon_fdt_patch(working_fdt, fdt_key, NULL);
	}
	return 0;
}

/* Raise an integer to a power */
static inline uint64_t ipow(uint64_t base, uint64_t exp)
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

int checkboard(void)
{
        int core_mVolts, dram_mVolts0, dram_mVolts1;

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	{
		uchar value = 0xf3;
		/* Configure MDIO mux to connect MGMT port _and_ QLM4 to MDIO 0. */
		/* Connect just QLM0 to MDIO 1, as only QLM0 has GMX interfaces */
		i2c_set_bus_num(0);
		i2c_write(0x71, 0, 1, &value, 1);
		cvmx_wait_usec(11000);	/* Let the write complete */

	}
#endif
	if (octeon_show_info()) {

		int mcu_rev_maj = 0;
		int mcu_rev_min = 0;
		char mcu_ip_msg[64] = { 0 };
		char tmp[10];
		int characters, idx = 0, value;

		if (twsii_mcu_read(0x00) == 0xa5
		    && twsii_mcu_read(0x01) == 0x5a) {
			gd->arch.mcu_rev_maj = mcu_rev_maj = twsii_mcu_read(2);
			gd->arch.mcu_rev_min = mcu_rev_min = twsii_mcu_read(3);
		}

		core_mVolts  = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 8);
		dram_mVolts0 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 7); /* DDR0 1.5V */
		dram_mVolts1 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 6); /* DDR1 1.5V */

		if (twsii_mcu_read(0x14) == 1)
			sprintf(mcu_ip_msg, "MCU IPaddr: %d.%d.%d.%d, ",
				twsii_mcu_read(0x10),
				twsii_mcu_read(0x11),
				twsii_mcu_read(0x12), twsii_mcu_read(0x13));
		printf("MCU rev: %d.%02d, %sCPU voltage: %d.%03d DDR{0,1} voltages: %d.%03d,%d.%03d\n",
		       gd->arch.mcu_rev_maj, gd->arch.mcu_rev_min, mcu_ip_msg,
		       core_mVolts  / 1000, core_mVolts  % 1000,
		       dram_mVolts0 / 1000, dram_mVolts0 % 1000,
		       dram_mVolts1 / 1000, dram_mVolts1 % 1000);

#define LED_CHARACTERS 8
		value = core_mVolts;

		idx = sprintf(tmp, "%d ", (int)gd->cpu_clk);
		characters = LED_CHARACTERS - idx;

		if (value >= 1000) {
			idx += sprintf(tmp + idx, "%d", value / 1000);
			characters = LED_CHARACTERS - idx;
		}
		characters -= 1;	/* Account for decimal point */
		value %= 1000;
		value = DIV_ROUND_UP(value, ipow(10, max(3 - characters, 0)));

		idx += sprintf(tmp + idx, ".%0*d", min(3, characters), value);

		/* Display CPU speed and voltage on display */
		octeon_led_str_write(tmp);
		print_isp_volt(isp_voltage_labels, BOARD_ISP_TWSI_ADDR, 8);
		print_isp_volt(isp_voltage_labels, BOARD_ISP_TWSI_ADDR, 7);
		print_isp_volt(isp_voltage_labels, BOARD_ISP_TWSI_ADDR, 6);
	} else {
		octeon_led_str_write("Boot    ");
	}

	return 0;
}

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
	octeon_board_get_clock_info(EBB6800_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBB6800, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	octeon_led_str_write("Booting.");

	return 0;
}
