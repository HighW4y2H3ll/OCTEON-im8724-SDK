.NOTPARALLEL:

include ../.config

PKG:=busybox
VERSION:=1.26.2
DIR:=${PKG}-${VERSION}
BUSYBOX_LINKS=`xargs < ${DIR}/busybox.links`
TESTSUITE=${ROOT}/examples/busybox-testsuite

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR} CC="${CC}" CFLAGS="${TOOLCHAIN_ABI}" LDFLAGS="${TOOLCHAIN_ABI}" busybox.links
	${MAKE} -C ${DIR} CC="${CC}" CFLAGS="${TOOLCHAIN_ABI}" LDFLAGS="${TOOLCHAIN_ABI}"

.PHONY: install
install: ${DIR}
	mkdir -p ${ROOT}/bin
	mkdir -p ${ROOT}/sbin
	mkdir -p ${ROOT}/usr/bin
	mkdir -p ${ROOT}/usr/sbin
	mkdir -p ${ROOT}/proc
	mkdir -p ${ROOT}/sys
	mkdir -p ${ROOT}/mnt
	mkdir -p ${ROOT}/mnth
	mkdir -p ${ROOT}/tmp
	mkdir -p ${ROOT}/var
	mkdir -p ${ROOT}/etc
	mkdir -p ${ROOT}/root
	mkdir -p ${ROOT}/lib/modules
	mkdir -p ${ROOT}/home
	cp ${DIR}/busybox ${ROOT}/bin/busybox
	for link in ${BUSYBOX_LINKS}; do ln -s /bin/busybox ${ROOT}$$link; done
	# The following link is needed for initramfs based boots
	ln -s /sbin/init ${ROOT}/init
	cp ${SOURCE_DIR}/cpu-load ${ROOT}/usr/bin/
	cp ${SOURCE_DIR}/net-load ${ROOT}/usr/bin/
ifdef CFG_BUSYBOX_TESTSUITE
	mkdir -p ${ROOT}/examples
	cp ${DIR}/.config ${ROOT}/examples/
	cp -R ${DIR}/testsuite ${TESTSUITE}
	cp ${SOURCE_DIR}/busybox-runtest ${TESTSUITE}/runtest
	rm -r ${TESTSUITE}/wget					# Networking may not be up
	rm -r ${TESTSUITE}/du					# Doesn't test anything
	rm ${TESTSUITE}/mv/mv-files-to-dir			# Unsupported touch option
	rm ${TESTSUITE}/mv/mv-refuses-mv-dir-to-subdir		# Unsupported touch option
	rm ${TESTSUITE}/touch/touch-touches-files-after-non-existent-file			# touch -t doesn't work
	rm -r ${TESTSUITE}/hostname
	rm ${TESTSUITE}/echo/echo-prints-slash*		# Unsupported od option
	rm ${TESTSUITE}/echo/echo-prints-dash		# Unsupported od option
	rm ${TESTSUITE}/echo/echo-prints-non-opts	# Unsupported od option
	rm ${TESTSUITE}/cp/cp-does-not-copy-unreadable-file  # id not enabled
	sed -i "s:../../busybox:/bin/busybox:g" ${TESTSUITE}/strings/strings-works-like-GNU	# Fix busybox path
	sed -i "s:../../busybox:/bin/busybox:g" ${TESTSUITE}/expand/expand-works-like-GNU	# Fix busybox path
	sed -i "s:../../busybox:/bin/busybox:g" ${TESTSUITE}/unexpand/unexpand-works-like-GNU	# Fix busybox path
	sed -i "s:BUSYBOX=.*:BUSYBOX=/bin/busybox:" ${TESTSUITE}/which/which-uses-default-path	# Fix busybox path
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/tail/tail-n-works
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/tail/tail-works
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/tar/tar_with_link_with_size
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/tar/tar_with_prefix_fields
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/cut/cut-cuts-a-field
	sed -i "s/\$$ECHO/echo/g" ${TESTSUITE}/tr/tr-works
endif

${DIR}:
	tar -xf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
	ln -s ${DIR} busybox
	cp ${STORAGE}/busybox.config ${DIR}/.config
ifdef OCTEON_LE
	sed -i 's/mips64/mips64el/g' ${DIR}/.config
endif
	sed -i "s/.NOTPARALLEL: .depend/#.NOTPARALLEL: .depend/g" ${DIR}/Makefile
ifeq (${MAKE_VERSION},3.79.1)
	# The busybox makefile uses the "|" operator to signal dependencies
	# which isn't supported by older make versions. Here we force the
	# build at once option for this old version of make. This allows us
	# to bypass the stuff make has trouble with. This is needed on
	# Redhat 9.
	sed -i "s/# CONFIG_BUILD_AT_ONCE is not set/CONFIG_BUILD_AT_ONCE=y/g" ${DIR}/.config
endif
ifdef TOOLCHAIN_ABI_64
	# Disable large file system support for n64 ABI.
	sed -i "s/CONFIG_LFS=y/# CONFIG_LFS is not set/" ${DIR}/.config
endif
	${MAKE} -C ${DIR} oldconfig > /dev/null

