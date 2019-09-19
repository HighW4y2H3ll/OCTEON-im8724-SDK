#include <linux/types.h>
#include <linux/sysfs.h>
#include <linux/platform_data/niagara.h>
#include <linux/platform_data/at24.h>

#include "niagara_cpld.h"

#define PIM0_I2C_SFP_CLIENTS_START	10
#define PIM1_I2C_SFP_CLIENTS_START	20

#define CPLD_ADDR_SFP_RX_LOS		0x0a

#define IM13233_SFP_ATTR(m, n)						\
	IM_DEV_N_RO_ATTR(pim##m##_sfp##n##_pres, pres,			\
			 0x8008 + m*0x100, 0x1, n);			\
	IM_DEV_N_RW_ATTR(pim##m##_sfp##n##_tx_dis, tx_dis,		\
			 0x8009 + m*0x100, 0x1, n);			\
	IM_DEV_N_RO_ATTR(pim##m##_sfp##n##_rx_los, rx_los,		\
			 0x800a + m*0x100, 0x1, n);			\
	IM_DEV_N_RO_ATTR(pim##m##_sfp##n##_tx_fault, tx_fault,		\
			 0x800b + m*0x100, 0x1, n);			\
	static struct attribute *im_pim##m##_sfp##n##_attrs[] = {	\
		&im_dev_attr_pim##m##_sfp##n##_pres.dev_attr.attr,	\
		&im_dev_attr_pim##m##_sfp##n##_tx_dis.dev_attr.attr,	\
		&im_dev_attr_pim##m##_sfp##n##_rx_los.dev_attr.attr,	\
		&im_dev_attr_pim##m##_sfp##n##_tx_fault.dev_attr.attr,	\
		NULL							\
	};								\
	static struct attribute_group im_pim##m##_sfp##n##_group = {	\
		.name = "sfp" #n,					\
		.attrs = im_pim##m##_sfp##n##_attrs			\
	}

IM13233_SFP_ATTR(0, 0);
IM13233_SFP_ATTR(0, 1);
IM13233_SFP_ATTR(0, 2);
IM13233_SFP_ATTR(0, 3);
IM13233_SFP_ATTR(0, 4);
IM13233_SFP_ATTR(0, 5);
IM13233_SFP_ATTR(0, 6);
IM13233_SFP_ATTR(0, 7);
IM13233_SFP_ATTR(1, 0);
IM13233_SFP_ATTR(1, 1);
IM13233_SFP_ATTR(1, 2);
IM13233_SFP_ATTR(1, 3);
IM13233_SFP_ATTR(1, 4);
IM13233_SFP_ATTR(1, 5);
IM13233_SFP_ATTR(1, 6);
IM13233_SFP_ATTR(1, 7);

