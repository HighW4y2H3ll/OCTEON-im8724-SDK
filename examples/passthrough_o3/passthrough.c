/***********************license start***************
 * Copyright (c) 2014  Cavium Inc. (support@cavium.com). All rights
 * reserved.
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
 * File version info: $Id: passthrough.c 87283 2013-08-23 18:13:43Z lrosenboim $
 */
#include <stdio.h>
#include <string.h>

#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-spinlock.h"
#include "cvmx-fpa.h"
#include "cvmx-wqe.h"
#include "cvmx-ilk.h"
#include "cvmx-pip.h"
#include "cvmx-ipd.h"
#include "cvmx-pko.h"
#include "cvmx-dfa.h"
#include "cvmx-pow.h"
#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"
#include "cvmx-bootmem.h"
#include "cvmx-helper.h"
#include "cvmx-app-hotplug.h"
#include "cvmx-helper-cfg.h"
#include "cvmx-config-parse.h"
#include "cvmx-atomic.h"
#include "cvmx-pki.h"
#include "cvmx-helper-pki.h"
#include "cvmx-helper-bgx.h"

#include "passthrough.h"

/* Compile-time configuration: */
/* The following two conditionals are set in Makefile */
//#define PACKET_ORDERING  1
//#define ENABLE_PASSTHROUGH_HOTPLUG  1

//#define PKO3_FULL_API  1
//#define DUMP_PACKETS  1
//#define DUMP_STATS  1
//#define SWAP_MAC_ADDR  1

/* Arrange atomic counters in a structure to be able to control
 * their allocation and alignment with respect to each other.
 * In order to be able to get their physical address,
 * the counters must be allocated from cvmx_bootmem.
 * Place each counter on its own cache line to avoid threshing.
 */
struct atomic_counters_s {
	int64_t packets __attribute__((aligned(CVMX_CACHE_LINE_SIZE)));
	int64_t errors __attribute__((aligned(CVMX_CACHE_LINE_SIZE)));
	int64_t outstanding __attribute__((aligned(CVMX_CACHE_LINE_SIZE)));
};

static CVMX_SHARED struct atomic_counters_s *counters;

CVMX_SHARED unsigned long long loop_cycles[CVMX_MAX_CORES];

/* These variables are only valid on init_core and for simulator only */
static unsigned long long run_cycles = 0;
static unsigned int term_num;

static int volatile core_unplug_requested  = 0;
static int volatile app_shutdown_requested = 0;

#undef max
#define max(a, b) ((b > a) ? (b) : (a))

#if EXAMPLE_ROUTE_DMAC_PKTS
extern CVMX_SHARED int grpA, grpB;
#endif

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
static int app_shutdown = 1;

/* Passthrough hotplug callback arguments */
typedef struct passthrough_hp_cb_args {
	void *mem_ptr;
} passthrough_hp_cb_args_t;

void passthrough_shutdown_callback(passthrough_hp_cb_args_t* data)
{
	if (cvmx_is_init_core())
		printf("Hotplug callback called for core #%d data %p\n",
				cvmx_get_core_num(), data->mem_ptr);
	app_shutdown_requested = 1;
}

void passthrough_unplug_callback(passthrough_hp_cb_args_t* data)
{
	printf("Unlpug callback called for core #%d data %p\n",
			cvmx_get_core_num(), data->mem_ptr);
	core_unplug_requested = 1;
}

CVMX_SHARED passthrough_hp_cb_args_t passthrough_hp_cb_args_data;

int init_hotplug(void)
{
	cvmx_app_hotplug_callbacks_t cb;
	passthrough_hp_cb_args_data.mem_ptr = (void*)0xA5A5A5A5;

	if (is_core_being_hot_plugged())
		printf("core=%d is being hotplugged\n", cvmx_get_core_num());

	if (cvmx_is_init_core()) {
		bzero(&cb, sizeof(cb));
		cb.shutdown_callback = (void(*)(void*))passthrough_shutdown_callback;
		cb.unplug_core_callback = (void(*)(void*))passthrough_unplug_callback;
		/* Register application for hotplug. this only needs to be done once */
		cvmx_app_hotplug_register_cb(&cb, &passthrough_hp_cb_args_data, app_shutdown);
	}
	/* Activate hotplug */
	if (cvmx_app_hotplug_activate()) {
		printf("ERROR: cvmx_hotplug_activate() failed\n");
		return -1;
	}
	return 0;
}
#endif

