.NOTPARALLEL:

PKG:=linuxptp
VERSION:=1.7
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	CROSS_COMPILE=${CROSS}- ${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}" STRIP=${STRIP}

.PHONY: install
install:
	cp ${DIR}/ptp4l ${ROOT}/usr/bin
	cp ${DIR}/phc2sys ${ROOT}/usr/bin
	sudo mknod ${ROOT}/dev/ptp0 c 252 0

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tgz
