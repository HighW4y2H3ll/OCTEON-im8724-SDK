.NOTPARALLEL:

PKG:=openssh
VERSION:=6.1p1
DIR:=${PKG}-${VERSION}

ZLIB_DIR=${shell pwd}/zlib
SSL_DIR=${shell pwd}/openssl

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/Makefile
	${MAKE} -C ${DIR}

${DIR}/Makefile:
	cd ${DIR} && ./configure --quiet --prefix=/usr --host=${CROSS} --with-ssl-dir=${SSL_DIR} --with-zlib=${ZLIB_DIR} LD="$(CC) ${CFLAGS}" --disable-strip

.PHONY: install
install:
	${MAKE} -C ${DIR} install DESTDIR=${ROOT}
	cat ${ROOT}/usr/etc/sshd_config						\
	  | sed 's/UsePrivilegeSeparation sandbox/UsePrivilegeSeparation no/'	\
	  | sed 's/#PermitEmptyPasswords no/PermitEmptyPasswords yes/' 		\
	   > ${ROOT}/usr/etc/sshd_config_new
	mv ${ROOT}/usr/etc/sshd_config_new ${ROOT}/usr/etc/sshd_config

${DIR}:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	
