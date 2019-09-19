/***********************license start************************************
 * Copyright (c) 2003-2013 Cavium (support@cavium.com). All rights
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
 * $Id: cmd_octeon.c 129228 2015-12-04 15:51:32Z cchavva $
 *
 */

#include <common.h>
#include <command.h>
#include <exports.h>
#include <linux/ctype.h>
#include <net.h>
#include <elf.h>
#include <malloc.h>
#include <asm/mipsregs.h>
#include <asm/processor.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-coremask.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon-boot-info.h>
#include <asm/arch/octeon-feature.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/bootoct_cmd.h>

extern char uboot_start;
extern char tlbwi_and_eret;

extern boot_info_block_t boot_info_block_array[];

DECLARE_GLOBAL_DATA_PTR;

int forceboot = 0;
uint32_t boot_flags = 0;

/**
 * Allocate TLB entries for boot info array
 *
 * @param[in] pcm coremask to allocate for
 *
 * @return 0 for success, -1 if out of memory
 */
static int alloc_tlbs(const cvmx_coremask_t *pcm)
{
	int core;

	cvmx_coremask_for_each_core(core, pcm) {
		if (boot_info_block_array[core].tlb_entries != NULL)
			continue;

		debug("%s: Allocating %d TLB entries for core %d\n",
		      __func__, NUM_OCTEON_TLB_ENTRIES, core);
		boot_info_block_array[core].tlb_entries =
				calloc(sizeof(octeon_tlb_entry_t_bl),
				       NUM_OCTEON_TLB_ENTRIES);
		if (boot_info_block_array[core].tlb_entries == NULL) {
			printf("%s: Error: out of memory allocating %d TLB entries for core %d\n",
			       __func__, NUM_OCTEON_TLB_ENTRIES, core);
			return -1;
		}
	}
	return 0;
}

static int load_elf(uint32_t elf_addr, int argc, char *const argv[],
		    uint32_t stack_size, uint32_t heap_size,
		    cvmx_coremask_t *pcm, int app_index);

void setup_se_app_boot_info(cvmx_coremask_t *se_core_mask)
{
	cvmx_coremask_t fuse_coremask;
	cvmx_coremask_t app_coremask;
	linux_app_boot_info_t *labi;

	octeon_get_available_coremask(&fuse_coremask);
	cvmx_coremask_maskoff(&app_coremask, &fuse_coremask, se_core_mask);

	labi = (linux_app_boot_info_t *) (0x80000000 + LABI_ADDR_IN_BOOTLOADER);
	labi->labi_signature = LABI_SIGNATURE;
	if (labi->avail_coremask)
		labi->avail_coremask &= cvmx_coremask_get32(&app_coremask);
	else
		labi->avail_coremask = cvmx_coremask_get32(&app_coremask);
	labi->cur_exception_base = cur_exception_base;
	octeon_translate_gd_to_linux_app_global_data(&labi->gd);
}

/**
 * Parse comma separated numbers into an array
 *
 * @param[out] values values read for each node
 * @param[in] str string to parse
 * @param base 0 for auto, otherwise 8, 10 or 16 for the number base
 *
 * @return number of values read.
 */
static int octeon_parse_nodes(uint64_t values[CVMX_MAX_NODES],
			      const char *str, int base)
{
	int node = 0;
	char *sep;

	do {
		debug("Parsing node %d: \"%s\"\n", node, str);
		values[node] = simple_strtoull(str, &sep, base);
		debug("  node %d: 0x%llx\n", node, values[node]);
		str = sep + 1;
	} while (++node < CVMX_MAX_NODES && *sep == ',');

	debug("%s: returning %d\n", __func__, node);
	return node;
}

/**
 * Parse command line arguments
 *
 * @param argc			number of arguments
 * @param[in] argv		array of argument strings
 * @param cmd			command type
 * @param[out] boot_args	parsed values
 *
 * @return number of arguments parsed
 */
int octeon_parse_bootopts(int argc, char *const argv[],
			  enum octeon_boot_cmd_type cmd,
			  struct octeon_boot_args *boot_args)
{
	uint64_t node_values[CVMX_MAX_NODES];
	int arg, j;
	int num_values;
	int node;

	debug("%s(%d, %p, %d, %p)\n", __func__, argc, argv, cmd, boot_args);
	memset(boot_args, 0, sizeof(*boot_args));
	boot_args->stack_size = DEFAULT_STACK_SIZE;
	boot_args->heap_size = DEFAULT_HEAP_SIZE;
	boot_args->node_mask = 0;

	for (arg = 0; arg < argc; arg++) {
		debug("  argv[%d]: %s\n", arg, argv[arg]);
		if (cmd == BOOTOCT && !strncmp(argv[arg], "stack=", 6)) {
			boot_args->stack_size = simple_strtoul(argv[arg] + 6,
							       NULL, 0);
		} else if (cmd == BOOTOCT && !strncmp(argv[arg], "heap=", 5)) {
			boot_args->heap_size = simple_strtoul(argv[arg] + 5,
							      NULL, 0);
		} else if (!strncmp(argv[arg], "debug", 5)) {
			puts("setting debug flag!\n");
			boot_args->boot_flags |= OCTEON_BL_FLAG_DEBUG;
		} else if (cmd == BOOTOCT && !strncmp(argv[arg], "break", 5)) {
			puts("setting break flag!\n");
			boot_args->boot_flags |= OCTEON_BL_FLAG_BREAK;
		} else if (!strncmp(argv[arg], "forceboot", 9)) {
			boot_args->forceboot = true;
		} else if (!strncmp(argv[arg], "nodemask=", 9)) {
			boot_args->node_mask = simple_strtoul(argv[arg] + 9,
							      NULL, 16);
		} else if (!strncmp(argv[arg], "numcores=", 9)) {
			memset(node_values, 0, sizeof(node_values));
			num_values = octeon_parse_nodes(node_values,
							argv[arg] + 9, 0);
			for (j = 0; j < num_values; j++)
				boot_args->num_cores[j] = node_values[j];
			boot_args->num_cores_set = true;
		} else if (!strncmp(argv[arg], "skipcores=", 10)) {
			memset(node_values, 0, sizeof(node_values));
			num_values = octeon_parse_nodes(node_values,
							argv[arg] + 10, 0);
			for (j = 0; j < num_values; j++)
				boot_args->num_skipped[j] = node_values[j];
			boot_args->num_skipped_set = true;
		} else if (!strncmp(argv[arg], "coremask=", 9)) {
			memset(node_values, 0, sizeof(node_values));
			num_values = octeon_parse_nodes(node_values,
							argv[arg] + 9, 16);
			for (j = 0; j < num_values; j++)
				cvmx_coremask_set64_node(&boot_args->coremask,
							 j, node_values[j]);
			boot_args->coremask_set = true;
		} else if (cmd == BOOTOCTLINUX &&
			   !strncmp(argv[arg], "namedblock=", 11)) {
			boot_args->named_block = argv[arg] + 11;
		} else if (!strncmp(argv[arg], "endbootargs", 11)) {
			boot_args->endbootargs = 1;
			arg++;
			if (argc >= arg && cmd != BOOTOCTLINUX)
				boot_args->app_name = argv[arg];
			break;
		} else {
			debug(" Unknown argument \"%s\"\n", argv[arg]);
		}
	}

	if (boot_args->coremask_set && boot_args->num_cores_set) {
		puts("Warning: both coremask and numcores are set, using coremask.\n");
	} else if (!boot_args->coremask_set && !boot_args->num_cores_set) {
#ifdef CONFIG_OCTEON_SIM_SW_DIFF
		if (!boot_args->node_mask)
			boot_args->node_mask = gd->arch.node_mask;
		cvmx_coremask_for_each_node(node, boot_args->node_mask) {
			uint64_t mask;
			mask = cvmx_read_csr_node(node, CVMX_CIU_FUSE);
			cvmx_coremask_set64_node(&boot_args->coremask, node,
						 mask);
		}
#else
		cvmx_coremask_set_core(&boot_args->coremask, 0);
#endif
		boot_args->coremask_set = true;
	} else if ((!boot_args->coremask_set) && boot_args->num_cores_set) {
		cvmx_coremask_for_each_node(node, gd->arch.node_mask)
			cvmx_coremask_set64_node(&boot_args->coremask, node,
				((1ull << boot_args->num_cores[node]) - 1) <<
					boot_args->num_skipped[node]);
		boot_args->coremask_set = true;
	}
	/* Update the node mask based on the coremask or the number of cores */
	for (j = 0; j < CVMX_MAX_NODES; j++) {
		if (cvmx_coremask_get64_node(&boot_args->coremask, j))
			boot_args->node_mask |= 1 << j;
	}
	debug("%s: return %d\n", __func__, arg);
	return arg;
}


/**
 * Break (stop) core
 */
void break_core_start_app(void)
{
		asm volatile ("break\n");
}


/* ======================================================================
 * Interpreter command to boot an Octeon simple exec ELF image from memory.
 * ====================================================================== */
int do_bootocteon(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	struct octeon_boot_args boot_args;
	unsigned long addr = 0;	/* Address of the ELF image     */
	cvmx_coremask_t core_mask;
	int i;
	int nodemask = gd->arch.node_mask;
	int rcode;
	int arg_start = 1;
#if defined(DEBUG)
	int node;
#endif

	/* Use coremask_override to restrict cores used on bringup boards
	 * This value is anded with coremask to determine allowed cores
	 */

	/* -------------------------------------------------- */
	if (argc >= 2) {
		/* See if first argument is a hex address based on first two
		 * digits.
		 */
		if (isxdigit(argv[1][0]) &&
		    (isxdigit(argv[1][1]) || argv[1][1] == 'x' ||
		     argv[1][1] == 'X' || argv[1][1] == '\0')) {
			addr = simple_strtoul(argv[1], NULL, 16);
			arg_start++;
		}
	}
	if (!addr)
		addr = load_addr;

	/* Last arg seems to get a \n on it, so clean it up */
	if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '\n')
		argv[argc - 1][strlen(argv[argc - 1]) - 1] = '\0';

	i = octeon_parse_bootopts(argc - arg_start, argv + arg_start,
				  BOOTOCT, &boot_args);
	if (boot_args.endbootargs) {
		argv += (i + arg_start);
		argc -= (i + arg_start);
	}
