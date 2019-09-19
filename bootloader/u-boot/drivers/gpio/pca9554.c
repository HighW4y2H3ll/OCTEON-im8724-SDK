/*
 * Copyright 2008 Extreme Engineering Solutions, Inc.
 *
 * Copyright 2015 Cavium, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Driver for NXP's pca9554 16 bit I2C gpio expander
 */

#include <common.h>
#include <i2c.h>
#include <malloc.h>
#include <linux/string.h>
#include <pca9554.h>

/* Default to an address that hopefully won't corrupt other i2c devices */
#ifndef CONFIG_SYS_I2C_PCA9554_ADDR
# define CONFIG_SYS_I2C_PCA9554_ADDR	(~0)
#endif

#ifndef CONFIG_SYS_I2C_PCA9554_BUS
# define CONFIG_SYS_I2C_PCA9554_BUS	0
#endif

enum {
	PCA9554_CMD_INFO,
	PCA9554_CMD_DEVICE,
	PCA9554_CMD_OUTPUT,
	PCA9554_CMD_INPUT,
	PCA9554_CMD_INVERT,
};


static int pca9554_ngpio(uint8_t bus, uint8_t chip)
{
	return 8;
}

/*
 * Modify masked bits in register
 */
static int pca9554_reg_write(uint8_t bus, uint8_t chip, uint addr, uint mask,
			     uint data)
{
	uint8_t valb;

	if (i2c_set_bus_num(bus))
		return -1;

	if (i2c_read(chip, addr, 1, &valb, 1))
		return -1;

	valb &= ~mask;
	valb |= data;

	return i2c_write(chip, addr, 1, &valb, 1);
}

static int pca9554_reg_read(uint8_t bus, uint8_t chip, uint addr, uint *data)
{
	uint8_t valb;

	if (i2c_set_bus_num(bus))
		return -1;

	if (i2c_read(chip, addr, 1, &valb, 1))
		return -1;
	*data = (int)valb;

	return 0;
}

/*
 * Set output value of IO pins in 'mask' to corresponding value in 'data'
 * 0 = low, 1 = high
 */
int pca9554_set_val(uint8_t bus, uint8_t chip, uint mask, uint data)
{
	return pca9554_reg_write(bus, chip, PCA9554_OUT0, (mask >> 8) & 0xff,
				 (data >> 8) & 0xff);
}

/*
 * Set read polarity of IO pins in 'mask' to corresponding value in 'data'
 * 0 = read pin value, 1 = read inverted pin value
 */
int pca9554_set_pol(uint8_t bus, uint8_t chip, uint mask, uint data)
{
	return pca9554_reg_write(bus, chip, PCA9554_POL0, mask & 0xff,
				 data & 0xff);
}

/*
 * Set direction of IO pins in 'mask' to corresponding value in 'data'
 * 0 = output, 1 = input
 */
int pca9554_set_dir(uint8_t bus, uint8_t chip, uint mask, uint data)
{
	return pca9554_reg_write(bus, chip, PCA9554_CONF0, mask & 0xff,
				 data & 0xff);
}

/*
 * Read current logic level of all IO pins
 */
int pca9554_get_val(uint8_t bus, uint8_t chip)
{
	uint val;
	uint data;

	if (pca9554_reg_read(bus, chip, PCA9554_IN0, &val) < 0)
		return -1;
	data = val;

	return (int)data;
}

#ifdef CONFIG_CMD_PCA9554
#ifdef CONFIG_CMD_PCA9554_INFO
/*
 * Display pca9554 information
 */
static int pca9554_info(uint8_t bus, uint8_t chip)
{
	int i;
	uint data;
	int nr_gpio = pca9554_ngpio(bus, chip);
	int msb = nr_gpio - 1;

	printf("pca9554@ 0x%x:0x%x (%d pins):\n\n", bus, chip, nr_gpio);
	printf("gpio pins: ");
	for (i = msb; i >= 0; i--)
		printf("%x", i);
	printf("\n");
	for (i = 11 + nr_gpio; i > 0; i--)
		printf("-");
	printf("\n");

	printf("conf:      ");
	if (pca9554_reg_read(bus, chip, PCA9554_CONF0, &data) < 0)
		return -1;
	for (i = 7; i>= 0; i--)
		printf("%c", data & (1 << i) ? 'i' : 'o');
	printf("\n");

	printf("invert:    ");
	if (pca9554_reg_read(bus, chip, PCA9554_POL0, &data) < 0)
		return -1;
	for (i = 7; i >= 0; i--)
		printf("%c", data & (1 << i) ? '1' : '0');
	printf("\n");

	printf("input:     ");
	if (pca9554_reg_read(bus, chip, PCA9554_IN0, &data) < 0)
		return -1;
	for (i = 7; i >= 0; i--)
		printf("%c", data & (1 << i) ? '1' : '0');
	printf("\n");

	printf("output:    ");
	if (pca9554_reg_read(bus, chip, PCA9554_OUT0, &data) < 0)
		return -1;
	for (i = 7; i >= 0; i--)
		printf("%c", data & (1 << i) ? '1' : '0');
	printf("\n");

	return 0;
}
#endif /* CONFIG_CMD_PCA9554_INFO */

