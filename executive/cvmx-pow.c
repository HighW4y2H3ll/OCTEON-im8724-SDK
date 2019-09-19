/***********************license start***************
 * Copyright (c) 2003-2017  Cavium Inc. (support@cavium.com). All rights
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
 * Interface to the hardware Packet Order / Work unit.
 *
 * <hr>$Revision: 29727 $<hr>
 */

#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-pow.h"
#include "cvmx-fpa.h"
#include "cvmx-ipd.h"
#include "cvmx-bootmem.h"
#include "cvmx-helper-cfg.h"
#include "cvmx-helper-fpa.h"

static const int debug = 0;

/**
 * @INTERNAL
 * This structure stores the internal POW state captured by
 * cvmx_pow_capture(). It is purposely not exposed to the user
 * since the format may change without notice.
 */
typedef struct {
	cvmx_pow_tag_load_resp_t sstatus[CVMX_MAX_CORES][8];
	cvmx_pow_tag_load_resp_t smemload[2048][8];
	cvmx_pow_tag_load_resp_t sindexload[64][8];
} __cvmx_pow_dump_t;

typedef enum {
	CVMX_POW_LIST_UNKNOWN,
	CVMX_POW_LIST_FREE,
	CVMX_POW_LIST_INPUT,
	CVMX_POW_LIST_CORE,
	CVMX_POW_LIST_DESCHED,
	CVMX_POW_LIST_NOSCHED,
	CVMX_POW_LIST_CONF
} __cvmx_pow_list_types_t;

typedef struct __cvmx_entry_list_s {
	uint8_t name_idx;
	int num_idx;
} __cvmx_entry_list_t;

/* Define structure to dump data for the CN7XXX targets. Since 78XX chip
 * has more queues, cores and entries the values for this chip will be
 * used. */
typedef struct {
	uint64_t n_cores, n_model, n_node;
        cvmx_pow_tag_load_resp_t sso_core_state[CVMX_MAX_CORES][6];
	uint64_t sso_ppx_grpmsk[CVMX_MAX_CORES][2][4];
	cvmx_pow_tag_load_resp_t sso_ient[4096][5];
	uint64_t ipl_free[5];
	uint64_t sso_grps[256][5];
	uint64_t sso_xaq[256][5];
	uint64_t sso_iaq[256];
	uint64_t sso_desched[256];
	uint64_t sso_conf[256];
} __cvmx_sso_dump_7xxx_t;

static const char *__cvmx_pow_list_names[] = {
	"Unknown",
	"Free List",
	"Queue",
	"Core",
	"Desched",
	"Nosched",
	"Conflict"
};

static char *oct_tag_type_string_v3[] = {
	"ORDERED",
	"ATOMIC",
	"UNTAGGED",
	"EMPTY"
};

/**
 * Return the number of POW entries supported by this chip
 *
 * @return Number of POW entries
 */
int cvmx_pow_get_num_entries(void)
{
	if (OCTEON_IS_MODEL(OCTEON_CN30XX))
		return 64;
	else if (OCTEON_IS_MODEL(OCTEON_CN31XX) || OCTEON_IS_MODEL(OCTEON_CN50XX))
		return 256;
	else if (OCTEON_IS_MODEL(OCTEON_CN52XX)
		 || OCTEON_IS_MODEL(OCTEON_CN61XX)
		 || OCTEON_IS_MODEL(OCTEON_CNF71XX)
		 || OCTEON_IS_MODEL(OCTEON_CN70XX))
		return 512;
	else if (OCTEON_IS_MODEL(OCTEON_CN63XX) || OCTEON_IS_MODEL(OCTEON_CN66XX))
		return 1024;
	else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		return 2048;
	else if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		return 4096;
	else if (OCTEON_IS_MODEL(OCTEON_CN73XX)
		 || OCTEON_IS_MODEL(OCTEON_CNF75XX))
		return 1024;
	else
		return 512;	/* conservative default */
}

/**
 * Return the number of queues used by this chip. Right now is used only
 * for the CN7XXX targets.
 *
 * @return Number of queues
 */
static int cvmx_get_num_queues()
{
	if (OCTEON_IS_MODEL(OCTEON_CN73XX) || OCTEON_IS_MODEL(OCTEON_CNF75XX))
		return 64;
	else if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		return 256;
	else
		return 0;
}

int cvmx_pow_get_dump_size(void)
{
	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		return sizeof(__cvmx_sso_dump_7xxx_t);
	return sizeof(__cvmx_pow_dump_t);
}

