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

#define OCTEON_CN52XX                   0x000d0700
#define OCTEON_CN63XX                   0x000d9000
#define OCTEON_REFERENCE_CLOCK          50      /* Most all Octeon boards use a 50Mhz reference clock */
#define UART_PORT                       0       /* Output to the first uart. This can be 0 or 1 */
#define UART_BAUD_RATE                  115200  /* Baud rate for the uart */
#define STACK_CACHE_LINES               54      /* Number of cache lines to use in scratch for a stack */
#define NAND_BOOT_TIM_MULT              8       /* NAND timing values for early boot */
/* IMPORTANT: NAND_BOOT_PAGE_BITS _must_ be updated to match the NAND part used. */

#if OCTEON_MODEL == OCTEON_CN52XX
#define NAND_BOOT_PAGE_BITS             11      /* NAND on EBT5200 uses 2048 byte pages, or 1<<11 bits */
#else
#define NAND_BOOT_PAGE_BITS             12      /* NAND on EBB6300 uses 4096 byte pages, or 1<<12 bits */
#endif
#define NAND_BOOT_ADDRESS_CYCLES        5       /* Number of NAND address cycles to strobe. Normally either 4 or 5 */
#define USE_INDEX_ALIASING              1       /* Should L2 index alias be used? */
//#define USE_LONG_BIST                   1       /* Run long BIST on L2? */

/* These defines are Octeon CSRs and register offsets from the manual */
#define NDF_BT_PG_INFO      0x8001070001000018
#define NPEI_DBG_DATA       0x80011F0000008510
#define MIO_RST_BOOT        0x8001180000001600
#define L2C_DBG             0x8001180080000030
#define L2C_CFG             0x8001180080000000
#define L2T_ERR             0x8001180080000008
#define L2C_LCKOFF          0x8001180080000060
#define L2C_LCKBASE         0x8001180080000058
#define L2C_CTL             0x8001180080800000
#define L2C_BST_MEM0        0x8001180080C007F8
#define UART_BASE(port)     0x8001180000000800+0x400*(port)
#define UART_LCR            0x18
#define UART_FCR            0x50
#define UART_DLL            0x80
#define UART_DLH            0x88
#define UART_LSR            0x28
#define UART_THR            0x40
#define UART_USR            0x138
#define COP0_CVMCTL         $9,7
#define COP0_CVMMMECTL      $11,7
#define COP0_STATUS         $12,0

    # Global symbols for jumps
    .globl __start
    .globl main
    .globl uart_write_char
    .globl uart_write_string
    .globl uart_write_hex
    .globl jump_to_bootloader
    .globl handle_exception_stub

    # This macro converts a symbol into a relative offset so we can use symbols
    # in flash that are linked at L2 addresses. This macro clobbers "ra".
    .macro dla_rel output, symbol
    dla     \output, \symbol
    dla     ra, 1f
    dsubu   \output, ra
    bgezal  $0, 1f
     nop
1:  daddu   \output, ra
    .endm


    #
    # This is our entry point. The processor starts fetching instructions here
    #
    .text
    .set noreorder
    .ent __start
__start:
    # Enable 64bit addressing and disable BEV. BEV causes exceptions to jump
    # to flash addresses which don't exist in NAND.
    mfc0    t0, COP0_STATUS
    ori     t0, 1<<7
    and     t0, ~(1<<22)
    mtc0    t0, COP0_STATUS

    # Set NAND page size and columns
    dli     t0, (NAND_BOOT_TIM_MULT<<7) | (NAND_BOOT_ADDRESS_CYCLES<<3) | (NAND_BOOT_PAGE_BITS-9)
    dli     t1, NDF_BT_PG_INFO
    sd      t0, 0(t1)
    ld      $0, 0(t1)
    # NOTE: The above sequence of instructions must all fit in the first 256 bytes
    #       of NAND flash. This is so the number of address cycles can be set
    #       before we do a fetch from something other than address zero.

    # Disable Icache prefetching so a stray prefetch can't access DRAM that
    # isn't setup yet.
    dmfc0   t0, COP0_CVMCTL
    ori     t0, 1<<13
    dmtc0   t0, COP0_CVMCTL

    # Setup UART to UART_BAUD_RATE,8,N,1
    dli     s0, UART_BASE(UART_PORT)
    li      t0, 0x83            /* Set 8 bits and allow divisor programming */
    sd      t0, UART_LCR(s0)
    li      t0, 0x1             /* Enable the UART FIFO */
    sd      t0, UART_FCR(s0)
