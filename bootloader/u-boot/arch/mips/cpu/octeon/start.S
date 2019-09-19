/*
 *  Startup Code for OCTEON 64-bit CPU-core
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
 *  Copyright 2004, 2005, 2010 - 2015 Cavium Inc..
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/arch/cvmx-bootloader.h>
#include <asm/arch/octeon_mem_map.h>
#include <asm/arch/octeon-boot-info.h>
#include <asm/arch/cvmx-boot-vector.h>

/* COP0 registers definitions */
#define COP0_INDEX_REG		$0,0
#define COP0_ENTRYLO0_REG	$2,0
#define COP0_ENTRYLO1_REG	$3,0
#define COP0_CONTEXT_REG	$4,0
#define COP0_USERLOCAL_REG	$4,2
#define COP0_PAGEMASK_REG	$5,0
#define COP0_PAGEGRAIN_REG	$5,1
#define COP0_WIRED_REG		$6,0
#define COP0_HWRENA_REG		$7,0
#define COP0_BADVADDR_REG	$8,0
#define COP0_COUNT_REG		$9,0
#define COP0_CVMCOUNT_REG	$9,6
#define COP0_CVMCTL_REG		$9,7
#define COP0_ENTRYHI_REG	$10,0
#define COP0_COMPARE_REG	$11,0
#define COP0_CVMMEMCTL_REG	$11,7
#define COP0_STATUS_REG		$12,0
#define COP0_CAUSE_REG		$13,0
#define COP0_EPC_REG		$14,0
#define COP0_PROC_ID_REG	$15,0
#define COP0_EBASE_REG		$15,1
#define COP0_CONFIG1_REG	$16,1
#define COP0_CONFIG3_REG	$16,3
#define COP0_CONFIG4_REG	$16,4
#define COP0_CVMMEMCTL2_REG	$16,6
#define COP0_XCONTEXT_REG	$20,0
#define COP0_MDEBUG_REG		$22,0
#define COP0_DEBUG_REG		$23,0
#define COP0_DEPC_REG		$24,0
#define COP0_PERF_CNT0_REG	$25,1
#define COP0_PERF_CNT1_REG	$25,3
#define COP0_PERF_CNT2_REG	$25,5
#define COP0_PERF_CNT3_REG	$25,7
#define COP0_CACHEERR_ICACHE	$27,0
#define COP0_CACHEERR_DCACHE	$27,1
#define COP0_ERROREPC_REG	$30,0
#define COP0_DESAVE_REG		$31,0
#define COP0_KSCRATCH1_REG	$31,2
#define COP0_KSCRATCH2_REG	$31,3
#define COP0_KSCRATCH3_REG	$31,4
#define COP0_KSCRATCH4_REG	$31,5

#define	EBASE_CORE_MASK		0x3FF

#define UART_PORT			CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT

#define UART_BASE(port)			0x8001180000000800+0x400*(port)
#define UART_LCR			0x18
#define UART_MCR			0x20
#define UART_LSR			0x28
#define UART_MSR			0x30
#define UART_SCR			0x38
#define UART_THR			0x40
#define UART_FCR			0x50
#define UART_DLL			0x80
#define UART_DLH			0x88
#define UART_USR			0x138

#define OCTEON_CIU_SOFT_RST		0x8001070000000740

#define	OCTEON_GPIO_RX_DAT		0x8001070000000880
#define	OCTEON_GPIO_TX_CLR		0x8001070000000890
#define	OCTEON_GPIO_TX_SET		0x8001070000000888
#define	OCTEON_GPIO_TX_CLEAR		0x8001070000000890

#define	OCTEON_L2C_WPAR_PP0		0x8001180080840000
#define OCTEON_MIO_BOOT_BASE		0x8001180000000000
#define OCTEON_MIO_BOOT_REG_CFG0_OFF	0x0000
#define OCTEON_MIO_BOOT_LOC_CFG0_OFF	0x0080
#define OCTEON_MIO_BOOT_LOC_ADR_OFF	0x0090
#define OCTEON_MIO_BOOT_LOC_DAT_OFF	0x0098
#define	OCTEON_MIO_RST_BOOT		0x8001180000001600
#define OCTEON_MIO_BOOT_REG_CFG0	0x8001180000000000
#define	OCTEON_MIO_BOOT_REG_TIM0	0x8001180000000040
#define OCTEON_MIO_BOOT_LOC_CFG0	0x8001180000000080
#define OCTEON_MIO_BOOT_LOC_ADR		0x8001180000000090
#define OCTEON_MIO_BOOT_LOC_DAT		0x8001180000000098
#define	OCTEON_MIO_FUSE_DAT3		0x8001180000001418
#define OCTEON_L2D_FUS3			0x80011800800007B8
#define	OCTEON_LMC0_DDR_PLL_CTL		0x8001180088000258

#define OCTEON_RST			0x8001180006000000
#define OCTEON_RST_BOOT_OFFSET		0x1600
#define OCTEON_RST_SOFT_RST_OFFSET	0x1680
#define OCTEON_RST_COLD_DATAX_OFFSET(X)	(0x17C0 + (X) * 8)
#define OCTEON_RST_BOOT			0x8001180006001600
#define OCTEON_RST_SOFT_RST		0x8001180006001680
#define OCTEON_RST_COLD_DATAX(X)	(0x80011800060017C0 + (X) * 8)

#define OCTEON_OCX_COM_NODE		0x8001180011000000
#define OCTEON_L2C_OCI_CTL		0x8001180080800020
#define OCTEON_L2C_TAD_CTL		0x8001180080800018
#define OCTEON_L2C_CTL			0x8001180080800000

#define OCTEON_DBG_DATA			0x80011F00000001E8
#define OCTEON_PCI_READ_CMD_E		0x80011F0000001188
#define OCTEON_NPEI_DBG_DATA		0x80011F0000008510
#define OCTEON_CIU_WDOG(X)		(0x8001070000000500 + (X) * 8)
#define OCTEON_CIU_PP_POKE(X)		(0x8001070000000580 + (X) * 8)
#define OCTEON_CIU3_WDOG(X)		(0x8001010000020000 + (X) * 8)
#define OCTEON_CIU3_PP_POKE(X)		(0x8001010000030000 + (X) * 8)
#define OCTEON_OCX_COM_LINKX_CTL(X)	(0x8001180011000020 + (X) * 8)
#define OCTEON_SLI_CTL_STATUS		0x80011F0000028570
#define OCTEON_GSERX_SCRATCH(X)		(0x8001180090000020 + (X) * 0x1000000)

/* Offset in cvmx_seapi_signature structure of context
 * NOTE: These must match the cvmx_seapi_signature data structure and
 * cvmx_seapi_context data structure in seapi_public.h.
 */

/** Offset of magic header in the signature */
#define SEAPI_SIG_MAGIC_OFFSET			0
/** Offset of the ver_maj field in the signature */
#define SEAPI_SIG_VER_MAJ_OFFSET		8
/** Offset of the ver_min field in the signature */
#define SEAPI_SIG_VER_MIN_OFFSET		10
/** Offset of the CRC in the signature */
#define SEAPI_SIG_CRC_OFFSET			12
/** Offset of the call table physical address in the signature */
#define SEAPI_SIG_CALL_TABLE_PADDR_OFFSET	16
/** Offset of the syscall pointer in the signature */
#define SEAPI_SYSCALL_OFFSET			24
/** Offset of the context data structure in the signature */
#define SEAPI_CONTEXT_OFFSET			32

/* The following offsets are from the beginning of the context section */
/** Offset of the registers within the SE API context data structure */
#define SEAPI_CONTEXT_REGS_OFFSET		0
/** Offset of the SE return address  within the SE API context data structure */
#define SEAPI_CONTEXT_SE_RETURN_ADDR_OFFSET	256
/** Offset of the old entrylo0 register */
#define SEAPI_CONTEXT_SE_C0_ENTRYLO0_OFFSET	264
/** Offset of the old entrylo1 register */
#define SEAPI_CONTEXT_SE_C0_ENTRYLO1_OFFSET	272
/** Offset of the old entryhi register */
#define SEAPI_CONTEXT_SE_C0_ENTRYHI_OFFSET	280
/** Offset of the pagemask register */
#define SEAPI_CONTEXT_SE_C0_PAGEMASK_OFFSET	288
/** Offset of the index register */
#define SEAPI_CONTEXT_SE_C0_INDEX_OFFSET	292
/** Offset of the status register */
#define SEAPI_CONTEXT_SE_C0_STATUS		296
/** Offset of the lock physical address */
#define SEAPI_CONTEXT_SE_LOCK_ADDR_OFFSET	304
/** U-Boot GD address */
#define SEAPI_CONTEXT_UBOOT_GD			312
/** U-Boot GP register */
#define SEAPI_CONTEXT_UBOOT_GP			320
/** U-Boot TLB entrylo0 */
#define SEAPI_CONTEXT_UBOOT_C0_ENTRYLO0		328
/** U-Boot TLB entrylo1 */
#define SEAPI_CONTEXT_UBOOT_C0_ENTRYLO1		336
/** U-Boot TLB entryhi */
#define SEAPI_CONTEXT_UBOOT_C0_ENTRYHI		344
/** U-Boot TLB pagemask register */
#define SEAPI_CONTEXT_UBOOT_C0_PAGEMASK		352
/** U-Boot stack address */
#define SEAPI_CONTEXT_UBOOT_STACK		356

/* Speed up flash.  Here we use a hardcoded value that will
 * work up to 1 GHz.  The speed will be adjusted again once
 * the actual speed of the CPU is determined, but this setting here
 * makes things much faster, as the hardware default at boot is
 * very slow.
 *
 * PAGEM[63]	 = 0x00
 * WAITM[62]	 = 0x00
 * PAGES[61:60]	 = 0x00
 * ALE[59:54]	 = 0x00 (0x04 if CONFIG_OCTEON_FLASH_USES_ALE is set)
 * PAGE[53:48]	 = 0x07
 * WAIT[47:42]	 = 0x3F
 * PAUSE[41:36]	 = 0x00
 * WR_HLD[35:30] = 0x09
 * RD_HLD[29:24] = 0x07
 * OE[17:12]	 = 0x0D
 * CE[11:6]	 = 0x0D
 * ADR[5:0]	 = 0x03
 */

/** Enable page mode */
#ifndef CONFIG_OCTEON_START_FLASH_PAGEM
# ifdef CONFIG_OCTEON_PAGE_MODE
#  define CONFIG_OCTEON_START_FLASH_PAGEM	1
# else
#  define CONFIG_OCTEON_START_FLASH_PAGEM	0
# endif
#endif

/** Enable wait mode (not used for flash) */
#ifndef CONFIG_OCTEON_START_FLASH_WAITM
# define CONFIG_OCTEON_START_FLASH_WAITM	0
#endif

/** Number of reads per page, 0=8 bytes, 1=2 bytes, 2=4bytes, 3=8bytes */
#ifndef CONFIG_OCTEON_START_FLASH_PAGES
# define CONFIG_OCTEON_START_FLASH_PAGES	0
#endif

/** Flash ALE cycle count, must be non-zero for ALE mode */
#ifndef CONFIG_OCTEON_START_FLASH_ALE
# define CONFIG_OCTEON_START_FLASH_ALE		4
#endif

/** Flash cycle count between page reads */
#ifndef CONFIG_OCTEON_START_FLASH_PAGE
# define CONFIG_OCTEON_START_FLASH_PAGE		7
#endif

/** Flash wait cycle count (not used) */
#ifndef CONFIG_OCTEON_START_FLASH_WAIT
# define CONFIG_OCTEON_START_FLASH_WAIT		0x3f
#endif

/** Flash pause cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_PAUSE
# define CONFIG_OCTEON_START_FLASH_PAUSE	0
#endif

/** Flash write hold cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_WR_HLD
# define CONFIG_OCTEON_START_FLASH_WR_HLD	9
#endif

/** Flash read hold cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_RD_HLD
# define CONFIG_OCTEON_START_FLASH_RD_HLD	7
#endif

/** Flash write enable cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_WE
# define CONFIG_OCTEON_START_FLASH_WE		8
#endif

/** Flash output enable cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_OE
# define CONFIG_OCTEON_START_FLASH_OE		0xd
#endif

/** Flash chip enable cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_CE
# define CONFIG_OCTEON_START_FLASH_CE		0xd
#endif

/** Flash address cycle count */
#ifndef CONFIG_OCTEON_START_FLASH_ADR
# define CONFIG_OCTEON_START_FLASH_ADR		0x3
#endif

#ifndef CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING
# define CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING		\
	(CONFIG_OCTEON_START_FLASH_PAGEM << 63)		|	\
	(CONFIG_OCTEON_START_FLASH_WAITM << 62)		|	\
	(CONFIG_OCTEON_START_FLASH_PAGES << 60)		|	\
	(CONFIG_OCTEON_START_FLASH_PAGE << 48)		|	\
	(CONFIG_OCTEON_START_FLASH_WAIT<< 42)		|	\
	(CONFIG_OCTEON_START_FLASH_PAUSE << 36)		|	\
	(CONFIG_OCTEON_START_FLASH_WR_HLD << 30)	|	\
	(CONFIG_OCTEON_START_FLASH_RD_HLD << 24)	|	\
	(CONFIG_OCTEON_START_FLASH_WE << 18)		|	\
	(CONFIG_OCTEON_START_FLASH_OE << 12)		|	\
	(CONFIG_OCTEON_START_FLASH_CE << 6)		|	\
	(CONFIG_OCTEON_START_FLASH_ADR)
#endif

#ifndef CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING_ALE
# define CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING_ALE		\
	(CONFIG_OCTEON_START_FLASH_PAGEM << 63)		|	\
	(CONFIG_OCTEON_START_FLASH_WAITM << 62)		|	\
	(CONFIG_OCTEON_START_FLASH_PAGES << 60)		|	\
	(CONFIG_OCTEON_START_FLASH_ALE << 54)		|	\
	(CONFIG_OCTEON_START_FLASH_PAGE << 48)		|	\
	(CONFIG_OCTEON_START_FLASH_WAIT<< 42)		|	\
	(CONFIG_OCTEON_START_FLASH_PAUSE << 36)		|	\
	(CONFIG_OCTEON_START_FLASH_WR_HLD << 30)	|	\
	(CONFIG_OCTEON_START_FLASH_RD_HLD << 24)	|	\
	(CONFIG_OCTEON_START_FLASH_OE << 12)		|	\
	(CONFIG_OCTEON_START_FLASH_CE << 6)		|	\
	(CONFIG_OCTEON_START_FLASH_ADR)
#endif

/**
 * Remote node ID in a multi-node setup.  If there are more than two nodes
 * we only care about any other node other than node 0 for 78xx reset purposes.
 */
#define OCTEON_78XX_REMOTE_NODE_ID	1

/** PRID for CN56XX */
#define OCTEON_PRID_CN56XX		0x04
/** PRID for CN52XX */
#define OCTEON_PRID_CN52XX		0x07
/** PRID for CN63XX */
#define OCTEON_PRID_CN63XX		0x90
/** PRID for CN68XX */
#define OCTEON_PRID_CN68XX		0x91
/** PRID for CN66XX */
#define OCTEON_PRID_CN66XX		0x92
/** PRID for CN61XX */
#define OCTEON_PRID_CN61XX		0x93
/** PRID for CNF71XX */
#define OCTEON_PRID_CNF71XX		0x94
/** PRID for CN78XX */
#define OCTEON_PRID_CN78XX		0x95
/** PRID for CN70XX */
#define OCTEON_PRID_CN70XX		0x96
/** PRID for CN73XX */
#define OCTEON_PRID_CN73XX		0x97
/** PRID for CNF75XX */
#define OCTEON_PRID_CNF75XX		0x98

#define RVECENT(f,n)	\
	b	f; nop
#define XVECENT(f,bev)	\
	b	f	;	\
	li	k0, bev

/* func argument is used to create a  mark, must be unique */
#define GETOFFSET(reg, func)	\
	bal	func ##_mark;	\
	nop;			\
	.word	.;		\
func ##_mark:			\
	lw	reg, 0(ra);	\
	dsubu	reg, ra, reg

#define JAL(func)		\
	bal	func ##_mark;	\
	 nop;			\
	.word .;		\
func ##_mark:			\
	lw	t8, 0(ra);	\
	dsubu	t8, ra, t8;	\
	la	t9, func;	\
	daddu	t9, t9, t8;	\
	jalr	t9;		\
	nop

	.set	arch=octeon2
	.set	noreorder

