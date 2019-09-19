######################*license start*###################################
#   Copyright (c) 2003-2015 Cavium Inc. (support@cavium.com). All rights
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
# File version info: $Id: Makefile 153709 2017-02-06 00:06:50Z cchavva $


ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

MODEL=${OCTEON_MODEL}


# Default simulation parameters
INTF_TO_USE?="sgmii0"
NUM_PROCESSORS=4
INPUT_FILES=-i input-0.data -i input-1.data -i input-2.data -i input-3.data
DIFF_FILES=diff input-0.data output-0.data && diff input-1.data output-1.data && diff input-2.data output-2.data && diff input-3.data output-3.data
DATA_FILE=input-0.data
NUM_CORES=16

CFLAGS_LOCAL = -g -O2 -W -Wall -Wno-unused-parameter

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

TMP=${findstring npi, ${INTF_TO_USE}}
ifneq ($(TMP),)
    INPUT_FILES=-i input-32.data -i input-33.data -i input-34.data  -i input-35.data
    DIFF_FILES=diff input-32.data output-32.data && diff input-33.data output-33.data && diff input-34.data output-34.data && diff input-35.data output-35.data
    DATA_FILE=input-32.data
endif

TMP=${findstring OCTEON_CN3020,${MODEL}}
ifeq ($(TMP),)
TMP=${findstring OCTEON_CN31XX,${MODEL}}
endif
ifeq ($(TMP),)
TMP=${findstring OCTEON_CN50XX,${MODEL}}
endif
ifneq ($(TMP),)
    NUM_PROCESSORS=2
    INPUT_FILES=-i input-0.data -i input-1.data -i input-2.data
    DIFF_FILES=diff input-0.data output-0.data && diff input-1.data output-1.data && diff input-2.data output-2.data
    NUM_CORES=$(NUM_PROCESSORS)
endif

TMP=${findstring OCTEON_CN3010,${MODEL}}
ifneq ($(TMP),)
    NUM_PROCESSORS=1
    INPUT_FILES=-i input-0.data -i input-1.data -i input-2.data
    DIFF_FILES=diff input-0.data output-0.data && diff input-1.data output-1.data && diff input-2.data output-2.data
    NUM_CORES=$(NUM_PROCESSORS)
endif

TMP=${findstring OCTEON_CN3005,${MODEL}}
ifneq ($(TMP),)
    NUM_PROCESSORS=1
    INPUT_FILES=-i input-0.data -i input-1.data
    DIFF_FILES=diff input-0.data output-0.data && diff input-1.data output-1.data
    NUM_CORES=$(NUM_PROCESSORS)
