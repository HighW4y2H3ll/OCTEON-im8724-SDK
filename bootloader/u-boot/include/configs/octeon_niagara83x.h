/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2015 Cavium, Inc. <support@cavium.com>
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
 * Octeon ROUTER7000-REF board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_OCTEON_NIAGARA83X_MAJOR		1
#define CONFIG_OCTEON_NIAGARA83X_MINOR		0

/** Needed to build the CPLD driver under drivers/misc */
#define CONFIG_NIAGARA

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		1
#endif

#define CONFIG_OCTEON_USB_OCTEON3	/** Enable USB support on OCTEON III */
#define CONFIG_SYS_NO_FLASH		1

/** The CN70XX has two TWSI buses and the Niagara 83X board uses both */
#define CONFIG_SYS_MAX_I2C_BUS		1

#include "octeon_common.h"
/* "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM
/*#define CONFIG_EEPROM_TLV_BASE_ADDRESS	0x8000*/

#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN 2
#define CONFIG_CMD_EEPROM

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NIAGARA83X_BOARD_EEPROM_TWSI_ADDR
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	7	/* 128 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_SYS_LPC_IO_BASE			0x10020000

#define CONFIG_OCTEON_LPC_UART

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_REG_SIZE		1
#define UART0_BASE				0x3f8
#define CONFIG_SYS_NS16550_COM1		(CONFIG_SYS_LPC_IO_BASE + UART0_BASE)

#define CONFIG_OCTEON_NIAGARA83X_W83627

#define CONFIG_SYS_NS16550_CLK	14769000

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define OCTEON_NIAGARA83X_DEF_DRAM_FREQ	666

/* eMMC support */
#define CONFIG_OCTEON_MMC		/* Enable MMC support */
#define CONFIG_MMC_MBLOCK		/* Multi-block support */
#define CONFIG_CMD_MMC			/* Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/* Enable multiple MMC devices */
#define CONFIG_MMC
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000

#define CONFIG_OCTEON_MMC_MAX_FREQUENCY	26000000

#define CONFIG_SPI			/** Enable SPI support		*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_STMICRO	/** Enable ST Micro SPI flash	*/

#define CONFIG_SF_DEFAULT_BUS		0	/** Default SPI bus	*/
#define CONFIG_SF_DEFAULT_CS		0	/** Default SPI chip select */
#define CONFIG_SF_DEFAULT_SPEED		16000000 /** Default SPI speed	*/
#define CONFIG_OCTEON_SPI_BOOT_START	0x10000
#define CONFIG_OCTEON_SPI_BOOT_END	0x400000

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_SPI_M95XXX		/** M95256 SPI NOR		*/
#define CONFIG_SPI_M95XXX_CS	0	/** Chip select offset		*/

#define CONFIG_CMD_SPI_EEPROM		/** spi-eeprom command		*/
#define CONFIG_SYS_SPI_EEPROM_PAGE_WRITE_BITS		6
#define CONFIG_SYS_SPI_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/** Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_RGMII_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
    defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_VITESSE
# include "octeon_network.h"
#endif

/** Sensors used, 0 = local, 1 = remote */
#define CONFIG_DTT_SENSORS		{ 0, 1 }
#define CONFIG_CMD_DTT			/** Add dtt temperature command	*/
/** Enable support for ADM1021 temperature sensor */
#define CONFIG_DTT_ADM1021
#define CONFIG_SYS_I2C_DTT_ADDR		0x4c	/** temp sensor address	*/
#define CONFIG_SYS_DTT_BUS_NUM		0	/** temp sensor bus	*/

/** Parameters for temperature sensor */
#define CONFIG_SYS_DTT_ADM1021						\
{									\
	{								\
		.i2c_addr	= CONFIG_SYS_I2C_DTT_ADDR,		\
		.conv_rate	= 2,	/** default */			\
		.enable_alert	= 1,					\
		.enable_local	= 1,					\
		.max_local	= 80,					\
		.min_local	= 4,					\
		.enable_remote	= 1,					\
		.max_remote	= 100,					\
		.min_remote	= 4					\
	},								\
	{								\
		.i2c_addr	= CONFIG_SYS_I2C_DTT_ADDR,		\
		.conv_rate	= 2,	/** default */			\
		.enable_alert	= 1,					\
		.enable_local	= 1,					\
		.max_local	= 80,					\
		.min_local	= 4,					\
		.enable_remote	= 1,					\
		.max_remote	= 100,					\
		.min_remote	= 4					\
	},								\
}

#define CONFIG_CMD_DATE			/** Date RTC command		*/
#define CONFIG_RTC_MCP7940N		/** Compatible with MCP7940N	*/
#define CONFIG_SYS_I2C_RTC_ADDR	0x6f	/** RTC I2C address		*/
#define CONFIG_SYS_RTC_BUS_NUM	0	/** RTC I2C bus number		*/

#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/

#define CONFIG_CMD_SAVEENV
#define CONFIG_SYS_MMC_ENV_DEV		0	/* MMC device for env */
/** Store the environment just in front of the first partition */
#define CONFIG_ENV_OFFSET		0x0fc000
#define CONFIG_ENV_IS_IN_MMC		1
#undef CONFIG_ENV_IS_IN_FLASH

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"autoload=n\0"							\
	""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(8*1024)
#define CONFIG_ENV_ADDR		CONFIG_ENV_OFFSET


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)


/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Configure QLM */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_niagara83x_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
