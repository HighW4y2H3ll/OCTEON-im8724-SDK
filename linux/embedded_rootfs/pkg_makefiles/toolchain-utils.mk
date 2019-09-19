include ../.config

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
ifdef TOOLCHAIN_UTILS_GPROF
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/gprof ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_LDD
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/ldd ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_OBJDUMP
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/objdump ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_NM
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/nm ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_READELF
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/readelf ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_STRINGS
	rm -f ${ROOT}/usr/bin/strings
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/strings ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_ADDR2LINE
	rm -f ${ROOT}/usr/bin/addr2line
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/addr2line ${ROOT}/usr/bin/
endif	
ifdef TOOLCHAIN_UTILS_GDB_SIMPLE_EXE
	rm -f ${ROOT}/usr/bin/mipsisa64-octeon-elf-gdb
	cp ${TOOLCHAIN_DIR}/${CROSS}/sys-root/usr/bin/mipsisa64-octeon-elf-gdb ${ROOT}/usr/bin/
endif

