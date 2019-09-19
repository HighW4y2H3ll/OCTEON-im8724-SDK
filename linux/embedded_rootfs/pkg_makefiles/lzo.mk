.NOTPARALLEL:

include ../.config

PKG:=lzo
VERSION:=2.03
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC}" CFLAGS="${CFLAGS}" AR="${AR} rc"

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && ./configure --prefix=${ROOT}/usr --quiet --host=${CROSS} CFLAGS="${CFLAGS}"


