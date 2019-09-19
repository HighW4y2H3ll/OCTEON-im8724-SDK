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


/*
 * File version info: $Id: passthrough.c 129707 2015-12-14 23:08:02Z cchavva $
 *
 */

#include <stdio.h>
#include <string.h>

#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-spinlock.h"
#include "cvmx-fpa.h"
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
#include "cvmx-srio.h"
#include "cvmx-config-parse.h"
#include "cvmx-helper-ipd.h"
#include "cvmx-helper-bgx.h"

/*
 * Configure passthrough to run with lockless pko operations.
 */
//#define ENABLE_LOCKLESS_PKO
#define FAU_PACKETS     ((cvmx_fau_reg_64_t)(CVMX_FAU_REG_AVAIL_BASE + 0))   /**< Fetch and add for counting packets processed */
#define FAU_ERRORS      ((cvmx_fau_reg_64_t)(CVMX_FAU_REG_AVAIL_BASE + 8))   /**< Fetch and add for counting detected errors */
#define FAU_OUTSTANDING ((cvmx_fau_reg_64_t)(CVMX_FAU_REG_AVAIL_BASE + 16))  /**< Fetch and add for counting outstanding packets */

static CVMX_SHARED uint64_t start_cycle;
static CVMX_SHARED uint64_t stop_cycle;
static unsigned int packet_termination_num;

static int volatile core_unplug_requested  = 0;
static int volatile app_shutdown_requested = 0;

//#define ENABLE_PASSTHROUGH_HOTPLUG 1

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
    passthrough_hp_cb_args_data.mem_ptr = (void*)0xA5A5A5A5;

    if (is_core_being_hot_plugged())
    {
        printf("core=%d is being hotplugged\n", cvmx_get_core_num());
    }

    if (cvmx_is_init_core())
    {
        cvmx_app_hotplug_callbacks_t cb;
        bzero(&cb, sizeof(cb));
        cb.shutdown_callback =  (void(*)(void*))passthrough_shutdown_callback;
        cb.unplug_core_callback =  (void(*)(void*))passthrough_unplug_callback;
        /* Register application for hotplug. this only needs to be done once */
        cvmx_app_hotplug_register_cb(&cb, &passthrough_hp_cb_args_data, app_shutdown);
    }

    /* Activate hotplug */
    if (cvmx_app_hotplug_activate())
    {
        printf("ERROR: cvmx_hotplug_activate() failed\n");
        return -1;
    }
    return 0;
}

#endif

/* Note: The dump_packet routine that used to be here has been moved to
    cvmx_helper_dump_packet. */
//#define DUMP_PACKETS 1
//#define DUMP_STATS
//#define SWAP_MAC_ADDR

#ifdef SWAP_MAC_ADDR
static inline void
swap_mac_addr(uint64_t pkt_ptr)
{
    uint16_t s;
    uint32_t w;

    /* assuming an IP/IPV6 pkt i.e. L2 header is 2 byte aligned, 4 byte non-aligned */
    s = *(uint16_t*)pkt_ptr;
    w = *(uint32_t*)(pkt_ptr+2);
    *(uint16_t*)pkt_ptr = *(uint16_t*)(pkt_ptr+6);
    *(uint32_t*)(pkt_ptr+2) = *(uint32_t*)(pkt_ptr+8);
    *(uint16_t*)(pkt_ptr+6) = s;
    *(uint32_t*)(pkt_ptr+8) = w;
}
#endif

/**
 * Setup the Cavium Simple Executive Libraries using defaults
 *
 * @param num_packet_buffers
 *               Number of outstanding packets to support
 * @return Zero on success
 */
static int application_init_simple_exec(int num_packet_buffers)
{
    int result;

    if (cvmx_helper_initialize_fpa(num_packet_buffers, num_packet_buffers, CVMX_PKO_MAX_OUTPUT_QUEUES * 4, 0, 0))
        return -1;

    if (cvmx_helper_initialize_sso(num_packet_buffers))
        return -1;

    if (octeon_has_feature(OCTEON_FEATURE_NO_WPTR))
    {
	printf("Enabling [NO_WPTR]\n");
	cvmx_helper_ipd_set_wqe_no_ptr_mode(1);
    }
#ifdef __LITTLE_ENDIAN_BITFIELD
	cvmx_helper_ipd_pkt_wqe_le_mode(1);
#endif

    cvmx_helper_cfg_opt_set(CVMX_HELPER_CFG_OPT_USE_DWB, 0);
    result = cvmx_helper_initialize_packet_io_global();

    /* Don't enable RED on simulator */
    if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_SIM)
        if(!octeon_has_feature(OCTEON_FEATURE_PKI))
		    cvmx_helper_setup_red(num_packet_buffers/4, num_packet_buffers/8);

    /* Leave 16 bytes space for the ethernet header */
    if(!octeon_has_feature(OCTEON_FEATURE_PKI))
	cvmx_write_csr(CVMX_PIP_IP_OFFSET, 2);

    cvmx_helper_cfg_set_jabber_and_frame_max();
    cvmx_helper_cfg_store_short_packets_in_wqe();

    /* Initialize the FAU registers. */
    cvmx_fau_atomic_write64(FAU_ERRORS, 0);
    if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
    {
        cvmx_fau_atomic_write64(FAU_PACKETS, 0);
        cvmx_fau_atomic_write64(FAU_OUTSTANDING, 0);
    }

    return result;
}

