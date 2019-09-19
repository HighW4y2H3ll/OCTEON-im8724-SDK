/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com). All rights
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

 * This Software, including technical data, may be subject to U.S. export
 * control laws, including the U.S. Export Administration Act and its associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION
 * OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Module to support operations on bitmap of cores. Coremask can be used to
 * select a specific core, a group of cores, or all available cores, for
 * initialization and differentiation of roles within a single shared binary
 * executable image.
 *
 * <hr>$Revision: 163638 $<hr>
 *
 */

#if defined(__U_BOOT__)
#include <linux/ctype.h>
#include <errno.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-spinlock.h>
#include <asm/arch/cvmx-coremask.h>
#include <asm/arch/cvmx-bootmem.h>
#elif defined(CVMX_BUILD_FOR_LINUX_KERNEL)
#include <linux/ctype.h>
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-asm.h>
#include <asm/octeon/cvmx-spinlock.h>
#include <asm/octeon/cvmx-coremask.h>
#include <asm/octeon/cvmx-bootmem.h>
#else
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include "cvmx.h"
#include "cvmx-asm.h"
#include "cvmx-spinlock.h"
#include "cvmx-coremask.h"
#include "cvmx-bootmem.h"
#endif

#ifndef CVMX_BUILD_FOR_LINUX_HOST
#define  CVMX_COREMASK_MAX_SYNCS  20	/* maximum number of coremasks for barrier sync */

/**
 * This structure defines the private state maintained by coremask module.
 *
 */
CVMX_SHARED static struct {

	cvmx_spinlock_t lock;			/**< mutex spinlock */

	struct {
		cvmx_coremask_t coremask;	/**< coremask specified for barrier */
		cvmx_coremask_t checkin;	/**< bitmask of cores checking in */
		volatile unsigned int exit;	/**< variable to poll for exit condition */

	} s[CVMX_COREMASK_MAX_SYNCS];

} state = {
	{CVMX_SPINLOCK_UNLOCKED_VAL}, { {{{0,},}, {{0,},}, 0}, },
};

/* when run on HOST cvmx_get_core_num() is not applicable,
 * so this func as well
 */

/**
 * Wait (stall) until all cores in the given coremask has reached this point
 * in the program execution before proceeding.
 *
 * @param  pcm  the group of cores performing the barrier sync
 *
 */
void cvmx_coremask_barrier_sync(const cvmx_coremask_t *pcm)
{
	int i;
	unsigned int target;
#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
	assert(pcm != NULL && !((long)pcm & 3));
#endif
	cvmx_spinlock_lock(&state.lock);

	for (i = 0; i < CVMX_COREMASK_MAX_SYNCS; i++) {

		if (cvmx_coremask_is_empty(&state.s[i].coremask)) {
			/* end of existing coremask list, create new entry,
			 * fall-thru
			 */
			cvmx_coremask_copy(&state.s[i].coremask, pcm);
		}

		/* Is it our coremask? */
		if (cvmx_coremask_cmp(&state.s[i].coremask, pcm) == 0) {

			target = (state.s[i].exit + 1);

			/* Check in to the checkin coremask */
			cvmx_coremask_set_self(&state.s[i].checkin);

			/* See if everyone has checked in */
			if (cvmx_coremask_cmp(&state.s[i].checkin, pcm) == 0) {
				cvmx_coremask_clear_all(&state.s[i].checkin);
				state.s[i].exit = target;	/* signal exit condition */
			}
			cvmx_spinlock_unlock(&state.lock);

			/* Wait until all cores have checked in */
			while (state.s[i].exit != target) ;

			return;
		}
	}

	/* error condition - we're out of sync states */
	cvmx_spinlock_unlock(&state.lock);
#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
	assert(0);
#endif
}

#endif


