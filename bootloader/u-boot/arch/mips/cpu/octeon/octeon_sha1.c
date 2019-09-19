/*
 * FIPS-180-2 compliant SHA-256 implementation
 *
 * Copyright (C) 2001-2003  Christophe Devine
 * Copyright (C) 2012       Cavium, Inc. <support@cavium.com>
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
#include "sha1.h"
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

void sha1_starts(sha1_context *ctx)
{
	ctx->total[0] = 0;
	ctx->total[1] = 0;

	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xC3D2E1F0;

}

void sha1_update(sha1_context *ctx, const unsigned char *input,
		 unsigned int ilen)
{
	int fill;
	unsigned long left;
	uint64_t *ptr;
	uint64_t tmp;

	if (ilen <= 0)
		return;

	left = ctx->total[0] & 0x3f;
	fill = 64 - left;

	ctx->total[0] += ilen;
	ctx->total[0] &= 0xFFFFFFFF;

	if (ctx->total[0] < (unsigned long)ilen)
		ctx->total[1]++;

	/* Set the IV to the MD5 magic start value */
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[0]) << 32 | ctx->state[1], 0);
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[2]) << 32 | ctx->state[3], 1);
	CVMX_MT_HSH_IV(((uint64_t)ctx->state[4]) << 32, 2);

	if (left && ilen >= fill) {
		memcpy((void *)(ctx->buffer + left), (void *)input, fill);
		ptr = (uint64_t *)ctx->buffer;
		CVMX_MT_HSH_DAT(*ptr++, 0);
		CVMX_MT_HSH_DAT(*ptr++, 1);
		CVMX_MT_HSH_DAT(*ptr++, 2);
		CVMX_MT_HSH_DAT(*ptr++, 3);
		CVMX_MT_HSH_DAT(*ptr++, 4);
		CVMX_MT_HSH_DAT(*ptr++, 5);
		CVMX_MT_HSH_DAT(*ptr++, 6);
		CVMX_MT_HSH_STARTSHA(*ptr++);
		input += fill;
		ilen -= fill;
		left = 0;
	}

	while (ilen >= 64) {
		ptr = (uint64_t *)input;
		CVMX_MT_HSH_DAT(*ptr++, 0);
		CVMX_MT_HSH_DAT(*ptr++, 1);
		CVMX_MT_HSH_DAT(*ptr++, 2);
		CVMX_MT_HSH_DAT(*ptr++, 3);
		CVMX_MT_HSH_DAT(*ptr++, 4);
		CVMX_MT_HSH_DAT(*ptr++, 5);
		CVMX_MT_HSH_DAT(*ptr++, 6);
		CVMX_MT_HSH_STARTSHA(*ptr++);
		input += 64;
		ilen -= 64;
	}

	CVMX_MF_HSH_IV(tmp, 0);
	ctx->state[0] = tmp >> 32;
	ctx->state[1] = tmp & 0xffffffff;

	CVMX_MF_HSH_IV(tmp, 1);
	ctx->state[2] = tmp >> 32;
	ctx->state[3] = tmp & 0xffffffff;

	CVMX_MF_HSH_IV(tmp, 2);
	ctx->state[4] = tmp >> 32;

	if (ilen > 0) {
		memcpy((void *)(ctx->buffer + left), (void *)input, ilen);
	}
}

