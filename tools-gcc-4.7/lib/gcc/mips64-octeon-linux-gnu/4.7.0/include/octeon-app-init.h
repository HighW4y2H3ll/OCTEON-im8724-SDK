/***********************license start***************
 *  Copyright (c) 2003-2008 Cavium Networks (support@cavium.com). All rights
 *  reserved.
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials provided
 *        with the distribution.
 *
 *      * Neither the name of Cavium Networks nor the names of
 *        its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written
 *        permission.
 *
 *  TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 *  AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
 *  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 *  RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 *  REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 *  DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 *  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 *  PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 *  POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 *  OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 *  For any questions regarding licensing please contact marketing@caviumnetworks.com
 *
 ***********************license end**************************************/

/* Structures used to pass information from the bootloader to the application.
   This should not be used by the application directly.  */

#ifndef __OCTEON_APP_INIT_H__
#define __OCTEON_APP_INIT_H__

/* Define to allow conditional compilation in CVMX depending on
   version of the stand-alone Octeon support in newlib.  */
/* Version 2: added OCTEON_BL_FLAG_CONSOLE_PCI */
/* Version 3: added OCTEON_BL_FLAG_BREAK */
/* Version 4: added __octeon_uart_unlock */
#define OCTEON_APP_INIT_H_VERSION   4

/* Macro indicates that bootmem related structures are now in
   cvmx-bootmem.h.  */
#define OCTEON_APP_INIT_BOOTMEM_STRUCTS_MOVED

typedef enum
{
  /* If set, core should do app-wide init, only one core per app will have 
     this flag set.  */ 
  BOOT_FLAG_INIT_CORE     = 1,  
  OCTEON_BL_FLAG_DEBUG    = 1 << 1,
  OCTEON_BL_FLAG_NO_MAGIC = 1 << 2,
  OCTEON_BL_FLAG_CONSOLE_UART1 = 1 << 3,  /* If set, use uart1 for console */
  OCTEON_BL_FLAG_CONSOLE_PCI = 1 << 4,  /* If set, use PCI console */
  OCTEON_BL_FLAG_BREAK	  = 1 << 5, /* Call exit on break on serial port */
  /* Be sure to update OCTEON_APP_INIT_H_VERSION when new fields are added
  ** and to conditionalize the new flag's usage based on the version. */
} octeon_boot_descriptor_flag_t;

#define OCTEON_CURRENT_DESC_VERSION     7
/* Version 7 changes: Change names of deprecated fields */
#define OCTEON_ARGV_MAX_ARGS            (64)

#define OCTOEN_SERIAL_LEN 20

/* Bootloader structure used to pass info to Octeon executive startup code.
   NOTE: all fields are deprecated except for:
   * desc_version
   * desc_size,
   * heap_base
   * heap_end
   * eclock_hz
   * flags
   * argc
   * argv
   * cvmx_desc_vaddr
   * debugger_flags_base_addr

   All other fields have been moved to the cvmx_descriptor, and the new 
   fields should be added there. They are left as placeholders in this 
   structure for binary compatibility.  */
typedef struct
{   
  /* Start of block referenced by assembly code - do not change! */
  uint32_t desc_version;
  uint32_t desc_size;
  uint64_t stack_top;
  uint64_t heap_base;
  uint64_t heap_end;
  uint64_t deprecated17;   
  uint64_t deprecated16;
  /* End of block referenced by assembly code - do not change! */
  uint32_t deprecated18;
  uint32_t deprecated15;
  uint32_t deprecated14;
  uint32_t argc;  /* argc for main() */
  uint32_t argv[OCTEON_ARGV_MAX_ARGS];  /* argv for main() */
  uint32_t flags;   /* Flags for application */
  uint32_t core_mask;   /* Coremask running this image */
  uint32_t dram_size;  /* DEPRECATED, DRAM size in megabyes. Used up to SDK 1.8.1 */
  uint32_t phy_mem_desc_addr;  
  uint32_t debugger_flags_base_addr;  /* used to pass flags from app to debugger. */
  uint32_t eclock_hz;  /* CPU clock speed, in hz. */
  uint32_t deprecated10;  
  uint32_t deprecated9;  
  uint16_t deprecated8;
  uint8_t deprecated7;
  uint8_t deprecated6;
  uint16_t deprecated5;
  uint8_t deprecated4;
  uint8_t deprecated3;
  char deprecated2[OCTOEN_SERIAL_LEN];
  uint8_t deprecated1[6];
  uint8_t deprecated0;
  uint64_t cvmx_desc_vaddr;  /* Address of cvmx descriptor */
} octeon_boot_descriptor_t;

/* Debug flags bit definitions.  */
#define DEBUG_FLAG_CORE_DONE    0x1

#endif /* __OCTEON_APP_INIT_H__ */
