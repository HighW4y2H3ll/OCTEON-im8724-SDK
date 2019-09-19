# Copyright (c) 2003-2005, Cavium Inc. All rights reserved.
#
# This Software is the property of Cavium Inc.  The Software and all
# accompanying documentation are copyrighted.  The Software made available
# here constitutes the proprietary information of Cavium Inc.  You
# agree to take reasonable steps to prevent the disclosure, unauthorized use
# or unauthorized distribution of the Software.  You shall use this Software
# solely with Cavium hardware.
#
# Except as expressly permitted in a separate Software License Agreement
# between You and Cavium Inc.  you shall not modify, decompile,
# disassemble, extract, or otherwise reverse engineer this Software.  You
# shall not make any copy of the Software or its accompanying documentation,
# except for copying incident to the ordinary and intended use of the
# Software and the Underlying Program and except for the making of a single
# archival copy.
#
# This Software, including technical data, may be subject to U.S.  export
# control laws, including the U.S.  Export Administration Act and its
# associated regulations, and may be subject to export or import regulations
# in other countries.  You warrant that You will comply strictly in all
# respects with all such regulations and acknowledge that you have the
# responsibility to obtain licenses to export, re-export or import the
# Software.
#
# TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# AND WITH ALL FAULTS AND CAVIUM MAKES NO PROMISES, REPRESENTATIONS OR
# WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
# TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.


kernel_source := ../linux

ifdef OCTEON_LE
CROSS ?= mips64el-octeon-linux-gnu
else
CROSS ?= mips64-octeon-linux-gnu
endif

default: all
obj-m         :=  intercept-example.o
intercept-example-objs := intercept.o

intercept-example.o: $(intercept-example-objs)
	$(LD) -r $^ -o $@

all:
	$(MAKE) -C $(kernel_source) SUBDIRS=`pwd` modules ARCH=mips \
	    CROSS_COMPILE=${CROSS}-

clean:
	rm -f *.mod.c *.o *.ko

