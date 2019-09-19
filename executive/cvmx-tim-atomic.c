/***********************license start***************
 * Copyright (c) 2003-2014  Cavium Inc. (support@cavium.com). All rights
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
 * New lockless timer API, inspired by ODP timer definition.
 *
 *
 * <hr>$Revision: 95861 $<hr>
 */

#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-bootmem.h"
#include "cvmx-clock.h"
#include "cvmx-fpa.h"
#include "cvmx-wqe.h"
#include "cvmx-tim-atomic.h"
#include "cvmx-global-resources.h"

/* Include stdatomic if we are using C11 code.  */
#ifdef __GNUC__
# if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#  include <stdatomic.h>
#  define __HAS_ATOMIC
# endif
#endif

static const int debug = 0;

#ifndef __HAS_ATOMIC
/* Used for readability from C11, define our own versions.
 * Documentation on the atomic functions built into GCC 4.7 found here:
 * http://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/_005f_005fatomic-Builtins.html
 */
#undef _Atomic
#define	_Atomic
#define atomic_fetch_sub_explicit(PTR,VAL,AMM) __atomic_fetch_sub ((PTR), (VAL), (AMM))
#define atomic_fetch_add_explicit(PTR,VAL,AMM) __atomic_fetch_add ((PTR), (VAL), (AMM))
#define atomic_store_explicit(PTR,VAL,AMM) __atomic_store_n((PTR), (VAL), (AMM))
#define atomic_exchange_explicit(PTR,VAL,AMM) __atomic_exchange_n((PTR), (VAL), (AMM))
#define atomic_load_explicit(PTR,AMM) __atomic_load_n((PTR), (AMM))
#define memory_order_seq_cst __ATOMIC_SEQ_CST

#endif

/** Default atomic memory model, be conservative for now */
#define	AMM	memory_order_seq_cst
#define ASSERT(s) assert(s)	/* Internal validation */
#define STATIC_ASSERT(s) __extension__({_Static_assert(s, #s); })


#define	CVMX_TIMER_MAGIC	0x2838485861166226ull

typedef struct {
	uint64_t first_chunk_paddr;
#if __BYTE_ORDER == __BIG_ENDIAN
	_Atomic uint32_t num_entries;
	_Atomic int32_t chunk_remainder;
#else
	_Atomic int32_t chunk_remainder;
	_Atomic uint32_t num_entries;
#endif
	/* the remaining 16 bytes are not touched by hardware */
	uint64_t curr_chunk;
	uint64_t pad;
} cvmx_tim_atomic_bucket_entry_t;

union cvmx_timer_info_u {
	uint64_t raw_u64[4];	/* 32-bytes size boundary */
	struct {
		uint64_t	start_cycle;
#if __BYTE_ORDER == __BIG_ENDIAN
		uint32_t	tick_ns;
		uint32_t	tick_cycles;
		uint64_t	chunk_size:24;
		uint64_t	fpa_pool:16;
		uint64_t	num_buckets_exp:6;
		uint64_t	node:2;
		uint64_t	ring:8;
		uint64_t	flags:8;
#else
		uint32_t	tick_cycles;
		uint32_t	tick_ns;
		uint64_t	flags:8;
		uint64_t	ring:8;
		uint64_t	node:2;
		uint64_t	num_buckets_exp:6;
		uint64_t	fpa_pool:16;
		uint64_t	chunk_size:24;
#endif
		uint64_t	magic;
	};
	/* Bucket 0 starts right after this structure */
};

static int cvmx_timer_hw_init(cvmx_timer_id_t timer_id);
static int cvmx_timer_hw_stop(cvmx_timer_id_t timer_id);
static unsigned cvmx_timer_curr_bucket(cvmx_timer_id_t timer_id);
static unsigned cvmx_timer_min_resolution(void);
static int cvmx_timer_resource_alloc(unsigned node);
static int cvmx_timer_resource_free(unsigned node, unsigned ring_id);

/**
 * Determine TIM core revision, based on SoC model
 */
static int cvmx_timer_version(void)
{
	static int processed = 0;
	if (processed)
		return processed;
	
	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		OCTEON_IS_MODEL(OCTEON_CNF75XX))
		processed = 3;
	else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		processed = 2;
	else
		processed = 1;

	return processed;
}

/**
 * Hardware-specific maximum ring size (i.e. number of buckets) 
 * Return \<n> where 2^n is the maximum ring size.
 * @INTERNAL
 */
static unsigned
cvmx_timer_max_ring_exp(void)
{
	static unsigned processed = 0;
	if (processed)
		return processed;

	if (cvmx_timer_version() > 1)
		processed = 20;
	else
		processed = 10;

	return processed;
}

/**
 * Model-specific number of timer rings per chip
 * @INTERNAL
 */
static unsigned
cvmx_timer_rings_per_node(void)
{
	static unsigned processed = 0;
	if (processed)
		return processed;

	if (cvmx_timer_version() > 1)
		processed = 64;
	else
		processed = 16;

	return processed;
}

static inline void * cvmx_timer_chunk_alloc(cvmx_timer_id_t timer_id)
{
	unsigned timer_pool = timer_id->fpa_pool;
	return cvmx_fpa_alloc(timer_pool);
}

static inline void cvmx_timer_chunk_free(cvmx_timer_id_t timer_id, void *ptr)
{
	unsigned timer_pool = timer_id->fpa_pool;
	cvmx_fpa_free(ptr, timer_pool, 0);
}


/**
 *
 * @return Returns a timeout handle that can be used to cancel timeout.
 */
