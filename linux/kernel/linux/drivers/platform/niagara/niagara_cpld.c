#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/platform_data/niagara.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/slab.h>
#include <linux/mutex.h>

#include <linux/i2c.h>
#include <linux/i2c-mux.h>
#include <linux/i2c-algo-bit.h>

#define CPLD_LEN	0x200
#define CPLD_CHIP_SEL	6

#include "niagara_cpld.h"
#include "niagara_cpld_i2c.h"
#include "niagara-i2c-algo.h"

#include <asm/octeon/cvmx.h>

#define IMT_N818_CPLD  "imt,n818-cpld"
#define IMT_N820_CPLD  "imt,n820-cpld"
#define IMT_N8724_CPLD "imt,im8724-cpld"

extern int enable_bitbang;
extern struct semaphore octeon_bootbus_sem;

static uint8_t *niagara_cpld_base = 0;

static inline void cpldw(uint16_t offset, uint8_t val);
static inline uint8_t cpldr(uint16_t offset);

static inline void cpldw(uint16_t offset, uint8_t val)
{
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		down(&octeon_bootbus_sem);
	writeb(val, niagara_cpld_base + offset);
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		up(&octeon_bootbus_sem);
}

static inline uint8_t cpldr(uint16_t offset)
{
	uint8_t val;
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		down(&octeon_bootbus_sem);
	val = readb(niagara_cpld_base + offset);
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		up(&octeon_bootbus_sem);
	return val;
}

DEFINE_MUTEX(cpld_mutex);

/* LOCKING SCHEME
 * - the cpld_mutex protects any multi-register access, e.g. I2C or SPI.
 * For I2C transactions through either the bitbang root or the smbus root,
 * the i2c mux grabs the same mutex, so these are mutually exclusive. All
 * I2C transactions go through these muxes, so this is sufficient protection.
 */

void niagara_cpld_lock(void)
{
	mutex_lock(&cpld_mutex);
}
EXPORT_SYMBOL(niagara_cpld_lock);

void niagara_cpld_unlock(void)
{
	mutex_unlock(&cpld_mutex);
}
EXPORT_SYMBOL(niagara_cpld_unlock);

static struct i2c_adapter niagara_i2c_root;
static struct i2c_adapter niagara_i2c_bit_root;
static struct i2c_algo_bit_data niagara_i2c_bit_algo;

struct niagara_i2c_bus {
	struct list_head list;
	struct i2c_mux_core *muxc;
	int chan;
};

static LIST_HEAD(niagara_i2c_bus_list);

int niagara_add_i2c(uint16_t bus, const char *busname,
		    struct i2c_adapter **adap, int bitbang)
{
	int retval = 0;

	struct niagara_i2c_bus *l;

	int chan = bus;

	list_for_each_entry(l, &niagara_i2c_bus_list, list) {
		if (l->chan == chan) {
			pr_warning("Bus 0x%x already exists!", bus);
			return -EINVAL;
		}
	}

	l = kzalloc(sizeof(struct niagara_i2c_bus), GFP_KERNEL);
	if (!l)
		goto alloc_fail;

	l->chan = chan;

	if (enable_bitbang && bitbang)
		l->muxc = i2c_mux_alloc(&niagara_i2c_bit_root,
				      &niagara_i2c_bit_root.dev,
				      1, 0, 0,
					  niagara_i2c_mux_select, niagara_i2c_mux_deselect);
	else
		l->muxc = i2c_mux_alloc(&niagara_i2c_root,
				&niagara_i2c_root.dev,
				1, 0, 0,
				niagara_i2c_mux_select, niagara_i2c_mux_deselect);
	if (!l->muxc)
		return -ENOMEM;

	retval = i2c_mux_add_adapter(l->muxc, 0, l->chan, 0);
	if (retval) {
		pr_warning("couldn't add i2c mux, bus '%d': %d", bus, retval);
		goto i2c_add_fail;
	}

	if (adap)
		*adap = l->muxc->adapter[0];

	if (busname)
		strncpy(l->muxc->adapter[0]->name, busname, sizeof(l->muxc->adapter[0]->name));
	else
		sprintf(l->muxc->adapter[0]->name, "bus_%04x", bus);

