#This does not include the bootloader headers as they conflict with the glibc's elf.h header.
CFLAGS = -g -O3 -W -Wall -Werror \
	-D_GNU_SOURCE -DUSE_RUNTIME_MODEL_CHECKS=1 -DCVMX_BUILD_FOR_LINUX_HOST=1 \
	-D_FILE_OFFSET_BITS=64 \
	-U_FORTIFY_SOURCE \
	-I ../remote-lib \
	-I $(OCTEON_ROOT)/target/include \
	$(EXTRA_CFLAGS)

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

CROSS=
CC=$(CROSS)gcc
CXX=$(CROSS)g++
STRIP=$(CROSS)strip

BINS=$(DIR)/oct-remote-coredump

all: $(BINS) $(LIBS)
	- $(STRIP) $(BINS)
	- sudo chown -f root $(BINS)
	- sudo chmod -f +s,g+r,o+r $(BINS)

$(DIR)/oct-remote-coredump: oct-remote-coredump.c oct-remote-coredump-elf.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)

clean:
	- sudo rm -f $(BINS)
	- rm -f $(BINS)
