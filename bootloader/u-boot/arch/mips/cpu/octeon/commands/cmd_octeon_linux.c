/*
 * Copyright (c) 2001 William L. Pitts
 * All rights reserved.
 * Copyright (c) 2003-2014 Cavium Inc. (support@cavium.com).
 * Redistribution and use in source and binary forms are freely
 * permitted provided that the above copyright notice and this
 * paragraph and the following disclaimer are duplicated in all
 * such forms.
 *
 * This software is provided "AS IS" and without any express or
 * implied warranties, including, without limitation, the implied
 * warranties of merchantability and fitness for a particular
 * purpose.
 */

#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <net.h>
#include <elf.h>
#include <asm/arch/octeon_eeprom_types.h>
#include <asm/arch/lib_octeon.h>
#include <asm/mipsregs.h>
#include <asm/processor.h>
#include <asm/arch/cvmx-coremask.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon-boot-info.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/bootoct_cmd.h>
#include "octeon_biendian.h"

DECLARE_GLOBAL_DATA_PTR;

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

int valid_elf_image(unsigned long addr);	/* from cmd_elf.c */
unsigned long load_elf_image(unsigned long addr);
uint64_t load_elf64_image(unsigned long addr, uint64_t load_override);

void start_linux(void);
void start_elf(void);
static int alloc_elf32_image(unsigned long addr);
static uint64_t alloc_elf64_linux_image(unsigned long addr, int64_t *,
                                        struct cvmx_bootmem_named_block_desc *);
extern void asm_launch_linux_entry_point(void);

volatile int start_core0 = 0;
extern uint32_t cur_exception_base;
extern boot_info_block_t boot_info_block_array[];

extern void InitTLBStart(void);	/* in start.S */
extern void start_app_hotplug(void);

void setup_linux_app_boot_info(cvmx_coremask_t *linux_core_mask)
{
	cvmx_coremask_t fuse_coremask;
	cvmx_coremask_t app_coremask;
	linux_app_boot_info_t *labi =
	    (linux_app_boot_info_t *) (0x80000000 + LABI_ADDR_IN_BOOTLOADER);

	octeon_get_available_coremask(&fuse_coremask);

	cvmx_coremask_maskoff(&app_coremask, &fuse_coremask, linux_core_mask);

	labi->labi_signature = LABI_SIGNATURE;
	debug("labi->labi_signature = 0x%x\n", labi->labi_signature);
	labi->start_core0_addr = (uint32_t) & start_core0;
	debug("labi->start_core0_addr = 0x%x\n", labi->start_core0_addr);
	if (labi->avail_coremask)
		labi->avail_coremask &= cvmx_coremask_get32(&app_coremask);
	else
		labi->avail_coremask = cvmx_coremask_get32(&app_coremask);
	debug("labi->avail_coremask = 0x%x\n", labi->avail_coremask);
	labi->pci_console_active = (getenv("pci_console_active")) ? 1 : 0;
	debug("labi->pci_console_active = 0x%x\n", labi->pci_console_active);
	labi->icache_prefetch_disable =
	    (getenv("icache_prefetch_disable")) ? 1 : 0;
	debug("labi->icache_prefetch_disable = 0x%x\n",
	      labi->icache_prefetch_disable);
	labi->no_mark_private_data = (getenv("no_mark_private_data")) ? 1 : 0;
	debug("labi->no_mark_private_data = 0x%x\n",labi->no_mark_private_data);
	/* Physical, with xkphys */
	labi->InitTLBStart_addr = MAKE_XKPHYS(cvmx_ptr_to_phys(&InitTLBStart));
	debug("labi->InitTLBStart_addr = 0x%llx\n", labi->InitTLBStart_addr);
	labi->start_app_addr = (uint32_t) &start_app_hotplug;	/* Virtual */
	debug("labi->start_app_addr = 0x%x\n", labi->start_app_addr);
	labi->cur_exception_base = cur_exception_base;
	debug("labi->cur_exception_base = 0x%x\n", labi->cur_exception_base);
	labi->compact_flash_common_base_addr =
	    cvmx_bootinfo_array[0].compact_flash_common_base_addr;
	debug("labi->compact_flash_common_base_addr = 0x%x\n",
	      labi->compact_flash_common_base_addr);
	labi->compact_flash_attribute_base_addr =
	    cvmx_bootinfo_array[0].compact_flash_attribute_base_addr;
	debug("labi->compact_flash_attribute_base_addr = 0x%x\n",
	      labi->compact_flash_attribute_base_addr);
	labi->led_display_base_addr =
	    cvmx_bootinfo_array[0].led_display_base_addr;
	debug("labi->led_display_base_addr = 0x%x\n",
	      labi->led_display_base_addr);
	octeon_translate_gd_to_linux_app_global_data(&labi->gd);

	debug("sizeof(linux_app_boot_info_t) %d start_core0 address is %x\n",
	      sizeof(*labi), labi->start_core0_addr);
}

