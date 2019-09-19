include ../.config

PKG:=valgrind
VERSION:=3.0.0
DIR:=${PKG}-${VERSION}

.PHONY: all
ifdef TOOLCHAIN_LITTLE_ENDIAN
all:
else
all: install
endif

install:
	mkdir -p ${ROOT}/usr/local
	cd ${ROOT}/usr/local && tar -xzf ${STORAGE}/${DIR}.tar.gz
