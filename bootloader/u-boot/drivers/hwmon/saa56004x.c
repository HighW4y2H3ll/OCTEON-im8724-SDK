/*
 * (C) Copyright 2011
 * Cavium Inc., Inc. <support@cavium.com>
 *
 * Based on adm1021.c which is
 *
 * (C) Copyright 2003
 * Murray Jensen, CSIRO-MIT, Murray.Jensen@csiro.au
 *
 * based on dtt/lm75.c which is ...
 *
 * (C) Copyright 2001
 * Bill Hunter,  Wave 7 Optics, williamhunter@mediaone.net
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
 * Philips Semiconducter/NPX SA56004X
 * "Digital temperature sensor with overtemperature alarms"
 */

#include <common.h>

#include <i2c.h>
#include <dtt.h>

#ifndef CONFIG_SYS_I2C_DTT_BUS
# define CONFIG_SYS_I2C_DTT_BUS		0
#endif

#define DTT_READ_LOC_VALUE	0x00
#define DTT_READ_REM_VALUE	0x01
#define DTT_READ_STATUS		0x02
#define DTT_READ_CONFIG		0x03
#define DTT_READ_CONVRATE	0x04
#define DTT_READ_LOC_HIGHLIM	0x05
#define DTT_READ_LOC_LOWLIM	0x06
#define DTT_READ_REM_HIGHLIM	0x07
#define DTT_READ_REM_LOWLIM	0x08
#define DTT_READ_REM_LOVALUE	0x10
#define DTT_READ_REM_OFFSET_HI	0x11
#define DTT_READ_REM_OFFSET_LO	0x12
#define DTT_READ_REM_HI_SET_LB	0x13	/* Remote high setpoint low byte */
#define DTT_READ_REM_LO_SET_LB	0x14	/* Remote low setpoint low byte */
#define DTT_READ_REM_CRIT_SET	0x19	/* Remote T_CRIT setpoint */
#define DTT_READ_LOC_CRIT_SET	0x20	/* Local T_CRIT setpoint */
#define DTT_READ_TCRIT_HYST	0x21	/* T_CRIT hysteresis */
#define DTT_READ_LOC_TMP_LB	0x22	/* Local temperature low byte */
#define DTT_READ_ALERT_MODE	0xBF	/* Alert mode */
#define DTT_READ_DEVID		0xfe

#define DTT_WRITE_CONFIG	0x09
#define DTT_WRITE_CONVRATE	0x0a
#define DTT_WRITE_LOC_HIGHLIM	0x0b
#define DTT_WRITE_LOC_LOWLIM	0x0c
#define DTT_WRITE_REM_HIGHLIM	0x0d
#define DTT_WRITE_REM_LOWLIM	0x0e
#define DTT_WRITE_ONESHOT	0x0f
#define DTT_WRITE_REM_OFFSET_HI	0x11
#define DTT_WRITE_REM_OFFSET_LO	0x12
#define DTT_WRITE_REM_HI_SET_LB	0x13	/* Remote high setpoint low byte */
#define DTT_WRITE_REM_LO_SET_LB	0x14	/* Remote low setpoint low byte */
#define DTT_WRITE_REM_CRIT_SET	0x19	/* Remote T_CRIT setpoint */
#define DTT_WRITE_LOC_CRIT_SET	0x20	/* Local T_CRIT setpoint */
#define DTT_WRITE_TCRIT_HYST	0x21	/* T_CRIT hysteresis */
#define DTT_WRITE_ALERT_MODE	0xBF	/* Alert mode */


#define DTT_STATUS_BUSY		0x80	/* 1=ADC Converting */
#define DTT_STATUS_LHIGH	0x40	/* 1=Local High Temp Limit Tripped */
#define DTT_STATUS_LLOW		0x20	/* 1=Local Low Temp Limit Tripped */
#define DTT_STATUS_RHIGH	0x10	/* 1=Remote High Temp Limit Tripped */
#define DTT_STATUS_RLOW		0x08	/* 1=Remote Low Temp Limit Tripped */
#define DTT_STATUS_OPEN		0x04	/* 1=Remote Sensor Open-Circuit */
#define DTT_STATUS_RCRIT	0x02	/* 1=Remote critical temp alarm */
#define DTT_STATUS_LCRIT	0x01	/* 1=Local critical temp alarm */

#define DTT_CONFIG_ALERT_MASKED	0x80	/* 0=ALERT Enabled, 1=ALERT Masked */
#define DTT_CONFIG_STANDBY	0x40	/* 0=Run, 1=Standby */

#define DTT_ADM1021_DEVID	0x41

typedef struct {
	uint32_t i2c_addr:7;	/* 7bit i2c chip address */
	uint32_t conv_rate:4;	/* conversion rate */
	uint32_t enable_alert:1;	/* enable alert output pin */
	uint32_t enable_local:1;	/* enable internal temp sensor */
	uint32_t max_local:8;	/* internal temp maximum */
	uint32_t min_local:8;	/* internal temp minimum */
	uint32_t enable_remote:1;	/* enable remote temp sensor */
	uint32_t :2;		/* pad */
	uint32_t max_remote:8;	/* remote temp maximum */
	uint32_t min_remote:8;	/* remote temp minimum */
	uint32_t crit_remote:8;	/* remote critical temperature */
	uint32_t crit_local:8;	/* local critical temperature */
} dtt_cfg_t;

