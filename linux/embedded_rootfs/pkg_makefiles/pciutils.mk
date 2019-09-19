.NOTPARALLEL:

PKG:=pciutils
VERSION:=3.5.5
DIR:=${PKG}-${VERSION}

ZLIBSEARCH=${shell pwd}/zlib-1.2.3

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CFLAGS="${CFLAGS} -I ${ZLIBSEARCH}" LDFLAGS="${CFLAGS} -L ${ZLIBSEARCH}" RANLIB=${RANLIB} AR=${AR} ZLIB=yes CROSS_COMPILE=${CROSS}- 
.PHONY: install
install:
	cp ${DIR}/lspci ${ROOT}/bin/
	cp ${DIR}/setpci ${ROOT}/bin/
	mkdir -p ${ROOT}/usr/local/share/
	cp ${DIR}/pci.ids.gz ${ROOT}/usr/local/share/

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	sed -i "s/#define PCI_HAVE_PM_INTEL_CONF/#undef PCI_HAVE_PM_INTEL_CONF/g" ${DIR}/lib/configure
	sed -i "s/HWDB=/HWDB=no/g" ${DIR}/Makefile

