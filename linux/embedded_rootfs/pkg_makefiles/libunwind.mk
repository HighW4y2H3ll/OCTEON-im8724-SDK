.NOTPARALLEL:

include ../.config

BUILD:=${shell pwd}

LIBUNWIND_VERSION = 1.1
LIBUNWIND_SITE = http://download.savannah.gnu.org/releases/libunwind
LIBUNWIND_INSTALL_STAGING = YES
LIBUNWIND_LICENSE_FILES = COPYING
LIBUNWIND_LICENSE = MIT


PKG:=libunwind
VERSION:=1.1
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
${DIR}/Makefile: ${DIR}/configure
	cd ${DIR} && \
	CC="${CC} ${TOOLCHAIN_ABI}" \
	./configure \
	--host=${CC:-gcc=} \
	--enable-maintainer-mode \
	--enable-dependency-tracking \
	--with-gnu-ld \
	--enable-debug \
	--prefix=/usr
#${DIR}/Makefile: ${DIR} $*.am $*.in; cd ${DIR} && automake

build: ${DIR} ${DIR}/Makefile
	#if grep '^CC =' ${DIR}/Makefile | grep -q ${CC} ; then : ; else unset CFLAGS && cd ${DIR} && CC="${CC} ${TOOLCHAIN_ABI}" ./configure --host=mips64-octeon-linux-gnu --without-zlib --prefix=/usr ; fi
	${MAKE} -C ${DIR}
	${MAKE} -C ${DIR} install DESTDIR=${BUILD}/${DIR}

.PHONY: install
install: ${DIR}
	cp -d ${DIR}/usr/lib/libunwind*so* ${ROOT}/${LIBDIR}

tarball = ${STORAGE}/${PKG}-${VERSION}.tar.gz
${tarball}:
	cd ${@D} && wget $(LIBUNWIND_SITE)/${@F}

${DIR}: ${tarball}
	tar -zxf ${tarball}
	for p in ${STORAGE}/${PKG}-*.patch; do patch -d ${DIR} -p1 <$$p; done