static int __cvmx_pow_capture_v1(void *buffer, int buffer_size)
{
	__cvmx_pow_dump_t *dump = (__cvmx_pow_dump_t *) buffer;
	int num_cores;
	int num_pow_entries = cvmx_pow_get_num_entries();
	int core;
	int index;
	int bits;

	if (buffer_size < (int)sizeof(__cvmx_pow_dump_t)) {
		cvmx_dprintf("cvmx_pow_capture: Buffer too small. " "At least %d bytes are needed.\n", (int)sizeof(__cvmx_pow_dump_t));
		return -1;
	}

	num_cores = cvmx_octeon_num_cores();

	/* Read all core related state */
	for (core = 0; core < num_cores; core++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.sstatus.mem_region = CVMX_IO_SEG;
		load_addr.sstatus.is_io = 1;
		load_addr.sstatus.did = CVMX_OCT_DID_TAG_TAG1;
		load_addr.sstatus.coreid = core;
		for (bits = 0; bits < 8; bits++) {
			load_addr.sstatus.get_rev = (bits & 1) != 0;
			load_addr.sstatus.get_cur = (bits & 2) != 0;
			load_addr.sstatus.get_wqp = (bits & 4) != 0;
			if ((load_addr.sstatus.get_cur == 0) && load_addr.sstatus.get_rev)
				dump->sstatus[core][bits].u64 = -1;
			else
				dump->sstatus[core][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}

	/* Read all internal POW entries */
	for (index = 0; index < num_pow_entries; index++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.smemload.mem_region = CVMX_IO_SEG;
		load_addr.smemload.is_io = 1;
		load_addr.smemload.did = CVMX_OCT_DID_TAG_TAG2;
		load_addr.smemload.index = index;
		for (bits = 0; bits < 3; bits++) {
			load_addr.smemload.get_des = (bits & 1) != 0;
			load_addr.smemload.get_wqp = (bits & 2) != 0;
			dump->smemload[index][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}

	/* Read all group and queue pointers */
	for (index = 0; index < 16; index++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.sindexload.mem_region = CVMX_IO_SEG;
		load_addr.sindexload.is_io = 1;
		load_addr.sindexload.did = CVMX_OCT_DID_TAG_TAG3;
		load_addr.sindexload.qosgrp = index;
		for (bits = 0; bits < 4; bits++) {
			load_addr.sindexload.get_rmt = (bits & 1) != 0;
			load_addr.sindexload.get_des_get_tail = (bits & 2) != 0;
			/* The first pass only has 8 valid index values */
			if ((load_addr.sindexload.get_rmt == 0) && (load_addr.sindexload.get_des_get_tail == 0) && (index >= 8))
				dump->sindexload[index][bits].u64 = -1;
			else
				dump->sindexload[index][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}
	return 0;
}

static int __cvmx_pow_capture_v2(void *buffer, int buffer_size)
{
	__cvmx_pow_dump_t *dump = (__cvmx_pow_dump_t *) buffer;
	int num_cores;
	int num_pow_entries = cvmx_pow_get_num_entries() >> 1;
	int core;
	int index;
	int bits;

	if (buffer_size < (int)sizeof(__cvmx_pow_dump_t)) {
		cvmx_dprintf("cvmx_pow_capture: Buffer too small. " "At least %d bytes are needed.\n", (int)sizeof(__cvmx_pow_dump_t));
		return -1;
	}

	num_cores = cvmx_octeon_num_cores();

	/* Read all core related state */
	for (core = 0; core < num_cores; core++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.sstatus_cn68xx.mem_region = CVMX_IO_SEG;
		load_addr.sstatus_cn68xx.is_io = 1;
		load_addr.sstatus_cn68xx.did = CVMX_OCT_DID_TAG_TAG5;
		load_addr.sstatus_cn68xx.coreid = core;
		for (bits = 1; bits < 6; bits++) {
			load_addr.sstatus_cn68xx.opcode = bits;
			dump->sstatus[core][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}
	/* Read all internal POW entries */
	for (index = 0; index < num_pow_entries; index++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.smemload_cn68xx.mem_region = CVMX_IO_SEG;
		load_addr.smemload_cn68xx.is_io = 1;
		load_addr.smemload_cn68xx.did = CVMX_OCT_DID_TAG_TAG2;
		load_addr.smemload_cn68xx.index = index;
		for (bits = 1; bits < 5; bits++) {
			load_addr.smemload_cn68xx.opcode = bits;
			dump->smemload[index][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}

	/* Read all group and queue pointers */
	for (index = 0; index < 64; index++) {
		cvmx_pow_load_addr_t load_addr;
		load_addr.u64 = 0;
		load_addr.sindexload_cn68xx.mem_region = CVMX_IO_SEG;
		load_addr.sindexload_cn68xx.is_io = 1;
		load_addr.sindexload_cn68xx.did = CVMX_OCT_DID_TAG_TAG3;
		for (bits = 1; bits < 7; bits++) {
			/* When opcode is IPL_IQ, qos_grp specifies IQ (or QOS). */
			if (bits == 1)
				load_addr.sindexload_cn68xx.qos_grp = index & 0x7;
			/* When opcode is IPL_DESCHED, qos_grp specifies group. */
			else if (bits == 2)
				load_addr.sindexload_cn68xx.qos_grp = index;
			/* For all other opcodes qos_grp is reserved. */
			else
				load_addr.sindexload_cn68xx.qos_grp = 0;
			load_addr.sindexload_cn68xx.opcode = bits;
			dump->sindexload[index][bits].u64 = cvmx_read_csr(load_addr.u64);
		}
	}
	return 0;
}

static int __cvmx_sso_capture_v3(void *buffer, int buffer_size)
{
	__cvmx_sso_dump_7xxx_t *dump = (__cvmx_sso_dump_7xxx_t *) buffer;
	int num_cores;
	int num_pow_entries;
	int num_queues;
	int core;
	int index;
	int node;

	if (buffer_size < cvmx_pow_get_dump_size()) {
		cvmx_dprintf("cvmx_pow_capture: Buffer too small. " "At least %d bytes are needed.\n", (int)sizeof(__cvmx_sso_dump_7xxx_t));
		return -1;
	}

	num_cores = cvmx_octeon_num_cores();
	if (num_cores > CVMX_MAX_CORES) {
		cvmx_dprintf("cvmx_pow_capture: Number of cores (%d) is more than can be dumped (%d). Only first %d cores will be dumped.\n",
				num_cores, CVMX_MAX_CORES, CVMX_MAX_CORES);
		num_cores = CVMX_MAX_CORES;
	}
	dump->n_cores = num_cores;
	dump->n_node = cvmx_get_node_num();
	node = dump->n_node;

	/* Read all core related state */
	for (core = 0; core < num_cores; core++) {
		dump->sso_core_state[core][1].u64 = cvmx_read_csr_node(node, CVMX_SSO_SL_PPX_PENDTAG(core));
		dump->sso_core_state[core][2].u64 = cvmx_read_csr_node(node, CVMX_SSO_SL_PPX_PENDWQP(core));
		dump->sso_core_state[core][3].u64 = cvmx_read_csr_node(node, CVMX_SSO_SL_PPX_TAG(core));
		dump->sso_core_state[core][4].u64 = cvmx_read_csr_node(node, CVMX_SSO_SL_PPX_WQP(core));
		dump->sso_core_state[core][5].u64 = cvmx_read_csr_node(node, CVMX_SSO_SL_PPX_LINKS(core));
	}

	for (index = 0; index < 5; index++) {
		dump->ipl_free[index] = cvmx_read_csr_node(node, CVMX_SSO_IPL_FREEX(index));
	}

	num_queues = cvmx_get_num_queues();
	for (index = 0; index < num_queues; index++) {
		dump->sso_iaq[index] = cvmx_read_csr_node(dump->n_node, CVMX_SSO_IPL_IAQX(index));
		dump->sso_desched[index] = cvmx_read_csr_node(dump->n_node, CVMX_SSO_IPL_DESCHEDX(index));
		dump->sso_conf[index] = cvmx_read_csr_node(dump->n_node, CVMX_SSO_IPL_CONFX(index));
	}

	num_pow_entries = cvmx_pow_get_num_entries();
	for (index = 0; index < num_pow_entries; index++) {
		dump->sso_ient[index][0].u64 = cvmx_read_csr_node(node, CVMX_SSO_IENTX_TAG(index));
		dump->sso_ient[index][1].u64 = cvmx_read_csr_node(node, CVMX_SSO_IENTX_WQPGRP(index));
		dump->sso_ient[index][2].u64 = cvmx_read_csr_node(node, CVMX_SSO_IENTX_PENDTAG(index));
		dump->sso_ient[index][3].u64 = cvmx_read_csr_node(node, CVMX_SSO_IENTX_LINKS(index));
		dump->sso_ient[index][4].u64 = cvmx_read_csr_node(node, CVMX_SSO_IENTX_QLINKS(index));
	}

	return -1;
}

/**
 * Store the current POW internal state into the supplied
 * buffer. It is recommended that you pass a buffer of at least
 * 128KB. The format of the capture may change based on SDK
 * version and Octeon chip.
 *
 * @param buffer Buffer to store capture into
 * @param buffer_size
 *               The size of the supplied buffer
 *
 * @return Zero on sucess, negative on failure
 */
int cvmx_pow_capture(void *buffer, int buffer_size)
{
	if (buffer_size < cvmx_pow_get_dump_size()) {
		cvmx_dprintf("cvmx_pow_capture: Buffer too small. " "At least %d bytes are needed.\n",
			     cvmx_pow_get_dump_size());
		return -1;
	}
	/* This code is used from host/remote-utils/oct-remote-pow.c */
	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		return __cvmx_sso_capture_v3(buffer, buffer_size);
	if (octeon_has_feature(OCTEON_FEATURE_CN68XX_WQE))
		return __cvmx_pow_capture_v2(buffer, buffer_size);
	else
		return __cvmx_pow_capture_v1(buffer, buffer_size);
}

/**
 * Function to display a POW internal queue to the user
 *
 * @param name       User visible name for the queue
 * @param name_param Parameter for printf in creating the name
 * @param valid      Set if the queue contains any elements
 * @param has_one    Set if the queue contains exactly one element
 * @param head       The head pointer
 * @param tail       The tail pointer
 */
static void __cvmx_pow_display_list(const char *name, int name_param, int valid, int has_one, uint64_t head, uint64_t tail)
{
	cvmx_dprintf("%s", name);
	if (name_param >= 0)
		cvmx_dprintf(" %d", name_param);
	cvmx_dprintf(": ");
	cvmx_dprintf("head=%llu(0x%llx) tail=%llu(0x%llx)", CAST64(head), CAST64(head), CAST64(tail), CAST64(tail));
	if (valid) {
		if (has_one)
			cvmx_dprintf(" One element");
		else
			cvmx_dprintf(" Multiple elements");
	} else
		cvmx_dprintf(" Empty");
	cvmx_dprintf("\n");
}

/**
 * Mark which list a POW entry is on. Print a warning message if the
 * entry is already on a list. This happens if the POW changed while
 * the capture was running.
 *
 * @param entry_num  Entry number to mark
 * @param entry_type List type
 * @param index      Index for the entry type
 * @param entry_list Array to store marks
 *
 * @return 0 on success, -1 if already on a list
 */
static int __cvmx_pow_entry_mark_list(int entry_num, __cvmx_pow_list_types_t entry_type,
		int index, struct __cvmx_entry_list_s entry_list[])
{
	if (entry_list[entry_num].name_idx != 0) {
		cvmx_dprintf("\nWARNING: Entry %d already on list %s",
				entry_num,
				__cvmx_pow_list_names[entry_list[entry_num].name_idx]);
		if (entry_list[entry_num].num_idx >= 0)
			cvmx_dprintf(" %d", entry_list[entry_num].num_idx);
		cvmx_dprintf(", but we tried to add it to %s\n",
				__cvmx_pow_list_names[entry_type]);
		return -1;
	}

	entry_list[entry_num].name_idx = entry_type;
	entry_list[entry_num].num_idx = index;

	return 0;
}

/**
 * Display a list and mark all elements on the list as belonging to
 * the list.
 *
 * @param entry_type Type of the list to display and mark
 * @param index      Index for the entry type
 * @param dump       POW capture data
 * @param entry_list Array to store marks in
 * @param valid      Set if the queue contains any elements
 * @param has_one    Set if the queue contains exactly one element
 * @param head       The head pointer
 * @param tail       The tail pointer
 */
static void __cvmx_pow_display_list_and_walk(__cvmx_pow_list_types_t entry_type, int index,
		__cvmx_pow_dump_t * dump, struct __cvmx_entry_list_s entry_list[], int valid,
		int has_one, uint64_t head, uint64_t tail)
{
	__cvmx_pow_display_list(__cvmx_pow_list_names[entry_type], index, valid, has_one, head, tail);
	if (!valid)
		return;
	if (has_one) {
		__cvmx_pow_entry_mark_list(head, entry_type, index, entry_list);
		return;
	}

	while (head != tail) {
		if (__cvmx_pow_entry_mark_list(head, entry_type, index, entry_list))
			break;
		/* The OCTEON_FEATURE_CN78XX_WQE feature should be checked
		 * first, because cn7xxx chips containe the
		 * OCTEON_FEATURE_PKND feature. */
		if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
			__cvmx_sso_dump_7xxx_t *dump7x;

			dump7x = (__cvmx_sso_dump_7xxx_t*)dump;
			if (OCTEON_IS_MODEL(OCTEON_CN78XX))
				head = dump7x->sso_ient[head][3].s_sso_ientx_links_cn78xx.prev_index;
			else
				head = dump7x->sso_ient[head][3].s_sso_ientx_links_cn73xx.prev_index;
		} else if (octeon_has_feature(OCTEON_FEATURE_PKND)) {
			if (entry_type >= CVMX_POW_LIST_INPUT && entry_type < CVMX_POW_LIST_CORE)

				head = dump->smemload[head][4].s_smemload3_cn68xx.next_index;
			else
				head = dump->smemload[head][4].s_smemload3_cn68xx.fwd_index;
		} else
			head = dump->smemload[head][0].s_smemload0.next_index;
	}
	__cvmx_pow_entry_mark_list(tail, entry_type, index, entry_list);
}

void __cvmx_pow_display_v1(void *buffer, int buffer_size)
{
	__cvmx_pow_dump_t *dump = (__cvmx_pow_dump_t *) buffer;
	int num_pow_entries = cvmx_pow_get_num_entries();
	int num_cores;
	int core;
	int index;
	struct __cvmx_entry_list_s entry_list[2048];

	if (buffer_size < (int)sizeof(__cvmx_pow_dump_t)) {
		cvmx_dprintf("cvmx_pow_dump: Buffer too small\n");
		return;
	}

	for (index = 0; index < 2048; index++) {
		entry_list[index].name_idx = 0;
		entry_list[index].num_idx = -1;
	}
	num_cores = cvmx_octeon_num_cores();

	/* Print the free list info */
	__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_FREE, -1, dump, entry_list,
					 dump->sindexload[0][0].sindexload0.free_val,
					 dump->sindexload[0][0].sindexload0.free_one, dump->sindexload[0][0].sindexload0.free_head, dump->sindexload[0][0].sindexload0.free_tail);

	/* Print the core state */
	for (core = 0; core < num_cores; core++) {
		const int bit_rev = 1;
		const int bit_cur = 2;
		const int bit_wqp = 4;
		cvmx_dprintf("Core %d State:  tag=%s,0x%08x", core, OCT_TAG_TYPE_STRING(dump->sstatus[core][bit_cur].s_sstatus2.tag_type), dump->sstatus[core][bit_cur].s_sstatus2.tag);
		if (dump->sstatus[core][bit_cur].s_sstatus2.tag_type != CVMX_POW_TAG_TYPE_NULL_NULL) {
			__cvmx_pow_entry_mark_list(dump->sstatus[core][bit_cur].s_sstatus2.index, CVMX_POW_LIST_CORE, core, entry_list);
			cvmx_dprintf(" grp=%d", dump->sstatus[core][bit_cur].s_sstatus2.grp);
			cvmx_dprintf(" wqp=0x%016llx", CAST64(dump->sstatus[core][bit_cur | bit_wqp].s_sstatus4.wqp));
			cvmx_dprintf(" index=%d", dump->sstatus[core][bit_cur].s_sstatus2.index);
			if (dump->sstatus[core][bit_cur].s_sstatus2.head)
				cvmx_dprintf(" head");
			else
				cvmx_dprintf(" prev=%d", dump->sstatus[core][bit_cur | bit_rev].s_sstatus3.revlink_index);
			if (dump->sstatus[core][bit_cur].s_sstatus2.tail)
				cvmx_dprintf(" tail");
			else
				cvmx_dprintf(" next=%d", dump->sstatus[core][bit_cur].s_sstatus2.link_index);
		}

		if (dump->sstatus[core][0].s_sstatus0.pend_switch) {
			cvmx_dprintf(" pend_switch=%d", dump->sstatus[core][0].s_sstatus0.pend_switch);
			cvmx_dprintf(" pend_switch_full=%d", dump->sstatus[core][0].s_sstatus0.pend_switch_full);
			cvmx_dprintf(" pend_switch_null=%d", dump->sstatus[core][0].s_sstatus0.pend_switch_null);
		}

		if (dump->sstatus[core][0].s_sstatus0.pend_desched) {
			cvmx_dprintf(" pend_desched=%d", dump->sstatus[core][0].s_sstatus0.pend_desched);
			cvmx_dprintf(" pend_desched_switch=%d", dump->sstatus[core][0].s_sstatus0.pend_desched_switch);
			cvmx_dprintf(" pend_nosched=%d", dump->sstatus[core][0].s_sstatus0.pend_nosched);
			if (dump->sstatus[core][0].s_sstatus0.pend_desched_switch)
				cvmx_dprintf(" pend_grp=%d", dump->sstatus[core][0].s_sstatus0.pend_grp);
		}

		if (dump->sstatus[core][0].s_sstatus0.pend_new_work) {
			if (dump->sstatus[core][0].s_sstatus0.pend_new_work_wait)
				cvmx_dprintf(" (Waiting for work)");
			else
				cvmx_dprintf(" (Getting work)");
		}
		if (dump->sstatus[core][0].s_sstatus0.pend_null_rd)
			cvmx_dprintf(" pend_null_rd=%d", dump->sstatus[core][0].s_sstatus0.pend_null_rd);
		if (dump->sstatus[core][0].s_sstatus0.pend_nosched_clr) {
			cvmx_dprintf(" pend_nosched_clr=%d", dump->sstatus[core][0].s_sstatus0.pend_nosched_clr);
			cvmx_dprintf(" pend_index=%d", dump->sstatus[core][0].s_sstatus0.pend_index);
		}
		if (dump->sstatus[core][0].s_sstatus0.pend_switch || (dump->sstatus[core][0].s_sstatus0.pend_desched && dump->sstatus[core][0].s_sstatus0.pend_desched_switch)) {
			cvmx_dprintf(" pending tag=%s,0x%08x", OCT_TAG_TYPE_STRING(dump->sstatus[core][0].s_sstatus0.pend_type), dump->sstatus[core][0].s_sstatus0.pend_tag);
		}
		if (dump->sstatus[core][0].s_sstatus0.pend_nosched_clr)
			cvmx_dprintf(" pend_wqp=0x%016llx\n", CAST64(dump->sstatus[core][bit_wqp].s_sstatus1.pend_wqp));
		cvmx_dprintf("\n");
	}

	/* Print out the state of the nosched list and the 16 deschedule lists. */
	__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_NOSCHED, 0, dump, entry_list,
					 dump->sindexload[0][2].sindexload1.nosched_val,
					 dump->sindexload[0][2].sindexload1.nosched_one,
					 dump->sindexload[0][2].sindexload1.nosched_head, dump->sindexload[0][2].sindexload1.nosched_tail);
	for (index = 0; index < 16; index++) {
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_DESCHED, index, dump, entry_list,
						 dump->sindexload[index][2].sindexload1.des_val,
						 dump->sindexload[index][2].sindexload1.des_one,
						 dump->sindexload[index][2].sindexload1.des_head, dump->sindexload[index][2].sindexload1.des_tail);
	}

	/* Print out the state of the 8 internal input queues */
	for (index = 0; index < 8; index++) {
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_INPUT, index, dump, entry_list,
						 dump->sindexload[index][0].sindexload0.loc_val,
						 dump->sindexload[index][0].sindexload0.loc_one,
						 dump->sindexload[index][0].sindexload0.loc_head, dump->sindexload[index][0].sindexload0.loc_tail);
	}

	/* Print out the state of the 16 memory queues */
	for (index = 0; index < 8; index++) {
		const char *name;
		if (dump->sindexload[index][1].sindexload2.rmt_is_head)
			name = "Queue %da Memory (is head)";
		else
			name = "Queue %da Memory";
		__cvmx_pow_display_list(name, index,
					dump->sindexload[index][1].sindexload2.rmt_val,
					dump->sindexload[index][1].sindexload2.rmt_one,
					dump->sindexload[index][1].sindexload2.rmt_head, dump->sindexload[index][3].sindexload3.rmt_tail);
		if (dump->sindexload[index + 8][1].sindexload2.rmt_is_head)
			name = "Queue %db Memory (is head)";
		else
			name = "Queue %db Memory";
		__cvmx_pow_display_list(name, index,
					dump->sindexload[index + 8][1].sindexload2.rmt_val,
					dump->sindexload[index + 8][1].sindexload2.rmt_one,
					dump->sindexload[index + 8][1].sindexload2.rmt_head, dump->sindexload[index + 8][3].sindexload3.rmt_tail);
	}

	/* Print out each of the internal POW entries. Each entry has a tag, group,
	   wqe, and possibly a next pointer. The next pointer is only valid if this
	   entry isn't make as a tail */
	for (index = 0; index < num_pow_entries; index++) {
		cvmx_dprintf("Entry %d(%-10s", index, __cvmx_pow_list_names[entry_list[index].name_idx]);
		if (entry_list[index].num_idx >= 0) {
			cvmx_dprintf(" %d", entry_list[index].num_idx);
		}
		cvmx_dprintf("): tag=%s,0x%08x grp=%d wqp=0x%016llx",
				OCT_TAG_TYPE_STRING(dump->smemload[index][0].s_smemload0.tag_type),
				dump->smemload[index][0].s_smemload0.tag,
				dump->smemload[index][0].s_smemload0.grp,
				CAST64(dump->smemload[index][2].s_smemload1.wqp));
		if (dump->smemload[index][0].s_smemload0.tail)
			cvmx_dprintf(" tail");
		else
			cvmx_dprintf(" next=%d", dump->smemload[index][0].s_smemload0.next_index);
		if (entry_list[index].name_idx == CVMX_POW_LIST_NOSCHED) {
			cvmx_dprintf(" nosched=%d", dump->smemload[index][1].s_smemload2.nosched);
			if (dump->smemload[index][1].s_smemload2.pend_switch) {
				cvmx_dprintf(" pending tag=%s,0x%08x",
				       OCT_TAG_TYPE_STRING(dump->smemload[index][1].s_smemload2.pend_type), dump->smemload[index][1].s_smemload2.pend_tag);
			}
		}
		cvmx_dprintf("\n");
	}
}

void __cvmx_pow_display_v2(void *buffer, int buffer_size)
{
	__cvmx_pow_dump_t *dump = (__cvmx_pow_dump_t *) buffer;
	int num_pow_entries = cvmx_pow_get_num_entries();
	int num_cores;
	int core;
	int index;
	struct __cvmx_entry_list_s entry_list[2048];

	if (buffer_size < (int)sizeof(__cvmx_pow_dump_t)) {
		cvmx_dprintf("cvmx_pow_dump: Buffer too small, pow_dump_t = 0x%x, buffer_size = 0x%x\n", (int)sizeof(__cvmx_pow_dump_t), buffer_size);
		return;
	}

	for (index = 0; index < 2048; index++) {
		entry_list[index].name_idx = 0;
		entry_list[index].num_idx = -1;
	}
	num_cores = cvmx_octeon_num_cores();

	/* Print the free list info */
	{
		int valid[3], has_one[3], head[3], tail[3], qnum_head, qnum_tail;
		int idx;

		valid[0] = dump->sindexload[0][4].sindexload1_cn68xx.queue_val;
		valid[1] = dump->sindexload[0][5].sindexload1_cn68xx.queue_val;
		valid[2] = dump->sindexload[0][6].sindexload1_cn68xx.queue_val;
		has_one[0] = dump->sindexload[0][4].sindexload1_cn68xx.queue_one;
		has_one[1] = dump->sindexload[0][5].sindexload1_cn68xx.queue_one;
		has_one[2] = dump->sindexload[0][6].sindexload1_cn68xx.queue_one;
		head[0] = dump->sindexload[0][4].sindexload1_cn68xx.queue_head;
		head[1] = dump->sindexload[0][5].sindexload1_cn68xx.queue_head;
		head[2] = dump->sindexload[0][6].sindexload1_cn68xx.queue_head;
		tail[0] = dump->sindexload[0][4].sindexload1_cn68xx.queue_tail;
		tail[1] = dump->sindexload[0][5].sindexload1_cn68xx.queue_tail;
		tail[2] = dump->sindexload[0][6].sindexload1_cn68xx.queue_tail;
		qnum_head = dump->sindexload[0][4].sindexload1_cn68xx.qnum_head;
		qnum_tail = dump->sindexload[0][4].sindexload1_cn68xx.qnum_tail;

		cvmx_dprintf("Free List: qnum_head=%d, qnum_tail=%d\n", qnum_head, qnum_tail);
		cvmx_dprintf("Free0: valid=%d, one=%d, head=%llu, tail=%llu\n", valid[0], has_one[0], CAST64(head[0]), CAST64(tail[0]));
		cvmx_dprintf("Free1: valid=%d, one=%d, head=%llu, tail=%llu\n", valid[1], has_one[1], CAST64(head[1]), CAST64(tail[1]));
		cvmx_dprintf("Free2: valid=%d, one=%d, head=%llu, tail=%llu\n", valid[2], has_one[2], CAST64(head[2]), CAST64(tail[2]));

		idx = qnum_head;
		while (valid[0] || valid[1] || valid[2]) {
			int qidx = idx % 3;

			if (head[qidx] == tail[qidx])
				valid[qidx] = 0;

			if (__cvmx_pow_entry_mark_list(head[qidx], CVMX_POW_LIST_FREE, -1, entry_list))
				break;
			head[qidx] = dump->smemload[head[qidx]][4].s_smemload3_cn68xx.fwd_index;
			//printf("qidx = %d, idx = %d, head[qidx] = %d\n", qidx, idx, head[qidx]);
			idx++;
		}
	}

	/* Print the core state */
	for (core = 0; core < num_cores; core++) {
		int pendtag = 1;
		int pendwqp = 2;
		int tag = 3;
		int wqp = 4;
		int links = 5;

		if (dump->sstatus[core][tag].s_sstatus2_cn68xx.tag_type != CVMX_POW_TAG_TYPE_NULL_NULL) {
			__cvmx_pow_entry_mark_list(dump->sstatus[core][tag].s_sstatus2_cn68xx.index, CVMX_POW_LIST_CORE, core, entry_list);
		}
		cvmx_dprintf("Core %d State: tag=%s,0x%08x", core,
		       OCT_TAG_TYPE_STRING(dump->sstatus[core][tag].s_sstatus2_cn68xx.tag_type), dump->sstatus[core][tag].s_sstatus2_cn68xx.tag);
		cvmx_dprintf(" grp=%d", dump->sstatus[core][tag].s_sstatus2_cn68xx.grp);
		cvmx_dprintf(" wqp=0x%016llx", CAST64(dump->sstatus[core][wqp].s_sstatus3_cn68xx.wqp));
		cvmx_dprintf(" index=%d", dump->sstatus[core][tag].s_sstatus2_cn68xx.index);
		cvmx_dprintf(" prev=%d", dump->sstatus[core][links].s_sstatus4_cn68xx.revlink_index);
		cvmx_dprintf(" next=%d", dump->sstatus[core][links].s_sstatus4_cn68xx.link_index);
		if (dump->sstatus[core][links].s_sstatus4_cn68xx.head)
			cvmx_dprintf(" head");
		if (dump->sstatus[core][links].s_sstatus4_cn68xx.tail)
			cvmx_dprintf(" tail");
		if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_switch) {
			cvmx_dprintf(" pend_switch=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_switch);
			cvmx_dprintf(" pending tag=%s,0x%08x",
			       OCT_TAG_TYPE_STRING(dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_type), dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_tag);
		}
		if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_desched) {
			cvmx_dprintf(" pend_desched=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_desched);
			cvmx_dprintf(" pend_nosched=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_nosched);
		}
		if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_get_work) {
			if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_get_work_wait)
				cvmx_dprintf(" (Waiting for work)");
			else
				cvmx_dprintf(" (Getting work)");
		}
		if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_alloc_we)
			cvmx_dprintf(" pend_alloc_we=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_alloc_we);
		if (dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_nosched_clr) {
			cvmx_dprintf(" pend_nosched_clr=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_nosched_clr);
			cvmx_dprintf(" pend_index=%d", dump->sstatus[core][pendtag].s_sstatus0_cn68xx.pend_index);
		}
		if (dump->sstatus[core][pendwqp].s_sstatus1_cn68xx.pend_nosched_clr)
			cvmx_dprintf(" pend_wqp=0x%016llx\n", CAST64(dump->sstatus[core][pendwqp].s_sstatus1_cn68xx.pend_wqp));
		cvmx_dprintf("\n");
	}

	/* Print out the state of the nosched list and the 16 deschedule lists. */
	__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_NOSCHED, 0, dump, entry_list,
					 dump->sindexload[0][3].sindexload0_cn68xx.queue_val,
					 dump->sindexload[0][3].sindexload0_cn68xx.queue_one,
					 dump->sindexload[0][3].sindexload0_cn68xx.queue_head, dump->sindexload[0][3].sindexload0_cn68xx.queue_tail);
	for (index = 0; index < 64; index++) {
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_DESCHED, index, dump, entry_list,
						 dump->sindexload[index][2].sindexload0_cn68xx.queue_val,
						 dump->sindexload[index][2].sindexload0_cn68xx.queue_one,
						 dump->sindexload[index][2].sindexload0_cn68xx.queue_head, dump->sindexload[index][2].sindexload0_cn68xx.queue_tail);
	}

	/* Print out the state of the 8 internal input queues */
	for (index = 0; index < 8; index++) {
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_INPUT, index, dump, entry_list,
						 dump->sindexload[index][1].sindexload0_cn68xx.queue_val,
						 dump->sindexload[index][1].sindexload0_cn68xx.queue_one,
						 dump->sindexload[index][1].sindexload0_cn68xx.queue_head, dump->sindexload[index][1].sindexload0_cn68xx.queue_tail);
	}

	/* Print out each of the internal POW entries. Each entry has a tag, group,
	   wqe, and possibly a next pointer. The next pointer is only valid if this
	   entry isn't make as a tail */
	for (index = 0; index < num_pow_entries; index++) {
		cvmx_dprintf("Entry %d(%-10s", index, __cvmx_pow_list_names[entry_list[index].name_idx]);
		if (entry_list[index].num_idx >= 0) {
			cvmx_dprintf(" %d", entry_list[index].num_idx);
		}
		cvmx_dprintf("): tag=%s,0x%08x grp=%d wqp=0x%016llx",
				OCT_TAG_TYPE_STRING(dump->smemload[index][1].s_smemload0_cn68xx.tag_type),
				dump->smemload[index][1].s_smemload0_cn68xx.tag,
				dump->smemload[index][2].s_smemload1_cn68xx.grp,
				CAST64(dump->smemload[index][2].s_smemload1_cn68xx.wqp));
		cvmx_dprintf(" next_index=%d", dump->smemload[index][4].s_smemload3_cn68xx.next_index);
		cvmx_dprintf(" fwd_index=%d", dump->smemload[index][4].s_smemload3_cn68xx.fwd_index);
		if (dump->smemload[index][1].s_smemload0_cn68xx.tail)
			cvmx_dprintf(" tail");
		if (dump->smemload[index][3].s_smemload2_cn68xx.pend_switch) {
			cvmx_dprintf(" nosched=%d", dump->smemload[index][2].s_smemload1_cn68xx.nosched);
			cvmx_dprintf(" pending tag=%s,0x%08x",
			       OCT_TAG_TYPE_STRING(dump->smemload[index][3].s_smemload2_cn68xx.pend_type), dump->smemload[index][3].s_smemload2_cn68xx.pend_tag);
		}
		cvmx_dprintf("\n");
	}
}

static void sso_free_list_show_init_v3(__cvmx_sso_dump_7xxx_t *dump, struct __cvmx_entry_list_s *entry_list)
{
	int index;
	int end_idx;
	int qnum_head;
	int qnum_tail;
	int cur[5];
	cvmx_sso_ipl_freex_t free;
	int queue_val;
	int queue_head;
	int queue_tail;

	free.u64 = dump->ipl_free[0];
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		qnum_head = free.cn78xx.qnum_head;
		qnum_tail = free.cn78xx.qnum_tail;
	} else {
		qnum_head = free.cn73xx.qnum_head;
		qnum_tail = free.cn73xx.qnum_tail;
	}
	cvmx_dprintf("Free List: qnum_head=%d, qnum_tail=%d\n",
			qnum_head, qnum_tail);
	for (index = 0; index < 5; index++) {
		free.u64 = dump->ipl_free[index];
		if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
			queue_val = free.cn78xx.queue_val;
			queue_head = free.cn78xx.queue_head;
			queue_tail = free.cn78xx.queue_tail;
		} else {
			queue_val = free.cn73xx.queue_val;
			queue_head = free.cn73xx.queue_head;
			queue_tail = free.cn73xx.queue_tail;
		}
		cvmx_dprintf("Free List%d: valid:%d, head:%d, tail:%d\n",
				index, queue_val,
				queue_head, queue_tail);
		cur[index] = queue_head;
	}
	cvmx_dprintf("\n");

	/* Get the start and end values for the free list loop. */
	index = qnum_head;
	free.u64 = dump->ipl_free[(qnum_tail + 4) % 5];
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		end_idx = free.cn78xx.queue_tail;
	} else {
		end_idx = free.cn73xx.queue_tail;
	}
	while (1) {
		if (__cvmx_pow_entry_mark_list(cur[index], CVMX_POW_LIST_FREE, -1, entry_list))
			break;
		if (cur[index] == end_idx)
			break;
		if (OCTEON_IS_MODEL(OCTEON_CN78XX))
			cur[index] = dump->sso_ient[cur[index]][3].s_sso_ientx_links_cn78xx.prev_index;
		else
			cur[index] = dump->sso_ient[cur[index]][3].s_sso_ientx_links_cn73xx.prev_index;
		index = (index + 1) % 5;
	}
}

