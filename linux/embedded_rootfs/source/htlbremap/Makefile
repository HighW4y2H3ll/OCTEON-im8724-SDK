#
# WARNING: This make file isn't the one used for building libhtlbremap.so
# and including it into the filesystem. This is in pkg_makefiles/htlbremap.mk.
# This makefile is purely for development testing.
#

CC ?= mips64-octeon-linux-gnu-gcc
OCTEON_EXTRA_CFLAGS ?= -march=octeon
CFLAGS = -Wall -W -Werror -O2 -g -fPIC -fvisibility=hidden $(OCTEON_EXTRA_CFLAGS)

OBJS = htlbremap.o switch-stack.o stack.o heap.o prog.o
OBJS_32 = $(OBJS:%=n32/%)
OBJS_64 = $(OBJS:%=64/%)
LIBS = libhtlbremap.so libhtlbremap-n32.so
INCLUDES = htlbremap.h

.PHONY: all
all: $(LIBS)

libhtlbremap.so: $(OBJS_64)
	$(CC) -shared -Wl,-Bsymbolic $(CFLAGS) -o $@ $^

libhtlbremap-n32.so: $(OBJS_32)
	$(CC) -shared -mabi=n32 -Wl,-Bsymbolic $(CFLAGS) -o $@ $^

64/%.o: %.c $(INCLUDES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -mabi=$(@D) -c -o $@ $<

n32/%.o: %.c $(INCLUDES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -mabi=$(@D) -c -o $@ $<

64/%.o: %.S $(INCLUDES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -mabi=$(@D) -c -o $@ $<

n32/%.o: %.S $(INCLUDES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -mabi=$(@D) -c -o $@ $<

clean:
	rm -f $(LIBS) $(OBJS_32) $(OBJS_64)
	$(MAKE) -C tests clean

# Run the testsuite.
check: $(LIBS)
	$(MAKE) -k -C tests check 2>&1|tee test.log

# Check the hugetlb setup.
test:
	mount | grep /mnth | grep hugetlbfs
	expr `cat /proc/sys/vm/nr_hugepages` '>' 0
	$(MAKE) -C tests TESTS="htlb-n32 htlb-n64"
