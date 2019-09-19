/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2012 Cavium, Inc. <support@cavium.com>
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
 * Octeon EBB6300 board.
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

#define CONFIG_OCTEON_USB_OCTEON2	/* Enable USB support on OCTEON II */

#include "octeon_common.h"

/* CONFIG_OCTEON_EBB6300 set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define EBB6300_DEF_DRAM_FREQ		533

#define CONFIG_OCTEON_ENABLE_PAL		/* Board has a PAL */

#define CONFIG_OCTEON_ENABLE_LED_DISPLAY	/* Board has an LED display */

#define OCTEON_CF_RESET_GPIO		5   /* Reset is GPIO 5 */

#define CONFIG_SYS_IDE_MAXDEVICE	1
#define CONFIG_SYS_IDE_MAXBUS		1
#define CONFIG_LBA48			/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/* 64-bit LBA support */
#define CONFIG_IDE_RESET		/* Board suppots IDE reset */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /* Make compile happy */
/* Base address of Common memory for Compact flash */

/** An I2C switch is present */
#define CONFIG_I2C_MUX

/** Maximum number of hops */
#define CONFIG_SYS_I2C_MAX_HOPS			2

/** We need to support multiple I2C buses, one per physical and muxed bus */
#define CONFIG_SYS_NUM_I2C_BUSES		10
/**
 * Configuration for the two 8-port I2C switches
 */
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
}

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define BOARD_ISP_TWSI_ADDR		0x08
#define CONFIG_ISPPAC_POWER

/* Board has a MCU */
#define BOARD_MCU_AVAILABLE		1

/* Address of MCU on TWSI bus */
#define BOARD_MCU_TWSI_ADDR		0x60

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_EBB6300_BOARD_EEPROM_TWSI_ADDR
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Real-time Clock */
#define CONFIG_RTC_DS1337
/* Address of RTC on TWSI bus */
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS


/*
 * The EBB6300 does not use the internal arbiter in Octeon.
 * Enable this for boards that do.
 */
/* #define USE_OCTEON_INTERNAL_ARBITER */

/* Change the NAND MTD partitioning here.  The JFFS2 filesystem should be kept
 * as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabel for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nand0:4m(nandboot)ro,"		\
	"2m(boot-env)ro,"				\
	"8m(ubifs),"					\
	"256m(ubifs-kernel),"				\
	"256m(ubifs-bigfs);"				\
	"octeon_nor0:2176k(bootloader)ro,"		\
	"2304k(kernel),"				\
	"3704k(cramfs),"				\
	"8k(environment)ro\0"

#define MTDIDS_DEFAULT	"nand0=octeon_nand0,nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_XAUII_ENET
#define CONFIG_OCTEON_MGMT_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET)
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
 * Define the available PHYs
 */
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_MARVELL
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_BROADCOM
# include "octeon_network.h"
#endif

/* Add NAND support */
#include "config_octeon_nand.h"

/** Enable multi-bit ECC support */
#define CONFIG_BCH
#define CONFIG_NAND_ECC_BCH

/* "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM
/* "qlm" command */
#define CONFIG_CMD_QLM
/* Enable "sntp" command for NTP */
#define CONFIG_CMD_SNTP

/** Enable MTD partitioning support */
#define CONFIG_CMD_MTDPARTS

/** Enable JFFS2 support */
#define CONFIG_CMD_JFFS2

/** Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_DATE
#define CONFIG_CMD_FLASH		/** flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/** EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/** EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/** FAT support			*/
#define CONFIG_FAT_WRITE		/** FAT write support		*/
/*#define CONFIG_EXT4_WRITE*/		/** EXT4 write support		*/
#define CONFIG_CMD_IDE			/** IDE harddisk support		*/

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_cf=fatload ide 0 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "ls=fatls ide 0\0"				\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(256)	/* max number of sectors on one chip */
#define CONFIG_OCTEON_FLASH_PAGE_MODE		/** Speeds up flash timing early */

/* Width of CFI bus to start scanning */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT

/* Enable extra elements in CFI driver for storing flash geometry */
#define CONFIG_SYS_FLASH_CFI  		1
/* Enable CFI flash driver */
#define CONFIG_FLASH_CFI_DRIVER		1

/* We're not RAM_RESIDENT so CONFIG_ENV_IS_IN_FLASH will be set. */
#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(8*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

/* Maximum NAND flash device */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
/* Force width to 8-bits */
#define CONFIG_SYS_NAND_FORCE_WIDTH	8	/* Force to 8-bit width */
/* Not used for Octeon, but must be set */
#define CONFIG_SYS_NAND_BASE		0
/* Maximum number of NAND chips */
#define CONFIG_SYS_NAND_MAX_CHIPS CONFIG_SYS_MAX_NAND_DEVICE

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

