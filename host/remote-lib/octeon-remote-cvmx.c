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
 * This file supplies wrapper functions around the octeon-remote API to allow
 * CVMX based applications to run with minimal changes. These functions are
 * just renames of octeon-remote functions into the CVMX equivalent. These
 * are documented in cvmx-access.h.
 *
 * $Id: octeon-remote-cvmx.c 127530 2015-11-03 22:01:29Z cchavva $
 */
#include <stdint.h>
#include "cvmx.h"
#include "octeon-remote.h"
#undef CVMX_ADD_IO_SEG
#define CVMX_ADD_IO_SEG(a) (a)

uint64_t cvmx_read_csr(uint64_t csr_addr)
{
    return octeon_remote_read_csr(csr_addr);
}

uint64_t cvmx_read_csr_node(uint64_t node __attribute__ ((unused)), uint64_t csr_addr)
{
    return octeon_remote_read_csr(csr_addr);
}

void cvmx_write_csr(uint64_t csr_addr, uint64_t val)
{
    octeon_remote_write_csr(csr_addr, val);
}

void cvmx_write_csr_node(uint64_t node __attribute__ ((unused)), uint64_t csr_addr, uint64_t val)
{
    octeon_remote_write_csr(csr_addr, val);
}

uint64_t cvmx_read64_uint64(uint64_t csr_addr)
{
    return octeon_remote_read_mem64(csr_addr);
}

void cvmx_write64_uint64(uint64_t csr_addr, uint64_t val)
{
    octeon_remote_write_mem64(csr_addr, val);
}

uint32_t cvmx_read64_uint32(uint64_t csr_addr)
{
    if (octeon_remote_is_mem_le())
        csr_addr ^= 0x4ULL;
    return octeon_remote_read_mem32(csr_addr);
}

void cvmx_write64_uint32(uint64_t csr_addr, uint32_t val)
{
    if (octeon_remote_is_mem_le())
        csr_addr ^= 0x4ULL;
    octeon_remote_write_mem32(csr_addr, val);
}

int cvmx_dpop(uint64_t val)
{
    int r = 0;
    /* Unfortunately older compilers don't have popcount */
    while (val)
    {
        if (val&1)
            r++;
        val>>=1;
    }
    return r;
}

uint32_t cvmx_pop(uint32_t val)
{
	return cvmx_dpop(val);
}


uint32_t cvmx_get_proc_id(void)
{
    return octeon_remote_get_model();
}

uint8_t cvmx_fuse_read_byte(int byte_addr)
{
    cvmx_mio_fus_rcmd_t read_cmd;

    read_cmd.u64 = 0;
    read_cmd.s.addr = byte_addr;
    read_cmd.s.pend = 1;
    octeon_remote_write_csr(CVMX_MIO_FUS_RCMD, read_cmd.u64);
    while ((read_cmd.u64 = octeon_remote_read_csr(CVMX_MIO_FUS_RCMD)) && read_cmd.s.pend)
        ;
    return(read_cmd.s.dat);
}

int cvmx_fuse_read(int fuse)
{
    return((cvmx_fuse_read_byte(fuse >> 3) >> (fuse & 0x7)) & 1);
}

/**
 * Wait for the specified number of core clock cycles
 *
 * @param cycles
 */
void cvmx_wait(uint64_t cycles)
{
    usleep(cycles/1000);
}

/**
 * Wait for the specified number of micro seconds
 *
 * @param usec   micro seconds to wait
 */
void cvmx_wait_usec(uint64_t usec)
{
    usleep(usec);
}

/**
 * Wait for the specified number of io clock cycles
 *
 * @param cycles
 */
void cvmx_wait_io(uint64_t cycles)
{
    uint64_t done = cvmx_clock_get_count(CVMX_CLOCK_SCLK) + cycles;
                                                                                
    while (cvmx_clock_get_count(CVMX_CLOCK_SCLK) < done)
    {
        /* Spin */
    }
}

unsigned int cvmx_get_node_num(void)
{
	return 0;
}
