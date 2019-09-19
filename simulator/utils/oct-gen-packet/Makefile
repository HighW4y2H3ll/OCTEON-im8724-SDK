
ifndef OCTEON_ROOT
OCTEON_ROOT = ../../..
endif

CC = gcc
CFLAGS = -Wall -W -Wno-unused

DEB_VERSION = $(shell ls /etc/debian_version 2>&1)
ifneq (${DEB_VERSION}, /etc/debian_version)
CFLAGS += -static
endif

all: oct-gen-packet

OBJ = oct-gen-packet.o crc32.o

oct-gen-packet: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lpthread

clean:
	rm -f oct-gen-packet oct-gen-packet.o crc32.o

