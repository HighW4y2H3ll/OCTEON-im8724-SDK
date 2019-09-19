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
 * Utility to read POW state
 *
 * <hr>$Revision: 70571 $<hr>
 */
#include <stdint.h>
#include <stdio.h>
#include <getopt.h>
#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "cvmx-pow.h"

int stop_cores = 0;

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"stop-cores", 0, 0, 's'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    int option_index = 0;
    int show_usage = 0;

    while (1)
    {
        char option = getopt_long(argc, argv, "shv", long_options, &option_index);
        if (option <= 0)
            break;

        switch (option)
        {
            case 's':
                stop_cores = 1;
                break;
            case 'h':
            default:
                show_usage = 1;
                break;
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING);
                return -1; 
                break;
        }
    }

    if (show_usage)
    {
        printf("\n"
               "Cavium Octeon POW state display\n"
               "\n"
               "Usage:\n"
               "    %s [options]\n"
               "\n"
               "Supported Options:\n"
               "  -s --stop-cores   # Stop the cores before capturing the POW state\n"
               "  -h --help         # Show this usage\n"
               "\n", argv[0]);
        printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
        return -1;
    }
    else
        return 0;
}

/**
 * Main
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char * const *argv)
{
    int number_of_cores;
    uint32_t coremask;
    char buffer[512<<10];

    if (parse_options(argc, argv))
        return -1;

    /* Open a connection */
    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    octeon_remote_lock();

    /* Determine the number of cores */
    number_of_cores = cvmx_octeon_num_cores();
    coremask = (1ull<<number_of_cores)-1;
    printf("Detected %d cores\n", number_of_cores);

    /* Stop the cores if needed. This makes the POW more stable
        so we get better results */
    if (stop_cores)
    {
        octeon_remote_stop_cores(coremask);
        usleep(100000);
    }

    /* Capture the POW state */
    cvmx_pow_capture(buffer, sizeof(buffer));

    /* Restart the cores */
    if (stop_cores)
        octeon_remote_start_cores(coremask);

    /* Finally display the POW state */
    octeon_remote_unlock();
    cvmx_pow_display(buffer, sizeof(buffer));
    octeon_remote_close();
    return 0;
}

