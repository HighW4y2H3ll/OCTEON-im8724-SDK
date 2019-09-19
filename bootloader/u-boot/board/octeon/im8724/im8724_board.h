#ifndef _IM8724_BOARD_H
#define _IM8724_BOARD_H


/* defines for BGX interfaces,
 *with ether mode reconfiguration */
enum bgx_iface_e {
        BGX_MS_IFACE_0,
        BGX_MS_IFACE_1,
        BGX_MS_IFACE_2,
        BGX_MS_IFACE_3,
        BGX_MS_IFACE_4,
        BGX_MS_IFACE_5,
	BGX_MS_IFACE_MAX
};

enum bgx_iface_mode_e {
	IFACE_MODE_DIS,
	IFACE_MODE_1G,
	IFACE_MODE_10G,
	IFACE_MODE_40G,
	IFACE_MODE_25G,
	IFACE_MODE_10GB		/*  10G Bypass mode for Gearbox.  */
};

/* Vitesse 8221 registers */
#define VSC8221_PHY_CTL_REG             0x17
#define VSC8221_PHY_SGMII_MODE          0xba20

#define VSC8221_MODE_CTL_REG            0x0
#define VSC8221_CTL_SW_RESET            (1 << 15)
#define VSC8221_CTL_MSB_SPEED           (1 << 6)
#define VSC8221_CTL_LSB_SPEED           (1 << 13)
#define VSC8221_CTL_FULL_DUPLEX         (1 << 8)
#define VSC8221_CTL_AUTONG_EN           (1 << 12)

#define VSC8221_STATUS_REG              0x1
#define VSC8221_LINK_STATUS             (1 << 2)

#define VSC8221_ISR_MASK_REG            0x19
#define VSC8221_ISR_MSK_PIN_ENA         (1 << 15)
#define VSC8221_ISR_MSK_LINK_STATE      (1 << 13)

#define VSC8221_ISR_STATUS_REG          0x1a
#define VSC8221_ISR_PENDING             (1 << 15)
#define VSC8221_ISR_LINK_STATE          (1 << 13)

#define VSC8221_AUXIL_CS_REG            0x1c

/* Fan management application thresholds 
 * and default settings */
#define CPU_SET				40
#define MAX_TEMP_ALARM_THRESH		87
#define MAX_TEMP_CRITICAL_THRESH	95
#define DEFAULT_FAN_RPM_VALUE		0x40
#define MAX_FAN_RPM_VALUE		0xff

/* IM13026, FAN, board cpld registers */
#define CPLD_FAN_ADDR_SIGNATURE 	0x00
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
#define MAX6699_DEV_ADDR		0x4c
#define MAX6699_REG_TEMP_CRIT(nr)	(0x20 + nr)
#define ADM1032_DEV_ADDR		0x4c
#define ADM1032_REG_R_LOCAL_TEMP	0x00
#define ADM1032_REG_R_LOCAL_HIGH	0x05
#define ADM1032_REG_W_LOCAL_HIGH	0x0b
#define ADM1032_REG_R_REMOTE_TEMPH	0x01
#define ADM1032_REG_R_REMOTE_HIGHH	0x07
#define ADM1032_REG_W_REMOTE_HIGHH	0x0d

#define MGMT_PHY_MODE_10M_FD		"10/full"
#define MGMT_PHY_MODE_10M_HD		"10/half"
#define MGMT_PHY_MODE_100M_FD		"100/full"
#define MGMT_PHY_MODE_100M_HD		"100/half"
#define MGMT_PHY_MODE_1000M_FD		"1000/full"
#define MGMT_PHY_MODE_1000M_HD		"1000/half"
#define MGMT_PHY_MODE_AUTO		"auto"

#define VSC8221_MODE_CTL_REG		0x0
#define VSC8221_CTL_SW_RESET		(1 << 15)
#define VSC8221_CTL_MSB_SPEED		(1 << 6)
#define VSC8221_CTL_LSB_SPEED		(1 << 13)
#define VSC8221_CTL_FULL_DUPLEX		(1 << 8)
#define VSC8221_CTL_AUTONG_EN		(1 << 12)

#define CPLD_PIM_PRES_BIT(pim)				 \
	((pim) ? CPLD_IOB_PRES : CPLD_IOA_PRES)
#define CPLD_PIM_RDY_BIT(pim)				  \
	((pim) ? CPLD_IOB_RDY : CPLD_IOA_RDY)

#define CPLD_PIM_PRESENT(pim)				  \
	(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_PRES_BIT(pim))

#define CPLD_PIM_MISSING(pim)				  \
	(!(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_PRES_BIT(pim)))

#define CPLD_PIM_READY(pim)				   \
	(cpld_rd(CPLD_ADDR_IO_STAT) & CPLD_PIM_RDY_BIT(pim))

#define CPLD_PIM_PWR_ENABLE(pim)			      \
	((pim) ? (CPLD_IOB_PE | CPLD_IOB_HP_PE) :	     \
		 (CPLD_IOA_PE | CPLD_IOA_HP_PE))

uint16_t pim_signature[MAX_IM8724_MODULES] = { 0, 0 };
uint16_t pim_mgbmode[MAX_IM8724_MODULES] = { 0, 0 };       /*  Marvell GB mode 25G = 0 / 10G = 1  */

#define IS_CPLD_IM13233(slot) (pim_signature[slot] == IM13233_SIG)
#define IS_CPLD_IM13264(slot) (pim_signature[slot] == IM13264_SIG)
#define IS_CPLD_IM13262(slot) (pim_signature[slot] == IM13262_SIG)

#define IS_CPLD_IM_UNK(slot) (!IS_CPLD_IM13233(slot) && \
					!IS_CPLD_IM13264(slot) && \
					!IS_CPLD_IM13262(slot))

#define GET_MAX_SLOT_PORTS(slot) (IS_CPLD_IM13233(slot) \
						? MAX_SFP_PORTS_PER_MODULE \
						: (IS_CPLD_IM13264(slot) \
							? MAX_QSFP_PORTS_PER_MODULE \
							: (IS_CPLD_IM13262(slot) \
								? MAX_SFP28_PORTS_PER_MODULE \
								: MAX_SFP_PORTS_PER_MODULE)))

#define CPLD_IM132XX(slot, name) (IS_CPLD_IM13233(slot) \
						? CPLD_IM13233_ ## name \
						: (IS_CPLD_IM13264(slot) \
							? CPLD_IM13264_ ## name \
							: (IS_CPLD_IM13262(slot) \
								? CPLD_IM13262_ ## name \
								: CPLD_IM13233_ ## name)))

#ifdef CONFIG_PHY_MARVELL88X5113
int set_5113_led ( int );
int initialize_5113 ( int );
int check_5113_presence ( int );
#else
int set_5113_led ( int pim ) { return 0 ; };
int initialize_5113 ( int pim ) { return 0 ; };
int check_5113_presence ( int pim ) { return 0 ; };
#endif

#define FANS_UNINITIALIZED (-1)

#endif // _IM8724_BOARD_H
