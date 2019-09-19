.NOTPARALLEL:

DIR=oct-linux-mdio
CFLAGS += -DUSE_RUNTIME_MODEL_CHECKS=1

.PHONY: all
all: ${DIR}/oct-linux-mdio install

${DIR}/oct-linux-mdio: ${SOURCE_DIR}/oct-linux-mdio.c
	mkdir -p ${DIR}
	${CC} ${CFLAGS} -W -Wall -DUSE_RUNTIME_MODEL_CHECKS=1 -I ${OCTEON_ROOT}/executive -I${SOURCE_DIR} -o $@ ${SOURCE_DIR}/oct-linux-mdio.c ${OCTEON_ROOT}/executive/cvmx-sysinfo.c ${OCTEON_ROOT}/executive/cvmx-warn.c ${OCTEON_ROOT}/executive/cvmx-clock.c ${OCTEON_ROOT}/executive/octeon-feature.c 

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/oct-linux-mdio ${ROOT}/usr/bin/oct-linux-mdio