/* 7-segment display */
#if CONFIG_OCTEON_SIM_HW_DIFF || !defined(CONFIG_OCTEON_ENABLE_LED_DISPLAY)
.macro	_led_write_chars_4 r0,r1,d0,d1,d2,d3
	nop
.endm
.macro	_led_write_chars_8 r0,r1,d0,d1,d2,d3,d4,d5,d6,d7
	nop
.endm
.macro	_led_write_one_char r0,r1,off
	nop
.endm
#else
#if CONFIG_OCTEON_KODAMA
# define LED_REG_VAL	0x80001a00
# define LED_BASE_ADDR	0xba000000
#else
#define LED_REG_VAL	0x80001d02
#define LED_BASE_ADDR	0xbd020000
#endif
.macro	_led_write_one_char r0,r1,off
	sb	\r1, \off(\r0)
.endm
.macro	_led_write_chars_top4 r0,r1,d0,d1,d2,d3
	/* Map LED display */
	dli	\r0, 0x8001180000000020
	dli	\r1, LED_REG_VAL
	sd	\r1, 0(\r0);
	/* Write string */
	li	\r0, LED_BASE_ADDR	/* Base address of LED */
	li	\r1, \d0
	_led_write_one_char \r0,\r1,0xf8
	li	\r1, \d1
	_led_write_one_char \r0,\r1,0xf9
	li	\r1, \d2
	_led_write_one_char \r0,\r1,0xfa
	li	\r1, \d3
	_led_write_one_char \r0,\r1,0xfb
.endm
.macro	_led_write_chars_bot4 r0,r1,d0,d1,d2,d3
	/* Map LED display */
	dli	\r0, 0x8001180000000020
	dli	\r1, LED_REG_VAL
	sd	\r1, 0(\r0);
	/* Write string */
	li	\r0, LED_BASE_ADDR	/* Base address of LED */
	li	\r1, \d0
	_led_write_one_char \r0,\r1,0xfc
	li	\r1, \d1
	_led_write_one_char \r0,\r1,0xfd
	li	\r1, \d2
	_led_write_one_char \r0,\r1,0xfe
	li	\r1, \d3
	_led_write_one_char \r0,\r1,0xff
.endm
.macro	_led_write_chars_8 r0,r1,d0,d1,d2,d3,d4,d5,d6,d7
	/* Map LED display */
	dli	\r0, 0x8001180000000020
	dli	\r1, LED_REG_VAL
	sd	\r1, 0(\r0);
	/* Write string */
	li	\r0, LED_BASE_ADDR   /* Base address of LED */
	li	\r1, \d0
	_led_write_one_char \r0,\r1,0xf8
	li	\r1, \d1
	_led_write_one_char \r0,\r1,0xf9
	li	\r1, \d2
	_led_write_one_char \r0,\r1,0xfa
	li	\r1, \d3
	_led_write_one_char \r0,\r1,0xfb
	li	\r1, \d4
	_led_write_one_char \r0,\r1,0xfc
	li	\r1, \d5
	_led_write_one_char \r0,\r1,0xfd
	li	\r1, \d6
	_led_write_one_char \r0,\r1,0xfe
	li	\r1, \d7
	_led_write_one_char \r0,\r1,0xff
.endm
.macro	_dump_reg32 r0,r1,reg
	bal	1f
	 nop
	.byte	'0'
	.byte	'1'
	.byte	'2'
	.byte	'3'
	.byte	'4'
	.byte	'5'
	.byte	'6'
	.byte	'7'
	.byte	'8'
	.byte	'9'
	.byte	'A'
	.byte	'B'
	.byte	'C'
	.byte	'D'
	.byte	'E'
	.byte	'F'
1:
	/* Map LED display */
	dli	\r0, 0x8001180000000020
	dli	\r1, LED_REG_VAL
	sd	\r1, 0(\r0);
	li	\r0, LED_BASE_ADDR   /* Base address of LED */
	/* Write string */
	srl	\r1, \reg, 28
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xf8(\r0)
	srl	\r1, \reg, 24
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xf9(\r0)
	srl	\r1, \reg, 20
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xfa(\r0)
	srl	\r1, \reg, 16
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xfb(\r0)
	srl	\r1, \reg, 12
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xfc(\r0)
	srl	\r1, \reg, 8
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xfd(\r0)
	srl	\r1, \reg, 4
	andi	\r1, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xfe(\r0)
	andi	\r1, \reg, 0xf
	addu	\r1, ra
	lb	\r1, (\r1)
	sb	\r1, 0xff(\r0)
.endm
#endif /* CONFIG_OCTEON_SIM_HW_DIFF */

.macro	_get_gpio r0
	dli	\r0, OCTEON_GPIO_RX_DAT
	ld	\r0, (\r0)
	nop
.endm

#if __PIC__ > 0
.macro	__acquire_gp r0,r1
	/* Branch and link to get current PC in ra */
	bal	1f
	 nop
	.word	_GLOBAL_OFFSET_TABLE_	/* This contains the linked address of the
					 * GOT
					 */
	.word	. - 4			/* This contains the link time address of
					 * the prev. word, which is also what the
					 * link time expected PC value is
					 */
1:	move	gp, ra			/* Move current PC into gp register */
	lw	\r0, 0(ra)		/* Load linked address of the GOT into r0 */
	/* Load the link time address of the GOT storage location into r1 */
	lw	\r1, 4(ra)
	dsubu	\r0, \r1		/* Subtract r1 from r0. */
				/* r0 = offset link-time GOT address and the link
				 * time expected PC
				 */
				 /* Add this offset to the current PC (copied into
				 * gp above), so that gp now has the current runtime
				 * GOT address
				 */
	daddu	gp, \r0		/* calculate current location of offset table */
.endm
#endif

/**
 * Print out a character out the serial port
 *
 * r0, r1 are registers used by the macro
 * val is the value to output
 */
.macro	dbgchar	r0,r1,val
#ifdef DEBUG
	dli	\r0, UART_BASE(UART_PORT)
	/* Wait for space to transmit a character */
1:
	ld	\r1, UART_LSR(\r0)
	bbit0	\r1, 5, 1b
	 nop
	/* Send it */
	dli	\r1, \val
	sd	\r1, UART_THR(\r0)
#endif
.endm

.macro	printchar	r0,r1,val
	dli	\r0, UART_BASE(UART_PORT)
	/* wait for space to transmit a character */
1:
	ld	\r1, UART_LSR(\r0)
	bbit0	\r1, 5, 1b
	 nop
	/* send it */
	li	\r1, \val
	sd	\r1, UART_THR(\r0)
.endm

/**
 * Print out a character out the serial port
 *
 * r0, r1 are registers used by the macro
 * val is the value to output
 */
.macro	dbgreg	r0,r1,r2
#ifdef DEBUG
	dli	\r0, UART_BASE(UART_PORT)
	/* Wait for space to transmit a character */
	1:
	ld	\r1, UART_LSR(\r0)
	bbit0	\r1, 5, 1b
	nop
	/* Send it */
	sd	\r1, UART_THR(\r2)
#endif
.endm


.macro	dbghex	r0, r1, r2, r3, r4
#ifdef DEBUG
	dbgchar	\r0, \r1, '0'
	dbgchar \r0, \r1, 'x'
	li	\r3, 15
1:
	dext	\r2, \r4, 60, 4
	daddiu	\r2, '0'
	slti	\r0, \r2, 0x3a
	bnez	\r0, 2f
	 nop
	daddiu	\r2, 0x41 - 0x30 - 10	/* Add 'a' */
2:
	dbgreg	\r0, \r1, \r2
	dsll	\r4, 4
	bnez	\r3, 1b
	 daddiu	\r3, -1
#endif
.endm

/* Saved register usage:
 * s0:	not used
 * s1:	not used
 * s2:	Address U-Boot loaded into in L2 cache
 * s3:	Start address
 * s4:	flags
 *		1:	booting from RAM
 *		2:	executing out of cache
 *		4:	booting from flash
 * s5:	u-boot size (data end - _start)
 * s6:	offset in flash.
 * s7:	_start physical address
 * s8:
 */

	.set noreorder	/* We don't want the assembler to reorder instructions */

#undef mips64

	.extern _GLOBAL_OFFSET_TABLE_

	.globl _start
	.text
_start:
	RVECENT(reset,0)	/* U-boot entry point */
	/* The above jump instruction/nop are considered part of the
	 * bootloader_header_t structure but are not changed when the header is
	 * updated.
	 */

	/* Leave room for bootloader_header_t header at start of binary.  This
	 * header is used to identify the board the bootloader is for, what
	 * address it is linked at, failsafe/normal, etc.  It also contains a
	 * CRC of the entire image.
	 */


	.org 0x200

	XVECENT(romExcHandle,0x200)	/* bfc00200: R4000 tlbmiss vector */
	RVECENT(romReserved,65)
	RVECENT(romReserved,66)
	RVECENT(romReserved,67)
	RVECENT(romReserved,68)
	RVECENT(romReserved,69)
	RVECENT(romReserved,70)
	RVECENT(romReserved,71)
	RVECENT(romReserved,72)
	RVECENT(romReserved,73)
	RVECENT(romReserved,74)
	RVECENT(romReserved,75)
	RVECENT(romReserved,76)
	RVECENT(romReserved,77)
	RVECENT(romReserved,78)
	RVECENT(romReserved,79)
	XVECENT(romExcHandle,0x280)	/* bfc00280: R4000 xtlbmiss vector */
	RVECENT(romReserved,81)
	RVECENT(romReserved,82)
	RVECENT(romReserved,83)
	RVECENT(romReserved,84)
	RVECENT(romReserved,85)
	RVECENT(romReserved,86)
	RVECENT(romReserved,87)
	RVECENT(romReserved,88)
	RVECENT(romReserved,89)
	RVECENT(romReserved,90)
	RVECENT(romReserved,91)
	RVECENT(romReserved,92)
	RVECENT(romReserved,93)
	RVECENT(romReserved,94)
	RVECENT(romReserved,95)
	XVECENT(romExcHandle,0x300)	/* bfc00300: R4000 cache vector */
	RVECENT(romReserved,97)
	RVECENT(romReserved,98)
	RVECENT(romReserved,99)
	RVECENT(romReserved,100)
	RVECENT(romReserved,101)
	RVECENT(romReserved,102)
	RVECENT(romReserved,103)
	RVECENT(romReserved,104)
	RVECENT(romReserved,105)
	RVECENT(romReserved,106)
	RVECENT(romReserved,107)
	RVECENT(romReserved,108)
	RVECENT(romReserved,109)
	RVECENT(romReserved,110)
	RVECENT(romReserved,111)
	XVECENT(romExcHandle,0x380)	/* bfc00380: R4000 general vector */
	RVECENT(romReserved,113)
	RVECENT(romReserved,114)
	RVECENT(romReserved,115)
	RVECENT(romReserved,116)
	RVECENT(romReserved,116)
	RVECENT(romReserved,118)
	RVECENT(romReserved,119)
	RVECENT(romReserved,120)
	RVECENT(romReserved,121)
	RVECENT(romReserved,122)
	RVECENT(romReserved,123)
	RVECENT(romReserved,124)
	RVECENT(romReserved,125)
	RVECENT(romReserved,126)
	RVECENT(romReserved,127)
	XVECENT(romExcHandle,0x400)	/* bfc00400: */
	RVECENT(romReserved,129)
	RVECENT(romReserved,130)
	RVECENT(romReserved,131)
	RVECENT(romReserved,132)
	RVECENT(romReserved,133)
	RVECENT(romReserved,134)
	RVECENT(romReserved,135)
	RVECENT(romReserved,136)
	RVECENT(romReserved,137)
	RVECENT(romReserved,138)
	RVECENT(romReserved,139)
	RVECENT(romReserved,140)
	RVECENT(romReserved,141)
	RVECENT(romReserved,142)
	RVECENT(romReserved,143)
	RVECENT(debugHandler,0x480)	/* bfc00480:  Debug vector*/
	RVECENT(romReserved,145)
	RVECENT(romReserved,146)
	RVECENT(romReserved,147)
	RVECENT(romReserved,148)
	RVECENT(romReserved,149)
	RVECENT(romReserved,150)
	RVECENT(romReserved,151)
	RVECENT(romReserved,152)
	RVECENT(romReserved,153)
	RVECENT(romReserved,154)
	RVECENT(romReserved,155)
	RVECENT(romReserved,156)
	RVECENT(romReserved,157)
	RVECENT(romReserved,158)
	RVECENT(romReserved,159)

#ifdef CONFIG_OCTEON_AUTHENTIK_STAGE2
	.org	0x2000
	RVECENT(reset,0)
#endif
#if CONFIG_OCTEON_BIG_STACK_SIZE
	.balign	16
big_stack_end:
	.skip	CONFIG_OCTEON_BIG_STACK_SIZE, 0
big_stack_start:
	.dword	0
#endif
	/* Reserve extra space so that when we use the boot bus local memory
	 * segment to remap the debug exception vector we don't overwrite
	 * anything useful
	 */

	.balign	8
	.globl	asm_reset
asm_reset:
reset:
	nop
	synci	0(zero)
	mfc0	k0, COP0_STATUS_REG
	ori	k0, 0x00E0		/* enable 64 bit mode for CSR access */
	mtc0	k0, COP0_STATUS_REG

	/* Save the address we're booting from, strip off low bits */
	bal	1f
	 nop
1:
	move	s3, ra
	dins	s3, zero, 0, 12

	/* Disable boot bus moveable regions */
	dli	k0, OCTEON_MIO_BOOT_LOC_CFG0
	sd	zero, 0(k0)
	sd	zero, 8(k0)

#if !CONFIG_OCTEON_SIM_HW_DIFF
	/* Disable the watchdog timer
	 * First we check if we're running on CN78XX, CN73XX or CNF75XX to see
	 * if we use CIU3 or CIU.
	 */
	mfc0	t0, COP0_PROC_ID_REG
	ext	t0, t0, 8, 8
	/* Assume CIU */
	dli	t1, OCTEON_CIU_WDOG(0)
	dli	t2, OCTEON_CIU_PP_POKE(0)
	blt	t0, OCTEON_PRID_CN78XX, wd_use_ciu
	 nop
	beq	t0, OCTEON_PRID_CN70XX, wd_use_ciu
	 nop
	/* Use CIU3 */
	dli	t1, OCTEON_CIU3_WDOG(0)
	dli	t2, OCTEON_CIU3_PP_POKE(0)
wd_use_ciu:
	sd	zero, 0(t2)		/* Pet the dog */
	sd	zero, 0(t1)		/* Disable watchdog timer */
#endif

	/* Errata: CN76XX has a node ID of 3. change it to zero here.
	 * This needs to be done before we relocate to L2 as addresses change
	 * For 76XX pass 1.X we need to zero out the OCX_COM_NODE[ID],
	 * L2C_OCI_CTL[GKSEGNODE] and CP0 of Root.CvmMemCtl2[KSEGNODE].
	 */
	mfc0	a4, COP0_PROC_ID_REG
	/* Check for 78xx pass 1.x processor ID */
	andi	a4, 0xffff
	blt	a4, (OCTEON_PRID_CN78XX << 8), 1f
	 nop

	/* Zero out alternate package for now */
	dins	a4, zero, 6, 1
	bge	a4, ((OCTEON_PRID_CN78XX << 8) | 0x08), 1f
	 nop

	/* 78xx or 76xx here, first check for bug #27141 */
	dli	a5, OCTEON_SLI_CTL_STATUS
	ld	a6, 0(a5)
	andi	a7, a4, 0xff
	andi	a6, a6, 0xff

	beq	a6, a7, not_bug27141
	 nop

	/* core 0 proc_id rev_id field does not match SLI_CTL_STATUS rev_id field */
	/* We just hit bug #27141.  Need to reset the chip and try again */

	dli	a4, OCTEON_RST_SOFT_RST
	ori	a5, zero, 0x1	/* set the reset bit */

reset_78xx_27141:
	sync
	synci	0(zero)
	cache	9, 0(zero)
	sd	a5, 0(a4)
	wait
	b	reset_78xx_27141
	 nop

not_bug27141:
	/* 76XX pass 1.x has the node number set to 3 */
	mfc0	a4, COP0_EBASE_REG
	ext	a4, a4, 0, 10
	bne	a4, 0x180, 1f	/* Branch if not node 3 core 0 */
	 nop

	/* Clear OCX_COM_NODE[ID] */
	dli	a5, OCTEON_OCX_COM_NODE
	ld	a4, 0(a5)
	dins	a4, zero, 0, 2
	sd	a4, 0(a5)
	ld	zero, 0(a5)

	/* Clear L2C_OCI_CTL[GKSEGNODE] */
	dli	a5, OCTEON_L2C_OCI_CTL
	ld	a4, 0(a5)
	dins	a4, zero, 4, 2
	sd	a4, 0(a5)
	ld	zero, 0(a5)

	/* Clear CP0 Root.CvmMemCtl2[KSEGNODE] */
	dmfc0	a4, COP0_CVMMEMCTL2_REG
	dins	a4, zero, 12, 2
	dmtc0	a4, COP0_CVMMEMCTL2_REG
