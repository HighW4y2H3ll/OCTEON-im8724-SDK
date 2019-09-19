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

#include <common.h>
#include <command.h>

#ifdef CONFIG_CMD_OCTEON_NAND

#include <watchdog.h>
#include <malloc.h>
#include <nand.h>
#include <asm/byteorder.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_nand.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/octeon_mem_map.h>

#ifdef DEBUG
#define debug_printf    printf
#else
#define debug_printf(...)
#endif

#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-nand.h>
#include <asm/arch/cvmx-bootloader.h>
/* Special command for dealing with Octeon specific NAND boot */

static int chip = -1;		/* Current chip selected */

static unsigned char *oct_get_buf_storage(void)
{
	static unsigned char *buf_storage = NULL;

	if (!buf_storage) {
		char tmp_name[64];

		snprintf(tmp_name, sizeof(tmp_name), "%s_nand_dma_buf",
			 OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX);
		buf_storage =
			(u8 *)cvmx_bootmem_alloc_named_range_once(CVMX_NAND_MAX_PAGE_AND_OOB_SIZE,
								  0x10000,
								  0x7fffffff,
								  8, tmp_name,
								  NULL);
		if (!buf_storage)
			printf("Error allocating named buffer storage %s\n",
			       tmp_name);
	}
	return buf_storage;
}

int oct_nand_get_cur_chip(void)
{
	return chip;
}

int oct_nand_set_cur_chip(int chip_select)
{
	chip = chip_select;
	return chip;
}

/* Copies from OCTEON boot ECC protected memory (doing ECC decode) to a supplied
 * memory buffer
 */
int oct_nand_copy_from_boot_nand(void *dest_addr, u64 nand_addr, int len)
{
	int bytes_remaining = len;
	int bytes;
	int read_size = CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
	    CVMX_NAND_BOOT_ECC_ECC_SIZE;
	unsigned char *buf = oct_get_buf_storage();
	int errors;
	int page_size = cvmx_nand_get_page_size(chip);

	int crc_skip = nand_addr & (CVMX_NAND_BOOT_ECC_BLOCK_SIZE - 1);
	/* Start reading at an aligned address */
	nand_addr &= ~(CVMX_NAND_BOOT_ECC_BLOCK_SIZE - 1);
	if (!buf) {
		printf("%s(0x%p, 0x%llx, 0x%x): cannot allocate buffer\n",
		       __func__, dest_addr, nand_addr, len);
		return OCT_NAND_ERROR;
	}

	for (; bytes_remaining > 0; nand_addr += CVMX_NAND_BOOT_ECC_BLOCK_SIZE) {
		/* Adjust the read address based on location in page */
		int offset = ((nand_addr & (page_size - 1)) /
			      CVMX_NAND_BOOT_ECC_BLOCK_SIZE)
		    * CVMX_NAND_BOOT_ECC_ECC_SIZE;

		bytes = cvmx_nand_page_read(chip, nand_addr + offset,
					    cvmx_ptr_to_phys(buf), read_size);
		if (bytes != read_size) {
			return OCT_NAND_READ_ERROR;
		}
		errors = cvmx_nand_correct_boot_ecc(buf);
		if (errors <= 1) {
			int copy_size = MIN(CVMX_NAND_BOOT_ECC_BLOCK_SIZE -
					    crc_skip,
					    bytes_remaining);
			memcpy(dest_addr, buf + crc_skip, copy_size);
			bytes_remaining -= CVMX_NAND_BOOT_ECC_BLOCK_SIZE -
			    crc_skip;
			dest_addr += copy_size;
			crc_skip = 0;
		} else
			return OCT_NAND_ECC_UNCOR;
	}

	return len;
}

/**
 * Checks to see if address is part of a bad block.
 * This does the 'standard' bad block checks, but also takes
 * into account the Octeon NAND boot ECC format.  Most blocks
 * with this format will appear to be bad, so we do extra checks
 * for blocks that fail the standard bad block test.  If a 'bad' block
 * passes Octeon ECC, then it is treated as good.
 * One complication is that some bad blocks are all 0x00 bytes, which
 * also happens to be a valid Octeon ECC encoding.  This special case is
 * checked for, and treated as a bad block.
 *
 * @param chip_select
 *                  Chip select for operation
 * @param nand_addr Address of block to check.  (Any address within block is
 *                  OK.)
 * @param check_octeon_ecc
 *                  Do extra checks for Octeon ECC.  Without these checks, many
 *                  Octeon NAND boot blocks will be reported as bad.
 *
 * @return 1 if bad
 *         0 if OK
 *	   -1 on error
 */
