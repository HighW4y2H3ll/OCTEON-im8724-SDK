/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc.y (support@cavium.com). All rights 
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

 *   * Neither the name of Cavium Inc.y nor the names of
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
 * File version info: $Id: linux-filter.c 135023 2016-03-16 06:02:20Z cchavva $
 *
 */

#include <stdio.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#endif

#include "cvmx.h"
#include "cvmx-fpa.h"
#include "cvmx-pko.h"
#include "cvmx-pow.h"
#include "cvmx-pip.h"
#include "cvmx-ipd.h"
#include "cvmx-pki.h"
#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"
#include "cvmx-helper.h"
#include "cvmx-helper-cfg.h"
#include "cvmx-app-config.h"
#include "cvmx-utils.h"

/* This define is the POW group packet destine to the Linux kernel should
    use. This must match the ethernet driver's pow_receive_group parameter */
#define TO_LINUX_GROUP          (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE) ? 23 : 14)

/* This define is the POW group packet from the Linux kernel use. This must
    match the ethernet driver's pow_send_group parameter */
#define FROM_LINUX_GROUP        (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE) ? 22 : 13)

/* This define is the POW group this program uses for packet interception.
    Packets from intercept_port are routed to this POW group instead of the
    normal ethernet pow_receive_group */
#define FROM_INPUT_PORT_GROUP   (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE) ? 24 : 0)

/* This is the Octeon hardware port number to intercept. Packets coming
    in this port are intercepted by linux-filter and processed. Packets
    received from the ethernet POW0 device are sent out this port */
CVMX_SHARED int intercept_port = 0;

/* wqe pool */
CVMX_SHARED int wqe_pool = -1;

/**
 * Determine if the supplied work queue entry and packet
 * should be filtered.
 *
 * @param work   Work queue entry to check
 * @return Non zero if packet should be filtered out
 */
static int is_filtered_packet(cvmx_wqe_t *work)
{
    /* As an example, block all IP broadcasts */
    return (cvmx_wqe_is_l2_bcast(work) && cvmx_wqe_is_l3_ip(work));
}

/**
 * Free the buffers back to the fpa.
 *
 * @param work   Work queue entry with buffers to free
 * @return Zero on success, error otherwise
 */
static int free_work(cvmx_wqe_t *work)
{
    if (octeon_has_feature(OCTEON_FEATURE_PKI)) {
	cvmx_buf_ptr_pki_t	pki_ptr;
	cvmx_buf_ptr_pki_t	next_pki_ptr;
	cvmx_wqe_78xx_t		*wqe;
	void			*buf_addr;
	int			segs;

	wqe = (cvmx_wqe_78xx_t*)work;
	pki_ptr = wqe->packet_ptr;
	buf_addr = CASTPTR(void, cvmx_ptr_to_phys(work) - 128);
	segs = cvmx_wqe_get_bufs(work);

	while (segs--) {
	    next_pki_ptr =
		    *(cvmx_buf_ptr_pki_t *)(cvmx_phys_to_ptr(pki_ptr.addr - 8));
	    cvmx_fpa_free(buf_addr, cvmx_wqe_get_aura(work), 0);
	    pki_ptr = next_pki_ptr;
	    buf_addr = CASTPTR(void, (pki_ptr.addr & ~127) - 128);
	}
    } else {
	cvmx_helper_free_packet_data(work);
	cvmx_wqe_free(work);
    }

    return 0;
}

/**
 * Main entry point
 *
 * @return exit code
 */
int main(int argc, char *argv[])
{
    long port_override = -1;
    cvmx_sysinfo_t *sysinfo = cvmx_sysinfo_get();
    cvmx_ipd_ctl_status_t ipd_reg;

    cvmx_skip_app_config_set();
    cvmx_user_app_init();

    /* Have one core do the hardware initialization */
    if (cvmx_is_init_core())
    {
	if (argc > 1)
	    port_override = strtol(argv[1], NULL, 0);

        printf("\n\nLoad the Linux ethernet driver with:\n"
               "\t $ modprobe octeon-ethernet\n");

        if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_SIM)
	    cvmx_wait(1000000000);
        printf("Waiting for ethernet module to complete initialization...\n\n\n");
	if (octeon_has_feature(OCTEON_FEATURE_PKI)) {
		cvmx_pki_buf_ctl_t buf_ctl;
		do
		{
			buf_ctl.u64 = cvmx_read_csr(CVMX_PKI_BUF_CTL);
		} while (!buf_ctl.cn78xx.pki_en);
	} else {
		do
		{
			ipd_reg.u64 = cvmx_read_csr(CVMX_IPD_CTL_STATUS);
		} while (!ipd_reg.s.ipd_en);
	}

	/* Wait a second for things to really get started. */
        if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_SIM)
	    cvmx_wait_usec(1000000);
