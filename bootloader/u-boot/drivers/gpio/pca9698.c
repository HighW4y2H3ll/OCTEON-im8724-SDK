/*
 * (C) Copyright 2011
 * Dirk Eibach,  Guntermann & Drunck GmbH, eibach@gdsys.de
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Driver for NXP's pca9698 40 bit I2C gpio expander
 */

#include <common.h>
#include <i2c.h>
#include <asm/errno.h>
#include <pca9698.h>
#include <command.h>
#include <malloc.h>

#ifndef CONFIG_SYS_I2C_PCA9698_ADDR
# define CONFIG_SYS_I2C_PCA9698_ADDR	(~0)
#endif
#ifndef CONFIG_SYS_i2C_PCA9698_BUS
# define CONFIG_SYS_I2C_PCA9698_BUS	(~0)
#endif

/*
 * The pca9698 registers
 */
#define PCA9698_REG_INPUT		0x00
#define PCA9698_REG_OUTPUT		0x08
#define PCA9698_REG_POLARITY		0x10
#define PCA9698_REG_CONFIG		0x18

#define PCA9698_BUFFER_SIZE		5
#define PCA9698_GPIO_COUNT		40

enum {
	PCA9698_CMD_INFO,
	PCA9698_CMD_DEVICE,
	PCA9698_CMD_OUTPUT,
	PCA9698_CMD_INPUT,
	PCA9698_CMD_INVERT,
};

static int pca9698_read40(u8 addr, u8 offset, u8 *buffer)
{
	u8 command = offset | 0x80;  /* autoincrement */

	return i2c_read(addr, command, 1, buffer, PCA9698_BUFFER_SIZE);
}

static int pca9698_write40(u8 addr, u8 offset, u8 *buffer)
{
	u8 command = offset | 0x80;  /* autoincrement */

	return i2c_write(addr, command, 1, buffer, PCA9698_BUFFER_SIZE);
}

static void pca9698_set_bit(unsigned gpio, u8 *buffer, unsigned value)
{
	unsigned byte = gpio / 8;
	unsigned bit = gpio % 8;

	if (value)
		buffer[byte] |= (1 << bit);
	else
		buffer[byte] &= ~(1 << bit);
}

int pca9698_request(unsigned gpio, const char *label)
{
	if (gpio >= PCA9698_GPIO_COUNT)
		return -EINVAL;

	return 0;
}

void pca9698_free(unsigned gpio)
{
}

int pca9698_direction_input(u8 addr, unsigned gpio)
{
	u8 data[PCA9698_BUFFER_SIZE];
	int res;

	res = pca9698_read40(addr, PCA9698_REG_CONFIG, data);
	if (res)
		return res;

	pca9698_set_bit(gpio, data, 1);

	return pca9698_write40(addr, PCA9698_REG_CONFIG, data);
}

int pca9698_direction_output(u8 addr, unsigned gpio, int value)
{
	u8 data[PCA9698_BUFFER_SIZE];
	int res;

	res = pca9698_set_value(addr, gpio, value);
	if (res)
		return res;

	res = pca9698_read40(addr, PCA9698_REG_CONFIG, data);
	if (res)
		return res;

	pca9698_set_bit(gpio, data, 0);

	return pca9698_write40(addr, PCA9698_REG_CONFIG, data);
}

int pca9698_get_value(u8 addr, unsigned gpio)
{
	unsigned config_byte = gpio / 8;
	unsigned config_bit = gpio % 8;
	unsigned value;
	u8 data[PCA9698_BUFFER_SIZE];
	int res;

	res = pca9698_read40(addr, PCA9698_REG_INPUT, data);
	if (res)
		return -1;

	value = data[config_byte] & (1 << config_bit);

	return !!value;
}

int pca9698_set_value(u8 addr, unsigned gpio, int value)
{
	u8 data[PCA9698_BUFFER_SIZE];
	int res;

	res = pca9698_read40(addr, PCA9698_REG_OUTPUT, data);
	if (res)
		return res;

	pca9698_set_bit(gpio, data, value);

	return pca9698_write40(addr, PCA9698_REG_OUTPUT, data);
}

int pca9698_set_pol(u8 addr, unsigned gpio, int invert)
{
	u8 data[PCA9698_BUFFER_SIZE];
	int res;

	res = pca9698_read40(addr, PCA9698_REG_POLARITY, data);
	if (res)
		return -1;
	if (invert)
		data[gpio / 8] |= (1 << (gpio % 8));
	else
		data[gpio / 8] &= ~(1 << (gpio % 8));
	res = pca9698_write40(addr, gpio, data);
	return res;
}

#ifdef CONFIG_CMD_PCA9698
# ifdef CONFIG_CMD_PCA9698_INFO
/**
 * Display PCA9698 information
 */
