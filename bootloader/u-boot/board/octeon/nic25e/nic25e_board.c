/*
 * (C) Copyright 2004-2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <i2c.h>
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
#include <spi_flash.h>
#include <spi.h>

DECLARE_GLOBAL_DATA_PTR;

extern int uboot_avago_init(int i2c_bus, int i2c_base_addr,
			    uint32_t chip, uint32_t ring, bool enable_fec);

void board_configure_qlms(void)
{
	int tx_swing = (int)getenv_ulong("tx_swing", 16, 0xaUL);
	octeon_configure_qlm(2, 6250, CVMX_QLM_MODE_XLAUI, 0, 0, 2, 0);
	octeon_configure_qlm(4, 8000, CVMX_QLM_MODE_SATA_2X1, 0, 0, 0, 0);

	octeon_qlm_tune_v3(0, 2, 6250, tx_swing, 0xa0);
}

void octeon_board_poll(void)
{
	cvmx_update_rx_activity_led(0, 0, true);
}

void board_net_preinit(void)
{
	/* Turn on SFP+ transmitters */
	gpio_set_value(16, 0);

	/* Take the Avago AVSP-5410-0P1 out of reset */
	gpio_set_value(7, 1);
	mdelay(50);
	debug("Initializing Avago AVSP-5410-0P1 (TODO)\n");
	mdelay(50);
}

void board_net_postinit(void)
{
	bool enable_fec = getenv("disable_fec") ? false : true;

	uboot_avago_init(0, 0x4e, 0, 0, enable_fec, 0x18);
}

int early_board_init(void)
{
	octeon_board_get_clock_info(NIC25E_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC25E, 1, 0);

	octeon_board_get_mac_addr();

	gpio_direction_input(0);	/* Failsafe		*/
	gpio_direction_output(1, 1);	/* General GPIO LED	*/
	gpio_direction_output(2, 0);	/* SFP+ 0 Link status	*/
	gpio_direction_output(3, 0);	/* SFP+ 0 Activity	*/
	gpio_direction_input(6);	/* AVSP-INTERR#		*/
	/* Reset the Avago AVSP-5410-0P1 */
	gpio_direction_output(7, 0);	/* AVSP-5410 reset	*/
	/* Reset the eMMC device */
	gpio_direction_output(8, 0);	/* eMMC reset		*/

	/* Give us local access to the temperature sensor */
	gpio_direction_output(9, 1);	/* temp switch		*/
	gpio_direction_input(10);	/* therm event		*/
	gpio_direction_input(11);	/* Avago AVSP-5410 power good */
	gpio_direction_input(12);	/* PWR-in ALT#		*/
	gpio_direction_input(13);	/* ALT-P#		*/

	gpio_direction_output(16, 1);	/* SFP0 TX disable	*/
	gpio_direction_input(17);	/* SFP0 MOD-ABS		*/
	gpio_direction_input(18);	/* SFP0 RX LOS		*/
	gpio_direction_input(19);	/* SFP0 TX-fault	*/

	/* Note that the SA56004 will be programmed again later with DTT */
	/* Internal SA56004 temp, approximates ambient/board temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x20, 80);

	/* Internal Octeon temp, approximates junction temp. */
	i2c_reg_write(CONFIG_SYS_I2C_DTT_ADDR, 0x19, 105);

	return 0;
}

struct avago_hdl;

bool avago_is_i2c_communication_method(struct avago_hdl *ah)
{
	return true;
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

U_BOOT_CMD(fixspi32, 1, 0, do_fixspi32,
	   "Fixes early CN73xx based Liquid I/O boards to enable SPI booting",
	   "Programs the SPI EEPROM to always use 32-bit addressing, needed to\n"
	   "fix early boards with improperly programmed SPI NOR flashes");
