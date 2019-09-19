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
 * Octeon NIC10E board.
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
# define CONFIG_OCTEON_PCI_HOST		1 /* BCM57711 is on the PCIE bus */
#endif

#define CONFIG_OCTEON_USB_OCTEON2	/* Enable USB support on OCTEON II */

#include "octeon_common.h"

/* CONFIG_OCTEON_NIC10E set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define NIC10E_DEF_DRAM_FREQ		533

/* Addresses for various things on boot bus.  These addresses
 * are used to configure the boot bus mappings.
 */

/* Remap the Octeon TWSI Slave Address. Default is 0x77. */
/* #define TWSI_BUS_SLAVE_ADDRESS 0x65 */

/* The NIC10e board does not have a MCU */
#define BOARD_MCU_AVAILABLE		0

#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_NIC10E_BOARD_EEPROM_TWSI_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/* Change the NAND/NOR MTD partitioning here.  The JFFS2 filesystem should be
 * kept as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabel for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:1m(bootloader)ro,"	\
	"2944k(kernel),"				\
	"128k(environment)ro;"				\
	"octeon_nand0:4m(nandboot)ro,"			\
	"2m(boot-env)ro,"				\
	"8m(ubifs),"					\
	"256m(ubifs-kernel),"				\
	"256m(ubifs-bigfs)\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0,nand0=octeon_nand0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_XAUI_ENET	/* Board has XAUI interface */

/* Enable Octeon built-in networking if XAUI support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

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

# include "octeon_network.h"
#endif

#include "config_octeon_nand.h"

#define CONFIG_CMD_OCTEON_TLVEEPROM
#define CONFIG_CMD_QLM

/* Console information is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (4*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */
#define CONFIG_SYS_CFI_FLASH_STATUS_POLL	/* Polling support */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT
#define CONFIG_CFI_FLASH_USE_WEAK_ACCESSORS

#define CONFIG_SYS_FLASH_CFI		1
#define CONFIG_FLASH_CFI_DRIVER		1

#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(128*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

#define CONFIG_SYS_MAX_NAND_DEVICE	2
/* Not used for Octeon, but must be set */
#define CONFIG_SYS_NAND_BASE		0
#define CONFIG_SYS_NAND_MAX_CHIPS CONFIG_SYS_MAX_NAND_DEVICE

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)
#if 1
/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG
#endif
/**
 * According to the HRM these addresses are reserved to extend 10 bit
 * addressing
 */
#define CONFIG_SYS_I2C_NOPROBES		{0x78, 0x79, 0x7a, 0x7b}

/* DTT sensor
 * The NIC10E has a SAA56004X
 */
#define CONFIG_DTT_SAA56004X
#define CONFIG_CMD_DTT
#define CONFIG_DTT_SENSORS		{ 0, 1 }
#define CONFIG_SYS_I2C_DTT_ADDR		0x4A

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

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_nic10e_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#define OCTEON_CONFIG_DFM	(1)

/* Samsung K4B2G0846C */
#define DFM_CFG_NUM_BANKS          8
#define DFM_CFG_DRAM_WIDTH         8
#define DFM_CFG_ROW_BITS          15
#define DFM_CFG_COL_BITS          10
#define DFM_CFG_NUM_RANKS          2
#define DFM_CFG_TCKMIN	        1500
#define DFM_CFG_TAAMIN	       13125
#define DFM_CFG_TWR	       15000 /* ps, Write Recovery Time (tWR)                     */
#define DFM_CFG_TRCD	       13125 /* ps, Minimum RAS to CAS delay (tRCD)               */
#define DFM_CFG_TRRD	        6000 /* ps, Minimum Row Active to Row Active delay (tRRD) */
#define DFM_CFG_TRP	       13125 /* ps, Minimum Row Precharge Delay (tRP)             */
#define DFM_CFG_TRAS	       36000 /* ps, Minimum Active to Precharge (tRAS)            */
#define DFM_CFG_TRC	       49125 /* ps, Minimum Active to Active/Refresh Delay (tRC)  */
#define DFM_CFG_TRFC	      160000 /* ps, Minimum Refresh Recovery Delay (tRFC)         */
#define DFM_CFG_TWTR	        7500 /* ps, Internal write to read command delay (tWTR)   */
#define DFM_CFG_TRTP	        7500 /* ps, Min Internal Rd to Precharge Cmd Delay (tRTP) */
#define DFM_CFG_TFAW	       30000 /* ps, Minimum Four Activate Window Delay (tFAW)     */
#define DFM_CFG_ADDR_MIRROR        1
#define DFM_EARLY_DQX              0 /* Set for the rare case when ADD/CMD signals are
                                        routed shorter than the shortest DQS/DQ. */
#define DFM_CFG_CAS_LATENCIES ((1<<(11-4)) | (1<<(10-4)) | (1<<(9-4)) | (1<<(8-4)) | (1<<(7-4)) | (1<<(6-4)) | (1<<(5-4)))

#define OCTEON_NIC10E_CN63XX_DFM_ODT_CONFIGURATION \
    /* RANKS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1            RODT_CTL    RODT_MASK    reserved */ \
    /* =====   ======== ============== ========================================== ========= ============== ======== */ \
    /*   1 */ {   0,    0x00000001ULL, OCTEON_NIC10E_MODEREG_PARAMS1_1RANK_1SLOT,    5,     0x00000000ULL,  0  },     \
    /*   2 */ {   0,    0x00000101ULL, OCTEON_NIC10E_MODEREG_PARAMS1_2RANK_1SLOT,    5,     0x00000000ULL,  0  }

#define OCTEON_CN63XX_DFM_ODT_CONFIGURATION OCTEON_NIC10E_CN63XX_DFM_ODT_CONFIGURATION

#endif	/* __CONFIG_H__ */
