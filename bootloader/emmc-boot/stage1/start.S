/*
 * Copyright 2012 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <regdef.h>
#include "config.h"

#define STACK_CACHE_LINES	54	/* Number of cache lines to use in scratch for a stack */

#define	L2C_BASE		0x8001180080000000
#define L2C_DBG			0x8001180080000030
#define L2C_CFG			0x8001180080000000
#define L2T_ERR			0x8001180080000008
#define L2C_LCKOFF		0x8001180080000060
#define L2C_LCKBASE		0x8001180080000058
#define L2C_CTL			0x8001180080800000
#define L2C_BST_MEM0		0x8001180080C007F8

#ifdef GPIO_DEBUG	/* Not used yet */
# define GPIO_BASE		0x8001070000000000
# define GPIO_CFG_OFF		(0x0800 + (8 * GPIO_PIN))
# define GPIO_TX_SET_OFF	(0x888)
#endif

#define MIO_BASE		0x8001180000000000
#define MIO_RST_BOOT_OFF	(0x1600)
#define MIO_RST_BOOT		(MIO_BASE + MIO_RST_BOOT_OFF)
#define UART_BASE(port)		(MIO_BASE + 0x800 + (0x400 * (port)))
#define UART_LCR		0x18
#define UART_FCR		0x50
#define UART_DLL		0x80
#define UART_DLH		0x88
#define UART_LSR		0x28
#define UART_THR		0x40
#define UART_USR		0x138

/* COP0 registers used */
#define COP0_CVMCTL		$9,7
#define COP0_CVMMEMCTL		$11,7
#define COP0_STATUS		$12,0
#define COP0_PRID		$15,0
#define COP0_EBASE_REG		$15,1

#define PRID_78XX		0x95
#define PRID_70XX		0x96

	/* Global symbols for jumps */
	.globl __start
	.globl partition_table
	.globl main
	.globl uart_write_char
	.globl uart_write_string
	.globl uart_write_hex
	.globl jump_to_bootloader
	.globl partition1
	.globl partition2
	.globl partition3
	.globl partition4
	.globl move_and_start
	.globl move_and_start_end

	.text
	.set	arch=octeon2
	.set	noreorder
	.ent	__start
__start:
	/* Enable 64-bit addressing and disable BEV.  BEV causes exceptions to
	 * jump to flash addresses which don't exist.
	 */
	mfc0	t0, COP0_STATUS
	ori	t0, 1<<7	/* Enable 64-bit KSEG0 */
	li	t1, 1		/* Also used in UART code below! */
	ins	t0, t1, 30, 1	/* Enable COP2 */
	ins	t0, zero, 22, 1	/* Turn off exception vectors */
	mtc0	t0, COP0_STATUS

	/* Disable Icache prefetching so a stray prefetch can't access DRAM that
	 * isn't setup yet.
	 * Also enable unaligned load/stores
	 */
	dmfc0	t0, COP0_CVMCTL
	ori	t0, (1<<13|1<<14)
	dmtc0	t0, COP0_CVMCTL

	/* Setup UART to UART_BAUD_RATE,8,N,1 */
	dli	s0, UART_BASE(UART_PORT)
	li	t0, 0x83            /* Set 8 bits and allow divisor programming */
	sd	t0, UART_LCR(s0)
	sd	t1, UART_FCR(s0)    /* t1 initialized to 1 above */
	dli	t0, MIO_BASE    /* Get Octeon2's sclk multiplier from MIO_RST_BOOT */
	mfc0	t2, COP0_PRID
	ext	t2, t2, 8, 8
	sltiu	t2, t2, PRID_78XX
	bnez	t2, is_octeon2
	 li	t2, 6
	dins	t0, t2, 24, 4
