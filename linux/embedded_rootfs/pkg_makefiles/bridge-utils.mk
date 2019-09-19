.NOTPARALLEL:

PKG:=bridge-utils
VERSION:=1.5
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR} CFLAGS="${CFLAGS}"

${DIR}/Makefile:
	cd ${DIR} && patch -p0 < ${STORAGE}/bridge-utils-1.5-in.patch
	cd ${DIR} && ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS} \
		--with-linux-headers=${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/include \
		CFLAGS="${CFLAGS}" LDFLAGS="${TOOLCHAIN_ABI}"

.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} install

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	
