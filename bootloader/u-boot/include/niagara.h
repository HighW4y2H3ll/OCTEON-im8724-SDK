/*
 * (C) Copyright 2015 Cavium, Inc. <support@cavium.com>
 *
 * (C) Copyright 2015 Interface Masters
 * Marcel Birthelmer <marcelb@interfacemasters.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _NIAGARA_H_
#define _NIAGARA_H_

/* COMMON REGISTER ADDRESSES AND BITS */

/* CPLD Common Addresses */

#define	CPLD_ADDR_CPLD_SIG_L		0x00
#define	CPLD_ADDR_CPLD_SIG_H		0x01
#define CPLD_ADDR_CPLD_REV		0x02
/*
 * Remote access interface
 */
#define CPLD_ADDR_BA_CTL		0x1c

#define CPLD_BA_CTL_B_MD_REG		0x00
#define CPLD_BA_CTL_B_MD_SPI		0x20
#define CPLD_BA_CTL_B_MD_I2C		0x40
#define CPLD_BA_CTL_B_MD_MI		0x60

#define CPLD_BA_CTL_EB_EN		0x80

/*
 * SPI interface
 */

#define CPLD_ADDR_MI_BSL		0x1e
#define CPLD_ADDR_I2C_BSL		0x1f

#define CPLD_ADDR_SPI_CMD		0x28
#define CPLD_ADDR_SPI_CFG		0x29
#define CPLD_ADDR_SPI_DATA_0		0x2a
#define CPLD_ADDR_SPI_DATA_1		0x2b
#define CPLD_ADDR_SPI_ADDR_0		0x2c
#define CPLD_ADDR_SPI_ADDR_1		0x2d
#define CPLD_ADDR_SPI_ADDR_2		0x2e
#define CPLD_ADDR_SPI_ADDR_3		0x2f

#define CPLD_SPI_CMD_ST			0x01
#define CPLD_SPI_CMD_ADDR_W_1		0x00
#define CPLD_SPI_CMD_ADDR_W_2		0x02
#define CPLD_SPI_CMD_ADDR_W_3		0x04
#define CPLD_SPI_CMD_ADDR_W_4		0x06
#define CPLD_SPI_CMD_OP_W8		0x00
#define CPLD_SPI_CMD_OP_W16		0x10
#define CPLD_SPI_CMD_OP_WS		0x20
#define CPLD_SPI_CMD_OP_WA		0x30
#define CPLD_SPI_CMD_OP_R8		0x40
#define CPLD_SPI_CMD_OP_R16		0x50
#define CPLD_SPI_CMD_OP_RS		0x60
#define CPLD_SPI_CMD_OP_RAM		0xf0

/*
 * MDIO/MDC interface.
 */
#define CPLD_ADDR_MI_CSR		0x30
#define CPLD_ADDR_MI_MCTL		0x31
#define CPLD_ADDR_MI_DATA_0		0x32
#define CPLD_ADDR_MI_DATA_1		0x33
#define CPLD_ADDR_MI_DEVAD		0x34
#define CPLD_ADDR_MI_PRTAD		0x35


#define CPLD_MDIO_CSR_ST		0x01

#define CPLD_MDIO_MCTL_C45		0x00
#define CPLD_MDIO_MCTL_C22		0x80

#define CPLD_MDIO_MCTL_SCLK_MAN		0x00
#define CPLD_MDIO_MCTL_SCLK_32		0x10
#define CPLD_MDIO_MCTL_SCLK_16		0x20
#define CPLD_MDIO_MCTL_SCLK_8		0x30

/*
 * I2C interface.
 */
#define CPLD_ADDR_I2C_CTL		0x3e
#define CPLD_ADDR_I2C_CMD		0x38
#define CPLD_ADDR_I2C_STAT		0x39
#define CPLD_ADDR_I2C_MCTL		0x3a
#define CPLD_ADDR_I2C_DEV_ID0		0x3b
#define CPLD_ADDR_I2C_DEV_ID1		0x3c
#define CPLD_ADDR_I2C_DB1		0x3d
#define CPLD_ADDR_I2C_DB2		0x3e
#define CPLD_ADDR_I2C_DB3		0x3f

#define CPLD_I2C_CTL_RATE_1000		0x00
#define CPLD_I2C_CTL_RATE_500		0x40
#define CPLD_I2C_CTL_RATE_100		0x80
#define CPLD_I2C_CTL_RATE_50		0xc0

#define CPLD_I2C_CTL_SDA_SCL_HIGH	0x0C

#define CPLD_I2C_CTL_SDA_OUT		0x08
#define CPLD_I2C_CTL_SCL_OUT		0x04
#define CPLD_I2C_CTL_SDA_IN		0x02
#define CPLD_I2C_CTL_SCL_IN		0x01

#define CPLD_I2C_STAT_ERR_MASK		0x3C
#define CPLD_I2C_STAT_BUSY		0x01
#define CPLD_I2C_STAT_TOUT		0x10
#define CPLD_I2C_STAT_NO_ACK		0x08
#define CPLD_I2C_STAT_CS_INV		0x04
#define CPLD_I2C_STAT_I2C_FLT		0x20
#define CPLD_I2C_STAT_RESET		0x01


void cpld_init(uint64_t base);
uint8_t cpld_rd(uint8_t offset);
void cpld_wr(uint8_t offset, uint8_t val);

#ifdef CONFIG_SYS_NIAGARA_REMOTE

uint8_t cpld_rd_remote(uint8_t target, uint8_t offset);
void cpld_wr_remote(uint8_t target, uint8_t offset, uint8_t val);

#endif

#if defined CONFIG_NIAGARA_I2C && defined CONFIG_NIAGARA_I2C_PREFIX

#define NIAGARA_I2C(x) CONFIG_NIAGARA_I2C_PREFIX##_##x

void NIAGARA_I2C(i2c_init) (int speed, int slaveaddr);
int NIAGARA_I2C(i2c_probe)(uchar addr);
int  NIAGARA_I2C(i2c_read)(uchar chip, uint addr, int alen, uchar *buffer, int len);
int  NIAGARA_I2C(i2c_write)(uchar chip, uint addr, int alen, uchar *buffer, int len);
int NIAGARA_I2C(i2c_set_bus_num)(unsigned int bus);
unsigned int NIAGARA_I2C(i2c_get_bus_num)(void);

#endif

#endif
