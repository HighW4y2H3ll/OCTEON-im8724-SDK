
BOOTLOADER_HEADERS = -I ../../bootloader/u-boot/include -I ../../bootloader/u-boot/arch/mips/include/asm/arch-octeon
BOOTLOADER_FILES = ../../bootloader/u-boot/arch/mips/cpu/octeon
BOOTLOADER_DEFINES =

CFLAGS = -g -O0 -W -Wall -Wno-unused-parameter \
	-D_GNU_SOURCE -DUSE_RUNTIME_MODEL_CHECKS=1 -DCVMX_BUILD_FOR_LINUX_HOST=1 \
	-I ../remote-lib \
	-I $(OCTEON_ROOT)/target/include \
	$(BOOTLOADER_HEADERS) $(EXTRA_CFLAGS) $(BOOTLOADER_DEFINES)


ifdef CAVIUM_ONLY
CFLAGS += -DCAVIUM_ONLY=1
endif

ifndef DIR
DIR=.
LIBS := ../remote-lib/libocteon-remote.a
else
LIBS := $(DIR)/libocteon-remote.a
endif
LIBS_CXX := ${LIBS}

ifndef CROSS
tmp = $(shell find /usr/lib* -name libnss_files.a)
ifneq (,$(findstring libnss,$(tmp)))
CFLAGS+= -static
LIBS_FLAGS += -lc -lnss_files -lnss_dns -lresolv
LIBS_CXX += -Wl,-lstdc++,-lc,-lnss_files,-lnss_dns,-lresolv
endif
endif

LIBELF := $(shell find /usr/lib* -name libelf.a)

CROSS=
CC=$(CROSS)gcc
CXX=$(CROSS)g++
STRIP=$(CROSS)strip

BINS = \
	$(DIR)/oct-remote-boot \
	$(DIR)/oct-remote-boot-cn70xx \
	$(DIR)/oct-remote-bootcmd \
	$(DIR)/oct-remote-console \
	$(DIR)/oct-remote-core \
	$(DIR)/oct-remote-csr \
	$(DIR)/oct-remote-ddr \
	$(DIR)/oct-remote-eeprom \
	$(DIR)/oct-remote-load \
	$(DIR)/oct-remote-memory \
	$(DIR)/oct-remote-ocla \
	$(DIR)/oct-remote-print-buf \
	$(DIR)/oct-remote-pow \
	$(DIR)/oct-remote-profile \
	$(DIR)/oct-remote-qlm \
	$(DIR)/oct-remote-reset \
	$(DIR)/oct-remote-save \
	$(DIR)/oct-remote-tra \
	$(DIR)/oct-remote-mmc

ifndef CROSS
GCC_GTEQ_472 := $(shell expr `${CC} -dumpversion | sed -e 's/\.\([0-9][0-9]\)/\1/g' -e 's/\.\([0-9]\)/0\1/g' -e 's/^[0-9]\{3,4\}$$/&00/'` \>= 40702)
ifeq "$(GCC_GTEQ_472)" "1"
BINS +=	$(DIR)/oct-remote-app-ctl
endif
endif

.PHONY: all
all: $(BINS)
#	- $(STRIP) $(BINS)
	- sudo chown -f root $(BINS)
ifndef OCTEON_LIQUIDIO
	- sudo chmod -f +s,g+r,o+r $(BINS)
endif

CVMX_COMMON_FILES = $(OCTEON_ROOT)/executive/cvmx-clock.c \
		 $(OCTEON_ROOT)/executive/cvmx-l2c.c \
		 $(OCTEON_ROOT)/executive/cvmx-twsi.c \
		 $(OCTEON_ROOT)/executive/cvmx-warn.c \
		 $(OCTEON_ROOT)/executive/octeon-model.c

CVMX_FILES = $(OCTEON_ROOT)/executive/cvmx-csr-db.c \
		 $(OCTEON_ROOT)/executive/cvmx-csr-db-support.c \
		 $(OCTEON_ROOT)/executive/cvmx-helper-jtag.c \
		 $(OCTEON_ROOT)/executive/cvmx-qlm.c \
		 $(OCTEON_ROOT)/executive/cvmx-qlm-tables.c \
		 $(OCTEON_ROOT)/executive/cvmx-srio.c \
		 $(CVMX_COMMON_FILES)