int do_bootoctlinux(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	uint64_t entry_addr = 0;
	struct octeon_boot_args boot_args;
	struct cvmx_bootmem_named_block_desc *linux_named_block = NULL;
	uint32_t addr = 0;		/* Address of the ELF image     */
	uint32_t image_flags;
	int forceboot = 0;
	int arg_start = 1;
	cvmx_coremask_t core_mask;
	static cvmx_coremask_t empty_cm = CVMX_COREMASK_EMPTY;
	uint32_t boot_flags = 0;
	int rcode = 0;
	int arg_count;

	cvmx_coremask_clear_all(&empty_cm);
	cvmx_coremask_clear_all(&core_mask);

	if (argc >= 2 && (isxdigit(argv[1][0]) && (isxdigit(argv[1][1]) ||
						   argv[1][1] == 'x' ||
						   argv[1][1] == 'X' ||
						   argv[1][1] == '\0'))) {
		addr = simple_strtoul(argv[1], NULL, 16);
		if (!addr)
			addr = load_addr;
		arg_start++;
	}
	if (addr == 0)
		addr = load_addr;


	debug("%s: arg start: %d\n", __func__, arg_start);
	arg_count = octeon_parse_bootopts(argc - arg_start, argv + arg_start,
					  BOOTOCTLINUX, &boot_args);

#ifdef DEBUG
	{
	int i;
	debug("%s:\n"
	      " named block: %s\n"
	      " node mask: 0x%x\n"
	      " stack size: 0x%x\n"
	      " heap size: 0x%x\n"
	      " boot flags: 0x%x\n"
	      " force boot: %s\n"
	      " coremask set: %s\n"
	      " num cores set: %s\n"
	      " num skipped set: %s\n"
	      " endbootargs: %s\n",
	      __func__,
	      boot_args.named_block ? boot_args.named_block : "none",
	      boot_args.node_mask,
	      boot_args.stack_size,
	      boot_args.heap_size,
	      boot_args.boot_flags,
	      boot_args.forceboot ? "true" : "false",
	      boot_args.coremask_set ? "true" : "false",
	      boot_args.num_cores_set ? "true" : "false",
	      boot_args.num_skipped_set ? "true" : "false",
	      boot_args.endbootargs ? "true" : "false");
	debug(" num cores: ");
	for (i = 0; i < CVMX_MAX_NODES; i++)
		debug("%s%d", i > 0 ? ", " : "", boot_args.num_cores[i]);
	debug("\n num skipped: ");
	for (i = 0; i < CVMX_MAX_NODES; i++)
		debug("%s%d", i > 0 ? ", " : "", boot_args.num_skipped[i]);
	debug("\n coremask:\n");
	cvmx_coremask_print(&boot_args.coremask);
	}
#endif

	boot_flags |= boot_args.boot_flags;
	if (boot_args.endbootargs) {
		debug("endbootargs set, adjusting argc from %d to %d, arg_count: %d, arg_start: %d\n",
		      argc, argc - (arg_count + arg_start), arg_count,
		      arg_start);
		argc -= (arg_count + arg_start);
		argv += (arg_count + arg_start);
	}

	if (boot_args.named_block) {
		linux_named_block =
			(void *)cvmx_bootmem_find_named_block(boot_args.named_block);
		if (!linux_named_block) {
			printf("Specified named block %s not found\n",
			       boot_args.named_block);
			return 1;
		}
	}
	debug("%s: argc: %d, argv[0]: %s\n", __func__,
	      argc, argc > 0 ? argv[0] : "[none]");
	forceboot |= boot_args.forceboot;
	/* numcores specification overrides a coremask on the same command line
	 */
	cvmx_coremask_copy(&core_mask, &boot_args.coremask);

	/* Remove cores from coremask based on environment variable stored in
	 * flash
	 */
	if (validate_coremask(&core_mask) != 0) {
		puts("Invalid coremask.\n");
		return 1;
	} else if (cvmx_coremask_is_empty(&core_mask)) {
		puts("Coremask is empty after coremask_override mask.  Nothing to do.\n");
		return 0;
	}

	if (!valid_elf_image(addr))
		return 1;

	/* Free memory that was reserved for kernel image.  Don't check return
	 * code, as this may be the second kernel loaded, and loading will fail
	 * later if the required address isn't available.
	 */
	cvmx_bootmem_phy_named_block_free(OCTEON_LINUX_RESERVED_MEM_NAME, 0);

	if (((Elf32_Ehdr *)addr)->e_ident[EI_CLASS] == ELFCLASS32) {
		if (alloc_elf32_image(addr)) {
			entry_addr = load_elf_image(addr);
			/* make sure kseg0 addresses are sign extended */
			if ((int32_t) entry_addr < 0)
				entry_addr |= 0xffffffff00000000ull;
		}
	} else {
		/* valid_elf_image ensures only 32 and 64 bit class values */
		int64_t override_loadaddr = 0;
                entry_addr = alloc_elf64_linux_image(addr, &override_loadaddr,
						     linux_named_block);
		if (entry_addr)
			load_elf64_image(addr, override_loadaddr);
	}
	if (!entry_addr) {
		printf("## ERROR loading File!\n");
		return -1;
	}

	debug("%s: Entry address: 0x%llx\n", __func__, entry_addr);
	if (cvmx_coremask_intersects(&core_mask, &coremask_to_run)) {
		puts("ERROR: Can't load code on core twice!  Provided coremask:\n");
		cvmx_coremask_print(&core_mask);
		puts("overlaps previously loaded coremask:\n");
		cvmx_coremask_print(&coremask_to_run);
		return -1;
	}
	image_flags = OCTEON_BOOT_DESC_IMAGE_LINUX;
	if (is_little_endian_elf(addr))
		image_flags |= OCTEON_BOOT_DESC_LITTLE_ENDIAN;

	printf("## Loading %s-endian Linux kernel with entry point: 0x%08llx ...\n",
	       image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN ? "little" : "big",
	       entry_addr);

#ifdef DEBUG
	debug("Setting up boot descriptor block with core mask:\n");
	cvmx_coremask_print(&core_mask);
	debug("entry address: 0x%llx\n", entry_addr);
#endif
	/* Add coremask to global mask of cores that have been set up and are
	 * runable
	 */
	cvmx_coremask_or(&coremask_to_run, &coremask_to_run, &core_mask);
	
	if (octeon_setup_boot_desc_block(&core_mask, argc, argv, entry_addr,
					 0, 0, boot_flags, 0,
					 image_flags, 0, &empty_cm,
					 -1)) {
		puts("ERROR setting up boot descriptor block, core_mask:\n");
		cvmx_coremask_print(&core_mask);
		return -1;
	}
#ifdef DEBUG
	debug("Setting up boot vector to 0x%p, core mask:\n",
	      (void *)&start_linux);
	cvmx_coremask_print(&core_mask);
#endif

	if (cvmx_coremask_is_core_set(&core_mask, 0) || forceboot)
		octeon_bootloader_shutdown();

	if (octeon_setup_boot_vector((uint32_t) start_linux, &core_mask)) {
		puts("ERROR setting boot vectors, core_mask:\n");
		cvmx_coremask_print(&core_mask);
		return -1;
	}

	/* Check environment for forceboot flag */
	if (getenv("forceboot"))
		forceboot |= 1;

	setup_linux_app_boot_info(&core_mask);

	puts("Bootloader: Done loading app on coremask:\n");
	cvmx_coremask_print(&core_mask);
	/* Start other cores, but only if core zero is in mask */
	if (cvmx_coremask_is_core_set(&core_mask, 0) || forceboot) {
		puts("Starting cores:\n");
		cvmx_coremask_print(&coremask_to_run);
		start_cores(&coremask_to_run);	/* Does not return */
	}

	return rcode;
}

