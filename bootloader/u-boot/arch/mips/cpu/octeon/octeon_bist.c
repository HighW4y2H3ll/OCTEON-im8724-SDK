/***********************license start************************************
 * Copyright (c) 2004-2015 Cavium, Inc. <support@cavium.com>.  All rights
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
 * marketing@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <linux/ctype.h>
#include <asm/mipsregs.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-l2c.h>
#include <asm/arch/octeon-model.h>
#include <asm/arch/octeon_hal.h>
#include <asm/arch/octeon_boot.h>

struct bist_memctl_cacheerr {
	uint8_t	cop0_memctl;
	uint8_t	cop0_cacheerr_icache;
};

DECLARE_GLOBAL_DATA_PTR;

extern void octeon_led_str_write(const char *str);

extern void bist_read_memctl_cacheerr(void);
extern void bist_read_memctl_cacheerr_end(void);

/**
 * Enable all cores
 *
 * This function powers up all cores (if needed) and takes them out of
 * reset.  For OCX configurations it also sets it so all nodes share the
 * same memory mapping.
 */
static int octeon_bist_check_iob(void)
{
#ifndef CONFIG_OCTEON_SIM_HW_DIFF
	union cvmx_rst_boot rst_boot;
	int core;
	struct cvmx_coremask coremask = CVMX_COREMASK_EMPTY;
	uint64_t cop0_read_func[16];
	const int func_size =
		&bist_read_memctl_cacheerr_end - &bist_read_memctl_cacheerr;
	const int num_dwords = (func_size + 7) / 8;
	volatile struct bist_memctl_cacheerr
				memctl_cacheerr_buf[CVMX_MIPS_MAX_CORES];
	uint64_t buf_addr =
		MAKE_XKPHYS(cvmx_ptr_to_phys2((void *)memctl_cacheerr_buf));
	uint64_t bist_val;
	uint64_t cop0_cvmmemctl;
	uint64_t cop0_cacheerr_icache;
	int node;
	bool failure = false;

	debug("%s\n", __func__);

	if (OCTEON_IS_MODEL(OCTEON_CN70XX)) {
		/* For CN70XX we don't have access to the L2 cache to share
		 * with other cores, so we ignore errors if this is not the
		 * first boot.
		 *
		 * We ignore cold resets and warm resets but if reset for any
		 * other reason we don't check the BIST results.
		 */
		rst_boot.u64 = cvmx_read_csr(CVMX_RST_BOOT);
		bist_val = cvmx_read_csr(CVMX_IOB_PP_BIST_STATUS);
		debug("%s: CN70XX detected.  rst_boot lboot_oci: 0x%x, lboot_ext: 0x%x, lboot: 0x%x, bist_val: 0x%llx\n",
		      __func__, rst_boot.s.lboot_oci, rst_boot.s.lboot_ext,
		      rst_boot.s.lboot, bist_val);
		return  (!(rst_boot.s.lboot_oci || rst_boot.s.lboot_ext ||
			  (rst_boot.s.lboot & ~3)));
	}
	memset((void *)memctl_cacheerr_buf, 0xff, sizeof(memctl_cacheerr_buf));
	memset(cop0_read_func, 0, sizeof(cop0_read_func));
	memcpy(cop0_read_func, &bist_read_memctl_cacheerr, func_size);

	/* Program the xkphys address of the buffer into the code */
	cop0_read_func[0] |= ((buf_addr >> 48) & 0xffff) << 32;
	cop0_read_func[0] |= ((buf_addr >> 32) & 0xffff);
	cop0_read_func[1] |= ((buf_addr >> 16) & 0xffff) << 32;
	cop0_read_func[1] |= (buf_addr & 0xffff);
	debug("COP0 read function copied %u bytes (%d dwords) from %p to %p, code0: 0x%llx, code1: 0x%llx, buf addr: 0x%llx\n",
	      func_size, num_dwords, &bist_read_memctl_cacheerr, cop0_read_func,
	      cop0_read_func[0], cop0_read_func[1], buf_addr);

	debug("%s: Programming boot bus moveable region\n", __func__);
	octeon_set_moveable_region(0x1fc00000, 0, true, cop0_read_func,
				   num_dwords);

	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		debug("Powering down cores on node %d\n", node);
		cvmx_write_csr_node(node, CVMX_CIU_PP_RST, node ? -1 : ~1);
		udelay(100);
		cvmx_write_csr_node(node, CVMX_RST_PP_POWER, node ? -1 : ~1);
	}
	mdelay(1);
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		debug("Powering up cores on node %d\n", node);
		/* Power up all the cores before taking them
		 * out of reset, put them in reset first
		 */
		cvmx_write_csr_node(node, CVMX_RST_PP_POWER, 0);
	}
	mdelay(1);
	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		/* Force all nodes to use the same memory
		 * mapping                                                             *
		 */
		union cvmx_l2c_oci_ctl oci_ctl;
		if (node == 0)
			continue;

		debug("%s: Taking node %d out of reset\n", __func__, node);
		oci_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
		oci_ctl.s.gksegnode = 0;
		cvmx_write_csr_node(node, CVMX_L2C_OCI_CTL, oci_ctl.u64);
		cvmx_read_csr_node(node, CVMX_L2C_OCI_CTL);
		CVMX_SYNCW;
	}

	/* Get the values from core 0 */
	CVMX_MF_CVM_MEM_CTL(cop0_cvmmemctl);
	CVMX_MF_CACHE_ERR(cop0_cacheerr_icache);

	memctl_cacheerr_buf[0].cop0_memctl = (cop0_cvmmemctl >> 56) & 0xff;
	memctl_cacheerr_buf[0].cop0_cacheerr_icache =
				(cop0_cacheerr_icache >> 32) & 0x3f;
	CVMX_SYNCW;

	mdelay(1);
	octeon_get_available_coremask(&coremask);

	/* Take the cores out of reset one by one so that we don't get cache
	 * collisions writing to the cache error buffer.
	 */
	cvmx_coremask_for_each_core(core, &coremask) {
		union cvmx_ciu_pp_rst pp_rst;
		node = cvmx_coremask_core_to_node(core);
		debug("%s: Taking core %d out of reset\n", __func__, core);
		pp_rst.u64 = cvmx_read_csr_node(node, CVMX_CIU_PP_RST);
		pp_rst.u64 &= ~(1 << (core % CVMX_COREMASK_MAX_CORES_PER_NODE));
		cvmx_write_csr_node(node, CVMX_CIU_PP_RST, pp_rst.u64);
		mdelay(1);
	}

	cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
		if (cvmx_read_csr_node(node, CVMX_CIU_PP_RST))
			printf("Not all cores came out of reset on node %d\n",
			       node);
	}

	CVMX_SYNC;
	cvmx_coremask_for_each_core(core, &coremask) {
		if (memctl_cacheerr_buf[core].cop0_memctl ||
		    memctl_cacheerr_buf[core].cop0_cacheerr_icache) {
			printf("%s: core: %d: memctl: 0x%x, cacheerr: 0x%x\n",
			       __func__, core,
			       memctl_cacheerr_buf[core].cop0_memctl,
			       memctl_cacheerr_buf[core].cop0_cacheerr_icache);
			failure = true;
		}
	}

	/* Put all cores back in reset */
	cvmx_coremask_for_each_node(node, gd->arch.node_mask)
		cvmx_write_csr_node(node, CVMX_CIU_PP_RST, node ? -1 : ~1);

	mdelay(1);
	/* Power them all down */
	cvmx_coremask_for_each_node(node, gd->arch.node_mask)
		cvmx_write_csr_node(node, CVMX_RST_PP_POWER, node ? -1 : ~1);

	/* Disable boot bus moveable region */
	octeon_set_moveable_region(0, 0, false, NULL, 0);

	if (failure)
		return -1;
