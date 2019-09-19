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
 * Octeon SFF7800 board.
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
# define CONFIG_OCTEON_PCI_HOST		1
#endif

#define CONFIG_OCTEON_OCX		/** Enable NUMA OCI support */
/** Enable USB support on OCTEON III */
#define CONFIG_OCTEON_USB_OCTEON3

/** 78xx only supports 1 USB controller but 2 with OCX */
#ifdef CONFIG_OCTEON_OCX
# define CONFIG_USB_MAX_CONTROLLER_COUNT		2
#else
# define CONFIG_USB_MAX_CONTROLLER_COUNT		1
#endif

#define CONFIG_OCTEON_BIG_STACK_SIZE            0x4000  /** 16K stack */

#include "octeon_common.h"

/* CONFIG_OCTEON_SFF7800 set by Makefile in include/config.h */

/** Default DDR clock if tuple doesn't exist in EEPROM */
#define SFF7800_DEF_DRAM_FREQ		667

#define CONFIG_LBA48			/** 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/** 64-bit LBA support */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /* Make compile happy */
/* Base address of Common memory for Compact flash */

/* eMMC support */
#define CONFIG_OCTEON_MMC		/** Enable MMC support */
#define CONFIG_MMC_MBLOCK		/** Multi-block support */
#define CONFIG_CMD_MMC			/** Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/** Enable multiple MMC devices */
#define CONFIG_MMC
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#if 0
#define BOARD_ISP_TWSI_ADDR		0x08
#define CONFIG_ISPPAC_POWER
#endif

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/**
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"octeon_nor0:4m(bootloader)ro,"		\
	"61312k(storage),"				\
	"128k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

#define CONFIG_PHY_SFP

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_XAUI_ENET
#define CONFIG_OCTEON_XFI_ENET
#define CONFIG_OCTEON_XLAUI_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_XFI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_SFP
# define CONFIG_OCTEON_SFP
# define CONFIG_CMD_SFP
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MARVELL
# define CONFIG_PHY_VITESSE
# define CONFIG_PHY_BROADCOM
# define CONFIG_PHY_CORTINA
# define CONFIG_PHY_CORTINA_CS4223
# include "octeon_network.h"
#endif


/** "tlv_eeprom" command This board does not have a TLV EEPROM */
#undef CONFIG_CMD_OCTEON_TLVEEPROM

/** Enable MTD partitioning support */
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
#define CONFIG_CMD_OCTEON_TLVEEPROM	/** Enable TLV EEPROM command	*/

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

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (128*1024*1024)	/** Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/** max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(2048)	/** max number of sectors on one chip */

/** Width of CFI bus to start scanning */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
/** Enable extra elements in CFI driver for storing flash geometry */
#define CONFIG_SYS_FLASH_CFI  		1
/** Enable CFI flash driver */
#define CONFIG_FLASH_CFI_DRIVER		1
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

/* We're not RAM_RESIDENT so CONFIG_ENV_IS_IN_FLASH will be set. */
#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1	/** Store environment in flash */
#endif

/** Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(128*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/*
 * Speed up flash.  Here we use a hardcoded value that will
 * work up to 1 GHz.  The speed will be adjusted again once
 * the actual speed of the CPU is determined, but this setting here
 * makes things much faster, as the hardware default at boot is
 * very slow.
 *
 * PAGEM[63]	 = 0x00
 * WAITM[62]	 = 0x00
 * PAGES[61:60]	 = 0x00
 * ALE[59:54]	 = 0x08
 * PAGE[53:48]	 = 0x18
 * WAIT[47:42]	 = 0x00
 * PAUSE[41:36]	 = 0x10
 * WR_HLD[35:30] = 0x18
 * RD_HLD[29:24] = 0x00
 * WE[23:18]	 = 0x3f
 * OE[17:12]	 = 0x20
 * CE[11:6]	 = 0x3f
 * ADR[5:0]	 = 0x10
 */

/** ALE cycle count */
#define CONFIG_OCTEON_START_FLASH_ALE		8
/** PAGE cycle count */
#define CONFIG_OCTEON_START_FLASH_PAGE		0x18
/** pause cycle count */
#define CONFIG_OCTEON_START_FLASH_PAUSE		0x10
/** Write hold cycle count */
#define CONFIG_OCTEON_START_FLASH_WR_HLD	0x18
/** Read hold cycle count */
#define CONFIG_OCTEON_START_FLASH_RD_HLD	0
/** Write enable cycle count */
#define CONFIG_OCTEON_START_FLASH_WE		0x3f
/** Output enable cycle count */
#define CONFIG_OCTEON_START_FLASH_OE		0x20
/** Chip enable cycle count */
#define CONFIG_OCTEON_START_FLASH_CE		0x3f
/** Address cycle count */
#define CONFIG_OCTEON_START_FLASH_ADR		0x10

