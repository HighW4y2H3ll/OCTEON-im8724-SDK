.NOTPARALLEL:

include ../.config

ifdef TOOLCHAIN_ABI_N32
   SSL_TARGET=linux-octeon32
endif
ifdef TOOLCHAIN_ABI_64
   SSL_TARGET=linux-octeon64
endif

PKG:=openssl
VERSION:=1.0.1p
DIR:=${PKG}-${VERSION}

.PHONY: all
all: install

${DIR}/.build_complete: ${DIR}/Makefile.bak
	${MAKE} -j1 -C ${DIR} AR="${AR} -r"
	touch ${DIR}/.build_complete

${DIR}/Makefile.bak: ${DIR}/Configure
	cd ${DIR} && ./Configure shared --openssldir=/usr ${SSL_TARGET}

.PHONY: install
install: ${DIR}/.build_complete
	${MAKE} -j1 -C ${DIR} INSTALL_PREFIX=${ROOT} install_sw
ifdef CFG_OPENSSL_TESTSUITE
	mkdir -p ${ROOT}/examples/openssl-testsuite
	for f in `cd ${DIR}/test; find . -perm -1 -and -not -type l -and -not -type d -and -not -name bctest -and -not -name evp_test -and -not -name ssltest -and -not -name test_padlock -and -not -name \*.c`; \
	do ${STRIP} -o ${ROOT}/examples/openssl-testsuite/$$f ${DIR}/test/$$f; done
	cp ${SOURCE_DIR}/openssl-runtest ${ROOT}/examples/openssl-testsuite/runtest
endif
	- ln -s ${DIR} openssl

${DIR}/Configure:
	tar -zxf ${STORAGE}/${PKG}-${VERSION}.tar.gz
	cd ${DIR} && patch -p0 < ${STORAGE}/openssl.patch