#endif
	return 0;
}


static int displayErrorReg_1(const char *reg_name, uint64_t addr,
			     uint64_t expected, uint64_t mask)
{
	uint64_t bist_val;
	bist_val = cvmx_read_csr(addr);
	bist_val = (bist_val & mask) ^ expected;
	if (bist_val) {
		printf("BIST FAILURE: %s, error bits "
		       "((bist_val & mask) ^ expected): 0x%llx\n",
		       reg_name, bist_val);
		return 1;
	}
	return 0;
}

#ifdef CONFIG_OCTEON_OCX
static int displayErrorReg_1_node(int node, const char *reg_name, uint64_t addr,
				  uint64_t expected, uint64_t mask)
{
	uint64_t bist_val;
	bist_val = cvmx_read_csr_node(node, addr);
	bist_val = (bist_val & mask) ^ expected;
	if (bist_val) {
		printf("BIST FAILURE: %s, error bits "
		       "((bist_val & mask) ^ expected): 0x%llx\n",
		       reg_name, bist_val);
		return 1;
	}
	return 0;
}
#else
static inline int displayErrorReg_1_node(int node, const char *reg_name,
					 uint64_t addr, uint64_t expected,
					 uint64_t mask)
{
	return displayErrorReg_1(reg_name, addr, expected, mask);
}
#endif

static int displayErrorRegC0_cvmmem(const char *reg_name, uint64_t expected,
				    uint64_t mask)
{
	uint64_t bist_val;

	bist_val = read_64bit_c0_cvmmemctl();

	bist_val = (bist_val & mask) ^ expected;
	if (bist_val) {
		printf("BIST FAILURE: %s, error bits: 0x%llx\n", reg_name,
		       bist_val);
		return (1);
	}
	return (0);
}

static int displayErrorRegC0_Icache(const char *reg_name, uint64_t expected,
				    uint64_t mask)
{
	uint64_t bist_val;

	bist_val = read_64bit_c0_cacheerr();

	bist_val = (bist_val & mask) ^ expected;
	if (bist_val) {
		printf("BIST FAILURE: %s, error bits: 0x%llx\n", reg_name,
		       bist_val);
		return (1);
	}
	return (0);
}

/* We want a mask that is all 1's in bit positions not covered
 * by the coremask once it is shifted.
 */
static inline uint64_t make_cm_mask(uint64_t cm, int shift)
{
	uint64_t cm_override_mask;
	if (OCTEON_IS_MODEL(OCTEON_CN63XX))
		cm_override_mask = 0x3f;
	else if (OCTEON_IS_MODEL(OCTEON_CN61XX)
		 || OCTEON_IS_MODEL(OCTEON_CNF71XX)
		 || OCTEON_IS_MODEL(OCTEON_CN70XX))
		cm_override_mask = 0xf;
	else if (OCTEON_IS_MODEL(OCTEON_CN66XX))
		cm_override_mask = 0x3ff;
	else if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		cm_override_mask = 0xffffffff;
	else {
		puts("ERROR: unsupported OCTEON model\n");
		return ~0ull;
	}

	return ((~0ull & ~(cm_override_mask << shift)) | (cm << shift));
}

/**
 * This function starts the BIST test
 */
int octeon_bist_start(void)
{
	/* Enable soft BIST so that BIST is run on soft resets. */
	if (!OCTEON_IS_MODEL(OCTEON_CN38XX_PASS2) &&
	    !OCTEON_IS_MODEL(OCTEON_CN31XX))
		cvmx_write_csr(CVMX_CIU_SOFT_BIST, 1);
	return 0;
}

static uint64_t convert_ciu_fuse_to_coremask_cn78xx(uint64_t ciu_fuse)
{
	uint64_t fuse = 0ull;
	int max_cores = 48;
	int i;

	for (i = 0; i < max_cores; i++) {
		uint64_t value;
		value = ((ciu_fuse >> i) & 0x1);

		/* Take care of even cores */
		if (i < (max_cores / 2)) {
			fuse |= value << (i*2);
		} else {
			/* Take care of odd cores */
			uint64_t bit = i - (max_cores / 2);
			fuse |= value << ((bit * 2) + 1);
		}
	}
	return fuse;
}

