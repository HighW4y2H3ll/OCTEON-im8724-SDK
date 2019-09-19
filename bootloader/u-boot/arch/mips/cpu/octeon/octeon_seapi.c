/*
 * (C) Copyright 2015
 * Cavium, Inc, <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* This file contains an API similar to the U-Boot native API in order for
 * Simple Executive applications to call specific functions in U-Boot.
 */

#include <common.h>
#include <malloc.h>
#include <fs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/processor.h>
#include <asm/mipsregs.h>
#include <api_public.h>
#include <asm/arch/seapi_public.h>
#include <part.h>
#ifdef CONFIG_OCTEON_QLM
# include <asm/arch/octeon_qlm.h>
#endif
#if defined(CONFIG_CMD_USB) && defined(CONFIG_USB_STORAGE)
# include <usb.h>
#endif

/** Size of the bounce buffer to use for SE API calls */
#ifdef CONFIG_OCTEON_SEAPI_BOUNCE_BUFFER_SIZE
# define BOUNCE_BUFFER_SIZE	CONFIG_OCTEON_SEAPI_BOUNCE_BUFFER_SIZE
#else
# define BOUNCE_BUFFER_SIZE	65536
#endif

DECLARE_GLOBAL_DATA_PTR;

typedef int (*se_cfp_t)(uint64_t sig_addr, int call, uint64_t retval_addr,
			uint64_t arg0, uint64_t arg1, uint64_t arg2,
			uint64_t arg3, uint64_t arg4);

/** Spinlock around all SE API calls, used by assembly code. */
cvmx_spinlock_t seapi_lock = { OCTEON_SPINLOCK_UNLOCKED_VAL };

/** Local pointer to signature */
struct cvmx_seapi_signature *seapi_sig;

/** Local pointer to call table */
static  se_cfp_t *call_table = NULL;

/** Local pointer to bounce buffer */
static uint8_t *bounce_buffer = NULL;
/** XKPHYS address of bounce buffer */
static uint64_t bounce_buffer_addr = 0;
/** Used for ENUM, not implemented yet */
#if defined(CONFIG_CMD_IDE) || defined(CONFIG_CMD_MMC) || defined(CONFIG_CMD_SATA) || defined(CONFIG_CMD_USB) || defined(CONFIG_USB_STORAGE)
static struct cvmx_storage_spec specs[cvmx_storage_max] =
	{ { 0, 0, 0, cvmx_storage_ide, ""}, };
#endif
/** Size of bounce buffer */
static uint32_t bounce_buffer_size = BOUNCE_BUFFER_SIZE;

/** SE native API assembly code entry */
extern int uboot_se_napi_entry(struct cvmx_seapi_signature *sig, int call,
			       uint64_t retval_addr, void *va, ...);

extern int fs_write(const char *filename, ulong addr, int offset, int len);

/** Convert U-Boots storage type to a string */
static const char *storage_dev_type_to_str(enum cvmx_seapi_storage_dev_type type)
{
	switch (type) {
	case CVMX_STORAGE_DEV_IDE:
		return "ide";
	case CVMX_STORAGE_DEV_SCSI:
		return "scsi";
	case CVMX_STORAGE_DEV_USB:
		return "usb";
	case CVMX_STORAGE_DEV_MMC:
		return "mmc";
	case CVMX_STORAGE_DEV_SATA:
		return "sata";
	default:
		return NULL;
	}
}

/** Get the storage type based on the cookie value */
static enum cvmx_storage_type
get_storage_type_from_cookie(const void *cookie)
{
	struct block_dev_desc *dd = (struct block_dev_desc *)cookie;
	switch (dd->if_type) {
	case IF_TYPE_IDE:
		return cvmx_storage_ide;
	case IF_TYPE_SCSI:
		return cvmx_storage_max;
	case IF_TYPE_ATAPI:
		return cvmx_storage_ide;
	case IF_TYPE_USB:
		return cvmx_storage_usb;
	case IF_TYPE_MMC:
	case IF_TYPE_SD:
		return cvmx_storage_mmc;
	case IF_TYPE_SATA:
	case IF_TYPE_SATA_PMP:
		return cvmx_storage_sata;
	default:
		return -CVMX_STORAGE_DEV_UNKNOWN;
	}
}

/** Copy the SDI data structure to the SE application */
static void copy_sdi_to_se(uint64_t se_sdi_addr,
			   struct cvmx_seapi_device_info *sdi)
{
	uint64_t sdi_addr = MAKE_XKPHYS(cvmx_ptr_to_phys(sdi));
	memcpy64(MAKE_XKPHYS(se_sdi_addr), sdi_addr, sizeof(*sdi));
}

