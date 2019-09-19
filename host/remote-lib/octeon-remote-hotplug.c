/***********************license start***************
 * Copyright (c) 2003-2017 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
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
 * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 ***********************license end**************************************/

/**
 * @file
 *
 * This file contains a remote variant of cvmx-app-hotplug.c library
 * with those methods needed to remotely manipulate SE-S applications
 * from an x86 host via PCIe.
 *
 * $Id$
 */

#define	MIN(a, b) (((a) < (b)) ? (a) : (b))
#include <assert.h>
#include "cvmx.h"
#include "cvmx-bootmem.h"
#include "cvmx-coremask.h"
#include "cvmx-app-hotplug.h"
#include "octeon-remote.h"

#define CVMX_MBOX_BIT_HOTPLUG_SHUTDOWN          0
#define CVMX_MBOX_BIT_HOTPLUG_REMOVECORES       1
#define CVMX_MBOX_BIT_HOTPLUG_ADDCORES          2
#define CVMX_MBOX_CIU3_MBOX_PER_CORE 10

/* the hotplug_global_lock used instead of the hgp->hotplug_gloabal_lock */
cvmx_atomic_spinlock_t hotplug_global_lock;

/* octeon_remote_read_mem2() read using 'mem64' to take care of endianess */
void octeon_remote_read_mem2(void *buffer, uint64_t pa, int len)
{
	uint64_t *p = buffer;
	int i = len / sizeof(uint64_t);
	for (; i >= 0; i--)
		p[i] = octeon_remote_read_mem64(pa + i * sizeof(uint64_t));
}

/* octeon_remote_write_mem2() write using 'mem64' to take care of endianess */
void octeon_remote_write_mem2(uint64_t pa, void *buffer, int len)
{
	uint64_t *p = buffer;
	int i = len / sizeof(uint64_t);
	for (; i >= 0; i--)
		octeon_remote_write_mem64(pa + i * sizeof(uint64_t), p[i]);
}

/* A remote variant of the SE mailbox signal routine */
void (*cvmx_mbox_signal) (unsigned int bit, unsigned int core);

static void cvmx_mbox_ciu_signal(unsigned int bit, unsigned int core)
{
	uint64_t mask = 1ull << bit;

	octeon_remote_write_csr(CVMX_CIU_MBOX_SETX(core), mask);
	octeon_remote_read_csr(CVMX_CIU_MBOX_SETX(core));
}

/*
 * 10 mbox per core starting from zero.
 * Base mbox is core * 10
 */
static unsigned int cvmx_mbox_ciu3_base_mbox_intsn(unsigned int core)
{
	core &= (1 << CVMX_NODE_NO_SHIFT) - 1;
	/* SW (mbox) are 0x04 in bits 12..19 */
	return 0x04000 + CVMX_MBOX_CIU3_MBOX_PER_CORE * core;
}

static unsigned int cvmx_mbox_ciu3_mbox_intsn_for_core(unsigned int core,
						       unsigned int mbox)
{
	return cvmx_mbox_ciu3_base_mbox_intsn(core) + mbox;
}

static void cvmx_mbox_ciu3_signal(unsigned int bit, unsigned int core)
{
	cvmx_ciu3_iscx_w1s_t isc_w1s;
	unsigned int intsn = cvmx_mbox_ciu3_mbox_intsn_for_core(core, bit);
	int node = (core >> CVMX_NODE_NO_SHIFT) & CVMX_NODE_MASK;
	uint64_t addr = CVMX_CIU3_ISCX_W1S(intsn);

	isc_w1s.u64 = 0;
	isc_w1s.s.raw = 1;
	assert(node == 0);	/* Do not support CCPI with remote utils */
	octeon_remote_write_csr(addr, isc_w1s.u64);
	octeon_remote_read_csr(addr);
}

