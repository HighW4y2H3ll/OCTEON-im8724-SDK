.NOTPARALLEL:

PKG:=flex
VERSION:=2.6.4
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && CC="${CC}" CFLAGS="${CFLAGS}" ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS}

.PHONY: install
install:
	${MAKE} -j1 -C ${DIR} install
	rm ${ROOT}/usr/bin/flex

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && patch -p1 < ${STORAGE}/flex-2.6.4-1.patch
