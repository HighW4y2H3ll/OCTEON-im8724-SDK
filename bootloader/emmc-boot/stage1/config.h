/*
 * Copyright 2012 - 2013 Cavium, Inc. <support@cavium.com>
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
#ifndef __CONFIG_H__
#define __CONFIG_H__

/* NOTE: Not all features can be enabled due to size limitations.  The maximum
 * size of the binary is 8192 bytes.
 */

/**
 * Failsafe GPIO signal.  If failsafe is selected the FAT filesystem will be
 * bypassed.
 *
 * Do not define this if there is no failsafe GPIO.
 */
/* #define GPIO_FAILSAFE		0 */

/**
 * Whether failsafe is active high or low
 */
#define GPIO_FAILSAFE_LEVEL	1	/* 1 = Active high, 0 = active low */

/**
 * UART port to output to
 */
#define UART_PORT		0	/* Output to the first uart. This can be 0 or 1 */

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
#define STAGE1_START_STRING	"\r\nOCTEON eMMC stage 1 bootloader\r\n"

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
#define STAGE2_DRAM_LOAD_ADDR	0xFFFFFFFF81004000ull
/**
 * Address where the stage 1 bootloader should branch to start the stage 2
 * bootloader.
 */
#define STAGE2_START_ADDR	0xFFFFFFFF81004000ull
/**
 * Maximum size for the stage 2 bootloader.  This can be increased if it is
 * guaranteed that the chip it is running on will have a larger L2 cache.
 * Currently the minimum cache size on all shipping eMMC capable chips is
 * 512K. 8K is reserved for the stage1 bootloader.
 */
#define STAGE2_DRAM_SIZE	((512 - 17) * 1024)

/**
 * File name for the stage 2 bootloader.  The file name must be a valid DOS 8.3
 * filename.  Unlike a standard 8.3 filename no '.' is present.  The first part
 * must be padded out at the end up to a total of 8 characters and the last 3
 * characters must be present at the end.
 *
 *  The file name has the following restrictions:
 * - A-Z, 0-9, - and _ are supported
 * - Must be 11 characters in length
 * - First part must be 8 characters, padded with spaces at the end if necessary
 * - Last 3 digits must be 3 characters, padded with spaces at the end.
 *
 * For example, if the file name is supposed to be "boot.bin" then the
 * file name defined would be "BOOT    BIN".
 */
#define STAGE_2_FILENAME	"OCTBOOT2BIN"

/**
 * Backup filename in case the stage 2 filename is corrupt or not found.
 */
#define STAGE_2_BACKUP_FILENAME	"OCTBOOT2BAK"

/**
 * Environment file to pass to bootloader
 *
 * NOTE: The stage 2 bootloader currently does not support this.
 */
/* #define STAGE2_ENV_FILENAME	"OCTBOOT2ENV" */

/**
 * Location in RAM where the environment should be loaded.
 */
#define RAM_ENV_ADDR (U_BOOT_RAM_ENV_ADDR | 0xffffffff80002000ull)

/**
 * Validates the CRC32 in the environment file.
 */
#undef VALIDATE_ENV_CRC
/**
 * Define this in order to validate that the stage 2 bootloader is  not corrupt.
 */
#define STAGE1_VALIDATE_STAGE2_CRC

/**
 * Maximum supported width of MMC card and bus.
 * NOTE: If 8 doesn't work try 4 or 1.
 */
#define MMC_MAX_WIDTH		8

/**
 * Send command to MMC to initialize the block length.  Some devices don't like
 * this.
 */
#undef MMC_SET_BLOCKLEN

/**
 * Defined if the stage 1 bootloader can reset the MMC bus before starting
 * the stage 2 bootloader.  This is not needed with U-Boot.
 */
#define STAGE1_RESET_MMC

/**
 * If we fail to load from FAT, try directly reading the MMC device following
 * the stage 1 bootloader.
 */
#define STAGE_2_LAST_FAILSAFE

/**
 * If we fail to load either stage 2 bootloader from FAT try loading starting
 * at this sector.  Make sure that this area of the eMMC is not partitioned.
 */
#define STAGE_2_LAST_FAILSAFE_START_SECTOR	16

/**
 * Maximum number of times to retry reading the stage 2 bootloader from
 * the FAT partition
 */
#define MAX_FAT_RETRIES				16

/**
 * reference clock frequency.
 */
#define REF_CLOCK		50000000	/* 50 MHz */

/**
 * Minimum MMC bus speed
 */
#define MMC_MIN_SPEED		400000		/* 400KHz */

/**
 * Maximum MMC bus speed
 */
#define MMC_MAX_SPEED		20000000	/* 20MHz */

#endif	/* __CONFIG_H__ */
