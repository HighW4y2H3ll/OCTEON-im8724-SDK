#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/platform_data/niagara.h>
#include <linux/of_platform.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/platform_data/at24.h>
#include <linux/phy.h>

#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-helper.h>
#include <asm/octeon/cvmx-bgxx-defs.h>

#include "niagara_cpld.h"
#include "im8724_cpld.h"

#define IM8724_10G_PORTS_COUNT            4

IM_DEV_SFP_ATTR(0);
IM_DEV_SFP_ATTR(1);
IM_DEV_SFP_ATTR(2);
IM_DEV_SFP_ATTR(3);

IM_DEV_RO_ATTR(cpld_sig_l,		CPLD_ADDR_CPLD_SIG_L,	0xff,	0);
IM_DEV_RO_ATTR(cpld_sig_h,		CPLD_ADDR_CPLD_SIG_H,	0xff,	0);
IM_DEV_RO_ATTR(cpld_rev,		CPLD_ADDR_CPLD_REV,	0xf,	0);
IM_DEV_RO_ATTR(board_rev,		CPLD_ADDR_CPLD_REV,	0x7,	4);
IM_DEV_RO_ATTR(prod_id,			CPLD_ADDR_PID_R,	0x3,	0);
IM_DEV_RW_ATTR(pwr_down,		CPLD_ADDR_MISC_CSR,	0x1,	0);
IM_DEV_RW_ATTR(bdsl,			CPLD_ADDR_MISC_CSR,	0x2,	2);
IM_DEV_RW_ATTR(cn_failsf,		CPLD_ADDR_MISC_CSR,	0x1,	4);
IM_DEV_RW_ATTR(flash_we,		CPLD_ADDR_MISC_CSR,	0x1,	6);
IM_DEV_RW_ATTR(pg_sl,			CPLD_ADDR_MISC_CSR,	0x1,	7);
IM_DEV_RW_ATTR(wde,			CPLD_ADDR_WD_CTL,	0x3,	6);
IM_DEV_RO_ATTR(ioa_card_pres,		CPLD_ADDR_IO_STAT,	0x1,	0);
IM_DEV_RO_ATTR(ioa_card_ready,		CPLD_ADDR_IO_STAT,	0x1,	1);
IM_DEV_RO_ATTR(iob_card_pres,		CPLD_ADDR_IO_STAT,	0x1,	4);
IM_DEV_RO_ATTR(iob_card_ready,		CPLD_ADDR_IO_STAT,	0x1,	5);
IM_DEV_RO_ATTR(ioa_card_sig_l,		CPLD_ADDR_PIM0_SIG_L,	0xff,	0);
IM_DEV_RO_ATTR(ioa_card_sig_h,		CPLD_ADDR_PIM0_SIG_H,	0xff,	0);
IM_DEV_RO_ATTR(iob_card_sig_l,		CPLD_ADDR_PIM1_SIG_L,	0xff,	0);
IM_DEV_RO_ATTR(iob_card_sig_h,		CPLD_ADDR_PIM1_SIG_H,	0xff,	0);
IM_DEV_RO_ATTR(ioa_card_board_rev,	CPLD_ADDR_PIM0_REV,	0x7,	4);
IM_DEV_RO_ATTR(iob_card_board_rev,	CPLD_ADDR_PIM1_REV,	0x7,	4);
IM_DEV_RO_ATTR(ioa_card_cpld_rev,	CPLD_ADDR_PIM0_REV,	0xf,	0);
IM_DEV_RO_ATTR(iob_card_cpld_rev,	CPLD_ADDR_PIM1_REV,	0xf,	0);

static struct attribute *im8724_misc_attributes[] = {
	&im_dev_attr_cpld_sig_l.dev_attr.attr,
	&im_dev_attr_cpld_sig_h.dev_attr.attr,
	&im_dev_attr_cpld_rev.dev_attr.attr,
	&im_dev_attr_board_rev.dev_attr.attr,
	&im_dev_attr_prod_id.dev_attr.attr,
	&im_dev_attr_pwr_down.dev_attr.attr,
	&im_dev_attr_bdsl.dev_attr.attr,
	&im_dev_attr_cn_failsf.dev_attr.attr,
	&im_dev_attr_flash_we.dev_attr.attr,
	&im_dev_attr_pg_sl.dev_attr.attr,
	&im_dev_attr_wde.dev_attr.attr,
	&im_dev_attr_ioa_card_pres.dev_attr.attr,
	&im_dev_attr_ioa_card_ready.dev_attr.attr,
	&im_dev_attr_iob_card_pres.dev_attr.attr,
	&im_dev_attr_iob_card_ready.dev_attr.attr,
	&im_dev_attr_ioa_card_sig_l.dev_attr.attr,
	&im_dev_attr_ioa_card_sig_h.dev_attr.attr,
	&im_dev_attr_iob_card_sig_l.dev_attr.attr,
	&im_dev_attr_iob_card_sig_h.dev_attr.attr,
	&im_dev_attr_ioa_card_board_rev.dev_attr.attr,
	&im_dev_attr_iob_card_board_rev.dev_attr.attr,
	&im_dev_attr_ioa_card_cpld_rev.dev_attr.attr,
	&im_dev_attr_iob_card_cpld_rev.dev_attr.attr,
	NULL
};