static void *
cvmx_timer_tmo_insert(cvmx_timer_id_t timer_id, unsigned bucket_index,
			uint64_t sso_word)
{
	cvmx_tim_atomic_bucket_entry_t *bucket_ptr; 
	int chunk_size;
	_Atomic int32_t *remainder_ptr;
	uint64_t *curr_chunk;
	uint64_t *new_chunk = NULL;
	uint64_t new_paddr;
	int32_t remainder;

	STATIC_ASSERT(sizeof(*timer_id) == sizeof(*bucket_ptr));
	ASSERT(bucket_index < (1u << timer_id->num_buckets_exp));

	/* Pointer to the bucket structure we need to handle */
	bucket_ptr = (cvmx_tim_atomic_bucket_entry_t*) timer_id;
	bucket_ptr += 1 + bucket_index;

	/* Number of work entries in a chunk */
	chunk_size = timer_id->chunk_size - 1;

	/* Address of the semaphore */
	remainder_ptr = &bucket_ptr->chunk_remainder;

	/*
	 * Here is how the semaphore operates:
	 * A positive value allows multiple entry;
	 * A zero value allows a single entry;
	 * A negative value - wait in the pan;
	 */

__retry:
	/* This is the main lock semaphore */
	remainder = atomic_fetch_sub_explicit(remainder_ptr, 1, AMM);

	if (cvmx_unlikely(remainder < 0)) {
		/* 
		 * Pan: another thread has the remainder==0 case
		 * Wait until release and retry.
		 */
		while(atomic_load_explicit(remainder_ptr,AMM) < 0)
			/* Wait */ ;;
		goto __retry;
	}

	/* <curr_chunk> contains vaddr of chunk inside uint64_t */
	curr_chunk = (void *)(uintptr_t) bucket_ptr->curr_chunk;

	if (cvmx_likely(remainder > 0))
		{
		if (debug > 2)
			cvmx_dprintf("%s: chunk at %p remainder %d\n",
				__func__, curr_chunk, (int)remainder);

		/* Sufficient room in current chunk */
		ASSERT(remainder < chunk_size);

		/* Our slot is per pre-decrement remainder */
		curr_chunk += chunk_size - remainder;

		/* Store work entry */
		atomic_store_explicit(curr_chunk, sso_word, AMM);

		/* Atomically increment entry count */
		atomic_fetch_add_explicit(&bucket_ptr->num_entries, 1, AMM);

		/* vaddr of the work entry is the handle */
		return(curr_chunk);
		}

	/* This code branch is exclusive - only one thread may enter it */
	ASSERT(remainder == 0);

	/* Allocate new chunk and try to insert it to the bucket */
	new_chunk = cvmx_timer_chunk_alloc(timer_id);
	if (cvmx_unlikely(new_chunk == NULL))
		{
		/* Reset to 0, release another thread from waiting pan */
		atomic_store_explicit(remainder_ptr, 0, AMM);
		return NULL;
		}

	new_paddr = cvmx_ptr_to_phys(new_chunk);

	if (debug > 1)
		memset( new_chunk, 0xee, sizeof(uint64_t)*timer_id->chunk_size);

	/* NOTE:
	 * TIM processes a bucket without zeroing the FIRST_CHUNK field.
	 */

	/* Check if the first chunk needs to be allocated */
	if (atomic_load_explicit(&bucket_ptr->num_entries,AMM) == 0)
		{
		bucket_ptr->first_chunk_paddr = new_paddr;
		}
	/* Additional chunk need to be hooked up */
	else
		{
		/* Store chunk link pointer in last chunk word */
		curr_chunk[chunk_size] = new_paddr;
		}

	/* Save new chunk as current */
	curr_chunk = new_chunk;
	bucket_ptr->curr_chunk = (uint64_t)(uintptr_t) curr_chunk;

	/* New chunk allocated, store work in first word */
	atomic_store_explicit(curr_chunk, sso_word, AMM);

	/* Atomically increment entry count */
	atomic_fetch_add_explicit(&bucket_ptr->num_entries, 1, AMM);

	/* Last op is to unlock other cores by bumping up remainder */
	atomic_store_explicit(remainder_ptr, chunk_size - 1, AMM);

	return curr_chunk;
}

/**
 * Encode a work queue entry to the timer work entry word
 * @INTERNAL
 */
static inline uint64_t
cvmx_timer_wqe_encode(cvmx_wqe_t *wqe)
{
	if(wqe == NULL) {
		return 0ULL;
	} else if (cvmx_timer_version() == 3) {
		/* TIM_MEM_S structure in chunk */
		return
			(uint64_t)cvmx_ptr_to_phys(wqe) |
			(uint64_t)cvmx_wqe_get_xgrp(wqe) << 52 |
			(uint64_t)cvmx_wqe_get_tt(wqe) << 50;
	} else {
		/* pointer to wqe address in timer chunk */
		return cvmx_ptr_to_phys(wqe);
	}
}


/**
 * Request timeout with an absolute timer tick
 *
 */
cvmx_timer_status_t
cvmx_timer_arm_absolute(cvmx_timer_id_t timer_id, uint64_t abs_tick,
		cvmx_wqe_t *wqe, cvmx_timer_handle_t *handle_ptr)
{
	int64_t ticks_from_now;
	uint64_t curr_tick;

	if (timer_id->magic != CVMX_TIMER_MAGIC)
		return CVMX_TIMER_STATUS_HANDLE;	/* Invalid magic */
	curr_tick = cvmx_timer_current_tick(timer_id);
	ticks_from_now = (int64_t)abs_tick - curr_tick;
	
	return cvmx_timer_arm_relative(timer_id, ticks_from_now, wqe, handle_ptr);
}

/**
 * Request timeout with a relative timer tick
 *
 */
cvmx_timer_status_t
cvmx_timer_arm_relative(cvmx_timer_id_t timer_id, int64_t ticks_from_now,
		cvmx_wqe_t *wqe, cvmx_timer_handle_t *handle_ptr)
{
	unsigned bucket_index;
	unsigned curr_bucket;
	uint64_t work_word;
	unsigned long long now;
	void *handle;
	if (timer_id->magic != CVMX_TIMER_MAGIC)
		return CVMX_TIMER_STATUS_HANDLE;	/* Invalid magic */

	now = cvmx_timer_current_tick(timer_id);

	/* Make sure the specified time won't wrap our bucket list */
	if (ticks_from_now >= ((1ll << timer_id->num_buckets_exp) -1)) {
		cvmx_dprintf("%s: WARNING: "
			"Failed to arm a timer too far out: now+%lld\n",
			__func__, (long long signed) ticks_from_now);
		return CVMX_TIMER_STATUS_TOO_FAR_AWAY;
	}

	/* Since we have no way to synchronize, we can't update a timer that is
	   being used by the hardware. Two buckets forward should be safe */
	if (ticks_from_now < 2) {
		cvmx_dprintf("%s: WARNING: "
			"Timeout override from now%+lld\n",
			__func__, (long long signed) ticks_from_now);
		ticks_from_now = 2;
	}

	/* Relative tick count is the bucket index, wrapped around ring size */
	curr_bucket = cvmx_timer_curr_bucket(timer_id);
	bucket_index = curr_bucket + (unsigned) ticks_from_now;
	bucket_index &= (1u << timer_id->num_buckets_exp) -1;

	if (debug)
		cvmx_dprintf("INFO: %s: rtick=%u curr buck=%u into buck=%u\n",
			__func__, (unsigned) ticks_from_now,
			curr_bucket, bucket_index);

	/* Encode the WQE into the work entry word */
	work_word = cvmx_timer_wqe_encode(wqe);

	/* Insert work entry word into the bucket list */
	handle = cvmx_timer_tmo_insert(timer_id, bucket_index, work_word);

	if (debug)
		cvmx_dprintf("INFO: %s: work %llx at %p\n", __func__,
			(unsigned long long) work_word, handle);

	if (handle == NULL)
		return CVMX_TIMER_STATUS_NO_MEMORY;


	/* Fill in timeout handle to be used for cvmx_timer_cancel() */
	if (handle_ptr != NULL) {
		handle_ptr->word[0] = now + ticks_from_now - 2;
		handle_ptr->word[1] = (uint64_t)(uintptr_t) handle;
		/* make it ready to use instantly */
		CVMX_SYNCW;
	}

	return CVMX_TIMER_STATUS_SUCCESS;
}

