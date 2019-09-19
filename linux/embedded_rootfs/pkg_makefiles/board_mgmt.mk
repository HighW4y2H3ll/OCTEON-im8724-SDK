.NOTPARALLEL:

include ../.config


DIR=board_mgmt
SRC_DIR=${SOURCE_DIR}/${DIR}

.PHONY: all
all: build install

.PHONY: build
build:
	${MAKE} -C ${SRC_DIR}

.PHONY: install
install:
	mkdir -p ${ROOT}/sbin
	cp ${SRC_DIR}/board_mgmt ${ROOT}/sbin/board_mgmt
	cp ${SRC_DIR}/sfp_info ${ROOT}/bin/sfp_info
