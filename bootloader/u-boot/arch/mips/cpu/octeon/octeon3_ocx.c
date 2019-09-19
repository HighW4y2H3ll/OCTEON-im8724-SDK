/***********************license start************************************
 * Copyright (c) 2014 Cavium, Inc. <support@cavium.com>.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

/* file ocx-setup.c
 *
 * called inside of board_init_f() so:
 *	- no global variables are allowed
 *	- no printfs are allowed
 */

#include <common.h>
#include <linux/types.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-ocx-defs.h>
#include <asm/arch/cvmx-coremask.h>
#include <asm/arch/lib_octeon.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

#define MAX_LINKS 3

struct lk_info {
	/** Node ID at end of link */
	cvmx_ocx_com_node_t		node;
	/** Remote node link info */
	cvmx_ocx_com_linkx_ctl_t	ctl[MAX_LINKS];
	/** Unique values received over links */
	uint64_t			unique_value[MAX_LINKS];
};

extern void init_ccpi_early(bool is_master);
extern int init_ccpi_connection2(int is_master, uint64_t gbaud, bool trace);

/**
 * Add a node to the exists mask
 *
 * @param node   Node to add
 */
static inline void cvmx_numa_set_exists(int node)
{
	gd->arch.node_mask |= (1 << node);
}

/**
 * Get a bitmask of the nodes that exist
 *
 * @return bitmask
 */
static inline uint64_t cvmx_numa_get_exists_mask(void)
{
	return gd->arch.node_mask;
}

/**
 * Return true if a node exists
 *
 * @param node   Node to check
 *
 * @return Non zero if the node exists
 */
static inline int cvmx_numa_exists(int node)
{
	return gd->arch.node_mask & (1 << node);
}

/**
 * Return true if there is only one node
 *
 * @return
 */
static inline int cvmx_numa_is_only_one(void)
{
	return gd->arch.node_mask == 1;
}

/**
 * During OCX link enumeration, we need to write unique values to
 * OCX_TLKX_LNK_DATA so that we can determine which links connects
 * to which node. This function creates these unique values. When
 * making values for the local node, use local_link as -1.
 *
 * @param local_link The node where we write the value is connected to
 *			local_link". If we're working on the local node, use -1.
 * @param remote_link
 *                   	Which link on the remote node is this value for (0-3)
 *
 * @return Unique value that can be sent using OCX_TLKX_LNK_DATA, then received
 *         		with OCX_RLKX_LNK_DATA.
 */
static inline uint64_t ocx_unique_key(int local_link, int remote_link)
{
	uint64_t result = 0xabcdull << 32;
	result |= (local_link & 0xff) << 16;
	result |= remote_link & 0xff;
	return result;
}

/**
 * Return if the link at the suplied index points to a node that we've already
 * discovered. This happens if two links connect to the same node for more
 * bandwidth.
 *
 * @param unique_value
 *               Conenction data for finding nodes
 * @param index  Index into unique_value that we are seeing if its a duplicate
 *
 * @return Index of the duplicate or -1 if its not a duplicate
 */
static int ocx_duplicate_node(struct lk_info *lk_info, int index)
{
	int link;
	struct lk_info *li;

	for (link = 0; link < index; link++) {
		li = &lk_info[link];
		if ((li->unique_value[0] == lk_info[index].unique_value[0]) &&
		    (li->unique_value[1] == lk_info[index].unique_value[1]) &&
		    (li->unique_value[2] == lk_info[index].unique_value[2]))
			return link;
	}
	return -1;
}

/**
 * This function reads CSRs for a remote node before OCX is completely
 * setup. It should be used during node bringup, but not after L2 is aware of
 * the other nodes.
 *
 * @param node    Node ID to read from
 * @param address CSR address to read
 *
 * @return Value read
 */
static uint64_t ocx_pp_read(int node, uint64_t address)
{
	if (node == 0) {
		return cvmx_read_csr(address);
	} else {
		uint64_t data;
		cvmx_ocx_pp_cmd_t pp_cmd;

		address |= (uint64_t)node << CVMX_NODE_IO_SHIFT;
		pp_cmd.u64 = 0;
		pp_cmd.s.ld_cmd = 3; /* 8 byte load */
		pp_cmd.s.ld_op = 1;
		pp_cmd.s.addr = address;
		cvmx_write_csr(CVMX_OCX_PP_RD_DATA, -1);
		cvmx_write_csr(CVMX_OCX_PP_CMD, pp_cmd.u64);

		do {
			data = cvmx_read_csr(CVMX_OCX_PP_RD_DATA);
		} while (data == (uint64_t)-1);

		return data;
	}
}