dtt_cfg_t dttcfg[] = CONFIG_SYS_DTT_SAA56004X;

void dtt_set_addr(unsigned dev, int8_t addr)
{
	if (dev > ARRAY_SIZE(dttcfg)) {
		printf("%s: dev %u is out of range!\n", __func__, dev);
		return;
	}
	dttcfg[dev].i2c_addr = addr;
}

int dtt_get_temp(int sensor);

int dtt_read(int sensor, int reg)
{
	dtt_cfg_t *dcp = &dttcfg[sensor >> 1];
	uchar data;
	unsigned current_i2c_bus = i2c_get_bus_num();
	int rc;

	if (current_i2c_bus != CONFIG_SYS_I2C_DTT_BUS)
		i2c_set_bus_num(CONFIG_SYS_I2C_DTT_BUS);
	rc = i2c_read(dcp->i2c_addr, reg, 1, &data, 1);
	if (current_i2c_bus != CONFIG_SYS_I2C_DTT_BUS)
		i2c_set_bus_num(current_i2c_bus);

	return rc ? -1 : (int)data;
} /* dtt_read() */

int dtt_write(int sensor, int reg, int val)
{
	dtt_cfg_t *dcp = &dttcfg[sensor >> 1];
	uchar data;
	unsigned current_i2c_bus = i2c_get_bus_num();
	int rc;

	data = (uchar)(val & 0xff);

	if (current_i2c_bus != CONFIG_SYS_I2C_DTT_BUS)
		i2c_set_bus_num(CONFIG_SYS_I2C_DTT_BUS);
	rc = i2c_write(dcp->i2c_addr, reg, 1, &data, 1);
	if (current_i2c_bus != CONFIG_SYS_I2C_DTT_BUS)
		i2c_set_bus_num(current_i2c_bus);

	return rc ? -1 : 0;
} /* dtt_write() */

int dtt_init_one(int sensor)
{
	dtt_cfg_t *dcp = &dttcfg[sensor >> 1];
	int reg, val;

	if (((sensor & 1) == 0 ? dcp->enable_local : dcp->enable_remote) == 0)
		return 1;	/* sensor is disabled (or rather ignored) */

	/*
	 * Setup High Limit register
	 */
	if ((sensor & 1) == 0) {
		reg = DTT_WRITE_LOC_HIGHLIM;
		val = dcp->max_local;
	}
	else {
		reg = DTT_WRITE_REM_HIGHLIM;
		val = dcp->max_remote;
	}
	if (dtt_write (sensor, reg, val) != 0)
		return 1;

	/*
	 * Setup Low Limit register
	 */
	if ((sensor & 1) == 0) {
		reg = DTT_WRITE_LOC_LOWLIM;
		val = dcp->min_local;
	}
	else {
		reg = DTT_WRITE_REM_LOWLIM;
		val = dcp->min_remote;
	}
	if (dtt_write (sensor, reg, val) != 0)
		return 1;

	/*
	 * Setup critical register
	 */
	if ((sensor & 1) == 0) {
		reg = DTT_WRITE_LOC_CRIT_SET;
		val = dcp->crit_local;
	}
	else {
		reg = DTT_WRITE_REM_CRIT_SET;
		val = dcp->crit_remote;
	}
	if (dtt_write (sensor, reg, val) != 0)
		return 1;

	/* shouldn't hurt if the rest gets done twice */

	/*
	 * Setup Conversion Rate register
	 */
	if (dtt_write (sensor, DTT_WRITE_CONVRATE, dcp->conv_rate) != 0)
		return 1;

	/*
	 * Setup configuraton register
	 */
	val = 0;				/* running */
	if (dcp->enable_alert == 0)
		val |= DTT_CONFIG_ALERT_MASKED;	/* mask ALERT pin */
	if (dtt_write (sensor, DTT_WRITE_CONFIG, val) != 0)
		return 1;

	return 0;
} /* _dtt_init() */

#if 0
int
dtt_init (void)
{
	int i;
	unsigned char sensors[] = CONFIG_DTT_SENSORS;
	const char *const header = "DTT:   ";

	/* switch to correct I2C bus */
	I2C_SET_BUS(CONFIG_SYS_DTT_BUS_NUM);

	for (i = 0; i < sizeof(sensors); i++) {
		if (_dtt_init(sensors[i]) != 0)
			printf ("%s%d FAILED INIT\n", header, i+1);
		else
			printf ("%s%d is %i C\n", header, i+1,
				dtt_get_temp(sensors[i]));
	}

	return (0);
} /* dtt_init() */
#endif

int dtt_get_temp(int sensor)
{
	signed char val;

	if ((sensor & 1) == 0)
		val = dtt_read(sensor, DTT_READ_LOC_VALUE);
	else
		val = dtt_read(sensor, DTT_READ_REM_VALUE);

	return (int) val;
} /* dtt_get_temp() */
