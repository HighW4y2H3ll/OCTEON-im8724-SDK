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
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/

/**
 * This file contains common functions used for DRAM such as initializing DRAM
 * and DFM memory, testing memory, clearing memory, and importing environment
 * variables from DRAM.
 */

#include <common.h>
#include <environment.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_dram.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-core.h>
#include <asm/arch/cvmx-bootloader.h>
#include <asm/arch/cvmx-l2c.h>
#include <asm/arch/octeon-model.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-l2c-defs.h>
#include <asm/arch/cvmx-gpio.h>

DECLARE_GLOBAL_DATA_PTR;

extern ulong uboot_end_data;
extern ulong uboot_end;
extern char uboot_start;

extern int octeon_check_env(const env_t *env, size_t size);

/** Short cut to convert a number to megabytes */
#define MB(X)	((uint64_t)(X) * (uint64_t)(1024 * 1024))

#ifdef CONFIG_OCTEON_OCX
/**
 * Zeros out all memory for the specified node.
 *
 * @param node	node number to zero memory for.
 */
static void octeon_clear_mem_node(int node)
{
	uint64_t start, size;

	debug("%s(%d)\n", __func__, node);
	if (node == 0) {
		printf("%s: Error, this should only be used for non-zero nodes\n",
		       __func__);
		return;
	}
	size = MB(gd->arch.ram_sizes[node]);
	start = MAKE_XKPHYS((uint64_t)node << CVMX_NODE_MEM_SHIFT);

	/* We need to skip the boot bus hole */
	debug("%s: Clearing 0x%llx bytes on node %d starting at 0x%llx\n",
	      __func__, min(size, MB(256)), node, start);
	octeon_bzero64_pfs(start, min(size, MB(256)));

	/* This is extremely unlikely, 256MB for a multi-node system?!?!? */
	if (size <= MB(256))
		return;

	start = MAKE_XKPHYS(((uint64_t)node << CVMX_NODE_MEM_SHIFT) + MB(512));
	size -= MB(256);
	debug("%s: Clearing 0x%llx bytes on node %d starting at 0x%llx\n",
	      __func__, size, node, start);
	octeon_bzero64_pfs(start, size);
}
#endif

#ifndef CONFIG_OCTEON_DISABLE_DDR3
static uint32_t
get_octeon3_ddr3_reference_clock(uint32_t alt_refclk,
				 uint32_t ddr_hertz,
				 const dimm_config_t *dimm_config)
{
	uint32_t ddr_ref_hertz = gd->arch.ddr_ref_hertz;
	int ddr_type;
	int spd_dimm_type;

	debug("%s(%u, %u, %p)\n", __func__, alt_refclk, ddr_hertz, dimm_config);
	if (!OCTEON_IS_OCTEON3())
		return ddr_ref_hertz;

	/* we know whether alternate refclk is always wanted
	 * we also know already if we want 2133 MT/s
	 * if alt refclk not always wanted, then probe DDR and
	 * DIMM type if DDR4 and RDIMMs, then set desired refclk
	 * to 100MHz, otherwise to default (50MHz)
	 * depend on ddr_initialize() to do the refclk selection
	 * and validation/
	 */
	if (alt_refclk) {
		/* if alternate refclk was specified, let it override
		 * everything
		 */
		ddr_ref_hertz = alt_refclk * 1000000;
		printf("%s: DRAM init: %d MHz refclk is REQUESTED ALWAYS\n",
		       __func__, alt_refclk);
	} else if (ddr_hertz > 1000000000) {
		ddr_type = get_ddr_type(dimm_config, 0);
		spd_dimm_type = get_dimm_module_type(dimm_config, 0, ddr_type);

		debug("ddr type: 0x%x, dimm type: 0x%x\n", ddr_type, spd_dimm_type);
		/* Is DDR4 and RDIMM just to be sure. */
		if ((ddr_type == DDR4_DRAM) &&
                    ((spd_dimm_type == 1) || (spd_dimm_type == 5) || (spd_dimm_type == 8))) {
			/* Yes, we require 100MHz refclk, so set it. */
			ddr_ref_hertz = 100000000;
			puts("DRAM init: 100 MHz refclk is REQUIRED\n");
		}
	}
	debug("%s: speed: %u\n", __func__, ddr_ref_hertz);
	return ddr_ref_hertz;
}
#endif

/**
 * Obtain the memclk speed in MHz for non-OCX cases
 *
 * @param	cpu_id		ID of CPU we're running on
 *
 * @return	Memory clock speed in MHz
 */
static uint32_t get_memclk_speed(uint32_t cpu_id)
{
	int ddr_int = 0;
	debug("%s(0x%x)\n", __func__, cpu_id);
	if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		/* Check to see if interface 0 is enabled */
		cvmx_l2c_cfg_t l2c_cfg;
		l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);
		if (!l2c_cfg.s.dpres0) {
			debug("DDR interface 0 disabled, checking 1\n");
			if (l2c_cfg.s.dpres1) {
				debug("DDR interface 1 enabled\n");
				ddr_int = 1;
			} else
				puts("ERROR: no DRAM controllers initialized\n");
		}
	}
	return divide_nint(measure_octeon_ddr_clock(cpu_id,
						    NULL,
						    gd->cpu_clk,
						    0, 0,
						    ddr_int, 0,
						    0),
			   1000000);
}

#if defined(CONFIG_OCTEON_EBB7800) || defined(CONFIG_OCTEON_EBB7804)
/**
 * CN78XX pass 1.0 has broken i2c so we have to use other methods of configuring
 * the DIMM configuration.
 *
 * @param	board_type	Incoming board type
 * @return	"fixed" board type.
 */