1:
#if defined(CONFIG_SYS_NO_FLASH) && !defined (CONFIG_OCTEON_SIM_HW_DIFF) && \
    !defined(CONFIG_OCTEON_SIM_SPEED)
	dli	a6, OCTEON_MIO_BOOT_REG_CFG0
	sd	zero, 0(a6)
#else
	/* Speed up flash.  Here we use a hardcoded value that will
	 * work up to 1 GHz.  The speed will be adjusted again once
	 * the actual speed of the CPU is determined, but this setting here
	 * makes things much faster, as the hardware default at boot is
	 * very slow.
	 *
	 * PAGEM[63]	 = 0x00
	 * WAITM[62]	 = 0x00
	 * PAGES[61:60]	 = 0x00
	 * ALE[59:54]	 = 0x00 (0x04 if CONFIG_OCTEON_FLASH_USES_ALE is set)
	 * PAGE[53:48]	 = 0x07
	 * WAIT[47:42]	 = 0x3F
	 * PAUSE[41:36]	 = 0x00
	 * WR_HLD[35:30] = 0x09
	 * RD_HLD[29:24] = 0x07
	 * OE[17:12]	 = 0x0D
	 * CE[11:6]	 = 0x0D
	 * ADR[5:0]	 = 0x03
	 */
	dli	a6, OCTEON_MIO_BOOT_REG_CFG0
#ifdef CONFIG_OCTEON_START_FLASH_CFG
	dli	a5, CONFIG_OCTEON_START_FLASH_CFG
	sd	a5, 0(a6)
#endif
	ld	a6, 0(a6)
	bbit0	a6, 31, no_flash	/* Skip if flash not enabled */
	 nop
	bbit1	a6, 29, ale_mode
	 nop
	dli	a4, (CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING)
	b	flash_speedup
	 nop
ale_mode:
	dli	a4, (CONFIG_OCTEON_START_FLASH_SPEEDUP_TIMING_ALE)

flash_speedup:
	dli	a5, OCTEON_MIO_BOOT_REG_TIM0	/* MIO_BOOT_REG_TIM0 */
	sd	a4, 0(a5)
#endif /* CONFIG_SYS_NO_FLASH */
	/* Put the flash address in the start of the EBASE register to
	 * enable our exception handler but only for core 0.
	 */
	mfc0	a4, COP0_EBASE_REG
	ext	a4, a4, 0, 10
	bnez	a4, no_flash
	/* OK in delay slot */
	ext	a6, a6, 0, 16		/* Get the base address in flash */
	sll	a6, a6, 16
	mtc0	a6, COP0_EBASE_REG	/* Enable exceptions */

#ifdef DEBUG
	/* Program the serial port UART early */
	dli	t2, UART_BASE(UART_PORT)
	dli	t3, 0x7
	sd	t3, UART_FCR(t2)	/* Enable FIFOs */

	dli	t3, 0x93	/* Enable divisor latch, 8,N,1 */
	sd	t3, UART_LCR(t2)

	/* Get the I/O clock in order to calculate the baud rate
	 * Note that different Octeon processors require different methods to
	 * get the I/O clock rate.
	 */
	mfc0	t0, COP0_PROC_ID_REG
	ext	t0, t0, 8, 8
	slti	t1, t0, OCTEON_PRID_CN78XX
	bnez	t1, 1f
	 nop
	/* Get IO clock for Octeon 3 */
	dli	t1, OCTEON_RST_BOOT
	ld	t0, 0(t1)
	ext	t0, t0, 24, 5	/* Get PNR_MUL value */
	li	t1, 50000000	/* Reference clock, 50MHz */
	b 	have_ioclock
	 dmul	t0, t0, t1	/* Get the I/O clock speed */

1:	/* Not octeon 3 */
	beq	t0, OCTEON_PRID_CN56XX, 3f
	 nop
	beq	t0, OCTEON_PRID_CN56XX, 3f
	 nop
	slti	t1, t0, OCTEON_PRID_CN63XX
	bnez	t1, 4f
	 nop
	/* Must be Octeon 2 */
	dli	t1, OCTEON_MIO_RST_BOOT
	ld	t0, 0(t1)
	ext	t0, t0, 24, 5	/* Get PNR_MUL value */
	li	t1, 50000000	/* Reference clock, 50MHz */
	b	have_ioclock
	 dmul	t0, t0, t1

3:	/* Must be CN56XX or CN52XX */
	dli	t1, OCTEON_NPEI_DBG_DATA
	ld	t0, 0(t1)
	ext	t0, t0, 18, 5	/* PLL_MUL pins */
	dli	t1, 50000000	/* Reference clock, 50MHz */
	b	have_ioclock
	 dmul	t0, t0, t1

4:	/* Must be CN3XXX, CN58XX or CN50XX */
	dli	t1, OCTEON_DBG_DATA
	ld	t0, 0(t1)
	ext	t0, t0, 18, 5	/* PLL_MUL pins */
	dli	t1, 50000000	/* Reference clock */
	/* Note that only Octeon+ and later have the dmul instruction */
	dmult	t0, t1
	mflo	t0

have_ioclock:
#if (CONFIG_BAUDRATE <= 0)
# error CONFIG_BAUDRATE must contain a valid baudrate!
#endif
	dsrl	t0, 4		/* Divide by 16 */
	dli	t1, CONFIG_BAUDRATE
	divu	t0, t1
	mflo	t1		/* T1 contains the baudrate divisor */

	ext	t0, t1, 0, 8	/* 8 LSBs of divisor */
	sd	t0, UART_DLL(t2)
	ext	t0, t1, 8, 8	/* 8 MSBs of divisor */
	sd	t0, UART_DLH(t2)

	/* Wait (16 * divisor * 2) + 10000 clock cycles */
	dmfc0	t3, COP0_CVMCOUNT_REG
	dsll	t1, 5	/* Divisor * 2 * 16 */
	daddiu	t1, 10000
	daddu	t1, t1, t3
baud_wait:
	dmfc0	t3, COP0_CVMCOUNT_REG
	blt	t3, t1, baud_wait
	 nop

	dli	t3, 0x13	/* Enable divisor latch, 8,N,1 */
	sd	t3, UART_LCR(t2)
	 /* Enable flow control */
	dli	t3, 0x02	/* Enable RTS, disable auto flow control */
	sd	t3, UART_MCR(t2)
	dbgchar	t0, t1, 'D'
	dbgchar	t0, t1, 'b'
#endif	/* DEBUG */

no_flash:
	/* Zero out various registers */
	mtc0	zero, COP0_DEPC_REG
	mtc0	zero, COP0_EPC_REG
	mtc0	zero, COP0_CAUSE_REG
	mfc0	a4, COP0_PROC_ID_REG
	ext	a4, a4, 8, 8
	blt	a4, OCTEON_PRID_CN63XX, octeon1_plus
	 mtc0	zero, COP0_DESAVE_REG

	/* The following are only available on Octeon 2 or later */
	mtc0	zero, COP0_KSCRATCH1_REG
	mtc0	zero, COP0_KSCRATCH2_REG
	mtc0	zero, COP0_KSCRATCH3_REG
	mtc0	zero, COP0_USERLOCAL_REG

	/* Turn off ROMEN bit to disable ROM */
	dli	a1, OCTEON_MIO_RST_BOOT
	/* For OCTEON 3 we use RST_BOOT instead of MIO_RST_BOOT.
	 * The difference is bits 24-26 are 6 instead of 0 for the address.
	 */
	/* Only CN66XX and later have ROM so we can skip this */
	blt	a4, OCTEON_PRID_CN66XX, octeon1_plus
	 nop
	/* For Octeon 2 and CN70XX we can ignore the watchdog */
	blt	a4, OCTEON_PRID_CN78XX, watchdog_ok
	 nop

	dli	a1, OCTEON_RST_BOOT

	beq	a4, OCTEON_PRID_CN70XX, watchdog_ok
	 nop

	ld	a2, 0(a1)
	/* There is a bug where some registers don't get properly reset when
	 * the watchdog timer causes a reset.  In this case we need to force
	 * a reset.
	 */
	bbit0	a2, 11, watchdog_ok	/* Skip if watchdog not hit */
	 dins	a2, zero, 2, 18	/* Don't clear LBOOT, LBOOT_EXT or LBOOT_OCI */
	/* Clear bit indicating reset due to watchdog */
	ori	a2, 1 << 11
	sd	a2, 0(a1)

	/* Disable watchdog */
	dli	a1, OCTEON_CIU3_PP_POKE(0)
	sd	zero, 0(a1)
	dli	a1, OCTEON_CIU3_WDOG(0)
	sd	zero, 0(a1)

	/* Record this in the GSER0_SCRATCH register in bit 11 */
	dli	a1, OCTEON_GSERX_SCRATCH(0)
	ld	a2, 0(a1)
	ori	a2, 1 << 11
	sd	a2, 0(a1)

	/* See if the UART is initialized by checking LCR.CLS != 0 */
	dli	a1, UART_BASE(UART_PORT)
	ld	a0, UART_LCR(a1)
	beqz	a0, no_uart
	 nop
	dla	a0, MESSAGE_RESETTING_WDOG
	bal	uart_write_string
	 nop
	/* Wait for UART to finish */
	dli	a1, UART_BASE(UART_PORT)
no_uart:
	dli	a1, OCTEON_RST_SOFT_RST
	li	a2, 1
	sd	a2, 0(a1)
	wait

	/* We should never get here */
watchdog_ok:
	ld	a2, 0(a1)
	/* Don't clear LBOOT/LBOOT_EXT or LBOOT_OCI */
	dins	a2, zero, 2, 18
	dins	a2, zero, 60, 1	/* Clear ROMEN bit */
	sd	a2, 0(a1)

octeon1_plus:
	/* Start of Octeon setup */

	/* In PCI the code before InitTLBStart is only executed on core 0.
	 * All other cores branch from the boot bus local memory section
	 * to the InitTLBStart in the relocated u-boot image.
	 */
#if defined(CONFIG_OCTEON_PCIX_WORKAROUND)
	/* Only core 0 executes this. */
	mfc0	a0, COP0_EBASE_REG
	andi	a0, EBASE_CORE_MASK		/* get core */
	bnez	a0, pcix_workaround_end
	 nop

	/* Check chip revision, and skip PCIX workaround if we are not on pass 1 */
	mfc0	a4, COP0_PROC_ID_REG
	andi	a4, 0xff
	bnez	a4, pcix_workaround_end
	 nop

	/* Set valid value for PCI_CTL_STATUS_2[TSR_HWM] */
	dli	a5, OCTEON_PCI_READ_CMD_E
	dli	a4, (1<<4)
	sw	a4, 0(a5)
pcix_workaround_end:
#endif
	/* Check what core we are - if core 0, branch to init tlb
	 * loop in flash.  Otherwise, look up address of init tlb
	 * loop that was saved in the boot vector block.
	 */
	mfc0	a0, COP0_EBASE_REG
	andi	a0, EBASE_CORE_MASK		/* get core */
	beqz	a0, InitTLBStart_local
	 nop

	break
	/* We should never get here - non-zero cores now go directly to
	 * tlb init from the boot stub in movable region.
	 */

	.globl InitTLBStart
InitTLBStart:
InitTLBStart_local:
	/* If we don't have working memory yet configure a bunch of
	 * scratch memory, and set the stack pointer to the top
	 * of it.  This allows us to go to C code without having
	 * memory set up
	 *
	 * Warning: do not change SCRATCH_STACK_LINES as this can impact the
	 * transition from start.S to crti.asm. crti requires 590 bytes of
	 * stack space.
	 */
	cache	1,0(zero)	/* Clear Dcache so cvmseg works right */
#if CONFIG_OCTEON_BIG_STACK_SIZE
	rdhwr	v0, $0
	bnez	v0, 1f
	 nop
	dla	sp, big_stack_start - 16
	b	stack_clear_done
	 nop
1:
#endif
#define SCRATCH_STACK_LINES 0x36   /* MAX is 0x36 */
	dmfc0	v0, COP0_CVMMEMCTL_REG
	dins	v0, zero, 0, 9
	/* setup SCRATCH_STACK_LINES scratch lines of scratch */
	ori	v0, 0x100 | SCRATCH_STACK_LINES
	dmtc0	v0, COP0_CVMMEMCTL_REG
	/* set stack to top of scratch memory */
	li	sp, 0xffff8000 + (SCRATCH_STACK_LINES * 128)
	/* Clear scratch for CN63XX pass 2.0 errata Core-15169*/
	li	t0, 0xffff8000
clear_scratch:
	sd	zero, 0(t0)
	addiu	t0, 8
	bne	t0, sp, clear_scratch
	 nop

	dbgchar	k0,k1,'1'
	/* This code run on all cores - core 0 from flash,
	 * the rest from DRAM.	When booting from PCI, non-zero cores
	 * come directly here from the boot vector - no earlier code in this
	 * file is executed.
	 */

	/* Some generic initialization is done here as well, as we need this
	 * done on all cores even when booting from PCI
	 */
stack_clear_done:
	/* Clear watch registers. */
	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* STATUS register */
	mfc0	k0, CP0_STATUS
	li	k1, ~ST0_IE
	and	k0, k1
	mtc0	k0, CP0_STATUS

	/* CAUSE register */
	mtc0	zero, CP0_CAUSE

	/* Init Timer */
	dmtc0	zero, CP0_COUNT
	dmtc0	zero, CP0_COMPARE


	mfc0	a5, COP0_STATUS_REG
	li	v0, 0xE0		/* enable 64 bit mode for CSR access */
	or	v0, v0, a5
	mtc0	v0, COP0_STATUS_REG


	dli	v0, 1 << 29  /* Enable large physical address support in TLB */
	mtc0	v0, COP0_PAGEGRAIN_REG

InitTLB:
	dmtc0	zero, COP0_ENTRYLO0_REG
	dmtc0	zero, COP0_ENTRYLO1_REG
	mtc0	zero, COP0_PAGEMASK_REG
	dmtc0	zero, COP0_CONTEXT_REG
	/* Use an offset into kseg0 so we won't conflict with Mips1 legacy
	 * TLB clearing
	 */
	dli	v0, 0xFFFFFFFF90000000
	mfc0	a0, COP0_CONFIG1_REG
	srl	a0, a0, 25
	/* Check if config4 reg present */
	mfc0	a1, COP0_CONFIG3_REG
	bbit0	a1, 31, 2f
	 and	a0, a0, 0x3F		/* a0 now has the max mmu entry index */
	mfc0	a1, COP0_CONFIG4_REG
	bbit0	a1, 14, 2f		/* check config4[MMUExtDef] */
	 nop
	/* append config4[MMUSizeExt] to most significant bit of
	 * config1[MMUSize-1]
	 */
	ins	a0, a1, 6, 8
	and	a0, a0, 0x3fff	/* a0 now includes max entries for cn6xxx */
2:
	dmtc0	zero, COP0_XCONTEXT_REG
	mtc0	zero, COP0_WIRED_REG

InitTLBloop:
	dmtc0	v0, COP0_ENTRYHI_REG
	nop
	nop
	tlbp
	nop
	nop
	mfc0	v1, COP0_INDEX_REG
	daddiu	v0, v0, 1<<13
	bgez	v1, InitTLBloop
	nop
	nop

	mtc0	a0, COP0_INDEX_REG
	nop
	nop
	tlbwi
	nop
	nop
	bnez	a0, InitTLBloop
	 daddiu	a0, -1

	mthi	zero
	mtlo	zero

	/* Set up status register */
	mfc0	v0, COP0_STATUS_REG
	/* Enable COP0 and COP2 access */
	li	a4, (1 << 28) | (1 << 30)
	or	v0, a4

	/* Must leave BEV set here, as DRAM is not configured for core 0.
	 * Also, BEV must be 1 later on when the exception base address is set.
	 */

	/* Mask all interrupts */
	ins	v0, zero, 0, 16
	/* Clear NMI (used to start cores other than core 0) */
#if 0
	li	a4, ~(1 << 19)
	and	v0, a4
