.NOTPARALLEL:

PKG:=ptpd
VERSION:=1.0.0
DIR:=${PKG}-${VERSION}

.PHONY: all
all: ${DIR} ${ROOT}/usr/bin/ptpd

${DIR}/src/ptpd:
	${MAKE} -C ${DIR}/src CC="${CC}" CFLAGS="${CFLAGS}" LDFLAGS="${CFLAGS}"

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	patch --quiet -p0 < ${STORAGE}/ptpd-fix-limits.patch

${ROOT}/usr/bin/ptpd: ${DIR}/src/ptpd
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/src/ptpd ${ROOT}/usr/bin/

