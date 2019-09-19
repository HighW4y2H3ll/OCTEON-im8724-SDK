/*
 * (C) Copyright 2015-2016 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon NIC73 board.
 */

#ifndef __OCTEON_NIC73_H__
#define __OCTEON_NIC73_H__

/** Enable the more powerful hush command parser */
#define CONFIG_SYS_HUSH_PARSER

#define CONFIG_OCTEON_BIG_STACK_SIZE		0x4000	/** 16K stack */

#define CONFIG_API		/** Enable U-Boot native applications */

/** NOR flash is only used for expansion ROM */
#define CONFIG_OCTEON_EXPANSION_ROM_ONLY

/** Enable Octeon secure-mode by disabling bar1 on PCIE bus 0 */
#define CONFIG_OCTEON_SECURE_MODE_BUS	0

#include "octeon_common.h"

/** Default DDR clock if tuple doesn't exist in EEPROM */
#define NIC73_DEF_DRAM_FREQ		800

/** Enable SE application native API support */
#define CONFIG_OCTEON_SE_NAPI

/** TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define CONFIG_ISPPAC_POWER

/** ISP power controller TWSI address */
#define BOARD_ISP_TWSI_ADDR		0x08

/** The NIC73 board does not have a MCU */
#define BOARD_MCU_AVAILABLE		0

/** i2c eeprom address */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NIC73_BOARD_EEPROM_TWSI_ADDR

/** Default eeprom address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

/** Enable support for PMBus power management chips */
#define CONFIG_PMBUS

/** Enable support for the MP2953A chip */
#define CONFIG_PMBUS_MP2953

/** Power management chips are configured via I2C */
#define CONFIG_POWER_I2C

#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/** 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5	/** Delay for page write */

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/** autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/* Flash timing parameters */

/** Page mode (disabled) */
#define CONFIG_OCTEON_START_FLASH_PAGEM		0
/** Pages set to 0 */
#define CONFIG_OCTEON_START_FLASH_PAGES		0
/** ALE not used */
#define CONFIG_OCTEON_START_FLASH_ALE		0
/** Page count (not used) */
#define CONFIG_OCTEON_START_FLASH_PAGE		8
/** Wait count, only used when WAITM = 1 */
#define CONFIG_OCTEON_START_FLASH_WAIT		0x31
/** Pause count */
#define CONFIG_OCTEON_START_FLASH_PAUSE		4
/** Write hold count */
#define CONFIG_OCTEON_START_FLASH_WR_HLD	2
/** Read hold count */
#define CONFIG_OCTEON_START_FLASH_RD_HLD	3
/** Write enable count */
#define CONFIG_OCTEON_START_FLASH_WE		4
/** Output enable count */
#define CONFIG_OCTEON_START_FLASH_OE		4
/** Chip enable count */
#define CONFIG_OCTEON_START_FLASH_CE		8
/** Address count */
#define CONFIG_OCTEON_START_FLASH_ADR		2

/** Initialize MIO_BOOT_REG_CFG0 to this value */
#define CONFIG_OCTEON_START_FLASH_CFG			0x300804f1fb0

/** No failsafe for NIC cards */
/*#define CONFIG_OCTEON_NO_FAILSAFE*/

#define CONFIG_OCTEON_QLM	/** Enable QLM support */

/** Define this to enable built-in octeon ethernet support (we use XFI) */
#define CONFIG_OCTEON_XAUI_ENET

/** Enable SFP support */
#define CONFIG_PHY_SFP

/** Enable generic SFP command */
#define CONFIG_CMD_SFP

/** Enable Octeon SFP support */
#define CONFIG_OCTEON_SFP

