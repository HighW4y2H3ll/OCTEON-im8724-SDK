.NOTPARALLEL:

include ../.config

PKG:=libuuid
VERSION:=1.0.3
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

${DIR}/Makefile: ${DIR}
	cd ${DIR} && \
	./configure --prefix=${ROOT}/usr --target=${CROSS} --host=${CROSS} --libdir=${ROOT}/${LIBDIR}

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR} PREFIX=/usr

.PHONY: install
install:	${DIR}
	${MAKE} -C ${DIR} install

${DIR}:
	tar -xaf ${STORAGE}/${PKG}-${VERSION}.tar.gz
