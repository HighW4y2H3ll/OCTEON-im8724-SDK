.NOTPARALLEL:

include ../.config

PKG:=dosfstools
VERSION:=3.0.26
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC}" CFLAGS="${CFLAGS}" LDFLAGS="${CFLAGS}"

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/mkfs.fat ${ROOT}/usr/sbin/mkfs.fat
	ln -s mkfs.fat ${ROOT}/usr/sbin/mkdosfs
	ln -s mkfs.fat ${ROOT}/usr/sbin/mkfs.vfat
	cp ${DIR}/fsck.fat ${ROOT}/usr/sbin/fsck.fat
	ln -s fsck.fat ${ROOT}/usr/sbin/fsck.vfat
	cp ${DIR}/fatlabel ${ROOT}/usr/sbin/fatlabel
	ln -s fatlabel ${ROOT}/usr/sbin/dosfslabel

${DIR}:
	tar -xvf ${STORAGE}/${PKG}-${VERSION}.tar.gz
