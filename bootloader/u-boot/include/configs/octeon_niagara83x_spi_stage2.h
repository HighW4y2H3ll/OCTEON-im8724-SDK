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
 * Octeon Niagara83x SPI boards.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_OCTEON_NIAGARA83X_MAJOR		1
#define CONFIG_OCTEON_NIAGARA83X_MINOR		0

/**
 * Build configuration for cn70xx
 */
#ifndef CONFIG_OCTEON_CN70XX
# define CONFIG_OCTEON_CN70XX
#endif

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

/**
 * Enable SPI boot support
 */
#define CONFIG_OCTEON_SPI_BOOT
#define CONFIG_OCTEON_NO_FAILSAFE
#define CONFIG_OCTEON_DISABLE_UBI
#define CONFIG_OCTEON_DISABLE_JFFS2
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_OCTEON_DISABLE_NETWORKING
#define CONFIG_OCTEON_DISABLE_QLM

#include "octeon_common_spi_stage2.h"

/* CONFIG_OCTEON_GENERIC_NAND set by Makefile in include/config.h */
#define SPI_STAGE2_BOARD_TYPE			CVMX_BOARD_TYPE_NIAGARA83X
/**
 * Add command to boot stage 3 bootloader out of NAND
 */
#define CONFIG_CMD_OCTEON_SPI_BOOT_STAGE3

/**
 * Start of where the SPI bootloader images are placed.  Must be on an erase
 * block boundary/
 */
#define CONFIG_OCTEON_SPI_BOOT_START		0x10000

/**
 * End of the NAND boot partition which is searched for a bootloader.  Must be
 * on an erase block boundary.  It should be large enough to be able to handle
 * at least one bad block.
 */
#define CONFIG_OCTEON_SPI_BOOT_END		0x600000

/**
 * Address to load the stage 3 bootloader for booting
 */
#define CONFIG_OCTEON_STAGE3_LOAD_ADDR		0x81000000

/**
 * Define the GPIO signal used to indicate the failsafe bootloader
 * should be used.
 */
#define CONFIG_OCTEON_FAILSAFE_GPIO		0


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

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	1	/** autoboot after X seconds	*/
#define CONFIG_BOOTCOMMAND	"bootstage3"

#define CONFIG_OCTEON_MD5
#define CONFIG_OCTEON_BOOT_BUS
#define CONFIG_CMD_OCTEON_BOOTBUS
#undef	CONFIG_BOOTARGS

#include "octeon_cmd_conf.h"

#define CONFIG_CMD_LOADB
#define CONFIG_CMD_FDT
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
/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"bootcmd=bootstage3\0"						\
	"bootdelay=0\0"

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_CMD_SAVEENV		/** Enable saveenv command	*/
/** Store the environment just in front of the first partition */
#define CONFIG_ENV_OFFSET		0x0e000

#define CONFIG_ENV_IS_IN_SPI		1	/** Environment in MMC	*/
#undef CONFIG_ENV_IS_IN_FLASH

/* Size of Primary Environment */
#define CONFIG_ENV_SIZE		(8*1024)
/** Address of primary environment */
#define CONFIG_ENV_ADDR		CONFIG_ENV_OFFSET

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)

/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_niagara83x_shared.h"

#endif	/* __CONFIG_H__ */