/**
 * Clean up and properly shutdown the simple exec libraries.
 *
 * @return Zero on success. Non zero means some resources are
 *         unaccounted for. In this case error messages will have
 *         been displayed during shutdown.
 */
static int application_shutdown_simple_exec(void)
{
    int result = 0;

    cvmx_helper_shutdown_packet_io_global();

    cvmx_helper_uninitialize_sso();

    result = cvmx_helper_shutdown_fpa_pools(0);

    return result;
}

/**
 * Process incoming packets. Just send them back out the
 * same interface.
 *
 */
void application_main_loop(void)
{
    cvmx_wqe_t *    work;
    uint64_t        port;
    cvmx_pow_wait_t pow_wait = CVMX_POW_WAIT;
    cvmx_pko_lock_t lock_type = CVMX_PKO_LOCK_ATOMIC_TAG;
    cvmx_buf_ptr_t  packet_ptr;
    cvmx_pko_command_word0_t pko_command;
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
    int holds_atomic_tag = 0;
#endif
    int queue, ret, corenum;
    int error;
    bool simulator;

    corenum = cvmx_get_core_num();
    simulator = cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM;

    /* Build a PKO pointer to this packet */
    pko_command.u64 = 0;
#ifdef __LITTLE_ENDIAN_BITFIELD
    pko_command.s.le = 1;
#endif
#ifdef ENABLE_LOCKLESS_PKO
    lock_type = CVMX_PKO_LOCK_NONE;
#endif

    if (simulator)
    {
       pko_command.s.size0 = CVMX_FAU_OP_SIZE_64;
       pko_command.s.subone0 = 1;
       pko_command.s.reg0 = FAU_OUTSTANDING;
       pow_wait = CVMX_POW_NO_WAIT;
    }

    while (1)
    {

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
        if ((core_unplug_requested || app_shutdown_requested) && (app_shutdown))
        {
            printf("core=%d : is unplugged\n",cvmx_get_core_num());
            if (holds_atomic_tag) cvmx_pow_tag_sw_null_nocheck();
            break;
        }
#endif
	work = cvmx_pow_work_request_sync(pow_wait);

        /* get the next packet/work to process from the POW unit. */
	if (cvmx_unlikely(work == NULL)) {
            if (simulator) {
               if (cvmx_fau_fetch_and_add64(FAU_PACKETS, 0)
		    >= packet_termination_num)
                   break;
	    }
               continue;
	}

        /* Check for errored packets, and drop.
	 * If sender does not respond
         * to backpressure or backpressure is not sent,
         * packets may be truncated if
         * the GMX fifo overflows
         */
	if (cvmx_unlikely((error = cvmx_wqe_get_rcv_err(work))))
	{
            /* Work has error, so drop */
            cvmx_helper_free_packet_data(work);
	    cvmx_wqe_free(work);
            continue;
        }

	/* Determine packet ingress port */
        port = cvmx_wqe_get_port(work);

        /*
         * Insert packet processing here.
         *
         * Define DUMP_PACKETS to dump packets to the console.
         * Note that due to multiple cores executing in parallel, the output
         * will likely be interleaved.
         *
         */
        #ifdef DUMP_PACKETS
        printf("Processing packet\n");
        cvmx_helper_dump_packet(work);
        #endif

#ifdef DUMP_STATS
        printf ("port to send out: %lu\n", port);
        cvmx_helper_show_stats(port);
#endif


        /*
         * Begin packet output by requesting a tag switch to atomic.
         * Writing to a packet output queue must be synchronized across cores.
         */
	queue = cvmx_pko_get_base_queue(port);
#ifdef ENABLE_LOCKLESS_PKO
	queue += corenum;
#else
	queue += (corenum % cvmx_pko_get_num_queues(port));
#endif
        cvmx_pko_send_packet_prepare(port, queue, lock_type);

        if (simulator)
        {
           /* Increment the total packet counts */
           cvmx_fau_atomic_add64(FAU_PACKETS, 1);
           cvmx_fau_atomic_add64(FAU_OUTSTANDING, 1);
        }

        /* Interlaken - fix port number */
        if (cvmx_unlikely(((port & 0xefe) == 0x480)))/*0x400,0x401,0x500,0x501*/
            port &= ~0x80;


	/* Access packet data */
	packet_ptr = cvmx_wqe_get_packet_ptr(work);

        #ifdef SWAP_MAC_ADDR
        if (cvmx_wqe_is_l3_ip(work))
            swap_mac_addr((uint64_t)cvmx_phys_to_ptr(packet_ptr.s.addr));
        #endif

	#ifdef	CHECKSUM_GEN
	/* Set L3/L4 checksum genereation request */
        if (cvmx_wqe_is_l3_ip(work))
		pko_command.s.ipoffp1 = 1 + cvmx_wqe_get_l3_offset(work);
	else
		pko_command.s.ipoffp1 = 0;
	#endif

	/* Fill per-packet PKO command fields */
	pko_command.s.total_bytes = cvmx_wqe_get_len(work);
	ret = cvmx_wqe_get_bufs(work);
	cvmx_warn_if(ret == 0 || ret >= (1 << 6), "Too many segments\n");
	pko_command.s.segs = ret;

        /* For SRIO interface, build the header and remove SRIO RX word 0 */
        if (cvmx_unlikely(octeon_has_feature(OCTEON_FEATURE_SRIO))
	    && port >= 40 && port < 44)
        {
            if (cvmx_srio_omsg_desc(port, &packet_ptr, NULL) >= 0)
                pko_command.s.total_bytes -= 8;
        }

	/* This must be called before packet_finish() to prevent race */
	cvmx_wqe_free(work);

        /*
         * Send the packet and wait for the tag switch to complete before
         * accessing the output queue. This ensures the locking required
         * for the queue.
         *
         */
        ret = cvmx_pko_send_packet_finish(port, queue, pko_command,
				packet_ptr, lock_type);

        if (cvmx_unlikely(ret))
        {
            printf("Failed to send packet using cvmx_pko_send_packet_finish\n");
            cvmx_fau_atomic_add64(FAU_ERRORS, 1);
	    /* FIXME: This will not always work since wqe has been freed */
            cvmx_helper_free_packet_data(work);
        }

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
        holds_atomic_tag = 1;
#endif
    }
}



