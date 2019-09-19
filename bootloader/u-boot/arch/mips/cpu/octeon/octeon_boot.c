/*
 * (C) Copyright 2004-2013
 * Cavium Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/**
 *
 * $Id: octeon_boot.c 165791 2017-09-01 19:13:10Z cchavva $
 *
 */

#if !defined(__U_BOOT__)
# include "bootoct.h"
# include <endian.h>
# include <string.h>
# define swab64 __bswap_64
#else
# include <common.h>
# include <malloc.h>
# include <exports.h>
# include <watchdog.h>
# include <fdt_support.h>
# include <linux/ctype.h>
# include <asm/arch/octeon_eeprom_types.h>
# include <asm/arch/octeon_dram.h>
# include <asm/arch/cvmx-bootloader.h>
# include <environment.h>
# include <asm/mipsregs.h>
# include <asm/processor.h>
# include <asm/arch/cvmx-app-init.h>
# include <asm/arch/octeon_boot_bus.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/octeon-model.h>
# include <asm/arch/lib_octeon_shared.h>
# include <asm/arch/lib_octeon.h>
# include <asm/arch/cvmx-access.h>
# include <asm/arch/octeon-boot-info.h>
# include <asm/arch/cvmx-app-init.h>
# include <asm/arch/cvmx-bootmem.h>
# include <asm/arch/cvmx-sysinfo.h>
# include <asm/arch/octeon_boot.h>
# include <asm/arch/cvmx-coremask.h>
# include <asm/arch/octeon-feature.h>
# include <asm/arch/cvmx-boot-vector.h>
# include <asm/arch/cvmx-helper-fdt.h>
# ifdef CONFIG_OCTEON_HW_BCH
#  include <asm/arch/cvmx-bch.h>
# endif

/** Maximum possible buses for the cores in Octeon */
#define MAX_CORE_TADS		8

/************************************************************************/
/******* Global variable definitions ************************************/
/************************************************************************/
uint32_t cur_exception_base = EXCEPTION_BASE_INCR;
struct octeon_boot_descriptor boot_desc[CVMX_MIPS_MAX_CORES];
struct cvmx_bootinfo cvmx_bootinfo_array[CVMX_MIPS_MAX_CORES];

struct boot_info_block boot_info_block_array[CVMX_MIPS_MAX_CORES];
cvmx_coremask_t coremask_to_run;
uint64_t boot_cycle_adjustment;
extern volatile int start_core0;
/************************************************************************/
/************************************************************************/
/* Used for error checking on load commands */
extern ulong load_reserved_addr;	/* Base address of reserved memory for loading */
extern ulong load_reserved_size;	/* Size of reserved memory for loading */

#ifdef CONFIG_HW_WATCHDOG
extern void hw_watchdog_disable(void);
#endif
#if defined(CONFIG_USB_OCTEON) || defined(CONFIG_USB_EHCI) || \
    defined(CONFIG_USB_XHCI)
