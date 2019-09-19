.NOTPARALLEL:

include ../.config

PKG:=i2c-tools
VERSION:=3.1.2
DIR:=${PKG}-${VERSION}

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR}

TOOL_FILES	:= i2cdetect i2cdump i2cget i2cset

.PHONY: install
install: ${DIR}
	sudo /bin/cp ${DIR}/eeprom/decode-dimms ${ROOT}/usr/bin/
	sudo /bin/cp ${DIR}/tools/i2cdetect ${ROOT}/usr/bin/
	sudo /bin/cp ${DIR}/tools/i2cdump ${ROOT}/usr/bin/
	sudo /bin/cp ${DIR}/tools/i2cget ${ROOT}/usr/bin/
	sudo /bin/cp ${DIR}/tools/i2cset ${ROOT}/usr/bin/
	sudo /bin/mknod ${ROOT}/dev/i2c-0 c 89 0
	sudo /bin/mknod ${ROOT}/dev/i2c-1 c 89 1

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
