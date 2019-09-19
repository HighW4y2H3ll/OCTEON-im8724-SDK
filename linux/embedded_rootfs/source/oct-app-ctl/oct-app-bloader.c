/***********************license start************************************
 * Copyright (c) 2013 Cavium (support@cavium.com). All rights
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
 * For any questions regarding licensing please contact support@cavium.com
 *
 ***********************license end**************************************/

/**
 * @file
 *
 * $Id: oct-app-bloader.c 154265 2017-02-15 00:07:03Z cchavva $
 *
 * DESCRIPTION
 *
 * For booting an SE application from Linux userland, the bootloader
 * cooperation is required.
 * All code that is tied to the interface with the bootloader
 * should go in this file.
 *
 * In essense this module performs all actions that are required
 * by start_app() in cmd_octeon.c file of the bootloader
 * including even some tweaking of the TLB mappings.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>

#include "oct-app.h"


void print_boot_block(boot_info_block_t * bib)
{
	char ff[128];
	char s[32];
	int i;

	if( verbose <= 1)
		return;

	/* Convert flag bits into string */
	memset(ff, 0, sizeof(ff));

	for(i = 0; i < 32; i ++ ) {
		if(!(bib->flags & (1<<i)))
			continue;
		sprintf(s, "%d=", i);
		strcat(ff, s);
		if( (1<<i) == BOOT_INFO_FLAG_DISABLE_ICACHE_PREFETCH )
			strcat(ff, "DisableIcachePrefetch,");
		else if( (1<<i) == BOOT_INFO_FLAG_LITTLE_ENDIAN )
			strcat(ff, "LittleEndian,");
		else  
			strcat(ff, "?,");
	}
	strcat(ff, ".");

	fprintf(stderr, "INFO: "
		"info block:\n"
		" tlb_entries=%p" " entry_point=%#llx boot_desc=%#llx\n"
		" stack_top=%#llx exc_ base=%#x cvmx_desc=%#llx\n"
		" flags: %s\n",
		 bib->tlb_entries,
		(unsigned long long) bib->entry_point,
		(unsigned long long) bib->boot_desc_addr,
		(unsigned long long) bib->stack_top,
		bib->exception_base,
		(unsigned long long) bib->cvmx_desc_addr,
		ff);
}

/*
 * Tweak the TLB mapping array to match some peculiar behavior of
 * the start_app() bootloader routine
 */
static int oct_boot_desc_tlb_adjust( cvmx_app_boot_record_t * cabr )
{
	app_vaddr_t v_start, v_end;
	octeon_tlb_entry_t_bl * tlb_array;
	octeon_tlb_entry_t_bl * tlb_entry;
	unsigned i;

	tlb_array = cabr->tlb_entries;

	for(i = 0; i < NUM_OCTEON_TLB_ENTRIES; i++ ) {
		tlb_entry = &tlb_array[i];

		if( tlb_entry->page_size == 0 )
			break;
		v_start = tlb_entry->virt_base;
		v_end = v_start + TLB_PAGE_SIZE_FROM_EXP(tlb_entry->page_size)
			- 1;

		/* Signal address range issue to the application */
		if( v_start < 0x10000000 || v_end > 0x1fffffff )
			cabr->boot_cfg_flags |=
				CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;

		/* Adjust half-size TLB per bootloader convention */
		if( tlb_entry->phy_page1 == ~0ULL )
			tlb_entry->phy_page1 = tlb_entry->phy_page0;

		/* Second-half only mapping not allowed */
		if( tlb_entry->phy_page0 == ~0ULL ) {
			fprintf(stderr, "ERROR: "
				"Bad TLB mapping at entry %u\n", i );
			return -1;
		}
	}

	if( i >= 32 ) {
		fprintf(stderr, "WARNING: "
			"More than 32 TLB entries have been used, "
			"could exceed bootloader limitation\n");
		/* TBD: this could be fatal */
	}

	return 0;
}



