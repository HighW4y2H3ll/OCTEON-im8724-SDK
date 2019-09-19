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
 * Support library for the hardware work queue timers.
 *
 * <hr>$Revision: 166783 $<hr>
 */

#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-bootmem.h"
#include "cvmx-helper-fpa.h"
#include "cvmx-tim.h"

/* CSR typedefs have been moved to cvmx-tim-defs.h */

/**
 * Global structure holding the state of all timers.
 */
	/* The cvmx_tim and timer_config structures was moved from CVMX_SHARED
	 * to new named block, in order to provide shared accses among multiple
	 * loadsets. Below are the pointers which will be mapped to them
	 */
	cvmx_tim_config_t 	*timer_config = NULL;
	CVMX_SHARED cvmx_tim_t 		*cvmx_tim = NULL;

/**
 * Setup a timer for use. Must be called before the timer
 * can be used.
 *
 * @param tick      Time between each bucket in microseconds. This must not be
 *                  smaller than 1024/(clock frequency in MHz).
 * @param max_ticks The maximum number of ticks the timer must be able
 *                  to schedule in the future. There are guaranteed to be enough
 *                  timer buckets such that:
 *                  number of buckets >= max_ticks.
 * @return Zero on success. Negative on error. Failures are possible
 *         if the number of buckets needed is too large or memory
 *         allocation fails for creating the buckets.
 */
int cvmx_tim_setup(uint64_t tick, uint64_t max_ticks)
{
	uint64_t timer_id;
	int error = -1;
	uint64_t tim_clock_hz = cvmx_clock_get_rate(CVMX_CLOCK_TIM); 
	uint64_t hw_tick_ns; 
	uint64_t hw_tick_ns_allowed; 
	uint64_t tick_ns = 1000 * tick; 
	int i;
	uint32_t temp;
	int timer_thr = 1024; 
	int timer_pool;
	uint64_t timer_pool_size;
	uint64_t mem_block_size = 0;
	cvmx_tim_bucket_entry_t *bucket;
	unsigned num_rings = cvmx_octeon_num_cores();

	if (num_rings > CVMX_TIM_NUM_TIMERS)
		num_rings = CVMX_TIM_NUM_TIMERS;

	/* For the simulator */
	if (tim_clock_hz == 0)
		tim_clock_hz = 800000000;

	timer_pool = (int)cvmx_fpa_get_timer_pool();
	timer_pool_size = cvmx_fpa_get_timer_pool_block_size();
	
	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		cvmx_tim_fr_rn_tt_t fr_tt;
		fr_tt.u64 = cvmx_read_csr(CVMX_TIM_FR_RN_TT);
		timer_thr = fr_tt.s.fr_rn_tt;
	}

	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX))
	{
		/* Use minimum required safeguard dividend, see tick param description above */
		timer_thr = 1024;
	}

	/* Be wary of the use of ticks (period) vs. cycles (freq) in this func */
	
	hw_tick_ns = timer_thr * 1000000000ull / tim_clock_hz;
	/*
	 * Double the minimal allowed tick to 2 * HW tick.  tick between
	 * (hw_tick_ns, 2*hw_tick_ns) will set config_ring1.s.interval
	 * to zero, or 1024 cycles. This is not enough time for the timer unit
	 * to fetch the bucket data, Resulting in timer ring error interrupt
	 * be always generated. Avoid such setting in software.
	 */
	hw_tick_ns_allowed = hw_tick_ns * 2;

#if TIM_DEBUG
	cvmx_dprintf("INFO: cvmx_tim_setup: tim_clock_hz=%lu, timer_thr=%d, tick_ns=%lu, hw_tick_ns=%lu\n", tim_clock_hz, timer_thr, tick_ns, hw_tick_ns);
