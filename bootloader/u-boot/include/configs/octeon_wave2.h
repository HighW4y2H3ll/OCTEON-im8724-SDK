/*
 * (C) Copyright 2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon Wave2 board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Build configuration for cn70xx
 */
#define CONFIG_OCTEON_CN70XX

#define CONFIG_OCTEON_WAVE2_MAJOR		1
#define CONFIG_OCTEON_WAVE2_MINOR		0
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
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1	/** Wave 2 only has 1 port */
#define CONFIG_SYS_NO_FLASH		1

/** The CN70XX has two TWSI buses and the Wave 2 board uses only bus 0 */
#define CONFIG_SYS_MAX_I2C_BUS		1

#include "octeon_common.h"

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define WAVE2_DEF_DRAM_FREQ	500

#define CONFIG_LBA48			/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/* 64-bit LBA support */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /* Make compile happy */
/* Base address of Common memory for Compact flash */

/* eMMC support */
#define CONFIG_OCTEON_MMC		/* Enable MMC support */
#define CONFIG_MMC_MBLOCK		/* Multi-block support */
#define CONFIG_CMD_MMC			/* Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/* Enable multiple MMC devices */
#define CONFIG_MMC
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_XAUI_ENET

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
# define CONFIG_PHY_AQUANTIA
# define CONFIG_LIBPHY_10G
# include "octeon_network.h"
#endif


/** "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM

/** Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_WAVE2_BOARD_EEPROM_TWSI_ADDR

/** Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/** Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE

#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/

/* SPI NOR flash support */
#define CONFIG_SF_DEFAULT_BUS		0	/** Default SPI bus	*/
#define CONFIG_SF_DEFAULT_CS		0	/** Default SPI chip select */
#define CONFIG_SF_DEFAULT_SPEED		25000000/** Default SPI speed	*/

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_STMICRO	/** Enable ST Micro SPI flash	*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_M95XXX		/** M95256 SPI NOR		*/
#define CONFIG_SPI_M95XXX_CS	0	/** Chip select offset		*/

#define CONFIG_OCTEON_SPI_BOOT_END	0x400000/** End offset for SPI bootloaders */

#define CONFIG_CMD_SAVEENV
#define CONFIG_SYS_MMC_ENV_DEV		0
/** Store the environment just in front of the first partition */
#define CONFIG_ENV_OFFSET		0x0c000
#define CONFIG_ENV_IS_IN_SPI_FLASH	1
#define CONFIG_ENV_SECT_SIZE		256
#undef CONFIG_ENV_IS_IN_FLASH

/*
 * Miscellaneous configurable options
 */
/** Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "autoload=n\0"							\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/** Size of Primary Environment Sector */
#define CONFIG_ENV_SIZE		(8*1024)

/** Address of Primary Environment Sector */
#define CONFIG_ENV_ADDR		CONFIG_ENV_OFFSET


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/** Enable support for real-time clock */
#define CONFIG_RTC_DS1337

/** Real-time clock I2C address */
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/** Real-time clock i2c bus */
#define CONFIG_SYS_RTC_BUS_NUM		0

/** TI TMP42X temperature sensor driver support */
#define CONFIG_DTT_TMP42X

/** Temperature sensor I2C bus number */
#define CONFIG_SYS_I2C_DTT_BUS		0

/** Temperature sensor address */
#define CONFIG_SYS_I2C_DTT_ADDR		0x4c

/** Enable "dtt" command */
#define CONFIG_CMD_DTT

/* Default settings for TMP42X driver (see above) */
#define CONFIG_SYS_DTT_TMP42X	{ 0, 1, 0, 0, 1, 1, 2, 0, 0, 0}

/* Sensors used */
#define CONFIG_DTT_SENSORS	{ 0, 1 }

/** Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/** Configure QLM */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_wave2_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
