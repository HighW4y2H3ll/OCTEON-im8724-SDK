/***********************license start************************************
 * Copyright (c) 2008-2014  Cavium Inc. (support@cavium.com).
 * All rights reserved.
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
 ***********************license end**************************************/

#include <common.h>
#include <command.h>
#include <linux/stddef.h>
#include <flash.h>

#ifdef CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE
# include <watchdog.h>
# include <malloc.h>
# include <asm/byteorder.h>
# include <asm/addrspace.h>
# include <asm/arch/octeon_boot.h>
# ifdef CONFIG_CMD_OCTEON_NAND
#  include <asm/arch/octeon_nand.h>
# endif
#endif
#ifdef CONFIG_OCTEON_SPI_BOOT_END
# include "spi_flash.h"
#endif
#include <asm/arch/cvmx.h>
#ifdef CONFIG_CMD_OCTEON_NAND
# include <asm/arch/cvmx-nand.h>
#endif
#include <asm/arch/cvmx-bootloader.h>
#ifdef CONFIG_OCTEON_HW_BCH
# include <nand.h>
# include <linux/mtd/nand.h>
# ifndef CONFIG_OCTEON_NAND_BOOT_DEV
#  define CONFIG_OCTEON_NAND_BOOT_DEV		0
# endif
# ifndef CONFIG_OCTEON_NAND_BOOT_START
#  define CONFIG_OCTEON_NAND_BOOT_START		0
# endif
#endif
#if defined(CONFIG_OCTEON_MMC)
# include <part.h>
# include <fat.h>
#endif

#ifdef CONFIG_OCTEON_MMC
#define DOS_PART_DISKSIG_OFFSET	0x1b8
#define DOS_PART_TBL_OFFSET	0x1be
#define DOS_PART_MAGIC_OFFSET	0x1fe
#define DOS_PBR_FSTYPE_OFFSET	0x36
#define DOS_PBR32_FSTYPE_OFFSET	0x52
#define DOS_PBR_MEDIA_TYPE_OFFSET	0x15
#define DOS_MBR	0
#define DOS_PBR	1

typedef struct dos_partition {
	unsigned char boot_ind;		/* 0x80 - active			*/
	unsigned char head;		/* starting head			*/
	unsigned char sector;		/* starting sector			*/
	unsigned char cyl;		/* starting cylinder			*/
	unsigned char sys_ind;		/* What partition type			*/
	unsigned char end_head;		/* end head				*/
	unsigned char end_sector;	/* end sector				*/
	unsigned char end_cyl;		/* end cylinder				*/
	unsigned char start4[4];	/* starting sector counting from 0	*/
	unsigned char size4[4];		/* nr of sectors in partition		*/
} dos_partition_t;
# ifndef CONFIG_OCTEON_STAGE3_DEVNAME
#  define CONFIG_OCTEON_STAGE3_DEVNAME		"mmc"
# endif
# ifndef CONFIG_OCTEON_STAGE3_DEVNO
#  define CONFIG_OCTEON_STAGE3_DEVNO		0
# endif
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_OCTEON_SPI_BOOT_END
/**
 * Data structure used when analyzing the SPI NOR for bootloader images and
 * free space.
 */
struct bootloader_map {
	uint32_t offset;	/** Offset into SPI NOR */
	size_t size;		/** Size of image in SPI NOR including padding */
	size_t image_size;	/** Size of image in SPI NOR (no padding) */
	bootloader_image_t type;/** Type of bootloader image */
	bool free_space;	/** Block is free space (not necessarily erased) */
};
#endif

extern ulong load_addr;

/*
 * Helpers
 */
uint32_t calculate_header_crc(const bootloader_header_t * header);
uint32_t calculate_image_crc(const bootloader_header_t * header);

#ifdef CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE
uint32_t get_image_size(const bootloader_header_t * header);
int validate_header(const bootloader_header_t * header);
int validate_data(const bootloader_header_t * header);

#if !defined(CONFIG_SYS_NO_FLASH)
extern flash_info_t flash_info[];
#endif

#ifdef DEBUG
#define DBGUPD printf
#else
#define DBGUPD(_args...)
#endif

/* This needs to be overridden for some boards, but this is correct for most.
 * This is only used for dealing with headerless images (and headerless failsafe
 * images currently on boards.)
 */
#ifndef CONFIG_SYS_NORMAL_BOOTLOADER_BASE
# define CONFIG_SYS_NORMAL_BOOTLOADER_BASE 0x1fd00000
#endif
/**
 * Prepares the update of the bootloader in NOR flash.  Performs any
 * NOR specific checks (block alignment, etc). Patches up the image for
 * relocation and prepares the environment for bootloader_flash_update command.
 *
 * @param image_addr Address of image in DRAM.  May not have a bootloader header
 *                   in it.
 * @param length     Length of image in bytes if the image does not have a header.
 *                   0 if image has a hader.
 * @param burn_addr  Address in flash (not remapped) to programm the image to
 * @param failsafe   Flag to allow failsafe image burning.
 *
 * @return 0 on success
 *         1 on failure
 */
#define ERR_ON_OLD_BASE
int do_bootloader_update_nor(uint32_t image_addr, int length,
			     uint32_t burn_addr, int failsafe)
{
#if defined(CONFIG_SYS_NO_FLASH)
	printf("ERROR: Bootloader not compiled with NOR flash support\n");
	return 1;
#else
	uint32_t failsafe_size, failsafe_top_remapped;
	uint32_t burn_addr_remapped, image_size, normal_top_remapped;
	flash_info_t *info;
	char tmp[16] __attribute__ ((unused));	/* to hold 32 bit numbers in hex */
	int sector = 0;
	bootloader_header_t *header;
	int rc;

	header = cvmx_phys_to_ptr(image_addr);

	DBGUPD("%s(0x%x, 0x%x, 0x%x, %s)\n", __func__, image_addr, length,
	       burn_addr, failsafe ? "failsafe" : "normal");
	DBGUPD("LOOKUP_STEP                0x%x\n", LOOKUP_STEP);
	DBGUPD("CFG_FLASH_BASE             0x%x\n", CONFIG_SYS_FLASH_BASE);

	/* File with rev 1.1 headers are not relocatable, so _must_ be burned
	 * at the address that they are linked at.
	 */
	if (header->maj_rev == 1 && header->min_rev == 1) {
		if (burn_addr && burn_addr != header->address) {
			printf("ERROR: specified address (0x%x) does not match "
			       "required burn address (0x%llx\n)\n",
			       burn_addr, header->address);
			return 1;
		}
		burn_addr = header->address;
	}

	/* If we have at least one bank of non-zero size, we have some NOR */
	if (!flash_info[0].size) {
		puts("ERROR: No NOR Flash detected on board, can't burn NOR "
		     "bootloader image\n");
		return 1;
	}

	/* check the burn address allignement */
	if ((burn_addr & (LOOKUP_STEP - 1)) != 0) {
		printf("Cannot programm normal image at 0x%x: address must be\n"
		       " 0x%x bytes alligned for normal boot lookup\n",
		       burn_addr, LOOKUP_STEP);
		return 1;
	}

	/* for failsage checks are easy */
	if (failsafe && (burn_addr != FAILSAFE_BASE)) {
		printf("ERROR: Failsafe image must be burned to address 0x%x\n",
		       FAILSAFE_BASE);
		return 1;
	}

	if (burn_addr && (burn_addr < FAILSAFE_BASE)) {
		printf("ERROR: burn address 0x%x out of boot range\n",
		       burn_addr);
		return 1;
	}

	if (!failsafe) {
#ifndef CONFIG_OCTEON_NO_FAILSAFE
		/* find out where failsafe ends */
		failsafe_size = get_image_size((bootloader_header_t *)
					       CONFIG_SYS_FLASH_BASE);
		debug("%s: failsafe size @%08x: 0x%x\n", __func__,
		      CONFIG_SYS_FLASH_BASE, failsafe_size);
		if (failsafe_size == 0) {
			/* failsafe does not have header - assume fixed size
			 * old image
			 */
			puts("Failsafe has no valid header, assuming old image. "
			     "Using default failsafe size\n");
			failsafe_size =
			    CONFIG_SYS_NORMAL_BOOTLOADER_BASE - FAILSAFE_BASE;

			/* must default to CONFIG_SYS_NORMAL_BOOTLOADER_BASE */
			if (!burn_addr)
				burn_addr = CONFIG_SYS_NORMAL_BOOTLOADER_BASE;
			else if (CONFIG_SYS_NORMAL_BOOTLOADER_BASE != burn_addr) {
				printf("WARNING: old failsafe image will not be able to start\n"
				       "image at any address but 0x%x\n",
				       CONFIG_SYS_NORMAL_BOOTLOADER_BASE);
#ifdef ERR_ON_OLD_BASE
				return 1;
#endif
			}
		}		/* old failsafe */
#else
		debug("%s: Failsafe support disabled\n", __func__);
		failsafe_size = 0;
#endif		/* CONFIG_OCTEON_NO_FAILSAFE */

		DBGUPD("failsafe size is 0x%x\n", failsafe_size);
		DBGUPD("%s: burn address: 0x%x\n", __func__, burn_addr);
		/* Locate the next flash sector */
		failsafe_top_remapped = CONFIG_SYS_FLASH_BASE + failsafe_size;
		DBGUPD("failsafe_top_remapped 0x%x\n", failsafe_top_remapped);
		info = &flash_info[0];	/* no need to look into any other banks */
		/* scan flash bank sectors */
		for (sector = 0; sector < info->sector_count; ++sector) {
			DBGUPD("%d: 0x%lx\n", sector, info->start[sector]);
			if (failsafe_top_remapped <= info->start[sector])
				break;
		}

		if (sector == info->sector_count) {
			puts("Failsafe takes all the flash??  Can not burn normal image\n");
			return 1;
		}

		/* Move failsafe top up to the sector boundary */
		failsafe_top_remapped = info->start[sector];

		DBGUPD("Found next sector after failsafe is at remapped addr 0x%x\n",
		       failsafe_top_remapped);
		failsafe_size = failsafe_top_remapped - CONFIG_SYS_FLASH_BASE;
		DBGUPD("Alligned up failsafe size is 0x%x\n", failsafe_size);

		/* default to the first sector after the failsafe */
		if (!burn_addr) {
			burn_addr = FAILSAFE_BASE + failsafe_size;
			DBGUPD("Setting burn address to 0x%x, failsafe size: 0x%x\n",
			       burn_addr, failsafe_size);
		/* check for overlap */
		} else if (FAILSAFE_BASE + failsafe_size > burn_addr) {
			puts("ERROR: can not burn: image overlaps with failsafe\n");
			printf("burn address is 0x%x, in-flash failsafe top is 0x%x\n",
			       burn_addr, FAILSAFE_BASE + failsafe_size);
			return 1;
		}
		/* done with failsafe checks */
	}

	if (length)
		image_size = length;
	else
		image_size = get_image_size((bootloader_header_t *)image_addr);
	if (!image_size) {
		/* this is wierd case. Should never happen with good image */
		printf("ERROR: image has size field set to 0??\n");
		return 1;
	}

	/* finally check the burn address' CKSSEG limit */
	if ((burn_addr + image_size) >= (uint64_t) CKSSEG) {
		puts("ERROR: can not burn: image exceeds KSEG1 area\n");
		printf("burnadr is 0x%x, top is 0x%x\n", burn_addr,
		       burn_addr + image_size);
		return 1;
	}
	DBGUPD("burn_addr: 0x%x, image_size: 0x%x\n", burn_addr, image_size);
	/* Look up the last sector to use by the new image */
	burn_addr_remapped = burn_addr - FAILSAFE_BASE + CONFIG_SYS_FLASH_BASE;
	DBGUPD("burn_addr_remapped 0x%x\n", burn_addr_remapped);
	normal_top_remapped = burn_addr_remapped + image_size;
	/* continue flash scan - now for normal image top */
	if (failsafe)
		sector = 0;	/* is failsafe, we start from first sector here */
	for (; sector < info->sector_count; ++sector) {
		DBGUPD("%d: 0x%lx\n", sector, info->start[sector]);
		if (normal_top_remapped <= info->start[sector])
			break;
	}
	if (sector == info->sector_count) {
		puts("ERROR: not enough room in flash bank for the image??\n");
		return 1;
	}
	/* align up for environment variable set up */
	normal_top_remapped = info->start[sector];

	DBGUPD("normal_top_remapped 0x%x\n", normal_top_remapped);
	/* if there is no header (length != 0) - check burn address and
	 * give warning
	 */
	if (length && CONFIG_SYS_NORMAL_BOOTLOADER_BASE != burn_addr) {
#ifdef ERR_ON_OLD_BASE
		puts("ERROR: burning headerless image at other that defailt address\n"
		     "Image look up will not work.\n");
		printf("Default burn address: 0x%x requested burn address: 0x%x\n",
		       CONFIG_SYS_NORMAL_BOOTLOADER_BASE, burn_addr);
		return 1;
#else
		puts("WARNING: burning headerless image at other that defailt address\n"
		     "Image look up will not work.\n");
		printf("Default burn address: 0x%x requested burn address: 0x%x\n",
		       CONFIG_SYS_NORMAL_BOOTLOADER_BASE, burn_addr);
#endif
	}

	printf("Image at 0x%x is ready for burning\n", image_addr);
	printf("           Header version: %d.%d\n", header->maj_rev,
	       header->min_rev);
	printf("           Header size %d, data size %d\n", header->hlen,
	       header->dlen);
	printf("           Header crc 0x%x, data crc 0x%x\n", header->hcrc,
	       header->dcrc);
	printf("           Image link address is 0x%llx\n", header->address);
	printf("           Image burn address on flash is 0x%x\n", burn_addr);
	printf("           Image size on flash 0x%x\n",
	       normal_top_remapped - burn_addr_remapped);

	DBGUPD("burn_addr_remapped 0x%x normal_top_remapped 0x%x\n",
	       burn_addr_remapped, normal_top_remapped);
	if (flash_sect_protect(0, burn_addr_remapped, normal_top_remapped - 1)) {
		puts("Flash unprotect failed\n");
		return 1;
	}
	if (flash_sect_erase(burn_addr_remapped, normal_top_remapped - 1)) {
		puts("Flash erase failed\n");
		return 1;
	}

	puts("Copy to Flash... ");
	/* Note: Here we copy more than we should - whatever is after the image
	 * in memory gets copied to flash.
	 */
	rc = flash_write((char *)image_addr, burn_addr_remapped,
			 normal_top_remapped - burn_addr_remapped);
	if (rc != 0) {
		flash_perror(rc);
		return 1;
	}
	puts("done\n");

#ifdef CONFIG_ENV_IS_IN_FLASH
	/* Erase the environment so that older bootloader will use its default
	 * environment.  This will ensure that the default
	 * 'bootloader_flash_update' macro is there.  HOWEVER, this is only
	 * useful if a legacy sized failsafe u-boot image is present.
	 * If a new larger failsafe is present, then that macro will be incorrect
	 * and will erase part of the failsafe.
	 * The 1.9.0 u-boot needs to have its link address and
	 * normal_bootloader_size/base modified to work with this...
	 */
	if (header->maj_rev == 1 && header->min_rev == 1) {
		puts("Erasing environment due to u-boot downgrade.\n");
		flash_sect_protect(0, CONFIG_ENV_ADDR,
				   CONFIG_ENV_ADDR + CONFIG_ENV_SIZE - 1);
		if (flash_sect_erase(CONFIG_ENV_ADDR,
				     CONFIG_ENV_ADDR + CONFIG_ENV_SIZE - 1)) {
			puts("Environment erase failed\n");
			return 1;
		}

	}
#endif
	return 0;
#endif
}

