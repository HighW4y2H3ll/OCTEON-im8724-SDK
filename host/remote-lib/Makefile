
BOOTLOADER_HEADERS = -I ../../bootloader/u-boot/include/asm/arch
BOOTLOADER_DEFINES =

CFLAGS = -g -O3 -W -Wall -D_GNU_SOURCE -DUSE_RUNTIME_MODEL_CHECKS=1 -DCVMX_BUILD_FOR_LINUX_HOST=1 \
	-I $(OCTEON_ROOT)/target/include \
	$(BOOTLOADER_HEADERS) $(EXTRA_CFLAGS) $(BOOTLOADER_DEFINES)

DIR=.
CROSS=
CC=$(CROSS)gcc
AR=$(CROSS)ar
GCC_GTEQ_472 := $(shell expr `${CC} -dumpversion | sed -e 's/\.\([0-9][0-9]\)/\1/g' -e 's/\.\([0-9]\)/0\1/g' -e 's/^[0-9]\{3,4\}$$/&00/'` \>= 40702)

.PHONY: all
all: $(DIR)/libocteon-remote.a

OBJ = $(DIR)/octeon-remote.o \
	$(DIR)/octeon-remote-map.o \
	$(DIR)/octeon-remote-console.o \
	$(DIR)/octeon-remote-cvmx.o \
	$(DIR)/octeon-remote-debug-handler.o \
	$(DIR)/octeon-remote-gdbremote.o \
	$(DIR)/octeon-remote-pci.o \
	$(DIR)/octeon-remote-linux.o \
	$(DIR)/octeon-remote-mips-asm.o \
	$(DIR)/cvmx-bootmem.o \
	$(DIR)/octeon-feature.o

ifeq "$(GCC_GTEQ_472)" "1"
OBJ += $(DIR)/octeon-remote-hotplug.o
endif

ifdef CAVIUM_ONLY
# Macraigor code was written under NDA and can't be distributed.
CFLAGS += -DCAVIUM_ONLY=1
OBJ += $(DIR)/octeon-remote-macraigor.o
endif

$(DIR)/libocteon-remote.a: $(OBJ)
	$(AR) -cr $@ $^

$(DIR)/octeon-remote.o: octeon-remote.c octeon-remote.h
$(DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR)/cvmx-bootmem.o: ${OCTEON_ROOT}/executive/cvmx-bootmem.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR)/octeon-feature.o: ${OCTEON_ROOT}/executive/octeon-feature.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR)/octeon-remote-mips-asm.o: $(DIR)/mips-octeon_debug_handler.bin $(DIR)/mips-octeon2_debug_handler.bin $(DIR)/mips-octeon3_debug_handler.bin $(DIR)/mips-octeon_profile_handler.bin
	$(CC) $(CFLAGS) -c -o $@ octeon-remote-mips-asm.S -I$(DIR)

$(DIR)/mips-%.o: mips-%.S
	mips64-octeon-linux-gnu-gcc -W -Wall -o $@ -c $<
$(DIR)/mips-%.bin: $(DIR)/mips-%.o
	mips64-octeon-linux-gnu-objcopy -j .text -O binary $< $@

.PHONY: clean
clean:
	rm -f $(DIR)/*.o $(DIR)/*.a $(DIR)/*.bin

