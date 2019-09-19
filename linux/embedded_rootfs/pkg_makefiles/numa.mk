.NOTPARALLEL:

PKG:=numactl
VERSION:=2.0.9
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	sed -i '/\/share\/man/d' ${PKG}-${VERSION}/Makefile
	${MAKE} -C ${DIR} WITHOUT_XATTR=1 CROSS=${CROSS}- CFLAGS="${CFLAGS} -I ${ROOT}/usr/include" LDFLAGS="${CFLAGS}"

.PHONY: install
install: build
	${MAKE} -C ${DIR} PREFIX=${ROOT}/usr  install

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz

