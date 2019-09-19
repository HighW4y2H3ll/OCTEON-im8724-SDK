/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
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


/*
 * File version info: $Id: adma.c,v 1.4 2012/03/15 22:50:00 wshi Exp wshi $
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-coremask.h"
#include "cvmx-adma.h"

#ifdef __linux__

#include "adma-linux.c"

#else

static void dump_buf(char *title, char *buf, int len)
{
    unsigned char *p;
    int i;

    p = (unsigned char*) buf;

    printf("%s:\n    ", title);
    for (i = 0; i < len; i++)
    {
	printf("%02x", *p);
	if ((i + 1) % 20 == 0)
	    printf("\n    ");
        p++;
    }
    printf("\n\n");
}
#endif

int main(int argc, char *argv[])
{
#ifndef __linux__
    int i, val;
#define BUFSZ 1024
    char buf[BUFSZ];
#endif

    cvmx_user_app_init();

    if (cvmx_is_init_core())
    {
        if (!OCTEON_IS_MODEL(OCTEON_CNF71XX)) {
	    printf("adma runs only on a CNF71XX.\n");
#ifdef __linux__
	    return -1;
#else
            cvmx_reset_octeon();
#endif
	}

        cvmx_bbp_adma_init(0);

#ifdef __linux__
        return task_adma_linux(argc, argv);
#else
	/*
	 * The SE app demonstrates the usage of cvmx_adma_io() by
	 * writing and reading back SMEM on each tile,
	 *
	 * 1. initialize a buffer of BUFSZ bytes,
	 * 2. write the content of the buffer to the SMEM (starting
	 *    from offset n) on the tile, and
	 * 3. read back the content of the SMEM.
	 *
	 * After step 2 or 3, the first 128 bytes of the buffer are
	 * displayed.
	 */

#define ADMA_IO(mem, mem_str, chan, offset)				\
	do {								\
	    val = cvmx_adma_io((void *)buf,				\
	        (CVMX_BBP_PHY_##mem##_BASE + offset), BUFSZ,		\
		CVMX_ADMA_CHAN_##chan, CVMX_ADMA_MAX_BURST16, 0, 100);	\
	    if (val != CVMX_ADMA_IO_SUCCESS)				\
	        printf("cvmx_adma_io returns %d for %s\n",		\
		    val, mem_str);					\
	    else							\
	        dump_buf("Buffer content after " mem_str,		\
		    (char *)buf, 128);					\
	} while(0)
	    
#if 0
	for (i = 0; i < BUFSZ; i++) buf[i] = 0x33;
	ADMA_IO(SMEM2, "writing to SMEM2", RD3);
	ADMA_IO(SMEM2, "reading from SMEM2", WR3);
#endif

#if 1
	for (i = 0; i < BUFSZ; i++) buf[i] = 0x11;
	ADMA_IO(SMEM0, "writing to SMEM0", RD1, 2);
	ADMA_IO(SMEM0, "reading from SMEM0 right after writing", WR1, 2);

	cvmx_wait_usec(3000000);
	ADMA_IO(SMEM0, "reading from SMEM0 after 3 secs", WR1, 2);

	cvmx_wait_usec(3000000);
	for (i = 0; i < BUFSZ; i++) buf[i] = 0x44;
	ADMA_IO(SMEM0, "writing to SMEM0", RD1, 0);

	cvmx_wait_usec(3000000);
	ADMA_IO(SMEM0, "reading from SMEM0 after buf[i] set to 0x44", WR1, 0);

	cvmx_wait_usec(3000000);
	ADMA_IO(SMEM0, "reading from SMEM0 after buf[i] set to 0x44 again", WR1, 0);
#endif

#if 0
	for (i = 0; i < BUFSZ; i++) buf[i] = 0x22;
	ADMA_IO(SMEM1, "writing to SMEM1", RD2);
	ADMA_IO(SMEM1, "reading from SMEM1", WR2);

	cvmx_wait_usec(3000000);
	ADMA_IO(SMEM1, "reading from SMEM1 after 3 secs", WR2);

	cvmx_wait_usec(3000000);
	for (i = 0; i < BUFSZ; i++) buf[i] = 0x33;
	ADMA_IO(SMEM1, "writing to SMEM1", RD2);
	ADMA_IO(SMEM1, "reading from SMEM1", WR2);
#endif

        cvmx_reset_octeon();
#endif /* __linux__ */
    }

    return 0;
}
