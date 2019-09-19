#ifndef _IM8724_CPLD_H_
#define _IM8724_CPLD_H_

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

#define CPLD_ADDR_FAN_CS        0x09
#define CPLD_ADDR_FAN_DC        0x0A

/* System Faults Status Register */
#define CPLD_ADDR_FLT_STAT      0x0F
#define CPLD_PEX_ERR            (1 << 7)
#define CPLD_PEX_INTA           (1 << 6)
#define CPLD_HP_ALERT           (1 << 4)
#define CPLD_USB_OV             (1 << 3)
#define CPLD_CORE_ALERT         (1 << 2)
#define CPLD_TM_OVERT           (1 << 1)
#define CPLD_TM_ALERT           (1 << 0)

#define CPLD_ADDR_INT_EN_0      0x11
#define CPLD_ADDR_INT_STAT_0    0x10
#define CPLD_INT_OVERT_EV       (1 << 7)
#define CPLD_INT_MGT_PHY        (1 << 6)
#define CPLD_INT_IO_PR_DET      (1 << 5)
#define CPLD_INT_IOB_EV         (1 << 4)
#define CPLD_INT_IOA_EV         (1 << 3)
#define CPLD_INT_PS_EV          (1 << 2)
#define CPLD_INT_TEMP_EV        (1 << 1)
#define CPLD_INT_CS4318_EV      (1 << 0)

#define CPLD_ADDR_INT_STAT_1    0x12
#define CPLD_ADDR_INT_EN_1      0x13
#define CPLD_INT_FAN_EV         (1 << 6)
#define CPLD_INT_PEX_EV         (1 << 5)
#define CPLD_INT_MISC_FLT_EV    (1 << 4)
#define CPLD_INT_SFP_INT_3      (1 << 3)
#define CPLD_INT_SFP_INT_2      (1 << 2)
#define CPLD_INT_SFP_INT_1      (1 << 1)
#define CPLD_INT_SFP_INT_0      (1 << 0)

#define CPLD_STAT_0_TEMP_EV     (1 << 1)
#define CPLD_STAT_0_OVERT_EV    (1 << 7)
#define CPLD_STAT_1_FAN_EV      (1 << 6)

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
#define CPLD_LD_COL              (1 << 1)
#define CPLD_LD_ACT              (1 << 0)

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

#define SYS_LED_OK               0
#define SYS_LED_INIT             1
#define SYS_LED_TEMP_EV          2
#define SYS_LED_FAN_EV           3

/* These regs hold LED data for ports from IO cards */
#define CPLD_W0_LED_CTL          0x4E
#define CPLD_W2_LED_CTL          0x4F
#define CPLD_W4_LED_CTL          0x50
#define CPLD_W6_LED_CTL          0x51
#define CPLD_W8_LED_CTL          0x52
#define CPLD_W10_LED_CTL         0x53
#define CPLD_W12_LED_CTL         0x54
#define CPLD_W14_LED_CTL         0x55

#define CPLD_PIM_SIG_L           0x0
#define CPLD_PIM_SIG_H           0x1
#define CPLD_PIM_CPLD_REV        0x2
#define PIM_BRD_REV_MASK         0x70
#define PIM_CPLD_REV_MASK        0x0F

/* IM13233 IO cards control regs */
#define CPLD_IM13233_SIG_VH      0x01
#define CPLD_IM13233_SIG_VL      0x4C
#define IM13233_SIG              ((CPLD_IM13233_SIG_VH << 8) | CPLD_IM13233_SIG_VL)

#define CPLD_IM13233_RST_CTL     0x04
#define CPLD_IM13233_MI_AS       0x05
#define CPLD_IM13233_MISC_CSR    0x07
#define CPLD_IM13233_RDY         (1 << 7)
#define CPLD_IM13233_TM_OVERT    (1 << 1)
#define CPLD_IM13233_TM_ALERT    (1 << 0)

#define CPLD_IM13233_SFP_PRES    0x08
#define CPLD_IM13233_SFP_TX_DIS  0x09
#define CPLD_IM13233_SFP_RX_LOS  0x0A
#define CPLD_IM13233_SFP_TX_FLT  0x0B
#define CPLD_IM13233_ST_LED_CTL  0x16
#define CPLD_IM13233_LC_1        (1 << 3)
#define CPLD_IM13233_LC_0        (1 << 2)
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


/* IM13026, FAN, board cpld registers */
#define CPLD_FAN_ADDR_FC_SS            0x04
#define CPLD_FAN_ADDR_FC_STAT          0x05
#define CPLD_FAN_ADDR_FAN_PWR_EN       0x07
#define CPLD_FAN_ADDR_FAN_PWR_ST       0x08

/* IM8724 I2C defines */
#define CPLD_ADDR_I2C_BLOCK             0x38
#define CPLD_ADDR_I2C_CSR               (CPLD_ADDR_I2C_BLOCK + 0)
#define CPLD_ADDR_I2C_DA                (CPLD_ADDR_I2C_BLOCK + 1)
#define CPLD_ADDR_I2C_DATA              (CPLD_ADDR_I2C_BLOCK + 2)
#define CPLD_ADDR_I2C_ST                (CPLD_ADDR_I2C_BLOCK + 3)
#define CPLD_ADDR_I2C_ADDR_0            (CPLD_ADDR_I2C_BLOCK + 4)
#define CPLD_ADDR_I2C_ADDR_1            (CPLD_ADDR_I2C_BLOCK + 5)
#define CPLD_ADDR_I2C_CTRL              (CPLD_ADDR_I2C_BLOCK + 6)

#define CPLD_I2C_CTL_R_1000             0x00
#define CPLD_I2C_CTL_R_500              0x10
#define CPLD_I2C_CTL_R_100              0x20
#define CPLD_I2C_CTL_R_50               0x30

#define CPLD_I2C_STAT_ACT               0x01

#define CPLD_I2C_CSR_WRITE              0x10
#define CPLD_I2C_CSR_READ               0x00
#define CPLD_I2C_CSR_ACK_POLL           0x20
#define CPLD_I2C_CSR_ST                 0x01
#define CPLD_I2C_STAT_MASK              0x07

#define CPLD_ADDR_SFP_PRES              0x0B
#define CPLD_ADDR_SFP_RX_LOS		    0x0D

#endif /* _IM8724_CPLD_H_ */
