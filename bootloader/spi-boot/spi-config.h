/*
 * Copyright 2013-2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* NOTE: Not all features can be enabled due to size limitations.  The maximum
 * size of the binary is 8192 bytes.
 */

#define CONFIG_SPI_FREQUENCY	16000000	/** Frequency to run SPI bus at */

/* Only enable one of the following SPI device types below */

#define CONFIG_SPI_NOR			/** Use a SPI NOR */

/** Enable M95XXX SPI EEPROM support */
/* #undef CONFIG_SPI_EEPROM_M95XXX */

/** How much to increment when searching for the bootloader header */
#define CONFIG_HEADER_SEARCH_INCREMENT		(64)

/** When to stop searching for a bootloader header */
#define CONFIG_HEADER_SEARCH_END		0xe000

/* UART configuration */
#define UART_NODE		0	/** Node for UART with 78XX */
/**
 * UART port to output to
 */
#define UART_PORT		0	/** Output to the first uart. This can be 0 or 1 */

/**
 * Baud rate to use
 */
#define UART_BAUD_RATE		115200	/* Baud rate for the uart */

/**
 * This enables printing out verbose information.  Note that this grows the
 * size of the stage 1 bootloader.
 */
#define VERBOSE_OUTPUT

/**
 * String that is printed out at the very beginning just before executing
 * C code.  This is optional and does not need to be defined.
 */
#define STAGE1_START_STRING	"\r\nOCTEON SPI stage 1 bootloader\r\n"

/**
 * Address where the stage 1 bootloader is loaded by the on-chip ROM
 */
#define STAGE1_DRAM_ADDR	0xFFFFFFFF81000000ull

/**
 * Maximum size of the stage 1 bootloader.  The on-chip ROM only loads
 * the first 8K.
 */
#define STAGE1_BOOTLOADER_SIZE	(8*1024)

/**
 * Address where the stage 2 bootloader will be loaded.  Note that this is
 * not in DRAM but the L2 cache so this address can be changed to any
 * valid address.
 */
#define STAGE2_LOAD_ADDR	0xFFFFFFFF81002000ull

/**
 * Address where the stage 1 bootloader should branch to start the stage 2
 * bootloader.
 */
#define STAGE2_START_ADDR	0xFFFFFFFF81000000ull
/**
 * Maximum size for the stage 2 bootloader.  This can be increased if it is
 * guaranteed that the chip it is running on will have a larger L2 cache.
 * Currently the minimum cache size on all shipping eMMC capable chips is
 * 512K. 8K is reserved for the stage1 bootloader.
 */
#define STAGE2_DRAM_SIZE	((512 - 9) * 1024)

/**
 * Define this in order to validate that the stage 2 bootloader is  not corrupt.
 */
#define STAGE1_VALIDATE_STAGE2_CRC

/**
 * reference clock frequency.
 */
#define REF_CLOCK		50000000	/* 50 MHz */

#endif	/* __CONFIG_H__ */
