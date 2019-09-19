/***********************license start************************************
 * Copyright (c) 2005-2013 Cavium, Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium, Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/


/* This file sets configuration options that are common to all Octeon
 * bootloaders, and is included by each board's config header file.
 */
#ifndef __OCTEON_COMMON_H__
#define __OCTEON_COMMON_H__

#ifndef __U_BOOT__
# define __U_BOOT__
#endif

#define _MIPS_ARCH_OCTEON2	1

/* This is the address that U-Boot is linked to run at. */
#ifndef CONFIG_SYS_TEXT_BASE
#  define CONFIG_SYS_TEXT_BASE		0xC0004000
#endif

/** We always run U-Boot in big endian mode */
#define CONFIG_SYS_BIG_ENDIAN

/** Limit image size to 2MB */
#ifndef CONFIG_BOARD_SIZE_LIMIT
# define CONFIG_BOARD_SIZE_LIMIT	0x00200000
#endif

/** Physical memory requires 64-bit addressing */
#define CONFIG_PHYS_64BIT

/** GD contains board type */
#define CONFIG_BOARD_TYPES

#define CONFIG_OCTEON_EMMC_STAGE2	1

/* Stage 1 bootloader address and size */
#define CONFIG_OCTEON_EMMC_STAGE1_ADDR	0xFFFFFFFF81000000ull
#define CONFIG_OCTEON_EMMC_STAGE1_SIZE	(8 * 1024)

#define CONFIG_OCTEON_EMMC_STAGE2_CACHE_ADDR	0xFFFFFFFF81004000ull
#define CONFIG_OCTEON_EMMC_STAGE2_CACHE_SIZE	\
			(512 * 1024 - CONFIG_OCTEON_EMMC_STAGE1_SIZE - 8192)

/* This enables detailed output support when debugging memory initialization */

/** Enable serial driver */
#define CONFIG_OCTEON_SERIAL

/** Enable TWSI (I2C) driver */
#define CONFIG_OCTEON_I2C

#define CONFIG_CMD_OCTEON_BOOT_STAGE3		/** Enable bootstage3 command */

/**
 * Filename used if the environment variable octeon_stage3_bootloader is
 * not set.
 */
#ifndef CONFIG_OCTEON_STAGE3_FILENAME
# define CONFIG_OCTEON_STAGE3_FILENAME		"u-boot-octeon_stage3.bin"
#endif

/**
 * Filename used if the environment variable octeon_stage3_failsafe_bootloader
 * is not set.
 */
#ifndef CONFIG_OCTEON_STAGE3_FAILSAFE_FILENAME
#define CONFIG_OCTEON_STAGE3_FAILSAFE_FILENAME	"u-boot-octeon_stage3_failsafe.bin"
#endif

#if !defined(__U_BOOT_HOST__) && !defined(__BYTE_ORDER)
/* Set endian macros for simple exec includes, but not for host utilities */
#define __BYTE_ORDER __BIG_ENDIAN
#endif

#define CONFIG_MIPS32			1  /* MIPS32 CPU core	*/
#define CONFIG_OCTEON			1

/* EMMC booting is only supported on OCTEON2 and later so disable
 * DDR2 support.
 */
#define CONFIG_OCTEON_DISABLE_DDR2

/*Failsafe support is disabled */
#define CONFIG_OCTEON_NO_FAILSAFE

/* Assume no flash */
#define CONFIG_SYS_NO_FLASH		1

/* Disable network support */
#define OCTEON_NO_NETWORK

/* Disable boot bus support */
#define CONFIG_OCTEON_NO_BOOT_BUS	1

/* Used to control conditional compilation for shared code between simple
 * exec and u-boot, used for bootmem allocator in cvmx-bootmem-shared.c
 */
#define CONFIG_OCTEON_U_BOOT

/* Enable snprintf/vsnprintf support */
#define CONFIG_SYS_VSPRINTF

/* Default CPU reference frequency used if cpu reference clock reading fails.
 */
#ifndef DEFAULT_CPU_REF_FREQUENCY_MHZ
# define DEFAULT_CPU_REF_FREQUENCY_MHZ	50
#endif

/* Used if multiplier read fails, and for DRAM refresh rates */
#ifndef DEFAULT_ECLK_FREQ_MHZ
# define DEFAULT_ECLK_FREQ_MHZ		400
#endif

#define	CONFIG_TIMESTAMP		/* Print image info with timestamp */

/* Steps to use when looking for alternative bootloader in FLASH */
#define LOOKUP_STEP			(64*1024)

