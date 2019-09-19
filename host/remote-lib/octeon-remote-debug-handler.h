/***********************license start************************************
 * Copyright (c) 2009 Cavium Inc. (support@cavium.com). All rights
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
#ifndef OCTEON_REMOTE_DEBUG_HANDLER_H
#define OCTEON_REMOTE_DEBUG_HANDLER_H

/**
 * @file
 *
 * This provides a simplied interface to install EJTAG debug exception
 * handlers into Octeon's memory.
 *
 * $Id: octeon-remote-map.h 41588 2009-03-19 19:41:00Z vmalov $
 */
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define PER_CORE_VAR_OFFSET	1024 /* the Variable region in the per-core 8KB space */

typedef enum
{
    OCTEON_REMOTE_DEBUG_HANDLER,    /* Save all registers, stop, and then restore all registers */
    OCTEON_REMOTE_PROFILE_HANDLER,  /* Sample PC and performance counters and keep running */
    OCTEON_REMOTE_SAVE_HANDLER,     /* Save all registers and keep running */
} octeon_remote_debug_handler_t;

int octeon_remote_debug_handler_install(octeon_remote_debug_handler_t handler, uint64_t data);
int octeon_remote_debug_handler_remove(void);
uint64_t octeon_remote_debug_handler_get_base(int core);
int octeon_remote_debug_handler_is_installed(void);

#ifdef	__cplusplus
}
#endif

#endif
