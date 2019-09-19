
.PHONY: install
install:
	mkdir -p ${ROOT}/lib32
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/lib32; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/lib32/; done
	${STRIP} -g -S -d ${ROOT}/lib32/*.so
	mkdir -p ${ROOT}/usr/lib32
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/lib32; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/usr/lib32/; done
