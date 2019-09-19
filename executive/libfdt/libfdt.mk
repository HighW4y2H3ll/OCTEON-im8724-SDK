#/***********************license start***************
# Copyright (c) 2003-2007 Cavium Inc. (support@cavium.com). All rights
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
# AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
# OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
# RESPECT TO THE SOFTWARE, aplINCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
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
# ***********************license end**************************************/

#
#  component Makefile fragment
#

#  standard component Makefile header
sp              :=  $(sp).x
dirstack_$(sp)  :=  $(d)
d               :=  $(dir)

#  component specification

LIBRARY_FDT := $(EXEC_OBJ_DIR)/libfdt.a

LIBFDT_OBJ_$(d)  :=  \
	$(EXEC_OBJ_DIR)/fdt.o \
	$(EXEC_OBJ_DIR)/fdt_ro.o \
	$(EXEC_OBJ_DIR)/fdt_rw.o \
	$(EXEC_OBJ_DIR)/fdt_strerror.o \
	$(EXEC_OBJ_DIR)/fdt_sw.o \
	$(EXEC_OBJ_DIR)/fdt_wip.o \
	$(EXEC_OBJ_DIR)/fdt_addresses.o \
	$(EXEC_OBJ_DIR)/fdt_empty_tree.o \
	$(EXEC_OBJ_DIR)/cvmx-helper-fdt.o


$(LIBFDT_OBJ_$(d)):  CFLAGS_LOCAL := -I$(d) -I$(d)/libfdt -O2 -g -W -Wall -Wno-unused-parameter -Wundef -G0
$(LIBFDT_OBJ_$(d)):  CFLAGS_GLOBAL := $(filter-out -fprofile-%,$(CFLAGS_GLOBAL))

#  standard component Makefile rules

LIBFDT_DEPS_$(d)   :=  $(LIBFDT_OBJ_$(d):.o=.d)

LIBS_LIST   :=  $(LIBS_LIST) $(LIBRARY_FDT)

LIBFDT_CLEAN_LIST  :=  $(LIBFDT_CLEAN_LIST) $(LIBFDT_OBJ_$(d)) $(LIBFDT_DEPS_$(d)) $(LIBRARY_FDT)

-include $(LIBFDT_DEPS_$(d))

$(LIBRARY_FDT): $(LIBFDT_OBJ_$(d))
	$(AR) -cr $@ $^

$(EXEC_OBJ_DIR)/%.o:	$(d)/%.c
	$(COMPILE)

$(EXEC_OBJ_DIR)/%.o:	$(d)/libfdt/%.c
	$(COMPILE)

$(EXEC_OBJ_DIR)/%.o:	$(d)/%.S
	$(ASSEMBLE)

#  standard component Makefile footer

d   :=  $(dirstack_$(sp))
sp  :=  $(basename $(sp))
