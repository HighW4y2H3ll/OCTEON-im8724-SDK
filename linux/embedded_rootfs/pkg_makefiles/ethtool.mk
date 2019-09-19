.NOTPARALLEL:

PKG:=ethtool
VERSION:=4.11
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR}

.PHONY: install
install:
	cp ${DIR}/ethtool ${ROOT}/usr/sbin/ethtool

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && ./configure --quiet --host=${CROSS} CFLAGS="${CFLAGS}" LDFLAGS=""
