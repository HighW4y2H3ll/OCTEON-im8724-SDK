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
# File version info: $Id: Makefile 69916 2012-02-14 14:47:03Z bprakash $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

#  standard common Makefile fragment
                                                                                
include $(OCTEON_ROOT)/common.mk

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter -I$(OCTEON_ROOT)/target/include

TARGET = hello$(PREFIX)

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
all:
	echo "hello: This example doesn't support Linux"
else
all: $(TARGET)
endif

$(TARGET): hello.c
	$(CC) -o $(TARGET) $(CFLAGS_LOCAL) $(CFLAGS_GLOBAL) hello.c

run: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -numcores=1

run4: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -numcores=4

clean:
	rm -f $(TARGET)
	rm -f output log.txt.gz mem?_*.txt pctrace.txt hello-*
