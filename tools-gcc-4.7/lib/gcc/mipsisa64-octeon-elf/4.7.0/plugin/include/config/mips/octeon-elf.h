/* Macros for mips*-octeon-elf target.
   Copyright (C) 2004, 2005, 2006 Cavium Networks.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* Add MASK_SOFT_FLOAT and MASK_OCTEON_UNALIGNED.  */

#undef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_SOFT_FLOAT_ABI | MASK_OCTEON_UNALIGNED)

/* Forward -m*octeon-useun.  */

#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC "%{mocteon-useun} %{mno-octeon-useun}"

/* Enable backtrace including on machine exceptions by default.  */

#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC "-fasynchronous-unwind-tables"

/* Without ASM_PREFERRED_EH_DATA_FORMAT, output_call_frame_info emits
   pointer-sized addresses for FDE addresses.  For 64-bit targets, it does
   it without properly "switching over" to 64-bit as described in the DWARF3
   spec. GDB can fall back on .eh_frames and misinterpret FDE addresses.
   Instead let's be explicit and use augmentation to describe the encoding if
   pointer size is 64.  */

#undef ASM_PREFERRED_EH_DATA_FORMAT
#define ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  ((CODE) == 1 && POINTER_SIZE == 64                                    \
   ? (ABI_HAS_64BIT_SYMBOLS ? DW_EH_PE_udata8 : DW_EH_PE_udata4)        \
   : DW_EH_PE_absptr)

/* Link to libc library.  */

#undef LIB_SPEC
#define LIB_SPEC "-lc"

/* Link to startup file.  */

#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crti%O%s crtbegin%O%s crt0%O%s"

/* Default our test-only n64 configuration to -G0 since that is what
   the kernel uses.  */

#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS \
"%{mabi=64:%{!G*: -G0}}"

/* Pass linker emulation mode for N32.  */

#undef LINK_SPEC
#define LINK_SPEC "\
%(endian_spec) \
%{G*} %{mips1} %{mips2} %{mips3} %{mips4} %{mips32} %{mips32r2} %{mips64} \
%{mips64r2} %{bestGnum} %{shared} %{non_shared} \
%{mabi=n32:-melf32e%{!EL:b}%{EL:l}octeonn32} \
%{mabi=64:-melf64e%{!EL:b}%{EL:l}octeon}"

/* Override because of N32.  */

#undef LOCAL_LABEL_PREFIX
#define LOCAL_LABEL_PREFIX ((mips_abi == ABI_N32) ? "." : "$")

/* 32 gprs and 32fprs and 2 for hi/lo. */
#define DWARF_FRAME_REGISTERS 32*2+2

/* Append the core number to the GCOV filename FN.  */

#define GCOV_TARGET_SUFFIX_LENGTH 3
#define ADD_GCOV_TARGET_SUFFIX(FN)		\
do						\
  {						\
    char *fn = FN;				\
    int core;					\
    char s[4];					\
						\
    asm ("rdhwr %0, $0" : "=r"(core));		\
    sprintf (s, "%d", core);			\
    strcat (fn, s);				\
  }						\
while (0)

/* Override because simple-exec does not have TLS and can never
   have TLS variables as the bootloader never sets up the TLS
   egister. */
#undef HAVE_AS_TLS
#define HAVE_AS_TLS 0
