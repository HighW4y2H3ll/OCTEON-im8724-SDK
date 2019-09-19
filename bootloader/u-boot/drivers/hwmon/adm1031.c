/*
 * (C) Copyright 2003
 * Murray Jensen, CSIRO-MIT, Murray.Jensen@csiro.au
 *
 * based on dtt/lm75.c which is ...
 *
 * (C) Copyright 2001
 * Bill Hunter,  Wave 7 Optics, williamhunter@mediaone.net
 *
 * (C) Copyright 2013
 * Aaron Williams, Cavium, Inc. <support@cavium.com>
 *
 * based on adm1021.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Analog Devices's ADM1031
 * "Intelligent Temperature Monitor and Dual PWM Fan Controller"
 */

#include <common.h>

#include <i2c.h>
#include <dtt.h>

/* Following macros takes channel parameter starting from 0 to 2 */
#define ADM1031_REG_FAN_SPEED(nr)	(0x08 + (nr))
#define ADM1031_REG_FAN_DIV(nr)		(0x20 + (nr))
#define ADM1031_REG_PWM			(0x22)
#define ADM1031_REG_FAN_MIN(nr)		(0x10 + (nr))
#define ADM1031_REG_FAN_FILTER		(0x23)

#define ADM1031_REG_TEMP_OFFSET(nr)	(0x0d + (nr))
#define ADM1031_REG_TEMP_MAX(nr)	(0x14 + 4 * (nr))
#define ADM1031_REG_TEMP_MIN(nr)	(0x15 + 4 * (nr))
#define ADM1031_REG_TEMP_CRIT(nr)	(0x16 + 4 * (nr))

#define ADM1031_REG_TEMP(nr)		(0x0a + (nr))
#define ADM1031_REG_AUTO_TEMP(nr)	(0x24 + (nr))

#define ADM1031_REG_STATUS(nr)		(0x2 + (nr))

#define ADM1031_REG_CONF1		0x00
#define ADM1031_REG_CONF2		0x01
#define ADM1031_REG_EXT_TEMP		0x06

#define ADM1031_CONF1_MONITOR_ENABLE	0x01	/* Monitoring enable */
#define ADM1031_CONF1_PWM_INVERT	0x08	/* PWM Invert */
#define ADM1031_CONF1_AUTO_MODE		0x80	/* Auto FAN */

#define ADM1031_CONF2_PWM1_ENABLE	0x01
#define ADM1031_CONF2_PWM2_ENABLE	0x02
#define ADM1031_CONF2_TACH1_ENABLE	0x04
#define ADM1031_CONF2_TACH2_ENABLE	0x08
#define ADM1031_CONF2_TEMP_ENABLE(chan)	(0x10 << (chan))

#define ADM1031_UPDATE_RATE_MASK	0x1c
#define ADM1031_UPDATE_RATE_SHIFT	2

typedef u9 auto_chan_table_t[8][2];

struct dtt_cfg {
	uint i2c_bus;
	uint i2c_addr:7;	/* 7bit i2c chip address */
	uint tach_mode:1;	/* 0 = use tach pin, 1 = use sense resistor */
	uint pwm_invert:1;	/* 1 = invert PWM output */
	uint fan_fault:1;	/* 1 = enable fan_fault logic pin (default 1) */
	uint pwm_mode:2;	/* 0 = remote temp 1 controls fan 1,
				 *     temp 2 controls fan 2
				 * 1 = remote temp 1 controls fans 1 & 2
				 * 2 = remote temp 2 controls fans 1 & 2
				 * 3 = max of local, temp 1 and temp 2
				 *     controls fans 1 & 2
				 */
	uint auto_fan_speed:1;	/* 1 = automatic fan speed control, 0 = sw control */
	uint fan1_spinup_time:14;/* Spin-up time in ms */
	uint fan1_pwm_freq:14;	/* PWM frequency in hz */
	u8 pwm_enable[2];
	u8 tach_enable[2];
	u8 fan_div[2];		/* Fan speed divider, 1, 2, 4 or 8 */
	u8 fan_min[2];		/* Minimum fan PWM */
	u8 temp_enable[3];
	u8 auto_temp[3];
	u8 auto_temp_min[3];	/* Minimum speed temp */
	u8 auto_temp_off[3];	/* Temp where fan turns off */
	u8 auto_temp_max[3];	/* Maximum speed temp */
	s8 temp_offset[3];
	s8 temp_min[3];		/* Minimum temp alarm */
	s8 temp_max[3];		/* Maximum temp alarm */
	s8 temp_crit[3];	/* THERM alarm */
	const auto_chan_table_t *chan_select_table;
};

