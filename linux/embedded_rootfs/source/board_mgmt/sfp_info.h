#ifndef __SFP_INFO_H_
#define __SFP_INFO_H_


#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define PIM_MAX_COUNT	2
#define RXAUI_MAX_COUNT	4
#define SFP_MAX_COUNT	8

#define PAGE_SIZE		128
#define VEN_ASCII_LEN	16
#define MAX_FILE_NAME	256
#define STR_I2C		"i2c"
#define PIM_DIAG_FOLDER	"/sys/niagara/pim%d/diag/%ssfp%d/speed"
#define SYS_DIAG_FOLDER	"/sys/niagara/diag/sfp%d/speed"

#define MAJOR_VER	0
#define MINOR_VER	4

#define SRC_INFO    0
#define SRC_DIAG    1

/* FINISAR application Note AN-2030 */
#define SFP_XFP_TEMPERATURE_ADDR      96
#define SFP_XFP_VOLTAGE_ADDR          98
#define SFP_XFP_CURRENT_ADDR         100
#define SFP_XFP_TX_POWER_ADDR        102
#define SFP_XFP_RX_POWER_ADDR        104

#define QSFP_LOS_ADDR                   3
#define QSFP_TX_FAULT_ADDR              4
#define QSFP_TEMPERATURE_ADDR          22
#define QSFP_VOLTAGE_ADDR              26
#define QSFP_RX_POWER_ADDR             34
#define QSFP_TX_BIAS_CURRENT_ADDR      42
#define QSFP_TX_POWER_ADDR             50

/* 0xA2 */
/* Calibration constants */
#define SFP_RX_PWR4_ADDR              56
#define SFP_RX_PWR3_ADDR              60
#define SFP_RX_PWR2_ADDR              64
#define SFP_RX_PWR1_ADDR              68
#define SFP_RX_PWR0_ADDR              72
#define SFP_TX_CURRENT_SLOPE_ADDR     76
#define SFP_TX_CURRENT_OFFSET_ADDR    78
#define SFP_TX_POWER_SLOPE_ADDR       80
#define SFP_TX_POWER_OFFSET_ADDR      82
#define SFP_T_SLOPE_ADDR              84
#define SFP_T_OFFSET_ADDR             86
#define SFP_V_SLOPE_ADDR              88
#define SFP_V_OFFSET_ADDR             90


#endif // __SFP_INFO_H_