extern int usb_stop(void);
#endif
#ifdef CONFIG_OCTEON_INTERNAL_ENET
extern int octeon_network_hw_shutdown(int mode);
#endif
#ifdef CONFIG_HW_WATCHDOG
extern void hw_watchdog_disable(void);
#endif
#ifdef CONFIG_OCTEON_MGMT_ENET
	extern int octeon_mgmt_port_shutdown(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

/************************************************************************/
uint32_t get_except_base_reg(void)
{
	return read_c0_ebase();
}

void set_except_base_addr(uint32_t addr)
{
	write_c0_ebase(addr);
}

uint64_t get_cop0_cvmctl_reg(void)
{
	return read_64bit_c0_cvmctl();
}

void set_cop0_cvmctl_reg(uint64_t value)
{
	write_64bit_c0_cvmctl(value);
}

uint64_t get_cop0_cvmmemctl_reg(void)
{
	return read_64bit_c0_cvmmemctl();
}

void set_cop0_cvmmemctl_reg(uint64_t value)
{
	write_64bit_c0_cvmmemctl(value);
}

uint32_t get_core_num(void)
{

	return (0x3FF & get_except_base_reg());
}

uint32_t get_except_base_addr(void)
{

	return (~0x3FF & get_except_base_reg());
}

/**
 * Build an exception table to branch to U-Boot's exception handler and sets
 * the ebase register
 *
 * @param exception_table	pointer to exception table.  The table
 * 				must be in KSEG0 and 4K aligned.
 */
int octeon_build_exception_table(void *exception_table)
{
	uint32_t instructions[4] = {
		0x3c19c000,	/* lui   t9, 0xc000	*/
		0x27390000,	/* addiu t9, 0x0000	*/
		0x03200009,	/* jr t9		*/
		0x00000000	/*  nop			*/
	};
	int offset;
	uint32_t value;

	value = (uint32_t)exception_table;
	if (value & ((1 << 12) - 1))
		return -1;

	memset(exception_table, 0, 0x500);
	memcpy(exception_table, instructions, sizeof(instructions));
	for (offset = 0x200; offset < 0x500; offset += 0x80) {
		/* Set offset for addiu instruction */
		instructions[1] = (instructions[1] & 0xffff0000) | offset;
		memcpy(exception_table + offset, instructions,
		       sizeof(instructions));
	}
	set_except_base_addr(value);
	return 0;
}

void copy_default_except_handlers(uint32_t addr_arg)
{
	uint8_t *addr = cvmx_phys_to_ptr(addr_arg);
	uint32_t nop_loop[2] = { 0x1000ffff, 0x0 };
	/* Set up some dummy loops for exception handlers to help with debug */
	memset(addr, 0, 0x800);
	memcpy(addr + 0x00, nop_loop, 8);
	memcpy(addr + 0x80, nop_loop, 8);
	memcpy(addr + 0x100, nop_loop, 8);
	memcpy(addr + 0x180, nop_loop, 8);
	memcpy(addr + 0x200, nop_loop, 8);
	memcpy(addr + 0x280, nop_loop, 8);
	memcpy(addr + 0x300, nop_loop, 8);
	memcpy(addr + 0x380, nop_loop, 8);
	memcpy(addr + 0x400, nop_loop, 8);
	memcpy(addr + 0x480, nop_loop, 8);
	memcpy(addr + 0x500, nop_loop, 8);
	memcpy(addr + 0x580, nop_loop, 8);
	memcpy(addr + 0x600, nop_loop, 8);
	memcpy(addr + 0x680, nop_loop, 8);

}

/**
 * Provide current cycle counter as a return value
 *
 * @return current cycle counter
 */
uint64_t octeon_get_cycles(void)
{
	return read_64bit_c0_cvmcount();
}

void octeon_delay_cycles(uint64_t cycles)
{
	uint64_t start = octeon_get_cycles();
	while (start + cycles > octeon_get_cycles()) ;
}

uint32_t octeon_get_cop0_status(void)
{
	return read_c0_status();
}

void cvmx_set_cycle(uint64_t cycle)
{
	write_64bit_c0_cvmcount(cycle);
}
#endif /* __U_BOOT__ */

/* Translate the u-boot data structure to linux_app_global_data_t which is
 * the same as the old u-boot gd_t for compatibility with older apps.  This
 * allows gd_t to change without breaking anything.
 */
void octeon_translate_gd_to_linux_app_global_data(linux_app_global_data_t *lagd)
{
#ifdef __U_BOOT__
	debug("Converting gd at %p to linux app global at %p, size 0x%x\n",
	      gd, lagd, sizeof(*lagd));
#endif
	memset(lagd, 0, sizeof(*lagd));
	lagd->bd = gd->bd;
	lagd->flags = 0;
	lagd->baudrate = gd->baudrate;
	lagd->have_console = gd->have_console;
	lagd->ram_size = gd->ram_size;
	lagd->reloc_off = gd->reloc_off;
	lagd->env_addr = gd->env_addr;
	lagd->env_valid = gd->env_valid;
	lagd->cpu_clock_mhz = gd->cpu_clk;
	lagd->ddr_clock_mhz = gd->mem_clk;
	lagd->ddr_ref_hertz = gd->arch.ddr_ref_hertz;
	lagd->mcu_rev_maj = gd->arch.mcu_rev_maj;
	lagd->mcu_rev_min = gd->arch.mcu_rev_min;
	lagd->console_uart = gd->arch.console_uart;

	memcpy(&(lagd->board_desc), (void *)&(gd->arch.board_desc),
	       sizeof(octeon_eeprom_board_desc_t));
	memcpy(&(lagd->clock_desc), (void *)&(gd->arch.clock_desc),
	       sizeof(octeon_eeprom_clock_desc_t));
	memcpy(&(lagd->mac_desc), (void *)&(gd->arch.mac_desc),
	       sizeof(octeon_eeprom_mac_addr_t));

	lagd->jt = gd->jt;
	lagd->err_msg = NULL;
	lagd->uboot_flash_address = gd->arch.uboot_flash_address;
	lagd->uboot_flash_size = gd->arch.uboot_flash_size;
	lagd->dfm_ram_size = gd->arch.dfm_ram_size;

	if (gd->flags & GD_FLG_RELOC)
		lagd->flags |= LA_GD_FLG_RELOC;
	if (gd->flags & GD_FLG_DEVINIT)
		lagd->flags |= LA_GD_FLG_DEVINIT;
	if (gd->flags & GD_FLG_CLOCK_DESC_MISSING)
		lagd->flags |= LA_GD_FLG_CLOCK_DESC_MISSING;
	if (gd->flags & GD_FLG_BOARD_DESC_MISSING)
		lagd->flags |= LA_GD_FLG_BOARD_DESC_MISSING;
	if (gd->flags & GD_FLG_DDR_VERBOSE)
		lagd->flags |= LA_GD_FLG_DDR_VERBOSE;
	if (gd->flags & GD_FLG_DDR0_CLK_INITIALIZED)
		lagd->flags |= LA_GD_FLG_DDR0_CLK_INITIALIZED;
	if (gd->flags & GD_FLG_DDR1_CLK_INITIALIZED)
		lagd->flags |= LA_GD_FLG_DDR1_CLK_INITIALIZED;
	if (gd->flags & GD_FLG_DDR2_CLK_INITIALIZED)
		lagd->flags |= LA_GD_FLG_DDR2_CLK_INITIALIZED;
	if (gd->flags & GD_FLG_DDR3_CLK_INITIALIZED)
		lagd->flags |= LA_GD_FLG_DDR3_CLK_INITIALIZED;
	if (gd->flags & GD_FLG_RAM_RESIDENT)
		lagd->flags |= LA_GD_FLG_RAM_RESIDENT;
	if (gd->flags & GD_FLG_FAILSAFE_MODE)
		lagd->flags |= LA_GD_FLG_FAILSAFE_MODE;
	if (gd->flags & GD_FLG_DDR_TRACE_INIT)
		lagd->flags |= LA_GD_FLG_DDR_TRACE_INIT;
	if (gd->flags & GD_FLG_DFM_CLK_INITIALIZED)
		lagd->flags |= LA_GD_FLG_DFM_CLK_INITIALIZED;
	if (gd->flags & GD_FLG_DFM_VERBOSE)
		lagd->flags |= LA_GD_FLG_DFM_VERBOSE;
	if (gd->flags & GD_FLG_DFM_TRACE_INIT)
		lagd->flags |= LA_GD_FLG_DFM_TRACE_INIT;
	if (gd->flags & GD_FLG_MEMORY_PRESERVED)
		lagd->flags |= LA_GD_FLG_MEMORY_PRESERVED;
}

#ifdef __U_BOOT__
int octeon_setup_boot_desc_block_board(cvmx_coremask_t *pcm, int argc,
				       char *const argv[], uint64_t entry_point,
				       uint32_t stack_size, uint32_t heap_size,
				       uint32_t boot_flags,
				       uint64_t stack_heep_base_addr,
				       uint32_t image_flags,
				       uint32_t config_flags,
				       cvmx_coremask_t *new_pcm,
				       int app_index)
__attribute__((__weak__));

int octeon_setup_boot_desc_block_board(cvmx_coremask_t *pcm, int argc,
				       char *const argv[], uint64_t entry_point,
				       uint32_t stack_size, uint32_t heap_size,
				       uint32_t boot_flags,
				       uint64_t stack_heep_base_addr,
				       uint32_t image_flags,
				       uint32_t config_flags,
				       cvmx_coremask_t *new_pcm,
				       int app_index)
{
	return 0;
}
#endif /* __U_BOOT__ */

static void  cores_common_bootinfo_init(void *arg)
{
	cvmx_coremask_t hw, avail;
	struct cvmx_cores_common_bootinfo *ccbi = arg;
	int i;

	octeon_get_available_coremask(&hw);
	cvmx_coremask_maskoff(&avail, &hw, &coremask_to_run);
	for (i = 512; i < 1024; i++) /* clear nodes 4..7 core-bits */
		cvmx_coremask_clear_core(&avail, i);

	memset(ccbi, 0, sizeof(cvmx_cores_common_bootinfo_t));
	ccbi->magic = CVMX_COMMON_BOOTINFO_MAGIC;
	ccbi->version = CVMX_COMMON_BOOTINFO_VERSION;
	cvmx_coremask_copy(&ccbi->hardware_coremask, &hw);
	cvmx_coremask_copy(&ccbi->avail_coremask, &avail);
}

/**
 * Fills in the boot descriptor block
 */
int octeon_setup_boot_desc_block(cvmx_coremask_t *pcm, int argc,
				 char *const argv[], uint64_t entry_point,
				 uint32_t stack_size, uint32_t heap_size,
				 uint32_t boot_flags,
				 uint64_t stack_heep_base_addr,
				 uint32_t image_flags,
				 uint32_t config_flags, cvmx_coremask_t *new_pcm,
				 int app_index)
__attribute__ ((__weak__));

int octeon_setup_boot_desc_block(cvmx_coremask_t *pcm, int argc,
				 char *const argv[], uint64_t entry_point,
				 uint32_t stack_size, uint32_t heap_size,
				 uint32_t boot_flags,
				 uint64_t stack_heep_base_addr,
				 uint32_t image_flags,
				 uint32_t config_flags, cvmx_coremask_t *new_pcm,
				 int app_index)
{
#ifdef __U_BOOT__
	uint64_t cf_cs0_addr, cf_cs1_addr;
# if defined(CONFIG_OF_LIBFDT) && defined(CONFIG_OCTEON_BOOT_BUS)
	const void *nodep;
	int nodeoffset;
	int len;
# endif
	int cf_cs0 = -1, cf_cs1 = -1;
	int fdt_size;
	int node;
	uint64_t new_fdt_addr;
	void *new_fdt;
#endif
	int hplug = 0;
	int rc = 0;
	uint32_t first_core = cvmx_coremask_get_first_core(pcm);
	int core;
	cvmx_coremask_t cores_to_activate;

	/* Set up application descriptors for all cores running this application.
	 * These are only used during early boot - the simple exec init code
	 * copies any information of interest before the application starts.
	 *  This is stored in the top of the heap, and is guaranteed to be
	 * looked at before malloc is called.
	 */
#if !defined(__U_BOOT__)
	cvmx_sysinfo_t *si = cvmx_sysinfo_get();
	linux_app_boot_info_t *labi;
#endif

	/* Set up space for passing arguments to application */
	int argv_size = 0;
	int i;
	uint32_t argv_array[OCTEON_ARGV_MAX_ARGS] = { 0 };
	int64_t argv_data;

#ifdef __U_BOOT__
	fdt_size = cvmx_fdt_get_fdt_size(gd->fdt_blob);
	if (fdt_size < 0) {
		puts("Error getting flat device tree size!\n");
		return -1;
	}

	fdt_size = (fdt_size + 0x3ff) & ~0x3ff;
	debug("%s: Allocating %d bytes for device tree\n", __func__, fdt_size);
	new_fdt_addr = cvmx_bootmem_phy_alloc(fdt_size, OCTEON_FDT_BASE,
					      1 << 29, 0, 0);
	if (new_fdt_addr < 0) {
		puts("Could not allocate FDT memory\n");
		return -1;
	}
	new_fdt = cvmx_phys_to_ptr(new_fdt_addr);

#ifdef CONFIG_CMD_FDT
	memcpy(new_fdt, working_fdt, cvmx_fdt_get_fdt_size(working_fdt));
#else
	memcpy(new_fdt, gd->fdt_blob, cvmx_fdt_get_fdt_size(gd->fdt_blob));
#endif
	debug("%s: Allocated application FDT at address %p\n",
	      __func__, new_fdt);

	debug("%s: Reserving first 1MB of memory\n", __func__);
	rc = cvmx_bootmem_reserve_memory(0, OCTEON_RESERVED_LOW_BOOT_MEM_SIZE,
					 "__low_reserved", 0);
	if (!rc)
		printf("Error reserving low 1MB of memory\n");
#else
	labi = &global_linux_boot_info->labi;
#endif

	if (!cvmx_coremask_is_empty(new_pcm))
		hplug = 1;
	if (!hplug) {
		for (i = 0; i < argc; i++)
			argv_size += strlen(argv[i]) + 1;

		debug("%s::%d entrypoint: 0x%llx, coremask:\n",
		      __FILE__, __LINE__, entry_point);
		cvmx_coremask_dprint(pcm);

		/* Allocate permanent storage for the argv strings.
		 * This must be from 32 bit addressable memory.
		 */
		if (app_index  < 0) {
			argv_data = cvmx_bootmem_phy_alloc(argv_size + 64, 0,
							   1ull << 29, 8, 0);
		} else {
			char tmp_block_name[64];
			sprintf(tmp_block_name, "argv_data_%08x", app_index);
			argv_data = cvmx_bootmem_phy_named_block_alloc(argv_size + 64, 0ull,
					1ull << 29, 8, tmp_block_name, 0);
		}

		if (argv_data >= 0) {
			char *argv_ptr = cvmx_phys_to_ptr(argv_data);
			void *argv_data_begin = argv_ptr;
			void *argv_data_end;

			for (i = 0; i < argc; i++) {
				/* Argv array is of 32 bit physical addresses */
				argv_array[i] = (uint32_t) argv_ptr;
				argv_ptr = strcpy(argv_ptr, argv[i]) +
					   strlen(argv[i]) + 1;
				argv_size += strlen(argv[i]) + 1;
			}
			argv_data_end = argv_ptr;
			if (image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN) {
				/* make it readable for little-endian */
				uint64_t *p = argv_data_begin;
				while ((void *)p < argv_data_end) {
					*p = swab64(*p);
					p++;
				}
			}
		} else {
			puts("ERROR: unable to allocate memory for argument data\n");
		}
	}
#if OCTEON_APP_INIT_H_VERSION >= 1	/* The UART1 flag is new */
	/* If console is on uart 1 pass this to executive */
	{
#if defined (__U_BOOT__)
		if (gd->arch.console_uart == 1)
#else
		if (lagd->console_uart == 1)
#endif
			boot_flags |= OCTEON_BL_FLAG_CONSOLE_UART1;
	}
#endif
#if !defined(__U_BOOT__)
	if (labi->pci_console_active)
#else
	if (getenv("pci_console_active"))
#endif
	{
#if OCTEON_APP_INIT_H_VERSION >= 2	/* The PCI flag is new */
		boot_flags |= OCTEON_BL_FLAG_CONSOLE_PCI;
#else
		boot_flags |= 1 << 4;
#endif
	}

	/* Bootloader boot_flags only used in simulation environment, default
	 * to no magic as required by hardware
	 */
	boot_flags |= OCTEON_BL_FLAG_NO_MAGIC;

#if CONFIG_OCTEON_SIM_SETUP && !defined(CONFIG_OCTEON_NO_BOOT_BUS)
	/* Get debug and no_magic flags from oct-sim script if running on simulator */
	boot_flags &= ~(OCTEON_BL_FLAG_DEBUG | OCTEON_BL_FLAG_NO_MAGIC);
	boot_flags |= *(uint32_t *) OCTEON_BOOTLOADER_FLAGS_ADDR &
				(OCTEON_BL_FLAG_DEBUG | OCTEON_BL_FLAG_NO_MAGIC);
#endif

	if (hplug)
		cvmx_coremask_xor(&cores_to_activate, pcm, new_pcm);
	else
		cvmx_coremask_copy(&cores_to_activate, pcm);

	/* Go through and zero out all of the nodes in the device tree that are
	 * not in use.
	 */
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		if (cvmx_coremask_get64_node(&cores_to_activate, node) == 0) {
			int offset;
			char soc_path[64];
			snprintf(soc_path, sizeof(soc_path), "/soc@%llx",
				 (unsigned long long)node * (1ULL << CVMX_NODE_MEM_SHIFT));

			offset = fdt_path_offset(new_fdt, soc_path);
			if (offset < 0) {
				debug("%s: Could not find fdt path %s for CPU node %d\n",
				      __func__, soc_path, node);
				continue;
			}
			debug("%s: NOPing FDT node at %d for %s for cpu node %d\n",
			      __func__, offset, soc_path, node);
			fdt_nop_node(new_fdt, offset);
		}
	}

	if (image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN) {
		/* For little-endian we need to create a LE version of
		 * the FDT.
		 */
		uint32_t fdt_size = fdt_totalsize(new_fdt);
		uint64_t *p, *pend;
		p = (uint64_t *) new_fdt;
		pend = (uint64_t *)(new_fdt + ((fdt_size + 7) & ~7));
		while (p <= pend) {
			*p = swab64(*p);
			p++;
		}
	}

	debug("%s: Activating cores:\n", __func__);
	cvmx_coremask_dprint(&cores_to_activate);
	debug("coremask:\n");
	cvmx_coremask_dprint(pcm);
	debug("new coremask:\n");
	cvmx_coremask_dprint(new_pcm);
	cvmx_coremask_for_each_core(core, &cores_to_activate) {
		debug("%s: Activating core %d\n",  __func__, core);
		cvmx_bootinfo_array[core].flags = 0;
#if (OCTEON_CURRENT_DESC_VERSION != 6) && (OCTEON_CURRENT_DESC_VERSION != 7)
#error Incorrect boot descriptor version for this bootloader release, check toolchain version!
#endif

#if !defined(__U_BOOT__)
		if (labi->icache_prefetch_disable)
#else
		if (getenv("icache_prefetch_disable"))
#endif
			boot_info_block_array[core].flags |=
			    BOOT_INFO_FLAG_DISABLE_ICACHE_PREFETCH;
		if (image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN)
			boot_info_block_array[core].flags |=
				BOOT_INFO_FLAG_LITTLE_ENDIAN;
		boot_info_block_array[core].entry_point = entry_point;
		if (!hplug) {
			boot_info_block_array[core].exception_base =
			    cur_exception_base;
		} else {
			boot_info_block_array[core].exception_base =
			    boot_info_block_array[first_core].exception_base;
		}

		/* Align initial stack value to 16 byte alignment */
		boot_info_block_array[core].stack_top =
		    (stack_heep_base_addr + stack_size) & (~0xFull);

		if (image_flags & OCTEON_BOOT_DESC_IMAGE_LINUX) {
			/* Linux is 64-bit and will work with any paddr */
			int64_t addr;
			addr = cvmx_bootmem_phy_alloc(sizeof(boot_desc[0]), 0,
						      0x7fffffff, 0, 0);
			if (addr < 0) {
				puts("ERROR allocating memory for boot descriptor block\n");
				return -1;
			}
			boot_info_block_array[core].boot_desc_addr = addr;
			addr = cvmx_bootmem_phy_alloc(sizeof(cvmx_bootinfo_t),
						      0, 0x7fffffff, 0, 0);
			if (addr < 0) {
				puts("ERROR allocating memory for cvmx bootinfo block\n");
				return -1;
			}
			boot_desc[core].cvmx_desc_vaddr = addr;
		} else {
			/* For simple exec we put these in Heap mapping */
			boot_desc[core].heap_base =
			    cvmx_bootinfo_array[core].heap_base =
				stack_heep_base_addr + stack_size;
			boot_desc[core].heap_end =
			    cvmx_bootinfo_array[core].heap_end =
				stack_heep_base_addr + stack_size + heap_size;
			/* Put boot descriptor at top of heap, align address */
			boot_info_block_array[core].boot_desc_addr =
			    (boot_desc[core].heap_end - sizeof(boot_desc[0])) & ~0x7ULL;
			/* Put boot descriptor below boot descriptor near
			 * top of heap, align address
			 */
			boot_desc[core].cvmx_desc_vaddr =
			    (boot_info_block_array[core].boot_desc_addr -
			     sizeof(cvmx_bootinfo_t)) & (~0x7ull);
		}

		/* The boot descriptor will be copied into the top of the heap
		 * for the core.  The application init code is responsible for
		 * copying the parts of the descriptor block that the
		 * application is interested in, as space is not reserved in the
		 * heap for this data.
		 * Note that the argv strings themselves are put into
		 * permanently allocated storage.
		 */

		/* Most fields in the boot (app) descriptor are depricated, and
		 * have been moved to the cvmx_bootinfo structure.  They are set
		 * here to ease tranistion, but may be removed in the future.
		 */

		boot_desc[core].argc = argc;
		memcpy(boot_desc[core].argv, argv_array, sizeof(argv_array));
		if (image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN) {
			/* make it readable for little-endian */
			int i;
			for (i = 0; i < OCTEON_ARGV_MAX_ARGS; i += 2) {
				uint32_t t = boot_desc[core].argv[i];
				boot_desc[core].argv[i] = boot_desc[core].argv[i + 1];
				boot_desc[core].argv[i + 1] = t;
			}
		}
		boot_desc[core].desc_version = OCTEON_CURRENT_DESC_VERSION;
		boot_desc[core].desc_size = sizeof(boot_desc[0]);
		/* boot_flags set later, copied from bootinfo block */
		boot_desc[core].debugger_flags_base_addr =
		    BOOTLOADER_DEBUG_FLAGS_BASE;

		/* Set 'master' core for application.  This is the only core
		 * that will do certain init code such as copying the exception
		 * vectors for the app, etc.
		 * NOTE: This is deprecated, and the application should use the
		 * coremask to choose a core to to setup with.
		 */
		if (core == first_core)
			cvmx_bootinfo_array[core].flags |= BOOT_FLAG_INIT_CORE;
		/* end of valid fields in boot descriptor */

#if (CVMX_BOOTINFO_MAJ_VER != 1)
#error Incorrect cvmx descriptor version for this bootloader release, check simple executive version!
#endif

		cvmx_bootinfo_array[core].flags |= boot_flags;
		cvmx_bootinfo_array[core].major_version = CVMX_BOOTINFO_MAJ_VER;
		cvmx_bootinfo_array[core].minor_version = CVMX_BOOTINFO_MIN_VER;
		if (hplug) {
			cvmx_coremask_copy(&cvmx_bootinfo_array[core].ext_core_mask,
					   new_pcm);
			cvmx_bootinfo_array[core].core_mask =
				cvmx_coremask_get32(new_pcm);
		} else {
			cvmx_bootinfo_array[core].core_mask =
				cvmx_coremask_get32(pcm);
			cvmx_coremask_copy(&cvmx_bootinfo_array[core].ext_core_mask,
					   pcm);
		}
		/* Convert from bytes to Megabytes */
#if !defined(__U_BOOT__)
		cvmx_bootinfo_array[core].phy_mem_desc_addr =
			si->phy_mem_desc_addr;
#else
		cvmx_bootinfo_array[core].phy_mem_desc_addr =
			((uint32_t)__cvmx_bootmem_internal_get_desc_ptr()) & 0x7FFFFFF;
#endif

#if defined (__U_BOOT__)
		/* Convert from bytes to Megabytes */
		cvmx_bootinfo_array[core].dram_size =
			/*gd->ram_size/(1024 * 1024);*/
			gd->arch.ram_sizes[cvmx_coremask_core_to_node(core)];
		cvmx_bootinfo_array[core].dclock_hz =
			gd->mem_clk * 1000000;
		cvmx_bootinfo_array[core].eclock_hz =
			gd->cpu_clk;
		cvmx_bootinfo_array[core].board_type =
			gd->arch.board_desc.board_type;
		cvmx_bootinfo_array[core].board_rev_major =
			gd->arch.board_desc.rev_major;
		cvmx_bootinfo_array[core].board_rev_minor =
			gd->arch.board_desc.rev_minor;
		cvmx_bootinfo_array[core].stack_top =
			boot_info_block_array[core].stack_top;

		cvmx_bootinfo_array[core].mac_addr_count = gd->arch.mac_desc.count;
		memcpy(cvmx_bootinfo_array[core].mac_addr_base,
		       (void *)gd->arch.mac_desc.mac_addr_base,
		       sizeof(cvmx_bootinfo_array[core].mac_addr_base));
		strncpy(cvmx_bootinfo_array[core].board_serial_number,
			(char *)(gd->arch.board_desc.serial_str),
			CVMX_BOOTINFO_OCTEON_SERIAL_LEN);

		if (image_flags & OCTEON_BOOT_DESC_LITTLE_ENDIAN) {
			uint64_t swap_array[4];
			memcpy(swap_array, cvmx_bootinfo_array[core].board_serial_number,
			       sizeof(swap_array));
			for (i = 0; i < 4; i++)
				swap_array[i] = swab64(swap_array[i]);
			memcpy(cvmx_bootinfo_array[core].board_serial_number, swap_array,
			       sizeof(swap_array));
		}
#else
		/* Convert from bytes to Megabytes */
		cvmx_bootinfo_array[core].dram_size =
			lagd->ram_size/(1024 * 1024);
		cvmx_bootinfo_array[core].dclock_hz =
			lagd->ddr_clock_mhzs * 1000000;
		cvmx_bootinfo_array[core].eclock_hz =
			lagd->cpu_clock_mhz * 1000000;
		cvmx_bootinfo_array[core].board_type =
			lagd->board_desc.board_type;
		cvmx_bootinfo_array[core].board_rev_major =
			lagd->board_desc.rev_major;
		cvmx_bootinfo_array[core].board_rev_minor =
			lagd->board_desc.rev_minor;
		cvmx_bootinfo_array[core].mac_addr_count =
			lagd->mac_desc.count;
		cvmx_bootinfo_array[core].stack_top =
			boot_info_block_array[core].stack_top;
		memcpy(cvmx_bootinfo_array[core].mac_addr_base,
		       (void *)lagd->mac_desc.mac_addr_base,
		       sizeof(cvmx_bootinfo_array[core].mac_addr_base));
		strncpy(cvmx_bootinfo_array[core].board_serial_number,
			(char *)(lagd->board_desc.serial_str),
			CVMX_BOOTINFO_OCTEON_SERIAL_LEN);
#endif
		debug("board type is: %d, %s\n",
		      cvmx_bootinfo_array[core].board_type,
		      cvmx_board_type_to_string(cvmx_bootinfo_array[core].board_type));
		cvmx_bootinfo_array[core].exception_base_addr = cur_exception_base;
		cvmx_bootinfo_array[core].stack_size = stack_size;
#if (CVMX_BOOTINFO_MIN_VER >= 1)
# if !defined(__U_BOOT__)
		cvmx_bootinfo_array[core].compact_flash_common_base_addr =
			labi->compact_flash_common_base_addr;

		cvmx_bootinfo_array[core].compact_flash_attribute_base_addr =
			labi->compact_flash_attribute_base_addr;

		cvmx_bootinfo_array[core].led_display_base_addr =
			labi->led_display_base_addr;
# else
#  ifdef OCTEON_CF_COMMON_BASE_ADDR
		cf_cs0_addr = OCTEON_CF_COMMON_BASE_ADDR;
#  else
		cf_cs0_addr = 0;
#  endif
#  ifdef OCTEON_CF_ATTRIB_BASE_ADDR
		cf_cs1_addr = OCTEON_CF_ATTRIB_BASE_ADDR;
#  else
		cf_cs1_addr = 0;
#  endif

#  ifdef OCTEON_CF_TRUE_IDE_CS0_ADDR
		cf_cs0_addr = OCTEON_CF_TRUE_IDE_CS0_ADDR;
#   ifdef OCTEON_CF_TRUE_IDE_CS1_ADDR
		cf_cs1_addr = OCTEON_CF_TRUE_IDE_CS1_ADDR;
#   endif
#  endif
#  if defined(CONFIG_OF_LIBFDT) && defined(CONFIG_OCTEON_BOOT_BUS)
		nodeoffset = fdt_path_offset(gd->fdt_blob,
					     "/soc/bootbus/compact-flash");
		if (nodeoffset >= 0) {
			nodep = fdt_getprop(gd->fdt_blob, nodeoffset,
					    "reg", &len);
			if (nodep && len >= 12) {
				cf_cs0 = fdt32_to_cpu(((int *)nodep)[0]);
				octeon_boot_bus_get_range(cf_cs0, -1,
							  &cf_cs0_addr,
							  NULL);
			}
			if (nodep && len == 24) {
				cf_cs1 = fdt32_to_cpu(((int *)nodep)[3]);
				octeon_boot_bus_get_range(cf_cs1, -1,
							  &cf_cs1_addr,
							  NULL);
			}
			nodep = fdt_getprop(gd->fdt_blob, nodeoffset,
					    "cavium,true-ide", NULL);
			/* Only adjust the address in non-True-IDE mode */
			if (!nodep) {
				debug("Not True-IDE mode\n");
				cf_cs0_addr |= (1 << 11);
			}
		}
#  endif
		debug("CF CS0: %d, addr: 0x%llx, CS1: %d, addr: 0x%llx\n",
		      cf_cs0, cf_cs0_addr, cf_cs1, cf_cs1_addr);
		cvmx_bootinfo_array[core].compact_flash_common_base_addr =
			cf_cs0_addr;
		cvmx_bootinfo_array[core].compact_flash_attribute_base_addr =
			cf_cs1_addr;

#  ifdef CONFIG_OCTEON_ENABLE_LED_DISPLAY
		{
		uint64_t led_addr;
		octeon_boot_bus_get_dev_info("/soc/bootbus/led-display",
					     "avago,hdsp-253x",
					     &led_addr, NULL);

		led_addr |= 0xf8;
		cvmx_bootinfo_array[core].led_display_base_addr = led_addr;
		}
#  else
		cvmx_bootinfo_array[core].led_display_base_addr = 0;
#  endif
# endif
#endif /* (CVMX_BOOTINFO_MAJ_VER >= 1) */

#if (CVMX_BOOTINFO_MIN_VER >= 2)
		/* Set DFA reference if available */
# if defined (__U_BOOT__)
		cvmx_bootinfo_array[core].dfa_ref_clock_hz =
		    gd->arch.clock_desc.dfa_ref_clock_mhz_x_8 / 8 * 1000000;
# else
		cvmx_bootinfo_array[core].dfa_ref_clock_hz =
		    lagd->clock_desc.dfa_ref_clock_mhz_x_8 / 8 * 1000000;
# endif

# if (CVMX_BOOTINFO_MIN_VER >= 3)
#  if defined(__U_BOOT__)
		cvmx_bootinfo_array[core].fdt_addr = (uint32_t)new_fdt;
#  else
		cvmx_bootinfo_array[core].fdt_addr = (uint32_t) OCTEON_FDT_BASE;
#  endif
# endif /* (CVMX_BOOTINFO_MIN_VER >= 3) */
		/* Copy boot_flags settings to config flags */
		if (boot_flags & OCTEON_BL_FLAG_DEBUG)
			cvmx_bootinfo_array[core].config_flags |=
			    CVMX_BOOTINFO_CFG_FLAG_DEBUG;
# if OCTEON_APP_INIT_H_VERSION >= 3
		if (boot_flags & OCTEON_BL_FLAG_BREAK)
			cvmx_bootinfo_array[core].config_flags |=
			    CVMX_BOOTINFO_CFG_FLAG_BREAK;
# endif
		if (boot_flags & OCTEON_BL_FLAG_NO_MAGIC)
			cvmx_bootinfo_array[core].config_flags |=
			    CVMX_BOOTINFO_CFG_FLAG_NO_MAGIC;
		/* Set config flags */
		if (CONFIG_OCTEON_PCI_HOST)
			cvmx_bootinfo_array[core].config_flags |=
			    CVMX_BOOTINFO_CFG_FLAG_PCI_HOST;
# ifdef CONFIG_OCTEON_PCI_TARGET
		else if (CONFIG_OCTEON_PCI_TARGET)
			cvmx_bootinfo_array[core].config_flags |=
			    CVMX_BOOTINFO_CFG_FLAG_PCI_TARGET;
# endif
		cvmx_bootinfo_array[core].config_flags |= config_flags;
#endif /* (CVMX_BOOTINFO_MIN_VER >= 3) */
#if (CVMX_BOOTINFO_MIN_VERR >= 4)
		cvmx_coremask_copy(&cvmx_bootinfo_array[core].ext_core_mask,
				   new_pcm);
#endif /* (CVMX_BOOTINFO_MIN_VER >= 4) */
		/* Fill in remaining fields of boot_desc array */

		/* NOTE: This will break on 78XX since core_mask is limited to 32 bits */
		boot_desc[core].core_mask =
			cvmx_coremask_get32(&cvmx_bootinfo_array[core].ext_core_mask);

		boot_desc[core].flags = cvmx_bootinfo_array[core].flags;
		boot_desc[core].eclock_hz = cvmx_bootinfo_array[core].eclock_hz;

		/* Copy deprecated fields from cvmx_bootinfo array to boot
		 * descriptor for compatibility.
		 */
		/* Erroneously used in SDK 1.8.1, so keep for a while */
		boot_desc[core].dram_size = cvmx_bootinfo_array[core].dram_size;
		/* End deprecated boot descriptor fields */

		debug("boot_info_block_array[%d] at %p\n", core,
		      &boot_info_block_array[core]);
		if (image_flags & OCTEON_BOOT_DESC_IMAGE_LINUX) {
			/* copy boot desc for use by app */
#ifdef __U_BOOT__
			memcpy((void *)(uint32_t) boot_info_block_array[core].boot_desc_addr,
			       (void *)&boot_desc[core],
			       boot_desc[core].desc_size);
			memcpy((void *)(uint32_t) boot_desc[core].cvmx_desc_vaddr,
			       (void *)&cvmx_bootinfo_array[core],
			       sizeof(cvmx_bootinfo_t));
#else
			memcpy((void *) boot_info_block_array[core].boot_desc_addr,
			       (void *)&boot_desc[core],
			       boot_desc[core].desc_size);
			memcpy((void *) boot_desc[core].cvmx_desc_vaddr,
			       (void *)&cvmx_bootinfo_array[core],
			       sizeof(cvmx_bootinfo_t));
#endif
		} else {
			/* Copy boot descriptor and cmvx_bootinfo structures to correct virtual address
			 * for the application to read them.
			 */
			if (boot_info_block_array[core].tlb_entries == NULL) {
				boot_info_block_array[core].tlb_entries =
					(octeon_tlb_entry_t_bl *)
						calloc(sizeof(octeon_tlb_entry_t_bl),
						       NUM_OCTEON_TLB_ENTRIES);
				if (!boot_info_block_array[core].tlb_entries) {
					printf("%s: Error: out of memory allocating %d TLB entries for core %d\n",
					       __func__, NUM_OCTEON_TLB_ENTRIES, core);
					return -1;
				}
				debug("%s: Allocated %d tlb entries for core %d\n",
				      __func__, NUM_OCTEON_TLB_ENTRIES, core);
			} else {
				debug("%s: TLB entries already allocated for core %d\n",
				       __func__, core);
			}
#if !defined(__U_BOOT__)
			mem_copy_tlb(boot_info_block_array[core].tlb_entries,
				     (uint64_t) boot_info_block_array[core].boot_desc_addr,
				     (uint64_t) &boot_desc[core],
				     boot_desc[core].desc_size);
			mem_copy_tlb(boot_info_block_array[core].tlb_entries,
				     (uint64_t) boot_desc[core].cvmx_desc_vaddr,
				     (uint64_t) &cvmx_bootinfo_array[core],
				     sizeof(cvmx_bootinfo_t));
#else
			/* Cast to int32_t, as we need sign extension in the address */
			mem_copy_tlb(boot_info_block_array[core].tlb_entries,
				     boot_info_block_array[core].boot_desc_addr,
				     (int32_t) (&boot_desc[core]),
				     boot_desc[core].desc_size);
			mem_copy_tlb(boot_info_block_array[core].tlb_entries,
				     boot_desc[core].cvmx_desc_vaddr,
				     (int32_t) (&cvmx_bootinfo_array[core]),
				     sizeof(cvmx_bootinfo_t));
#endif
		}

	}
	if (!hplug) {
		copy_default_except_handlers(cur_exception_base);
		cur_exception_base += EXCEPTION_BASE_INCR;
#if !defined(__U_BOOT__)
		labi->cur_exception_base += EXCEPTION_BASE_INCR;
#endif
	}

#ifdef __U_BOOT__
	debug("%s: Setting up boot descriptor block\n", __func__);
	rc = octeon_setup_boot_desc_block_board(pcm, argc, argv,
						entry_point, stack_size,
						heap_size, boot_flags,
						stack_heep_base_addr,
						image_flags,
						config_flags,
						new_pcm,
						app_index);

	if (rc != 0)
		printf("ERROR: board-specific boot descriptor initialization returned %d\n",
		       rc);
#endif

	debug("stack expected: 0x%llx, actual: 0x%llx\n",
	      stack_heep_base_addr + stack_size, boot_desc[0].stack_top);
	debug("heap_base expected: 0x%llx, actual: 0x%llx\n",
	      stack_heep_base_addr + stack_size, boot_desc[0].heap_base);
	debug("heap_top expected: 0x%llx, actual: 0x%llx\n",
	      stack_heep_base_addr + stack_size + heap_size,
	      boot_desc[0].heap_end);
	debug("Entry point (virt): 0x%llx\n", entry_point);

	return rc;
}

