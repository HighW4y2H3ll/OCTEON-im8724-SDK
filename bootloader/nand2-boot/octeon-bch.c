/***********************license start***************
 * Copyright (c) 2013  Cavium Inc. (support@cavium.com).  All rights
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
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
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
 * Interface to the CN70XX, CN78XX hardware BCH engine.
 *
 * <hr>$Revision: 79788 $<hr>
 */
#undef DEBUG
#include "cvmx.h"
#include "cvmx-bch-defs.h"
#include "octeon-bch.h"
#include "octeon-fpa.h"
#include "octeon-queue.h"

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

cvmx_bch_app_config_t bch_config = {
	.command_queue_pool =	{OCTEON_BCH_FPA_POOL,
				 OCTEON_BCH_FPA_BUFFER_SIZE,
				 OCTEON_BCH_FPA_NUM_BUFFERS},
};

uint8_t
bch_fpa_buffers[OCTEON_BCH_FPA_NUM_BUFFERS][OCTEON_BCH_FPA_BUFFER_SIZE]
	__attribute__((aligned(128)));

/**
 * Initialize the BCH block
 *
 * @return Zero on success, negative on failure
 */
int octeon_bch_initialize(void)
{
	cvmx_bch_cmd_buf_t bch_cmd_buf;
	cvmx_bch_ctl_t bch_ctl;
	cvmx_cmd_queue_result_t result;
	int bch_pool = bch_config.command_queue_pool.pool_num;
	uint64_t bch_pool_size = bch_config.command_queue_pool.buffer_size;
	int i;
	uint32_t id;
	bool is_70xx;

	debug("%s: pool: %d, pool size: %lu, buffer count: %lu\n", __func__,
	      bch_pool, bch_pool_size,
	      bch_config.command_queue_pool.buffer_count);
	/* Initialize FPA pool for BCH pool buffers */
	octeon_fpa_global_initialize();

	for (i = bch_config.command_queue_pool.buffer_count - 1; i >= 0; i--)
		octeon_fpa_free(bch_fpa_buffers[i], bch_pool, 0);

	result = octeon_cmd_queue_initialize(CVMX_CMD_QUEUE_BCH,
					     0, bch_pool,
					     bch_pool_size);
	if (result != CVMX_CMD_QUEUE_SUCCESS) {
		cvmx_dprintf("%s: Error %d initializing command queue for BCH "
			     "pool: %d, size: %lu\n",
			     __func__, result, bch_pool,
			     (uint64_t)bch_pool_size);
		return -1;
	}

	bch_cmd_buf.u64 = 0;

	/* Read Octeon device ID */
	asm("mfc0 %0, $15,0" : "=r"(id));
	is_70xx = ((id >> 8) & 0xffff) == 0x0d96;

	if (is_70xx) {
		bch_cmd_buf.cn70xx.dwb = bch_pool_size / 128;
		bch_cmd_buf.cn70xx.pool = bch_pool;
	} else {
		bch_cmd_buf.cn73xx.ldwb = 1;
#warning Fix this function for CN73xx!
		printf("%s: Fix for non-70xx!\n", __func__);
		return -1;
	}
	bch_cmd_buf.s.size = bch_pool_size / 8;
	bch_cmd_buf.s.ptr =
		octeon_ptr_to_phys(octeon_cmd_queue_buffer(CVMX_CMD_QUEUE_BCH)) >> 7;
#ifdef DEBUG
	debug("%s: queue buffer: 0x%lx\n", __func__,
	      octeon_cmd_queue_buffer(CVMX_CMD_QUEUE_BCH));
#endif
	cvmx_write_csr(CVMX_BCH_CMD_BUF, bch_cmd_buf.u64);
	cvmx_write_csr(CVMX_BCH_GEN_INT, 7);
	cvmx_write_csr(CVMX_BCH_GEN_INT_EN, 0);
	bch_ctl.u64 = cvmx_read_csr(CVMX_BCH_CTL);
	bch_ctl.s.free_ena = 1;
	bch_ctl.s.one_cmd = 0;
	bch_ctl.s.erase_disable = 0;
	cvmx_write_csr(CVMX_BCH_CTL, bch_ctl.u64);
	cvmx_read_csr(CVMX_BCH_CMD_BUF);
	debug("%s: Success\n", __func__);
	return 0;
}

/**
 * Shutdown the BCH block
 *
 * @return Zero on success, negative on failure
 */
int octeon_bch_shutdown(void)
{
	cvmx_bch_ctl_t bch_ctl;

	debug("%s: ENTER\n", __func__);
	bch_ctl.u64 = cvmx_read_csr(CVMX_BCH_CTL);
	bch_ctl.s.reset = 1;
	cvmx_write_csr(CVMX_BCH_CTL, bch_ctl.u64);
	cvmx_wait(4);

	octeon_cmd_queue_shutdown(CVMX_CMD_QUEUE_BCH);

	return 0;
}

/**
 * Sets the internal FPA pool data structure for bch pool.
 * @param pool	fpa pool number to use
 * @param buffer_size	buffer size of pool
 * @param buffer_count	number of buffers to allocate to pool
 */
