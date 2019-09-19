.NOTPARALLEL:

include ../.config

PKG:=parted
VERSION:=3.1
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

LIBUUID=${shell pwd}/libuuid-1.0.3

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	[ -d ${LIBUUID}/uuid ] && rm -rf ${LIBUUID}/uuid; mkdir ${LIBUUID}/uuid && cp ${LIBUUID}/*.h ${LIBUUID}/uuid/
	cd ${DIR} && ./configure --host=${CROSS} CFLAGS="${CFLAGS} -I${LIBUUID}" LDFLAGS="${LDFLAGS} -L${LIBUUID}/.libs" --disable-device-mapper --without-readline

.PHONY: install
install: ${DIR}
	mkdir -p ${ROOT}/sbin
	cp ${DIR}/parted/.libs/parted ${ROOT}/sbin/parted
	cp ${DIR}/partprobe/.libs/partprobe ${ROOT}/sbin/partprobe
	cp ${DIR}/libparted/.libs/libparted.so.2.0.0 ${ROOT}/lib64/
	ln -s /lib64/libparted.so.2.0.0 ${ROOT}/lib64/libparted.so.2
	ln -s /lib64/libparted.so.2.0.0 ${ROOT}/lib64/libparted.so

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.xz
