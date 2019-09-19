.NOTPARALLEL:

PKG:=iproute2
VERSION:=4.10.0
DIR:=${PKG}-${VERSION}
BUILD:=${shell pwd}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}/Makefile
	${MAKE} -C ${DIR}

.PHONY: install
install:
	${MAKE} -C ${DIR} DESTDIR=${ROOT} STRIP=${STRIP} install

${DIR}/Makefile: ${DIR}
	cd ${DIR} && CC="${CC} ${CFLAGS}" AR=${AR} DESTDIR=${ROOT} ./configure --prefix=${ROOT}/usr --host=${CROSS}
	sed -i 's/HAVE_SELINUX:=y/HAVE_SELINUX:=n/g' ${DIR}/Config
	sed -i 's/HAVE_MNL:=y/HAVE_MNL:=n/g' ${DIR}/Config
	sed -i 's/TC_CONFIG_IPSET:=y/#TC_CONFIG_IPSET/g' ${DIR}/Config
	sed -i 's/IPT_LIB_DIR/#IPT_LIB_DIR/g' ${DIR}/Config
	echo "TC_CONFIG_NO_XT:=y" >> ${DIR}/Config

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
