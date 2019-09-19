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
 * Octeon EBH5610 board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
** Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
** bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
** host system performs the pci bus mapping instead.  Note that pci
** commands are enabled to allow access to configuration space for
** both modes.
*/
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST	0
#endif

#define CONFIG_OCTEON_USB_OCTEON	/* Enable USB support on OCTEON I */

#include "octeon_common.h"

/* Uses same DRAM config as ebh5600 board */
#include "octeon_ebh5600_shared.h"

/** Disable DDR3 support */
#define CONFIG_OCTEON_DISABLE_DDR3

#define CONFIG_OCTEON		1

/* CONFIG_OCTEON_EBH5610 set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define EBH5610_DEF_DRAM_FREQ			200

#define CONFIG_OCTEON_ENABLE_PAL		/* Board has a PAL */

#define CONFIG_OCTEON_ENABLE_LED_DISPLAY	/* Board has an LED display */

#define CONFIG_SYS_IDE_MAXDEVICE		1
#define CONFIG_SYS_IDE_MAXBUS			1
#define OCTEON_CF_RESET_GPIO			5   /* Reset is GPIO 5 */
#define CONFIG_LBA48				/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA
#define CONFIG_IDE_RESET
#define CONFIG_SYS_ATA_BASE_ADDR		0 /* Make compile happy */

/* Remap the Octeon TWSI Slave Address. Default is 0x77.  This
 * works around the conflict with the ispPAC-POWR1220AT8 address
 * on this board.
 */
#define TWSI_BUS_SLAVE_ADDRESS 0x65

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define BOARD_ISP_TWSI_ADDR		0x77

#define BOARD_MCU_AVAILABLE		1
#define BOARD_MCU_TWSI_ADDR		0x60
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_EBH5600_BOARD_EEPROM_TWSI_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

#define CONFIG_RTC_DS1337
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/*
 * The EBH5610 does not use the internal arbiter in Octeon.
 * Enable this for boards that do.
 */
/* #define USE_OCTEON_INTERNAL_ARBITER */

/* Change the NOR MTD partitioning here.  The JFFS2 filesystem should be kept
 * as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabe for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:1m(bootloader),"		\
	"2560k(kernel),"				\
	"4600k(cramfs),"				\
	"8k(environment)\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_RGMII_ENET
#define CONFIG_OCTEON_MGMT_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET)
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
 * Define the available PHYs
 */
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_BROADCOM
# define CONFIG_PHY_MARVELL

# include "octeon_network.h"
#endif

#define CONFIG_CMD_OCTEON_TLVEEPROM
#define CONFIG_CMD_QLM

#define CONFIG_CMD_SDRAM

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_DATE
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/
#define CONFIG_CMD_IDE			/* IDE harddisk support		*/
/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

#ifdef CONFIG_CMD_EEPROM
# define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
# define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5
#endif

/* Offset from base at which data is repeated so that it can be
 * read as a block
 */
#define CONFIG_SYS_ATA_DATA_OFFSET		0x400

/* Not sure what this does, probably offset from base
 * of the command registers
 */
#define CONFIG_SYS_ATA_REG_OFFSET		0

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_cf=fatload ide 0 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "ls=fatls ide 0\0"				\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CONFIG_OCTEON_FLASH_PAGE_MODE		/** Speeds up flash timing early */

#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
#define CONFIG_CFI_FLASH_USE_WEAK_ACCESSORS

#define CONFIG_SYS_FLASH_CFI		1
#define CONFIG_FLASH_CFI_DRIVER		1

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(2 * CONFIG_SYS_HZ) /* Timeout for Flash Erase */
#define CONFIG_SYS_FLASH_WRITE_TOUT	(2 * CONFIG_SYS_HZ) /* Timeout for Flash Write */

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
#define CONFIG_SYS_DCACHE_SIZE		(16 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(32 * 1024)

#if CONFIG_OCTEON_PCI_HOST
# define CONFIG_E1000
# define CONFIG_CMD_SATA			/* Enable the sata command */
# define CONFIG_SYS_SATA_MAX_DEVICE	4	/* Support up to 4 devices */
/* Enable support for the SIL3124 family */
# define CONFIG_LIBATA
# define CONFIG_SATA_SIL
#endif

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* DTT sensor
 * The ebh5610 board has a TI TMP421 temperature sensor.
 *
 * - range -55C to +127C
 * - Remote channel 1 enabled
 * - Remote channel 2 disabled
 * - Remote channel 3 disabled
 * - Local channel enabled
 * - Resistance correction enabled
 * - conversion rate 0x02 = 0.25 conversions/second
 * - local temp sensor enabled, min set to 0 deg, max set to 70 deg
 * - remote temp sensor enabled, min set to 0 deg, max set to 70 deg
 * - n factor channel 1
 * - n factor channel 2
 * - n factor channel 3
 */
#define CONFIG_SYS_I2C_DTT_ADDR		0x4c
#define CONFIG_DTT_TMP42X
#define CONFIG_SYS_DTT_TMP42X		{ 0, 1, 0, 0, 1, 1, 2, 0, 0, 0}
#define CONFIG_DTT_SENSORS		{ 0, 1 }
#define CONFIG_CMD_DTT

#endif	/* __CONFIG_H__ */
