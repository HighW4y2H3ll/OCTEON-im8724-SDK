ifndef OCTEON_ROOT
OCTEON_ROOT = ../..
endif

all: application-target

#  standard common Makefiles fragments
include $(OCTEON_ROOT)/common.mk
dir := $(OCTEON_ROOT)/executive
include $(dir)/cvmx.mk

#  application specification
TARGET = t8724-support$(PREFIX)

CFLAGS_LOCAL = -g -O3 -W -Wall -Wno-unused-parameter -lm
CFLAGS_LOCAL += -I${OCTEON_ROOT}/bootloader/u-boot/include/asm/arch
OBJS = $(OBJ_DIR)/fans_mgmt.o $(OBJ_DIR)/leds_mgmt.o $(OBJ_DIR)/octeon_i2c.o \
	$(OBJ_DIR)/imt_cpld.o $(OBJ_DIR)/t8724-support.o $(OBJ_DIR)/cortina_phys.o

include $(OCTEON_ROOT)/application.mk

clean:
	rm -f $(TARGET)
	rm -fr $(OBJ_DIR)
	rm -f $(CVMX_CONFIG)
	rm -f .cstamp*

distclean: clean
	rm -rf $(OBJ_DIR) $(TARGET)

