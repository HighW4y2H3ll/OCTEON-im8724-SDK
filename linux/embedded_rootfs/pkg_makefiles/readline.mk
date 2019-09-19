.NOTPARALLEL:

PKG:=readline
VERSION:=7.0
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS} CFLAGS="${CFLAGS}" LDFLAGS="${TOOLCHAIN_ABI}"

.PHONY: install
install:
	${MAKE} -C ${DIR} install

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	
