/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2012 Cavium, Inc.
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
 * Octeon nic2e board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "octeon_common.h"

/* Change flash base from default since it is small enough to not be remapped */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define NIC2E_DEF_DRAM_FREQ		533

/* Addresses for various things on boot bus.  These addresses
 * are used to configure the boot bus mappings.
 */

/* Remap the Octeon TWSI Slave Address. Default is 0x77. */
/* #define TWSI_BUS_SLAVE_ADDRESS 0x65 */

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define CONFIG_ISPPAC_POWER
#define BOARD_ISP_TWSI_ADDR		0x08

/* The nic2e board does not have a MCU */
#define BOARD_MCU_AVAILABLE		0

#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NIC2E_BOARD_EEPROM_TWSI_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/* Change the NAND/NOR MTD partitioning here.  The JFFS2 filesystem should be
 * kept as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabel for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:1m(boot-loader)ro,"	\
	"2944k(kernel),"				\
	"128k(environment)ro;"				\
	"octeon_nand0:4m(nandboot)ro,"			\
	"2m(boot-env)ro,"				\
	"8m(fs-boot),"					\
	"256m(ubifs-kernel),"				\
	"256m(ubifs-bigfs)\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0,nand0=octeon_nand0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) ||	\
	defined(CONFIG_OCTEON_XAUI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_BROADCOM
# include "octeon_network.h"
#endif

#include "config_octeon_nand.h"

#define CONFIG_CMD_OCTEON_TLVEEPROM
#define CONFIG_CMD_QLM

/* Console information is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (4*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */
#define CONFIG_SYS_CFI_FLASH_STATUS_POLL	/* Polling support */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
#define CONFIG_CFI_FLASH_USE_WEAK_ACCESSORS

#define CONFIG_SYS_FLASH_CFI		1
#define CONFIG_FLASH_CFI_DRIVER		1

#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(128*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

#define CONFIG_SYS_MAX_NAND_DEVICE	2
/* Not used for Octeon, but must be set */
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_SYS_NAND_MAX_CHIPS CONFIG_SYS_MAX_NAND_DEVICE

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

/* DTT sensor
 * The nic2e has a SAA56004X
 */
#define CONFIG_DTT_SAA56004X
#define CONFIG_CMD_DTT
#define CONFIG_DTT_SENSORS		{ 0, 1 }
#define CONFIG_SYS_I2C_DTT_ADDR		0x4A

#define CONFIG_SYS_DTT_SAA56004X \
	{ {	.i2c_addr = CONFIG_SYS_I2C_DTT_ADDR,	\
		.conv_rate = 8,				\
		.enable_alert = 1,			\
		.enable_local = 1,			\
		.max_local = 75,			\
		.min_local = 0,				\
		.enable_remote = 1,			\
		.max_remote = 85,			\
		.min_remote = 0,			\
		.crit_remote = 100,			\
		.crit_local = 80			\
	} }

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_nic2e_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