/*
 * Prepare a segment with command-line arguments
 *
 * The present version of "cvmx-app-init.c" requires the args buffer
 * to be KSEG0 addresable, hence one more named bootmem block is
 * allocated to store arguments, which is then shared by all cores
 * running this app.
 *
 * The args buffer contains first a 64-bit argc, then an array
 * of 64-bit physical addresses of each argument, then the actual
 * argument strings.
 */
int oct_args_prep( cvmx_app_boot_record_t * cabr_ptr, 
		int argc, char ** argv )
{
	off_t arg_off;
	size_t args_len;
	void * args_buf;
	paddr_t args_buf_phys;
	paddr_t * arg_ptr;
	int64_t * argc_ptr ;
	vmap_range_t vrange;
	int i;

	if( argc > OCTEON_ARGV_MAX_ARGS ) {
		fprintf(stderr, "WARNING: "
			"Argument count truncated from %d to %d\n",
			argc, OCTEON_ARGV_MAX_ARGS );
		argc = OCTEON_ARGV_MAX_ARGS;
	}

	if(verbose) fprintf(stderr, "INFO: argc=%d\n", argc );
	/* Calculate length of all argv[] strings */
	for( args_len = i = 0; i < argc; i ++ ) {
		args_len += strlen(argv[i]) +1;
		if(verbose) fprintf(stderr, "INFO: argv[%u]='%s'\n",
				i, argv[i] );
	}

	/* Add the heading overhead to the required buffer length */
	args_len += sizeof(app_vaddr_t) * (argc+1) ;
	args_len += sizeof(int64_t);

	/* Align size oo 128 Bytes */
	args_len += (1<<7) - 1;
	args_len &= ~((1<<7) -1);

	/* Build nane for bootmem block, used to cleanup on shutdown */
	snprintf( cabr_ptr->arg_buf_name, sizeof(cabr_ptr->arg_buf_name),
		"arg_buf_app_%04x",
		cabr_ptr->cabr_hotplug_app_index );

	if(verbose > 2) fprintf(stderr, "DEBUG: "
		"allocating named block \"%s\" size %lu\n",
		cabr_ptr->arg_buf_name, (unsigned long)args_len);

	/* Allocate bootmem  buffer for args contents */
	args_buf_phys = cvmx_bootmem_phy_named_block_alloc(
			args_len,
			0, 1ull<<29,	/* KSEG0 addresable */
			0x10,
			cabr_ptr->arg_buf_name, 0);
	if( args_buf_phys == 0ull ) {
		/* No need to free the buffer if failed to allocate */
		cabr_ptr->arg_buf_name[0] = '\0';
		fprintf(stderr, "ERROR: "
			"Failed to allocate arg buffer block\n");
		return -1;
	}

	/* This is a KSEG0 addresable block */
	args_buf = map_phys_range( args_buf_phys, args_len, &vrange);

	if( args_buf == NULL ) {
		fprintf(stderr, "ERROR: "
			"Failed to map arg buffer block into virtual space\n");
		return -1;
	}

	if(verbose>2) fprintf(stderr, "DEBUG: "
		"arg block at phys %#llx virt %p\n",
		(unsigned long long) args_buf_phys, args_buf);


	/* Copy argument strings to the buffer, and set target arg pointers */
	arg_off = sizeof(int64_t) + (argc + 1) * sizeof(paddr_t);
	for(i = 0; i < argc; i ++ ) {
		strcpy( args_buf + arg_off, argv[i] );
		arg_ptr = args_buf + sizeof(int64_t) + i * sizeof(paddr_t);
		* arg_ptr = cabr_ptr->argv[i] = args_buf_phys + arg_off;
		if(verbose> 2) fprintf(stderr, "DEBUG: "
				"cabr_argv[%u]=@%llx='%s'\n", i, 
				(unsigned long long) cabr_ptr->argv[i],
				(char *) args_buf + arg_off );
		arg_off += strlen(argv[i]) + 1;
	}

	/* Place a null pointer at the end */
	arg_ptr = args_buf + sizeof(int64_t) + argc * sizeof(paddr_t);
	* arg_ptr = 0ull;

	/* Lastly, place argc into the arg buffer */
	argc_ptr = args_buf ;
	* argc_ptr = cabr_ptr->argc = argc;

	if( verbose ) fprintf(stderr, "INFO: "
		"Command-line argc %d size %u/%u paddr=%#llx\n",
		argc, (unsigned) arg_off, (unsigned) args_len,
		(unsigned long long) args_buf_phys  );


	unmap_phys_range( &vrange);

	return arg_off;
}

