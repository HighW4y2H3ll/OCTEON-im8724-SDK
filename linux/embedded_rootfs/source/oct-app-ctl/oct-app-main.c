/***********************license start************************************
 * Copyright (c) 2013 Cavium Inc. (support@cavium.com). All rights
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
 **********************license end**************************************/

/*
 * @file
 *
 * $Id: oct-app-main.c 167085 2017-10-12 21:41:53Z cchavva $
 *
 * DESCRIPTION
 *
 * This is the top level source for the utility that controls
 * SE (bare metal) application from Linux.
 *
 * It supports 4 basic operations:
 * "boot" will start a new app on a number of cores
 * "add" will hotplug additional cores onto a running app
 * "del" will unplug a number of cores from a running app
 * "shutdown" will kill an app and release all the core it uses
 *
 * One of the use cases for this is to live-update a data-plane
 * bare-metal app from the control-plane system running on Linux.\
 * Another use case is to be able to migrate processor cores between
 * the control-plane (Linux) and data-plane (SE) to dynamically adjust
 * the resources to the traffic load pattern.
 *
 * This source file contains the top-level logic for these operations.
 *
 */

#define _GNU_SOURCE
 
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sched.h>
#include <cvmx-coremask.h>
#include <cvmx-spinlock.h>
#include <cvmx-mbox.h>
#include <cvmx-app-hotplug.h>
#include <cvmx-boot-vector.h>

#include "oct-app.h"

#define CVMX_USE_1_TO_1_TLB_MAPPINGS 0	/* Not supported in user-land */

/**** Global Variables ***/
int verbose = 0;
const char * progname;			/* original argv[0] */
void * 	dram_map_addr;			/* bootloader params at low mem */
int	devmem_fd = -1;			/* /dev/mem file descriptor */

/* Hotplug common data structure */
cvmx_app_hotplug_info_t *hotplug_info_ptr;
cvmx_app_hotplug_global_t *hotplug_global_ptr;
cvmx_app_boot_master_t * global_master_ptr ;

uint64_t global_linux_boot_info_phys = 0;

static int oct_app_cores_available(void);

static const char __help_common_opt[] =
			"\t<common-options> := "
			"[-verbose[=N]]"
			"\n";

/*
 * Several help functions to indicate indended usage
 */
static int oct_app_main_help(void)
{
	fprintf(stderr, "Usage:\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t%s <action> ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <action-options>\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t<action> := "
			"boot, add, del, shut\n");
	fprintf(stderr, __help_common_opt);
	return -1;
}

static int oct_app_boot_help(void)
{
	fprintf(stderr, "Usage:\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t%s boot ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <boot-options> <app-file-name>"
		"<app-options>\n");
	fprintf(stderr, "\t<boot-options> := "
			"[-numcores=<numcores>] "
			"[-mask=<coremask>>] "
			"[-stack=<stack-size>] "
			"[-heap=<heap-size>] "
			"[-console=<console-port>] "
			"[-debug] "
			"\n");
	fprintf(stderr, __help_common_opt);
	return -1;
}