#endif
	ori	v0, 0xE4		/* enable 64 bit, disable interrupts */
	mtc0	v0, COP0_STATUS_REG

	dli	v0,0xE000000F		/* enable all readhw locations */
	mtc0	v0, COP0_HWRENA_REG

	dmfc0	v0, COP0_CVMCTL_REG
	mfc0	a4, COP0_PROC_ID_REG
	li	a5, 0x000d0000		/* Octeon pass1 chip id */
	bne	a4, a5, skip_icachetch_disable
	 nop
	/* disable icache prefectch - errata core 8 (pass1 only) */
	ori	v0, 1<<13
skip_icachetch_disable:
	ori	v0, 1<<14	/* enable fixup of unaligned mem access */
	dmtc0	v0, COP0_CVMCTL_REG

	/* Setup scratch memory.  This is also done in
	 * cvmx_user_app_init, and this code will be removed
	 * from the bootloader in the near future.
	 */
	dmfc0	v0, COP0_CVMMEMCTL_REG
	mfc0	a4, COP0_PROC_ID_REG
	li	a5, 0x000d9000		/* Octeon pass1 chip id */
	bgt	a5, a4, 71f
	 nop
	ori	a6, a5, 8		/* Octeon cn63xx pass2 chip id */
	bge	a4, a6, 71f
	 nop
	li	a6, 4
	ins	v0, a6, 11, 4	/* Set WBTHRESH=4 as per Core-14752 errata */
71:
	dmtc0	v0, COP0_CVMMEMCTL_REG

	/* Errata        L2C-24656, Set L2C_LAD_CTL[MAXLFB] = 0 on CN73XX */
	mfc0	a4, COP0_PROC_ID_REG
	ext	a4, a4, 8, 8
	blt	a4, OCTEON_PRID_CN73XX, 72f
	nop
	dli	v0, OCTEON_L2C_TAD_CTL
	ld	t1, 0(v0)
	dins	t1, zero, 0, 4
	sd	t1, 0(v0)
	ld	zero, 0(v0)

72:

	# Apply workaround for errata Core-16057
	dmfc0	v0, COP0_CVMCTL_REG
	mfc0	v1, COP0_PROC_ID_REG
	andi	t1, v1, 0xfff8
	xori	t1, 0x9000		/* 63-P1 */
	beqz	t1, 63f
	 andi	t1, v1, 0xfff8
	xori	t1, 0x9008		/* 63-P2 */
	beqz	t1, 63f
	 andi	t1, v1, 0xfff8
	xori	t1, 0x9100		/* 68-p1 */
	beqz	t1, 63f
	 andi	t1, v1, 0xff00
	xori	t1, t1, 0x9200		/* 66-PX */
	bnez	t1, 631f		/* Skip WAR for others */
	 andi	t1, v1, 0x00ff
	slti	t1, t1, 2		/* 66-P1.2 and later good */
	beqz	t1, 631f
	 nop

63:	/* Core-16057 work around */
	or	v0, v0, 0x2000		/* Set IPERF bit */
	dmtc0	v0, COP0_CVMCTL_REG
631:
	/* No Core-16057 work around */

	/* clear these to avoid immediate interrupt in noperf mode */
	dmtc0	zero, COP0_COMPARE_REG	/* clear timer interrupt */
	dmtc0	zero, COP0_COUNT_REG	/* clear timer interrupt */
	dmtc0	zero, COP0_PERF_CNT0_REG/* clear perfCnt0 */
	dmtc0	zero, COP0_PERF_CNT1_REG/* clear perfCnt1 */

	/* If we're running on a node other than 0 then we need to set KSEGNODE
	 * to 0.  The nice thing with this code is that it also autodetects if
	 * we're running on a processor that supports CVMMEMCTL2 or not since
	 * only processors that have this will have a non-zero node ID.  Because
	 * of this there's no need to check if we're running on a 78XX.
	 */
	mfc0    t1, COP0_EBASE_REG
	dext    t1, t1, 7, 3            /* Extract node number */
	beqz    t1, is_node0            /* If non-zero then we're not node 0 */
	nop
	dmfc0   t1, COP0_CVMMEMCTL2_REG
	dins    t1, zero, 12, 4
	dmtc0   t1, COP0_CVMMEMCTL2_REG
is_node0:

	/* Set up TLB mappings for u-boot code in flash. */

#if __PIC__ > 0
/******************************************************************************/
/* Start of GP hack.  This needs to be done once properly for all code.
 * old relocation hacks need to be removed.
 */
	/* Branch and link to get current PC in ra */
	bal	2f
	 nop
	/* This contains the linked address of the GOT */
	.word	_GLOBAL_OFFSET_TABLE_
	/* The ra register now contains the runtime address of the above
	 * memory location
	 */
	/* This contains the link time address of the previous word, */
	.word	. - 4
2:
	move	gp, ra		/* Move current PC into gp register */
	lw	a5, 0(ra)	/* Load linked address of the GOT into a5 */
	lw	a6, 4(ra)	/* Load the link time address of the GOT
				 * storage location into a6
				 */
	dsubu	a5, a6		/* Subtract a6 from t1. */
	/* a5 now contains the difference between the link-time GOT table
	 * address and the link time expected PC
	 */

	/* Add this difference to the current PC (copied into gp above) so
	 * that gp now has the current runtime GOT table address
	 */
	daddu	gp, a5		/* calculate current location of offset table */
/* End of GP hack. */
/******************************************************************************/
#endif

	/* Use a bal to get the current PC into ra.  Since this bal is to
	 * the address immediately following the delay slot, the ra is
	 * the address of the label.  We then use this to get the actual
	 * address that we are executing from.
	 */
	bal	__dummy
	 nop

__dummy:
	dbgchar a5, a6, '2'
	/* Get the actual address that we are running at */
	la	a6, _start		/* Linked address of _start */
	la	a7, __dummy
	dsubu	t0, a7, a6		/* offset of __dummy label from _start*/
	dsubu	a7, ra, t0		/* a7 now has actual address of _start*/

	/* Save actual _start address in s7.  This is where we
	 * are executing from, as opposed to where the code is
	 * linked.
	 */
	move	s7, a7
	move	s4, zero

	/* s7 has actual address of _start.  If this is
	 * on the boot bus, it will be between 0xBFC000000 and 0xBFFFFFFF.
	 * If it is on the boot bus, use 0xBFC00000 as the physical address
	 * for the TLB mapping, as we will be adjusting the boot bus
	 * to make this adjustment.
	 * If we are running from DRAM (remote-boot), then we want to use the
	 * real address in DRAM.
	 */

	/* Check to see if we are running from flash - we expect that to
	 * be 0xb0000000-0xbfffffff (0x10000000-0x1fffffff, unmapped/uncached)
	 */
	li	t2, 0xb0000000
	dsubu	t2, s7
	slt	s4, s7, t2
	bltz	t2, uboot_in_flash
	 nop

	/* If we're not core 0 then we don't care about cache */
	mfc0	t2, COP0_EBASE_REG
	andi	t2, EBASE_CORE_MASK
	bnez	t2, uboot_in_ram
	 nop

	/* Find out if we're OCTEON I or OCTEON + which don't support running
	 * out of cache.
	 */
	mfc0	t2, COP0_PROC_ID_REG
	ext	t2, t2, 8, 8
	li	s4, 1
	blt	t2, 0x90, uboot_in_ram
	 nop

	/* U-Boot can be executing either in RAM or L2 cache.  Now we need to
	 * check if DRAM is initialized.  The way we do that is to look at
	 * the reset bit of the LMC0_DDR_PLL_CTL register (bit 7)
	 */
	dli	t2, OCTEON_LMC0_DDR_PLL_CTL
	ld	t2, 0(t2)
	bbit1	t2, 7, uboot_in_ram
	 nop

	/* We must be executing out of cache */
	b	uboot_in_ram
	 li	s4, 2

uboot_in_flash:
	dbgchar t1, t2, '3'
	/* Set s4 to 4 to indicate we're running in FLASH */
	li	s4, 4

#if defined(CONFIG_OCTEON_DISABLE_L2_CACHE_INDEX_ALIASING)
	/* By default, L2C index aliasing is enabled.  In some cases it may
	 * need to be disabled.  The L2C index aliasing can only be disabled
	 * if U-Boot is running out of L2 cache and the L2 cache has not been
	 * used to store anything.
	 */
	dli	t1, OCTEON_L2C_CTL
	ld	t2, 0(t1)
	ori	t2, 1
	sd	t2, 0(t1)
#endif

	/* Use BFC00000 as physical address for TLB mappings when booting
	 * from flash, as we will adjust the boot bus mappings to make this
	 * mapping correct.
	 */
	li	a7, 0xBFC00000
	dsubu	s6, s7, a7  /* Save flash offset in s6 */

#if defined(CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2) && \
    !defined(CONFIG_OCTEON_SIM_SPEED)
	/* For OCTEON II we check to see if the L2 cache is big enough to hold
	 * U-Boot.  If it is big enough then we copy ourself from flash to the
	 * L2 cache in order to speed up execution.
	 */

	/* Check for OCTEON 2 */
	mfc0	t1, COP0_PROC_ID_REG
	ext	t1, t1, 8, 8
	blt	t1, 0x90, l2_cache_too_small	/* Branch if not OCTEON II */
	 nop
	/* Get number of L2 cache sets */
	beq	t1, OCTEON_PRID_CN63XX, got_l2_sets	/* CN63XX */
	 li	t2, 1 << 10
	beq	t1, OCTEON_PRID_CN68XX, got_l2_sets	/* CN68XX */
	 li	t2, 1 << 11
	beq	t1, OCTEON_PRID_CN66XX, got_l2_sets	/* CN66XX */
	 li	t2, 1 << 10
	beq	t1, OCTEON_PRID_CN61XX, got_l2_sets	/* CN61XX */
	 li	t2, 1 << 9
	beq	t1, OCTEON_PRID_CNF71XX, got_l2_sets	/* CNF71XX */
	 li	t2, 1 << 9
	beq	t1, OCTEON_PRID_CN78XX, got_l2_sets	/* CN78XX */
	 li	t2, 1 << 13
	beq	t1, OCTEON_PRID_CN70XX, got_l2_sets	/* CN70XX */
	 li	t2, 1 << 10
	beq	t1, OCTEON_PRID_CN73XX, got_l2_sets	/* CN73XX */
	 li	t2, 1 << 11
	beq	t1, OCTEON_PRID_CNF75XX, got_l2_sets	/* CNF75XX */
	 li	t2, 1 << 11
	b	l2_cache_too_small	/* Unknown OCTEON model */
	 nop

got_l2_sets:
	/* Get number of associations */
	dli	t0, OCTEON_MIO_FUSE_DAT3
	ld	t0, 0(t0)
	dext	t0, t0, 32, 3

	beq	t1, 0x96, process_70xx_l2sets
	 nop
	/* 0 = 16-way, 1 = 12-way, 2 = 8-way, 3 = 4-way, 4-7 reserved */
	beqz	t0, got_l2_ways
	 li	t3, 16
	beq	t0, 1, got_l2_ways
	 li	t3, 12
	beq	t0, 2, got_l2_ways
	 li	t3, 8
	beq	t0, 3, got_l2_ways
	 li	t3, 4
	b	l2_cache_too_small
	 nop

process_70xx_l2sets:
	/* For 70XX, the number of ways is defined as:
	 * 0 - full cache (4-way) 512K
	 * 1 - 3/4 ways (3-way) 384K
	 * 2 - 1/2 ways (2-way) 256K
	 * 3 - 1/4 ways (1-way) 128K
	 * 4-7 illegal (aliased to 0-3)
	 */
	andi	t0, 3
	beqz	t0, got_l2_ways
	 li	t3, 4
	beq	t0, 1, got_l2_ways
	 li	t3, 3
	beq	t0, 2, got_l2_ways
	 li	t3, 2
	li	t3, 1

got_l2_ways:
	dmul	a1, t2, t3		/* Calculate cache size */
	dsll	a1, 7			/* Ways * Sets * cache line size (128) */
	daddiu	a1, a1, -128		/* Adjust cache size for copy code */

	/* Calculate size of U-Boot image */
	li	t1, CONFIG_SYS_MONITOR_BASE
	la	s5, uboot_end_data
	subu	s5, s5, t1	/* size = uboot_end - _start */

	daddu	t2, s5, s7	/* t2 = end address */
	daddiu	t2, t2, 127
	ins	t2, zero, 0, 7	/* Round up to cache line for memcpy */

	slt	t1, a1, s5	/* See if we're bigger than the L2 cache */
	bnez	t1, l2_cache_too_small
	 nop
	/* Address we plan to load at in the L2 cache */
	dli	t9, OCTEON_L2_UBOOT_ADDR
# ifdef CONFIG_OCTEON_L2_MEMCPY_IN_CACHE
	/* Enable all ways for PP0.  Authentik ROM may have disabled these */
	dli	a1, OCTEON_L2C_WPAR_PP0
	sd	zero, 0(a1)

	/* Address to place our memcpy code */
	dli	a0, OCTEON_L2_MEMCPY_ADDR
	/* The following code writes a simple memcpy routine into the cache
	 * to copy ourself from flash into the L2 cache.  This makes the
	 * memcpy routine a lot faster since each instruction can potentially
	 * require four read cycles to flash over the boot bus.
	 */
	/* Zero cache line in the L2 cache */
	zcb	(a0)
	synci	0(zero)
	dli	a1, 0xdd840000dd850008	/* ld a0, 0(t0);  ld a1, 8(t0) */
	sd	a1, 0(a0)
	dli	a1, 0xdd860010dd870018	/* ld a2, 16(t0); ld a3, 24(t0) */
	sd	a1, 8(a0)
	dli	a1, 0xfda40000fda50008	/* sd a0, 0(t1);  sd a1, 8(t1) */
	sd	a1, 16(a0)
	dli	a1, 0xfda60010fda70018	/* sd a2, 16(t1); sd a3, 24(t1) */
	sd	a1, 24(a0)
	dli	a1, 0x258c0020158efff6	/* addiu t0, 32; bne t0, t2, -40 */
	sd	a1, 32(a0)
	dli	a1, 0x25ad002003e00008	/* addiu t1, 32; jr ra */
	sd	a1, 40(a0)
	sd	zero, 48(a0)		/* nop; nop */

	/* Syncronize the caches */
	sync
	synci	0(zero)

	move	t0, s7
	move	t1, t9

	/* Do the memcpy operation in L2 cache to copy ourself from flash
	 * to the L2 cache.
	 */
	jalr	a0
	 nop

# else
	/* Copy ourself to the L2 cache from flash, 32 bytes at a time */
	/* This code is now written to the L2 cache using the coed above */
1:
	ld	a0, 0(t0)
	ld	a1, 8(t0)
	ld	a2, 16(t0)
	ld	a3, 24(t0)
	sd	a0, 0(t1)
	sd	a1, 8(t1)
	sd	a2, 16(t1)
	sd	a3, 24(t1)
	addiu	t0, 32
	bne	t0, t2, 1b
	addiu	t1, 32
# endif	/* CONFIG_OCTEON_L2_MEMCPY_IN_CACHE */

	/* Adjust the start address of U-Boot and the global pointer */
	subu	t0, s7, t9	/* t0 = address difference */
	move	s7, t9		/* Update physical address */
	move	s2, t9
#if __PIC__ > 0
	subu	gp, gp, t0	/* Adjust gp */
#endif
	sync
	synci	0(zero)

	/* Now we branch to the L2 cache.  We first get our PC then adjust it
	 */
	bal	3f
	 nop
3:
	/* Don't add any instructions here! */
	subu	t9, ra, t0
	/* Give ourself 16 bytes */
	addiu	t9, 0x10

	jal	t9		/* Branch to address in L2 cache */

	 nop
	nop
	/* Add instructions after here */

	move	a7, s7

	b	uboot_in_ram
	 ori	s4, 2		/* Running out of L2 cache */

l2_cache_too_small:	/* We go here if we can't copy ourself to L2 */
#endif /* CONFIG_OCTEON_COPY_FROM_FLASH_TO_L2 */

	/* This code is only executed if booting from flash. */
	/*  For flash boot (_not_ RAM boot), we do a workaround for
	 * an LLM errata on CN38XX and CN58XX parts.
	 */

#ifdef CONFIG_OCTEON_LLM_WORKAROUND
	/* Workaround for LLM bug where resetting
	 * the chip during LLM activity causes problems.
	 * Fix is to do another reset.	Note that this workaround
	 * does not handle the case where the user application
	 * does a soft reset during LLM activity.
	 */
	mfc0	a0, COP0_EBASE_REG
	andi	a0, EBASE_CORE_MASK		/* get core */
	bnez	a0, llm_workaround_end
	 nop

	mfc0	v0, COP0_STATUS_REG
	srl	v0, 20
	andi	v0, 1
	bnez	v0, llm_workaround_end	/* soft reset, so just continue */
	 nop

	/* This was a hard reset, so we will do a soft reset here */
	/* OCTEON III doesn't have this problem so we don't check */
	dli	a4, OCTEON_CIU_SOFT_RST
	li	a5, 1
	sd	a5, 0(a4)

	/* Loop forever after issuing soft reset */
