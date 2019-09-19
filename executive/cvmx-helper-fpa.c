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
 * Helper functions for FPA setup.
 *
 * <hr>$Revision: 75216 $<hr>
 */

#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include "linux/export.h"
#include "asm/octeon/cvmx.h"
#include "asm/octeon/cvmx-fpa.h"
#include "asm/octeon/cvmx-helper.h"
#include "asm/octeon/cvmx-helper-fpa.h"
#include "asm/octeon/cvmx-global-resources.h"
#else
#include "cvmx.h"
#include "cvmx-bootmem.h"
#include "cvmx-fpa-defs.h"
#include "cvmx-fpa.h"
#include "cvmx-helper-fpa.h"
#include "cvmx-ipd.h"
#include "cvmx-dfa.h"
#include "cvmx-tim.h"
#include "cvmx-pko.h"
#include "cvmx-pki.h"
#include "cvmx-global-resources.h"
#endif


void cvmx_helper_fpa1_dump(int node)
{
	int pool_num = 0, pools_max = cvmx_fpa_get_max_pools();

	if (node == -1) node = cvmx_get_node_num();

	cvmx_printf("FPA pool status: pools count: %u\n", pools_max);
	cvmx_printf("---------------------------------------------------- \n");
	cvmx_printf("%5s %5s %12s %16s\n",
		"POOL", "Size", "Free", "Name");

	while (pool_num < pools_max) {
		int pool = pool_num++;
		unsigned sz = cvmx_fpa_get_block_size(pool);
		char *s = "";
		if (sz == 0)
			continue;
		if (cvmx_fpa_get_pool_owner(pool) != cvmx_get_app_id())
			s = "*";

		cvmx_printf("%5u %5u %12u %16s %1s\n", pool, sz,
				     cvmx_fpa_get_current_count(pool),
				     cvmx_fpa_get_name(pool), s);
		}
}

void cvmx_helper_fpa3_dump(unsigned node)
{
	int lpool, laura;
	unsigned intr;
	char line[128];

	intr = cvmx_read_csr_node(node, CVMX_FPA_ERR_INT);
	memset(line, '*', 80); line[80] = '\0';
	cvmx_printf("\n%s\n", line);
	cvmx_printf("   FPA3 on node %u: intr=%#x\n", node, intr);
	cvmx_printf("%s\n", line);
	cvmx_printf("%6s %5s %14s %14s %16s %s\n",
		"POOL","Size","Free", "","Name", "Intr");

	for(lpool = 0; lpool < cvmx_fpa3_num_pools(); lpool++) {
		cvmx_fpa3_pool_t pool;
		cvmx_fpa_poolx_cfg_t pool_cfg;
		cvmx_fpa_poolx_available_t avail_reg;
		unsigned bsz;
		unsigned long long  bcnt;

		pool = __cvmx_fpa3_pool(node, lpool);
		pool_cfg.u64 = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_CFG(pool.lpool));
		bsz = pool_cfg.cn78xx.buf_size << 7;
		if (bsz == 0)
			continue;
		avail_reg.u64 = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_AVAILABLE(pool.lpool));
		bcnt = avail_reg.cn78xx.count;
		intr = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_INT(pool.lpool));

		cvmx_printf("%6u %5u %14llu %14s %16s %#4x\n",
			lpool, bsz, bcnt, "", cvmx_fpa3_get_pool_name(pool),
			intr);
	}
	cvmx_printf("\n");

	cvmx_printf("%6s %5s %14s %14s %16s %s\n",
		"AURA", "POOL", "Allocated", "Remaining","Name","Intr");

	for(laura = 0; laura < cvmx_fpa3_num_auras(); laura++) {
		cvmx_fpa3_gaura_t aura;
		cvmx_fpa_aurax_cnt_t cnt_reg;
		cvmx_fpa_aurax_cnt_limit_t limit_reg;
		cvmx_fpa_aurax_pool_t aurax_pool;
		unsigned long long cnt, limit, rem;
		unsigned pool;
		const char *name;

		aura = __cvmx_fpa3_gaura(node, laura);
		aurax_pool.u64 = cvmx_read_csr_node(aura.node,
			CVMX_FPA_AURAX_POOL(aura.laura));
		pool = aurax_pool.cn78xx.pool;
		if (pool == 0)
			continue;
		cnt_reg.u64 = cvmx_read_csr_node(aura.node,
			CVMX_FPA_AURAX_CNT(aura.laura));
		limit_reg.u64 = cvmx_read_csr_node(aura.node,
			CVMX_FPA_AURAX_CNT_LIMIT(aura.laura));
		cnt = cnt_reg.cn78xx.cnt;
		limit = limit_reg.cn78xx.limit;
		rem = limit - cnt;
		name = cvmx_fpa3_get_aura_name(aura);
		intr = cvmx_read_csr_node(aura.node,
			CVMX_FPA_AURAX_INT(aura.laura));

		if (limit ==CVMX_FPA3_AURAX_LIMIT_MAX)
			cvmx_printf("%6u %5u %14llu %14s %16s %#4x\n",
				laura, pool, cnt, "unlimited", name, intr);
		else
			cvmx_printf("%6u %5u %14llu %14llu %16s %#4x\n",
				laura, pool, cnt, rem, name, intr);

	}
	cvmx_printf("\n");
}

