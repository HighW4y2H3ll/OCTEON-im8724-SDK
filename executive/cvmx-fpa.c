/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights
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
 * Support library for the hardware Free Pool Allocator.
 *
 * <hr>$Revision: 167433 $<hr>
 *
 */

#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include "linux/export.h"
#include "linux/module.h"
#include "asm/octeon/cvmx.h"
#include "asm/octeon/cvmx-fpa.h"
#include "asm/octeon/cvmx-helper.h"
#include "asm/octeon/cvmx-global-resources.h"
#else
#include "cvmx.h"
#include "cvmx-fpa.h"
#include "cvmx-helper.h"
#include "cvmx-helper-fpa.h"
#include "cvmx-ipd.h"
#include "cvmx-bootmem.h"
#include "cvmx-global-resources.h"
#endif

static const int debug = 0;

#ifndef CVMX_ENABLE_FPA_CHECKS
#define CVMX_ENABLE_FPA_CHECKS	0
#endif

/* Due to suspected errata, we may not be able to let the FPA_AURAX_CNT
 * get too close to 0, to avoid a spurious wrap-around error
 */
const unsigned __cvmx_fpa3_cnt_offset = 32;

/* For advanced checks, a guard-band is created around the internal
 * stack, to make sure the stack is not overwritten.
 */
const uint64_t magic_pattern = 0xbab4faced095f00d;
const unsigned guard_band_size = CVMX_ENABLE_FPA_CHECKS << 10; /* 1KiB default*/

#define CVMX_CACHE_LINE_SHIFT   (7)

#define	CVMX_FPA3_NAME_LEN	(16)

typedef struct {
	char		name[CVMX_FPA3_NAME_LEN];
	uint64_t	stack_paddr;	/* Internal stack storage */
	uint64_t	bufs_paddr;	/* Buffer pool base address */
	uint64_t	stack_psize;	/* Internal stack storage size */
	uint64_t	bufs_psize;	/* Buffer pool raw size */
	uint64_t	buf_count;	/* Number of buffer filled */
	uint64_t	buf_size;	/* Buffer size */
} cvmx_fpa3_poolx_info_t;

typedef struct {
	char		name[CVMX_FPA3_NAME_LEN];
	unsigned	buf_size;	/* Buffer size */
} cvmx_fpa3_aurax_info_t;

typedef struct {
	char		name[CVMX_FPA1_NAME_SIZE];
	uint64_t	size; 		/* Block size of pool buffers */
	uint64_t	buffer_count;
	uint64_t	base_paddr;	/* Base physical addr */
			/* if buffer is allocated at initialization */
} cvmx_fpa1_pool_info_t;


/**
 * FPA1/FPA3 info structure is stored in a named block
 * that is allocated once and shared among applications.
 */
static CVMX_SHARED cvmx_fpa1_pool_info_t * cvmx_fpa1_pool_info;
static CVMX_SHARED cvmx_fpa3_poolx_info_t *
	cvmx_fpa3_pool_info[CVMX_MAX_NODES];
static CVMX_SHARED cvmx_fpa3_aurax_info_t *
	cvmx_fpa3_aura_info[CVMX_MAX_NODES];

/**
 * Return the size of buffers held in a POOL
 *
 * @param pool is the POOL handle
 * @return buffer size in bytes
 *
 */
int cvmx_fpa3_get_pool_buf_size(cvmx_fpa3_pool_t pool)
{
	cvmx_fpa_poolx_cfg_t pool_cfg;

	if (!__cvmx_fpa3_pool_valid(pool))
		return -1;

	pool_cfg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool));
	return pool_cfg.cn78xx.buf_size << CVMX_CACHE_LINE_SHIFT;
}

/**
 * Return the size of buffers held in a buffer pool
 *
 * @param pool is the pool number
 *
 * This function will work with CN78XX models in backward-compatible mode
 */
unsigned cvmx_fpa_get_block_size(int pool)
{
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		return cvmx_fpa3_get_pool_buf_size(
			cvmx_fpa3_aura_to_pool(
				cvmx_fpa1_pool_to_fpa3_aura(pool)));
	} else {
		if ((unsigned) pool >= CVMX_FPA1_NUM_POOLS)
			return 0;
		if (cvmx_fpa1_pool_info == NULL)
		    cvmx_fpa_global_init_node(0);
		return cvmx_fpa1_pool_info[pool].size;
	}
}

/**
 * The function returns the number of free buffers in a pool
 *
 * @param pool_num is the pool number
 *
 * This function will work with CN78XX models in backward-compatible mode
 */
int cvmx_fpa_get_current_count(int pool_num)
{
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		return cvmx_fpa3_get_available(
			cvmx_fpa1_pool_to_fpa3_aura(pool_num));
	} else
		return cvmx_fpa1_get_available(pool_num);
}

/**
 * Return the name of an FPA AURA
 *
 * @param aura is the AURA handle
 *
 * Only supported on CN78XX.
 */
const char *cvmx_fpa3_get_aura_name(cvmx_fpa3_gaura_t aura)
{
	cvmx_fpa3_aurax_info_t *pinfo;

	pinfo = cvmx_fpa3_aura_info[aura.node];
	if (pinfo != NULL)
		return pinfo[aura.laura].name;
	else
		return NULL;
}

/**
 * Return the name of an FPA POOL
 *
 * @param pool is the POOL handle
 *
 * Only supported on CN78XX.
 */
const char *cvmx_fpa3_get_pool_name(cvmx_fpa3_pool_t pool)
{
	cvmx_fpa3_poolx_info_t *pinfo;

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo != NULL)
		return pinfo[pool.lpool].name;
	else
		return NULL;
}

/**
 * Return the name of an FPA pool
 *
 * @param pool_num is the pool number
 *
 * This function will work with CN78XX models in backward-compatible mode
 */
const char *cvmx_fpa_get_name(int pool_num)
{
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		return cvmx_fpa3_get_aura_name(
			cvmx_fpa1_pool_to_fpa3_aura(pool_num));
	} else
		if ((unsigned) pool_num >= CVMX_FPA1_NUM_POOLS)
			return NULL;
		if (cvmx_fpa1_pool_info == NULL)
		    cvmx_fpa_global_init_node(0);
		return cvmx_fpa1_pool_info[pool_num].name;
}

#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
static void cvmx_fpa3_set_aura_name(cvmx_fpa3_gaura_t aura, const char *name)
{
	cvmx_fpa3_aurax_info_t *pinfo;

	pinfo = cvmx_fpa3_aura_info[aura.node];
	if (pinfo == NULL)
		return;
	pinfo += aura.laura;
	memset(pinfo->name, 0, sizeof(pinfo->name));
	if (name != NULL)
		strncpy(pinfo->name, name, sizeof(pinfo->name));
}

static void cvmx_fpa3_set_pool_name(cvmx_fpa3_pool_t pool, const char *name)
{
	cvmx_fpa3_poolx_info_t *pinfo;

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo == NULL)
		return;
	pinfo += pool.lpool;
	memset(pinfo->name, 0, sizeof(pinfo->name));
	if (name != NULL)
		strncpy(pinfo->name, name, sizeof(pinfo->name));
}

static void cvmx_fpa_set_name(int pool_num, const char *name)
{
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		cvmx_fpa3_set_aura_name(
			cvmx_fpa1_pool_to_fpa3_aura(pool_num), name);
	} else {
		cvmx_fpa1_pool_info_t *pinfo;
		if ((unsigned) pool_num >= CVMX_FPA1_NUM_POOLS)
			return;
		if (cvmx_fpa1_pool_info == NULL)
		    cvmx_fpa_global_init_node(0);
		pinfo = &cvmx_fpa1_pool_info[pool_num];
		memset(pinfo->name, 0, sizeof(pinfo->name));
		if (name != NULL)
			strncpy(pinfo->name, name, sizeof(pinfo->name));
	}
}