#endif

	/* Make sure the timers are stopped */
	cvmx_tim_stop();

	/* Reinitialize out timer state */
	memset(cvmx_tim, 0, sizeof(*cvmx_tim));

	if (tick_ns < hw_tick_ns_allowed) {
		cvmx_dprintf("ERROR: cvmx_tim_setup: Requested tick %lu(ns) is smaller than"
			     " the minimal ticks allowed by hardware %lu(ns)\n", (unsigned long int)tick_ns, (unsigned long int)hw_tick_ns_allowed);
		return error;
	} else if (tick_ns > 4194304 * hw_tick_ns) {
		cvmx_dprintf("ERROR: cvmx_tim_setup: Requested tick %lu(ns) is greater than" " the max ticks %lu(ns)\n", (unsigned long int)tick_ns, (unsigned long int)hw_tick_ns);
		return error;
	}

	/* Keep doubling to determine shift */
	for (i = 2; i < 20; i++) {
		if (tick_ns < (hw_tick_ns << i))
			break;
	}

	cvmx_tim->max_ticks = (uint32_t) max_ticks;
	cvmx_tim->bucket_shift = (uint32_t) (i - 1 + 10);	/* To make wrap around calc easier */
	cvmx_tim->tick_cycles = tick * tim_clock_hz / 1000000; /* Cycles per input tick value, also cycles per bucket */

	temp = (max_ticks * cvmx_tim->tick_cycles) >> cvmx_tim->bucket_shift;

	/* Round up to nearest power of 2 */
	temp -= 1;
	temp = temp | (temp >> 1);
	temp = temp | (temp >> 2);
	temp = temp | (temp >> 4);
	temp = temp | (temp >> 8);
	temp = temp | (temp >> 16);
	
	cvmx_tim->num_buckets = temp + 1;
	cvmx_tim->num_rings = num_rings;

#if TIM_DEBUG
	cvmx_dprintf("INFO: cvmx_tim_setup: tick_cycles=%lu, bucket_shift=%d, num_buckets=%d, max_ticks=%d\n", cvmx_tim->tick_cycles, 
				cvmx_tim->bucket_shift, cvmx_tim->num_buckets, cvmx_tim->max_ticks);
#endif

	/* Ensure input params fall into permitted ranges */
	if ((cvmx_tim->num_buckets < 3) || cvmx_tim->num_buckets > 1048576) {
		cvmx_dprintf("ERROR: cvmx_tim_setup: num_buckets out of range\n");
		return error;
	}

	/* Allocate the timer buckets from hardware addressable memory */
	mem_block_size = num_rings * cvmx_tim->num_buckets * sizeof(cvmx_tim_bucket_entry_t);

	cvmx_tim->bucket = cvmx_bootmem_alloc(mem_block_size, CVMX_CACHE_LINE_SIZE);
	if (cvmx_tim->bucket == NULL) {
		cvmx_dprintf("ERROR: cvmx_tim_setup: allocation problem, size = %luKB\n", (unsigned long int)(mem_block_size/1000));
		return error;
	}
	memset(cvmx_tim->bucket, 0, mem_block_size);

#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
	/* Initialize FPA pool for timer buffers */
	cvmx_fpa_global_initialize();
	
	/* Setup aura */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX))
	{
		if(timer_config->timer_pool.buffer_count != 0)
		{
			if(__cvmx_helper_initialize_fpa_pool(timer_pool,
				timer_pool_size,
				timer_config->timer_pool.buffer_count,
				"TIM Chunks"))
			{
				cvmx_dprintf("ERROR: %s: failed\n", __func__);
				return -1;
			}
			
#if TIM_DEBUG
			cvmx_dprintf("INFO: %s: timer_pool = %d, timer_pool_size = %lu, buffer_count = %lu\n", 
				__func__,
				timer_pool, timer_pool_size,
				timer_config->timer_pool.buffer_count);
#endif
		}
	}
