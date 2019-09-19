ifndef OCTEON_ROOT
OCTEON_ROOT = ../../../..
endif

CC = mips64-octeon-linux-gnu-gcc

TARGET = board_mgmt
TARGET2 = sfp_info

OCTEON_TARGET=linux_64

# standard common Makefile fragment
include $(OCTEON_ROOT)/common.mk

all: $(TARGET) $(TARGET2)

# include relevant component Makefile fragments
dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

CFLAGS = -Wall -W -O2 -g -fvisibility=hidden $(CPPFLAGS_GLOBAL)

$(TARGET): board_mgmt.o im8724_phy.o mdio_octeon.o board_common.o
$(TARGET2): sfp_info.o board_common.o

clean:
	$(RM) board_mgmt sfp_info *.o *~