int oct_nand_block_is_bad(int chip_select, uint64_t nand_addr,
			  int check_octeon_ecc)
{
	/* Cache the results of the most recent check,
	 * so that we can answer quickly about all pages in
	 * a block.
	 */
	static u64 cached_block_addr = ~0ull;
	static int cached_block_is_bad = 0;

	unsigned char *buf = oct_get_buf_storage();
	int page_size = cvmx_nand_get_page_size(chip_select);
	int pages_per_block = cvmx_nand_get_pages_per_block(chip_select);
	int block_size = page_size * pages_per_block;
	int read_size = page_size + octeon_nand_get_oob_size(chip_select);
	int block_is_bad = 0;
	int bytes;
	int i;
	int page_to_check;
	u64 nand_addr_read;

	/* Set the bad block position */
	int bad_block_pos = page_size + (page_size > 512 ?
					 NAND_LARGE_BADBLOCK_POS :
					 NAND_SMALL_BADBLOCK_POS);

	if (!buf) {
		printf("%s(%d, 0x%llx, %d): cannot allocate buffer\n",
		       __func__, chip_select, nand_addr, check_octeon_ecc);
		return OCT_NAND_ERROR;
	}

	/* Since NAND is marked bad in terms of blocks, we only
	 * care about what block the supplied address is in
	 */
	nand_addr &= ~(block_size - 1);

	/* Return cached value if we have a match */
	if (nand_addr == cached_block_addr)
		return cached_block_is_bad;

	nand_addr_read = nand_addr;
	for (page_to_check = 0; page_to_check < 2; page_to_check++) {
		/* Check first and last pages of block */
		bytes = cvmx_nand_page_read(chip_select, nand_addr_read,
					    cvmx_ptr_to_phys(buf), read_size);
		if (bytes != read_size) {
			printf("ERROR: %s: error reading NAND.\n",
			       __FUNCTION__);
			return 1;
		}
		if (buf[bad_block_pos] != 0xFF)
			block_is_bad = 1;

		if (block_is_bad) {
			if (check_octeon_ecc) {
				/* Check to see if there is a valid Octeon ECC
				 * encoded block at the beginning of the page.
				 * If so, the block is good, otherwise it is
				 * bad
				 */
				if (!cvmx_nand_correct_boot_ecc(buf)) {
					/* No ECC errors detected */
					/* Some bad blocks are set to all 0x00,
					 * including OOB.  This meets all bad
					 * block marking requirements, but is
					 * also a valid Octeon BOOT bus
					 * encoding.  Treat this as a bad block.
					 */
					for (i = 0; i < read_size; i++)
						if (buf[i])
							break;
					if (i == read_size) {
						block_is_bad = 1;
						goto page_is_bad_exit;
					}
					block_is_bad = 0;
				}
			} else
				goto page_is_bad_exit;
		}
		nand_addr_read += block_size - page_size;
	}

page_is_bad_exit:
	/* Cache most recent lookup so looking up pages in same block is fast */
	cached_block_addr = nand_addr;
	cached_block_is_bad = block_is_bad;
	return block_is_bad;
}

/* Checks to see if the page (including OOB) at given address is blank
 * returns: 1 if blank
 *          0 if !blank
 *	    -1 on error
 */
int oct_nand_page_is_blank(u64 nand_addr)
{
	int chip = oct_nand_get_cur_chip();
	unsigned char *buf = oct_get_buf_storage();
	int page_size = cvmx_nand_get_page_size(chip);
	int read_size = page_size + octeon_nand_get_oob_size(chip);
	int bytes;
	int i;

	if (!buf) {
		printf("%s(0x%llx): cannot allocate buffer\n",
		       __func__, nand_addr);
		return OCT_NAND_ERROR;
	}


	if (nand_addr & (page_size - 1))
		printf("WARNING: non page aligned address passed to %s\n",
		       __FUNCTION__);

	nand_addr &= ~(page_size - 1);

	/* Check to see if the page/block is bad.
	 * Treat bad blocks as non-blank, so they won't get written to
	 */
	if (oct_nand_block_is_bad(chip, nand_addr, 1)) {
		printf("page at addr 0x%llx is bad, returning !blank\n",
		       nand_addr);
		return 0;
	}

	bytes = cvmx_nand_page_read(chip, nand_addr, cvmx_ptr_to_phys(buf),
				    read_size);
	if (bytes != read_size) {
		printf("ERROR: %s: error reading NAND.\n", __FUNCTION__);
		return 0;
	}
	for (i = 0; i < read_size; i++) {
		if (buf[i] != 0xff)
			return (0);
	}
	return 1;
}

/* Checks to see if the block (including OOB) at given address is blank
 * returns: 1 if blank
 *          0 if !blank
 */
int oct_nand_block_is_blank(u64 nand_addr)
{
	int page_size = cvmx_nand_get_page_size(chip);
	int pages_per_block = cvmx_nand_get_pages_per_block(chip);
	int block_size = page_size * pages_per_block;
	int page;

	if (nand_addr & (block_size - 1)) {
		printf("ERROR: %s: nand_addr is not block aligned.\n",
		       __func__);
		return 0;
	}
	for (page = 0; page < pages_per_block; page++) {
		if (!oct_nand_page_is_blank(nand_addr + page_size * page))
			return 0;
	}
	return 1;
}

void dump_hex(unsigned char *buffer)
{
	int i;
	for (i = 0; i < 256; i++) {
		printf("%02x ", buffer[i]);
		if (i % 16 == 15)
			printf("\n");
	}
}

/* Writes to NAND using Octeon NAND-boot ECC */
oct_nand_status_t oct_nand_boot_write(u64 nand_addr, void *data, int len,
				      u32 flags)
{
	u64 tmp_addr;
	int input_remaining;
	int output_remaining;
	int chip = oct_nand_get_cur_chip();
	unsigned char *buf = oct_get_buf_storage();
	int page_size = cvmx_nand_get_page_size(chip);
	int oob_size = octeon_nand_get_oob_size(chip);
	int ecc_result = 0;
	int offset;		/* offset within NAND page while computing ECC
				 * (may limit size of final write)
				 */

	if (nand_addr & (page_size - 1)) {
		printf("ERROR: write start must be page aligned.\n");
		return OCT_NAND_BAD_PARAM;
	}

	if (!buf)
		return OCT_NAND_ERROR;

	debug("NAND chip %d page size: %d, OOB size: %d\n",
	      chip, page_size, oob_size);

	/* Round length up to page size */
	len = (len + page_size - 1) & ~(page_size - 1);

	/* First check to make sure all pages are erased.  We don't count the
	 * OOB data against the length supplied, as that is where the ECC data
	 * will go.  BAD blocks are returned as !blank, so we will also skip
	 * bad blocks
	 */
	for (tmp_addr = nand_addr; tmp_addr < nand_addr + len;
	     tmp_addr += page_size) {
		if (oct_nand_block_is_bad(chip, tmp_addr, 1)) {
			printf("ERROR: Block containing addr 0x%llx is bad, "
			       "unable to write to page\n", tmp_addr);
			return OCT_NAND_ERROR;
		}
		if (!oct_nand_page_is_blank(tmp_addr)) {
			printf("ERROR: page at addr 0x%llx is not blank, "
			       "unable to write to page\n", tmp_addr);
			return OCT_NAND_ERROR;
		}
	}

	/* Now we have verified that the required flash is erased (and not bad),
	 * now generate the ECC and write the data to the flash.
	 */
	input_remaining = len;
	output_remaining = len;
	for (tmp_addr = nand_addr; tmp_addr < nand_addr + len;
	     tmp_addr += page_size) {
		u64 buf1_storage[CVMX_NAND_MAX_PAGE_AND_OOB_SIZE / 8];
		unsigned char *buf1 = (unsigned char *)buf1_storage;

		memset(buf, 0, sizeof(buf));
		offset = 0;
		while (input_remaining > 0 &&
		       offset <= (page_size + oob_size -
				  CVMX_NAND_BOOT_ECC_BLOCK_SIZE -
				  CVMX_NAND_BOOT_ECC_ECC_SIZE)) {
			memcpy(buf + offset, data,
			       MIN(CVMX_NAND_BOOT_ECC_BLOCK_SIZE,
				   input_remaining));
			cvmx_nand_compute_boot_ecc(buf + offset, buf + offset +
						   CVMX_NAND_BOOT_ECC_BLOCK_SIZE);
			offset += CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
			    CVMX_NAND_BOOT_ECC_ECC_SIZE;
			data += CVMX_NAND_BOOT_ECC_BLOCK_SIZE;
			input_remaining -= CVMX_NAND_BOOT_ECC_BLOCK_SIZE;
		}
		debug("Writing buffer at 0x%p (0x%llx) to NAND chip %d, address 0x%llx\n",
		      buf, cvmx_ptr_to_phys(buf), chip, tmp_addr);
		/* We have filled up a page, so write it */
		if (cvmx_nand_page_write(chip, tmp_addr, cvmx_ptr_to_phys(buf))) {
			printf("cvmx_nand_page_write() failed, address "
			       "0x%08llx\n", tmp_addr);
			return OCT_NAND_WRITE_ERROR;
		}

		udelay(10);
		debug("Reading buffer back to 0x%p (0x%llx), length %d\n",
		      buf1, cvmx_ptr_to_phys(buf1), offset);
		/* Now read the block to make sure it matches */
		if (cvmx_nand_page_read(chip, tmp_addr,
					cvmx_ptr_to_phys(buf1),
					offset) != offset) {
			printf("ERROR reading back NAND to verify write.\n");
			return OCT_NAND_READ_ERROR;
		}
		/* Perform error correction */
		offset = 0;
		while ((output_remaining > 0) &&
		       (offset <= (page_size + oob_size -
				  CVMX_NAND_BOOT_ECC_BLOCK_SIZE -
				  CVMX_NAND_BOOT_ECC_ECC_SIZE)) &&
		       (ecc_result <= 1)) {
			ecc_result = cvmx_nand_correct_boot_ecc(buf1 + offset);
			if (ecc_result == 1)
				debug("Corrected one bit error in %d byte ECC "
				      "block at offset %u, NAND offset 0x%llx\n",
				      CVMX_NAND_BOOT_ECC_BLOCK_SIZE, offset,
				      tmp_addr + offset);
			offset += CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
			    CVMX_NAND_BOOT_ECC_ECC_SIZE;
			output_remaining -= CVMX_NAND_BOOT_ECC_BLOCK_SIZE;
		}

		if (ecc_result == 2) {
			printf("Too many ECC errors in NAND flash in %u byte "
			       "block starting at page offset 0x%llx, "
			       "NAND offset 0x%llx\n",
			       CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
			       CVMX_NAND_BOOT_ECC_ECC_SIZE,
			       tmp_addr, tmp_addr + offset);
			return OCT_NAND_WRITE_ERROR;
		}

		if (memcmp(buf, buf1, offset)) {
			int i;
			for (i = 0; i < offset; i++) {
				if (buf[i] != buf1[i]) {
					printf("Error offset at 0x%llx (%lld), page offset %d\n",
					       tmp_addr + offset + i,
					       tmp_addr + offset + i, i);
					printf("Expected 0x%02x, read back 0x%02x\n",
					       buf[i], buf1[i]);
				}
			}
		}
	}
	return OCT_NAND_SUCCESS;
}

