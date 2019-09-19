/*
 * (C) Copyright 2004-2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <fdtdec.h>
#include <i2c.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/cvmx-bgxx-defs.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-board.h>
#include <asm/arch/octeon_pci.h>
#include <asm/arch/cvmx-rst-defs.h>
#include <malloc.h>
#include "../common/phy/avago.h"
#include "../common/phy/avago_avsp.h"
#include "../common/phy/avago_serdes.h"
#include <spi_flash.h>
#include <spi.h>
#include <net.h>
#include <asm/arch/octeon_fdt.h>

DECLARE_GLOBAL_DATA_PTR;

static bool eth0_10g = false;
static bool eth1_10g = false;
static bool avago_initialized = false;

extern int uboot_garnet_init(int i2c_bus, int i2c_base_addr, uint32_t chip,
			     uint32_t ring, bool enable_fec,
			     uint32_t host_tx_atten,
			     struct avago_serdes_tx_eq *line_tx_eq,
			     bool enable_10g);

/**
 * Enables and disables forward error correction
 *
 * @param	i2c_bus		i2c bus number of Garnet
 * @param	i2c_base_addr	i2c base address of Garnet
 * @param	chip		chip address
 * @param	ring		ring address
 * @param	enable_fec	true to enable fec (25G only), false to disable
 * @param	enable_10g	true if configured for 10g, false for 25g
 *
 * @return	0 for success, otherwise error
 */
extern int uboot_garnet_set_fec(int i2c_bus, int i2c_base_addr, uint32_t chip,
				uint32_t ring, bool enable_fec,
				bool enable_10g);

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
#endif

/**
 * Callback to configure the QLMs
 */
void board_configure_qlms(void)
{
	int tx_swing = 0x14;
	int preemp_tap = 0;
	int clk_source = gd->arch.board_desc.rev_major >= 3 ? 1 : 0;
	int eth0_speed, eth1_speed;
	int eth0_baud, eth1_baud;
	int eth0_mode, eth1_mode;

	eth0_speed = getenv_ulong("octeth0_speed", 10, 25);
	if (eth0_speed != 10 && eth0_speed != 25) {
		printf("Invalid speed %d for octeth%d.  octeth%d_speed be either 10 or 25.\n",
		       eth0_speed, 0, 0);
		eth0_speed = 25;
	} else if (eth0_speed == 10) {
		eth0_10g = true;
	}
	eth1_speed = getenv_ulong("octeth1_speed", 10, 25);
	if (eth1_speed != 10 && eth1_speed != 25) {
		printf("Invalid speed %d for octeth%d.  octeth%d_speed be either 10 or 25.\n",
		       eth1_speed, 1, 1);
		eth1_speed = 25;
	} else if (eth1_speed == 10) {
		eth1_10g = true;
	}
	eth0_baud = eth0_speed == 25 ? 6316 : 103125;
	eth1_baud = eth1_speed == 25 ? 6316 : 103125;
	eth0_mode = eth0_speed == 25 ? CVMX_QLM_MODE_XLAUI : CVMX_QLM_MODE_XFI;
	eth1_mode = eth1_speed == 25 ? CVMX_QLM_MODE_XLAUI : CVMX_QLM_MODE_XFI;
	debug("%s: clock source: %d\n", __func__, clk_source);
	debug("%s: configuring QLM %d for %s at %d baud.\n", __func__, 2,
	      eth0_speed == 25 ? "XLAUI" : "XFI", eth0_baud);
	octeon_configure_qlm(2, eth0_baud, eth0_mode, 0, 0, 3, clk_source);

	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E) {
		debug("%s: configuring QLM %d for %s at %d baud.\n", __func__, 3,
		      eth1_speed == 25 ? "XLAUI" : "XFI", eth1_baud);
		octeon_configure_qlm(3, eth1_baud, eth1_mode, 0, 0, 3,
				     clk_source);
	}

	if (gd->arch.board_desc.rev_major >= 3) {
		debug("%s: Configuring QLM %d for %s at %d baud.\n",
		      __func__, 4, "SATA", 3000);
		octeon_configure_qlm(4, 3000, CVMX_QLM_MODE_SATA_2X1, 0, 0, 0, 0);
		octeon_qlm_tune_v3(0, 4, 3000, -1, -1, 7, -1);
	}


	octeon_qlm_tune_v3(0, 2, eth0_baud, tx_swing, preemp_tap, -1, -1);
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E)
		octeon_qlm_tune_v3(0, 3, eth1_baud, tx_swing, preemp_tap, -1, -1);
}

