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

/**
 * @file octeon_debugger_handler.S
 *
 * Debug exception handler used with the Octeon EJTAG debug exception
 * to provide access to core registers through memory.
 *
 * $Id: octeon_debugger_handler.S 24122 2007-04-27 16:27:15Z cchavva $
 *
 */

#define INCLUDE_BINARY(name, filename)  \
    .globl name##_begin;                \
    .globl name##_end;                  \
    .data;                              \
name##_begin:;                          \
    .incbin filename;                   \
name##_end:;

INCLUDE_BINARY(octeon_remote_debug_handler, "mips-octeon_debug_handler.bin")
INCLUDE_BINARY(octeon_remote_debug_handler2, "mips-octeon2_debug_handler.bin")
INCLUDE_BINARY(octeon_remote_debug_handler3, "mips-octeon3_debug_handler.bin")
INCLUDE_BINARY(octeon_remote_profile_handler, "mips-octeon_profile_handler.bin")

