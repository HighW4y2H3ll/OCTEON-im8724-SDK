export SHELL=/bin/bash
DISK=/dev/OVERRIDE_ME
MOUNT_POINT=/mnt/${USER}/linux_dev
KERNEL_VERSION=${shell make -s -C ../kernel/linux --no-print-directory kernelrelease ARCH=mips CROSS_COMPILE=mips64-octeon-linux-gnu-}
ifdef OCTEON_LE
TOOLCHAIN=${OCTEON_ROOT}/tools-le/mips64el-octeon-linux-gnu
TOOLS_INSTALL_DIR=usr/local/Cavium_Networks/OCTEON-SDK/tools-le
REPLACE_N32=as c++ cc g++ gcc mips64el-octeon-linux-gnu-gcc-?.?.?
COMPILER_PREFIX=${OCTEON_ROOT}/tools-le/bin/mips64el-octeon-linux-gnu
BUILD=build-le
else
TOOLCHAIN=${OCTEON_ROOT}/tools/mips64-octeon-linux-gnu
TOOLS_INSTALL_DIR=usr/local/Cavium_Networks/OCTEON-SDK/tools
REPLACE_N32=as c++ cc g++ gcc mips64-octeon-linux-gnu-gcc-?.?.?
COMPILER_PREFIX=${OCTEON_ROOT}/tools/bin/mips64-octeon-linux-gnu
BUILD=build
endif

# To allow flexibility in rebuild of debian-octeon tarballs, we accumulate
# patches for trivial infrastructure changes (/etc/xxx tweaks).
# Some are already applied in BE-or-LE tarballs, so patch in a forgiving
# manner, ignoring already-applied chunks, and leaving no .rej  files.
# This way same patch list can be used in both-endian
PATCH = patch -N -r- -p0

EMBEDDED_ROOTFS=../embedded_rootfs

.SILENT: default
.PHONY: default
default:
	echo "This Makefile will setup an Octeon Debian root"
	echo "filesystem on a compact flash. All information"
	echo "on the compact flash will be erased. In order"
	echo "to execute the Makefile, perform the following"
	echo "command with ROOT privileges:"
	echo ""
	echo "    make DISK=/dev/XXX compact-flash"
	echo ""
	echo "Replace XXX with the name of your compact flash"
	echo "device. This is normally 'sda'. Make sure you use"
	echo "the correct device."
	echo "ALL INFORMATION ON THIS DEVICE WILL BE ERASED."
	echo ""


MOUNT_PART1=mkdir -p ${MOUNT_POINT} && mount ${DISK}1 ${MOUNT_POINT}
MOUNT_PART2=mkdir -p ${MOUNT_POINT} && mount ${DISK}2 ${MOUNT_POINT}
UMOUNT=sync && umount ${MOUNT_POINT} && rmdir ${MOUNT_POINT}

.PHONY: compact-flash
compact-flash: env-check safety-banner partition mkfs kernel debian toolchain toolchain32 octeon-utils toolchain-o3

.PHONY: compact-flash-le
compact-flash-le: env-check safety-banner partition mkfs kernel debian-le toolchain toolchain32 octeon-utils toolchain-o3

.PHONY: target-dir
target-dir:  MOUNT_POINT=${shell pwd}/target-dir
target-dir:  MOUNT_PART1=mkdir -p ${MOUNT_POINT}
target-dir:  MOUNT_PART2=mkdir -p ${MOUNT_POINT}
target-dir:  UMOUNT=

ifdef OCTEON_LE
target-dir: env-check clean kernel debian-le toolchain toolchain32 toolchain-o3 octeon-utils
	tar cf - ${MOUNT_POINT} | cksum > ${MOUNT_POINT}/checksum
else
target-dir: env-check clean kernel debian toolchain toolchain32 toolchain-o3 octeon-utils
	tar cf - ${MOUNT_POINT} | cksum > ${MOUNT_POINT}/checksum
endif

.PHONY: env-check
.SILENT: env-check
env-check:
	if [ -d "${OCTEON_ROOT}" ]; then true; else echo "OCTEON_ROOT is not set!"; false; fi