/**
 * Determine if a number is approximately equal to a match
 * value. Checks if the supplied value is within 5% of the
 * expected value.
 *
 * @param value    Value to check
 * @param expected Value needs to be within 5% of this value.
 * @return Non zero if the value is out of range.
 */
static int cycle_out_of_range(float value, float expected)
{
    uint64_t range = expected / 5; /* 5% */
    if (range<1)
        range = 1;

    /* The expected time check is disabled for Linux right now. Since the
        Linux kernel is configured for a 6 Mhz clock, there are a couple
        of context switch during this test. On the real chip the clock will
        be set the real value (600 Mhz) alleviating this problem. */
#ifndef __linux__
    return ((value < expected - range) || (value > expected + range));
#else
    return 0;
#endif
}


/**
 * Perform application specific shutdown
 *
 * @param num_processors
 *               The number of processors available.
 */
static void application_shutdown(int num_processors)
{
    uint64_t count, run_cycles = stop_cycle - start_cycle;
    float cycles_packet;

    /* The following speed checks assume you are using the original test data
        and executing with debug turned off. */
    const float * expected_cycles;
    const float cn78xx_cycles[48] = {500.0, 400.0, 300.0, 180.0,
	160.0, 140.0, 120.0, 100.0,
	[8 ... 47] = 80.0};
    const float cn68xx_cycles[32] = {350.0, 175.0, 130.0, 92.0, 90.0, 85.0, 80.0, 65.0, 65.0, 65.0, 65.0, 65.0 , 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0, 65.0};
    const float cn6xxx_cycles[10] = {350.0, 175.0, 130.0, 92.0, 90.0, 85.0, 80.0, 65.0, 65.0, 65.0};
    const float cn50xx_cycles[2] = {282.0, 156.0};
    const float cn3xxx_cycles[16] = {244.0, 123.0, 90.0, 63.0, 55.0, 47.0, 42.0, 39.0, 38.0, 38.0, 38.0, 38.0, 38.0, 38.0, 38.0, 38.0};
    const float cn3020_cycles[2] = {272.0, 150.0};
    const float cn3010_cycles[1] = {272.0};
    const float cn3005_cycles[1] = {315.0};

    if (OCTEON_IS_MODEL(OCTEON_CN3005))
        expected_cycles = cn3005_cycles;
    else if (OCTEON_IS_MODEL(OCTEON_CN3020))
        expected_cycles = cn3020_cycles;
    else if (OCTEON_IS_MODEL(OCTEON_CN30XX))
        expected_cycles = cn3010_cycles;
    else if (OCTEON_IS_MODEL(OCTEON_CN50XX))
        expected_cycles = cn50xx_cycles;
    else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
        expected_cycles = cn68xx_cycles;
    else if (OCTEON_IS_OCTEON2() || OCTEON_IS_MODEL(OCTEON_CN70XX))
        expected_cycles = cn6xxx_cycles;
    else if (OCTEON_IS_OCTEON3())
        expected_cycles = cn78xx_cycles;
    else
        expected_cycles = cn3xxx_cycles;

    /* Display a rough calculation for the cycles/packet. If you need
        accurate results, run lots of packets. */
    count = cvmx_fau_fetch_and_add64(FAU_PACKETS, 0);
    cycles_packet = run_cycles / (float)count;
    printf("Total %lld packets in %lld cycles (%2.2f cycles/packet)[expected %2.2f cycles/packet]\n",
           (unsigned long long)count, (unsigned long long)run_cycles, cycles_packet, expected_cycles[num_processors-1]);

    if (cycle_out_of_range(cycles_packet, expected_cycles[num_processors-1]))
        printf("Cycles-per-packet is larger than the expected!\n");

    /* Display the results if a failure was detected. */
    if (cvmx_fau_fetch_and_add64(FAU_ERRORS, 0))
        printf("Errors detected. TEST FAILED\n");

    /* Wait for PKO to complete */
    printf("Waiting for packet output to finish\n");
    while (cvmx_fau_fetch_and_add64(FAU_OUTSTANDING, 0) != 0)
    {
        /* Spinning again */
    }

    /* Delay so the last few packets make it out. The fetch and add
        is a little ahead of the hardware */
    cvmx_wait(1000000);
}

