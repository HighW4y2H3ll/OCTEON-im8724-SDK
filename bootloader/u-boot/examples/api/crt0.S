/*
 * (C) Copyright 2007 Semihalf
 *
 * Written by: Rafal Jaworowski <raj@semihalf.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#if defined(CONFIG_PPC)

	.text
	.globl _start
_start:
	lis	%r11, search_hint@ha
	addi	%r11, %r11, search_hint@l
	stw	%r1, 0(%r11)
	b	main


	.globl syscall
syscall:
	lis	%r11, syscall_ptr@ha
	addi	%r11, %r11, syscall_ptr@l
	lwz	%r11, 0(%r11)
	mtctr	%r11
	bctr

#elif defined(CONFIG_ARM)

	.text
	.globl _start
_start:
	ldr	ip, =search_hint
	str	sp, [ip]
	b	main


	.globl syscall
syscall:
	ldr	ip, =syscall_ptr
	ldr	pc, [ip]
#elif defined(CONFIG_MIPS)
#include <asm/regdef.h>
#include <asm/mipsregs.h>
	.text
	.set	push
	.set	noreorder
	.globl	_start
	.globl	__got_start
	.globl	__got_end
	.globl	__data_start
	.globl	__data_end
	.globl	__sdata_start
	.globl	__sdata_end
	.globl	__bss_start
	.globl	__bss_end
	.globl	_end
	.globl	_gp

	.org	0
	.ent	_start
_start:
APP_BRANCH:	b	__begin
APP_NOP:	 nop
APP_SIGNATURE:		.string		"U-Boot-Native-Application"
APP_VERSION_MAJOR:	.byte		1
APP_VERSION_MINOR:	.byte		0
APP_START:		.word		_start
APP_TEXT_START:		.word		__text_start
APP_TEXT_END:		.word		__text_end
APP_GOT_START:		.word		__got_start
APP_GOT_END:		.word		__got_end
APP_RO_DATA_START:	.word		__ro_data_start
APP_RO_DATA_END:	.word		__ro_data_end
APP_DATA_START:		.word		__data_start
APP_DATA_END:		.word		__data_end
APP_SDATA_START:	.word		__sdata_start
APP_SDATA_END:		.word		__sdata_end
APP_BSS_START:		.word		__bss_start
APP_BSS_END:		.word		__bss_end
APP_END:		.word		_end
APP_LOADER_VERSION_MAJ:	.byte		0
APP_LOADER_VERSION_MIN:	.byte		0
APP_LOADER_FLAGS:	.short		0
APP_API_SIG_ADDR:	.word		0
	/* Leave plenty of room to grow */

	.org	0x200
__begin:
	/* Save current GP and RA on the stack */
	addiu	sp, sp, -16
	sd	gp, 0(sp)
	sd	ra, 8(sp)
#if __PIC__ > 0
	/* Now initialize the global pointer */
	/* Calculate GP */
	bal	1f		/* Get current PC */
	 nop

	/* This contains the linked address of the GOT */
	.word	_GLOBAL_OFFSET_TABLE_
	/* The ra register now contains the runtime address of the above
	 * memory location.
	 */
	.word	. - 4
1:
	/* Put current PC in GP register */
	move	gp, ra
	/* Load linked address of the GOT into t0 */
	lw	t0, 0(ra)
	/* Load the link time address of the GOT storage location into t1 */
	lw	t1, 4(ra)
	/* Find the offset */
	dsubu	t0, t1
	/* Add it to the GP */
	daddu	gp, t0
#endif
	/* We have the APP API signature in the header so copy it into
	 * search_hint.
	 */
	la	t0, APP_API_SIG_ADDR
	lw	t0, 0(t0)
	la	t1, search_hint
	bnez	t0, 2f
	 sw	t0, 0(t1)

	/* If it is somehow zero then we use the usual method of searching
	 * starting at the stack pointer.
	 */
	sw	sp, 0(t1)
2:
	/* Now branch and link to main */
	la	t9, main
	jal	t9
	 nop
	/* Restore original GP and RA and pop the stack */
	ld	gp, 0(sp)
	ld	ra, 8(sp)
	jr	ra
	 addiu	sp, sp, 16
	.end	_start

	.globl	syscall
	.ent	syscall
syscall:
	la	t0, syscall_ptr
	lw	t9, 0(t0)
	j	t9
	 nop
	.end	syscall

	/* Restore settings */
	.set	pop
#else
#error No support for this arch!
#endif

	.globl syscall_ptr
	.balign	4
syscall_ptr:	.long	0

	.globl search_hint
search_hint:	.long   0

_end:	.word	_end