static const unsigned char sha1_padding[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void sha1_finish(sha1_context * ctx, unsigned char output[20])
{
	unsigned long last, padn;
	unsigned long high, low;
	unsigned char msglen[8];

	high = (ctx->total[0] >> 29)
		| (ctx->total[1] << 3);
	low = (ctx->total[0] << 3);

	PUT_UINT32_BE(high, msglen, 0);
	PUT_UINT32_BE(low, msglen, 4);

	last = ctx->total[0] & 0x3F;
	padn = (last < 56) ? (56 - last) : (120 - last);

	sha1_update(ctx, (unsigned char *) sha1_padding, padn);
	sha1_update(ctx, msglen, 8);

	PUT_UINT32_BE(ctx->state[0], output, 0);
	PUT_UINT32_BE(ctx->state[1], output, 4);
	PUT_UINT32_BE(ctx->state[2], output, 8);
	PUT_UINT32_BE(ctx->state[3], output, 12);
	PUT_UINT32_BE(ctx->state[4], output, 16);
}

void sha1_csum(const unsigned char *input, unsigned int ilen,
	       unsigned char *output)
{
	sha1_context ctx;

	memset(&ctx, 0, sizeof(ctx));
	sha1_starts(&ctx);
	sha1_update(&ctx, input, ilen);
	sha1_finish(&ctx, output);
}

/*
 * Output = SHA-1( input buffer ). Trigger the watchdog every 'chunk_sz'
 * bytes of input processed.
 */
void sha1_csum_wd(const unsigned char *input, unsigned int ilen,
		  unsigned char *output, unsigned int chunk_sz)
{
	sha1_context ctx;
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	const unsigned char *end, *curr;
	int chunk;
#endif
	memset(&ctx, 0, sizeof(ctx));
	sha1_starts(&ctx);

#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	curr = input;
	end = input + ilen;
	while (curr < end) {
		chunk = end - curr;
		if (chunk > chunk_sz)
			chunk = chunk_sz;
		sha1_update (&ctx, curr, chunk);
		curr += chunk;
		WATCHDOG_RESET();
	}
#else
	sha1_update(&ctx, input, ilen);
#endif

	sha1_finish(&ctx, output);
}

/*
 * Output = HMAC-SHA-1( input buffer, hmac key )
 */
void sha1_hmac (const unsigned char *key, int keylen,
		const unsigned char *input, unsigned int ilen,
		unsigned char *output)
{
	int i;
	sha1_context ctx;
	unsigned char k_ipad[64];
	unsigned char k_opad[64];
	unsigned char tmpbuf[20];

	memset(k_ipad, 0x36, 64);
	memset(k_opad, 0x5C, 64);

	for (i = 0; i < keylen; i++) {
		if (i >= 64)
			break;

		k_ipad[i] ^= key[i];
		k_opad[i] ^= key[i];
	}

	sha1_starts(&ctx);
	sha1_update(&ctx, k_ipad, 64);
	sha1_update(&ctx, input, ilen);
	sha1_finish(&ctx, tmpbuf);

	sha1_starts(&ctx);
	sha1_update(&ctx, k_opad, 64);
	sha1_update(&ctx, tmpbuf, 20);
	sha1_finish(&ctx, output);

	memset(k_ipad, 0, 64);
	memset(k_opad, 0, 64);
	memset(tmpbuf, 0, 20);
	memset(&ctx, 0, sizeof (sha1_context));
}

static const char _sha1_src[] = "_sha1_src";

#ifdef SELF_TEST
/*
 * FIPS-180-1 test vectors
 */
static const char sha1_test_str[3][57] = {
	{"abc"},
	{"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"},
	{""}
};

static const unsigned char sha1_test_sum[3][20] = {
	{0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
	 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D},
	{0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
	 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1},
	{0x34, 0xAA, 0x97, 0x3C, 0xD4, 0xC4, 0xDA, 0xA4, 0xF6, 0x1E,
	 0xEB, 0x2B, 0xDB, 0xAD, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6F}
};

/*
 * Checkup routine
 */
int sha1_self_test(void)
{
	int i, j;
	unsigned char buf[1000];
	unsigned char sha1sum[20];
	sha1_context ctx;

	for (i = 0; i < 3; i++) {
		printf("  SHA-1 test #%d: ", i + 1);

		sha1_starts(&ctx);

		if (i < 2)
			sha1_update(&ctx, (unsigned char *)sha1_test_str[i],
				    strlen (sha1_test_str[i]));
		else {
			memset(buf, 'a', 1000);
			for (j = 0; j < 1000; j++)
				sha1_update (&ctx, buf, 1000);
		}

		sha1_finish(&ctx, sha1sum);

		if (memcmp(sha1sum, sha1_test_sum[i], 20) != 0) {
			printf ("failed\n");
			return 1;
		}

		puts("passed\n");
	}

	puts("\n");
	return 0;
}
#else
int sha1_self_test(void)
{
	return 0;
}
#endif
