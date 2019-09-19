/***********************license start***************
 * Copyright (c) 2003-2016  Cavium Inc. (support@cavium.com). All rights
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
 * Functions and typedefs for using Octeon in HiGig/HiGig+/HiGig2 mode over
 * XAUI.
 *
 * <hr>$Revision: 144255 $<hr>
 */

#ifndef __CVMX_HIGIG_H__
#define __CVMX_HIGIG_H__
#include "cvmx-wqe.h"
#include "cvmx-pki.h"
#include "cvmx-pki-resources.h"
#include "cvmx-helper.h"
#include "cvmx-helper-bgx.h"
#include "cvmx-helper-util.h"
#include "cvmx-bgxx-defs.h"

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

typedef struct {
	union {
		uint32_t u32;
		struct {
			uint32_t start:8;
					 /**< 8-bits of Preamble indicating start of frame */
			uint32_t hgi:2;
					 /**< HiGig interface format indicator
                                            00 = Reserved
                                            01 = Pure preamble - IEEE standard framing of 10GE
                                            10 = XGS header - framing based on XGS family definition In this
                                                format, the default length of the header is 12 bytes and additional
                                                bytes are indicated by the HDR_EXT_LEN field
                                            11 = Reserved */
			uint32_t cng_high:1;
					 /**< Congestion Bit High flag */
			uint32_t hdr_ext_len:3;
					 /**< This field is valid only if the HGI field is a b'10' and it indicates the extension
                                            to the standard 12-bytes of XGS HiGig header. Each unit represents 4
                                            bytes, giving a total of 16 additional extension bytes. Value of b'101', b'110'
                                            and b'111' are reserved. For HGI field value of b'01' this field should be
                                            b'01'. For all other values of HGI it is don't care. */
			uint32_t src_modid_6:1;
					 /**< This field is valid only if the HGI field is a b'10' and it represents Bit 6 of
                                            SRC_MODID (bits 4:0 are in Byte 4 and bit 5 is in Byte 9). For HGI field
                                            value of b'01' this field should be b'0'. For all other values of HGI it is don't
                                            care. */
			uint32_t dst_modid_6:1;
					 /**< This field is valid only if the HGI field is a b'10' and it represents Bit 6 of
                                            DST_MODID (bits 4:0 are in Byte 7 and bit 5 is in Byte 9). ). For HGI field
                                            value of b'01' this field should be b'1'. For all other values of HGI it is don't
                                            care. */
			uint32_t vid_high:8;
					 /**< 8-bits of the VLAN tag information */
			uint32_t vid_low:8;
					 /**< 8 bits LSB of the VLAN tag information */
		} s;
	} dw0;
	union {
		uint32_t u32;
		struct {
			uint32_t src_modid_low:5;
					 /**< Bits 4:0 of Module ID of the source module on which the packet ingress (bit
                                            5 is in Byte 9 and bit 6 Is in Byte 1) */
			uint32_t opcode:3;
					 /**< XGS HiGig op-code, indicating the type of packet
                                            000 =     Control frames used for CPU to CPU communications
                                            001 =     Unicast packet with destination resolved; The packet can be
                                                      either Layer 2 unicast packet or L3 unicast packet that was
                                                      routed in the ingress chip.
                                            010 =     Broadcast or unknown Unicast packet or unknown multicast,
                                                      destined to all members of the VLAN
                                            011 =     L2 Multicast packet, destined to all ports of the group indicated
                                                      in the L2MC_INDEX which is overlayed on DST_PORT/DST_MODID fields
                                            100 =     IP Multicast packet, destined to all ports of the group indicated
                                                      in the IPMC_INDEX which is overlayed on DST_PORT/DST_MODID fields
                                            101 =     Reserved
                                            110 =     Reserved
                                            111 =     Reserved */
			uint32_t pfm:2;
					 /**< Three Port Filtering Modes (0, 1, 2) used in handling registed/unregistered
                                            multicast (unknown L2 multicast and IPMC) packets. This field is used
                                            when OPCODE is 011 or 100 Semantics of PFM bits are as follows;
                                            For registered L2 multicast packets:
                                                PFM= 0 � Flood to VLAN
                                                PFM= 1 or 2 � Send to group members in the L2MC table
                                            For unregistered L2 multicast packets:
                                                PFM= 0 or 1 � Flood to VLAN
                                                PFM= 2 � Drop the packet */
			uint32_t src_port_tgid:6;
					 /**< If the MSB of this field is set, then it indicates the LAG the packet ingressed
                                            on, else it represents the physical port the packet ingressed on. */
			uint32_t dst_port:5;
					 /**< Port number of destination port on which the packet needs to egress. */
			uint32_t priority:3;
					 /**< This is the internal priority of the packet. This internal priority will go through
                                            COS_SEL mapping registers to map to the actual MMU queues. */
			uint32_t header_type:2;
					 /**< Indicates the format of the next 4 bytes of the XGS HiGig header
                                            00 = Overlay 1 (default)
                                            01 = Overlay 2 (Classification Tag)
                                            10 = Reserved
                                            11 = Reserved */
			uint32_t cng_low:1;
					 /**< Semantics of CNG_HIGH and CNG_LOW are as follows: The following
                                            encodings are to make it backward compatible:
                                            [CNG_HIGH, CNG_LOW] - COLOR
                                            [0, 0] � Packet is green
                                            [0, 1] � Packet is red
                                            [1, 1] � Packet is yellow
                                            [1, 0] � Undefined */
			uint32_t dst_modid_low:5;
					 /**< Bits [4-: 0] of Module ID of the destination port on which the packet needs to egress. */
		} s;
	} dw1;
	union {
		uint32_t u32;
		struct {
			uint32_t dst_t:1;
					 /**< Destination Trunk: Indicates that the destination port is a member of a trunk
                                            group. */
			uint32_t dst_tgid:3;
					 /**< Destination Trunk Group ID: Trunk group ID of the destination port. The
                                            DO_NOT_LEARN bit is overlaid on the second bit of this field. */
			uint32_t ingress_tagged:1;
					 /**< Ingress Tagged: Indicates whether the packet was tagged when it originally
                                            ingressed the system. */
			uint32_t mirror_only:1;
					 /**< Mirror Only: XGS 1/2 mode: Indicates that the packet was switched and only
                                            needs to be mirrored. */
			uint32_t mirror_done:1;
					 /**< Mirroring Done: XGS1/2 mode: Indicates that the packet was mirrored and
                                            may still need to be switched. */
			uint32_t mirror:1;
					 /**< Mirror: XGS3 mode: a mirror copy packet. XGS1/2 mode: Indicates that the
                                            packet was switched and only needs to be mirrored. */

			uint32_t src_modid_5:1;
					 /**< Source Module ID: Bit 5 of Src_ModID (bits 4:0 are in byte 4 and bit 6 is in
                                            byte 1) */
			uint32_t dst_modid_5:1;
					 /**< Destination Module ID: Bit 5 of Dst_ModID (bits 4:0 are in byte 7 and bit 6
                                            is in byte 1) */
			uint32_t l3:1;
					 /**< L3: Indicates that the packet is L3 switched */
			uint32_t label_present:1;
					 /**< Label Present: Indicates that header contains a 20-bit VC label: HiGig+
                                            added field. */
			uint32_t vc_label_16_19:4;
					 /**< VC Label: Bits 19:16 of VC label: HiGig+ added field */
			uint32_t vc_label_0_15:16;
					 /**< VC Label: Bits 15:0 of VC label: HiGig+ added field */
		} o1;
		struct {
			uint32_t classification:16;
					  /**< Classification tag information from the HiGig device FFP */
			uint32_t reserved_0_15:16;

		} o2;
	} dw2;
} cvmx_higig_header_t;