#ifdef DEBUG
	debug("%s:\n"
	      " app name: %s\n"
	      " node mask: 0x%x\n"
	      " stack size: 0x%x\n"
	      " heap size: 0x%x\n"
	      " boot flags: 0x%x\n"
	      " force boot: %s\n"
	      " coremask set: %s\n"
	      " num cores set: %s\n"
	      " num skipped set: %s\n",
	      __func__, boot_args.app_name ? boot_args.app_name : "none",
	      boot_args.node_mask,
	      boot_args.stack_size,
	      boot_args.heap_size,
	      boot_args.boot_flags,
	      boot_args.forceboot ? "true" : "false",
	      boot_args.coremask_set ? "true" : "false",
	      boot_args.num_cores_set ? "true" : "false",
	      boot_args.num_skipped_set ? "true" : "false");
	debug(" num cores: ");
	for (node = 0; node < CVMX_MAX_NODES; node++)
		debug("%s%d", node > 0 ? ", " : "", boot_args.num_cores[node]);
	debug("\n num skipped: ");
	for (node = 0; node < CVMX_MAX_NODES; node++)
		debug("%s%d", node > 0 ? ", " : "", boot_args.num_cores[node]);
	debug("\n coremask:\n");
	cvmx_coremask_print(&boot_args.coremask);
	debug(" argc: %d, argv[0]: %p(%p)\n", argc, argv, *argv);
#endif

	boot_flags |= boot_args.boot_flags;

	/* Remove cores from coremask based on environment variable stored in
	 * flash
	 */
	if (boot_args.coremask_set) {
		if (cvmx_coremask_is_empty(&boot_args.coremask)) {
			puts("Coremask is empty after coremask_override mask.  Nothing to do.\n");
			return 0;
		}
		cvmx_coremask_copy(&core_mask, &boot_args.coremask);
	} else {
		cvmx_coremask_clear_all(&core_mask);
		cvmx_coremask_set_core(&core_mask, 0);
	}

	if (validate_coremask(&core_mask)) {
		puts("Invalid coremask.\n");
		return 1;
	}

	/* Check environment for forceboot flag */
	if (getenv("forceboot"))
		boot_args.forceboot |= 1;

	forceboot |= boot_args.forceboot;

	debug("remove cores which are not on nodes specified with nodemask: 0x%x, new_pcm:\n",
	      nodemask);
	cvmx_coremask_dprint(&core_mask);

	if (cvmx_coremask_intersects(&core_mask, &coremask_to_run) != 0) {
		puts("ERROR: Can't load code on core twice! (provided "
		     "coremask:\n");
		cvmx_coremask_print(&core_mask);
		puts("overlaps with previously loaded coremask:\n");
		cvmx_coremask_print(&coremask_to_run);
		return -1;
	}

	debug("Bootloader: Booting Octeon Executive application at 0x%08lx, "
	      "stack size: 0x%x, heap size: 0x%x, nodemask=0x%x core mask:\n",
	      addr, boot_args.stack_size, boot_args.heap_size, nodemask);

	cvmx_coremask_dprint(&core_mask);

	rcode = load_elf(addr, argc, argv,
			 boot_args.stack_size, boot_args.heap_size, &core_mask, -1);

#if CONFIG_OCTEON_SIM_HW_DIFF
	if (rcode < 0) {
		/* Hack to get simulator to exit on error */
		/* If we have an error loading the ELF file, we want to abort
		 * the simulation
		 */
		printf("Aborting simulation - ignore errors below this....\n");
		if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
			cvmx_coremask_t coremask;
			/* get all-available-cores coremask */
			octeon_get_available_coremask(&coremask);
			cvmx_coremask_clear_self(&coremask); /* exclude itself */
			/* set all cores but me to stop by executing 'break' */
			octeon_setup_boot_vector((uint32_t) &break_core_start_app, &coremask);
			start_cores(&coremask);
		} else {
			cvmx_write_csr(CVMX_CIU_PP_RST, 0);
		}
		OCTEON_SYNC;
		asm volatile ("break\n");	/* ... and finaly stop itself */
	}
#endif
	if (rcode < 0)
		puts("ERROR: Unable to properly load application\n");

	return rcode;
}


/* ====================================================================== */
U_BOOT_CMD(bootoct, 128, 0, do_bootocteon,
	   "Boot from an Octeon Executive ELF image in memory",
	   " [elf_address [stack=stack_size] [heap=heap_size] [coremask=mask_to_run | numcores=core_cnt_to_run] [forceboot] [debug] [break] [endbootargs] [app name] [app_args ...] .\n"
	   "elf_address - address of ELF image to load. defaults to $(loadaddr). If 0, default load address used.\n"
	   "stack       - size of stack in bytes.  Defaults to 1 megabyte\n"
	   "heap        - size of heap in bytes.  Defaults to 3 megabytes\n"
	   "coremask    - mask of cores to run on.  Anded with coremask_override environment\n"
	   "              variable to ensure only working cores are used\n"
	   "numcores    - number of cores to run on.  Runs on specified number of cores, taking into\n"
	   "              account the coremask_override.\n"
	   "skipcores   - only meaningful with numcores.  Skips this many cores (starting from 0) when\n"
	   "              loading the numcores cores. For example, setting skipcores to 1 will skip core 0\n"
	   "              and load the application starting at the next available core.\n"
	   "debug       - if present, bootloader passes debug flag to application which will cause\n"
	   "              the application to stop at a breakpoint on startup\n"
	   "break       - if present, exit program when control-c is received on the console\n"
	   "forceboot   - if set, boots application even if core 0 is not in mask\n"
	   "endbootargs - if set, bootloader does not process any further arguments and only passes\n"
	   "              the arguments that follow to the application.  If not set, the application\n"
	   "              receives all command line as arguments.\n"
	   "app name    - U-boot has no way of knowing the name of the application so this must be the first argument.\n"
	   "\n");

struct elf64headertype {
	char ident[16];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint64_t entry;
	uint64_t phoff;
	uint64_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
};
struct elf64programheadertype {
	uint32_t type;
	uint32_t flags;
	uint64_t offset;
	uint64_t vaddr;
	uint64_t paddr;
	uint64_t filesz;
	uint64_t memsz;
	uint64_t align;
};
struct elf64sectionheadertype {
	uint32_t name;
	uint32_t type;
	uint64_t flags;
	uint64_t addr;
	uint64_t offset;
	uint64_t size;
	uint32_t link;
	uint32_t info;
	uint64_t addralign;
	uint64_t entsize;
};

struct elf32headertype {
	char ident[16];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
};
struct elf32programheadertype {
	uint32_t type;
	uint32_t offset;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t filesz;
	uint32_t memsz;
	uint32_t flags;
	uint32_t align;
};
struct elf32sectionheadertype {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
};

typedef struct {
	uint32_t mem_addr;
	uint32_t mem_size;
	uint32_t file_offset;
	uint32_t file_size;
	uint32_t flags;
	uint32_t type;

} segment_desc_t;

typedef struct mem_list_entry {
	uint64_t start_offset;
	uint64_t size;
	struct mem_list_entry *next;
} mem_list_entry_t;

mem_list_entry_t *alloc_mem_list_entry(void)
{
	mem_list_entry_t *tmp;
	tmp = malloc(sizeof(mem_list_entry_t));
	tmp->start_offset = 0;
	tmp->size = 0;
	tmp->next = NULL;

	return (tmp);
}

void print_oct_tlb_entry(octeon_tlb_entry_t_bl * te)
{
	printf("p1=%16llx p2=%16llx  v=%16llx  sz=%8x sh=%d \n", te->phy_page0,
	       te->phy_page1, te->virt_base, te->page_size, te->flags.seg_type);
}

void print_boot_block(boot_info_block_t * i)
{
	printf("e=%16llx b=%16llx s=%16llx ex=%8x cvmx=%16llx flags=%16llx\n",
	       i->entry_point, i->boot_desc_addr, i->stack_top,
	       i->exception_base, i->cvmx_desc_addr, i->flags);
}

void print_boot_array(const cvmx_coremask_t *pcm)
{
	int core, i;
	cvmx_coremask_for_each_core(core, pcm) {
		printf("c=%02d ", core);
		print_boot_block(&boot_info_block_array[core]);
	}
	cvmx_coremask_for_each_core(core, pcm) {
		if (!boot_info_block_array[core].tlb_entries) {
			printf("c=%02d [tlb table empty] ", core);
			continue;
		}
		for (i = 0; i < NUM_OCTEON_TLB_ENTRIES; i++) {
			if (boot_info_block_array[core].tlb_entries[i].virt_base != 0) {
				printf("c=%02d %02d ", core, i);
				print_oct_tlb_entry(&boot_info_block_array[core].tlb_entries[i]);
			}
		}
	}
}

