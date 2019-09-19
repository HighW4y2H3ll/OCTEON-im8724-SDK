CFLAGS = -g -O3 -W -Wall -Wno-unused-parameter \
	-D_GNU_SOURCE -DUSE_RUNTIME_MODEL_CHECKS=1 -DCVMX_BUILD_FOR_LINUX_HOST=1 \
	-I ../remote-lib \
	-I $(OCTEON_ROOT)/target/include \
	-I ../../bootloader/u-boot/include $(EXTRA_CFLAGS)

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

BINS=$(DIR)/oct-debug-agent

all: $(BINS) $(LIBS)
	- $(STRIP) $(BINS)
	- sudo chown -f root $(BINS)
	- sudo chmod -f +s,g+r,o+r $(BINS)

$(DIR)/oct-debug-agent: agent.c proxy.c remote.c $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LIBS_FLAGS)

clean:
	- sudo rm -f $(DIR)/oct-debug-agent
	- rm -f $(DIR)/oct-debug-agent
