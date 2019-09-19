/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon CNF7300 board.
 */

#ifndef __CONFIG_OCTEON_CNF7300_H__
#define __CONFIG_OCTEON_CNF7300_H__

/** GPIO 0 is not connected for failsafe support */
#define CONFIG_OCTEON_NO_FAILSAFE
/**
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		1
#endif

/** Default speed for the SPI bus */
#define CONFIG_SYS_SPI_SPEED		250000

#define CONFIG_OCTEON_BIG_STACK_SIZE	0x4000	/** 16K stack */

#include "octeon_common.h"

/* CONFIG_OCTEON_CNF73XX set by Makefile in include/config.h */

/** Default DDR clock if tuple doesn't exist in EEPROM */
#define CNF7300_DEF_DRAM_FREQ		400

#define CONFIG_LBA48			/** 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/** 64-bit LBA support */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /** Make compile happy */
/* Base address of Common memory for Compact flash */

/* eMMC support */
#define CONFIG_OCTEON_MMC		/** Enable MMC support */
#define CONFIG_MMC_MBLOCK		/** Multi-block support */
#define CONFIG_CMD_MMC			/** Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/** Enable multiple MMC devices */
#define CONFIG_MMC			/** Enable MMC support */
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000	/** Minimum MMC bus speed */

/** Maximum number of MMC devices needed for API */
#define CONFIG_SYS_MMC_MAX_DEVICE	1

#if 0
/** Enable support for native U-Boot application API */
#define CONFIG_API
#endif


/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	5	/** autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/**
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"octeon_nor0:4m(bootloader)ro,"		\
	"28544k(storage),"				\
	"128k(environment)ro\0"

/** MTD device mapping */
#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/** Enable SFP support */
#define CONFIG_PHY_SFP

/** Enable generic SFP command */
#define CONFIG_CMD_SFP

/** Enable Octeon SFP support */
#define CONFIG_OCTEON_SFP

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET	/** SGMII ethernet support */

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif
#define CONFIG_OCTEON_MGMT_ENET

/* PCI console is supported since oct-remote-boot is supported.
 * Enable MUX and oct-remote-bootcmd support as well
 */
#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD

#include "octeon_cmd_conf.h"

/** Support a larger TFTP block size */
#define CONFIG_TFTP_BLOCKSIZE	4096

/** Maximum receive packet size (defaults to 1536) */
#define CONFIG_OCTEON_NETWORK_MRU	9216

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE	/** Gigabit Ethernet PHY support */
# define CONFIG_PHY_MARVELL	/** Marvell PHY support */
# include "octeon_network.h"
#endif



#define CONFIG_CMD_OCTEON_TLVEEPROM	/** "tlv_eeprom" command */

/* Enable MTD partitioning support */
#define CONFIG_CMD_MTDPARTS

/* Enable JFFS2 support */
#define CONFIG_CMD_JFFS2

/** Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_FLASH		/** flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/

/*
 * Miscellaneous configurable options
 */
/** Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_mmc=fatload mmc 1 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "autoload=n\0"					\
        "ls=fatls mmc 0\0"				\
	"bootcmd=printenv;run namedalloc;run bootcby${bootby}\0"\
	"namedalloc=namedalloc initrd 0x6000000 0x30800000;\0"	\
	"bootcbytftp=tftp 0x28000000 lsm_os.gz; md5sum 0x28000000 ${filesize}; unzip 0x28000000 0x20000000 0x8000000; md5sum 0x20000000 ${filesize}; tftp 0x30800000 lsm_rd.gz; md5sum 0x30800000 ${filesize};bootoctlinux 0x20000000 numcores=16 mem=0 endbootargs rd_name=initrd;\0"	\
	"bootcbyflash=unzip 0x17e20000 0x20000000; cp.l 0x18320000 0x30800000 0xA00000; bootoctlinux 0x20000000 numcores=16 mem=0 endbootargs rd_name=initrd;\0" \
	"bootcbymmc=fatload mmc 0 0x28000000 lsm_os.gz; unzip 0x28000000 0x20000000 0x8000000; fatload mmc 0 0x30800000 lsm_rd.gz; bootoctlinux 0x20000000 numcores=16 mem=0 endbootargs rd_name=initrd;\0"			\
	"bootcby=setenv bootby flash;saveenv;run bootbyflash\0"	\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
/* NOTE: The schematic is wrong, it's a Macronix MX29GL256E device */
#define CONFIG_SYS_FLASH_SIZE	        (32*1024*1024)	/** Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(2048)	/* max number of sectors on one chip */

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
#define CONFIG_ENV_SIZE		(128*1024)	/** Size of environment */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

/*-----------------------------------------------------------------------
 * Cache Configuration (not used)
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)	/** L1 Data cache size */
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)	/** L1 icache size */

#if CONFIG_OCTEON_PCI_HOST
/** Right now only the Intel E1000 driver has been ported */
# define CONFIG_E1000
#endif

#define CONFIG_PCA9554			/** PCA9554 GPIO expander */

#define CONFIG_CMD_PCA9554		/** Command for GPIO expander */
#define CONFIG_CMD_PCA9554_INFO		/** Info command for expander */

/** The CNF75XX has three TWSI buses */
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		2

/** Maximum number of hops with I2C muxes */
#define CONFIG_SYS_I2C_MAX_HOPS		2

/** Enable i2c FDT support */
#define CONFIG_OCTEON_I2C_FDT

#define CONFIG_SYS_NUM_I2C_BUSES	13

#define CONFIG_SYS_I2C_BUSES				\
{							\
	{0, {I2C_NULL_HOP}},				\
	{1, {I2C_NULL_HOP}},				\
	{2, {I2C_NULL_HOP}},				\
	{1, {{I2C_MUX_PCA9546, 0x77, 0}}},		\
	{1, {{I2C_MUX_PCA9546, 0x77, 1}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 0}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 1}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 2}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 3}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 4}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 5}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 6}}},		\
	{2, {{I2C_MUX_PCA9548, 0x70, 7}}},		\
}

#undef CONFIG_SYS_I2C_DIRECT_BUS
#define CONFIG_I2C_MULTI_BUS

#define CONFIG_RTC_X1205			/** RTC clock chip */
#define CONFIG_SYS_I2C_RTC_ADDR		0x6F	/** RTC I2C address */
#define CONFIG_SYS_RTC_BUS_NUM		0	/** RTC I2C bus */

/** Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/** Configure QLM support */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_cnf7300_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_OCTEON_CNF7300_H__ */
