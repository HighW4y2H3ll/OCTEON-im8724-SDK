#***********************license start************************************
# OCTEON SDK
#
# Copyright (c) 2009-2010 Cavium Inc. All rights reserved.
#
# This file, which is part of the OCTEON SDK from Cavium Inc. 
# contains proprietary and confidential information of Cavium Inc. and
# its suppliers.
#
# Any licensed reproduction, distribution, modification, or other use of
# this file or the confidential information or patented inventions
# embodied in this file is subject to your license agreement with Cavium
# Networks. Unless you and Cavium Inc. have agreed otherwise in
# writing, the applicable license terms can be found at:
# licenses/cavium-license-type2.txt
#
# All other use and disclosure is prohibited.
#
# Contact Cavium Inc. at info@cavium.com for more information.
#*********************license end**************************************

EXTRA_CFLAGS = -W -Wall -Wno-unused-parameter -O3 -g -idirafter ../../../kernel/linux/include
OUTPUT_DIR=.

ifneq ($(findstring mips, $(CC)),)
	EXTRA_CFLAGS += -DUSE_PTP_CLOCK=1
else
	EXTRA_CFLAGS += -DUSE_PTP_CLOCK=0
endif

objs  = ${OUTPUT_DIR}/timestamp.o
objs += ${OUTPUT_DIR}/packetio.o
objs += ${OUTPUT_DIR}/packetio-raw.o
objs += ${OUTPUT_DIR}/packetio-udp.o
objs += ${OUTPUT_DIR}/packetio-udp6.o
objs += ${OUTPUT_DIR}/ptplib.o

all: ${OUTPUT_DIR}/ptp-1588v2 ${OUTPUT_DIR}/client-1588v2

${OUTPUT_DIR}/%.o: %.c
	${CC} ${CFLAGS} ${EXTRA_CFLAGS} -c -o $@ $^

${OUTPUT_DIR}/ptp-1588v2: ${OUTPUT_DIR}/main.o ${objs}
	${CC} ${CFLAGS} -o $@ $^ -lrt

${OUTPUT_DIR}/client-1588v2: ${OUTPUT_DIR}/client-main.o ${objs}
	${CC} ${CFLAGS} -o $@ $^ -lrt

clean:
	rm -f ${OUTPUT_DIR}/ptp-1588v2 ${OUTPUT_DIR}/client-1588v2 ${OUTPUT_DIR}/client-main.o ${OUTPUT_DIR}/main.o ${objs}