#if defined(__U_BOOT__)
/* octeon_write64_byte and octeon_read64_byte are only used by the debugger
 * stub.  The debugger will generate KSEG0 addresses that are not in the 64 bit
 * compatibility space, so we detect that and fix it up.  This should probably
 * be addressed in the debugger itself, as this fixup makes some valid 64 bit
 * address inaccessible.
 */
#define DO_COMPAT_ADDR_FIX
void octeon_write64_byte(uint64_t csr_addr, uint8_t val)
{

	volatile uint32_t addr_low = csr_addr & 0xffffffff;
	volatile uint32_t addr_high = csr_addr >> 32;

	if (!addr_high && (addr_low & 0x80000000)) {
#ifdef DO_COMPAT_ADDR_FIX
		addr_high = ~0;
#endif
#if 0
		char tmp_str[500];
		sprintf(tmp_str, "fixed read64_byte at low  addr: 0x%x\n",
			addr_low);
		simprintf(tmp_str);
		sprintf(tmp_str, "fixed read64_byte at high addr: 0x%x\n",
			addr_high);
		simprintf(tmp_str);
#endif

	}

	asm volatile ("	.set	push				\n"
		      "	.set	mips64				\n"
		      "	.set	noreorder			\n"
		      /* Standard twin 32 bit -> 64 bit construction */
		      "	dsll	%[addrh], 32			\n"
		      "	dsll	%[addrl], 32			\n"
		      "	dsrl	%[addrl], 32			\n"
		      "	daddu	%[addrh], %[addrh], %[addrl]	\n"
		      /* Combined value is in addrh */
		      "	sb	%[val], 0(%[addrh])		\n"
		      "	.set	pop				\n"
		      : : [val] "r"(val),
		      [addrh] "r"(addr_high),
		      [addrl] "r"(addr_low) : "memory");
}