/* returns phys addr of the hotplug_global named block */
static uint64_t _get_hotplug_global_ptr(void)
{
	uint64_t hgp, sz;
	uint64_t magic_offset;
	static uint64_t hotplug_global_paddr;
	uint64_t magic;

	/* Return the pointer if already initialized */
	if (cvmx_likely(hotplug_global_paddr != 0))
		return hotplug_global_paddr;
	/* Remote utilities can not allocate atomically */
	if (octeon_remote_named_block_find
	    (CVMX_APP_HOTPLUG_INFO_REGION_NAME, &hgp, &sz) == 0)
		goto __notvalid;

	if (sz != CVMX_APP_HOTPLUG_INFO_REGION_SIZE)
		goto __notvalid;

	magic_offset = offsetof(struct cvmx_app_hotplug_global, magic_version);
	magic = octeon_remote_read_mem64(hgp + magic_offset);
	if (magic != CVMX_HOTPLUG_MAGIC_VERSION)
		goto __notvalid;

	/* Select mailbox signal function based on model */
	if (cvmx_mbox_signal == NULL) {
		uint32_t model = octeon_remote_get_model() & OCTEON_FAMILY_MASK;
		if (model == OCTEON_CN73XX_PASS1_0 ||
		    model == OCTEON_CN78XX_PASS1_0)
			cvmx_mbox_signal = cvmx_mbox_ciu3_signal;
		else
			cvmx_mbox_signal = cvmx_mbox_ciu_signal;
	}

	hotplug_global_paddr = hgp;
	return hgp;
__notvalid:
	cvmx_printf("ERROR: %s: Hotplug info region is invalid\n", __func__);
	return 0;
}

static void *cvmx_app_get_hotplug_global_ptr(void)
{
	return (void *)(uintptr_t) _get_hotplug_global_ptr();
}

void oct_remote_read_coremask(uint64_t remoteaddr, cvmx_coremask_t *pcm)
{
	uint64_t p1, *p2, sz;

	p2 = (uint64_t *) pcm;
	p1 = remoteaddr;
	sz = sizeof(*pcm);
	while (sz) {
		*p2 = octeon_remote_read_mem64(p1);
		p2++;
		p1 += sizeof(uint64_t);
		sz -= sizeof(uint64_t);
	}
}

void oct_remote_write_coremask(uint64_t remoteaddr, const cvmx_coremask_t *pcm)
{
	uint64_t p1, *p2, sz;

	p2 = (uint64_t *) pcm;
	p1 = remoteaddr;
	sz = sizeof(*pcm);
	while (sz) {
		octeon_remote_write_mem64(p1, *p2);
		p2++;
		p1 += sizeof(uint64_t);
		sz -= sizeof(uint64_t);
	}
}

/**
 * Returns non-zero if any app is currently being currently booted,
 * hotplugged or * shutdown.
 * Only one app can be under a boot, hotplug or shutdown condition.
 * Before booting an app this methods should be used to check whether boot or
 * shutdown activity is in progress and proceed with the boot or shutdown only
 * when there is no other activity.
 *
 */
int is_app_under_boot_or_shutdown(void)
{
	uint64_t ret1, ret2;
	cvmx_app_hotplug_global_t *hgp;

	hgp = cvmx_app_get_hotplug_global_ptr();
	cvmx_spinlock_lock(&hotplug_global_lock);
	ret1 = octeon_remote_read_mem64((uintptr_t) &hgp->app_under_boot);
	ret2 = octeon_remote_read_mem64((uintptr_t) &hgp->app_under_shutdown);
	cvmx_spinlock_unlock(&hotplug_global_lock);
	return ret1 | ret2;
}

/**
 * Return the hotplug info structure (cvmx_app_hotplug_info_t) pointer for the
 * application with the specified index.
 *
 * @param index        index of application.
 * @return             Returns hotplug info struct on success, NULL on failure
 *
 */
