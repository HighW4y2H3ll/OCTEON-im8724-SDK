/***********************license start************************************
 * Copyright (c) 2003-2008 Cavium Networks (support@cavium.com). All rights
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
 *     * Neither the name of Cavium Networks nor the names of
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
 * For any questions regarding licensing please contact marketing@caviumnetworks.com
 *
 **********************license end**************************************/

/**
 * @file
 *
 * Utility to read or write memory over a remote interface
 *
 * $Id$
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

static uint64_t address      = 0;
static int      print_as_string = 1;

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
           "Usage: oct-remote-print-buf address \n"
           "     : where address is the address of the first CPU buffer descriptor"
           "\n");
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "hv", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'h':
                usage(argv);
                return -1;
            case 1:
                break;
            default:
                usage(argv);
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING);
                return -1;
        }
    }

    if (optind < argc)
    {
        address = convert_number(argv[optind]);
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
                if (i == 0) printf("%16llux : ", (unsigned long long)address+i);
                else if (i % 32 == 0) printf("\n%16llux : ", (unsigned long long)address+i);
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
                    uint16_t data = cvmx_cpu_to_be16(*(uint32_t *) (&read_val[i]));
                    printf("%04x ", data & 0xffff);
                }
                break;
            case 4:
                {
                    uint32_t data = cvmx_cpu_to_be32(*(uint32_t *) (&read_val[i]));
                    printf("%08x ", data);
                }
                break;
            case 8:
                {
                    uint64_t data = cvmx_cpu_to_be64(*(uint64_t *) (&read_val[i]));
                    printf("%016llx ", (unsigned long long)data);
                }
                break;
            }
        }
        address += read_cnt;
    }
    printf("\n");
}


struct print_buf_desc
{
    uint64_t buf_start;
    uint64_t buf_end;
    uint64_t buf_wptr;
    uint32_t wrap_cnt;
    uint32_t magic;
};

int read_print_buf_desc(struct print_buf_desc *buf_desc, uint64_t address)
{

    buf_desc->buf_start = octeon_remote_read_mem64(address);
    buf_desc->buf_end   = octeon_remote_read_mem64(address+8);
    buf_desc->buf_wptr  = octeon_remote_read_mem64(address+16);
    buf_desc->wrap_cnt  = octeon_remote_read_mem32(address+24);
    buf_desc->magic     = octeon_remote_read_mem32(address+28);

    printf("s=%016llx e=%016llx w=%016llx c=%08x m=%08x l=%08llx \n", 
           (unsigned long long)buf_desc->buf_start,  
           (unsigned long long)buf_desc->buf_end,
           (unsigned long long)buf_desc->buf_wptr,
           buf_desc->wrap_cnt, buf_desc->magic,
           (unsigned long long)(buf_desc->buf_wptr - buf_desc->buf_start));
    return 0;
}

static struct print_buf_desc cpu_print_buf_desc[32] ;
static int cpu_cnt;

void init_cpu_print_buf_desc(void)
{
#define PRINT_BUF_OFFSET 0x80
    int i;

    for (i=0; i<32; i++)
    {
        read_print_buf_desc(&cpu_print_buf_desc[i], address);
        address += PRINT_BUF_OFFSET;
        if (cpu_print_buf_desc[i].magic != 0xdeadbeef)
        {
            cpu_cnt = i;
            break;
        }
    }
}

void show_print_buf(struct print_buf_desc *buf)
{

    uint32_t len = buf->buf_wptr - buf->buf_start ;
    if (buf->wrap_cnt == 0) print_mem(buf->buf_start & 0x7fffffff, len, 1);
    else
    {
        uint32_t len1 = buf->buf_end - buf->buf_wptr;
        print_mem(buf->buf_wptr &  0x7fffffff, len1, 1);
        print_mem(buf->buf_start & 0x7fffffff, len, 1);
    }

}

void show_all_print_bufs()
{

    int i;
    for (i=0; i<cpu_cnt; i++)
    {
        printf("------------------  CPU = %02d -------------- \n", i);
        show_print_buf(&cpu_print_buf_desc[i]);
    }

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

    //print_mem(address, 100, 1);
    init_cpu_print_buf_desc();
    show_all_print_bufs();
    octeon_remote_close();
    return 0;

}

