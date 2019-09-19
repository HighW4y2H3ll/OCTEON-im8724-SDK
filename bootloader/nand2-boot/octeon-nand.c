/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export
 * control laws, including the U.S. Export Administration Act and its associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Simplified interface to the NAND flash controller.  This is basically a
 * somewhat simplified version of cvmx-nand.c with most of the debbuging stuff
 * stripped out for size reasons.
 */
#undef DEBUG
#include "nand-config.h"
#include <tinylibc.h>
#include "octeon.h"
#include "cvmx-ndf-defs.h"
#include "cvmx-mio-defs.h"
#include <cvmx-swap.h>
#include "octeon-nand.h"
#include "octeon-fpa.h"
#include "octeon-bch.h"
#include <tiny_mem.h>

#define NAND_COMMAND_RNDOUT		0x05
#define NAND_COMMAND_READ_ID            0x90
#define NAND_COMMAND_READ_PARAM_PAGE    0xec
#define NAND_COMMAND_RESET              0xff
#define NAND_COMMAND_STATUS             0x70
#define NAND_COMMAND_READ               0x00
#define NAND_COMMAND_READ_FIN           0x30
#define NAND_COMMAND_ERASE              0x60
#define NAND_COMMAND_ERASE_FIN          0xd0
#define NAND_COMMAND_PROGRAM            0x80
#define NAND_COMMAND_PROGRAM_FIN        0x10
#define NAND_COMMAND_RNDOUT_START	0xE0
#define NAND_COMMAND_GET_FEATURES	0xEE
#define NAND_COMMAND_SET_FEATURES	0xEF
#define NAND_TIMEOUT_USECS_READ         100000
#define NAND_TIMEOUT_USECS_WRITE        1000000
#define NAND_TIMEOUT_USECS_BLOCK_ERASE  1000000

#define NAND_FEATURE_TIMING_MODE		0x01
#define NAND_FEATURE_OUTPUT_DRIVE_STRENGTH1	0x10
#define NAND_FEATURE_OUTPUT_DRIVE_STRENGTH2	0x80
#define NAND_FEATURE_R_B_PULLDOWN_STRENGTH	0x81
#define NAND_FEATURE_ARRAY_OP_MODE		0x90

#undef _DEBUG
#ifdef DEBUG
# define _DEBUG	1
#else
# define _DEBUG 0
#endif

/*
 * Output a debug text when condition "cond" is met. The "cond" should be
 * computed by a preprocessor in the best case, allowing for the best
 * optimization.
 */
