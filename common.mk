#/***********************license start***************
# * Copyright (c) 2003-2007  Cavium Inc. (support@cavium.com). All rights
# * reserved.
# *
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions are
# * met:
# *
# *     * Redistributions of source code must retain the above copyright
# *       notice, this list of conditions and the following disclaimer.
# *
# *     * Redistributions in binary form must reproduce the above
# *       copyright notice, this list of conditions and the following
# *       disclaimer in the documentation and/or other materials provided
# *       with the distribution.
# *
# *     * Neither the name of Cavium Inc. nor the names of
# *       its contributors may be used to endorse or promote products
# *       derived from this software without specific prior written
# *       permission.
# *
# * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
# * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
# * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
# *
# *
# * For any questions regarding licensing please contact marketing@cavium.com
# *
# ***********************license end**************************************/
#
#  common Makefile fragment
#
#  $Id: common.mk 135195 2016-03-17 16:19:18Z cchavva $
#

#  Octeon tools

#  Octeon default flags
CPPFLAGS_GLOBAL = -I$(OCTEON_ROOT)/target/include -Iconfig  -I$(OCTEON_ROOT)/executive/libfdt

CPPFLAGS_GLOBAL += $(OCTEON_CPPFLAGS_GLOBAL_ADD)

CPPFLAGS_GLOBAL += -W -Wall -Werror -Wno-unused-parameter -Wdeclaration-after-statement -Wno-error=declaration-after-statement

CFLAGS_GLOBAL = $(CPPFLAGS_GLOBAL)
CFLAGS_GLOBAL += $(OCTEON_CFLAGS_GLOBAL_ADD)

ASFLAGS_GLOBAL = $(CPPFLAGS_GLOBAL) -g
ASFLAGS_GLOBAL += $(OCTEON_ASFLAGS_GLOBAL_ADD)

LDFLAGS_GLOBAL =
LDFLAGS_GLOBAL += $(OCTEON_LDFLAGS_GLOBAL_ADD)

LDFLAGS_PATH = -L$(OCTEON_ROOT)/target/lib

include $(OCTEON_ROOT)/common-config.mk
# Add flags set by common-config.mk
CFLAGS_GLOBAL += $(CFLAGS_COMMON_CONFIG)
ASFLAGS_GLOBAL += $(CFLAGS_COMMON_CONFIG)

ifeq ($(CVMX_USE_1_TO_1_TLB_MAPPINGS), 1)
CFLAGS_GLOBAL += -DCVMX_USE_1_TO_1_TLB_MAPPINGS=1
else
CFLAGS_GLOBAL += -DCVMX_USE_1_TO_1_TLB_MAPPINGS=0
endif

# Add profile-feedback flags.
ifdef FDO_PASS
ifeq ($(FDO_PASS), 1)
CFLAGS_GLOBAL += -fprofile-generate -ftest-coverage
LDFLAGS_GLOBAL += -fprofile-generate -ftest-coverage
else # 1
ifeq ($(FDO_PASS), 2)
CFLAGS_GLOBAL += -fprofile-use
LDFLAGS_GLOBAL += -fprofile-use
else # 2
$(error FDO_PASS should either be 1 or 2)
endif # 2
endif # 1
endif # FDO_PASS

ifdef OCTEON_ENABLE_LTO
CFLAGS_GLOBAL += -flto
LDFLAGS_GLOBAL += -flto
endif

ifdef OCTEON_DISABLE_BACKTRACE
CFLAGS_GLOBAL += -fno-asynchronous-unwind-tables -DOCTEON_DISABLE_BACKTRACE
endif

ifndef OCTEON_TARGET
   OCTEON_TARGET=cvmx_64
endif


# Use Octeon2 ISA when compiling for OCTEON II models.
OCTEON_ISA = octeon
USE_OCTEON2_ISA = 0
ifneq ($(findstring OCTEON_CN6, $(OCTEON_MODEL)),)
   USE_OCTEON2_ISA = 1
else
   ifneq ($(findstring OCTEON_CNF71, $(OCTEON_MODEL)),)
       USE_OCTEON2_ISA = 1
   endif
endif

ifneq ($(findstring OCTEON_CN7, $(OCTEON_MODEL)),)
   USE_OCTEON3_ISA = 1
else
   ifneq ($(findstring OCTEON_CNF75, $(OCTEON_MODEL)),)
       USE_OCTEON3_ISA = 1
   endif
   ifneq ($(findstring OCTEON_CN23, $(OCTEON_MODEL)),)
       USE_OCTEON3_ISA = 1
   endif
endif

ifeq ($(USE_OCTEON2_ISA),1)
   OCTEON_ISA = octeon2
   # Enable Core-14449 errata in OCTEON II models.
   CFLAGS_GLOBAL += -mfix-cn63xxp1
   CFLAGS_LOCAL += -mfix-cn63xxp1
   LDFLAGS_GLOBAL += -mfix-cn63xxp1
