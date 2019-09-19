#ifndef IMCPLD_I2C_H_
#define IMCPLD_I2C_H_

int i2c_bit_niagara_add_bus(struct i2c_adapter *, int);
int i2c_bit_niagara_add_tpm_bus(struct i2c_adapter *);

int niagara_i2c_mux_select(struct i2c_mux_core *, u32);
int niagara_i2c_mux_deselect(struct i2c_mux_core *, u32);

#endif