#if OCTEON_MODEL == OCTEON_CN52XX
    dli     t0, NPEI_DBG_DATA   /* Get Octeon's clock multiplier from NPEI_DBG_DATA */
    ld      t1, 0(t0)
    dsrl    t1, 18              /* Multiplier is bits <22:18> */
    andi    t1, 0x1f
#else
    dli     t0, MIO_RST_BOOT    /* Get Octeon2's sclk multiplier from MIO_RST_BOOT */
    ld      t1, 0(t0)
    dsrl    t1, 24              /* Multiplier is bits <29:24> */
    andi    t1, 0x3f
#endif
    dmul    s1, t1, OCTEON_REFERENCE_CLOCK*1000000  /* Multiplier*ref*1000000 = Core clock in Hz */
    # Set the divisor, which is (core clock) / UART_BAUD_RATE / 16
    li      t0, UART_BAUD_RATE
    ddiv    $0, s1, t0          /* Divide Octeon's clock by the baudrate */
    mflo    t0                  /* Result of divide was in LO */
    sra     t0, 4               /* Divide by 16 to convert to uart divisor */
    sra     t1, t0, 8           /* Get higher byte for DLH */
    sd      t1, UART_DLH(s0)
    andi    t1, t0, 0xff        /* Get lower byte for DLL */
    sd      t1, UART_DLL(s0)
    # Enable the UART after the divisor is set
    ld      t0, UART_LCR(s0)    /* Read LCR so we can update the divisor latch */
    and     t0, 0x7f            /* Force divisor latch to be zero */
    sd      t0, UART_LCR(s0)    /* Store the new LCR */
    ld      $0, UART_LCR(s0)    /* Read it back to make sure the update is complete */
    li      t0, 0xff            /* Spin for a while waiting for the uart to be ready */
1:  bnez    t0, 1b
     addu   t0, -1

    # Print out the banner message
    dla_rel a0, MESSAGE_HELLO
    bal     uart_write_string
     nop

    # See if we should run long BIST on L2. Results will be checked later
    # in the C code.
#if USE_LONG_BIST
#if OCTEON_MODEL == OCTEON_CN52XX
    dli     t0, L2C_CFG
    ld      t1, 0(t0)           /* Load the L2 config register */
    or      t1, 1 << 18         /* Set the long BIST start bit */
    sd      t1, 0(t0)           /* Store the new L2 config */
    ld      t1, 0(t0)           /* Read the status */
1:  bbit1   t1, 19, 1b          /* Loop until BIST is complete */
     ld     t1, 0(t0)           /* Read the status */
#else
    dli     t0, L2C_BST_MEM0
    ld      t1, 0(t0)           /* Load the L2 bist register */
    or      t1, 1 << 62         /* Set the clear bit */
    sd      t1, 0(t0)           /* Store the new L2 bist register */
    or      t1, 1 << 63         /* Set the start bit */
    sd      t1, 0(t0)           /* Store the new L2 bist register */
1:  bbit1   t1, 63, 1b          /* Loop until BIST is complete */
     ld     t1, 0(t0)           /* Read the status */
#endif
#endif

    # If the index aliasing feature is going to be used, it must be
    # enabled before anything is put into the cache.
#if OCTEON_MODEL == OCTEON_CN52XX
#if USE_INDEX_ALIASING
    dli     t0, L2C_CFG
    ld      t1, 0(t0)           /* Load the L2 config register */
    or      t1, 1 << 7          /* Set the index aliasing bit */
    sd      t1, 0(t0)           /* Store the new L2 config */
    ld      $0, 0(t0)           /* Make sure write has completed */
