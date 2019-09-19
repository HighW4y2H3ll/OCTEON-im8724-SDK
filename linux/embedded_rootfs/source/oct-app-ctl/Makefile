#.NOTPARALLEL:
######################*license start*###################################
# Copyright (c) 2013 Cavium Inc. (support@cavium.com). All rights
# reserved.
#
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#
#     * Neither the name of Cavium Inc. nor the names of
#       its contributors may be used to endorse or promote products
#       derived from this software without specific prior written
#       permission.
#
# TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
# OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
# RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
#
#
# For any questions regarding licensing please contact marketing@cavium.com
#
#####################*license end*#####################################/


ifdef TOOLCHAIN_ABI_N32
  OCTEON_TARGET=linux_n32
else
  OCTEON_TARGET=linux_64
endif

ROOTFS_PATH=../../
LIBELF_PATH=../../build/elfutils-0.155/libelf
LIBELF_LIB=libelf.a

BOOTLOADER_HEADERS=	\
  -idirafter $(LIBELF_PATH)	\
  -idirafter ${OCTEON_ROOT}/bootloader/u-boot/arch/mips/include \
  -idirafter ${OCTEON_ROOT}/bootloader/u-boot/include

MATCH=${findstring DUSE_RUNTIME_MODEL_CHECKS=1, ${OCTEON_CPPFLAGS_GLOBAL_ADD}}
ifneq (${MATCH}, DUSE_RUNTIME_MODEL_CHECKS=1)
${warning requires runtime-model, check your environment}
all:
else

all: $(LIBELF_PATH)/$(LIBELF_LIB) application-target
	-@cp $(TARGET) $(TARGET_DIR)

$(LIBELF_PATH)/$(LIBELF_LIB):
	$(MAKE) -C $(ROOTFS_PATH) libelf PKG_TARGET=build

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification

TARGET = oct-app-ctl

OBJS = 		\
  $(OBJ_DIR)/oct-app-main.o	\
  $(OBJ_DIR)/oct-app-elf.o	\
  $(OBJ_DIR)/oct-app-tlb.o	\
  $(OBJ_DIR)/oct-app-bloader.o

# There must be a cleaner way to acheive this:
LIBS_LIST += $(LIBELF_PATH)/$(LIBELF_LIB)

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter $(BOOTLOADER_HEADERS) $(BOOTLOADER_DEFINES)
include $(OCTEON_ROOT)/application.mk
endif

install: 

clean:
	rm -f $(TARGET)
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)