int cvmx_coremask_str2bmp(cvmx_coremask_t *pcm, char *hexstr)
{
	int i, j;
	int l;		/* length of the hexstr in characters */
	int lb;		/* number of bits taken by hexstr */
	int hldr_offset;/* holder's offset within the coremask */
	int hldr_xsz;	/* holder's size in the number of hex digits */
	cvmx_coremask_holder_t h;
	char c;

#define MINUS_ONE (hexstr[0] == '-' && hexstr[1] == '1' && hexstr[2] == 0)
	if (MINUS_ONE) {
		cvmx_coremask_set_all(pcm);
		return 0;
	}

	/* Skip '0x' from hexstr */
	if (hexstr[0] == '0' && (hexstr[1] == 'x' || hexstr[1] == 'X'))
		hexstr += 2;

	if (!strlen(hexstr)) {
		cvmx_dprintf("%s: Error: hex string is empty\n", __func__);
		return -2;
	}
	/* Trim leading zeros */
	while (*hexstr == '0')
		hexstr++;

	cvmx_coremask_clear_all(pcm);
	l = strlen(hexstr);

	/* If length is 0 then the hex string must be all zeros */
	if (l == 0)
		return 0;

	for (i = 0; i < l; i++) {
		if (isxdigit((int)hexstr[i]) == 0) {
			cvmx_dprintf("%s: Non-hex digit within hexstr\n",
			    __func__);
			return -2;
		}
	}

	lb = (l - 1) * 4;
	if (hexstr[0] > '7')
		lb += 4;
	else if (hexstr[0] > '3')
		lb += 3;
	else if (hexstr[0] > '1')
		lb += 2;
	else
		lb += 1;
	if (lb > CVMX_MIPS_MAX_CORES) {
		cvmx_dprintf("%s: hexstr (%s) is too long\n", __func__,
		    hexstr);
		return -1;
	}

	hldr_offset = 0;
	hldr_xsz = 2 * sizeof(cvmx_coremask_holder_t);
	for (i = l; i > 0; i -= hldr_xsz) {
		c = hexstr[i];
		hexstr[i] = 0;
		j = i - hldr_xsz;
		if (j < 0)
			j = 0;
		h = strtoull(&hexstr[j], NULL, 16);
#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
		if (errno == EINVAL) {
			cvmx_dprintf("%s: strtou returns w/ EINVAL\n",
			    __func__);
			return -2;
		}
#endif
		pcm->coremask_bitmap[hldr_offset] = h;
		hexstr[i] = c;
		hldr_offset++;
	}

	return 0;
}

int cvmx_coremask_bmp2str(const cvmx_coremask_t *pcm, char *hexstr)
{
	int i, n;
	char *p;
	char *fmt1, *fmt2;

	if (sizeof(cvmx_coremask_holder_t) <= sizeof(uint32_t)) {
		fmt1 = "%x";
		fmt2 = "%08x";
	} else {
		fmt1 = "%llx";
		fmt2 = "%016llx";
	}

	n = 0;
	p = hexstr;
	for (i = CVMX_COREMASK_BMPSZ - 1; i >= 0; i--)
		if (pcm->coremask_bitmap[i])
			break;
	if (i < 0) {
		sprintf(&p[n], "0");
		return 0;
	}

	if (pcm->coremask_bitmap[i])
		n = sprintf(&p[n], fmt1, pcm->coremask_bitmap[i]);

	for (i--; i >= 0; i--)
		n += sprintf(&p[n], fmt2, pcm->coremask_bitmap[i]);

	return 0;
}

void cvmx_coremask_print(const cvmx_coremask_t *pcm)
{
	int i, j;
	int start;
	int found = 0;
	/* Print one node per line.  Since the bitmap is stored LSB to MSB
	 * we reverse the order when printing.
	 */
	if (!octeon_has_feature(OCTEON_FEATURE_MULTINODE)) {
		start = 0;
		for (j = CVMX_COREMASK_MAX_CORES_PER_NODE - CVMX_COREMASK_HLDRSZ;
		     j >= 0; j -= CVMX_COREMASK_HLDRSZ) {
			if (pcm->coremask_bitmap[j / CVMX_COREMASK_HLDRSZ] != 0)
				start = 1;
			if (start)
				cvmx_printf(" 0x%llx",
				       (unsigned long long)pcm->coremask_bitmap[j / CVMX_COREMASK_HLDRSZ]);
		}
		if (start)
			found = 1;
		/* If the coremask is empty print <EMPTY> so it is not confusing.  */
		if (!found)
			cvmx_printf("<EMPTY>");
		cvmx_printf("\n");
		return;
	}

	for (i = 0; i < CVMX_MAX_USED_CORES_BMP;
	     i += CVMX_COREMASK_MAX_CORES_PER_NODE) {
		cvmx_printf("%s  node %d:", i > 0 ? "\n" : "",
		       cvmx_coremask_core_to_node(i));
		start = 0;
		for (j = i + CVMX_COREMASK_MAX_CORES_PER_NODE - CVMX_COREMASK_HLDRSZ;
		     j >= i;
		     j -= CVMX_COREMASK_HLDRSZ) {
			/* Don't start printing until we get a non-zero word. */
			if (pcm->coremask_bitmap[j / CVMX_COREMASK_HLDRSZ] != 0)
				start = 1;

			if (start)
				cvmx_printf(" 0x%llx",
				       (unsigned long long)pcm->coremask_bitmap[j / CVMX_COREMASK_HLDRSZ]);
		}
		if (start)
			found = 1;
	}
	i /= CVMX_COREMASK_HLDRSZ;
	for (; i < CVMX_COREMASK_BMPSZ; i++)
		if (pcm->coremask_bitmap[i])
			cvmx_printf("  EXTRA GARBAGE[%i]: %016llx\n", i, (unsigned long long)pcm->coremask_bitmap[i]);
	/* If the coremask is empty print <EMPTY> so it is not confusing.  */
	if (!found)
		cvmx_printf("<EMPTY>");
	cvmx_printf("\n");
}


