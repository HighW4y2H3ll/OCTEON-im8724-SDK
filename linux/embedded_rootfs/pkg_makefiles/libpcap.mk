.NOTPARALLEL:

include ../.config

PKG:=libpcap
VERSION:=1.8.1
DIR:=${PKG}-${VERSION}

ifdef CFG_ENABLE_IPV6	
	IPV6=--enable-ipv6
else
	IPV6=--disable-ipv6
endif

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && ac_cv_linux_vers=2 ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS} -with-pcap=linux ${IPV6} CC="${CC} ${CFLAGS}"

.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} install

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && patch -p1 < ${STORAGE}/${PKG}-${VERSION}-1.patch 
	cd ${DIR} && patch -p1 < ${STORAGE}/${PKG}-${VERSION}-2.patch 