void oct_args_free( cvmx_app_boot_record_t * cabr_ptr )
{
	if(verbose) fprintf(stderr, "INFO: "
		"Freeing argument buffer for app %d\n",
		cabr_ptr->cabr_hotplug_app_index );

	if( strlen( cabr_ptr->arg_buf_name ) == 0 )
		return ;

	cvmx_bootmem_free_named( cabr_ptr->arg_buf_name );
	cabr_ptr->arg_buf_name[0] = '\0';
}

/*
 * Setup command line arguments that will be passed to the application
 *
 */
static int oct_boot_desc_args_setup( 
		cvmx_app_boot_record_t * cabr,
		struct octeon_boot_descriptor *boot_desc
		)
{
	unsigned i, argc, le;

	/*
	 * TBD:
	 * the need for word-swapping actually depends on the difference
	 * between host and target endianness, and not on the endianness
	 * of the application alone.
	 */

	if( cabr->boot_info_flags & BOOT_INFO_FLAG_LITTLE_ENDIAN )
		le = 1;
	else
		le = 0;

	boot_desc->argc = argc = cabr->argc ;
	for( i = 0; i < argc; i ++ ) {
		/* XXX reverse pairs for Little-Endian */
		boot_desc->argv[i ^ le] = cabr->argv[i];
	}
	return i;
}

/*
 * Initialize default exception handlers
 */
static void oct_boot_init_except_handlers( paddr_t exc_base )
{
	void *addr;
	unsigned i;
	vmap_range_t vrange;
	/* This is MIPS-specific */
	static const uint32_t nop_loop[2] = { 0x1000ffff, 0x0 };

	addr = map_phys_range( exc_base, 0x800, &vrange);
	memset(addr, 0, 0x800);

        /* Set up some dummy loops for exception handlers to help with debug */
	for(i = 0; i < 0x700; i += 0x80 )
		memcpy(addr + i, nop_loop, sizeof(nop_loop));

	unmap_phys_range( &vrange);
}

/*
 * Setting of hardware attributes in SE boot info put in a separate function
 * for readability
 */
