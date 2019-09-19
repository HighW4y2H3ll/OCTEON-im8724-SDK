#ifndef __LEDS_MGMT_H_
#define __LEDS_MGMT_H_

/* Bus Allocation Control IM8724 Ext */
#define  CPLD_BA_CTL_SASL_MASK	  0x1f
#define  CPLD_BA_CTL_SASL_PIM0	  0x00
#define  CPLD_BA_CTL_SASL_PIM1	  0x01
#define  CPLD_BA_CTL_SASL_FAN	   0x1e

/* PIM IO Status register */
#define CPLD_ADDR_IO_STAT       0x15
#define CPLD_IOB_RDY	    (1 << 5)
#define CPLD_IOB_PRES	   (1 << 4)
#define CPLD_IOA_RDY	    (1 << 1)
#define CPLD_IOA_PRES	   (1 << 0)

/*
 * PIM IO Power Control register
 */
#define CPLD_ADDR_IO_PWR_CTL     0x16
#define CPLD_USB_PE	      (1 << 4)
#define CPLD_IOB_PE	      (1 << 3)
#define CPLD_IOB_HP_PE	   (1 << 2)
#define CPLD_IOA_PE	      (1 << 1)
#define CPLD_IOA_HP_PE	   (1 << 0)

/* LED Control Registers */
#define CPLD_LED_CFG	     0x48
#define CPLD_LD_COL	      (1 << 1)
#define CPLD_LD_ACT	      (1 << 0)

#define CPLD_ST_LED_CTL	  0x4A
#define CPLD_COL_1_1	     (1 << 7)
#define CPLD_COL_1_0	     (1 << 6)
#define CPLD_LC_1_1	      (1 << 5)
#define CPLD_LC_1_0	      (1 << 4)
#define CPLD_COL_0_1	     (1 << 3)
#define CPLD_COL_0_0	     (1 << 2)
#define CPLD_LC_0_1	      (1 << 1)
#define CPLD_LC_0_0	      (1 << 0)

#define CPLD_PWR_LED_CTL	 0x4B
#define CPLD_M_COL_1	     (1 << 7)
#define CPLD_M_COL_0	     (1 << 6)
#define CPLD_M_ACT_1	     (1 << 5)
#define CPLD_M_ACT_0	     (1 << 4)
#define CPLD_COL_1	       (1 << 3)
#define CPLD_COL_0	       (1 << 2)
#define CPLD_LC_1		(1 << 1)
#define CPLD_LC_0		(1 << 0)

#define CPLD_S0_LED_CTL	  0x4C
#define CPLD_S1_COL_1	    (1 << 7)
#define CPLD_S1_COL_0	    (1 << 6)
#define CPLD_S1_ACT_1	    (1 << 5)
#define CPLD_S1_ACT_0	    (1 << 4)
#define CPLD_S0_COL_1	    (1 << 3)
#define CPLD_S0_COL_0	    (1 << 2)
#define CPLD_S0_ACT_1	    (1 << 1)
#define CPLD_S0_ACT_0	    (1 << 0)

#define CPLD_S2_LED_CTL	  0x4D
#define CPLD_S3_COL_1	    (1 << 7)
#define CPLD_S3_COL_0	    (1 << 6)
#define CPLD_S3_ACT_1	    (1 << 5)
#define CPLD_S3_ACT_0	    (1 << 4)
#define CPLD_S2_COL_1	    (1 << 3)
#define CPLD_S2_COL_0	    (1 << 2)
#define CPLD_S2_ACT_1	    (1 << 1)
#define CPLD_S2_ACT_0	    (1 << 0)

#define LED_CLR_GREEN	    0x0
#define LED_CLR_YELLOW	   0x1
#define LED_CLR_ORANGE	   0x2
#define LED_CLR_RED	      0x3

#define LED_CNTRL_OFF	    0x0
#define LED_CNTRL_ON	     0x1
#define LED_CNTRL_BLINKING       0x3

#define SYS_LED_OK               0
#define SYS_LED_INIT             1
#define SYS_LED_TEMP_EV          2
#define SYS_LED_FAN_EV           3

/* These regs hold LED data for ports from IO cards */
#define CPLD_W0_LED_CTL	  0x4E
#define CPLD_W2_LED_CTL	  0x4F
#define CPLD_W4_LED_CTL	  0x50
#define CPLD_W6_LED_CTL	  0x51
#define CPLD_W8_LED_CTL	  0x52
#define CPLD_W10_LED_CTL	 0x53
#define CPLD_W12_LED_CTL	 0x54
#define CPLD_W14_LED_CTL	 0x55

