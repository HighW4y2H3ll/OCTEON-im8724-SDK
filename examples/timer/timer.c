/***********************license start***************
 * Copyright (c) 2014  Cavium Inc. (support@cavium.com). All rights 
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


/*
 * File version info: $Id: timer.c 115303 2015-03-27 01:47:27Z lrosenboim $
 */
 
#include <stdio.h>
#include <cvmx.h>
#include <cvmx-helper.h>
#include <cvmx-fpa.h>
#include <cvmx-ipd.h>
#include <cvmx-pow.h>
#include <cvmx-atomic.h>
#include <cvmx-tim.h>
#include <cvmx-tim-atomic.h>

#define	NUM_TIMEOUTS	190
#define	RUN_TIME_NS	(40*1000*1000ULL)
#define	MIN_TIMEOUT_NS	(100*1000)	/* 100us */
#define	MAX_TIMEOUT_NS	(RUN_TIME_NS/3)
#define	MAX_CORES	48
#define	TIMER_GROUP	3
#define	FIRST_TIMEOUT_NS (15*1000*1000ULL)

CVMX_SHARED int wqe_pool;
CVMX_SHARED struct {
	int64_t warn_ave_del, sum_del;
	int32_t count_events, count_cancel, count_rearm, count_retire;
	int32_t error_count, warn_count, cancel_fail, spurs;
} shared_stats;

/* Functions to measure time using core clock in nanoseconds */
unsigned long long core_time_now_ns(void)
{
	unsigned long long t;
	t = cvmx_clock_get_count(CVMX_CLOCK_CORE);
	t = 1000000000ULL * t / cvmx_clock_get_rate(CVMX_CLOCK_CORE);
	return t;
}

enum event_state {UNKNOWN = 0, ARMED, RETIRED, CANCELLED};

struct timeout_s {
	cvmx_timer_handle_t handle;
	cvmx_wqe_t *wqe;
	unsigned long long expires;
	unsigned use_count, warn_count, cancel_count;
	enum event_state state;
} ;

CVMX_SHARED struct timeout_s timeout_table[MAX_CORES][ NUM_TIMEOUTS ];