typedef struct {
	union {
		uint32_t u32;
		struct {
			uint32_t k_sop:8;
					  /**< The delimiter indicating the start of a packet transmission */
			uint32_t reserved_21_23:3;
			uint32_t mcst:1;
					  /**< MCST indicates whether the packet should be unicast or
                                            multicast forwarded through the XGS switching fabric
                                            - 0: Unicast
                                            - 1: Mulitcast */
			uint32_t tc:4;
					  /**< Traffic Class [3:0] indicates the distinctive Quality of Service (QoS)
                                            the switching fabric will provide when forwarding the packet
                                            through the fabric */
			uint32_t dst_modid_mgid:8;
					  /**< When MCST=0, this field indicates the destination XGS module to
                                            which the packet will be delivered. When MCST=1, this field indicates
                                            higher order bits of the Multicast Group ID. */
			uint32_t dst_pid_mgid:8;
					  /**< When MCST=0, this field indicates a port associated with the
                                            module indicated by the DST_MODID, through which the packet
                                            will exit the system. When MCST=1, this field indicates lower order
                                            bits of the Multicast Group ID */
		} s;
	} dw0;
	union {
		uint32_t u32;
		struct {
			uint32_t src_modid:8;
					  /**< Source Module ID indicates the source XGS module from which
                                            the packet is originated. (It can also be used for the fabric multicast
                                            load balancing purpose.) */
			uint32_t src_pid:8;
					  /**< Source Port ID indicates a port associated with the module
                                            indicated by the SRC_MODID, through which the packet has
                                            entered the system */
			uint32_t lbid:8;
					  /**< Load Balancing ID indicates a packet flow hashing index
                                            computed by the ingress XGS module for statistical distribution of
                                            packet flows through a multipath fabric */
			uint32_t dp:2;
					  /**< Drop Precedence indicates the traffic rate violation status of the
                                            packet measured by the ingress module.
                                            - 00: GREEN
                                            - 01: RED
                                            - 10: Reserved
                                            - 11: Yellow */
			uint32_t reserved_3_5:3;
			uint32_t ppd_type:3;
					  /**< Packet Processing Descriptor Type
                                            - 000: PPD Overlay1
                                            - 001: PPD Overlay2
                                            - 010~111: Reserved */
		} s;
	} dw1;
	union {
		uint32_t u32;
		struct {
			uint32_t dst_t:1;
					  /**< Destination Trunk: Indicates that the destination port is a member of a trunk
                                            group. */
			uint32_t dst_tgid:3;
					  /**< Destination Trunk Group ID: Trunk group ID of the destination port. The
                                            DO_NOT_LEARN bit is overlaid on the second bit of this field. */
			uint32_t ingress_tagged:1;
					  /**< Ingress Tagged: Indicates whether the packet was tagged when it originally
                                            ingressed the system. */
			uint32_t mirror_only:1;
					  /**< Mirror Only: XGS 1/2 mode: Indicates that the packet was switched and only
                                            needs to be mirrored. */
			uint32_t mirror_done:1;
					  /**< Mirroring Done: XGS1/2 mode: Indicates that the packet was mirrored and
                                            may still need to be switched. */
			uint32_t mirror:1;
					  /**< Mirror: XGS3 mode: a mirror copy packet. XGS1/2 mode: Indicates that the
                                            packet was switched and only needs to be mirrored. */
			uint32_t reserved_22_23:2;
			uint32_t l3:1;
					  /**< L3: Indicates that the packet is L3 switched */
			uint32_t label_present:1;
					  /**< Label Present: Indicates that header contains a 20-bit VC label: HiGig+
                                            added field. */
			uint32_t vc_label:20;
					  /**< Refer to the HiGig+ Architecture Specification */
		} o1;
		struct {
			uint32_t classification:16;
					  /**< Classification tag information from the HiGig device FFP */
			uint32_t reserved_0_15:16;
		} o2;
	} dw2;
	union {
		uint32_t u32;
		struct {
			uint32_t vid:16;
					  /**< VLAN tag information */
			uint32_t pfm:2;
					  /**< Three Port Filtering Modes (0, 1, 2) used in handling registed/unregistered
                                            multicast (unknown L2 multicast and IPMC) packets. This field is used
                                            when OPCODE is 011 or 100 Semantics of PFM bits are as follows;
                                            For registered L2 multicast packets:
                                                PFM= 0 � Flood to VLAN
                                                PFM= 1 or 2 � Send to group members in the L2MC table
                                            For unregistered L2 multicast packets:
                                                PFM= 0 or 1 � Flood to VLAN
                                                PFM= 2 � Drop the packet */
			uint32_t src_t:1;
					  /**< If the MSB of this field is set, then it indicates the LAG the packet ingressed
                                            on, else it represents the physical port the packet ingressed on. */
			uint32_t reserved_11_12:2;
			uint32_t opcode:3;
					  /**< XGS HiGig op-code, indicating the type of packet
                                            000 =     Control frames used for CPU to CPU communications
                                            001 =     Unicast packet with destination resolved; The packet can be
                                                      either Layer 2 unicast packet or L3 unicast packet that was
                                                      routed in the ingress chip.
                                            010 =     Broadcast or unknown Unicast packet or unknown multicast,
                                                      destined to all members of the VLAN
                                            011 =     L2 Multicast packet, destined to all ports of the group indicated
                                                      in the L2MC_INDEX which is overlayed on DST_PORT/DST_MODID fields
                                            100 =     IP Multicast packet, destined to all ports of the group indicated
                                                      in the IPMC_INDEX which is overlayed on DST_PORT/DST_MODID fields
                                            101 =     Reserved
                                            110 =     Reserved
                                            111 =     Reserved */
			uint32_t hdr_ext_len:3;
					  /**< This field is valid only if the HGI field is a b'10' and it indicates the extension
                                            to the standard 12-bytes of XGS HiGig header. Each unit represents 4
                                            bytes, giving a total of 16 additional extension bytes. Value of b'101', b'110'
                                            and b'111' are reserved. For HGI field value of b'01' this field should be
                                            b'01'. For all other values of HGI it is don't care. */
			uint32_t reserved_0_4:5;
		} s;
	} dw3;
} cvmx_higig2_header_t;


