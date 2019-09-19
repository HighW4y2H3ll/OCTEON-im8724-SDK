/***********************license start************************************
 * Copyright (c) 2009-2013 Cavium, Inc. (support@cavium.com). All rights
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
#include <asm/arch/cvmx.h>
#include <nand.h>
#include <exports.h>
#include <watchdog.h>
#include <asm/arch/cvmx-nand.h>
#include <asm/arch/octeon_nand.h>
#include <libfdt.h>
#include <fdt.h>
#include <linux/ctype.h>
#include <asm/errno.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/octeon_mem_map.h>
#include <asm/arch/cvmx-bootloader.h>
#ifdef CONFIG_OCTEON_HW_BCH
# include <asm/arch/cvmx-bch.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/** Flags used in octeon_nand_priv */
#define OCTEON_NAND_GET_SET_FEATURE		1

#define NUM_BCH_CMD_BUFFERS			2

/**
 * struct octeon_nand_priv - Private OCTEON NAND control structure
 * @selected_chip:	Currently selected chip
 * @selected_page:	Currently selected page
 * @data_len:		Total amount of data available
 * @index:		Current offset in page to read from
 * @mtd:		Pointer to mtd data structure for this chip
 * @chip:		Pointer to nand_chip data structure for this chip
 * @data:		Data buffer used for DMA of NAND pages
 * @use_status:
 * @status:
 * @flags:
 * @ecclayout:		ECC layout used when hardware BCH is used
 * @eccmask:		ECC mask XORed with ECC data for erased pages
 */
struct octeon_nand_priv {
	int selected_chip;
	int selected_page;
	int data_len;
	int index;
	struct mtd_info *mtd;
	struct nand_chip *chip;
	uint8_t *data;
	int use_status;
	uint8_t status;
	unsigned flags;
#ifdef CONFIG_OCTEON_HW_BCH
	struct nand_ecclayout *ecclayout;
	unsigned char *eccmask;
#endif
};

enum nand_extended_section_type {
	NAND_EXTENDED_UNUSED = 0,
	NAND_EXTENDED_SECTION_TYPE = 1,
	NAND_EXTENDED_ECC = 2,
};

struct nand_extended_section_info {
	uint8_t type;
	uint8_t length;
};

struct nand_extended_ecc_info {
	uint8_t ecc_bits;	/** Number of bits ECC correctability */
	uint8_t ecc_size;	/** 1 << ecc_size */
	uint16_t max_lun_bad_blocks;	/** Max bad blocks per LUN */
	uint16_t block_endurance;
	uint16_t reserved;
};

struct nand_extended_param_page_hdr {
	uint16_t crc;
	char sig[4];	/* Should be 'EPPS' */
	uint8_t reserved0[10];
	struct nand_extended_section_info section_types[8];
};

uint8_t octeon_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;

	debug("%s: offset: 0x%x, ", __func__, priv->index);
	if (priv->use_status) {
		debug("returning status: 0x%x\n", priv->status);
		return priv->status;
	}

	if (priv->index < priv->data_len) {
		debug("returning 0x%x\n", priv->data[priv->index]);
		return priv->data[priv->index++];
	} else {
		printf("error: No data to read, returning 0xff\n");
		return 0xff;
	}
}

uint16_t octeon_read_word(struct mtd_info * mtd)
{
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;
	uint16_t result;

	debug("%s: offset: 0x%x\n", __func__, priv->index);
	if (priv->use_status)
		return priv->status | (priv->status << 8);
	if (priv->index + 1 < priv->data_len) {
		result = le16_to_cpup((uint16_t *)(priv->data +
						   priv->index));
		priv->index += 2;
		return result;
	} else {
		printf("error: No data to read\n");
		return 0xffff;
	}
}

/* Write data to software buffer that will be used for later commands */
void octeon_write_buf(struct mtd_info *mtd, const uint8_t * buf, int len)
{
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;

	/* printf("%s called, len: %d\n", __FUNCTION__, len); */

	WATCHDOG_RESET();
	debug("%s: len: %d, index: %d, total length: %d\n",
	      __func__, len, priv->index, priv->data_len);
#ifdef DEBUG
	print_buffer(0, buf, 1, len, 0);
#endif
	memcpy(priv->data + priv->index, buf, len);
	priv->index += len;
	/* Linux sometimes thinks there is less OOB data than the chip really
	 * has.  Make sure all OOB is set to 0xff
	 */
	/*memset(priv->data + priv->index, 0xff,
	       sizeof(priv->data) - priv->index);*/
	WATCHDOG_RESET();
}

void octeon_read_buf(struct mtd_info *mtd, uint8_t * buf, int len)
{
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;

	debug("%s len: %d, index: %d, total length: %d\n",
	      __func__, len, priv->index, priv->data_len);

	WATCHDOG_RESET();
	if (len <= priv->data_len - priv->index) {
		memcpy(buf, priv->data + priv->index, len);
		priv->index += len;
	} else {
		printf("octeon_read_buf: Not enough data for read of %d bytes.\n"
		       "data length: %d, data index: %d\n",
		       len, priv->data_len, priv->index);
		priv->data_len = 0;
	}
	WATCHDOG_RESET();
}

/**
 * Verify that the data in the buffer matches what was last read taking into
 * account ECC error correction.
 *
 * @param mtd	pointer to mtd data structure
 * @param buf	pointer to buffer to verify against
 * @param len	length of data to verify
 *
 * @return	0 for success or -EFAULT if there are too many bad bits
 */
int octeon_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;
	int i;
	int badbits = 0;

	debug("%s(%p, %p, %d)\n", __func__, mtd, buf, len);

	if (len > priv->data_len - priv->index) {
		printf("%s beyond end of buffer (%d requested, %u available)\n",
		       __func__, len, priv->data_len - priv->index);
		return -EINVAL;
	}
	for (i = 0; i < len; i++)
		badbits += __builtin_popcount((uint32_t)
				(*(uint8_t *)(priv->data + priv->index + i)
				 ^ buf[i]));

	priv->index += len;
	if (badbits > nand->ecc.strength) {
		debug("Detected over %d bits at offset %d\n",
		      nand->ecc.strength, i);
		return -EFAULT;
	} else {
		debug("%s: %d bits differ\n", __func__, badbits);
		return 0;
	}
}

void octeon_select_chip(struct mtd_info *mtd, int chip)
{
	/* We don't need to do anything here */
}

int octeon_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	struct nand_chip *chip = mtd->priv;
	struct octeon_nand_priv *priv = chip->priv;
	int chip_select = priv->selected_chip;

	/* Check to see if the blockis marked bad, taking into account
	 * that some blocks are written with the Octeon NAND boot specific ECC
	 * that can look like a bad block (but isn't)
	 */
	WATCHDOG_RESET();
	return oct_nand_block_is_bad(chip_select, ofs, 1);
}

#ifdef CONFIG_OCTEON_HW_BCH
/**
 * Round up the ECC strength to the next value supported by the BCH engine
 * @param strength	ECC strength required by NAND device
 *
 * @return 	ECC strength supported by OCTEON BCH engine that meets the ECC
 * 		requirements or -1 if not supported.
 */
int octeon_nand_bch_strength_adjust(int strength)
{
	static const uint64_t valid = 0x1101010101010110;
	int i = strength;

	if (strength < 4) {
		printf("NAND ECC must be at least 4.\n");
		return -1;
	}
	if (strength == 64)
		return strength;

	while (!(valid & (1ull << i)) && i < 64) {
		i++;
	}
	if (i >= 64) {
		printf("NAND ECC strength %d too high\n", strength);
		return -1;
	}
	return i;
}

/**
 * Detect and correct multi-bit ECC for a page
 *
 * @param mtd	mtd block structure
 * @param dat	raw data
 * @param read_ecc	ecc from chip
 * @param calc_ecc	Not used
 *
 * @return number of bits corrected or -1 if unrecoverable
 */
