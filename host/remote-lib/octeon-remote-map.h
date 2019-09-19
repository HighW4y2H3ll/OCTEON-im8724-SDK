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
#ifndef OCTEON_REMOTE_MAP_H
#define OCTEON_REMOTE_MAP_H

/**
 * @file
 *
 * This provides a simplied interface to mmaping /dev/mem to gain access to
 * physical addresses.
 *
 * $Id: octeon-remote-map.h 69916 2012-02-14 14:47:03Z bprakash $
 */
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
    void *base;
    int length;
} octeon_remote_map_cookie_t;

void *octeon_remote_map(uint64_t physical_address, int length, octeon_remote_map_cookie_t *cookie);
void *octeon_remote_map_file(const char *filename, uint64_t physical_address, int length, octeon_remote_map_cookie_t *cookie);
void octeon_remote_unmap(octeon_remote_map_cookie_t *cookie);

#ifdef	__cplusplus
}
#endif

#endif