.PHONY: safety-banner
.SILENT: safety-banner
ifdef OCTEON_LE
safety-banner: ../kernel/linux/vmlinux.64 debian_octeon-le.tgz
else
safety-banner: ../kernel/linux/vmlinux.64 debian_octeon.tgz
endif
	echo "**************************************************"
	echo "About to erase everything on ${DISK}."
	fdisk -l ${DISK} | grep Disk
	echo "You have 5 seconds to hit Control-C to stop this."
	echo "**************************************************"
	if [ `sfdisk -s ${DISK}` -gt 16200000 ]; then echo "Refusing to destroy disk ${DISK}, larger than 16GB!" && false; fi
	sleep 5

.PHONY: partition
partition:
	sync
	umount ${MOUNT_POINT} ${DISK}1 ${DISK}2 >/dev/null 2>&1 || true
	sync
	dd if=/dev/zero of=${DISK} bs=512 count=10
	sync
	echo -e "n\np\n1\n\n+64M\nt\n6\nn\np\n2\n\n\nw\n" | fdisk ${DISK} ||\
		( : workaround ubuntu-14.10 fdisk problem; \
		partprobe; d=`echo ${DISK} | sed s,/dev/,,`; \
		[ 3 = `grep -c ' '$$d /proc/partitions` ]; \
		)

.PHONY: mkfs
mkfs:
	sync
	umount ${MOUNT_POINT} ${DISK}1 ${DISK}2 >/dev/null 2>&1 || true
	mkfs.vfat ${DISK}1
	mke2fs -q -j ${DISK}2

.PHONY: kernel
kernel: ../kernel/linux/vmlinux.64
	sync
	umount ${MOUNT_POINT} ${DISK}1 ${DISK}2 >/dev/null 2>&1 || true
	${MOUNT_PART1}
	${COMPILER_PREFIX}-strip -o ${MOUNT_POINT}/vmlinux.64 ../kernel/linux/vmlinux.64
	${UMOUNT}

.PHONY: debian
debian: debian_octeon.tgz
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	tar -C ${MOUNT_POINT} -zxf debian_octeon.tgz
	mkdir -p ${MOUNT_POINT}/lib/modules/${KERNEL_VERSION}/
	if [ "`find ../kernel/linux/ -name "*.ko"`" != "" ]; then cd ../kernel/linux/ && find . -name "*.ko" | xargs cp --parents --target-directory=${MOUNT_POINT}/lib/modules/${KERNEL_VERSION}; fi
	# Rename the name of the ethernet driver that needs to be loaded
	# at boot time.
	mknod ${MOUNT_POINT}/dev/mtdblock0 b 31 0
	# Etch doesn't supply ttyS0 devices, so we may need to add them
	rm -f ${MOUNT_POINT}/dev/ttyS0
	mknod ${MOUNT_POINT}/dev/ttyS0 c 4 64
	rm -f ${MOUNT_POINT}/dev/ttyS1
	mknod ${MOUNT_POINT}/dev/ttyS1 c 4 65
	# and the pci-console tty, for serial-free bringup
	rm -f ${MOUNT_POINT}/dev/ttyPCI0
	mknod ${MOUNT_POINT}/dev/ttyPCI0 c 4 96
	# Real Time Clock is also missing
	rm -f ${MOUNT_POINT}/dev/rtc
	mknod ${MOUNT_POINT}/dev/rtc c 254 0
	# Etch doesn't supply sd? devices, so we may need to add them
	let minor=0; \
	for dev in a b c d e f g h; do \
		mknod ${MOUNT_POINT}/dev/sd$$dev b 8 $$minor; \
		let minor=minor+1; \
		for ((i=1;i<16;i++)); do \
			mknod ${MOUNT_POINT}/dev/sd$$dev$$i b 8 $$minor; \
			let minor=minor+1; \
		done; \
	done
	# /dev/cfa* has incorrect major numbers.
	rm -f ${MOUNT_POINT}/dev/cfa*
	mknod ${MOUNT_POINT}/dev/cfa b 252 0
	for ((i=1;i<5;i++)); do \
		mknod ${MOUNT_POINT}/dev/cfa$$i b 252 $$i; \
	done
	# Fix compact flash mounts in /etc/fstab
	sed -i 's/cfa/sda/g' ${MOUNT_POINT}/etc/fstab
	${UMOUNT}


