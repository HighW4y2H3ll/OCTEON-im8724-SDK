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
 * Octeon MOONSHOT board.
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

#define CONFIG_OCTEON_USB_OCTEON2	/* Enable USB support on OCTEON II */

/* Moonshot's console UART is set to port 1 for bring-up */
#define CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT		1

#include "octeon_common.h"

/* CONFIG_OCTEON_MOONSHOT set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define MOONSHOT_DEF_DRAM_FREQ			533

/* Maximum IDE bus */
#define CONFIG_LBA48				/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA			/* 64-bit LBA mode */

/* MOONSHOT has two I2C busses but only one is used */
#define CONFIG_SYS_NUM_I2C_BUSES		2
#define CONFIG_SYS_MAX_I2C_BUS			1
#define CONFIG_SYS_SPD_BUS_NUM			0
#define CONFIG_SYS_DTT_BUS_NUM			0

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR		OCTEON_MOONSHOT_BOARD_EEPROM_TWSI_ADDR
/* Default system EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR		CONFIG_SYS_I2C_EEPROM_ADDR
/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY			0 /* autoboot after X seconds */

#undef	CONFIG_BOOTARGS

/*
 * The MOONSHOT does not use the internal arbiter in Octeon.
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
	"octeon_nor0:2176k(bootloader)ro,"		\
	"2304k(kernel),"				\
	"3704k(cramfs),"				\
	"8k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_MGMT_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_SGMII_ENET) || \
	defined(CONFIG_OCTEON_XAUI_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

/* Since we can boot over the PCIE bus, enable support for the PCI console
 * and oct-remote-bootcmd.  MUX is required as well.
 */
#define CONFIG_SYS_PCI_CONSOLE
#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD /* Deprecated */

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_BROADCOM
# include "octeon_network.h"
#endif

/* Enable "tlv_eeprom "command */
#define CONFIG_CMD_OCTEON_TLVEEPROM
/* Enable "qlm" command */
#define CONFIG_CMD_QLM

/* Enable JFFS2 support */
#define CONFIG_CMD_JFFS2
/* Enable MTD partition support */
#define CONFIG_CMD_MTDPARTS
/* Enable "date" command to get and set date since we have a RTC. */
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "linux_usb=fatload usb 0 $(loadaddr) vmlinux.64;bootoctlinux $(fileaddr)\0"		\
        "ls=fatls ide 0\0"				\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        (8*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */

/* Starting bus width to check for CFI flash */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_8BIT

/* Flash driver uses extra elements in common flash structure to store
 * geometry information.
 */
#define CONFIG_SYS_FLASH_CFI		1
/* Enable CFI flash driver */
#define CONFIG_FLASH_CFI_DRIVER		1

/* We're not RAM resident usualy so env is in flash */
#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE	1
#else
# define	CONFIG_ENV_IS_IN_FLASH	1
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(8*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

#if 0
#if CONFIG_OCTEON_PCI_HOST
# define CONFIG_LIBATA
# define CONFIG_CMD_SATA
# define CONFIG_SATA_AHCI
# define CONFIG_SYS_SATA_MAX_DEVICE	8
# define CONFIG_SYS_SATA_MAX_LUN	16
#endif
#endif

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_moonshot_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
