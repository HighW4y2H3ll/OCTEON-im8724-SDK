OUTPUT_FORMAT("elf64-tradbigmips")
OUTPUT_ARCH(mips)
ENTRY(__start)
SECTIONS
{
	PROVIDE (__executable_start = SEGMENT_START("text-segment", 0xFFFFFFFF81000000));
	. = SEGMENT_START("text-segment", 0xffffffff81000000) + SIZEOF_HEADERS;
	. = 0xFFFFFFFF81000000;

        .text ALIGN(4): {
                *(.text.init)
                *(.text)
                *(.text.*)
        }
	PROVIDE(__etext = .);

        .rodata		: { *(.rodata .rodata.* .gnu.linkonce.r.*) }
        .rodata1	: { *(.rodata1) }

        .data ALIGN(8): {
		*(.data)
		*(.data.*)
		SORT(CONSTRUCTORS)
	}

	PROVIDE(_gp = ALIGN(16));
	PROVIDE(__got_start = .);
        .got ALIGN(16): { *(.got) }
	PROVIDE(__got_end = .);

	.sdata ALIGN(8): { *(.sdata .sdata.* .gnu.linkonce.s.*) }
	. = ALIGN(8);
	PROVIDE(_edata = .);

	. = ALIGN(16);
	PROVIDE(__bss_start = .);
	. = ALIGN(8);
	.sbss	: {
		*(.dynsbss)
		*(.sbss .sbss.* .gnu.linkonce.sb.*)
	}
	. = ALIGN(8);
        .bss	: {
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
	}
	. = ALIGN(8);
	.scommon		: { *(.scommon .scommon.*) }
	. = ALIGN(8);
	.cvmx_shared_bss	: { *(.cvmx_shared_bss .cvmx_shared_bss.*) }
	. = ALIGN(8);
	PROVIDE(_ebss = .);
        /* End must be aligned on a cache line */
        . = ALIGN(128);
	PROVIDE(__end = .);

        /* DWARF debug sections.
         *        Symbols in the DWARF debugging sections are relative to the beginning
         *        of the section so we begin them at 0.  */
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
        /DISCARD/ : {
                *(.note.GNU-stack)
                *(.comment)
                *(.debug)
                *(.line)
                *(.gnu.attributes)
		*(.debug_aranges)
		*(.debug_info)
		*(.debug_abbrev)
		*(.debug_line)
		*(.debug_str)
		*(.debug_loc)
		*(.debug_ranges)
		*(.gnu_debuglink)
		*(.gnu.lto_*)
        }
}