static const struct attribute_group im_misc_group = {
	.name = "misc",
	.attrs = im8724_misc_attributes
};

IM_DEV_RW_ATTR(max_speed,		CPLD_FAN_ADDR_FC_SS,	0x1,	0);
IM_DEV_RO_ATTR(alert0,			CPLD_FAN_ADDR_FC_STAT,	0x1,	0);
IM_DEV_RO_ATTR(alert1,			CPLD_FAN_ADDR_FC_STAT,	0x1,	1);
IM_DEV_RW_ATTR(power0,			CPLD_FAN_ADDR_FAN_PWR_EN, 0x1,	0);
IM_DEV_RW_ATTR(power1,			CPLD_FAN_ADDR_FAN_PWR_EN, 0x1,	1);
IM_DEV_RO_ATTR(pg0,			CPLD_FAN_ADDR_FAN_PWR_ST, 0x1,	0);
IM_DEV_RO_ATTR(pg1,			CPLD_FAN_ADDR_FAN_PWR_ST, 0x1,	1);
IM_DEV_RO_ATTR(flt0,			CPLD_FAN_ADDR_FAN_PWR_ST, 0x1,	4);
IM_DEV_RO_ATTR(flt1,			CPLD_FAN_ADDR_FAN_PWR_ST, 0x1,	5);

static struct attribute *im8724_13026_fan_attributes[] = {
	&im_dev_attr_max_speed.dev_attr.attr,
	&im_dev_attr_alert0.dev_attr.attr,
	&im_dev_attr_alert1.dev_attr.attr,
	&im_dev_attr_power0.dev_attr.attr,
	&im_dev_attr_power1.dev_attr.attr,
	&im_dev_attr_pg0.dev_attr.attr,
	&im_dev_attr_pg1.dev_attr.attr,
	&im_dev_attr_flt0.dev_attr.attr,
	&im_dev_attr_flt1.dev_attr.attr,
	NULL
};

static const struct attribute_group im_fan_group = {
	.name = "fan",
	.attrs = im8724_13026_fan_attributes
};

IM_DEV_RW_ATTR(int_en_0,	CPLD_ADDR_INT_EN_0,	0x7f,	0);
IM_DEV_RW_ATTR(cs4318_ena,	CPLD_ADDR_INT_EN_0,	0x1,	0);
IM_DEV_RW_ATTR(temp_ena,	CPLD_ADDR_INT_EN_0,	0x1,	1);
IM_DEV_RW_ATTR(ps_ena,		CPLD_ADDR_INT_EN_0,	0x1,	2);
IM_DEV_RW_ATTR(ioa_ena,		CPLD_ADDR_INT_EN_0,	0x1,	3);
IM_DEV_RW_ATTR(iob_ena,		CPLD_ADDR_INT_EN_0,	0x1,	4);
IM_DEV_RW_ATTR(io_pr_det_ena,	CPLD_ADDR_INT_EN_0,	0x1,	5);
IM_DEV_RW_ATTR(mgt_phy_ena,	CPLD_ADDR_INT_EN_0,	0x1,	6);
IM_DEV_RW_ATTR(overt_sde,	CPLD_ADDR_INT_EN_0,	0x1,	7);
IM_DEV_RO_ATTR(int_stat_0,	CPLD_ADDR_INT_STAT_0,	0x7f,	0);
IM_DEV_RO_ATTR(cs4318_ev,	CPLD_ADDR_INT_STAT_0,	0x1,	0);
IM_DEV_RO_ATTR(temp_ev,		CPLD_ADDR_INT_STAT_0,	0x1,	1);
IM_DEV_RO_ATTR(ps_ev,		CPLD_ADDR_INT_STAT_0,	0x1,	2);
IM_DEV_RO_ATTR(ioa_ev,		CPLD_ADDR_INT_STAT_0,	0x1,	3);
IM_DEV_RO_ATTR(iob_ev,		CPLD_ADDR_INT_STAT_0,	0x1,	4);
IM_DEV_RO_ATTR(io_pr_det,	CPLD_ADDR_INT_STAT_0,	0x1,	5);
IM_DEV_RO_ATTR(mgt_phy,		CPLD_ADDR_INT_STAT_0,	0x1,	6);
IM_DEV_RO_ATTR(overt_ev,	CPLD_ADDR_INT_STAT_0,	0x1,	7);
IM_DEV_RW_ATTR(int_en_1,	CPLD_ADDR_INT_EN_1,	0x7f,	0);
IM_DEV_RW_ATTR(sfp0_ena,	CPLD_ADDR_INT_EN_1,	0x1,	0);
IM_DEV_RW_ATTR(sfp1_ena,	CPLD_ADDR_INT_EN_1,	0x1,	1);
IM_DEV_RW_ATTR(sfp2_ena,	CPLD_ADDR_INT_EN_1,	0x1,	2);
IM_DEV_RW_ATTR(sfp3_ena,	CPLD_ADDR_INT_EN_1,	0x1,	3);
IM_DEV_RW_ATTR(misc_flt_ena,	CPLD_ADDR_INT_EN_1,	0x1,	4);
IM_DEV_RW_ATTR(pex_ena,		CPLD_ADDR_INT_EN_1,	0x1,	5);
IM_DEV_RW_ATTR(fan_ena,		CPLD_ADDR_INT_EN_1,	0x1,	6);
IM_DEV_RO_ATTR(int_stat_1,	CPLD_ADDR_INT_STAT_1,	0x7f,	0);
IM_DEV_RO_ATTR(sfp0_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	0);
IM_DEV_RO_ATTR(sfp1_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	1);
IM_DEV_RO_ATTR(sfp2_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	2);
IM_DEV_RO_ATTR(sfp3_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	3);
IM_DEV_RO_ATTR(misc_flt_ev,	CPLD_ADDR_INT_STAT_1,	0x1,	4);
IM_DEV_RO_ATTR(pex_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	5);
IM_DEV_RO_ATTR(fan_ev,		CPLD_ADDR_INT_STAT_1,	0x1,	6);

