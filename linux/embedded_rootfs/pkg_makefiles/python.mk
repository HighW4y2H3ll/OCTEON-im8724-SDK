.NOTPARALLEL:

include ../.config

PKG:=Python
VERSION:=2.7.13
DIR:=${PKG}-${VERSION}
PREFIX="/usr/local"
PPWD=${shell pwd}/${DIR}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}/Makefile
	${MAKE} -C ${DIR} clean
	${MAKE} -C ${DIR} install
	${MAKE} -C ${DIR} clean
	cd ${DIR} && PATH=${PPWD}/python-host/bin:${PATH} CROSS_COMPILE=${CROSS} ./configure --quiet \
		--host=${CROSS} --target=${CROSS} --build=x86_64-linux-gnu \
		--prefix=${PREFIX} --disable-ipv6 ac_cv_file__dev_ptmx=yes    \
		ac_cv_file__dev_ptc=no ac_cv_have_long_long_format=yes
	PATH=${PPWD}/python-host/bin:${PATH} ${MAKE} -C ${DIR} 

.PHONY: install
install:
	- cd ${DIR} && CROSS_COMPILE=${CROSS} PATH=${PPWD}/python-host/bin:${PATH} ${MAKE} install DESTDIR=${ROOT}
	mkdir -p ${ROOT}/${PREFIX}/lib
	cp -r ${DIR}/build/lib.linux2-mips64-2.7/* ${ROOT}/${PREFIX}/lib
	ln -s ${PREFIX}/bin/python ${ROOT}/usr/bin

${DIR}/Makefile: ${DIR}
	cd ${DIR} &&  ./configure --quiet CC=gcc CXX=g++ AR=ar RANLIB=ranlib CFLAGS='-g -O2' --prefix=${PPWD}/python-host

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.xz
