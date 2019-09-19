/***********************license start***************
 * Copyright (C) 2015 Cavium Inc. (support@cavium.com).  All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

#ifndef __SEAPI_PUBLIC_H__
#define __SEAPI_PUBLIC_H__

#define CVMX_SEAPI_EINVAL	1	/** Invalid argument */
#define CVMX_SEAPI_ENODEV	2	/** No device */
#define CVMX_SEAPI_ENOMEM	3	/** No memory */
#define CVMX_SEAPI_BUSY		4	/** Device is busy */
#define CVMX_SEAPI_EIO		5	/** I/O error */
#define CVMX_SEAPI_ESYSCALL	6	/** Syscall error */
#define CVMX_SEAPI_NOENV	7	/** Environment variable not set */
#define CVMX_SEAPI_OTHER	8	/** Other error */

typedef int (*cvmx_syscall_t)(int, int *, ...);

#define CVMX_SEAPI_SIG_VERSION_MAJ	1	/** Major version */
/* 1.1: Added setenv and saveenv support */
#define CVMX_SEAPI_SIG_VERSION_MIN	1	/** Minor version */
#define CVMX_SEAPI_SIG_MAGIC		"UBootAPI"
#define CVMX_SEAPI_SIG_MAGLEN		8
/** Stack size for U-Boot context (normally it's 192K) */
#define CVMX_SEAPI_STACK_SIZE		(256 * 1024)
/** Name of named block where the signature and call table can be found */
#define CVMX_SEAPI_NAMED_BLOCK_NAME	"U-Boot_SE_API"

/** Maximum size of value parameter in bytes */
#define CVMX_SEAPI_SETENV_MAX_VALUE_LENGTH	4096

enum cvmx_storage_type {
	cvmx_storage_ide,	/** Compact flash card */
	cvmx_storage_usb,	/** USB storage */
	cvmx_storage_mmc,	/** MMC/SD storage */
	cvmx_storage_sata,	/** SATA storage */
	cvmx_storage_max
};

struct cvmx_storage_spec {
	int	max_dev;
	int	enum_started;
	int	enum_ended;
	enum cvmx_storage_type type;
	char	*name;
};

/**
 * Context information used when SE apps call U-Boot.
 *
 * NOTE: Any changes made to this data structure requires that the offset
 * defines in start.S also be updated.
 */
struct cvmx_seapi_context {
	uint64_t	regs[32];	/** Caller's registers */
	uint64_t	se_return_addr;	/** 256 Caller's return address */
	uint64_t	se_c0_entrylo0;	/** 264 c0_entrylo0 from caller */
	uint64_t	se_c0_entrylo1;	/** 272 c0_entrylo1 from caller */
	uint64_t	se_c0_entryhi;	/** 280 c0_entryhi from caller */
	uint32_t	se_c0_pagemask;	/** 288 c0_pagemask from caller */
	uint32_t	se_c0_index;	/** 292 C0_index from caller */
	uint32_t	se_c0_status;	/** 296 C0_status from caller */
	uint32_t        pad;
	uint64_t	lock_addr;	/** 304 Physical location of spinlock */
	uint32_t	uboot_gd;	/** 312 GD address */
	uint32_t	pad2;
	uint64_t	uboot_gp;	/** 320 GP register (GOT) */
	/** 328 U-Boot TLB entryl00 register */
	uint64_t	uboot_c0_entrylo0;
	/** 336 U-Boot TLB entrylo1 register */
	uint64_t	uboot_c0_entrylo1;
	/** 344 U-Boot TLB entryhi register */
	uint64_t	uboot_c0_entryhi;
	/** 348 U-Boot pagemask register */
	uint32_t	uboot_c0_pagemask;
	uint32_t	uboot_stack;	/** 352 Local U-Boot stack address */
};

/**
 * Signature data structure for SE syscall support
 */
struct cvmx_seapi_signature {
	char		magic[CVMX_SEAPI_SIG_MAGLEN];	/** magic string */
	uint16_t	ver_maj;		/** API major version */
	uint16_t	ver_min;		/** API minor version */
	uint32_t	sig_crc;		/** CRC of this struct */
	uint64_t	call_table_paddr;	/** Physical address of call table */
	uint64_t	syscall_paddr;		/** Address of entry point */
	struct cvmx_seapi_context	context;/** Context information */
};

/**
 * Operations supported
 *
 * ni = not fully implemented or tested
 */
enum {
	CVMX_SEAPI_RSVD = 0,
	CVMX_SEAPI_DEV_ENUM,		/** Not supported */
	CVMX_SEAPI_DEV_OPEN,		/** Open a storage device */
	CVMX_SEAPI_DEV_CLOSE,		/** Close a storage device */
	CVMX_SEAPI_DEV_READ,		/** Raw read a storage device (ni) */
	CVMX_SEAPI_DEV_WRITE,		/** Raw write a storage device (ni) */
	CVMX_SEAPI_DEV_FILE_READ,	/** Read from a file */
	CVMX_SEAPI_DEV_FILE_WRITE,	/** Write to a file */
	CVMX_SEAPI_ENV_GET,		/** Get a U-Boot environment variable */
	CVMX_SEAPI_DEV_USB_START,	/** Calls usb_stor_scan() */
	CVMX_SEAPI_QLM_TUNE,		/** Tune QLM tx parameters */
	CVMX_SEAPI_ENV_SET,		/** Set a U-Boot environment variable */
	CVMX_SEAPI_ENV_SAVE,		/** Save all environment variables */
	CVMX_SEAPI_MAXCALL,
};

