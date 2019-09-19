.NOTPARALLEL:

PKG:=bash
VERSION:=4.4
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install 

.PHONY: build 
build: ${DIR}
	cd ${DIR} && CC="${CC}" ./configure --target=${CROSS} --host=${CROSS} \
		CFLAGS="${CFLAGS}" bash_cv_job_control_missing=present 
	${MAKE} -C ${DIR} 

.PHONY: install
install:
	mkdir -p ${ROOT}/bin
	cp -f ${DIR}/bash ${ROOT}/bin

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.gz

