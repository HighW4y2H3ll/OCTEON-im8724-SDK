/*
 * (C) Copyright 2004,2005, 2012 Cavium, Inc.
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
#include <i2c.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

static int nac38_twsi_write8(int dev_addr, int ia, uint8_t val)
{
	return i2c_write(dev_addr, ia, 1, &val, 1);
}

static int nac38_twsi_read(int dev_addr, int ia)
{
	uint8_t val;
	i2c_read(dev_addr, ia, 1, &val, 1);
	return val;
}

void W83792d_ASR(int asr_type, int asr_timer)
{
	unsigned char temp;

	if (asr_type == 0) {	/* Disable ASR */
		temp = 0xcc;
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x01,
				  temp);

		temp = 0x00;	/* Disable 4 Minutes Watchdog */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x04,
				  temp);

		temp =
		    nac38_twsi_read(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x40);
		temp = temp & (~0x10);
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x40,
				  temp);
	} else if (asr_type == 1) {	/* Enable ASR */
		/* Enable Watchdog Function */
		temp =
		    nac38_twsi_read(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x40);
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x40,
				  temp | 0x10);

		/* read to clear stus */
		temp =
		    nac38_twsi_read(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x03);

		temp = 0x00;	/* Disable 4 Minutes */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x04,
				  temp);

		temp = asr_timer;	/* set ASR Timer */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x04,
				  temp);

		temp = 0x33;	/* Enable Hard watchdog */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x01,
				  temp);
	} else {			/* Trigger ASR */
		temp = asr_timer;	/* set ASR Timer */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x04,
				  temp);
	}
}

void init_w83792d(void)
{
	unsigned char temp3_cfg;

	temp3_cfg = nac38_twsi_read(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x4b);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x4b,
			  temp3_cfg | 0x04);	/* enable fan 7 */

	/* set tempearture1 to therml diode, 2 and 3 to thermistor */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x59, 0x10);

	temp3_cfg = nac38_twsi_read(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x5d);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x5d,
			  temp3_cfg | 0x01);	/* enable vbat */

	/* enable fan 1, 2 */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x47, 0x99);
	/* enable fan 3, 4 */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x5b, 0x99);
	/* enable fan 5 */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x5c, 0x19);
	/* enable fan 7 */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x9e, 0x09);

	/* Disable ASR */
	W83792d_ASR(0, 0);

	/* Smart Fan Initial */
	char *s;
	if ((s = getenv("CPU_Temp")) != NULL) {
		unsigned smart_fan_target;
		smart_fan_target = (unsigned)simple_strtoul(s, NULL, 0);
		if (smart_fan_target > 65) {
			printf(" Overriding CPU_Temp  %2d  \n",
			       smart_fan_target);
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x85, 0x41);	/* TTarget_MB   65 C */
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x86, 0x41);	/* TTarget_CPU1  65 C */
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x96, 0x41);	/* TTarget_CPU2   65 C */
		} else {
			printf(" Using CPU_Temp from environment: %2d  \n",
			       smart_fan_target);
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x85, smart_fan_target);
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x86, smart_fan_target);
			nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
					  0x96, smart_fan_target);
		}
	} else {
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
				  0x85, 0x41);	/* TTarget_MB   65 C */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
				  0x86, 0x41);	/* TTarget_CPU1  65 C */
		nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR,
				  0x96, 0x41);	/* TTarget_CPU2   65 C */
	}

	/* Fan Pre-Scale Register,  PWM frequency < 20kHz */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x80, 0x2D);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x82, 0x2D);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x93, 0x2D);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0xA0, 0x2D);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0xA1, 0x2D);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0xA2, 0x2D);

	/* Set to PWM mode, FAN_OUT1 to sync. with CPU temperature */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x81, 0x8B);
	/* Set to PWM mode, FAN_OUT2 to sync. with SYS temperature */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x83, 0x8B);
	/* Set to PWM mode, FAN_OUT3 to sync. with HDD temperature */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x94, 0x8B);
	/* Set FAN_OUT4 to sync. with CPU temperature, PWM mode */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0xA3, 0xAB);
	/*    nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0xA4, 0xAB);        // Set FAN_OUT5 to sync. with CPU temperature, PWM mode */
	/* Set Tolerance of target temperature to zero degree */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x87, 0x00);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x97, 0x00);

	/* Set DC mode  FAN Start and Non-Stop duty cycle ratio to 3/15 */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x88, 0x33);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x89, 0x33);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x98, 0x33);

	/* Set FAN Stop Time to zero, then FAN will not stop */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x8C, 0x00);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x8D, 0x00);
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x9A, 0x00);

	/* Set FAN Step Down Time (times in 10ths of seconds supposedly) */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x8E, 0x60);
	/* Set FAN Step Up Time */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x8F, 0x60);
	/* Enable Smart FAN control */
	nac38_twsi_write8(CONFIG_SYS_I2C_SYSMON_W83792D_ADDR, 0x84, 0x15);
}

