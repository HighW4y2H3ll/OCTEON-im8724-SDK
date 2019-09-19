/***********************license start************************************
 * Copyright (c) 2011 Cavium, Inc. (support@cavium.com). All rights reserved.
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
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/

/**
 *
 * $Id: octeon_exec.c 100960 2014-07-03 06:15:04Z awilliams $
 *
 */
/*
 * Platform specific override functions used for the go and various exec
 * commands
 */

#include <common.h>
#include <config.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_dram.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access-native.h>
#include <asm/mipsregs.h>
#include <asm/processor.h>

extern void asm_launch_linux_entry_point(uint64_t arg0, uint64_t arg1,
					 uint64_t arg2, uint64_t arg3,
					 void (*entry) (int, char *const[]));
#ifdef CONFIG_API
DECLARE_GLOBAL_DATA_PTR;

int do_go_exec_uboot_app(ulong (*entry)(int, char *const[]), int argc,
			 char *const argv[])
{
	int64_t addr;
	uint64_t page_vaddr;
	uint64_t lo1;
	struct octeon_uboot_native_app_header *header;
	uint32_t page_size;
	int num_pages = 1;
	int rc;
	uint32_t size;
	uint32_t copy_size;

	header = (struct octeon_uboot_native_app_header *)entry;
	debug("U-Boot native application found!\n"
	      "signature:         %s\n"
	      "version:           %d.%d\n"
	      "start address:     0x%x\n"
	      "text start:        0x%x\n"
	      "text end:          0x%x\n"
	      "GOT start:         0x%x\n"
	      "GOT end:           0x%x\n"
	      "ro data start:     0x%x\n"
	      "ro data end:       0x%x\n"
	      "data start:        0x%x\n"
	      "data end:          0x%x\n"
	      "sdata start:       0x%x\n"
	      "sdata end:         0x%x\n"
	      "bss start address: 0x%x\n"
	      "bss end address: 0x%x\n"
	      "end address:       0x%x\n",
	      header->app_signature,
	      header->app_version_major, header->app_version_minor,
	      header->app_start,
	      header->app_text_start, header->app_text_end,
	      header->app_got_start, header->app_got_end,
	      header->app_ro_data_start, header->app_ro_data_end,
	      header->app_data_start, header->app_data_end,
	      header->app_sdata_start, header->app_sdata_end,
	      header->app_bss_start, header->app_bss_end,
	      header->app_end);

	size = header->app_end - header->app_start;
	copy_size = header->app_sdata_end - header->app_start;
	/* Make sure size is sane */
	if (size > 16 * 1024 * 1024) {
		printf("Native U-Boot application size 0x%x is too large\n",
		       size);
		return -1;
	}
#if CONFIG_SYS_MALLOC_LEN > 14 * 1024 * 1024
# error revisit the starting address here for a larger heap
#endif
	if (header->app_start < 0xc0100000 || header->app_start > 0xdffff000 ||
	    header->app_end < 0xc0100000 || header->app_end > 0xdfffffff)  {
		puts("U-Boot native application start or end address out of range\n");
		return -1;
	}

	page_vaddr = header->app_start | 0xffffffff00000000ULL;

	/* Calculate the page size we need to map the application */
	page_size = 4096;
	while (page_size * num_pages < size) {
		num_pages++;
		if (num_pages > 2) {
			num_pages = 1;
			page_size *= 4;
		}
	};

	/* Make sure that the app start address is on a page boundary */
	if (header->app_start & (page_size - 1)) {
		printf("Cannot allocate pages for current start address with a page size of %d\n",
		       page_size);
		return -1;
	}

	octeon_dram_clear_access_errors(true);

	/* Reserve lower 1 MB after freeing all temp named blocks */
	rc = cvmx_bootmem_reserve_memory(0,
					 OCTEON_RESERVED_LOW_BOOT_MEM_SIZE,
					 "__low_reserved", 0);
	if (!rc) {
		puts("Error reserving low 1MB of memory\n");
		return -1;
	}

	debug("size: 0x%x, page size: 0x%x, num_pages: %d\n", size, page_size,
	      num_pages);

	addr = cvmx_bootmem_phy_named_block_alloc(page_size * num_pages,
						  0x80000000, 0,
						  page_size,
						  "u-boot-app",
						  CVMX_BOOTMEM_FLAG_END_ALLOC);
	if (addr < 0) {
		debug("Could not allocate 0x%llx bytes in high memory\n",
		      page_size * num_pages);
		addr = cvmx_bootmem_phy_named_block_alloc(page_size * num_pages,
							  0x20000000, 0,
							  page_size,
							  "u-boot-app",
							  CVMX_BOOTMEM_FLAG_END_ALLOC);
	} else {
		debug("Hi allocate return address: 0x%llx\n", addr);
	}

	if (addr < 0) {
		debug("Could not allocate 0x%llx bytes above boot bus\n",
		      page_size * num_pages);
		addr = cvmx_bootmem_phy_named_block_alloc(page_size * num_pages,
							  0, 0,
							  page_size,
							  "u-boot-app",
							  CVMX_BOOTMEM_FLAG_END_ALLOC);
		debug("Low allocate return address: 0x%llx\n", addr);
	}

	if (addr < 0) {
		puts("Cannot allocate memory for app.\n");
		return 0;
	}

	debug("Allocating app memory at physical address 0x%llx\n", addr);

	/* Fill in TLB entry */
	lo1 = (num_pages == 1) ? 1 : (((addr + page_size) >> 12) << 6) | 7;

	debug("Mapping %d physical address 0x%llx to 0x%llx with page size %d\n",
	      num_pages, addr, page_vaddr, page_size);
	write_one_tlb64(0, (page_size | (page_size - 1)) & ~(4096 - 1),
			page_vaddr, ((addr >> 12) << 6)  | 7, lo1);

	debug("Copying app to virtual address 0x%08llx\n",
	      page_vaddr & 0xffffffff);
	memcpy((void *)header->app_start, entry, copy_size);

	debug("Initializing native API\n");

	api_init();

	/* Copy U-Boot environment */
	octeon_setup_ram_env();

	/* Fill in our portion of the header */
	header = (struct octeon_uboot_native_app_header *)(header->app_start);
	header->loader_version_major =
				OCTEON_UBOOT_NATIVE_APP_LOADER_VERSION_MAJOR;
	header->loader_version_minor =
				OCTEON_UBOOT_NATIVE_APP_LOADER_VERSION_MINOR;
	header->loader_flags = 0;
	header->loader_api_sig_addr = (uint32_t)gd->arch.api_sig_addr;

#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_disable();
#endif

	/* Clear the BSS (should already be clear) */
	if (header->app_bss_start < header->app_end) {
		debug("Clearing %d bytes for bss starting at 0x%x\n",
		      header->app_bss_end - header->app_bss_start,
		      header->app_bss_start);
		memset((void *)header->app_bss_start, 0,
		       header->app_bss_end - header->app_bss_start);
	}

	CVMX_ICACHE_INVALIDATE;

	entry = (ulong (*)(int, char * const []))header->app_start;

	debug("%s: Calling native app at 0x%p\n", __func__, entry);
#if 1
	rc = entry(argc, argv);

	debug("Clearing TLB\n");
	write_one_tlb64(0, 0, 0, 0, 0);

	cvmx_bootmem_free_named("u-boot-app");

	printf("App returned %d\n", rc);
#else
	rc = 0;
#endif
	return CMD_RET_SUCCESS;
}
#endif