llm_workaround_loop:
	j	llm_workaround_loop
	 nop
llm_workaround_end:
#endif

uboot_in_ram:
	/* U-boot address is now in reg a7, and is 4 MByte aligned.
	 * (boot bus addressing has been adjusted to make this happen for flash,
	 * and for DRAM this alignment must be provided by the remote boot
	 * utility.
	 */
	dbgchar	a0, a1, '4'
	/* See if we're in KSEG0 range, if so set EBASE register to handle
	 * exceptions.
	 */
	dli	a1, 0x20000000
	bge	a7, a1, 1f
	 nop
	/* Convert our physical address to KSEG0 */
	li	a1, 0x80000000
	or	a1, a1, a7
	mtc0	a1, COP0_EBASE_REG
1:
	/* U-boot now starts at 0xBFC00000.  Use a single 4 MByte TLB mapping
	 * to map u-boot.
	 */
	move	a0, a6		/* Virtual addr in a0 */
	move	a1, a7		/* Physical addr in a1 */

	/* Now we need to remove the MIPS address space bits.  For this we
	 * need to determine if it is a 32 bit compatability address or not.
	 */

	li	t0, 0x80000000 /* 'lowest' address in compatability space */
	dsubu	t0, t0, a1
	bltz	t0, compat_space
	 nop

	/* We have a xkphys address, so strip off top bit */
	dli	t0, 0x7fffffffffffffff
	and	a1, a1, t0
	b	addr_fixup_done
	 nop



compat_space:
	dbgchar	a2, a4, '5'
	dli	a2, 0x1fffffff
	and	a1, a1, a2  /* Mask phy addr to remove address space bits */

addr_fixup_done:
	/* Currenty the u-boot image size is limited to 4 MBytes.  In order to
	 * support larger images the flash mapping will need to be changed to
	 * be able to access more than that before C code is run.  Until that
	 * is done, we just use a 4 MByte mapping for the secondary cores as
	 * well.
	 */
	/* page size (only support 4 Meg binary size for now for core 0)
	 * This limitation is due to the fact that the boot vector is
	 * 0xBFC00000 which only makes 4MB available.  Later more flash
	 * address space will be available after U-Boot has been copied to
	 * RAM.	 For now assume that it is in flash.
	 */
	dbgchar	a2, a4, '6'
	li	a2, 2*1024*1024

	mfc0	a4, COP0_EBASE_REG
	andi	a4, EBASE_CORE_MASK		/* get core */
	beqz	a4, core_0_tlb
	 nop

	/* Now determine how big a mapping to use for secondary cores,
	 * which need to map all of u-boot + heap in DRAM
	 */
	/* Here we look at the alignment of the the physical address,
	 * and use the largest page size possible.  In some cases
	 * this can result in an oversize mapping, but for secondary cores
	 * this mapping is very short lived.
	 */

	/* Physical address in a1 */
	li	a2, 1
1:
	sll	a2, 1
	and	a5, a1, a2
	beqz	a5, 1b
	 nop

	/* a2 now contains largest page size we can use */
core_0_tlb:
	JAL(single_tlb_setup)

	/* Check if we're running from cache */
	bbit1	s4, 1, uboot_in_cache
	 nop

	/* If we are already running from ram, we don't need to muck
	 * with boot bus mappings.
	 */
	li	t2, 0xb0000000
	dsubu	t2, s7
	bgez	t2, uboot_in_ram2
	 nop

uboot_in_cache:
#if CONFIG_OCTEON_BIG_STACK_SIZE
	/* The large stack is only for core 0.  For all other cores we need to
	 * use the L1 cache otherwise the other cores will stomp on top of each
	 * other unless even more space is reserved for the stack space for
	 * each core.  With potentially 96 cores this gets excessive.
	 */
	mfc0	v0, COP0_EBASE_REG
	andi	a0, EBASE_CORE_MASK
	bnez	a0, no_big_stack
	 nop
	dla	sp, big_stack_start
	daddiu	sp, -16

no_big_stack:
#endif
	/* We now have the TLB set up, so we need to remap the boot bus.
	 * This is tricky, as we are running from flash, and will be changing
	 * the addressing of the flash.
	 */
	dbgchar	a4, a5, '7'
	/* Enable movable boot bus region 0, at address 0x10000000 */
	dli	a4, OCTEON_MIO_BOOT_BASE
	dli	a5, 0x81000000
	sd	a5, OCTEON_MIO_BOOT_LOC_CFG0_OFF(a4)

	/* Copy code to that remaps the boot bus to movable region */

	sd	zero, OCTEON_MIO_BOOT_LOC_DAT_OFF(a4)

	la	a6, change_boot_mappings
	GETOFFSET(a5, change_boot_mappings);
	daddu	a5, a5, a6

	ld	a7, 0(a5)
	sd	a7, OCTEON_MIO_BOOT_LOC_DAT_OFF(a4)
	ld	a7, 8(a5)
	sd	a7, OCTEON_MIO_BOOT_LOC_DAT_OFF(a4)

	/* Read from an RML register to ensure that the previous writes have
	 * completed before we branch to the movable region.
	 */
	ld	zero, OCTEON_MIO_BOOT_LOC_CFG0_OFF(a4)


	/* Compute value for boot bus configuration register */
	/* Read region 0 config so we can _modify_ the base address field */
	dli	a4, OCTEON_MIO_BOOT_REG_CFG0	/* region 0 config */
	ld	a0, 0(a4)
	dli	a4, 0xf0000000		/* Mask off bits we want to save */
	and	a4, a4, a0
	dli	a0, 0x0fff0000		/* Force size to max */
	or	a4, a4, a0

	move	a5, s6
	/* Convert to 64k blocks, as used by boot bus config */
	srl	a5, 16
	li	a6, 0x1fc0	/* 'normal' boot bus base config value */
	subu	a6, a6, a5	/* Subtract offset */
	/* combine into register value to pass to boot bus routine */
	or	a0, a4, a6

	dbgchar	k0, k1, '8'
	/* Branch there */
	la	a1, __mapped_continue_label
	dli	a2, OCTEON_MIO_BOOT_REG_CFG0
	/* If region 0 is not enabled we can skip it */
	ld	a4, 0(a2)
	bbit0	a4, 31, __mapped_continue_label
	 nop
	li	a4, 0x10000000
	j	a4
	 synci	0(zero)

	/* We never get here, as we go directly to __mapped_continue_label */
	break


uboot_in_ram2:
	dbgchar	a0, a4, '9'
	/* Now jump to address in TLB mapped memory to continue execution */
	la	a4, __mapped_continue_label
	synci	0(a4)
	j	a4
	 nop

__mapped_continue_label:
	nop
	nop
	nop
	nop

	dbgchar	a0, a1, 'A'
	/* Check if we are core 0, if we are not then we need
	 * to vector to code in DRAM to do application setup, and
	 * skip the rest of the bootloader.  Only core 0 runs the bootloader
	 * and sets up the tables that the other cores will use for
	 * configuration.
	 */
	mfc0	a0, COP0_EBASE_REG
	andi	a0, EBASE_CORE_MASK   /* get core */
	/* if (__all_cores_are_equal==0 && core==0),
	 * then jump to execute BL on core 0; else 'go to next line'
	 * (core_0_cont1 is executed ONLY when k0=a0=0(core0_ID))
	 */
	lw	t0, __all_cores_are_equal
	beq	a0, t0, core_0_cont1
	 nop

	/* other cores look up addr from dram */
        /* DRAM controller already set up by first core */
        li      a1, (BOOT_VECTOR_NUM_WORDS * 4)
        mul     a0, a0, a1

        /* Now find out the boot vector base address from the moveable boot
         * bus region.
         */

        /* Get the address of the boot bus moveable region */
        dli     t8, OCTEON_MIO_BOOT_BASE
        ld      t9, OCTEON_MIO_BOOT_LOC_CFG0_OFF(t8)
        /* Make sure it's enabled */
        bbit0   t9, 31, invalid_boot_vector
         dext   t9, t9, 3, 24
        dsll    t9, t9, 7
        /* Make address XKPHYS */
        dli     t0, 1 << 63
        or      t9, t9, t0

        ld      t0, OCTEON_BOOT_MOVEABLE_MAGIC_OFFSET(t9)
        dli     t1, OCTEON_BOOT_MOVEABLE_MAGIC1
        bne     t0, t1, invalid_boot_vector
         nop

        /* Load base address of boot vector table */
        ld      t0, OCTEON_BOOT_VECTOR_MOVEABLE_OFFSET(t9)
        /* Add offset for core */
        daddu   a1, t0, a0

	mfc0	v0, COP0_STATUS_REG
	move	v1, v0
	ins	v1, zero, 19, 1		/* Clear NMI bit */
	mtc0	v1, COP0_STATUS_REG

        /* Get app start function address */
        lw      t9, 8(a1)
        beqz    t9, invalid_boot_vector
         nop

        j       t9
         lw      k0, 12(a1)      /* Load global data (deprecated) */

invalid_boot_vector:
        dla     a0, MESSAGE_INVALID_BOOT_VECTOR
        bal     uart_write_string
         nop

        wait
        b       invalid_boot_vector
         nop

	/* Check if we are resetting via NMI.  If so, skip the bootloader
	 * and directly enter the application startup code.  Also clear NMI
	 */
	#mfc0	v0, COP0_STATUS_REG
	#move	v1, v0
	#ins	v1, zero, 19, 1		/* Clear NMI bit */
	#bbit1	v0, 19, init_secondary	/* Branch if NMI */
	 #mtc0	v1, COP0_STATUS_REG

	#beqz	a0, core_0_cont1
	 #nop

#init_secondary:
	#li	a1, (BOOT_VECTOR_NUM_WORDS * 4)
	#mul	a0, a0, a1
	#li	a1, BOOT_VECTOR_BASE
	#daddu	a1, a1, a0

	/* No cache init required */
	#lw	t9, 8(a1)
	#lw	k0, 12(a1)
	#j	t9
	 #nop
__all_cores_are_equal:
	/* The following .word tell if 'all_cores_are_equal' or core0 is special
	 * By default (for the first execution) the core0 should be special,
	 * in order to behave like the old(existing not-modified) bootloader
	 * and run the bootloader on core 0 to follow the existing design.
	 * However after that we make 'all_cores_equal' which allows to run SE
	 * applications on core0 like on any other core. NOTE that value written
	 * to '__all_cores_are_equal' should not match any core ID.
	 */
	.word 	0

core_0_cont1:
	li	t0, 0xffffffff
	sw	t0, __all_cores_are_equal
	/* From here on, only core 0 runs, other cores have branched
	 * away.
	 */
#if __PIC__ > 0
	/* Initialize GOT pointer.
	 * Global symbols can't be resolved before this is done, and as such we
	 * can't use any global symbols in this code.  We use the
	 * bal/ move xxx,ra combination to access data in a PC relative manner
	 * to avoid this.  This code will correctly set the gp regardless of
	 * whether the code has already been relocated or not.
	 * This code determines the current gp by computing the link time
	 * (gp - pc) and adding this to the current pc.
	 * runtime_gp = runtime_pc + (linktime_gp - linktime_pc)
	 * U-boot is running from the address it is linked at at this time,
	 * so this general case code is not strictly necessary here.
	 */

	/* Branch and link to get current PC in ra */
	bal	1f
	 nop

/*	.extern _GLOBAL_OFFSET_TABLE_  moved higher up - used in image header */
	/* This contains the linked address of the GOT */
	.word	_GLOBAL_OFFSET_TABLE_
	/* The ra register now contains the runtime address of the above
	 * memory location
	 */

	.word	. - 4		       /* This contains the link time address
					* of the previous word, which is also
					* what the link time expected PC value
					* is.
					*/
1:
	move	gp, ra		/* Move current PC into gp register */
	lw	a5, 0(ra)	/* Load linked address of the GOT into a5 */
	lw	a6, 4(ra)	/* Load the link time address of the GOT storage
				 * location into a6
				 */
	subu	a5, a6		/* Subtract a6 from t1. */
	/* a5 now contains the difference between the link-time GOT table
	 * address and the link time expected PC
	 */

	/* Add this difference to the current PC (copied into gp above) so
	 * that gp now has the current runtime GOT table address
	 */
	addu	gp, a5	# calculate current location of offset table
#endif

	/* Wait for UART transmit buffer to empty */
	dli	a0, UART_BASE(UART_PORT)
1:
	ld	a1, UART_LSR(a0)
	bbit0	a1, 6, 1b
	 nop

	la	t9, board_init_f  /* doesn't return... */
	move	a1, s3
	j	t9
	 move	a0, s4

/*
 * void relocate_code (addr_sp, gd, addr_moni)
 *
 * This "function" does not return, instead it continues in RAM
 * after relocating the monitor code.
 *
 * a0 = addr_sp
 * a1 = gd address (on stack)
 * a2 = destination address (physical)
 */
	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	la	t9, relocate_code_octeon
	j	t9
	 move	a3, zero	/* No mapping */
	.end	relocate_code

/**
 * void relocate_code_octeon (addr_sp, gd, addr_moni, map_size)
 *
 * This "function" does not return, instead it continues in RAM
 * after relocating the monitor code.
 *
 * a0 = addr_sp
 * a1 = gd address (on stack)
 * a2 = destination address (physical)
 * a3 = TLB page size (when TLB mapping used
 */

	.globl	relocate_code_octeon
	.ent	relocate_code_octeon
relocate_code_octeon:
	move	v0, a1		/* Save gd address */

	move	sp, a0		/* Set new stack pointer		*/


	li	a4, CONFIG_SYS_MONITOR_BASE /* Text base, 0xC0000000 */
	la	a7, in_ram
#if __PIC__ > 0
	lw	a6, -12(a7)	/* a6 <-- uboot_end_data	*/
#else
	lw	a6, -8(a7)
#endif
	move	a5, a2

	/*
	 * a4 = source address
	 * a5 = target address
	 * a6 = source end address
	 */

/* Use 64 bit copies to relocate code for speed.  We need to be careful to
 * not copy too much as BSS comes immediately after the initialized data,
 * and bss clearing is done _before_ the copy, so if too much is copied we get
 * garbage in some bss variable(s).
 * The Linker script is constructed to align the end of the initialized data
 * so that we can use 8 byte chunks.
 */
	pref	0, 0(a4)
	pref	0, 128(a4)
	pref	0, 256(a4)
	beq	a4, a5, copyDone
	 dsubu	a1, a6, 128
1:
	pref	0, 256(a4)
	ld	t0, 0(a4)
	ld	t1, 8(a4)
	ld	t2, 16(a4)
	ld	t3, 24(a4)
	sd	t0, 0(a5)
	sd	t1, 8(a5)
	sd	t2, 16(a5)
	sd	t3, 24(a5)
	ld	t0, 32(a4)
	ld	t1, 40(a4)
	ld	t2, 48(a4)
	ld	t3, 56(a4)
	sd	t0, 32(a5)
	sd	t1, 40(a5)
	sd	t2, 48(a5)
	sd	t3, 56(a5)
	ld	t0, 64(a4)
	ld	t1, 72(a4)
	ld	t2, 80(a4)
	ld	t3, 88(a4)
	sd	t0, 64(a5)
	sd	t1, 72(a5)
	sd	t2, 80(a5)
	sd	t3, 88(a5)
	ld	t0, 96(a4)
	ld	t1, 104(a4)
	ld	t2, 112(a4)
	ld	t3, 120(a4)
	daddiu	a4, 128
	sd	t0, 96(a5)
	sd	t1, 104(a5)
	sd	t2, 112(a5)
	sd	t3, 120(a5)
	syncw
	cache	27, 0(a5)		/* Flush cache line */
	blt	a4, a1, 1b
	 daddu	a5, 128			/* delay slot			*/

	/* Copy the last few dwords */
2:
	ld	t0, 0(a4)
	daddu	a4, 8
	sd	t0, 0(a5)
	blt	a4, a6, 2b
	 daddu	a5, 8

	syncw
	cache	27, 0(a5)
	/* If caches were enabled, we would have to flush them here. */
