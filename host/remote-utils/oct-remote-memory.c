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
 * Utility to read or write memory over a remote interface
 *
 * $Id: oct-remote-memory.c 127530 2015-11-03 22:01:29Z cchavva $
 *
 *
 */
#include <stdint.h>
#include <stdio.h>
#include "cvmx.h"
#include "cvmx-version.h"
#include "octeon-remote.h"
#include "oct-remote-common.h"
#include "cvmx-swap.h"

static int      access_width = 8;
static int      do_write     = 0;
static uint64_t address      = 0;
static uint64_t value        = 0;
static int      count = 0;
static int      print_as_string = 0;

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

void usage(char * const *argv)
{
    printf("\n"
           "Usage: %s [-w width] [-c count] address [value]\n"
           "    Read or write bytes at a physical memory address.\n"
           "\n"
           "    width   Width of the access. Must be 8, 4, 2, or 1. Default is 8 bytes.\n"
           "    address Address of memory to read or write.\n"
           "    value   If present, write this value to the address. Otherwise do a read.\n"
       "    count   Is used only whe doing read and specifies the number of elements \n"
       "            to read\n"
           "\n"
           "\n", argv[0]);
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"width", 1, 0, 'w'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "w:hvc:s", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'w':
                access_width = convert_number(optarg);
                if ((access_width != 8) && (access_width != 4) &&
                    (access_width != 2) && (access_width != 1))
                {
                    printf("Illegal width. Must be 8, 4, 2, or 1\n");
                    return -1;
                }
                break;
            case 'c':
                count = convert_number(optarg);
                break;
            case 's':
                print_as_string = 1;
                break;
            default:
                usage(argv);
	    /* fallthrough */
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
                return -1;
        }
    }

    if (optind < argc)
    {
        address = convert_number(argv[optind]);
        if (optind+1 < argc)
        {
            value = convert_number(argv[optind+1]);
            do_write = 1;
        }
        else
            do_write = 0;
    }
    else
    {
        usage(argv);
        return -1;
    }
    return 0;
}


void print_mem(uint64_t address, int cnt,int width)
{
    uint8_t read_val[2048];
    int i;
    
    if ( print_as_string == 1) width = 1;
    cnt = cnt * width;
    while (cnt > 0)
    {
        int read_cnt;
        read_cnt = (cnt > 2048) ? 2048 : cnt ;
        cnt = cnt - read_cnt;
        octeon_remote_read_mem(&read_val,address, 2048);
        for (i=0; i< read_cnt; i+=width)
        {
            if (print_as_string == 0)
            {
                if (i == 0)
                    printf("%16llx : ", (unsigned long long)(address+i));
                else if (i % 32 == 0)
                    printf("\n%16llx : ", (unsigned long long)(address+i));
            }
            switch (width)
            {
            case 1:
                if (print_as_string)
                {
                    printf("%c",(char)read_val[i]);
                }
                else
                {
                    printf("%02x ",read_val[i]);
                }
                break;
            case 2:
                {
                    uint16_t data = *(uint16_t *) (&read_val[i]);
                    if (octeon_remote_is_mem_le())
                        data = cvmx_cpu_to_le16(data);
                    else
                        data = cvmx_cpu_to_be16(data);
                    printf("%04x ", data & 0xffff);
                }
                break;
            case 4:
                {
                    uint32_t data = *(uint32_t *) (&read_val[i]);
                    if (octeon_remote_is_mem_le())
                        data = cvmx_cpu_to_le32(data);
                    else
                        data = cvmx_cpu_to_be32(data);
                    printf("%08x ", data);
                }
                break;
            case 8:
                {
                    uint64_t data = *(uint64_t *) (&read_val[i]);
                    if (octeon_remote_is_mem_le())
                        data = cvmx_cpu_to_le64(data);
                    else
                        data = cvmx_cpu_to_be64(data);
                    printf("%016llx ", (unsigned long long)data);
                }
                break;
            }
        }
        address += read_cnt;
    }
    printf("\n");
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
    if (parse_options(argc, argv))
        return -1;

    /* Open a connection */
    if (octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0))
        return -1;

    if (!octeon_remote_mem_access_ok())
    {
        octeon_remote_close();
        printf("ERROR: DRAM not setup, board needs to be booted\n");
        return -1;
    }

    /* Determine if we're doing a read or a write */
    if (do_write)
    {
        switch (access_width)
        {
            case 8:
                octeon_remote_write_mem64(address, value);
                break;
            case 4:
                octeon_remote_write_mem32(address, value);
                break;
            case 2:
                octeon_remote_write_mem16(address, value);
                break;
            default:
            {
                uint8_t v = value;
                octeon_remote_write_mem(address, &v, 1);
                break;
            }
        }
    }
    else
    {
        if (count!=0)
        {
            print_mem(address, count, access_width);
        }
        else
        {
            switch (access_width)
            {
                case 8:
                    {
                        uint64_t v = octeon_remote_read_mem64(address);
                        printf("Address 0x%016llx = 0x%016llx\n",
                               (unsigned long long)address, (unsigned long long)v);
                        break;
                    }
                case 4:
                    {
                        uint32_t v = octeon_remote_read_mem32(address);
                        printf("Address 0x%016llx = 0x%08x\n",
                               (unsigned long long)address, v);
                        break;
                    }
                case 2:
                    {
                        uint16_t v = octeon_remote_read_mem16(address);
                        printf("Address 0x%016llx = 0x%04x\n",
                               (unsigned long long)address, 0xffff & v);
                        break;
                    }
               default:
                   {
                       uint8_t v;
                       octeon_remote_read_mem(&v, address, 1);
                       printf("Address 0x%016llx = 0x%02x\n",
                              (unsigned long long)address, 0xff & v);
                       break;
                   }
            }
        }
    }
    octeon_remote_close();
    return 0;
}

