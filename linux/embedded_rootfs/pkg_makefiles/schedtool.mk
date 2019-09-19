.NOTPARALLEL:

include ../.config

PKG:=schedtool
VERSION:=1.3.0
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}"


.PHONY: install
install: ${DIR}
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/schedtool ${ROOT}/usr/bin/schedtool

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
