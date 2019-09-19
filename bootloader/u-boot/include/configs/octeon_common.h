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


/**
 * This file sets configuration options that are common to all Octeon
 * bootloaders, and is included by each board's config header file.
 */
#ifndef __OCTEON_COMMON_H__
#define __OCTEON_COMMON_H__

#ifndef __U_BOOT__
# define __U_BOOT__
#endif

/** This is the address that U-Boot is linked to run at. */
#ifndef CONFIG_SYS_TEXT_BASE
# define CONFIG_SYS_TEXT_BASE		0xC0000000
#endif
/** This is the address that standalone applications are loaded at */
#ifndef CONFIG_STANDALONE_LOAD_ADDR
# define CONFIG_STANDALONE_LOAD_ADDR	0xD0000000
#endif

/** We always run U-Boot in big endian mode */
#define CONFIG_SYS_BIG_ENDIAN

/**
 * Limit image size to 2MB
 */
#ifndef CONFIG_BOARD_SIZE_LIMIT
# define CONFIG_BOARD_SIZE_LIMIT 0x00200000
#endif

#ifndef CONFIG_OCTEON_SPI_STAGE2_START
/** Start offset for the SPI stage 2 bootloader.  It must be on an erase
 * boundary as well as bigger than the stage 1 bootloader.  64K ought to be
 * enough for anybody.
 */
# define CONFIG_OCTEON_SPI_STAGE2_START	0x00010000
#endif

/** This enables detailed output support when debugging memory initialization */

/** Physical memory requires 64-bit addressing */
#define CONFIG_PHYS_64BIT

/** GD contains board type */
#define CONFIG_BOARD_TYPES

#if !defined(__U_BOOT_HOST__) && !defined(__BYTE_ORDER)
/** Set endian macros for simple exec includes, but not for host utilities */
#define __BYTE_ORDER __BIG_ENDIAN
#endif

/** Enable MIPS32 and OCTEON support which includes MIPS64 */
#define CONFIG_MIPS32			1  /* MIPS32 CPU core	*/
#define CONFIG_OCTEON			1

/** Enable snprintf/vsnprintf support */
#define CONFIG_SYS_VSPRINTF

/**
 * Default CPU reference frequency used if cpu reference clock reading fails.
 */
#ifndef DEFAULT_CPU_REF_FREQUENCY_MHZ
# define DEFAULT_CPU_REF_FREQUENCY_MHZ	50
#endif

/** Used if multiplier read fails, and for DRAM refresh rates */
#ifndef DEFAULT_ECLK_FREQ_MHZ
# define DEFAULT_ECLK_FREQ_MHZ		400
#endif

/** Enable OCTEON Flash support */
#ifndef CONFIG_OCTEON_NAND_BOOT
# define CONFIG_OCTEON_FLASH
#else
# include "config_octeon_nand.h"
#endif

#define	CONFIG_TIMESTAMP		/** Print image info with timestamp */

/** Steps to use when looking for alternative bootloader in FLASH */
#define LOOKUP_STEP			(64*1024)

/** Maximum size of prompt */
#define CONFIG_SYS_MAX_PROMPT_LEN	(64)

#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_64BIT		/** Define this since we're always 64-bit. */
#define CONFIG_64BIT_PHYS_ADDR	/** Define for 64-bit physical addresses */
#define CONFIG_SYS_SUPPORT_64BIT_DATA	/** Supports 64-bit data types */

/** MIPS timer frequency */
#define CONFIG_SYS_MIPS_TIMER_FREQ	(gd->cpu_clk)

/** Address mapping support between virtual and physical addresses */
#define CONFIG_ADDR_MAP
#define CONFIG_SYS_NUM_ADDR_MAP			8
#define CONFIG_ADDR_MAP_CODE_IDX		0
#define CONFIG_ADDR_MAP_FIRST_256MB_IDX		1
#define CONFIG_ADDR_MAP_BOOT_BUS_IDX		2
#define CONFIG_ADDR_MAP_REST_MEM_IDX		3
#define CONFIG_ADDR_MAP_PCI0_BASE		4
#define CONFIG_ADDR_MAP_PCI1_BASE		5
#define CONFIG_ADDR_MAP_PCI2_BASE		6
#define CONFIG_ADDR_MAP_PCI3_BASE		7
#define CONFIG_ADDR_MAP_SECOND_256MB_IDX	8

/** Enable hashing support */
#define CONFIG_OCTEON_SHA1
#define CONFIG_OCTEON_SHA256
#define CONFIG_OCTEON_MD5

#define __HAVE_ARCH_SHA1
#define __HAVE_ARCH_SHA256
#define __HAVE_ARCH_MD5

/** Enable serial driver */
#define CONFIG_OCTEON_SERIAL

/** Enable TWSI (I2C) support */
#define CONFIG_OCTEON_I2C