/* Returns entry point from elf 32 image */
unsigned long elf32_entry(unsigned long addr)
{
	Elf32_Ehdr *ehdr;	/* Elf header structure pointer     */
	ehdr = (Elf32_Ehdr *) addr;
	return ehdr->e_entry;
}

int do_bootoctelf(cmd_tbl_t * cmdtp, int flag, int argc, char *const argv[])
{
	unsigned long addr = 0;		/* Address of the ELF image     */
	uint64_t entry_addr = 0;
	unsigned long mem_size = 0;	/* size of memory for ELF image */
	int i;
	struct octeon_boot_args boot_args;
	static uint32_t saved_exception_base = 0;

	cvmx_coremask_t core_mask;
	uint32_t boot_flags = 0;
	/* -------------------------------------------------- */
	int rcode = 0;
	int num_cores = 0;
	int arg_start = 1;

	cvmx_coremask_clear_all(&core_mask);
	cvmx_coremask_set_core(&core_mask, 0);
#if CONFIG_OCTEON_SIM_SW_DIFF
	/* Default is to run on all cores on simulator */
	cvmx_coremask_set64(&core_mask, cvmx_read_csr(CVMX_CIU_FUSE));
#endif

	if (argc < 2) {
		addr = load_addr;
	} else if (argc >= 3) {
		if (isxdigit(argv[arg_start][0]) &&
		    (isxdigit(argv[arg_start][1] ||
		     argv[arg_start][1] == 'x' ||
		     argv[arg_start][1] == 'X') ||
		     argv[arg_start][1] == '\0')) {
			addr = simple_strtoul(argv[arg_start], NULL, 16);
			arg_start++;
		}
		if (!addr)
			addr = load_addr;
		if (isxdigit(argv[arg_start][0]) &&
		    (isxdigit(argv[arg_start][1]) ||
		     argv[arg_start][1] == 'x' ||
		     argv[arg_start][1] == 'X' ||
		     argv[arg_start][1] == '\0')) {
			mem_size = simple_strtoul(argv[2], NULL, 16);
			arg_start++;
		}
	} else {	/* argc == 2 */
		if (isxdigit(argv[arg_start][0]) &&
		    (isxdigit(argv[arg_start][1]) ||
		     argv[arg_start][1] == 'x' ||
		     argv[arg_start][1] == 'X' ||
		     argv[arg_start][1] == '\0')) {
			addr = simple_strtoul(argv[1], NULL, 16);
			arg_start++;
		}
		if (!addr)
			addr = load_addr;
		mem_size = 0;
		printf("Warning: No memory size provided, this application "
		       "won't co-exist with simple exec. applications or "
		       "Linux.\n");
	}

	i = octeon_parse_bootopts(argc - arg_start, argv + arg_start,
				  BOOTOCTELF, &boot_args);
	if (boot_args.endbootargs) {
		argv += (i + arg_start);
		argc -= (i + arg_start);
	}
	/* numcores specification overrides a coremask on the same command line
	 */

	/* Remove cores from coremask based on environment variable stored
	 * in flash
	 */
	cvmx_coremask_copy(&core_mask, &boot_args.coremask);
	if (cvmx_coremask_is_empty(&core_mask)) {
		printf("Coremask is empty after coremask_override mask.  "
		       "Nothing to do.\n");
		return 0;
	} else if (validate_coremask(&core_mask) != 0) {
		printf("Invalid coremask.\n");
		return 1;
	}

	/* Force the exception base to be zero (IOS needs this), but still
	 * attempt to play well with others.
	 */
	if (!saved_exception_base) {
		saved_exception_base = cur_exception_base;
		cur_exception_base = 0;
	}

	num_cores = cvmx_coremask_get_core_count(&core_mask);
	if (num_cores != 1) {
		printf("Warning: Coremask has %d cores set.  Application must "
		       "support multiple cores for proper operation.\n",
		       num_cores);
	}

	if (!valid_elf_image(addr))
		return 1;

	if (cvmx_coremask_intersects(&core_mask, &coremask_to_run)) {
		puts("ERROR: Can't load code on core twice! (provided coremask:\n");
		cvmx_coremask_print(&core_mask);
		puts("overlaps previously loaded coremask:\n");
		cvmx_coremask_print(&coremask_to_run);
		return -1;
	}

	if (((Elf32_Ehdr *) addr)->e_ident[EI_CLASS] == ELFCLASS32) {
		if (mem_size) {
			uint32_t mem_base;
			/* Mask of kseg0 bit, we want phys addr */
			mem_base = elf32_entry(addr) & 0x7fffffff;
			/* Round down to  Mbyte boundary */
			mem_base &= ~0xfffff;
			printf("Allocating memory for ELF: Base addr, 0x%x, "
			       "size: 0x%lx\n", mem_base, mem_size);

			/* For ELF images the user must specify the size of
			 * memory above the entry point of the elf image that
			 * is reserved for the ELF.  Here we allocate that
			 * memory so that other software can co-exist with with
			 * the ELF image.
			 */
			if (0 > cvmx_bootmem_phy_alloc(mem_size, mem_base,
						       mem_base + mem_size,
						       0, 0)) {
				printf("Error allocating 0x%lx bytes of memory "
				       "at base address 0x%x for ELF image!\n",
				       mem_size, mem_base);
				return 0;
			}
		}
		entry_addr = load_elf_image(addr);
		/* make sure kseg0 addresses are sign extended */
		if ((int32_t) entry_addr < 0)
			entry_addr |= 0xffffffff00000000ull;
	} else {		/* valid_elf_image ensures only 32 and 64 bit class values */
		puts("ERROR: only loading of 32 bit  ELF images is supported\n");
	}

	if (!entry_addr) {
		puts("## ERROR loading File!\n");
		return -1;
	}

	printf("## Loading ELF image with entry point: 0x%08llx ...\n",
	       entry_addr);
#ifdef DEBUG
	debug("Setting up boot descriptor block with core mask:\n");
	cvmx_coremask_print(&core_mask);
	debug("entry addr 0x%llx\n", entry_addr);
#endif

	/* ELF images uses same setup as linux */
	if (octeon_setup_boot_desc_block(&core_mask, argc, argv, entry_addr, 0,
					 0, boot_flags, 0,
					 OCTEON_BOOT_DESC_IMAGE_LINUX, 0, 0,
					 -1)) {
		puts("ERROR setting up boot descriptor block, core_mask:\n");
		cvmx_coremask_print(&core_mask);
		return -1;
	}

	if (octeon_setup_boot_vector((uint32_t) start_elf, &core_mask)) {
		puts("ERROR setting boot vectors, core_mask:\n");
		cvmx_coremask_print(&core_mask);
		return -1;
	}

	/* Add coremask to global mask of cores that have been set up and are
	 * runable
	 */
	cvmx_coremask_or(&coremask_to_run, &coremask_to_run, &core_mask);

	/* Check environment for forceboot flag */
	if (getenv("forceboot"))
 		boot_args.forceboot |= 1;

	puts("Bootloader: Done loading app on coremask:\n");
	cvmx_coremask_print(&core_mask);
	/* Start other cores, but only if core zero is in mask */
	if (cvmx_coremask_is_core_set(&core_mask, 0) || boot_args.forceboot) {
		octeon_bootloader_shutdown();
		puts("Starting cores:\n");
		cvmx_coremask_print(&coremask_to_run);
		start_cores(&coremask_to_run);	/* Does not return */
	}

	cur_exception_base = saved_exception_base;
	return rcode;
}

