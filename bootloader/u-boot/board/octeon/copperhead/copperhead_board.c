/*
 * (C) Copyright 2004-2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <search.h>
#include <command.h>
#include <spi_flash.h>
#include <spi.h>
#include <fdtdec.h>
#include <env_callback.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-board.h>
#include <asm/arch/cvmx-spemx-defs.h>
#include <asm/arch/octeon_pci.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-sli-defs.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_CMD_OCTEON_VPD
/**
 * Outputs the warning and over temperature values
 *
 * @param[out]	warn_temp	Integer portion of warning temperature
 * @param[out]	over_temp_frac	Fractional portion of warning temperature
 * @param[out]	over_temp	Integer portion of over temperature
 * @param[out]	over_temp_frac	Fractional portion of over temperature
 *
 * @return 0 for success, error otherwise
 */
extern int ssd_smbus_get_temps(int *warn_temp, int *warn_temp_frac,
			       int *over_temp, int *over_temp_frac);

/**
 * Gets the board model and serial number
 *
 * @param[out]	model_no	21 character \0 terminated model number
 * @param[out]	serial_no	41 character \0 terminated serial number
 *
 * @return	0 for success, -1 on error
 */
int board_get_ssd_vpd_model_serial(char *model_no, char *serial_no)
{
	const char *cmodel;

	switch (octeon_get_available_core_count()) {
	case 12:
		cmodel = "CN2350";
		break;
	case 16:
		cmodel = "CN2360";
		break;
	default:
		cmodel = "UNKNOWN";
		break;
	}
	strncpy(serial_no, (const char *)gd->arch.board_desc.serial_str, 21);
	snprintf(model_no, 21, "%s-210SVPT-G", cmodel);

	return 0;
}

/**
 * Returns the maximum power draw of the board in watts
 *
 * @param initial	true for initial power, false for maximum power
 *
 * @return power in watts or 0 if unknown
 */
int board_get_ssd_vpd_power(bool initial)
{
	switch (octeon_get_available_core_count()) {
	case 12:
		return 27;
	case 16:
		return 32;
	default:
		return 0;
	}
}
#endif /* CONFIG_CMD_OCTEON_VPD */

/**
 * This is called to enable or disable access to the VPD EEPROM and SA56004X
 *
 * @param	enable	Set to true to enable the EEPROM, false to disable
 *			and make it available to the PCIe host
 */
void board_enable_vpd(bool enable)
{
	gpio_set_value(9, enable ? 1 : 0);
}

void board_configure_qlms(void)
{
	octeon_configure_qlm(4, 3000, CVMX_QLM_MODE_SATA_2X1, 0, 0, 0, 0);
	octeon_configure_qlm(5, 103125, CVMX_QLM_MODE_XFI_1X2, 0, 0, 2, 0);
	/* Apply amplitude tunning to 10G interface */
	octeon_qlm_tune_v3(0, 4, 3000, -1, -1, 7, -1);

	octeon_qlm_tune_v3(0, 5, 103125, 0x0d, 0xb2, -1, -1);
	octeon_qlm_set_channel_v3(0, 5, 0);
	octeon_qlm_dfe_disable(0, 5, -1, 103125, CVMX_QLM_MODE_XFI_1X2);

	debug("QLM 4 reference clock: %d\n"
	      "DLM 5 reference clock: %d\n",
	      cvmx_qlm_measure_clock(4),
	      cvmx_qlm_measure_clock(5));
}

void octeon_board_poll(void)
{
	octeon_board_restore_pf();
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

	board_enable_vpd(true);
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
	/* Set conversion rate to 1.0 */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0xA, 0x04);
	board_enable_vpd(false);
	i2c_set_bus_speed(100000);
}

int early_board_init(void)
{
	octeon_board_get_clock_info(COPPERHEAD_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_COPPERHEAD, 1, 0);

	octeon_board_get_mac_addr();

	gpio_direction_input(0);	/* Failsafe		*/
	gpio_direction_output(1, 1);	/* General GPIO LED	*/

	/* Reset the Broadcom BCM84856, NOTE: reset is active LOW */
	gpio_direction_output(7, 0);	/* BCM84856 reset	*/
	/* Reset the eMMC device */
	gpio_direction_output(8, 0);	/* eMMC reset		*/

	/* Give us local access to the temperature sensor */
	gpio_direction_output(9, 1);	/* temp switch		*/
	gpio_direction_input(10);	/* therm event		*/
	gpio_direction_input(12);	/* PWR-in ALT#		*/
	gpio_direction_input(13);	/* ALT-P#		*/

	program_temp();

	mdelay(1);
	gpio_set_value(7, 1);	/* Take PHY out of reset */

	return 0;
}

/**
 * Perform late-stage board initialization
 *
 * @return 0 for success
 */
int late_board_init(void)
{
	const char *altbootcmd;

	altbootcmd = getenv("altbootcmd");
	if (gpio_get_value(15)) {
		setenv("bootcmd", altbootcmd);
		debug("Changed bootcmd to alternate boot command \"%s\"\n",
		      altbootcmd);
		setenv("netsingle", "1");
	}

	octeon_board_restore_pf();

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

/**
 * Save the PCIe reg0 which can get trashed if there's an FLR interrupt
 *
 * @return	0 for success
 */
int early_board_init_r(void)
{
	octeon_board_save_pcie_id_override();

	return 0;
}

U_BOOT_CMD(fixspi32, 1, 0, do_fixspi32,
	   "Fixes early CN73xx based Liquid I/O boards to enable SPI booting",
	   "Programs the SPI EEPROM to always use 32-bit addressing, needed to\n"
	   "fix early boards with improperly programmed SPI NOR flashes");
