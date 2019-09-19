#include <linux/i2c.h>
#include <linux/i2c-mux.h>
#include <linux/platform_data/niagara.h>

#include "niagara_cpld.h"

int niagara_i2c_mux_select(struct i2c_mux_core *muxc, u32 chan)
{
	int cpld_bus = (chan) & 0xff;
	int cpld_sasl = (chan >> 8) & 0x7f;
	int isremote = (chan >> 15) & 0x1;

	niagara_cpld_lock();
	if (isremote) {
		/* set up I2C_BSL at the destination CPLD */
		niagara_cpld_remote_write(cpld_sasl, CPLD_ADDR_I2C_BSL, cpld_bus);

		/* configure I2C module routing to external I2C */
		niagara_cpld_write(CPLD_ADDR_BA_CTL,
				   CPLD_BA_CTL_EB_EN | CPLD_BA_CTL_B_MD_I2C | cpld_sasl);
	} else {
		/* set up I2C_BSL at the local CPLD */
		niagara_cpld_write(CPLD_ADDR_I2C_BSL, cpld_bus);

		/* configure I2C module routing to local I2C */
		niagara_cpld_write(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_B_MD_I2C);
	}

	return 0;
}

int niagara_i2c_mux_deselect(struct i2c_mux_core *muxc, u32 chan)
{
	niagara_cpld_write(CPLD_ADDR_I2C_BSL, 0);
	niagara_cpld_unlock();
	return 0;
}