/** Device type, currently only storage devices are supported. */
enum cvmx_seapi_dev_type {
	CVMX_SEAPI_DEV_TYPE_NONE	= 0,
	CVMX_SEAPI_DEV_TYPE_NET		= 1,	/** Unsupported */
	CVMX_SEAPI_DEV_TYPE_STOR	= 2	/** Storage device */
};

/** Filesystem type.  Note that ANY can work for reads but not writes. */
enum cvmx_seapi_fs_type {
	CVMX_SEAPI_FS_TYPE_ANY		= 0,	/** Autodetect filesystem */
	CVMX_SEAPI_FS_TYPE_FAT		= 1,	/** FAT file system */
	CVMX_SEAPI_FS_TYPE_EXT		= 2,	/** EXT2/3/4 filesystem */
};

enum cvmx_seapi_storage_dev_type {
	CVMX_STORAGE_DEV_UNKNOWN = 0,
	CVMX_STORAGE_DEV_IDE	= 0x10,
	CVMX_STORAGE_DEV_SCSI	= 0x20,
	CVMX_STORAGE_DEV_USB	= 0x40,
	CVMX_STORAGE_DEV_MMC	= 0x80,
	CVMX_STORAGE_DEV_SATA	= 0x100
};

/** Device state */
enum cvmx_seapi_dev_state {
	CVMX_DEV_STATE_CLOSED	= 0,	/** Device has been opened */
	CVMX_DEV_STATE_OPEN	= 1	/** Device is closed */
};

typedef uint64_t cvmx_lbasize_t;

/** Partition information */
struct cvmx_seapi_part_info {
	cvmx_lbasize_t	start;		/** Start block number */
	cvmx_lbasize_t	size;		/** Size of partition in blocks */
	uint32_t	blksz;		/** Size of block */
	uint8_t		name[32];	/** Partition name */
	uint8_t		type[32];	/** String type description */
	uint8_t		dos_type;	/** DOS type value */
	bool		bootable;	/** True if partition is marked bootable */
	char		uuid[37];	/** Filesystem UUID as string (optional) */
};

/**
 * This data structure is used for opening a storage device.
 */
struct cvmx_seapi_storage_dev_info {
	enum cvmx_seapi_storage_dev_type	if_type; /** Type of interface */
	uint32_t				index;	/** Interface index */
	int					part;	/** Partition number */
	enum cvmx_seapi_fs_type			fs_type;/** Filesystem type */
	/**
	 * Optional bounce buffer size, default is 64K if set to zero.  If
	 * the buffer size cannot be allocated then the previous size will be
	 * used.  No error will be received.  The buffer can only be increased,
	 * not decreased.
	 */
	uint32_t				buf_size;
};

/**
 * Device info data structure.  This data structure is designed to mirror
 * the U-Boot device_info data structure in include/api_public.h.
 */
struct cvmx_seapi_device_info {
	enum cvmx_seapi_dev_type	type;	/** Device type, only storage */
	uint32_t			cookie;	/** 32-bit pointer used internally */
	union {
		struct {
			/** Storage device type */
			enum cvmx_seapi_storage_dev_type 	stor_type;
			/** device number */
			int					dev;
			/** Size of device in blocks */
			cvmx_lbasize_t				size;
			/** Block size in bytes */
			uint32_t				blksz;
			/** Partition number */
			int					part_idx;
			/** Filesystem type */
			int					fs_type;
			/** Partition information */
			struct cvmx_seapi_part_info		part_info;
		} storage;
		struct {
			uint8_t 	hw_addr[6];
		} net;	/* Unsupported */
	} info;
	enum cvmx_seapi_dev_state	state;
	int				more;	/** True if there are more devices */
};

/**
 * Syscall function prototype
 *
 * @param	sig_paddr	Physical signature address
 * @param	call		function call number
 * @param	retval_addr	Optional return value address
 * @param	arg0		argument 0
 * @param	arg1		argument 1
 * @param	arg2		argument 2
 * @param	arg3		argument 3
 * @param	arg4		argument 4
 *
 * @return	Return code from call, negative values reflect errors.
 *
 * NOTE: No more than 8 parameters total are supported since all arguments
 * must be passed in registers $a0-$a7.
 */
typedef int (*uboot_seapi_syscall)(uint64_t sig_paddr, int call,
				   uint64_t retval_addr, uint64_t arg0,
				   uint64_t arg1, uint64_t arg2, uint64_t arg3,
				   uint64_t arg4);

/**
 * this handle is used when calling U-Boot SEAPI functions
 */
struct cvmx_seapi_handle {
	uint64_t sig_paddr;	/** Physical address of signature in memory */
	struct cvmx_seapi_signature *sig;	/** Virt pointer to signature */
	uboot_seapi_syscall syscall;		/** Virt addr of syscall func */
};

#endif /* __SEAPI_PUBLIC_H__ */
