/*
 * (C) Copyright 2013 - 2016 Cavium, Inc. <support@cavium.com>
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
 *
 * This file deals with working around various errata.
 */

#include <common.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/cvmx-coremask.h>
#include <asm/arch/cvmx-ciu3-defs.h>
#include <asm/arch/cvmx-rst-defs.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * This function works around various chip errata early in the initialization
 * process.
 *
 * @return 0 for success
 */
int __octeon_early_errata_workaround_f(void)
{
	/* Change default setting for CN58XX pass 2 */
	if (OCTEON_IS_MODEL(OCTEON_CN58XX_PASS2))
		cvmx_write_csr(CVMX_MIO_FUS_EMA, 2);
	return 0;
}

int octeon_early_errata_workaround_f(void)
	__attribute__((weak, alias("__octeon_early_errata_workaround_f")));

/**
 * This function works around various chip errata late in the initialization
 * process.
 *
 * @return 0 for success
 */
int __octeon_late_errata_workaround_f(void)
{
	if (OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X)) {
		/* Disable Fetch under fill on CN63XXp1 due to errata
		 * Core-14317
		 */
		u64 val;
		val = read_64bit_c0_cvmctl();
		val |= (1ull << 19);	/* CvmCtl[DEFET] */
		write_64bit_c0_cvmctl(val);
	}

	/* Apply workaround for Errata L2C-16862 */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX_PASS1_X) ||
	      OCTEON_IS_MODEL(OCTEON_CN68XX_PASS2_0) ||
	      OCTEON_IS_MODEL(OCTEON_CN68XX_PASS2_1)) {
		cvmx_l2c_ctl_t l2c_ctl;
		cvmx_iob_to_cmb_credits_t iob_cmb_credits;
		cvmx_iob1_to_cmb_credits_t iob1_cmb_credits;

		l2c_ctl.u64 = cvmx_read_csr(CVMX_L2C_CTL);

		if (OCTEON_IS_MODEL(OCTEON_CN68XX_PASS1_X))
			l2c_ctl.cn68xxp1.maxlfb = 6;
		else
			l2c_ctl.cn68xx.maxlfb = 6;
		cvmx_write_csr(CVMX_L2C_CTL, l2c_ctl.u64);

		iob_cmb_credits.u64 = cvmx_read_csr(CVMX_IOB_TO_CMB_CREDITS);
		iob_cmb_credits.s.ncb_rd = 6;
		cvmx_write_csr(CVMX_IOB_TO_CMB_CREDITS, iob_cmb_credits.u64);

		iob1_cmb_credits.u64 = cvmx_read_csr(CVMX_IOB1_TO_CMB_CREDITS);
		iob1_cmb_credits.s.pko_rd = 6;
		cvmx_write_csr(CVMX_IOB1_TO_CMB_CREDITS, iob1_cmb_credits.u64);
#if 0
		l2c_ctl.u64 = cvmx_read_csr(CVMX_L2C_CTL);
		iob_cmb_credits.u64 = cvmx_read_csr(CVMX_IOB_TO_CMB_CREDITS);
		iob1_cmb_credits.u64 = cvmx_read_csr(CVMX_IOB1_TO_CMB_CREDITS);
		printf("L2C_CTL[MAXLFB] = %d, IOB_TO_CMB_CREDITS[ncb_rd] = %d, IOB1_TO_CMB_CREDITS[pko_rd] = %d\n",
			l2c_ctl.s.maxlfb, iob_cmb_credits.s.ncb_rd,
			iob1_cmb_credits.s.pko_rd);
#endif
	}

	return 0;
}

int octeon_late_errata_workaround_f(void)
	__attribute__((weak, alias("__octeon_late_errata_workaround_f")));

#ifndef CONFIG_OCTEON_SIM_HW_DIFF
#define MAX_CORE_RESET_ATTEMPTS		5
#define RESET_WAIT_TIMER		65536