#define IM13233_DIAG_ATTR(m, n, k)						\
	IM_SFP_SPEED_N_RW_ATTR(pim##m##_diag##n##_speed, speed, k);		\
	IM_SFP_MEDIA_N_RO_ATTR(pim##m##_diag##n##_media, media, k);		\
	IM_SFP_TEMP_N_RO_ATTR(pim##m##_diag##n##_temp, temp, k);		\
	static struct attribute *im_pim##m##_diag##n##_attrs[] = {		\
		&im_sfp_speed_attr_pim##m##_diag##n##_speed.dev_attr.attr,	\
		&im_sfp_media_attr_pim##m##_diag##n##_media.dev_attr.attr,	\
		&im_sfp_temp_attr_pim##m##_diag##n##_temp.dev_attr.attr,	\
		NULL								\
	};									\
	static struct attribute_group im_pim##m##_diag##n##_group = {		\
		.name = "sfp" #n,						\
		.attrs = im_pim##m##_diag##n##_attrs				\
	}

IM13233_DIAG_ATTR(0, 0, PIM0_I2C_SFP_CLIENTS_START);
IM13233_DIAG_ATTR(0, 1, (PIM0_I2C_SFP_CLIENTS_START+1));
IM13233_DIAG_ATTR(0, 2, (PIM0_I2C_SFP_CLIENTS_START+2));
IM13233_DIAG_ATTR(0, 3, (PIM0_I2C_SFP_CLIENTS_START+3));
IM13233_DIAG_ATTR(0, 4, (PIM0_I2C_SFP_CLIENTS_START+4));
IM13233_DIAG_ATTR(0, 5, (PIM0_I2C_SFP_CLIENTS_START+5));
IM13233_DIAG_ATTR(0, 6, (PIM0_I2C_SFP_CLIENTS_START+6));
IM13233_DIAG_ATTR(0, 7, (PIM0_I2C_SFP_CLIENTS_START+7));
IM13233_DIAG_ATTR(1, 0, PIM1_I2C_SFP_CLIENTS_START);
IM13233_DIAG_ATTR(1, 1, (PIM1_I2C_SFP_CLIENTS_START+1));
IM13233_DIAG_ATTR(1, 2, (PIM1_I2C_SFP_CLIENTS_START+2));
IM13233_DIAG_ATTR(1, 3, (PIM1_I2C_SFP_CLIENTS_START+3));
IM13233_DIAG_ATTR(1, 4, (PIM1_I2C_SFP_CLIENTS_START+4));
IM13233_DIAG_ATTR(1, 5, (PIM1_I2C_SFP_CLIENTS_START+5));
IM13233_DIAG_ATTR(1, 6, (PIM1_I2C_SFP_CLIENTS_START+6));
IM13233_DIAG_ATTR(1, 7, (PIM1_I2C_SFP_CLIENTS_START+7));

#define IM13233_LED_ATTR(_pim, _name, _reg, _shift)			\
	IM_DEV_N_RW_ATTR(pim##_pim##_##_name##_led_mode, mode, _reg, 0x3, _shift); \
	IM_DEV_N_RW_ATTR(pim##_pim##_##_name##_led_color, color, _reg, 0x3, _shift + 2); \
	static struct attribute* im_pim##_pim##_##_name##_led_attrs[] = { \
		&im_dev_attr_pim##_pim##_##_name##_led_mode.dev_attr.attr, \
		&im_dev_attr_pim##_pim##_##_name##_led_color.dev_attr.attr, \
		NULL							\
	};								\
	static struct attribute_group im_pim##_pim##_##_name##_led_group = { \
		.name	= #_name,					\
		.attrs	= im_pim##_pim##_##_name##_led_attrs		\
	}

IM13233_LED_ATTR(0, system, 0x8016, 0);
IM13233_LED_ATTR(1, system, 0x8116, 0);

#define IM13233_CFG_LED_ATTR(_pim, _reg)				\
	IM_DEV_N_RW_ATTR(pim##_pim##_cfg_led_act, phy_led_act, _reg, 0x1, 0);	\
	IM_DEV_N_RW_ATTR(pim##_pim##_cfg_led_col, phy_led_col, _reg, 0x1, 1);	\
	static struct attribute* im_pim##_pim##_cfg_led_attrs[] = {	\
		&im_dev_attr_pim##_pim##_cfg_led_act.dev_attr.attr, 	\
		&im_dev_attr_pim##_pim##_cfg_led_col.dev_attr.attr, 	\
		NULL							\
	};								\
	static struct attribute_group im_pim##_pim##_cfg_led_group = {	\
		.name	= "cfg",					\
		.attrs	= im_pim##_pim##_cfg_led_attrs			\
	}

IM13233_CFG_LED_ATTR(0, 0x8048);
IM13233_CFG_LED_ATTR(1, 0x8148);

#define IM13233_SFP_LED_ATTR(_pim, _offset, _name, _reg, _shift)			\
	IM_DEV_N_RW_ATTR(pim##_pim##_##_name##_led_lc, lc, _reg, 0x3, _shift); \
	IM_DEV_N_RW_ATTR(pim##_pim##_##_name##_led_col, col, _reg, 0x3, _shift + 2); \
	IM_DEV_N_RW_ATTR(pim##_pim##_##_name##_led_index, index, _reg - _offset, 0xff, 0); \
	static struct attribute* im_pim##_pim##_##_name##_led_attrs[] = { \
		&im_dev_attr_pim##_pim##_##_name##_led_lc.dev_attr.attr, \
		&im_dev_attr_pim##_pim##_##_name##_led_col.dev_attr.attr, \
		&im_dev_attr_pim##_pim##_##_name##_led_index.dev_attr.attr, \
		NULL							\
	};								\
	static struct attribute_group im_pim##_pim##_##_name##_led_group = { \
		.name	= #_name,					\
		.attrs	= im_pim##_pim##_##_name##_led_attrs		\
	}

IM13233_SFP_LED_ATTR(0, 2, sfp0, 0x804c, 0);
IM13233_SFP_LED_ATTR(0, 1, sfp1, 0x804c, 4);
IM13233_SFP_LED_ATTR(0, 2, sfp2, 0x804f, 0);
IM13233_SFP_LED_ATTR(0, 1, sfp3, 0x804f, 4);
IM13233_SFP_LED_ATTR(0, 2, sfp4, 0x8052, 0);
IM13233_SFP_LED_ATTR(0, 1, sfp5, 0x8052, 4);
IM13233_SFP_LED_ATTR(0, 2, sfp6, 0x8055, 0);
IM13233_SFP_LED_ATTR(0, 1, sfp7, 0x8055, 4);

IM13233_SFP_LED_ATTR(1, 2, sfp0, 0x814c, 0);
IM13233_SFP_LED_ATTR(1, 1, sfp1, 0x814c, 4);
IM13233_SFP_LED_ATTR(1, 2, sfp2, 0x814f, 0);
IM13233_SFP_LED_ATTR(1, 1, sfp3, 0x814f, 4);
IM13233_SFP_LED_ATTR(1, 2, sfp4, 0x8152, 0);
IM13233_SFP_LED_ATTR(1, 1, sfp5, 0x8152, 4);
IM13233_SFP_LED_ATTR(1, 2, sfp6, 0x8155, 0);
IM13233_SFP_LED_ATTR(1, 1, sfp7, 0x8155, 4);

#define IM13233_MISC_ATTR(m)                                      \
        IM_DEV_N_RO_ATTR(pim##m##_cpld_sig_l, cpld_sig_l,         \
                         0x8000 + m*0x100, 0xff, 0);              \
        IM_DEV_N_RO_ATTR(pim##m##_cpld_sig_h, cpld_sig_h,         \
                         0x8001 + m*0x100, 0xff, 0);              \
        IM_DEV_N_RO_ATTR(pim##m##_cpld_rev, cpld_rev,             \
                         0x8002 + m*0x100, 0xf, 0);               \
        IM_DEV_N_RW_ATTR(pim##m##_phy_rst, phy_rst,               \
                         0x8004 + m*0x100, 0x1, 0);               \
        IM_DEV_N_RW_ATTR(pim##m##_s_rst, s_rst,                   \
                         0x8004 + m*0x100, 0x1, 7);               \
        IM_DEV_N_RO_ATTR(pim##m##_tm_alert, tm_alert,             \
                         0x8007 + m*0x100, 0x1, 0);               \
        IM_DEV_N_RO_ATTR(pim##m##_tm_overt, tm_overt,             \
                         0x8007 + m*0x100, 0x1, 1);               \
        IM_DEV_N_RO_ATTR(pim##m##_ready, ready,                   \
                         0x8007 + m*0x100, 0x1, 7);               \
        IM_DEV_N_RO_ATTR(pim##m##_temp_ev, temp_ev,               \
                         0x8010 + m*0x100, 0x1, 1);               \
        IM_DEV_N_RO_ATTR(pim##m##_phy_ev, phy_ev,                 \
                         0x8010 + m*0x100, 0x1, 2);               \
        IM_DEV_N_RO_ATTR(pim##m##_pbtn_ev, pbtn_ev,               \
                         0x8010 + m*0x100, 0x1, 6);               \
        IM_DEV_N_RW_ATTR(pim##m##_temp_en, temp_en,               \
                         0x8011 + m*0x100, 0x1, 1);               \
        IM_DEV_N_RW_ATTR(pim##m##_phy_en, phy_en,                 \
                         0x8011 + m*0x100, 0x1, 2);               \
        IM_DEV_N_RW_ATTR(pim##m##_pbtn_en, pbtn_en,               \
                         0x8011 + m*0x100, 0x1, 6);               \
        static struct attribute* im_pim##m##_misc_attrs[] = {     \
                &im_dev_attr_pim##m##_cpld_sig_l.dev_attr.attr,   \
                &im_dev_attr_pim##m##_cpld_sig_h.dev_attr.attr,   \
                &im_dev_attr_pim##m##_cpld_rev.dev_attr.attr,     \
                &im_dev_attr_pim##m##_phy_rst.dev_attr.attr,      \
                &im_dev_attr_pim##m##_s_rst.dev_attr.attr,        \
                &im_dev_attr_pim##m##_tm_alert.dev_attr.attr,     \
                &im_dev_attr_pim##m##_tm_overt.dev_attr.attr,     \
                &im_dev_attr_pim##m##_ready.dev_attr.attr,        \
                &im_dev_attr_pim##m##_temp_ev.dev_attr.attr,      \
                &im_dev_attr_pim##m##_phy_ev.dev_attr.attr,       \
                &im_dev_attr_pim##m##_pbtn_ev.dev_attr.attr,      \
                &im_dev_attr_pim##m##_temp_en.dev_attr.attr,      \
                &im_dev_attr_pim##m##_phy_en.dev_attr.attr,       \
                &im_dev_attr_pim##m##_pbtn_en.dev_attr.attr,      \
                NULL                                              \
        };                                                        \
        static struct attribute_group im_pim##m##_misc_group = {  \
                .name   = "misc",                                 \
                .attrs  = im_pim##m##_misc_attrs                  \
        }