#endif
#else
    dli     t0, L2C_CTL
    ld      t1, 0(t0)           /* Load the L2 control register */
#if USE_INDEX_ALIASING
    and     t1, -2              /* Clear the index aliasing disable bit */
#else
    or      t1, 1               /* Set the index aliasing disable bit */
#endif
    sd      t1, 0(t0)           /* Store the new L2 control */
    ld      $0, 0(t0)           /* Make sure write has completed */
#endif

    # Lock stage 1 in the L2 cache
#if OCTEON_MODEL == OCTEON_CN52XX
    dli     t0, L2C_DBG
    sd      $0, 0(t0)           /* Make Core 0 the debug core */
    dli     t0, L2C_LCKOFF      /* Lock 1 line at a time, base is moved each time */
    li      t1, 0x1
    sd      t1, 0(t0)

    dla     s0, __start         /* Address we start locking at */
    dla     s1, _end            /* Get the end of our code */
    dsubu   s1, s0              /* Subtract start from end to get our size in bytes */
    andi    s1, 0x7fff          /* _end gets extra bits set up high, so strip them off */
    daddu   s1, 127             /* Round size up when converting to cache lines */
    srl     s1, 7               /* Divide by cache line size to get number of lines */

    /* Calculate the physical address taking into account index aliasing
        if (((cvmx_l2c_cfg_t)(cvmx_read_csr(CVMX_L2C_CFG))).s.idxalias)
        {
            int alias_shift = CVMX_L2C_IDX_ADDR_SHIFT + 2 * CVMX_L2_SET_BITS - 1;
            uint64_t addr_tmp = addr ^ (addr & ((1 << alias_shift) - 1)) >> CVMX_L2_SET_BITS;
            lckbase.s.lck_base = addr_tmp >> 7;
        }
        else
        {
            lckbase.s.lck_base = addr >> 7;
        }
        CVMX_L2C_IDX_ADDR_SHIFT is 7, based on cache line of 128 bytes
        CVMX_L2_SET_BITS is 9 (512 sets) for CN52XX
        alias shift is 7 + 2 * 9 -1 = 24
    */
lock_loop:
    dli     t1, 0x7FFFFFFF
    and     t1, s0              /* Get physical address of current line */
#ifdef USE_INDEX_ALIASING
    dli     t0, (1 << 24) - 1
    and     t0, s0              /* Mask for index XOR */
    srl     t0, 9               /* Shift masked address before XOR */
    xor     t0, s0
#endif
    srl     t1, 7 - 4           /*7 for cach line size, 3 for LCK_BASE reg layout (C code uses bitfield) */
    ori     t1, 1               /* Set enable bit */

    dli     t0, L2C_LCKBASE     /* Lock starting at computed address */
    sd      t1, 0(t0)
    ld      $0, 0(t0)           /* Make sure write completed */

    pref    30, 0(s0)           /* Prepare for store, making sure L2 doesn't read from dram */
    ld      $0, 0(s0)           /* Load the line, locking it into L2 */

    dsubu   s1, 1               /* Reduce the remaining lines by 1 */
    bnez    s1, lock_loop       /* Stop locking once the count reaches zero */
     daddu   s0, 128            /* Move onto the next cache line */

    /* Stop locking the cach */
    dli     t0, L2C_LCKBASE
    sd      $0, 0(t0)
#else
    dla     s0, __start         /* Address we start locking at */
    dla     s1, _end            /* Get the end of our code */
    dsubu   s1, s0              /* Subtract start from end to get our size in bytes */
    andi    s1, 0x7fff          /* _end gets extra bits set up high, so strip them off */
    daddu   s1, 127             /* Round size up when converting to cache lines */
    srl     s1, 7               /* Divide by cache line size to get number of lines */
lock_loop:
    pref    30, 0(s0)           /* Prepare for store, making sure L2 doesn't read from dram */
    cache   31, 0(s0)           /* Lock the line into L2 */
    dsubu   s1, 1               /* Reduce the remaining lines by 1 */
    bnez    s1, lock_loop       /* Stop locking once the count reaches zero */
     daddu   s0, 128            /* Move onto the next cache line */
