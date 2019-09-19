#ifndef _IM8724_CPLD_H__
#define _IM8724_CPLD_H__

/*
 * CPLD addresses.
 */

#define CPLD_ADDR_PID_R         0x03
#define CPLD_ADDR_RST_CTL_0     0x04
#define CPLD_BIT_RE_BOOT        0x40
#define CPLD_BIT_ZL_CLK         0x10
#define CPLD_BIT_CS4318         0x08
#define CPLD_BIT_MGT_PHY        0x04
#define CPLD_BIT_PEX_PCIE       0x02
#define CPLD_BIT_CN_CPU         0x01

#define CPLD_ADDR_RST_CTL_1     0x05

#define CPLD_ADDR_MI_CSR        0x30
#define CPLD_ADDR_MI_MCTL       0x31
#define CPLD_ADDR_MI_AD_0       0x32
#define CPLD_ADDR_MI_AD_1       0x33
#define CPLD_ADDR_MI_DEVAD      0x34
#define CPLD_ADDR_MI_PRTAD      0x35

#define CPLD_MDIO_CLAUSE22      1
#define CPLD_MDIO_CLAUSE45      0

#define CPLD_ADDR_PS_CSR        0x07
#define CPLD_PS1_PRSNT          (1 << 7)
#define CPLD_PS1_OK             (1 << 6)
#define CPLD_PS1_INT            (1 << 5)
#define CPLD_PS1_OFF            (1 << 4)
#define CPLD_PS0_PRSNT          (1 << 3)
#define CPLD_PS0_OK             (1 << 2)
#define CPLD_PS0_INT            (1 << 1)
#define CPLD_PS0_OFF            (1 << 0)

#define CPLD_ADDR_MI_AS         0x06
#define CPLD_ADDR_MISC_CSR      0x08
#define CPLD_ADDR_FAN_CS        0x09
#define CPLD_ADDR_FAN_DC        0x0A
#define CPLD_ADDR_SFP_PRES      0x0B
#define CPLD_ADDR_SFP_TX_DIS    0x0C
#define CPLD_ADDR_SFP_RX_LOS    0x0D
#define CPLD_ADDR_SFP_TX_FAULT  0x0E
#define CPLD_ADDR_INT_STAT_0    0x10
#define CPLD_ADDR_INT_EN_0      0x11
#define CPLD_ADDR_INT_STAT_1    0x12
#define CPLD_ADDR_INT_EN_1      0x13
#define CPLD_ADDR_WD_CTL        0x14
#define CPLD_ADDR_P_CTL_0       0x18
#define CPLD_ADDR_P_CTL_1       0x19
#define CPLD_ADDR_STRAP_R6      0x1A

/* System Faults Status Register */
#define CPLD_ADDR_FLT_STAT      0x0F
#define CPLD_PEX_ERR            (1 << 7)
#define CPLD_PEX_INTA           (1 << 6)
#define CPLD_HP_ALERT           (1 << 4)
#define CPLD_USB_OV             (1 << 3)
#define CPLD_CORE_ALERT         (1 << 2)
#define CPLD_TM_OVERT           (1 << 1)
#define CPLD_TM_ALERT           (1 << 0)

/* Bus Allocation Control IM8724 Ext */
#define  CPLD_BA_CTL_SASL_MASK          0x1f
#define  CPLD_BA_CTL_SASL_PIM0          0x00
#define  CPLD_BA_CTL_SASL_PIM1          0x01
#define  CPLD_BA_CTL_SASL_FAN           0x1e

/* PIM IO Status register */
#define CPLD_ADDR_IO_STAT       0x15
#define CPLD_IOB_RDY            (1 << 5)
#define CPLD_IOB_PRES           (1 << 4)
#define CPLD_IOA_RDY            (1 << 1)
#define CPLD_IOA_PRES           (1 << 0)

/*
 * PIM IO Power Control register
 */
