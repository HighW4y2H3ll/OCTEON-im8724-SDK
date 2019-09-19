.NOTPARALLEL:

include ../.config

PKG:=cav_sh
DIR:=${PKG}

.PHONY: all
all: build install

${DIR}:
	mkdir ${DIR}
	cp $(SOURCE_DIR)/busybox-util/Makefile ${DIR}
	cp $(SOURCE_DIR)/busybox-util/cav_sh.c ${DIR}

.PHONY: build
build: ${DIR}
	${MAKE} -C ${DIR}


.PHONY: install
install:
	cp $(DIR)/$(PKG)		$(ROOT)/bin/
	cp ${ETC_FILES}/inittab 	${ROOT}/etc/
	cp ${ETC_FILES}/fstab 		${ROOT}/etc/
	cp ${ETC_FILES}/rc    		${ROOT}/sbin/
	cp ${ETC_FILES}/passwd		${ROOT}/etc/
	cp ${ETC_FILES}/group		${ROOT}/etc/
	cp ${ETC_FILES}/shadow		${ROOT}/etc/
	cp ${ETC_FILES}/gshadow		${ROOT}/etc/
	cp ${ETC_FILES}/protocols	${ROOT}/etc/
	cp ${ETC_FILES}/hosts		${ROOT}/etc/
	cp ${ETC_FILES}/host.conf	${ROOT}/etc/
	ln -s /proc/mounts ${ROOT}/etc/mtab
	mkdir -p ${ROOT}/usr/share/udhcpc/
	cp ${ETC_FILES}/udhcpc.script   ${ROOT}/usr/share/udhcpc/
	cp ${ETC_FILES}/udhcpc.renew    ${ROOT}/usr/share/udhcpc/
	cp ${ETC_FILES}/udhcpc.nak      ${ROOT}/usr/share/udhcpc/
	cp ${ETC_FILES}/udhcpc.deconfig ${ROOT}/usr/share/udhcpc/
	cp ${ETC_FILES}/udhcpc.bound    ${ROOT}/usr/share/udhcpc/
	ln -s udhcpc.script		${ROOT}/usr/share/udhcpc/default.script
	cp ${ETC_FILES}/udhcpd.conf     ${ROOT}/etc/
	mkdir -p ${ROOT}/var/run
	mkdir -p ${ROOT}/var/lib/misc
	touch    ${ROOT}/var/lib/misc/udhcpd.leases