static int octeon_nand_bch_correct(struct mtd_info *mtd,
				   unsigned char *dat,
				   unsigned char *read_ecc,
				   unsigned char *calc_ecc)
{
	volatile cvmx_bch_response_t response;
	struct nand_chip *chip = mtd->priv;
	const struct octeon_nand_priv *priv = chip->priv;
	int rc;
	ulong start;
	int i;
	uint8_t data_buffer[chip->ecc.size + chip->ecc.bytes];

	debug("%s(%p, %p, %p, %p)\n", __func__, mtd, dat, read_ecc, calc_ecc);

	memcpy(data_buffer, dat, chip->ecc.size);
	memcpy(data_buffer + chip->ecc.size, read_ecc, chip->ecc.bytes);
	for (i = 0; i < chip->ecc.bytes; i++)
		data_buffer[chip->ecc.size + i] ^= priv->eccmask[i];

	memset((void *)&response, 0, sizeof(response));
#ifdef DEBUG
	debug("ecc:\n");
	print_buffer(0, read_ecc, 1, chip->ecc.bytes, 0);
	debug("buffer:\n");
	print_buffer(0, dat, 1, chip->ecc.size, 0);
	debug("Calling cvmx_bch_decode...\n");
#endif
	rc = cvmx_bch_decode(data_buffer, chip->ecc.size, chip->ecc.strength,
			     dat, &response);
	if (rc) {
		printf("%s: cvmx_bch_decode failed\n", __func__);
		return -1;
	}
	start = get_timer(0);
	while (!response.s.done && get_timer(start) < 10)
		udelay(1);
	debug("%s: response done: %d, uncorrectable: %d, erased: %d, num_errors: %d, time: %lu\n",
	      __func__, response.s.done, response.s.uncorrectable, response.s.erased,
	      response.s.num_errors, get_timer(start));
#ifdef DEBUG
	debug("ecc:\n");
	print_buffer(0, read_ecc, 1, chip->ecc.bytes, 0);
	debug("buffer:\n");
	print_buffer(0, dat, 1, chip->ecc.size, 0);
#endif
	if (!response.s.done) {
		printf("%s: Error: BCH engine timeout\n", __func__);
		cvmx_bch_shutdown();
		cvmx_bch_initialize();
		return -1;
	}
	if (response.s.erased) {
		debug("%s: NAND block is erased\n", __func__);
		return 0;
	}
	if (response.s.uncorrectable) {
		printf("%s: Cannot correct NAND block\n", __func__);
		return -1;
	}
	return response.s.num_errors;
}

/**
 * @INTERNAL
 * Given a page, calculate the ECC code
 *
 * @param chip	Pointer to NAND chip data structure
 * @param buf	Buffer to calculate ECC on
 * @param[out] code	Buffer to hold ECC data
 *
 * @return 0 for success or -1 on failure
 */
static int octeon_nand_bch_calculate_ecc_internal(struct nand_chip *chip,
						  const unsigned char *buf,
						  unsigned char *code)
{
	volatile cvmx_bch_response_t response;
	const struct octeon_nand_priv *priv = chip->priv;
	int rc;
	int i;
	ulong start;
	void *ecc_ptr;
	uint8_t ecc_buffer[chip->ecc.bytes];

	debug("%s(%p, %p, %p)\n", __func__, chip, buf, code);
	if ((ulong)code % 8) {
		debug("Using ECC bounce buffer of %d bytes\n", chip->ecc.bytes);
		ecc_ptr = ecc_buffer;
		memcpy(ecc_buffer, code, chip->ecc.bytes);
	} else
		ecc_ptr = code;

	memset((void *)&response, 0, sizeof(response));
#ifdef DEBUG
	debug("pointers: code: %p, ecc_ptr: %p\n", code, ecc_ptr);
	memset(ecc_ptr, 0, chip->ecc.bytes);
#endif
	debug("Calling cvmx_bch_encode...\n");
	rc = cvmx_bch_encode(buf, chip->ecc.size, chip->ecc.strength, ecc_ptr,
			     &response);
	debug("Done, rc: %d\n", rc);
	if (rc) {
		printf("cvmx_bch_encode failed\n");
		return -1;
	}
	start = get_timer(0);
	while (!response.s.done && get_timer(start) <  100)
		;

	debug("%s: status done: %d, uncorrectable: %d, erased: %d, num errors: %d\n",
	      __func__, response.s.done, response.s.uncorrectable, response.s.erased,
	      response.s.num_errors);
#ifdef DEBUG
	debug("buffer:\n");
	print_buffer(0, buf, 1, chip->ecc.size, 0);
	debug("ecc:\n");
	print_buffer(0, ecc_ptr, 1, chip->ecc.bytes, 0);
#endif
	if (!response.s.done) {
		printf("cvmx_bch_encode timed out, response done: %d, uncorrectable: %d, num_errors: %d, erased: %d\n",
		       response.s.done, response.s.uncorrectable,
		       response.s.num_errors, response.s.erased);
		cvmx_bch_shutdown();
		cvmx_bch_initialize();
		return -1;
	}
	if ((ulong)code % 8) {
		debug("  Copying bounce buffer\n");
		memcpy(code, ecc_buffer, chip->ecc.bytes);
	}

	for (i = 0; i < chip->ecc.bytes; i++)
		code[i] ^= priv->eccmask[i];

	debug("%s: EXIT (0)\n", __func__);
	return 0;
}

/**
 * Given a page, calculate the ECC code
 *
 * @param mtd	Pointer to mtd data structure
 * @param buf	Buffer to calculate ECC on
 * @param[out] code	Buffer to hold ECC data
 *
 * @return 0 for success or -1 on failure
 */
static int octeon_nand_bch_calculate_ecc(struct mtd_info *mtd,
					 const unsigned char *buf,
					 unsigned char *code)
{
	struct nand_chip *chip = mtd->priv;

	debug("%s(%p, %p, %p)\n", __func__, mtd, buf, code);
	return octeon_nand_bch_calculate_ecc_internal(chip, buf, code);
}

/**
 * This function is called prior to a read or write operation to configure
 * the hardware ECC engine.  Since Octeon's BCH engine is independent of
 * NAND we do nothing.
 *
 * @param mtd	Pointer to MTD data structure
 * @param mode	Mode for ECC to run in
 *
 * @return 0 for success
 */
static void octeon_nand_bch_hwecc_ctl(struct mtd_info *mtd, int mode)
{
	debug("%s(%p, %d)\n", __func__, mtd, mode);
}

/**
 * Calculate the ECC OOC layout
 *
 * @param chip	Chip to calculate layout for
 *
 * @return 0 for success, otherwise failure
 */
static int octeon_nand_calc_ecc_layout(struct nand_chip *chip)
{
	struct nand_ecclayout *layout = chip->ecc.layout;
	struct octeon_nand_priv *priv = chip->priv;
	int oobsize = cvmx_nand_get_oob_size(priv->selected_chip);
	int i;
	int layout_alloc = 0;

	debug("%s(%p)\n", __func__, chip);
	if (!layout) {
		layout = malloc(sizeof(*layout));
		if (!layout) {
			printf("%s: Out of memory\n", __func__);
			return -1;
		}
		chip->ecc.layout = layout;
		layout_alloc = 1;
	}
	memset(layout, 0, sizeof(*layout));
	layout->eccbytes = chip->ecc.steps * chip->ecc.bytes;
	debug("  oob size: %d, eccbytes: %u, steps: %u, bytes: %u\n", oobsize,
	      layout->eccbytes, chip->ecc.steps, chip->ecc.bytes);
	/* Reserve 2 bytes for bad block marker */
	if (layout->eccbytes + 2 > oobsize) {
		printf("no suitable oob scheme available "
		       "for oobsize %d eccbytes %u\n", oobsize,
		       layout->eccbytes);
		goto fail;
	}
	/* put ecc bytes at oob tail */
	for (i = 0; i < layout->eccbytes; i++)
		layout->eccpos[i] = oobsize - layout->eccbytes + i;

#ifdef DEBUG
	debug("ECC layout:\n");
	print_buffer(0, layout->eccpos, 4, layout->eccbytes, 0);
#endif
	layout->oobfree[0].offset = 2;
	layout->oobfree[0].length = oobsize - 2 - layout->eccbytes;
	chip->ecc.layout = layout;
	priv->ecclayout = layout;

	debug("  layout eccbytes: %d, free offset: %d, free length: %d\n",
	      layout->eccbytes, layout->oobfree[0].offset,
	      layout->oobfree[0].length);
	return 0;

fail:
	if (layout_alloc)
		free(layout);
	debug("%s: EXIT (-1)\n", __func__);
	return -1;
}

/**
 * Initialize hardware bch support
 *
 * @param chip			- Pointer to chip data structure
 *
 * @return 0 for success, otherwise failure
 */