IM13233_MISC_ATTR(0);
IM13233_MISC_ATTR(1);

static ssize_t type_show(struct kobject *kobj, struct kobj_attribute *attr,
			 char *buf)
{
	return sprintf(buf, "%s\n", "im13233");
}

static struct kobj_attribute pim_type = __ATTR_RO(type);

struct im13233 {
	struct platform_device *pdev;
	struct kobject *kobj;
	struct kobject *kobj_led;
	struct kobject *kobj_i2c;
	struct attribute_group *sfp_group[8];
	struct attribute_group *led_group[10];
	struct attribute_group *misc_group;
	struct attribute_group *diag_group[8];
} im13233[] = {
	{
		.sfp_group = {
			&im_pim0_sfp0_group,
			&im_pim0_sfp1_group,
			&im_pim0_sfp2_group,
			&im_pim0_sfp3_group,
			&im_pim0_sfp4_group,
			&im_pim0_sfp5_group,
			&im_pim0_sfp6_group,
			&im_pim0_sfp7_group,
		},
		.led_group = {
			&im_pim0_cfg_led_group,
			&im_pim0_system_led_group,
			&im_pim0_sfp0_led_group,
			&im_pim0_sfp1_led_group,
			&im_pim0_sfp2_led_group,
			&im_pim0_sfp3_led_group,
			&im_pim0_sfp4_led_group,
			&im_pim0_sfp5_led_group,
			&im_pim0_sfp6_led_group,
			&im_pim0_sfp7_led_group,
		},
		.misc_group =
			&im_pim0_misc_group,
		.diag_group = {
			&im_pim0_diag0_group,
			&im_pim0_diag1_group,
			&im_pim0_diag2_group,
			&im_pim0_diag3_group,
			&im_pim0_diag4_group,
			&im_pim0_diag5_group,
			&im_pim0_diag6_group,
			&im_pim0_diag7_group,
		},
	},
	{
		.sfp_group = {
			&im_pim1_sfp0_group,
			&im_pim1_sfp1_group,
			&im_pim1_sfp2_group,
			&im_pim1_sfp3_group,
			&im_pim1_sfp4_group,
			&im_pim1_sfp5_group,
			&im_pim1_sfp6_group,
			&im_pim1_sfp7_group,
		},
		.led_group = {
			&im_pim1_cfg_led_group,
			&im_pim1_system_led_group,
			&im_pim1_sfp0_led_group,
			&im_pim1_sfp1_led_group,
			&im_pim1_sfp2_led_group,
			&im_pim1_sfp3_led_group,
			&im_pim1_sfp4_led_group,
			&im_pim1_sfp5_led_group,
			&im_pim1_sfp6_led_group,
			&im_pim1_sfp7_led_group,
		},
		.misc_group =
			&im_pim1_misc_group,
		.diag_group = {
			&im_pim1_diag0_group,
			&im_pim1_diag1_group,
			&im_pim1_diag2_group,
			&im_pim1_diag3_group,
			&im_pim1_diag4_group,
			&im_pim1_diag5_group,
			&im_pim1_diag6_group,
			&im_pim1_diag7_group,
		},
	}
};