#define CONFIG_OCTEON_PAGE_MODE	/** Speeds up flash */

/**
 * DTT sensor
 * The sff7800 has a SAA56004X
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
#define CONFIG_SYS_I2C_DTT_ADDR		0x4A
/**
 * I2C bus for DTT sensor
 */
#define CONFIG_SYS_I2C_DTT_BUS		1

/**
 * Temperature sensor settings
 */
#define CONFIG_SYS_DTT_SAA56004X 		\
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

#if CONFIG_OCTEON_PCI_HOST
/* Right now only the Intel E1000 driver has been ported */
# define CONFIG_E1000
# define CONFIG_LIBATA
# define CONFIG_CMD_SATA			/* Enable the sata command */
# define CONFIG_SYS_SATA_MAX_DEVICE	2	/* Support up to 2 devices */
/* Enable support for the SIL3124 family */
# define CONFIG_SATA_SIL
#endif

/* The CN78XX has two TWSI buses and the SFF7800 board uses both of them */
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		3
#undef CONFIG_SYS_I2C_DIRECT_BUS

/** An I2C MUX is present */
#define CONFIG_I2C_MUX

/** Maximum number of i2c hops */
#define CONFIG_SYS_I2C_MAX_HOPS		1

/** Number of i2c buses */
#define CONFIG_SYS_NUM_I2C_BUSES	26


/** Configure for the I2C MUXes to handle the SFP+ ports */
#define CONFIG_SYS_I2C_BUSES			\
{						\
	{0, {I2C_NULL_HOP} },			\
	{1, {I2C_NULL_HOP} },			\
	{2, {I2C_NULL_HOP} },			\
	{3, {I2C_NULL_HOP} },			\
	{0, {{I2C_MUX_PCA9546, 0x71, 0} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 1} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 3} } },	\
	{0, {{I2C_MUX_PCA9546, 0x72, 0} } },	\
	{0, {{I2C_MUX_PCA9546, 0x72, 1} } },	\
	{0, {{I2C_MUX_PCA9546, 0x72, 2} } },	\
	{0, {{I2C_MUX_PCA9546, 0x72, 3} } },	\
	{0, {{I2C_MUX_PCA9546, 0x73, 0} } },	\
	{0, {{I2C_MUX_PCA9546, 0x73, 1} } },	\
	{0, {{I2C_MUX_PCA9546, 0x73, 2} } },	\
	{0, {{I2C_MUX_PCA9546, 0x73, 3} } },	\
	{2, {{I2C_MUX_PCA9546, 0x71, 0} } },	\
	{2, {{I2C_MUX_PCA9546, 0x71, 1} } },	\
	{2, {{I2C_MUX_PCA9546, 0x71, 3} } },	\
	{2, {{I2C_MUX_PCA9546, 0x72, 0} } },	\
	{2, {{I2C_MUX_PCA9546, 0x72, 1} } },	\
	{2, {{I2C_MUX_PCA9546, 0x72, 2} } },	\
	{2, {{I2C_MUX_PCA9546, 0x72, 3} } },	\
	{2, {{I2C_MUX_PCA9546, 0x73, 0} } },	\
	{2, {{I2C_MUX_PCA9546, 0x73, 1} } },	\
	{2, {{I2C_MUX_PCA9546, 0x73, 2} } },	\
	{2, {{I2C_MUX_PCA9546, 0x73, 3} } }	\
}

#define CONFIG_PCA953X				/** PCA9554 GPIO support */
#define CONFIG_SYS_I2C_PCA953X_ADDR	0x21	/** First PCA9554 i2c address */
#define CONFIG_SYS_I2C_PCA953X_BUS	0	/** PCA9554 i2c bus number */
#define CONFIG_CMD_PCA953X			/** Enable PCA953X GPIO command */
#define CONFIG_CMD_PCA953X_INFO			/** Enable info command */
/** Declare all of the PCA9554 chips */
#define CONFIG_SYS_I2C_PCA953X_WIDTH		\
{						\
	{0x21, 0, 8},				\
	{0x22, 0, 8},				\
	{0x23, 0, 8},				\
	{0x24, 0, 8},				\
	{0x25, 0, 8},				\
	{0x26, 0, 8}				\
}

#define CONFIG_RTC_DS1337			/** Real-time clock chip */
#define CONFIG_SYS_I2C_RTC_ADDR		0x68	/** Real-time clock i2c address */
#define CONFIG_SYS_RTC_BUS_NUM		0	/** Real-time clock i2c bus */

/** Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/** Configure QLM support */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_sff7800_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