endif
TMP=${findstring OCTEON_CN52XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=4
endif
TMP=${findstring OCTEON_CN56XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=12
endif
TMP=${findstring OCTEON_CN63XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=6
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-0.data
        DIFF_FILES=diff input-0.data output-0.data
        DATA_FILE=input-0.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=4
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
endif

TMP=${findstring OCTEON_CN66XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=10
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-16.data -i input-17.data -i input-18.data -i input-19.data
        DIFF_FILES=diff input-16.data output-16.data && diff input-17.data output-17.data && diff input-18.data output-18.data && diff input-19.data output-19.data
        QLM_LANE_MASK=-qlm_is_sgmii_mask=2
        DATA_FILE=input-16.data
    endif
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-0.data
        DIFF_FILES=diff input-0.data output-0.data
        DATA_FILE=input-0.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=4
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-16.data
        DIFF_FILES=diff input-16.data output-16.data
        DATA_FILE=input-16.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=2
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
endif

TMP=${findstring OCTEON_CN61XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=4
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-16.data -i input-17.data -i input-18.data -i input-19.data
        DIFF_FILES=diff input-16.data output-16.data && diff input-17.data output-17.data && diff input-18.data output-18.data && diff input-19.data output-19.data
        QLM_LANE_MASK=-qlm_is_sgmii_mask=1
        DATA_FILE=input-16.data
    endif
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-0.data
        DIFF_FILES=diff input-0.data output-0.data
        DATA_FILE=input-0.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=4
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-16.data
        DIFF_FILES=diff input-16.data output-16.data
        DATA_FILE=input-16.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=1
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
endif

TMP=${findstring OCTEON_CN70XX,${MODEL}}
ifeq ($(TMP),)
TMP=${findstring OCTEON_CN71XX,${MODEL}}
endif

ifneq ($(TMP),)
    NUM_CORES=4
    INPUT_FILES=-i input-0.data
    DIFF_FILES=diff input-0.data output-0.data
    DATA_FILE=input-0.data
    CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-16.data
        DIFF_FILES=diff input-16.data output-16.data
        DATA_FILE=input-16.data
    endif
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-0.data
        DIFF_FILES=diff input-0.data output-0.data
        DATA_FILE=input-16.data
    endif
    GMX=${findstring agl, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-24.data
        DIFF_FILES=diff input-24.data output-24.data
        DATA_FILE=input-24.data
	NUM_PROCESSORS=1
    endif
endif

TMP=${findstring OCTEON_CN68XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=32
    QLM_LANE_MASK=#-qlm_is_disabled_mask=6  # to disable ilk
    QLM_LANE_MASK=#-qlm_is_ilk_mask=6       # to enable ilk
    INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
    DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
    DATA_FILE=input-2048.data
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2112.data
        DIFF_FILES=diff input-2112.data output-2112.data
        DATA_FILE=input-2112.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=1
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data -i input-2576.data -i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2560.data output-2560.data && diff input-2576.data output-2576.data && diff input-2592.data output-2592.data && diff input-2608.data output-2608.data
        DATA_FILE=input-2560.data
    endif
    GMX=${findstring xaui2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2624.data
        DIFF_FILES=diff input-2624.data output-2624.data
        DATA_FILE=input-2624.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=4
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2816.data -i input-2832.data -i input-2848.data -i input-2864.data
        DIFF_FILES=diff input-2816.data output-2816.data && diff input-2832.data output-2832.data && diff input-2848.data output-2848.data && diff input-2864.data output-2864.data
        DATA_FILE=input-2816.data
    endif
    GMX=${findstring xaui3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2880.data
        DIFF_FILES=diff input-2880.data output-2880.data
        DATA_FILE=input-2880.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=8
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3072.data -i input-3088.data -i input-3104.data -i input-3120.data
        DIFF_FILES=diff input-3072.data output-3072.data && diff input-3088.data output-3088.data && diff input-3104.data output-3104.data && diff input-3120.data output-3120.data
        DATA_FILE=input-3072.data
    endif
    GMX=${findstring xaui4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3136.data
        DIFF_FILES=diff input-3136.data output-3136.data
        DATA_FILE=input-3136.data
        QLM_LANE_MASK=-qlm_is_xaui_mask=0x10
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring ilk0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1024.data
        DIFF_FILES=diff input-1024.data output-1024.data
        DATA_FILE=input-1024.data
        QLM_LANE_MASK=-qlm_is_ilk_mask=6
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring ilk1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1280.data
        DIFF_FILES=diff input-1280.data output-1280.data
        DATA_FILE=input-1280.data
        QLM_LANE_MASK=-qlm_is_ilk_mask=6
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring npi, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-256.data -i input-257.data -i input-258.data  -i input-259.data
        DIFF_FILES=diff input-256.data output-256.data && diff input-257.data output-257.data && diff input-258.data output-258.data && diff input-259.data output-259.data
        DATA_FILE=input-256.data
        QLM_LANE_MASK=-qlm_is_sgmii_mask=0x1
    endif
endif

TMP=${findstring OCTEON_CNF71XX,${MODEL}}
ifneq ($(TMP),)
    INPUT_FILES=-i input-0.data -i input-1.data
    DIFF_FILES=diff input-0.data output-0.data && diff input-1.data output-1.data
endif

# cn76xx is a subset of cn78xx
USE_CN78XX=0
ifneq ($(findstring OCTEON_CN78XX, $(MODEL)),)
  USE_CN78XX=1
else
  ifneq ($(findstring OCTEON_CN76XX, $(MODEL)),)
    USE_CN78XX=1
  endif
endif

ifeq ($(USE_CN78XX),1)
    NUM_CORES=48
    QLM_LANE_MASK=
    INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
    DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
    DATA_FILE=input-2048.data
    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data
        DIFF_FILES=diff input-2048.data output-2048.data
        DATA_FILE=input-2048.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data -i input-2320.data -i input-2336.data -i input-2352.data
        DIFF_FILES=diff input-2304.data output-2304.data && diff input-2320.data output-2320.data && diff input-2336.data output-2336.data && diff input-2352.data output-2352.data
        DATA_FILE=input-2304.data
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data
        DIFF_FILES=diff input-2304.data output-2304.data
        DATA_FILE=input-2304.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data -i input-2576.data -i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2560.data output-2560.data && diff input-2576.data output-2576.data && diff input-2592.data output-2592.data && diff input-2608.data output-2608.data
        DATA_FILE=input-2560.data
    endif
    GMX=${findstring xaui2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data
        DIFF_FILES=diff input-2560.data output-2560.data
        DATA_FILE=input-2560.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2816.data -i input-2832.data -i input-2848.data -i input-2864.data
        DIFF_FILES=diff input-2816.data output-2816.data && diff input-2832.data output-2832.data && diff input-2848.data output-2848.data && diff input-2864.data output-2864.data
        DATA_FILE=input-2816.data
    endif
    GMX=${findstring xaui3, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2816.data
        DIFF_FILES=diff input-2816.data output-2816.data
        DATA_FILE=input-2816.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3072.data -i input-3088.data -i input-3104.data -i input-3120.data
        DIFF_FILES=diff input-3072.data output-3072.data && diff input-3088.data output-3088.data && diff input-3104.data output-3104.data && diff input-3120.data output-3120.data
        DATA_FILE=input-3072.data
    endif
    GMX=${findstring xaui4, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3072.data
        DIFF_FILES=diff input-3072.data output-3072.data
        DATA_FILE=input-3072.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii5, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3328.data -i input-3344.data -i input-3360.data -i input-3376.data
        DIFF_FILES=diff input-3328.data output-3328.data && diff input-3344.data output-3344.data && diff input-3360.data output-3360.data && diff input-3376.data output-3376.data
        DATA_FILE=input-3328.data
    endif
    GMX=${findstring xaui5, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-3328.data
        DIFF_FILES=diff input-3328.data output-3328.data
        DATA_FILE=input-3328.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring ilk0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1024.data
        DIFF_FILES=diff input-1024.data output-1024.data
        DATA_FILE=input-1024.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring ilk1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-1280.data
        DIFF_FILES=diff input-1280.data output-1280.data
        DATA_FILE=input-1280.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring npi, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-256.data -i input-257.data -i input-258.data  -i input-259.data
        DIFF_FILES=diff input-256.data output-256.data && diff input-257.data output-257.data && diff input-258.data output-258.data && diff input-259.data output-259.data
        DATA_FILE=input-256.data
    endif
endif


# cn23xx is same as cn73xx
USE_CN73XX=0
ifneq ($(findstring OCTEON_CN73XX, $(MODEL)),)
    USE_CN73XX=1
else
    ifneq ($(findstring OCTEON_CN23XX, $(MODEL)),)
        USE_CN73XX=1
    endif
endif

ifeq ($(USE_CN73XX),1)
    NUM_CORES=16
    INPUT_FILES=-i input-2048.data -i input-2064.data -i input-2080.data -i input-2096.data
    DIFF_FILES=diff input-2048.data output-2048.data && diff input-2064.data output-2064.data && diff input-2080.data output-2080.data && diff input-2096.data output-2096.data
    DATA_FILE=input-2048.data

    GMX=${findstring xaui0, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2048.data
        DIFF_FILES=diff input-2048.data output-2048.data
        DATA_FILE=input-2048.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data -i input-2320.data -i input-2336.data -i input-2352.data
        DIFF_FILES=diff input-2304.data output-2304.data && diff input-2320.data output-2320.data && diff input-2336.data output-2336.data && diff input-2352.data output-2352.data
        DATA_FILE=input-2304.data
    endif
    GMX=${findstring xaui1, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2304.data
        DIFF_FILES=diff input-2304.data output-2304.data
        DATA_FILE=input-2304.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
    GMX=${findstring sgmii2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data -i input-2576.data -i input-2592.data -i input-2608.data
        DIFF_FILES=diff input-2560.data output-2560.data && diff input-2576.data output-2576.data && diff input-2592.data output-2592.data && diff input-2608.data output-2608.data
        DATA_FILE=input-2560.data
    endif
    GMX=${findstring xaui2, ${INTF_TO_USE}}
    ifneq ($(GMX),)
        INPUT_FILES=-i input-2560.data
        DIFF_FILES=diff input-2560.data output-2560.data
        DATA_FILE=input-2560.data
        CFLAGS_LOCAL += -DSINGLE_PORT_SIM
    endif
endif

TMP=${findstring OCTEON_CNF75XX,${MODEL}}
ifneq ($(TMP),)
    NUM_CORES=16
    ifeq ($(INTF_TO_USE),)
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
    endif
    ifeq ($(DATA_FILE),)
	@echo $(INTF_TO_USE) not supported on $(MODEL)
    endif
endif

ifndef OCTEON_SIM_PACKET_TCP_PORT
OCTEON_SIM_PACKET_TCP_PORT=2000
endif

all: application-target $(DATA_FILE)

#  standard common Makefile fragment

include $(OCTEON_ROOT)/common.mk

#  include relevant component Makefile fragments

dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification

ifeq ($(hotplug),yes)
	TARGET = passthrough_hotplug$(PREFIX)
else
	TARGET = passthrough$(PREFIX)
endif

OBJS = $(OBJ_DIR)/passthrough.o

PASS_MADE_WITH = $(shell echo "$(OBJ_DIR)/pass_made_with-OCTEON_MODEL=$(CFLAGS_LOCAL)=." | sed 's/\ /,/g')
PASS_MADE_WITH_ALL = $(OBJ_DIR)/pass_made_with-*

$(PASS_MADE_WITH):
	mkdir -p $(OBJ_DIR)
	rm -rf $(PASS_MADE_WITH_ALL)
	touch "$(PASS_MADE_WITH)"

$(OBJS) : $(PASS_MADE_WITH)

DEPS = $(OBJS:%.o=%.d)

ifeq ($(hotplug),yes)
	CFLAGS_LOCAL += -DENABLE_PASSTHROUGH_HOTPLUG
	OBJS = $(OBJ_DIR)/passthrough_hotplug.o
$(OBJ_DIR)/passthrough_hotplug.o: passthrough.c
	${COMPILE}
endif

include $(OCTEON_ROOT)/application.mk

$(DATA_FILE): build_data.sh
	./build_data.sh ${INTF_TO_USE}

clean:
	rm -f $(TARGET) output.log input-*.data output-*.data run-all.log
	rm -f $(CLEAN_LIST) $(OBJS) $(DEPS) $(PASS_MADE_WITH_ALL)
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
	oct-sim $(TARGET) $(PERF) -quiet -serve=${OCTEON_SIM_PACKET_TCP_PORT} -maxcycles=120000000 -numcores=${NUM_PROCESSORS} ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK}
	#
	# Diff the output with the original input. They should match
	#
	if ${DIFF_FILES}; \
	then \
		echo "Diffing packets - TEST PASSED"; \
	else \
		echo "Diffing packets - TEST FAILED" && false; \
	fi

run_vz: $(TARGET) $(DATA_FILE)
	oct-packet-io -w 5 -p ${OCTEON_SIM_PACKET_TCP_PORT} -c 1 -o output-%d.data ${INPUT_FILES} &
	oct-sim $(TARGET) -vz ${SEL_VZ_INTERVAL} -serve=${OCTEON_SIM_PACKET_TCP_PORT} -maxcycles=120000000 -numcores=${NUM_PROCESSORS} ${PASSTHROUGH_SIM_MODES} ${QLM_LANE_MASK}
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
