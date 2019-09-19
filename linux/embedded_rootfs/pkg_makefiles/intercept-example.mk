.NOTPARALLEL:

.PHONY: all
all: build install

.PHONY: build
build:
	${MAKE} -C ${KERNEL_DIR}/../intercept-example

.PHONY: install
install:
	mkdir -p ${ROOT}/lib/modules/${KERNEL_VERSION}
	cp ${KERNEL_DIR}/../intercept-example/intercept-example.ko ${ROOT}/lib/modules/${KERNEL_VERSION}/

