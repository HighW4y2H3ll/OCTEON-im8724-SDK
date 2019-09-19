.NOTPARALLEL:

PKG:=rt-tests
VERSION:=0.74
DIR:=${PKG}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}" STRIP=${STRIP}

.PHONY: install
install:
	cp ${DIR}/cyclictest ${ROOT}/usr/sbin/
	cp ${DIR}/hackbench ${ROOT}/usr/sbin/
	cp ${DIR}/pmqtest ${ROOT}/usr/sbin/
	cp ${DIR}/ptsematest ${ROOT}/usr/sbin/
	cp ${DIR}/sigwaittest ${ROOT}/usr/sbin/

${DIR}:
	tar -xvf ${STORAGE}/INTERNAL-${PKG}-${VERSION}.tar.bz2
