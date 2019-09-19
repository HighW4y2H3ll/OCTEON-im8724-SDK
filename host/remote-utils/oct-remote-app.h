/***********************license start***************
 * Copyright (c) 2017  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 ***********************license end**************************************/

#ifndef CVMX_BUILD_FOR_LINUX_HOST
#error "This code should be compiled ONLY for LINUX_HOST (X86-64)!"
#endif

/* Needed for bd_t */
#include "asm/u-boot.h"

/* Needed for octeon_eeprom_board_desc_t in octeon-boot-info.h */
#include "asm/arch-octeon/octeon_eeprom_types.h"

/* Needed for gd_t in octeon_boot.h */
#include "asm/global_data.h"

/* Header needed for "linux_app_boot_info", "boot_init_vector" */
#include "asm/arch-octeon/octeon-boot-info.h"

/* Header needed for  cvmx_bootinfo_t */
#include "cvmx-app-init.h"

/* Header needed for "octeon_boot_descriptor_t" */
#include "asm/arch-octeon/octeon_boot.h"


/* For readability - identify address types */
typedef	uint64_t paddr_t;
typedef	uint64_t app_vaddr_t;

/* u-boot defines this type aliase, conflicts with cvmx-*-defs.h structs */
#undef	u64

#define	CVMX_APP_MAX_SEGMENTS	32

typedef struct cvmx_app_boot_segment {
	enum {
		CABS_TYPE_NONE = 0,
		CABS_TYPE_TEXT,
		CABS_TYPE_RO_DATA,
		CABS_TYPE_RW_SHARED,
		CABS_TYPE_RW_PRIVATE,
		CABS_TYPE_STACK_HEAP,
		CABS_TYPE_MAX
	} cabs_type;
	uint32_t	mem_size, file_size;
	uint64_t	file_offset;
	uint64_t	vaddr;
} cvmx_app_boot_segment_t;

/*
 * This data structure is allocated for each code running an SE application
 * and contains all of the data types required to interface with the
 * bootloader, and some data items shared between separate invocations
 * of the app-boot utility.
 *
 * It is currently separate from struct cvmx_app_hotplug_global,
 * only referencing it's cvmx_app_hotplug_info array element by
 * hotplug_app_index. The coremasks for this app are stored in
 * cvmx_app_hotplug_info.
 */

typedef
struct cvmx_app_boot_record {
	uint32_t	cabr_magic0;

	unsigned short
		cabr_hotplug_app_index;	/* -> cvmx_app_hotplug_info */
	short
		cabr_core,		/* current core id */
		cabr_init_core;		/* first core for this app */
	paddr_t		cabr_paddr;	/* used for freeing this struct */

	boot_info_block_t	cabr_boot_info_block
			__attribute__((aligned(128)));

	/* Application TLB prototypes, heap, stack locations */
	octeon_tlb_entry_t_bl	tlb_entries[NUM_OCTEON_TLB_ENTRIES];

	uint32_t	cabr_magic1;

	/* Various per-program fields */
	app_vaddr_t	cabr_entry_point,
			cabr_heap_base,
			cabr_heap_end,
			cabr_stack_base;
	unsigned int
			cabr_heap_size,
			cabr_stack_size;

	int		argc;
	paddr_t		argv[OCTEON_ARGV_MAX_ARGS];
	char		arg_buf_name[32];

	/* Flags */
	unsigned	boot_info_flags; /* BOOT_INFO_FLAG_* */
	unsigned	boot_desc_flags; /* boot_octeon_descriptor_flags */
	unsigned	boot_cfg_flags;	 /* CVMX_BOOTINFO_CFG_FLAG_* */
	unsigned	no_mark_private_data;

} cvmx_app_boot_record_t;

#define	CVMX_APP_BOOT_RECORD_MAGIC	0xb007cab4


