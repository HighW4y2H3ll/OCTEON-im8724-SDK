.NOTPARALLEL:

include ../.config

PKG:=kexec-tools
VERSION:=2.0.15
DIR:=${PKG}-${VERSION}

unexport LDFLAGS

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}/Makefile
	${MAKE} -C ${DIR}

.PHONY: install
install: build
	mkdir -p ${ROOT}/usr/sbin
	${MAKE} -C ${DIR} install DESTDIR=${ROOT}/

${DIR}/Makefile: ${DIR}
	cd ${DIR} && ./configure --prefix=/usr --host=${CROSS} CFLAGS="${CFLAGS}"

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && patch -p0 < ${STORAGE}/${PKG}-${VERSION}-1.patch
	cd ${DIR} && patch -p0 < ${STORAGE}/${PKG}-${VERSION}-2.patch
	cd ${DIR} && patch -p1 < ${STORAGE}/${PKG}-${VERSION}-3.patch