	if (retval < 0) {
		pr_warning("couldn't add i2c bus '%d': %d", bus, retval);
		goto i2c_add_fail;
	}

	list_add(&(l->list), &niagara_i2c_bus_list);

	return 0;

i2c_add_fail:
	kfree(l);

alloc_fail:

	return retval;
}

int niagara_del_all_i2c(void)
{
	struct niagara_i2c_bus *pos, *n;
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_N818)
		n818_i2c_deinit();
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_N820)
		n820_i2c_deinit();
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_IM8724)
		im8724_i2c_deinit();
	list_for_each_entry_safe_reverse(pos, n, &niagara_i2c_bus_list, list) {
		list_del(&(pos->list));

		i2c_mux_del_adapters(pos->muxc);

		kfree(pos);
	}

	i2c_del_adapter(&niagara_i2c_root);
	if (enable_bitbang)
		i2c_del_adapter(&niagara_i2c_bit_root);

	return 0;
}

static int niagara_i2c_init(void)
{
	int err;

	memset(&niagara_i2c_root, 0, sizeof(struct i2c_adapter));
	memset(&niagara_i2c_bit_root, 0, sizeof(struct i2c_adapter));
	memset(&niagara_i2c_bit_algo, 0, sizeof(struct i2c_algo_bit_data));

	if ((niagara_cpld_read(CPLD_ADDR_CPLD_SIG_L) == 0x7C) &&
	    (niagara_cpld_read(CPLD_ADDR_CPLD_REV) & 0xf) < 2) {
		niagara_i2c_root.algo_data = &niagara_i2c_bit_algo;
		niagara_i2c_root.class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
		strcpy(niagara_i2c_root.name, "im_bitbang");
		i2c_bit_niagara_add_bus(&niagara_i2c_root, 0);
	} else {
		if (enable_bitbang) {
			niagara_i2c_bit_root.algo_data = &niagara_i2c_bit_algo;
			niagara_i2c_bit_root.class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
			strcpy(niagara_i2c_bit_root.name, "im_bitbang");
			i2c_bit_niagara_add_bus(&niagara_i2c_bit_root, 0);
		}
		niagara_i2c_bus_init();
		niagara_i2c_root.retries = 3;
		strcpy(niagara_i2c_root.name, "im_native");
		if ((err = niagara_i2c_add_bus(&niagara_i2c_root)))
			return err;
	}

	return 0;
}

uint8_t niagara_cpld_read(uint16_t offset)
{
	uint8_t val = cpldr(offset & 0xff);

	pr_devel("cpld_read: [%04x] %02x", offset, val);

	return val;
}
EXPORT_SYMBOL(niagara_cpld_read);

void niagara_cpld_write(uint16_t offset, uint8_t val)
{
	pr_devel("cpld_write: [%04x] %02x", offset, val);

	cpldw(offset & 0xff, val);
}
EXPORT_SYMBOL(niagara_cpld_write);

