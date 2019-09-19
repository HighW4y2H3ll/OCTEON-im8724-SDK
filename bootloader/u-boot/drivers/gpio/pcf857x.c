/*
 * Copyright (C) 2014 Cavium, Inc.
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * Based loosly on the pca853x driver
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/* Driver for NXP's pcf857x and related GPIO expander chips */

#include <common.h>
#include <i2c.h>
#include <linux/string.h>
#include <malloc.h>
#include <pcf857x.h>

/* Default to an address that hopefully won't corrupt other i2c devices */
#ifndef CONFIG_SYS_I2C_PCF857X_ADDR
# define CONFIG_SYS_I2C_PCF857X_ADDR	(~0)
#endif
#ifndef CONFIG_SYS_I2C_PCF857X_BUS
# define CONFIG_SYS_I2C_PCF857X_BUS	(0)
#endif


/** Commands */
enum {
	PCF857X_CMD_DEVICE,
	PCF857X_CMD_OUTPUT,
	PCF857X_CMD_INPUT,
};


/**
 * Returns the values from the chip
 *
 * @param bus	I2C bus number
 * @param chip	I2C chip address
 *
 * @return	8 bits of GPIO data read from the pins or -1 on error
 */
int pcf857x_get_val(uint8_t bus, uint8_t chip)
{
	uint8_t reg;

	if (i2c_read(chip, 0, 0, &reg, 1))
		return -1;
	return reg;
}

/**
 * Sets values on the output of the chip
 * @param bus	I2C bus number
 * @param chip	I2C chip address
 * @param mask	Mask for the data
 * @param data	Bits to set
 *
 * @return -1 on error, 0 for success
 */
int pcf857x_set_val(uint8_t bus, uint8_t chip, uint mask, uint data)
{
	uint8_t reg = 0;
	i2c_set_bus_num(bus);
	if (i2c_read(chip, 0, 0, &reg, 1))
		return -1;
	reg &= ~mask;
	reg |= data;
	return i2c_write(chip, 0, 0, &reg, 1);
}

#ifdef CONFIG_CMD_PCF857X
cmd_tbl_t cmd_pcf857x[] = {
	U_BOOT_CMD_MKENT(device, 3, 0, (void *)PCF857X_CMD_DEVICE, "", ""),
	U_BOOT_CMD_MKENT(output, 4, 0, (void *)PCF857X_CMD_OUTPUT, "", ""),
	U_BOOT_CMD_MKENT(input, 3, 0, (void *)PCF857X_CMD_INPUT, "", ""),
};

/**
 * Display PCF857X data
 *
 * @param[in]	cmdtp	Command info
 * @param	flags	Command flags
 * @param	argc	Number of arguments
 * @param[in]	argv	Array of command line arguments
 *
 * @return Command response
 */
int do_pcf857x(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static uint8_t bus = CONFIG_SYS_I2C_PCF857X_BUS;
	static uint8_t chip = CONFIG_SYS_I2C_PCF857X_ADDR;
	int ret = CMD_RET_USAGE, val;
	ulong ul_arg2 = 0;
	ulong ul_arg3 = 0;
	const char *chip_str = argv[2];
	cmd_tbl_t *c;

	c = find_cmd_tbl(argv[1], cmd_pcf857x, ARRAY_SIZE(cmd_pcf857x));

	/* All commands but "device" require 'maxargs' argument */
	if (!c || !((argc == (c->maxargs)) ||
	    (((int)c->cmd == PCF857X_CMD_DEVICE) &&
	     (argc == (c->maxargs - 1)))))
		return CMD_RET_USAGE;

	/* arg2 used as chip number or pin number */
	if (argc > 2) {
		chip_str = strchr(argv[2], ':');
		if (chip_str) {
			char *dup_str = strdup(argv[2]);
			dup_str[chip_str - argv[2]] = '\0';
			bus = simple_strtoul(dup_str, NULL, 16);
			free(dup_str);
			chip_str++;
			ul_arg2 = simple_strtoul(chip_str, NULL, 16);
		} else {
			ul_arg2 = simple_strtoul(argv[2], NULL, 16);
		}
	}

	/* arg3 used as pin */
	if (argc > 3)
		ul_arg3 = simple_strtoul(argv[3], NULL, 16) & 0x1;

	switch ((int)c->cmd) {
	case PCF857X_CMD_DEVICE:
		if (argc == 3)
			chip = (uint8_t)ul_arg2;
		printf("Current device address: 0x%x:0x%x\n", bus, chip);
		ret = CMD_RET_SUCCESS;
		break;
	case PCF857X_CMD_INPUT:
		val = pcf857x_get_val(bus, chip) & (1 << ul_arg2);
		if (val < 0)
			ret = CMD_RET_FAILURE;
		else
			printf("chip 0x%02x, pin 0x%lx = %d\n",
			       chip, ul_arg2, val);
		break;
	case PCF857X_CMD_OUTPUT:
		ret = pcf857x_set_val(bus, chip, (1 << ul_arg2),
				      (ul_arg3 << ul_arg2));
		if (ret)
			ret = CMD_RET_FAILURE;
		break;
	}

	if (ret == CMD_RET_FAILURE)
		eprintf("Error talking to chip at 0x%x:0x%x\n", bus, chip);

	return ret;
}

U_BOOT_CMD(pcf757x, 5, 1, do_pcf857x,
	   "pcf857x gpio access",
	   "device [[bus:]dev]\n"
	   "       - show or set current device address\n"
	   "pcf857x output pin 0|1\n"
	   "       - set pin as output and drive low or high\n"
	   "pcf857x input pin\n"
	   "       - read pin value"
);

#endif	/* CONFIG_CMD_PCF857X */