/* This routine is used to show debug information for SSO in case of the
 * CN7XXX chips. */
static void __cvmx_sso_display_v3(void *buffer, int buffer_size)
{
	__cvmx_sso_dump_7xxx_t *dump = (__cvmx_sso_dump_7xxx_t *) buffer;
	int num_pow_entries;
	int num_queues;
	struct __cvmx_entry_list_s entry_list[4096];
	uint64_t core;
	int index;
	int wqpgrp_grp;
	int wqpgrp_wqp;
	int links_next;
	int links_prev;

	for (index = 0; index < 4096; index++) {
		entry_list[index].name_idx = 0;
		entry_list[index].num_idx = -1;
	}

	cvmx_dprintf("\n");
	/* Print the free list info */
	sso_free_list_show_init_v3(dump, entry_list);

	/* Print the core state */
	for (core = 0; core < dump->n_cores; core++) {
		cvmx_sso_sl_ppx_pendtag_t ptag;
		cvmx_sso_sl_ppx_pendwqp_t pwqp;
		cvmx_sso_sl_ppx_tag_t tag;
		cvmx_sso_sl_ppx_wqp_t wqp;

		ptag.u64 = dump->sso_core_state[core][1].u64;
		pwqp.u64 = dump->sso_core_state[core][2].u64;
		tag.u64 = dump->sso_core_state[core][3].u64;
		wqp.u64 = dump->sso_core_state[core][4].u64;

		if (tag.s.tt != CVMX_POW_TAG_TYPE_NULL_NULL) {
			__cvmx_pow_entry_mark_list(tag.s.index, CVMX_POW_LIST_CORE, core, entry_list);
		}
		cvmx_dprintf("Core %lu State: tag=%s,0x%08x",
				(long unsigned int)core,
				oct_tag_type_string_v3[tag.s.tt], tag.s.tag);
		cvmx_dprintf(" grp=%d", tag.s.grp);
		cvmx_dprintf(" wqp=0x%016lx", (long unsigned int)wqp.s.wqp);
		cvmx_dprintf(" index=%d", tag.s.index);
		if (tag.s.head)
			cvmx_dprintf(" head");
		if (tag.s.tail)
			cvmx_dprintf(" tail");
		if (ptag.s.pend_switch) {
			cvmx_dprintf(" pend_switch=%d", ptag.s.pend_switch);
			cvmx_dprintf(" pending tag=%s,0x%08x",
					oct_tag_type_string_v3[ptag.s.pend_tt], ptag.s.pend_tag);
		}
		if (ptag.s.pend_desched) {
			cvmx_dprintf(" pend_desched=%d", ptag.s.pend_desched);
			cvmx_dprintf(" pend_nosched=%d", ptag.s.pend_nosched);
		}
		if (pwqp.s.pend_get_work) {
			if (pwqp.s.pend_get_work_wait)
				cvmx_dprintf(" (Waiting for work)");
			else
				cvmx_dprintf(" (Getting work)");
		}
		if (pwqp.s.pend_alloc_we)
			cvmx_dprintf(" pend_alloc_we=%d", pwqp.s.pend_alloc_we);
		if (ptag.s.pend_nosched_clr) {
			cvmx_dprintf(" pend_nosched_clr=%d", pwqp.s.pend_nosched_clr);
			cvmx_dprintf(" pend_index=%d", pwqp.s.pend_index);
		}
		if (pwqp.s.pend_nosched_clr)
			cvmx_dprintf(" pend_wqp=0x%016lx", (long unsigned int)pwqp.s.pend_wqp);
		cvmx_dprintf("\n");
	}
	cvmx_dprintf("\n");

	num_queues = cvmx_get_num_queues();
	/* Print out the status for the internal admission queues. */
	for (index = 0; index < num_queues; index++) {
		cvmx_sso_ipl_iaqx_t iaq_state;
		iaq_state.u64 = dump->sso_iaq[index];
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_INPUT, index, (void *)dump, entry_list,
				iaq_state.s.queue_val,
				iaq_state.s.queue_one,
				iaq_state.s.queue_head,
				iaq_state.s.queue_tail);
	}
	cvmx_dprintf("\n");

	/* Print out the status of the deschedule lists. */
	for (index = 0; index < num_queues; index++) {
		cvmx_sso_ipl_deschedx_t dsch_state;
		dsch_state.u64 = dump->sso_desched[index];
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_DESCHED, index, (void *)dump, entry_list,
				dsch_state.s.queue_val,
				dsch_state.s.queue_one,
				dsch_state.s.queue_head,
				dsch_state.s.queue_tail);
	}
	cvmx_dprintf("\n");

	/* Print out the status of the conflicte lists. */
	for (index = 0; index < num_queues; index++) {
		cvmx_sso_ipl_confx_t conf_state;
		conf_state.u64 = dump->sso_conf[index];
		__cvmx_pow_display_list_and_walk(CVMX_POW_LIST_CONF, index, (void *)dump, entry_list,
				conf_state.s.queue_val,
				conf_state.s.queue_one,
				conf_state.s.queue_head,
				conf_state.s.queue_tail);
	}
	cvmx_dprintf("\n");

	/* Print entries. */
	num_pow_entries = cvmx_pow_get_num_entries();
	for (index = 0; index < num_pow_entries; index++) {
		cvmx_dprintf("Entry %d(%-10s", index, __cvmx_pow_list_names[entry_list[index].name_idx]);
		if (entry_list[index].num_idx >= 0) {
			cvmx_dprintf(" %d", entry_list[index].num_idx);
		}
		if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
			wqpgrp_grp = dump->sso_ient[index][1].s_sso_ientx_wqpgrp_cn78xx.grp;
			wqpgrp_wqp = dump->sso_ient[index][1].s_sso_ientx_wqpgrp_cn78xx.wqp;
			links_next = dump->sso_ient[index][3].s_sso_ientx_links_cn78xx.next_index;
			links_prev = dump->sso_ient[index][3].s_sso_ientx_links_cn78xx.prev_index;
		} else {
			wqpgrp_grp = dump->sso_ient[index][1].s_sso_ientx_wqpgrp_cn73xx.grp;
			wqpgrp_wqp = dump->sso_ient[index][1].s_sso_ientx_wqpgrp_cn73xx.wqp;
			links_next = dump->sso_ient[index][3].s_sso_ientx_links_cn73xx.next_index;
			links_prev = dump->sso_ient[index][3].s_sso_ientx_links_cn73xx.prev_index;
		}
		cvmx_dprintf("): tag=%s,0x%08x grp=%d wqp=0x%016llx",
				oct_tag_type_string_v3[dump->sso_ient[index][0].s_sso_ientx_tag_cn78xx.tt],
				dump->sso_ient[index][0].s_sso_ientx_tag_cn78xx.tag,
				wqpgrp_grp, CAST64(wqpgrp_wqp));
		cvmx_dprintf(" next_index=%d", links_next);
		cvmx_dprintf(" fwd_index=%d", links_prev);
		if (dump->sso_ient[index][0].s_sso_ientx_tag_cn78xx.tail)
			cvmx_dprintf(" tail");
		if (dump->sso_ient[index][2].s_sso_ientx_pendtag_cn78xx.pend_switch) {
			cvmx_dprintf(" pending tag=%s,0x%08x",
					oct_tag_type_string_v3[dump->sso_ient[index][2].s_sso_ientx_pendtag_cn78xx.pend_tt],
					dump->sso_ient[index][2].s_sso_ientx_pendtag_cn78xx.pend_tag);
		}
		cvmx_dprintf("\n");
	}
}