static int oct_app_add_help(void)
{
	fprintf(stderr, "Usage:\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t%s add ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <add-options> "
			"[-iindex=<app-index> | <app-file-name>]"
		"\n");
	fprintf(stderr, "\t<add-options> := "
			"[-numcores=<numcores>] "
			"[-mask=<coremask>>] "
			"\n");
	fprintf(stderr, __help_common_opt);
	fprintf(stderr, "\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}
 
static int oct_app_del_help(void)
{
	fprintf(stderr, "Usage:\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t%s del ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <del-options> "
		"[-iindex=<app-index> | <app-file-name>]"
		"\n");
	fprintf(stderr, "\t<del-options> := "
			"[-n=<numcores>] "
			"[-m=<coremask>>] "
			"\n");
	fprintf(stderr, __help_common_opt);
	fprintf(stderr, "\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}
 

static int oct_app_kill_help(void)
{
	fprintf(stderr, "Usage:\n");
	if( strstr(progname, "ctl"))
		fprintf(stderr, "\t%s shut ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> "
			"[-iindex=<app-index> | <app-file-name>] "
		"\n");
	fprintf(stderr, __help_common_opt);
	fprintf(stderr, "\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}
 


/*
 * Parse generic command options
 */
int oct_app_cmd_genopt( int argc, char ** argv, struct app_params * params )
{
	char * sval = NULL;
	char opt;

	while( argc > 0 ) {
		if( argv[0][0] != '-' ) {
			opt = '\0'; sval = "";
			break;
		} else {
			opt = argv[0][1];
			if((sval = strchr(argv[0], '=')) != NULL) {
				sval = sval + 1;
			} else if( argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if( opt == 'v' ) {
			verbose = 1;
			if (sval != NULL)
				verbose = strtoul(sval, NULL, 0);
		}
	
		argv++; argc--;
	}

	return 0;
}

/*
 * Parse "boot" action options
 */
int oct_app_cmd_boot_getopt(int argc, char ** argv, struct app_params * params)
{
	const char * sval = NULL;
	char opt;

	params->app_name = NULL;

	while( argc > 0 ) {
		if( argv[0][0] != '-' ) {
			opt = '\0'; sval = "";
			params->app_name = argv[0];
			params->argc = argc;
			params->argv = argv;
			/* Arguments following <app-name> are <app-args> */
			break;
		} 

		opt = argv[0][1];
		sval = "";

		/* First check for valueless options */
		if( opt == 'd' )
	    		params->boot_desc_flags |= OCTEON_BL_FLAG_DEBUG;
		else if( opt == 'b' )
			params->boot_desc_flags |= OCTEON_BL_FLAG_BREAK;
		else {
			/* Get the option value string */
			if((sval = strchr(argv[0], '=')) != NULL) {
				sval = sval + 1;
			} else if( argc > 1 && isdigit(argv[1][0]) ) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if( opt == 'n' ) {
			params->numcores = strtoul(sval, NULL, 0);
		}
		if( opt == 'c' ) {
			params->console_uart = strtoul(sval, NULL, 0);
		}
		if( opt == 'm' ) {
			cvmx_coremask_set64( &params->coremask,
				strtoull( sval, NULL, 16));
		}

		if( opt == 's' ) {
			params->stack_size = strtoul(sval, NULL, 0);
			if (params->stack_size < ( 1 << 20))
				params->stack_size = 1 << 20 ;
		}
	
		if( opt == 'h' ) {
			params->heap_size = strtoul(sval, NULL, 0);
			if( params->heap_size < (3<<20))
				params->heap_size = 3<<20;
		}
	

		//XXX- verify that no options are missing here

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if( params->app_name == NULL ) {
		fprintf(stderr, "ERROR: "
			"app name is required\n");
		return -1;
	}

	return 0;
}

/*
 * Parse "add" action options
 */
int oct_app_cmd_add_getopt(int argc, char ** argv, struct app_params * params)
{
	const char * sval = NULL;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while( argc > 0 ) {
		if( argv[0][0] != '-' ) {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			if((sval = strchr(argv[0], '=')) != NULL) {
				sval = sval + 1;
			} else if( argc > 1 && isdigit(argv[1][0]) ) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if( opt == 'n' ) {
			params->numcores = strtoul(sval ,NULL, 0);
		}

		if( opt == 'i' ) {
			params->app_index = strtoul(sval ,NULL, 0);
		}

		if( opt == 'm' ) {
			cvmx_coremask_set64( &params->coremask,
				strtoull( sval, NULL, 16));
		}

		//XXX- verify that no options are missing here

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if( params->app_name == NULL && params->app_index == -1 ) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}

/*
 * Parse "del" action options
 */
int oct_app_cmd_del_getopt(int argc, char ** argv, struct app_params * params)
{
	const char * sval = NULL;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while( argc > 0 ) {
		if( argv[0][0] != '-' ) {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			if((sval = strchr(argv[0], '=')) != NULL) {
				sval = sval + 1;
			} else if( argc > 1 && isdigit(argv[1][0]) ) {
				argc--; argv++;
				sval = argv[0];
			}
		}
		if( opt == 'n' ) {
			params->numcores = strtoul(sval ,NULL, 0);
		}
		if( opt == 'm' ) {
			cvmx_coremask_set64( &params->coremask,
				strtoull( sval, NULL, 16));
			params->numcores = 0;
		}

		if( opt == 'i' ) {
			params->app_index = strtoul(sval, NULL, 0);
		}
	
		//XXX- verify that no options are missing here

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if( params->app_name == NULL && params->app_index == -1 ) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}
/*
 * Parse "kill" action options
 */
int oct_app_cmd_kill_getopt(int argc, char ** argv, struct app_params * params)
{
	const char * sval;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while( argc > 0 ) {
		if( argv[0][0] != '-' ) {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			if((sval = strchr(argv[0], '=')) != NULL) {
				sval = sval + 1;
			} else if( argc > 1 && isdigit(argv[1][0]) ) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if( opt == 'i' ) {
			params->app_index = strtoul(sval, NULL, 0);
		}
	
		//XXX- verify that no options are missing here

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if( params->app_name == NULL && params->app_index == -1 ) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}


/*
 * Map bootloader memory
 * The lower 1MiB of DRAM contains important bootloader data structures
 * namely boot_vect[core] is located here for old bootloader versions.
 */
int map_bootloader_mem(void)
{
	if( devmem_fd < 0 )
		if ((devmem_fd = open("/dev/mem", O_RDWR)) < 0) {
		    perror("open");
		    return -1;
		}

	/* Map the entire bootloader memory */
	if (!dram_map_addr) {
		dram_map_addr = mmap(NULL, 0x100000,
			PROT_READ|PROT_WRITE, MAP_SHARED, devmem_fd, 0x0) ;
	if (dram_map_addr == MAP_FAILED) {
           perror("mmap");
           return -1;
        }
    }
    return 0;
}

/*
 * Map the common hotplog data structure
 */
static int map_hotplug_info(void)
{
	cvmx_bootmem_named_block_desc_t *block_desc;
	size_t pg_sz = sysconf(_SC_PAGESIZE), size;
	off_t offset;
	char *vaddr;

	if( devmem_fd < 0 ) {
		if ((devmem_fd = open("/dev/mem", O_RDWR)) < 0) {
		    perror("open");
		    return -1;
		}
	}

	/* Map low memory needed for any operation */
	if (map_bootloader_mem()) {
		fprintf(stderr,"ERROR: unable to map bootloader memory\n");
		return -1;
	}

	/* Find global hotplug named block 
	 * it should have been created by bootloader or kernel
	 */
	block_desc = (void *) cvmx_bootmem_find_named_block(
			CVMX_APP_HOTPLUG_INFO_REGION_NAME);

	if (!block_desc) {
		fprintf(stderr, "ERROR: "
			"Hotplug info region not found\n");
		return -1;
	}

	if(verbose > 2) fprintf(stderr, "DEBUG: "
		"hotplug info phy addr 0x%llx ptr %p\n", 
		(unsigned long long) block_desc->base_addr, 
		cvmx_phys_to_ptr(block_desc->base_addr));

        /* 
         * We need to mmap() this memory, since this was allocated from the
         * kernel bootup code and does not reside in the RESERVE32 region. 
         */
        size = CVMX_APP_HOTPLUG_INFO_REGION_SIZE + pg_sz-1;
        offset = block_desc->base_addr & ~(pg_sz-1);
        vaddr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED,
		devmem_fd, offset) ;
        if (vaddr == MAP_FAILED) {
             perror("mmap");
             return -1;
        }

	hotplug_global_ptr =
		(cvmx_app_hotplug_global_t *)
		(vaddr + ( block_desc->base_addr & (pg_sz-1)));

	if (hotplug_global_ptr->magic_version != CVMX_HOTPLUG_MAGIC_VERSION) {
		fprintf(stderr, "ERROR: "
			"Invalid hotplug structure found, "
			"may be old version\n");
		return -1;
	}

	hotplug_info_ptr = hotplug_global_ptr->hotplug_info_array;

	return 0;
}

/*
 * Display current running application
 */
static int oct_app_info_show(const struct app_params * params )
{
	unsigned int i;
	int app_cnt=0;
	cvmx_app_hotplug_info_t *hpinfo;  
	char hexstr1[ CVMX_MIPS_MAX_CORES/4+1  ] ;
	char hexstr2[ CVMX_MIPS_MAX_CORES/4+1  ] ;

	for (i=0; i<CVMX_APP_HOTPLUG_MAX_APPS; i++) {

		hpinfo = &hotplug_info_ptr[i];
		if (!hpinfo->valid_magic)
			continue;
		app_cnt++;

		if (app_cnt == 1) {
			printf("%5s %20s %24s %24s\n",
				"Index", "Application Name",
				"Running Coremask", "Hotplug Active Coremask");
			printf( "----------------------------------------"
				"----------------------------------------\n");
		}
		cvmx_coremask_bmp2str(
			&hpinfo->coremask, hexstr1 );
		cvmx_coremask_bmp2str(
			&hpinfo->hotplug_activated_coremask, hexstr2 );
		printf("  %3d %20s %24s %24s\n",
			i,hpinfo->app_name, hexstr1, hexstr2 );
	} /* for i */

	/* Refresh availbale coremask if needed */
	(void) oct_app_cores_available();

	/* Count actual coures available */
	i = cvmx_coremask_get_core_count( &hotplug_global_ptr->avail_coremask );

	cvmx_coremask_bmp2str( &hotplug_global_ptr->avail_coremask, hexstr1 );
	printf("Available %d cores, Coremask=0x%s\n", i, hexstr1 );
	return 0;
}

/*
 * Create or find a Master Boot Record named block
 */
static int app_boot_master_create(void)
{
	size_t pg_sz = sysconf(_SC_PAGESIZE) ;
	void  *vaddr;
	const cvmx_bootmem_named_block_desc_t * block_desc;
	cvmx_app_boot_master_t * master_ptr ;

	if ( global_master_ptr )
		return 0; /* globals are already setup */

	if (map_bootloader_mem()) {
		fprintf(stderr, "ERROR: unable to map bootloader memory\n");
		return -1;
	}

	/* Find the master boot block if one exists */
	block_desc = cvmx_bootmem_find_named_block( CVMX_APP_BOOT_MASTER_NAME );

	if (NULL != block_desc) {
		master_ptr = cvmx_phys_to_ptr(block_desc->base_addr);
		/* verify master boot block signature */
		if (master_ptr->magic != 0xb00710c4) {
			fprintf( stderr, "ERROR: "
				"invalid master boot block\n");
			return -1;
		}
	} else {
		/* Allocate the master boot block if it doesn't already exist */
		/*
		* NOTE: This allocation is non-atomic
		* it is not used by an app, so no need to be KSEG0 mappable
		*/
		vaddr = cvmx_bootmem_alloc_named_range(
			sizeof(cvmx_app_boot_master_t), 
        		0x0, 0x0,
			pg_sz,
			CVMX_APP_BOOT_MASTER_NAME );
		if (vaddr == NULL) {
			fprintf( stderr, "ERROR: "
				"failed to allocate master boot block\n");
			return -1;
		}
		memset( vaddr, 0, sizeof(cvmx_app_boot_master_t));
		master_ptr = vaddr;
		master_ptr->magic = 0xb00710c4;
	}

	/* Set the global pointer */
	global_master_ptr = master_ptr;

    return 0;
}

/*
 * Initialize the primary boot record
 */
static int cvmx_app_boot_record_init( 
	struct app_params * params,
	cvmx_app_boot_record_t * cabr)
{

	memset( &cabr->tlb_entries, 0, sizeof(cabr->tlb_entries) );

	/* Fill in hotplug application slot */
	cabr->cabr_hotplug_app_index = params->app_index ;
	cabr->no_mark_private_data = params->no_mark_private_data;

	cabr->cabr_stack_size = params->stack_size;
	cabr->cabr_heap_size = params->heap_size;

	return 0;
}

/*
 * Get access to boot-loader LABI structure
 * map it once if necesary.
 * Return NULL if the structure does not pass sanity or can not be mapped.
 */
linux_app_boot_info_t * oct_app_get_labi(void)
{
	static linux_app_boot_info_t * labi = NULL;

	assert( dram_map_addr != NULL );

	if( labi != NULL)
		return labi;

	labi = (linux_app_boot_info_t *)
		(dram_map_addr + LABI_ADDR_IN_BOOTLOADER);

	if (labi->labi_signature != LABI_SIGNATURE) {
		fprintf(stderr, "ERROR: "
			"The bootloader version is incorrect\n");
		return NULL;
	}


	return labi;
}

/*
 * Clone the primary boot record for each of the cores running an app
 */
static void oct_app_boot_record_clone(
		const cvmx_app_boot_record_t * cabr_primary,
		cvmx_app_boot_record_t * cabr_core,
		unsigned core
		)
{
	int i;

	memset( &cabr_core->tlb_entries, 0, sizeof(cabr_core->tlb_entries) );

	cabr_core->cabr_hotplug_app_index = 
		cabr_primary->cabr_hotplug_app_index;

	cabr_core->cabr_init_core =
		cabr_primary->cabr_init_core;
	cabr_core->cabr_core = core;

	cabr_core->no_mark_private_data = 
		cabr_primary->no_mark_private_data;
	cabr_core->cabr_entry_point  = 
		cabr_primary->cabr_entry_point ;

	cabr_core->cabr_heap_base =
		cabr_primary->cabr_heap_base;
	cabr_core->cabr_stack_size =
		cabr_primary->cabr_stack_size;
	cabr_core->cabr_heap_size =
		cabr_primary->cabr_heap_size;

	cabr_core->cabr_boot_info_block =
		cabr_primary->cabr_boot_info_block;

	cabr_core->boot_info_flags =
		cabr_primary->boot_info_flags ;
	cabr_core->boot_desc_flags =
		cabr_primary->boot_desc_flags ;
	cabr_core->boot_cfg_flags =
		cabr_primary->boot_cfg_flags ;

	cabr_core->argc =
		cabr_primary->argc ;

	for(i = 0; i < cabr_primary->argc && i < OCTEON_ARGV_MAX_ARGS; i++)
		cabr_core->argv[i] = cabr_primary->argv[i];

	strncpy( cabr_core->arg_buf_name, cabr_primary->arg_buf_name,
		sizeof(cabr_core->arg_buf_name));
}


/*
 * Access the per-core Boot Record data structure
 * or allocate one if not already present
 * If <core> is -1, the primary per-app boot record is returned.
 */
static cvmx_app_boot_record_t *  app_boot_record_core_get(
		struct app_params * params,
		int core)
{
	cvmx_app_hotplug_info_t * hip ;
	cvmx_app_boot_record_t * cabr_ptr ;
	size_t pg_sz = sysconf(_SC_PAGESIZE), size;
	uint64_t paddr ;
	void * vaddr ;
	off_t offset;
	int allocated = 0 ;

	hip = &hotplug_global_ptr->hotplug_info_array[ params->app_index ];

	if( core >= 0 )
		paddr = global_master_ptr->cabr_paddr[core];
	else {
		paddr = hip->cvmx_app_boot_record_ptr;
	}


	/* Per-core boot record needs to be allocated */
	if( paddr == 0ULL ) {
		/* This structure must be KSEG0 addresable */
		paddr = cvmx_bootmem_phy_alloc(
				sizeof(cvmx_app_boot_record_t),
				0, 1ull<<29, 128, 0);
		if (paddr == -1ULL || paddr == 0ULL) {
			fprintf(stderr, "ERROR: "
				"error allocationg per-core boot record "
				" for core %d\n",
				core );
			   return NULL;
		}
		allocated = 1 ;

		if(verbose>2) fprintf(stderr, "DEBUG: "
			" allocated core %d cabr paddr=%#llx\n",
			core, (unsigned long long) paddr );
	}

	/* Store newly allocated per-core stucture in master record */
	if( allocated ) {
		if( core >= 0 )
			global_master_ptr->cabr_paddr[core] = paddr ;
		else {
			hip->cvmx_app_boot_record_ptr = paddr;
		}
	}

	/* Map the new physical block and obtain virtual address */
        size = sizeof( cvmx_app_boot_record_t )  + pg_sz-1;
        offset = paddr & ~(pg_sz-1);

	/* TBD: maybe mapping an area already mapped, will it work ? */
        vaddr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED,
		devmem_fd, offset) ;

        if (vaddr  == MAP_FAILED) {
		/* XXX- cleanup allocated "cabr" ? */
		perror("mmap");
		return NULL;
        }

        cabr_ptr = vaddr + (paddr & (pg_sz-1));

	if( allocated ) {
		memset(cabr_ptr, 0, sizeof(* cabr_ptr ));
		cabr_ptr->cabr_magic0 = CVMX_APP_BOOT_RECORD_MAGIC;
		cabr_ptr->cabr_magic1 = CVMX_APP_BOOT_RECORD_MAGIC;
		cabr_ptr->cabr_paddr = paddr;
		cabr_ptr->cabr_core = core;
		cabr_ptr->cabr_hotplug_app_index = 
				params->app_index;
	}

	return cabr_ptr ;
}

/*
 * Free boot record memory
 *
 * This should work for either per-core or primary boot record
 */
static int app_boot_record_free( cvmx_app_boot_record_t * cabr )
{
	cvmx_app_hotplug_info_t * hip ;
	paddr_t paddr;
	unsigned app_index;
	short core;
	int ret;
	bool primary;

	if( cabr == NULL )
		return 0;

	core = cabr->cabr_core;
	app_index = cabr->cabr_hotplug_app_index;
	hip = &hotplug_global_ptr->hotplug_info_array[ app_index ];

	assert( core == -1 || core < CVMX_MAX_CORES );

	assert( app_index < CVMX_APP_HOTPLUG_MAX_APPS );

	assert( cabr->cabr_magic0 == CVMX_APP_BOOT_RECORD_MAGIC );
	assert( cabr->cabr_magic1 == CVMX_APP_BOOT_RECORD_MAGIC );

	/* Obtain physical address for this boot record, and delete reference */
	if( core >= 0 ) {
		paddr = global_master_ptr->cabr_paddr[core];
		global_master_ptr->cabr_paddr[core] = 0ull;
		primary = false;
		if(verbose>1) fprintf(stderr,"INFO: "
				"Freeing boot record for core %d\n", core );
	} else {
		paddr = hip->cvmx_app_boot_record_ptr;
		hip->cvmx_app_boot_record_ptr = 0ull;
		/* shutdown overwrites *<hip>, use alternate storage */
		if( paddr == 0ull )
			paddr = cabr->cabr_paddr;
		primary = true;
		if(verbose>1) fprintf(stderr,"INFO: "
				"Freeing primary boot record app_index %d\n",
				app_index );
	}


	/* Free pages mapped for this core or primary */
	(void) oct_app_memory_free( cabr->tlb_entries, primary );

	/* Free argument buffer */
	if( primary )
		(void) oct_args_free( cabr );

	cabr->cabr_magic0 = cabr->cabr_magic1 = 0ul;
	cabr->cabr_paddr = 0ull;

	if( paddr == 0ull ) {
		fprintf(stderr, "ERROR: "
			"Failed to retreive boot block physical address\n");
		return -1;
	}


	if( verbose>2 ) fprintf(stderr, "DEBUG: "
		"Freeing boot record paddr=%#llx vaddr=%p\n",
		(unsigned long long) paddr, cabr );

	/* Now free the boot record itself */
	ret = __cvmx_bootmem_phy_free(
		paddr, sizeof( cvmx_app_boot_record_t ), 0);

	/* ret = 1 on success */
	if( ret <= 0 )
		fprintf(stderr, "WARNING: "
			"Error freeing physican bootmem at %#llx\n",
			(unsigned long long) paddr );

	/* Note: master boot record is never freed */

	return ret;
}

/*
 * Obtain a slot in the hotplug named block app array
 * and fill app info into the slot
 * NOTE: this operation needs to be atomic
 */
static int app_index_allocate(  struct app_params * params )
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	unsigned index ;

	/* Obtain exclusive access */
	cvmx_spinlock_lock( &hgp->hotplug_global_lock );

	/* Walk the app array to find an empty slot */
	for( index = 0; index < CVMX_APP_HOTPLUG_MAX_APPS; index ++ ) {
		if( hgp->hotplug_info_array[index].valid_magic )
			continue;
		/* If empty slot found, own it */
		hgp->hotplug_info_array[index].valid_magic =
			CVMX_HOTPLUG_MAGIC_VALID;
		strncpy( hgp->hotplug_info_array[index].app_name,
			params->app_name,
			CVMX_APP_HOTPLUG_MAX_APPNAME_LEN );
		CVMX_SYNCW;
		break;
	} /* for index */

	/* Release exclusivce access */
	cvmx_spinlock_unlock( &hgp->hotplug_global_lock );

	if( index < CVMX_APP_HOTPLUG_MAX_APPS ) {
		params->app_index = index ;
		return 0;
	}
	return -ENOENT;
}

/*
 * Deallocate an app index
 */
int oct_app_index_free( struct app_params * params )
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	unsigned index ;

	index = params->app_index;

	/* Obtain exclusive access */
	cvmx_spinlock_lock( &hgp->hotplug_global_lock );

	/* Clear the entire structure, inclusive of <valid_magic> field */
	if( index < CVMX_APP_HOTPLUG_MAX_APPS ) {
		memset( &hgp->hotplug_info_array[index], 0,
			sizeof( hgp->hotplug_info_array[index] ) );
	}

	CVMX_SYNCW;
	/* Release exclusivce access */
	cvmx_spinlock_unlock( &hgp->hotplug_global_lock );

	return 0;
}

/*
 * Check and print available coremask
 *
 */
static int oct_app_cores_available(void)
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	char hexstr[CVMX_MIPS_MAX_CORES/4+1] ;

	assert( hgp != NULL );

	if (hgp->magic_version != CVMX_HOTPLUG_MAGIC_VERSION ) {
		fprintf(stderr, "ERROR: "
			"Invalid hotplug structure found, "
			"may be old version\n");
		return -1;
	}

        cvmx_coremask_bmp2str( &hgp->avail_coremask, hexstr );

	if(verbose>2) fprintf(stderr, "DEBUG: "
		"Available coremask: 0x%s\n", hexstr );

	if( cvmx_coremask_is_empty( &hgp->avail_coremask ))
		return -1;
	else
		return 0;
}

/*
 * Allocate request number of processor cores
 * for a new app or to add to a running app
 *
 * TBD:
 * poke in /sys/devices/system/cpu/cpuXX/online to release
 * some cores from Linux to free needed number of cores.
 */
static int oct_app_cores_allocate( struct app_params * params,
	unsigned ncores,
	cvmx_coremask_t * add_coremask
	 )
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	cvmx_coremask_t coremask ;
	unsigned corecount;
	int i;


	if( add_coremask != NULL )
		coremask = * add_coremask ;
	else
		memset( &coremask, 0, sizeof(coremask) );

	corecount = 0 ;

	/* Obtain exclusive access */
	cvmx_spinlock_lock( &hgp->hotplug_global_lock );

	/* First try to apply specific coremask requested */
	if( add_coremask != NULL ) {
		/* Count how many cores the mask requests */
		for(i = ncores = 0; i < 64; i ++ )
			if( cvmx_coremask_is_core_set( &coremask, i))
				ncores ++ ;

		/* <AND> requested and available coremasks */
		cvmx_coremask_and( &coremask, add_coremask,
			&hgp->avail_coremask );

		/* If <AND> result is the same, we can get the exact mask */
		if( cvmx_coremask_cmp( add_coremask, &coremask ) == 0 ) {
			cvmx_coremask_xor( &hgp->avail_coremask, &coremask,
				&hgp->avail_coremask );
			corecount = ncores ;
		}
	}

	/* if no specific mask requested, or strict mask allocation failed */
	if( corecount == 0 ) {
		/* Try to claim the number of cores requested */
		cvmx_coremask_for_each_core( i, &hgp->avail_coremask) {
			if( !cvmx_coremask_is_core_set(
				&hgp->avail_coremask, i ))
				continue;
			cvmx_coremask_clear_core( &hgp->avail_coremask, i );
			cvmx_coremask_set_core( &coremask, i );
			corecount ++ ;
			if( ncores == corecount )
				break ;
		} /* cvmx_coremask_for_each_core */
	}

	/* Sync shared data structure */
	CVMX_SYNCW;
	/* Release exclusivce access */
	cvmx_spinlock_unlock( &hgp->hotplug_global_lock );

	if( corecount == 0 ) {
		fprintf(stderr, "ERROR: "
			"%u CPU cores requested, "
			"%u CPU cores available, "
			"aborting.\n",
			ncores, corecount );
			return -EBUSY;
	} else if (ncores > corecount && corecount > 0 ) {
		fprintf(stderr, "WARNING: "
			"%u CPU cores requested, "
			"%u CPU cores available, "
			"proceeding with fewer cores.",
			ncores, corecount );
	} else if (add_coremask != NULL &&
		   cvmx_coremask_cmp( add_coremask, &coremask ) != 0 ) {
		char hexstr[ CVMX_MIPS_MAX_CORES/4+1] ;
		cvmx_coremask_bmp2str( &coremask, hexstr );
		fprintf(stderr, "WARNING: "
			"requested coremask invalid, actual coremask=0x%s\n",
			hexstr);
	}

	/* Save obtained coremask in params */
	params->coremask = coremask ;

	return corecount ;
}

/*
 * Deallocate processor cores in case of an error starting an app
 */
int oct_app_cores_free( struct app_params * params )
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	int i;

	/* Obtain exclusive access */
	cvmx_spinlock_lock( &hgp->hotplug_global_lock );

	/* Release the cores that were claimed earlier */
	cvmx_coremask_for_each_core( i, &params->coremask) {
		cvmx_coremask_set_core( &hgp->avail_coremask, i );

	} /* cvmx_coremask_for_each_core */

	CVMX_SYNCW;

	/* Release exclusive access */
	cvmx_spinlock_unlock( &hgp->hotplug_global_lock );

	return 0;
}

/*
 * Find the location of the system-wide boot vector table
 * for a given node.
 */
static boot_init_vector_t *oct_app_get_boot_vector(unsigned node, unsigned core)
{
        boot_init_vector_t *boot_vect;
        cvmx_mio_boot_loc_cfgx_t boot_loc_cfg;
        uint64_t addr = 0, io_addr, value;

        boot_loc_cfg.u64 = cvmx_read_csr_node(node, CVMX_MIO_BOOT_LOC_CFGX(0));
        if (!boot_loc_cfg.s.en) {
		fprintf(stderr, "ERROR: MIO_BOOT_LOC_CFG not enabled\n" );
		goto _old_bloader;
	}

	addr = boot_loc_cfg.s.base << 7;
	io_addr = MAKE_XKPHYS(addr | (1ULL << 48)
                              | (uint64_t)node << CVMX_NODE_IO_SHIFT);

	if (verbose>1) fprintf(stderr, "DEBUG: "
		"Moveable address = %#llx\n", (unsigned long long) io_addr);

        value = cvmx_read64_uint64(io_addr + OCTEON_BOOT_MOVEABLE_MAGIC_OFFSET);

	if (verbose>1) fprintf(stderr, "DEBUG: "
		"Moveable magic = %#llx\n", (unsigned long long) value);

	/* The previous newest vector from u-boot */
        if (value != OCTEON_BOOT_MOVEABLE_MAGIC &&
            value != OCTEON_BOOT_MOVEABLE_MAGIC1) {
		fprintf(stderr, "ERROR: "
			"MOVEABLE_MAGIC invalid %#llx\n",
			(unsigned long long)value);
		goto _old_bloader;
        }
        value = cvmx_read64_uint64(io_addr +OCTEON_BOOT_VECTOR_MOVEABLE_OFFSET);
        if (value == 0) {
		fprintf(stderr, "ERROR: MOVEABLE_OFFSET is zero!\n");
		goto _old_bloader;
        }

        /* Make physical address */
        addr = value & ~(1ULL << 63);

	/* Convert to virtual address */
	boot_vect = cvmx_phys_to_ptr(addr);

	if (verbose>1)
		fprintf(stderr, "DEBUG: "
			"new boot_vect location paddr %#llx, vaddr %p\n",
			(unsigned long long)addr, boot_vect);
	return boot_vect;

_old_bloader:
	boot_vect = (boot_init_vector_t *)
				(dram_map_addr + BOOTLOADER_BOOT_VECTOR);
	fprintf(stderr,"WARNING: old bootloader assumed\n");
	if (verbose) fprintf(stderr, "INFO: "
			"boot_vect paddr %#llx, vaddr %p\n",
			(unsigned long long)BOOTLOADER_BOOT_VECTOR, boot_vect);
	/* FIXME: should find a way to validate old boot vector too */
	return boot_vect;
}

/*
 * Fill-in the boot vector shared with bootloader
 */
static int oct_app_start_cores(
	struct app_params * params,
	cvmx_coremask_t * pcm)
{
	const linux_app_boot_info_t *labi ;
	boot_init_vector_t *boot_vect;
	cvmx_app_boot_record_t * cabr_ptr ;
	uint64_t boot_info_block_paddr;
	uint64_t cabr_paddr ;
	unsigned int node = 0;	/* boot_vect is on node 0 */
	int core;

	labi = oct_app_get_labi();

	if(verbose>2) fprintf(stderr,"DEBUG: "
		"start_core0_addr %#x\n", labi->start_core0_addr);


	cvmx_coremask_for_each_core( core, pcm ) {

		boot_vect = oct_app_get_boot_vector(node, core);

		/* Get per-core structure address virtual address */
		cabr_ptr = app_boot_record_core_get( params, core ) ;

		/* per-core boot record should have been allocated earlier */
		if (cabr_ptr == NULL)
			continue;

		/* boot_vect[core].k0_val is already setup. Verify. */
		if(verbose>2)
			fprintf(stderr, "DEBUG: Old values: "
				"boot_vect[core].k0_val %#x; "
				"boot_vect[core].code_addr %#llx; "
				"boot_vect[core].start_func_addr %#x\n",
				boot_vect[core].k0_val,
				(unsigned long long)
				boot_vect[core].code_addr,
				boot_vect[core].app_start_func_addr
				);

		/* Fill-in entry-points */
		boot_vect[core].code_addr =
				labi->InitTLBStart_addr;
		boot_vect[core].app_start_func_addr =
				labi->start_app_addr;

		if(verbose>2)
			fprintf(stderr, "DEBUG: New values: "
				"boot_vect[core].code_addr %#llx; "
				"boot_vect[core].start_func_addr %#x\n",
				(unsigned long long)
				boot_vect[core].code_addr,
				boot_vect[core].app_start_func_addr
				);

		/* Get per-core boot structure physical address */
		cabr_paddr = global_master_ptr->cabr_paddr[core]  ;

		/* Calculate per-core boot_info_block physical address */
		boot_info_block_paddr = cabr_paddr + 
			offsetof( cvmx_app_boot_record_t,
				cabr_boot_info_block );

		if(verbose>2) fprintf(stderr, "DEBUG: "
			" core %u: cabr vaddr=%p paddr=%#llx\n",
			core, cabr_ptr, (unsigned long long) cabr_paddr );

		/* Write boot_info_block address into boot vector */
		boot_vect[core].boot_info_addr =
			MAKE_KSEG0(boot_info_block_paddr) ;

		if(verbose>2) fprintf(stderr, "DEBUG: "
			"core: %u boot_info_block paddr=%#llx\n",
			core,
			(unsigned long long) boot_info_block_paddr
			);

	} /* cvmx_coremask_for_each_core */

	/* Make sure the boot vector is flushed before causing NMIs */
	OCTEON_SYNC;

	/* Drain all output before giving the SE app a chance to run */
	fflush(stderr);
	tcdrain(fileno(stderr));

	for (node = 0; node < CVMX_MAX_NODES; node++) {
		uint64_t node_core_mask;
		node_core_mask = cvmx_coremask_get64_node(pcm, node);
		if (node_core_mask == 0) 
			continue;
		if (verbose > 2)  {
			fprintf(stderr, "DEBUG: Writing %#llx to CVMX_CIU_NMI on node %u\n",
				(unsigned long long) node_core_mask, node );
			fflush(stderr);
			tcdrain(fileno(stderr));
		}
		if (octeon_has_feature(OCTEON_FEATURE_CIU3))
			cvmx_write_csr_node(node, CVMX_CIU3_NMI, node_core_mask);
		else
			cvmx_write_csr_node(node, CVMX_CIU_NMI, node_core_mask);
		usleep(10000);
	}  /* for node */
	return 0;
}

/*
 * Booting of a new app
 */
int oct_app_boot( struct app_params * params )
{
	cvmx_app_boot_record_t * cabr_ptr ;
	cvmx_app_boot_segment_t	_segments[ CVMX_APP_MAX_SEGMENTS ];
	char hexstr[ CVMX_MIPS_MAX_CORES/4+1  ] ;
	cvmx_app_hotplug_info_t * hip ;
	const linux_app_boot_info_t *labi ;
	short allocated_cores ;
	int core;
	int fd;
	int ret;

	if( oct_app_cores_available() < 0 ) {
		fprintf(stderr, "ERROR: "
			"No cores are available\n");
		return -1;
	}

	/* Obtain a hotplug app slot */
	if( app_index_allocate( params ) < 0 ) {
		fprintf(stderr, "ERROR: "
			"Maximum number of applications already running\n");
		return -1;
	}

	/* Lock hotplugging mechanism */
	set_app_under_boot(params->app_index+1);

	labi = oct_app_get_labi();

	/* This flag is needed early */
	params->no_mark_private_data = labi->no_mark_private_data;

	hip = &hotplug_global_ptr->hotplug_info_array[ params->app_index ];

	/* Open file */
	if ((fd = open(params->app_name, O_RDONLY)) < 0) {
		perror(params->app_name);
		oct_app_index_free( params );
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	    }

	/* Obtain the needed CPU cores */
	allocated_cores = 0;

	if( !cvmx_coremask_is_empty( &params->coremask ))
		allocated_cores = oct_app_cores_allocate( params,
				0, &params->coremask  );

	if(  allocated_cores <= 0 )
		allocated_cores = oct_app_cores_allocate( params,
				params->numcores, NULL  ) ;

	if( allocated_cores <= 0 ) {
	    /* Cleanup */
	    oct_app_index_free( params );
	    /* Unlock hotplugging mechanism */
	    set_app_under_boot(0);
            return -1;
        }

	/* Get/Create primary boot record */
	cabr_ptr =  app_boot_record_core_get( params, -1 );
	if( cabr_ptr == NULL ){
		oct_app_index_free( params );
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}

	cvmx_app_boot_record_init( params, cabr_ptr );

	oct_tlb_flags_init( cabr_ptr );

	/* Parse ELF structure into first core boot record */
	if( elf_file_prep( fd, cabr_ptr, _segments, CVMX_APP_MAX_SEGMENTS ) < 0 ) {
		oct_app_index_free( params );
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}

	/* Add arguments segment */
	if( oct_args_prep( cabr_ptr, params->argc, params->argv ) < 0 ) {
		fprintf(stderr, "ERROR: failed to prep arguments\n");
		oct_app_index_free( params );
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}

	/* Save initial boot flags for all subsequent cores */
	cabr_ptr->boot_info_flags = params->boot_info_flags ;
	cabr_ptr->boot_desc_flags = params->boot_desc_flags ;
 
	/* Create primary memory map */
	ret = oct_tlb_primary_init( fd, 
		cabr_ptr->tlb_entries,
		_segments,
		 CVMX_APP_MAX_SEGMENTS  );
	if( ret < 0 ) {
		goto _app_boot_fail ;
	}

	/* Create heap/stack templace segment in the primary boot record */
	cabr_ptr->cabr_heap_base =
		    oct_tlb_primary_heap_stack_locate(
			cabr_ptr->tlb_entries,
			cabr_ptr->cabr_heap_size, cabr_ptr->cabr_stack_size );

	cabr_ptr->cabr_init_core =
		cvmx_coremask_get_first_core( &params->coremask );

	/* Prepare memory for the additional cores */
	cvmx_coremask_for_each_core( core, &params->coremask ) {
		cvmx_app_boot_record_t * cabr_core_ptr ;

		if(verbose) fprintf(stderr, "INFO: "
			"Making meta-data for core %d\n", core );

		cabr_core_ptr = app_boot_record_core_get(params, core);
		if( cabr_core_ptr == NULL ) {
			/* XXX free the failed core XXX */
			ret = -2;
			break;
		}

		/* Copy essential fields from primary CABR to per-core */
		oct_app_boot_record_clone( cabr_ptr, cabr_core_ptr, core );

		/* Clone memory map from primary */
		ret = oct_app_memory_clone( 
			cabr_ptr->tlb_entries,
			cabr_core_ptr->tlb_entries );

		if( ret < 0 )
			break;

		/* Adjust boot descriptor in cloned boot record */
		ret = oct_boot_desc_setup( params, cabr_core_ptr );
		if( ret < 0 )
			break;

	}

	if( ret < 0 ) {
		fprintf(stderr, "ERROR: "
			"failed to setup boot descriptors\n");
		goto _app_boot_fail ;
	}

	/* Store obtained coremask in hotplug app slot */
	hip->coremask = params->coremask ;

        /* Convert coremask to string for printing */
        cvmx_coremask_bmp2str( &params->coremask, hexstr );

	if(verbose) fprintf(stderr, "INFO: "
		"Launching application %s on coremask 0x%s\n",
		params->app_name, hexstr );

	if( oct_app_start_cores( params, &params->coremask ) < 0 ) {
		fprintf(stderr, "ERROR: "
			"Error starting application\n");
		goto _app_boot_fail ;
        }

	/* Unlock hotplugging mechanism */
	set_app_under_boot(0);

	/* Wait for the SE app to print its wares */
	usleep(250000);
	cvmx_coremask_bmp2str( &hip->coremask, hexstr );
	fprintf(stderr, "INFO: SUCCESS, App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr );
        cvmx_coremask_bmp2str( &hotplug_global_ptr->avail_coremask, hexstr );
	fprintf(stderr, "INFO: Available cores 0x%s\n", hexstr );

	return 0;

    _app_boot_fail:
	/* XXX + free allocated program memories XXX */

	/* Xfree args buffer */
	oct_args_free( cabr_ptr );
	oct_app_cores_free( params );
	oct_app_index_free( params );
	/* Unlock hotplugging mechanism */
	set_app_under_boot(0);
	return -1 ;
}

/*
 * Derive app_index from app_name if index is not supplied
 */
static int oct_app_index_verify( struct app_params * params )
{
	cvmx_app_hotplug_info_t * hip ;
	unsigned i ;

	    
	/* Find app_index if unspecified */
	if( params->app_index < 0 ) {
		if( params->app_name == NULL ) {
			fprintf(stderr, "ERROR: "
				"Either app index or name is required\n");
			return -1;
		}

		for( i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i ++ ) {
			hip = &hotplug_global_ptr->hotplug_info_array[i];
			if( !hip->valid_magic )
				continue;
			if(hip->valid_magic != CVMX_HOTPLUG_MAGIC_VALID) {
				fprintf(stderr, "ERROR: "
					"App '%s' info not valid\n",
					params->app_name );
				return -1;
			}
			if( !strcmp(params->app_name, hip->app_name) )
				break;
		}
		if(i >= CVMX_APP_HOTPLUG_MAX_APPS ) {
			fprintf(stderr, "ERROR: "
				"App '%s' not found\n", params->app_name );
			return -1;
		}
		params->app_index = i;
		fprintf(stderr, "INFO: "
			"App '%s' found at index %d\n",
			params->app_name, i);
		return params->app_index ;
	} /* uf app_index == -1 */

	if( params->app_index >= CVMX_APP_HOTPLUG_MAX_APPS ) {
		fprintf(stderr, "ERROR: "
			"Application index %d out of range\n",
			params->app_index );
		return -1;
	}
	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];
	if( hip->valid_magic != CVMX_HOTPLUG_MAGIC_VALID ) {
		fprintf(stderr, "ERROR: "
			"Application index %d is not valid\n",
			params->app_index );
		return -1;

	}
	return params->app_index;
}

/*
 * Add cores to a running app
 */
static int oct_add_cores( struct app_params * params )
{
	cvmx_app_hotplug_info_t * hip ;
	cvmx_coremask_t add_coremask ;
	cvmx_app_boot_record_t * cabr_ptr ;
	char hexstr[ CVMX_MIPS_MAX_CORES/4+1  ] ;
        int hot_pluggable, cores_allocated;
	int core;
	int ret, i;

	add_coremask = params->coremask;
	cores_allocated = 0 ;

	/* Find app_index if unspecified */
	ret = oct_app_index_verify( params );
	if( ret < 0 )
		return ret;

	if( is_app_under_boot_or_shutdown() ) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n" );
		return -1;
	}

	if( oct_app_cores_available() < 0 ) {
		fprintf(stderr, "ERROR: "
			"There are no available cores to be plugged\n");
		return -1;
	}


	/* Get primary boot record pointer */
	cabr_ptr = app_boot_record_core_get(params, -1);
	if( cabr_ptr == NULL ){
		return -1;
	}

        hot_pluggable = is_app_hotpluggable(params->app_index);

        if (hot_pluggable == -1) {
            fprintf(stderr, "ERROR: "
			"Failed to get info on application at index=%d"
                       ".\n", params->app_index);
               return -1;
        } else if (hot_pluggable == 0) {
            fprintf(stderr, "ERROR: "
			"Application at index=%d is not registered for"
			" hotplug.\n", params->app_index);
            return -1;
        }

	/* Lock the hotplugging mechanism */
	set_app_under_boot( params->app_index+1 );

	/* Obtain pointer to app slot info block */
	hip = &hotplug_global_ptr->hotplug_info_array[ params->app_index ];

	/* Try to obtain CPU cores according to specific mask */
	if( !cvmx_coremask_is_empty( &add_coremask )) {
		cores_allocated =
			oct_app_cores_allocate( params, 0, &add_coremask );
	}

	/* Obtain the needed CPU cores by count */
	if( cores_allocated == 0 ) {
		cores_allocated = 
			oct_app_cores_allocate( params,
				params->numcores, NULL );
	}

	if( cores_allocated <= 0 ) {
		fprintf(stderr, "ERROR: "
			"Failed to add %u cores to app at index %u\n",
			params->numcores, params->app_index );
            return -1;
        }

	params->numcores = cores_allocated;
	cvmx_coremask_copy( &add_coremask, &params->coremask );
	cvmx_coremask_or( &params->coremask, &add_coremask, &hip->coremask );
	
	/* Spit out all relevant coremasks for debugging */
        cvmx_coremask_bmp2str( &add_coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Adding %d cores to app %u coremask=0x%s\n",
		cores_allocated, params->app_index, hexstr );

        cvmx_coremask_bmp2str( &hip->hotplug_activated_coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Hotplug activated cores 0x%s\n", hexstr );

	/* Clear down-going core masks to avoid confusion */
        cvmx_coremask_clear_all( &hip->shutdown_cores );
        cvmx_coremask_clear_all( &hip->unplug_cores );

	/* hplugged_cores contains the new cores only */
	cvmx_coremask_copy( &hip->hplugged_cores, &add_coremask );

        cvmx_coremask_bmp2str( &hip->hplugged_cores, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Hotplug cores 0x%s\n", hexstr );

	oct_tlb_flags_init( cabr_ptr );

	cabr_ptr->boot_desc_flags |= OCTEON_BL_FLAG_HPLUG_CORES;

	cvmx_coremask_for_each_core( core, &add_coremask ) {
		cvmx_app_boot_record_t * cabr_core_ptr ;

		if(verbose) fprintf(stderr, "INFO: "
			"Preparing meta-data for core %d\n", core );

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if( cabr_core_ptr == NULL ) {
			/* XXX free the failed core XXX */
			ret = -2;
			break;
		}

		/* Copy essential fields from primary CABR to per-core */
		oct_app_boot_record_clone( cabr_ptr, cabr_core_ptr, core );

		cabr_core_ptr->boot_desc_flags |= OCTEON_BL_FLAG_HPLUG_CORES;

		/* Clone memory map from primary */
		ret = oct_app_memory_clone( 
			cabr_ptr->tlb_entries,
			cabr_core_ptr->tlb_entries );

		if( ret < 0 )
			break;

		/* Adjust boot descriptor in cloned boot record */
		ret = oct_boot_desc_setup( params, cabr_core_ptr );
		if( ret < 0 )
			break;
	}

	if( ret < 0 ) {
		fprintf(stderr, "ERROR: "
			"failed to setup boot descriptors\n");
		goto _app_add_fail ;
	}

	if( oct_app_start_cores( params, &add_coremask ) < 0 ) {
		fprintf(stderr, "ERROR: "
			"Error starting application\n");
		goto _app_add_fail ;
        }
 
	/* 
	 * wait for new cores to show up in activated_coremask before
	 * firing the callback
	 */
	i = 5 * 1000000;
	do {
		cvmx_coremask_t tmp_mask;
		unsigned t = 10000;

		CVMX_SYNC;
		cvmx_coremask_and( &tmp_mask, &add_coremask, 
			&hip->hotplug_activated_coremask);
		if( 0 == cvmx_coremask_cmp( &add_coremask, &tmp_mask ))
			break;

		if(verbose) {
			fprintf(stderr, "INFO: "
				"Waiting for cores to register\n");
			t = 250000;
		}

		usleep(t);
		i -= t;
	} while(i > 0 );

	if( i <= 0 ) {
		fprintf(stderr, "WARNING: "
			"Timeout while waiting for new cores to start\n");
		usleep(250000);
	}


        cvmx_coremask_bmp2str( &hip->coremask, hexstr );

	if(verbose) {
		fprintf(stderr, "INFO: "
			"Sending MBOX to cores 0x%s\n", hexstr );
	}

	fflush(stderr);
	tcdrain(fileno(stderr));
 
	/* Send a Mailbox interrupt to invoke callback */
        cvmx_app_hotplug_call_add_cores_callback(params->app_index);

	/* Wait a little for the callbacks to run - no flag to wait on */
	usleep(250000);

	/* Append these new cores to the application coremask */
	cvmx_coremask_or( &hip->coremask, &hip->coremask,
			&add_coremask );

        cvmx_coremask_bmp2str( &hip->coremask, hexstr );

	fprintf(stderr, "INFO: SUCCESS, App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr );

	/* Clear the cores-added mask */
	cvmx_coremask_clear_all( &hip->hplugged_cores );

	/* Unlock the hotplugging mechanism */
	set_app_under_boot( 0 );

	return 0;

    _app_add_fail:

	/* Reverse the effect of adding new cores to hplugged_cores */
	cvmx_coremask_clear_all( &hip->hplugged_cores );

	/* Make the cores allocated available again */
	oct_app_cores_free( params );

	/* Unlock the hotplugging mechanism */
	set_app_under_boot( 0 );

	return -1;
}

/*
 * Delete cores from a running application
 * all but the first initial core can be put to sleep
 * by this operation, reducing the number of cores to 1.
 */
static int oct_del_cores( struct app_params * params )
{
	cvmx_app_hotplug_info_t * hip ;
	cvmx_coremask_t del_coremask;
	cvmx_app_boot_record_t * cabr_ptr ;
	char hexstr[ CVMX_MIPS_MAX_CORES/4+1  ] ;
	int core, i, ret;
	int hot_pluggable;
	unsigned n, numcores;
	unsigned cores_deleted;

	del_coremask = params->coremask;
	numcores = params->numcores;
	cores_deleted = 0 ;

	ret = oct_app_index_verify( params );
	if( ret < 0 )
		return ret ;

	/* Get primary boot record pointer for this app */
	cabr_ptr = app_boot_record_core_get(params, -1);
	if( cabr_ptr == NULL ){
		return -1;
	}

        hot_pluggable = is_app_hotpluggable(params->app_index);

        if (hot_pluggable == -1) {
            fprintf(stderr, "ERROR: "
			"Failed to get info on application at index=%d"
                       ".\n", params->app_index);
               return -1;
        } else if (hot_pluggable == 0) {
            fprintf(stderr, "ERROR: "
			"Application at index=%d is not registered for"
			" hotplug.\n", params->app_index);
            return -1;
        }

	if( is_app_under_boot_or_shutdown() ) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n" );
		return -1;
	}

	/* Lock the hotplugging mechanism */
	set_app_under_shutdown( params->app_index+1 );

	/* Obtain pointer to app slot info block */
	hip = &hotplug_global_ptr->hotplug_info_array[ params->app_index ];

	/* Filter explicit coremask with the actual cores running the app */
	if( !cvmx_coremask_is_empty( &del_coremask )) {
		numcores = cvmx_coremask_get_core_count(&del_coremask);
		cvmx_coremask_and( &del_coremask, &del_coremask,
			&hip->coremask );
		/* Make sure no one is trying to unplug the init_core */
		cvmx_coremask_clear_core( &del_coremask,
			cabr_ptr->cabr_init_core );
		cores_deleted = cvmx_coremask_get_core_count( &del_coremask );
	}

	/* Find cores to delete by requested count */
	if( cores_deleted < numcores ) {
		if( numcores < cores_deleted )
			numcores = cores_deleted;

		/* Limit numcores to running cores - 1 */
		n = cvmx_coremask_get_core_count( &hip->coremask ) - 1;
		if( numcores > n )
			numcores = n;

		while( cores_deleted < numcores ) {
			cvmx_coremask_t mask;

			cvmx_coremask_copy( &mask, &hip->coremask );
			cvmx_coremask_maskoff(&mask, &mask, &del_coremask );

			/* Library does not have last core number from mask */
			i = -1;
			cvmx_coremask_for_each_core( core, &mask ) {
				if( core == cabr_ptr->cabr_init_core )
					continue;
				if( cvmx_coremask_is_core_set( &mask, core ) )
					i = core;
			}

			if( i == -1 )
				break;

			cvmx_coremask_set_core(&del_coremask, i );
			cores_deleted ++;
		}
	}

	/* Make double sure no one is trying to unplug the init_core */
	cvmx_coremask_clear_core( &del_coremask, cabr_ptr->cabr_init_core );

	cores_deleted = cvmx_coremask_get_core_count( &del_coremask );

	if( cores_deleted == 0 ) {
		fprintf(stderr, "ERROR: "
			"Can not unplug application primary core\n");
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown( 0 );
		return 0;
	} else if (!cvmx_coremask_is_empty( &params->coremask ) &&
		cvmx_coremask_cmp( &del_coremask, &params->coremask) != 0) {
		cvmx_coremask_bmp2str( &del_coremask, hexstr );
		fprintf(stderr, "WARNING: "
			"requested coremask invalid, actual coremask=0x%s\n",
			hexstr);
	} else if( params->numcores > 0 && params->numcores != cores_deleted ){
		fprintf(stderr, "WARNING: "
			"requested core count is reduced to %u\n",
			cores_deleted );
	}

	/* Spit out all relevant coremasks for debugging */
        cvmx_coremask_bmp2str( &hip->hotplug_activated_coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Hotplug activated cores 0x%s\n", hexstr );

        cvmx_coremask_bmp2str( &del_coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Unplugging %d cores from app %u coremask=0x%s\n",
		cores_deleted, params->app_index, hexstr );

        cvmx_coremask_copy( &hip->unplug_cores, &del_coremask );
        cvmx_coremask_clear_all( &hip->shutdown_cores );
        cvmx_coremask_clear_all( &hip->hplugged_cores );

	/* Store actual coremask in params */
	cvmx_coremask_copy( &params->coremask, &del_coremask );

	/* Drain all output before poking the application */
	fflush(stderr);
	tcdrain(fileno(stderr));

	/* The actual operation is located in the executive library */
        ret = cvmx_app_hotplug_unplug_cores(
		params->app_index, &del_coremask, 0);

	if( ret != 0 ) {
		fprintf(stderr, "ERROR: "
			"Failed to unplug cores in mask=0x%s\n", hexstr );
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown( 0 );
		return -1;
	}

	/* The above function does not implement a waiting loop, we do */
	i = 2 * 1000000;
	do {
		cvmx_coremask_t mask;
		unsigned t = 1000;

		CVMX_SYNC;

		/* Wait for all unplugged cores to deactivate */
		cvmx_coremask_and( &mask, &del_coremask,
			&hip->hotplug_activated_coremask );
		if( cvmx_coremask_is_empty( &mask ) )
			break;

		if(verbose) {
			fprintf(stderr, "INFO: "
				"Waiting for application to unplug\n");
			t = 250000;
		}
		usleep(t);
		i -= t;
	} while( i > 0 );

	if( i <= 0 ) {
		fprintf(stderr, "ERROR: "
			"Timeout out waiting for cores to confirm request\n");
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown( 0 );
		return -1;
	}

	/* Wait for SE app outputs to drain */
	usleep(250000);

        cvmx_coremask_bmp2str( &hip->hotplug_activated_coremask, hexstr );
	if(verbose > 2) fprintf(stderr, "DEBUG: "
		"Hotplug activated cores changed to 0x%s\n", hexstr );

	/* Make unplugged cores available again */
	oct_app_cores_free( params );

	/* Free all per-core memories */
	cvmx_coremask_for_each_core( core, &del_coremask ) {
		cvmx_app_boot_record_t * cabr_core_ptr ;

		if(verbose) fprintf(stderr, "INFO: "
			"Cleaning up after core %d\n", core );

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if( cabr_core_ptr == NULL ) {
			fprintf(stderr, "ERROR: "
				"Failed to obtain boot record for core %d\n",
				core );
			continue;
		}

		/* The following call releases all per-core memories */
		ret = app_boot_record_free( cabr_core_ptr );
		if( ret < 0 ) {
			fprintf(stderr, "ERROR: "
				"Failed to clean-up after core %d\n",
				core );
		}
	} /* for each core */

	/* Update running coremask for the app */
	cvmx_coremask_maskoff( &hip->coremask, &hip->coremask, &del_coremask );

	/* Clear the unplugged coremask */
        cvmx_coremask_clear_all( &hip->unplug_cores );

	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown( 0 );

        cvmx_coremask_bmp2str( &hip->coremask, hexstr );
	fprintf(stderr, "SUCCESS: App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr );

	return 0;
}

/*
 * Shutdown a running app
 */
static int oct_app_kill( struct app_params * params )
{
	cvmx_app_hotplug_info_t * hip ;
	cvmx_coremask_t del_coremask;
	cvmx_app_boot_record_t * cabr_ptr ;
	char hexstr[ CVMX_MIPS_MAX_CORES/4+1  ] ;
	int core, ret, i;
	unsigned cores_deleted;
	int hot_pluggable;

	ret = oct_app_index_verify( params );
	if( ret < 0 )
		return ret ;

	/* Get primary boot record pointer for this app */
	cabr_ptr = app_boot_record_core_get(params, -1);
	if( cabr_ptr == NULL ){
		return -1;
	}

        hot_pluggable = is_app_hotpluggable(params->app_index);

        if (hot_pluggable == -1) {
            fprintf(stderr, "ERROR: "
			"Failed to get info on application at index=%d"
                       ".\n", params->app_index);
               return -1;
        } else if (hot_pluggable == 0) {
            fprintf(stderr, "ERROR: "
			"Application at index=%d is not registered for"
			" hotplug.\n", params->app_index);
            return -1;
        }

	if( is_app_under_boot_or_shutdown() ) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n" );
		return -1;
	}

	/* Lock the hotplugging mechanism */
	set_app_under_shutdown( params->app_index+1 );

	/* Obtain pointer to app slot info block */
	hip = &hotplug_global_ptr->hotplug_info_array[ params->app_index ];

	/* Shutdown all cores running this app */
	cvmx_coremask_copy( &del_coremask, &hip->coremask) ;
	cvmx_coremask_copy( &params->coremask, &del_coremask );
	cores_deleted = cvmx_coremask_get_core_count( &del_coremask );

	if( cores_deleted == 0 ) {
		fprintf(stderr, "ERROR: "
			"Can not shutdown requested cores\n");
		goto _oct_kill_fail;
	}

        cvmx_coremask_bmp2str( &hip->hotplug_activated_coremask, hexstr );
	if(verbose>1) fprintf(stderr, "INFO: "
		"Hotplug activated cores 0x%s\n", hexstr );

	/* Spit out all relevant coremasks for debugging */
        cvmx_coremask_bmp2str( &del_coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: "
		"Shuttong down %d cores from app %u coremask=0x%s\n",
		cores_deleted, params->app_index, hexstr );

        cvmx_coremask_copy( &hip->shutdown_cores, &del_coremask );
        cvmx_coremask_clear_all( &hip->unplug_cores );
        cvmx_coremask_clear_all( &hip->hplugged_cores );

	/* Drain all output before poking the application */
	fflush(stderr);
	tcdrain(fileno(stderr));

	/* The action code is in the executive library */
        ret = cvmx_app_hotplug_shutdown_request( &del_coremask, 0);
	if( ret != 0 ) {
		fprintf(stderr, "ERROR: "
			"Failed to shutdown cores in mask=0x%s\n", hexstr );
		goto _oct_kill_fail;
	}

	/* cvmx_app_hotplug_shutdown_complete() is implemented poorly,
	 * here is a cleaner way.
	 */
	i = 5 * 1000000;
	do {
		unsigned t = 1000;

		if(verbose) {
			fprintf(stderr, "INFO: Waiting for app to shutdown\n");
			t = 250000;
		}

		usleep(t);
		i -= t;

		/* Wait for all unplogged cores to deactivate */
		if( cvmx_coremask_is_empty( &hip->hotplug_activated_coremask ))
			break;
		if( hip->shutdown_done )
			break;
	} while( i > 0 );

	if( i <= 0 ) {
		fprintf(stderr, "ERROR: "
			"Timeout out waiting for cores to shutdown\n");
		goto _oct_kill_fail;
	}

	/* Wait for the SE app output to drain */
	usleep(250000);

        cvmx_coremask_bmp2str( &hip->hotplug_activated_coremask, hexstr );
	if(verbose>2) fprintf(stderr, "DEBUG: "
		"Hotplug activated cores changed to 0x%s, shutdown_done=%d\n",
		hexstr, (int) hip->shutdown_done );

	/* Make unplugged cores available again */
	oct_app_cores_free( params );

	/* Free all per-core memories */
	cvmx_coremask_for_each_core( core, &del_coremask ) {
		cvmx_app_boot_record_t * cabr_core_ptr ;

		if(verbose) fprintf(stderr, "INFO: "
			"Cleaning up after core %d\n", core );

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if( cabr_core_ptr == NULL ) {
			fprintf(stderr, "ERROR: "
				"Failed to obtain boot record for core %d\n",
				core );
			continue;
		}

		/* The following call releases all per-core memories */
		ret = app_boot_record_free( cabr_core_ptr );
		if( ret < 0 ) {
			fprintf(stderr, "ERROR: "
				"Failed to clean-up after core %d\n",
				core );
		}
	} /* for each core */

	/* Free the primary boot record */
	if(verbose) fprintf(stderr, "INFO: "
		"Cleaning up primary data\n");

	ret = app_boot_record_free( cabr_ptr );
	if( ret < 0 ) {
		fprintf(stderr, "ERROR: "
			"Failed to clean-up primary boot record\n");
		}

	/* Update running coremask for the app */
	cvmx_coremask_maskoff( &hip->coremask, &hip->coremask, &del_coremask );

	/* Clear the shutdown  coremask */
        cvmx_coremask_clear_all( &hip->shutdown_cores );
	hip->shutdown_done = 0;

	/* zero out the entire application slot */
	memset( hip, 0, sizeof(*hip));

	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown( 0 );

	fprintf(stderr, "SUCCESS: App index %u now down\n",
		params->app_index );

	return 0;

  _oct_kill_fail:
	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown( 0 );
	return -1;
}

/* We are the real main, no cvmx-app-init-linux here.*/
#undef main
int main(int argc, char *argv[])
{
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	struct app_params prm;
	cvmx_sysinfo_t *system_info;
	cpu_set_t cpumask;
	int i;

	/* First we must do a little bit of CVMX initialization so we
	 * can use the library functions.
	 */
	
	/* Get the list of logical cpus we could run on */
	if (sched_getaffinity(0, sizeof(cpumask), &cpumask)) {
		perror("sched_getaffinity failed");
		exit(errno);
	}
	/* Find the first CPU we can use */
	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, &cpumask))
			break;
	}
	/* Skip over it */
	i++;
	/* Clear all others so system_info->init_core is always
	 * consistent.
	 */
	for (; i < CPU_SETSIZE; i++)
		CPU_CLR(i, &cpumask);

	if (sched_setaffinity(0, sizeof(cpumask),  &cpumask)) {
		perror("sched_setaffinity failed");
		exit(errno);
	}

	cvmx_linux_enable_xkphys_access(0);
	cvmx_sysinfo_linux_userspace_initialize();
	system_info = cvmx_sysinfo_get();
	cvmx_bootmem_init(system_info->phy_mem_desc_addr);
	system_info->init_core = cvmx_get_core_num();

	/* Initialize mailbox facility */
	cvmx_mbox_initialize(0);

	memset( &prm, 0, sizeof(prm));

	/* Default action */
	prm.command = APP_CMD_UNDEF;

	/* Fill-in default application parameters */
	prm.numcores = 1;
	prm.heap_size = 3 << 20;
	prm.stack_size = 1 << 20;
	prm.app_index = -1;

	progname = argv[0];

	/* See if command action is in program name */
	if( strstr( argv[0], "ctl" )) {
		prm.command = APP_CMD_UNDEF;
		argv++; argc--;
	}

	if( argc < 1)
		return oct_app_main_help();

	/* Decode command action */
	if( strstr( argv[0], "info") || strstr(argv[0], "show" ))
		prm.command = APP_CMD_INFO;
	else if( strstr( argv[0], "boot") || strstr(argv[0], "start" ))
		prm.command = APP_CMD_BOOT;
	else if( strstr( argv[0], "unplug") || strstr(argv[0], "remove" ))
		prm.command = APP_CMD_DEL;
	else if( strstr( argv[0], "del") || strstr(argv[0], "rm" ))
		prm.command = APP_CMD_DEL;
	else if( strstr( argv[0], "add") || strstr(argv[0], "plug" ))
		prm.command = APP_CMD_ADD;
	else if( strstr( argv[0], "halt" ) || strstr(argv[0], "kill") ||
		strstr(argv[0], "shut") )
		prm.command = APP_CMD_KILL;
	else
		return oct_app_main_help();

	/* First arg, action has been processed */
	argv++; argc--;

	/* Scan for generic options */
	if( oct_app_cmd_genopt( argc, argv, &prm ) < 0 )
		return oct_app_main_help();

	/* Parse the remainder of options depending on the action */
	if( prm.command == APP_CMD_BOOT ) {
		if( oct_app_cmd_boot_getopt(argc, argv, &prm ) < 0 )
			return oct_app_boot_help();
	}
	
	if( prm.command == APP_CMD_ADD ) {
		if( oct_app_cmd_add_getopt(argc, argv, &prm ) < 0 )
			return oct_app_add_help();
	}
	
	if( prm.command == APP_CMD_DEL ) {
		if( oct_app_cmd_del_getopt(argc, argv, &prm ) < 0 )
			return oct_app_del_help();
	}
	
	if( prm.command == APP_CMD_KILL ) {
		if( oct_app_cmd_kill_getopt(argc, argv, &prm ) < 0 )
			return oct_app_kill_help();
	}
	
	/* APP_CMD_INFO has no specific options */

	if (app_boot_master_create()) {
		fprintf( stderr, "ERROR: naster boot record init failed\n");
		return -1;
	    }

	/* Setup hotplug named block and pointers */
	if (map_hotplug_info()) {
		fprintf(stderr, "ERROR: Unable to map hotplug info\n");
		return -1;
		}
    
	cvmx_coremask_bmp2str( &prm.coremask, hexstr );
	if(verbose) fprintf(stderr, "INFO: action=%u coremask=0x%s count=%u\n",
		prm.command, hexstr, prm.numcores );

	/*
	 * Perform the action requested
	 */
	if( prm.command == APP_CMD_INFO )
		return oct_app_info_show( &prm );

	if( prm.command == APP_CMD_BOOT )
		return  oct_app_boot( &prm );

	if( prm.command == APP_CMD_ADD )
		return oct_add_cores( &prm );

	if( prm.command == APP_CMD_DEL )
		return oct_del_cores( &prm );

	if( prm.command == APP_CMD_KILL )
		return oct_app_kill( &prm );

	fprintf(stderr,"ERROR: "
		"Failed to understand command\n");

	/* XXX- Note: this program does not do self-cleaning,
	 * but depends on all process resources to be cleaned up by Linux
	 */

	return 0;
}
/* End of File */
