#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/platform_data/niagara.h>

#include "niagara-i2c-algo.h"
#include "niagara_cpld.h"

static u32 niagara_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static int niagara_cpld_i2c_wait(struct i2c_adapter *adap)
{
	unsigned long end = get_jiffies_64() + adap->timeout;
	uint8_t stat;

	while (time_is_after_jiffies(end)) {
		stat = niagara_cpld_read(CPLD_ADDR_I2C_STAT);
		if (stat & CPLD_I2C_STAT_ERR_MASK) {
			dev_dbg(&adap->dev, "error stat:0x%02x\n", stat);
			return -EAGAIN;
		}
		if (!(stat & CPLD_I2C_STAT_BUSY))
			break;
		udelay(100);
	}

	if (time_is_before_jiffies(end))
		return -ETIMEDOUT;

	return 0;
}

static int niagara_i2c_read(struct i2c_adapter *adap, struct i2c_msg *msg)
{
	int err, i = 0;
	u8 dev_id1 = (msg->addr << 1) | 1;
	u8 seq;
	int len = msg->len;

	niagara_cpld_write(CPLD_ADDR_I2C_DEV_ID1, dev_id1);

	seq = SEQ_0_TX_DEV1;
	if (len == 0) {
		seq |= SEQ_3_STOP_RST;

		niagara_cpld_write(CPLD_ADDR_I2C_CMD, seq);

		err = niagara_cpld_i2c_wait(adap);
		if (err)
			return err;
	}

	while (len) {
		switch (len) {
		case 3:
			seq |= SEQ_1_RX_DB1_ACK;
		case 2:
			seq |= SEQ_2_RX_DB2_ACK;
		case 1:
			seq |= SEQ_3_RX_DB3_NACK_STOP;
			break;
		default:
			seq |= SEQ_1_RX_DB1_ACK | SEQ_2_RX_DB2_ACK;
		}
		niagara_cpld_write(CPLD_ADDR_I2C_CMD, seq);

		err = niagara_cpld_i2c_wait(adap);
		if (err)
			return err;

		seq = 0;

		switch (len) {
		case 3:
			msg->buf[i++] = niagara_cpld_read(CPLD_ADDR_I2C_DB1);
		case 2:
			msg->buf[i++] = niagara_cpld_read(CPLD_ADDR_I2C_DB2);
		case 1:
			msg->buf[i++] = niagara_cpld_read(CPLD_ADDR_I2C_DB3);
			len = 0;
			break;
		default:
			msg->buf[i++] = niagara_cpld_read(CPLD_ADDR_I2C_DB1);
			msg->buf[i++] = niagara_cpld_read(CPLD_ADDR_I2C_DB2);
			len -= 2;
		}
	}

	return 0;
}

static int niagara_i2c_write(struct i2c_adapter *adap, struct i2c_msg *msg)
{
	int err, i = 0;
	u8 dev_id1 = msg->addr << 1;
	u8 seq;
	int len = msg->len;

	niagara_cpld_write(CPLD_ADDR_I2C_DEV_ID1, dev_id1);
	if (len == 0) {
		seq = SEQ_0_TX_DEV1 | SEQ_3_STOP_RST;
		niagara_cpld_write(CPLD_ADDR_I2C_CMD, seq);

		err = niagara_cpld_i2c_wait(adap);
		if (err)
			return err;
	}

	/*
	 * One could use SEQ0_TX_DEV0_INV for a slight efficiency
	 * boost.
	 */
	seq = SEQ_0_TX_DEV1;

	while (len) {
		switch (len) {
		case 3:
			seq |= SEQ_1_TX_DB1;
			niagara_cpld_write(CPLD_ADDR_I2C_DB1, msg->buf[i++]);
		case 2:
			seq |= SEQ_2_TX_DB2;
			niagara_cpld_write(CPLD_ADDR_I2C_DB2, msg->buf[i++]);
		case 1:
			seq |= SEQ_3_TX_DB3_STOP;
			niagara_cpld_write(CPLD_ADDR_I2C_DB3, msg->buf[i++]);
			len = 0;
			break;
		default:
			seq |= SEQ_1_TX_DB1 | SEQ_2_TX_DB2 | SEQ_3_NOP;
			niagara_cpld_write(CPLD_ADDR_I2C_DB1, msg->buf[i++]);
			niagara_cpld_write(CPLD_ADDR_I2C_DB2, msg->buf[i++]);
			len -= 2;
			break;
		}
		niagara_cpld_write(CPLD_ADDR_I2C_CMD, seq);

		err = niagara_cpld_i2c_wait(adap);
		if (err)
			return err;
		seq = 0;
	}

	return 0;
}

static int niagara_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs,
			    int num)
{
	int i, err;

	for (i = 0; i < num; i++) {
		if (msgs[i].flags & I2C_M_RD) {
			err = niagara_i2c_read(adap, &msgs[i]);
			if (err)
				return err;
		} else {
			err = niagara_i2c_write(adap, &msgs[i]);
			if (err)
				return err;
		}
	}

	return num;
}

static const struct i2c_algorithm niagara_i2c_algo = {
	.master_xfer = niagara_i2c_xfer,
	.smbus_xfer = NULL,
	.functionality = niagara_i2c_func,
};

void niagara_i2c_bus_init_ext(void)
{
	niagara_cpld_write(CPLD_ADDR_I2C_STAT, CPLD_I2C_STAT_RESET);
	niagara_cpld_write(CPLD_ADDR_I2C_MCTL,
			   CPLD_I2C_CTL_RATE_500 | CPLD_I2C_CTL_SDA_SCL_HIGH);
	niagara_cpld_write(CPLD_ADDR_I2C_CMD, SEQ_BUS_RESET);
}

int niagara_i2c_add_bus_ext(struct i2c_adapter *adap)
{

	adap->algo = &niagara_i2c_algo;

	return i2c_add_adapter(adap);
}