static int octeon_nand_hw_bch_init(struct nand_chip *chip)
{
	int i;
	struct octeon_nand_priv *priv = chip->priv;
	unsigned char *erased_page = NULL;
	unsigned int eccsize = chip->ecc.size;
	unsigned int eccbytes = chip->ecc.bytes;
	int rc;
	int bch_initialized = 0;
	int64_t bch_cmd_memory = -1;
	uint64_t bch_block_size;
	int64_t bch_pool;
	char tmp_name[64];
	uint8_t erased_ecc[eccbytes];

	debug("%s(%p) ENTER\n", __func__, chip);
	debug("  ecc size: %u, ecc bytes: %u\n", chip->ecc.size,
	      chip->ecc.bytes);
	priv->eccmask = NULL;
	if (octeon_nand_calc_ecc_layout(chip)) {
		printf("%s: Error calculating ECC layout\n", __func__);
		goto fail;
	}

	bch_block_size = cvmx_fpa_get_bch_pool_block_size();
	bch_pool = cvmx_fpa_get_bch_pool();
	snprintf(tmp_name, sizeof(tmp_name), "%s_fpa_alloc_bch",
		 OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX);
	bch_cmd_memory =
		cvmx_bootmem_phy_named_block_alloc(bch_block_size * NUM_BCH_CMD_BUFFERS,
						   0x100000,
						   0x40000000, 128,
						   tmp_name,
						   CVMX_BOOTMEM_FLAG_END_ALLOC);
	debug("%s: BCH command pool %lld memory: 0x%llx, block size: %llu\n",
	      __func__, bch_pool, bch_cmd_memory, bch_block_size);
	if (bch_cmd_memory < 0) {
		printf("Unable to allocate %llu bytes for FPA pool %lld\n",
		       bch_block_size * NUM_BCH_CMD_BUFFERS, bch_pool);
		goto fail;
	}
	debug("Adding %d FPA buffers to pool %lld\n", NUM_BCH_CMD_BUFFERS,
	      bch_pool);
	cvmx_fpa_enable();
	for (i = NUM_BCH_CMD_BUFFERS - 1; i >= 0; i--) {
		cvmx_fpa_free((void *)(uint32_t)(bch_cmd_memory + i * bch_block_size),
			      bch_pool, 0);
	}

	debug("  Initializing bch\n");
	rc = cvmx_bch_initialize();
	if (rc) {
		printf("%s: Error initializing BCH subsystem\n", __func__);
		goto fail;
	}
	bch_initialized = 1;

	/* Compute and store the inverted ecc of an erased ecc block */
	priv->eccmask = kmalloc(eccbytes, GFP_KERNEL);
	if (!priv->eccmask) {
		printf("%s: Out of memory\n", __func__);
		goto fail;
	}
	debug("  ecc size: %u\n", eccsize);
	erased_page = kmalloc(eccsize, GFP_KERNEL);
	if (!erased_page) {
		printf("%s: Out of memory\n", __func__);
		goto fail;
	}
	memset(erased_page, 0xff, eccsize);
	memset(priv->eccmask, 0, eccbytes);
	memset(erased_ecc, 0, eccbytes);

	if (octeon_nand_bch_calculate_ecc_internal(chip, erased_page,
						   erased_ecc)) {
		printf("%s: Calculating ECC failed\n", __func__);
		goto fail;
	}

	debug("%s: Freeing erased page %p\n", __func__, erased_page);
	kfree(erased_page);

#ifdef DEBUG
	debug("calculated ecc for erased page:\n");
	print_buffer(0, erased_ecc, 1, eccbytes, 0);
#endif
	for (i = 0; i < eccbytes; i++)
		priv->eccmask[i] = erased_ecc[i] ^ 0xff;

#ifdef DEBUG
	debug("ecc mask:\n");
	print_buffer(0, priv->eccmask, 1, eccbytes, 0);
	debug("%s: EXIT (0)\n", __func__);
#endif
	return 0;

fail:
	if (priv->eccmask) {
		kfree(priv->eccmask);
		priv->eccmask = NULL;
	}
	if (erased_page)
		kfree(erased_page);
	if (bch_initialized)
		cvmx_bch_shutdown();
	if (bch_cmd_memory > 0)
		cvmx_bootmem_free_named(tmp_name);
	debug("%s: EXIT (-1)\n", __func__);
	return -1;
}

static int octeon_nand_hw_bch_read_page(struct mtd_info *mtd,
					struct nand_chip *chip, uint8_t *buf,
					int oob_required, int page)
{
	struct octeon_nand_priv *priv = chip->priv;
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_code = chip->buffers->ecccode;
	uint32_t *eccpos = chip->ecc.layout->eccpos;

	debug("%s(%p, %p, %p, %d, %d)\n", __func__, mtd, chip, buf,
	      oob_required, page);

	chip->read_buf(mtd, buf, mtd->writesize);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	for (i = 0; i < chip->ecc.total; i++)
		ecc_code[i] = chip->oob_poi[eccpos[i]];
#ifdef DEBUG
	debug("pos:\n");
	print_buffer(0, eccpos, 4, chip->ecc.total, 0);
	debug("ecc code:\n");
	print_buffer(0, ecc_code, 1, chip->ecc.total, 0);
#endif

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		int stat;

		debug("%s: Correcting block offset %d, ecc offset %d\n",
		      __func__, p - buf, i);
		stat = chip->ecc.correct(mtd, p, &ecc_code[i], NULL);
		if (stat < 0) {
			mtd->ecc_stats.failed++;
			printf("Cannot correct NAND page %d\n", page);
		} else {
			mtd->ecc_stats.corrected += stat;
		}
	}
	priv->index += mtd->writesize + mtd->oobsize;
	return 0;
}

static int octeon_nand_hw_bch_write_page(struct mtd_info *mtd,
					 struct nand_chip *chip,
					 const uint8_t *buf, int oob_required)
{
	struct octeon_nand_priv *priv = chip->priv;
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	const uint8_t *p = buf;
	uint32_t *eccpos = chip->ecc.layout->eccpos;
	uint8_t *ecc_calc = chip->buffers->ecccalc;

	debug("%s(%p, %p, %p, %d)\n", __func__, mtd, chip, buf, oob_required);

	/* Hardware ECC calculation */
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);
		debug("%s: block offset %d, ecc offset %d\n", __func__,
		      p - buf, i);
	}

	for (i = 0; i < chip->ecc.total; i++)
		chip->oob_poi[eccpos[i]] = ecc_calc[i];

#ifdef DEBUG
	debug("oob:\n");
	print_buffer(0, chip->oob_poi, 1, mtd->oobsize, 0);
	debug("pos:\n");
	print_buffer(0, eccpos, 4, chip->ecc.total, 0);
#endif
	debug("start index: %d\n", priv->index);
	/* priv->index += mtd->writesize + mtd->oobsize;*/
	debug("end index: %d\n", priv->index);
	debug("%s: Calling write_page_raw(%p, %p, %p, 1)\n", __func__, mtd,
	      chip, buf);
	chip->write_buf(mtd, buf, mtd->writesize);
	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;
}

/**
 * nand_write_page_raw - [INTERN] raw page write function
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @buf: data buffer
 * @oob_required: must write chip->oob_poi to OOB
 *
 * Not for syndrome calculating ECC controllers, which use a special oob layout.
 */
static int octeon_nand_write_page_raw(struct mtd_info *mtd,
				      struct nand_chip *chip,
				      const uint8_t *buf, int oob_required)
{
	chip->write_buf(mtd, buf, mtd->writesize);
	if (oob_required)
		chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;
}

/**
 * octeon_nand_write_oob_std - [REPLACEABLE] the most common OOB data write
 *                             function
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @page: page number to write
 */