/**
 * Main entry point
 *
 * @return exit code
 */
int main(int argc, char *argv[])
{
    cvmx_sysinfo_t *sysinfo;
    struct cvmx_coremask coremask_passthrough;
    int result = 0;

#ifndef ENABLE_PASSTHROUGH_HOTPLUG
#define CORE_MASK_BARRIER_SYNC\
        cvmx_coremask_barrier_sync(&coremask_passthrough)
#else
#define CORE_MASK_BARRIER_SYNC if (!is_core_being_hot_plugged())\
        cvmx_coremask_barrier_sync(&coremask_passthrough)
#endif
#define IS_INIT_CORE	(cvmx_is_init_core())

#ifdef ENABLE_USING_CONFIG_STRING
    if (IS_INIT_CORE) {
	    cvmx_dprintf("Using config string \n");
	    cvmx_set_app_config_str(app_config_str);
    }
#endif

    cvmx_user_app_init();

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
    if (init_hotplug())
        return -1;
#endif

    /* compute coremask_passthrough on all cores for the first barrier sync below */
    sysinfo = cvmx_sysinfo_get();
    cvmx_coremask_copy(&coremask_passthrough, &sysinfo->core_mask);

    if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
    {
        if (OCTEON_IS_MODEL(OCTEON_CN3005) || OCTEON_IS_MODEL(OCTEON_CNF71XX))
            packet_termination_num = 3032;
        else if (OCTEON_IS_MODEL(OCTEON_CN31XX) || OCTEON_IS_MODEL(OCTEON_CN3010) || OCTEON_IS_MODEL(OCTEON_CN50XX))
            packet_termination_num = 4548;
        else if (OCTEON_IS_MODEL(OCTEON_CN70XX))
            packet_termination_num = 1516;
        else
//#define SINGLE_PORT_SIM
#ifdef SINGLE_PORT_SIM
            packet_termination_num = 1516;
#else
            packet_termination_num = 6064;
#endif
    }
    else
       packet_termination_num = 1000;

    /*
     * elect a core to perform boot initializations, as only one core needs to
     * perform this function.
     *
     */

    if (IS_INIT_CORE) {
        printf("Version: %s\n", cvmx_helper_get_version());

        if (octeon_has_feature(OCTEON_FEATURE_SRIO))
        {
            int srio0, srio1;
            if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
                srio0 = 1;
                srio1 = 2;
            } else {
                srio0 = 4;
                srio1 = 5;
            }
            if (cvmx_helper_interface_get_mode(srio0) == CVMX_HELPER_INTERFACE_MODE_SRIO)
                cvmx_srio_initialize(0, 0);
            if (cvmx_helper_interface_get_mode(srio1) == CVMX_HELPER_INTERFACE_MODE_SRIO)
                cvmx_srio_initialize(1, 0);
        }

        /* 64 is the minimum number of buffers that are allocated to receive
           packets, but the real hardware, allocate above this minimal number. */
        if ((result = application_init_simple_exec(packet_termination_num+80)) != 0) {
            printf("Simple Executive initialization failed.\n");
            printf("TEST FAILED\n");
            return result;
        }
    }

    CORE_MASK_BARRIER_SYNC;


