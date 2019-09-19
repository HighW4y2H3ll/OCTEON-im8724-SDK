.NOTPARALLEL:

include ../.config
BUILD:=${shell pwd}

unexport PYTHON

DIR:=perf

.PHONY: all
all: build install

build: ${DIR}
	PATH=${BUILD}/usr/bin:${PATH} \
	${MAKE} -C ${KERNEL_DIR}/tools/perf \
	ARCH=mips CROSS_COMPILE=$(CROSS)- OUTPUT=${BUILD}/${DIR}/ \
	NO_LIBPERL=1 NO_LIBPYTHON=1 NO_NEWT=1 NO_GTK2=1 WERROR=0 \
	KERNEL_DIR=${KERNEL_DIR} \
	EXTRA_CFLAGS="$(TOOLCHAIN_ABI) \
		-I${BUILD}/elfutils-0.155/usr/include -L${BUILD}/elfutils-0.155/usr/lib \
		-I${BUILD}/libunwind-1.1/usr/include -L${BUILD}/libunwind-1.1/usr/lib \
		-ldw -lelf -lunwind \
		-I../lib -I../lib/traceevent \
		"

.PHONY: install
install: ${DIR}
	cp ${DIR}/perf ${ROOT}/usr/bin

${DIR}:
	mkdir -p ${DIR}
