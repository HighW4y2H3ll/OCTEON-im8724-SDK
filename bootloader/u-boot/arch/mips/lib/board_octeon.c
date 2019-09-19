/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2004-2014
 * Cavium Inc., Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <net.h>
#include <environment.h>
#include <nand.h>
#include <flash.h>
#include <nand.h>
#include <addr_map.h>
#include <errno.h>
#include <watchdog.h>
#include <asm/mipsregs.h>
#include <sata.h>
#include <miiphy.h>
#include <linux/ctype.h>
#include <search.h>
#include <fdt_support.h>
#ifdef CONFIG_CMD_USB
# include <usb.h>
#endif
#ifdef CONFIG_CMD_DTT
# include <dtt.h>
#endif
#if CONFIG_OCTEON_PCI_HOST
#include <asm/io.h>
#endif
#include <asm/addrspace.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-core.h>
#include <asm/arch/cvmx-bootloader.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon-boot-info.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon-pci-console.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/cvmx-adma.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/arch/cvmx-l2c.h>
#include <asm/arch/octeon_cf.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_mdio.h>
#include <asm/arch/octeon_dram.h>
#include <asm/arch/octeon_errata.h>
#ifdef CONFIG_OCTEON_SE_NAPI
# include <asm/arch/seapi_public.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/** Short cut to convert a number to megabytes */
#define MB(X)	((uint64_t)(X) * (uint64_t)(1024 * 1024))

#define OCTEON_MAC_MEMINFO_BLOCK_NAME	"__mac_meminfo"

#define MAC_MEMINFO_ALIGNMENT		(8)
#define MAC_MEMINFO_ALIGNMASK		(MAC_MEMINFO_ALIGNMENT - 1)

struct mac_sharedmem_info {
	uint16_t count;			/** Number of MAC addresses */
	uint8_t base_mac_addr[6];	/** Starting MAC Address */
};

extern ulong uboot_end_data;
extern ulong uboot_end;
extern char uboot_start;
#ifdef DEBUG
extern void *_start;
#endif

extern char uboot_prompt[];
#ifdef CONFIG_SYS_HUSH_PARSER
extern char uboot_prompt_hush_ps2[];
#endif

#if CONFIG_OCTEON_PCI_HOST
extern volatile uint64_t pci_mem1_base;
extern volatile uint64_t pci_mem2_base;
extern volatile uint64_t pci_mem3_base;
extern volatile uint64_t pci_mem4_base;
#endif

extern int octeon_check_env(const env_t *env, size_t size);

#if defined(CONFIG_HW_WATCHDOG)
extern void hw_watchdog_start(int msecs);
#endif

#ifdef CONFIG_CMD_SPI
extern int spi_init(void);
#endif

#ifdef CONFIG_OCTEON_SE_NAPI
extern int seapi_init(void);
#endif

/* Device tree */
extern char __dtb_begin;
extern char __dtb_end;

#ifdef CONFIG_OCTEON_MMC
extern int mmc_initialize(bd_t *bis);
#endif

#ifdef CONFIG_OCTEON_BOOTCMD
extern int octeon_pci_bootcmd_unlock(void);
#endif

#ifdef CONFIG_SYS_PCI_CONSOLE
extern int uboot_octeon_pci_console_init(void);
#endif

#ifdef CONFIG_CMD_NVME
extern int nvme_initialize(void);
#endif

static struct mac_sharedmem_info *global_mac_info;

ulong monitor_flash_len;

const char version_string[] =
    U_BOOT_VERSION " (Build time: " U_BOOT_DATE " - " U_BOOT_TIME ")";

/*
 * mips_io_port_base is the begin of the address space to which x86 style
 * I/O ports are mapped.
 */
const unsigned long mips_io_port_base = 0;

/* Used for error checking on load commands */
ulong load_reserved_addr = 0;	/* Base address of reserved memory for loading */
ulong load_reserved_size = 0;	/* Size of reserved memory for loading */

#ifdef CONFIG_OCTEON_OCX
extern int oci_init(void);
#endif
extern int timer_init(void);
extern int octeon_bist_start(void);
extern int octeon_bist(void);
extern int octeon_env_init(void);
extern int init_twsi_bus(void);
#ifdef CONFIG_CMD_DTT
extern void dtt_init(void);
#endif
#ifdef CONFIG_CMD_IDE
/* defined in octeon_board_cf.c or board specific file. */
extern int octeon_board_ide_init(void);
#endif
/* Defined in board/octeon/common/octeon_board_display.c */
extern int display_board_info(void);

#if defined(CONFIG_OCTEON_FLASH) && !defined(CONFIG_ENV_IS_IN_NAND)
/* Defined in octeon_flash.c */
extern void octeon_fix_env_reloc(void);
#endif

#if (defined(CONFIG_CMD_OCTEON_NAND) || defined(CONFIG_CMD_NAND)) \
	&& !CONFIG_OCTEON_SIM_NO_FLASH
extern int oct_nand_probe(void);
#endif

#ifdef CONFIG_OCTEON_I2C_FDT
int octeon_i2c_process_fdt(void);
#endif

#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
int octeon_remote_hotplug_init(void);
#endif

#define SET_K0(val)	\
	asm volatile ("add $26, $0, %[rt]" :: [rt] "d" (val):"memory")

#define STACK_SIZE	(64*1024UL + CONFIG_SYS_BOOTPARAMS_LEN)

int __board_early_init_f(void)
{
	/*
	 * Nothing to do in this dummy implementation
	 */
	return 0;
}
int board_early_init_f(void)
	__attribute__((weak, alias("__board_early_init_f")));

/**
 * Early UART initialization
 */
int __early_uart_init_f(void)
{
#ifdef CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT
	gd->arch.console_uart = CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT;
#endif
	/* Initialize the serial port early */
	gd->baudrate = CONFIG_BAUDRATE;
#ifdef CONFIG_OCTEON_SERIAL
	octeon_uart_setup(gd->arch.console_uart);
#endif
	return 0;
}
int early_uart_init_f(void) __attribute__((weak,alias("__early_uart_init_f")));

/**
 * Perform pre-relocation FDT initialization.
 */
int __board_fdt_init_f(void)
{
	gd->fdt_blob = (void *)MAKE_KSEG0((uint32_t)(&__dtb_begin));
	return fdt_check_header(gd->fdt_blob);
}

int board_fdt_init_f(void)
	__attribute__((weak, alias("__board_fdt_init_f")));

/**
 * This function is called at the beginning of running after relocation
 */
int early_board_init_r(void)
{
	return 0;
}
int early_board_init_r(void) __attribute__((weak));

void board_configure_qlms(void)
{
}
void board_configure_qlms(void) __attribute__((weak));

void __board_fdt_get_limits(char **begin, char **end)
{
	*begin = &__dtb_begin;
	*end = &__dtb_end;
}

void board_fdt_get_limits(char **begin, char **end)
	__attribute__((weak, alias("__board_fdt_get_limits")));

#ifdef CONFIG_CMD_NET
void board_net_preinit(void)
{
	return;
}

void board_net_preinit(void)
	__attribute__((weak));

void board_net_postinit(void)
{
	return;
}

void board_net_postinit(void)
	__attribute__((weak));
#endif

int misc_init_r(void)
{
	return 0;
}
int misc_init_r(void) __attribute__((weak));

int octeon_mac_info_init(void)
	__attribute__((weak, alias("__octeon_mac_info_init")));
int __octeon_mac_info_init(void)
{
	size_t size = sizeof(struct mac_sharedmem_info) +
					(gd->arch.mac_desc.count * 6);

	if (!global_mac_info) {
		global_mac_info =
		cvmx_bootmem_alloc_named_range_once(size, 0, 0,
						    MAC_MEMINFO_ALIGNMENT,
				      OCTEON_MAC_MEMINFO_BLOCK_NAME, NULL);
		if (!global_mac_info) {
			printf("%s: Error: could not allocate named block %s\n",
			       __func__, OCTEON_MAC_MEMINFO_BLOCK_NAME);
			return -1;
		}
	}
	global_mac_info->count = gd->arch.mac_desc.count;
	memcpy((void *)global_mac_info->base_mac_addr,
	       (void *)gd->arch.mac_desc.mac_addr_base, 6);
	return 0;
}

/**
 * This function sets up the CPU and bus clocks early in the boot process.
 * The DRAM clock will be configured later.
 */
int octeon_setup_clocks(void)
	__attribute__((weak, alias("__octeon_setup_clocks")));

int __octeon_setup_clocks(void)
{
	const u64 ref_clock = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull);

	if (octeon_has_feature(OCTEON_FEATURE_NPEI)) {
		cvmx_npei_dbg_data_t npei_dbg_data;
		npei_dbg_data.u64 = cvmx_read_csr(CVMX_PEXP_NPEI_DBG_DATA);
		gd->cpu_clk = ref_clock * npei_dbg_data.s.c_mul;
		gd->bus_clk = gd->cpu_clk;
	} else if (OCTEON_IS_OCTEON3()) {
		cvmx_rst_boot_t rst_boot;
		rst_boot.u64 = cvmx_read_csr_node(0, CVMX_RST_BOOT);
		gd->cpu_clk = ref_clock * rst_boot.s.c_mul;
		gd->bus_clk = ref_clock * rst_boot.s.pnr_mul;
	} else if (octeon_has_feature(OCTEON_FEATURE_PCIE)) {
		cvmx_mio_rst_boot_t mio_rst_boot;
		mio_rst_boot.u64 = cvmx_read_csr(CVMX_MIO_RST_BOOT);
		gd->cpu_clk = ref_clock * mio_rst_boot.s.c_mul;
		gd->bus_clk = ref_clock * mio_rst_boot.s.pnr_mul;
	} else {
		cvmx_dbg_data_t dbg_data;
		dbg_data.u64 = cvmx_read_csr(CVMX_DBG_DATA);
		gd->cpu_clk = ref_clock * dbg_data.s.c_mul;
		gd->bus_clk = gd->cpu_clk;
	}
	debug("%s: cpu: %lu, bus: %lu\n", __func__, gd->cpu_clk, gd->bus_clk);
	debug("%s: core: %llu, I/O: %llu\n", __func__,
	      cvmx_clock_get_rate(CVMX_CLOCK_CORE),
	      cvmx_clock_get_rate(CVMX_CLOCK_SCLK));
	return 0;
}

static void octeon_print_octeon_ids(void)
{
	int node;
	uint8_t buffer[10];

	printf("Octeon unique ID%s: ", gd->arch.node_mask == 1 ? "" : "s");
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		if (octeon_get_unique_id(node, buffer))
			printf("%snot set", node ? ", " : "");
		else
			printf("%s%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			       node ? ", " : "", buffer[0], buffer[1],
			       buffer[2], buffer[3], buffer[4], buffer[5],
			       buffer[6], buffer[7], buffer[8], buffer[9]);
	}
	puts("\n");
}