/** Copy the SDI data structure from the SE application */
static void copy_sdi_from_se(struct cvmx_seapi_device_info *sdi,
			    uint64_t se_sdi_addr)
{
	uint64_t sdi_addr = MAKE_XKPHYS(cvmx_ptr_to_phys(sdi));
	memcpy64(sdi_addr, MAKE_XKPHYS(se_sdi_addr), sizeof(*sdi));
}

void SEAPI_stor_init(void)
{
#ifdef CONFIG_CMD_IDE
	specs[cvmx_storage_ide].max_dev = CONFIG_SYS_IDE_MAXDEVICE;
	specs[cvmx_storage_ide].enum_started = 0;
	specs[cvmx_storage_ide].enum_ended = 0;
	specs[cvmx_storage_ide].type = cvmx_storage_ide;
	specs[cvmx_storage_ide].name = "ide";
#endif
#ifdef CONFIG_CMD_MMC
	specs[cvmx_storage_mmc].max_dev = CONFIG_SYS_MMC_MAX_DEVICE;
	specs[cvmx_storage_mmc].enum_started = 0;
	specs[cvmx_storage_mmc].enum_ended = 0;
	specs[cvmx_storage_mmc].type = cvmx_storage_mmc;
	specs[cvmx_storage_mmc].name = "mmc";
#endif
#ifdef CONFIG_CMD_SATA
	specs[cvmx_storage_sata].max_dev = CONFIG_SYS_SATA_MAX_DEVICE;
	specs[cvmx_storage_sata].enum_started = 0;
	specs[cvmx_storage_sata].enum_ended = 0;
	specs[cvmx_storage_sata].type = cvmx_storage_sata;
	specs[cvmx_storage_sata].name = "sata";
#endif
#if defined(CONFIG_CMD_USB) && defined(CONFIG_USB_STORAGE)
	specs[cvmx_storage_usb].max_dev = USB_MAX_STOR_DEV;
	specs[cvmx_storage_usb].enum_started = 0;
	specs[cvmx_storage_usb].enum_ended = 0;
	specs[cvmx_storage_usb].type = cvmx_storage_usb;
	specs[cvmx_storage_usb].name = "usb";
#endif
}

/**
 * Open a storage device
 *
 * @param	sig_addr	Signature physical address
 * @param	call		Should be CVMX_SEAPI_DEV_OPEN
 * @param	retval_addr	Not used
 * @param	arg0		Physical address of storage device info
 * @param	arg1		Storage device info physical address
 * @param	arg2		Not used
 * @param	arg3		Not used
 * @param	arg4		Not used
 *
 * @return	0 if successful, otherwise error.
 *
 * NOTE: This must be called after USB is started for USB devices and before
 * any read/write operations.
 */
