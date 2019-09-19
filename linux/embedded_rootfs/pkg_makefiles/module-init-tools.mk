.NOTPARALLEL:

PKG:=module-init-tools
VERSION:=3.12
DIR:=${PKG}-${VERSION}

TARGET_BUILD = ${PKG}-target
HOST_BUILD = ${PKG}-host

HOST_GARBAGE_FLAGS = CC CFLAGS LD CXX CXXFLAGS AR LDFLAGS LIBDIR CROSS RANLIB

.PHONY: all
all: target-build  host-build install

.PHONY: target-build
target-build: ${DIR} ${TARGET_BUILD}/Makefile
	${MAKE} -C ${TARGET_BUILD}

${TARGET_BUILD}/Makefile:
	mkdir -p ${TARGET_BUILD}
	cd ${TARGET_BUILD} && ../${DIR}/configure --quiet --prefix=${ROOT} --host=${CROSS} CFLAGS="${CFLAGS}"

.PHONY: host-build
host-build: ${DIR} ${HOST_BUILD}/Makefile
	unset ${HOST_GARBAGE_FLAGS} && ${MAKE} -C ${HOST_BUILD} sbin_PROGRAMS=depmod

${HOST_BUILD}/Makefile:
	mkdir -p ${HOST_BUILD}
	unset ${HOST_GARBAGE_FLAGS} && cd ${HOST_BUILD} && ../${DIR}/configure --quiet

.PHONY: install
install:
	-rm -f ${ROOT}/sbin/lsmod ${ROOT}/bin/lsmod
	-rm -f ${ROOT}/sbin/insmod ${ROOT}/bin/insmod
	-rm -f ${ROOT}/sbin/modprobe ${ROOT}/bin/modprobe
	-rm -f ${ROOT}/sbin/rmmod ${ROOT}/bin/rmmod
	${MAKE} -C ${TARGET_BUILD} install
	-rm -f ${ROOT}/sbin/insmod.static ${ROOT}/bin/insmod.static
	-rm -f ${ROOT}/sbin/generate-modprobe.conf$ {ROOT}/bin/generate-modprobe.conf

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
	cd ${DIR} && patch -p1 < ${STORAGE}/module-init-tools-mans.patch