static int display_banner(void)
{

	printf("\n\n%s\n\n", version_string);
	if (!OCTEON_IS_OCTEON1PLUS())
		octeon_print_octeon_ids();
	return 0;
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config(ulong size)
{
	puts("Flash: ");
	print_size(size, "\n");
}

/**
 * Configures the NOR flash
 */
void __octeon_configure_nor_flash(void)
{
	int size;
	bd_t *bd = gd->bd;

	if (octeon_check_nor_flash_enabled(gd->arch.flash_base_address)) {
		debug("%s: NOR flash detected at 0x%x\n",
		      __func__, gd->arch.flash_base_address);
		size = flash_init();
		display_flash_config(size);
		if (bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE) {
			gd->arch.flash_base_address = 0x1fc00000 - size;
			gd->arch.flash_size = size;
		}
		bd->bi_flashstart = gd->arch.flash_base_address;
		bd->bi_flashsize = size;
#if CONFIG_SYS_MONITOR_BASE == CONFIG_SYS_FLASH_BASE
		/* reserved area for U-Boot */
		bd->bi_flashoffset = monitor_flash_len;
#else
		bd->bi_flashoffset = 0;
#endif
	} else {
		printf("Flash boot bus region not enabled, skipping NOR flash config\n");
		bd->bi_flashstart = 0;
		bd->bi_flashsize = 0;
	}
}

void octeon_configure_nor_flash(void)
	__attribute__((weak, alias("__octeon_configure_nor_flash")));
#endif

static int init_baudrate(void)
{
	char tmp[64];		/* long enough for environment variables */
	int i = getenv_f("baudrate", tmp, sizeof(tmp));

	gd->baudrate = (i > 0)
	    ? (int)simple_strtoul(tmp, NULL, 10)
	    : CONFIG_BAUDRATE;

	return 0;
}

/**
 * Check if address 0 has has been written and prints a message if it has.
 *
 * @param msg	Optional message to add to error message, can be NULL.
 */
static void null_ptr_check(const char *msg)
{
	if (*(u64 *)MAKE_KSEG0(0) != 0ull)
		printf("Error: detected write to NULL pointer. %s\n",
		       msg ? msg : "");
}

/**
 * Debug version of null_ptr_check()
 *
 * @param msg	Optional message to add to error message, can be NULL.
 */
static inline void dnull_ptr_check(const char *msg)
{
#ifdef DEBUG
	cvmx_l2c_int_reg_t l2c_int_reg;
	null_ptr_check(msg);
	if (OCTEON_IS_OCTEON2()) {
		l2c_int_reg.u64 = cvmx_read_csr(CVMX_L2C_INT_REG);
		if (l2c_int_reg.u64 != 0)
			printf("L2C_INT_REG: %s\n"
			       "  holerd:       %d\n"
			       "  holewr:       %d\n"
			       "  vrtwr:        %d\n"
			       "  vrtidrng:     %d\n"
			       "  vrtadrng:     %d\n"
			       "  vrtpe:        %d\n"
			       "  bigwr:        %d\n"
			       "  bigrd:        %d\n"
			       "  tad0:         %d\n"
			       "  tad1:         %d\n"
			       "  tad2:         %d\n"
			       "  tad3:         %d\n",
			       msg,
			       l2c_int_reg.s.holerd, l2c_int_reg.s.holewr,
			       l2c_int_reg.s.vrtwr, l2c_int_reg.s.vrtidrng,
			       l2c_int_reg.s.vrtadrng, l2c_int_reg.s.vrtpe,
			       l2c_int_reg.s.bigwr, l2c_int_reg.s.bigrd,
			       l2c_int_reg.s.tad0, l2c_int_reg.s.tad1,
			       l2c_int_reg.s.tad2, l2c_int_reg.s.tad3);
	}
#endif
}

/*
 * Breath some life into the board...
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */
#ifndef CONFIG_OCTEON_NO_BOOT_BUS
int octeon_boot_bus_init_board(void);
#endif
int early_board_init(void);
int failsafe_scan(void);
int octeon_bist(void);
int late_board_init(void);

int octeon_dram_test(void) __attribute__((weak));
int octeon_dram_test(void)
{
	debug("Empty memory test\n");
	return 0;
}

/**
 * Enables console output early since the serial port is initialized early.
 */
static int octeon_early_console_init_f(void)
{
	gd->have_console = 1;
	return 0;
}

#if !CONFIG_OCTEON_SIM_NO_FLASH && !defined(CONFIG_SYS_NO_FLASH) && \
    !defined(CONFIG_OCTEON_NAND_STAGE2) && \
    !defined(CONFIG_OCTEON_EMMC_STAGE2) && \
    !defined(CONFIG_OCTEON_SPI_STAGE2) && \
    !defined(CONFIG_OCTEON_AUTHENTIK_STAGE2)

extern flash_info_t flash_info[];
u32 octeon_find_and_validate_normal_bootloader(int rescan_flag);
u32 get_image_size(const bootloader_header_t * header);

/**
 * Initializes U-Boot flash environment settings and protects bootloader flash
 * areas.
 *
 * @return 0 for success, -1 on error
 */
static int octeon_boot_init_flash(void)
{
	int rc = 0;
	flash_info_t *info;
	int sector;
	unsigned long flash_unused_address, flash_unused_size;
	unsigned long uboot_flash_address, uboot_flash_size;
	/* Set up a variety of environment variables for use by u-boot
	 * and applications (through environment named block).
	 */

	/* Set env_addr and env_size variables to the address and size
	 * of the u-boot environment.
	 */
#if defined(CONFIG_ENV_IS_IN_FLASH) || defined(CONFIG_ENV_IS_IN_RAM)
	setenv_hex("env_addr", CONFIG_ENV_ADDR);
#endif
	setenv_hex("env_size", CONFIG_ENV_SIZE);

	/* Describe physical flash */
	setenv_hex("flash_base_addr", CONFIG_SYS_FLASH_BASE);
	setenv_hex("flash_size", CONFIG_SYS_FLASH_SIZE);

	if (gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_FLASH) {
		/* Here we want to set some environment variables that
		 * describe how the flash is currently used, and protect
		 * the region that has u-boot code.
		 */
		if (gd->flags & GD_FLG_FAILSAFE_MODE) {
			uboot_flash_size = gd->arch.uboot_flash_size;
			uboot_flash_address = gd->arch.uboot_flash_address;
		} else {
			uboot_flash_address =
			octeon_find_and_validate_normal_bootloader(1);
			uboot_flash_size =
			get_image_size((bootloader_header_t *)
				       (uboot_flash_address));
			debug("%s: u-boot flash address: 0x%lx, size: 0x%lx\n",
			      __func__, uboot_flash_address, uboot_flash_size);
		}

		flash_unused_address = uboot_flash_address + uboot_flash_size;
		/* Limit to physical addresses only */
		flash_unused_address &= 0x1fffffff;
		/* Align this to the correct sector */
		info = &flash_info[0];
		for (sector = 0; sector < info->sector_count; ++sector) {
			debug("flash sector %5d: 0x%lx 0x%lx\n", sector,
			      flash_unused_address,
			      info->start[sector]);
			if (flash_unused_address <= info->start[sector])
				break;
		}
		if (sector == info->sector_count) {
			puts("ERROR: No unused memory available in flash\n");
			rc = -1;
		}

		flash_unused_address = info->start[sector];

		flash_unused_size = CONFIG_SYS_FLASH_SIZE -
			(flash_unused_address - CONFIG_SYS_FLASH_BASE);
		/* Set addr size for flash space that is available for
		 * application use
		 */
		setenv_hex("uboot_flash_addr", uboot_flash_address);
		setenv_hex("uboot_flash_size",
			   flash_unused_address - uboot_flash_address);

		setenv_hex("flash_unused_addr", flash_unused_address);
		setenv_hex("flash_unused_size", flash_unused_size);

		/* We now want to protect the flash from the base to
		 * the unused address
		 */
		flash_protect(FLAG_PROTECT_SET,
			      CONFIG_SYS_FLASH_BASE,
			      flash_unused_address - 1, info);

		/* Set obsolete bootloader_flash_update to do the new
		 * bootloaderupdate command.
		 */
		setenv("bootloader_flash_update", "bootloaderupdate");
	} else {
		debug("Clearing u-boot flash address and size environment variables\n");
		setenv("uboot_flash_addr", NULL);
		setenv("uboot_flash_size", NULL);
	}
	return rc;
}
#endif

#if !CONFIG_OCTEON_SIM_MEM_LAYOUT || defined(CONFIG_NET_MULTI) || \
    defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_MGMT_ENET) || \
    defined(CONFIG_OCTEON_SGMII_ENET) || defined(CONFIG_OCTEON_XAUI_ENET) || \
    defined(CONFIG_OCTEON_XLAUI_ENET)
/**
 * Configures the memory used for loading applications and Linux.
 *
 * @returns 0 for success, -1 on error.
 */
int octeon_configure_load_memory(void)
	__attribute__((weak, alias("__octeon_configure_load_memory")));

int __octeon_configure_load_memory(void)
{
	char *eptr;
	uint32_t addr;
	uint32_t size;
	eptr = getenv("octeon_reserved_mem_load_size");

	if (!eptr || !strcmp("auto", eptr)) {
		/* Pick a size that we think is appropriate.
		 * Please note that for small memory boards this guess
		 * will likely not be ideal.
		 * Please pick a specific size for boards/applications
		 * that require it.
		 */
		if (OCTEON_IS_OCTEON1PLUS()) {
			if (gd->ram_size <= MB(256))
				size = ((gd->ram_size * 2) / 5) & ~0xFFFFF;
			else if (gd->ram_size <= MB(512))
				size = min(MB(128),
					   (((gd->ram_size - MB(256)) * 2) / 5) & ~0xFFFFF);
			else
				size = min(MB(256),
					   (((gd->ram_size - MB(512)) * 2) / 3) & ~0xFFFFF);
		} else {
			if (gd->ram_size <= MB(256))
				size = min(MB(128),
					   ((gd->ram_size * 2) / 5) & ~0xFFFFF);
			else
				size = min(MB(256),
					   ((gd->ram_size - MB(256)) / 3) & ~0xFFFFF);
		}
	} else {
		size = simple_strtol(eptr, NULL, 16);
		debug("octeon_reserved_mem_load_size=0x%08x\n", size);
	}
	if (size) {
		debug("Linux reserved load size 0x%08x\n", size);
		eptr = getenv("octeon_reserved_mem_load_base");
		if (!eptr || !strcmp("auto", eptr)) {
			uint64_t mem_top;
			/* Leave some room for previous allocations that
			 * are made starting at the top of the low
			 * 256 Mbytes of DRAM
			 */
			int adjust = MB(1);
			if (gd->ram_size <= MB(512))
				adjust = MB(17);
			/* Put block at the top of DDR0, or bottom of
			 * DDR2
			 */
			if ((gd->ram_size <= MB(256)) ||
				(size > (gd->ram_size - MB(256))))
				mem_top = MIN(gd->ram_size - adjust,
					      MB(256) - adjust);
			else if ((gd->ram_size <= MB(512)) ||
				 (size > (gd->ram_size - MB(512))))
				mem_top = MIN(gd->ram_size - adjust,
					      MB(512) - adjust);
			else
				/* We have enough room, so set
				 * mem_top so that the block is
				 * at the base of the DDR2
				 * segment
				 */
				mem_top = MB(512) + size;
			/* Adjust for boot bus memory hole on OCTEON II
			 * and later.
			 */
			if (!OCTEON_IS_OCTEON1PLUS() &&
			    (gd->ram_size > MB(256)))
				mem_top += MB(256);
			debug("Adjusted memory top is 0x%llx\n", mem_top);
			addr = mem_top - size;
			if (addr > MB(512))
				addr = MB(512);
			if ((addr >= MB(256)) && addr < MB(512)) {
				/* The address landed in the boot-bus
				 * memory hole.  Dig it out of the hole.
				 */
				if (OCTEON_IS_OCTEON1PLUS())
					addr = MB(256) - size - MB(16);
				else
					addr = MB(512);
			}
		} else {
			addr = simple_strtol(eptr, NULL, 16);
		}

		if (0 > cvmx_bootmem_phy_named_block_alloc(size, addr,
							   addr + size, 0,
							   OCTEON_BOOTLOADER_LOAD_MEM_NAME,
							   0)) {
			printf("ERROR: Unable to allocate bootloader reserved memory (addr: 0x%x, size: 0x%x).\n",
			       addr, size);
		} else {
			/* Set default load address to base of memory
			 * reserved for loading. The setting of the
			 * env. variable also sets the load_addr global
			 * variable.
			 * This environment variable is overridden each
			 * boot if a reserved block is created.
			 */
			char str[20];
			snprintf(str, sizeof(str), "0x%x", addr);
			setenv("loadaddr", str);
			load_reserved_addr = addr;
			load_reserved_size = size;
			debug("Setting load address to 0x%08x, size 0x%x\n",
			      addr, size);
		}
		return 0;
	} else {
		puts("WARNING: No reserved memory for image loading.\n");
		return -1;
	}
}
#endif

