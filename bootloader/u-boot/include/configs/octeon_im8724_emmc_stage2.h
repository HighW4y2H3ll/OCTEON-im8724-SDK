
/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2014 Cavium, Inc. <support@cavium.com>
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
 * Octeon generic eMMC boards.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_OCTEON_IM8724_MAJOR		1
#define CONFIG_OCTEON_IM8724_MINOR		0

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

#define CONFIG_OCTEON_BIG_STACK_SIZE           0x4000  /** 16K stack */

#define CONFIG_OCTEON_MMC_MAX_FREQUENCY	10000000

#include "octeon_common_emmc_stage2.h"

/* Default DDR clock if tuple doesn't exist in EEPROM, according to
 * CN78XX datasheet this CPU can not support 2400MT/s,
 * the best it can do is 2133MT/s (for alternative 100Mhz reference clock).
 */
#define IM8724_DEF_DRAM_FREQ		1066 /* 800 */

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x54
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_OFFSET		0 /* EEPROM is 64KB */
#undef CONFIG_ENV_IS_IN_FLASH
#define CONFIG_CMD_ERASEENV

#define CONFIG_CMD_OCTEON_TLVEEPROM    /** "tlv_eeprom" command        */
#define CONFIG_EEPROM_TLV_BASE_ADDRESS (32*1024)
	/* Should use ENV_SIZE (8k) but causes header dependency issues */
/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS      5       /* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS  5

#define CONFIG_I2C_ENV_EEPROM_BUS	1
#define CONFIG_SYS_EEPROM_I2C_BUS_NUM	CONFIG_I2C_ENV_EEPROM_BUS

/* GPIO Failsafe pin used to load alternate stage 3 */
#undef CONFIG_OCTEON_FAILSAFE_GPIO
#define CONFIG_OCTEON_FAILSAFE_GPIO	2
/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY		1 /* autoboot after X seconds	*/
#define CONFIG_BOOTCOMMAND		"bootstage3"

#define CONFIG_OCTEON_MD5
#undef CONFIG_CMD_OCTEON_BOOTBUS

#undef	CONFIG_BOOTARGS

#include "octeon_cmd_conf.h"

#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

/* Enable "date" command to set and get date from RTC */
#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_CMD_EXT2			/* EXT2fs support		*/
#define CONFIG_CMD_EXT4			/* EXT4fs support		*/
#define CONFIG_CMD_LOADB
#undef CONFIG_CMD_LOADS

#undef CONFIG_CMD_OCTEON
#undef CONFIG_CMD_OCTEON_ELF
/*#undef CONFIG_CMD_OCTEON_MEM*/
#undef CONFIG_CMD_OCTEON_LINUX
#undef CONFIG_CMD_STRINGS
#undef CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_MISC
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_OCTEON_REGINFO
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_ASKENV
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_CONSOLE
#undef CONFIG_CMD_SOURCE
#undef CONFIG_CMD_XIMG
#undef CONFIG_MII
#undef CONFIG_CMD_MII
#define CONFIG_CMD_UNZIP
#undef CONFIG_CMD_BOOTM
#undef CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE
/*#undef CONFIG_CMD_MEMORY*/
/*#undef CONFIG_CMD_I2C*/
/*#undef CONFIG_CMD_OCTEON_EEPROM*/
#define CONFIG_MENUPROMPT "Hit ctrl-x to stop booting..."
#define CONFIG_MENUKEY 24 /* ctrl-x */
#undef CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_STOPKEY

/*
 * Miscellaneous configurable options
 */
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "bootcmd=bootstage3\0"						\
        "bootdelay=1\0"

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#undef CONFIG_ENV_IS_NOWHERE
#define	CONFIG_ENV_IS_NOWHERE	1

/* Address and size of Primary Environment Sector	*/
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE		(8*1024)

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_im8724_shared.h"

#define CONFIG_OCTEON_DISABLE_DDR2

#endif	/* __CONFIG_H__ */
