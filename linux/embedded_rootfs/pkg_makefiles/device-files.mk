
.PHONY: install
install:
	sudo mkdir -m 0755 ${ROOT}/dev
	sudo mknod -m 0600 ${ROOT}/dev/console c 5 1
	sudo mknod -m 0666 ${ROOT}/dev/null c 1 3