int octeon_bist_7XXX(int node)
{
	int bist_failures = 0;
	int i;
	int count = 0;
	int dclk = 0, tclk = 0, vclk = 0;
	cvmx_ciu_fuse_t fuse;
	uint64_t ciu_fuse;
	uint64_t bist_val;
	int iob_bist_status;

#ifndef CONFIG_OCTEON_OCX
	node = 0;	/* Optimize compiler */
#endif

	fuse.u64 = cvmx_read_csr_node(node, CVMX_CIU_FUSE);
	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		ciu_fuse = convert_ciu_fuse_to_coremask_cn78xx(fuse.u64);
	else
		ciu_fuse = fuse.u64;

	bist_failures +=
	displayErrorRegC0_cvmmem("COP0_CVMMEMCTL_REG", 0ull,
				 0xff00000000000000ull);

	bist_failures +=
	displayErrorRegC0_Icache("COP0_ICACHEERR_REG",
				 0,
			  0x35ull << 32);

	/* Set this register to remove clock gating, required for accessing
	   DENC, TDEC, VDEC blocks. */
	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		cvmx_mhbwx_cfg_t mhbwx_cfg;
		// 9 = DENC; 12 = TDEC; 14 = VDEC
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(9));
		dclk = mhbwx_cfg.s.ab_clk_gating_ena;
		mhbwx_cfg.s.ab_clk_gating_ena = 0;
		cvmx_write_csr(CVMX_MHBWX_CFG(9), mhbwx_cfg.u64);
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(12));
		tclk = mhbwx_cfg.s.ab_clk_gating_ena;
		mhbwx_cfg.s.ab_clk_gating_ena = 0;
		cvmx_write_csr(CVMX_MHBWX_CFG(12), mhbwx_cfg.u64);
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(14));
		vclk = mhbwx_cfg.s.ab_clk_gating_ena;
		mhbwx_cfg.s.ab_clk_gating_ena = 0;
		cvmx_write_csr(CVMX_MHBWX_CFG(14), mhbwx_cfg.u64);
		for (i = 0; i < 20; i++) {
			int j;
			bist_failures +=
			displayErrorReg_1_node(node, "CVMX_MHBWX_MEM_BIST_STATUS0",
						CVMX_MHBWX_MEM_BIST_STATUS0(i), 0, ~0ull);
			bist_failures +=
			displayErrorReg_1_node(node, "CVMX_MHBWX_MEM_BIST_STATUS1",
						CVMX_MHBWX_MEM_BIST_STATUS1(i), 0, ~0ull);
			for (j = 0; j < 8; j++) {
				bist_failures +=
				displayErrorReg_1_node(node, "CVMX_MHBWX_EXTX_MEM_BIST_STATUS",
						CVMX_MHBWX_EXTX_MEM_BIST_STATUS(j, i), 0, ~0ull);
			}
		}
	}

	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		count = 6;
		bist_failures +=
		displayErrorReg_1_node(node, "CVMX_ASE_BIST_STATUS0",
					CVMX_ASE_BIST_STATUS0, 0, ~0ull);
		bist_failures +=
		displayErrorReg_1_node(node, "CVMX_ASE_BIST_STATUS1",
					CVMX_ASE_BIST_STATUS1, 0, ~0ull);
	} else if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		count = 3;
		bist_failures +=
	    	displayErrorReg_1("CVMX_BCH_BIST_RESULT", CVMX_BCH_BIST_RESULT,
				  0, ~0ull);
	} else if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		count = 1;
		bist_failures +=
	    	displayErrorReg_1("CVMX_BCH_BIST_RESULT", CVMX_BCH_BIST_RESULT,
				  0, ~0ull);
	}
	for (i = 0; i < count; i++) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_BGXX_CMR_BIST_STATUS",
					       CVMX_BGXX_CMR_BIST_STATUS(i),
					       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_BGXX_SPU_BIST_STATUS",
					       CVMX_BGXX_SPU_BIST_STATUS(i),
					       0, ~0ull);
	}
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_CIU3_BIST", CVMX_CIU3_BIST,
				       0, ~0ull);
	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		for (i = 0; i < 2; i++) {
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_DENCX_BIST_STATUS",
					       CVMX_DENCX_BIST_STATUS(i),
					       0, ~0ull);
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_FDEQX_BIST_STATUS0",
					       CVMX_FDEQX_BIST_STATUS0(i),
					       0, ~0ull);
		}
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_DLFE_BIST_STATUS",
					       CVMX_DLFE_BIST_STATUS,
					       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_DLFE_BIST_STATUS1",
					       CVMX_DLFE_BIST_STATUS1,
					       0, ~0ull);
	}

	if (!OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_DFA_BIST0", CVMX_DFA_BIST0,
				       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_DFA_BIST1", CVMX_DFA_BIST1,
				       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_HNA_BIST0",
				       CVMX_HNA_BIST0, 0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_HNA_BIST1", CVMX_HNA_BIST1,
				       0, ~0ull);
	}
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_DPI_BIST_STATUS",
				       CVMX_DPI_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_FPA_BIST_STATUS",
				       CVMX_FPA_BIST_STATUS, 0, ~0ull);
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_ILK_BIST_SUM",
					       CVMX_ILK_BIST_SUM, 0, ~0ull);
	}

	iob_bist_status = octeon_bist_check_iob();

	bist_failures += displayErrorReg_1_node(node, "CVMX_IOBN_BIST_STATUS",
						CVMX_IOBN_BIST_STATUS, 0, ~0ull);
	bist_val = cvmx_read_csr_node(node, CVMX_IOBN_PP_BIST_STATUS);
	bist_val &= ciu_fuse;
	if (bist_val && iob_bist_status)
		bist_failures += displayErrorReg_1_node(node,
							"CVMX_IOBN_PP_BIST_STATUS",
							CVMX_IOBN_PP_BIST_STATUS,
							0, ciu_fuse);
	bist_failures += displayErrorReg_1_node(node, "CVMX_IOBP_BIST_STATUS",
						CVMX_IOBP_BIST_STATUS, 0, ~0ull);
	bist_val = cvmx_read_csr_node(node, CVMX_IOBP_PP_BIST_STATUS);
	if (bist_val && iob_bist_status)
		bist_failures += displayErrorReg_1_node(node,
							"CVMX_IOBP_PP_BIST_STATUS",
							CVMX_IOBP_PP_BIST_STATUS,
							0, ciu_fuse);

	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_KEY_BIST_REG",
				       CVMX_KEY_BIST_REG, 0, ~0ull);
	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		count = 4;
	else
		count = 2;
	for (i = 0; i < count; i++) {
		bist_failures +=
			displayErrorReg_1_node(node,
					       "CVMX_L2C_CBCX_BIST_STATUS(i)",
					       CVMX_L2C_CBCX_BIST_STATUS(i),
					       0, ~0ull);
	}
	if (OCTEON_IS_MODEL(OCTEON_CN78XX))
		count = 8;
	else
		count = 4;
	for (i = 0; i < count; i++) {
		bist_failures +=
			displayErrorReg_1_node(node,
					       "CVMX_L2C_TBFX_BIST_STATUS(i)",
					       CVMX_L2C_TBFX_BIST_STATUS(i),
					       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node,
					       "CVMX_L2C_TDTX_BIST_STATUS(i)",
					       CVMX_L2C_TDTX_BIST_STATUS(i),
					       0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node,
					       "CVMX_L2C_TTGX_BIST_STATUS(i)",
					       CVMX_L2C_TTGX_BIST_STATUS(i),
					       0, ~0ull);
	}
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_LBK_BIST_RESULT",
				       CVMX_LBK_BIST_RESULT, 0, ~0ull);

	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_MIO_BOOT_BIST_STAT",
				       CVMX_MIO_BOOT_BIST_STAT, 0, ~0ull);
	for (i = 0; i < 2; i++) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_MIXX_BIST(i)",
					       CVMX_MIXX_BIST(i), 0, ~0ull);
	}
	/* Add Bist status for NQM block */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		for (i = 0; i < 4; i++) {
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_PEMX_BIST_STATUS(i)",
					       CVMX_PEMX_BIST_STATUS(i),
					       0, ~0ull);
		}
	}
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKI_BIST_STATUS0",
				       CVMX_PKI_BIST_STATUS0, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKI_BIST_STATUS1",
				       CVMX_PKI_BIST_STATUS1, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKI_BIST_STATUS2",
				       CVMX_PKI_BIST_STATUS2, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_LUT_BIST_STATUS",
				       CVMX_PKO_NCB_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_NCB_BIST_STATUS",
				       CVMX_PKO_NCB_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PDM_BIST_STATUS",
				       CVMX_PKO_PDM_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PEB_BIST_STATUS",
				       CVMX_PKO_PEB_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PSE_DQ_BIST_STATUS",
				       CVMX_PKO_PSE_DQ_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PSE_PQ_BIST_STATUS",
				       CVMX_PKO_PSE_PQ_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PSE_SQ1_BIST_STATUS",
				       CVMX_PKO_PSE_SQ1_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PSE_SQ2_BIST_STATUS",
				       CVMX_PKO_PSE_SQ2_BIST_STATUS, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PKO_PSE_SQ3_BIST_STATUS",
				       CVMX_PKO_PSE_SQ3_BIST_STATUS, 0, ~0ull);
	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_PKO_PSE_SQ4_BIST_STATUS",
					       CVMX_PKO_PSE_SQ4_BIST_STATUS, 0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_PKO_PSE_SQ5_BIST_STATUS",
					       CVMX_PKO_PSE_SQ5_BIST_STATUS, 0, ~0ull);
	}
	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		for (i = 0; i < 2; i++) {
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_PNBX_BIST_STATUS",
					       CVMX_PNBX_BIST_STATUS(i), 0, ~0ull);
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_RDECX_BIST_STATUS",
					       CVMX_RDECX_BIST_STATUS(i), 0, ~0ull);
		}
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_PSM_BIST_STATUS",
					CVMX_PSM_BIST_STATUS, 0, ~0ull);
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_RFIF_BIST_STATUS",
					CVMX_RFIF_BIST_STATUS, 0x1fff0000, ~0ull);

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_RMAP_BIST_STATUS",
					CVMX_RMAP_BIST_STATUS, 0, ~0ull);
	}

	if (!OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_RAD_REG_BIST_RESULT",
				       CVMX_RAD_REG_BIST_RESULT, 0, ~0ull);
	}

	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_RNM_BIST_STATUS",
				       CVMX_RNM_BIST_STATUS, 0, ~0ull);