#undef debug_cond
#undef debug
#define debug_cond(cond, fmt, args...)	\
	do { if (cond) printf(fmt, ##args); } while (0)

#define debug(fmt, args...)		debug_cond(_DEBUG, fmt, ##args)

#define CVMX_NAND_ROUNDUP(_Dividend, _Divisor) (((_Dividend)+((_Divisor)-1))/(_Divisor))

#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
#undef min
#define min(X, Y)                               \
        ({ typeof (X) __x = (X);                \
           typeof (Y) __y = (Y);                \
                (__x < __y) ? __x : __y; })

#undef max
#define max(X, Y)                               \
        ({ typeof (X) __x = (X);                \
           typeof (Y) __y = (Y);                \
                (__x > __y) ? __x : __y; })
#endif

static const onfi_speed_mode_desc_t onfi_speed_modes[] = {

	{50, 30, 100, 20, 50},	/* Mode 0 */
	{25, 15, 45, 10, 25},	/* Mode 1 */
	{17, 15, 35, 10, 15},	/* Mode 2 */
	{15, 10, 30, 5, 10},	/* Mode 3 */
	{12, 10, 25, 5, 10},	/* Mode 4, requires EDO timings */
	{10, 7, 20, 5, 10},	/* Mode 5, requries EDO timings */
	{10, 10, 25, 5, 12},	/* Mode 6, requires EDO timings */
	{15, 10, 30, 5, 15},	/* Micron NAND512WXA2S 3.3v */
	{25, 15, 45, 10, 25},	/* Micron NAND512RxA2S 1.8v */
	{25, 15, 50, 10, 0},	/* Micron NAND128/256WxA 3.3v */
	{40, 20, 60, 10, 0},	/* Micron NAND256RxA 1.8v */
};

/**
 * Array indexed by bootbus chip select with information
 * about NAND devices.
 */
struct cvmx_nand_state cvmx_nand_state[8];
static struct cvmx_nand_state cvmx_nand_default;
static cvmx_nand_initialize_flags_t cvmx_nand_flags;
static uint8_t *cvmx_nand_buffer;

void octeon_cmdfunc(struct octeon_nand_chip *chip, unsigned command, int column,
		    int page_addr);

/* Inline memset */
static inline void __cvmx_nand_clear_cmd(cvmx_nand_cmd_t *cmd)
{
	cmd->u64[0] = 0;
	cmd->u64[1] = 0;
}

/**
 * Dump a buffer out in hex for debug
 *
 * @param buffer_address
 *               Starting physical address
 * @param buffer_length
 *               Number of bytes to display
 */
static inline void __cvmx_nand_hex_dump(void *buffer, int buffer_length)
{
	print_buffer(0, buffer, 1, buffer_length, 0);
}

/* Compute the CRC for the ONFI parameter page.  Adapted from sample code
** in the specification.
*/
static uint16_t __onfi_parameter_crc_compute(uint8_t *data)
{
	const int order = 16;	/* Order of the CRC-16 */
	unsigned long i, j, c, bit;
	unsigned long crc = 0x4F4E;	/* Initialize the shift register with 0x4F4E */
	const unsigned long crcmask = ((((unsigned long)1 << (order - 1)) - 1) << 1) | 1;
	const unsigned long crchighbit = (unsigned long)1 << (order - 1);

	for (i = 0; i < 254; i++) {
		c = (unsigned long)data[i];
		for (j = 0x80; j; j >>= 1) {
			bit = crc & crchighbit;
			crc <<= 1;
			if (c & j)
				bit ^= crchighbit;
			if (bit)
				crc ^= 0x8005;
		}
		crc &= crcmask;
	}
	return crc;
}

/**
 * For OCTEON III there can be only one controller for the boot bus.  This
 * function selects NAND to be the controller and returns the old controller;
 *
 * @param select	Set to 1 to select NAND or 0 for eMMC/SD
 *
 * @return Previously selected controller
 */
static inline int __cvmx_nand_select(int select)
{
	cvmx_mio_boot_ctl_t mio_boot_ctl;

	if (OCTEON_IS_OCTEON3()) {
		debug("%s(%d) O3\n", __func__, select);
		mio_boot_ctl.u64 = cvmx_read_csr(CVMX_MIO_BOOT_CTL);
		cvmx_write_csr(CVMX_MIO_BOOT_CTL, !!select);
		return mio_boot_ctl.s.sel;
	} else {
		debug("%s(%d) !O3\n", __func__, select);
	}
	return 0;
}

/**
 * @INTERNAL
 * Get the number of bits required to encode the column bits. This
 * does not include padding to align on a byte boundary.
 *
 * @param chip   NAND chip to get data for
 *
 * @return Number of column bits
 */
static inline int __cvmx_nand_get_column_bits(int chip)
{
	return cvmx_pop(cvmx_nand_state[chip].page_size - 1);
}

/**
 * @INTERNAL
 * Get the number of bits required to encode the row bits. This
 * does not include padding to align on a byte boundary.
 *
 * @param chip   NAND chip to get data for
 *
 * @return Number of row bits
 */
static inline int __cvmx_nand_get_row_bits(int chip)
{
	return (fls(cvmx_nand_state[chip].blocks) - 1) +
		(fls(cvmx_nand_state[chip].pages_per_block) - 1);
}

/**
 * @INTERNAL
 * Get the number of address cycles required for this NAND part.
 * This include column bits, padding, page bits, and block bits.
 *
 * @param chip   NAND chip to get data for
 *
 * @return Number of address cycles on the bus
 */
static inline int __cvmx_nand_get_address_cycles(int chip)
{
	int address_bits = ((__cvmx_nand_get_column_bits(chip) + 7) >> 3) << 3;
	address_bits += ((__cvmx_nand_get_row_bits(chip) + 7) >> 3) << 3;
	return (address_bits + 7) >> 3;
}


/**
 * Validate the ONFI parameter page and return a pointer to
 * the config values.
 *
 * @param param_page Pointer to the raw NAND data returned after a parameter
 *		     page read.  It will contain at least 3 copies of the
 *		     parameter structure.
 *
 * @return Pointer to a validated paramter page, or NULL if one couldn't be found.
 */
cvmx_nand_onfi_param_page_t *
cvmx_nand_onfi_process(cvmx_nand_onfi_param_page_t param_page[3])
{
	int index;

	for (index = 0; index < 3; index++) {
		uint16_t crc = __onfi_parameter_crc_compute((void *)&param_page[index]);
		if (crc == cvmx_le16_to_cpu(param_page[index].crc))
			break;

		printf("%s: Paramter page %d is corrupt. (Expected CRC: 0x%04x, computed: 0x%04x)\n",
			     __func__, index,
			     cvmx_le16_to_cpu(param_page[index].crc),
			     crc);
	}

	if (index == 3) {
		printf("%s: All parameter pages fail CRC check.  Checking to see if any look sane.\n",
		       __func__);

		if (!memcmp(param_page, param_page + 1, 256)) {
			/* First and second copies match, now check some values */
			if (param_page[0].pages_per_block != 0
			    && param_page[0].pages_per_block != 0xFFFFFFFF
			    && param_page[0].page_data_bytes != 0
			    && param_page[0].page_data_bytes != 0xFFFFFFFF
			    && param_page[0].page_spare_bytes != 0
			    && param_page[0].page_spare_bytes != 0xFFFF
			    && param_page[0].blocks_per_lun != 0
			    && param_page[0].blocks_per_lun != 0xFFFFFFFF
			    && param_page[0].timing_mode != 0
			    && param_page[0].timing_mode != 0xFFFF) {
				/* Looks like we have enough values to use */
				printf("%s: Page 0 looks sane, using even though CRC fails.\n",
				       __func__);
				index = 0;
			}
		}
	}

	if (index == 3) {
		printf("%s: WARNING: ONFI part but no valid ONFI parameter pages found.\n",
			     __func__);
		return NULL;
	}

	return param_page + index;
}

void __set_onfi_timing_mode(int *tim_par, int clocks_us, int mode)
{
	const onfi_speed_mode_desc_t *mp = &onfi_speed_modes[mode];	/* use shorter name to fill in timing array */
	int margin;
	int pulse_adjust;

	if ((unsigned)mode >= sizeof(onfi_speed_modes)/sizeof(onfi_speed_modes[0])) {
		printf("%s: invalid ONFI timing mode: %d\n",
			     __func__, mode);
		return;
	}

	/* Adjust the read/write pulse duty cycle to make it more even.  The
	 * cycle time requirement is longer than the sum of the high low times,
	 * so we exend both the high and low times to meet the cycle time
	 * requirement.
	 */
	pulse_adjust = ((mp->twc - mp->twh - mp->twp) / 2 + 1) * clocks_us;

	/* Add a small margin to all timings. */
	margin = 2 * clocks_us;
	/* Update timing parameters based on supported mode */
	tim_par[1] = CVMX_NAND_ROUNDUP(mp->twp * clocks_us + margin + pulse_adjust, 1000);	/* Twp, WE# pulse width */
	tim_par[2] = CVMX_NAND_ROUNDUP(max(mp->twh, mp->twc - mp->twp) * clocks_us + margin + pulse_adjust, 1000);	/* Tw, WE# pulse width high */
	tim_par[3] = CVMX_NAND_ROUNDUP(mp->tclh * clocks_us + margin, 1000);	/* Tclh, CLE hold time */
	tim_par[4] = CVMX_NAND_ROUNDUP(mp->tals * clocks_us + margin, 1000);	/* Tals, ALE setup time */
	tim_par[5] = tim_par[3];	/* Talh, ALE hold time */
	tim_par[6] = tim_par[1];	/* Trp, RE# pulse width */
	tim_par[7] = tim_par[2];	/* Treh, RE# high hold time */
}

/* Internal helper function to set chip configuration to use default values */
static void __set_chip_defaults(int chip, int clocks_us)
{
	if (!cvmx_nand_default.page_size)
		return;
	cvmx_nand_state[chip].page_size = cvmx_nand_default.page_size;	/* NAND page size in bytes */
	cvmx_nand_state[chip].oob_size = cvmx_nand_default.oob_size;	/* NAND OOB (spare) size in bytes (per page) */
	cvmx_nand_state[chip].pages_per_block = cvmx_nand_default.pages_per_block;
	cvmx_nand_state[chip].blocks = cvmx_nand_default.blocks;
	cvmx_nand_state[chip].onfi_timing = cvmx_nand_default.onfi_timing;
	__set_onfi_timing_mode(cvmx_nand_state[chip].tim_par, clocks_us,
			       cvmx_nand_state[chip].onfi_timing);
	if (_DEBUG) {

		printf("%s: Using default NAND parameters.\n", __func__);
		printf("%s: Defaults: page size: %d, OOB size: %d, pages per block %d, blocks: %d, timing mode: %d\n",
			     __func__, cvmx_nand_state[chip].page_size,
			     cvmx_nand_state[chip].oob_size,
			     cvmx_nand_state[chip].pages_per_block,
			     cvmx_nand_state[chip].blocks,
			     cvmx_nand_state[chip].onfi_timing);
	}
}

/* Do the proper wait for the ready/busy signal.  First wait
 * for busy to be valid, then wait for busy to de-assert.
 */
static int __wait_for_busy_done(void)
{
	cvmx_nand_cmd_t cmd;

	__cvmx_nand_clear_cmd(&cmd);

	cmd.wait.two = 2;
	cmd.wait.r_b = 0;
	cmd.wait.n = 2;

	/* Wait for RB to be valied (tWB).
	 * Use 5 * tWC as proxy.  In some modes this is
	 * much longer than required, but does not affect performance
	 * since we will wait much longer for busy to de-assert.
	 */
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;
	cmd.wait.r_b = 1;	/* Now wait for busy to be de-asserted */
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;

	return CVMX_NAND_SUCCESS;
}

/**
 * Sets the buffer to use for low-level NAND read/write operations
 *
 * @param buffer	pointer to buffer
 */
void cvmx_nand_set_buffer(void *buffer)
{
	cvmx_nand_buffer = (uint8_t *)buffer;
}

/**
 * Initializes based on ONFI parameters
 * @param chip		chip number to initialize
 * @param clocks_us	Number of clock cycles per microsecond.
 *
 * @return CVMX_NAND_SUCCESS for success, error otherwise.
 */
static cvmx_nand_status_t
cvmx_nand_initialize_onfi(int chip, uint64_t clocks_us)
{
	cvmx_nand_onfi_param_page_t *onfi_param_page;
	uint8_t features[4];
	int mode_mask;
	int mode;
	int i;
	struct cvmx_nand_state *state = &cvmx_nand_state[chip];
	cvmx_nand_status_t rc;
	int status;

	rc = cvmx_nand_read_param_page(chip,
				       octeon_ptr_to_phys(cvmx_nand_buffer),
				       256 * 3);
	if (rc < 0) {
		printf("%s: Error %d reading parameter page for chip %d\n",
		       __func__, rc, chip);
		return rc;
	}
	onfi_param_page = cvmx_nand_onfi_process(
			(cvmx_nand_onfi_param_page_t *)cvmx_nand_buffer);
	if (!onfi_param_page) {
		if (_DEBUG)
			printf("%s: ONFI parameter page missing or invalid.\n",
			       __func__);
		return CVMX_NAND_ERROR;
	}

	/* ONFI NAND parts are described by a parameter page.  Here we extract
	 * the configuration values from the parameter page that we need to
	 * access the chip.
	 */
	state->page_size = cvmx_le32_to_cpu(onfi_param_page->page_data_bytes);
	state->oob_size = cvmx_le16_to_cpu(onfi_param_page->page_spare_bytes);
	state->pages_per_block = le32_to_cpu(onfi_param_page->pages_per_block);
	state->blocks = cvmx_le32_to_cpu(onfi_param_page->blocks_per_lun)
				* onfi_param_page->number_lun;
	mode_mask = cvmx_le16_to_cpu(onfi_param_page->timing_mode);
	if (mode_mask <= 0x3f) {
		mode = 0;
		for (i = 0; i < 6; i++) {
			if (mode_mask & (1 << i))
				mode = i;
		}
		state->onfi_timing = mode;
	} else {
		state->onfi_timing = 0;
	}

	/* If get/set parameters are supported attempt
	 * to put the chip into the mode.
	 */
	if (cvmx_le16_to_cpu(onfi_param_page->optional_commands) & 4) {
		debug("%s: Setting timing parameter mode to %d\n",
		      __func__, state->onfi_timing);
		memset(features, 0, sizeof(features));
		features[0] = state->onfi_timing;
		cvmx_nand_set_feature(chip, NAND_FEATURE_TIMING_MODE,
				      features);
	}
	__set_onfi_timing_mode(state->tim_par, clocks_us, state->onfi_timing);
	status = cvmx_nand_get_status(chip);
	if ((status & 0xE3) != 0xE0) {
		printf("%s: Status 0x%x setting timing feature\n",
		       __func__, status);
	}
	if (state->page_size + state->oob_size > CVMX_NAND_MAX_PAGE_AND_OOB_SIZE) {
		printf("%s: ERROR: Page size (%d) + OOB size (%d) is greater than max size (%d)\n",
		       __func__, state->page_size, state->oob_size, CVMX_NAND_MAX_PAGE_AND_OOB_SIZE);
		return CVMX_NAND_ERROR;
	}
	return CVMX_NAND_SUCCESS;
}

/**
 * Called to initialize the NAND controller for use. Note that
 * you must be running out of L2 or memory and not NAND before
 * calling this function.
 * When probing for NAND chips, this function attempts to autoconfigure based on
 * the NAND parts detected.  It currently supports autodetection for ONFI parts
 * (with valid parameter pages), and some Samsung NAND parts (decoding ID bits.)
 *  If autoconfiguration fails, the defaults set with __set_chip_defaults()
 * prior to calling cvmx_nand_initialize() are used.
 * If defaults are set and the CVMX_NAND_INITIALIZE_FLAGS_DONT_PROBE flag is
 * provided, the defaults are used for all chips in the active_chips mask.
 *
 * @param flags  Optional initialization flags
 *               If the CVMX_NAND_INITIALIZE_FLAGS_DONT_PROBE flag is passed,
 *		 chips are not probed, and the default parameters (if set with
 *		 cvmx_nand_set_defaults) are used for all chips in the
 *		  active_chips mask.
 * @param active_chips
 *               Each bit in this parameter represents a chip select that might
 *               contain NAND flash. Any chip select present in this bitmask may
 *               be connected to NAND. It is normally safe to pass 0xff here and
 *               let the API probe all 8 chip selects.
 *
 * @return Zero on success, a negative cvmx_nand_status error code on failure
 */
cvmx_nand_status_t cvmx_nand_initialize(cvmx_nand_initialize_flags_t flags,
					int active_chips)
{
	int chip;
	int start_chip;
	int stop_chip;
	uint64_t clocks_us;
	union cvmx_ndf_misc ndf_misc;
	uint8_t nand_id_buffer[16];
	int nand_selected;

	debug("%s(0x%x, 0x%x)\n", __func__, flags, active_chips);
	memset(cvmx_nand_state, 0, sizeof(cvmx_nand_state));
	memset(&cvmx_nand_default, 0, sizeof(cvmx_nand_default));

	cvmx_nand_flags = flags;

	nand_selected = __cvmx_nand_select(1);

	/* Disable boot mode and reset the fifo */
	ndf_misc.u64 = cvmx_read_csr(CVMX_NDF_MISC);
	ndf_misc.s.rd_cmd = 0;
	ndf_misc.s.bt_dma = 0;
	ndf_misc.s.bt_dis = 1;
	ndf_misc.s.ex_dis = 0;
	ndf_misc.s.rst_ff = 1;
	cvmx_write_csr(CVMX_NDF_MISC, ndf_misc.u64);
	cvmx_read_csr(CVMX_NDF_MISC);

	/* Bring the fifo out of reset */
	udelay(1);
	ndf_misc.s.rst_ff = 0;
	cvmx_write_csr(CVMX_NDF_MISC, ndf_misc.u64);
	cvmx_read_csr(CVMX_NDF_MISC);
	udelay(1);

	/* Clear the ECC counter */
	/* cvmx_write_csr(CVMX_NDF_ECC_CNT, cvmx_read_csr(CVMX_NDF_ECC_CNT)); */

	/* Clear the interrupt state */
	cvmx_write_csr(CVMX_NDF_INT, cvmx_read_csr(CVMX_NDF_INT));
	cvmx_write_csr(CVMX_NDF_INT_EN, 0);
	cvmx_write_csr(CVMX_MIO_NDF_DMA_INT,
		       cvmx_read_csr(CVMX_MIO_NDF_DMA_INT));
	cvmx_write_csr(CVMX_MIO_NDF_DMA_INT_EN, 0);

	start_chip = 0;
	stop_chip = 8;

	/* Figure out how many clocks are in one microsecond, rounding up */
	clocks_us = CVMX_NAND_ROUNDUP(octeon_clock_get_rate_node(0,
								 CVMX_CLOCK_SCLK),
				      1000000);

	/* Probe and see what NAND flash we can find */
	for (chip = start_chip; chip < stop_chip; chip++) {
		union cvmx_mio_boot_reg_cfgx mio_boot_reg_cfg;
		int probe_failed;

		/* Skip chip selects that the caller didn't supply in the
		 * active chip bits
		 */
		if (((1 << chip) & active_chips) == 0)
			continue;

		mio_boot_reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(chip));
		/* Enabled regions can't be connected to NAND flash */
		if (mio_boot_reg_cfg.s.en)
			continue;

		/* Start out with some sane, but slow, defaults */
		cvmx_nand_state[chip].page_size = 0;
		cvmx_nand_state[chip].oob_size = 64;
		cvmx_nand_state[chip].pages_per_block = 64;
		cvmx_nand_state[chip].blocks = 100;

		/* Set timing mode to ONFI mode 0 for initial accesses */
		__set_onfi_timing_mode(cvmx_nand_state[chip].tim_par,
				       clocks_us, 0);

		/* Put the index of which timing parameter to use.  The indexes
		 * are into the tim_par which match the indexes of the 8 timing
		 * parameters that the hardware supports.
		 * Index 0 is not software controlled, and is fixed by
		 * hardware.
		 */
		cvmx_nand_state[chip].clen[0] = 0;	/* Command doesn't need to be held before WE */
		cvmx_nand_state[chip].clen[1] = 1;	/* Twp, WE# pulse width */
		cvmx_nand_state[chip].clen[2] = 3;	/* Tclh, CLE hold time */
		cvmx_nand_state[chip].clen[3] = 1;

		cvmx_nand_state[chip].alen[0] = 4;	/* Tals, ALE setup time */
		cvmx_nand_state[chip].alen[1] = 1;	/* Twp, WE# pulse width */
		cvmx_nand_state[chip].alen[2] = 2;	/* Twh, WE# pulse width high */
		cvmx_nand_state[chip].alen[3] = 5;	/* Talh, ALE hold time */

		cvmx_nand_state[chip].rdn[0] = 0;
		cvmx_nand_state[chip].rdn[1] = 6;	/* Trp, RE# pulse width */
		cvmx_nand_state[chip].rdn[2] = 7;	/* Treh, RE# high hold time */
		cvmx_nand_state[chip].rdn[3] = 0;

		cvmx_nand_state[chip].wrn[0] = 1;	/* Twp, WE# pulse width */
		cvmx_nand_state[chip].wrn[1] = 2;	/* Twh, WE# pulse width high */

		/* Probe and see if we get an answer.  Read more than required,
		 * as in 16 bit mode only every other byte is valid.
		 * Here we probe twice, once in 8 bit mode, and once in 16 bit
		 * mode to autodetect the width.
		 */
		probe_failed = 0;

		memset(cvmx_nand_buffer, 0xff, 16);
		if (_DEBUG)
			printf("%s: reading ID from chip %d\n", __func__, chip);
		if (cvmx_nand_read_id(chip, 0x0,
				      octeon_ptr_to_phys(cvmx_nand_buffer),
				      16) < 16) {
			if (_DEBUG)
				printf("%s: Failed to probe chip %d\n",
					     __func__, chip);
			probe_failed = 1;
		}
		if (((cvmx_nand_buffer[0] == 0xff) &&
		     (cvmx_nand_buffer[1] == 0xff) &&
		     (cvmx_nand_buffer[2] == 0xff) &&
		     (cvmx_nand_buffer[3] == 0xff)) ||
		    ((cvmx_nand_buffer[0] == 0) &&
		     (cvmx_nand_buffer[1] == 0) &&
		     (cvmx_nand_buffer[2] == 0) &&
		     (cvmx_nand_buffer[3] == 0))) {
			if (_DEBUG)
				printf("%s: Probe returned nothing for chip %d\n",
					     __func__, chip);
			probe_failed = 1;
		}
		/* Neither 8 or 16 bit mode worked, so go on to next chip select */
		if (probe_failed)
			continue;

		/* Save copy of ID for later use */
		memcpy(nand_id_buffer, cvmx_nand_buffer, sizeof(nand_id_buffer));

		if (_DEBUG)
			printf("%s: NAND chip %d has ID %x %x %x %x %x %x %x %x\n",
			       __func__, chip, cvmx_nand_buffer[0],
			       cvmx_nand_buffer[1], cvmx_nand_buffer[2],
			       cvmx_nand_buffer[3], cvmx_nand_buffer[4],
			       cvmx_nand_buffer[5], cvmx_nand_buffer[6],
			       cvmx_nand_buffer[7]);
		cvmx_nand_reset(chip);

		/* Read more than required, as in 16 bit mode only every other
		 * byte is valid.
		 */
		if (cvmx_nand_read_id(chip, 0x20,
				      octeon_ptr_to_phys(cvmx_nand_buffer),
				      8) < 8) {
			if (1)
				printf("%s: Failed to probe chip %d\n",
					     __func__, chip);
			continue;
		}

		if ((cvmx_nand_buffer[0] == 'O') &&
		    (cvmx_nand_buffer[1] == 'N') &&
		    (cvmx_nand_buffer[2] == 'F') &&
		    (cvmx_nand_buffer[3] == 'I')) {
			/* We have an ONFI part, so read the parameter page */
			if (_DEBUG)
				printf("%s: NAND chip %d is ONFI\n",
					     __func__, chip);

			if (cvmx_nand_initialize_onfi(chip, clocks_us)) {
				puts("Error initializing ONFI\n");
				__cvmx_nand_select(nand_selected);
				return -1;
			}
			/* We only support one chip, so break */
			break;
		} else {
			/* We have a non-ONFI part. */
			if (1)
				printf("%s: Chip %d doesn't support ONFI.  ID: 0x%02x 0x%02x\n",
					     __func__, chip, nand_id_buffer[0],
					     nand_id_buffer[1]);
			continue;
#if 0
			switch (nand_id_buffer[0]) {
			case 0xEC: {
				/* We have a Samsung part, so decode part info
				 * from ID bytes
				 */
				uint64_t nand_size_bits =	/* Plane size */
					(64 * 1024 * 1024ULL) <<
						((nand_id_buffer[4] & 0x70) >> 4);
				/* NAND page size in bytes */
				cvmx_nand_state[chip].page_size =
					1024 << (nand_id_buffer[3] & 0x3);
				/* NAND OOB (spare) size in bytes (per page) */
				cvmx_nand_state[chip].oob_size =
					(cvmx_nand_state[chip].page_size / 512) *
						((nand_id_buffer[3] & 4) ? 16 : 8);
				cvmx_nand_state[chip].pages_per_block =
					(0x10000 << ((nand_id_buffer[3] & 0x30) >> 4))
						/ cvmx_nand_state[chip].page_size;

				nand_size_bits *= 1 << ((nand_id_buffer[4] & 0xc) >> 2);

				cvmx_nand_state[chip].oob_size = cvmx_nand_state[chip].page_size / 64;
				if (nand_id_buffer[3] & 0x4)
					cvmx_nand_state[chip].oob_size *= 2;

				cvmx_nand_state[chip].blocks =
					nand_size_bits /
					(8ULL
					 * cvmx_nand_state[chip].page_size
					 * cvmx_nand_state[chip].pages_per_block);
				switch (nand_id_buffer[1]) {
				case 0xD3:	/* K9F8G08U0M */
				case 0xDC:	/* K9F4G08U0B */
					cvmx_nand_state[chip].onfi_timing = 6;
					break;
				default:
					cvmx_nand_state[chip].onfi_timing = 2;
					break;
				}

				if (_DEBUG) {
					printf("%s: Samsung NAND chip detected, using parameters decoded from ID bytes.\n",
						     __func__);
					printf("%s: Defaults: page size: %d, OOB size: %d, pages per block %d, part size: %d MBytes, timing mode: %d\n",
						     __func__,
						     cvmx_nand_state[chip].page_size,
						     cvmx_nand_state[chip].oob_size,
						     cvmx_nand_state[chip].pages_per_block,
						     (int)(nand_size_bits / (8 * 1024 * 1024)),
						     cvmx_nand_state[chip].onfi_timing);
					printf("%s: Address cycles: %d, column bits: %d, row bits: %d, block count: %d\n",
						     __func__,
						     __cvmx_nand_get_address_cycles(chip),
						     __cvmx_nand_get_column_bits(chip),
						     __cvmx_nand_get_row_bits(chip),
						     cvmx_nand_state[chip].blocks);
				}

				__set_onfi_timing_mode(cvmx_nand_state[chip].tim_par,
						       clocks_us,
						       cvmx_nand_state[chip].onfi_timing);
				if (cvmx_nand_state[chip].page_size + cvmx_nand_state[chip].oob_size > CVMX_NAND_MAX_PAGE_AND_OOB_SIZE) {
					printf("%s: ERROR: Page size (%d) + OOB size (%d) is greater than max size (%d)\n",
						     __func__,
						     cvmx_nand_state[chip].page_size,
						     cvmx_nand_state[chip].oob_size,
						     CVMX_NAND_MAX_PAGE_AND_OOB_SIZE);
					__cvmx_nand_select(nand_selected);
					return CVMX_NAND_ERROR;
				}

				/* We have completed setup for this Samsung
				 * chip, so go on to next chip.
				 */
				continue;
			}
			default:
				printf("%s: Unknown non-ONFI NAND device ID %x %x\n",
				       __func__, nand_id_buffer[0],
				       nand_id_buffer[1]);
				break;
			}
#endif
		}

		/*  We were not able to automatically identify the NAND chip
		 * parameters.  If default values were configured, use them.
		 */
		if (cvmx_nand_default.page_size) {
			__set_chip_defaults(chip, clocks_us);
		} else {

			if (_DEBUG)
				printf("%s: Unable to determine NAND parameters, and no defaults supplied.\n",
				       __func__);
		}
	}
	__cvmx_nand_select(nand_selected);
	debug("%s: Success\n", __func__);
	return CVMX_NAND_SUCCESS;
}

/**
 * Call to shutdown the NAND controller after all transactions
 * are done. In most setups this will never be called.
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_shutdown(void)
{
	memset(&cvmx_nand_state, 0, sizeof(cvmx_nand_state));
	return CVMX_NAND_SUCCESS;
}

/**
 * Returns a bitmask representing the chip selects that are
 * connected to NAND chips. This can be called after the
 * initialize to determine the actual number of NAND chips
 * found. Each bit in the response coresponds to a chip select.
 *
 * @return Zero if no NAND chips were found. Otherwise a bit is set for
 *         each chip select (1<<chip).
 */
int cvmx_nand_get_active_chips(void)
{
	int chip;
	int result = 0;
	for (chip = 0; chip < 8; chip++) {
		if (cvmx_nand_state[chip].page_size)
			result |= 1 << chip;
	}
	return result;
}

/**
 * Override the timing parameters for a NAND chip
 *
 * @param chip     Chip select to override
 * @param tim_mult
 * @param tim_par
 * @param clen
 * @param alen
 * @param rdn
 * @param wrn
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_set_timing(int chip, int tim_mult, int tim_par[8],
					int clen[4], int alen[4], int rdn[4],
					int wrn[2])
{
	int i;

	if (!cvmx_nand_state[chip].page_size)
		return CVMX_NAND_INVALID_PARAM;

	cvmx_nand_state[chip].tim_mult = tim_mult;
	for (i = 0; i < 8; i++)
		cvmx_nand_state[chip].tim_par[i] = tim_par[i];
	for (i = 0; i < 4; i++)
		cvmx_nand_state[chip].clen[i] = clen[i];
	for (i = 0; i < 4; i++)
		cvmx_nand_state[chip].alen[i] = alen[i];
	for (i = 0; i < 4; i++)
		cvmx_nand_state[chip].rdn[i] = rdn[i];
	for (i = 0; i < 2; i++)
		cvmx_nand_state[chip].wrn[i] = wrn[i];

	return CVMX_NAND_SUCCESS;
}

/**
 * @INTERNAL
 * Get the number of free bytes in the NAND command queue
 *
 * @return Number of bytes in queue
 */
static inline int __cvmx_nand_get_free_cmd_bytes(void)
{
	union cvmx_ndf_misc ndf_misc;
	ndf_misc.u64 = cvmx_read_csr(CVMX_NDF_MISC);
	return (int)ndf_misc.s.fr_byt;
}

/**
 * Submit a command to the NAND command queue. Generally this
 * will not be used directly. Instead most programs will use the other
 * higher level NAND functions.
 *
 * @param cmd    Command to submit
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_submit(cvmx_nand_cmd_t cmd)
{
	switch (cmd.s.op_code) {
		/* All these commands fit in one 64bit word */
	case 0:		/* NOP */
	case 1:		/* Timing */
	case 2:		/* WAIT */
	case 3:		/* Chip Enable/Disable */
	case 4:		/* CLE */
	case 8:		/* Write */
	case 9:		/* Read */
	case 10:		/* Read EDO */
	case 15:		/* Bus Aquire/Release */
		if (__cvmx_nand_get_free_cmd_bytes() < 8)
			return CVMX_NAND_NO_MEMORY;
		cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[1]);
		break;

	case 5:		/* ALE commands take either one or two 64bit words */
		if (cmd.ale.adr_byte_num < 5) {
			if (__cvmx_nand_get_free_cmd_bytes() < 8)
				return CVMX_NAND_NO_MEMORY;
			cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[1]);
		} else {
			if (__cvmx_nand_get_free_cmd_bytes() < 16)
				return CVMX_NAND_NO_MEMORY;
			cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[1]);
			cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[0]);
		}
		break;

	case 11:		/* Wait status commands take two 64bit words */
		if (__cvmx_nand_get_free_cmd_bytes() < 16) {
			return CVMX_NAND_NO_MEMORY;
		}
		cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[1]);
		cvmx_write_csr(CVMX_NDF_CMD, cmd.u64[0]);
		break;

	default:
		return CVMX_NAND_INVALID_PARAM;
	}

	return CVMX_NAND_SUCCESS;
}

