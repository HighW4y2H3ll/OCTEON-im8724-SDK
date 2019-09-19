include ../.config

.PHONY: install
install:
	rm -f ${ROOT}/usr/bin/strings
	cp -ar ${TOOLCHAIN_DIR}/${CROSS}/sys-root/* ${ROOT}/
ifndef CONFIG_octeon-libraries-n32
ifndef CONFIG_octeon2-libraries-n32
	rm -Rf ${ROOT}/lib32
	rm -Rf ${ROOT}/usr/lib32
endif
endif
ifndef CONFIG_octeon2-libraries-64
	rm -Rf ${ROOT}/lib64/octeon2
	rm -Rf ${ROOT}/usr/lib64/octeon2
endif
	rm -Rf ${ROOT}/lib64/libbfd.a
	rm -Rf ${ROOT}/lib64/libiberty.a