static uint16_t fixup_cn78xx_board_type(uint16_t board_type)
{
	uint16_t ddr_board_type = board_type;
	debug("%s(%u)\n", __func__, board_type);
	if (!OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_0))
		return ddr_board_type;
	if (board_type == CVMX_BOARD_TYPE_EBB7800) {
		gpio_direction_input(1);
		if (!gpio_get_value(1)) {
			ddr_board_type = CVMX_BOARD_TYPE_EBB7800_CFG1;
			puts("DDR Configuration 1: gpio 1 = ON\n");
		} else {
			ddr_board_type = CVMX_BOARD_TYPE_EBB7800_CFG0;
			puts("DDR Configuration 0: gpio 1 = OFF\n");
		}
	} else if (board_type == CVMX_BOARD_TYPE_EBB7804) {
		gpio_direction_input(1);
		if (!gpio_get_value(1)) {
			ddr_board_type = CVMX_BOARD_TYPE_EBB7804_CFG1;
			puts("DDR Configuration 1: gpio 1 = ON\n");
		} else {
			ddr_board_type = CVMX_BOARD_TYPE_EBB7804_CFG0;
			puts("DDR Configuration 0: gpio 1 = OFF\n");
		}
	}
	return ddr_board_type;
}
#endif

/**
 * This function clears DRAM after initializing it.
 */
static void octeon_clear_dram(void)
{
#ifdef CONFIG_OCTEON_OCX
	int node;
#endif
	if (gd->flags & GD_FLG_RAM_RESIDENT) {
		debug("Preserving environment in RAM\n");
		octeon_bzero64_pfs(0xffffffff80000000ULL,
				   U_BOOT_RAM_ENV_ADDR);
		octeon_bzero64_pfs(0xffffffff80000000ULL |
		(U_BOOT_RAM_ENV_ADDR_2
		+ U_BOOT_RAM_ENV_SIZE),
		OCTEON_DRAM_LATE_ZERO_OFFSET
		- (U_BOOT_RAM_ENV_ADDR_2
		+ U_BOOT_RAM_ENV_SIZE));
	} else if ((gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE) &&
		   !(gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_FLASH) &&
		   (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN76XX) ||
		    OCTEON_IS_MODEL(OCTEON_CNF75XX) ) &&
		   !octeon_check_env((env_t *)U_BOOT_CACHE_ENV_ADDR,
				     U_BOOT_CACHE_ENV_SIZE)) {
		debug("%s: Found environment in cache, preserving\n",
		      __func__);
		cvmx_l2c_lock_mem_region(MAKE_XKPHYS(U_BOOT_CACHE_ENV_ADDR),
					 U_BOOT_CACHE_ENV_SIZE);
		/* If the environment is loaded into the L2
		 * cache then preserve it.
		 */
#if (U_BOOT_CACHE_ENV_ADDR + U_BOOT_CACHE_ENV_SIZE < OCTEON_DRAM_LATE_ZERO_OFFSET)
		octeon_bzero64_pfs(0xffffffff80000000ULL,
				   U_BOOT_CACHE_ENV_ADDR);
		octeon_bzero64_pfs(0xffffffff80000000ULL
				   + U_BOOT_CACHE_ENV_ADDR
				   + U_BOOT_CACHE_ENV_SIZE,
				   OCTEON_DRAM_LATE_ZERO_OFFSET
				    - (U_BOOT_CACHE_ENV_ADDR +
				       U_BOOT_CACHE_ENV_SIZE));
#elif (U_BOOT_CACHE_ENV_ADDR + U_BOOT_CACHE_ENV_SIZE == OCTEON_DRAM_LATE_ZERO_OFFSET)
		octeon_bzero64_pfs(0xffffffff80000000ULL,
				   U_BOOT_CACHE_ENV_ADDR);
#else
# error U-Boot Cache environment address too high!
#endif
	} else {
		debug("Clearing memory from 0 to 0x%llx\n",
		      OCTEON_DRAM_LATE_ZERO_OFFSET);
		octeon_bzero64_pfs(0xffffffff80000000ULL,
				   OCTEON_DRAM_LATE_ZERO_OFFSET);
	}
#ifdef CONFIG_OCTEON_OCX
	/* Clear memory for additional nodes */
	cvmx_coremask_for_each_node(node, gd->arch.node_mask & 0xe)
		octeon_clear_mem_node(node);
#endif
}

/**
 * Initialize system DRAM
 */
int octeon_init_dram(void)
{
	uint64_t uboot_addr;
	uint32_t mem_mbytes = 0;
	uint32_t measured_ddr_hertz = 0;
	uint32_t ddr_config_valid_mask = 0;
	uint32_t ddr_hertz;
	uint32_t ddr_ref_hertz;
	uint32_t cpu_id = cvmx_get_proc_id();
	const ddr_configuration_t *ddr_config_ptr;
	uint16_t ddr_board_type = gd->arch.board_desc.board_type;
	const char *eptr;
	uint32_t new_mem_mbytes;
#ifdef CONFIG_OCTEON_OCX
	int node;
#endif
	debug("Initializing DRAM\n");

	if (gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE) {
		uboot_addr = cvmx_ptr_to_phys(&uboot_start);
		if (uboot_addr >= 0x10000000 && uboot_addr < 0x20000000) {
			debug("Locking %u bytes of cache starting at 0x%llx\n",
			(void *)&uboot_end - (void *)&uboot_start,
			cvmx_ptr_to_phys(&uboot_start));
			cvmx_l2c_lock_mem_region(cvmx_ptr_to_phys(&uboot_start),
						 (void *)&uboot_end - (void *)&uboot_start);
		}
	}
#if CONFIG_OCTEON_SIM_NO_DDR
# if defined(CONFIG_OCTEON_NAND_STAGE2) || defined(CONFIG_OCTEON_EMMC_STAGE2) \
     || defined(CONFIG_OCTEON_AUTHENTIK_STAGE2)
	/* For simulating NAND boot, we don't use normal oct-sim helper, so we
	 * don't get the DRAM size in the special location
	 */
	gd->ram_size = MB(128);
	gd->arch.ram_sizes[0] = (128);
#  ifdef CONFIG_OCTEON_OCX
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		int i;
		int node;
		cvmx_coremask_for_each_node(i, gd->arch.node_mask)
			gd->arch.ram_sizes[i] = 128;
	}
