#/***********************license start***************
# * Copyright (c) 2003-2007  Cavium Inc. (support@cavium.com). All rights
# * reserved.
# *
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions are
# * met:
# *
# *     * Redistributions of source code must retain the above copyright
# *       notice, this list of conditions and the following disclaimer.
# *
# *     * Redistributions in binary form must reproduce the above
# *       copyright notice, this list of conditions and the following
# *       disclaimer in the documentation and/or other materials provided
# *       with the distribution.
# *
# *     * Neither the name of Cavium Inc. nor the names of
# *       its contributors may be used to endorse or promote products
# *       derived from this software without specific prior written
# *       permission.
# *
# * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
# * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
# * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
# *
# *
# * For any questions regarding licensing please contact marketing@cavium.com
# *
# ***********************license end**************************************/

SIM_OPTS=-quiet -uart0=2020 -noperf
FLAGS=-mabi=64 -mno-abicalls -fno-pic -g -W -Wall -ffreestanding -static
FLAGS+=-DDEBUG
CFLAGS=${FLAGS} -Os -I${OCTEON_ROOT}/target/include -fwhole-program -fno-reorder-functions
ASFLAGS=${FLAGS} -O

.PHONY: all
all: cn5xxx cn6xxx

.PHONY: cn5xxx
cn5xxx: nand-boot-cn5xxx.nand nand-boot-cn5xxx.bin

.PHONY: cn6xxx
cn6xxx: nand-boot-cn6xxx.nand nand-boot-cn6xxx.bin

nand-boot-cn5xxx.elf: start-cn5xxx.o main-cn5xxx.o 
	mips64-octeon-linux-gnu-gcc ${FLAGS} -o $@ $^ -nostartfiles -Wl,-Ttext -Wl,0xffffffff80100000

nand-boot-cn6xxx.elf: start-cn6xxx.o main-cn6xxx.o
	mips64-octeon-linux-gnu-gcc ${FLAGS} -o $@ $^ -nostartfiles -Wl,-Ttext -Wl,0xffffffff80100000

%-cn5xxx.o: %.S
	mips64-octeon-linux-gnu-gcc -DOCTEON_MODEL=OCTEON_CN52XX ${ASFLAGS} -c -o $@ $^
%-cn6xxx.o: %.S
	mips64-octeon-linux-gnu-gcc -DOCTEON_MODEL=OCTEON_CN63XX ${ASFLAGS} -c -o $@ $^

%-cn5xxx.o: %.c
	mips64-octeon-linux-gnu-gcc -DOCTEON_MODEL=OCTEON_CN52XX ${CFLAGS} -c -o $@ $^
%-cn6xxx.o: %.c
	mips64-octeon-linux-gnu-gcc -DOCTEON_MODEL=OCTEON_CN63XX ${CFLAGS} -c -o $@ $^

%-cn5xxx.o: %.S
	mips64-octeon-linux-gnu-gcc -DOCTEON_MODEL=OCTEON_CN52XX ${ASFLAGS} -c -o $@ $^

trivial-stage2.o: trivial-stage2.S
	mips64-octeon-linux-gnu-gcc ${ASFLAGS} -c -o $@ $^

trivial-stage2.bin: trivial-stage2.elf
	mips64-octeon-linux-gnu-objcopy $^ -R .MIPS.options -O binary $@
	make -C ../u-boot tools
	../u-boot/tools/update_octeon_header trivial-stage2.bin generic --nandstage2 --text_base=0xffffffff80004000

%.bin: %.elf
	mips64-octeon-linux-gnu-objcopy $^ -R .MIPS.options -O binary $@

%.nand: %.bin ecc-encode
	./ecc-encode < $*.bin > $@

ecc-encode: ecc-encode.c
	gcc -W -Wall -O3 -g -o $@ $^


trivial-stage2.elf: trivial-stage2.o
	mips64-octeon-linux-gnu-gcc ${FLAGS} -o $@ $^ -nostartfiles -Wl,-Ttext -Wl,0xffffffff80004000

combined-cn5xxx.nand: trivial-stage2.nand nand-boot-cn5xxx.nand
	cat nand-boot-cn5xxx.nand /dev/zero | dd bs=264 count=64 | cat - trivial-stage2.nand /dev/zero | dd bs=264 count=2048 > combined-cn5xxx.nand
combined-cn6xxx.nand: trivial-stage2.nand nand-boot-cn6xxx.nand
	cat nand-boot-cn6xxx.nand /dev/zero | dd bs=264 count=64 | cat - trivial-stage2.nand /dev/zero | dd bs=264 count=2048 > combined-cn6xxx.nand


.PHONY: clean
clean:
	rm -f *.nand *.bin *.elf *.o ecc-encode

# -ld=0:nand-boot.elf isn't requred, but allows the simulator to get
#	the symbols. The data is overwritten by the next argument.
# -ld=0x0:0x8000000 gives us 128 Mbytes of memory
# -ld=0x1fc00000:0x10000 Even though boot memory is comming from NAND,
#	some consistency checks in the simulator expect it to be in the
#	memory map. This memory is replaced by the NAND during early
#	boot.
# -ndf=0:2048x64x2112,nand-boot.nand sets up NAND on chip select 0,
#	with 2048 blocks, each block with 64 pages, and each page with
#	2048 bytes of data plus 64 bytes of OOB.
# ${SIM_OPTS} Whatever extra args someone might want
.PHONY: run-cn5xxx
run-cn5xxx: combined-cn5xxx.nand
	cn52xx-simulator64 -ld=0:nand-boot-cn5xxx.elf -ld=0x0:0x8000000 -ld=0x1fc00000:0x10000 -ndf=0:2048x64x2112,combined-cn5xxx.nand ${SIM_OPTS}
.PHONY: run-cn6xxx
run-cn6xxx: combined-cn6xxx.nand
	cn63xx-simulator64 -ld=0:nand-boot-cn6xxx.elf -ld=0x0:0x8000000 -ld=0x1fc00000:0x10000 -ndf=0:2048x64x2112,combined-cn6xxx.nand ${SIM_OPTS}
.PHONY: run
run:
	echo "Use run-cn5xxx or run-cn6xxx"