/* Validates the header that is at nand_addr, and returns the
** size in bytes.  If invalid:
** -1 : wrong magic
** -2 : header CRC incorrect
** -3 : data CRC incorrect
** -4 : uncorrectable ECC error
** -5 : NAND read error
*/
int oct_nand_validate_image(u64 nand_addr)
{
	int errors = 0;
	int read_size = CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
	    CVMX_NAND_BOOT_ECC_ECC_SIZE;
	int chip = oct_nand_get_cur_chip();
	int bytes;
	unsigned char *buf = oct_get_buf_storage();
	const bootloader_header_t *header = (const bootloader_header_t *)buf;
	u32 dcrc;
	u32 crc;
	int page_size = cvmx_nand_get_page_size(chip);
	int data_length;
	int crc_bytes_remaining;
	int crc_skip;

	if (!buf) {
		printf("%s(0x%llx): Could not allocate buffer\n", __func__,
		       nand_addr);
		return OCT_NAND_ERROR;
	}
	/* Read first block, which should contain header */

	bytes = cvmx_nand_page_read(chip, nand_addr, cvmx_ptr_to_phys(buf),
				    read_size);

	if (cvmx_nand_correct_boot_ecc(buf) > 1)
		return OCT_NAND_ECC_UNCOR;

	if ((header->magic != BOOTLOADER_HEADER_MAGIC))
		return OCT_NAND_BAD_MAGIC;

	crc = crc32(0, (void *)header, offsetof(bootloader_header_t, hcrc));
	crc = crc32(crc, (void *)header + offsetof(bootloader_header_t, hlen),
		    header->hlen - offsetof(bootloader_header_t, hlen));
	if (crc != header->hcrc)
		return OCT_NAND_BAD_HCRC;

	/* Save data crc for later use */
	dcrc = header->dcrc;

	/* Data starts at nand_addr + header->hlen */
	nand_addr += header->hlen;

	/* We need to always read CVMX_NAND_BOOT_ECC_BLOCK_SIZE size blocks so
	 * that we can do ECC.
	 * The data may start at an offset into this first block
	 */
	data_length = crc_bytes_remaining = header->dlen;

	crc_skip = nand_addr & (CVMX_NAND_BOOT_ECC_BLOCK_SIZE - 1);
	/* Start reading at an aligned address */
	nand_addr &= ~(CVMX_NAND_BOOT_ECC_BLOCK_SIZE - 1);

	crc = 0;
	for (; crc_bytes_remaining > 0; nand_addr +=
	     CVMX_NAND_BOOT_ECC_BLOCK_SIZE) {
		/* Adjust the read address based on location in page */
		int offset = ((nand_addr & (page_size -
					    1)) /
			      CVMX_NAND_BOOT_ECC_BLOCK_SIZE) *
		    CVMX_NAND_BOOT_ECC_ECC_SIZE;

		bytes = cvmx_nand_page_read(chip, nand_addr + offset,
					    cvmx_ptr_to_phys(buf), read_size);
		if (bytes != read_size) {
			return OCT_NAND_READ_ERROR;
		}
		errors = cvmx_nand_correct_boot_ecc(buf);
		if (errors <= 1) {
			if (errors == 1)
				debug("Correctable ECC error at NAND addr: 0x%llx\n",
				      nand_addr + offset);
			crc = crc32(crc, (void *)buf + crc_skip,
				    MIN(CVMX_NAND_BOOT_ECC_BLOCK_SIZE -
					crc_skip, crc_bytes_remaining));
			crc_bytes_remaining -=
			    CVMX_NAND_BOOT_ECC_BLOCK_SIZE - crc_skip;
			if (crc_skip)
				crc_skip = 0;
		} else {
			debug("Uncorrectable ECC error at NAND "
			      "addr: 0x%llx\n", nand_addr + offset);
			return OCT_NAND_ECC_UNCOR;
		}
	}

	if (dcrc != crc)
		return OCT_NAND_BAD_DCRC;

	return data_length;
}

