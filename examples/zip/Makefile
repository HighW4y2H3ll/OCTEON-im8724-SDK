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
# File version info: $Id: Makefile 69856 2012-02-10 17:33:22Z bprakash $



ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

MODEL=${OCTEON_MODEL}

ifeq (${MODEL},OCTEON_CN30XX)
#
all:
	@echo "zip: This example doesn't apply to CN30xx"
else
#
all: application-target
endif



#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification

TARGET = zip$(PREFIX)

ifndef COMPILED_INPUT
    OBJS = $(OBJ_DIR)/zip.o
    CFLAGS_LOCAL = -O2 -g
else
.NOTPARALLEL:

    OBJS = $(OBJ_DIR)/zip.o $(OBJ_DIR)/input-file.o $(OBJ_DIR)/input_gz-file.o
    CFLAGS_LOCAL = -O2 -g -DCOMPILED_INPUT
endif
include $(OCTEON_ROOT)/application.mk

#

runv: $(TARGET) input.gz
	rm -f output output.gz
	oct-sim $(TARGET) -quiet -numcores=1 -trace=zip,iob

run: $(TARGET) input.gz
	rm -f output output.gz
	oct-sim $(TARGET) -quiet -numcores=1
	@ if diff input output; \
	then \
		echo "VERIFYING OUTPUT: input and output files match"; \
	else \
		echo "VERIFYING OUTPUT: ERROR - input and output files differ"; \
	fi

input.gz: input
	gzip -c input > input.gz

$(OBJ_DIR)/input-file.o: input

$(OBJ_DIR)/input_gz-file.o: input.gz

clean:
	rm -f $(TARGET) input.gz output output.gz log.* sizes.log
	rm -f output log.txt.gz mem?_*.txt pctrace.txt
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)
