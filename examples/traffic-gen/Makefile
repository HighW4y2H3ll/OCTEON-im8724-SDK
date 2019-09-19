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
# File version info: $Id: Makefile 143543 2016-08-16 00:15:09Z cchavva $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

ifeq (${OCTEON_MODEL},OCTEON_CN30XX)
all:
	@echo "traffic-gen: This example requires multiple cores and is not supported on CN30xx"
else

all: application-target

endif

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments
ifdef CVMX_CMD_DUMP
CFLAGS_GLOBAL += -DCVMX_CMD_DUMP=1	# Enable 'dump' cmd
CFLAGS_GLOBAL += -DCVMX_DUMP_PKI=1	# Display PKI configuration and statistics
CFLAGS_GLOBAL += -DCVMX_DUMP_PKO=1	# Display PKO configuration and statistics
CFLAGS_GLOBAL += -DCVMX_DUMP_SSO=1	# Display SSO configuration and statistics
CFLAGS_GLOBAL += -DCVMX_DUMP_BGX=1	# Display BGX configuration and status
CFLAGS_GLOBAL += -DCVMX_DUMP_GSER=1	# Display GSER configuration and status
endif
dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification

TARGET = traffic-gen$(PREFIX)

CFLAGS_LOCAL = -g -O3 -W -Wall -Wno-unused-parameter -lm
CFLAGS_LOCAL += -I${OCTEON_ROOT}/bootloader/u-boot/include/asm/arch
OBJS = $(OBJ_DIR)/traffic-gen.o $(OBJ_DIR)/traffic-gen-pktio.o

include $(OCTEON_ROOT)/application.mk

clean:
	rm -f $(TARGET)
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)
	rm -f .cstamp*
ifdef CVMX_CMD_DUMP
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/libcvmx.a
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/cvmx-qlm.o
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/cvmx-pki.o
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/cvmx-helper-bgx.o
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/cvmx-helper-pko3.o
	rm -f $(OCTEON_EXEC_DIR)/$(OBJ_DIR)/cvmx-pow.o
endif

distclean: clean
	rm -rf $(OBJ_DIR)

.PHONY: flash
flash: ${TARGET}
	mount /mnt/cf1
	mipsisa64-octeon-elf-strip -o /mnt/cf1/traffic-gen ${TARGET}

.PHONY: tftp
tftp: ${TARGET}
	mipsisa64-octeon-elf-strip -o /tftpboot/traffic-gen ${TARGET}

