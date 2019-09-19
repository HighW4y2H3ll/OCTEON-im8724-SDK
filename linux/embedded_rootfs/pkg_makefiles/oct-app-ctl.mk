include ../.config
.NOTPARALLEL:

DIR=${shell pwd}/oct-app-ctl

.PHONY: all
ifdef OCTEON_LE
all:
else
all: ${DIR}/oct-app-ctl install
endif

${DIR}/oct-app-ctl:
	mkdir -p ${DIR}
	${MAKE} -C ${SOURCE_DIR}/oct-app-ctl TARGET_DIR=${DIR} all

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	- cp ${DIR}/oct-app-ctl ${ROOT}/usr/bin/oct-app-ctl
	- ln -f -s ./oct-app-ctl ${ROOT}/usr/bin/oct-app-boot
	- ln -f -s ./oct-app-ctl ${ROOT}/usr/bin/oct-app-info
	- ln -f -s ./oct-app-ctl ${ROOT}/usr/bin/oct-app-add
	- ln -f -s ./oct-app-ctl ${ROOT}/usr/bin/oct-app-del
	- ln -f -s ./oct-app-ctl ${ROOT}/usr/bin/oct-app-shutdown

