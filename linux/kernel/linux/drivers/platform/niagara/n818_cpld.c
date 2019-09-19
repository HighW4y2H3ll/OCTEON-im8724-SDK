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
#include "n818_cpld.h"

struct im818info {
	struct platform_device *dev;
	struct kobject *kobj;
	struct kobject *kobj_led;
	struct kobject *kobj_i2c;
	struct i2c_client *i2c_temp;
	struct i2c_client *i2c_m24512;
	struct i2c_client *i2c_poe;
};

static struct im818info im818_device;
static struct im818info *im818 = &im818_device;
static struct i2c_adapter niagara_tpm_i2c_root;
static struct i2c_algo_bit_data niagara_tpm_i2c_bit_algo;

IM_DEV_RO_ATTR(cpld_sig,	CPLD_ADDR_CPLD_SIG_L,	0xff, 0);
IM_DEV_RO_ATTR(cpld_rev,	CPLD_ADDR_CPLD_REV,	0x0f, 0);
IM_DEV_RO_ATTR(board_rev,	CPLD_ADDR_CPLD_REV,	0x07, 4);
IM_DEV_RW_ATTR(poe_fbc_en,	CPLD_ADDR_AUX_CTL,	0x01, 5);

IM_DEV_RW_ATTR(phy_coma,	CPLD_ADDR_MISC_CSR,	0x01, 2);
IM_DEV_RO_ATTR(tm_overt,	CPLD_ADDR_MISC_CSR,	0x01, 7);
IM_DEV_RO_ATTR(tm_alert,	CPLD_ADDR_MISC_CSR,	0x01, 6);

IM_DEV_RO_ATTR(gps_id,		CPLD_ADDR_MISC_STAT,    0x01, 0);
IM_DEV_RW_ATTR(scratchpad,	CPLD_ADDR_S_PAD,	0xff, 0);
IM_DEV_RW_ATTR(pwr_led_ctl,	CPLD_ADDR_PWR_LED_CTL,  0x1, 4);
IM_DEV_RW_ATTR(mgm_led_col,	CPLD_ADDR_TEST_R0,      0x1, 3);
IM_DEV_RW_ATTR(mgm_led_act,	CPLD_ADDR_TEST_R0,      0x1, 2);
IM_DEV_RW_ATTR(lan_led_col,	CPLD_ADDR_TEST_R0,      0x1, 1);
IM_DEV_RW_ATTR(lan_led_act,	CPLD_ADDR_TEST_R0,      0x1, 0);

static struct attribute *im818_misc_attributes[] = {
	&im_dev_attr_cpld_sig.dev_attr.attr,
	&im_dev_attr_cpld_rev.dev_attr.attr,
	&im_dev_attr_board_rev.dev_attr.attr,
	&im_dev_attr_poe_fbc_en.dev_attr.attr,

	&im_dev_attr_phy_coma.dev_attr.attr,
	&im_dev_attr_tm_overt.dev_attr.attr,
	&im_dev_attr_tm_alert.dev_attr.attr,
    &im_dev_attr_gps_id.dev_attr.attr,

	&im_dev_attr_scratchpad.dev_attr.attr,

    &im_dev_attr_pwr_led_ctl.dev_attr.attr,
    &im_dev_attr_mgm_led_col.dev_attr.attr,
    &im_dev_attr_mgm_led_act.dev_attr.attr,
    &im_dev_attr_lan_led_col.dev_attr.attr,
    &im_dev_attr_lan_led_act.dev_attr.attr,
	NULL
};

static const struct attribute_group im_misc_group = {
	.name = "misc",
	.attrs = im818_misc_attributes
};

IM_DEV_RW_ATTR(power,		CPLD_ADDR_RST_CTL, 0x1, 7);
IM_DEV_RW_ATTR(cpu,		CPLD_ADDR_RST_CTL, 0x1, 0);
IM_DEV_RW_ATTR(tpm,		CPLD_ADDR_SD_CTL,  0x1, 4);

static struct attribute *im818_reset_attributes[] = {
	&im_dev_attr_power.dev_attr.attr,
	&im_dev_attr_cpu.dev_attr.attr,
	&im_dev_attr_tpm.dev_attr.attr,
	NULL
};

static const struct attribute_group im_reset_group = {
	.name = "reset",
	.attrs = im818_reset_attributes
};

static const struct attribute_group *im818_groups[] = {
	&im_reset_group,
	&im_misc_group,
};

IM_DEV_LED_ATTR(system, CPLD_ADDR_SYS_LED_CTL, 0);
IM_DEV_LED_ATTR(power, CPLD_ADDR_PWR_LED_CTL, 0);
IM_DEV_LED_ATTR(lan0, CPLD_ADDR_LAN_LED_CTL_0, 0);
IM_DEV_LED_ATTR(lan1, CPLD_ADDR_LAN_LED_CTL_0, 4);
IM_DEV_LED_ATTR(lan2, CPLD_ADDR_LAN_LED_CTL_1, 0);
IM_DEV_LED_ATTR(lan3, CPLD_ADDR_LAN_LED_CTL_1, 4);
IM_DEV_LED_ATTR(mgmt, CPLD_ADDR_MGMT_LED_CTL, 0);