uint8_t octeon_read64_byte(uint64_t csr_addr)
{

	uint8_t val;

	uint32_t addr_low = csr_addr & 0xffffffff;
	uint32_t addr_high = csr_addr >> 32;

	if (!addr_high && (addr_low & 0x80000000)) {
#ifdef DO_COMPAT_ADDR_FIX
		addr_high = ~0;
#endif
#if 0
		char tmp_str[500];
		sprintf(tmp_str, "fixed read64_byte at low  addr: 0x%x\n",
			addr_low);
		simprintf(tmp_str);
		sprintf(tmp_str, "fixed read64_byte at high addr: 0x%x\n",
			addr_high);
		simprintf(tmp_str);
#endif

	}

	asm volatile ("	.set	push				\n"
		      "	.set	mips64				\n"
		      "	.set	noreorder			\n"
		      /* Standard twin 32 bit -> 64 bit construction */
		      "	dsll	%[addrh], 32			\n"
		      "	dsll	%[addrl], 32			\n"
		      "	dsrl	%[addrl], 32			\n"
		      "	daddu	%[addrh], %[addrh], %[addrl]	\n"
		      /* Combined value is in addrh */
		      "	lb	%[val], 0(%[addrh])		\n"
		      "	.set	pop				\n"
		      : [val] "=&r"(val)
		      : [addrh] "r"(addr_high), [addrl] "r"(addr_low)
		      : "memory");

	return val;
}