void print_tlb_array(octeon_tlb_entry_t_bl * tlb_array, int num_entries)
{
	int i;

	printf("TLB addr: %p\n", tlb_array);
	for (i = 0; i < num_entries; i++) {
		int shared = tlb_array[i].flags.seg_type;;
		if (!tlb_array[i].page_size)
			break;
		printf("TLB[%02d] page0: 0x%010llx", i, tlb_array[i].phy_page0);
		printf(" page1: 0x%010llx", tlb_array[i].phy_page1);
		printf(" pagesize:      0x%08lx (%010d)",
		       TLB_PAGE_SIZE_FROM_EXP(tlb_array[i].page_size),
		       tlb_array[i].page_size);
		printf(" virt : 0x%llx - 0x%llx seg_type=%d \n",
		       tlb_array[i].virt_base,
		       tlb_array[i].virt_base +
		       (1ULL << (tlb_array[i].page_size + 1)), shared);
#if 0
		printf("TLB[%02d] flags   : 0x%x\n", i, tlb_array[i].flags);
#endif
	}

}

/**
 * Computes the required TLB mapping size for a given region.  Now limits
 * mappings to a max size, and returns the max size if the request is larger
 * than that.
 *
 * @param vaddr_start
 *                  start virtual address
 * @param vaddr_end end virtual address
 * @param num_tlb_entries
 *                  number of TLB entries to use.  MUST BE 1.
 * @param alignment_exp
 *                  variable to put page (NOT TLB entry/VADDR) alignment
 *                  exponent in
 *
 * @return Mapping size in bytes on success
 *         0 on failures
 */
uint64_t get_tlb_mapping_size(uint64_t vaddr_start, uint64_t vaddr_end,
			      uint32_t num_tlb_entries,
			      uint32_t * alignment_exp)
{

	uint32_t tlb_page_size_exp;
	uint32_t tlb_page_size;

	/* assume only one TLB entry per range for now.... */
	if (num_tlb_entries != 1 || !alignment_exp)
		return 0;

	/* cycle through all page sizes, from smallest to largest allowed.
	 * We limit ourselves to 16 MByte pages (32 Mbytes per tlb entry.
	 */
	for (tlb_page_size_exp = 12; tlb_page_size_exp < 24;
	     tlb_page_size_exp += 2) {
		tlb_page_size = 0x1UL << tlb_page_size_exp;
		if (2 * tlb_page_size +
		    ALIGN_ADDR_DOWN(vaddr_start,
				    ALIGN_MASK(tlb_page_size_exp)) > vaddr_end) {
			*alignment_exp = tlb_page_size_exp;
			return 2 * tlb_page_size;
		}
		/* we align the start/end addresses to page alignments, and
		 * then see if the range is smaller that two pages.
		 */
	}

	/* Return max size if won't fit in one  */
	tlb_page_size = 0x1UL << tlb_page_size_exp;
	*alignment_exp = tlb_page_size_exp;
	return 2 * tlb_page_size;
}

/* Returns 0 if range is not already mapped, !0 if mapped */
static int is_range_tlb_mapped(octeon_tlb_entry_t_bl * tlb_array,
			       uint64_t vaddr_low, uint64_t vaddr_high)
{

	int tlb_index = 0;

	debug("checking range: 0x%llx - 0x%llx\n", vaddr_low, vaddr_high);
	for (tlb_index = 0; tlb_index < get_num_tlb_entries(); tlb_index++) {
		if (tlb_array[tlb_index].page_size != 0) {
			uint64_t entry_base = tlb_array[tlb_index].virt_base;
			uint64_t entry_top = entry_base +
			    (1ull << (tlb_array[tlb_index].page_size + 1)) - 1;

			debug("checking entry %d: 0x%llx - 0x%llx\n",
			      tlb_index, entry_base, entry_top);
			if (vaddr_low >= entry_base && vaddr_low <= entry_top)
				return 1;
			if (vaddr_high >= entry_base && vaddr_high <= entry_top)
				return 1;
			/* New region does not start or end within block, see
			 * if it contains the block.
			 */
			if (vaddr_low <= entry_base && vaddr_high >= entry_top)
				return 1;
		}
	}
	debug("Range check passed!\n");

	return 0;

}

/* Add a single tlb entry, and allocate memory for it. */
static int add_tlb_mapping_single(uint8_t node,
				  octeon_tlb_entry_t_bl * tlb_array,
				  uint64_t vaddr_low, uint64_t vaddr_high,
				  tlb_flags_t tlb_flags)
{
	int64_t page0, page1;
	uint32_t alignment_exp;
	uint64_t map_size;
	int tlb_index = 0;
	uint64_t vbase;
	uint64_t pbase = (uint64_t)node << CVMX_NODE_MEM_SHIFT;
	uint64_t pmax = ((uint64_t)(node + 1) << CVMX_NODE_MEM_SHIFT) - 1;

	int use_hi_mem_for_app = 0;
	/* For non-63XX chips, if env variable set, use the high 256 meg block
	 * for TLB mapped program code/data.  This leaves more low memory
	 * available for the application.  This may be useful for some
	 * 32 bit applications since the high 256 Meg block is not 32 bit
	 * addressable.
	 */
	if (OCTEON_IS_OCTEON1PLUS()
	    && getenv("octeon_load_app_hi"))
		use_hi_mem_for_app = 1;

	map_size = get_tlb_mapping_size(vaddr_low, vaddr_high,
					1, &alignment_exp);
	if (!map_size)
		return -1;

	debug("add_tlb_mapping_single: vaddr: 0x%llx - 0x%llx, "
	      "mapping size: 0x%llx, alignment exp: %d, align_mask: 0x%llx\n",
	      vaddr_low, vaddr_high, map_size, alignment_exp,
	      ALIGN_MASK(alignment_exp));
	vbase = vaddr_low & ALIGN_MASK(alignment_exp + 1);

	if (is_range_tlb_mapped(tlb_array, vbase, vbase + map_size - 1)) {
		printf("ERROR: TLB overlap - out of virtual memory in "
		       "0x10000000 - 0x20000000 range\n");
		return -1;
	}
	/* Try to allocate from high 256 meg block first if requested */
	if (use_hi_mem_for_app)
		page0 = cvmx_bootmem_phy_alloc(map_size >> 1,
					       0x410000000ull,
					       0x41FFFFFFFull,
					       1 << alignment_exp,
					       0);
	else
		page0 = -1;

	if (page0 < 0) {
		page0 = cvmx_bootmem_phy_alloc(map_size >> 1, pbase, pmax,
					       1 << alignment_exp, 0);
		if (page0 < 0)
			page0 = cvmx_bootmem_phy_alloc(map_size >> 1, 0, 0,
						       1 << alignment_exp, 0);
	}

	/* Check to see if we can use just a single page.  If so, we will map
	 * one page twice.
	 */
	if (vbase + (map_size >> 1) > vaddr_high) {
		/* A half mapping will work, so re-use page 0.  The app should
		 * never use the upper half of the virt. range, and it if does
		 * it will only mess itself up.
		 */
		page1 = page0;
		debug("Using a half mapping!\n");

	} else {
		/* Map a real page 1 */
		if (use_hi_mem_for_app)
			page1 = cvmx_bootmem_phy_alloc(map_size >> 1,
						       0x410000000ull,
						       0x41FFFFFFFull,
						       1 << alignment_exp, 0);
		else
			page1 = -1;
		if (page1 < 0)

			page1 = cvmx_bootmem_phy_alloc(map_size >> 1,
						       pbase, pmax,
						       1 << alignment_exp, 0);
		if (page1 < 0)
			page1 = cvmx_bootmem_phy_alloc(map_size >> 1, 0, 0,
						       1 << alignment_exp, 0);
	}

	if (page0 < 0 || page1 < 0) {
		printf("Bootloader error: out of memory.\n");
		return -1;
	}

	debug("add_tlb_mapping: phy blocks used: 0x%llx, 0x%llx\n", page0,
	      page1);

	/* Find first free TLB entry */

	while (tlb_array[tlb_index].page_size != 0
	       && tlb_index < get_num_tlb_entries())
		tlb_index++;

	if (tlb_index == get_num_tlb_entries())
		return -1;

	/* Only support 1 TLB entry/mapping currently */
	/* page size used as valid indicator */
	tlb_array[tlb_index].page_size = alignment_exp;
	tlb_array[tlb_index].phy_page0 = page0;
	tlb_array[tlb_index].phy_page1 = page1;
	tlb_array[tlb_index].virt_base = vbase;
	tlb_array[tlb_index].flags = tlb_flags;

	return tlb_index;
}

/* Add a tlb mapping for a specified region.  Uses TLB mapping sizes up to
 * 32 MBytes (16 MByte pages) - uses multiple TLB entries for large regions
 */
int add_tlb_mapping(uint8_t node,
		    octeon_tlb_entry_t_bl * tlb_array, uint64_t vaddr_low,
		    uint64_t vaddr_high, int num_entries,
		    tlb_flags_t tlb_flags)
{
	uint32_t alignment_exp;
	uint64_t map_size;
	uint64_t vbase;
	int tlb_index = -1;

	debug("%s(%u, %p, 0x%llx, 0x%llx, %d, [0x%x, 0x%x, 0x%x, 0x%x])\n", __func__,
	      node, tlb_array, vaddr_low, vaddr_high, num_entries, tlb_flags.ri,
	      tlb_flags.xi, tlb_flags.cca, tlb_flags.seg_type);
	if (!tlb_array) {
		printf("%s: Error: TLB array is NULL\n", __func__);
		return -1;
	}
	map_size = get_tlb_mapping_size(vaddr_low, vaddr_high, 1,
					&alignment_exp);
	if (!map_size) {
		debug("Can't get mapping size!\n");
		return -1;
	}
	debug("add_tlb_mapping: vaddr: 0x%llx - 0x%llx, mapping size: 0x%llx, "
	      "alignment exp: %d, align_mask: 0x%llx, num entries: %d\n",
	      vaddr_low, vaddr_high, map_size, alignment_exp,
	      ALIGN_MASK(alignment_exp), num_entries);

	/* Align low address to match TLB size selected. */
	vbase = vaddr_low & ALIGN_MASK(alignment_exp + 1);

	debug("vbase: 0x%llx\n", vbase);

	while (num_entries > 0 && vbase < vaddr_high) {
		uint64_t end_addr = vbase + map_size - 1;

		/* Map less then entire mapping if we don't have to */
		if (vaddr_high < vbase + map_size - 1)
			end_addr = vaddr_high;
		tlb_index = add_tlb_mapping_single(node, tlb_array,
						   vbase, end_addr,
						   tlb_flags);
		if (tlb_index < 0) {
			debug("Error adding TLB entry.\n");
			return -1;
		}
		vbase += map_size;
		num_entries--;

		/* Figure out mapping size for next segment, as not all
		 * segments need to be 32 MByte
		 */
		map_size = get_tlb_mapping_size(vbase, vaddr_high,
						1, &alignment_exp);
		if (!map_size) {
			debug("Can't get mapping size (loop)!\n");
			return -1;
		}
		if (vbase != (vbase & ALIGN_MASK(alignment_exp + 1))) {
			debug("ERROR creating 2nd+ TLB mappings\n");
			return -1;
		}
	}

	if (num_entries < 0 || vaddr_high > vbase) {
		debug("Error generating TLB mappings: entries: %d, vlow: 0x%llx, vhigh: 0x%llx\n",
		      num_entries, vaddr_low, vaddr_high);
		/* We did not complete successfully */
		return -1;
	}

	return tlb_index;
}

