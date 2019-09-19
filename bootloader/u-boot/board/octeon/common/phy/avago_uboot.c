/***********************license start************************************
 * Copyright (c) 2015 - 2016 Cavium Inc. (support@cavium.com).
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 ***********************license end**************************************/

/**
 * Doxygen File Header
 *
 * @file
 * @brief	Code to initialize Avago in U-Boot.
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include "avago.h"
#include "avago_avsp.h"
#include "avago_serdes.h"
#include "avago_serdes_dfe.h"
#include "avago_spico.h"
#include "avago_avsp.h"
#include "avago_sbus.h"
#include "avago_logging.h"
#include "avago_sbus_master_0704_heal.h"
#include "avago_serdes_0x2064_0241.h"
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-app-init.h>
#include <i2c.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef DEBUG
static int avago_debug_level = 6;
#else
static int avago_debug_level = 0;
#endif

extern bool avago_i2c_sbus_fn(struct avago_hdl *ah, uint32_t sbus_addr,
			      uint8_t reg_addr, uint8_t command,
			      uint32_t *sbus_data);
extern bool avago_i2c_sbus(struct avago_hdl *ah, uint32_t sbus_addr,
			   uint8_t reg_addr, uint8_t command,
			   uint32_t *sbus_data);

/**
 * Performs phase calibration on the host side for the specified chip and ring
 *
 * @param	ah	Avago avago_init_handle
 * @param	chip	chip number
 * @param	ring	ring number
 */
static void phase_cal_host_serdes(struct avago_hdl *ah, uint32_t chip,
				  uint32_t ring)
{
	uint32_t addr = 0, serdes = 0;
	int rc = 0;

	for (serdes = 6; serdes <=9; serdes++) {
		addr = avago_make_addr3 (chip, ring, serdes);
		rc = avago_spico_int_check(ah, __func__, __LINE__,
					   addr, 0xb, 0x1);
		if (!rc)
			printf("%s: SPICO interrupt %u at address 0x%x failed\n",
				__func__, 0xb, addr);
	}
}

/**
 * Allocates and initializes the Avago handle
 *
 * @param	i2c_bus		i2c bus number for the handle
 * @param	i2c_base_address	i2c address of Avago device
 * @param	chip		chip number for Avago device
 * @param	ring		ring number for Avago device
 *
 * @return	pointer to Avago handle or NULL if error
 */
static struct avago_hdl *avago_init_handle(int i2c_bus, int i2c_base_addr,
					   uint32_t chip, uint32_t ring)
{
	struct avago_hdl *ah;

	ah = avago_construct(i2c_bus, i2c_base_addr);
	if (!ah)
		return NULL;
	ah->debug = avago_debug_level;
	ah->sbus_fn = avago_i2c_sbus;
	ah->serdes_int_fn = avago_serdes_spico_int_sbus_fn;
	debug("%s: Set sbus_fn to %p\n", __func__, ah->sbus_fn);
	ah->chips = 1;
	ah->sbus_rings = 1;
	return ah;
}

/**
 * Sets the TX attenuation of a SerDes
 *
 * @param	ah	Avago handle
 * @param	chip	chip number
 * @param	ring	ring number
 * @param	serdes	Serdes to set
 * @param	tx_atten	TX attenuation to use for the host side
 */
static void uboot_avago_tx_atten_set_per_lane(struct avago_hdl *ah,
					      uint32_t chip, uint32_t ring,
					      uint32_t serdes,
					      uint32_t tx_atten)
{
	uint32_t addr;

	addr = avago_make_addr3(chip, ring, serdes);
	/* set TX atten to something */
	avago_spico_int(ah, addr, 0x15, (0x4000 + tx_atten));
}

/**
 * Sets the TX attenuation of a SerDes
 *
 * @param	ah	Avago handle
 * @param	chip	chip number
 * @param	ring	ring number
 * @param	tx_atten	TX attenuation to use
 */
