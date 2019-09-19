#ifndef __LEDS_MGMT_H_
#define __LEDS_MGMT_H_

#define MGMT_OPT_PIM_PHYS         0x1
#define MGMT_OPT_FP_PHYS          0x2
#define MGMT_OPT_MGMT_PHYS        0x4
#define MGMT_OPT_ALL_PHYS         (MGMT_OPT_PIM_PHYS | MGMT_OPT_FP_PHYS | MGMT_OPT_MGMT_PHYS)

/* Leds mgmt constat defines */
#define MAX_IM8724_MODULES          2
#define MAX_SFP_PORTS_PER_MODULE	8
#define MAX_QSFP_PORTS_PER_MODULE	2
#define MAX_SFP28_PORTS_PER_MODULE	2
#define MAX_IM1326X_PORTS_PER_MODULE (((MAX_SFP28_PORTS_PER_MODULE) > (MAX_QSFP_PORTS_PER_MODULE)) \
                                        ? (MAX_SFP28_PORTS_PER_MODULE) : (MAX_QSFP_PORTS_PER_MODULE))
#define MAX_PORTS_PER_MODULE        (((MAX_SFP_PORTS_PER_MODULE) > (MAX_IM1326X_PORTS_PER_MODULE)) \
                                        ? (MAX_SFP_PORTS_PER_MODULE) : (MAX_IM1326X_PORTS_PER_MODULE))
#define MAX_FRONTPANEL_PORTS	  4

#define BUF_SIZE		128

#define DEFAULT_BOARD_TEMP	65

#define ACTIVITY_UNDEF		  2
#define HI_TEMPERATURE		 70

#define SPEED_UNDEF		  0
#define SPEED_1G		  1
#define SPEED_10M		  2
#define SPEED_100M		  3
#define SPEED_10G		  10
#define SPEED_40G		  40
#define SPEED_25G		  25

#define IFACE_OFF		0
#define IFACE_ON		1

#define NPATH                   "/sys/niagara/"
#define CPATH                   "/sys/class/"

#define MAIN_BOARD_LEDS_CFG	NPATH"leds/led_cfg"
#define BOARD_FP_COL_CFG	NPATH"leds/c_col"
#define BOARD_FP_ACT_CFG	NPATH"leds/c_act"
#define BOARD_MGMT_COL_CFG	NPATH"leds/v_col"
#define BOARD_MGMT_ACT_CFG	NPATH"leds/v_act"

#define IOA_CARD_PRES		NPATH"misc/ioa_card_pres"
#define IOB_CARD_PRES		NPATH"misc/iob_card_pres"
#define IOA_CARD_SIGN		NPATH"misc/ioa_card_sig_l"
#define IOB_CARD_SIGN		NPATH"misc/iob_card_sig_l"

#define IO_CARD_13233_SIG_VALUE	0x4c
#define IO_CARD_13264_SIG_VALUE	0x57
#define IO_CARD_13262_SIG_VALUE	0x5f

#define IO_CARD_POWER_ENABLE           NPATH"power/io%c_pe"
#define IO_CARD_HOTPLUG_POWER_ENABLE   NPATH"power/io%c_hp_pe"

#define IO_CARD_S_SFP_PRES_FMT	NPATH"pim%d/sfp%d/pres"
#define IO_CARD_S_SFP_RXLOS_FMT	NPATH"pim%d/sfp%d/rx_los"
#define IO_CARD_S_SFP_TXDIS_FMT	NPATH"pim%d/sfp%d/tx_dis"
#define IO_CARD_S_SFP_SPEED_FMT	NPATH"pim%d/diag/sfp%d/speed"
#define IO_CARD_S_SFP_TEMP_FMT	NPATH"pim%d/diag/sfp%d/temp"
#define IO_CARD_S_M_TYPE_FMT	NPATH"pim%d/diag/sfp%d/media"

#define IO_CARD_Q_SFP_PRES_FMT	NPATH"pim%d/qsfp%d/pres"
#define IO_CARD_Q_SFP_RXLOS_FMT	NPATH"pim%d/qsfp%d/rx_los"
#define IO_CARD_Q_SFP_TXDIS_FMT	NPATH"pim%d/qsfp%d/tx_dis"
#define IO_CARD_Q_SFP_SPEED_FMT	NPATH"pim%d/diag/qsfp%d/speed"
#define IO_CARD_Q_SFP_TEMP_FMT	NPATH"pim%d/diag/qsfp%d/temp"
#define IO_CARD_Q_M_TYPE_FMT	NPATH"pim%d/diag/qsfp%d/media"

