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
 * Octeon Thunder board.
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

#include "octeon_common.h"

/* Defaults to use if bootloader cannot autodetect settings */
#define OCTEON_DDR_CLOCK_MHZ			266

/* Addresses for various things on boot bus.  These addresses
 * are used to configure the boot bus mappings.
 */
/* CF related settings */
#define CONFIG_SYS_ATA_BASE_ADDR	0
#define CONFIG_SYS_ATA_DATA_OFFSET	0x400
#define CONFIG_SYS_ATA_REG_OFFSET	0

#define CONFIG_SYS_IDE_MAXBUS		1
#define CONFIG_SYS_IDE_MAXDEVICE	1

#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/
#define CONFIG_CMD_IDE			/* IDE harddisk support		*/

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define BOARD_ISP_TWSI_ADDR		0x08

/** Only one I2C bus */
#define CONFIG_SYS_NUM_I2C_BUSES	1

#define BOARD_MCU_AVAILABLE		0
#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_THUNDER_EEPROM_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5
/*  uses configuration eeprom */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/* Enable PCI console support. */
#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

/*
 * The Thunder does not use the internal arbiter in Octeon.
 * Enable this for boards that do.
 */
/* #define USE_OCTEON_INTERNAL_ARBITER */

/* Change the NAND/NOR MTD partitioning here.  The JFFS2 filesystem should be
 * kept as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabel for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:1m(bootloader)ro,"	\
	"2560k(kernel),"				\
	"4600k(cramfs),"				\
	"8k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_RGMII_ENET
/* Define this to enable spi4000 support */
#define CONFIG_OCTEON_SPI4000_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MARVELL
# include "octeon_network.h"
#endif

#define CONFIG_CMD_OCTEON_TLVEEPROM

#define CONFIG_CONSOLE_IS_IN_ENV

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/

#define CONFIG_CMD_DATE

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"				\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */

#define CONFIG_SYS_FLASH_CFI_WIDTH		FLASH_CFI_8BIT
#define CONFIG_SYS_FLASH_INTERFACE_WIDTH	FLASH_CFI_X8
#define CONFIG_SYS_FLASH_CFI			1
#define CONFIG_FLASH_CFI_DRIVER			1

#define OCTEON_GPIO_FAILSAFE_BIT		8

/* Since flash is optional assume the environment is in RAM */
#define CONFIG_ENV_IS_IN_RAM			1

/* NOTE: These must match cvmx-bootloader.h */
/* Defines for RAM based environment set by the host or the previous bootloader
 * in a chain boot configuration.
 */
#define U_BOOT_RAM_ENV_ADDR		(0x1000)
#define U_BOOT_RAM_ENV_SIZE		(0x1000)
#define U_BOOT_RAM_ENV_CRC_SIZE		(0x4)
#define U_BOOT_RAM_ENV_ADDR_2	(U_BOOT_RAM_ENV_ADDR + U_BOOT_RAM_ENV_SIZE)

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE			U_BOOT_RAM_ENV_SIZE
#define CONFIG_ENV_ADDR			U_BOOT_RAM_ENV_ADDR

/* Real-time Clock */
#define CONFIG_RTC_DS1337
#define CONFIG_SYS_I2C_RTC_ADDR		0x68

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(8 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(32 * 1024)

#if 0
/* DTT sensor
 * The Thunder board has an LM83 temperature sensor.
 *
 * - enable D1 critical temp interrupt
 * - enable D2 critical temp interrupt
 * - enable D3 critical temp interrupt
 * - enable local critical temp interrupt
 * - critical temperature
 * - interrupt active low
 * - local highpoint alarm temp
 * - D1 highpoint alarm temp
 * - D2 highpoint alarm temp
 * - D3 highpoint alarm temp
 */
#define CONFIG_SYS_I2C_DTT_ADDR 0x2A
#define CONFIG_DTT_LM83
#define CONFIG_SYS_DTT_LM83	{	.enable_d1_crit = 1,    \
					.enable_d2_crit = 0,    \
					.enable_d3_crit = 0,    \
					.enable_local_crit = 1, \
					.tcrit_temp = 100,      \
					.invert_int = 0,        \
					.local_highpt = 60,     \
					.d1_highpt = 85,        \
					.d2_highpt = 127,       \
					.d3_highpt = 127        \
				}
#define CONFIG_DTT_SENSORS     	{ 0, 1 }
#define CONFIG_CMD_DTT
#endif

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_thunder_shared.h"

/** Disable DDR3 support */
#define CONFIG_OCTEON_DISABLE_DDR3

/*-----------------------------------------------------------------------
 * DRAM Module Organization
 *
 * Octeon can be configured to use two pairs of DIMM's, lower and
 * upper, providing a 128/144-bit interface or one to four DIMM's
 * providing a 64/72-bit interface.  This structure contains the TWSI
 * addresses used to access the DIMM's Serial Presence Detect (SPD)
 * EPROMS and it also implies which DIMM socket organization is used
 * on the board.  Software uses this to detect the presence of DIMM's
 * plugged into the sockets, compute the total memory capacity, and
 * configure DRAM controller.  All DIMM's must be identical.
 */

/* Thunder has one pair of DIMM sockets. DIMM 0 must be populated.
 * Populating DIMM 1 enables the 128/144-bit interface.
 */
#define DRAM_SOCKET_CONFIGURATION	OCTEON_THUNDER_DRAM_SOCKET_CONFIGURATION

/* Board delay in picoseconds */
#define DDR_BOARD_DELAY			OCTEON_THUNDER_DDR_BOARD_DELAY

#endif	/* __CONFIG_H__ */