/**
 * Memset in 64 bit chunks to a 64 bit addresses.  Needs
 * wrapper to be useful.
 *
 * @param start_addr start address (must be 8 byte alligned)
 * @param val        value to write
 * @param byte_count number of bytes - must be multiple of 8.
 */
static void octeon_memset64_64only(uint64_t start_addr_arg, uint8_t val,
				   uint64_t byte_count_arg)
{
	volatile uint64_t byte_count = byte_count_arg;
	volatile uint64_t start_addr = start_addr_arg;
	volatile uint64_t val64;
	volatile uint32_t val_low;

	if (byte_count & 0x7)
		return;

	if (!byte_count)
		return;

	val_low = val | ((uint32_t) val << 8) | ((uint32_t) val << 16)
			| ((uint32_t) val << 24);

	val64 = ((uint64_t) val_low << 32) | val_low;

	asm volatile ("	.set	push			\n"
		      "	.set	mips64			\n"
		      "	.set	noreorder		\n"
		      /* Now do real work..... */
		      "	1:				\n"
		      "	 daddi	%[cnt], -8		\n"
		      "	 sd	%[val], 0(%[addr])	\n"
		      "	 bne	$0, %[cnt], 1b		\n"
		      "	 daddi	%[addr], 8		\n"
		      "	.set	pop			\n"
		      :
		      : [cnt] "r"(byte_count),
		      [addr] "r"(start_addr),
		      [val] "r"(val64)
		      : "memory");
}

/**
 * memcpy in 64 bit chunks to a 64 bit addresses.  Needs wrapper
 * to be useful.
 *
 * @param dest_addr destination address (must be 8 byte alligned)
 * @param src_addr destination address (must be 8 byte alligned)
 * @param byte_count number of bytes - must be multiple of 8.
 */
static void octeon_memcpy64_64only(uint64_t dest_addr, uint64_t src_addr,
				   uint64_t byte_count)
{
	volatile uint32_t count_low = byte_count & 0xffffffff;
	volatile uint32_t count_high = byte_count >> 32;

	volatile uint32_t src_low = src_addr & 0xffffffff;
	volatile uint32_t src_high = src_addr >> 32;
	volatile uint32_t dest_low = dest_addr & 0xffffffff;
	volatile uint32_t dest_high = dest_addr >> 32;
	uint32_t tmp;

	if (byte_count & 0x7)
		return;

	asm volatile ("	.set	push			\n"
		      "	.set	mips64			\n"
		      "	.set	noreorder		\n"
		      /* Standard twin 32 bit -> 64 bit construction */
		      "	dsll	%[cnth], 32		\n"
		      "	dsll	%[cntl], 32		\n"
		      "	dsrl	%[cntl], 32		\n"
		      "	daddu	%[cnth], %[cnth], %[cntl] \n"
		      /* Combined value is in cnth */
		      /* Standard twin 32 bit -> 64 bit construction */
		      "	dsll	%[srch], 32		\n"
		      "	dsll	%[srcl], 32		\n"
		      "	dsrl	%[srcl], 32		\n"
		      "	daddu	%[srch], %[srch], %[srcl] \n"
		      /* Combined value is in srch */
		      /* Standard twin 32 bit -> 64 bit construction */
		      "	dsll	%[dsth], 32		\n"
		      "	dsll	%[dstl], 32		\n"
		      "	dsrl	%[dstl], 32		\n"
		      "	daddu	%[dsth], %[dsth], %[dstl] \n"
		      /* Combined value is in dsth */
		      /* Now do real work..... */
		      "	1:				\n"
		      "	 ld	%[tmp], 0(%[srch])	\n"
		      "	 daddi	%[cnth], -8		\n"
		      "	 sd	%[tmp], 0(%[dsth])	\n"
		      "	 daddi	%[dsth], 8		\n"
		      "	 bne	$0, %[cnth], 1b		\n"
		      "	 daddi	%[srch], 8		\n"
		      "	.set	pop			\n"
		      : [tmp] "=&r"(tmp)
		      : [cnth] "r"(count_high), [cntl] "r"(count_low),
		      [dsth] "r"(dest_high),[dstl] "r"(dest_low),
		      [srcl] "r"(src_low), [srch] "r"(src_high)
		      : "memory");

}

uint64_t memcpy64(uint64_t dest_addr, uint64_t src_addr, uint64_t count)
{

	/* Caller must make sure that the 64 bit src/dest addresses are
	 * valid.  They are directly accessed in this function
	 */
	uint64_t to_copy = count;

	if ((src_addr & 0x7) != (dest_addr & 0x7)) {
		while (to_copy--) {
			cvmx_write64_uint8(dest_addr++,
					   cvmx_read64_uint8(src_addr++));
		}
	} else {
		while ((dest_addr & 0x7) && to_copy) {
			to_copy--;
			cvmx_write64_uint8(dest_addr++,
					   cvmx_read64_uint8(src_addr++));
		}
		if (to_copy >= 8) {
			octeon_memcpy64_64only(dest_addr, src_addr,
					       to_copy & ~0x7ull);
			dest_addr += to_copy & ~0x7ull;
			src_addr += to_copy & ~0x7ull;
			to_copy &= 0x7ull;
		}
		while (to_copy--) {
			cvmx_write64_uint8(dest_addr++,
					   cvmx_read64_uint8(src_addr++));
		}
	}
	return count;
}

void memset64(uint64_t start_addr, uint8_t value, uint64_t len)
{

	if (!(start_addr & 0xffffffff00000000ull) && (start_addr & (1 << 31)))
		start_addr |= 0xffffffff00000000ull;
	start_addr = MAKE_XKPHYS(start_addr);

	while ((start_addr & 0x7) && (len-- > 0)) {
		cvmx_write64_uint8(start_addr++, value);
	}
	if (len >= 8) {
		octeon_memset64_64only(start_addr, value, len & ~0x7ull);
		start_addr += len & ~0x7ull;
		len &= 0x7;
	}
	while (len-- > 0) {
		cvmx_write64_uint8(start_addr++, value);
	}

}
#endif

/**
 * Copy memory using the tlb descriptor array to map virtual addresses to
 * physical addresses.
 *
 * @param[in] tlb_array	array of TLB descriptors to use for mapping
 * @param dest_virt	destination virtual address
 * @param src_addr	source address to copy from
 * @param len		number of bytes to copy
 *
 * @return	number of bytes copied or ~0UL if it failed.
 */
#if defined(__U_BOOT__)
uint32_t mem_copy_tlb(octeon_tlb_entry_t_bl * tlb_array, uint64_t dest_virt,
		      uint64_t src_addr, uint32_t len)
#else
uint64_t mem_copy_tlb(octeon_tlb_entry_t_bl * tlb_array, uint64_t dest_virt,
		      uint64_t src_addr, uint32_t len)
#endif
{
	uint64_t cur_src;
	uint32_t cur_len;
	uint64_t dest_phys;
	uint32_t chunk_len;
	int tlb_index;
	uint32_t page_size;
	octeon_tlb_entry_t_bl *tlb_entry;

	/* Caller must make sure that src_addr address is valid - it is
	 * used directly
	 */
	cur_src = src_addr;

	cur_len = len;
	debug("tlb_ptr: %p, dvirt: 0x%llx, sphy: 0x%llx, len: %d\n",
	      tlb_array, dest_virt, src_addr, len);
	if (!tlb_array) {
		printf("%s: Error: NULL TLB array\n", __func__);
#if defined(__U_BOOT__)
		return ~0;
#else
		return ~0ULL;
#endif
	}

	for (tlb_index = 0; tlb_index < get_num_tlb_entries() && cur_len > 0;
	     tlb_index++) {
		page_size =
		    TLB_PAGE_SIZE_FROM_EXP(tlb_array[tlb_index].page_size);

		tlb_entry = &tlb_array[tlb_index];
		/* We found a page0 mapping that contains the start of the
		 * block
		 */
#if 0
		debug("tlb_array[%d].virt_base: 0x%llx, phy_page0: 0x%llx, phy_page1: 0x%llx, page_size: 0x%x\n",
			tlb_index, tlb_array[tlb_index].virt_base,
			tlb_array[tlb_index].phy_page0,
			tlb_array[tlb_index].phy_page1,
			page_size);
#endif
		if (dest_virt >= (tlb_entry->virt_base)
		    && dest_virt < (tlb_entry->virt_base + page_size)) {
			dest_phys = tlb_entry->phy_page0 +
				    (dest_virt - tlb_entry->virt_base);
			chunk_len =
			    MIN((tlb_entry->virt_base  + page_size) - dest_virt,
				cur_len);

#ifdef __U_BOOT__
			dest_phys = MAKE_XKPHYS(dest_phys);
#endif
			debug("Copying(0) 0x%x bytes from 0x%llx to 0x%llx\n",
			      chunk_len, cur_src, dest_phys);
			memcpy64(dest_phys, cur_src, chunk_len);

			cur_len -= chunk_len;
			dest_virt += chunk_len;
			cur_src += chunk_len;
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
			    MIN((tlb_entry->virt_base + 2 * page_size) - dest_virt,
				cur_len);

			dest_phys = MAKE_XKPHYS(dest_phys);
			debug("Copying(1) 0x%x bytes from 0x%llx to 0x%llx\n",
			      chunk_len, cur_src, dest_phys);
			memcpy64(dest_phys, cur_src, chunk_len);
			cur_len -= chunk_len;
			dest_virt += chunk_len;
			cur_src += chunk_len;
		}
	}
	if (cur_len != 0) {
		printf("ERROR copying memory using TLB mappings, cur_len: %d !\n",
		       cur_len);
		return ~0UL;
	} else {
		return len;
	}
}

#if defined(__U_BOOT__)
/**
 * Specialized function for quickly clearing DRAM during boot.
 * Clears memory in 1kbyte chunks, as this speeds things up when
 * clearing DRAM while running from flash.
 * Caller must provide cache line aligned address, and count that is
 * multiple of OCTEON_BZERO_PFS_STRIDE Bytes.
 *
 * @param start_addr Start address.  Must be cache line aligned.
 *
 * @param count      Number of bytes to clear.  Must be multiple of OCTEON_BZERO_PFS_STRIDE.
 */
