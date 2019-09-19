.NOTPARALLEL:

PKG:=iputils
VERSION:=s20121221
DIR:=${PKG}-${VERSION}
PWD = ${shell pwd}
LFLAGS = ${CFLAGS} -I$(PWD)/openssl/include -L$(PWD)/openssl

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC} ${LFLAGS}" USE_CAP=no USE_GNUTLS=no DESTDIR=${ROOT}

.PHONY: install
install:
	rm -f ${ROOT}/bin/arping
	${STRIP} -o ${ROOT}/bin/arping ${DIR}/arping
	rm -f ${ROOT}/bin/ping
	${STRIP} -o ${ROOT}/bin/ping ${DIR}/ping
	rm -f ${ROOT}/bin/tracepath
	${STRIP} -o ${ROOT}/bin/tracepath ${DIR}/tracepath
	rm -f ${ROOT}/bin/ping6
	${STRIP} -o ${ROOT}/bin/ping6 ${DIR}/ping6
	rm -f ${ROOT}/bin/tracepath6
	${STRIP} -o ${ROOT}/bin/tracepath6 ${DIR}/tracepath6
	rm -f ${ROOT}/bin/traceroute6
	${STRIP} -o ${ROOT}/bin/traceroute6 ${DIR}/traceroute6

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