#ifdef ENABLE_LOCKLESS_PKO
    /* First core do some runtime sanity check.
       Make sure there is enough queues for each core online */
    if (IS_INIT_CORE) {
	    int cores_online = cvmx_coremask_get_core_count(&coremask_passthrough);

            if ((cores_online  > CVMX_PKO_QUEUES_PER_PORT_INTERFACE0)
            || (cores_online > CVMX_PKO_QUEUES_PER_PORT_INTERFACE1))  {
            printf ("Lockless PKO operation requires each running\n");
            printf ("core to use a dedicated PKO queue for each port\n");
                   printf ("%d cores are online\n", cores_online);
            printf ("Interface 0 has %d queues per port \n",
                                       CVMX_PKO_QUEUES_PER_PORT_INTERFACE0);
            printf ("Interface 1 has %d queues per port \n",
                                       CVMX_PKO_QUEUES_PER_PORT_INTERFACE1);
            printf ("Failed to enable Lockless PKO   \n");
            return -1;
        }
        printf("Enable Lockless PKO\n");
    }
    CORE_MASK_BARRIER_SYNC;
#endif

    cvmx_helper_initialize_packet_io_local();

    /* Remember when we started the test.  For accurate numbers it needs to be as
       close as possible to the running of the application main loop. */
    if (IS_INIT_CORE) {
        if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
        {
	    unsigned cnt;
	    start_cycle = cvmx_get_cycle();
            printf("Waiting to give packet input (~1Gbps) time to read the packets...\n");
	    do {
	        cnt = cvmx_sso_get_total_wqe_count();
		if (500000+start_cycle < cvmx_get_cycle()) {
                    printf("wqe count = %u of %u\n", cnt, packet_termination_num);
		    start_cycle += 500000;
		}
	    } while(cnt < packet_termination_num);

            printf("Done waiting\n");
        }

        start_cycle = cvmx_get_cycle();
    }

    CORE_MASK_BARRIER_SYNC;
    application_main_loop();

#ifdef ENABLE_PASSTHROUGH_HOTPLUG
    if (core_unplug_requested)
        cvmx_app_hotplug_core_shutdown();

    cvmx_coremask_copy(&coremask_passthrough, &sysinfo->core_mask);
    printf("Shutdown coremask=");
    cvmx_coremask_print(&coremask_passthrough);
#endif

    cvmx_coremask_barrier_sync(&coremask_passthrough);


    /* Remember when we stopped the test. This could have been done in the
       application_shutdown, but for accurate numbers it needs to be as close as
       possible to the running of the application main loop. */
    if (cvmx_is_init_core())
        stop_cycle = cvmx_get_cycle();

    cvmx_coremask_barrier_sync(&coremask_passthrough);

#if CVMX_PKO_USE_FAU_FOR_OUTPUT_QUEUES
    /* Free the prefetched output queue buffer if allocated */
    {
        void * buf_ptr = cvmx_phys_to_ptr(cvmx_scratch_read64(CVMX_SCR_OQ_BUF_PRE_ALLOC));
        if (buf_ptr)
		cvmx_fpa_free(buf_ptr, cvmx_fpa_get_pko_pool(), 0);
    }
#endif

    /* use core 0 to perform application shutdown as well. */
    if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM &&
        cvmx_is_init_core())
    {

        int num_processors;
	num_processors = cvmx_coremask_get_core_count(&coremask_passthrough);
        application_shutdown(num_processors);

        if ((result = application_shutdown_simple_exec()) != 0) {
            printf("Simple Executive shutdown failed.\n");
            printf("TEST FAILED\n");
        }
    }

    cvmx_coremask_barrier_sync(&coremask_passthrough);
#ifdef ENABLE_PASSTHROUGH_HOTPLUG
    if (app_shutdown_requested)
        cvmx_app_hotplug_core_shutdown();
#endif

    return result;
}