/**
 * Cancel a timeout using the handle returned from cvmx_timer_arm
 *
 * @return Returns the work entry pointer that was associated
 * with the cancelled timeout.
 *
 */
cvmx_wqe_t *
cvmx_timer_cancel(cvmx_timer_id_t timer_id, cvmx_timer_handle_t *handle)
{
	cvmx_wqe_t *wqe;
	uint64_t word;
	_Atomic uint64_t *handleptr;

	if (handle == NULL)
		return NULL;

	/* Check if its not too late to cancel */
	word = cvmx_timer_current_tick(timer_id);
	if (handle->word[0] < word) {
		if (debug)
			cvmx_dprintf("ERROR: %s:(too late) exp %llu now %llu\n",
				__func__,
				(unsigned long long) handle->word[0],
				(unsigned long long) word);

		return NULL;
	}

	handleptr = (void *)(uintptr_t) handle->word[1];

	if (debug)
		cvmx_dprintf("INFO: %s: exp %llu now %llu at %p\n",__func__,
			(unsigned long long) handle->word[0],
			(unsigned long long) word, handleptr);

	word = atomic_exchange_explicit(handleptr, 0, AMM);

	/* Mask off physical address on CN78XX */
	wqe = cvmx_phys_to_ptr(word & ((1ull << 42)-1));

	if (debug)
		cvmx_dprintf("INFO: %s: work %llx at %p\n", __func__,
			(unsigned long long) word, handleptr);

	return wqe;
}

/**
 * Return current timer tick value in 'internal_ticks' = requested min_tmo / 2)
 *
 * This should be used to convert relative to absolute timeouts
 */
uint64_t
cvmx_timer_current_tick(cvmx_timer_id_t timer_id)
{
	return cvmx_clock_get_count(CVMX_CLOCK_TIM) / timer_id->tick_cycles;
}

/**
 * Return maximum timeout value for a timer in timer ticks it is
 * (num_buckets-1)ticks, because current bucket should not be overwritten
 */
uint64_t
cvmx_timer_maximum_tmo(cvmx_timer_id_t timer_id)
{
	return (1ull << timer_id->num_buckets_exp) - 1;
}

/**
 * Convert timer 'internal' ticks to nano-seconds
 */
uint64_t
cvmx_timer_tick_to_ns(cvmx_timer_id_t timer_id, uint64_t ticks)
{
	return timer_id->tick_ns * ticks;
}

/**
 * Convert time in nano-seconds into timer 'internal' ticks
 */
uint64_t
cvmx_timer_ns_to_tick(cvmx_timer_id_t timer_id, uint64_t ns)
{
	return ns / timer_id->tick_ns;
}

/**
 * Initialize named memory block on atomic allocation
 * @INTERNAL
 */
static void
cvmx_timer_mem_init(void *ptr)
{
	/* Zero out control structure, rings will be zeroed later */
	memset(ptr, 0, sizeof(union cvmx_timer_info_u));
}

/**
 * Create a timer instance
 *
 */
cvmx_timer_id_t
cvmx_timer_create(const char *_name, unsigned node, unsigned pool,
		uint64_t min_tmo, uint64_t max_tmo)
{
	unsigned mem_sz;
	void *mem_ptr;
	cvmx_timer_id_t timer_id;
	unsigned bucket_count, bucket_count_exp;
	uint64_t clock_hz, tick_cycles;
	unsigned ring_id;
	unsigned pool_bufsiz;
	uint64_t tick_ns;
	char name[100];

	/* Allocate a timer hardware unit */
	ring_id = cvmx_timer_resource_alloc(node);
	if (ring_id > cvmx_timer_rings_per_node()) {
		return NULL;
	}
	
	/* Calculate resolution */
	tick_ns = cvmx_timer_min_resolution();
	if ((min_tmo >> 1) > tick_ns)
		tick_ns = min_tmo >> 1;

	/* Calculate number of buckets as 2^N */
	bucket_count = max_tmo / tick_ns + 1;
	for (bucket_count_exp =0; bucket_count_exp  < 32; bucket_count_exp ++) {
		if ((1u << bucket_count_exp ) > bucket_count)
			break;
	}

	/* Validate <N> is in range */
	if (bucket_count_exp < 8)
		bucket_count_exp = 8;
	if (bucket_count_exp  >= 32 ||
	    bucket_count_exp > cvmx_timer_max_ring_exp()) {
		return NULL;	/* ring too big */
	}

	if (debug)
		cvmx_dprintf("INFO: %s: tick_ns=%lu buckets=%lu\n",
			__func__, (long) tick_ns, 1lu << bucket_count_exp);

	/* Calculate named block size */
	STATIC_ASSERT(sizeof(*timer_id) ==
		sizeof(cvmx_tim_atomic_bucket_entry_t));
	mem_sz = (1 << bucket_count_exp) + 1;
	mem_sz *= sizeof(*timer_id);

	/* Make sure the pool is alive */
	pool_bufsiz = cvmx_fpa_get_block_size(pool);
	if (pool_bufsiz == 0) {
		return NULL;	/* pool is uninitialized */
	}

	if (debug)
		cvmx_dprintf("INFO: %s: pool=%u bufsize=%u\n",
			__func__, pool, pool_bufsiz);
	sprintf(name, "%s.%d", _name, node);
	/* Allocate memory for control struct, ring */
	mem_ptr = cvmx_bootmem_alloc_named_range_once(
		mem_sz,
		0, 0,
		32, name,
		cvmx_timer_mem_init
		);

	if (mem_ptr == NULL) {
		return NULL;	/* out of memory */
	}

	timer_id = mem_ptr;
	if (timer_id->magic != 0ULL) {
		return NULL;	/* name already taken */
	}

	/* Clock that feeds the timer frequency */
 	clock_hz =  cvmx_clock_get_rate(CVMX_CLOCK_TIM);

	/* Calculate how many clock cycles per tick */
	tick_cycles = tick_ns * clock_hz / 1000000000ULL ;

	/* Recompute tick_ns to account for rounding error in 'tick_cycles' */
	tick_ns = (1000000000ULL * tick_cycles) / clock_hz;

	if (debug)
		cvmx_dprintf("INFO: %s: tick_ns=%llu clock=%llu "
			"tick_cycles=%llu\n",
			__func__, (unsigned long long)tick_ns,
			(unsigned long long)clock_hz,
			(unsigned long long)tick_cycles);

	/* Fill in most info fields */
	timer_id->node = node;
	timer_id->tick_ns = tick_ns;
	timer_id->tick_cycles = tick_cycles;
	timer_id->ring = ring_id;
	timer_id->fpa_pool = pool;
	timer_id->num_buckets_exp = bucket_count_exp;
	timer_id->chunk_size = pool_bufsiz >> 3;


	/* Watch for overflow */
	ASSERT(timer_id->tick_ns == tick_ns);
	ASSERT(timer_id->tick_cycles == tick_cycles);

	/* Zero out all buckets in the ring */
	memset(&timer_id[1], 0, 
		sizeof(cvmx_tim_atomic_bucket_entry_t) << bucket_count_exp);

	/* Flush ring before handing off to hardware */
	CVMX_SYNCW;

	/* Configure, enable the hardware unit per timer attributes */
	if (cvmx_timer_hw_init(timer_id) < 0) {
		cvmx_bootmem_free_named(name);
		return NULL;	/* chip-specific error */
	}

	/* Now the timer is available to other cores too */
	timer_id->magic = CVMX_TIMER_MAGIC;
	timer_id->start_cycle = cvmx_timer_current_tick(timer_id);
	CVMX_SYNCW;
	return timer_id;
}

