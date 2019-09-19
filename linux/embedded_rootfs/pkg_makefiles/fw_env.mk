.NOTPARALLEL:

DIR=fw_env
SRC_DIR=${SOURCE_DIR}/${DIR}

.PHONY: all
all: build install

.PHONY: build
build:
	${MAKE} -C ${SRC_DIR}

.PHONY: install
install:
	mkdir -p ${ROOT}/sbin
	${STRIP} -o ${ROOT}/sbin/fw_printenv ${SRC_DIR}/fw_env
	ln -s /sbin/fw_printenv ${ROOT}/sbin/fw_setenv