static int im13233_i2c_init(int id);

int im13233_init(int id, struct kobject *parent)
{
	int i, err;
	char buf[32];

	im13233[id].pdev = platform_device_register_simple("im13233", id, NULL, 0);
	if (!im13233[id].pdev)
		return -ENOMEM;

	snprintf(buf, sizeof(buf), "pim%d", id);
	im13233[id].kobj = kobject_create_and_add(buf, parent);
	if (!im13233[id].kobj) {
		err = -ENODEV;
		goto out_pdev;
	}

	/* Set the LED_CFG reg to let SW CS4343 phy control */
	niagara_cpld_lock();
	niagara_cpld_remote_write(id, 0x48, 3);
	niagara_cpld_unlock();

	im13233[id].kobj_led = kobject_create_and_add("led", im13233[id].kobj);
	if (!im13233[id].kobj_led) {
		err = -ENODEV;
		goto out_kobj;
	}

	err = sysfs_create_file(im13233[id].kobj, &pim_type.attr);
	if (err)
		goto out_kobj_led;

	for (i = 0; i < ARRAY_SIZE(im13233[id].sfp_group); i++) {
		err = sysfs_create_group(im13233[id].kobj, im13233[id].sfp_group[i]);
		if (err)
			goto out_sfp_group;
	}

	for (i = 0; i < ARRAY_SIZE(im13233[id].led_group); i++) {
		err = sysfs_create_group(im13233[id].kobj_led, im13233[id].led_group[i]);
		if (err)
			goto out_led_group;
	}

	err = sysfs_create_group(im13233[id].kobj, im13233[id].misc_group);
	if (err)
		goto out_misc_group;

	err = im13233_i2c_init(id);
	if (err)
		goto out_led_group;

	return 0;

out_misc_group:
	sysfs_remove_group(im13233[id].kobj, im13233[id].misc_group);

out_led_group:
	for (i--; i >= 0; i--)
		sysfs_remove_group(im13233[id].kobj_led, im13233[id].led_group[i]);
	i = ARRAY_SIZE(im13233[id].sfp_group);
out_sfp_group:
	for (i--; i >= 0; i--)
		sysfs_remove_group(im13233[id].kobj, im13233[id].sfp_group[i]);
	sysfs_remove_file(im13233[id].kobj, &pim_type.attr);
out_kobj_led:
	kobject_put(im13233[id].kobj_led);
out_kobj:
	kobject_put(im13233[id].kobj);
out_pdev:
	platform_device_del(im13233[id].pdev);
	return err;
}