static int
cvmx_fpa3_aura_cfg(cvmx_fpa3_gaura_t aura, cvmx_fpa3_pool_t pool,
	uint64_t limit, uint64_t threshold, int ptr_dis)
{
	cvmx_fpa3_aurax_info_t *pinfo;
	cvmx_fpa_aurax_cfg_t aura_cfg;
	cvmx_fpa_poolx_cfg_t pool_cfg;
	cvmx_fpa_aurax_cnt_t cnt_reg;
	cvmx_fpa_aurax_cnt_limit_t limit_reg;
	cvmx_fpa_aurax_cnt_threshold_t thresh_reg;
	cvmx_fpa_aurax_int_t int_reg;
	unsigned block_size;

	if (debug)
		cvmx_dprintf("%s: AURA %u:%u POOL %u:%u\n",
			__func__,aura.node,aura.laura,pool.node,pool.lpool);

	if (aura.node != pool.node) {
		cvmx_printf("ERROR: %s: AURA/POOL node mismatch\n",__func__);
		return -1;
	}

        if (!__cvmx_fpa3_aura_valid(aura)) {
		cvmx_printf("ERROR: %s: AURA invalid\n", __func__);
		return -1;
	}

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL invalid\n", __func__);
		return -1;
	}

	/* Record POOL block size in AURA info entry */
	pool_cfg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool));

	block_size = pool_cfg.cn78xx.buf_size << 7;
	pinfo = cvmx_fpa3_aura_info[aura.node];
	if (pinfo == NULL)
		return -1;
	pinfo += aura.laura;

	pinfo->buf_size = block_size;

	/* block_size should be >0 except for POOL=0 which is never enabled*/
	if( pool_cfg.cn78xx.ena && block_size == 0) {
		cvmx_printf("ERROR; %s: POOL buf_size invalid\n", __func__);
		return -1;
	}

	/* Initialize AURA count, limit and treshold registers */
	cnt_reg.u64 = 0;
	cnt_reg.cn78xx.cnt = 0 + __cvmx_fpa3_cnt_offset;

	limit_reg.u64 = 0;
	limit_reg.cn78xx.limit = limit;
	/* Apply count offset, unless it cases a wrap-around */
	if ((limit + __cvmx_fpa3_cnt_offset) <  CVMX_FPA3_AURAX_LIMIT_MAX) 
		limit_reg.cn78xx.limit += __cvmx_fpa3_cnt_offset;

	thresh_reg.u64 = 0;
	thresh_reg.cn78xx.thresh = threshold;
	/* Apply count offset, unless it cases a wrap-around */
	if ((threshold + __cvmx_fpa3_cnt_offset) <  CVMX_FPA3_AURAX_LIMIT_MAX) 
		thresh_reg.cn78xx.thresh += __cvmx_fpa3_cnt_offset;

	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_CNT(aura.laura), 
		cnt_reg.u64);
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_CNT_LIMIT(aura.laura),
		limit_reg.u64);
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_CNT_THRESHOLD(aura.laura),
		 thresh_reg.u64);

	/* Clear any pending error interrupts */
	int_reg.u64 = 0;
	int_reg.cn78xx.thresh = 1;


	/* Follow a write to clear FPA_AURAX_INT[THRESH] with a read as
	   a workaround to Errata FPA-23410. If FPA_AURAX_INT[THRESH]
	   isn't clear, try again. */
	do {
		cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_INT(aura.laura),
			int_reg.u64);
		int_reg.u64 = cvmx_read_csr_node(aura.node, CVMX_FPA_AURAX_INT(aura.laura)); 
	} while (int_reg.s.thresh);

	/* Disable backpressure etc.*/
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_CNT_LEVELS(aura.laura), 0);
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_POOL_LEVELS(aura.laura), 0);

	aura_cfg.u64 = 0;
	aura_cfg.s.ptr_dis = ptr_dis;
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_CFG(aura.laura),
		aura_cfg.u64);
	cvmx_write_csr_node(aura.node, CVMX_FPA_AURAX_POOL(aura.laura), 
		pool.lpool);

	return 0;
}

/**
 * @INTERNAL
 *
 * Fill a newly created FPA3 POOL with buffers
 * using a temporary AURA.
 */
