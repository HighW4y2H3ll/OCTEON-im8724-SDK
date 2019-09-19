/*
 * (C) Copyright 2003
 * Wolfgang Denk Engineering, <wd@denx.de>
 *
 * (C) Copyright 2013
 * Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

OUTPUT_FORMAT("elf32-ntradbigmips", "elf32-ntradbigmips", "elf32-ntradbigmips");
OUTPUT_ARCH(mips);
ENTRY(_start);
SECTIONS
{
	/*. = 0x00000000; */
	PROVIDE (__executable_start = SEGMENT_START("text-segment", CONFIG_SYS_TEXT_BASE));
	. = SEGMENT_START("text-segment", CONFIG_SYS_TEXT_BASE) + SIZEOF_HEADERS;
	. = ALIGN(4);
	.text       :
	{
		*(.text) *(.text.*)
		*(.data_in_text.*)
	}
	uboot_start = _start;

	/* Set up the global pointer and global offset table */
	_gp = ALIGN(16);
	. = ALIGN(16);
	__got_start = .;
	.got  : { *(.got) }
	__got_end = .;

	. = ALIGN(8);
	.rodata  : { *(.rodata) *(.rodata.str*) *(.rodata.*) }

	/* .data holds pre-initialized data */
	. = ALIGN(8);
	.data  : { *(.data) *(.data.*) }

	/* .sdata holds string data */
	. = ALIGN(8);
	.sdata  : { *(.sdata) *(.sdata.*) }

	/* This is used to hold various sorted lists, for example the list of
	 * U-Boot commands and i2c interfaces.
	 */
	. = ALIGN(8);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	}

	. = ALIGN(8);
	uboot_end_data = .;  /*  align so we can copy 8 bytes at a time */
	num_got_entries = (__got_end - __got_start) >> 2;

	. = ALIGN(8);
	.sbss  : { *(.sbss) *(.sbss.*) }
	.bss  : { *(.bss) *(.bss.*) }
	.scommon : { *(.scommon) *(.scommon.*) }
	.cvmx_shared_bss : { *(.cvmx_shared_bss) *(.cvmx_shared_bss.*) }
	/* Record the end of the binary so we know where to stop copying */
	. = ALIGN(8);
	uboot_end = .;
	/* DWARF debug sections.
	Symbols in the DWARF debugging sections are relative to the beginning
	of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	.gptab.sdata : { *(.gptab.data) *(.gptab.sdata) }
	.gptab.sbss : { *(.gptab.bss) *(.gptab.sbss) }
	/DISCARD/ : { *(.note.GNU-stack) }
}
