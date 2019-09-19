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
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_board_phy.h>
#include <pci.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_fdt.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

#if 0
static const char *isp_voltage_labels[] = {
	NULL      ,		/* Chan 0  */
	"DCOK_QLM",		/* Chan 1  */
	NULL      ,		/* Chan 2  */
	"1.1V    ",		/* Chan 3  */
	"1.5V    ",		/* Chan 4  */
	"2.5V    ",		/* Chan 5  */
	NULL      ,		/* Chan 6  */
	"DDR 1.5V",		/* Chan 7  */
	"Core VDD",		/* Chan 8  */
	"3.3V    ",		/* Chan 9  */
	"5.0V    ",		/* Chan 10 */
	"12V / 3 ",		/* Chan 11 */
	"VCCA    ",		/* VCCA    */
	"VCCINP  ",		/* VCCINP  */
	0
};
#endif

extern void print_isp_volt(const char **labels, uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts(uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts_avg(int loops, uint16_t isp_dev_addr, uint8_t adc_chan);

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	cvmx_mio_qlmx_cfg_t mio_qlmx;
	char *eptr;

	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(2));
	if (mio_qlmx.s.qlm_spd == 15) {
		fdt_key = "0,none";	/* Disabled */
	} else if (mio_qlmx.s.qlm_cfg == 3) {
		fdt_key = "0,xaui";
	} else if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "0,sgmii";
	}
	else {
		fdt_key = "0,none";	/* Disabled */
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	mio_qlmx.u64 = cvmx_read_csr(CVMX_MIO_QLMX_CFG(0));
	if (mio_qlmx.s.qlm_spd == 15) {
		fdt_key = "1,none";	/* Disabled */
	} else if (mio_qlmx.s.qlm_cfg == 3) {
		fdt_key = "1,xaui";
	} else if (mio_qlmx.s.qlm_cfg == 2) {
		fdt_key = "1,sgmii";
	} else {
		fdt_key = "1,none";
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);


	/*
	 * If 'enable_spi_eeprom' is set in the environment, clear bit
	 * 2 of register 0x15 in the CPLD to enable the SPI EEPROM,
	 * then hijack octeon_fdt_patch to enable/disable the eeprom
	 * in the device tree.
	 */
	eptr = getenv("enable_spi_eeprom");
	fdt_key = "9,none";
	if (eptr) {
		uchar rv;
		int ret;
		ret = i2c_read(0x6c, 0x15, 1, &rv, 1);
		if (ret == 0) {
			if (rv == 0)
				fdt_key = "9,eeprom";
			i2c_reg_write(0x6c, 0x15, rv);
		}
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

int checkboard(void)
{
        int core_mVolts, dram_mVolts;

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

		char mcu_ip_msg[64] = { 0 };

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
		char tmp[10];
		int characters, idx = 0, value = core_mVolts;

		idx = sprintf(tmp, "%lu ", gd->cpu_clk);
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

#if 0				/* DEBUG use only */
	{

		char *eptr = getenv("jtag_ic50");
		if (eptr) {
			/* Do experimental JTAG changes for pass 1.1 */
			int qlm_num;

			int ic50_val = simple_strtol(eptr, NULL, 10);
			int ir50_val = ic50_val;
			eptr = getenv("jtag_ir50");

			if (eptr)
				ir50_val = simple_strtol(eptr, NULL, 10);
			else
				puts("Warning: jtag_ir50 val not set, using ic50 value\n");

			printf("Setting QLM DAC ir50=0x%x, ic50=0x%x via JTAG chain\n",
			       ir50_val, ic50_val);

			/* New ss are xored with default values, which are 0x11 */
			ic50_val ^= 0x11;
			ir50_val ^= 0x11;

			cvmx_helper_qlm_jtag_init();
			for (qlm_num = 0; qlm_num < 3; qlm_num++) {
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num, 34);
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ir50_val);	/* ir50dac */
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ic50_val);	/* ic50dac */
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num,
								 300 - 44);
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num, 34);
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ir50_val);	/* ir50dac */
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ic50_val);	/* ic50dac */
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num,
								 300 - 44);
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num, 34);
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ir50_val);	/* ir50dac */
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ic50_val);	/* ic50dac */
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num,
								 300 - 44);
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num, 34);
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ir50_val);	/* ir50dac */
				cvmx_helper_qlm_jtag_shift(qlm_num, 5, ic50_val);	/* ic50dac */
				cvmx_helper_qlm_jtag_shift_zeros(qlm_num,
								 300 - 44);
				cvmx_helper_qlm_jtag_update(qlm_num);
			}
		}
	}
#endif
	return 0;
}

/**
 * Due to bugs in the MCU I2C polling we disable it
 */
int board_early_init_f(void)
{
	mdelay(10);
	debug("Disabling MCU TWSI polling\n");
	i2c_set_bus_num(0);
	i2c_reg_write(BOARD_MCU_TWSI_ADDR, 0xF, 1);
	mdelay(10);
	return 0;
}

/**
 * Once relocation has occurred and we're no longer running out of flash
 * we can re-enable polling.
 */
int late_board_init(void)
{
	debug("Enabling MCU TWSI polling\n");
	mdelay(10);
	i2c_set_bus_num(0);
	i2c_reg_write(BOARD_MCU_TWSI_ADDR, 0xF, 0);
	mdelay(10);
	return 0;
}

int early_board_init(void)
{
	cvmx_mio_fus_pll_t fus_pll;

	int reg;
	i2c_set_bus_num(0);

	/* configure clk_out pin */
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
	octeon_board_get_clock_info(EBB6100_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBB6100, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = 50000000;

	octeon_board_get_mac_addr();

	octeon_led_str_write("Booting.");
	return 0;
}
