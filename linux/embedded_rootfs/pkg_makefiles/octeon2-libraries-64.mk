include ../.config

.PHONY: install
install:
	mkdir -p ${ROOT}/lib64/octeon2
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/lib64; for shared in `find octeon2 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/lib64/; done
	${STRIP} -g -S -d ${ROOT}/lib64/octeon2/*.so
	mkdir -p ${ROOT}/usr/lib64/octeon2
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/lib64; for shared in `find octeon2 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/usr/lib64/; done
ifndef CONFIG_octeon-libraries-64
	# Move ld.so.1 from octeon2 directory to /lib64.
	mv ${ROOT}/lib64/octeon2/ld*.so* ${ROOT}/lib64
else
	# Remove ld.so.1 from octeon2 directories, it is not used.
	- rm ${ROOT}/lib64/octeon2/ld*.so*
endif

