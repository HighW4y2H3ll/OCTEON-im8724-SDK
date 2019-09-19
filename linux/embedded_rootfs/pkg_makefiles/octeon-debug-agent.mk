.NOTPARALLEL:
include ../.config

DIR=${shell pwd}/octeon-debug-agent

.PHONY: all
all: ${DIR} build install

${DIR}:
	mkdir -p ${DIR}

.PHONY: build
build:
	${MAKE} -C ${OCTEON_ROOT}/host/remote-lib DIR=${DIR} CROSS=${CROSS}- EXTRA_CFLAGS="${TOOLCHAIN_ABI}"
	${MAKE} -C ${OCTEON_ROOT}/host/debug-agent DIR=${DIR} CROSS=${CROSS}- EXTRA_CFLAGS="${TOOLCHAIN_ABI}"

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/oct-debug-agent ${ROOT}/usr/bin/