#  endif
# else
	gd->ram_size = MB(*((uint32_t *) OCTEON_BOOTLOADER_MEMSIZE_ADDR));
	gd->arch.ram_sizes[0] =
		(*(((uint32_t *)OCTEON_BOOTLOADER_MEMSIZE_BASE_ADDR) + 0));
#  ifdef CONFIG_OCTEON_OCX
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		int i;
		/* note that these sizes are in in MBytes */
		cvmx_coremask_for_each_node(i, gd->arch.node_mask)
			gd->arch.ram_sizes[i] =
				(*(((uint32_t *)OCTEON_BOOTLOADER_MEMSIZE_BASE_ADDR) + i));
	}
#  endif
# endif
	return 0;
#endif

	if (gd->flags & GD_FLG_RAM_RESIDENT && getenv("dram_size_mbytes")) {
		debug("U-Boot is RAM resident\n");
		/* DRAM size has been based by the remote entity that configured
		 * the DRAM controller, so we use that size rather than
		 * re-computing by reading the DIMM SPDs. */
		mem_mbytes = simple_strtoul(getenv("dram_size_mbytes"),
					      NULL, 0);
		printf("Using DRAM size from environment: %d MBytes\n",
		       mem_mbytes);
		gd->mem_clk = get_memclk_speed(cpu_id);
		debug("DDR clock is %lu MHz\n", gd->mem_clk);
	} else {	/* Not RAM-resident case */
#ifndef CONFIG_OCTEON_DISABLE_DDR3
                int alt_refclk = gd->arch.ddr_alt_ref_MHz;
#endif
		debug("U-Boot is not RAM-resident\n");
#if defined(CONFIG_OCTEON_EBB7800) || defined(CONFIG_OCTEON_EBB7804)
		ddr_board_type = fixup_cn78xx_board_type(ddr_board_type);
#endif
		/* TODO: Make this multi-node aware */
		ddr_config_ptr = lookup_ddr_config_structure(cpu_id,
							     ddr_board_type,
							     gd->arch.board_desc.rev_major,
							     gd->arch.board_desc.rev_minor,
							     &ddr_config_valid_mask);

		if (!ddr_config_ptr) {
			printf("ERROR: unable to determine DDR configuration "
			       "for board type: %s (%d)\n",
			       cvmx_board_type_to_string(gd->arch.board_desc.board_type),
			       gd->arch.board_desc.board_type);
			return -1;
		}
		/* Check for special case of mismarked 3005 samples,
		 * and adjust cpuid
		 */
		if ((cpu_id == OCTEON_CN3010_PASS1) &&
		    (cvmx_read_csr(CVMX_L2D_FUS3) & (1ull << 34)))
			cpu_id |= 0x10;

		ddr_hertz = gd->mem_clk * 1000000;

		if ((eptr = getenv("ddr_clock_hertz")) != NULL) {
			ddr_hertz = simple_strtoul(eptr, NULL, 0);
			gd->mem_clk = divide_nint(ddr_hertz, 1000000);
			printf("Parameter found in environment.  ddr_clock_hertz = %d\n",
			       ddr_hertz);
		}

		ddr_ref_hertz = gd->arch.ddr_ref_hertz;

#ifndef CONFIG_OCTEON_DISABLE_DDR3
		ddr_ref_hertz =
			get_octeon3_ddr3_reference_clock(alt_refclk,
				ddr_hertz,
				&ddr_config_ptr[0].dimm_config_table[0]);
#endif  /* ifndef CONFIG_OCTEON_DISABLE_DDR3 */

		ddr_ref_hertz = getenv_ulong("ddr_ref_hertz", 10, ddr_ref_hertz);

		debug("Initializing DDR, clock = %uhz, reference = %uhz\n",
		      ddr_hertz, ddr_ref_hertz);
#ifdef CONFIG_OCTEON_OCX
		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			debug("%s: Initializing memory for node %d\n",
			     __func__, node);
			mem_mbytes = octeon_ddr_initialize(cpu_id,
							   cvmx_clock_get_rate_node(node, CVMX_CLOCK_CORE),
							   ddr_hertz,
							   ddr_ref_hertz,
							   ddr_config_valid_mask,
							   ddr_config_ptr,
							   &measured_ddr_hertz,
							   gd->arch.board_desc.board_type,
							   gd->arch.board_desc.rev_major,
							   gd->arch.board_desc.rev_minor,
							   node);
			if (mem_mbytes > 0)
				gd->arch.ram_sizes[node] = mem_mbytes;
			debug("Flushing L2 cache after initializing node %d, node mask: 0x%x\n",
			      node, gd->arch.node_mask);
			cvmx_l2c_flush();
			debug("Node %d mem size in MBYTES: %d\n",
			      node, mem_mbytes);
		}
#else
		mem_mbytes = octeon_ddr_initialize(cpu_id,
						   cvmx_clock_get_rate(CVMX_CLOCK_CORE),
						   ddr_hertz,
						   ddr_ref_hertz,
						   ddr_config_valid_mask,
						   ddr_config_ptr,
						   &measured_ddr_hertz,
						   gd->arch.board_desc.board_type,
						   gd->arch.board_desc.rev_major,
						   gd->arch.board_desc.rev_minor,
						   0
  						);
		debug("Mem size in MBYTES: %u\n", mem_mbytes);