#if 1
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_PEXP_SLI_BIST_STATUS",
				       CVMX_PEXP_SLI_BIST_STATUS, 0, ~0ull);
#endif
	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		for (i = 0; i < 2; i++) {
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_SRIOX_BIST_STATUS",
					       CVMX_SRIOX_BIST_STATUS(i), 0, ~0ull);
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_TDECX_BIST_STATUS0",
					       CVMX_TDECX_BIST_STATUS0(i), 0, ~0ull);
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_TDECX_BIST_STATUS1",
					       CVMX_TDECX_BIST_STATUS1(i), 0, ~0ull);
		}
	}

	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_SSO_BIST_STATUS0",
				       CVMX_SSO_BIST_STATUS0, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_SSO_BIST_STATUS1",
				       CVMX_SSO_BIST_STATUS1, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_SSO_BIST_STATUS2",
				       CVMX_SSO_BIST_STATUS2, 0, ~0ull);
	bist_failures +=
		displayErrorReg_1_node(node, "CVMX_TIM_BIST_RESULT",
				       CVMX_TIM_BIST_RESULT, 0, ~0ull);

	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_ULFE_BIST_STATUS0",
					CVMX_ULFE_BIST_STATUS0, 0, ~0ull);

		for (i = 0; i < 2; i++) {
			bist_failures +=
				displayErrorReg_1_node(node, "CVMX_VDECX_BIST_STATUS",
					CVMX_VDECX_BIST_STATUS(i), 0, ~0ull);
		}

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_WPSE_BIST_STATUS",
					CVMX_WPSE_BIST_STATUS, 0, ~0ull);

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_WRCE_BIST_STATUS",
					CVMX_WRCE_BIST_STATUS, 0, ~0ull);

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_WRDE_BIST_STATUS",
					CVMX_WRDE_BIST_STATUS, 0, ~0ull);

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_WRSE_BIST_STATUS",
					CVMX_WRSE_BIST_STATUS, 0, ~0ull);

		bist_failures +=
			displayErrorReg_1_node(node, "CVMX_WTXE_BIST_STATUS",
					CVMX_WTXE_BIST_STATUS, 0, ~0ull);
	}

	if (OCTEON_IS_MODEL(OCTEON_CNF75XX)) {
		cvmx_mhbwx_cfg_t mhbwx_cfg;
		// 9 = DENC; 12 = TDEC; 14 = VDEC
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(9));
		mhbwx_cfg.s.ab_clk_gating_ena = dclk;
		cvmx_write_csr(CVMX_MHBWX_CFG(9), mhbwx_cfg.u64);
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(12));
		mhbwx_cfg.s.ab_clk_gating_ena = tclk;
		cvmx_write_csr(CVMX_MHBWX_CFG(12), mhbwx_cfg.u64);
		mhbwx_cfg.u64 = cvmx_read_csr(CVMX_MHBWX_CFG(14));
		mhbwx_cfg.s.ab_clk_gating_ena = vclk;
		cvmx_write_csr(CVMX_MHBWX_CFG(14), mhbwx_cfg.u64);
	}

	return 0;
}