#define TEMP_TO_REG(val)		(((val) < 0 ? ((val - 500) / 1000) : \
					((val + 500) / 1000)))
#define TEMP_FROM_REG(val)		((val) * 1000)

#define TEMP_FROM_REG_EXT(val, ext)	(TEMP_FROM_REG(val) + (ext) * 125)
#define TEMP_OFFSET_TO_REG(val)	(TEMP_TO_REG(val) & 0x8f)
#define TEMP_OFFSET_FROM_REG(val)	TEMP_FROM_REG((val) < 0 ? \
						      (val) : 0x70 : (val))

#define FAN_FROM_REG(reg, div)		((reg) ? \
					 (11250 * 60) / ((reg) * (div)) : 0)
#ifndef clamp_val
# define clamp_val(val, min, max) ({		\
	typeof(val) __val = (val);		\
	typeof(val) __min = (min);		\
	typeof(val) __max = (max);		\
	__val = __val < __min ? min: __val;	\
	__val > __max ? __max : __val; })
#endif

static int FAN_TO_REG(int reg, int div)
{
	int tmp;
	tmp = FAN_FROM_REG(clamp_val(reg, 0, 65535), div);
	return tmp > 255 ? 255 : tmp;
}

#define FAN_DIV_FROM_REG(reg)		(1<<(((reg)&0xc0)>>6))
#define PWM_TO_REG(val)			(clamp_val((val), 0, 255) >> 4)
#define PWM_FROM_REG(val)		((val) << 4)
#define FAN_CHAN_FROM_REG(reg)		(((reg) >> 5) & 7)
#define FAN_CHAN_TO_REG(val, reg)	(((reg) & 0x1F) | (((val) << 5) & 0xe0))
#define AUTO_TEMP_MIN_TO_REG(val, reg)	((((val) / 500) & 0xf8) | ((reg) & 0x7))
#define AUTO_TEMP_RANGE_FROM_REG(reg)	(5000 * (1 << ((reg) & 0x7)))
#define AUTO_TEMP_MIN_FROM_REG(reg)	(1000 * ((((reg) >> 3) & 0x1f) << 2))
#define AUTO_TEMP_MIN_FROM_REG_DEG(reg)	((((reg) >> 3) & 0x1f) << 2)
#define AUTO_TEMP_OFF_FROM_REG(reg)	(AUTO_TEMP_MIN_FROM_REG(reg) - 5000)
#define AUTO_TEMP_MAX_FROM_REG(reg)		\
	(AUTO_TEMP_RANGE_FROM_REG(reg) + AUTO_TEMP_MIN_FROM_REG(reg))

static int AUTO_TEMP_MAX_TO_REG(int val, int reg, int pwm)
{
	int ret;
	int range = val - AUTO_TEMP_MIN_FROM_REG(reg);

	range = ((val - AUTO_TEMP_MIN_FROM_REG(reg))*10)/(16 - pwm);
	ret = ((reg & 0xf8) |
	       (range < 10000 ? 0 :
		range < 20000 ? 1 :
		range < 40000 ? 2 : range < 80000 ? 3 : 4));
	return ret;
}

/* FAN auto control */
#define GET_FAN_AUTO_BITFIELD(data, idx)	\
	(*(data)->chan_select_table)[DAN_CHAN_FROM_REG((data)->conf1)][idx % 2]

/*
 * The tables below contains the possible values for the auto fan
 * control bitfields. the index in the table is the register value.
 * MSb is the auto fan control enable bit, so the four first entries
 * in the table disables auto fan control when both bitfields are zero.
 */
static const auto_chan_table_t auto_channel_select_table_adm1031 = {
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 2 /* 0b010 */ , 4 /* 0b100 */ },
	{ 2 /* 0b010 */ , 2 /* 0b010 */ },
	{ 4 /* 0b100 */ , 4 /* 0b100 */ },
	{ 7 /* 0b111 */ , 7 /* 0b111 */ },
};