void cvmx_helper_fpa_dump(int node)
{
	if (node == -1) node = cvmx_get_node_num();
	if (octeon_has_feature(OCTEON_FEATURE_FPA3))
		cvmx_helper_fpa3_dump(node);
	else
		cvmx_helper_fpa1_dump(node);
}

int cvmx_helper_shutdown_fpa_pools(int node)
{

	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		cvmx_fpa3_gaura_t aura;
		cvmx_fpa3_pool_t pool;
		int laura, lpool;

		for (laura = 0; laura < cvmx_fpa3_num_auras(); laura++) {
			aura = __cvmx_fpa3_gaura(node, laura);
			(void) cvmx_fpa3_shutdown_aura(aura);
		}

		for (lpool = 0; lpool < cvmx_fpa3_num_pools(); lpool++) {
			pool = __cvmx_fpa3_pool(node, lpool);
			(void) cvmx_fpa3_shutdown_pool(pool);
		}
	} else {
		int pool;

		for (pool = 0; pool < CVMX_FPA1_NUM_POOLS; pool++) {
			if (cvmx_fpa_get_block_size(pool) > 0)
				cvmx_fpa_shutdown_pool(pool);
		}
		if (!OCTEON_IS_MODEL(OCTEON_CN68XX))
			cvmx_fpa_disable();
	}

	return 0;
}

/**
 * @INTERNAL
 * OBSOLETE
 *
 * Allocate memory for and initialize a single FPA pool.
 *
 * @param pool    Pool to initialize
 * @param buffer_size  Size of buffers to allocate in bytes
 * @param buffers Number of buffers to put in the pool. Zero is allowed
 * @param name    String name of the pool for debugging purposes
 * @return Zero on success, non-zero on failure
 *
 * This function is only for transition, will be removed.
 */
int __cvmx_helper_initialize_fpa_pool(int pool, uint64_t buffer_size,
				      uint64_t buffers, const char *name)
{
	return cvmx_fpa_setup_pool(pool, name, NULL, buffer_size, buffers);
}


#ifndef CVMX_BUILD_FOR_LINUX_KERNEL

/**
 * @INTERNAL
 * Allocate memory and initialize the FPA pools using memory
 * from cvmx-bootmem. Specifying zero for the number of
 * buffers will cause that FPA pool to not be setup. This is
 * useful if you aren't using some of the hardware and want
 * to save memory. Use cvmx_helper_initialize_fpa instead of
 * this function directly.
 *
 * @param pip_pool Should always be CVMX_FPA_PACKET_POOL
 * @param pip_size Should always be CVMX_FPA_PACKET_POOL_SIZE
 * @param pip_buffers
 *                 Number of packet buffers.
 * @param wqe_pool Should always be CVMX_FPA_WQE_POOL
 * @param wqe_size Should always be CVMX_FPA_WQE_POOL_SIZE
 * @param wqe_entries
 *                 Number of work queue entries
 * @param pko_pool Should always be CVMX_FPA_OUTPUT_BUFFER_POOL
 * @param pko_size Should always be CVMX_FPA_OUTPUT_BUFFER_POOL_SIZE
 * @param pko_buffers
 *                 PKO Command buffers. You should at minimum have two per
 *                 each PKO queue.
 * @param tim_pool Should always be CVMX_FPA_TIMER_POOL
 * @param tim_size Should always be CVMX_FPA_TIMER_POOL_SIZE
 * @param tim_buffers
 *                 TIM ring buffer command queues. At least two per timer bucket
 *                 is recommended.
 * @param dfa_pool Should always be CVMX_FPA_DFA_POOL
 * @param dfa_size Should always be CVMX_FPA_DFA_POOL_SIZE
 * @param dfa_buffers
 *                 DFA command buffer. A relatively small (32 for example)
 *                 number should work.
 * @return Zero on success, non-zero if out of memory
 */
