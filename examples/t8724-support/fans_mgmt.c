#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include <cvmx.h>
#include <cvmx-platform.h>

#include "imt_cpld.h"
#include "fans_mgmt.h"

#define DEBUG         0

#if DEBUG
#define debug   printf
#else
#define debug(...)
#endif

static uint8_t cpu_set_temp = 0;
static uint64_t curtime = 0;
static int fan_is_init = 0;

static int get_max_cpu_brd_temp(uint8_t *temp);
static uint64_t hw_get_cycles(void);
static int init_temp_sensors(void);
static void reset_device(void);

int fans_init(void)
{
	int indx;

	if (!cpld_is_init) {
		printf("Error: CPLD is not init!\n");
		return -1;
	}

	/* Check fan board presence */
	if (cpld_rd_remote(0x1e, CPLD_FAN_ADDR_SIGNATURE) != 0x8d) {
		printf("Fan Board CPLD not detected, no fan speed control!\n");
		return -1;
	}

	/* Check cpu/board temperature sensorce, init thresholds */
	if (init_temp_sensors() < 0) {
		printf("Temp sensorce check/init failure!\n");
		return -1;
	}

	/* Turn off CPLD max fans speed bit */
	cpld_wr_remote(0x1e, CPLD_FAN_ADDR_FC_SS, 0);

	printf("Set initial fans speed...\n");
	for (indx = 0; indx < ADT7470_REG_PWM_MAX_NUMBER; indx++) {
		cpld_i2c_write(0x9e, 0x0, 0x2c, ADT7470_REG_PWM(indx), DEFAULT_FAN_RPM_VALUE);
		cpld_i2c_write(0x9e, 0x0, 0x2f, ADT7470_REG_PWM(indx), DEFAULT_FAN_RPM_VALUE);
	}
	printf(" [done]\n]Fan/temp sensors board init is finished!\n");
	curtime = hw_get_cycles();
	fan_is_init = 1;
	return 0;
}

void fans_mgmt(void)
{
	int fan_speed, indx;
	uint8_t cpu_tmp = MAX_TEMP_ALARM_THRESH;

	if (!fan_is_init) {
		debug("Error: Fans are not initialized!\n");
		return;
	}

	if (hw_get_cycles() < 1000000000ULL + curtime)
		return;

	if ((get_max_cpu_brd_temp(&cpu_tmp) < 0) &&
	    (get_max_cpu_brd_temp(&cpu_tmp) < 0))
		fan_speed = 0xff;
	else {
		if (cpu_tmp < MAX_TEMP_ALARM_THRESH)
			fan_speed = DEFAULT_FAN_RPM_VALUE + (MAX_FAN_RPM_VALUE - DEFAULT_FAN_RPM_VALUE) *
				(cpu_tmp - cpu_set_temp)/(MAX_TEMP_ALARM_THRESH - cpu_set_temp);
		else
			fan_speed = 0xff;
	}

	if (cpu_tmp >= MAX_TEMP_CRITICAL_THRESH) {
		printf("FAIL: Critical box temperature is reached!\n");
		reset_device();
	}

	debug("cpu_tmp:%d,cpu_set_temp:%d,fan_speed:%d,error:%d,I:%d\n",
				cpu_tmp, cpu_set_temp, fan_speed, error, I);

	for (indx = 0; indx < ADT7470_REG_PWM_MAX_NUMBER; indx++) {
		cpld_i2c_write(0x9e, 0x0, 0x2c, ADT7470_REG_PWM(indx), (uint8_t) fan_speed);
		cpld_i2c_write(0x9e, 0x0, 0x2f, ADT7470_REG_PWM(indx), (uint8_t) fan_speed);
	}
	curtime = hw_get_cycles();
}

/* Read the cycles register state on cnMIPS Coprocessor 0 (COP0) */
static uint64_t hw_get_cycles(void)
{
	uint64_t result;

	asm volatile (
		"   dmfc0 %[rt], $9, 6"
		: [rt] "=d" (result)
 		:
	);
	return (result);
}

static void reset_device(void)
{
	if (cpld_is_init)
		reset_unit();
	printf("Reset CN78XX!\n");
	cvmx_reset_octeon();
}

static int init_im8724_temp_sensors(void)
{
	uint8_t cval, nval, cpld_reg;
#if !CHECK_CPU_TEMP_SENSORS_ONLY
	uint8_t sasl;
	int pim;
#endif
	int i;

	/* Enable Over-Temperature Shut Down Enable. */
	cpld_reg = cpld_rd(CPLD_ADDR_INT_EN_0) | 0x80;
	cpld_wr(CPLD_ADDR_INT_EN_0, cpld_reg);

	debug("Setting over temp threshold to %dC\n", MAX_TEMP_CRITICAL_THRESH);
        /* MAX6699 */
        for (i=0; i<5; ++i) {
		nval = MAX_TEMP_CRITICAL_THRESH;
                octeon_i2c_read(1, MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &cval, 1);
                octeon_i2c_write(1, MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &nval, 1);
                octeon_i2c_read(1, MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &nval, 1);
                printf("Changed temp crit thresh [max6699]: %uC->%uC\n", cval, nval);
        }

#if !CHECK_CPU_TEMP_SENSORS_ONLY
        /* ADM1032 */
        for (pim=0; pim<2; ++pim) {
                sasl = pim ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
                for (i=0; i<2; ++i) {
                        cval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_TEMP_CRIT(i));
                        cpld_i2c_write(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_TEMP_CRIT(i), MAX_TEMP_CRITICAL_THRESH);
                        nval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_TEMP_CRIT(i));
                        printf("Changed temp crit thresh [adm1032-pim%d]: %uC->%uC\n", pim, cval, nval);
                }
        }
#endif
	return 0;
}

static int get_im8742_max_cpu_brd_temp(uint8_t *temp)
{
	return octeon_i2c_read(1, MAX6699_DEV_ADDR, 0x1, 1, temp, 1);
}

static int init_temp_sensors(void)
{
	if (get_max_cpu_brd_temp(&cpu_set_temp) < 0)
		if (get_max_cpu_brd_temp(&cpu_set_temp) < 0)
			return -1;
	if (cpu_set_temp > CPU_SET)
		cpu_set_temp = CPU_SET;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_IM8724)	
		return init_im8724_temp_sensors();
	return -1;
}


static int get_max_cpu_brd_temp(uint8_t *temp)
{
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_IM8724)
		return get_im8742_max_cpu_brd_temp(temp);
	return -1;
}

static uint32_t get_adt7470_tach(int addr, int indx)
{
	uint32_t val = cpld_i2c_read(0x9e, 0x0, addr, ADT7470_REG_TACH(indx));
	val |= ((uint16_t)cpld_i2c_read(0x9e, 0x0, addr, ADT7470_REG_TACH(indx) + 1) << 8);

	if(0xffff != val)
		return val;
	else
		return 0;
}

uint32_t get_fans_lowest(void)
{
	int indx;
	uint32_t val, ret = 0xfffe; // 0xffff at adt7470 means 100rpm or lower

	for (indx = 0; indx < ADT7470_REG_TACH_MAX_NUMBER; indx++) {
		val = get_adt7470_tach(0x2c, indx);
		if(val < ret)
			ret = val;

		val = get_adt7470_tach(0x2f, indx);
		if(val < ret)
			ret = val;

		if(!ret)
			break;
	}

	return ret;
}