int octeon_add_user_boot_bus_fdt_devices(void)
{
	int rc;
	static const char *lcd_compat[] = { "samsung,s6a0069", NULL };
	rc = octeon_boot_bus_add_fdt_handler("lcd-display",
					     (void *)lcd_compat,
					     &octeon_boot_bus_generic_init);
	return rc;
}

int checkboard(void)
{
	gpio_direction_output(6, 1);	/* GPIO 6 controls power loss */
	gpio_direction_output(12, 1);	/* GPIO 12 controls power */
	gpio_direction_output(5, 1);	/* GPIO 5 enables the LAN ports that are
					 * on the bypass relays
					 */
	init_w83792d();
	return 0;
}

int early_board_init(void)
{
	int cpu_ref = 33;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NAC38, 1, 0);

	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.clock_desc), ee_buf,
		       sizeof(octeon_eeprom_clock_desc_t));
	}

	if (gd->arch.clock_desc.cpu_ref_clock_mhz_x_8) {
		cpu_ref = gd->arch.clock_desc.cpu_ref_clock_mhz_x_8 / 8;
		gd->mem_clk = gd->arch.clock_desc.ddr_clock_mhz;
	} else {
		gd->flags |= GD_FLG_CLOCK_DESC_MISSING;
		/* Default values */
		cpu_ref = 33;
		gd->mem_clk = NAC38_REV1_DEF_DRAM_FREQ;
	}

	/* Some sanity checks */
	if (cpu_ref <= 0) {
		/* Default to 33 */
		cpu_ref = 33;
	}
	if (gd->mem_clk <= 0) {
		/* Default to 33 */
		gd->mem_clk = 200;
	}

	/*
	 * For the cn58xx the DDR reference clock frequency is used to
	 * configure the appropriate internal DDR_CK/DDR2_REF_CLK ratio in
	 * order to generate the ddr clock frequency specified by
	 * ddr_clock_mhz.  For cn38xx this setting should match the state
	 * of the DDR2 output clock divide by 2 pin DDR2_PLL_DIV2.
	 *
	 * For DDR@_PLL_DIV2 = 0 the DDR_CK/DDR2_REF_CLK ratio is 4
	 * For DDR@_PLL_DIV2 = 1 the DDR_CK/DDR2_REF_CLK ratio is 2
	 */
#define FIXED_DDR_REF_CLOCK_RATIO	4

	/*
	 * More details about DDR clock configuration used for LMC
	 * configuration for the CN58XX.  Not used for CN38XX.  Since the
	 * reference clock frequency is not known it is inferred from the
	 * specified DCLK frequency divided by the DDR_CK/DDR2_REF_CLK
	 * ratio.
	 */
	gd->arch.ddr_ref_hertz =
	    gd->mem_clk * 1000000 / FIXED_DDR_REF_CLOCK_RATIO;

	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_DBG_DATA);
	data = data >> 18;
	data &= 0x1f;

	gd->cpu_clk = data * cpu_ref;

	/* adjust for 33.33 Mhz clock */
	if (cpu_ref == 33)
		gd->cpu_clk += (data) / 4 + data / 8;

	if (gd->cpu_clk < 100 || gd->cpu_clk > 600)
		gd->cpu_clk = DEFAULT_ECLK_FREQ_MHZ;

	gd->cpu_clk *= 1000000;

	return 0;

}

int do_power(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	if ((strncmp(argv[1], "off", 3) == 0)
	    || strncmp(argv[1], "down", 4)) {
		gpio_set_value(12, 1);
	}
	if (strncmp(argv[1], "loss_e", 6) == 0) {
		gpio_set_value(6, 1);
		udelay(200);
		gpio_set_value(6, 0);
	}
	return 0;
}

U_BOOT_CMD(power, 2, 1, do_power,
	   "Perform power off\n",
	   "off     - Power off the system\n"
	   "down    - Power off the system\n"
	   "loss_en - Enable power loss function (power on after power loss)\n");