static cvmx_app_hotplug_info_t *cvmx_app_hotplug_get_info_at_index(int index)
{
	cvmx_app_hotplug_global_t *hgp;
	uint64_t valid_magic, hip;

	assert(sizeof(cvmx_app_hotplug_info_t) == 1024u);

	hgp /*pa */  = cvmx_app_get_hotplug_global_ptr();
	if (!hgp)
		return NULL;

	hip /*pa */  = (uintptr_t) &hgp->hotplug_info_array[index];
	valid_magic = octeon_remote_read_mem64(
	    (uintptr_t) &hgp->hotplug_info_array[index].valid_magic);
	if (index < CVMX_APP_HOTPLUG_MAX_APPS)
		if (valid_magic == CVMX_HOTPLUG_MAGIC_VALID)
			return (cvmx_app_hotplug_info_t *) hip;

	return NULL;
}

/**
 * Determines if SE application at the index specified is hotpluggable.
 *
 * @param index        index of application.
 * @return             Returns -1  on error.
 *                     0 -> The application is not hotpluggable
 *                     1 -> The application is hotpluggable
*/
int is_app_hotpluggable(int index)
{
	cvmx_app_hotplug_info_t *ai;
	cvmx_coremask_t pcm;

	ai = cvmx_app_hotplug_get_info_at_index(index);
	if (!ai) {
		cvmx_dprintf("\nERROR: "
			"Failed to get hotplug info for app at index=%d\n",
			index);
		return -1;
	}

	oct_remote_read_coremask((uintptr_t) &ai->hotplug_activated_coremask,
				 &pcm);
	if (!cvmx_coremask_is_empty(&pcm))
		return 1;

	return 0;
}

/**
 * This routine sends a request to a running target application
 * to unplug a specified set cores
 * @param index        is the index of the target application
 * @param pcm          Coremask of the cores to be unplugged from the app.
 * @param wait         1 - Wait for shutdown completion
 *                     0 - Do not wait
 * @return             0 on success, -1 on error
 *
 */
int cvmx_app_hotplug_unplug_cores(int index, const struct cvmx_coremask *pcm,
				  int wait)
{
	cvmx_app_hotplug_info_t *ai;
	int i;
	struct cvmx_coremask tmp_cm, coremask;

	ai /*pa */  = cvmx_app_hotplug_get_info_at_index(index);
	if (!ai) {
		cvmx_dprintf("\nERROR: "
			"Failed to get hotplug info for app at index=%d\n",
			index);
		return -1;
	}

	oct_remote_write_coremask((uintptr_t) &ai->unplug_cores, pcm);

	oct_remote_read_coremask((uintptr_t) &ai->coremask, &coremask);
	cvmx_coremask_and(&tmp_cm, &coremask, pcm);

	if (cvmx_coremask_cmp(&tmp_cm, pcm)) {
		cvmx_dprintf
		    ("\nERROR: Not all cores requested are a part of the app "
		     "r:\n");
		cvmx_coremask_print(pcm);
		cvmx_coremask_print(&coremask);
		return -1;
	}

	if (!cvmx_coremask_cmp(&coremask, pcm)) {
		cvmx_dprintf
		    ("\nERROR: Trying to remove all cores in app.  r:\n");
		cvmx_coremask_print(pcm);
		cvmx_coremask_print(&coremask);
		return -1;
	}

	/* Send IPIs to all application cores to request unplug/remove_cores
	   callback */
	cvmx_coremask_for_each_core(i, &coremask) {
		cvmx_mbox_signal(CVMX_MBOX_BIT_HOTPLUG_REMOVECORES, i);
	}

	/* Update coremask of removed cores */
	cvmx_coremask_maskoff(&coremask, &coremask, pcm);
	/* Write updated coremask to the target */
	oct_remote_write_coremask((uintptr_t) &ai->coremask, &coremask);

	/* Wait for the unplugged cores to be unactivated */
	while (wait) {
		cvmx_coremask_t mask, hac;
		/* Refresh our copy of the app_info structure */
		ai = cvmx_app_hotplug_get_info_at_index(index);
		oct_remote_read_coremask(
		    (uintptr_t) &ai->hotplug_activated_coremask, &hac);
		cvmx_coremask_and(&mask, pcm, &hac);
		if (cvmx_coremask_is_empty(&mask))
			break;
	}

	return 0;
}