static int
cvmx_fpa3_pool_populate(
	cvmx_fpa3_pool_t pool, unsigned buf_cnt, unsigned buf_sz,
	void *mem_ptr, unsigned mem_node)
{
	cvmx_fpa3_poolx_info_t *pinfo;
	cvmx_fpa3_gaura_t aura;
	cvmx_fpa3_pool_t zero_pool;
	cvmx_fpa_poolx_cfg_t pool_cfg;
	cvmx_fpa_poolx_start_addr_t pool_start_reg;
	cvmx_fpa_poolx_end_addr_t pool_end_reg;
        cvmx_fpa_poolx_available_t avail_reg;
	cvmx_fpa_poolx_threshold_t thresh_reg;
	cvmx_fpa_poolx_int_t int_reg;
	unsigned block_size, align;
	unsigned long long mem_size;
	uint64_t paddr;
	unsigned i;

	if (debug)
		cvmx_dprintf("%s: POOL %u:%u buf_sz=%u count=%d\n",
			__func__, pool.node, pool.lpool, buf_sz, buf_cnt);

	if (!__cvmx_fpa3_pool_valid(pool))
		return -1;

	zero_pool = __cvmx_fpa3_pool(pool.node, 0);

	pool_cfg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool));

	block_size = pool_cfg.cn78xx.buf_size << 7;

	if (pool_cfg.cn78xx.nat_align) {
		/* Assure block_size is legit */
		if (block_size > (1 << 17)) {
			cvmx_printf("ERROR: %s: POOL %u:%u "
				"block size %u is not valid\n",
				__func__, pool.node, pool.lpool, block_size);
			return -1;
		}
	}
	align = CVMX_CACHE_LINE_SIZE;

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo == NULL)
		return -1;
	pinfo += pool.lpool;

	if (pinfo->buf_size != block_size ||
	    block_size != buf_sz) {
		cvmx_printf("ERROR: %s: POOL %u:%u buffer size mismatch\n",
			__func__, pool.node, pool.lpool);
		return -1;
	}

	if (mem_ptr == NULL) {
		/* When allocating our own memory
		 * make sure at least 'buf_cnt' blocks
		 * will fit into it.
		 */
		mem_size = (long long)buf_cnt * block_size +
			(block_size - 128);

		mem_ptr = cvmx_helper_mem_alloc(
					mem_node,
					mem_size,
					align);

		if (mem_ptr == NULL) {
			cvmx_printf("ERROR: %s: POOL %u:%u out of memory, "
			"could not allocate %llu bytes\n",
			__func__, pool.node, pool.lpool, mem_size);
			return -1;
		}

		/* Record memory base for use in shutdown */
		pinfo->bufs_paddr = cvmx_ptr_to_phys(mem_ptr);
	} else {
		/* caller-allocated memory is sized simply, may reduce count */
		mem_size = (long long)buf_cnt * block_size;
		/* caller responsable to free this memory too */
	}

	/* Recalculate buf_cnt after possible alignment adjustment */
	buf_cnt = mem_size / block_size;

	/* Get temporary AURA */
	aura = cvmx_fpa3_reserve_aura(pool.node, -1);
	if (!__cvmx_fpa3_aura_valid(aura))
		return -1;

	/* Attach the temporary AURA to the POOL */
	(void) cvmx_fpa3_aura_cfg(aura, pool, buf_cnt, buf_cnt + 1, 0);

	/* Set AURA count to buffer count to avoid wrap-around */
	cvmx_write_csr_node(aura.node,
			CVMX_FPA_AURAX_CNT(aura.laura), buf_cnt);

	/* Set POOL threshold just above buf count so it does not misfire */
	thresh_reg.u64 = 0;
	thresh_reg.cn78xx.thresh = buf_cnt + 1;
	cvmx_write_csr_node(pool.node, CVMX_FPA_POOLX_THRESHOLD(pool.lpool),
		thresh_reg.u64);

	/* Set buffer memory region bounds checking */
	paddr = (cvmx_ptr_to_phys(mem_ptr) >> 7) << 7;;
	pool_start_reg.u64 = pool_end_reg.u64 = 0;
	pool_start_reg.cn78xx.addr = paddr >> 7;
	pool_end_reg.cn78xx.addr = (paddr + mem_size + 127) >> 7;

	cvmx_write_csr_node(pool.node, CVMX_FPA_POOLX_START_ADDR(pool.lpool),
		pool_start_reg.u64);
	cvmx_write_csr_node(pool.node, CVMX_FPA_POOLX_END_ADDR(pool.lpool),
		pool_end_reg.u64);

	/* Make sure 'paddr' is divisible by 'block_size' */
	i = (paddr % block_size);
	if (i > 0) {
		i = block_size - i;
		paddr += i;
		mem_size -= i;
	}

	/* The above alignment mimics how the FPA3 hardware
	 * aligns pointers to to the buffer size, which only
	 * needs to be multiple of cache line size
	 */

	if (debug && (paddr != cvmx_ptr_to_phys(mem_ptr)))
		cvmx_dprintf("%s: pool mem paddr %#llx adjusted to %#llx"
			" for block size %#x\n",
			__func__, CAST_ULL(cvmx_ptr_to_phys(mem_ptr)),
			CAST_ULL(paddr), block_size);

	for (i = 0; i < buf_cnt; i++) {
		void *ptr = cvmx_phys_to_ptr(paddr);

		cvmx_fpa3_free_nosync(ptr, aura, 0);

		paddr += block_size;

		if ((paddr + block_size - 1) >= (paddr + mem_size))
			break;
	}

	if (debug && (i < buf_cnt)) {
		cvmx_dprintf("%s: buffer count reduced from %u to %u\n",
			__func__, buf_cnt, i);
		buf_cnt = i;
	}

	/* Wait for all buffers to reach the POOL before removing temp AURA */
	do {
		CVMX_SYNC;
	        avail_reg.u64 = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_AVAILABLE(pool.lpool));
	} while( avail_reg.cn78xx.count < buf_cnt);

	/* Detach the temporary AURA */
	(void) cvmx_fpa3_aura_cfg(aura, zero_pool, 0, 0, 0);

	/* Release temporary AURA */
	(void) cvmx_fpa3_release_aura(aura);

	/* Clear all POOL interrupts */
	int_reg.u64 = 0;
	int_reg.cn78xx.ovfls = 1;
	int_reg.cn78xx.crcerr = 1;
	int_reg.cn78xx.range = 1;
	int_reg.cn78xx.thresh = 1;
	cvmx_write_csr_node(pool.node, CVMX_FPA_POOLX_INT(pool.lpool),
		int_reg.u64);

	/* Record buffer count for shutdown */
	pinfo->buf_count = buf_cnt;

	return buf_cnt;
}

/**
 * @INTERNAL
 *
 * Fill a legacy FPA pool with buffers
 */
static int cvmx_fpa1_fill_pool(cvmx_fpa1_pool_t pool,
	int num_blocks, void *buffer)
{
	cvmx_fpa_poolx_start_addr_t pool_start_reg;
	cvmx_fpa_poolx_end_addr_t pool_end_reg;
	unsigned block_size = cvmx_fpa_get_block_size(pool);
	unsigned mem_size;
	char *bufp;

	if ((unsigned) pool >= CVMX_FPA1_NUM_POOLS)
		return -1;

	mem_size = block_size * num_blocks;

	if (buffer == NULL) {
		buffer = cvmx_helper_mem_alloc(0, mem_size,
					       CVMX_CACHE_LINE_SIZE);

		cvmx_fpa1_pool_info[pool].base_paddr =
			cvmx_ptr_to_phys(buffer);
	} else {
		/* Align user-supplied buffer to cache line size */
		unsigned off = (CVMX_CACHE_LINE_SIZE - 1) &
			cvmx_ptr_to_phys(buffer);
		if (off > 0) {
			buffer += CVMX_CACHE_LINE_SIZE - off;
			mem_size -= CVMX_CACHE_LINE_SIZE - off;
			num_blocks = mem_size / block_size;
		}
	}

	if (debug)
		cvmx_dprintf("%s: memory at %p size %#x\n",
			__func__, buffer, mem_size);

	pool_start_reg.u64 = 0;
	pool_end_reg.u64 = 0;

	/* buffer pointer range checks are highly recommended, but optional */
#if CVMX_ENABLE_FPA_CHECKS
	/* Set FPA bounds checking registers */
	pool_start_reg.cn61xx.addr = cvmx_ptr_to_phys(buffer) >> 7;
	pool_end_reg.cn61xx.addr =
		cvmx_ptr_to_phys(buffer + mem_size + 127) >> 7;
#else
	pool_start_reg.cn61xx.addr = 1;		/* catch NULL pointers */
	pool_end_reg.cn61xx.addr = (1ull << (40 - 7)) - 1; /* max paddr */
#endif
	if (!OCTEON_IS_OCTEON1PLUS() && 
	    !OCTEON_IS_MODEL(OCTEON_CN63XX)) {
		cvmx_write_csr(CVMX_FPA_POOLX_START_ADDR(pool),
			pool_start_reg.u64);
		cvmx_write_csr(CVMX_FPA_POOLX_END_ADDR(pool),
			pool_end_reg.u64);
	}
 
	bufp = buffer;
	while (num_blocks--) {
		cvmx_fpa1_free(bufp, pool, 0);
		cvmx_fpa1_pool_info[pool].buffer_count++;
		bufp += block_size;
	}
	return 0;
}

/**
 * @INTERNAL
 *
 * Setup a legacy FPA pool
 */
static int
cvmx_fpa1_pool_init(cvmx_fpa1_pool_t pool_id, int num_blocks, int block_size,
			void *buffer)
{
	int max_pool = cvmx_fpa_get_max_pools();

	if (pool_id < 0 || pool_id >= max_pool) {
		cvmx_printf("ERROR: %s pool %d invalid\n", __func__, pool_id);
		return -1;
	}

	if (cvmx_fpa1_pool_info == NULL)
	    cvmx_fpa_global_init_node(0);

	if (debug)
		cvmx_dprintf("%s: initializing info pool %d\n",
			 __func__, pool_id);

	cvmx_fpa1_pool_info[pool_id].size = block_size;
	cvmx_fpa1_pool_info[pool_id].buffer_count = 0;

	if (debug)
		cvmx_dprintf("%s: enabling unit for pool %d\n",
			 __func__, pool_id);

	return 0;
}
#endif /* !CVMX_BUILD_FOR_LINUX_KERNEL */