cmd_tbl_t cmd_pca9554[] = {
	U_BOOT_CMD_MKENT(device, 3, 0, (void *)PCA9554_CMD_DEVICE, "", ""),
	U_BOOT_CMD_MKENT(output, 4, 0, (void *)PCA9554_CMD_OUTPUT, "", ""),
	U_BOOT_CMD_MKENT(input, 3, 0, (void *)PCA9554_CMD_INPUT, "", ""),
	U_BOOT_CMD_MKENT(invert, 4, 0, (void *)PCA9554_CMD_INVERT, "", ""),
#ifdef CONFIG_CMD_PCA9554_INFO
	U_BOOT_CMD_MKENT(info, 2, 0, (void *)PCA9554_CMD_INFO, "", ""),
#endif
};

int do_pca9554(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static uint8_t chip = CONFIG_SYS_I2C_PCA9554_ADDR;
	static uint8_t bus = CONFIG_SYS_I2C_PCA9554_BUS;
	int ret = CMD_RET_USAGE, val;
	ulong ul_arg2 = 0;
	ulong ul_arg3 = 0;
	const char *chip_str = argv[2];
	cmd_tbl_t *c;

	c = find_cmd_tbl(argv[1], cmd_pca9554, ARRAY_SIZE(cmd_pca9554));

	/* All commands but "device" require 'maxargs' arguments */
	if (!c || !((argc == (c->maxargs)) ||
		(((int)c->cmd == PCA9554_CMD_DEVICE) &&
		 (argc == (c->maxargs - 1))))) {
		return CMD_RET_USAGE;
	}

	/* arg2 used as chip number or pin number*/
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

	/* arg3 used as pin or invert value */
	if (argc > 3)
		ul_arg3 = simple_strtoul(argv[3], NULL, 16) & 0x1;

	switch ((int)c->cmd) {
#ifdef CONFIG_CMD_PCA9554_INFO
	case PCA9554_CMD_INFO:
		ret = pca9554_info(bus, chip);
		if (ret)
			ret = CMD_RET_FAILURE;
		break;
#endif

	case PCA9554_CMD_DEVICE:
		if (argc == 3)
			chip = (uint8_t)ul_arg2;
		printf("Current device address: 0x%x:0x%x\n", bus, chip);
		ret = CMD_RET_SUCCESS;
		break;

	case PCA9554_CMD_INPUT:
		ret = pca9554_set_dir(bus, chip, (1 << ul_arg2),
				PCA9554_DIR_IN << ul_arg2);
		val = (pca9554_get_val(bus, chip) & (1 << ul_arg2)) != 0;

		if (ret)
			ret = CMD_RET_FAILURE;
		else
			printf("chip 0x%02x, pin 0x%lx = %d\n", chip, ul_arg2,
			       val);
		break;

	case PCA9554_CMD_OUTPUT:
		ret = pca9554_set_dir(bus, chip, (1 << ul_arg2),
				(PCA9554_DIR_OUT << ul_arg2));
		if (!ret)
			ret = pca9554_set_val(bus, chip, (1 << ul_arg2),
					      (ul_arg3 << ul_arg2));
		if (ret)
			ret = CMD_RET_FAILURE;
		break;

	case PCA9554_CMD_INVERT:
		ret = pca9554_set_pol(bus, chip, (1 << ul_arg2),
				      (ul_arg3 << ul_arg2));
		if (ret)
			ret = CMD_RET_FAILURE;
		break;
	}

	if (ret == CMD_RET_FAILURE)
		eprintf("Error talking to chip at 0x%x:0x%x\n", bus, chip);

	return ret;
}

U_BOOT_CMD(
	pca9554,	5,	1,	do_pca9554,
	"pca9554 gpio access",
	"device [[bus:]dev]\n"
	"	- show or set current device address\n"
#ifdef CONFIG_CMD_PCA9554_INFO
	"pca9554 info\n"
	"	- display info for current chip\n"
#endif
	"pca9554 output pin 0|1\n"
	"	- set pin as output and drive low or high\n"
	"pca9554 invert pin 0|1\n"
	"	- disable/enable polarity inversion for reads\n"
	"pca9554 input pin\n"
	"	- set pin as input and read value"
);

#endif /* CONFIG_CMD_PCA9554 */
