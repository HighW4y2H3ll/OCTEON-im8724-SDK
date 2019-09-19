#ifndef __LEDS_MGMT_H_
#define __LEDS_MGMT_H_

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

#define ACTIVITY_UNDEF	    2
#define SPEED_UNDEF		  0
#define SPEED_1G		  1
#define SPEED_10M		  2
#define SPEED_100M		  3
#define SPEED_10G		  10
#define SPEED_40G		  40
#define SPEED_25G		  25

#define LED_CLR_GREEN		0x0
#define LED_CLR_YELLOW		0x1
#define LED_CLR_ORANGE		0x2
#define LED_CLR_OFF		0x3

#define SFP_RXLOS_FLAG		0x01
#define SFP_PRES_FLAG		0x02
#define LED_ACT_FLAG		0x04
#define LED_COL_FLAG		0x08

#define INFO_EEPROM_ADDR		0x50
#define QSFP_TX_RX_LOSS_OFFSET	0x03
#define QSFP_RX1_LOS		(1 << 0)
#define QSFP_RX2_LOS		(1 << 1)
#define QSFP_RX3_LOS		(1 << 2)
#define QSFP_RX4_LOS		(1 << 3)
#define QSFP_TX1_LOS		(1 << 4)
#define QSFP_TX2_LOS		(1 << 5)
#define QSFP_TX3_LOS		(1 << 6)
#define QSFP_TX4_LOS		(1 << 7)

typedef struct {
	cvmx_bgxx_cmrx_rx_stat0_t       rx_stat0;
	cvmx_bgxx_cmrx_tx_stat6_t       tx_stat6;
	cvmx_bgxx_cmrx_tx_stat7_t       tx_stat7;
	cvmx_bgxx_cmrx_tx_stat8_t       tx_stat8;
	cvmx_bgxx_cmrx_tx_stat9_t       tx_stat9;
	cvmx_bgxx_cmrx_tx_stat10_t      tx_stat10;
	cvmx_bgxx_cmrx_tx_stat11_t      tx_stat11;
	cvmx_bgxx_cmrx_tx_stat12_t      tx_stat12;
	cvmx_bgxx_cmrx_tx_stat13_t      tx_stat13;
} lmac_pdu_stats_t;


typedef struct mgmt_led_info_s {
	int led_act;
	int led_col;
	int rx_stats;
	int tx_stats;
} mgmt_led_info_t;

typedef struct led_info_s {
	int sfp_pres;
	int sfp_rxlos;
	int led_act;
	int led_col;
} led_info_t;

typedef struct sfp_status_s {
	unsigned long sfp_pres;
	unsigned long sfp_rxlos;
	uint8_t activity;
	uint8_t speed;
} sfp_status_t;

int pim_leds_init(void);
void pim_leds_mgmt(int slot);

int fp_leds_init(void);
void fp_leds_mgmt(void);

int sys_leds_init(void);
void sys_leds_mgmt(void);

int leds_init(void);
void leds_mgmt(void);

#endif /* __LEDS_MGMT_H_ */
