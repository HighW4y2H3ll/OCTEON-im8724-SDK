/***********************license start***************
 * Copyright (c) 2010  Cavium Networks (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Networks nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM  NETWORKS MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

#ifndef __RENAME_CVMX_H__
#define __RENAME_CVMX_H__

/* Rename some functions so we don't export them into libc. */
#define __cvmx_debug_handler_stage2 __octeon_debug_handler_stage2
#define __cvmx_debug_handler_stage3 __octeon_debug_handler_stage3
#define cvmx_debug_init __octeon_debug_init
#define cvmx_debug_finish __octeon_debug_finish
#define cvmx_debug_trigger_exception __octeon_debug_trigger_exception
#define cvmx_debug_uart_comm __octeon_debug_uart_comm
#define cvmx_debug_remote_comm __octeon_debug_remote_comm
#define __cvmx_debug_stack_top __octeon_debug_stack_top
#define cvmx_debug_uart_process_debug_interrupt __octeon_debug_uart_process_debug_interrupt
#define __cvmx_debug_mode_exception_occured __octeon_debug_mode_exception_occured
#define __cvmx_debug_mode_exception_ignore __octeon_debug_mode_exception_ignore
#define __cvmx_debug_save_regs_area __octeon_debug_save_regs_area
#define __cvmx_debug_save_mpy_regs_area __octeon_debug_save_mpy_regs_area
#define __cvmx_debug_save_fp_regs_area __octeon_debug_save_fp_regs_area
#define __cvmx_debug_save_fp_control_area __octeon_debug_save_fp_control_area
#define cvmx_debug_uart __octeon_debug_uart
#define __cvmx_debug_mode_exception_save_area __octeon_debug_mode_exception_save_area
#define __cvmx_debug_is_octeon3 __octeon_debug_is_octeon3
#define __cvmx_debug_has_wide_mult __octeon_debug_has_wide_mult


#define cvmx_coremask_barrier_sync __octeon_coremask_barrier_sync
#define cvmx_uart_setup __octeon_uart_setup
#define cvmx_uart_setup2 __octeon_uart_setup2
#define cvmx_clock_get_rate __octeon_clock_get_rate
#define cvmx_coremask_print __octeon_coremask_print
#define cvmx_get_avail_coremask __octeon_get_avail_coremask
#define cvmx_get_hardware_coremask __octeon_get_hardware_coremask

#define cvmx_bootmem_init __octeon_bootmem_init
#define cvmx_bootmem_alloc __octeon_bootmem_alloc
#define cvmx_bootmem_alloc_address __octeon_bootmem_alloc_address
#define cvmx_bootmem_alloc_range __octeon_bootmem_alloc_range
#define cvmx_bootmem_alloc_range __octeon_bootmem_alloc_range
#define cvmx_bootmem_alloc_named __octeon_bootmem_alloc_named
#define cvmx_bootmem_alloc_named_address __octeon_bootmem_alloc_named_address
#define cvmx_bootmem_alloc_named_range __octeon_bootmem_alloc_named_range
#define cvmx_bootmem_free_named __octeon_bootmem_free_named
#define cvmx_bootmem_find_named_block __octeon_bootmem_find_named_block
#define cvmx_bootmem_available_mem __octeon_bootmem_available_mem
#define cvmx_bootmem_print_named __octeon_bootmem_print_named
#define cvmx_bootmem_phy_alloc __octeon_bootmem_phy_alloc
#define cvmx_bootmem_phy_named_block_alloc __octeon_bootmem_phy_named_block_alloc
#define cvmx_bootmem_phy_named_block_find __octeon_bootmem_phy_named_block_find
#define cvmx_bootmem_phy_available_mem __octeon_bootmem_phy_available_mem
#define cvmx_bootmem_phy_named_block_free __octeon_bootmem_phy_named_block_free
#define __cvmx_bootmem_phy_free __octeon_bootmem_phy_free
#define cvmx_bootmem_phy_named_block_print __octeon_bootmem_phy_named_block_print
#define cvmx_bootmem_phy_list_print __octeon_bootmem_phy_list_print
#define cvmx_bootmem_phy_mem_list_init __octeon_bootmem_phy_mem_list_init
#define cvmx_bootmem_lock __octeon_bootmem_lock
#define cvmx_bootmem_unlock __octeon_bootmem_unlock
#define __cvmx_bootmem_internal_get_desc_ptr __octeon_bootmem_internal_get_desc_ptr
#define cvmx_bootmem_alloc_named_range_once __octeon_bootmem_alloc_named_range_once
#define cvmx_bootmem_alloc_named_flags __octeon_bootmem_alloc_named_flags
#define cvmx_bootmem_alloc_named_range_flags __octeon_bootmem_alloc_named_range_flags
#define cvmx_bootmem_reserve_memory __octeon_bootmem_reserve_memory
#define __cvmx_bootmem_find_named_block_flags __octeon_bootmem_find_named_block_flags
#define cvmx_bootmem_phy_alloc_range __octeon_bootmem_phy_alloc_range
#define cvmx_bootmem_alloc_node __octeon_bootmem_alloc_node
#define cvmx_bootmem_phy_mem_list_init_multi __octeon_bootmem_phy_mem_list_init_multi
#define cvmx_clock_get_rate_node __octeon_clock_get_rate_node

#define cvmx_warn __octeon_warn

#define octeon_feature_init __octeon_feature_init
#define old_octeon_has_feature __octeon_has_feature
#define octeon_feature_map __octeon_feature_map
#define CVMX_BOOTMEM_NAMED_SET_NAME __OCTEON_BOOTMEM_NAMED_SET_NAME

#define cvmx_coremask_bmp2str __octeon_coremask_bmp2str
#define cvmx_coremask_str2bmp __octeon_coremask_str2bmp
#define __cvmx_phys_addr_to_ptr __octeon_phys_addr_to_ptr

#define octeon_attr_init __octeon_attr_init
#define octeon_attr_map __octeon_attr_map
#define octeon_has_attr __octeon_has_attr

#ifndef __ASSEMBLY__
#include <stdint.h>
#include "cvmx.h"
#include "cvmx-warn.h"
#include "cvmx-access.h"
#include "octeon-model.h"

#endif



#endif