static const struct attribute_group *im818_led_groups[] = {
	&im_led_system_group,
        &im_led_power_group,
	&im_led_lan0_group,
	&im_led_lan1_group,
	&im_led_lan2_group,
	&im_led_lan3_group,
        &im_led_mgmt_group,
};

int n818_sysfs_init(void)
{
	int err = 0;
	int i = 0;

	im818->dev = platform_device_register_simple("im818", -1, NULL, 0);

	im818->kobj = kobject_create_and_add("niagara", NULL);

	for (i = 0; i < ARRAY_SIZE(im818_groups); i++) {
		err = sysfs_create_group(im818->kobj, im818_groups[i]);
		if (err)
			goto unwind_im818_groups;
	}

	im818->kobj_led = kobject_create_and_add("led", im818->kobj);

	for (i = 0; i < ARRAY_SIZE(im818_led_groups); i++) {
		err = sysfs_create_group(im818->kobj_led, im818_led_groups[i]);
		if (err)
			goto unwind_im818_led_groups;
	}

	im818->kobj_i2c = kobject_create_and_add("i2c", im818->kobj);

	return 0;

unwind_im818_led_groups:
	/* remove groups in reverse order */
	for (i--; i >= 0; i--)
		sysfs_remove_group(im818->kobj_led, im818_led_groups[i]);

	kobject_put(im818->kobj_led);
	im818->kobj_led = NULL;

	i = ARRAY_SIZE(im818_groups);
unwind_im818_groups:
	/* remove groups in reverse order */
	for (i--; i >= 0; i--)
		sysfs_remove_group(im818->kobj, im818_groups[i]);

	kobject_put(im818->kobj);
	im818->kobj = NULL;
	platform_device_unregister(im818->dev);

	return err;
}

void n818_sysfs_remove(void)
{
	int i = 0;

	if (im818->kobj_led) {
		for (i = 0; i < ARRAY_SIZE(im818_led_groups); i++) {
			sysfs_remove_group(im818->kobj_led,
					   im818_led_groups[i]);
		}

		kobject_put(im818->kobj_led);
		im818->kobj_led = NULL;
	}

	if (im818->kobj_i2c) {
		kobject_put(im818->kobj_i2c);
		im818->kobj_i2c = NULL;
	}

	if (im818->kobj) {
		for (i = 0; i < ARRAY_SIZE(im818_groups); i++) {
			sysfs_remove_group(im818->kobj, im818_groups[i]);
		}

		kobject_put(im818->kobj);
		im818->kobj = NULL;
	}

	if (im818->dev)
		platform_device_unregister(im818->dev);
}

static struct i2c_board_info n818_temp_info = {
	I2C_BOARD_INFO("lm90", 0x4c),
};

static struct i2c_board_info n818_poe_info = {
	I2C_BOARD_INFO("poe", 0x40),
};

static struct at24_platform_data tlv_eeprom = {
        .byte_len = 65536,
        .page_size = 128,
        .flags = AT24_FLAG_ADDR16,
};

static struct i2c_board_info n818_m24512_info = {
        I2C_BOARD_INFO("at24", 0x54),
        .platform_data = &tlv_eeprom,
};

extern int i2c_bit_niagara_add_tpm_bus(struct i2c_adapter *);

int n818_i2c_init(void)
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

	client = i2c_new_device(adap, &n818_temp_info);
	if (!client) {
		pr_err("unable to add niagara i2c temperature monitor");
		return -ENODEV;
	}
	im818->i2c_temp = client;
	
	client = i2c_new_device(adap, &n818_m24512_info);
	if (!client) {
		pr_err("unable to add niagara i2c EEPROM device");
		return -ENODEV;
	}
	im818->i2c_m24512 = client;
	
	client = i2c_new_device(adap, &n818_poe_info);
		if (!client) {
		pr_err("unable to add niagara i2c POE device");
		return -ENODEV;
	}
	im818->i2c_poe = client;

	return 0;

clear_all_entries:
	niagara_del_all_i2c();
	return err;
}

void n818_i2c_deinit(void)
{
	i2c_unregister_device(im818->i2c_poe);
	im818->i2c_poe = NULL;
	
	i2c_unregister_device(im818->i2c_m24512);
	im818->i2c_m24512 = NULL;

	i2c_unregister_device(im818->i2c_temp);
	im818->i2c_temp = NULL;

	i2c_del_adapter(&niagara_tpm_i2c_root);
}