uint8_t niagara_cpld_remote_read(uint8_t sasl, uint16_t offset)
{
	uint8_t val, cmd;

	niagara_cpld_write(CPLD_ADDR_BA_CTL, 0x80 | (sasl & 0x7f));
	niagara_cpld_write(CPLD_ADDR_SPI_CFG, 0x80);
	niagara_cpld_write(CPLD_ADDR_SPI_ADDR_0, 0x80 | (offset & 0x7f));
	cmd = CPLD_SPI_CMD_OP_R8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST;
	niagara_cpld_write(CPLD_ADDR_SPI_CMD, cmd);

	while (niagara_cpld_read(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST)
		continue;

	val = niagara_cpld_read(CPLD_ADDR_SPI_DATA_0);

	return val;
}
EXPORT_SYMBOL(niagara_cpld_remote_read);

void niagara_cpld_remote_write(uint8_t sasl, uint16_t offset, uint8_t val)
{
	uint8_t cmd;

	niagara_cpld_write(CPLD_ADDR_BA_CTL, 0x80 | (sasl & 0x7f));
	niagara_cpld_write(CPLD_ADDR_SPI_CFG, 0x80);
	niagara_cpld_write(CPLD_ADDR_SPI_ADDR_0, offset & 0x7f);
	niagara_cpld_write(CPLD_ADDR_SPI_DATA_0, val);
	cmd = CPLD_SPI_CMD_OP_W8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST;
	niagara_cpld_write(CPLD_ADDR_SPI_CMD, cmd);

	while (niagara_cpld_read(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST)
		continue;
}
EXPORT_SYMBOL(niagara_cpld_remote_write);

ssize_t read_field(struct device *dev, struct device_attribute *d,
		   char *buf)
{
	uint8_t reg_val;
	struct im_device_attribute *attr = to_im_dev_attr(d);

	if (attr->reg & ATTR_REG_REMOTE_MASK) {
		niagara_cpld_lock();
		reg_val = niagara_cpld_remote_read(ATTR_CPLD(attr->reg),
						   ATTR_ADDR(attr->reg));
		niagara_cpld_unlock();
	} else {
		reg_val = niagara_cpld_read(attr->reg);
	}

	reg_val >>= attr->shift;
	reg_val &= attr->mask;

	return sprintf(buf, "0x%x\n", reg_val);
}

ssize_t write_field(struct device *dev, struct device_attribute *d,
		    const char *buf, size_t count)
{
	struct im_device_attribute *attr = to_im_dev_attr(d);
	uint8_t new_val = simple_strtol(buf, NULL, 0);
	uint8_t reg_val;

	if (attr->reg & ATTR_REG_REMOTE_MASK) {
		niagara_cpld_lock();
		reg_val = niagara_cpld_remote_read(ATTR_CPLD(attr->reg),
						   ATTR_ADDR(attr->reg));
	} else {
		 reg_val = niagara_cpld_read(attr->reg);
	}

	reg_val &= ~(attr->mask << attr->shift);

	new_val &= attr->mask;

	reg_val |= (new_val << attr->shift);

	if (attr->reg & ATTR_REG_REMOTE_MASK) {
		niagara_cpld_remote_write(ATTR_CPLD(attr->reg),
					  ATTR_ADDR(attr->reg),
					  reg_val);
		niagara_cpld_unlock();
	} else {
		niagara_cpld_write(attr->reg, reg_val);
	}

	return count;
}

struct i2c_client **niagara_i2c_client_list[2];

ssize_t i2c_read_field(struct device *dev, struct device_attribute *d,
		   char *buf)
{
	struct im_i2c_dev_attribute *attr = to_im_i2c_dev_attr(d);
	int32_t ret_val;
	uint8_t reg_val;

	if (!niagara_i2c_client_list[attr->lst] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x0\n");

	if ((ret_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[attr->lst][attr->cid], attr->reg)) < 0)
		return 0;
	reg_val = (uint8_t)ret_val;
	reg_val >>= attr->shift;
	reg_val &= attr->mask;
	return sprintf(buf, "0x%x\n", reg_val);
}

ssize_t i2c_write_field(struct device *dev, struct device_attribute *d,
		    const char *buf, size_t count)
{
	struct im_i2c_dev_attribute *attr = to_im_i2c_dev_attr(d);
	uint8_t new_val = simple_strtol(buf, NULL, 0);
	int32_t ret_val;
	uint8_t reg_val;

	if (!niagara_i2c_client_list[attr->lst] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return count;
	if ((ret_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[attr->lst][attr->cid], attr->reg)) < 0)
		return count;
	reg_val = (uint8_t)ret_val;
	reg_val &= ~(attr->mask << attr->shift);
	new_val &= attr->mask;
	reg_val |= (new_val << attr->shift);
	i2c_smbus_write_byte_data(niagara_i2c_client_list[attr->lst][attr->cid], attr->reg, reg_val);
	return count;
}

ssize_t sfp_read_speed(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_sfp_speed_attr *attr = to_im_sfp_speed_attr(d);
	int32_t reg_val[2];

	if (!niagara_i2c_client_list[0] || !niagara_i2c_client_list[1] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x0\n");

	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 0)) < 0)
		return sprintf(buf, "0x0\n");
	if (reg_val[0] == 0x0C || reg_val[0] == 0x0D || reg_val[0] == 0x11)
		return sprintf(buf, "0x28\n");

	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 3)) < 0)
		return sprintf(buf, "0x0\n");
	if ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 6)) < 0)
		return sprintf(buf, "0x0\n");
	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf))
		return sprintf(buf, "0xa\n");
	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf))
		return sprintf(buf, "0x1\n");

	// SFP28 detection (SFF-8024)
	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 36)) < 0)
		return sprintf(buf, "0x0\n");
	if (reg_val[0] == 0x2 || reg_val[0] == 0x3 || reg_val[0] == 0x4) {
		if ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 12)) < 0)
			return sprintf(buf, "0x0\n");
		return sprintf(buf, "0x%x\n", reg_val[1] / 10);
	}

	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 110)) < 0)
		return sprintf(buf, "0x0\n");
	if ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 118)) < 0)
		return sprintf(buf, "0x0\n");

	/* if mismatch, fixing it */
	if ((reg_val[0] & 0x08) != (reg_val[1] & 0x08)) {
		if (!(reg_val[0] & 0x08))
			reg_val[1] |= 0x08;
		else
			reg_val[1] &= ~0x08;
		if (i2c_smbus_write_byte_data(niagara_i2c_client_list[1][attr->cid], 118, (uint8_t)reg_val[1]))
			return sprintf(buf, "0x0\n");
	}
	return sprintf(buf, "0x%x\n", ((reg_val[0] & 0x08)) ? 10 : 1);
}