/**
 * This function writes CSRs for a remote node before OCX is completely
 * setup. It should be used during node bringup, but not after L2 is aware of
 * the other nodes.
 *
 * @param node    Node ID to write to
 * @param address CSR address to write
 * @param data    Data to write
 */
static void ocx_pp_write(int node, uint64_t address, uint64_t data)
{
	if (node == 0) {
		cvmx_write_csr(address, data);
	} else {
		cvmx_ocx_pp_cmd_t pp_cmd;

		address |= (uint64_t)node << CVMX_NODE_IO_SHIFT;
		pp_cmd.u64 = 0;
		pp_cmd.s.wr_mask = 0xff;
		pp_cmd.s.addr = address;
		cvmx_write_csr(CVMX_OCX_PP_WR_DATA, data);
		cvmx_write_csr(CVMX_OCX_PP_CMD, pp_cmd.u64);
	}
}

/**
 * Initialize the OCI so all Nodes are ready to be used
 *
 * @return Zero on success, negative on failure.
 */
static int init_oci(void)
{
	int link, rlink, ll, rl, rid;
	int node, unused_node;
	uint64_t exists_mask;
	/* Index MAX_LINKS is used for the local node */
	struct lk_info lk_info[MAX_LINKS + 1];
	struct lk_info *li;
	struct lk_info *lim = &lk_info[MAX_LINKS];
	union cvmx_ocx_com_node com_node;
	union cvmx_ocx_com_linkx_ctl link_ctl;
	union cvmx_rst_boot rst_boot;
	union cvmx_rst_ocx rst_ocx;
	union cvmx_l2c_oci_ctl l2c_oci_ctl;

	memset(lk_info, 0, sizeof(lk_info));

	debug("%s()\n", __func__);
	/* Only one node should be up (the one I'm on).
	 * Set its ID to be fixed.
	 */
	cvmx_numa_set_exists(0);

	l2c_oci_ctl.u64 = cvmx_read_csr(CVMX_L2C_OCI_CTL);
	debug("%s: Starting node mask: 0x%x\n", __func__, l2c_oci_ctl.s.enaoci);
	if (l2c_oci_ctl.s.enaoci != 0) {
		debug("%s: OCI already initialized, ID node mask: 0x%x\n",
		      __func__, l2c_oci_ctl.s.enaoci);
		for (ll = 0; ll < 4; ll++) {
			if (l2c_oci_ctl.s.enaoci & (1 << ll))
				cvmx_numa_set_exists(ll);
		}
		return 0;
	}

	com_node.u64 = cvmx_read_csr(CVMX_OCX_COM_NODE);
	com_node.s.fixed = 1;
	cvmx_write_csr(CVMX_OCX_COM_NODE, com_node.u64);

	/* Write a unique value to OCX_TLKX_LNK_DATA for every possible link.
	 * This allows us to later figure out which link goes where.  Also mark
	 * all link as unrecoverable so its state can't change later
	 */
	debug("Loop through local links sending unique values over OCX_TLKX_LNK_DATA\n");
	for (link = 0; link < MAX_LINKS; link++) {
		uint64_t local_unique;

		li = &lk_info[link];
		/* Don't allow this link to recover if it goes down. Once up
		 * links should stay up
		 */
		link_ctl.u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(link));
		link_ctl.s.auto_clr = 0;
		cvmx_write_csr(CVMX_OCX_COM_LINKX_CTL(link), link_ctl.u64);

		/* Get the link state again. It could have changed during the
		 * modification of AUTO_CLR
		 */
		lim->ctl[link].u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(link));

		/* Skip invalid links */
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up) {
			debug("    Local link %d: Down, skipping\n", link);
			continue;
		}

		/* Write a unique value so ee can see where this link connects
		 * to
		 */
		local_unique = ocx_unique_key(-1, link);
		cvmx_write_csr(CVMX_OCX_TLKX_LNK_DATA(link), local_unique);
		debug("    Local link %d: Write link data 0x%llx\n",
		      link, local_unique);
		rid = lim->ctl[link].s.id;

		/* Loop through possible remote links */
		for (rlink = 0; rlink < MAX_LINKS; rlink++) {
			uint64_t remote_unique;
			cvmx_ocx_com_linkx_ctl_t *lnk = &li->ctl[rlink];

			/* Read the links state and make sure it doesn't auto
			 * recover from errors
			 */
			lnk->u64 = ocx_pp_read(rid,
					       CVMX_OCX_COM_LINKX_CTL(rlink));
			lnk->s.auto_clr = 0;
			ocx_pp_write(rid,
				     CVMX_OCX_COM_LINKX_CTL(rlink), lnk->u64);
			/* Skip invalid links */
			if (!lnk->s.valid || !lnk->s.up) {
				debug("       Remote link %d: Down, skipping\n",
				      rlink);
				continue;
			}

			/* Write a unique value so we can see where the remote
			 * link connects to
			 */
			remote_unique = ocx_unique_key(link, rlink);
			debug("       Remote link %d: Write link data 0x%llx\n",
			      rlink, remote_unique);
			ocx_pp_write(rid, CVMX_OCX_TLKX_LNK_DATA(rlink),
				     remote_unique);
		}
	}

	debug("Reading link data for all links\n");
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up)
			continue;

		lim->unique_value[link] =
				cvmx_read_csr(CVMX_OCX_RLKX_LNK_DATA(link));
		debug("    Local link %d: Read link data 0x%llx\n",
		      link, lim->unique_value[link]);
		rid = lim->ctl[link].s.id;

		for (rlink = 0; rlink < MAX_LINKS; rlink++) {
			li->unique_value[rlink] =
 				ocx_pp_read(rid, CVMX_OCX_RLKX_LNK_DATA(rlink));
			debug("        Remote link %d: Read link data 0x%llx\n",
			      rlink, li->unique_value[rlink]);
		}
		li->node.u64 = ocx_pp_read(rid, CVMX_OCX_COM_NODE);
	}

	debug("Finding fixed node IDs\n");
	/* Loop through once reserving all fixed node IDs */
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up)
			continue;

		if (ocx_duplicate_node(lk_info, link) != -1) {
			debug("    Local link %d: Duplicate node, skipping\n",
			      link);
			continue;
		}
		if (li->node.s.fixed) {
			rid = li->node.s.id;
			/* Mark fixed nodes as existing so we don't reuse their
			 * node ID
			 */
			if (cvmx_numa_exists(rid)) {
				debug("Fixed ID %d conflicts with existing node\n",
				      rid);
			}
			cvmx_numa_set_exists(rid);
			debug("    Local link %d: Fixed node ID %d\n",
			      link, rid);
		}
	}

	debug("Assigning node IDs\n");
	/* Loop through again finding node IDs for unassigned nodes */
	for (link = 0; link < MAX_LINKS; link++) {
		int dup;
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up)
			continue;

		dup = ocx_duplicate_node(lk_info, link);
		if (dup != -1) {
			debug("    Local link %d: Duplicate node, skipping\n",
			      link);
			li->node = lk_info[dup].node;
			continue;
		}
		if (!li->node.s.fixed) {
			/* Find a clear exists bit */
			int node;
			for (node = 0; node < CVMX_MAX_NODES; node++) {
				if (!cvmx_numa_exists(node))
					break;
			}
			if (node >= CVMX_MAX_NODES) {
				debug("Somehow we found more nodes than we support.  Skipping new node");
				return -1;
			}
			li->node.s.fixed = 1;
			li->node.s.id = node;
			cvmx_numa_set_exists(node);	/* add node to mask */
			debug("    Local link %d: Assigned node ID %d\n",
			      link, node);
		}
	}

	/* Find an unused node number.  This will be used for links that are
	 * down
	 */
	for (unused_node = 0; unused_node < CVMX_MAX_NODES; unused_node++) {
		if (!cvmx_numa_exists(unused_node))
			break;
	}

	debug("Determining which node each link connects to\n");
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up) {
			li->node.s.id = unused_node;
			continue;
		}
		debug("    Local link %d: Searching remote links\n", link);
		for (rlink = 0; rlink < MAX_LINKS; rlink++) {
			uint64_t search;
			int found = 0;
			if (!li->ctl[rlink].s.valid || !li->ctl[rlink].s.up) {
				li->ctl[rlink].s.id = unused_node;
				continue;
			}
			search = li->unique_value[rlink] & 0xfffffffffffffffull;
			/* debug("       Remote link %d: Looking for 0x%lx\n",
				rlink, search); */
			for (ll = -1; ll < MAX_LINKS; ll++) {
				for (rl = 0; rl < MAX_LINKS; rl++) {
					uint64_t runique;
					runique = ocx_unique_key(ll, rl);
					/* debug(
					"       Checking [%d][%d] 0x%lx\n",
						ll, rl, runique); */
					if (search == runique) {
						int node2 = (ll == -1) ?
							0 : lk_info[ll].node.s.id;
						debug("        Node ID: %d, link: %d => Node ID: %d, link: %d\n",
						      li->node.s.id,
						      rlink, node2, rl);
						li->ctl[rlink].s.id = node2;
						found = 1;
					}
				}
			}
			if (!found) {
				debug("        Node ID: %d, link: %d => Unknown\n",
				      li->node.s.id, rlink);
			}
		}
	}

	debug("Programming remote links and node IDs\n");
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up)
			continue;

		if (ocx_duplicate_node(lk_info, link) != -1) {
			debug("    Local link %d: Duplicate node, skipping\n",
				link);
			continue;
		}
		debug("    Local link %d: Assign node ID %d\n",
				link, li->node.s.id);
		rid = lim->ctl[link].s.id;

		ocx_pp_write(rid, CVMX_OCX_COM_NODE, li->node.u64);
		for (rlink = 0; rlink < MAX_LINKS; rlink++) {
			if (!li->ctl[rlink].s.valid || !li->ctl[rlink].s.up) {
				debug("        Remote link %d: Down\n", rlink);
			} else {
				debug("        Remote link %d: Connects to node ID %d\n",
				      rlink, li->ctl[rlink].s.id);
			}
			ocx_pp_write(rid, CVMX_OCX_COM_LINKX_CTL(rlink),
				    li->ctl[rlink].u64);
		}
	}

	debug("Programming local links\n");
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		if (!lim->ctl[link].s.valid || !lim->ctl[link].s.up) {
			debug("    Local link %d: Down\n", link);
		} else {
			debug("    Local link %d: Connects to node ID %d\n",
				link, li->node.s.id);
		}
		link_ctl.u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(link));
		link_ctl.s.id = li->node.s.id;
		cvmx_write_csr(CVMX_OCX_COM_LINKX_CTL(link), link_ctl.u64);
	}

	debug("Checking the PP_CMD still works\n");
	for (link = 0; link < MAX_LINKS; link++) {
		li = &lk_info[link];
		link_ctl.u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(link));
		if (!link_ctl.s.valid || !link_ctl.s.up)
			continue;

		debug("    Local link %d: Checking\n", link);
		if (link_ctl.s.id != li->node.s.id) {
			debug("        Failed: Local link ID %d doesn't match expect node ID %d\n",
			      link_ctl.s.id, li->node.s.id);
		}
		com_node.u64 =
			ocx_pp_read(link_ctl.s.id, CVMX_OCX_COM_NODE);
		if (com_node.s.fixed && (com_node.s.id == li->node.s.id) &&
		    link_ctl.s.id == li->node.s.id) {
			debug("        Passed\n");
		} else {
			debug("        Failed\n");
		}
	}

	/* All OCX links are up and running. Now tell local L2 that OCX is OK */
	debug("Configuring L2 for OCX on all nodes\n");
	exists_mask = cvmx_numa_get_exists_mask();
	for (node = 0; node < CVMX_MAX_NODES; node++) {
		if (node == 0) {
			l2c_oci_ctl.u64 = cvmx_read_csr(CVMX_L2C_OCI_CTL);
			l2c_oci_ctl.s.iofrcl = 0;
			l2c_oci_ctl.s.gksegnode = 0;
			l2c_oci_ctl.s.enaoci = exists_mask;
			cvmx_write_csr(CVMX_L2C_OCI_CTL, l2c_oci_ctl.u64);
		} else if (cvmx_numa_exists(node)) {
			l2c_oci_ctl.u64 = ocx_pp_read(node, CVMX_L2C_OCI_CTL);
			l2c_oci_ctl.s.iofrcl = 0;
			l2c_oci_ctl.s.gksegnode = node;
			l2c_oci_ctl.s.enaoci = exists_mask;
			ocx_pp_write(node, CVMX_L2C_OCI_CTL, l2c_oci_ctl.u64);
		}
	}

	/* Configure node 0 to reset if any of the current OCX links go down */
	for (node = 0; node < CVMX_MAX_NODES; node++) {
		if (!cvmx_numa_exists(node) || (node == 0))
			continue;

		rst_boot.u64 = cvmx_read_csr_node(node, CVMX_RST_BOOT);
		rst_ocx.u64 = cvmx_read_csr_node(node, CVMX_RST_OCX);
		for (link = 0; link < MAX_LINKS; link++) {
			link_ctl.u64 = cvmx_read_csr_node(node,
							  CVMX_OCX_COM_LINKX_CTL(link));
			if (!link_ctl.s.valid || !link_ctl.s.up)
				continue;
			rst_boot.s.lboot_oci |= (1 << link);
			/* Don't clear LBOOT_EXT or LBOOT */
			rst_boot.s.lboot_ext = 0;
			rst_boot.s.lboot = 0;
			cvmx_write_csr_node(node, CVMX_RST_BOOT, rst_boot.u64);
			rst_ocx.s.rst_link |= (1 << link);
			cvmx_write_csr_node(node, CVMX_RST_OCX, rst_ocx.u64);

		}
	}

	/* Put all cores in the other node in reset */
	for (node = 1; node < CVMX_MAX_NODES; node++) {
		if (cvmx_numa_exists(node)) {
			cvmx_write_csr_node(node, CVMX_CIU_PP_RST, -1ull);
			cvmx_write_csr_node(node, CVMX_RST_PP_POWER, -1ull);
		}
	}

	if (cvmx_numa_is_only_one()) {
		debug("Single node - OCX is not functional, starting to boot node0\n");
	} else {
		debug("OCX is functional, starting to boot nodes\n");
	}
	return 0;
}