/**
 * Return the hotplug application index structure for the application running on the
 * given coremask.
 *
 * @param pcm          Coremask of application.
 * @return             Returns hotplug application index on success.
 *                     -1 on failure
 *
 */
int cvmx_app_hotplug_get_index(const struct cvmx_coremask *pcm)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_global_t *hgp;
	cvmx_coremask_t cm;
	uint64_t valid_magic;
	int i;

	hgp = cvmx_app_get_hotplug_global_ptr();

	if (!hgp)
		return -1;

	if (cvmx_coremask_is_empty(pcm)) {
		cvmx_dprintf("%s: empty coremask\n", __func__);
		return -1;
	}

	/* Look for the current app's info */
	for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {
		hip = cvmx_app_hotplug_get_info_at_index(i) /*ret paddr */;
		if (!hip)
			continue;	/* skip empty slots */

		oct_remote_read_coremask((uintptr_t) &hip->coremask, &cm);

		if (cvmx_coremask_cmp(&cm, pcm))
			continue;

		valid_magic =
		    octeon_remote_read_mem64((uintptr_t) &hip->valid_magic);
		if (valid_magic != CVMX_HOTPLUG_MAGIC_VALID) {
			cvmx_dprintf("%s: hotplug info %d not valid\n",
				     __func__, i);
			break;
		}

		return i;
	}

	return -1;
}

/**
 * Return the hotplug info structure (cvmx_app_hotplug_info_t) pointer for the
 * application running on the given coremask.
 *
 * @param pcm          Coremask of application.
 * @return             Returns hotplug info struct on success, NULL on failure
 *
 */
cvmx_app_hotplug_info_t *cvmx_app_hotplug_get_info(
	const struct cvmx_coremask *pcm)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_global_t *hgp;
	int i;

	hgp = cvmx_app_get_hotplug_global_ptr();
	if (!hgp)
		return NULL;

	i = cvmx_app_hotplug_get_index(pcm);

	if (i < 0)
		return NULL;
	hip = cvmx_app_hotplug_get_info_at_index(i);
	if (!hip)
		return NULL;

	return hip;
}

/**
 * This routine sends a shutdown request to a running target application.
 *
 * @param pcm          Coremask the application is running on.
 * @param wait         1 - Wait for shutdown completion
 *                     0 - Do not wait
 * @return             0 on success, -1 on error
 *
 */

int cvmx_app_hotplug_shutdown_request(const struct cvmx_coremask *pcm, int wait)
{
	int i;
	cvmx_app_hotplug_info_t *hotplug_info_ptr;
	cvmx_coremask_t hac;
	uint64_t val;
	hotplug_info_ptr /*pa */  = cvmx_app_hotplug_get_info(pcm);
	if (!hotplug_info_ptr) {
		cvmx_dprintf
		    ("\nERROR: Failed to get hotplug info for coremask\n");
		cvmx_coremask_print(pcm);
		return -1;
	}

	oct_remote_write_coremask((uintptr_t) &hotplug_info_ptr->
				  shutdown_cores, pcm);

	val = octeon_remote_read_mem64((uintptr_t) &hotplug_info_ptr->
				       shutdown_callback);

	if (val == 0) {
		cvmx_dprintf("\nERROR: "
			"Target application has not registered for hotplug!\n");
		return -1;
	}

	oct_remote_read_coremask(
	    (uintptr_t) &hotplug_info_ptr->hotplug_activated_coremask, &hac);

	if (cvmx_coremask_cmp(&hac, pcm)) {
		cvmx_dprintf("\nERROR: "
			"Not all application cores have activated hotplug\n");
			return -1;
	}

	/* Send IPIs to all application cores to request shutdown */
	cvmx_coremask_for_each_core(i, pcm)
	    cvmx_mbox_signal(CVMX_MBOX_BIT_HOTPLUG_SHUTDOWN, i);

	if (wait) {
		val = 0;
		while (val == 0) {
			usleep(10000);
			val = octeon_remote_read_mem64((uintptr_t) &
						       hotplug_info_ptr->
						       shutdown_done);

			if (val == 0)
				cvmx_dprintf("WARNING: "
				"Waiting for application to shutdown\n");
			else
				break;
		}
	}
	return 0;
}