is_octeon2:
	ld	t1, MIO_RST_BOOT_OFF(t0)
	dins	t1, zero, 60, 1     /* Disable ROM */
	dins	t1, zero, 2, 19     /* Mask lboot, lboot_ext, lboot_oci which
				       are R/W1C fields, to avoid from clearing */
	sd	t1, MIO_RST_BOOT_OFF(t0)
	ext	t1, t1, 24, 6       /* Multiplier is bits <29:24> */
	/* Multiplier*ref*1000000 = Core clock in Hz */
	dmul	t0, t1, REF_CLOCK / UART_BAUD_RATE
	ext	t1, t0, 12, 8        /* Divide by 16 to convert tu UART divisor */
	sd	t1, UART_DLH(s0)    /* Write high byte */
	ext	t1, t0, 4, 8        /* Write low byte */
	sd	t1, UART_DLL(s0)
	/* Enable the UART after the divisor is set */
	ld	t0, UART_LCR(s0)    /* Read LCR so we can update the divisor latch */
	and	t0, 0x7f            /* Force divisor latch to be zero */
	sd	t0, UART_LCR(s0)    /* Store the new LCR */
	ld	$0, UART_LCR(s0)    /* Read it back to make sure the update is complete */
	li	t0, 0xff            /* Spin for a while waiting for the uart to be ready */
1:	bnez	t0, 1b
	 addu	t0, -1
	/* Set up local stack memory */
	dmfc0	v0, COP0_CVMMEMCTL
	dins	v0, zero, 0, 9
	ori	v0, 0x100 | STACK_CACHE_LINES
	dmtc0	v0, COP0_CVMMEMCTL
	li	sp, 0xffff8000 + ((STACK_CACHE_LINES - 1) * 128)

	/* Clear scratch */
	li	t0, 0xffff8000
clear_scratch:
	sd	zero, 0(t0)
	bne	t0, sp, clear_scratch
	 addi	t0, 8

#ifdef STAGE1_START_STRING
	/* NOTE: STAGE1_START_STRING is hard-coded to use address 0x81000200 */
	/* The assembler does not reorder like it's supposed to with
	 * .set reorder
	 */
	lui	a0, 0x8100
	jal	uart_write_string
	 ori	a0, 0x0200
#endif

	/* Jump to C code */
	jal	main		/* main must never return */
	/* NOP not needed, dli is harmless */
	.end __start

/*
 * void uart_write_char(char c)
 */
	.ent uart_write_char
uart_write_char:
	dli	t0, UART_BASE(UART_PORT)
1:
	ld	t1, UART_LSR(t0)/* Read LSR so we can see if the FIFO has room */
	bbit0	t1, 5, 1b
	 nop
	j ra			/* We're done, just return to the caller */
	 sd	a0, UART_THR(t0)/* write the char in the delay slot */
	.end uart_write_char

/*
 * void uart_write_string(const char *str)
 */
	.ent uart_write_string
uart_write_string:
	dli	t0, UART_BASE(UART_PORT)
	move	t1, a0		/* Save a0 since C code expects that */
1:
	lbu	t2, 0(t1)	/* Read next byte of string */
	beqz	t2, 3f		/* Jump out if the byte is a zero */
	 daddiu	t1, 1		/* Increment out pointer to the next byte */
2:
	ld	t3, UART_LSR(t0)/* Read LSR so we can see if the FIFO has room */
	bbit0	t3, 5, 2b	/* Bit 5 signals that the TX FIFO has room */
	 nop
	b	1b		/* Jump to process the next byte and */
	 sd	t2, UART_THR(t0)/* write the current byte in the delay slot */
3:
	j ra			/* We're done, just return to the caller */
	 nop
	.end uart_write_string

/*
 * void uart_write_hex(uint64_t number)
 */
	.ent uart_write_hex
uart_write_hex:
	move	t9, a0		/* Save a0 since C code expects that */
	move	t8, ra		/* Save ra */
	bal	uart_write_char	/* Output '0' */
	 li	a0, '0'
	bal	uart_write_char	/* Output 'x' */
	 li	a0, 'x'
	li	t3, 15		/* We need 16 digits total */
	move	t2, t9