#if !CONFIG_OCTEON_SIM_MEM_LAYOUT
int octeon_configure_linux_reserved_memory(void)
	__attribute__((weak, alias("__octeon_configure_linux_reserved_memory")));

int __octeon_configure_linux_reserved_memory(void)
{
	/* Reserve memory for Linux kernel.  The kernel requires specific
	 * physical addresses, so by reserving this here we can load simple
	 * exec applications and do other memory allocation without
	 * interfering with loading a kernel image.  This is freed and used
	 * when loading a Linux image.  If a Linux image is not loaded, then
	 * this is freed just before the applications start.
	 */
	char *eptr;
	uint32_t size;
	uint32_t addr;

	eptr = getenv("octeon_reserved_mem_linux_size");
	if (!eptr || !strcmp("auto", eptr)) {
		if (OCTEON_IS_OCTEON1PLUS() &&
		    (gd->ram_size >= MB(512)))
			size = MIN((gd->ram_size - MB(256)) / 2, MB(192));
		else if (gd->ram_size >= MB(1024))
			size = MIN(gd->ram_size / 2, MB(240));
		else
			size = MIN(gd->ram_size / 2, MB(192));
	} else {
		size = simple_strtol(eptr, NULL, 16);
	}
	if (size) {
		eptr = getenv("octeon_reserved_mem_linux_base");
		if (!eptr || !strcmp("auto", eptr))
			/* Start right after the bootloader */
			addr = OCTEON_LINUX_RESERVED_MEM;
		else
			addr = simple_strtol(eptr, NULL, 16);
		debug("Reserving 0x%x bytes of memory at 0x%x for the Linux kernel\n",
		      size, addr);
		/* Allocate the memory, and print message if we fail */
		if (0 > cvmx_bootmem_phy_named_block_alloc(size, addr,
							   addr + size, 0,
							   OCTEON_LINUX_RESERVED_MEM_NAME,
							   0)) {
			printf("ERROR: Unable to allocate linux reserved memory (addr: 0x%x, size: 0x%x).\n",
			       addr, size);
			return -1;
		}
	}
	return 0;
}

#endif
/**
 * Sets the board name environment variable
 */
void octeon_set_boardname(void)
	__attribute__((weak, alias("__octeon_set_boardname")));

void __octeon_set_boardname(void)
{
	char boardname[256];
	char *s = boardname;
	strncpy(boardname,
	       cvmx_board_type_to_string(cvmx_sysinfo_get()->board_type),
	       sizeof(boardname));

	while (*s) {
		*s = tolower(*s);
		s++;
	}

	setenv("boardname", boardname);
}

/**
 * Configures the prompt
 */
void octeon_configure_prompt(void)
	__attribute__((weak, alias("__octeon_configure_prompt")));
void __octeon_configure_prompt(void)
{
	char __attribute__((unused)) board_name[CONFIG_SYS_MAX_PROMPT_LEN];
	char prompt[CONFIG_SYS_MAX_PROMPT_LEN];
	int offset = 0;

	/* Generate the u-boot prompt based on various conditions */
	if (!getenv("prompt")) {
		debug("prompt environment variable not set.\n");
#if defined(CONFIG_SYS_PROMPT) && !defined(CONFIG_OCTEON_AUTO_PROMPT)
		debug("Setting prompt from defined %s\n", CONFIG_SYS_PROMPT);
		strncpy(prompt, CONFIG_SYS_PROMPT,
			CONFIG_SYS_MAX_PROMPT_LEN - 12);
#else
		strncpy(board_name,
			cvmx_board_type_to_string(cvmx_sysinfo_get()->board_type),
			CONFIG_SYS_MAX_PROMPT_LEN - 20);
		octeon_adjust_board_name(board_name, sizeof(board_name)-1);
		lowcase(board_name);
		snprintf(prompt, sizeof(prompt), "Octeon %s", board_name);
		if (!memcmp("cust_", board_name, 5))
			offset = 5;
		snprintf(prompt, sizeof(prompt), "Octeon %s", &board_name[offset]);
#endif
		if (gd->flags & GD_FLG_FAILSAFE_MODE)
			snprintf(uboot_prompt, sizeof(uboot_prompt),
				 "%s(Failsafe)", prompt);
		else if (gd->flags & GD_FLG_RAM_RESIDENT)
			snprintf(uboot_prompt, sizeof(uboot_prompt),
				 "%s(ram)", prompt);
		else
			snprintf(uboot_prompt, sizeof(uboot_prompt),
				 "%s", prompt);
		strncpy(prompt, uboot_prompt, sizeof(prompt));
#ifdef CONFIG_SYS_HUSH_PARSER
		snprintf(uboot_prompt, sizeof(uboot_prompt), "%s=>", prompt);
		snprintf(uboot_prompt_hush_ps2, sizeof(uboot_prompt_hush_ps2),
			 "%s>", prompt);
#else
		snprintf(uboot_prompt, sizeof(uboot_prompt), "%s# ", prompt);
#endif

		debug("Setting prompt to %s\n", uboot_prompt);
	} else {
		debug("prompt environment set, using that.\n");
	}
}

/**
 * Clear DRAM memory while preserving the environment stored in the L2 cache if
 * it is present.
 */
void octeon_clear_dram(void) __attribute__((weak, alias("__octeon_clear_dram")));

void __octeon_clear_dram(void)
{
	uint32_t size;
	bool save_env = false;
	const uint64_t env_paddr = U_BOOT_CACHE_ENV_ADDR;

	puts("Clearing DRAM.....");

	/* Clear first 256MB */
	debug("\n    Checking for environment, bootflags: 0x%lx\n",
	      gd->bd->bi_bootflags);
	if ((gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE) &&
	    !(gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_FLASH) &&
	    (OCTEON_IS_MODEL(OCTEON_CN73XX)  ||
	     OCTEON_IS_MODEL(OCTEON_CNF75XX) ||
	     OCTEON_IS_MODEL(OCTEON_CN76XX)  ||
	     OCTEON_IS_MODEL(OCTEON_CN78XX))) {
#if ((U_BOOT_CACHE_ENV_ADDR) >= 0x20000000)
# error U_BOOT_CACHE_ENV_ADDR must be less than 0x20000000!
#endif
		save_env = !octeon_check_env((env_t *)U_BOOT_CACHE_ENV_ADDR,
					     U_BOOT_CACHE_ENV_SIZE);;
		debug("%s: environment %sdetected\n", __func__,
		      save_env ? "" : "not ");
	}
	if (save_env) {
		if (OCTEON_DRAM_LATE_ZERO_OFFSET < U_BOOT_CACHE_ENV_ADDR) {
			size = (uint32_t)(env_paddr - OCTEON_DRAM_LATE_ZERO_OFFSET);
			debug("\n    Clearing from 0x%llx to 0x%llx\n",
			      (uint64_t)OCTEON_DRAM_LATE_ZERO_OFFSET,
			      (uint64_t)OCTEON_DRAM_LATE_ZERO_OFFSET + size);
			octeon_clear_mem_pfs(OCTEON_DRAM_LATE_ZERO_OFFSET, size);
		}

		size = MIN(gd->ram_size, MB(256));
		size -= (env_paddr + U_BOOT_CACHE_ENV_SIZE);
		debug("\n    Clearing from 0x%llx to 0x%llx\n",
		      env_paddr + U_BOOT_CACHE_ENV_SIZE,
		      env_paddr + U_BOOT_CACHE_ENV_SIZE + size);

		octeon_clear_mem_pfs(env_paddr + U_BOOT_CACHE_ENV_SIZE, size);
		cvmx_l2c_unlock_mem_region(MAKE_XKPHYS(env_paddr),
					   U_BOOT_CACHE_ENV_SIZE);

	} else {
		debug("Clearing from 0x%llx to 0x%llx\n",
		      (uint64_t)OCTEON_DRAM_LATE_ZERO_OFFSET,
		      (uint64_t)MIN(gd->ram_size,
				    (MB(256) - OCTEON_DRAM_LATE_ZERO_OFFSET)));
		octeon_clear_mem_pfs(OCTEON_DRAM_LATE_ZERO_OFFSET,
				     MIN(gd->ram_size,
					 MB(256) - OCTEON_DRAM_LATE_ZERO_OFFSET));
	}
	putc('.');
	if (gd->ram_size <= MB(256))
		goto done;

	if (!OCTEON_IS_OCTEON1PLUS()) {
		debug("\n    Clearing from 0x%llx to 0x%llx\n",
		      0x20000000ull,
		      0x20000000ull + (gd->ram_size - MB(256)));
		octeon_clear_mem_pfs(0x20000000ull,
				     gd->ram_size - MB(256));
	} else {
		debug("\n    Clearing from 0x%llx to 0x%llx\n",
		      0x410000000ull,
		      0x410000000ull +
		      MIN(gd->ram_size - MB(256), MB(256)));
		octeon_clear_mem_pfs(0x410000000ull,
				     (u64)MIN(gd->ram_size - MB(256), MB(256)));
		putc('.');
		if (gd->ram_size <= MB(512))
			goto done;

		debug("\n%s: Clearing 0x%llx bytes of memory starting at 0x20000000\n",
		      __func__, gd->ram_size - MB(512));
		octeon_clear_mem_pfs(0x20000000ull,
				     gd->ram_size - MB(512));
		putc('.');
	}
done:
	if (save_env) {
#if U_BOOT_CACHE_ENV_SIZE != U_BOOT_RAM_ENV_SIZE
# error U_BOOT_CACHE_ENV_SIZE and U_BOOT_RAM_ENV_SIZE must match!
#endif
		debug("Copying environment from 0x%x to 0x%x\n",
		      U_BOOT_CACHE_ENV_ADDR, U_BOOT_RAM_ENV_ADDR);
		memcpy((void *)U_BOOT_RAM_ENV_ADDR,
		       (void *)U_BOOT_CACHE_ENV_ADDR,
		       U_BOOT_RAM_ENV_SIZE);
		octeon_clear_mem_pfs(MAKE_XKPHYS(U_BOOT_CACHE_ENV_ADDR),
				     U_BOOT_CACHE_ENV_SIZE);
	}
	puts(" done\n");
	CVMX_SYNC;
	return;
}

extern int octeon_enable_all_cores_cn78xx(void);

/**
 * Enable all cores
 *
 * This function powers up all cores (if needed) and takes them out of
 * reset.  For OCX configurations it also sets it so all nodes share the
 * same memory mapping.
 */
static void octeon_enable_all_cores(void)
{
	int node = 0;

	debug("%s\n", __func__);
	if (OCTEON_IS_OCTEON3()) {
		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			debug("Powering up cores on node %d\n", node);
			/* Power up all the cores before taking them
			 * out of reset
			 */
			cvmx_write_csr_node(node, CVMX_RST_PP_POWER, 0);
			cvmx_read_csr_node(node, CVMX_RST_PP_POWER);
		}
#ifndef CONFIG_OCTEON_SIM_SPEED
		mdelay(1);
#endif
	}
#ifndef CONFIG_OCTEON_SIM_HW_DIFF
	/* Take all cores out of reset to reduce power usage. The secondary
	 * cores will spin on a wait loop conserving power until a NMI is
	 * received.
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
	    OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		octeon_enable_all_cores_cn78xx();
		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			/* Force all nodes to use the same memory
			 * mapping                                                             *
			 */
			union cvmx_l2c_oci_ctl oci_ctl;
			if (node == 0)
				continue;

			oci_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
			oci_ctl.s.gksegnode = 0;
			cvmx_write_csr_node(node, CVMX_L2C_OCI_CTL, oci_ctl.u64);
			cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);

			cvmx_write_csr_node(node, CVMX_CIU_PP_RST, 0);
		}
	} else
