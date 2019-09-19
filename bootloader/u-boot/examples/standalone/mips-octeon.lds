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
OUTPUT_FORMAT("elf32-ntradbigmips", "elf32-ntradbigmips", "elf32-ntradlittlemips");
OUTPUT_ARCH(mips);
ENTRY(_start);
SECTIONS
{
	PROVIDE (__executable_start = SEGMENT_START("text-segment", 0xD0000000));
	. = SEGMENT_START("text-segment", 0xD0000000);
	. = ALIGN(4);
	__text_start = .;
	.text       :
	{
		*(.text) *(.text.*)
		*(.data_in_text.*)
	}
	__text_end = .;
	. = ALIGN(8);
	__ro_data_start = .;
	.rodata  : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}
	__ro_data_end = .;
	. = ALIGN(8);
	__data_start = .;
	.data  : {
		*(.data)
	}
	__data_end = .;
	. = .;

	_gp = ALIGN(16);
	. = ALIGN(16);
	__got_start = .;
	.got : {
	  *(.got)
	}
	__got_end = .;

	. = ALIGN(8);
	__sdata_start = .;
	.sdata  : {
		*(.sdata)
	}
	__sdata_end = .;

	. = ALIGN(8);
	__bss_start = .;
	.sbss (NOLOAD) : {
		*(.sbss)
	}
	.bss (NOLOAD)  : {
		*(.bss) . = ALIGN(16);
	}
	__bss_end = .;
	. = ALIGN(8);
	_end = .;

	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.reginfo) }
	/DISCARD/ : { *(.comment) }
	/DISCARD/ : { *(.gnu.attributes) }
}