static struct i2c_board_info temp_info = {
	I2C_BOARD_INFO("adm1032", 0x4c),
};
static struct i2c_client *i2c_adm1032[2] = { 0 };

static struct at24_platform_data sfp_eeprom_data = {
	.byte_len       = 256,
	.page_size      = 1,
	.flags          = AT24_FLAG_READONLY,
};

static struct at24_platform_data sfp_diag_data = {
	.byte_len       = 256,
	.page_size      = 1,
	.flags          = 0,
};

static struct i2c_board_info sfp_eeprom_info = {
	I2C_BOARD_INFO("24c02", 0x50),
	.platform_data  = &sfp_eeprom_data,
};
static struct i2c_client *i2c_24c02[2][8];

static struct i2c_board_info sfp_diag_info = {
	I2C_BOARD_INFO("24c02", 0x51),
	.platform_data  = &sfp_diag_data,
};

static int im13233_i2c_init(int id)
{
	struct i2c_adapter *adap;
	u16 base = 0x8000 + id * 0x100;
	int shift = id ?PIM1_I2C_SFP_CLIENTS_START:
			PIM0_I2C_SFP_CLIENTS_START;
	char name[32];
	int err, indx;

	snprintf(name, sizeof(name), "pim%d_temp", id);
	err = niagara_add_i2c(base + 9, name, &adap, 0);
	if (err)
		return err;

	i2c_adm1032[id] = i2c_new_device(adap, &temp_info);
	if (i2c_adm1032 == NULL) {
		return -ENODEV;
	}

	for (indx = 0; indx < 8; indx++) {
		snprintf(name, sizeof(name), "pim%d_sfp%d", id, indx);
		err = niagara_add_i2c(base + indx, name, &adap, 0);
		if (err)
			return err;

		i2c_24c02[id][indx] = i2c_new_device(adap, &sfp_eeprom_info);
		if (!i2c_24c02[id][indx])
			return -ENODEV;
		if (niagara_i2c_client_list[0])
			niagara_i2c_client_list[0][shift + indx] = i2c_24c02[id][indx];

		if (niagara_i2c_client_list[1]) {
			niagara_i2c_client_list[1][shift + indx] = i2c_new_device(adap, &sfp_diag_info);
			if (!niagara_i2c_client_list[1][shift + indx])
				return -ENODEV;
		}
	}

	im13233[id].kobj_i2c = kobject_create_and_add("diag", im13233[id].kobj);
	if (!im13233[id].kobj_i2c) {
		return -ENODEV;
	}

	for (indx = 0; indx < ARRAY_SIZE(im13233[id].diag_group); indx++) {
		err = sysfs_create_group(im13233[id].kobj_i2c, im13233[id].diag_group[indx]);
		if (err)
			return -ENODEV;
	}
	pr_info("pim%d: im13233 probed\n", id);
	return 0;
}

