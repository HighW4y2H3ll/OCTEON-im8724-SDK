.NOTPARALLEL:

include ../.config

PKG:=lm_sensors
VERSION:=3.4.0
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR}

TOOL_FILES	:= 

.PHONY: install
install: ${DIR}
	sudo /bin/cp ${DIR}/prog/sensors/sensors ${ROOT}/usr/bin/
	/bin/cp ${DIR}/lib/libsensors.so.4.4.0 ${ROOT}/lib64/
	ln -s /lib64/libsensors.so.4.4.0 ${ROOT}/lib64/libsensors.so.4
	ln -s /lib64/libsensors.so.4.4.0 ${ROOT}/lib64/libsensors.so

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
