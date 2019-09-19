/***********************license start************************************
 * Copyright (c) 2003-2009 Cavium Inc. (support@cavium.com). All rights
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
 **********************license end**************************************/

/**
 * @file
 *
 * Load a file into memory on a remote Octeon
 *
 * $Id: oct-remote-bootcmd.c 69916 2012-02-14 14:47:03Z bprakash $
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "cvmx.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"

void usage(const char *argv[])
{
        printf("\n"
               "Usage: %s \"command\"\n"
               "    Give command to bootloader running on Octeon in PCI slot.\n"
               "Command must be double quoted if it contains whitespace.\n"
               "\n", argv[0]);
}

int main(int argc, const char *argv[])
{
    int retval;

    if (print_help_message(usage, argv, argc) == -1)
	return -1;

    if (argc != 2)
    {
	usage(argv);
        return -1;
    }

    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    retval = octeon_remote_send_bootcmd(argv[1], 200);
    if (retval)
        printf("ERROR sending command to bootloader\n");

    octeon_remote_close();
    return retval;
}

