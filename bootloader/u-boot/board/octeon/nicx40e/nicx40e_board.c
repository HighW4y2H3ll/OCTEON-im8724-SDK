/*
 * (C) Copyright 2016 Cavium Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/gpio.h>
#include <pca9555.h>
#include <spi_flash.h>
#include <spi.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

static void kill_fdt_phy(void *fdt, int offset, void *arg)
{
	int len, phy_offset;
	const fdt32_t *php;
	uint32_t phandle;

	php = fdt_getprop(fdt, offset, "phy-handle", &len);
	if (php && len == sizeof(*php)) {
		phandle = fdt32_to_cpu(*php);
		fdt_nop_property(fdt, offset, "phy-handle");
		phy_offset = fdt_node_offset_by_phandle(fdt, phandle);
		if (phy_offset > 0)
			fdt_nop_node(fdt, phy_offset);
	}
}

int board_fixup_fdt(void)
{
	const char *mode;
	char *fdt_key;

	mode = getenv("p0_mode");
	if (mode && !strcmp(mode, "xfi"))
		fdt_key = "4,xfi";
	else
		fdt_key = "4,xlaui";

	octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL,
				true, NULL, NULL);


	mode = getenv("p1_mode");
	if (mode && !strcmp(mode, "xfi"))
		fdt_key = "5,xfi";
	else
		fdt_key = "5,xlaui";

	octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL,
				true, NULL, NULL);
	return 0;
}

static void program_temp(void)
{
	union cvmx_rst_boot rst_boot;
	const int warn_temp = 85;
	const int over_temp = 105;
	uint8_t val;

	rst_boot.u64 = cvmx_read_csr(CVMX_RST_BOOT);
	debug("%s: lboot: %d, lboot_ext: %d\n", __func__,
	      rst_boot.s.lboot, rst_boot.s.lboot_ext);
	/* Return if this is a warm reboot since the temperature sensor
	 * should already be programmed.
	 */
	if (!((rst_boot.s.lboot & 0x3f) || (rst_boot.s.lboot_ext & 0xf)))
		return;
	/* Do this as fast as possible, 400KHz is supported */
	i2c_set_bus_speed(400000);

	gpio_set_value(1, 1);
	debug("%s: Warning temp: %d, critical temp: %d\n", __func__,
	      warn_temp, over_temp);
	/* Note that the SA56004 will be programmed again later with DTT */
	/* Internal SA56004 temp, approximates ambient/board temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x20, warn_temp);

	/* Internal Octeon temp, approximates junction temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x19, over_temp);

	/* Internal Octeon temp, approximates junction temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0xd, warn_temp);
	/* Set local warn temp 5C lower */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0xb, warn_temp - 5);
	/* Enable the fault queue, requiring three consecutive measurements
	 * before triggering T_CRIT.
	 */
	val = i2c_reg_read(CONFIG_SYS_I2C_DTT_ADDR, 0x03);
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x9, val | 1);
	gpio_set_value(1, 0);
	i2c_set_bus_speed(100000);
}

static int checkboardinfo(void)
{
#if 0
	int core_mVolts, dram_mVolts0, dram_mVolts1;

	debug("In %s\n", __func__);
	if (octeon_show_info()) {

		core_mVolts  = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 8);
		dram_mVolts0 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 7); /* DDR0 */
		dram_mVolts1 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 6); /* DDR1 */

		printf("CPU voltage: %d.%03d DDR{0,1} voltages: %d.%03d,%d.%03d\n",
		       core_mVolts  / 1000, core_mVolts  % 1000,
		       dram_mVolts0 / 1000, dram_mVolts0 % 1000,
		       dram_mVolts1 / 1000, dram_mVolts1 % 1000);

	}
#endif
	return 0;
}

/* Here is the description of the parameters that are passed to QLM configuration
 * 	param0 : The QLM to configure
 * 	param1 : Speed to configure the QLM at
 * 	param2 : Mode the QLM to configure
 * 	param3 : 1 = RC, 0 = EP
 * 	param4 : 0 = GEN1, 1 = GEN2, 2 = GEN3
 * 	param5 : ref clock select, 0 = 100Mhz, 1 = 125MHz, 2 = 156MHz
 * 	param6 : ref clock input to use:
 * 		 0 - external reference (QLMx_REF_CLK)
 * 		 1 = common clock 0 (QLMC_REF_CLK0)
 * 		 2 = common_clock 1 (QLMC_REF_CLK1)
 */