int octeon_bist_usb(int cnt, int clear_bist)
{
	int bist_failures = 0;
	uint64_t reg_val;

	reg_val = cvmx_read_csr(CVMX_USBDRDX_UCTL_CTL(cnt));
	if (clear_bist) {
		reg_val |= (1ULL << 63); //Set CLEAR_BIST bit
	}
	else {
		reg_val &= (~(1ULL << 63));//Clear CLEAR_BIST bit
	}
	cvmx_write_csr(CVMX_USBDRDX_UCTL_CTL(cnt), reg_val);

	reg_val = cvmx_read_csr(CVMX_USBDRDX_UCTL_CTL(cnt));
	reg_val |= (1ULL << 62); //SET START_BIST
	reg_val |= 7ULL; //SET UPHY_RST, UAHC_RST and UCTL_RST
	cvmx_write_csr(CVMX_USBDRDX_UCTL_CTL(cnt), reg_val);

	udelay(10000); //Wait 10 ms

	bist_failures +=
		displayErrorReg_1("CVMX_USBDRDX_UCTL_BIST_STATUS(i)",
				CVMX_USBDRDX_UCTL_BIST_STATUS(cnt), 0, ~0ull);

	reg_val = cvmx_read_csr(CVMX_USBDRDX_UCTL_CTL(cnt));
	if (clear_bist) {
		reg_val &= (~(1ULL << 63)); //Clear CLEAR_BIST
	}
	reg_val &= (~(1ULL << 62)); //Clear START_BIST
	reg_val &= (~(7ULL)); //Clear UPHY_RST, UAHC_RST and UCTL_RST
	cvmx_write_csr(CVMX_USBDRDX_UCTL_CTL(cnt), reg_val);

	return 0;
}

int octeon_bist_70XX(void)
{
	uint64_t bist_val;
	int bist_failures = 0;
	int i;

	bist_failures +=
	    displayErrorRegC0_cvmmem("COP0_CVMMEMCTL_REG", 0ull,
				     0xff00000000000000ull);

	bist_failures +=
	    displayErrorRegC0_Icache("COP0_ICACHEERR_REG",
				     0,
				     0x35ull << 32);

	bist_failures +=
	    displayErrorReg_1("CVMX_AGL_GMX_BIST", CVMX_AGL_GMX_BIST, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_BCH_BIST_RESULT", CVMX_BCH_BIST_RESULT,
			      0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_CIU_BIST", CVMX_CIU_BIST, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_DFA_BIST0", CVMX_DFA_BIST0, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_DFA_BIST1", CVMX_DFA_BIST1, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_DPI_BIST_STATUS", CVMX_DPI_BIST_STATUS,
			      0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_FPA_BIST_STATUS", CVMX_FPA_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_IPD_BIST_STATUS", CVMX_IPD_BIST_STATUS, 0,
			      ~0ull);
	for (i = 0; i < 2; i++) {
		bist_failures +=
	 		displayErrorReg_1("CVMX_GMXX_BIST(i)", CVMX_GMXX_BIST(i),
					0, ~0ull);
	}
	bist_failures +=
		displayErrorReg_1("CVMX_PCSXX_BIST_STATUS_REG(i)",
				CVMX_PCSXX_BIST_STATUS_REG(0), 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_IOB_BIST_STATUS", CVMX_IOB_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_IPD_BIST_STATUS", CVMX_IPD_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_KEY_BIST_REG", CVMX_KEY_BIST_REG, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_L2C_CBCX_BIST_STATUS(0)",
				CVMX_L2C_CBCX_BIST_STATUS(0), 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_L2C_TBFX_BIST_STATUS(0)",
				CVMX_L2C_TBFX_BIST_STATUS(0), 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_L2C_TDTX_BIST_STATUS(0)",
				CVMX_L2C_TDTX_BIST_STATUS(0), 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_L2C_TTGX_BIST_STATUS(0)",
				CVMX_L2C_TTGX_BIST_STATUS(0), 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_MIO_BOOT_BIST_STAT",
				CVMX_MIO_BOOT_BIST_STAT, 0, ~0ull);
	for (i = 0; i < 3; i++) {
		if (OCTEON_IS_MODEL(OCTEON_CN70XX_PASS1_X))
			break;
		bist_failures +=
	 		displayErrorReg_1("CVMX_PEMX_BIST_STATUS(i)",
				CVMX_PEMX_BIST_STATUS(i), 0, ~0ull);
		bist_failures +=
	 		displayErrorReg_1("CVMX_PEMX_BIST_STATUS2(i)",
				CVMX_PEMX_BIST_STATUS2(i), 0, ~0ull);
	}
	bist_failures +=
	    displayErrorReg_1("CVMX_PIP_BIST_STATUS", CVMX_PIP_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_PKO_REG_BIST_RESULT",
				CVMX_PKO_REG_BIST_RESULT, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_RAD_REG_BIST_RESULT",
				CVMX_RAD_REG_BIST_RESULT, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_RNM_BIST_STATUS", CVMX_RNM_BIST_STATUS, 0,
			      ~0ull);
	if (!OCTEON_IS_MODEL(OCTEON_CN70XX_PASS1_0))
		bist_failures +=
	    		displayErrorReg_1("CVMX_PEXP_SLI_BIST_STATUS",
					  CVMX_PEXP_SLI_BIST_STATUS, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_POW_BIST_STAT", CVMX_POW_BIST_STAT, 0,
			      ~0ull);

	bist_val = cvmx_read_csr(CVMX_IOB_PP_BIST_STATUS);
	if (bist_val && octeon_bist_check_iob())
		bist_failures +=
			displayErrorReg_1("CVMX_IOB_PP_BIST_STATUS",
					  CVMX_IOB_PP_BIST_STATUS, 0, ~0ull);
	return 0;
}

