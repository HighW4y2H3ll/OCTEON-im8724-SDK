.NOTPARALLEL:

PKG:=mtd-utils
VERSION:=2.0.0
DIR:=${PKG}-${VERSION}

LIBSEARCH=-L${shell pwd}/zlib-1.2.3 -L${shell pwd}/lzo-2.03/src/.libs/  -L${shell pwd}/e2fsprogs-1.42.6/lib/

BINARIES = ftl_format flash_erase nanddump doc_loadbios \
	ftl_check mkfs.jffs2 flash_lock flash_unlock flash_speed flash_stress \
	flash_otp_info  flash_otp_dump mtd_debug flashcp nandwrite jffs2dump \
	nftldump nftl_format docfdisk \
	sumtool #jffs2reader

UBI_BINS = \
	ubiupdatevol ubimkvol ubirmvol ubicrc32 ubinfo ubiattach \
	ubidetach ubinize ubiformat ubirename mtdinfo ubirsvol \
	ubiblock

.PHONY: all
all: build install

.PHONY: build
build: ${DIR}
	cd ${DIR} && WITHOUT_XATTR=1 CROSS=${CROSS}- CFLAGS="${CFLAGS} -I ${ROOT}/usr/include -I ../lzo*/include" LDFLAGS="${CFLAGS} ${LIBSEARCH}" ./configure --host=${CROSS}
	${MAKE} -C ${DIR}

.PHONY: install
install: ${BINARIES:%=%-install} ${UBI_BINS:%=%-install}

.PHONY: ${BINARIES:%=%-install}
${BINARIES:%=%-install}:
	cp ${DIR}/${@:%-install=%} ${ROOT}/usr/bin/${@:%-install=%}
	cp ${DIR}/misc-utils/flash_eraseall  ${ROOT}/usr/bin/

.PHONY: ${UBI_BINS:%=%-install}
${UBI_BINS:%=%-install}:
	cp ${DIR}/${@:%-install=%} ${ROOT}/usr/bin/${@:%-install=%}
	cp ${DIR}/mkfs.ubifs  ${ROOT}/usr/bin/

${DIR}:
	tar -jxf ${STORAGE}/${PKG}-${VERSION}.tar.bz2
	#cd ${PKG}-${VERSION} && patch -p0 < ${STORAGE}/${PKG}-ubifs.patch && cd -
	#sed -i "s/make -C .*ubi-utils/#make of ubi-utils disabled/g" ${DIR}/Makefile

