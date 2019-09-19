.NOTPARALLEL:

include ../.config


DIR=htlbremap
SRC_DIR=${SOURCE_DIR}/${DIR}

.PHONY: all
all: ${SRC_DIR}/*
	mkdir -p ${DIR}
	# If the libraries are present under ${SRC_DIR}, the embedded rootfs
	# skips building this library because of VPATH.
	${MAKE} -C ${SRC_DIR} clean
	${MAKE} -C ${DIR} -f ${SRC_DIR}/Makefile VPATH=${SRC_DIR}
ifdef TOOLCHAIN_ABI_64
	cp ${DIR}/libhtlbremap.so ${ROOT}/${LIBDIR}/libhtlbremap.so
endif
ifdef TOOLCHAIN_ABI_N32
	cp ${DIR}/libhtlbremap-n32.so ${ROOT}/${LIBDIR}/libhtlbremap.so
endif