/** Maximum number of I2C buses on OCTEON */
#ifndef CONFIG_MAX_I2C_NUM
# define CONFIG_MAX_I2C_NUM		2
#endif
#ifndef CONFIG_SYS_MAX_I2C_BUS
# define CONFIG_SYS_MAX_I2C_BUS		1
#endif

/** Support DOS partitions for media */
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

/** Allow command line auto complete */
#define CONFIG_AUTO_COMPLETE

/** Enable BZIP2 compression */
#ifndef CONFIG_BZIP2
# define CONFIG_BZIP2
#endif

/** Enable GZIP/ZIP compression */
#ifndef CONFIG_ZIP
# define CONFIG_ZIP
#endif
#ifndef CONFIG_GZIP
# define CONFIG_GZIP
#endif

/** Enable LZMA compression */
#ifndef CONFIG_LZMA
# define CONFIG_LZMA
#endif

/** Enable zlib support */
#ifndef CONFIG_ZLIB
# define CONFIG_ZLIB
#endif

/**
 * This makes TFTP print out hashes based on the size of the file returned
 * by the server.
 */
#define CONFIG_TFTP_TSIZE

/** valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	\
		{ 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, \
		  1000000, 1152000, 1500000, 2000000, 2500000, 3000000, \
		  3500000, 4000000, 5000000, 10000000 }

/* default baudrate */
#define CONFIG_BAUDRATE			115200

/** default baudrate for downloads */
#define CONFIG_DOWNLOAD_BAUDRATE	115200

/** Don't print out console info on bootup */
#define CONFIG_SYS_CONSOLE_INFO_QUIET	1
#define CONFIG_SILENT_CONSOLE

/** Enable overwrite of previous console environment settings */
#define CONFIG_SYS_CONSOLE_ENV_OVERWRITE

/** Board specific environment callbacks */
#ifndef CONFIG_BOARD_ENV_CALLBACK_LIST
# define CONFIG_BOARD_ENV_CALLBACK_LIST		""
#endif

/** Environment variables with callbacks */
#define CONFIG_ENV_CALLBACK_LIST_STATIC			\
	"console_uart:console_uart,"			\
	"pci_console_active:pci_console_active,"	\
	"watchdog_enable:watchdog_enable,"		\
	"watchdog_timeout:watchdog_timeout,"		\
	"pci_secure_mode:pci_secure_mode,"		\
	CONFIG_BOARD_ENV_CALLBACK_LIST

/** Enable the call to overwrite_console() */
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE

/**
 * If overwrite_console returns 1 then stdin, stdout and stderr are switched
 * to the serial port, otherwise the environment settings are used.
 */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

/** Console is stored in the environment */
#define CONFIG_CONSOLE_IS_IN_ENV

/** We support multiple inputs and outputs */
#define CONFIG_CONSOLE_MUX

#define	CONFIG_SYS_CBSIZE	1024	/** Console I/O Buffer Size   */

/** Print Buffer Size */
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + CONFIG_SYS_MAX_PROMPT_LEN + 16)
#define	CONFIG_SYS_MAXARGS		64	/** max number of command args*/

/** Memory allocated for heap */
#define CONFIG_SYS_MALLOC_LEN		(12*1024*1024)	/** Large for jffs2 */

/** Default load address */
#ifndef CONFIG_SYS_LOAD_ADDR
# define CONFIG_SYS_LOAD_ADDR		0x20000000
#endif

/** Timer frequency - do not change! This MUST be 1000! */
#define CONFIG_SYS_HZ			1000ull

/** Cache line size common in all Octeon processors */
#define CONFIG_SYS_CACHELINE_SIZE	128

/** If set passes "memsize" to Linux in bytes rather than MB */
#define CONFIG_MEMSIZE_IN_BYTES

/** Base of SDRAM */
#define CONFIG_SYS_SDRAM_BASE		0x80000000     /* Cached addr */

/** Default addresses to use for memory test */
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + 0x100000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0xffffff)

/** Enable support for a more comprehensive memory test */
#define CONFIG_SYS_ALT_MEMTEST

/** let the eth address be writeable */
/* #define CONFIG_ENV_OVERWRITE		1*/

/** display details about env callbacks */
#define CONFIG_CMD_ENV_CALLBACK

/** display details about env flags */
#define CONFIG_CMD_ENV_FLAGS

/** Enable regular expressions for some commands */
#define CONFIG_REGEX

/**
 * Use low-level I2C bus controller rather than the high level controller.
 * The high-level controller can read and write a maximum of 8 bytes per
 * call.  The low-level controller does not have this limitation.
 */
#define CONFIG_OCTEON_I2C_LOW_LEVEL

/** Enable I2C (TWSI) support */
#define CONFIG_SYS_I2C