$(DIR)/oct-remote-boot: $(LIBS) oct-remote-boot.c crc32.c $(BOOTLOADER_FILES)/lib_octeon_shared.c $(CVMX_COMMON_FILES) oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-boot-cn70xx: $(LIBS) oct-remote-boot.c crc32.c $(BOOTLOADER_FILES)/lib_octeon_shared.c $(CVMX_COMMON_FILES) oct-remote-common.c
	$(CC) $(CFLAGS) -DCONFIG_OCTEON_CN70XX=1 -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-bootcmd: $(LIBS) oct-remote-bootcmd.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-console: $(LIBS) oct-remote-console.c
ifndef CROSS
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS) -lncurses
else
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
endif
$(DIR)/oct-remote-core: $(LIBS) oct-remote-core.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-csr: $(LIBS) oct-remote-csr.c oct-remote-common.c $(OCTEON_ROOT)/executive/cvmx-csr-db.c $(CVMX_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-ddr: $(LIBS) oct-remote-ddr.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-eeprom: $(LIBS) oct-remote-eeprom.c $(CVMX_COMMON_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-load: $(LIBS) oct-remote-load.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-memory: $(LIBS) oct-remote-memory.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-ocla: $(LIBS) oct-remote-ocla.c $(OCTEON_ROOT)/executive/cvmx-ocla.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-print-buf: $(LIBS) oct-remote-print-buf.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-pow: $(LIBS) oct-remote-pow.c $(OCTEON_ROOT)/executive/cvmx-pow.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-profile: $(LIBS) oct-remote-profile.cpp $(OCTEON_ROOT)/executive/cvmx-clock.c
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS_CXX)
$(DIR)/oct-remote-qlm: $(LIBS) oct-remote-qlm.c oct-remote-common.c $(CVMX_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-reset: $(LIBS) oct-remote-reset.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-save: $(LIBS) oct-remote-save.c oct-remote-common.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)
$(DIR)/oct-remote-tra: $(LIBS) oct-remote-tra.c $(OCTEON_ROOT)/executive/cvmx-tra.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)

$(DIR)/oct-remote-mmc: $(LIBS) oct-remote-mmc.c crc32.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIB_FLAGS)


CVMX_SRC = 	$(OCTEON_ROOT)/executive/cvmx-coremask.c \
		$(OCTEON_ROOT)/executive/octeon-feature.c \
		$(OCTEON_ROOT)/executive/cvmx-sysinfo.c \
		$(OCTEON_ROOT)/executive/cvmx-bootmem.c

$(DIR)/oct-remote-app-ctl: oct-remote-app-elf.o oct-remote-app-ctl.o oct-remote-app-tlb.o oct-remote-app-bloader.o $(LIBS)
	$(CC) $(CFLAGS) -include /usr/include/elf.h -include /usr/include/libelf.h -include /usr/include/errno.h -D__mips__ -o $@ $(CVMX_COMMON_FILES) $(CVMX_SRC) $^ $(LIB_FLAGS) $(LIBELF) -lz

oct-remote-app-elf.o: oct-remote-app-elf.c
	$(CC) $(CFLAGS) -include /usr/include/elf.h -include /usr/include/libelf.h -include /usr/include/errno.h -D__mips__ -c -o $@ $<

oct-remote-app-ctl.o: oct-remote-app-ctl.c
	$(CC) $(CFLAGS) -include /usr/include/elf.h -include /usr/include/libelf.h -include /usr/include/errno.h -D__mips__ -c -o $@ $<

oct-remote-app-tlb.o: oct-remote-app-tlb.c
	$(CC) $(CFLAGS) -include /usr/include/elf.h -include /usr/include/libelf.h -include /usr/include/errno.h -D__mips__ -c -o $@ $<

oct-remote-app-bloader.o: oct-remote-app-bloader.c
	$(CC) $(CFLAGS) -include /usr/include/elf.h -include /usr/include/libelf.h -include /usr/include/errno.h -D__mips__ -c -o $@ $<

$(LIBS):
	$(MAKE) -C ../remote-lib all

.PHONY: clean
clean:
	rm -f $(BINS) *.o