/* IM13233 IO cards control regs */
#define CPLD_PIM_SIG_L	   0x0
#define CPLD_PIM_SIG_H	   0x1
#define CPLD_IM13233_SIG_VH      0x01
#define CPLD_IM13233_SIG_VL      0x4C
#define IM13233_SIG	      ((CPLD_IM13233_SIG_VH << 8) | CPLD_IM13233_SIG_VL)

#define CPLD_PIM_CPLD_REV	0x2
#define PIM_BRD_REV_MASK	 0x70
#define PIM_CPLD_REV_MASK	0x0F

#define CPLD_IM13233_RST_CTL     0x04
#define CPLD_IM13233_MI_AS       0x05
#define CPLD_IM13233_MISC_CSR    0x07
#define CPLD_IM13233_RDY	 (1 << 7)
#define CPLD_IM13233_TM_OVERT    (1 << 1)
#define CPLD_IM13233_TM_ALERT    (1 << 0)

#define CPLD_IM13233_SFP_PRES    0x08
#define CPLD_IM13233_SFP_TX_DIS  0x09
#define CPLD_IM13233_SFP_RX_LOS  0x0A
#define CPLD_IM13233_SFP_TX_FLT  0x0B
#define CPLD_IM13233_ST_LED_CTL  0x16
#define CPLD_IM13233_LC_1	(1 << 3)
#define CPLD_IM13233_LC_0	(1 << 2)
#define CPLD_IM13233_S_COL_1     (1 << 1)
#define CPLD_IM13233_S_COL_0     (1 << 0)

#define CPLD_IM13233_P_CTL       0x18
#define CPLD_IM13233_SPARE_R     0x1A
#define CPLD_IM13233_SD_CTL      0x1B

/* LEDs for XFI ports control */
#define CPLD_IM13233_LED_CFG     0x48
#define CPLD_IM13233_LED_COL     (1 << 1)
#define CPLD_IM13233_LED_ACT     (1 << 0)

#define CPLD_IM13233_LED_INDX_0  0x4A
#define CPLD_IM13233_LED_INDX_1  0x4B
#define CPLD_IM13233_LED_CTL_A   0x4C
#define CPLD_IM13233_LED_INDX_2  0x4D
#define CPLD_IM13233_LED_INDX_3  0x4E
#define CPLD_IM13233_LED_CTL_B   0x4F
#define CPLD_IM13233_LED_INDX_4  0x50
#define CPLD_IM13233_LED_INDX_5  0x51
#define CPLD_IM13233_LED_CTL_C   0x52
#define CPLD_IM13233_LED_INDX_6  0x53
#define CPLD_IM13233_LED_INDX_7  0x54
#define CPLD_IM13233_LED_CTL_D   0x55

/* IM13264 IO cards control regs */
#define CPLD_IM13264_SIG_VH      0x01
#define CPLD_IM13264_SIG_VL      0x57
#define IM13264_SIG              ((CPLD_IM13264_SIG_VH << 8) | CPLD_IM13264_SIG_VL)

#define CPLD_IM13264_RST_CTL     0x04
#define CPLD_IM13264_MI_AS       0x05
#define CPLD_IM13264_MISC_SR     0x07
#define CPLD_IM13264_MISC_CSR    CPLD_IM13264_MISC_SR
#define CPLD_IM13264_RDY         (1 << 7)
#define CPLD_IM13264_TM_OVERT    (1 << 1)
#define CPLD_IM13264_TM_ALERT    (1 << 0)

#define CPLD_IM13264_QSFP_PRES   0x08
#define CPLD_IM13264_QSFP_MODSEL 0x09
#define CPLD_IM13264_QSFP_LPMODE 0x0A
#define CPLD_IM13264_ST_LED_CTL  0x16
#define CPLD_IM13264_LC_1        (1 << 3)
#define CPLD_IM13264_LC_0        (1 << 2)
#define CPLD_IM13264_S_COL_1     (1 << 1)
#define CPLD_IM13264_S_COL_0     (1 << 0)

#define CPLD_IM13264_INT_STATE   0x0E
#define CPLD_IM13264_INT_EN      0x0F
#define CPLD_IM13264_P_CTL       0x18
#define CPLD_IM13264_SPARE_R     0x1A
#define CPLD_IM13264_SD_CTL      0x1B

/* LEDs for XLAUI ports control */
#define CPLD_IM13264_LED_CFG     0x48
#define CPLD_IM13264_LED_COL     (1 << 1)
#define CPLD_IM13264_LED_ACT     (1 << 0)

