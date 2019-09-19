#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/platform_data/niagara.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/platform_data/at24.h>

#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-helper.h>

#include "niagara_cpld.h"
#include "n820_cpld.h"

struct im820info {
	struct platform_device *dev;
	struct kobject *kobj;
	struct kobject *kobj_led;
	struct kobject *kobj_i2c;
	struct i2c_client *i2c_temp;
	struct i2c_client *i2c_m24512;
	struct i2c_client *i2c_poe;
};

static struct im820info im820_device;
static struct im820info *im820 = &im820_device;
static struct i2c_adapter niagara_tpm_i2c_root;
static struct i2c_algo_bit_data niagara_tpm_i2c_bit_algo;

IM_DEV_RO_ATTR(cpld_sig,	CPLD_ADDR_CPLD_SIG_L,	0xff, 0);
IM_DEV_RO_ATTR(cpld_rev,	CPLD_ADDR_CPLD_REV,	0x0f, 0);
IM_DEV_RO_ATTR(board_rev,	CPLD_ADDR_CPLD_REV,	0x07, 4);
IM_DEV_RW_ATTR(poe_fbc_en,	CPLD_ADDR_AUX_CTL,	0x01, 5);

IM_DEV_RW_ATTR(phy_coma,	CPLD_ADDR_MISC_CSR,	0x01, 2);
IM_DEV_RO_ATTR(tm_overt,	CPLD_ADDR_MISC_CSR,	0x01, 7);
IM_DEV_RO_ATTR(tm_alert,	CPLD_ADDR_MISC_CSR,	0x01, 6);
IM_DEV_RO_ATTR(gps_id,		CPLD_ADDR_MISC_STAT,	0x01, 0);

IM_DEV_RO_ATTR(pwr0_ok,		CPLD_ADDR_TEST_0,	0x01, 0);
IM_DEV_RO_ATTR(pwr1_ok,		CPLD_ADDR_TEST_0,	0x01, 1);

IM_DEV_RO_ATTR(fan_speed,	CPLD_ADDR_FAN_CS,	0xff, 0);
IM_DEV_RW_ATTR(fan_pwm,		CPLD_ADDR_FAN_DC,	0xff, 0);
IM_DEV_RW_ATTR(scratchpad,	CPLD_ADDR_S_PAD,	0xff, 0);

IM_DEV_RW_ATTR(long_reset,      CPLD_ADDR_MISC_STAT,    0x1, 5);
IM_DEV_RW_ATTR(short_reset,     CPLD_ADDR_INT_STAT,     0x1, 6);
IM_DEV_RW_ATTR(pwr_led_ctl,     CPLD_ADDR_PWR_LED_CTL,  0x1, 4);
IM_DEV_RW_ATTR(wan_led_ctl,     CPLD_ADDR_WAN_LED_CTL,  0x1, 4);
IM_DEV_RW_ATTR(lan_led_ctl,     CPLD_ADDR_WAN_LED_CTL,  0x1, 5);

static struct attribute *im820_misc_attributes[] = {
	&im_dev_attr_cpld_sig.dev_attr.attr,
	&im_dev_attr_cpld_rev.dev_attr.attr,
	&im_dev_attr_board_rev.dev_attr.attr,
	&im_dev_attr_poe_fbc_en.dev_attr.attr,

	&im_dev_attr_phy_coma.dev_attr.attr,
	&im_dev_attr_tm_overt.dev_attr.attr,
	&im_dev_attr_tm_alert.dev_attr.attr,
	&im_dev_attr_gps_id.dev_attr.attr,

	&im_dev_attr_pwr0_ok.dev_attr.attr,
	&im_dev_attr_pwr1_ok.dev_attr.attr,

	&im_dev_attr_fan_speed.dev_attr.attr,
	&im_dev_attr_fan_pwm.dev_attr.attr,
	&im_dev_attr_scratchpad.dev_attr.attr,
        &im_dev_attr_long_reset.dev_attr.attr,
        &im_dev_attr_short_reset.dev_attr.attr,
        &im_dev_attr_pwr_led_ctl.dev_attr.attr,
        &im_dev_attr_wan_led_ctl.dev_attr.attr,
        &im_dev_attr_lan_led_ctl.dev_attr.attr,
	NULL
};

static const struct attribute_group im_misc_group = {
	.name = "misc",
	.attrs = im820_misc_attributes
};

IM_DEV_RO_ATTR(lte0_antctrl,	CPLD_ADDR_LTE_STAT, 0x07, 1);
IM_DEV_RO_ATTR(lte0_dpr,	CPLD_ADDR_LTE_STAT, 0x01, 0);
IM_DEV_RO_ATTR(lte1_antctrl,	CPLD_ADDR_LTE_STAT, 0x07, 4);
IM_DEV_RO_ATTR(lte1_dpr,	CPLD_ADDR_LTE_STAT, 0x01, 7);