static void uboot_avago_tx_atten_set(struct avago_hdl *ah, uint32_t chip,
				     uint32_t ring, uint32_t tx_atten)
{
	uint32_t serdes;

	for (serdes = 6; serdes <= 9; serdes++) {
		uboot_avago_tx_atten_set_per_lane(ah, chip, ring, serdes,
						  tx_atten);
		mdelay(50);
	}
}

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
int uboot_garnet_set_fec(int i2c_bus, int i2c_base_addr, uint32_t chip,
			 uint32_t ring, bool enable_fec, bool enable_10g)
{
	struct avago_hdl *ah;
	int rc;
	int old_i2c_bus = i2c_get_bus_num();

	ah = avago_init_handle(i2c_bus, i2c_base_addr, chip, ring);
	if (!ah)
		return -1;

	i2c_set_bus_num(i2c_bus);

	if (enable_10g) {
		/* Low byte is the multiplier for 161MHz clock */
		rc = avago_spico_int(ah, avago_make_addr3(chip, ring, 0xfd),
				     0x24, 0x0040);
	} else {
		rc = avago_spico_int(ah, avago_make_addr3(chip, ring, 0xfd),
				     0x24, enable_fec ? 0x40a0: 0x80a0);
	}
	free(ah);

	i2c_set_bus_num(old_i2c_bus);

	return rc;
}

int uboot_garnet_init(int i2c_bus, int i2c_base_addr, uint32_t chip,
		     uint32_t ring, bool enable_fec, uint32_t host_tx_atten,
		     struct avago_serdes_tx_eq *line_tx_eq, bool enable_10g)
{
	const int serdes_rom_size = ARRAY_SIZE(avago_serdes_2064_0241_rom);
	const bool squelch_tx = true;
	const bool run_adaptive = false;
	const enum avsp_supervisor_mode run_mode =
				AVSP_SUPERVISOR_MODE_TUNE_IF_LOCKED_SIGNAL;
	const char *device_name;
	const int electrical_idle_threshold = 0;
	const uint32_t *sbus_rom;
	struct avago_hdl *ah;
	int rc;
	int sbus_rom_size;
	bool reuse_ical = true;
	struct avsp_supervisor_config self_heal_config;
	bool disable_self_heal = getenv_yesno("disable_avago_heal") > 0;
	int old_i2c_speed;

	debug("%s(0x%x, 0x%x, 0x%x, %s)\n", __func__, i2c_bus, i2c_base_addr,
	      chip, enable_fec ? "enabled" : "disabled");

	avago_debug_level = getenv_ulong("avago_debug_level", 10, 0);

	if (avago_debug_level)
		printf("%s(i2c bus: %d, i2c base addr: 0x%x, chip: %d, ring: 0x%x, FEC: %sabled): debug level: %d\n",
		       __func__, i2c_bus, i2c_base_addr, chip, ring,
		       enable_fec ? "en" : "dis", avago_debug_level);

	i2c_set_bus_num(i2c_bus);
	old_i2c_speed = i2c_get_bus_speed();
	i2c_set_bus_speed(400000);

	ah = avago_init_handle(i2c_bus, i2c_base_addr, chip, ring);
	if (!ah)
		return -1;

	debug("%s: Initializing IP\n", __func__);
	avago_init_ip_info(ah, true);	/* Reset chip */
	mdelay(100);
	debug("%s: Getting chip name\n", __func__);
	device_name = avago_get_chip_name(ah, avago_make_addr3(chip, ring, 0));
	if (ah->return_code < 0) {
		printf("%s: Error getting device info and name\n", __func__);
		return -1;
	}
	debug("%s: device name: %s\n", __func__, device_name);

	sbus_rom_size = ARRAY_SIZE(avago_sbus_master_0704_heal_rom);
	sbus_rom = avago_sbus_master_0704_heal_rom;
	debug("Loading self-healing SBus ROM, size: %u bytes\n",
	      sbus_rom_size);

