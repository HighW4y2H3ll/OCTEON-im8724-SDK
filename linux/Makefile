.NOTPARALLEL:

# override mount-points as needed
MOUNT_POINT=/mnt/cf1
MOUNT_POINT2=/mnt/cf2
# override these empty partition-names, unless they are pre-configured in /etc/fstab
DISKp1=
DISKp2=

TFTPBOOT=/tftpboot

DEFAULT_CROSS_COMPILE = $(shell \
	if grep -q ^CONFIG_CPU_LITTLE_ENDIAN kernel/linux/.config && \
	which mips64el-octeon-linux-gnu-gcc > /dev/null 2>&1 ; then \
		echo -n mips64el-octeon-linux-gnu-; \
	else \
		echo -n mips64-octeon-linux-gnu-; fi)


CROSS_COMPILE ?= $(strip $(DEFAULT_CROSS_COMPILE))

export CROSS_COMPILE
ARCH = mips
export ARCH

STRIP=${CROSS_COMPILE}strip

# Extract kernel version.
# Cannot be :=, because kernel/linux may come into existence _during_ make
# Must be silent on missing kernel/linux
KERNEL_VERSION = ${shell [ ! -d kernel/linux ] || \
	${MAKE} -C kernel/linux --quiet ARCH=mips CROSS_COMPILE=${CROSS_COMPILE} kernelrelease}

######################################################################
# Tell the user what to do
######################################################################
.DEFAULT: help
.PHONY: help
.SILENT: help
help:
	echo "Supply the build target:"
	echo "    kernel               - Build the Linux kernel supporting all Cavium Octeon reference boards"
	echo "    kernel-deb           - Linux kernel without the rootfs"
	echo "    sim                  - Octeon simulation environment"
	echo "    setup-octeon         - Enable config options for running on all OCTEON hardware"
	echo "    setup-octeon-sim     - Enable config options for running on all OCTEON simulation"
	echo "    setup-octeon2        - Enable config options for running only on OCTEON II and later hardware"
	echo "    setup-octeon2-sim    - Enable config options for running only on OCTEON II and later simulation"
	echo "    flash                - Copy kernel onto compact flash at mount ${MOUNT_POINT}"
	echo "    strip                - Strip symbols out of the kernel image"
	echo "    tftp                 - Copy a stripped kernel to ${TFTPBOOT}"
	echo "    test                 - Test an existing simulator build"
	echo "    clean                - Remove all generated files and the KERNEL CONFIG"
	echo
######################################################################
# Kernel build targets
######################################################################

.PHONY: kernel-config sim-config
kernel-config sim-config: \
  %-config:
	cp kernel/$*.config kernel/linux/.config

.PHONY: setup-octeon2
setup-octeon2:
	if [ ! -e kernel/linux/.config ]; then cp kernel/kernel.config kernel/linux/.config; fi
	sed -i 's/# CONFIG_CAVIUM_OCTEON2 is not set/CONFIG_CAVIUM_OCTEON2=y/g' kernel/linux/.config


.PHONY: setup-octeon2-sim
setup-octeon2-sim:
	if [ ! -e kernel/linux/.config ]; then cp kernel/sim.config kernel/linux/.config; fi
	sed -i 's/# CONFIG_CAVIUM_OCTEON2 is not set/CONFIG_CAVIUM_OCTEON2=y/g' kernel/linux/.config

.PHONY: setup-octeon
setup-octeon:
	if [ ! -e kernel/linux/.config ]; then cp kernel/kernel.config kernel/linux/.config; fi
	sed -i 's/^CONFIG_CAVIUM_OCTEON2=y/# CONFIG_CAVIUM_OCTEON2 is not set/g' kernel/linux/.config


.PHONY: setup-octeon-sim
setup-octeon-sim:
	if [ ! -e kernel/linux/.config ]; then cp kernel/sim.config kernel/linux/.config; fi
	sed -i 's/^CONFIG_CAVIUM_OCTEON2=y/# CONFIG_CAVIUM_OCTEON2 is not set/g' kernel/linux/.config


.PHONY: sim
sim: embedded_rootfs/.config 
	if [ ! -e kernel/linux/.config ]; then cp kernel/sim.config kernel/linux/.config; fi
	- rm -f kernel/linux/usr/initramfs_data.cpio*
	- rm kernel/linux/vmlinux
	- find kernel/linux/ -name \*.ko -delete
	sed -i -e '/CONFIG_INITRAMFS_SOURCE/ d' -e '/CONFIG_INITRAMFS_COMPRESSION/ d' kernel/linux/.config
	echo 'CONFIG_INITRAMFS_SOURCE=""' >> kernel/linux/.config
	${MAKE} -C kernel/linux silentoldconfig
	${MAKE} -C kernel/linux modules
	${MAKE} -C embedded_rootfs all ext2
	${MAKE} -C kernel

