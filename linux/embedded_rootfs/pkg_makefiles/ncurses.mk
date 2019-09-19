include ../.config

PKG:=ncurses
VERSION:=6.0
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}/Makefile
	${MAKE} -C ${DIR}

.PHONY: install
install: build
	${MAKE} -C ${DIR} install DESTDIR=${ROOT}

${DIR}/Makefile: ${DIR}
	if [ ! -f  ${DIR}/Makefile ]; then \
	cd ${DIR} && CC="${CC}" CFLAGS="${CFLAGS}" ./configure --with-shared --without-manpages  --without-ada --quiet --prefix=/usr --host=${CROSS}; \
	patch -p0 < ${STORAGE}/${PKG}-${VERSION}-build.patch; \
	fi

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz


