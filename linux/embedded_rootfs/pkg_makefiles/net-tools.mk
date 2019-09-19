.NOTPARALLEL:

include ../.config

PKG:=net-tools
VERSION:=1.60
DIR:=${PKG}-${VERSION}

CONFIG_ANSWERS=n# bool 'Does your system support GNU gettext?' I18N n
CONFIG_ANSWERS+=y# bool 'UNIX protocol family' HAVE_AFUNIX y
CONFIG_ANSWERS+=y# bool 'INET (TCP/IP) protocol family' HAVE_AFINET y
ifdef CFG_ENABLE_IPV6	
CONFIG_ANSWERS+=y# bool 'INET6 (IPv6) protocol family' HAVE_AFINET6 n
else
CONFIG_ANSWERS+=n# bool 'INET6 (IPv6) protocol family' HAVE_AFINET6 n
endif
CONFIG_ANSWERS+=y# bool 'Novell IPX/SPX protocol family' HAVE_AFIPX y
CONFIG_ANSWERS+=y# bool 'Appletalk DDP protocol family' HAVE_AFATALK y
CONFIG_ANSWERS+=n# bool 'AX25 (packet radio) protocol family' HAVE_AFAX25 y
CONFIG_ANSWERS+=y# bool 'NET/ROM (packet radio) protocol family' HAVE_AFNETROM y
CONFIG_ANSWERS+=n# bool 'Rose (packet radio) protocol family' HAVE_AFROSE n
CONFIG_ANSWERS+=n# bool 'X.25 (CCITT) protocol family' HAVE_AFX25 y
CONFIG_ANSWERS+=n# bool 'Econet protocol family' HAVE_AFECONET n
CONFIG_ANSWERS+=n# bool 'DECnet protocol family' HAVE_AFDECnet n
CONFIG_ANSWERS+=n# bool 'Ash protocol family' HAVE_AFASH n
CONFIG_ANSWERS+=y# bool 'Ethernet (generic) support' HAVE_HWETHER y
CONFIG_ANSWERS+=y# bool 'ARCnet support' HAVE_HWARC y
CONFIG_ANSWERS+=y# bool 'SLIP (serial line) support' HAVE_HWSLIP y
CONFIG_ANSWERS+=y# bool 'PPP (serial line) support' HAVE_HWPPP y
CONFIG_ANSWERS+=y# bool 'IPIP Tunnel support' HAVE_HWTUNNEL y
CONFIG_ANSWERS+=n# bool 'STRIP (Metricom radio) support' HAVE_HWSTRIP y
CONFIG_ANSWERS+=n# bool 'Token ring (generic) support' HAVE_HWTR y
CONFIG_ANSWERS+=n# bool 'AX25 (packet radio) support' HAVE_HWAX25 y
CONFIG_ANSWERS+=n# bool 'Rose (packet radio) support' HAVE_HWROSE n
CONFIG_ANSWERS+=y# bool 'NET/ROM (packet radio) support' HAVE_HWNETROM y
CONFIG_ANSWERS+=n# bool 'X.25 (generic) support' HAVE_HWX25 y
CONFIG_ANSWERS+=y# bool 'DLCI/FRAD (frame relay) support' HAVE_HWFR y
CONFIG_ANSWERS+=n# bool 'SIT (IPv6-in-IPv4) support' HAVE_HWSIT n
CONFIG_ANSWERS+=n# bool 'FDDI (generic) support' HAVE_HWFDDI n
CONFIG_ANSWERS+=n# bool 'HIPPI (generic) support' HAVE_HWHIPPI n
CONFIG_ANSWERS+=n# bool 'Ash hardware support' HAVE_HWASH n
CONFIG_ANSWERS+=n# bool '(Cisco)-HDLC/LAPB support' HAVE_HWHDLCLAPB n
CONFIG_ANSWERS+=y# bool 'IrDA support' HAVE_HWIRDA y
CONFIG_ANSWERS+=n# bool 'Econet hardware support' HAVE_HWEC n
CONFIG_ANSWERS+=n# bool 'IP Masquerading support' HAVE_FW_MASQUERADE n
CONFIG_ANSWERS+=n# bool 'Build iptunnel and ipmaddr' HAVE_IP_TOOLS n
CONFIG_ANSWERS+=n# bool 'Build mii-tool' HAVE_MII n	

.PHONY: all
all: build install

.PHONY: build
build: ${DIR} ${DIR}/config.h
	${MAKE} -j1 -C ${DIR} COPTS="-D_GNU_SOURCE ${CFLAGS}" LOPTS="${CFLAGS}"

${DIR}/config.h:
	rm -f ${DIR}/answers
	for answer in ${CONFIG_ANSWERS}; do echo $$answer >> ${DIR}/answers;done
	cat ${DIR}/answers | ${MAKE} -C ${DIR} config

.PHONY: install
install: ${DIR}
	rm -f ${ROOT}/bin/hostname
	rm -f ${ROOT}/sbin/ifconfig
	rm -f ${ROOT}/sbin/route
	rm -f ${ROOT}/bin/netstat
	${MAKE} -j1 -C ${DIR}  COPTS="-D_GNU_SOURCE ${CFLAGS}" LOPTS="${CFLAGS}" BASEDIR=${ROOT} install
	rm -f ${ROOT}/sbin/rarp # rarp is no longer supported under Linux

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
	cd ${DIR} && patch -p0 < ${STORAGE}/net-tools-fix-default.patch
	