static int octeon3_reset_wait_pend(int node, uint64_t cores)
{
	union cvmx_ciu_pp_rst_pending pending;
	int timer = RESET_WAIT_TIMER;

	/* First check if cores are already out of reset, if so then we can
	 * skip this.
	 */
	if ((cores == cvmx_read_csr_node(node, CVMX_CIU_PP_RST)) &&
	    !cvmx_read_csr_node(node, CVMX_CIU_PP_RST_PENDING))
		return 0;

	debug("%s: Writing 0x%llx to node %d PP_RST\n", __func__, cores, node);
	cvmx_write_csr_node(node, CVMX_CIU_PP_RST, cores);
	do {
		cvmx_wait(1);
		pending.u64 = cvmx_read_csr_node(node, CVMX_CIU_PP_RST_PENDING);
	} while (pending.s.pend && (timer-- >= 0));

	return pending.s.pend ? -1 : 0;
}

static int octeon3_all_cores_out_of_reset_wait(int node, uint64_t cores,
					       uint64_t core_mask)
{
	int cores_not_up = 0;
	bool pending_timeout = false;

	volatile uint32_t *memptr_reset =
				CASTPTR(uint32_t,
					BOOTLOADER_DEBUG_CORE_RESET_COUNT);
	volatile uint32_t *memptr_revid =
				CASTPTR(uint32_t,
					BOOTLOADER_DEBUG_CORE_REVID_COUNT);

	debug("%s(%d, 0x%llx, 0x%llx)\n", __func__, node, cores, core_mask);
	while (cores) {
		core_mask = (core_mask << 1) & 0xfffffffffffe;

		cores &= core_mask;

		if (octeon3_reset_wait_pend(node, cores)) {
			puts("Error: timeout waiting for pending out of reset\n");
			cores_not_up++;
			pending_timeout = true;
			break;
		} else {
			/* core is out of reset, wait for it to start code. */
			mdelay(1);
		}
		debug("  %s reset ptr: %p, revid ptr: %p\n", __func__,
		      memptr_reset, memptr_revid);
		debug("  %s reset cnt: %x, revid cnt: %x\n", __func__,
		      *memptr_reset, *memptr_revid);
	}

	debug("%s(%d, 0x%llx, 0x%llx) pending_timeout: %d\n",
	      __func__, node, cores, core_mask, pending_timeout);
	return pending_timeout ? -1 : 0;
}

extern void revid_check_patch(void);

int octeon_enable_all_cores_cn78xx(void)
	__attribute__((weak, alias("__octeon_enable_all_cores_cn78xx")));