#ifdef SWAP_MAC_ADDR
static inline
void swap_mac_addr(void * pkt_ptr)
{
	uint32_t w;
	uint16_t s;

	/* assuming an IP/IPV6 pkt i.e. L2 header is 2 byte aligned, 4 byte non-aligned */
	s = *(uint16_t *)pkt_ptr;
	w = *(uint32_t *)(pkt_ptr + 2);
	*(uint16_t *)pkt_ptr = *(uint16_t *)(pkt_ptr + 6);
	*(uint32_t *)(pkt_ptr + 2) = *(uint32_t *)(pkt_ptr + 8);
	*(uint16_t *)(pkt_ptr + 6) = s;
	*(uint32_t *)(pkt_ptr + 8) = w;
}
#endif

/**
 * Setup the Cavium Simple Executive Libraries using defaults
 *
 * @param num_packet_buffers  Number of outstanding packets to support
 * @return Zero on success
 */
static int application_init_simple_exec(int node, int num_packet_buffers)
{
	int result;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM) {
		cvmx_helper_setup_simulator_io_buffer_counts(node, num_packet_buffers,
				CVMX_PKO_MAX_OUTPUT_QUEUES * 4);
	}
	cvmx_fpa_global_init_node(node);
	if (cvmx_helper_initialize_sso_node(node, num_packet_buffers))
		return -1;

	cvmx_helper_cfg_opt_set(CVMX_HELPER_CFG_OPT_USE_DWB, 0);

	if (PKI_MODIFY_DEFAULT_CONFIG)
		cvmx_helper_pki_no_dflt_init(node);

	cvmx_helper_pki_set_dflt_bp_en(node, false);
	result = cvmx_helper_initialize_packet_io_node(node);

	if (PKI_MODIFY_DEFAULT_CONFIG) {
		/* Modify and scheduling related interface/port parameters here*/
		if (application_init_packet_input(node, num_packet_buffers)) {
			cvmx_dprintf("Passthrough initialization failure, node %d\n", node);
			return -1;
		}
		/* Modify any general port/style/tag related parameters here*/
		application_setup_pki_ports(node);
		cvmx_helper_ipd_and_packet_input_enable_node(node);
	}

	cvmx_helper_cfg_set_jabber_and_frame_max();

	return result;
}

/**
 * Clean up and properly shutdown the simple exec libraries.
 *
 * @return Zero on success. Non zero means some resources are
 *         unaccounted for. In this case error messages will have
 *         been displayed during shutdown.
 */
static int application_shutdown_simple_exec(int node)
{
	int result = 0;

	cvmx_helper_shutdown_packet_io_global_cn78xx(node);
	cvmx_helper_uninitialize_sso_node(node);
	result = cvmx_helper_shutdown_fpa_pools(node);
	return result;
}

/**
 * The main loop using the new API
 */