/** Set the TWSI clock to a conservative 100KHz. */
#ifndef CONFIG_SYS_I2C_SPEED
# define CONFIG_SYS_I2C_SPEED		(100000)
#endif

/** We have our own optimized copy of memset/memcpy */
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMCMP
#define __HAVE_ARCH_MEMCPY

/** Use optimized memcpy */
#define CONFIG_USE_ARCH_MEMCPY

/** Use optimized memset */
#define CONFIG_USE_ARCH_MEMSET

/** We have our own optimized copy of crc32 */
#define __HAVE_ARCH_CRC32

/** Size of memory allocated for passing parameters to the Linux kernel */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128*1024)

#ifndef CONFIG_SYS_NO_FLASH
/** Put all common flash definitions together */

/** Remapped base of flash */
# ifndef CONFIG_SYS_FLASH_BASE
#  define CONFIG_SYS_FLASH_BASE		(0x1fc00000 - CONFIG_SYS_FLASH_SIZE)
# endif
# define CONFIG_SYS_FLASH_EMPTY_INFO		1	/** Prints empty secs */
# define CONFIG_SYS_FLASH_USE_BUFFER_WRITE		/** Speeds up writes */
/* # define CONFIG_SYS_CFI_FLASH_STATUS_POLL */		/** Polling support */
# define CONFIG_SYS_FLASH_PROTECTION			/** Protects RO parts */
# define CONFIG_FLASH_CFI_MTD		/** Enable MTD support for CFI flash */
# define CONFIG_MTD_DEVICE		/** Enable MTD support */
# define CONFIG_MTD_PARTITIONS		/** Enable MTD partitions used by Linux */

/** Allow concatination of multiple MTD devices */
# define CONFIG_MTD_CONCAT


/** Timeout for Flash Erase in ticks */
# define CONFIG_SYS_FLASH_ERASE_TOUT	(2 * CONFIG_SYS_HZ)

/** Timeout for Flash Write in ticks */
# define CONFIG_SYS_FLASH_WRITE_TOUT	(2 * CONFIG_SYS_HZ)
#elif defined(CONFIG_OCTEON_NAND_BOOT)
# define CONFIG_MTD_DEVICE	/** Enable MTD support */
# define CONFIG_MTD_PARTITIONS	/** Enable MTD partitions used by Linux */
#endif /* !CONFIG_SYS_NO_FLASH */

/** Set if EEPROM contains OCTEON EEPROM tuples */
#define CONFIG_OCTEON_EEPROM_TUPLES		1

/** Size (in bytes) of the address in the EEPROM.  Should be 2 */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2

/** Location of failsafe bootloader in flash, do not change this */
#define FAILSAFE_BASE				0x1fc00000

/**
 * Enable support for the OCTEON GPIO driver
 */
#define CONFIG_OCTEON_GPIO

/**
 * Which GPIO pin is used to indicate that the failsafe bootloader should be
 * used.
 */
#define CONFIG_OCTEON_FAILSAFE_GPIO		0

/** The following #defines are needed to get flash environment right */
#define	CONFIG_SYS_MONITOR_BASE			CONFIG_SYS_TEXT_BASE

/**
 * Starting address to look for a non-failsafe bootloader, this should not
 * be changed for historical reasons.
 */
#define	CONFIG_SYS_MONITOR_LEN			(192 << 10)

/*
 * Define CONFIG_PCI only if the system is known to provide a PCI
 * clock to Octeon.  A bus error exception will occur if the inactive
 * Octeon PCI core is accessed.  U-boot is not currently configured to
 * recover when a exception occurs.
 */
#if CONFIG_OCTEON_PCI_HOST
# ifndef OCTEON_FIRST_PCIE_BUSNO
	/**
	 * Starting PCIE bus number.  Note that this must be 1 for the IDT
	 * PCIe switch.
	 */
#  define OCTEON_FIRST_PCIE_BUSNO		0
# endif
# define CONFIG_PCI_INDIRECT_BRIDGE
/** Enable PCI support */
# define CONFIG_PCI
# define CONFIG_OCTEON_PCI

/** Enable PCI PnP support (assigning resources) */
# define CONFIG_PCI_PNP				/* Plug 'n Play support */

/** Skip the host bridge configuration */
# define CONFIG_PCIAUTO_SKIP_HOST_BRIDGE

/** Enable 64-bit support where possible */
# define CONFIG_SYS_PCI_64BIT			/* 64 bit support */

/** Enable the 'pci' command */
# define CONFIG_CMD_PCI				/* "pci" command */
#endif

/** Enable FIT images for command scripts */
#define CONFIG_FIT

/** Enable the "ver" environment variable */
#define CONFIG_VERSION_VARIABLE

/** USB stuff */
#if defined(CONFIG_OCTEON_USB_OCTEON) || defined(CONFIG_OCTEON_USB_OCTEON2) \
	|| defined(CONFIG_OCTEON_USB_OCTEON3)