static int oct_nand_dump(uint64_t start_addr, uint64_t len)
{
	unsigned char *buf = oct_get_buf_storage();
	uint64_t page, end;
	int page_size = cvmx_nand_get_page_size(chip);
	int oob_size = cvmx_nand_get_oob_size(chip);

	if (!buf) {
		printf("%s(0x%llx, 0x%llx): cannot allocate buffer\n",
		       __func__, start_addr, len);
		return OCT_NAND_ERROR;
	}
	/* Convert from addresses to pages */
	page = start_addr / page_size;
	if (!len)
		end = page;
	else
		end = (start_addr + len) / page_size;

	while (page <= end) {
		/* Read the next block */
		int i;
		int read_size = page_size + oob_size;
		int bytes;
		int pages_per_block;
		/* Read more than required in cases when total read size
		 * is not a multiple of 8.
		 */
		read_size = (read_size + 7) & ~0x7;

		bytes = cvmx_nand_page_read(chip, page_size * page,
						cvmx_ptr_to_phys(buf),
						read_size);
		pages_per_block = cvmx_nand_get_pages_per_block(chip);
		if (bytes != read_size) {
			printf("Error reading page %llu, bytes read: %d\n",
			       page, bytes);
				return 1;
		}
		/* Dump page data */
		printf("Address 0x%llx, (Block 0x%llx, page 0x%llx) data:\n",
		       page * page_size, page / pages_per_block,
		       page % pages_per_block);

		for (i = 0; i < page_size; i++) {
			if (i % 16 == 0)
				printf("0x%04llx:", page * page_size + i);
			printf(" %02x", buf[i]);
			if (i % 16 == 15)
				printf("\n");
		}

		/* Dump OOB data */
		printf("Address 0x%llx, (Block 0x%llx, page 0x%llx) OOB:\n",
		       page * page_size, page / pages_per_block,
		       page % pages_per_block);
		for (i = 0; i < oob_size; i++) {
			if (i % 16 == 0)
				printf("0x%04x:", i);
			printf(" %02x", buf[page_size + i]);
			if (i % 16 == 15)
				printf("\n");
		}
		printf("\n");

		page++;
	}
	return 0;
}