#define CPLD_IM13264_LED_INDX_0  0x4A
#define CPLD_IM13264_LED_INDX_1  0x4B
#define CPLD_IM13264_LED_CTL_A   0x4C

/* IM13262 IO cards control regs */
#define CPLD_IM13262_SIG_VH        0x01
#define CPLD_IM13262_SIG_VL        0x5F
#define IM13262_SIG                ((CPLD_IM13262_SIG_VH << 8) | CPLD_IM13262_SIG_VL)

#define CPLD_IM13262_RST_CTL       0x04
#define CPLD_IM13262_GB_PHYAD      0x05

#define CPLD_IM13262_MISC_SR       0x07
#define CPLD_IM13262_MISC_CSR      CPLD_IM13262_MISC_SR
#define CPLD_IM13262_RDY           (1 << 7)
#define CPLD_IM13262_TM_OVERT      (1 << 1)
#define CPLD_IM13262_TM_ALERT      (1 << 0)

#define CPLD_IM13262_SFP_PRES      0x08
#define CPLD_IM13262_SFP_TX_DIS    0x09
#define CPLD_IM13262_SFP_RX_LOS    0x0A
#define CPLD_IM13262_SFP_TX_FAULT  0x0B
#define CPLD_IM13262_SFP_RS        0x0C

#define CPLD_IM13262_INT_STATE     0x0E
#define CPLD_IM13262_INT_EN        0x0F

#define CPLD_IM13262_ST_LED_CTL    0x16
#define CPLD_IM13262_LC_1          (1 << 3)
#define CPLD_IM13262_LC_0          (1 << 2)
#define CPLD_IM13262_S_COL_1       (1 << 1)
#define CPLD_IM13262_S_COL_0       (1 << 0)

#define CPLD_IM13262_P_CTL         0x18

#define CPLD_IM13262_SPARE_R       0x1A
#define CPLD_IM13262_SD_CTL        0x1B
#define CPLD_IM13262_BA_CTL        0x1C

#define CPLD_IM13262_MI_BSL        0x1E
#define CPLD_IM13262_I2C_BSL       0x1F

#define CPLD_IM13262_UFM_CTL       0x40
#define CPLD_IM13262_UFM_ADDR      0x41
#define CPLD_IM13262_UFM_DATAIN_L  0x42
#define CPLD_IM13262_UFM_DATAIN_H  0x43
#define CPLD_IM13262_UFM_DATAOUT_L 0x44
#define CPLD_IM13262_UFM_DATAOUT_H 0x45

#define CPLD_IM13262_LED_CFG       0x48

#define CPLD_IM13262_LED_INDX_0    0x4A
#define CPLD_IM13262_LED_INDX_1    0x4B
#define CPLD_IM13262_LED_CTL_A     0x4C

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
#define BUF_SIZE		  128

#define ACTIVITY_UNDEF		  2
#define SPEED_UNDEF		  0
#define SPEED_1G		  1
#define SPEED_10M		  2
#define SPEED_100M		  3
#define SPEED_10G		  10
#define SPEED_40G		  40
#define SPEED_25G		  25

#define IFACE_OFF		  0
#define IFACE_ON		  1

#define CPLD_PIM_PRES_BIT(pim)			   \
	((pim) ? CPLD_IOB_PRES : CPLD_IOA_PRES)
#define CPLD_PIM_RDY_BIT(pim)			     \
	((pim) ? CPLD_IOB_RDY : CPLD_IOA_RDY)

#define CPLD_PIM_PRESENT(pim)			     \
	(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_PRES_BIT(pim))

#define CPLD_PIM_MISSING(pim)			     \
	(!(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_PRES_BIT(pim)))

#define CPLD_PIM_READY(pim)				\
	(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_RDY_BIT(pim))

#define CPLD_PIM_PWR_ENABLE(pim)			      \
	((pim) ? (CPLD_IOB_PE | CPLD_IOB_HP_PE) :	    \
		 (CPLD_IOA_PE | CPLD_IOA_HP_PE))

#define LED_CLR_GREEN		0x0
#define LED_CLR_YELLOW		0x1
#define LED_CLR_ORANGE		0x2
#define LED_CLR_OFF		0x3

#define LED_CNTRL_OFF		0x0
#define LED_CNTRL_ON		0x1
#define LED_CNTRL_BLINKING	0x3

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

#define DEBUG		   1

#ifdef DEBUG
#define debug(level, fmt, args...)       if (level <= dbg_mode) printf(fmt, ##args)
#else
#define debug(level, fmt, args...)
#endif

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