/**
 * Initialize global configuration for FPA block for specified node.
 *
 * @param node is the node number
 *
 * @note this function sets the initial QoS averaging timing parameters,
 * for the entire FPA unit (per node), which may be overriden on a
 * per AURA basis.
 */
int cvmx_fpa_global_init_node(int node)
{
        /* There are just the initial parameter values */
#define FPA_RED_AVG_DLY	1
#define FPA_RED_LVL_DLY	 3
#define FPA_QOS_AVRG 0
        /* Setting up avg_dly and prb_dly, enable bits */
        if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		char pool_info_name[32] = "cvmx_fpa3_pools_";
		char aura_info_name[32] = "cvmx_fpa3_auras_";
		char ns[2] = "0";

		ns[0] += node;
		strcat(pool_info_name, ns);
		strcat(aura_info_name, ns);

                cvmx_fpa3_config_red_params(node,
			FPA_QOS_AVRG, FPA_RED_LVL_DLY, FPA_RED_AVG_DLY);

		/* Allocate the pinfo named block */
		cvmx_fpa3_pool_info[node] =
			cvmx_bootmem_alloc_named_range_once(
				sizeof(cvmx_fpa3_pool_info[0][0]) *
				cvmx_fpa3_num_pools(),
				0, 0, 0,
				pool_info_name,
				NULL);

		cvmx_fpa3_aura_info[node] =
			cvmx_bootmem_alloc_named_range_once(
				sizeof(cvmx_fpa3_aura_info[0][0]) *
				cvmx_fpa3_num_auras(),
				0, 0, 0,
				aura_info_name,
				NULL);

		//XXX add allocation error check

		/* Setup zero_pool on this node */
		cvmx_fpa3_reserve_pool(node, 0);
		cvmx_fpa3_pool_info[node][0].buf_count = 0;
	} else {
		char pool_info_name[32] = "cvmx_fpa_pool";

		/* Allocate the pinfo named block */
		cvmx_fpa1_pool_info =
			cvmx_bootmem_alloc_named_range_once(
				sizeof(cvmx_fpa1_pool_info[0]) *
				CVMX_FPA1_NUM_POOLS,
				0, 0, 0,
				pool_info_name,
				NULL);

		cvmx_fpa1_enable();
	}

	return 0;
}
EXPORT_SYMBOL(cvmx_fpa_global_init_node);


#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
/**
 * @INTERNAL
 * Disable an empty FPA POOL 
 *
 * @param pool is the POOL handle
 *
 * Only supported on CN78XX.
 */
static void cvmx_fpa3_disable_pool(cvmx_fpa3_pool_t pool)
{
	cvmx_fpa_poolx_cfg_t pool_cfg;

	if (debug)
		cvmx_dprintf("%s: POOL %u:%u\n",
			__func__, pool.node, pool.lpool);

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL invalid\n", __func__);
		return;
	}

	pool_cfg.u64 = 0;
	pool_cfg.cn78xx.ena = 0;

	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool), pool_cfg.u64);
	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_STACK_BASE(pool.lpool), 0);
	cvmx_write_csr_node(pool.node,
		 CVMX_FPA_POOLX_STACK_ADDR(pool.lpool), 0);
	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_STACK_END(pool.lpool), 0);
}

static void __memset_u64(uint64_t *ptr, uint64_t pattern, unsigned words)
{

	while(words --)
		*ptr++ = pattern;
}

#if CVMX_ENABLE_FPA_CHECKS
static int __memcmp_u64(uint64_t *ptr, uint64_t pattern, int words)
{
	while(words > 0) {
		if(*ptr != pattern)
			break;
		ptr ++;
		words --;
	}
	if (words <= 0)
		return 0;

	if (debug)
		cvmx_dprintf("%s: %p = %#llx\n", __func__, ptr, CAST_ULL(*ptr));
	return -1;
}

/**
 * @INTERNAL
 *
 * Perform a sanity check on the FPA3 POOL internal stack,
 * by verifying the stack's guard band is intact,
 * which should almost prove that the stack has not been
 * steamrolled over by some rogue software process,
 * or due to buffer size violation.
 */
static int
cvmx_fpa3_pool_stack_check(cvmx_fpa3_pool_t pool)
{
	cvmx_fpa3_poolx_info_t *pinfo;
	uint64_t paddr, paddr1;
	void *ptr;
	unsigned size;

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL invalid\n", __func__);
		return -1;
	}

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo == NULL) {
		cvmx_fpa_global_init_node(pool.node);
		pinfo = cvmx_fpa3_pool_info[pool.node];
		if (pinfo == NULL) { 
			cvmx_printf("ERROR: %s: FPA on node#%u not intialized\n",
			__func__, pool.node);
		return -1;
		}
	}

	pinfo += pool.lpool;

	paddr = pinfo->stack_paddr;

	paddr1 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_STACK_BASE(pool.lpool));

	size = paddr1 - paddr;
	ptr = cvmx_phys_to_ptr(paddr);

	if (debug)
		cvmx_dprintf("%s: %p %u\n",__func__, ptr, size);

	if (__memcmp_u64(ptr, magic_pattern, size >> 3) == 0) {
		paddr += pinfo->stack_psize;
		paddr1 = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_STACK_END(pool.lpool));
		paddr1 += 128;
		size = paddr - paddr1;
		ptr = cvmx_phys_to_ptr(paddr1);
		if (debug)
			cvmx_dprintf("%s: %p %u\n",__func__, ptr, size);
		if (__memcmp_u64(ptr, magic_pattern, size >> 3) == 0)
			return 0;
	}
	cvmx_printf("ERROR: %s: POOL %u:%u stack guard band corrupt\n",
		__func__, pool.node, pool.lpool);
	return -1;
}
#endif /* CVMX_ENABLE_FPA_CHECKS */

/**
 * @INTERNAL
 * Initialize pool pointer-storage memory
 *
 * Unlike legacy FPA, which used free buffers to store pointers that
 * exceed on-chip memory, FPA3 requires a dedicated memory buffer for
 * free pointer stack back-store.
 *
 * @param pool - pool to initialize
 * @param mem_node - if memory should be allocated from a different node
 * @param max_buffer_cnt - maximum block capacity of pool
 * @param align - buffer alignment mode,
 *   current FPA_NATURAL_ALIGNMENT is supported
 * @param buffer_sz - size of buffers in pool
 */