#endif

#ifdef DEBUG
    # Tell the user how much L2 we locked
    dla_rel a0, MESSAGE_LOCKED_L2
    bal     uart_write_string
     nop
    dla     t0, __start
    bal     uart_write_hex
     dsubu  a0, s0, t0
    dla_rel a0, MESSAGE_LOCKED_L2_2
    bal     uart_write_string
     nop

    # Tell the user we are about to start copying code
    dla_rel a0, MESSAGE_COPY_CODE
    bal     uart_write_string
     nop
#endif

    # Copy our code to L2
    dla     s0, __start         /* Get the start of our code */
    dla     s1, _end            /* Get the end of our code */
    dsubu   s1, s0              /* Subtract start from end to get our size in bytes */
    andi    s1, 0x7fff          /* _end gets extra bits set up high, so strip them off */

#ifdef DEBUG
    dla_rel a0, MESSAGE_CODE_SIZE
    bal     uart_write_string
     nop
    bal     uart_write_hex
     move   a0, s1
    dla_rel a0, MESSAGE_CODE_SIZE_2
    bal     uart_write_string
     nop
#endif

    dla_rel t0, __start         /* This is our virtual location in flash */
copy_code_loop:
    ld      t3, 0(t0)           /* Load us from flash */
    daddu    t0, 8              /* Increment to next location in flash */
    sd      t3, 0(s0)           /* Store into L2 */
    daddu   s1, -8              /* Decrement our loop counter */
    bgez    s1, copy_code_loop  /* Loop until we're done */
     daddu   s0, 8              /* Increment the location in L2 */

#ifdef DEBUG
    # Tell the user we are about to jump into L2
    dla_rel a0, MESSAGE_JUMP_L2
    bal     uart_write_string
     nop
#endif

    # Jump to code into L2
    sync                        /* Make sure all writes have been flushed */
    synci   0($0)               /* Clear Icache in case it is out of sync with L2 */
    dla     t0, in_l2
    j       t0
     nop

    # We are now running in L2 and are no longer dependent on the NAND flash
    # to be error free.
in_l2:
#ifdef DEBUG
    dla     a0, MESSAGE_RUNNING_l2
    bal     uart_write_string
     nop
#endif

    # Setup stack in scratch so we can jump to C code
    dmfc0   t0, COP0_CVMMMECTL
    and     t0, -64
    or      t0, STACK_CACHE_LINES | (1<<8)
    dmtc0   t0, COP0_CVMMMECTL
    li      sp, -32768 + STACK_CACHE_LINES * 128

    # Jump to C code
    dla     t0, main
    jal     t0
     nop

    # Main should never return. If it does, just loop here
1:  b       1b
     nop
    # These are the messages we display before getting to the C code