static int oct_nand_scan(void)
{
	u64 nand_addr;
	bootloader_header_t header_copy;
	unsigned char *buf = oct_get_buf_storage();
	int page_size = cvmx_nand_get_page_size(chip);
	int pages_per_block = cvmx_nand_get_pages_per_block(chip);
	int block_size = page_size * pages_per_block;
	int errors = 0;
	int read_size = CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
		    CVMX_NAND_BOOT_ECC_ECC_SIZE;

	if (!buf) {
		printf("%s(): cannot allocate buffer\n", __func__);
		return OCT_NAND_ERROR;
	}
	if (!page_size) {
		printf("ERROR: No NAND configured (run probe)\n");
		return 1;
	}

	printf("Scanning NAND for recognized images\n");
	for (nand_addr = 0; nand_addr < MAX_NAND_SEARCH_ADDR;
	     nand_addr += page_size) {
		/* Adjust the read address based on location in page */
		int offset;
		int bytes;

		offset = ((nand_addr & (page_size - 1)) /
				CVMX_NAND_BOOT_ECC_BLOCK_SIZE) *
			  CVMX_NAND_BOOT_ECC_ECC_SIZE;

		bytes = cvmx_nand_page_read(chip, nand_addr + offset,
					    cvmx_ptr_to_phys(buf), read_size);
		if (bytes != read_size) {
			printf("Error reading NAND addr 0x%08llx "
			       "(bytes_read: %d, expected: %d)\n",
			       nand_addr, bytes, read_size);
			return 1;
		}

		errors = cvmx_nand_correct_boot_ecc(buf);
		if (errors <= 1) {
			const bootloader_header_t *header =
			    (const bootloader_header_t *)buf;

			/* Block is good, see if it contains a
			 * bootloader image header
			 */
			if ((header->magic == BOOTLOADER_HEADER_MAGIC)) {
				int rval;

				/* Check the CRC of the header */
				u32 crc;
				crc = crc32(0, (void *)header, 12);
				crc = crc32(crc, (void *)header + 16,
					    header->hlen - 16);

				if (crc != header->hcrc) {
					printf("ERROR: 0x%llx Header CRC "
					       "mismatch, expected: 0x%08x, "
					       "found: 0x%08x\n",
					       nand_addr, header->hcrc, crc);
				} else {
					header_copy = *header;
					printf("\nImage header at addr: 0x%llx, "
					       "size: 0x%x, crc: 0x%x ",
					       nand_addr, header->dlen,
					       header->dcrc);
					rval = oct_nand_validate_image(nand_addr);
					if (rval < 0) {
						printf("Invalid image(%d).\n",
						       rval);
					} else {
						printf("Valid image.\n");
						if (header_copy.flags &
						    BL_HEADER_FLAG_FAILSAFE)
							printf("Failsafe");
						else
							printf("Normal");
						if (header_copy.image_type ==
						    BL_HEADER_IMAGE_STAGE2)
							printf("NAND stage2 ");
						else if
						    (header_copy.image_type ==
						     BL_HEADER_IMAGE_STAGE3)
							printf("NAND stage3 ");
						else if
						    (header_copy.image_type ==
						     BL_HEADER_IMAGE_UBOOT_ENV)
							printf("U-boot environment (version: %d)\n",
							       (int)header_copy.address);

						if (header_copy.board_type)
							printf("image for %s board\n",
							       cvmx_board_type_to_string(header_copy.board_type));
						else
							printf("\n");
						/* Skip the image, as we
						 * have validated it
						 */
						nand_addr +=
						    ((header->dlen + page_size - 1) & ~(page_size - 1)) - page_size;
					}
				}
			}
		} else {
			/* Uncorrectable errors detected. */

			/* Check to see if we have a bad block */
			if (oct_nand_block_is_bad(chip, nand_addr, 1)) {
				printf("Bad NAND block at addr: 0x%llx\n",
				       nand_addr & ~(block_size - 1));
				/* Skip rest of block */
				nand_addr += block_size -
				    (nand_addr & (block_size - 1)) - page_size;
			}
		}
	}
	printf("\n");
	return 0;
}

