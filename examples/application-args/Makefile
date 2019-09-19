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
# File version info: $Id: Makefile 104532 2014-09-15 23:37:36Z egoranov $
#

#  standard common Makefile fragment
                                                                                
include $(OCTEON_ROOT)/common.mk

TARGET = app-args$(PREFIX)

ifeq (linux,$(findstring linux,$(OCTEON_TARGET)))
all:
	echo "application-args: This example doesn't support Linux"
else
all: $(TARGET) ../hello/hello$(PREFIX)
endif

../hello/hello$(PREFIX):
	make -C ../hello hello$(PREFIX)


CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter


$(TARGET): app-args.c
	mipsisa64-octeon-elf-gcc -o $(TARGET) $(CFLAGS_LOCAL) $(CFLAGS_GLOBAL) app-args.c

run-args: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -stacksize=0x12345 -heapsize=0x54321 -log=sep,std -appargs="-arg1 -arg2=foo -another_arg"

run-env2: $(TARGET)
	oct-sim $(TARGET) -quiet -noperf -envfile=env2 -log=sep,std

run: run-args

run-env-multi: $(TARGET) ../hello/hello$(PREFIX)
	oct-sim $(TARGET) -ld0x11000000:../hello/hello$(PREFIX) -quiet -noperf -envfile=env-multi -numcores=2 -log=sep,std

run-env-multi-78: $(TARGET) ../hello/hello$(PREFIX)
	oct-sim $(TARGET) -ld=0x11000000:../hello/hello$(PREFIX) -quiet -noperf -envfile=env-multi-78 -numcores=2,2 -numnodes=2 -memsize=384,384
	

clean:
	rm -f output log.txt.gz mem?_*.txt pctrace.txt pp-log.*
	rm -f $(TARGET)
