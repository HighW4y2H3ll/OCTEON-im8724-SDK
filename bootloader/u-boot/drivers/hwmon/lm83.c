/*
 * (C) Copyright 2011
 * Cavium Inc., Inc. <support@cavium.com>
 *
 * (C) Copyright 2006
 * Heiko Schocher, DENX Software Enginnering <hs@denx.de>
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
 * On Semiconductor's LM83 Temperature Sensor
 */
#undef DEBUG
#include <common.h>
#include <i2c.h>
#include <dtt.h>

/*
 * Device code
 */
#ifndef CONFIG_SYS_I2C_DTT_ADDR
#define DTT_I2C_DEV_CODE 		0x28	/* ON Semi's LM83 device */
#else
#define DTT_I2C_DEV_CODE		CONFIG_SYS_I2C_DTT_ADDR
#endif
#define DTT_READ_LOCAL_TEMP		0x00
#define DTT_READ_D2_REMOTE_TEMP		0x01
#define DTT_READ_STATUS			0x02
#define DTT_READ_CONFIG			0x03
#define DTT_READ_LOC_HI_SETPT		0x05	/* Local high setpoint */
#define DTT_READ_D2_REMOTE_HI_SETPT	0x07	/* Remote D2 high setpoint */
#define DTT_WRITE_CONFIG		0x09
#define DTT_WRITE_LOCAL_HI_SETPT	0x0B
#define DTT_WRITE_D2_REMOTE_HI_SETPT	0x0D
#define DTT_READ_D1_REMOTE_TEMP		0x30
#define DTT_READ_D3_REMOTE_TEMP		0x31
#define DTT_READ_STATUS2		0x35
#define DTT_READ_D1_REMOTE_HI_SETPT	0x38
#define DTT_READ_D3_REMOTE_HI_SETPT	0x3A
#define DTT_READ_TCRIT_SETPT		0x42
#define DTT_WRITE_D1_REMOTE_HI_SETPT	0x50
#define DTT_WRITE_D3_REMOTE_HI_SETPT	0x52
#define DTT_WRITE_TCRIT_SETPT		0x5A
#define DTT_READ_MANUFACTURER_ID	0xFE	/* 0x01 */
#define DTT_READ_STEPPING		0xFF	/* Die stepping */

static struct dtt_sensor_config {
	int	enable_d1_crit:1;	/* Enable D1 critical temp interrupt */
	int	enable_d2_crit:1;	/* Enable D2 critical temp interrupt */
	int	enable_d3_crit:1;	/* Enable D3 critical temp interrupt */
	int	enable_local_crit:1;	/* Enable local critical temp int. */
	int	tcrit_temp:8;		/* Critical temp (max=127C) */
	int	invert_int:1;		/* 1 = interrupt line is active low */
	int	local_highpt:8;		/* local highpoint alarm temp in C */
	int	d1_highpt:8;		/* D1 highpoint alarm temp in C */
	int	d2_highpt:8;		/* D2 highpoint alarm temp in C */
	int	d3_highpt:8;		/* D3 highpoint alarm temp in C */
} dtt_sensor_config
#ifdef CONFIG_SYS_DTT_LM83
	= CONFIG_SYS_DTT_LM83;
#else
	= {
		.enable_d1_crit = 0,
		.enable_d2_crit = 0,
		.enable_d3_crit = 0,
		.enable_local_crit = 0,
		.tcrit_temp = 127,
		.invert_int = 0,
		.local_highpt = 127,
		.d1_highpt = 127,
		.d2_highpt = 127,
		.d3_highpt = 127
	};
#endif

int dtt_read(int sensor, int reg)
{
    int dlen = 1;
    uchar data[2];

    /*
     * Calculate sensor address and register.
     */
    sensor = DTT_I2C_DEV_CODE + ((sensor >> 2) & 0x03); /* calculate address of lm83 */

    /*
     * Now try to read the register.
     */
    if (i2c_read(sensor, reg, 1, data, dlen) != 0)
	return -1;

    return (int)data[0];
} /* dtt_read() */


int dtt_write(int sensor, int reg, int val)
{
    uchar data;

    /*
     * Calculate sensor address and register.
     */
    sensor = DTT_I2C_DEV_CODE + ((sensor >> 2) & 0x03); /* calculate address of lm83 */

    data = (char)(val & 0xff);

    /*
     * Write value to register.
     */
    if (i2c_write(sensor, reg, 1, &data, 1) != 0)
	return 1;

    return 0;
} /* dtt_write() */

#define DTT_MANU	0x3e
#define DTT_REV		0x3f
#define DTT_CONFIG	0x40
#define DTT_ADR		0x48

int dtt_init_one(int sensor)
{
	uint8_t	status1;
	uint8_t	status2;
	uint8_t	reg;
	int man;
	int stepping;

	if (sensor & 3) {
		debug("LM83: not initializing for sensor %d\n", sensor);
		return 0;
	}

	if ((man = dtt_read(sensor, DTT_READ_MANUFACTURER_ID)) != 1) {
		printf("LM83: Invalid manufacturer ID\n");
		return -1;
	}
	status1 = dtt_read(sensor, DTT_READ_STATUS);
	status2 = dtt_read(sensor, DTT_READ_STATUS2);
	debug("LM83: status1: 0x%x, status2: 0x%x\n", status1, status2);

	dtt_write(sensor, DTT_WRITE_TCRIT_SETPT, dtt_sensor_config.tcrit_temp);
	dtt_write(sensor, DTT_WRITE_D1_REMOTE_HI_SETPT, dtt_sensor_config.d1_highpt);
	dtt_write(sensor, DTT_WRITE_D2_REMOTE_HI_SETPT, dtt_sensor_config.d2_highpt);
	dtt_write(sensor, DTT_WRITE_D3_REMOTE_HI_SETPT, dtt_sensor_config.d3_highpt);
	dtt_write(sensor, DTT_WRITE_LOCAL_HI_SETPT, dtt_sensor_config.local_highpt);

	reg = 0;
	if (dtt_sensor_config.invert_int)
		reg |= 0x82;
	if (dtt_sensor_config.enable_local_crit)
		reg |= 0x84;
	if (dtt_sensor_config.enable_d3_crit)
		reg |= 0x88;
	if (dtt_sensor_config.enable_d2_crit)
		reg |= 0x90;
	if (dtt_sensor_config.enable_d1_crit)
		reg |= 0xA0;
	if (dtt_write (sensor, DTT_WRITE_CONFIG, reg) < 0)
		return 1;

	/* The LM81 needs 400ms to get the correct values ... */
	udelay (400000);
	stepping = dtt_read (sensor, DTT_READ_STEPPING);
	if (stepping < 0)
		return 1;

	debug ("DTT:   Found LM83@%x Stepping: %d\n", sensor >> 2, stepping);
	return 0;
} /* _dtt_init() */


int dtt_get_temp(int sensor)
{
	int status;
	switch (sensor & 3) {
	case 0:	/* local */
		return dtt_read(sensor, DTT_READ_LOCAL_TEMP);
	case 1:
		return dtt_read(sensor, DTT_READ_D1_REMOTE_TEMP);
	case 2:
		status = dtt_read(sensor, DTT_READ_STATUS);
		/* Check if D2 is open */
		if (status > 0 && status & 0x4)
			return 0;
		return dtt_read(sensor, DTT_READ_D2_REMOTE_TEMP);
	case 3:
		return dtt_read(sensor, DTT_READ_D3_REMOTE_TEMP);
	}
	return -1;
} /* dtt_get_temp() */
