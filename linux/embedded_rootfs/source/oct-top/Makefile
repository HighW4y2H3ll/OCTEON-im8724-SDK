.NOTPARALLEL:

UTILSDIR=${OCTEON_ROOT}/host/remote-utils
DIR=/tmp/octeon-remote-utils
CROSS=mips64-octeon-linux-gnu

.PHONY: all 
all: ${DIR} build install 

${DIR}:
	mkdir -p ${DIR}
build:
	${MAKE} -C ${OCTEON_ROOT}/host/remote-lib DIR=${DIR} CROSS=${CROSS}- 
	${MAKE} -C ${OCTEON_ROOT}/host/remote-utils DIR=${DIR} CROSS=${CROSS}- 

install: 
	cp ${DIR}/oct-remote-profile $(TARGET_DIR) 
	cp -f oct-top $(TARGET_DIR)
	cp -f dwarfdump $(TARGET_DIR)/dwarfdump
	cp -f oct-linux-profile $(TARGET_DIR)
