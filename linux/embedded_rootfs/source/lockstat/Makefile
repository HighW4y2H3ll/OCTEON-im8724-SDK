CROSS ?= mips64-octeon-linux-gnu
CXX = $(CROSS)-g++
CC = $(CROSS)-gcc
CFLAGS = -g -O2
HEADERS = wrappers.h cond.h mutex.h spin.h stat-time.h stats.h utils.h

all: liblockstat.so test.out

# Used by the embedded rootfs makefile.
install:
	cp -f liblockstat.so $(LIBDIR)
	cp -f test.out $(BINDIR)/lockstat-test

liblockstat.so: lockstat.cc $(HEADERS)
	$(CXX) $(CFLAGS) -fPIC -shared -o $@ $< -ldl

test: test.out liblockstat.so
	LD_PRELOAD=./liblockstat.so ./$<

test.out: test.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

clean:
	rm -rf liblockstat.so test

.PHONY: test
