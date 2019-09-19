.NOTPARALLEL:

-include .config

BASE:=${shell pwd}
ifdef TOOLCHAIN_LITTLE_ENDIAN
TMP_NAME:=/tmp/le/${subst /,_,${BASE}}
BUILD:=${BASE}/build-le
else
TMP_NAME:=/tmp/${subst /,_,${BASE}}
BUILD:=${BASE}/build
endif
MOUNT_POINT:=${TMP_NAME}-mount

######################################################################
# These variables are exported to package makes to make life easier
######################################################################

export STORAGE=${BASE}/storage
export SOURCE_DIR=${BASE}/source
export ETC_FILES=${BASE}/etc-files
export ROOT=${TMP_NAME}-rootfs
ifdef TOOLCHAIN_LITTLE_ENDIAN
export CROSS=mips64el-octeon-linux-gnu
export OCTEON_LE=1
else
export CROSS=mips64-octeon-linux-gnu
endif
export CC=${CROSS}-gcc
export CXX=${CROSS}-g++
export LD=${CROSS}-ld
export AR=${CROSS}-ar
export RANLIB=${CROSS}-ranlib
export STRIP=${CROSS}-strip
export CFLAGS=${TOOLCHAIN_ABI} -Os -fomit-frame-pointer
export CXXFLAGS=${TOOLCHAIN_ABI} -Os -fomit-frame-pointer

unexport CROSS_COMPILE

ifdef OCTEON_LINUX_ROOT
  export KERNEL_DIR=${OCTEON_LINUX_ROOT}/kernel/linux/.
else
  export KERNEL_DIR=${BASE}/../kernel/linux/.
endif

export KERNEL_VERSION=${shell if [ -e ${KERNEL_DIR}/.config ]; then ARCH=mips CROSS_COMPILE=${CROSS}- ${MAKE} -s -C ${KERNEL_DIR} --no-print-directory kernelrelease; fi}

ifdef OCTEON_TOOLS_ROOT
  export TOOLCHAIN_DIR=${OCTEON_TOOLS_ROOT}
else
  # Ugly way to get where the tools are
  # 1) Use "which" to get the full path to the compiler
  # 2) Use "patsubst" to remove /bin/${CROSS}-gcc
  export TOOLCHAIN_DIR=${patsubst %/bin/${CROSS}-gcc,%,${shell which ${CROSS}-gcc}}
endif

ISA_LIB=
OCTEON_ISA=octeon

ifdef TOOLCHAIN_ABI_N32_OCTEON
  export TOOLCHAIN_ABI_N32=1
endif

ifdef TOOLCHAIN_ABI_N32_OCTEON2
  export TOOLCHAIN_ABI_N32=1
  ISA_LIB=/octeon2
  OCTEON_ISA=octeon2
endif

ifdef TOOLCHAIN_ABI_N32_OCTEON3
  export TOOLCHAIN_ABI_N32=1
  ISA_LIB=-fp
  OCTEON_ISA=octeon3
endif

ifdef TOOLCHAIN_ABI_64_OCTEON
  export TOOLCHAIN_ABI_64=1
endif

ifdef TOOLCHAIN_ABI_64_OCTEON2
  export TOOLCHAIN_ABI_64=1
  ISA_LIB=/octeon2
  OCTEON_ISA=octeon2
endif

ifdef TOOLCHAIN_ABI_64_OCTEON3
  export TOOLCHAIN_ABI_64=1
  ISA_LIB=-fp
  OCTEON_ISA=octeon3
endif

export OCTEONISA=${OCTEON_ISA}
export OCTEON_EXTRA_CFLAGS=-march=${OCTEON_ISA}
export OCTEON_LIBDIR64=lib64${ISA_LIB}
export OCTEON_LIBDIR32=lib32${ISA_LIB}

ifdef TOOLCHAIN_ABI_N32
  export TOOLCHAIN_ABI=-mabi=n32 ${OCTEON_EXTRA_CFLAGS}
  #export LDFLAGS=-melf32btsmipn32
  export LIBDIR=/usr/${OCTEON_LIBDIR32}
endif

ifdef TOOLCHAIN_ABI_64
  export TOOLCHAIN_ABI=-mabi=64 ${OCTEON_EXTRA_CFLAGS}