#endif
		gd->mem_clk = divide_nint(measured_ddr_hertz, 1000000);

		debug("Measured DDR clock %d Hz\n", measured_ddr_hertz);

		if (measured_ddr_hertz != 0) {
			if (!gd->mem_clk) {
				/* If ddr_clock not set, use measured clock
				 * and don't warn
				 */
				gd->mem_clk = divide_nint(measured_ddr_hertz,
							  1000000);
			} else if ((measured_ddr_hertz > ddr_hertz + 3000000) ||
				   (measured_ddr_hertz < ddr_hertz - 3000000)) {
				printf("\nWARNING:\n");
				printf("WARNING: Measured DDR clock mismatch!  "
				       "expected: %lld MHz, measured: %lldMHz, "
				       "cpu clock: %lu MHz\n",
				       divide_nint(ddr_hertz, 1000000),
				       divide_nint(measured_ddr_hertz, 1000000),
				       gd->cpu_clk);
				if (!(gd->flags & GD_FLG_RAM_RESIDENT))
					printf("WARNING: Using measured clock "
					       "for configuration.\n");
				printf("WARNING:\n\n");
				gd->mem_clk =
				    divide_nint(measured_ddr_hertz, 1000000);
			}
		}

		if (gd->flags & GD_FLG_CLOCK_DESC_MISSING)
			puts("Warning: Clock descriptor tuple not found in eeprom, using defaults\n");

		if (gd->flags & GD_FLG_BOARD_DESC_MISSING)
			puts("Warning: Board descriptor tuple not found in eeprom, using defaults\n");

		new_mem_mbytes = getenv_ulong("dram_size_mbytes", 0,
					      mem_mbytes);
		if (mem_mbytes > 0 && new_mem_mbytes != mem_mbytes) {
			/* Override the actual DRAM size. */
			if (new_mem_mbytes < mem_mbytes) {
				printf("!!! Overriding DRAM size: dram_size_mbytes = %d MBytes !!!\n",
				       mem_mbytes);
				mem_mbytes = new_mem_mbytes;
			} else {
				puts("!!! Memory size override failed due to actual memory size being smaller than requested size !!!\n");
			}
		}
	}

	if (mem_mbytes > 0) {
		gd->ram_size = MB(mem_mbytes);
		gd->arch.ram_sizes[0] = mem_mbytes;
		/* For 6XXX generate a proper error when reading/writing
		 * non-existant memory locations.
		 */
#ifdef CONFIG_OCTEON_OCX
		cvmx_coremask_for_each_node(node, gd->arch.node_mask)
			cvmx_l2c_set_big_size_node(node,
						   gd->arch.ram_sizes[node], 0);
#else
		cvmx_l2c_set_big_size((uint64_t)mem_mbytes, 0);
#endif

		debug("Ram size %uMiB (0x%llx)\n", mem_mbytes, gd->ram_size);

		/* If already running in memory don't zero it. */
		/* Zero the low Megabyte of DRAM used by bootloader.
		 * The rest is zeroed later when running from DRAM.
		 */
#if !defined(CONFIG_NO_CLEAR_DDR)
		/* Preserve environment in RAM if it exists */
		if (!(gd->flags & GD_FLG_MEMORY_PRESERVED))
			octeon_clear_dram();
		debug("Done clearing memory\n");
#endif /* !CONFIG_NO_CLEAR_DDR */
		return 0;
	}
	return -1;
}

/**
 * Clear DRAM ECC errors for Octeon II chips
 */
static void octeon_dram_clear_octeon2_ecc_errors(void)
{
	int tad, lmc, num_lmc;

	cvmx_write_csr(CVMX_L2C_INT_REG,
		       cvmx_read_csr(CVMX_L2C_INT_REG));
	for (tad = 0; tad < CVMX_L2C_TADS; tad++) {
		cvmx_write_csr(CVMX_L2C_ERR_TDTX(tad),
			       cvmx_read_csr(CVMX_L2C_ERR_TDTX(tad)));
		cvmx_write_csr(CVMX_L2C_ERR_TTGX(tad),
			       cvmx_read_csr(CVMX_L2C_ERR_TTGX(tad)));
	}


	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		num_lmc = 4;
	else
		num_lmc = 1;

	for (lmc = 0; lmc < num_lmc; lmc++) {
		cvmx_lmcx_dll_ctl2_t ctl;
		ctl.u64 = cvmx_read_csr(CVMX_LMCX_DLL_CTL2(lmc));
		if ((OCTEON_IS_MODEL(OCTEON_CN68XX) && ctl.cn68xx.intf_en) ||
                    ! OCTEON_IS_MODEL(OCTEON_CN68XX))
			cvmx_write_csr(CVMX_LMCX_INT(lmc),
				       cvmx_read_csr(CVMX_LMCX_INT(lmc)));
	}
}

/**
 * Clear DRAM ECC errors for CN70XX chips
 */
static void octeon_dram_clear_70xx_ecc_errors(void)
{
	cvmx_write_csr(CVMX_CIU_CIB_LMCX_RAWX(0, 0),
		       cvmx_read_csr(CVMX_CIU_CIB_LMCX_RAWX(0, 0)));
	cvmx_write_csr(CVMX_CIU_CIB_L2C_RAWX(0),
		       cvmx_read_csr(CVMX_CIU_CIB_L2C_RAWX(0)));
	cvmx_write_csr(CVMX_L2C_TADX_INT(0),
		       cvmx_read_csr(CVMX_L2C_TADX_INT(0)));
}

