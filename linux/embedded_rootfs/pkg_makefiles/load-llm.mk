.NOTPARALLEL:

DIR=load-llm

include ${OCTEON_ROOT}/common-config.mk

CFLAGS = ${TOOLCHAIN_ABI} -W -Wall -I ${OCTEON_ROOT}/target/include \
	${CFLAGS_COMMON_CONFIG} -DUSE_RUNTIME_MODEL_CHECKS=1

.DEFAULT: all

.PHONY: all
all: ${DIR}/load_llm_nrw ${DIR}/load_llm_wide install

${DIR}/load_llm_nrw: ${SOURCE_DIR}/load_llm.c ${SOURCE_DIR}/config/cvmx-config.h
	mkdir -p ${DIR}
	${CC} ${CFLAGS} -I ${SOURCE_DIR}/config -o $@ ${SOURCE_DIR}/load_llm.c ${OCTEON_ROOT}/executive/cvmx-llm.c ${OCTEON_ROOT}/executive/cvmx-sysinfo.c $(OCTEON_ROOT)/executive/cvmx-warn.c $(OCTEON_ROOT)/executive/cvmx-clock.c $(OCTEON_ROOT)/executive/octeon-feature.c

${DIR}/load_llm_wide: ${SOURCE_DIR}/load_llm.c ${SOURCE_DIR}/config/cvmx-config.h
	mkdir -p ${DIR}
	${CC} ${CFLAGS} -I ${SOURCE_DIR}/config -DLLM64 -o $@ ${SOURCE_DIR}/load_llm.c ${OCTEON_ROOT}/executive/cvmx-llm.c ${OCTEON_ROOT}/executive/cvmx-sysinfo.c $(OCTEON_ROOT)/executive/cvmx-warn.c $(OCTEON_ROOT)/executive/cvmx-clock.c $(OCTEON_ROOT)/executive/octeon-feature.c

${SOURCE_DIR}/config/cvmx-config.h: ${SOURCE_DIR}/config/executive-config.h
	cd ${SOURCE_DIR} ; cvmx-config $?


.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${DIR}/load_llm_nrw ${ROOT}/usr/bin/load_llm_nrw
	cp ${DIR}/load_llm_wide ${ROOT}/usr/bin/load_llm_wide