#ifdef DEBUG
extern int early_uart_init_f(void);
#endif

/**
 * Call this function to take secondary nodes and cores out of
 * reset and have them start running threads
 *
 * @return Zero on success, negative on failure.
 */
int oci_init(void)
{
	union cvmx_gserx_phy_ctl gserx_phy_ctl;
	union cvmx_ocx_com_linkx_ctl ocx_com_linkx_ctl;
	union cvmx_rst_ocx rst_ocx;
	int result = 0;
#ifdef DEBUG
	early_uart_init_f();
	gd->have_console = 1;
#endif
#if defined(CONFIG_OCTEON_FAILSAFE_GPIO) && !defined(CONFIG_OCTEON_NO_FAILSAFE)
	if (gpio_get_value(CONFIG_OCTEON_FAILSAFE_GPIO))
		return 0;
#endif

	if (OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_X)) {
		debug("%s: No OCX for pass 1.X\n", __func__);
		return 0;
	}
	if (!OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		debug("%s: Octeon does not support OCX/CCPI\n", __func__);
		return 0;
	}

	debug("%s() start...\n", __func__);

	gserx_phy_ctl.u64 = cvmx_read_csr(CVMX_GSERX_PHY_CTL(8));
	if (gserx_phy_ctl.s.phy_reset) {
		debug("CCPI/OCX disabled.\n");
		return 0;
	}
	ocx_com_linkx_ctl.u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(0));
	rst_ocx.u64 = cvmx_read_csr(CVMX_RST_OCX);

	debug("Initializing OCX connection, ocx_com_link0_ctl: 0x%llx, rst_ocx: 0x%llx\n",
	      ocx_com_linkx_ctl.u64, rst_ocx.u64);
	if (!(ocx_com_linkx_ctl.s.valid && ocx_com_linkx_ctl.s.up)) {
		debug("%s: Calling init_ccpi_early()\n", __func__);

		init_ccpi_early(true);

		debug("%s: Calling init_ccpi_connection2()\n", __func__);
		result = init_ccpi_connection2(true, 10312, false);
		if (result) {
			printf("Could not initialize OCX/CCPI links\n");
			return 0;
		}
	} else {
		debug("Connection already initialized, up and valid\n");
	}
	debug("Waiting to initialize OCX/CCPI\n");
	/* Give the other node time to put the core back in reset */
	ocx_com_linkx_ctl.u64 = cvmx_read_csr(CVMX_OCX_COM_LINKX_CTL(0));
	debug("ocx_com_link0_ctl: 0x%llx\n",
	      ocx_com_linkx_ctl.u64);
	mdelay(5000);
	result |= init_oci();
	debug("...OCI_SETUP end.\n");

	return 0;
}