void board_configure_qlms(void)
{
	const char *mode;

	octeon_init_qlm(0);
	mdelay(100);

	mode = getenv("p0_mode");
	if (mode && !strcmp(mode, "xfi"))
		octeon_configure_qlm_cn78xx(0, 4, 103125, CVMX_QLM_MODE_XFI,
					    0, 0, 2, 0);
	else
		octeon_configure_qlm_cn78xx(0, 4, 103125, CVMX_QLM_MODE_XLAUI,
					    0, 0, 2, 0);

	mode = getenv("p1_mode");
	if (mode && !strcmp(mode, "xfi"))
		octeon_configure_qlm_cn78xx(0, 5, 103125, CVMX_QLM_MODE_XFI,
					    0, 0, 2, 0);
	else
		octeon_configure_qlm_cn78xx(0, 5, 103125, CVMX_QLM_MODE_XLAUI,
					    0, 0, 2, 0);


#ifdef DEBUG
	debug("QLM 4 reference clock: %d\n"
	      "QLM 5 reference clock: %d\n",
              cvmx_qlm_measure_clock(4),
	      cvmx_qlm_measure_clock(5));
#endif
	/* Apply amplitude tunning to 10G interface */
	octeon_qlm_tune_v3(0, 4, 103125, 0x13, 0x00, -1, -1);
	octeon_qlm_tune_v3(0, 5, 103125, 0x13, 0x00, -1, -1);
	/* override the default pre-ctle for low loss channels */
	octeon_qlm_set_channel_v3(0, 4, 0);
	octeon_qlm_set_channel_v3(0, 5, 0);
}

void octeon_board_poll(void)
{
	cvmx_update_rx_activity_led(4, 0, true);
	cvmx_update_rx_activity_led(5, 0, true);
}

/**
 * Resets the VSC7224 devices and the QSFP+ slots then take everything out
 * of reset and enable it.
 */
void board_net_preinit(void)
{
	const int bus = CONFIG_SYS_I2C_PCA9555_BUS;
	const int addr = CONFIG_SYS_I2C_PCA9555_ADDR;

	debug("%s: Resetting QSFP modules and VSC7224 devices\n", __func__);
	/* P02 = LPMODE-P0 = 0
	 * P03 = LPMODE-P1 = 0
	 * P06 = MODESEL-P0# = 0
	 * P07 = MODESEL-P1# = 0
	 * P11 (9) = VSC7224-0-RST = 1 (active high)
	 * P12 (10) = VSC7224-1-RST = 1 (active high)
	 * P13 (11) = VSC7224-2-RST = 1 (active high)
	 * P14 (12) = VSC7224-3-RST = 1 (active high)
	 * P15 (13) = QSFP-P1-RESET# = 0
	 * P16 = (14) = QSFP-P0-RESET# = 0
	 */
	pca9555_set_val(bus, addr, 0x7ecc, 0x1e00);
	mdelay(10);

	/* Take both QSFP+ modules out of low-power mode and out of reset and
	 * enable access to the eeprom.  Also take the VSC7224s out of reset
	 * and the QSFP modules out of reset.
	 *
	 * P02 = LPMODE-P0 = 0
	 * P03 = LPMODE-P1 = 0
	 * P06 = MODESEL-P0# = 0
	 * P07 = MODESEL-P1# = 0
	 * P11 (9) = VSC7224-0-RST = 0 (active high)
	 * P12 (10) = VSC7224-1-RST = 0 (active high)
	 * P13 (11) = VSC7224-2-RST = 0 (active high)
	 * P14 (12) = VSC7224-3-RST = 0 (active high)
	 * P15 (13) = QSFP-P1-RESET# = 1
	 * P16 = (14) = QSFP-P0-RESET# = 1
	 */
	pca9555_set_val(bus, addr, 0x7ecc, 0x6000);
	mdelay(50);
}

void board_net_postinit(void)
{
	debug("%s: Initializing Vitesse VSC7224\n", __func__);
	if (octeon_vitesse_init_vsc7224(gd->fdt_blob)) {
		printf("%s: Error initializing Vitesse reclocking chip\n",
		       __func__);
	}
}

int checkboard(void)
{
	const int bus = CONFIG_SYS_I2C_PCA9555_BUS;
	const int addr = CONFIG_SYS_I2C_PCA9555_ADDR;

        checkboardinfo();

	gpio_direction_input(0);	/* Normal/failsafe boot */
	/* Turn on temp sensor */
	gpio_direction_output(1, 1);	/* GPIO-TEMP-SWITCH */
	gpio_direction_input(2);	/* GPIO-THERM-EVENT */
	gpio_direction_input(3);	/* GPIO-PWR-IN-ALT# */
	gpio_direction_input(4);	/* GPIO-ALT-P# */
	gpio_direction_input(5);	/* VCS7224-0-LOS */
	gpio_direction_input(6);	/* VCS7224-1-LOS */
	gpio_direction_input(7);	/* VSC7224-2-LOS */
	gpio_direction_output(8, 0);	/* EMMC-RST# */
	gpio_direction_input(9);	/* VSC7224-3-LOS */
	gpio_direction_input(10);	/* MOD-PRS-P0# */
	gpio_direction_input(11);	/* MOD-PRS-P1# */
	gpio_direction_input(12);	/* FANIN1-GPIO */
	gpio_direction_input(13);	/* FANIN2-GPIO */
	gpio_direction_output(14, 1);	/* GPIO-P0-ACTIVITY# */
	gpio_direction_output(15, 1);	/* GPIO-P0-LINK# */
	gpio_direction_output(16, 1);	/* GPIO-P1-ACTIVITY# */
	gpio_direction_output(17, 1);	/* GPIO-P1-LINK# */
	gpio_direction_input(18);	/* GPIO-EXT-INT# */
	gpio_direction_output(19, 0);	/* OCT-THERMAL-TRIP_L */

	/* Configure GPIO expander pins for input and output
	 * P00 = INT-P0# (in)
	 * P01 = INT-P1# (in)
	 * P02 = LPMODE-P0 (out)
	 * P03 = LPMODE-P1 (out)
	 * P04 = FAN-ALERT# (in)
	 * P05 = FANCTL-GPIO (out)
	 * P06 = MODESEL-P0# (out)
	 * P07 = MODESEL-P1# (out)
	 * P10 (8) = POWER-LED-GPIO (out)
	 * P11 (9) = VSC7224-0-RST = 0 (active high) (out)
	 * P12 (10) = VSC7224-1-RST = 0 (active high) (out)
	 * P13 (11) = VSC7224-2-RST = 0 (active high) (out)
	 * P14 (12) = VSC7224-3-RST = 0 (active high) (out)
	 * P15 (13) = QSFP-P1-RESET# = 1 (out)
	 * P16 = (14) = QSFP-P0-RESET# = 1 (out)
	 * P17 = EVENT-DIMM# (in)
	 */
	pca9555_set_dir(bus, addr, 0xffff, 0x8013);
	mdelay(50);

	return 0;
}