/**
 * Clear DRAM ECC errors for CN78XX chip onwards
 */
static void octeon_dram_clear_7xxx_ecc_errors(void)
{
	int tad, node, i, lmc, j;

	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		for (tad = 0; tad < CVMX_L2C_TADS; tad++) {
			debug("%s: clearing errors for node: %d, tad: %d\n",
			      __func__, node, tad);
			cvmx_write_csr_node(node, CVMX_L2C_TADX_INT(tad),
			       cvmx_read_csr_node(node, CVMX_L2C_TADX_INT(tad)));

			/* Also clear CIU3 INT */
			for (i = 0; i < 20; i++) {
				uint64_t intsn;
				intsn = ((0x80000 + i) + (tad << 8));
				/* Check for pending interrupt */
				if ((cvmx_read_csr_node(node,
							CVMX_CIU3_ISCX_CTL(intsn))) & 1) {
					cvmx_write_csr_node(node,
							    CVMX_CIU3_ISCX_W1C(intsn),
							    1);
				}
			}
		}
		if (OCTEON_IS_MODEL(OCTEON_CN78XX))
			i = 4;
		else
			i = 2;
		for (lmc = 0; lmc < i; lmc++) {
			cvmx_lmcx_dll_ctl2_t ctl;
			ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(lmc));

                        /* We can't rely on the intf_en field. Must use !dreset */
			if (!ctl.cn78xx.dreset)
				cvmx_write_csr_node(node, CVMX_LMCX_INT(lmc),
						   cvmx_read_csr_node(node, CVMX_LMCX_INT(lmc)));
			else
				continue;

			/* Also clear CIU3 INT */
			for (j = 0; j < 14; j++) {
				uint64_t intsn;
				intsn = ((0x88000 + j) + (lmc << 12));
				/* Check for pending interrupt */
				if ((cvmx_read_csr_node(node,
							CVMX_CIU3_ISCX_CTL(intsn))) & 1) {
					cvmx_write_csr_node(node,
							    CVMX_CIU3_ISCX_W1C(intsn),
							    1);
				}
			}
		}
	}
}

/**
 * Clear all DRAM ECC errors
 *
 * @return 0 for success
 */
int octeon_dram_clear_ecc_errors(void)
{
#ifdef CONFIG_OCTEON_CN70XX
	octeon_dram_clear_70xx_ecc_errors();
#else
	/* Clear DDR/L2C ECC error bits */
	if (OCTEON_IS_OCTEON2()) {
		octeon_dram_clear_octeon2_ecc_errors();
	} else if (OCTEON_IS_OCTEON3()) {
		if (OCTEON_IS_MODEL(OCTEON_CN70XX))
			octeon_dram_clear_70xx_ecc_errors();
		else if (OCTEON_IS_MODEL(OCTEON_CN7XXX))
			octeon_dram_clear_7xxx_ecc_errors();
		else
			printf("%s: Error: Add support for OCTEON 3 model\n",
			       __func__);
	} else {
		cvmx_write_csr(CVMX_L2D_ERR, cvmx_read_csr(CVMX_L2D_ERR));
		cvmx_write_csr(CVMX_L2T_ERR, cvmx_read_csr(CVMX_L2T_ERR));
	}
	if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		cvmx_l2c_cfg_t l2c_cfg;
		l2c_cfg.u64 = cvmx_read_csr(CVMX_L2C_CFG);
		if (l2c_cfg.cn56xx.dpres0) {
			cvmx_write_csr(CVMX_LMCX_MEM_CFG0(0),
				       cvmx_read_csr(CVMX_LMCX_MEM_CFG0(0)));
		}
		if (l2c_cfg.cn56xx.dpres1) {
			cvmx_write_csr(CVMX_LMCX_MEM_CFG0(1),
				       cvmx_read_csr(CVMX_LMCX_MEM_CFG0(1)));
		}
	} else if (OCTEON_IS_MODEL(OCTEON_CN3XXX)
		   || OCTEON_IS_MODEL(OCTEON_CN5XXX)) {
		cvmx_write_csr(CVMX_LMC_MEM_CFG0,
			       cvmx_read_csr(CVMX_LMC_MEM_CFG0));
	}
#endif
	return 0;
}

#if DO_SIMPLE_DRAM_TEST_FROM_FLASH
/**
 * Very simple DRAM test to run from flash for cases when bootloader boot
 * can't complete.
 */
int octeon_dram_test(void)
{
	uint32_t *addr;
	uint32_t *start = (void *)(OCTEON_L2_MEMCPY_ADDR + 0x0400000);
	uint32_t *end   = (void *)(OCTEON_L2_MEMCPY_ADDR + 0x2400000);
	uint32_t val, incr, readback, pattern;
	int error_limit;
	int i;

	if (gd->flags & GD_FLG_RAM_RESIDENT) {
		printf("Skipping DDR test in ram-resident mode.\n");
		return 0;
	}
	pattern = 0;
	printf("Performing simple DDR test from flash.\n");

#define DRAM_TEST_ERROR_LIMIT 200

	incr = 0x4321;
	for (i = 0; i < 10; i++) {
		error_limit = DRAM_TEST_ERROR_LIMIT;

		printf("\rPattern %08X  Writing..."
		       "%12s" "\b\b\b\b\b\b\b\b\b\b", pattern, "");

		for (addr = start, val = pattern; addr < end; addr++) {
			*addr = val;
			val += incr;
		}

		puts("Reading...");

		for (addr = start, val = pattern; addr < end; addr++) {
			readback = *addr;
			if (readback != val && error_limit-- > 0) {
				if (error_limit + 1 == DRAM_TEST_ERROR_LIMIT)
					puts("\n");
				printf("Mem error @ 0x%08X: "
				       "found %08X, expected %08X, "
				       "xor %08X\n",
				       (uint) addr, readback, val,
				       readback ^ val);
			}
			val += incr;
		}

		/*
		 * Flip the pattern each time to make lots of zeros and
		 * then, the next time, lots of ones.  We decrement
		 * the "negative" patterns and increment the "positive"
		 * patterns to preserve this feature.
		 */
		if (pattern & 0x80000000)
			pattern = -pattern;	/* complement & increment */
		else
			pattern = ~pattern;
		if (error_limit <= 0)
			printf("Too many errors, printing stopped\n");
		incr = -incr;
	}

	puts("\n");
	return 0;

}
#endif

