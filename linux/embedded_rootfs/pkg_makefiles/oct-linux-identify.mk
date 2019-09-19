.NOTPARALLEL:

DIR=oct-linux-identify

.PHONY: all
all: ${DIR}/oct-linux-identify install

${DIR}/oct-linux-identify: ${SOURCE_DIR}/oct-linux-identify.c
	mkdir -p ${DIR}
	${CC} ${CFLAGS} -g -W -Wall -DUSE_RUNTIME_MODEL_CHECKS=1 -I${OCTEON_ROOT}/target/include -I${SOURCE_DIR} -o $@ ${SOURCE_DIR}/oct-linux-identify.c ${OCTEON_ROOT}/executive/octeon-model.c ${OCTEON_ROOT}/executive/cvmx-sysinfo.c ${OCTEON_ROOT}/executive/cvmx-warn.c ${OCTEON_ROOT}/executive/cvmx-clock.c ${OCTEON_ROOT}/executive/octeon-feature.c

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/oct-linux-identify ${ROOT}/usr/bin/oct-linux-identify