static struct attribute *im8724_events_attributes[] = {
        &im_dev_attr_int_en_0.dev_attr.attr,
        &im_dev_attr_cs4318_ena.dev_attr.attr,
        &im_dev_attr_temp_ena.dev_attr.attr,
        &im_dev_attr_ps_ena.dev_attr.attr,
        &im_dev_attr_ioa_ena.dev_attr.attr,
        &im_dev_attr_iob_ena.dev_attr.attr,
        &im_dev_attr_io_pr_det_ena.dev_attr.attr,
        &im_dev_attr_mgt_phy_ena.dev_attr.attr,
        &im_dev_attr_overt_sde.dev_attr.attr,
        &im_dev_attr_int_stat_0.dev_attr.attr,
        &im_dev_attr_cs4318_ev.dev_attr.attr,
        &im_dev_attr_temp_ev.dev_attr.attr,
        &im_dev_attr_ps_ev.dev_attr.attr,
        &im_dev_attr_ioa_ev.dev_attr.attr,
        &im_dev_attr_iob_ev.dev_attr.attr,
        &im_dev_attr_io_pr_det.dev_attr.attr,
        &im_dev_attr_mgt_phy.dev_attr.attr,
        &im_dev_attr_overt_ev.dev_attr.attr,
        &im_dev_attr_int_en_1.dev_attr.attr,
        &im_dev_attr_sfp0_ena.dev_attr.attr,
        &im_dev_attr_sfp1_ena.dev_attr.attr,
        &im_dev_attr_sfp2_ena.dev_attr.attr,
        &im_dev_attr_sfp3_ena.dev_attr.attr,
        &im_dev_attr_misc_flt_ena.dev_attr.attr,
        &im_dev_attr_pex_ena.dev_attr.attr,
        &im_dev_attr_fan_ena.dev_attr.attr,
        &im_dev_attr_int_stat_1.dev_attr.attr,
        &im_dev_attr_sfp0_ev.dev_attr.attr,
        &im_dev_attr_sfp1_ev.dev_attr.attr,
        &im_dev_attr_sfp2_ev.dev_attr.attr,
        &im_dev_attr_sfp3_ev.dev_attr.attr,
        &im_dev_attr_misc_flt_ev.dev_attr.attr,
        &im_dev_attr_pex_ev.dev_attr.attr,
        &im_dev_attr_fan_ev.dev_attr.attr,
	NULL
};

static const struct attribute_group im_events_group = {
	.name = "events",
	.attrs = im8724_events_attributes
};

IM_DEV_RW_ATTR(tm_alert,	CPLD_ADDR_FLT_STAT, 0x1, 0);
IM_DEV_RW_ATTR(tm_overt,	CPLD_ADDR_FLT_STAT, 0x1, 1);
IM_DEV_RW_ATTR(core_alert,	CPLD_ADDR_FLT_STAT, 0x1, 2);
IM_DEV_RW_ATTR(usb_ov,		CPLD_ADDR_FLT_STAT, 0x1, 3);
IM_DEV_RW_ATTR(hp_alert,	CPLD_ADDR_FLT_STAT, 0x1, 4);
IM_DEV_RW_ATTR(pex_inta,	CPLD_ADDR_FLT_STAT, 0x1, 6);
IM_DEV_RW_ATTR(pex_err,		CPLD_ADDR_FLT_STAT, 0x1, 7);

