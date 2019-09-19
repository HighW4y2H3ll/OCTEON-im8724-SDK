/*
 * (C) Copyright 2007-2008
 * Dirk Eibach,  Guntermann & Drunck GmbH, eibach@gdsys.de
 * based on lm75.c by Bill Hunter
 *
 * (C) Copyright 2011
 * Aaron Williams, Cavium Inc., Inc. <aaron.williams@caviumnetworks.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

/*
 * TI TMP421, TMP422 and TMP423 Temperature Sensor
 *
 * The datasheet can be found at:
 * http://focus.ti.com/lit/ds/sbos398b/sbos398b.pdf
 *
 * The TMP421 and TMP422 devices can use I2C addresses 0x4C-0x4F.  The TMP423A
 * is at address 0x3C and the TMP423B is at 0x3D.
 */

/* Sensors are defined as follows:
 * 0 = local sensor
 * 1 = remote sensor 1 (TMP421, TMP422 and TMP423)
 * 2 = remote sensor 2 (TMP422 and TMP423)
 * 3 = remote sensor 3 (TMP423)
 */

#include <common.h>
#include <i2c.h>
#include <dtt.h>

#ifndef CONFIG_SYS_DTT_BUS_NUM
# define CONFIG_SYS_DTT_BUS_NUM	0
#endif

#define DTT_I2C_DEV_CODE	0x4C	/* TI TMP42X device */

/* Register offsets */
#define DTT_READ_TEMP_LOC_MSB	0x00
#define DTT_READ_TEMP_RMT1_MSB	0x01
#define DTT_READ_TEMP_RMT2_MSB	0x02
#define DTT_READ_TEMP_RMT3_MSB	0x03
#define DTT_READ_STATUS		0x08
#define DTT_CONFIG1		0x09
#define DTT_CONFIG2		0x0A
#define DTT_CONVERSION_RATE	0x0B
#define DTT_ONE_SHOT		0x0F
#define DTT_READ_TEMP_LOC_LSB	0x10
#define DTT_READ_TEMP_RMT1_LSB	0x11
#define DTT_READ_TEMP_RMT2_LSB	0x12
#define DTT_READ_TEMP_RMT3_LSB	0x13
#define DTT_N_CORRECTION1	0x21
#define DTT_N_CORRECTION2	0x22
#define DTT_N_CORRECTION3	0x23
#define DTT_SOFTWARE_RESET	0xFC
#define DTT_MANUFACTURER_ID	0xFE	/* 0x55 */
#define DTT_DEVICE_ID		0xFF	/* 0x21, 0x22 or 0x23 */

/* Manufacturer and device IDs */
#define DTT_TMP42X_MANUFACTURER	0x55
#define DTT_DEVICE_ID_TMP421	0x21
#define DTT_DEVICE_ID_TMP422	0x22
#define DTT_DEVICE_ID_TMP423	0x23

#ifndef CONFIG_SYS_I2C_DTT_ADDR
#define CONFIG_SYS_I2C_DTT_ADDR	0x4c
#endif

static int num_remote_sensors = 0;

/* DTT sensor
 *
 * - i2c addr 0x4c
 * - range -55C to +127C
 * - Remote channel 1 enabled
 * - Remote channel 2 disabled
 * - Remote channel 3 disabled
 * - Local channel enabled
 * - Resistance correction enabled
 * - conversion rate 0x02 = 0.25 conversions/second
 * - n factor channel 1
 * - n factor channel 2
 * - n factor channel 3
 */
static struct dtt_sensor_config {
	u8	range;		/* 0 = -55C to +127C, 1 = -55C to +150C */
	u8	en_remote1;	/* 1 = enabled, 0 = disabled 		*/
	u8	en_remote2;	/* 1 = enabled, 0 = disabled (TMP422,423) */
	u8	en_remote3;	/* 1 = enabled, 0 = disabled (TMP423)	*/
	u8	en_local;	/* 1 = enabled, 0 = disabled 		*/
	u8	en_rc;		/* Resistance correction enable 	*/
	u8	cr;		/* Conversion rate
				 *	0 = 0.0625/second
				 *	1 = 0.125/second
				 *	2 = 0.25/second
				 *	3 = 0.5/second
				 *	4 = 1/second
				 *	5 = 2/second
				 *	6 = 4/second
				 *	7 = 8/second
				 */
	int	n_factor_ch_1;	/* See table 8 in specification, default 0 */
	int	n_factor_ch_2;
	int	n_factor_ch_3;
} dtt_sensor_config
#ifdef CONFIG_SYS_DTT_TMP42X
	= CONFIG_SYS_DTT_TMP42X;
#else
	/* Default to all sensors enabled, 4 seconds between updates */
	= { 0, 1, 1, 1, 1, 1, 7, 0, 0, 0 };
#endif

/*
 * Device code
 */

int dtt_read(int sensor, int reg)
{
	int dlen;
	uchar data[2];

	/*
	 * Calculate sensor address and register.
	 */
	sensor = CONFIG_SYS_I2C_DTT_ADDR; /* address of TMP42X is adjustable */

	dlen = 1;

	/*
	 * Now try to read the register.
	 */
	if (i2c_read(sensor, reg, 1, data, dlen) != 0)
		return -1;

	debug("DTT: read address 0x%x returned 0x%x\n", reg, data[0]);

	return (int)data[0];
}				/* dtt_read() */

