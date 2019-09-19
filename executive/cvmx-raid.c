/***********************license start***************
 * Copyright (c) 2003-2015  Cavium Inc. (support@cavium.com). All rights
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

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
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
 * Interface to RAID block. This is not available on all chips.
 *
 * <hr>$Revision: 112019 $<hr>
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/fpa.h>
#include <asm/octeon/cvmx-cmd-queue.h>
#include <asm/octeon/cvmx-raid.h>
#else
#include "cvmx.h"
#include "cvmx-cmd-queue.h"
#include "cvmx-raid.h"
#include "cvmx-fpa.h"
#include "cvmx-bootmem.h"
#endif

CVMX_SHARED cvmx_raid_config_t raid_config = {.command_queue_pool = {-1,4096,0} };
#define RAID_CMD_SEG_LEN 8

/**
 * Initialize the RAID block
 *
 * @param polynomial Coefficients for the RAID polynomial
 *
 * @return Zero on success, negative on failure
 */
int cvmx_raid_initialize(cvmx_rad_reg_polynomial_t polynomial)
{
	cvmx_cmd_queue_result_t result;
	cvmx_rad_reg_cmd_buf_t rad_reg_cmd_buf;
	int outputbuffer_pool;
	unsigned outputbuffer_pool_size, outputbuffer_cnt;

	outputbuffer_pool = raid_config.command_queue_pool.pool_num;
	outputbuffer_pool_size = raid_config.command_queue_pool.buffer_size;
	outputbuffer_cnt = raid_config.command_queue_pool.buffer_count;

	if (outputbuffer_cnt == 0)
		outputbuffer_cnt = 16;

	cvmx_write_csr(CVMX_RAD_REG_POLYNOMIAL, polynomial.u64);

	/*Initialize FPA pool for raid command queue buffers*/
#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
	if (outputbuffer_pool_size < 4096)
		outputbuffer_pool_size = 4096;

	/* Reserve and fill a pool */
	outputbuffer_pool = cvmx_fpa_setup_pool(
			outputbuffer_pool,"RAIDcmdq", NULL,
			outputbuffer_pool_size, outputbuffer_cnt);

	if (outputbuffer_pool < 0) {
		cvmx_dprintf("ERROR: %s: fpa pool %d configuration failed\n",
			     __func__,
			     (int) raid_config.command_queue_pool.pool_num);
	}

	/* NOTE: on CN78XX the pool_num is in fact LAURA */
	raid_config.command_queue_pool.pool_num = outputbuffer_pool;
#endif

	result = cvmx_cmd_queue_initialize(CVMX_CMD_QUEUE_RAID, 0, outputbuffer_pool, outputbuffer_pool_size);
	if (result != CVMX_CMD_QUEUE_SUCCESS)
		return -1;

	rad_reg_cmd_buf.u64 = 0;
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		rad_reg_cmd_buf.cn78xx.aura = outputbuffer_pool |
			(cvmx_get_node_num() << 10);
		rad_reg_cmd_buf.cn78xx.size = outputbuffer_pool_size / RAID_CMD_SEG_LEN;
		cvmx_write_csr(CVMX_RAD_REG_CMD_BUF, rad_reg_cmd_buf.u64);
		cvmx_write_csr(CVMX_RAD_REG_CMD_PTR, cvmx_ptr_to_phys(cvmx_cmd_queue_buffer(CVMX_CMD_QUEUE_RAID)));
	} else {
		rad_reg_cmd_buf.cn52xx.dwb = outputbuffer_pool_size / CVMX_CACHE_LINE_SIZE;
		rad_reg_cmd_buf.cn52xx.pool = outputbuffer_pool;
		rad_reg_cmd_buf.s.size = outputbuffer_pool_size / RAID_CMD_SEG_LEN;
		rad_reg_cmd_buf.s.ptr = cvmx_ptr_to_phys(cvmx_cmd_queue_buffer(CVMX_CMD_QUEUE_RAID)) >> 7;
		cvmx_write_csr(CVMX_RAD_REG_CMD_BUF, rad_reg_cmd_buf.u64);
	}
	return 0;
}