static inline void __cvmx_higig_set_qos_priority_o68(void)
{
	int i;
	cvmx_pip_pri_tblx_t pip_pri_tbl;

	for (i = 0; i < 64; i++) {
		pip_pri_tbl.u64 = cvmx_read_csr(CVMX_PIP_PRI_TBLX(i));
		pip_pri_tbl.s.hg2_qos = i & 7;
		cvmx_write_csr(CVMX_PIP_PRI_TBLX(i), pip_pri_tbl.u64);
	}
}

static inline void __cvmx_higig_set_qos_priority_o1(void)
{
	int i;

	for (i = 0; i < 64; i++) {
		cvmx_pip_hg_pri_qos_t pip_hg_pri_qos;
		pip_hg_pri_qos.u64 = 0;
		pip_hg_pri_qos.s.up_qos = 1;
		pip_hg_pri_qos.s.pri = i;
		pip_hg_pri_qos.s.qos = i & 7;
		cvmx_write_csr(CVMX_PIP_HG_PRI_QOS, pip_hg_pri_qos.u64);
	}

}

/*
 * The function below for pre-CN68XX models was derived from
 * a version that combines them with CN68XX, and needs to be
 * reviewed to see on which specific models it may in fact work.
 */
static inline int cvmx_higig_init_o1(int interface, int enable_higig2)
{
	cvmx_pip_prt_cfgx_t pip_prt_cfg;
	cvmx_gmxx_rxx_udd_skp_t gmx_rx_udd_skp;
	cvmx_gmxx_txx_min_pkt_t gmx_tx_min_pkt;
	cvmx_gmxx_txx_append_t gmx_tx_append;
	cvmx_gmxx_tx_ifg_t gmx_tx_ifg;
	cvmx_gmxx_tx_ovr_bp_t gmx_tx_ovr_bp;
	cvmx_gmxx_rxx_frm_ctl_t gmx_rx_frm_ctl;
	cvmx_gmxx_tx_xaui_ctl_t gmx_tx_xaui_ctl;
	int pknd;
	int header_size = (enable_higig2) ? 16 : 12;

	/* Setup PIP to handle HiGig */
	pknd = interface * 16;
	pip_prt_cfg.u64 = cvmx_read_csr(CVMX_PIP_PRT_CFGX(pknd));
	pip_prt_cfg.s.dsa_en = 0;
	pip_prt_cfg.s.higig_en = 1;
	pip_prt_cfg.s.hg_qos = 1;
	pip_prt_cfg.s.skip = header_size;
	cvmx_write_csr(CVMX_PIP_PRT_CFGX(pknd), pip_prt_cfg.u64);

	/* Setup some sample QoS defaults. These can be changed later */
	__cvmx_higig_set_qos_priority_o1();

	/* Setup GMX RX to treat the HiGig header as user data to ignore */
	gmx_rx_udd_skp.u64 = cvmx_read_csr(CVMX_GMXX_RXX_UDD_SKP(0, interface));
	gmx_rx_udd_skp.s.len = header_size;
	gmx_rx_udd_skp.s.fcssel = 0;
	cvmx_write_csr(CVMX_GMXX_RXX_UDD_SKP(0, interface), gmx_rx_udd_skp.u64);

	/* Disable GMX preamble checking */
	gmx_rx_frm_ctl.u64 = cvmx_read_csr(CVMX_GMXX_RXX_FRM_CTL(0, interface));
	gmx_rx_frm_ctl.s.pre_chk = 0;
	cvmx_write_csr(CVMX_GMXX_RXX_FRM_CTL(0, interface), gmx_rx_frm_ctl.u64);

	/* Setup GMX TX to pad properly min sized packets */
	gmx_tx_min_pkt.u64 = cvmx_read_csr(CVMX_GMXX_TXX_MIN_PKT(0, interface));
	gmx_tx_min_pkt.s.min_size = 59 + header_size;
	cvmx_write_csr(CVMX_GMXX_TXX_MIN_PKT(0, interface), gmx_tx_min_pkt.u64);

	/* Setup GMX TX to not add a preamble */
	gmx_tx_append.u64 = cvmx_read_csr(CVMX_GMXX_TXX_APPEND(0, interface));
	gmx_tx_append.s.preamble = 0;
	cvmx_write_csr(CVMX_GMXX_TXX_APPEND(0, interface), gmx_tx_append.u64);

	/* Reduce the inter frame gap to 8 bytes */
	gmx_tx_ifg.u64 = cvmx_read_csr(CVMX_GMXX_TX_IFG(interface));
	gmx_tx_ifg.s.ifg1 = 4;
	gmx_tx_ifg.s.ifg2 = 4;
	cvmx_write_csr(CVMX_GMXX_TX_IFG(interface), gmx_tx_ifg.u64);

	/* Disable GMX backpressure */
	gmx_tx_ovr_bp.u64 = cvmx_read_csr(CVMX_GMXX_TX_OVR_BP(interface));
	gmx_tx_ovr_bp.s.bp = 0;
	gmx_tx_ovr_bp.s.en = 0xf;
	gmx_tx_ovr_bp.s.ign_full = 0xf;
	cvmx_write_csr(CVMX_GMXX_TX_OVR_BP(interface), gmx_tx_ovr_bp.u64);

	if (enable_higig2) {
		/* Enable HiGig2 support and forwarding of virtual port backpressure
		   to PKO */
		cvmx_gmxx_hg2_control_t gmx_hg2_control;
		gmx_hg2_control.u64 = cvmx_read_csr(CVMX_GMXX_HG2_CONTROL(interface));
		gmx_hg2_control.s.hg2rx_en = 1;
		gmx_hg2_control.s.hg2tx_en = 1;
		gmx_hg2_control.s.logl_en = 0xffff;
		gmx_hg2_control.s.phys_en = 1;
		cvmx_write_csr(CVMX_GMXX_HG2_CONTROL(interface), gmx_hg2_control.u64);
	}

	/* Enable HiGig */
	gmx_tx_xaui_ctl.u64 = cvmx_read_csr(CVMX_GMXX_TX_XAUI_CTL(interface));
	gmx_tx_xaui_ctl.s.hg_en = 1;
	cvmx_write_csr(CVMX_GMXX_TX_XAUI_CTL(interface), gmx_tx_xaui_ctl.u64);

	return 0;
}