#define CPLD_ADDR_IO_PWR_CTL     0x16
#define CPLD_USB_PE              (1 << 4)
#define CPLD_IOB_PE              (1 << 3)
#define CPLD_IOB_HP_PE           (1 << 2)
#define CPLD_IOA_PE              (1 << 1)
#define CPLD_IOA_HP_PE           (1 << 0)

/* LED Control Registers */
#define CPLD_LED_CFG             0x48
#define CPLD_C_COL               (1 << 3)
#define CPLD_C_ACT               (1 << 2)
#define CPLD_V_COL               (1 << 1)
#define CPLD_V_ACT               (1 << 0)

#define CPLD_ST_LED_CTL          0x4A
#define CPLD_COL_1_1             (1 << 7)
#define CPLD_COL_1_0             (1 << 6)
#define CPLD_LC_1_1              (1 << 5)
#define CPLD_LC_1_0              (1 << 4)
#define CPLD_COL_0_1             (1 << 3)
#define CPLD_COL_0_0             (1 << 2)
#define CPLD_LC_0_1              (1 << 1)
#define CPLD_LC_0_0              (1 << 0)

#define CPLD_PWR_LED_CTL         0x4B
#define CPLD_M_COL_1             (1 << 7)
#define CPLD_M_COL_0             (1 << 6)
#define CPLD_M_ACT_1             (1 << 5)
#define CPLD_M_ACT_0             (1 << 4)
#define CPLD_COL_1               (1 << 3)
#define CPLD_COL_0               (1 << 2)
#define CPLD_LC_1                (1 << 1)
#define CPLD_LC_0                (1 << 0)

#define CPLD_S0_LED_CTL          0x4C
#define CPLD_S1_COL_1            (1 << 7)
#define CPLD_S1_COL_0            (1 << 6)
#define CPLD_S1_ACT_1            (1 << 5)
#define CPLD_S1_ACT_0            (1 << 4)
#define CPLD_S0_COL_1            (1 << 3)
#define CPLD_S0_COL_0            (1 << 2)
#define CPLD_S0_ACT_1            (1 << 1)
#define CPLD_S0_ACT_0            (1 << 0)

#define CPLD_S2_LED_CTL          0x4D
#define CPLD_S3_COL_1            (1 << 7)
#define CPLD_S3_COL_0            (1 << 6)
#define CPLD_S3_ACT_1            (1 << 5)
#define CPLD_S3_ACT_0            (1 << 4)
#define CPLD_S2_COL_1            (1 << 3)
#define CPLD_S2_COL_0            (1 << 2)
#define CPLD_S2_ACT_1            (1 << 1)
#define CPLD_S2_ACT_0            (1 << 0)

#define LED_CLR_GREEN            0x0
#define LED_CLR_YELLOW           0x1
#define LED_CLR_ORANGE           0x2
#define LED_CLR_RED              0x3

#define LED_CNTRL_OFF            0x0
#define LED_CNTRL_ON             0x1
#define LED_CNTRL_BLINKING       0x3

/* These regs hold LED data for ports from IO cards */
#define CPLD_W0_LED_CTL          0x4E
#define CPLD_W2_LED_CTL          0x4F
#define CPLD_W4_LED_CTL          0x50
#define CPLD_W6_LED_CTL          0x51
#define CPLD_W8_LED_CTL          0x52
#define CPLD_W10_LED_CTL         0x53
#define CPLD_W12_LED_CTL         0x54
#define CPLD_W14_LED_CTL         0x55

/* Fan CPLD addresses */
#define CPLD_FAN_ADDR_SIG_L		0x9e00
#define CPLD_FAN_ADDR_SIG_H		0x9e01
#define CPLD_FAN_ADDR_REV		0x9e02
#define CPLD_FAN_ADDR_FC_SS		0x9e04
#define CPLD_FAN_ADDR_FC_STAT		0x9e05
#define CPLD_FAN_ADDR_FAN_PWR_EN	0x9e07
#define CPLD_FAN_ADDR_FAN_PWR_ST	0x9e08

