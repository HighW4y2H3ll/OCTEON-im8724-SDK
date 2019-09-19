#ifndef _NIAGARA_I2C_NATIVE_H_
#define _NIAGARA_I2C_NATIVE_H_

#include <linux/i2c.h>

#define SEQ_0_NOP               0
#define SEQ_0_TX_DEV1           1
#define SEQ_0_TX_DEV0_INV       2
#define SEQ_0_RES               3

#define SEQ_1_NOP               (0 << 2)
#define SEQ_1_TX_DB1            (1 << 2)
#define SEQ_1_TX_DB1_RS_TX_DEV0 (2 << 2)
#define SEQ_1_RX_DB1_ACK        (3 << 2)

#define SEQ_2_NOP               (0 << 4)
#define SEQ_2_TX_DB2            (1 << 4)
#define SEQ_2_TX_DB2_RS_TX_DEV0 (2 << 4)
#define SEQ_2_RX_DB2_ACK        (3 << 4)

#define SEQ_3_NOP               (0 << 6)
#define SEQ_3_TX_DB3_STOP       (1 << 6)
#define SEQ_3_STOP_RST          (2 << 6)
#define SEQ_3_RX_DB3_NACK_STOP  (3 << 6)

#define SEQ_BUS_RESET (SEQ_0_NOP | SEQ_1_NOP | SEQ_2_NOP | SEQ_3_STOP_RST)

extern int i2c_ext_algo;

void	niagara_i2c_bus_init(void);
int	niagara_i2c_add_bus(struct i2c_adapter *);

#endif
