/***********************license start************************************
 * Copyright (c) 2017 Cavium Inc. (support@cavium.com). All rights
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
 * $Id: oct-app-main.c 118877 2015-06-03 18:30:06Z egoranov $
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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <cvmx.h>
#include <cvmx-coremask.h>
#include <cvmx-mbox.h>
#include <cvmx-app-hotplug.h>
#include <cvmx-boot-vector.h>
#include <arpa/inet.h> /* for htonl(), htons() */

#include "octeon-remote.h"
#include "oct-remote-app.h"
#include "cvmx.h"
#include "cvmx-swap.h"
#include "cvmx-mio-defs.h"

#define CVMX_USE_1_TO_1_TLB_MAPPINGS 0	/* Not supported in user-land */


/**** Global Variables ***/
int verbose;
const char *progname;			/* original argv[0] */
void *dram_map_addr;			/* bootloader params at low mem */
/* shadow structs and pointers to them; R/W: read/write from/to target */
cvmx_app_boot_master_t master_g, *global_master_ptr = NULL;
struct cvmx_cores_common_bootinfo ccbi_g;
cvmx_app_hotplug_global_t hp_g, *hotplug_global_ptr = &hp_g;

cvmx_app_boot_record_t cabr_primary_g, cabr_core_g, *cabr_ptr;
linux_app_boot_info_t labi_g;
paddr_t master_pa, hp_pa = 0, ccbi_pa;
size_t hp_len, ccbi_len;
/* Hotplug common data structure */
cvmx_app_hotplug_info_t *hotplug_info_ptr;

uint64_t global_linux_boot_info_phys;

static int oct_app_cores_available(void);

static const char __help_common_opt[] =
			"\t<common-options> := "
			"[-verbose[=N]]"
			"\n";

/* return phys addr (cast to (cvmx_app_hotplug_info_t *)) inside of
 * the hotplug_global named block mapped to
 * the shadow structure hp_g.hotplug_info_array[index]
 */
static cvmx_app_hotplug_info_t *get_hip_pa(int app_index)
{
	assert(hp_pa != 0);
	assert(app_index >= 0);
	assert(sizeof(cvmx_app_hotplug_info_t) == 1024u);

	cvmx_app_hotplug_info_t *hip_pa = (cvmx_app_hotplug_info_t *) (hp_pa
		+ offsetof(struct cvmx_app_hotplug_global, hotplug_info_array)
		+ app_index * sizeof(cvmx_app_hotplug_info_t));
	return hip_pa;
}

/* save hotplug related structures to target in BE
 * (bootloader/Linux require this) and read them with endianess auto-corrected,
 * so all values are meaningful
 */

static void save_master_g_on_target(void)
{
	int i;
	cvmx_app_boot_master_t *master_s = (cvmx_app_boot_master_t *) master_pa;
	assert(master_pa != 0);
#define masterW64(x) \
	octeon_remote_write_mem64((uintptr_t)&master_s->x, global_master_ptr->x)
#define masterW32(x) \
	octeon_remote_write_mem32((uintptr_t)&master_s->x, global_master_ptr->x)

	masterW32(magic);
	/* skip 'version' */
	/* skip 'mum_cores' */
	for (i = 0; i < CVMX_MAX_CORES; i++)
		masterW64(cabr_paddr[i]);
}

static void read_master_g_from_target(void)
{
	int i;
	cvmx_app_boot_master_t *master_s = (cvmx_app_boot_master_t *) master_pa;
	assert(master_pa != 0);
#define masterR64(x) \
(global_master_ptr->x = octeon_remote_read_mem64((uintptr_t)&master_s->x))
#define masterR32(x) \
(global_master_ptr->x = octeon_remote_read_mem32((uintptr_t)&master_s->x))

	masterR32(magic);
	/* skip 'version' */
	/* skip 'mum_cores' */
	for (i = 0; i < CVMX_MAX_CORES; i++)
		masterR64(cabr_paddr[i]);
}

static void save_hp_g_on_target(void)
{
	int i;
	/* write hp_g members to target (auto-convert endianess) */
	assert(hp_pa != 0);
	assert(hotplug_global_ptr != NULL);

	cvmx_app_hotplug_global_t *hpg = (cvmx_app_hotplug_global_t *)hp_pa;
#define hpgW64(x) \
	octeon_remote_write_mem64((uintptr_t)&hpg->x, hotplug_global_ptr->x)
/*
#define hpgW32(x) \
	*octeon_remote_write_mem32((uintptr_t)&hpg->x, hotplug_global_ptr->x)
#define hpgW16(x) \
	*octeon_remote_write_mem16((uintptr_t)&hpg->x, hotplug_global_ptr->x)
#define hpgW8(x)  \
	*octeon_remote_write_mem((uintptr_t)&hpg->x, &hotplug_global_ptr->x, 1)
*/
#define hpgWcoremask(x)	\
	oct_remote_write_coremask((uintptr_t)&hpg->x, &hotplug_global_ptr->x)
#define hipWcoremask(x)	\
	oct_remote_write_coremask((uintptr_t)&hip_pa->x, &hip->x)
#define hipW64(x) octeon_remote_write_mem64((uintptr_t)&hip_pa->x, hip->x)

	for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {
		cvmx_app_hotplug_info_t *hip_pa = get_hip_pa(i);
		cvmx_app_hotplug_info_t *hip;

		hip = &hotplug_global_ptr->hotplug_info_array[i];
		hipWcoremask(coremask);
		hipWcoremask(hotplug_activated_coremask);
		hipWcoremask(hplugged_cores);
		hipWcoremask(shutdown_cores);
		hipWcoremask(unplug_cores);
		hipW64(shutdown_callback);
		hipW64(unplug_callback);
		hipW64(cores_added_callback);
		hipW64(cores_removed_callback);
		hipW64(data);
		hipW64(app_shutdown);
		hipW64(shutdown_done);
		hipW64(cvmx_app_boot_record_ptr);
		octeon_remote_write_mem((uintptr_t)&hip_pa->app_name[0],
					hip->app_name,
					CVMX_APP_HOTPLUG_MAX_APPNAME_LEN);
		/* skip uint64_t unused[7]; */
		hipW64(valid_magic);
	}
	hpgWcoremask(avail_coremask);
	/* skip cvmx_spinlock_t hotplug_global_lock; It will be used directly
	 * ONLY in remote octeon memory, not is shadow structure
	 */
	hpgW64(app_under_boot);
	hpgW64(app_under_shutdown);
	/* skip uint64_t reserved[8]; */
	hpgW64(magic_version);
}

static void read_hp_g_from_target(void)
{
	int i;

	/* read hp_g members from target (auto-convert endianess) */
	assert(hp_pa != 0);
	assert(hotplug_global_ptr != NULL);

	cvmx_app_hotplug_global_t *hpg = (cvmx_app_hotplug_global_t *)hp_pa;
#define hpgR64(x) \
	(hotplug_global_ptr->x = octeon_remote_read_mem64((uintptr_t)&hpg->x))
/*#define hpgR32(x) \
	*hotplug_global_ptr->x = octeon_remote_read_mem32((uintptr_t)&hpg->x)*/
/*#define hpgR16(x) \
	*hotplug_global_ptr->x = octeon_remote_read_mem16((uintptr_t)&hpg->x)*/
/*#define hpgR8(x)  \
	*octeon_remote_read_mem(&hotplug_global_ptr->x, (uintptr_t)&hpg->x, 1)*/
#define hpgRcoremask(x)	\
	oct_remote_read_coremask((uintptr_t)&hpg->x, &hotplug_global_ptr->x)
#define hipRcoremask(x)	oct_remote_read_coremask((uintptr_t)&hip_pa->x, &hip->x)
#define hipR64(x) (hip->x = octeon_remote_read_mem64((uintptr_t)&hip_pa->x))

	for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {
		cvmx_app_hotplug_info_t *hip_pa = get_hip_pa(i);
		cvmx_app_hotplug_info_t *hip;

		hip = &hotplug_global_ptr->hotplug_info_array[i];
		hipR64(valid_magic);
		if (hip->valid_magic != CVMX_HOTPLUG_MAGIC_VALID)
			continue;	/* skip invalid/empty slots */

		hipRcoremask(coremask);
		hipRcoremask(hotplug_activated_coremask);
		hipRcoremask(hplugged_cores);
		hipRcoremask(shutdown_cores);
		hipRcoremask(unplug_cores);
		hipR64(shutdown_callback);
		hipR64(unplug_callback);
		hipR64(cores_added_callback);
		hipR64(cores_removed_callback);
		hipR64(data);
		hipR64(app_shutdown);
		hipR64(shutdown_done);
		hipR64(cvmx_app_boot_record_ptr);
		octeon_remote_read_mem(hip->app_name,
					(uintptr_t)&hip_pa->app_name[0],
					CVMX_APP_HOTPLUG_MAX_APPNAME_LEN);
		/* skip uint64_t unused[7]; */
		hipR64(valid_magic);
	}
	hpgRcoremask(avail_coremask);
	/* skip cvmx_spinlock_t hotplug_global_lock; */
	hpgR64(app_under_boot);
	hpgR64(app_under_shutdown);
	/* skip uint64_t reserved[8]; */
	hpgR64(magic_version);
}

void save_cabr_on_target(cvmx_app_boot_record_t *cabr_ptr)
{
	/* write cabr members to target (auto-convert endianess) */
#define cabrW64(x) octeon_remote_write_mem64((uintptr_t)&cabr->x, cabr_ptr->x)
#define cabrW32(x) octeon_remote_write_mem32((uintptr_t)&cabr->x, cabr_ptr->x)
#define cabrW16(x) octeon_remote_write_mem16((uintptr_t)&cabr->x, cabr_ptr->x)
#define cabrW8(x)  octeon_remote_write_mem((uintptr_t)&cabr->x, &cabr_ptr->x, 1)
	int i;

	assert(cabr_ptr->cabr_paddr != 0);
	cvmx_app_boot_record_t *cabr;

	cabr = (cvmx_app_boot_record_t *)cabr_ptr->cabr_paddr;
	cabrW32(cabr_magic0);
	cabrW16(cabr_hotplug_app_index);
	cabrW16(cabr_core);
	cabrW16(cabr_init_core);
	cabrW64(cabr_paddr);
	octeon_remote_write_mem64(
		(uintptr_t)&cabr->cabr_boot_info_block.tlb_entries,
		(uintptr_t)cabr_ptr->cabr_boot_info_block.tlb_entries);
	cabrW64(cabr_boot_info_block.entry_point);
	cabrW64(cabr_boot_info_block.boot_desc_addr);
	cabrW64(cabr_boot_info_block.stack_top);
	cabrW32(cabr_boot_info_block.exception_base);
	cabrW64(cabr_boot_info_block.cvmx_desc_addr);
	cabrW64(cabr_boot_info_block.flags);
	for (i = 0; i < NUM_OCTEON_TLB_ENTRIES; i++) {
		cabrW64(tlb_entries[i].phy_page0);
		cabrW64(tlb_entries[i].phy_page1);
		cabrW64(tlb_entries[i].virt_base);
		cabrW32(tlb_entries[i].page_size);
		octeon_remote_write_mem64(
			(uintptr_t)&cabr->tlb_entries[i].flags,
			(uint64_t)cabr_ptr->tlb_entries[i].flags.ri << 63
			| (uint64_t)cabr_ptr->tlb_entries[i].flags.xi << 62
			| (uint64_t)cabr_ptr->tlb_entries[i].flags.cca << 59
			| (uint64_t)cabr_ptr->tlb_entries[i].flags.seg_type
			<< 56);
	}
	cabrW32(cabr_magic1);
	cabrW64(cabr_entry_point);
	cabrW64(cabr_heap_base);
	cabrW64(cabr_heap_end);
	cabrW64(cabr_stack_base);
	cabrW32(cabr_heap_size);
	cabrW32(cabr_stack_size);
	cabrW32(argc);
	for (i = 0; i < 64; i++)
		cabrW64(argv[i]);

	for (i = 0; i < 32; i++)
		cabrW8(arg_buf_name[i]);

	cabrW32(boot_info_flags);
	cabrW32(boot_desc_flags);
	cabrW32(boot_cfg_flags);
	cabrW32(no_mark_private_data);
}
void read_cabr_from_target(cvmx_app_boot_record_t *cabr_ptr)
{
	/* read cabr members from target (auto-convert endianess) */
#define cabrR32(x) (cabr_ptr->x = octeon_remote_read_mem32((uintptr_t)&cabr->x))
#define cabrR16(x) (cabr_ptr->x = octeon_remote_read_mem16((uintptr_t)&cabr->x))
#define cabrR64(x) (cabr_ptr->x = octeon_remote_read_mem64((uintptr_t)&cabr->x))
#define cabrR8(x)  octeon_remote_read_mem(&cabr_ptr->x, (uintptr_t)&cabr->x, 1)

	int i;
	uint64_t v1;

	assert(cabr_ptr->cabr_paddr != 0);
	cvmx_app_boot_record_t *cabr;

	cabr = (cvmx_app_boot_record_t *)cabr_ptr->cabr_paddr;
	cabrR32(cabr_magic0);
	cabrR16(cabr_hotplug_app_index);
	cabrR16(cabr_core);
	cabrR16(cabr_init_core);
	cabrR64(cabr_paddr);
	cabr_ptr->cabr_boot_info_block.tlb_entries =
		(octeon_tlb_entry_t_bl *)octeon_remote_read_mem64(
			(uintptr_t)&cabr->cabr_boot_info_block.tlb_entries);
	cabrR64(cabr_boot_info_block.entry_point);
	cabrR64(cabr_boot_info_block.boot_desc_addr);
	cabrR64(cabr_boot_info_block.stack_top);
	cabrR32(cabr_boot_info_block.exception_base);
	cabrR64(cabr_boot_info_block.cvmx_desc_addr);
	cabrR64(cabr_boot_info_block.flags);
	for (i = 0; i < NUM_OCTEON_TLB_ENTRIES; i++) {
		cabrR64(tlb_entries[i].phy_page0);
		cabrR64(tlb_entries[i].phy_page1);
		cabrR64(tlb_entries[i].virt_base);
		cabrR32(tlb_entries[i].page_size);
		v1 = octeon_remote_read_mem64(
			(uintptr_t)&cabr->tlb_entries[i].flags);
		*(uint64_t *)&cabr_ptr->tlb_entries[i].flags =
			((v1 & 0x8000000000000000ull) >> 63) |	/* .ri */
			((v1 & 0x4000000000000000ull) >> 61) |	/* .xi */
			((v1 & 0x3800000000000000ull) >> 57) |	/* .ccbi_addr */
			((v1 & 0x0700000000000000ull) >> 51);	/* .seg_type */
	}
	cabrR32(cabr_magic1);
	cabrR64(cabr_entry_point);
	cabrR64(cabr_heap_base);
	cabrR64(cabr_heap_end);
	cabrR64(cabr_stack_base);
	cabrR32(cabr_heap_size);
	cabrR32(cabr_stack_size);
	cabrR32(argc);
	for (i = 0; i < 64; i++)
		cabrR64(argv[i]);

	for (i = 0; i < 32; i++)
		cabrR8(arg_buf_name[i]);

	cabrR32(boot_info_flags);
	cabrR32(boot_desc_flags);
	cabrR32(boot_cfg_flags);
	cabrR32(no_mark_private_data);
}


