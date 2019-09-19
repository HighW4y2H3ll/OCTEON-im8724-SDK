
ARCH ?= mips
export ARCH


#
# Build the 64 bit kernel
#
.PHONY: linux
linux: linux/.config
	${MAKE} -C linux vmlinux.64 modules

#
# Build the modules only
#
.PHONY: modules
modules: linux/.config
	${MAKE} -C linux modules

#
# Choose the config based on the board type
#
linux/.config:
	${error Kernel not configured}

#
# Menuconfig
#
menuconfig:
	${MAKE} -C linux $@

#
# Clean the kernel sources to force a rebuild
#
.PHONY: clean
clean:
	${MAKE} -C linux clean

#
# Clean everything
#
.PHONY: distclean
distclean:
	${MAKE} -C linux mrproper

