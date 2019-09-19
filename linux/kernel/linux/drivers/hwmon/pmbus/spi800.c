/*
 * Hardware monitoring driver for Sparkle 800
 *
 * Copyright (c) 2014 Interface Masters Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/i2c/pmbus.h>
#include "pmbus.h"

/* Sparkle 800W power supply Registers */

#define SPI800_PAGE					0x00
#define SPI800_OPERATION				0x01
#define SPI800_ON_OFF_CONFIG				0x02
#define SPI800_CLEAR_FAULTS				0x03

#define SPI800_SMBALERT_MASK				0x1B
#define SPI800_FAN_CONFIG_1_2				0x3A
#define SPI800_FAN_COMMAND_1				0x3B
#define SPI800_IOUT_OC_WARN_LIMIT			0x4A

#define SPI800_STATUS_WORD				0x79

#define SPI800_STATUS_VOUT				0x7A
#define SPI800_STATUS_OV_FAULT				(1 << 7)
#define SPI800_STATUS_UV_FAULT				(1 << 4)

#define SPI800_STATUS_IOUT				0x7B
#define SPI800_STATUS_OC_FAULT				(1 << 7)
#define SPI800_STATUS_OC_WARNING			(1 << 5)
#define SPI800_STATUS_POWER_LIMITING			(1 << 2)
#define SPI800_STATUS_POUT_OP_FAULT			(1 << 1)
#define SPI800_STATUS_POUT_OP_WARNING			(1 << 0)

#define SPI800_STATUS_INPUT				0x7C
#define SPI800_STATUS_VIN_OV_WARNING			(1 << 6)
#define SPI800_STATUS_VIN_UV_WARNING			(1 << 5)
#define SPI800_STATUS_VIN_UV_FAULT			(1 << 4)
#define SPI800_STATUS_UNIT_IS_OFF			(1 << 3)
#define SPI800_STATUS_IIN_OC_FAULT			(1 << 2)
#define SPI800_STATUS_IIN_OC_WARNING			(1 << 1)
#define SPI800_STATUS_PIN_OP_WARNING			(1 << 0)

#define SPI800_STATUS_TEMPERATURE			0x7D
#define SPI800_STATUS_OT_FAULT				(1 << 7)
#define SPI800_STATUS_OT_WARNING			(1 << 6)

#define SPI800_STATUS_CML				0x7E

#define SPI800_STATUS_FAN_1_2				0x81
#define SPI800_STATUS_FAN1_FAULT			(1 << 7)
#define SPI800_STATUS_FAN1_WARNING			(1 << 5)
#define SPI800_STATUS_FAN1_SPEED_OVERRIDDEN		(1 << 3)

#define SPI800_READ_VIN					0x88
#define SPI800_READ_IIN					0x89
#define SPI800_READ_VOUT				0x8B
#define SPI800_READ_IOUT				0x8C
#define SPI800_READ_TEMPERATURE_1			0x8D
#define SPI800_READ_TEMPERATURE_2			0x8E
#define SPI800_READ_FAN_SPEED_1				0x90
#define SPI800_READ_POUT				0x96
#define SPI800_READ_PIN					0x97

static int SPI800_read_word_data(struct i2c_client *client, int page, int reg)
{
	int ret;

	if (page)
		return -ENXIO;

	switch (reg)
	{
	case PMBUS_STATUS_WORD:
	case PMBUS_FAN_COMMAND_1:
	case PMBUS_IOUT_OC_WARN_LIMIT:
	case PMBUS_READ_VIN:
	case PMBUS_READ_IIN:
	case PMBUS_READ_VOUT:
	case PMBUS_READ_IOUT:
	case PMBUS_READ_TEMPERATURE_1:
	case PMBUS_READ_TEMPERATURE_2:
	case PMBUS_READ_FAN_SPEED_1:
	case PMBUS_READ_FAN_SPEED_2:
	case PMBUS_READ_POUT:
	case PMBUS_READ_PIN:
		ret = pmbus_read_word_data(client, 0, reg);
		break;
	case PMBUS_VIRT_RESET_VOUT_HISTORY:
	case PMBUS_VIRT_RESET_IOUT_HISTORY:
	case PMBUS_VIRT_RESET_TEMP_HISTORY:
	case PMBUS_VIRT_RESET_VIN_HISTORY:
	case PMBUS_VIRT_RESET_PIN_HISTORY:
		ret = 0;
		break;
	default:
		ret = -ENODATA;
		break;
	}
	return ret;
}

static int SPI800_write_word_data(struct i2c_client *client, int page, int reg, u16 word)
{
	return pmbus_write_word_data(client, 0, reg, 0);
}

