include ../.config

.PHONY: install
install:
	mkdir -p ${ROOT}/lib32/octeon2
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/lib32; for shared in `find octeon2 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/lib32/; done
	${STRIP} -g -S -d ${ROOT}/lib32/octeon2/*.so
	mkdir -p ${ROOT}/usr/lib32/octeon2
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/lib32; for shared in `find octeon2 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/usr/lib32/; done
ifndef CONFIG_octeon-libraries-n32
	# Move ld.so.1 from octeon2 directory to /lib32.
	mv ${ROOT}/lib32/octeon2/ld*.so* ${ROOT}/lib32
else
	# Remove ld.so.1 from octeon2 directories, it is not used.
	- rm ${ROOT}/lib32/octeon2/ld*.so*
endif