static int
__cvmx_helper_initialize_fpa(int pip_pool, int pip_size, int pip_buffers,
			     int wqe_pool, int wqe_size, int wqe_entries,
			     int pko_pool, int pko_size, int pko_buffers,
			     int tim_pool, int tim_size, int tim_buffers,
			     int dfa_pool, int dfa_size, int dfa_buffers)
{
	cvmx_fpa_enable();
	if (pip_buffers > 0) {
		cvmx_fpa_setup_pool(pip_pool, "PKI_POOL", NULL, pip_size, pip_buffers);
	}
	/* Allocate WQE pool only if it is distinct from packet pool */
	if (wqe_entries > 0 && wqe_pool != pip_pool) {
		cvmx_fpa_setup_pool(wqe_pool, "WQE_POOL", NULL, wqe_size, wqe_entries);
	}
	/* cn78xx PKO allocates its own FPA pool per HW constraints */
	if (pko_buffers > 0) {
		if (!octeon_has_feature(OCTEON_FEATURE_PKI))
			cvmx_fpa_setup_pool(pko_pool, "PKO_POOL", NULL, pko_size, pko_buffers);
	}
	if (tim_buffers > 0) {
		cvmx_fpa_setup_pool(tim_pool, "TIM_POOL", NULL, tim_size, tim_buffers);
	}
	if (dfa_buffers > 0) {
		cvmx_fpa_setup_pool(dfa_pool, "DFA_POOL", NULL, dfa_size, dfa_buffers);
	}
	return 0;
}

/**
 * Allocate memory and initialize the FPA pools using memory
 * from cvmx-bootmem. Sizes of each element in the pools is
 * controlled by the cvmx-config.h header file. Specifying
 * zero for any parameter will cause that FPA pool to not be
 * setup. This is useful if you aren't using some of the
 * hardware and want to save memory.
 *
 * @param packet_buffers
 *               Number of packet buffers to allocate
 * @param work_queue_entries
 *               Number of work queue entries
 * @param pko_buffers
 *               PKO Command buffers. You should at minimum have two per
 *               each PKO queue.
 * @param tim_buffers
 *               TIM ring buffer command queues. At least two per timer bucket
 *               is recommended.
 * @param dfa_buffers
 *               DFA command buffer. A relatively small (32 for example)
 *               number should work.
 * @return Zero on success, non-zero if out of memory
 */
int cvmx_helper_initialize_fpa(int packet_buffers, int work_queue_entries,
			       int pko_buffers, int tim_buffers, int dfa_buffers)
{
	int packet_pool = (int)cvmx_fpa_get_packet_pool();
	int wqe_pool = (int)cvmx_fpa_get_wqe_pool();
	int outputbuffer_pool = (int)cvmx_fpa_get_pko_pool();
	int timer_pool;
	int dfa_pool = (int)cvmx_fpa_get_dfa_pool();
	int rv;
	
	cvmx_create_tim_named_block_once();

	timer_pool = (int)cvmx_fpa_get_timer_pool();

	rv = __cvmx_helper_initialize_fpa(packet_pool,
					  cvmx_fpa_get_packet_pool_block_size(),
					  packet_buffers,
					  wqe_pool,
					  cvmx_fpa_get_wqe_pool_block_size(),
					  work_queue_entries,
					  outputbuffer_pool,
					  cvmx_fpa_get_pko_pool_block_size(),
					  pko_buffers,
					  timer_pool,
					  cvmx_fpa_get_timer_pool_block_size(),
					  tim_buffers,
					  dfa_pool,
					  cvmx_fpa_get_dfa_pool_block_size(),
					  dfa_buffers
 					);

	return rv;
}

#endif