1:
	dext	a0, t2, 60, 4	/* Extract [63:60] for display in hex */
	addiu	a0, '0'		/* Add '0' to convert to '0'-'9' */
	slti	t0, a0, 0x3a	/* Flag if 0-9 instead of a-f */
	addiu	t1, a0, 0x41-0x30-10 /* Add 'a' */
	bal	uart_write_char /* Output char */
	 movz	a0, t1, t0	/* Choose if 0-9 or a-f */
	dsll	t2, 4		/* Shift to the next digit */
	bnez	t3, 1b		/* Loop while we still have digits */
	 addu	t3, -1		/* One less digit to do */
	move	ra, t8		/* Restore ra */
	j ra			/* We're done, just return to the caller */
	 move	a0, t9		/* Restore a0 */
	.end uart_write_hex

/*
 * void jump_to_bootloader(uint64_t address)
 */
	.ent	jump_to_bootloader
jump_to_bootloader:
	sync			/* Complete any writes */
	cache	1, 0(zero)	/* Invalidate Dcache */
	j	a0
	 cache	0, 0(zero)	/* Invalidate Icache in delay slot */
	.end jump_to_bootloader

/*
 *	NOTE: There are 16 bytes still free here!
 */
	.org	0x1b8
disk_signature:	.word	0

	.org	0x1be
/*
 * NOTE: partition 1 is a test partition
 * The format of each partition record is as follows:
 * Offset	Size	Description
 * 0		1	status (0x80 = bootable (active), 0x00 = non-bootable)
 * 1		3	CHS address of first absolute sector in partition
 * 			The format is:
 *				hhhhhhhhccsssssscccccccc
 *				765432109854321076543210
 *			Where:
 *				h = head, c = cylinder, s = sector
 *				The number underneath is the bit number
 *				So cylinder bit 9 is the MSB of the 2nd byte.
 * 4		1	Partition type (0x0c = FAT32 LBA, 0x83 = Linux, 0x82=swap)
 * 5		3	CHS address of the last absolute sector in the partition
 *			This is the same format as the first sector
 * 8		4	Logical block address of the first absolute sector
 * 12		4	Number of sectors in the partition
 *
 * See http://en.wikipedia.org/wiki/Master_boot_record for a detailed
 * explaination of the master boot record.
 *
 * NOTE: LBA and sector counts are in little-endian format!
 */
partition_table:
partition1:	.byte	0x00			/* Status */
		.byte	0x00, 0x00, 0x00	/* CHS of first absolute sector */
		.byte	0x00			/* Partition type */
		.byte	0x00, 0x00, 0x00	/* CHS of last absolute sector */
		.byte	0x00, 0x00, 0x00, 0x00  /* LBA of first absolute sector */
		.byte	0x00, 0x00, 0x00, 0x00	/* Number of sectors in partition */

partition2:	.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00

partition3:	.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00

partition4:	.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00
		.byte	0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00
		.byte	0x00, 0x00, 0x00, 0x00
/* The MBR signature is the last two bytes of the MBR sector.  It must contain
 * 0x55 0xAA
 */
	.org	0x1fe
mbr_signature:	.byte	0x55, 0xaa

#ifdef STAGE1_START_STRING
	.org	0x200
/* NOTE: do not move, hard-coded above. */
stage1_start_string:	.string 	STAGE1_START_STRING
#endif

	/**
	 * void move_and_start(uint64_t dest, uint64_t src_start, uint64_t size)
	 */
.ent	move_and_start
move_and_start:
	move	t9, a0		/* Save start address */
	/* If the destination and start addresses are the same we can skip
	 * the move operation.
	 */
	beq	a0, a1, mas_do_start
	 dadd	a2, a1, a2	/* Calculate end address */
	daddiu	a2, 39		/* Make sure we get the last word in and round up */
	dins	a2, zero, 0, 5
loop:
	ld	t0, 0(a1)
	daddiu	a1, a1, 8
	sd	t0, 0(a0)
	bne	a1, a2, loop
	 daddiu	a0, a0, 8
mas_do_start:
	j	t9
	 cache	0, 0(zero)
move_and_start_end:
.end	move_and_start