void start_os_common(void)
{

	int core_num;
	uint64_t val;
	boot_info_block_t *boot_info_ptr;

	core_num = get_core_num();

	/* Set local cycle counter based on global counter in IPD */
	octeon_init_cvmcount();

	/* look up boot descriptor block for this core */
	boot_info_ptr = &boot_info_block_array[core_num];

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
#ifdef PROFILE
	/* Define this to disable conditional clocking for acurate profiling */
	val |= (1ull << 17);	/* DISCE for perf counters */
#endif
	set_cop0_cvmctl_reg(val & 0xfffff3);

	/* Disable core stalls on write buffer full */
	if (OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2)) {
		val = get_cop0_cvmmemctl_reg();
		val |= 1ull << 29;	/* set diswbfst */
#ifdef PROFILE
		val |= 1ull << 19;	/* Mclkalways, for perf counters */
#endif

#if 0
		/* 14:11 is wb thresh */
		val &= ~((0xf) << 11);
		val |= ((0xc) << 11);

		/* set lines of scratch */
		val &= ~0x3f;
		val |= 0x2;
#endif
		set_cop0_cvmmemctl_reg(val);
	}
	/* Sync up cores before starting main */
	octeon_sync_cores();
}

void start_linux(void)
{
	int core_num = get_core_num();
	struct octeon_boot_descriptor *cur_boot_desc = &boot_desc[core_num];
	uint64_t boot_desc_addr;
	boot_info_block_t *boot_info_ptr;
	uint64_t arg0;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t le_flag;
	uint64_t addr;
#ifdef DEBUG
	static cvmx_spinlock_t lock = { .value = OCTEON_SPINLOCK_UNLOCKED_VAL };
#endif

	start_os_common();

	/* look up boot descriptor block for this core */
	boot_desc_addr =
	    MAKE_XKPHYS(uboot_tlb_ptr_to_phys(&boot_desc[core_num]));
	boot_info_ptr = &boot_info_block_array[core_num];

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 * a0 = argc
	 * a1 = argv (32 bit physical addresses, not pointers)
	 * a2 = init core
	 * a3 = boot descriptor address
	 * a4/t0 = entry point (only used by assembly stub)
	 */
	/* Branch to start of linux kernel, passing args */
	/* NOTE - argv array is not 64 bit pointers, but is 32 bit addresses */
	/* Here we need to branch to a stub in ASM to clear the TLB entry used
	 * by u-boot.  This stub is run from an XKPHYS address
	 */

	arg0 = cur_boot_desc->argc;
	arg1 = (int32_t) cur_boot_desc->argv;
	arg2 = !!(cur_boot_desc->flags & BOOT_FLAG_INIT_CORE);
	arg3 = boot_desc_addr;
	arg4 = boot_info_ptr->entry_point;
	le_flag = (boot_info_ptr->flags & BOOT_INFO_FLAG_LITTLE_ENDIAN) ? 1 : 0;
	addr =
	    MAKE_XKPHYS(uboot_tlb_ptr_to_phys((void *)&asm_launch_linux_entry_point));

#ifdef DEBUG
	cvmx_spinlock_lock(&lock);
	debug("%s: argc=0x%llx, argv=0x%llx, arg2=0x%llx, arg3=0x%llx, arg4=0x%llx, addr=0x%llx\n",
	      __func__, arg0, arg1, arg2, arg3, arg4, addr);
	debug("%s: asm_launch_linux_entry_point=%p\n", __func__,
	      (void *)&asm_launch_linux_entry_point);
	debug("%s: boot_desc[%u]=%p, phys addr=0x%llx\n", __func__, core_num,
	      &boot_desc[core_num], boot_desc_addr);
	cvmx_spinlock_unlock(&lock);
	octeon_sync_cores();
#endif

	/* Set up TLB registers to clear desired entry.  The actual
	 * tlbwi instruction is done in ASM when running from unmapped DRAM
	 */
	write_64bit_c0_entrylo0(0);
	write_c0_pagemask(0);
	write_64bit_c0_entrylo1(0);
	write_64bit_c0_entryhi(0xFFFFFFFF91000000ull);
	write_c0_index(get_num_tlb_entries() - 1);

	asm volatile ("       .set push         \n"
		      "       .set mips64       \n"
		      "       .set noreorder    \n"
		      "       move  $4, %[arg0] \n"
		      "       move  $5, %[arg1] \n"
		      "       move  $6, %[arg2] \n"
		      "       move  $7, %[arg3] \n"
		      "       move  $8, %[arg4] \n"
		      "       move  $2, %[le_flag] \n"
		      "       j     %[addr]     \n"
		      "       nop               \n"
		      "       .set pop          \n"
		      : :
		      [arg0] "r"(arg0),
		      [arg1] "r"(arg1),
		      [arg2] "r"(arg2),
		      [arg3] "r"(arg3),
		      [arg4] "r"(arg4),
		      [le_flag] "r"(le_flag),
		      [addr] "r"(addr)
		      : "$2", "$4", "$5", "$6", "$7", "$8");
	/* Make sure that GCC doesn't use the explicit registers that we want
	 * to use
	 */
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
	__builtin_unreachable();
#endif
}

