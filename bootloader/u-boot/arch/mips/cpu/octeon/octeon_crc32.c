/***********************license start************************************
 * Copyright (c) 2004-2011 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 * The watchdog portion of this file is derived from lib/crc32.c.
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
 * support@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <compiler.h>
#include <u-boot/crc.h>
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
#include <watchdog.h>
#endif
#include "u-boot/zlib.h"
#include <asm/arch/cvmx-asm.h>


#define ZEXPORT

/* Implement CRC using Octeon's CRC hardware unit.  This provides a significant
 * speed up, especially when the code is executing from flash (and the data
 * being CRC'ed is in flash, as is done during boot.)
 */
uint32_t ZEXPORT crc32_no_comp(uint32_t crc, const Bytef *buf, uInt len)
{
	volatile uint64_t *ptr64;
	volatile uint8_t *ptr = (void *)buf;
	uint32_t poly = 0;

	poly = 0x04c11db7; /* Bit reversed standard CRC32 polynomial - 0xedb88320 */
	CVMX_MT_CRC_POLYNOMIAL(poly);
	/* Do endian byte swap on CRC, then use reflect mode of
	 * MT IV to reverse the bits within the bytes.  This results
	 * in a bit-reversed IV. */
	CVMX_ES32(crc, crc);
	CVMX_MT_CRC_IV_REFLECT(crc);
	while ((((uint32_t)ptr) & 0x7) && len) {
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);
		len--;
	}
	ptr64 = (void *)ptr;
	while (len > 8) {
		CVMX_MT_CRC_DWORD_REFLECT(*ptr64++);
		len -= 8;
	}
	ptr = (void *)ptr64;
	while (len--)
		CVMX_MT_CRC_BYTE_REFLECT(*ptr++);

	CVMX_MF_CRC_IV_REFLECT(crc);

	return crc;
}

uint32_t ZEXPORT crc32 (uint32_t crc, const Bytef *p, uInt len)
{
     return crc32_no_comp(crc ^ 0xffffffffL, p, len) ^ 0xffffffffL;
}

/*
 * Calculate the crc32 checksum triggering the watchdog every 'chunk_sz' bytes
 * of input.
 */
uint32_t ZEXPORT crc32_wd (uint32_t crc,
			   const unsigned char *buf,
			   uInt len, uInt chunk_sz)
{
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	const unsigned char *end, *curr;
	int chunk;

	curr = buf;
	end = buf + len;
	while (curr < end) {
		chunk = end - curr;
		if (chunk > chunk_sz)
			chunk = chunk_sz;
		crc = crc32 (crc, curr, chunk);
		curr += chunk;
		WATCHDOG_RESET ();
	}
#else
	crc = crc32 (crc, buf, len);
#endif

	return crc;
}

void crc32_wd_buf(const unsigned char *input, unsigned int ilen,
		  unsigned char *output, unsigned int chunk_sz)
{
	uint32_t crc;
	crc = crc32_wd(0, input, ilen, chunk_sz);
	crc = htonl(crc);
	memcpy(output, &crc, sizeof(crc));
}