/**
 * Finds an empty tlb entry in the destination TLB descriptor array and copies
 * the source TLB entry into it.
 *
 * @param[out] dest_tlb_array	array of TLB entries
 * @param[in] src_entry_ptr	pointer to entry to add
 *
 * @return index where entry was added or 0xFFFFFFFF if the array is full.
 */
uint32_t copy_tlb_entry(octeon_tlb_entry_t_bl * dest_tlb_array,
			const octeon_tlb_entry_t_bl * src_entry_ptr)
{
	int32_t i;
	for (i = 0; i < get_num_tlb_entries(); i++) {
		if (!dest_tlb_array[i].page_size)
			break;
	}
	if (i == get_num_tlb_entries())
		return ~0UL;

	memcpy(&dest_tlb_array[i], src_entry_ptr,
	       sizeof(octeon_tlb_entry_t_bl));

	return i;
}

/**
 * Fill in a 64-bit virtual address range with a value similar to memset
 *
 * @param[in] tlb_array		array of tlb descriptors
 * @param dest_virt		destination virtual address
 * @param val			value to fill in
 * @param len			number of bytes to fill in
 *
 * @return number of bytes written or 0 if error
 */
uint32_t mem_set_tlb(octeon_tlb_entry_t_bl * const tlb_array,
		     uint64_t dest_virt,
		     uint8_t val, uint32_t len)
{
	uint32_t cur_len;
	uint64_t dest_phys;
	uint32_t chunk_len;
	int tlb_index;
	uint32_t page_size;
	octeon_tlb_entry_t_bl *tlb_entry;

	cur_len = len;

	for (tlb_index = 0;
	     tlb_index < get_num_tlb_entries() && cur_len > 0;
	     tlb_index++) {
		page_size = TLB_PAGE_SIZE_FROM_EXP(tlb_array[tlb_index].page_size);

		tlb_entry = &tlb_array[tlb_index];

		/* We found a page0 mapping that contains the start of the block */
		if (dest_virt >= tlb_entry->virt_base &&
		    dest_virt < (tlb_entry->virt_base + page_size)) {
			dest_phys =
			    tlb_entry->phy_page0 + (dest_virt -
						    tlb_entry->virt_base);
			chunk_len =
			    MIN((tlb_entry->virt_base + page_size) - dest_virt,
				cur_len);

			debug("Zeroing 0x%x bytes at 0x%llx (phys), 0x%llx virt\n",
			      chunk_len, dest_phys, dest_virt);
			memset64(dest_phys, val, chunk_len);

			cur_len -= chunk_len;
			dest_virt += chunk_len;
		}
		/* We found a page1 mapping that contains the start of the block */
		if (cur_len > 0
		    && dest_virt >= (tlb_entry->virt_base + page_size)
		    && dest_virt < (tlb_entry->virt_base + 2 * page_size)) {
			dest_phys =
			    tlb_entry->phy_page1 + (dest_virt -
						    (tlb_entry->virt_base +
						     page_size));
			chunk_len =
			    MIN((tlb_entry->virt_base +
				 2 * page_size) - dest_virt, cur_len);

			debug("Zeroing 0x%x bytes at 0x%llx (phys), 0x%llx virt\n",
			      chunk_len, dest_phys, dest_virt);
			memset64(dest_phys, val, chunk_len);

			cur_len -= chunk_len;
			dest_virt += chunk_len;
		}
	}
	if (cur_len != 0) {
		printf("ERROR clearing memory using TLB mappings!\n");
		return 0;
	} else {
		return len;
	}
}

/* Force this function not to be inlined as the local copies
   are read from the inline-asm but are not correctly described
   there. */
static void octeon_setup_crt0_tlb(uint32_t stack_addr_ptr,
				  uint32_t entry_addr_ptr,
				  uint32_t desc_vaddr_ptr) noinline;

static void octeon_setup_crt0_tlb(uint32_t stack_addr_ptr,
				  uint32_t entry_addr_ptr,
				  uint32_t desc_vaddr_ptr)
{
	uint64_t unmapped_ram_addr;

	/* Look this up before we set TLB registers, as uboot_tlb_ptr_to_phys
	 * does a TLB lookup and disturbs the registers we care about
	 */
	unmapped_ram_addr = MAKE_XKPHYS(uboot_tlb_ptr_to_phys(&tlbwi_and_eret));

	/* Set up TLB registers to clear desired entry.  The actual
	 * tlbwi instruction is done in ASM when running from unmapped DRAM
	 */
	write_64bit_c0_entrylo0(0);
	write_c0_pagemask(0);
	write_64bit_c0_entrylo1(0);
	write_64bit_c0_entryhi(0xFFFFFFFF91000000ull);
	write_c0_index(get_num_tlb_entries() - 1);

	/* We need to execute the final instructions that clears the final TLB
	 * entry from unmapped DRAM.  Here we figure out what address we need
	 * to jump to do do that.
	 */

	if (OCTEON_IS_OCTEON3()) {
		/* Clear extra performance counters */
		asm volatile (
			      "    .set push                   \n"
			      "    .set mips64                 \n"
			      "    dmtc0    $0, $25, 5         \n"
			      "    dmtc0    $0, $25, 7         \n"
			      "    .set pop                    \n"
		);
	}
	asm volatile (
		      /* Use $at as temp register */
		      "       .set push                        \n"
		      "       .set mips64                      \n"
		      "       .set noreorder                   \n"
		      "       .set noat                        \n"
		      /* Set stack address */
		      "       ld       $sp, 0(%[s_addr])       \n"
		      "       ld       %[e_addr],  0(%[e_addr])\n"
		      /* Clear performance counters */
		      "       dmtc0    $0, $25, 1              \n"
		      "       dmtc0    $0, $25, 3              \n"
		      /* resume address after error processing */
		      "       dmtc0    %[e_addr], $30, 0       \n"
		      /* Error EPC handler address */
		      "       dmtc0    %[e_addr], $14, 0       \n"
		      /* Enable unaligned instructions */
		      "       dmfc0    $at, $9,7               \n"
		      "       ori      $at, 0x1000             \n"
		      "       dmtc0    $at, $9,7               \n"
		      "       j        %[ram_addr]             \n"
		      /* Pass descriptor address in r16 */
		      "        ld       $s0, 0(%[d_addr])      \n"
		      "       .set pop              \n"
		      ::[s_addr] "r"(stack_addr_ptr),
		      [e_addr] "r"(entry_addr_ptr),
		      [d_addr] "r"(desc_vaddr_ptr),
		      [ram_addr] "r"(unmapped_ram_addr));
    __builtin_unreachable ();
}

#define OCTEON_TLBWI                       asm volatile ("tlbwi" : : )
#define OCTEON_TLBR                        asm volatile ("tlbr" : : )
#define OCTEON_EHB                         asm volatile ("ehb" : : )
#define OCTEON_NOP                         asm volatile ("nop" : : )

#define TLB_DIRTY   (0x1ULL<<2)
#define TLB_VALID   (0x1ULL<<1)
#define TLB_GLOBAL  (0x1ULL<<0)

void bl_write_tlb(uint64_t hi, uint64_t lo0, uint64_t lo1, uint64_t mask,
		  uint64_t index)
{
#if 0
#ifdef DEBUG
	printf("Writing TLB entry:\n index: 0x%llx\n hi: 0x%llx\n lo0: 0x%llx\n"
	       " lo1: 0x%llx\n mask: 0x%llx\n",
	       index, hi, lo0, lo1, mask);
#endif
#endif

	write_64bit_c0_entryhi(hi);
	write_64bit_c0_entrylo0(lo0);
	write_64bit_c0_entrylo1(lo1);
	write_c0_pagemask(mask);
	write_c0_index(index);
	write_c0_wired(index + 1);
	OCTEON_EHB;
	OCTEON_TLBWI;
	OCTEON_EHB;
}

/**
 * Processes boot descriptor block, and then starts application.
 * Stack pointer must be valid on entering this function,
 * stack pointer is set to address defined in boot descriptor before app is
 * started.
 * This is run on all cores.
 */
