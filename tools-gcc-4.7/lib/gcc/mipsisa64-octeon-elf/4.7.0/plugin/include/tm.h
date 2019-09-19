#ifndef GCC_TM_H
#define GCC_TM_H
#define TARGET_CPU_DEFAULT (((MASK_SPLIT_ADDRESSES)|MASK_EXPLICIT_RELOCS)|MASK_RELAX_PIC_CALLS)
#ifndef MIPS_ABI_DEFAULT
# define MIPS_ABI_DEFAULT ABI_EABI
#endif
#ifndef MIPS_CPU_STRING_DEFAULT
# define MIPS_CPU_STRING_DEFAULT "octeon"
#endif
#ifdef IN_GCC
# include "options.h"
# include "insn-constants.h"
# include "config/elfos.h"
# include "config/newlib-stdint.h"
# include "config/mips/mips.h"
# include "config/mips/elf.h"
# include "config/mips/octeon.h"
# include "config/mips/octeon-elf.h"
# include "config/initfini-array.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE && !defined USED_FOR_TARGET
# include "insn-flags.h"
#endif
# include "defaults.h"
#endif /* GCC_TM_H */