PHONY: debian-le
debian-le: debian_octeon-le.tgz
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	tar -C ${MOUNT_POINT} -zxf debian_octeon-le.tgz
	- ${PATCH} -d ${MOUNT_POINT}/etc < ntp-ntpstats.patch
	- ${PATCH} -d ${MOUNT_POINT}/etc < inittab.patch
	- ${PATCH} -d ${MOUNT_POINT}/etc/init.d < module-init-tools-depmod.patch
	- ${PATCH} -d ${MOUNT_POINT}/etc/init.d < ntp-ntpdate.patch
	mkdir -p ${MOUNT_POINT}/lib/modules/${KERNEL_VERSION}/
	if [ "`find ../kernel/linux/ -name "*.ko"`" != "" ]; then cd ../kernel/linux/ && find . -name "*.ko" | xargs cp --parents --target-directory=${MOUNT_POINT}/lib/modules/${KERNEL_VERSION}; fi
	${UMOUNT}


.PHONY: toolchain
toolchain:
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	rm -fr ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}
	rm -f ${MOUNT_POINT}/lib32
	rm -f ${MOUNT_POINT}/lib64
	rm -f ${MOUNT_POINT}/usr/lib32
	rm -f ${MOUNT_POINT}/usr/lib64
	# Etch has stuff in /usr/lib/gcc. Don't delete it
	#rm -f ${MOUNT_POINT}/usr/lib/gcc
	mkdir -p ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	cp -R ${TOOLCHAIN}/sys-root/usr 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	- find ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/ -name stdc++.h.gch | xargs rm -r
	cp -R ${TOOLCHAIN}/sys-root/lib32 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	cp -R ${TOOLCHAIN}/sys-root/lib64 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	ln -s /${TOOLS_INSTALL_DIR}/lib32 ${MOUNT_POINT}/lib32
	ln -s /${TOOLS_INSTALL_DIR}/lib64 ${MOUNT_POINT}/lib64
	ln -s /${TOOLS_INSTALL_DIR}/usr/lib32 ${MOUNT_POINT}/usr/lib32
	ln -s /${TOOLS_INSTALL_DIR}/usr/lib64 ${MOUNT_POINT}/usr/lib64
	ln -s /${TOOLS_INSTALL_DIR}/usr/lib/gcc ${MOUNT_POINT}/usr/lib/gcc

