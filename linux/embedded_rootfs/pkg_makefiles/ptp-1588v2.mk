PKG:=ptp-1588v2
DIR:=${PKG}

.PHONY: all
all: ${ROOT}/usr/bin/ptp-1588v2

${DIR}/ptp-1588v2: ${SOURCE_DIR}/ptp-1588v2/*.c ${SOURCE_DIR}/ptp-1588v2/*.h
	mkdir -p ${DIR}
	${MAKE} -C ${SOURCE_DIR}/ptp-1588v2 CC="${CC}" CFLAGS="${CFLAGS}" OUTPUT_DIR=${shell pwd}/${DIR}

${ROOT}/usr/bin/ptp-1588v2: ${DIR}/ptp-1588v2
	mkdir -p ${ROOT}/usr/bin
	cp $^ $@