#if CVMX_PKO_USE_FAU_FOR_OUTPUT_QUEUES
        #error Linux-filter cannot be built with CVMX_PKO_USE_FAU_FOR_OUTPUT_QUEUES
#endif

        if (OCTEON_IS_MODEL(OCTEON_CN38XX) || OCTEON_IS_MODEL(OCTEON_CN58XX))
        {
            cvmx_gmxx_inf_mode_t mode;

            /* Choose interface that is enabled and in RGMII mode. */
            mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));
            if (mode.s.en && mode.s.type == 0) {
	        /* Use interface 0 */
	        intercept_port = 0;
            } else {
	        /* Use interface 1 */
	        intercept_port = 16;
            }
        }

	/* Their is no interface 0 on nic_xle_4g card, use interface 1. */
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_NIC_XLE_4G)
	    intercept_port = 16;

	if (port_override > 0)
	    intercept_port = port_override;

	__cvmx_helper_init_port_valid();

	__cvmx_import_app_config_from_named_block(CVMX_APP_CONFIG);

	__cvmx_helper_init_port_config_data_local();

	if (octeon_has_feature(OCTEON_FEATURE_PKI)) {
	    struct cvmx_pki_port_config prt_cfg;
	    struct cvmx_pki_qpg_config qpg_cfg;
            uint64_t grp_msk[4] = {0, 0, 0, 0};

	    /* Need to steal input port group */
	    cvmx_pki_get_port_config(intercept_port, &prt_cfg);
	    cvmx_pki_show_qpg_entries(cvmx_get_node_num(), 0x4);
	    cvmx_pki_read_qpg_entry(cvmx_get_node_num(), prt_cfg.style_cfg.parm_cfg.qpg_base,
				    &qpg_cfg);
	    qpg_cfg.grp_ok = FROM_INPUT_PORT_GROUP;
	    qpg_cfg.grp_bad = FROM_INPUT_PORT_GROUP;
	    cvmx_pki_write_qpg_entry(cvmx_get_node_num(), prt_cfg.style_cfg.parm_cfg.qpg_base,
				    &qpg_cfg);

	    /* Use aura from port for packet xmit */
	    wqe_pool = qpg_cfg.aura_num;
	    if (wqe_pool == 0) {
		cvmx_printf("ERROR: FPA aura setup failed!.\n");
		return -1;
	    }

            grp_msk[FROM_INPUT_PORT_GROUP >> 6] |= 1ull << (FROM_INPUT_PORT_GROUP & 0x3f);
            grp_msk[FROM_LINUX_GROUP >> 6] |= 1ull << (FROM_LINUX_GROUP & 0x3f);
	    cvmx_pow_set_xgrp_mask(cvmx_get_core_num(), 0x1 << 0, grp_msk);
	    cvmx_pow_set_xgrp_mask(cvmx_get_core_num(), 0x1 << 1, grp_msk);
	} else if (octeon_has_feature(OCTEON_FEATURE_PKND)) {
	    cvmx_pip_prt_tagx_t tag_config;
	    cvmx_gmxx_prtx_cfg_t prt_cfg;
	    int pkind;
	    int iface = (intercept_port >> 8) - 8;
	    int iport = (intercept_port >> 4) & 0xf;
	    
	    wqe_pool = cvmx_fpa_get_wqe_pool();
	    if (iface < 0)
		iface = 0;

	    prt_cfg.u64 = cvmx_read_csr(CVMX_GMXX_PRTX_CFG(iport, iface));
	    pkind = prt_cfg.s.pknd;

	    tag_config.u64 = cvmx_read_csr(CVMX_PIP_PRT_TAGX(pkind));
	    tag_config.s.grp = FROM_INPUT_PORT_GROUP & 0xf;
	    tag_config.s.grp_msb = (FROM_INPUT_PORT_GROUP >> 4) & 3;
	    cvmx_write_csr(CVMX_PIP_PRT_TAGX(pkind), tag_config.u64);
	    cvmx_pow_set_group_mask(cvmx_get_core_num(),
				    (1<<FROM_INPUT_PORT_GROUP)|(1<<FROM_LINUX_GROUP));
	} else {
	    /* Change the group for only the port we're interested in */
	    cvmx_pip_port_tag_cfg_t tag_config;

	    wqe_pool = cvmx_fpa_get_wqe_pool();
	    tag_config.u64 = cvmx_read_csr(CVMX_PIP_PRT_TAGX(intercept_port));
	    tag_config.s.grp = FROM_INPUT_PORT_GROUP;
	    cvmx_write_csr(CVMX_PIP_PRT_TAGX(intercept_port), tag_config.u64);
	}
	if (octeon_has_feature(OCTEON_FEATURE_FPA3))
	    cvmx_printf("Load POW driver with:\n"
		"\t $ modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=%d ptp_tx_group=%d pki_packet_pool=%i\n",
			TO_LINUX_GROUP, FROM_LINUX_GROUP, wqe_pool);
	else
	    cvmx_printf("Load POW driver with:\n"
		"\t $ modprobe octeon-pow-ethernet receive_group=1 broadcast_groups=4 ptp_rx_group=%d ptp_tx_group=%d\n",
			TO_LINUX_GROUP, FROM_LINUX_GROUP);
        /* We need to call cvmx_cmd_queue_initialize() to get the pointer to
            the named block. The queues are already setup by the ethernet
            driver, so we don't actually need to setup a queue. Pass some
            invalid parameters to cause the queue setup to fail */
	if (!octeon_has_feature(OCTEON_FEATURE_FPA3))
		cvmx_cmd_queue_initialize(0, 0, -1, 0);
        printf("Waiting for packets from port %d... \n", intercept_port);
    }

    /* Wait for global hardware init to complete */
    cvmx_coremask_barrier_sync(&sysinfo->core_mask);

    /* Setup scratch registers used to prefetch output queue buffers for packet output */
    cvmx_pko_initialize_local();

    if (!octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
	    /* Accept any packet except for the ones destined to the Linux group */
	    cvmx_pow_set_group_mask(cvmx_get_core_num(),
				    (1<<FROM_INPUT_PORT_GROUP)|(1<<FROM_LINUX_GROUP));
    }

    /* Wait for hardware init to complete */
    cvmx_coremask_barrier_sync(&sysinfo->core_mask);

    while (1)
    {
	cvmx_wqe_t *work;
#ifdef __linux__
        /* Under Linux there better thing to do than halt the CPU waiting for
            work to show up. Here we use NO_WAIT so we can continue processing
            instead of stalling for work */
	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		work = cvmx_sso_work_request_grp_sync_nocheck(FROM_LINUX_GROUP, CVMX_POW_NO_WAIT);
		if (!work)
			work = cvmx_sso_work_request_grp_sync_nocheck(FROM_INPUT_PORT_GROUP,
							      CVMX_POW_NO_WAIT);
	} else
		work = cvmx_pow_work_request_sync(CVMX_POW_NO_WAIT);

        if (work == NULL)
        {
            /* Yield to other processes since we don't have anything to do */
            usleep(0);
            continue;
        }