/**
 * Dump a POW capture to the console in a human readable format.
 *
 * @param buffer POW capture from cvmx_pow_capture()
 * @param buffer_size
 *               Size of the buffer
 */
void cvmx_pow_display(void *buffer, int buffer_size)
{
	if (buffer_size < cvmx_pow_get_dump_size()) {
		cvmx_dprintf("cvmx_pow_display: Buffer too small. " "At least %d bytes are needed.\n",
			     cvmx_pow_get_dump_size());
		return;
	}
	cvmx_dprintf("POW Display Start\n");

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		__cvmx_sso_display_v3(buffer, buffer_size);
	else if (octeon_has_feature(OCTEON_FEATURE_CN68XX_WQE))
		__cvmx_pow_display_v2(buffer, buffer_size);
	else
		__cvmx_pow_display_v1(buffer, buffer_size);

	cvmx_dprintf("POW Display End\n");
}

#ifndef	CVMX_BUILD_FOR_LINUX_HOST

#define __CVMX_SSO_XAQ_BUF_SIZE	4096

static int cvmx_sso3_init(unsigned node, int wqe_entries)
{
	cvmx_sso_xaq_aura_t aura_reg;
	cvmx_sso_nw_tim_t nw_tim;
	cvmx_sso_aw_cfg_t aw_cfg;
	unsigned grp;
	/* Setup an FPA pool to store the SSO queues */
	const int MAX_SSO_ENTRIES = 4096;
	int num_blocks;
	cvmx_fpa3_gaura_t aura;

	/* avoid repeated intialization */
	aw_cfg.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_CFG);
	if (aw_cfg.s.rwen) {
		cvmx_printf("WARNING: %s: SSO already enabled\n", __func__);
		return 0;
	}

	/*
	 * Assuming all WQE entries need DRAM space,
	 * even those that can fit on-chip.
	 */
	if(wqe_entries == 0)
		wqe_entries = MAX_SSO_ENTRIES;

	/* HRM formula:
	 * CVM_OCT_SSO_NUMBER_XAQ_BUFS = 256*2 + 48 + RNDUP(n / 416)
	 */

	num_blocks =  (wqe_entries+415) / 416;
	num_blocks = 256 * 2 + 48 + num_blocks;

	/* Make sure the SSO is disabled */
	aw_cfg.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_CFG);
	aw_cfg.s.rwen = 0;
	aw_cfg.s.xaq_byp_dis = 0;
	aw_cfg.s.xaq_alloc_dis = 0;
	cvmx_write_csr_node(node, CVMX_SSO_AW_CFG, aw_cfg.u64);

	/* Allocate dedicated pool/aura for SSO DRAM buffers */
	aura = cvmx_fpa3_setup_aura_and_pool(node, -1, "SSO AURA", NULL,
			__CVMX_SSO_XAQ_BUF_SIZE, num_blocks);

	if (debug)
		cvmx_dprintf("%s: aura=%d num_blocks=%u\n",
			__func__, aura.laura, num_blocks);

	/* Initialize the group/qos queues (256 for 78xx, 64 for 73xx) */
	for (grp = 0; grp < cvmx_sso_num_xgrp(); grp++) {
		cvmx_sso_xaqx_head_ptr_t hptr;
		cvmx_sso_xaqx_head_next_t hnext;
		cvmx_sso_xaqx_tail_ptr_t tptr;
		cvmx_sso_xaqx_tail_next_t tnext;
		uint64_t addr;
		void *buffer = cvmx_fpa3_alloc(aura);

		if (buffer == NULL) {
			cvmx_dprintf("ERROR: "
				"SSO Failed to allocate buffer for xgrp %u\n", grp);
			break;
		}
		addr = cvmx_ptr_to_phys(buffer);

		/* HRM Sec 9.12.1:
		 * The final 256 buffers are used to initialize each
		 * SSO_XAQ(0..255)_HEAD_PTR and SSO_XAQ(0..255)_TAIL_PTR
		 * to identical values (per group).
		 */
		hptr.u64 = hnext.u64 = 0;
		tptr.u64 = tnext.u64 = 0;
		hptr.s.ptr =  addr >> 7;
		tptr.s.ptr =  addr >> 7;
		tnext.s.ptr = hnext.s.ptr = 0;

		/*
		From HRM:
		 SSO External Admission Queue Head Pointer Register
		 SSO_XAQ(0..255)_HEAD_PTR
		These registers contain the pointer to the first entry of
		the external linked list(s) for a particular group.
		Software must initialize the external linked list(s)
		by programming
		SSO_XAQ(0..255)_HEAD_PTR, SSO_XAQ(0..255)_HEAD_NEXT,
		SSO_XAQ(0..255)_TAIL_PTR and SSO_XAQ(0..255)_TAIL_NEXT
		to identical values. See Table 9–3 for the address.
		*/
		cvmx_write_csr_node(node,
			CVMX_SSO_XAQX_HEAD_PTR(grp), hptr.u64);
		cvmx_write_csr_node(node,
			CVMX_SSO_XAQX_HEAD_NEXT(grp), hnext.u64);
		cvmx_write_csr_node(node,
			CVMX_SSO_XAQX_TAIL_NEXT(grp), tnext.u64);
		cvmx_write_csr_node(node,
			CVMX_SSO_XAQX_TAIL_PTR(grp), tptr.u64);

	}

	/* Set the aura number */
	aura_reg.u64 = 0;
	aura_reg.s.laura = aura.laura;
	aura_reg.s.node = aura.node;
	cvmx_write_csr_node(node, CVMX_SSO_XAQ_AURA, aura_reg.u64);

	/* Set work timeout to 1023 * 1k cycles */
	nw_tim.u64 = 0;
	nw_tim.s.nw_tim = 0;//hrm says 4 is typical but for 1023k cycles its 0
	cvmx_write_csr_node(node, CVMX_SSO_NW_TIM, nw_tim.u64);

	/* Enable the SSO access to FPA */
	aw_cfg.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_CFG);
	aw_cfg.s.rwen = 1;
	aw_cfg.s.xaq_byp_dis = 0;
	aw_cfg.s.xaq_alloc_dis = 0;
	cvmx_write_csr_node(node, CVMX_SSO_AW_CFG, aw_cfg.u64);

	return 0;
}