#if defined(CONFIG_CMD_OCTEON_NAND) && !defined(CONFIG_OCTEON_HW_BCH)
/**
 * NAND specific update routine.  Handles erasing the previous
 * image if it exists.
 *
 * @param image_addr Address of image in DRAM.  Always
 *                   has an image header.
 *
 * @return 0 on success
 *         1 on failure
 */
int do_bootloader_update_nand(uint32_t image_addr)
{
	const bootloader_header_t *new_header;
	const bootloader_header_t *header;
	int chip = oct_nand_get_cur_chip();
	int page_size = cvmx_nand_get_page_size(chip);
	int oob_size = octeon_nand_get_oob_size(chip);
	int pages_per_block = cvmx_nand_get_pages_per_block(chip);
	int bytes;

	uint64_t block_size = page_size * pages_per_block;

	uint64_t nand_addr = block_size;
	uint64_t buf_storage[2200 / 8] = { 0 };
	unsigned char *buf = (unsigned char *)buf_storage;
	int read_size = CVMX_NAND_BOOT_ECC_BLOCK_SIZE + 8;
	uint64_t old_image_nand_addr = 0;
	int old_image_size = 0;
	int required_len;
	int required_blocks;
	int conseq_blank_blocks;
	uint64_t erase_base;
	uint64_t erase_end;
	header = (void *)buf;
	new_header = (void *)image_addr;

	if (!cvmx_nand_get_active_chips()) {
		puts("ERROR: No NAND Flash detected on board, can't burn "
		     "NAND bootloader image\n");
		return 1;
	}

	/* Find matching type (failsafe/normal, stage2/stage3) of image that
	 * is currently in NAND, if present.  Save location for later erasing
	 */
	while ((nand_addr =
		oct_nand_image_search(nand_addr, MAX_NAND_SEARCH_ADDR,
				      new_header->image_type))) {
		/* Read new header */
		bytes =
		    cvmx_nand_page_read(chip, nand_addr, cvmx_ptr_to_phys(buf),
					read_size);
		if (bytes != read_size) {
			printf("Error reading NAND addr 0x%llx (bytes_read: %d, expected: %d)\n",
			       nand_addr, bytes, read_size);
			return 1;
		}
		/* Check a few more fields from the headers */

		if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_GENERIC
		    && header->board_type != CVMX_BOARD_TYPE_GENERIC) {
			/* If the board type of the running image is generic,
			 * don't do any board matching.  When looking for images
			 * in NAND to overwrite, treat generic board type images
			 * as matching all board types.
			 */
			if (new_header->board_type != header->board_type) {
				puts("WARNING: A bootloader for a different "
				     "board type was found and skipped (not erased.)\n");
				/* Different board type, so skip (this is
				 * strange to find.....
				 */
				nand_addr +=
				    ((header->hlen + header->dlen + page_size - 1)
				    & ~(page_size - 1));
				continue;
			}
		}
		if ((new_header->flags & BL_HEADER_FLAG_FAILSAFE) !=
		    (new_header->flags & BL_HEADER_FLAG_FAILSAFE)) {
			/* Not a match, so skip */
			nand_addr +=
			    ((header->hlen + header->dlen + page_size -
			      1) & ~(page_size - 1));
			continue;
		}

		/* A match, so break out */
		old_image_nand_addr = nand_addr;
		old_image_size = header->hlen + header->dlen;
		printf("Found existing bootloader image of same type at NAND addr: 0x%llx\n",
		       old_image_nand_addr);
		break;
	}
	/* nand_addr is either 0 (no image found), or has the address of the
	 * image we will delete after the write of the new image.
	 */
	if (!nand_addr)
		puts("No existing matching bootloader found in flash\n");

	/* Find a blank set of _blocks_ to put the new image in.  We want
	 * to make sure that we don't put any part of it in a block with
	 * something else, as we want to be able to erase it later.
	 */
	required_len = new_header->hlen + new_header->dlen;
	required_blocks = (required_len + block_size - 1) / block_size;

	conseq_blank_blocks = 0;
	read_size = page_size + oob_size;
	for (nand_addr = block_size; nand_addr < MAX_NAND_SEARCH_ADDR;
	     nand_addr += block_size) {
		if (oct_nand_block_is_blank(nand_addr)) {
			conseq_blank_blocks++;
			if (conseq_blank_blocks == required_blocks) {
				/* We have a large enough blank spot */
				nand_addr -=
				    (conseq_blank_blocks - 1) * block_size;
				break;
			}
		} else
			conseq_blank_blocks = 0;
	}

	if (nand_addr >= MAX_NAND_SEARCH_ADDR) {
		puts("ERROR: unable to find blank space for new bootloader\n");
		return 1;
	}
	printf("New bootloader image will be written at blank address 0x%llx, length 0x%x\n",
	       nand_addr, required_len);

	/* Write the new bootloader to blank location. */
	if (0 > oct_nand_boot_write(nand_addr, (void *)image_addr, required_len, 0)) {
		puts("ERROR: error while writing new image to flash.\n");
		return 1;
	}

	/* Now erase the old bootloader of the same type.
	 * We know these are not bad NAND blocks since they have valid data
	 * in them.
	 */
	erase_base = old_image_nand_addr & ~(block_size - 1);
	erase_end =
	    ((old_image_nand_addr + old_image_size + block_size -
	      1) & ~(block_size - 1));
	for (nand_addr = erase_base; nand_addr < erase_end;
	     nand_addr += block_size) {
		if (cvmx_nand_block_erase(chip, nand_addr)) {
			printf("cvmx_nand_block_erase() failed, addr 0x%08llx\n",
			       nand_addr);
			return 1;
		}

	}

	puts("Bootloader update in NAND complete.\n");
	return 0;
}