copyDone:
	/* Jump to where we've relocated ourselves.
	 */

	/* We now need to redo the TLB.	 We can call it directly
	 * since we are now running from the linked address.
	 */
	/* Now replace the single TLB mapping that was set up in flash. */
	move	a1, a2

	la	a0, _start
	/* Mapping size in a3 from above */
	move	a2, a3
	bal	single_tlb_setup
	 nop




	/* We aren't changing execution (virtual) addresses,
	 * so we don't need any address fixups here.
	 */
	la	a4, in_ram
	j	a4
	 nop
	.word	uboot_end_data
	.word	uboot_end
#if __PIC__ > 0
	.word	num_got_entries
#endif

in_ram:
	dmfc0	a0, COP0_CVMMEMCTL_REG
	dins	a0, zero, 0, 9
	mfc0	a4, COP0_PROC_ID_REG
	li	a5, 0x000d9000 /* Octeon cn63xx pass1 chip id */
	bgt	a5, a4, 51f
	 ori	 a0, 0x104	/* setup 4 lines of scratch */
	ori	a6, a5, 8      /* Octeon cn63xx pass2 chip id */
	bge	a4, a6, 51f
	 nop
	li	a6, 4
	ins	a0, a6, 11, 4  /* Set WBTHRESH=4 as per Core-14752 errata */
51:
	dmtc0	a0, COP0_CVMMEMCTL_REG

	move	a0, a1
	move	a1, a2
	move	a0, v0	/* Saved gd pointer */

	/* We are still running at the linked address */
	li	a1, CONFIG_SYS_MONITOR_BASE
	la	t9, board_init_r	/* doesn't return, runs main_loop() */
	j	t9
	 synci	0(zero)			/* Invalidate the iCache */
	.end	relocate_code_octeon


	/* Exception handlers.
	 */
romReserved:
	b romReserved
	 nop

	.globl	 romExcHandle
romExcHandle:
	/* setup stub and stack area on scratchpad */
	.set	push
	.set	noat
	sync
	cache	0, 0(zero)	/* Invalidate I-Cache */
	cache	1, 0(zero)	/* Invalidate D-Cache */
	synci	0(zero)
	dmfc0	k0, COP0_EBASE_REG
	andi	k0, k0, EBASE_CORE_MASK
	beqz	k0, 1f
	 dmfc0	k0, COP0_CVMMEMCTL_REG
	wait
1:
	dins	k0, zero, 0, 9
	/* Set local memory to 6912 bytes (0x36 cache lines) and make
	 * CVMSEG available for load/store operations in kernel/debug mode.
	 */
	ori	k0, 0x136	     /* CVMSEGENAK | 0x36 */
	dmtc0	k0, COP0_CVMMEMCTL_REG
	dmfc0	k0, COP0_CVMMEMCTL_REG

/* Note: locally defined for bootloader purpose */
#define CVMX_SCRATCH_BASE	-32768 /* 0xffffffffffff8000 */
#define CVMX_SCRATCH_SIZE	(0x36 * 128)
	/* Store 64 bit CPU Registers */
	la	k1, (CVMX_SCRATCH_BASE + CVMX_SCRATCH_SIZE)
	/* Clear scratch for CN63XX pass 2.0 errata Core-15169 */
	la	k0, CVMX_SCRATCH_BASE
clear_scratch_exc:
	sd	zero, 0(k0)
	daddiu	k0, 8
	bne	k0, k1, clear_scratch_exc
	 nop
	dsubu	k1, (96 + 1) * 8
	move	k0, k1

	sd	$0,	0(k1)
	sd	$1,	8(k1)
	sd	$2,    16(k1)
	sd	$3,    24(k1)
	sd	$4,    32(k1)
	sd	$5,    40(k1)
	sd	$6,    48(k1)
	sd	$7,    56(k1)
	sd	$8,    64(k1)
	sd	$9,    72(k1)
	sd	$10,   80(k1)
	sd	$11,   88(k1)
	sd	$12,   96(k1)
	sd	$13,  104(k1)
	sd	$14,  112(k1)
	sd	$15,  120(k1)
	sd	$16,  128(k1)
	sd	$17,  136(k1)
	sd	$18,  144(k1)
	sd	$19,  152(k1)
	sd	$20,  160(k1)
	sd	$21,  168(k1)
	sd	$22,  176(k1)
	sd	$23,  184(k1)
	sd	$24,  192(k1)
	sd	$25,  200(k1)
	sd	$26,  208(k1)
	sd	$27,  216(k1)
	sd	$28,  224(k1)
	sd	$29,  232(k1)
	sd	$30,  240(k1)
	sd	$31,  248(k1)
	.set	pop

	/* ...
	 * COP0 are read from c-asm macros
	*/

	/* acquire gp and set new sp onto scratchpad and,
	 * invoke c-wrapper for crash reporter
	*/
	move	sp, k1
#if __PIC__ > 0
	__acquire_gp	a5,a6
#endif

/* show TRAP and Core # */
_show_trap_and_core_num:
#if !CONFIG_OCTEON_SIM_HW_DIFF && defined(CONFIG_OCTEON_ENABLE_LED_DISPLAY)
	/* Print "TRAP" on rev 2 and later boards */
	_led_write_chars_8 k0,k1,'T','R','A','P',' ',' ',' ',' '
	li     k0, LED_BASE_ADDR   /* Base address of LED */
	li	k1, 0x23
	_led_write_one_char k0, k1, 0xfd

	bal	1f
	 nop
	.byte	'0'
	.byte	'1'
	.byte	'2'
	.byte	'3'
	.byte	'4'
	.byte	'5'
	.byte	'6'
	.byte	'7'
	.byte	'8'
	.byte	'9'
	.byte	'A'
	.byte	'B'
	.byte	'C'
	.byte	'D'
	.byte	'E'
	.byte	'F'
1:	mfc0	k1, COP0_EBASE_REG
	nop
	ext	k1, k1, 8, 2
	beqz	k1, 2f		/* Only write this if it is non-zero */
	 nop
	daddu	k1, ra
	lb	k1, (k1)
	nop
	_led_write_one_char k0, k1, 0xfd
	li	k1, 0x23
	_led_write_one_char k0, k1, 0xfc
2:
	ext	k1, k1, 4, 4	/* Get the core ID bits 4-7 */
	daddu	k1, ra
	lb	k1, (k1)
	nop
	_led_write_one_char k0, k1, 0xfe
	mfc0	k1, COP0_EBASE_REG
	andi	k1, 0xF		/* Get the core ID bits 0-3 */
	daddu	k1, ra
	lb	k1, (k1)
	nop
	_led_write_one_char k0, k1, 0xff
/* show TRAP and Core # */
#endif /* !CONFIG_OCTEON_SIM_HW_DIFF */

	/* Exception dumping should be improved, but still
	 * needs to be in assembly (or at least completely position independent)
	 * We skip stage 2 since we've already saved the registers.
	 */
	dli	k0, CVMX_SCRATCH_BASE + CVMX_SCRATCH_SIZE - ((96 + 1) * 8)
	move	k1, k0

	b	exception_handler_stage3
	 nop

#if CONFIG_OCTEON_SIM_HW_DIFF
	/* End simulation with break instruction */
	break
	break
#endif /*CONFIG_OCTEON_SIM_HW_DIFF*/
romExcLoop:
	wait
	b	romExcLoop
	 nop

#define BOOTLOADER_DEBUG_TRAMPOLINE_DIV_4	\
	(BOOTLOADER_DEBUG_TRAMPOLINE_CORE >> 2)

debugHandler:
	.globl debugHandler_entrypoint
	and	v0, a4

	.balign	8	/* Copied 8 bytes at a time, so make sure aligned */
	.ent   debugHandler_entrypoint
debugHandler_entrypoint:
	dmtc0	k0, COP0_DESAVE_REG
	li	k0, 0x80000000
	lw	k0, BOOTLOADER_DEBUG_TRAMPOLINE(k0)
	bnez	k0, debugHandler_notzero
	 nop

	mfc0	k0, $15, 1		/* read exception base reg. */
	andi	k0, 0x3ff		/* mask off core ID */
	sll	k0, 2			/* multiply by 4 words */
	addiu	k0, BOOTLOADER_DEBUG_TRAMPOLINE_CORE

	rotr	k0, k0, 31		/* set bit 31 for kseg0 access */
	addiu	k0, 1
	rotr	k0, k0, 1

	lw	k0, 0(k0)
	beqz	k0, debugHandler_zero
	 nop

debugHandler_notzero:
	j	k0
	 dmfc0	k0, COP0_DESAVE_REG
debugHandler_zero:
	dmfc0	k0, COP0_DESAVE_REG
	deret

	.globl OcteonBreak
OcteonBreak:
	break
	break
	.end  debugHandler_entrypoint


	/* This is the entry point of all cores except for Core 0. This code
	 * is installed by uboot at the reset vector in bootbus moveable
	 * region 0. Secondary cores never execute out of flash.
	 */
	.balign	8
	.globl	SecondaryCoreInit
	.ent	SecondaryCoreInit
SecondaryCoreInit:
	/* Enable 64 bit addressing */
	mfc0	v0, COP0_STATUS_REG
	mfc0	a0, COP0_EBASE_REG

	or	v0, 0xE0
	andi	a0, EBASE_CORE_MASK

	mtc0	v0, COP0_STATUS_REG

	/* Handle Core-14345 errata, (only in Octeon2 pass1),
	 * clear L1 Dcache virtual tags if the core hit an NMI
	 */
	cache	17, 0($0)

	/* Read the core number from EBASE so we can calculate where our jump
	 * location is stored in ram. This should always be InitTLBStart or
	 * zero.
	 */
	ori	a1, zero, BOOT_VECTOR_NUM_WORDS * 4
	lui	v0, 0xbfc0

	mul	a0, a0, a1
	ld	v0, 0x0078(v0)

	daddu	a1, v0, a0
	nop


	/* If the execution address is zero then we need to sleep until we
	 * receive a NMI. If non-zero, then we should load the stack pointer
	 * and jump to the new execution address
	 */
	ld	a0, 0(a1)
	nop
	sd	$0, 0(a1)

1:
	wait
	nop

	b 1b
	nop

2:
	j	a0
	/* Overwrite the execution address so a core will hang if run again */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	.dword	BOOTLOADER_BOOT_VECTOR

	.end	SecondaryCoreInit

	.balign	8
	.globl	simple_tlb_setup
	.ent	simple_tlb_setup
	/*
	 * a0  Virtual address
	 * a1  Physical address
	 * a2  start TLB index
	 * a3  length to map
	 */

#define SIMPLE_TLB_PAGE_SIZE	(32*1024)
simple_tlb_setup:
	/* Format physical address for entry low */
	nop
	dsrl	a1, a1, 12
	dsll	a1, a1, 6
	ori	a1, a1, 0x7		/* set DVG bits */

	li	 a4, SIMPLE_TLB_PAGE_SIZE
	daddu	 a5, a4, a4		/* mapping size */
	dsll	 a6, a4, 1
	daddi	 a6, a6, -1		/* pagemask */
	dsrl	 a4, a4, 6		/* adjust for adding with entrylo */


simple_tlb_setup_loop:
	mtc0	 a6, COP0_PAGEMASK_REG
	mtc0	a2, COP0_INDEX_REG
	daddi	 a2, a2, -1

	dmtc0	a1, COP0_ENTRYLO0_REG
	daddu	 a1, a1, a4

	dmtc0	a1, COP0_ENTRYLO1_REG
	daddu	 a1, a1, a4

	dmtc0	a0, COP0_ENTRYHI_REG
	daddu	 a0, a0, a5

	ehb
	tlbwi
	dsubu	 a3, a3, a5
	bgtz	 a3, simple_tlb_setup_loop
	 nop

	jr  ra
	 nop
	.end   simple_tlb_setup


	.balign	8
	.globl	single_tlb_setup
	.ent	single_tlb_setup
	/* Sets up a single TLB entry.	Virtual/physical addresses
	 * must be properly aligned.
	 * a0  Virtual address
	 * a1  Physical address
	 * a2  page (_not_ mapping) size
	 */
single_tlb_setup:

	/* Determine the number of TLB entries available, and
	 * use the top one.
	 */
	mfc0	a3, COP0_CONFIG1_REG
	srl	a3, a3, 25
	mfc0	a5, COP0_CONFIG3_REG /* Check if config4 reg present */
	bbit0	a5, 31, single_tlb_setup_cont
	 and	a3, a3, 0x3F	     /* a3 now has the max mmu entry index */
	mfc0	a5, COP0_CONFIG4_REG
	bbit0	a5, 14, single_tlb_setup_cont	/* check config4[MMUExtDef] */
	 nop
	/* append config4[MMUSizeExt] to most significant bit of
	 * config1[MMUSize-1]
	 */
	ins	a3, a5, 6, 8
	and	a3, a3, 0x3fff	/* a3 now includes max entries for cn6xxx */

single_tlb_setup_cont:

	/* Format physical address for entry low */
	nop
	dsrl	a1, a1, 12
	dsll	a1, a1, 6
	ori	a1, a1, 0x7	/* set DVG bits */

	move	a4, a2
	daddu	a5, a4, a4	/* mapping size */
	dsll	a6, a4, 1
	daddiu	a6, a6, -1	/* pagemask */
	dsrl	a4, a4, 6	/* adjust for adding with entrylo */

	/* Now set up mapping */
	mtc0	a6, COP0_PAGEMASK_REG
	mtc0	a3, COP0_INDEX_REG

	dmtc0	a1, COP0_ENTRYLO0_REG
	daddu	a1, a1, a4

	dmtc0	a1, COP0_ENTRYLO1_REG
	daddu	a1, a1, a4

	dmtc0	a0, COP0_ENTRYHI_REG
	daddu	a0, a0, a5

	ehb
	tlbwi
	jr  ra
	 nop
	.end   single_tlb_setup


/**
 * This code is moved to a movable boot bus region,
 * and it is responsible for changing the flash mappings and
 * jumping to run from the TLB mapped address.
 *
 * @param a0	New address for boot bus region 0
 * @param a1	Address to branch to afterwards
 * @param a2	Address of MIO_BOOT_REG_CFG0
 */
	.balign	8
change_boot_mappings:
	sd	a0, 0(a2)
	sync
	j a1	    /* Jump to new TLB mapped location */
	 synci	0(zero)

/* Clear the u-boot TLB mapping, and eret to the start of
 * the simple executive application.  The TLB index to be written
 * is already set up, as is the error EPC value.
 */
	.globl tlbwi_and_eret
	.ent   tlbwi_and_eret
tlbwi_and_eret:
	tlbwi
	nop
	nop
	cache 0, 0($0)		    /* Flush icache */
	eret

	.end tlbwi_and_eret

#ifdef CONFIG_OCTEON_SE_NAPI
/**
 * This is called from a SE application to call a native U-Boot SE API call
 *
 * int uboot_se_napi_entry(uint64_t seapi_signature_paddr)
 */
	.globl	uboot_se_napi_entry
	.ent	uboot_se_napi_entry
uboot_se_napi_entry:
	/* Convert sig addr to XKPHYS */
	dli	t3, 0x8000000000000000
	or	t0, a0, t3
	/* Check the magic number */
	ld	t1, SEAPI_SIG_MAGIC_OFFSET(t0)
	dli	t2, 0x55426f6f74415049	/* UBootAPI */
	beq	t1, t2, 1f	/* If match, continue */
	 ori	v0, zero, 1
	j	ra

1:
	move	v0, zero
	/* Add context offset */
	daddiu	t0, t0, SEAPI_CONTEXT_OFFSET

	/* Aquire the lock */
	/* Get the physical address of the lock */
	ld	t1, SEAPI_CONTEXT_SE_LOCK_ADDR_OFFSET(t0)
	/* Convert to XKPHYS */
	or	t1, t1, t3
	/* Now aquire the spinlock protecting U-Boot, this is the same code
	 * as in cvmx_spinlock_lock()
	 */