/***
 * @INTERNAL
 *
 * Configure the SSO on-chip resources
 *
 * The on-chip buffers are reserved fairly to all groups,
 * so that high-priority groups would not monopolize the SSO
 * and starve lower priority queues of resources.
 * Also, this should result in early spill of SSO buffers onto DRAM
 * which is important for validating the chip.
 */
static int cvmx_sso3_setup(unsigned node)
{
	unsigned grp;
	unsigned iaq_rsvd_cnt = 0;
	unsigned taq_rsvd_cnt = 0;
	cvmx_sso_aw_we_t aw_we;
	cvmx_sso_taq_cnt_t taq_cnt;
	cvmx_sso_aw_add_t aw_add;
	cvmx_sso_taq_add_t taq_add;
	cvmx_sso_grpx_iaq_thr_t iaq_thr;
	cvmx_sso_grpx_taq_thr_t taq_thr;
	unsigned iaq_rsvd, taq_rsvd;
	unsigned iaq_max, taq_max;

	aw_add.u64 = taq_add.u64 = iaq_thr.u64 = taq_thr.u64 = 0;

	/*
	 * SSO_AW_WE[FREE_CNT] indicates the number of available
	 * IAQ entries — a total number available for all groups.
	 */
	aw_we.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_WE);

	/*
	 * SSO_TAQ_CNT[FREE_CNT] indicates the number of available
	 * TAQ entries available to all groups,
	 */
	taq_cnt.u64 = cvmx_read_csr_node(node, CVMX_SSO_TAQ_CNT);

	if (debug)
		cvmx_dprintf("%s: initial IAQ rsvd_free=%u TAQ rsvd_free=%u\n",
			__func__,
			(unsigned) aw_we.s.rsvd_free,
			(unsigned) taq_cnt.s.rsvd_free);

	/* Give out half of buffers fairly, rest left floating */
	iaq_rsvd = aw_we.s.free_cnt / cvmx_sso_num_xgrp() / 2;

	/* Enforce minimum per HRM */
	if (iaq_rsvd < 2)
		iaq_rsvd = 2;

	iaq_max = iaq_rsvd << 7;
	if (iaq_max >= (1 << 13))
		iaq_max = (1 << 13) -1;

	if (debug)
		cvmx_dprintf("%s: IAQ free=%d rsvd_thr=%u max_thr=%u\n",
			__func__, aw_we.s.free_cnt, iaq_rsvd, iaq_max);

	/* Give out half of all buffers fairly, other half floats */
	taq_rsvd = taq_cnt.s.free_cnt / cvmx_sso_num_xgrp() / 2;

	/* Enforce minimum per HRM */
	if (taq_rsvd < 3)
		taq_rsvd = 3;

	/* Enforce maximum to avoid erratum */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X)) {
		taq_max = 1264 / cvmx_sso_num_xgrp();
		if (taq_rsvd > taq_max)
			taq_rsvd = taq_max;
	} else {
		/* Provide for streaming performance, enforce field max */
		taq_max = taq_rsvd << 3;
		if (taq_max >= (1 << 11))
			taq_max = (1 << 11) - 1;
	}

	if (debug)
		cvmx_dprintf("%s: TAQ free=%u rsvd_thr=%u rsvd_max=%u\n",
			__func__, taq_cnt.s.free_cnt, taq_rsvd, taq_max);

	/* Add per-group QoS params */
	for (grp = 0; grp < cvmx_sso_num_xgrp(); grp++) {

		aw_add.u64 = taq_add.u64 = 0;

		iaq_thr.u64 =
			cvmx_read_csr_node(node, CVMX_SSO_GRPX_IAQ_THR(grp));
		aw_add.s.rsvd_free = iaq_rsvd - iaq_thr.s.rsvd_thr;
		iaq_thr.s.rsvd_thr = iaq_rsvd;
		iaq_thr.s.max_thr =  iaq_max;
		cvmx_write_csr_node(node, CVMX_SSO_GRPX_IAQ_THR(grp),
			iaq_thr.u64);

		iaq_rsvd_cnt += iaq_thr.s.rsvd_thr;

		/*
		 * Updates to this field must also update
		 * SSO_AW_ADD[RSVD_FREE].
		 */
		if (aw_add.s.rsvd_free)
			cvmx_write_csr_node(node, CVMX_SSO_AW_ADD, aw_add.u64);

		/*
		 * Changes to this field must also update
		 * SSO_TAQ_ADD[RSVD_FREE].
		 * Must not be changed after traffic is sent to this group
		 */
		taq_thr.u64 =
			cvmx_read_csr_node(node, CVMX_SSO_GRPX_TAQ_THR(grp));
		taq_add.s.rsvd_free = taq_rsvd - taq_thr.s.rsvd_thr;
		taq_thr.s.rsvd_thr = taq_rsvd;
		taq_thr.s.max_thr = taq_max;
		cvmx_write_csr_node(node, CVMX_SSO_GRPX_TAQ_THR(grp),
			taq_thr.u64);
		taq_rsvd_cnt += taq_thr.s.rsvd_thr;

		/* Add the delta of added reserved buffers to TAQ_CNT */
		if (taq_add.s.rsvd_free)
			cvmx_write_csr_node(node, CVMX_SSO_TAQ_ADD, taq_add.u64);
	} /* for grp */

	aw_we.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_WE);
	taq_cnt.u64 = cvmx_read_csr_node(node, CVMX_SSO_TAQ_CNT);

	if (debug) {
		cvmx_dprintf("%s: total IAQ rsvd_cnt=%u TAQ rsvd_cnt=%u\n",
			__func__, iaq_rsvd_cnt, taq_rsvd_cnt);

		/*
		 * Software must initialize SSO_AW_WE[RSVD_FREE]
		 * to the total of all groups’ [RSVD_THR]
		 */
		cvmx_dprintf("%s: Reserved IAQ %u TAQ %u\n",
			__func__, aw_we.s.rsvd_free, taq_cnt.s.rsvd_free);
	}

	/*
	 * Verify SSO_AW_WE[RSVD_FREE], TAQ_CNT[RSVD_FREE] are equal to sum
	 * of IAQ[RSVD_THR], TAQ[RSRVD_THR] fields
	 * Note that TAQ free sum could be lower than expected
	 * due to some cores holding a TAQ entry from the last
	 * processed WQE, and not having switched tag to NULL
	 * when it finished its main loop.
	 */
	if (aw_we.s.rsvd_free != iaq_rsvd_cnt) {
		cvmx_dprintf("WARNING: SSO resource allocation math: "
		"IAQ %u != %u, TAQ %u != %u\n",
		(unsigned) aw_we.s.rsvd_free, (unsigned) iaq_rsvd_cnt,
		(unsigned) taq_cnt.s.rsvd_free, (unsigned) taq_rsvd_cnt);

		aw_we.s.rsvd_free = iaq_rsvd_cnt;
		taq_cnt.s.rsvd_free = taq_rsvd_cnt;

	cvmx_write_csr_node(node, CVMX_SSO_AW_WE, aw_we.u64);
	cvmx_write_csr_node(node, CVMX_SSO_TAQ_CNT, taq_cnt.u64);
	}

	return 0;
}


