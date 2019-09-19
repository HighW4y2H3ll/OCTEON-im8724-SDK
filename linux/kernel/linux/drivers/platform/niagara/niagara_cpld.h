#ifndef _NIAGARA_CPLD_H
#define _NIAGARA_CPLD_H

#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/stat.h>
#include <linux/netdevice.h>
#include <linux/phy.h>

#define IM_ATTR(_name, _mode, _show, _store, _reg, _mask, _shift) {	\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.reg = _reg,							\
	.mask = _mask,							\
	.shift = _shift							\
}

#define IM_DEV_N_RO_ATTR(_vname, _name, _reg, _mask, _shift)		\
	static struct im_device_attribute im_dev_attr_##_vname =	\
		IM_ATTR(_name, S_IRUGO, read_field,			\
			NULL, _reg, _mask, _shift)

#define IM_DEV_RO_ATTR(_name, _reg, _mask, _shift)			\
	IM_DEV_N_RO_ATTR(_name, _name, _reg, _mask, _shift)

#define IM_DEV_N_RW_ATTR(_vname, _name, _reg, _mask, _shift)		\
	static struct im_device_attribute im_dev_attr_##_vname =	\
		IM_ATTR(_name, S_IRUGO | S_IWUSR, read_field,		\
			write_field, _reg, _mask, _shift)

#define IM_DEV_RW_ATTR(_name, _reg, _mask, _shift)			\
	IM_DEV_N_RW_ATTR(_name, _name, _reg, _mask, _shift)

#define IM_DEV_LED_ATTR(_name, _reg, _shift)				\
	IM_DEV_N_RW_ATTR(_name##_mode, mode, _reg, 0x3, _shift);	\
	IM_DEV_N_RW_ATTR(_name##_color, color, _reg, 0x3, _shift + 2);	\
	static struct attribute* im_##_name##_attributes[] = {	\
		&im_dev_attr_##_name##_mode.dev_attr.attr,		\
		&im_dev_attr_##_name##_color.dev_attr.attr,		\
		NULL							\
	};								\
	static const struct attribute_group im_led_##_name##_group = {	\
		.name = #_name,						\
		.attrs= im_##_name##_attributes				\
	}

#define IM_DEV_SFP_LED_ATTR(_name, _reg, _shift)			\
	IM_DEV_N_RW_ATTR(_name##_act, act, _reg, 0x1, _shift);		\
	IM_DEV_N_RW_ATTR(_name##_blink, blink, _reg, 0x1, _shift + 1);	\
	IM_DEV_N_RW_ATTR(_name##_color, color, _reg, 0x3, _shift + 2);	\
	static struct attribute* im_sfp_led_##_name##_attributes[] = {	\
		&im_dev_attr_##_name##_act.dev_attr.attr,		\
		&im_dev_attr_##_name##_blink.dev_attr.attr,		\
		&im_dev_attr_##_name##_color.dev_attr.attr,		\
		NULL							\
	};								\
	static const struct attribute_group im_sfp_led_##_name##_group = { \
		.name = #_name,						\
		.attrs= im_sfp_led_##_name##_attributes			\
	}

struct im_device_attribute {
	struct device_attribute dev_attr;
	uint16_t reg;
	uint8_t mask;
	uint8_t shift;
};

#define IM_I2C_ATTR(_name, _mode, _show, _store, _lst, _cid, _reg, _mask, _shift) { \
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _lst,							\
	.cid = _cid,							\
	.reg = _reg,							\
	.mask = _mask,							\
	.shift = _shift							\
}

#define IM_I2C_N_RO_ATTR(_vname, _name, _lst, _cid, _reg, _mask, _shift) \
	static struct im_i2c_dev_attribute im_i2c_attr_##_vname =	\
		IM_I2C_ATTR(_name, S_IRUGO, i2c_read_field,		\
			NULL, _lst, _cid, _reg, _mask, _shift)

#define IM_I2C_RO_ATTR(_name, _lst, _cid, _reg, _mask, _shift)		\
	IM_I2C_N_RO_ATTR(_name, _lst, _cid, _name, _reg, _mask, _shift)