/** Enable "usb" command */
# define CONFIG_CMD_USB

/** Enable USB mass storage support */
# define CONFIG_USB_STORAGE
/**
 * Fixup binary output from some storage devices which causes console
 * to freeze
 */
# define CONFIG_USB_BIN_FIXUP

# ifdef CONFIG_OCTEON_USB_OCTEON2 /** OCTEON II USB support */
/** OCTEON II uses EHCI */
#  define CONFIG_USB_EHCI
/** Enable OCTEON II EHCI support (initializes clocks, etc.) */
#  define CONFIG_USB_EHCI_OCTEON2
/** Maximum number of USB root ports to support */
#  define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	2
/** Descriptors are big endian */
#  define CONFIG_EHCI_DESC_BIG_ENDIAN
# elif defined(CONFIG_OCTEON_USB_OCTEON3)

/** Enable USB XHCI host support */
#  define CONFIG_USB_XHCI

/** Enable OCTEON XHCI USB support */
#  define CONFIG_USB_XHCI_OCTEON

/** Some broken USB devices need extra time to power up */
#  ifndef CONFIG_USB_HUB_MIN_POWER_ON_DELAY
#   define CONFIG_USB_HUB_MIN_POWER_ON_DELAY	1000
#  endif

/** There is only one root port per controller but two controllers */
#   define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS	2

#  ifndef CONFIG_USB_MAX_CONTROLLER_COUNT
#   define CONFIG_USB_MAX_CONTROLLER_COUNT	2
#  endif
# else /* OCTEON I */
#  define CONFIG_USB_OCTEON	/** OCTEON I USB driver support */
# endif
#endif

/**
 * Defines the default timeout in seconds for the watchdog timer.
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

/** Minimum supported UART number */
#ifndef CONFIG_OCTEON_MIN_CONSOLE_UART
# define CONFIG_OCTEON_MIN_CONSOLE_UART		0
#endif

/** Maximum supported UART number */
#ifndef CONFIG_OCTEON_MAX_CONSOLE_UART
# define CONFIG_OCTEON_MAX_CONSOLE_UART		1
#endif

/**
 * Memory mapped by startup code of the Linux kernel where the bd_info,
 * boot arguments, flat device tree and any other data needed by the
 * Linux kernel.
 *
 * This should be the same as OCTEON_RESERVED_LOW_MEM_SIZE.
 */
#define CONFIG_SYS_BOOTMAPSZ			(1*1024*1024)

/** Enable device tree support */
#ifndef CONFIG_OF_LIBFDT
# define CONFIG_OF_LIBFDT
# define CONFIG_OF_CONTROL
#endif
#ifndef CONFIG_LMB
# define CONFIG_LMB
#endif

#ifndef CONFIG_OCTEON_NO_BOOT_BUS
# define CONFIG_OCTEON_BOOT_BUS	/** Enable support for devices on the boot bus */
#endif

/**
 * This enables some additional device tree checks for the NOR flash size
 * and address.  This is a common area where mistakes are made so it's best
 * this is enabled to detect misconfigured device trees.
 *
 * Note that this will slow down U-Boot a bit, especially when running out
 * of flash (i.e. when U-Boot can't copy itself to the L2 cache).
 */
#define CONFIG_OCTEON_ENABLE_DT_FLASH_CHECKS

/**
 * This tells U-Boot to copy itself from flash into the L2 cache very early
 * on to speed up the boot process.  The resulting code in start.S will
 * first check that it is running on an OCTEON II processor and that the
 * L2 cache size is big enough.
 *
 * This can have a very dramatic improvement in bootup time, especially when
 * a lot of memory is installed (i.e. a CN68XX)
 */
#define CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2

/**
 * This tells U-Boot to write a special memcpy routine that copies U-Boot
 * from flash to the L2 cache in the L2 cache.  This speeds up the memcpy
 * significantly though the overall impact to the bootup time is around 1/2
 * a second.
 */
#define CONFIG_OCTEON_L2_MEMCPY_IN_CACHE

/**
 * This specifies the address where U-Boot will be copied to in the L2 cache
 * when running out of flash.
 */
#define OCTEON_L2_UBOOT_ADDR		0xffffffff81000000

/**
 * This specifies the address where U-Boot will write a memcpy routine used
 * to copy itself from flash to the L2 cache.
 */
#define OCTEON_L2_MEMCPY_ADDR		0xffffffff81400000

/**
 * This allows U-Boot to automatically generate the prompt based on the board
 * name.
 */
#define CONFIG_OCTEON_AUTO_PROMPT

/**
 * Maximum number of blocks that can be transferred in an operation for MMC
 */
#define CONFIG_SYS_MMC_MAX_BLK_COUNT	8191

#endif /* __OCTEON_COMMON_H__ */
