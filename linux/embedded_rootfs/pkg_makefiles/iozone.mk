.NOTPARALLEL:

include ../.config

PKG:=iozone3
VERSION:=263
DIR:=${PKG}_${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR}/src/current CC="${CC} ${CFLAGS}" linux

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/src/current/iozone ${ROOT}/usr/bin/iozone

${DIR}:
	tar -jxf ${STORAGE}/${PKG}_${VERSION}.tar.bz2

