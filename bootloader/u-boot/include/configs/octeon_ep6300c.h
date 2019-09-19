/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2012 Cavium, Inc. <support@cavium.com>
 * All rights reserved.
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
 * Octeon EP6300C board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
** Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
** bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
** host system performs the pci bus mapping instead.  Note that pci
** commands are enabled to allow access to configuration space for
** both modes.
*/
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		1
#endif

#define CONFIG_OCTEON_USB_OCTEON2	/* Enable USB support on OCTEON II */

#include "octeon_common.h"

/* CONFIG_OCTEON_EP6300C set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define EP6300C_DEF_DRAM_FREQ  			533

/* Addresses for various things on boot bus.  These addresses
 * are used to configure the boot bus mappings.
 */

/* Compact flash reset uses the BCSR FPGA on this board */
#define OCTEON_CF_RESET_BCSR			1

/* Maximum number of IDE devices */
#define CONFIG_SYS_IDE_MAXDEVICE		1
/* Maximum number of IDE busses */
#define CONFIG_SYS_IDE_MAXBUS			1

/* Offset from base at which data is repeated so that it can be
 * read as a block
 */
#define CONFIG_SYS_ATA_DATA_OFFSET		0x400

/* Not sure what this does, probably offset from base
 * of the command registers
 */
#define CONFIG_SYS_ATA_REG_OFFSET		0

#define CONFIG_LBA48				/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA			/* 64-bit support */
#define CONFIG_IDE_RESET			/* Enable IDE reset support */
#define CONFIG_SYS_ATA_BASE_ADDR		0 /* Make compile happy */
#define CONFIG_IDE_PREINIT			/* Enable pre-initialization */
#define CONFIG_IDE_LED				/* Enable LED */

/* The EP6300C uses a board control and status register FPGA */
#define CONFIG_OCTEON_BCSR

/* EP6300C uses configuration eeprom. */
#define CONFIG_SYS_I2C_EEPROM_ADDR		OCTEON_EP6300C_BOARD_EEPROM_TWSI_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR		CONFIG_SYS_I2C_EEPROM_ADDR
/* These speed up writes to the serial EEPROM */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/*
 * The EP6300C does not use the internal arbiter in Octeon.
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
	"mtdparts=octeon_nand0:4m(bootloader)ro,"	\
	"2m(boot-env)ro,"				\
	"8m(fs-boot),"					\
	"256m(kernel),"					\
	"256m(bigfs);"					\
	"octeon_nor0:4m(bootloader)ro,"			\
	"4m(kernel),"					\
	"8m(cramfs),"					\
	"49024k(jffs2big),"				\
	"128k(environment)ro\0"

#define MTDIDS_DEFAULT	"nand0=octeon_nand0,nor0=octeon_nor0\0"

/* Define this for XAUI */
#define CONFIG_MII_CLAUSE_45

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET	/* Board has SGMII interface */
#define CONFIG_OCTEON_XAUI_ENET	/* Board has XAUI interface */
#define CONFIG_OCTEON_MGMT_ENET	/* Board has management enet interface */

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

/* Board can be a PCIe target so enable PCI console support */
#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define the available PHYs
 */
# define CONFIG_PHYLIB_10G
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_BROADCOM
# include "octeon_network.h"
#endif

#include "config_octeon_nand.h"

#define CONFIG_CMD_OCTEON_TLVEEPROM	/* Enable tlv_eeprom command	*/

/* NOTE: Remove the QLM command from end user products! */
#define CONFIG_CMD_QLM			/* QLM command support.		*/

/* Console is in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_MTDPARTS		/* mtdparts command support	*/
#define CONFIG_CMD_JFFS2		/* JFFS2 support		*/
#define CONFIG_JFFS2_LZO		/* LZO compression support	*/
#define CONFIG_JFFS2_SUMMARY		/* JFFS2 summary support	*/
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/
#define CONFIG_CMD_IDE			/* IDE harddisk support		*/

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_cf=fatload ide 0 $(loadaddr) vmlinux.64;bootoctlinux $(loadaddr)\0"		\
        "ls=fatls ide 0\0"				\
        "autoload=n\0"					\
        PCI_CONSOLE_ENV					\
        ""

#ifdef CONFIG_SYS_PCI_CONSOLE
# define PCI_CONSOLE_ENV	"stdin=serial,pci,bootcmd\0"	\
				"stdout=serial,pci\0"		\
				"stderr=serial,pci\0"
#else
# define PCI_CONSOLE_ENV
#endif

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE		(64*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */
#define CONFIG_OCTEON_FLASH_PAGE_MODE		/** Speeds up flash timing early */

#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_SYS_CFI_FLASH_STATUS_POLL
#define CONFIG_SYS_FLASH_CFI		1	/* Enable CFI flash support */
#define CONFIG_FLASH_CFI_DRIVER 	1	/* Enable CFI flash driver */

#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1	/* Environment is stored in flash */
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(128*1024)	/* Flash sector size */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

#define CONFIG_SYS_MAX_NAND_DEVICE	1
/* Not used for Octeon, but must be set */
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_SYS_NAND_MAX_CHIPS	CONFIG_SYS_MAX_NAND_DEVICE
#define CONFIG_SYS_NAND_FORCE_WIDTH	8

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
# define CONFIG_SYS_SATA_MAX_DEVICE	32	/* Support up to 2 devices */
/** Enable support for the SIL3124 family */
# define CONFIG_SATA_SIL
/*# define CONFIG_SATA_AHCI*/
# define CONFIG_SYS_SATA_MAX_LUN	16
/*# define CONFIG_SATA_PMP*/
#endif

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* DTT sensor
 * The ep6300c board has a TI TMP421 temperature sensor.  This part is basically
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
#define CONFIG_SYS_I2C_DTT_ADDR	0x4c
#define CONFIG_DTT_TMP42X
#define CONFIG_SYS_DTT_TMP42X	{ 0, 1, 0, 0, 1, 1, 2, 0, 0, 0}
#define CONFIG_DTT_SENSORS	{ 0, 1 }
#define CONFIG_CMD_DTT

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_ep6300c_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#define OCTEON_CONFIG_DFM	(1)

/* MT41J128M8JP-15E:F  16 Meg x 8 x 8 banks  */
#define DFM_CFG_NUM_BANKS          8
#define DFM_CFG_DRAM_WIDTH         8
#define DFM_CFG_ROW_BITS          15
#define DFM_CFG_COL_BITS          10
#define DFM_CFG_NUM_RANKS          2
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

#define OCTEON_EP6300C_CN63XX_DFM_ODT_CONFIGURATION \
    /* RANKS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1            RODT_CTL    RODT_MASK    reserved */ \
    /* =====   ======== ============== ========================================== ========= ============== ======== */ \
    /*   1 */ {   0,    0x00000101ULL, OCTEON_EP6300C_MODEREG_PARAMS1_1RANK_1SLOT,    5,     0x00000000ULL,  0  },     \
    /*   2 */ {   0,    0x00000001ULL, OCTEON_EP6300C_MODEREG_PARAMS1_2RANK_1SLOT,    5,     0x00000000ULL,  0  }

#define OCTEON_CN63XX_DFM_ODT_CONFIGURATION OCTEON_EP6300C_CN63XX_DFM_ODT_CONFIGURATION

#endif	/* __CONFIG_H__ */