static void _start_app(int do_sync)
{
	int entry;
	int core_num;
	uint64_t val;
	octeon_tlb_entry_t_bl *tlb_entries;
	boot_info_block_t *boot_info_ptr;
	boot_init_vector_t boot_vect;
	volatile uint64_t stack_top;
	volatile uint64_t entry_point;
	volatile uint64_t desc_vaddr;
	int num_tlb_entries;
	boot_init_vector_t *boot_vect_ptr;
	static cvmx_spinlock_t lock = { .value = OCTEON_SPINLOCK_UNLOCKED_VAL };

	core_num = get_core_num();
#ifdef DEBUG
	cvmx_spinlock_lock(&lock);
	debug("Starting app on core %d\n", core_num);
	cvmx_spinlock_unlock(&lock);
#endif

	boot_vect_ptr = octeon_get_boot_vector();
	if (!boot_vect_ptr) {
		cvmx_spinlock_lock(&lock);
		printf("Error: core %d cannot map boot vector address to pointer\n",
		       core_num);
		cvmx_spinlock_unlock(&lock);
		return;
	}

	boot_vect = boot_vect_ptr[core_num];

#ifdef DEBUG
	cvmx_spinlock_lock(&lock);
	debug("%s: core num: %d, code addr: 0x%llx, app_start_func_addr: 0x%x, k0: 0x%x, boot_info_addr: 0x%llx, flags: 0x%x\n",
	      __func__, core_num,
	      boot_vect.code_addr, boot_vect.app_start_func_addr,
	      boot_vect.k0_val, boot_vect.boot_info_addr, boot_vect.flags);
	cvmx_spinlock_unlock(&lock);
#endif
	/* Here we look up the boot_info_ptr again.  We only need to do this for
	 * the bootoct Linux utility case, but since u-boot set up
	 * boot_info_addr properly for its own use it works in both cases.
	 */
	boot_info_ptr = (void *)(uint32_t) boot_vect.boot_info_addr;
#ifdef DEBUG
	cvmx_spinlock_lock(&lock);
	debug("Boot info block for core %d:\n"
	      "  tlb entries ptr: %p\n"
	      "  entry point: 0x%llx\n"
	      "  boot descriptor: 0x%llx\n"
	      "  stack top: 0x%llx\n"
	      "  exception base: 0x%x\n"
	      "  cvmx_desc_addr: 0x%llx\n"
	      "  flags: 0x%llx\n", core_num,
	      boot_info_ptr->tlb_entries, boot_info_ptr->entry_point,
	      boot_info_ptr->boot_desc_addr, boot_info_ptr->stack_top,
	      boot_info_ptr->exception_base, boot_info_ptr->cvmx_desc_addr,
	      boot_info_ptr->flags);
	cvmx_spinlock_unlock(&lock);
#endif
	tlb_entries = boot_info_ptr->tlb_entries;
	if (!tlb_entries) {
		cvmx_spinlock_lock(&lock);
		printf("Error: tlb_entries for core %d is NULL!!!\n", core_num);
		cvmx_spinlock_unlock(&lock);
		return;
	}

	/* Set local cycle counter based on global counter in IPD */
	octeon_init_cvmcount();

	set_except_base_addr(boot_info_ptr->exception_base);

	/* Check to see if we should enable icache prefetching.  On pass 1
	 * it should always be disabled
	 */
	val = get_cop0_cvmctl_reg();
	if ((boot_info_ptr->flags & BOOT_INFO_FLAG_DISABLE_ICACHE_PREFETCH))
		val |= (1ull << 13);
	else
		val &= ~(1ull << 13);
	/* Disable Fetch under fill on CN63XXp1 due to errata Core-14317 */
	if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X))
		val |= (1ull << 19);	/*CvmCtl[DEFET] */
	set_cop0_cvmctl_reg(val & 0xfffff3);

	/* Disable core stalls on write buffer full */
	if (OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2)) {
		val = get_cop0_cvmmemctl_reg();
		val |= 1ull << 29;	/* set diswbfst */
		set_cop0_cvmmemctl_reg(val);
	}

	/* Allow use of XI bit in TLB */
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX)) {
		uint32_t val;
		CVMX_MF_PAGEGRAIN(val);
		val |= 1 << 30;
		CVMX_MT_PAGEGRAIN(val);
	}

	num_tlb_entries = get_num_tlb_entries();

	for (entry = 0; entry < num_tlb_entries; entry++) {
		octeon_tlb_entry_t_bl *tlb_entry = &tlb_entries[entry];
		if (!tlb_entry->page_size)
			continue;

		if (tlb_entry->phy_page0 != tlb_entry->phy_page1) {
			bl_write_tlb(tlb_entry->virt_base,
				     ((uint64_t) tlb_entry->flags.xi << 62) |
				     tlb_entry->phy_page0 >> 6 |
				     (tlb_entry->flags.cca << 3) | 0x7,
				     ((uint64_t) tlb_entry->flags.xi << 62) |
				     tlb_entry->phy_page1 >> 6 |
				     (tlb_entry->flags.cca << 3) | 0x7,
				     ~(ALIGN_MASK(tlb_entry->page_size) << 1),
				     entry);
		} else {
			/* We used to map both halves of the virtual
			 * space to the same page.
			 * Better to make one page invalid
			 */
			bl_write_tlb(tlb_entry->virt_base,
				     ((uint64_t) tlb_entry->flags.xi << 62) |
				     tlb_entry->phy_page0 >> 6 |
				     (tlb_entry->flags.cca << 3) | 0x7,
				     0,	/* Page 1 is invalid */
				     ~(ALIGN_MASK(tlb_entry->page_size) << 1),
				     entry);
		}
	}

	/* Sync up cores before starting main */
	if (do_sync)
		octeon_sync_cores();

	/* Pass address of stack top variable.  Also enables cavium only
	 * unaligned load/store instructions
	 * This function does not return, but performs an eret to the entry
	 * point of the application.
	 */
	/* Make local copies of arguments */
	stack_top = boot_info_ptr->stack_top;
	entry_point = boot_info_ptr->entry_point;
	desc_vaddr = boot_info_ptr->boot_desc_addr;

	/*
	 * Flush i-cache. This is needed if we have shutdown one application
	 * and are launching a new one on the same core, since i-cache may be
	 * out of sync with the new TLB entries we have setup above.
	 */
	__asm__ volatile ("synci 0($0)");

	octeon_setup_crt0_tlb((uint32_t) & stack_top, (uint32_t) & entry_point,
			      (uint32_t) & desc_vaddr);
}
void start_app(void)
{
	_start_app(1);
}

void start_app_hotplug(void)
{
	_start_app(0);
}

/* Functions to truncate 64 bit ELF headers to 32 bit for processing */
void copy_elf64_header(struct elf32headertype *dest_eh, void *elf64_eh)
{
	struct elf64headertype *elf64_ptr = elf64_eh;

	dest_eh->type = elf64_ptr->type;
	dest_eh->machine = elf64_ptr->machine;
	dest_eh->version = elf64_ptr->version;
	dest_eh->entry = elf64_ptr->entry;
	dest_eh->phoff = (uint32_t) elf64_ptr->phoff;
	dest_eh->shoff = (uint32_t) elf64_ptr->shoff;
	dest_eh->flags = elf64_ptr->flags;
	dest_eh->ehsize = (uint32_t) elf64_ptr->ehsize;
	dest_eh->phentsize = (uint32_t) elf64_ptr->phentsize;
	dest_eh->phnum = (uint32_t) elf64_ptr->phnum;
	dest_eh->shentsize = (uint32_t) elf64_ptr->shentsize;
	dest_eh->shnum = (uint32_t) elf64_ptr->shnum;
	dest_eh->shstrndx = (uint32_t) elf64_ptr->shstrndx;

}

void copy_elf64_section_header(struct elf32sectionheadertype *dest_sh,
			       void *elf64_sh)
{
	struct elf64sectionheadertype *elf64_sh_ptr = elf64_sh;

	dest_sh->name = elf64_sh_ptr->name;
	dest_sh->type = elf64_sh_ptr->type;
	dest_sh->flags = (uint32_t) elf64_sh_ptr->flags;
	dest_sh->addr = (uint32_t) elf64_sh_ptr->addr;
	dest_sh->offset = (uint32_t) elf64_sh_ptr->offset;
	dest_sh->size = (uint32_t) elf64_sh_ptr->size;
	dest_sh->link = elf64_sh_ptr->link;
	dest_sh->info = elf64_sh_ptr->info;
	dest_sh->addralign = (uint32_t) elf64_sh_ptr->addralign;
	dest_sh->entsize = (uint32_t) elf64_sh_ptr->entsize;

}

void copy_elf64_program_header(struct elf32programheadertype *dest_ph,
			       void *elf64_ph)
{
	struct elf64programheadertype *elf64_ph_ptr = elf64_ph;

	dest_ph->type = elf64_ph_ptr->type;
	dest_ph->offset = (uint32_t) (elf64_ph_ptr->offset);
	dest_ph->vaddr = (uint32_t) (elf64_ph_ptr->vaddr);
	dest_ph->paddr = (uint32_t) (elf64_ph_ptr->paddr);
	dest_ph->filesz = (uint32_t) (elf64_ph_ptr->filesz);
	dest_ph->memsz = (uint32_t) (elf64_ph_ptr->memsz);
	dest_ph->flags = elf64_ph_ptr->flags;
	dest_ph->align = (uint32_t) (elf64_ph_ptr->align);
}

struct elf_info {
#define MAX_ELF_SEGMENTS    (20)
	char *elf_ptr;
	struct elf32headertype *ehp;
	struct elf32headertype eh;
	struct elf32programheadertype ph;
	struct elf32sectionheadertype sh;
	segment_desc_t ro_segments[MAX_ELF_SEGMENTS];
	segment_desc_t rw_segments[MAX_ELF_SEGMENTS];
	segment_desc_t shared_segment;
	uint64_t ro_min_vaddr;
	uint64_t ro_max_vaddr;
	uint64_t rw_min_vaddr;
	uint64_t rw_max_vaddr;
};

