.NOTPARALLEL:

include ../.config

PKG:=trace-cmd
VERSION:=
DIR:=${PKG}

CROSS_COMPILE ?= mips64-octeon-linux-gnu-
export CROSS_COMPILE

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} NO_PYTHON=1 CFLAGS="-g -Wall ${CFLAGS}" prefix=/usr all_cmd 


.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} NO_PYTHON=1 CFLAGS="-g -Wall ${CFLAGS}" prefix=/usr DESTDIR=${ROOT} install_cmd

${DIR}:
	tar -jxf ${STORAGE}/${PKG}.tar.bz2
	(patch -p1 <  ${STORAGE}/${PKG}.patch)