#if OCTEON_MODEL == OCTEON_CN52XX
MESSAGE_HELLO:      .string "Octeon NAND Boot\r\n\0"
#else
MESSAGE_HELLO:      .string "Octeon2 NAND Boot\r\n\0"
#endif
MESSAGE_BANNER:     .string "\r\n****************************\r\n\0"
MESSAGE_R0:         .string "r0 = \0"
MESSAGE_R1:         .string "\r\nat = \0"
MESSAGE_R2:         .string "\r\nv0 = \0"
MESSAGE_R3:         .string "\r\nv1 = \0"
MESSAGE_R4:         .string "\r\na0 = \0"
MESSAGE_R5:         .string "\r\na1 = \0"
MESSAGE_R6:         .string "\r\na2 = \0"
MESSAGE_R7:         .string "\r\na3 = \0"
MESSAGE_R8:         .string "\r\na4 = \0"
MESSAGE_R9:         .string "\r\na5 = \0"
MESSAGE_R10:        .string "\r\na6 = \0"
MESSAGE_R11:        .string "\r\na7 = \0"
MESSAGE_R12:        .string "\r\nt0 = \0"
MESSAGE_R13:        .string "\r\nt1 = \0"
MESSAGE_R14:        .string "\r\nt2 = \0"
MESSAGE_R15:        .string "\r\nt3 = \0"
MESSAGE_R16:        .string "\t\ts0 = \0"
MESSAGE_R17:        .string "\t\ts1 = \0"
MESSAGE_R18:        .string "\t\ts2 = \0"
MESSAGE_R19:        .string "\t\ts3 = \0"
MESSAGE_R20:        .string "\t\ts4 = \0"
MESSAGE_R21:        .string "\t\ts5 = \0"
MESSAGE_R22:        .string "\t\ts6 = \0"
MESSAGE_R23:        .string "\t\ts7 = \0"
MESSAGE_R24:        .string "\t\tt8 = \0"
MESSAGE_R25:        .string "\t\tt9 = \0"
MESSAGE_R26:        .string "\t\tk0 = \0"
MESSAGE_R27:        .string "\t\tk1 = \0"
MESSAGE_R28:        .string "\t\tgp = \0"
MESSAGE_R29:        .string "\t\tsp = \0"
MESSAGE_R30:        .string "\t\ts8 = \0"
MESSAGE_R31:        .string "\t\tra = \0"
MESSAGE_COP0_CAUSE: .string "\r\nCOP0 CAUSE =\t\0"
MESSAGE_COP0_STATUS:.string "\r\nCOP0 STATUS =\t\0"
MESSAGE_COP0_BADVADDR:.string "\r\nCOP0 BADVADDR =\t\0"
MESSAGE_COP0_EPC:   .string "\r\nCOP0 EPC =\t\0"
MESSAGE_COP0_EBASE: .string "\r\nCOP0 EBASE =\t\0"
#ifdef DEBUG
MESSAGE_LOCKED_L2:  .string "Locked \0"
MESSAGE_LOCKED_L2_2:.string " bytes of L2\r\n\0"
MESSAGE_COPY_CODE:  .string "Copying code to L2\r\n\0"
MESSAGE_CODE_SIZE:  .string "Stage 1 is \0"
MESSAGE_CODE_SIZE_2: .string " bytes\r\n\0"
MESSAGE_JUMP_L2:    .string "Transfering from NAND to L2\r\n\0"
MESSAGE_RUNNING_l2: .string "Running from L2\r\n\0"
#endif
    .end __start

#
# void uart_write_char(char c)
#
    .align 4
    .ent uart_write_char
uart_write_char:
    dli     t0, UART_BASE(UART_PORT)
1:
    ld      t1, UART_LSR(t0)/* Read LSR so we can see if the FIFO has room */
    and     t1, 1<<5        /* Bit 5 signals that the TX FIFO has room */
    beqz    t1, 1b          /* Loop if there isn't any room */
     nop
    j ra                    /* We're done, just return to the caller */
     sd     a0, UART_THR(t0)/* write the char in the delay slot */
    .end uart_write_char

#
# void uart_write_string(const char *str)
#
    .ent uart_write_string
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
    .end uart_write_string

#
# void uart_write_hex(uint64_t number)
#
    .ent uart_write_hex
uart_write_hex:
    move    t9, a0          /* Save a0 since C code expects that */
    move    t8, ra          /* Save ra */
    bal     uart_write_char /* Output '0' */
     li     a0, '0'
    bal     uart_write_char /* Output 'x' */
     li     a0, 'x'
    li      t3, 15          /* We need 16 digits total */
    move    t2, t9
1:
    dext    a0, t2, 60, 4   /* Extract [63:60] for display in hex */
    addiu   a0, '0'         /* Add '0' to convert to '0'-'9' */
    slti    t0, a0, 0x3a    /* Flag if 0-9 instead of a-f */
    addiu   t1, a0, 0x41-0x30-10 /* Add 'a' */
    bal     uart_write_char /* Output char */
     movz   a0, t1, t0      /* Choose if 0-9 or a-f */
    dsll    t2, 4           /* Shift to the next digit */
    bnez    t3, 1b          /* Loop while we still have digits */
     addu    t3, -1         /* One less digit to do */
    move    ra, t8          /* Restore ra */
    j ra                    /* We're done, just return to the caller */
     move   a0, t9          /* Restore a0 */
    .end uart_write_hex