#endif
		cvmx_write_csr(CVMX_CIU_PP_RST, 0);
}

/**
 * This function is called after the init sequence called in board_init_f.
 *
 * @param bootflag	bootflag passed to board_init_f
 *
 * NOTE: functionality was moved here in order to free up stack space when
 * calling the init sequence.
 */
static void board_init_f_finish(ulong bootflag)
{
	u64 addr;
	u64 u_boot_mem_top;	/* top of memory, as used by u-boot */
	u64 heap_base_addr;
	u64 map_size;
	u64 u_boot_base_phys;	/* Physical address of u-boot code */
	u64 addr_sp_phys;	/* Stack pointer physical address */
	u64 addr_bd_phys;	/* Address of board data structure in RAM */
	u64 new_gd_addr;
	u32 stack_virt;
	u32 gd_virt;
	ulong len  = (ulong)&uboot_end - CONFIG_SYS_MONITOR_BASE;
	bd_t *bd = gd->bd;
#ifdef CONFIG_OCTEON_OCX
	int node;
#endif

	debug("%s(%lu)\n", __func__, bootflag);

	/* Round u-boot length up to a nice boundary */
	len = (len + 0xFFFF) & ~0xFFFF;

	/*
	 * Now that we have DRAM mapped and working, we can
	 * relocate the code and continue running from DRAM.
	 */

	/* Put u-boot at the highest address available in DRAM.
	 * For 3XXX/5XXX chips, this will likely be in the range
	 * 0x410000000-0x41fffffff
	 */
	if (gd->ram_size <= MB(256)) {
		addr = MIN(gd->ram_size, MB(256));
		/* We need to leave some room for the named block structure that
		 * will be allocated when the bootmem allocator
		 * is initialized.
		 */
		addr -= CVMX_BOOTMEM_NUM_NAMED_BLOCKS *
			sizeof(cvmx_bootmem_named_block_desc_t) + 128;
	} else {
		if (OCTEON_IS_OCTEON1PLUS()) {
			u64 offset = MIN(MB(256), gd->ram_size - MB(256));
			addr = 0x410000000ULL + offset;
		} else {
			/* CN63XX and newer */
			u64 offset = gd->ram_size - MB(256);
			addr = 0x20000000ULL + offset;
		}
	}

	u_boot_mem_top = addr;

	/* Print out any saved error messages that were logged during startup
	 * before the serial port was enabled.  This only works if the
	 * error message is a string constant in flash, but most things
	 * early on will be.
	 */
	if (gd->arch.err_msg)
		printf("Error message from before serial ready: %s\n",
		       gd->arch.err_msg);

	/* We can reserve some RAM "on top" here.
	 */

	debug("U-Boot link addr: 0x%x\n", CONFIG_SYS_MONITOR_BASE);

	/* round down to next 4 kB limit.
	 */
	addr &= ~(4096 - 1);
	debug("Top of RAM usable for U-Boot at: %08llx\n", addr);

	/* Reserve memory for U-Boot code, data & bss */
	addr = (uint64_t) ((uint64_t) addr - (uint64_t) MAX(len, 512 * 1024));

	/* In TLB mapped case, stack/heap go above (higher address) u-boot in
	 * DRAM as the base address of u-boot has alignment constraints due to
	 * the TLB usage.
	 * Reserve space for them here (before u-boot base address is
	 * determined).
	 */
	addr -= STACK_SIZE;
	addr -= TOTAL_MALLOC_LEN;

	/* Determine alignment required to use a single TLB entry, and
	 * round the DRAM address down to that alignment.   The minimum
	 * alignment is 4 MBytes, as this is required for PCI BAR
	 * mappings.
	 */
	map_size = 1;
	while (map_size < u_boot_mem_top - addr)
		map_size = map_size << 1;
	map_size = map_size >> 1;	/* TLB page size is 1/2 of mapping
					 * alignment
					 */
	debug("Using TLB mapping size of 0x%llx bytes for DRAM mapping.\n",
	      map_size);
	addr &= ~(map_size - 1);
	addr |= CONFIG_SYS_MONITOR_BASE & 0x0000FFFF;
	u_boot_base_phys = addr;

	debug("Reserving %ldk for U-Boot at: %08llx\n", (len + 1023) / 1024, addr);
#if !CONFIG_OCTEON_SIM_SPEED
	debug("Clearing 0x%llx bytes at addr 0x%llx\n",
	      u_boot_mem_top - u_boot_base_phys,
	      u_boot_base_phys);
	octeon_bzero64_pfs(MAKE_XKPHYS(u_boot_base_phys),
			   (u_boot_mem_top - u_boot_base_phys +
			    OCTEON_BZERO_PFS_STRIDE_MASK) &
			   ~OCTEON_BZERO_PFS_STRIDE_MASK);
#endif

	/* Put heap after code/data when using TLB */
	heap_base_addr = u_boot_base_phys + len;
	debug("u_boot_base_phys: 0x%llx, len: 0x%lx\n", u_boot_base_phys, len);
	/* For the TLB case we want the stack to be TLB mapped.  Place it
	 * after the u-boot code/data/heap.
	 */
	addr_sp_phys = heap_base_addr + TOTAL_MALLOC_LEN + STACK_SIZE;
	debug("Reserving %dk for malloc() at: %08llx\n",
	      TOTAL_MALLOC_LEN >> 10, heap_base_addr);

	debug("Stack top physical address: 0x%llx\n", addr_sp_phys);
	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */

	addr_sp_phys -= (sizeof(bd_t) + 0xf) & ~0xFull;
	addr_bd_phys = addr_sp_phys;
	gd->bd = (bd_t *) (uint32_t) (CONFIG_SYS_MONITOR_BASE +
					(addr_bd_phys - u_boot_base_phys));
	debug("Reserving %zu bytes for Board Info at: 0x%08llx\n", sizeof(bd_t),
	      addr_bd_phys);
	addr_sp_phys -= (sizeof(gd_t) + 0xf) & ~0xFull;
	new_gd_addr = addr_sp_phys;
	debug("Reserving %zu Bytes for Global Data at: 0x%08llx\n",
	      sizeof(gd_t), new_gd_addr);

	/* Reserve memory for boot params.
	 */
	addr_sp_phys -= CONFIG_SYS_BOOTPARAMS_LEN;
	bd->bi_boot_params = addr_sp_phys;
	debug("Reserving %dk for boot params() at: %08llx\n",
	      CONFIG_SYS_BOOTPARAMS_LEN >> 10, addr_sp_phys);

	/*
	 * Finally, we set up a new (bigger) stack.
	 *
	 * Leave some safety gap for SP, force alignment on 16 byte boundary
	 * Clear initial stack frame
	 */
	addr_sp_phys -= 16;
	addr_sp_phys &= ~0xF;
	memset64(MAKE_XKPHYS(addr_sp_phys), 0, 8);

	bd->bi_uboot_ram_addr = u_boot_base_phys;
	bd->bi_uboot_ram_used_size = u_boot_mem_top - bd->bi_uboot_ram_addr;

	debug("Stack Pointer at:   %08llx, stack size: 0x%08lx\n",
	      addr_sp_phys, STACK_SIZE);
	printf("Base DRAM address used by u-boot: 0x%08llx, size: 0x%lx\n",
	       bd->bi_uboot_ram_addr, bd->bi_uboot_ram_used_size);
        puts("DRAM: ");
#ifdef CONFIG_OCTEON_OCX
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		if (node != 0)
			puts(", ");
		print_size((uint64_t)gd->arch.ram_sizes[node] << 20, "");
	}
	puts("\n");
#else
        print_size(gd->ram_size, "\n");
#endif
	debug("Top of fixed address reserved memory: 0x%08x\n",
	      BOOTLOADER_END_RESERVED_SPACE);

#ifdef DEBUG
#define PRINT_RESERVED_ADDR(X)	printf("Reserved address %s: 0x%08x\n",\
					       #X, X)
	PRINT_RESERVED_ADDR(EXCEPTION_BASE_BASE);
	PRINT_RESERVED_ADDR(BOOTLOADER_DEBUG_REG_SAVE_BASE);
	PRINT_RESERVED_ADDR(BOOTLOADER_DEBUG_STACK_BASE);
	PRINT_RESERVED_ADDR(BOOTLOADER_PCI_READ_BUFFER_BASE);
	PRINT_RESERVED_ADDR(BOOTLOADER_BOOTMEM_DESC_ADDR);
	PRINT_RESERVED_ADDR(BOOTLOADER_END_RESERVED_SPACE);
#undef PRINT_RESERVED_ADDR
#endif

	if ((BOOTLOADER_END_RESERVED_SPACE & 0x7fffffff) >=
	    (bd->bi_uboot_ram_addr & 0x7fffffff)) {
		printf("\n\n*************************************\n");
		printf("ERROR: U-boot memory usage overlaps fixed address reserved area!\n");
		printf("U-boot base address: 0x%08qx, reserved top: 0x%08x\n",
		       bd->bi_uboot_ram_addr, BOOTLOADER_END_RESERVED_SPACE);
		printf("*************************************\n\n");
	}

	/*
	 * Save local variables to board info struct
	 */
	bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;	/* start of  DRAM memory */
	bd->bi_memsize = gd->ram_size;		/* size  of  DRAM memory in bytes */
	bd->bi_baudrate = gd->baudrate;		/* Console Baudrate */
	bd->bi_uboot_map_size = map_size;	/* Size of TLB mapping */

	debug("gd address: %p, new_addr: 0x%llx\n", gd, new_gd_addr);
	/* Use 64-bit copy here, as new stack could be outside of the
	 * 32-bit address space (physically addressed).
	 * gd pointer is kseg0 address in scratch, so casting it to an
	 * int32 is proper.
	 */
	debug("Relocating bd from: %p to 0x%08llx, size: 0x%x\n",
	      bd, MAKE_XKPHYS(addr_bd_phys), sizeof(bd));
	memcpy64(MAKE_XKPHYS(addr_bd_phys), (int32_t)bd, sizeof(*bd));

	debug("Relocating gd from: %p to 0x%08llx, size: 0x%x\n",
	      gd, MAKE_XKPHYS(new_gd_addr), sizeof(*gd));
	memcpy64(MAKE_XKPHYS(new_gd_addr), (int32_t)gd, sizeof(*gd));
#ifdef DEBUG
	debug("current address: 0x%llx\n", cvmx_ptr_to_phys(&_start));
#endif
	debug("relocating and jumping to code in DRAM at addr: 0x%llx\n", addr);

	stack_virt = CONFIG_SYS_MONITOR_BASE + addr_sp_phys - u_boot_base_phys;
	gd_virt = CONFIG_SYS_MONITOR_BASE + new_gd_addr - u_boot_base_phys;

	debug("Virtual stack top: 0x%x, virt new gd: 0x%x\n",
	      stack_virt, gd_virt);

	relocate_code_octeon(stack_virt, (void *)(u32)gd_virt,
			     MAKE_XKPHYS(addr), map_size);
}

#ifdef CONFIG_HW_WATCHDOG
extern void octeon_watchdog_disable_all(void);

/**
 * Disables all watchdog timers across all cores
 */
static int disable_wdog_early(void)
{
	octeon_watchdog_disable_all();
	return 0;
}
#endif

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

