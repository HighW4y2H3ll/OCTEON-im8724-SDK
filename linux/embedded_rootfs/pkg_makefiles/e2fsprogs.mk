.NOTPARALLEL: 
include ../.config

PKG:=e2fsprogs
VERSION:=1.42.6
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build:  ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR} 

${DIR}/Makefile:
	cd ${DIR} && ./configure  --quiet --prefix=${ROOT}/usr --host=${CROSS} LDFLAGS="$(LDFLAGS)" CFLAGS="$(CFLAGS)" CC="$(CC) $(CFLAGS)"
	
.PHONY: install
install:
	${MAKE} -C ${DIR} install install-libs

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && patch -p1 < ${STORAGE}/e2fsprogs.patch