retry_lock:
	ll	t2, 0(t1)
	bnez	t2, retry_lock
	li	t2, 1
	sc	t2, 0(t1)
	beqz	t2, retry_lock
	 sync

	/* Save some of the registers we modify */
	/* We don't have to save a0-a7 but we do it anyway */
	sd	a0, 32(t0)
	sd	a1, 40(t0)
	sd	a2, 48(t0)
	sd	a3, 56(t0)
	sd	a4, 64(t0)
	sd	a5, 72(t0)
	sd	a6, 80(t0)
	sd	a7, 88(t0)
	/* Save s0 - s7 */
	sd	s0, 128(t0)
	sd	s1, 136(t0)
	sd	s2, 144(t0)
	sd	s3, 152(t0)
	sd	s4, 160(t0)
	sd	s5, 168(t0)
	sd	s6, 176(t0)
	sd	s7, 184(t0)
	/* Save k0 and k1 */
	sd	k0, 208(t0)
	sd	k1, 216(t0)
	/* Save gp, sp, s8, ra */
	sd	gp, 224(t0)
	sd	sp, 232(t0)
	sd	s8, 240(t0)
	sd	ra, 248(t0)
	sd	ra, SEAPI_CONTEXT_SE_RETURN_ADDR_OFFSET(t0)
	move	s0, t0	/* Save context address */

	/* Save the index register (probably not needed) */
	mfc0	t1, COP0_INDEX_REG
	sd	t1, SEAPI_CONTEXT_SE_C0_INDEX_OFFSET(t0)

	/* Save the status register */
	mfc0	t1, COP0_STATUS_REG
	sd	t1, SEAPI_CONTEXT_SE_C0_STATUS(t0)

	/* Get the maximum TLB entry */
	mfc0	t1, COP0_CONFIG1_REG
	srl	t1, t1, 25
	/* Check of config4 reg is present */
	mfc0	t2, COP0_CONFIG3_REG
	bbit0	t2, 31, 2f
	 andi	t1, t1, 0x3f

	mfc0	t2, COP0_CONFIG4_REG
	bbit0	t2, 14, 2f	/* Check config4[MMUExtDef] */
	 nop
	/* Append config4[MMUSizeExt] to most significant bit of
	 * config1[MMUSize - 1]
	 */
	ins	t1, t2, 6, 8
	andi	s1, t1, 0x3fff	/* s1 now includes max entries for cn6xxx */
2:
	/* Read the last TLB entry and save it */
	mtc0	s1, COP0_INDEX
	tlbr
	dmfc0	t1, COP0_ENTRYLO0_REG
	sd	t1, SEAPI_CONTEXT_SE_C0_ENTRYLO0_OFFSET(t0)
	dmfc0	t1, COP0_ENTRYLO1_REG
	sd	t1, SEAPI_CONTEXT_SE_C0_ENTRYLO1_OFFSET(t0)
	dmfc0	t1, COP0_ENTRYHI_REG
	sd	t1, SEAPI_CONTEXT_SE_C0_ENTRYHI_OFFSET(t0)
	mfc0	t1, COP0_PAGEMASK_REG
	sw	t1, SEAPI_CONTEXT_SE_C0_PAGEMASK_OFFSET(t0)

	/* Store the U-Boot TLB entry */
	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYLO0(t0)
	dmtc0	t1, COP0_ENTRYLO0_REG
	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYLO1(t0)
	dmtc0	t1, COP0_ENTRYLO1_REG
	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYHI(t0)
	dmtc0	t1, COP0_ENTRYHI_REG
	lw	t1, SEAPI_CONTEXT_UBOOT_C0_PAGEMASK(t0)
	mtc0	t1, COP0_PAGEMASK_REG
	ehb
	tlbp
	ehb
	mfc0	t1, COP0_INDEX

	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYLO0(t0)
	dmtc0	t1, COP0_ENTRYLO0_REG
	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYLO1(t0)
	dmtc0	t1, COP0_ENTRYLO1_REG
	ld	t1, SEAPI_CONTEXT_UBOOT_C0_ENTRYHI(t0)
	dmtc0	t1, COP0_ENTRYHI_REG
	lw	t1, SEAPI_CONTEXT_UBOOT_C0_PAGEMASK(t0)
	mtc0	t1, COP0_PAGEMASK_REG
	ehb
	mtc0	s1, COP0_INDEX
	ehb
	tlbwi			/* Add our TLB entry */
	ehb
	cache	0, 0(zero)	/* Flush icache */
	/* Load k0, gp and sp */
	lw	k0, SEAPI_CONTEXT_UBOOT_GD(t0)
	ld	gp, SEAPI_CONTEXT_UBOOT_GP(t0)
	lw	sp, SEAPI_CONTEXT_UBOOT_STACK(t0)
	/* Disable interrupts and get the status register */
	/* Make sure we're running in kernel mode with interrupts disabled */
	li	t2, 0xffffffe4
	mfc0	t1, COP0_STATUS_REG
	and	t1, t1, t2
	/* Set error level since U-Boot expects it */
	ori	t1, 0x00e4
	mtc0	t1, COP0_STATUS_REG
	ehb
	nop

	/* Call the C code */
	la	t9, seapi_syscall
	jal	t9
	 nop

	/* Convert sig addr to XKPHYS */
	move	t0, s0	/* Restore context address */
	/* Restore TLB entry, s1 should be preserved */
	mtc0	s1, COP0_INDEX_REG
	ld	t1, SEAPI_CONTEXT_SE_C0_ENTRYLO0_OFFSET(t0)
	dmtc0	t1, COP0_ENTRYLO0_REG
	ld	t1, SEAPI_CONTEXT_SE_C0_ENTRYLO1_OFFSET(t0)
	dmtc0	t1, COP0_ENTRYLO1_REG
	ld	t1, SEAPI_CONTEXT_SE_C0_ENTRYHI_OFFSET(t0)
	dmtc0	t1, COP0_ENTRYHI_REG
	lw	t1, SEAPI_CONTEXT_SE_C0_PAGEMASK_OFFSET(t0)
	mtc0	t1, COP0_PAGEMASK_REG
	tlbwi	/* Restore the TLB entry */
	lw	t1, SEAPI_CONTEXT_SE_C0_INDEX_OFFSET(t0)
	mtc0	t1, COP0_INDEX_REG

	/* Restore registers */
	ld	a0, 32(t0)
	ld	a1, 40(t0)
	ld	a2, 48(t0)
	ld	a3, 56(t0)
	ld	a4, 64(t0)
	ld	a5, 72(t0)
	ld	a6, 80(t0)
	ld	a7, 88(t0)
	ld	s0, 128(t0)
	ld	s1, 136(t0)
	ld	s2, 144(t0)
	ld	s3, 152(t0)
	ld	s4, 160(t0)
	ld	s5, 168(t0)
	ld	s6, 176(t0)
	ld	s7, 184(t0)
	ld	k0, 208(t0)
	ld	k1, 216(t0)
	ld	gp, 224(t0)
	ld	sp, 232(t0)
	ld	s8, 240(t0)
	ld	ra, 248(t0)
	dmtc0	ra, COP0_ERROREPC_REG
	lw	t1, SEAPI_CONTEXT_SE_C0_STATUS(t0)

	/* Unlock the spinlock */
	ld	t1, SEAPI_CONTEXT_SE_LOCK_ADDR_OFFSET(t0)
	dli	t2, 0x8000000000000000
	or	t1, t1, t2
	sd	zero, 0(t1)
	syncw
	/* Enable interrupts and clear the error level and return */
	cache	0, 0(zero)
	ei
	eret
	.end	uboot_se_napi_entry
#endif /* CONFIG_OCTEON_SE_NAPI */
/*
 * Launch 64-bit Linux kernel entry point from a 32-bit U-boot
 * a0-a3 normal args, set up by C code.	 We never come back,
 * so we keep this simple.
 * a4 is entry point
 * v0 non-zero if we are switching to little-endian mode
 * Calling C code sets up TLB to be ready for a write that clears the TLB
 * entry that u-boot uses.  This code is executed from XKPHYS address space
 * to allow the TLB entry to be removed.
 */
	.globl asm_launch_linux_entry_point
	.ent   asm_launch_linux_entry_point
asm_launch_linux_entry_point:
	tlbwi
	beqz	v0, 1f /* big-endian */
	 nop
	dmfc0	v0, COP0_CVMCTL_REG
	ori	v0, v0, 2 /* little-endian */
	nop
	dmtc0	v0, COP0_CVMCTL_REG
	nop
	cache	0, 0($0)
	nop
	cache	1, 0($0)
	nop

1:
	j	a4
	 cache	0, 0($0)	      /* Flush icache in delay slot*/
	/*
	 * In LE mode things instructions get scrambled up, put in
	 * some NOP so the scrambling doesn't do something
	 * unexpected.
	 */
	nop
	nop
	.end   asm_launch_linux_entry_point

/* Basic exception handler (dump registers) in all ASM.	 When using the TLB for
 * mapping u-boot C code, we can't branch to that C code for exception handling
 * (TLB is disabled for some exceptions.
 */

#
# void octeon_uart_write_char(char c)
#
	.globl octeon_uart_write_char
	.ent octeon_uart_write_char
octeon_uart_write_char:
	dli	a4, UART_BASE(UART_PORT)
1:
	ld	a5, UART_LSR(a4)/* Read LSR so we can see if the FIFO has room */
	bbit0	a5, 5, 1b	/* Bit 5 signals that the TX FIFO has room */
	 nop			/* Loop if there isn't any room */
	j ra			/* We're done, just return to the caller */
	 sd	a0, UART_THR(a4)/* write the char in the delay slot */
	.end octeon_uart_write_char

/*
 * void octeon_uart_write_lf(void)
 * Writes a carriage-return followed by linefeed
 */
	.globl octeon_uart_write_lf
	.ent octeon_uart_write_lf
octeon_uart_write_lf:
	dli	a4, UART_BASE(UART_PORT)
1:
	ld	a5, UART_LSR(a4)	/* Read LSR for FIFO info */
	bbit0	a5, 5, 1b		/* Loop if TX FIFO full */
	 li	a5, '\r'		/* valid in delay slot */
	sd	a5, UART_THR(a4)	/* Write character */
2:
	ld	a5, UART_LSR(a4)	/* Read LSR again */
	bbit0	a5, 5, 2b		/* Loop if TX FIFO full */
	 li	a5, '\n'		/* valid in delay slot */
	sd	a5, UART_THR(a4)	/* Write linefeed */
3:
	ld	a5, UART_LSR(a4)	/* Flush output */
	bbit0	a5, 6, 3b
	 nop
	j	ra			/* Done, return */
	 nop
	.end octeon_uart_write_lf

#
# void uart_write_string(const char *str)
#
	.ent uart_write_string
uart_write_string:
	dli	a4, UART_BASE(UART_PORT)
	move	t9, ra		/* Save ra */
	bal	uart_write_str_get_addr
	 nop
uart_write_str_get_addr:
	la	a5, _start
	la	a6, uart_write_str_get_addr
	dsubu	t0, a6, a5
	dsubu	t0, ra, t0	/* Actual address of _start */
	dsubu	t8, a0, a5	/* t8 is the offset of the string from _start */
	daddu	a5, t8, t0	/* a5 is now the actual address of the string */
	move	ra, t9		/* Restore ra */
1:
	lbu	a6, 0(a5)	/* Read next byte of string */
	beqz	a6, 3f		/* Jump out if the byte is a zero */
	 daddu	a5, 1		/* Increment out pointer to the next byte */
2:
	ld	a7, UART_LSR(a4)/* Read LSR so we can see if the FIFO has room */
	and	a7, 1<<5	/* Bit 5 signals that the TX FIFO has room */
	beqz	a7, 2b		/* Loop if there isn't any room */
	 nop
	b	1b		/* Jump to process the next byte and */
	 sd	a6, UART_THR(a4)/* write the current byte in the delay slot */
3:
	/* Flush the output */
	ld	a6, UART_LSR(a4)
	bbit0	a6, 6, 3b
	 nop
	j	ra		/* We're done, just return to the caller */
	 nop
	.end uart_write_string

#
# void uart_write_hex(uint64_t number)
#
	.ent uart_write_hex
uart_write_hex:
	move	t9, a0			/* Save a0 since C code expects that */
	move	t8, ra			/* Save ra */
	bal	octeon_uart_write_char	/* Output '0' */
	 li	a0, '0'
	bal	octeon_uart_write_char	/* Output 'x' */
	 li	a0, 'x'
	li	a7, 15			/* We need 16 digits total */
	move	a6, t9
	/* v0 being non zero signals we've passed the zero padding */
	move	v0, zero
1:
	dext	a0, a6, 60, 4		/* Extract [63:60] for display in hex */
	or	v0, a0			/* OR in the current digit, when non zero print */
	beqz	v0, 2f
	 addiu	a0, '0'			/* Add '0' to convert to '0'-'9' */
	slti	a4, a0, 0x3a		/* Flag if 0-9 instead of a-f */
	addiu	a5, a0, 0x41-0x30-10	/* Add 'a' */
	bal	octeon_uart_write_char	/* Output char */
	 movz	a0, a5, a4		/* Choose if 0-9 or a-f */
2:
	dsll	a6, 4			/* Shift to the next digit */
	bnez	a7, 1b			/* Loop while we still have digits */
	 addiu	a7, -1			/* One less digit to do */
	/* We need to write a 0 if we didn't write any digits */
	bnez	v0, 3f
	 nop
	bal	octeon_uart_write_char	/* Output '0' */
	 li	a0, '0'
3:
	/* Wait for TX to complete */
	dli	a4, UART_BASE(UART_PORT)
4:
	/* Flush the output */
	ld	a5, UART_LSR(a4)
	bbit0	a5, 6, 4b
	 nop

	move	ra, t8			/* Restore ra */
	j	ra			/* We're done, just return to the caller */
	 move	a0, t9			/* Restore a0 */
	.end uart_write_hex



	.globl exception_handler_stage2
	.ent exception_handler_stage2
exception_handler_stage2:
	.set	push
	.set	noat
	dli	k0, CVMX_SCRATCH_BASE + CVMX_SCRATCH_SIZE - ((96 + 1) * 8)
	sd	$0, 0*8(k0)
	sd	$1, 1*8(k0)
	sd	$2, 2*8(k0)
	sd	$3, 3*8(k0)
	sd	$4, 4*8(k0)
	sd	$5, 5*8(k0)
	sd	$6, 6*8(k0)
	sd	$7, 7*8(k0)
	sd	$8, 8*8(k0)
	sd	$9, 9*8(k0)
	sd	$10, 10*8(k0)
	sd	$11, 11*8(k0)
	sd	$12, 12*8(k0)
	sd	$13, 13*8(k0)
	sd	$14, 14*8(k0)
	sd	$15, 15*8(k0)
	sd	$16, 16*8(k0)
	sd	$17, 17*8(k0)
	sd	$18, 18*8(k0)
	sd	$19, 19*8(k0)
	sd	$20, 20*8(k0)
	sd	$21, 21*8(k0)
	sd	$22, 22*8(k0)
	sd	$23, 23*8(k0)
	sd	$24, 24*8(k0)
	sd	$25, 25*8(k0)
	sd	$26, 26*8(k0)
	sd	$27, 27*8(k0)
	sd	$28, 28*8(k0)
	sd	$29, 29*8(k0)
	sd	$30, 30*8(k0)
	sd	$31, 31*8(k0)
	b	    exception_handler_stage3
	 nop
	.set 	pop
	.end exception_handler_stage2

	.globl exception_handler_stage3
	.ent exception_handler_stage3
exception_handler_stage3:
	li	a1, 0			/* Start register */
	li	a2, 31			/* End register */
	bal	octeon_uart_write_lf
	 nop
1:
	bal	octeon_uart_write_char
	 li	a0, 'R'
	bal	octeon_uart_write_char
	 li	a0, 'e'
	bal	octeon_uart_write_char
	 li	a0, 'g'
	bal	octeon_uart_write_char
	 li	a0, ':'
	bal	octeon_uart_write_char
	 li	a0, ' '
	bal	uart_write_hex
	 move	a0, a1
	bal	octeon_uart_write_char
	 li	a0, ' '
	bal	uart_write_hex
	 ld	a0, 0(k0)
	bal	octeon_uart_write_lf
	 nop
	daddu	k0, 8
	bne	a1, a2, 1b
	 daddiu	a1, 1

	/* Print status */
	dla	a0, MESSAGE_STATUS
	bal	uart_write_string
	 nop
	bal	uart_write_hex
	 dmfc0	a0, COP0_STATUS_REG

	dmfc0	a0, COP0_STATUS_REG
	bbit0	a0, 19, 2f		/* Is NMI? */
	 nop
	dla	a0, MESSAGE_NMI
	bal	uart_write_string
	 nop
	dmfc0	a0, COP0_STATUS_REG
	bbit0	a0, 20, 2f		/* Is NMI, is watchdog? */
	 nop
	dla	a0, MESSAGE_WATCHDOG
	bal	uart_write_string
	 nop
2:
	dla	a0, MESSAGE_CAUSE
	bal	uart_write_string
	 nop

	bal	uart_write_hex
	 dmfc0	a0, COP0_CAUSE_REG

	dla	a0, MESSAGE_CAUSE_ARRAY
	/* Convert to real address */
	bal	__except_get_addr
	 nop