#elif defined(CONFIG_OCTEON_HW_BCH) && defined(CONFIG_OCTEON_NAND_BOOT_END)

/**
 * NAND specific update routine.  Handles erasing the previous
 * image if it exists.  Note that this uses the standard U-Boot NAND
 * facilities which make use of the hardware BCH engine.
 *
 * @param image_addr Address of image in DRAM.  Always
 *                   has an image header.
 *
 * @return 0 on success
 *         1 on failure
 */
int do_bootloader_update_nand(uint32_t image_addr)
{
	const int dev = CONFIG_OCTEON_NAND_BOOT_DEV;
	const loff_t end = CONFIG_OCTEON_NAND_BOOT_END;
	const bootloader_header_t *new_header = (void *)image_addr;
	nand_info_t *nand = &nand_info[CONFIG_OCTEON_NAND_BOOT_DEV];
	int page_size = nand->writesize;
	int pages_per_block = nand->erasesize / nand->writesize;
	size_t bytes_written = 0;
	uint64_t block_size = page_size * pages_per_block;
	loff_t last_image_addr = 0;
	loff_t nand_addr = block_size;
	uint64_t buf_storage[(NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE) / 8] = { 0 };
	unsigned char *buf = (unsigned char *)buf_storage;
	const bootloader_header_t *header = (void *)buf;
	uint64_t old_image_nand_addr = 0;
	int old_image_size = 0;
	int image_no = 0;
	size_t write_length;
	int rc;
	int required_len;
	int required_blocks;
	int conseq_blank_blocks;
	loff_t erase_base, erase_size;

	if (!cvmx_nand_get_active_chips()) {
		puts("ERROR: No NAND Flash detected on board, can't burn "
		     "NAND bootloader image\n");
		return 1;
	}

	/* Find matching type (failsafe/normal, stage2/stage3) of image that
	 * is currently in NAND, if present.  Save location for later erasing
	 */
	while ((nand_addr = octeon_nand_image_search(dev,
						     image_no,
						     -1,
						     new_header->image_type,
						     &bytes_written, nand_addr,
						     end)) > 0) {
		rc = octeon_nand_validate_image(dev, nand_addr, end);
		if (rc) {
			printf("Found bad NAND image at offset 0x%llx\n",
			       nand_addr);
			nand_addr += page_size;
			continue;
		} else {
			printf("Found good NAND image at offset 0x%llx\n",
			       nand_addr);
		}
		last_image_addr = nand_addr;
		/* Read new header */
		rc = octeon_nand_read_dev(CONFIG_OCTEON_NAND_BOOT_DEV,
					  nand_addr, page_size,
					  end - nand_addr,
					  (void *)buf, &nand_addr);
		if (rc) {
			printf("Error reading NAND addr 0x%llx\n",
			       nand_addr);
			return 1;
		}

		/* Check a few more fields from the headers */
		if (gd->board_type != CVMX_BOARD_TYPE_GENERIC
		    && header->board_type != CVMX_BOARD_TYPE_GENERIC) {
			/* If the board type of the running image is generic,
			 * don't do any board matching.  When looking for images
			 * in NAND to overwrite, treat generic board type images
			 * as matching all board types.
			 */
			if (new_header->board_type != header->board_type) {
				puts("WARNING: A bootloader for a different "
				     "board type was found and skipped (not erased.)\n");
				/* Different board type, so skip (this is
				 * strange to find.....
				 */
				nand_addr +=
				    ((header->hlen + header->dlen + page_size - 1)
				    & ~(page_size - 1));
				continue;
			}
		}
		if ((new_header->flags & BL_HEADER_FLAG_FAILSAFE) !=
		    (new_header->flags & BL_HEADER_FLAG_FAILSAFE)) {
			/* Not a match, so skip */
			nand_addr +=
			    ((header->hlen + header->dlen + page_size -
			      1) & ~(page_size - 1));
			continue;
		}

		/* A match, so break out */
		old_image_nand_addr = last_image_addr;
		old_image_size = header->hlen + header->dlen;
		printf("Found existing bootloader image of the same type at NAND address 0x%llx\n",
		       old_image_nand_addr);
		break;
	}
	/* nand_addr is either 0 (no image found), or has the address of the
	 * image we will delete after the write of the new image.
	 */
	if (!last_image_addr)
		puts("No existing matching bootloader found in flash\n");

	/* Find a blank set of _blocks_ to put the new image in.  We want
	 * to make sure that we don't put any part of it in a block with
	 * something else, as we want to be able to erase it later.
	 */
	required_len = new_header->hlen + new_header->dlen;
	required_blocks = (required_len + block_size - 1) / block_size;

	conseq_blank_blocks = 0;
	for (nand_addr = block_size; nand_addr < end; nand_addr += block_size) {
		if (octeon_nand_is_block_blank(dev, nand_addr)) {
			debug("NAND address 0x%llx is blank\n", nand_addr);
			conseq_blank_blocks++;
			if (conseq_blank_blocks == required_blocks) {
				/* We have a large enough blank spot */
				debug("Found %d consecutive blank blocks\n",
				      conseq_blank_blocks);
				nand_addr -=
				    (conseq_blank_blocks - 1) * block_size;
				break;
			}
		} else {
			debug("NAND address 0x%llx is not blank\n", nand_addr);
			conseq_blank_blocks = 0;
		}
	}

	if (nand_addr >= end) {
		puts("ERROR: unable to find blank space for new bootloader\n");
		return 1;
	}
	printf("New bootloader image will be written at blank address 0x%llx, length 0x%x\n",
	       nand_addr, required_len);
	write_length = required_len;
	bytes_written = 0;
	/* Write the new bootloader to blank location. */
	if (nand_write_skip_bad(nand, nand_addr, &write_length, &bytes_written,
				CONFIG_OCTEON_NAND_BOOT_END - nand_addr,
				(u_char *)image_addr, 0) != 0 ||
				(bytes_written != required_len)) {
		puts("ERROR: error while writing new image to flash.\n");
		return 1;
	}
	printf("Wrote %d bytes at NAND offset 0x%llx\n", bytes_written,
	       nand_addr);
	/* Now erase the old bootloader of the same type.
	 * We know these are not bad NAND blocks since they have valid data
	 * in them.
	 */
	erase_base = old_image_nand_addr & ~(block_size - 1);
	erase_size = (old_image_size + block_size - 1) & ~(block_size - 1);

	debug("Erasing old image at 0x%llx\n", erase_base);
	if (nand_erase(nand, erase_base, erase_size)) {
		printf("nand_erase() failed, addr 0x%08llx, size 0x%llx\n",
		       erase_base, erase_size);
		return 1;
	}

	puts("Bootloader update in NAND complete.\n");
	return 0;
}
#endif		/* CONFIG_CMD_OCTEON_NAND */

#ifdef CONFIG_OCTEON_SPI_BOOT_END
/**
 * Given a size, round it up to the next SPI erase block size
 * @param[in]	sf	SPI flash data structure
 * @param	size	size to round up
 *
 * @return	rounded up size
 */
static uint32_t spi_size_round_up(const struct spi_flash *sf, uint32_t size)
{
	return (size + sf->erase_size - 1) & ~(sf->erase_size - 1);
}

/**
 * Validates the bootloader found at the specified offset in the SPI NOR
 *
 * @param sf		pointer to spi flash data structure
 * @param offset	offset of bootloader in spi NOR flash
 * @param size		size of bootloader
 *
 * @return 	0 if a valid bootloader was found at the offset
 *		1 if no valid bootloader was found
 *		-1 on error
 */
int validate_spi_bootloader(struct spi_flash *sf, uint32_t offset, size_t size)
{
	struct bootloader_header *header;
	uint8_t *buffer;
	int rc;

	buffer = calloc(size, 1);
	if (!buffer) {
		printf("Out of memory!  Could not allocate 0x%x bytes\n", size);
		return -1;
	}
	debug("%s(%p, 0x%x, 0x%x)\n", __func__, sf, offset, size);
	rc = spi_flash_read(sf, offset, size, buffer);
	if (rc) {
		puts("Error reading bootloader\n");
		rc = -1;
	} else {
		header = (struct bootloader_header *)buffer;
		if (validate_header(header) || validate_data(header)) {
			printf("Invalid bootloader found at offset 0x%x\n",
			       offset);
			rc = 1;
		} else if (header->board_type != CVMX_BOARD_TYPE_GENERIC &&
			   header->board_type != gd->arch.board_desc.board_type) {
			printf("Invalid bootloader board type 0x%x found at offset 0x%x\n",
			       header->board_type, offset);
			rc = 1;
		} else {
			rc = 0;
		}
	}

	free(buffer);

	return rc;
}

/**
 * Update a SPI stage 1 bootloader.
 *
 * @param[in]	sf	SPI Flash descriptor
 * @param[in]	buffer	Pointer to stage 1 bootloader buffer
 * @param	erase_size	Number of bytes to erase
 * @param	image_size	Size of image in bytes
 *
 * @return	0 for success, error otherwise.
 */
static int update_spi_stage1(struct spi_flash *sf, const uint8_t *buffer,
			     size_t image_size)
{
	int rc;
	size_t erase_size = spi_size_round_up(sf, image_size);

	rc = spi_flash_erase(sf, 0, erase_size);
	if (rc) {
		puts("Error erasing SPI flash\n");
		return rc;
	}
	puts("Writing stage 1 SPI bootloader to offset 0.\n");
	rc = spi_flash_write(sf, 0, image_size, buffer);
	if (rc) {
		puts("Error writing stage 1 bootloader to SPI flash\n");
		return rc;
	}

	printf("Successfully wrote %zu bytes.\n", image_size);
	return 0;
}

/**
 * Print out the bootloader image map
 *
 * @param[in]	map	bootloader map to print
 * @param	count	Number of map entries
 */