static const auto_chan_table_t auto_channel_select_table_adm1030 = {
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 2 /* 0b10 */		, 0 },
	{ 0xff /* invalid */	, 0 },
	{ 0xff /* invalid */	, 0 },
	{ 3 /* 0b11 */		, 0 },
};

/*
 * That function checks if a bitfield is valid and returns the other bitfield
 * nearest match if no exact match where found.
 */
static int
get_fan_auto_nearest(struct dtt_cfg *data, int chan, u8 val, u8 reg)
{
	int i;
	int first_match = -1, exact_match = -1;
	u8 other_reg_val =
	    (*data->chan_select_table)[FAN_CHAN_FROM_REG(reg)][chan ? 0 : 1];

	if (val == 0)
		return 0;

	for (i = 0; i < 8; i++) {
		if ((val == (*data->chan_select_table)[i][chan]) &&
		    ((*data->chan_select_table)[i][chan ? 0 : 1] ==
		     other_reg_val)) {
			/* We found an exact match */
			exact_match = i;
			break;
		} else if (val == (*data->chan_select_table)[i][chan] &&
			   first_match == -1) {
			/*
			 * Save the first match in case of an exact match has
			 * not been found
			 */
			first_match = i;
		}
	}

	if (exact_match >= 0)
		return exact_match;
	else if (first_match >= 0)
		return first_match;

	return -EINVAL;
}

int
dtt_read (int sensor, int reg)
{
	dtt_cfg_t *dcp = &dttcfg[sensor / 3];
	uchar data;

	if (i2c_read(dcp->i2c_addr, reg, 1, &data, 1) != 0)
		return -1;

	return (int)data;
} /* dtt_read() */

int
dtt_write (int sensor, int reg, int val)
{
	dtt_cfg_t *dcp = &dttcfg[sensor/3];
	uchar data;

	data = (uchar)(val & 0xff);

	if (i2c_write(dcp->i2c_addr, reg, 1, &data, 1) != 0)
		return 1;

	return 0;
} /* dtt_write() */

int
dtt_init_one(int sensor)
{
	dtt_cfg_t *dcp = &dttcfg[sensor/3];
	int reg, val;
	int rc;

	if (sensor % 3 != 0)
		return 0;

	i2c_set_bus_num(dcp->i2c_bus);
	val = 1 | (dcp->tach_mode << 1) | (dcp->pwm_invert << 3) |
	      (dcp->fan_fault << 4) | (dcp->pwm_mode << 5) |
	      (dcp->auto_fan_speed << 7);

	rc = i2c_write(dcp->i2c_addr, 1, 0, &val, 1);
	if (rc) {
		printf("Could not write to DTT device at address 0x%x\n",
		       dcp->i2c_addr);
		return -1;
	}

	val = (dcp->pwm_enable[0] ? 1 : 0) |
	      (dcp->pwm_enable[1] ? 2 : 0) |
	      (dcp->tach_enable[0] ? 4 : 0) |
	      (dcp->tach_enable[1] ? 8 : 0) |
	      (dcp->temp_enable[0] ? 0x10 : 0) |
	      (dcp->temp_enable[1] ? 0x20 : 0) |
	      (dcp->temp_enable[2] ? 0x40 : 0);
	rc |= i2c_write(dcp->i2c_addr, 1, 1, &val, 1);


	return 0;
error:
	return -1;
} /* dtt_init_one() */

int
dtt_get_temp (int sensor)
{
	char val;
	dtt_cfg_t *dcp = &dttcfg[sensor / 3];
	int rc;

	i2c_set_bus_num(dcp->i2c_bus);
	switch (sensor % 3) {
	case 0:
		rc = i2c_read(dcp->i2c_addr, DTT_READ_LOCAL_TEMP, 1, &val, 1);
		break;
	case 1:
		rc = i2c_read(dcp->i2c_addr, DTT_READ_REMOTE_TEMP1, 1, &val, 1);
		break;
	case 2:
		rc = i2c_read(dcp->i2c_addr, DTT_READ_REMOTE_TEMP2, 1, &val, 1);
		break;
	default:
		rc = -1;
	}
	if (rc) {
		printf("DTT%d: Error reading temperature\n", sensor);
		return 1000;
	}

	debug("%s: sensor %d: %dC\n", __func__, sensor, (int)val);
	return (int) val;
} /* dtt_get_temp() */