static inline int cvmx_higig_init_o68(int interface, int enable_higig2)
{
	cvmx_pip_prt_cfgx_t pip_prt_cfg;
	cvmx_gmxx_rxx_udd_skp_t gmx_rx_udd_skp;
	cvmx_gmxx_txx_min_pkt_t gmx_tx_min_pkt;
	cvmx_gmxx_txx_append_t gmx_tx_append;
	cvmx_gmxx_tx_ifg_t gmx_tx_ifg;
	cvmx_gmxx_tx_ovr_bp_t gmx_tx_ovr_bp;
	cvmx_gmxx_rxx_frm_ctl_t gmx_rx_frm_ctl;
	cvmx_gmxx_tx_xaui_ctl_t gmx_tx_xaui_ctl;
	cvmx_pko_reg_read_idx_t pko_index;
	cvmx_pko_mem_iport_ptrs_t pko_mem_iport;
	cvmx_pko_reg_min_pkt_t pko_min_pkt;
	int pknd;
	int header_size = (enable_higig2) ? 16 : 12;

	/* Setup PIP to handle HiGig */
	pknd = cvmx_helper_get_pknd(interface, 0);
	pip_prt_cfg.u64 = cvmx_read_csr(CVMX_PIP_PRT_CFGX(pknd));
	pip_prt_cfg.s.dsa_en = 0;
	pip_prt_cfg.s.higig_en = 1;
	pip_prt_cfg.s.hg_qos = 1;
	pip_prt_cfg.s.skip = header_size;
	cvmx_write_csr(CVMX_PIP_PRT_CFGX(pknd), pip_prt_cfg.u64);

	/* Setup some sample QoS defaults. These can be changed later */
	__cvmx_higig_set_qos_priority_o68();

	/* Setup GMX RX to treat the HiGig header as user data to ignore */
	gmx_rx_udd_skp.u64 = cvmx_read_csr(CVMX_GMXX_RXX_UDD_SKP(0, interface));
	gmx_rx_udd_skp.s.len = header_size;
	gmx_rx_udd_skp.s.fcssel = 0;
	cvmx_write_csr(CVMX_GMXX_RXX_UDD_SKP(0, interface), gmx_rx_udd_skp.u64);

	/* Encounter Higig header in setting min-pkt size */

	pko_index.u64 = 0;
	pko_index.s.index = cvmx_helper_get_pko_port(interface, 0);
	cvmx_write_csr(CVMX_PKO_REG_READ_IDX, pko_index.u64);

	/* Use size2 for higig and size3 for higig2 */
	pko_mem_iport.u64 = cvmx_read_csr(CVMX_PKO_MEM_IPORT_PTRS);
	pko_mem_iport.s.min_pkt = (enable_higig2) ? 2 : 3;
	cvmx_write_csr(CVMX_PKO_MEM_IPORT_PTRS, pko_mem_iport.u64);

	pko_min_pkt.u64 = cvmx_read_csr(CVMX_PKO_REG_MIN_PKT);
	if (enable_higig2)
		pko_min_pkt.s.size2 = 59 + header_size;
	else
		pko_min_pkt.s.size3 = 59 + header_size;
	cvmx_write_csr(CVMX_PKO_REG_MIN_PKT, pko_min_pkt.u64);

	/* Disable GMX preamble checking */
	gmx_rx_frm_ctl.u64 = cvmx_read_csr(CVMX_GMXX_RXX_FRM_CTL(0, interface));
	gmx_rx_frm_ctl.s.pre_chk = 0;
	cvmx_write_csr(CVMX_GMXX_RXX_FRM_CTL(0, interface), gmx_rx_frm_ctl.u64);

	/* Setup GMX TX to pad properly min sized packets */
	gmx_tx_min_pkt.u64 = cvmx_read_csr(CVMX_GMXX_TXX_MIN_PKT(0, interface));
	gmx_tx_min_pkt.s.min_size = 59 + header_size;
	cvmx_write_csr(CVMX_GMXX_TXX_MIN_PKT(0, interface), gmx_tx_min_pkt.u64);

	/* Setup GMX TX to not add a preamble */
	gmx_tx_append.u64 = cvmx_read_csr(CVMX_GMXX_TXX_APPEND(0, interface));
	gmx_tx_append.s.preamble = 0;
	cvmx_write_csr(CVMX_GMXX_TXX_APPEND(0, interface), gmx_tx_append.u64);

	/* Reduce the inter frame gap to 8 bytes */
	gmx_tx_ifg.u64 = cvmx_read_csr(CVMX_GMXX_TX_IFG(interface));
	gmx_tx_ifg.s.ifg1 = 4;
	gmx_tx_ifg.s.ifg2 = 4;
	cvmx_write_csr(CVMX_GMXX_TX_IFG(interface), gmx_tx_ifg.u64);

	/* Disable GMX backpressure */
	gmx_tx_ovr_bp.u64 = cvmx_read_csr(CVMX_GMXX_TX_OVR_BP(interface));
	gmx_tx_ovr_bp.s.bp = 0;
	gmx_tx_ovr_bp.s.en = 0xf;
	gmx_tx_ovr_bp.s.ign_full = 0xf;
	cvmx_write_csr(CVMX_GMXX_TX_OVR_BP(interface), gmx_tx_ovr_bp.u64);

	if (enable_higig2) {
		/* Enable HiGig2 support and forwarding of virtual port backpressure
		   to PKO */
		cvmx_gmxx_hg2_control_t gmx_hg2_control;
		gmx_hg2_control.u64 = cvmx_read_csr(CVMX_GMXX_HG2_CONTROL(interface));
		gmx_hg2_control.s.hg2rx_en = 1;
		gmx_hg2_control.s.hg2tx_en = 1;
		gmx_hg2_control.s.logl_en = 0xffff;
		gmx_hg2_control.s.phys_en = 1;
		cvmx_write_csr(CVMX_GMXX_HG2_CONTROL(interface), gmx_hg2_control.u64);
	}

	/* Enable HiGig */
	gmx_tx_xaui_ctl.u64 = cvmx_read_csr(CVMX_GMXX_TX_XAUI_CTL(interface));
	gmx_tx_xaui_ctl.s.hg_en = 1;
	cvmx_write_csr(CVMX_GMXX_TX_XAUI_CTL(interface), gmx_tx_xaui_ctl.u64);

	return 0;
}

