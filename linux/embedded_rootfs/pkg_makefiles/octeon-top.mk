.NOTPARALLEL:
include ../.config

OCT_TOP_SRC=${SOURCE_DIR}/oct-top

.PHONY: install
install:
	mkdir -p ${ROOT}/usr/bin
	cp ${OCT_TOP_SRC}/oct-top ${ROOT}/usr/bin/
	cp ${OCT_TOP_SRC}/dwarfdump ${ROOT}/usr/bin/dwarfdump