static int octeon_nand_write_oob_std(struct mtd_info *mtd,
				     struct nand_chip *chip,
				     int page)
{
	int status = 0;
	const uint8_t *buf = chip->oob_poi;
	int length = mtd->oobsize;

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize, page);
	chip->write_buf(mtd, buf, length);
	/* Send command to program the OOB data */
	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

	status = chip->waitfunc(mtd, chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

/**
 * octeon_nand_read_page_raw - [INTERN] read raw page data without ecc
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @buf: buffer to store read data
 * @oob_required: caller requires OOB data read to chip->oob_poi
 * @page: page number to read
 *
 * Not for syndrome calculating ECC controllers, which use a special oob layout.
 */
static int octeon_nand_read_page_raw(struct mtd_info *mtd,
				     struct nand_chip *chip,
				     uint8_t *buf, int oob_required, int page)
{
	chip->read_buf(mtd, buf, mtd->writesize);
	if (oob_required)
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	return 0;
}

/**
 * octeon_nand_read_oob_std - [REPLACEABLE] the most common OOB data read function
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @page: page number to read
 */
static int octeon_nand_read_oob_std(struct mtd_info *mtd,
				    struct nand_chip *chip,
				    int page)
{
	chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	return 0;
}
#endif	/* CONFIG_OCTEON_HW_BCH */

#ifndef cmdprint
# ifdef DEBUG
#  define cmdprint printf
# else
#  define cmdprint(...)
# endif
#endif
void octeon_cmdfunc(struct mtd_info *mtd, unsigned command, int column,
		    int page_addr)
{
	int status;
	struct nand_chip *nand = mtd->priv;
	struct octeon_nand_priv *priv = nand->priv;
	int buffer_length;

	cmdprint("%s called, cmd: 0x%x, col: 0x%x, page_addr: 0x%x\n",
		 __FUNCTION__, command, column, page_addr);

	priv->use_status = 0;
	if (command != NAND_CMD_PAGEPROG)
		priv->index = 0;

	WATCHDOG_RESET();
	switch (command) {
	case NAND_CMD_READID:
		cmdprint("READID, chip: %d, data_buf: %p, data_addr: 0x%llx\n",
			 priv->selected_chip, priv->data,
			 cvmx_ptr_to_phys(priv->data));
		/* Read length must be a multiple of 8, so read a little more
		 * than we require.
		 */
		priv->data_len = cvmx_nand_read_id(priv->selected_chip, column,
						   cvmx_ptr_to_phys(priv->data),
						   16);
		if (priv->data_len < 16) {
			cmdprint("READID failed with %d\n", priv->data_len);
			priv->data_len = 0;
		}
		status = cvmx_nand_reset(priv->selected_chip);
		if (status)
			cmdprint("RESET failed with %d\n", status);
		break;

	case NAND_CMD_READOOB:
		cmdprint("READOOB page_addr=0x%x\n", page_addr);
		priv->index = 8;
		/* Read length must be a multiple of 8, so we start reading
		 * 8 bytes from the end of page.
		 */
		priv->data_len = cvmx_nand_page_read(priv->selected_chip,
						     ((uint64_t)page_addr << nand->page_shift) +
						     (1 << nand->page_shift) -
						     priv->index,
						     cvmx_ptr_to_phys(priv->data),
						     mtd->oobsize +
						     priv->index);
		if (priv->data_len < mtd->oobsize + priv->index) {
			cmdprint("READOOB failed with %d\n", priv->data_len);
			priv->data_len = 0;
		}
		debug("  data length: %d\n", priv->data_len);
		break;

	case NAND_CMD_READ0:
		cmdprint("READ0 page_addr: 0x%x, column: 0x%x, data: 0x%p\n",
			 page_addr, column, priv->data);
		/* Make sure buffer length is rounded up to multiple of 8 */
		buffer_length = (1 << nand->page_shift) + mtd->oobsize;
		buffer_length = (buffer_length + 7) & ~7;
#ifdef DEBUG
		memset(priv->data, 0xee, buffer_length);
#endif
		/* Here mtd->oobsize _must_ already be a multiple of 8 */
		priv->data_len = cvmx_nand_page_read(priv->selected_chip,
						     column +
						     ((uint64_t)page_addr << nand->page_shift),
						     cvmx_ptr_to_phys(priv->data),
						     buffer_length);
		if (priv->data_len < (1 << nand->page_shift) + mtd->oobsize) {
			cmdprint("READ0 failed with %d\n", priv->data_len);
			priv->data_len = 0;
		}
		priv->selected_page = page_addr;
#ifdef DEBUG
		debug("  Warning: address is truncated to 32-bits in debug print\n");
		print_buffer((unsigned long)page_addr << nand->page_shift,
			     priv->data,
			     1, 1 << nand->page_shift, 0);
		debug("OOB: (%d bytes)\n", mtd->oobsize);
		print_buffer(0, priv->data + (1 << nand->page_shift), 1,
			     mtd->oobsize, 0);
#endif
		cmdprint("READ0 length %d\n", priv->data_len);
		break;

	case NAND_CMD_ERASE1:
		cmdprint("ERASE1 page_addr=0x%x\n", page_addr);
		if (cvmx_nand_block_erase(priv->selected_chip,
					  (uint64_t)page_addr << nand->page_shift)) {
			cmdprint("ERASE1 failed\n");
		}
		break;

	case NAND_CMD_ERASE2:
		/* We do all erase processing in the first command, so ignore
		 * this one
		 */
		break;

	case NAND_CMD_STATUS:
		cmdprint("cmdfunc: STATUS\n");
		priv->status = cvmx_nand_get_status(priv->selected_chip);
		priv->use_status = 1;
		break;

	case NAND_CMD_SEQIN:
		cmdprint("SEQIN column=%d page_addr=0x%x\n", column, page_addr);
		/* If we don't seem to be doing sequential writes then erase
		 * all data assuming it is old
		 */

		if (priv->index != column) {
			debug("Clearing data, index: 0x%x, column: 0x%x\n",
			      priv->index, column);
			memset(priv->data, 0xff, sizeof(priv->data));
		}

		priv->index = column;
		priv->selected_page = page_addr;
		priv->data_len = mtd->writesize + mtd->oobsize;
		debug("  data length: %d\n", priv->data_len);
		break;

	case NAND_CMD_PAGEPROG:
		cmdprint("cmdfunc: PAGEPROG (page=0x%x)\n", priv->selected_page);
#ifdef DEBUG
		print_buffer(priv->selected_page << nand->page_shift, priv->data,
			     1, 1 << nand->page_shift, 0);
		debug("OOB: (%d bytes)\n", mtd->oobsize);
		print_buffer(0, priv->data + (1 << nand->page_shift), 1,
			     mtd->oobsize, 0);
#endif
		status = cvmx_nand_page_write(priv->selected_chip,
					      (uint64_t)priv->selected_page <<
							nand->page_shift,
					      cvmx_ptr_to_phys(priv->data));
		if (status)
			cmdprint("PAGEPROG failed with %d\n", status);
		break;

	case NAND_CMD_RESET:
		cmdprint("RESET\n");
		priv->index = 0;
		priv->data_len = 0;
		memset(priv->data, 0xff, sizeof(priv->data));
		status = cvmx_nand_reset(priv->selected_chip);
		if (status)
			cmdprint("RESET failed with %d\n", status);
		break;

	case NAND_CMD_RNDOUT:
		/* Don't do anything. */
		cmdprint("RNDOUT column %d\n", column);
		priv->index = column;
		break;

	case NAND_CMD_PARAM:
	{
		int param_size;
		cmdprint("PARAM\n");
		param_size = mtd->writesize ?
			mtd->writesize
			: cvmx_nand_get_page_size(priv->selected_chip);
		priv->data_len =
			cvmx_nand_read_param_page(priv->selected_chip,
						  cvmx_ptr_to_phys(priv->data),
						  param_size);
		if (priv->data_len < 0) {
			cmdprint("PARAM failed with %d\n", priv->data_len);
			cmdprint("cvmx_nand_read_param_page(%d, 0x%llx, %d)\n",
				 priv->selected_chip,
				 cvmx_ptr_to_phys(priv->data), param_size);
		}
		priv->index = column;
		break;
	}

	default:
		printf("Octeon NAND octeon_cmdfunc: unsupported command 0x%x\n",
		       command);
		break;
	}
	WATCHDOG_RESET();

	cmdprint("\n");
	return;
}

int octeon_waitfunc(struct mtd_info *mtd, struct nand_chip *chip)
{

	/* Don't do anything here, as we have already waited for busy */
	return 0;
}

/**
 * Calculates the ONFI CRC16 needed for the extended parameter page
 *
 * @param crc	starting CRC value
 * @param p	pointer to data to calculate CRC over
 * @param len	length in bytes
 *
 * @return crc result
 */
static uint16_t octeon_onfi_crc16(uint16_t crc, uint8_t const *p, size_t len)
{
	int i;
	while (len--) {
		crc ^= *p++ << 8;
		for (i = 0; i < 8; i++)
			crc = (crc << 1) ^ ((crc & 0x8000) ? 0x8005 : 0);
	}

	return crc;
}

/**
 * Given an extended parameter page, calculate the size of the data structure.
 * The size is variable and is made up based on whatever data is placed in it.
 *
 * @param hdr	pointer to extended parameter page header
 *
 * @return length of extended parameter block or -1 if error.
 *
 * NOTE: This function does not verify the CRC, only the signature.
 */
static int calc_ext_param_page_size(struct nand_extended_param_page_hdr *hdr)
{
	int i;
	int length = 0;
	int ext_table_offset = 0;
	int ext_table_size = 0;
	struct nand_extended_section_info *ext_table;

	if (hdr->sig[0] != 'E' ||
	    hdr->sig[1] != 'P' ||
	    hdr->sig[2] != 'P' ||
	    hdr->sig[3] != 'S')
		return -1;

	for (i = 0; i < 8; i++) {
		if (hdr->section_types[i].type == NAND_EXTENDED_UNUSED)
			goto done;
		if (hdr->section_types[i].length > 0)
			length += 16 * hdr->section_types[i].length;
		if (hdr->section_types[i].type == NAND_EXTENDED_SECTION_TYPE) {
			ext_table_offset = length + sizeof(*hdr);
			ext_table_size = 8 * hdr->section_types[i].length;
		}
	}
	if (ext_table_offset != 0) {
		ext_table = (struct nand_extended_section_info *)
			((uint8_t *)hdr + ext_table_offset);
		for (i = 0; i < ext_table_size; i++) {
			if (ext_table[i].type == NAND_EXTENDED_UNUSED)
				goto done;
			length += ext_table[i].length;
		}
	}
done:
	return length + sizeof(struct nand_extended_param_page_hdr);
}

/**
 * Given a pointer to a NAND extended parameter page, return a pointer to the
 * next extended parameter page even if the current block is corrupt.
 */
struct nand_extended_param_page_hdr *
calc_next_ext_page(struct nand_extended_param_page_hdr *hdr, int *offset)
{
	uint8_t *ptr = (uint8_t *)(hdr + 1);
	*offset += sizeof(*hdr);
	while (*offset < 1024 - sizeof(*hdr)) {
		hdr = (struct nand_extended_param_page_hdr *)ptr;
		if (hdr->sig[0] == 'E' &&
		    hdr->sig[1] == 'P' &&
		    hdr->sig[2] == 'P' &&
		    hdr->sig[3] == 'S')
			return hdr;
		*offset += 8;
		ptr += 8;
	}
	return NULL;
}

/**
 * Reads the extended parameter page looking for ECC data
 *
 * @param chip - NAND chip data structure
 *
 * @returns 0 for success, -1 if invalid or unavailable extended parameter page
 */
static int octeon_read_extended_parameters(struct nand_chip *chip)
{
	struct octeon_nand_priv *priv = (struct octeon_nand_priv *)chip->priv;
	struct nand_extended_param_page_hdr *hdr;
	struct nand_extended_ecc_info *ecc_info;
	int size;
	int i;
	int offset;

	debug("%s: ENTER\n", __func__);
	size = 1 << chip->page_shift;
	debug("  subpage size: %d, page shift: %d, page size: %d\n",
	      chip->subpagesize, chip->page_shift, 1 << chip->page_shift);
	if (cvmx_nand_read_param_page(priv->selected_chip,
				      cvmx_ptr_to_phys(priv->data),
				      size) != size) {
		printf("%s: Could not read extended parameters from NAND chip %d\n",
		       __func__, priv->selected_chip);
		return -1;
	}

	offset = sizeof(cvmx_nand_onfi_param_page_t);
	if (chip->onfi_params.num_param_pages)
		offset *= chip->onfi_params.num_param_pages;
	else
		offset *= 3;
	hdr = (struct nand_extended_param_page_hdr *)&priv->data[offset];
#ifdef DEBUG
	debug("Parameter page read, page count: %d, offset: 0x%x\n",
	      chip->onfi_params.num_param_pages, offset);
	print_buffer(0, priv->data, 1, size, 0);
#endif
	/* Look for a valid header */
	do {
		size = calc_ext_param_page_size(hdr);
		if (size < sizeof(*hdr))
			continue;

		if (octeon_onfi_crc16(ONFI_CRC_BASE,
				      (uint8_t *)hdr->sig, size - 2) ==
							le16_to_cpu(hdr->crc))
			break;
		hdr = calc_next_ext_page(hdr, &offset);
	} while (hdr);

	debug("%s: Found valid extended parameter page at offset %d\n",
		__func__, offset);

	/* Since the types are always in order then section type 2 for
	 * extended ECC information must be within the first two entries.
	 */
	offset = 0;
	for (i = 0; i < 2; i++) {
		if (hdr->section_types[i].type == NAND_EXTENDED_ECC)
			break;
		if (hdr->section_types[i].type == NAND_EXTENDED_UNUSED) {
			debug("%s: No ECC section found\n", __func__);
			return 0;
		}

		offset += hdr->section_types[i].length * 16;
	}

	ecc_info = (struct nand_extended_ecc_info *)
					(((uint8_t *)(hdr + 1)) + offset);

	debug("Found extended ecc header at offset %d in header\n", offset);
#ifdef CONFIG_OCTEON_HW_BCH
	chip->ecc.strength = octeon_nand_bch_strength_adjust(ecc_info->ecc_bits);
#else
	chip->ecc.strength = ecc_info->ecc_bits;
#endif
	chip->ecc.size = 1 << ecc_info->ecc_size;
	if (chip->ecc.strength < 0 || chip->ecc.size > 2048) {
		printf("NAND ecc size of %d or strength %d not supported\n",
		       ecc_info->ecc_bits, chip->ecc.size);
		return -1;
	}
	debug("%s: ecc strength: %d, ecc size: %d\n", __func__,
	      chip->ecc.strength, chip->ecc.size);

	return 0;
}

int octeon_scan_bbt(struct mtd_info *mtd)
{
	/* Don't do anything here - we don't want to scan. */
	return 0;
}

/**
 * Read ONFI data and fill in various chip parameters from it
 *
 * @param chip chip data structure to fill in
 *
 * @return 0 for success, -1 on error
 */
static int octeon_nand_scan_onfi(struct nand_chip *chip)
{
#ifdef CONFIG_SYS_NAND_ONFI_DETECTION
	struct octeon_nand_priv *priv = (struct octeon_nand_priv *)chip->priv;
	cvmx_nand_onfi_param_page_t *onfi_params;
	static const uint8_t revision_decode[17] =
		{0, 0, 10, 20, 21, 22, 23, 30, 31, 0, 0, 0, 0, 0, 0, 0, 0};

	if (cvmx_nand_read_id(priv->selected_chip, 0x20,
			      cvmx_ptr_to_phys(priv->data), 8) < 8) {
		debug("ONFI detection failed for chip %d\n",
		      priv->selected_chip);
		return -1;
	}

	if (priv->data[0] != 'O' ||
	    priv->data[1] != 'N' ||
	    priv->data[2] != 'F' ||
	    priv->data[3] != 'I') {
		debug("ONFI not supported for chip %d\n", priv->selected_chip);
		return -1;
	}
	debug("Reading ONFI data\n");
	if (cvmx_nand_read_param_page(priv->selected_chip,
				      cvmx_ptr_to_phys(priv->data),
				      256 * 3) < 256 * 3) {
		printf("%s: Error reading ONFI parameter data for chip %d\n",
		       __func__, priv->selected_chip);
		return -1;
	}

	cvmx_nand_reset(priv->selected_chip);
	debug("ONFI data:\n");
#ifdef DEBUG
	print_buffer(0, priv->data, 1, 256 * 3, 0);
#endif
	onfi_params =
		cvmx_nand_onfi_process((cvmx_nand_onfi_param_page_t *)priv->data);
	if (!onfi_params) {
		printf("%s: Invalid ONFI parameter data for chip %d\n",
			__func__, priv->selected_chip);
		return -1;
	}

	memcpy(&chip->onfi_params, onfi_params,
	       sizeof(struct nand_onfi_params));

	if (le16_to_cpu(chip->onfi_params.opt_cmd) & 4)
		priv->flags |= OCTEON_NAND_GET_SET_FEATURE;

	chip->onfi_version =
		revision_decode[fls(le16_to_cpu(chip->onfi_params.revision))];
	debug("ONFI revision %d\n", chip->onfi_version);

	chip->page_shift =
		fls(le32_to_cpu(chip->onfi_params.byte_per_page)) - 1;
	chip->ecc.strength = chip->onfi_params.ecc_bits;

	if (chip->onfi_params.programs_per_page <= 1)
		chip->options |= NAND_NO_SUBPAGE_WRITE;

	if (chip->onfi_params.ecc_bits == 0) {
		chip->ecc.mode = NAND_ECC_NONE;
		chip->ecc.bytes = 0;
		chip->ecc.strength = 0;
	} else if (chip->onfi_params.ecc_bits == 1) {
		chip->ecc.mode = NAND_ECC_SOFT;
		chip->ecc.bytes = 3;
		chip->ecc.size = 256;
		chip->ecc.strength = 1;
		debug("NAND chip %d using single bit ECC\n",
		      priv->selected_chip);
#ifdef CONFIG_OCTEON_HW_BCH
	} else if (octeon_has_feature(OCTEON_FEATURE_BCH)) {
		debug("Using hardware ECC syndrome support\n");
		chip->ecc.mode = NAND_ECC_HW_SYNDROME;
		chip->ecc.strength = chip->onfi_params.ecc_bits;
		chip->ecc.read_page = octeon_nand_hw_bch_read_page;
		chip->ecc.write_page = octeon_nand_hw_bch_write_page;
		chip->ecc.read_page_raw = octeon_nand_read_page_raw;
		chip->ecc.write_page_raw = octeon_nand_write_page_raw;
		chip->ecc.read_oob = octeon_nand_read_oob_std;
		chip->ecc.write_oob = octeon_nand_write_oob_std;
		if (chip->onfi_params.ecc_bits == 0xff) {
			/* If 0xff then we need to access the extended parameter
			 * page.
			 */
			if (octeon_read_extended_parameters(chip)) {
				printf("%s: Error reading ONFI extended parameter data for chip %d\n",
				       __func__, priv->selected_chip);
				return -1;
			}
		} else {
			chip->ecc.size = 512;
		}

		/* The number of ECC bits required is m * t
		 * where (2^m) - 1 > bits per ecc block and
		 * t is the number of correctible bits.  So if
		 * a block is 512 bytes and 4 bits of ECC are
		 * to be supported then m = 13 since
		 * (2^13) - 1 > (512 * 8).  This requires a
		 * total of 52 bits.  Rounding up this is 7
		 * bytes.
		 *
		 * OCTEON is hard coded for m=15.
		 * OCTEON requires ((15 * t) + 7) / 8
		 */
		chip->ecc.bytes = ((15 * chip->ecc.strength) + 7) / 8;
		if (!chip->ecc.strength)
			chip->ecc.strength =
				(chip->ecc.bytes * 8) / fls(8 * chip->ecc.size);
		chip->ecc.calculate = octeon_nand_bch_calculate_ecc;
		chip->ecc.steps = (1 << chip->page_shift) / chip->ecc.size;
		chip->ecc.calculate = octeon_nand_bch_calculate_ecc;
		chip->ecc.correct = octeon_nand_bch_correct;
		chip->ecc.hwctl = octeon_nand_bch_hwecc_ctl;
		debug("NAND chip %d using hw_bch ECC for %d bits of correction per %d byte block.  ECC size is %d bytes\n",
		      priv->selected_chip,
		      chip->ecc.strength,
		      chip->ecc.size,
		      chip->ecc.bytes);
#endif
	} else {
		chip->ecc.mode = NAND_ECC_SOFT_BCH;
		chip->ecc.strength = chip->onfi_params.ecc_bits;
		if (chip->onfi_params.ecc_bits == 0xff) {
			/* If 0xff then we need to access the extended parameter
			 * page.
			 */
			if (!(le16_to_cpu(chip->onfi_params.features) & (1 << 7))) {
				printf("%s: Can't get ECC info, extended parameter page missing for chip %d\n",
				       __func__, priv->selected_chip);
				return -1;
			}
			if (octeon_read_extended_parameters(chip)) {
				printf("%s: Error reading ONFI extended parameter data for chip %d\n",
				       __func__, priv->selected_chip);
				return -1;
			}
		} else {
			chip->ecc.size = 512;
		}
		/* The number of ECC bits required is m * t
		 * where (2^m) - 1 > bits per ecc block and
		 * t is the number of correctible bits.  So if
		 * a block is 512 bytes and 4 bits of ECC are
		 * to be supported then m = 13 since
		 * (2^13) - 1 > (512 * 8).  This requires a
		 * total of 52 bits.  Rounding up this is 7
		 * bytes.
		 */
		chip->ecc.bytes = (((fls(chip->ecc.size) - 1 + 3 + 1) *
				    chip->ecc.strength) + 7) / 8;
		chip->ecc.steps = (1 << chip->page_shift) / chip->ecc.size;
		debug("NAND chip %d using soft_bch ECC for %d bits of correction per %d byte block.  ECC size is %d bytes\n",
		      priv->selected_chip,
		      chip->ecc.strength,
		      chip->ecc.size,
		      chip->ecc.bytes);
	}
	priv->mtd->writesize = le32_to_cpu(onfi_params->page_data_bytes);
#endif
	return 0;
}

/**
 * Get the size of oobsize, writesize and erasesize
 *
 * @param mtd	MTD data structure pointer
 * @param chip	NAND chip data structure pointer
 * @param id_data	Not used.
 *
 * @return	0 for success.
 */
static int octeon_nand_init_size(struct mtd_info *mtd, struct nand_chip *chip,
				 u8 *id_data)
{
	struct octeon_nand_priv *priv = (struct octeon_nand_priv *)chip->priv;

	debug("%s(%p, %p, %p)\n", __func__, mtd, chip, id_data);
	mtd->oobsize = cvmx_nand_get_oob_size(priv->selected_chip);
	mtd->writesize = cvmx_nand_get_page_size(priv->selected_chip);
	mtd->erasesize = 1 << (fls(cvmx_nand_get_pages_per_block(priv->selected_chip)) - 1);
	mtd->erasesize *= mtd->writesize;

	priv->mtd = mtd;
	debug("NAND %d OOB size: %d, write size: %d, erase size: %d\n",
	      priv->selected_chip, mtd->oobsize, mtd->writesize,
	      mtd->erasesize);
	/* OCTEON only supports 8-bit width */
	return 0;
}

int board_nand_init(struct nand_chip *chip)
{
	char tmp_name[64];
	struct octeon_nand_priv *nand_priv;
	static int cur_chip_select = 0;
	int i;
	int offset;
	char fdt_path[64];
	const char *ecc_mode;
	uint32_t *prop;
#if 0
	/* Don't do this here, as in some cases we need to do this before
	 * probing for NOR devices.
	 */
	if (cur_chip_select == 0) {
		/* Do cvmx_nand_stuff init stuff here */
		oct_nand_probe();
	}
#endif

	/* Find next probed NAND chip */
	for (i = cur_chip_select; i < 8; i++) {
		debug("%s: Looking for NAND chip select %d\n", __func__, i);
		WATCHDOG_RESET();
		if ((1 << i) & cvmx_nand_get_active_chips()) {
			debug("NAND chip found at cs %d\n", i);
			WATCHDOG_RESET();
			cur_chip_select = i;
			break;
		}
	}

	/* We have no more NAND chips */
	if (i == 8) {
		debug("No NAND chips found\n");
		return (-1);
	}

	/* We have found a NAND chip, so set it up.  oct_nand_probe()
	 * has done all the chip specific stuff behind the scenes.
	 * (it needs to be able to correctly identify the device paramaters,
	 * or nothing will work.)
	 */

	/* Do stuff that used to be in octeon_nand_scan */
	chip->priv = nand_priv = malloc(sizeof(struct octeon_nand_priv));
	if (!nand_priv) {
		puts("ERROR allocating memory for NAND structures\n");
		return -1;
	}
	memset(nand_priv, 0, sizeof(struct octeon_nand_priv));
	nand_priv->selected_chip = cur_chip_select;
	nand_priv->chip = chip;
	chip->numchips = 1;
	snprintf(tmp_name, sizeof(tmp_name), "%s_nand_dma_buf",
		 OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX);
	nand_priv->data =
		cvmx_bootmem_alloc_named_range_once(CVMX_NAND_MAX_PAGE_AND_OOB_SIZE,
						    0x10000, 0x7fffffff,
						    8, tmp_name, NULL);
	if (!nand_priv->data) {
		free(chip->priv);
		puts("ERROR allocating memory for NAND structures\n");
		return -1;
	}

	/* Functions that we define with something useful */
	chip->read_byte = octeon_read_byte;
	chip->read_word = octeon_read_word;
	chip->write_buf = octeon_write_buf;
	chip->read_buf = octeon_read_buf;
	chip->verify_buf = octeon_verify_buf;
	chip->block_bad = octeon_block_bad;	/* May not be needed */
	chip->cmdfunc = octeon_cmdfunc;

	/* Some functions we override, but they end up being no-ops. */
	chip->select_chip = octeon_select_chip;
	chip->waitfunc = octeon_waitfunc;
	chip->init_size = octeon_nand_init_size;

	/* We generally don't want to scan the whole flash
	 * to generate the BBT, so we have a dummy function that does
	 * nothing.
	 * The BBT scan functions do funky things to read the OOB which
	 * currently are not supported.
	 */
	chip->scan_bbt = octeon_scan_bbt;

#ifdef CONFIG_NAND_ECC_BCH
	chip->ecc.mode = NAND_ECC_SOFT_BCH;
#else
	chip->ecc.mode = NAND_ECC_SOFT;
#endif
#ifdef CONFIG_OCTEON_NAND_ECC_SIZE
	chip->ecc.size = CONFIG_OCTEON_NAND_ECC_SIZE;
#endif
#if CONFIG_OCTEON_NAND_ECC_BYTES
	chip->ecc.bytes = CONFIG_OCTEON_NAND_ECC_BYTES;
#endif

	octeon_nand_scan_onfi(chip);

#ifdef CONFIG_OF_LIBFDT
	offset = fdt_path_offset(gd->fdt_blob, "/soc/nand-flash-interface");
	if ((offset >= 0) &&
	    (!fdt_node_check_compatible(gd->fdt_blob, offset,
					"cavium,octeon-5230-nand"))) {
		snprintf(fdt_path, sizeof(fdt_path), "flash@%d", cur_chip_select);
		offset = fdt_subnode_offset(gd->fdt_blob, offset, fdt_path);
		if (offset >= 0) {
			ecc_mode = (const char *)fdt_getprop(gd->fdt_blob,
							     offset,
							     "nand-ecc-mode",
							     NULL);
			if (ecc_mode && !strcmp(ecc_mode, "soft_bch")) {
				chip->ecc.mode = NAND_ECC_SOFT_BCH;
				prop = (uint32_t *)fdt_getprop(gd->fdt_blob,
							       offset,
							       "nand-ecc-size",
							       NULL);
				if (prop)
					chip->ecc.size = fdt32_to_cpu(*prop);
				prop = (uint32_t *)fdt_getprop(gd->fdt_blob,
							       offset,
							       "nand-ecc-bytes",
							       NULL);
				if (prop)
					chip->ecc.bytes = fdt32_to_cpu(*prop);
				debug("NAND ecc mode %s, ecc size: %d, ecc bytes: %d\n",
				      ecc_mode, chip->ecc.size, chip->ecc.bytes);
			} else if (ecc_mode && !strcmp(ecc_mode, "soft")) {
				chip->ecc.mode = NAND_ECC_SOFT;
			} else if (ecc_mode && !strcmp(ecc_mode, "none")) {
				chip->ecc.mode = NAND_ECC_NONE;
#ifdef CONFIG_OCTEON_HW_BCH
			} else if (ecc_mode && !strcmp(ecc_mode, "hw_syndrome")) {
				chip->ecc.mode = NAND_ECC_HW_SYNDROME;
				chip->ecc.calculate =
						octeon_nand_bch_calculate_ecc;
				chip->ecc.correct = octeon_nand_bch_correct;
				chip->ecc.hwctl = octeon_nand_bch_hwecc_ctl;
				if (octeon_nand_hw_bch_init(chip)) {
					printf("Error initializing HW ECC support\n");
					return -1;
				}
#endif
			} else if (ecc_mode) {
				printf("ERROR : Unknown or unsupported NAND ECC mode %s\n",
				       ecc_mode);
				return -1;
			}
			debug("NAND ecc mode %s\n", ecc_mode);
		}
	}
#endif

	if ((ecc_mode = getenv("nand_ecc_mode")) != NULL) {
		if (!strcmp(ecc_mode, "soft_bch")) {
			chip->ecc.mode = NAND_ECC_SOFT_BCH;
			chip->ecc.bytes = getenv_ulong("nand_ecc_size", 0, 7);
		} else if (!strcmp(ecc_mode, "soft")) {
			chip->ecc.mode = NAND_ECC_SOFT;
		} else if (!strcmp(ecc_mode, "none")) {
			chip->ecc.mode = NAND_ECC_NONE;
		} else {
			printf("Unknown NAND ECC mode \"%s\"\n", ecc_mode);
		}
	}

	/* Move on to next chip on next invocation */
	cur_chip_select++;
	return 0;
}

int octeon_nand_get_oob_size(int nand_chip)
{

	return cvmx_nand_get_oob_size(nand_chip) & ~0x7;
}

int oct_nand_probe(void)
{
	int i;
	int active_nand_mask;
	int chip = -1;
	int debug_flag;

	/* Detect NAND flash */
	WATCHDOG_RESET();
	debug("%s: Calling cvmx_nand_initialize()\n", __func__);

	if (getenv("octeon_debug_nand")) {
		debug_flag = CVMX_NAND_INITIALIZE_FLAGS_DEBUG;
		printf("OCTEON NAND debugging enabled.\n");
	} else {
#ifdef DEBUG
		debug_flag = CVMX_NAND_INITIALIZE_FLAGS_DEBUG;
#else
		debug_flag = 0;
#endif
	}

	if (cvmx_nand_initialize(debug_flag, 0xff)) {
		debug("%s: Could not initialize NAND\n", __func__);
		return OCT_NAND_ERROR;
	}
	WATCHDOG_RESET();
	/* Find the first nand chip and use it */
	debug("%s: Calling cvmx_nand_get_active_chips()\n", __func__);
	active_nand_mask = cvmx_nand_get_active_chips();
	debug("%s: Active nand chip mask: 0x%x\n", __func__, active_nand_mask);

	for (i = 0; i < 8; i++) {
		if ((1 << i) & active_nand_mask) {
			chip = i;
			break;
		}
	}

	/* The ebt5200 board has both NAND and NOR, and they can be swapped
	 * between chip selects 0/1.  We can't tell which way it is set up other
	 * than probing to see what is there.  Older bootloader init code
	 * unconditionally enabled CS0, even in NAND boot mode (stage 0 clears
	 * it), so we handle two cases:
	 *  - CS0 enabled so NAND not detected.  This happens when a board
	 *    configured for NAND boot is booted over EJTAG/PCI.
	 *  - CS0 disnabled, NAND detected.  This is normal when booted via
	 *    stage1.
	 * In both cases we then want to set the base/size/enable on CS1 for
	 * the NOR chip.  Note that bad things seem to happen if we try CFI
	 * probing of a NAND chip.
	 * Try this for both ebt5200 boards and the generic board type.
	 */
	if (chip <= 0) {
		cvmx_mio_boot_reg_cfgx_t reg_cfg;
		cvmx_mio_boot_reg_cfgx_t reg_cfg_orig_cs0;
		reg_cfg_orig_cs0.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
		if (chip < 0) {
			reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
			reg_cfg.s.en = 0;
			cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(0), reg_cfg.u64);

			/* Detect NAND flash */
			debug("%s: Calling cvmx_nand_initialize()\n", __func__);
			if (cvmx_nand_initialize(0 & CVMX_NAND_INITIALIZE_FLAGS_DEBUG, 0xff)) {
				printf("ERROR initializing NAND\n");
				return OCT_NAND_ERROR;
			}
			WATCHDOG_RESET();

			/* Find the first nand chip and use it */
			debug("%s: Calling cvmx_nand_get_active_chips()\n",
			      __func__);
			active_nand_mask = cvmx_nand_get_active_chips();
			for (i = 0; i < 8; i++) {
				if ((1 << i) & active_nand_mask) {
					chip = i;
					break;
				}
			}

		}
		if (chip == 0) {
			/* NAND is on CS 0, rather than boot flash.  Copy CS 0
			 * config to CS1, as the early boot code set up CS 0 for
			 * boot flash.  Only do this if CS1 is not alread
			 * enabled, and CS1 doesn't have a NAND chip detected on
			 * it.
			 */
			if (!(((cvmx_mio_boot_reg_cfgx_t)
				cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(1))).s.en)
			      && !(active_nand_mask & 0x2)) {
				reg_cfg.u64 =
				    cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
				reg_cfg.s.en = 1;
				cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(1),
					       reg_cfg.u64);
				cvmx_write_csr(CVMX_MIO_BOOT_REG_TIMX(1),
					       cvmx_read_csr
					       (CVMX_MIO_BOOT_REG_TIMX(0)));
			}
		} else {
			/* Just restore CS0 config */
			cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(0),
				       reg_cfg_orig_cs0.u64);
		}
	}

	WATCHDOG_RESET();
	debug("%s: Calling cvmx_nand_get_blocks(%d)\n", __func__, chip);
	if (cvmx_nand_get_blocks(chip) < 1)
		return OCT_NAND_ERROR;

	WATCHDOG_RESET();
