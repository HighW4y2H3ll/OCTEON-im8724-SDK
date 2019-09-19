/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2014 Cavium, Inc. <support@cavium.com>
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
 * Octeon NIC4E board.
 */

#ifndef __OCTEON_SWORDFISH_H__
#define __OCTEON_SWORDFISH_H__

#define CONFIG_OCTEON_USB_OCTEON2	/* Enable USB support on OCTEON II */

#include "octeon_common.h"

/* Change flash base from default since it is small enough to not be remapped */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define SWORDFISH_DEF_DRAM_FREQ		533

/**
 * The swordfish board automatically enables USB storage so remove the
 * usb start message.
 */
#define CONFIG_OCTEON_HIDE_USB_START_MSG

/* Addresses for various things on boot bus.  These addresses
 * are used to configure the boot bus mappings.
 */

/* Remap the Octeon TWSI Slave Address. Default is 0x77. */
/* #define TWSI_BUS_SLAVE_ADDRESS 0x65 */

/** Enable SE application native API support */
#define CONFIG_OCTEON_SE_NAPI

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define CONFIG_ISPPAC_POWER
#define BOARD_ISP_TWSI_ADDR		0x08

/* The Swordfish board does not have a MCU */
#define BOARD_MCU_AVAILABLE		0

#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_SWORDFISH_BOARD_EEPROM_TWSI_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_I2C_MAX_HOPS		2
#define CONFIG_SYS_NUM_I2C_BUSES	6
#undef CONFIG_SYS_I2C_DIRECT_BUS

/** An I2C mux is present */
#define CONFIG_I2C_MUX

/** Enable I2C FDT support, needed for PMIC */
#define CONFIG_OCTEON_I2C_FDT

/** Configure for the I2C MUX to handle the SFP ports */
#define CONFIG_SYS_I2C_BUSES			\
{						\
	{0, {I2C_NULL_HOP} },			\
	{1, {I2C_NULL_HOP} },			\
	{0, {{I2C_MUX_PCA9548, 0x74, 0} } },	\
	{0, {{I2C_MUX_PCA9548, 0x74, 1} } },	\
	{0, {{I2C_MUX_PCA9548, 0x74, 2} } },	\
	{0, {{I2C_MUX_PCA9548, 0x74, 3} } }	\
}

/** Enable support for PMBus power management chips */
#define CONFIG_PMBUS
/** Enable support for the MP2953A chip */
#define CONFIG_PMBUS_MP2953

/** Power management chips are configured via I2C */
#define CONFIG_POWER_I2C

#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** No failsafe for NIC cards */
#define CONFIG_OCTEON_NO_FAILSAFE

/* Change the NOR MTD partitioning here.  The JFFS2 filesystem should be
 * kept as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabel for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:1536k(boot-loader)ro,"	\
	"6528k(storage),"				\
	"128k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_XAUI_ENET
#define CONFIG_OCTEON_RXAUI_ENET
#define CONFIG_OCTEON_SGMII_ENET
/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_RXAUI_ENET) || \
    defined(CONFIG_OCTEON_SGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

/** Enable SFP support */
#define CONFIG_PHY_SFP
#define CONFIG_OCTEON_SFP

/** Enable generic SFP command */
#define CONFIG_CMD_SFP

#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define the available PHYs
 */
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_VITESSE
# define CONFIG_OCTEON_VSC8490
# include "octeon_network.h"
#endif

#define CONFIG_CMD_OCTEON_TLVEEPROM
#define CONFIG_CMD_QLM

/* Console information is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3/4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FS_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/
#define CONFIG_FS_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_EXT4_WRITE		/* EXT4 write support		*/

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
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(135)	/* max number of sectors on one chip */
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
#define CONFIG_ENV_SIZE		(8*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

/**
 * DTT sensor
 * The Swordfish has a SAA56004X
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
#include "octeon_swordfish_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __OCTEON_SWORDFISH_H__ */
