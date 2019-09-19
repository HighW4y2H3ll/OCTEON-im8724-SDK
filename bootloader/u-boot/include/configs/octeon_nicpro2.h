/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2012 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * This file contains the configuration parameters for
 * Octeon NIC Pro 2 board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define NICPRO2_REV1_DEF_DRAM_FREQ		266

#define CONFIG_SYS_NO_FLASH			  1 /* NIC Pro 2 has no flash */

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST	0
#endif

#include "octeon_common.h"

#undef CONFIG_OCTEON_FLASH

#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NICPRO2_EEPROM_ADDR

#define CONFIG_I2C_MUX
#define CONFIG_SYS_NUM_I2C_BUSES	5
#define CONFIG_SYS_I2C_MAX_HOPS		2

#define CONFIG_SYS_I2C_BUSES			\
{						\
	{0, {I2C_NULL_HOP} },			\
	{0, {{I2C_MUX_PCA9546, 0x76, 0} } },	\
	{0, {{I2C_MUX_PCA9546, 0x76, 1} } },	\
	{0, {{I2C_MUX_PCA9546, 0x76, 2} } },	\
	{0, {{I2C_MUX_PCA9546, 0x76, 3} } }	\
}

#undef	CONFIG_BOOTARGS

/* Enable support for the PCI console and boot command */
#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_PCI_CONSOLE
#define CONFIG_OCTEON_BOOTCMD

/*
 * The NIC Pro 2 does not use the internal arbiter in Octeon.
 * Enable this for boards that do.
 */
/* #define USE_OCTEON_INTERNAL_ARBITER */

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_RGMII_ENET
#define CONFIG_OCTEON_SPI_IXF18201_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SPI4000_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MARVELL
# include "octeon_network.h"
#endif

#define CONFIG_CMD_SDRAM

#define CONFIG_NET_RETRY_COUNT		3

#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_OCTEON_TLVEEPROM

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS			\
        "autoload=n\0"					\
        "serial_console_active=1\0"			\
        "disable_spi_ixf18201=1\0"			\
        ""

/* Since flash is optional assume the environment is in RAM */
#define CONFIG_ENV_IS_IN_RAM		1

/* NOTE: These must match cvmx-bootloader.h */
/* Defines for RAM based environment set by the host or the previous bootloader
 * in a chain boot configuration.
 */
#define U_BOOT_RAM_ENV_ADDR		(0x1000)
#define U_BOOT_RAM_ENV_SIZE		(0x1000)
#define U_BOOT_RAM_ENV_CRC_SIZE		(0x4)
#define U_BOOT_RAM_ENV_ADDR_2	(U_BOOT_RAM_ENV_ADDR + U_BOOT_RAM_ENV_SIZE)

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE			U_BOOT_RAM_ENV_SIZE
#define CONFIG_ENV_ADDR			U_BOOT_RAM_ENV_ADDR

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(16 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(32 * 1024)

#define CONFIG_HW_WATCHDOG
#define WDT_HW_TIMEOUT			10


/* DTT sensor
 * The NIC Pro 2 board has an LM83 temperature sensor.
 *
 * - enable D1 critical temp interrupt
 * - enable D2 critical temp interrupt
 * - enable D3 critical temp interrupt
 * - enable local critical temp interrupt
 * - critical temperature
 * - interrupt active low
 * - local highpoint alarm temp
 * - D1 highpoint alarm temp
 * - D2 highpoint alarm temp
 * - D3 highpoint alarm temp
 */
#define CONFIG_SYS_I2C_DTT_ADDR	0x2A
#define CONFIG_DTT_LM83
#define CONFIG_SYS_DTT_LM83	{	.enable_d1_crit = 1,	\
					.enable_d2_crit = 0,	\
					.enable_d3_crit = 0,	\
					.enable_local_crit = 1,	\
					.tcrit_temp = 100,	\
					.invert_int = 0,	\
					.local_highpt = 60,	\
					.d1_highpt = 85,	\
					.d2_highpt = 127,	\
					.d3_highpt = 127	\
				}
#define CONFIG_DTT_SENSORS	{ 0, 1 }
#define CONFIG_CMD_DTT

#include "octeon_nicpro2_shared.h"

/** Disable DDR3 support */
#define CONFIG_OCTEON_DISABLE_DDR3

#endif	/* __CONFIG_H__ */
