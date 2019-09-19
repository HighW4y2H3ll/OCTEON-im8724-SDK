PATH := /usr/local/Cavium_Networks/OCTEON-SDK/tools/usr/bin:$(PATH)
CC = gcc
OCTEON_EXTRA_CFLAGS ?= -march=octeon
CFLAGS = -g -O0 $(OCTEON_EXTRA_CFLAGS)

TESTS = setup htlb-64 htlb-n32 hello-64 hello-n32 \
  verbose-hello-64 verbose-hello-n32 \
  mmap-malloc-n32 mmap-malloc-2-n32 large-malloc-n32 large-malloc-64 \
  trimming-n32 trimming-64 shlib-1-64 shlib-1-n32 \
  random-stack-64 random-stack-n32 no-random-stack-64 no-random-stack-n32 \
  $(TESTS_NO_ERROR) $(TESTS_WITH_ARG)

#TESTS = setup htlb-64 htlb-n32 hello-64 hello-n32 \
  verbose-hello-64 verbose-hello-n32 \
  mmap-malloc-n32 mmap-malloc-2-n32 large-malloc-n32 large-malloc-64 \
  trimming-n32 trimming-64 bug2026-64 bug2026-n32 shlib-1-64 shlib-1-n32 \
  random-stack-64 random-stack-n32 no-random-stack-64 no-random-stack-n32 \
  $(TESTS_NO_ERROR) $(TESTS_WITH_ARG)

# Check that we fall back on something after hugetlb morecore is exhausted.
TESTS_NO_ERROR = over-malloc-n32 over-malloc-64

TESTS_WITH_ARG = \
  over-malloc-n32-arg-2097152 over-malloc-64-arg-2097152 \
  over-malloc-n32-arg-1048576 over-malloc-64-arg-1048576 \
  over-malloc-n32-arg-1024 over-malloc-64-arg-1024 \
  over-malloc-free-n32-arg-2097152 over-malloc-free-64-arg-2097152 \
  over-malloc-free-n32-arg-1048576 over-malloc-free-64-arg-1048576 \
  over-malloc-free-n32-arg-1024 over-malloc-free-64-arg-1024 \
  malloc-free-n32-arg-2097152 malloc-free-64-arg-2097152 \
  malloc-free-n32-arg-1048576 malloc-free-64-arg-1048576 \
  malloc-free-n32-arg-1024 malloc-free-64-arg-1024

BINS = $(sort $(foreach I,$(TESTS), $(shell echo $(I) | \
  sed -e 's/-arg-.*//' -e 's/.*random-//')))

# ??? Finish this.
.precious: $(BINS)

check: $(TESTS:%=test-%)
	@echo "### Successfully completed $(words $(TESTS)) tests."

# ??? Finish this.
clean:
	rm -rf $(BINS)

# Set up hugetlb.
test-setup:
	@echo "### Running $@"
	mkdir -p /mnth
	gcc --version | grep Cavium
	mount | grep /mnth | grep hugetlbfs || mount -t hugetlbfs none /mnth
	echo 60 > /proc/sys/vm/nr_hugepages
	expr `grep HugePages_Free /proc/meminfo | awk '{print $$2}'` '=' 60

# Pass the block size.
$(TESTS_WITH_ARG:%=test-%):
	$(MAKE) $(shell echo $@ | sed -e 's/-arg-.*//') \
	  ARG=$(shell echo $@ | sed -e 's/.*-arg-//')

test-htlb-n32 test-htlb-64: \
  test-%: %
	@echo "### Running $@"
	./$(@:test-%=%)

LIBHTLBREMAP_64 = ../libhtlbremap.so
LIBHTLBREMAP_n32 = ../libhtlbremap-n32.so

test-verbose-hello-n32 test-verbose-hello-64: \
  test-verbose-%: % libs
	@echo "### Running $@ $(ARG)"
	LD_PRELOAD=$(LIBHTLBREMAP_$(shell echo $* | sed -e 's/.*-//')) \
	  HTLBREMAP_DEBUG=1 ./$< $(ARG) 2>&1|tee $@.out
	grep -q htlbremap: $@.out

rand = /proc/sys/kernel/randomize_va_space 
test-random-stack-64 test-random-stack-n32: \
  test-random-%: % libs
	cp $(rand) rand.save
	echo 1 > $(rand)
	LD_PRELOAD=$(LIBHTLBREMAP_$(shell echo $* | sed -e 's/.*-//')) \
	  HTLBREMAP_SIGNAL_ON_ERROR=0 \
	  ./$< $(ARG); ret=$$?; cp rand.save $(rand); exit $$ret

test-no-random-stack-64 test-no-random-stack-n32: \
  test-no-random-%: % libs
	cp $(rand) rand.save
	echo 0 > $(rand)
	LD_PRELOAD=$(LIBHTLBREMAP_$(shell echo $* | sed -e 's/.*-//')) \
	  HTLBREMAP_SIGNAL_ON_ERROR=1 \
	  ./$< $(ARG); ret=$$?; cp rand.save $(rand); exit $$ret

$(TESTS_NO_ERROR:%=test-%): \
  test-%: % libs
	@echo "### Running $@ $(ARG)"
	LD_PRELOAD=$(LIBHTLBREMAP_$(shell echo $* | sed -e 's/.*-//')) \
	  ./$< $(ARG)

test-%: % libs
	@echo "### Running $@ $(ARG)"
	LD_PRELOAD=$(LIBHTLBREMAP_$(shell echo $* | sed -e 's/.*-//')) \
	  HTLBREMAP_SIGNAL_ON_ERROR=1 \
	  ./$< $(ARG)

libs:
	$(MAKE) -C ..

test2-%: %
	./$<


mmap-malloc-2-n32: CFLAGS = -O2 -pthread


%-n32: %.c
	$(CC) $(CFLAGS) -mabi=n32 -o $@ $^

%-64: %.c
	$(CC) $(CFLAGS) -o $@ $^

%-n32: %.cc
	$(CXX) $(CFLAGS) -mabi=n32 -o $@ $^

%-64: %.cc
	$(CXX) $(CFLAGS) -o $@ $^

# Disable these built-in implicit rules.
%: %.c
%: %.o

