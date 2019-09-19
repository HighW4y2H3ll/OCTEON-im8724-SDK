.NOTPARALLEL:

include ../.config
BUILD:=${shell pwd}
PKG:=flex
VERSION:=2.6.4
DIR:=${PKG}-${VERSION}
DIR_HOST:=${PKG}_host-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR_HOST} ${DIR_HOST}/Makefile
	${MAKE} -C ${DIR_HOST}

${DIR_HOST}/Makefile:
	cd ${DIR_HOST} && ./configure --quiet --prefix=${BUILD}/usr --host=i386 CC=gcc CFLAGS=

.PHONY: install
install:
	${MAKE} -j1 -C ${DIR_HOST} install

${DIR_HOST}:
	tar -zxf ${STORAGE}/${DIR}.tar.gz
	mv ${DIR} ${DIR_HOST}