init_fnc_t *init_sequence[] = {
	octeon_early_errata_workaround_f,	/* 0 */
	octeon_setup_clocks,			/* 1 */
	board_fdt_init_f,			/* 2 */
	timer_init,				/* 3 */
#ifdef CONFIG_OCTEON_OCX
	oci_init,
#endif
#ifdef CONFIG_HW_WATCHDOG
	disable_wdog_early,
#endif
	octeon_bist_start,			/* 4 */
	early_uart_init_f,			/* 5 */
	octeon_early_console_init_f,	/* We can enable the console early */
#if !defined(CONFIG_OCTEON_SIM_HW_DIFF)
	init_twsi_bus,				/* 7 */
#endif
	board_early_init_f,			/* 8 */
#if defined(CONFIG_OCTEON_DISABLE_FAILSAFE_SCAN) || \
    (!CONFIG_OCTEON_SIM_NO_FLASH		&& \
     !defined(CONFIG_OCTEON_NAND_STAGE2)	&& \
     !defined(CONFIG_OCTEON_EMMC_STAGE2)	&& \
     !defined(CONFIG_OCTEON_SPI_STAGE2)		&& \
     !defined(CONFIG_OCTEON_AUTHENTIK_STAGE2))

	failsafe_scan,				/* 9 */
#endif
#if !defined(CONFIG_OCTEON_SIM_NO_FLASH) && !defined(CONFIG_OCTEON_NO_BOOT_BUS) \
	&& !defined(CONFIG_OCTEON_SIM_HW_DIFF)
	octeon_boot_bus_init,			/* 10 */
#endif
						/* 11 is empty. */
	early_board_init,			/* 12 */
	octeon_env_init,	/* 13 initialize environment */
	octeon_import_ram_env,	/* 14 import env if booting from RAM */
	init_baudrate,		/* 15 initialze baudrate settings */
	serial_init,		/* 16 serial communications setup */
	console_init_f,		/* 17 Initialize the console */
	display_banner,		/* 18 say that we are here */
#if !CONFIG_OCTEON_SIM_HW_DIFF
	octeon_bist,		/* 19 */
#endif
	octeon_init_dram,	/* 20 */
	octeon_dram_test,
#if OCTEON_CONFIG_DFM
	octeon_init_dfm,
#endif
	octeon_late_errata_workaround_f,
	checkboard,
	display_board_info,
	NULL,
};

/**
 * This is the first function called after running the assembly code in start.S.
 *
 * @param bootflag	Flags passed by the assembly code (stored in $S4).
 */
void board_init_f(ulong bootflag)
{
	gd_t gd_data;
	bd_t bd;		/* Local copy */
	init_fnc_t **init_fnc_ptr;
	static const ulong image_size = (ulong)&uboot_end_data - CONFIG_SYS_MONITOR_BASE;
	int init_func_count = 0;

	/* Offsets from u-boot base for stack/heap.  These offsets will be
	 * the same for both physical and virtual maps.
	 */
	SET_K0(&gd_data);

	/* Pointer is writeable since we allocated a register for it */
	memset((void *)gd, 0, sizeof(gd_t));
	memset((void *)&bd, 0, sizeof(bd));

	bd.bi_bootflags = bootflag;

	gd->bd = &bd;
	/* We have at least one node */
	gd->arch.node_mask = 1;

	if (bootflag & OCTEON_BD_FLAG_BOOT_RAM)
		gd->flags |= GD_FLG_RAM_RESIDENT;

	/* Lock ourselves into L2 cache */
	if (bootflag & OCTEON_BD_FLAG_BOOT_CACHE)
		cvmx_l2c_lock_mem_region(cvmx_ptr_to_phys(&uboot_start),
					 image_size);

#if !CONFIG_OCTEON_SIM_HW_DIFF
	/* Disable the watchdog timer for core 0 since it may still be running
	 * after a soft reset as a PCI target.
	 */
	cvmx_write_csr(CVMX_CIU_WDOGX(0), 0);
#endif
#ifdef ENABLE_BOARD_DEBUG
	int gpio_status_data = 1;
#endif
	debug("%s: gd: %p\n", __func__, gd);

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
#ifdef ENABLE_BOARD_DEBUG
		gpio_status(gpio_status_data++);
#endif
		debug("Calling %d: 0x%p\n", init_func_count, *init_fnc_ptr);
		debug("%s: Boot flag: 0x%lx\n", __func__, bootflag);
		if ((*init_fnc_ptr) () != 0) {
			printf("hanging, init function: %d\n", init_func_count);
			if (OCTEON_IS_MODEL(OCTEON_CN63XX)) {
				/* 63XX_HACK sometimes dimms are not detected */
				printf("AUTO resetting board due to init failing!\n");
				do_reset(NULL, 0, 0, NULL);
			}
			hang();
		}
		debug("%s: Done calling %d (0x%p)\n", __func__,
		      init_func_count, init_fnc_ptr);
		init_func_count++;
	}

	debug("%s: Done calling init sequence\n", __func__);
	debug("%s: gd: %p\n", __func__, gd);
	/* Flush ourselves from L2 into DRAM */
	if (bootflag & OCTEON_BD_FLAG_BOOT_CACHE) {
		CVMX_SYNCW;
		debug(" flushing L2 cache...\n");
		cvmx_l2c_flush();

		debug("  Invalidating icache\n");
		CVMX_ICACHE_INVALIDATE;
		debug("  Done.\n  Unlocking U-Boot from cache...\n\n");
#ifdef DEBUG
		debug("  start: %p\n", &uboot_start);
		debug("  start: 0x%llx, size: 0x%lx\n",
		      cvmx_ptr_to_phys(&uboot_start), image_size);
#endif
		debug(" Done.\n");
	}

	board_init_f_finish(bootflag);
	/* NOTREACHED - relocate_code() does not return */
}

/************************************************************************
 *
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 *
 ************************************************************************
 */
void board_init_r(gd_t * id, u64 dest_addr)
{
	char *dtb_begin, *dtb_end;
	int64_t idle_loop;
	char *s;
	bd_t *bd;
	int rc;
	int64_t __attribute__ ((unused)) bm_val;
	char __attribute__((unused)) *eptr;
	uint32_t __attribute__((unused)) addr;
	uint32_t size;
	char tmp[10];
	uint64_t *u64_ptr;
#if !CONFIG_OCTEON_SIM_HW_DIFF
	int i;
	int num_cores;
#endif /* !CONFIG_OCTEON_SIM_HW_DIFF */

	/* Convince GCC to really do what we want, otherwise the old value is
	 * still used.
	 */
	SET_K0(id);

	gd = id;
	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	debug("Now running in RAM - U-Boot at: %08llx\n", dest_addr);

	gd->reloc_off =
	    (dest_addr & 0xFFFFFFFF) - (u64) CONFIG_SYS_MONITOR_BASE;

	monitor_flash_len = (ulong)&uboot_end_data - dest_addr;

	debug("Relocation offset: 0x%lx, monitor base: 0x%08llx\n",
	      gd->reloc_off, (u64) CONFIG_SYS_MONITOR_BASE);

#if !defined(CONFIG_SYS_NO_FLASH)
	/* Here we need to update the boot bus mappings to put the flash
	 * boot bus mappings back at the expected locations.  We
	 * no longer need the adjusted mappings for the TLB usage.
	 */
	{
		cvmx_mio_boot_reg_cfgx_t __attribute__ ((unused)) reg_cfg;
		reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFG0);
		if (reg_cfg.s.en != 0) {
			reg_cfg.s.base = ((CONFIG_SYS_FLASH_BASE >> 16) & 0x1fff);
			cvmx_write_csr(CVMX_MIO_BOOT_REG_CFG0, reg_cfg.u64);
		}
	}
#endif

	bd = gd->bd;
	debug("U-Boot memory end: %p\n", &uboot_end);
	debug("Setting heap memory to start at 0x%x, size: 0x%x\n",
	      ((uint32_t)(&uboot_end) + 0xFFFF) & ~0xFFFF, TOTAL_MALLOC_LEN);

	mem_malloc_init(((u32)(&uboot_end) + 0xFFFF) & ~0xFFFF,
			TOTAL_MALLOC_LEN);
	debug("Adding map entry for 0x%x, address: 0x%llx, size: 0x%x, index: %d\n",
	      CONFIG_SYS_TEXT_BASE, bd->bi_uboot_ram_addr,
	      bd->bi_uboot_map_size, CONFIG_ADDR_MAP_CODE_IDX);

	/* Fix the gd environment address */
#if !defined(CONFIG_ENV_IS_IN_RAM) && !defined(CONFIG_SYS_NO_FLASH) &&	\
	defined(CONFIG_OCTEON_FLASH) && 				\
	!defined(CONFIG_ENV_IS_IN_NAND)
	octeon_fix_env_reloc();
#endif
	/* Map u-boot's virtual to physical mapping */
	addrmap_set_entry(CONFIG_SYS_TEXT_BASE, bd->bi_uboot_ram_addr,
			  bd->bi_uboot_map_size * 2, CONFIG_ADDR_MAP_CODE_IDX);

	/* Map the first 256MB of memory 1:1 */
	size = MIN(gd->ram_size, MB(256));
	addrmap_set_entry(0, 0, size, CONFIG_ADDR_MAP_FIRST_256MB_IDX);

	/* Map the boot bus 1:1 */
	addrmap_set_entry(0x10000000, 0x10000000, 0x10000000,
			  CONFIG_ADDR_MAP_BOOT_BUS_IDX);
	if (gd->ram_size > MB(256)) {
		/* The CN3XXX and CN5XXX place the second 256MB of memory
		 * at address 0x410000000 to 0x41FFFFFFF but this isn't really
		 * accessible to U-Boot so we don't worry about it.
		 */
		if (OCTEON_IS_OCTEON1PLUS()) {
			if (gd->ram_size > MB(512)) {
				size = min(0x60000000, gd->ram_size - MB(512));
				addrmap_set_entry(0x20000000, 0x20000000,
						  size,
						  CONFIG_ADDR_MAP_REST_MEM_IDX);
			}
		} else {
			/* CN63XX and newer */
			/* The CN6XXX and newer are different.  All memory above
			 * 256MB has it's address shifted by 256MB by the boot
			 * bus.
			 * U-Boot only cares about the other 1.5GB in KUSEG.
			 */
			size = gd->ram_size - MB(256);
			size = min(size, 0x60000000);
			addrmap_set_entry(0x20000000, 0x20000000, size,
					  CONFIG_ADDR_MAP_REST_MEM_IDX);
		}
	}

#if CONFIG_OCTEON_PCI_HOST
	/* Add BAR address mapping.  Note that this must match the code in
	 * octeon_pci.c
	 */
	addrmap_set_entry(OCTEON_PCI_SLOT0_BAR_ADDR,
			  XKPHYS_TO_PHYS(pci_mem1_base + OCTEON_PCI_SLOT0_BAR_ADDR),
			  OCTEON_PCI_SLOT0_BAR_SIZE,
			  CONFIG_ADDR_MAP_PCI0_BASE);
	if (!OCTEON_IS_OCTEON1PLUS())
		addrmap_set_entry(OCTEON_PCI_SLOT1_BAR_ADDR,
				  XKPHYS_TO_PHYS(pci_mem2_base + OCTEON_PCI_SLOT1_BAR_ADDR),
				  OCTEON_PCI_SLOT1_BAR_SIZE, CONFIG_ADDR_MAP_PCI1_BASE);
	if (OCTEON_IS_OCTEON3() && !OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		addrmap_set_entry(OCTEON_PCI_SLOT2_BAR_ADDR,
				  XKPHYS_TO_PHYS(pci_mem3_base + OCTEON_PCI_SLOT2_BAR_ADDR),
				  OCTEON_PCI_SLOT2_BAR_SIZE, CONFIG_ADDR_MAP_PCI2_BASE);
		if (OCTEON_IS_MODEL(OCTEON_CN78XX) || OCTEON_IS_MODEL(OCTEON_CN73XX)) {
			addrmap_set_entry(OCTEON_PCI_SLOT3_BAR_ADDR,
				  XKPHYS_TO_PHYS(pci_mem4_base + OCTEON_PCI_SLOT3_BAR_ADDR),
				  OCTEON_PCI_SLOT3_BAR_SIZE, CONFIG_ADDR_MAP_PCI3_BASE);
		}
	}
