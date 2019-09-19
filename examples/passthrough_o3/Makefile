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
# File version info: $Id: Makefile 88164 2013-09-11 17:38:25Z cchavva $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

MODEL=${OCTEON_MODEL}


# Default simulation parameters
packet_order:=no
INTF_TO_USE?="sgmii0"
NUM_PROCESSORS?=4

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter
PERF=
TRACE=
MEM?=640
MAX_CYCLES=500000000

default: all

######################*instrument for otrace: three examples*######################
#
# It's convenient sometimes to set
#    OCTEON_USE_LOCAL_LIB=1	# build the cvmx library locally
#
# 1. Embed static function entry/exit probes for all the functions
#    OCTEON_CFLAGS_GLOBAL_ADD += -finstrument-functions
#
# 2. Instrument everything except for those in the files with ``cvmx'' in their names
#    OCTEON_CFLAGS_GLOBAL_ADD += -finstrument-functions
#    OCTEON_CFLAGS_GLOBAL_ADD += -finstrument-functions-exclude-file-list=cvmx
#
# 3. Instrument everything except for those in the files under the executive directory
#    OCTEON_CFLAGS_GLOBAL_ADD += -finstrument-functions
#    OCTEON_CFLAGS_GLOBAL_ADD += -finstrument-functions-exclude-file-list=${OCTEON_ROOT}/executive
#

SEL_VZ_INTERVAL=-noperf=application_main_loop -wait=application_main_loop -quiet

