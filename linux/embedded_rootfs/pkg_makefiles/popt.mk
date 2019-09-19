.NOTPARALLEL:

PKG:=popt
VERSION:=1.16
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && CC="${CC} ${CFLAGS}" CFLAGS="${CFLAGS}" ./configure --quiet --prefix=${ROOT}/usr --host=${CROSS}

.PHONY: install
install: ${DIR}
	${MAKE} -C ${DIR} install libdir=${ROOT}/${OCTEON_LIBDIR64}

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	ln -s ${PKG}-${VERSION} popt
	
