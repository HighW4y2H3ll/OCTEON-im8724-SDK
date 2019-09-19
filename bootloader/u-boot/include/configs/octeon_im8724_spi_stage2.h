/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_OCTEON_SPI_BOOT
/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST			0
#endif

#define CONFIG_OCTEON_IM8724_MAJOR		1
#define CONFIG_OCTEON_IM8724_MINOR		0

#define CONFIG_OCTEON_NO_FAILSAFE
#define CONFIG_OCTEON_DISABLE_UBI
#define CONFIG_OCTEON_DISABLE_JFFS2
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_OCTEON_DISABLE_NETWORKING
#define CONFIG_OCTEON_DISABLE_QLM

#define CONFIG_CMD_MEMTEST

#define CONFIG_OCTEON_BIG_STACK_SIZE           0x4000  /** 16K stack */

#include "octeon_common_spi_stage2.h"

/**
 * Default speed to run the SPI bus at.  It doesn't hurt to be conservative.
 */
#ifndef CONFIG_SF_DEFAULT_SPEED
# define CONFIG_SF_DEFAULT_SPEED        16000000
#endif

/**
 * Starting address for SPI stage 2 bootloader search, should be erase block
 * aligned and bigger than the stage 1 bootloader size.  64K ought to be enough
 * for anybody.
 */
#ifndef CONFIG_OCTEON_SPI_STAGE2_START
# define CONFIG_OCTEON_SPI_STAGE2_START 0x10000
#endif

//#define CONFIG_SUPPORT_EMMC_BOOT - generic only
#undef CONFIG_OCTEON_MAX_MMC_SLOT
#define CONFIG_OCTEON_MAX_MMC_SLOT 1

#define CONFIG_OCTEON_SPI		/** Enable OCTEON SPI driver	*/
#define CONFIG_SPI			/** Enable SPI support		*/
#define CONFIG_SPI_FLASH		/** Enable SPI flash driver	*/
#define CONFIG_SPI_FLASH_STMICRO	/** Enable ST Micro SPI flash	*/
#define CONFIG_CMD_SPI			/** Enable SPI command		*/
#define CONFIG_CMD_SF			/** Enable SPI flash command	*/
#define CONFIG_SPI_M95XXX		/** M95256 SPI NOR		*/
#define CONFIG_SPI_M95XXX_CS	0	/** Chip select offset		*/

#define CONFIG_OCTEON_MD5
#undef CONFIG_OCTEON_FAILSAFE_GPIO
#define CONFIG_OCTEON_NO_STAGE3_FAILSAFE

/* CONFIG_OCTEON_GENERIC_NAND set by Makefile in include/config.h */
#define SPI_STAGE2_BOARD_TYPE			CVMX_BOARD_TYPE_CUST_IM8724

/**
 * Add command to boot stage 3 bootloader out of NAND
 */
#define CONFIG_CMD_OCTEON_SPI_BOOT_STAGE3

/**
 * Start of where the SPI bootloader images are placed.  Must be on an erase
 * block boundary/
 */
#define CONFIG_OCTEON_SPI_BOOT_START		0x100000

/**
 * End of the NAND boot partition which is searched for a bootloader.  Must be
 * on an erase block boundary.  It should be large enough to be able to handle
 * at least one bad block.
 */
#define CONFIG_OCTEON_SPI_BOOT_END		0x400000

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define IM8724_DEF_DRAM_FREQ           		800 /* 1066 */

/* Address of board EEPROM on TWSI bus */
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x54
/* Default EEPROM address */
#define CONFIG_SYS_DEF_EEPROM_ADDR		CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_I2C_ENV_EEPROM_BUS		1
#define CONFIG_SYS_EEPROM_I2C_BUS_NUM		CONFIG_I2C_ENV_EEPROM_BUS

/* These speed up writes to the serial EEPROM by enabling page writes.
 * Please see the datasheet for details.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5	/* 32 bytes */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5

#define CONFIG_NIAGARA

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY			1 /* autoboot after X seconds	*/
#define CONFIG_BOOTCOMMAND			"bootstage3"

#undef	CONFIG_BOOTARGS
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

#define CONFIG_CMD_FAT			/* FAT support			*/
#define CONFIG_CMD_EXT2			/* EXT2fs support		*/
#define CONFIG_CMD_EXT4			/* EXT4fs support		*/
#define CONFIG_CMD_LOADB
#undef CONFIG_CMD_LOADS

#include "octeon_cmd_conf.h"

#define CONFIG_SYS_LONGHELP

#undef CONFIG_BZIP2
#undef CONFIG_ZIP
#undef CONFIG_LZMA
#undef CONFIG_REGEX
#undef CONFIG_FIT
#undef CONFIG_CMD_MEMINFO
#undef CONFIG_CMD_ASKENV
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_XIMG
#undef CONFIG_CMD_GPIO
#undef CONFIG_CMD_CRAMFS
#undef CONFIG_CMD_OCTEON_REGINFO
#undef CONFIG_CMD_I2C
#undef CONFIG_CMD_GREPENV
#undef CONFIG_CMD_OCTEON_LINUX
#undef CONFIG_CMD_OCTEON_ELF
#undef CONFIG_CMD_OCTEON
#undef CONFIG_CMD_OCTEON_MEM
#undef CONFIG_CMD_TIME
#undef CONFIG_CMD_STRINGS
#undef CONFIG_CMD_SOURCE
#undef CONFIG_CMD_NMI
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_CONSOLE
#undef CONFIG_CMD_SAVEENV
#undef CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_ENV
#undef CONFIG_CMD_FDT
#undef CONFIG_CMD_I2C
#undef CONFIG_CMD_BOOTM
#undef CONFIG_CMD_MEMORY

#undef CONFIG_ENV_IS_IN_FLASH
#undef CONFIG_CMD_OCTNAND

/* "tlv_eeprom" command */
#define CONFIG_CMD_OCTEON_TLVEEPROM

#undef CONFIG_HW_WATCHDOG

#define CONFIG_OCTEON_DISABLE_DDR2

#undef CONFIG_SYS_CBSIZE
#define CONFIG_SYS_CBSIZE		256

#undef CONFIG_OCTEON_QLM

/*
 * Miscellaneous configurable options
 */

/* Environment variables that will be set by default */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "autoload=n\0"							\
        "\0"
#define CONFIG_ENV_SIZE			4096

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(32 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(78 * 1024)


/* Include shared board configuration, consisting mainly of DRAM details. */
#include "octeon_im8724_shared.h"

#endif	/* __CONFIG_H__ */
