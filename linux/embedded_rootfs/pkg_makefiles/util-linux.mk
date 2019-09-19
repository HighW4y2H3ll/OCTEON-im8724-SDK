.NOTPARALLEL:

include ../.config

PKG:=util-linux
VERSION:=2.33.1
DIR:=${PKG}-${VERSION}

NCURSES=${shell pwd}/ncurses-6.0

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	rm -f ${NCURSES}/lib/libtinfo.so.6 ${NCURSES}/lib/libtinfo.so
	ln -s ${NCURSES}/lib/libncurses.so.6.0 ${NCURSES}/lib/libtinfo.so.6
	ln -s ${NCURSES}/lib/libncurses.so.6.0 ${NCURSES}/lib/libtinfo.so
	cd ${DIR} && ./configure --host=${CROSS} \
			CPPFLAGS="-I ${ROOT}/usr/include/ncurses -I ${ROOT}/usr/include/ ${CFLAGS} -I${NCURSES}" \
			CFLAGS="-I ${ROOT}/usr/include/ncurses -I ${ROOT}/usr/include/ ${CFLAGS} -I${NCURSES}" LDFLAGS="${LDFLAGS} -L${NCURSES}/lib" \
			--disable-all-programs --enable-fdisks --enable-libfdisk --enable-libuuid --enable-libsmartcols \
			--disable-widechar --without-python --without-btrfs --without-readline --without-ncursesw

.PHONY: install
install: ${DIR}
	mkdir -p ${ROOT}/sbin
	cp ${DIR}/.libs/fdisk ${ROOT}/sbin/fdisk
	cp ${DIR}/.libs/cfdisk ${ROOT}/sbin/cfdisk
	cp ${DIR}/.libs/sfdisk ${ROOT}/sbin/sfdisk
	cp ${DIR}/.libs/libfdisk.so.1.1.0 ${ROOT}/lib64/
	ln -s /lib64/libfdisk.so.1.1.0 ${ROOT}/lib64/libfdisk.so.1
	ln -s /lib64/libfdisk.so.1.1.0 ${ROOT}/lib64/libfdisk.so
	cp ${DIR}/.libs/libsmartcols.so.1.1.0 ${ROOT}/lib64/
	ln -s /lib64/libsmartcols.so.1.1.0 ${ROOT}/lib64/libsmartcols.so.1
	ln -s /lib64/libsmartcols.so.1.1.0 ${ROOT}/lib64/libsmartcols.so
	cp ${DIR}/.libs/libuuid.so.1.3.0 ${ROOT}/lib64/
	ln -s /lib64/libuuid.so.1.3.0 ${ROOT}/lib64/libuuid.so.1
	ln -s /lib64/libuuid.so.1.3.0 ${ROOT}/lib64/libuuid.so
	ln -s /lib64/libncurses.so.6.0 ${ROOT}/lib64/libtinfo.so.6
	ln -s /lib64/libncurses.so.6.0 ${ROOT}/lib64/libtinfo.so

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.xz