__except_get_addr:
	la	a5, _start
	la	a6, __except_get_addr
	dsubu	t0, a6, a5	/* t0 is the offset of __except_get_addr from _start */
	dsubu	t0, ra, t0	/* t0 is the actual address of _start */
	dsubu	t8, a0, a5	/* t8 is offset of array from _start */
	daddu	a5, t8, t0	/* a5 is now the actual address of the array */

	dmfc0	a0, COP0_CAUSE_REG
	andi	a0, a0, 0x7c

	daddu	a0, a5, a0	/* Calculate address in array */

	bal	uart_write_string
	 lw	a0, 0(a0)	/* Load address in array */

	dla	a0, MESSAGE_EBASE
	bal	uart_write_string
	 nop
	bal	uart_write_hex
	 mfc0	a0, COP0_EBASE_REG

	dla	a0, MESSAGE_EPC
	bal	uart_write_string
	 nop
	bal	uart_write_hex
	 dmfc0	a0, COP0_EPC_REG

	dla	a0, MESSAGE_BADVADDR
	bal	uart_write_string
	 nop

	bal	uart_write_hex
	 dmfc0	a0, COP0_BADVADDR_REG

	dla	a0, MESSAGE_INSTRUCTION
	bal	uart_write_string
	 nop
	dmfc0	a0, COP0_EPC_REG
	bal	uart_write_hex
	 lwu	a0, 0(a0)

	/* Print out stack */
	/* Load $sp into s0 */
	ld	s0, -24(k0)
	/* If we're using the L1 cache as the stack space skip printing out
	 * a stack trace since it will not be valid.
	 */
	dli	t0, 0xffffffffffff8000
	bge	s0, t0, skip_stack
	 nop
	dla	a0, MESSAGE_STACK
	bal	uart_write_string
	 nop

	bal	uart_write_hex
	 move	a0, s0
	bal	octeon_uart_write_lf
	 nop

	/* Range check start and end of stack and adjust if necessary */
	daddiu	s1, s0, -0x100	/* Start address */
	daddiu	s2, s0, 0x400	/* End address */
	dli     t0, 0xFFFFFFFFFFFF0000
	blt     s1, t0, 1f
	 nop
	ori     t0, 0x8000
	bge     s1, t0, 1f
	 nop
	move    s1, t0
1:
	dli	t0, 0xFFFFFFFFFFFF9AF0
	slt	t1, s2, t0
	bnez	t1, stack_loop
	 nop
	move	s2, t0

stack_loop:
	bal	uart_write_hex	/* Write address */
	 move	a0, s1
	bne	s1, s0, not_sp
	 nop
	bal	octeon_uart_write_char	/* Write '*' if address is SP */
	 li	a0, '*'
	b	skip_space
	 nop

not_sp:
	bal	octeon_uart_write_char	/* Otherwise space */
	 li	a0, ' '

skip_space:
	bal	octeon_uart_write_char	/* Write ': ' */
	 li	a0, ':'
	bal	octeon_uart_write_char
	 li	a0, ' '

	bal	uart_write_hex		/* Write two 64-bit values */
	 ld	a0, 0(s1)
	bal	octeon_uart_write_char
	 li	a0, ' '
	bal	uart_write_hex
	 ld	a0, 8(s1)
	bal	octeon_uart_write_lf
	 daddiu	s1, 0x10
	slt	a0, s1, s2
	bnez	a0, stack_loop		/* Loop through entries */
	 nop

skip_stack:
	dla	a0, MESSAGE_WILL_RESET
	bal	uart_write_string
	 nop

	/* Wait for UART transmit buffer to empty */
	dli	a0, UART_BASE(UART_PORT)
uart_tx_fifo_not_empty:
	ld	a1, UART_LSR(a0)
	bbit0	a1, 6, uart_tx_fifo_not_empty
	 nop

	/* Disable boot bus moveable regions */
	dli	t0, OCTEON_MIO_BOOT_LOC_CFG0
	sd	zero, 0(t0)
	sd	zero, 8(t0)
	/* Soft reset */
	dmfc0	a4, COP0_PROC_ID_REG
	ext	a4, a4, 8, 8

	.set	push
	.set	at

	/* CN78XX, CN73XX and CN75XX use CIU3 instead of CIU addresses */
	blt	a4, OCTEON_PRID_CN78XX, wdog_ciu
	 nop
	beq	a4, OCTEON_PRID_CN70XX, wdog_ciu
	 nop
	dli	a0, OCTEON_RST_SOFT_RST

	/* Perform reset */
	sync
	synci	0(zero)
	cache	9, 0(zero)
	li	a1, 1
	sd	a1, 0(a0)
2:
	wait
	b	2b
	 nop

wdog_ciu:
	dli	a1, OCTEON_CIU_WDOG(0)
	ld	a1, 0(a1)
	andi	a1, 3
	blt	a1, 3, 1f	/* Reset if NMI won't reset */
	 mfc0	a2, COP0_STATUS_REG
	bbit1	a2, 19, 2f	/* If NMI, skip reset and wait */
	 nop
1:
	dli	a0, OCTEON_RST_SOFT_RST
	beq	a4, OCTEON_PRID_CN70XX, reset_cn70xx
	 nop
	 /* non-CN70XX uses CIU_SOFT_RST instead of RST_SOFT_RST */


	dli	a0, OCTEON_CIU_SOFT_RST

reset_cn70xx:
	 /* Perform reset */
	li	a1, 1
	sync
	synci	0(zero)
	cache	9, 0(zero)
	sd	a1, 0(a0)
2:
	sync
	synci	0(zero)
	cache	9, 0(zero)
	wait
	b	2b
	 nop

.set	pop

/* NOTE: The following strings must be padded to a multiple of 4 bytes. */
	.balign 4
MESSAGE_INVALID_BOOT_VECTOR:    .string "Invalid boot vector\r\n   "
	.balign 4
MESSAGE_STATUS:		.string "status:          "
	.balign	4
MESSAGE_CAUSE:		.string "\r\ncause:       "
	.balign	4
MESSAGE_EBASE:		.string "\r\nebase:       "
	.balign	4
MESSAGE_EPC:		.string "\r\nepc:         "
	.balign	4
MESSAGE_BADVADDR:	.string "\r\nbadvaddr:    "
	.balign	4
MESSAGE_WILL_RESET:	.string "\r\n\n\nBoard will reset shortly...\r\n"
	.balign	4
MESSAGE_INSTRUCTION:	.string "\r\ninstruction: "
	.balign	4
MESSAGE_STACK:		.string "\r\nstack:       "
	.balign	4
MESSAGE_NMI:		.string " (NMI)"
	.balign	4
MESSAGE_WATCHDOG:	.string ", (Watchdog)"
	.balign	4
MESSAGE_CAUSE_INT:	.string " (Interrupt)"
	.balign	4
MESSAGE_CAUSE_MOD:	.string " (TLB modification exception)"
	.balign	4
MESSAGE_CAUSE_TLBL:	.string " (TLB exception load/fetch)"
	.balign	4
MESSAGE_CAUSE_TLBS:	.string " (TLB exception store)"
	.balign	4
MESSAGE_CAUSE_ADEL:	.string " (Address error exception (load or inst fetch))"
	.balign	4
MESSAGE_CAUSE_ADES:	.string " (Address error exception (store))"
	.balign	4
MESSAGE_CAUSE_IBE:	.string " (Bus Error exception (inst fetch))"
	.balign	4
MESSAGE_CAUSE_DBE:	.string " (Bus error exception (data reference: load or store))"
	.balign	4
MESSAGE_CAUSE_SYS:	.string " (Syscall exception)"
	.balign	4
MESSAGE_CAUSE_BP:	.string " (Breakpoint exception)"
	.balign	4
MESSAGE_CAUSE_RI:	.string " (Reserved instruction exception)"
	.balign	4
MESSAGE_CAUSE_CPU:	.string " (Coprocessor Unusable exception)"
	.balign	4
MESSAGE_CAUSE_OV:	.string " (Arithmetic overflow exception)"
	.balign	4
MESSAGE_CAUSE_TR:	.string " (Trap exception)"
	.balign	4
MESSAGE_CAUSE_UNKNOWN:	.string " (Unknown reserved exception)"
	.balign	4
MESSAGE_CAUSE_FPE:	.string " (Floating point exception)"
	.balign	4
MESSAGE_CAUSE_ID0:	.string " (Implementation dependent 0x10)"
	.balign	4
MESSAGE_CAUSE_ID1:	.string " (Implementation dependent 0x11)"
	.balign	4
MESSAGE_CAUSE_C2E:	.string " (Precise CP2 exception)"
	.balign	4
MESSAGE_CAUSE_MDMX:	.string " (MDMX unusable exception)"
	.balign	4
MESSAGE_CAUSE_WATCH:	.string " (WatchHi/WatchLo address exception)"
	.balign	4
MESSAGE_CAUSE_MCHECK:	.string " (Machine check)"
	.balign	4
MESSAGE_CAUSE_THREAD:	.string " (Thread allocation/deallocation/scheduling)"
	.balign	4
MESSAGE_CAUSE_CACHEERR:	.string " (Cache error)"
	.balign	4
MESSAGE_RESETTING_WDOG:	.string "\n\rResetting due to watchdog\n\r"
	.balign	8
MESSAGE_CAUSE_ARRAY:
.word	MESSAGE_CAUSE_INT	/* 0 */
.word	MESSAGE_CAUSE_MOD	/* 1 */
.word	MESSAGE_CAUSE_TLBL	/* 2 */
.word	MESSAGE_CAUSE_TLBS	/* 3 */
.word	MESSAGE_CAUSE_ADEL	/* 4 */
.word	MESSAGE_CAUSE_ADES	/* 5 */
.word	MESSAGE_CAUSE_IBE	/* 6 */
.word	MESSAGE_CAUSE_DBE	/* 7 */
.word	MESSAGE_CAUSE_SYS	/* 8 */
.word	MESSAGE_CAUSE_BP	/* 9 */
.word	MESSAGE_CAUSE_RI	/* 10 */
.word	MESSAGE_CAUSE_CPU	/* 11 */
.word	MESSAGE_CAUSE_OV	/* 12 */
.word	MESSAGE_CAUSE_TR	/* 13 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 14 */
.word	MESSAGE_CAUSE_FPE	/* 15 */
.word	MESSAGE_CAUSE_ID0	/* 16 */
.word	MESSAGE_CAUSE_ID1	/* 17 */
.word	MESSAGE_CAUSE_C2E	/* 18 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 19 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 20 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 21 */
.word	MESSAGE_CAUSE_MDMX	/* 22 */
.word	MESSAGE_CAUSE_WATCH	/* 23 */
.word	MESSAGE_CAUSE_MCHECK	/* 24 */
.word	MESSAGE_CAUSE_THREAD	/* 25 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 26 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 27 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 28 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 29 */
.word	MESSAGE_CAUSE_CACHEERR	/* 30 */
.word	MESSAGE_CAUSE_UNKNOWN	/* 31 */
	.end exception_handler_stage3

	.balign	8
	.globl	nmi_exception_handler
	.ent	nmi_exception_handler
nmi_exception_handler:
	mtc0	k0, COP0_DESAVE_REG
	dli	k0, UART_BASE(UART_PORT)
1:
	ld	k1, UART_LSR(k0)/* Read LSR so we can see if the FIFO has room */
	bbit0	k1, 5, 1b	/* Bit 5 signals that the TX FIFO has room */
	 nop			/* Loop if there isn't any room */
	li	k1, 'n'
	sd	k1, UART_THR(k0)/* write the char in the delay slot */

2:
	ld	k1, UART_LSR(k0)/* Read LSR so we can see if the FIFO has room */
	bbit0	k1, 5, 2b	/* Bit 5 signals that the TX FIFO has room */
	 nop			/* Loop if there isn't any room */

	li	k1, 'm'
	sd	k1, UART_THR(k0)/* write the char in the delay slot */

3:
	ld	k1, UART_LSR(k0)/* Read LSR so we can see if the FIFO has room */
	bbit0	k1, 5, 3b	/* Bit 5 signals that the TX FIFO has room */
	 nop			/* Loop if there isn't any room */
	li	k1, 'i'
	sd	k1, UART_THR(k0) /* write the char in the delay slot */

4:
	ld	k1, UART_LSR(k0)/* Read LSR so we can see if the FIFO has room */
	bbit0	k1, 5, 4b	/* Bit 5 signals that the TX FIFO has room */
	 nop			/* Loop if there isn't any room */

	li	k1, ' '
	sd	k1, UART_THR(k0)

	/* Record NMI in scratch register bit 11 */
	dli	k0, OCTEON_GSERX_SCRATCH(0)
	ld	k1, 0(k0)
	ori	k1, (1 << 11)
	j romExcHandle
	 sd	k1, 0(k0)	/* Write it in the delay slot */
	.end nmi_exception_handler

	.balign	8
	.globl exception_handler_stage1
	.ent exception_handler_stage1
exception_handler_stage1:
	j	exception_handler_stage2
	 nop
	.end exception_handler_stage1

#ifndef CONFIG_OCTEON_SIM_HW_DIFF
	/* The following code is executed by the non-zero cores as part of the
	 * code put into the boot cache memory.  That code jumps to here, where
	 * we increment the reset count and check for the proper rev_id.
	 * If we have the proper rev_id, increment the correct rev_id counter
	 * and jump back into the boot cache code after doing the things that
	 * would have been done in that code.
	 *
	 * NOTE: This code should only be called if the CPU is a CN78XX.
	 */

	.set	arch=octeon3
	.globl	revid_check_patch
	.ent	revid_check_patch
	.set	push
	.set	arch=octeon3
	.balign	8
revid_check_patch:
	/* Increment the reset count to indicate we got here */
	dli	k0, BOOTLOADER_DEBUG_CORE_RESET_COUNT
	lai	k1, (k0)

	/* Now check to see if proc_id == sli_ctl_status[rev_id] */
	mfc0	k1, COP0_PROC_ID_REG
	andi	k1, 0xff

	dli	k0, OCTEON_SLI_CTL_STATUS
	ld	k0, 0(k0)
	andi	k0, 0xff
	beq	k0, k1, 2f
	 nop

	/* Oops, we hit the bug, hang forever */
1:
	wait
	b	1b
	 nop

2:
	dli	k0, BOOTLOADER_DEBUG_CORE_REVID_COUNT
	lai	k0, (k0)	/* Increment the rev_ok count */

	/* Get core number */
	rdhwr	k0, $0
	sll	k0, k0, 5

	dli	k1, 0xffffffffbfc00000
	ld	k1, 120(k1)	/* code address to branch to */
	daddu	k1, k1, k0
	dli	k0, 0xffffffffbfc00000
	sd	zero, 0(k1)	/* Zero out boot vector entry */
	jr	k0		/* Return to reset vector to enter wait loop */
	 sync

	.set	pop
	.end	revid_check_patch


	/**
	 * This function is expected to be programmed into the boot bus
	 * moveable region with the first four word lower halfs modified
	 * with a kseg0 address to store the COP0_CVMMEMCTL,
	 * COP0_CACHEERR (icache) and COP0_CACHEERR (dcache)
	 */
	.global	bist_read_memctl_cacheerr
	.ent	bist_read_memctl_cacheerr
	.balign	8
bist_read_memctl_cacheerr:
	lui	t0, 0x8000		/* Address [48:63] */
	ori	t0, 0x0000		/* Address [32:47] */
	lui	t1, 0x0000		/* Address [16:31] */
	ori	t1, 0x0000		/* Address [0:15] */
	cache	1, 0(zero)		/* Invalidate entire L1 DCache */
	dins	t1, t0, 32, 32		/* Combine for address */
	/* Set ERL and KX */
	mfc0	t0, COP0_STATUS_REG
	ori	t0, 0x84
	mtc0	t0, COP0_STATUS_REG
	rdhwr	t2, $0			/* Get core number */
	dsll	t2, t2, 1		/* Multiply by 2 for offset */
	daddu	t1, t1, t2		/* T1 = address + 2 * core number */
	dmfc0	t0, COP0_CVMMEMCTL_REG
	dext	t0, t0, 56, 8
	dmfc0	t2, COP0_CACHEERR_ICACHE
	dext	t2, t2, 32, 6
	dins	t0, t2, 8, 8		/* Combine CVMMEMCTL and CACHEERR(icache) */
	sh	t0, 0x0000(t1)		/* Store bits 32-37 of CACHEERR_ICACHE */
	sync
1:
	wait
	b	1b
	 nop
	.end	bist_read_memctl_cacheerr
	.global bist_read_memctl_cacheerr_end
bist_read_memctl_cacheerr_end:
#endif /* CONFIG_OCTEON_SIM_HW_DIFF */