#define IM_I2C_N_RW_ATTR(_vname, _name, _lst, _cid, _reg, _mask, _shift) \
	static struct im_i2c_dev_attribute im_i2c_attr_##_vname =	\
		IM_I2C_ATTR(_name, S_IRUGO | S_IWUSR, i2c_read_field,	\
			i2c_write_field, _lst, _cid, _reg, _mask, _shift)

#define IM_I2C_RW_ATTR(_name, _lst, _cid, _reg, _mask, _shift)		\
	IM_I2C_N_RW_ATTR(_name, _name, _lst, _cid, _reg, _mask, _shift)

struct im_i2c_dev_attribute {
	struct device_attribute dev_attr;
	uint16_t lst;
	uint16_t cid;
	uint16_t reg;
	uint8_t mask;
	uint8_t shift;
};

#define IM_SFP_SPEED_ATTR(_name, _mode, _show, _store, _cid) {		\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}

#define IM_SFP_SPEED_N_RW_ATTR(_vname, _name, _cid)			\
	static struct im_sfp_speed_attr im_sfp_speed_attr_##_vname =	\
		IM_SFP_SPEED_ATTR(_name, S_IRUGO | S_IWUSR,		\
			sfp_read_speed, sfp_write_speed, _cid)

#define IM_SFP_SPEED_RW_ATTR(_name, _cid)				\
	IM_SFP_SPEED_N_RW_ATTR(_name, _name, _cid)

struct im_sfp_speed_attr {
	struct device_attribute dev_attr;
	uint16_t cid;
};

#define IM_QSFP_TX_DIS_N_RW_ATTR(_vname, _name, _cid, m, n)		\
	static struct im_qsfp_attr im_dev_attr_##_vname =		\
		IM_QSFP_TX_DIS_ATTR(_name, S_IRUGO | S_IWUSR,		\
			qsfp_read_tx_dis, qsfp_write_tx_dis, _cid)

#define IM_QSFP_TX_DIS_RW_ATTR(_name, _cid, m, n)			\
	IM_QSFP_TX_DIS_N_RW_ATTR(_name, _name, _cid, m, n)

#define IM_QSFP_TX_DIS_ATTR(_name, _mode, _show, _store, _cid) {	\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}

#define IM_QSFP_TX_FAULT_N_RO_ATTR(_vname, _name, _cid, m, n)		\
	static struct im_qsfp_attr im_dev_attr_##_vname =		\
		IM_QSFP_TX_FAULT_ATTR(_name, S_IRUGO | S_IWUSR,	\
			qsfp_read_tx_fault, NULL, _cid)

#define IM_QSFP_TX_FAULT_RO_ATTR(_name, _cid, m, n)			\
	IM_QSFP_TX_FAULT_N_RO_ATTR(_name, _name, _cid, m, n)

#define IM_QSFP_TX_FAULT_ATTR(_name, _mode, _show, _store, _cid) {	\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}

#define IM_QSFP_RX_LOS_N_RO_ATTR(_vname, _name, _cid, m, n)		\
	static struct im_qsfp_attr im_dev_attr_##_vname =		\
		IM_QSFP_RX_LOS_ATTR(_name, S_IRUGO | S_IWUSR,		\
			qsfp_read_rx_los, NULL, _cid)

#define IM_QSFP_RX_LOS_RO_ATTR(_name, _cid, m, n)			\
	IM_QSFP_RX_LOS_N_RO_ATTR(_name, _name, _cid, m, n)

#define IM_QSFP_RX_LOS_ATTR(_name, _mode, _show, _store, _cid) {	\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}
struct im_qsfp_attr {
	struct device_attribute dev_attr;
	uint16_t cid;
};

#define IM_SFP_MEDIA_ATTR(_name, _mode, _show, _store, _cid) {		\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}

#define IM_SFP_MEDIA_N_RO_ATTR(_vname, _name, _cid)			\
	static struct im_sfp_media_attr im_sfp_media_attr_##_vname =	\
		IM_SFP_MEDIA_ATTR(_name, S_IRUGO | S_IWUSR,		\
			sfp_read_media, NULL, _cid)

#define IM_SFP_MEDIA_RO_ATTR(_name, _cid)				\
	IM_SFP_MEDIA_N_RO_ATTR(_name, _name, _cid)

