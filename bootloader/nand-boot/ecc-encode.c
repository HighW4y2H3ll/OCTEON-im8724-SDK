/***********************license start***************
 * Copyright (c) 2003-2007 Cavium Inc. (support@cavium.com). All rights
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
#include <string.h>
#include <unistd.h>

void make_ecc(unsigned char *block, unsigned char *eccp)
{
	unsigned char pd0, pd1, pd2;
	int i, j;

	pd0 = pd1 = pd2 = 0;

	for (i = 0; i < 256; i++)	/* PD0<0> */
		pd0 ^= (block[i] ^ (block[i] >> 2) ^ (block[i] >> 4) ^ (block[i] >> 6)) & 1;
	for (i = 0; i < 256; i++)	/* PD0<1> */
		pd0 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 4) ^ (block[i] >> 5)) & 1) << 1;
	for (i = 0; i < 256; i++)	/* PD0<2> */
		pd0 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^ (block[i] >> 3)) & 1) << 2;
	for (i = 0; i < 128; i++)	/* PD0<3> */
		pd0 ^= ((block[2*i] ^ (block[2*i] >> 1) ^ (block[2*i] >> 2) ^
			(block[2*i] >> 3) ^ (block[2*i] >> 4) ^ (block[2*i] >> 5) ^
			(block[2*i] >> 6) ^ (block[2*i] >> 7)) & 1) << 3;
	for (i = 0; i < 64; i++)	/* PD0<4> */
		for (j = 0; j < 2; j++)
			pd0 ^= ((block[4*i+j] ^ (block[4*i+j] >> 1) ^ (block[4*i+j] >> 2) ^
				(block[4*i+j] >> 3) ^ (block[4*i+j] >> 4) ^ (block[4*i+j] >> 5) ^
				(block[4*i+j] >> 6) ^ (block[4*i+j] >> 7)) & 1) << 4;
	for (i = 0; i < 32; i++)	/* PD0<5> */
		for (j = 0; j < 4; j++)
			pd0 ^= ((block[8*i+j] ^ (block[8*i+j] >> 1) ^ (block[8*i+j] >> 2) ^
				(block[8*i+j] >> 3) ^ (block[8*i+j] >> 4) ^ (block[8*i+j] >> 5) ^
				(block[8*i+j] >> 6) ^ (block[8*i+j] >> 7)) & 1) << 5;
	for (i = 0; i < 16; i++)	/* PD0<6> */
		for (j = 0; j < 8; j++)
			pd0 ^= ((block[16*i+j] ^ (block[16*i+j] >> 1) ^ (block[16*i+j] >> 2) ^
				(block[16*i+j] >> 3) ^ (block[16*i+j] >> 4) ^ (block[16*i+j] >> 5) ^
				(block[16*i+j] >> 6) ^ (block[16*i+j] >> 7)) & 1) << 6;
	for (i = 0; i < 8; i++)		/* PD0<7> */
		for (j = 0; j < 16; j++)
			pd0 ^= ((block[32*i+j] ^ (block[32*i+j] >> 1) ^ (block[32*i+j] >> 2) ^
				(block[32*i+j] >> 3) ^ (block[32*i+j] >> 4) ^ (block[32*i+j] >> 5) ^
				(block[32*i+j] >> 6) ^ (block[32*i+j] >> 7)) & 1) << 7;
	for (i = 0; i < 4; i++)		/* PD1<0> */
		for (j = 0; j < 32; j++)
			pd1 ^= ((block[64*i+j] ^ (block[64*i+j] >> 1) ^ (block[64*i+j] >> 2) ^
				(block[64*i+j] >> 3) ^ (block[64*i+j] >> 4) ^ (block[64*i+j] >> 5) ^
				(block[64*i+j] >> 6) ^ (block[64*i+j] >> 7)) & 1) << 0;
	for (i = 0; i < 2; i++)		/* PD1<1> */
		for (j = 0; j < 64; j++)
			pd1 ^= ((block[128*i+j] ^ (block[128*i+j] >> 1) ^ (block[128*i+j] >> 2) ^
				(block[128*i+j] >> 3) ^ (block[128*i+j] >> 4) ^ (block[128*i+j] >> 5) ^
				(block[128*i+j] >> 6) ^ (block[128*i+j] >> 7)) & 1) << 1;
	for (i = 0; i < 128; i++)	/* PD1<2> */
		pd1 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^
			(block[i] >> 3) ^ (block[i] >> 4) ^ (block[i] >> 5) ^
			(block[i] >> 6) ^ (block[i] >> 7)) & 1) << 2;
	/* PD1<3> */
	/* PD1<4> */
	for (i = 0; i < 256; i++)	/* PD1<5> */
		pd1 ^= (((block[i] >> 1) ^ (block[i] >> 3) ^ (block[i] >> 5) ^ (block[i] >> 7)) & 1) << 5;
	for (i = 0; i < 256; i++)	/* PD1<6> */
		pd1 ^= (((block[i] >> 2) ^ (block[i] >> 3) ^ (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 6;
	for (i = 0; i < 256; i++)	/* PD1<7> */
		pd1 ^= (((block[i] >> 4) ^ (block[i] >> 5) ^ (block[i] >> 6) ^ (block[i] >> 7)) & 1) << 7;
	for (i = 0; i < 128; i++)	/* PD2<0> */
		pd2 ^= ((block[2*i+1] ^ (block[2*i+1] >> 1) ^ (block[2*i+1] >> 2) ^
			(block[2*i+1] >> 3) ^ (block[2*i+1] >> 4) ^ (block[2*i+1] >> 5) ^
			(block[2*i+1] >> 6) ^ (block[2*i+1] >> 7)) & 1) << 0;
	for (i = 0; i < 64; i++)	/* PD2<1> */
		for (j = 2; j < 4; j++)
			pd2 ^= ((block[4*i+j] ^ (block[4*i+j] >> 1) ^ (block[4*i+j] >> 2) ^
				(block[4*i+j] >> 3) ^ (block[4*i+j] >> 4) ^ (block[4*i+j] >> 5) ^
				(block[4*i+j] >> 6) ^ (block[4*i+j] >> 7)) & 1) << 1;
	for (i = 0; i < 32; i++)	/* PD2<2> */
		for (j = 4; j < 8; j++)
			pd2 ^= ((block[8*i+j] ^ (block[8*i+j] >> 1) ^ (block[8*i+j] >> 2) ^
				(block[8*i+j] >> 3) ^ (block[8*i+j] >> 4) ^ (block[8*i+j] >> 5) ^
				(block[8*i+j] >> 6) ^ (block[8*i+j] >> 7)) & 1) << 2;
	for (i = 0; i < 16; i++)	/* PD2<3> */
		for (j = 8; j < 16; j++)
			pd2 ^= ((block[16*i+j] ^ (block[16*i+j] >> 1) ^ (block[16*i+j] >> 2) ^
				(block[16*i+j] >> 3) ^ (block[16*i+j] >> 4) ^ (block[16*i+j] >> 5) ^
				(block[16*i+j] >> 6) ^ (block[16*i+j] >> 7)) & 1) << 3;
	for (i = 0; i < 8; i++)		/* PD2<4> */
		for (j = 16; j < 32; j++)
			pd2 ^= ((block[32*i+j] ^ (block[32*i+j] >> 1) ^ (block[32*i+j] >> 2) ^
				(block[32*i+j] >> 3) ^ (block[32*i+j] >> 4) ^ (block[32*i+j] >> 5) ^
				(block[32*i+j] >> 6) ^ (block[32*i+j] >> 7)) & 1) << 4;
	for (i = 0; i < 4; i++)		/* PD2<5> */
		for (j = 32; j < 64; j++)
			pd2 ^= ((block[64*i+j] ^ (block[64*i+j] >> 1) ^ (block[64*i+j] >> 2) ^
				(block[64*i+j] >> 3) ^ (block[64*i+j] >> 4) ^ (block[64*i+j] >> 5) ^
				(block[64*i+j] >> 6) ^ (block[64*i+j] >> 7)) & 1) << 5;
	for (i = 0; i < 2; i++)		/* PD2<6> */
		for (j = 64; j < 128; j++)
			pd2 ^= ((block[128*i+j] ^ (block[128*i+j] >> 1) ^ (block[128*i+j] >> 2) ^
				(block[128*i+j] >> 3) ^ (block[128*i+j] >> 4) ^ (block[128*i+j] >> 5) ^
				(block[128*i+j] >> 6) ^ (block[128*i+j] >> 7)) & 1) << 6;
	for (i = 128; i < 256; i++)	/* PD2<7> */
		pd2 ^= ((block[i] ^ (block[i] >> 1) ^ (block[i] >> 2) ^
			(block[i] >> 3) ^ (block[i] >> 4) ^ (block[i] >> 5) ^
			(block[i] >> 6) ^ (block[i] >> 7)) & 1) << 7;

	eccp[0] = pd0;
	eccp[1] = pd1;
	eccp[2] = pd2;
}

int main()
{
	int rv;
	unsigned char block[256];
	unsigned char ecc[8];

	memset(ecc, 0, 8);
        memset(block, 0xff, 256);

	while ((rv = read(0, block, 256)) > 0)
	{
		make_ecc(block, ecc);
		write(1, block, 256);
		write(1, ecc, 8);
                memset(block, 0xff, 256);
	}
        return 0;
}