void octeon_bch_set_cmd_que_pool_config (int64_t pool, uint64_t buffer_size,
				       uint64_t buffer_count)
{
	bch_config.command_queue_pool.pool_num = pool;
	bch_config.command_queue_pool.buffer_size = buffer_size;
	bch_config.command_queue_pool.buffer_count = buffer_count;
}

/**
 * Gets the FPA pool data from internal data structure
 * @param bch_pool pointer to the fpa data structure to copy data
 */
void octeon_bch_get_cmd_que_pool_config(cvmx_fpa_pool_config_t *bch_pool)
{
	*bch_pool = bch_config.command_queue_pool;
}

/**
 * Given a data block calculate the ecc data and fill in the response
 *
 * @param[in] block	8-byte aligned pointer to data block to calculate ECC
 * @param block_size	Size of block in bytes, must be a multiple of two.
 * @param ecc_level	Number of errors that must be corrected.  The number of
 * 			parity bytes is equal to ((15 * ecc_level) + 7) / 8.
 * 			Must be 4, 8, 16, 24, 32, 40, 48, 56, 60 or 64.
 * @param[out] ecc	8-byte aligned pointer to where ecc data should go
 * @param[in] response	pointer to where responses will be written.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_bch_encode(const void *block, uint16_t block_size,
		    uint8_t ecc_level, void *ecc,
		    volatile octeon_bch_response_t *response)
{
	cvmx_bch_command_t command;
	cvmx_cmd_queue_result_t result;

	debug("%s(%p, %u, %u, %p, %p) ENTRY\n", __func__, block, block_size,
	      ecc_level, ecc, response);
	memset(&result, 0, sizeof(result));
	memset(&command, 0, sizeof(command));
	command.s.cword.ecc_gen = CVMX_BCH_INST_ECC_GENERATION;
	command.s.cword.ecc_level = ecc_level;
	command.s.cword.size = block_size;

	command.s.oword.ptr = octeon_ptr_to_phys(ecc);
	command.s.iword.ptr = octeon_ptr_to_phys((void *)block);
	command.s.resp.ptr = octeon_ptr_to_phys((void *)response);
	debug("Command: cword: 0x%lx, oword: 0x%lx, iword: 0x%lx, resp: 0x%lx\n",
	      command.u64[0], command.u64[1], command.u64[2], command.u64[3]);
	result = octeon_cmd_queue_write(CVMX_CMD_QUEUE_BCH,
					sizeof(command) / sizeof(uint64_t),
					command.u64);
	if (result != CVMX_CMD_QUEUE_SUCCESS) {
		cvmx_dprintf("%s: Error writing to queue\n", __func__);
		return -1;
	}

	octeon_bch_write_doorbell(1);

	return 0;
}

/**
 * Given a data block and ecc data correct the data block
 *
 * @param[in] block_ecc_in	8-byte aligned pointer to data block with ECC
 *				data concatenated to the end to correct
 * @param block_size		Size of block in bytes, must be a multiple of
 *				two.
 * @param ecc_level		Number of errors that must be corrected.  The
 *				number of parity bytes is equal to
 *				((15 * ecc_level) + 7) / 8.
 * 				Must be 4, 8, 16, 24, 32, 40, 48, 56, 60 or 64.
 * @param[out] block_out	8-byte aligned pointer to corrected data buffer.
 * 				This should not be the same as block_ecc_in.
 * @param[in] response		pointer to where responses will be written.
 *
 * @return Zero on success, negative on failure.
 */
int octeon_bch_decode(const void *block_ecc_in, uint16_t block_size,
		      uint8_t ecc_level, volatile void *block_out,
		      volatile octeon_bch_response_t *response)
{
	cvmx_bch_command_t command;
	cvmx_cmd_queue_result_t result;

	debug("%s(%p, %u, %u, %p, %p) ENTRY\n", __func__, block_ecc_in,
	      block_size, ecc_level, block_out, response);
	memset(&result, 0, sizeof(result));
	memset(&command, 0, sizeof(command));
	command.s.cword.ecc_gen = CVMX_BCH_INST_BLOCK_CORRECTION;
	command.s.cword.ecc_level = ecc_level;
	command.s.cword.size = block_size;

	command.s.oword.ptr = octeon_ptr_to_phys((void *)block_out);
 	command.s.iword.ptr = octeon_ptr_to_phys((void *)block_ecc_in);
	command.s.resp.ptr = octeon_ptr_to_phys((void *)response);
	debug("Command: cword: 0x%lx, oword: 0x%lx, iword: 0x%lx, resp: 0x%lx\n",
	      command.u64[0], command.u64[1], command.u64[2], command.u64[3]);
	result = octeon_cmd_queue_write(CVMX_CMD_QUEUE_BCH,
					sizeof(command) / sizeof(uint64_t),
					command.u64);
	if (result != CVMX_CMD_QUEUE_SUCCESS) {
		cvmx_dprintf("%s: Error writing to queue\n", __func__);
		return -1;
	}

	octeon_bch_write_doorbell(1);
	return 0;
}
