/*
 * FIPS-180-2 compliant SHA-256 implementation
 *
 * Copyright (C) 2001-2003  Christophe Devine
 * Copyrignt (C) 2012       Cavium, Inc. <support@cavium.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <watchdog.h>
#include "sha256.h"
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-key.h>
#include <asm/arch/cvmx-swap.h>

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i) {				\
	(n) = ( (unsigned long) (b)[(i)    ] << 24 )	\
	    | ( (unsigned long) (b)[(i) + 1] << 16 )	\
	    | ( (unsigned long) (b)[(i) + 2] <<  8 )	\
	    | ( (unsigned long) (b)[(i) + 3]       );	\
}
#endif
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i) {				\
	(b)[(i)    ] = (unsigned char) ( (n) >> 24 );	\
	(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );	\
	(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );	\
	(b)[(i) + 3] = (unsigned char) ( (n)       );	\
}
#endif

void sha256_starts(sha256_context * ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;

	ctx->state[0] = 0x6A09E667;
	ctx->state[1] = 0xBB67AE85;
	ctx->state[2] = 0x3C6EF372;
	ctx->state[3] = 0xA54FF53A;
	ctx->state[4] = 0x510E527F;
	ctx->state[5] = 0x9B05688C;
	ctx->state[6] = 0x1F83D9AB;
	ctx->state[7] = 0x5BE0CD19;
}

void sha256_csum_wd(const unsigned char *input, unsigned int ilen,
		    unsigned char *output, unsigned int chunk_sz)
{
	sha256_context ctx;
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	const unsigned char *end, *curr;
	int chunk;
#endif

	memset(&ctx, 0, sizeof(ctx));
	sha256_starts(&ctx);

#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	curr = input;
	end = input + ilen;
	while (curr < end) {
		chunk = end - curr;
		if (chunk > chunk_sz)
			chunk = chunk_sz;
		sha256_update(&ctx, curr, chunk);
		curr += chunk;
		WATCHDOG_RESET();
	}
#else
	sha256_update(&ctx, input, ilen);
#endif
	sha256_finish(&ctx, output);
}

void sha256_update(sha256_context *ctx, const uint8_t *input, uint32_t length)
{
	uint32_t left, fill;
	uint64_t *ptr;
	uint64_t tmp;

	if (!length)
		return;

	left = ctx->total[0] & 0x3F;
	fill = 64 - left;

	ctx->total[0] += length;
	ctx->total[0] &= 0xFFFFFFFF;

	if (ctx->total[0] < length)
		ctx->total[1]++;

	CVMX_MT_HSH_IV(((uint64_t)ctx->state[0]) << 32 | ctx->state[1], 0);
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[2]) << 32 | ctx->state[3], 1);
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[4]) << 32 | ctx->state[5], 2);
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[6]) << 32 | ctx->state[7], 3);

	if (left && length >= fill) {
		memcpy((void *) (ctx->buffer + left), (void *) input, fill);
		ptr = (uint64_t *)ctx->buffer;
		CVMX_MT_HSH_DAT(*ptr++, 0);
		CVMX_MT_HSH_DAT(*ptr++, 1);
		CVMX_MT_HSH_DAT(*ptr++, 2);
		CVMX_MT_HSH_DAT(*ptr++, 3);
		CVMX_MT_HSH_DAT(*ptr++, 4);
		CVMX_MT_HSH_DAT(*ptr++, 5);
		CVMX_MT_HSH_DAT(*ptr++, 6);
		CVMX_MT_HSH_STARTSHA256(*ptr++);

		length -= fill;
		input += fill;
		left = 0;
	}

	while (length >= 64) {
		ptr = (uint64_t *)input;
		CVMX_MT_HSH_DAT(*ptr++, 0);
		CVMX_MT_HSH_DAT(*ptr++, 1);
		CVMX_MT_HSH_DAT(*ptr++, 2);
		CVMX_MT_HSH_DAT(*ptr++, 3);
		CVMX_MT_HSH_DAT(*ptr++, 4);
		CVMX_MT_HSH_DAT(*ptr++, 5);
		CVMX_MT_HSH_DAT(*ptr++, 6);
		CVMX_MT_HSH_STARTSHA256(*ptr++);
		length -= 64;
		input += 64;
	}

	CVMX_MF_HSH_IV(tmp, 0);
	ctx->state[0] = tmp >> 32;
	ctx->state[1] = tmp & 0xffffffff;

	CVMX_MF_HSH_IV(tmp, 1);
	ctx->state[2] = tmp >> 32;
	ctx->state[3] = tmp & 0xffffffff;

	CVMX_MF_HSH_IV(tmp, 2);
	ctx->state[4] = tmp >> 32;
	ctx->state[5] = tmp & 0xffffffff;

	CVMX_MF_HSH_IV(tmp, 3);
	ctx->state[6] = tmp >> 32;
	ctx->state[7] = tmp & 0xffffffff;

	if (length)
		memcpy((void *) (ctx->buffer + left), (void *) input, length);
}

static const uint8_t sha256_padding[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void sha256_finish(sha256_context *ctx, uint8_t digest[SHA256_SUM_LEN])
{
	uint32_t last, padn;
	uint32_t high, low;
	uint8_t msglen[8];

	high = ((ctx->total[0] >> 29)
		| (ctx->total[1] << 3));
	low = (ctx->total[0] << 3);

	PUT_UINT32_BE(high, msglen, 0);
	PUT_UINT32_BE(low, msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	sha256_update(ctx, sha256_padding, padn);
	sha256_update(ctx, msglen, 8);

	PUT_UINT32_BE(ctx->state[0], digest, 0);
	PUT_UINT32_BE(ctx->state[1], digest, 4);
	PUT_UINT32_BE(ctx->state[2], digest, 8);
	PUT_UINT32_BE(ctx->state[3], digest, 12);
	PUT_UINT32_BE(ctx->state[4], digest, 16);
	PUT_UINT32_BE(ctx->state[5], digest, 20);
	PUT_UINT32_BE(ctx->state[6], digest, 24);
	PUT_UINT32_BE(ctx->state[7], digest, 28);
}