int read_elf_info(struct elf_info *ei, uint32_t elf_addr)
{
	char *strtab_ptr;
	unsigned int strtab_size;
	uint32_t elf_type = 0;
	int h;
	uint32_t shared_data_addr = 0;
	int ro_seg_index = 0;
	int rw_seg_index = 0;

	ei->elf_ptr = cvmx_phys_to_ptr(elf_addr);

	ei->ehp = (void *)ei->elf_ptr;

	if (ei->ehp->ident[0] != 127 || ei->ehp->ident[1] != 'E'
	    || ei->ehp->ident[2] != 'L' || ei->ehp->ident[3] != 'F') {
		printf("Pointer(%p) not to ELF data!\n", ei->elf_ptr);
		return -1;	/* not ELF file */
	}
	if (ei->ehp->ident[4] != ELFCLASS64 && ei->ehp->ident[4] != ELFCLASS32) {
		printf("Data isn't an ELF32 or ELF64 file!\n");
		return -1;	/* not ELF file */
	}
	elf_type = ei->ehp->ident[4];
	if (ei->ehp->ident[5] != 2) {
		printf("Data isn't in big-endian mode.\n");
		return -1;	/* not ELF file */
	}
	if (ei->ehp->type != 2) {
		printf("Data isn't an executable!\n");
		return -1;	/* not ELF file */
	}
	if (ei->ehp->machine != 8 && ei->ehp->machine != 9) {
		printf("Data isn't an executable for a MIPS R3000 or R4000!\n");
		return -1;	/* not ELF file */
	}

	if (elf_type == ELFCLASS32)
		memcpy(&ei->eh, ei->elf_ptr, sizeof(ei->eh));
	else
		copy_elf64_header(&ei->eh, ei->elf_ptr);

	debug("Entry point for ELF executable(q): 0x%x\n", ei->eh.entry);

	/* Need to find the string table section entry so that we can
	 * get names of sections
	 */
	for (h = ei->eh.shnum - 1; h >= 0; h--) {
		if (elf_type == ELFCLASS32)
			memcpy(&ei->sh,
			       ei->elf_ptr + ei->eh.shoff +
			       ei->eh.shentsize * h, sizeof(ei->sh));
		else
			copy_elf64_section_header(&ei->sh,
						  ei->elf_ptr + ei->eh.shoff +
						  ei->eh.shentsize * h);

		if (ei->sh.type == 3)	/* I have a string section */
		{

			strtab_ptr = ei->elf_ptr + ei->sh.offset;
			strtab_size = ei->sh.size;

			if (0 ==
			    strncmp(".shstrtab", &strtab_ptr[ei->sh.name],
				    200)) {
				debug("Found .shstrtab at offset 0x%x\n",
				      ei->sh.offset);
				break;
			}
		}
	}
	if (h < 0) {
		printf("ERROR: unable to find section header string table, aborting\n");
		return -1;	/* I couldn't find a section header string table */
	}

	/* I want to find the stack and heap */
	for (h = 0; h < ei->eh.shnum; h++) {
		if (elf_type == ELFCLASS32)
			memcpy(&ei->sh,
			       ei->elf_ptr + ei->eh.shoff + ei->eh.shentsize * h,
			       sizeof(ei->sh));
		else
			copy_elf64_section_header(&ei->sh,
						  ei->elf_ptr + ei->eh.shoff +
						  ei->eh.shentsize * h);

		/* printf("segment: %s found, addr: 0x%llx\n",
		 *        &strtab_ptr[ei->sh.name], ei->sh.addr);
		 */

		if (ei->sh.name < (unsigned)strtab_size &&
		    (strcmp(&strtab_ptr[ei->sh.name], ".cvmx_shared_hw") == 0)) {
			printf("Error: cvmx_shared_hw is no longer supported.\n");
			return -1;
		}
		if (ei->sh.name < (unsigned)strtab_size
		    && (strncmp(&strtab_ptr[ei->sh.name], ".cvmx_shared", 12) == 0)) {
			/* Only make note of first entry, as we want this to
			 * match the start of the segment
			 */
			if (!shared_data_addr) {
				shared_data_addr = ei->sh.addr;
				debug("found shared data at: 0x%x\n",
				      shared_data_addr);
			}
		}
	}
#define MAX_ELF_SEGMENTS    (20)
	/*  Now process the program headers. */
	for (h = 0; h < ei->eh.phnum; h++) {
		if (elf_type == ELFCLASS32)
			memcpy(&ei->ph,
			       ei->elf_ptr + ei->eh.phoff + ei->eh.phentsize * h,
			       sizeof(ei->ph));
		else
			copy_elf64_program_header(&ei->ph,
						  ei->elf_ptr + ei->eh.phoff +
						  ei->eh.phentsize * h);

		/* I have a loadable segment that has a non-zero memory size */
		if (ei->ph.type == 1 && ei->ph.memsz) {
			/* Check to see if the shared address is anywhere in
			 * this segment
			 */
			if (shared_data_addr >= ei->ph.vaddr
			    && shared_data_addr <= (ei->ph.vaddr + ei->ph.memsz)) {
				debug("Shared segment : 0x%x bytes to address 0x%x\n",
				      ei->ph.filesz, ei->ph.vaddr);
				ei->shared_segment.file_offset = ei->ph.offset;
				ei->shared_segment.file_size = ei->ph.filesz;
				ei->shared_segment.mem_addr = ei->ph.vaddr;
				ei->shared_segment.mem_size = ei->ph.memsz;
			} else if (!(ei->ph.flags & PF_W)) {
				debug("Readonly segment: 0x%x bytes to address 0x%x\n",
				      ei->ph.filesz, ei->ph.vaddr);
				/* Read only segment */
				ei->ro_segments[ro_seg_index].file_offset =
				    ei->ph.offset;
				ei->ro_segments[ro_seg_index].file_size =
				    ei->ph.filesz;
				ei->ro_segments[ro_seg_index].mem_addr =
				    ei->ph.vaddr;
				ei->ro_segments[ro_seg_index].mem_size =
				    ei->ph.memsz;
				ro_seg_index++;
			} else {
				debug("Writable segment: 0x%x bytes to address 0x%x\n",
				      ei->ph.filesz, ei->ph.vaddr);
				/* writable segment */
				ei->rw_segments[rw_seg_index].file_offset =
				    ei->ph.offset;
				ei->rw_segments[rw_seg_index].file_size =
				    ei->ph.filesz;
				ei->rw_segments[rw_seg_index].mem_addr =
				    ei->ph.vaddr;
				ei->rw_segments[rw_seg_index].mem_size =
				    ei->ph.memsz;
				rw_seg_index++;
			}
		}
	}

	/*  determine address range for each type */
	if (ro_seg_index) {
		int i;
		ei->ro_min_vaddr = ei->ro_segments[0].mem_addr;
		ei->ro_max_vaddr =
		    ei->ro_segments[0].mem_addr + ei->ro_segments[0].mem_size;
		for (i = 1; i < ro_seg_index; i++) {
			if (ei->ro_min_vaddr > ei->ro_segments[i].mem_addr)
				ei->ro_min_vaddr = ei->ro_segments[i].mem_addr;
			if (ei->ro_max_vaddr < (ei->ro_segments[i].mem_addr +
						ei->ro_segments[i].mem_size))
				ei->ro_max_vaddr = (ei->ro_segments[i].mem_addr
						    + ei->ro_segments[i].mem_size);
		}
		ei->ro_max_vaddr--;
	}

	if (rw_seg_index) {
		int i;
		ei->rw_min_vaddr = ei->rw_segments[0].mem_addr;
		ei->rw_max_vaddr =
		    ei->rw_segments[0].mem_addr + ei->rw_segments[0].mem_size;
		for (i = 1; i < rw_seg_index; i++) {
			if (ei->rw_min_vaddr > ei->rw_segments[i].mem_addr)
				ei->rw_min_vaddr = ei->rw_segments[i].mem_addr;
			if (ei->rw_max_vaddr < (ei->rw_segments[i].mem_addr +
						ei->rw_segments[i].mem_size))
				ei->rw_max_vaddr = (ei->rw_segments[i].mem_addr +
						    ei->rw_segments[i].mem_size);
		}
		ei->rw_max_vaddr--;
	}

	debug(" ro range: 0x%llx - 0x%llx\n rw range: 0x%llx - 0x%llx\n"
	      " shared range: 0x%x - 0x%x\n",
	      ei->ro_min_vaddr, ei->ro_max_vaddr, ei->rw_min_vaddr,
	      ei->rw_max_vaddr, ei->shared_segment.mem_addr,
	      ei->shared_segment.mem_addr + ei->shared_segment.mem_size - 1);

	return 0;
}