static void print_image_map(const struct bootloader_map *map, int count)
{
	int i;
	const char *type;

	for (i = 0; i < count; i++) {
		switch (map[i].type) {
		case BL_HEADER_IMAGE_STAGE1:
			type = "stage 1 bootloader";
			break;
		case BL_HEADER_IMAGE_STAGE2:
			type = "stage 2 bootloader";
			break;
		case BL_HEADER_IMAGE_STAGE3:
			type = "stage 3 bootloader";
			break;
		case BL_HEADER_IMAGE_NOR:
			type = "final stage bootloader";
			break;
		case BL_HEADER_IMAGE_UNKNOWN:
		default:
			type = "unknown or undefined";
			break;
		}

		printf("%d: offset: 0x%x, image size: %#x, map size: 0x%x, type: %s, free: %s\n",
		       i, map[i].offset, map[i].image_size, map[i].size, type,
		       map[i].free_space ? "yes" : "no");
	}
}

/**
 * Writes a bootloader to the SPI flash, erasing the flash first
 *
 * @param	sf	SPI flash data structure
 * @param	offset	Erase block aligned offset to install bootloader
 * @param[in]	header	Header of image to install
 *
 * @return	0 for success, -1 on error
 */
static int spi_write_image(struct spi_flash *sf, uint32_t offset,
			   const struct bootloader_header *header)
{
	uint8_t buffer[sf->erase_size];
	int rc;
	uint32_t blen = header->hlen + header->dlen;
	uint32_t elen = spi_size_round_up(sf, blen);
	int i, j;

	debug("%s offset: 0x%x, image size: 0x%x, address: %p\n",
	      __func__, offset, blen, header);
	if (offset & (sf->erase_size - 1)) {
		printf("%s: Error: offset 0x%x is not a multiple of the erase block size!\n",
		       __func__, offset);
		return -1;
	}
	for (i = offset; i < offset + elen; i += sf->erase_size) {
		bool erase_needed = false;
		rc = spi_flash_read(sf, i, sf->erase_size, buffer);
		if (rc) {
			printf("%s: Error reading SPI flash, offset: 0x%x, len: 0x%x\n",
			       __func__, i, sf->erase_size);
			return -1;
		}
		for (j = 0; j < sf->erase_size; j++) {
			if (buffer[j] != 0xff) {
				erase_needed = true;
				break;
			}
		}
		if (erase_needed) {
			rc = spi_flash_erase(sf, i, sf->erase_size);
			if (rc) {
				printf("%s: Error erasing SPI flash, offset: 0x%x, len: 0x%x\n",
				       __func__, i, sf->erase_size);
				return -1;
			}
		}
	}

	rc = spi_flash_write(sf, offset, blen, (void *)header);
	if (rc) {
		printf("%s: Error writing SPI flash, offset: 0x%x, len: 0x%x\n",
		       __func__, offset, blen);
		return -1;
	}
	return 0;
}

/**
 * Builds a map of bootloaders and free space in the SPI flash
 *
 * @param	sf	SPI flash datastructure
 * @param[out]	map	Pointer to map of SPI flash
 * @param[out]	count	Number of valid entries in the map
 * @param[out]	size	Size of map array (maximum entries)
 *
 * @return	0 for success, -1 on error.  On error, count and size will be 0
 *		and map will be NULL.
 */
int spi_build_map(struct spi_flash *sf, struct bootloader_map *map[],
		  int *count, int *size)
{
	struct bootloader_header search_header;
	uint32_t spi_end = min(sf->size, CONFIG_OCTEON_SPI_BOOT_END);
	uint32_t offset= CONFIG_OCTEON_SPI_STAGE2_START;
	int rc;
	int image_index = 0;
	size_t isize;
	bool update_index = false;
	int bl_count = 0;
	int fs_count = 0;
	int s2_count = 0;
	struct bootloader_map *bm;

	*size = 12;
	*map = calloc(sizeof(struct bootloader_map), 12);
	if (!(*map)) {
		printf("%s: Out of memory allocating map\n", __func__);
		rc = -1;
		goto done;
	}

	/* Scan the entire space for bootloaders and free space */
	do {
		rc = spi_flash_read(sf, offset, sizeof(search_header),
				    &search_header);
		if (rc) {
			printf("Error reading offset 0x%x of SPI flash\n",
			       offset);
			goto done;
		}
		if (!validate_header(&search_header)) {
			if (update_index)
				image_index++;
			update_index = false;
			isize = search_header.hlen + search_header.dlen;
			debug("Found valid 0x%x byte stage %d header at offset 0x%x\n",
			      isize,
			      search_header.image_type == BL_HEADER_IMAGE_STAGE2 ? 2 : 3,
			      offset);
			if (isize > (2 << 20)) {
				rc = 1;
				printf("Invalid bootloader image size %u exceeds 2MB\n",
				       isize);
				goto done;
			}
			debug("Validating bootloader...\n");
			rc = validate_spi_bootloader(sf, offset, isize);
			if (rc < 0) {
				printf("Error reading bootloader at offset: 0x%x, size: 0x%x from SPI flash\n",
				       offset, isize);
				goto done;
			}
			bm = &(*map)[image_index];
			debug("map: %p map[%d] %p bm: %p\n", map, image_index,
			      map[image_index], bm);
			bm->size = spi_size_round_up(sf, isize);
			bm->image_size = isize;
			bm->offset = offset;
			if (rc == 0) {
				debug("%s: Found valid stage %d bootloader at offset: 0x%x, size: 0x%x\n",
				      __func__,
				      search_header.image_type == BL_HEADER_IMAGE_STAGE2 ? 2 : 3,
				      offset, isize);
				bm->free_space = false;
				bm->type = search_header.image_type;
				if (search_header.image_type == BL_HEADER_IMAGE_STAGE2)
					s2_count++;
				bl_count++;
			} else {
				debug("%s: Found invalid bootloader at offset: 0x%x, size: 0x%x\n",
				      __func__, offset, isize);
				bm->free_space = true;
				bm->type = BL_HEADER_IMAGE_UNKNOWN;
				fs_count++;
			}
			offset += bm->size;
			image_index++;
			rc = 0;
			debug("%s: offset now 0x%x\n", __func__, offset);
		} else {
			debug("Invalid header found at offset 0x%x\n", offset);
			update_index = true;
			bm = &(*map)[image_index];
			debug("index: %d, bm: %p\n", image_index, bm);
			if (!bm->size) {
				bm->offset = offset;
				bm->free_space = true;
				fs_count++;
			}
			bm->size += sf->erase_size;
			offset += sf->erase_size;
			rc = 0;
		}
		debug("%s: index now %d, update index: %s, offset now 0x%x\n",
		      __func__, image_index, update_index ? "true" : "false",
		      offset);
	} while (offset < spi_end && image_index < (*size - 2));

	if (image_index >= *size - 2) {
		printf("Warning: found more than the expected number of bootloaders,\n"
		       "found %d stage 2 bootloaders, %d total bootloaders and %d free spaces\n",
		       s2_count, bl_count, fs_count);
	}
	debug("%s: Found %d stage 2 bootloaders, %d total bootloaders and %d free spaces\n",
	      __func__, s2_count, bl_count, fs_count);
	if (update_index)
		image_index++;
	*count = image_index;

#ifdef DEBUG
	debug("Bootloader map:\n");
	print_image_map(*map, *count);
#endif
done:
	if (rc) {
		if (*map) {
			free(*map);
			*map = NULL;
		}
		*size = 0;
	}
	return rc;
}

/**
 * This function reorders bootloaders and re-writes them plus the new bootloader
 * as needed.  This function will optimize things so that all of the bootloaders
 * are stored contiguously with minimal free space between them and with all of
 * the stage 2 bootloaders (if any) installed before final stage bootloaders.
 *
 * @param		sf	SPI flash data structure
 * @param[in,out]	map	Existing image map.  It will be overwritten by
 *				the new map.  It should be big enough to hold
 *				up to two additional entries for the new header
 *				and a potential free space at the end.
 * @param[in,out]	count	Number of entries in the map.
 * @param[in]		header	New bootloader to write
 * @param		header_index	Index in the incoming MAP file where the
 *					new image is to be written.
 * @param		insert_before	If true the new bootloader is to be
 *					inserted at header_index rather than
 *					replacing the existing bootloader there.
 *
 * @return		0 for success, error otherwise
 */