ssize_t sfp_write_speed(struct device *dev, struct device_attribute *d,
			const char *buf, size_t count)
{
	struct im_sfp_speed_attr *attr = to_im_sfp_speed_attr(d);
	uint8_t new_val = simple_strtol(buf, NULL, 0);
	int32_t reg_val[2];

	reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 0);
	if (reg_val[0] < 0 || reg_val[0] == 0x0C || reg_val[0] == 0x0D || reg_val[0] == 0x11)
		return count;
	if (!niagara_i2c_client_list[0] || !niagara_i2c_client_list[1] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return count;

	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 3)) < 0)
		return count;
	if ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 6)) < 0)
		return count;
	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf))
		return count;
	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf))
		return count;

	if (new_val != 1 && new_val != 10)
		return count;
	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 110)) < 0)
		return count;
	if ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 118)) < 0)
		return count;
	if (new_val == 1) {
		reg_val[0] &= ~0x08; reg_val[1] &= ~0x08;
	} else {
		reg_val[0] |= 0x08; reg_val[1] |= 0x08;
	}
	if (i2c_smbus_write_byte_data(niagara_i2c_client_list[1][attr->cid], 110, (uint8_t)reg_val[0]) ||
	    i2c_smbus_write_byte_data(niagara_i2c_client_list[1][attr->cid], 118, (uint8_t)reg_val[1]))
		return count;
	return count;
}

ssize_t sfp_read_media(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_sfp_media_attr *attr = to_im_sfp_media_attr(d);
	int32_t reg_val;

	if (!niagara_i2c_client_list[0] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x%x\n", PORT_MEDIA_UNKNOWN);

	if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 0)) < 0)
		return sprintf(buf, "0x%x\n", PORT_MEDIA_UNKNOWN);
	if (reg_val != 0x0C && reg_val != 0x0D && reg_val != 0x11) {
		if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 6)) < 0)
			return sprintf(buf, "0x%x\n", PORT_MEDIA_UNKNOWN);
		if ((reg_val & 0x8))
			return sprintf(buf, "0x%x\n", PORT_MEDIA_1000X);

		if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 12)) < 0)
			return sprintf(buf, "0x%x\n", PORT_MEDIA_UNKNOWN);
		if (reg_val < 100)
			return sprintf(buf, "0x%x\n", PORT_MEDIA_1000X);
	}

	if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 8)) < 0)
		return sprintf(buf, "0x%x\n", PORT_MEDIA_UNKNOWN);
	if ((reg_val & 0xc) == 0x00)
			return sprintf(buf, "0x%x\n", PORT_MEDIA_SR);
	return sprintf(buf, "0x%x\n", PORT_MEDIA_CX);
}

