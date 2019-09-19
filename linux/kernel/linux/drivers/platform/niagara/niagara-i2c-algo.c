#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/platform_data/niagara.h>

#define NIAGARA_SMBUS_XFER

#include "niagara-i2c-algo.h"
#include "niagara_cpld.h"

extern int niagara_i2c_add_bus_ext(struct i2c_adapter *adap);
extern void niagara_i2c_bus_init_ext(void);
int i2c_ext_algo = 1;
int enable_bitbang = 0;

static int niagara_smbus_xfer(struct i2c_adapter *adap, u16 addr, unsigned short flags, char read_write,
			u8 command, int size, union i2c_smbus_data *data)
{
	uint8_t result = 0;
	uint8_t op = 0;

	if ((size != I2C_SMBUS_QUICK) && (data == NULL))
		return -EINVAL;

	if ((flags & ~I2C_M_RD)) {
		pr_err("im i2c error: flags %08x", flags);
		return -EINVAL;
	}

	niagara_cpld_write(CPLD_ADDR_I2C_DA, (addr & 0x7f) << 1);
	niagara_cpld_write(CPLD_ADDR_I2C_CTL,
			CPLD_I2C_CTL_RATE_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
	switch (size) {
		case I2C_SMBUS_QUICK:
			op = CPLD_I2C_CSR_ACK_POLL;
			break;
	case I2C_SMBUS_BYTE_DATA:
			niagara_cpld_write(CPLD_ADDR_I2C_ADDR_0, command);
			op = ((read_write == I2C_SMBUS_WRITE) ? CPLD_I2C_CSR_WRITE : CPLD_I2C_CSR_READ);
			break;
	}

	if (size != I2C_SMBUS_QUICK && read_write == I2C_SMBUS_WRITE)
		niagara_cpld_write(CPLD_ADDR_I2C_DATA, data->byte);
	niagara_cpld_write(CPLD_ADDR_I2C_CSR, CPLD_I2C_CSR_ST | op);

	while (niagara_cpld_read(CPLD_ADDR_I2C_CSR) & CPLD_I2C_CSR_ST) ;

	result = niagara_cpld_read(CPLD_ADDR_I2C_STAT) & CPLD_I2C_STAT_MASK;

	if (result != 0) {
		if (size != I2C_SMBUS_QUICK)
			pr_debug("i2c stat error: %02x, addr %02x", result, addr);
		return -EIO;
	}

	if (size != I2C_SMBUS_QUICK && read_write == I2C_SMBUS_READ) {
		data->byte = niagara_cpld_read(CPLD_ADDR_I2C_DATA);
		pr_debug("i2c rd [%02x:%02x] == [%02x]", addr, command, data->byte);
	}
	return 0;
}

static u32 niagara_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_QUICK | I2C_FUNC_SMBUS_BYTE;
}

static const struct i2c_algorithm niagara_i2c_algo = {
        .master_xfer   = NULL,
	.smbus_xfer    = niagara_smbus_xfer,
        .functionality = niagara_i2c_func,
};

void niagara_i2c_bus_init(void)
{
	if (i2c_ext_algo) {
		niagara_i2c_bus_init_ext();
		return;
	}
	niagara_cpld_write(CPLD_ADDR_I2C_STAT, CPLD_I2C_STAT_ACT);
	niagara_cpld_write(CPLD_ADDR_I2C_CTL, CPLD_I2C_CTL_RATE_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
}

int niagara_i2c_add_bus(struct i2c_adapter *adap)
{
	if (i2c_ext_algo)
		return niagara_i2c_add_bus_ext(adap);
        adap->algo = &niagara_i2c_algo;
        return i2c_add_adapter(adap);
}
