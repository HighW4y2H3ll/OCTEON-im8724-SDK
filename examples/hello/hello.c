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
 * File version info: $Id: hello.c 104184 2014-09-10 16:54:34Z cchavva $
 */
 
#include <stdio.h>


int main(void)
{
    /* There are two main ways for a program running on the simulator to
    ** produce output: printf and simprintf.  Both (by default) generate
    ** output to the stdout of the simulator, with each line prepended
    ** with a string similar to 'PP#:~CONSOLE-> ', with # being the number (id)
    ** of the core generating the output.  The -log option may be used
    ** to direct this output to log files.
    */

    /* printf() provides maximum flexibility, but is slow due to
    ** the format string being processed in simulated code. Normal
    ** buffering is done by the C library.
    */
    printf("\n");
    printf("\n");
    printf("Hello world!\n");

#if 0
    /* simprintf() passes the format string and up to 7 arguments to the
    ** simulator and is much faster than standard printfs.  It is limited
    ** to 7 arguments of integer types, and all must use long long (%ll)
    ** formats in order to be processed properly by the host.
    ** No buffering is done - output of each simprintf call is immediate.
    ** See README in the 'runtime' directory for more info.
    */
    simprintf("Hello again - a big number is 0x%llx\n", 0x1234567887654321ULL);
#endif

    printf("Hello example run successfully.\n");
    return 0;
}