ssize_t sfp_read_temp(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_sfp_temp_attr *attr = to_im_sfp_temp_attr(d);
	int32_t reg_val[2];
	short temp = 0;

	if ((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 0)) < 0)
		return sprintf(buf, "0x0\n");

	if (reg_val[0] == 0x0C || reg_val[0] == 0x0D || reg_val[0] == 0x11) {
		if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 22)) < 0)
			|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 23)) < 0))
			return sprintf(buf, "0\n");

		temp = (int)(((short int)((reg_val[0] << 8) | reg_val[1])) / 256);
	} else {
		if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 94)) < 0)
			|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 92)) < 0))
			return sprintf(buf, "0\n");

		if((reg_val[1] & 0x40) && reg_val[0]) {
			if(reg_val[1] & 0x10) {
				short int value, value_slope, value_offset;

				if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 96)) < 0)
					|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 97)) < 0))
					return sprintf(buf, "0\n");
				value = (short int)((((uint32_t) reg_val[0]) << 8)
					| ((uint32_t) reg_val[1]));

				if(!value)
					return sprintf(buf, "0\n");

				if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 84)) < 0)
					|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 85)) < 0))
					return sprintf(buf, "0\n");
				value_slope = (short int)((((uint32_t) reg_val[0]) << 8)
					| ((uint32_t) reg_val[1]));

				if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 86)) < 0)
					|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 87)) < 0))
					return sprintf(buf, "0\n");
				value_offset = (short int)((((uint32_t) reg_val[0]) << 8)
					| ((uint32_t) reg_val[1]));

				temp = (((int) value * (int) value_slope) / 256 + (int) value_offset) / 256;
			} else {
				if (((reg_val[0] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 96)) < 0)
					|| ((reg_val[1] = i2c_smbus_read_byte_data(niagara_i2c_client_list[1][attr->cid], 97)) < 0))
					return sprintf(buf, "0\n");

				temp = (int)(((short int)((reg_val[0] << 8) | reg_val[1])) / 256);
			}
		} else
			return sprintf(buf, "0\n");
	}

	return sprintf(buf, "%d\n", (int)temp);
}

ssize_t qsfp_read_tx_dis(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_qsfp_attr *attr = to_im_qsfp_attr(d);
	int32_t reg_val;

	if (!niagara_i2c_client_list[0] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x0\n");

	if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 195)) < 0)
		return sprintf(buf, "0x0\n");

	if (!(reg_val & 0x10))
		return sprintf(buf, "0x0\n");

	if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 86)) < 0)
		return sprintf(buf, "0x0\n");

	return sprintf(buf, "0x%x\n", (reg_val & 0xf));
}

ssize_t qsfp_write_tx_dis(struct device *dev, struct device_attribute *d,
			const char *buf, size_t count)
{
	struct im_qsfp_attr *attr = to_im_qsfp_attr(d);
	uint8_t new_val = simple_strtol(buf, NULL, 0);
	int32_t reg_val;

	if (!niagara_i2c_client_list[0] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return count;

	if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 195)) < 0)
		return count;

	if (!(reg_val & 0x10))
		return count;

	i2c_smbus_write_byte_data(niagara_i2c_client_list[0][attr->cid], 86, (uint8_t)(new_val & 0xf));
	return count;
}

ssize_t qsfp_read_tx_fault(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_qsfp_attr *attr = to_im_qsfp_attr(d);
	int32_t reg_val;

        if (!niagara_i2c_client_list[0] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x0\n");

        if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 195)) < 0)
		return sprintf(buf, "0x0\n");

	if (!(reg_val & 0x8))
		return sprintf(buf, "0x0\n");

        if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 4)) < 0)
		return sprintf(buf, "0x0\n");

	return sprintf(buf, "0x%x\n", (reg_val & 0xf));
}

ssize_t qsfp_read_rx_los(struct device *dev, struct device_attribute *d, char *buf)
{
	struct im_qsfp_attr *attr = to_im_qsfp_attr(d);
	int32_t reg_val;

        if (!niagara_i2c_client_list[0] || attr->cid >= MAX_I2C_CLIENTS_NUMBER)
		return sprintf(buf, "0x0\n");

        if ((reg_val = i2c_smbus_read_byte_data(niagara_i2c_client_list[0][attr->cid], 3)) < 0)
		return sprintf(buf, "0x0\n");

	return sprintf(buf, "0x%x\n", (reg_val & 0xf));
}

static struct i2c_client *im_i2c_client_array[2][MAX_I2C_CLIENTS_NUMBER];
static struct of_device_id of_niagara_match[];