void app_o78_main_loop(void)
{
	cvmx_wqe_t *work;
	uint64_t port;
#if defined(PACKET_ORDERING)
	uint32_t tag, *ptag = &tag;
#else
	uint32_t *ptag = NULL;
#endif
	int ret;
	int queue = -1;
	bool simulator = false;
	void *outstand_pkts = NULL;
	cvmx_pow_wait_t wait = CVMX_POW_WAIT;
#if defined(PKO3_FULL_API)
	cvmx_pko3_pdesc_t desc;
#ifdef SWAP_MAC_ADDR
	uint8_t mac_hdr1[12];
	uint8_t mac_hdr2[12];
#endif
#else
	unsigned len; int aura;
	cvmx_buf_ptr_pki_t ptr;
#endif
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
	int holds_atomic_tag = 0;
#endif

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM) {
		simulator = true;
		outstand_pkts = &(counters->outstanding);
		wait = CVMX_POW_NO_WAIT;
	}
	while (1) {
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
		if ((core_unplug_requested || app_shutdown_requested) && (app_shutdown)) {
			printf("core=%d : is unplugged\n", cvmx_get_core_num());
			if (holds_atomic_tag)
				cvmx_pow_tag_sw_null_nocheck();
			break;
		}
#endif
		/* Get the next packet/work to process from the SSO unit. */
		work = cvmx_pow_work_request_sync(wait);
		if (cvmx_unlikely(work == NULL)) {
			if (simulator &&
				cvmx_atomic_get64(&(counters->packets)) >= term_num)
				break;
			continue;
		}
		/* Process software events */
		if (cvmx_unlikely(cvmx_wqe_is_soft(work))) {
#ifdef DUMP_PACKETS
			printf("Software generated event:\n");
			cvmx_helper_dump_packet(work);
#endif
			cvmx_helper_free_pki_pkt_data(work);
			cvmx_wqe_pki_free(work);
			continue;
		}
#if EXAMPLE_ROUTE_DMAC_PKTS
		if (cvmx_wqe_get_xgrp(work) == grpA)
			cvmx_dprintf("\nPacket frm port %d rcvd on grpA = %d",
					cvmx_wqe_get_port(work), grpA);
		else if (cvmx_wqe_get_xgrp(work) == grpB)
			cvmx_dprintf("\nPacket frm port %d rcvd on grpB = %d",
					cvmx_wqe_get_port(work), grpB);
#endif
		if (cvmx_unlikely(ret = cvmx_wqe_get_rcv_err(work))) {
			/* Inform about severe known problems only and
			 * drop the packet. */
			switch (ret) {
			case CVMX_PKI_OPCODE_RE_PARTIAL:
			case CVMX_PKI_OPCODE_RE_JABBER:
			case CVMX_PKI_OPCODE_RE_MEMOUT:
				break;
			default:
				printf("Recv error=%#x\n", ret);
				break;
			}
			cvmx_helper_free_pki_pkt_data(work);
			cvmx_wqe_pki_free(work);
			cvmx_atomic_add64_nosync(&(counters->errors), 1);
			cvmx_atomic_add64_nosync(&(counters->packets), 1);
			continue;
		}
		/*
		 * Insert packet processing here.
		 * Define DUMP_PACKETS to dump packets to the console.
		 * Note that due to multiple cores executing in parallel, the output
		 * will likely be interleaved.
		 *
		 */
		port = cvmx_wqe_get_port(work);
#ifdef DUMP_PACKETS
		printf("Processing packet\n");
		cvmx_helper_dump_packet(work);
#endif

#ifdef DUMP_STATS
		printf ("port to send out: %lu\n", port);
		cvmx_helper_show_stats(port);
#endif
#if 0
		/*
		 * 1. Convert from loopback to forwarder:
		 * BGX0:0(0x800) <=> BGX1:0(0x900)
		 * 2. Drop packet on BGX2:0(0xA00).
		 */
		switch (port) {
		/* BGX1.0 <=> BGX2.0 */
		case 0x800: port = 0x900; break;
		case 0x900: port = 0x800; break;
		/* BGX0.0 => Drop. */
		case 0xA00:
			cvmx_helper_free_pki_pkt_data(work);
			cvmx_wqe_pki_free(work);
			break;
		/* Loopback on the remaining ports. */
		default: break;
		}
#endif
		queue = cvmx_pko3_get_queue_base(port);

		/* Check if destination port is unknown */
		if (cvmx_unlikely(queue < 0)) {
			cvmx_atomic_add64_nosync(&(counters->errors), 1);
			/* Redirect to NULL interface to discard: */
			queue = CVMX_PKO3_IPD_PORT_NULL;
		}
		if (simulator) {
			/* Increment the total packet counts */
			cvmx_atomic_add64_nosync(&(counters->packets), 1);
			cvmx_atomic_add64_nosync(&(counters->outstanding), 1);
		}
		/*
		 * Construct a tag for egress ordering:
		 * Base the tag on original flow hash, plus the egress queue number
		 */
		if (ptag != NULL)
			*ptag = cvmx_pow_tag_compose(0xe9, cvmx_wqe_get_tag(work) + queue);

#if defined(PKO3_FULL_API)
		ret = cvmx_pko3_pdesc_from_wqe(&desc, (void *) work, true);
		cvmx_wqe_pki_free(work);

#ifdef SWAP_MAC_ADDR
		/* Reverse MAC addresses */
		cvmx_pko3_pdesc_hdr_pop(&desc, &mac_hdr1, 12);
		memcpy(mac_hdr2, mac_hdr1 + 6, 6);
		memcpy(mac_hdr2 + 6, mac_hdr1, 6);
		cvmx_pko3_pdesc_hdr_push(&desc, &mac_hdr2, 12, 2);
#endif
		if (simulator) {
			ret = cvmx_pko3_pdesc_notify_decrement(&desc, outstand_pkts);
			if (cvmx_unlikely(ret < 0))
				cvmx_dprintf("%s: error setting decrement\n",__func__);
		}
		ret = cvmx_pko3_pdesc_transmit(&desc, queue, ptag);
#else
		ptr = cvmx_wqe_get_pki_pkt_ptr(work);
		len = cvmx_wqe_get_len(work);
		aura = cvmx_wqe_get_aura(work);
		cvmx_wqe_pki_free(work);

		ret = cvmx_pko3_xmit_link_buf(queue, ptr, len, aura, outstand_pkts, ptag);
#endif
		if(cvmx_unlikely(ret != 0)) {
			printf("ERROR: pko3 transmit %d\n", ret);
			cvmx_atomic_add64_nosync(&(counters->errors), 1);
		}
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
		holds_atomic_tag = 1;
#endif
	}
}