IM_DEV_RO_ATTR(wpan0,		CPLD_ADDR_WLAN_STAT, 0x01, 0);
IM_DEV_RO_ATTR(wwan0,		CPLD_ADDR_WLAN_STAT, 0x01, 1);
IM_DEV_RO_ATTR(wlan0,		CPLD_ADDR_WLAN_STAT, 0x01, 2);
IM_DEV_RO_ATTR(wpan1,		CPLD_ADDR_WLAN_STAT, 0x01, 4);
IM_DEV_RO_ATTR(wwan1,		CPLD_ADDR_WLAN_STAT, 0x01, 5);
IM_DEV_RO_ATTR(wlan1,		CPLD_ADDR_WLAN_STAT, 0x01, 6);

IM_DEV_RW_ATTR(lte1_pcm_en,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 5);
IM_DEV_RW_ATTR(lte0_pcm_en,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 4);
IM_DEV_RW_ATTR(lte1_rst,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 3);
IM_DEV_RW_ATTR(lte0_rst,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 2);
IM_DEV_RW_ATTR(wlan1_dis,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 1);
IM_DEV_RW_ATTR(wlan0_dis,	CPLD_ADDR_LTE_WLAN_CTL, 0x01, 0);

static struct attribute *im820_lan_attributes[] = {
		&im_dev_attr_lte0_antctrl.dev_attr.attr,
		&im_dev_attr_lte0_dpr.dev_attr.attr,
		&im_dev_attr_lte1_antctrl.dev_attr.attr,
		&im_dev_attr_lte1_dpr.dev_attr.attr,
		&im_dev_attr_wpan0.dev_attr.attr,
		&im_dev_attr_wwan0.dev_attr.attr,
		&im_dev_attr_wlan0.dev_attr.attr,
		&im_dev_attr_wpan1.dev_attr.attr,
		&im_dev_attr_wwan1.dev_attr.attr,
		&im_dev_attr_wlan1.dev_attr.attr,
		&im_dev_attr_lte0_pcm_en.dev_attr.attr,
		&im_dev_attr_lte1_pcm_en.dev_attr.attr,
		&im_dev_attr_lte0_rst.dev_attr.attr,
		&im_dev_attr_lte1_rst.dev_attr.attr,
		&im_dev_attr_wlan0_dis.dev_attr.attr,
		&im_dev_attr_wlan1_dis.dev_attr.attr,
		NULL
};

static const struct attribute_group im_lan_group = {
	.name = "lan",
	.attrs = im820_lan_attributes
};

IM_DEV_RW_ATTR(power,		CPLD_ADDR_RST_CTL, 0x1, 7);
IM_DEV_RW_ATTR(cpu,		CPLD_ADDR_RST_CTL, 0x1, 0);
IM_DEV_RW_ATTR(tpm,		CPLD_ADDR_SD_CTL,  0x1, 4);

static struct attribute *im820_reset_attributes[] = {
	&im_dev_attr_power.dev_attr.attr,
	&im_dev_attr_cpu.dev_attr.attr,
	&im_dev_attr_tpm.dev_attr.attr,
	NULL
};

static const struct attribute_group im_reset_group = {
	.name = "reset",
	.attrs = im820_reset_attributes
};

static const struct attribute_group *im820_groups[] = {
	&im_reset_group,
	&im_misc_group,
	&im_lan_group,
};

IM_DEV_LED_ATTR(system,  CPLD_ADDR_SYS_LED_CTL,   0);
IM_DEV_LED_ATTR(power,   CPLD_ADDR_PWR_LED_CTL,   0);
IM_DEV_LED_ATTR(wlan0,   CPLD_ADDR_WLAN_LED_CTL,  0);
IM_DEV_LED_ATTR(wlan1,   CPLD_ADDR_WLAN_LED_CTL,  4);
IM_DEV_LED_ATTR(wan,     CPLD_ADDR_WAN_LED_CTL,   0);
IM_DEV_LED_ATTR(lan0,    CPLD_ADDR_LAN10_LED_CTL, 0);
IM_DEV_LED_ATTR(lan1,    CPLD_ADDR_LAN10_LED_CTL, 4);
IM_DEV_LED_ATTR(lan2,    CPLD_ADDR_LAN32_LED_CTL, 0);
IM_DEV_LED_ATTR(lan3,    CPLD_ADDR_LAN32_LED_CTL, 4);

static const struct attribute_group *im820_led_groups[] = {
	&im_led_system_group,
	&im_led_power_group,
	&im_led_wlan0_group,
	&im_led_wlan1_group,
	&im_led_wan_group,
	&im_led_lan0_group,
	&im_led_lan1_group,
	&im_led_lan2_group,
	&im_led_lan3_group,
};