/**
 * Polling function that updates the link status and activity LEDs
 */
void octeon_board_poll(void)
{
	cvmx_update_rx_activity_led(0, 0, true);
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E)
		cvmx_update_rx_activity_led(1, 0, true);

	octeon_board_restore_pf();
}

/**
 * Callback called before network initialization
 */
void board_net_preinit(void)
{
	/* Turn on SFP+ transmitters */
	gpio_set_value(16, 0);
	gpio_set_value(21, 0);

	/* Take the Avago AVSP-5410-0P1 out of reset */
	gpio_set_value(7, 1);
}

#ifdef CONFIG_CMD_OCTEON_VPD
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
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E)
		snprintf(model_no, 21, "%s-225SVPN-G", cmodel);
	else
		snprintf(model_no, 21, "%s-25SVPN-G", cmodel);

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

/**
 * This is called to enable or disable access to the VPD EEPROM
 *
 * @param	enable	Set to true to enable the EEPROM, false to disable
 *			and make it available to the PCIe host
 */
void board_enable_vpd(bool enable)
{
	gpio_set_value(9, enable ? 1 : 0);
}
#endif /* CONFIG_CMD_OCTEON_VPD */

/**
 * Turns test pattern output on or off.
 *
 * @param	interface	interface to configure
 * @param	lane		port index or lane to configure
 * @param	enable		Set to true to turn on, false to turn off
 */
void bgx_set_test_pattern(int interface, int lane, bool enable)
{
	union cvmx_bgxx_spux_br_tp_control tp_control;

	tp_control.u64 =
		cvmx_read_csr(CVMX_BGXX_SPUX_BR_TP_CONTROL(lane, interface));
	tp_control.s.tx_tp_en = enable ? 1 : 0;
	tp_control.s.scramble_tp = enable ? 1 : 0;
	cvmx_write_csr(CVMX_BGXX_SPUX_BR_TP_CONTROL(lane, interface),
		       tp_control.u64);
}

static bool get_enable_fec(int port)
{
	char env_name[16];
	const char *value;

	snprintf(env_name, sizeof(env_name), "enable_fec%d", port);
	value = getenv(env_name);

	if (!value)
		return false;

	switch (*value) {
	case 'y':
	case 'Y':
	case '1':
	case 't':
	case 'T':
		return true;
	default:
		return false;
	}
}

/**
 * Callback called after network initializtion.  This initializes the
 * Avago/Broadcom AVSP-5410.
 */
