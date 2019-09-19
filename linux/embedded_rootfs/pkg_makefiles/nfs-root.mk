include ../.config

INIT_FILE=${ROOT}/init

.PHONY: install
install:
	rm -f ${INIT_FILE}
	echo "#!/bin/sh" >> ${INIT_FILE}
	echo "echo Updating module dependencies" >> ${INIT_FILE}
	echo "depmod -a" >> ${INIT_FILE}
	echo "echo Loading ethernet driver" >> ${INIT_FILE}
	echo "modprobe octeon-ethernet" >> ${INIT_FILE}
ifdef NFS_ROOT_USE_DHCP
	echo "echo Getting a DHCP lease" >> ${INIT_FILE}
	echo "udhcpc -i ${NFS_ROOT_NET_DEVICE}" >> ${INIT_FILE}
else
	echo "echo Configuring ${NFS_ROOT_NET_DEVICE} ip ${NFS_ROOT_IP} netmask ${NFS_ROOT_NETMASK} broadcast ${NFS_ROOT_BROADCAST}" >> ${INIT_FILE}
	echo "ifconfig ${NFS_ROOT_NET_DEVICE} ${NFS_ROOT_IP} netmask ${NFS_ROOT_NETMASK} broadcast ${NFS_ROOT_BROADCAST}" >> ${INIT_FILE}
ifneq (${NFS_ROOT_GATEWAY},linux_64)
	echo "echo Adding default route to ${NFS_ROOT_GATEWAY}" >> ${INIT_FILE}
	echo "route add default gw ${NFS_ROOT_GATEWAY}" >> ${INIT_FILE}
endif	
endif
	echo "mkdir -p /rootfs" >> ${INIT_FILE}
	echo "echo Mounting ${NFS_ROOT_SERVER}" >> ${INIT_FILE}
	echo "mount -t nfs -o ${NFS_ROOT_OPTIONS} ${NFS_ROOT_SERVER} /rootfs" >> ${INIT_FILE}
	echo "cd /rootfs" >> ${INIT_FILE}
	echo "echo Performing chroot" >> ${INIT_FILE}
	echo "exec /usr/sbin/chroot . /sbin/init" >> ${INIT_FILE}
	chmod +x ${INIT_FILE}


