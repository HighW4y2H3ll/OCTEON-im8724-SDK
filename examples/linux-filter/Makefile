######################*license start*###################################
#   Copyright (c) 2003-2008 Cavium Inc.y (support@cavium.com). All rights
#   reserved.
# 
# 
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
# 
#       * Redistributions of source code must retain the above copyright
#         notice, this list of conditions and the following disclaimer.
# 
#       * Redistributions in binary form must reproduce the above
#         copyright notice, this list of conditions and the following
#         disclaimer in the documentation and/or other materials provided
#         with the distribution.
# 
#       * Neither the name of Cavium Inc.y nor the names of
#         its contributors may be used to endorse or promote products
#         derived from this software without specific prior written
#         permission.
# 
#   TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#   AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
#   OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
#   RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
#   REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
#   DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
#   OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
#   PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
#   POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
#   OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
# 
# 
#   For any questions regarding licensing please contact marketing@cavium.com
# 
#
#  linux-filter Makefile
#
#  $Id: Makefile 79826 2013-01-11 00:36:54Z cchavva $
#

memory=384
uart=2020
packet_port=2000
linux_path=${OCTEON_ROOT}/linux

kernel=-ld0x11000000:${linux_path}/kernel/linux/vmlinux.64
filesystem=-ld0x40000000:${linux_path}/embedded_rootfs/rootfs.ext2

#  default target

default: application-target

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include needed component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk


#  application specification

TARGET := linux-filter$(PREFIX)

OBJS = $(OBJ_DIR)/linux-filter.o

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter

include $(OCTEON_ROOT)/application.mk

#  clean target

clean:
	rm -f $(TARGET) config/cvmx-config.h
	rm -fr $(OBJ_DIR)

run: $(TARGET)
	oct-sim $(TARGET) ${kernel} ${filesystem} -envfile=u-boot-env -memsize=${memory} -uart0=${uart} -serve=${packet_port} -quiet -noperf -numcores=2