.PHONY: toolchain32
toolchain32:
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	rm -rf ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr/bin32
	mkdir ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr/bin32
	cd ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr/bin32 && ln -s ../bin/* .
	if [ -e ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/tools/usr/bin/gdbn32 ]; then \
	  cd ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr/bin32; \
	  ln -sf ../bin/gdbservern32 gdbserver; \
	  ln -sf ../bin/gdbn32 gdb;  cd -; fi
	cd ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr/bin32; \
		for f in ${REPLACE_N32}; do \
			rm $$f && echo -e "#!/bin/bash\n/${TOOLS_INSTALL_DIR}/usr/bin/$$f -mabi=n32 \"\$$@\"" > $$f; \
			chmod +x $$f; \
		done
	${COMPILER_PREFIX}-gcc -mabi=n32 -fPIC -shared -o ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/lib32/libhtlbremap.so ${EMBEDDED_ROOTFS}/source/htlbremap/{*.c,*.S}
	${UMOUNT}

.PHONY: toolchain-o3
toolchain-o3:
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	rm -fr ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/lib64-fp
	rm -fr ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/lib32-fp
	rm -f ${MOUNT_POINT}/lib32-fp
	rm -f ${MOUNT_POINT}/lib64-fp
	rm -f ${MOUNT_POINT}/usr/lib32-fp
	rm -f ${MOUNT_POINT}/usr/lib64-fp
	# Etch has stuff in /usr/lib/gcc. Don't delete it
	#rm -f ${MOUNT_POINT}/usr/lib/gcc
	mkdir -p ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	cp -R ${TOOLCHAIN}/sys-root/usr/lib64-fp 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr
	cp -R ${TOOLCHAIN}/sys-root/usr/lib32-fp 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/usr
	cp -R ${TOOLCHAIN}/sys-root/lib64-fp 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	cp -R ${TOOLCHAIN}/sys-root/lib32-fp 	${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/
	- find ${MOUNT_POINT}/${TOOLS_INSTALL_DIR}/ -name stdc++.h.gch | xargs rm -r
	ln -s /${TOOLS_INSTALL_DIR}/lib32-fp ${MOUNT_POINT}/lib32-fp
	ln -s /${TOOLS_INSTALL_DIR}/lib64-fp ${MOUNT_POINT}/lib64-fp
	ln -s /${TOOLS_INSTALL_DIR}/usr/lib32-fp ${MOUNT_POINT}/usr/lib32-fp
	ln -s /${TOOLS_INSTALL_DIR}/usr/lib64-fp ${MOUNT_POINT}/usr/lib64-fp

.PHONY: octeon-utils
octeon-utils:
	sync
	umount ${MOUNT_POINT} >/dev/null 2>&1 || true
	${MOUNT_PART2}
	if [ -e ${EMBEDDED_ROOTFS}/.config ] ; then echo "Overwriting ,config file under ${EMBEDDED_ROOTFS}" ; fi;
ifdef OCTEON_LE
	cat ${EMBEDDED_ROOTFS}/default.config | \
		sed "s/TOOLCHAIN_BIG_ENDIAN=y/#TOOLCHAIN_BIG_ENDIAN is not set/g" | \
		sed "s/# TOOLCHAIN_LITTLE_ENDIAN is not set/TOOLCHAIN_LITTLE_ENDIAN=y/g" > ${EMBEDDED_ROOTFS}/.config
else
	cp ${EMBEDDED_ROOTFS}/default.config ${EMBEDDED_ROOTFS}/.config
endif
	# Remove old perf and rebuild
	-rm -rf ${EMBEDDED_ROOTFS}/${BUILD}/perf
	make -C ${EMBEDDED_ROOTFS} flex_host m4_host bison_host
	make -C ${EMBEDDED_ROOTFS} libelf ROOT=${MOUNT_POINT}/${TOOLS_INSTALL_DIR}
	make -C ${EMBEDDED_ROOTFS} libunwind ROOT=${MOUNT_POINT}/${TOOLS_INSTALL_DIR}
	make -C ${EMBEDDED_ROOTFS} perf ROOT=${MOUNT_POINT}

	# Remove old kexec-tools and rebuild
	-rm -rf ${EMBEDDED_ROOTFS}/${BUILD}/kexec-tools
	make -C ${EMBEDDED_ROOTFS} kexec-tools ROOT=${MOUNT_POINT}

	# Remove old oct-remote utils and rebuild
	-rm -rf ${EMBEDDED_ROOTFS}/${BUILD}/octeon-remote-utils
	make -C ${EMBEDDED_ROOTFS} octeon-remote-utils ROOT=${MOUNT_POINT}

	# Remove old schedtool and rebuild.
	-rm -rf ${EMBEDDED_ROOTFS}/${BUILD}/schedtool*
	make -C ${EMBEDDED_ROOTFS} schedtool ROOT=${MOUNT_POINT}

ifndef OCTEON_LE
	# Install valgrind
	make -C ${EMBEDDED_ROOTFS} valgrind ROOT=${MOUNT_POINT}
	# Remove old bootoct and oct-shutdown. And rebuild.
	#-rm -rf ${EMBEDDED_ROOTFS}/build/bootoct
	#make -C ${EMBEDDED_ROOTFS} bootoct ROOT=${MOUNT_POINT}
	#-rm -rf ${EMBEDDED_ROOTFS}/build/oct-shutdown-app
	#make -C ${EMBEDDED_ROOTFS} oct-shutdown-app ROOT=${MOUNT_POINT}
	# Rebuild oct-app-ctl which replaced bootoct and oct-shutdown-app
	-rm -rf ${EMBEDDED_ROOTFS}/build/oct-app-ctl
	make -C ${EMBEDDED_ROOTFS} oct-app-ctl ROOT=${MOUNT_POINT}
	# Install oct-top
	make -C ${EMBEDDED_ROOTFS}/source/oct-top TARGET_DIR=${MOUNT_POINT}/usr/bin all
endif
	${UMOUNT}

.PHONY: clean
clean:
	rm -rf ${MOUNT_POINT}


