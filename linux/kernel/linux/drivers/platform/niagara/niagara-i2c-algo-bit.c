#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/platform_data/niagara.h>

#include "niagara_cpld.h"

struct i2c_niagara_bit_data {
	uint8_t sda_set_mask;
	uint8_t sda_get_mask;
	uint8_t scl_set_mask;
	uint8_t scl_get_mask;
	uint16_t ctl_reg;
};

static const struct i2c_niagara_bit_data i2c_bit_data = {
	.ctl_reg = CPLD_ADDR_I2C_CTL,
	.scl_get_mask = CPLD_I2C_CTL_SCL_IN,
	.scl_set_mask = CPLD_I2C_CTL_SCL_OUT,
	.sda_get_mask = CPLD_I2C_CTL_SDA_IN,
	.sda_set_mask = CPLD_I2C_CTL_SDA_OUT
};

static void i2c_niagara_setsda(void *data, int state)
{
	const struct i2c_niagara_bit_data *bit_data = &i2c_bit_data;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg) & ~bit_data->sda_set_mask;

	if (state)
		ctl |= bit_data->sda_set_mask;
	niagara_cpld_write(bit_data->ctl_reg, ctl);
}

static void i2c_niagara_setscl(void *data, int state)
{
	const struct i2c_niagara_bit_data *bit_data = &i2c_bit_data;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg) & ~bit_data->scl_set_mask;

	if (state)
		ctl |= bit_data->scl_set_mask;
	niagara_cpld_write(bit_data->ctl_reg, ctl);
}

static int i2c_niagara_getsda(void *data)
{
	const struct i2c_niagara_bit_data *bit_data = &i2c_bit_data;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg);

	return ((ctl & bit_data->sda_get_mask) == bit_data->sda_get_mask);
}

static int i2c_niagara_getscl(void *data)
{
	const struct i2c_niagara_bit_data *bit_data = &i2c_bit_data;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg);

	return ((ctl & bit_data->scl_get_mask) == bit_data->scl_get_mask);
}

static int i2c_bit_niagara_init(struct i2c_adapter *adap)
{
	struct i2c_algo_bit_data *algo_data =
	    (struct i2c_algo_bit_data *)adap->algo_data;

	algo_data->getscl = i2c_niagara_getscl;
	algo_data->getsda = i2c_niagara_getsda;
	algo_data->setscl = i2c_niagara_setscl;
	algo_data->setsda = i2c_niagara_setsda;
	algo_data->udelay = 10;
	algo_data->data = NULL;

	return 0;
}

int i2c_bit_niagara_add_bus(struct i2c_adapter *adap, int new)
{
	int rval = i2c_bit_niagara_init(adap);

	if (rval)
		return rval;

	return i2c_bit_add_bus(adap);
}

static const struct i2c_niagara_bit_data tpm_i2c = {
	.ctl_reg = CPLD_ADDR_SD_CTL,
	.scl_get_mask = CPLD_I2C_CTL_SCL_IN,
	.scl_set_mask = CPLD_I2C_CTL_SCL_OUT,
	.sda_get_mask = CPLD_I2C_CTL_SDA_IN,
	.sda_set_mask = CPLD_I2C_CTL_SDA_OUT
};

static void i2c_tpm_setsda(void *data, int state)
{
	const struct i2c_niagara_bit_data *bit_data = &tpm_i2c;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg) & ~bit_data->sda_set_mask;

	if (state)
		ctl |= bit_data->sda_set_mask;
	niagara_cpld_write(bit_data->ctl_reg, ctl);
}

static void i2c_tpm_setscl(void *data, int state)
{
	const struct i2c_niagara_bit_data *bit_data = &tpm_i2c;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg) & ~bit_data->scl_set_mask;

	if (state)
		ctl |= bit_data->scl_set_mask;
	niagara_cpld_write(bit_data->ctl_reg, ctl);
}

static int i2c_tpm_getsda(void *data)
{
	const struct i2c_niagara_bit_data *bit_data = &tpm_i2c;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg);

	return ((ctl & bit_data->sda_get_mask) == bit_data->sda_get_mask);
}

static int i2c_tpm_getscl(void *data)
{
	const struct i2c_niagara_bit_data *bit_data = &tpm_i2c;
	uint8_t ctl = niagara_cpld_read(bit_data->ctl_reg);

	return ((ctl & bit_data->scl_get_mask) == bit_data->scl_get_mask);
}

static int i2c_bit_tpm_init(struct i2c_adapter *adap)
{
	struct i2c_algo_bit_data *algo_data =
	    (struct i2c_algo_bit_data *)adap->algo_data;

	algo_data->getscl = i2c_tpm_getscl;
	algo_data->getsda = i2c_tpm_getsda;
	algo_data->setscl = i2c_tpm_setscl;
	algo_data->setsda = i2c_tpm_setsda;
	algo_data->udelay = 10;
	algo_data->data = NULL;

	return 0;
}

int i2c_bit_niagara_add_tpm_bus(struct i2c_adapter *adap)
{
	int rval = i2c_bit_tpm_init(adap);

	if (rval)
		return rval;

	return i2c_bit_add_bus(adap);
}