int octeon_bist_6XXX_usb(void)
{
	int bist_failures = 0;
	cvmx_uctlx_if_ena_t usb_if_en;
	usb_if_en.u64 = cvmx_read_csr(CVMX_UCTLX_IF_ENA(0));
	usb_if_en.s.en = 1;
	cvmx_write_csr(CVMX_UCTLX_IF_ENA(0), usb_if_en.u64);
	cvmx_read_csr(CVMX_UCTLX_IF_ENA(0));
	cvmx_wait(1000000);

	bist_failures +=
	    displayErrorReg_1("CVMX_UCTLX_BIST_STATUS",
				      CVMX_UCTLX_BIST_STATUS(0), 0, ~0ull);
	return bist_failures;
}

int octeon_bist_6XXX(void)
{
	/* Mask POW BIST_STAT with coremask so we don't report errors on known
	 * bad cores
	 */
	char *cm_str = getenv("coremask_override");
	uint64_t cm_override = ~0ull;
	int bist_failures = 0;
	int tad = 0;

	bist_failures +=
	    displayErrorRegC0_cvmmem("COP0_CVMMEMCTL_REG", 0ull,
				     0xfc00000000000000ull);

	bist_failures +=
	    displayErrorRegC0_Icache("COP0_ICACHEERR_REG",
				     0,
				     0x37ull << 32);
	if (cm_str)
		cm_override = simple_strtoul(cm_str, NULL, 0);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		bist_failures +=
		    displayErrorReg_1("SSO_BIST_STAT", CVMX_SSO_BIST_STAT,
				      0ull, make_cm_mask(cm_override, 16));
	else
		bist_failures +=
		    displayErrorReg_1("POW_BIST_STAT", CVMX_POW_BIST_STAT,
				      0ull, make_cm_mask(cm_override, 16));

	if (!OCTEON_IS_MODEL(OCTEON_CNF71XX))
        {
        	bist_failures +=
                    displayErrorReg_1("CVMX_AGL_GMX_BIST", CVMX_AGL_GMX_BIST, 0, ~0ull);
                bist_failures +=
                    displayErrorReg_1("CVMX_MIXX_BIST0", CVMX_MIXX_BIST(0), 0, ~0ull);
                if (!OCTEON_IS_MODEL(OCTEON_CN68XX))
                        bist_failures += displayErrorReg_1("CVMX_MIXX_BIST1",
                                                           CVMX_MIXX_BIST(1), 0, ~0ull);
        }
	bist_failures +=
	    displayErrorReg_1("CVMX_CIU_BIST", CVMX_CIU_BIST, 0, ~0ull);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		bist_failures +=
		    displayErrorReg_1("CVMX_CIU_PP_BIST_STAT",
				      CVMX_CIU_PP_BIST_STAT, 0, ~0ull);

	if (!OCTEON_IS_MODEL(OCTEON_CNF71XX))
        {
            bist_failures +=
                displayErrorReg_1("CVMX_DFA_BIST0", CVMX_DFA_BIST0, 0, ~0ull);
            bist_failures +=
                displayErrorReg_1("CVMX_DFA_BIST1", CVMX_DFA_BIST1, 0, ~0ull);
        }
	bist_failures +=
	    displayErrorReg_1("CVMX_DPI_BIST_STATUS", CVMX_DPI_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_FPA_BIST_STATUS", CVMX_FPA_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_GMXX_BIST0", CVMX_GMXX_BIST(0), 0, ~0ull);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		bist_failures += displayErrorReg_1("CVMX_GMXX_BIST1",
						   CVMX_GMXX_BIST(1), 0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_GMXX_BIST2",
						   CVMX_GMXX_BIST(2), 0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_GMXX_BIST3",
						   CVMX_GMXX_BIST(3), 0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_ILK_BIST_SUM",
						   CVMX_ILK_BIST_SUM, 0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_TIM_BIST_RESULT",
						   CVMX_TIM_BIST_RESULT, 0, ~0ull);
	}
	else
		bist_failures += displayErrorReg_1("CVMX_TIM_REG_BIST_RESULT",
					   	CVMX_TIM_REG_BIST_RESULT, 0, ~0ull);

	bist_failures +=
	    displayErrorReg_1("CVMX_IOB_BIST_STATUS", CVMX_IOB_BIST_STATUS, 0,
			      ~0ull);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		bist_failures += displayErrorReg_1("CVMX_IOB1_BIST_STATUS",
						   CVMX_IOB1_BIST_STATUS,
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_ZIP_CTL_BIST_STATUS",
						   CVMX_ZIP_CTL_BIST_STATUS,
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_ZIP_CORE0_BIST_STATUS",
						   CVMX_ZIP_COREX_BIST_STATUS(0),
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_ZIP_CORE1_BIST_STATUS",
						   CVMX_ZIP_COREX_BIST_STATUS(1),
						   0, ~0ull);
	}
	bist_failures +=
	    displayErrorReg_1("CVMX_IPD_BIST_STATUS", CVMX_IPD_BIST_STATUS, 0,
			      ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_KEY_BIST_REG", CVMX_KEY_BIST_REG, 0, ~0ull);
	bist_failures +=
	    displayErrorReg_1("CVMX_MIO_BOOT_BIST_STAT",
			      CVMX_MIO_BOOT_BIST_STAT, 0, ~0ull);

	if (!OCTEON_IS_MODEL(OCTEON_CN63XX_PASS1_X)) {
		int i;
		cvmx_mio_rst_ctlx_t mio_rst;
		/* Only some of the PCIe BIST can be checked before the ports
		 * are configured.  Here we only check to see if they are
		 * configured, but do not configure them.  PCIe BIST is also
		 * checked by the cvmx_pcie_initialize() function, so PCIe
		 * BIST is checked if used.
		 */

                if (!OCTEON_IS_MODEL(OCTEON_CNF71XX))
			bist_failures += displayErrorReg_1("CVMX_PCSXX_BIST_STATUS_REG",
							   CVMX_PCSXX_BIST_STATUS_REG
							   (0), 0, ~0ull);
		for (i = 0; i < 2; i++) {
			mio_rst.u64 = cvmx_read_csr(CVMX_MIO_RST_CTLX(i));
			if (mio_rst.cn63xx.prtmode == 1) {
				if (mio_rst.cn63xx.rst_done) {
					bist_failures += displayErrorReg_1("CVMX_PEMX_BIST_STATUS(i)",
									   CVMX_PEMX_BIST_STATUS(i),
									   0,
									   ~0ull);
					bist_failures += displayErrorReg_1("CVMX_PEMX_BIST_STATUS2(i)",
									   CVMX_PEMX_BIST_STATUS2(i), 0,
									   ~0ull);
				} else {
					printf("Skipping PCIe port %d BIST, "
					       "reset not done. "
					       "(port not configured)\n", i);
				}
			} else {
				printf("Skipping PCIe port %d BIST, "
				       "in EP mode, can't tell if clocked.\n",
				       i);
			}
		}
	} else {
		printf("NOTICE: Skipping PCIe BIST registers on "
		       "CN63XX pass 1.\n");
	}
	bist_failures += displayErrorReg_1("CVMX_PEXP_SLI_BIST_STATUS",
					   CVMX_PEXP_SLI_BIST_STATUS, 0, ~0ull);
	bist_failures += displayErrorReg_1("CVMX_PIP_BIST_STATUS",
					   CVMX_PIP_BIST_STATUS, 0, ~0ull);
	bist_failures += displayErrorReg_1("CVMX_PKO_REG_BIST_RESULT",
					   CVMX_PKO_REG_BIST_RESULT, 0, ~0ull);
	bist_failures += displayErrorReg_1("CVMX_RAD_REG_BIST_RESULT",
					   CVMX_RAD_REG_BIST_RESULT, 0, ~0ull);
	bist_failures += displayErrorReg_1("CVMX_RNM_BIST_STATUS",
					   CVMX_RNM_BIST_STATUS, 0, ~0ull);
	bist_failures += displayErrorReg_1("CVMX_TRA0_BIST_STATUS",
					   CVMX_TRA_BIST_STATUS, 0, ~0ull);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		bist_failures += displayErrorReg_1("CVMX_TRA1_BIST_STATUS",
						   CVMX_TRAX_BIST_STATUS(1),
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_TRA2_BIST_STATUS",
						   CVMX_TRAX_BIST_STATUS(2),
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_TRA3_BIST_STATUS",
						   CVMX_TRAX_BIST_STATUS(3),
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_L2C_BST", CVMX_L2C_BST, 0,
						   make_cm_mask(cm_override, 32) | 0xffffffff);
	}
	else
	{
		bist_failures += displayErrorReg_1("CVMX_L2C_BST", CVMX_L2C_BST, 0,
						   make_cm_mask(cm_override, 32) | 0x11111);
	}
        if (!OCTEON_IS_MODEL(OCTEON_CNF71XX))
        	bist_failures += displayErrorReg_1("CVMX_ZIP_CMD_BIST_RESULT",
        					   CVMX_ZIP_CMD_BIST_RESULT, 0, ~0ull);


	for (tad = 0; tad < CVMX_L2C_TADS; tad++) {
		bist_failures += displayErrorReg_1("CVMX_L2C_BST_MEMX",
						   CVMX_L2C_BST_MEMX(tad),
						   0, 0x1f);
		bist_failures += displayErrorReg_1("CVMX_L2C_BST_TDTX",
						   CVMX_L2C_BST_TDTX(tad),
						   0, ~0ull);
		bist_failures += displayErrorReg_1("CVMX_L2C_BST_TTGX",
						   CVMX_L2C_BST_TTGX(tad),
						   0, ~0ull);
	}

