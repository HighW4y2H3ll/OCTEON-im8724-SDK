/*
 * (C) Copyright 2003
 * Wolfgang Denk Engineering, <wd@denx.de>
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

/*
OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-bigmips")
*/
OUTPUT_FORMAT("elf32-ntradbigmips", "elf32-ntradbigmips", "elf32-ntradbigmips")
OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text       :
	{
		_start = .;
		arch/mips/cpu/octeon/start.o	(.text*)
		*(.text) *(.text.*)
		*(.data_in_text.*)
	}

	uboot_start = _start;

	. = ALIGN(8);
	.rodata  : { *(.rodata)  *(.rodata.*)}

	. = ALIGN(8);
	.data  : { *(.data) *(.data.*) }

	_gp = ALIGN(16);

	. = ALIGN(16);
	__got_start = .;
	.got  : { *(.got) }
	__got_end = .;
	num_got_entries = (__got_end - __got_start) >> 2;

	. = ALIGN(8);
	.sdata  : { *(.sdata) *(.sdata.*) }

	. = ALIGN(8);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)))
	}

	. = ALIGN(8);
	uboot_end_data = .;
	__image_copy_end = .;

	.sbss  : { *(.sbss) *(.sbss.*) }
	.bss  : { *(.bss) *(.bss.*) }
	.scommon : { *(.scommon) *(.scommon.*) }
	.cvmx_shared_bss : { *(.cvmx_shared_bss) *(.cvmx_shared_bss.*) }
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
