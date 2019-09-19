/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon NICX40E board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		0
#endif

/** Enable the more powerful hush command parser */
#define CONFIG_SYS_HUSH_PARSER

#define CONFIG_OCTEON_BIG_STACK_SIZE		0x4000	/** 16K stack */
#define CONFIG_SYS_NO_FLASH		/** No NOR flash present */

/** Enable Octeon secure-mode by disabling bar1 on PCIE bus 0 */
#define CONFIG_OCTEON_SECURE_MODE_BUS	0

#include "octeon_common.h"

/* CONFIG_OCTEON_NICX40E set by Makefile in include/config.h */

/** Default DDR clock if tuple doesn't exist in EEPROM */
#define NICX40E_DEF_DRAM_FREQ		800

#define CONFIG_LBA48			/** 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/** 64-bit LBA support */

/* eMMC support */
#define CONFIG_OCTEON_MMC		/** Enable MMC support */
#define CONFIG_MMC_MBLOCK		/** Multi-block support */
#define CONFIG_CMD_MMC			/** Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/** Enable multiple MMC devices */
#define CONFIG_MMC			/** Enable MMC support */
#define CONFIG_SYS_MMC_MAX_DEVICE	1	/** Number of MMC devices */
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS


#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD

#include "octeon_cmd_conf.h"

/*
 * Define the available PHYs
 */
#define CONFIG_PHY_VITESSE		/** Enable Vitesse support */
#define CONFIG_OCTEON_VITESSE_VSC7224	/** Vitesse reclocking chip, i2c based */
#define CONFIG_PHY_SFP			/** Enable SFP parsing support */
#define CONFIG_CMD_SFP			/** Enable command to display SFP info */
/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_XAUI_ENET
#define CONFIG_OCTEON_XLAUI_ENET
#define CONFIG_OCTEON_XFI_ENET
#define CONFIG_OCTEON_XLAUI_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#define CONFIG_OCTEON_INTERNAL_ENET
/** Configure QLM support */
#define CONFIG_OCTEON_QLM

#define CONFIG_OCTEON_SE_NAPI

/**
 * Define available PHYs
 */
#include "octeon_network.h"

#define CONFIG_SYS_PCI_CONSOLE
#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD

#define CONFIG_CMD_OCTEON_TLVEEPROM	/** "tlv_eeprom" command */

#define CONFIG_CMD_QLM			/** "qlm" command */

#define CONFIG_OCTEON_SE_NAPI		/** Enable SE native API support */

/** i2c eeprom address */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NICX40E_BOARD_EEPROM_TWSI_ADDR

/** Default eeprom address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_mmc=fatload mmc 1 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "autoload=n\0"					\
        "ls=fatls mmc 0\0"				\
        ""

#define CONFIG_CMD_EXT2			/** EXT2/3/4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FS_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/
#define CONFIG_FS_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_EXT4_WRITE		/** EXT4 write support		*/
#define CONFIG_CMD_PART			/** Enable part command		*/

#define CONFIG_SF_DEFAULT_BUS		0	/** Default SPI bus	*/
#define CONFIG_SF_DEFAULT_CS		0	/** Default SPI chip select */
#define CONFIG_SF_DEFAULT_SPEED		25000000/** Default SPI speed	*/

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_OCTEON_SPI		/** Enable Octeon SPI driver */
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver */
#define CONFIG_SPI_FLASH_MICRON		/** Enable Micron SPI flash */
#define CONFIG_CMD_SPI			/** Enable SPI command */
#define CONFIG_CMD_SF			/** Enable SPI flash command */
#define CONFIG_SPI_FLASH_BAR		/** Enable full access to SPI flash */

/** Store environment in SPI flash */
#define	CONFIG_ENV_IS_IN_SPI_FLASH	1

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

/** Environment SPI flash sector size */
#define CONFIG_ENV_SECT_SIZE		256

#define CONFIG_ENV_SIZE		(8*1024)	/** Environment size */
#define CONFIG_ENV_OFFSET	0xe000		/** Environment offset */

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/**
 * DTT sensor
 * The NICX40E has a SA56004X
 */
#define CONFIG_DTT_SAA56004X
/**
 * Enable DTT command
 */
#define CONFIG_CMD_DTT
/**
 * Enable DTT driver support
 */
#define CONFIG_DTT_SENSORS		{ 0, 1 }
/**
 * Address of DTT sensor on I2C bus
 */
#define CONFIG_SYS_I2C_DTT_ADDR		0x4C
/**
 * I2C bus for DTT sensor
 */
#define CONFIG_SYS_I2C_DTT_BUS		0

/**
 * Temperature sensor settings
 */
#define CONFIG_SYS_DTT_SAA56004X 		\
{ {	.i2c_addr = CONFIG_SYS_I2C_DTT_ADDR,	\
	.conv_rate = 4,				\
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

#define CONFIG_SYS_NUM_I2C_BUSES	2	/** Number of i2c buses */

#define CONFIG_PCA9555			/** GPIO expander used with SFP slots */
#define CONFIG_PCA953X			/** GPIO expander used with SFP slots */
#define CONFIG_SYS_I2C_PCA9555_BUS	1	/** PCA9555 I2C Bus number */
#define CONFIG_SYS_I2C_PCA9555_ADDR	0x21	/** PCA9555 I2C address */
#define CONFIG_SYS_I2C_PCA953X_WIDTH	{ 0x21, 1, 16 }
#define CONFIG_CMD_PCA9555
#define CONFIG_CMD_PCA9555_INFO			/** Enable info command */

/** Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_nicx40e_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

/* The following line should be defined for all PCIe connected LiquidIO card
 * It will do initialization needed for oct-remote-app-ctl host utilty to
 * provide hotplug features when controlling start/add_cores/del_cores/shutdown
 * hotplugable SE application from x86 HOST
 */
#define CONFIG_OCTEON_REMOTE_HOTPLUG

#endif	/* __CONFIG_H__ */
