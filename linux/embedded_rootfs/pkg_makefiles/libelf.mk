.NOTPARALLEL:

include ../.config

BUILD:=${shell pwd}

PKG:=elfutils
VERSION:=0.155
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	if grep '^CC =' ${DIR}/Makefile | grep -q ${CC} ; then : ; else unset CFLAGS && cd ${DIR} && CC="${CC} ${TOOLCHAIN_ABI}" ./configure --host=mips64-octeon-linux-gnu --without-zlib --prefix=/usr ; fi
	${MAKE} -C ${DIR}
	${MAKE} -C ${DIR} install DESTDIR=${BUILD}/${DIR}

.PHONY: install
LIBS = $(foreach L, elf dw dwfl asm, $(wildcard ${DIR}/usr/lib/lib${L}*.so*))
install: build
	cp -d ${LIBS} ${ROOT}/${LIBDIR}/

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