/**
 * @INTERNAL
 * Build the set of command common to most transactions
 * @param chip      NAND chip to program
 * @param cmd_data  NAND command for CLE cycle 1
 * @param num_address_cycles
 *                  Number of address cycles to put on the bus
 * @param nand_address
 *                  Data to be put on the bus. It is translated according to
 *                  the rules in the file information section.
 *
 * @param cmd_data2 If non zero, adds a second CLE cycle used by a number of NAND
 *                  transactions.
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
static inline cvmx_nand_status_t __cvmx_nand_build_pre_cmd(int chip,
							   int cmd_data,
							   int num_address_cycles,
							   uint64_t nand_address,
							   int cmd_data2)
{
	cvmx_nand_status_t result;
	cvmx_nand_cmd_t cmd;

	/* Send timing parameters */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.set_tm_par.one = 1;
	cmd.set_tm_par.tim_mult = cvmx_nand_state[chip].tim_mult;
	/* tim_par[0] unused */
	cmd.set_tm_par.tim_par1 = cvmx_nand_state[chip].tim_par[1];
	cmd.set_tm_par.tim_par2 = cvmx_nand_state[chip].tim_par[2];
	cmd.set_tm_par.tim_par3 = cvmx_nand_state[chip].tim_par[3];
	cmd.set_tm_par.tim_par4 = cvmx_nand_state[chip].tim_par[4];
	cmd.set_tm_par.tim_par5 = cvmx_nand_state[chip].tim_par[5];
	cmd.set_tm_par.tim_par6 = cvmx_nand_state[chip].tim_par[6];
	cmd.set_tm_par.tim_par7 = cvmx_nand_state[chip].tim_par[7];
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Send bus select */

	__cvmx_nand_clear_cmd(&cmd);
	cmd.bus_acq.fifteen = 15;
	cmd.bus_acq.one = 1;
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Send chip select */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.chip_en.chip = chip;
	cmd.chip_en.one = 1;
	cmd.chip_en.three = 3;
	cmd.chip_en.width = 1;
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Send wait, fixed time
	 * This meets chip enable to command latch enable timing.
	 * This is tCS - tCLS from the ONFI spec.
	 * Use tWP as a proxy, as this is adequate for
	 * all ONFI 1.0 timing modes.
	 */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.wait.two = 2;
	cmd.wait.n = 1;
	if (cvmx_nand_submit(cmd))
		return CVMX_NAND_NO_MEMORY;

	/* Send CLE */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.cle.cmd_data = cmd_data;
	cmd.cle.clen1 = cvmx_nand_state[chip].clen[0];
	cmd.cle.clen2 = cvmx_nand_state[chip].clen[1];
	cmd.cle.clen3 = cvmx_nand_state[chip].clen[2];
	cmd.cle.four = 4;
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Send ALE */
	if (num_address_cycles) {
		__cvmx_nand_clear_cmd(&cmd);
		cmd.ale.adr_byte_num = num_address_cycles;
		if (cvmx_nand_state[chip].flags & CVMX_NAND_NUMONYX) {
			nand_address /= cvmx_nand_state[chip].page_size;
			cmd.ale.adr_bytes_l = nand_address;
			cmd.ale.adr_bytes_h = nand_address >> 32;
		} else if (num_address_cycles < __cvmx_nand_get_address_cycles(chip)) {
			cmd.ale.adr_bytes_l = nand_address;
			cmd.ale.adr_bytes_h = nand_address >> 32;
		} else {
			int column_bits = __cvmx_nand_get_column_bits(chip);
			int column_shift = ((column_bits + 7) >> 3) << 3;
			int column = nand_address & (cvmx_nand_state[chip].page_size - 1);
			int row = nand_address >> column_bits;
			cmd.ale.adr_bytes_l = column + (row << column_shift);
			cmd.ale.adr_bytes_h = row >> (32 - column_shift);
		}
		cmd.ale.alen1 = cvmx_nand_state[chip].alen[0];
		cmd.ale.alen2 = cvmx_nand_state[chip].alen[1];
		cmd.ale.alen3 = cvmx_nand_state[chip].alen[2];
		cmd.ale.alen4 = cvmx_nand_state[chip].alen[3];
		cmd.ale.five = 5;
		result = cvmx_nand_submit(cmd);
		if (result)
			return result;
	}

	/* Send CLE 2 */
	if (cmd_data2) {
		__cvmx_nand_clear_cmd(&cmd);
		cmd.cle.cmd_data = cmd_data2;
		cmd.cle.clen1 = cvmx_nand_state[chip].clen[0];
		cmd.cle.clen2 = cvmx_nand_state[chip].clen[1];
		cmd.cle.clen3 = cvmx_nand_state[chip].clen[2];
		cmd.cle.four = 4;
		result = cvmx_nand_submit(cmd);
		if (result)
			return result;
	}

	return CVMX_NAND_SUCCESS;
}