void octeon_bzero64_pfs(uint64_t start_addr_arg, uint64_t count_arg)
{
	/* not sure why the volatiles need to be here
	 * without them calling memset in function breaks - wrong args given
	 */
	volatile uint64_t count = count_arg;
	volatile uint64_t start_addr = start_addr_arg;
	uint32_t extra_bytes = 0;
	uint64_t extra_addr;

	if (count == 0)
		return;

	if (start_addr & CVMX_CACHE_LINE_MASK) {
		puts("ERROR: bad alignment in octeon_bzero64_pfs\n");
		return;
	}

	/* Handle lengths that aren't a multiple of the stride by doing
	 * a normal memset on the remaining (small) amount of memory
	 */
	extra_bytes = count & OCTEON_BZERO_PFS_STRIDE_MASK;
	count -= extra_bytes;
	extra_addr = start_addr_arg + count;

#if 0
	printf("octeon_bzero64_pfs: start: 0x%x, count: 0x%x\n", start_addr,
	       count);
#endif

	if (!OCTEON_IS_OCTEON1PLUS()) {
		/* NOTE: All prefetches (including prepare for store) are
		 * dropped when STATUS[ERL] == 1, so prepare for store
		 * _doesn't_ work here.
		 */
		asm volatile ("	.set	push			\n"
			      "	.set	arch=octeon2		\n"
			      "	.set	noreorder		\n"
			      /* Now do real work..... */
			      "	1:				\n"
			      "	 daddi	%[cnt], -128		\n"
			      "	 zcbt	(%[addr])		\n"
			      "	 bne	$0, %[cnt], 1b		\n"
			      "	 daddi	%[addr],128		\n"
			      "	.set	pop			\n"
			      : [cnt] "+r"(count),
			      [addr] "+r"(start_addr)
			      : : "memory");
	} else {
#if !CONFIG_OCTEON_SIM_HW_DIFF
		/* The simulator is pessimistic in its simulation of prepare for
		 * store operations, and randomizes the cache lines.  U-boot
		 * uses it in a manner where we do get all zeros, but we can't
		 * run this code in simulation
		 */
		asm volatile ("	.set	push			\n"
			      "	.set	mips64			\n"
			      "	.set	noreorder		\n"
			      /* Now do real work..... */
			      "	1:				\n"
			      "	 daddi	%[cnt], -1024		\n"
			      "	 pref	30, 0(%[addr])		\n"
			      "	 pref	30, 128(%[addr])	\n"
			      "	 pref	30, 256(%[addr])	\n"
			      "	 pref	30, 384(%[addr])	\n"
			      "	 pref	30, 512(%[addr])	\n"
			      "	 pref	30, 640(%[addr])	\n"
			      "	 pref	30, 768(%[addr])	\n"
			      "	 pref	30, 896(%[addr])	\n"
			      "	 bne	$0, %[cnt], 1b		\n"
			      "	 daddi	%[addr], 1024		\n"
			      "	.set	pop			\n"
			      : [cnt] "+r"(count), [addr] "+r"(start_addr)
			      : : "memory");

		/* Invalidate the DCACHE, as the PFS will zero in L2 the
		  * memory, but won't invalidate the local core's DCACHE.
		  */
		CVMX_DCACHE_INVALIDATE;
#else
		memset64(start_addr, 0, count);
#endif
	}

	if (extra_bytes)
		memset64(extra_addr, 0, extra_bytes);
}

/**
 * Given a pointer in U-Boot convert it to a physical address
 *
 * @param[in]	ptr	pointer to convert
 * @return	physical address or 0xdeadbeefdeadbeef if not found.
 */
uint64_t uboot_tlb_ptr_to_phys(void *ptr)
{
	uint64_t eh_val = (int)ptr;
	uint64_t lo0, lo1;
	uint64_t physical_addr;
	uint32_t pagemask;
	uint32_t offset;
	int index;

	/* Clear low bits of address to properly form entry_hi */
	eh_val &= ~0ull << 13;

	/* Here we need to probe the TLB and determine what
	 * phsical address coresponds to the provided pointer.
	 * This needs to work both when running from flash and
	 * when running from DRAM.
	 */
	write_64bit_c0_entryhi(eh_val);
	__asm__ __volatile__("tlbp");
	index = read_c0_index();

	/* Return all 0xdeadbeefdeadbeef on error (0 is valid, so can't use
	 * that)
	 */
	if (index < 0)
		return (0xdeadbeefdeadbeefULL);

	/* Read matching TLB entry */
	__asm__ __volatile__("tlbr");

	lo0 = (read_64bit_c0_entrylo0() & ~(0x7ULL)) << 6;
	lo1 = (read_64bit_c0_entrylo1() & ~(0x7ULL)) << 6;
	pagemask = read_c0_pagemask() | 0x1fff;	/* Fill in low bits of pagemask */

	/* First get offset into mapping, to determine which physical
	 * page it is in. Pagemask is mask for physical page size, and each
	 * TLB entry maps two pages (lo0/lo1).
	 */
	offset = (uint32_t)ptr & pagemask;
	physical_addr = lo0;
	if (offset > (pagemask >> 1))
		physical_addr = lo1;
	physical_addr &= ~((uint64_t)(pagemask >> 1));
	physical_addr += (uint32_t) ptr & (pagemask >> 1);

	return physical_addr;
}

extern void idle_core_start_app(void);	/* in cmd_octeon.c */
extern void InitTLBStart(void);	/* in start.S */
extern void OcteonBreak(void);	/* in start.S */

/**
 * Returns a pointer to the current boot vector and allocates it if it does
 * not exist.
 *
 * @param node  node to return boot vector for
 *
 * @return pointer to boot vector or NULL if error.
 */
boot_init_vector_t *octeon_get_boot_vector(void)
{
	void *p = cvmx_boot_vector_get();

	debug("%s()\n", __func__);
	if (!p) {
		printf("%s: Error: cannot allocate memory for boot vector\n",
		       __func__);
		return NULL;
	}
	return p;
}

extern void debugHandler_entrypoint(void);

/**
 * Programs the boot bus moveable region
 * @param	base	base address to place the boot bus moveable region
 *			(bits [31:7])
 * @param	region_num	Selects which region, 0 or 1 for node 0,
 *				2 or 3 for node 1
 * @param	enable		Set true to enable, false to disable
 * @param	data		Pointer to data to put in the region, up to
 * 				16 dwords.
 * @param	num_words	Number of data dwords (up to 32)
 *
 * @return	0 for success, -1 on error
 */
int octeon_set_moveable_region(uint32_t base, int region_num, bool enable,
			       const uint64_t *data, unsigned num_words)
{
	union cvmx_mio_boot_loc_cfgx boot_loc_cfg;
	union cvmx_mio_boot_loc_adr boot_loc_adr;
	int i;
	int node = region_num >> 1;

	debug("%s(0x%x, %d, %d, %p, %u)\n", __func__, base, region_num, enable,
	      data, num_words);
	if (num_words > 32) {
		printf("%s: Too many words (%d) for region %d\n", __func__,
		       num_words, region_num);
		return -1;
	}
	if (base & 0x7f) {
		printf("%s: Error: base address 0x%x must be 128 byte aligned\n",
		       __func__, base);
		return -1;
	}
	if (region_num > ((gd->arch.node_mask > 1) ? 3 : 1)) {
		printf("%s: Region number %d out of range\n",
		       __func__, region_num);
		return -1;
	}
	if (!data && num_words > 0) {
		printf("%s: Error: NULL data\n", __func__);
		return -1;
	}
	region_num &= 1;

	boot_loc_cfg.u64 = 0;
	boot_loc_cfg.s.en = enable;
	boot_loc_cfg.s.base = base >> 7;
	debug("%s: Setting MIO_BOOT_LOC_CFG(%d) on node %d to 0x%llx\n",
	      __func__, region_num, node, boot_loc_cfg.u64);
	cvmx_write_csr_node(node, CVMX_MIO_BOOT_LOC_CFGX(region_num & 1),
			    boot_loc_cfg.u64);
	boot_loc_adr.u64 = 0;
	boot_loc_adr.s.adr = (region_num ? 0x80 : 0x00) >> 3;
	debug("%s: Setting MIO_BOOT_LOC_ADR start to 0x%llx\n", __func__,
	      boot_loc_adr.u64);
	cvmx_write_csr_node(node, CVMX_MIO_BOOT_LOC_ADR, boot_loc_adr.u64);
	for (i = 0; i < num_words; i++) {
		debug("  0x%02llx: 0x%016llx\n",
		      cvmx_read_csr_node(node, CVMX_MIO_BOOT_LOC_ADR), data[i]);
		cvmx_write_csr_node(node, CVMX_MIO_BOOT_LOC_DAT, data[i]);
	}
	return 0;
}

/**
 * This initializes the boot bus moveable region for both the boot vector
 * and the exception handler.
 *
 * @return	0 for success, error otherwise
 */
int octeon_boot_bus_moveable_init(void)
{
	const uint64_t *handler_code;
	boot_init_vector_t *p;
	int rc;

	debug("%s()\n", __func__);
	p = octeon_get_boot_vector();
	if (!p)
		return -1;

	/* Install the 2nd moveable region to the debug exception main entry
	 * point. This way the debugger will work even if there isn't any
	 * flash.
	 */

	handler_code = (const uint64_t *)debugHandler_entrypoint;
	rc = octeon_set_moveable_region(0x1fc00480, 1, true,
					handler_code, 16);
#ifdef DEBUG
	debug("Moveable boot:\n");
	print_buffer(0xbfc00000, (void *)0xbfc00000, 4, 32, 0);
#endif
	return rc;
}

int octeon_setup_boot_vector(uint32_t func_addr, cvmx_coremask_t *pcm)
{
	/* setup boot vectors for other cores..... */
	int core;
	boot_init_vector_t *boot_vect = NULL;

	debug("%s(0x%x, %p) ENTER\n", __func__, func_addr, pcm);
	cvmx_coremask_dprint(pcm);
	if (cvmx_coremask_is_empty(pcm)) {
		debug("%s: No cores to initialize\n", __func__);
		return 0;
	}

	boot_vect = octeon_get_boot_vector();

	debug("%s: Boot vector at %p\n", __func__, boot_vect);

	if (!boot_vect) {
		printf("%s: Error: boot vector unavailable\n", __func__);
		return -1;
	}

	cvmx_coremask_for_each_core(core, pcm) {
		/* Have other cores jump directly to DRAM, rather than running
		 * out of flash.  This address is set here because is is the
		 * address in DRAM, rather than the address in FLASH.
		 */
#if 0
		debug("u-boot ram base: 0x%llx\n", gd->bd->bi_uboot_ram_addr);

		debug("address of initTLBSTart: %p, start: %p\n",
		       &InitTLBStart, &uboot_start);

		debug("addr to jump to: 0x%llx\n",
		      gd->bd->bi_uboot_ram_addr + ((char *)&InitTLBStart -
						   (char *)&uboot_start));
#endif
		/* Here we need to figure out the physical address of the
		 * InitTLBStart function.
		 */
		boot_vect[core].code_addr =
			MAKE_XKPHYS(uboot_tlb_ptr_to_phys(&InitTLBStart));
		debug("Setting core %d code_addr address(0x%p) to: 0x%llx\n",
		      core, &(boot_vect[core].code_addr),
		      boot_vect[core].code_addr);
		if (func_addr) {
			/* The app_start_func_addr will be a virtual address
			 * that is used after the TLB has been set up.
			 */
			boot_vect[core].app_start_func_addr = func_addr;
#if 0
			debug("Setting core %d app_start address(0x%p) to: 0x%lx\n",
			      core, &(boot_vect[core].app_start_func_addr),
			      boot_vect[core].app_start_func_addr);
#endif
		} else {
			/* If fewer than all cores are run, set rest to stop,
			 * otherwise simulator runs forever
			 */
			boot_vect[core].app_start_func_addr =
					(uint32_t) & idle_core_start_app;
		}

		boot_vect[core].k0_val = (uint32_t) gd;	/* pass global data pointer */

		boot_vect[core].boot_info_addr =
		    (uint32_t) (&boot_info_block_array[core]);
		debug("boot_info_addr: 0x%llx\n",
		      boot_vect[core].boot_info_addr);
	}

	core = cvmx_coremask_get_first_core(pcm);
	debug("core: 0x%x, coremask:\n", core);
	cvmx_coremask_dprint(pcm);
	debug("Address of start app for start core %d: 0x%08x\n", core,
	      boot_vect[core].app_start_func_addr);

	OCTEON_SYNC;
	return 0;
}