int do_oct_nand(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int rc;

	const char *cmd;

	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (!strcmp("probe", cmd)) {
		rc = oct_nand_probe();
		if (rc < 0) {
			printf("NAND flash not found\n");
		} else {
			printf("probe found NAND flash on boot bus %d.\n", rc);
		}
		return 0;
	} else if (!strcmp("dump", cmd)) {
		/* Only dump full pages.... */
		u64 start_addr, len;
		int page_size = cvmx_nand_get_page_size(chip);

		if (argc < 3)
			goto usage;

		start_addr = simple_strtoull(argv[2], NULL, 16);
		if (argc == 4)
			len = simple_strtoull(argv[3], NULL, 16);
		else
			len = 0;

		if (!page_size) {
			printf("ERROR: No NAND configured (run probe)\n");
			return 1;
		}

		return oct_nand_dump(start_addr, len);
	} else if (!strcmp("erase", cmd)) {
		u64 start_addr, length;
		int force = 0;
		u64 i;
		int page_size = cvmx_nand_get_page_size(chip);
		int pages_per_block = cvmx_nand_get_pages_per_block(chip);
		if (argc < 3 || argc > 5)
			goto usage;
		start_addr = simple_strtoull(argv[2], NULL, 16);
		if (argc >= 4)
			length = simple_strtoull(argv[3], NULL, 16);
		else
			length = page_size * pages_per_block;

		if (argc == 5) {
			if (strcmp(argv[4], "force"))
				goto usage;
			printf("WARNING: Forced erase: erasing bad blocks.\n");
			force = 1;
		}
		if (!page_size) {
			printf("ERROR: No NAND configured (run probe)\n");
			return 1;
		}

		if (start_addr & ((u64) page_size * pages_per_block - 1)) {
			printf("ERROR: erase start not at block boundary "
			       "(maybe want: 0x%llx or 0x%llx)\n",
			       start_addr & ~((u64) page_size *
					      pages_per_block - 1),
			       (start_addr +
				page_size *
				pages_per_block) & ~((u64) page_size *
						     pages_per_block - 1));
			return 1;
		}
		if ((length & (page_size * pages_per_block - 1))) {
			printf("ERROR: erase length not multiple of block size "
			       "(maybe want: 0x%llx)\n",
			       (length + (page_size * pages_per_block - 1)) &
			       ~(page_size * pages_per_block - 1));
			return 1;
		}

		for (i = start_addr; i < start_addr + length;
		     i += page_size * pages_per_block) {
			if (oct_nand_block_is_bad(chip, i, 1)) {
				if (force) {
					printf("WARNING: Erasing bad bock at "
					       "address 0x%llx\n", i);
				} else {
					printf("Not erasing bad block at "
					       "address 0x%llx\n", i);
					continue;
				}
			}
			if (cvmx_nand_block_erase(chip, i)) {
				printf("cvmx_nand_block_erase() failed, "
				       "addr 0x%08llx\n", i);
				return 1;
			}
		}

		return 0;
	} else if (!strcmp("write", cmd)) {
		u64 nand_addr;
		u32 dram_addr, length;
		int page_size = cvmx_nand_get_page_size(chip);
		unsigned char *data_ptr;

		if (!page_size) {
			printf("ERROR: No NAND configured (run probe)\n");
			return 1;
		}
		if (argc != 3 && argc != 5)
			goto usage;

		nand_addr = simple_strtoull(argv[2], NULL, 16);
		if (argc == 5) {
			dram_addr = simple_strtoul(argv[3], NULL, 16);
			length = simple_strtoul(argv[4], NULL, 16);
		} else {
			/* We look up the length/address from environment
			 * variables....
			 * filesize=3410
			 * fileaddr=20000000
			 */
			if (getenv("filesize") && getenv("fileaddr")) {
				dram_addr = simple_strtoul(getenv("fileaddr"),
							   NULL, 16);
				length = simple_strtoul(getenv("filesize"),
							NULL, 16);

				/* Round length up to pagesize */
				length =
				    (length + page_size - 1) & ~(page_size - 1);
			} else {
				printf("ERROR: filesize and fileaddr "
				       "environment variables\n"
				       "must be set if address and size "
				       "are not given\n");
				goto usage;
			}
		}
		data_ptr = (void *)dram_addr;

		if (nand_addr & (page_size - 1)) {
			printf("ERROR: write address not at page boundary "
			       "(maybe want: 0x%08llx)\n",
			       nand_addr & ~(page_size - 1));
			return 1;
		}

		return oct_nand_boot_write(nand_addr, data_ptr, length, 0);
	} else if (!strcmp("scan", cmd)) {
		return oct_nand_scan();
	}

usage:
	cmd_usage(cmdtp);
	return 1;
}

/* This function is used to scan for the environment before the serial port
 * is initialized.  In order to enable these debug prints the 'env_init'
 * function must be called after the serial port is configured.
 */
#if 1
#define special_debug_printf(...)
#else
#define special_debug_printf    printf
#endif

#ifndef CONFIG_OCTEON_HW_BCH
/* Look for an image of a given type in the NAND address range given.  If an
 * image type of 0 is passed, matches all valid images
 */
