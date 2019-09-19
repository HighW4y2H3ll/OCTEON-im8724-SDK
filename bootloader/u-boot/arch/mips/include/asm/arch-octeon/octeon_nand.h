/***********************license start************************************
 * Copyright (c) 2008, 2010 Cavium Inc. (support@cavium.com). All rights
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
 *     * Neither the name of Cavium Inc. nor the names of
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
 * marketing@cavium.com
 *
 ***********************license end**************************************/

/* Header file for exported functions from cmd_oct_nand.c */

#ifndef __OCTEON_NAND_H__
#define __OCTEON_NAND_H__

/**
 * Return codes for NAND functions
 */
typedef enum {
	OCT_NAND_SUCCESS = 0,
	OCT_NAND_ERROR = -1,
	OCT_NAND_BAD_HCRC = -2,
	OCT_NAND_BAD_DCRC = -3,
	OCT_NAND_ECC_UNCOR = -4,
	OCT_NAND_READ_ERROR = -5,
	OCT_NAND_WRITE_ERROR = -6,
	OCT_NAND_BAD_MAGIC = -7,
	OCT_NAND_BAD_PARAM = -8,
} oct_nand_status_t;

/**
 * Probes boot bus for NAND chips and populates cvmx-nand internal
 * structures.
 *
 * @return active chip on success
 *          -1 on error
 */
int oct_nand_probe (void);

/**
 * Return the currently active NAND chip.
 *
 * @return Currently active NAND chip select.
 */
int oct_nand_get_cur_chip (void);

/**
 * Sets the currently active NAND chip.
 *
 * @return Currently active NAND chip select.
 */
int oct_nand_set_cur_chip (int chip_select);

/* Flags for oct_nand_boot_write */
/* Currently no flags */
/**
 * Writes to NAND using Octeon boot ECC.
 *
 * @param nand_addr NAND address to write to.  Must be page aligned.
 * @param data      Pointer to data to write in memory.
 * @param len       Number of bytes to write
 * @param flags     Flags to write function.
 *
 * @return 0 on success
 *         <0 on failure
 */
oct_nand_status_t oct_nand_boot_write (uint64_t nand_addr, void *data, int len,
				       uint32_t flags);

/**
 * Validates the header that is at nand_addr, and returns the
 * size in bytes.
 *
 * @param nand_addr NAND address of image to validate.
 *
 * @return >0 : Size of valid image in bytes.
 *         For invalid images: oct_nand_status_t
 */
int oct_nand_validate_image (uint64_t nand_addr);

/**
 * Copies data from NAND, doing Octeon NAND boot ECC correction.
 *
 * @param dest_addr Destination in DRAM to write data to
 * @param nand_addr NAND address to read from.
 * @param len       Length in bytes to read.
 *
 * @return Number of bytes read (== len) on success
 *         Negative oct_nand_status_t
 */
int oct_nand_copy_from_boot_nand (void *dest_addr, uint64_t nand_addr, int len);

/**
 * Scans NAND for a valid image with a bootloader header.  Returns the address
 * that the image is located at on success, or 0 on failure.
 * Only valid images are returned.
 *
 * @param start_addr NAND address to start searching from.
 * @param max_addr   Address at which search stops.
 * @param image_type Type of image to find.  Value of 0 returns all valid
 *                   images.
 *
 * @return Address of valid image on success
 *         0 if none found.
 */
uint64_t oct_nand_image_search (uint64_t start_addr, uint64_t max_addr,
				uint16_t image_type);

/**
 * Checks to see if a give block is blank.
 *
 * @param nand_addr Address of block to check for blankness.  Must be block
 *		    aligned.
 *
 * @return 1 if blank
 *         0 if not blank
 */
int oct_nand_block_is_blank (uint64_t nand_addr);
/**
 * Checks to see if a given page is blank.
 *
 * @param nand_addr Address of page to check for blankness.  Must be page
 *                  aligned.
 *
 * @return 1 if blank
 *         0 if not blank
 */
