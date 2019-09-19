/*
 * This file contains the configuration parameters for
 * Octeon IM8724 board.
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
# define CONFIG_OCTEON_PCI_HOST			1
#endif

#undef CONFIG_OCTEON_OCX
#define CONFIG_OCTEON_OCX		/** Enable NUMA OCI support */

#define CONFIG_SYS_NO_FLASH                     1
#define CONFIG_OCTEON_BIG_STACK_SIZE		0x4000	/** 16K stack */

#include "octeon_common.h"

/* CONFIG_OCTEON_IM8724 set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM, according to
 * CN78XX datasheet this CPU can not support 2400MT/s,
 * the best it can do is 2133MT/s (for alternative 100Mhz reference clock).
 */
#define IM8724_DEF_DRAM_FREQ			1066 /* 800 */

#define CONFIG_NIAGARA
#define CONFIG_SYS_NIAGARA_REMOTE

/* eMMC support */
#define CONFIG_OCTEON_MMC		/* Enable MMC support */
#define CONFIG_MMC_MBLOCK		/* Multi-block support */
#define CONFIG_CMD_MMC			/* Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/* Enable multiple MMC devices */
#define CONFIG_MMC
#define CONFIG_OCTEON_MIN_BUS_SPEED_HZ  100000
#define CONFIG_OCTEON_MAX_MMC_SLOT      2

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x54
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

#define CONFIG_I2C_ENV_EEPROM_BUS	1
#define CONFIG_SYS_EEPROM_I2C_BUS_NUM	CONFIG_I2C_ENV_EEPROM_BUS

/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	2	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_XFI_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET) || defined(CONFIG_OCTEON_QSGMII_ENET) || \
	defined(CONFIG_OCTEON_XLAUI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define the available PHYs
 */
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_VITESSE
# define CONFIG_PHY_CORTINA
# define CONFIG_PHY_CORTINA_CS4223
# define CONFIG_PHY_CORTINA_CS4318
# define CONFIG_PHY_MARVELL88X5113

# include "octeon_network.h"
#endif

/* "qlm" command */
#define CONFIG_CMD_QLM

/* Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_DATE                 /** Date command for RTC        */
#define CONFIG_CMD_EXT2                 /** EXT2/3 filesystem support   */
#define CONFIG_CMD_EXT4                 /** EXT4 filesystem support     */
#define CONFIG_CMD_FAT                  /** FAT support                 */
#define CONFIG_FAT_WRITE                /** FAT write support           */

/* SPI NOR flash support */
#define CONFIG_SF_DEFAULT_BUS           0
#define CONFIG_SF_DEFAULT_CS            0
#define CONFIG_SF_DEFAULT_SPEED         50000000

#define CONFIG_OCTEON_SPI               /* Enable OCTEON SPI driver     */
#define CONFIG_SPI_FLASH                /* Enable SPI flash driver      */
#define CONFIG_SPI_FLASH_MACRONIX       /* Enable MXC SPI flash */
#define CONFIG_CMD_SPI                  /* Enable SPI command           */
#define CONFIG_CMD_SF                   /* Enable SPI flash command     */

/** Enable NVME support */
#define CONFIG_NVME
#define CONFIG_CMD_NVME
#define CONFIG_SYS_NVME_MAX_DEVICE	8

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default (set ddr_use_ecc=0, if ECC issue) */
#define	CONFIG_EXTRA_ENV_SETTINGS			\
        "autoload=n\0"					\
        "ddr_ref_hertz=100000000\0"			\
        ""

#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_OFFSET		0       /* EEPROM is 64KB */
#define CONFIG_ENV_IS_IN_EEPROM		1
#undef CONFIG_ENV_IS_IN_FLASH
#define CONFIG_CMD_ERASEENV

/* Address and size of Primary Environment Sector       */
#define CONFIG_ENV_SIZE			(8*1024)
#define CONFIG_ENV_ADDR			CONFIG_ENV_OFFSET


#define CONFIG_CMD_OCTEON_TLVEEPROM     /** "tlv_eeprom" command        */
#define CONFIG_EEPROM_TLV_BASE_ADDRESS  (32*1024)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/* The CN78XX has two TWSI buses and the IM8724 board uses both of them */
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		2

#define CONFIG_RTC_MCP7940N                     /** Real-time clock support */
#define CONFIG_SYS_I2C_RTC_ADDR         0x6F    /** RTC I2C address */
#define CONFIG_SYS_RTC_BUS_NUM          1       /** RTC I2C bus number */

/* Enable watchdog support */
/*#define CONFIG_HW_WATCHDOG*/

/** Configure QLM support */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_im8724_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#define CONFIG_STOPKEY

#endif	/* __CONFIG_H__ */