u64 oct_nand_image_search(u64 start_addr, u64 max_addr, u16 image_type)
{
	unsigned char *buf = oct_get_buf_storage();
	int page_size;
	int chip = oct_nand_get_cur_chip();
	u64 nand_addr;
	bootloader_header_t header_copy;
	int errors;
	int read_size;

	if (chip < 0) {
		special_debug_printf("No nand chips found\n");
		return 0;
	}
	if (!buf) {
		printf("%s(0x%llx, 0x%llx, 0x%x): cannot allocate buffer\n",
		       __func__, start_addr, max_addr, image_type);
		return (unsigned long long)-1;
	}


	page_size = cvmx_nand_get_page_size(chip);

	/* Round start address up to a page boundary */
	start_addr = (start_addr + page_size - 1) & ~(page_size - 1);

	special_debug_printf("Using NAND chip %d, page size: %d\n", chip,
			     page_size);
	errors = 0;
	read_size = CVMX_NAND_BOOT_ECC_BLOCK_SIZE +
	    CVMX_NAND_BOOT_ECC_ECC_SIZE;
	if (!page_size) {
		special_debug_printf("ERROR: No NAND configured (run probe)\n");
		return 0;
	}
	special_debug_printf("Scanning NAND for for image type: %d\n",
			     image_type);
	for (nand_addr = start_addr; nand_addr < max_addr;
	     nand_addr += page_size) {
		/* Adjust the read address based on location in page */
		int offset = ((nand_addr & (page_size - 1)) /
			      CVMX_NAND_BOOT_ECC_BLOCK_SIZE) *
		    CVMX_NAND_BOOT_ECC_ECC_SIZE;
		int bytes = cvmx_nand_page_read(chip, nand_addr + offset,
						cvmx_ptr_to_phys(buf),
						read_size);
		if (bytes != read_size) {
			special_debug_printf("Error reading NAND addr %d "
					     "(bytes_read: %d, expected: %d)\n",
					     nand_addr, bytes, read_size);
			return -1;
		}
		errors = cvmx_nand_correct_boot_ecc(buf);
		if (errors <= 1) {
			const bootloader_header_t *header;
			if (errors == 1)
				special_debug_printf("Correctable ECC error at "
						     "NAND address: 0x%llx\n",
						     nand_addr + offset);

			header = (const bootloader_header_t *)buf;

			/* Block is good, see if it contains a bootloader image
			 * header
			 */
			if ((header->magic == BOOTLOADER_HEADER_MAGIC)) {
				u32 crc;
				special_debug_printf("\nFound a header at addr 0x%llx\n",
						     nand_addr);

				/* Check the CRC of the header */
				crc = crc32(0, (void *)header, 12);
				crc =
				    crc32(crc, (void *)header + 16,
					  header->hlen - 16);

				if (crc == header->hcrc) {
					/* Copy header, as validate_image() call
					 * will overwrite global buffer
					 */
					header_copy = *header;
					if (oct_nand_validate_image(nand_addr) >= 0) {
						if (!image_type
						    || header_copy.image_type == image_type) {
							/* We found a valid
							 * image of the type we
							 * were looking for, so
							 * return the address of it.
							 */
							special_debug_printf("Image type match at addr 0x%llx\n",
									     nand_addr);
							return nand_addr;
						}
						/* Skip the image, as we have
						 * validated it
						 */
						special_debug_printf("\nSkipping image type: %d\n",
								     header_copy.image_type);
						nand_addr +=
						    ((header_copy.hlen +
						      header_copy.dlen +
						      page_size -
						      1) & ~(page_size - 1)) -
						    page_size;
					} else {
						special_debug_printf("Image CRC failed\n");
					}
				} else {
					special_debug_printf("Header CRC failed\n");
				}
			}
		} else {
			special_debug_printf("Uncorrectable ECC error at NAND "
					     "address: 0x%llx\n",
					     nand_addr + offset);
		}
	}

	special_debug_printf("Done looking for image type: %d, none found.\n",
			     image_type);
	return 0;

}
#endif

U_BOOT_CMD(octnand, 5, 0, do_oct_nand,
	   "NAND operations related to NAND boot",
	   "probe - probes for NAND on Octeon boot bus\n"
	   "erase <nand addr> [length] [force] - erase blocks of NAND. addr must be block\n"
	   "                                     aligned\n"
	   "                                     force must be set to erase blocks\n"
	   "                                     that are bad\n"
	   "scan - scans for recognized images in NAND\n"
	   "write <nand dest addr> [<dram src addr> <length in bytes>]\n"
	   "      - Adds ECC encoding and writes to NAND\n"
	   "        Uses fileaddr and filesize env. variable if only 1 arg\n"
	   "dump <nand addr> <length in bytes> - dumps raw nand DATA\n"
	   "                                     (always full pages)\n"
	   "\n"
	   "This command only writes to contiguous sections of good blocks, as\n"
	   "the first stage of NAND boot does not support skipping of bad blocks.\n"
	   "\n");

#if defined(CONFIG_ENV_IS_IN_NAND)
int eraseenv(int exclude_latest);
int do_eraseenv (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nand_erase_options_t nand_erase_options;

	memset(&nand_erase_options, 0, sizeof(nand_erase_options));
	nand_erase_options.length = CONFIG_ENV_RANGE;
	nand_erase_options.offset = CONFIG_ENV_OFFSET;
	if (CONFIG_ENV_RANGE < CONFIG_ENV_SIZE) {
		puts("Range too small for environment size\n");
		return -1;
	}
	if (nand_erase_opts(&nand_info[0], &nand_erase_options)) {
		puts("Error erasing NAND environment.\n");
		return -1;
	}
	return 0;
}

U_BOOT_CMD(
	eraseenv, 1, 0,	do_eraseenv,
	"eraseenv - erase environment variables from persistent storage\n",
	NULL
);

#endif

#endif
