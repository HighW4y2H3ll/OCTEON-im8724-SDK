#
# (C) Copyright 2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifeq ($(CPU),octeon)
	CROSS_COMPILE ?= mips64-octeon-linux-gnu-
	STANDALONE_LOAD_ADDR = 0x80200000 -T mips-octeon.lds
else
	CROSS_COMPILE ?= mips_4KC-
	STANDALONE_LOAD_ADDR = 0x80200000 -T mips.lds
endif

ifdef CONFIG_SYS_LITTLE_ENDIAN
ENDIANNESS := -EL
endif

ifdef CONFIG_SYS_BIG_ENDIAN
ENDIANNESS := -EB
endif

# Default to EB if no endianess is configured
ENDIANNESS ?= -EB

PLATFORM_CPPFLAGS += -DCONFIG_MIPS -D__MIPS__

#
# From Linux arch/mips/Makefile
#
# GCC uses -G 0 -mabicalls -fpic as default.  We don't want PIC in the kernel
# code since it only slows down the whole thing.  At some point we might make
# use of global pointer optimizations but their use of $28 conflicts with
# the current pointer optimization.
#
# Note that for Octeon we require -fPIC in order to work properly with their
# toolchain.
#
# The DECStation requires an ECOFF kernel for remote booting, other MIPS
# machines may also.  Since BFD is incredibly buggy with respect to
# crossformat linking we rely on the elf2ecoff tool for format conversion.
#
# cflags-y			+= -G 0 -mno-abicalls -fno-pic -pipe
# cflags-y			+= -msoft-float
# LDFLAGS_vmlinux		+= -G 0 -static -n -nostdlib
# MODFLAGS			+= -mlong-calls
#
# On the other hand, we want PIC in the U-Boot code to relocate it from ROM
# to RAM. $28 is always used as gp.
#
ifeq ($(CPU),octeon)
	PLATFORM_CPPFLAGS	+= -G 0 -mabicalls -mabi=n32
	PLATFORM_CPPFLAGS	+= -msoft-float -D__MIPS64__
	PLATFORM_LDFLAGS	+= -G 0 -static -n -nostdlib -m elf32btsmipn32
	LDFLAGS_FINAL           += --gc-sections
else
	PLATFORM_CPPFLAGS	+= -G 0 -mabicalls -fpic
        PLATFORM_CPPFLAGS	+= -msoft-float
        PLATFORM_LDFLAGS	+= -G 0 -static -n -nostdlib
       LDFLAGS_FINAL		+= --gc-sections -pie
endif

PLATFORM_REFLAGS                += -ffunction-sections -fdata-sections
OBJCFLAGS                       += --remove-section=.dynsym
