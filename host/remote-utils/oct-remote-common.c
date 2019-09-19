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
 * Interface to common functions used by multiple oct-remote-* utilities.
 *
 * $Id: oct-remote-common.c 40514 2009-02-18 12:10:30Z cchavva $
 */

#include "cvmx.h"
#include "cvmx-version.h"
#include "oct-remote-common.h"

/**
 * Print command usage when invoked with -h/--help.
 *
 * @param func      Name of the function pointer, to print the usage
 * @param func_args Arguments to the function pointer
 * @param argc      Number of arguments the command is invoked with
 *
 * @return -1 if invoked with -h/--help option else return 0.
 */
int print_help_message(usage_ftype *func, const char **func_args, int argc)
{
    int c;
                                                                                
    /* Parse command line options using getopt_long */
    while (1)
    {
        static struct option long_options[] =
        {
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, (char * const *)func_args, "hv",
                         long_options, &option_index);                                                                                 
        /* Detect the end of the options. */
        if (c == -1)
            break;
                                                                                
        switch (c)
        {
        case 'h':
        case '?':
            /* getopt_long already printed an error message. */
            func (func_args);
	    /* fallthrough */
        case 'v': 
            printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
            return -1;
            break;
                                                                                
        default:
            break;
        }
    }
    return 0;
}



/**
 * Process an SPD argument that could either be an SPD TWSI address
 * or a filename containing SPD contents.
 * 
 * @param d_cfg_ptr pointer to DIMM structure to fill in
 * @param spd_arg   SPD argument (number or filename)
 * @param index     DIMM index (only non-zero for 128 bit interfaces)
 * 
 * @return 0 on success
 *         -1 on failure
 */
int process_spd_arg(dimm_config_t *d_cfg_ptr, const char *spd_arg, int index)
{
    FILE *fp;
    int addr;

    /* Try to open the arg as a file, if this fails treat is as an address */

    fp = fopen(spd_arg, "r");
    if (fp)
    {
        int i;
        d_cfg_ptr->spd_ptrs[index] = malloc(128);
        unsigned int val;
        if (!d_cfg_ptr->spd_ptrs[index])
        {
            printf("ERROR: unable to allocate memory for SPD values\n");
            exit(-1);
        }
        /* We have a file, so allocate some memory, and read contents. */
        for (i = 0; i < 128; i++)
        {
            if (!fscanf(fp, "%x", &val))
                break;
            d_cfg_ptr->spd_ptrs[index][i] = val;
        }

        /* We need at least 64 values */
        if (i < 64)
        {
            printf("ERROR: Fewer than the minimum of 64 values found in spd file: %s\n", spd_arg);
	    fclose (fp);
            return -1;
        }
        printf("Read %d values from spd file: %s\n", i, spd_arg);
    }
    else
    {
        addr = strtol(spd_arg, NULL, 0);
        d_cfg_ptr->spd_addrs[index]=addr;
    }

    if (fp)
	fclose (fp);
    return 0;
}