/**
 * @INTERNAL
 * Build the set of command common to most transactions
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
static inline cvmx_nand_status_t __cvmx_nand_build_post_cmd(void)
{
	cvmx_nand_status_t result;
	cvmx_nand_cmd_t cmd;

	/* Send chip deselect */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.chip_dis.three = 3;
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Send bus release */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.bus_rel.fifteen = 15;
	result = cvmx_nand_submit(cmd);
	if (result)
		return result;

	/* Ring the doorbell */
	cvmx_write_csr(CVMX_NDF_DRBELL, 1);
	return CVMX_NAND_SUCCESS;
}

/**
 * @INTERNAL
 * Setup the NAND DMA engine for a transfer
 *
 * @param is_write Non zero if this is a write
 * @param buffer_address
 *                 Physical memory address to DMA to/from
 * @param buffer_length
 *                 Length of the DMA in bytes
 */
static inline void __cvmx_nand_setup_dma(int is_write,
					 uint64_t buffer_address,
					 int buffer_length)
{
	union cvmx_mio_ndf_dma_cfg ndf_dma_cfg;

	ndf_dma_cfg.u64 = 0;
	ndf_dma_cfg.s.en = 1;
	ndf_dma_cfg.s.rw = is_write;	/* One means DMA reads from memory and writes to flash */
	ndf_dma_cfg.s.clr = 0;
	ndf_dma_cfg.s.size = ((buffer_length + 7) >> 3) - 1;
	ndf_dma_cfg.s.adr = buffer_address;
	CVMX_SYNCWS;
	cvmx_write_csr(CVMX_MIO_NDF_DMA_CFG, ndf_dma_cfg.u64);
}

