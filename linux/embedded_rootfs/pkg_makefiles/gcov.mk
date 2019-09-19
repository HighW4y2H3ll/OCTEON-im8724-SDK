include ../.config

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
ifdef CONFIG_gcov
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gcov ${ROOT}/usr/bin/
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gcov-dump ${ROOT}/usr/bin/
endif