#define OCTEON_BL_FLAG_HPLUG_CORES (1 << 6)
int load_elf_additional_cores(uint32_t elf_addr, int argc, char *const argv[],
			      uint32_t stack_size, uint32_t heap_size,
			      cvmx_coremask_t *pcm, cvmx_coremask_t *new_pcm)
{
	uint32_t stack_heap_base_address = 0;
	uint32_t config_flags = 0;
	cvmx_coremask_t new_cores;
	int first_core;
	int core;
	tlb_flags_t tlb_flags = { 0, 0, 0, 0 };
	int tlb_index = 0;
	int segment;

	struct elf_info elfinfo;
	struct elf_info *ei = &elfinfo;

	cvmx_coremask_xor(&new_cores, new_pcm, pcm);
	first_core = cvmx_coremask_get_first_core(pcm);

	memset(ei, 0, sizeof(elfinfo));

	read_elf_info(ei, elf_addr);

	if (heap_size < sizeof(boot_desc[0])) {
		printf("ERROR: heap too small: 0x%x, must be at least 0x%x "
		       "bytes for temp boot descriptor storage\n",
		       heap_size, sizeof(boot_desc[0]));
		return -1;
	}
	/* printf("new cores=%08x core_mask=%08x ext_core_mask=%08x\n",
	 *        new_cores, core_mask, ext_core_mask);
	 */

	if (boot_info_block_array[first_core].tlb_entries == NULL) {
		printf("%s: Error: tlb entries for core %d is NULL\n",
		       __func__, first_core);
		return -1;
	}

	if (alloc_tlbs(&new_cores)) {
		printf("%s: Error allocating TLBs\n", __func__);
		return -1;
	}

	cvmx_coremask_for_each_core(core, &new_cores) {
		int i;
		for (i = 0; i < NUM_OCTEON_TLB_ENTRIES; i++) {
			if (boot_info_block_array[first_core].tlb_entries[i].virt_base == 0)
				continue;
			if ((boot_info_block_array[first_core].tlb_entries[i].flags.seg_type == RO_TLB_SEGMENT)
			    || (boot_info_block_array[first_core].tlb_entries[i].flags.seg_type == SHARED_TLB_SEGMENT)) {
				copy_tlb_entry(boot_info_block_array[core].tlb_entries,
					       &boot_info_block_array[first_core].tlb_entries[i]);
			}
		}
	}

	/* Here we 'mark' the stack, heap, and data segment for the application.
	 * This allows us to use the SYNCWS instruction.  This is new is Octeon
	 * pass2, see the HW manual for more information.
	 */
	if (!getenv("no_mark_private_data"))
		tlb_flags.cca = 0x7;
	else
		tlb_flags.cca = 0x0;

	if (ei->rw_min_vaddr < 0x10000000 || ei->rw_max_vaddr > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("rw segment too big\n");
	}

	cvmx_coremask_for_each_core(core, &new_cores) {
		debug("core: %d\n", core);
		tlb_flags.seg_type = RW_TLB_SEGMENT;

		tlb_index =
		    add_tlb_mapping(cvmx_coremask_core_to_node(core),
				    boot_info_block_array[core].tlb_entries,
				    ei->rw_min_vaddr, ei->rw_max_vaddr, 28,
				    tlb_flags);
		debug("RW tlb_index = %d\n\n", tlb_index);
		if (tlb_index < 0) {
			printf("ERROR CREATING TLB MAPPING (rw)\n");
			return -1;
		}
	}

	/* Put stack/heap base above cvmx shared segment end, and align so that
	 * we won't overlap the shared mapping
	 */
	stack_heap_base_address =
	    ei->shared_segment.mem_addr + ei->shared_segment.mem_size - 1;
	stack_heap_base_address = (stack_heap_base_address +
		(1 << 25)) & ~((1 << 25) - 1);	/* 32 MByte alignment */

	if (stack_heap_base_address < 0x10000000
	    || stack_heap_base_address + stack_size + heap_size > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("stack/heap segment too big\n");
	}
	cvmx_coremask_for_each_core(core, &new_cores) {
		/* Determine base address of stack/heap mapping based on size,
		 * just use fake addresses for calculations
		 */
		uint32_t alignment_exp;
		uint32_t map_size =
		    get_tlb_mapping_size(stack_heap_base_address,
					 stack_heap_base_address + stack_size +
					 heap_size, 1, &alignment_exp);
		if (!map_size) {
			printf("ERROR CREATING TLB MAPPING for stack/heap\n");
			return -1;
		}

		/* Adjust stack/heap to start at bottom of mapping, as this
		 * makes half-mappings possible
		 */
		stack_heap_base_address =
		    stack_heap_base_address & ALIGN_MASK(alignment_exp + 1);

		/* We now have the mapping size, so now we can compute the base
		 * address for the mapping
		 */
		debug("core: %d\n", core);
		tlb_flags.seg_type = STACK_HEAP_TLB_SEGMENT;
		tlb_index =
		    add_tlb_mapping(cvmx_coremask_core_to_node(core),
				    boot_info_block_array[core].tlb_entries,
				    stack_heap_base_address,
				    stack_heap_base_address + stack_size +
				    heap_size - 1, 28, tlb_flags);
		debug("Stack/heap tlb_index = %d\n\n", tlb_index);
		if (tlb_index < 0) {
			printf("ERROR CREATING TLB MAPPING for stack/heap\n");
			return -1;
		}
	}

	/* Copy the writable segments for each core. */
	cvmx_coremask_for_each_core(core, &new_cores) {
		for (segment = 0; segment < MAX_ELF_SEGMENTS; segment++) {
			if (ei->rw_segments[segment].file_size) {
				debug("%s: Copying TLB for core %d\n",
				      __func__, core);
				mem_copy_tlb(boot_info_block_array[core].tlb_entries,
					     ei->rw_segments[segment].mem_addr,
					     MAKE_XKPHYS(ei->rw_segments[segment].file_offset +
							 elf_addr),
					     ei->rw_segments[segment].file_size);
				mem_set_tlb(boot_info_block_array[core].tlb_entries,
					    ei->rw_segments[segment].mem_addr +
					    ei->rw_segments[segment].file_size,
					    0x0,
					    ei->rw_segments[segment].mem_size -
					    ei->rw_segments[segment].file_size);
			}
		}
	}
	boot_flags |= OCTEON_BL_FLAG_HPLUG_CORES;
	if (octeon_setup_boot_desc_block(pcm, argc, argv, ei->eh.entry,
					 stack_size, heap_size, boot_flags,
					 stack_heap_base_address, 0,
					 config_flags, new_pcm, -1)) {
		puts("ERROR setting up boot descriptor block, core_mask:\n");
		cvmx_coremask_print(pcm);
		cvmx_coremask_print(new_pcm);
		return -1;
	}
#ifdef DEBUG_TLB
	cvmx_coremask_for_each_core(core, new_pcm) {
		printf("\nCore %d:\n", core);
		print_tlb_array(boot_info_block_array[core].tlb_entries, 32);
		printf("----------------------------------\n");
	}
#endif

	return 0;
}