static int
cvmx_fpa3_pool_stack_init(cvmx_fpa3_pool_t pool, unsigned mem_node,
	unsigned max_buffer_cnt, enum cvmx_fpa3_pool_alignment_e align,
	unsigned buffer_sz)
{
	cvmx_fpa3_poolx_info_t *pinfo;
	uint64_t stack_paddr;
	void *mem_ptr;
	unsigned stack_memory_size;
	cvmx_fpa_poolx_cfg_t pool_cfg;
	cvmx_fpa_poolx_fpf_marks_t pool_fpf_marks;

	if (debug)
		cvmx_dprintf("%s: POOL %u:%u bufsz=%u maxbuf=%u\n",
			__func__,pool.node,pool.lpool, buffer_sz,
			max_buffer_cnt);

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL invalid\n", __func__);
		return -1;
	}

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo == NULL) {
		cvmx_printf("ERROR: %s: FPA on node#%u is not intialized\n",
			__func__, pool.node);
		return -1;
	}
	pinfo += pool.lpool;

	/* Calculate stack size based on buffer count with one line to spare */
	stack_memory_size = (max_buffer_cnt * 128) / 29 + 128 + 127;

	/* Increase stack size by band guard */
	stack_memory_size += guard_band_size << 1;

	/* Align size to cache line */
	stack_memory_size = (stack_memory_size >> 7) << 7;

	/* Allocate internal stack */
	mem_ptr = cvmx_helper_mem_alloc(mem_node, stack_memory_size,
				      CVMX_CACHE_LINE_SIZE);

	if (debug)
		cvmx_dprintf("%s: stack_mem=%u ptr=%p\n",
			__func__, stack_memory_size, mem_ptr);

	if (mem_ptr == NULL) {
		cvmx_dprintf("ERROR: %s"
			"Failed to allocate stack for POOL %u:%u\n",
			__func__, pool.node, pool.lpool);
		return -1;
	}

	/* Initialize guard bands */
	if (guard_band_size > 0) {
		__memset_u64(mem_ptr,
			magic_pattern, guard_band_size >> 3);
		__memset_u64(mem_ptr + stack_memory_size - guard_band_size,
			magic_pattern, guard_band_size >> 3);
	}

	pinfo->stack_paddr = cvmx_ptr_to_phys(mem_ptr);
	pinfo->stack_psize = stack_memory_size;

	/* Calculate usable stack start */
	stack_paddr = cvmx_ptr_to_phys(mem_ptr + guard_band_size);

	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_STACK_BASE(pool.lpool), stack_paddr);
	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_STACK_ADDR(pool.lpool), stack_paddr);

	/* Calculate usable stack end  - start of last cache line */
	stack_paddr = stack_paddr + stack_memory_size -
		(guard_band_size << 1);

	cvmx_write_csr_node(pool.node,
		 CVMX_FPA_POOLX_STACK_END(pool.lpool), stack_paddr);

	if (debug)
		cvmx_dprintf("%s: Stack paddr %#llx - %#llx\n", __func__,
			CAST_ULL(cvmx_read_csr_node(pool.node,
				CVMX_FPA_POOLX_STACK_BASE(pool.lpool))),
			CAST_ULL(cvmx_read_csr_node(pool.node,
				CVMX_FPA_POOLX_STACK_END(pool.lpool)))
			);

	/* Setup buffer size for this pool until it is shutdown */
	pinfo->buf_size = buffer_sz;

	pool_cfg.u64 = 0;
	pool_cfg.cn78xx.buf_size = buffer_sz >> 7;
	pool_cfg.cn78xx.l_type = 0x2;
	pool_cfg.cn78xx.ena = 0;
	if (align == FPA_NATURAL_ALIGNMENT) {
		pool_cfg.cn78xx.nat_align = 1;
	}

	/* FPA-26117, FPA-22443 */
	pool_fpf_marks.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_FPF_MARKS(pool.lpool));
	pool_fpf_marks.s.fpf_rd = 0x80;
	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_FPF_MARKS(pool.lpool), pool_fpf_marks.u64);

	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool), pool_cfg.u64);
	pool_cfg.cn78xx.ena = 1;
	cvmx_write_csr_node(pool.node,
		CVMX_FPA_POOLX_CFG(pool.lpool), pool_cfg.u64);

	/* Pool is now ready to be filled up */
	return 0;
}

/**
 * Create an FPA POOL and fill it up with buffers
 *
 * @param node is the node number for the pool and memory location
 * @param desired_pool is the local pool number desired
 * 	or -1 for first available
 * @param name is the symbolic name to assign the POOL
 * @param block_size is the size of all buffers held in this POOL
 * @param num_blocks is the number of free buffers to fill into the POOL
 * @param buffer is an optionally caller-supplied memory for the buffers
 * 	or NULL to cause the buffer memory to be allocated automatically.
 * @return the POOL handle
 *
 * Note: if the buffer memory is supplied by caller, the application
 * will be responsable to free this memory.
 *
 * Only supported on CN78XX.
 */
cvmx_fpa3_pool_t
cvmx_fpa3_setup_fill_pool(int node, int desired_pool, const char *name,
		unsigned block_size, unsigned num_blocks, void *buffer)
{
	cvmx_fpa3_pool_t pool;
	unsigned mem_node;
	int rc;

	if (node < 0)
		node = cvmx_get_node_num();

	if (debug)
		cvmx_dprintf("%s: desired pool=%d bufsize=%u cnt=%u '%s'\n",
			__func__, desired_pool, block_size, num_blocks, name);

	/* Use memory from the node local to the AURA/POOL */
	mem_node = node;

	if (num_blocks == 0 || num_blocks > 1<<30) {
		cvmx_printf("ERROR: %s: invalid block count %u\n",
			__func__, num_blocks);
		return CVMX_FPA3_INVALID_POOL;
	}

	/*
	 * Check for block size validity:
	 * With user-supplied buffer, can't increase block size, 
	 * so make sure it is at least 128, and is aligned to 128
	 * For all cases make sure it is not too big
	 */
	if ((buffer != NULL && 
	        ((block_size < CVMX_CACHE_LINE_SIZE) ||
	        (block_size & (CVMX_CACHE_LINE_SIZE-1)))) ||
	    (block_size > (1<<17))) {
			cvmx_printf("ERROR: %s: invalid block size %u\n",
				__func__, block_size);
			return CVMX_FPA3_INVALID_POOL;
		}

	if (block_size < CVMX_CACHE_LINE_SIZE)
		block_size = CVMX_CACHE_LINE_SIZE;

	/* Reserve POOL */
	pool = cvmx_fpa3_reserve_pool(node, desired_pool);

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL %u:%d not available\n",
			__func__, node, desired_pool);
		return CVMX_FPA3_INVALID_POOL;
	}

	/* Initialize POOL with stack storage */
	rc = cvmx_fpa3_pool_stack_init(pool, mem_node, num_blocks,
			FPA_NATURAL_ALIGNMENT, block_size);
	if (rc < 0) {
		cvmx_printf("ERROR: %s: POOL %u:%u stack setup failed\n",
			__func__, pool.node, pool.lpool);
		cvmx_fpa3_release_pool(pool);
		return CVMX_FPA3_INVALID_POOL;
	}

	/* Populate the POOL with buffers */
	rc = cvmx_fpa3_pool_populate(pool, num_blocks, block_size,
			buffer, mem_node);
	if (rc < 0) {
		cvmx_printf("ERROR: %s: POOL %u:%u memory fill failed\n",
			__func__, pool.node, pool.lpool);
		cvmx_fpa3_release_pool(pool);
		return CVMX_FPA3_INVALID_POOL;
	}

	cvmx_fpa3_set_pool_name(pool, name);

	return pool;
}

/**
 * Attach an AURA to an existing POOL
 *
 * @param pool is the handle of the POOL to be attached
 * @param desired_aura is the number of the AURA resired
 * 	or -1 for the AURA to be automatically assigned
 * @param name is a symbolic name for the new AURA
 * @param block_size is the size of all buffers that will be handed
 * 	out by this AURA
 * @param num_blocks is the maximum number of buffers that can be
 * 	handed out by this AURA, and can not exceed the number
 * 	of buffers filled into the attached POOL
 * @return the AURA handle
 *
 * Only supported on CN78XX.
 */