int late_board_init(void)
{
	/* Enable all of the SFP+ transmitters */
	return 0;
}

int early_board_init(void)
{
	octeon_board_get_clock_info(NICX40E_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NICX40E, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull);

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	program_temp();
	return 0;
}

/** Read non-volatile configuration register */
#define CMD_READ_NV_CONFIG_REG		0xb5

/** Write non-volatile configuration register, must enable writes first */
#define CMD_WRITE_NV_CONFIG_REG		0xb1

#define CMD_WRITE_DISABLE		0x04
#define CMD_WRITE_ENABLE		0x06

/** Maximum SPI clock rate (20 MHz) */
#define OCTEON_SPI_MAX_CLOCK_HZ_7XXX	20000000

extern int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
			      size_t cmd_len, void *data, size_t data_len);

extern int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response,
			 size_t len);

extern int spi_flash_cmd_write(struct spi_slave *spi,
			       const u8 *cmd, size_t cmd_len,
			       const void *data, size_t data_len);

extern int spi_flash_set_4byte(struct spi_flash *flash, u8 jedec_mfr,
			       bool enable);

static inline int spi_flash_cmd_write_enable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_WRITE_ENABLE, NULL, 0);
}

/* Disable writing on the SPI flash */
static inline int spi_flash_cmd_write_disable(struct spi_flash *flash)
{
	return spi_flash_cmd(flash->spi, CMD_WRITE_DISABLE, NULL, 0);
}

#if 0
static int do_fixspi32(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	struct spi_flash *flash = NULL;
	int ret;
	uint8_t cmd[1];
	uint8_t data[4];

	flash = spi_flash_probe(0, 0, OCTEON_SPI_MAX_CLOCK_HZ_7XXX, 0);
	if (!flash) {
		puts("Could not access SPI flash device\n");
		return CMD_RET_FAILURE;
	}

	ret = spi_claim_bus(flash->spi);
	if (ret < 0) {
		puts("Unable to claim SPI bus\n");
		return CMD_RET_FAILURE;
	}

	cmd[0] = CMD_READ_NV_CONFIG_REG;
	ret = spi_flash_cmd_read(flash->spi, cmd, 1, data, 2);
	if (ret < 0) {
		puts("Error reading non-volatile configuration register\n");
		goto done;
	}
	printf("Old non-volatile configuration register value: 0x%02x%02x\n",
	       data[0], data[1]);
	if (data[0] & 1) {
		printf("Setting default to 4-byte addressing mode.\n");
		data[0] &= ~1;

		ret = spi_flash_cmd_write_enable(flash);
		if (ret < 0) {
			puts("Unable to enable writes\n");
			goto done;
		}

		cmd[0] = CMD_WRITE_NV_CONFIG_REG;
		ret = spi_flash_cmd_write(flash->spi, cmd, 1, data, 2);
		if (ret < 0) {
			printf("Error writing to NV config register\n");
			goto done;
		}

		ret = spi_flash_cmd_write_disable(flash);
		if (ret < 0) {
			puts("Unable to disable writes\n");
			goto done;
		}
		puts("SPI NOR programmed to default to 32-bit (4 byte) addressing mode\n");
	} else {
		puts("SPI NOR flash already fixed.\n");
	}
	/* Put device in 32-bit (4-byte) mode. */
	spi_flash_set_4byte(flash, 0x20, true);
	done:
	spi_release_bus(flash->spi);

	return ret < 0 ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
}

U_BOOT_CMD(fixspi32, 1, 0, do_fixspi32,
	   "Fixes early CN73xx based Liquid I/O boards to enable SPI booting",
	   "Programs the SPI EEPROM to always use 32-bit addressing, needed to\n"
	   "fix early boards with improperly programmed SPI NOR flashes");
#endif