static struct attribute *im8724_fault_attributes[] = {
	&im_dev_attr_tm_alert.dev_attr.attr,
	&im_dev_attr_tm_overt.dev_attr.attr,
	&im_dev_attr_core_alert.dev_attr.attr,
	&im_dev_attr_usb_ov.dev_attr.attr,
	&im_dev_attr_hp_alert.dev_attr.attr,
	&im_dev_attr_pex_inta.dev_attr.attr,
	&im_dev_attr_pex_err.dev_attr.attr,
	NULL
};

static const struct attribute_group im_fault_group = {
	.name = "fault",
	.attrs = im8724_fault_attributes
};

IM_DEV_RO_ATTR(ps1_prsnt,	CPLD_ADDR_PS_CSR, 0x1, 7);
IM_DEV_RO_ATTR(ps1_ok,		CPLD_ADDR_PS_CSR, 0x1, 6);
IM_DEV_RO_ATTR(ps1_int,		CPLD_ADDR_PS_CSR, 0x1, 5);
IM_DEV_RW_ATTR(ps1_off,		CPLD_ADDR_PS_CSR, 0x1, 4);
IM_DEV_RO_ATTR(ps0_prsnt,	CPLD_ADDR_PS_CSR, 0x1, 3);
IM_DEV_RO_ATTR(ps0_ok,		CPLD_ADDR_PS_CSR, 0x1, 2);
IM_DEV_RO_ATTR(ps0_int,		CPLD_ADDR_PS_CSR, 0x1, 1);
IM_DEV_RW_ATTR(ps0_off,		CPLD_ADDR_PS_CSR, 0x1, 0);
IM_DEV_RW_ATTR(usb_pe,		CPLD_ADDR_IO_PWR_CTL, 0x1, 4);
IM_DEV_RW_ATTR(iob_pe,		CPLD_ADDR_IO_PWR_CTL, 0x1, 3);
IM_DEV_RW_ATTR(iob_hp_pe,	CPLD_ADDR_IO_PWR_CTL, 0x1, 2);
IM_DEV_RW_ATTR(ioa_pe,		CPLD_ADDR_IO_PWR_CTL, 0x1, 1);
IM_DEV_RW_ATTR(ioa_hp_pe,	CPLD_ADDR_IO_PWR_CTL, 0x1, 0);

static struct attribute *im8724_power_attributes[] = {
	&im_dev_attr_ps0_prsnt.dev_attr.attr,
	&im_dev_attr_ps0_ok.dev_attr.attr,
	&im_dev_attr_ps0_int.dev_attr.attr,
	&im_dev_attr_ps0_off.dev_attr.attr,
	&im_dev_attr_ps1_prsnt.dev_attr.attr,
	&im_dev_attr_ps1_ok.dev_attr.attr,
	&im_dev_attr_ps1_int.dev_attr.attr,
	&im_dev_attr_ps1_off.dev_attr.attr,
	&im_dev_attr_usb_pe.dev_attr.attr,
	&im_dev_attr_iob_pe.dev_attr.attr,
	&im_dev_attr_iob_hp_pe.dev_attr.attr,
	&im_dev_attr_ioa_pe.dev_attr.attr,
	&im_dev_attr_ioa_hp_pe.dev_attr.attr,
	NULL
};

static const struct attribute_group im_power_group = {
	.name = "power",
	.attrs = im8724_power_attributes
};

IM_DEV_RW_ATTR(cn_cpu,		CPLD_ADDR_RST_CTL_0, 0x1, 0);
IM_DEV_RW_ATTR(pex_pcie,	CPLD_ADDR_RST_CTL_0, 0x1, 1);
IM_DEV_RW_ATTR(mgmt_phy,	CPLD_ADDR_RST_CTL_0, 0x1, 2);
IM_DEV_RW_ATTR(cs4318,		CPLD_ADDR_RST_CTL_0, 0x1, 3);
IM_DEV_RW_ATTR(zl_clk,		CPLD_ADDR_RST_CTL_0, 0x1, 4);
IM_DEV_RW_ATTR(re_boot,		CPLD_ADDR_RST_CTL_0, 0x1, 6);
IM_DEV_RW_ATTR(ioa_card,	CPLD_ADDR_RST_CTL_1, 0x1, 0);
IM_DEV_RW_ATTR(iob_card,	CPLD_ADDR_RST_CTL_1, 0x1, 1);
IM_DEV_RW_ATTR(ioa_pcie,	CPLD_ADDR_RST_CTL_1, 0x1, 2);
IM_DEV_RW_ATTR(iob_pcie,	CPLD_ADDR_RST_CTL_1, 0x1, 3);
IM_DEV_RW_ATTR(ssd0_pcie,	CPLD_ADDR_RST_CTL_1, 0x1, 4);
IM_DEV_RW_ATTR(ssd1_pcie,	CPLD_ADDR_RST_CTL_1, 0x1, 5);