/**
 * Retreive timer handle based on name
 *
 */
cvmx_timer_id_t
cvmx_timer_id_from_name(const char *name)
{
	void * mem_ptr;
	cvmx_timer_id_t timer_id;
	const cvmx_bootmem_named_block_desc_t *desc;

	desc = cvmx_bootmem_find_named_block(name);

	if (desc == NULL) {
		return NULL;	/* not found */
	}

	mem_ptr = cvmx_phys_to_ptr(desc->base_addr);

	timer_id = mem_ptr;
	if (timer_id != NULL && timer_id->magic == CVMX_TIMER_MAGIC) {
		return timer_id;	/* found */
	}

	return NULL;	/* misc error */
}

/**
 * Hardware-specific minimum tick resolution
 * Returns nanosecond of minumum tick duration.
 *
 * This function is called before the per-ring
 * intialization, and may apply global initialization
 * if required.
 *
 * @INTERNAL
 */
static unsigned
cvmx_timer_min_resolution(void)
{
	unsigned ret;
	if (cvmx_timer_version() == 3)
		ret = 1000;	/* 1usec */
	else if (cvmx_timer_version() == 2) {
		uint64_t fr_rn_tt_ns = 1024; /* Default prescaler clock */
		cvmx_tim_fr_rn_tt_t fr_rn_tt;
		cvmx_tim_control_t control; 

		/*
		 * The 68xx resulution is not fixed,
		 * but depends on a global prescaler
		 * that can be changed only if the timer
		 * is globally disabled.
		 */
		control.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);
		fr_rn_tt.u64 = cvmx_read_csr(CVMX_TIM_FR_RN_TT);

		if (control.cn68xx.enable_timers == 0) {
			/* Timer is stopped, initialize prescaler value */
			fr_rn_tt.cn68xx.fr_rn_tt =
				fr_rn_tt_ns *
				cvmx_clock_get_rate(CVMX_CLOCK_TIM)
				/ 1000000000ULL;
			cvmx_write_csr(CVMX_TIM_FR_RN_TT, fr_rn_tt.u64);
		} else {
			/* If global timer is running, use prescaler value */
			fr_rn_tt_ns = fr_rn_tt.cn68xx.fr_rn_tt * 1000000000ULL
					/ cvmx_clock_get_rate(CVMX_CLOCK_TIM);

		}
		ret = fr_rn_tt_ns;	/* fastest tick period in ns */
	} else {
		/* legacy models: 1024 CLOCK_TIM cycles */
		ret = 1024 * 1000000000ULL 
			/ cvmx_clock_get_rate(CVMX_CLOCK_TIM);
	}
	return ret;
}
/**
 * Destroy a shared timer
 *
 * The application is required to make sure all timeouts have expired
 * prior to destroying the ring to avoid memory leaks.
 */
cvmx_timer_status_t
cvmx_timer_destroy(cvmx_timer_id_t timer_id, const char *_name)
{
	cvmx_tim_atomic_bucket_entry_t *bucket;
	unsigned bucket_index, ring_id;
	const cvmx_bootmem_named_block_desc_t *desc;
	void *mem_ptr;
	int res;
	char name[100];

	sprintf(name, "%s.%d", _name, timer_id->node);
	desc = cvmx_bootmem_find_named_block(name);
	if (desc == NULL)
		return CVMX_TIMER_STATUS_NOT_FOUND;	/* invalid name */

	mem_ptr = cvmx_phys_to_ptr(desc->base_addr);
	if (mem_ptr != (void *)timer_id)
		return CVMX_TIMER_STATUS_MISMATCH;	/* name mismatch */

	if (timer_id->magic != CVMX_TIMER_MAGIC)
		return CVMX_TIMER_STATUS_HANDLE;	/* Invalid magic */


	/* Clear magic number to stop client use */
	timer_id->magic = 0;
	bucket = (void *) &timer_id[1];

	/* Cause the ring to stop */
	if (cvmx_timer_hw_stop(timer_id) < 0)
		return CVMX_TIMER_STATUS_BUSY;

	/* Save HW ring# for resource freeing */
	ring_id = timer_id->ring;

	/* We assume that all WQEs have expired
	 * and free only the chunks themselves
	 */
	
	/* iterate through all buckets, and free all chunks that are attached */
	for(bucket_index = 0;
	    bucket_index < (1u << timer_id->num_buckets_exp);
	    bucket_index ++)
	{
		uint64_t paddr;
		uint64_t *chunk;
		cvmx_tim_atomic_bucket_entry_t *bptr;
		uint32_t num_ent, n;

		n = timer_id->chunk_size - 1;
		bptr = &bucket[bucket_index];
		paddr = bptr->first_chunk_paddr;
		while((num_ent = atomic_load_explicit(&bptr->num_entries, AMM))>0)
		{
			chunk = cvmx_phys_to_ptr(paddr);
			paddr = chunk[n];
			cvmx_timer_chunk_free(timer_id, chunk);
			if (num_ent > n)
				atomic_fetch_sub_explicit(&bptr->num_entries,
					n, AMM);
			else
				atomic_fetch_sub_explicit(&bptr->num_entries,
					num_ent, AMM);
		}
	} /* for bucket index */

	/* Now free the named block of the timer */
	cvmx_bootmem_free_named(name);

	/* release global resource. */
	res = cvmx_timer_resource_free(timer_id->node, ring_id);

	return res;
}