#if 1
	if (bist_failures) {
		printf("BIST Errors found (%d).\n", bist_failures);
		printf("'1' bits above indicate unexpected BIST status.\n");
	} else {
		printf("BIST check passed.\n");
	}
#endif

	if (0) {
		/* Print TDTX bist on LED */

		char str[10];
		uint64_t val = cvmx_read_csr(CVMX_L2C_BST_TDTX(0));
		int ival = (val >> 8) & 0xff;
		if (!bist_failures)
			sprintf(str, "PASS    ");
		else if (bist_failures == 1 && ival && !(val & ~0xFF00ull))
			sprintf(str, "%02x    %d ", ival,
				__builtin_popcount(ival));
		else
			sprintf(str, "FAIL    ");

		octeon_led_str_write(str);

	}
	return (0);
}

int octeon_bist(void)
{
	/* Mask POW BIST_STAT with coremask so we don't report errors on
	 * known bad cores
	 */
#ifndef CONFIG_OCTEON_CN70XX
	uint32_t val;
	char *cm_str = getenv("coremask_override");
	int bist_failures = 0;
#endif

#ifdef CONFIG_OCTEON_CN70XX
	return octeon_bist_70XX();
#else
	if (OCTEON_IS_MODEL(OCTEON_CN70XX))
		return (octeon_bist_70XX());

	if (OCTEON_IS_MODEL(OCTEON_CN73XX)
	    || OCTEON_IS_MODEL(OCTEON_CNF75XX))
		return (octeon_bist_7XXX(0));

	if (OCTEON_IS_MODEL(OCTEON_CN78XX)) {
# ifdef CONFIG_OCTEON_OCX
		int node;
		int err_cnt = 0;
		cvmx_coremask_for_each_node(node, gd->arch.node_mask) {
			debug("Running BIST for 78xx node %d\n", node);
			err_cnt += octeon_bist_7XXX(node);
		}
		return err_cnt;
# else
		return octeon_bist_7XXX(0);
# endif
	}

	if (OCTEON_IS_OCTEON2())
		return (octeon_bist_6XXX());
	val = (uint32_t) cvmx_read_csr(CVMX_POW_BIST_STAT);
	if (cm_str) {
		uint32_t cm_override = simple_strtoul(cm_str, NULL, 0);
		/* Shift coremask override to match up with core BIST
		 * bits in register
		 */
		cm_override = cm_override << 16;
		val &= cm_override;
	}
	if (val) {
		printf("BIST FAILURE: POW_BIST_STAT: 0x%08x\n", val);
		bist_failures++;
	}
	bist_failures +=
	    displayErrorRegC0_cvmmem("COP0_CVMMEMCTL_REG", 0ull,
				     0xfc00000000000000ull);
	bist_failures +=
	    displayErrorRegC0_Icache("COP0_ICACHEERR_REG",
				     0x007f7f0000000000ull,
				     0x007f7f0000000000ull | 0x3full << 32);

	/*  NAME    REGISTER     EXPECTED    MASK   */
	bist_failures += displayErrorReg_1("GMX0_BIST", CVMX_GMXX_BIST(0),
					   0ull, 0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN31XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX))
		bist_failures +=
		    displayErrorReg_1("GMX1_BIST", CVMX_GMXX_BIST(1), 0ull,
				      0xffffffffffffffffull);
	bist_failures +=
	    displayErrorReg_1("IPD_BIST_STATUS", CVMX_IPD_BIST_STATUS, 0ull,
			      0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN31XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX))
		bist_failures +=
		    displayErrorReg_1("KEY_BIST_REG", CVMX_KEY_BIST_REG, 0ull,
				      0xffffffffffffffffull);
	bist_failures += displayErrorReg_1("L2D_BST0", CVMX_L2D_BST0,
					   0ull, 1ull << 34);
	bist_failures += displayErrorReg_1("L2C_BST0", CVMX_L2C_BST0,
					   0, 0x1full);
	bist_failures += displayErrorReg_1("L2C_BST1", CVMX_L2C_BST1,
					   0, 0xffffffffffffffffull);
	bist_failures += displayErrorReg_1("L2C_BST2", CVMX_L2C_BST2,
					   0, 0xffffffffffffffffull);
	bist_failures += displayErrorReg_1("CIU_BIST", CVMX_CIU_BIST,
					   0, 0xffffffffffffffffull);
	bist_failures +=
	    displayErrorReg_1("PKO_REG_BIST_RESULT", CVMX_PKO_REG_BIST_RESULT,
			      0, 0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN56XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX)) {
		bist_failures +=
		    displayErrorReg_1("NPI_BIST_STATUS", CVMX_NPI_BIST_STATUS,
				      0, 0xffffffffffffffffull);
	}
	if (!OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		bist_failures +=
		    displayErrorReg_1("PIP_BIST_STATUS", CVMX_PIP_BIST_STATUS,
				      0, 0xffffffffffffffffull);
	}
	bist_failures +=
	    displayErrorReg_1("RNM_BIST_STATUS", CVMX_RNM_BIST_STATUS, 0,
			      0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN31XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN56XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX)
	    ) {
		bist_failures +=
		    displayErrorReg_1("SPX0_BIST_STAT",
				      CVMX_SPXX_BIST_STAT(0), 0,
				      0xffffffffffffffffull);
		bist_failures +=
		    displayErrorReg_1("SPX1_BIST_STAT",
				      CVMX_SPXX_BIST_STAT(1), 0,
				      0xffffffffffffffffull);
	}
	bist_failures +=
	    displayErrorReg_1("TIM_REG_BIST_RESULT", CVMX_TIM_REG_BIST_RESULT,
			      0, 0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX))
		bist_failures +=
		    displayErrorReg_1("TRA_BIST_STATUS", CVMX_TRA_BIST_STATUS,
				      0, 0xffffffffffffffffull);
	bist_failures +=
	    displayErrorReg_1("MIO_BOOT_BIST_STAT", CVMX_MIO_BOOT_BIST_STAT, 0,
			      0xffffffffffffffffull);
	bist_failures +=
	    displayErrorReg_1("IOB_BIST_STATUS", CVMX_IOB_BIST_STATUS, 0,
			      0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN56XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX)
	    ) {
		bist_failures +=
		    displayErrorReg_1("DFA_BST0", CVMX_DFA_BST0, 0,
				      0xffffffffffffffffull);
		bist_failures +=
		    displayErrorReg_1("DFA_BST1", CVMX_DFA_BST1, 0,
				      0xffffffffffffffffull);
	}
	bist_failures +=
	    displayErrorReg_1("FPA_BIST_STATUS", CVMX_FPA_BIST_STATUS, 0,
			      0xffffffffffffffffull);
	if (!OCTEON_IS_MODEL(OCTEON_CN30XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN50XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX))
		bist_failures +=
		    displayErrorReg_1("ZIP_CMD_BIST_RESULT",
				      CVMX_ZIP_CMD_BIST_RESULT, 0,
				      0xffffffffffffffffull);

	if (!OCTEON_IS_MODEL(OCTEON_CN38XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN58XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN52XX)) {
		bist_failures +=
		    displayErrorReg_1("USBNX_BIST_STATUS(0)",
				      CVMX_USBNX_BIST_STATUS(0), 0, ~0ull);
	}
	if ((OCTEON_IS_MODEL(OCTEON_CN56XX))
	    || (OCTEON_IS_MODEL(OCTEON_CN52XX))) {
		bist_failures +=
		    displayErrorReg_1("AGL_GMX_BIST", CVMX_AGL_GMX_BIST, 0,
				      ~0ull);
		bist_failures +=
		    displayErrorReg_1("IOB_BIST_STATUS", CVMX_IOB_BIST_STATUS,
				      0, ~0ull);
		bist_failures +=
		    displayErrorReg_1("MIXX_BIST(0)", CVMX_MIXX_BIST(0), 0,
				      ~0ull);
		bist_failures +=
		    displayErrorReg_1("NPEI_BIST_STATUS",
				      CVMX_PEXP_NPEI_BIST_STATUS, 0, ~0ull);
		bist_failures +=
		    displayErrorReg_1("RAD_REG_BIST_RESULT",
				      CVMX_RAD_REG_BIST_RESULT, 0, ~0ull);
		bist_failures +=
		    displayErrorReg_1("RNM_BIST_STATUS", CVMX_RNM_BIST_STATUS,
				      0, ~0ull);
	}
	if (OCTEON_IS_MODEL(OCTEON_CN56XX)) {
		bist_failures +=
		    displayErrorReg_1("PCSX0_BIST_STATUS_REG(0)",
				      CVMX_PCSXX_BIST_STATUS_REG(0), 0, ~0ull);
		bist_failures +=
		    displayErrorReg_1("PCSX1_BIST_STATUS_REG(1)",
				      CVMX_PCSXX_BIST_STATUS_REG(1), 0, ~0ull);
	}
	if (bist_failures) {
		printf("BIST Errors found.\n");
		printf("'1' bits above indicate unexpected BIST status.\n");
	} else {
		printf("BIST check passed.\n");
	}
#endif
	return 0;
}