/*
 * Several help functions to indicate indended usage
 */
static int oct_app_main_help(void)
{
	fprintf(stderr, "Usage:\n");
	if (strstr(progname, "ctl"))
		fprintf(stderr, "\t%s <action> ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <action-options>\n");
	if (strstr(progname, "ctl"))
		fprintf(stderr, "\t<action> := "
			"boot, add, del, shut\n");
	fprintf(stderr, __help_common_opt);
	return -1;
}

static int oct_app_boot_help(void)
{
	fprintf(stderr, "Usage:\n");
	if (strstr(progname, "ctl"))
		fprintf(stderr, "\t%s boot ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <boot-options> <app-file-name>"
		"<app-options>\n");
	fprintf(stderr, "\t<boot-options> := "
			"[-numcores=<numcores>] "
			"[-mask=<coremask>] "
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
	if (strstr(progname, "ctl"))
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
	fprintf(stderr,
		"\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}

static int oct_app_del_help(void)
{
	fprintf(stderr, "Usage:\n");
	if (strstr(progname, "ctl"))
		fprintf(stderr, "\t%s del ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> <del-options> "
		"[-iindex=<app-index> | <app-file-name>]"
		"\n");
	fprintf(stderr, "\t<del-options> := "
			"[-n=<numcores>] "
			"[-m=<coremask>] "
			"\n");
	fprintf(stderr, __help_common_opt);
	fprintf(stderr,
		"\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}


static int oct_app_kill_help(void)
{
	fprintf(stderr, "Usage:\n");
	if (strstr(progname, "ctl"))
		fprintf(stderr, "\t%s shut ", progname);
	else
		fprintf(stderr, "\t%s ", progname);
	fprintf(stderr, "<common-options> "
			"[-iindex=<app-index> | <app-file-name>] "
		"\n");
	fprintf(stderr, __help_common_opt);
	fprintf(stderr,
		"\tNote: One of: <app-name> or <app-index> is required\n");
	return -1;
}



/*
 * Parse generic command options
 */
int oct_app_cmd_genopt(int argc, char **argv, struct app_params *params)
{
	char *sval = NULL;
	char opt;

	while (argc > 0) {
		if (argv[0][0] != '-') {
			opt = '\0'; sval = "";
			break;
		} else {
			opt = argv[0][1];
			sval = strchr(argv[0], '=');
			if (sval != NULL) {
				sval = sval + 1;
			} else if (argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if (opt == 'v') {
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
int oct_app_cmd_boot_getopt(int argc, char **argv, struct app_params *params)
{
	const char *sval = NULL;
	char opt;

	params->app_name = NULL;

	while (argc > 0) {
		if (argv[0][0] != '-') {
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
		if (opt == 'd')
			params->boot_desc_flags |= OCTEON_BL_FLAG_DEBUG;
		else if (opt == 'b')
			params->boot_desc_flags |= OCTEON_BL_FLAG_BREAK;
		else {
			/* Get the option value string */
			sval = strchr(argv[0], '=');
			if (sval != NULL) {
				sval = sval + 1;
			} else if (argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if (opt == 'n')
			params->numcores = strtoul(sval, NULL, 0);
		if (opt == 'c')
			params->console_uart = strtoul(sval, NULL, 0);
		if (opt == 'm')
			cvmx_coremask_set64(&params->coremask,
				strtoull(sval, NULL, 16));
		if (opt == 's') {
			params->stack_size = strtoul(sval, NULL, 0);
			if (params->stack_size < (1 << 20))
				params->stack_size = 1 << 20;
		}

		if (opt == 'h') {
			params->heap_size = strtoul(sval, NULL, 0);
			if (params->heap_size < (3<<20))
				params->heap_size = 3<<20;
		}


		/*XXX- verify that no options are missing here */

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if (params->app_name == NULL) {
		fprintf(stderr, "ERROR: "
			"app name is required\n");
		return -1;
	}

	return 0;
}

/*
 * Parse "add" action options
 */
int oct_app_cmd_add_getopt(int argc, char **argv, struct app_params *params)
{
	const char *sval = NULL;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while (argc > 0) {
		if (argv[0][0] != '-') {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			sval = strchr(argv[0], '=');
			if (sval != NULL) {
				sval = sval + 1;
			} else if (argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if (opt == 'n')
			params->numcores = strtoul(sval, NULL, 0);

		if (opt == 'i')
			params->app_index = strtoul(sval, NULL, 0);

		if (opt == 'm')
			cvmx_coremask_set64(&params->coremask,
				strtoull(sval, NULL, 16));

		/*XXX- verify that no options are missing here*/

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if (params->app_name == NULL && params->app_index == -1) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}

/*
 * Parse "del" action options
 */
int oct_app_cmd_del_getopt(int argc, char **argv, struct app_params *params)
{
	const char *sval = NULL;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while (argc > 0) {
		if (argv[0][0] != '-') {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			sval = strchr(argv[0], '=');
			if (sval != NULL) {
				sval = sval + 1;
			} else if (argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}
		if (opt == 'n')
			params->numcores = strtoul(sval, NULL, 0);
		if (opt == 'm') {
			cvmx_coremask_set64(&params->coremask,
				strtoull(sval, NULL, 16));
			params->numcores = 0;
		}

		if (opt == 'i')
			params->app_index = strtoul(sval, NULL, 0);

		/*XXX- verify that no options are missing here*/

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if (params->app_name == NULL && params->app_index == -1) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}
/*
 * Parse "kill" action options
 */
int oct_app_cmd_kill_getopt(int argc, char **argv, struct app_params *params)
{
	const char *sval;
	char opt;

	params->app_name = NULL;
	params->app_index = -1;

	while (argc > 0) {
		if (argv[0][0] != '-') {
			params->app_name = argv[0];
			opt = '\0'; sval = "";
		} else {
			opt = argv[0][1];
			sval = strchr(argv[0], '=');
			if (sval != NULL) {
				sval = sval + 1;
			} else if (argc > 1 && isdigit(argv[1][0])) {
				argc--; argv++;
				sval = argv[0];
			}
		}

		if (opt == 'i')
			params->app_index = strtoul(sval, NULL, 0);

		/*XXX- verify that no options are missing here*/

		argv++; argc--;
	}

	/* Verify mandatory option is present */
	if (params->app_name == NULL && params->app_index == -1) {
		fprintf(stderr, "ERROR: "
			"Either app index or name is required\n");
		return -1;
	}

	return 0;
}


/*
 * Map the common hotplug data structure
 */
static int map_hotplug_info(int reset)
{
	uint64_t ccbi_addr = 0;
	uint64_t ccbi_size = sizeof(cvmx_cores_common_bootinfo_t);
	int ret;

	/* First check if (__common_bootinfo named block) exists and copy it
	 * If not found do create it and fill the appropriate values
	 * (all cores present and idle(XXX:core0?- BL runs)) in ccbi_g
	 */
	ret = octeon_remote_named_block_find(
		CVMX_APP_COMMON_BOOTINFO_NAME, &ccbi_addr, &ccbi_size);
	if (ret == 1) { /* named block was found, copy it to ccbi_g */
		if (reset == 0) {
			octeon_remote_read_mem2(&ccbi_g, ccbi_addr, ccbi_size);
		} else {
			memset(&ccbi_g, 0,
			       sizeof(cvmx_cores_common_bootinfo_t));
			cvmx_coremask_set_cores(&ccbi_g.hardware_coremask, 0,
						octeon_remote_get_num_cores());
			cvmx_coremask_set_cores(&ccbi_g.avail_coremask, 0,
						octeon_remote_get_num_cores());
#define RESERVE_CORE_0 0
#if RESERVE_CORE_0
			/* first core runs BL - not available */
			cvmx_coremask_clear_core(&ccbi_g.avail_coremask, 0);
#endif
			ccbi_g.magic = CVMX_COMMON_BOOTINFO_MAGIC;
			ccbi_g.version = CVMX_COMMON_BOOTINFO_VERSION;
			octeon_remote_write_mem2(ccbi_addr, &ccbi_g, ccbi_size);
		}

		if (verbose) {
			if (reset == 0)
				printf("<%s> named block "
					"FOUND and COPIED in ccbi_g\n",
					CVMX_APP_COMMON_BOOTINFO_NAME);
			else
				printf("<%s> named block was RESET\n",
					CVMX_APP_COMMON_BOOTINFO_NAME);
		}
	} else {	/* named block not found */
		ccbi_addr = octeon_remote_named_block_alloc(
			CVMX_APP_COMMON_BOOTINFO_SIZE,
			0x0, 1ull << 28, 0,
			CVMX_APP_COMMON_BOOTINFO_NAME);
		if (ccbi_addr == ~0ull) { /* create ccbi named block failed */
			fprintf(stderr,
				"ERROR: ccbi named_block not created!\n");
			return -1;
		}
		memset(&ccbi_g, 0, sizeof(cvmx_cores_common_bootinfo_t));
		cvmx_coremask_set_cores(&ccbi_g.hardware_coremask, 0,
					octeon_remote_get_num_cores());
		cvmx_coremask_set_cores(&ccbi_g.avail_coremask, 0,
					octeon_remote_get_num_cores());
#if RESERVE_CORE_0
		/* first core runs BL - not available */
		cvmx_coremask_clear_core(&ccbi_g.avail_coremask, 0);
#endif
		ccbi_g.magic = CVMX_COMMON_BOOTINFO_MAGIC;
		ccbi_g.version = CVMX_COMMON_BOOTINFO_VERSION;
		octeon_remote_write_mem2(ccbi_addr, &ccbi_g, ccbi_size);
		ccbi_pa = ccbi_addr;
		ccbi_len = ccbi_size;
		if (verbose) {
			printf("__common_bootinfo named block"
				" CREATED and FILLED\n");
			printf("pa=%p, ccbi_g.magic=%llx, "
				"ccbi_g.version=%llx\n",
				(void *)ccbi_pa, (long long)ccbi_g.magic,
				(long long)ccbi_g.version);
		}
	}
	/* Then check if hotplug named block exists and copy it
	 * If not found, create it and fill appropriate values
	 */
	ret = octeon_remote_named_block_find(
		CVMX_APP_HOTPLUG_INFO_REGION_NAME, &hp_pa, &hp_len);
	if (ret == 1) { /* named block was found, copy it to hp_g */
		if (reset == 0) { /* do not reset hotplug_data */
			read_hp_g_from_target();
			if (verbose)
				printf("<%s> named_block FOUND and "
					"COPIED in hp_g\n",
					CVMX_APP_HOTPLUG_INFO_REGION_NAME);
		} else {
			/* do reset hotplug_data */
			memset(&hp_g, 0, CVMX_APP_HOTPLUG_INFO_REGION_SIZE);
			hp_g.magic_version = CVMX_HOTPLUG_MAGIC_VERSION;

			/* Validate signature ccbi_g.magic */
			if (ccbi_g.magic != CVMX_COMMON_BOOTINFO_MAGIC) {
				printf("ERROR: ccbi_g.magic != "
				"CVMX_COMMON_BOOTINFO_MAGIC\n");
				return -1; /*if 'magic' does not match - exit*/
			}
			cvmx_coremask_copy(&hp_g.avail_coremask,
					   &ccbi_g.avail_coremask);

			hotplug_global_ptr = (cvmx_app_hotplug_global_t *)&hp_g;
			save_hp_g_on_target();

			printf("<%s> named_block was RESET.\n",
				CVMX_APP_HOTPLUG_INFO_REGION_NAME);
		}
	} else {	/* named block not found - create and fill it */
		hp_pa = octeon_remote_named_block_alloc(
			CVMX_APP_HOTPLUG_INFO_REGION_SIZE,
			0x0, 1ull << 29, 0,
			CVMX_APP_HOTPLUG_INFO_REGION_NAME);
		if (hp_pa == ~0ull) { /* create hotplug named block failed */
			fprintf(stderr,
				"ERROR: hotplug named_block not created!\n");
			return -1;
		}
		/* .. then fill them */
		memset(&hp_g, 0, CVMX_APP_HOTPLUG_INFO_REGION_SIZE);
		hp_g.magic_version = CVMX_HOTPLUG_MAGIC_VERSION;

		/* Validate signature ccbi_g.magic */
		if (ccbi_g.magic != CVMX_COMMON_BOOTINFO_MAGIC) {
			printf("ERROR: ccbi_g.magic != "
			"CVMX_COMMON_BOOTINFO_MAGIC\n");
			return -1; /* if 'magic' does not match - exit */
		}
		cvmx_coremask_copy(&hp_g.avail_coremask,
				   &ccbi_g.avail_coremask);
		/* right now only hp_g.magic_version and
		 * hp_g.avail_coremask are filled
		 */

		/* save the just created/filled struct hp_g to its named block
		 * NOTE: This is done just ONCE, to prevent overwriting members
		 * which are set by the SE app running on MIPS (hotplug_app)
		 * (i.e. callbacks, hotplug_activated_coremask, shutdown_done)
		 * Then all changes from remote_app_ctl are applied to both -
		 * the shadow structure hp_g and the hotplug_named_block, since
		 * at different (code) places either one or other are used
		 * It is OK to fill the shadow structure hp_g by reading from
		 * the hotplug_named_block when needed in order to get changes
		 * done (by the SE app or oct-remote-app-ctl)
		 * only to the hotplug_named_block
		 */
		hotplug_global_ptr = (cvmx_app_hotplug_global_t *)&hp_g;
		save_hp_g_on_target();

		if (verbose)
			printf("<%s> named_block CREATED - pa=%p\n",
				CVMX_APP_HOTPLUG_INFO_REGION_NAME,
				(void *) hp_pa);
	}

	hotplug_global_ptr = (cvmx_app_hotplug_global_t *)&hp_g;
	if (hp_g.magic_version != CVMX_HOTPLUG_MAGIC_VERSION) {
		fprintf(stderr, "ERROR: Invalid hotplug structure found, "
			"may be old version\n");
		printf("hp_g.magic_version=%llx, "
			"CVMX_HOTPLUG_MAGIC_VERSION=%llx\n",
			(long long)hp_g.magic_version,
		       CVMX_HOTPLUG_MAGIC_VERSION);
		return -1;
	}

	hotplug_info_ptr = hotplug_global_ptr->hotplug_info_array;
	if (verbose)
		fprintf(stderr, "DEBUG: "
			"Hotplug info region found...(hp_pa=%#llx)\n",
			(unsigned long long) hp_pa);

	return 0;
}

/*
 * Display current running application
 * Return number of available cores
 */
static int oct_app_info_show(const struct app_params *params)
{
	unsigned int i;
	int app_cnt = 0;
	cvmx_app_hotplug_info_t *hpinfo;
	char hexstr1[CVMX_MIPS_MAX_CORES/4+1];
	char hexstr2[CVMX_MIPS_MAX_CORES/4+1];

	read_hp_g_from_target();

	for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {

		hpinfo = &hotplug_info_ptr[i];
		if (!hpinfo->valid_magic)
			continue;
		app_cnt++;

		if (app_cnt == 1) {
			printf("%5s %20s %24s %24s\n",
				"Index", "Application Name",
				"Running Coremask", "Hotplug Active Coremask");
			printf("----------------------------------------"
				"----------------------------------------\n");
		}
		cvmx_coremask_bmp2str(
			&hpinfo->coremask, hexstr1);
		cvmx_coremask_bmp2str(
			&hpinfo->hotplug_activated_coremask, hexstr2);
		printf("  %3d %20s %24s %24s\n",
			i, hpinfo->app_name, hexstr1, hexstr2);
	} /* for i */

	/* Refresh available coremask if needed */
	(void) oct_app_cores_available();

	/* Count actual coures available */
	i = cvmx_coremask_get_core_count(&hotplug_global_ptr->avail_coremask);

	cvmx_coremask_bmp2str(&hotplug_global_ptr->avail_coremask, hexstr1);
	printf("Available %d cores, Coremask=0x%s\n", i, hexstr1);
	/* return number of available cores (to be used in scripts) */
	return i;
}

/*
 * Create or find a Master Boot Record named block
 */
static int app_boot_master_create(int reset)
{
	size_t pg_sz = sysconf(_SC_PAGESIZE);
	int ret;
	uint64_t mbr_addr = 0, mbr_size = 0;

	if (global_master_ptr && reset == 0)
		return 0; /* globals are already setup */

	/* Set the global pointer */
	global_master_ptr = &master_g;

	ret = octeon_remote_named_block_find(
		CVMX_APP_BOOT_MASTER_NAME, &mbr_addr, &mbr_size);
	if (ret == 1) { /* mbr named block found */
		if (verbose)
			printf("<%s> named_block found - pa=%p\n",
				CVMX_APP_BOOT_MASTER_NAME, (void *)mbr_addr);

		master_pa = mbr_addr;
		if (reset == 0) {
			read_master_g_from_target();
			if (master_g.magic != 0xb00710c4) {
				fprintf(stderr, "ERROR: "
					"invalid master boot block 'magic'\n");
				return -1;
			}
		} else {
			memset(&master_g, 0, sizeof(cvmx_app_boot_master_t));
			master_g.magic = 0xb00710c4;
			save_master_g_on_target();
		}
	} else { /* mbr named block not found - Allocate master boot block */
		mbr_addr = octeon_remote_named_block_alloc(
			sizeof(cvmx_app_boot_master_t),
			0, 0,
			pg_sz,
			CVMX_APP_BOOT_MASTER_NAME);
		if (~0ull == mbr_addr) {
			fprintf(stderr, "ERROR: "
				"failed to allocate master boot block\n");
			return -1;
		} else {
			if (verbose)
				fprintf(stderr, "INFO: "
					"Allocated new master boot block\n");
		}

		memset(&master_g, 0, sizeof(cvmx_app_boot_master_t));
		master_g.magic = 0xb00710c4;
		master_pa = mbr_addr;
		save_master_g_on_target();
	}
	if (verbose)
		fprintf(stderr, "DEBUG: master_pa=%#llx\n",
			(unsigned long long) master_pa);
	return 0;
}

/*
 * Initialize the primary boot record
 */
static int cvmx_app_boot_record_init(
	struct app_params *params,
	cvmx_app_boot_record_t *cabr)
{

	memset(&cabr->tlb_entries, 0, sizeof(cabr->tlb_entries));

	/* Fill in hotplug application slot */
	cabr->cabr_hotplug_app_index = params->app_index;
	cabr->no_mark_private_data = params->no_mark_private_data;

	cabr->cabr_stack_size = params->stack_size;
	cabr->cabr_heap_size = params->heap_size;

	if (cabr == &cabr_primary_g)
		memcpy(&cabr_primary_g, cabr, sizeof(*cabr));
	else if (cabr == &cabr_core_g)
		memcpy(&cabr_core_g, cabr, sizeof(*cabr));

	assert(cabr->cabr_paddr != 0);
	save_cabr_on_target(cabr);
	return 0;
}

/*
 * Get access to boot-loader LABI structure
 * map it once if necesary.
 * Return NULL if the structure does not pass sanity or can not be mapped.
 */
linux_app_boot_info_t *oct_app_get_labi(void)
{
	static linux_app_boot_info_t *labi = NULL;

	if (labi != NULL)
		return labi;

	/* We will get data values correct for x86 (i.e in LE) and if they are
	 * written back for the target MIPS, they should be converted to BE.
	 * First read labi with octeon_remote_read_mem2() in order to
	 * swap uint32_t members and endianess - needed for all uint32 members.
	 */
	octeon_remote_read_mem2(&labi_g, LABI_ADDR_IN_BOOTLOADER,
				sizeof(labi_g));
	/* FIX: InitTLBStart_addr is uint64_t and should not be swapped
	 * - swap it back
	 */
	labi_g.InitTLBStart_addr = __builtin_bswap64(labi_g.InitTLBStart_addr);
	/* FIX: read labi_g.gd verbatim to prevent any member swapping;
	 * NOTE that the endianess will be in BE, so if data should be displayed
	 * they should be converted by cvmx_beXX_to_cpu(), but when written back
	 * for the target MIPS, they should be just copied (assigned)
	 */
	octeon_remote_read_mem(&labi_g.gd,
				LABI_ADDR_IN_BOOTLOADER +
				offsetof(linux_app_boot_info_t, gd),
				sizeof(labi_g.gd));
	labi = &labi_g;
#ifdef DEBUG
	printf("sizeof(linux_app_boot_info_t)=%ld\n",
	       sizeof(linux_app_boot_info_t));
	printf("labi->labi_signature = 0x%x\n", (labi->labi_signature));
	printf("labi->start_core0_addr = 0x%x\n", (labi->start_core0_addr));
	printf("labi->avail_coremask = 0x%x\n", (labi->avail_coremask));
	printf("labi->pci_console_active = 0x%x\n", (labi->pci_console_active));
	printf("labi->icache_prefetch_disable = 0x%x\n",
	       (labi->icache_prefetch_disable));
	printf("labi->no_mark_private_data = 0x%x\n",
	       (labi->no_mark_private_data));
	printf("labi->InitTLBStart_addr = 0x%llx\n",
	       (unsigned long long)__builtin_bswap64(labi->InitTLBStart_addr));
	printf("labi->start_app_addr = 0x%x\n", (labi->start_app_addr));
	printf("labi->cur_exception_base = 0x%x\n", (labi->cur_exception_base));
	printf("labi->compact_flash_common_base_addr = 0x%x\n",
	       (labi->compact_flash_common_base_addr));
	printf("labi->compact_flash_attribute_base_addr = 0x%x\n",
	       (labi->compact_flash_attribute_base_addr));
	printf("labi->led_display_base_addr = 0x%x\n",
	       (labi->led_display_base_addr));
	printf("labi->gd.baudrate = %d\n", cvmx_be32_to_cpu(labi->gd.baudrate));
	printf("labi->gd.flags = %d\n", cvmx_be32_to_cpu(labi->gd.flags));
	printf("labi->gd.have_console = %d\n",
	       cvmx_be32_to_cpu(labi->gd.have_console));
	printf("labi->gd.board_desc.board_type = %d\n",
	       cvmx_be16_to_cpu(labi->gd.board_desc.board_type));
	printf("labi->gd.board_desc.rev_major = %d\n",
	       labi->gd.board_desc.rev_major);
	printf("labi->gd.board_desc.rev_minor = %d\n",
	       labi->gd.board_desc.rev_minor);
	printf("labi->gd.board_desc.serial_str = <%s>\n",
	       labi->gd.board_desc.serial_str);
	printf("labi->gd.cpu_clock_mhz = <%d>\n",
	       cvmx_be32_to_cpu(labi->gd.cpu_clock_mhz));
	printf("labi->gd.ddr_clock_mhz = <%d>\n",
	       cvmx_be32_to_cpu(labi->gd.ddr_clock_mhz));
	printf("labi->gd.ddr_ref_hertz = <%d>\n",
	       cvmx_be32_to_cpu(labi->gd.ddr_ref_hertz));
	printf("sizeof(labi->gd.bd=%p)=%ld\n",
	       labi->gd.bd, sizeof(labi->gd.bd));
	{
		unsigned int i;
		uint8_t *p = (uint8_t *)(void *)labi;
		for (i = 0; i < sizeof(*labi); i++) {
			if (i%16 == 0)
				printf("\nlabi:%3x:", i);
			printf(" %02x ", p[i]);
		}
	}
#endif
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
		const cvmx_app_boot_record_t *cabr_primary,
		cvmx_app_boot_record_t *cabr_core,
		unsigned core
		)
{
	int i;

	memset(&cabr_core->tlb_entries, 0, sizeof(cabr_core->tlb_entries));

	cabr_core->cabr_hotplug_app_index =
		cabr_primary->cabr_hotplug_app_index;

	cabr_core->cabr_init_core =
		cabr_primary->cabr_init_core;
	cabr_core->cabr_core = core;

	cabr_core->no_mark_private_data =
		cabr_primary->no_mark_private_data;
	cabr_core->cabr_entry_point  =
		cabr_primary->cabr_entry_point;

	cabr_core->cabr_heap_base =
		cabr_primary->cabr_heap_base;
	cabr_core->cabr_stack_size =
		cabr_primary->cabr_stack_size;
	cabr_core->cabr_heap_size =
		cabr_primary->cabr_heap_size;

	cabr_core->cabr_boot_info_block =
		cabr_primary->cabr_boot_info_block;

	cabr_core->boot_info_flags =
		cabr_primary->boot_info_flags;
	cabr_core->boot_desc_flags =
		cabr_primary->boot_desc_flags;
	cabr_core->boot_cfg_flags =
		cabr_primary->boot_cfg_flags;

	cabr_core->argc =
		cabr_primary->argc;

	for (i = 0; i < cabr_primary->argc && i < OCTEON_ARGV_MAX_ARGS; i++)
		cabr_core->argv[i] = cabr_primary->argv[i];

	strncpy(cabr_core->arg_buf_name, cabr_primary->arg_buf_name,
		sizeof(cabr_core->arg_buf_name));
}


/*
 * Access the per-core Boot Record data structure
 * or allocate one if not already present
 * If <core> is -1, the primary per-app boot record is returned.
 */
static cvmx_app_boot_record_t *app_boot_record_core_get(
		struct app_params *params,
		int core)
{
	cvmx_app_boot_record_t *cabr_ptr;
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_info_t *hip_pa;
	uint64_t paddr;
	int allocated = 0;

	hip = &(hotplug_global_ptr->hotplug_info_array[params->app_index]);
	hip_pa = get_hip_pa(params->app_index);

	if (core >= 0) {
		paddr = global_master_ptr->cabr_paddr[core];
		cabr_ptr = &cabr_core_g;
	} else {
		paddr = octeon_remote_read_mem64(
				(uintptr_t)&hip_pa->cvmx_app_boot_record_ptr);
		cabr_ptr = &cabr_primary_g;
	}

	/* Per-core boot record needs to be allocated */
	if (paddr == 0ULL) {
		/* This structure must be KSEG0 addresable */
		paddr = cvmx_bootmem_phy_alloc(
				sizeof(cvmx_app_boot_record_t),
				0, 1ull<<29, 128, 0);
		if (paddr == -1ULL || paddr == 0ULL) {
			fprintf(stderr, "ERROR: "
				"error allocationg per-core boot record "
				" for core %d\n",
				core);
			   return NULL;
		}
		allocated = 1;

		if (verbose > 2)
			fprintf(stderr, "DEBUG: "
				" allocated core %d cabr paddr=%#llx\n",
				core, (unsigned long long) paddr);
	} else {
		cabr_ptr->cabr_paddr = paddr;
		read_cabr_from_target(cabr_ptr);
	}

	if (allocated) {
		memset(cabr_ptr, 0, sizeof(*cabr_ptr));
		cabr_ptr->cabr_magic0 = CVMX_APP_BOOT_RECORD_MAGIC;
		cabr_ptr->cabr_magic1 = CVMX_APP_BOOT_RECORD_MAGIC;
		cabr_ptr->cabr_paddr = paddr;
		cabr_ptr->cabr_core = core;
		cabr_ptr->cabr_hotplug_app_index = params->app_index;
		save_cabr_on_target(cabr_ptr);
		/* Store the addr of the newly allocated per-core stucture */
		if (core >= 0) {
			global_master_ptr->cabr_paddr[core] = paddr;
			save_master_g_on_target();
		} else {
			hip->cvmx_app_boot_record_ptr = paddr;
			octeon_remote_write_mem64(
			(uintptr_t)&hip_pa->cvmx_app_boot_record_ptr, paddr);
		}
	}

	if (verbose)
		fprintf(stderr, "DEBUG: "
			"%s(*params, core=%d) returns %s(pa=%#llx, va=%p)\n",
			__func__, core,
			(core >= 0) ? "&cabr_core_g" : "&cabr_primary_g",
			(unsigned long long) cabr_ptr->cabr_paddr, cabr_ptr);
	return cabr_ptr;
}

/*
 * Free boot record memory
 *
 * This should work for either per-core or primary boot record
 */
static int app_boot_record_free(cvmx_app_boot_record_t *cabr)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_info_t *hip_pa;
	paddr_t paddr;
	unsigned app_index;
	short core;
	int ret;
	bool primary;

	if (cabr == NULL)
		return 0;

	core = cabr->cabr_core;
	app_index = cabr->cabr_hotplug_app_index;
	hip = &hotplug_global_ptr->hotplug_info_array[app_index];
	hip_pa = get_hip_pa(app_index);

	assert(core == -1 || core < CVMX_MAX_CORES);

	assert(app_index < CVMX_APP_HOTPLUG_MAX_APPS);

	assert(cabr->cabr_magic0 == CVMX_APP_BOOT_RECORD_MAGIC);
	assert(cabr->cabr_magic1 == CVMX_APP_BOOT_RECORD_MAGIC);

	/* Obtain physical address for this boot record, and delete reference */
	if (core >= 0) {
		paddr = global_master_ptr->cabr_paddr[core];
		global_master_ptr->cabr_paddr[core] = 0ull;
		/* master_g was changed - save it on target */
		save_master_g_on_target();
		primary = false;
		if (verbose > 1)
			fprintf(stderr, "INFO: "
				"Freeing boot record for core %d\n", core);
	} else {
		paddr = octeon_remote_read_mem64(
			(uintptr_t)&hip_pa->cvmx_app_boot_record_ptr);

		hip->cvmx_app_boot_record_ptr = 0ull;
		octeon_remote_write_mem64(
			(uintptr_t)&hip_pa->cvmx_app_boot_record_ptr, 0);

		/* shutdown overwrites *<hip>, use alternate storage */
		if (paddr == 0ull)
			paddr = cabr->cabr_paddr;
		primary = true;
		if (verbose > 1)
			fprintf(stderr, "INFO: "
				"Freeing primary boot record app_index %d\n",
				app_index);
	}


	/* Free pages mapped for this core or primary */
	(void) oct_app_memory_free(cabr->tlb_entries, primary);

	/* Free argument buffer */
	if (primary)
		(void) oct_args_free(cabr);

	cabr->cabr_magic0 = cabr->cabr_magic1 = 0ul;

	/* cabr was changed - save it (assert(cabr->cabr_paddr != 0)) */
	save_cabr_on_target(cabr);

	/* ..then we can zero cabr->cabr_paddr and remote_write it too */
	{
		uint64_t pa = cabr->cabr_paddr;
		cabr->cabr_paddr = 0ull;
		octeon_remote_write_mem64(pa, 0);
	}

	if (paddr == 0ull) {
		fprintf(stderr, "ERROR: "
			"Failed to retreive boot block physical address\n");
		return -1;
	}


	if (verbose > 2)
		fprintf(stderr, "DEBUG: "
			"Freeing boot record paddr=%#llx vaddr=%p\n",
			(unsigned long long) paddr, cabr);

	/* Now free the boot record itself */
	ret = __cvmx_bootmem_phy_free(
		paddr, sizeof(cvmx_app_boot_record_t), 0);

	/* ret = 1 on success */
	if (ret <= 0)
		fprintf(stderr, "WARNING: "
			"Error freeing physical bootmem at %#llx\n",
			(unsigned long long) paddr);

	/* Note: master boot record is never freed */

	return ret;
}

/*
 * Obtain a slot in the hotplug named block app array
 * and fill app info into the slot
 * NOTE: this operation needs to be atomic
 */
static int app_index_allocate(struct app_params *params)
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	cvmx_app_hotplug_global_t *hgp_pa = (void *)hp_pa;
	unsigned index;

	/* Obtain exclusive access */
	octeon_remote_spinlock_lock((uintptr_t)&hgp_pa->hotplug_global_lock);

	/* Walk the app array to find an empty slot */
	for (index = 0; index < CVMX_APP_HOTPLUG_MAX_APPS; index++) {
		if (hgp->hotplug_info_array[index].valid_magic)
			continue;
		/* If empty slot found, own it */
		hgp->hotplug_info_array[index].valid_magic =
			CVMX_HOTPLUG_MAGIC_VALID;
		octeon_remote_write_mem64((uintptr_t)
			&hgp_pa->hotplug_info_array[index].valid_magic,
			CVMX_HOTPLUG_MAGIC_VALID);

		strncpy(hgp->hotplug_info_array[index].app_name,
			params->app_name,
			CVMX_APP_HOTPLUG_MAX_APPNAME_LEN);
		octeon_remote_write_mem(
			(uintptr_t)&hgp_pa->hotplug_info_array[index].app_name,
			params->app_name,
			CVMX_APP_HOTPLUG_MAX_APPNAME_LEN);
		break;
	} /* for index */

	/* Release exclusive access */
	octeon_remote_spinlock_unlock((uintptr_t)&hgp_pa->hotplug_global_lock);

	if (index < CVMX_APP_HOTPLUG_MAX_APPS) {
		params->app_index = index;
		return 0;
	}
	return -ENOENT;
}

/*
 * Deallocate an app index
 */
int oct_app_index_free(struct app_params *params)
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	cvmx_app_hotplug_global_t *hgp_pa = (void *)hp_pa;
	unsigned index;

	index = params->app_index;

	/* Obtain exclusive access */
	octeon_remote_spinlock_lock((uintptr_t)&hgp_pa->hotplug_global_lock);

	/* Clear the entire structure, inclusive of <valid_magic> field */
	if (index < CVMX_APP_HOTPLUG_MAX_APPS) {
		cvmx_app_hotplug_info_t hotplug_info_struct;

		memset(&hgp->hotplug_info_array[index], 0,
			sizeof(hgp->hotplug_info_array[index]));
		memset(&hotplug_info_struct, 0, sizeof(hotplug_info_struct));
		octeon_remote_write_mem(
			(uintptr_t)&hgp_pa->hotplug_info_array[index],
			&hotplug_info_struct,
			sizeof(hotplug_info_struct));
	}

	/* Release exclusive access */
	octeon_remote_spinlock_unlock((uintptr_t)&hgp_pa->hotplug_global_lock);

	return 0;
}

/*
 * Check and print available coremask
 *
 */
static int oct_app_cores_available(void)
{
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];

	read_hp_g_from_target();

	if (hp_g.magic_version != CVMX_HOTPLUG_MAGIC_VERSION) {
		fprintf(stderr, "ERROR: "
			"Invalid hotplug structure found, "
			"may be old version\n");
		return -1;
	}
	cvmx_coremask_bmp2str(&hp_g.avail_coremask, hexstr);

	if (verbose > 2)
		fprintf(stderr, "DEBUG: "
			"Available coremask: 0x%s\n", hexstr);
	if (cvmx_coremask_is_empty(&hp_g.avail_coremask))
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
static int oct_app_cores_allocate(struct app_params *params,
	unsigned ncores,
	cvmx_coremask_t *add_coremask
	)
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	cvmx_app_hotplug_global_t  *hgp_pa = (cvmx_app_hotplug_global_t *)hp_pa;
	cvmx_coremask_t coremask;
	unsigned corecount;
	int i;


	if (add_coremask != NULL)
		coremask = *add_coremask;
	else
		memset(&coremask, 0, sizeof(coremask));

	corecount = 0;

	/* Obtain exclusive access */
	octeon_remote_spinlock_lock((uintptr_t)&hgp_pa->hotplug_global_lock);

	read_hp_g_from_target();

	/* First try to apply specific coremask requested */
	if (add_coremask != NULL) {
		/* Count how many cores the mask requests */
		for (i = ncores = 0; i < 64; i++)
			if (cvmx_coremask_is_core_set(&coremask, i))
				ncores++;

		/* <AND> requested and available coremasks */
		oct_remote_read_coremask(
			(uintptr_t)&hgp_pa->avail_coremask,
					 &hgp->avail_coremask);
		cvmx_coremask_and(&coremask, add_coremask,
				  &hgp->avail_coremask);

		/* If <AND> result is the same, we can get the exact mask */
		if (cvmx_coremask_cmp(add_coremask, &coremask) == 0) {
			cvmx_coremask_xor(&hgp->avail_coremask, &coremask,
				&hgp->avail_coremask);
			oct_remote_write_coremask(
				(uintptr_t)&hgp_pa->avail_coremask,
				&hgp->avail_coremask);
			/* flush PCI write buffers */
			(void) octeon_remote_get_model();
			corecount = ncores;
		}
	}

	/* if no specific mask requested, or strict mask allocation failed */
	if (corecount == 0) {
		/* Try to claim the number of cores requested */
		cvmx_coremask_for_each_core(i, &hgp->avail_coremask) {
			if (!cvmx_coremask_is_core_set(
				&hgp->avail_coremask, i))
				continue;
			cvmx_coremask_clear_core(&hgp->avail_coremask, i);
			oct_remote_write_coremask(
				(uintptr_t)&hgp_pa->avail_coremask,
				&hgp->avail_coremask);
			/* flush PCI write buffers */
			(void) octeon_remote_get_model();
			cvmx_coremask_set_core(&coremask, i);
			corecount++;
			if (ncores == corecount)
				break;
		} /* cvmx_coremask_for_each_core */
	}

	/* Release exclusivce access */
	octeon_remote_spinlock_unlock((uintptr_t)&hgp_pa->hotplug_global_lock);

	if (corecount == 0) {
		fprintf(stderr, "ERROR: "
			"%u CPU cores requested, "
			"%u CPU cores available, "
			"aborting.\n",
			ncores, corecount);
			return -EBUSY;
	} else if (ncores > corecount && corecount > 0) {
		fprintf(stderr, "WARNING: "
			"%u CPU cores requested, "
			"%u CPU cores available, "
			"proceeding with fewer cores.",
			ncores, corecount);
	} else if (add_coremask != NULL &&
		   cvmx_coremask_cmp(add_coremask, &coremask) != 0) {
		char hexstr[CVMX_MIPS_MAX_CORES/4+1];
		cvmx_coremask_bmp2str(&coremask, hexstr);
		fprintf(stderr, "WARNING: "
			"requested coremask invalid, actual coremask=0x%s\n",
			hexstr);
	}

	/* Save obtained coremask in params */
	params->coremask = coremask;
	if (verbose) {
		fprintf(stderr,	"INFO: allocated_cores_coremask:");
		cvmx_coremask_print(&coremask);
	}

	return corecount;
}

/*
 * Deallocate processor cores in case of an error starting an app
 */
int oct_app_cores_free(struct app_params *params)
{
	cvmx_app_hotplug_global_t *hgp = hotplug_global_ptr;
	cvmx_app_hotplug_global_t  *hgp_pa = (cvmx_app_hotplug_global_t *)hp_pa;
	cvmx_coremask_t ac;
	int i;

	/* Obtain exclusive access */
	octeon_remote_spinlock_lock((uintptr_t)&hgp_pa->hotplug_global_lock);

	/* Release the cores that were claimed earlier */
	oct_remote_read_coremask((uintptr_t)&hgp_pa->avail_coremask, &ac);
	cvmx_coremask_for_each_core(i, &params->coremask) {
		cvmx_coremask_set_core(&hgp->avail_coremask, i);
		cvmx_coremask_set_core(&ac, i);

	} /* cvmx_coremask_for_each_core */
	oct_remote_write_coremask((uintptr_t)&hgp_pa->avail_coremask, &ac);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	/* Release exclusive access */
	octeon_remote_spinlock_unlock((uintptr_t)&hgp_pa->hotplug_global_lock);

	return 0;
}

/*
 * Find the location of the system-wide boot vector table
 * for a given node.
 */
static boot_init_vector_t *oct_app_get_boot_vector(unsigned node, unsigned core)
{
	/* NOTE: The returned (*boot_vect) will be the boot vector physical addr
	 * and just will be cast to virtual (boot_init_vector_t *boot_vect)
	 */
	boot_init_vector_t *boot_vect;
	cvmx_mio_boot_loc_cfgx_t boot_loc_cfg;
	uint64_t magic, addr = 0, value;
	int i;
	/* oct-remote_app_ctl supports only node 0 */
/* boot_loc_cfg.u64 = cvmx_read_csr_node(node, CVMX_MIO_BOOT_LOC_CFGX(0)); */
	boot_loc_cfg.u64 = octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_CFGX(0));
	if (!boot_loc_cfg.s.en) {
		fprintf(stderr, "ERROR: MIO_BOOT_LOC_CFG not enabled\n");
		goto _old_bloader;
	}

	/* !!! reading octeon's local cache regions trhu pci doesn't work !!!
	 * Instead we will extract these data by csr access and store only
	 * what we need, i.e BOOT_MOVEABLE_MAGIC and BOOT_MOVEABLE_OFFSET
	 */
	octeon_remote_write_csr(CVMX_MIO_BOOT_LOC_ADR, 0ull);
	for (i = 0; i < 16; i++) {
		value = octeon_remote_read_csr(CVMX_MIO_BOOT_LOC_DAT);
		if (verbose)
			fprintf(stderr, "%2d: %#llx\n", i,
				(unsigned long long) value);
		switch (i * sizeof(uint64_t)) {
		case OCTEON_BOOT_MOVEABLE_MAGIC_OFFSET/*0x70*/:
			magic = value;
			break;
		case OCTEON_BOOT_VECTOR_MOVEABLE_OFFSET/*0x78*/:
			addr = value;
			break;
		default:
			break;
		}
	}

	if (verbose > 1)
		fprintf(stderr, "DEBUG: "
			"Moveable address = %#llx\n",
			(unsigned long long) addr);

	if (verbose > 1)
		fprintf(stderr, "DEBUG: "
			"Moveable magic = 0x%llx\n",
			(unsigned long long) magic);

	/* The previous newest vector from u-boot */
	if (magic != OCTEON_BOOT_MOVEABLE_MAGIC &&
	    magic != OCTEON_BOOT_MOVEABLE_MAGIC1) {
		fprintf(stderr, "ERROR: "
			"MOVEABLE_MAGIC invalid %#llx\n",
			(unsigned long long)magic);
		goto _old_bloader;
	}

	/* Convert to ptr for return (it is still paddr) */
	boot_vect = (boot_init_vector_t *) addr;

	if (verbose > 1)
		fprintf(stderr, "DEBUG: "
			"new boot_vect location paddr %#llx, vaddr %p\n",
			(unsigned long long)addr, boot_vect);
	return boot_vect;

_old_bloader:
	boot_vect = (boot_init_vector_t *)
			octeon_remote_read_mem64(BOOTLOADER_BOOT_VECTOR);
	fprintf(stderr, "WARNING: old bootloader assumed\n");
	if (verbose)
		fprintf(stderr, "INFO: "
			"boot_vect paddr %#llx, vaddr %p\n",
			(unsigned long long)BOOTLOADER_BOOT_VECTOR, boot_vect);
	/* FIXME: should find a way to validate old boot vector too */
	return boot_vect;

}

/*
 * Fill-in the boot vector shared with bootloader
 */
static int oct_app_start_cores(
	struct app_params *params,
	cvmx_coremask_t *pcm)
{
	const linux_app_boot_info_t *labi;
	boot_init_vector_t *boot_vect;
	cvmx_app_boot_record_t *cabr_ptr;
	uint64_t boot_info_block_paddr;
	uint64_t cabr_paddr;
	unsigned int node = 0;	/* boot_vect is on node 0 */
	int core;

	labi = oct_app_get_labi();

	if (verbose > 2)
		fprintf(stderr, "DEBUG: "
			"start_core0_addr %#x\n", labi->start_core0_addr);

	cvmx_coremask_for_each_core(core, pcm) {
		boot_vect = oct_app_get_boot_vector(node, core);

		/* Get per-core structure address virtual address */
		cabr_ptr = app_boot_record_core_get(params, core);

		/* per-core boot record should have been allocated earlier */
		if (cabr_ptr == NULL)
			continue;

		/* NOTE: Since this app is compiled for host(INTEL CPU) it uses
		 * __LITTLE_ENDIAN. The same apply for the included cvmx*.[hc]
		 * However the structures shared with the bootloader was
		 * compiled with __BIG_ENDIAN, so here we swap some uint32_t
		 * members (k0_val, app_start_func_addr) appropriately
		 * */
		if (verbose > 2)
			fprintf(stderr, "DEBUG: Old values: "
				"boot_vect[core].k0_val %#x; "
				"boot_vect[core].code_addr %#llx; "
				"boot_vect[core].start_func_addr %#x\n",
				octeon_remote_read_mem32(
					(uint64_t)&boot_vect[core] +
					offsetof/*k0_val*/(
						struct boot_init_vector,
						app_start_func_addr)),
				(unsigned long long)octeon_remote_read_mem64(
					(uint64_t)&boot_vect[core] +
					offsetof(struct boot_init_vector,
						 code_addr)),
				octeon_remote_read_mem32(
					(uint64_t)&boot_vect[core] +
					offsetof/*app_start_func_addr*/
					(struct boot_init_vector, k0_val))
			);

		/* Fill-in entry-points */
		if (verbose)
			fprintf(stderr,
				"labi->InitTLBStart_addr=%#llx, "
				"labi->start_app_addr=%#x\n",
				(unsigned long long)cvmx_cpu_to_be64(
					(unsigned long long)
					labi->InitTLBStart_addr),
					labi->start_app_addr);
		octeon_remote_write_mem64((uint64_t)&boot_vect[core] +
					offsetof(struct boot_init_vector,
						 code_addr),
					cvmx_cpu_to_be64(
						labi->InitTLBStart_addr));
		octeon_remote_write_mem32((uint64_t)&boot_vect[core] +
					offsetof/*app_start_func_addr*/
					(struct boot_init_vector, k0_val),
					labi->start_app_addr);
		/* in order to fix endianess we also have to swap uint32_t
		 * 'pad' and 'flags' members of 'struct boot_init_vector'.
		 * However 'pad' is not used, so we only write 'flags' over it
		 */
		octeon_remote_write_mem32((uint64_t)&boot_vect[core] +
			offsetof/*flags*/(struct boot_init_vector, pad),
			octeon_remote_read_mem32((uint64_t)&boot_vect[core] +
				offsetof/*pad*/
				(struct boot_init_vector, flags)));

		if (verbose > 2)
			fprintf(stderr, "DEBUG: New values: "
				"boot_vect[core].code_addr %#llx; "
				"boot_vect[core].start_func_addr %#x\n",
				(unsigned long long)octeon_remote_read_mem64(
					(uint64_t)&boot_vect[core] +
					offsetof(struct boot_init_vector,
						 code_addr)),
				octeon_remote_read_mem32(
					(uint64_t)&boot_vect[core] +
					offsetof/*app_start_func_addr*/
					(struct boot_init_vector, k0_val))
			);
		/* Get per-core boot structure physical address
		 * we do this by first updating master_g,
		 * then assign master_g.cabr_paddr[core] to cabr_paddr
		 */
		read_master_g_from_target();
		cabr_paddr = master_g.cabr_paddr[core];
		save_master_g_on_target();

		/* Calculate per-core boot_info_block physical address */
		boot_info_block_paddr = cabr_paddr +
			offsetof(cvmx_app_boot_record_t,
				cabr_boot_info_block);

		if (verbose > 2)
			fprintf(stderr, "DEBUG: "
				" core %u: cabr vaddr=%p paddr=%#llx\n",
				core, cabr_ptr,
				(unsigned long long) cabr_paddr);

		/* Write boot_info_block address into boot vector */
		octeon_remote_write_mem64((uint64_t)&boot_vect[core] +
					offsetof(struct boot_init_vector,
						 boot_info_addr),
					boot_info_block_paddr);

		if (verbose > 2)
			fprintf(stderr, "DEBUG: "
				"core: %u boot_info_block paddr=%#llx\n",
				core,
				(unsigned long long) boot_info_block_paddr
				);
		/* flush PCI write buffers */
		(void) octeon_remote_get_model();
	} /* cvmx_coremask_for_each_core */

	/* Drain all output before giving the SE app a chance to run */
	fflush(stderr);
	tcdrain(fileno(stderr));

	for (node = 0; node < CVMX_MAX_NODES; node++) {
		uint64_t node_core_mask;
		node_core_mask = cvmx_coremask_get64_node(pcm, node);

		if (node_core_mask == 0)
			continue;
		if (verbose > 2) {
			fprintf(stderr, "DEBUG: "
				"Writing %#llx to CVMX_CIU_NMI on node %u\n",
				(unsigned long long) node_core_mask, node);
			fflush(stderr);
			tcdrain(fileno(stderr));
		}
		if (octeon_has_feature(OCTEON_FEATURE_CIU3))
			octeon_remote_write_csr(CVMX_CIU3_NMI, node_core_mask);
		else
			octeon_remote_write_csr(CVMX_CIU_NMI, node_core_mask);
		usleep(10000);
		/* flush PCI write buffers */
		(void) octeon_remote_get_model();
	}  /* for node */
	return 0;
}




/*
 * Booting of a new app
 */
int oct_app_boot(struct app_params *params)
{
	cvmx_app_boot_record_t *cabr_ptr;
	cvmx_app_boot_segment_t	_segments[CVMX_APP_MAX_SEGMENTS];
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_info_t *hip_pa;
	cvmx_app_hotplug_global_t *hgp_pa = (void *)hp_pa;
	const linux_app_boot_info_t *labi;
	short allocated_cores;
	int core;
	int fd;
	int ret;

	bzero(_segments, sizeof(_segments));
	if (oct_app_cores_available() < 0) {
		fprintf(stderr, "ERROR: "
			"No cores are available\n");
		return -1;
	}
	/* Obtain a hotplug app slot */
	if (app_index_allocate(params) < 0) {
		fprintf(stderr, "ERROR: "
			"Maximum number of applications already running\n");
		return -1;
	}
	/* Lock hotplugging mechanism */
	set_app_under_boot(params->app_index+1);

	labi = oct_app_get_labi();

	/* This flag is needed early */
	params->no_mark_private_data = labi->no_mark_private_data;

	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];
	hip_pa = get_hip_pa(params->app_index);

	/* Open file */
	fd = open(params->app_name, O_RDONLY);
	if (fd < 0) {
		perror(params->app_name);
		oct_app_index_free(params);
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	    }

	/* Obtain the needed CPU cores */
	allocated_cores = 0;

	if (!cvmx_coremask_is_empty(&params->coremask))
		allocated_cores = oct_app_cores_allocate(params,
				0, &params->coremask);

	if (allocated_cores <= 0)
		allocated_cores = oct_app_cores_allocate(params,
				params->numcores, NULL);

	if (allocated_cores <= 0) {
		/* Cleanup */
		oct_app_index_free(params);
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}
	/* Get/Create primary boot record */
	cabr_ptr =  app_boot_record_core_get(params, -1);
	if (cabr_ptr == NULL) {
		oct_app_index_free(params);
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}
	cvmx_app_boot_record_init(params, cabr_ptr);
	oct_tlb_flags_init(cabr_ptr);

	/* Parse ELF structure into first core boot record */
	if (elf_file_prep(fd, cabr_ptr, _segments, CVMX_APP_MAX_SEGMENTS) < 0) {

		oct_app_index_free(params);
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}

	/* Add arguments segment */
	if (oct_args_prep(cabr_ptr, params->argc, params->argv) < 0) {
		fprintf(stderr, "ERROR: failed to prep arguments\n");
		oct_app_index_free(params);
		/* Unlock hotplugging mechanism */
		set_app_under_boot(0);
		return -1;
	}

	/* Save initial boot flags for all subsequent cores */
	cabr_ptr->boot_info_flags = params->boot_info_flags;
	cabr_ptr->boot_desc_flags = params->boot_desc_flags;

	/* Create primary memory map */
	ret = oct_tlb_primary_init(fd,
		cabr_ptr->tlb_entries,
		_segments,
		 CVMX_APP_MAX_SEGMENTS);

	if (ret < 0) {
		fprintf(stderr, "ERROR: oct_tlb_primary_init(...) FAIL!\n");
		goto _app_boot_fail;
	}
	/* Create heap/stack templace segment in the primary boot record */
	cabr_ptr->cabr_heap_base =
		    oct_tlb_primary_heap_stack_locate(
			cabr_ptr->tlb_entries,
			cabr_ptr->cabr_heap_size, cabr_ptr->cabr_stack_size);
	cabr_ptr->cabr_init_core =
		cvmx_coremask_get_first_core(&params->coremask);

	/* save cabr on target */
	if (verbose)
		printf("DEBUG: writing cabr_ptr %p to pa=%#llx\n",
		       cabr_ptr, (unsigned long long) cabr_ptr->cabr_paddr);
	save_cabr_on_target(cabr_ptr);

	/* Prepare memory for the additional cores */
	cvmx_coremask_for_each_core(core, &params->coremask) {
		cvmx_app_boot_record_t *cabr_core_ptr;

		if (verbose)
			fprintf(stderr, "INFO: "
				"Making meta-data for core %d\n", core);

		cabr_core_ptr = app_boot_record_core_get(params, core);
		if (cabr_core_ptr == NULL) {
			/* XXX free the failed core XXX */
			ret = -2;
			break;
		}

		/* Copy essential fields from primary CABR to per-core */
		oct_app_boot_record_clone(cabr_ptr, cabr_core_ptr, core);

		if (verbose) {
			fprintf(stderr, "DEBUG: "
				"cabr_ptr=%p, cabr_core_ptr=%p (core=%d)\n",
				cabr_ptr, cabr_core_ptr, core);
			fprintf(stderr, "DEBUG: "
				"cabr_ptr->tlb_entries=%p,"
				"cabr_core_ptr->tlb_entries=%p (core=%d)\n",
				cabr_ptr->tlb_entries,
				cabr_core_ptr->tlb_entries, core);
			oct_tlb_array_print(cabr_ptr->tlb_entries,
					    "cabr_ptr TLBs");
		}

		/* Clone memory map from primary */
		ret = oct_app_memory_clone(
			cabr_ptr->tlb_entries,
			cabr_core_ptr->tlb_entries);

		if (ret < 0)
			break;

		/* Adjust boot descriptor in cloned boot record */
		ret = oct_boot_desc_setup(params, cabr_core_ptr);
		save_cabr_on_target(cabr_core_ptr);
		if (ret < 0)
			break;
	}

	if (ret < 0) {
		fprintf(stderr, "ERROR: "
			"failed to setup boot descriptors\n");
		goto _app_boot_fail;
	}

	/* Store obtained coremask in hotplug app slot */
	hip->coremask = params->coremask;
	oct_remote_write_coremask(
		(uintptr_t)&hip_pa->coremask, &params->coremask);

	hip->valid_magic = CVMX_HOTPLUG_MAGIC_VALID;
	octeon_remote_write_mem64(
		(uintptr_t)&hip_pa->valid_magic, CVMX_HOTPLUG_MAGIC_VALID);

	/* Convert coremask to string for printing */
	cvmx_coremask_bmp2str(&params->coremask, hexstr);

	if (verbose)
		fprintf(stderr, "INFO: "
			"Launching application %s on coremask 0x%s\n",
			params->app_name, hexstr);

	if (oct_app_start_cores(params, &params->coremask) < 0) {
		fprintf(stderr, "ERROR: "
			"Error starting application\n");
		goto _app_boot_fail;
	}

	/* Unlock hotplugging mechanism */
	set_app_under_boot(0);

	/* Wait for the SE app to print its wares */
	usleep(250000);
	cvmx_coremask_bmp2str(&hip->coremask, hexstr);
	fprintf(stderr, "INFO: SUCCESS, App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr);
	oct_remote_read_coremask(
		(uintptr_t)&hgp_pa->avail_coremask,
		&hotplug_global_ptr->avail_coremask);

	cvmx_coremask_bmp2str(&hotplug_global_ptr->avail_coremask, hexstr);
	fprintf(stderr, "INFO: Available cores 0x%s\n", hexstr);

	return 0;

_app_boot_fail:
	/* XXX + free allocated program memories XXX */

	/* Xfree args buffer */
	oct_args_free(cabr_ptr);
	oct_app_cores_free(params);
	oct_app_index_free(params);
	/* Unlock hotplugging mechanism */
	set_app_under_boot(0);
	return -1;
}

/*
 * Derive app_index from app_name if index is not supplied
 */
static int oct_app_index_verify(struct app_params *params)
{
	cvmx_app_hotplug_info_t *hip;
	unsigned i;

	read_hp_g_from_target();
	/* Find app_index if unspecified */
	if (params->app_index < 0) {
		if (params->app_name == NULL) {
			fprintf(stderr, "ERROR: "
				"Either app index or name is required\n");
			return -1;
		}

		for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {

			hip = &hotplug_global_ptr->hotplug_info_array[i];

			if (!hip->valid_magic)
				continue;

			if (hip->valid_magic != CVMX_HOTPLUG_MAGIC_VALID) {
				fprintf(stderr, "ERROR: "
					"App '%s' info not valid\n",
					params->app_name);
				return -1;
			}
			if (!strcmp(params->app_name, hip->app_name))
				break;
		}

		if (i >= CVMX_APP_HOTPLUG_MAX_APPS) {
			fprintf(stderr, "ERROR: "
				"App '%s' not found\n", params->app_name);
			return -1;
		}
		params->app_index = i;
		if (verbose)
			fprintf(stderr, "INFO: "
				"App '%s' found at index %d\n",
				params->app_name, i);
		return params->app_index;
	} /* uf app_index == -1 */

	if (params->app_index >= CVMX_APP_HOTPLUG_MAX_APPS) {
		fprintf(stderr, "ERROR: "
			"Application index %d out of range\n",
			params->app_index);
		return -1;
	}
	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];
	if (hip->valid_magic != CVMX_HOTPLUG_MAGIC_VALID) {
		fprintf(stderr, "ERROR: "
			"Application index %d is not valid\n",
			params->app_index);
		return -1;

	}
	return params->app_index;
}

/*
 * Add cores to a running app
 */
static int oct_add_cores(struct app_params *params)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_info_t *hip_pa;
	cvmx_coremask_t add_coremask;
	cvmx_coremask_t empty_cm;
	cvmx_app_boot_record_t *cabr_ptr;
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	int hot_pluggable, cores_allocated;
	int core;
	int ret, i;

	cvmx_coremask_clear_all(&empty_cm);

	add_coremask = params->coremask;
	cores_allocated = 0;

	read_hp_g_from_target();

	/* Find app_index if unspecified */
	ret = oct_app_index_verify(params);
	if (ret < 0)
		return ret;

	if (is_app_under_boot_or_shutdown()) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n");
		return -1;
	}

	if (oct_app_cores_available() < 0) {
		fprintf(stderr, "ERROR: "
			"There are no available cores to be plugged\n");
		return -1;
	}


	/* Get primary boot record pointer */
	cabr_ptr = app_boot_record_core_get(params, -1);
	if (cabr_ptr == NULL)
		return -1;

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
	set_app_under_boot(params->app_index+1);

	/* Obtain pointer to app slot info block */
	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];
	hip_pa = get_hip_pa(params->app_index);

	/* Try to obtain CPU cores according to specific mask */
	if (!cvmx_coremask_is_empty(&add_coremask)) {
		cores_allocated =
			oct_app_cores_allocate(params, 0, &add_coremask);
	}

	/* Obtain the needed CPU cores by count */
	if (cores_allocated == 0) {
		cores_allocated =
			oct_app_cores_allocate(params,
				params->numcores, NULL);
	}

	if (cores_allocated <= 0) {
		fprintf(stderr, "ERROR: "
			"Failed to add %u cores to app at index %u\n",
			params->numcores, params->app_index);
		return -1;
	}

	params->numcores = cores_allocated;
	cvmx_coremask_copy(&add_coremask, &params->coremask);
	oct_remote_read_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);
	cvmx_coremask_or(&params->coremask, &add_coremask, &hip->coremask);

	/* Spit out all relevant coremasks for debugging */
	cvmx_coremask_bmp2str(&add_coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
		"Adding %d cores to app %u coremask=0x%s\n",
		cores_allocated, params->app_index, hexstr);

	oct_remote_read_coremask(
		(uintptr_t)&hip_pa->hotplug_activated_coremask,
		&hip->hotplug_activated_coremask);
	cvmx_coremask_bmp2str(&hip->hotplug_activated_coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
		"Hotplug activated cores 0x%s\n", hexstr);

	/* Clear down-going core masks to avoid confusion */
	cvmx_coremask_clear_all(&hip->shutdown_cores);
	cvmx_coremask_clear_all(&hip->unplug_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->shutdown_cores,
				  &empty_cm);
	oct_remote_write_coremask((uintptr_t)&hip_pa->unplug_cores, &empty_cm);

	/* hplugged_cores contains the new cores only */
	cvmx_coremask_copy(&hip->hplugged_cores, &add_coremask);
	oct_remote_write_coremask(
		(uintptr_t)&hip_pa->hplugged_cores, &add_coremask);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	cvmx_coremask_bmp2str(&hip->hplugged_cores, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
			"Hotplug cores 0x%s\n", hexstr);

	oct_tlb_flags_init(cabr_ptr);

	cabr_ptr->boot_desc_flags |= OCTEON_BL_FLAG_HPLUG_CORES;

	cvmx_coremask_for_each_core(core, &add_coremask) {
		cvmx_app_boot_record_t *cabr_core_ptr;

		if (verbose)
			fprintf(stderr, "INFO: "
				"Preparing meta-data for core %d\n", core);

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if (cabr_core_ptr == NULL) {
			/* XXX free the failed core XXX */
			ret = -2;
			break;
		}

		/* Copy essential fields from primary CABR to per-core */
		oct_app_boot_record_clone(cabr_ptr, cabr_core_ptr, core);

		cabr_core_ptr->boot_desc_flags |= OCTEON_BL_FLAG_HPLUG_CORES;

		/* Clone memory map from primary */
		ret = oct_app_memory_clone(
			cabr_ptr->tlb_entries,
			cabr_core_ptr->tlb_entries);

		if (ret < 0)
			break;

		/* Adjust boot descriptor in cloned boot record */
		ret = oct_boot_desc_setup(params, cabr_core_ptr);

		/* update target memory with changed shadow structure
		 * (*cabr_core_ptr)
		 */
		save_cabr_on_target(cabr_core_ptr);
		if (ret < 0)
			break;
	}

	if (ret < 0) {
		fprintf(stderr, "ERROR: "
			"failed to setup boot descriptors\n");
		goto _app_add_fail;
	}

	if (oct_app_start_cores(params, &add_coremask) < 0) {
		fprintf(stderr, "ERROR: "
			"Error starting application\n");
		goto _app_add_fail;
	}

	/*
	 * wait for new cores to show up in activated_coremask before
	 * firing the callback
	 */
	i = 5 * 1000000;
	do {
		cvmx_coremask_t tmp_mask, hac;
		unsigned t = 10000;

		oct_remote_read_coremask(
			(uintptr_t)&hip_pa->hotplug_activated_coremask, &hac);
		cvmx_coremask_and(&tmp_mask, &add_coremask, &hac);
		if (0 == cvmx_coremask_cmp(&add_coremask, &tmp_mask))
			break;

		if (verbose) {
			fprintf(stderr, "INFO: "
				"Waiting for cores to register\n");
			t = 250000;
		}

		usleep(t);
		i -= t;
	} while (i > 0);

	if (i <= 0) {
		fprintf(stderr, "ERROR:WARNING: "
			"Timeout while waiting for new cores to start\n");
		usleep(250000);
		return -1;
	}

	/* hotplug_global named block on target got modified - read it back */
	oct_remote_read_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);

	cvmx_coremask_bmp2str(&hip->coremask, hexstr);

	if (verbose)
		fprintf(stderr, "INFO: "
			"Sending MBOX to cores 0x%s\n", hexstr);

	fflush(stderr);
	tcdrain(fileno(stderr));

	/* Send a Mailbox interrupt to invoke callback */
	cvmx_app_hotplug_call_add_cores_callback(params->app_index);

	/* Wait a little for the callbacks to run - no flag to wait on */
	usleep(250000);

	/* Append these new cores to the application coremask */
	cvmx_coremask_or(&hip->coremask, &hip->coremask, &add_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();
	cvmx_coremask_bmp2str(&hip->coremask, hexstr);

	fprintf(stderr, "INFO: SUCCESS, App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr);

	/* Clear the cores-added mask */
	cvmx_coremask_clear_all(&hip->hplugged_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->hplugged_cores,
				  &empty_cm);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	/* Unlock the hotplugging mechanism */
	set_app_under_boot(0);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	return 0;

_app_add_fail:

	/* Reverse the effect of adding new cores to hplugged_cores */
	cvmx_coremask_clear_all(&hip->hplugged_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->hplugged_cores,
				  &empty_cm);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	/* Make the cores allocated available again */
	oct_app_cores_free(params);

	/* Unlock the hotplugging mechanism */
	set_app_under_boot(0);
	/* flush PCI write buffers */
	(void) octeon_remote_get_model();

	return -1;
}

/*
 * Delete cores from a running application
 * all but the first initial core can be put to sleep
 * by this operation, reducing the number of cores to 1.
 */
static int oct_del_cores(struct app_params *params)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_coremask_t del_coremask, empty_coremask;
	cvmx_app_boot_record_t *cabr_ptr;
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	int core, i, ret;
	int hot_pluggable;
	unsigned n, numcores;
	unsigned cores_deleted;
	cvmx_app_hotplug_info_t *hip_pa;

	del_coremask = params->coremask;
	numcores = params->numcores;
	cores_deleted = 0;
	cvmx_coremask_clear_all(&empty_coremask);

	ret = oct_app_index_verify(params);
	if (ret < 0)
		return ret;

	hip_pa = get_hip_pa(params->app_index);

	/* Get primary boot record pointer for this app */
	cabr_ptr/*va*/ = app_boot_record_core_get(params, -1);
	if (cabr_ptr == NULL)
		return -1;

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

	if (is_app_under_boot_or_shutdown()) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n");
		return -1;
	}

	/* Lock the hotplugging mechanism */
	set_app_under_shutdown(params->app_index + 1);

	/* Obtain pointer to app slot info block */
	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];

	oct_remote_read_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);
	/* Filter explicit coremask with the actual cores running the app */
	if (!cvmx_coremask_is_empty(&del_coremask)) {
		numcores = cvmx_coremask_get_core_count(&del_coremask);
		cvmx_coremask_and(&del_coremask, &del_coremask, &hip->coremask);
		/* Make sure no one is trying to unplug the init_core */
		cvmx_coremask_clear_core(&del_coremask,
			cabr_ptr->cabr_init_core);
		cores_deleted = cvmx_coremask_get_core_count(&del_coremask);
	}

	/* Find cores to delete by requested count */
	if (cores_deleted < numcores) {
		if (numcores < cores_deleted)
			numcores = cores_deleted;

		/* Limit numcores to running cores - 1 */
		n = cvmx_coremask_get_core_count(&hip->coremask) - 1;
		if (numcores > n)
			numcores = n;

		while (cores_deleted < numcores) {
			cvmx_coremask_t mask;

			cvmx_coremask_copy(&mask, &hip->coremask);
			cvmx_coremask_maskoff(&mask, &mask, &del_coremask);

			/* Library does not have last core number from mask */
			i = -1;
			cvmx_coremask_for_each_core(core, &mask) {
				if (core == cabr_ptr->cabr_init_core)
					continue;
				if (cvmx_coremask_is_core_set(&mask, core))
					i = core;
			}

			if (i == -1)
				break;

			cvmx_coremask_set_core(&del_coremask, i);
			cores_deleted++;
		}
	}

	/* Make double sure no one is trying to unplug the init_core */
	cvmx_coremask_clear_core(&del_coremask, cabr_ptr->cabr_init_core);

	cores_deleted = cvmx_coremask_get_core_count(&del_coremask);

	if (cores_deleted == 0) {
		fprintf(stderr, "ERROR: "
			"Can not unplug application primary core\n");
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown(0);
		return 0;
	} else if (!cvmx_coremask_is_empty(&params->coremask) &&
		cvmx_coremask_cmp(&del_coremask, &params->coremask) != 0) {
		cvmx_coremask_bmp2str(&del_coremask, hexstr);
		fprintf(stderr, "WARNING: "
			"requested coremask invalid, actual coremask=0x%s\n",
			hexstr);
	} else if (params->numcores > 0 && params->numcores != cores_deleted) {
		fprintf(stderr, "WARNING: "
			"requested core count is reduced to %u\n",
			cores_deleted);
	}

	oct_remote_read_coremask((uintptr_t)&hip_pa->hotplug_activated_coremask,
				 &hip->hotplug_activated_coremask);
	/* Spit out all relevant coremasks for debugging */
	cvmx_coremask_bmp2str(&hip->hotplug_activated_coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
		    "Hotplug activated cores 0x%s\n", hexstr);

	cvmx_coremask_bmp2str(&del_coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
		"Unplugging %d cores from app %u coremask=0x%s\n",
		cores_deleted, params->app_index, hexstr);

	cvmx_coremask_copy(&hip->unplug_cores, &del_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->unplug_cores,
				  &del_coremask);

	cvmx_coremask_clear_all(&hip->shutdown_cores);
	cvmx_coremask_clear_all(&hip->hplugged_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->shutdown_cores,
				  &empty_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->hplugged_cores,
				  &empty_coremask);

	/* Store actual coremask in params */
	cvmx_coremask_copy(&params->coremask, &del_coremask);

	/* Drain all output before poking the application */
	fflush(stderr);
	tcdrain(fileno(stderr));

	/* The actual operation is located in the executive library */
	ret = cvmx_app_hotplug_unplug_cores(
		params->app_index, &del_coremask, 0/*wait*/);
	oct_remote_read_coremask(
		(uintptr_t)&hip_pa->hotplug_activated_coremask,
		&hip->hotplug_activated_coremask);

	if (ret != 0) {
		fprintf(stderr, "ERROR: "
			"Failed to unplug cores in mask=0x%s\n", hexstr);
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown(0);
		return -1;
	}

	/* The above function does not implement a waiting loop, we do */
	i = 2 * 1000000;
	do {
		cvmx_coremask_t mask;
		unsigned t = 1000;

		oct_remote_read_coremask(
			(uintptr_t)&hip_pa->hotplug_activated_coremask,
			&hip->hotplug_activated_coremask);

		/* Wait for all unplugged cores to deactivate */
		cvmx_coremask_and(&mask, &del_coremask,
				  &hip->hotplug_activated_coremask);
		if (cvmx_coremask_is_empty(&mask))
			break;

		if (verbose) {
			fprintf(stderr, "INFO: "
				"Waiting for application to unplug\n");
			t = 250000;
		}
		usleep(t);
		i -= t;
	} while (i > 0);

	if (i <= 0) {
		fprintf(stderr, "ERROR: "
			"Timeout out waiting for cores to confirm request\n");
		/* Unlock the hotplugging mechanism */
		set_app_under_shutdown(0);
		return -1;
	}

	/* Wait for SE app outputs to drain */
	usleep(250000);

	oct_remote_read_coremask((uintptr_t)&hip_pa->hotplug_activated_coremask,
				 &hip->hotplug_activated_coremask);
	cvmx_coremask_bmp2str(&hip->hotplug_activated_coremask, hexstr);
	if (verbose > 2)
		fprintf(stderr, "DEBUG: "
			"Hotplug activated cores changed to 0x%s\n", hexstr);

	/* Make unplugged cores available again */
	oct_app_cores_free(params);

	/* Free all per-core memories */
	cvmx_coremask_for_each_core(core, &del_coremask) {
		cvmx_app_boot_record_t *cabr_core_ptr;

		if (verbose)
			fprintf(stderr, "INFO: "
				"Cleaning up after core %d\n", core);

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if (cabr_core_ptr == NULL) {
			fprintf(stderr, "ERROR: "
				"Failed to obtain boot record for core %d\n",
				core);
			continue;
		}

		/* The following call releases all per-core memories */
		ret = app_boot_record_free(cabr_core_ptr);
		if (ret < 0) {
			fprintf(stderr, "ERROR: "
				"Failed to clean-up after core %d\n",
				core);
		}
	} /* for each core */

	/* Update running coremask for the app */
	cvmx_coremask_maskoff(&hip->coremask, &hip->coremask, &del_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);

	/* Clear the unplugged coremask */
	cvmx_coremask_clear_all(&hip->unplug_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->unplug_cores,
				  &empty_coremask);

	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown(0);

	/* sync the shadow structure */
	read_hp_g_from_target();

	cvmx_coremask_bmp2str(&hip->coremask, hexstr);
	fprintf(stderr, "SUCCESS: App index %u now runs on cores 0x%s\n",
		params->app_index, hexstr);

	return 0;
}

/*
 * Shutdown a running app
 */
static int oct_app_kill(struct app_params *params)
{
	cvmx_app_hotplug_info_t *hip;
	cvmx_app_hotplug_info_t *hip_pa;
	cvmx_coremask_t del_coremask, empty_cm;
	cvmx_app_boot_record_t *cabr_ptr;
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	int core, ret, i;
	unsigned cores_deleted;
	int hot_pluggable;

	ret = oct_app_index_verify(params);
	if (ret < 0)
		return ret;

	hip_pa = get_hip_pa(params->app_index);

	/* Get primary boot record pointer for this app */
	cabr_ptr = app_boot_record_core_get(params, -1);
	if (cabr_ptr == NULL)
		return -1;

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

	if (is_app_under_boot_or_shutdown()) {
		fprintf(stderr, "ERROR: "
			"Another application is being booted or shutdown, "
			"Try again later.\n");
		return -1;
	}

	/* Lock the hotplugging mechanism */
	set_app_under_shutdown(params->app_index+1);

	/* Obtain pointer to app slot info block */
	read_hp_g_from_target();
	hip = &hotplug_global_ptr->hotplug_info_array[params->app_index];

	/* Shutdown all cores running this app */
	cvmx_coremask_copy(&del_coremask, &hip->coremask);
	cvmx_coremask_copy(&params->coremask, &del_coremask);
	cores_deleted = cvmx_coremask_get_core_count(&del_coremask);

	if (cores_deleted == 0) {
		fprintf(stderr, "ERROR: "
			"Can not shutdown requested cores\n");
		goto _oct_kill_fail;
	}

	cvmx_coremask_bmp2str(&hip->hotplug_activated_coremask, hexstr);
	if (verbose > 1)
		fprintf(stderr, "INFO: "
			"Hotplug activated cores 0x%s\n", hexstr);

	/* Spit out all relevant coremasks for debugging */
	cvmx_coremask_bmp2str(&del_coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: "
			"Shutting down %d cores from app %u coremask=0x%s\n",
			cores_deleted, params->app_index, hexstr);

	cvmx_coremask_copy(&hip->shutdown_cores, &del_coremask);
	cvmx_coremask_clear_all(&hip->unplug_cores);
	cvmx_coremask_clear_all(&hip->hplugged_cores);

	cvmx_coremask_clear_all(&empty_cm);
	oct_remote_write_coremask((uintptr_t)&hip_pa->shutdown_cores,
				  &del_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->unplug_cores,
				  &empty_cm);
	oct_remote_write_coremask((uintptr_t)&hip_pa->hplugged_cores,
				  &empty_cm);

	/* Drain all output before poking the application */
	fflush(stderr);
	tcdrain(fileno(stderr));

	/* The action code is in the executive library */
	ret = cvmx_app_hotplug_shutdown_request(&del_coremask, 0/*:wait: 0*/);

	if (ret != 0) {
		fprintf(stderr, "ERROR: "
			"Failed to shutdown cores in mask=0x%s\n", hexstr);
		goto _oct_kill_fail;
	}

	/* cvmx_app_hotplug_shutdown_complete() is implemented poorly,
	 * here is a cleaner way.
	 */
	i = 5 * 1000000;
	do {
		unsigned t = 1000;
		uint64_t shutdown_done;

		if (verbose) {
			fprintf(stderr, "INFO: Waiting for app to shutdown\n");
			t = 250000;
		}

		usleep(t);
		i -= t;

		/* Wait for all unplugged cores to deactivate */
		oct_remote_read_coremask(
			(uintptr_t)&hip_pa->hotplug_activated_coremask,
					 &hip->hotplug_activated_coremask);

		if (cvmx_coremask_is_empty(&hip->hotplug_activated_coremask))
			break;

		shutdown_done = octeon_remote_read_mem64(
					(uintptr_t)&hip_pa->shutdown_done);
		if (shutdown_done) {
			/* keep the shadow structure in sync */
			hip->shutdown_done = shutdown_done;
			break;
		}
	} while (i > 0);

	if (i <= 0) {
		fprintf(stderr, "ERROR: "
			"Timeout out waiting for cores to shutdown\n");
		goto _oct_kill_fail;
	}

	/* Wait for the SE app output to drain */
	usleep(250000);

	read_hp_g_from_target();
	cvmx_coremask_bmp2str(&hip->hotplug_activated_coremask, hexstr);
	if (verbose > 2)
		fprintf(stderr, "DEBUG: "
			"Hotplug activated cores changed to 0x%s, shutdown_done=%d\n",
			hexstr, (int) hip->shutdown_done);

	/* Make unplugged cores available again */
	oct_app_cores_free(params);

	/* Free all per-core memories */
	cvmx_coremask_for_each_core(core, &del_coremask) {
		cvmx_app_boot_record_t *cabr_core_ptr;

		if (verbose)
			fprintf(stderr, "INFO: "
				"Cleaning up after core %d\n", core);

		/* Get or Create per-core boot record */
		cabr_core_ptr = app_boot_record_core_get(params, core);
		if (cabr_core_ptr == NULL) {
			fprintf(stderr, "ERROR: "
				"Failed to obtain boot record for core %d\n",
				core);
			continue;
		}

		/* The following call releases all per-core memories */
		ret = app_boot_record_free(cabr_core_ptr);
		if (ret < 0) {
			fprintf(stderr, "ERROR: "
				"Failed to clean-up after core %d\n",
				core);
		}
	} /* for each core */

	/* Free the primary boot record */
	if (verbose)
		fprintf(stderr, "INFO: "
			"Cleaning up primary data\n");

	ret = app_boot_record_free(cabr_ptr);
	if (ret < 0)
		fprintf(stderr, "ERROR: "
			"Failed to clean-up primary boot record\n");

	/* Update running coremask for the app */
	cvmx_coremask_maskoff(&hip->coremask, &hip->coremask, &del_coremask);
	oct_remote_write_coremask((uintptr_t)&hip_pa->coremask, &hip->coremask);

	/* Clear the shutdown  coremask */
	cvmx_coremask_clear_all(&hip->shutdown_cores);
	oct_remote_write_coremask((uintptr_t)&hip_pa->shutdown_cores,
				  &empty_cm);
	hip->shutdown_done = 0;
	octeon_remote_write_mem64((uintptr_t)&hip_pa->shutdown_done, 0);

	/* zero out the entire application slot */
	memset(hip, 0, sizeof(*hip));
	octeon_remote_write_mem((uintptr_t)hip_pa, hip, sizeof(*hip));

	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown(0);

	fprintf(stderr, "SUCCESS: App index %u now down\n",
		params->app_index);

	return 0;

_oct_kill_fail:
	/* Unlock the hotplugging mechanism */
	set_app_under_shutdown(0);
	return -1;
}

/*
 * Reset Hotplug data (Allow to reset hotplug data without turn off the power)
 */
static int oct_app_reset(struct app_params *params)
{
	/* Reset master block */
	if (app_boot_master_create(1/*reset*/)) {
		fprintf(stderr, "ERROR: master boot record init failed\n");
		return -1;
	}
	/* Reset hotplug named block data */
	if (map_hotplug_info(1/*reset*/)) {
		fprintf(stderr, "ERROR: Unable to reset hotplug data\n");
		return -1;
	}
	return 0;
}



/*
 * Top-level function for the utility
 */
int main(int argc, char *argv[])
{
	char hexstr[CVMX_MIPS_MAX_CORES/4+1];
	struct app_params prm;
	int rc;

	rc = octeon_remote_open(OCTEON_REMOTE_DEFAULT, 0);
	if (rc)
		return rc;

	if (!octeon_remote_mem_access_ok()) {
		printf("ERROR: DRAM not setup, board needs to be booted\n");
		rc = -EPERM;
		goto main_err;
	}

#if 0
	fprintf(stderr, "%s\n" "Version: %s, Built: %s %s\n",
		"Copyright (c) 2010-2013 Cavium, Inc. All rights reserved.",
		"$Id: oct-app-main.c 118877 2015-06-03 18:30:06Z egoranov $",
		__DATE__, __TIME__);
#endif

	memset(&prm, 0, sizeof(prm));

	/* Default action */
	prm.command = APP_CMD_UNDEF;

	/* Fill-in default application parameters */
	prm.numcores = 1;
	prm.heap_size = 3 << 20;
	prm.stack_size = 1 << 20;
	prm.app_index = -1;

	progname = argv[0];

	/* See if command action is in program name */
	if (strstr(argv[0], "ctl")) {
		prm.command = APP_CMD_UNDEF;
		argv++; argc--;
	}

	if (argc < 1)
		return oct_app_main_help();

	/* Decode command action */
	if (strstr(argv[0], "info") || strstr(argv[0], "show"))
		prm.command = APP_CMD_INFO;
	else if (strstr(argv[0], "boot") || strstr(argv[0], "start"))
		prm.command = APP_CMD_BOOT;
	else if (strstr(argv[0], "unplug") || strstr(argv[0], "remove"))
		prm.command = APP_CMD_DEL;
	else if (strstr(argv[0], "del") || strstr(argv[0], "rm"))
		prm.command = APP_CMD_DEL;
	else if (strstr(argv[0], "add") || strstr(argv[0], "plug"))
		prm.command = APP_CMD_ADD;
	else if (strstr(argv[0], "halt") || strstr(argv[0], "kill") ||
		strstr(argv[0], "shut"))
		prm.command = APP_CMD_KILL;
	else if (strstr(argv[0], "hp_rst"))
		prm.command = APP_CMD_RESET;
	else
		return oct_app_main_help();

	/* First arg, action has been processed */
	argv++; argc--;

	/* Scan for generic options */
	if (oct_app_cmd_genopt(argc, argv, &prm) < 0)
		return oct_app_main_help();

	/* Parse the remainder of options depending on the action */
	if (prm.command == APP_CMD_BOOT) {
		if (oct_app_cmd_boot_getopt(argc, argv, &prm) < 0)
			return oct_app_boot_help();
	}

	if (prm.command == APP_CMD_ADD) {
		if (oct_app_cmd_add_getopt(argc, argv, &prm) < 0)
			return oct_app_add_help();
	}

	if (prm.command == APP_CMD_DEL) {
		if (oct_app_cmd_del_getopt(argc, argv, &prm) < 0)
			return oct_app_del_help();
	}

	if (prm.command == APP_CMD_KILL) {
		if (oct_app_cmd_kill_getopt(argc, argv, &prm) < 0)
			return oct_app_kill_help();
	}

	/* APP_CMD_INFO has no specific options */
	if (app_boot_master_create(0/*reset*/)) {
		fprintf(stderr, "ERROR: master boot record init failed\n");
		return -1;
	    }

	/* Setup hotplug named block and pointers */
	if (map_hotplug_info(0/*reset*/)) {
		fprintf(stderr, "ERROR: Unable to map hotplug info\n");
		return -1;
	}
	if (app_boot_master_create(0/*reset*/)) {
		fprintf(stderr, "ERROR: master boot record init failed\n");
		return -1;
	}


	cvmx_coremask_bmp2str(&prm.coremask, hexstr);
	if (verbose)
		fprintf(stderr, "INFO: action=%u coremask=0x%s count=%u\n",
			prm.command, hexstr, prm.numcores);
#if 1
/* this is an experimental code which wait for started SE app to register for
 * hotplugging. I general case one can start SE app which will not support
 * hotplugging and at this case ALWAYS (rc != hac)
 * However for 'dataplane_live_update' scenario the SE app will support hotplug
 */
	/* wait for 'boot' or 'shut' to complete && (rc == hac)
	 * however do not wait if this is 'info' command
	 */
	while (1 && prm.command != APP_CMD_INFO) {
		cvmx_coremask_t rc;	/* running_coremask */
		cvmx_coremask_t hac;	/* hotplug_activated_coremask */
		cvmx_app_hotplug_info_t *hip_pa;
		int i, ret1, ret2 = 0;

		ret1 = is_app_under_boot_or_shutdown();

		for (i = 0; i < CVMX_APP_HOTPLUG_MAX_APPS; i++) {
			hip_pa = get_hip_pa(i);

			oct_remote_read_coremask((uintptr_t)&hip_pa->coremask,
						 &rc);
			oct_remote_read_coremask(
				(uintptr_t)&hip_pa->hotplug_activated_coremask,
						 &hac);

			if (cvmx_coremask_is_empty(&rc))
				continue; /* skip empty slots */
			ret2 = cvmx_coremask_cmp(&rc, &hac);

			if (ret1 == 0 && ret2 == 0)
				break;
		}
		if (ret1 == 0 && ret2 == 0)
			break;
		if (cvmx_coremask_is_empty(&rc) && ret1 == 0)
			break;
	};
#endif
	/* update hp_g shadow structure (hotplug_activated_mask) */
	/*
	 * Perform the action requested
	 */
	if (prm.command == APP_CMD_INFO)
		return oct_app_info_show(&prm);

	if (prm.command == APP_CMD_BOOT)
		return  oct_app_boot(&prm);

	if (prm.command == APP_CMD_ADD)
		return oct_add_cores(&prm);

	if (prm.command == APP_CMD_DEL)
		return oct_del_cores(&prm);

	if (prm.command == APP_CMD_KILL)
		return oct_app_kill(&prm);

	if (prm.command == APP_CMD_RESET)
		return oct_app_reset(&prm);

	fprintf(stderr, "ERROR: "
		"Failed to understand command\n");

	/* XXX- Note: this program does not do self-cleaning,
	 * but depends on all process resources to be cleaned up by Linux
	 */
main_err:
	octeon_remote_close();
	return rc;

}

/* End of File */