#ifdef CONFIG_CMD_OCTEON_NAND
	debug("%s: Calling oct_nand_set_cur_chip(%d)\n", __func__, chip);
	oct_nand_set_cur_chip(chip);
#endif
	return chip;
}

/**** NAND helper routines used elsewhere ****/

/**
 * Adjust a chip/partition size down for bad blocks so we don't
 * read/write past the end of a chip/partition by accident.
 *
 * @param[inout]	size	number of bytes to check
 * @param		offset	starting offset to read from
 * @param		dev	NAND device number
 */
void octeon_nand_adjust_size_for_badblocks(loff_t *size, loff_t offset, int dev)
{
	/* We grab the nand info object here fresh because this is usually
	 * called after arg_off_size() which can change the value of dev.
	 */
	nand_info_t *nand = &nand_info[dev];
	loff_t maxoffset = offset + *size;
	int badblocks = 0;

	/* count badblocks in NAND from offset to offset + size */
	for (; offset < maxoffset; offset += nand->erasesize) {
		if (nand_block_isbad(nand, offset))
			badblocks++;
	}
	/* adjust size if any bad blocks found */
	if (badblocks) {
		if (badblocks * nand->erasesize > *size)
			*size = 0;
		else
			*size -= badblocks * nand->erasesize;
		printf("size adjusted to 0x%llx (%d bad blocks)\n",
		       (unsigned long long)*size, badblocks);
	}
}

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
			 void *buffer, loff_t *next)
{
	nand_info_t *nand = &nand_info[dev];
	int rc;
	size_t rwsize;
	size_t read_size = size;

	debug("%s(%d, 0x%llx, 0x%llx, 0x%p, 0x%p)\n", __func__,
	      dev, start, size, buffer, next);
	octeon_nand_adjust_size_for_badblocks(&size, start, dev);
	if (!size) {
		printf("%s: Too many bad blocks, cannot read %u bytes\n",
		       __func__, read_size);
		return -1;
	}
	rwsize = size;
	debug("  rwsize: 0x%x\n", rwsize);
	rc = nand_read_skip_bad(nand, start, &rwsize, NULL, read_size,
				(u_char *)buffer);
	if (rc < 0) {
		printf("%s(%d, 0x%llx, 0x%llx, %p, %p) error reading NAND\n",
		       __func__, dev, start, size, buffer, next);
		return -1;
	}
	if (next) {
		*next = rwsize + start;
		debug("%s: Next address: 0x%llx\n", __func__, *next);
	}
	return 0;
}