	rc = avsp_upload_firmware_u32(ah, chip, false,
				      serdes_rom_size,
				      avago_serdes_2064_0241_rom,
				      sbus_rom_size, sbus_rom);
	if (rc) {
		printf("%s: Error uploading firmware\n", __func__);
		return -1;
	}
	debug("Firmware upload complete.\n");

	if (!disable_self_heal) {
		debug("Self-healing enabled, initializing supervisor config\n");
		reuse_ical = false;
		avsp_supervisor_config_init(ah, run_mode, squelch_tx, reuse_ical,
					run_adaptive, &self_heal_config);
		/* Turn on self-healing */
		debug("Set self healing config\n");
		avsp_supervisor_set_config(ah, 0, &self_heal_config);
		debug("Setting signal OK electrical idle threshold to %d\n",
		      electrical_idle_threshold);
		avsp_supervisor_set_signal_ok(ah, 0, 0x2f,
					      electrical_idle_threshold);
	}

	debug("%s: Configuring for %dG operation\n", __func__,
	      enable_10g ? 10 : 25);

	if (enable_10g) {
		/* Low byte is the multiplier for 161MHz clock */
		rc = avago_spico_int(ah, avago_make_addr3(chip, ring, 0xfd),
				     0x24, 0x0040);
	} else {
		rc = avago_spico_int(ah, avago_make_addr3(chip, ring, 0xfd),
				     0x24, enable_fec ? 0x40a0: 0x80a0);
	}

	if (!rc)
		printf("%s: Error %sabling FEC\n", __func__,
		       enable_fec ? "en" : "dis");
	else
		debug("FEC is on\n");

	if (!disable_self_heal) {
		/* Change self-healing mode from TUNE_IF_LOCKED_SIGNAL to
		 * TUNE_IF_SIGNAL.  The former waits for both frequency lock
		 * and signal amplitude, the latter just checks signal
		 * amplitude.
		 */
		avago_spico_int(ah, avago_make_addr3(chip, ring, 0xfd),
				0x26, 0x6206);
	}

	mdelay(500);
	debug("Host tx attenuation is set to %d\n", host_tx_atten);
	uboot_avago_tx_atten_set(ah, chip, ring, host_tx_atten);
	rc = avago_serdes_set_tx_eq(ah, avago_make_addr3(chip, ring, 5),
				    line_tx_eq);
	/* uboot_avago_line_tx_atten(ah, chip, ring, line_tx_atten);*/
	i2c_set_bus_speed(old_i2c_speed);
	if (ah)
		free(ah);

	return rc ? -1 : 0;
}

/**
 * Perform phase calibration
 *
 * @param	i2c_bus		I2C bus number
 * @param	i2c_base_addr	I2C base address
 * @param	chip		Chip number
 * @param	ring		Ring number
 */
void avago_serdes_init_tx_phase_cal(int i2c_bus, int i2c_base_addr,
                        uint32_t chip, uint32_t ring)
{
	struct avago_hdl *ah;
	int old_i2c_speed;
	static int retry1 = 0, retry2 = 0;

	i2c_set_bus_num(i2c_bus);
	old_i2c_speed = i2c_get_bus_speed();
	i2c_set_bus_speed(400000);

	ah = avago_init_handle(i2c_bus, i2c_base_addr, chip, ring);
	if (!ah)
		return;

	avago_init_ip_info(ah, false);

	if (i2c_base_addr == 0x4e)
		retry1++;
	else
		retry2++;


	avago_get_chip_name(ah, avago_make_addr3(chip, ring, 0));
	if (ah->return_code < 0) {
		printf("%s: Error getting device info and name\n", __func__);
		return;
	}

	printf("Tx Phase Cal %d %d\n", retry1, retry2);
	phase_cal_host_serdes(ah, chip, ring);

	i2c_set_bus_speed(old_i2c_speed);

	if (ah)
		free(ah);
}