#  export LDFLAGS=-Wl,-melf64btsmip
  export LIBDIR=/usr/${OCTEON_LIBDIR64}
endif

######################################################################
# List of all modules *.ko in kernel
######################################################################
MODULES_ORDER=${shell cat ${KERNEL_DIR}/modules.order}
LINUX_KERNEL_MODULES:=$(subst kernel,$(KERNEL_DIR),$(MODULES_ORDER))

######################################################################
# Create a list of all possible packages from all the *.mk files
######################################################################

ALL_PACKAGES_MK=${shell find ${BASE}/pkg_makefiles -name "*.mk" -printf "%f "}
ALL_PACKAGES=${ALL_PACKAGES_MK:.mk=}

######################################################################
# Create a list of the configured packages from the .config file
######################################################################

PACKAGES=${shell if [ -e .config ]; then grep CONFIG_.*=y .config | sed -e "s/CONFIG_//" -e "s/=y//";fi}

######################################################################
# Default target - Give the user some help
######################################################################

.DEFAULT: help
.SILENT: help
.PHONY: help
help:
	echo
	echo
	echo "menuconfig        - Configure the packages for the filesystem (GUI)"
	echo "config            - Configure the packages for the filesystem (Command line)"
	echo "oldconfig         - Check the existing config file"
	echo "all               - Build the filessytem"
	echo "squashfs          - Package the filesystem into Squashfs"
	echo "cramfs            - Package the filesystem into Cramfs"
	echo "ext3              - Package the filesystem into Ext3"
	echo "ext2              - Package the filesystem into Ext2"
	echo "lzma		- Package the filesystem into lzma"
	echo "initramfs         - Use the CPIO archive as an initramfs"
	echo "clean             - Delete the builds and all filesystem files"
	echo "clean-root        - Delete the filesystem files leaving the builds"
	echo "distclean         - Delete all generated files, including the config"
	echo
	echo "Currently configured packages:"
	echo ${PACKAGES}
	echo

######################################################################
# The all target does the actual building of the packages and creation
# of the filesystem. The .config file must have already been
# generated. The list of packaged was created earlier by searching for
# CONFIG-*=y in the configuration file.
######################################################################

.PHONY: all
all: configured .root_complete