#endif	/* !CVMX_BUILD_FOR_LINUX_KERNEL */

	cvmx_tim->start_time = 0;

	/* Loop through all timers */
	for(timer_id = 0; timer_id < num_rings; timer_id++)
	{
		int interval = ((1 << (cvmx_tim->bucket_shift - 10)) - 1); /* Required in ticks */
		
		bucket = cvmx_tim->bucket + timer_id * cvmx_tim->num_buckets;
		
		if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
			cvmx_tim_ringx_ctl0_t ring_ctl0;
			cvmx_tim_ringx_ctl1_t ring_ctl1;
			cvmx_tim_ringx_ctl2_t ring_ctl2;
			cvmx_tim_reg_flags_t reg_flags;

			/* Tell the hardware where about the bucket array */
			ring_ctl2.u64 = 0;
			ring_ctl2.cn68xx.csize = timer_pool_size / 8;
			ring_ctl2.cn68xx.base = cvmx_ptr_to_phys(bucket) >> 5;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL2(timer_id), ring_ctl2.u64);

			reg_flags.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);
			ring_ctl1.u64 = 0;
			ring_ctl1.s.cpool = ((reg_flags.s.ena_dfb == 0) ? timer_pool : 0);
			ring_ctl1.s.bsize = cvmx_tim->num_buckets - 1;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL1(timer_id), ring_ctl1.u64);

			ring_ctl0.u64 = 0;
			ring_ctl0.cn68xx.timercount = interval + timer_id * interval / num_rings;
			ring_ctl0.cn68xx.ena = 1;
			ring_ctl0.cn68xx.intc = 1;
			ring_ctl0.cn68xx.interval = interval;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL0(timer_id), ring_ctl0.u64);
			cvmx_read_csr(CVMX_TIM_RINGX_CTL0(timer_id));
		} 
		else if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
				 OCTEON_IS_MODEL(OCTEON_CN73XX) ||
				 OCTEON_IS_MODEL(OCTEON_CNF75XX))
		{
			cvmx_tim_ringx_ctl0_t ring_ctl0;
			cvmx_tim_ringx_ctl1_t ring_ctl1;
			cvmx_tim_ringx_ctl2_t ring_ctl2;
			cvmx_tim_dbg3_t tim_dbg3;
			cvmx_tim_ringx_aura_t ring_aura;
			uint64_t expire_offset = 0;
			
			/* 1. Clear TIM_RING_CTL1 to make sure no ring traversals during programming, en = 0 */
			ring_ctl1.u64 = 0;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL1(timer_id), ring_ctl1.u64);

			/* 2. Wait 1 ring interval period, in core_clocks */
			interval =  cvmx_clock_get_count(CVMX_CLOCK_CORE) *
				(long long) cvmx_tim->tick_cycles /
				(long long) tim_clock_hz ;
			cvmx_wait(interval);
			
			/* 3. Check RINGS_PENDING_VEC to make sure no ring is pending */
			tim_dbg3.u64 = cvmx_read_csr(CVMX_TIM_DBG3);
			if(tim_dbg3.cn78xx.rings_pending_vec)
			{
				cvmx_dprintf("ERROR: cvmx_tim_setup: At least one ring is pending service, rings_pending_vec = %lu\n", (unsigned long int)tim_dbg3.cn78xx.rings_pending_vec);
				return error;
			}
			
			/* 4. Check TIM_CUR_ACTIVE_RINGS to make sure no ring is active */
			/* Only for diag purposes */
			
			/* 5. Set timer ring aura */
#if TIM_DEBUG
			cvmx_dprintf("INFO: %s: Timer%lu: AURA(pool) = %#x\n",
				__func__, timer_id, timer_pool);
#endif
			ring_aura.u64 = 0;
			ring_aura.cn78xx.aura = timer_pool;
			cvmx_write_csr(CVMX_TIM_RINGX_AURA(timer_id), ring_aura.u64);
			
			/* 6. Program bucket information (CTL2) */
#if TIM_DEBUG
			cvmx_dprintf("INFO: %s: Timer%lu: CTL2 csize = %lu, base = %lu\n",
				__func__, timer_id, (timer_pool_size/8),
				(unsigned long)(cvmx_ptr_to_phys(bucket) >> 5));
#endif
			ring_ctl2.u64 = 0;
			ring_ctl2.cn78xx.csize = timer_pool_size / 8; /* Number of eight-byte words per chunk. CSIZE mod(16) should be zero */
			ring_ctl2.cn78xx.base = cvmx_ptr_to_phys(bucket) >> 5; /* Pointer[41:5] to bucket[0] */
			cvmx_write_csr(CVMX_TIM_RINGX_CTL2(timer_id), ring_ctl2.u64);

			/* 7. Setup timings (CTL0) */
			interval = cvmx_tim->tick_cycles;
			expire_offset = interval + (timer_id * interval / num_rings);
#if TIM_DEBUG
			cvmx_dprintf("INFO: %s: Timer%lu: "
				"CTL0 expire_offset = %lu, interval = %d\n",
				__func__, timer_id, expire_offset, interval);