int dtt_write(int sensor, int reg, int val)
{
	int dlen;
	uchar data[2];

	/*
	 * Calculate sensor address and register.
	 */
	sensor = CONFIG_SYS_I2C_DTT_ADDR; /* address of TMP42X is adjustable */

	dlen = 1;
	data[0] = (uchar)(val & 0xff);

	debug("DTT: wrote 0x%x to address 0x%x\n", val, reg);
	/*
	 * Write value to register.
	 */
	if (i2c_write(sensor, reg, 1, data, dlen) != 0)
		return 1;

	return 0;
}				/* dtt_write() */

int dtt_init_one(int sensor)
{
	u8 manufacturer;
	u8 device;

	sensor = DTT_I2C_DEV_CODE + sensor; /* calculate address of tmp42x */

	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);
	manufacturer = dtt_read(sensor, DTT_MANUFACTURER_ID);
	device = dtt_read(sensor, DTT_DEVICE_ID);

	if (manufacturer != DTT_TMP42X_MANUFACTURER) {
		printf("DTT: Unknown manufacturer id 0x%x\n", manufacturer);
		return -1;
	}

	switch (device) {
	case DTT_DEVICE_ID_TMP421:
		num_remote_sensors = 1;
		dtt_sensor_config.en_remote2 = 0;
		dtt_sensor_config.en_remote3 = 0;
		break;
	case DTT_DEVICE_ID_TMP422:
		num_remote_sensors = 2;
		dtt_sensor_config.en_remote3 = 0;
		break;
	case DTT_DEVICE_ID_TMP423:
		num_remote_sensors = 3;
		break;
	default:
		printf("DTT: Unrecognized manufacturer 0x%x, device 0x%x\n",
			manufacturer, device);
		return -1;
	}

	dtt_write(sensor, DTT_SOFTWARE_RESET, 1);

	dtt_write(sensor, DTT_CONFIG1, dtt_sensor_config.range << 2);

	dtt_write(sensor, DTT_CONFIG2,
		  (dtt_sensor_config.en_rc << 2) 	|
		  (dtt_sensor_config.en_local << 3) 	|
		  (dtt_sensor_config.en_remote1 << 4) 	|
		  (dtt_sensor_config.en_remote2 << 5)	|
		  (dtt_sensor_config.en_remote3 << 6));

	dtt_write(sensor, DTT_CONVERSION_RATE, dtt_sensor_config.cr);

	dtt_write(sensor, DTT_N_CORRECTION1, dtt_sensor_config.n_factor_ch_1);

	if ((device == DTT_DEVICE_ID_TMP422) ||
	    (device == DTT_DEVICE_ID_TMP423))
		dtt_write(sensor, DTT_N_CORRECTION2,
			  dtt_sensor_config.n_factor_ch_2);

	if (device == DTT_DEVICE_ID_TMP423)
		dtt_write(sensor, DTT_N_CORRECTION3,
			  dtt_sensor_config.n_factor_ch_3);

	return 0;
}

int dtt_get_temp(int sensor)
{
	s16 temp;
	if (sensor > num_remote_sensors) {
		printf("Sensor %d is out of range of %d..%d\n",
		       sensor, 0, num_remote_sensors);
		return 0;
	}
	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);

	switch (sensor) {
	case 0:
		if (!dtt_sensor_config.en_local) {
			printf("DTT:  Local sensor 0 not enabled!\n");
			return 0;
		}
		temp = (dtt_read(0, DTT_READ_TEMP_LOC_MSB) << 8)
			| dtt_read(0, DTT_READ_TEMP_LOC_LSB);
		break;
	case 1:
		if (!dtt_sensor_config.en_remote1) {
			printf("DTT:  Remote sensor 1 not enabled!\n");
			return 0;
		}
		temp = (dtt_read(0, DTT_READ_TEMP_RMT1_MSB) << 8)
			| dtt_read(0, DTT_READ_TEMP_RMT1_LSB);
		break;
	case 2:
		if (!dtt_sensor_config.en_remote2) {
			printf("DTT:  Remote sensor 2 not enabled!\n");
			return 0;
		}
		temp = (dtt_read(0, DTT_READ_TEMP_RMT2_MSB) << 8)
			| dtt_read(0, DTT_READ_TEMP_RMT2_LSB);
		break;
	case 3:
		if (!dtt_sensor_config.en_remote3) {
			printf("DTT:  Remote sensor 3 not enabled!\n");
			return 0;
		}
		temp = (dtt_read(0, DTT_READ_TEMP_RMT3_MSB) << 8)
			| dtt_read(0, DTT_READ_TEMP_RMT3_LSB);
		break;
	default:
		printf("DTT:  Invalid sensor number %d!\n", sensor);
		return 0;
	}

	/* Ignore LSB for now, U-Boot only prints natural numbers */
	temp >>= 8;

	if (dtt_sensor_config.range)
		temp += 64;

	debug("DTT: Sensor %d returned %dC\n", sensor, temp);

	return temp;
}

#if 0
int dtt_init(void)
{
	const char *const header = "DTT:   ";

	if (_dtt_init() != 0) {
		printf("%s FAILED INIT\n", header);
		return -1;
	}

	if (dtt_sensor_config.en_local)
		debug("%s local is %i C\n", header, dtt_get_temp(0));

	if ((num_remote_sensors >= 1) && dtt_sensor_config.en_remote1)
		debug("%s%d is %i C\n", header, 1, dtt_get_temp(1));

	if ((num_remote_sensors >= 2) && dtt_sensor_config.en_remote2)
		debug("%s%d is %i C\n", header, 2, dtt_get_temp(2));

	if ((num_remote_sensors >= 3) && dtt_sensor_config.en_remote3)
		debug("%s%d is %i C\n", header, 3, dtt_get_temp(3));

	return 0;
}
#endif