void start_elf(void)
{
	int core_num = get_core_num();
	boot_info_block_t *boot_info_ptr;
	uint64_t boot_desc_addr;
	uint64_t arg0;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t addr;
#ifdef DEBUG
	static cvmx_spinlock_t lock = { .value = OCTEON_SPINLOCK_UNLOCKED_VAL };
#endif

	start_os_common();

	/* look up boot descriptor block for this core */
	boot_desc_addr = MAKE_XKPHYS(uboot_tlb_ptr_to_phys(&boot_desc[core_num]));
	boot_info_ptr = &boot_info_block_array[core_num];

	/*
	 * a0 = 0
	 * a1 = boot descriptor address
	 * a2 = 0
	 * a3 = 0
	 * a4/t0 = entry point (only used by assembly stub)
	 */
	/* Branch to entry point of ELF file, passing args */
	/* Here we need to branch to a stub in ASM to clear the TLB entry used
	 * by u-boot.  This stub is run from an XKPHYS address
	 */
	arg0 = 0;
	arg1 = boot_desc_addr;
	arg2 = 0;
	arg3 = 0;
	arg4 = boot_info_ptr->entry_point;
	addr = MAKE_XKPHYS(uboot_tlb_ptr_to_phys((void *)&asm_launch_linux_entry_point));

#ifdef DEBUG
	cvmx_spinlock_lock(&lock);
	debug("%s: argc=0x%llx, argv=0x%llx, arg2=0x%llx, arg3=0x%llx, arg4=0x%llx, addr=0x%llx\n",
	      __func__, arg0, arg1, arg2, arg3, arg4, addr);
	debug("%s: asm_launch_linux_entry_point=%p\n", __func__,
	      (void *)&asm_launch_linux_entry_point);
	debug("%s: boot_desc[%u]=%p, phys addr=0x%llx\n", __func__, core_num,
	      &boot_desc[core_num], boot_desc_addr);
	cvmx_spinlock_unlock(&lock);
	octeon_sync_cores();
#endif

	/* Set up TLB registers to clear desired entry.  The actual
	 * tlbwi instruction is done in ASM when running from unmapped DRAM
	 */
	write_64bit_c0_entrylo0(0);
	write_c0_pagemask(0);
	write_64bit_c0_entrylo1(0);
	write_64bit_c0_entryhi(0xFFFFFFFF91000000ull);
	write_c0_index(get_num_tlb_entries() - 1);

	asm volatile ("       .set push         \n"
		      "       .set mips64       \n"
		      "       .set noreorder    \n"
		      "       move  $4, %[arg0] \n"
		      "       move  $5, %[arg1] \n"
		      "       move  $6, %[arg2] \n"
		      "       move  $7, %[arg3] \n"
		      "       move  $8, %[arg4] \n"
		      "       j     %[addr]     \n"
		      "       nop               \n"
		      "       .set pop          \n"
		      : :
		      [arg0] "r"(arg0),
		      [arg1] "r"(arg1),
		      [arg2] "r"(arg2),
		      [arg3] "r"(arg3),
		      [arg4] "r"(arg4),
		      [addr] "r"(addr)
		      :"$4", "$5", "$6", "$7", "$8");
	/* Make sure that GCC doesn't use the explicit registers that we want
	 * to use
	 */
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
	__builtin_unreachable();
#endif
}