int main(void)
{
	cvmx_coremask_t coremask;
	unsigned long long time_start;
	const char timer_name[] = "SharedTIM";
	cvmx_timer_id_t timer1;
	cvmx_timer_status_t trs;
	unsigned long long now;
	unsigned long long max_tmo_ns;
	long long del, limit;
	unsigned long long warn_ave_del;
	unsigned i, error_count = 0, warn_count = 0, cancel_fail_cnt = 0;
	unsigned count_cancel, count_rearm, count_events, count_retire, spurs;
	signed long long  sum_del;
	cvmx_wqe_t *wqe;
	cvmx_wqe_t *wqp;
	cvmx_wqe_t *wqp_null = cvmx_phys_to_ptr(0x80);
	unsigned n, core = cvmx_get_core_num();
	unsigned long long abs_ticks;
	const bool debug = 0;

	cvmx_user_app_init();

	if (debug) {
		printf("Core clock %lld Hz\n",
			(long long) cvmx_clock_get_rate(CVMX_CLOCK_CORE));
		del =  cvmx_clock_get_count(CVMX_CLOCK_CORE);
		cvmx_wait_usec(100);
		printf("core tick %lld equivalent to 100 usec\n",
			(long long) cvmx_clock_get_count(CVMX_CLOCK_CORE)-del);
	}

	cvmx_coremask_copy(&coremask, &(cvmx_sysinfo_get())->core_mask);
	n = cvmx_coremask_get_core_count(&coremask);
	if (n > MAX_CORES) {
		printf("ERROR: Too many cores, aborting.\n");
		return -4;
	}

	if (OCTEON_IS_INIT_CORE()) {
		unsigned timer_pool;

		cvmx_fpa_enable();

		timer_pool = cvmx_fpa_get_timer_pool();
		wqe_pool = cvmx_fpa_get_wqe_pool();
		if( wqe_pool == -1) wqe_pool = 1;

		if( __cvmx_helper_initialize_fpa_pool(timer_pool, 256,
			n * NUM_TIMEOUTS, "Timer Bufs") < 0) {
			printf("Failed FPA initialization\n");
			return -1;
		}

		if( __cvmx_helper_initialize_fpa_pool(wqe_pool, sizeof(*wqe),
			n * NUM_TIMEOUTS, "WQE Bufs") < 0) {
			printf("Failed FPA initialization\n");
			return -1;
		}

		if (cvmx_helper_initialize_sso( 100 + n * NUM_TIMEOUTS )) {
			printf("Failed SSO initialization\n");
			return -1;
		}

		printf("Request creating timer '%s' %u..%u usec\n",
			timer_name,
			(unsigned)MIN_TIMEOUT_NS/1000,
			(unsigned)MAX_TIMEOUT_NS/1000 );

		timer1 = cvmx_timer_create(timer_name, cvmx_get_node_num(), timer_pool,
			MIN_TIMEOUT_NS, MAX_TIMEOUT_NS);
		if (timer1 == NULL) {
			cvmx_dprintf("ERROR: failed to create timer '%s'\n",
				     timer_name);
			return -1;
		}

		printf("Timer '%s' created: max=%u usec (with pool %u)\n",
			timer_name,
			(unsigned)(cvmx_timer_tick_to_ns(timer1, 
			cvmx_timer_maximum_tmo(timer1)) / 1000),
			timer_pool);

		if (debug)
			cvmx_helper_fpa_dump(0);

		cvmx_coremask_barrier_sync(&coremask);
		cvmx_wait(20000);
	}
	else
	{
		char _timer_name[100];
		
		sprintf(_timer_name, "%s.%d", timer_name, cvmx_get_node_num());
		cvmx_coremask_barrier_sync(&coremask);
		timer1 = cvmx_timer_id_from_name(_timer_name);
		if (timer1 == NULL) {
			cvmx_dprintf("ERROR: failed bind timer '%s'\n", _timer_name);
			return -1;
			}
		cvmx_wait(20000);
	}

	if (debug)
		printf("%s: timer_id=%p NUM_TIMEOUTS=%d\n", 
			__func__, (void *) timer1, NUM_TIMEOUTS);

	max_tmo_ns = cvmx_timer_tick_to_ns(timer1, cvmx_timer_maximum_tmo(timer1));
	core = cvmx_get_local_core_num();
	if (core >= MAX_CORES) {
		cvmx_dprintf("ERROR: core# %u out of range\n", core);
		return -1;
	}

	/* Initialize all timeout objects */
	for(i = 0; i < NUM_ELEMENTS(timeout_table[core]); i++)
	{
		/* Allocate with patiance */
		do {
			wqe = cvmx_fpa_alloc(wqe_pool);
			if(wqe == NULL) printf("ERROR: wqe: %d\n", wqe_pool);
		} while(wqe == NULL);

		memset(wqe, 0, sizeof(*wqe));
		cvmx_wqe_set_port(wqe, ~0);
		cvmx_wqe_set_grp(wqe, TIMER_GROUP);
		cvmx_wqe_set_aura(wqe, wqe_pool);
		cvmx_wqe_set_soft(wqe, true);
		cvmx_wqe_set_tt(wqe, CVMX_POW_TAG_TYPE_ATOMIC);
		cvmx_wqe_set_tag(wqe, (0x5a << 24) | (core << 16) | (0xffff & i));
		timeout_table[core][i].wqe = wqe;
		wqe->word2.u64 = 0;
	}

	/* first step - arm all timers to expire at the same time */
	/* schedule all events with absolute "first timeout" from now */
	if (debug)
		printf("core #%d cvmx_timer_arm_absolute() %d wqe_entries\n",
			core, NUM_TIMEOUTS);
	now = core_time_now_ns();

	abs_ticks = cvmx_timer_current_tick(timer1) + 
			cvmx_timer_ns_to_tick(timer1, FIRST_TIMEOUT_NS);

	for(i = 0; i < NUM_ELEMENTS(timeout_table[core]); i++)
	{
		wqe->word2.u64 = timeout_table[core][i].use_count;

		trs = cvmx_timer_arm_absolute(timer1, abs_ticks,
			timeout_table[core][i].wqe,
			&timeout_table[core][i].handle);
		if (cvmx_unlikely(trs != CVMX_TIMER_STATUS_SUCCESS)) {
			printf("ERROR: failed to arm timer\n");
			error_count ++;
			continue;
		}

		timeout_table[core][i].expires = now + FIRST_TIMEOUT_NS;
		timeout_table[core][i].state = ARMED;

		if (debug)
			printf("event %u,%u armed "
				"[us]: now=%llu, .expires=%llu, "
				"curr_tick=%llu abs_tick=%llu\n",
				core, i,
				(unsigned long long)core_time_now_ns()/1000,
				(unsigned long long)timeout_table[core][i].expires/1000,
				(unsigned long long)cvmx_timer_current_tick(timer1),
				abs_ticks
			);
	}
	
	i = 0;
	count_events = 0;
	count_cancel = 0;
	count_rearm = 0;
	count_retire = 0;
	sum_del = 0;
	warn_ave_del = 0;
	cancel_fail_cnt = 0;
	spurs = 0;
	
	if (debug)
		printf(">> core #%d: Start main_loop()...\n", core);

	/* Wait for all cores before start the main loop */
	cvmx_coremask_barrier_sync(&coremask);

	time_start = now = core_time_now_ns();

	/* Enter semi-infinite loop */
	while((now - time_start) <= (RUN_TIME_NS + MAX_TIMEOUT_NS)){
		unsigned long long ns;

		wqe = cvmx_pow_work_request_sync(1/*wait*/);
		now = core_time_now_ns();

		if (wqe == NULL)
			continue;

		/* extract owner core and table index from tag */
		
		i = cvmx_wqe_get_tag(wqe);

		if ((i >> 24) != 0x5a) {
			printf("ERROR: Invalid event tag wqe=%p\n",wqe);
			error_count ++;
			continue;
		}

		core = 0xff & (i >> 16);
		i &= 0xffff;

		if (core >= NUM_ELEMENTS(timeout_table) ||
		    i >= NUM_ELEMENTS(timeout_table[core])) {
			printf("ERROR: Invalid event index wqe=%p\n",wqe);
			error_count ++;
			continue;
		}

		del = now - timeout_table[core][i].expires;
		limit = MIN_TIMEOUT_NS * 3;

		/*
		 * Spusious work entries have been ibserved on the simulator,
		 * and need to be ignored.
		 */
		if (timeout_table[core][i].state != ARMED ||
			wqe->word2.u64 != timeout_table[core][i].use_count) {
simprintf("SPURIOUS: @%llu event %llu:%llu wqe %#llx exp %llu\n",
now, core, i, wqe, timeout_table[core][i].expires);
			spurs ++;
			continue;
		}

		if(cvmx_unlikely(timeout_table[core][i].wqe == NULL)) {
			printf("ERROR: Cancelled wqe=%p "
				"event %u,%u delivered\n",
				wqe, core, i);
			error_count ++;
			continue;
		}
		else if(cvmx_unlikely(timeout_table[core][i].wqe != wqe)) {
			printf("ERROR: WQE mismatch %p != %p\n", wqe,
				timeout_table[core][i].wqe);
			error_count ++;
			continue;
		}

		if (debug)
			printf("DEBUG: wqe=%p event=%u,%u exp=%llu now=%llu\n",
				wqe, core, i,
				(unsigned long long)timeout_table[core][i].handle.word[0],
				(unsigned long long)cvmx_timer_current_tick(timer1));

		timeout_table[core][i].state = UNKNOWN;
		count_events ++;

		sum_del += del; /* 'del' is signed */
		limit = MIN_TIMEOUT_NS * 3 + MIN_TIMEOUT_NS * n / 16;
		if (debug) limit *= 2;

		if (cvmx_unlikely(abs(del) > limit)) {
			timeout_table[core][i].warn_count++;
			warn_count++;
			warn_ave_del += abs(del) / MIN_TIMEOUT_NS;
		}

		/* re-arm, unless it will run beyond run-time */
		ns = limit + rand() % (max_tmo_ns - limit);
		if ((now + ns) < (time_start + RUN_TIME_NS)) {
			del = cvmx_timer_ns_to_tick(timer1, ns);
			trs = cvmx_timer_arm_relative(
				timer1, del,
				timeout_table[core][i].wqe,
				&timeout_table[core][i].handle);
			if (cvmx_unlikely(trs != CVMX_TIMER_STATUS_SUCCESS)) {
				printf("ERROR: failed to arm timer\n");
				error_count ++;
				continue;
			}
			/* update expected expiration time */
			timeout_table[core][i].expires = now + ns;
			count_rearm++;
// simprintf("REARM: @%llu event %llu:%llu wqe %#llx exp %llu\n",
// now, core, i, wqe, now+ns);
			timeout_table[core][i].state = ARMED;
			wqe->word2.u64 = ++timeout_table[core][i].use_count;
		}
		else
		{
			/* retire timeout */
			timeout_table[core][i].expires = 0;
			timeout_table[core][i].wqe = NULL;

			cvmx_fpa_free(wqe, wqe_pool, 0);
			if (debug) 
				cvmx_dprintf("DEBUG: wqe=%p retired\n",wqe);
			count_retire++;
// simprintf("RETIRE: @%llu event %llu:%llu wqe %llx\n",
// now, core, i, wqe);
			timeout_table[core][i].state = RETIRED;
			wqe->word2.u64 = ++timeout_table[core][i].use_count;
		}

		/* Push WQE and event table changes */
		CVMX_SYNC;

		/* Keep processing fast for the first batch
		 * of "all-at-once" events
		 */
		if (timeout_table[core][i].use_count <= 1)
			continue;

		/* Exercise cancellations */
		core = cvmx_get_local_core_num();
		i = rand() % NUM_TIMEOUTS;

		if (core >= NUM_ELEMENTS(timeout_table) ||
		    i >= NUM_ELEMENTS(timeout_table[core])) {
			printf("ERROR: Invalid event index wqe=%p\n",wqe);
			continue;
		}

		/* Chose a victim */
		wqp = timeout_table[core][i].wqe;
		if (wqp == NULL || wqe == wqp)
			continue;

		/* Lockout the event from other cores */
		cvmx_pow_tag_sw_full(wqp_null, cvmx_wqe_get_tag(wqp),
			CVMX_POW_TAG_TYPE_ATOMIC, TIMER_GROUP);

		/* Do not touch event entry until the tag switch is done */
		cvmx_pow_tag_sw_wait();

		now = core_time_now_ns();
		del = timeout_table[core][i].expires - now;

		/* Don't try to cancel events that are ebout to expire */
		limit = 4 * MIN_TIMEOUT_NS;
		if (del <= limit)
			continue;

		/* Count cancel attempts for each event */
		timeout_table[core][i].cancel_count++;

		wqp = cvmx_timer_cancel(timer1, &timeout_table[core][i].handle);
		if(cvmx_unlikely(wqp != timeout_table[core][i].wqe))
		{
			if (debug)
				cvmx_dprintf("ERROR: "
					" event %u,%u cancel failed,"
					" del=%lld ns (%lld ticks)"
					" lag %lld ns\n",
					core, i, del, del/MIN_TIMEOUT_NS,
					core_time_now_ns() - now);
			cancel_fail_cnt ++;
// simprintf("CANFAIL: @%llu event %llu:%llu wqe %#llx\n",
// now, core, i, wqp);
		}
		else
		{
			timeout_table[core][i].expires = 0;
			timeout_table[core][i].wqe = NULL;

			cvmx_fpa_free(wqp, wqe_pool, 0);
			if (debug)
				cvmx_dprintf("DEBUG: wqe=%p cancelled\n",wqp);
			count_cancel ++;
// simprintf("CANCEL: @%llu event %llu:%llu wqe %#llx\n",
// now, core, i, wqp);
			timeout_table[core][i].state = CANCELLED;
			wqp->word2.u64 = ++timeout_table[core][i].use_count;
			CVMX_SYNCW;
		}
		
	} /* main loop */

	if (debug)
		printf(">> core #%d: ... main_loop() finished\n", core);

	cvmx_wait_usec(5*MIN_TIMEOUT_NS/1000);

	/* Wait for all cores to finish main loop */
	cvmx_coremask_barrier_sync(&coremask);

	/* End of main loop, check clean termination */
	core = cvmx_get_local_core_num();

	if (debug) cvmx_dprintf("DEBUG: cleaning up\n");

	/* Check that all events have been consumed */
	for(i = 0; i < NUM_ELEMENTS(timeout_table[core]); i++) {
		if (timeout_table[core][i].expires != 0 ||
			(timeout_table[core][i].wqe != NULL)) {
			error_count ++;
			now = core_time_now_ns();
			del = timeout_table[core][i].expires - now;
			printf("ERROR: event %u,%u "
				"has not been consumed, "
				"used %u warned %u cancelled %u, ",
				core, i, 
				timeout_table[core][i].use_count,
				timeout_table[core][i].warn_count,
				timeout_table[core][i].cancel_count);

			if (del > 0)
				printf( "expected to expire in %lld ticks\n", 
					del/MIN_TIMEOUT_NS);
			else
				printf( "should have expired %lld ticks ago\n", 
					-del/MIN_TIMEOUT_NS);

			wqe = cvmx_timer_cancel(timer1, 
					&timeout_table[core][i].handle);
			if (wqe == NULL) {
				printf("ERROR: event %u,%u "
				"could not be cancelled\n",
				core, i);
				continue;
			}
			timeout_table[core][i].expires = 0;
			timeout_table[core][i].wqe = NULL;
			cvmx_fpa_free(wqe, wqe_pool, 0);
			if (debug) 
				cvmx_dprintf("DEBUG: wqe=%p cleaned up\n",wqe);
		}
	}

	if (OCTEON_IS_INIT_CORE()) {
		cvmx_timer_id_t timer2;
		const char timer2_name[] = "SharedTimer2";
		printf("Waiting some\n");
		cvmx_wait_usec(100);

		timer2 = cvmx_timer_create( timer2_name, cvmx_get_node_num(), 
						cvmx_fpa_get_timer_pool(),
						MIN_TIMEOUT_NS,
						MAX_TIMEOUT_NS);
		if (timer2 == NULL) {
			printf("ERROR: Creating 2nd timer\n");
			error_count ++;
		}

		if (cvmx_timer_destroy(timer2, timer2_name) < 0) {
			printf("ERROR: destroying 2nd timer_id=%p\n", timer2);
			error_count ++;
		}

		del = cvmx_timer_destroy(timer1, timer_name);
		if (del < 0) {
			printf("ERROR: Failed to destroy timer\n");
			error_count ++;
		}

		cvmx_helper_uninitialize_sso();
		del = cvmx_helper_shutdown_fpa_pools(0);
		if (del < 0) {
			printf("ERROR: Failed to stop FPA\n");
			error_count ++;
		}

		cvmx_atomic_add32(&shared_stats.count_events, count_events);
		cvmx_atomic_add32(&shared_stats.count_cancel, count_cancel);
		cvmx_atomic_add32(&shared_stats.count_rearm, count_rearm);
		cvmx_atomic_add32(&shared_stats.count_retire, count_retire);
		cvmx_atomic_add32(&shared_stats.error_count, error_count);
		cvmx_atomic_add64(&shared_stats.sum_del, sum_del);
		cvmx_atomic_add32(&shared_stats.warn_count, warn_count);
		cvmx_atomic_add64(&shared_stats.warn_ave_del, 
				  warn_count ? warn_ave_del/warn_count : 0);
		cvmx_atomic_add32(&shared_stats.cancel_fail, cancel_fail_cnt);
		cvmx_atomic_add32(&shared_stats.spurs, spurs);

		cvmx_coremask_barrier_sync(&coremask);

		printf("Statistics (%d cores X %d WQEs each)\n", n, NUM_TIMEOUTS);
		printf("average delta=%lld usec, events:%lu, cancels:%lu "
			"rearms:%lu retires:%lu\n",
			shared_stats.count_events == 0 ? 0 :
			(long long)shared_stats.sum_del/shared_stats.count_events/1000,
			(long) shared_stats.count_events,
			(long) shared_stats.count_cancel,
			(long) shared_stats.count_rearm,
			(long) shared_stats.count_retire);
		if (shared_stats.warn_count != 0)
			printf("WARNING: (warn_count=%u, warn_ave_del=%d ticks)\n",
				(unsigned int)shared_stats.warn_count,
				(unsigned int)(shared_stats.warn_ave_del/n));

		if (shared_stats.spurs != 0)
			printf("WARNING: Spurious events count %d\n",
				(int)shared_stats.spurs);

		if (shared_stats.cancel_fail != 0)
			printf("ERROR: cancel_fail=%u\n", (unsigned int)shared_stats.cancel_fail);
		if (shared_stats.error_count != 0)
			printf("ERROR: error_count=%u\n", (unsigned int)shared_stats.error_count);
		if (shared_stats.count_cancel == 0)
			printf("ERROR: NO CANCELS (count_cancel = 0)\n");
		if (shared_stats.count_rearm == 0)
			printf("ERROR: NO RE-ARMS (count_rearm = 0)\n");
		printf("End of example: %s\n",
			(shared_stats.error_count != 0
			|| shared_stats.cancel_fail != 0
			|| shared_stats.count_cancel == 0
			|| shared_stats.count_rearm == 0 )
			? "FAILED" : "PASSED");
		return -shared_stats.error_count;

	} else {
		cvmx_atomic_add32(&shared_stats.count_events, count_events);
		cvmx_atomic_add32(&shared_stats.count_cancel, count_cancel);
		cvmx_atomic_add32(&shared_stats.count_rearm, count_rearm);
		cvmx_atomic_add32(&shared_stats.count_retire, count_retire);
		cvmx_atomic_add32(&shared_stats.error_count, error_count);
		cvmx_atomic_add64(&shared_stats.sum_del, sum_del);
		cvmx_atomic_add32(&shared_stats.warn_count, warn_count);
		cvmx_atomic_add64(&shared_stats.warn_ave_del, 
				  warn_count ? warn_ave_del/warn_count : 0);
		cvmx_atomic_add32(&shared_stats.cancel_fail, cancel_fail_cnt);
		cvmx_atomic_add32(&shared_stats.spurs, spurs);

		cvmx_coremask_copy(&coremask, &(cvmx_sysinfo_get())->core_mask);
		cvmx_coremask_barrier_sync(&coremask);
		return 0;
	}
}
