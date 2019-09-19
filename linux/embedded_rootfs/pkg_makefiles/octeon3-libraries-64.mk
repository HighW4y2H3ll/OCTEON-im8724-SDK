
.PHONY: install
install:
	mkdir -p ${ROOT}/lib64-fp
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/lib64-fp; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/lib64-fp/; done
	${STRIP} -g -S -d ${ROOT}/lib64-fp/*.so
	mkdir -p ${ROOT}/usr/lib64-fp
	cd ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/lib64-fp; for shared in `find . -maxdepth 1 -name "*.so*"`; do cp -d --parents $$shared ${ROOT}/usr/lib64-fp/; done
