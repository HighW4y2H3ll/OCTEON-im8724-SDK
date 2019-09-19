#ifndef _LEDS_MGMT_H_
#define _LEDS_MGMT_H_

/* Fan management application thresholds 
 * and default settings */
#define CPU_SET				60
#define MAX_TEMP_ALARM_THRESH		87
#define MAX_TEMP_CRITICAL_THRESH	95
#define DEFAULT_FAN_RPM_VALUE		0x40
#define MAX_FAN_RPM_VALUE		0xff

/* Check only CPU temp sensors only or not? */
#define CHECK_CPU_TEMP_SENSORS_ONLY	1

/* IM13026, FAN, board cpld registers */
#define CPLD_FAN_ADDR_SIGNATURE		0x00
#define CPLD_FAN_ADDR_FC_SS		0x04
#define CPLD_FAN_ADDR_FC_STAT		0x05
#define CPLD_FAN_ADDR_FAN_PWR_EN	0x07
#define CPLD_FAN_ADDR_FAN_PWR_ST	0x08

/* ADT7470 defines */
#define ADT7470_REG_PWM_MAX_NUMBER	4
#define ADT7470_REG_PWM_BASE_ADDR	0x32
#define ADT7470_REG_PWM(x)		(ADT7470_REG_PWM_BASE_ADDR + (x))

#define ADT7470_REG_TACH_MAX_NUMBER	4
#define ADT7470_REG_TACH_BASE_ADDR	0x2A
#define ADT7470_REG_TACH(x)		(ADT7470_REG_TACH_BASE_ADDR + (x))

/* IM8724 temperature sensors specific defines */
#define MAX6699_DEV_ADDR			0x4c
#define MAX6699_REG_TEMP_CRIT(nr)		(0x20 + nr)
#define ADM1032_DEV_ADDR			0x4c
#define ADM1032_REG_TEMP_CRIT(nr)		(0x16 + 4 * (nr))

int fans_init(void);
void fans_mgmt(void);

#endif /* _LEDS_MGMT_H_ */