int spi_reorder_bootloaders(struct spi_flash *sf,
			    struct bootloader_map *map,
			    int *count, const struct bootloader_header *header,
			    int header_index,
			    bool insert_before)
{
	struct bootloader_map *new_map = NULL;
	int *map_indices = NULL;
	int new_count = 0;
	int i, j;
	uint32_t offset;
	uint8_t **buffers = NULL;
	int rc = 0;
	const uint32_t bsize = header->hlen + header->dlen;
	const uint32_t berase = spi_size_round_up(sf, bsize);

	debug("%s(%p, %p, %p(%d), %d, %s) bsize: 0x%x, berase: 0x%x\n",
	      __func__, sf, map, count, *count, header_index,
	      insert_before ? "true" : "false", bsize, berase);
	if (header_index >= *count) {
		printf("%s: Error: header index %d out of range 0..%d\n",
		       __func__, header_index, *count - 1);
		rc = -1;
		goto done;
	}

	/* Count number of bootloaders in the map */
	for (i = 0; i < *count; i++)
		if (!map[i].free_space)
			new_count++;

	debug("%s: Found %d bootloaders in the map\n", __func__, new_count);

	if (map[header_index].free_space) {
		debug("%s: New image being written into free space\n", __func__);
		new_count++;
	}

	/* Add one for likely free space at the end */
	new_map = (struct bootloader_map *)calloc(sizeof(struct bootloader_map),
						  max(new_count + 3, 10));
	if (!new_map) {
		printf("%s: Out of memory for new_map\n", __func__);
		rc = -1;
		goto done;
	}

	map_indices = (int *)calloc(sizeof(int), new_count + 2);
	if (!map_indices) {
		printf("%s: Outof memory for map_indices\n", __func__);
		rc = -1;
		goto done;
	}
	j = 0;

	/* First put all of the stage 2 bootloader */
	for (i = 0; i < *count; i++) {
		if ((i == header_index) &&
		    (header->image_type == BL_HEADER_IMAGE_STAGE2)) {
			debug("%s new stage 2 bootloader to new index %d\n",
			      insert_before ? "inserting" : "adding", j);
			new_map[j].free_space = false;
			new_map[j].image_size = bsize;
			new_map[j].size = berase;
			new_map[j].type = header->image_type;
			new_map[j].offset = map[i].offset;
			map_indices[j] = -1;
			j++;
			if (insert_before && !map[i].free_space) {
				new_map[j] = map[i];
				map_indices[j] = i;
				j++;
				new_count++;
			}
		} else if (!map[i].free_space &&
			   (map[i].type == BL_HEADER_IMAGE_STAGE2)) {
			debug("%s: Existing bootloader moving from index %d to index %d\n",
			      __func__, i, j);
			map_indices[j] = i;
			new_map[j++] = map[i];
		} else if (map[i].free_space) {
			debug("%s: Skipping free space index %d\n", __func__, i);
		}
	}
	debug("%s: Found %d stage 2 bootloaders\n", __func__, j);
	/* Now put the rest of them */
	for (i = 0; i < *count; i++) {
		if ((i == header_index) &&
		    (header->image_type != BL_HEADER_IMAGE_STAGE2)) {
			debug("%s new stage 3 bootloader to new index %d\n",
			      insert_before ? "inserting" : "adding", j);
			new_map[j].free_space = false;
			new_map[j].image_size = bsize;
			new_map[j].size = berase;
			new_map[j].type = header->image_type;
			new_map[j].offset = map[header_index].offset;
			map_indices[j] = -1;
			j++;
			if (insert_before && !map[i].free_space) {
				new_map[j] = map[i];
				map_indices[j] = i;
				j++;
				new_count++;
			}
		} else if (!map[i].free_space &&
			   map[i].type != BL_HEADER_IMAGE_STAGE2) {
			map_indices[j] = i;
			new_map[j++] = map[i];
		} else if (map[i].free_space) {
			debug("%s: Skipping free space index %d\n", __func__, i);
		}
	}

	debug("%s: Found %d total bootloaders (including one being added or overwritten)\n",
	      __func__, j);
	/* Now we should have a contiguous array of bootloaders so we need to
	 * fix the offsets.
	 */
	offset = CONFIG_OCTEON_SPI_STAGE2_START;
	debug("%s: Starting new layout\n", __func__);
	for (i = 0; i < new_count; i++) {
		debug("  Image: %d, start offset: 0x%x, type: stage %d\n",
		      i, offset,
		      new_map[i].type == BL_HEADER_IMAGE_STAGE2 ? 2 : 3);
		new_map[i].offset = offset;
		if (map_indices[i] >= 0 && !map[map_indices[i]].free_space)
			offset += new_map[i].size;
		else
			offset += berase;
	}
	/* Make sure they all fit */
	if (offset > CONFIG_OCTEON_SPI_BOOT_END) {
		puts("Error: new bootloaders will not fit in the space allocated for bootloaders\n");
		rc = -1;
		goto done;
	}

	debug("%s: Allocating pointers for %d buffers\n", __func__,
	      new_count + 2);
	buffers = (uint8_t **)calloc(new_count + 2, sizeof(uint8_t *));
	if (!buffers) {
		printf("%s: Out of memory for buffers!\n", __func__);
		rc = -1;
		goto done;
	}
	/* Allocate buffers for all of the images we need to move */
	for (i = 0; i < new_count; i++) {
		/* We use a NULL buffer for the index we're overwriting with
		 * the new bootloader.
		 */
		if (map_indices[i] < 0) {
			debug("  New image to be written at new index %d\n", i);
			continue;
		}
		if (new_map[i].offset != map[map_indices[i]].offset) {
			printf("Image %d will be moved from offset 0x%x to offset 0x%x\n",
			       map_indices[i], map[map_indices[i]].offset,
			       new_map[i].offset);
			debug("  image %d moving from index %d to %d and changing offset from 0x%x to 0x%x\n",
			      i, map_indices[i], i,
			      map[map_indices[i]].offset, new_map[i].offset);
			buffers[i] = (uint8_t *)calloc(new_map[i].image_size,
						       sizeof(uint8_t));
			debug("%s: Allocated buffer %d at %p, 0x%x bytes\n",
			      __func__, i, buffers[i], new_map[i].image_size);
			if (!buffers[i]) {
				printf("%s: Could not allocate %u bytes for buffers[%d]\n",
				       __func__, new_map[i].image_size, i);
				rc = -1;
				goto done;
			}
		}
	}

	for (i = 0; i < new_count; i++) {
		/* Skip any images that don't need to be moved */
		if (!buffers[i] || map_indices[i] < 0)
			continue;
		debug("%s: Reading image %d at offset: 0x%x, size: 0x%x\n",
		      __func__, i, map[map_indices[i]].offset,
		      map[map_indices[i]].image_size);
		rc = spi_flash_read(sf, map[map_indices[i]].offset,
				    map[map_indices[i]].image_size, buffers[i]);
		if (rc) {
			printf("%s: SPI flash read for image at offset 0x%x failed\n",
			       __func__, map[map_indices[i]].offset);
			goto done;
		}
	}
	/* If we're here then we have been able to read all of the images that
	 * need to be moved.
	 */
	for (i = 0; i < new_count; i++) {
		/* Only erase and overwrite images that move or are new */
		if (!buffers[i] && map_indices[i] >= 0) {
			debug("%s: Image %d does not need to be moved or overwritten\n",
			      __func__, i);
		} else if (!buffers[i] && map_indices[i] < 0) {
			debug("%s: Writing new bootloader at offset: 0x%x, size: 0x%x\n",
			      __func__, new_map[i].offset, bsize);
			rc = spi_write_image(sf, new_map[i].offset, header);
			if (rc) {
				printf("%s: Error writing 0x%x byte image to offset 0x%x\n",
				       __func__, bsize, new_map[i].offset);
				goto done;
			}
		} else {

			debug("%s: Writing image: %d, offset: 0x%x, size: 0x%x bytes\n",
			      __func__, i, new_map[i].offset, new_map[i].size);
			rc = spi_write_image(sf, new_map[i].offset,
					     (struct bootloader_header *)buffers[i]);
			if (rc) {
				printf("%s: Error writing %s bootloader index: %d, size: 0x%x to offset 0x%x\n",
				       __func__, buffers[i] ? "old" : "new", i,
				       new_map[i].image_size, new_map[i].offset);
				goto done;
			}
		}
		offset = new_map[i].offset + new_map[i].size;
	}

	if (offset < CONFIG_OCTEON_SPI_BOOT_END) {
		uint8_t buffer[sf->erase_size];
		debug("%s: Adding free space at the end of the map for 0x%x bytes\n",
		      __func__, CONFIG_OCTEON_SPI_BOOT_END - offset);
		new_map[new_count].free_space = true;
		new_map[new_count].offset = offset;
		new_map[new_count].size = CONFIG_OCTEON_SPI_BOOT_END - offset;
		new_map[new_count].image_size = 0;
		new_map[new_count].type = BL_HEADER_IMAGE_UNKNOWN;
		for (; offset < CONFIG_OCTEON_SPI_BOOT_END;
		     offset += sf->erase_size) {
			bool erase_needed = false;

			rc = spi_flash_read(sf, offset, sf->erase_size, buffer);
			if (rc) {
				printf("Error reading 0x%x bytes at offset 0x%x\n",
				       sf->erase_size, offset);
				goto done;
			}
			for (i = 0; i < sf->erase_size; i++) {
				if (buffer[i] != 0xff) {
					erase_needed = true;
					break;
				}
			}
			if (erase_needed) {
				debug("Erasing free space offset 0x%x\n",
				      offset);
				spi_flash_erase(sf, offset, sf->erase_size);
			} else {
				debug("Erase not needed for offset 0x%x\n",
				      offset);
			}
		}
		new_count++;
	}
	debug("Bootloader operation successful\n");
	for (i = 0; i < new_count; i++)
		map[i] = new_map[i];

	*count = new_count;

done:
	/* Free everything */
	if (new_map)
		free(new_map);
	if (map_indices)
		free(map_indices);
	if (buffers) {
		for (i = 0; i < new_count; i++)
			debug("%s: buffer %d: %p\n", __func__, i, buffers[i]);
			if (buffers[i])
				free(buffers[i]);
		free(buffers);
	}
	return rc;
}

/**
 * Installs a stage 2 or stage 3 bootloader
 *
 * @param		sf	SPI flash data structure
 * @param[in]		header	New bootloader to install
 * @param[in,out]	map	map of SPI NOR
 * @param[in,out]	count	Number of map entries
 * @param		failsafe	True if bootloader is to be a failsafe
 *					bootloader
 *
 * @return	0 for success, error otherwise
 */
