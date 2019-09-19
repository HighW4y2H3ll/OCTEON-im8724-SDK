#include <sys/asm.h>

	.abicalls
	.set noreorder

	.text
	.globl		switch_stack_and_call
	.hidden		switch_stack_and_call
	.ent		switch_stack_and_call
	.type		switch_stack_and_call, @function
switch_stack_and_call:
	PTR_ADDIU	$sp,$sp,-16
	REG_S		$s0,8($sp)
	REG_S		$ra,0($sp)

	move		$s0,$sp
	move		$sp,$a0
	move		$t9,$a1
	move		$a0,$a2
	jalr		$t9
	nop
	move		$sp,$s0

	REG_L		$ra,0($sp)
	REG_L		$s0,8($sp)
	PTR_ADDIU	$sp,$sp,16

	jr		$ra
	nop
END (switch_stack_and_call)	

	.align	2
	.globl	local_syscall
	.hidden local_syscall
	.type	local_syscall,@function
	.ent	local_syscall,0
local_syscall:
	.frame	$sp, SZREG, $ra
	.mask 0x00010000, -SZREG
	.fmask 0x00000000, 0
	PTR_ADDIU $sp, -SZREG
	REG_S $s0, ($sp)

	move $s0, $a0
	move $a0, $a1		/* shift arg1 - arg7.  */
	move $a1, $a2
	move $a2, $a3
	move $a3, $a4
	move $a4, $a5
	move $a5, $a6
	move $a6, $a7

	move $v0, $s0		/* Syscall number -> v0 */
	syscall			/* Do the system call.  */

	REG_L $s0, ($sp)
	bne $a3, $0, error
	nop

	PTR_ADDIU $sp, SZREG
	jr	$ra
	nop

error:
	SETUP_GP64 (0, local_syscall)

	PTR_LA $t9, local_syscall_error
	jr $t9

	RESTORE_GP64
	PTR_ADDIU $sp, SZREG
	jr	$ra

END (local_syscall)