/* Maximum size of prompt */
#define CONFIG_SYS_MAX_PROMPT_LEN	(64)

#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_64BIT		/* Define this since we're always 64-bit. */
#define CONFIG_64BIT_PHYS_ADDR	/* Define for 64-bit physical addresses */

/* MIPS timer frequency */
#define CONFIG_SYS_MIPS_TIMER_FREQ	(gd->cpu_clk)

/* Address mapping support between virtual and physical addresses */
#define CONFIG_ADDR_MAP
#define CONFIG_SYS_NUM_ADDR_MAP		7
#define CONFIG_ADDR_MAP_CODE_IDX	0
#define CONFIG_ADDR_MAP_FIRST_256MB_IDX	1
#define CONFIG_ADDR_MAP_BOOT_BUS_IDX	2
#define CONFIG_ADDR_MAP_REST_MEM_IDX	3
#define CONFIG_ADDR_MAP_PCI0_BASE	4
#define CONFIG_ADDR_MAP_PCI1_BASE	5
#define CONFIG_ADDR_MAP_PCI2_BASE	6
#define CONFIG_ADDR_MAP_PCI3_BASE	7

/* Which GPIO pin is used to indicate that the failsafe bootloader should be
 * used.
 */
#define CONFIG_OCTEON_FAILSAFE_GPIO		0

/* Support DOS partitions for media */
#define CONFIG_DOS_PARTITION

/* Allow command line auto complete */
#define CONFIG_AUTO_COMPLETE

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	\
		{ 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }

/* default baudrate */
#define CONFIG_BAUDRATE			115200

/* default baudrate for downloads */
#define CONFIG_DOWNLOAD_BAUDRATE	115200

/* Don't print out console info on bootup */
#define CONFIG_SYS_CONSOLE_INFO_QUIET	1

/* Console is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

#define	CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size   */

/* Print Buffer Size */
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + CONFIG_SYS_MAX_PROMPT_LEN + 16)
#define	CONFIG_SYS_MAXARGS		64	/* max number of command args*/

/* Memory allocated for heap */
#define CONFIG_SYS_MALLOC_LEN		(2*1024*1024)

/* Default load address */
#ifndef CONFIG_SYS_LOAD_ADDR
# define CONFIG_SYS_LOAD_ADDR		0x20000000
#endif

/** Default stage 3 load address */
#ifndef CONFIG_OCTEON_STAGE3_LOAD_ADDR
# define CONFIG_OCTEON_STAGE3_LOAD_ADDR	0x80400000
#endif

/**
 * Enable support for the OCTEON GPIO driver
 */
#define CONFIG_OCTEON_GPIO

/**
 * Maximum size for stage 3 bootloader
 */
#ifndef CONFIG_OCTEON_STAGE3_MAX_SIZE
# define CONFIG_OCTEON_STAGE3_MAX_SIZE	(2 * 1024 * 1024)
#endif

/**
 * MMC device number for stage 3 bootloader
 */
#ifndef CONFIG_OCTEON_STAGE3_DEVNO
# define CONFIG_OCTEON_STAGE3_DEVNO	0
#endif

#ifndef CONFIG_OCTEON_STAGE3_DEVNAME
# define CONFIG_OCTEON_STAGE3_DEVNAME	"mmc"
#endif

/* Timer frequency - do not change! */
#define CONFIG_SYS_HZ			1000ull

/* Cache line size common in all Octeon processors */
#define CONFIG_SYS_CACHELINE_SIZE	128

/* If set passes "memsize" to Linux in bytes rather than MB */
#define CONFIG_MEMSIZE_IN_BYTES

/* Base of SDRAM */
#define CONFIG_SYS_SDRAM_BASE		0x80000000     /* Cached addr */

/* Addresses to use for memory test */
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + 0x100000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0xffffff)

/* let the eth address be writeable */
#define CONFIG_ENV_OVERWRITE		1

/* Set the TWSI clock to a conservative 100KHz. */
#define CONFIG_SYS_I2C_SPEED		(100000)

/* We have our own copy of memset/memcpy */
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCMP

/* We have our own copy of crc32 */
#define __HAVE_ARCH_CRC32

/* Enable hashing support */
#define CONFIG_OCTEON_SHA1
#define CONFIG_OCTEON_SHA256
#define CONFIG_OCTEON_

#define __HAVE_ARCH_SHA1
#define __HAVE_ARCH_SHA256
#define __HAVE_ARCH_MD5

