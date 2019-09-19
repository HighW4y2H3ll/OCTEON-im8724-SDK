.NOTPARALLEL:

include ../.config

PKG:=nvme-cli
VERSION:=20170901
DIR:=${PKG}

.PHONY: all
all: build install

${DIR}/NVME-VERSION-FILE: ${DIR}
	${MAKE} -C ${DIR} NVME-VERSION-FILE

-include ${DIR}/NVME-VERSION-FILE

.PHONY: build
build: ${DIR}/nvme

${DIR}/nvme: ${DIR}/NVME-VERSION-FILE
	${MAKE} -C ${DIR} CC="${CC} ${CFLAGS}"

.PHONY: install
install:	${DIR}/nvme
	${MAKE} -C ${DIR} PREFIX=/usr DESTDIR=${ROOT} install CC="${CC} ${CFLAGS}"

${DIR}:
	tar -xaf ${STORAGE}/${PKG}-${VERSION}.tar.xz
