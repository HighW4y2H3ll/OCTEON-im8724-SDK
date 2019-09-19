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
 * Octeon Rainier board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/** Rainier's console is on UART port 1. */
#define CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT		0

/** TWSI address for ispPAC-POWR1220AT8 voltage controller chip */
#define BOARD_ISP_TWSI_ADDR	0x17

/** Driver for ISPPAC voltage controller chip */
#define CONFIG_ISPPAC_POWER

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

/* #undef CONFIG_OCTEON_OCX */		/** Enable NUMA OCI support */
/** Enable USB support on OCTEON III */
#define CONFIG_OCTEON_USB_OCTEON3

/** 78xx only supports 1 USB controller but 2 with OCX */
#ifdef CONFIG_OCTEON_OCX
# define CONFIG_USB_MAX_CONTROLLER_COUNT		2
#else
# define CONFIG_USB_MAX_CONTROLLER_COUNT		1
#endif


#define CONFIG_OCTEON_BIG_STACK_SIZE		0x4000	/** 16K stack */

#include "octeon_common.h"

/* CONFIG_OCTEON_RAINIER set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define RAINIER_DEF_DRAM_FREQ		800

#define CONFIG_LBA48			/** 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/** 64-bit LBA support */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /** Make compile happy */
/* Base address of Common memory for Compact flash */

/** Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_RAINIER_BOARD_EEPROM_TWSI_ADDR

/** Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/** autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/**
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"octeon_nor0:4096k(bootloader)ro,"		\
	"126848k(storage),"				\
	"128k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/** Enable SFP support */
#define CONFIG_PHY_SFP

/** Enable generic SFP command */
#define CONFIG_CMD_SFP

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_XAUI_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#define CONFIG_OCTEON_BOOTCMD

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MARVELL
# define CONFIG_PHY_CORTINA
# define CONFIG_PHY_CORTINA_CS4223
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_SFP		/** Enable SFP parsing support */
# define CONFIG_CMD_SFP		/** Enable command to display SFP information */
# include "octeon_network.h"
#endif


/** "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM

/** Enable MTD partitioning support */
#define CONFIG_CMD_MTDPARTS

/** Enable JFFS2 support */
#define CONFIG_CMD_JFFS2

/** Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/


/*
 * Miscellaneous configurable options
 */
/** Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_usb=fatload usb 1 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "autoload=n\0"					\
        "ls=fatls usb 1\0"				\
        "console_uart=0\0"				\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
/** Flash size (bytes) */
#define CONFIG_SYS_FLASH_SIZE	        (128*1024*1024)

/** max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_BANKS	1

/** max number of sectors on one chip */
#define CONFIG_SYS_MAX_FLASH_SECT	(2048)

/** Width of CFI bus to start scanning */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT

/** Enable extra elements in CFI driver for storing flash geometry */
#define CONFIG_SYS_FLASH_CFI  		1

/** Enable CFI flash driver */
#define CONFIG_FLASH_CFI_DRIVER		1

/** Speeds up writes */
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

/* We're not RAM_RESIDENT so CONFIG_ENV_IS_IN_FLASH will be set. */
#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1
#endif

/** Size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(128*1024)

/** Address of primary environment sector */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

#if CONFIG_OCTEON_PCI_HOST
/** Enable ATA support for SATA */
# define CONFIG_LIBATA
/** Enable sata command */
# define CONFIG_CMD_SATA
# define CONFIG_SYS_SATA_MAX_DEVICE	8	/** Support up to 8 devices */
/** Enable AHCI PCIe SATA controller */
# define CONFIG_SATA_AHCI
#endif

/** The CN78XX has two TWSI buses and the RAINIER board uses both of them */
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		1

/** Enable i2c FDT support */
#define CONFIG_OCTEON_I2C_FDT

/** ST M41T62 RTC chip */
#define CONFIG_RTC_M41T62

/** Address on I2C bus of RTC */
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/** I2C bus number of real-time clock */
#define CONFIG_SYS_RTC_BUS_NUM		1

/** Enable support for SA56004X */
#define CONFIG_DTT_SAA56004X

/** Enable dtt command */
#define CONFIG_CMD_DTT

/** Enable the two sensors */
#define CONFIG_DTT_SENSORS		{ 0, 1 }

/** Address of temperature sensor */
#define CONFIG_SYS_I2C_DTT_ADDR		0x4C

/** Temperature sensor settings */
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

/** Enable support for PCA953X device */
#define CONFIG_PCA953X

/** PCA9505 i2c address */
#define CONFIG_SYS_I2C_PCA953X_ADDR	0x22

/** PCA9505 i2c bus number */
#define CONFIG_SYS_I2C_PCA953X_BUS	0

/** Address, bus and number of GPIO pins per device */
#define CONFIG_SYS_I2C_PCA953X_WIDTH		\
{						\
	{0x21, 0, 16},				\
	{0x22, 0, 8},				\
	{0x23, 0, 8}				\
}

/** Add pca953x command */
#define CONFIG_CMD_PCA953X

/** Allow displaying device info */
#define CONFIG_CMD_PCA953X_INFO

#define CONFIG_PCA9698	/** Enable driver for PCA9505 */

#define CONFIG_CMD_PCA9698	/** Add pca9698 command */
#define CONFIG_CMD_PCA9698_INFO	/** Add pca9698 info command */

/** Enable driver for PCA9555 16 pin GPIO controller */
#define CONFIG_PCA9555

/** Enable "pca9555" command */
#define CONFIG_CMD_PCA9555

/** Enable "pca9555 info" command */
#define CONFIG_CMD_PCA9555_INFO

/** Address of PCA9555 16 pin GPIO controller */
#define CONFIG_SYS_I2C_PCA9555_ADDR	0x21

/** I2C bus number for PCA9555 16-pin GPIO controller */
#define CONFIG_SYS_I2C_PCA9555_BUS	0

/** An I2C mux is present */
#define CONFIG_I2C_MUX

/** With the muxes the max hops is 2 */
#define CONFIG_SYS_I2C_MAX_HOPS		2

/** Number of i2c buses with muxes */
#define CONFIG_SYS_NUM_I2C_BUSES	14

/** Configure the I2C MUX to handle the SFP ports */
#define CONFIG_SYS_I2C_BUSES			\
{						\
	{0, {I2C_NULL_HOP} },			\
	{1, {I2C_NULL_HOP} },			\
	{0, {{I2C_MUX_PCA9548, 0x70, 0} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 1} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 2} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 3} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 4} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 5} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 6} } },	\
	{0, {{I2C_MUX_PCA9548, 0x70, 7} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 0} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 1} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 2} } },	\
	{0, {{I2C_MUX_PCA9546, 0x71, 3} } },	\
}

#undef CONFIG_SYS_I2C_DIRECT_BUS
#define CONFIG_I2C_MULTI_BUS

/** Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/** Configure QLM support */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_rainier_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

#endif	/* __CONFIG_H__ */