/**
 * Parse the ELF file and do bootmem allocs for
 * the memory.
 *
 * @param addr   address of ELF image
 *
 * @return 0 on failure
 *         !0 on success
 */
static int alloc_elf32_image(unsigned long addr)
{
	Elf32_Ehdr *ehdr;	/* Elf header structure pointer     */
	Elf32_Phdr *phdr;	/* Segment header ptr               */
	int i;

	/* -------------------------------------------------- */

	ehdr = (Elf32_Ehdr *) cvmx_phys_to_ptr(addr);

	for (i = 0; i < ehdr->e_phnum; ++i) {
		phdr = (Elf32_Phdr *) cvmx_phys_to_ptr(addr + ehdr->e_phoff +
						       (i * sizeof(Elf32_Phdr)));
		if (phdr->p_type != PT_LOAD) {
			printf("Skipping non LOAD program header (type 0x%x)\n",
			       phdr->p_type);
		} else if (phdr->p_memsz > 0) {
			if (0 > cvmx_bootmem_phy_alloc(phdr->p_memsz,
						       (uint32_t) phdr->p_paddr & 0x7fffffff,
						       0, 0, 0)) {
				printf("Error allocating 0x%x bytes for elf image at physical address 0x%x!\n",
				       (uint32_t)phdr->p_memsz, (uint32_t)(phdr->p_paddr & 0x7fffffff));
				return (0);
			}
			printf("Allocated memory for ELF segment: addr: 0x%x, size 0x%x\n",
			       (uint32_t) phdr->p_paddr & 0x7fffffff,
			       (uint32_t) phdr->p_memsz);
		}
	}
	return 1;
}