void board_net_postinit(void)
{
	bool enable_fec0 = get_enable_fec(0);
	bool enable_fec1 = get_enable_fec(1);
	struct avago_serdes_tx_eq line_tx_eq;
	ulong tx_atten0 = getenv_ulong("avago_line_tx_atten0", 0, 0x18);
	ulong tx_atten1 = getenv_ulong("avago_line_tx_atten1", 0, 0x18);
	cvmx_helper_interface_mode_t mode;
	bool is_10g;

	line_tx_eq.pre = getenv_ulong("avago_pre0", 0, 0);
	line_tx_eq.atten = getenv_ulong("avago_atten0", 0, 0);
	line_tx_eq.post = getenv_ulong("avago_post0", 0, 0);
	line_tx_eq.slew = getenv_ulong("avago_slew0", 0, 0);
	line_tx_eq.pre2 = getenv_ulong("avago_pre20", 0, 0);
	line_tx_eq.pre3 = getenv_ulong("avago_pre30", 0, 0);
	line_tx_eq.vert = getenv_ulong("avago_vert0", 0, 0);

	/* Set attenuation to 24 */
	mode = cvmx_helper_interface_get_mode(0);
	is_10g = (mode == CVMX_HELPER_INTERFACE_MODE_XFI);
	if (is_10g)
		enable_fec0 = false;
	debug("Initializing Avago port 0, FEC: %sabled, speed: %dGbps\n",
	      enable_fec0 ? "en" : "dis", is_10g ? 10 : 25);
	uboot_garnet_init(0, 0x4e, 0, 0, enable_fec0, tx_atten0, &line_tx_eq,
			  is_10g);

	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E) {
		line_tx_eq.pre = getenv_ulong("avago_pre1", 0, 0);
		line_tx_eq.atten = getenv_ulong("avago_atten1", 0, 0);
		line_tx_eq.post = getenv_ulong("avago_post1", 0, 0);
		line_tx_eq.slew = getenv_ulong("avago_slew1", 0, 0);
		line_tx_eq.pre2 = getenv_ulong("avago_pre21", 0, 0);
		line_tx_eq.pre3 = getenv_ulong("avago_pre31", 0, 0);
		line_tx_eq.vert = getenv_ulong("avago_vert1", 0, 0);
		mode = cvmx_helper_interface_get_mode(1);
		is_10g = (mode == CVMX_HELPER_INTERFACE_MODE_XFI);
		if (is_10g)
			enable_fec1 = false;
		debug("Initializing Avago port 1, FEC: %sabled, speed: %dGbps\n",
		      enable_fec1 ? "en" : "dis", is_10g ? 10 : 25);
		/* Set attenuation to 24 */
		uboot_garnet_init(0, 0x47, 0, 0, enable_fec1, tx_atten1,
				  &line_tx_eq, is_10g);
	}
	avago_initialized = true;
}

/**
 * Callback indicating type of communications to use for AVSP-5410
 */
bool avago_is_i2c_communication_method(struct avago_hdl *ah)
{
	return true;
}

/**
 * Callback to fixup the flat device-tree.
 */