#if CONFIG_OCTEON_PCI_HOST
/* Right now only the Intel E1000 driver has been ported */
# define CONFIG_E1000
# define CONFIG_LIBATA
# define CONFIG_CMD_SATA			/* Enable the sata command */
# define CONFIG_SYS_SATA_MAX_DEVICE	4	/* Support up to 4 devices */
/* Enable support for the SIL3124 family */
# define CONFIG_SATA_SIL
#endif

/* DTT sensor
 * The ebb6300 board has a TI TMP421 temperature sensor.  This part is basically
 * compatible to the ADM1021 that is supported by U-Boot.
 *
 * - range -55C to +127C
 * - Remote channel 1 enabled
 * - Remote channel 2 disabled
 * - Remote channel 3 disabled
 * - Local channel enabled
 * - Resistance correction enabled
 * - conversion rate 0x02 = 0.25 conversions/second
 * - local temp sensor enabled, min set to 0 deg, max set to 70 deg
 * - remote temp sensor enabled, min set to 0 deg, max set to 70 deg
 * - n factor channel 1
 * - n factor channel 2
 * - n factor channel 3
 */
/* TWSI address of temperature sensor */
#define CONFIG_SYS_I2C_DTT_ADDR	0x4c
/* TI TMP42X temperature sensor driver support */
#define CONFIG_DTT_TMP42X
/* Default settings for TMP42X driver (see above) */
#define CONFIG_SYS_DTT_TMP42X	{ 0, 1, 0, 0, 1, 1, 2, 0, 0, 0}
/* Sensors used */
#define CONFIG_DTT_SENSORS	{ 0, 1 }
/* Enable "dtt" command */
#define CONFIG_CMD_DTT

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_ebb6300_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

/* This board has DFM memory connected */
#define OCTEON_CONFIG_DFM	(1)

/* MT41J128M8JP-15E:F  16 Meg x 8 x 8 banks  */
#define DFM_CFG_NUM_BANKS          8 /* Number of banks */
#define DFM_CFG_DRAM_WIDTH         8 /* DRAM width */
#define DFM_CFG_ROW_BITS          14 /* Row bits */
#define DFM_CFG_COL_BITS          10 /* Column bits */
#define DFM_CFG_NUM_RANKS          2 /* Number of ranks */
#define DFM_CFG_TCKMIN	        1500
#define DFM_CFG_TAAMIN	       13125
#define DFM_CFG_TWR	       15000 /* ps, Write Recovery Time (tWR)                     */
#define DFM_CFG_TRCD	       13500 /* ps, Minimum RAS to CAS delay (tRCD)               */
#define DFM_CFG_TRRD	        6000 /* ps, Minimum Row Active to Row Active delay (tRRD) */
#define DFM_CFG_TRP	       13500 /* ps, Minimum Row Precharge Delay (tRP)             */
#define DFM_CFG_TRAS	       36000 /* ps, Minimum Active to Precharge (tRAS)            */
#define DFM_CFG_TRC	       49500 /* ps, Minimum Active to Active/Refresh Delay (tRC)  */
#define DFM_CFG_TRFC	      110000 /* ps, Minimum Refresh Recovery Delay (tRFC)         */
#define DFM_CFG_TWTR	        7500 /* ps, Internal write to read command delay (tWTR)   */
#define DFM_CFG_TRTP	        7500 /* ps, Min Internal Rd to Precharge Cmd Delay (tRTP) */
#define DFM_CFG_TFAW	       30000 /* ps, Minimum Four Activate Window Delay (tFAW)     */
#define DFM_CFG_ADDR_MIRROR        1
#define DFM_EARLY_DQX		   0 /* Set for the rare case when ADD/CMD signals are
					routed shorter than the shortest DQS/DQ. */
#define DFM_CFG_CAS_LATENCIES ((1<<(11-4)) | (1<<(10-4)) | (1<<(9-4)) | (1<<(8-4)) | (1<<(7-4)) | (1<<(6-4)) | (1<<(5-4)))

#define OCTEON_EBB6300_CN63XX_DFM_ODT_CONFIGURATION \
    /* RANKS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1            RODT_CTL    RODT_MASK    reserved */ \
    /* =====   ======== ============== ========================================== ========= ============== ======== */ \
    /*   1 */ {   0,    0x00000001ULL, OCTEON_EBB6300_MODEREG_PARAMS1_1RANK_1SLOT,    5,     0x00000000ULL,  0  },     \
    /*   2 */ {   0,    0x00000101ULL, OCTEON_EBB6300_MODEREG_PARAMS1_2RANK_1SLOT,    5,     0x00000000ULL,  0  }

#define OCTEON_CN63XX_DFM_ODT_CONFIGURATION OCTEON_EBB6300_CN63XX_DFM_ODT_CONFIGURATION

#endif	/* __CONFIG_H__ */
