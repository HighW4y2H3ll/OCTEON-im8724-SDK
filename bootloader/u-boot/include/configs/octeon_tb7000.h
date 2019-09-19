/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2013 Cavium, Inc. <support@cavium.com>
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
 * Octeon TB7000 board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Build configuration for cn70xx
 */
#define CONFIG_OCTEON_CN70XX

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST	1
#endif

#define CONFIG_OCTEON_USB_OCTEON3	/** Enable USB support on OCTEON III */

#include "octeon_common.h"

#undef CONFIG_OCTEON_I2C_LOW_LEVEL

#define CONFIG_OCTEON_FLASH_USES_ALE

#define CONFIG_CMD_MEMTEST

/* CONFIG_OCTEON_TB7000 set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define TB7000_DEF_DRAM_FREQ		800

#define CONFIG_OCTEON_ENABLE_LED_DISPLAY	/** Board has an LED display */

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

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define BOARD_ISP_TWSI_ADDR		0x08
#define CONFIG_ISPPAC_POWER

/* Address of MCU on TWSI bus */
#define BOARD_MCU_TWSI_ADDR		0x60

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_TB7000_BOARD_EEPROM_TWSI_ADDR
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/* The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:3m(bootloader)ro,"	\
	"2m(kernel),"					\
	"3064k(cramfs),"				\
	"8k(environment)ro;"				\
	"octeon_nand0:6m(bootloader)ro,"		\
	"2m(environment),"				\
	"8m(kernel),"					\
	"512m(rootfs)\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0,nand0=octeon_nand0\0"

/**
 * Start of where the NAND bootloader images are placed.  Must be on an erase
 * block boundary/
 */
#define CONFIG_OCTEON_NAND_BOOT_START		0

/**
 * End of the NAND boot partition which is searched for a bootloader.  Must be
 * on an erase block boundary.  It should be large enough to be able to handle
 * at least one bad block.
 */
#define CONFIG_OCTEON_NAND_BOOT_END		0x600000

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_QSGMII_ENET
#define CONFIG_OCTEON_XAUI_ENET
#define CONFIG_OCTEON_MGMT_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

/* PCI console is supported since oct-remote-boot is supported.
 * Enable MUX and oct-remote-bootcmd support as well
 */
#define CONFIG_SYS_PCI_CONSOLE
#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MICREL
# define CONFIG_PHY_VITESSE
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_MARVELL
# include "octeon_network.h"
#endif

/** "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM

/* Enable MTD partitioning support */
#define CONFIG_CMD_MTDPARTS

/* Enable JFFS2 support */
#define CONFIG_CMD_JFFS2

/* Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_FLASH		/** flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/
/*#define CONFIG_EXT4_WRITE*/		/** EXT4 write support		*/
#define CONFIG_CMD_EXT4_WRITE		/** Enable EXT4 write command	*/

/* SPI NOR flash support */
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		16000000

/**
 * Start of where the SPI bootloader images are placed.  Must be on an erase
 * block boundary.
 */
#define CONFIG_OCTEON_SPI_BOOT_START		0x10000

/**
 * End of the SPI boot partition which is searched for a bootloader.  Must be
 * on an erase block boundary.
 */
#define CONFIG_OCTEON_SPI_BOOT_END		0x400000

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_SPI			/** Enable SPI support		*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_STMICRO	/** Enable ST Micro SPI flash	*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_M95XXX		/** M95256 SPI NOR		*/
#define CONFIG_SPI_M95XXX_CS	0	/** Chip select offset		*/
#define CONFIG_CMD_SPI_EEPROM		/** spi-eeprom command		*/
#define CONFIG_SYS_SPI_EEPROM_PAGE_WRITE_BITS	6
#define CONFIG_SYS_SPI_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Maximum number of NAND devices */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_FORCE_WIDTH	8	/* Force to 8-bit width */
/* Not used for Octeon, but must be set */
#define CONFIG_SYS_NAND_BASE		0
/* Maximum number of NAND devices */
#define CONFIG_SYS_NAND_MAX_CHIPS	CONFIG_SYS_MAX_NAND_DEVICE

/* Enable multi-bit ECC support */
#define CONFIG_OCTEON_HW_BCH
#include "config_octeon_nand.h"

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_mmc=fatload mmc 0 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "autoload=n\0"					\
        "ls=fatls mmc 0\0"	\
        "m=md 0x1f400000\0"	\
        "md5=md5sum $(fileaddr) $(filesize)\0" \
        "ethprime=octrgmii0\0"

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CONFIG_OCTEON_FLASH_PAGE_MODE	/** Speeds up NOR flash */

/* Width of CFI bus to start scanning */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
/* Enable extra elements in CFI driver for storing flash geometry */
#define CONFIG_SYS_FLASH_CFI  		1
/* Enable CFI flash driver */
#define CONFIG_FLASH_CFI_DRIVER		1
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

/* We're not RAM_RESIDENT so CONFIG_ENV_IS_IN_FLASH will be set. */
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
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

#if CONFIG_OCTEON_PCI_HOST
/* Right now only the Intel E1000 driver has been ported */
# define CONFIG_E1000
# define CONFIG_LIBATA
# define CONFIG_CMD_SATA			/* Enable the sata command */
# define CONFIG_SYS_SATA_MAX_DEVICE	2	/* Support up to 2 devices */
/* Enable support for the SIL3124 family */
# define CONFIG_SATA_SIL
#endif

/* The CN70XX has two TWSI buses and the TB7000 board uses both of them */
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		1

/* Real-time Clock */
#define CONFIG_RTC_DS1337
/* Address of RTC on TWSI bus */
#define CONFIG_SYS_I2C_RTC_ADDR		0x68
#define CONFIG_SYS_RTC_BUS_NUM		0

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Configure QLM */
#define CONFIG_OCTEON_QLM

/* TI TMP42X temperature sensor driver support */
#define CONFIG_DTT_TMP42X
/* Default settings for TMP42X driver (see above) */
#define CONFIG_SYS_DTT_TMP42X	{ 0, 1, 0, 0, 1, 1, 2, 0, 0, 0}
/* Sensors used */
#define CONFIG_DTT_SENSORS	{ 0, 1 }
/* Enable "dtt" command */
#define CONFIG_CMD_DTT

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_tb7000_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
