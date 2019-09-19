.NOTPARALLEL:

PKG:=mii-tool
VERSION:=1.9.1.1
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}"

.PHONY: install
install:
	cp ${DIR}/mii-tool ${ROOT}/usr/sbin/

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
	cd ${DIR} && patch -p0 < ${STORAGE}/mii-tool-missing-include.patch
	