#endif /* CONFIG_OCTEON_PCI_HOST */
#if !CONFIG_OCTEON_SIM_HW_DIFF
	/* Disable the watchdog timers, as these are still running after a soft
	 * reset as a PCI target
	 */
	num_cores = cvmx_octeon_num_cores();
	for (i = 0; i < num_cores; i++)
		cvmx_write_csr(CVMX_CIU_WDOGX(i), 0);

	debug("About to flush L2 cache\n");
	/* Flush and unlock L2 cache (soft reset does not unlock locked lines)
	 */
	cvmx_l2c_flush();

# ifdef CONFIG_OCTEON_EMMC_STAGE2
	/* Unlock memory regions locked by the stage 1 bootloader */
	cvmx_l2c_unlock_mem_region(CONFIG_OCTEON_EMMC_STAGE1_ADDR,
				   CONFIG_OCTEON_EMMC_STAGE1_SIZE);
	cvmx_l2c_unlock_mem_region(CONFIG_OCTEON_EMMC_STAGE2_CACHE_ADDR,
				   CONFIG_OCTEON_EMMC_STAGE2_CACHE_SIZE);
	cvmx_l2c_unlock_mem_region(U_BOOT_RAM_ENV_ADDR, 2 * U_BOOT_RAM_ENV_SIZE);
# endif
# ifdef CONFIG_OCTEON_AUTHENTIK_STAGE2
	/* Unlock memory regions locked by the stage 1 bootloader */
	cvmx_l2c_unlock_mem_region(CONFIG_OCTEON_AUTHENTIK_STAGE1_ADDR,
				   CONFIG_OCTEON_AUTHENTIK_STAGE1_SIZE);
	cvmx_l2c_unlock_mem_region(CONFIG_OCTEON_AUTHENTIK_STAGE2_CACHE_ADDR,
				   CONFIG_OCTEON_AUTHENTIK_STAGE2_CACHE_SIZE);
# endif
#endif /* !CONFIG_OCTEON_SIM_HW_DIFF */

#if !CONFIG_OCTEON_SIM_SPEED && !defined(CONFIG_NO_CLEAR_DDR)
	if (!(gd->flags & GD_FLG_MEMORY_PRESERVED)) {
		/* Zero all of memory above OCTEON_DRAM_LATE_ZERO_OFFSET */
		debug("ram size: 0x%llx (%llu), sizeof=%u\n",
		      gd->ram_size, gd->ram_size, sizeof(gd->ram_size));
		octeon_clear_dram();
	} else {
		puts("DRAM contents preserved, not clearing.\n");
        }
#endif
#ifndef CONFIG_OCTEON_SIM_HW_DIFF
	debug("Clearing DRAM ECC errors\n");
	octeon_dram_clear_ecc_errors();
#endif
	debug("Initializing phy mem list: ram_size: 0x%llx, low reserved size: 0x%x\n",
	      gd->ram_size, OCTEON_RESERVED_LOW_MEM_SIZE);
#ifdef CONFIG_OCTEON_OCX
	if (gd->arch.node_mask == 1) {
#endif
		/* call old single-node func: it uses only gd->ram_size */
		if (!cvmx_bootmem_phy_mem_list_init(gd->ram_size,
						    OCTEON_RESERVED_LOW_MEM_SIZE,
						    (void *)MAKE_KSEG0(BOOTLOADER_BOOTMEM_DESC_SPACE))) {
			printf("FATAL: Error initializing free memory list\n");
			hang();
		}
#ifdef CONFIG_OCTEON_OCX
	} else {
		/* call multi-node variant: assumes gd->arch.ram_sizes[0..3] are set */
		if (!cvmx_bootmem_phy_mem_list_init_multi(gd->arch.node_mask,
							  (uint32_t *)(gd->arch.ram_sizes),
							  OCTEON_RESERVED_LOW_MEM_SIZE,
							  (void *)MAKE_KSEG0(BOOTLOADER_BOOTMEM_DESC_SPACE))) {
			printf("FATAL: Error initializing free memory list\n");
			hang();
		}
	}
#endif

	debug("Bootloader boot memory descriptor at %p\n",
	      (void *)MAKE_KSEG0(BOOTLOADER_BOOTMEM_DESC_SPACE));
	/* Here we allocate the memory that u-boot is _already_ using.  Note
	 * that if the location of u-boot changes, this may need to be done
	 * differently.  In particular, this _won't_ work if u-boot is
	 * placed at the beginning of a DRAM region, as that is where the free
	 * block size/length is stored, and u-boot will be corrupted when
	 * the bootmem list is created.
	 */
	debug("Allocating named block for used memory.\n");
	if (0 >
	    (bm_val =
	     cvmx_bootmem_phy_named_block_alloc(gd->bd->bi_uboot_ram_used_size,
						gd->bd->bi_uboot_ram_addr,
						gd->bd->bi_uboot_ram_addr +
						gd->bd->bi_uboot_ram_used_size,
						0, "__uboot_code_data", 0))) {
		printf("ERROR: Unable to allocate DRAM for u-boot from bootmem allocator.\n");
		debug("ub used_size: 0x%lx, addr: 0x%llx, bootmem_retval: 0x%llx\n",
		      gd->bd->bi_uboot_ram_used_size,
		      gd->bd->bi_uboot_ram_addr, bm_val);
	}

	board_fdt_get_limits(&dtb_begin, &dtb_end);
	debug("Copying FDT from 0x%p to 0x%x, %u bytes\n",
	      dtb_begin, OCTEON_FDT_BASE, dtb_end - dtb_begin);
	bm_val = cvmx_bootmem_phy_named_block_alloc(OCTEON_FDT_MAX_SIZE,
						    OCTEON_FDT_BASE,
						    0,
						    0,
						    OCTEON_FDT_BLOCK_NAME,
						    0);
	if (bm_val < 0) {
		printf("Error: Could not reserve %u bytes for FDT at address 0x%x\n",
		       OCTEON_FDT_MAX_SIZE, OCTEON_FDT_BASE);
	}
#ifdef CONFIG_CMD_SPI
	debug("Initializing SPI interface\n");
	spi_init();		/* go init the SPI */
#endif
#if defined(CONFIG_ENV_IS_IN_SPI_FLASH) || defined(CONFIG_ENV_IS_IN_FLASH) || \
    defined(CONFIG_ENV_IS_IN_EEPROM)    || defined(CONFIG_ENV_IS_IN_RAM)
	/* Load environment from SPI NOR, NOR flash, EEPROM or RAM */
	env_relocate();
#endif

	/* Copy the initial FDT to its final location. */
	if (dtb_end - dtb_begin > OCTEON_FDT_MAX_SIZE) {
		printf("WARNING: Embedded DTB image too large: %d\n",
		       dtb_end - dtb_begin);
	} else {
		memcpy((char *)OCTEON_FDT_BASE, dtb_begin,
		       dtb_end - dtb_begin);

		gd->fdt_blob = (void *)OCTEON_FDT_BASE;

		working_fdt = (struct fdt_header *)gd->fdt_blob;

		/* Reserve space to grow */
		debug("Increase FDT size to 0x%x\n", OCTEON_FDT_MAX_SIZE);
		rc = fdt_open_into(gd->fdt_blob, (void *)gd->fdt_blob,
				   OCTEON_FDT_MAX_SIZE);
		if (rc)
			printf("Error: Cannot increase size of FDT: %s\n",
			       fdt_strerror(rc));
	}
	/** Install our own exception handler */
	bm_val = cvmx_bootmem_phy_named_block_alloc(OCTEON_UBOOT_EXCEPT_HANDLER_SIZE,
						    OCTEON_UBOOT_EXCEPT_HANDLER_BASE,
						    0x100000,
						    OCTEON_UBOOT_EXCEPT_HANDLER_ALIGNMENT,
						    OCTEON_UBOOT_EXCEPT_HANDLER_NAME,
						    0);
	if (bm_val > 0) {
		if (octeon_build_exception_table(CASTPTR(void, MAKE_KSEG0(bm_val))))
			printf("Error: could not allocate exception handler\n");
	}

	debug("BOOTLOADER_BOOTMEM_DESC_ADDR: 0x%x\n",
	      BOOTLOADER_BOOTMEM_DESC_ADDR);
	debug("BOOTLOADER_BOOTMEM_DESC_SPACE: 0x%x\n",
	      BOOTLOADER_BOOTMEM_DESC_SPACE);
	debug("size of bootmem desc: %d, location: %p\n",
	      sizeof(cvmx_bootmem_desc_t),
	      (void *)MAKE_KSEG0(BOOTLOADER_BOOTMEM_DESC_SPACE));

	debug("Setting up simple executive support\n");
	/* setup cvmx_sysinfo structure so we can use simple executive
	 * functions.
	 */
	octeon_setup_cvmx_sysinfo();

	if (early_board_init_r())
		puts("Error returned by early_board_init_r()\n");

#if defined(ENV_IS_IN_FLASH)
	debug("Relocating the environment function pointers.\n");
	if (gd->flags & GD_FLG_RAM_RESIDENT) {
		/* Restore to FLASH location */
		env_t *ep = (env_t *)CONFIG_ENV_ADDR;
		gd->env_addr = CONFIG_ENV_ADDR;
		debug("Setting gd environment address to 0x%lx\n", gd->env_addr);
		if (ep->crc == crc32(0, ep->data, CONFIG_ENV_SIZE - 4)) {
			debug("Found good CRC for environment.\n");
			gd->env_valid = 1;
		} else
			debug("CRC mismatch for environment at 0x%p\n", ep);
	}
#endif
#if (defined(CONFIG_CMD_OCTEON_NAND) || defined(CONFIG_CMD_NAND)) && \
     !CONFIG_OCTEON_SIM_NO_FLASH
	/* NAND must be done after bootmem allocator is set up, and before
	 * NOR  probing to handle some corner cases.
	 */
	if (!getenv("disable_nand")) {
		int chips;
		debug("Probing NAND memory\n");
		WATCHDOG_RESET();
		chips = oct_nand_probe();
		WATCHDOG_RESET();
		debug("Done probing NAND, rc: %d\n", chips);
		puts("NAND:  ");
		nand_init();	/* go init the NAND (do this after flash init...) */
		WATCHDOG_RESET();
		debug("Relocating environment\n");
		/* Relocate environment function pointers */
		env_relocate();
		debug("Environment relocated\n");
	}
#endif

#ifndef CONFIG_OCTEON_SIM
	if ((gd->flags & GD_FLG_RAM_RESIDENT) ||
	    (gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE)) {
		/* If we're loaded by an external boot loader then we may be
		 * passed environment variables.  In this case we first load
		 * the environment, if any, from flash and the default
		 * environment, then overwrite any entries passed from the
		 * external bootloader.
		 *
		 * The difference between this and a standard import is that
		 * import clears out the environment variables from the flash
		 * first, which we do not want.
		 */
		env_t *ep = (env_t *)U_BOOT_RAM_ENV_ADDR;
		uint32_t crc;
		debug("Importing environment from remote boot loader.\n");

		memcpy(&crc, &ep->crc, sizeof(crc));

		if ((crc == crc32(0, ep->data,
				 U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE))
		    && (!himport_r(&env_htab, (char *)ep->data,
				   U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE,
				   '\0', H_NOCLEAR, 0, NULL))) {
			printf("Error: could not import environment from boot loader\n");
		}
# if !CONFIG_NO_CLEAR_DDR
		else {
			/* We can safely clear the passed-in environment
			 * variables now.
			 */
			memset((char *)U_BOOT_RAM_ENV_ADDR, 0,
			       U_BOOT_RAM_ENV_SIZE);
			memset((char *)U_BOOT_RAM_ENV_ADDR_2, 0,
			       U_BOOT_RAM_ENV_SIZE);
		}
# endif
	} else if ((gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE) &&
		   ((OCTEON_IS_OCTEON3() && !OCTEON_IS_MODEL(OCTEON_CN70XX)) ||
		    OCTEON_IS_MODEL(OCTEON_CN66XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN68XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN63XX))) {

		env_t *ep = (env_t *)U_BOOT_CACHE_ENV_ADDR;
		int rc;

		debug("Importing environment from cache\n");

		if (ep->crc == crc32(0, ep->data,
				     U_BOOT_CACHE_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE)) {
			debug("Found environment in cache, loading...\n");
			rc = himport_r(&env_htab, (char *)ep->data,
				       U_BOOT_CACHE_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE,
				       '\0', H_NOCLEAR, 0, NULL);
			if (rc)
				puts("Error: could not import environment from cache\n");
		} else {
			debug("Environment not found in L2 cache\n");
		}

	}