static
int load_elf(uint32_t elf_addr, int argc, char *const argv[],
	     uint32_t stack_size, uint32_t heap_size,
	     cvmx_coremask_t *pcm, int app_index)
{
	uint32_t stack_heap_base_address = 0;
	uint32_t config_flags = 0;

	struct elf_info elfinfo;
	struct elf_info *ei = &elfinfo;
	int segment, core;
	int tlb_index = 0;
	tlb_flags_t tlb_flags = { 0, 0, 0, 0 };
	int prev_tlb_index = 0;	/* Used to track entries to copy */
	int i;
	cvmx_coremask_t empty_coremask;
	int first_core = cvmx_coremask_get_first_core(pcm);

	memset(ei, 0, sizeof(elfinfo));
	cvmx_coremask_clear_all(&empty_coremask);

	read_elf_info(ei, elf_addr);

	debug("%s: first core: %d, core mask:\n", __func__, first_core);
	cvmx_coremask_dprint(pcm);

	if (heap_size < sizeof(boot_desc[0])) {
		printf("ERROR: heap too small: 0x%x, must be at least 0x%x "
		       "bytes for temp boot descriptor storage\n",
		       heap_size, sizeof(boot_desc[0]));
		return -1;
	}

	/* We have 3 regions to map:
	 *   code/readonly data
	 *   shared data
	 *   private data (per core)
	 */

	/* We need these mapped the same on all cores, and we only want
	 * to allocate physical memory once
	 */
	if (ei->ro_min_vaddr < 0x10000000 || ei->ro_max_vaddr > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("ro segment too big, min: 0x%llx, max: 0x%llx\n",
		      ei->ro_min_vaddr, ei->ro_max_vaddr);
	}

	debug("RO Coremask:\n");
	cvmx_coremask_dprint(pcm);

	if (alloc_tlbs(pcm)) {
		printf("%s: Error allocating TLB arrays\n", __func__);
		return -1;
	}

	cvmx_coremask_for_each_core(core, pcm) {
		tlb_flags.seg_type = RO_TLB_SEGMENT;
		if (core == first_core) {
			debug("Adding TLB mapping for core %d\n", core);
			tlb_index =
			    add_tlb_mapping(cvmx_coremask_core_to_node(core),
					    boot_info_block_array[core].tlb_entries,
					    ei->ro_min_vaddr,
					    ei->ro_max_vaddr, 28,
					    tlb_flags);
			debug("RO tlb_index = %d\n\n", tlb_index);
			if (tlb_index < 0) {
				printf("ERROR CREATING TLB MAPPING (read only)\n");
				debug("core: %d, min vaddr: 0x%llx, max vaddr: 0x%llx\n",
				      core, ei->ro_min_vaddr, ei->ro_max_vaddr);
				return -1;
			}
		} else {
			/* We may need to copy more than one TLB entry.
			 * (older bootloaders only allowed 1 TLB entry
			 * per segment)
			 */
			for (i = prev_tlb_index; i <= tlb_index; i++)
				copy_tlb_entry(boot_info_block_array[core].tlb_entries,
					       &boot_info_block_array[first_core].tlb_entries[i]);
		}
	}
	prev_tlb_index = tlb_index + 1;

	/* Enable XI bit usage if supported.  Mark data, cvmx_shared,
	 * stack, heap as not executable
	 */
	if (!OCTEON_IS_MODEL(OCTEON_CN3XXX))
		tlb_flags.xi = 0x1;

	if (ei->shared_segment.mem_addr < 0x10000000 ||
	    (ei->shared_segment.mem_addr +
	     ei->shared_segment.mem_size - 1) > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("Shared segment too big: start addr: 0x%x, end: 0x%x\n",
		      ei->shared_segment.mem_addr,
		      (ei->shared_segment.mem_addr +
		       ei->shared_segment.mem_size - 1));
	}

	if (ei->shared_segment.mem_size) {
		cvmx_coremask_for_each_core(core, pcm) {
			tlb_flags.seg_type = SHARED_TLB_SEGMENT;
			if (first_core == core) {
				tlb_index =
				    add_tlb_mapping(cvmx_coremask_core_to_node(core),
						    boot_info_block_array[core].tlb_entries,
						    ei->shared_segment.mem_addr,
						    ei->shared_segment.mem_addr +
						    ei->shared_segment.mem_size - 1,
						    28,
						    tlb_flags);
				debug("Shared tlb_index = %d\n\n",
				      tlb_index);
				if (tlb_index < 0) {
					printf("ERROR CREATING TLB MAPPING (CVMX_SHARED)\n");
					return -1;
				}
			} else {
				for (i = prev_tlb_index; i <= tlb_index; i++)
					copy_tlb_entry(boot_info_block_array[core].tlb_entries,
						       &boot_info_block_array[first_core].tlb_entries[i]);
			}
		}
		prev_tlb_index = tlb_index + 1;
	}

	/* Here we 'mark' the stack, heap, and data segment for the
	 * application.  This allows us to use the SYNCWS instruction.
	 *  This is new is Octeon pass2, see the HW manual for more
	 * information.
	 */
	if (!getenv("no_mark_private_data"))
		tlb_flags.cca = 0x7;
	else
		tlb_flags.cca = 0x0;

	if (ei->rw_min_vaddr < 0x10000000 || ei->rw_max_vaddr > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("rw segment too big\n");
	}

	cvmx_coremask_for_each_core(core, pcm) {
		debug("core: %d\n", core);
		tlb_flags.seg_type = RW_TLB_SEGMENT;
		tlb_index =
		    add_tlb_mapping(cvmx_coremask_core_to_node(core),
				    boot_info_block_array[core].tlb_entries,
				    ei->rw_min_vaddr,
				    ei->rw_max_vaddr, 28, tlb_flags);
		debug("RW tlb_index = %d\n\n", tlb_index);
		if (tlb_index < 0) {
			printf("ERROR CREATING TLB MAPPING (rw)\n");
			return -1;
		}
	}

	/* Need to allow stack to be anywhere now that we allow mappings
	 * outside of the boot bus window
	 */

	/* Put stack/heap base above cvmx shared segment end, and align
	 * so that we won't overlap the shared mapping
	 */
	stack_heap_base_address =
	    ei->shared_segment.mem_addr + ei->shared_segment.mem_size - 1;
	stack_heap_base_address = (stack_heap_base_address +
			(1 << 25)) & ~((1 << 25) - 1);	/* 32 MByte alignment */

	if (stack_heap_base_address < 0x10000000
	    || stack_heap_base_address + stack_size + heap_size > 0x1fffffff) {
		config_flags |= CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING;
		debug("stack/heap segment too big\n");
	}
	cvmx_coremask_for_each_core(core, pcm) {
		/* Determine base address of stack/heap mapping based
		 * on size, just use fake addresses for calculations
		 */
		uint32_t alignment_exp;
		uint32_t map_size;

		map_size = get_tlb_mapping_size(stack_heap_base_address,
						stack_heap_base_address +
						stack_size + heap_size,
						1, &alignment_exp);
		if (!map_size) {
			printf("ERROR CREATING TLB MAPPING for stack/heap\n");
			return -1;
		}

		/* Adjust stack/heap to start at bottom of mapping, as
		 * this makes half-mappings possible
		 */
		stack_heap_base_address =
		    stack_heap_base_address & ALIGN_MASK(alignment_exp + 1);

		/* We now have the mapping size, so now we can compute
		 * the base address for the mapping
		 */
		debug("core: %d\n", core);
		tlb_flags.seg_type = STACK_HEAP_TLB_SEGMENT;
		tlb_index = add_tlb_mapping(cvmx_coremask_core_to_node(core),
					    boot_info_block_array[core].tlb_entries,
					    stack_heap_base_address,
					    stack_heap_base_address +
					    stack_size + heap_size - 1, 28,
					    tlb_flags);
		debug("Stack/heap tlb_index = %d\n\n", tlb_index);
		if (tlb_index < 0) {
			printf("ERROR CREATING TLB MAPPING for stack/heap\n");
			return -1;
		}
	}
	/* Set TLB flags back to default value */
	tlb_flags.cca = 0x0;
	tlb_flags.xi = 0x0;
	tlb_flags.ri = 0x0;
	tlb_flags.seg_type = 0;

	debug("Done with TLB mappings, now copying program data\n");

#ifdef DEBUG_TLB
	cvmx_coremask_for_each_core(core, pcm) {
		printf("\nCore %d:\n", core);
		print_tlb_array(boot_info_block_array[core].tlb_entries, 32);
		printf("----------------------------------\n");
	}
#endif

	/* copy memory segments.... */

	/* Make one copy of the readonly segments, shared by
	 * all cores
	 */
	for (segment = 0; segment < MAX_ELF_SEGMENTS; segment++) {
		if (ei->ro_segments[segment].file_size) {
			debug("Copying read-only TLB segment for segment %d, core %d\n",
			      segment, first_core);
			mem_copy_tlb(boot_info_block_array[first_core].tlb_entries,
				     ei->ro_segments[segment].mem_addr,
				     MAKE_XKPHYS(ei->ro_segments[segment].file_offset +
						 elf_addr),
			ei->ro_segments[segment].file_size);
			mem_set_tlb(boot_info_block_array[first_core].tlb_entries,
				    ei->ro_segments[segment].mem_addr +
				    ei->ro_segments[segment].file_size,
				    0x0,
				    ei->ro_segments[segment].mem_size -
				    ei->ro_segments[segment].file_size);
		}
	}
	/* Copy the shared segment */
	if (ei->shared_segment.mem_size) {
		debug("Copying shared TLB segment for core %d\n", first_core);
		mem_copy_tlb(boot_info_block_array[first_core].tlb_entries,
			     ei->shared_segment.mem_addr,
			     MAKE_XKPHYS(ei->shared_segment.file_offset +
					 elf_addr),
			     ei->shared_segment.file_size);
		mem_set_tlb(boot_info_block_array[first_core].tlb_entries,
			    ei->shared_segment.mem_addr +
			    ei->shared_segment.file_size,
			    0x0,
			    ei->shared_segment.mem_size -
			    ei->shared_segment.file_size);
	}

	/* Copy the writable segments for each core. */
	cvmx_coremask_for_each_core(core, pcm) {
		for (segment = 0; segment < MAX_ELF_SEGMENTS; segment++) {
			if (ei->rw_segments[segment].file_size) {
				debug("Copying writeable TLB segment for segment %d, core %d\n",
				      segment, first_core);
				mem_copy_tlb(boot_info_block_array[core].tlb_entries,
					     ei->rw_segments[segment].mem_addr,
					     MAKE_XKPHYS(ei->rw_segments[segment].file_offset +
							 elf_addr),
					     ei->rw_segments[segment].file_size);
				mem_set_tlb(boot_info_block_array[core].tlb_entries,
					    ei->rw_segments[segment].mem_addr +
					    ei->rw_segments[segment].file_size,
					    0x0,
					    ei->rw_segments[segment].mem_size -
					    ei->rw_segments[segment].file_size);
			}
		}
	}

	if (config_flags & CVMX_BOOTINFO_CFG_FLAG_OVERSIZE_TLB_MAPPING)
		printf("NOTICE: oversize TLB mappings for application used, "
		       "application can't use 1-1 TLB mappings.\n");

	if (octeon_setup_boot_desc_block(pcm, argc, argv, ei->eh.entry,
					 stack_size, heap_size, boot_flags,
					 stack_heap_base_address, 0,
					 config_flags, &empty_coremask,
					 app_index)) {
		puts("ERROR setting up boot descriptor block, core_mask:\n");
		cvmx_coremask_print(pcm);
		return -1;
	}

	/* Start other cores, but only if core zero is in mask */
	if ((cvmx_coremask_is_core_set(pcm, 0)) || forceboot)
		octeon_bootloader_shutdown();

	if (octeon_setup_boot_vector((uint32_t) start_app, pcm)) {
		puts("ERROR setting boot vectors, core_mask:\n");
		cvmx_coremask_print(pcm);
		return -1;
	}

	/* Add coremask to global mask of cores that have been set up and are
	 * runable
	 */
	cvmx_coremask_or(&coremask_to_run, &coremask_to_run, pcm);

	puts("Bootloader: Done loading app on coremask:\n");
	cvmx_coremask_print(pcm);

	/* Start other cores, but only if core zero is in mask */
	if ((cvmx_coremask_is_core_set(pcm, 0)) || forceboot) {
		start_cores(&coremask_to_run);	/* does not return */
	}

	return 0;
}

/**
 * Idle cores come here and will spin on a wait loop conserving power until
 * NMI is received.
 */
void idle_core_start_app(void)
{
	uint64_t addr;
	cvmx_bootmem_named_block_desc_t *block_desc;

	addr = cvmx_bootmem_phy_named_block_find(IDLE_CORE_BLOCK_NAME,
						 CVMX_BOOTMEM_FLAG_NO_LOCKING);

	if (!addr) {
		printf("Named block(%s) is not created\n",
		       IDLE_CORE_BLOCK_NAME);
		/* loop here, should not happen */
		while (1)
			asm volatile ("wait");
	}

	block_desc = (cvmx_bootmem_named_block_desc_t *) cvmx_phys_to_ptr(addr);
	asm volatile ("       .set push                \n"
		      "       .set mips64              \n"
		      "       .set noreorder           \n"
		      "       move	$25, %[addr]   \n"
		      "       jr	$25            \n"
		      "        nop                     \n"
		      "       .set pop "
		      :: [addr] "r"(block_desc->base_addr)
		      : "$2");
	/* We should never get here */
	printf("Error: return from idle loop not expected!\n");
	while (1)
		asm volatile ("wait");
}