struct im_sfp_media_attr {
	struct device_attribute dev_attr;
	uint16_t cid;
};

#define IM_SFP_TEMP_ATTR(_name, _mode, _show, _store, _cid) {		\
	.dev_attr = __ATTR(_name, _mode, _show, _store),		\
	.cid = _cid,							\
}

#define IM_SFP_TEMP_N_RO_ATTR(_vname, _name, _cid)			\
	static struct im_sfp_temp_attr im_sfp_temp_attr_##_vname =	\
		IM_SFP_TEMP_ATTR(_name, S_IRUGO | S_IWUSR,		\
			sfp_read_temp, NULL, _cid)

#define IM_SFP_TEMP_RO_ATTR(_name, _cid)				\
	IM_SFP_TEMP_N_RO_ATTR(_name, _name, _cid)

struct im_sfp_temp_attr {
	struct device_attribute dev_attr;
	uint16_t cid;
};

#define ATTR_REG_REMOTE_MASK	0x8000
#define ATTR_REG_SASL_MASK	0x7f00

#define ATTR_CPLD(x)  ((x & ATTR_REG_SASL_MASK) >> 8)
#define ATTR_ADDR(x)  (x & 0xff)

#define to_im_dev_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_device_attribute, dev_attr)

#define to_im_i2c_dev_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_i2c_dev_attribute, dev_attr)

#define to_im_sfp_speed_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_sfp_speed_attr, dev_attr)

#define to_im_sfp_media_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_sfp_media_attr, dev_attr)

#define to_im_sfp_temp_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_sfp_temp_attr, dev_attr)

#define to_im_qsfp_attr(_dev_attr)					\
	container_of(_dev_attr, struct im_qsfp_attr, dev_attr)

#define MAX_I2C_CLIENTS_NUMBER	40

enum port_media_mode_e {
	PORT_MEDIA_SR,      /** Optical */
	PORT_MEDIA_CX,      /** Passive copper */
	PORT_MEDIA_1000X,   /** 1000Base-X */
	PORT_MEDIA_UNKNOWN, /** Unknown/default mode */
	PORT_MEDIA_MAX = PORT_MEDIA_UNKNOWN
};

extern struct i2c_client **niagara_i2c_client_list[2];

ssize_t read_field(struct device *, struct device_attribute *, char *);
ssize_t write_field(struct device *, struct device_attribute *,
		    const char *, size_t);

ssize_t i2c_read_field(struct device *, struct device_attribute *, char *);
ssize_t i2c_write_field(struct device *, struct device_attribute *,
		    const char *, size_t);

ssize_t sfp_read_speed(struct device *dev, struct device_attribute *d, char *buf);
ssize_t sfp_write_speed(struct device *dev, struct device_attribute *d,
			const char *buf, size_t count);

ssize_t sfp_read_media(struct device *dev, struct device_attribute *d, char *buf);

ssize_t sfp_read_temp(struct device *dev, struct device_attribute *d, char *buf);

ssize_t qsfp_read_tx_fault(struct device *dev, struct device_attribute *d, char *buf);
ssize_t qsfp_read_tx_dis(struct device *dev, struct device_attribute *d, char *buf);
ssize_t qsfp_write_tx_dis(struct device *dev, struct device_attribute *d,
			 const char *buf, size_t count);
ssize_t qsfp_read_rx_los(struct device *dev, struct device_attribute *d, char *buf);

int	niagara_add_i2c(uint16_t, const char *, struct i2c_adapter **, int);

int	n818_sysfs_init(void);
void n818_sysfs_remove(void);
int	n818_i2c_init(void);
void n818_i2c_deinit(void);

int	n820_sysfs_init(void);
void n820_sysfs_remove(void);
int	n820_i2c_init(void);
void n820_i2c_deinit(void);

int	im8724_sysfs_init(void);
void im8724_sysfs_remove(void);
int	im8724_i2c_init(void);
void im8724_i2c_deinit(void);
int	im8724_pim_probe(int);
int	im8724_board_link_status(u32 interface, u32 port_index);
int	im8724_board_phy_status(struct phy_device *phydev);
int	im13233_board_link_status(u32 interface, u32 port_index);
int	im13264_board_link_status(u32 interface, u32 port_index);
int	im13262_board_link_status(u32 interface, u32 port_index);
int	im13233_init(int, struct kobject *);
int	im13264_init(int, struct kobject *);
int	im13262_init(int, struct kobject *);