/**
 * @INTERNAL
 * Perform a low level NAND read command
 *
 * @param chip   Chip to read from
 * @param nand_command1
 *               First command cycle value
 * @param address_cycles
 *               Number of address cycles after comand 1
 * @param nand_address
 *               NAND address to use for address cycles
 * @param nand_command2
 *               NAND command cycle 2 if not zero
 * @param buffer_address
 *               Physical address to DMA into
 * @param buffer_length
 *               Length of the transfer in bytes
 *
 * @return Number of bytes transfered or a negative error code
 */
static inline int __cvmx_nand_low_level_read(int chip,
					     int nand_command1,
					     int address_cycles,
					     uint64_t nand_address,
					     int nand_command2,
					     uint64_t buffer_address,
					     int buffer_length)
{
	cvmx_nand_cmd_t cmd;
	union cvmx_mio_ndf_dma_cfg ndf_dma_cfg;
	int bytes;
	int nand_selected;
	int status = CVMX_NAND_ERROR;
	unsigned int start;

	nand_selected = __cvmx_nand_select(1);

	/* Build the command and address cycles */
	if ((status = __cvmx_nand_build_pre_cmd(chip, nand_command1,
						address_cycles, nand_address,
						nand_command2))) {
		debug("%s: Error building pre command\n", __func__);
		goto error;
	}
	/* Send WAIT.  This waits for some time, then
	 * waits for busy to be de-asserted.
	 */
	if ((status = __wait_for_busy_done())) {
		debug("%s: Error waiting for busy done\n", __func__);
		goto error;
	}

	/* Wait for tRR after busy de-asserts.
	 * Use 2* tALS as proxy.  This is overkill in
	 * the slow modes, but not bad in the faster ones.
	 */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.wait.two = 2;
	cmd.wait.n = 4;
	if ((status = cvmx_nand_submit(cmd))) {
		debug("%s: submit error\n", __func__);
		goto error;
	}

	if ((status = cvmx_nand_submit(cmd))) {
		debug("%s: submit error 2\n", __func__);
		goto error;
	}

	/* Send READ */
	__cvmx_nand_clear_cmd(&cmd);
	cmd.rd.data_bytes = buffer_length;
	if (cvmx_nand_state[chip].onfi_timing >= 4
	    && cvmx_nand_state[chip].onfi_timing <= 6)
		/* READ_EDO command is required for ONFI timing modes 4 and 5 */
		cmd.rd.nine = 10;
	else
		cmd.rd.nine = 9;
	cmd.rd.rdn1 = cvmx_nand_state[chip].rdn[0];
	cmd.rd.rdn2 = cvmx_nand_state[chip].rdn[1];
	cmd.rd.rdn3 = cvmx_nand_state[chip].rdn[2];
	cmd.rd.rdn4 = cvmx_nand_state[chip].rdn[3];
	if ((status = cvmx_nand_submit(cmd))) {
		debug("%s: submit error 3\n", __func__);
		goto error;
	}

	__cvmx_nand_setup_dma(0, buffer_address, buffer_length);

	if ((status = __cvmx_nand_build_post_cmd())) {
		debug("%s: post cmd error\n", __func__);
		goto error;
	}

	/* Wait for the DMA to complete */
	start = get_timer(0);
	do {
		ndf_dma_cfg.u64 = cvmx_read_csr(CVMX_MIO_NDF_DMA_CFG);
		if (ndf_dma_cfg.s.en == 0)
			break;
	} while (get_timer(start) < NAND_TIMEOUT_USECS_READ);
	if (ndf_dma_cfg.s.en) {
		status = CVMX_NAND_TIMEOUT;
		debug("%s: DMA timeout\n", __func__);
		goto error;
	}
	/* Return the number of bytes transfered */
	ndf_dma_cfg.u64 = cvmx_read_csr(CVMX_MIO_NDF_DMA_CFG);
	bytes = ndf_dma_cfg.s.adr - buffer_address;
#ifdef DEBUG
	if (_DEBUG)
		__cvmx_nand_hex_dump(octeon_phys_to_ptr(buffer_address), bytes);
#endif
	__cvmx_nand_select(nand_selected);
	return bytes;

error:
	debug("%s: Error %d\n", __func__, status);
	__cvmx_nand_select(nand_selected);
	return status;
}

/**
 * Read a page from NAND. If the buffer has room, the out of band
 * data will be included.
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               Location in NAND to read. See description in file comment
 * @param buffer_address
 *               Physical address to store the result at
 * @param buffer_length
 *               Number of bytes to read
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *	   failure
 */
int cvmx_nand_page_read(int chip, uint64_t nand_address,
			uint64_t buffer_address, int buffer_length)
{
	int bytes;
	int command2;

	if (!cvmx_nand_state[chip].page_size)
		return CVMX_NAND_INVALID_PARAM;

	command2 = NAND_COMMAND_READ_FIN;

	bytes = __cvmx_nand_low_level_read(chip, NAND_COMMAND_READ,
					   __cvmx_nand_get_address_cycles(chip),
					   nand_address, command2,
					   buffer_address, buffer_length);
	return bytes;
}

/**
 * Read random data from NAND.  This adjusts the column address before starting
 * a read operation.
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               Location in NAND to read.  NOTE: only the column bits are used.
 * @param buffer_address
 *               Physical address to store the result at
 * @param buffer_length
 *               Number of bytes to read
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on
 *	   failure
 */
int cvmx_nand_random_data_out(int chip, uint64_t nand_address,
			      uint64_t buffer_address, int buffer_length)
{
	int bytes, column_bits, column_mask;

	column_bits = __cvmx_nand_get_column_bits(chip);
	column_mask = (1 << column_bits) - 1;

	bytes = __cvmx_nand_low_level_read(chip, NAND_COMMAND_RNDOUT,
					   (column_bits + 7) / 8,
					   nand_address & column_mask,
					   NAND_COMMAND_RNDOUT_START,
					   buffer_address, buffer_length);

	return bytes;
}

/**
 * Read the NAND ID information
 *
 * @param chip   Chip select for NAND flash
 * @param nand_address
 *               NAND address to read ID from. Usually this is either 0x0 or 0x20.
 * @param buffer_address
 *               Physical address to store data in
 * @param buffer_length
 *               Length of the buffer. Usually this is 4-8 bytes.  For 16 bit mode, this must be twice
 *               as large as the actual expected data.
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code on failure
 */
int cvmx_nand_read_id(int chip, uint64_t nand_address, uint64_t buffer_address,
		      int buffer_length)
{
	int bytes;

	bytes = __cvmx_nand_low_level_read(chip, NAND_COMMAND_READ_ID, 1,
					   nand_address, 0, buffer_address,
					   buffer_length);
	return bytes;
}

/**
 * Read the NAND parameter page
 *
 * @param chip   Chip select for NAND flash
 * @param buffer_address
 *               Physical address to store data in
 * @param buffer_length
 *               Length of the buffer.  Usually 1024 bytes for 8 bit, 2048
 *		 for 16 bit mode.
 *
 * @return Bytes read on success, a negative cvmx_nand_status_t error code
 *	   on failure
 */
int cvmx_nand_read_param_page(int chip, uint64_t buffer_address,
			      int buffer_length)
{
	int bytes;

	debug("%s(%d, 0x%lx, %d)\n", __func__, chip, buffer_address,
	      buffer_length);
	bytes = __cvmx_nand_low_level_read(chip, NAND_COMMAND_READ_PARAM_PAGE,
					   1, 0x0, 0,
					   buffer_address, buffer_length);
	return bytes;
}

/**
 * Get the status of the NAND flash
 *
 * @param chip   Chip select for NAND flash
 *
 * @return NAND status or a negative cvmx_nand_status_t error code on failure
 */
static int cvmx_nand_get_status(int chip)
{
	int status;

	*((uint8_t *) cvmx_nand_buffer) = 0xff;
	status = __cvmx_nand_low_level_read(chip, NAND_COMMAND_STATUS, 0, 0, 0,
					    octeon_ptr_to_phys(cvmx_nand_buffer),
					    8);
	if (status > 0)
		status = *((uint8_t *) cvmx_nand_buffer);

	return status;
}

/**
 * Gets the specified feature number
 *
 * @param chip     Chip select for NAND flash
 * @param feat_num Feature number to get
 * @param feature  P1 - P4 of the feature data
 *
 * @return cvmx_nand_status_t error code
 */
cvmx_nand_status_t cvmx_nand_get_feature(int chip, uint8_t feat_num,
					 uint8_t feature[4])
{
	int status;

	if (feature == NULL)
		return CVMX_NAND_INVALID_PARAM;

	status = __cvmx_nand_low_level_read(chip, NAND_COMMAND_GET_FEATURES, 1,
					    feat_num, 0,
					    octeon_ptr_to_phys(cvmx_nand_buffer),
					    8);
	if (status > 0) {
		memcpy(feature, cvmx_nand_buffer, 4);
		return CVMX_NAND_SUCCESS;
	}
	return status;
}

/**
 * Sets the specified feature number
 *
 * @param chip     Chip select for NAND flash
 * @param feat_num Feature number to get
 * @param feature  P1 - P4 of the feature data
 *
 * @return cvmx_nand_status_t error code
 */