/**
 * Determine, if a number is approximately equal to a match value.
 * Checks, if the supplied value is within 5% of the expected value.
 *
 * @param value    Value to check
 * @param expected Value needs to be within 5% of this value.
 * @return Non zero if the value is out of range.
 */
static int cycle_out_of_range(float value, float expected)
{
	/* The expected time check is disabled for Linux right now. Since the
	 * Linux kernel is configured for a 6 Mhz clock, there are a couple
	 * of context switch during this test. On the real chip the clock will
	 * be set the real value (600 Mhz) alleviating this problem.
	 */
#ifndef __linux__
	return (value > (expected * 1.1));
#else
	return 0;
#endif
}

/**
 * Perform application specific shutdown
 *
 * @param num_processors  The number of processors available.
 */
static void application_shutdown(int num_processors)
{
	float cycles_packet;
	float freq;
	uint64_t count;

	/* The following speed checks assume that you are using the original
	 * test data and executing with debug turned off.
	 */
	const float expected_cycles[48] = {
#if defined(PKO3_FULL_API) && defined(PACKET_ORDERING)
		675, 338, 225, 170, 135, 113, 97, 87, [8 ... 47] = 83
#elif defined(PKO3_FULL_API)
		640, 320, 214, 160, 128, 108, 92, 82, 76, 76, 76, [11 ... 47] = 75
#elif defined(PACKET_ORDERING)
		320, 160, 107, 82, [4 ... 47] = 81
#else
		256, 128, 86, 65, 52, 44, 38, 33, 30, 26, 24, 22, [12 ... 47] = 20
#endif
	};

	/* Display a rough calculation for the cycles/packet.
	 * If you need accurate results, run lots of packets.
	 */
	count = cvmx_atomic_get64(&(counters->packets));
	freq = cvmx_clock_get_rate_node(cvmx_get_node_num(), CVMX_CLOCK_CORE);

	cycles_packet = run_cycles / (float)count;
	printf("PERF: Finished processing %u packets on %u cores in %lld cycles\n",
			(unsigned) count, (unsigned) num_processors,
			(unsigned long long) run_cycles);
	printf("PERF: Performed at %.2f cycles/packet [expected %.2f cycles/packet]\n",
			cycles_packet, expected_cycles[num_processors-1]);
	printf("PERF: Estimated throughput %.2f Mpps total, %.2f Mpps/core \n",
			(freq/cycles_packet/1000000.),
			(freq/cycles_packet/1000000./num_processors));

	if (cycle_out_of_range(cycles_packet, expected_cycles[num_processors-1]))
		printf("WARNING: Cycles-per-packet is larger than the expected!\n");

	/* Display the results if a failure was detected. */
	if (cvmx_atomic_get64(&(counters->errors)))
		printf("Errors detected. TEST FAILED errors=%lld\n",
				(long long) cvmx_atomic_get64(&(counters->errors)));

	/* Wait for PKO to complete */
	printf("Waiting for packet output to finish %llu\n",
			(long long) cvmx_atomic_get64(&(counters->outstanding)));
	while (cvmx_atomic_get64(&(counters->outstanding)) != 0)
		cvmx_wait(1000);

	/* Delay so the last few packets make it out. The fetch and add
	 * is a little ahead of the hardware.
	 */
	cvmx_wait(1000000);
}

/**
 * Get the total number of packets waiting to be processed in the SSO.
 *
 * The packets could be spread across multiple nodes.
 */
static long get_pkt_count(unsigned nodemask)
{
	long ret = 0;
	unsigned node;

	for(node = 0; node < CVMX_MAX_NODES; node++) {
		if (!(nodemask & (1 << node)))
			continue;
		ret += cvmx_sso3_get_wqe_count(node);
	}
	return ret;
}

/**
 * Main entry point
 *
 * @return exit code
 */