/* Don't include cvmx_get_avail_coremask and cvmx_get_hardware_coremask while building for newlib.  */
#if !defined(__OCTEON_NEWLIB__)

/* local helping function - will work ONLY with size%8 = 0 (OK for this use) */
static
int __cvmx_copy_from_bootmem(int64_t bootmem_src_addr, void *dst_ptr, int size)
{
	int i;
	int64_t base_addr = (1ull << 63) | bootmem_src_addr;
	uint64_t *ptr64 = (uint64_t *)dst_ptr;

	for (i = 0; i < size/8; i++) {
		ptr64[i] = cvmx_read64_uint64(base_addr);
		base_addr += 8;
	}
	return 0;
}

/*
 * Gets the 'avail_coremask' from the dedicated (BOOTINFO) named block
 * @param[out] coremask pointer where to copy avail_coremask
 * @return 0 on success, -1 otherwise
 */
int cvmx_get_avail_coremask(cvmx_coremask_t *coremask)
{
	cvmx_cores_common_bootinfo_t ccbi;
	const cvmx_bootmem_named_block_desc_t *ccbi_desc;
	static cvmx_coremask_t avail_coremask = CVMX_COREMASK_EMPTY;
	
	if (coremask == NULL)
		return -1; /* ERROR: wrong param - 'return -1' */
	
	if (!cvmx_coremask_is_empty(&avail_coremask)) {
		/* return the cashed result */
		cvmx_coremask_copy(coremask, &avail_coremask);
		return 0;
	}
	
	ccbi_desc =
		cvmx_bootmem_find_named_block(CVMX_APP_COMMON_BOOTINFO_NAME);
	if (!ccbi_desc) {
		cvmx_dprintf("Info: named_block<%s> not found.\n",
			     CVMX_APP_COMMON_BOOTINFO_NAME);
		return -1;
	}
	if (ccbi_desc) { /* 'common bootinfo' named block is found - use it*/
		__cvmx_copy_from_bootmem(ccbi_desc->base_addr, &ccbi,
					sizeof(cvmx_cores_common_bootinfo_t));
		/* Validate signature */
		if (ccbi.magic != CVMX_COMMON_BOOTINFO_MAGIC)
			return -1; /* if 'magic' does not match - exit */
		/* the members from the initial(1) version are always valid */
		/* only avail_coremask is need - fill it in */
		cvmx_coremask_copy(coremask, &ccbi.avail_coremask);
		/* the extra (version) members (if any) are valid when
		 * (2 <= ccbi.version <= CVMX_COMMON_BOOTINFO_VERSION)
		 * if (ccbi.version >= 2) { xxx = ccbi.ver2_member; }
		 */
		/* cache the result in the local static 'avail_coremask' */
		cvmx_coremask_copy(&avail_coremask, coremask);
	}
	return 0;
}


/*
 * Gets the 'hardware_coremask' from the dedicated (BOOTINFO) named block
 * @param[out] coremask pointer where to copy hardware_coremask
 * @return 0 on success, -1 otherwise
 */
int cvmx_get_hardware_coremask(cvmx_coremask_t *coremask)
{
	cvmx_cores_common_bootinfo_t ccbi;
	const cvmx_bootmem_named_block_desc_t *ccbi_desc;
	static cvmx_coremask_t hardware_coremask = CVMX_COREMASK_EMPTY;
	
	if (coremask == NULL)
		return -1; /* ERROR: wrong param - 'return -1' */
	
	if (!cvmx_coremask_is_empty(&hardware_coremask)) {
		/* return the cashed result */
		cvmx_coremask_copy(coremask, &hardware_coremask);
		return 0;
	}
	
	/* get the hardware_coremask from the BOOTINFO name block */
	ccbi_desc =
		cvmx_bootmem_find_named_block(CVMX_APP_COMMON_BOOTINFO_NAME);
	if (!ccbi_desc) {
		cvmx_dprintf("Info: named_block<%s> not found.\n",
			     CVMX_APP_COMMON_BOOTINFO_NAME);
		return -1;
	}
	if (ccbi_desc) { /* 'common bootinfo' named block is found - use it*/
		__cvmx_copy_from_bootmem(ccbi_desc->base_addr, &ccbi,
					sizeof(cvmx_cores_common_bootinfo_t));
		/* Validate signature */
		if (ccbi.magic != CVMX_COMMON_BOOTINFO_MAGIC)
			return -1; /* if 'magic' does not match - exit */
		/* the members from the initial(1) version are always valid */
		/* only hardware_coremask is need - fill it in */
		cvmx_coremask_copy(coremask, &ccbi.hardware_coremask);
		/* the extra (version) members (if any) are valid when
		 * (2 <= ccbi.version <= CVMX_COMMON_BOOTINFO_VERSION)
		 * if (ccbi.version >= 2) { xxx = ccbi.ver2_member; }
		 */
		/* cache the result in the local static 'hardware_coremask' */
		cvmx_coremask_copy(&hardware_coremask, coremask);
	}
	return 0;
}

#endif