static int
cvmx_timer_init_cn78xx(cvmx_timer_id_t timer_id)
{
	cvmx_tim_control_t control; 
	cvmx_tim_ringx_ctl0_t ring_ctl0;
	cvmx_tim_ringx_ctl1_t ring_ctl1;
	cvmx_tim_ringx_ctl2_t ring_ctl2;
	cvmx_tim_ringx_aura_t ring_aura;
	cvmx_tim_dbg3_t tim_dbg3;
	unsigned interval, ring_id, node;
	uint64_t expire_offset;
	uint64_t bucket_paddr;
	
	node = timer_id->node;
	ring_id = timer_id->ring;

	/* Physical address of the first bucket in the ring */
	bucket_paddr = cvmx_ptr_to_phys(&timer_id[1]);

	/* Make sure the ring is disabled */
	ring_ctl1.u64 = cvmx_read_csr_node(node, CVMX_TIM_RINGX_CTL1(ring_id));
	if (ring_ctl1.cn78xx.ena != 0 )
		return -1;

	/* Check RINGS_PENDING_VEC to make sure our ring is not pending */
	tim_dbg3.u64 = cvmx_read_csr_node(node, CVMX_TIM_DBG3);
	if(tim_dbg3.cn78xx.rings_pending_vec & (1u << ring_id))
		{
		cvmx_dprintf("ERROR: %s: "
			"At least one ring is pending service, "
			"rings_pending_vec = %lu\n",
			__func__,
			 (unsigned long int)tim_dbg3.cn78xx.rings_pending_vec);
			return -1;
		}
			
	/* Set timer ring aura */
	if (debug)
		cvmx_dprintf("INFO: %s: Timer%u: AURA(pool) = %#x\n",
			__func__, ring_id, timer_id->fpa_pool);

	ring_aura.u64 = 0;
	ring_aura.cn78xx.aura = timer_id->fpa_pool | (cvmx_get_node_num() << 10);
	cvmx_write_csr_node(node, CVMX_TIM_RINGX_AURA(ring_id), ring_aura.u64);

	/* program bucket information (CTL2) */

	ring_ctl2.u64 = 0;

	/* CSIZE mod(16) should be zero */
	ASSERT((timer_id->chunk_size & 0xf) == 0);
	/* Number of eight-byte words per chunk. */
	ring_ctl2.cn78xx.csize = timer_id->chunk_size;
	ASSERT(ring_ctl2.cn78xx.csize == timer_id->chunk_size);

	ASSERT((bucket_paddr & ((1 << 5)-1)) == 0);
	/* Pointer[41:5] to bucket[0] */
	ring_ctl2.cn78xx.base = bucket_paddr >> 5;

	if (debug) cvmx_dprintf("INFO: %s: "
		"Timer%u: CTL2 csize = %u, base = %#x\n",
		__func__, ring_id,
		(unsigned) ring_ctl2.cn78xx.csize,
		(unsigned) ring_ctl2.cn78xx.base);

	cvmx_write_csr_node(node, CVMX_TIM_RINGX_CTL2(ring_id), ring_ctl2.u64);

	/* Setup timings (CTL0) */
	interval = timer_id->tick_cycles - 1;
	expire_offset = interval + (ring_id * interval / 
		cvmx_timer_rings_per_node());

	if (debug) cvmx_dprintf("INFO: %s: Timer%u: "
		"CTL0 expire_offset = %llu, interval = %d\n",
		__func__, ring_id, (long long)expire_offset, interval);

	ring_ctl0.u64 = 0;
	/* Offset for multiple rings to avoid congestion */
	ring_ctl0.cn78xx.expire_offset =  expire_offset;
	/* Timer interval, in CYCLES, not tick period */
	ring_ctl0.cn78xx.interval = interval;
	cvmx_write_csr_node(node, CVMX_TIM_RINGX_CTL0(ring_id), ring_ctl0.u64);

	/* 8. Enable ring (CTL1) */
	/* wait for rcf_busy to go low before proceeding */
	do {
	    ring_ctl1.u64 = cvmx_read_csr_node(node,
			CVMX_TIM_RINGX_CTL1(ring_id));
	} while (ring_ctl1.cn78xx.rcf_busy);

	ring_ctl1.u64 = 0;
	ring_ctl1.cn78xx.ena_dfb = 0;	/* Free chunks */
	ring_ctl1.cn78xx.ena_ldwb = 1;	/* DWB */
	ring_ctl1.cn78xx.bucket = 0;	/* 1st bucket */
	ring_ctl1.cn78xx.intc = 1; /* error interrupts not supported yet */
	ring_ctl1.cn78xx.bsize = (1u << timer_id->num_buckets_exp) - 1;
	ASSERT(ring_ctl1.cn78xx.bsize == (1u << timer_id->num_buckets_exp) - 1);
	ring_ctl1.cn78xx.ena = 1;
	cvmx_write_csr_node(node, CVMX_TIM_RINGX_CTL1(ring_id), ring_ctl1.u64);

	if (debug) cvmx_dprintf("INFO: %s: Timer%u: "
		"CTL1 bucket = %d, intc = %d, bsize = %d, ena = %u\n",
		__func__, ring_id,
		ring_ctl1.cn78xx.bucket, ring_ctl1.cn78xx.intc,
		ring_ctl1.cn78xx.bsize, ring_ctl1.cn78xx.ena);


	/* Start the global timer if it's stopped */
	control.u64 = cvmx_read_csr_node(node, CVMX_TIM_REG_FLAGS);

	if (control.cn78xx.enable_timers == 0) {
		control.cn78xx.reset = 1; /* Reset pulse for FR timer */
		control.cn78xx.enable_timers = 1;
		cvmx_write_csr_node(node, CVMX_TIM_REG_FLAGS, control.u64);
	}

	return 0;
}


