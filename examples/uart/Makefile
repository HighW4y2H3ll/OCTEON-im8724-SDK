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
# File version info: $Id: Makefile 77242 2012-10-05 23:04:24Z cchavva $


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
	echo "uart: This example doesn't support Linux"
else
check: application-target uart_irq$(PREFIX)
endif

TARGET = uart$(PREFIX)
OBJS   = $(OBJ_DIR)/uart.o
CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused

include $(OCTEON_ROOT)/application.mk

uart_irq$(PREFIX): $(CVMX_CONFIG) $(OBJ_DIR) $(OBJ_DIR)/uart_irq.o $(LIBS_LIST)
	$(CC) $(OBJ_DIR)/uart_irq.o $(LIBS_LIST) $(LDFLAGS_GLOBAL) -o $@

clean:
	rm -f $(TARGET) uart_irq$(PREFIX) *.log
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)

run: $(TARGET)
	oct-sim $(TARGET) -quiet -uart0=2020

run_irq: uart_irq$(PREFIX)
	oct-sim uart_irq$(PREFIX) -quiet -uart0=2020