/**
 * Returns if a page is blank or not
 *
 * @param	dev	NAND device to check
 * @param	offset	offset of page to check
 *
 * @return 0 if page is not blank or 1 if it is blank
 */
int octeon_nand_is_page_blank(int dev, uint64_t offset)
{
	nand_info_t *nand = &nand_info[dev];
	uint64_t buf[NAND_MAX_PAGESIZE / 8];
	size_t read_length = nand->writesize;
	int i;
	int rc;

	/* Check for bad block */
	if (nand_block_isbad(nand, offset & ~(nand->erasesize - 1))) {
		printf("%s: page at offset 0x%llx is bad\n", __func__, offset);
		return 0;
	}

	/* Read a page */
	rc = nand_read(nand, offset, &read_length, (uint8_t *)buf);
	if (rc && rc != -EUCLEAN) {
		printf("%s: error reading offset 0x%llx\n", __func__, offset);
		return 0;
	}

	for (i = 0; i < nand->writesize / 8; i++)
		if (buf[i] != 0xffffffffffffffffull)
			return 0;

	return 1;
}

/**
 * Like octeon_nand_is_page_blank but instead returns if a block is blank or not
 *
 * @param	dev	NAND device to check
 * @param	offset	Offset of erase block to check
 *
 * @return 0 if block is not blank or 1 if it is blank
 */