static int cvmx_initialize_sso_68xx(int wqe_entries)
{
	int cvm_oct_sso_number_rwq_bufs;
	int i;
	int64_t paddr;
	cvmx_sso_cfg_t sso_cfg;
	cvmx_fpa_fpfx_marks_t fpa_marks;
	cvmx_sso_err_enb_t sso_err_enb;
	const unsigned rwq_size = 256; /* Fixed, can not change */

	/* avoid repeated intialization */
	sso_cfg.u64 = cvmx_read_csr(CVMX_SSO_CFG);
	if (sso_cfg.s.rwen) {
		cvmx_printf("WARNING: %s: SSO already enabled\n",__func__);
		return 1;
	}

	/*
	 * CN68XX-P1 may reset with the wrong values, put in
	 * the correct values (FPA-15816).
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX_PASS1_X)) {
		fpa_marks.u64 = 0;
		fpa_marks.s.fpf_wr = 0xa4;
		fpa_marks.s.fpf_rd = 0x40;
		cvmx_write_csr(CVMX_FPA_FPF8_MARKS, fpa_marks.u64);
	}

	/*
	 * Section 7.10.1:
	 * CVM_OCT_SSO_NUMBER_RWQ_BUFS = 8 + 48 + RNDUP ( n ÷ 26 )
	 */
	cvm_oct_sso_number_rwq_bufs = ((wqe_entries + 25) / 26) + 48 + 8 + 20;

	/* Make sure RWI/RWO is disabled. */
	sso_cfg.u64 = cvmx_read_csr(CVMX_SSO_CFG);
	sso_cfg.s.rwen = 0;
	cvmx_write_csr(CVMX_SSO_CFG, sso_cfg.u64);

	for (i = cvm_oct_sso_number_rwq_bufs - 8; i > 0; i--) {
		cvmx_sso_rwq_psh_fptr_t fptr;

		paddr = cvmx_bootmem_phy_alloc(rwq_size, 0, 0,
			CVMX_CACHE_LINE_SIZE, 0);

		if (paddr <= 0)
			break;

		for (;;) {
			fptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_PSH_FPTR);
			if (!fptr.s.full)
				break;
			cvmx_wait(1000);
		}

		fptr.s.fptr = paddr >> 7;
		cvmx_write_csr(CVMX_SSO_RWQ_PSH_FPTR, fptr.u64);
	}

	if (paddr == 0) {
		cvmx_printf("ERROR: %s: Out of memory\n", __func__);
		return -1;
	}

	for (i = 0; i < 8; i++) {
		cvmx_sso_rwq_head_ptrx_t head_ptr;
		cvmx_sso_rwq_tail_ptrx_t tail_ptr;

		paddr = cvmx_bootmem_phy_alloc(rwq_size, 0, 0,
			CVMX_CACHE_LINE_SIZE, 0);
		if (paddr <= 0)
			break;

		head_ptr.u64 = 0;
		tail_ptr.u64 = 0;
		head_ptr.s.ptr = paddr >> 7;
		tail_ptr.s.ptr = head_ptr.s.ptr;
		cvmx_write_csr(CVMX_SSO_RWQ_HEAD_PTRX(i), head_ptr.u64);
		cvmx_write_csr(CVMX_SSO_RWQ_TAIL_PTRX(i), tail_ptr.u64);
	}

	if (paddr == 0) {
		cvmx_printf("ERROR: %s: Out of memory\n", __func__);
		return -1;
	}

	sso_cfg.u64 = cvmx_read_csr(CVMX_SSO_CFG);
	sso_cfg.s.rwen = 1;
	sso_cfg.s.dwb = cvmx_helper_cfg_opt_get(CVMX_HELPER_CFG_OPT_USE_DWB);
	sso_cfg.s.rwq_byp_dis = 0;
	sso_cfg.s.rwio_byp_dis = 0;
	cvmx_write_csr(CVMX_SSO_CFG, sso_cfg.u64);

	/* Enable SSO error interrupt */
	sso_err_enb.u64 = cvmx_read_csr(CVMX_SSO_ERR_ENB);
	sso_err_enb.s.fpe_ie = 1;
	cvmx_write_csr(CVMX_SSO_ERR_ENB, sso_err_enb.u64);

	return 0;
}

int cvmx_helper_initialize_sso_node(unsigned node, int wqe_entries)
{
        if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		int rc;
		(void) cvmx_sso3_setup(node);
		rc = cvmx_sso3_init(node, wqe_entries);
		return rc;
	} else if (octeon_has_feature(OCTEON_FEATURE_CN68XX_WQE))
		return cvmx_initialize_sso_68xx(wqe_entries);
	else
		return 0;	/* older POW does not need to be initialized */
}

int cvmx_helper_initialize_sso(int wqe_entries)
{
	unsigned node = cvmx_get_node_num();
	return cvmx_helper_initialize_sso_node(node, wqe_entries);
}

