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
 * This provides a simplied interface to mmaping /dev/mem to gain access to
 * physical addresses.
 *
 * $Id: octeon-remote-map.c 79870 2013-01-12 01:59:14Z cchavva $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "octeon-remote.h"
#include "octeon-remote-map.h"

void *octeon_remote_map_file(const char *filename, uint64_t physical_address, int length, octeon_remote_map_cookie_t *cookie)
{
    OCTEON_REMOTE_DEBUG_CALLED("0x%llx, %d, %p", (unsigned long long)physical_address, length, cookie);
    void *result;
    int pagemask = ~(sysconf(_SC_PAGESIZE)-1);
    int file_handle = open(filename, O_RDWR);
    if (file_handle<0)
    {
        octeon_remote_perror(-1, "open %s", filename);
        OCTEON_REMOTE_DEBUG_RETURNED("NULL");
        return NULL;
    }

    /* Align the size and address to the page boundary. */
    uint64_t offset = physical_address - (physical_address & (int64_t)pagemask);
    int alength = (length + offset + ~pagemask) & pagemask;

    result = mmap64(NULL, alength, PROT_READ|PROT_WRITE, MAP_SHARED, file_handle, physical_address & (int64_t)pagemask);
    if (result == MAP_FAILED)
    {
        octeon_remote_perror(-1, "mmap64");
        close(file_handle);
        OCTEON_REMOTE_DEBUG_RETURNED("NULL");
        return NULL;
    }

    close(file_handle);
    cookie->base = result;
    cookie->length = alength;
    OCTEON_REMOTE_DEBUG_RETURNED("%p", result + offset);
    return result + offset;
}

void *octeon_remote_map(uint64_t physical_address, int length, octeon_remote_map_cookie_t *cookie)
{
    return octeon_remote_map_file("/dev/mem", physical_address, length, cookie);
}

void octeon_remote_unmap(octeon_remote_map_cookie_t *cookie)
{
    OCTEON_REMOTE_DEBUG_CALLED("%p", cookie);
    munmap(cookie->base, cookie->length);
    OCTEON_REMOTE_DEBUG_RETURNED();
}

