######################*license start*###################################
#   Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
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
#       * Neither the name of Cavium Inc. nor the names of
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
# File version info: $Id: Makefile 79826 2013-01-11 00:36:54Z cchavva $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

# Sample minimal makefile.
# The only required targets are 'clean' and the test executable

MODEL=${OCTEON_MODEL}

ifeq (${MODEL},OCTEON_CN31XX)
  NUM_PROCESSORS=2
else
  NUM_PROCESSORS=12
endif

all: application-target

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification

TARGET = low-latency-mem$(PREFIX)

OBJS = $(OBJ_DIR)/low-latency-mem.o

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter

include $(OCTEON_ROOT)/application.mk

clean:
	rm -f $(TARGET) *.log
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)

run: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=$(NUM_PROCESSORS) -llsize=655360 | tee output.log | grep CONSOLE

run-linux:
	# Please see README-Linux.txt for addition steps needed to
	# run this example under Linux
	cd $(OCTEON_ROOT)/linux/kernel; ./oct-linux -quiet -numcores=4 -llsize=655360