int spi_install_stage2_stage3(struct spi_flash *sf,
			      const struct bootloader_header *header,
			      struct bootloader_map *map,
			      int *count, bool failsafe)
{
	int s2_count = 0;
	int s3_count = 0;
	int i;
	int first_s2 = -1, last_s2 = -1, first_s3 = -1, last_s3 = -1;
	uint8_t *buffer = NULL;
	int rc = 0;
	bool is_stage2 = header->image_type == BL_HEADER_IMAGE_STAGE2;
	struct bootloader_map *new_map = NULL;
	size_t bsize = header->hlen + header->dlen;

	/* First see if there are any stage 2 or stage 3 bootloaders */
	for (i = 0; i < *count; i++) {
		if (!map[i].free_space) {
			switch (map[i].type) {
			case BL_HEADER_IMAGE_STAGE2:
				if (first_s2 < 0)
					first_s2 = i;
				last_s2 = i;
				s2_count++;
				break;
			case BL_HEADER_IMAGE_STAGE3:
			case BL_HEADER_IMAGE_NOR:
				s3_count++;
				if (first_s3 < 0)
					first_s3 = i;
				last_s3 = i;
				break;
			default:
				debug("Unknown image type %d at index %d\n",
				      map[i].type, i);
				break;
			}
		}
	}
	debug("%s: first stage 2 index: %d, last stage 2 index: %d, first stage 3 index: %d, last stage 3 index: %d\n",
	       __func__, first_s2, last_s2, first_s3, last_s3);
	/* If we found them, record their offsets and lengths */
	if (s2_count > 2)
		printf("Warning: %d stage %d bootloaders were found.\n",
		       s2_count, 2);
	if (s3_count > 2)
		printf("Warning: %d stage %d bootloaders were found.\n",
		       s3_count, 3);

	debug("%s: Found %d stage 2 bootloaders and %d stage 3 bootloaders\n",
	      __func__, s2_count, s3_count);
	debug("%s: First s2 index: %d, last s2 index: %d, first s3 index: %d, last s3 index: %d\n",
	      __func__, first_s2, last_s2, first_s3, last_s3);
	/* If the flash is blank, just write it at the beginning */
	if (s2_count == 0 && s3_count == 0) {
		debug("No bootloaders found, putting at the beginning\n");
		printf("Writing 0x%x bytes to offset 0x%x\n",
		       bsize, map[0].offset);
		rc = spi_write_image(sf, map[0].offset, header);
		goto done;
	}

	if (failsafe && is_stage2) {	/* Stage 2 failsafe */
		bool insert = (s2_count == 0) && s3_count;
		rc = spi_reorder_bootloaders(sf, map, count, header,
					     0, insert);
	} else if (failsafe) {		/* Stage 3 failsafe */
		int index;

		if (s3_count > 0)
			index = first_s3;
		else
			/* Note: this works even if last_s2 is -1 */
			index = last_s2 + 1;
		debug("%s: Adding failsafe S3 bootloader at index %d\n",
		      __func__, s2_count);

		rc = spi_reorder_bootloaders(sf, map, count, header,
					     index, false);
	} else if (is_stage2) {		/* Stage 2 non-failsafe */
		bool insert;
		int index;
		uint32_t free_space;

		switch (s2_count) {
		case 0:
			index = 0;
			break;
		case 1:
			index = 1;
			break;
		default:
			index = last_s2;
			break;
		}
		free_space = map[index].size;
		if (map[index + 1].free_space)
			free_space += map[index + 1].size;
		if (free_space >= bsize)
			insert = false;
		else
			insert = s3_count > 0;

		debug("%s: %s S2 bootloader at index %d\n", __func__,
		      insert ? "inserting" : "adding", index);
		rc = spi_reorder_bootloaders(sf, map, count, header,
					     index, insert);
	} else {			/* Stage 3 non-failsafe */
		int index;

		switch (s3_count) {
		case 0:
			index = last_s2 >= 0 ? last_s2 + 1 : 0;
			break;
		case 1:
			if (first_s3 < 0) {
				printf("ERROR: first_s3 should never be < 0 here!\n");
				rc = -1;
				goto done;
			}
			index = first_s3 + 1;
			break;
		default:
			index = s2_count + s3_count - 1;
			break;
		}
		debug("%s: Adding S3 bootloader at index %d, mapped: %d\n",
		      __func__, index, index);
		rc = spi_reorder_bootloaders(sf, map, count, header,
					     index, false);
	}

	if (rc)
		printf("%s: Error installing stage %d bootloader\n", __func__,
		       is_stage2 ? 2 : 3);

done:
	if (buffer)
		free(buffer);
	if (new_map)
		free(new_map);
	return rc;
}

/**
 * Update a SPI bootloader
 *
 * @param image_addr	Address bootloader image is located in RAM
 * @param image_size	Size of image, required for stage 1 bootloader
 * @param failsafe	True if image is failsafe stage 3 bootloader
 * @param stage1	True if image is stage 1 bootloader
 *
 * @return 0 on success
 *         1 on failure
 *
 * NOTE: This code is a real mess and should be re-written!
 */
int do_bootloader_update_spi(uint32_t image_addr, size_t image_size,
			     bool failsafe, bool stage1)
{
	struct bootloader_map *image_map;
	const struct bootloader_header *header;
	uint8_t *buffer;
	struct spi_flash *sf = NULL;
	int rc = -1;
	int num_images = 0;
	int map_size = 0;
	bool is_stage2 = false;

	memset(image_map, 0, sizeof(image_map));
	if (!image_addr)
		image_addr = getenv_ulong("fileaddr", 16, 0);
	if (!image_addr)
		image_addr = getenv_ulong("loadaddr", 16, load_addr);

	sf = spi_flash_probe(0, 0, CONFIG_SF_DEFAULT_SPEED, SPI_MODE_0);
	if (!sf) {
		puts("SPI flash not found\n");
		goto error;
	}

	debug("size: %u bytes, erase block size: %u bytes\n",
	      sf->size, sf->erase_size);

	if (sf->size < CONFIG_OCTEON_SPI_BOOT_END) {
		printf("Error: Bootloader section end offset 0x%x is greater than the size of the SPI flash (0x%x)\n",
		       CONFIG_OCTEON_SPI_BOOT_END, sf->size);
		goto error;
	}

	buffer = CASTPTR(uint8_t, image_addr);
	if (stage1) {
		debug("Updating stage 1 bootloader\n");

		rc = update_spi_stage1(sf, buffer, image_size);
		return rc;
	}

	header = CASTPTR(struct bootloader_header, image_addr);

	is_stage2 = header->image_type == BL_HEADER_IMAGE_STAGE2;
	printf("%s %u byte bootloader found at address 0x%x\n",
	       is_stage2 ? "Stage 2" : "Final stage", image_size, image_addr);

	rc = spi_build_map(sf, &image_map, &num_images, &map_size);

	if (rc) {
		printf("Error building SPI flash bootloader map\n");
		goto error;
	}
	puts("Starting SPI bootloader map:\n");
	print_image_map(image_map, num_images);

	rc = spi_install_stage2_stage3(sf, header, image_map, &num_images,
				       failsafe);
	if (rc)
		goto error;
	puts("Ending SPI bootloader map:\n");
	print_image_map(image_map, num_images);

error:
	if (image_map)
		free(image_map);

	if (sf)
		free(sf);
	return rc;
}
#endif

#ifdef CONFIG_OCTEON_MMC
/**
 * Detects if an emmc image is stage 1 or stage 2/3
 *
 * @param[in]	buffer	pointer to buffer to check
 *
 * @return	1 if stage 1, 2 if stage 2 or later, -1 on error.
 */
int emmc_detect_stage(const void *buffer)
{
	int i;
	const uint8_t *ptr = (uint8_t *)buffer;
	const bootloader_header_t *header;
	bool is_stage1 = true;

	for (i = DOS_PART_TBL_OFFSET; i < DOS_PART_MAGIC_OFFSET; i++)
		if (ptr[i] != 0) {
			debug("%s: Found value %02x at offset 0x%x, not eMMC Stage 1\n",
			      __func__, ptr[i], i);
			is_stage1 = false;
			break;
		}

	if (is_stage1 && ((ptr[DOS_PART_MAGIC_OFFSET] != 0x55) ||
			  (ptr[DOS_PART_MAGIC_OFFSET + 1] != 0xaa))) {
		debug("%s: MBR signature invalid, not stage 1\n", __func__);
		is_stage1 = false;
	}

	if (is_stage1) {
		debug("%s: stage 1 bootloader\n", __func__);
		return 1;
	}

	header = (bootloader_header_t *)buffer;
	if (!validate_header(header) && !validate_data(header)) {
		debug("%s: Valid stage 2 or later bootloader found\n", __func__);
		return 2;
	}

	debug("%s: Unknown bootloader type\n", __func__);
	return -1;
}

/**
 * Checks if the partition table is valid for a stage 1 bootloader
 *
 * @param[in]	buffer		Master Boot Record sector
 * @param[out]	bootable	Bootable partition number or -1 if none
 * @param[out]	start		Starting sector of the beginnint partition
 *
 * @return	0 if valid for a stage 1 bootloader
 *		-1 if invalid for the stage 1 bootloader
 *		1 if sector is not a MBR or uninitialized
 */
static int mmc_check_partitions(const uint8_t buffer[512], int *bootable,
				uint32_t *start)
{
	int slot;
	struct dos_partition *pt;
	uint32_t strt, size;

	*bootable = -1;
	if ((buffer[DOS_PART_MAGIC_OFFSET + 0] != 0x55) ||
	    (buffer[DOS_PART_MAGIC_OFFSET + 1] != 0xaa)) {
		debug("%s: Invalid signature %02x%02x\n", __func__,
		      buffer[DOS_PART_MAGIC_OFFSET + 0],
		      buffer[DOS_PART_MAGIC_OFFSET + 1]);
		return 1;
	}
	pt = (struct dos_partition *)&buffer[DOS_PART_TBL_OFFSET];
	if (!strncmp((char *)&buffer[DOS_PBR_FSTYPE_OFFSET], "FAT", 3) ||
	    !strncmp((char *)&buffer[DOS_PBR32_FSTYPE_OFFSET], "FAT32", 5)) {
		printf("Found FAT boot sector instead of partition table\n");
		return -1;
	}
	*start = 0xffffffff;
	for (slot = 0; slot < 4; slot++) {
		if (pt->boot_ind != 0 && pt->boot_ind != 0x80) {
			debug("Invalid boot indicator for partition %d\n",
			      slot + 1);
			return -1;
		}
		if (pt->boot_ind == 0x80) {
			debug("%s: Found bootable partition %d\n",
			      __func__, slot + 1);
			*bootable = slot + 1;
		}
		strt = pt->start4[0] | (pt->start4[1] << 8) |
		       (pt->start4[2] << 16) | (pt->start4[3] << 24);
		size = pt->size4[0] | (pt->size4[1] << 8) |
		       (pt->size4[2] << 16) | (pt->size4[3] << 24);
		if (size && (strt < 16)) {
			printf("No room for stage 1 bootloader in partition %d\n",
			       slot + 1);
			return -1;
		}
		if (strt < *start && size > 0) {
			debug("%s: Current first logical partition: %d, start sector: %u\n",
			      __func__, slot + 1, strt);
			*start = strt;
		}
		pt++;
	}
	return 0;
}

/**
 * Update an eMMC bootloader
 *
 * @param image_addr	Address bootloader image is located in RAM
 * @param image_size	Size of image, currently not used and can be 0
 * @param failsafe	True if image is failsafe stage 3 bootloader
 * @param stage1	True if image is stage 1 bootloader
 *
 * @return 0 on success
 *         1 on failure
 */