static int do_aapl_init(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	struct avago_serdes_tx_eq line_tx_eq;
	int chip = 0;
	int ring = 0;
	bool enable_fec = true;

	if (argc == 4) {
		chip = simple_strtol(argv[1], NULL, 0);
		argc--;
		argv++;
	}
	if (argc == 3) {
		ring = simple_strtol(argv[1], NULL, 0);
		argc--;
		argv++;
	}

	if (argc == 2)
		enable_fec = !!simple_strtol(argv[1], NULL, 0);

	memset(&line_tx_eq, 0, sizeof(line_tx_eq));
	uboot_garnet_init(0, 0x4e, chip, ring, enable_fec, 0x18, &line_tx_eq,
			  false);

	return CMD_RET_SUCCESS;
}

static int read_aapl_addr(const char *str, uint32_t *chip, uint32_t *ring,
			  uint32_t *sbus_addr)
{
	char *ptr = NULL;

	*chip = *ring = 0;

	if (strstr(str, ":") == NULL) { /* No chip or ring */
		*chip = *ring = 0;
		*sbus_addr = simple_strtoul(str, &ptr, 0);
		if (*sbus_addr & ~0xff) {
			*sbus_addr = 0;
			return -1;
		}
	} else {	/* At least chip or ring */
		*sbus_addr = simple_strtol(str, &ptr, 16); /* Read as hex */
	}
	if (str == ptr && *ptr == '*') {
		ptr++;
		*sbus_addr = AVAGO_ADDR_BROADCAST;
	}
	if (*ptr == ':') {
		/* [chip:]ring:sbus */
		ptr++;
		*ring = *sbus_addr;
		if (*ptr == '*') {
			ptr++;
			*sbus_addr = AVAGO_ADDR_BROADCAST;
		} else {
			*sbus_addr = simple_strtol(ptr, &ptr, 16);
		}
		if (*ptr == ':') {
			/* chip:ring:sbus */
			ptr++;
			*chip = *ring;
			*ring = *sbus_addr;
			if (*ptr == '*') {
				ptr++;
				*sbus_addr = AVAGO_ADDR_BROADCAST;
			} else {
				*sbus_addr = simple_strtol(ptr, &ptr, 16);
			}
		}
	}

	/* Don't bother with lane for now */
	return 0;
}

static int do_serdes_int(cmd_tbl_t *cmdtp, int flags, int argc,
			 char * const argv[])
{
	uint32_t chip, ring, sbus_addr, interrupt, data, ret_data;
	struct avago_hdl *ah;
	const char *device_name;

	if (argc != 4)
		return CMD_RET_USAGE;

	read_aapl_addr(argv[1], &chip, &ring, &sbus_addr);
	interrupt = simple_strtoul(argv[2], NULL, 0);
	data = simple_strtoul(argv[3], NULL, 0);
	ah = avago_init_handle(0, 0x4e, chip, ring);
	avago_init_ip_info(ah, false);
	device_name = avago_get_chip_name(ah, avago_make_addr3(chip, ring, 0));
	debug("Sending interrupt 0x%x with data 0x%x to %s at %x:%x:%x\n",
	      interrupt, data, device_name, chip, ring, sbus_addr);

	ret_data = avago_spico_int(ah, avago_make_addr3(chip, ring, sbus_addr),
				   interrupt, data);
	debug("Interrupt 0x%x returned 0x%x\n", interrupt, ret_data);
	if (ah)
		free(ah);
	return CMD_RET_SUCCESS;
}

static int do_serdes_tx_invert(cmd_tbl_t *cmdtp, int flags, int argc,
			       char * const argv[])
{
	uint32_t chip, ring, sbus_addr;
	struct avago_hdl *ah;
	const char *device_name;
	bool invert;

	if (argc != 3)
		return CMD_RET_USAGE;

	read_aapl_addr(argv[1], &chip, &ring, &sbus_addr);
	invert = !!simple_strtoul(argv[2], NULL, 0);
	ah = avago_init_handle(0, 0x4e, chip, ring);
	avago_init_ip_info(ah, false);
	device_name = avago_get_chip_name(ah, avago_make_addr3(chip, ring,
							       sbus_addr));
	debug("%sabling tx inversion for SerDes on %s at %x:%x:%x\n",
	      invert ? "Ena" : "Dis", device_name, chip, ring,  sbus_addr);

	avago_serdes_set_tx_invert(ah, avago_make_addr3(chip, ring, sbus_addr),
				   invert);
	if (ah)
		free(ah);

	return CMD_RET_SUCCESS;
}