int main(int argc, char *argv[])
{
	int node, result = 0;
	cvmx_sysinfo_t *sysinfo;
	struct cvmx_coremask coremask;
	unsigned cnt, nodemask = 0, num_nodes = 0;
	unsigned core = cvmx_get_local_core_num();
	unsigned long long start_cycle;
	bool simulator = false;

#ifndef ENABLE_PASSTHROUGH_HOTPLUG
#define CORE_MASK_BARRIER_SYNC \
	cvmx_coremask_barrier_sync(&coremask)
#else
#define CORE_MASK_BARRIER_SYNC if (!is_core_being_hot_plugged()) \
	cvmx_coremask_barrier_sync(&coremask)
#endif

	if (!(OCTEON_IS_OCTEON3() && !OCTEON_IS_MODEL(OCTEON_CN70XX))) {
		printf("passthrough_o3 is not supported on this model\n");
		return (-1);
	}
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		simulator = true;

	cvmx_user_app_init();

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
	if (init_hotplug())
		return (-1);
#endif
	/* Compute coremask on all cores for the barrier sync. */
	sysinfo = cvmx_sysinfo_get();
	cvmx_coremask_copy(&coremask, &sysinfo->core_mask);

	if (simulator)
		term_num = 6064;
	else
		term_num = 1000;

	/* Elect a core to perform boot initializations. */
	if (cvmx_is_init_core()) {
		printf("Version: %s\n", cvmx_helper_get_version());

		/* sRIO needs to be initialized before packet-io */
		if (octeon_has_feature(OCTEON_FEATURE_SRIO)) {
			extern int cvmx_srio_initialize(int srio_port, int flags);
			cvmx_srio_initialize(0, 0);
			cvmx_srio_initialize(1, 0);
		}
		for (node = 0; node < CVMX_MAX_NODES; node++) {
			if (cvmx_coremask_get64_node(&coremask, node)) {
				/* 64 is the minimum number of buffers that are allocated
				 * to receive packets, but the real hardware, allocate
				 * above this minimal number.
				 */
				result = application_init_simple_exec(node, term_num + 80);
				if (result != 0) {
					printf("Simple Executive initialization failed node %d.\n", node);
					printf("TEST FAILED\n");
					return result;
				}
				nodemask |= 1 << node;
				num_nodes ++;
			}
		}
		/* Create and atomic counters */
		counters = cvmx_bootmem_alloc(sizeof(*counters), CVMX_CACHE_LINE_SIZE);
		memset(counters, 0, sizeof(*counters));
		CVMX_SYNCWS;
	}
	CORE_MASK_BARRIER_SYNC;
	cvmx_helper_initialize_packet_io_local();

	if (simulator && cvmx_is_init_core()) {
		start_cycle = cvmx_get_cycle();
		term_num *= num_nodes;

		printf("Waiting to give packet input (~1Gbps) time to read the packets...\n");
		do {
			cnt = get_pkt_count(nodemask);
			if ((500000 + start_cycle) < cvmx_get_cycle()) {
				printf("wqe count = %u of %u\n", cnt, term_num);
				start_cycle += 500000;
			}
		} while(cnt < term_num);
		printf("Done waiting\n");
	}
	CORE_MASK_BARRIER_SYNC;

	/* Enter packet processing loop: */
	start_cycle = cvmx_get_cycle();
	app_o78_main_loop();
	loop_cycles[core] = cvmx_get_cycle() - start_cycle;

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
	if (core_unplug_requested)
		cvmx_app_hotplug_core_shutdown();

	cvmx_coremask_copy(&coremask, &sysinfo->core_mask);
	printf("Shutdown coremask=");
	cvmx_coremask_print(&coremask);
#endif
	/* Wait for all cores to finish the loop before summing up results */
	cvmx_coremask_barrier_sync(&coremask);

	/* Use core 0 to perform application shutdown as well. */
	if (simulator && cvmx_is_init_core()) {
		unsigned i, ncores;

		ncores = cvmx_coremask_get_core_count(&coremask);
		for (i = 0; i < ncores; i++)
			run_cycles = max(run_cycles, loop_cycles[i]);

		application_shutdown(ncores);
		__cvmx_bootmem_phy_free(cvmx_ptr_to_phys(counters), sizeof(*counters), 0);
		counters = NULL;
		for (node = 0; node < CVMX_MAX_NODES; node++) {
			if (cvmx_coremask_get64_node(&coremask, node)) {
				result = application_shutdown_simple_exec(node);
				if (result  != 0) {
					printf("Simple Executive shutdown failed node %d.\n", node);
					printf("TEST FAILED\n");
				}
			}
		}
	}
	cvmx_coremask_barrier_sync(&coremask);
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
	if (app_shutdown_requested)
		cvmx_app_hotplug_core_shutdown();
#endif
	printf("Program ended.\n");
	return result;
}