/*
 * Install a HiGig/HiGig2 PCAM entry that skips over
 * the header before parsing Layer-2 header for every
 * port configured for HiGig* support.
 */
static int __cvmx_pki_pcam_higig(int node, int style, int hsz)
{
	struct cvmx_pki_pcam_input pcam_input;
	struct cvmx_pki_pcam_action pcam_action;
	enum cvmx_pki_term field;
	int index;
	int bank;
	uint64_t cl_mask = CVMX_PKI_CLUSTER_ALL;

	memset(&pcam_input, 0, sizeof(pcam_input));
	memset(&pcam_action, 0, sizeof(pcam_action));

	field = CVMX_PKI_PCAM_TERM_HIGIG;
	bank = field & 0x01;

	index = cvmx_pki_pcam_entry_alloc(node,
		CVMX_PKI_FIND_AVAL_ENTRY, bank, cl_mask);
	if (index < 0) {
		cvmx_dprintf("ERROR: Allocating pcam entry node=%d bank=%d\n",
			node, bank);
		return -1;
	}

	pcam_input.style = style;
	pcam_input.style_mask = 0xff;
	pcam_input.field = field;
	pcam_input.field_mask = 0xff;
	pcam_input.data = 0;
	pcam_input.data_mask = 0;
	pcam_action.parse_mode_chg = CVMX_PKI_PARSE_NO_CHG;
	pcam_action.layer_type_set = CVMX_PKI_LTYPE_E_NONE;
	pcam_action.style_add = 0;
	pcam_action.pointer_advance = hsz;

	cvmx_pki_pcam_write_entry(node, index, cl_mask,
			pcam_input, pcam_action);
	return 0;
}