int octeon_nand_is_block_blank(int dev, uint64_t offset)
{
	nand_info_t *nand = &nand_info[dev];
	uint64_t end;

	offset &= ~((uint64_t)(nand->erasesize - 1));
	end = offset + nand->erasesize;
	for (; offset < end; offset += nand->writesize)
		if (!octeon_nand_is_page_blank(dev, offset))
			return 0;
	return 1;
}

#ifdef CONFIG_OCTEON_HW_BCH
extern int validate_bootloader_header(const bootloader_header_t *header);

/**
 * Scans the NAND flash for a valid bootloader header
 *
 * @param	dev	NAND device number
 * @param	count	indicate which matching bootloader, 0 for first match,
 * 			1 for second match, etc.
 * @param	board_type	board type to match against or -1 for any
 * @param	image_type	type of image to search for or 0 for all valid.
 * @param[out]	image_size	size of image on NAND
 *
 * @return	nand address of beginning of bootloader or -1 on error
 */
int64_t octeon_nand_image_search(int dev, int count, int board_type,
				 uint16_t image_type, uint32_t *image_size,
				 uint64_t start, uint64_t end)
{
	uint64_t nand_addr;
	nand_info_t *nand = &nand_info[dev];
	int page_size;
	uint8_t buf_storage[NAND_MAX_PAGESIZE] = { 0 };
	bootloader_header_t *header = (bootloader_header_t *)buf_storage;
	int rc;

	page_size = nand->writesize;

	for (nand_addr = start; nand_addr < end; nand_addr += page_size) {
		rc = octeon_nand_read_dev(dev, nand_addr, page_size,
					  end - nand_addr,
					  buf_storage, NULL);
		if (rc) {
			debug("%s: nand_read_dev of address 0x%llx returned %d\n",
			      __func__, nand_addr, rc);
			continue;
		}
		if (validate_bootloader_header(header) &&
		    ((board_type < 0) ^ (header->board_type == board_type)) &&
		    ((image_type == 0) ^ (header->image_type == image_type))) {
			if (count > 0) {
				count--;
			} else {
				if (image_size)
					*image_size =
						header->hlen + header->dlen;
				return nand_addr;
			}
		}
	}
	return -1;
}