#if OCTEON_CONFIG_DFM && !CONFIG_OCTEON_SIM_NO_DDR
/**
 * Initialize the DFM memory
 */
int octeon_init_dfm(void)
{
	int mem_mbytes = 0;

	/* Check if HFA is present */
	if (!octeon_has_feature(OCTEON_FEATURE_HFA))
		return 0;

	if (getenv("disable_dfm")) {
		printf("disable_dfm is set, skipping DFM initialization\n");
		return 0;
	}
	mem_mbytes = octeon_dfm_initialize();

	if (mem_mbytes >= 0) {
		gd->arch.dfm_ram_size = MB(mem_mbytes);
		return 0;
	} else {
		return -1;
	}
}
#endif

/**
 * Clears DRAM, while taking care to not overwrite DRAM already in use
 * by u-boot
 */
void octeon_clear_mem_pfs(uint64_t base_addr, uint64_t size)
{
	uint64_t ub_base = gd->bd->bi_uboot_ram_addr;
	uint64_t ub_size = gd->bd->bi_uboot_ram_used_size;

	debug("%s: Clearing base address: 0x%llx, size: 0x%llx, "
	      "ub_base: 0x%llx, ub_size: 0x%llx\n",
	      __func__, base_addr, size, ub_base, ub_size);

	if (ub_base >= base_addr && ub_base < (base_addr + size)) {
		/* We need to avoid overwriting the u-boot section */
		octeon_bzero64_pfs(MAKE_XKPHYS(base_addr), ub_base - base_addr);
		octeon_bzero64_pfs(MAKE_XKPHYS(ub_base + ub_size),
				   size - (ub_base - base_addr) - (ub_size));
	} else {
		/* just clear it */
		octeon_bzero64_pfs(MAKE_XKPHYS(base_addr), size);
	}
	debug("Done clearing memory, ub_base: 0x%llx\n", ub_base);
}

/**
 * Import any environment variables passed via a remote bootloader.  It is
 * only active if U-Boot was already loaded into RAM.
 */
int octeon_import_ram_env(void)
{
	env_t *ep = (env_t *)cvmx_phys_to_ptr(U_BOOT_RAM_ENV_ADDR);
	char *s;
	uint32_t crc;
	int i;
	int old_env_size;

	if ((gd->flags & GD_FLG_RAM_RESIDENT) &&
	    !(gd->bd->bi_bootflags & OCTEON_BD_FLAG_BOOT_CACHE)) {
		/* Import any environment variables passed by the remote
		 * bootloader.
		 * Make a copy of the passed-in environment variables and
		 * append the default environment variables after them.
		 * Later on the passed-in environment variables will be
		 * re-imported to override any that are stored in flash.
		 */
		debug("Importing environment from RAM address 0x%x\n",
		      U_BOOT_RAM_ENV_ADDR);
		crc = crc32(0, ep->data, U_BOOT_RAM_ENV_SIZE - U_BOOT_RAM_ENV_CRC_SIZE);
		if (ep->crc != crc) {
			printf("Environment passed by remote boot loader has "
			       "a bad CRC!\n");
			return 0;
		}
		/* Copy the environment before modifying */
		memcpy((char *)cvmx_phys_to_ptr(U_BOOT_RAM_ENV_ADDR_2), ep,
		       U_BOOT_RAM_ENV_SIZE);
		ep = (env_t *)cvmx_phys_to_ptr(U_BOOT_RAM_ENV_ADDR_2);
		/* Get size of environment minus last terminator*/
		s = (char *)&ep->data[0];
		for (i = 0; s[i] != '\0' || s[i+1] != '\0'; i++)
			;
		i++;
		debug("RAM environment is %u bytes\n", i);
		/* Get size of existing environment */
		s = (char *)gd->env_addr;
		if (s) {
			for (old_env_size = 0;
			     s[old_env_size] != '\0' ||
		             s[old_env_size+1] != '\0';
			     old_env_size++)
				;
			/* Append the old environment to the end of the new one */
			old_env_size = min(old_env_size, U_BOOT_RAM_ENV_SIZE - i);
			if (old_env_size >= U_BOOT_RAM_ENV_SIZE - i) {
				printf("Truncating old environment from %u to %u bytes\n",
				old_env_size, U_BOOT_RAM_ENV_SIZE - i);
				old_env_size = U_BOOT_RAM_ENV_SIZE - i;
			}

			if (old_env_size > 0)
				memcpy(&ep->data[i], (char *)gd->env_addr,
				       old_env_size + 1);
		}
		gd->env_addr =
			(unsigned long)cvmx_phys_to_ptr(U_BOOT_RAM_ENV_ADDR_2 +
							U_BOOT_RAM_ENV_CRC_SIZE);
		gd->env_valid = 1;
#ifdef DEBUG
		/* Print out environment variables */
		s = (char *)ep->data;
		puts("Early Environment:\n");
		while (*s) {
			putc(*s++);
			if (*s == '\0') {
				s++;
				puts("\n");
			}
		}
#endif
	}
	return 0;
}