int im13233_i2c_deinit(int id)
{
	int shift = id ?PIM1_I2C_SFP_CLIENTS_START:
			PIM0_I2C_SFP_CLIENTS_START;
	int indx;

	if (i2c_adm1032[id]) {
		i2c_unregister_device(i2c_adm1032[id]);
		i2c_adm1032[id] = NULL;
	}

	if (im13233[id].kobj_i2c) {
		for (indx = 0; indx < ARRAY_SIZE(im13233[id].diag_group); indx++)
			sysfs_remove_group(im13233[id].kobj_i2c, im13233[id].diag_group[indx]);
	}

	for (indx = 0; indx < 8; indx++) {
		if (i2c_24c02[id][indx]) {
			i2c_unregister_device(i2c_24c02[id][indx]);
			if (niagara_i2c_client_list[0] && niagara_i2c_client_list[0][shift+indx])
				niagara_i2c_client_list[0][shift+indx] = NULL;
			i2c_24c02[id][indx] = NULL;
		}
		if (niagara_i2c_client_list[1] && niagara_i2c_client_list[1][shift+indx]) {
			i2c_unregister_device(niagara_i2c_client_list[1][shift+indx]);
			niagara_i2c_client_list[1][shift+indx] = NULL;
		}
	}
	return 0;
}

int im13233_board_link_status(u32 interface, u32 port_index)
{
	int slot = interface < 4 ? 1 : 0;
	int link_status = 0;

	if (interface >= 2 && interface <= 5 && port_index < 4) {
		niagara_cpld_lock();
		link_status = ! (niagara_cpld_remote_read(slot, CPLD_ADDR_SFP_RX_LOS) &
					(0x01 << (port_index + 4 * (1 - interface % 2))));
		niagara_cpld_unlock();
		pr_devel("SFP Rx loss is %d for iface %d with and port %d.\n",
			!link_status, interface, port_index);
	}
	return link_status;
}