int	niagara_del_all_i2c(void);

void cvm_oct_sfp_i2c_set(struct net_device *, struct i2c_client *);
void cvm_oct_phy_i2c_set(struct net_device *, struct i2c_client *);

extern int(* board_link_status)(u32 interface, u32 port_index);
extern int(* board_phy_status)(struct phy_device *phydev);

/*
 * IO Modules CPLD Signatures
 */

#define CPLD_IM13233_SIG_VL      0x4C
#define CPLD_IM13264_SIG_VL      0x57
#define CPLD_IM13262_SIG_VL      0x5f

/*
 * CPLD Common Addresses
 */
#define CPLD_ADDR_CPLD_SIG_L		0x00
#define CPLD_ADDR_CPLD_SIG_H		0x01
#define CPLD_ADDR_CPLD_REV		0x02

#define CPLD_ADDR_SD_CTL		0x1b

/*
 * I2C interface.
 */
#ifdef NIAGARA_SMBUS_XFER
#define CPLD_ADDR_I2C_BLOCK		0x38
#define CPLD_ADDR_I2C_CSR		(CPLD_ADDR_I2C_BLOCK + 0)
#define CPLD_ADDR_I2C_DA		(CPLD_ADDR_I2C_BLOCK + 1)
#define CPLD_ADDR_I2C_DATA		(CPLD_ADDR_I2C_BLOCK + 2)
#define CPLD_ADDR_I2C_STAT		(CPLD_ADDR_I2C_BLOCK + 3)
#define CPLD_ADDR_I2C_ADDR_0		(CPLD_ADDR_I2C_BLOCK + 4)
#define CPLD_ADDR_I2C_ADDR_1		(CPLD_ADDR_I2C_BLOCK + 5)
#define CPLD_ADDR_I2C_CTL		(CPLD_ADDR_I2C_BLOCK + 6)

#define CPLD_I2C_CTL_RATE_1000		0x00
#define CPLD_I2C_CTL_RATE_500		0x10
#define CPLD_I2C_CTL_RATE_100		0x20
#define CPLD_I2C_CTL_RATE_50		0x30

#define CPLD_I2C_STAT_ACT		0x01
#endif

#define CPLD_BA_CTL_EB_EN		0x80
#define CPLD_I2C_CSR_WRITE		0x10
#define CPLD_I2C_CSR_READ		0x00
#define CPLD_I2C_CSR_ACK_POLL		0x20
#define CPLD_I2C_CSR_ST			0x01
#define CPLD_I2C_STAT_MASK		0x07

#define CPLD_ADDR_I2C_CMD		0x38
#ifndef NIAGARA_SMBUS_XFER
#define CPLD_ADDR_I2C_CTL		0x3e
#define CPLD_ADDR_I2C_STAT		0x39
#endif
#define CPLD_ADDR_I2C_MCTL		0x3a
#define CPLD_ADDR_I2C_DEV_ID0		0x3b
#define CPLD_ADDR_I2C_DEV_ID1		0x3c
#define CPLD_ADDR_I2C_DB1		0x3d
#define CPLD_ADDR_I2C_DB2		0x3e
#define CPLD_ADDR_I2C_DB3		0x3f

#ifndef NIAGARA_SMBUS_XFER
#define CPLD_I2C_CTL_RATE_1000		0x00
#define CPLD_I2C_CTL_RATE_500		0x40
#define CPLD_I2C_CTL_RATE_100		0x80
#define CPLD_I2C_CTL_RATE_50		0xc0
#endif

#define CPLD_I2C_CTL_SDA_SCL_HIGH	0x0C

#define CPLD_I2C_CTL_SDA_OUT		0x08
#define CPLD_I2C_CTL_SCL_OUT		0x04
#define CPLD_I2C_CTL_SDA_IN		0x02
#define CPLD_I2C_CTL_SCL_IN		0x01

#define CPLD_I2C_STAT_ERR_MASK		0x3C
#define CPLD_I2C_STAT_BUSY		0x01
#define CPLD_I2C_STAT_RESET		0x01

#endif
