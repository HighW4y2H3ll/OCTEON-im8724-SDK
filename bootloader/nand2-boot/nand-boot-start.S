/*
 * Copyright 2013 Cavium, Inc. <support@cavium.com>
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
#include "spi-config.h"

#define STACK_CACHE_LINES	54	/* Number of cache lines to use in scratch for a stack */

/* COP0 registers used */
#define COP0_CVMCTL		$9,7
#define COP0_CVMMEMCTL		$11,7
#define COP0_STATUS		$12,0

	/* Global symbols for jumps */
	.globl __start
	.globl __octeon_init
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
	b	__init
	 nop
.end	__start

	.org	0x100
	.ent	__init
__init:
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
	jal	main		/* must never return */
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
	 cache	0, 0(zero)	/* Invalidate Icache */
move_and_start_end:
	.end	move_and_start