#  Disable Core-14449 errate in Octeon II models.
ifdef OCTEON_NO_FIX_CN63XXP1
   CFLAGS_GLOBAL += -mno-fix-cn63xxp1
   CFLAGS_LOCAL += -mno-fix-cn63xxp1
   LDFLAGS_GLOBAL += -mno-fix-cn63xxp1
endif # OCTEON_NO_FIX_CN63XXP1
endif

ifeq ($(USE_OCTEON3_ISA),1)
   OCTEON_ISA = octeon3
endif

# Use the ISA provided by embedded rootfs
ifdef OCTEONISA
   OCTEON_ISA = $(OCTEONISA)
endif

OCTEONLE =

# Use little endian toolchain and linker script
ifdef OCTEON_LE
   OCTEONLE = el
endif

LDFLAGS_GLOBAL += -march=${OCTEON_ISA}

SUPPORTED_TARGETS=linux_64 linux_n32 linux_o32 cvmx_n32 cvmx_64
ifeq ($(findstring $(OCTEON_TARGET), $(SUPPORTED_TARGETS)),)
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif

ifeq (${OCTEON_TARGET},linux_64)
    PREFIX=-linux_64${OCTEONLE}
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=${OCTEON_ISA} -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=64 -march=${OCTEON_ISA} -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=64 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux${OCTEONLE}.ld
else # linux_64
ifeq (${OCTEON_TARGET},linux_n32)
    PREFIX=-linux_n32${OCTEONLE}
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=${OCTEON_ISA} -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=n32 -march=${OCTEON_ISA} -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=n32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-n32${OCTEONLE}.ld
else # linux_n32
ifeq (${OCTEON_TARGET},linux_o32)
    PREFIX=-linux_o32${OCTEONLE}
    CFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=${OCTEON_ISA} -Dmain=appmain
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=${OCTEON_TARGET} -mabi=32 -march=${OCTEON_ISA} -Dmain=appmain
    LDFLAGS_GLOBAL += -mabi=32 -static -lrt -Xlinker -T -Xlinker $(OCTEON_ROOT)/target/lib/cvmx-shared-linux-o32.ld
else # linux_o32
ifeq (${OCTEON_TARGET},cvmx_n32)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32 -march=${OCTEON_ISA}
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_n32 -mabi=n32 -march=${OCTEON_ISA}
    LDFLAGS_GLOBAL += -mabi=n32
    PREFIX=-cvmx_n32
else # cvmx_n32
ifeq (${OCTEON_TARGET},cvmx_64)
    CFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64 -march=${OCTEON_ISA}
    ASFLAGS_GLOBAL += -DOCTEON_TARGET=cvmx_64 -march=${OCTEON_ISA}
else # cvmx_64
    ${error Invalid value for OCTEON_TARGET. Supported values: ${SUPPORTED_TARGETS}}
endif # cvmx_64
endif # cvmx_n32
endif # linux_o32
endif # linux_n32
endif # linux_64

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
    CC = mips64${OCTEONLE}-octeon-linux-gnu-gcc
    AR = mips64${OCTEONLE}-octeon-linux-gnu-ar
    LD = mips64${OCTEONLE}-octeon-linux-gnu-ld
    STRIP = mips64${OCTEONLE}-octeon-linux-gnu-strip
    OBJDUMP = mips64${OCTEONLE}-octeon-linux-gnu-objdump
    NM = mips64${OCTEONLE}-octeon-linux-gnu-nm
else
    CC = mipsisa64-octeon-elf-gcc
    AR = mipsisa64-octeon-elf-ar
    LD = mipsisa64-octeon-elf-ld
    STRIP = mipsisa64-octeon-elf-strip
    OBJDUMP = mipsisa64-octeon-elf-objdump
    NM = mipsisa64-octeon-elf-nm
endif

#  build object directory

OBJ_DIR = obj$(PREFIX)-$(OCTEON_ISA)


ifndef OCTEON_EXEC_DIR
    OCTEON_EXEC_DIR = $(OCTEON_ROOT)/executive
endif

ifeq ($(OCTEON_USE_LOCAL_LIB),1)
EXEC_OBJ_DIR = obj$(PREFIX)-$(OCTEON_ISA)
else
EXEC_OBJ_DIR = $(OCTEON_EXEC_DIR)/obj$(PREFIX)-$(OCTEON_ISA)
endif

ifdef OCTEON_SINGLE_DIR
EXEC_OBJ_DIR = $(OCTEON_SINGLE_DIR)/obj$(PREFIX)-$(OCTEON_ISA)
OBJ_DIR = $(OCTEON_SINGLE_DIR)/obj$(PREFIX)-$(OCTEON_ISA)
endif

OBJ_DIR_STAMP = $(OBJ_DIR)/.stamp
EXEC_OBJ_DIR_STAMP = $(EXEC_OBJ_DIR)/.execstamp
#  standard compile line

COMPILE = $(CC) $(CFLAGS_GLOBAL) $(CFLAGS_LOCAL) -MMD -c -o $@ $<

ASSEMBLE = $(CC) $(ASFLAGS_GLOBAL) $(ASFLAGS_LOCAL) -MMD -c -o $@ $<