cvmx_fpa3_gaura_t
cvmx_fpa3_set_aura_for_pool(cvmx_fpa3_pool_t pool, int desired_aura,
	const char *name, unsigned block_size, unsigned num_blocks)
{
	cvmx_fpa3_gaura_t aura;
	cvmx_fpa_poolx_available_t avail_reg;
	const char *emsg;
	int rc;

	if (debug)
		cvmx_dprintf("%s: aura=%d bufsize=%u cnt=%u '%s'\n",
			__func__, desired_aura, block_size, num_blocks, name);

        if (!__cvmx_fpa3_pool_valid(pool)) {
		cvmx_printf("ERROR: %s: POOL argument invalid\n", __func__);
		return CVMX_FPA3_INVALID_GAURA;
	}

	/* Verify the AURA buffer count limit is not above POOL buffer count */
	avail_reg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_AVAILABLE(pool.lpool));
	if (avail_reg.cn78xx.count < num_blocks) {
		cvmx_printf("WARNING: %s: "
			"AURA %u:%u buffer count limit %u "
			"reduced to POOL available count %u\n",
			__func__, aura.node, aura.laura,
			num_blocks, (unsigned) avail_reg.cn78xx.count);
		num_blocks = avail_reg.cn78xx.count;
	}

	/* Reserve an AURA number, follow desired number */
	aura = cvmx_fpa3_reserve_aura(pool.node, desired_aura);

        if (!__cvmx_fpa3_aura_valid(aura)) {
		cvmx_printf("ERROR: %s: AURA %u:%d not available\n",
			__func__, pool.node, desired_aura);
		return CVMX_FPA3_INVALID_GAURA;
	}

	/* Initialize AURA attached to the above POOL */
	rc = cvmx_fpa3_aura_cfg(aura, pool, num_blocks, num_blocks + 1, 0);
	if (rc < 0) {
		emsg = "AURA configuration";
		goto _fail;
	}

	cvmx_fpa3_set_aura_name(aura, name);

	return aura;

_fail:
	cvmx_printf("ERROR: %s: %s\n", __func__, emsg);
	cvmx_fpa3_release_aura(aura);
	return CVMX_FPA3_INVALID_GAURA;
}

/**
 * Create a combination of an AURA and a POOL
 *
 * @param node is the node number for the pool and memory location
 * @param desired_aura is the number of the AURA resired
 * 	or -1 for the AURA to be automatically assigned
 * @param name is a symbolic name for the new AURA
 * @param block_size is the size of all buffers that will be handed
 * 	out by this AURA
 * @param num_blocks is the maximum number of buffers that can be
 * 	handed out by this AURA, and can not exceed the number
 * 	of buffers filled into the attached POOL
 * @param buffer is an optionally caller-supplied memory for the buffers
 * 	or NULL to cause the buffer memory to be allocated automatically.
 *
 * @return the AURA handle
 *
 * Note: if the buffer memory is supplied by caller, the application
 * will be responsable to free this memory.
 * The POOL number is always automatically assigned.
 *
 * Only supported on CN78XX.
 */
cvmx_fpa3_gaura_t
cvmx_fpa3_setup_aura_and_pool(int node, int desired_aura,
		const char *name, void *buffer,
		unsigned block_size, unsigned num_blocks)
{
	cvmx_fpa3_gaura_t aura = CVMX_FPA3_INVALID_GAURA;
	cvmx_fpa3_pool_t pool = CVMX_FPA3_INVALID_POOL;
	const char *emsg = "";
	unsigned mem_node;
	int rc;

	if (debug)
		cvmx_dprintf("%s: aura=%d size=%u cnt=%u '%s'\n",
			__func__, desired_aura, block_size, num_blocks, name);

	if (node < 0)
		node = cvmx_get_node_num();

	if (num_blocks == 0 || num_blocks > 1<<30) {
		cvmx_printf("ERROR: %s: invalid block count %u\n",
			__func__, num_blocks);
		return CVMX_FPA3_INVALID_GAURA;
	}

	/* Use memory from the node local to the AURA/POOL */
	mem_node = node;

	/* Reserve an AURA number, follow desired number */
	aura = cvmx_fpa3_reserve_aura(node, desired_aura);

        if (!__cvmx_fpa3_aura_valid(aura)) {
		emsg = "AURA not available";
		goto _fail;
	}

	/* Reserve POOL dynamically to underpin this AURA */
	pool = cvmx_fpa3_reserve_pool(node, -1);

        if (!__cvmx_fpa3_pool_valid(pool)) {
		emsg = "POOL not available";
		goto _fail;
	}

	/*
	 * Check for block size validity:
	 * With user-supplied buffer, can't increase block size, 
	 * so make sure it is at least 128, and is aligned to 128
	 * For all cases make sure it is not too big
	 */
	if ((buffer != NULL && 
	        ((block_size < CVMX_CACHE_LINE_SIZE) ||
	        (block_size & (CVMX_CACHE_LINE_SIZE-1)))) ||
	    block_size > (1<<17)) {
		cvmx_printf("ERROR: %s: invalid block size %u\n",
			__func__, block_size);
		emsg = "invalid block size";
		goto _fail;
	}

	if (block_size < CVMX_CACHE_LINE_SIZE)
		block_size = CVMX_CACHE_LINE_SIZE;

	/* Initialize POOL with stack storage */
	rc = cvmx_fpa3_pool_stack_init(pool, mem_node, num_blocks,
			FPA_NATURAL_ALIGNMENT, block_size);
	if (rc < 0) {
		emsg = "POOL Stack setup";
		goto _fail;
	}

	/* Populate the AURA/POOL with buffers */
	rc = cvmx_fpa3_pool_populate(pool, num_blocks, block_size,
			buffer, mem_node);
	if (rc < 0) {
		emsg = "POOL buffer memory";
		goto _fail;
	}

	/* Initialize AURA attached to the above POOL */
	rc = cvmx_fpa3_aura_cfg(aura, pool, num_blocks, num_blocks + 1, 0);
	if (rc < 0) {
		emsg = "AURA configuration";
		goto _fail;
	}

	cvmx_fpa3_set_aura_name(aura, name);
	cvmx_fpa3_set_pool_name(pool, name);

	if (debug)
		cvmx_dprintf("%s: AURA %u:%u ready, avail=%lld\n",
			__func__, aura.node, aura.laura,
			cvmx_fpa3_get_available(aura));

	return aura;

_fail:
	cvmx_printf("ERROR: %s: Failed in %s\n", __func__, emsg);
	/* These will silently fail if POOL/AURA is not valid */
	cvmx_fpa3_release_aura(aura);
	cvmx_fpa3_release_pool(pool);
	return CVMX_FPA3_INVALID_GAURA;
}

/**
 * Setup a legacy FPA pool
 *
 * @param desired_pool is the POOL number desired or -1 for automatic
 * 	assignment
 * @param name is the symbolic POOL name
 * @param block_size is the size of all buffers held in this POOL
 * @param num_blocks is the number of free buffers to fill into the POOL
 * @param buffer is an optionally caller-supplied memory for the buffers
 * 	or NULL to cause the buffer memory to be allocated automatically.
 * @return pool number or -1 on error.
 *
 * Note: if the buffer memory is supplied by caller, the application
 * will be responsable to free this memory.
 */