cvmx_coremask_t *get_coremask_override(cvmx_coremask_t *pcm)
{
	cvmx_coremask_t pcm_override = CVMX_COREMASK_MAX;
	char *cptr;
	/* The old code sets the number of cores to be to 16 in this case. */
        cvmx_coremask_set_cores(pcm, 0, 16);

	if (OCTEON_IS_OCTEON2() || OCTEON_IS_OCTEON3())
		cvmx_coremask_copy(pcm, &pcm_override);

	cptr = getenv("coremask_override");
	if (cptr) {
		if (cvmx_coremask_str2bmp(pcm, cptr) < 0)
			return NULL;
	}
	return pcm;

}

/* Validate the coremask that is passed to a boot* function. */
int validate_coremask(cvmx_coremask_t *pcm)
{
	cvmx_coremask_t coremask_override;
	cvmx_coremask_t fuse_coremask;
	if (!get_coremask_override(&coremask_override))
		return -1;

	octeon_get_available_coremask(&fuse_coremask);

	if (!cvmx_coremask_is_subset(&fuse_coremask, pcm)) {
		puts("ERROR: Can't boot cores that don't exist!\nAvailable coremask:\n");
		cvmx_coremask_print(&fuse_coremask);
		return -1;
	}

	if (!cvmx_coremask_is_subset(&coremask_override, pcm)) {
		cvmx_coremask_t print_cm;
		puts("Notice: coremask changed from:\n");
		cvmx_coremask_print(pcm);
		puts("based on coremask_override of:\n");
		cvmx_coremask_print(&coremask_override);
		cvmx_coremask_and(&print_cm, pcm, &coremask_override);
		puts("to:\n");
		cvmx_coremask_print(&print_cm);
	}
#if 0
	if (core_mask & ~coremask_from_eeprom) {
		/* The user has manually changed the coremask_override env.
		 * variable to run code on known bad cores.  Print a warning
		 * message.
		 */
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING: You have changed the coremask_override and are running code on non-functional cores.\n");
		puts("WARNING: The program may crash and/or produce unreliable results.\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		puts("WARNING:\n");
		int delay = 5;
		do {
			printf("%d    \r", delay);
			cvmx_wait(600000000ULL);	/* Wait a while to make sure warning is seen. */
		} while (delay-- > 0);
		puts("\n");
	}
#endif
	return 0;
}

/**
 * In CIU_FUSE for the 78XX, odd and even cores are separated out.
 * For example, a CIU_FUSE value of 0xfffffefffffe indicates that bits 0 and 1
 * are set.
 * This function converts the bit number in the CIU_FUSE register to a
 * physical core number.
 */
static int convert_ciu_fuse_to_physical_core(int core, int max_cores)
{
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return core;
    	else if (!OCTEON_IS_MODEL(OCTEON_CN78XX))
        	return core;
	else if (core < (max_cores / 2))
		return core * 2;
	else
		return ((core - (max_cores / 2)) * 2) + 1;
}

/**
 * Get the total number of fuses blown as well as the number blown per tad.
 *
 * @param	coremask	fuse coremask
 * @param[out]	tad_blown_count	number of cores blown for each tad
 * @param	num_tads	number of tads
 * @param	max_cores	maximum number of cores
 *
 * @return	void
 */
void fill_tad_corecount(uint64_t coremask, int tad_blown_count[], int num_tads,
		      int max_cores)
{
	int core, physical_core;

	for (core = 0; core < max_cores; core++) {
		if (!(coremask & (1ULL << core))) {
			int tad;
			physical_core =
				convert_ciu_fuse_to_physical_core(core,
								  max_cores);
			tad = physical_core % num_tads;
			tad_blown_count[tad]++;
		}
	}
}

uint64_t get_core_pattern(int num_tads, int max_cores)
{
	uint64_t pattern = 1ULL;
	int cnt;

	for (cnt = 1; cnt < (max_cores / num_tads); cnt++)
		pattern |= pattern << num_tads;

	return pattern;
}

/**
 * For CN78XX and CN68XX this function returns the logical coremask from the
 * CIU_FUSE register value. For other models there is no difference.
 *
 * @param ciu_fuse_value	fuse value from CIU_FUSE register
 * @return logical coremask of CIU_FUSE value.
 */
uint64_t get_logical_coremask(uint64_t ciu_fuse_value)
{
	int tad_blown_count[MAX_CORE_TADS] = {0};
	int tad;
	uint64_t logical_coremask = 0;
	uint64_t tad_mask, pattern;
	int num_tads, max_cores;

#if CONFIG_OCTEON_SIM_SW_DIFF
	return ciu_fuse_value;
#endif

	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		num_tads = 8;
		max_cores = 48;
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		   OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		num_tads = 4;
		max_cores = 16;
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		num_tads = 4;
		max_cores = 32;
	} else {
		/* Most Octeon devices don't need any mapping. */
		return ciu_fuse_value;
	}

	pattern = get_core_pattern(num_tads, max_cores);
	fill_tad_corecount(ciu_fuse_value, tad_blown_count,
					      num_tads, max_cores);

	for (tad = 0; tad < num_tads; tad++) {
		tad_mask = pattern << tad;
		logical_coremask |= tad_mask >> (tad_blown_count[tad] * num_tads);
	}
	return logical_coremask;
}

/**
 * Returns the available coremask either from env or fuses.
 * If the fuses are blown and locked, they are the definitive coremask.
 *
 * @param pcm	pointer to coremask to fill in
 * @return pointer to coremask
 */
cvmx_coremask_t *octeon_get_available_coremask(cvmx_coremask_t *pcm)
{
	uint64_t cores;
	uint64_t ciu_fuse;
	cvmx_coremask_clear_all(pcm);

	if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
		int node;

		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			ciu_fuse = (cvmx_read_csr_node(node, CVMX_CIU_FUSE)
						       & 0x0000FFFFFFFFFFFFULL);

			ciu_fuse = get_logical_coremask(ciu_fuse);
			cvmx_coremask_set64_node(pcm, node, ciu_fuse);
		}
		return pcm;
	} else {
		ciu_fuse = (cvmx_read_csr(CVMX_CIU_FUSE) & 0x0000FFFFFFFFFFFFULL);
		ciu_fuse = get_logical_coremask(ciu_fuse);
	}

	if (OCTEON_IS_MODEL(OCTEON_CN38XX)) {
		/* Here we only need it if no core fuses are blown and the
		 * lockdown fuse is not blown.  In all other cases the cores
		 * fuses are definitive and we don't need a coremask override.
		 */
		if (ciu_fuse == 0xffff && !cvmx_octeon_fuse_locked()) {
			/* get coremask from environment */
			return get_coremask_override(pcm);
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN58XX)
		   || OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		/* Some early chips did not have fuses blown to follow the
		 * even/odd requirements.
		 * Set the coremask to take this possibility into account.
		 */
		uint32_t coremask = 0;

		/* Do even cores */
		cores = __builtin_popcountll(ciu_fuse & 0x5555);
		coremask |= 0x5555 >> (16 - 2 * cores);
		/* Do odd cores */
		cores = __builtin_popcountll(ciu_fuse & 0xAAAA);
		coremask |= 0xAAAA >> (16 - 2 * cores);
		cvmx_coremask_set64(pcm, coremask);
		return pcm;
	} else if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		cvmx_coremask_set64(pcm, ciu_fuse);
	}

	/* Get number of cores from fuse register, convert to coremask */
	cores = __builtin_popcountll(ciu_fuse);

	cvmx_coremask_set_cores(pcm, 0, cores);

	return pcm;
}

uint32_t octeon_get_available_core_count(void)
{
	cvmx_coremask_t coremask;
	octeon_get_available_coremask(&coremask);
	return cvmx_coremask_get_core_count(&coremask);
}

static void setup_env_named_block(void)
{
	int64_t addr;
	int i;
	char *env_ptr = (void *)gd->env_addr;
	int env_size;

	/* Determine the actual size of the environment that is used,
	 * look for two NULs in a row
	 */
	for (i = 0; i < ENV_SIZE - 1; i++) {
		if (env_ptr[i] == 0 && env_ptr[i + 1] == 0)
			break;
	}
	env_size = i + 1;

	/* Allocate a named block for the environment */
	addr = cvmx_bootmem_phy_named_block_alloc(env_size + 1, 0ull,
						  1ull << 29, 0ull,
						  OCTEON_ENV_MEM_NAME,
						  CVMX_BOOTMEM_FLAG_END_ALLOC);
	if (addr >= 0) {
		char *named_ptr = cvmx_phys_to_ptr(addr);
		if (named_ptr) {
			memset(named_ptr, 0x0, env_size + 1);
			memcpy(named_ptr, env_ptr, env_size);
		}
	}
}

void start_cores(cvmx_coremask_t *pcm_to_start)
{
	int node;
	cvmx_cores_common_bootinfo_t *ccbi;
	cvmx_coremask_t invert_coremask = CVMX_COREMASK_MAX;
	cvmx_coremask_t avail_coremask;
	boot_init_vector_t *boot_vect = octeon_get_boot_vector();

	/* create named block with extra bootinfo common to all cores */
	ccbi = cvmx_bootmem_alloc_named_range_once(
					CVMX_APP_COMMON_BOOTINFO_SIZE,
					0x0, 1ull << 28, 0,
					CVMX_APP_COMMON_BOOTINFO_NAME,
					cores_common_bootinfo_init);

	if (!ccbi) {
		printf("Error: cvmx_bootmem_alloc_named_range_once(%s)\n",
			CVMX_APP_COMMON_BOOTINFO_NAME);
		return;
	}

#ifdef DEBUG
	debug("Bringing coremask out of reset:\n");
	cvmx_coremask_print(pcm_to_start);
#endif

	/* Set cores that are not going to be started to branch to a 'break'
	 * instruction at boot
	 */
	cvmx_coremask_clear_all(&invert_coremask);
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		cvmx_coremask_set64_node(&invert_coremask, node,
					(1ULL << CVMX_MAX_CORES) - 1);
	}
	cvmx_coremask_xor(&invert_coremask, &invert_coremask, pcm_to_start);

	debug("%s: octeon_setup_boot_vector(0, &invert_coremask) invert_coremask...\n", __func__);
	cvmx_coremask_dprint(&invert_coremask);

	if (cvmx_coremask_get_core_count(&invert_coremask))
		octeon_setup_boot_vector(0, &invert_coremask);

	setup_env_named_block();

	debug("Boot vector base: %p\n", boot_vect);
	debug("App start function addr: 0x%x\n", boot_vect[0].app_start_func_addr);

	/* Clear cycle counter so that all cores will be close */
	debug("Bootloader: Starting app at cycle: %d\n",
	      (uint32_t) boot_cycle_adjustment);

	boot_cycle_adjustment = octeon_get_cycles();
	OCTEON_SYNC;

