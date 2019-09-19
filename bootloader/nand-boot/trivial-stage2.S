/***********************license start***************
 * Copyright (c) 2003-2007 Cavium Inc. (support@cavium.com). All rights
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
#include <regdef.h>
#define __ASSEMBLY__
#include "../../executive/cvmx-bootloader.h"

#define UART_PORT                       0       /* Output to the first uart. This can be 0 or 1 */

/* These defines are Octeon CSRs and register offsets from the manual */
#define UART_BASE(port)     0x8001180000000800+0x400*(port)
#define UART_LCR            0x18
#define UART_FCR            0x50
#define UART_DLL            0x80
#define UART_DLH            0x88
#define UART_LSR            0x28
#define UART_THR            0x40
#define UART_USR            0x138

    #
    # This is our entry point. The processor starts fetching instructions here
    #
    .set noreorder
    .text
    .globl __start
__start:
    b   skip_header     /* Jump */
     nop
    .word   0 /* BOOTLOADER_HEADER_MAGIC */
    .word   0           /* Header CRC */
    .short  0 /* skip_header - __start */
    .short  0 /* BOOTLOADER_HEADER_CURRENT_MAJOR_REV */
    .short  0 /* BOOTLOADER_HEADER_CURRENT_MINOR_REV */
    .short  0           /* CVMX_BOARD_TYPE_GENERIC */
    .word   0           /* Data length */
    .word   0           /* Data CRC */
    .dword  0           /* Start address */
    .word   0           /* Flags */
    .short  0           /* BL_HEADER_IMAGE_STAGE2 */
    .short  0           /* Reserved */
    .word   0           /* Reserved 1 */
    .word   0           /* Reserved 2 */
    .word   0           /* Reserved 3 */
    .word   0           /* Reserved 4 */
    .space  BOOTLOADER_HEADER_COMMENT_LEN
    .space  BOOTLOADER_HEADER_VERSION_LEN
skip_header:
    # Print out the banner message
    dla     a0, MESSAGE_HELLO
    jal     uart_write_string
     nop
    .org    +65536
1:  b       1b
     nop

#
# void uart_write_string(const char *str)
#
uart_write_string:
    dli     t0, UART_BASE(UART_PORT)
    move    t1, a0          /* Save a0 since C code expects that */
1:
    lbu     t2, 0(t1)       /* Read next byte of string */
    beqz    t2, 3f          /* Jump out if the byte is a zero */
     daddu   t1, 1          /* Increment out pointer to the next byte */
2:
    ld      t3, UART_LSR(t0)/* Read LSR so we can see if the FIFO has room */
    and     t3, 1<<5        /* Bit 5 signals that the TX FIFO has room */
    beqz    t3, 2b          /* Loop if there isn't any room */
     nop
    b       1b              /* Jump to process the next byte and */
     sd      t2, UART_THR(t0) /* write the current byte in the delay slot */
3:
    j ra                    /* We're done, just return to the caller */
     nop

    # These are the messages we display before getting to the C code
MESSAGE_HELLO:      .string "Trival stage 2 - started\r\n\0"