static int cvmx_uninitialize_sso_68xx(void)
{
	cvmx_fpa_quex_available_t queue_available;
	cvmx_sso_cfg_t sso_cfg;
	cvmx_sso_rwq_pop_fptr_t pop_fptr;
	cvmx_sso_rwq_psh_fptr_t fptr;
	cvmx_sso_fpage_cnt_t fpage_cnt;
	int num_to_transfer, i;
	cvmx_sso_err_enb_t sso_err_enb;
	uint64_t paddr;
	const unsigned rwq_size = 256; /* Fixed, can not change */

	/*
	 * If the free page error (FPE) interrupt enable is set in
	 * SSO_ERR_ENB[FPE_IE], it should be cleared before extracting pointers
	 */
	sso_err_enb.u64 = cvmx_read_csr(CVMX_SSO_ERR_ENB);
	sso_err_enb.cn68xx.fpe_ie = 0;
	cvmx_write_csr(CVMX_SSO_ERR_ENB, sso_err_enb.u64);

	sso_cfg.u64 = cvmx_read_csr(CVMX_SSO_CFG);

	if (sso_cfg.s.rwen == 0) {
		cvmx_dprintf("WARNING: %s: SSO already down\n", __func__);
		return 0;
	}

	sso_cfg.s.rwen = 0;
	sso_cfg.s.rwq_byp_dis = 1;
	cvmx_write_csr(CVMX_SSO_CFG, sso_cfg.u64);
	cvmx_read_csr(CVMX_SSO_CFG);
	queue_available.u64 = cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(8));

	/* Make CVMX_FPA_QUEX_AVAILABLE(8) % 16 == 0 */
	for (num_to_transfer = (16 - queue_available.s.que_siz) % 16; num_to_transfer > 0; num_to_transfer--) {
		do {
			pop_fptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_POP_FPTR);
		} while (!pop_fptr.s.val);
		for (;;) {
			fptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_PSH_FPTR);
			if (!fptr.s.full)
				break;
			cvmx_wait(1000);
		}
		fptr.s.fptr = pop_fptr.s.fptr;
		cvmx_write_csr(CVMX_SSO_RWQ_PSH_FPTR, fptr.u64);
	}
	cvmx_read_csr(CVMX_SSO_CFG);

	do {
		queue_available.u64 = cvmx_read_csr(CVMX_FPA_QUEX_AVAILABLE(8));
	} while (queue_available.s.que_siz % 16);

	sso_cfg.s.rwen = 1;
	sso_cfg.s.rwq_byp_dis = 0;
	cvmx_write_csr(CVMX_SSO_CFG, sso_cfg.u64);

	for (i = 0; i < 8; i++) {
		cvmx_sso_rwq_head_ptrx_t head_ptr;
		cvmx_sso_rwq_tail_ptrx_t tail_ptr;

		head_ptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_HEAD_PTRX(i));
		tail_ptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_TAIL_PTRX(i));
		if (head_ptr.s.ptr != tail_ptr.s.ptr) {
			cvmx_dprintf("head_ptr.s.ptr != tail_ptr.s.ptr, idx: %d\n", i);
		}

		paddr = ((uint64_t) head_ptr.s.ptr) << 7;
		__cvmx_bootmem_phy_free(paddr, rwq_size, 0);
	}

	do {
		do {
			pop_fptr.u64 = cvmx_read_csr(CVMX_SSO_RWQ_POP_FPTR);
			if (pop_fptr.s.val == 0)
				continue;
			paddr = ((uint64_t) pop_fptr.s.fptr) << 7;
			__cvmx_bootmem_phy_free(paddr, rwq_size, 0);
		} while (pop_fptr.s.val);
		fpage_cnt.u64 = cvmx_read_csr(CVMX_SSO_FPAGE_CNT);
	} while (fpage_cnt.s.fpage_cnt);

	sso_cfg.s.rwen = 0;
	sso_cfg.s.rwq_byp_dis = 0;
	cvmx_write_csr(CVMX_SSO_CFG, sso_cfg.u64);

	return 0;
}

/**
 * @INTERNAL
 *
 * Shutdown the SSO on the current node and release all its buffers
 *
 */
static int cvmx_sso3_shutdown(unsigned node)
{
	unsigned i, retries;
	unsigned laura;
	cvmx_fpa3_gaura_t aura;
	cvmx_sso_aw_cfg_t aw_cfg;
	cvmx_sso_aw_status_t aw_status;
	cvmx_sso_xaq_aura_t aura_reg;
	cvmx_sso_page_cnt_t page_cnt;
	cvmx_sso_grpx_aq_cnt_t aq_cnt;
	const unsigned cache_lines =
		__CVMX_SSO_XAQ_BUF_SIZE / CVMX_CACHE_LINE_SIZE;

	aw_cfg.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_CFG);
	if (aw_cfg.s.rwen == 0) {
		cvmx_dprintf("WARNING: %s: SSO already down\n",__func__);
		return 0;
	}

	aw_status.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_STATUS);

	if (debug)
		cvmx_dprintf("%s: total WQE count = %u cached = %u\n", __func__,
			(unsigned) cvmx_sso3_get_wqe_count(node),
			(unsigned) aw_status.s.xaq_buf_cached);

	/* Get the aura number and node, keep aura node separate from node */
	aura_reg.u64 = cvmx_read_csr_node(node, CVMX_SSO_XAQ_AURA);
	aura = __cvmx_fpa3_gaura(node, laura = aura_reg.s.laura);

	/* No need to disable error interrupts, here */
	aw_cfg.s.xaq_byp_dis = 1;
	aw_cfg.s.xaq_alloc_dis = 1;
	aw_cfg.s.rwen = 0;
	cvmx_write_csr_node(node, CVMX_SSO_AW_CFG, aw_cfg.u64);

	/* Make sure all free buffers drained */
	retries = 0x10000;
	do {
		aw_status.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_STATUS);
		cvmx_wait(1000);
	} while (aw_status.s.xaq_buf_cached && (retries-- > 0) );

	if (retries == 0) {
		cvmx_dprintf("%s: error stopping SSO\n", __FUNCTION__);
		return -1;
	}

	/* remaining SSO pointers in the system */
	page_cnt.s.cnt = cvmx_read_csr_node(node, CVMX_SSO_PAGE_CNT);

	if(debug)
		cvmx_dprintf("%s: freeing bufs, page_cnt=%u, "
			"xaq_buf_cached=%u\n",
			__func__,
			(unsigned) page_cnt.s.cnt,
			(unsigned) aw_status.s.xaq_buf_cached);

	 /* One buffer per group for initial head and tail */
	 for (i = 0; i < cvmx_sso_num_xgrp(); i++) {
		cvmx_sso_xaqx_head_ptr_t hptr;
		cvmx_sso_xaqx_tail_ptr_t tptr;
		cvmx_sso_xaqx_head_next_t hnxt;
		cvmx_sso_xaqx_tail_next_t tnxt;

		/* extract all pointers */
		hptr.u64 = cvmx_read_csr_node(node, CVMX_SSO_XAQX_HEAD_PTR(i));
		tptr.u64 = cvmx_read_csr_node(node, CVMX_SSO_XAQX_TAIL_PTR(i));
		hnxt.u64 = cvmx_read_csr_node(node, CVMX_SSO_XAQX_HEAD_NEXT(i));
		tnxt.u64 = cvmx_read_csr_node(node, CVMX_SSO_XAQX_TAIL_NEXT(i));
		aq_cnt.u64 = cvmx_read_csr_node(node,CVMX_SSO_GRPX_AQ_CNT(i));

		if(debug)
			cvmx_dprintf("%s: group %u "
				"HEAD_PTR %#llx "
				"TAILPTR %#llx "
				"HEAD_NEXT %#llx "
				"TAIL_NEXT %#llx "
				"AQ_CNT %u\n",
				__func__, i,
				(unsigned long long) hptr.u64,
				(unsigned long long) tptr.u64,
				(unsigned long long) hnxt.u64,
				(unsigned long long) tnxt.u64,
				(unsigned) aq_cnt.u64);

		/* Make sure the SSO group has no pending entry */
		if (aq_cnt.s.aq_cnt != 0)
			cvmx_dprintf("%s: ERROR: "
				"could not stop SSO group %d, AQ_CNT=%u\n",
				__func__, i, (unsigned) aq_cnt.s.aq_cnt);

		/* XXX
		 * This is still not in line with HRM Sec 9.12.3
		 * pseudo-code, need to revisit it on the silicon
		 * and then maybe have the simulator agree with RTL
		 */
		if (hptr.s.ptr == tptr.s.ptr) {
			uint64_t addr;
			void * ptr;
			/* Free the head/tail buffer */
			addr = hptr.s.ptr;
			addr <<= 7;
			ptr = cvmx_phys_to_ptr(addr);
			if (addr == 0)
				continue;
			cvmx_fpa3_free(ptr, aura, cache_lines);
			/* Clear pointers */
			cvmx_write_csr_node(node,
				CVMX_SSO_XAQX_HEAD_PTR(i), 0);
			cvmx_write_csr_node(node,
				CVMX_SSO_XAQX_HEAD_NEXT(i), 0);
			cvmx_write_csr_node(node,
				CVMX_SSO_XAQX_TAIL_NEXT(i), 0);
			cvmx_write_csr_node(node,
				CVMX_SSO_XAQX_TAIL_PTR(i), 0);
		}
	}

	/* Wait for FPA pipeline to clear */
	cvmx_wait(1000);

	/* there should not be any remaining pointers at this point */
	if (page_cnt.s.cnt != 0 ) {
		cvmx_dprintf("ERROR: "
			"%s failed to free buffers, remaining=%llu\n",
			__func__, (unsigned long long) page_cnt.s.cnt);
		return -1;
	}
	if(debug) {
		cvmx_sso_aw_we_t aw_we;
		cvmx_sso_taq_cnt_t taq_cnt;

		cvmx_dprintf("%s: done, page_cnt=%u\n", __func__,
			(unsigned) page_cnt.s.cnt);

		aw_we.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_WE);
		taq_cnt.u64 = cvmx_read_csr_node(node, CVMX_SSO_TAQ_CNT);

		cvmx_dprintf("%s: Reserved IAQ %u TAQ %u\n",
			__func__, aw_we.s.rsvd_free, taq_cnt.s.rsvd_free);
	}

	/* Release the aura and pool too */
	cvmx_fpa3_shutdown_aura_and_pool(aura);

	return 0;
}

/**
 * Shutdown the SSO in a specific node
 *
 * Frees associated buffers and turns off the block.
 * It is implemented for CN68XX and CN78XX models.
 */
int cvmx_helper_uninitialize_sso_node(unsigned node)
{
        if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		return cvmx_sso3_shutdown(node);
	else if (octeon_has_feature(OCTEON_FEATURE_CN68XX_WQE))
		return cvmx_uninitialize_sso_68xx();
	else
		return 0;	/* nothing to do for old POW */
}

/**
 * Shutdown the SSO
 *
 * Frees associated buffers and turns off the block.
 * It is implemented for CN68XX and CN78XX models.
 */
int cvmx_helper_uninitialize_sso(void)
	{
	unsigned node = cvmx_get_node_num();
	return cvmx_helper_uninitialize_sso_node(node);
	}

/**
 * @INTERNAL
 *
 * Obtain FPA default buffer size and AURA
 * for software-created WQEs from current PKI
 * configuration
 */
