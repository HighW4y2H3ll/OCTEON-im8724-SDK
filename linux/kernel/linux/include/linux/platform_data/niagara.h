/*
 * Copyright (c) 2013 vIPtela, Inc.
 *
 * Author:
 *	Ruslan Babayev <ruslan@viptela.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __NIAGARA_H_
#define __NIAGARA_H_

#include <linux/i2c.h>

uint8_t	niagara_cpld_read(uint16_t);
void	niagara_cpld_write(uint16_t, uint8_t);
void	niagara_cpld_lock(void);
void	niagara_cpld_unlock(void);
void	niagara_cpld_remote_write(uint8_t, uint16_t, uint8_t);
uint8_t	niagara_cpld_remote_read(uint8_t, uint16_t);

int niagara_fiber_sfp_rx_los_get(int interface, int port_index);

/*
 * Remote access interface
 */
#define CPLD_ADDR_BA_CTL		0x1c

#define CPLD_BA_CTL_B_MD_REG		0x00
#define CPLD_BA_CTL_B_MD_SPI		0x20
#define CPLD_BA_CTL_B_MD_I2C		0x40
#define CPLD_BA_CTL_B_MD_MI		0x60
#define CPLD_BA_CTL_EB_EN		0x80

#define CPLD_ADDR_SPI_BSL		0x1d
#define CPLD_ADDR_I2C_BSL		0x1f

/*
 * SPI interface
 */

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
#define CPLD_SPI_CMD_ADDR_W_2		0x04
#define CPLD_SPI_CMD_ADDR_W_3		0x08
#define CPLD_SPI_CMD_ADDR_W_4		0x0C
#define CPLD_SPI_CMD_OP_W8		0x00
#define CPLD_SPI_CMD_OP_W16		0x10
#define CPLD_SPI_CMD_OP_WS		0x20
#define CPLD_SPI_CMD_OP_WA		0x30
#define CPLD_SPI_CMD_OP_R8		0x40
#define CPLD_SPI_CMD_OP_R16		0x50
#define CPLD_SPI_CMD_OP_RS		0x60
#define CPLD_SPI_CMD_OP_RAM		0xf0

#endif