#define IO_CARD_SFP_PRES_FMT(m)   (m) ? IO_CARD_Q_SFP_PRES_FMT : IO_CARD_S_SFP_PRES_FMT
#define IO_CARD_SFP_RXLOS_FMT(m)  (m) ? IO_CARD_Q_SFP_RXLOS_FMT : IO_CARD_S_SFP_RXLOS_FMT
#define IO_CARD_SFP_TXDIS_FMT(m)  (m) ? IO_CARD_Q_SFP_TXDIS_FMT : IO_CARD_S_SFP_TXDIS_FMT
#define IO_CARD_SFP_SPEED_FMT(m)  (m) ? IO_CARD_Q_SFP_SPEED_FMT : IO_CARD_S_SFP_SPEED_FMT
#define IO_CARD_SFP_TEMP_FMT(m)   (m) ? IO_CARD_Q_SFP_TEMP_FMT : IO_CARD_S_SFP_TEMP_FMT
#define IO_CARD_MEDIA_TYPE_FMT(m) (m) ? IO_CARD_Q_M_TYPE_FMT : IO_CARD_S_M_TYPE_FMT

#define IO_CARD_LED_ACT_CFG	NPATH"pim%d/led/cfg/phy_led_act"
#define IO_CARD_LED_COL_CFG	NPATH"pim%d/led/cfg/phy_led_col"

#define IO_CARD_LED_S_ACT_FMT	NPATH"pim%d/led/sfp%d/lc"
#define IO_CARD_LED_S_COL_FMT	NPATH"pim%d/led/sfp%d/col"
#define IO_CARD_LED_S_INDEX_FMT	NPATH"pim%d/led/sfp%d/index"

#define IO_CARD_LED_Q_ACT_FMT	NPATH"pim%d/led/qsfp%d/lc"
#define IO_CARD_LED_Q_COL_FMT	NPATH"pim%d/led/qsfp%d/col"
#define IO_CARD_LED_Q_INDEX_FMT	NPATH"pim%d/led/qsfp%d/index"

#define IO_CARD_LED_ACT_FMT(m)  (m) ? IO_CARD_LED_Q_ACT_FMT : IO_CARD_LED_S_ACT_FMT
#define IO_CARD_LED_COL_FMT(m)  (m) ? IO_CARD_LED_Q_COL_FMT : IO_CARD_LED_S_COL_FMT
#define IO_CARD_LED_INDEX_FMT(m) (m) ? IO_CARD_LED_Q_INDEX_FMT : IO_CARD_LED_S_INDEX_FMT

#define PIM_LED_ACT_FMT		NPATH"leds/w%d_act"
#define PIM_LED_COL_FMT		NPATH"leds/w%d_col"

#define PIM_TM_ALERT		NPATH"pim%d/misc/tm_alert"
#define PIM_TM_OVERT		NPATH"pim%d/misc/tm_overt"
#define PIM_TEMP_EV		NPATH"pim%d/misc/temp_ev"
#define PIM_TEMP_EN		NPATH"pim%d/misc/temp_en"
#define PIM_SW_RST		NPATH"pim%d/misc/s_rst"

#define PWR_LED_COLOR		NPATH"leds/pwr%d_col"
#define PWR_LED_MODE		NPATH"leds/pwr%d_lc"
#define SYS_LED_COLOR		NPATH"leds/status_col"
#define SYS_LED_MODE		NPATH"leds/status_lc"

#define PIM_SYSLED_COLOR	NPATH"pim%d/led/system/color"
#define PIM_SYSLED_MODES	NPATH"pim%d/led/system/mode"

#define TEMP_ALERT_ENABLE	NPATH"events/temp_ena"
#define TEMP_ALERT_EV		NPATH"events/temp_ev"
#define OVER_TEMP_ENABLE	NPATH"events/overt_sde"
#define OVER_TEMP_EV		NPATH"events/overt_ev"

