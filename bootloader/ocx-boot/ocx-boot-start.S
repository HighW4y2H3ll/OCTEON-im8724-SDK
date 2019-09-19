/*
 * Copyright 2013 - 2017 Cavium, Inc. <support@cavium.com>
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
#include "ocx-config.h"

/** Number of cache lines to use in scratch for a stack */
#define STACK_CACHE_LINES	54

#define MIO_BASE		0x8001180000000000
#define MIO_RST_BOOT_OFF	(0x1600)
#define L2C_BASE		0x8001180080800000
#define L2C_OCI_CTL_OFF		(0x0020)
#define OCX_BASE		0x8001180011000000
#define OCX_COM_NODE_OFF	(0x0000)
#define OCX_COM_LINKX_CTL(X)	(0x0020 + ((X) * 8))


/** COP0 registers used */
#define COP0_CVMCTL		$9,7
#define COP0_CVMMEMCTL		$11,7
#define COP0_STATUS		$12,0
#define COP0_PRID		$15,0

#define PRID_78XX		0x95

	/* Global symbols for jumps */
	.globl __start
	.globl main
	.globl jump_to_bootloader
	.globl __end
	.globl move_and_start
	.globl move_and_start_end
	.globl __bss_start
	.globl _ebss

	.text
	.set	arch=octeon3
	.set	noreorder
.org	0
	.ent	__start
__start:
	bal	__init
	 nop
.end	__start

	/* Leave room for the bootloader header */
.org	0xc0
	.ent	__init
__init:
	daddiu	s0, ra, -8	/* Get our starting address */
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

	/* Reset GKSEGNODE to the correct value */
	dli	t0, OCX_BASE
	ld	t2, OCX_COM_NODE_OFF(t0)
	ext	t2, t2, 0, 2	/* Get OCX_COM_NODE.ID */
	dli	t0, L2C_BASE
	ld	t1, L2C_OCI_CTL_OFF(t0)
	dins	t1, t2, 4, 2		/* Set GKSEGNODE = ID */
	sd	t1, L2C_OCI_CTL_OFF(t0)
	/* See if we're running at the expected address, if not, copy the
	 * bootloader to the expected address.
	 */
	dla	a0, __start
	beq	a0, s0, no_move	/* Skip if our address is correct */
	 nop
	/* If the start address is not correct (i.e. executing out of parallel
	 * NOR flash) then we move the bootloader to the correct address and
	 * branch there.  Branching to the beginning shouldn't hurt since the
	 * above check will just skip this code.
	 */
	move	a1, s0		/* Source start address from above */
	lh	t0, 0x10(s0)	/* Load header length */
	lw	a2, 0x18(s0)	/* Load data length after header */
	addu	a2, t0, a2	/* Get total length */
	addiu	a2, 63		/* Round up */
	b	move_and_start
	 ins	a2, zero, 0, 6

no_move:

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

	 /* Clear BSS memory */
	dla	t0, __bss_start
	dla	t1, _ebss
clear_bss:
	sd	zero, 0(t0)
	sd	zero, 8(t0)
	bne	t0, t1, clear_bss
	 daddiu	t0, 16

	/* Jump to C code */
	jal	main
	 nop

1:
	wait
	b	1b
	 nop
	.end __init

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

	/**
	 * void move_and_start(uint64_t dest, uint64_t src_start, uint64_t size)
	 */
	.ent	move_and_start
move_and_start:
	move	t9, a0		/* Save start address */
	dadd	a2, a1, a2	/* Calculate end address */
	daddiu	a2, 39		/* Make sure we get the last word in and round up */
	dins	a2, zero, 0, 5	/* Make block boundary */
loop:
	ld	t0, 0(a1)
	ld	t1, 8(a1)
	ld	t2, 16(a1)
	ld	t3, 24(a1)
	daddiu	a1, a1, 32
	sd	t0, 0(a0)
	sd	t1, 8(a0)
	sd	t2, 16(a0)
	sd	t3, 24(a0)
	bne	a1, a2, loop
	 daddiu	a0, a0, 32

	j	t9
	 cache	0, 0(zero)
move_and_start_end:
	.end	move_and_start

	.word	_edata
	.word	__end
	.word	__bss_start
	.word	_ebss
