/***********************license start***************
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
 ***********************license end**************************************/

/**
 * @file
 *
 * This file supports accessing the u-boot "PCI console" structures over a
 * remote transport. This console was originally only supported over PCI,
 * hence the name, but can now be accessed over PCI, PCIe, and EJTAG.
 *
 * $Id: octeon-remote-console.c 69916 2012-02-14 14:47:03Z bprakash $
 */

#define	MIN(a,b) (((a)<(b))?(a):(b))
#include "cvmx.h"
#include "cvmx-bootmem.h"
#include "octeon-pci-console.h"
#include "octeon-remote.h"

static struct
{
    uint64_t console_desc_addr;
    unsigned int console_num;
    uint32_t num_consoles;
    uint64_t console_addr;
    uint32_t console_buffer_size;
    uint64_t input_base_addr;
    uint64_t output_base_addr;
    uint64_t base_addr;
    uint32_t rd_idx;
    uint32_t wr_idx;
} console_cache;

static int console_buffer_free_bytes(uint32_t buffer_size, uint32_t wr_idx, uint32_t rd_idx)
{
    if (rd_idx >= buffer_size || wr_idx >= buffer_size)
        return -1;

    return (((buffer_size -1) - (wr_idx - rd_idx))%buffer_size);
}
static int console_buffer_avail_bytes(uint32_t buffer_size, uint32_t wr_idx, uint32_t rd_idx)
{
    if (rd_idx >= buffer_size || wr_idx >= buffer_size)
        return -1;

    return (buffer_size - 1 - console_buffer_free_bytes(buffer_size, wr_idx, rd_idx));
}

static int console_cache_update(uint64_t console_desc_addr, unsigned int console_num, int is_output)
{
    if (!console_desc_addr)
        return -1;
    if ((console_desc_addr != console_cache.console_desc_addr) ||
        (console_num != console_cache.console_num))
    {
        console_cache.num_consoles = octeon_remote_read_mem32(console_desc_addr + offsetof(octeon_pci_console_desc_t, num_consoles));
        if (console_num >= console_cache.num_consoles)
        {
            printf("ERROR: attempting to access non-existant console: %d\n", console_num);
            return(-1);
        }
        console_cache.console_desc_addr = console_desc_addr;
        console_cache.console_num = console_num;
        console_cache.console_addr = octeon_remote_read_mem64(console_cache.console_desc_addr + offsetof(octeon_pci_console_desc_t, console_addr_array) + console_cache.console_num*8);
        console_cache.console_buffer_size = octeon_remote_read_mem32(console_cache.console_addr + offsetof(octeon_pci_console_t, buf_size));
        console_cache.input_base_addr = octeon_remote_read_mem64(console_cache.console_addr + offsetof(octeon_pci_console_t, input_base_addr));
        console_cache.output_base_addr = octeon_remote_read_mem64(console_cache.console_addr + offsetof(octeon_pci_console_t, output_base_addr));
    }
    if (is_output)
    {
        console_cache.base_addr = console_cache.output_base_addr;
        uint64_t index = octeon_remote_read_mem64(console_cache.console_addr + offsetof(octeon_pci_console_t, output_read_index));
        console_cache.rd_idx = index>>32;
        console_cache.wr_idx = index;
    }
    else
    {
        console_cache.base_addr = console_cache.input_base_addr;
        uint64_t index = octeon_remote_read_mem64(console_cache.console_addr + offsetof(octeon_pci_console_t, input_read_index));
        console_cache.rd_idx = index>>32;
        console_cache.wr_idx = index;
    }

    return 0;
}


int octeon_remote_console_host_write(uint64_t console_desc_addr, unsigned int console_num, const char * buffer, int write_reqest_size, uint32_t flags __attribute__ ((unused)))
{
    if (console_cache_update(console_desc_addr, console_num, 0))
        return -1;
    int bytes_to_write = console_buffer_free_bytes(console_cache.console_buffer_size, console_cache.wr_idx, console_cache.rd_idx);
    if (bytes_to_write <= 0)
        return bytes_to_write;
    bytes_to_write = MIN(bytes_to_write, write_reqest_size);
    /* Check to see if what we want to write is not contiguous, and limit ourselves to the contiguous block*/
    if (console_cache.wr_idx + bytes_to_write >= console_cache.console_buffer_size)
        bytes_to_write = console_cache.console_buffer_size - console_cache.wr_idx;

    octeon_remote_write_mem(console_cache.base_addr + console_cache.wr_idx, buffer, bytes_to_write);
    octeon_remote_write_mem32(console_cache.console_addr + offsetof(octeon_pci_console_t, input_write_index), (console_cache.wr_idx + bytes_to_write)%console_cache.console_buffer_size);

    return bytes_to_write;

}

int octeon_remote_console_host_read(uint64_t console_desc_addr, unsigned int console_num, char * buffer, int buf_size, uint32_t flags __attribute__ ((unused)))
{
    if (console_cache_update(console_desc_addr, console_num, 1))
        return -1;

    int bytes_to_read = console_buffer_avail_bytes(console_cache.console_buffer_size, console_cache.wr_idx, console_cache.rd_idx);
    if (bytes_to_read <= 0)
        return bytes_to_read;

    bytes_to_read = MIN(bytes_to_read, buf_size);
    /* Check to see if what we want to read is not contiguous, and limit ourselves to the contiguous block*/
    if (console_cache.rd_idx + bytes_to_read >= console_cache.console_buffer_size)
        bytes_to_read = console_cache.console_buffer_size - console_cache.rd_idx;

    octeon_remote_read_mem(buffer, console_cache.base_addr + console_cache.rd_idx, bytes_to_read);
    octeon_remote_write_mem32(console_cache.console_addr + offsetof(octeon_pci_console_t, output_read_index), (console_cache.rd_idx + bytes_to_read)%console_cache.console_buffer_size);

    return bytes_to_read;
}


int octeon_remote_console_host_write_avail(uint64_t console_desc_addr, unsigned int console_num)
{
    if (console_cache_update(console_desc_addr, console_num, 0))
        return -1;
    return console_buffer_free_bytes(console_cache.console_buffer_size, console_cache.wr_idx, console_cache.rd_idx);
}


int octeon_remote_console_host_read_avail(uint64_t console_desc_addr, unsigned int console_num)
{
    if (console_cache_update(console_desc_addr, console_num, 1))
        return -1;
    return console_buffer_avail_bytes(console_cache.console_buffer_size, console_cache.wr_idx, console_cache.rd_idx);
}

