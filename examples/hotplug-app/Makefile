######################*license start*###################################
#   Copyright (c) 2003-2017 Cavium Inc. (support@cavium.com). All rights
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
#   For any questions regarding licensing please contact marketing@cavium.comcom
# 

#
# File version info: $Id: Makefile 49202 2010-05-13 00:45:26Z cchavva $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

all: check

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
check:
	echo "hotplug_app: This example doesn't support Linux"
else
check: application-target
endif

TARGET = hotplug_app$(PREFIX)
OBJS   = $(OBJ_DIR)/hotplug_app.o
QUIET_CALLBACKS = -DQUIET_CALLBACKS=1 # don't send DEBUG messages to UART
CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused $(QUIET_CALLBACKS)

include $(OCTEON_ROOT)/application.mk


clean:
	rm -f $(TARGET)  *.log
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)

run: run2

run1: $(TARGET)
	oct-sim $(TARGET) -quiet
run2: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=2
run4: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=4
run8: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=8
run16: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=16