#endif
	/* For the Octeon simulator we need to be able to import the environment
	 */
#if defined(CONFIG_OCTEON_SIM) && !defined(CONFIG_OCTEON_NO_BOOT_BUS)
# define OCTEON_ENV_FLASH_ADDR (0x9FE00000UL)
# define OCTEON_ENV_FLASH_SIZE (0x10000UL)
	debug("Looking for environment at %p\n",
	      (char *)(OCTEON_ENV_FLASH_ADDR));
	if (*(char *)(OCTEON_ENV_FLASH_ADDR) != '\0') {
		printf("Copying user supplied environment from file flash\n");
		if (!env_import((char *)(OCTEON_ENV_FLASH_ADDR - ENV_HEADER_SIZE), 0)) {
			printf("Error %d importing user supplied environment\n", errno);
		}
	} else {
		debug("No user supplied environment found\n");
	}
#endif
	debug("Allocating and configuring boot vectors\n");
	octeon_boot_bus_moveable_init();

	debug("Enabling all cores\n");
	octeon_enable_all_cores();

	debug("Allocating idle core named block\n");
	/* Create a named block for idle cores to loop */
	idle_loop = cvmx_bootmem_phy_named_block_alloc(IDLE_CORE_BLOCK_SIZE,
						       OCTEON_IDLE_CORE_MEM,
						       0, 0,
						       IDLE_CORE_BLOCK_NAME, 0);
	if (0 > idle_loop)
		printf("ERROR: Unable to allocate DRAM for idle loop from bootmem allocator.\n");

#if !CONFIG_OCTEON_SIM_MEM_LAYOUT || defined(CONFIG_NET_MULTI) || \
    defined(CONFIG_OCTEON_RGMII_ENET) || defined(CONFIG_OCTEON_MGMT_ENET) || \
    defined(CONFIG_OCTEON_SGMII_ENET) || defined(CONFIG_OCTEON_XAUI_ENET) || \
    defined(CONFIG_OCTEON_XLAUI_ENET)
	octeon_configure_load_memory();
#endif

#if !CONFIG_OCTEON_SIM_MEM_LAYOUT
	octeon_configure_linux_reserved_memory();
#endif
	/* Initialize the named block with the following code so that cores will
	 * spin on a    wait loop conserving power until a NMI is received.
	 *
	 * 42000020  1: wait
	 * 1000fffe     b 1b
	 * 00000000     nop
	 */
	memset(cvmx_phys_to_ptr(idle_loop), 0, IDLE_CORE_BLOCK_SIZE);
	*(int64_t *) cvmx_phys_to_ptr(idle_loop) = 0x420000201000fffeull;
	CVMX_SYNCW;
	/* Put bootmem descriptor address in known location for host */
	/* Make sure it is not in kseg0, as we want physical addresss */
	u64_ptr = (void *)(0x80000000 | BOOTLOADER_BOOTMEM_DESC_ADDR);
	*u64_ptr = ((u32) __cvmx_bootmem_internal_get_desc_ptr()) & 0x7fffffffull;
	debug("bootmem descriptor address (looked up): 0x%llx\n",
	      ((u32) __cvmx_bootmem_internal_get_desc_ptr()) & 0x7fffffffull);

	dnull_ptr_check("Bootmem descriptors initialized");
	/* Set up prompt */
	debug("Setting up U-Boot prompt for board type %u.\n",
	      cvmx_sysinfo_get()->board_type);

	octeon_configure_prompt();

	dnull_ptr_check("Prompt initialized");

#if defined(CONFIG_HW_WATCHDOG)
	if (getenv("watchdog_enable")) {
		int msecs = 0;

		msecs = getenv_ulong("watchdog_timeout", 10,
				     CONFIG_OCTEON_WD_TIMEOUT * 1000);
		debug("Starting hardware watchdog with NMI timeout %d ms\n",
		      msecs);
		if (msecs > 0)
			hw_watchdog_start(msecs);
	}
#endif

	dnull_ptr_check("Watchdog initialized");
	/* Initialize the rest of the devices on the boot bus like
	 * Compact Flash, PSRAM, etc.
	 */
#if !defined(CONFIG_OCTEON_NO_BOOT_BUS) && !defined(CONFIG_OCTEON_SIM_HW_DIFF)
	debug("Performing late boot bus initialization.\n");
	octeon_boot_bus_late_init();
#endif
#if CONFIG_OCTEON_SIM_NO_FLASH
	{
		/* Configure boot bus region 1 to allow entire 256 MByte region
		 * to be accessed during simulation.
		 */
		uint64_t flash_size = MB(256);
		cvmx_mio_boot_reg_cfgx_t reg_cfg;
		printf("Configuring boot bus for full 256MiB access\n");
		reg_cfg.u64 = 0;
		cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(0), reg_cfg.u64);
		reg_cfg.s.en = 1;
		/* In 64k blocks, +4MByte alias of low 4MBytes of flash */
		reg_cfg.s.size = (flash_size >> 16) - 1;
		/* Mask to put physical address in boot bus window */
		reg_cfg.s.base = ((0x10000000 >> 16) & 0x1fff);
		cvmx_write_csr(CVMX_MIO_BOOT_REG_CFGX(1), reg_cfg.u64);
	}
#endif
#if !defined(CONFIG_SYS_NO_FLASH) && !CONFIG_OCTEON_SIM_NO_FLASH
	/* Do flash init _after_ NAND probing, as we may have done some chip
	 * select enable fixups as part of the NAND probing.
	 */
	debug("Checking for NOR flash...\n");
	WATCHDOG_RESET();

	octeon_configure_nor_flash();
	WATCHDOG_RESET();
	dnull_ptr_check("NOR flash configured");
#endif /* CONFIG_SYS_NO_FLASH */

#ifdef DEBUG
	cvmx_bootmem_phy_list_print();
#endif

#if (CONFIG_OCTEON_EEPROM_TUPLES)
	{
		int addr;
		uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
		octeon_eeprom_chip_capability_t *cc_ptr = (void *)ee_buf;

		WATCHDOG_RESET();
		debug("Reading eeprom...\n");
		/* Read coremask from eeprom, and set environment variable if
		 * present.  Print warning if not present in EEPROM.
		 * Currently ignores voltage/freq fields, and just uses first
		 * capability tuple.
		 * This is only valid for certain parts.
		 */
		addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
						 EEPROM_CHIP_CAPABILITY_TYPE,
						 0, ee_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
		if (addr >= 0 &&
		    ((OCTEON_IS_MODEL(OCTEON_CN38XX)
		      && ((cvmx_read_csr(CVMX_CIU_FUSE) & 0xffff) == 0xffff)
		      && !cvmx_octeon_fuse_locked())
		     || OCTEON_IS_MODEL(OCTEON_CN56XX)
		     || OCTEON_IS_MODEL(OCTEON_CN52XX)
		     || OCTEON_IS_OCTEON2()
		     || OCTEON_IS_OCTEON3())) {
			snprintf(tmp, sizeof(tmp), "0x%x", cc_ptr->coremask);
			setenv("coremask_override", tmp);
		} else {
			/* No chip capability tuple found, so we need to check
			 * if we expect one.
			 * CN31XX chips will all have fuses blown
			 * appropriately.
			 * CN38XX chips may have fuses blown, but may not.  We
			 * will check to see if
			 * we need a chip capability tuple and only warn if we
			 * need it but don't have it.
			 */
			if (OCTEON_IS_MODEL(OCTEON_CN38XX)) {
				/* Here we only need it if no core fuses are
				 * blown and the lockdown fuse is not blown.
				 * In all other cases the cores fuses are
				 * definitive and we don't need a coremask
				 * override.
				 */
				if (((cvmx_read_csr(CVMX_CIU_FUSE) & 0xffff) ==
				     0xffff)
				    && !cvmx_octeon_fuse_locked())
					printf("Warning: No chip capability tuple found in eeprom, coremask_override may be set incorrectly\n");
				else
					/* Clear coremask_override as we have a
					 * properly fused part, and don't need
					 * it.
					 */
					setenv("coremask_override", NULL);
			} else {
				/* 31xx and 30xx will always have core fuses
				 * blown appropriately
				 */
				setenv("coremask_override", NULL);
			}
		}
	}
#endif

#ifdef CONFIG_CMD_NET
	/* board MAC address */
	if (eth_getenv_enetaddr("ethaddr", bd->bi_enetaddr)) {
		debug("Setting ethernet address to %pM...\n", bd->bi_enetaddr);
	} else {
		debug("ethaddr not found in environment\n");
	}
#endif
	/* Configuring QLMs */
	debug("Configuring QLMs\n");
	board_configure_qlms();
	debug("Performing board FDT fixups\n");
	board_fixup_fdt();
	debug("Performing OCTEON FDT fixups\n");

	octeon_fixup_fdt();
	debug("Packing flat device tree\n");
	fdt_pack((void *)gd->fdt_blob);
	dnull_ptr_check("FDT and EEPROM");

#ifdef CONFIG_OCTEON_I2C_FDT
	/* We need to do this after packing */
	octeon_i2c_process_fdt();
	dnull_ptr_check("i2c process fdt done");
#endif

	/* Set numcores env variable to indicate the number of cores available
	 */
	snprintf(tmp, sizeof(tmp), "%d", octeon_get_available_core_count());
	debug("Setting numcores to %s\n", tmp);
	setenv("numcores", tmp);
	/* Some people have requested a hex value for eval purposes */
	snprintf(tmp, sizeof(tmp), "0x%x", octeon_get_available_core_count());
	setenv("numcoreshex", tmp);


	WATCHDOG_RESET();
	debug("Setting IP address...\n");
	/* IP Address */
	bd->bi_ip_addr = getenv_IPaddr("ipaddr");
	debug("Done.\n");

	debug("Reserving first 1MB of memory\n");
	rc = cvmx_bootmem_reserve_memory(0, OCTEON_RESERVED_LOW_BOOT_MEM_SIZE,
					 "__low_reserved", 0);
	if (!rc)
		puts("Error reserving low 1MB of memory\n");

#if defined(CONFIG_OCTEON_MMC)
	if (!getenv("disable_mmc")) {
		puts("MMC:   ");
		mmc_initialize(gd->bd);
		debug("%s: Done initializing MMC\n", __func__);
		dnull_ptr_check("MMC initialized");
# if defined(CONFIG_ENV_IS_IN_MMC)
		debug("Loading environment from MMC\n");
		env_relocate_spec();
		dnull_ptr_check("MMC environment loaded\n");
# endif
	}
#endif
#ifdef CONFIG_CMD_USB
	debug("%s: Initializing USB\n", __func__);
	rc = board_usb_preinit();
	if (!rc) {
		rc = -1;
		if (!OCTEON_IS_OCTEON3() &&
			((fdt_node_offset_by_compatible(gd->fdt_blob, 0,
							"cavium,octeon-6335-uctl") >= 0 ) ||
							(fdt_node_offset_by_compatible(gd->fdt_blob, 0,
										       "cavium,octeon-5750-usbn") >= 0))) {
			/* Scan for USB devices automatically on boot.
			 * The host port used can be selected with the 'usb_host_port'
			 * env variable, which can also disable the scanning completely.
			 */
			if (!getenv("disable_usb_scan")) {
				rc = usb_init();
			}
# ifdef CONFIG_USB_EHCI_OCTEON2
			else if (getenv("enable_usb_ehci_clock")) {
				rc = usb_init();
			}
# endif
	       }
	       board_usb_postinit();
	}