int board_fixup_fdt(void)
{
	int node;
	const bool rev3 = (gd->arch.board_desc.rev_major >= 3);
	const bool nic225e =
		(gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E);
	const char *fdt_key =
		(gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NIC225E) ?
		"3,nic225e" : "3,nic125e";
	uint32_t range_data[5 * 8];
	int rc;

	rc = octeon_fdt_patch_rename(working_fdt, fdt_key, NULL, true,
				     NULL, NULL);
	if (rc)
		printf("%s: Error fixing up board with key %s\n", __func__,
		       fdt_key);

	debug("%s() rev3: %s\n", __func__, rev3 ? "true" : "false");
	/* Patch the PHY configuration based on board revision */
	rc = octeon_fdt_patch_rename(working_fdt,
				     rev3 ? "3,nor-flash" : "3,no-nor-flash",
			      "cavium,board-trim", false, NULL, NULL);
	if (rc)
		printf("%s: Error fixing up NOR flash in device tree\n",
		       __func__);

	rc = octeon_fdt_patch_rename(working_fdt, rev3 ? "4,sata" : "4,none",
				     NULL, false, NULL, NULL);
	if (rc)
		printf("%s: Error fixing up AHCI in device tree\n", __func__);

	if (!rev3 || !nic225e) {
		/* Modify the ranges for CS 0 */
		node = fdt_node_offset_by_compatible(working_fdt, -1,
						     "cavium,octeon-3860-bootbus");
		if (node < 0) {
			printf("%s: Error: cannot find boot bus in device tree!\n",
			       __func__);
			return -1;
		}
		rc = fdtdec_get_int_array(working_fdt, node, "ranges",
					  range_data, 5 * 8);
		if (rc) {
			printf("%s: Error reading ranges from boot bus FDT\n",
			       __func__);
			return -1;
		}
		range_data[2] = cpu_to_fdt32(0x10000);
		range_data[3] = 0;
		range_data[4] = 0;

		rc = fdt_setprop(working_fdt, node, "ranges", range_data,
				 sizeof(range_data));
		if (rc) {
			printf("%s: Error updating boot bus ranges in fdt\n",
			       __func__);
		}
	}
	return rc;
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

/**
 * Early initialization callback reads the TLV EEPROM and configures GPIOs
 */
int early_board_init(void)
{
	octeon_board_get_clock_info(NIC225E_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_NIC225E, 1, 0);

	octeon_board_get_mac_addr();

	gpio_direction_input(0);	/* Failsafe		*/
	gpio_direction_output(1, 1);	/* General GPIO LED	*/
	gpio_direction_output(2, 0);	/* SFP+ 0 Link status	*/
	gpio_direction_output(3, 0);	/* SFP+ 0 Activity	*/
	gpio_direction_output(4, 0);	/* SFP+ 1 Link status	*/
	gpio_direction_output(5, 0);	/* SFP+ 1 Activity	*/
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

	gpio_direction_output(21, 1);	/* SFP1 TX disable	*/
	gpio_direction_input(22);	/* SFP1 MOD-ABS		*/
	gpio_direction_input(23);	/* SFP1 RX LOS		*/
	gpio_direction_input(24);	/* SFP1 TX-fault	*/

	program_temp();

	return 0;
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

/**
 * Operations to perform just before the U-Boot command prompt
 *
 * @return	0 for success
 */
int late_board_init(void)
{
#ifdef CONFIG_DTT_SAA56004X
	program_temp();

	board_enable_vpd(false);
#endif
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

U_BOOT_CMD(fixspi32, 1, 0, do_fixspi32,
	   "Fixes early CN73xx based Liquid I/O boards to enable SPI booting",
	   "Programs the SPI EEPROM to always use 32-bit addressing, needed to\n"
	   "fix early boards with improperly programmed SPI NOR flashes");

int on_enablefec(const char *name, const char *value, enum env_op op, int flags)
{
	int device;
	int i2c_bus = 0;
	int i2c_addr;
	cvmx_helper_interface_mode_t mode;
	bool is_10g = false;
	if (!avago_initialized)
		return 0;

	debug("%s(%s, %s, %d, 0x%x)\n", __func__, name, value, op, flags);

	if (!strcmp(name, "enable_fec0")) {
		device = 0;
		i2c_addr = 0x4e;
		is_10g = eth0_10g;
	} else if (!strcmp(name, "enable_fec1")) {
		device = 1;
		i2c_addr = 0x47;
		is_10g = eth1_10g;
	} else {
		printf("%s(%s, %s, %d, 0x%x): invalid name\n",
		       __func__, name, value, op, flags);
		return -1;
	}

	mode = cvmx_helper_interface_get_mode(device);
	if (mode == CVMX_HELPER_INTERFACE_MODE_XFI) {
		printf("FEC for eth%d can only be set for 25G mode\n", device);
		return -1;
	}
	switch (op) {
	case env_op_create:
	case env_op_overwrite:
		switch (*value) {
		case '1':
		case 'y':
		case 'Y':
		case 't':
		case 'T':
			printf("Enabling forward error correction for eth%d\n",
			       device);
			uboot_garnet_set_fec(i2c_bus, i2c_addr, 0, 0,
					     true, eth0_10g);
			break;
		default:
			printf("Disabling forward error correction for eth%d\n",
			       device);
			uboot_garnet_set_fec(i2c_bus, i2c_addr, 0, 0,
					     false, eth1_10g);
			break;
		}
		break;
	case env_op_delete:
	default:
		printf("disabling forward error correction for eth%d\n",
		       device);
		uboot_garnet_set_fec(i2c_bus, i2c_addr, 0, 0, false, is_10g);
	}

	return 0;
}


U_BOOT_ENV_CALLBACK(enable_fec0, on_enablefec);
U_BOOT_ENV_CALLBACK(enable_fec1, on_enablefec);
