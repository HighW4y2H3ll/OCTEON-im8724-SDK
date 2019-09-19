.NOTPARALLEL:

include ../.config


DIR=airflow
SRC_DIR=${SOURCE_DIR}/${DIR}

.PHONY: all
all: build install

.PHONY: build
build:
	${MAKE} -C ${SRC_DIR}

.PHONY: install
install:
	mkdir -p ${ROOT}/sbin
	cp ${SRC_DIR}/airflow ${ROOT}/sbin/airflow
