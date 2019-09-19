.NOTPARALLEL:

DIR=rio-control

.PHONY: all
all: ${DIR}/rio-control install

${DIR}/rio-control: ${SOURCE_DIR}/rio-control.c
	mkdir -p ${DIR}
	${CC} ${CFLAGS} -I ${KERNEL_DIR}/arch/mips/include -g -W -Wall -o $@ ${SOURCE_DIR}/rio-control.c

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/rio-control ${ROOT}/usr/bin/rio-control