int do_bootloader_update_emmc(uint32_t image_addr, size_t image_size,
			      bool failsafe, bool stage1)
{
	uint8_t stage1_buffer[8192];
	uint8_t part_buffer[512];
	int ret;
	uint32_t start = 0;
	uint32_t size;
	int dev_no = 0;
	const char *dev_name;
	block_dev_desc_t *dev_desc;
	int i;
	disk_partition_t parts[4];
	int boot_partition = -1;
	const bootloader_header_t *header = (bootloader_header_t *)image_addr;
	uint64_t space;

	ret = emmc_detect_stage((void *)image_addr);
	if (ret != 1 && ret != 2) {
		printf("Unknown eMMC image type %d, not stage 1, 2 or later.\n",
		       ret);
		return CMD_RET_FAILURE;
	}
	if (ret == 1 && !stage1) {
		puts("Detected stage 1 bootloader\n");
		stage1 = true;
	}
	memset(parts, 0, sizeof(parts));

	dev_no = CONFIG_OCTEON_STAGE3_DEVNO;
	dev_name = getenv("octeon_stage2_devname");
	if (!dev_name)
		dev_name = getenv("octeon_stage3_devname");
	if (!dev_name)
		dev_name = CONFIG_OCTEON_STAGE3_DEVNAME;

	dev_no = (int)getenv_ulong("octeon_stage3_devno", 0, (unsigned long)-1);
	if (dev_no < 0)
		dev_no = getenv_ulong("octeon_stage2_devno", 0, 0);

	dev_desc = get_dev(dev_name, dev_no);
	if (!dev_desc) {
		printf("Error: device %s %d is not available for a stage 1 bootloader\n",
		       dev_name, dev_no);
		return CMD_RET_FAILURE;
	}

	debug("%s: Reading partition sector, dev: %s %d\n", __func__,
	      dev_name, dev_no);
	/* Read partition */
	ret = dev_desc->block_read(dev_no, 0, 1, part_buffer);
	if (ret != 1) {
		printf("Error reading the partition table\n");
		return CMD_RET_FAILURE;
	}

	ret = mmc_check_partitions(part_buffer, &boot_partition, &start);
	if (ret < 0) {
		printf("Invalid MBR, cannot add %sbootloader\n",
		       stage1 ? "stage 1 " : "");
		return CMD_RET_FAILURE;
	}
	if (!stage1 && ret) {
		printf("Please install the stage 1 bootloader first.\n");
		return CMD_RET_FAILURE;
	}
	if (stage1) {
		memcpy(stage1_buffer, (void *)image_addr, sizeof(stage1_buffer));

		for (i = 0; i < 4; i++) {
			ret = get_partition_info_dos(dev_desc, i + 1, &parts[i]);
			if (ret) {
				debug("Invalid partition %d\n", i + 1);
				continue;
			} else {
				/* Make suer there's enough space for the 8k
				 * stage 1 bootloader.
				 */
				if (parts[i].start < 16 && parts[i].size > 0) {
					printf("Cannot install stage 1 bootloader due to overlap with partition %d (%s)\n",
					       i + 1, parts[i].name);
					return CMD_RET_FAILURE;
				}
				if (parts[i].bootable &&
				    (boot_partition > 0) &&
				    (boot_partition != i + 1)) {
					printf("Error: more than one bootable partition.  Both partitions %d and %d are bootable\n",
					       boot_partition, i + 1);
					return CMD_RET_FAILURE;
				}
			}
		}
		memcpy(stage1_buffer, (void *)image_addr, sizeof(stage1_buffer));
		memcpy(&stage1_buffer[DOS_PART_DISKSIG_OFFSET],
		       &part_buffer[DOS_PART_DISKSIG_OFFSET],
		       DOS_PART_MAGIC_OFFSET - DOS_PART_TBL_OFFSET);
		ret = dev_desc->block_write(dev_no, 0,
					    sizeof(stage1_buffer)/512,
					    stage1_buffer);
		if (ret != sizeof(stage1_buffer)/512) {
			printf("Error writing stage 1 bootloader\n");
			return CMD_RET_FAILURE;
		}
		return CMD_RET_SUCCESS;
	}

	space = start * 512ULL - sizeof(stage1_buffer);
	size = get_image_size(header);
	if (failsafe && space < size) {
		printf("Not enough space for the stage 2 bootloader following the stage 1 bootloader\n"
		       "The stage 2 bootloader is %u bytes but only %llu bytes are available.\n"
		       "Please re-partition the MMC device for a failsafe bootloader.\n",
		       size, space);
		return CMD_RET_FAILURE;
	}

	if (failsafe) {
		int num_sectors = (size + 511) / 512;

		ret = dev_desc->block_write(dev_no, 0x10, num_sectors,
					    (void *)image_addr);
		if (ret != num_sectors) {
			printf("Error writing failsafe bootloader\n");
			return CMD_RET_FAILURE;
		}
	} else {
		if (OCTEON_IS_MODEL(OCTEON_CN61XX) ||
		    OCTEON_IS_MODEL(OCTEON_CNF71XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN70XX)) {
			const char *boardname = getenv("boardname");
			if (!boardname) {
				printf("Error: boardname environment variable not set\n");
				return CMD_RET_FAILURE;
			}
			printf("For the non-failsafe bootloader please use the \"fatwrite\" command\n"
		               "to write the stage 2 bootloader to a bootable FAT16 or FAT32\n"
			       "filesystem and name the file \"octboot2.bin\" then write the final stage 3\n"
			       "bootloader as \"u-boot-octeon_%s.bin\" in the root directory.\n",
			       boardname);
		} else {
			puts("For the non-failsafe bootloader please use the \"fatwrite\" command\n"
			     "to write the stage 2 bootloader to a bootable FAT16 or FAT32\n"
			     "filesystem and name the file \"octboot2.bin\".\n");
		}
	}
	return CMD_RET_SUCCESS;
}
#endif

/**
 * Command for updating a bootloader image in flash.  This function
 * parses the arguments, and validates the header (if header exists.)
 * Actual flash updating is done by flash type specific functions.
 *
 * @return 0 on success
 *         1 on failure
 */