/** Size of memory allocated for passing parameters to the Linux kernel */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128*1024)

/** Set if EEPROM contains OCTEON EEPROM tuples */
#define CONFIG_OCTEON_EEPROM_TUPLES		1

/** Size (in bytes) of the address in the EEPROM.  Should be 2 */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2

/** Maximum number of I2C buses on OCTEON */
#define CONFIG_MAX_I2C_NUM			2


/* Location of failsafe bootloader in flash */
#define FAILSAFE_BASE				0x1fc00000

/* The following #defines are needed to get flash environment right */
#define	CONFIG_SYS_MONITOR_BASE			CONFIG_SYS_TEXT_BASE

/* Starting address to look for a non-failsafe bootloader */
#define	CONFIG_SYS_MONITOR_LEN			(192 << 10)

/* Enable the "ver" environment variable */
#define CONFIG_VERSION_VARIABLE

/** Enable overwrite of previous console environment settings */
#define CONFIG_SYS_CONSOLE_ENV_OVERWRITE

/** Enable the call to overwrite_console() */
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE

/**
 * If overwrite_console returns 1 then stdin, stdout and stderr are switched
 * to the serial port, otherwise the environment settings are used.
 */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

/** We support multiple inputs and outputs */
#define CONFIG_CONSOLE_MUX

/** Enable I2C (TWSI) support */
#define CONFIG_SYS_I2C

/* Defines the default timeout in seconds for the watchdog timer.
 * Note that on most OCTEON devices this is 5 seconds though the
 * CN68XX supports 10 seconds.
 */
#ifndef CONFIG_OCTEON_WD_TIMEOUT
# define CONFIG_OCTEON_WD_TIMEOUT		5	/* 5 seconds */
#endif

/** Default UART to use for the console */
#ifndef CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT
# define CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT	0
#endif

#ifndef CONFIG_OCTEON_MIN_CONSOLE_UART
# define CONFIG_OCTEON_MIN_CONSOLE_UART		0
#endif
#ifndef CONFIG_OCTEON_MAX_CONSOLE_UART
# define CONFIG_OCTEON_MAX_CONSOLE_UART		1
#endif

/** Enable device tree support */
#ifndef CONFIG_OF_LIBFDT
# define CONFIG_OF_LIBFDT
# define CONFIG_OF_CONTROL
#endif
#ifndef CONFIG_LMB
# define CONFIG_LMB
#endif

/* eMMC support */
#define CONFIG_OCTEON_MMC		/* Enable MMC support */
#define CONFIG_MMC_MBLOCK		/* Multi-block support */
#define CONFIG_CMD_MMC			/* Enable mmc command */
#define CONFIG_SYS_MMC_SET_DEV		/* Enable multiple MMC devices */
#define CONFIG_MMC
#ifndef CONFIG_OCTEON_MMC_MAX_FREQUENCY
# define CONFIG_OCTEON_MMC_MAX_FREQUENCY	20000000
#endif
#ifndef CONFIG_OCTEON_MMC_MAX_WIDTH
# define CONFIG_OCTEON_MMC_MAX_WIDTH	8
#endif
#ifndef CONFIG_OCTEON_MAX_MMC_SLOT
# define CONFIG_OCTEON_MAX_MMC_SLOT	4	/* Maximum MMC bus */
#endif
/* Bit mask of available slots when device tree is not present */
#ifndef CONFIG_OCTEON_AVAIL_MMC_SLOTS
# define CONFIG_OCTEON_AVAIL_MMC_SLOTS	(1/*|2|4|8*/)
#endif

/* Memory mapped by startup code of the Linux kernel where the bd_info,
 * boot arguments, flat device tree and any other data needed by the
 * Linux kernel.
 *
 * This should be the same as OCTEON_RESERVED_LOW_MEM_SIZE.
 */
#define CONFIG_SYS_BOOTMAPSZ			(1*1024*1024)

/**
 * Maximum number of blocks that can be transferred in an operation for MMC
 */
#define CONFIG_SYS_MMC_MAX_BLK_COUNT	8191

/** There is no stored environment for the stage 2 bootloader */
#define CONFIG_ENV_IS_NOWHERE

/* There is no environment support */
#undef CONFIG_CMD_SAVEENV
#undef CONFIG_CMD_ASKENV
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_SAVEENV

/** Use a small 4K buffer to hold the pre-defined environment variables */
#define CONFIG_ENV_SIZE			4096

#endif /* __OCTEON_COMMON_H__ */