#endif
			ring_ctl0.u64 = 0;
			/* Offset for multiple rings to avoid congestion */
			ring_ctl0.cn78xx.expire_offset =  expire_offset;
			/* Timer interval, in CYCLES, not tick period */
			ring_ctl0.cn78xx.interval = interval;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL0(timer_id), ring_ctl0.u64);

			/* 8. Enable ring (CTL1) */
			/* wait for rcf_busy to go low before proceeding */
			do {
			    ring_ctl1.u64 =
				cvmx_read_csr(CVMX_TIM_RINGX_CTL1(timer_id));
			} while (ring_ctl1.cn78xx.rcf_busy);
			ring_ctl1.u64 = 0;
			ring_ctl1.cn78xx.ena_dfb = 0;	/* Free chunks */
			ring_ctl1.cn78xx.ena_ldwb = 1;	/* DWB */
			ring_ctl1.cn78xx.bucket = 0;	/* 1st bucket */
			ring_ctl1.cn78xx.intc = 1; /* error interrupts not supported yet */
			ring_ctl1.cn78xx.bsize = cvmx_tim->num_buckets - 1;
			ring_ctl1.cn78xx.ena = 1;
			cvmx_write_csr(CVMX_TIM_RINGX_CTL1(timer_id), ring_ctl1.u64);
#if TIM_DEBUG
			cvmx_dprintf("INFO: %s: Timer%lu: "
				"CTL1 bucket = %d, intc = %d, bsize = %d, "
				"ena = %u\n",
				__func__, timer_id,
				ring_ctl1.cn78xx.bucket,
				ring_ctl1.cn78xx.intc,
				ring_ctl1.cn78xx.bsize,
				ring_ctl1.cn78xx.ena);
#endif
		}
		else 
		{
			cvmx_tim_mem_ring0_t config_ring0;
			cvmx_tim_mem_ring1_t config_ring1;
			/* Tell the hardware where about the bucket array */
			config_ring0.u64 = 0;
			config_ring0.s.first_bucket = cvmx_ptr_to_phys(bucket) >> 5;
			config_ring0.s.num_buckets = cvmx_tim->num_buckets - 1;
			config_ring0.s.ring = timer_id;
			cvmx_write_csr(CVMX_TIM_MEM_RING0, config_ring0.u64);

			/* Tell the hardware the size of each chunk block in pointers */
			config_ring1.u64 = 0;
			config_ring1.s.enable = 1;
			config_ring1.s.pool = timer_pool;
			config_ring1.s.words_per_chunk = timer_pool_size / 8;
			config_ring1.s.interval = interval;
			config_ring1.s.ring = timer_id;
			cvmx_write_csr(CVMX_TIM_MEM_RING1, config_ring1.u64);
		}
	}
	
	return 0;
}

/**
 * Start the hardware timer processing
 */
void cvmx_tim_start(void)
{
	cvmx_tim_control_t control; 

	control.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);

	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX))
	{
		control.cn78xx.reset = 1; /* Reset pulse for FR timer */
		control.cn78xx.enable_timers = 1;
	}
	else
	{
		control.s.enable_dwb = 1;
		control.s.enable_timers = 1;
	}

#if TIM_DEBUG
	cvmx_dprintf("INFO: cvmx_tim_start: starting timer\n");
#endif

	/* Remember when we started the timers */
	cvmx_tim->start_time = cvmx_clock_get_count(CVMX_CLOCK_TIM);
	cvmx_write_csr(CVMX_TIM_REG_FLAGS, control.u64);
}

/**
 * Stop the hardware timer processing. Timers stay configured.
 */
void cvmx_tim_stop(void)
{
	cvmx_tim_control_t control;
	
	control.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);

	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX))
	{
		control.cn78xx.enable_timers = 0; 
	}
	else
	{
		control.s.enable_dwb = 0;
		control.s.enable_timers = 0;
	}
	
#if TIM_DEBUG
	cvmx_dprintf("INFO: cvmx_tim_start: stopping timer\n");
#endif
	
	cvmx_write_csr(CVMX_TIM_REG_FLAGS, control.u64);
}

/**
 * Stop the timer. After this the timer must be setup again
 * before use.
 */