int do_bootloader_update(cmd_tbl_t * cmdtp, int flag, int argc,
			 char *const argv[])
{
	uint32_t image_addr = 0;
	uint32_t image_len = 0;
	uint32_t burn_addr = 0;
	const bootloader_header_t *header, *header2;
	bool failsafe = false;
	bool force_nand = false;
	bool force_spi = false;
	bool force_mmc = false;
	uint32_t offset = 0;

	if (argc >= 2) {
		if (!strcmp(argv[1], "nand")) {
			debug("Forced NAND bootloader update\n");
			force_nand = true;
			argc--;
			argv++;
		}
#ifdef CONFIG_OCTEON_SPI_BOOT_END
		else if (!strcmp(argv[1], "spi")) {
			debug("Forced SPI bootloader update\n");
			force_spi = true;
			argc--;
			argv++;
			if (!strcmp(argv[1], "failsafe")) {
				debug("%s: SPI failsafe mode enabled\n",
				      __func__);
				failsafe = true;
				argc--;
				argv++;
			}
		}
#endif
#ifdef CONFIG_OCTEON_MMC
		else if (!strcmp(argv[1], "mmc") || !strcmp(argv[1], "sd")) {
			force_mmc = true;
			argc--;
			argv++;
			if (!strcmp(argv[1], "failsafe")) {
				debug("%s: MMC failsafe mode enabled\n",
				      __func__);
				failsafe = true;
				argc--;
				argv++;
			}
		}
#endif
	}
#if defined(CONFIG_SYS_NO_FLASH) && defined(CONFIG_OCTEON_SPI_BOOT_END)
	if (!force_nand && !force_mmc) {
		force_spi = true;
		if (!strcmp(argv[1], "failsafe")) {
			debug("%s: SPI failsafe mode enabled\n",
			      __func__);
			failsafe = true;
			argc--;
			argv++;
		}
	}
#endif
	if (argc >= 2)
		image_addr = simple_strtoul(argv[1], NULL, 16);
	if (argc >= 3)
		image_len = simple_strtoul(argv[2], NULL, 16);
	if (argc >= 4) {
		if (force_spi || force_nand || force_mmc) {
			if (!strcmp("failsafe", argv[3])) {
				debug("%s: NAND/SPI/MMC failsafe mode enabled\n",
				      __func__);
				failsafe = true;
			}
		} else {
			burn_addr = simple_strtoul(argv[3], NULL, 16);
		}
	}
	if ((argc >= 5) && (strcmp("failsafe", argv[4]) == 0)) {
		debug("%s: Failsafe mode enabled\n", __func__);
		failsafe = true;
	}

	/* If we don't support failsafe images, we need to put the image at the
	 * base of flash, so we treat all images like failsafe image in this
	 * case.
	 */
#if defined(CONFIG_OCTEON_NO_FAILSAFE)
	if (!force_spi && !force_nand && !force_mmc) {
		debug("%s: No NOR failsafe support for this board\n", __func__);
		burn_addr = 0x1fc00000;
		failsafe = true;
	}
#endif
#if defined(CONFIG_OCTEON_SPI_NO_FAILSAFE)
	if (force_spi) {
		debug("%s: No SPI failsafe support for this board\n", __func__);
		failsafe = true;
	}
#endif

	if (!burn_addr)
		burn_addr = getenv_hex("burnaddr", 0);

	if (!image_addr) {
		image_addr = getenv_hex("fileaddr", load_addr);
		if (!image_addr) {
			puts("ERROR: Unable to get image address from "
			     "'fileaddr' environment variable\n");
			return 1;
		}
	}

	DBGUPD("%s: burn address: 0x%x, image address: 0x%x\n",
	      __func__, burn_addr, image_addr);

	/* Figure out what kind of flash we are going to update.  This will
	 * typically come from the bootloader header.  If the bootloader does
	 * not have a header, then it is assumed to be a legacy NOR image, and
	 * a destination NOR flash address must be supplied.  NAND images
	 * _must_ have a header.
	 */
	header = (void *)image_addr;
#ifndef CONFIG_SYS_NO_FLASH
	if (header->magic != BOOTLOADER_HEADER_MAGIC && !force_mmc && !force_spi) {
		/* No header, assume headerless NOR bootloader image */
		puts("No valid bootloader header found.  Assuming old headerless image\n"
		     "Image checks cannot be performed\n");

		if (!burn_addr) {
			burn_addr = CONFIG_SYS_NORMAL_BOOTLOADER_BASE;
			DBGUPD("Unable to get burn address from 'burnaddr' environment variable,\n");
			DBGUPD(" using default 0x%x\n", burn_addr);
		}
		/* We only need image length for the headerless case */
		if (!image_len) {
			image_len = getenv_hex("filesize", 0);
			if (!image_len) {
				puts("ERROR: Unable to get image size from "
				     "'filesize' environment variable\n");
				return 1;
			}
		}

		return do_bootloader_update_nor(image_addr, image_len,
						burn_addr, failsafe);
	}
#endif
	if (!force_mmc) {
		/* We have a header, so validate image */
		if (validate_header(header)) {
			puts("ERROR: Image header has invalid CRC.\n");
			return 1;
		}
		if (validate_data(header))	/* Errors printed */
			return 1;

		/* We now have a valid image, so determine what to do with it. */
		printf("Valid bootloader image found at 0x%p.\n", header);
	}
	/* Check to see that it is for the board we are running on */
	if (!force_mmc && header->board_type != cvmx_sysinfo_get()->board_type) {
		printf("Current board type: %s (%d), image board type: %s (%d)\n",
		       cvmx_board_type_to_string(cvmx_sysinfo_get()->board_type),
		       cvmx_sysinfo_get()->board_type,
		       cvmx_board_type_to_string(header->board_type),
		       header->board_type);
		if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_GENERIC
		    && header->board_type != CVMX_BOARD_TYPE_GENERIC) {
			puts("ERROR: Bootloader image is not for this board type.\n");
			return 1;
		} else {
			puts("Loading mismatched image since current "
			     "or new bootloader is generic.\n");
		}
	}

	/* SDK 1.9.0 NOR images have rev of 1.1 and unkown image_type */
	if (!force_mmc && !force_nand && !force_spi &&
	    ((header->image_type == BL_HEADER_IMAGE_NOR) ||
	     (header->image_type == BL_HEADER_IMAGE_UNKNOWN &&
	      header->maj_rev == 1 && header->min_rev == 1))) {
		debug("Updating NOR bootloader\n");
		return do_bootloader_update_nor(image_addr, 0,
						burn_addr, failsafe);
#if defined(CONFIG_CMD_OCTEON_NAND) || defined(CONFIG_OCTEON_NAND_BOOT_END)
	} else if (!force_spi && !force_mmc &&
		   (header->image_type == BL_HEADER_IMAGE_STAGE2 ||
		    header->image_type == BL_HEADER_IMAGE_STAGE3 ||
		    force_nand)) {
		debug("Updating NAND bootloader\n");
		return (do_bootloader_update_nand(image_addr));
#endif
#if defined(CONFIG_OCTEON_SPI_BOOT_END)
	} else if (!force_nand && !force_mmc &&
		   (header->image_type == BL_HEADER_IMAGE_STAGE2 ||
		    header->image_type == BL_HEADER_IMAGE_STAGE3 ||
		    header->image_type == BL_HEADER_IMAGE_PRE_UBOOT ||
		    header->image_type == BL_HEADER_IMAGE_STAGE1 ||
		    force_spi)) {
		bool stage1;

		stage1 = header->image_type == BL_HEADER_IMAGE_PRE_UBOOT ||
			 header->image_type == BL_HEADER_IMAGE_STAGE1;
		debug("Updating SPI bootloader\n");

		image_len = header->hlen + header->dlen;
		if (stage1) {
			/* There can be a stage 1.5 bootloader combined with the
			 * stage 1 bootloader.  Check for this and add it to the
			 * length if found.  Start searching on the next 64 byte
			 * boundary after the stage 1 bootloader.
			 */
			offset = (get_image_size(header) + 0x3f) & ~0x3f;
			header2 = (bootloader_header_t *)(image_addr + offset);
			if (offset > 8192) {
				printf("Error: stage 1 bootloader size %d exceeds 8192 bytes\n",
				       get_image_size(header));
				return 1;
			} else {
				debug("Rounded stage 1 bootloader size: %d\n",
				      offset);
			}
			debug("Checking for stage 1.5 bootloader at 0x%p, hlen: 0x%x, dlen: 0x%x...\n",
			      header2, header2->hlen, header2->dlen);
			if (!validate_header(header2) &&
			    !validate_data(header2)) {
				debug("Found a stage 1.5 bootloader at offset 0x%x\n",
				      offset);
				image_len = offset + get_image_size(header2);
			} else {
				printf("Invalid or missing stage 1.5 bootloader at 0x%p, header: %d, data: %d\n",
				       header2, validate_header(header2),
				       validate_data(header2));
				return 1;
			}
		}
		debug("Updating spi bootloader\n");
		return do_bootloader_update_spi(image_addr,
						image_len,
						failsafe,
						stage1);
#endif
#if defined(CONFIG_OCTEON_STAGE3_DEVNAME) || defined(CONFIG_OCTEON_STAGE2_DEVNAME)
	} else if (!force_nand && !force_spi &&
		   (header->image_type == BL_HEADER_IMAGE_STAGE2 ||
		    header->image_type == BL_HEADER_IMAGE_STAGE3 ||
		    header->image_type == BL_HEADER_IMAGE_PRE_UBOOT ||
		    (emmc_detect_stage((void *)image_addr) > 0) ||
		    force_mmc)) {
		bool stage1 = (emmc_detect_stage((void *)image_addr) == 1);
		debug("Updating eMMC bootloader\n");
		return do_bootloader_update_emmc(image_addr, 0,
						 failsafe, stage1);
#else
	} else {
		puts("ERROR: This bootloader not compiled for this medium\n");
		return 1;
#endif
	}

	return 1;
}

/**
 * Validate image header. Intended for actual header discovery.
 * Not NAND or NOR specific
 *
 * @param header     Address of expected image header.
 *
 * @return 0  on success
 *         1  on failure
 */
#define BOOTLOADER_MAX_SIZE 0x200000	/* something way too large, but it
					 * should not hopefully run ower
					 * memory end
					 */
int validate_header(const bootloader_header_t *header)
{
	uint32_t crc;

	if (header->magic == BOOTLOADER_HEADER_MAGIC) {
		/* check if header length field valid */
		if (header->hlen > BOOTLOADER_HEADER_MAX_SIZE) {
			puts("Corrupted header length field\n");
			return 1;
		}

		if ((header->maj_rev == 1) && (header->min_rev == 0)) {
			puts("Image header version 1.0, relocation not supported\n");
			return 1;
		}
		/* Check the CRC of the header */
		crc = calculate_header_crc(header);
		if (crc == header->hcrc)
			return 0;
		else {
			printf("Header crc check failed: header value 0x%x, caluclated value 0x%x \n",
			       header->hcrc, crc);
			return 1;
		}
	}

	return 1;
}

/**
 * Validate image data.
 * Not NAND or NOR specific
 *
 * @param header     Address of expected image header.
 *
 * @return 0  on success
 *         1  on failure
 */
int validate_data(const bootloader_header_t * header)
{
	uint32_t image_size, crc;

	if ((image_size = get_image_size(header)) > BOOTLOADER_MAX_SIZE) {
		printf("Image has length %d - too large?\n", image_size);
		return 1;
	}

	crc = calculate_image_crc(header);
	if (crc != header->dcrc) {
		printf("Data crc failed: data value 0x%x, calculated value 0x%x\n",
		       header->dcrc, crc);
		return 1;
	}
	return 0;
}

/**
 *  Given valid header returns image size (data + header); or 0
 */
uint32_t get_image_size(const bootloader_header_t * header)
{
	uint32_t isize = 0;
	if (!validate_header(header))
		/* failsafe has valid header - get the image size */
		isize = header->hlen + header->dlen;

	return isize;
}

int do_bootloader_validate(cmd_tbl_t * cmdtp, int flag, int argc,
			   char *const argv[])
{
	uint32_t image_addr = 0;
	const bootloader_header_t *header;

	if (argc >= 2)
		image_addr = simple_strtoul(argv[1], NULL, 16);

	if (!image_addr) {
		image_addr = getenv_hex("fileaddr", load_addr);
		if (!image_addr) {
			puts("ERROR: Unable to get image address from "
			     "'fileaddr' environment variable\n");
			return 1;
		}
	}

	header = (void *)image_addr;
	if (validate_header(header)) {
		puts("Image does not have valid header\n");
		return 1;
	}

	if (validate_data(header))
		return 1;

	printf("Image validated. Header size %d, data size %d\n", header->hlen,
	       header->dlen);
	printf("                 Header crc 0x%x, data crc 0x%x\n",
	       header->hcrc, header->dcrc);
	printf("                 Image link address is 0x%llx\n",
	       header->address);

	return 0;
}

U_BOOT_CMD(bootloaderupdate, 5, 0, do_bootloader_update,
	   "Update the bootloader in flash",
	   "[nand | spi] [image_address] [image_length]\n"
	   "Updates the the bootloader in flash.  Uses bootloader header if present\n"
	   "to validate image.\n"
	   "where:\n"
	   "  nand          - forces updating the NAND bootloader (optional)\n"
	   "  spi           - forces updating the SPI bootloader (optional)\n"
	   "  image_address - address image is located in RAM\n"
	   "  image_size    - size of image in hex\n"
	   "\n"
	   "If the image size and address are not specified then\n"
	   "$(fileaddr) and $(filesize) will be used.");

U_BOOT_CMD(bootloadervalidate, 2, 0, do_bootloader_validate,
	   "Validate the bootloader image",
	   "[image_address]\n"
	   "Validates the bootloader image.  Image must have bootloader header.\n"
	   "Validates header and image crc32\n");

#endif	/* CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE */

int do_nmi(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	cvmx_coremask_t coremask = CVMX_COREMASK_EMPTY;
	uint64_t cores;
	int node;

	if (argc > 1) {
		if (cvmx_coremask_str2bmp(&coremask, argv[1])) {
			puts("Error: could not parse coremask string\n");
			return -1;
		}
	} else {
		cvmx_coremask_set_self(&coremask);
	}

	if (octeon_has_feature(OCTEON_FEATURE_MULTINODE)) {
		for (node = CVMX_MAX_NODES - 1; node >= 0; node--) {
			cores = cvmx_coremask_get64_node(&coremask, node);
			cvmx_write_csr_node(node, CVMX_CIU3_NMI, cores);

		}
	} else {
		cores = cvmx_coremask_get64(&coremask);
		if (octeon_has_feature(OCTEON_FEATURE_CIU3))
			cvmx_write_csr(CVMX_CIU3_NMI, cores);
		else
			cvmx_write_csr(CVMX_CIU_NMI, cores);
	}
	return 0;
}

U_BOOT_CMD(nmi, 2, 0, do_nmi,
	   "Generate a non-maskable interrupt",
	   "Generate a non-maskable interrupt on core 0");