static int cvmx_helper_pki_default_aura(cvmx_fpa3_gaura_t *gaura,
	unsigned *wqe_skip, unsigned *data_skip)
{
	cvmx_pki_qpg_tblx_t qpg_tbl;
	cvmx_pki_stylex_buf_t stylex_buf;
	cvmx_pki_clx_stylex_cfg_t stylex_cfg;
	unsigned node = cvmx_get_node_num();
	const unsigned default_style = 0;
	const unsigned default_cluster = 0;
	unsigned qpg_index;
	int buf_sz;

	stylex_cfg.u64 = cvmx_read_csr_node(node,
		CVMX_PKI_CLX_STYLEX_CFG(default_style, default_cluster));
	stylex_buf.u64 = cvmx_read_csr_node(node,
		CVMX_PKI_STYLEX_BUF(default_style));

	qpg_index = stylex_cfg.s.qpg_base;

	qpg_tbl.u64 = cvmx_read_csr_node(node,
		 CVMX_PKI_QPG_TBLX(qpg_index));

	*gaura = __cvmx_fpa3_gaura(node, qpg_tbl.s.laura);
	buf_sz = stylex_buf.s.mb_size << 3;
	*data_skip = stylex_buf.s.first_skip << 3;
	*wqe_skip = stylex_buf.s.wqe_skip << 7;

	return buf_sz;
}

static int cvmx_helper_ipd_default_pool(unsigned *pool)
{
	cvmx_ipd_packet_mbuff_size_t mbuff_size;
	cvmx_ipd_ctl_status_t ipd_ctl = {0};

	if (! octeon_has_feature(OCTEON_FEATURE_NO_WPTR)) {
		*pool = cvmx_ipd_cfg.packet_pool.pool_num;
		return cvmx_ipd_cfg.packet_pool.buffer_size ;
	}

	mbuff_size.u64 = cvmx_read_csr(CVMX_IPD_PACKET_MBUFF_SIZE);
	ipd_ctl.u64 = cvmx_read_csr(CVMX_IPD_CTL_STATUS);
	if (!ipd_ctl.s.no_wptr)
		*pool = cvmx_read_csr(CVMX_IPD_WQE_FPA_QUEUE) & 7;

	return mbuff_size.s.mb_size << 3;
}

/**
 * Allocate a work-queue entry for delivering software-initiated
 * event notifications.
 * The application data is copied into the work-queue entry,
 * if the space is sufficient.
 */
cvmx_wqe_t * cvmx_wqe_soft_create(void *data_p, unsigned data_sz)
{
	void *wqe_ptr = NULL, *data_ptr;
	unsigned pool = 0;
	unsigned wqe_skip = 0, data_skip = 0;
	cvmx_fpa3_gaura_t aura = CVMX_FPA3_INVALID_GAURA;
	unsigned buf_sz = 128;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		buf_sz = cvmx_helper_pki_default_aura(
			&aura, &wqe_skip, &data_skip);
		if (cvmx_unlikely(data_skip == 0))
			data_skip = sizeof(cvmx_wqe_78xx_t);
		buf_sz -= data_skip;
		if(buf_sz >= data_sz)
			wqe_ptr = cvmx_fpa3_alloc(aura);
	} else {
		buf_sz = cvmx_helper_ipd_default_pool(&pool);
		data_skip = offsetof(cvmx_wqe_t, packet_data);
		buf_sz -= data_skip;
		if(buf_sz >= data_sz)
			wqe_ptr = cvmx_fpa1_alloc(pool);
	}

	if(wqe_ptr == NULL)
		return NULL;

	data_ptr = wqe_ptr + data_skip;

	if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		cvmx_wqe_78xx_t *wqe = wqe_ptr + wqe_skip;
		wqe->word0.u64 = 0;
		wqe->word1.u64 = 0;
		wqe->word2.u64 = 0;
		wqe->word4.u64 = 0;
		wqe->word0.bufs = 1;
		wqe->word2.software = 1;
		wqe->word0.aura = aura.node << 10 | aura.laura;
		wqe->packet_ptr.u64 = 0;
		if (data_sz > 0) {
			wqe->packet_ptr.size = data_sz;
			wqe->packet_ptr.addr = cvmx_ptr_to_phys(data_ptr);
			memcpy(data_ptr, data_p, data_sz);
		}
		cvmx_wqe_set_len(wqe_ptr + wqe_skip, data_sz);
	} else {
		cvmx_wqe_t *work = wqe_ptr + wqe_skip;
		work->word0.u64 = 0;
		work->word1.u64 = 0;
		work->word2.u64 = 0;
		work->word2.s.software = 1;
		work->packet_ptr.u64 = 0;
		work->packet_ptr.s.pool = pool;
		if (data_sz > 0) {
			work->packet_ptr.s.size = data_sz;
			work->packet_ptr.s.addr = cvmx_ptr_to_phys(data_ptr);
			memcpy(data_ptr, data_p, data_sz);
		}
		cvmx_wqe_set_len(work, data_sz);
	}


	return wqe_ptr + wqe_skip;
}

#endif	/* CVMX_BUILD_FOR_LINUX_HOST */

#ifdef CVMX_DUMP_SSO
/*
 * Show SSO integrated configuration.
 * See function prototype in cvmx-pow.h
 */
#define SSO_PRN_HEADLEN  32 
#define SSO_PRN_DATALEN  48
#define SSO_PRN_LINELEN  (SSO_PRN_HEADLEN + SSO_PRN_DATALEN)
#define SSO_PRN_DPLEN(__n)  (SSO_PRN_DATALEN / __n)

#define DLMPRINT(__format, ...) \
do { \
	int __n; \
	sprintf(lines[1], __format, ## __VA_ARGS__); \
	__n = SSO_PRN_LINELEN - strlen(lines[1]) - 1; \
	memset(lines[0], '-', __n);  lines[0][__n] = '\0'; \
	cvmx_printf("%s %s\n", lines[0], lines[1]); \
} while (0)

#define PARPRINT(__offs, __head, __format, ...) \
do {\
	cvmx_printf("%*s%-*s", __offs, "", SSO_PRN_HEADLEN - __offs, __head); \
	cvmx_printf(__format, ## __VA_ARGS__); \
} while (0)

int cvmx_sso_config_dump(unsigned int node)
{
	int group, base, core, gcore, set, i, k;
	cvmx_sso_aw_we_t awwe;
	cvmx_sso_gwe_cfg_t gwecfg;
	cvmx_sso_grpx_pri_t grppri;
	cvmx_sso_grpx_iaq_thr_t iaqthr;
	cvmx_sso_grpx_taq_thr_t taqthr;
	cvmx_sso_ppx_sx_grpmskx_t grpmsk;
	cvmx_sysinfo_t *sysinfo;
	uint64_t coremasks[2];
	uint32_t crc32, pcrc32;
	char lines[4][256];

	if (!OCTEON_IS_MODEL(OCTEON_CN7XXX))
		return 0;

	sysinfo = cvmx_sysinfo_get();
	memset(lines[3], '*', SSO_PRN_LINELEN);  lines[3][SSO_PRN_LINELEN] = '\0';
	cvmx_printf("\n%s\n", lines[3]);
	cvmx_printf("   SSO Configuration (Node %d)\n", node);
	cvmx_printf("%s\n", lines[3]);
	awwe.u64 = cvmx_read_csr_node(node, CVMX_SSO_AW_WE);
	gwecfg.u64 = cvmx_read_csr_node(node, CVMX_SSO_GWE_CFG);
	PARPRINT(0, "Reserved free entires", "%*d\n", SSO_PRN_DPLEN(1), awwe.s.rsvd_free);
	PARPRINT(0, "Total free entires", "%*d\n", SSO_PRN_DPLEN(1), awwe.s.free_cnt);
	DLMPRINT("Groups:");
	for (group = 0, pcrc32 = 0, base = 0; group < (int)cvmx_sso_num_xgrp(); group++) {
		CVMX_MT_CRC_POLYNOMIAL(0x1edc6f41);
		CVMX_MT_CRC_IV(0xffffffff);
		grppri.u64 = cvmx_read_csr_node(node, CVMX_SSO_GRPX_PRI(group));
		CVMX_MT_CRC_DWORD(grppri.u64);
		iaqthr.u64 = cvmx_read_csr_node(node, CVMX_SSO_GRPX_IAQ_THR(group));
		CVMX_MT_CRC_DWORD(iaqthr.u64);
		taqthr.u64 = cvmx_read_csr_node(node, CVMX_SSO_GRPX_TAQ_THR(group));
		CVMX_MT_CRC_DWORD(taqthr.u64);
		coremasks[0] = coremasks[1] = 0;
		i = group / 64;
		k = group % 64;
		for (set = 0; set < 2; set++) {
			for (core = 0; core < (int)cvmx_octeon_num_cores(); core++) {
				gcore = node << CVMX_NODE_NO_SHIFT | core;
				if (cvmx_coremask_is_core_set(&sysinfo->core_mask, gcore) == 0)
					continue;
				grpmsk.u64 = cvmx_read_csr_node(node, CVMX_SSO_PPX_SX_GRPMSKX(core, set, i));
				if ((1ull << k) & grpmsk.s.grp_msk)
					coremasks[set] |= (1ull << core);
			}
			CVMX_MT_CRC_DWORD(coremasks[set]);
		}
		CVMX_MF_CRC_IV(crc32);
		if (crc32 == pcrc32)
			continue;
		if (group > 0 && (group - 1) != base)
			cvmx_printf("\nGROUP(s) %02d-%02d -- same as GROUP %02d\n",
				group - 1, base + 1, base);
		pcrc32 = crc32;
		base = group;
		cvmx_printf("Group %d:\n", group);
		PARPRINT(3, "Priority", "%*d\n", SSO_PRN_DPLEN(1), grppri.s.pri);
		if (grppri.s.affinity != 0)
			PARPRINT(3, "Affinity", "%*d\n", SSO_PRN_DPLEN(1), grppri.s.affinity);
		if (gwecfg.cn78xx.dis_wgt_credit == 0) /* All CN7XXX have the same register. */
			PARPRINT(3, "Arb.Weight", "%*d\n", SSO_PRN_DPLEN(1), grppri.s.weight);
		PARPRINT(3, "Coremask(Set0)", "%*llx\n", SSO_PRN_DPLEN(1), (long long)coremasks[0]);
		PARPRINT(3, "Coremask(Set1)", "%*llx\n", SSO_PRN_DPLEN(1), (long long)coremasks[1]);
		PARPRINT(3, "", "%*s%*s%*s%*s\n", SSO_PRN_DPLEN(4), "",
			SSO_PRN_DPLEN(4), "CNT", SSO_PRN_DPLEN(4), "RSRD", SSO_PRN_DPLEN(4), "MAX");
		PARPRINT(3, "IAQ Entries", "%*s%*d%*d%*d\n", SSO_PRN_DPLEN(4), "",
			SSO_PRN_DPLEN(4), iaqthr.s.grp_cnt, SSO_PRN_DPLEN(4),iaqthr.s.rsvd_thr,
			SSO_PRN_DPLEN(4), iaqthr.s.max_thr);
		PARPRINT(3, "TAQ Entries", "%*s%*d%*d%*d\n", SSO_PRN_DPLEN(4), "",
			SSO_PRN_DPLEN(4), taqthr.s.grp_cnt, SSO_PRN_DPLEN(4),taqthr.s.rsvd_thr,
			SSO_PRN_DPLEN(4), taqthr.s.max_thr);
	}
	if ((group - 1) != base)
		cvmx_printf("\nGROUP(s) %02d-%02d -- same as GROUP %02d\n",
			group - 1, base + 1, base);
	return 0;
}

#endif /* CVMX_DUMP_SSO */