void cvmx_tim_shutdown(void)
{
	uint32_t bucket;
	uint64_t timer_id;
	uint64_t entries_per_chunk;
	int timer_pool = (int)cvmx_fpa_get_timer_pool();
	uint64_t timer_pool_size = cvmx_fpa_get_timer_pool_block_size();
	unsigned num_rings;

	/* Make sure the timers are stopped */
	cvmx_tim_stop();

	entries_per_chunk = timer_pool_size / 8 - 1;
	num_rings = cvmx_tim->num_rings;

	/* Now walk all buckets freeing the chunks */
	for (timer_id = 0; timer_id < num_rings; timer_id++) {
#if TIM_DEBUG
		cvmx_dprintf("INFO: %s: Ring %u\n", __func__,
			(unsigned) timer_id);
#endif
		for (bucket = 0; bucket < cvmx_tim->num_buckets; bucket++) {
			uint64_t chunk_addr;
			uint64_t next_chunk_addr;
			cvmx_tim_bucket_entry_t *bucket_ptr = cvmx_tim->bucket + timer_id * cvmx_tim->num_buckets + bucket;
			CVMX_PREFETCH128(CAST64(bucket_ptr));	/* prefetch the next cacheline for future buckets */

			/* Each bucket contains a list of chunks */
			chunk_addr = bucket_ptr->first_chunk_addr;
			while (bucket_ptr->num_entries) {
#ifdef DEBUG
				cvmx_dprintf("Freeing Timer Chunk 0x%llx\n", CAST64(chunk_addr));
#endif
				/* Read next chunk pointer from end of the current chunk */
				next_chunk_addr = cvmx_read_csr(CVMX_ADD_SEG(CVMX_MIPS_SPACE_XKPHYS, chunk_addr + timer_pool_size - 8));

				cvmx_fpa_free(cvmx_phys_to_ptr(chunk_addr), timer_pool, 0);
				chunk_addr = next_chunk_addr;
				if (bucket_ptr->num_entries > entries_per_chunk)
					bucket_ptr->num_entries -= entries_per_chunk;
				else
					bucket_ptr->num_entries = 0;
			}
		}
	}
}

const char tim_named_block_name[CVMX_BOOTMEM_NAME_LEN] = "TIMER_NAMED_BLOCK";

void cvmx_create_tim_named_block_once(void)
{
	if (timer_config)
		return;	/* tim_named_block already created - return */

	/* Create named block (allocate memory) for struct timer_control and
	 * struct cvmx_tim and assign the global timer_config to point there
	 */
	timer_config =
	(cvmx_tim_config_t *) cvmx_bootmem_alloc_named_range_once(
		sizeof(cvmx_tim_config_t) + sizeof(cvmx_tim_t),
		0, 0,
		32, tim_named_block_name,
		NULL
	);
	if (timer_config == NULL) {
		return;	/* out of memory */
	}
	/* set defaults */
	timer_config->timer_pool.pool_num = 3;
	timer_config->timer_pool.buffer_size = 1024;
	timer_config->timer_pool.buffer_count = 16;
	/* The struct cvmx_tim will be immediately after the struct timer_config
	 * Assign the global cvmx_tim to point to it
	 */
	cvmx_tim = (void *)timer_config + sizeof(cvmx_tim_config_t);
}

void cvmx_tim_set_fpa_pool_config(int64_t pool, uint64_t buffer_size,
			       uint64_t buffer_count)
{
#if TIM_DEBUG
	cvmx_dprintf("INFO: cvmx_tim_set_fpa_pool_config: pool = %lu, buffer_size = %lu, buffer_count = %lu\n", pool, buffer_size, buffer_count);
#endif
	/* Create named block (allocate memory) for struct timer_control and
	 * struct cvmx_tim and assign the global timer_config to point there
	 */
	cvmx_create_tim_named_block_once();
	if (timer_config == NULL) {
		return;	/* out of memory */
	}
	timer_config->timer_pool.pool_num = pool;
	timer_config->timer_pool.buffer_size = buffer_size;
	timer_config->timer_pool.buffer_count = buffer_count;
}

void cvmx_tim_get_fpa_pool_config(cvmx_fpa_pool_config_t *timer_pool)
{
	/* Create named block (allocate memory) for struct timer_control and
	 * struct cvmx_tim and assign the global timer_config to point there
	 */
	cvmx_create_tim_named_block_once();
	if (timer_config == NULL) {
		timer_pool = NULL;
		return;	/* out of memory */
	}

	*timer_pool = timer_config->timer_pool;
}