#else
        /* In standalone CVMX, we have nothing to do if there isn't work, so
            use the WAIT flag to reduce power usage */
	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		work = cvmx_sso_work_request_grp_sync_nocheck(FROM_LINUX_GROUP, CVMX_POW_NO_WAIT);
		if (!work)
			work = cvmx_sso_work_request_grp_sync_nocheck(FROM_INPUT_PORT_GROUP,
							      CVMX_POW_NO_WAIT);
	} else
	    work = cvmx_pow_work_request_sync(CVMX_POW_WAIT);
        if (work == NULL)
            continue;
#endif

        /* Check for errored packets, and drop.  If sender does not respond to
            backpressure or backpressure is not sent, packets may be truncated
            if the GMX fifo overflows. */
        if (cvmx_wqe_get_rcv_err(work))
        {
	    cvmx_printf("RCV_ERR: %i.\n", cvmx_wqe_get_grp(work));
            /* Work has error, so drop */
	    free_work(work);
            continue;
        }

        /* See if we should filter this packet */
        if (is_filtered_packet(work))
        {
            printf("Received %u byte packet. Filtered.\n", cvmx_wqe_get_len(work));
	    free_work(work);
        }
        else if (cvmx_wqe_get_xgrp(work) == FROM_LINUX_GROUP)
        {
	    uint64_t queue = cvmx_pko_get_base_queue(intercept_port);
            cvmx_pko_command_word0_t pko_command;
	    cvmx_pko_return_value_t send_status;

            printf("Received %u byte packet from Linux. Sending to PKO\n", cvmx_wqe_get_len(work));

	    if (octeon_has_feature(OCTEON_FEATURE_PKO3)) {
		cvmx_pko3_pdesc_t pdesc;
		cvmx_wqe_78xx_t *wqe = (cvmx_wqe_78xx_t*)work;
		int i, rc, segs = cvmx_wqe_get_bufs(work);
		cvmx_buf_ptr_pki_t pki_ptr = wqe->packet_ptr;
		uint64_t buf_addr = cvmx_ptr_to_phys(work) - 128;
		int len = cvmx_wqe_get_len(work);

		cvmx_pko3_pdesc_init(&pdesc);
		for (i = 0; i < segs; i++) {
		    int segment_size = pki_ptr.size;

		    if (segment_size > len)
			segment_size = len;

		    cvmx_pko3_pdesc_buf_append(&pdesc, cvmx_phys_to_ptr(pki_ptr.addr),
					   segment_size, wqe_pool);
		    /* Free packet to fpa manually, as octeon3-ethernet has a different buffer format */
		    cvmx_pko3_pdesc_append_free(&pdesc, buf_addr, wqe_pool);
		    pki_ptr = *(cvmx_buf_ptr_pki_t*)(cvmx_phys_to_ptr(pki_ptr.addr - 8));
		    buf_addr = (pki_ptr.addr & ~127) - 128;
		    len -= segment_size;
		}

		cvmx_pko3_pdesc_set_free(&pdesc, false);
		rc = cvmx_pko3_pdesc_transmit(&pdesc, queue, NULL);
		if (rc < 0) {
		    printf("Failed to send packet using cvmx_pko3_pdesc_transmit\n");
		    free_work(work);
		}
	    } else {
		cvmx_pko_send_packet_prepare(intercept_port, queue, CVMX_PKO_LOCK_CMD_QUEUE);

		/* Build a PKO pointer to this packet */
		pko_command.u64 = 0;
		pko_command.s.segs = cvmx_wqe_get_bufs(work);
		pko_command.s.total_bytes = cvmx_wqe_get_len(work);

		if (cvmx_wqe_is_l4_udp_or_tcp(work) && !cvmx_wqe_is_l3_frag(work))
		    pko_command.s.ipoffp1 = 14 + 1;
		else
		    pko_command.s.ipoffp1 = 0;

		/* Send the packet */
		send_status = cvmx_pko_send_packet_finish(intercept_port, queue,
							  pko_command, work->packet_ptr,
							  CVMX_PKO_LOCK_CMD_QUEUE);
		if (send_status != CVMX_PKO_SUCCESS)
		{
		    printf("Failed to send packet using cvmx_pko_send_packet\n");
		    cvmx_helper_free_packet_data(work);
		}
		cvmx_wqe_free(work);
	    }
        }
        else
        {
	    printf("Received %u byte packet. Sending to Linux.\n",
	      cvmx_wqe_get_len(work));
#ifdef __linux__
            /* If we're running under Linux userspace we can't desched since
                the ethernet driver might give away our tag. Use submit work
                instead */
            if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		    cvmx_pow_work_submit_node(work, cvmx_wqe_get_tag(work), cvmx_wqe_get_tt(work), TO_LINUX_GROUP, cvmx_get_node_num());
	    else
		cvmx_pow_work_submit(work, cvmx_wqe_get_tag(work), cvmx_wqe_get_tt(work), cvmx_wqe_get_qos(work), TO_LINUX_GROUP);
#else
            /* Forward the packet to the linux kernel.
               It is recommented to switch explicitly to an ATOMIC tag during deschedule.
               Please see documentation of cvmx_pow_tag_sw_desched() for details. */
            if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		cvmx_pow_work_submit_node(work, cvmx_wqe_get_tag(work),
					  cvmx_wqe_get_tt(work), TO_LINUX_GROUP,
					  cvmx_get_node_num());
	    else
		cvmx_pow_tag_sw_desched(work->word1.tag, CVMX_POW_TAG_TYPE_ATOMIC, TO_LINUX_GROUP, 0);
#endif
        }
    }

    return 0;
}
