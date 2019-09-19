/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*
 * This file contains the configuration parameters for
 * Octeon generic eMMC boards.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_OCTEON_NIAGARA83X_MAJOR		1
#define CONFIG_OCTEON_NIAGARA83X_MINOR		0

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

#define CONFIG_OCTEON_MMC_MAX_FREQUENCY	20000000

#include "octeon_common_emmc_stage2.h"

#define CONFIG_SYS_LPC_IO_BASE		0x10020000

#define CONFIG_OCTEON_LPC_UART

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define UART0_BASE			0x3f8
#define CONFIG_SYS_NS16550_COM1		(CONFIG_SYS_LPC_IO_BASE + UART0_BASE)

#define CONFIG_OCTEON_NIAGARA83X_W83627

/*
#ifdef CONFIG_SYS_NS16550_SERIAL
# undef CONFIG_OCTEON_SERIAL

# define CONFIG_CONS_INDEX		1

# define CONFIG_SYS_NS16550_CLK		14769000
#endif
*/
# define CONFIG_CONS_INDEX		1
# define CONFIG_SYS_NS16550_CLK		14769000

#define CONFIG_OCTEON_AUTO_PROMPT	/** Auto-generate prompt */
/* Default DDR clock if tuple doesn't exist in EEPROM */
#define OCTEON_NIAGARA83X_DEF_DRAM_FREQ		666

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR	(OCTEON_NIAGARA83X_BOARD_EEPROM_TWSI_ADDR)
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR

#define CONFIG_BOOTDELAY	0
#define CONFIG_BOOTCOMMAND	"bootstage3"

#define CONFIG_SPI			/** Enable SPI support		*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_STMICRO	/** Enable ST Micro SPI flash	*/

#define CONFIG_SF_DEFAULT_BUS		0	/** Default SPI bus	*/
#define CONFIG_SF_DEFAULT_CS		0	/** Default SPI chip select */
#define CONFIG_SF_DEFAULT_SPEED		16000000 /** Default SPI speed	*/
#define CONFIG_OCTEON_SPI_BOOT_START	0x10000
#define CONFIG_OCTEON_SPI_BOOT_END	0x400000

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/

#define CONFIG_OCTEON_MD5
#define CONFIG_OCTEON_BOOT_BUS
#define CONFIG_CMD_OCTEON_BOOTBUS
#undef	CONFIG_BOOTARGS

#include "octeon_cmd_conf.h"

#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_CMD_EXT2			/* EXT2fs support		*/
#define CONFIG_CMD_EXT4			/* EXT4fs support		*/
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_FDT
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_NET

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
#undef CONFIG_CMD_SAVEENV
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
/* #undef CONFIG_CMD_OCTEON_EEPROM*/
#define CONFIG_CMD_OCTEON_TLVEEPROM

/*
 * Miscellaneous configurable options
 */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"bootcmd=bootstage3\0"			\
	"bootdelay=0\0"

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_niagara83x_shared.h"

#endif	/* __CONFIG_H__ */