static int pca9698_info(uint8_t bus, uint8_t chip)
{
	int i;
	u8 data[PCA9698_BUFFER_SIZE];
	int msb = 39;

	printf("pca9698@ 0x%x:0x%x (40 pins):\n\n", bus, chip);
	puts("gpio pins: ");
	for (i = msb; i >= 0; i--)
		printf("%x", i >> 4);
	puts("\n           ");
	for (i = msb; i >= 0; i--)
		printf("%x", i % 16);
	putc('\n');
	for (i = 11 + 40; i >= 0; i--)
		putc('-');
	if (pca9698_read40(chip, PCA9698_REG_CONFIG, data) < 0)
		return -1;
	puts("\nconf:      ");
	for (i = msb; i >= 0; i--)
		putc(data[i / 8] & (1 << (i % 8)) ? 'i' : 'o');
	if (pca9698_read40(chip, PCA9698_REG_POLARITY, data))
		return -1;
	puts("\ninvert:    ");
	for (i = msb; i >= 0; i--)
		putc(data[i / 8] & (1 << (i % 8)) ? '1' : '0');
	if (pca9698_read40(chip, PCA9698_REG_INPUT, data))
		return -1;
	puts("\ninput:     ");
	for (i = msb; i >= 0; i--)
		putc(data[i / 8] & (1 << (i % 8)) ? '1' : '0');
	if (pca9698_read40(chip, PCA9698_REG_OUTPUT, data))
		return -1;
	puts("\noutput:    ");
	for (i = msb; i >= 0; i--)
		putc(data[i / 8] & (1 << (i % 8)) ? '1' : '0');
	putc('\n');
	return 0;
}
# endif

static cmd_tbl_t cmd_pca9698[] = {
	U_BOOT_CMD_MKENT(device, 3, 0, (void *)PCA9698_CMD_DEVICE, "", ""),
	U_BOOT_CMD_MKENT(output, 4, 0, (void *)PCA9698_CMD_OUTPUT, "", ""),
	U_BOOT_CMD_MKENT(input, 3, 0, (void *)PCA9698_CMD_INPUT, "", ""),
	U_BOOT_CMD_MKENT(invert, 4, 0, (void *)PCA9698_CMD_INVERT, "", ""),
# ifdef CONFIG_CMD_PCA953X_INFO
	U_BOOT_CMD_MKENT(info, 2, 0, (void *)PCA9698_CMD_INFO, "", ""),
# endif
};

static int do_pca9698(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static uint8_t chip = CONFIG_SYS_I2C_PCA9698_ADDR;
	static uint8_t bus = CONFIG_SYS_I2C_PCA9698_BUS;
	int ret = CMD_RET_USAGE;
	ulong ul_arg2 = 0;
	ulong ul_arg3 = 0;
	const char *chip_str = argv[2];
	cmd_tbl_t *c;
	int old_bus = i2c_get_bus_num();

	c = find_cmd_tbl(argv[1], cmd_pca9698, ARRAY_SIZE(cmd_pca9698));

	/* All commands but "device" require 'maxargs' arguments */
	if (!c || !((argc == (c->maxargs)) ||
	    (((int)c->cmd == PCA9698_CMD_DEVICE) &&
	      (argc == (c->maxargs - 1))))) {
		return CMD_RET_USAGE;
	}

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

	/* arg3 used as pin or invert value */
	if (argc > 3)
		ul_arg3 = simple_strtoul(argv[3], NULL, 16) & 0x1;

	i2c_set_bus_num(bus);
	switch ((int)c->cmd) {
# ifdef CONFIG_CMD_PCA9698_INFO
	case PCA9698_CMD_INFO:
		ret = pca9698_info(bus, chip);
		ret = ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
		break;
# endif
	case PCA9698_CMD_DEVICE:
		if (argc == 3)
			chip = (uint8_t)ul_arg2;
		printf("Current device address: 0x%x:0x%x\n", bus, chip);
		ret = CMD_RET_SUCCESS;
		break;
	case PCA9698_CMD_INPUT:
		ret = pca9698_direction_input(chip, ul_arg2);
		if (!ret) {
			ret = pca9698_get_value(chip, ul_arg2);
		}
		if (ret < 0) {
			ret = CMD_RET_FAILURE;
		} else {
			printf("chip 0x%02x, pin 0x%lx = %d\n", chip, ul_arg2,
			       ret);
			ret = CMD_RET_SUCCESS;
		}
		break;
	case PCA9698_CMD_OUTPUT:
		ret = pca9698_direction_output(chip, ul_arg2, ul_arg3);
		if (!ret)
			ret = pca9698_set_value(chip, ul_arg2, ul_arg3);
		ret = ret ? CMD_RET_FAILURE : CMD_RET_SUCCESS;
		break;
	case PCA9698_CMD_INVERT:
		if (pca9698_set_pol(chip, ul_arg2, ul_arg3))
			ret = CMD_RET_FAILURE;
		else
			ret = CMD_RET_SUCCESS;
	}

	if (ret == CMD_RET_FAILURE)
		eprintf("Error talking to chip at 0x%x:0x%x\n", bus, chip);

	i2c_set_bus_num(old_bus);

	return ret;
}

U_BOOT_CMD(
	pca9698,	5,	1,	do_pca9698,
	"pca9698 gpio access",
	"device [[bus:]dev]\n"
	"       - show or set current device address\n"
# ifdef CONFIG_CMD_PCA953X_INFO
	"pca9698 info\n"
	"       - display info for current chip\n"
# endif
	"pca9698 output pin 0|1\n"
	"       - set pin as output and drive low or high\n"
	"pca9698 invert pin 0|1\n"
	"       - disable/enable polarity inversion for reads\n"
	"pca9698 input pin\n"
	"       - set pin as input and read value"
);
#endif