static void __octeon_dram_clear_access_errors_cn7xxx(int node, bool verbose)
{
	cvmx_l2c_tadx_int_t l2c_tad_int;
	int tad;
	int do_hang = 0;

#ifndef CONFIG_OCTEON_OCX
	node = 0;	/* Optimize out node */
#endif
	debug("%s(%d, %s)\n", __func__, node, verbose ? "verbose" : "brief");
	for (tad = 0; tad < CVMX_L2C_TADS; tad++) {
		l2c_tad_int.u64 = cvmx_read_csr_node(node,
						     CVMX_L2C_TADX_INT(tad));
		/* Default case is no errors */
		if (!l2c_tad_int.u64)
			continue;
		if (OCTEON_IS_MODEL(OCTEON_CN78XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN73XX) ||
		    OCTEON_IS_MODEL(OCTEON_CNF75XX) ||
		    OCTEON_IS_MODEL(OCTEON_CN76XX)) {
#ifdef CONFIG_OCTEON_OCX
			if (node)
				printf("Node %d: ", node);
#endif
			if (l2c_tad_int.s.wrdisoci) {
				printf("L2C_TAD%d_INT.wrdisoci is set.\n", tad);
				puts("This indicates an illegal write to a remote node with L2C_TAD_CTL.enaoci clear\n");
				do_hang = 1;
			}
			if (l2c_tad_int.s.rddisoci) {
				printf("L2C_TAD%d_INT.rddisoci is set.\n", tad);
				puts("This indicates an illegal read to a remote node with L2C_TAD_CTL.enaoci clear\n");
				do_hang = 1;
			}
		}
		if (l2c_tad_int.s.rtgdbe) {
			printf("L2C_TAD%d_INT.rtgdbe is set.\n", tad);
			puts("This indicates that a double bit error has been detected.\n");
			do_hang = 1;
		}
		if (l2c_tad_int.s.rtgsbe) {
			printf("L2C_TAD%d_INT.rtgsbe is set.\n", tad);
			puts("This indicates that a single bit error has been detected.\n"
			     "Please double check the memory settings or consider replacing the bad memory\n");
			mdelay(10000);
		}
		if (l2c_tad_int.cn70xx.wrdislmc) {
			printf("L2C_TAD%d_INT.wrdislmc is set.\n", tad);
			puts("This indicates an illegal write to a disabled LMC.\n");
		}
		if (l2c_tad_int.cn70xx.rddislmc) {
			printf("L2C_TAD%d_INT.rddislmc is set.\n", tad);
			puts("This indicates an illegal read from a disabled LMC.\n");
		}
		if (l2c_tad_int.s.bigrd && verbose) {
			printf("L2C_TAD%d_INT.bigrd is set.\n", tad);
			puts("This inidicates that a read reference past L2C_BIG_CTL.maxdram\n"
			     "has occurred.  This can happen during normal operation and may be beneign.\n");
		}
		if (l2c_tad_int.s.bigwr) {
			printf("L2C_TAD%d_INT.bigwr is set.\n", tad);
			puts("This indicates that a write reference past L2C_BIG_CTL.maxdram\n"
			     "has occurred.\n");
		}
		if (l2c_tad_int.cn70xx.holerd) {
			printf("L2C_TAD%d_INT.holerd is set.\n", tad);
			puts("This indicates that an illegal read to the boot bus has occurred.\n");
		}
		if (l2c_tad_int.cn70xx.holewr) {
			printf("L2C_TAD%d_INT.holewr is set.\n", tad);
			puts("This indicates that an illegal write to the boot bus has occurred.\n");
		}

		if (l2c_tad_int.cn70xx.noway && verbose) {
			printf("L2C_TAD%d_INT.noway is set.\n", tad);
			puts("This indicates that a WAY in the l2 cache could not be allocated and\n"
			     "is a hint of possible performance problems.\n");
		}
		if (l2c_tad_int.cn70xx.tagdbe) {
			printf("L2C_TAD%d_INT.tagdbe is set.\n", tad);
			puts("A TAG double-bit error occurred.\n");
			do_hang = 1;
		}
		if (l2c_tad_int.cn70xx.tagsbe) {
			printf("L2C_TAD%d_INT.tagsbe is set.\n", tad);
			puts("A TAG single-bit occurred.\n");
			mdelay(10000);
		}
		if (l2c_tad_int.cn70xx.fbfdbe) {
			printf("L2C_TAD%d_INT.fbfdbe is set.\n", tad);
			puts("A FBF double bit error has occurred.\n");
			do_hang = 1;
		}
		if (l2c_tad_int.cn70xx.fbfsbe) {
			printf("L2C_TAD%d_INT.fbfsbe is set.\n", tad);
			puts("A FBF single bit error has occurred.\n");
			mdelay(10000);
		}
		if (l2c_tad_int.cn70xx.sbfdbe) {
			printf("L2C_TAD%d_INT.sbfdbe is set.\n", tad);
			puts("A SBF double bit error has occurred.\n");
			do_hang = 1;
		}
		if (l2c_tad_int.cn70xx.sbfsbe) {
			printf("L2C_TAD%d_INT.sbfsbe is set.\n", tad);
			puts("A SBF single bit error has occurred.\n");
			mdelay(10000);
		}
		if (l2c_tad_int.s.l2ddbe) {
			printf("L2C_TAD%d_INT.l2ddbe is set.\n", tad);
			puts("A L2D double-bit error has occurred.\n");
			do_hang = 1;
		}
		if (l2c_tad_int.s.l2dsbe) {
			printf("L2C_TAD%d_INT.l2dsbe is set.\n", tad);
			puts("A L2D single-bit error has occurred.\n");
			mdelay(10000);
		}
		if (verbose) {
			cvmx_l2c_tadx_err_t l2c_tad_err;
			uint64_t addr;

			l2c_tad_err.u64 = cvmx_read_csr_node(node,
							     CVMX_L2C_TADX_ERR(tad));

			addr = l2c_tad_err.s.addr;
			/* adjust address for hole */
			if (addr >= 0x10000000)
				addr += 0x10000000;
#ifdef CONFIG_OCTEON_OCX
			if (OCTEON_IS_MODEL(OCTEON_CN78XX) && node)
				printf("  CPU node:    %d\n", node);
#endif
			printf("  bigrd:       %d\n"
			       "  bigwr:       %d\n"
			       "  holerd:      %d\n"
			       "  holewr:      %d\n"
			       "  XMC command: 0x%x\n"
			       "  source:      0x%x\n"
			       "  node:        %d\n"
			       "  address:     0x%llx\n",
			       l2c_tad_err.s.bigrd,
			       l2c_tad_err.s.bigwr,
			       l2c_tad_err.cn70xx.holerd,
			       l2c_tad_err.cn70xx.holewr,
			       l2c_tad_err.cn70xx.source,
			       l2c_tad_err.s.cmd,
			       l2c_tad_err.s.node,
			       addr);
		}
		debug("%s: Clearing pending interrupts\n", __func__);
		/* Clear pending interrupts */
		if (l2c_tad_int.u64)
			cvmx_write_csr_node(node, CVMX_L2C_TADX_INT(tad),
					    l2c_tad_int.u64);
	}
	if (do_hang) {
		puts("Bootloader halted due to uncorrectable memory errors\n");
		hang();
	}
}