/**
 * This routine is only required if cvmx_app_hotplug_shutdown_request() was called
 * with wait=0. This routine waits for the application shutdown to complete.
 *
 * @param pcm          Coremask the application is running on.
 * @return             0 on success, -1 on error
 *
 */
int cvmx_app_hotplug_shutdown_complete(const struct cvmx_coremask *pcm)
{
	cvmx_app_hotplug_info_t *hotplug_info_ptr;
	uint64_t val;

	hotplug_info_ptr /*pa */  = cvmx_app_hotplug_get_info(pcm);
	if (!hotplug_info_ptr) {
		cvmx_dprintf
		    ("\nERROR: Failed to get hotplug info for coremask:\n");
		cvmx_coremask_print(pcm);
		return -1;
	}

	do {
		val = octeon_remote_read_mem64(
		    (uintptr_t) &hotplug_info_ptr->shutdown_done);
		if (val == 0) {
			printf("Waiting for shutdown done\n");
			usleep(200000);
		}
	} while (val == 0);

	return 0;
}

/**
 * This routine invokes the cores_added callbacks.
 */
int cvmx_app_hotplug_call_add_cores_callback(int index)
{
	cvmx_app_hotplug_info_t *ai;
	cvmx_coremask_t cm;
	int i;

	ai = cvmx_app_hotplug_get_info_at_index(index);
	if (!ai) {
		cvmx_dprintf("\nERROR: "
			"Failed to get hotplug info for app at index=%d\n",
			index);
		return -1;
	}
	/* Send IPIs to all application cores to request add_cores callback */
	oct_remote_read_coremask((uintptr_t) &ai->coremask, &cm);
	cvmx_coremask_for_each_core(i, &cm)
	    cvmx_mbox_signal(CVMX_MBOX_BIT_HOTPLUG_ADDCORES, i);

	return 0;
}

/**
 * Sets or clear the app_under_boot value. This when set signifies that an app
 * is being currently booted or hotplugged with a new core.
 *
 *
 * @param val     sets the app_under_boot to the specified value. This should be
 *                set to 1 while app any is being booted and cleared after the
 *                application has booted up.
 *
 * LR: Need to add return value to make this into a working lock
 */
void set_app_under_boot(int val)
{
	cvmx_app_hotplug_global_t *hgp;

	hgp = cvmx_app_get_hotplug_global_ptr();
	cvmx_spinlock_lock(&hotplug_global_lock);
	octeon_remote_write_mem64((uintptr_t) &hgp->app_under_boot, val);
	cvmx_spinlock_unlock(&hotplug_global_lock);
}

/**
 * Sets or clear the app_under_shutdown value. This when set signifies that an
 * app is being currently shutdown or some cores of an app are being shutdown.
 *
 * @param val     sets the app_under_shutdown to the specified value. This
 *                should be set to 1 while any app is being shutdown and cleared
 *                after the shutdown of the app is complete.
 *
 */
void set_app_under_shutdown(int val)
{
	cvmx_app_hotplug_global_t *hgp;

	hgp = cvmx_app_get_hotplug_global_ptr();
	cvmx_spinlock_lock(&hotplug_global_lock);
	octeon_remote_write_mem64((uintptr_t) &hgp->app_under_shutdown, val);
	cvmx_spinlock_unlock(&hotplug_global_lock);
}

/*
List of required functions:

cvmx_app_hotplug_shutdown_request
+ cvmx_app_hotplug_call_add_cores_callback
+ cvmx_app_hotplug_unplug_cores
+ set_app_under_shutdown
+ set_app_under_boot
+ is_app_under_boot_or_shutdown
+ is_app_hotpluggable
*/