EXPORT_SYMBOL(cvmx_raid_initialize);

/**
 * Shutdown the RAID block. RAID must be idle when
 * this function is called.
 *
 * @return Zero on success, negative on failure
 */
int cvmx_raid_shutdown(void)
{
	cvmx_rad_reg_ctl_t rad_reg_ctl;

	if (cvmx_cmd_queue_length(CVMX_CMD_QUEUE_RAID)) {
		cvmx_dprintf("ERROR: cvmx_raid_shutdown: RAID not idle.\n");
		return -1;
	}

	rad_reg_ctl.u64 = cvmx_read_csr(CVMX_RAD_REG_CTL);
	rad_reg_ctl.s.reset = 1;
	cvmx_write_csr(CVMX_RAD_REG_CTL, rad_reg_ctl.u64);
	cvmx_wait(100);

	cvmx_cmd_queue_shutdown(CVMX_CMD_QUEUE_RAID);
	cvmx_write_csr(CVMX_RAD_REG_CMD_BUF, 0);

	if (raid_config.command_queue_pool.buffer_count != 0) {
		cvmx_fpa_shutdown_pool(cvmx_fpa_get_raid_pool());
	}

	return 0;
}

EXPORT_SYMBOL(cvmx_raid_shutdown);

/**
 * Submit a command to the RAID block
 *
 * @param num_words Number of command words to submit
 * @param words     Command words
 *
 * @return Zero on success, negative on failure
 */
int cvmx_raid_submit(int num_words, cvmx_raid_word_t words[])
{
	cvmx_cmd_queue_result_t result = cvmx_cmd_queue_write(CVMX_CMD_QUEUE_RAID, 1, num_words, (uint64_t *) words);
	if (result == CVMX_CMD_QUEUE_SUCCESS)
		cvmx_write_csr(CVMX_ADDR_DID(CVMX_FULL_DID(14, 0)), num_words);
	return result;
}

void cvmx_raid_set_cmd_que_pool_config(int64_t pool, uint64_t buffer_size,
			      uint64_t buffer_count)
{
	raid_config.command_queue_pool.pool_num = pool;
	raid_config.command_queue_pool.buffer_size = buffer_size;
	raid_config.command_queue_pool.buffer_count = buffer_count;
}

void cvmx_raid_config_control_word(cvmx_raid_word_t *word, int number_input_blocks, int block_length, int qout, int pout, int qcmp, int pcmp, int qxor, int pxor, int wqe)
{
	word->u64 = 0;

	word->cword.qout = qout;
	word->cword.q_cmp = qcmp;
	word->cword.q_xor = qxor;

	word->cword.pout = pout;
	word->cword.p_cmp = pcmp;
	word->cword.p_xor = pxor;

	word->cword.wqe = wqe;

	word->cword.size = block_length;
	word->cword.iword = number_input_blocks;
}

void cvmx_raid_config_input_word(cvmx_raid_word_t *word, int qen, uint64_t qmult, int pen, uint64_t input_ptr)
{
	word->u64 = 0;

	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		word->iword_78xx.qen = qen;
		word->iword_78xx.qmult = qmult;
		word->iword_78xx.pen = pen;

		word->iword_78xx.addr = input_ptr;
	} else {
		word->iword.qen = qen;
		word->iword.qmult = qmult;
		word->iword.pen = pen;

		word->iword.addr = input_ptr;
	}
}

void cvmx_raid_config_output_word(cvmx_raid_word_t *word, int no_cache, uint64_t output_ptr)
{
	word->u64 = 0;

	word->oword.nc = no_cache;
	word->oword.addr = output_ptr;
}

void cvmx_raid_config_resp_word(cvmx_raid_word_t *word, uint64_t resp_ptr)
{
	word->u64 = 0;

	word->respword.addr = resp_ptr;
}

void cvmx_raid_get_cmd_que_pool_config(cvmx_fpa_pool_config_t* cmd_que_pool)
{
	*cmd_que_pool = raid_config.command_queue_pool;
}

EXPORT_SYMBOL(cvmx_raid_submit);

