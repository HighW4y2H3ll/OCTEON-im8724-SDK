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
 * Octeon EAP7000-REF board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Build configuration for cn70xx
 */
#define CONFIG_OCTEON_CN70XX

#define CONFIG_OCTEON_EAP7000_REF_MAJOR		1
#define CONFIG_OCTEON_EAP7000_REF_MINOR		1
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

/** The CN70XX has two TWSI buses and the EAP7000 Ref board uses only bus 0 */
#define CONFIG_SYS_MAX_I2C_BUS		2
#define CONFIG_MAX_I2C_NUM		1

#include "octeon_common.h"
#undef CONFIG_OCTEON_EEPROM_TUPLES
#undef CONFIG_SYS_I2C_EEPROM_ADDR_LEN

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define EAP7000_REF_DEF_DRAM_FREQ	500

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

#undef	CONFIG_BOOTARGS

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#undef CONFIG_CMD_EEPROM

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_ATHEROS
# include "octeon_network.h"
#endif


/* "tlv_eeprom" command */
#undef CONFIG_CMD_OCTEON_TLVEEPROM

/* Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/

/* SPI NOR flash support */
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		25000000

#define CONFIG_OCTEON_SPI		/* Enable OCTEON SPI driver	*/
#define CONFIG_SPI_FLASH		/* Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_MACRONIX	/* Enable MXC SPI flash	*/
#define CONFIG_CMD_SPI			/* Enable SPI command		*/
#define CONFIG_CMD_SF			/* Enable SPI flash command	*/
#define CONFIG_OCTEON_SPI_NO_FAILSAFE	/** No failsafe for SPI booting	*/

/** End of SPI boot for SPI booting */
#define CONFIG_OCTEON_SPI_BOOT_END	0x200000
#define CONFIG_CMD_SAVEENV
#define CONFIG_SYS_MMC_ENV_DEV		0
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

#define CONFIG_RTC_DS1337
#define CONFIG_SYS_I2C_RTC_ADDR		0x68
#define CONFIG_SYS_RTC_BUS_NUM		0

#define CONFIG_DTT_TMP42X
#define CONFIG_SYS_I2C_DTT_BUS		0
#define CONFIG_SYS_I2C_DTT_ADDR		0x4c
#define CONFIG_DTT_SENSORS		{ 0, 1 }
#define CONFIG_CMD_DTT

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Configure QLM */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_eap7000_ref_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