unsigned long do_go_exec(ulong (*entry) (int, char *const[]), int argc,
			 char *const argv[])
{
	uint64_t arg0, arg1, arg4;
	uint64_t asm_addr;
#ifdef CONFIG_API
	struct octeon_uboot_native_app_header *uboot_native_header;

	debug("Looking for native U-Boot application\n");
	/* Look for native U-Boot app */
	uboot_native_header = (struct octeon_uboot_native_app_header *)entry;

#ifdef DEBUG
	debug("  nop: 0x%x\n", uboot_native_header->app_nop_insn);
	debug("  signature: \"%s\"\n", uboot_native_header->app_signature);
#endif
	if (uboot_native_header->app_nop_insn == 0 &&
	    !memcmp(uboot_native_header->app_signature,
		    OCTEON_UBOOT_NATIVE_APP_SIGNATURE,
		    sizeof(OCTEON_UBOOT_NATIVE_APP_SIGNATURE)) &&
	    uboot_native_header->loader_api_sig_addr == 0) {
		/* We found a native U-Boot app */
		debug("Found native U-Boot APP\n");
		return do_go_exec_uboot_app(entry, argc, argv);
	}
#endif
	arg0 = argc;
	/* Cast away const.... */
	arg1 = MAKE_XKPHYS(uboot_tlb_ptr_to_phys((void *)&argv[0]));
	/* want sign extension... */
	arg4 = (int32_t) MAKE_KSEG0((uint32_t) entry);
	asm_addr =
	    MAKE_XKPHYS(uboot_tlb_ptr_to_phys(&asm_launch_linux_entry_point));

	octeon_restore_std_mips_config();

	/* Set up the environment in RAM to pass off to next application
	 * or bootloader.  Required for NAND boot.
	 */
	octeon_setup_ram_env();
	/* Set up TLB registers to clear desired entry.  The actual tlbwi
	 * instruction is done in ASM when running from unmapped DRAM
	 * Don't use write_one_tlb64, as this does the actual TLB update,
	 * which will remove the mapping we are currently running from.
	 */
	write_64bit_c0_entrylo0(0);
	write_c0_pagemask(0);
	write_64bit_c0_entrylo1(0);
	write_64bit_c0_entryhi(0xFFFFFFFF91000000ull);
	write_c0_index(get_num_tlb_entries() - 1);

	asm volatile ("	.set push		\n"
		      "	.set mips64		\n"
		      "	.set noreorder		\n"
		      "	move	$4, %[arg0]	\n"
		      "	move	$5, %[arg1]	\n"
		      " move	$2, $0		\n" /* Big endian */
		      "	j	%[addr]		\n"
		      "	move	$8, %[arg4]	\n"
		      "	.set pop		\n"
		      : :
		      [arg0] "r"(arg0),
		      [arg1] "r"(arg1),
		      [arg4] "r"(arg4),
		      [addr] "r"(asm_addr)
		      : "$2", "$4", "$5", "$8");
	printf("### Application terminated, rc not available\n");
	return entry(argc, argv);
}

/**
 * Override for do_bootelf_exec in order to handle platform specific
 * settings.
 */
unsigned long do_bootelf_exec (ulong (*entry) (int, char *const[]),
			       int argc, char *const argv[])
{
	unsigned long ret;

	/* Restore to a sane MIPS configuration, i.e. disable
	 * watchdog, USB, networking and other cleanup.
	 */
	octeon_restore_std_mips_config();
	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	ret = entry(argc, argv);

	return ret;
}
