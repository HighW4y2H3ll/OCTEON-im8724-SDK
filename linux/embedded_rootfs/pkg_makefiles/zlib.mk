.NOTPARALLEL:

include ../.config

PKG:=zlib
VERSION:=1.2.3
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	if grep 'CC=' ${DIR}/Makefile | grep -q ${CC} ; then : ; else unset CFLAGS && cd ${DIR} && CC="${CC} ${TOOLCHAIN_ABI}" ./configure --shared --prefix=${ROOT}/usr ; fi
	${MAKE} -C ${DIR}


.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} install libdir=${ROOT}/${LIBDIR}
	- ln -s ${DIR} zlib

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
