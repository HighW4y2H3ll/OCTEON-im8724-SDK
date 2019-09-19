.NOTPARALLEL:

ARCH ?= mips
export ARCH

CROSS_COMPILE ?= $(CROSS)-
export CROSS_COMPILE

DEPMOD_PROGRAM = module-init-tools-host/depmod
BB_DEPMOD_PROGRAM = busybox/examples/depmod.pl

.PHONY: all
all: build install

.PHONY: build
build:
	${MAKE} -C ${KERNEL_DIR} modules

.PHONY: install
install:
	mkdir -p ${ROOT}/lib/modules/${KERNEL_VERSION}
	if [ "`find ${KERNEL_DIR} -name "*.ko"`" != "" ]; then \
	cd ${KERNEL_DIR} && find . -name "*.ko" | xargs cp -f --parents --target-directory=${ROOT}/lib/modules/${KERNEL_VERSION}; fi
	if [ -x ${DEPMOD_PROGRAM} ] ; then ${DEPMOD_PROGRAM} -b ${ROOT} ${KERNEL_VERSION} ; \
	elif [ -x $(BB_DEPMOD_PROGRAM) ] ; then $(BB_DEPMOD_PROGRAM) -b ${ROOT}/lib/modules -F ${KERNEL_DIR}/System.map ; \
	fi