static cvmx_nand_status_t cvmx_nand_set_feature(int chip, uint8_t feat_num,
						const uint8_t feature[4])
{
	cvmx_nand_status_t status = CVMX_NAND_SUCCESS;
	union cvmx_mio_ndf_dma_cfg ndf_dma_cfg;
	cvmx_nand_cmd_t cmd;
	int nand_selected;
	unsigned int start;

	if (feature == NULL)
		return CVMX_NAND_INVALID_PARAM;

	nand_selected = __cvmx_nand_select(1);
	if ((status = __cvmx_nand_build_pre_cmd(chip, NAND_COMMAND_SET_FEATURES,
						1, feat_num, 0)))
		goto done;

	memcpy(cvmx_nand_buffer, feature, 4);
	memset(cvmx_nand_buffer + 4, 0, 4);

	__cvmx_nand_clear_cmd(&cmd);
	cmd.wr.data_bytes = 8;
	cmd.wr.eight = 8;
	cmd.wr.wrn1 = cvmx_nand_state[chip].wrn[0];
	cmd.wr.wrn2 = cvmx_nand_state[chip].wrn[1];
	if ((status = cvmx_nand_submit(cmd)))
		goto done;

	__cvmx_nand_setup_dma(1, octeon_ptr_to_phys(cvmx_nand_buffer), 8);

	if ((status = __wait_for_busy_done()))
		goto done;

	if ((status = __cvmx_nand_build_post_cmd()))
		goto done;

	start = get_timer(0);
	do {
		ndf_dma_cfg.u64 = cvmx_read_csr(CVMX_MIO_NDF_DMA_CFG);
		if (ndf_dma_cfg.s.en == 0)
			break;
	} while (get_timer(start) < NAND_TIMEOUT_USECS_WRITE / 1000);
	if (ndf_dma_cfg.s.en) {
		status = CVMX_NAND_TIMEOUT;
		goto done;
	}
done:
	__cvmx_nand_select(nand_selected);
	return status;
}

/**
 * Get the page size, excluding out of band data. This  function
 * will return zero for chip selects not connected to NAND.
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Page size in bytes or a negative cvmx_nand_status_t error code
 *	   on failure
 */
static int cvmx_nand_get_page_size(int chip)
{
	return cvmx_nand_state[chip].page_size;
}

/**
 * Reset the NAND flash
 *
 * @param chip   Chip select for NAND flash
 *
 * @return Zero on success, a negative cvmx_nand_status_t error code on failure
 */
cvmx_nand_status_t cvmx_nand_reset(int chip)
{
	cvmx_nand_status_t status = CVMX_NAND_SUCCESS;
	int nand_selected;

	if (!cvmx_nand_state[chip].page_size)
		return CVMX_NAND_INVALID_PARAM;

	nand_selected = __cvmx_nand_select(1);
	if ((status = __cvmx_nand_build_pre_cmd(chip, NAND_COMMAND_RESET,
						0, 0, 0)))
		goto done;


	/* WAIT for R_B to signal reset is complete  */
	if ((status = __wait_for_busy_done()))
		goto done;

	if ((status = __cvmx_nand_build_post_cmd()))
		goto done;
done:
	__cvmx_nand_select(nand_selected);
	return status;
}

cvmx_nand_status_t cvmx_nand_set_defaults(int page_size, int oob_size,
					  int pages_per_block, int blocks,
					  int onfi_timing_mode)
{
	if (!page_size || !oob_size || !pages_per_block || !blocks
	    || (unsigned)onfi_timing_mode > sizeof(onfi_speed_modes)/sizeof(onfi_speed_modes[0]))
		return CVMX_NAND_INVALID_PARAM;

	cvmx_nand_default.page_size = page_size;
	cvmx_nand_default.oob_size = oob_size;
	cvmx_nand_default.pages_per_block = pages_per_block;
	cvmx_nand_default.blocks = blocks;
	cvmx_nand_default.onfi_timing = onfi_timing_mode;

	return CVMX_NAND_SUCCESS;
}

/***********************/
/* High Level Routines */
/***********************/

static int octeon_nand_bch_calculate_ecc_internal(struct octeon_nand_chip *chip,
						  const unsigned char *buf,
						  unsigned char *code);
uint8_t octeon_read_byte(struct octeon_nand_chip *chip)
{

	debug("%s: offset: 0x%x\n", __func__, chip->index);
	if (chip->use_status)
		return chip->status;

	if (chip->index < chip->data_len) {
		return chip->buffers.databuf[chip->index++];
	} else {
		printf("error: No data to read\n");
		return 0xff;
	}
}

void octeon_read_buf(struct octeon_nand_chip *chip, u8 *buf, int len)
{
	if (len <= chip->data_len - chip->index) {
		debug("%s: Copying %d bytes from %p to %p, index: %d\n",
		      __func__, len, chip->buffers.databuf + chip->index, buf,
		      chip->index);
		memcpy(buf, chip->buffers.databuf + chip->index, len);
		chip->index += len;
	} else {
		printf("%s: Not enough data for read of %d bytes, data_len: %d, index: %d\n.\n",
		       __func__, len, chip->data_len, chip->index);
		chip->data_len = 0;
	}
}


/**
 * Calculate the ECC OOC layout
 *
 * @param chip	Chip to calculate layout for
 *
 * @return 0 for success, otherwise failure
 */
static int octeon_nand_calc_ecc_layout(struct octeon_nand_chip *chip)
{
	struct nand_ecclayout *layout = &chip->layout;
	uint16_t oobsize = cvmx_nand_get_oob_size(chip->selected_chip);
	unsigned i;

	debug("%s(%p)\n", __func__, chip);
	memset(layout, 0, sizeof(*layout));
	chip->ecc.layout = layout;
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

	debug("  layout eccbytes: %d, free offset: %d, free length: %d\n",
	      layout->eccbytes, layout->oobfree[0].offset,
	      layout->oobfree[0].length);
	return 0;

fail:
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
static int octeon_nand_hw_bch_init(struct octeon_nand_chip *chip)
{
	unsigned int i;
	unsigned char *erased_page = NULL;
	unsigned int eccsize = chip->ecc.size;
	unsigned int eccbytes = chip->ecc.bytes;
	int rc;
	uint8_t *erased_ecc = NULL;
	static char bch_initialized = 0;

	debug("%s(%p) ENTER\n", __func__, chip);
	debug("  ecc size: %u, ecc bytes: %u\n", chip->ecc.size,
	      chip->ecc.bytes);
	erased_ecc = malloc(eccbytes);
	debug("%s: erased_ecc: %p\n", __func__, erased_ecc);
	if (!erased_ecc) {
		printf("%s: Out of memory\n", __func__);
		goto fail;
	}
	if (octeon_nand_calc_ecc_layout(chip)) {
		printf("%s: Error calculating ECC layout\n", __func__);
		goto fail;
	}

	if (!bch_initialized) {
		debug("  Initializing bch\n");
		rc = octeon_bch_initialize();
		if (rc) {
			printf("%s: Error initializing BCH subsystem\n", __func__);
			goto fail;
		}
	}
	bch_initialized = 1;

	/* Compute and store the inverted ecc of an erased ecc block */
	debug("  ecc size: %u\n", eccsize);
	erased_page = malloc(eccsize);
	debug("%s: erased_page: %p\n", __func__, erased_page);
	if (!erased_page) {
		printf("%s: Out of memory\n", __func__);
		goto fail;
	}
	memset(erased_page, 0xff, eccsize);
	memset(chip->eccmask, 0, eccbytes);
	memset(erased_ecc, 0, eccbytes);

	debug("%s: Calculating internal ECC\n", __func__);
	if (octeon_nand_bch_calculate_ecc_internal(chip, erased_page,
						   erased_ecc)) {
		printf("%s: Calculating ECC failed\n", __func__);
		goto fail;
	}

	debug("%s: Freeing erased page %p\n", __func__, erased_page);
	free(erased_page);

#ifdef DEBUG
	debug("calculated ecc for erased page:\n");
	print_buffer(0, erased_ecc, 1, eccbytes, 0);
#endif
	for (i = 0; i < eccbytes; i++)
		chip->eccmask[i] = erased_ecc[i] ^ 0xff;

	free(erased_ecc);
#ifdef DEBUG
	debug("ecc mask:\n");
	print_buffer(0, chip->eccmask, 1, eccbytes, 0);
	debug("%s: EXIT (0)\n", __func__);
#endif
	return 0;

fail:
	if (erased_ecc)
		free(erased_ecc);
	if (erased_page)
		free(erased_page);
	if (bch_initialized)
		octeon_bch_shutdown();
	debug("%s: EXIT (-1)\n", __func__);
	return -1;
}

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
 * @INTERNAL
 * Given a page, calculate the ECC code
 *
 * @param chip	Pointer to NAND chip data structure
 * @param buf	Buffer to calculate ECC on
 * @param[out] code	Buffer to hold ECC data
 *
 * @return 0 for success or -1 on failure
 */
static int octeon_nand_bch_calculate_ecc_internal(struct octeon_nand_chip *chip,
						  const unsigned char *buf,
						  unsigned char *code)
{
	static volatile octeon_bch_response_t response;
	int rc;
	int i;
	unsigned int start;
	void *ecc_ptr;
	uint8_t *ecc_buffer = NULL;

	debug("%s(%p, %p, %p)\n", __func__, chip, buf, code);
	if ((ulong)code % 8) {
		debug("Using ECC bounce buffer of %d bytes\n", chip->ecc.bytes);
		ecc_buffer = malloc(chip->ecc.bytes);
		if (!ecc_buffer) {
			printf("%s: Out of memory\n", __func__);
			return -1;
		}
		ecc_ptr = ecc_buffer;
	} else
		ecc_ptr = code;

	memset((void *)&response, 0, sizeof(response));
#ifdef DEBUG
	debug("pointers: code: %p, ecc_ptr: %p\n", code, ecc_ptr);
	memset(ecc_ptr, 0, chip->ecc.bytes);
#endif
	debug("Calling cvmx_bch_encode...\n");

	rc = octeon_bch_encode(buf, chip->ecc.size, chip->ecc.strength, ecc_ptr,
			       &response);
	debug("Done, rc: %d\n", rc);
	if (rc) {
		printf("octeon_bch_encode failed\n");
		goto fail;
	}
	start = get_timer(0);
	while (!response.done && get_timer(start) <  1000)
		;

	debug("%s: status done: %d, uncorrectable: %d, erased: %d, num errors: %d\n",
	      __func__, response.done, response.uncorrectable, response.erased,
	      response.num_errors);
#ifdef DEBUG
	debug("buffer:\n");
	print_buffer(0, buf, 1, chip->ecc.size, 0);
	debug("ecc:\n");
	print_buffer(0, ecc_ptr, 1, chip->ecc.bytes, 0);
#endif
	if (!response.done) {
		printf("octeon_bch_encode timed out, response done: %d, uncorrectable: %d, num_errors: %d, erased: %d\n",
		       response.done, response.uncorrectable,
		       response.num_errors, response.erased);
		octeon_bch_shutdown();
		octeon_bch_initialize();
		goto fail;
	}
	if ((ulong)code % 8) {
		debug("  Copying bounce buffer\n");
		memcpy(code, ecc_buffer, chip->ecc.bytes);
	}

	for (i = 0; i < chip->ecc.bytes; i++)
		code[i] ^= chip->eccmask[i];

	if (ecc_buffer)
		free(ecc_buffer);
	debug("%s: EXIT (0)\n", __func__);
	return 0;
fail:
	if (ecc_buffer)
		free(ecc_buffer);
	return -1;
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
static int octeon_nand_bch_correct(struct octeon_nand_chip *chip,
				   unsigned char *dat,
				   unsigned char *read_ecc)
{
	static volatile octeon_bch_response_t response;
	int rc;
	unsigned int start, stop;
	int i;
	static uint8_t data_buffer[2048 + 128];


	debug("%s(%p, %p, %p)\n", __func__, chip, dat, read_ecc);

	memcpy(data_buffer, dat, chip->ecc.size);
	memcpy(data_buffer + chip->ecc.size, read_ecc, chip->ecc.bytes);
#ifdef DEBUG
	debug("ECC mask:\n");
	print_buffer(0, chip->eccmask, 1, chip->ecc.bytes, 0);
#endif
	for (i = 0; i < chip->ecc.bytes; i++)
		data_buffer[chip->ecc.size + i] ^= chip->eccmask[i];

	response.u16 = 0;
#ifdef DEBUG
	printf("buffer at %p:\n", data_buffer);
	print_buffer(0, data_buffer, 1, chip->ecc.size, 0);
	debug("ecc:\n");
	print_buffer(0, data_buffer + chip->ecc.size, 1, chip->ecc.bytes, 0);
	debug("Calling cvmx_bch_decode...\n");
#endif
	rc = octeon_bch_decode(data_buffer, chip->ecc.size, chip->ecc.strength,
			       dat, &response);
	if (rc) {
		printf("%s: cvmx_bch_decode failed\n", __func__);
		return -1;
	}
	stop = start = get_timer(0);
	while (!response.done && (stop = get_timer(start)) < 1000)
		udelay(1);

	debug("%s: response done: %d, uncorrectable: %d, erased: %d, num_errors: %d, time: %u\n",
	       __func__, response.done, response.uncorrectable, response.erased,
	       response.num_errors, stop - start);
#ifdef DEBUG
	debug("ecc:\n");
	print_buffer(0, read_ecc, 1, chip->ecc.bytes, 0);
	debug("buffer:\n");
	print_buffer(0, dat, 1, chip->ecc.size, 0);
#endif
	if (!response.done) {
		printf("%s: Error: BCH engine timeout\n", __func__);
		return -1;
	}
	if (response.erased) {
		debug("%s: NAND block is erased\n", __func__);
		return 0;
	}
	if (response.uncorrectable) {
		debug("%s: Cannot correct NAND block\n", __func__);
		return -1;
	}

	return response.num_errors;
}

/**
 * Reads an entire page from the current chip into the buffer after performing
 * error correction.
 *
 * @param chip		Chip data structure
 * @param buf		Pointer to buffer to read page into
 * @param oob_required	Currently not used, ignored.
 * @param page		Page number to read
 *
 * @return 0 for success, -1 if ECC failed, -2 if block is bad.
 */
int octeon_nand_hw_bch_read_page(struct octeon_nand_chip *chip,
				 uint8_t *buf,
				 int oob_required, int page)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_code = chip->buffers.ecccode;
	uint32_t *eccpos = chip->ecc.layout->eccpos;

	debug("%s(%p, %p, %d, %d)\n", __func__, chip, buf,
	      oob_required, page);

	/* According to the ONFI specification the first and last page within
	 * an erase block will have the first byte of the OOB space set to zero
	 * In our case we check both of these whenever we come across the first
	 * page in an erase block.
	 */
	if (page % chip->pages_per_block == 0) {
		debug("Checking for bad block\n");
		/* Check if block is bad */
		octeon_cmdfunc(chip, NAND_CMD_READ0, 0,
			       page + chip->pages_per_block - 1);
		if (octeon_nand_block_is_bad(chip)) {
			printf("Page %d: Block is bad\n", page);
			return -2;
		}
	}
	octeon_cmdfunc(chip, NAND_CMD_READ0, 0, page);
	if (page % chip->pages_per_block == 0) {
		debug("Checking for bad block\n");
		if (octeon_nand_block_is_bad(chip)) {
			printf("Page %d: Block is bad\n", page);
			return -2;
		}
	}
	octeon_read_buf(chip, buf, chip->writesize);
	octeon_read_buf(chip, chip->oob_poi, chip->oobsize);

	for (i = 0; i < chip->ecc.total; i++)
		ecc_code[i] = chip->oob_poi[eccpos[i]];
#ifdef DEBUG
	printf("pos:\n");
	print_buffer(0, eccpos, 4, chip->ecc.total, 0);
	printf("ecc code:\n");
	print_buffer(0, ecc_code, 1, chip->ecc.total, 0);
#endif

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		int stat;

		debug("%s: Correcting block offset %ld, ecc offset %d\n",
		      __func__, p - buf, i);
		stat = octeon_nand_bch_correct(chip, p, &ecc_code[i]);
		if (stat < 0) {
			printf("Cannot correct NAND page %d\n", page);
			return -1;
		}
	}
	chip->index += chip->writesize + chip->oobsize;
	return 0;
}

