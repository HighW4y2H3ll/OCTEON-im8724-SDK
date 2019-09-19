
.PHONY: install
install:
	mkdir -p ${ROOT}/lib64
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/lib64; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/lib64/; done
	${STRIP} -g -S -d ${ROOT}/lib64/*.so
	mkdir -p ${ROOT}/usr/lib64
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/lib64; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/usr/lib64/; done