ifneq ($(findstring OCTEON_CN78XX, $(MODEL)),)
    NUM_CORES=48
    QLM_LANE_MASK=#-qlm_is_disabled_mask=6  # to disable ilk
    QLM_LANE_MASK=#-qlm_is_ilk_mask=6       # to enable ilk
    INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
    DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
    DIFF_FILES1=diff input-2048.data output-n1-2048.data && diff input-2064.data output-n1-2064.data && diff input-2080.data output-n1-2080.data && diff input-2096.data output-n1-2096.data
    DATA_FILE=input-2048.data
    GMX=${findstring sgmii0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
        DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
	DIFF_FILES1=diff input-2048.data output-n1-2048.data && diff input-2064.data output-n1-2064.data && diff input-2080.data output-n1-2080.data && diff input-2096.data output-n1-2096.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data
        DIFF_FILES=diff input-2048.data output-2048.data
        DIFF_FILES1=diff input-2048.data output-n1-2048.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data -i input-2320.data -i input-2336.data -i input-2352.data
        DIFF_FILES=diff input-2304.data output-2304.data && diff input-2320.data output-2320.data && diff input-2336.data output-2336.data && diff input-2352.data output-2352.data
        DIFF_FILES1=diff input-2304.data output-n1-2304.data && diff input-2320.data output-n1-2320.data && diff input-2336.data output-n1-2336.data && diff input-2352.data output-n1-2352.data
        DATA_FILE=input-2304.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data
        DIFF_FILES=diff input-2304.data output-2304.data
	DIFF_FILES1=diff input-2304.data output-n1-2304.data
        DATA_FILE=input-2304.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data -i input-2576.data -i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2560.data output-2560.data && diff input-2576.data output-2576.data && diff input-2592.data output-2592.data && diff input-2608.data output-2608.data
        DIFF_FILES1=diff input-2560.data output-n1-2560.data && diff input-2576.data output-n1-2576.data && diff input-2592.data output-n1-2592.data && diff input-2608.data output-n1-2608.data
        DATA_FILE=input-2560.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data
        DIFF_FILES=diff input-2560.data output-2560.data
        DIFF_FILES1=diff input-2560.data output-n1-2560.data
        DATA_FILE=input-2560.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2816.data -i input-2832.data -i input-2848.data -i input-2864.data
        DIFF_FILES=diff input-2816.data output-2816.data && diff input-2832.data output-2832.data && diff input-2848.data output-2848.data && diff input-2864.data output-2864.data
        DIFF_FILES=diff input-2816.data output-n1-2816.data && diff input-2832.data output-n1-2832.data && diff input-2848.data output-n1-2848.data && diff input-2864.data output-n1-2864.data
        DATA_FILE=input-2816.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2816.data
        DIFF_FILES=diff input-2816.data output-2816.data
        DIFF_FILES1=diff input-2816.data output-n1-2816.data
        DATA_FILE=input-2816.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3072.data -i input-3088.data -i input-3104.data -i input-3120.data
        DIFF_FILES=diff input-3072.data output-3072.data && diff input-3088.data output-3088.data && diff input-3104.data output-3104.data && diff input-3120.data output-3120.data
        DIFF_FILES1=diff input-3072.data output-n1-3072.data && diff input-3088.data output-n1-3088.data && diff input-3104.data output-n1-3104.data && diff input-3120.data output-n1-3120.data
        DATA_FILE=input-3072.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3072.data
        DIFF_FILES=diff input-3072.data output-3072.data
	DIFF_FILES1=diff input-3072.data output-n1-3072.data
        DATA_FILE=input-3072.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii5, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3328.data -i input-3344.data -i input-3360.data -i input-3376.data
        DIFF_FILES=diff input-3328.data output-3328.data && diff input-3344.data output-3344.data && diff input-3360.data output-3360.data && diff input-3376.data output-3376.data
        DIFF_FILES1=diff input-3328.data output-n1-3328.data && diff input-3344.data output-n1-3344.data && diff input-3360.data output-n1-3360.data && diff input-3376.data output-n1-3376.data
        DATA_FILE=input-3328.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui5, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3328.data
        DIFF_FILES=diff input-3328.data output-3328.data
	DIFF_FILES1=diff input-3328.data output-n1-3328.data
        DATA_FILE=input-3328.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring ilk0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1024.data -i input-1025.data \
		-i input-1026.data -i input-1027.data \
		-i input-1028.data -i input-1029.data \
		-i input-1030.data -i input-1031.data
        DIFF_FILES=diff input-1024.data output-1024.data && \
		diff input-1025.data output-1025.data && \
		diff input-1026.data output-1026.data && \
		diff input-1027.data output-1027.data && \
		diff input-1028.data output-1028.data && \
		diff input-1029.data output-1029.data && \
		diff input-1030.data output-1030.data && \
		diff input-1031.data output-1031.data
        DIFF_FILES1=diff input-1024.data output-n1-1024.data && \
		diff input-1025.data output-n1-1025.data && \
		diff input-1026.data output-n1-1026.data && \
		diff input-1027.data output-n1-1027.data && \
		diff input-1028.data output-n1-1028.data && \
		diff input-1029.data output-n1-1029.data && \
		diff input-1030.data output-n1-1030.data && \
		diff input-1031.data output-n1-1031.data
        DATA_FILE=input-1024.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring ilk1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1280.data -i input-1281.data \
		-i input-1282.data -i input-1283.data
        DIFF_FILES=diff input-1280.data output-1280.data && \
        	diff input-1281.data output-1281.data && \
        	diff input-1282.data output-1282.data && \
        	diff input-1283.data output-1283.data
        DIFF_FILES1=diff input-1280.data output-n1-1280.data && \
        	diff input-1281.data output-n1-1281.data && \
        	diff input-1282.data output-n1-1282.data && \
        	diff input-1283.data output-n1-1283.data
        DATA_FILE=input-1280.data
        QLM_LANE_MASK=#-qlm_is_ilk_mask=6 # not supported in sim
    endif
    GMX=${findstring npi, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-256.data -i input-257.data -i input-258.data  -i input-259.data
        DIFF_FILES=diff input-256.data output-256.data && diff input-257.data output-257.data && diff input-258.data output-258.data && diff input-259.data output-259.data
	DIFF_FILES1=diff input-256.data output-n1-256.data && diff input-257.data output-n1-257.data && diff input-258.data output-n1-258.data && diff input-259.data output-n1-259.data
        DATA_FILE=input-256.data
        QLM_LANE_MASK=#-qlm_is_sgmii_mask=0x1 # not supported in sim
    endif
else ifneq ($(findstring OCTEON_CN73XX, $(MODEL)),)
    NUM_CORES=16
    QLM_LANE_MASK=#-qlm_is_disabled_mask=6  # to disable ilk
    QLM_LANE_MASK=#-qlm_is_ilk_mask=6       # to enable ilk
    ifeq ($(GMX),)
	INTF_TO_USE=sgmii0
    endif

    GMX=${findstring sgmii0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data -i input-2064.data \
	    -i input-2080.data -i input-2096.data
        DIFF_FILES=diff input-2048.data output-2048.data && \
	    diff input-2064.data output-2064.data && \
	    diff input-2080.data output-2080.data && \
	    diff input-2096.data output-2096.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data
        DIFF_FILES=diff input-2304.data output-2304.data
        DIFF_FILES1=diff input-2304.data output-n1-2304.data
        DATA_FILE=input-2304.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring sgmii2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data -i input-2576.data
        DIFF_FILES=diff input-2560.data output-2560.data && \
	diff input-2576.data output-2576.data
        DATA_FILE=input-2560.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xfi3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2592.data output-2592.data && \
	diff input-2608.data output-2608.data
        DATA_FILE=input-2592.data
        QLM_LANE_MASK=
    endif
    ifeq ($(DATA_FILE),)
	@echo $(INTF_TO_USE) is not supported for $(MODEL)
    endif
else ifneq ($(findstring OCTEON_CN76XX, $(MODEL)),)
    NUM_CORES=16
    QLM_LANE_MASK=#-qlm_is_disabled_mask=6  # to disable ilk
    QLM_LANE_MASK=#-qlm_is_ilk_mask=6       # to enable ilk

    ifeq (${INTF_TO_USE},)
        INTF_TO_USE=sgmii0
    endif

    GMX=${findstring sgmii0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data -i input-2064.data \
	 -i input-2080.data -i input-2096.data
        DIFF_FILES=diff input-2048.data output-2048.data && \
	diff input-2064.data output-2064.data && \
	diff input-2080.data output-2080.data && \
	diff input-2096.data output-2096.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data
        DIFF_FILES=diff input-2304.data output-2304.data
        DATA_FILE=input-2304.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xfi2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        DATA_FILE=input-2560.data
        INPUT_FILES=-i input-2560.data -i input-2576.data \
	-i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2560.data output-2560.data && \
	diff input-2576.data output-2576.data && \
	diff input-2592.data output-2592.data && \
	diff input-2608.data output-2608.data
        QLM_LANE_MASK=
    endif
else ifneq ($(findstring OCTEON_CNF75XX, $(MODEL)),)
    NUM_CORES=16
    QLM_LANE_MASK=#-qlm_is_disabled_mask=6  # to disable ilk
    QLM_LANE_MASK=#-qlm_is_ilk_mask=6       # to enable ilk

    ifeq (${INTF_TO_USE},)
        INTF_TO_USE=sgmii0
    endif

    GMX=${findstring sgmii0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
        DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data
        DIFF_FILES=diff input-2048.data output-2048.data
        DIFF_FILES1=diff input-2048.data output-n1-2048.data
        DATA_FILE=input-2048.data
        QLM_LANE_MASK=
    endif
    GMX=${findstring srio, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-576.data -i input-578.data 
        DIFF_FILES=diff input-576.data output-576.data && \
		diff input-578.data output-578.data
        DATA_FILE=input-576.data
        QLM_LANE_MASK=
    endif
endif # MODEL

ifndef OCTEON_SIM_PACKET_TCP_PORT
OCTEON_SIM_PACKET_TCP_PORT=2010
endif
ifndef OCTEON_SIM_PACKET_TCP_PORT1
OCTEON_SIM_PACKET_TCP_PORT1=2011
endif

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification
ifeq ($(packet_order),yes)
	CFLAGS_LOCAL += -DPACKET_ORDERING
	STIMULUS = diverse
else
	STIMULUS = identical
endif


ifeq ($(hotplug),yes)
	TARGET = passthrough_hotplug$(PREFIX)
else
	TARGET = passthrough$(PREFIX)
endif

ifeq ($(hotplug),yes)
	CFLAGS_LOCAL += -DENABLE_PASSTHROUGH_HOTPLUG
	OBJS = $(OBJ_DIR)/passthrough_hotplug.o
$(OBJ_DIR)/passthrough_hotplug.o: passthrough.c
	${COMPILE}
else
	OBJS = $(OBJ_DIR)/passthrough.o
endif

OBJS +=  $(OBJ_DIR)/passthrough-pktio.o

PASS_MADE_WITH = $(shell echo "$(OBJ_DIR)/pass_made_with-OCTEON_MODEL=$(CFLAGS_LOCAL)=." | sed 's/\ /,/g')
PASS_MADE_WITH_ALL = $(OBJ_DIR)/pass_made_with-*

$(PASS_MADE_WITH):
	mkdir -p $(OBJ_DIR)
	rm -rf $(PASS_MADE_WITH_ALL)
	touch "$(PASS_MADE_WITH)"

$(OBJS) : $(PASS_MADE_WITH)
$(DATA_FILE) : $(PASS_MADE_WITH)

DEPS = $(OBJS:%.o=%.d)

include $(OCTEON_ROOT)/application.mk

# Disable most rules for unsupported models
ifndef DATA_FILE
all: dummy
run: dummy
run-n1: dummy
run-n2: dummy

dummy:
	@echo Unsupported model $(MODEL)
else
all: $(TARGET) $(DATA_FILE)
endif
$(DATA_FILE): build_data.sh
	./build_data.sh ${INTF_TO_USE} $(STIMULUS)

clean:
	rm -f $(TARGET) output.log input-*.data output-*.data run-all.log
	rm -f $(CLEAN_LIST) $(OBJS) $(DEPS)
	rm -f $(CVMX_CONFIG)

clobber: clean
	rm -rf $(OBJ_DIR)

run: $(TARGET) $(DATA_FILE)
	#
	# Start oct-packet-io to pipe packet in and out of the simulator. It
	# will terminate automatically when the simulator terminates. Input
	# is taken from input-[0-3].data. Output is logged to output-[0-3].data.
	# The option "-c 1" is used to force the output cycle count to match
	# the input. This allows a simple diff to look for errors.
	#
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	#
	# Run the simulation. All outout will go in output.log but console
	# messages will also display to the user.
	#
	oct-sim $(TARGET) $(PERF) -memsize=$(MEM) -quiet -serve=${OCTEON_SIM_PACKET_TCP_PORT} -maxcycles=$(MAX_CYCLES) -numcores=${NUM_PROCESSORS} ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK} $(TRACE)
	#
	# Diff the output with the original input. They should match
	#
	@if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED"; \
	else \
		echo "Diffing packets - TEST FAILED" && false; \
	fi

run-n1: $(TARGET) $(DATA_FILE)
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	echo "bootcmd=bootoct 0x10000000 forceboot numcores=0,${NUM_PROCESSORS}" > /tmp/env2
	oct-sim $(TARGET) $(PERF) -memsize=$(MEM),$(MEM) -quiet -serve=1:${OCTEON_SIM_PACKET_TCP_PORT} -maxcycles=$(MAX_CYCLES) -numcores=${NUM_PROCESSORS} ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK} -numnodes=2 -envfile=/tmp/env2 $(TRACE)
	if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED"; \
	else \
		echo "Diffing packets - TEST FAILED" && false; \
	fi

# run passthrough on 2 nodes, one load-set per node:
run-n2: $(TARGET) $(DATA_FILE)
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT1} -c 1 -o output-n1-%d.data ${INPUT_FILES} &
	echo "bootcmd=bootoct 0x11000000 numcores=0,${NUM_PROCESSORS} nodemask=0x2; bootoct 0x10000000 numcores=${NUM_PROCESSORS},0 nodemask=0x1" > /tmp/env-multi-78
	oct-sim $(TARGET) -ld=0x11000000:$(TARGET) -quiet -noperf -serve=${OCTEON_SIM_PACKET_TCP_PORT} -serve=1:${OCTEON_SIM_PACKET_TCP_PORT1} -maxcycles=$(MAX_CYCLES) -numcores=${NUM_PROCESSORS},${NUM_PROCESSORS} -numnodes=2 -envfile=/tmp/env-multi-78 -memsize=$(MEM),$(MEM) ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK} $(TRACE)
	if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED node0"; \
	else \
		echo "Diffing packets - TEST FAILED node0" && false; \
	fi
	if ${DIFF_FILES1}; \
	then \
		echo "Diffing packets - TEST PASSED node1"; \
	else \
		echo "Diffing packets - TEST FAILED node1" && false; \
	fi

ENVFILE=/tmp/env-s2

# run passthrough on 2 nodes, one load-set for both nodes
run-multi-node: $(TARGET) $(DATA_FILE)
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT1} -c 1 -o output-n1-%d.data ${INPUT_FILES} &
	echo "bootcmd=bootoct 0x10000000 numcores=${NUM_PROCESSORS},${NUM_PROCESSORS} nodemask=0x3" > $(ENVFILE)
	oct-sim $(TARGET) -ld=0x11000000:$(TARGET) -quiet=60000000 -noperf -serve=${OCTEON_SIM_PACKET_TCP_PORT} -serve=1:${OCTEON_SIM_PACKET_TCP_PORT1} -maxcycles=$(MAX_CYCLES) -numcores=${NUM_PROCESSORS},${NUM_PROCESSORS} -numnodes=2 -envfile=$(ENVFILE) -memsize=$(MEM),$(MEM) ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK} $(TRACE)
	if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED node0"; \
	else \
		echo "Diffing packets - TEST FAILED node0" && false; \
	fi
	if ${DIFF_FILES1}; \
	then \
		echo "Diffing packets - TEST PASSED node1"; \
	else \
		echo "Diffing packets - TEST FAILED node1" && false; \
	fi
	rm -f $(ENVFILE)

run_vz: $(TARGET) $(DATA_FILE)
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	oct-sim $(TARGET) -vz ${SEL_VZ_INTERVAL} -serve=${OCTEON_SIM_PACKET_TCP_PORT} -maxcycles=$(MAX_CYCLES) -numcores=${NUM_PROCESSORS} ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK}
	if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED"; \
	else \
		echo "Diffing packets - TEST FAILED" && false; \
	fi

run-linux: $(DATA_FILE)
	# Please see README-Linux.txt for addition steps needed to
	# run this example under Linux
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	cd $(OCTEON_ROOT)/linux/kernel; ./oct-linux -quiet -numcores=$(NUM_PROCESSORS)


run-all:
	for ((cores=1; cores<=$(NUM_CORES); cores++)); do \
		make -s run NUM_PROCESSORS=$${cores}; \
	done | tee run-all.log | grep "cycles/packet"


.PHONY: flash
flash: ${TARGET}
	mount /mnt/cf1
	mipsisa64-octeon-elf-strip -o /mnt/cf1/${TARGET} ${TARGET}
	umount /mnt/cf1

