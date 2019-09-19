#/***********************license start***************
# * Copyright (c) 2003-2014  Cavium Inc. (support@cavium.com). All rights
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

FLAGS=-mabi=64 -mno-abicalls -fno-pic -g -W -Wall -static -ffreestanding \
	-static -msym32 -DOCX_BOOT

# FLAGS+=-DDEBUG
# LDFLAGS=-nostartfiles -Wl,-Ttext,0xffffffff81000000 \
#	-Wl,-Bstatic -Wl,-e,__start \
#	-nostdlib -Wl,--gc-sections -Wl,-T,ocx-boot.lds
NM=mips64-octeon-linux-gnu-nm
CC=mips64-octeon-linux-gnu-gcc
LD=mips64-octeon-linux-gnu-ld
LDBFD=mips64-octeon-linux-gnu-ld.bfd
OBJCOPY=mips64-octeon-linux-gnu-objcopy

LDFLAGS=-EB -Bstatic --gc-sections -G 0 -n -nostdlib

CFLAGS=${FLAGS} -Os -I${OCTEON_ROOT}/target/include -I${OCTEON_ROOT}/executive \
	-I. -I../stage1-common -fno-reorder-functions -march=octeon3 \
	-nodefaultlibs -mno-check-zero-division -DOCTEON_MODEL=OCTEON_CN78XX \
	-fwhole-program -msoft-float -DOCTEON_STAGE1_BOOTLOADER
ASFLAGS=${FLAGS} -O -march=octeon3 -msoft-float

OCX_SRCS=ocx-boot.c ocx-boot-start.S

SYSTEM_MAP = \
	$(NM) $1 | \
	grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
	LC_ALL=C sort

.PHONY: all
all: depend ocx-boot.bin

System-ocx-boot.map: ocx-boot.elf
	@$(call SYSTEM_MAP,$<) > $@

ocx-boot.elf:	ocx-boot-start.o ocx-boot.o ocx-boot.lds
	$(LDBFD) ${LDFLAGS} \
		-T ocx-boot.lds -o $@ $^ -Map ocx-boot.map \
		-Ttext 0xffffffff81000000

ocx-boot-start.o: ocx-boot-start.S ocx-config.h
	$(CC) ${ASFLAGS} -c -o $@ ocx-boot-start.S

ocx-boot.o: ocx-boot.c depend
	$(CC) ${CFLAGS} -c -o $@ $<

ocx-boot.bin: ocx-boot.elf
	$(OBJCOPY) $^ -R .MIPS.options -R .MIPS.abiflags  -O binary $@
	../u-boot/tools/update_octeon_header --stage1 $@ GENERIC \
	--text_base=0x81000000

depend:	.depend

.depend: $(OCX_SRCS)
	rm -f ./.depend
	$(CC) -MM -I. -I../stage1-common $^ > ./.depend

include .depend

.PHONY: clean
clean:
	rm -f *.bin *.elf *.o *.map

distclean:
	rm -f *.bin *.elf *.o *.map .depend

tftp: /tftpboot/ocx-boot.bin
	cp $^ $@

# DO NOT DELETE
