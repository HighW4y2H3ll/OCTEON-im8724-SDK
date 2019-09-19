/****************************************************************
 * Copyright (c) 2010-2011, Cavium Inc. (support@cavium.com). All rights
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
 **************************************************************************/

/**
 * @file
 *
 * Utility to modify QLM jtag chains
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "cvmx.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "cvmx-qlm.h"
#include "cvmx-helper-jtag.h"

void usage(const char *argv[])
{
    printf("\n"
           "Usage:\n"
           "  %s <qlm>\n"
           "    Read and display all QLM jtag settings.\n"
           "\n"
           "  %s <qlm> <lane> <name> <value>\n"
           "    Write a QLM \"lane\" jtag setting of \"name\" as \"value\".\n"
           "\n"
           "    qlm     Which QLM to access.\n"
           "    lane    Which lane to write a setting for or \"all\" for all lanes.\n"
           "    name    Name of qlm setting to write.\n"
           "    value   The value can be in decimal of hex (0x...).\n"
           "\n"
           "Incorrect settings can damage chips, so be careful!\n"
           "\n", argv[0], argv[0]);
}

uint64_t convert_number(const char *str)
{
    unsigned long long result;
    if ((str[0] == '0') && (str[1] == 'x'))
    {
        if (sscanf(str+2, "%llx", &result) != 1)
            result = 0;
    }
    else
    {
        if (sscanf(str, "%lli", &result) != 1)
            result = 0;
    }
    return result;
}

/**
 * Main
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char *argv[])
{
    int qlm;
    int c_arg;

    if (print_help_message(usage, argv, argc) == -1)
        return -1;

    /* Make sure we got the correct number of arguments */
    if (((argc - 2) % 3) != 0)
    {
        printf("Invalid number of arguments %d\n", argc);
        usage(argv);
        return -1;
    }

    /* Open a connection */
    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_remote_mem_access_ok())
    {
	octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    if (OCTEON_IS_OCTEON3()) {
        printf("oct-remote-qlm is not supported in OCTEON3\n");
        return 0;
    }

    cvmx_qlm_init();
    qlm = convert_number(argv[1]);
    if (qlm < 0 || qlm >= cvmx_qlm_get_num())
    {
        printf("ERROR: Invalid qlm(%d) passed\n", qlm);
        return -1;
    }

    if (argc >= 5)
    {
        int lane;
        int value;
        int field_count;
        int num_fields = (argc - 2) / 3;
        c_arg = 2;
        for (field_count = 0; field_count < num_fields; field_count++)
        {
            char name[30];
            if (strcmp(argv[c_arg], "all") == 0)
                lane = -1;
            else
                lane = (int)convert_number(argv[c_arg]);
            if (strlen(argv[c_arg + 1]) >= 30)
            {
                printf("Invalid qlm name passed.\n");
                return -1;
            }
            strcpy(name, argv[c_arg + 1]);
            value = convert_number(argv[c_arg + 2]);
            cvmx_qlm_jtag_set(qlm, lane, name, value);
            /* Assert serdes_tx_byp to force the new settings to override the
               QLM default. */
            if (strncmp(name, "biasdrv_", 8) == 0 || strncmp(name, "tcoeff_", 7) == 0)
                 cvmx_qlm_jtag_set(qlm, lane, "serdes_tx_byp", 1);
            c_arg += 3;
            if (lane == -1)
                break;
        }
    }
    else
        cvmx_qlm_display_registers(qlm);

    octeon_remote_close();
    return 0;
}
