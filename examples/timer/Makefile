######################*license start*###################################
#   Copyright (c) 2014 Cavium Inc. (support@cavium.com). All rights
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
# File version info: $Id: Makefile 118129 2015-05-20 22:12:55Z egoranov $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

# IMPORTANT: 78xx_sim should be run with '-noperf' (for now) !!!
# automaticaly add '-noperf' for 78XX
ifneq (,$(findstring OCTEON_CN78, $(OCTEON_MODEL)))
	export PERF ?= -noperf
endif

all: application-target

#  standard common Makefile fragment
                                                                                
include $(OCTEON_ROOT)/common.mk

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter -I$(OCTEON_ROOT)/target/include

TARGET = timer$(PREFIX)


OBJS = $(OBJ_DIR)/timer.o

include $(OCTEON_ROOT)/application.mk

run: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=1 $(PERF) $(TRACE)

run2: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=2 $(PERF) $(TRACE)

run4: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=4 $(PERF) $(TRACE)

run8: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=8 $(PERF) $(TRACE)

run16: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=16 $(PERF) $(TRACE)

run32: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=32 $(PERF) $(TRACE)

run48: $(TARGET)
	oct-sim $(TARGET) -quiet -numcores=48 $(PERF) $(TRACE)

	
	
ifeq (,$(findstring cvmx_n32, $(OCTEON_TARGET)))
	# allow multi-node tests ONLY for OCTEON_TARGET != cvmx_n32 (n32 ABI can not access OCI nodes)

   ifneq (,$(findstring OCTEON_CN78, $(OCTEON_MODEL)))
	# allow multi-node tests ONLY on supported platforms (OCTEON_CN78XX)
#run 2 nodes: node#0 : hello; node#1 : timer; i.e.test timer works on node#1; run ~117M cycles
run_n2ht_c3: $(TARGET)
	oct-sim ../hello/hello -ld=0x11000000:$(TARGET) -quiet -numcores=3 -numnodes=2 -envfile=env-multi-78 -memsize=384,384 $(PERF) $(TRACE) -log=sep,std

#run 2 nodes: node#0 : timer; node#1 : timer; i.e.test timer works on both nodes
run_n2tt_c3: $(TARGET)
	oct-sim $(TARGET) -ld=0x11000000:$(TARGET) -quiet -numcores=3 -numnodes=2 -envfile=env-multi-78 -memsize=384,384 $(PERF) $(TRACE) -log=sep,std

   else	#ifneq (,$(findstring OCTEON_CN78, $(OCTEON_MODEL)))
	
run_n2ht_c3 run_n2tt_c3: 
	@echo "Multi-node tests are not defined/supported for this MODEL <$(OCTEON_MODEL)>."

   endif	#ifneq (,$(findstring OCTEON_CN78, $(OCTEON_MODEL)))


else	#ifeq (,$(findstring cvmx_n32, $(OCTEON_TARGET)))

run_n2ht_c3 run_n2tt_c3: 
	@echo "ERROR: n32 ABI does not support multi-node"

endif	#ifeq (,$(findstring cvmx_n32, $(OCTEON_TARGET)))

clean:
	rm -f $(TARGET) $(TARGET:.o=.d)