int __octeon_enable_all_cores_cn78xx(void)
{
	cvmx_coremask_t coremask = CVMX_COREMASK_EMPTY;
	uint64_t cores[2] = { 0, 0 };
	uint64_t core_mask[2] = { 0xfffffffffffe, 0xffffffffffff };
	uint64_t cores_out_of_reset[2];
	int cores_not_up = 0;
	uint32_t core_count = 0;
	int node;
	struct boot_init_vector *boot_init_vec;
	volatile uint32_t *memptr_reset, *memptr_revid;
	uint32_t reset_count = 0;
	uint32_t revid_count = 0;
	bool pending_timeout = false;
	bool core_reset_failure = true;
	int ret;
	int core;
	bool rst_not_done = false;

	debug("%s() ENTRY\n", __func__);

	boot_init_vec = octeon_get_boot_vector();
	if (!boot_init_vec) {
		printf("%s: Error: could not find boot vector\n", __func__);
		return -1;
	}
	debug("%s: mio_boot_loc_cfg0: 0x%llx\n", __func__,
	      cvmx_read_csr(CVMX_MIO_BOOT_LOC_CFGX(0)));
	debug("%s: rst_pp_power: 0x%llx\n", __func__,
	      cvmx_read_csr(CVMX_RST_PP_POWER));
	debug("%s: ciu_pp_rst: 0x%llx\n", __func__, cvmx_read_csr(CVMX_CIU_PP_RST));
	memptr_reset = CASTPTR(uint32_t, BOOTLOADER_DEBUG_CORE_RESET_COUNT);
	memptr_revid = CASTPTR(uint32_t, BOOTLOADER_DEBUG_CORE_REVID_COUNT);

	debug("  reset ptr: %p, revid ptr: %p\n", memptr_reset, memptr_revid);
	debug("  reset cnt: %x, revid cnt: %x\n", *memptr_reset, *memptr_revid);
	octeon_get_available_coremask(&coremask);

	debug("%s: available coremask:\n", __func__);
#ifdef DEBUG
	cvmx_coremask_print(&coremask);
#endif
	cvmx_coremask_for_each_node(node, gd->arch.node_mask)
		core_mask[node] = cvmx_coremask_get64_node(&coremask, node);

	core_mask[0] &= ~1ull;

	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		rst_not_done |=
			(cvmx_read_csr_node(node, CVMX_CIU_PP_RST) &
							core_mask[node]);
	}

	if (!rst_not_done) {
		debug("%s: cores already out of reset\n", __func__);
		return 0;
	}
	CVMX_SYNCW;

	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		cores[node] = core_mask[node];
		core_count = __builtin_popcountll(cores[node]);

		if (node) {
			/* Force all nodes to use the same memory mapping */
			union cvmx_l2c_oci_ctl oci_ctl;
			oci_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
			oci_ctl.s.gksegnode = 0;
			cvmx_write_csr_node(node, CVMX_L2C_OCI_CTL, oci_ctl.u64);
			cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
		}


		do {
			debug("%s: node: %d attempting reset iteration: %d, cores: 0x%llx\n",
			      __func__, node, cores_not_up, cores[node]);
			cvmx_coremask_for_each_core(core, &coremask) {
				if (core != 0) {
					boot_init_vec[core].code_addr =
						MAKE_XKPHYS(cvmx_ptr_to_phys(&revid_check_patch));
				debug("%s: boot_init_vec[%d].code_addr (%p): 0x%llx\n",
				      __func__, core,
				      &boot_init_vec[core].code_addr,
	  			      boot_init_vec[core].code_addr);
				}
			}
			*memptr_reset = 0;
			*memptr_revid = 0;
			CVMX_SYNCW;
			cvmx_write_csr_node(node, CVMX_RST_PP_POWER, cores[node]);
			cvmx_read_csr_node(node, CVMX_RST_PP_POWER);
			mdelay(1);
			cvmx_write_csr_node(node, CVMX_RST_PP_POWER, 0);
			cvmx_read_csr_node(node, CVMX_RST_PP_POWER);
			mdelay(1);
			cores_out_of_reset[node] = cores[node];

			pending_timeout =
				!!octeon3_all_cores_out_of_reset_wait(node,
						     cores_out_of_reset[node],
						     core_mask[node]);

			reset_count = *memptr_reset;
			revid_count = *memptr_revid;

			if (!pending_timeout) {
				debug("%s: node %d: reset pending clear.  reset: %d, revid: %d, core count: %d\n",
				      __func__, node, reset_count, revid_count,
				      core_count);
				debug("%s: ciu_pp_rst for node %d: 0x%llx\n",
				      __func__, node,
				      cvmx_read_csr_node(node, CVMX_CIU_PP_RST));
				debug("%s: rst_pp_power for node %d: 0x%llx\n",
				      __func__, node,
				      cvmx_read_csr_node(node, CVMX_RST_PP_POWER));
				if ((reset_count == core_count) &&
				    (revid_count == core_count)) {
					cores_not_up =
						MAX_CORE_RESET_ATTEMPTS + 1;
					core_reset_failure = false;
					/* Everything is good */
				} else {
					debug("%s: node %d: not all cores up reset: %d, revid: %d\n",
					      __func__, node,
					      reset_count, revid_count);
					ret = octeon3_reset_wait_pend(node, cores[node]);
					if (ret)
						puts("Error: timeout waiting for pending reset to finish\n");
					cores_not_up++;
				}
			} else {
				debug("%s: node %d: reset pending not clear. reset: %d, revid: %d\n",
				      __func__, node, reset_count, revid_count);
				/* Put cores back in reset */
				ret = octeon3_reset_wait_pend(node, cores[node]);
				if (ret)
					puts("Error: timeout waiting for pending resets to finish\n");
				cores_not_up++;
			}
		} while (cores_not_up < MAX_CORE_RESET_ATTEMPTS);

		if (core_reset_failure) {
			puts("ERROR!!  Could not bring cores up from reset.  Resetting chip and starting over.\n");
			mdelay(100);
			node = (gd->arch.node_mask & 2) ? 1 : 0;
			do {
				cvmx_write_csr_node(node, CVMX_RST_SOFT_RST, 1);
			} while (1);
		}
	}
	debug("%s: Reset successful\n", __func__);

#ifndef DEBUG
	*memptr_reset = 0;
	*memptr_revid = 0;
#endif
	return 0;
}
#endif /* CONFIG_OCTEON_SIM_HW_DIFF */