int cvmx_fpa1_setup_pool(int desired_pool, const char *name, void *buffer,
			unsigned block_size, unsigned num_blocks)
{
	cvmx_fpa1_pool_t pool = CVMX_FPA1_INVALID_POOL;
	int rc;

	if (debug)
		cvmx_dprintf("%s: desired pool %d, name '%s', mem %p "
			"size %u count %u\n",
			__func__, desired_pool, name, buffer,
			block_size, num_blocks);

	/* Reserve desired pool or get one dynamically */
	pool = cvmx_fpa1_reserve_pool(desired_pool);

	/* Validate reserved pool, if succesful */
	if (pool < 0 || pool >= cvmx_fpa_get_max_pools()) {
		/* global resources would have printed an error message here */
		return CVMX_FPA1_INVALID_POOL;
	}

	/* Initialize the pool */
	rc = cvmx_fpa1_pool_init(pool, num_blocks, block_size, buffer);
	if (rc < 0) {
		cvmx_printf("ERROR: %s: failed pool %u init\n", __func__, pool);
		cvmx_fpa1_release_pool(pool);
		return CVMX_FPA1_INVALID_POOL;
	}

	rc = cvmx_fpa1_fill_pool(pool, num_blocks, buffer);
	if (rc < 0) {
		cvmx_printf("ERROR: %s: failed pool %u memory\n", __func__, pool);
		cvmx_fpa1_release_pool(pool);
		return CVMX_FPA1_INVALID_POOL;
	}

	if (debug)
		cvmx_dprintf("%s: pool %d filled up\b", __func__, pool);

	cvmx_fpa_set_name(pool, name);
	return pool;
}

/**
 * Setup an FPA pool with buffers
 *
 * @param pool is the POOL number desired or -1 for automatic assignment
 * @param name is the symbolic POOL name
 * @param buffer is an optionally caller-supplied memory for the buffers
 * 	or NULL to cause the buffer memory to be allocated automatically.
 * @param block_size is the size of all buffers held in this POOL
 * @param num_blocks is the number of free buffers to fill into the POOL
 * @param buffer is an optionally caller-supplied memory for the buffers
 * 	or NULL to cause the buffer memory to be allocated automatically.
 *
 * @return pool number or -1 on error.
 *
 * Note: if the buffer memory is supplied by caller, the application
 * will be responsable to free this memory.
 * This function will work with CN78XX models in backward-compatible mode
 */
int cvmx_fpa_setup_pool(int pool, const char *name, void *buffer,
			uint64_t block_size, uint64_t num_blocks)
{
	if (octeon_has_feature(OCTEON_FEATURE_FPA3)) {
		cvmx_fpa3_gaura_t aura;
		aura = cvmx_fpa3_setup_aura_and_pool(-1, pool, name, buffer,
				block_size, num_blocks);
		if (!__cvmx_fpa3_aura_valid(aura))
			return -1;
		if (aura.laura >= CVMX_FPA1_NUM_POOLS && pool >= 0)
			cvmx_printf("WARNING: %s: AURA %u is out of range"
				" for backward-compatible operation\n",
				__func__, aura.laura);
		return aura.laura;
	} else
		return cvmx_fpa1_setup_pool(pool, name, buffer,
				block_size, num_blocks);
}

/**
 * Discard an AURA
 *
 * All buffers allocated via this AURA must be freed prior to
 * discarding the AURA to avoid memory leaks.
 *
 * @param aura is the AURA handle
 *
 * @return 0 on success or -1 on error
 *
 * Note: since the buffers belong with a POOL, discarding an AURA
 * does not remove any buffers from the attached POOL. nor does it
 * free any memory.
 *
 * Only supported on CN78XX.
 */
int cvmx_fpa3_shutdown_aura(cvmx_fpa3_gaura_t aura)
{
	cvmx_fpa_aurax_cnt_t cnt_reg;
	cvmx_fpa3_pool_t zero_pool;
	cvmx_fpa3_aurax_info_t *pinfo;

	if (!__cvmx_fpa3_aura_valid(aura))
		return -1;

	pinfo = cvmx_fpa3_aura_info[aura.node];
	if (pinfo == NULL)
		return -1;
	pinfo += aura.laura;

	zero_pool = __cvmx_fpa3_pool(aura.node, 0);

	/* If this AURA is unused, done */
	if (pinfo->buf_size == 0)
		return 0;

	/* Check the all buffers have been returned */
	cnt_reg.u64 = cvmx_read_csr_node(aura.node,
		CVMX_FPA_AURAX_CNT(aura.laura));

	if( cnt_reg.cn78xx.cnt != __cvmx_fpa3_cnt_offset ) {
		cvmx_printf("WARNING: %s: AURA %u:%u is missing %u buffers\n",
			__func__, aura.node, aura.laura,
			(unsigned) cnt_reg.cn78xx.cnt - __cvmx_fpa3_cnt_offset);
	}

	/* Untie AURA from POOL, reset POR register values */
	(void) cvmx_fpa3_aura_cfg(aura, zero_pool, 0, 0, 0);

	memset(pinfo, 0, sizeof(*pinfo));
	(void) cvmx_fpa3_release_aura(aura);

	return 0;
}

/**
 * Destroy an FPA POOL and free its memory
 *
 * @param pool is the POOL handle
 *
 * @return 0 on success, -1 on error
 *
 * All buffers allocated from this POOL must be freed
 * and all AURA attached to this POOL must have been
 * discarded prior to calling this function.
 * Buffer memory will be freed if allocated automatically,
 * otherwise, if the memory was supplied by the caller,
 * after succesful return from this functon the caller
 * supplied memory can be safely freed by the application.
 *
 * Only supported on CN78XX.
 */
int cvmx_fpa3_shutdown_pool(cvmx_fpa3_pool_t pool)
{
	cvmx_fpa_poolx_start_addr_t pool_start_reg;
	cvmx_fpa_poolx_end_addr_t pool_end_reg;
	cvmx_fpa_aurax_pool_t aura_pool;
	cvmx_fpa_poolx_available_t avail_reg;
	cvmx_fpa3_poolx_info_t *pinfo;
	cvmx_fpa3_pool_t zero_pool;
	cvmx_fpa3_gaura_t aura;
	uint64_t stack_paddr;
	void *stack_base; 
	unsigned long long stack_size;
	int laura, i;

	if (!__cvmx_fpa3_pool_valid(pool))
		return -1;

	pinfo = cvmx_fpa3_pool_info[pool.node];
	if (pinfo == NULL)
		return -1;
	pinfo += pool.lpool;

	/* If this POOL is unused, done */
	if (pinfo->buf_size == 0)
		return 0;

	/* Check that no AURA is tied to this POOL, shutdown those that are */
	for (laura = 0; laura < cvmx_fpa3_num_auras(); laura++) {
		cvmx_fpa3_gaura_t aura = __cvmx_fpa3_gaura(pool.node, laura);

		aura_pool.u64 = cvmx_read_csr_node(aura.node,
			CVMX_FPA_AURAX_POOL(aura.laura));
		if (aura_pool.cn78xx.pool == pool.lpool) {
			cvmx_printf("WARNING: %s: "
				"AURA %u:%u implied shutdown\n",
				__func__, aura.node, aura.laura);
			cvmx_fpa3_shutdown_aura(aura);
		}
	}

	/* Check that all buffers have been returned to the pool */
	avail_reg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_AVAILABLE(pool.lpool));

	if (avail_reg.cn78xx.count != pinfo->buf_count) {
		cvmx_printf("WARNING: %s: POOL %u:%u is missing %lld buffers\n",
			__func__, pool.node, pool.lpool,
			(long long) pinfo->buf_count -
			(long long) avail_reg.cn78xx.count);
	}

	/* Extract all pointers from the POOL using temporary aura */
	zero_pool = __cvmx_fpa3_pool(pool.node, 0);

	/* Get temporary AURA */
	aura = cvmx_fpa3_reserve_aura(pool.node, -1);
	if (!__cvmx_fpa3_aura_valid(aura))
		return -1;

	/* This is how many buffers we need to extract */
	i = pinfo->buf_count; //avail_reg.cn78xx.count;

	/* Attach the temporary AURA to the POOL */
	(void) cvmx_fpa3_aura_cfg(aura, pool, i, i + 1, 0);

	/* Extract all pointers from POOL */
	while(i--)
		(void) cvmx_fpa3_alloc(aura);

	/* Detach the temporary AURA */
	(void) cvmx_fpa3_aura_cfg(aura, zero_pool, 0, 0, 0);

	/* Release temporary AURA */
	(void) cvmx_fpa3_release_aura(aura);

	/* Now there should be no more pointer in the POOL */
	i = 1 << 20;
	do {
		CVMX_SYNC;
	        avail_reg.u64 = cvmx_read_csr_node(pool.node,
			CVMX_FPA_POOLX_AVAILABLE(pool.lpool));
	} while( avail_reg.cn78xx.count > 0 && i--);

	if (i <= 0)
		cvmx_printf("WARNING: %s: POOL %u:%d could not be cleaned\n",
			__func__, pool.node, pool.lpool);