int n820_sysfs_init(void)
{
	int err = 0;
	int i = 0;

	im820->dev = platform_device_register_simple("im820", -1, NULL, 0);

	im820->kobj = kobject_create_and_add("niagara", NULL);

	for (i = 0; i < ARRAY_SIZE(im820_groups); i++) {
		err = sysfs_create_group(im820->kobj, im820_groups[i]);
		if (err)
			goto unwind_im820_groups;
	}

	im820->kobj_led = kobject_create_and_add("led", im820->kobj);

	for (i = 0; i < ARRAY_SIZE(im820_led_groups); i++) {
		err = sysfs_create_group(im820->kobj_led, im820_led_groups[i]);
		if (err)
			goto unwind_im820_led_groups;
	}

	im820->kobj_i2c = kobject_create_and_add("i2c", im820->kobj);

	return 0;

unwind_im820_led_groups:
	/* remove groups in reverse order */
	for (i--; i >= 0; i--)
		sysfs_remove_group(im820->kobj_led, im820_led_groups[i]);

	kobject_put(im820->kobj_led);
	im820->kobj_led = NULL;

	i = ARRAY_SIZE(im820_groups);
unwind_im820_groups:
	/* remove groups in reverse order */
	for (i--; i >= 0; i--)
		sysfs_remove_group(im820->kobj, im820_groups[i]);

	kobject_put(im820->kobj);
	im820->kobj = NULL;
	platform_device_unregister(im820->dev);

	return err;
}

void n820_sysfs_remove(void)
{
	int i = 0;

	if (im820->kobj_led) {
		for (i = 0; i < ARRAY_SIZE(im820_led_groups); i++) {
			sysfs_remove_group(im820->kobj_led,
					   im820_led_groups[i]);
		}

		kobject_put(im820->kobj_led);
		im820->kobj_led = NULL;
	}

	if (im820->kobj_i2c) {
		kobject_put(im820->kobj_i2c);
		im820->kobj_i2c = NULL;
	}

	if (im820->kobj) {
		for (i = 0; i < ARRAY_SIZE(im820_groups); i++) {
			sysfs_remove_group(im820->kobj, im820_groups[i]);
		}

		kobject_put(im820->kobj);
		im820->kobj = NULL;
	}

	if (im820->dev)
		platform_device_unregister(im820->dev);
}

static struct i2c_board_info n820_temp_info = {
	I2C_BOARD_INFO("max6657", 0x4c),
};

static struct i2c_board_info n820_poe_info = {
	I2C_BOARD_INFO("poe", 0x40),
};

static struct at24_platform_data tlv_eeprom = {
        .byte_len = 65536,
        .page_size = 128,
        .flags = AT24_FLAG_ADDR16,
};

static struct i2c_board_info n820_m24512_info = {
        I2C_BOARD_INFO("at24", 0x54),
        .platform_data = &tlv_eeprom,
};

extern int i2c_bit_niagara_add_tpm_bus(struct i2c_adapter *);

int n820_i2c_init(void)
{
	struct i2c_adapter *adap;
	struct i2c_client *client;
	int err;

	/*
	 * Renesas TPM bus is very unfriendly - stretches SCL by up to
	 * 1.5s for some operations, essentially tying up the entire I2C
	 * controller. So we give it its own bit-banged controller.
	 */

	niagara_tpm_i2c_root.algo_data = &niagara_tpm_i2c_bit_algo;
	strcpy(niagara_tpm_i2c_root.name, "tpm");
	i2c_bit_niagara_add_tpm_bus(&niagara_tpm_i2c_root);

	err = niagara_add_i2c(0x10, "temp", &adap, 0);
	if (err) {
		pr_err("unable to add niagara i2c adapter");
		goto clear_all_entries;
	}

	client = i2c_new_device(adap, &n820_temp_info);
	if (!client) {
		pr_err("unable to add niagara i2c temperature monitor");
		return -ENODEV;
	}
	im820->i2c_temp = client;

	client = i2c_new_device(adap, &n820_m24512_info);
	if (!client) {
		pr_err("unable to add niagara i2c EEPROM device");
		return -ENODEV;
	}
	im820->i2c_m24512 = client;

	client = i2c_new_device(adap, &n820_poe_info);
	if (!client) {
		pr_err("unable to add niagara i2c pse controller");
		return -ENODEV;
	}
	im820->i2c_poe = client;

	return 0;

clear_all_entries:
	niagara_del_all_i2c();
	return err;
}

void n820_i2c_deinit(void)
{
	i2c_unregister_device(im820->i2c_poe);
	im820->i2c_poe = NULL;

	i2c_unregister_device(im820->i2c_m24512);
	im820->i2c_m24512 = NULL;

	i2c_unregister_device(im820->i2c_temp);
	im820->i2c_temp = NULL;

	i2c_del_adapter(&niagara_tpm_i2c_root);
}