/**
 * Parse the Linux ELF file and do bootmem allocs for
 * the memory.  This does special handling for mapped kernels that
 * can be loaded at any memory location, as apposed to unmapped kernels that
 * must be loaded at the physical address in the ELF header.
 *
 * For mapped kernels, we do an unconstrained bootmem alloc, and then
 * update the ELF header in memory to reflect where the segments should be
 * copied to.
 * The generic ELF loading code that is used after this relies on the ELF header
 * to describe the physical addresses that should be used.
 *
 * NOTE: We update the physical load address and the entry point address in the
 * elf image so the the ELF loading routine will do the correct thing.
 *
 * @param addr   address of ELF image
 *
 * @return 0 on failure
 *         !0 on success (the entry address to use)
 */
static uint64_t alloc_elf64_linux_image(unsigned long addr, int64_t *alloc_addr_override,
                                        struct cvmx_bootmem_named_block_desc *linux_named_block)
{
	Elf64_Ehdr *ehdr;	/* Elf header structure pointer     */
	Elf64_Phdr *phdr;	/* Segment header ptr */
	int i, num_pt_load;
	uint64_t phys_addr, memsz;
	int mapped_segment = 0;
	int64_t alloc_addr;
	int64_t  entry_addr = 0;
	struct elf_accessors *a = get_elf_accessors(addr);

	/* -------------------------------------------------- */

	ehdr = (Elf64_Ehdr *) addr;

	num_pt_load = 0;
	for (i = 0; i < a->w16(ehdr->e_phnum); ++i) {
		phdr = (Elf64_Phdr *) (uint32_t) (addr + a->w64(ehdr->e_phoff) +
						  (i * sizeof(Elf64_Phdr)));
		memsz = a->w64(phdr->p_memsz);
		if (memsz > 0 && a->w32(phdr->p_type) == PT_LOAD)
			num_pt_load++;
	}

	/*
	 * WARNING: here we do casts from 64 bit addresses to 32 bit
	 * pointers.  This only works if the 64 addresses are in the
	 * 32 bit compatibility space.
	 */
	for (i = 0; i < a->w16(ehdr->e_phnum); ++i) {
		phdr = (Elf64_Phdr *) (uint32_t) (addr + a->w64(ehdr->e_phoff) +
						  (i * sizeof(Elf64_Phdr)));
		memsz = a->w64(phdr->p_memsz);
		if (memsz > 0 && a->w32(phdr->p_type) == PT_LOAD) {
			uint64_t vaddr = a->w64(phdr->p_vaddr);
			uint64_t paddr = a->w64(phdr->p_paddr);
			if (((vaddr >= 0xFFFFFFFFc0000000ULL &&
			      vaddr <= 0xFFFFFFFFFFFFFFFFULL)
			     || (vaddr >= 0xC000000000000000ULL &&
				 vaddr < 0xFFFFFFFF80000000ULL)
			     || (vaddr >= 0x0000000000000000ULL &&
				 vaddr < 0x8000000000000000ULL)
			    ))
				mapped_segment = 1;
			else
				mapped_segment = 0;

			/* change virtual xkphys address to plain physical
			 * address that bootmem functions operate on.
			 */
			phys_addr = octeon_fixup_xkphys(paddr);
			/* Only strip bit 63 if it is the only high bit set, for
			 * compatability addresses, we need to do more
			 */
			if ((phys_addr & (0xffffffffull << 32)) ==
			    (0xffffffffull << 32))
				phys_addr &= 0x7fffffffull;
			else
				phys_addr &= ~(0x1ull << 63);

			if (linux_named_block) {
				uint64_t named_start = linux_named_block->base_addr;
				uint64_t named_end = linux_named_block->base_addr +
					linux_named_block->size;

				/* Check only when not mapped */
				if ((!mapped_segment) &&
				    (phys_addr < named_start ||
				     (phys_addr + memsz) > named_end)) {
					printf("Image doesn't fit in specified named block\n");
					/* Reset before return */
					linux_named_block = NULL;
					return 0;
				}
				/* Fix up entry_addr only if it lies in this segment */
				if (a->w64(ehdr->e_entry) >= paddr &&
				    a->w64(ehdr->e_entry) <= (paddr + memsz)) {
                                        entry_addr = a->w64(ehdr->e_entry);
                                        alloc_addr = named_start;
				}
                        } else if (!mapped_segment || num_pt_load > 1) {
				printf("Allocating memory for ELF segment: "
				       "addr: 0x%llx (adjusted to: 0x%llx), "
				       "size 0x%x\n", paddr, phys_addr,
				       (uint32_t) memsz);
				alloc_addr =
				    cvmx_bootmem_phy_alloc(memsz, phys_addr, 0, 0, 0);
				entry_addr = a->w64(ehdr->e_entry);
			} else {
				/* We have a mapped kernel, so allocate anywhere */
				uint64_t alignment = 0x100000;
				uint64_t limit_hi, limit_lo;

				/* mapped kernel must be in 32-bit memory */
				limit_hi = 1 << 30;
				limit_lo = 1 << 28;

				/* Get power of 2 >= memory size, then adjust
				 * for MIPS dual-page TLB entries
				 */
				while (alignment < memsz)
					alignment <<= 1;
				alignment = alignment >> 1;
				printf("Allocating memory for mapped kernel "
				       "segment, alignment: 0x%llx\n",
				       alignment);
				alloc_addr =
				    cvmx_bootmem_phy_alloc(memsz,
						limit_lo, limit_hi,
						alignment, 0);
				if (alloc_addr > 0) {
					/* If we succeed, then we need to fixup
					 * the ELF header so the loader will do
					 * the right thing.
					 */
					uint64_t entry_offset = a->w64(ehdr->e_entry) - paddr;
					paddr = alloc_addr;
					/*
					 * We are going to be jumping
					 * here, so it needs to be the
					 * xkphys address.
					 */
					entry_addr = paddr + entry_offset + 0x8000000000000000ull;
					*alloc_addr_override = alloc_addr;
				}
				printf("Allocated memory for ELF segment: addr: 0x%llx, size 0x%x\n",
				       paddr, (uint32_t)memsz);
			}
			if (0 > alloc_addr) {
				puts("Error allocating memory for elf image!\n");
				return 0;
			}
		}
	}
	return entry_addr;
}