/**
 * Validate a bootloader image at the specified NAND address
 *
 * @param	dev		nand device to read from
 * @param	nand_addr	starting address of bootloader in NAND
 * @param	end		end address of bootloader in NAND
 *
 * @return	0 for a valid image, invalid otherwise, -1 on error
 */
int octeon_nand_validate_image(int dev, loff_t nand_addr, loff_t end)
{
	nand_info_t *nand = &nand_info[dev];
	uint8_t buf_storage[NAND_MAX_PAGESIZE] = { 0 };
	const bootloader_header_t *header = (bootloader_header_t *)buf_storage;
	loff_t addr = nand_addr;
	u32 crc, dcrc;
	int page_size = nand->writesize;
	int crc_bytes_remaining;
	int rc;

	rc = octeon_nand_read_dev(dev, nand_addr, page_size, end - nand_addr,
				  buf_storage, &addr);
	if (rc < 0) {
		printf("%s: Error reading header at address 0x%llx\n",
		       __func__, nand_addr);
		return rc;
	}
	if (!validate_bootloader_header(header)) {
		printf("%s: Invalid bootloader header at NAND offset 0x%llx\n",
		       __func__, nand_addr);
		return -1;
	}
	crc = crc32(0, buf_storage + offsetof(bootloader_header_t, hlen),
		    header->hlen - offsetof(bootloader_header_t, hlen));
	dcrc = header->dcrc;

	/* Data starts at nand_addr + header.hlen */
	crc_bytes_remaining = header->dlen;
	if (header->hlen + header->dlen < page_size) {
		crc = crc32(0, buf_storage + header->hlen, header->dlen);
		debug("%s: Image size < 1 page, calculated crc: 0x%x, header expects 0x%x\n",
		      __func__, crc, header->dcrc);
		return !(crc == header->dcrc);
	}

	/* Calculate first block of data CRC skipping the header */
	crc = crc32(0, buf_storage + header->hlen, page_size - header->hlen);
	crc_bytes_remaining -= (page_size - header->hlen);

	while (crc_bytes_remaining > 0) {
		debug("%s: Reading address 0x%llx\n", __func__, addr);
		rc = octeon_nand_read_dev(dev, addr, page_size, end - addr,
					  buf_storage, &addr);
		if (rc < 0) {
			printf("%s: Error reading dev %d offset 0x%llx\n",
			       __func__, dev, addr);
			return rc;
		}
		if (crc_bytes_remaining > page_size) {
			crc = crc32(crc, buf_storage, page_size);
			crc_bytes_remaining -= page_size;
		} else {
			crc = crc32(crc, buf_storage, crc_bytes_remaining);
			crc_bytes_remaining = 0;
		}
	}
	debug("%s: Calculated data CRC: 0x%x, expect 0x%x\n", __func__, crc,
	      dcrc);
	return !(crc == dcrc);
}
#endif