static int
cvmx_timer_init_cn68xx(cvmx_timer_id_t timer_id)
{	
	cvmx_tim_control_t control; 
	cvmx_tim_ringx_ctl0_t ring_ctl0;
	cvmx_tim_ringx_ctl1_t ring_ctl1;
	cvmx_tim_ringx_ctl2_t ring_ctl2;
	cvmx_tim_ringx_dbg1_t ring_dbg1;
	uint64_t fr_rn_tt_ns;
	cvmx_tim_fr_rn_tt_t fr_rn_tt;
	unsigned interval, ring_id;
	uint64_t expire_offset;
	uint64_t bucket_paddr;
	long long clock_hz;
	
	ring_id = timer_id->ring;
 	clock_hz =  cvmx_clock_get_rate(CVMX_CLOCK_TIM);

	/* Physical address of the first bucket in the ring */
	bucket_paddr = cvmx_ptr_to_phys(&timer_id[1]);
	ASSERT((bucket_paddr >> 36) == 0);	/* expect [35:0] addr (CTL2) */

	/* Make sure the ring is disabled (CTL0) */
	ring_ctl0.u64 = cvmx_read_csr(CVMX_TIM_RINGX_CTL0(ring_id));
	if (ring_ctl0.cn68xx.ena != 0 )
		return -1;

	/* wait for ring_esr == 0 before proceeding */
	do {
		ring_dbg1.u64 = cvmx_read_csr(CVMX_TIM_RINGX_DBG1(ring_id));
	} while (ring_dbg1.cn68xx.ring_esr != 0);

	/* TIM prescaler has already been set at this point */
	fr_rn_tt.u64 = cvmx_read_csr(CVMX_TIM_FR_RN_TT);
	fr_rn_tt_ns = fr_rn_tt.cn68xx.fr_rn_tt * 1000000000ULL
				/ clock_hz;

	/*
	 * If TIM is not enabled yet, enable it now.
	 */
	control.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);
	if (control.cn68xx.enable_timers == 0) {
		control.cn68xx.ena_gpio = 0; /* disable GPIO edge counting */
		control.cn68xx.reset = 1; /* Reset pulse for FR timer */
		control.cn68xx.enable_timers = 1;
		cvmx_write_csr(CVMX_TIM_REG_FLAGS, control.u64);
	}

	/* Set timer ring aura(pool0..8) (CTL1) */
	if (debug)
		cvmx_dprintf("INFO: %s: Timer%u: FPA_Pool(pool) = %#x\n",
			__func__, ring_id, timer_id->fpa_pool);

	/* CTL1 */
	ring_ctl1.u64 = 0;
	ring_ctl1.cn68xx.ena_prd = 0;	/* disable periodic mode */
	ring_ctl1.cn68xx.cpool = timer_id->fpa_pool & 0x7; /* pool(0..7) */
	ring_ctl1.cn68xx.ena_dfb = 0;	/* Free chunks */
	ring_ctl1.cn68xx.ena_dwb = 1;	/* DWB */
	ring_ctl1.cn68xx.bucket = 0;	/* 1st bucket */
	ring_ctl1.cn68xx.bsize = (1u << timer_id->num_buckets_exp) - 1;
	ASSERT(ring_ctl1.cn68xx.bsize == (1u << timer_id->num_buckets_exp) - 1);
	cvmx_write_csr(CVMX_TIM_RINGX_CTL1(ring_id), ring_ctl1.u64);

	/* program bucket information (CTL2) */
	ring_ctl2.u64 = 0;

	/* CSIZE mod(16) should be zero */
	ASSERT((timer_id->chunk_size & 0xf) == 0);

	/* Number of eight-byte words per chunk. */
	ring_ctl2.cn68xx.csize = timer_id->chunk_size;
	ASSERT(ring_ctl2.cn68xx.csize == timer_id->chunk_size);

	ASSERT((bucket_paddr & ((1 << 5)-1)) == 0);
	/* Pointer[35:5] to bucket[0] */
	ring_ctl2.cn68xx.base = bucket_paddr >> 5;

	if (debug) cvmx_dprintf("INFO: %s: "
		"Timer%u: CTL2 csize = %u, base = %#x\n",
		__func__, ring_id,
		(unsigned) ring_ctl2.cn68xx.csize,
		(unsigned) ring_ctl2.cn68xx.base);

	cvmx_write_csr(CVMX_TIM_RINGX_CTL2(ring_id), ring_ctl2.u64);

	/* Setup timings (CTL0) */
	interval = timer_id->tick_ns / fr_rn_tt_ns - 1;
	expire_offset = interval + (ring_id * interval / 
		cvmx_timer_rings_per_node());

	if (debug) cvmx_dprintf("INFO: %s: Timer%u: "
		"CTL0 expire_offset = %llu, interval = %d\n",
		__func__, ring_id, (long long)expire_offset, interval);

	ring_ctl0.u64 = 0;
	ring_ctl0.cn68xx.intc = 1; /* error interrupts not supported yet */
	ring_ctl0.cn68xx.ena = 0;
	/* Offset for multiple rings to avoid congestion */
	ring_ctl0.cn68xx.timercount =  expire_offset;
	/* Timer interval, in FR_RN_TT ticks */
	ring_ctl0.cn68xx.interval = interval;
	cvmx_write_csr(CVMX_TIM_RINGX_CTL0(ring_id), ring_ctl0.u64);

	if (debug) cvmx_dprintf("INFO: %s: Timer%u: "
		"CTL1 bucket = %d, intc = %d, bsize = %d, ena = %u\n",
		__func__, ring_id,
		ring_ctl1.cn68xx.bucket, ring_ctl0.cn68xx.intc,
		ring_ctl1.cn68xx.bsize, ring_ctl0.cn68xx.ena);

	/* interval calculation changed actual tick_ns, recompute */
	interval = ring_ctl0.cn68xx.interval + 1;
	timer_id->tick_ns = interval * fr_rn_tt_ns;
	timer_id->tick_cycles = clock_hz * timer_id->tick_ns
				/ 1000000000ULL ;

	/* Start this timer (ring) */
	ring_ctl0.u64 = cvmx_read_csr(CVMX_TIM_RINGX_CTL0(ring_id));
	ring_ctl0.cn68xx.ena = 1;
	cvmx_write_csr(CVMX_TIM_RINGX_CTL0(ring_id), ring_ctl0.u64);

	return 0;
}

/*
 * legacy TIM architecture - available on the following octeon models
 *  cn30xx; cn31xx; cn38xx; cn38xxp2; cn50xx; cn52xx; cn52xxp1;
 *  cn56xx; cn56xxp1; cn58xx; cn58xxp1; cn61xx; cn63xx; cn63xxp1;
 *  cn66xx; cn70xx; cn70xxp1; cnf71xx;
 */