U_BOOT_CMD(bootoctlinux, 32, 0, do_bootoctlinux,
	   "Boot from a linux ELF image in memory",
	   "elf_address [coremask=mask_to_run | numcores=core_cnt_to_run] "
	   "[forceboot] [skipcores=core_cnt_to_skip] [namedblock=name] [endbootargs] [app_args ...]\n"
	   "elf_address - address of ELF image to load. If 0, default load address\n"
	   "              is  used.\n"
	   "coremask    - mask of cores to run on.  Anded with coremask_override\n"
	   "              environment variable to ensure only working cores are used\n"
	   "numcores    - number of cores to run on.  Runs on specified number of cores,\n"
	   "              taking into account the coremask_override.\n"
	   "skipcores   - only meaningful with numcores.  Skips this many cores\n"
	   "              (starting from 0) when loading the numcores cores.\n"
	   "              For example, setting skipcores to 1 will skip core 0\n"
	   "              and load the application starting at the next available core.\n"
	   "forceboot   - if set, boots application even if core 0 is not in mask\n"
	   "namedblock	- specifies a named block to load the kernel\n"
	   "endbootargs - if set, bootloader does not process any further arguments and\n"
	   "              only passes the arguments that follow to the kernel.\n"
	   "              If not set, the kernel gets the entire commnad line as\n"
	   "              arguments.\n" "\n");

U_BOOT_CMD(bootoctelf, 32, 0, do_bootoctelf,
	   "Boot a generic ELF image in memory. NOTE: This command does not\n"
	   "              support simple executive applications, use bootoct for those.",
	   "elf_address [mem_size [coremask=mask_to_run | numcores=core_cnt_to_run] "
	   "[skipcores=core_cnt_to_skip] [forceboot] ]\n"
	   "elf_address - address of ELF image to load.  If 0, default load address is\n"
	   "              used.\n"
	   "mem_size    - amount of memory to reserve for ELF file.  Starts at ELF\n"
	   "              entry point rounded down to 1 MByte alignment.\n"
	   "              No memory allocated if not specified.  (Program will not\n"
	   "              coexist with simple executive applications or Linux if memory\n"
	   "              is not allocated properly.\n"
	   "coremask    - mask of cores to run on.  Anded with coremask_override\n"
	   "              environment variable to ensure only working cores are used.\n"
	   "              Note: Most ELF files are only suitable for booting on a\n"
	   "              single core.\n"
	   "forceboot   - if set, boots application even if core 0 is not in mask\n"
	   "numcores    - number of cores to run on.  Runs on specified number of\n"
	   "              cores, taking into account the coremask_override.\n"
	   "skipcores   - only meaningful with numcores.  Skips this many cores\n"
	   "              (starting from 0) when loading the numcores cores.\n"
	   "              For example, setting skipcores to 1 will skip core 0\n"
	   "              and load the application starting at the next available core.\n"
	   "\n");