#endif
	dnull_ptr_check("USB initialized");
	WATCHDOG_RESET();

#if defined(CONFIG_ENV_IS_IN_FAT)
# ifdef CONFIG_USB_STORAGE
	/* If storage is on USB then we'll scan it here. */
	if (rc == 0 && strcmp(FAT_ENV_INTERFACE, "usb") == 0)
		usb_stor_scan(1);
# endif
	env_relocate_spec();
#endif

#ifndef CONFIG_OCTEON_DISABLE_QLM
        /* Initialize QLM */
        cvmx_qlm_init();

	dnull_ptr_check("QLM initialized");
#endif
#if defined(CONFIG_PCI)
	/*
	 * Do pci configuration
	 */
	if (getenv("disable_pci")) {
		printf("Skipping PCI due to 'disable_pci' environment variable\n");
	} else {
		debug("Initializing PCI\n");
		pci_init();
	}
	dnull_ptr_check("PCI initialized");
#endif

	/** leave this here (after malloc(), environment and PCI are working)
	 **/
	WATCHDOG_RESET();
	debug("Initializing stdio...\n");
	/* Initialize stdio devices */
	stdio_init();
	dnull_ptr_check("stdio initialized");


#ifdef CONFIG_SYS_PCI_CONSOLE
	/* Check to see if we need to set up a PCI console block. Set up
	 * structure if either count is set (and greater than 0) or
	 * pci_console_active is set
	 */
	debug("Setting up PCI console\n");
	if (uboot_octeon_pci_console_init())
		puts("WARNING: Could not initialize PCI console support!\n");
	debug("PCI read buffer base at 0x%x\n",
	      MAKE_KSEG0(BOOTLOADER_PCI_READ_BUFFER_BASE));
	dnull_ptr_check("PCI console initialized");
#endif

	/* leave this here (after malloc(), environment and PCI are working) */
	debug("Initializing jump table...\n");
	jumptable_init();

	debug("Initializing console...\n");
	/* Initialize the console (after the relocation and devices init) */
	/* Must be done after environment variables are set */

	/* stdin, stdout and stderr must be set to serial before calling
	 * console_init_r()
	 */
	setenv("stdin", "serial");
	setenv("stdout", "serial");
	setenv("stderr", "serial");
	/* We can't fully initialize stdin, stdout and stderr at this time
	 * since console_init_r() will switch it back to serial.
	 */
	console_init_r();
	dnull_ptr_check("console initialized");
	debug("Console initialized\n");
	/* Must change std* variables after devices_init()
	 * Force bootup settings based on pci_console_active, overriding the
	 * std* variables
	 */
#ifdef CONFIG_SYS_PCI_CONSOLE
# ifdef CONFIG_OCTEON_BOOTCMD
	debug("Setting stdin, stdout and stderr with serial, pci and bootcmd input...\n");
# else
	debug("Setting stdin, stdout and stderr with serial and pci input...\n");
# endif
	if (getenv("pci_console_active")) {
		if (getenv("serial_console_active")) {
			printf("Using PCI console, serial port also enabled.\n");
# ifdef CONFIG_OCTEON_BOOTCMD
			setenv("stdin", "serial,pci,bootcmd");
# else
			setenv("stdin", "serial,pci");
# endif
			setenv("stdout", "serial,pci");
			setenv("stderr", "serial,pci");
		} else {
			printf("Using PCI console, serial port disabled.\n");
# ifdef CONFIG_OCTEON_BOOTCMD
			setenv("stdin", "pci,bootcmd");
# else
			setenv("stdin", "pci");
# endif
			setenv("stdout", "pci");
			setenv("stderr", "pci");
		}
		octeon_led_str_write("PCI CONS");
	} else {
		debug("PCI console not active\n");
# ifdef CONFIG_OCTEON_BOOTCMD
		dnull_ptr_check("console stdin 2");
		setenv("stdin", "serial,pci,bootcmd");
# else
		setenv("stdin", "serial,pci");
		dnull_ptr_check("console stdin 3");
# endif
		setenv("stdout", "serial");
		dnull_ptr_check("console stdout 2");
		setenv("stderr", "serial");
		dnull_ptr_check("console stderr 2");
	}
	dnull_ptr_check("console initialized 2");
#else
	debug("Setting stdin, stdout and stderr to serial port\n");
	setenv("stdin", "serial");
	setenv("stdout", "serial");
	setenv("stderr", "serial");
	dnull_ptr_check("console initialized 3");
#endif
	/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/
	WATCHDOG_RESET();

	/* We want to print out the board/chip info again, as this is very
	 * useful for automated testing. and board identification.
	 */
	if (getenv("pci_console_active"))
		display_board_info();

	/* Initialize from environment. Note that in most cases the environment
	 * variable will have been set from the reserved memory block allocated
	 * above.
	 * By also checking this here, this allows the load address to be set in
	 * cases where there is no reserved block configured.
	 * If loadaddr is not set, set it to something.  This default should
	 * never really be used,
	 * but we need to handle this case.
	 */
	debug("Setting load address...\n");
#ifdef CONFIG_ENV_IS_NOWHERE
	{
		char tmp[20];
		load_addr = CONFIG_SYS_LOAD_ADDR;
		snprintf(tmp, sizeof(tmp), "0x%lx", load_addr);
		setenv("loadaddr", tmp);
	}
#else
	if ((s = getenv("loadaddr")) != NULL) {
		load_addr = simple_strtoul(s, NULL, 16);
	} else {
		u64 addr = 0x400000;	/* Default to 4MB */
		char tmp[20];
# ifdef CONFIG_SYS_LOAD_ADDR
		addr = CONFIG_SYS_LOAD_ADDR;
# endif
		snprintf(tmp, sizeof(tmp), "0x%llx", addr);
# if !CONFIG_OCTEON_SIM_MEM_LAYOUT
		if (!(gd->flags & GD_FLG_RAM_RESIDENT))
			printf("WARNING: loadaddr not set, defaulting to %s.  Please either define a load reserved block or set the loadaddr environment variable.\n",
			       tmp);
# endif
		setenv("loadaddr", tmp);
	}
#endif

#if !CONFIG_OCTEON_SIM_NO_FLASH && !defined(CONFIG_SYS_NO_FLASH) && \
    !defined(CONFIG_OCTEON_NAND_STAGE2) && \
    !defined(CONFIG_OCTEON_EMMC_STAGE2) && \
    !defined(CONFIG_OCTEON_SPI_STAGE2) && \
    !defined(CONFIG_OCTEON_AUTHENTIK_STAGE2)
	octeon_boot_init_flash();
	dnull_ptr_check("flash size initialized");
#endif

#ifdef CONFIG_CMD_NET
	debug("Getting bootfile...\n");
	if ((s = getenv("bootfile")) != NULL)
		copy_filename(BootFile, s, sizeof(BootFile));
#endif /* CFG_COMMAND_NET */
	octeon_set_boardname();

	WATCHDOG_RESET();

	/* miscellaneous platform dependent initialisations */
	debug("Configuring misc...\n");
	misc_init_r();
	dnull_ptr_check("misc initialized");
	WATCHDOG_RESET();

#ifdef CONFIG_CMD_IDE
	debug("Configuring IDE\n");

	/* Always do ide_init if PCI enabled to allow for PCI IDE
	 * devices
	 */
	ide_init();
	WATCHDOG_RESET();
	dnull_ptr_check("compact flash initialized");
#endif
#ifdef CONFIG_PCI
# if defined(CONFIG_CMD_SATA)
	sata_initialize();
# endif
# if defined(CONFIG_CMD_NVME)
	nvme_initialize();
# endif
#endif
#ifdef CONFIG_CMD_DTT
	debug("Initializing temperature sensor...\n");
	dtt_init();
	WATCHDOG_RESET();
#endif
#if defined(CONFIG_CMD_NET) && defined(CONFIG_NET_MULTI) && \
    !defined(CONFIG_OCTEON_DISABLE_NETWORKING)
	debug("Fixing up the MAC address again\n");
	octeon_fixup_fdt_mac_addr();
	puts("Net:   ");
	board_net_preinit();
	eth_initialize(gd->bd);
	debug("Net configured.\n");
	debug("Initializing board MDIO interfaces/PHYs\n");
	board_mdio_init();
	board_net_postinit();
	WATCHDOG_RESET();
	dnull_ptr_check("network initialized");
#endif

	/* verify that boot_init_vector type is the correct size */
	if (BOOT_VECTOR_NUM_WORDS * 4 != sizeof(boot_init_vector_t)) {
		printf("ERROR: boot_init_vector_t size mismatch: define: %d, type: %d\n",
		       BOOT_VECTOR_NUM_WORDS * 4,
		       sizeof(boot_init_vector_t));
		hang();
	}
	debug("Storing MAC address info in named block...\n");

#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
    debug("Doing remote hotplug initialization...\n");
    octeon_remote_hotplug_init();
    WATCHDOG_RESET();
#endif

	debug("Doing late board init...\n");
	late_board_init();
	dnull_ptr_check("late board initialized");
	WATCHDOG_RESET();

	debug("Setting serial number to %s\n",
	      (char *)gd->arch.board_desc.serial_str);
	/* Set the serial # */
	setenv_force("serial#", (char *)gd->arch.board_desc.serial_str);

	/* Set a few environment flags based on the U-Boot mode */
	setenv("octeon_failsafe_mode",
	       (gd->flags & GD_FLG_FAILSAFE_MODE) ? "1" : "0");
	setenv("octeon_ram_mode",
	       (gd->flags & GD_FLG_RAM_RESIDENT) ? "1" : "0");
#ifdef CONFIG_CMD_FDT
	set_working_fdt_addr(working_fdt);
#endif
	octeon_mac_info_init();

#if defined(CONFIG_OCTEON_BOOTCMD) && defined(CONFIG_SYS_PCI_CONSOLE)
	debug("Unlocking PCI boot command console\n");
	/* Unlock the bootcmd console */
	octeon_pci_bootcmd_unlock();
#endif

#ifdef CONFIG_OCTEON_SE_NAPI
	seapi_init();
#endif

#if defined(CONFIG_USB_STORAGE) && !defined(CONFIG_OCTEON_HIDE_USB_START_MSG)
	if ((fdt_node_offset_by_compatible(gd->fdt_blob, 0,
					   "cavium,octeon-6335-uctl") >= 0 ) ||
	    (fdt_node_offset_by_compatible(gd->fdt_blob, 0,
					   "cavium,octeon-5750-usbn") >= 0) ||
	    (fdt_node_offset_by_compatible(gd->fdt_blob, 0,
					   "cavium,octeon-7130-usb-uctl") >= 0))
		puts("Type the command \'usb start\' to scan for USB storage devices.\n\n");
#endif

	/* Check if NULL address is still zero */
	null_ptr_check(NULL);

	debug("Entering main loop.\n");
	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;)
		main_loop();

	/* NOTREACHED - no way out of command loop except booting */
}

#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
int octeon_remote_hotplug_init(void)
{
	const bool reserve_core_0 = false;
	cvmx_coremask_t cm;
	/* setup labi */
	extern void setup_linux_app_boot_info(cvmx_coremask_t *linux_core_mask);

	cvmx_coremask_clear_all(&cm);
	if (reserve_core_0)
		cvmx_coremask_set_core(&cm, 0);
	setup_linux_app_boot_info(&cm);
	debug("EXECUTED setup_linux_app_boot_info(&cm)\n");

	/* set boot_vect for all cores to run 'idle' */
	if (reserve_core_0)
		cvmx_coremask_set_cores(&cm, 1,
					cvmx_octeon_num_cores()/*16*/-1);
	else
		cvmx_coremask_set_cores(&cm, 0,
					cvmx_octeon_num_cores()/*16*/);

	octeon_setup_boot_vector(0/*func_addr*/, &cm);
	debug("EXECUTED octeon_setup_boot_vector(0/*func_addr*/, &cm)\n");
	return 0;
}
#endif