#ifndef CONFIG_OCTEON_CN70XX
static void __octeon_dram_clear_access_errors_cn6xxx(bool verbose)
{
	cvmx_l2c_int_reg_t l2c_int_reg;

	l2c_int_reg.u64 = cvmx_read_csr(CVMX_L2C_INT_REG);
	if (l2c_int_reg.u64 != 0 && verbose)
		printf("L2C_INT_REG is 0x%llx, expected to be zero\n",
		       l2c_int_reg.u64);
	if (l2c_int_reg.s.bigrd && verbose)
		puts("L2C_INT_REG.bigrd is set.  This may be beneign and\n"
		     "indicates that a memory read occurred to an invalid\n"
		     "memory address.  This can be caused during normal\n"
		     "operation or due to a more serious bug.  It may be\n"
		     "caused by either software or hardware DMA beyond\n"
		     "the end of memory.\n");
	if (l2c_int_reg.s.bigwr && verbose)
		puts("L2C_INT_REG.bigwr is set.  This indicates that a\n"
		     "write to an invalid memory location has occurred\n"
		     "either due to the CPU or a DMA access.\n");
	if (l2c_int_reg.s.holerd && verbose)
		puts("L2C_INT_REG.holerd is set.  This indicates that\n"
		     "a read to an invalid address on the boot bus has\n"
		     "occurred (0xB0000000 - 0xBFFFFFFF).\n");
	if (l2c_int_reg.s.holewr && verbose)
		puts("L2C_INT_REG.holewr is set.  This indicates that\n"
		     "a write to an invalid address on the boot bus has\n"
		     "occurred (0xB0000000 - 0xBFFFFFFF).\n");
	if ((l2c_int_reg.s.vrtwr || l2c_int_reg.s.vrtidrng ||
	     l2c_int_reg.s.vrtadrng || l2c_int_reg.s.vrtpe) && verbose)
		puts("L2C_INT_REG.vrtxx bit is set.  This is unexpected!\n");
	if (l2c_int_reg.u64 != 0) {
		if (l2c_int_reg.u64 != 0 && l2c_int_reg.s.bigrd == 0 &&
		    verbose) {
			cvmx_l2c_err_xmc_t l2c_err_xmc;
			uint64_t address;
			l2c_err_xmc.u64 = cvmx_read_csr(CVMX_L2C_ERR_XMC);
			address = l2c_err_xmc.s.addr;
			if (!l2c_int_reg.s.holerd &&
			    !l2c_int_reg.s.holewr &&
			    address >= 0x10000000)
				address += 0x10000000;
			printf("L2C address: 0x%llx, sid: 0x%x, cmd: 0x%x\n",
			       address, l2c_err_xmc.s.sid,
			       l2c_err_xmc.s.cmd);
		}
		cvmx_write_csr(CVMX_L2C_INT_REG, l2c_int_reg.u64);
	}
}
#endif

/**
 * This clears any bits set in the l2c_int_reg before exiting U-Boot.
 *
 * @param verbose	Prints out the status information according to which
 *			bits are set in the CVMX_L2C_INT_REG CSR.
 */
void octeon_dram_clear_access_errors(bool verbose)
{
	debug("%s(%s)\n", __func__, verbose ? "verbose" : "quiet");
#ifdef CONFIG_OCTEON_CN70XX
	__octeon_dram_clear_access_errors_cn7xxx(0, verbose);
#else
	if (OCTEON_IS_OCTEON3()) {
# ifdef CONFIG_OCTEON_OCX
		int node;
		cvmx_coremask_for_each_node(node, gd->arch.node_mask)
			__octeon_dram_clear_access_errors_cn7xxx(node, verbose);
# else
		__octeon_dram_clear_access_errors_cn7xxx(0, verbose);
# endif
	} else if (OCTEON_IS_OCTEON2()) {
		__octeon_dram_clear_access_errors_cn6xxx(verbose);
	}
#endif
}
