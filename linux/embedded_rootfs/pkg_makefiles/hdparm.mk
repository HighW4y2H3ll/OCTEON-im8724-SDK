.NOTPARALLEL:

PKG:=hdparm
VERSION:=9.9
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}" STRIP=${STRIP}

.PHONY: install
install:
	cp ${DIR}/hdparm ${ROOT}/usr/sbin/

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