extern void
octeon_remote_read_mem2(void *buffer, uint64_t physical_address, int length);
extern void
octeon_remote_write_mem2(uint64_t physical_address, void *buffer, int length);
extern void
oct_tlb_array_print(const octeon_tlb_entry_t_bl *tlb_array, const char *info);
extern void
oct_remote_read_coremask(uint64_t remoteaddr, cvmx_coremask_t *pcm);
extern void
oct_remote_write_coremask(uint64_t remoteaddr, const cvmx_coremask_t *pcm);
extern void
save_cabr_on_target(cvmx_app_boot_record_t *cabr_ptr);
extern void
oct_tlb_array_print(const octeon_tlb_entry_t_bl *tlb_array, const char *info);



/*
 * Each cvmx_app_boot_record_t structure is only allocated if the appropriate
 * core is actually running an SE app.
 * This block contains physical pointers to each one of the per-core structure
 * if allocated, or NULL if none is allocated.
 * Ultimately, this structure can be merged with cvmx_app_hotplug_global
 * and moved to shared header files
 */
typedef struct cvmx_app_boot_master {
	uint32_t	magic;
	unsigned short	version;
	unsigned short	mum_cores;
	uint64_t	cabr_paddr[CVMX_MAX_CORES];

} cvmx_app_boot_master_t;

#define	CVMX_APP_BOOT_MASTER_NAME	"cvmx-app-boot-master-block"

#define	CABR_MAGIC		0x0e1f0e1fUL

/*
 * Prototypes
 */
extern linux_app_boot_info_t *oct_app_get_labi(void);
extern int elf_file_prep(int fd, cvmx_app_boot_record_t *cabr_ptr,
	cvmx_app_boot_segment_t *segments, unsigned max_segments);
extern int oct_args_prep(cvmx_app_boot_record_t *cabr_ptr,
		int argc, char **argv);
extern void oct_args_free(cvmx_app_boot_record_t *cabr_ptr);

extern void oct_tlb_flags_init(cvmx_app_boot_record_t *cabr_ptr);

extern int oct_app_memory_clone(
		const octeon_tlb_entry_t_bl *primary_array,
		octeon_tlb_entry_t_bl *new_array);


extern int oct_tlb_primary_init(
		int elf_fd,
		octeon_tlb_entry_t_bl *tlb_array,
		cvmx_app_boot_segment_t *seg,
		unsigned num_seg);

extern int oct_app_memory_free(
		const octeon_tlb_entry_t_bl *tlb_array,
		bool primary);

extern app_vaddr_t oct_tlb_primary_heap_stack_locate(
		octeon_tlb_entry_t_bl *tlb_array,
		size_t heap_size,
		size_t stack_size);

extern int oct_tlb_memcpy(
		const octeon_tlb_entry_t_bl *tlb_array,
		app_vaddr_t dest_virt,
		const void *buf, size_t len);

extern cvmx_app_boot_master_t *global_master_ptr;

struct app_params {
	enum {
		APP_CMD_UNDEF,
		APP_CMD_INFO,
		APP_CMD_BOOT,
		APP_CMD_ADD,
		APP_CMD_DEL,
		APP_CMD_KILL,
		APP_CMD_RESET
	} command;
	unsigned int	boot_info_flags,
			boot_desc_flags;
	unsigned int	console_uart;
	unsigned long	stack_size;
	unsigned long	heap_size;
	unsigned short	numcores;
	cvmx_coremask_t	coremask;
	char		*app_name;
	short		app_index;
	unsigned
			flag_num_cores_strict:1,
			no_mark_private_data:1,
			image_little_endian:1,
			unused:1;
	int		argc;
	char		**argv;
};


extern int oct_boot_desc_setup(
		struct app_params *params,
		cvmx_app_boot_record_t *cabr_ptr);

/* Structure for maintaining virtual mappings of physical address ranges */
typedef	struct vmap_range {
	void *vbase;
	size_t	vlength;
} vmap_range_t;

extern void *map_phys_range(paddr_t phys_base, size_t phys_size,
		vmap_range_t *vrange);
extern  int unmap_phys_range(vmap_range_t *vrange);

extern void print_boot_block(boot_info_block_t *i);

/* Global variable to control amount of progress information */
extern int verbose;

/* This flag needs to go into "octeon_boot.h" temp work-around */
#ifndef	OCTEON_BL_FLAG_HPLUG_CORES
#define OCTEON_BL_FLAG_HPLUG_CORES (1 << 6)
#endif