.root_complete: .config ${BASE}/pkg_kconfig/*.kconfig ${BASE}/pkg_makefiles/*.mk ${KERNEL_DIR}/.config
	${MAKE} force-build

.PHONY: force-build
force-build: configured oldconfig clean-root ${PACKAGES}
	touch .root_complete

.SILENT: configured
.PHONY: configured
configured:
	if [ -e .config ]; then true; else echo "Not configured. Run \"make menuconfig\"" && false; fi

.PHONY: ${ALL_PACKAGES}
${ALL_PACKAGES}: ${ROOT} ${BUILD}
	@echo "**** $@"
	${MAKE} -C ${BUILD} -f ${BASE}/pkg_makefiles/$@.mk ${PKG_TARGET}

# Force dependencies so kernel-modules can run depmod.
ifeq (,$(findstring ${MAKE_VERSION}, 3.79.1))
ifneq (,$(findstring module-init-tools, $(PACKAGES)))
kernel-modules: | module-init-tools
else
kernel-modules: | busybox
endif
endif

######################################################################
# Targets for cleaning up
######################################################################

.PHONY: clean
clean: clean-root
	rm -rf ${BUILD} .kconfig_include .config.* .tmpconfig.h
	$(MAKE) CC=gcc CFLAGS=-Os LDFLAGS="" -C config clean

.PHONY: clean-root
clean-root:
	rm -f .root_complete rootfs.cpio rootfs.cpio.gz rootfs.ext3 rootfs.ext2 rootfs.squashfs rootfs.cramfs rootfs.lzma
	sudo rm -rf ${ROOT}

.PHONY: distclean
distclean: clean
	cp default.config .config
	${MAKE} -s oldconfig

${BUILD} ${ROOT}:
	mkdir -p $@
	chmod 777 $@

######################################################################
# Targets for supporting menuconfig
######################################################################

config/conf config/mconf: .kconfig_include config/Makefile
	$(MAKE) CC=gcc CFLAGS=-Os LDFLAGS="" -C config ncurses conf mconf

.PHONY: menuconfig
menuconfig: config/mconf
	@./config/mconf embedded_rootfs.kconfig

.PHONY: config
config: config/conf
	@./config/conf embedded_rootfs.kconfig

.PHONY: oldconfig
oldconfig: config/conf
	@./config/conf -s embedded_rootfs.kconfig

.PHONY: .kconfig_include
.kconfig_include: ${BASE}/pkg_kconfig/*.kconfig
	find ${BASE}/pkg_kconfig -name "*.kconfig" -printf "source \"pkg_kconfig/%f\"\n" | sort > $@

######################################################################
# Targets for packaging the filesystem
######################################################################

update-files: ${LINUX_KERNEL_MODULES}
	cd ${KERNEL_DIR} && find . -iname '*.ko' | \
	xargs -r sudo cp --parents -f --target-directory=${ROOT}/lib/modules/${KERNEL_VERSION}

${BASE}/rootfs.cpio: update-files
	cd ${ROOT} && sudo find . | sudo cpio --format=newc -oF ${ROOT}.cpio
	sudo chmod a+r ${ROOT}.cpio
	cp ${ROOT}.cpio ${BASE}/rootfs.cpio
	sudo /bin/rm -f ${ROOT}.cpio

${BASE}/rootfs.cpio.gz: ${BASE}/rootfs.cpio
	gzip -9 < ${BASE}/rootfs.cpio > ${BASE}/rootfs.cpio.gz

.PHONY: squashfs
squashfs:
	mksquashfs ${ROOT} rootfs.squashfs -be -noappend
	chmod -x rootfs.squashfs

.PHONY: lzma
lzma: ${BASE}/rootfs.cpio
	lzma -9 < ${BASE}/rootfs.cpio > ${BASE}/rootfs.cpio.lzma

.PHONY: cramfs
cramfs:
ifdef TOOLCHAIN_LITTLE_ENDIAN
	/sbin/mkfs.cramfs -N little ${ROOT} rootfs.cramfs
else
	/sbin/mkfs.cramfs -N big ${ROOT} rootfs.cramfs
endif

.PHONY: ext3
ext3: ${BASE}/rootfs.cpio
	sudo mkdir -p ${MOUNT_POINT}
	sync
	- grep -q ${MOUNT_POINT} /proc/mounts && sudo umount ${MOUNT_POINT}
	# Create the filesystem as the size of the CPIO plus 16MB
	dd if=/dev/zero of=${ROOT}.ext3 bs=1M count=$$(($$(stat -c %s ${BASE}/rootfs.cpio) / 1048576 + 16))
	/sbin/mke2fs -qjF ${ROOT}.ext3
	sudo mount -o loop ${ROOT}.ext3 ${MOUNT_POINT}
	sudo cp -R ${ROOT}/. ${MOUNT_POINT}/
	sync
	sudo umount ${MOUNT_POINT}
	sudo rmdir ${MOUNT_POINT}
	mv ${ROOT}.ext3 rootfs.ext3

.PHONY: ext2
ext2: ${BASE}/rootfs.cpio
	sudo mkdir -p ${MOUNT_POINT}
	sync
	- grep -q ${MOUNT_POINT} /proc/mounts && sudo umount ${MOUNT_POINT}
	# Create the filesystem as the size of the CPIO plus 16MB
	dd if=/dev/zero of=${ROOT}.ext2 bs=1M count=$$(($$(stat -c %s ${BASE}/rootfs.cpio) / 1048576 + 16))
	/sbin/mke2fs -qF ${ROOT}.ext2
	sudo mount -o loop ${ROOT}.ext2 ${MOUNT_POINT}
	sudo cp -R ${ROOT}/. ${MOUNT_POINT}/
	sync
	sudo umount ${MOUNT_POINT}
	sudo rmdir ${MOUNT_POINT}
ifdef OCTEON_LE
	mv ${ROOT}.ext2 /tmp/rootfs-le.ext2
	rfs_endian_swap /tmp/rootfs-le.ext2 rootfs-le.ext2
	rm -f /tmp/rootfs-le.ext2
else
	mv ${ROOT}.ext2 rootfs.ext2
endif

.PHONY: initramfs	
initramfs: ${BASE}/rootfs.cpio ${BASE}/rootfs.cpio.gz