static inline int cvmx_higig_init_o3(int xiface, int port, int enable_higig2)
{
	int header_size = (enable_higig2) ? 16 : 12;
	struct cvmx_pki_global_config gcfg;
        struct cvmx_pki_pkind_config pcfg;
	int pkind;
	cvmx_bgxx_cmrx_config_t cmr_config;
	cvmx_bgxx_smux_rx_udd_skp_t udd_skip;
	cvmx_bgxx_spux_misc_control_t misc_ctl;
	cvmx_bgxx_smux_hg2_control_t hg2_control;
	cvmx_bgxx_smux_tx_ifg_t ifg;
	cvmx_bgxx_smux_rx_frm_ctl_t frm_ctl;
	cvmx_bgxx_smux_tx_ctl_t tx_ctl;
	cvmx_bgxx_smux_tx_append_t txa;
	cvmx_xiface_t xi = cvmx_helper_xiface_to_node_interface(xiface);
	int node = xi.node;
	int interface = xi.interface;

	/* Verify that the interface belongs to BGX */
	if (interface >= CVMX_HELPER_MAX_GMX)
		return -1;

        cmr_config.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_CMRX_CONFIG(port, interface));

	/* HiGig is not supported for SGMII/XCV interfaces */
	if ((cmr_config.s.lmac_type == 0) || (cmr_config.s.lmac_type == 5))
	    return -2;

	/*
	 * NOTE: Code below assumes each BGX interface has
	 * a unique PKIND, and a unuque initial STYLE.
	 */

	/* Setup PKI to handle HiGig: Global and PKIND */
	cvmx_pki_read_global_config(node, &gcfg);
	gcfg.gbl_pen.hg_pen = 1;
	cvmx_pki_write_global_config(node, &gcfg);

	pkind = cvmx_helper_get_pknd(xiface, port);
	cvmx_pki_read_pkind_config(node, pkind, &pcfg);
        pcfg.parse_en.hg_en = 1;
        pcfg.parse_en.hg2_en = (enable_higig2)? 1: 0;
	cvmx_pki_write_pkind_config(node, pkind, &pcfg);

	/* PKI: Install a PCAM entry to skip HiGiG/HiGiG2 header in parser */
	__cvmx_pki_pcam_higig(node, pcfg.initial_style, header_size);

	/* BGX: Reduce Inter-frame gap from 12 to 8 bytes */
	ifg.u64 = 0;
	ifg.s.ifg1 = 4;
	ifg.s.ifg2 = 4;
	cvmx_write_csr_node(node, CVMX_BGXX_SMUX_TX_IFG(port, interface), ifg.u64);

	/* BGX: Configure user-data length to HiGig2 header size */
	udd_skip.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SMUX_RX_UDD_SKP(port, interface));
	udd_skip.s.fcssel = 0;
	udd_skip.s.len = header_size;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SMUX_RX_UDD_SKP(port, interface), udd_skip.u64);

	/* BGX: Enable more idle skips for small IFG (SMU only) */
	misc_ctl.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SPUX_MISC_CONTROL(port, interface));
	misc_ctl.s.skip_after_term = 1;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SPUX_MISC_CONTROL(port, interface), misc_ctl.u64);

	/* BGX HiGig2 control (SMU only) */
	hg2_control.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SMUX_HG2_CONTROL(port, interface));
	hg2_control.s.hg2tx_en = (enable_higig2) ? 1 : 0;
	hg2_control.s.hg2rx_en = (enable_higig2) ? 1 : 0;
	hg2_control.s.phys_en = (enable_higig2) ? 1 : 0;
	hg2_control.s.logl_en = (enable_higig2) ? 1 : 0;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SMUX_HG2_CONTROL(port, interface), hg2_control.u64);

	/* BGX RX Control - Disable preamble checks */
	frm_ctl.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SMUX_RX_FRM_CTL(port, interface));
	frm_ctl.s.pre_chk = 0;
	frm_ctl.s.pre_strp = 0;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SMUX_RX_FRM_CTL(port, interface), frm_ctl.u64);

	/* BGX TX Control */
	tx_ctl.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SMUX_TX_CTL(port, interface));
	tx_ctl.s.hg_en = 1;
	tx_ctl.s.hg_pause_hgi = (enable_higig2)? 1: 0;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SMUX_TX_CTL(port, interface), tx_ctl.u64);

	txa.u64 = cvmx_read_csr_node(node,
		CVMX_BGXX_SMUX_TX_APPEND(port, interface));
	txa.s.preamble = 0;
	cvmx_write_csr_node(node,
		CVMX_BGXX_SMUX_TX_APPEND(port, interface), txa.u64);

	/* Disable standard flow control for HiGig2 */
	if (enable_higig2) {
		cvmx_bgxx_smux_cbfc_ctl_t cbfc_ctl;
		cbfc_ctl.u64 = cvmx_read_csr_node(node,
			CVMX_BGXX_SMUX_CBFC_CTL(port, interface));
		cbfc_ctl.s.rx_en = 0;
		cbfc_ctl.s.tx_en = 0;
		cvmx_write_csr_node(node,
			CVMX_BGXX_SMUX_CBFC_CTL(port, interface),
			cbfc_ctl.u64);
	}

	/* Configure PKO */

	/*
	 * NOTE:
	 * PKO3 has only a global min packet register,
	 * which is not appropriate for HiGig.
	 */

	/*
	 * FIXME:
	 * HiGig QoS not implemented yet, may require PKI microcode fix.
	 */
	return 0;
}

/**
 * Initialize the HiGig aspects of a XAUI interface. This function
 * should be called before the cvmx-helper generic init.
 *
 * @param interface Interface that this operation applies to
 * @param port is the interface port the operation applies to
 * @param enable_higig2
 *                  Non zero to enable HiGig2 support. Zero to support HiGig
 *                  and HiGig+.
 *
 * @return Zero on success, negative on failure
 *
 * NOTE:
 * This function name and signature has changed from previous releases
 * to support Octeon-3, where each interface may have up to 4 ports,
 * and each may be individually enabled to accept the HiGig/HiGig2 custom
 * frame format.
 */
static inline int cvmx_higig_init(int interface, int port, int enable_higig2)
{

	if (octeon_has_feature(OCTEON_FEATURE_PKI)) {
		return cvmx_higig_init_o3(interface, port, enable_higig2);
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		cvmx_warn_if(port != 0, "Interface port range exceeded");
		return cvmx_higig_init_o68(interface, enable_higig2);
	} else {
		cvmx_warn_if(port != 0, "Interface port range exceeded");
		return cvmx_higig_init_o1(interface, enable_higig2);
	}

}

#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif //  __CVMX_HIGIG_H__