static struct attribute *im_reset_attributes[] = {
	&im_dev_attr_cn_cpu.dev_attr.attr,
	&im_dev_attr_pex_pcie.dev_attr.attr,
	&im_dev_attr_mgmt_phy.dev_attr.attr,
	&im_dev_attr_cs4318.dev_attr.attr,
	&im_dev_attr_zl_clk.dev_attr.attr,
	&im_dev_attr_re_boot.dev_attr.attr,
	&im_dev_attr_ioa_card.dev_attr.attr,
	&im_dev_attr_iob_card.dev_attr.attr,
	&im_dev_attr_ioa_pcie.dev_attr.attr,
	&im_dev_attr_iob_pcie.dev_attr.attr,
	&im_dev_attr_ssd0_pcie.dev_attr.attr,
	&im_dev_attr_ssd1_pcie.dev_attr.attr,
	NULL
};

static const struct attribute_group im_reset_group = {
	.name = "reset",
	.attrs = im_reset_attributes
};

IM_DEV_RW_ATTR(led_cfg,		CPLD_LED_CFG, 0xf, 0);
IM_DEV_RW_ATTR(v_act,		CPLD_LED_CFG, 0x1, 0);
IM_DEV_RW_ATTR(v_col,		CPLD_LED_CFG, 0x1, 1);
IM_DEV_RW_ATTR(c_act,		CPLD_LED_CFG, 0x1, 2);
IM_DEV_RW_ATTR(c_col,		CPLD_LED_CFG, 0x1, 3);
IM_DEV_RW_ATTR(st_led,		CPLD_ST_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(status_lc,	CPLD_ST_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(status_col,	CPLD_ST_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(mphy_act,	CPLD_ST_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(mphy_col,	CPLD_ST_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(pwr_led,		CPLD_PWR_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(pwr0_lc,		CPLD_PWR_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(pwr0_col,	CPLD_PWR_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(pwr1_lc,		CPLD_PWR_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(pwr1_col,	CPLD_PWR_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(s0_led,		CPLD_S0_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(s0_act,		CPLD_S0_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(s0_col,		CPLD_S0_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(s1_act,		CPLD_S0_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(s1_col,		CPLD_S0_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(s2_led,		CPLD_S2_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(s2_act,		CPLD_S2_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(s2_col,		CPLD_S2_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(s3_act,		CPLD_S2_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(s3_col,		CPLD_S2_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w0_led,		CPLD_W0_LED_CTL, 0xff,0);
IM_DEV_RW_ATTR(w0_act,		CPLD_W0_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w0_col,		CPLD_W0_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w1_act,		CPLD_W0_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w1_col,		CPLD_W0_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w2_led,		CPLD_W2_LED_CTL, 0xff,0);
IM_DEV_RW_ATTR(w2_act,		CPLD_W2_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w2_col,		CPLD_W2_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w3_act,		CPLD_W2_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w3_col,		CPLD_W2_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w4_led,		CPLD_W4_LED_CTL, 0xff,0);
IM_DEV_RW_ATTR(w4_act,		CPLD_W4_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w4_col,		CPLD_W4_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w5_act,		CPLD_W4_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w5_col,		CPLD_W4_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w6_led,		CPLD_W6_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(w6_act,		CPLD_W6_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w6_col,		CPLD_W6_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w7_act,		CPLD_W6_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w7_col,		CPLD_W6_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w8_led,		CPLD_W8_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(w8_act,		CPLD_W8_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w8_col,		CPLD_W8_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w9_act,		CPLD_W8_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w9_col,		CPLD_W8_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w10_led,		CPLD_W10_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(w10_act,		CPLD_W10_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w10_col,		CPLD_W10_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w11_act,		CPLD_W10_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w11_col,		CPLD_W10_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w12_led,		CPLD_W12_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(w12_act,		CPLD_W12_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w12_col,		CPLD_W12_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w13_act,		CPLD_W12_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w13_col,		CPLD_W12_LED_CTL, 0x3, 6);
IM_DEV_RW_ATTR(w14_led,		CPLD_W14_LED_CTL, 0xff, 0);
IM_DEV_RW_ATTR(w14_act,		CPLD_W14_LED_CTL, 0x3, 0);
IM_DEV_RW_ATTR(w14_col,		CPLD_W14_LED_CTL, 0x3, 2);
IM_DEV_RW_ATTR(w15_act,		CPLD_W14_LED_CTL, 0x3, 4);
IM_DEV_RW_ATTR(w15_col,		CPLD_W14_LED_CTL, 0x3, 6);

static struct attribute *im_leds_attributes[] = {
	&im_dev_attr_led_cfg.dev_attr.attr,
	&im_dev_attr_v_col.dev_attr.attr,
	&im_dev_attr_v_act.dev_attr.attr,
	&im_dev_attr_c_col.dev_attr.attr,
	&im_dev_attr_c_act.dev_attr.attr,
	&im_dev_attr_st_led.dev_attr.attr,
	&im_dev_attr_mphy_act.dev_attr.attr,
	&im_dev_attr_mphy_col.dev_attr.attr,
	&im_dev_attr_status_lc.dev_attr.attr,
	&im_dev_attr_status_col.dev_attr.attr,
	&im_dev_attr_pwr_led.dev_attr.attr,
	&im_dev_attr_pwr0_lc.dev_attr.attr,
	&im_dev_attr_pwr0_col.dev_attr.attr,
	&im_dev_attr_pwr1_lc.dev_attr.attr,
	&im_dev_attr_pwr1_col.dev_attr.attr,
	&im_dev_attr_s0_led.dev_attr.attr,
	&im_dev_attr_s0_act.dev_attr.attr,
	&im_dev_attr_s0_col.dev_attr.attr,
	&im_dev_attr_s1_act.dev_attr.attr,
	&im_dev_attr_s1_col.dev_attr.attr,
	&im_dev_attr_s2_led.dev_attr.attr,
	&im_dev_attr_s2_act.dev_attr.attr,
	&im_dev_attr_s2_col.dev_attr.attr,
	&im_dev_attr_s3_act.dev_attr.attr,
	&im_dev_attr_s3_col.dev_attr.attr,
	&im_dev_attr_w0_led.dev_attr.attr,
	&im_dev_attr_w0_act.dev_attr.attr,
	&im_dev_attr_w0_col.dev_attr.attr,
	&im_dev_attr_w1_act.dev_attr.attr,
	&im_dev_attr_w1_col.dev_attr.attr,
	&im_dev_attr_w2_led.dev_attr.attr,
	&im_dev_attr_w2_act.dev_attr.attr,
	&im_dev_attr_w2_col.dev_attr.attr,
	&im_dev_attr_w3_act.dev_attr.attr,
	&im_dev_attr_w3_col.dev_attr.attr,
	&im_dev_attr_w4_led.dev_attr.attr,
	&im_dev_attr_w4_act.dev_attr.attr,
	&im_dev_attr_w4_col.dev_attr.attr,
	&im_dev_attr_w5_act.dev_attr.attr,
	&im_dev_attr_w5_col.dev_attr.attr,
	&im_dev_attr_w6_led.dev_attr.attr,
	&im_dev_attr_w6_act.dev_attr.attr,
	&im_dev_attr_w6_col.dev_attr.attr,
	&im_dev_attr_w7_act.dev_attr.attr,
	&im_dev_attr_w7_col.dev_attr.attr,
	&im_dev_attr_w8_led.dev_attr.attr,
	&im_dev_attr_w8_act.dev_attr.attr,
	&im_dev_attr_w8_col.dev_attr.attr,
	&im_dev_attr_w9_act.dev_attr.attr,
	&im_dev_attr_w9_col.dev_attr.attr,
	&im_dev_attr_w10_led.dev_attr.attr,
	&im_dev_attr_w10_act.dev_attr.attr,
	&im_dev_attr_w10_col.dev_attr.attr,
	&im_dev_attr_w11_act.dev_attr.attr,
	&im_dev_attr_w11_col.dev_attr.attr,
	&im_dev_attr_w12_led.dev_attr.attr,
	&im_dev_attr_w12_act.dev_attr.attr,
	&im_dev_attr_w12_col.dev_attr.attr,
	&im_dev_attr_w13_act.dev_attr.attr,
	&im_dev_attr_w13_col.dev_attr.attr,
	&im_dev_attr_w14_led.dev_attr.attr,
	&im_dev_attr_w14_act.dev_attr.attr,
	&im_dev_attr_w14_col.dev_attr.attr,
	&im_dev_attr_w15_act.dev_attr.attr,
	&im_dev_attr_w15_col.dev_attr.attr,
	NULL
};

static const struct attribute_group im_leds_group = {
	.name = "leds",
	.attrs = im_leds_attributes
};

static const struct attribute_group *im8724_groups[] = {
	&im_sfp0_group,
	&im_sfp1_group,
	&im_sfp2_group,
	&im_sfp3_group,
	&im_misc_group,
	&im_events_group,
	&im_fault_group,
	&im_power_group,
	&im_reset_group,
	&im_leds_group,
	&im_fan_group,
};

IM_DIAG_SFP_ATTR(0);
IM_DIAG_SFP_ATTR(1);
IM_DIAG_SFP_ATTR(2);
IM_DIAG_SFP_ATTR(3);

struct im8724info {
	struct platform_device *dev;
	struct kobject *kobj;
	struct kobject *kobj_i2c;
	struct i2c_client *i2c_fans[2];
	struct i2c_client *i2c_sfps[4];
	struct i2c_client *i2c_ps[2];
	const struct attribute_group *diag_group[4];
	int num_sfps;
	u8 pim_sig[2];
} im8724_device = {
	.diag_group = {
		&im_diag_sfp0_group,
		&im_diag_sfp1_group,
		&im_diag_sfp2_group,
		&im_diag_sfp3_group,
	}
};

static struct im8724info *im8724 = &im8724_device;

int im8724_sysfs_init(void)
{
	int err = 0;
	int i = 0;

	im8724->dev = platform_device_register_simple("im8724", -1, NULL, 0);

	im8724->kobj = kobject_create_and_add("niagara", NULL);
	im8724->kobj_i2c = NULL;

	for (i = 0; i < ARRAY_SIZE(im8724_groups); i++) {
		err = sysfs_create_group(im8724->kobj, im8724_groups[i]);
		if (err)
			goto unwind_im8724_groups;
	}
	return 0;

unwind_im8724_groups:
	/* remove groups in reverse order */
	for (i--; i >= 0; i--)
		sysfs_remove_group(im8724->kobj, im8724_groups[i]);

	kobject_put(im8724->kobj);
	im8724->kobj = NULL;
	platform_device_unregister(im8724->dev);
	return err;
}

void im8724_sysfs_remove(void)
{
	int i = 0;
	
	if (im8724->kobj) {
		for (i = 0; i < ARRAY_SIZE(im8724_groups); i++)
			sysfs_remove_group(im8724->kobj, im8724_groups[i]);

		if (im8724->kobj_i2c) {
			for (i = 0; i < im8724->num_sfps; i++)
				sysfs_remove_group(im8724->kobj_i2c, im8724->diag_group[i]);
		}

		kobject_put(im8724->kobj);
		im8724->kobj = NULL;
	}

	if (im8724->dev)
		platform_device_unregister(im8724->dev);
}

int im8724_pim_probe(int slot)
{
	u8 val;
	int retries;

	/* Check for module presence.  */
	val = niagara_cpld_read(CPLD_ADDR_IO_STAT);
	if ((slot && !(val & CPLD_IOB_PRES)) || (!slot && !(val & CPLD_IOA_PRES)))
		return 0;

        val = niagara_cpld_read(CPLD_ADDR_IO_PWR_CTL);
        if ((val & CPLD_PIM_PWR_ENABLE(slot)) != CPLD_PIM_PWR_ENABLE(slot)) {
		pr_warn("IO card %c: power not enabled! turning on\n", slot ? 'B' : 'A');
		niagara_cpld_write(CPLD_ADDR_IO_PWR_CTL, val | CPLD_PIM_PWR_ENABLE(slot));
		mdelay(200);
	}

	retries = 5;
	while (retries--) {
		/*
		 * Read PIM signature.
		 */
		niagara_cpld_lock();
		im8724->pim_sig[slot] = niagara_cpld_remote_read(slot, 0);
		niagara_cpld_unlock();
		switch (im8724->pim_sig[slot]) {
		case CPLD_IM13233_SIG_VL:
			return im13233_init(slot, im8724->kobj);
		case CPLD_IM13264_SIG_VL:
			return im13264_init(slot, im8724->kobj);
		case CPLD_IM13262_SIG_VL:
			return im13262_init(slot, im8724->kobj);
		default:
			pr_warn("IO card %c: unknown signature 0x%02x\n",
					slot ? 'B' : 'A', im8724->pim_sig[slot]);
			mdelay(100);
		}
	}
	return -ENODEV;
}

int im8724_board_link_status(u32 interface, u32 port_index)
{
	int link_status = 0;

	if (interface < 2) {
		if (port_index != 0 && port_index != 1)
			return 0;
		link_status = ! (niagara_cpld_read(CPLD_ADDR_SFP_RX_LOS) & (0x01 << (2 * interface - port_index)));
		pr_devel("SFP Rx loss is %d for iface %d with and port %d.\n",
			!link_status, interface, port_index);
	}
	else if (interface >= 2 && interface <= 5) {
		int slot = interface < 4 ? 1 : 0;

		switch (im8724->pim_sig[slot]) {
			case CPLD_IM13233_SIG_VL:
				link_status = im13233_board_link_status(interface, port_index);
				break;
			case CPLD_IM13264_SIG_VL:
				link_status = im13264_board_link_status(interface, port_index);
				break;
			case CPLD_IM13262_SIG_VL:
				link_status = im13262_board_link_status(interface, port_index);
				break;
		}
	}
	return link_status;
}

int im8724_board_phy_status(struct phy_device *phydev)
{
	int port_index = phydev->mdio.addr;
	u64 register_base = *(u64*)(phydev->mdio.bus->priv + sizeof(struct mii_bus *));

	pr_devel("port index: %d, smi_base: 0x%llx\n",
			port_index, (unsigned long long )register_base);
	if (port_index > 3)
		return 0;
	if (register_base != OCTEON_MDIO_BUS0_BASE)
		port_index += 2;
	return !(niagara_cpld_read(CPLD_ADDR_SFP_RX_LOS) & (1 << port_index));
}

static struct i2c_board_info im8724_fan_info[] = {
        {
                I2C_BOARD_INFO("adt7470", 0x2f),
        },
        {
                I2C_BOARD_INFO("adt7470", 0x2c),
        },
};

static struct i2c_board_info im8724_ps_info[] = {
	{
		I2C_BOARD_INFO("spi800", 0x58),
	},
	{
		I2C_BOARD_INFO("spi800", 0x59),
	},
};

static struct at24_platform_data sfp_eeprom_data = {
	.byte_len       = 256,
	.page_size      = 1,
	.flags          = AT24_FLAG_READONLY,
};

static struct i2c_board_info sfp_eeprom_info = {
	I2C_BOARD_INFO("24c02", 0x50),
	.platform_data  = &sfp_eeprom_data,
};

static struct at24_platform_data sfp_diag_data = {
	.byte_len       = 256,
	.page_size      = 1,
	.flags          = 0,
};

static struct i2c_board_info sfp_diag_info = {
	I2C_BOARD_INFO("24c02", 0x51),
	.platform_data  = &sfp_diag_data,
};

int im8724_i2c_init(void)
{
	cvmx_bgxx_cmr_global_config_t gconfig;
	struct i2c_adapter *adap;
	struct i2c_client *client;
	int err = 0, i, k = 0;
	char name[16];

	err = niagara_add_i2c(0x9e00, "fan", &adap, 0);
	if (err) {
		pr_info("IM8724 fans I2C init failed\n");
		goto clear_all_entries;
	}

	for (i = 0; i < ARRAY_SIZE(im8724_fan_info); i++) {
		client = i2c_new_device(adap, &im8724_fan_info[i]);
		if (!client) {
			pr_err("unable to add niagara i2c adt7470 device");
			err = -ENODEV;
			goto clear_all_entries;
		}
		im8724->i2c_fans[i] = client;
	}

	err = niagara_add_i2c(0x8, "ps", &adap, 1);
	if (err) {
		pr_info("IM8724 power supply I2C bus init failed\n");
		goto clear_all_entries;
	}

	for (i = 0; i < ARRAY_SIZE(im8724_ps_info); i++)
		im8724->i2c_ps[i] = i2c_new_device(adap, &im8724_ps_info[i]);

	/* Identify BGX1 interface mode and number of RXAUI frontpanel  ports */
	gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
	im8724->num_sfps = gconfig.s.pmux_sds_sel ? 2 : 4;
	for (i = 0; i < im8724->num_sfps; i++) {
		snprintf(name, sizeof(name), "rxaui_sfp%d", i);
		err = niagara_add_i2c(i, name, &adap, 0);
		if (err) {
			pr_info("IM8724 rxaui sfp%d I2C init failed\n", i);
			goto clear_all_entries;
		}

		client = i2c_new_device(adap, &sfp_eeprom_info);
		if (!client) {
			pr_err("unable to add niagara i2c sfp eeprom");
			err = -ENODEV;
			goto clear_all_entries;
		}
		im8724->i2c_sfps[i] = client;
		if (niagara_i2c_client_list[0])
			niagara_i2c_client_list[0][IM8724_I2C_SFP_CLIENTS_START + i] = client;

		client = i2c_new_device(adap, &sfp_diag_info);
		if (!client) {
			pr_err("unable to add niagara i2c sfp diag");
			err = -ENODEV;
			goto clear_all_entries;
		}
		if (niagara_i2c_client_list[1])
			niagara_i2c_client_list[1][IM8724_I2C_SFP_CLIENTS_START + i] = client;
	}

	im8724->kobj_i2c = kobject_create_and_add("diag", im8724->kobj);
	for (k = 0; k < im8724->num_sfps; k++) {
		err = sysfs_create_group(im8724->kobj_i2c, im8724->diag_group[k]);
		if (err)
			goto out_diag_group;
	}

	goto done;

clear_all_entries:
	niagara_del_all_i2c();
out_diag_group:
	for (k--; k >= 0; k--)
		sysfs_remove_group(im8724->kobj_i2c, im8724->diag_group[k]);
done:
	return err;
}

void im8724_i2c_deinit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(im8724_fan_info); i++) {
		if (im8724->i2c_fans[i])
			i2c_unregister_device(im8724->i2c_fans[i]);
		im8724->i2c_fans[i] = NULL;
	}

	for (i = 0; i < ARRAY_SIZE(im8724_ps_info); i++) {
		if (im8724->i2c_ps[i])
			i2c_unregister_device(im8724->i2c_ps[i]);
		im8724->i2c_fans[i] = NULL;
	}

	for (i = 0; i < im8724->num_sfps; i++) {
		if (im8724->i2c_sfps[i])
			i2c_unregister_device(im8724->i2c_sfps[i]);
		im8724->i2c_sfps[i] = NULL;
		if (niagara_i2c_client_list[1][IM8724_I2C_SFP_CLIENTS_START + i])
			i2c_unregister_device(niagara_i2c_client_list[1][IM8724_I2C_SFP_CLIENTS_START + i]);
		niagara_i2c_client_list[0][IM8724_I2C_SFP_CLIENTS_START + i] = NULL;
		niagara_i2c_client_list[1][IM8724_I2C_SFP_CLIENTS_START + i] = NULL;
	}
}
