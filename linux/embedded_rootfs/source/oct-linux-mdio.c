/****************************************************************
 * Copyright (c) 2003-2007, Cavium Inc. (support@cavium.com). All rights
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **************************************************************************/

/**
 * @file
 *
 * Utility to read and write SMI/MDIO under Linux
 *
 * $Id: oct-linux-memory.c 22261 2007-02-09 01:08:14Z kreese $
 *
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cvmx.h"
#include "cvmx-mdio.h"

/**
 * Convert a string into a 64bit number. Unfortunately
 * the normal sscanf %lli format doesn't support hex numbers
 * with the top bit set.
 *
 * @param str    String to convert to a number
 * @return The number, or zero on failure
 */
static uint64_t convert_number(const char *str)
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
    cvmx_linux_enable_xkphys_access(0);

    cvmx_sysinfo_linux_userspace_initialize();
    /* Make sure we got the correct number of arguments */
    if ((argc != 4) && (argc != 5))
    {
        printf("\n"
               "Usage: oct-linux-mdio <bus> <phy> <reg> [value]\n"
               "    Read or write a SMI/MDIO register.\n"
               "\n"
               "    bus     SMI/MDIO bus number. Normally zero.\n"
               "    phy     SMI/MDIO device address.\n"
               "    reg     Device register to read/write.\n"
               "    value   If present, write this value. Otherwise do a read.\n"
               "\n");
        return -1;
    }

    int bus = convert_number(argv[1]);
    int phy = convert_number(argv[2]);
    int reg = convert_number(argv[3]);

    if (argc == 5)
    {
        cvmx_mdio_write(bus, phy, reg, convert_number(argv[4]));
    }
    else
    {
        int value = cvmx_mdio_read(bus, phy, reg);
        printf("Bus %d, phy %d, reg %d = %d (0x%x)\n", bus, phy, reg, value, value);
    }
    return 0;
}