static int
cvmx_timer_init_legacy(cvmx_timer_id_t timer_id)
{	
	cvmx_tim_control_t control; 
	cvmx_tim_reg_read_idx_t tim_reg_idx;
	cvmx_tim_mem_ring0_t tim_ring0;
	cvmx_tim_mem_ring1_t tim_ring1;
	cvmx_tim_mem_debug0_t tim_debug0;
	cvmx_tim_mem_debug1_t tim_debug1;
	long long clock_hz;
	unsigned interval, ring_id;
	uint64_t bucket_paddr;
	
	ring_id = timer_id->ring;

	/* Physical address of the first bucket in the ring */
	bucket_paddr = cvmx_ptr_to_phys(&timer_id[1]);
	ASSERT((bucket_paddr >> 36) == 0);	/* expect [35:0] addr (CTL2) */

_retry:
	/* Set index register to access ONLY 'ring_id' ring */
	tim_reg_idx.u64 = 0;
	tim_reg_idx.s.index = ring_id;	/* ring 0..15 */
	tim_reg_idx.s.inc = 0;	/* do not incr. - access ONLY this ring */
	cvmx_write_csr(CVMX_TIM_REG_READ_IDX, tim_reg_idx.u64);
	/* Possible race here, unlikely - do poor-man's spinlock just in case */
	/* Make sure the ring is disabled */
	tim_debug0.u64 = cvmx_read_csr(CVMX_TIM_MEM_DEBUG0);
	tim_reg_idx.u64 = cvmx_read_csr(CVMX_TIM_REG_READ_IDX);
	if (tim_reg_idx.s.index != ring_id)
		goto _retry;

	if (tim_debug0.cn70xx.ena != 0 ) {
		cvmx_dprintf("ERROR: %s: Timer%u: already running\n",
			__func__, ring_id);
		return -1;
	}

	/* Set timer ring aura(pool0..8) (CTL1) */
	if (debug)
		cvmx_dprintf("INFO: %s: Timer%u: FPA_Pool(pool) = %#x\n",
			__func__, ring_id, timer_id->fpa_pool);

	/* RING1 */
	tim_ring1.u64 = 0;
	tim_ring1.cn70xx.pool = timer_id->fpa_pool & 0x7; /* pool(0..7) */
	/* CSIZE mod(16) should be zero */
	ASSERT((timer_id->chunk_size & 0xf) == 0);
	/* Number of eight-byte words per chunk. */
	tim_ring1.cn70xx.words_per_chunk = timer_id->chunk_size; /* CSIZE */
	ASSERT(tim_ring1.cn70xx.words_per_chunk == timer_id->chunk_size);
	interval = (timer_id->tick_cycles + 512)/1024;
	tim_ring1.cn70xx.interval = interval - 1; 	/* INTERVAL */
	tim_ring1.cn70xx.ring = ring_id;		/* ring_id */
	cvmx_write_csr(CVMX_TIM_MEM_RING1, tim_ring1.u64);

	/* The 1024 division have changed the actual tick_ns, recompute */
 	clock_hz =  cvmx_clock_get_rate(CVMX_CLOCK_TIM);
	timer_id->tick_cycles = interval * 1024;
	timer_id->tick_ns = (1000000000ULL * timer_id->tick_cycles) / clock_hz;
	
	/* RING0 */
	tim_ring0.u64 = 0;

	ASSERT((bucket_paddr & ((1 << 5)-1)) == 0);
	/* Pointer[35:5] to bucket[0] */
	tim_ring0.cn70xx.first_bucket = bucket_paddr >> 5;
	tim_ring0.cn70xx.num_buckets = (1u << timer_id->num_buckets_exp) - 1;
	ASSERT(tim_ring0.cn70xx.num_buckets ==
		(1u << timer_id->num_buckets_exp) - 1);
	tim_ring0.cn70xx.ring = ring_id;
	
	if (debug) cvmx_dprintf("INFO: %s: "
		"Timer%u: RING1 csize = %u, bsize=%u, base = %#x\n",
		__func__, ring_id,
		(unsigned) tim_ring1.cn70xx.words_per_chunk,
		(unsigned) tim_ring0.cn70xx.num_buckets,
		(unsigned) tim_ring0.cn70xx.first_bucket);

	cvmx_write_csr(CVMX_TIM_MEM_RING0, tim_ring0.u64);

	if (debug) cvmx_dprintf("INFO: %s: Timer%u: interval = %d\n",
				__func__, ring_id, interval);

	
	if (debug) {
		tim_debug0.u64 = cvmx_read_csr(CVMX_TIM_MEM_DEBUG0);
		tim_debug1.u64 = cvmx_read_csr(CVMX_TIM_MEM_DEBUG1);
		cvmx_dprintf("INFO: %s: Timer%u: bsize = %d, ena = %u\n",
			__func__, ring_id,
			tim_debug1.cn70xx.bsize,
			tim_debug0.cn70xx.ena);
	}

	/*
	 * If global TIM is not enabled yet 
	 * start the global timer (for the first time or if it's stopped)
	 */
	control.u64 = cvmx_read_csr(CVMX_TIM_REG_FLAGS);
	if (control.cn70xx.enable_timers == 0) {
		control.cn70xx.enable_dwb = 1; /* do not write back */
		control.cn70xx.enable_timers = 1;
		cvmx_write_csr(CVMX_TIM_REG_FLAGS, control.u64);
	}

	/* Start this timer (ring) */
	/* note that tim_ring1.u64 keep all previous settings */
	tim_ring1.cn70xx.enable = 1;
	cvmx_write_csr(CVMX_TIM_MEM_RING1, tim_ring1.u64);

	return 0;
}

static int
cvmx_timer_stop_cn78xx(cvmx_timer_id_t timer_id)
{
	cvmx_tim_ringx_ctl1_t ring_ctl1;
	unsigned interval;
	long long clock_hz;
	unsigned node, ring_id;

	ring_id = timer_id->ring;
	node = timer_id->node;

	/* Clear TIM_RING_CTL1 to disable the ring */
	ring_ctl1.u64 = 0;
	cvmx_write_csr_node(node, CVMX_TIM_RINGX_CTL1(ring_id), ring_ctl1.u64);

	/* Calculate how many clock cycles per tick */
	clock_hz =  cvmx_clock_get_rate(CVMX_CLOCK_TIM);

	/* Wait 1 ring interval period, in core_clocks */
	interval =
		cvmx_clock_get_count(CVMX_CLOCK_CORE) * timer_id->tick_cycles
			/ clock_hz ;

	/* Waiting for one ring interval should suffice */
	cvmx_wait(interval);

	/* wait for rcf_busy to go low before proceeding */
	do {
	    ring_ctl1.u64 = cvmx_read_csr_node(node,
			CVMX_TIM_RINGX_CTL1(ring_id));
	} while (ring_ctl1.cn78xx.rcf_busy && interval--);

	return 0;
}

