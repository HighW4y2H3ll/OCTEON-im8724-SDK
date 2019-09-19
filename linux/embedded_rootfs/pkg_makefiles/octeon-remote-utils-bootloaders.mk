.NOTPARALLEL:
include ../.config

DIR=${shell pwd}/octeon-remote-utils-bootloaders

.PHONY: all
all: ${DIR} install

${DIR}:
	mkdir -p ${DIR}


.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${OCTEON_ROOT}/target/bin/memtest.bin ${ROOT}/
	- cp ${OCTEON_ROOT}/target/bin/u-boot-octeon_*.bin ${ROOT}/

