
.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	if [ -e ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gdb64 ]; then \
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gdb*64 ${ROOT}/usr/bin/; \
	else cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gdb* ${ROOT}/usr/bin/; fi