static int
cvmx_timer_stop_cn68xx(cvmx_timer_id_t timer_id)
{
	cvmx_tim_ringx_ctl0_t ring_ctl0;
	cvmx_tim_ringx_dbg1_t ring_dbg1;
	unsigned ring_id;

	ring_id = timer_id->ring;

	/* wait for ring_esr == 0 before proceeding */
	do {
		ring_dbg1.u64 = cvmx_read_csr(CVMX_TIM_RINGX_DBG1(ring_id));
	} while (ring_dbg1.cn68xx.ring_esr != 0);

	/* Stop this timer (ring) */
	ring_ctl0.u64 = cvmx_read_csr(CVMX_TIM_RINGX_CTL0(ring_id));
	ring_ctl0.cn68xx.ena = 0;
	cvmx_write_csr(CVMX_TIM_RINGX_CTL0(ring_id), ring_ctl0.u64);
	
	return 0;
}

static int
cvmx_timer_stop_legacy(cvmx_timer_id_t timer_id)
{
	cvmx_tim_mem_ring1_t tim_ring1;
	unsigned ring_id;
	unsigned long long interval, clock_hz;

	ring_id = timer_id->ring;

	tim_ring1.u64 = 0;
	tim_ring1.cn70xx.ring = ring_id;
	tim_ring1.cn70xx.pool = timer_id->fpa_pool & 0x7;
	tim_ring1.cn70xx.words_per_chunk = timer_id->chunk_size;
	tim_ring1.cn70xx.interval = 0;	/* die fast */
	tim_ring1.cn70xx.enable = 0;	/* Stop the ring */
	cvmx_write_csr(CVMX_TIM_MEM_RING1, tim_ring1.u64);

	/* Calculate how many clock cycles per tick */
	clock_hz =  cvmx_clock_get_rate(CVMX_CLOCK_TIM);

	/* Wait 1 fastest ring interval period, in core_clocks */
	interval = cvmx_clock_get_count(CVMX_CLOCK_CORE) * 1024ULL / clock_hz ;

	cvmx_wait(interval);

	return 0;
}

static unsigned
cvmx_timer_curr_bucket(cvmx_timer_id_t timer_id)
{
	cvmx_tim_ringx_ctl1_t ring_ctl1;
	unsigned node, ring_id;

	ring_id = timer_id->ring;
	node = timer_id->node;

	if (cvmx_timer_version() == 3) {

		ring_ctl1.u64 =
			cvmx_read_csr_node(node, CVMX_TIM_RINGX_CTL1(ring_id));

		return ring_ctl1.cn78xx.bucket;
	}
	else if (cvmx_timer_version() == 2) {

		ring_ctl1.u64 =
			cvmx_read_csr(CVMX_TIM_RINGX_CTL1(ring_id));

		return ring_ctl1.cn68xx.bucket;
	}
	else
	{	
		/* legacy TIM architecture - all other octeon models
		 * NOTE: We do not use TIME_MEM_DEBUDx regs in order to avoid
		 * locking (semaphore) when write to CVMX_TIM_REG_READ_IDX reg
		 * This code was confirmed to return the same curr_bucket (+/-1)
		 * as TIME_MEM_DEBUGx registers
		 */

		uint64_t curr_tick;
		curr_tick = cvmx_timer_current_tick(timer_id)
				- timer_id->start_cycle;
		curr_tick &= (1u << timer_id->num_buckets_exp) - 1;
		return (unsigned) curr_tick;
	}
}

/*
 * Hardware-specific timer initialization
 * @INTERNAL
 *
 * Dispatch the per-model initialization function here.
 */
static int
cvmx_timer_hw_init(cvmx_timer_id_t timer_id)
{
	if (cvmx_timer_version() == 3)
		return cvmx_timer_init_cn78xx(timer_id);
	else if (cvmx_timer_version() == 2)
		return cvmx_timer_init_cn68xx(timer_id);
	else
		return cvmx_timer_init_legacy(timer_id);
}

/*
 * @INTERNAL
 *
 * Dispatch the chip-specific stop function from here
 */
static int
cvmx_timer_hw_stop(cvmx_timer_id_t timer_id)
{
	int res;

	if (cvmx_timer_version() == 3)
		res = cvmx_timer_stop_cn78xx(timer_id);
	else if (cvmx_timer_version() == 2)
		res = cvmx_timer_stop_cn68xx(timer_id);
	else
		res = cvmx_timer_stop_legacy(timer_id);
	return res;
}

/*
 * Handle global resource allocation for new and legacy timers
 *
 */
static int
cvmx_timer_resource_alloc(unsigned node)
{
	unsigned num_cores;
	int tim_num;

	/* Initialize global resource, returns 0 if existing or new */
        if(cvmx_create_global_resource_range(CVMX_GR_TAG_TIM(node),
		cvmx_timer_rings_per_node())) {
                cvmx_dprintf("ERROR: %s: Failed to create global resource\n",
			__func__);
                return -1;
        }

	/* Reserve rings for legacy timer, one per core */
	num_cores = cvmx_octeon_num_cores();

	if (debug)
		cvmx_dprintf("%s: num_cores=%d\n", __func__, num_cores);

	/* Reserve the rings intended for legacy timer API */
	(void) cvmx_reserve_global_resource_range(CVMX_GR_TAG_TIM(node),
		0,		/* owner: not used */
		0,		/* base: Ring0 for Core0 */
		num_cores);	/* num_elements */

	/* Allocate one ring and return its numer */
	tim_num = cvmx_allocate_global_resource_range(CVMX_GR_TAG_TIM(node),
		0,		/* owner: not used */
		1,		/* num_elements */
		1);		/* alignment */

	/* Free the legacy rings */
	(void) cvmx_free_global_resource_range_with_base( CVMX_GR_TAG_TIM(node),
		0, num_cores);

	if (tim_num < 0) {
		cvmx_dprintf("ERROR: %s: Out of free timers\n", __func__);
	}

	if (debug)
		cvmx_dprintf("%s: allocated ring %d\n", __func__, tim_num);

	return tim_num;
}

static int
cvmx_timer_resource_free(unsigned node, unsigned ring_id)
{
	int res;

	res = cvmx_free_global_resource_range_with_base(CVMX_GR_TAG_TIM(node),
		ring_id, 1);
	if (debug)
		cvmx_dprintf("%s: free ring %u result %d\n",
			__func__, ring_id, res);
	return res;
}

