include ../.config

PKG:=procps
VERSION:=3.2.7
DIR:=${PKG}-${VERSION}

ifdef TOOLCHAIN_ABI_64
LIB64 =  ${OCTEON_LIBDIR64}
else
LIB64 =  ${OCTEON_LIBDIR32}
endif

NCURSES=ncurses-6.0

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}/Makefile
	${MAKE} -C ${DIR} \
		CPPFLAGS="-I ${ROOT}/usr/include/ncurses -I ${ROOT}/usr/include/ ${CFLAGS} -I../${NCURSES}" \
		CFLAGS="-I ${ROOT}/usr/include/ncurses -I ${ROOT}/usr/include/ ${CFLAGS} -I../${NCURSES}" \
		LDFLAGS="-L ../${NCURSES}/lib"

.PHONY: install
install: build
	sudo ${MAKE} -C ${DIR} install DESTDIR=${ROOT}/ lib64=${LIB64}

${DIR}/Makefile: ${DIR}
	cd ${DIR} 

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	patch -p1 < ${STORAGE}/procps-3.2.7-makefile.patch