static int SEAPI_dev_open(uint64_t sig_addr, int call, uint64_t retval_addr,
			  uint64_t arg0, uint64_t arg1, uint64_t arg2,
			  uint64_t arg3, uint64_t arg4)
{
	struct cvmx_seapi_device_info sdi;
	uint64_t sdi_addr;
	struct cvmx_seapi_storage_dev_info dinfo;
	uint64_t dinfo_addr;
	char *if_type_str;
	block_dev_desc_t *dd;
	disk_partition_t part;
	int ret;

	dinfo_addr = arg0;
	if (!dinfo_addr)
		return -CVMX_SEAPI_EINVAL;

	memcpy64(MAKE_XKPHYS(cvmx_ptr_to_phys(&dinfo)),
		 MAKE_XKPHYS(dinfo_addr), sizeof(dinfo));

	switch (dinfo.if_type) {
	case CVMX_STORAGE_DEV_IDE:
		if_type_str = "ide";
		break;
	case CVMX_STORAGE_DEV_SCSI:
		if_type_str = "scsi";
		break;
	case CVMX_STORAGE_DEV_USB:
		if_type_str = "usb";
		break;
	case CVMX_STORAGE_DEV_MMC:
		if_type_str = "mmc";
		break;
	case CVMX_STORAGE_DEV_SATA:
		if_type_str = "sata";
		break;
	default:
		debug("Unknown interface type %d\n", dinfo.if_type);
		return -CVMX_SEAPI_EINVAL;
	}

	dd = get_dev(if_type_str, dinfo.index);
	if (!dd || dd->type == DEV_TYPE_UNKNOWN) {
		debug("%s: bad device\n", __func__);
		return -CVMX_SEAPI_ENODEV;
	}

	ret = get_partition_info(dd, dinfo.part, &part);
	if (ret) {
		debug("%s: Could not get disk partition info for %s %d:%d\n",
		      __func__, if_type_str, dinfo.index, dinfo.part);
		return -CVMX_SEAPI_ENODEV;
	}

	/** Allocate or grow the bounce buffer if needed */
	if (dinfo.buf_size && dinfo.buf_size > bounce_buffer_size) {
		if (bounce_buffer)
			free(bounce_buffer);
		bounce_buffer = memalign(ARCH_DMA_MINALIGN, dinfo.buf_size);
		if (!bounce_buffer)
			bounce_buffer = memalign(ARCH_DMA_MINALIGN,
						 bounce_buffer_size);
		else
			bounce_buffer_size = dinfo.buf_size;
		bounce_buffer_addr = MAKE_XKPHYS(cvmx_ptr_to_phys(bounce_buffer));
		debug("%s: Increased bounce buffer size to %u bytes\n",
		      __func__, bounce_buffer_size);
	}
	/* arg is address to the device_info struct */
	sdi_addr = arg1;
	if (sdi_addr == 0) {
		debug("%s: NULL SDI address\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	copy_sdi_from_se(&sdi, sdi_addr);

	if (sdi.state == CVMX_DEV_STATE_OPEN) {
		debug("%s: SDI already open\n", __func__);
		return -CVMX_SEAPI_BUSY;
	}

	if (sdi.cookie != 0) {
		debug("%s: SDI cookie not NULL\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	sdi.cookie = (uint32_t)dd;
	sdi.info.storage.stor_type = dinfo.if_type;
	sdi.info.storage.dev = dd->dev;
	sdi.info.storage.size = dd->lba;
	sdi.info.storage.blksz = dd->blksz;
	sdi.info.storage.fs_type = dinfo.fs_type;
	debug("%s: Setting fs type to %d\n", __func__, dinfo.fs_type);
	sdi.info.storage.part_idx = dinfo.part;
	sdi.info.storage.part_info.start = part.start;
	sdi.info.storage.part_info.size = part.size;
	sdi.info.storage.part_info.blksz = part.blksz;
	sdi.info.storage.part_info.dos_type = part.type[0];
	sdi.info.storage.part_info.bootable = part.bootable;
	memcpy(sdi.info.storage.part_info.name, part.name, 32);
	memcpy(sdi.info.storage.part_info.type, part.type, 32);
	memcpy(sdi.info.storage.part_info.uuid, part.uuid, 37);

	sdi.state = CVMX_DEV_STATE_OPEN;

	copy_sdi_to_se(sdi_addr, &sdi);

	return 0;
}

/**
 * Closes a previously opened storage device
 *
 * @param	sig_addr	Physical address of signature
 * @param	call		Should be CVMX_SEAPI_DEV_CLOSE
 * @param	retval_addr	Not used
 * @param	arg0		Storage device information physical address
 * @param	arg1		Not used
 * @param	arg2		Not used
 * @param	arg3		Not used
 * @param	arg4		Not used
 *
 * @return	0 for success, otherwise error
 *
 * @NOTE:	This must be called when all i/O operations are done.  It does
 *		NOT free the bounce buffer.
 */
static int SEAPI_dev_close(uint64_t sig_addr, int call, uint64_t retval_addr,
			   uint64_t arg0, uint64_t arg1, uint64_t arg2,
			   uint64_t arg3, uint64_t arg4)
{
	struct cvmx_seapi_device_info sdi;
	uint64_t sdi_addr;
	int err = 0;

	sdi_addr = arg0;
	if (sdi_addr == 0)
		return -CVMX_SEAPI_EINVAL;

	copy_sdi_from_se(&sdi, sdi_addr);

	if (sdi.state == CVMX_DEV_STATE_CLOSED)
		return 0;

	if (sdi.cookie == 0)
		return -CVMX_SEAPI_ENODEV;

	sdi.state = CVMX_DEV_STATE_CLOSED;


	copy_sdi_to_se(sdi_addr, &sdi);

	return err;
}

/**
 * Raw write to a device, not supported yet
 */
static int SEAPI_dev_write(uint64_t sig_addr, int call, uint64_t retval_addr,
			   uint64_t arg0, uint64_t arg1, uint64_t arg2,
			   uint64_t arg3, uint64_t arg4)
{
	return -CVMX_SEAPI_ESYSCALL;
}

/**
 * Read a file from an open device
 *
 * @param	sig_addr	Physical signature address
 * @param	call		Must be CVMX_SEAPI_DEV_FILE_READ
 * @param	retval_addr	Not used
 * @param	arg0		SEAPI device info physical address
 * @param	arg1		Physical address of path and filename
 * @param	arg2		Physical buffer address to read into
 * @param	arg3		Physical address of starting read offset,
 *				overwritten with the new read offset.
 * @param	arg4		Physical address of number of bytes to read,
 *				outputs the total number of bytes read.
 *
 * @return	Number of bytes read if >= 0, otherwise error
 *
 * NOTE: The maximum file size is 32MB.  Reads larger than the bounce buffer
 * size will become increasingly slow and offsets will also be slow.
 */
static int SEAPI_dev_file_read(uint64_t sig_addr, int call, uint64_t retval_addr,
			       uint64_t arg0, uint64_t arg1, uint64_t arg2,
			       uint64_t arg3, uint64_t arg4)
{
	struct cvmx_seapi_device_info sdi;
	uint64_t sdi_addr;
	uint64_t filename_addr;
	char filename[2048];
	uint64_t offset_addr;
	uint64_t buf_addr;
	uint64_t len_addr;
	const char *if_type_str;
	char dev_part_str[64];
	int len;
	int offset;
	int bytes_left;
	int bytes_read = 0;
	int buf_offset = 0;
	int total_bytes_read = 0;
	char c;

	if (!bounce_buffer) {
		bounce_buffer = memalign(ARCH_DMA_MINALIGN, bounce_buffer_size);
		if (!bounce_buffer)
			return -CVMX_SEAPI_ENOMEM;
		bounce_buffer_addr = cvmx_ptr_to_phys(bounce_buffer);
		bounce_buffer_addr = MAKE_XKPHYS(bounce_buffer_addr);
	}

	sdi_addr = arg0;
	if (sdi_addr == 0)
		return -CVMX_SEAPI_EINVAL;

	copy_sdi_from_se(&sdi, sdi_addr);
	if (sdi.state != CVMX_DEV_STATE_OPEN)
		return -CVMX_SEAPI_ENODEV;
	filename_addr = arg1;
	if (filename_addr == 0)
		return -CVMX_SEAPI_EINVAL;
	filename_addr = MAKE_XKPHYS(filename_addr);

	for (len = 0, c = 0; len < sizeof(filename); len++) {
		c = cvmx_read64_uint8(filename_addr + len);
		filename[len] = c;
		if (c == '\0')
			break;
	}

	if ((len == 0) || (c != '\0'))
		return -CVMX_SEAPI_EINVAL;

	buf_addr = arg2;
	if (buf_addr == 0)
		return -CVMX_SEAPI_EINVAL;
	buf_addr = MAKE_XKPHYS(buf_addr);

	offset_addr = arg3;
	if (!offset_addr)
		offset = 0;
	else
		offset = cvmx_read64_int32(MAKE_XKPHYS(offset_addr));

	len_addr = arg4;
	if (len_addr == 0)
		return -CVMX_SEAPI_EINVAL;

	len_addr = MAKE_XKPHYS(len_addr);
	len = cvmx_read64_int32(len_addr);
	if (len < 0)
		return -CVMX_SEAPI_EINVAL;

	if ((offset + len) > 0x2000000) {
		debug("%s: Error: read too large\n", __func__);
		return -CVMX_SEAPI_EIO;
	}


	if (sdi.info.storage.part_idx > 0)
		snprintf(dev_part_str, sizeof(dev_part_str), "%d:%d",
			 sdi.info.storage.dev, sdi.info.storage.part_idx);
	else
		snprintf(dev_part_str, sizeof(dev_part_str), "%d",
			 sdi.info.storage.dev);
	if_type_str = storage_dev_type_to_str(sdi.info.storage.stor_type);
	if (!if_type_str) {
		debug("%s: Unknown device interface type %d\n", __func__,
		      sdi.info.storage.stor_type);
		return -CVMX_SEAPI_EINVAL;
	}
	debug("%s: Calling fs_set_blk_dev(%s, %s, %d)\n", __func__,
	      if_type_str, dev_part_str, sdi.info.storage.fs_type);

	bytes_left = len;
	while (bytes_left > 0) {
		if (fs_set_blk_dev(if_type_str, dev_part_str, sdi.info.storage.fs_type)) {
			debug("%s: Could not set block device for read fs_set_blk_dev(%s, %s, %d)\n",
			      __func__, if_type_str, dev_part_str,
			      sdi.info.storage.fs_type);
			return -CVMX_SEAPI_ENODEV;
		}

		if ((offset + min(bytes_left, bounce_buffer_size)) >= 0x2000000) {
			debug("%s: File offset too large!\n", __func__);
			break;
		}
		bytes_read = fs_read(filename, (ulong)bounce_buffer, offset,
				     min(bytes_left, bounce_buffer_size));
		debug("%s: read %s: %d bytes, offset: %d, bytes left: %d\n",
		      __func__, filename, bytes_read, offset, bytes_left);
		if (bytes_read <= 0)
			break;
		offset += bytes_read;
		bytes_left -= bytes_read;
		total_bytes_read += bytes_read;
		memcpy64(buf_addr + buf_offset, bounce_buffer_addr,
			 bytes_read);
		buf_offset += bytes_read;
	}

	cvmx_write64_int32(len_addr, total_bytes_read);
	cvmx_write64_int32(offset_addr, offset);

	if (bytes_read <= 0 && total_bytes_read == 0)
		return -CVMX_SEAPI_EIO;
	else
		return total_bytes_read;
}

/**
 * Writes to a file.
 *
 * @param	sig_addr	Physical address of signature
 * @param	call		Must be CVMX_SEAPI_DEV_FILE_WRITE
 * @param	retval_addr	Not used
 * @param	arg0		Physical address of SEAPI device info
 * @param	arg1		Physical address of path and filename
 * @param	arg2		Physical address of buffer to write
 * @param	arg3		Physical address of file offset or 0 if NULL.
 *				Note that for now only offset zero is supported.
 * @param	arg4		Physical address of number of bytes to write,
 *				outputs the number of bytes written, must be
 *				<= the size of the bounce buffer.
 * @return	Number of bytes written if >= 0, otherwise error.
 *
 * NOTE: The size of a file write cannot exceed the size of the bounce buffer.
 */
static int SEAPI_dev_file_write(uint64_t sig_addr, int call, uint64_t retval_addr,
				uint64_t arg0, uint64_t arg1, uint64_t arg2,
				uint64_t arg3, uint64_t arg4)
{
	struct cvmx_seapi_device_info sdi;
	uint64_t sdi_addr;
	uint64_t filename_addr;
	char filename[2048];
	char dev_part_str[64];
	const char *if_type_str;
	uint64_t offset_addr;
	uint64_t buf_addr;
	uint64_t len_addr;
	int len;
	int offset;
	int bytes_written = 0;
	char c;
	int ret;

	if (!bounce_buffer) {
		bounce_buffer = memalign(ARCH_DMA_MINALIGN, bounce_buffer_size);
		if (!bounce_buffer)
			return -CVMX_SEAPI_ENOMEM;
		bounce_buffer_addr = cvmx_ptr_to_phys(bounce_buffer);
		bounce_buffer_addr = MAKE_XKPHYS(bounce_buffer_addr);
	}

	sdi_addr = arg0;
	if (sdi_addr == 0) {
		debug("%s: Invalid SDI address\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	copy_sdi_from_se(&sdi, sdi_addr);
	if (sdi.state != CVMX_DEV_STATE_OPEN)
		return -CVMX_SEAPI_ENODEV;
	filename_addr = arg1;
	if (filename_addr == 0) {
		debug("%s: Invalid filename address\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}
	filename_addr = MAKE_XKPHYS(filename_addr);

	for (len = 0, c = 0; len < sizeof(filename); len++) {
		c = cvmx_read64_uint8(filename_addr + len);
		filename[len] = c;
		if (c == '\0')
			break;
	}

	if ((len == 0) || (c != '\0')) {
		debug("%s: Invalid filename\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	buf_addr = arg2;
	if (buf_addr == 0) {
		debug("%s: Invalid buffer address\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	offset_addr = arg3;
	if (offset_addr == 0)
		offset = 0;
	else
		offset = cvmx_read64_int32(MAKE_XKPHYS(offset_addr));

	if (offset) {
		debug("%s: Non-zero offset not supported\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}
	len_addr = arg4;
	if (len_addr == 0) {
		debug("%s: Invalid length address\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	len_addr = MAKE_XKPHYS(len_addr);
	len = cvmx_read64_int32(len_addr);

	if (len < 0) {
		debug("%s: Invalid length\n", __func__);
		return -CVMX_SEAPI_EINVAL;
	}

	buf_addr = MAKE_XKPHYS(buf_addr);
	memcpy64(bounce_buffer_addr, buf_addr, len);

	if (sdi.info.storage.part_idx > 0)
		snprintf(dev_part_str, sizeof(dev_part_str), "%d:%d",
			 sdi.info.storage.dev, sdi.info.storage.part_idx);
	else
		snprintf(dev_part_str, sizeof(dev_part_str), "%d",
			 sdi.info.storage.dev);
	if_type_str = storage_dev_type_to_str(sdi.info.storage.stor_type);
	if (!if_type_str) {
		debug("%s: Unknown device interface type %d\n", __func__,
		      sdi.info.storage.stor_type);
		return -CVMX_SEAPI_EINVAL;
	}

	ret = -CVMX_SEAPI_EIO;

	switch (sdi.info.storage.fs_type) {
#if defined(CONFIG_FS_FAT) && defined(CONFIG_FAT_WRITE)
	case CVMX_SEAPI_FS_TYPE_FAT:
		ret = file_fat_write(filename, bounce_buffer, len);
		debug("%s: file_fat_write(%s, 0x%p, 0x%x) returned %d\n",
		      __func__, filename, bounce_buffer, len, ret);
		if (ret > 0) {
			bytes_written = ret;
		} else {
			bytes_written = 0;
		}
		break;
#endif
#if defined(CONFIG_FS_EXT4) && defined(CONFIG_EXT4_WRITE)
	case CVMX_SEAPI_FS_TYPE_EXT:
		ret = ext4fs_write(filename, bounce_buffer, len);
		if (ret == 0)
			bytes_written = len;
		break;
#endif
	default:
		debug("%s: Unsupported filesystem type 0x%x for write\n",
		      __func__, sdi.info.storage.fs_type);
		return -CVMX_SEAPI_EINVAL;
	}
	cvmx_write64_int32(len_addr, bytes_written);

	return ret >= 0 ? bytes_written : ret;
}

/**
 * Get a U-Boot environment variable
 *
 * @param sig_addr	Physical address of signature (not used)
 * @param call		system call number (not used)
 * @param retval_addr	Physical address to write return data (not used)
 * @param arg0		Physical address of environment variable name
 * @param arg1		Physical address to write environment value
 * @param arg2		Size of buffer to write to
 *
 * @return		0 for success, CVMX_SEAPI_EINVAL or CVMX_SEAPI_NOENV
 */
static int SEAPI_getenv(uint64_t sig_addr, int call, uint64_t retval_addr,
			uint64_t arg0, uint64_t arg1, uint64_t arg2,
			uint64_t arg3, uint64_t arg4)
{
	uint64_t env_addr;
	uint64_t val_addr;
	int max_size;
	int len;
	char env_name[1024];
	char *val;
	char c;
	int slen;

	env_addr = arg0;
	val_addr = arg1;
	max_size = arg2;
	debug("env_addr: 0x%llx, val_addr: 0x%llx, max_size: 0x%x\n",
	      env_addr, val_addr, max_size);
	if (env_addr == 0 || val_addr == 0)
		return -CVMX_SEAPI_EINVAL;

	env_addr = MAKE_XKPHYS(env_addr);

	for (len = 0; len < 1024; len++) {
		c = cvmx_read64_uint8(env_addr + len);
		env_name[len] = c;
		if (c == '\0')
			break;
	}

	if ((len == 0) || (c != '\0'))
		return -CVMX_SEAPI_EINVAL;

	val_addr = MAKE_XKPHYS(val_addr);
	memset64(val_addr, 0, max_size);
	val = getenv(env_name);
	if (val) {
		slen = strlen(val);
		if (slen < max_size)
			max_size = slen + 1;

		debug("Result for %s is %s, copying %d characters to 0x%llx\n",
		      env_name, val, max_size, val_addr);
		for (len = 0; len < max_size; len++)
			cvmx_write64_uint8(val_addr + len, val[len]);
		return 0;
	} else {
		debug("No result for %s\n", env_name);
		return -CVMX_SEAPI_NOENV;
	}
}

/**
 * Set a U-Boot environment variable
 *
 * @param sig_addr	Physical address of signature (not used)
 * @param call		system call number (not used)
 * @param retval_addr	Physical address to write return data (not used)
 * @param arg0		Physical address of environment variable name
 * @param arg1		Physical address to read environment value from
 * @param arg2		Size of NULL-terminate value in bytes, must not exceed
 *			CVMX_SEAPI_SETENV_MAX_VALUE_LENGTH. Set to 0 to
 *			delete the environment variable.
 *
 * @return		0 for success, CVMX_SEAPI_EINVAL or CVMX_SEAPI_NOENV
 */
static int SEAPI_setenv(uint64_t sig_addr, int call, uint64_t retval_addr,
			uint64_t arg0, uint64_t arg1, uint64_t arg2,
			uint64_t arg3, uint64_t arg4)
{
	uint64_t env_addr;
	uint64_t val_addr;
	int max_size;
	int len;
	char env_name[1024];
	char env_value[CVMX_SEAPI_SETENV_MAX_VALUE_LENGTH + 1];
	char *val;
	char c;
	int rc;

	env_addr = arg0;
	val_addr = arg1;
	max_size = min(arg2, CVMX_SEAPI_SETENV_MAX_VALUE_LENGTH);

	debug("%s: env_addr: 0x%llx, val_addr: 0x%llx, max_size: 0x%x\n",
	      __func__, env_addr, val_addr, max_size);
	if (env_addr == 0)
		return -CVMX_SEAPI_EINVAL;

	env_addr = MAKE_XKPHYS(env_addr);
	if (val_addr)
		val_addr = MAKE_XKPHYS(val_addr);
	else
		max_size = 0;

	for (len = 0; len < 1024; len++) {
		c = cvmx_read64_uint8(env_addr + len);
		env_name[len] = c;
		if (c == '\0')
			break;
	}

	if (!max_size || !arg1) {
		rc = setenv(env_name, NULL);
		return rc ? -CVMX_SEAPI_NOENV : 0;
	}

	for (len = 0; len < max_size; len++) {
		c = cvmx_read64_uint8(val_addr + len);
		env_value[len] = c;
	}
	env_value[len] = '\0';

	rc = setenv(env_name, env_value);
	debug("%s: rc: %d\n", __func__, rc);
	return rc ? -CVMX_SEAPI_NOENV : 0;
}

/**
 * Saves all U-Boot environment variables
 *
 * @param sig_addr	Physical address of signature (not used)
 * @param call		system call number (not used)
 * @param retval_addr	Physical address to write return data (not used)
 *
 * @return		0 for success, CVMX_SEAPI_EINVAL or CVMX_SEAPI_NOENV
 */
static int SEAPI_saveenv(uint64_t sig_addr, int call, uint64_t retval_addr,
			 uint64_t arg0, uint64_t arg1, uint64_t arg2,
			 uint64_t arg3, uint64_t arg4)
{
	int rc;

	debug("%s: Entry\n", __func__);
#if !defined(CONFIG_ENV_IS_NOWHERE)
	rc = saveenv();
#else
	return -CVMX_SEAPI_NOENV;
#endif
	debug("%s: rc: %d\n", __func__, rc);
	return rc ? -CVMX_SEAPI_NOENV : 0;
}

/**
 * Starts USB and scans for storage devices
 *
 * @param	sig_addr	Physical signature address
 * @param	call		Must be CVMX_SEAPI_DEV_USB_START
 * @param	retval_addr	Not used
 * @param	arg0		Not used
 * @param	arg1		Not used
 * @param	arg2		Not used
 * @param	arg3		Not used
 * @param	arg4		Not used
 *
 * @return	0 if successful or CVMX_SEAPI_EIO for I/O error
 */
static int SEAPI_usb_start(uint64_t sig_addr, int call, uint64_t retval_addr,
			   uint64_t arg0, uint64_t arg1, uint64_t arg2,
			   uint64_t arg3, uint64_t arg4)
{
#ifdef CONFIG_USB_STORAGE
	usb_stop();
	if (usb_init() < 0) {
		debug("%s: Initializing USB failed\n", __func__);
		return -CVMX_SEAPI_ENODEV;
	}
	debug("%s: Starting USB and scanning storage devices\n", __func__);
	return usb_stor_scan(0);
#else
	debug("%s: USB storage not supported\n", __func__);
	return -CVMX_SEAPI_EIO;
#endif
}

/**
 * Configure QLM TX parameters
 *
 * @param sig_addr	Physical address of signature (not used)
 * @param call		system call number (not used)
 * @param retval_addr	Physical address to write return data (not used)
 * @param arg0		node number for OCX (0 if not used)
 * @param arg1		QLM number to tune
 * @param arg2		Baud rate
 * @param arg3		TX voltage swing value
 * @param arg4		Pre-emphasis values
 *
 * @return		0 for success, CVMX_SEAPI_EINVAL
 *
 * @see		octeon_qlm_tune_v3() in octeon_qlm.h
 */
static int SEAPI_qlm_tune(uint64_t sig_addr, int call, uint64_t retval_addr,
			  uint64_t arg0, uint64_t arg1, uint64_t arg2,
			  uint64_t arg3, uint64_t arg4, uint64_t arg5,
			  uint64_t arg6)
{
#ifdef CONFIG_OCTEON_QLM
	if (!OCTEON_IS_MODEL(OCTEON_CN73XX) && !OCTEON_IS_MODEL(OCTEON_CN78XX))
		return -CVMX_SEAPI_EINVAL;
	octeon_qlm_tune_v3(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
	return 0;
#else
	return -CVMX_SEAPI_EINVAL;
#endif
}

/**
 * Initializes support for the Simple Exec API
 *
 * @return 0 for success, error otherwise
 */
int seapi_init(void)
{
	struct cvmx_seapi_signature *sig = NULL;
	struct cvmx_seapi_context *ctx;

	SEAPI_stor_init();
	/* Allocate the signature data structure in KSEG0 */
	sig = cvmx_bootmem_alloc_named_range(sizeof(*sig),
					     0, 0x0fffffff, 0,
					     CVMX_SEAPI_NAMED_BLOCK_NAME);
	if (!sig)
		return -CVMX_SEAPI_ENOMEM;

	memset(sig, 0, sizeof(*sig));
	debug("U-Boot SE API signature at 0x%p, size: 0x%x\n", sig,
	      sizeof(*sig));
	seapi_sig = sig;
	call_table = calloc(sizeof(se_cfp_t *), CVMX_SEAPI_MAXCALL);
	if (!call_table) {
		puts("Error: could not allocate call table\n");
		return -1;
	}

	debug("Call table at 0x%p\n", call_table);
	/* call_table[CVMX_SEAPI_DEV_ENUM] = SEAPI_dev_enum; */
	call_table[CVMX_SEAPI_DEV_OPEN] = SEAPI_dev_open;
	call_table[CVMX_SEAPI_DEV_CLOSE] = SEAPI_dev_close;
	call_table[CVMX_SEAPI_DEV_FILE_READ] = SEAPI_dev_file_read;
	call_table[CVMX_SEAPI_DEV_FILE_WRITE] = SEAPI_dev_file_write;
	call_table[CVMX_SEAPI_ENV_GET] = SEAPI_getenv;
	call_table[CVMX_SEAPI_DEV_USB_START] = SEAPI_usb_start;
	call_table[CVMX_SEAPI_QLM_TUNE] = SEAPI_qlm_tune;
	call_table[CVMX_SEAPI_ENV_SET] = SEAPI_setenv;
	call_table[CVMX_SEAPI_ENV_SAVE] = SEAPI_saveenv;

	debug("Copying magic\n");
	memcpy(sig->magic, CVMX_SEAPI_SIG_MAGIC, CVMX_SEAPI_SIG_MAGLEN);
	sig->ver_maj = CVMX_SEAPI_SIG_VERSION_MAJ;
	sig->ver_min = CVMX_SEAPI_SIG_VERSION_MIN;
	sig->syscall_paddr = cvmx_ptr_to_phys(&uboot_se_napi_entry);
	debug("Syscall address 0x%llx\n", sig->syscall_paddr);
	sig->call_table_paddr = cvmx_ptr_to_phys(call_table);
	ctx = &sig->context;
	debug("Context at 0x%p\n", ctx);
	/* Allocate the stack */
	ctx->uboot_stack = (uint32_t)calloc(1, CVMX_SEAPI_STACK_SIZE);
	if (!ctx->uboot_stack) {
		printf("%s: Error: could not allocate stack\n", __func__);
		return -1;
	}
	ctx->uboot_stack += CVMX_SEAPI_STACK_SIZE;
	ctx->uboot_stack -= 16;
	ctx->uboot_stack &= 0xfffffff0;	/* Align to 16 bytes */
	debug("U-Boot stack at 0x%x\n", ctx->uboot_stack);
	ctx->lock_addr = cvmx_ptr_to_phys(&seapi_lock);
	ctx->uboot_gd = (uint32_t)gd;
	debug("lock at 0x%llx, gd: 0x%x\n", ctx->lock_addr, ctx->uboot_gd);
	asm volatile("	move	$gp,%[gp_storage]	\n"
		     : [gp_storage] "=d" (ctx->uboot_gp));
	/* Read the U-Boot TLB entry */
	write_c0_index(get_num_tlb_entries() - 1);
	debug("Reading U-Boot TLB entry... calling tlbr...\n");
	tlb_read();
	debug("Done with tlbr\n");
	ctx->uboot_c0_entrylo0 = read_64bit_c0_entrylo0();
	ctx->uboot_c0_entrylo1 = read_64bit_c0_entrylo1();
	ctx->uboot_c0_entryhi = read_64bit_c0_entryhi();
	ctx->uboot_c0_pagemask = read_c0_pagemask();
	debug("entrylo0: 0x%llx, entrylo1: 0x%llx, entryhi: 0x%llx, pagemask: 0x%x\n",
	      ctx->uboot_c0_entrylo0, ctx->uboot_c0_entrylo1,
	      ctx->uboot_c0_entryhi, ctx->uboot_c0_pagemask);
	return 0;
}

/**
 * Entry point from the assembly code
 *
 * @param sig_addr	Physical address of the signature
 * @param call		system call number
 * @param retval_addr	physical return value address
 * @param arg0		Call dependent
 * @param arg1		Call dependent
 * @param arg2		Call dependent
 * @param arg3		Call dependent
 * @param arg4		Call dependent
 *
 * @return		return value from the call
 */
int seapi_syscall(uint64_t sig_addr, int call, uint64_t retval_addr,
		  uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3,
		  uint64_t arg4)
{
	int rc;

	debug("%s(0x%llx, %d, 0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx)\n",
	      __func__, sig_addr, call, retval_addr,
	      arg0, arg1, arg2, arg3, arg4);
	if ((call < 0) || (call >= CVMX_SEAPI_MAXCALL) || !call_table[call]) {
		debug("%s: syscall #%d does not have a handler\n",
		      __func__, call);
		return -CVMX_SEAPI_ESYSCALL;
	}

	if (!call_table) {
		debug("%s: Call table not initialized\n", __func__);
		return -CVMX_SEAPI_ESYSCALL;
	}

	if (!call_table[call]) {
		debug("Invalid call number 0x%x\n", call);
		return -CVMX_SEAPI_ESYSCALL;
	}
	debug("%s: Calling %p\n", __func__, call_table[call]);
	rc = call_table[call](sig_addr, call, retval_addr, arg0, arg1, arg2,
			      arg3, arg4);
	debug("%s: call returned %d\n", __func__, rc);
	if (retval_addr != 0)
		cvmx_write64_uint32(MAKE_XKPHYS(retval_addr), rc);

	return rc;
}
