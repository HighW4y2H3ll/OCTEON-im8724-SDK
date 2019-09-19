.NOTPARALLEL:
include ../.config

DIR=${shell pwd}/octeon-remote-utils

.PHONY: all
all: ${DIR} build install aliases

${DIR}:
	mkdir -p ${DIR}

.PHONY: build
build:
	${MAKE} -C ${OCTEON_ROOT}/host/remote-lib DIR=${DIR} CROSS=${CROSS}- EXTRA_CFLAGS="${TOOLCHAIN_ABI}"
	${MAKE} -C ${OCTEON_ROOT}/host/remote-utils DIR=${DIR} CROSS=${CROSS}- EXTRA_CFLAGS="${TOOLCHAIN_ABI}"
	${MAKE} -C ${OCTEON_ROOT}/host/coredump-util DIR=${DIR} CROSS=${CROSS}- EXTRA_CFLAGS="${TOOLCHAIN_ABI}"

${ROOT}/usr/bin/oct-linux-%:
	echo "#!/bin/sh" > $@
	echo "OCTEON_REMOTE_PROTOCOL=linux ${@:${ROOT}/usr/bin/oct-linux-%=oct-remote-%} \"\$$@\"" >> $@
	chmod +x $@

${ROOT}/usr/bin/oct-pci-%:
	echo "#!/bin/sh" > $@
	echo "OCTEON_REMOTE_PROTOCOL=pci ${@:${ROOT}/usr/bin/oct-pci-%=oct-remote-%} \"\$$@\"" >> $@
	chmod +x $@

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/oct-remote-* ${ROOT}/usr/bin/

ALIASES = boot bootcmd console core csr ddr load memory pow profile reset save tra qlm coredump

.PHONY: aliases
aliases: ${ALIASES:%=${ROOT}/usr/bin/oct-linux-%} ${ALIASES:%=${ROOT}/usr/bin/oct-pci-%}
	# The following aliases don't make sense under Linux
	rm -f ${ROOT}/usr/bin/oct-linux-boot
	rm -f ${ROOT}/usr/bin/oct-linux-bootcmd
	rm -f ${ROOT}/usr/bin/oct-linux-console
	rm -f ${ROOT}/usr/bin/oct-linux-ddr
	rm -f ${ROOT}/usr/bin/oct-linux-load
	rm -f ${ROOT}/usr/bin/oct-linux-save
	rm -f ${ROOT}/usr/bin/oct-linux-reset