static void oct_boot_init_hw_attr(
	struct app_params * params,
	cvmx_app_boot_record_t * cabr,
	cvmx_bootinfo_t * cbi
	)
{
        const linux_app_boot_info_t *labi;
	const linux_app_global_data_t *lagd;

	/* init global pointers */
	labi = oct_app_get_labi();
	lagd = &labi->gd;

	/* FIXME: Do not use LAGD, use sysinfo instead */

	/* Print out some fields from LAGD */
	if(verbose > 1) {
		fprintf(stderr, "DEBUG: "
			"lagd baud %lu have_console %d uart %d "
			" ram_size %#llx cpu clk %lu MHz\n",
			(unsigned long) lagd->baudrate, lagd->have_console,
			lagd->console_uart, (unsigned long long)lagd->ram_size,
			(unsigned long) lagd->cpu_clock_mhz );

	}

	/* Collect and transform hardware attributes as needed */
	cbi->dram_size = lagd->ram_size/(1024 * 1024);
	cbi->dclock_hz = lagd->ddr_clock_mhz * 1000000;
	cbi->eclock_hz = lagd->cpu_clock_mhz;// lagd->cpu_clock_mhz is in [Hz] units
	cbi->board_type = lagd->board_desc.board_type;
	cbi->board_rev_major = lagd->board_desc.rev_major;
	cbi->board_rev_minor = lagd->board_desc.rev_minor;
	cbi->mac_addr_count = lagd->mac_desc.count;
 
	memcpy(cbi->mac_addr_base,
                       (void *)lagd->mac_desc.mac_addr_base,
                       sizeof(cbi->mac_addr_base));
	strncpy(cbi->board_serial_number,
                        (char *)(lagd->board_desc.serial_str),
                        CVMX_BOOTINFO_OCTEON_SERIAL_LEN);

	cbi->compact_flash_common_base_addr =
                    labi->compact_flash_common_base_addr;
	cbi->compact_flash_attribute_base_addr =
                    labi->compact_flash_attribute_base_addr;
	cbi->led_display_base_addr =
                    labi->led_display_base_addr;
	cbi->dfa_ref_clock_hz =
                    lagd->clock_desc.dfa_ref_clock_mhz_x_8 / 8 * 1000000;
#if 0
	// XXX this flag is temporarily disabled until I can figure out why
	/* Console redirection details */
	if(labi->pci_console_active) 
		cabr->boot_desc_flags |= OCTEON_BL_FLAG_CONSOLE_PCI;
#endif
	if( lagd->console_uart == 1 || params->console_uart == 1) {
		if(verbose) fprintf(stderr, "INFO: "
			"Application console is on UART1\n");
		cabr->boot_desc_flags |= OCTEON_BL_FLAG_CONSOLE_UART1;
	}

	/* Hardware-dependent flags */
	if (labi->icache_prefetch_disable)
		cabr->boot_info_flags |= BOOT_INFO_FLAG_DISABLE_ICACHE_PREFETCH;

	/* LE is not really supported for SE */
	if( cabr->boot_info_flags & BOOT_INFO_FLAG_LITTLE_ENDIAN )
		cabr->boot_desc_flags |= OCTEON_BOOT_DESC_LITTLE_ENDIAN;

	/* Translate boot flags to config flags */
	if( cabr->boot_desc_flags & OCTEON_BL_FLAG_DEBUG )
		cabr->boot_cfg_flags |= CVMX_BOOTINFO_CFG_FLAG_DEBUG;
	if( cabr->boot_desc_flags & OCTEON_BL_FLAG_BREAK )
		cabr->boot_cfg_flags |= CVMX_BOOTINFO_CFG_FLAG_BREAK;
	if( cabr->boot_desc_flags & OCTEON_BL_FLAG_NO_MAGIC )
		cabr->boot_cfg_flags |= CVMX_BOOTINFO_CFG_FLAG_NO_MAGIC;

	/*
	 * These flags are not supported:
	 * Compile-time config		cvmx_boot_info->flags
	 * CONFIG_OCTEON_PCI_HOST	CVMX_BOOTINFO_CFG_FLAG_PCI_HOST
	 * CONFIG_OCTEON_PCI_TARGET	CVMX_BOOTINFO_CFG_FLAG_PCI_TARGET
	 */
	cbi->flags |= cabr->boot_desc_flags;
	cbi->config_flags |= cabr->boot_cfg_flags;

}

/*
 * Setup the content of the boot_info_block for bootloader
 *
 * This is done only for a single core, i.e the primary per-app
 * boot record, and subsequently cloned for every running core.
 */
