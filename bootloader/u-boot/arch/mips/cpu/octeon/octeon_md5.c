/***********************license start***************
 * Copyright (c) 2012  Cavium Inc. (support@cavium.com). All rights
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

#include <common.h>
#include <watchdog.h>
#include <u-boot/md5.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-key.h>
#include <asm/arch/cvmx-swap.h>

void md5(unsigned char *input, int len, unsigned char output[16])
{
	const uint64_t bits = cvmx_cpu_to_le64(len * 8);
	const uint64_t *ptr = (const uint64_t *)input;
	uint8_t chunk[64];

	/* Set the IV to the MD5 magic start value */
	CVMX_MT_HSH_IV(0x0123456789abcdefull, 0);
	CVMX_MT_HSH_IV(0xfedcba9876543210ull, 1);

	/* MD5 input is in the following form:
	 * 1) User data
	 * 2) Byte 0x80
	 * 3) Optional zero padding
	 * 4) Original Data length in bits as an 8 byte unsigned integer
	 * Zero padding is added to make 1-4 an even multiple of 64 bytes
	 */

	/* Iterate through 64 bytes at a time */
	while (len >= 64) {
		CVMX_MT_HSH_DAT(*ptr++, 0);
		CVMX_MT_HSH_DAT(*ptr++, 1);
		CVMX_MT_HSH_DAT(*ptr++, 2);
		CVMX_MT_HSH_DAT(*ptr++, 3);
		CVMX_MT_HSH_DAT(*ptr++, 4);
		CVMX_MT_HSH_DAT(*ptr++, 5);
		CVMX_MT_HSH_DAT(*ptr++, 6);
		CVMX_MT_HSH_STARTMD5(*ptr++);
		len -= 64;
	}

	/* The rest of the data will need to be copied into a chunk */
	if (len > 0)
		memcpy(chunk, ptr, len);
	chunk[len] = 0x80;
	memset(chunk + len + 1, 0, 64 - len - 1);

	ptr = (const uint64_t *)chunk;
	CVMX_MT_HSH_DAT(*ptr++, 0);
	CVMX_MT_HSH_DAT(*ptr++, 1);
	CVMX_MT_HSH_DAT(*ptr++, 2);
	CVMX_MT_HSH_DAT(*ptr++, 3);
	CVMX_MT_HSH_DAT(*ptr++, 4);
	CVMX_MT_HSH_DAT(*ptr++, 5);
	CVMX_MT_HSH_DAT(*ptr++, 6);

	/* Check to see if there is room for the bit count */
	if (len < 56) {
		CVMX_MT_HSH_STARTMD5(bits);
	} else {
		CVMX_MT_HSH_STARTMD5(*ptr);
		/* Another block was needed */
		CVMX_MT_HSH_DATZ(0);
		CVMX_MT_HSH_DATZ(1);
		CVMX_MT_HSH_DATZ(2);
		CVMX_MT_HSH_DATZ(3);
		CVMX_MT_HSH_DATZ(4);
		CVMX_MT_HSH_DATZ(5);
		CVMX_MT_HSH_DATZ(6);
		CVMX_MT_HSH_STARTMD5(bits);
	}

	/* Get the final MD5 */
	CVMX_MF_HSH_IV(((uint64_t *)output)[0], 0);
	CVMX_MF_HSH_IV(((uint64_t *)output)[1], 1);
}

void md5_wd(unsigned char *input, int len, unsigned char output[16],
	    unsigned int chunk_sz)
{
	WATCHDOG_RESET();
	md5(input, len, output);
	WATCHDOG_RESET();
}
