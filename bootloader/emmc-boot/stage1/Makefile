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

FLAGS=-mabi=64 -mno-abicalls -fno-pic -g -W -Wall -ffreestanding -static -msym32
LDFLAGS=-nostartfiles -Wl,-Ttext,0xffffffff81000000 -Wl,-Bstatic -Wl,-e,__start
CFLAGS=${FLAGS} -Os -I${OCTEON_ROOT}/target/include \
	-I${OCTEON_ROOT}/executive -march=octeon2 -DOCTEON_MODEL=${OCTEON_MODEL} \
	-fwhole-program -fno-reorder-functions -mno-check-zero-division -fno-asynchronous-unwind-tables

ASFLAGS=${FLAGS} -O -march=octeon2

.PHONY: all
$(OCTEON_ROOT)/target/bin/emm-boot.bin: emm-boot.bin
	cp emm-boot.bin $(OCTEON_ROOT)/target/bin/emm-boot.bin

emm-boot:	emm-boot.bin

emm-boot.elf:	start.o main.o
	mips64-octeon-linux-gnu-gcc ${FLAGS} -o $@ $^ ${LDFLAGS}

start.o: start.S config.h
	mips64-octeon-linux-gnu-gcc ${ASFLAGS} -c -o $@ start.S

main.o: main.c config.h fat.h mmc.h
	mips64-octeon-linux-gnu-gcc ${CFLAGS} -c -o $@ main.c

emm-boot.bin: emm-boot.elf
	mips64-octeon-linux-gnu-objcopy $^ -R .MIPS.options -R .MIPS.abiflags  -O binary $@

.PHONY: clean
clean:
	rm -f *.bin *.o *.elf