#if defined(__U_BOOT__)
	/* Cores have already been taken out of reset to conserve power.
	 * We need to send a NMI to get the cores out of their wait loop
	 */
	octeon_get_available_coremask(&avail_coremask);
	debug("Available coremask:\n");
	cvmx_coremask_dprint(&avail_coremask);
	debug("Starting coremask:\n");
	cvmx_coremask_dprint(pcm_to_start);
	debug("Sending NMIs to other cores\n");
	if (octeon_has_feature(OCTEON_FEATURE_CIU3)) {
		uint64_t avail_cm;
		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			avail_cm = cvmx_coremask_get64_node(&avail_coremask,
							    node);

			if (avail_cm != 0) {
				debug("Sending NMI  to node %d, coremask=0x%llx, CIU3_NMI=0x%llx\n",
				      node, avail_cm, (node > 0 ? -1ull : -2ull) & avail_cm);
				cvmx_write_csr_node(node, CVMX_CIU3_NMI,
			       			    (node > 0 ? -1ull : -2ull) & avail_cm);
			}
		}
	} else {
		cvmx_write_csr(CVMX_CIU_NMI,
			       -2ull & cvmx_coremask_get64(&avail_coremask));
	}

#else
	/* hotplug uses this part, we send NMI to all cores, regardless */
	cvmx_write_csr(CVMX_CIU_NMI,
		       -1ull & cvmx_coremask_get64(&avail_coremask));
#endif

	debug("Done sending NMIs\n");
	if (!(cvmx_coremask_is_core_first_core(pcm_to_start, 0))) {
#if 0
		uint64_t powthrottle=0;
		debug("Core 0 is not first core\n");
		/* core 0 not being run, so just loop here */
		/* This is disabled for now since start_core0 will never be
		 * true.
		 */
		if (OCTEON_IS_OCTEON3()) {

			/* crank up power throttling */
			__asm__ __volatile__ (
				"	.set push			\n"
				"	.set mips64			\n"
				"	.set arch=octeon		\n"
				"	dmfc0	%[powthrottle], $11,6	\n"
				"	move	$24, %[powthrottle]	\n"
				"	dins	$24, $0, 16, 8\n"
				"	dmtc0	$24, $11,6		\n"
				"	.set pop			\n"
				: [powthrottle] "=r" (powthrottle)
				:: "$24");
		}
#endif
		while (!start_core0)
			/* We wait because for now start_core0 will never be
			 * set true!  Reduces power usage.  If this changes
			 * re-enable the power throttling and replace with
			 * ';'
			 */
#ifdef CONFIG_OCTEON_SIM_SW_DIFF
			__asm__ __volatile__ ("break");
#else
			__asm__ __volatile__ ("wait");
#endif

#if 0
		if (OCTEON_IS_OCTEON3()) {
			/* restore power throttling */
			__asm__ __volatile__ (
				"	.set push			\n"
				"	.set mips64			\n"
				"	dmtc0	%[powthrottle], $11,6	\n"
				"	.set pop			\n"
				:: [powthrottle] "r" (powthrottle));
		}
#endif
	}
	debug("Branching to app_start_func_addr 0x%08x\n",
	      boot_vect[0].app_start_func_addr);

	/* Run the app_start_func for the core - if core 0 is not being run,
	 * this will branch to a 'break' instruction to stop simulation.
	 */
	((void (*)(void))boot_vect[0].app_start_func_addr) ();
}

/**
 * This synchronizes all of the cores but this code looks broken if we're
 * purely relying on the delay.
 */
void octeon_sync_cores(void)
{
	int core = cvmx_get_core_num();
	cvmx_coremask_barrier_sync(&cvmx_bootinfo_array[core].ext_core_mask);
}

#if (CONFIG_OCTEON_EBT3000 || CONFIG_OCTEON_EBT5800)
int octeon_ebt3000_rev1(void)
{
	uint64_t addr;

	addr = gd->arch.pal_addr;

	return (!(octeon_read64_byte(addr+0) == 0xa5
	        && octeon_read64_byte(addr+1) == 0x5a));
}
#endif

#if (BOARD_MCU_AVAILABLE)
int octeon_mcu_read_cpu_ref(void)
{

	if (twsii_mcu_read(0x00) == 0xa5 && twsii_mcu_read(0x01) == 0x5a)
		return (((twsii_mcu_read(6) << 8) + twsii_mcu_read(7)) / (8));
	else
		return (-1);

}

int octeon_mcu_read_ddr_clock(void)
{
	uint64_t addr;

#ifndef OCTEON_PAL_BASE_ADDR
	if (octeon_boot_bus_get_dev_info("/soc/bootbus/pal",
					 "cavium,ebt3000-pal", &addr, NULL))
		return -1;
#else
	addr = OCTEON_PAL_BASE_ADDR;
#endif
	if (twsii_mcu_read(0x00) == 0xa5 && twsii_mcu_read(0x01) == 0x5a)
		return ((((twsii_mcu_read(4) << 8) +
			  twsii_mcu_read(5)) /
			   (1 + octeon_read64_byte(addr + 7))));
	else
		return (-1);

}
#endif

/* Restore the core configuration to be closer to standard
 * MIPS configuration.  This is used by NAND boot stage 2
 * to set up the environment before starting stage 3.
 * We don't free named blocks here, because that may corrupt
 * the stage 3 image that has been loaded.  Stage 3 is responsible
 * for its own memory management.
 */
void octeon_setup_ram_env(void)
{
	char *c_ptr = (void *)0x1004;
	uint32_t *ulptr = (void *)0x1000;

	c_ptr += sprintf(c_ptr, "autoload=n") + 1;
	c_ptr +=
	    sprintf(c_ptr, "dram_size_mbytes=%llu",
		    gd->ram_size / (1024 * 1024)) + 1;
	*c_ptr = '\0';		/* Terminate with two NULLS */

	*ulptr = crc32(0, (void *)0x1004, 0xffc);

}

void octeon_restore_std_mips_config(void)
{
	uint64_t val;

	debug("%s: Entry\n", __func__);
#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_disable();
#endif
#ifdef CONFIG_OCTEON_INTERNAL_ENET
	debug("%s: Shutting down network\n", __func__);
	octeon_network_hw_shutdown(1);
#endif

#ifdef CONFIG_OCTEON_MGMT_ENET
	debug("%s: Shutting down management\n", __func__);
	octeon_mgmt_port_shutdown();
#endif

#ifdef CONFIG_USB_OCTEON
	debug("%s: Stopping USB\n", __func__);
	usb_stop();
#endif
	/* Clear any memory access errors */
	octeon_dram_clear_access_errors(true);

	/* Disable CVMSEG */
	/* Should entire register be set to default ? */
	val = get_cop0_cvmmemctl_reg();
	val &= ~0x1ffull;
	set_cop0_cvmmemctl_reg(val);
	/* Restore EBASE register */
	val = get_except_base_addr();
	val &= 0xfff;		/* Preserve CPU core number */
	val |= 0x80000000;
	set_except_base_addr(val);
}

int octeon_bootloader_shutdown(void)
{
	int rc;
	bool api_enable = getenv("api_enable") != NULL;

	debug("%s: Entry\n", __func__);

#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_disable();
#endif

	if (!api_enable) {
#ifdef CONFIG_OCTEON_INTERNAL_ENET
		debug("%s: Shutting down network\n", __func__);
		octeon_network_hw_shutdown(1);
#endif

#ifdef CONFIG_OCTEON_MGMT_ENET
		debug("%s: Shutting down management\n", __func__);
		octeon_mgmt_port_shutdown();
#endif

#if defined(CONFIG_USB_OCTEON) || defined(CONFIG_USB_EHCI) || \
    defined(CONFIG_USB_XHCI)
		debug("%s: Stopping USB\n", __func__);
		usb_stop();
#endif

#ifdef CONFIG_OCTEON_HW_BCH
		cvmx_bch_shutdown();
#endif
	}
	/* Clear any memory access errors */
	octeon_dram_clear_access_errors(true);

	if (!api_enable) {
		/* Free temp blocks last, as previous systems being shut down
		 * may still rely on them
		 */
		octeon_free_tmp_named_blocks();
	}

	/* Reserve lower 1 MB after freeing all temp named blocks */
	rc = cvmx_bootmem_reserve_memory(0,
					 OCTEON_RESERVED_LOW_BOOT_MEM_SIZE,
					 "__low_reserved", 0);
	if (!rc) {
		printf("Error reserving low 1MB of memory\n");
	}
	return 0;
}

void arch_preboot_os(void)
{
	octeon_bootloader_shutdown();
}

void octeon_free_tmp_named_blocks(void)
{
	int i;
	cvmx_bootmem_desc_t *bootmem_desc_ptr =
	    __cvmx_bootmem_internal_get_desc_ptr();

	cvmx_bootmem_named_block_desc_t *named_block_ptr =
	    cvmx_phys_to_ptr(bootmem_desc_ptr->named_block_array_addr);
	for (i = 0; i < bootmem_desc_ptr->named_block_num_blocks; i++) {
		if (named_block_ptr[i].size &&
		    !strncmp(OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX,
			     named_block_ptr[i].name,
			     strlen(OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX))) {
			/* Found temp block, so free */
			debug("%s: Freeing %s\n", __func__,
			      named_block_ptr[i].name);
			cvmx_bootmem_phy_named_block_free(named_block_ptr[i].name,
							  0);
		}
	}
}

/* Checks to see if the just completed load is within the bounds of the
 * reserved memory block, and prints a warning if it is not.
 */
void octeon_check_mem_load_range(void)
{
	/* Check to see if we have overflowed the reserved memory for loading */
	if (load_reserved_size) {
		ulong faddr = 0;
		ulong fsize = 0;
		char *s;

		if ((s = getenv("fileaddr")) != NULL)
			faddr = simple_strtoul(s, NULL, 16);
		if ((s = getenv("filesize")) != NULL)
			fsize = simple_strtoul(s, NULL, 16);
		if (faddr < load_reserved_addr
		    || faddr > (load_reserved_addr + load_reserved_size)
		    || (faddr + fsize) > (load_reserved_addr + load_reserved_size)) {
			/* We have loaded some data outside of the reserved
			 * area.
			 */
			puts("WARNING: Data loaded outside of the reserved "
			     "load area, memory corruption may occur.\n");
			puts("WARNING: Please refer to the bootloader memory "
			     "map documentation for more information.\n");
		}

	}
}

/* We need to provide a sysinfo structure so that we can use parts of the
 * simple executive
 */
void octeon_setup_cvmx_sysinfo(void)
{
	struct cvmx_sysinfo *si;

	cvmx_sysinfo_minimal_initialize(0x1fffffff & (unsigned long)
					__cvmx_bootmem_internal_get_desc_ptr(),
					gd->arch.board_desc.board_type,
					gd->arch.board_desc.rev_major,
					gd->arch.board_desc.rev_minor,
					gd->cpu_clk);

	si = cvmx_sysinfo_get();
#ifdef CONFIG_CMD_FDT
	si->fdt_addr = (uint32_t)working_fdt;
#else
	si->fdt_addr = (uint32_t)gd->fdt_blob;
#endif
}

#endif /* defined(__U_BOOT__) */
