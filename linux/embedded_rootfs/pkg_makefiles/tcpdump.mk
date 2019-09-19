.NOTPARALLEL:

include ../.config

PKG:=tcpdump
VERSION:=4.9.0
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
ifdef CFG_ENABLE_IPV6	
	sed -i "s/td_cv_buggygetaddrinfo=yes/td_cv_buggygetaddrinfo=no/" ${DIR}/configure
endif
	cd ${DIR} && ac_cv_linux_vers=2 ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS} --without-crypto CFLAGS="${CFLAGS}" ${IPV6}

.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} install

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	
