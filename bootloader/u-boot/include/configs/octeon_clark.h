/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2013 Cavium, Inc. <support@cavium.com>
 *
 * Copyright (c) 2014 by cisco Systems, Inc.
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
 * the Clark board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Build configuration for cn70xx
 */
#define CONFIG_OCTEON_CN70XX

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

/* NOTE: Change this when the CN70XX is installed! */
#define CONFIG_OCTEON_USB_OCTEON3	/* Enable USB support on OCTEON III */

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH			1

#include "octeon_common.h"
#undef CONFIG_OCTEON_FLASH
#undef CONFIG_OCTEON_EEPROM_TUPLES
#undef CONFIG_SYS_I2C_EEPROM_ADDR_LEN

/* CONFIG_OCTEON_EVB7000_SFF set by Makefile in include/config.h */

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define CUST_CLARK_DEF_DRAM_FREQ	533

#define CONFIG_LBA48			/* 48-bit mode */
#define CONFIG_SYS_64BIT_LBA		/* 64-bit LBA support */
#define CONFIG_SYS_ATA_BASE_ADDR	0 /* Make compile happy */
/* Base address of Common memory for Compact flash */

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_SGMII_ENET
#define CONFIG_OCTEON_QSGMII_ENET

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

#undef CONFIG_CMD_EEPROM

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
#define CONFIG_PHY_GIGE
#define CONFIG_PHY_MARVELL
#include "octeon_network.h"
#endif

/* "qlm" command */
#define CONFIG_CMD_QLM

#define CONFIG_CMD_EXT2			/* EXT2/3 filesystem support	*/
#define CONFIG_CMD_EXT4			/* EXT4 filesystem support	*/
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_FAT_WRITE		/* FAT write support		*/

#define CONFIG_ENV_IS_IN_RAM

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

/*
 *
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS			\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

#if CONFIG_OCTEON_PCI_HOST
/* Right now only the Intel E1000 driver has been ported */
# define CONFIG_E1000
# define CONFIG_LIBATA
/* Enable support for the SIL3124 family */
# define CONFIG_SATA_SIL
#endif

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Configure QLM */
#define CONFIG_OCTEON_QLM

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_clark_shared.h"

/** Disable DDR2 support */
#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
