/* Macros for mips*-octeon-linux target.
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

/* Forward -m*octeon-useun.  */

#undef SUBTARGET_ASM_SPEC
#define SUBTARGET_ASM_SPEC			 \
  "%{!mno-abicalls:%{mplt:-call_nonpic;:-KPIC}}" \
  " %{mocteon-useun} %{mno-octeon-useun} %{msoft-float} %{msingle-float}"

/* A standard GNU/Linux mapping.  On most targets, it is included in
   CC1_SPEC itself by config/linux.h, but mips.h overrides CC1_SPEC
   and provides this hook instead.  */
#undef SUBTARGET_CC1_SPEC
#define SUBTARGET_CC1_SPEC "%{profile:-p} -fasynchronous-unwind-tables"

/* Octeon3 should use /lib*-fp . */
#undef GNU_USER_DYNAMIC_LINKERN32
#define GNU_USER_DYNAMIC_LINKERN32 "%{ march=octeon3: /lib32-fp/ld.so.1 ; \
				    : /lib32/ld.so.1 }"

#undef GNU_USER_DYNAMIC_LINKER64
#define GNU_USER_DYNAMIC_LINKER64 "%{ march=octeon3: /lib64-fp/ld.so.1 ; \
				    : /lib64/ld.so.1 }"

/* include executable stack flag at end of asm file */
#undef TARGET_ASM_FILE_END
#define TARGET_ASM_FILE_END file_end_indicate_exec_stack


/* Force libc providing ssp */
#undef TARGET_LIBC_PROVIDES_SSP
#define TARGET_LIBC_PROVIDES_SSP
