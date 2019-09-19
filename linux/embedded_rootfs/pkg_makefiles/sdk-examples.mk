include ../.config

ifdef TOOLCHAIN_ABI_N32
  ABI=linux_n32
endif
ifdef TOOLCHAIN_ABI_64
  ABI=linux_64
endif

OCTEONLE=
ifdef OCTEON_LE
  OCTEONLE=el
endif

DIR=${shell pwd}/sdk-examples

.PHONY: all
all: install

ifdef SDK_EXAMPLES_CRYPTO
install: build_SDK_EXAMPLES_CRYPTO
.PHONY: build_SDK_EXAMPLES_CRYPTO
build_SDK_EXAMPLES_CRYPTO:
	#${MAKE} -C ${OCTEON_ROOT}/examples/crypto OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/crypto OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-crypto
	${MAKE} -C ${OCTEON_ROOT}/examples/crypto OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-crypto TARGET=${DIR}-crypto/crypto-${ABI}${OCTEONLE}
endif
ifdef SDK_EXAMPLES_NAMED_BLOCK
install: build_SDK_EXAMPLES_NAMED_BLOCK
.PHONY: build_SDK_EXAMPLES_NAMED_BLOCK
build_SDK_EXAMPLES_NAMED_BLOCK:
	#${MAKE} -C ${OCTEON_ROOT}/examples/named-block OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/named-block OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-named-block
	${MAKE} -C ${OCTEON_ROOT}/examples/named-block OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-named-block TARGET=${DIR}-named-block/named-block-${ABI}${OCTEONLE}
endif
ifdef SDK_EXAMPLES_PASSTHROUGH
install: build_SDK_EXAMPLES_PASSTHROUGH
.PHONY: build_SDK_EXAMPLES_PASSTHROUGH
build_SDK_EXAMPLES_PASSTHROUGH:
	#${MAKE} -C ${OCTEON_ROOT}/examples/passthrough OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/passthrough OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-passthrough
	${MAKE} -C ${OCTEON_ROOT}/examples/passthrough OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-passthrough TARGET=${DIR}-passthrough/passthrough-${ABI}${OCTEONLE}
endif

ifdef SDK_EXAMPLES_PASSTHROUGH_O3
install: build_SDK_EXAMPLES_PASSTHROUGH_O3
.PHONY: build_SDK_EXAMPLES_PASSTHROUGH_O3
build_SDK_EXAMPLES_PASSTHROUGH_O3:
	${MAKE} -C ${OCTEON_ROOT}/examples/passthrough_o3 INTF_TO_USE=sgmii0  OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-passthrough_o3 TARGET=${DIR}-passthrough_o3/passthrough_o3-${ABI}${OCTEONLE}
endif

ifdef SDK_EXAMPLES_LINUX_FILTER
install: build_SDK_EXAMPLES_LINUX_FILTER
.PHONY: build_SDK_EXAMPLES_LINUX_FILTER
build_SDK_EXAMPLES_LINUX_FILTER:
	#${MAKE} -C ${OCTEON_ROOT}/examples/linux-filter OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/linux-filter OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-linux-filter
	${MAKE} -C ${OCTEON_ROOT}/examples/linux-filter OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-linux-filter TARGET=${DIR}-linux-filter/linux-filter-${ABI}${OCTEONLE}
endif
ifdef SDK_EXAMPLES_LOW_LATENCY_MEM
install: build_SDK_EXAMPLES_LOW_LATENCY_MEM
.PHONY: build_SDK_EXAMPLES_LOW_LATENCY_MEM
build_SDK_EXAMPLES_LOW_LATENCY_MEM:
	#${MAKE} -C ${OCTEON_ROOT}/examples/low-latency-mem OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/low-latency-mem OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-low-latency-mem
	${MAKE} -C ${OCTEON_ROOT}/examples/low-latency-mem OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-low-latency-mem TARGET=${DIR}-low-latency-mem/low-latency-mem-${ABI}${OCTEONLE}
endif
ifdef SDK_EXAMPLES_ZIP
install: build_SDK_EXAMPLES_ZIP
.PHONY: build_SDK_EXAMPLES_ZIP
build_SDK_EXAMPLES_ZIP:
	#${MAKE} -C ${OCTEON_ROOT}/examples/zip OCTEON_TARGET=${ABI} clean
	${MAKE} -C ${OCTEON_ROOT}/examples/zip OCTEON_TARGET=${ABI} config/cvmx-config.h OCTEON_SINGLE_DIR=${DIR}-zip
	${MAKE} -C ${OCTEON_ROOT}/examples/zip OCTEON_TARGET=${ABI} OCTEON_SINGLE_DIR=${DIR}-zip TARGET=${DIR}-zip/zip-${ABI}${OCTEONLE}
endif

.PHONY: install
install:
	mkdir -p ${ROOT}/examples
ifdef SDK_EXAMPLES_CRYPTO
	cp ${DIR}-crypto/crypto-${ABI}${OCTEONLE} ${ROOT}/examples/crypto
endif
ifdef SDK_EXAMPLES_NAMED_BLOCK
	cp ${DIR}-named-block/named-block-${ABI}${OCTEONLE} ${ROOT}/examples/named-block
endif
ifdef SDK_EXAMPLES_PASSTHROUGH
	cp ${DIR}-passthrough/passthrough-${ABI}${OCTEONLE} ${ROOT}/examples/passthrough
endif
ifdef SDK_EXAMPLES_PASSTHROUGH_O3
	- cp ${DIR}-passthrough_o3/passthrough_o3-${ABI}${OCTEONLE} ${ROOT}/examples/passthrough_o3
endif
ifdef SDK_EXAMPLES_LINUX_FILTER
	cp ${DIR}-linux-filter/linux-filter-${ABI}${OCTEONLE} ${ROOT}/examples/linux-filter
endif
ifdef SDK_EXAMPLES_LOW_LATENCY_MEM
	cp ${DIR}-low-latency-mem/low-latency-mem-${ABI}${OCTEONLE} ${ROOT}/examples/low-latency-mem
endif
ifdef SDK_EXAMPLES_ZIP
	cp ${DIR}-zip/zip-${ABI}${OCTEONLE} ${ROOT}/examples/zip
endif
