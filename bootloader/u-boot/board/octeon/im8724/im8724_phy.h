#ifndef _IM8724_PHY_H_
#define _IM8724_PHY_H_

#include <common.h>

#define PIM1_PRESENT			0x1
#define PIM2_PRESENT			0x2

#define VSC8221_MODE_STAT_REG		0x01
#define VSC8221_MODE_LINK_STAT		(1 << 2)
#define VSC8221_AUX_STAT_REG		0x1c
#define VSC8221_AUX_SPEED_MASK		3
#define VSC8221_AUX_STAT_SPEED1		(1 << (VSC8221_AUX_SPEED_MASK+1))
#define VSC8221_AUX_STAT_SPEED2		(1 << (VSC8221_AUX_SPEED_MASK))
#define VSC8221_LED_CTL_REG		0x1b
#define VSC8221_LED_PIN2_MASK		0x0c00
#define VSC8221_LED_PIN2_CFG		0x0800
#define VSC8221_LED_PIN1_MASK		0x0300
#define VSC8221_LED_PIN1_CFG		0x0000
#define VSC8221_LED_PIN0_MASK		0x00c0
#define VSC8221_LED_PIN0_CFG		0x0000
#define VSC8221_LED_CFGS_MASK		0x003f
#define VSC8221_LED_CFGS_MODE		0x0033

struct cs_reg_modify {
        u16 reg;
        u16 set_bits;
};

int init_cs4343_phys(int pim);
int init_cs4318_phys(int sgmii_mode);

#endif