static int niagara_probe(struct platform_device *pdev)
{
	struct device_node *np;
	int err = 0;

	pr_debug("CPLD device node check...");
	np = pdev->dev.of_node;
	if (np == NULL)
		return -EINVAL;

	pr_debug("CPLD device node detected at %p", np);
	pr_debug("CPLD base address check...");
	niagara_cpld_base = of_iomap(np, 0);
	if (!niagara_cpld_base)
		return -ENODEV;

	if (of_device_is_compatible(np, IMT_N8724_CPLD)) {
		enable_bitbang = 1;
		i2c_ext_algo = 0;
	}

	pr_debug("CPLD base address %p", niagara_cpld_base);
	err = niagara_i2c_init();
	if (err) {
		pr_warn("Niagara I2C init failed!");
		goto err_niagara_i2c_out;
	}

	if (of_device_is_compatible(np, IMT_N818_CPLD)) {
		err = n818_sysfs_init();
		if (err) {
			n818_sysfs_remove();
			goto err_niagara_sysfs_out;
		}
		err = n818_i2c_init();
		if (err) {
			n818_sysfs_remove();
			goto err_niagara_platform_i2c_out;
		}
	} else if (of_device_is_compatible(np, IMT_N820_CPLD)) {
		err = n820_sysfs_init();
		if (err) {
			n820_sysfs_remove();
			goto err_niagara_sysfs_out;
		}
		err = n820_i2c_init();
		if (err) {
			n820_sysfs_remove();
			goto err_niagara_platform_i2c_out;
		}
	} else if (of_device_is_compatible(np, IMT_N8724_CPLD)) {
		niagara_i2c_client_list[0] = im_i2c_client_array[0];
		niagara_i2c_client_list[1] = im_i2c_client_array[1];

		board_link_status = im8724_board_link_status;
		board_phy_status = im8724_board_phy_status;
		err = im8724_sysfs_init();
		if (err) {
			pr_err("IM8724 sysfs initialization failed\n");
			im8724_sysfs_remove();
			goto err_niagara_sysfs_out;
		}
		err = im8724_i2c_init();
		if (err) {
			pr_err("IM8724 i2c subsystem initialization failed\n");
			im8724_sysfs_remove();
			goto err_niagara_platform_i2c_out;
		}
		err = im8724_pim_probe(0);
		if (err) {
			pr_err("IM8724 IO card A  probe failed, error: %d\n", err);
		}
		err = im8724_pim_probe(1);
		if (err) {
			pr_err("IM8724 IO card B probe failed, error: %d\n", err);
		}
	}
	return 0;

err_niagara_platform_i2c_out:
	niagara_del_all_i2c();
	niagara_i2c_client_list[0] = NULL;
	niagara_i2c_client_list[1] = NULL;
err_niagara_sysfs_out:
err_niagara_i2c_out:
	return err;
}

static int niagara_remove(struct platform_device *pdev)
{
	struct device_node *np;

	np = pdev->dev.of_node;
	if (np == NULL)
		return -EINVAL;

	iounmap(niagara_cpld_base);

	if (of_device_is_compatible(np, IMT_N818_CPLD)) {
		n818_sysfs_remove();
		niagara_del_all_i2c();
	} else if (of_device_is_compatible(np, IMT_N820_CPLD)) {
		n820_sysfs_remove();
		niagara_del_all_i2c();
	} else if (of_device_is_compatible(np, IMT_N8724_CPLD)) {
		niagara_del_all_i2c();
		im8724_sysfs_remove();

		board_link_status = 0;
		board_phy_status = 0;
        } else {
		return -ENODEV;
	}

	return 0;
}

static struct of_device_id niagara_of_match[] = {
	{ .compatible = IMT_N818_CPLD, },
	{ .compatible = IMT_N820_CPLD, },
	{ .compatible = IMT_N8724_CPLD, },
	{},
};
MODULE_DEVICE_TABLE(of, niagara_of_match);

static struct platform_driver niagara_driver = {
	.probe		= niagara_probe,
	.remove		= niagara_remove,
	.driver = {
		.name	= "niagara",
		.owner	= THIS_MODULE,
		.of_match_table = niagara_of_match,
	},
};

module_platform_driver(niagara_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Interface Masters <support@interfacemasters.com>");
MODULE_DESCRIPTION("Niagara CPLD access driver");