#ifndef cmdprint
# ifdef DEBUG
#  define cmdprint printf
# else
#  define cmdprint(...)
# endif
#endif
void octeon_cmdfunc(struct octeon_nand_chip *chip, unsigned command, int column,
		    int page_addr)
{
	cmdprint("%s called, cmd: 0x%x, col: 0x%x, page_addr: 0x%x\n",
		 __func__, command, column, page_addr);

	int status;
	int buffer_length;

	chip->use_status = 0;

	switch (command) {
	case NAND_CMD_READID:
		cmdprint("READID, chip: %d, data_buf: %p, data_addr: 0x%llx\n",
			 chip->selected_chip, chip->buffers.databuf,
			 octeon_ptr_to_phys(chip->buffers.databuf));
		/* Read length must be a multiple of 8, so read a little more
		 * than we require.
		 */
		chip->data_len = cvmx_nand_read_id(chip->selected_chip, column,
						   octeon_ptr_to_phys(chip->buffers.databuf),
						   16);
		if (chip->data_len < 16) {
			cmdprint("READID failed with %d\n", chip->data_len);
			chip->data_len = 0;
		}
		status = cvmx_nand_reset(chip->selected_chip);
		if (status) {
			cmdprint("RESET failed with %d\n", status);
		}
		break;

	case NAND_CMD_READOOB:
		cmdprint("READOOB page_addr=0x%x\n", page_addr);
		chip->index = 8;
		/* Read length must be a multiple of 8, so we start reading
		 * 8 bytes from the end of page.
		 */
		chip->data_len = cvmx_nand_page_read(chip->selected_chip,
						     (page_addr << chip->page_shift) +
						     chip->pagesize -
						     chip->index,
						     octeon_ptr_to_phys(chip->buffers.databuf),
						     chip->oobsize +
						     chip->index);
		if (chip->data_len < chip->oobsize + chip->index) {
			cmdprint("READOOB failed with %d\n", chip->data_len);
			chip->data_len = 0;
		}
		debug("  data length: %d\n", chip->data_len);
		break;

	case NAND_CMD_READ0:
		cmdprint("READ0 page_addr: 0x%x, column: 0x%x, data: 0x%p\n",
			 page_addr, column, chip->buffers.databuf);
		/* Make sure buffer length is rounded up to multiple of 8 */
		buffer_length = chip->pagesize + chip->oobsize;
		buffer_length = (buffer_length + 7) & ~7;
		/* Here mtd->oobsize _must_ already be a multiple of 8 */
#ifdef DEBUG
		memset(chip->buffers.databuf, 0xee, buffer_length);
#endif
		chip->data_len = cvmx_nand_page_read(chip->selected_chip,
						     column +
						     (page_addr << chip->page_shift),
						     octeon_ptr_to_phys(chip->buffers.databuf),
						     buffer_length);
		if (chip->data_len < chip->pagesize + chip->oobsize) {
			cmdprint("READ0 failed with %d\n", chip->data_len);
			chip->data_len = 0;
		}
		chip->selected_page = page_addr;
		chip->index = 0;
#ifdef DEBUG
		printf("data at %p:\n", chip->buffers.databuf);
		print_buffer(page_addr << chip->page_shift, chip->buffers.databuf,
			     1, chip->pagesize, 0);
		printf("OOB: (%d bytes)\n", chip->oobsize);
		print_buffer(0, chip->buffers.databuf + chip->pagesize,
			     1, chip->oobsize, 0);
#endif
		cmdprint("READ0 length %d\n", chip->data_len);
		break;

	case NAND_CMD_STATUS:
		cmdprint("cmdfunc: STATUS\n");
		chip->status = cvmx_nand_get_status(chip->selected_chip);
		chip->use_status = 1;
		break;

	case NAND_CMD_RESET:
		cmdprint("RESET\n");
		chip->index = 0;
		chip->data_len = 0;
		memset(chip->buffers.databuf, 0xff, sizeof(chip->buffers.databuf));
		status = cvmx_nand_reset(chip->selected_chip);
		if (status) {
			cmdprint("RESET failed with %d\n", status);
		}
		break;

	case NAND_CMD_RNDOUT:
		/* Don't do anything. */
		cmdprint("RNDOUT column %d\n", column);
		chip->index = column;
		break;

	case NAND_CMD_PARAM:
		cmdprint("PARAM\n");
		unsigned param_size = chip->pagesize;
		chip->data_len =
			cvmx_nand_read_param_page(chip->selected_chip,
						  octeon_ptr_to_phys(chip->buffers.databuf),
						  param_size);
#ifdef DEBUG
		debug("parameter page:\n");
		print_buffer(0, chip->buffers.databuf, 1, param_size, 0);
#endif
		chip->index = column;
		break;

	default:
		printf("Octeon NAND octeon_cmdfunc: unsupported command 0x%x\n",
		       command);
		break;
	}

	cmdprint("\n");
	return;
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

/*
 * Check if the NAND chip is ONFI compliant, returns 1 if it is, 0 otherwise.
 */
static int nand_flash_detect_onfi(struct octeon_nand_chip *chip)
{
	struct nand_onfi_params *p = &chip->onfi_params;
	int i;
	int val;

	/* Try ONFI for unknown chip or LP */
	i = cvmx_nand_read_id(chip->selected_chip, 0x20,
			      octeon_ptr_to_phys(chip->buffers.databuf), 16);
	if (i < 16) {
		puts("Error reading ID\n");
		return -1;
	}
	if (octeon_read_byte(chip) != 'O' || octeon_read_byte(chip) != 'N' ||
	    octeon_read_byte(chip) != 'F' || octeon_read_byte(chip) != 'I')
		return 0;

	octeon_cmdfunc(chip, NAND_CMD_PARAM, 0, -1);
	for (i = 0; i < 3; i++) {
		octeon_read_buf(chip, (uint8_t *)p, sizeof(*p));
		if (octeon_onfi_crc16(ONFI_CRC_BASE, (uint8_t *)p, 254) ==
				      le16_to_cpu(p->crc)) {
			debug("ONFI param page %d valid\n", i);
			break;
		}
	}

	if (i == 3)
		return 0;

	/* Check version */
	val = le16_to_cpu(p->revision);
	if (val & (1 << 9))
		chip->onfi_version = 40;
	if (val & (1 << 8))
		chip->onfi_version = 32;
	else if (val & (1 << 7))
		chip->onfi_version = 31;
	else if (val & (1 << 6))
		chip->onfi_version = 30;
	else if (val & (1 << 5))
		chip->onfi_version = 23;
	else if (val & (1 << 4))
		chip->onfi_version = 22;
	else if (val & (1 << 3))
		chip->onfi_version = 21;
	else if (val & (1 << 2))
		chip->onfi_version = 20;
	else if (val & (1 << 1))
		chip->onfi_version = 10;
	else
		chip->onfi_version = 0;

	if (!chip->onfi_version) {
		debug("%s: unsupported ONFI version: %d\n", __func__, val);
		return 0;
	}

	chip->writesize = le32_to_cpu(p->byte_per_page);
	chip->pages_per_block = le32_to_cpu(p->pages_per_block);
	chip->erasesize = chip->pages_per_block * chip->writesize;
	chip->oobsize = le16_to_cpu(p->spare_bytes_per_page);
	chip->chipsize = le32_to_cpu(p->blocks_per_lun);
	chip->chipsize *= (uint64_t)chip->erasesize * p->lun_count;
	chip->badblockpos = chip->pagesize;
	chip->num_onfi_pages = p->num_param_pages ? p->num_param_pages : 3;

	debug("ONFI flash detected\n"
	      "  writesize: %u\n"
	      "  erasesize: %u\n"
	      "  oob size:  %u\n"
	      "  chip size: %lu\n"
	      "  parameter pages: %d\n",
	     chip->writesize, chip->erasesize, chip->oobsize, chip->chipsize,
	     chip->num_onfi_pages);
	return 1;
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
static struct nand_extended_param_page_hdr *
calc_next_ext_page(struct octeon_nand_chip *chip,
		   struct nand_extended_param_page_hdr *hdr,
		   int *offset)
{
	uint8_t *ptr = (uint8_t *)(hdr + 1);
	*offset += sizeof(*hdr);
	while (*offset < (int)(chip->pagesize - sizeof(*hdr))) {
		hdr = (struct nand_extended_param_page_hdr *)ptr;
		if (hdr->sig[0] == 'E' &&
		    hdr->sig[1] == 'P' &&
		    hdr->sig[2] == 'P' &&
		    hdr->sig[3] == 'S')
			return hdr;
		*offset += 8;
		ptr += 8;
	}
	printf("%s: Extended parameter page not found\n", __func__);
	return NULL;
}

/**
 * Reads the extended parameter page looking for ECC data
 *
 * @param chip - NAND chip data structure
 *
 * @returns 0 for success, -1 if invalid or unavailable extended parameter page
 */
static int octeon_read_extended_parameters(struct octeon_nand_chip *chip)
{
	struct nand_extended_param_page_hdr *hdr;
	struct nand_extended_ecc_info *ecc_info;
	int size;
	int i;
	int offset;

	size = chip->pagesize;
	debug("%s: ENTER, size: %d\n", __func__, size);
	if (cvmx_nand_read_param_page(chip->selected_chip,
				      octeon_ptr_to_phys(chip->buffers.databuf),
				      size) != size) {
		printf("%s: Could not read extended parameters from NAND chip %d\n",
		       __func__, chip->selected_chip);
		return -1;
	}

	offset = sizeof(cvmx_nand_onfi_param_page_t) * chip->num_onfi_pages;
	hdr = (struct nand_extended_param_page_hdr *)&chip->buffers.databuf[offset];

	debug("  Start offset: %d\n", offset);
	/* Look for a valid header */
	do {
		size = calc_ext_param_page_size(hdr);
		if (size < (int)sizeof(*hdr))
			continue;
		debug("Extended parameter hdr: %p, offset: %d, size: %d\n",
		      hdr, offset, size);
		if (octeon_onfi_crc16(ONFI_CRC_BASE,
				      (uint8_t *)hdr->sig, size - 2) ==
							le16_to_cpu(hdr->crc))
			break;
		hdr = calc_next_ext_page(chip, hdr, &offset);
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

	chip->ecc.strength = octeon_nand_bch_strength_adjust(ecc_info->ecc_bits);
	chip->ecc.size = 1 << ecc_info->ecc_size;
	if (chip->ecc.size > 2048 || chip->ecc.strength < 0) {
		printf("ECC size of %d or strength %d not supported\n",
		       ecc_info->ecc_bits, chip->ecc.size);
		return -1;
	}
	debug("%s: ecc strength: %d, ecc size: %d\n", __func__,
	      chip->ecc.strength, chip->ecc.size);

	return 0;
}

/**
 * Read ONFI data and fill in various chip parameters from it
 *
 * @param chip chip data structure to fill in
 *
 * @return 0 for success, -1 on error
 */
static int octeon_nand_scan_onfi(struct octeon_nand_chip *chip)
{
	cvmx_nand_onfi_param_page_t *onfi_params;
	static const uint8_t revision_decode[17] =
		{0, 0, 10, 20, 21, 22, 23, 30, 31, 32, 40, 0, 0, 0, 0, 0, 0};

	if ((chip->data_len = cvmx_nand_read_id(chip->selected_chip, 0x20,
						octeon_ptr_to_phys(chip->buffers.databuf),
						8)) < 8) {
		printf("ONFI detection failed for chip %d\n",
		      chip->selected_chip);
		chip->data_len = 0;
		return -1;
	}
	cvmx_nand_reset(chip->selected_chip);

	if (chip->buffers.databuf[0] != 'O' ||
	    chip->buffers.databuf[1] != 'N' ||
	    chip->buffers.databuf[2] != 'F' ||
	    chip->buffers.databuf[3] != 'I') {
		printf("ONFI not supported for chip %d\n", chip->selected_chip);
		return -1;
	}
	debug("Reading ONFI data\n");
	if (cvmx_nand_read_param_page(chip->selected_chip,
				      octeon_ptr_to_phys(chip->buffers.databuf),
				      256 * 3) < 256 * 3) {
		printf("%s: Error reading ONFI parameter data for chip %d\n",
		       __func__, chip->selected_chip);
		return -1;
	}
#ifdef DEBUG
	printf("ONFI data:\n");
	print_buffer(0, chip->buffers.databuf, 1, 256 * 3, 0);
#endif
	onfi_params = cvmx_nand_onfi_process((cvmx_nand_onfi_param_page_t *)
					     chip->buffers.databuf);
	if (!onfi_params) {
		printf("%s: Invalid ONFI parameter data for chip %d\n",
			__func__, chip->selected_chip);
		return -1;
	}

	memcpy(&chip->onfi_params, onfi_params,
	       sizeof(struct nand_onfi_params));

	if (le16_to_cpu(chip->onfi_params.opt_cmd) & 4)
		chip->flags |= OCTEON_NAND_GET_SET_FEATURE;

	debug("le16_to_cpu(0x1234) == 0x%x\n", le16_to_cpu(0x1234));
	debug("cvmx_le16_to_cpu(0x1234) = 0x%x\n", cvmx_le16_to_cpu(0x1234));
	chip->onfi_version =
		revision_decode[fls(le16_to_cpu(chip->onfi_params.revision))];
	debug("ONFI revision %d (0x%x)\n", chip->onfi_version,
	      le16_to_cpu(chip->onfi_params.revision));

	chip->page_shift =
		fls(le32_to_cpu(chip->onfi_params.byte_per_page)) - 1;
	chip->pagesize = 1 << chip->page_shift;
	chip->ecc.strength = chip->onfi_params.ecc_bits;
	chip->writesize = le32_to_cpu(chip->onfi_params.byte_per_page);
	chip->erasesize =
		1 << (fls(le32_to_cpu(chip->onfi_params.pages_per_block)) - 1);
	chip->erasesize *= chip->writesize;

	chip->oobsize = le16_to_cpu(chip->onfi_params.spare_bytes_per_page);
	chip->chipsize =
		1 << (fls(le32_to_cpu(chip->onfi_params.blocks_per_lun)) - 1);
	chip->chipsize *= (uint64_t)chip->erasesize * chip->onfi_params.lun_count;
	chip->num_onfi_pages = chip->onfi_params.num_param_pages
				? chip->onfi_params.num_param_pages
				: 3;

	printf("Using hardware ECC syndrome support\n");
	chip->ecc.mode = NAND_ECC_HW_SYNDROME;
	chip->ecc.strength = chip->onfi_params.ecc_bits;
	if (chip->onfi_params.ecc_bits == 0xff) {
		/* If 0xff then we need to access the extended parameter
		 * page.
		 */
		if (octeon_read_extended_parameters(chip)) {
			printf("%s: Error reading ONFI extended parameter data for chip %d\n",
			       __func__, chip->selected_chip);
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
	chip->ecc.strength = (chip->ecc.bytes * 8) / fls(8 * chip->ecc.size);
	chip->ecc.steps = chip->pagesize / chip->ecc.size;
	chip->ecc.total = chip->ecc.steps * chip->ecc.bytes;
	debug("NAND chip %d using hw_bch ECC for %d bits of correction per %d byte block.  ECC size is %d bytes\n",
	      chip->selected_chip,
	      chip->ecc.strength,
	      chip->ecc.size,
	      chip->ecc.bytes);
	return 0;
}
#undef DEBUG