/*
 * Bay A/B (PIM 0/1) CPLD addresses
 */
#define CPLD_ADDR_PIM0_SIG_L		0x8000
#define CPLD_ADDR_PIM0_SIG_H		0x8001
#define CPLD_ADDR_PIM0_REV		0x8002
#define CPLD_ADDR_PIM1_SIG_L		0x8100
#define CPLD_ADDR_PIM1_SIG_H		0x8101
#define CPLD_ADDR_PIM1_REV		0x8102

#define IM8724_I2C_SFP_CLIENTS_START	0

#define CPLD_PIM_PWR_ENABLE(pim)			     		\
        ((pim) ? (CPLD_IOB_PE | CPLD_IOB_HP_PE) :			\
                 (CPLD_IOA_PE | CPLD_IOA_HP_PE))

#define IM_DEV_SFP_ATTR(_num)						\
	IM_DEV_N_RO_ATTR(sfp##_num##_pres, pres,			\
			 CPLD_ADDR_SFP_PRES, 0x1, _num);		\
	IM_DEV_N_RW_ATTR(sfp##_num##_tx_dis, tx_dis,			\
			 CPLD_ADDR_SFP_TX_DIS, 0x1, _num);		\
	IM_DEV_N_RO_ATTR(sfp##_num##_tx_fault, tx_fault,		\
			 CPLD_ADDR_SFP_TX_FAULT, 0x1, _num);		\
	IM_DEV_N_RO_ATTR(sfp##_num##_rx_los, rx_los,			\
			 CPLD_ADDR_SFP_RX_LOS, 0x1, _num);		\
	IM_DEV_N_RW_ATTR(sfp##_num##_int_en, int_en,			\
			 CPLD_ADDR_INT_EN_1, 0x1, _num);		\
	static struct attribute *im_##sfp##_num##_attributes[] = {	\
		&im_dev_attr_sfp##_num##_pres.dev_attr.attr,		\
		&im_dev_attr_sfp##_num##_tx_dis.dev_attr.attr,		\
		&im_dev_attr_sfp##_num##_tx_fault.dev_attr.attr,	\
		&im_dev_attr_sfp##_num##_rx_los.dev_attr.attr,		\
		&im_dev_attr_sfp##_num##_int_en.dev_attr.attr,		\
		NULL							\
	};								\
	static const struct attribute_group im_##sfp##_num##_group = {	\
		.name = "sfp" #_num,					\
		.attrs = im_sfp##_num##_attributes			\
	}

#define IM_DIAG_SFP_ATTR(_num)						\
	IM_SFP_SPEED_N_RW_ATTR(sfp##_num##_speed, speed, (IM8724_I2C_SFP_CLIENTS_START+_num));\
	IM_SFP_MEDIA_N_RO_ATTR(sfp##_num##_media, media, (IM8724_I2C_SFP_CLIENTS_START+_num));\
	IM_SFP_TEMP_N_RO_ATTR(sfp##_num##_temp, temp, (IM8724_I2C_SFP_CLIENTS_START+_num));\
	static struct attribute *im_diag_##sfp##_num##_attributes[] = {	\
		&im_sfp_speed_attr_sfp##_num##_speed.dev_attr.attr,	\
		&im_sfp_media_attr_sfp##_num##_media.dev_attr.attr,	\
		&im_sfp_temp_attr_sfp##_num##_temp.dev_attr.attr,	\
		NULL                                                    \
	};								\
	static const struct attribute_group im_diag_sfp##_num##_group = {  \
		.name = "sfp" #_num,					\
		.attrs = im_diag_sfp##_num##_attributes			\
	}

#define OCTEON_MDIO_BUS0_BASE		0x8001180000003880
#define OCTEON_MDIO_BUS1_BASE		0x8001180010003880
#define OCTEON_MDIO_BUS2_BASE		0x8001180100003880
#define OCTEON_MDIO_BUS3_BASE		0x8001180110003880

#endif /* _IM8724_CPLD_H__ */