#if CVMX_ENABLE_FPA_CHECKS
	(void) cvmx_fpa3_pool_stack_check(pool);/* continue despite errors */
#endif

	/* Extract buffer storage memory range */
	pool_start_reg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_START_ADDR(pool.lpool));
	pool_end_reg.u64 = cvmx_read_csr_node(pool.node,
		CVMX_FPA_POOLX_END_ADDR(pool.lpool));

	/* Disable the pool - also zeroes stack pointers */
	cvmx_fpa3_disable_pool(pool);

	/* Free buffer memory if allocated by us */
	if (pinfo->bufs_paddr != 0ull) {
		void *mem_base;
		unsigned long long mem_size;
		unsigned long long psize;
		uint64_t pbase;

		mem_base = cvmx_phys_to_ptr(pinfo->bufs_paddr);
		mem_size = (long long) pinfo->buf_size * pinfo->buf_count +
			(pinfo->buf_size - 128);
		pbase = (long long)pool_start_reg.cn78xx.addr << 7;
		psize = ((long long)pool_end_reg.cn78xx.addr -
			pool_start_reg.cn78xx.addr) << 7;

		if (pinfo->bufs_paddr != pbase) {
			cvmx_printf("ERROR: %s: buffer memory base mismatch\n",
				__func__);
			cvmx_dprintf(" %#llx != %#llx\n",
				(long long)pinfo->bufs_paddr, (long long)pbase);
		} else if (psize < mem_size) {
			cvmx_printf("ERROR: %s: buffer memory size mismatch\n",
				__func__);
			cvmx_dprintf(" %#llx != %#llx\n", mem_size, psize);
		} else {
			cvmx_helper_mem_free(mem_base, mem_size);
		}
	}

	/* Free the stack storage */
	stack_paddr = pinfo->stack_paddr;
	stack_size = pinfo->stack_psize;

	stack_base = cvmx_phys_to_ptr(stack_paddr);
	if (debug)
		cvmx_dprintf("%s: freeing stack at %p size %llu\n",
			__func__, stack_base, stack_size);

	cvmx_helper_mem_free(stack_base, stack_size);

	/* Make the POOL available for re-use */
	cvmx_fpa3_release_pool(pool);

	memset(pinfo, 0, sizeof(*pinfo));

	return 0;
}

/**
 * Shutdown a POOL and AURA combination
 *
 * This function should only be used for an a POOL that is
 * attached to exactly one AURA.
 *
 * @param aura is the AURA handle
 * @return 0 on success, -1 on error
 *
 * See also 'cvmx_fpa3_shotdown_pool'.
 *
 * Only supported on CN78XX.
 */
int cvmx_fpa3_shutdown_aura_and_pool(cvmx_fpa3_gaura_t aura)
{
	cvmx_fpa3_pool_t pool;
	int rc;

	pool = cvmx_fpa3_aura_to_pool(aura);

	rc = cvmx_fpa3_shutdown_aura(aura);

	if(rc < 0) {
		cvmx_printf("ERROR: %s: failed to shutdown AURA\n", __func__);
		return rc;
	}

	rc = cvmx_fpa3_shutdown_pool(pool);

	if(rc < 0)
		cvmx_printf("ERROR: %s: failed to shutdown POOL\n", __func__);

	return rc;
}

/**
 * Shut down a legacy FPA pool
 *
 * @param pool is a pool number
 * @return 0 on success, -1 on error
 *
 */
int cvmx_fpa1_shutdown_pool(cvmx_fpa1_pool_t pool)
{
	unsigned long long count = 0;

	if ((unsigned) pool >= CVMX_FPA1_NUM_POOLS)
		return -1;

	if (cvmx_fpa1_pool_info == NULL)
		return -1;

	while (count < cvmx_fpa1_pool_info[pool].buffer_count) {
		void *ptr = cvmx_fpa1_alloc(pool);
		if (!ptr) break;
		count++;
	}

	if (count < cvmx_fpa1_pool_info[pool].buffer_count) {
		cvmx_printf("ERROR: %s: POOL %d not empty, %llu/%llu.\n",
			__func__, pool, count,
			CAST_ULL(cvmx_fpa1_pool_info[pool].buffer_count));
	} else if (debug)
		cvmx_dprintf("%s POOL %d successful, %llu buffers cleared.\n",
			__func__, pool,
			CAST_ULL(cvmx_fpa1_pool_info[pool].buffer_count));

	cvmx_fpa1_pool_info[pool].buffer_count = 0;

	cvmx_helper_mem_free(
		cvmx_phys_to_ptr(cvmx_fpa1_pool_info[pool].base_paddr),
		cvmx_fpa1_pool_info[pool].size);
	cvmx_fpa1_pool_info[pool].size = 0;
	cvmx_fpa1_pool_info[pool].base_paddr = 0;

	cvmx_fpa_set_name(pool, NULL);
	cvmx_fpa1_release_pool(pool);

	return 0;
}

/**
 * Shutdown a Memory pool and validate that it had all of
 * the buffers originally placed in it. This should only be
 * called by one processor after all hardware has finished
 * using the pool. Most like you will want to have called
 * cvmx_helper_shutdown_packet_io_global() before this
 * function to make sure all FPA buffers are out of the packet
 * IO hardware.
 *
 * @param pool   Pool to shutdown
 *
 * @return 0 on success, -1 on error
 *
 * This function will work with CN78XX models in backward-compatible mode
 */
int cvmx_fpa_shutdown_pool(int pool)
{
	if (debug)
		cvmx_dprintf("%s: pool=%d\n",__func__,pool);

	if (octeon_has_feature(OCTEON_FEATURE_FPA3))
		return cvmx_fpa3_shutdown_aura_and_pool(
			cvmx_fpa1_pool_to_fpa3_aura( pool));
	else
		return cvmx_fpa1_shutdown_pool(pool);
}


/**
 * Perform a sanity check on a specific FPA pool
 *
 * NOTE: Although this is a generic API, it is only implemented
 * on CN78XX version of FPA.
 *
 * Returns 0 on success, -1 if any internal errors are encountered
 */
int cvmx_fpa_validate_pool(int pool)
{
	int ret = 0;

#if CVMX_ENABLE_FPA_CHECKS
	if (octeon_has_feature(OCTEON_FEATURE_FPA3))
		ret = cvmx_fpa3_pool_stack_check(
			    cvmx_fpa3_aura_to_pool(
				cvmx_fpa1_pool_to_fpa3_aura( pool)));
#endif
	return ret;
}

#endif /* !CVMX_BUILD_FOR_LINUX_KERNEL */