#define FAN_ENABLE_EV		NPATH"events/fan_ena"
#define FAN_STATUS_EV		NPATH"events/fan_ev"
#define FAN1_ALARM_CNTRL	"%s-002c/fan%d_alarm"
#define FAN2_ALARM_CNTRL	"%s-002f/fan%d_alarm"
#define FANS_NUMBER		4

#define PS_PRESENT		NPATH"power/ps%d_prsnt"
#define PS_OK			NPATH"power/ps%d_ok"
#define PS_INT			NPATH"power/ps%d_int"

#define FP_SFP_PRES_FMT		NPATH"sfp%d/pres"
#define FP_SFP_RXLOS_FMT	NPATH"sfp%d/rx_los"
#define FP_SFP_TXDIS_FMT	NPATH"sfp%d/tx_dis"
#define FP_SFP_SPEED_FMT	NPATH"diag/sfp%d/speed"
#define FP_SFP_TEMP_FMT	NPATH"diag/sfp%d/temp"
#define FP_LED_ACT_FMT		NPATH"leds/s%d_act"
#define FP_LED_COL_FMT		NPATH"leds/s%d_col"

#define MGMT_LED_ACT		NPATH"leds/mphy_act"
#define MGMT_LED_COL		NPATH"leds/mphy_col"

#define MGMT_IFACE_RX_STATS	CPATH"net/eth2/statistics/rx_bytes"
#define MGMT_IFACE_TX_STATS	CPATH"net/eth2/statistics/tx_bytes"

#define LED_CLR_GREEN		0x0
#define LED_CLR_YELLOW		0x1
#define LED_CLR_ORANGE		0x2
#define LED_CLR_RED		0x3
#define LED_CLR_OFF		0x3

#define LED_CNTRL_OFF		0x0
#define LED_CNTRL_ON		0x1
#define LED_CNTRL_BLINKING	0x3

#define MGMT_ALL_PHYS_STR       "all"
#define MGMT_ALL_STR            MGMT_ALL_PHYS_STR
#define MGMT_PIM_PHYS_STR       "pim"
#define MGMT_MGMT_PHYS_STR      "mgmt"
#define MGMT_FP_PHYS_STR        "fp"

#define DEBUG                   1

#define debug2(level, fmt, args...)      if (level <= dbg_mode) printf(fmt, ##args)
#ifdef DEBUG
#define debug(level, fmt, args...)       if (level <= dbg_mode) printf(fmt, ##args)
#else
#define debug(level, fmt, args...)
#endif

enum port_media_mode_e {
	PORT_MEDIA_UNKNOWN, /** Unknown/default mode */
	PORT_MEDIA_SR,      /** Optical */
	PORT_MEDIA_CX,      /** Passive copper */
	PORT_MEDIA_1000X,   /** 1000Base-X */
};

extern uint8_t dbg_mode;

typedef struct mgmt_led_info_s {
    int led_act;
    int led_col;
    int rx_stats;
    int tx_stats;
} mgmt_led_info_t;

typedef struct led_info_s {
    int sfp_pres;
    int sfp_rxlos;
    int sfp_txdis;
    int sfp_speed;
    int sfp_temperature;
    int media_type;
    int led_act;
    int led_col;
} led_info_t;

typedef struct sfp_status_s {
    unsigned long media_type;
    unsigned long sfp_pres;
    unsigned long sfp_rxlos;
    unsigned long speed;
    int sfp_tx_force_disable;
    uint8_t activity;
    int8_t delay;
} sfp_status_t;

typedef struct sys_leds_info_s {
    int sys_led_col;
    int sys_led_mode;
    int pwr_led_col[2];
    int pwr_led_mode[2];
    int pim_sys_led_col[2];
    int pim_sys_led_mode[2];
    int pim_tm_alert[2];
    int pim_tm_overt[2];
    int pim_temp_ev[2];
    int pim_sw_rst[2];
    int fan_alarm[8];
    int overtemp_ev;
    int temp_ev;
    int fan_ev;
    int pwr_pres[2];
    int pwr_int[2];
    int pwr_ok[2];
} sys_leds_info_t;

typedef struct io_card_power_s {
    int io_card_pe;
    int io_card_hp_pe;
} io_card_power_t;

#endif /* __LEDS_MGMT_H_ */