#
# void jump_to_bootloader(void *address)
#
    .ent jump_to_bootloader
jump_to_bootloader:
    sync                    /* Make sure all writes have been flushed */
    synci   0($0)           /* Clear Icache in case it is out of sync with L2 */
    j       a0              /* Jump to stage 2 */
     nop
    .end jump_to_bootloader

    .macro  display_reg reg
    dla_rel a0, MESSAGE_R\reg
    bal     uart_write_string
     nop
    bal     uart_write_hex
     ld     a0, -32768+\reg*8($0)
    .endm


#
# Low level default exception handler that just displays the registers
#
    .ent handle_exception
    .set push
    .set noat
handle_exception:
    sd $0, -32768+0*8($0)
    sd $1, -32768+1*8($0)
    sd $2, -32768+2*8($0)
    sd $3, -32768+3*8($0)
    sd $4, -32768+4*8($0)
    sd $5, -32768+5*8($0)
    sd $6, -32768+6*8($0)
    sd $7, -32768+7*8($0)
    sd $8, -32768+8*8($0)
    sd $9, -32768+9*8($0)
    sd $10, -32768+10*8($0)
    sd $11, -32768+11*8($0)
    sd $12, -32768+12*8($0)
    sd $13, -32768+13*8($0)
    sd $14, -32768+14*8($0)
    sd $15, -32768+15*8($0)
    sd $16, -32768+16*8($0)
    sd $17, -32768+17*8($0)
    sd $18, -32768+18*8($0)
    sd $19, -32768+19*8($0)
    sd $20, -32768+20*8($0)
    sd $21, -32768+21*8($0)
    sd $22, -32768+22*8($0)
    sd $23, -32768+23*8($0)
    sd $24, -32768+24*8($0)
    sd $25, -32768+25*8($0)
    sd $26, -32768+26*8($0)
    sd $27, -32768+27*8($0)
    sd $28, -32768+28*8($0)
    sd $29, -32768+29*8($0)
    sd $30, -32768+30*8($0)
    sd $31, -32768+31*8($0)

    dla_rel a0, MESSAGE_BANNER
    bal     uart_write_string
     nop

    display_reg 0
    display_reg 16
    display_reg 1
    display_reg 17
    display_reg 2
    display_reg 18
    display_reg 3
    display_reg 19
    display_reg 4
    display_reg 20
    display_reg 5
    display_reg 21
    display_reg 6
    display_reg 22
    display_reg 7
    display_reg 23
    display_reg 8
    display_reg 24
    display_reg 9
    display_reg 25
    display_reg 10
    display_reg 26
    display_reg 11
    display_reg 27
    display_reg 12
    display_reg 28
    display_reg 13
    display_reg 29
    display_reg 14
    display_reg 30
    display_reg 15
    display_reg 31

    dla_rel a0, MESSAGE_COP0_CAUSE
    bal     uart_write_string
     nop
    bal     uart_write_hex
     mfc0   a0, $13,0

    dla_rel a0, MESSAGE_COP0_STATUS
    bal     uart_write_string
     nop
    bal     uart_write_hex
     mfc0   a0, $12,0

    dla_rel a0, MESSAGE_COP0_BADVADDR
    bal     uart_write_string
     nop
    bal     uart_write_hex
     dmfc0  a0, $8,0

    dla_rel a0, MESSAGE_COP0_EPC
    bal     uart_write_string
     nop
    bal     uart_write_hex
     dmfc0  a0, $14,0

    dla_rel a0, MESSAGE_COP0_EBASE
    bal     uart_write_string
     nop
    bal     uart_write_hex
     mfc0   a0, $15,1

    dla_rel a0, MESSAGE_BANNER
    bal     uart_write_string
     nop

1:  b 1b
     nop
    .end handle_exception
    .set pop

    .set push
    .set noat
    .ent handle_exception_stub
handle_exception_stub:
    j handle_exception
    .end handle_exception_stub
    .set pop