int oct_nand_page_is_blank (uint64_t nand_addr);

/**
 * Checks to see if a given block is bad.
 *
 * @param chip_select NAND chip select to operation on.
 * @param nand_addr NAND address to check for being bad.  Any address within
 *                  a block may be passed.
 * @param check_octeon_ecc
 *                  Checks for valid Octeon boot ECC, and does not do normal
 *                  bad block checks for pages that have valid Octeon boot ECC.
 *                  (Octeon NAND boot ECC does not follow normal bad block
 *                  marking conventions, and this format is used by
 *                  the hardware.)
 *
 * @return 1 if bad
 *         0 if not bad
 */
int oct_nand_block_is_bad (int chip_select, uint64_t nand_addr,
			   int check_octeon_ecc);

/**
 * Return the number of bytes of OOB datat that u-boot is going to use.
 * We limit this to a multiple of 8 bytes to match the Octeon NAND DMA
 * restrictions.
 * The size returned by this call can be used directly with other cvmx_nand
 * functions without having to check the size.
 *
 * @param nand_chip NAND chip to get info on
 *
 * @return OOB size in bytes.
 */
int octeon_nand_get_oob_size (int nand_chip);

/**
 * Adjust a chip/partition size down for bad blocks so we don't
 * read/write past the end of a chip/partition by accident.
 *
 * @param[inout]	size	number of bytes to check
 * @param		offset	starting offset to read from
 * @param		dev	NAND device number
 */
void octeon_nand_adjust_size_for_badblocks(loff_t *size, loff_t offset,
					   int dev);

/**
 * Read data from a NAND device and skip over bad blocks.
 *
 * @param	dev		NAND device number to read from
 * @param	start		starting offset to read from, must be on a page boundary
 * @param	size		size of data to read into buffer
 * @param	max_size	Maximum number of bytes to read including bad
 * @param[out]	buffer		buffer to read data into
 * @param[out]	next		address of next page to read taking into account
 *				bad blocks.
 *
 * @return	0 for success, -1 on error
 */
int octeon_nand_read_dev(int dev, loff_t start, loff_t size, loff_t max_size,
			 void *buffer, loff_t *next);

/**
 * Returns if a page is blank or not
 *
 * @param	dev	NAND device to check
 * @param	offset	offset of page to check
 *
 * @return 0 if page is not blank or 1 if it is blank
 */
int octeon_nand_is_page_blank(int dev, uint64_t offset);

/**
 * Like octeon_nand_is_page_blank but instead returns if a block is blank or not
 *
 * @param	dev	NAND device to check
 * @param	offset	Offset of erase block to check
 *
 * @return 0 if block is not blank or 1 if it is blank
 */
int octeon_nand_is_block_blank(int dev, uint64_t offset);

#ifdef CONFIG_OCTEON_HW_BCH
/**
 * Scans the NAND flash for a valid bootloader header
 *
 * @param	dev	NAND device number
 * @param	count	indicate which matching bootloader, 0 for first match,
 * 			1 for second match, etc.
 * @param	board_type	board type to match against or -1 for any
 * @param	image_type	type of image to search for or 0 for all valid.
 * @param[out]	image_size	size of image on NAND
 * @param	start		start offset in NAND to search
 * @param	end		end offset in NAND to search
 *
 * @return	nand address of beginning of bootloader or -1 on error
 */
int64_t octeon_nand_image_search(int dev, int count, int board_type,
				 uint16_t image_type, uint32_t *image_size,
				 uint64_t start, uint64_t end);

/**
 * Validate a bootloader image at the specified NAND address
 *
 * @param	dev		nand device to read from
 * @param	nand_addr	starting address of bootloader in NAND
 * @param	end		end address of bootloader in NAND
 *
 * @return	0 for a valid image, invalid otherwise, -1 on error
 */
int octeon_nand_validate_image(int dev, loff_t nand_addr, loff_t end);
#endif	/* CONFIG_OCTEON_HW_BCH */

#endif	/* __OCTEON_NAND_H__ */