.PHONY: kernel
kernel: embedded_rootfs/.config
	if [ ! -e kernel/linux/.config ]; then cp kernel/kernel.config kernel/linux/.config; fi
	if ! grep -q -e 'CONFIG_INITRAMFS_SOURCE="../../embedded_rootfs/rootfs.cpio.gz"'  kernel/linux/.config; then \
		sed -i -e '/CONFIG_INITRAMFS_SOURCE/ d' -e '/CONFIG_INITRAMFS_COMPRESSION/ d' \
			-e '/CONFIG_RD_GZIP/ d' -e '/CONFIG_INITRAMFS_ROOT_UID/ d' \
			 -e '/CONFIG_INITRAMFS_ROOT_GID/ d' kernel/linux/.config; \
		echo 'CONFIG_INITRAMFS_SOURCE="../../embedded_rootfs/rootfs.cpio.gz"' >> kernel/linux/.config ; \
		echo 'CONFIG_RD_GZIP=y' >> kernel/linux/.config ; \
		echo '# CONFIG_INITRAMFS_COMPRESSION_NONE is not set' >> kernel/linux/.config ; \
		echo 'CONFIG_INITRAMFS_COMPRESSION_GZIP=y' >> kernel/linux/.config ; \
		echo 'CONFIG_INITRAMFS_ROOT_UID=0' >> kernel/linux/.config ; \
		echo 'CONFIG_INITRAMFS_ROOT_GID=0' >> kernel/linux/.config ; fi
	- rm kernel/linux/vmlinux
	- find kernel/linux/ -name \*.ko -delete
	${MAKE} -C kernel/linux silentoldconfig
	${MAKE} -C kernel/linux modules
	${MAKE} -C embedded_rootfs all initramfs
	${MAKE} -C kernel

.PHONY: kernel_menuconfig
kernel_menuconfig: kernel/linux/.config
	${MAKE} -C kernel/linux menuconfig

.PHONY: kernel-deb
kernel-deb:
	if [ ! -e kernel/linux/.config ]; then cp kernel/kernel.config kernel/linux/.config; fi
	- rm -f kernel/linux/usr/initramfs_data.cpio*
	sed -i -e '/CONFIG_INITRAMFS_SOURCE/ d' -e '/CONFIG_INITRAMFS_COMPRESSION/ d' kernel/linux/.config
	echo 'CONFIG_INITRAMFS_SOURCE=""' >> kernel/linux/.config
	${MAKE} -C kernel

.PHONY: debian
debian: kernel-deb

.PHONY: ebt3000
ebt3000: kernel


######################################################################
# If the embedded filesystem isn't configured, start menuconfig
######################################################################
embedded_rootfs/.config:
	cp embedded_rootfs/default.config embedded_rootfs/.config
	${MAKE} -sC embedded_rootfs oldconfig


######################################################################
# Targets to package the final kernel
######################################################################
.PHONY: flash
flash:
	# This target assumes the compact flash is already setup. Use
	# debian/Makefile to get this setup the first time.
	umount ${MOUNT_POINT} > /dev/null 2>&1 || true
	mount ${DISKp1} ${MOUNT_POINT}
	$(STRIP) -o ${MOUNT_POINT}/vmlinux.64 kernel/linux/vmlinux.64
	sync
	umount ${MOUNT_POINT}
	umount ${MOUNT_POINT2} > /dev/null 2>&1 || true
	mount ${DISKp2} ${MOUNT_POINT2}
	sudo mkdir -p ${MOUNT_POINT2}/lib/modules/${KERNEL_VERSION}
	if [ -e kernel/intercept-example/intercept-example.ko ]; then sudo cp kernel/intercept-example/intercept-example.ko ${MOUNT_POINT2}/lib/modules/${KERNEL_VERSION}/; fi
	if [ "`find kernel/linux/. -name "*.ko"`" != "" ]; then cd kernel/linux && find . -name "*.ko" | xargs sudo cp --parents --target-directory=${MOUNT_POINT2}/lib/modules/${KERNEL_VERSION}; fi
	sync
	umount ${MOUNT_POINT2}

.PHONY: strip
strip:
	$(STRIP) kernel/linux/vmlinux.64

.PHONY: tftp
tftp:
	$(STRIP) -o ${TFTPBOOT}/vmlinux.64 kernel/linux/vmlinux.64

.PHONY: pci
pci:
	oct-pci-reset
	oct-pci-load 0x21000000 kernel/linux/vmlinux.64
	oct-pci-bootcmd "bootoctlinux 21000000 root=/dev/sda2 numcores=16"


######################################################################
# Test the kernel on the simulator
######################################################################
.PHONY: test
test:
	${MAKE} -f Makefile.test


######################################################################
# Clean out everything
######################################################################
.PHONY: clean
clean:
	${MAKE} -C kernel/intercept-example clean
	${MAKE} -f Makefile.test clean
	echo "Removing the kernel config!"
	${MAKE} -C kernel distclean
	echo "Removing the Embedded Rootfs config!"
	${MAKE} -C embedded_rootfs distclean