/** Enable XFI Ethernet */
#define CONFIG_OCTEON_XFI_ENET
/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_RXAUI_ENET) || \
    defined(CONFIG_OCTEON_SGMII_ENET) || defined(CONFIG_OCTEON_XFI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

#include "octeon_cmd_conf.h"


/*
 * Define the available PHYs
 */
#define CONFIG_PHY_VITESSE		/** Enable Vitesse support */
#define CONFIG_OCTEON_VITESSE_VSC7224	/** Vitesse reclocking chip, i2c based */

/** Identify card for PXE booting */
#define CONFIG_BOOTP_VCI_STRING		"CaviumPXE:nic73:000000"

/** Support a larger TFTP block size */
#define CONFIG_TFTP_BLOCKSIZE	4096

/** Maximum receive packet size (defaults to 1536) */
#define CONFIG_OCTEON_NETWORK_MRU	9216

#include "octeon_network.h"

#define CONFIG_CMD_OCTEON_TLVEEPROM	/** Enable TLV EEPROM support */

/** Console information is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_EXT2			/** EXT2/3/4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FS_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/
#define CONFIG_FS_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_EXT4_WRITE		/** EXT4 write support		*/

#define CONFIG_SF_DEFAULT_BUS		0	/** Default SPI bus	*/
#define CONFIG_SF_DEFAULT_CS		0	/** Default SPI chip select */
#define CONFIG_SF_DEFAULT_SPEED		25000000/** Default SPI speed	*/

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_MICRON		/** Enable Micron SPI flash	*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_FLASH_BAR		/** Enable full access to SPI flash */

/** Environment is stored in SPI flash */
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

/** Size of environment */
#define CONFIG_ENV_SIZE			(8 * 1024)

/** Address in SPI flash is at at 56K (environment partition) */
#define CONFIG_ENV_OFFSET		0xe000

#define CONFIG_LIBATA			/** Enable ATA/SATA support	*/
#define CONFIG_CMD_SATA			/** Enable the sata command */
#define CONFIG_SYS_SATA_MAX_DEVICE	2	/* Support up to 2 devices */
#define CONFIG_SATA_AHCI		/** Enable AHCI driver */
#define CONFIG_SATA_AHCI_PLAT		/** Enable platform AHCI support */
#define CONFIG_SATA_AHCI_OCTEON		/** Enable Octeon AHCI support */
#define CONFIG_AHCI_SETFEATURES_XFER

#define CONFIG_CMD_PART			/** Enable part command */
/*
 * Miscellaneous configurable options
 */

/** Default environment settings */
#define	CONFIG_EXTRA_ENV_SETTINGS			\
        "autoload=n\0"					\
        "bootdelay=0\0"					\
        "cm_mtdparts=spi32766.0:56k(bootloader_failsafe)ro,8k(bootloader_env),131000k(nor_playground)\0" \
        "altbootcmd=dhcp; tftp; bootoctlinux; echo; echo Failed to PXE boot.  Will sleep and retry.; echo; sleep 60; run altbootcmd\0"

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_OCTEON_MMC		/** Enable Octeon MMC driver */

/** Needed for SE API */
#define CONFIG_SYS_MMC_MAX_DEVICE	1
/** Minimum MMC bus speed */
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ	100000
#define CONFIG_CMD_MMC				/** Enable mmc command */
#define CONFIG_MMC				/** Enable MMC support */
#define CONFIG_SYS_MMC_ENV_DEV		0	/** MMC device containing the env */
#define CONFIG_SYS_MMC_MAX_DEVICE 	1

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
/** L1 data cache size (not used */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)

/** instruction cache size (not used) */
#define CONFIG_SYS_ICACHE_SIZE		(72 * 1024)

/**
 * DTT sensor
 * The NIC73 has a SA56004X
 */
#define CONFIG_SYS_I2C_DTT_ADDR		0x4C

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_nic73_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

/** Make DHCP loop forever */
#undef CONFIG_BOOTP_MAY_FAIL

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/* Add NOR flash support */
#define CONFIG_CMD_FLASH		/** flinfo, erase, protect	*/
#define CONFIG_SYS_FLASH_SIZE		(1*1024*1024)	/** 1MB */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/** Max memory banks	*/
#define CONFIG_SYS_MAX_FLASH_SECT	32	/** Max flash sectors	*/
/** Width of flash */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
#define CONFIG_SYS_FLASH_CFI		1	/** Enable CFI NOR flash */
#define CONFIG_FLASH_CFI_DRIVER		1	/** Enable CFI NOR flash */
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE	/** Speed up writes	*/

/** Make dhcp loop forever */
#undef CONFIG_BOOTP_MAY_FAIL

/** Enable support for reading EEPROM override data */
#define CONFIG_OCTEON_PCI

/* The following line should be defined for all PCIe connected LiquidIO card
 * It will do initialization needed for oct-remote-app-ctl host utilty to
 * provide hotplug features when controlling start/add_cores/del_cores/shutdown
 * hotplugable SE application from x86 HOST
 */
#define CONFIG_OCTEON_REMOTE_HOTPLUG


#endif	/* __OCTEON_NIC73_H__ */