static int do_sbus_write(cmd_tbl_t *cmdtp, int flags, int argc,
			 char * const argv[])
{
	return CMD_RET_SUCCESS;
}

static int do_sbus_read(cmd_tbl_t *cmdtp, int flags, int argc,
			char * const argv[])
{
	return CMD_RET_SUCCESS;
}

static int do_aapl_info(cmd_tbl_t *cmdtp, int flags, int argc,
			char * const argv[])
{
	uint32_t chip, ring, sbus_addr;
	struct avago_hdl *ah;
	struct avago_state_table_options table_options;
	struct avago_addr addr_struct;

	if (argc != 2)
		return CMD_RET_USAGE;

	memset(&table_options, 0, sizeof(table_options));
	read_aapl_addr(argv[1], &chip, &ring, &sbus_addr);
	ah = avago_init_handle(0, 0x4e, chip, ring);
	avago_init_ip_info(ah, false);
	avago_addr_init(&addr_struct);

	avago_addr_to_struct(avago_make_addr3(chip, ring, sbus_addr),
			     &addr_struct);

	avago_device_info_options(ah, &addr_struct, AVAGO_UNKNOWN_IP,
				  &table_options);
	if (ah)
		free(ah);

	return CMD_RET_SUCCESS;

}

static cmd_tbl_t cmd_aapl_sub[] = {
	U_BOOT_CMD_MKENT(info, 1, 1, do_aapl_info, "Print device information", ""),
	U_BOOT_CMD_MKENT(init, 3, 1, do_aapl_init,
			 "Initializes the Avago device", ""),
#if 0
	U_BOOT_CMD_MKENT(dev_reset, CONFIG_SYS_MAXARGS, 1, do_dev_reset, "", ""),
#endif
	U_BOOT_CMD_MKENT(serdes_int, 4, 1, do_serdes_int,
			 "Sends an interrupt to the SerDes processor", ""),
	U_BOOT_CMD_MKENT(serdes_tx_invert, 3, 0, do_serdes_tx_invert,
			 "Sets inversion for the TX SerDes", ""),
	U_BOOT_CMD_MKENT(sbus_write, 4, 1, do_sbus_write, "", ""),
	U_BOOT_CMD_MKENT(sbus_read, 3, 1, do_sbus_read, "", "")
};

static int do_aapl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t *cp;

	if (argc < 2)
		return CMD_RET_USAGE;

	/* Drop initial "aapl" arg */
	argc--;
	argv++;

	cp = find_cmd_tbl(argv[0], cmd_aapl_sub, ARRAY_SIZE(cmd_aapl_sub));

	if (cp)
		return cp->cmd(cmdtp, flag, argc, argv);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(aapl, CONFIG_SYS_MAXARGS, 1, do_aapl, "Avago commands",
	   "aapl info [[chip:]ring:]serdes (ff for all)] - Displays device information\n"
	   "aapl init [[chip:]ring] [enable fec] - Initializes the Avago device\n"
#if 0
	   "aapl dev_reset - Resets the device\n"
#endif
	   "aapl serdes_int [[[chip:]ring:]serdes] [interrupt] [data] - Sends an interrurupt\n"
	   "aapl serdes_tx_invert [[[chip:]ring:]serdes] [1=invert,0=no invert] - Invert TX SerDes\n"
	   "aapl sbus_write [[[chip:]ring:]sbus addr] [address] [data] - Write to SBus memory address\n"
	   "aapl sbus_read [[[chip:]ring:]sbus addr] [address] - Read from SBus memory address\n"
);