static int SPI800_read_byte_data(struct i2c_client *client, int page, int reg)
{
	int ret = 0;
	int mfg_status;

	if (page > 0)
		return -ENXIO;

	switch (reg)
	{
	case PMBUS_STATUS_VOUT:
		mfg_status = pmbus_read_word_data(client, 0, SPI800_STATUS_VOUT);
		if (mfg_status < 0)
			return mfg_status;
		if (mfg_status & SPI800_STATUS_UV_FAULT)
			ret |= PB_VOLTAGE_UV_FAULT;
		if (mfg_status & SPI800_STATUS_OV_FAULT)
			ret |= PB_VOLTAGE_OV_FAULT;
		break;
	case PMBUS_STATUS_IOUT:
		mfg_status = pmbus_read_word_data(client, 0, SPI800_STATUS_IOUT);
		if (mfg_status < 0)
			return mfg_status;
		if (mfg_status & SPI800_STATUS_OC_FAULT)
			ret |= PB_IOUT_OC_FAULT;
		if (mfg_status & SPI800_STATUS_OC_WARNING)
			ret |= PB_IOUT_OC_WARNING;
		if (mfg_status & SPI800_STATUS_POWER_LIMITING)
			ret |= PB_POWER_LIMITING;
		if (mfg_status & SPI800_STATUS_POUT_OP_FAULT)
			ret |= PB_POUT_OP_FAULT;
		if (mfg_status & SPI800_STATUS_POUT_OP_WARNING)
			ret |= PB_POUT_OP_WARNING;
		break;
	case PMBUS_STATUS_TEMPERATURE:
		mfg_status = pmbus_read_word_data(client, 0, SPI800_STATUS_TEMPERATURE);
		if (mfg_status < 0)
			return mfg_status;
		if (mfg_status & SPI800_STATUS_OT_WARNING)
			ret |= PB_TEMP_OT_WARNING;
		if (mfg_status & SPI800_STATUS_OT_FAULT)
			ret |= PB_TEMP_OT_FAULT;
		break;
	case PMBUS_STATUS_INPUT:
		mfg_status = pmbus_read_word_data(client, 0, SPI800_STATUS_INPUT);
		if (mfg_status < 0)
			return mfg_status;
		if (mfg_status & SPI800_STATUS_VIN_OV_WARNING)
			ret |= PMBUS_VIN_OV_WARN_LIMIT;
		if (mfg_status & SPI800_STATUS_VIN_UV_WARNING)
			ret |= PMBUS_VIN_UV_WARN_LIMIT;
		if (mfg_status & SPI800_STATUS_VIN_UV_FAULT)
			ret |= PMBUS_VIN_UV_FAULT_LIMIT;
		if (mfg_status & SPI800_STATUS_IIN_OC_FAULT)
			ret |= PB_IIN_OC_FAULT;
		if (mfg_status & SPI800_STATUS_IIN_OC_WARNING)
			ret |= PB_IIN_OC_WARNING;
		if (mfg_status & SPI800_STATUS_PIN_OP_WARNING)
			ret |= PB_PIN_OP_WARNING;
		break;
	case PMBUS_STATUS_FAN_12:
		mfg_status = pmbus_read_word_data(client, 0, SPI800_STATUS_FAN_1_2);
		if (mfg_status < 0)
			return mfg_status;
		if (mfg_status & SPI800_STATUS_FAN1_FAULT)
			ret |= PB_FAN_FAN1_FAULT;
		if (mfg_status & SPI800_STATUS_FAN1_WARNING)
			ret |= PB_FAN_FAN1_WARNING;
		if (mfg_status & SPI800_STATUS_FAN1_SPEED_OVERRIDDEN)
			ret |= PB_FAN_FAN1_SPEED_OVERRIDE;
		break;
	default:
		ret = -ENODATA;
	}
	return ret;
}

static struct pmbus_driver_info SPI800_info = {
	.pages = 1,
	.format[PSC_VOLTAGE_IN] = linear,
	.format[PSC_VOLTAGE_OUT] = linear,
	.format[PSC_TEMPERATURE] = linear,
	.format[PSC_CURRENT_IN] = linear,
	.format[PSC_CURRENT_OUT] = linear,
	.format[PSC_POWER] = linear,
	.format[PSC_FAN] = linear,
	.func[0] = PMBUS_HAVE_VOUT | PMBUS_HAVE_IOUT | PMBUS_HAVE_TEMP
	| PMBUS_HAVE_VIN | PMBUS_HAVE_IIN | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT
	| PMBUS_HAVE_FAN12 | PMBUS_HAVE_TEMP2
	| PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_STATUS_IOUT | PMBUS_HAVE_STATUS_INPUT
	| PMBUS_HAVE_STATUS_TEMP | PMBUS_HAVE_STATUS_FAN12,
	.read_byte_data = SPI800_read_byte_data,
	.read_word_data = SPI800_read_word_data,
	.write_word_data = SPI800_write_word_data,
};

static int SPI800_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct pmbus_platform_data pdata;
	pdata.flags = PMBUS_SKIP_STATUS_CHECK;

	client->dev.platform_data = &pdata;

	return pmbus_do_probe(client, id, &SPI800_info);
}

static const struct i2c_device_id SPI800_id[] = {
	{"spi800", 1},
	{ }
};

MODULE_DEVICE_TABLE(i2c, SPI800_id);

/* This is the driver that will be inserted */
static struct i2c_driver SPI800_driver = {
	.driver = {
		.name = "spi800",
	},
	.probe = SPI800_probe,
	.remove = pmbus_do_remove,
	.id_table = SPI800_id,
};

static int __init SPI800_init(void)
{
	return i2c_add_driver(&SPI800_driver);
}

static void __exit SPI800_exit(void)
{
	i2c_del_driver(&SPI800_driver);
}

module_init(SPI800_init);
module_exit(SPI800_exit);

MODULE_AUTHOR("Wei Liu");
MODULE_DESCRIPTION("PMBus driver for Sparkle SPI800");
MODULE_LICENSE("GPL");