int oct_boot_desc_setup( 
		struct app_params * params,
		cvmx_app_boot_record_t * cabr_ptr
		)
{
	struct octeon_boot_descriptor oct_boot_desc;
	cvmx_bootinfo_t cvmx_boot_info;
	uint64_t cur_exception_base; 		/* Exception base address */
        cvmx_sysinfo_t *si;
	off_t off;
	paddr_t paddr;
	int core;
	int rc;


        si = cvmx_sysinfo_get();
	core = cabr_ptr->cabr_core ;

	memset( &oct_boot_desc , 0, sizeof(oct_boot_desc));
	memset( &cvmx_boot_info, 0, sizeof(cvmx_boot_info));

	/* Set cur_exception_base based on the application slot */
	cur_exception_base = SE_APP_EXCEPTION_BASE +
		params->app_index * SE_APP_EXCEPTION_SIZE;

	if(verbose>2) fprintf(stderr, "DEBUG: app %d core %d: exception base (phys) %#llx\n", 
			params->app_index, core,
			(unsigned long long) cur_exception_base);

	/* Initialize exception vectors for init core only */
	if( core == cabr_ptr->cabr_init_core ) {
		oct_boot_init_except_handlers( cur_exception_base );
		cabr_ptr->boot_desc_flags |= BOOT_FLAG_INIT_CORE;
	}
	/* This flag is supposedly needed if not running simulation */
	cabr_ptr->boot_desc_flags |= OCTEON_BL_FLAG_NO_MAGIC;

	/* Fixup TLB map for bootloader conventions */
	rc = oct_boot_desc_tlb_adjust( cabr_ptr );

	if( rc < 0 ) {
		fprintf(stderr, "ERROR: in desc_tlb_adjust\n");
		return rc;
	}

	/* Write TLB map physical address into boot_info_block */
	paddr = global_master_ptr->cabr_paddr[core];
	off = offsetof( cvmx_app_boot_record_t, tlb_entries );
	paddr += off;

	/* XXX hack due to pointer type declaration XXX */
#if 1
	{
	uint32_t *dst, paddr32 = MAKE_KSEG0(paddr);
	dst = (void *) &(cabr_ptr->cabr_boot_info_block.tlb_entries);
	dst[0] = dst[1] = paddr32;
	}
#else
	cabr_ptr->cabr_boot_info_block.tlb_entries = MAKE_KSEG0(paddr);
#endif

	/* Save actual app entry point in boot_info_block */
	cabr_ptr->cabr_boot_info_block.entry_point =
		cabr_ptr->cabr_entry_point ;

	/* Populate argc/argv fields in oct_boot_desc */
	rc = oct_boot_desc_args_setup( cabr_ptr, &oct_boot_desc);

	if( rc < 0 ) {
		fprintf(stderr, "ERROR: in desc_arg_setup\n");
		return rc;
	}

	/* Find top of stack */
	cabr_ptr->cabr_boot_info_block.stack_top =
		cabr_ptr->cabr_heap_base + cabr_ptr->cabr_stack_size;
	/* Align stack top to 16 bytes */
	cabr_ptr->cabr_boot_info_block.stack_top &= ~0xfull;

	if(verbose>2) fprintf(stderr, "DEBUG: "
		"heap base %#llx size %#x, stack size %#x, stack top %#llx\n",
		(unsigned long long) cabr_ptr->cabr_heap_base,
		cabr_ptr->cabr_heap_size,
		cabr_ptr->cabr_stack_size,
		(unsigned long long)cabr_ptr->cabr_boot_info_block.stack_top
		);

	oct_boot_desc.stack_top = cabr_ptr->cabr_boot_info_block.stack_top ;

	/* Calcolate heap range */
	oct_boot_desc.heap_base = cabr_ptr->cabr_heap_base +
		cabr_ptr->cabr_stack_size;

	oct_boot_desc.heap_end = cabr_ptr->cabr_heap_base +
		cabr_ptr->cabr_heap_size + cabr_ptr->cabr_stack_size;

	/* Miscleneneous oct_boot_desc fields */
	oct_boot_desc.desc_version = OCTEON_CURRENT_DESC_VERSION;
	oct_boot_desc.desc_size = sizeof(oct_boot_desc);
	oct_boot_desc.debugger_flags_base_addr =
		BOOTLOADER_DEBUG_FLAGS_BASE;

	cvmx_boot_info.major_version =	CVMX_BOOTINFO_MAJ_VER;
	cvmx_boot_info.minor_version =	CVMX_BOOTINFO_MIN_VER;

	cvmx_coremask_copy( &cvmx_boot_info.ext_core_mask, &params->coremask );
	cvmx_boot_info.core_mask = cvmx_coremask_get32( &params->coremask );
	cvmx_boot_info.phy_mem_desc_addr = si->phy_mem_desc_addr ;
	cvmx_boot_info.stack_top = cabr_ptr->cabr_boot_info_block.stack_top;

	/* Exception base goes to boot info block and cvmx desc */
	cabr_ptr->cabr_boot_info_block.exception_base = 
	cvmx_boot_info.exception_base_addr = cur_exception_base;

	/* Fill assorted hardware attributes for SE */
	oct_boot_init_hw_attr( params, cabr_ptr,  &cvmx_boot_info );

	/* Copy prepared flags to each of these structs */
	oct_boot_desc.flags |= cabr_ptr->boot_desc_flags;
	cvmx_boot_info.config_flags |= cabr_ptr->boot_cfg_flags;

	cabr_ptr->cabr_boot_info_block.flags |= cabr_ptr->boot_info_flags;

	oct_boot_desc.eclock_hz = cvmx_boot_info.eclock_hz;

	/* Locate oct_boot_desc at the top of the heap */
	cabr_ptr->cabr_boot_info_block.boot_desc_addr =
		oct_boot_desc.heap_end - sizeof(oct_boot_desc);
	cabr_ptr->cabr_boot_info_block.boot_desc_addr &= ~0x7ull;


	/* Replicate some fields into cvmx_desc too */
	cvmx_boot_info.heap_base = oct_boot_desc.heap_base;
	cvmx_boot_info.heap_end = oct_boot_desc.heap_end ;
	cvmx_boot_info.stack_size = cabr_ptr->cabr_stack_size;
	cvmx_boot_info.fdt_addr = (uint32_t) OCTEON_FDT_BASE;


	/* Locate cvmx_desc right after the oct_boot_desc */
	oct_boot_desc.cvmx_desc_vaddr =
		cabr_ptr->cabr_boot_info_block.boot_desc_addr -
		sizeof(cvmx_bootinfo_t);
	oct_boot_desc.cvmx_desc_vaddr &= ~0x7ull;
	cabr_ptr->cabr_boot_info_block.cvmx_desc_addr =
		oct_boot_desc.cvmx_desc_vaddr;

	/* Print info block for debug */
	print_boot_block( &cabr_ptr->cabr_boot_info_block );

	/* Print heap/stack locations for debug */
	if(verbose>2) fprintf(stderr, "DEBUG:"
		" oct_boot_desc:"
		" stack top  %#llx"
		" heap base %#llx"
		" heap_end %#llx"
		" flags %#x"
		"\n",
		(unsigned long long) oct_boot_desc.stack_top,
		(unsigned long long) oct_boot_desc.heap_base,
		(unsigned long long) oct_boot_desc.heap_end,
		oct_boot_desc.flags);

	if(verbose>2) fprintf(stderr, "DEBUG: "
		" cvmx_bootinfo flags=%#x\n",
		cvmx_boot_info.flags );
		
	/* Copy oct_boot_desc into core heap area */
	rc = oct_tlb_memcpy( cabr_ptr->tlb_entries,
		(app_vaddr_t) cabr_ptr->cabr_boot_info_block.boot_desc_addr,
		&oct_boot_desc,
		sizeof( oct_boot_desc ));

	if( rc < 0 ) {
		fprintf(stderr, "ERROR: failed to copy oct_boot_desc\n");
		return rc;
	}

	rc = oct_tlb_memcpy( cabr_ptr->tlb_entries,
		(app_vaddr_t) cabr_ptr->cabr_boot_info_block.cvmx_desc_addr,
		&cvmx_boot_info,
		sizeof(cvmx_boot_info));

	if( rc < 0 ) {
		fprintf(stderr, "ERROR: failed to copy cvmx_boot_into\n");
		return rc;
	}

	return 0;
}

/* End of File */
