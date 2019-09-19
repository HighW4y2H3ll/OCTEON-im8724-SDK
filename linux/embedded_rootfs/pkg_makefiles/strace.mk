.NOTPARALLEL:

include ../.config

PKG:=strace
VERSION:=4.8
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && ./configure --quiet --host=${CROSS} CFLAGS="${CFLAGS}"

.PHONY: install
install: ${DIR}
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/strace ${ROOT}/usr/bin/strace

${DIR}:
	tar -xzf ${STORAGE}/${PKG}-${VERSION}.tar.gz
