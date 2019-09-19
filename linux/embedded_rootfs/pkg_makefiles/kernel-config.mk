include ../.config

.PHONY: all
all:
	echo "Copying ${CFG_KERNEL_CONFIG_FILE} to ${KERNEL_DIR}/.config"
	cp ${CFG_KERNEL_CONFIG_FILE} ${KERNEL_DIR}/.config
	${MAKE} -s -C ${KERNEL_DIR} modules_prepare

