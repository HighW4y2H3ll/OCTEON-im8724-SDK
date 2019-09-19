/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
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
 * Read a block of memory and save it to a file.
 *
 * $Id: oct-remote-save.c 69916 2012-02-14 14:47:03Z bprakash $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cvmx.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"

#define	MIN(a,b) (((a)<(b))?(a):(b))

void usage(const char *argv[])
{
    printf("\n"
           "Usage: %s <address> <length> <file>\n"
           "    Read Octeon DRAM and write contents to a file.\n"
           "\n"
           "    address     Physical memory address to read from.\n"
           "\n"
           "    length      Number of bytes to read.\n"
           "\n"
           "    file        Filename to save data in.\n"
           "\n"
           "\n", argv[0]);
}

int main(int argc, const char *argv[])
{
    unsigned long long address, cur_address;
    const char *filename = argv[3];
    FILE *infile;
    int bytes_to_read;

    int length_to_read = 0;
    int retval = 0;
    char buffer[65536];

    if (print_help_message(usage, argv, argc) == -1)
        return -1;

    if (argc != 4)
    {
        usage(argv);
        return -1;
    }

    if (sscanf(argv[1], "%lli", &address) != 1)
    {
        printf("Failed to parse the address\n");
        return -1;
    }
    if (sscanf(argv[2], "%i", &length_to_read) != 1)
    {
        printf("Failed to parse the length\n");
        return -1;
    }
    if (length_to_read <= 0)
    {
        printf("ERROR: length must be positive\n");
        return -1;
    }

    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;


    if (!octeon_remote_mem_access_ok())
    {
        octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }
    infile = fopen(filename, "w");
    if (infile == NULL)
    {
        perror("Unable to open file");
        octeon_remote_close();
        return -1;
    }

    /* Explicitly lock access to the bus, otherwise this is done for every
    ** read/write, which slows the transfer way down.
    */
    octeon_remote_lock();
    cur_address = address;
    bytes_to_read = length_to_read;
    while (bytes_to_read > 0)
    {
        int count = MIN(bytes_to_read, (int)sizeof(buffer));
        if (count > 0)
        {
            octeon_remote_read_mem(buffer, cur_address, count);
            cur_address += count;
            bytes_to_read -= count;
        }

        fwrite(buffer, 1, count, infile);
    }
    octeon_remote_unlock();


    fclose(infile);
    octeon_remote_close();
    return retval;
}

