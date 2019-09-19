/***********************license start************************************
 * Copyright (c) 2013-2015 Cavium Inc. (support@cavium.com). All rights
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
 ***********************license end**************************************/

/* $Revision: 126098 $ */

#define RLEXTRAS_PATCH     1 // write to unused RL rank entries
#define WLEXTRAS_PATCH     1 // write to unused WL rank entries
#define ADD_48_OHM_SKIP    0
#define NOSKIP_40_48_OHM   1
#define NOSKIP_48_STACKED  1
#define NOSKIP_FOR_MINI    1
#define NOSKIP_FOR_2S_1R   1
#define MAJORITY_OVER_AVG  1
#define RANK_MAJORITY      MAJORITY_OVER_AVG && 1

// define how many HW RL samples per rank to take
// multiple samples will allow either:
// 1. looking for the best sample score
// 2. averaging the samples into a composite score
// symbol PICK_BEST_RANK_SCORE_NOT_AVG is used to choose
//
#define RLEVEL_AVG_LOOPS_DEFAULT     3
#define PICK_BEST_RANK_SCORE_NOT_AVG 1

#define FAILSAFE_CHECK           1

#define PERFECT_BITMASK_COUNTING 1

#define DISABLE_SW_WL_PASS_2  1
#define SW_WL_CHECK_PATCH     1
#define HW_WL_MAJORITY        1
// try HW WL base alternate if available when SW WL fails
#define SWL_TRY_HWL_ALT       HW_WL_MAJORITY && 1
// define how many HW WL samples to take for majority voting
// MUST BE odd!!
// assume there should only be 2 possible values that will show up,
// so treat ties as a problem!!!
#define WLEVEL_LOOPS_DEFAULT     5 // NOTE: do not change this without checking the code!!!

#define SWL_WITH_HW_ALTS_CHOOSE_SW 0 // FIXME: allow override?

// collect and print LMC utilization using SWL software algorithm
#define ENABLE_SW_WLEVEL_UTILIZATION 0 

#define COUNT_RL_CANDIDATES 1

#define LOOK_FOR_STUCK_BYTE      0
#define ENABLE_STUCK_BYTE_RESET  0

#define DDR3_DAC_OVERRIDE  1

#define DEFAULT_BEST_RANK_SCORE  9999999
#define MAX_RANK_SCORE_LIMIT     99 // FIXME?

#define ENABLE_WRITE_DESKEW_DEFAULT 0
#define ENABLE_WRITE_DESKEW_VERIFY  1

#define ENABLE_COMPUTED_VREF_ADJUSTMENT 1

#undef ENABLE_SLOT_CTL_ACCESS

#undef DEBUG_PERFORM_DDR3_SEQUENCE
#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
#define ddr_seq_print(format, ...) \
 do { \
            printf(format, ##__VA_ARGS__); \
 } while (0)
#else
#define ddr_seq_print(format, ...) do {} while (0)
#endif

typedef struct {
    unsigned char *rodt_ohms;
    unsigned char *rtt_nom_ohms;
    unsigned char *rtt_nom_table;
    unsigned char *rtt_wr_ohms;
    unsigned char *dic_ohms;
    short         *drive_strength;
    short         *dqx_strength;
} impedence_values_t;

static void perform_ddr_init_sequence(uint32_t cpu_id, int node, int rank_mask,
				       int ddr_interface_num);

// "mode" arg
#define DBTRAIN_TEST 0
#define DBTRAIN_DBI  1 
#define DBTRAIN_LFSR 2 
static int run_best_hw_patterns(uint32_t cpu_id, int node, int lmc, uint64_t phys_addr,
                                int mode, uint64_t *xor_data);

static int ddr_memory_preserved(void);

#define LMC_DDR3_RESET_ASSERT   0
#define LMC_DDR3_RESET_DEASSERT 1

static void
cn7xxx_lmc_ddr3_reset(int node, int ddr_interface_num, int reset)
{
    cvmx_lmcx_reset_ctl_t reset_ctl;
    /*
     * 4. Deassert DDRn_RESET_L pin by writing LMC(0..3)_RESET_CTL[DDR3RST] = 1
     *    without modifying any other LMC(0..3)_RESET_CTL fields.
     * 5. Read LMC(0..3)_RESET_CTL and wait for the result.
     * 6. Wait a minimum of 500us. This guarantees the necessary T = 500us
     *    delay between DDRn_RESET_L deassertion and DDRn_DIMM*_CKE* assertion.
     */
    ddr_print("LMC%d %s DDR_RESET_L\n", ddr_interface_num,
	      (reset == LMC_DDR3_RESET_DEASSERT) ? "De-asserting" : "Asserting");

    //DRAM_CSR_MODIFY(c, node, BDK_LMCX_RESET_CTL(ddr_interface_num), c.s.ddr3rst = reset);
    reset_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RESET_CTL(ddr_interface_num));
    reset_ctl.cn78xx.ddr3rst = reset;
    ddr_config_write_csr_node(node, CVMX_LMCX_RESET_CTL(ddr_interface_num), reset_ctl.u64);

    //BDK_CSR_READ(node, BDK_LMCX_RESET_CTL(ddr_interface_num));
    cvmx_read_csr_node(node, CVMX_LMCX_RESET_CTL(ddr_interface_num));

    cvmx_wait_usec(500);
}

static void
perform_lmc_reset(int node, int ddr_interface_num)
{
    /*
     * 5.9.6 LMC RESET Initialization
     *
     * The purpose of this step is to assert/deassert the RESET# pin at the
     * DDR3/DDR4 parts.
     *
     * This LMC RESET step is done for all enabled LMCs.
     *
     * It may be appropriate to skip this step if the DDR3/DDR4 DRAM parts
     * are in self refresh and are currently preserving their
     * contents. (Software can determine this via
     * LMC(0..3)_RESET_CTL[DDR3PSV] in some circumstances.) The remainder of
     * this section assumes that the DRAM contents need not be preserved.
     *
     * The remainder of this section assumes that the CN78XX DDRn_RESET_L pin
     * is attached to the RESET# pin of the attached DDR3/DDR4 parts, as will
     * be appropriate in many systems.
     *
     * (In other systems, such as ones that can preserve DDR3/DDR4 part
     * contents while CN78XX is powered down, it will not be appropriate to
     * directly attach the CN78XX DDRn_RESET_L pin to DRESET# of the
     * DDR3/DDR4 parts, and this section may not apply.)
     *
     * The remainder of this section describes the sequence for LMCn.
     *
     * Perform the following six substeps for LMC reset initialization:
     *
     * 1. If not done already, assert DDRn_RESET_L pin by writing
     * LMC(0..3)_RESET_ CTL[DDR3RST] = 0 without modifying any other
     * LMC(0..3)_RESET_CTL fields.
     */

    if ( !ddr_memory_preserved()) {
        /*
         * 2. Read LMC(0..3)_RESET_CTL and wait for the result.
         */

        cvmx_read_csr_node(node, CVMX_LMCX_RESET_CTL(ddr_interface_num));

        /*
         * 3. Wait until RESET# assertion-time requirement from JEDEC DDR3/DDR4
         * specification is satisfied (200 us during a power-on ramp, 100ns when
         * power is already stable).
         */

        cvmx_wait_usec(200);

        /*
         * 4. Deassert DDRn_RESET_L pin by writing LMC(0..3)_RESET_CTL[DDR3RST] = 1
         *    without modifying any other LMC(0..3)_RESET_CTL fields.
         * 5. Read LMC(0..3)_RESET_CTL and wait for the result.
         * 6. Wait a minimum of 500us. This guarantees the necessary T = 500us
         *    delay between DDRn_RESET_L deassertion and DDRn_DIMM*_CKE* assertion.
         */
        cn7xxx_lmc_ddr3_reset(node, ddr_interface_num, LMC_DDR3_RESET_DEASSERT);

        /* Toggle Reset Again */
        /* That is, assert, then de-assert, one more time */
        cn7xxx_lmc_ddr3_reset(node, ddr_interface_num, LMC_DDR3_RESET_ASSERT);
        cn7xxx_lmc_ddr3_reset(node, ddr_interface_num, LMC_DDR3_RESET_DEASSERT);
    }
}

static void perform_octeon3_ddr3_sequence(uint32_t cpu_id, int node, int rank_mask,
                                          int ddr_interface_num, int sequence)
{
    /*
     * 3. Without changing any other fields in LMC(0)_CONFIG, write
     *    LMC(0)_CONFIG[RANKMASK] then write both
     *    LMC(0)_SEQ_CTL[SEQ_SEL,INIT_START] = 1 with a single CSR write
     *    operation. LMC(0)_CONFIG[RANKMASK] bits should be set to indicate
     *    the ranks that will participate in the sequence.
     *
     *    The LMC(0)_SEQ_CTL[SEQ_SEL] value should select power-up/init or
     *    selfrefresh exit, depending on whether the DRAM parts are in
     *    self-refresh and whether their contents should be preserved. While
     *    LMC performs these sequences, it will not perform any other DDR3
     *    transactions. When the sequence is complete, hardware sets the
     *    LMC(0)_CONFIG[INIT_STATUS] bits for the ranks that have been
     *    initialized.
     *
     *    If power-up/init is selected immediately following a DRESET
     *    assertion, LMC executes the sequence described in the "Reset and
     *    Initialization Procedure" section of the JEDEC DDR3
     *    specification. This includes activating CKE, writing all four DDR3
     *    mode registers on all selected ranks, and issuing the required ZQCL
     *    command. The LMC(0)_CONFIG[RANKMASK] value should select all ranks
     *    with attached DRAM in this case. If LMC(0)_CONTROL[RDIMM_ENA] = 1,
     *    LMC writes the JEDEC standard SSTE32882 control words selected by
     *    LMC(0)_DIMM_CTL[DIMM*_WMASK] between DDR_CKE* signal assertion and
     *    the first DDR3 mode register write operation.
     *    LMC(0)_DIMM_CTL[DIMM*_WMASK] should be cleared to 0 if the
     *    corresponding DIMM is not present.
     *
     *    If self-refresh exit is selected, LMC executes the required SRX
     *    command followed by a refresh and ZQ calibration. Section 4.5
     *    describes behavior of a REF + ZQCS.  LMC does not write the DDR3
     *    mode registers as part of this sequence, and the mode register
     *    parameters must match at self-refresh entry and exit times.
     *
     * 4. Read LMC(0)_SEQ_CTL and wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be
     *    set.
     *
     * 5. Read LMC(0)_CONFIG[INIT_STATUS] and confirm that all ranks have
     *    been initialized.
     */

#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
    static const char *sequence_str[] = {
        "Power-up/init",
        "Read-leveling",
        "Self-refresh entry",
        "Self-refresh exit",
        "Illegal",
        "Illegal",
        "Write-leveling",
        "Init Register Control Words",
        "Mode Register Write",
        "MPR Register Access",
        "LMC Deskew/Internal Vref Training",
        "Offset Training"
    };
#endif

    cvmx_lmcx_seq_ctl_t seq_ctl;
    cvmx_lmcx_config_t  lmc_config;
    int timeout;

    lmc_config.u64 = cvmx_read_csr_node(node,
                                        CVMX_LMCX_CONFIG(ddr_interface_num));
    lmc_config.s.rankmask     = rank_mask;
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_CONFIG(ddr_interface_num),
                              lmc_config.u64);

    seq_ctl.u64    = 0;

    seq_ctl.s.init_start  = 1;
    seq_ctl.s.seq_sel    = sequence;

    ddr_seq_print("Performing LMC sequence: rank_mask=0x%02x, sequence=0x%x, %s\n",
		  rank_mask, sequence, sequence_str[sequence]);

    if (seq_ctl.s.seq_sel == 3)
        ddr_print("LMC%d: Exiting Self-refresh Rank_mask:%x\n", ddr_interface_num, rank_mask);

    if ((s = lookup_env_parameter("ddr_trigger_sequence%d", sequence)) != NULL) {
        int trigger = simple_strtoul(s, NULL, 0);
        if (trigger)
            pulse_gpio_pin(1, 2);
    }

    ddr_config_write_csr_node(node,
                              CVMX_LMCX_SEQ_CTL(ddr_interface_num),
                              seq_ctl.u64);
    cvmx_read_csr_node(node, CVMX_LMCX_SEQ_CTL(ddr_interface_num));

    timeout = 100;
    do {
        cvmx_wait_usec(100); /* Wait a while */
        seq_ctl.u64 = cvmx_read_csr_node(node,
                                         CVMX_LMCX_SEQ_CTL(ddr_interface_num));
        if (--timeout == 0) {
            error_print("Sequence %d timed out\n", sequence);
            break;
        }
    } while (seq_ctl.s.seq_complete != 1);

    ddr_seq_print("           LMC sequence=%x: Completed.\n", sequence);
}

#if 0
#define DO_LIKE_RANDOM_XOR 1
static int test_dram_byte64(uint32_t cpu_id, int node, int ddr_interface_num,
                            uint64_t p, uint64_t bitmask, uint64_t *xor_data)
{
    uint64_t p1, p2, d1, d2;
    uint64_t v, v1;
    uint64_t p2offset = 0x4000000;
    uint64_t datamask;
    uint64_t xor;
    int i, j, k;
    int errors = 0;
    int index;
    int bitno = (octeon_is_cpuid(OCTEON_CN7XXX)) ? 20: 18;
#if DO_LIKE_RANDOM_XOR
    uint64_t pattern1 = cvmx_rng_get_random64();
    uint64_t this_pattern;
#endif
    uint64_t bad_bits[2] = {0,0};

    if (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX))
        bitno = 18;

    // When doing in parallel, the caller must provide full 8-byte bitmask.
    // Byte lanes may be clear in the mask to indicate no testing on that lane.
    datamask = bitmask;

    // final address must include LMC and node
    p |= (ddr_interface_num << 7); /* Map address into proper interface */
    p |= (uint64_t)node << CVMX_NODE_MEM_SHIFT; // map to node

    p |= 1ull<<63;

    /* Add offset to both test regions to not clobber boot stuff
     * when running from L2.
     */
    p += 0x4000000;

    debug_print("N%d.LMC%d: test_dram_byte64: phys_addr=0x%lx/0x%lx (0x%lx)\n", 
                node, ddr_interface_num, p, p+p2offset, 1ULL<<bitno);

    /* The loop ranges and increments walk through a range of addresses avoiding bits that alias
     * to different memory interfaces (LMCs) on the CN88XX; ie we want to limit activity to a
     * single memory channel.
     */

    /* Store something into each location first */
    // NOTE: the ordering of loops is purposeful: fill full cachelines and flush
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
	for (j = 0; j < (1 << 12); j += (1 << 9)) {
	    for (i = 0; i < (1 << 7); i += 8) {
		index = i + j + k;
		p1 = p + index;
		p2 = p1 + p2offset;

#if DO_LIKE_RANDOM_XOR
		v = pattern1 * p1;
		v1 = v; // write the same thing to both areas
#else
		v = 0ULL;
		v1 = v;
#endif
		cvmx_write64_uint64(p1, v);
		cvmx_write64_uint64(p2, v1);

#ifdef __U_BOOT__
		/* Write back and invalidate the cache lines
		 *
		 * For OCX we cannot limit the number of L2 ways
		 * so instead we just write back and invalidate
		 * the L2 cache lines.  This is not possible
		 * when booting remotely, however so this is
		 * only enabled for U-Boot right now.
		 * Potentially the BDK can also take advantage
		 * of this.
		 */
		CVMX_CACHE_WBIL2(p1, 0);
		CVMX_CACHE_WBIL2(p2, 0);
#endif
	    }
	}
    }
#ifdef __U_BOOT__
    CVMX_DCACHE_INVALIDATE;
#endif

#if DO_LIKE_RANDOM_XOR
    this_pattern = cvmx_rng_get_random64();
#endif

    // modify the contents of each location in some way
    // NOTE: the ordering of loops is purposeful: modify full cachelines and flush
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
	for (j = 0; j < (1 << 12); j += (1 << 9)) {
	    for (i = 0; i < (1 << 7); i += 8) {
		index = i + j + k;
		p1 = p + index;
		p2 = p1 + p2offset;
#if DO_LIKE_RANDOM_XOR
		v  = cvmx_read64_uint64(p1) ^ this_pattern;
		v1 = cvmx_read64_uint64(p2) ^ this_pattern;
#else
		v = test_pattern[index%(sizeof(test_pattern)/sizeof(uint64_t))];
		v &= datamask;
		v1 = ~v;
#endif

		/* test_dram_byte_print("[0x%016llX]: 0x%016llX, [0x%016llX]: 0x%016llX\n",
		 *            p1, v, p2, v1);
		 */
		cvmx_write64_uint64(p1, v);
		cvmx_write64_uint64(p2, v1);

#ifdef __U_BOOT__
		/* Write back and invalidate the cache lines
		 *
		 * For OCX we cannot limit the number of L2 ways
		 * so instead we just write back and invalidate
		 * the L2 cache lines.  This is not possible
		 * when booting remotely, however so this is
		 * only enabled for U-Boot right now.
		 * Potentially the BDK can also take advantage
		 * of this.
		 */
		CVMX_CACHE_WBIL2(p1, 0);
		CVMX_CACHE_WBIL2(p2, 0);
#endif
	    }
	}
    }
#ifdef __U_BOOT__
    CVMX_DCACHE_INVALIDATE;
#endif

    // test the contents of each location by predicting what should be there
    // NOTE: the ordering of loops is purposeful: test full cachelines to detect
    //       an error occuring in any slot thereof
    for (k = 0; k < (1 << bitno); k += (1 << 14)) {
	for (j = 0; j < (1 << 12); j += (1 << 9)) {
	    for (i = 0; i < (1 << 7); i += 8) {
		int bybit = 1;
		uint64_t bymsk = 0xffULL; // start in byte lane 0

		index = i + j + k;
		p1 = p + index;
		p2 = p1 + p2offset;
#if DO_LIKE_RANDOM_XOR
		v = (p1 * pattern1) ^ this_pattern; // FIXME: this should predict what we find...???
		d1 = cvmx_read64_uint64(p1);
		d2 = cvmx_read64_uint64(p2);
#else
		v = test_pattern[index%(sizeof(test_pattern)/sizeof(uint64_t))];
		d1 = cvmx_read64_uint64(p1);
		d2 = ~cvmx_read64_uint64(p2);
#endif
		/* test_dram_byte_print("[0x%016llX]: 0x%016llX, [0x%016llX]: 0x%016llX\n",
		 *             p1, d1, p2, d2);
		 */

		xor = ((d1 ^ v) | (d2 ^ v)) & datamask; // union of error bits only in active byte lanes

                if (!xor)
                    continue;

                // accumulate bad bits
                bad_bits[0] |= xor;
                //bad_bits[1] |= ~mpr_data1 & 0xffUL; // cannot do ECC here

		while (xor != 0) {
		    debug_print("ERROR: [0x%016llX] [0x%016llX]  expected 0x%016llX xor %016llX\n",
				p1, p2, v, xor);
		    if (xor & bymsk) { // error(s) in this lane
			errors |= bybit; // set the byte error bit
			xor &= ~bymsk; // clear byte lane in error bits
			datamask &= ~bymsk; // clear the byte lane in the mask
			if (datamask == 0) { // nothing left to do
			    goto done_now; // completely done when errors found in all byte lanes in datamask
			}
		    }
		    bymsk <<= 8; // move mask into next byte lane
		    bybit <<= 1; // move bit into next byte position
		}
	    }
	}
    }

 done_now:
    if (xor_data != NULL) { // send the bad bits back...
        xor_data[0] = bad_bits[0];
        xor_data[1] = bad_bits[1]; // let it be zeroed
    }
    return errors;
}
#else
#ifdef __U_BOOT__
#define BDK_CACHE_WBI_L2(p)   CVMX_CACHE_WBIL2(p, 0)
#define BDK_DCACHE_INVALIDATE CVMX_DCACHE_INVALIDATE
#else
#define BDK_CACHE_WBI_L2(p)   do {} while (0)
#define BDK_DCACHE_INVALIDATE
#endif
#define bdk_rng_get_random64  cvmx_rng_get_random64
#define __bdk_dram_write64    cvmx_write64_uint64
#define __bdk_dram_read64     cvmx_read64_uint64
#define bdk_numa_get_address(n, p) ((p) | ((uint64_t)n) << CVMX_NODE_MEM_SHIFT)
#define AREA_BASE_OFFSET (1ULL << 26)
static int
/*dram_tuning_mem_xor*/
test_dram_byte64(uint32_t cpu_id, int node, int lmc, uint64_t p,
                 uint64_t bitmask, uint64_t *xor_data)
{
    uint64_t p1, p2, d1, d2;
    uint64_t v, v1;
    uint64_t p2offset = (1ULL << 26); // offset to area 2
    uint64_t datamask;
    uint64_t xor;
    uint64_t i, j, k;
    uint64_t ii;
    int errors = 0;
    //uint64_t index;
    uint64_t pattern1 = bdk_rng_get_random64();
    uint64_t pattern2 = 0;
    uint64_t bad_bits[2] = {0,0};
    int kbitno = (octeon_is_cpuid(OCTEON_CN7XXX)) ? 20: 18;
    cvmx_l2c_ctl_t l2c_ctl;
    int burst;
    int saved_dissblkdty;

    // Force full cacheline write-backs to boost traffic
    //BDK_CSR_MODIFY(c, node, BDK_L2C_CTL, c.s.dissblkdty = 1);
    l2c_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
    saved_dissblkdty = l2c_ctl.cn78xx.dissblkdty;
    l2c_ctl.cn78xx.dissblkdty = 1;
    ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_ctl.u64);

    if (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX))
        kbitno = 18;

    // Byte lanes may be clear in the mask to indicate no testing on that lane.
    datamask = bitmask;

    /* Add offset to both test regions to not clobber boot stuff
     * when running from L2 for NAND boot.
     */
    //if (p == 0) // only needed for rank 0
        p += AREA_BASE_OFFSET; // make sure base is out of the way of boot

    // final address must include LMC and node
    p |= (lmc << 7); /* Map address into proper interface */
    p = bdk_numa_get_address(node, p); /* Map to node */
    p |= 1ull<<63;

#define II_INC (1ULL << 22)
#define II_MAX (1ULL << 22)
#define K_INC  (1ULL << 14)
#define K_MAX  (1ULL << kbitno)
#define J_INC  (1ULL <<  9)
#define J_MAX  (1ULL << 12)
#define I_INC  (1ULL <<  3)
#define I_MAX  (1ULL <<  7)

    debug_print("N%d.LMC%d: dram_tuning_mem_xor: phys_addr=0x%lx/0x%lx (0x%lx)\n", 
                node, lmc, p, p+p2offset, 1ULL<<kbitno);

#if 0
    int ix;
    // add this loop to fill memory with the test pattern first
    // loops are ordered so that only entire cachelines are written 
    for (ii = 0; ii < II_MAX; ii += II_INC) { // FIXME? extend the range of memory tested!!
	for (k = 0; k < K_MAX; k += K_INC) {
	    for (j = 0; j < J_MAX; j += J_INC) {
		p1 = p + ii + k + j;
		p2 = p1 + p2offset;
		for (i = 0, ix = 0; i < I_MAX; i += I_INC, ix++) {

		    v = dram_tune_test_pattern[ix];
		    v1 = v; // write the same thing to both areas

		    __bdk_dram_write64(p1 + i, v);
		    __bdk_dram_write64(p2 + i, v1);

		}
		BDK_CACHE_WBI_L2(p1);
		BDK_CACHE_WBI_L2(p2);
	    }
	}
    } /* for (ii = 0; ii < (1ULL << 31); ii += (1ULL << 29)) */
#endif

    // loops are ordered so that only a single 64-bit slot is written to each cacheline at one time,
    // then the cachelines are forced out; this should maximize read/write traffic
    for (ii = 0; ii < II_MAX; ii += II_INC) { // FIXME? extend the range of memory tested!!
	for (k = 0; k < K_MAX; k += K_INC) {
	    for (i = 0; i < I_MAX; i += I_INC) {
		for (j = 0; j < J_MAX; j += J_INC) {

		    p1 = p + ii + k + j;
		    p2 = p1 + p2offset;
	    
		    v = pattern1 * (p1 + i);
		    v1 = v; // write the same thing to both areas

		    __bdk_dram_write64(p1 + i, v);
		    __bdk_dram_write64(p2 + i, v1);

		    BDK_CACHE_WBI_L2(p1);
		    BDK_CACHE_WBI_L2(p2);
		}
	    }
	}
    } /* for (ii = 0; ii < (1ULL << 31); ii += (1ULL << 29)) */

    BDK_DCACHE_INVALIDATE;

    debug_print("N%d.LMC%d: dram_tuning_mem_xor: done INIT loop\n", 
              node, lmc);

    /* Make a series of passes over the memory areas. */

    for (burst = 0; burst < 1/* was: dram_tune_use_bursts*/; burst++)
    {
	uint64_t this_pattern = bdk_rng_get_random64();
	pattern2 ^= this_pattern;

        /* XOR the data with a random value, applying the change to both
         * memory areas.
         */

	for (ii = 0; ii < II_MAX; ii += II_INC) { // FIXME? extend the range of memory tested!!
	    for (k = 0; k < K_MAX; k += K_INC) {
		for (i = 0; i < I_MAX; i += I_INC) { // FIXME: rearranged, did not make much difference?
		    for (j = 0; j < J_MAX; j += J_INC) {

			p1 = p + ii + k + j;
			p2 = p1 + p2offset;
	    
			v  = __bdk_dram_read64(p1 + i) ^ this_pattern;
			v1 = __bdk_dram_read64(p2 + i) ^ this_pattern;

			//BDK_CACHE_INV_L2(p1);
			//BDK_CACHE_INV_L2(p2);

			__bdk_dram_write64(p1 + i, v);
			__bdk_dram_write64(p2 + i, v1);

			BDK_CACHE_WBI_L2(p1);
			BDK_CACHE_WBI_L2(p2);
		    }
		}
	    }
	} /* for (ii = 0; ii < (1ULL << 31); ii += (1ULL << 29)) */

        BDK_DCACHE_INVALIDATE;

        debug_print("N%d.LMC%d: dram_tuning_mem_xor: done MODIFY loop\n", 
                  node, lmc);

        /* Look for differences in the areas. If there is a mismatch, reset
         * both memory locations with the same pattern. Failing to do so
         * means that on all subsequent passes the pair of locations remain
         * out of sync giving spurious errors.
         */
	// FIXME: change the loop order so that an entire cache line is compared at one time
	// FIXME: this is so that a read error that occurs *anywhere* on the cacheline will be caught,
	// FIXME: rather than comparing only 1 cacheline slot at a time, where an error on a different
	// FIXME: slot will be missed that time around
	// Does the above make sense?

	for (ii = 0; ii < II_MAX; ii += II_INC) { // FIXME? extend the range of memory tested!!
	    for (k = 0; k < K_MAX; k += K_INC) {
		for (j = 0; j < J_MAX; j += J_INC) {

		    p1 = p + ii + k + j;
		    p2 = p1 + p2offset;

		    // process entire cachelines in the innermost loop
		    for (i = 0; i < I_MAX; i += I_INC) {
			int bybit = 1;
			uint64_t bymsk = 0xffULL; // start in byte lane 0

			v = ((p1 + i) * pattern1) ^ pattern2; // FIXME: this should predict what we find...???
			d1 = __bdk_dram_read64(p1 + i);
			d2 = __bdk_dram_read64(p2 + i);

			xor = ((d1 ^ v) | (d2 ^ v)) & datamask; // union of error bits only in active byte lanes

                        if (!xor)
                            continue;

                        // accumulate bad bits
                        bad_bits[0] |= xor;
                        //bad_bits[1] |= ~mpr_data1 & 0xffUL; // cannot do ECC here

			while (xor != 0) {
			    debug_print("ERROR(%03d): [0x%016lX] [0x%016lX]  expected 0x%016lX d1 %016lX d2 %016lX\n",
					burst, p1, p2, v, d1, d2);
			    if (xor & bymsk) { // error(s) in this lane
				errors |= bybit; // set the byte error bit
				xor &= ~bymsk; // clear byte lane in error bits
				datamask &= ~bymsk; // clear the byte lane in the mask
#if EXIT_WHEN_ALL_LANES_HAVE_ERRORS
				if (datamask == 0) { // nothing left to do
				    return errors; // completely done when errors found in all byte lanes in datamask
				}
#endif /* EXIT_WHEN_ALL_LANES_HAVE_ERRORS */
			    }
			    bymsk <<= 8; // move mask into next byte lane
			    bybit <<= 1; // move bit into next byte position
			}
		    }
		    BDK_CACHE_WBI_L2(p1);
		    BDK_CACHE_WBI_L2(p2);
		}
	    }
	} /* for (ii = 0; ii < (1ULL << 31); ii += (1ULL << 29)) */

        debug_print("N%d.LMC%d: dram_tuning_mem_xor: done TEST loop\n", 
                  node, lmc);

    } /* for (int burst = 0; burst < dram_tune_use_bursts; burst++) */

    if (xor_data != NULL) { // send the bad bits back...
        xor_data[0] = bad_bits[0];
        xor_data[1] = bad_bits[1]; // let it be zeroed
    }

    // Restore original setting that could enable partial cacheline writes
    //BDK_CSR_MODIFY(c, node, BDK_L2C_CTL, c.s.dissblkdty = 0);
    l2c_ctl.u64 = cvmx_read_csr_node(node, CVMX_L2C_CTL);
    l2c_ctl.cn78xx.dissblkdty = saved_dissblkdty;
    ddr_config_write_csr_node(node, CVMX_L2C_CTL, l2c_ctl.u64);

    return errors;
}
#endif

static void
ddr4_mrw(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
         int mr_wr_addr, int mr_wr_sel, int mr_wr_bg1)
{
    cvmx_lmcx_mr_mpr_ctl_t lmc_mr_mpr_ctl;

    lmc_mr_mpr_ctl.u64 = 0;
    lmc_mr_mpr_ctl.cn78xx.mr_wr_addr               = (mr_wr_addr == -1) ? 0 : mr_wr_addr;
    lmc_mr_mpr_ctl.cn78xx.mr_wr_sel                = mr_wr_sel;
    lmc_mr_mpr_ctl.cn78xx.mr_wr_rank               = rank;
    //lmc_mr_mpr_ctl.cn78xx.mr_wr_pda_mask           =
    //lmc_mr_mpr_ctl.cn78xx.mr_wr_pda_enable         =
    //lmc_mr_mpr_ctl.cn78xx.mpr_loc                  =
    //lmc_mr_mpr_ctl.cn78xx.mpr_wr                   =
    //lmc_mr_mpr_ctl.cn78xx.mpr_bit_select           =
    //lmc_mr_mpr_ctl.cn78xx.mpr_byte_select          =
    //lmc_mr_mpr_ctl.cn78xx.mpr_whole_byte_enable    =
    lmc_mr_mpr_ctl.cn78xx.mr_wr_use_default_value  = (mr_wr_addr == -1) ? 1 : 0;
    lmc_mr_mpr_ctl.cn78xx.mr_wr_bg1                = mr_wr_bg1;
    ddr_config_write_csr_node(node,
			      CVMX_LMCX_MR_MPR_CTL(ddr_interface_num),
			      lmc_mr_mpr_ctl.u64);

    /* Mode Register Write */
    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rank, ddr_interface_num, 0x8);
}

#define InvA0_17(x) (x ^ 0x22bf8)

static void set_mpr_mode (uint32_t cpu_id, int node, int rank_mask,
                          int ddr_interface_num, int dimm_count, int mpr, int bg1)
{
    int rankx;

    ddr_print("All Ranks: Set mpr mode = %x %c-side\n",
              mpr, (bg1==0) ? 'A' : 'B');

    for (rankx = 0; rankx < dimm_count*4; rankx++) {
        if (!(rank_mask & (1 << rankx)))
            continue;
        if (bg1 == 0)
            ddr4_mrw(cpu_id, node, ddr_interface_num, rankx,          mpr<<2,   3, bg1); /* MR3 A-side */
        else
            ddr4_mrw(cpu_id, node, ddr_interface_num, rankx, InvA0_17(mpr<<2), ~3, bg1); /* MR3 B-side */
    }
}

static void do_ddr4_mpr_read(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
                             int page, int location, uint64_t *mpr_data)
{
    cvmx_lmcx_mr_mpr_ctl_t lmc_mr_mpr_ctl;

    lmc_mr_mpr_ctl.u64 = cvmx_read_csr_node(node,
					    CVMX_LMCX_MR_MPR_CTL(ddr_interface_num));
    lmc_mr_mpr_ctl.cn70xx.mr_wr_addr               = 0;
    lmc_mr_mpr_ctl.cn70xx.mr_wr_sel                = page; /* Page */
    lmc_mr_mpr_ctl.cn70xx.mr_wr_rank               = rank;
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_mask           =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_enable         =
    lmc_mr_mpr_ctl.cn70xx.mpr_loc                  = location;
    lmc_mr_mpr_ctl.cn70xx.mpr_wr                   = 0; /* Read=0, Write=1 */
    //lmc_mr_mpr_ctl.cn70xx.mpr_bit_select           =
    //lmc_mr_mpr_ctl.cn70xx.mpr_byte_select          =
    //lmc_mr_mpr_ctl.cn70xx.mpr_whole_byte_enable    =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_use_default_value  =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_bg1                =
    ddr_config_write_csr_node(node,
			      CVMX_LMCX_MR_MPR_CTL(ddr_interface_num),
			      lmc_mr_mpr_ctl.u64);

    /* MPR register access sequence */
    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rank, ddr_interface_num, 0x9);

    debug_print("LMC_MR_MPR_CTL                  : 0x%016llx\n", lmc_mr_mpr_ctl.u64);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mr_wr_addr: 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mr_wr_addr);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mr_wr_sel : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mr_wr_sel);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mpr_loc   : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mpr_loc);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mpr_wr    : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mpr_wr);

}

int set_rdimm_mode(uint64_t node, int ddr_interface_num, int enable)
{
    cvmx_lmcx_control_t lmc_control;
    int save_rdimm_mode;
    lmc_control.u64 = cvmx_read_csr_node(node,
                                         CVMX_LMCX_CONTROL(ddr_interface_num));
    save_rdimm_mode = lmc_control.s.rdimm_ena;
    lmc_control.s.rdimm_ena       = enable;
    ddr_print("Setting RDIMM_ENA = %x\n", enable);
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_CONTROL(ddr_interface_num),
                              lmc_control.u64);
    return (save_rdimm_mode);
}

#if 0
static void ddr4_mpr_read(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
                          int page, int location, uint64_t *mpr_data)
{
    cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
    cvmx_lmcx_rlevel_rankx_t save_lmc_rlevel_rank;

    int save_rdimm_ena = set_rdimm_mode(node, ddr_interface_num, 0);

    lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num));
    save_lmc_rlevel_rank.u64 = lmc_rlevel_rank.u64;

    lmc_rlevel_rank.cn78xx.byte8 += 8;
    lmc_rlevel_rank.cn78xx.byte7 += 8;
    lmc_rlevel_rank.cn78xx.byte6 += 8;
    lmc_rlevel_rank.cn78xx.byte5 += 8;
    lmc_rlevel_rank.cn78xx.byte4 += 8;
    lmc_rlevel_rank.cn78xx.byte3 += 8;
    lmc_rlevel_rank.cn78xx.byte2 += 8;
    lmc_rlevel_rank.cn78xx.byte1 += 8;
    lmc_rlevel_rank.cn78xx.byte0 += 8;

    ddr_config_write_csr_node(node,
                              CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num),
                              lmc_rlevel_rank.u64);

#if 0
    lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num));

    ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5x %5x %5x %5x %5x %5x %5x %5x %5x\n",
              rank,
              lmc_rlevel_rank.cn78xx.status,
              lmc_rlevel_rank.u64,
              lmc_rlevel_rank.cn78xx.byte8,
              lmc_rlevel_rank.cn78xx.byte7,
              lmc_rlevel_rank.cn78xx.byte6,
              lmc_rlevel_rank.cn78xx.byte5,
              lmc_rlevel_rank.cn78xx.byte4,
              lmc_rlevel_rank.cn78xx.byte3,
              lmc_rlevel_rank.cn78xx.byte2,
              lmc_rlevel_rank.cn78xx.byte1,
              lmc_rlevel_rank.cn78xx.byte0
              );
#endif

    set_mpr_mode (cpu_id, node, 1<<rank, ddr_interface_num, 0, /* mpr */ 1, /* bg1 */ 1); /* B-side */

    do_ddr4_mpr_read(cpu_id, node, ddr_interface_num, rank, page, location, mpr_data);

    mpr_data[0] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA0(ddr_interface_num));
    mpr_data[1] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA1(ddr_interface_num));
    mpr_data[2] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA2(ddr_interface_num));
    //ddr_print("MPR Read %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

    set_mpr_mode (cpu_id, node, 1<<rank, ddr_interface_num, 8, /* mpr */ 0, /* bg1 */ 1); /* B-side */

    set_rdimm_mode(node, ddr_interface_num, save_rdimm_ena);

    ddr_config_write_csr_node(node,
                              CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num),
                              save_lmc_rlevel_rank.u64);
}
#endif
#if 0
void ddr4_mpr_read(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
		   int page, int location, uint64_t *mpr_data)
{
    cvmx_lmcx_control_t lmc_control;
    cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, save_lmc_rlevel_rank;
    int delay;
    int save_rdimm_ena;

    set_rdimm_mode(node, ddr_interface_num, 0);

    for (delay=0; delay<32; ++delay) {
        lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num));
        save_lmc_rlevel_rank.u64 = lmc_rlevel_rank.u64;

        lmc_rlevel_rank.cn78xx.byte8 = delay;
        lmc_rlevel_rank.cn78xx.byte7 = delay;
        lmc_rlevel_rank.cn78xx.byte6 = delay;
        lmc_rlevel_rank.cn78xx.byte5 = delay;
        lmc_rlevel_rank.cn78xx.byte4 = delay;
        lmc_rlevel_rank.cn78xx.byte3 = delay;
        lmc_rlevel_rank.cn78xx.byte2 = delay;
        lmc_rlevel_rank.cn78xx.byte1 = delay;
        lmc_rlevel_rank.cn78xx.byte0 = delay;

        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num),
                                  lmc_rlevel_rank.u64);

        lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rank, ddr_interface_num));

        ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5x %5x %5x %5x %5x %5x %5x %5x %5x\n",
                  rank,
                  lmc_rlevel_rank.cn78xx.status,
                  lmc_rlevel_rank.u64,
                  lmc_rlevel_rank.cn78xx.byte8,
                  lmc_rlevel_rank.cn78xx.byte7,
                  lmc_rlevel_rank.cn78xx.byte6,
                  lmc_rlevel_rank.cn78xx.byte5,
                  lmc_rlevel_rank.cn78xx.byte4,
                  lmc_rlevel_rank.cn78xx.byte3,
                  lmc_rlevel_rank.cn78xx.byte2,
                  lmc_rlevel_rank.cn78xx.byte1,
                  lmc_rlevel_rank.cn78xx.byte0
                  );

        do_ddr4_mpr_read(cpu_id, node, ddr_interface_num, rank, page, location, mpr_data);

        mpr_data[0] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA0(ddr_interface_num));
        mpr_data[1] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA1(ddr_interface_num));
        mpr_data[2] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA2(ddr_interface_num));
        ddr_print("MPR Read %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);
    }

    set_rdimm_mode(node, ddr_interface_num, save_rdimm_ena);
}
#endif
#if 1
void ddr4_mpr_read(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
		   int page, int location, uint64_t *mpr_data)
{
#if 0
    cvmx_lmcx_control_t lmc_control;
    int save_rdimm_ena;
    int i;

    set_rdimm_mode(node, ddr_interface_num, 0);

    for (i=0; i<32; ++i) {
#endif

        do_ddr4_mpr_read(cpu_id, node, ddr_interface_num, rank, page, location, mpr_data);

        mpr_data[0] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA0(ddr_interface_num));
        mpr_data[1] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA1(ddr_interface_num));
        mpr_data[2] = cvmx_read_csr_node(node,
                                         CVMX_LMCX_MPR_DATA2(ddr_interface_num));
#if 0
        //ddr_print("MPR Read %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);
    }

    set_rdimm_mode(node, ddr_interface_num, save_rdimm_ena);
#endif
}
#endif

#ifdef ENABLE_DISPLAY_MPR_PAGE
/* Display MPR values for Page Location */
static void Display_MPR_Page_Location(uint32_t cpu_id, int node, int rank,
                                      int ddr_interface_num, int dimm_count,
                                      int page, int location, uint64_t *mpr_data)
{
    ddr4_mpr_read(cpu_id, node, ddr_interface_num, rank, page, location, mpr_data);
    ddr_print("MPR Page %d, Loc %d %016llx.%016llx.%016llx\n",
              page, location, mpr_data[2], mpr_data[1], mpr_data[0]);
}

/* Display MPR values for Page */
static void Display_MPR_Page(uint32_t cpu_id, int node, int rank_mask,
                             int ddr_interface_num, int dimm_count, int page)
{
    int rankx, location;
    uint64_t mpr_data[3];

    for (rankx = 0; rankx < dimm_count * 4;rankx++) {
        if (!(rank_mask & (1 << rankx)))
            continue;

        ddr_print("Rank %d: MPR values for Page %d\n", rankx, page);
        for (location = 0; location < 4; location++) {
            Display_MPR_Page_Location(cpu_id, node, rankx, ddr_interface_num, dimm_count,
                                      page, location, &mpr_data[0]);
        }

    } /* for (rankx = 0; rankx < dimm_count * 4; rankx++) */
}
#endif /* ENABLE_DISPLAY_MPR_PAGE */

void ddr4_mpr_write(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
                    int page, int location, uint8_t mpr_data)
{
    cvmx_lmcx_mr_mpr_ctl_t lmc_mr_mpr_ctl;

    lmc_mr_mpr_ctl.u64 = 0;
    lmc_mr_mpr_ctl.cn70xx.mr_wr_addr               = mpr_data;
    lmc_mr_mpr_ctl.cn70xx.mr_wr_sel                = page; /* Page */
    lmc_mr_mpr_ctl.cn70xx.mr_wr_rank               = rank;
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_mask           =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_enable         =
    lmc_mr_mpr_ctl.cn70xx.mpr_loc                  = location;
    lmc_mr_mpr_ctl.cn70xx.mpr_wr                   = 1; /* Read=0, Write=1 */
    //lmc_mr_mpr_ctl.cn70xx.mpr_bit_select           =
    //lmc_mr_mpr_ctl.cn70xx.mpr_byte_select          =
    //lmc_mr_mpr_ctl.cn70xx.mpr_whole_byte_enable    =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_use_default_value  =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_bg1                =
    ddr_config_write_csr_node(node,
			      CVMX_LMCX_MR_MPR_CTL(ddr_interface_num),
			      lmc_mr_mpr_ctl.u64);

    /* MPR register access sequence */
    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rank, ddr_interface_num, 0x9);

    debug_print("LMC_MR_MPR_CTL                  : 0x%016llx\n", lmc_mr_mpr_ctl.u64);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mr_wr_addr: 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mr_wr_addr);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mr_wr_sel : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mr_wr_sel);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mpr_loc   : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mpr_loc);
    debug_print("lmc_mr_mpr_ctl.cn70xx.mpr_wr    : 0x%02x\n", lmc_mr_mpr_ctl.cn70xx.mpr_wr);
}

void set_vref(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
	      int range, int value)
{
    cvmx_lmcx_mr_mpr_ctl_t lmc_mr_mpr_ctl;
    cvmx_lmcx_modereg_params3_t lmc_modereg_params3;
    int mr_wr_addr = 0;

    lmc_mr_mpr_ctl.u64 = 0;
    lmc_modereg_params3.u64 = cvmx_read_csr_node(node,
						 CVMX_LMCX_MODEREG_PARAMS3(ddr_interface_num));

    mr_wr_addr |= lmc_modereg_params3.s.tccd_l<<10; /* A12:A10 tCCD_L */
    mr_wr_addr |= 1<<7;         /* A7 1 = Enable(Training Mode) */
    mr_wr_addr |= range<<6;     /* A6 VrefDQ Training Range */
    mr_wr_addr |= value<<0;     /* A5:A0 VrefDQ Training Value */

    lmc_mr_mpr_ctl.cn70xx.mr_wr_addr               = mr_wr_addr;
    lmc_mr_mpr_ctl.cn70xx.mr_wr_sel                = 6; /* Write MR6 */
    lmc_mr_mpr_ctl.cn70xx.mr_wr_rank               = rank;
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_mask           =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_pda_enable         =
    //lmc_mr_mpr_ctl.cn70xx.mpr_loc                  = location;
    //lmc_mr_mpr_ctl.cn70xx.mpr_wr                   = 0; /* Read=0, Write=1 */
    //lmc_mr_mpr_ctl.cn70xx.mpr_bit_select           =
    //lmc_mr_mpr_ctl.cn70xx.mpr_byte_select          =
    //lmc_mr_mpr_ctl.cn70xx.mpr_whole_byte_enable    =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_use_default_value  =
    //lmc_mr_mpr_ctl.cn70xx.mr_wr_bg1                =
    ddr_config_write_csr_node(node,
			      CVMX_LMCX_MR_MPR_CTL(ddr_interface_num),
			      lmc_mr_mpr_ctl.u64);

    /* 0x8 = Mode Register Write */
    perform_octeon3_ddr3_sequence(cpu_id, node, 1<<rank, ddr_interface_num, 0x8);

    /* It is vendor specific whether Vref_value is captured with A7=1.
       A subsequent MRS might be necessary. */
    perform_octeon3_ddr3_sequence(cpu_id, node, 1<<rank, ddr_interface_num, 0x8);

    mr_wr_addr &= ~(1<<7);         /* A7 0 = Disable(Training Mode) */
    lmc_mr_mpr_ctl.cn70xx.mr_wr_addr               = mr_wr_addr;
    ddr_config_write_csr_node(node,
			      CVMX_LMCX_MR_MPR_CTL(ddr_interface_num),
			      lmc_mr_mpr_ctl.u64);
}

static void set_DRAM_output_inversion (uint32_t cpu_id,
                                       int ddr_interface_num,
                                       int dimm_count,
                                       int rank_mask,
                                       int node,
                                       int inversion)
{
    cvmx_lmcx_ddr4_dimm_ctl_t lmc_ddr4_dimm_ctl;
    cvmx_lmcx_dimmx_params_t lmc_dimmx_params;
    cvmx_lmcx_dimm_ctl_t lmc_dimm_ctl;
    int dimm_no;

    lmc_ddr4_dimm_ctl.u64 = 0;  /* Don't touch extenced register control words */
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_DDR4_DIMM_CTL(ddr_interface_num),
                              lmc_ddr4_dimm_ctl.u64);

    ddr_print("All DIMMs: Register Control Word          RC0 : %x\n", (inversion & 1));

    for (dimm_no=0; dimm_no < dimm_count; ++dimm_no) {

        lmc_dimmx_params.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMMX_PARAMS(dimm_no, ddr_interface_num));
        lmc_dimmx_params.s.rc0  = (lmc_dimmx_params.s.rc0 & ~1) | (inversion & 1);

        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_DIMMX_PARAMS(dimm_no, ddr_interface_num),
                                  lmc_dimmx_params.u64);
    }

    /* LMC0_DIMM_CTL */
    lmc_dimm_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num));
    lmc_dimm_ctl.s.dimm0_wmask         = 0x1;
    lmc_dimm_ctl.s.dimm1_wmask         = (dimm_count > 1) ? 0x0001 : 0x0000;

    ddr_print("LMC DIMM_CTL                                  : 0x%016llx\n", lmc_dimm_ctl.u64);
    ddr_config_write_csr_node(node,
                              CVMX_LMCX_DIMM_CTL(ddr_interface_num),
                              lmc_dimm_ctl.u64);

    perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
                                  ddr_interface_num, 0x7 ); /* Init RCW */
}

static void write_mpr_page0_pattern (uint32_t cpu_id, int node, int rank_mask,
                                     int ddr_interface_num, int dimm_count, int pattern, int location_mask)
{
    int rankx;
    int location;

    for (rankx = 0; rankx < dimm_count*4; rankx++) {
        if (!(rank_mask & (1 << rankx)))
            continue;
        for (location = 0; location < 4; ++location) {
            if (!(location_mask & (1 << location)))
                continue;

            ddr4_mpr_write(cpu_id, node, ddr_interface_num, rankx,
                           /* page */ 0, /* location */ location, pattern);
        }
    }
}

static void change_rdimm_mpr_pattern (uint32_t cpu_id, int node, int rank_mask,
                                      int ddr_interface_num, int dimm_count)
{
    int save_ref_zqcs_int;
    cvmx_lmcx_config_t  lmc_config;

    /*
      Okay, here is the latest sequence.  This should work for all
      chips and passes (78,88,73,etc).  This sequence should be run
      immediately after DRAM INIT.  The basic idea is to write the
      same pattern into each of the 4 MPR locations in the DRAM, so
      that the same value is returned when doing MPR reads regardless
      of the inversion state.  My advice is to put this into a
      function, change_rdimm_mpr_pattern or something like that, so
      that it can be called multiple times, as I think David wants a
      clock-like pattern for OFFSET training, but does not want a
      clock pattern for Bit-Deskew.  You should then be able to call
      this at any point in the init sequence (after DRAM init) to
      change the pattern to a new value.
      Mike

      A correction: PHY doesn't need any pattern during offset
      training, but needs clock like pattern for internal vref and
      bit-dskew training.  So for that reason, these steps below have
      to be conducted before those trainings to pre-condition
      the pattern.  David

      Note: Step 3, 4, 8 and 9 have to be done through RDIMM
      sequence. If you issue MRW sequence to do RCW write (in o78 pass
      1 at least), LMC will still do two commands because
      CONTROL[RDIMM_ENA] is still set high. We don't want it to have
      any unintentional mode register write so it's best to do what
      Mike is doing here.
      Andrew
    */


    /* 1) Disable refresh (REF_ZQCS_INT = 0) */

    debug_print("1) Disable refresh (REF_ZQCS_INT = 0)\n");

    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
    save_ref_zqcs_int         = lmc_config.cn78xx.ref_zqcs_int;
    lmc_config.cn78xx.ref_zqcs_int = 0;
    ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num),
                              lmc_config.u64);


    /* 2) Put all devices in MPR mode (Run MRW sequence (sequence=8)
       with MODEREG_PARAMS0[MPRLOC]=0,
       MODEREG_PARAMS0[MPR]=1, MR_MPR_CTL[MR_WR_SEL]=3, and
       MR_MPR_CTL[MR_WR_USE_DEFAULT_VALUE]=1) */

    debug_print("2) Put all devices in MPR mode (Run MRW sequence (sequence=8)\n");

    set_mpr_mode (cpu_id, node, rank_mask, ddr_interface_num, dimm_count, /* mpr */ 1, /* bg1 */ 0); /* A-side */
    set_mpr_mode (cpu_id, node, rank_mask, ddr_interface_num, dimm_count, /* mpr */ 1, /* bg1 */ 1); /* B-side */

    /*    a. Or you can set MR_MPR_CTL[MR_WR_USE_DEFAULT_VALUE]=0 and set
          the value you would like directly into
          MR_MPR_CTL[MR_WR_ADDR] */

    /* 3) Disable RCD Parity (if previously enabled) - parity does not
       work if inversion disabled */

    debug_print("3) Disable RCD Parity\n");

    /* 4) Disable Inversion in the RCD. */
    /*    a. I did (3&4) via the RDIMM sequence (seq_sel=7), but it
          may be easier to use the MRW sequence (seq_sel=8).  Just set
          MR_MPR_CTL[MR_WR_SEL]=7, MR_MPR_CTL[MR_WR_ADDR][3:0]=data,
          MR_MPR_CTL[MR_WR_ADDR][7:4]=RCD reg */

    debug_print("4) Disable Inversion in the RCD.\n");

    set_DRAM_output_inversion (cpu_id, ddr_interface_num, dimm_count, rank_mask, node,
                               1 /* 1=disable output inversion*/);

    /* 5) Disable CONTROL[RDIMM_ENA] so that MR sequence goes out
       non-inverted.  */

    debug_print("5) Disable CONTROL[RDIMM_ENA]\n");

    set_rdimm_mode(node, ddr_interface_num, 0);

    /* 6) Write all 4 MPR registers with the desired pattern (have to
       do this for all enabled ranks) */
    /*    a. MR_MPR_CTL.MPR_WR=1, MR_MPR_CTL.MPR_LOC=0..3,
          MR_MPR_CTL.MR_WR_SEL=0, MR_MPR_CTL.MR_WR_ADDR[7:0]=pattern */

    debug_print("6) Write all 4 MPR page 0 Training Patterns\n");

    write_mpr_page0_pattern (cpu_id, node, rank_mask,
                             ddr_interface_num, dimm_count, 0x55, 0x8);

    /* 7) Re-enable RDIMM_ENA */

    debug_print("7) Re-enable RDIMM_ENA\n");

    set_rdimm_mode(node, ddr_interface_num, 1);

    /* 8) Re-enable RDIMM inversion */

    debug_print("8) Re-enable RDIMM inversion\n");

    set_DRAM_output_inversion (cpu_id, ddr_interface_num, dimm_count, rank_mask, node,
                               0 /* 0=re-enable output inversion*/);

    /* 9) Re-enable RDIMM parity (if desired) */

    debug_print("9) Re-enable RDIMM parity (if desired)\n");

    /* 10)Take B-side devices out of MPR mode (Run MRW sequence
       (sequence=8) with MODEREG_PARAMS0[MPRLOC]=0,
       MODEREG_PARAMS0[MPR]=0, MR_MPR_CTL[MR_WR_SEL]=3, and
       MR_MPR_CTL[MR_WR_USE_DEFAULT_VALUE]=1) */

    debug_print("10)Take B-side devices out of MPR mode\n");

    set_mpr_mode (cpu_id, node, rank_mask, ddr_interface_num, dimm_count, /* mpr */ 0, /* bg1 */ 1);

    /*    a. Or you can set MR_MPR_CTL[MR_WR_USE_DEFAULT_VALUE]=0 and
          set the value you would like directly into
          MR_MPR_CTL[MR_WR_ADDR] */

    /* 11)Re-enable refresh (REF_ZQCS_INT=previous value) */

    debug_print("11)Re-enable refresh (REF_ZQCS_INT=previous value)\n");

    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
    lmc_config.cn78xx.ref_zqcs_int = save_ref_zqcs_int;
    ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num),
                              lmc_config.u64);

}

#if 0
// entry = 1 is valid, entry = 0 is invalid
static int
validity_matrix[4][4] = {[0] {1,1,1,0},  // valid pairs when cv == 0: 0,0 + 0,1 + 0,2 == "7"
			 [1] {0,1,1,1},  // valid pairs when cv == 1: 1,1 + 1,2 + 1,3 == "E"
			 [2] {1,0,1,1},  // valid pairs when cv == 2: 2,2 + 2,3 + 2,0 == "D"
			 [3] {1,1,0,1}}; // valid pairs when cv == 3: 3,3 + 3,0 + 3,1 == "B"
#endif
static int
validate_hwl_seq(int *wl, int *seq)
{
    int seqx; // sequence index, step through the sequence array
    int bitnum;
    seqx = 0;
    while (seq[seqx+1] >= 0) { // stop on next seq entry == -1
	// but now, check current versus next
#if 0
	if ( !validity_matrix [wl[seq[seqx]]] [wl[seq[seqx+1]]] )
	    return 1;
#else
	bitnum = (wl[seq[seqx]] << 2) | wl[seq[seqx+1]];
	if (!((1 << bitnum) & 0xBDE7)) // magic validity number (see matrix above)
	    return 1;
#endif
	seqx++;
    }
    return 0;
}

static int
Validate_HW_WL_Settings(int node, int ddr_interface_num,
                        cvmx_lmcx_wlevel_rankx_t *lmc_wlevel_rank,
                        int is_rdimm, int ecc_ena)
{
    int wl[9], byte, errors;

    // arrange the sequences so 
    int useq[] = { 0,1,2,3,8,4,5,6,7,-1 }; // index 0 has byte 0, etc, ECC in middle
    int rseq1[] = { 8,3,2,1,0,-1 }; // index 0 is ECC, then go down
    int rseq2[] = { 4,5,6,7,-1 }; // index 0 has byte 4, then go up
    int useqno[] = { 0,1,2,3,4,5,6,7,-1 }; // index 0 has byte 0, etc, no ECC
    int rseq1no[] = { 3,2,1,0,-1 }; // index 0 is byte 3, then go down, no ECC
    
    // in the CSR, bytes 0-7 are always data, byte 8 is ECC
    for (byte = 0; byte < (8+ecc_ena); byte++) {
	wl[byte] = (get_wlevel_rank_struct(lmc_wlevel_rank, byte) >> 1) & 3; // preprocess :-)
    }

    errors = 0;
    if (is_rdimm) { // RDIMM order
	errors  = validate_hwl_seq(wl, (ecc_ena) ? rseq1 : rseq1no);
	errors += validate_hwl_seq(wl, rseq2);
    } else { // UDIMM order
	errors  = validate_hwl_seq(wl, (ecc_ena) ? useq : useqno);
    }

    return errors;
}

static unsigned int EXTR_WR(uint64_t u, int x)
{
    return (unsigned int)(((u >> (x*12+5)) & 0x3UL) | ((u >> (51+x-2)) & 0x4UL));
}
static void INSRT_WR(uint64_t *up, int x, int v)
{
    uint64_t u = *up;
    u &= ~(((0x3UL) << (x*12+5)) | ((0x1UL) << (51+x)));
    *up = (u | ((v & 0x3UL) << (x*12+5)) | ((v & 0x4UL) << (51+x-2)));
    return;
}

/* Read out Deskew Settings for DDR */

typedef struct {
    uint16_t bits[8];
} deskew_bytes_t;
typedef struct {
    deskew_bytes_t bytes[9];
} deskew_data_t;

static void
Get_Deskew_Settings(int node, int ddr_interface_num,
                     deskew_data_t *dskdat, uint32_t cpu_id)
{
    cvmx_lmcx_phy_ctl_t phy_ctl;
    cvmx_lmcx_config_t  lmc_config;
    int bit_num, bit_index;
    int byte_lane, byte_limit;
    // NOTE: these are for pass 2.x
    int is_o78p2 = !octeon_is_cpuid(OCTEON_CN78XX_PASS1_X);
    int bit_end = (is_o78p2) ? 9 : 8;

    //lmc_config.u = BDK_CSR_READ(node, BDK_LMCX_CONFIG(ddr_interface_num));
    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
    byte_limit = ((!lmc_config.s.mode32b) ? 8 : 4) + lmc_config.s.ecc_ena;

    memset(dskdat, 0, sizeof(*dskdat));

    //BDK_CSR_MODIFY(phy_ctl, node, BDK_LMCX_PHY_CTL(ddr_interface_num),
    //		     phy_ctl.s.dsk_dbg_clk_scaler = 3);
    phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
    phy_ctl.s.dsk_dbg_clk_scaler = 3;
    ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

    for (byte_lane = 0; byte_lane < byte_limit; byte_lane++) {
        bit_index = 0;
	for (bit_num = 0; bit_num <= bit_end; ++bit_num) {	// NOTE: this is for pass 2.x

	    if (bit_num == 4) continue;
	    if ((bit_num == 5) && is_o78p2) continue;	// NOTE: this is for pass 2.x

            // set byte lane and bit to read
            //BDK_CSR_MODIFY(phy_ctl, node, BDK_LMCX_PHY_CTL(ddr_interface_num),
            //		     (phy_ctl.s.dsk_dbg_bit_sel = bit_num,
            //		      phy_ctl.s.dsk_dbg_byte_sel = byte_lane));
            //phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
            phy_ctl.s.dsk_dbg_bit_sel  = bit_num;
            phy_ctl.s.dsk_dbg_byte_sel = byte_lane;
            ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

            // start read sequence
            //BDK_CSR_MODIFY(phy_ctl, node, BDK_LMCX_PHY_CTL(ddr_interface_num),
            //		     phy_ctl.s.dsk_dbg_rd_start = 1);
            phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
            phy_ctl.s.dsk_dbg_rd_start = 1;
            ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

            // poll for read sequence to complete
            do {
                //phy_ctl.u = BDK_CSR_READ(node, BDK_LMCX_PHY_CTL(ddr_interface_num));
                phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
            } while (phy_ctl.s.dsk_dbg_rd_complete != 1);
			
            // record the data
            dskdat->bytes[byte_lane].bits[bit_index] = phy_ctl.s.dsk_dbg_rd_data & 0x3ff;
            bit_index++;

        } /* for (bit_num = 0; bit_num <= bit_end; ++bit_num) */
    } /* for (byte_lane = 0; byte_lane < byte_limit; byte_lane++) */
	
    return;
}

typedef struct {
    int saturated;   // number saturated
    int unlocked;    // number unlocked
    int nibrng_errs; // nibble range errors
    int nibunl_errs; // nibble unlocked errors
    //int nibsat_errs; // nibble saturation errors
    int bitval_errs; // bit value errors
#if LOOK_FOR_STUCK_BYTE
    int bytes_stuck; // byte(s) stuck
#endif
} deskew_counts_t;

#define MIN_BITVAL  17
#define MAX_BITVAL 110


static void Validate_Deskew_Training(uint32_t cpu_id, int node, int rank_mask,
                                     int ddr_interface_num, deskew_counts_t *counts,
                                     int print_flags)
{
    int byte_lane, bit_num, nib_num;
    int nibrng_errs, nibunl_errs, bitval_errs;
    cvmx_lmcx_config_t  lmc_config;
    uint16_t nib_min[2], nib_max[2], nib_unl[2];
    int16_t flags, deskew;
    // NOTE: these are for pass 2.x
    int is_o78p2 = !octeon_is_cpuid(OCTEON_CN78XX_PASS1_X);
    int bit_start = (is_o78p2) ? 9 : 8;
#if LOOK_FOR_STUCK_BYTE
    uint64_t bl_mask[2]; // enough for 128 values
    int bit_values;
#endif
    int print_enable = print_flags & 1;
    deskew_data_t dskdat;    
    int bit_index;
    const char *fc = " ?-=+*#&";
    int bit_last;

    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));

    memset(counts, 0, sizeof(deskew_counts_t));

    Get_Deskew_Settings(node, ddr_interface_num, &dskdat, cpu_id);

    if (print_enable) {
        ddr_print("N%d.LMC%d: Deskew Settings:          Bit =>      :",
		  node, ddr_interface_num);
	for (bit_num = 7; bit_num >= 0; --bit_num) {
            ddr_print(" %3d  ", bit_num);
	}
        ddr_print("\n");
    }

    for (byte_lane = 0; byte_lane < 8+lmc_config.s.ecc_ena; byte_lane++) {

        if (print_enable)
            ddr_print("N%d.LMC%d: Bit Deskew Byte %d %s               :",
                      node, ddr_interface_num, byte_lane,
                      (print_flags & 2) ? "FINAL" : "     ");

	nib_min[0] = 127; nib_min[1] = 127;
	nib_max[0] = 0;   nib_max[1] = 0;
	nib_unl[0] = 0;   nib_unl[1] = 0;

#if LOOK_FOR_STUCK_BYTE
        bl_mask[0] = bl_mask[1] = 0;
#endif

        if ((lmc_config.s.mode32b == 1) && (byte_lane == 4)) {
            bit_index = 3;
            bit_last  = 3;
            if (print_enable)
                ddr_print("                        ");
        } else {
            bit_index = 7;
            bit_last = bit_start;
        }

        for (bit_num = bit_last; bit_num >= 0; --bit_num) {
            if (bit_num == 4) continue;
            if ((bit_num == 5) && is_o78p2) continue;

	    nib_num = (bit_num > 4) ? 1 : 0;

	    flags = dskdat.bytes[byte_lane].bits[bit_index] & 7;
	    deskew = dskdat.bytes[byte_lane].bits[bit_index] >> 3;
            bit_index--;

            counts->saturated += !!(flags & 6);
            counts->unlocked  +=  !(flags & 1);

            nib_unl[nib_num]  +=  !(flags & 1);

            // Do range calc even when locked; it could happen that a bit
            // is still unlocked after final retry, and we want to have
            // an external retry if a RANGE error is present at exit...
            nib_min[nib_num] = min(nib_min[nib_num], deskew);
            nib_max[nib_num] = max(nib_max[nib_num], deskew);

#if LOOK_FOR_STUCK_BYTE
            bl_mask[(deskew >> 6) & 1] |= 1UL << (deskew & 0x3f);
#endif

            if (print_enable)
                ddr_print(" %3d %c", deskew, fc[flags^1]);

        } /* for (bit_num = bit_last; bit_num >= 0; --bit_num) */

	/*
          Now look for nibble errors

	  For bit 55, it looks like a bit deskew problem. When the upper nibble of byte 6
	   needs to go to saturation, bit 7 of byte 6 locks prematurely at 64.
	  For DIMMs with raw card A and B, can we reset the deskew training when we encounter this case?
	  The reset criteria should be looking at one nibble at a time for raw card A and B;
	  if the bit-deskew setting within a nibble is different by > 33, we'll issue a reset
	  to the bit deskew training.

	  LMC0 Bit Deskew Byte(6): 64 0 - 0 - 0 - 26 61 35 64
	*/
	// upper nibble range, then lower nibble range
	nibrng_errs  = ((nib_max[1] - nib_min[1]) > 33) ? 1 : 0;
	nibrng_errs |= ((nib_max[0] - nib_min[0]) > 33) ? 1 : 0;

	// check for nibble all unlocked
	nibunl_errs = ((nib_unl[0] == 4) || (nib_unl[1] == 4)) ? 1 : 0;

	// check for bit value errors, ie < 17 or > 110
        // FIXME? assume max always > MIN_BITVAL and min < MAX_BITVAL
	bitval_errs  = ((nib_max[1] > MAX_BITVAL) || (nib_max[0] > MAX_BITVAL)) ? 1 : 0;
	bitval_errs |= ((nib_min[1] < MIN_BITVAL) || (nib_min[0] < MIN_BITVAL)) ? 1 : 0;
	
	if (((nibrng_errs != 0) || (nibunl_errs != 0) || (bitval_errs != 0))
            && print_enable)
        {
	    ddr_print(" %c%c%c", (nibrng_errs)?'R':' ', (nibunl_errs)?'U':' ', (bitval_errs)?'V':' ');
        }

#if LOOK_FOR_STUCK_BYTE
        bit_values = __builtin_popcountll(bl_mask[0]) + __builtin_popcountll(bl_mask[1]);
        if (bit_values < 3) {
            counts->bytes_stuck |= (1 << byte_lane);
            if (print_enable)
                ddr_print("X");
        }
#endif
        if (print_enable)
            ddr_print("\n");

	counts->nibrng_errs |= (nibrng_errs << byte_lane);
	counts->nibunl_errs |= (nibunl_errs << byte_lane);
	counts->bitval_errs |= (bitval_errs << byte_lane);

#if LOOK_FOR_STUCK_BYTE
        // just for completeness, allow print of the stuck values bitmask after the bytelane print
        if ((bit_values < 3) && print_enable) {
            ddr_print("N%d.LMC%d: Deskew byte %d STUCK on value 0x%016llx.%016llx\n",
                      node, ddr_interface_num, byte_lane, 
                      bl_mask[1], bl_mask[0]);

        }
#endif

    } /* for (byte_lane = 0; byte_lane < 8+lmc_config.s.ecc_ena; byte_lane++) */

    return;
}

unsigned short
load_dac_override(uint32_t cpu_id, int node, int ddr_interface_num,
                  int dac_value, int byte)
{
    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
    int bytex = (byte == 0x0A) ? byte : byte + 1; // single bytelanes incr by 1; A is for ALL

    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    SET_DDR_DLL_CTL3(byte_sel, bytex);
    SET_DDR_DLL_CTL3(offset, dac_value>>1);

    ddr_dll_ctl3.cn73xx.bit_select    = 0x9; /* No-op */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn73xx.bit_select    = 0xC; /* Vref bypass setting load */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn73xx.bit_select    = 0xD; /* Vref bypass on. */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn73xx.bit_select    = 0x9; /* No-op */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    return ((unsigned short) GET_DDR_DLL_CTL3(offset));
}

// arg dac_or_dbi is 1 for DAC, 0 for DBI
// returns 9 entries (bytelanes 0 through 8) in settings[]
// returns 0 if OK, -1 if a problem
int read_DAC_DBI_settings(uint32_t cpu_id, int node, int ddr_interface_num,
                          int dac_or_dbi, int *settings)
{
    cvmx_lmcx_phy_ctl_t phy_ctl;
    int byte_lane, bit_num;
    int deskew;
    int dac_value;
    int new_deskew_layout = 0;

    new_deskew_layout = octeon_is_cpuid(OCTEON_CN73XX)||octeon_is_cpuid(OCTEON_CNF75XX);
    new_deskew_layout |= (octeon_is_cpuid(OCTEON_CN78XX) && (!octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)));

    phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
    phy_ctl.s.dsk_dbg_clk_scaler = 3;
    ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

    bit_num = (dac_or_dbi) ? 4 : 5;
    if ((bit_num == 5) && !new_deskew_layout) { // DBI not available
	return -1;
    }

    for (byte_lane = 8; byte_lane >= 0 ; --byte_lane) { // FIXME: always assume ECC is available

	//set byte lane and bit to read
        phy_ctl.s.dsk_dbg_bit_sel  = bit_num;
        phy_ctl.s.dsk_dbg_byte_sel = byte_lane;
        ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

	//start read sequence
        phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
        phy_ctl.s.dsk_dbg_rd_start = 1;
        ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

	//poll for read sequence to complete
	do {
            phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
	} while (phy_ctl.s.dsk_dbg_rd_complete != 1);
			
	deskew = phy_ctl.s.dsk_dbg_rd_data /*>> 3*/; // keep the flag bits where they are for DBI
	dac_value = phy_ctl.s.dsk_dbg_rd_data & 0xff;

	settings[byte_lane] =  (dac_or_dbi) ? dac_value : deskew;

    } /* for (byte_lane = 8; byte_lane >= 0 ; --byte_lane) { */

    return 0;
}

// print out the DBI settings array
// arg dac_or_dbi is 1 for DAC, 0 for DBI
void
display_DAC_DBI_settings(int node, int lmc, int dac_or_dbi,
                         int ecc_ena, int *settings, char *title)
{
    int byte;
    int flags;
    int deskew;
    const char *fc = " ?-=+*#&";

    ddr_print("N%d.LMC%d: %s %s Deskew Settings %d:0 :",
              node, lmc, title, (dac_or_dbi)?"DAC":"DBI", 7+ecc_ena);
    for (byte = (7+ecc_ena); byte >= 0; --byte) { // FIXME: what about 32-bit mode?
        if (dac_or_dbi) { // DAC
            flags  = 1; // say its locked to get blank
            deskew = settings[byte] & 0xff;
        } else { // DBI
            flags  = settings[byte] & 7;
            deskew = (settings[byte] >> 3) & 0x7f;
        }
        ddr_print(" %3d %c", deskew, fc[flags^1]);
    }
    ddr_print("\n");
}

// Evaluate the DAC settings array
static int
evaluate_DAC_settings(int ddr_interface_64b, int ecc_ena, int *settings)
{
    int byte, dac;
    int last = (ddr_interface_64b) ? 7 : 3;

    // this looks only for DAC values that are EVEN
    for (byte = (last+ecc_ena); byte >= 0; --byte) {
        dac  = settings[byte] & 0xff;
        if ((dac & 1) == 0)
            return 1;
    }
    return 0;
}

static void
Perform_Offset_Training(int node, int rank_mask, int ddr_interface_num, uint32_t cpu_id)
{
    cvmx_lmcx_phy_ctl_t lmc_phy_ctl;
    uint64_t orig_phy_ctl;
    const char *s;

    /*
     * 4.8.6 LMC Offset Training
     *
     * LMC requires input-receiver offset training.
     *
     * 1. Write LMC(0)_PHY_CTL[DAC_ON] = 1
     */
    lmc_phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
    orig_phy_ctl = lmc_phy_ctl.u64;
    lmc_phy_ctl.s.dac_on = 1;

    // allow full CSR override
    if ((s = lookup_env_parameter_ull("ddr_phy_ctl")) != NULL) {
        lmc_phy_ctl.u64 = strtoull(s, NULL, 0);
    }

    // do not print or write if CSR does not change...
    if (lmc_phy_ctl.u64 != orig_phy_ctl) {
        ddr_print("PHY_CTL                                       : 0x%016llx\n", lmc_phy_ctl.u64);
        ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), lmc_phy_ctl.u64);
    }

#if 0
    // FIXME? do we really need to show RODT here?
    {
        cvmx_lmcx_comp_ctl2_t lmc_comp_ctl2;
        lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));

        ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                  lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);
    }
#endif

    /*
     * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0B and
     *    LMC(0)_SEQ_CTL[INIT_START] = 1.
     *
     * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
     */
    /* Start Offset training sequence */
    perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0B);

}

static void
Perform_Internal_VREF_Training(int node, int rank_mask, int ddr_interface_num, uint32_t cpu_id)
{
    cvmx_lmcx_ext_config_t ext_config;
    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;

    // First, make sure all byte-lanes are out of VREF bypass mode
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    ddr_dll_ctl3.cn78xx.byte_sel   = 0x0A; /* all byte-lanes */
    ddr_dll_ctl3.cn78xx.bit_select = 0x09; /* No-op */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn78xx.bit_select = 0x0E; /* Vref bypass off. */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn78xx.bit_select = 0x09; /* No-op */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num), ddr_dll_ctl3.u64);

    /*
     * 4.8.7 LMC Internal Vref Training
     *
     * LMC requires input-reference-voltage training.
     *
     * 1. Write LMC(0)_EXT_CONFIG[VREFINT_SEQ_DESKEW] = 0.
     */
    ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
    ext_config.s.vrefint_seq_deskew = 0;

    ddr_seq_print("Performing LMC sequence: vrefint_seq_deskew = %d\n",
                  ext_config.s.vrefint_seq_deskew);

    ddr_config_write_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
                              ext_config.u64);

    /*
     * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0a and
     *    LMC(0)_SEQ_CTL[INIT_START] = 1.
     *
     * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
     */
    /* Start LMC Internal Vref Training */
    perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0A);
}

#define dbg_avg(format, ...) debug_print(format, ##__VA_ARGS__)
static int
process_samples_average(int16_t *bytes, int num_samples, int lmc, int lane_no)
{
    int i, sadj, sum = 0, ret, asum, trunc;
    int16_t smin = 32767, smax = -32768;

    dbg_avg("DBG_AVG%d.%d: ", lmc, lane_no);

    for (i = 0; i < num_samples; i++) {
        sum += bytes[i];
        if (bytes[i] < smin) smin = bytes[i];
        if (bytes[i] > smax) smax = bytes[i];
        dbg_avg(" %3d", bytes[i]);
    }

    dbg_avg(" (%3d, %3d)", smin, smax);

    asum = sum - smin - smax;

    sadj = divide_nint(asum * 10, (num_samples - 2));

    trunc = asum / (num_samples - 2);

    dbg_avg(" [%3d.%d, %3d]", sadj/10, sadj%10, trunc);

    sadj = divide_nint(sadj, 10);
    if (trunc & 1)
        ret = trunc;
    else if (sadj & 1)
        ret = sadj;
    else 
        ret = trunc + 1;

    dbg_avg(" -> %3d\n", ret);

    return ret;
}


#define DEFAULT_SAT_RETRY_LIMIT    11       // 1 + 10 retries

#if defined(__U_BOOT__)
#define default_lock_retry_limit   20       // 20 retries
#define deskew_validation_delay    10000   // 10 millisecs
#else
static int default_lock_retry_limit = 20;   // 20 retries; a var for debug overriding
static int deskew_validation_delay = 10000; // 10 millisecs; a var for debug overriding
#endif

static int Perform_Deskew_Training(uint32_t cpu_id, int node, int rank_mask,
                                   int ddr_interface_num, int spd_rawcard_AorB)
{
    int unsaturated, locked;
    int sat_retries, sat_retries_limit;
    int lock_retries, lock_retries_total, lock_retries_limit;
    int print_first;
    int print_them_all;
    deskew_counts_t dsk_counts;
    cvmx_lmcx_phy_ctl_t phy_ctl;
#if LOOK_FOR_STUCK_BYTE
    // provide override for STUCK BYTE RESETS
    int do_stuck_reset = ENABLE_STUCK_BYTE_RESET;
#endif
    char *s;
    int has_no_sat = octeon_is_cpuid(OCTEON_CN78XX_PASS2_X) || octeon_is_cpuid(OCTEON_CNF75XX);
    int disable_bitval_retries = 1; // default to disabled

    ddr_print("N%d.LMC%d: Performing Deskew Training.\n", node, ddr_interface_num);

    sat_retries = 0;
    sat_retries_limit = (has_no_sat) ? 5 : DEFAULT_SAT_RETRY_LIMIT;

    lock_retries_total = 0;
    unsaturated = 0;
    print_first = 1; // print the first one
    print_them_all = 0; // set to true for printing all normal deskew attempts

#if LOOK_FOR_STUCK_BYTE
    // provide override for STUCK BYTE RESETS
    if ((s = getenv("ddr_enable_stuck_byte_reset")) != NULL) {
	do_stuck_reset = !!simple_strtoul(s, NULL, 0);
    }
#endif

    // provide override for bitval_errs causing internal VREF retries
    if ((s = getenv("ddr_disable_bitval_retries")) != NULL) {
	disable_bitval_retries = !!simple_strtoul(s, NULL, 0);
    }

    lock_retries_limit = default_lock_retry_limit;
    if ((octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)) ||
        (octeon_is_cpuid(OCTEON_CN73XX)) ||
        (octeon_is_cpuid(OCTEON_CNF75XX)))
    {
        lock_retries_limit *= 2; // give new chips twice as many
    }

    do { /* while (sat_retries < sat_retry_limit) */

        /*
         * 4.8.8 LMC Deskew Training
         *
         * LMC requires input-read-data deskew training.
         *
         * 1. Write LMC(0)_EXT_CONFIG[VREFINT_SEQ_DESKEW] = 1.
         */

        {
            cvmx_lmcx_ext_config_t ext_config;
            //ext_config.u = BDK_CSR_READ(node, BDK_LMCX_EXT_CONFIG(ddr_interface_num));
            ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
            ext_config.s.vrefint_seq_deskew = 1;

	    ddr_seq_print("Performing LMC sequence: vrefint_seq_deskew = %d\n",
			  ext_config.s.vrefint_seq_deskew);

            //DRAM_CSR_WRITE(node, BDK_LMCX_EXT_CONFIG(ddr_interface_num), ext_config.u);
	    ddr_config_write_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num), ext_config.u64);
        }

        /*
         * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0A and
         *    LMC(0)_SEQ_CTL[INIT_START] = 1.
         *
         * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
         */

        //BDK_CSR_MODIFY(phy_ctl, node, BDK_LMCX_PHY_CTL(ddr_interface_num),
        //               phy_ctl.s.phy_dsk_reset = 1); /* Reset Deskew sequence */
	phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
	phy_ctl.s.phy_dsk_reset = 1; /* RESET Deskew sequence */
	ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

        perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0A); /* LMC Deskew Training */

	lock_retries = 0;

    perform_deskew_training:
        //BDK_CSR_MODIFY(phy_ctl, node, BDK_LMCX_PHY_CTL(ddr_interface_num),
        //               phy_ctl.s.phy_dsk_reset = 0);
	phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
	phy_ctl.s.phy_dsk_reset = 0; /* Normal Deskew sequence */
	ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

        perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0A); /* LMC Deskew Training */

        // Moved this from Validate_Deskew_Training
        /* Allow deskew results to stabilize before evaluating them. */
        cvmx_wait_usec(deskew_validation_delay);

        // Now go look at lock and saturation status...
        Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, &dsk_counts, print_first);
	if (print_first && !print_them_all) // after printing the first and not doing them all, no more
	    print_first = 0;

        unsaturated = (dsk_counts.saturated == 0);
	locked = (dsk_counts.unlocked == 0);

	// only do locking retries if unsaturated or rawcard A or B, otherwise full SAT retry
	if (unsaturated || (spd_rawcard_AorB && !has_no_sat)) {
	    if (!locked) { // and not locked
		lock_retries++;
		lock_retries_total++;
		if (lock_retries <= lock_retries_limit) {
		    goto perform_deskew_training;
		} else {
		    ddr_print("N%d.LMC%d: LOCK RETRIES failed after %d retries\n",
                              node, ddr_interface_num, lock_retries_limit);
		}
	    } else {
		if (lock_retries_total > 0) // only print if we did try
		    ddr_print("N%d.LMC%d: LOCK RETRIES successful after %d retries\n",
                              node, ddr_interface_num, lock_retries);
	    }
	} /* if (unsaturated || spd_rawcard_AorB) */

        ++sat_retries;

#if LOOK_FOR_STUCK_BYTE
        // FIXME: this is a bit of a hack at the moment...
        // We want to force a Deskew RESET hopefully to unstick the bytes values
        // and then resume normal deskew training as usual.
        // For now, do only if it is all locked...
        if (locked && (dsk_counts.bytes_stuck != 0)) {
            cvmx_lmcx_config_t lmc_config;
            lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
            if (do_stuck_reset && lmc_config.s.mode_x4dev) { // FIXME: only when x4!!
                unsaturated = 0; // to always make sure the while continues
                ddr_print("N%d.LMC%d: STUCK BYTES (0x%x), forcing deskew RESET\n",
                          node, ddr_interface_num, dsk_counts.bytes_stuck);
                continue; // bypass the rest to get back to the RESET
            } else {
                ddr_print("N%d.LMC%d: STUCK BYTES (0x%x), ignoring deskew RESET\n",
                          node, ddr_interface_num, dsk_counts.bytes_stuck);
            }
        }
#endif
	/*
	 * At this point, check for a DDR4 RDIMM that will not benefit from SAT retries; if so, exit
	 */
	if (spd_rawcard_AorB && !has_no_sat) { //
	    ddr_print("N%d.LMC%d: Deskew Training Loop: Exiting for RAWCARD == A or B.\n",
		      node, ddr_interface_num);
	    break; // no sat or lock retries
	}

    } while (!unsaturated && (sat_retries < sat_retries_limit));

    ddr_print("N%d.LMC%d: Deskew Training %s. %d sat-retries, %d lock-retries\n",
              node, ddr_interface_num,
              (sat_retries >= DEFAULT_SAT_RETRY_LIMIT) ? "Timed Out" : "Completed",
              sat_retries-1, lock_retries_total);

    // FIXME? add saturation to reasons for fault return - give it a chance via Internal VREF
    // FIXME? add OPTIONAL bit value to reasons for fault return - give it a chance via Internal VREF
    if ((dsk_counts.nibrng_errs != 0) ||
        (dsk_counts.nibunl_errs != 0) ||
        ((dsk_counts.bitval_errs != 0) && !disable_bitval_retries) ||
        !unsaturated)
    {
        debug_print("N%d.LMC%d: Nibble or Saturation Error(s) found, returning FAULT\n",
                  node, ddr_interface_num);
#if 1
        // FIXME: do we want this output always for errors? 
        Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, &dsk_counts, 1);
#endif
        return -1; // we did retry locally, they did not help
    }

    // NOTE: we (currently) always print one last training validation before starting Read Leveling...

    return 0;
}

static void Write_Deskew_Config(uint32_t cpu_id, int node, int rank_mask, int ddr_interface_num)
{

    /*
    **
    ** Reuse read bit-deskew settings for write bit-deskew.
    **
    ** 1) Run read bit deskew and make sure everything is
    **    locked and valid.
    **
    ** 2) Enable write_bit_deskew:
    **
    **    a.  DLL_CTL3.BIT_SELECT.set(10);   // Enable reuse read deskew settings
    **    b.  DLL_CTL3.BYTE_SEL.set(10);     // Select all bytes
    **    c.  DLL_CTL3.WR_DESKEW_ENA.set(1); // Enable write bit-deskew
    **    d.  Then write DLL_CTL3
    **    e.  DLL_CTL3.WR_DESKEW_LD.set(1);  // go
    **    f.  Then write DLL_CTL3
    **
    ** 3) Now that write-bit-deskew is on,  re-run Write and Read leveling.
    **
    */

    cvmx_lmcx_dll_ctl3_t ddr_dll_ctl3;
    ddr_dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

    ddr_dll_ctl3.cn73xx.bit_select    = 0xA; /* Enable reuse read deskew settings */
    ddr_dll_ctl3.cn73xx.byte_sel      = 0xA; /* Select all bytes */
    ddr_dll_ctl3.cn73xx.wr_deskew_ena = 0x1;  /* Enable write bit-deskew */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);

    ddr_dll_ctl3.cn73xx.wr_deskew_ld  = 1;  /* go */
    ddr_config_write_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num),	ddr_dll_ctl3.u64);
}


#define SCALING_FACTOR (1000)
// NOTE: this gets called for 1-rank and 2-rank DIMMs in single-slot config
static int compute_Vref_1slot_2rank(int rtt_wr, int rtt_park, int dqx_ctl, int rank_count)
{
    uint64_t Reff_s;
    uint64_t Rser_s = 15;
    uint64_t Vdd = 1200;
    uint64_t Vref;
    uint64_t rtt_wr_s = (((rtt_wr == 0) || rtt_wr == 99) ? 1*1024*1024 : rtt_wr); // 99 == HiZ
    uint64_t rtt_park_s = (((rtt_park == 0) || ((rank_count == 1) && (rtt_wr != 0))) ? 1*1024*1024 : rtt_park);
    uint64_t dqx_ctl_s = (dqx_ctl == 0 ? 1*1024*1024 : dqx_ctl);
    int Vref_value;
    uint64_t Rangepc = 6000; // range1 base
    uint64_t Vrefpc;
    int Vref_range = 0;

    //printf("rtt_wr = %d, rtt_park = %d, dqx_ctl = %d\n", rtt_wr, rtt_park, dqx_ctl);
    //printf("rtt_wr_s = %d, rtt_park_s = %d, dqx_ctl_s = %d\n", rtt_wr_s, rtt_park_s, dqx_ctl_s);

    Reff_s = divide_nint((rtt_wr_s * rtt_park_s) , (rtt_wr_s + rtt_park_s));
    //printf("Reff_s = %d\n", Reff_s);

    //Vl = (((Rser_s + dqx_ctl_s) * SCALING_FACTOR) / (Rser_s + dqx_ctl_s + Reff_s)) * Vdd / SCALING_FACTOR;
    //printf("Vl = %d\n", Vl);

    Vref = (((Rser_s + dqx_ctl_s) * SCALING_FACTOR) / (Rser_s + dqx_ctl_s + Reff_s)) + SCALING_FACTOR;
    //printf("Vref = %d\n", Vref);

    Vref = (Vref * Vdd) / 2 / SCALING_FACTOR;
    //printf("Vref = %d\n", Vref);

    Vrefpc = (Vref * 100 * 100) / Vdd;
    //printf("Vrefpc = %d\n", Vrefpc);

    if (Vrefpc < Rangepc) { // < range1 base, use range2
	Vref_range = 1 << 6; // set bit A6 for range2
	Rangepc = 4500; // range2 base is 45%
    }

    Vref_value = divide_nint(Vrefpc - Rangepc, 65);
    if (Vref_value < 0)
	Vref_value = Vref_range; // set to base of range
    else
	Vref_value |= Vref_range;
    //printf("Vref_value = %d (0x%02x)\n", Vref_value, Vref_value);

    debug_print("rtt_wr: %d, rtt_park: %d, dqx_ctl: %d, rank_count: %d\n",
              rtt_wr, rtt_park, dqx_ctl, rank_count);
    debug_print("rtt_wr_s: %lld, rtt_park_s: %lld, dqx_ctl_s: %lld, Vref_value: 0x%x, range: %d\n",
              rtt_wr_s, rtt_park_s, dqx_ctl_s, Vref_value ^ Vref_range, Vref_range ? 2 : 1);

    return Vref_value;
}

// NOTE: this gets called for 1-rank and 2-rank DIMMs in two-slot configs
static int compute_Vref_2slot_2rank(int rtt_wr, int rtt_park_00, int rtt_park_01, int dqx_ctl, int rtt_nom)
{
    //uint64_t Rser = 15;
    uint64_t Vdd = 1200;
    //uint64_t Vref;
    uint64_t Vl, Vlp, Vcm;
    uint64_t Rd0, Rd1, Rpullup;
    uint64_t rtt_wr_s = (((rtt_wr == 0) || rtt_wr == 99) ? 1*1024*1024 : rtt_wr); // 99 == HiZ
    uint64_t rtt_park_00_s = (rtt_park_00 == 0 ? 1*1024*1024 : rtt_park_00);
    uint64_t rtt_park_01_s = (rtt_park_01 == 0 ? 1*1024*1024 : rtt_park_01);
    uint64_t dqx_ctl_s = (dqx_ctl == 0 ? 1*1024*1024 : dqx_ctl);
    uint64_t rtt_nom_s = (rtt_nom == 0 ? 1*1024*1024 : rtt_nom);
    int Vref_value;
    uint64_t Rangepc = 6000; // range1 base
    uint64_t Vrefpc;
    int Vref_range = 0;

    // Rd0 = (RTT_NOM /*parallel*/ RTT_WR) + 15 = ((RTT_NOM * RTT_WR) / (RTT_NOM + RTT_WR)) + 15
    Rd0 = divide_nint((rtt_nom_s * rtt_wr_s), (rtt_nom_s + rtt_wr_s)) + 15;
    //printf("Rd0 = %ld\n", Rd0);

    // Rd1 = (RTT_PARK_00 /*parallel*/ RTT_PARK_01) + 15 = ((RTT_PARK_00 * RTT_PARK_01) / (RTT_PARK_00 + RTT_PARK_01)) + 15
    Rd1 = divide_nint((rtt_park_00_s * rtt_park_01_s), (rtt_park_00_s + rtt_park_01_s)) + 15;
    //printf("Rd1 = %ld\n", Rd1);

    // Rpullup = Rd0 /*parallel*/ Rd1 = (Rd0 * Rd1) / (Rd0 + Rd1)
    Rpullup = divide_nint((Rd0 * Rd1), (Rd0 + Rd1));
    //printf("Rpullup = %ld\n", Rpullup);

    // Vl = (DQX_CTL / (DQX_CTL + Rpullup)) * 1.2
    Vl = divide_nint((dqx_ctl_s * Vdd), (dqx_ctl_s + Rpullup));
    //printf("Vl = %ld\n", Vl);

    // Vlp = ((15 / Rd0) * (1.2 - Vl)) + Vl
    Vlp = divide_nint((15 * (Vdd - Vl)), Rd0) + Vl;
    //printf("Vlp = %ld\n", Vlp);

    // Vcm = (Vlp + 1.2) / 2
    Vcm = divide_nint((Vlp + Vdd), 2);
    //printf("Vcm = %ld\n", Vcm);

    // Vrefpc = (Vcm / 1.2) * 100
    Vrefpc = divide_nint((Vcm * 100 * 100), Vdd);
    //printf("Vrefpc = %ld\n", Vrefpc);

    if (Vrefpc < Rangepc) { // < range1 base, use range2
	Vref_range = 1 << 6; // set bit A6 for range2
	Rangepc = 4500; // range2 base is 45%
    }

    Vref_value = divide_nint(Vrefpc - Rangepc, 65);
    if (Vref_value < 0)
	Vref_value = Vref_range; // set to base of range
    else
	Vref_value |= Vref_range;
    //printf("Vref_value = %d (0x%02x)\n", Vref_value, Vref_value);

    debug_print("rtt_wr:%d, rtt_park_00:%d, rtt_park_01:%d, dqx_ctl:%d, rtt_nom:%d, Vref_value:%d (0x%x)\n",
	   rtt_wr, rtt_park_00, rtt_park_01, dqx_ctl, rtt_nom, Vref_value, Vref_value);

    return Vref_value;
}
// NOTE: only call this for DIMMs with 1 or 2 ranks, not 4.
int
compute_vref_value(int node, int ddr_interface_num, int rankx,
		   int dimm_count, int rank_count, impedence_values_t *imp_values,
                   int is_stacked_die, uint32_t cpu_id)
{
    int computed_final_vref_value = 0;

    /* Calculate an override of the measured Vref value
       but only for configurations we know how to...*/
    // we have code for 2-rank DIMMs in both 1-slot or 2-slot configs,
    // and can use the 2-rank 1-slot code for 1-rank DIMMs in 1-slot configs
    // and can use the 2-rank 2-slot code for 1-rank DIMMs in 2-slot configs

    int rtt_wr, dqx_ctl, rtt_nom, index;
    cvmx_lmcx_modereg_params1_t lmc_modereg_params1;
    cvmx_lmcx_modereg_params2_t lmc_modereg_params2;
    cvmx_lmcx_comp_ctl2_t comp_ctl2;
    int rtt_park;
    int rtt_park_00;
    int rtt_park_01;

    lmc_modereg_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));
    lmc_modereg_params2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS2(ddr_interface_num));
    comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
    dqx_ctl = imp_values->dqx_strength[comp_ctl2.s.dqx_ctl];

    // WR always comes from the current rank
    index   = (lmc_modereg_params1.u64 >> (rankx * 12 + 5)) & 0x03;
    if (!octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
        index |= lmc_modereg_params1.u64 >> (51+rankx-2) & 0x04;
    }
    rtt_wr  = imp_values->rtt_wr_ohms [index];

    // separate calculations for 1 vs 2 DIMMs per LMC
    if (dimm_count == 1) {
	// PARK comes from this rank if 1-rank, otherwise other rank
	index = (lmc_modereg_params2.u64 >> ((rankx ^ (rank_count - 1)) * 10 + 0)) & 0x07;
	rtt_park   = imp_values->rtt_nom_ohms[index];
	computed_final_vref_value = compute_Vref_1slot_2rank(rtt_wr, rtt_park, dqx_ctl, rank_count);
    } else {
	// get both PARK values from the other DIMM
	index = (lmc_modereg_params2.u64 >> ((rankx ^ 0x02) * 10 + 0)) & 0x07;
	rtt_park_00 = imp_values->rtt_nom_ohms[index];
	index = (lmc_modereg_params2.u64 >> ((rankx ^ 0x03) * 10 + 0)) & 0x07;
	rtt_park_01 = imp_values->rtt_nom_ohms[index];
	// NOM comes from this rank if 1-rank, otherwise other rank
	index   = (lmc_modereg_params1.u64 >> ((rankx ^ (rank_count - 1)) * 12 + 9)) & 0x07;
	rtt_nom = imp_values->rtt_nom_ohms[index];
	computed_final_vref_value = compute_Vref_2slot_2rank(rtt_wr, rtt_park_00, rtt_park_01, dqx_ctl, rtt_nom);
    }

#if ENABLE_COMPUTED_VREF_ADJUSTMENT
    {
        cvmx_lmcx_config_t lmc_config;
        cvmx_lmcx_control_t lmc_control;
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
	lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));

	/*
	  New computed Vref = existing computed Vref – X
 
	  The value of X is depending on different conditions. Both #122 and #139 are 2Rx4 RDIMM,
	  while #124 is stacked die 2Rx4, so I conclude the results into two conditions:

	  1. Stacked Die: 2Rx4
	     1-slot: offset = 7. i, e New computed Vref = existing computed Vref – 7
	     2-slot: offset = 6

          2. Regular: 2Rx4
             1-slot: offset = 3
	     2-slot:  offset = 2
	*/
	// we know we never get called unless DDR4, so test just the other conditions
	if((lmc_control.s.rdimm_ena == 1) &&
	   (rank_count == 2) &&
	   (lmc_config.s.mode_x4dev))
	{ // it must first be RDIMM and 2-rank and x4
            int adj;
	    if (is_stacked_die) { // now do according to stacked die or not...
		adj = (dimm_count == 1) ? -7 : -6;
	    } else {
		adj = (dimm_count == 1) ? -3 : -2;
	    }
            // we must have adjusted it, so print it out if verbosity is right
            debug_print("N%d.LMC%d.R%d: adjusting computed vref from %2d (0x%02x) to %2d (0x%02x)\n",
                        node, ddr_interface_num, rankx, 
                        computed_final_vref_value, computed_final_vref_value,
                        computed_final_vref_value + adj, computed_final_vref_value + adj);
            computed_final_vref_value += adj;
	}
    }
#endif /* ENABLE_COMPUTED_VREF_ADJUSTMENT */
    return computed_final_vref_value;
}

static void unpack_rlevel_settings(int ddr_interface_bytemask, int ecc_ena, 
				   rlevel_byte_data_t *rlevel_byte,
				   cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank)
{
    if ((ddr_interface_bytemask & 0xff) == 0xff) {
	if (ecc_ena) {
	    rlevel_byte[8].delay = lmc_rlevel_rank.s.byte7;
	    rlevel_byte[7].delay = lmc_rlevel_rank.s.byte6;
	    rlevel_byte[6].delay = lmc_rlevel_rank.s.byte5;
	    rlevel_byte[5].delay = lmc_rlevel_rank.s.byte4;
	    rlevel_byte[4].delay = lmc_rlevel_rank.s.byte8; /* ECC */
	} else {
	    rlevel_byte[7].delay = lmc_rlevel_rank.s.byte7;
	    rlevel_byte[6].delay = lmc_rlevel_rank.s.byte6;
	    rlevel_byte[5].delay = lmc_rlevel_rank.s.byte5;
	    rlevel_byte[4].delay = lmc_rlevel_rank.s.byte4;
	}
    } else {
	rlevel_byte[8].delay = lmc_rlevel_rank.s.byte8; /* unused */
	rlevel_byte[7].delay = lmc_rlevel_rank.s.byte7; /* unused */
	rlevel_byte[6].delay = lmc_rlevel_rank.s.byte6; /* unused */
	rlevel_byte[5].delay = lmc_rlevel_rank.s.byte5; /* unused */
	rlevel_byte[4].delay = lmc_rlevel_rank.s.byte4; /* ECC */
    }
    rlevel_byte[3].delay = lmc_rlevel_rank.s.byte3;
    rlevel_byte[2].delay = lmc_rlevel_rank.s.byte2;
    rlevel_byte[1].delay = lmc_rlevel_rank.s.byte1;
    rlevel_byte[0].delay = lmc_rlevel_rank.s.byte0;
}

static void pack_rlevel_settings(int ddr_interface_bytemask, int ecc_ena, 
				 rlevel_byte_data_t *rlevel_byte,
				 cvmx_lmcx_rlevel_rankx_t *final_rlevel_rank)
{
    cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank = *final_rlevel_rank; 

    if ((ddr_interface_bytemask & 0xff) == 0xff) {
	if (ecc_ena) {
	    lmc_rlevel_rank.s.byte7 = rlevel_byte[8].delay;
	    lmc_rlevel_rank.s.byte6 = rlevel_byte[7].delay;
	    lmc_rlevel_rank.s.byte5 = rlevel_byte[6].delay;
	    lmc_rlevel_rank.s.byte4 = rlevel_byte[5].delay;
	    lmc_rlevel_rank.s.byte8 = rlevel_byte[4].delay; /* ECC */
	} else {
	    lmc_rlevel_rank.s.byte7 = rlevel_byte[7].delay;
	    lmc_rlevel_rank.s.byte6 = rlevel_byte[6].delay;
	    lmc_rlevel_rank.s.byte5 = rlevel_byte[5].delay;
	    lmc_rlevel_rank.s.byte4 = rlevel_byte[4].delay;
	}
    } else {
	lmc_rlevel_rank.s.byte8 = rlevel_byte[8].delay;
	lmc_rlevel_rank.s.byte7 = rlevel_byte[7].delay;
	lmc_rlevel_rank.s.byte6 = rlevel_byte[6].delay;
	lmc_rlevel_rank.s.byte5 = rlevel_byte[5].delay;
	lmc_rlevel_rank.s.byte4 = rlevel_byte[4].delay;
    }
    lmc_rlevel_rank.s.byte3 = rlevel_byte[3].delay;
    lmc_rlevel_rank.s.byte2 = rlevel_byte[2].delay;
    lmc_rlevel_rank.s.byte1 = rlevel_byte[1].delay;
    lmc_rlevel_rank.s.byte0 = rlevel_byte[0].delay;

    *final_rlevel_rank = lmc_rlevel_rank;
}

/////////////////// These are the RLEVEL settings display routines

// flags
#define WITH_NOTHING 0
#define WITH_SCORE   1
#define WITH_AVERAGE 2
#define WITH_FINAL   4
#define WITH_COMPUTE 8
static void do_display_RL(int node, int ddr_interface_num,
			  cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank,
			  int rank, int flags, int score)
{
    char score_buf[16];
    char *msg_buf;
    char hex_buf[20];

    if (flags & WITH_SCORE)
	snprintf(score_buf, sizeof(score_buf), "(%d)", score);
    else {
	score_buf[0] = ' '; score_buf[1] = 0;
    }

    if (flags & WITH_AVERAGE) {
	msg_buf = "  DELAY AVERAGES  ";
    } else if (flags & WITH_FINAL) {
	msg_buf = "  FINAL SETTINGS  ";
    } else if (flags & WITH_COMPUTE) {
	msg_buf = "  COMPUTED DELAYS ";
    } else {
	snprintf(hex_buf, sizeof(hex_buf), "0x%016llX", (long long unsigned int)lmc_rlevel_rank.u64);
	msg_buf = hex_buf;
    }

    ddr_print("N%d.LMC%d.R%d: Rlevel Rank %#4x, %s  : %5d %5d %5d %5d %5d %5d %5d %5d %5d %s\n",
              node, ddr_interface_num, rank,
              lmc_rlevel_rank.s.status,
              msg_buf,
              lmc_rlevel_rank.s.byte8,
              lmc_rlevel_rank.s.byte7,
              lmc_rlevel_rank.s.byte6,
              lmc_rlevel_rank.s.byte5,
              lmc_rlevel_rank.s.byte4,
              lmc_rlevel_rank.s.byte3,
              lmc_rlevel_rank.s.byte2,
              lmc_rlevel_rank.s.byte1,
              lmc_rlevel_rank.s.byte0,
	      score_buf
              );
}

static inline void
display_RL(int node, int ddr_interface_num, cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank)
{
    do_display_RL(node, ddr_interface_num, lmc_rlevel_rank, rank, 0, 0);
}

static inline void
display_RL_with_score(int node, int ddr_interface_num, cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank, int score)
{
    do_display_RL(node, ddr_interface_num, lmc_rlevel_rank, rank, 1, score);
}

#if !PICK_BEST_RANK_SCORE_NOT_AVG
static inline void
display_RL_with_average(int node, int ddr_interface_num, cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank, int score)
{
    do_display_RL(node, ddr_interface_num, lmc_rlevel_rank, rank, 3, score);
}
#endif

static inline void
display_RL_with_final(int node, int ddr_interface_num, cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank)
{
    do_display_RL(node, ddr_interface_num, lmc_rlevel_rank, rank, 4, 0);
}

static inline void
display_RL_with_computed(int node, int ddr_interface_num, cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank, int score)
{
    do_display_RL(node, ddr_interface_num, lmc_rlevel_rank, rank, 9, score);
}

// flag values
#define WITH_RODT_BLANK      0
#define WITH_RODT_SKIPPING   1
#define WITH_RODT_BESTROW    2
#define WITH_RODT_BESTSCORE  3
// control
#define SKIP_SKIPPING 1

static const char *with_rodt_canned_msgs[4] = { "          ", "SKIPPING  ", "BEST ROW  ", "BEST SCORE" }; 

static void display_RL_with_RODT(int node, int ddr_interface_num,
				 cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank, int rank, int score,
				 int nom_ohms, int rodt_ohms, int flag)
{
    const char *msg_buf;
    char set_buf[20];
#if SKIP_SKIPPING
    if (flag == WITH_RODT_SKIPPING) return;
#endif
    msg_buf = with_rodt_canned_msgs[flag];
    if (nom_ohms < 0) {
        snprintf(set_buf, sizeof(set_buf), "    RODT %3d    ", rodt_ohms);
    } else {
        snprintf(set_buf, sizeof(set_buf), "NOM %3d RODT %3d", nom_ohms, rodt_ohms);        
    }

    ddr_print("N%d.LMC%d.R%d: Rlevel %s   %s  : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d)\n",
	      node, ddr_interface_num, rank,
	      set_buf, msg_buf,
	      lmc_rlevel_rank.s.byte8,
	      lmc_rlevel_rank.s.byte7,
	      lmc_rlevel_rank.s.byte6,
	      lmc_rlevel_rank.s.byte5,
	      lmc_rlevel_rank.s.byte4,
	      lmc_rlevel_rank.s.byte3,
	      lmc_rlevel_rank.s.byte2,
	      lmc_rlevel_rank.s.byte1,
	      lmc_rlevel_rank.s.byte0,
	      score
	      );
}

static void
do_display_WL(int node, int ddr_interface_num,
              cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank,
              int rank, int flags)
{
    char *msg_buf;
    char hex_buf[20];
    //int vbl;
    if (flags & WITH_FINAL) {
	msg_buf = "  FINAL SETTINGS  ";
        //vbl = VBL_NORM;
	ddr_print2("N%d.LMC%d.R%d: Wlevel Rank %#4x, %s  : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
            node, ddr_interface_num, rank,
            lmc_wlevel_rank.s.status,
            msg_buf,
            lmc_wlevel_rank.s.byte8,
            lmc_wlevel_rank.s.byte7,
            lmc_wlevel_rank.s.byte6,
            lmc_wlevel_rank.s.byte5,
            lmc_wlevel_rank.s.byte4,
            lmc_wlevel_rank.s.byte3,
            lmc_wlevel_rank.s.byte2,
            lmc_wlevel_rank.s.byte1,
            lmc_wlevel_rank.s.byte0
            );
    } else {
	snprintf(hex_buf, sizeof(hex_buf), "0x%016llX", (long long unsigned int)lmc_wlevel_rank.u64);
	msg_buf = hex_buf;
        //vbl = VBL_FAE;
	ddr_print("N%d.LMC%d.R%d: Wlevel Rank %#4x, %s  : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
            node, ddr_interface_num, rank,
            lmc_wlevel_rank.s.status,
            msg_buf,
            lmc_wlevel_rank.s.byte8,
            lmc_wlevel_rank.s.byte7,
            lmc_wlevel_rank.s.byte6,
            lmc_wlevel_rank.s.byte5,
            lmc_wlevel_rank.s.byte4,
            lmc_wlevel_rank.s.byte3,
            lmc_wlevel_rank.s.byte2,
            lmc_wlevel_rank.s.byte1,
            lmc_wlevel_rank.s.byte0
            );
    }
}

static inline void
display_WL(int node, int ddr_interface_num, cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank, int rank)
{
    do_display_WL(node, ddr_interface_num, lmc_wlevel_rank, rank, WITH_NOTHING);
}

static inline void
display_WL_with_final(int node, int ddr_interface_num, cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank, int rank)
{
    do_display_WL(node, ddr_interface_num, lmc_wlevel_rank, rank, WITH_FINAL);
}

// pretty-print bitmask adjuster
static uint64_t
PPBM(uint64_t bm)
{
    if (bm != 0ul) {
        while ((bm & 0x0fful) == 0ul)
            bm >>= 4;
    }
    return bm;
}

// xlate PACKED index to UNPACKED index to use with rlevel_byte
#define XPU(i,e) (((i) < 4)?(i):(((i)<8)?(i)+(e):4))
// xlate UNPACKED index to PACKED index to use with rlevel_bitmask
#define XUP(i,e) (((i) < 4)?(i):(e)?(((i)>4)?(i)-1:8):(i))

// flag values
#define WITH_WL_BITMASKS      0
#define WITH_RL_BITMASKS      1
#define WITH_RL_MASK_SCORES   2
#define WITH_RL_SEQ_SCORES    3
static void
do_display_BM(int node, int ddr_interface_num, int rank, void *bm, int flags, int ecc)
{
    if (flags == WITH_WL_BITMASKS) { // wlevel_bitmask array in PACKED index order, so just print them
        int *bitmasks = (int *)bm;

        ddr_print("N%d.LMC%d.R%d: Wlevel Debug Results                  : %05x %05x %05x %05x %05x %05x %05x %05x %05x\n",
              node, ddr_interface_num, rank,
              bitmasks[8],
              bitmasks[7],
              bitmasks[6],
              bitmasks[5],
              bitmasks[4],
              bitmasks[3],
              bitmasks[2],
              bitmasks[1],
              bitmasks[0]
              );
    } else
    if (flags == WITH_RL_BITMASKS) { // rlevel_bitmask array in PACKED index order, so just print them
        rlevel_bitmask_t *rlevel_bitmask = (rlevel_bitmask_t *)bm;
        ddr_print("N%d.LMC%d.R%d: Rlevel Debug Bitmasks        8:0      : %05lx %05lx %05lx %05lx %05lx %05lx %05lx %05lx %05lx\n",
                  node, ddr_interface_num, rank,
                  PPBM(rlevel_bitmask[8].bm),
                  PPBM(rlevel_bitmask[7].bm),
                  PPBM(rlevel_bitmask[6].bm),
                  PPBM(rlevel_bitmask[5].bm),
                  PPBM(rlevel_bitmask[4].bm),
                  PPBM(rlevel_bitmask[3].bm),
                  PPBM(rlevel_bitmask[2].bm),
                  PPBM(rlevel_bitmask[1].bm),
                  PPBM(rlevel_bitmask[0].bm)
                  );
    } else
    if (flags == WITH_RL_MASK_SCORES) { // rlevel_bitmask array in PACKED index order, so just print them
        rlevel_bitmask_t *rlevel_bitmask = (rlevel_bitmask_t *)bm;
        ddr_print("N%d.LMC%d.R%d: Rlevel Debug Bitmask Scores  8:0      : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                  node, ddr_interface_num, rank,
                  rlevel_bitmask[8].errs,
                  rlevel_bitmask[7].errs,
                  rlevel_bitmask[6].errs,
                  rlevel_bitmask[5].errs,
                  rlevel_bitmask[4].errs,
                  rlevel_bitmask[3].errs,
                  rlevel_bitmask[2].errs,
                  rlevel_bitmask[1].errs,
                  rlevel_bitmask[0].errs
                  );
    } else
    if (flags == WITH_RL_SEQ_SCORES) { // rlevel_byte array in UNPACKED index order, so xlate and print them
        rlevel_byte_data_t *rlevel_byte = (rlevel_byte_data_t *)bm;
        ddr_print("N%d.LMC%d.R%d: Rlevel Debug Non-seq Scores  8:0      : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                  node, ddr_interface_num, rank,
                  rlevel_byte[XPU(8,ecc)].sqerrs,
                  rlevel_byte[XPU(7,ecc)].sqerrs,
                  rlevel_byte[XPU(6,ecc)].sqerrs,
                  rlevel_byte[XPU(5,ecc)].sqerrs,
                  rlevel_byte[XPU(4,ecc)].sqerrs,
                  rlevel_byte[XPU(3,ecc)].sqerrs,
                  rlevel_byte[XPU(2,ecc)].sqerrs,
                  rlevel_byte[XPU(1,ecc)].sqerrs,
                  rlevel_byte[XPU(0,ecc)].sqerrs
                  );
    }
}

static inline void
display_WL_BM(int node, int ddr_interface_num, int rank, int *bitmasks)
{
    do_display_BM(node, ddr_interface_num, rank, (void *)bitmasks, WITH_WL_BITMASKS, 0);
}

static inline void
display_RL_BM(int node, int ddr_interface_num, int rank, rlevel_bitmask_t *bitmasks, int ecc_ena)
{
    do_display_BM(node, ddr_interface_num, rank, (void *)bitmasks, WITH_RL_BITMASKS, ecc_ena);
}

static inline void
display_RL_BM_scores(int node, int ddr_interface_num, int rank, rlevel_bitmask_t *bitmasks, int ecc_ena)
{
    do_display_BM(node, ddr_interface_num, rank, (void *)bitmasks, WITH_RL_MASK_SCORES, ecc_ena);
}

static inline void
display_RL_SEQ_scores(int node, int ddr_interface_num, int rank, rlevel_byte_data_t *bytes, int ecc_ena)
{
    do_display_BM(node, ddr_interface_num, rank, (void *)bytes, WITH_RL_SEQ_SCORES, ecc_ena);
}


#define RODT_OHMS_COUNT        8
#define RTT_NOM_OHMS_COUNT     8
#define RTT_NOM_TABLE_COUNT    8
#define RTT_WR_OHMS_COUNT      8
#define DIC_OHMS_COUNT         3
#define DRIVE_STRENGTH_COUNT  15

static unsigned char ddr4_rodt_ohms     [RODT_OHMS_COUNT     ] = {  0,  40,  60, 80, 120, 240, 34, 48 };
static unsigned char ddr4_rtt_nom_ohms  [RTT_NOM_OHMS_COUNT  ] = {  0,  60, 120, 40, 240,  48, 80, 34 };
static unsigned char ddr4_rtt_nom_table [RTT_NOM_TABLE_COUNT ] = {  0,   4,   2,  6,   1,   5,  3,  7 };
static unsigned char ddr4_rtt_wr_ohms   [RTT_WR_OHMS_COUNT   ] = {  0, 120,  240, 99, 80 }; // setting HiZ ohms to 99 for computed vref
static unsigned char ddr4_dic_ohms      [DIC_OHMS_COUNT      ] = { 34,  48 };
static short         ddr4_drive_strength[DRIVE_STRENGTH_COUNT] = {  0,   0, 26, 30, 34, 40, 48, 68, 0,0,0,0,0,0,0 };
static short         ddr4_dqx_strength  [DRIVE_STRENGTH_COUNT] = {  0,  24, 27, 30, 34, 40, 48, 60, 0,0,0,0,0,0,0 };
impedence_values_t ddr4_impedence_values = {
    .rodt_ohms             =  ddr4_rodt_ohms     ,
    .rtt_nom_ohms          =  ddr4_rtt_nom_ohms  ,
    .rtt_nom_table         =  ddr4_rtt_nom_table ,
    .rtt_wr_ohms           =  ddr4_rtt_wr_ohms   ,
    .dic_ohms              =  ddr4_dic_ohms      ,
    .drive_strength        =  ddr4_drive_strength,
    .dqx_strength          =  ddr4_dqx_strength  ,
};

static unsigned char ddr3_rodt_ohms     [RODT_OHMS_COUNT     ] = { 0, 20, 30, 40, 60, 120, 0, 0 };
static unsigned char ddr3_rtt_nom_ohms  [RTT_NOM_OHMS_COUNT  ] = { 0, 60, 120, 40, 20, 30, 0, 0 };
static unsigned char ddr3_rtt_nom_table [RTT_NOM_TABLE_COUNT ] = { 0, 2, 1, 3, 5, 4, 0, 0 };
static unsigned char ddr3_rtt_wr_ohms   [RTT_WR_OHMS_COUNT   ] = { 0, 60, 120 };
static unsigned char ddr3_dic_ohms      [DIC_OHMS_COUNT      ] = { 40, 34 };
static short         ddr3_drive_strength[DRIVE_STRENGTH_COUNT] = { 0, 24, 27, 30, 34, 40, 48, 60, 0,0,0,0,0,0,0 };
static impedence_values_t ddr3_impedence_values = {
    .rodt_ohms             =  ddr3_rodt_ohms     ,
    .rtt_nom_ohms          =  ddr3_rtt_nom_ohms  ,
    .rtt_nom_table         =  ddr3_rtt_nom_table ,
    .rtt_wr_ohms           =  ddr3_rtt_wr_ohms   ,
    .dic_ohms              =  ddr3_dic_ohms      ,
    .drive_strength        =  ddr3_drive_strength,
    .dqx_strength          =  ddr3_drive_strength,
};

uint64_t
hertz_to_psecs(uint64_t hertz)
{
    return divide_nint((uint64_t) 1000*1000*1000*1000, hertz); /* Clock in psecs */
}

#define DIVIDEND_SCALE 1000      /* Scale to avoid rounding error. */
uint64_t
psecs_to_mts(uint64_t psecs)
{
    //ddr_print("psecs %ld, divisor %ld\n", psecs, divide_nint((uint64_t)(2 * 1000000 * DIVIDEND_SCALE), psecs));
    return divide_nint(divide_nint((uint64_t)(2 * 1000000 * DIVIDEND_SCALE), psecs), DIVIDEND_SCALE);
}

#define WITHIN(v,b,m) (((v)>=((b)-(m)))&&((v)<=((b)+(m))))

// pretty-print version, only works with what comes from the SPD: tCKmin or tCKAVGmin
unsigned long
pretty_psecs_to_mts(uint64_t psecs)
{
    uint64_t ret = 0; // default to error
    if (WITHIN(psecs, 2500, 1))
        ret = 800;
    else if (WITHIN(psecs, 1875, 1))
        ret = 1066;
    else if (WITHIN(psecs, 1500, 1))
        ret = 1333;
    else if (WITHIN(psecs, 1250, 1))
        ret = 1600;
    else if (WITHIN(psecs, 1071, 1))
        ret = 1866;
    else if (WITHIN(psecs, 937, 1))
        ret = 2133;
    else if (WITHIN(psecs, 833, 1))
        ret = 2400;
    else if (WITHIN(psecs, 750, 1))
        ret = 2666;
    return ret;
}

uint64_t
mts_to_hertz(uint64_t mts)
{
    return ((mts * 1000 * 1000) / 2);
}

#define DEBUG_RC3X_COMPUTE 0
#define rc3x_print(...)                                                 \
    do { if (DEBUG_RC3X_COMPUTE) ddr_print(__VA_ARGS__); } while (0)

static int compute_rc3x (int64_t tclk_psecs)
{
    long speed;
    long tclk_psecs_min, tclk_psecs_max;
    long data_rate_mhz, data_rate_mhz_min, data_rate_mhz_max;
    int rc3x;

#define ENCODING_BASE 1240

    data_rate_mhz = psecs_to_mts(tclk_psecs);

    /* 2400 MT/s is a special case. Using integer arithmetic it rounds
       from 833 psecs to 2401 MT/s. Force it to 2400 to pick the
       proper setting from the table. */
    if (tclk_psecs == 833)
        data_rate_mhz = 2400;

    for (speed = ENCODING_BASE; speed < 3200; speed += 20) {
        int error = 0;

        tclk_psecs_min = hertz_to_psecs(mts_to_hertz(speed + 00)); /* Clock in psecs */
        tclk_psecs_max = hertz_to_psecs(mts_to_hertz(speed + 18)); /* Clock in psecs */

        data_rate_mhz_min = psecs_to_mts(tclk_psecs_min);;
        data_rate_mhz_max = psecs_to_mts(tclk_psecs_max);

        /* Force alingment to multiple to avound rounding errors. */
        data_rate_mhz_min = ((data_rate_mhz_min + 18) / 20) * 20;
        data_rate_mhz_max = ((data_rate_mhz_max + 18) / 20) * 20;

        error += (speed + 00 != data_rate_mhz_min);
        error += (speed + 20 != data_rate_mhz_max);

        rc3x = (speed - ENCODING_BASE) / 20;

        rc3x_print("rc3x: %02x speed: %4ld MT/s < f <= %4ld MT/s, psec: %3ld:%3ld %4ld:%4ld %s\n",
                   rc3x,
                   speed, speed + 20,
                   tclk_psecs_min, tclk_psecs_max,
                   data_rate_mhz_min, data_rate_mhz_max,
                   error ? "****" : "");

        if (data_rate_mhz <= (speed + 20)) {
            rc3x_print("rc3x: %4ld MT/s <= %4ld MT/s\n", data_rate_mhz, speed + 20);
            break;
        }
    }
    return rc3x;
}

static const int   rlevel_separate_ab  = 1;

static int init_octeon3_ddr3_interface(uint32_t cpu_id,
                                       const ddr_configuration_t *ddr_configuration,
                                       uint32_t ddr_hertz,
                                       uint32_t cpu_hertz,
                                       uint32_t ddr_ref_hertz,
                                       int board_type,
                                       int board_rev_maj,
                                       int board_rev_min,
                                       int ddr_interface_num,
                                       uint32_t ddr_interface_mask,
                                       int node
                                       )
{
#if defined(__U_BOOT__) || defined(unix)
    char *s;
#endif

#pragma pack(push,1)
    const dimm_odt_config_t *odt_1rank_config = ddr_configuration->odt_1rank_config;
    const dimm_odt_config_t *odt_2rank_config = ddr_configuration->odt_2rank_config;
    const dimm_odt_config_t *odt_4rank_config = ddr_configuration->odt_4rank_config;
    const dimm_config_t *dimm_config_table = ddr_configuration->dimm_config_table;
    const dimm_odt_config_t *odt_config;
    const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;
    int odt_idx;

    /*
    ** Compute clock rates to the nearest picosecond.
    */
    ulong tclk_psecs = hertz_to_psecs(ddr_hertz); /* Clock in psecs */
    ulong eclk_psecs = hertz_to_psecs(cpu_hertz); /* Clock in psecs */

    int row_bits, col_bits, num_banks, num_ranks, dram_width;
    int dimm_count = 0;
    int fatal_error = 0;        /* Accumulate and report all the errors before giving up */

    int safe_ddr_flag = 0; /* Flag that indicates safe DDR settings should be used */
    int ddr_interface_64b = 1;  /* Octeon II Default: 64bit interface width */
    int ddr_interface_bytemask;
    uint32_t mem_size_mbytes = 0;
    unsigned int didx;
    int bank_bits = 0;
    int bunk_enable;
    int rank_mask;
    int column_bits_start = 1;
    int row_lsb;
    int pbank_lsb;
    int use_ecc = 1;
    int mtb_psec;
    short ftb_Dividend;
    short ftb_Divisor;
    int tAAmin;
    int tCKmin;
    int CL, min_cas_latency = 0, max_cas_latency = 0, override_cas_latency = 0;
    int ddr_rtt_nom_auto, ddr_rodt_ctl_auto;
    int i;

    int spd_addr;
    int spd_org;
    int spd_banks;
    int spd_rdimm;
    int spd_dimm_type;
    int spd_ecc;
    uint32_t spd_cas_latency;
    int spd_mtb_dividend;
    int spd_mtb_divisor;
    int spd_tck_min;
    int spd_taa_min;
    int spd_twr;
    int spd_trcd;
    int spd_trrd;
    int spd_trp;
    int spd_tras;
    int spd_trc;
    int spd_trfc;
    int spd_twtr;
    int spd_trtp;
    int spd_tfaw;
    int spd_addr_mirror;
    int spd_package = 0;
    int spd_rawcard = 0;
    int spd_rawcard_AorB = 0;
    int spd_rdimm_registers = 0;

    int is_stacked_die = 0;
    int is_3ds_dimm = 0; // 3DS
    int lranks_per_prank = 1; // 3DS: logical ranks per package rank
    int lranks_bits = 0; // 3DS: logical ranks bits
    int die_capacity = 0; // in Mbits; only used for 3DS

    ddr_type_t ddr_type;

    /* FTB values are two's complement ranging from +127 to -128. */
    typedef signed char SC_t;

    int twr;
    int trcd;
    int trrd;
    int trp;
    int tras;
    int trc;
    int trfc;
    int twtr;
    int trtp;
    int tfaw;

    int ddr4_tCKAVGmin;
    int ddr4_tCKAVGmax;
    int ddr4_tRCDmin;
    int ddr4_tRPmin;
    int ddr4_tRASmin;
    int ddr4_tRCmin;
    int ddr4_tRFC1min;
    int ddr4_tRFC2min;
    int ddr4_tRFC4min;
    int ddr4_tFAWmin;
    int ddr4_tRRD_Smin;
    int ddr4_tRRD_Lmin;
    int ddr4_tCCD_Lmin;

    int wlevel_bitmask_errors = 0;
    int wlevel_loops;
    int default_rtt_nom[4];
    int dyn_rtt_nom_mask;
    impedence_values_t *imp_values;
    char default_rodt_ctl;
    // default to disabled (ie, try LMC restart, not chip reset)
    int ddr_disable_chip_reset = 1;
    const char *dimm_type_name;
    /* Allow the Write bit-deskew feature to be enabled when desired. */
    // NOTE: 78xx pass 2.x only, 73xx, 75xx
    int enable_write_deskew = ENABLE_WRITE_DESKEW_DEFAULT;
    int write_deskew_done = 0;
    int match_wlevel_rtt_nom = 0;

#if SWL_TRY_HWL_ALT
    typedef struct {
	uint16_t hwl_alt_mask; // mask of bytelanes with alternate
	uint16_t  hwl_alt_delay[9]; // bytelane alternate avail if mask=1
    } hwl_alt_by_rank_t;
    hwl_alt_by_rank_t hwl_alts[4];
#endif /* SWL_TRY_HWL_ALT */

#define DEFAULT_INTERNAL_VREF_TRAINING_LIMIT 3 // was: 5
    int internal_retries = 0;

    int deskew_training_errors;
    deskew_counts_t deskew_training_results;
    int disable_deskew_training = 0;
    int restart_if_dsk_incomplete = 0;
    int dac_eval_retries;
    int dac_settings[9];
    int num_samples;
    int sample, lane;
    int last_lane = ((ddr_interface_64b) ? 8 : 4) + use_ecc;
#define DEFAULT_DAC_SAMPLES 7 // originally was 5
#define DAC_RETRIES_LIMIT   2

    typedef struct {
        int16_t bytes[DEFAULT_DAC_SAMPLES];
    } bytelane_sample_t;
    bytelane_sample_t lanes[9];

#pragma pack(pop)

#if SWL_TRY_HWL_ALT
    memset(hwl_alts, 0, sizeof(hwl_alts));
#endif

    /* Initialize these to shut up the compiler. They are configured
       and used only for DDR4  */
    ddr4_tRRD_Lmin = 6000;
    ddr4_tCCD_Lmin = 6000;

    ddr_print("\nInitializing node %d DDR interface %d, DDR Clock %d, DDR Reference Clock %d, CPUID 0x%08x\n",
              node, ddr_interface_num, ddr_hertz, ddr_ref_hertz, cpu_id);

    if (dimm_config_table[0].spd_addrs[0] == 0 && !dimm_config_table[0].spd_ptrs[0]) {
        error_print("ERROR: No dimms specified in the dimm_config_table.\n");
        return -1;
    }

    // allow some overrides to be done

    // this one controls whether chip RESET is done, or LMC init restarted from step 6.9.6
    if ((s = lookup_env_parameter("ddr_disable_chip_reset")) != NULL) {
        ddr_disable_chip_reset = !!strtoul(s, NULL, 0);
    }

    // this one controls whether Deskew Training is performed
    if ((s = lookup_env_parameter("ddr_disable_deskew_training")) != NULL) {
        disable_deskew_training = !!strtoul(s, NULL, 0);
    }

#if !defined(__U_BOOT__)
    // this one is in Validate_Deskew_Training and controls a preliminary delay
    if ((s = lookup_env_parameter("ddr_deskew_validation_delay")) != NULL) {
        deskew_validation_delay = strtoul(s, NULL, 0);
    }
    // this one is in Perform_Deskew_Training and controls lock retries 
    if ((s = lookup_env_parameter("ddr_lock_retries")) != NULL) {
        default_lock_retry_limit = strtoul(s, NULL, 0);
    }
#endif /* !defined(__U_BOOT__) */

    // setup/override for write bit-deskew feature
    if (! octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
        if ((s = lookup_env_parameter("ddr_enable_write_deskew")) != NULL) {
            enable_write_deskew = !!strtoul(s, NULL, 0);
        } // else take default setting
    } else { // not pass 2.x
        enable_write_deskew = 0; // force disabled
    }

    if (ddr_verbose()) {
        printf("DDR SPD Table:");
        for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx) {
            if (dimm_config_table[didx].spd_addrs[0] == 0)
                break;
            printf(" --ddr%dspd=0x%02x", ddr_interface_num, dimm_config_table[didx].spd_addrs[0]);
            if (dimm_config_table[didx].spd_addrs[1] != 0)
                printf(",0x%02x", dimm_config_table[didx].spd_addrs[1]);
        }
        printf("\n");
    }

    /*
     * Walk the DRAM Socket Configuration Table to see what is installed.
     */
    for (didx = 0; didx < DDR_CFG_T_MAX_DIMMS; ++didx) {
        /* Check for lower DIMM socket populated */
        if (validate_dimm(&dimm_config_table[didx], 0)) {
            if (ddr_verbose())
                report_dimm(&dimm_config_table[didx], 0, dimm_count, ddr_interface_num);
            ++dimm_count;
        } else {
            break;
	}       /* Finished when there is no lower DIMM */
    }


    initialize_ddr_clock(cpu_id,
			 ddr_configuration,
			 cpu_hertz,
			 ddr_hertz,
			 ddr_ref_hertz,
			 ddr_interface_num,
			 ddr_interface_mask,
			 node);

    if (!odt_1rank_config)
        odt_1rank_config = disable_odt_config;
    if (!odt_2rank_config)
        odt_2rank_config = disable_odt_config;
    if (!odt_4rank_config)
        odt_4rank_config = disable_odt_config;

    if ((s = getenv("ddr_safe")) != NULL) {
        safe_ddr_flag = !!simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_safe = %d\n", safe_ddr_flag);
    }


    if (dimm_count == 0) {
        error_print("ERROR: DIMM 0 not detected.\n");
        return(-1);
    }

    if (custom_lmc_config->mode32b)
        ddr_interface_64b = 0;

    if ((s = lookup_env_parameter("ddr_interface_64b")) != NULL) {
        ddr_interface_64b = !!simple_strtoul(s, NULL, 0);
    }

    if (ddr_interface_64b == 1) {
        if (octeon_is_cpuid(OCTEON_CN70XX)) {
            error_print("64-bit interface width is not supported for this Octeon model\n");
            ++fatal_error;
        }
    }

    /* ddr_type only indicates DDR4 or DDR3 */
    ddr_type = (read_spd(&dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_DEVICE_TYPE) == 0x0C) ? 4 : 3;
    debug_print("DRAM Device Type: DDR%d\n", ddr_type);

    if (ddr_type == DDR4_DRAM) {
        int spd_module_type;
        int asymmetric;
        const char *signal_load[4] = {"", "MLS", "3DS", "RSV"};
        imp_values = &ddr4_impedence_values;

        spd_addr  = read_spd(&dimm_config_table[0], 0, DDR4_SPD_ADDRESSING_ROW_COL_BITS);
        spd_org   = read_spd(&dimm_config_table[0], 0, DDR4_SPD_MODULE_ORGANIZATION);
        spd_banks = 0xFF & read_spd(&dimm_config_table[0], 0, DDR4_SPD_DENSITY_BANKS);

        bank_bits = (2 + ((spd_banks >> 4) & 0x3)) + ((spd_banks >> 6) & 0x3);
        bank_bits = min((int)bank_bits, 4); /* Controller can only address 4 bits. */

	spd_package    = 0XFF & read_spd(&dimm_config_table[0], 0, DDR4_SPD_PACKAGE_TYPE);
        if (spd_package & 0x80) { // non-monolithic device
            is_stacked_die = ((spd_package & 0x73) == 0x11);
            ddr_print("DDR4: Package Type 0x%x (%s), %d die\n", spd_package,
                      signal_load[(spd_package & 3)], ((spd_package >> 4) & 7) + 1);
            is_3ds_dimm = ((spd_package & 3) == 2); // is it 3DS?
            if (is_3ds_dimm) { // is it 3DS?
                lranks_per_prank = ((spd_package >> 4) & 7) + 1;
                // FIXME: should make sure it is only 2H or 4H or 8H?
                lranks_bits = lranks_per_prank >> 1;
                if (lranks_bits == 4) lranks_bits = 3;
            }
        } else if (spd_package != 0) {
            // FIXME: print non-zero monolithic device definition
            ddr_print("DDR4: Package Type MONOLITHIC: %d die, signal load %d\n",
                      ((spd_package >> 4) & 7) + 1, (spd_package & 3));
        }

        asymmetric = (spd_org >> 6) & 1;
        if (asymmetric) {
            int spd_secondary_pkg = read_spd(&dimm_config_table[0], 0,
                                             DDR4_SPD_SECONDARY_PACKAGE_TYPE);
            ddr_print("DDR4: Module Organization: ASYMMETRICAL: Secondary Package Type 0x%x\n",
                      spd_secondary_pkg);
        } else {
            uint64_t bus_width = 8 << (0x07 & read_spd(&dimm_config_table[0], 0,
                                                  DDR4_SPD_MODULE_MEMORY_BUS_WIDTH));
            uint64_t ddr_width = 4 << ((spd_org >> 0) & 0x7);
            uint64_t module_cap;
            int shift = (spd_banks & 0x0F);
            die_capacity = (shift < 8) ? (256UL << shift) : ((12UL << (shift & 1)) << 10);
            ddr_print("DDR4: Module Organization: SYMMETRICAL: capacity per die %d %cbit\n",
                      (die_capacity > 512) ? (die_capacity >> 10) : die_capacity,
                      (die_capacity > 512) ? 'G' : 'M');
            module_cap = ((uint64_t)die_capacity << 20) / 8UL * bus_width / ddr_width *
                /* no. pkg ranks*/(1UL + ((spd_org >> 3) & 0x7));
            if (is_3ds_dimm) // is it 3DS?
                module_cap *= /* die_count */(uint64_t)(((spd_package >> 4) & 7) + 1);
            ddr_print("DDR4: Module Organization: SYMMETRICAL: capacity per module %lld GB\n",
                      module_cap >> 30);
        }

        spd_rawcard = 0xFF & read_spd(&dimm_config_table[0], 0, DDR4_SPD_REFERENCE_RAW_CARD);
        ddr_print("DDR4: Reference Raw Card 0x%x \n", spd_rawcard);

        spd_module_type = read_spd(&dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_MODULE_TYPE);
        if (spd_module_type & 0x80) { // HYBRID module
            ddr_print("DDR4: HYBRID module, type %s\n",
                      ((spd_module_type & 0x70) == 0x10) ? "NVDIMM" : "UNKNOWN");
        }
        spd_dimm_type   = spd_module_type & 0x0F;
        spd_rdimm       = (spd_dimm_type == 1) || (spd_dimm_type == 5) || (spd_dimm_type == 8);
	if (spd_rdimm) {
            int spd_mfgr_id, spd_register_rev, spd_mod_attr;
	    spd_mfgr_id =
                (0xFFU & read_spd(&dimm_config_table[0], 0, DDR4_SPD_REGISTER_MANUFACTURER_ID_LSB)) |
		((0xFFU & read_spd(&dimm_config_table[0], 0, DDR4_SPD_REGISTER_MANUFACTURER_ID_MSB)) << 8);
	    spd_register_rev = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR4_SPD_REGISTER_REVISION_NUMBER);
	    ddr_print("DDR4: RDIMM Register Manufacturer ID 0x%x Revision 0x%x\n",
		      spd_mfgr_id, spd_register_rev);

	    // RAWCARD A or B must be bit 7=0 and bits 4-0 either 00000(A) or 00001(B)
	    spd_rawcard_AorB = ((spd_rawcard & 0x9fUL) <= 1);
            // RDIMM Module Attributes
            spd_mod_attr = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR4_SPD_UDIMM_ADDR_MAPPING_FROM_EDGE);
            spd_rdimm_registers = ((1 << (spd_mod_attr & 3)) >> 1);
	    ddr_print("DDR4: RDIMM Module Attributes (0x%x): Register Type DDR4RCD%02d, DRAM rows %d, Registers %d\n",
                      spd_mod_attr, (spd_mod_attr >> 4) + 1,((1 << ((spd_mod_attr >> 2) & 3)) >> 1),
                      spd_rdimm_registers);
	}
        dimm_type_name = ddr4_dimm_types[spd_dimm_type];
    } else {
        const char *signal_load[4] = {"UNK", "MLS", "SLS", "RSV"};

        imp_values = &ddr3_impedence_values;

        spd_addr = read_spd(&dimm_config_table[0], 0, DDR3_SPD_ADDRESSING_ROW_COL_BITS);
        spd_org = read_spd(&dimm_config_table[0], 0, DDR3_SPD_MODULE_ORGANIZATION);
        spd_banks = read_spd(&dimm_config_table[0], 0, DDR3_SPD_DENSITY_BANKS) & 0xff;

        bank_bits = 3 + ((spd_banks >> 4) & 0x7);
        bank_bits = min((int)bank_bits, 3); /* Controller can only address 3 bits. */
        spd_dimm_type   = 0x0f & read_spd(&dimm_config_table[0], 0, DDR3_SPD_KEY_BYTE_MODULE_TYPE);
        spd_rdimm       = (spd_dimm_type == 1) || (spd_dimm_type == 5) || (spd_dimm_type == 9);

        spd_package	= 0xFF & read_spd(&dimm_config_table[0], 0, DDR3_SPD_SDRAM_DEVICE_TYPE);
        if (spd_package & 0x80) { // non-standard device
            ddr_print("DDR3: Device Type 0x%x (%s), %d die\n", spd_package,
                      signal_load[(spd_package & 3)], ((1 << ((spd_package >> 4) & 7)) >> 1));
        } else if (spd_package != 0) {
            // FIXME: print non-zero monolithic device definition
            ddr_print("DDR3: Device Type MONOLITHIC: %d die, signal load %d\n",
                      ((1 << (spd_package >> 4) & 7) >> 1), (spd_package & 3));
        }

        spd_rawcard	= 0xFF & read_spd(&dimm_config_table[0], 0, DDR3_SPD_REFERENCE_RAW_CARD);
        ddr_print("DDR3: Reference Raw Card 0x%x \n", spd_rawcard);

	if (spd_rdimm) {
            int spd_mfgr_id, spd_register_rev, spd_mod_attr;
	    spd_mfgr_id =
                (0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_REGISTER_MANUFACTURER_ID_LSB)) |
		((0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_REGISTER_MANUFACTURER_ID_MSB)) << 8);
	    spd_register_rev = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_REGISTER_REVISION_NUMBER);
	    ddr_print("DDR3: RDIMM Register Manufacturer ID 0x%x Revision 0x%x\n",
		      spd_mfgr_id, spd_register_rev);
            spd_mod_attr = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_ADDRESS_MAPPING); // Module Attributes
            spd_rdimm_registers = ((1 << (spd_mod_attr & 3)) >> 1);
	    ddr_print("DDR3: RDIMM Module Attributes (0x%x): DRAM rows %d, Registers %d\n", spd_mod_attr,
                      ((1 << ((spd_mod_attr >> 2) & 3)) >> 1), spd_rdimm_registers);
	}
#if 0
        {
            int spd_thermal_opts, spd_thermal_sensor;
	    spd_thermal_opts = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_SDRAM_THERMAL_REFRESH_OPTIONS);
	    ddr_print("DDR3: SDRAM Thermal and Refresh Options (0x%x): %s %s %s %s %s\n",
                      spd_thermal_opts,
                      (spd_thermal_opts & 0x80)?"PASR":"!PASR",
                      (spd_thermal_opts & 0x08)?"ODTS":"!ODTS",
                      (spd_thermal_opts & 0x04)?"ASR":"!ASR",
                      (spd_thermal_opts & 0x02)?"1X":"2X",
                      (spd_thermal_opts & 0x01)?"0-95C":"0-85C"
                      );
	    spd_thermal_sensor = 0xFFU & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MODULE_THERMAL_SENSOR);
	    ddr_print("DDR3: Module Thermal Sensor (0x%x): %s\n", spd_thermal_sensor,
                      (spd_thermal_sensor & 0x80) ? "Present" : "Not Present");
        }
#endif
        dimm_type_name = ddr3_dimm_types[spd_dimm_type];
    }

    debug_print("spd_addr        : %#06x\n", spd_addr );
    debug_print("spd_org         : %#06x\n", spd_org );
    debug_print("spd_banks       : %#06x\n", spd_banks );

    row_bits = 12 + ((spd_addr >> 3) & 0x7);
    col_bits =  9 + ((spd_addr >> 0) & 0x7);

    num_ranks =  1 + ((spd_org >> 3) & 0x7);
    dram_width = 4 << ((spd_org >> 0) & 0x7);
    num_banks = 1 << bank_bits;

    if ((s = lookup_env_parameter("ddr_num_ranks")) != NULL) {
        num_ranks = simple_strtoul(s, NULL, 0);
    }

#if 1
    // FIXME: recognize a DDR3 RDIMM with 4 ranks and 2 registers, and treat it specially 
    if ((ddr_type == DDR3_DRAM) && spd_rdimm && (num_ranks == 4) && (spd_rdimm_registers == 2)) {
        dimm_odt_config_t *odt_config;
        cvmx_lmcx_modereg_params1_t modereg_params1;

        ddr_print("DDR3: special 4-rank RDIMM: treat as two 2-rank DIMMs\n");

        num_ranks = 2;
        dimm_count = 2;
        restart_if_dsk_incomplete = 1;
        match_wlevel_rtt_nom = 1;

        // must point (virtual) 2nd DIMM's SPD at 1st DIMM's SPD
        ((dimm_config_t *)&dimm_config_table[1])->spd_addrs[0] = dimm_config_table[0].spd_addrs[0];

        // setup for modifying config table values - 2 ranks and 2 DIMMs
        odt_config = (dimm_odt_config_t *)&ddr_configuration->odt_2rank_config[1];

        odt_config->odt_mask = /*WODT_MASK*/ 0x05050505ULL;
        odt_config->rodt_ctl = /*RODT_MASK*/ 0x00000000ULL;
        odt_config->qs_dic   = /*RODT_CTL*/  rodt_ctl_40_ohm;

        // fetch the original settings
        modereg_params1.u64 = odt_config->modereg_params1.u64;
        // change specific settings here...
        modereg_params1.s.rtt_nom_00 = rttnom_60ohm;
        modereg_params1.s.rtt_nom_01 = rttnom_none;
        modereg_params1.s.rtt_nom_10 = rttnom_60ohm;
        modereg_params1.s.rtt_nom_11 = rttnom_none;
        // save final settings
        odt_config->modereg_params1.u64 = /*MODEREG_PARAMS1*/modereg_params1.u64;
    }
#endif


    /* FIX
    ** Check that values are within some theoretical limits.
    ** col_bits(min) = row_lsb(min) - bank_bits(max) - bus_bits(max) = 14 - 3 - 4 = 7
    ** col_bits(max) = row_lsb(max) - bank_bits(min) - bus_bits(min) = 18 - 2 - 3 = 13
    */
    if ((col_bits > 13) || (col_bits < 7)) {
        error_print("Unsupported number of Col Bits: %d\n", col_bits);
        ++fatal_error;
    }

    /* FIX
    ** Check that values are within some theoretical limits.
    ** row_bits(min) = pbank_lsb(min) - row_lsb(max) - rank_bits = 26 - 18 - 1 = 7
    ** row_bits(max) = pbank_lsb(max) - row_lsb(min) - rank_bits = 33 - 14 - 1 = 18
    */
    if ((row_bits > 18) || (row_bits < 7)) {
        error_print("Unsupported number of Row Bits: %d\n", row_bits);
        ++fatal_error;
    }

    if ((s = lookup_env_parameter("ddr_rdimm_ena")) != NULL) {
        spd_rdimm = !!simple_strtoul(s, NULL, 0);
    }

    wlevel_loops = WLEVEL_LOOPS_DEFAULT;
    // accept generic or interface-specific override
    if ((s = lookup_env_parameter("ddr_wlevel_loops")) == NULL)
        s = lookup_env_parameter("ddr%d_wlevel_loops", ddr_interface_num);
    if (s != NULL) {
        wlevel_loops = strtoul(s, NULL, 0);
    }

    if ((s = lookup_env_parameter("ddr_ranks")) != NULL) {
        num_ranks = simple_strtoul(s, NULL, 0);
    }

    bunk_enable = (num_ranks > 1);

    if (octeon_is_cpuid(OCTEON_CN7XXX))
        column_bits_start = 3;
    else
        printf("ERROR: Unsupported Octeon model: 0x%x\n", cpu_id);

    row_lsb = column_bits_start + col_bits + bank_bits - (! ddr_interface_64b);
    debug_print("row_lsb = column_bits_start + col_bits + bank_bits = %d\n", row_lsb);

    pbank_lsb = row_lsb + row_bits + bunk_enable;
    debug_print("pbank_lsb = row_lsb + row_bits + bunk_enable = %d\n", pbank_lsb);

    if (lranks_per_prank > 1) {
        pbank_lsb = row_lsb + row_bits + lranks_bits + bunk_enable;
        ddr_print("DDR4: 3DS: pbank_lsb = (%d row_lsb) + (%d row_bits) + (%d lranks_bits) + (%d bunk_enable) = %d\n",
                  row_lsb, row_bits, lranks_bits, bunk_enable, pbank_lsb);
    }

    mem_size_mbytes =  dimm_count * ((1ull << pbank_lsb) >> 20);
    if (num_ranks == 4) {
        /* Quad rank dimm capacity is equivalent to two dual-rank dimms. */
        mem_size_mbytes *= 2;
    }

    /* Mask with 1 bits set for for each active rank, allowing 2 bits per dimm.
    ** This makes later calculations simpler, as a variety of CSRs use this layout.
    ** This init needs to be updated for dual configs (ie non-identical DIMMs).
    ** Bit 0 = dimm0, rank 0
    ** Bit 1 = dimm0, rank 1
    ** Bit 2 = dimm1, rank 0
    ** Bit 3 = dimm1, rank 1
    ** ...
    */
    rank_mask = 0x1;
    if (num_ranks > 1)
        rank_mask = 0x3;
    if (num_ranks > 2)
        rank_mask = 0xf;

    for (i = 1; i < dimm_count; i++)
        rank_mask |= ((rank_mask & 0x3) << (2*i));



#if defined(__U_BOOT__)
    {
        /* If we are booting from RAM, the DRAM controller is already set up.  Just return the
        ** memory size */
        if (gd->flags & GD_FLG_RAM_RESIDENT) {
            ddr_print("Ram Boot: Skipping LMC config\n");
            return mem_size_mbytes;
        }
    }
#endif


    if (ddr_type == DDR4_DRAM) {
        spd_ecc         = !!(read_spd(&dimm_config_table[0], 0, DDR4_SPD_MODULE_MEMORY_BUS_WIDTH) & 8);
    } else {
        spd_ecc         = !!(read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEMORY_BUS_WIDTH) & 8);
    }

    ddr_print("Summary: %d %s%s %dRx%d %s, %d MB total, row bits=%d, col bits=%d, bank bits=%d, banks=%d\n",
              dimm_count, dimm_type_name, (dimm_count > 1) ? "s" : "",
              num_ranks, dram_width, (spd_ecc) ? "ECC" : "non-ECC",
              mem_size_mbytes, row_bits, col_bits, bank_bits, num_banks);

    if (ddr_type == DDR4_DRAM) {
        spd_cas_latency  = ((0xff & read_spd(&dimm_config_table[0], 0, DDR4_SPD_CAS_LATENCIES_BYTE0)) <<  0);
        spd_cas_latency |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR4_SPD_CAS_LATENCIES_BYTE1)) <<  8);
        spd_cas_latency |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR4_SPD_CAS_LATENCIES_BYTE2)) << 16);
        spd_cas_latency |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR4_SPD_CAS_LATENCIES_BYTE3)) << 24);
    } else {
        spd_cas_latency  = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_CAS_LATENCIES_LSB);
        spd_cas_latency |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_CAS_LATENCIES_MSB)) << 8);
    }
    debug_print("spd_cas_latency : %#06x\n", spd_cas_latency );

    if (ddr_type == DDR4_DRAM) {

        /* No other values for DDR4 MTB and FTB are specified at the
         * current time so don't bother reading them. Can't speculate how
         * new values will be represented.
         */
        int spdMTB = 125;
        int spdFTB = 1;

        tAAmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_CAS_LATENCY_TAAMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_CAS_LATENCY_FINE_TAAMIN);

        ddr4_tCKAVGmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MINIMUM_CYCLE_TIME_TCKAVGMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_CYCLE_TIME_FINE_TCKAVGMIN);

        ddr4_tCKAVGmax
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MAXIMUM_CYCLE_TIME_TCKAVGMAX)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MAX_CYCLE_TIME_FINE_TCKAVGMAX);

        ddr4_tRCDmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_RAS_CAS_DELAY_TRCDMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_RAS_TO_CAS_DELAY_FINE_TRCDMIN);

        ddr4_tRPmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ROW_PRECHARGE_DELAY_TRPMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ROW_PRECHARGE_DELAY_FINE_TRPMIN);

        ddr4_tRASmin
            = spdMTB * (((read_spd(&dimm_config_table[0], 0, DDR4_SPD_UPPER_NIBBLES_TRAS_TRC) & 0xf) << 8) +
                        ( read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ACTIVE_PRECHARGE_LSB_TRASMIN) & 0xff));

        ddr4_tRCmin
            = spdMTB * ((((read_spd(&dimm_config_table[0], 0, DDR4_SPD_UPPER_NIBBLES_TRAS_TRC) >> 4) & 0xf) << 8) +
                        (  read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ACTIVE_REFRESH_LSB_TRCMIN) & 0xff))
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ACT_TO_ACT_REFRESH_DELAY_FINE_TRCMIN);

        ddr4_tRFC1min
            = spdMTB * (((read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC1MIN) & 0xff) << 8) +
                        ( read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC1MIN) & 0xff));

        ddr4_tRFC2min
            = spdMTB * (((read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC2MIN) & 0xff) << 8) +
                        ( read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC2MIN) & 0xff));

        ddr4_tRFC4min
            = spdMTB * (((read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_MSB_TRFC4MIN) & 0xff) << 8) +
                        ( read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_REFRESH_RECOVERY_LSB_TRFC4MIN) & 0xff));

        ddr4_tFAWmin
            = spdMTB * (((read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_FOUR_ACTIVE_WINDOW_MSN_TFAWMIN) & 0xf) << 8) +
                        ( read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_FOUR_ACTIVE_WINDOW_LSB_TFAWMIN) & 0xff));

        ddr4_tRRD_Smin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ROW_ACTIVE_DELAY_SAME_TRRD_SMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ACT_TO_ACT_DELAY_DIFF_FINE_TRRD_SMIN);

        ddr4_tRRD_Lmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ROW_ACTIVE_DELAY_DIFF_TRRD_LMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_ACT_TO_ACT_DELAY_SAME_FINE_TRRD_LMIN);

        ddr4_tCCD_Lmin
            = spdMTB *        read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_CAS_TO_CAS_DELAY_TCCD_LMIN)
            + spdFTB * (SC_t) read_spd(&dimm_config_table[0], 0, DDR4_SPD_MIN_CAS_TO_CAS_DELAY_FINE_TCCD_LMIN);


        ddr_print("%-45s : %6d ps\n", "Medium Timebase (MTB)",          		   spdMTB);
        ddr_print("%-45s : %6d ps\n", "Fine Timebase   (FTB)",          		   spdFTB);

        ddr_print("%-45s : %6d ps (%ld MT/s)\n", "SDRAM Minimum Cycle Time (tCKAVGmin)",ddr4_tCKAVGmin,
                  pretty_psecs_to_mts(ddr4_tCKAVGmin));
        ddr_print("%-45s : %6d ps\n", "SDRAM Maximum Cycle Time (tCKAVGmax)",          ddr4_tCKAVGmax);
        ddr_print("%-45s : %6d ps\n", "Minimum CAS Latency Time (tAAmin)",         	   tAAmin);
        ddr_print("%-45s : %6d ps\n", "Minimum RAS to CAS Delay Time (tRCDmin)",       ddr4_tRCDmin);
        ddr_print("%-45s : %6d ps\n", "Minimum Row Precharge Delay Time (tRPmin)",     ddr4_tRPmin);
        ddr_print("%-45s : %6d ps\n", "Minimum Active to Precharge Delay (tRASmin)",   ddr4_tRASmin);
        ddr_print("%-45s : %6d ps\n", "Minimum Active to Active/Refr. Delay (tRCmin)", ddr4_tRCmin);
        ddr_print("%-45s : %6d ps\n", "Minimum Refresh Recovery Delay (tRFC1min)",     ddr4_tRFC1min);
        ddr_print("%-45s : %6d ps\n", "Minimum Refresh Recovery Delay (tRFC2min)",     ddr4_tRFC2min);
        ddr_print("%-45s : %6d ps\n", "Minimum Refresh Recovery Delay (tRFC4min)",     ddr4_tRFC4min);
        ddr_print("%-45s : %6d ps\n", "Minimum Four Activate Window Time (tFAWmin)",   ddr4_tFAWmin);
        ddr_print("%-45s : %6d ps\n", "Minimum Act. to Act. Delay (tRRD_Smin)", 	   ddr4_tRRD_Smin);
        ddr_print("%-45s : %6d ps\n", "Minimum Act. to Act. Delay (tRRD_Lmin)", 	   ddr4_tRRD_Lmin);
        ddr_print("%-45s : %6d ps\n", "Minimum CAS to CAS Delay Time (tCCD_Lmin)",     ddr4_tCCD_Lmin);

#define DDR4_TWR 15000
#define DDR4_TWTR_S 2500


        tCKmin	    = ddr4_tCKAVGmin;
        twr             = DDR4_TWR;
        trcd            = ddr4_tRCDmin;
        trrd            = ddr4_tRRD_Smin;
        trp             = ddr4_tRPmin;
        tras            = ddr4_tRASmin;
        trc             = ddr4_tRCmin;
        trfc            = ddr4_tRFC1min;
        twtr            = DDR4_TWTR_S;
        tfaw            = ddr4_tFAWmin;


        if (spd_rdimm) {
            spd_addr_mirror  = read_spd(&dimm_config_table[0], 0,DDR4_SPD_RDIMM_ADDR_MAPPING_FROM_REGISTER_TO_DRAM) & 0x1;
        } else {
            spd_addr_mirror  = read_spd(&dimm_config_table[0], 0,DDR4_SPD_UDIMM_ADDR_MAPPING_FROM_EDGE) & 0x1;
        }
        debug_print("spd_addr_mirror : %#06x\n", spd_addr_mirror );

    } else {
        spd_mtb_dividend = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEDIUM_TIMEBASE_DIVIDEND);
        spd_mtb_divisor  = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MEDIUM_TIMEBASE_DIVISOR);
        spd_tck_min      = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MINIMUM_CYCLE_TIME_TCKMIN);
        spd_taa_min      = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_CAS_LATENCY_TAAMIN);

        spd_twr          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_WRITE_RECOVERY_TWRMIN);
        spd_trcd         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_RAS_CAS_DELAY_TRCDMIN);
        spd_trrd         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ROW_ACTIVE_DELAY_TRRDMIN);
        spd_trp          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ROW_PRECHARGE_DELAY_TRPMIN);
        spd_tras         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ACTIVE_PRECHARGE_LSB_TRASMIN);
        spd_tras        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLES_TRAS_TRC)&0xf) << 8);
        spd_trc          = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_ACTIVE_REFRESH_LSB_TRCMIN);
        spd_trc         |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLES_TRAS_TRC)&0xf0) << 4);
        spd_trfc         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_REFRESH_RECOVERY_LSB_TRFCMIN);
        spd_trfc        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_REFRESH_RECOVERY_MSB_TRFCMIN)) << 8);
        spd_twtr         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_INTERNAL_WRITE_READ_CMD_TWTRMIN);
        spd_trtp         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_INTERNAL_READ_PRECHARGE_CMD_TRTPMIN);
        spd_tfaw         = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_FOUR_ACTIVE_WINDOW_TFAWMIN);
        spd_tfaw        |= ((0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_UPPER_NIBBLE_TFAW)&0xf) << 8);
        spd_addr_mirror  = 0xff & read_spd(&dimm_config_table[0], 0,DDR3_SPD_ADDRESS_MAPPING) & 0x1;
        spd_addr_mirror  = spd_addr_mirror && !spd_rdimm; /* Only address mirror unbuffered dimms.  */
        ftb_Dividend     = read_spd(&dimm_config_table[0], 0, DDR3_SPD_FINE_TIMEBASE_DIVIDEND_DIVISOR) >> 4;
        ftb_Divisor      = read_spd(&dimm_config_table[0], 0, DDR3_SPD_FINE_TIMEBASE_DIVIDEND_DIVISOR) & 0xf;
        ftb_Divisor      = (ftb_Divisor == 0) ? 1 : ftb_Divisor; /* Make sure that it is not 0 */

        debug_print("spd_twr         : %#06x\n", spd_twr );
        debug_print("spd_trcd        : %#06x\n", spd_trcd);
        debug_print("spd_trrd        : %#06x\n", spd_trrd);
        debug_print("spd_trp         : %#06x\n", spd_trp );
        debug_print("spd_tras        : %#06x\n", spd_tras);
        debug_print("spd_trc         : %#06x\n", spd_trc );
        debug_print("spd_trfc        : %#06x\n", spd_trfc);
        debug_print("spd_twtr        : %#06x\n", spd_twtr);
        debug_print("spd_trtp        : %#06x\n", spd_trtp);
        debug_print("spd_tfaw        : %#06x\n", spd_tfaw);
        debug_print("spd_addr_mirror : %#06x\n", spd_addr_mirror);

        mtb_psec        = spd_mtb_dividend * 1000 / spd_mtb_divisor;
        tAAmin          = mtb_psec * spd_taa_min;
        tAAmin         += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MIN_CAS_LATENCY_FINE_TAAMIN) / ftb_Divisor;
        tCKmin          = mtb_psec * spd_tck_min;
        tCKmin         += ftb_Dividend * (SC_t) read_spd(&dimm_config_table[0], 0, DDR3_SPD_MINIMUM_CYCLE_TIME_FINE_TCKMIN) / ftb_Divisor;

        twr             = spd_twr  * mtb_psec;
        trcd            = spd_trcd * mtb_psec;
        trrd            = spd_trrd * mtb_psec;
        trp             = spd_trp  * mtb_psec;
        tras            = spd_tras * mtb_psec;
        trc             = spd_trc  * mtb_psec;
        trfc            = spd_trfc * mtb_psec;
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X) && trfc < 260000) {
            int new_trfc = 260000; // default to this - because it works...
            if ((s = getenv("ddr_trfc")) != NULL) {
                new_trfc = simple_strtoul(s, NULL, 0);
                error_print("Parameter found in environment. ddr_trfc = %d\n", new_trfc);
                if ((new_trfc < 160000) || (new_trfc > 260000))
                    new_trfc = 260000; // back to default if out of range
            }
            ddr_print("N%d.LMC%d: Adjusting tRFC from %d to %d, for CN78XX Pass 2.x\n",
                      node, ddr_interface_num, trfc, new_trfc);
            trfc = new_trfc;
        }
        twtr            = spd_twtr * mtb_psec;
        trtp            = spd_trtp * mtb_psec;
        tfaw            = spd_tfaw * mtb_psec;

        ddr_print("Medium Timebase (MTB)                         : %6d ps\n", mtb_psec);
	ddr_print("Minimum Cycle Time (tCKmin)                   : %6d ps (%ld MT/s)\n", tCKmin,
                  pretty_psecs_to_mts(tCKmin));
        ddr_print("Minimum CAS Latency Time (tAAmin)             : %6d ps\n", tAAmin);
        ddr_print("Write Recovery Time (tWR)                     : %6d ps\n", twr);
        ddr_print("Minimum RAS to CAS delay (tRCD)               : %6d ps\n", trcd);
        ddr_print("Minimum Row Active to Row Active delay (tRRD) : %6d ps\n", trrd);
        ddr_print("Minimum Row Precharge Delay (tRP)             : %6d ps\n", trp);
        ddr_print("Minimum Active to Precharge (tRAS)            : %6d ps\n", tras);
        ddr_print("Minimum Active to Active/Refresh Delay (tRC)  : %6d ps\n", trc);
        ddr_print("Minimum Refresh Recovery Delay (tRFC)         : %6d ps\n", trfc);
        ddr_print("Internal write to read command delay (tWTR)   : %6d ps\n", twtr);
        ddr_print("Min Internal Rd to Precharge Cmd Delay (tRTP) : %6d ps\n", trtp);
        ddr_print("Minimum Four Activate Window Delay (tFAW)     : %6d ps\n", tfaw);
    }

    /* When the cycle time is within 1 psec of the minimum accept it
       as a slight rounding error and adjust it to exactly the minimum
       cycle time. This avoids an unnecessary warning. */
    if (_abs(tclk_psecs - tCKmin) < 2)
        tclk_psecs = tCKmin;

    if (tclk_psecs < (uint64_t)tCKmin) {
        error_print("WARNING!!!!: DDR Clock Rate (tCLK: %ld) exceeds DIMM specifications (tCKmin: %ld)!!!!\n",
                    tclk_psecs, (ulong) tCKmin);
    }


    ddr_print("DDR Clock Rate (tCLK)                         : %6d ps\n", tclk_psecs);
    ddr_print("Core Clock Rate (eCLK)                        : %6d ps\n", eclk_psecs);

    if ((s = getenv("ddr_use_ecc")) != NULL) {
        use_ecc = !!simple_strtoul(s, NULL, 0);
        error_print("Parameter found in environment. ddr_use_ecc = %d\n", use_ecc);
    }
    use_ecc = use_ecc && spd_ecc;

    ddr_interface_bytemask = ddr_interface_64b
        ? (use_ecc ? 0x1ff : 0xff)
        : (use_ecc ? 0x01f : 0x0f);

    ddr_print("DRAM Interface width: %d bits %s\n",
              ddr_interface_64b ? 64 : 32, use_ecc ? "+ECC" : "");




    ddr_print("\n------ Board Custom Configuration Settings ------\n");
    ddr_print("%-45s : %d\n", "MIN_RTT_NOM_IDX   ", custom_lmc_config->min_rtt_nom_idx);
    ddr_print("%-45s : %d\n", "MAX_RTT_NOM_IDX   ", custom_lmc_config->max_rtt_nom_idx);
    ddr_print("%-45s : %d\n", "MIN_RODT_CTL      ", custom_lmc_config->min_rodt_ctl);
    ddr_print("%-45s : %d\n", "MAX_RODT_CTL      ", custom_lmc_config->max_rodt_ctl);
    ddr_print("%-45s : %d\n", "MIN_CAS_LATENCY   ", custom_lmc_config->min_cas_latency);
    ddr_print("%-45s : %d\n", "OFFSET_EN         ", custom_lmc_config->offset_en);
    ddr_print("%-45s : %d\n", "OFFSET_UDIMM      ", custom_lmc_config->offset_udimm);
    ddr_print("%-45s : %d\n", "OFFSET_RDIMM      ", custom_lmc_config->offset_rdimm);
    ddr_print("%-45s : %d\n", "DDR_RTT_NOM_AUTO  ", custom_lmc_config->ddr_rtt_nom_auto);
    ddr_print("%-45s : %d\n", "DDR_RODT_CTL_AUTO ", custom_lmc_config->ddr_rodt_ctl_auto);
    if (spd_rdimm)
        ddr_print("%-45s : %d\n", "RLEVEL_COMP_OFFSET", custom_lmc_config->rlevel_comp_offset_rdimm);
    else
        ddr_print("%-45s : %d\n", "RLEVEL_COMP_OFFSET", custom_lmc_config->rlevel_comp_offset_udimm);
    ddr_print("%-45s : %d\n", "RLEVEL_COMPUTE    ", custom_lmc_config->rlevel_compute);
    ddr_print("%-45s : %d\n", "DDR2T_UDIMM       ", custom_lmc_config->ddr2t_udimm);
    ddr_print("%-45s : %d\n", "DDR2T_RDIMM       ", custom_lmc_config->ddr2t_rdimm);
    ddr_print("%-45s : %d\n", "FPRCH2            ", custom_lmc_config->fprch2);
    ddr_print("%-45s : %d\n", "PTUNE_OFFSET      ", custom_lmc_config->ptune_offset);
    ddr_print("%-45s : %d\n", "NTUNE_OFFSET      ", custom_lmc_config->ntune_offset);
    ddr_print("-------------------------------------------------\n");

    CL              = divide_roundup(tAAmin, tclk_psecs);

    ddr_print("Desired CAS Latency                           : %6d\n", CL);

    min_cas_latency = custom_lmc_config->min_cas_latency;


    if ((s = lookup_env_parameter("ddr_min_cas_latency")) != NULL) {
        min_cas_latency = simple_strtoul(s, NULL, 0);
    }

    {
        int base_CL;
        ddr_print("CAS Latencies supported in DIMM               :");
        base_CL = (ddr_type == DDR4_DRAM) ? 7 : 4;
        for (i=0; i<32; ++i) {
            if ((spd_cas_latency >> i) & 1) {
                ddr_print(" %d", i+base_CL);
                max_cas_latency = i+base_CL;
                if (min_cas_latency == 0)
                    min_cas_latency = i+base_CL;
            }
        }
        ddr_print("\n");

        /* Use relaxed timing when running slower than the minimum
           supported speed.  Adjust timing to match the smallest supported
           CAS Latency. */
        if (CL < min_cas_latency) {
            ulong adjusted_tclk = tAAmin / min_cas_latency;
            CL = min_cas_latency;
            ddr_print("Slow clock speed. Adjusting timing: tClk = %d, Adjusted tClk = %d\n",
                      tclk_psecs, adjusted_tclk);
            tclk_psecs = adjusted_tclk;
        }

        if ((s = getenv("ddr_cas_latency")) != NULL) {
            override_cas_latency = simple_strtoul(s, NULL, 0);
            error_print("Parameter found in environment. ddr_cas_latency = %d\n", override_cas_latency);
        }

        /* Make sure that the selected cas latency is legal */
        for (i=(CL-base_CL); i<32; ++i) {
            if ((spd_cas_latency >> i) & 1) {
                CL = i+base_CL;
                break;
            }
        }
    }

    if (CL > max_cas_latency)
        CL = max_cas_latency;

    if (override_cas_latency != 0) {
        CL = override_cas_latency;
    }

    ddr_print("CAS Latency                                   : %6d\n", CL);

    if ((CL * tCKmin) > 20000)
    {
        ddr_print("(CLactual * tCKmin) = %d exceeds 20 ns\n", (CL * tCKmin));
    }

    if (tclk_psecs < (ulong)tCKmin)
        error_print("WARNING!!!!!!: DDR3 Clock Rate (tCLK: %ld) exceeds DIMM specifications (tCKmin:%ld)!!!!!!!!\n",
                    tclk_psecs, (ulong)tCKmin);


    if ((num_banks != 4) && (num_banks != 8) && (num_banks != 16))
    {
        error_print("Unsupported number of banks %d. Must be 4 or 8.\n", num_banks);
        ++fatal_error;
    }

    if ((num_ranks != 1) && (num_ranks != 2) && (num_ranks != 4))
    {
        error_print("Unsupported number of ranks: %d\n", num_ranks);
        ++fatal_error;
    }

    if (octeon_is_cpuid(OCTEON_CN78XX) || octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX)) {
        if ((dram_width != 8) && (dram_width != 16) && (dram_width != 4)) {
            error_print("Unsupported SDRAM Width, %d.  Must be 4, 8 or 16.\n", dram_width);
            ++fatal_error;
        }
    } else if ((dram_width != 8) && (dram_width != 16)) {
        error_print("Unsupported SDRAM Width, %d.  Must be 8 or 16.\n", dram_width);
        ++fatal_error;
    }


    /*
    ** Bail out here if things are not copasetic.
    */
    if (fatal_error)
        return(-1);

    /*
     * 4.8.4 LMC RESET Initialization
     *
     * The purpose of this step is to assert/deassert the RESET# pin at the
     * DDR3/DDR4 parts.
     *
     * This LMC RESET step is done for all enabled LMCs.
     */
    perform_lmc_reset(node, ddr_interface_num);

    {
        char *s;
        cvmx_lmcx_control_t lmc_control;
        cvmx_lmcx_scramble_cfg0_t lmc_scramble_cfg0;
        cvmx_lmcx_scramble_cfg1_t lmc_scramble_cfg1;

        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
        lmc_scramble_cfg0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SCRAMBLE_CFG0(ddr_interface_num));
        lmc_scramble_cfg1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SCRAMBLE_CFG1(ddr_interface_num));

        if ((s = lookup_env_parameter_ull("ddr_scramble_cfg0")) != NULL) {
            lmc_scramble_cfg0.u64    = simple_strtoull(s, NULL, 0);
            lmc_control.s.scramble_ena = 1;
        }
        ddr_print("%-45s : 0x%016llx\n", "LMC_SCRAMBLE_CFG0", lmc_scramble_cfg0.u64);

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_SCRAMBLE_CFG0(ddr_interface_num),
				  lmc_scramble_cfg0.u64);

        if ((s = lookup_env_parameter_ull("ddr_scramble_cfg1")) != NULL) {
            lmc_scramble_cfg1.u64    = simple_strtoull(s, NULL, 0);
            lmc_control.s.scramble_ena = 1;
        }
        ddr_print("%-45s : 0x%016llx\n", "LMC_SCRAMBLE_CFG1", lmc_scramble_cfg1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_SCRAMBLE_CFG1(ddr_interface_num),
				  lmc_scramble_cfg1.u64);

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);
    }


    odt_idx = min(dimm_count - 1, 3);

    switch (num_ranks) {
    case 1:
        odt_config = odt_1rank_config;
        break;
    case 2:
        odt_config = odt_2rank_config;
        break;
    case 4:
        odt_config = odt_4rank_config;
        break;
    default:
        odt_config = disable_odt_config;
        error_print("Unsupported number of ranks: %d\n", num_ranks);
        ++fatal_error;
    }


    /* Parameters from DDR3 Specifications */
#define DDR3_tREFI         7800000    /* 7.8 us */
#define DDR3_ZQCS          80000ull   /* 80 ns */
#define DDR3_ZQCS_Interval 1280000000 /* 128ms/100 */
#define DDR3_tCKE          5000       /* 5 ns */
#define DDR3_tMRD          4          /* 4 nCK */
#define DDR3_tDLLK         512        /* 512 nCK */
#define DDR3_tMPRR         1          /* 1 nCK */
#define DDR3_tWLMRD        40         /* 40 nCK */
#define DDR3_tWLDQSEN      25         /* 25 nCK */

    /* Parameters from DDR4 Specifications */
#define DDR4_tMRD          8          /* 8 nCK */
#define DDR4_tDLLK         768        /* 768 nCK */

    //int init_loops = 20;
    //while (init_loops--){
    /*
     * 4.8.5 Early LMC Initialization
     *
     * All of DDR PLL, LMC CK, and LMC DRESET initializations must be
     * completed prior to starting this LMC initialization sequence.
     *
     * Perform the following five substeps for early LMC initialization:
     *
     * 1. Software must ensure there are no pending DRAM transactions.
     *
     * 2. Write LMC(0)_CONFIG, LMC(0)_CONTROL, LMC(0)_TIMING_PARAMS0,
     *    LMC(0)_TIMING_PARAMS1, LMC(0)_MODEREG_PARAMS0,
     *    LMC(0)_MODEREG_PARAMS1, LMC(0)_DUAL_MEMCFG, LMC(0)_NXM,
     *    LMC(0)_WODT_MASK, LMC(0)_RODT_MASK, LMC(0)_COMP_CTL2,
     *    LMC(0)_PHY_CTL, LMC(0)_DIMM0/1_PARAMS, and LMC(0)_DIMM_CTL with
     *    appropriate values. All sections in this chapter can be used to
     *    derive proper register settings.
     */

    /* LMC(0)_CONFIG */
    {
        /* .cn70xx. */
        cvmx_lmcx_config_t lmcx_config;

        lmcx_config.u64 = 0;

        lmcx_config.cn78xx.ecc_ena         = use_ecc;
        lmcx_config.cn78xx.row_lsb         = encode_row_lsb_ddr3(cpu_id, row_lsb, ddr_interface_64b);
        lmcx_config.cn78xx.pbank_lsb       = encode_pbank_lsb_ddr3(cpu_id, pbank_lsb, ddr_interface_64b);

        lmcx_config.cn78xx.idlepower       = 0; /* Disabled */

        if ((s = lookup_env_parameter("ddr_idlepower")) != NULL) {
            lmcx_config.cn78xx.idlepower = simple_strtoul(s, NULL, 0);
        }

        lmcx_config.cn78xx.forcewrite      = 0; /* Disabled */
        lmcx_config.cn78xx.ecc_adr         = 1; /* Include memory reference address in the ECC */

        if ((s = lookup_env_parameter("ddr_ecc_adr")) != NULL) {
            lmcx_config.cn78xx.ecc_adr = simple_strtoul(s, NULL, 0);
        }

        lmcx_config.cn78xx.reset           = 0;

        /*
         *  Program LMC0_CONFIG[24:18], ref_zqcs_int(6:0) to
         *  RND-DN(tREFI/clkPeriod/512) Program LMC0_CONFIG[36:25],
         *  ref_zqcs_int(18:7) to
         *  RND-DN(ZQCS_Interval/clkPeriod/(512*128)). Note that this
         *  value should always be greater than 32, to account for
         *  resistor calibration delays.
         */

        lmcx_config.cn78xx.ref_zqcs_int     = ((DDR3_tREFI/tclk_psecs/512) & 0x7f);
        lmcx_config.cn78xx.ref_zqcs_int    |= ((max(33ull, (DDR3_ZQCS_Interval/(tclk_psecs/100)/(512*128))) & 0xfff) << 7);


        lmcx_config.cn78xx.early_dqx       = 1; /* Default to enabled */

        if ((s = lookup_env_parameter("ddr_early_dqx")) == NULL)
            s = lookup_env_parameter("ddr%d_early_dqx", ddr_interface_num);
        if (s != NULL) {
            lmcx_config.cn78xx.early_dqx = simple_strtoul(s, NULL, 0);
        }

        lmcx_config.cn78xx.sref_with_dll        = 0;

        lmcx_config.cn78xx.rank_ena        = bunk_enable;
        lmcx_config.cn78xx.rankmask        = rank_mask; /* Set later */
        lmcx_config.cn78xx.mirrmask        = (spd_addr_mirror << 1 | spd_addr_mirror << 3) & rank_mask;
        lmcx_config.cn78xx.init_status     = rank_mask; /* Set once and don't change it. */
        lmcx_config.cn78xx.early_unload_d0_r0   = 0;
        lmcx_config.cn78xx.early_unload_d0_r1   = 0;
        lmcx_config.cn78xx.early_unload_d1_r0   = 0;
        lmcx_config.cn78xx.early_unload_d1_r1   = 0;
        lmcx_config.cn78xx.scrz                 = 0;
        if (octeon_is_cpuid(OCTEON_CN70XX)) {
            lmcx_config.cn78xx.mode32b          = 1; /* Read-only. Always 1. */
        }
        lmcx_config.cn78xx.mode_x4dev           = (dram_width == 4) ? 1 : 0;
        lmcx_config.cn78xx.bg2_enable           = ((ddr_type == DDR4_DRAM) && (dram_width == 16)) ? 0 : 1;

        if ((s = lookup_env_parameter_ull("ddr_config")) != NULL) {
            lmcx_config.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_CONFIG                                    : 0x%016llx\n", lmcx_config.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmcx_config.u64);
    }

    /* LMC(0)_CONTROL */
    {
        /* .s. */
        cvmx_lmcx_control_t lmc_control;
        lmc_control.u64 = cvmx_read_csr_node(node,
					     CVMX_LMCX_CONTROL(ddr_interface_num));
        lmc_control.s.rdimm_ena       = spd_rdimm;
        lmc_control.s.bwcnt           = 0; /* Clear counter later */
        if (spd_rdimm)
            lmc_control.s.ddr2t       = (safe_ddr_flag ? 1 : custom_lmc_config->ddr2t_rdimm );
        else
            lmc_control.s.ddr2t       = (safe_ddr_flag ? 1 : custom_lmc_config->ddr2t_udimm );
        lmc_control.s.pocas           = 0;
        lmc_control.s.fprch2          = (safe_ddr_flag ? 2 : custom_lmc_config->fprch2 );
        lmc_control.s.throttle_rd     = safe_ddr_flag ? 1 : 0;
        lmc_control.s.throttle_wr     = safe_ddr_flag ? 1 : 0;
        lmc_control.s.inorder_rd      = safe_ddr_flag ? 1 : 0;
        lmc_control.s.inorder_wr      = safe_ddr_flag ? 1 : 0;
        lmc_control.s.elev_prio_dis   = safe_ddr_flag ? 1 : 0;
        lmc_control.s.nxm_write_en    = 0; /* discards writes to
                                              addresses that don't exist
                                              in the DRAM */
        lmc_control.s.max_write_batch = 8;
        lmc_control.s.xor_bank        = 1;
        lmc_control.s.auto_dclkdis    = 1;
        lmc_control.s.int_zqcs_dis    = 0;
        lmc_control.s.ext_zqcs_dis    = 0;
        lmc_control.s.bprch           = 1;
        lmc_control.s.wodt_bprch      = 1;
        lmc_control.s.rodt_bprch      = 1;

        if ((s = lookup_env_parameter("ddr_xor_bank")) != NULL) {
            lmc_control.s.xor_bank = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_2t")) != NULL) {
            lmc_control.s.ddr2t = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_fprch2")) != NULL) {
            lmc_control.s.fprch2 = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_bprch")) != NULL) {
            lmc_control.s.bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_wodt_bprch")) != NULL) {
            lmc_control.s.wodt_bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rodt_bprch")) != NULL) {
            lmc_control.s.rodt_bprch = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_int_zqcs_dis")) != NULL) {
            lmc_control.s.int_zqcs_dis = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_ext_zqcs_dis")) != NULL) {
            lmc_control.s.ext_zqcs_dis = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter_ull("ddr_control")) != NULL) {
            lmc_control.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_CONTROL                                   : 0x%016llx\n", lmc_control.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);
    }

    /* LMC(0)_TIMING_PARAMS0 */
    {
        unsigned trp_value;
        cvmx_lmcx_timing_params0_t lmc_timing_params0;
        lmc_timing_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS0(ddr_interface_num));

        trp_value = divide_roundup(trp, tclk_psecs) - 1;
        ddr_print("TIMING_PARAMS0[TRP]: NEW 0x%x, OLD 0x%x\n", trp_value,
                  trp_value + (unsigned)(divide_roundup(max(4*tclk_psecs, 7500ull), tclk_psecs)) - 4);
#if 1
        if ((s = lookup_env_parameter_ull("ddr_use_old_trp")) != NULL) {
            if (!!simple_strtoull(s, NULL, 0)) {
                trp_value += divide_roundup(max(4*tclk_psecs, 7500ull), tclk_psecs) - 4;
                ddr_print("TIMING_PARAMS0[trp]: USING OLD 0x%x\n", trp_value);
            }
        }
#endif

        lmc_timing_params0.cn78xx.txpr     = divide_roundup(max(5*tclk_psecs, trfc+10000ull), 16*tclk_psecs);
        lmc_timing_params0.cn78xx.trp      = trp_value & 0x1f;
        lmc_timing_params0.cn78xx.tcksre   = divide_roundup(max(5*tclk_psecs, 10000ull), tclk_psecs) - 1;

        if (ddr_type == DDR4_DRAM) {
            int tzqinit = 4; // Default to 4, for all DDR4 speed bins
            if ((s = lookup_env_parameter("ddr_tzqinit")) != NULL) {
                tzqinit = simple_strtoul(s, NULL, 0);
            }
            lmc_timing_params0.cn78xx.tzqinit  = tzqinit; 
	    lmc_timing_params0.cn78xx.tzqcs    = divide_roundup(128*tclk_psecs, (16*tclk_psecs)); /* Always 8. */
            lmc_timing_params0.cn78xx.tcke     = divide_roundup(max(3*tclk_psecs, (ulong) DDR3_tCKE), tclk_psecs) - 1;
            lmc_timing_params0.cn78xx.tmrd     = divide_roundup((DDR4_tMRD*tclk_psecs), tclk_psecs) - 1;
            //lmc_timing_params0.cn78xx.tmod     = divide_roundup(max(24*tclk_psecs, 15000ull), tclk_psecs) - 1;
            lmc_timing_params0.cn78xx.tmod     = 25; /* 25 is the max allowed */
            lmc_timing_params0.cn78xx.tdllk    = divide_roundup(DDR4_tDLLK, 256);
        } else {
            lmc_timing_params0.cn78xx.tzqinit  = divide_roundup(max(512*tclk_psecs, 640000ull), (256*tclk_psecs));
            lmc_timing_params0.cn78xx.tzqcs    = divide_roundup(max(64*tclk_psecs, DDR3_ZQCS), (16*tclk_psecs));
            lmc_timing_params0.cn78xx.tcke     = divide_roundup(DDR3_tCKE, tclk_psecs) - 1;
            lmc_timing_params0.cn78xx.tmrd     = divide_roundup((DDR3_tMRD*tclk_psecs), tclk_psecs) - 1;
            lmc_timing_params0.cn78xx.tmod     = divide_roundup(max(12*tclk_psecs, 15000ull), tclk_psecs) - 1;
            lmc_timing_params0.cn78xx.tdllk    = divide_roundup(DDR3_tDLLK, 256);
        }

        if ((s = lookup_env_parameter_ull("ddr_timing_params0")) != NULL) {
            lmc_timing_params0.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("TIMING_PARAMS0                                : 0x%016llx\n", lmc_timing_params0.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_TIMING_PARAMS0(ddr_interface_num),
				  lmc_timing_params0.u64);
    }

    /* LMC(0)_TIMING_PARAMS1 */
    {
        unsigned txp, temp_trcd, trfc_dlr;
        cvmx_lmcx_timing_params1_t lmc_timing_params1;
        lmc_timing_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));


        /* .cn70xx. */
        lmc_timing_params1.s.tmprr    = divide_roundup(DDR3_tMPRR*tclk_psecs, tclk_psecs) - 1;

        lmc_timing_params1.cn78xx.tras     = divide_roundup(tras, tclk_psecs) - 1;

        temp_trcd = divide_roundup(trcd, tclk_psecs);
#if 1
        if (temp_trcd > 15)
            ddr_print("TIMING_PARAMS1[trcd]: need extension bit for 0x%x\n", temp_trcd);
#endif
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && (temp_trcd > 15)) {
            /* Let .trcd=0 serve as a flag that the field has
               overflowed. Must use Additive Latency mode as a
               workaround. */
            temp_trcd = 0;
        }
        lmc_timing_params1.cn78xx.trcd     = (temp_trcd >> 0) & 0xf;
        lmc_timing_params1.cn78xx.trcd_ext = (temp_trcd >> 4) & 0x1;

        lmc_timing_params1.cn78xx.twtr     = divide_roundup(twtr, tclk_psecs) - 1;
        lmc_timing_params1.cn78xx.trfc     = divide_roundup(trfc, 8*tclk_psecs);

        if (ddr_type == DDR4_DRAM) {
            /* Workaround bug 24006. Use Trrd_l. */
            lmc_timing_params1.cn78xx.trrd     = divide_roundup(ddr4_tRRD_Lmin, tclk_psecs) - 2;
        } else
            lmc_timing_params1.cn78xx.trrd     = divide_roundup(trrd, tclk_psecs) - 2;

        /*
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-800   tCLK = 2500 psec
        ** tXP = max( 3nCK, 7.5 ns)     DDR3-1066  tCLK = 1875 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1333  tCLK = 1500 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1600  tCLK = 1250 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-1866  tCLK = 1071 psec
        ** tXP = max( 3nCK, 6.0 ns)     DDR3-2133  tCLK =  937 psec
        */
        txp = (tclk_psecs < 1875) ? 6000 : 7500;
        txp = divide_roundup(max(3*tclk_psecs, txp), tclk_psecs) - 1;
#if 1
        if (txp > 7)
            ddr_print("TIMING_PARAMS1[txp]: need extension bit for 0x%x\n", txp);
#endif
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && (txp > 7)) {
            txp = 7; // max it out
        }
        lmc_timing_params1.cn78xx.txp      = (txp >> 0) & 7;
        lmc_timing_params1.cn78xx.txp_ext  = (txp >> 3) & 1;

        lmc_timing_params1.cn78xx.twlmrd   = divide_roundup(DDR3_tWLMRD*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.twldqsen = divide_roundup(DDR3_tWLDQSEN*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.tfaw     = divide_roundup(tfaw, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.txpdll   = divide_roundup(max(10*tclk_psecs, 24000ull), tclk_psecs) - 1;

        if ((ddr_type == DDR4_DRAM) && is_3ds_dimm) {
            /*
              4 Gb: tRFC_DLR = 90 ns
              8 Gb: tRFC_DLR = 120 ns
              16 Gb: tRFC_DLR = 190 ns FIXME?
             */
            // RNDUP[tRFC_DLR(ns) / (8 * TCYC(ns))]
            if (die_capacity == 0x1000) // 4 Gbit
                trfc_dlr = 90;
            else if (die_capacity == 0x2000) // 8 Gbit
                trfc_dlr = 120;
            else if (die_capacity == 0x4000) // 16 Gbit
                trfc_dlr = 190;
            else
                trfc_dlr = 0;

            if (trfc_dlr == 0) {
                ddr_print("N%d.LMC%d: ERROR: tRFC_DLR: die_capacity %u Mbit is illegal\n",
                          node, ddr_interface_num, die_capacity);
            } else {
                lmc_timing_params1.cn78xx.trfc_dlr = divide_roundup(trfc_dlr * 1000UL, 8*tclk_psecs);
                ddr_print("N%d.LMC%d: TIMING_PARAMS1[trfc_dlr] set to %u\n",
                          node, ddr_interface_num, lmc_timing_params1.cn78xx.trfc_dlr);
            }
        }

        if ((s = lookup_env_parameter_ull("ddr_timing_params1")) != NULL) {
            lmc_timing_params1.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("TIMING_PARAMS1                                : 0x%016llx\n", lmc_timing_params1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num),
				  lmc_timing_params1.u64);
    }

    /* LMC(0)_TIMING_PARAMS2 */
    if (ddr_type == DDR4_DRAM) {
        cvmx_lmcx_timing_params1_t lmc_timing_params1;
        cvmx_lmcx_timing_params2_t lmc_timing_params2;
        int temp_trrd_l;

        lmc_timing_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));
        lmc_timing_params2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS2(ddr_interface_num));
        ddr_print("TIMING_PARAMS2                                : 0x%016llx\n", lmc_timing_params2.u64);

        temp_trrd_l = divide_roundup(ddr4_tRRD_Lmin, tclk_psecs) - 2;
#if 1
        if (temp_trrd_l > 7)
            ddr_print("TIMING_PARAMS2[trrd_l]: need extension bit for 0x%x\n", temp_trrd_l);
#endif
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && (temp_trrd_l > 7)) {
            temp_trrd_l = 7; // max it out
        }
        lmc_timing_params2.cn78xx.trrd_l      = (temp_trrd_l >> 0) & 7;
        lmc_timing_params2.cn78xx.trrd_l_ext  = (temp_trrd_l >> 3) & 1;


        lmc_timing_params2.s.twtr_l = divide_nint(max(4*tclk_psecs, 7500ull), tclk_psecs) - 1; // correct for 1600-2400
        lmc_timing_params2.s.t_rw_op_max = 7;
        lmc_timing_params2.s.trtp = divide_roundup(max(4*tclk_psecs, 7500ull), tclk_psecs) - 1;

        ddr_print("TIMING_PARAMS2                                : 0x%016llx\n", lmc_timing_params2.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_TIMING_PARAMS2(ddr_interface_num),
				  lmc_timing_params2.u64);

        /* Workaround Errata 25823 - LMC: Possible DDR4 tWTR_L not met
           for Write-to-Read operations to the same Bank Group */
        if (lmc_timing_params1.cn78xx.twtr < (lmc_timing_params2.s.twtr_l - 4)) {
            lmc_timing_params1.cn78xx.twtr = lmc_timing_params2.s.twtr_l - 4;
	    ddr_print("ERRATA 25823: NEW: TWTR: %d, TWTR_L: %d\n",
                      lmc_timing_params1.cn78xx.twtr, lmc_timing_params2.s.twtr_l);
            ddr_print("TIMING_PARAMS1                                : 0x%016llx\n", lmc_timing_params1.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num),
                                      lmc_timing_params1.u64);
        }
    }

    /* LMC(0)_MODEREG_PARAMS0 */
    {
        /* .s. */
        cvmx_lmcx_modereg_params0_t lmc_modereg_params0;
        int param;

        lmc_modereg_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));


        if (ddr_type == DDR4_DRAM) {
            lmc_modereg_params0.s.cwl     = 0; /* 1600 (1250ps) */
            if (tclk_psecs < 1250)
                lmc_modereg_params0.s.cwl = 1; /* 1866 (1072ps) */
            if (tclk_psecs < 1072)
                lmc_modereg_params0.s.cwl = 2; /* 2133 (938ps) */
            if (tclk_psecs < 938)
                lmc_modereg_params0.s.cwl = 3; /* 2400 (833ps) */
            if (tclk_psecs < 833)
                lmc_modereg_params0.s.cwl = 4; /* 2666 (750ps) */
            if (tclk_psecs < 750)
                lmc_modereg_params0.s.cwl = 5; /* 3200 (625ps) */
        } else {
            /*
            ** CSR   CWL         CAS write Latency
            ** ===   ===   =================================
            **  0      5   (           tCK(avg) >=   2.5 ns)
            **  1      6   (2.5 ns   > tCK(avg) >= 1.875 ns)
            **  2      7   (1.875 ns > tCK(avg) >= 1.5   ns)
            **  3      8   (1.5 ns   > tCK(avg) >= 1.25  ns)
            **  4      9   (1.25 ns  > tCK(avg) >= 1.07  ns)
            **  5     10   (1.07 ns  > tCK(avg) >= 0.935 ns)
            **  6     11   (0.935 ns > tCK(avg) >= 0.833 ns)
            **  7     12   (0.833 ns > tCK(avg) >= 0.75  ns)
            */

            lmc_modereg_params0.s.cwl     = 0;
            if (tclk_psecs < 2500)
                lmc_modereg_params0.s.cwl = 1;
            if (tclk_psecs < 1875)
                lmc_modereg_params0.s.cwl = 2;
            if (tclk_psecs < 1500)
                lmc_modereg_params0.s.cwl = 3;
            if (tclk_psecs < 1250)
                lmc_modereg_params0.s.cwl = 4;
            if (tclk_psecs < 1070)
                lmc_modereg_params0.s.cwl = 5;
            if (tclk_psecs <  935)
                lmc_modereg_params0.s.cwl = 6;
            if (tclk_psecs <  833)
                lmc_modereg_params0.s.cwl = 7;
        }

        if ((s = lookup_env_parameter("ddr_cwl")) != NULL) {
            lmc_modereg_params0.s.cwl = simple_strtoul(s, NULL, 0) - 5;
        }

        if (ddr_type == DDR4_DRAM) {
            ddr_print("%-45s : %d, [0x%x]\n", "CAS Write Latency CWL, [CSR]",
                      lmc_modereg_params0.s.cwl + 9
                      + ((lmc_modereg_params0.s.cwl>2) ? (lmc_modereg_params0.s.cwl-3) * 2 : 0),
                      lmc_modereg_params0.s.cwl);
        } else {
            ddr_print("%-45s : %d, [0x%x]\n", "CAS Write Latency CWL, [CSR]",
                      lmc_modereg_params0.s.cwl + 5,
                      lmc_modereg_params0.s.cwl);
        }

        lmc_modereg_params0.s.mprloc  = 0;
        lmc_modereg_params0.s.mpr     = 0;
        lmc_modereg_params0.s.dll     = (ddr_type == DDR4_DRAM); /* 0 for DDR3 and 1 for DDR4 */
        lmc_modereg_params0.s.al      = 0;
        lmc_modereg_params0.s.wlev    = 0; /* Read Only */
        if (octeon_is_cpuid(OCTEON_CN70XX) || (ddr_type == DDR4_DRAM)) {
            lmc_modereg_params0.s.tdqs    = 0;
        } else {
            lmc_modereg_params0.s.tdqs    = 1;
        }
        lmc_modereg_params0.s.qoff    = 0;
        //lmc_modereg_params0.s.bl      = 0; /* Don't touch block dirty logic */

        if ((s = lookup_env_parameter("ddr_cl")) != NULL) {
            CL = simple_strtoul(s, NULL, 0);
            ddr_print("CAS Latency                                   : %6d\n", CL);
        }

        if (ddr_type == DDR4_DRAM) {
            lmc_modereg_params0.s.cl      = 0x0;
            if (CL > 9)
                lmc_modereg_params0.s.cl  = 0x1;
            if (CL > 10)
                lmc_modereg_params0.s.cl  = 0x2;
            if (CL > 11)
                lmc_modereg_params0.s.cl  = 0x3;
            if (CL > 12)
                lmc_modereg_params0.s.cl  = 0x4;
            if (CL > 13)
                lmc_modereg_params0.s.cl  = 0x5;
            if (CL > 14)
                lmc_modereg_params0.s.cl  = 0x6;
            if (CL > 15)
                lmc_modereg_params0.s.cl  = 0x7;
            if (CL > 16)
                lmc_modereg_params0.s.cl  = 0x8;
            if (CL > 18)
                lmc_modereg_params0.s.cl  = 0x9;
            if (CL > 20)
                lmc_modereg_params0.s.cl  = 0xA;
            if (CL > 24)
                lmc_modereg_params0.s.cl  = 0xB;
        } else {
            lmc_modereg_params0.s.cl      = 0x2;
            if (CL > 5)
                lmc_modereg_params0.s.cl  = 0x4;
            if (CL > 6)
                lmc_modereg_params0.s.cl  = 0x6;
            if (CL > 7)
                lmc_modereg_params0.s.cl  = 0x8;
            if (CL > 8)
                lmc_modereg_params0.s.cl  = 0xA;
            if (CL > 9)
                lmc_modereg_params0.s.cl  = 0xC;
            if (CL > 10)
                lmc_modereg_params0.s.cl  = 0xE;
            if (CL > 11)
                lmc_modereg_params0.s.cl  = 0x1;
            if (CL > 12)
                lmc_modereg_params0.s.cl  = 0x3;
            if (CL > 13)
                lmc_modereg_params0.s.cl  = 0x5;
            if (CL > 14)
                lmc_modereg_params0.s.cl  = 0x7;
            if (CL > 15)
                lmc_modereg_params0.s.cl  = 0x9;
        }

        lmc_modereg_params0.s.rbt     = 0; /* Read Only. */
        lmc_modereg_params0.s.tm      = 0;
        lmc_modereg_params0.s.dllr    = 0;

        param = divide_roundup(twr, tclk_psecs);

        if (ddr_type == DDR4_DRAM) {    /* DDR4 */
            lmc_modereg_params0.s.wrp     = 1;
            if (param > 12)
                lmc_modereg_params0.s.wrp = 2;
            if (param > 14)
                lmc_modereg_params0.s.wrp = 3;
            if (param > 16)
                lmc_modereg_params0.s.wrp = 4;
            if (param > 18)
                lmc_modereg_params0.s.wrp = 5;
            if (param > 20)
                lmc_modereg_params0.s.wrp = 6;
            if (param > 24)         /* RESERVED in DDR4 spec */
                lmc_modereg_params0.s.wrp = 7;
        } else {                /* DDR3 */
            lmc_modereg_params0.s.wrp     = 1;
            if (param > 5)
                lmc_modereg_params0.s.wrp = 2;
            if (param > 6)
                lmc_modereg_params0.s.wrp = 3;
            if (param > 7)
                lmc_modereg_params0.s.wrp = 4;
            if (param > 8)
                lmc_modereg_params0.s.wrp = 5;
            if (param > 10)
                lmc_modereg_params0.s.wrp = 6;
            if (param > 12)
                lmc_modereg_params0.s.wrp = 7;
        }

        lmc_modereg_params0.s.ppd     = 0;

        if ((s = lookup_env_parameter("ddr_wrp")) != NULL) {
            lmc_modereg_params0.s.wrp = simple_strtoul(s, NULL, 0);
        }

        ddr_print("%-45s : %d, [0x%x]\n", "Write recovery for auto precharge WRP, [CSR]",
                  param, lmc_modereg_params0.s.wrp);

        if ((s = lookup_env_parameter_ull("ddr_modereg_params0")) != NULL) {
            lmc_modereg_params0.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("MODEREG_PARAMS0                               : 0x%016llx\n",
		  lmc_modereg_params0.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
				  lmc_modereg_params0.u64);
    }

    /* LMC(0)_MODEREG_PARAMS1 */
    {
        /* .s. */
        cvmx_lmcx_modereg_params1_t lmc_modereg_params1;
        lmc_modereg_params1.u64 = odt_config[odt_idx].modereg_params1.u64;


        if ((s = lookup_env_parameter("ddr_rtt_nom_mask")) != NULL) {
            dyn_rtt_nom_mask    = simple_strtoul(s, NULL, 0);
        }


        /* Save the original rtt_nom settings before sweeping through settings. */
        default_rtt_nom[0] = lmc_modereg_params1.s.rtt_nom_00;
        default_rtt_nom[1] = lmc_modereg_params1.s.rtt_nom_01;
        default_rtt_nom[2] = lmc_modereg_params1.s.rtt_nom_10;
        default_rtt_nom[3] = lmc_modereg_params1.s.rtt_nom_11;

        ddr_rtt_nom_auto = custom_lmc_config->ddr_rtt_nom_auto;

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_nom_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_nom_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x7  << (i*12+9));
                lmc_modereg_params1.u64 |=  ( (value & 0x7) << (i*12+9));
                default_rtt_nom[i] = value;
                ddr_rtt_nom_auto = 0;
            }
        }


        if ((s = lookup_env_parameter("ddr_rtt_nom")) == NULL)
            s = lookup_env_parameter("ddr%d_rtt_nom", ddr_interface_num);
        if (s != NULL) {
            uint64_t value;
            value = simple_strtoul(s, NULL, 0);


            if (dyn_rtt_nom_mask & 1)
                default_rtt_nom[0] = lmc_modereg_params1.s.rtt_nom_00 = value;
            if (dyn_rtt_nom_mask & 2)
                default_rtt_nom[1] = lmc_modereg_params1.s.rtt_nom_01 = value;
            if (dyn_rtt_nom_mask & 4)
                default_rtt_nom[2] = lmc_modereg_params1.s.rtt_nom_10 = value;
            if (dyn_rtt_nom_mask & 8)
                default_rtt_nom[3] = lmc_modereg_params1.s.rtt_nom_11 = value;

            ddr_rtt_nom_auto = 0;
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_wr_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_wr_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                INSRT_WR(&lmc_modereg_params1.u64, i, value);
            }
        }

        // Make sure 78XX pass 1 has valid RTT_WR settings, because
        // configuration files may be set-up for later chips, and
        // 78XX pass 1 supports no RTT_WR extension bits
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
            for (i = 0; i < 4; ++i) {
                if (EXTR_WR(lmc_modereg_params1.u64, i) > 3) { // if 80 or undefined
                    INSRT_WR(&lmc_modereg_params1.u64, i, 1); // FIXME? always insert 120
                    ddr_print("RTT_WR_%d%d set to 120 for CN78XX pass 1\n", !!(i&2), i&1);
                }
            }
        }

        if ((s = lookup_env_parameter("ddr_dic")) != NULL) {
            uint64_t value = simple_strtoul(s, NULL, 0);
            for (i=0; i<4; ++i) {
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+7));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+7));
            }
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_dic_%1d%1d", !!(i&2), !!(i&1))) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+7));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+7));
            }
        }

        if ((s = lookup_env_parameter_ull("ddr_modereg_params1")) != NULL) {
            lmc_modereg_params1.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
                  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
                  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
                  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
                  lmc_modereg_params1.s.rtt_nom_11,
                  lmc_modereg_params1.s.rtt_nom_10,
                  lmc_modereg_params1.s.rtt_nom_01,
                  lmc_modereg_params1.s.rtt_nom_00);

        ddr_print("RTT_WR      %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 3)],
                  imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 2)],
                  imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 1)],
                  imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 0)],
                  EXTR_WR(lmc_modereg_params1.u64, 3),
                  EXTR_WR(lmc_modereg_params1.u64, 2),
                  EXTR_WR(lmc_modereg_params1.u64, 1),
                  EXTR_WR(lmc_modereg_params1.u64, 0));

        ddr_print("DIC         %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  imp_values->dic_ohms[lmc_modereg_params1.s.dic_11],
                  imp_values->dic_ohms[lmc_modereg_params1.s.dic_10],
                  imp_values->dic_ohms[lmc_modereg_params1.s.dic_01],
                  imp_values->dic_ohms[lmc_modereg_params1.s.dic_00],
                  lmc_modereg_params1.s.dic_11,
                  lmc_modereg_params1.s.dic_10,
                  lmc_modereg_params1.s.dic_01,
                  lmc_modereg_params1.s.dic_00);

        ddr_print("MODEREG_PARAMS1                               : 0x%016llx\n", lmc_modereg_params1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num),
				  lmc_modereg_params1.u64);
    }

    if (ddr_type == DDR4_DRAM) {
        cvmx_lmcx_modereg_params2_t lmc_modereg_params2;
        lmc_modereg_params2.u64 = odt_config[odt_idx].modereg_params2.u64;

#if 0
        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_vref_value_%1d%1d", !!(i&2), !!(i&1))) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params2.u64 &= ~((uint64_t)0x3f  << (i*10+3));
                lmc_modereg_params2.u64 |=  ( (value & 0x3f) << (i*10+3));
            }
        }
#endif
        if ((s = lookup_env_parameter("ddr_rtt_park")) != NULL) {
            uint64_t value = simple_strtoul(s, NULL, 0);
            for (i=0; i<4; ++i) {
                lmc_modereg_params2.u64 &= ~((uint64_t)0x7  << (i*10+0));
                lmc_modereg_params2.u64 |=  ( (value & 0x7) << (i*10+0));
            }
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_park_%1d%1d", !!(i&2), !!(i&1))) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params2.u64 &= ~((uint64_t)0x7  << (i*10+0));
                lmc_modereg_params2.u64 |=  ( (value & 0x7) << (i*10+0));
            }
        }

        if ((s = lookup_env_parameter_ull("ddr_modereg_params2")) != NULL) {
            lmc_modereg_params2.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("RTT_PARK    %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                  imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_11],
                  imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_10],
                  imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_01],
                  imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_00],
                  lmc_modereg_params2.s.rtt_park_11,
                  lmc_modereg_params2.s.rtt_park_10,
                  lmc_modereg_params2.s.rtt_park_01,
                  lmc_modereg_params2.s.rtt_park_00);

        ddr_print("%-45s :  0x%x,0x%x,0x%x,0x%x\n", "VREF_RANGE",
                  lmc_modereg_params2.s.vref_range_11,
                  lmc_modereg_params2.s.vref_range_10,
                  lmc_modereg_params2.s.vref_range_01,
                  lmc_modereg_params2.s.vref_range_00);

        ddr_print("%-45s :  0x%x,0x%x,0x%x,0x%x\n", "VREF_VALUE",
                  lmc_modereg_params2.s.vref_value_11,
                  lmc_modereg_params2.s.vref_value_10,
                  lmc_modereg_params2.s.vref_value_01,
                  lmc_modereg_params2.s.vref_value_00);

        ddr_print("MODEREG_PARAMS2                               : 0x%016llx\n", lmc_modereg_params2.u64);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS2(ddr_interface_num),
                                  lmc_modereg_params2.u64);
    }

    /* LMC(0)_MODEREG_PARAMS3 */
    if (ddr_type == DDR4_DRAM) {
        cvmx_lmcx_modereg_params3_t lmc_modereg_params3;

        lmc_modereg_params3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS3(ddr_interface_num));

        //lmc_modereg_params3.s.max_pd          =
        //lmc_modereg_params3.s.tc_ref          =
        //lmc_modereg_params3.s.vref_mon        =
        //lmc_modereg_params3.s.cal             =
        //lmc_modereg_params3.s.sre_abort       =
        //lmc_modereg_params3.s.rd_preamble     =
        //lmc_modereg_params3.s.wr_preamble     =
        //lmc_modereg_params3.s.par_lat_mode    =
        //lmc_modereg_params3.s.odt_pd          =
        //lmc_modereg_params3.s.ca_par_pers     =
        //lmc_modereg_params3.s.dm              =
        //lmc_modereg_params3.s.wr_dbi          =
        lmc_modereg_params3.s.rd_dbi          = 0; /* Disable as workaround to Errata 20547 */
        lmc_modereg_params3.s.tccd_l            = max(divide_roundup(ddr4_tCCD_Lmin, tclk_psecs), 5ull) - 4;
        //lmc_modereg_params3.s.lpasr           =
        //lmc_modereg_params3.s.crc             =
        //lmc_modereg_params3.s.gd              =
        //lmc_modereg_params3.s.pda             =
        //lmc_modereg_params3.s.temp_sense      =
        //lmc_modereg_params3.s.fgrm            =
        //lmc_modereg_params3.s.wr_cmd_lat      =
        //lmc_modereg_params3.s.mpr_fmt         =

        if (!octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
            int delay = 0;
            if ((lranks_per_prank == 4) && (ddr_hertz >= 1000000000))
                delay = 1;
            lmc_modereg_params3.s.xrank_add_tccd_l = delay;
            lmc_modereg_params3.s.xrank_add_tccd_s = delay;
        }

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS3(ddr_interface_num),
				  lmc_modereg_params3.u64);
        ddr_print("MODEREG_PARAMS3                               : 0x%016llx\n", lmc_modereg_params3.u64);
    }


#if 0
    /* LMC(0)_DUAL_MEMCFG */
    {
        /* .cn70xx. */
        dual_memcfg.cn78xx.cs_mask  = 0;
        dual_memcfg.cn78xx.row_lsb  = 0;
    }
#endif

    /* LMC(0)_NXM */
    {
        cvmx_lmcx_nxm_t lmc_nxm;
        int num_bits = row_lsb + row_bits + lranks_bits - 26;
        lmc_nxm.u64 = cvmx_read_csr_node(node, CVMX_LMCX_NXM(ddr_interface_num));

        /* .cn78xx. */
        if (rank_mask & 0x1)
            lmc_nxm.cn78xx.mem_msb_d0_r0 = num_bits;
        if (rank_mask & 0x2)
            lmc_nxm.cn78xx.mem_msb_d0_r1 = num_bits;
        if (rank_mask & 0x4)
            lmc_nxm.cn78xx.mem_msb_d1_r0 = num_bits;
        if (rank_mask & 0x8)
            lmc_nxm.cn78xx.mem_msb_d1_r1 = num_bits;

        lmc_nxm.cn78xx.cs_mask = ~rank_mask & 0xff; /* Set the mask for non-existant ranks. */

        if ((s = lookup_env_parameter_ull("ddr_nxm")) != NULL) {
            lmc_nxm.u64    = simple_strtoull(s, NULL, 0);
        }
        ddr_print("LMC_NXM                                       : 0x%016llx\n", lmc_nxm.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_NXM(ddr_interface_num),
				  lmc_nxm.u64);
    }

    /* LMC(0)_WODT_MASK */
    {
        cvmx_lmcx_wodt_mask_t lmc_wodt_mask;
        lmc_wodt_mask.u64 = odt_config[odt_idx].odt_mask;

        if ((s = lookup_env_parameter_ull("ddr_wodt_mask")) != NULL) {
            lmc_wodt_mask.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("WODT_MASK                                     : 0x%016llx\n", lmc_wodt_mask.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_WODT_MASK(ddr_interface_num),
				  lmc_wodt_mask.u64);
    }

    /* LMC(0)_RODT_MASK */
    {
        int rankx;
        cvmx_lmcx_rodt_mask_t lmc_rodt_mask;
        lmc_rodt_mask.u64 = odt_config[odt_idx].rodt_ctl;

        if ((s = lookup_env_parameter_ull("ddr_rodt_mask")) != NULL) {
            lmc_rodt_mask.u64    = simple_strtoull(s, NULL, 0);
        }

        ddr_print("%-45s : 0x%016llx\n", "RODT_MASK", lmc_rodt_mask.u64);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_RODT_MASK(ddr_interface_num),
                                  lmc_rodt_mask.u64);

        dyn_rtt_nom_mask = 0;
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            if (!(rank_mask & (1 << rankx)))
                continue;
            dyn_rtt_nom_mask |= ((lmc_rodt_mask.u64 >> (8*rankx)) & 0xff);
        }
        if (num_ranks == 4) {
            /* Normally ODT1 is wired to rank 1. For quad-ranked DIMMs
               ODT1 is wired to the third rank (rank 2).  The mask,
               dyn_rtt_nom_mask, is used to indicate for which ranks
               to sweep RTT_NOM during read-leveling. Shift the bit
               from the ODT1 position over to the "ODT2" position so
               that the read-leveling analysis comes out right. */
            int odt1_bit = dyn_rtt_nom_mask & 2;
            dyn_rtt_nom_mask &= ~2;
            dyn_rtt_nom_mask |= odt1_bit<<1;
        }
        ddr_print("%-45s : 0x%02x\n", "DYN_RTT_NOM_MASK", dyn_rtt_nom_mask);
    }

    /* LMC(0)_COMP_CTL2 */
    {
        /* .cn70xx. */

        cvmx_lmcx_comp_ctl2_t comp_ctl2;
        const ddr3_custom_config_t *custom_lmc_config = &ddr_configuration->custom_lmc_config;

        comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));

        comp_ctl2.cn78xx.dqx_ctl	= odt_config[odt_idx].odt_ena;
        comp_ctl2.cn78xx.ck_ctl		= (custom_lmc_config->ck_ctl  == 0) ? 4 : custom_lmc_config->ck_ctl;  /* Default 4=34.3 ohm */
        comp_ctl2.cn78xx.cmd_ctl	= (custom_lmc_config->cmd_ctl == 0) ? 4 : custom_lmc_config->cmd_ctl; /* Default 4=34.3 ohm */
        comp_ctl2.cn78xx.control_ctl	= (custom_lmc_config->ctl_ctl == 0) ? 4 : custom_lmc_config->ctl_ctl; /* Default 4=34.3 ohm */

        ddr_rodt_ctl_auto = custom_lmc_config->ddr_rodt_ctl_auto;
        if ((s = lookup_env_parameter("ddr_rodt_ctl_auto")) != NULL) {
            ddr_rodt_ctl_auto = !!simple_strtoul(s, NULL, 0);
        }

        default_rodt_ctl = odt_config[odt_idx].qs_dic;
        if ((s = lookup_env_parameter("ddr_rodt_ctl")) == NULL)
            s = lookup_env_parameter("ddr%d_rodt_ctl", ddr_interface_num);
        if (s != NULL) {
            default_rodt_ctl    = simple_strtoul(s, NULL, 0);
            ddr_rodt_ctl_auto = 0;
        }

        comp_ctl2.cn70xx.rodt_ctl = default_rodt_ctl;

	// if DDR4, force CK_CTL to 26 ohms if it is currently 34 ohms, and DCLK speed is 1 GHz or more...
	if ((ddr_type == DDR4_DRAM) && (comp_ctl2.s.ck_ctl == ddr4_driver_34_ohm) && (ddr_hertz >= 1000000000)) {
	    comp_ctl2.s.ck_ctl = ddr4_driver_26_ohm; // lowest for DDR4 is 26 ohms
	    ddr_print("N%d.LMC%d: Forcing DDR4 COMP_CTL2[CK_CTL] to %d, %d ohms\n",
                      node, ddr_interface_num, comp_ctl2.s.ck_ctl,
		      imp_values->drive_strength[comp_ctl2.s.ck_ctl]);
	}
	// if DDR4, 2DPC, UDIMM, force CONTROL_CTL and CMD_CTL to 26 ohms, if DCLK speed is 1 GHz or more...
	if ((ddr_type == DDR4_DRAM)
            && (dimm_count == 2)
            && ((spd_dimm_type == 2) || (spd_dimm_type == 6))
            && (ddr_hertz >= 1000000000))
        {
	    comp_ctl2.cn78xx.control_ctl = ddr4_driver_26_ohm; // lowest for DDR4 is 26 ohms
	    comp_ctl2.cn78xx.cmd_ctl     = ddr4_driver_26_ohm; // lowest for DDR4 is 26 ohms
	    ddr_print("N%d.LMC%d: Forcing DDR4 COMP_CTL2[CONTROL_CTL,CMD_CTL] to %d, %d ohms\n",
                      node, ddr_interface_num, ddr4_driver_26_ohm,
		      imp_values->drive_strength[ddr4_driver_26_ohm]);
	}

        if ((s = lookup_env_parameter("ddr_ck_ctl")) != NULL) {
            comp_ctl2.cn78xx.ck_ctl  = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_cmd_ctl")) != NULL) {
            comp_ctl2.cn78xx.cmd_ctl  = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_control_ctl")) != NULL) {
            comp_ctl2.cn70xx.control_ctl  = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_dqx_ctl")) != NULL) {
            comp_ctl2.cn78xx.dqx_ctl  = simple_strtoul(s, NULL, 0);
        }

        ddr_print("%-45s : %d, %d ohms\n", "DQX_CTL           ", comp_ctl2.cn78xx.dqx_ctl    , imp_values->drive_strength[comp_ctl2.cn78xx.dqx_ctl    ]);
        ddr_print("%-45s : %d, %d ohms\n", "CK_CTL            ", comp_ctl2.cn78xx.ck_ctl     , imp_values->drive_strength[comp_ctl2.cn78xx.ck_ctl     ]);
        ddr_print("%-45s : %d, %d ohms\n", "CMD_CTL           ", comp_ctl2.cn78xx.cmd_ctl    , imp_values->drive_strength[comp_ctl2.cn78xx.cmd_ctl    ]);
        ddr_print("%-45s : %d, %d ohms\n", "CONTROL_CTL       ", comp_ctl2.cn78xx.control_ctl, imp_values->drive_strength[comp_ctl2.cn78xx.control_ctl]);
	ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
		  comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[comp_ctl2.cn78xx.rodt_ctl]);

        ddr_print("%-45s : 0x%016llx\n", "COMP_CTL2", comp_ctl2.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				  comp_ctl2.u64);
    }

    /* LMC(0)_PHY_CTL */
    {
        /* .s. */
        cvmx_lmcx_phy_ctl_t lmc_phy_ctl;
        lmc_phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
        lmc_phy_ctl.s.ts_stagger           = 0;

#if 0
        if ((s = lookup_env_parameter_ull("ddr_phy_ctl")) != NULL) {
            lmc_phy_ctl.u64    = simple_strtoull(s, NULL, 0);
        }
#endif
        if (!octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && (lranks_per_prank > 1)) {
            lmc_phy_ctl.s.c0_sel = lmc_phy_ctl.s.c1_sel = 2; // C0 is TEN, C1 is A17
            ddr_print("N%d.LMC%d: 3DS: setting PHY_CTL[cx_csel] = %d\n",
                      node, ddr_interface_num, lmc_phy_ctl.s.c1_sel);
        }

        ddr_print("PHY_CTL                                       : 0x%016llx\n", lmc_phy_ctl.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_PHY_CTL(ddr_interface_num),
				  lmc_phy_ctl.u64);
    }

    /* LMC(0)_DIMM0/1_PARAMS */
    if (spd_rdimm) {
        cvmx_lmcx_dimm_ctl_t lmc_dimm_ctl;

        for (didx=0; didx<(unsigned)dimm_count; ++didx) {
            cvmx_lmcx_dimmx_params_t lmc_dimmx_params;
            int dimmx = didx;
            int rc;

            lmc_dimmx_params.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMMX_PARAMS(dimmx, ddr_interface_num));

            if (ddr_type == DDR4_DRAM) {

                cvmx_lmcx_dimmx_ddr4_params0_t lmc_dimmx_ddr4_params0;
                cvmx_lmcx_dimmx_ddr4_params1_t lmc_dimmx_ddr4_params1;
                cvmx_lmcx_ddr4_dimm_ctl_t lmc_ddr4_dimm_ctl;

                lmc_dimmx_params.s.rc0  = 0;
                lmc_dimmx_params.s.rc1  = 0;
                lmc_dimmx_params.s.rc2  = 0;

                rc = read_spd(&dimm_config_table[didx], 0, DDR4_SPD_RDIMM_REGISTER_DRIVE_STRENGTH_CTL);
                lmc_dimmx_params.s.rc3  = (rc >> 4) & 0xf;
                lmc_dimmx_params.s.rc4  = ((rc >> 0) & 0x3) << 2;
                lmc_dimmx_params.s.rc4 |= ((rc >> 2) & 0x3) << 0;

                rc = read_spd(&dimm_config_table[didx], 0, DDR4_SPD_RDIMM_REGISTER_DRIVE_STRENGTH_CK);
                lmc_dimmx_params.s.rc5  = ((rc >> 0) & 0x3) << 2;
                lmc_dimmx_params.s.rc5 |= ((rc >> 2) & 0x3) << 0;

                lmc_dimmx_params.s.rc6  = 0;
                lmc_dimmx_params.s.rc7  = 0;
                lmc_dimmx_params.s.rc8  = 0;
                lmc_dimmx_params.s.rc9  = 0;

                /*
                ** rc10               DDR4 RDIMM Operating Speed
                ** ====   =========================================================
                **  0                 tclk_psecs >= 1250 psec DDR4-1600 (1250 ps)
                **  1     1250 psec > tclk_psecs >= 1071 psec DDR4-1866 (1071 ps)
                **  2     1071 psec > tclk_psecs >=  938 psec DDR4-2133 ( 938 ps)
                **  3      938 psec > tclk_psecs >=  833 psec DDR4-2400 ( 833 ps)
                **  4      833 psec > tclk_psecs >=  750 psec DDR4-2666 ( 750 ps)
                **  5      750 psec > tclk_psecs >=  625 psec DDR4-3200 ( 625 ps)
                */
                lmc_dimmx_params.s.rc10        = 0;
                if (1250 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 1;
                if (1071 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 2;
                if (938  > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 3;
                if (833  > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 4;
                if (750  > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 5;

                lmc_dimmx_params.s.rc11 = 0;
                lmc_dimmx_params.s.rc12 = 0;
                lmc_dimmx_params.s.rc13 = (spd_dimm_type == 4) ? 0 : 4; /* 0=LRDIMM, 1=RDIMM */
                lmc_dimmx_params.s.rc13 |= (ddr_type == DDR4_DRAM) ? (spd_addr_mirror << 3) : 0;
                lmc_dimmx_params.s.rc14 = 0;
                //lmc_dimmx_params.s.rc15 = 4; /* 0 nCK latency adder */
                lmc_dimmx_params.s.rc15 = 0; /* 1 nCK latency adder */

                lmc_dimmx_ddr4_params0.u64 = 0;

                lmc_dimmx_ddr4_params0.s.rc8x = 0;
                lmc_dimmx_ddr4_params0.s.rc7x = 0;
                lmc_dimmx_ddr4_params0.s.rc6x = 0;
                lmc_dimmx_ddr4_params0.s.rc5x = 0;
                lmc_dimmx_ddr4_params0.s.rc4x = 0;

                lmc_dimmx_ddr4_params0.s.rc3x = compute_rc3x(tclk_psecs);

                lmc_dimmx_ddr4_params0.s.rc2x = 0;
                lmc_dimmx_ddr4_params0.s.rc1x = 0;

                lmc_dimmx_ddr4_params1.u64 = 0;

                lmc_dimmx_ddr4_params1.s.rcbx = 0;
                lmc_dimmx_ddr4_params1.s.rcax = 0;
                lmc_dimmx_ddr4_params1.s.rc9x = 0;

                lmc_ddr4_dimm_ctl.u64 = 0;
                lmc_ddr4_dimm_ctl.cn70xx.ddr4_dimm0_wmask = 0x004;
                lmc_ddr4_dimm_ctl.cn70xx.ddr4_dimm1_wmask = (dimm_count > 1) ? 0x004 : 0x0000;

		/*
		 * Handle any overrides from envvars here...
		 */
		if ((s = lookup_env_parameter("ddr_ddr4_params0")) != NULL) {
		    lmc_dimmx_ddr4_params0.u64 = simple_strtoul(s, NULL, 0);
		}

		if ((s = lookup_env_parameter("ddr_ddr4_params1")) != NULL) {
		    lmc_dimmx_ddr4_params1.u64 = simple_strtoul(s, NULL, 0);
		}

		if ((s = lookup_env_parameter("ddr_ddr4_dimm_ctl")) != NULL) {
		    lmc_ddr4_dimm_ctl.u64 = simple_strtoul(s, NULL, 0);
		}

		for (i=0; i<11; ++i) {
		    uint64_t value;
		    if ((s = lookup_env_parameter("ddr_ddr4_rc%1xx", i+1)) != NULL) {
			value = simple_strtoul(s, NULL, 0);
			if (i < 8) {
			    lmc_dimmx_ddr4_params0.u64 &= ~((uint64_t)0xff << (i*8));
			    lmc_dimmx_ddr4_params0.u64 |=           (value << (i*8));
			} else {
			    lmc_dimmx_ddr4_params1.u64 &= ~((uint64_t)0xff << ((i-8)*8));
			    lmc_dimmx_ddr4_params1.u64 |=           (value << ((i-8)*8));
			}
		    }
		}

		/*
		 * write the final CSR values
		 */
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_DIMMX_DDR4_PARAMS0(dimmx, ddr_interface_num),
					  lmc_dimmx_ddr4_params0.u64);

                ddr_config_write_csr_node(node,
					  CVMX_LMCX_DDR4_DIMM_CTL(ddr_interface_num),
					  lmc_ddr4_dimm_ctl.u64);

                ddr_config_write_csr_node(node,
					  CVMX_LMCX_DIMMX_DDR4_PARAMS1(dimmx, ddr_interface_num),
					  lmc_dimmx_ddr4_params1.u64);

                ddr_print("DIMM%d Register Control Words        RCBx:RC1x : %x %x %x %x %x %x %x %x %x %x %x\n",
                          dimmx,
                          lmc_dimmx_ddr4_params1.s.rcbx,
                          lmc_dimmx_ddr4_params1.s.rcax,
                          lmc_dimmx_ddr4_params1.s.rc9x,
                          lmc_dimmx_ddr4_params0.s.rc8x,
                          lmc_dimmx_ddr4_params0.s.rc7x,
                          lmc_dimmx_ddr4_params0.s.rc6x,
                          lmc_dimmx_ddr4_params0.s.rc5x,
                          lmc_dimmx_ddr4_params0.s.rc4x,
                          lmc_dimmx_ddr4_params0.s.rc3x,
                          lmc_dimmx_ddr4_params0.s.rc2x,
                          lmc_dimmx_ddr4_params0.s.rc1x );

            } else {
                rc = read_spd(&dimm_config_table[didx], 0, 69);
                lmc_dimmx_params.s.rc0         = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc1         = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 70);
                lmc_dimmx_params.s.rc2         = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc3         = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 71);
                lmc_dimmx_params.s.rc4         = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc5         = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 72);
                lmc_dimmx_params.s.rc6         = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc7         = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 73);
                lmc_dimmx_params.s.rc8         = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc9         = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 74);
                lmc_dimmx_params.s.rc10        = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc11        = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 75);
                lmc_dimmx_params.s.rc12        = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc13        = (rc >> 4) & 0xf;

                rc = read_spd(&dimm_config_table[didx], 0, 76);
                lmc_dimmx_params.s.rc14        = (rc >> 0) & 0xf;
                lmc_dimmx_params.s.rc15        = (rc >> 4) & 0xf;


                if ((s = ddr_getenv_debug("ddr_clk_drive")) != NULL) {
                    if (strcmp(s,"light") == 0) {
                        lmc_dimmx_params.s.rc5         = 0x0; /* Light Drive */
                    }
                    if (strcmp(s,"moderate") == 0) {
                        lmc_dimmx_params.s.rc5         = 0x5; /* Moderate Drive */
                    }
                    if (strcmp(s,"strong") == 0) {
                        lmc_dimmx_params.s.rc5         = 0xA; /* Strong Drive */
                    }
                    error_print("Parameter found in environment. ddr_clk_drive = %s\n", s);
                }

                if ((s = ddr_getenv_debug("ddr_cmd_drive")) != NULL) {
                    if (strcmp(s,"light") == 0) {
                        lmc_dimmx_params.s.rc3         = 0x0; /* Light Drive */
                    }
                    if (strcmp(s,"moderate") == 0) {
                        lmc_dimmx_params.s.rc3         = 0x5; /* Moderate Drive */
                    }
                    if (strcmp(s,"strong") == 0) {
                        lmc_dimmx_params.s.rc3         = 0xA; /* Strong Drive */
                    }
                    error_print("Parameter found in environment. ddr_cmd_drive = %s\n", s);
                }

                if ((s = ddr_getenv_debug("ddr_ctl_drive")) != NULL) {
                    if (strcmp(s,"light") == 0) {
                        lmc_dimmx_params.s.rc4         = 0x0; /* Light Drive */
                    }
                    if (strcmp(s,"moderate") == 0) {
                        lmc_dimmx_params.s.rc4         = 0x5; /* Moderate Drive */
                    }
                    error_print("Parameter found in environment. ddr_ctl_drive = %s\n", s);
                }


                /*
                ** rc10               DDR3 RDIMM Operating Speed
                ** ====   =========================================================
                **  0                 tclk_psecs >= 2500 psec DDR3/DDR3L-800 (default)
                **  1     2500 psec > tclk_psecs >= 1875 psec DDR3/DDR3L-1066
                **  2     1875 psec > tclk_psecs >= 1500 psec DDR3/DDR3L-1333
                **  3     1500 psec > tclk_psecs >= 1250 psec DDR3/DDR3L-1600
                **  4     1250 psec > tclk_psecs >= 1071 psec DDR3-1866
                */
                lmc_dimmx_params.s.rc10        = 0;
                if (2500 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 1;
                if (1875 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 2;
                if (1500 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 3;
                if (1250 > tclk_psecs)
                    lmc_dimmx_params.s.rc10    = 4;
            }

            if ((s = lookup_env_parameter("ddr_dimmx_params", i)) != NULL) {
                lmc_dimmx_params.u64 = simple_strtoul(s, NULL, 0);
            }

            for (i=0; i<16; ++i) {
                uint64_t value;
                if ((s = lookup_env_parameter("ddr_rc%d", i)) != NULL) {
                    value = simple_strtoul(s, NULL, 0);
                    lmc_dimmx_params.u64 &= ~((uint64_t)0xf << (i*4));
                    lmc_dimmx_params.u64 |=           (  value << (i*4));
                }
            }

            ddr_config_write_csr_node(node,
				      CVMX_LMCX_DIMMX_PARAMS(dimmx, ddr_interface_num),
				      lmc_dimmx_params.u64);

            ddr_print("DIMM%d Register Control Words         RC15:RC0 : %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
                      dimmx,
                      lmc_dimmx_params.s.rc15,
                      lmc_dimmx_params.s.rc14,
                      lmc_dimmx_params.s.rc13,
                      lmc_dimmx_params.s.rc12,
                      lmc_dimmx_params.s.rc11,
                      lmc_dimmx_params.s.rc10,
                      lmc_dimmx_params.s.rc9 ,
                      lmc_dimmx_params.s.rc8 ,
                      lmc_dimmx_params.s.rc7 ,
                      lmc_dimmx_params.s.rc6 ,
                      lmc_dimmx_params.s.rc5 ,
                      lmc_dimmx_params.s.rc4 ,
                      lmc_dimmx_params.s.rc3 ,
                      lmc_dimmx_params.s.rc2 ,
                      lmc_dimmx_params.s.rc1 ,
                      lmc_dimmx_params.s.rc0 );
        }


        if (ddr_type == DDR4_DRAM) {
            /* LMC0_DIMM_CTL */
            lmc_dimm_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num));
            lmc_dimm_ctl.s.dimm0_wmask         = 0xdf3f;
            lmc_dimm_ctl.s.dimm1_wmask         = (dimm_count > 1) ? 0xdf3f : 0x0000;
            lmc_dimm_ctl.s.tcws                = 0x4e0;
            lmc_dimm_ctl.s.parity              = custom_lmc_config->parity;

            if ((s = lookup_env_parameter("ddr_dimm0_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm0_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm1_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm1_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_parity")) != NULL) {
                lmc_dimm_ctl.s.parity = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_tcws")) != NULL) {
                lmc_dimm_ctl.s.tcws = simple_strtoul(s, NULL, 0);
            }

            ddr_print("LMC DIMM_CTL                                  : 0x%016llx\n", lmc_dimm_ctl.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_DIMM_CTL(ddr_interface_num),
                                      lmc_dimm_ctl.u64);

            perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
                                          ddr_interface_num, 0x7 ); /* Init RCW */

            /* Write RC0D last */
            lmc_dimm_ctl.s.dimm0_wmask         = 0x2000;
            lmc_dimm_ctl.s.dimm1_wmask         = (dimm_count > 1) ? 0x2000 : 0x0000;
            ddr_print("LMC DIMM_CTL                                  : 0x%016llx\n", lmc_dimm_ctl.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_DIMM_CTL(ddr_interface_num),
                                      lmc_dimm_ctl.u64);

            /* Don't write any extended registers the second time */
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_DDR4_DIMM_CTL(ddr_interface_num),
                                      0);

            perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
                                          ddr_interface_num, 0x7 ); /* Init RCW */
        } else {

            /* LMC0_DIMM_CTL */
            lmc_dimm_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num));
            lmc_dimm_ctl.s.dimm0_wmask         = 0xffff;
            lmc_dimm_ctl.s.dimm1_wmask         = (dimm_count > 1) ? 0xffff : 0x0000;
            lmc_dimm_ctl.s.tcws                = 0x4e0;
            lmc_dimm_ctl.s.parity              = custom_lmc_config->parity;

            if ((s = lookup_env_parameter("ddr_dimm0_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm0_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm1_wmask")) != NULL) {
                lmc_dimm_ctl.s.dimm1_wmask    = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_parity")) != NULL) {
                lmc_dimm_ctl.s.parity = simple_strtoul(s, NULL, 0);
            }

            if ((s = lookup_env_parameter("ddr_dimm_ctl_tcws")) != NULL) {
                lmc_dimm_ctl.s.tcws = simple_strtoul(s, NULL, 0);
            }

            ddr_print("LMC DIMM_CTL                                  : 0x%016llx\n", lmc_dimm_ctl.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_DIMM_CTL(ddr_interface_num),
                                      lmc_dimm_ctl.u64);

            perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
                                          ddr_interface_num, 0x7 ); /* Init RCW */
        }

    } else {
        /* Disable register control writes for unbuffered */
        cvmx_lmcx_dimm_ctl_t lmc_dimm_ctl;
        lmc_dimm_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DIMM_CTL(ddr_interface_num));
        lmc_dimm_ctl.s.dimm0_wmask         = 0;
        lmc_dimm_ctl.s.dimm1_wmask         = 0;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_DIMM_CTL(ddr_interface_num),
				  lmc_dimm_ctl.u64);
    }

    /*
     * Comments (steps 3 through 5) continue in perform_octeon3_ddr3_sequence()
     */
    {
        cvmx_lmcx_modereg_params0_t lmc_modereg_params0;

        if (ddr_memory_preserved()) {
            /* Contents are being preserved. Take DRAM out of
               self-refresh first. Then init steps can procede
               normally */
            perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
                                          ddr_interface_num, 3); /* self-refresh exit */
        }

        lmc_modereg_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
        lmc_modereg_params0.s.dllr    = 1; /* Set during first init sequence */
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);

        perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        lmc_modereg_params0.s.dllr    = 0; /* Clear for normal operation */
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);
    }

    if ((spd_rdimm) && (ddr_type == DDR4_DRAM) && octeon_is_cpuid(OCTEON_CN7XXX)) {
        ddr_print("Running init sequence 1\n");
        change_rdimm_mpr_pattern (cpu_id, node, rank_mask, ddr_interface_num, dimm_count);
    }

    memset(lanes, 0, sizeof(lanes));

    if (disable_deskew_training) { // FIXME: disable internal VREF if deskew is disabled?
        ddr_print("N%d.LMC%d: internal VREF Training disabled, leaving them in RESET.\n",
                  node, ddr_interface_num);
        num_samples = 0;
    } else if ((ddr_type == DDR4_DRAM) && !octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
        num_samples = DEFAULT_DAC_SAMPLES;
    } else {
        num_samples = 1; // if DDR3 or no ability to write DAC values
    }

 perform_internal_vref_training:

    for (sample = 0; sample < num_samples; sample++) { 

        dac_eval_retries = 0;

        do { // make offset and internal vref training repeatable

            /* 6.9.8 LMC Offset Training
               LMC requires input-receiver offset training. */
            Perform_Offset_Training(node, rank_mask, ddr_interface_num, cpu_id);

            /* 6.9.9 LMC Internal Vref Training
               LMC requires input-reference-voltage training. */
            Perform_Internal_VREF_Training(node, rank_mask, ddr_interface_num, cpu_id);

            // read and maybe display the DAC values for a sample
            read_DAC_DBI_settings(cpu_id, node, ddr_interface_num, /*DAC*/1, dac_settings);
            if ((num_samples == 1) /*|| ddr_verbose()*/) { // FIXME? verbosity
                display_DAC_DBI_settings(node, ddr_interface_num, /*DAC*/1, use_ecc,
                                         dac_settings, "Internal VREF");
            }

            // for DDR4, evaluate the DAC settings and retry if any issues
            if (ddr_type == DDR4_DRAM) {
                if (evaluate_DAC_settings(ddr_interface_64b, use_ecc, dac_settings)) {
                    if (++dac_eval_retries > DAC_RETRIES_LIMIT) {
                        ddr_print("N%d.LMC%d: DDR4 internal VREF DAC settings: retries exhausted; continuing...\n",
                                  node, ddr_interface_num); // FIXME? verbosity!!!
                    } else {
                        debug_print("N%d.LMC%d: DDR4 internal VREF DAC settings inconsistent; retrying....\n",
                                  node, ddr_interface_num);
                        continue; // try another sample
                    }
                }
                if (num_samples > 1) { // taking multiple samples, otherwise do nothing
                    // good sample or exhausted retries, record it
                    for (lane = 0; lane < last_lane; lane++) {
                        lanes[lane].bytes[sample] = dac_settings[lane];
                    }
                }
            }
            break; // done if DDR3, or good sample, or exhausted retries

        } while (1);

    } /* for (sample = 0; sample < num_samples; sample++) */

    if (num_samples > 1) {
        debug_print("N%d.LMC%d: DDR4 internal VREF DAC settings: processing multiple samples...\n",
                    node, ddr_interface_num);

        for (lane = 0; lane < last_lane; lane++) {
            dac_settings[lane] = process_samples_average(&lanes[lane].bytes[0], num_samples,
                                                         ddr_interface_num, lane);
        }
        display_DAC_DBI_settings(node, ddr_interface_num, /*DAC*/1, use_ecc, dac_settings, "Averaged VREF");

        // finally, write the final DAC values
        for (lane = 0; lane < last_lane; lane++) {
            load_dac_override(cpu_id, node, ddr_interface_num, dac_settings[lane], lane);
        }
    }

#if DDR3_DAC_OVERRIDE
    // as a second step, after internal VREF training, before starting deskew training:
    // for DDR3 and OCTEON3 not O78 pass 1.x, override the DAC setting to 127
    if ((ddr_type == DDR3_DRAM) && !octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && !disable_deskew_training) { // FIXME?
        load_dac_override(cpu_id, node, ddr_interface_num, 127, /* all */0x0A);
        ddr_print("N%d.LMC%d: Overriding DDR3 internal VREF DAC settings to 127.\n",
                  node, ddr_interface_num);
    }
#endif

    /*
     * 4.8.8 LMC Deskew Training
     *
     * LMC requires input-read-data deskew training.
     */
    if (! disable_deskew_training) {

        deskew_training_errors = Perform_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, spd_rawcard_AorB);

        // All the Deskew lock and saturation retries (may) have been done,
        //  but we ended up with  nibble errors; so, as a last ditch effort,
        //  try the Internal Vref Training again...
        if (deskew_training_errors) {
            if (internal_retries < DEFAULT_INTERNAL_VREF_TRAINING_LIMIT) {
                internal_retries++;
                ddr_print("N%d.LMC%d: Deskew training results still unsettled - retrying internal Vref training (%d)\n",
                          node, ddr_interface_num, internal_retries);
                goto perform_internal_vref_training;
            } else {
                if (restart_if_dsk_incomplete) {
                    ddr_print("N%d.LMC%d: INFO: Deskew training incomplete - %d retries exhausted, Restarting LMC init...\n",
                          node, ddr_interface_num, internal_retries);
                    return 0; // 0 indicates restart possible...
                }
                ddr_print("N%d.LMC%d: Deskew training incomplete - %d retries exhausted, but continuing...\n",
                          node, ddr_interface_num, internal_retries);
            }
        } /* if (deskew_training_errors) */

        // FIXME: treat this as the final DSK print from now on, and print if VBL_NORM or above
        // also, save the results of the original training in case we want them later
        Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, &deskew_training_results, 1);

#if !ENABLE_WRITE_DESKEW_VERIFY
        // do write bit-deskew if enabled...
        if (enable_write_deskew) {
            ddr_print("N%d.LMC%d: WRITE BIT-DESKEW feature enabled.\n", node, ddr_interface_num);
            Write_Deskew_Config(cpu_id, node, rank_mask, ddr_interface_num);
        } /* if (enable_write_deskew) */
#endif /* !ENABLE_WRITE_DESKEW_VERIFY */

    } else { /* if (! disable_deskew_training) */
        ddr_print("N%d.LMC%d: Deskew Training disabled, printing settings before HWL.\n",
                  node, ddr_interface_num);
        Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num,
                                 &deskew_training_results, 1);
    } /* if (! disable_deskew_training) */

#if 0
    /*
     * 4.8.8 LMC Deskew Training
     *
     * LMC requires input-read-data deskew training.
     *
     * 1. Write LMC(0)_EXT_CONFIG[VREFINT_SEQ_DESKEW] = 1.
     */
    {
        cvmx_lmcx_ext_config_t ext_config;
        ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
        ext_config.s.vrefint_seq_deskew = 1;

        ddr_seq_print("Performing LMC sequence: vrefint_seq_deskew = %d\n",
		      ext_config.s.vrefint_seq_deskew);

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
				  ext_config.u64);
    }

    /*
     * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0A and
     *    LMC(0)_SEQ_CTL[INIT_START] = 1.
     *
     * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
     */

    perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0A); /* LMC Deskew Training */
#endif

    /* LMC(0)_EXT_CONFIG */
    {
        cvmx_lmcx_ext_config_t ext_config;
        ext_config.u64  = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
        ext_config.s.vrefint_seq_deskew = 0;
        ext_config.s.read_ena_bprch = 1;
        ext_config.s.read_ena_fprch = 1;
        ext_config.s.drive_ena_fprch = 1;
        ext_config.s.drive_ena_bprch = 1;
        ext_config.s.invert_data = 0; // make sure this is OFF for all current chips

        if ((s = lookup_env_parameter("ddr_read_fprch")) != NULL) {
            ext_config.s.read_ena_fprch = strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_read_bprch")) != NULL) {
            ext_config.s.read_ena_bprch = strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_drive_fprch")) != NULL) {
            ext_config.s.drive_ena_fprch = strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_drive_bprch")) != NULL) {
            ext_config.s.drive_ena_bprch = strtoul(s, NULL, 0);
        }

        if (!octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) && (lranks_per_prank > 1)) {
            ext_config.s.dimm0_cid = ext_config.s.dimm1_cid = lranks_bits;
            ddr_print("N%d.LMC%d: 3DS: setting EXT_CONFIG[dimmx_cid] = %d\n",
                      node, ddr_interface_num, ext_config.s.dimm0_cid);
        }

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
				  ext_config.u64);
        ddr_print("%-45s : 0x%016llx\n", "EXT_CONFIG", ext_config.u64);
    }


    {
        cvmx_lmcx_config_t lmc_config;
        int save_ref_zqcs_int;
        uint64_t temp_delay_usecs;

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));

        /* Temporarily select the minimum ZQCS interval and wait
           long enough for a few ZQCS calibrations to occur.  This
           should ensure that the calibration circuitry is
           stabilized before read/write leveling occurs. */
        if (octeon_is_cpuid(OCTEON_CN7XXX)) {
            save_ref_zqcs_int         = lmc_config.cn78xx.ref_zqcs_int;
            lmc_config.cn78xx.ref_zqcs_int = 1 | (32<<7); /* set smallest interval */
        } else {
            save_ref_zqcs_int         = lmc_config.cn63xx.ref_zqcs_int;
            lmc_config.cn63xx.ref_zqcs_int = 1 | (32<<7); /* set smallest interval */
        }
        ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num),
                                  lmc_config.u64);
        cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));

        /* Compute an appropriate delay based on the current ZQCS
           interval. The delay should be long enough for the
           current ZQCS delay counter to expire plus ten of the
           minimum intarvals to ensure that some calibrations
           occur. */
        temp_delay_usecs = (((uint64_t)save_ref_zqcs_int >> 7)
                            * tclk_psecs * 100 * 512 * 128) / (10000*10000)
            + 10 * ((uint64_t)32 * tclk_psecs * 100 * 512 * 128) / (10000*10000);

        ddr_print ("Waiting %d usecs for ZQCS calibrations to start\n",
                   temp_delay_usecs);
        cvmx_wait_usec(temp_delay_usecs);

        if (octeon_is_cpuid(OCTEON_CN7XXX))
            lmc_config.cn78xx.ref_zqcs_int = save_ref_zqcs_int; /* Restore computed interval */
        else
            lmc_config.cn63xx.ref_zqcs_int = save_ref_zqcs_int; /* Restore computed interval */
        ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
        cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
    }


    /*
     * 4.8.9 LMC Write Leveling
     *
     * LMC supports an automatic write leveling like that described in the
     * JEDEC DDR3 specifications separately per byte-lane.
     *
     * All of DDR PLL, LMC CK, LMC DRESET, and early LMC initializations must
     * be completed prior to starting this LMC write-leveling sequence.
     *
     * There are many possible procedures that will write-level all the
     * attached DDR3 DRAM parts. One possibility is for software to simply
     * write the desired values into LMC(0)_WLEVEL_RANK(0..3). This section
     * describes one possible sequence that uses LMC's autowrite-leveling
     * capabilities.
     *
     * 1. If the DQS/DQ delays on the board may be more than the ADD/CMD
     *    delays, then ensure that LMC(0)_CONFIG[EARLY_DQX] is set at this
     *    point.
     *
     * Do the remaining steps 2-7 separately for each rank i with attached
     * DRAM.
     *
     * 2. Write LMC(0)_WLEVEL_RANKi = 0.
     *
     * 3. For x8 parts:
     *
     *    Without changing any other fields in LMC(0)_WLEVEL_CTL, write
     *    LMC(0)_WLEVEL_CTL[LANEMASK] to select all byte lanes with attached
     *    DRAM.
     *
     *    For x16 parts:
     *
     *    Without changing any other fields in LMC(0)_WLEVEL_CTL, write
     *    LMC(0)_WLEVEL_CTL[LANEMASK] to select all even byte lanes with
     *    attached DRAM.
     *
     * 4. Without changing any other fields in LMC(0)_CONFIG,
     *
     *    o write LMC(0)_SEQ_CTL[SEQ_SEL] to select write-leveling
     *
     *    o write LMC(0)_CONFIG[RANKMASK] = (1 << i)
     *
     *    o write LMC(0)_SEQ_CTL[INIT_START] = 1
     *
     *    LMC will initiate write-leveling at this point. Assuming
     *    LMC(0)_WLEVEL_CTL [SSET] = 0, LMC first enables write-leveling on
     *    the selected DRAM rank via a DDR3 MR1 write, then sequences through
     *    and accumulates write-leveling results for eight different delay
     *    settings twice, starting at a delay of zero in this case since
     *    LMC(0)_WLEVEL_RANKi[BYTE*<4:3>] = 0, increasing by 1/8 CK each
     *    setting, covering a total distance of one CK, then disables the
     *    write-leveling via another DDR3 MR1 write.
     *
     *    After the sequence through 16 delay settings is complete:
     *
     *    o LMC sets LMC(0)_WLEVEL_RANKi[STATUS] = 3
     *
     *    o LMC sets LMC(0)_WLEVEL_RANKi[BYTE*<2:0>] (for all ranks selected
     *      by LMC(0)_WLEVEL_CTL[LANEMASK]) to indicate the first write
     *      leveling result of 1 that followed result of 0 during the
     *      sequence, except that the LMC always writes
     *      LMC(0)_WLEVEL_RANKi[BYTE*<0>]=0.
     *
     *    o Software can read the eight write-leveling results from the first
     *      pass through the delay settings by reading
     *      LMC(0)_WLEVEL_DBG[BITMASK] (after writing
     *      LMC(0)_WLEVEL_DBG[BYTE]). (LMC does not retain the writeleveling
     *      results from the second pass through the eight delay
     *      settings. They should often be identical to the
     *      LMC(0)_WLEVEL_DBG[BITMASK] results, though.)
     *
     * 5. Wait until LMC(0)_WLEVEL_RANKi[STATUS] != 2.
     *
     *    LMC will have updated LMC(0)_WLEVEL_RANKi[BYTE*<2:0>] for all byte
     *    lanes selected by LMC(0)_WLEVEL_CTL[LANEMASK] at this point.
     *    LMC(0)_WLEVEL_RANKi[BYTE*<4:3>] will still be the value that
     *    software wrote in substep 2 above, which is 0.
     *
     * 6. For x16 parts:
     *
     *    Without changing any other fields in LMC(0)_WLEVEL_CTL, write
     *    LMC(0)_WLEVEL_CTL[LANEMASK] to select all odd byte lanes with
     *    attached DRAM.
     *
     *    Repeat substeps 4 and 5 with this new LMC(0)_WLEVEL_CTL[LANEMASK]
     *    setting. Skip to substep 7 if this has already been done.
     *
     *    For x8 parts:
     *
     *    Skip this substep. Go to substep 7.
     *
     * 7. Calculate LMC(0)_WLEVEL_RANKi[BYTE*<4:3>] settings for all byte
     *    lanes on all ranks with attached DRAM.
     *
     *    At this point, all byte lanes on rank i with attached DRAM should
     *    have been write-leveled, and LMC(0)_WLEVEL_RANKi[BYTE*<2:0>] has
     *    the result for each byte lane.
     *
     *    But note that the DDR3 write-leveling sequence will only determine
     *    the delay modulo the CK cycle time, and cannot determine how many
     *    additional CK cycles of delay are present. Software must calculate
     *    the number of CK cycles, or equivalently, the
     *    LMC(0)_WLEVEL_RANKi[BYTE*<4:3>] settings.
     *
     *    This BYTE*<4:3> calculation is system/board specific.
     *
     * Many techniques can be used to calculate write-leveling BYTE*<4:3> values,
     * including:
     *
     *    o Known values for some byte lanes.
     *
     *    o Relative values for some byte lanes relative to others.
     *
     *    For example, suppose lane X is likely to require a larger
     *    write-leveling delay than lane Y. A BYTEX<2:0> value that is much
     *    smaller than the BYTEY<2:0> value may then indicate that the
     *    required lane X delay wrapped into the next CK, so BYTEX<4:3>
     *    should be set to BYTEY<4:3>+1.
     *
     *    When ECC DRAM is not present (i.e. when DRAM is not attached to the
     *    DDR_CBS_0_* and DDR_CB<7:0> chip signals, or the DDR_DQS_<4>_* and
     *    DDR_DQ<35:32> chip signals), write LMC(0)_WLEVEL_RANK*[BYTE8] =
     *    LMC(0)_WLEVEL_RANK*[BYTE0], using the final calculated BYTE0 value.
     *    Write LMC(0)_WLEVEL_RANK*[BYTE4] = LMC(0)_WLEVEL_RANK*[BYTE0],
     *    using the final calculated BYTE0 value.
     *
     * 8. Initialize LMC(0)_WLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    LMC(0)_WLEVEL_RANKj = LMC(0)_WLEVEL_RANKi.
     */
    {
#pragma pack(push,1)
        cvmx_lmcx_wlevel_ctl_t wlevel_ctl;
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        cvmx_lmcx_modereg_params1_t modereg_params1;
        int rankx = 0;
        int wlevel_bitmask[9];
        int byte_idx;
        int ecc_ena;
        int ddr_wlevel_roundup = 0;
        int save_mode32b;
        int ddr_wlevel_printall = 0; // default to not print all samples bitmaps/delays
        int disable_hwl_validity = 0;
        int default_wlevel_rtt_nom;
#pragma pack(pop)

        if (wlevel_loops)
            ddr_print("N%d.LMC%d: Performing Hardware Write-Leveling\n", node, ddr_interface_num);
        else {
            wlevel_bitmask_errors = 1; /* Force software write-leveling to run */
            ddr_print("N%d.LMC%d: Forcing software Write-Leveling\n", node, ddr_interface_num);
	}

        default_wlevel_rtt_nom = (ddr_type == DDR3_DRAM) ? rttnom_20ohm : ddr4_rttnom_40ohm;

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
	ecc_ena = lmc_config.s.ecc_ena;
        save_mode32b = lmc_config.cn78xx.mode32b;
        lmc_config.cn78xx.mode32b         = (! ddr_interface_64b);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);

        if ((s = lookup_env_parameter("ddr_wlevel_roundup")) != NULL) {
            ddr_wlevel_roundup = simple_strtoul(s, NULL, 0);
        }

	if ((s = lookup_env_parameter("ddr_wlevel_printall")) != NULL) {
	    ddr_wlevel_printall = strtoul(s, NULL, 0);
	}

        // default to disable when RL sequential delay check is disabled
        disable_hwl_validity = custom_lmc_config->disable_sequential_delay_check;

	if ((s = lookup_env_parameter("ddr_disable_hwl_validity")) != NULL) {
	    disable_hwl_validity = !!strtoul(s, NULL, 0);
	}

        if ((s = lookup_env_parameter("ddr_wlevel_rtt_nom")) != NULL) {
            default_wlevel_rtt_nom = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_match_wlevel_rtt_nom")) != NULL) {
            match_wlevel_rtt_nom = !!simple_strtoul(s, NULL, 0);
        }
        if (match_wlevel_rtt_nom) {
            modereg_params1.u64 =
                cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));
        }

        // For DDR3, we do not touch WLEVEL_CTL fields OR_DIS or BITMASK
        if (ddr_type == DDR4_DRAM) {
            wlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_CTL(ddr_interface_num));
            wlevel_ctl.s.or_dis  = 1;    // now this is the default for DDR4
            wlevel_ctl.s.bitmask = 0xFF; // now this is the default for DDR4

            // when x4, use only the lower nibble
            if (dram_width == 4) {
                wlevel_ctl.s.bitmask = 0x0F;
            }

            // allow overrides
            if ((s = lookup_env_parameter("ddr_wlevel_ctl_or_dis")) != NULL) {
                wlevel_ctl.s.or_dis = !!strtoul(s, NULL, 0);
            }
            if ((s = lookup_env_parameter("ddr_wlevel_ctl_bitmask")) != NULL) {
                wlevel_ctl.s.bitmask = simple_strtoul(s, NULL, 0);
            }

            // print and write only if not defaults
            if ((wlevel_ctl.s.or_dis != 1) || (wlevel_ctl.s.bitmask != 0xFF)) {
                ddr_print("N%d.LMC%d: WLEVEL_CTL: or_dis=%d, bitmask=0x%02x\n",
                          node, ddr_interface_num,
                          wlevel_ctl.s.or_dis, wlevel_ctl.s.bitmask);
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
                                          wlevel_ctl.u64);
            }
        }

    start_hardware_write_leveling:

        // Start the hardware write-leveling loop per rank
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
	    int wloop = 0;
	    int wloop_retries = 0; // retries per sample for HW-related issues with bitmasks or values
            int wloop_retries_total = 0;
            int wloop_retries_exhausted = 0;
#define WLOOP_RETRIES_DEFAULT 5
            int wlevel_validity_errors;
            int wlevel_bitmask_errors_rank = 0;
            int wlevel_validity_errors_rank = 0;
#if HW_WL_MAJORITY
	    // array to collect counts of byte-lane values
	    // assume low-order 3 bits and even, so really only 2 bit values
	    int wlevel_bytes[9][4];
#endif            
            int rank_nom = 0;

            if (!(rank_mask & (1 << rankx)))
                continue;

            if (match_wlevel_rtt_nom) { // FIXME
                if (rankx == 0)
                    rank_nom = modereg_params1.s.rtt_nom_00;
                if (rankx == 1)
                    rank_nom = modereg_params1.s.rtt_nom_01;
                if (rankx == 2)
                    rank_nom = modereg_params1.s.rtt_nom_10;
                if (rankx == 3)
                    rank_nom = modereg_params1.s.rtt_nom_11;
                ddr_print("N%d.LMC%d.R%d: Setting WLEVEL_CTL[rtt_nom] to %d (%d)\n",
                          node, ddr_interface_num, rankx, rank_nom,
                          imp_values->rtt_nom_ohms[rank_nom]);
            }

#if HW_WL_MAJORITY
	    memset(wlevel_bytes, 0, sizeof(wlevel_bytes));
#endif

	    // restructure the looping so we can keep trying until we get the samples we want
	    //for (int wloop = 0; wloop < wlevel_loops; wloop++) {

	    while (wloop < wlevel_loops) {

                wlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_CTL(ddr_interface_num));

		wlevel_ctl.cn78xx.rtt_nom = (default_wlevel_rtt_nom > 0) ? (default_wlevel_rtt_nom - 1) : 7;

                if (match_wlevel_rtt_nom) {
                    wlevel_ctl.cn78xx.rtt_nom = (rank_nom > 0) ? (rank_nom - 1) : 7;
                }

                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                          0); /* Clear write-level delays */

		wlevel_bitmask_errors = 0; /* Reset error counters */
                wlevel_validity_errors = 0;

                for (byte_idx=0; byte_idx<9; ++byte_idx) {
                    wlevel_bitmask[byte_idx] = 0; /* Reset bitmasks */
                }

#if 0 // FIXME???
                /* Make separate passes for each byte to reduce power. */
                for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {

                    if (!(ddr_interface_bytemask&(1<<byte_idx)))
                        continue;

                    wlevel_ctl.cn78xx.lanemask = (1<<byte_idx);

                    ddr_config_write_csr_node(node,
                                              CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
                                              wlevel_ctl.u64);

                    /* Read and write values back in order to update the
                       status field. This insures that we read the updated
                       values after write-leveling has completed. */
                    ddr_config_write_csr_node(node,
                                              CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                              cvmx_read_csr_node(node,
                                                                 CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num)));

                    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx,
                                                  ddr_interface_num, 6); /* write-leveling */

                    do {
                        lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));
                    } while (lmc_wlevel_rank.cn78xx.status != 3);

                    wlevel_bitmask[byte_idx] = octeon_read_lmcx_ddr3_wlevel_dbg(node, ddr_interface_num, byte_idx);
                    if (wlevel_bitmask[byte_idx] == 0)
                        ++wlevel_bitmask_errors;
                } /* for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) */

                /* Restore lanemask default to avoid potential side-effects during read-leveling */
                wlevel_ctl.cn78xx.lanemask = 0x1ff;
                ddr_config_write_csr_node(node,
                                          CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
                                          wlevel_ctl.u64);
#else
                // do all the byte-lanes at the same time
                wlevel_ctl.cn78xx.lanemask = 0x1ff;

                ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_CTL(ddr_interface_num), wlevel_ctl.u64);

                /* Read and write values back in order to update the
                   status field. This insures that we read the updated
                   values after write-leveling has completed. */
                ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                          cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num)));

                perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx,
                                              ddr_interface_num, 6); /* write-leveling */

                do {
                    lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));
                } while (lmc_wlevel_rank.cn78xx.status != 3);

                lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                    wlevel_bitmask[byte_idx] = octeon_read_lmcx_ddr3_wlevel_dbg(node, ddr_interface_num, byte_idx);
                    if (wlevel_bitmask[byte_idx] == 0)
                        ++wlevel_bitmask_errors;
                } /* for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) */
#endif

                // check validity only if no bitmask errors
                if (wlevel_bitmask_errors == 0) {
                    if (((spd_dimm_type == 1) || (spd_dimm_type == 2)) && // normal [RU]DIMM
                        (dram_width != 16)    &&
                        (ddr_interface_64b)   &&
                        !(disable_hwl_validity))
                    { // bypass if [mini|SO]-[RU]DIMM or x16 or 32-bit
                        wlevel_validity_errors =
                            Validate_HW_WL_Settings(node, ddr_interface_num,
                                                    &lmc_wlevel_rank, spd_rdimm, ecc_ena);
                        wlevel_validity_errors_rank += (wlevel_validity_errors != 0);
                    }
                } else
                    wlevel_bitmask_errors_rank++;

		// before we print, if we had bitmask or validity errors, do a retry...
		if ((wlevel_bitmask_errors != 0) || (wlevel_validity_errors != 0)) {
		    if (wloop_retries < WLOOP_RETRIES_DEFAULT) {
			wloop_retries++;
                        wloop_retries_total++;
			// this printout is per-retry: only when VBL is high enough (DEV?)
                        // FIXME: do we want to show the bad bitmaps or delays here also?
			debug_print("N%d.LMC%d.R%d: H/W Write-Leveling had %s errors - retrying...\n",
                                    node, ddr_interface_num, rankx,
                                    (wlevel_bitmask_errors) ? "Bitmask" : "Validity");
			continue; // this takes us back to the top without counting a sample
		    } else { // ran out of retries for this sample
                        // retries exhausted, do not print at normal VBL
			debug_print("N%d.LMC%d.R%d: H/W Write-Leveling issues: %s errors\n",
                                    node, ddr_interface_num, rankx,
                                    (wlevel_bitmask_errors) ? "Bitmask" : "Validity");
                        wloop_retries_exhausted++;
		    }
		}
                // no errors or exhausted retries, use this sample
                wloop_retries = 0; //reset for next sample

		// when only 1 sample or forced, print the bitmasks then current HW WL
		if ((wlevel_loops == 1) || ddr_wlevel_printall) {
                    display_WL_BM(node, ddr_interface_num, rankx, wlevel_bitmask);
                    display_WL(node, ddr_interface_num, lmc_wlevel_rank, rankx);
                }

                if (ddr_wlevel_roundup) { /* Round up odd bitmask delays */
                    for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                        if (!(ddr_interface_bytemask&(1<<byte_idx)))
                            continue;
                        update_wlevel_rank_struct(&lmc_wlevel_rank,
                                                  byte_idx,
                                                  roundup_ddr3_wlevel_bitmask(wlevel_bitmask[byte_idx]));
                    } /* for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) */
                    ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                              lmc_wlevel_rank.u64);
                    display_WL(node, ddr_interface_num, lmc_wlevel_rank, rankx);
                }

#if HW_WL_MAJORITY
		// OK, we have a decent sample, no bitmask or validity errors
		for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                    int ix;
                    if (!(ddr_interface_bytemask&(1<<byte_idx)))
                        continue;
		    // increment count of byte-lane value
		    ix = (get_wlevel_rank_struct(&lmc_wlevel_rank, byte_idx) >> 1) & 3; // only 4 values
		    wlevel_bytes[byte_idx][ix]++;
		} /* for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) */
#endif

		wloop++; // if we get here, we have taken a decent sample

            } /* while (wloop < wlevel_loops) */

#if HW_WL_MAJORITY
	    // if we did sample more than once, try to pick a majority vote
	    if (wlevel_loops > 1) {
		// look for the majority in each byte-lane
		for (byte_idx = 0; byte_idx < (8+ecc_ena); ++byte_idx) {
		    int mx = -1, mc = 0, xc = 0, cc = 0; 
		    int ix, ic, alts;
		    for (ix = 0; ix < 4; ix++) {
			ic = wlevel_bytes[byte_idx][ix];
			// make a bitmask of the ones with a count
			if (ic > 0) {
			    mc |= (1 << ix);
			    cc++; // count how many had non-zero counts
			}
			// find the majority
			if (ic > xc) { // new max?
			    xc = ic; // yes
			    mx = ix; // set its index
			}
		    }
#if SWL_TRY_HWL_ALT
		    // see if there was an alternate
		    alts = (mc & ~(1 << mx)); // take out the majority choice
		    if (alts != 0) {
			for (ix = 0; ix < 4; ix++) {
			    if (alts & (1 << ix)) { // FIXME: could be done multiple times? bad if so
				hwl_alts[rankx].hwl_alt_mask |= (1 << byte_idx); // set the mask
				hwl_alts[rankx].hwl_alt_delay[byte_idx] = ix << 1; // record the value
                                if (ddr_wlevel_printall)
                                    ddr_print("N%d.LMC%d.R%d: SWL_TRY_HWL_ALT: Byte %d maj %d (%d) alt %d (%d).\n",
                                              node, ddr_interface_num, rankx, byte_idx, mx << 1, xc,
                                              ix << 1, wlevel_bytes[byte_idx][ix]);
			    }
			}
		    } else {
			debug_print("N%d.LMC%d.R%d: SWL_TRY_HWL_ALT: Byte %d maj %d alt NONE.\n",
				    node, ddr_interface_num, rankx, byte_idx, mx << 1);
		    }
#endif /* SWL_TRY_HWL_ALT */
		    if (cc > 2) { // unlikely, but...
			// assume: counts for 3 indices are all 1
			// possiblities are: 0/2/4, 2/4/6, 0/4/6, 0/2/6
			// and the desired?:   2  ,   4  ,     6, 0
			// we choose the middle, assuming one of the outliers is bad
			// NOTE: this is an ugly hack at the moment; there must be a better way
			switch (mc) {
			case 0x7: mx = 1; break; // was 0/2/4, choose 2
			case 0xb: mx = 0; break; // was 0/2/6, choose 0
			case 0xd: mx = 3; break; // was 0/4/6, choose 6
			case 0xe: mx = 2; break; // was 2/4/6, choose 4
			default:
			case 0xf: mx = 1; break; // was 0/2/4/6, choose 2?
			}
			error_print("N%d.LMC%d.R%d: HW WL MAJORITY: bad byte-lane %d (0x%x), using %d.\n",
				  node, ddr_interface_num, rankx, byte_idx, mc, mx << 1);
		    }
		    update_wlevel_rank_struct(&lmc_wlevel_rank, byte_idx, mx << 1);
		} /* for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) */

                ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                          lmc_wlevel_rank.u64);
		display_WL_with_final(node, ddr_interface_num, lmc_wlevel_rank, rankx);

	    } /* if (wlevel_loops > 1) */
#endif /* HW_WL_MAJORITY */
            // maybe print an error summary for the rank
            if ((wlevel_bitmask_errors_rank != 0) || (wlevel_validity_errors_rank != 0)) {
                ddr_print("N%d.LMC%d.R%d: H/W Write-Leveling errors - %d bitmask, %d validity, %d retries, %d exhausted\n",
                          node, ddr_interface_num, rankx,
                          wlevel_bitmask_errors_rank, wlevel_validity_errors_rank,
                          wloop_retries_total, wloop_retries_exhausted);
            }

        } /* for (rankx = 0; rankx < dimm_count * 4;rankx++) */

#if ENABLE_WRITE_DESKEW_VERIFY
        // we want to verify, so do this after 1st HWL and then do another
        if (! disable_deskew_training) {
            // do write bit-deskew if enabled...
            if (enable_write_deskew && !write_deskew_done) {
                ddr_print("N%d.LMC%d: WRITE BIT-DESKEW feature enabled.\n",node, ddr_interface_num);
                Write_Deskew_Config(cpu_id, node, rank_mask, ddr_interface_num);
                write_deskew_done = 1;
                goto start_hardware_write_leveling;
            } /* if (enable_write_deskew) */
        } /* if (! disable_deskew_training) */
#endif /* ENABLE_WRITE_DESKEW_VERIFY */

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        lmc_config.cn78xx.mode32b         = save_mode32b;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);
    }

    // At the end of HW Write Leveling, check on some DESKEW things...
    if (! disable_deskew_training) {
        deskew_counts_t dsk_counts;
	int retry_count = 0;

	ddr_print("N%d.LMC%d: Check Deskew Settings before Read-Leveling.\n", node, ddr_interface_num);

	do {
	    Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, &dsk_counts, 1);

	    // only RAWCARD A or B will not benefit from retraining if there's only saturation
            // or any rawcard if there is a nibble error
	    if ((!spd_rawcard_AorB && dsk_counts.saturated > 0) ||
		((dsk_counts.nibrng_errs != 0) || (dsk_counts.nibunl_errs != 0)))
	    {
		retry_count++;
		ddr_print("N%d.LMC%d: Deskew Status indicates saturation or nibble errors - retry %d Training.\n",
			  node, ddr_interface_num, retry_count);
		Perform_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, spd_rawcard_AorB);
	    } else
		break;
	} while (retry_count < 5);
    } /* if (! disable_deskew_training) */

#if 0
    /* Hard-code read-leveling values and dump MPR reads */
    if (ddr_type == DDR4_DRAM) {
        int rankx;
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t mpr_data[3];
            if (!(rank_mask & (1 << rankx)))
                continue;

            {                   /* Hard-code read-leveling values */
                cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;

                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
#if 0
                /* When forcing RDIMM_ENA = 0 */
                lmc_rlevel_rank.cn78xx.byte8 = 13;
                lmc_rlevel_rank.cn78xx.byte7 = 17;
                lmc_rlevel_rank.cn78xx.byte6 = 16;
                lmc_rlevel_rank.cn78xx.byte5 = 16;
                lmc_rlevel_rank.cn78xx.byte4 = 16;
                lmc_rlevel_rank.cn78xx.byte3 = 17;
                lmc_rlevel_rank.cn78xx.byte2 = 15;
                lmc_rlevel_rank.cn78xx.byte1 = 16;
                lmc_rlevel_rank.cn78xx.byte0 = 16;
#endif
#if 1
                /* Hynix HMA41GR7MFR8N-UH TD AB SKHynix H5AN4G8NMFR IDT4RCD0124KC0 8 2133 2Rx8 Reg ECC */
                /* Rank(0) Rlevel Rank   0x1, 0x004B34C30B30C34E :    11    13    12    12    11    12    12    13    14 (0) */
                /* Rank(1) Rlevel Rank   0x1, 0x004B34C30B30C34E :    11    13    12    12    11    12    12    13    14 (0) */
                lmc_rlevel_rank.cn78xx.byte8 = 11;
                lmc_rlevel_rank.cn78xx.byte7 = 13;
                lmc_rlevel_rank.cn78xx.byte6 = 12;
                lmc_rlevel_rank.cn78xx.byte5 = 12;
                lmc_rlevel_rank.cn78xx.byte4 = 11;
                lmc_rlevel_rank.cn78xx.byte3 = 12;
                lmc_rlevel_rank.cn78xx.byte2 = 12;
                lmc_rlevel_rank.cn78xx.byte1 = 13;
                lmc_rlevel_rank.cn78xx.byte0 = 14;
#endif
#if 0
                /* Transcend TS512MLH64V1H Micron D9RGQ - MT40A512M8HX-093E:A IDT4RCD0124KC0 4 2133 1Rx8 Reg ECC */
                /* Rank(0) Rlevel Rank   0x1, 0x004C34D30C30D34E :    12    13    13    12    12    12    13    13    14 (0) */
                lmc_rlevel_rank.cn78xx.byte8 = 12;
                lmc_rlevel_rank.cn78xx.byte7 = 13;
                lmc_rlevel_rank.cn78xx.byte6 = 13;
                lmc_rlevel_rank.cn78xx.byte5 = 12;
                lmc_rlevel_rank.cn78xx.byte4 = 12;
                lmc_rlevel_rank.cn78xx.byte3 = 12;
                lmc_rlevel_rank.cn78xx.byte2 = 13;
                lmc_rlevel_rank.cn78xx.byte1 = 13;
                lmc_rlevel_rank.cn78xx.byte0 = 14;
#endif

                ddr_config_write_csr_node(node,
					  CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num),
					  lmc_rlevel_rank.u64);
                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

                ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_rlevel_rank.cn78xx.status,
                          lmc_rlevel_rank.u64,
                          lmc_rlevel_rank.cn78xx.byte8,
                          lmc_rlevel_rank.cn78xx.byte7,
                          lmc_rlevel_rank.cn78xx.byte6,
                          lmc_rlevel_rank.cn78xx.byte5,
                          lmc_rlevel_rank.cn78xx.byte4,
                          lmc_rlevel_rank.cn78xx.byte3,
                          lmc_rlevel_rank.cn78xx.byte2,
                          lmc_rlevel_rank.cn78xx.byte1,
                          lmc_rlevel_rank.cn78xx.byte0
                          );
            }

#if 0
            ddr_print("Rank %d: MPR values for Page 2\n", rankx);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx,
			  /* page */ 2, /* location */ 0, &mpr_data[0]);
            ddr_print("MPR Page 2, Loc 0 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 2,
			  /* location */ 1, &mpr_data[0]);
            ddr_print("MPR Page 2, Loc 1 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 2,
			  /* location */ 2, &mpr_data[0]);
            ddr_print("MPR Page 2, Loc 2 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 2,
			  /* location */ 3, &mpr_data[0]);
            ddr_print("MPR Page 2, Loc 3 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr_print("Rank %d: MPR values for Page 0\n", rankx);
            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx,
			  /* page */ 0, /* location */ 0, &mpr_data[0]);
            ddr_print("MPR Page 0, Loc 0 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 0,
			  /* location */ 1, &mpr_data[0]);
            ddr_print("MPR Page 0, Loc 1 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 0,
			  /* location */ 2, &mpr_data[0]);
            ddr_print("MPR Page 0, Loc 2 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);

            ddr4_mpr_read(cpu_id, node, ddr_interface_num, rankx, /* page */ 0,
			  /* location */ 3, &mpr_data[0]);
            ddr_print("MPR Page 0, Loc 3 %016llx.%016llx.%016llx\n", mpr_data[2], mpr_data[1], mpr_data[0]);
#endif
        }
    } /*  (ddr_type == DDR4_DRAM) */
#endif

    /*
     * 4.8.10 LMC Read Leveling
     *
     * LMC supports an automatic read-leveling separately per byte-lane using
     * the DDR3 multipurpose register predefined pattern for system
     * calibration defined in the JEDEC DDR3 specifications.
     *
     * All of DDR PLL, LMC CK, and LMC DRESET, and early LMC initializations
     * must be completed prior to starting this LMC read-leveling sequence.
     *
     * Software could simply write the desired read-leveling values into
     * LMC(0)_RLEVEL_RANK(0..3). This section describes a sequence that uses
     * LMC's autoread-leveling capabilities.
     *
     * When LMC does the read-leveling sequence for a rank, it first enables
     * the DDR3 multipurpose register predefined pattern for system
     * calibration on the selected DRAM rank via a DDR3 MR3 write, then
     * executes 64 RD operations at different internal delay settings, then
     * disables the predefined pattern via another DDR3 MR3 write
     * operation. LMC determines the pass or fail of each of the 64 settings
     * independently for each byte lane, then writes appropriate
     * LMC(0)_RLEVEL_RANK(0..3)[BYTE*] values for the rank.
     *
     * After read-leveling for a rank, software can read the 64 pass/fail
     * indications for one byte lane via LMC(0)_RLEVEL_DBG[BITMASK]. Software
     * can observe all pass/fail results for all byte lanes in a rank via
     * separate read-leveling sequences on the rank with different
     * LMC(0)_RLEVEL_CTL[BYTE] values.
     *
     * The 64 pass/fail results will typically have failures for the low
     * delays, followed by a run of some passing settings, followed by more
     * failures in the remaining high delays.  LMC sets
     * LMC(0)_RLEVEL_RANK(0..3)[BYTE*] to one of the passing settings.
     * First, LMC selects the longest run of successes in the 64 results. (In
     * the unlikely event that there is more than one longest run, LMC
     * selects the first one.) Then if LMC(0)_RLEVEL_CTL[OFFSET_EN] = 1 and
     * the selected run has more than LMC(0)_RLEVEL_CTL[OFFSET] successes,
     * LMC selects the last passing setting in the run minus
     * LMC(0)_RLEVEL_CTL[OFFSET]. Otherwise LMC selects the middle setting in
     * the run (rounding earlier when necessary). We expect the read-leveling
     * sequence to produce good results with the reset values
     * LMC(0)_RLEVEL_CTL [OFFSET_EN]=1, LMC(0)_RLEVEL_CTL[OFFSET] = 2.
     *
     * The read-leveling sequence has the following steps:
     *
     * 1. Select desired LMC(0)_RLEVEL_CTL[OFFSET_EN,OFFSET,BYTE] settings.
     *    Do the remaining substeps 2-4 separately for each rank i with
     *    attached DRAM.
     *
     * 2. Without changing any other fields in LMC(0)_CONFIG,
     *
     *    o write LMC(0)_SEQ_CTL[SEQ_SEL] to select read-leveling
     *
     *    o write LMC(0)_CONFIG[RANKMASK] = (1 << i)
     *
     *    o write LMC(0)_SEQ_CTL[INIT_START] = 1
     *
     *    This initiates the previously-described read-leveling.
     *
     * 3. Wait until LMC(0)_RLEVEL_RANKi[STATUS] != 2
     *
     *    LMC will have updated LMC(0)_RLEVEL_RANKi[BYTE*] for all byte lanes
     *    at this point.
     *
     *    If ECC DRAM is not present (i.e. when DRAM is not attached to the
     *    DDR_CBS_0_* and DDR_CB<7:0> chip signals, or the DDR_DQS_<4>_* and
     *    DDR_DQ<35:32> chip signals), write LMC(0)_RLEVEL_RANK*[BYTE8] =
     *    LMC(0)_RLEVEL_RANK*[BYTE0]. Write LMC(0)_RLEVEL_RANK*[BYTE4] =
     *    LMC(0)_RLEVEL_RANK*[BYTE0].
     *
     * 4. If desired, consult LMC(0)_RLEVEL_DBG[BITMASK] and compare to
     *    LMC(0)_RLEVEL_RANKi[BYTE*] for the lane selected by
     *    LMC(0)_RLEVEL_CTL[BYTE]. If desired, modify LMC(0)_RLEVEL_CTL[BYTE]
     *    to a new value and repeat so that all BITMASKs can be observed.
     *
     * 5. Initialize LMC(0)_RLEVEL_RANK* values for all unused ranks.
     *
     *    Let rank i be a rank with attached DRAM.
     *
     *    For all ranks j that do not have attached DRAM, set
     *    LMC(0)_RLEVEL_RANKj = LMC(0)_RLEVEL_RANKi.
     *
     * This read-leveling sequence can help select the proper CN70XX ODT
     * resistance value (LMC(0)_COMP_CTL2[RODT_CTL]). A hardware-generated
     * LMC(0)_RLEVEL_RANKi[BYTEj] value (for a used byte lane j) that is
     * drastically different from a neighboring LMC(0)_RLEVEL_RANKi[BYTEk]
     * (for a used byte lane k) can indicate that the CN70XX ODT value is
     * bad. It is possible to simultaneously optimize both
     * LMC(0)_COMP_CTL2[RODT_CTL] and LMC(0)_RLEVEL_RANKn[BYTE*] values by
     * performing this read-leveling sequence for several
     * LMC(0)_COMP_CTL2[RODT_CTL] values and selecting the one with the best
     * LMC(0)_RLEVEL_RANKn[BYTE*] profile for the ranks.
     */

    {
#pragma pack(push,1)
        cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        cvmx_lmcx_comp_ctl2_t lmc_comp_ctl2;
        cvmx_lmcx_rlevel_ctl_t rlevel_ctl;
        cvmx_lmcx_control_t lmc_control;
        cvmx_lmcx_modereg_params1_t lmc_modereg_params1;
        unsigned char rodt_ctl;
        unsigned char rankx = 0;
        int rlevel_rodt_errors = 0;
        unsigned char ecc_ena;
        unsigned char rtt_nom;
        unsigned char rtt_idx;
        char min_rtt_nom_idx;
        char max_rtt_nom_idx;
        char min_rodt_ctl;
        char max_rodt_ctl;
        int rlevel_debug_loops = 1;
        unsigned char save_ddr2t;
        int rlevel_avg_loops;
        char ddr_rlevel_compute;
        char saved_ddr__ptune, saved_ddr__ntune, rlevel_comp_offset;
        char saved_int_zqcs_dis = 0;
        char disable_sequential_delay_check = 0;
        int maximum_adjacent_rlevel_delay_increment = 0;
        struct {
            uint64_t setting;
            int      score;
        } rlevel_scoreboard[RTT_NOM_OHMS_COUNT][RODT_OHMS_COUNT][4];
        int print_nom_ohms;
        int ddr_rlevel_printall = 0;

#ifdef ENABLE_HARDCODED_RLEVEL
        char part_number[21] = {0};
#endif /* ENABLE_HARDCODED_RLEVEL */

#if PERFECT_BITMASK_COUNTING
        typedef struct {
            uint8_t count[9][32]; // 8+ECC by 32 values
            uint8_t total[9];     // 8+ECC
        } rank_perfect_t;
        rank_perfect_t rank_perfect_counts[4];

        memset(rank_perfect_counts, 0, sizeof(rank_perfect_counts));
#endif /* PERFECT_BITMASK_COUNTING */

#pragma pack(pop)

        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
        save_ddr2t                    = lmc_control.cn78xx.ddr2t;

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        ecc_ena = lmc_config.cn78xx.ecc_ena;

        if ((s = lookup_env_parameter("ddr_rlevel_2t")) != NULL) {
            lmc_control.cn78xx.ddr2t = simple_strtoul(s, NULL, 0);
        }

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);

        ddr_print("LMC%d: Performing Read-Leveling\n", ddr_interface_num);

        rlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));

        rlevel_avg_loops = custom_lmc_config->rlevel_average_loops;
        if (rlevel_avg_loops == 0) {
            rlevel_avg_loops = RLEVEL_AVG_LOOPS_DEFAULT;
            if ((dimm_count == 1) || (num_ranks == 1)) // up the samples for these cases
                rlevel_avg_loops = rlevel_avg_loops * 2 + 1;
        }

        ddr_rlevel_compute = custom_lmc_config->rlevel_compute;
        rlevel_ctl.cn78xx.offset_en = custom_lmc_config->offset_en;
        rlevel_ctl.cn78xx.offset    = spd_rdimm
            ? custom_lmc_config->offset_rdimm
            : custom_lmc_config->offset_udimm;

        rlevel_ctl.cn78xx.delay_unload_0 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_1 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_2 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_3 = 1; /* should normally be set */

        // use OR_DIS=1 to try for better results
        rlevel_ctl.cn78xx.or_dis = 1;

        /* If we will be switching to 32bit mode level based on only
           four bits because there are only 4 ECC bits. */
        rlevel_ctl.cn78xx.bitmask = (ddr_interface_64b) ? 0xFF : 0x0F;

        // allow overrides
        if ((s = lookup_env_parameter("ddr_rlevel_ctl_or_dis")) != NULL) {
            rlevel_ctl.cn78xx.or_dis = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_rlevel_ctl_bitmask")) != NULL) {
            rlevel_ctl.cn78xx.bitmask = simple_strtoul(s, NULL, 0);
        }

        rlevel_comp_offset = spd_rdimm
            ? custom_lmc_config->rlevel_comp_offset_rdimm
            : custom_lmc_config->rlevel_comp_offset_udimm;

        if ((s = lookup_env_parameter("ddr_rlevel_offset")) != NULL) {
            rlevel_ctl.cn78xx.offset   = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_offset_en")) != NULL) {
            rlevel_ctl.cn78xx.offset_en   = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_rlevel_ctl")) != NULL) {
            rlevel_ctl.u64   = simple_strtoul(s, NULL, 0);
        }

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_RLEVEL_CTL(ddr_interface_num),
				  rlevel_ctl.u64);

        if ((s = lookup_env_parameter("ddr%d_rlevel_debug_loops", ddr_interface_num)) != NULL) {
            rlevel_debug_loops = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rtt_nom_auto")) != NULL) {
            ddr_rtt_nom_auto = !!simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_average")) != NULL) {
            rlevel_avg_loops = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_compute")) != NULL) {
            ddr_rlevel_compute = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_printall")) != NULL) {
            ddr_rlevel_printall = simple_strtoul(s, NULL, 0);
        }

        ddr_print("RLEVEL_CTL                                    : 0x%016llx\n", rlevel_ctl.u64);
        ddr_print("RLEVEL_OFFSET                                 : %6d\n", rlevel_ctl.cn78xx.offset);
        ddr_print("RLEVEL_OFFSET_EN                              : %6d\n", rlevel_ctl.cn78xx.offset_en);

        /* The purpose for the indexed table is to sort the settings
        ** by the ohm value to simplify the testing when incrementing
        ** through the settings.  (index => ohms) 1=120, 2=60, 3=40,
        ** 4=30, 5=20 */
        min_rtt_nom_idx = (custom_lmc_config->min_rtt_nom_idx == 0) ? 1 : custom_lmc_config->min_rtt_nom_idx;
        max_rtt_nom_idx = (custom_lmc_config->max_rtt_nom_idx == 0) ? 5 : custom_lmc_config->max_rtt_nom_idx;

        min_rodt_ctl = (custom_lmc_config->min_rodt_ctl == 0) ? 1 : custom_lmc_config->min_rodt_ctl;
        max_rodt_ctl = (custom_lmc_config->max_rodt_ctl == 0) ? 5 : custom_lmc_config->max_rodt_ctl;

        if ((s = lookup_env_parameter("ddr_min_rodt_ctl")) != NULL) {
            min_rodt_ctl = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_max_rodt_ctl")) != NULL) {
            max_rodt_ctl = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_min_rtt_nom_idx")) != NULL) {
            min_rtt_nom_idx = simple_strtoul(s, NULL, 0);
        }
        if ((s = lookup_env_parameter("ddr_max_rtt_nom_idx")) != NULL) {
            max_rtt_nom_idx = simple_strtoul(s, NULL, 0);
        }

#ifdef ENABLE_HARDCODED_RLEVEL
        if (custom_lmc_config->rlevel_table != NULL) {
            /* Check for hard-coded read-leveling settings */
            get_dimm_part_number(part_number, &dimm_config_table[0], 0, ddr_type);
            for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;


                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

                i = 0;
                while (custom_lmc_config->rlevel_table[i].part != NULL) {
                    debug_print("DIMM part number:\"%s\", SPD: \"%s\"\n",
                                custom_lmc_config->rlevel_table[i].part, part_number);
                    if ((strcmp(part_number, custom_lmc_config->rlevel_table[i].part) == 0)
                        && (_abs(custom_lmc_config->rlevel_table[i].speed - 2*ddr_hertz/(1000*1000)) < 10 )) {
                        ddr_print("Using hard-coded read leveling for DIMM part number: \"%s\"\n", part_number);
                        lmc_rlevel_rank.u64 = custom_lmc_config->rlevel_table[i].rlevel_rank[ddr_interface_num][rankx];
			ddr_config_write_csr_node(node,
						  CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num),
						  lmc_rlevel_rank.u64);
                        lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                        display_RL(node, ddr_interface_num, lmc_rlevel_rank, rankx);
                        rlevel_debug_loops = 0; /* Disable h/w read-leveling */
                        break;
                    }
                    ++i;
                }
            }
        }
#endif /* ENABLE_HARDCODED_RLEVEL */

        while(rlevel_debug_loops--) {
	    /* Initialize the error scoreboard */
#if 0
	    for (rtt_idx = 1; rtt_idx < RTT_NOM_OHMS_COUNT; ++rtt_idx) {
		rtt_nom = imp_values->rtt_nom_table[rtt_idx];
		for (rodt_ctl = RODT_OHMS_COUNT - 1; rodt_ctl > 0; --rodt_ctl) {
		    for (rankx = 0; rankx < dimm_count * 4; rankx++) {
			if (!(rank_mask & (1 << rankx)))
			    continue;
			rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score   = 0;
			rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting = 0;
		    }
		}
	    }
#else
	    memset(rlevel_scoreboard, 0, sizeof(rlevel_scoreboard));
#endif

	    if ((s = lookup_env_parameter("ddr_rlevel_comp_offset")) != NULL) {
		rlevel_comp_offset = strtoul(s, NULL, 0);
	    }

	    disable_sequential_delay_check = custom_lmc_config->disable_sequential_delay_check;

	    if ((s = lookup_env_parameter("ddr_disable_sequential_delay_check")) != NULL) {
		disable_sequential_delay_check = strtoul(s, NULL, 0);
	    }

	    maximum_adjacent_rlevel_delay_increment = custom_lmc_config->maximum_adjacent_rlevel_delay_increment;

	    if ((s = lookup_env_parameter("ddr_maximum_adjacent_rlevel_delay_increment")) != NULL) {
		maximum_adjacent_rlevel_delay_increment = strtoul(s, NULL, 0);
	    }

	    //lmc_comp_ctl2.u = BDK_CSR_READ(node, BDK_LMCX_COMP_CTL2(ddr_interface_num));
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
	    saved_ddr__ptune = lmc_comp_ctl2.cn78xx.ddr__ptune;
	    saved_ddr__ntune = lmc_comp_ctl2.cn78xx.ddr__ntune;

	    /* Disable dynamic compensation settings */
	    if (rlevel_comp_offset != 0) {
		lmc_comp_ctl2.cn78xx.ptune = saved_ddr__ptune;
		lmc_comp_ctl2.cn78xx.ntune = saved_ddr__ntune;

		/* Round up the ptune calculation to bias the odd cases toward ptune */
		lmc_comp_ctl2.cn78xx.ptune += divide_roundup(rlevel_comp_offset, 2);
		lmc_comp_ctl2.cn78xx.ntune -= rlevel_comp_offset/2;

		//lmc_control.u = BDK_CSR_READ(node, BDK_LMCX_CONTROL(ddr_interface_num));
                lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
		saved_int_zqcs_dis = lmc_control.s.int_zqcs_dis;
		lmc_control.s.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
		ddr_config_write_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

		lmc_comp_ctl2.cn78xx.byp = 1; /* Enable bypass mode */
		ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
		//BDK_CSR_READ(node, BDK_LMCX_COMP_CTL2(ddr_interface_num));
                cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
		//lmc_comp_ctl2.u = BDK_CSR_READ(node, BDK_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
                lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
		ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
			  lmc_comp_ctl2.cn78xx.ddr__ptune, lmc_comp_ctl2.cn78xx.ddr__ntune);
	    }

	    lmc_modereg_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

	    for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) {
		rtt_nom = imp_values->rtt_nom_table[rtt_idx];

		/* When the read ODT mask is zero the dyn_rtt_nom_mask is
		   zero than RTT_NOM will not be changing during
		   read-leveling.  Since the value is fixed we only need
		   to test it once. */
		if (dyn_rtt_nom_mask == 0) {
                    print_nom_ohms = -1; // flag not to print NOM ohms
                    if (rtt_idx != min_rtt_nom_idx)
                        continue;
                } else {
                    if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.s.rtt_nom_00 = rtt_nom;
                    if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.s.rtt_nom_01 = rtt_nom;
                    if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.s.rtt_nom_10 = rtt_nom;
                    if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.s.rtt_nom_11 = rtt_nom;
                    // FIXME? rank 0 ohms always?
                    print_nom_ohms = imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00];
                }

		ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num), lmc_modereg_params1.u64);
		ddr_print("\n");
		ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
			  lmc_modereg_params1.s.rtt_nom_11,
			  lmc_modereg_params1.s.rtt_nom_10,
			  lmc_modereg_params1.s.rtt_nom_01,
			  lmc_modereg_params1.s.rtt_nom_00);

		perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

                // Try RANK outside RODT to rearrange the output...
                for (rankx = 0; rankx < dimm_count * 4; rankx++) {
                    int byte_idx;
                    rlevel_byte_data_t rlevel_byte[9];
                    int average_loops;
                    int rlevel_rank_errors, rlevel_bitmask_errors, rlevel_nonseq_errors;
                    rlevel_bitmask_t rlevel_bitmask[9];
#if PICK_BEST_RANK_SCORE_NOT_AVG
                    int rlevel_best_rank_score;
#endif

                    if (!(rank_mask & (1 << rankx)))
                        continue;

                    for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) {
#if PICK_BEST_RANK_SCORE_NOT_AVG
                        rlevel_best_rank_score = DEFAULT_BEST_RANK_SCORE;
#endif
                        rlevel_rodt_errors = 0;
                        lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                        lmc_comp_ctl2.cn78xx.rodt_ctl = rodt_ctl;
                        ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
                        lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
                        cvmx_wait_usec(1); /* Give it a little time to take affect */
                        if (ddr_rlevel_printall) {
                            ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                                      lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);
                        }
                        memset(rlevel_byte, 0, sizeof(rlevel_byte));

			for (average_loops = 0; average_loops < rlevel_avg_loops; average_loops++) {
			    rlevel_bitmask_errors = 0;

			    if (! (rlevel_separate_ab && spd_rdimm && (ddr_type == DDR4_DRAM))) {
				/* Clear read-level delays */
				ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), 0);

                                /* read-leveling */
				perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx, ddr_interface_num, 1);

                                do {
                                    lmc_rlevel_rank.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                                } while (lmc_rlevel_rank.cn78xx.status != 3);
			    }

			    lmc_rlevel_rank.u64 =
                                cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

			    { // start bitmask interpretation block
                                int redoing_nonseq_errs = 0;

                                memset(rlevel_bitmask, 0, sizeof(rlevel_bitmask));

				if (rlevel_separate_ab && spd_rdimm && (ddr_type == DDR4_DRAM)) {
				    cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank_aside;
				    cvmx_lmcx_modereg_params0_t lmc_modereg_params0;

				    /* A-side */
				    lmc_modereg_params0.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
				    lmc_modereg_params0.s.mprloc = 0; /* MPR Page 0 Location 0 */
				    ddr_config_write_csr_node(node,
                                                              CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                                              lmc_modereg_params0.u64);

				    /* Clear read-level delays */
				    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), 0);

                                    /* read-leveling */
				    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx, ddr_interface_num, 1);

                                    do {
                                        lmc_rlevel_rank.u64 =
                                            cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                                    } while (lmc_rlevel_rank.cn78xx.status != 3);

				    lmc_rlevel_rank.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

				    lmc_rlevel_rank_aside.u64 = lmc_rlevel_rank.u64;

				    rlevel_bitmask[0].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 0);
				    rlevel_bitmask[1].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 1);
				    rlevel_bitmask[2].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 2);
				    rlevel_bitmask[3].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 3);
				    rlevel_bitmask[8].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 8);
				    /* A-side complete */


				    /* B-side */
				    lmc_modereg_params0.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
				    lmc_modereg_params0.s.mprloc = 3; /* MPR Page 0 Location 3 */
				    ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                                              lmc_modereg_params0.u64);

				    /* Clear read-level delays */
				    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), 0);

                                    /* read-leveling */
				    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx, ddr_interface_num, 1);

                                    do {
                                        lmc_rlevel_rank.u64 =
                                            cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                                    } while (lmc_rlevel_rank.cn78xx.status != 3);

				    lmc_rlevel_rank.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

				    rlevel_bitmask[4].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 4);
				    rlevel_bitmask[5].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 5);
				    rlevel_bitmask[6].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 6);
				    rlevel_bitmask[7].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, 7);
				    /* B-side complete */


				    update_rlevel_rank_struct(&lmc_rlevel_rank, 0, lmc_rlevel_rank_aside.s.byte0);
				    update_rlevel_rank_struct(&lmc_rlevel_rank, 1, lmc_rlevel_rank_aside.s.byte1);
				    update_rlevel_rank_struct(&lmc_rlevel_rank, 2, lmc_rlevel_rank_aside.s.byte2);
				    update_rlevel_rank_struct(&lmc_rlevel_rank, 3, lmc_rlevel_rank_aside.s.byte3);
                                    /* ECC A-side */
				    update_rlevel_rank_struct(&lmc_rlevel_rank, 8, lmc_rlevel_rank_aside.s.byte8);

				    lmc_modereg_params0.u64 =
                                        cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
				    lmc_modereg_params0.s.mprloc = 0; /* MPR Page 0 Location 0 */
				    ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                                              lmc_modereg_params0.u64);
				} /* if (rlevel_separate_ab && spd_rdimm && (ddr_type == DDR4_DRAM)) */

				/*
                                 * Evaluate the quality of the read-leveling delays from the bitmasks.
                                 * Also save off a software computed read-leveling mask that may be
                                 * used later to qualify the delay results from Octeon.
                                 */
				for (byte_idx = 0; byte_idx < (8+ecc_ena); ++byte_idx) {
                                    int bmerr;
				    if (!(ddr_interface_bytemask&(1<<byte_idx)))
					continue;
				    if (! (rlevel_separate_ab && spd_rdimm && (ddr_type == DDR4_DRAM))) {
					rlevel_bitmask[byte_idx].bm =
                                            octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, byte_idx);
				    }
                                    bmerr = validate_ddr3_rlevel_bitmask(&rlevel_bitmask[byte_idx], ddr_type);
				    rlevel_bitmask[byte_idx].errs = bmerr;
				    rlevel_bitmask_errors += bmerr;
#if PERFECT_BITMASK_COUNTING
                                    if ((ddr_type == DDR4_DRAM) && !bmerr) { // count only the "perfect" bitmasks
                                        // FIXME: could optimize this a bit?
                                        int delay = get_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx);
                                        rank_perfect_counts[rankx].count[byte_idx][delay] += 1;
                                        rank_perfect_counts[rankx].total[byte_idx] += 1;
                                    }
#endif /* PERFECT_BITMASK_COUNTING */
				}

				/* Set delays for unused bytes to match byte 0. */
				for (byte_idx = 0; byte_idx < 9; ++byte_idx) {
				    if (ddr_interface_bytemask & (1 << byte_idx))
					continue;
				    update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx, lmc_rlevel_rank.s.byte0);
				}

				/* Save a copy of the byte delays in physical
				   order for sequential evaluation. */
				unpack_rlevel_settings(ddr_interface_bytemask, ecc_ena, rlevel_byte, lmc_rlevel_rank);

                            redo_nonseq_errs:

                                rlevel_nonseq_errors  = 0;
				if (! disable_sequential_delay_check) {
                                    for (byte_idx = 0; byte_idx < 9; ++byte_idx) {
                                        rlevel_byte[byte_idx].sqerrs = 0;
                                    }
				    if ((ddr_interface_bytemask & 0xff) == 0xff) {

					/* Evaluate delay sequence across the whole range of bytes for standard dimms. */
					if ((spd_dimm_type == 1) || (spd_dimm_type == 5)) { /* 1=RDIMM, 5=Mini-RDIMM */
					    int register_adjacent_delay = _abs(rlevel_byte[4].delay - rlevel_byte[5].delay);
					    /* Registered dimm topology routes from the center. */
					    rlevel_nonseq_errors += nonsequential_delays(rlevel_byte, 0, 3+ecc_ena,
                                                                                       maximum_adjacent_rlevel_delay_increment);
					    rlevel_nonseq_errors += nonsequential_delays(rlevel_byte, 5, 7+ecc_ena,
                                                                                       maximum_adjacent_rlevel_delay_increment);
                                            // byte 5 sqerrs never gets cleared for RDIMMs
                                            rlevel_byte[5].sqerrs = 0;
					    if (register_adjacent_delay > 1) {
						/* Assess proximity of bytes on opposite sides of register */
						rlevel_nonseq_errors += (register_adjacent_delay-1) * RLEVEL_ADJACENT_DELAY_ERROR;
                                                // update byte 5 error
						rlevel_byte[5].sqerrs += (register_adjacent_delay-1) * RLEVEL_ADJACENT_DELAY_ERROR;
					    }
					}
					if ((spd_dimm_type == 2) || (spd_dimm_type == 6)) { /* 2=UDIMM, 6=Mini-UDIMM */
					    /* Unbuffered dimm topology routes from end to end. */
					    rlevel_nonseq_errors += nonsequential_delays(rlevel_byte, 0, 7+ecc_ena,
                                                                                       maximum_adjacent_rlevel_delay_increment);
					}
				    } else {
					rlevel_nonseq_errors += nonsequential_delays(rlevel_byte, 0, 3+ecc_ena,
                                                                                   maximum_adjacent_rlevel_delay_increment);
				    }
				} /* if (! disable_sequential_delay_check) */

#if 0
                                // FIXME FIXME: disabled for now, it was too much...

                                // Calculate total errors for the rank:
                                // we do NOT add nonsequential errors if mini-[RU]DIMM or x16;
                                // mini-DIMMs and x16 devices have unusual sequence geometries.
                                // Make the final scores for them depend only on the bitmasks...
                                rlevel_rank_errors = rlevel_bitmask_errors;
                                if ((spd_dimm_type != 5) &&
                                    (spd_dimm_type != 6) &&
                                    (dram_width != 16))
                                {
                                    rlevel_rank_errors += rlevel_nonseq_errors;
                                }
#else
                                rlevel_rank_errors = rlevel_bitmask_errors + rlevel_nonseq_errors;
#endif
                                
				// print original sample here only if we are not really averaging or picking best
                                // also do not print if we were redoing the NONSEQ score for using COMPUTED
				if (!redoing_nonseq_errs && (rlevel_avg_loops < 2)) {
                                    if (ddr_rlevel_printall) {
                                        display_RL_BM(node, ddr_interface_num, rankx, rlevel_bitmask, ecc_ena);
                                        display_RL_BM_scores(node, ddr_interface_num, rankx, rlevel_bitmask, ecc_ena);
                                        display_RL_SEQ_scores(node, ddr_interface_num, rankx, rlevel_byte, ecc_ena);
                                    }
                                    display_RL_with_score(node, ddr_interface_num, lmc_rlevel_rank, rankx, rlevel_rank_errors);
                                }

				if (ddr_rlevel_compute) {
                                    if (!redoing_nonseq_errs) {
                                        /* Recompute the delays based on the bitmask */
                                        for (byte_idx = 0; byte_idx < (8+ecc_ena); ++byte_idx) {
                                            if (!(ddr_interface_bytemask & (1 << byte_idx)))
                                                continue;
                                            update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx,
                                                                      compute_ddr3_rlevel_delay(rlevel_bitmask[byte_idx].mstart,
                                                                                                rlevel_bitmask[byte_idx].width,
                                                                                                rlevel_ctl));
                                        }

                                        /* Override the copy of byte delays with the computed results. */
                                        unpack_rlevel_settings(ddr_interface_bytemask, ecc_ena, rlevel_byte, lmc_rlevel_rank);

                                        redoing_nonseq_errs = 1;
                                        goto redo_nonseq_errs;

                                    } else {
                                        /* now print this if already printed the original sample */
                                        if ((rlevel_avg_loops < 2) || ddr_rlevel_printall) {
                                            display_RL_with_computed(node, ddr_interface_num,
                                                                     lmc_rlevel_rank, rankx,
                                                                     rlevel_rank_errors);
                                        }
                                    }
				} /* if (ddr_rlevel_compute) */


			    }  // end bitmask interpretation block

#if PICK_BEST_RANK_SCORE_NOT_AVG
			    // if it is a better (lower) score, then  keep it
			    if (rlevel_rank_errors < rlevel_best_rank_score) {
				rlevel_best_rank_score = rlevel_rank_errors;

				// save the new best delays and best errors
				for (byte_idx = 0; byte_idx < (8+ecc_ena); ++byte_idx) {
				    rlevel_byte[byte_idx].best = rlevel_byte[byte_idx].delay;
				    rlevel_byte[byte_idx].bestsq = rlevel_byte[byte_idx].sqerrs;
                                    // save bitmasks and their scores as well
                                    // xlate UNPACKED index to PACKED index to get from rlevel_bitmask
				    rlevel_byte[byte_idx].bm     = rlevel_bitmask[XUP(byte_idx, !!ecc_ena)].bm;
				    rlevel_byte[byte_idx].bmerrs = rlevel_bitmask[XUP(byte_idx, !!ecc_ena)].errs;
				}
			    }
#else /* PICK_BEST_RANK_SCORE_NOT_AVG */

			    /* Accumulate the total score across averaging loops for this setting */
			    rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score += rlevel_rank_errors;
			    debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d\n",
					rtt_nom, rodt_ctl, rankx,
					rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);

			    /* Accumulate the delay totals and loop counts
			       necessary to compute average delay results */
			    for (byte_idx = 0; byte_idx < 9; ++byte_idx) {
				if (rlevel_byte[byte_idx].delay != 0) { /* Don't include delay=0 in the average */
				    ++rlevel_byte[byte_idx].loop_count;
				    rlevel_byte[byte_idx].loop_total += rlevel_byte[byte_idx].delay;
				}
			    }
#endif /* PICK_BEST_RANK_SCORE_NOT_AVG */

			    rlevel_rodt_errors += rlevel_rank_errors;
			} /* for (average_loops = 0; average_loops < rlevel_avg_loops; average_loops++) */

#if PICK_BEST_RANK_SCORE_NOT_AVG
			/* We recorded the best score across the averaging loops */
			rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score = rlevel_best_rank_score;

			/* Restore the delays from the best fields that go with the best score */
			for (byte_idx = 0; byte_idx < 9; ++byte_idx) {
			    rlevel_byte[byte_idx].delay = rlevel_byte[byte_idx].best;
                            rlevel_byte[byte_idx].sqerrs = rlevel_byte[byte_idx].bestsq;
                        }
#else /* PICK_BEST_RANK_SCORE_NOT_AVG */
			/* Compute the average score across averaging loops */
			rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score =
			    divide_nint(rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score, rlevel_avg_loops);

			/* Compute the average delay results */
			for (byte_idx=0; byte_idx < 9; ++byte_idx) {
			    if (rlevel_byte[byte_idx].loop_count == 0)
				rlevel_byte[byte_idx].loop_count = 1;
			    rlevel_byte[byte_idx].delay = divide_nint(rlevel_byte[byte_idx].loop_total,
								      rlevel_byte[byte_idx].loop_count);
			}
#endif /* PICK_BEST_RANK_SCORE_NOT_AVG */

			lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

			pack_rlevel_settings(ddr_interface_bytemask, ecc_ena, rlevel_byte, &lmc_rlevel_rank);

			if (rlevel_avg_loops > 1) {

#if PICK_BEST_RANK_SCORE_NOT_AVG
                            // restore the "best" bitmasks and their scores for printing
                            for (byte_idx = 0; byte_idx < 9; ++byte_idx) {
                                if ((ddr_interface_bytemask & (1 << byte_idx)) == 0)
                                    continue;
                                // xlate PACKED index to UNPACKED index to get from rlevel_byte
                                rlevel_bitmask[byte_idx].bm   = rlevel_byte[XPU(byte_idx, !!ecc_ena)].bm;
                                rlevel_bitmask[byte_idx].errs = rlevel_byte[XPU(byte_idx, !!ecc_ena)].bmerrs;
                            }
                            // maybe print bitmasks/scores here  
                            if (ddr_rlevel_printall) {
                                display_RL_BM(node, ddr_interface_num, rankx, rlevel_bitmask, ecc_ena);
                                display_RL_BM_scores(node, ddr_interface_num, rankx, rlevel_bitmask, ecc_ena);
                                display_RL_SEQ_scores(node, ddr_interface_num, rankx, rlevel_byte, ecc_ena);
                            }
                            display_RL_with_RODT(node, ddr_interface_num, lmc_rlevel_rank, rankx,
                                                 rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score,
                                                 print_nom_ohms, imp_values->rodt_ohms[rodt_ctl],
                                                 WITH_RODT_BESTSCORE);
                            // FIXME: does this help make the output a little easier to focus?
                            if (ddr_rlevel_printall) {
                                ddr_print("-----------\n");
                            }

#else /* PICK_BEST_RANK_SCORE_NOT_AVG */
			    display_RL_with_average(node, ddr_interface_num, lmc_rlevel_rank, rankx,
						    rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);
#endif /* PICK_BEST_RANK_SCORE_NOT_AVG */
			}

			rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting = lmc_rlevel_rank.u64;

                    } /* for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) */
                } /* for (rankx = 0; rankx < dimm_count*4; rankx++) */
	    } /*  for (rtt_idx=min_rtt_nom_idx; rtt_idx<max_rtt_nom_idx; ++rtt_idx) */


	    /* Re-enable dynamic compensation settings. */
	    if (rlevel_comp_offset != 0) {
		lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));

		lmc_comp_ctl2.cn78xx.ptune = 0;
		lmc_comp_ctl2.cn78xx.ntune = 0;
		lmc_comp_ctl2.cn78xx.byp = 0; /* Disable bypass mode */
		ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
		cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read once */

		lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
		ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
			  lmc_comp_ctl2.cn78xx.ddr__ptune, lmc_comp_ctl2.cn78xx.ddr__ntune);

		lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
		lmc_control.s.int_zqcs_dis    = saved_int_zqcs_dis; /* Restore original setting */
		ddr_config_write_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

	    }


	    {
		int override_compensation = 0;
		if ((s = lookup_env_parameter("ddr__ptune")) != NULL) {
		    saved_ddr__ptune = strtoul(s, NULL, 0);
		    override_compensation = 1;
		}
		if ((s = lookup_env_parameter("ddr__ntune")) != NULL) {
		    saved_ddr__ntune = strtoul(s, NULL, 0);
		    override_compensation = 1;
		}
		if (override_compensation) {
		    lmc_comp_ctl2.cn78xx.ptune = saved_ddr__ptune;
		    lmc_comp_ctl2.cn78xx.ntune = saved_ddr__ntune;

		    lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
		    saved_int_zqcs_dis = lmc_control.s.int_zqcs_dis;
		    lmc_control.s.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
		    ddr_config_write_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num), lmc_control.u64);

		    lmc_comp_ctl2.cn78xx.byp = 1; /* Enable bypass mode */
		    ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
		    lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */

		    ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
			      lmc_comp_ctl2.cn78xx.ptune, lmc_comp_ctl2.cn78xx.ntune);
		}
	    }

	    { /* Evaluation block */
		int      best_rodt_score = DEFAULT_BEST_RANK_SCORE; /* Still at initial value? */
		int      auto_rodt_ctl = 0;
		int      auto_rtt_nom  = 0;
		int      rodt_score;
		int      rodt_row_skip_mask = 0;

		// just add specific RODT rows to the skip mask for DDR4 at this time...
		if (ddr_type == DDR4_DRAM) {
		    rodt_row_skip_mask |= (1 << ddr4_rodt_ctl_34_ohm); // skip RODT row 34 ohms for all DDR4 types
		    rodt_row_skip_mask |= (1 << ddr4_rodt_ctl_40_ohm); // skip RODT row 40 ohms for all DDR4 types
#if ADD_48_OHM_SKIP
		    rodt_row_skip_mask |= (1 << ddr4_rodt_ctl_48_ohm); // skip RODT row 48 ohms for all DDR4 types
#endif /* ADD_48OHM_SKIP */
#if NOSKIP_40_48_OHM
		    // For now, do not skip RODT row 40 or 48 ohm when ddr_hertz is above 1075 MHz
		    if (ddr_hertz > 1075000000) {
			rodt_row_skip_mask &= ~(1 << ddr4_rodt_ctl_40_ohm); // noskip RODT row 40 ohms
			rodt_row_skip_mask &= ~(1 << ddr4_rodt_ctl_48_ohm); // noskip RODT row 48 ohms
		    }
#endif /* NOSKIP_40_48_OHM */
#if NOSKIP_48_STACKED
		    // For now, do not skip RODT row 48 ohm for 2Rx4 stacked die DIMMs
		    if ((is_stacked_die) && (num_ranks == 2) && (dram_width == 4)) {
			rodt_row_skip_mask &= ~(1 << ddr4_rodt_ctl_48_ohm); // noskip RODT row 48 ohms
		    }
#endif /* NOSKIP_48_STACKED */
#if NOSKIP_FOR_MINI
                    // for now, leave all rows eligible when we have mini-DIMMs...
                    if ((spd_dimm_type == 5) || (spd_dimm_type == 6)) {
                        rodt_row_skip_mask = 0;
                    }
#endif /* NOSKIP_FOR_MINI */
#if NOSKIP_FOR_2S_1R
                    // for now, leave all rows eligible when we have a 2-slot 1-rank config
                    if ((dimm_count == 2) && (num_ranks == 1)) {
                        rodt_row_skip_mask = 0;
                    }
#endif /* NOSKIP_FOR_2S_1R */
		} else { // must be DDR3
                    rodt_row_skip_mask |= (1 << rodt_ctl_20_ohm); // skip RODT row 20 ohms for all DDR3
                }

		ddr_print("Evaluating Read-Leveling Scoreboard for AUTO settings.\n");
		for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) {
		    rtt_nom = imp_values->rtt_nom_table[rtt_idx];

		    /* When the read ODT mask is zero the dyn_rtt_nom_mask is
		       zero than RTT_NOM will not be changing during
		       read-leveling.  Since the value is fixed we only need
		       to test it once. */
		    if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
			continue;

		    for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) {
			rodt_score = 0;
			for (rankx = 0; rankx < dimm_count * 4;rankx++) {
			    if (!(rank_mask & (1 << rankx)))
				continue;
			    debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d\n",
					rtt_nom, rodt_ctl, rankx, rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);
			    rodt_score += rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score;
			}
			// FIXME: do we need to skip RODT rows here, like we do below in the by-RANK settings?

			/* When using automatic ODT settings use the ODT
			   settings associated with the best score for
			   all of the tested ODT combinations. */

			if ((rodt_score < best_rodt_score) || // always take lower score, OR
			    ((rodt_score == best_rodt_score) && // take same score if RODT ohms are higher
			     (imp_values->rodt_ohms[rodt_ctl] > imp_values->rodt_ohms[auto_rodt_ctl])))
                        {
                            debug_print("AUTO: new best score for rodt:%d (%d), new score:%d, previous score:%d\n",
                                        rodt_ctl, imp_values->rodt_ohms[rodt_ctl], rodt_score, best_rodt_score);
                            best_rodt_score = rodt_score;
                            auto_rodt_ctl   = rodt_ctl;
                            auto_rtt_nom    = rtt_nom;
                        }
		    } /* for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) */
		} /* for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) */

		lmc_modereg_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

		if (ddr_rtt_nom_auto) {
		    /* Store the automatically set RTT_NOM value */
		    if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.s.rtt_nom_00 = auto_rtt_nom;
		    if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.s.rtt_nom_01 = auto_rtt_nom;
		    if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.s.rtt_nom_10 = auto_rtt_nom;
		    if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.s.rtt_nom_11 = auto_rtt_nom;
		} else {
		    /* restore the manual settings to the register */
		    lmc_modereg_params1.s.rtt_nom_00 = default_rtt_nom[0];
		    lmc_modereg_params1.s.rtt_nom_01 = default_rtt_nom[1];
		    lmc_modereg_params1.s.rtt_nom_10 = default_rtt_nom[2];
		    lmc_modereg_params1.s.rtt_nom_11 = default_rtt_nom[3];
		}

		ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num), lmc_modereg_params1.u64);
		ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_11],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_10],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_01],
			  imp_values->rtt_nom_ohms[lmc_modereg_params1.s.rtt_nom_00],
			  lmc_modereg_params1.s.rtt_nom_11,
			  lmc_modereg_params1.s.rtt_nom_10,
			  lmc_modereg_params1.s.rtt_nom_01,
			  lmc_modereg_params1.s.rtt_nom_00);

		ddr_print("RTT_WR      %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                          imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 3)],
                          imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 2)],
                          imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 1)],
                          imp_values->rtt_wr_ohms[EXTR_WR(lmc_modereg_params1.u64, 0)],
                          EXTR_WR(lmc_modereg_params1.u64, 3),
                          EXTR_WR(lmc_modereg_params1.u64, 2),
                          EXTR_WR(lmc_modereg_params1.u64, 1),
                          EXTR_WR(lmc_modereg_params1.u64, 0));

		ddr_print("DIC         %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
			  imp_values->dic_ohms[lmc_modereg_params1.s.dic_11],
			  imp_values->dic_ohms[lmc_modereg_params1.s.dic_10],
			  imp_values->dic_ohms[lmc_modereg_params1.s.dic_01],
			  imp_values->dic_ohms[lmc_modereg_params1.s.dic_00],
			  lmc_modereg_params1.s.dic_11,
			  lmc_modereg_params1.s.dic_10,
			  lmc_modereg_params1.s.dic_01,
			  lmc_modereg_params1.s.dic_00);

		if (ddr_type == DDR4_DRAM) {
		    cvmx_lmcx_modereg_params2_t lmc_modereg_params2;
		    /*
		     * We must read the CSR, and not depend on odt_config[odt_idx].odt_mask2,
		     * since we could have overridden values with envvars.
		     * NOTE: this corrects the printout, since the CSR is not written with the old values...
		     */
		    lmc_modereg_params2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS2(ddr_interface_num));

		    ddr_print("RTT_PARK    %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
			      imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_11],
			      imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_10],
			      imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_01],
			      imp_values->rtt_nom_ohms[lmc_modereg_params2.s.rtt_park_00],
			      lmc_modereg_params2.s.rtt_park_11,
			      lmc_modereg_params2.s.rtt_park_10,
			      lmc_modereg_params2.s.rtt_park_01,
			      lmc_modereg_params2.s.rtt_park_00);

		    ddr_print("%-45s :  0x%x,0x%x,0x%x,0x%x\n", "VREF_RANGE",
			      lmc_modereg_params2.s.vref_range_11,
			      lmc_modereg_params2.s.vref_range_10,
			      lmc_modereg_params2.s.vref_range_01,
			      lmc_modereg_params2.s.vref_range_00);

		    ddr_print("%-45s :  0x%x,0x%x,0x%x,0x%x\n", "VREF_VALUE",
			      lmc_modereg_params2.s.vref_value_11,
			      lmc_modereg_params2.s.vref_value_10,
			      lmc_modereg_params2.s.vref_value_01,
			      lmc_modereg_params2.s.vref_value_00);
		}

		lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
		if (ddr_rodt_ctl_auto)
		    lmc_comp_ctl2.cn78xx.rodt_ctl = auto_rodt_ctl;
		else
		    lmc_comp_ctl2.cn78xx.rodt_ctl = default_rodt_ctl; // back to the original setting
		ddr_config_write_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num), lmc_comp_ctl2.u64);
		lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
		ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
			  lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);

		/* Use the delays associated with the best score for each individual rank */
		ddr_print("Evaluating Read-Leveling Scoreboard for per-RANK settings.\n");

		////////////////// this is the start of the RANK MAJOR LOOP

		for (rankx = 0; rankx < dimm_count * 4; rankx++) {
		    int best_rank_score = DEFAULT_BEST_RANK_SCORE; /* Start with an arbitrarily high score */
		    int best_rank_rtt_nom = 0;
		    //int best_rank_nom_ohms = 0;
		    int best_rank_ctl = 0;
		    int best_rank_ohms = 0;
		    int best_rankx = 0;
		    int dimm_rank_mask;
		    int MAX_RANK_SCORE;
		    cvmx_lmcx_rlevel_rankx_t saved_rlevel_rank;
		    //int rtt_nom_ohms;
		    int next_ohms;
		    int orankx;
		    int next_score = 0;
		    uint64_t byte_msk = 0x3f; // 6-bit fields
		    uint64_t best_byte, new_byte, temp_byte, orig_best_byte;
		    uint64_t rank_best_bytes[9];
		    int byte_idx, byte_sh;
		    uint64_t avg_byte;
		    int avg_diff;

		    if (!(rank_mask & (1 << rankx)))
			continue;

		    // some of the rank-related loops below need to operate only on the ranks of a single DIMM,
		    // so create a mask for their use here
		    if (num_ranks == 4)
			dimm_rank_mask = rank_mask; // should be 1111
		    else {
			dimm_rank_mask = rank_mask & 3; // should be 01 or 11
			if (rankx >= 2)
			    dimm_rank_mask <<= 2; // doing a rank on the second DIMM, should be 0100 or 1100
		    }
		    debug_print("DIMM rank mask: 0x%x, rank mask: 0x%x, rankx: %d\n", dimm_rank_mask, rank_mask, rankx);

		    ////////////////// this is the start of the BEST ROW SCORE LOOP

		    for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) {
			rtt_nom = imp_values->rtt_nom_table[rtt_idx];

			/* When the read ODT mask is zero the dyn_rtt_nom_mask is
			   zero than RTT_NOM will not be changing during
			   read-leveling.  Since the value is fixed we only need
			   to test it once. */
			if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
			    continue;

			debug_print("N%d.LMC%d.R%d: starting RTT_NOM %d (%d)\n",
				    node, ddr_interface_num, rankx, rtt_nom, imp_values->rtt_nom_ohms[rtt_nom]);

			for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) {
			    next_ohms = imp_values->rodt_ohms[rodt_ctl];

			    // skip RODT rows in mask, but *NOT* rows with too high a score;
			    // we will not use the skipped ones for printing or evaluating, but
			    // we need to allow all the non-skipped ones to be candidates for "best"
			    if (((1 << rodt_ctl) & rodt_row_skip_mask) != 0) {
				debug_print("N%d.LMC%d.R%d: SKIPPING rodt:%d (%d) with rank_score:%d\n",
					    node, ddr_interface_num, rankx, rodt_ctl, next_ohms, next_score);
				continue;
			    }
			    for (orankx = 0; orankx < dimm_count * 4; orankx++) { // this is ROFFIX-0528
				if (!(dimm_rank_mask & (1 << orankx))) // stay on the same DIMM
				    continue;

				next_score = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].score;

				if (next_score > best_rank_score) // always skip a higher score
				    continue;
				if (next_score == best_rank_score) { // if scores are equal
				    if (next_ohms < best_rank_ohms) // always skip lower ohms
					continue;
				    if (next_ohms == best_rank_ohms) { // if same ohms
					if (orankx != rankx) // always skip the other rank(s)
					    continue;
				    }
				    // else next_ohms are greater, always choose it
				}
				// else next_score is less than current best, so always choose it
				debug_print("N%d.LMC%d.R%d: new best score: rank %d, rodt %d(%3d), new best %d, previous best %d(%d)\n",
						node, ddr_interface_num, rankx, orankx, rodt_ctl, next_ohms, next_score,
						best_rank_score, best_rank_ohms);
				best_rank_score	    = next_score;
				best_rank_rtt_nom   = rtt_nom;
				//best_rank_nom_ohms  = rtt_nom_ohms;
				best_rank_ctl       = rodt_ctl;
				best_rank_ohms      = next_ohms;
				best_rankx          = orankx;
				lmc_rlevel_rank.u64 = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].setting;

			    } /* for (int orankx = 0; orankx < dimm_count * 4; orankx++) */
			} /* for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) */
		    } /* for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) */

		    ////////////////// this is the end of the BEST ROW SCORE LOOP

		    // DANGER, Will Robinson!! Abort now if we did not find a best score at all...
		    if (best_rank_score == DEFAULT_BEST_RANK_SCORE) {
			error_print("WARNING: no best rank score found for N%d.LMC%d.R%d - resetting node...\n",
				    node, ddr_interface_num, rankx);
#if defined(BDK) || defined(CVMX_BUILD_FOR_UBOOT)
			cvmx_wait_usec(500000);
			cvmx_reset_octeon_node(node);
			while (1);
#endif
		    }

                    // FIXME: relative now, but still arbitrary...
                    MAX_RANK_SCORE = best_rank_score;
                    if (ddr_type == DDR4_DRAM) {
                        // halve the range if 2 DIMMs unless they are single rank...
                        MAX_RANK_SCORE += (MAX_RANK_SCORE_LIMIT / ((num_ranks > 1) ? dimm_count : 1));
                    } else {
                        // Since DDR3 typically has a wider score range, keep more of them always
                        MAX_RANK_SCORE += MAX_RANK_SCORE_LIMIT;
                    }

		    if (!ecc_ena){
			lmc_rlevel_rank.s.byte8 = lmc_rlevel_rank.s.byte0; /* ECC is not used */
		    }

		    // at the end, write the best row settings to the current rank
		    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), lmc_rlevel_rank.u64);
		    lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

		    saved_rlevel_rank.u64 = lmc_rlevel_rank.u64;

		    ////////////////// this is the start of the PRINT LOOP
		    {
		    int pass;

		    // for pass==0, print current rank, pass==1 print other rank(s)
		    // this is done because we want to show each ranks RODT values together, not interlaced
#if COUNT_RL_CANDIDATES
                    // keep separates for ranks - pass=0 target rank, pass=1 other rank on DIMM
                    int mask_skipped[2] = {0,0};
                    int score_skipped[2] = {0,0};
                    int selected_rows[2] = {0,0};
                    int zero_scores[2] = {0,0};
#endif /* COUNT_RL_CANDIDATES */
		    for (pass = 0; pass < 2; pass++ ) {
			for (orankx = 0; orankx < dimm_count * 4; orankx++) {
			    if (!(dimm_rank_mask & (1 << orankx))) // stay on the same DIMM
				continue;

			    if (((pass == 0) && (orankx != rankx)) || ((pass != 0) && (orankx == rankx)))
				continue;

			    for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) {
				rtt_nom = imp_values->rtt_nom_table[rtt_idx];
				if (dyn_rtt_nom_mask == 0) {
                                    print_nom_ohms = -1;
                                    if (rtt_idx != min_rtt_nom_idx)
                                        continue;
                                } else {
                                    print_nom_ohms = imp_values->rtt_nom_ohms[rtt_nom];
                                }

				// cycle through all the RODT values...
				for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) {
				    cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank;
				    int temp_score = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].score;
				    int skip_row;
				    temp_rlevel_rank.u64 = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].setting;

				    // skip RODT rows in mask, or rows with too high a score;
				    // we will not use them for printing or evaluating...
#if COUNT_RL_CANDIDATES
                                    if ((1 << rodt_ctl) & rodt_row_skip_mask) {
                                        skip_row = WITH_RODT_SKIPPING;
                                        ++mask_skipped[pass];
                                    } else if (temp_score > MAX_RANK_SCORE) {
                                        skip_row = WITH_RODT_SKIPPING;
                                        ++score_skipped[pass];
                                    } else {
                                        skip_row = WITH_RODT_BLANK;
                                        ++selected_rows[pass];
                                        if (temp_score == 0)
                                            ++zero_scores[pass];
                                    }
				
#else /* COUNT_RL_CANDIDATES */
				    skip_row = (((1 << rodt_ctl) & rodt_row_skip_mask) || (temp_score > MAX_RANK_SCORE))
					            ? WITH_RODT_SKIPPING: WITH_RODT_BLANK;
#endif
                                    // identify and print the BEST ROW when it comes up
                                    if ((skip_row == WITH_RODT_BLANK) &&
                                        (best_rankx == orankx) &&
                                        (best_rank_rtt_nom == rtt_nom) &&
                                        (best_rank_ctl == rodt_ctl))
                                    {
                                        skip_row = WITH_RODT_BESTROW;
                                    }

                                    if (ddr_rlevel_printall) {
                                        display_RL_with_RODT(node, ddr_interface_num,
                                                             temp_rlevel_rank, orankx, temp_score,
                                                             print_nom_ohms,
                                                             imp_values->rodt_ohms[rodt_ctl],
                                                             skip_row);
                                    }

				} /* for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) */
			    } /* for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) */
			} /* for (int orankx = 0; orankx < dimm_count * 4; orankx++) { */
		    } /* for (int pass = 0; pass < 2; pass++ ) */
#if COUNT_RL_CANDIDATES
                    ddr_print("N%d.LMC%d.R%d: RLROWS: selected %d+%d, zero_scores %d+%d, mask_skipped %d+%d, score_skipped %d+%d\n",
                           node, ddr_interface_num, rankx,
                           selected_rows[0], selected_rows[1],
                           zero_scores[0], zero_scores[1],
                           mask_skipped[0], mask_skipped[1],
                           score_skipped[0], score_skipped[1]);
#endif /* COUNT_RL_CANDIDATES */
		    }
		    ////////////////// this is the end of the PRINT LOOP

		    // now evaluate which bytes need adjusting
		    // collect the new byte values; first init with current best for neighbor use
		    for (byte_idx = 0, byte_sh = 0; byte_idx < 8+ecc_ena; byte_idx++, byte_sh += 6) {
			rank_best_bytes[byte_idx] = (lmc_rlevel_rank.u64 >> byte_sh) & byte_msk;
		    }

		    ////////////////// this is the start of the BEST BYTE LOOP

		    for (byte_idx = 0, byte_sh = 0; byte_idx < 8+ecc_ena; byte_idx++, byte_sh += 6) {
			int sum = 0, count = 0;
			int count_less = 0, count_same = 0, count_more = 0;
#if FAILSAFE_CHECK
                        uint64_t count_byte; // save the value we counted around
#endif /* FAILSAFE_CHECK */
#if RANK_MAJORITY
			int rank_less = 0, rank_same = 0, rank_more = 0;
#endif /* RANK_MAJORITY */
			int neighbor;
			uint64_t neigh_byte;

			best_byte = orig_best_byte = rank_best_bytes[byte_idx];

			////////////////// this is the start of the BEST BYTE AVERAGING LOOP

			// validate the initial "best" byte by looking at the average of the unskipped byte-column entries
			// we want to do this before we go further, so we can try to start with a better initial value
			// this is the so-called "BESTBUY" patch set

			for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) {
			    rtt_nom = imp_values->rtt_nom_table[rtt_idx];
			    if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
				continue;

			    for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) {
				cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank;
				int temp_score;
				for (orankx = 0; orankx < dimm_count * 4; orankx++) { // average over all the ranks
				    if (!(dimm_rank_mask & (1 << orankx))) // stay on the same DIMM
					continue;
				    temp_score = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].score;
				    // skip RODT rows in mask, or rows with too high a score;
				    // we will not use them for printing or evaluating...

				    if (!((1 << rodt_ctl) & rodt_row_skip_mask) &&
					(temp_score <= MAX_RANK_SCORE))
				    {
					temp_rlevel_rank.u64 = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].setting;
					temp_byte = (temp_rlevel_rank.u64 >> byte_sh) & byte_msk;
					sum += temp_byte;
					count++;
				    }
				} /* for (int orankx = 0; orankx < dimm_count * 4; orankx++) */
			    } /* for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) */
			} /* for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) */

			////////////////// this is the end of the BEST BYTE AVERAGING LOOP


			avg_byte = divide_nint(sum, count); // FIXME: validate count and sum??
			avg_diff = (int)best_byte - (int)avg_byte;
			new_byte = best_byte;
			if (avg_diff != 0) {
			    // bump best up/dn by 1, not necessarily all the way to avg
			    new_byte = best_byte + ((avg_diff > 0) ? -1: 1);
			}

                        if (ddr_rlevel_printall) {
                            ddr_print("N%d.LMC%d.R%d: START:   Byte %d: best %d is different by %d from average %d, using %d.\n",
                                      node, ddr_interface_num, rankx,
                                      byte_idx, (int)best_byte, avg_diff, (int)avg_byte, (int)new_byte);
                        }
			best_byte = new_byte;
#if FAILSAFE_CHECK
                        count_byte = new_byte; // save the value we will count around
#endif /* FAILSAFE_CHECK */

			// At this point best_byte is either:
			// 1. the original byte-column value from the best scoring RODT row, OR
			// 2. that value bumped toward the average of all the byte-column values
			//
			// best_byte will not change from here on...

			////////////////// this is the start of the BEST BYTE COUNTING LOOP

			// NOTE: we do this next loop separately from above, because we count relative to "best_byte"
			// which may have been modified by the above averaging operation...

			for (rtt_idx = min_rtt_nom_idx; rtt_idx <= max_rtt_nom_idx; ++rtt_idx) {
			    rtt_nom = imp_values->rtt_nom_table[rtt_idx];
			    if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
				continue;

			    for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) {
				cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank;
				int temp_score;
				for (orankx = 0; orankx < dimm_count * 4; orankx++) { // count over all the ranks
				    if (!(dimm_rank_mask & (1 << orankx))) // stay on the same DIMM
					continue;
				    temp_score = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].score;
				    // skip RODT rows in mask, or rows with too high a score;
				    // we will not use them for printing or evaluating...
				    if (((1 << rodt_ctl) & rodt_row_skip_mask) ||
					(temp_score > MAX_RANK_SCORE))
				    {
					continue;
				    }
				    temp_rlevel_rank.u64 = rlevel_scoreboard[rtt_nom][rodt_ctl][orankx].setting;
				    temp_byte = (temp_rlevel_rank.u64 >> byte_sh) & byte_msk;

				    if (temp_byte == 0) // do not count it if illegal
					;//continue;
				    else if (temp_byte == best_byte)
					count_same++;
				    else if (temp_byte == best_byte - 1)
					count_less++;
				    else if (temp_byte == best_byte + 1)
					count_more++;
				    // else do not count anything more than 1 away from the best
#if RANK_MAJORITY
				    // FIXME? count is relative to best_byte; should it be rank-based?
				    if (orankx != rankx) // rank counts only on main rank
					continue;
				    else if (temp_byte == best_byte)
					rank_same++;
				    else if (temp_byte == best_byte - 1)
					rank_less++;
				    else if (temp_byte == best_byte + 1)
					rank_more++;
#endif /* RANK_MAJORITY */
				} /* for (int orankx = 0; orankx < dimm_count * 4; orankx++) */
			    } /* for (rodt_ctl = max_rodt_ctl; rodt_ctl >= min_rodt_ctl; --rodt_ctl) */
			} /* for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) */

                        if (ddr_rlevel_printall) {
#if RANK_MAJORITY
                            ddr_print("N%d.LMC%d.R%d: COUNT:   Byte %d: orig %d now %d, more %d same %d less %d (%d/%d/%d)\n",
                                      node, ddr_interface_num, rankx,
                                      byte_idx, (int)orig_best_byte, (int)best_byte,
                                      count_more, count_same, count_less,
                                      rank_more, rank_same, rank_less);
#else /* RANK_MAJORITY */
                            ddr_print("N%d.LMC%d.R%d: COUNT:   Byte %d: orig %d now %d, more %d same %d less %d\n",
                                      node, ddr_interface_num, rankx,
                                      byte_idx, (int)orig_best_byte, (int)best_byte,
                                      count_more, count_same, count_less);
#endif /* RANK_MAJORITY */
                        }

			////////////////// this is the end of the BEST BYTE COUNTING LOOP

			// choose the new byte value
			// we need to check that there is no gap greater than 2 between adjacent bytes
			//  (adjacency depends on DIMM type)
			// use the neighbor value to help decide
			// initially, the rank_best_bytes[] will contain values from the chosen lowest score rank
			new_byte = 0;

			// neighbor is index-1 unless we are index 0 or index 8 (ECC)
			neighbor = (byte_idx == 8) ? 3 : ((byte_idx == 0) ? 1 : byte_idx - 1);
			neigh_byte = rank_best_bytes[neighbor];


			// can go up or down or stay the same, so look at a numeric average to help
			new_byte = divide_nint(((count_more * (best_byte + 1)) +
						(count_same * (best_byte + 0)) +
						(count_less * (best_byte - 1))),
					       max(1, (count_more + count_same + count_less)));

			// use neighbor to help choose with average
			if ((byte_idx > 0) && (_abs(neigh_byte - new_byte) > 2) && !disable_sequential_delay_check) // but not for byte 0
			{
			    uint64_t avg_pick = new_byte;
			    if ((new_byte - best_byte) != 0)
				new_byte = best_byte; // back to best, average did not get better
			    else // avg was the same, still too far, now move it towards the neighbor
				new_byte += (neigh_byte > new_byte) ? 1 : -1;

                            if (ddr_rlevel_printall) {
                                ddr_print("N%d.LMC%d.R%d: AVERAGE: Byte %d: neighbor %d too different %d from average %d, picking %d.\n",
                                          node, ddr_interface_num, rankx,
                                          byte_idx, neighbor, (int)neigh_byte, (int)avg_pick, (int)new_byte);
                            }
			}
#if MAJORITY_OVER_AVG
			// NOTE:
			// For now, we let the neighbor processing above trump the new simple majority processing here.
			// This is mostly because we have seen no smoking gun for a neighbor bad choice (yet?).
			// Also note that we will ALWAYS be using byte 0 majority, because of the if clause above.
			else {
			    // majority is dependent on the counts, which are relative to best_byte, so start there
			    uint64_t maj_byte = best_byte;
#if RANK_MAJORITY
			    uint64_t rank_maj;
			    int rank_sum;
#endif
			    if ((count_more > count_same) && (count_more > count_less)) {
				maj_byte++;
			    } else if ((count_less > count_same) && (count_less > count_more)) {
				maj_byte--;
			    }
			    if (maj_byte != new_byte) {
				// print only when majority choice is different from average
                                if (ddr_rlevel_printall) {
                                    ddr_print("N%d.LMC%d.R%d: MAJORTY: Byte %d: picking majority of %d over average %d.\n",
                                              node, ddr_interface_num, rankx,
                                              byte_idx, (int)maj_byte, (int)new_byte);
                                }
				new_byte = maj_byte;
			    } else {
                                if (ddr_rlevel_printall) {
                                    ddr_print("N%d.LMC%d.R%d: AVERAGE: Byte %d: picking average of %d.\n",
                                              node, ddr_interface_num, rankx,
                                              byte_idx, (int)new_byte);
                                }
			    }
#if RANK_MAJORITY
			    // rank majority is dependent on the rank counts, which are relative to best_byte,
			    // so start there, and adjust according to the rank counts majority
			    rank_maj = best_byte;
			    if ((rank_more > rank_same) && (rank_more > rank_less)) {
				rank_maj++;
			    } else if ((rank_less > rank_same) && (rank_less > rank_more)) {
				rank_maj--;
			    }
			    rank_sum = rank_more + rank_same + rank_less;

			    // now, let rank majority possibly rule over the current new_byte however we got it
			    if (rank_maj != new_byte) { // only if different
				// Here is where we decide whether to completely apply RANK_MAJORITY or not
                                if (rank_maj < new_byte) { // ignore if less than
				    // print only when rank majority choice is selected
                                    if (ddr_rlevel_printall) {
                                        ddr_print("N%d.LMC%d.R%d: RANKMAJ: Byte %d: LESS: NOT using %d over %d.\n",
                                                  node, ddr_interface_num, rankx,
                                                  byte_idx, (int)rank_maj, (int)new_byte);
                                    }
                                } else 
				// For the moment, we do it ONLY when running 2-slot configs
				//  OR when rank_sum is big enough
				if ((dimm_count > 1) || (rank_sum > 2)) {
				    // print only when rank majority choice is selected
                                    if (ddr_rlevel_printall) {
                                        ddr_print("N%d.LMC%d.R%d: RANKMAJ: Byte %d: picking %d over %d.\n",
                                                  node, ddr_interface_num, rankx,
                                                  byte_idx, (int)rank_maj, (int)new_byte);
                                    }
				    new_byte = rank_maj;
				} else { // FIXME: print some info when we could have chosen RANKMAJ but did not
                                    if (ddr_rlevel_printall) {
                                        ddr_print("N%d.LMC%d.R%d: RANKMAJ: Byte %d: NOT using %d over %d (best=%d,sum=%d).\n",
                                                  node, ddr_interface_num, rankx,
                                                  byte_idx, (int)rank_maj, (int)new_byte,
                                                  (int)best_byte, rank_sum);
                                    }
				}
			    }
#endif /* RANK_MAJORITY */
			}
#else
			else {
                            if (ddr_rlevel_printall) {
                                ddr_print("N%d.LMC%d.R%d: AVERAGE: Byte %d: picking average of %d.\n",
                                          node, ddr_interface_num, rankx,
                                          byte_idx, (int)new_byte);
                            }
			}
#endif
#if FAILSAFE_CHECK
                        // one last check:
                        // if new_byte is still count_byte, BUT there was no count for that value, DO SOMETHING!!!
                        // FIXME: go back to original best byte from the best row
                        if ((new_byte == count_byte) && (count_same == 0)) {
                            new_byte = orig_best_byte;
                            if (ddr_rlevel_printall) {
                                ddr_print("N%d.LMC%d.R%d: FAILSAF: Byte %d: going back to original %d.\n",
                                          node, ddr_interface_num, rankx,
                                          byte_idx, (int)new_byte);
                            }
                        }
#endif /* FAILSAFE_CHECK */
#if PERFECT_BITMASK_COUNTING
                        // Look at counts for "perfect" bitmasks if we had any for this byte-lane.
                        // Remember, we only counted for DDR4, so zero means none or DDR3, and we bypass this...
                        if (rank_perfect_counts[rankx].total[byte_idx] > 0) {
                            // FIXME: should be more error checking, look for ties, etc...
                            int i, delay_count, delay_value, delay_max; 
                            uint32_t ties;
                            delay_value = -1;
                            delay_max = 0;
                            ties = 0;

                            for (i = 0; i < 32; i++) {
                                delay_count = rank_perfect_counts[rankx].count[byte_idx][i];
                                if (delay_count > 0) { // only look closer if there are any,,,
                                    if (delay_count > delay_max) {
                                        delay_max = delay_count;
                                        delay_value = i;
                                        ties = 0; // reset ties to none
                                    } else if (delay_count == delay_max) {
                                        if (ties == 0)
                                            ties = 1UL << delay_value; // put in original value
                                        ties |= 1UL << i; // add new value
                                    }
                                }
                            } /* for (i = 0; i < 32; i++) */

                            if (delay_value >= 0) {
                                if (ties != 0) {
                                    if (ties & (1UL << (int)new_byte)) {
                                        // leave choice as new_byte if any tied one is the same... 
                                        delay_value = (int)new_byte;
                                        if (ddr_rlevel_printall) {
                                            ddr_print("N%d.LMC%d.R%d: PERFECT: Byte %d: TIES (0x%lx) INCLUDED %d (%d)\n",
                                                      node, ddr_interface_num, rankx, byte_idx, ties,
                                                      (int)new_byte, delay_max);
                                        }
                                    } else {
                                        // FIXME: should choose a perfect one!!!
                                        // FIXME: for now, leave the choice as new_byte
                                        delay_value = (int)new_byte;
                                        if (ddr_rlevel_printall) {
                                            ddr_print("N%d.LMC%d.R%d: PERFECT: Byte %d: TIES (0x%lx) OMITTED %d (%d)\n",
                                                      node, ddr_interface_num, rankx, byte_idx, ties,
                                                      (int)new_byte, delay_max);
                                        }
                                    }
                                } /* if (ties != 0) */

                                if (delay_value != (int)new_byte) {
                                    delay_count = rank_perfect_counts[rankx].count[byte_idx][(int)new_byte];
                                    if (ddr_rlevel_printall) {
                                        ddr_print("N%d.LMC%d.R%d: PERFECT: Byte %d: DIFF from %d (%d), USING %d (%d)\n",
                                                  node, ddr_interface_num, rankx, byte_idx, (int)new_byte,
                                                  delay_count, delay_value, delay_max);
                                    }
                                    new_byte = (uint64_t)delay_value; // FIXME: make this optional via envvar?
                                } else {
                                    debug_print("N%d.LMC%d.R%d: PERFECT: Byte %d: SAME as %lld (%d)\n",
                                                node, ddr_interface_num, rankx, byte_idx, new_byte, delay_max);
                                }
                            }
                        } /* if (rank_perfect_counts[rankx].total[byte_idx] > 0) */
                        else {
                            if (ddr_type == DDR4_DRAM) { // only report when DDR4
                                // FIXME: remove or increase VBL for this output...
                                if (ddr_rlevel_printall) {
                                    ddr_print("N%d.LMC%d.R%d: PERFECT: Byte %d: ZERO perfect bitmasks\n",
                                              node, ddr_interface_num, rankx, byte_idx);
                                }
                            }
                        } /* if (rank_perfect_counts[rankx].total[byte_idx] > 0) */
#endif /* PERFECT_BITMASK_COUNTING */

                        if (ddr_rlevel_printall) {
                            ddr_print("N%d.LMC%d.R%d: SUMMARY: Byte %d: %s: orig %d now %d, more %d same %d less %d, using %d\n",
                                      node, ddr_interface_num, rankx,
                                      byte_idx, "AVG", (int)orig_best_byte,
                                      (int)best_byte, count_more, count_same, count_less, (int)new_byte);
                        }

			// update the byte with the new value (NOTE: orig value in the CSR may not be current "best")
			lmc_rlevel_rank.u64 &= ~(byte_msk << byte_sh);
			lmc_rlevel_rank.u64 |= (new_byte << byte_sh);

			rank_best_bytes[byte_idx] = new_byte; // save new best for neighbor use

		    } /* for (byte_idx = 0; byte_idx < 8+ecc_ena; byte_idx++) */

		    ////////////////// this is the end of the BEST BYTE LOOP

		    if (saved_rlevel_rank.u64 != lmc_rlevel_rank.u64) {
			ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num), lmc_rlevel_rank.u64);
			lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
			debug_print("Adjusting Read-Leveling per-RANK settings.\n");
		    } else {
			debug_print("Not Adjusting Read-Leveling per-RANK settings.\n");
		    }
		    display_RL_with_final(node, ddr_interface_num, lmc_rlevel_rank, rankx);

#if RLEXTRAS_PATCH
#define RLEVEL_RANKX_EXTRAS_INCR  4
		    if ((rank_mask & 0x0F) != 0x0F) { // if there are unused entries to be filled		    
			cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank = lmc_rlevel_rank; // copy the current rank
			int byte, delay;
			if (rankx < 3) {
			    debug_print("N%d.LMC%d.R%d: checking for RLEVEL_RANK unused entries.\n",
				      node, ddr_interface_num, rankx);
			    for (byte = 0; byte < 9; byte++) { // modify the copy in prep for writing to empty slot(s)
				delay = get_rlevel_rank_struct(&temp_rlevel_rank, byte) + RLEVEL_RANKX_EXTRAS_INCR;
				if (delay > (int)RLEVEL_BYTE_MSK) delay = RLEVEL_BYTE_MSK;
				update_rlevel_rank_struct(&temp_rlevel_rank, byte, delay);
			    }
			    if (rankx == 0) { // if rank 0, write rank 1 and rank 2 here if empty
				if (!(rank_mask & (1<<1))) { // check that rank 1 is empty
				    debug_print("N%d.LMC%d.R%d: writing RLEVEL_RANK unused entry R%d.\n",
                                              node, ddr_interface_num, rankx, 1);
				    //DRAM_CSR_WRITE(node, BDK_LMCX_RLEVEL_RANKX(ddr_interface_num, 1), temp_rlevel_rank.u);
                                    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(1, ddr_interface_num), temp_rlevel_rank.u64);
				}
				if (!(rank_mask & (1<<2))) { // check that rank 2 is empty
				    debug_print("N%d.LMC%d.R%d: writing RLEVEL_RANK unused entry R%d.\n",
                                              node, ddr_interface_num, rankx, 2);
				    //DRAM_CSR_WRITE(node, BDK_LMCX_RLEVEL_RANKX(ddr_interface_num, 2), temp_rlevel_rank.u);
                                    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(2, ddr_interface_num), temp_rlevel_rank.u64);
				}
			    }
			    // if ranks 0, 1 or 2, write rank 3 here if empty
			    if (!(rank_mask & (1<<3))) { // check that rank 3 is empty
				debug_print("N%d.LMC%d.R%d: writing RLEVEL_RANK unused entry R%d.\n",
					  node, ddr_interface_num, rankx, 3);
				//DRAM_CSR_WRITE(node, BDK_LMCX_RLEVEL_RANKX(ddr_interface_num, 3), temp_rlevel_rank.u);
                                ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(3, ddr_interface_num), temp_rlevel_rank.u64);
			    }
			}
		    }
#endif /* RLEXTRAS_PATCH */
#if 0  /* RLEXTRAS_PATCH */
		    {
#define RLEVEL_RANKX_EXTRAS_INCR  4
			cvmx_lmcx_rlevel_rankx_t temp_rlevel_rank = lmc_rlevel_rank;
			int byte, delay;
			if (rankx < 3) {
			    ddr_print("N%d.LMC%d.R%d: writing RLEVEL_RANK unused entries.\n",
					    node, ddr_interface_num, rankx);
			    for (byte = 0; byte < 9; byte++) {
				delay = get_rlevel_rank_struct(&temp_rlevel_rank, byte) + RLEVEL_RANKX_EXTRAS_INCR;
				if (delay > (int)RLEVEL_BYTE_MSK) delay = RLEVEL_BYTE_MSK;
				update_rlevel_rank_struct(&temp_rlevel_rank, byte, delay);
			    }
			    if (rankx == 0) { // if 0, write 1, and 2
				ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(1, ddr_interface_num), temp_rlevel_rank.u64);
				ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(2, ddr_interface_num), temp_rlevel_rank.u64);
			    }
			    // if 0, 1 or 2, write 3
			    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(3, ddr_interface_num), temp_rlevel_rank.u64);
			}
		    }
#endif /* RLEXTRAS_PATCH */
		} /* for (rankx = 0; rankx < dimm_count * 4; rankx++) */

		////////////////// this is the end of the RANK MAJOR LOOP

	    }  /* Evaluation block */
        } /* while(rlevel_debug_loops--) */

        lmc_control.cn78xx.ddr2t           = save_ddr2t;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);
        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
        ddr_print("DDR2T                                         : %6d\n", lmc_control.cn78xx.ddr2t); /* Display final 2T value */



        perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

            for (i=0; i<9; ++i) {
                if ((s = lookup_env_parameter("ddr%d_rlevel_rank%d_byte%d", ddr_interface_num, rankx, i)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);

                    update_rlevel_rank_struct(&lmc_rlevel_rank, i, value);
                }
            }

            if ((s = lookup_env_parameter_ull("ddr%d_rlevel_rank%d", ddr_interface_num, rankx)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                lmc_rlevel_rank.u64 = value;
            }

            if (parameter_set) {
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num),
					  lmc_rlevel_rank.u64);
                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                display_RL(node, ddr_interface_num, lmc_rlevel_rank, rankx);
            }
        }
    }

    /* Workaround Trcd overflow by using Additive latency. */
    if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) {
        cvmx_lmcx_modereg_params0_t lmc_modereg_params0;
        cvmx_lmcx_timing_params1_t lmc_timing_params1;
        cvmx_lmcx_control_t lmc_control;
        int rankx;

        lmc_timing_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));
        lmc_modereg_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));
        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));

        if (lmc_timing_params1.cn78xx.trcd == 0) {
            ddr_print("Workaround Trcd overflow by using Additive latency.\n");
            lmc_timing_params1.cn78xx.trcd = 12; /* Hard code this to 12 and enable additive latency */
            lmc_modereg_params0.s.al      = 2; /* CL-2 */
            lmc_control.s.pocas           = 1;

            ddr_print("MODEREG_PARAMS0                               : 0x%016llx\n",
                      lmc_modereg_params0.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                      lmc_modereg_params0.u64);
            ddr_print("TIMING_PARAMS1                                : 0x%016llx\n", lmc_timing_params1.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num),
                                      lmc_timing_params1.u64);

            ddr_print("LMC_CONTROL                                   : 0x%016llx\n", lmc_control.u64);
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_CONTROL(ddr_interface_num),
                                      lmc_control.u64);

            for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;

                ddr4_mrw(cpu_id, node, ddr_interface_num, rankx, -1, 1, 0); /* MR1 */
            }

        }
    }

    // this is here just for output, to allow check of the Deskew settings one last time...
    if (1/*! disable_deskew_training*/) { // FIXME: always print here?
        deskew_counts_t dsk_counts;
	ddr_print("N%d.LMC%d: Check Deskew Settings before software Write-Leveling.\n",
                  node, ddr_interface_num);
        Validate_Deskew_Training(cpu_id, node, rank_mask, ddr_interface_num, &dsk_counts, 3);
    }

    /* Workaround Errata 26304 (T88@2.0, O75@1.x, O78@2.x)

       When the CSRs LMCX_DLL_CTL3[WR_DESKEW_ENA] = 1 AND
       LMCX_PHY_CTL2[DQS[0..8]_DSK_ADJ] > 4, set
       LMCX_EXT_CONFIG[DRIVE_ENA_BPRCH] = 1.
    */
    if (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X) || octeon_is_cpuid(OCTEON_CNF75XX_PASS1_X)) {
        cvmx_lmcx_dll_ctl3_t dll_ctl3;
        cvmx_lmcx_phy_ctl2_t phy_ctl2;
        cvmx_lmcx_ext_config_t ext_config;
        int increased_dsk_adj = 0;
        int byte;

        phy_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL2(ddr_interface_num));
        ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
        dll_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL3(ddr_interface_num));

        for (byte = 0; byte < 8; ++byte) {
            if (!(ddr_interface_bytemask & (1 << byte)))
                continue;
            increased_dsk_adj |= (((phy_ctl2.u64 >> (byte * 3)) & 0x7) > 4);
        }

        if ((dll_ctl3.s.wr_deskew_ena == 1) && increased_dsk_adj) {
            ext_config.s.drive_ena_bprch = 1;
            ddr_config_write_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
                                      ext_config.u64);
            ddr_print("LMC%d: Forcing DRIVE_ENA_BPRCH for Workaround Errata 26304.\n", ddr_interface_num);
        }
    }

    { // Software Write-Leveling block

        /* Try to determine/optimize write-level delays experimentally. */
#pragma pack(push,1)
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank_hw_results;
        cvmx_lmcx_config_t lmc_config;
        int byte;
        int delay;
        int rankx = 0;
        int active_rank;
        int sw_wlevel_enable = 1; /* FIX... Should be customizable. */
        int interfaces;
	int measured_vref_flag;
        typedef enum {
            WL_ESTIMATED = 0,   /* HW/SW wleveling failed. Results
                                   estimated. */
            WL_HARDWARE  = 1,   /* H/W wleveling succeeded */
            WL_SOFTWARE  = 2,   /* S/W wleveling passed 2 contiguous
                                   settings. */
            WL_SOFTWARE1 = 3,   /* S/W wleveling passed 1 marginal
                                   setting. */
        } sw_wl_status_t;

        static const char *wl_status_strings[] = {
            "(e)",
            "   ",
            "   ",
            "(1)"
        };
        // FIXME: make HW-assist the default now?
        int sw_wlevel_hw_default = (! octeon_is_cpuid(OCTEON_CN78XX_PASS1_X)) ? 1 : 0;
#pragma pack(pop)

        if ((s = lookup_env_parameter("ddr_sw_wlevel_hw")) != NULL) {
            sw_wlevel_hw_default = !!strtoul(s, NULL, 0);
        }
        if (!ddr_interface_64b) // must use SW algo if 32-bit mode
            sw_wlevel_hw_default = 0;

        if ((s = lookup_env_parameter("ddr_software_wlevel")) != NULL) {
            sw_wlevel_enable = strtoul(s, NULL, 0);
        }

        cvmx_rng_enable();

#if SWL_WITH_HW_ALTS_CHOOSE_SW
        // Choose the SW algo for SWL if any HWL alternates were found
        // NOTE: we have to do this here, and for all, since HW-assist including ECC requires ECC enable
        for (rankx = 0; rankx < dimm_count * 4; rankx++) {
            if (!sw_wlevel_enable)
                break;
            if (!(rank_mask & (1 << rankx)))
                continue;

            // if we are doing HW-assist, and there are alternates, switch to SW-algorithm for all
            if (sw_wlevel_hw_default && hwl_alts[rankx].hwl_alt_mask) {
                ddr_print("N%d.LMC%d.R%d: Using SW algorithm for write-leveling this rank\n",
                          node, ddr_interface_num, rankx);
                sw_wlevel_hw_default = 0;
                break;
            }
        } /* for (rankx = 0; rankx < dimm_count * 4; rankx++) */
#endif

	/* Get the measured_vref setting from the config, check for an override... */
	/* NOTE: measured_vref=1 (ON) means force use of MEASURED Vref... */
        // NOTE: measured VREF can only be done for DDR4 
        if (ddr_type == DDR4_DRAM) {
            measured_vref_flag = custom_lmc_config->measured_vref;
            if ((s = lookup_env_parameter("ddr_measured_vref")) != NULL) {
                measured_vref_flag = !!strtoul(s, NULL, 0);
            }
        } else {
            measured_vref_flag = 0; // OFF for DDR3
        }

        /* Ensure disabled ECC for DRAM tests using the SW algo, else leave it untouched */
        if (!sw_wlevel_hw_default) {
            lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
            lmc_config.cn78xx.ecc_ena = 0;
            ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
        }

#ifndef __U_BOOT__
        limit_l2_ways(0, 0);       /* Disable l2 sets for DRAM testing */
#endif

        /* We need to track absolute rank number, as well as how many
        ** active ranks we have.  Two single rank DIMMs show up as
        ** ranks 0 and 2, but only 2 ranks are active. */
        active_rank = 0;

        interfaces = __builtin_popcount(ddr_interface_mask);

#define VREF_RANGE1_LIMIT 0x33 // range1 is valid for 0x00 - 0x32
#define VREF_RANGE2_LIMIT 0x18 // range2 is valid for 0x00 - 0x17
// full window is valid for 0x00 to 0x4A
// let 0x00 - 0x17 be range2, 0x18 - 0x4a be range 1
#define VREF_LIMIT        (VREF_RANGE1_LIMIT + VREF_RANGE2_LIMIT)
#define VREF_FINAL        (VREF_LIMIT - 1)

        for (rankx = 0; rankx < dimm_count * 4; rankx++) {
            uint64_t rank_addr;
            int vref_value, final_vref_value, final_vref_range = 0;
	    int start_vref_value = 0, computed_final_vref_value = -1;
            char best_vref_values_count, vref_values_count;
            char best_vref_values_start, vref_values_start;

            int bytes_failed;
            sw_wl_status_t byte_test_status[9];
            sw_wl_status_t sw_wl_rank_status = WL_HARDWARE;
	    int sw_wl_failed = 0;
            int sw_wlevel_hw = sw_wlevel_hw_default;

            if (!sw_wlevel_enable)
                break;

            if (!(rank_mask & (1 << rankx)))
                continue;

            ddr_print("N%d.LMC%d.R%d: Performing Software Write-Leveling %s\n",
                      node, ddr_interface_num, rankx,
                      (sw_wlevel_hw) ? "with H/W assist" : "with S/W algorithm");

	    if ((ddr_type == DDR4_DRAM) && (num_ranks != 4)) {
		// always compute when we can...
		computed_final_vref_value = compute_vref_value(node, ddr_interface_num, rankx,
							       dimm_count, num_ranks, imp_values,
                                                               is_stacked_die, cpu_id);
		if (!measured_vref_flag) // but only use it if allowed
		    start_vref_value = VREF_FINAL; // skip all the measured Vref processing, just the final setting
	    }

            /* Save off the h/w wl results */
            lmc_wlevel_rank_hw_results.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            vref_values_count = 0;
            vref_values_start = 0;
            best_vref_values_count = 0;
            best_vref_values_start = 0;

            /* Loop one extra time using the Final Vref value. */
            for (vref_value = start_vref_value; vref_value < VREF_LIMIT; ++vref_value) {
                if (ddr_type == DDR4_DRAM) {
                    if (vref_value < VREF_FINAL) {
                        int vrange, vvalue;
                        if (vref_value < VREF_RANGE2_LIMIT) {
                            vrange = 1; vvalue = vref_value;
                        } else {
                            vrange = 0; vvalue = vref_value - VREF_RANGE2_LIMIT;
                        }
		        set_vref(cpu_id, node, ddr_interface_num, rankx,
                                 vrange, vvalue);
                    } else { /* if (vref_value < VREF_FINAL) */
                        /* Print the final Vref value first. */

			/* Always print the computed first if its valid */
			if (computed_final_vref_value >= 0) {
			    ddr_print("N%d.LMC%d.R%d: Vref Computed Summary                 :"
				      "              %2d (0x%02x)\n",
				      node, ddr_interface_num,
				      rankx, computed_final_vref_value,
				      computed_final_vref_value);
			} 
			if (!measured_vref_flag) { // setup to use the computed
			    best_vref_values_count = 1;
			    final_vref_value = computed_final_vref_value;
			} else { // setup to use the measured
			    if (best_vref_values_count > 0) {
				best_vref_values_count = max(best_vref_values_count, 2);
#if 0
                                // NOTE: this already adjusts VREF via calculating 40% rather than 50%
				final_vref_value = best_vref_values_start + divide_roundup((best_vref_values_count-1)*4,10);

				ddr_print("N%d.LMC%d.R%d: Vref Training Summary                 :"
					  "    %2d <----- %2d (0x%02x) -----> %2d range: %2d\n",
					  node, ddr_interface_num, rankx, best_vref_values_start,
					  final_vref_value, final_vref_value,
					  best_vref_values_start+best_vref_values_count-1,
					  best_vref_values_count-1);
#else
				final_vref_value = best_vref_values_start + divide_nint(best_vref_values_count - 1, 2);
                                if (final_vref_value < VREF_RANGE2_LIMIT) {
                                    final_vref_range = 1;
                                } else {
                                    final_vref_range = 0; final_vref_value -= VREF_RANGE2_LIMIT;
                                }
                                {
                                    int vvlo = best_vref_values_start;
                                    int vrlo;
                                    int vvhi = best_vref_values_start + best_vref_values_count - 1;
                                    int vrhi;
                                    if (vvlo < VREF_RANGE2_LIMIT) {
                                        vrlo = 2;
                                    } else {
                                        vrlo = 1; vvlo -= VREF_RANGE2_LIMIT;
                                    }
                                    
                                    if (vvhi < VREF_RANGE2_LIMIT) {
                                        vrhi = 2;
                                    } else {
                                        vrhi = 1; vvhi -= VREF_RANGE2_LIMIT;
                                    }
                                    ddr_print("N%d.LMC%d.R%d: Vref Training Summary                 :"
                                              "  0x%02x/%1d <----- 0x%02x/%1d -----> 0x%02x/%1d, range: %2d\n",
                                              node, ddr_interface_num, rankx,
                                              vvlo, vrlo,
                                              final_vref_value, final_vref_range + 1,
                                              vvhi, vrhi,
                                              best_vref_values_count-1);
                                }
#endif

			    } else {
				/* If nothing passed use the default Vref value for this rank */
				cvmx_lmcx_modereg_params2_t lmc_modereg_params2;
				lmc_modereg_params2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS2(ddr_interface_num));
				final_vref_value = (lmc_modereg_params2.u64 >> (rankx * 10 + 3)) & 0x3f;
				final_vref_range = (lmc_modereg_params2.u64 >> (rankx * 10 + 9)) & 0x01;

				ddr_print("N%d.LMC%d.R%d: Vref Using Default                    :"
					  "    %2d <----- %2d (0x%02x) -----> %2d, range%1d\n",
					  node, ddr_interface_num, rankx,
                                          final_vref_value, final_vref_value, 
					  final_vref_value, final_vref_value, final_vref_range+1);
			    }
			}

			// allow override
                        if ((s = lookup_env_parameter("ddr%d_vref_value_%1d%1d",
                                                      ddr_interface_num, !!(rankx&2), !!(rankx&1))) != NULL) {
                            final_vref_value = strtoul(s, NULL, 0);
                        }

			set_vref(cpu_id, node, ddr_interface_num, rankx, final_vref_range, final_vref_value);

		    } /* if (vref_value < VREF_FINAL) */
                } /* if (ddr_type == DDR4_DRAM) */

                lmc_wlevel_rank.u64 = lmc_wlevel_rank_hw_results.u64; /* Restore the saved value */

		for (byte = 0; byte < 9; ++byte)
		    byte_test_status[byte] = WL_ESTIMATED;

                if (wlevel_bitmask_errors == 0) {

                    /* Determine address of DRAM to test for pass 1 of software write leveling. */
                    rank_addr  = active_rank * (1ull << (pbank_lsb - bunk_enable + (interfaces/2)));
                    // FIXME: these now put in by test_dram_byte()
                    //rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */
                    //rank_addr = cvmx_numa_get_address(node, rank_addr);
		    //rank_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;

		    /* Adjust address for boot bus hole in memory map. */
		    if (rank_addr > 0x10000000)
			rank_addr += 0x10000000;

                    debug_print("N%d.LMC%d.R%d: Active Rank %d Address: 0x%llx\n",
                                node, ddr_interface_num, rankx, active_rank, rank_addr);

		    { // start parallel write-leveling block for delay high-order bits
			int errors = 0;
			int byte_delay[9];
			uint64_t bytemask;
			int bytes_todo;
#define WL_MIN_NO_ERRORS_COUNT 3  // FIXME? three passes without errors
			int no_errors_count = 0;
                        uint64_t bad_bits[2];
#if ENABLE_SW_WLEVEL_UTILIZATION
                        uint64_t sum_dram_dclk = 0, sum_dram_ops = 0;
                        uint64_t start_dram_dclk, stop_dram_dclk;
                        uint64_t start_dram_ops, stop_dram_ops;
#endif

                        if (ddr_interface_64b) {
                            bytes_todo = (sw_wlevel_hw) ? ddr_interface_bytemask : 0xFF;
                            bytemask = ~0ULL;
                        } else { // 32-bit, must be using SW algo, only data bytes
                            bytes_todo = 0x0f;
                            bytemask = 0x00000000ffffffffULL;
                        }

			for (byte = 0; byte < 9; ++byte) {
			    if (!(bytes_todo & (1 << byte))) {
				byte_delay[byte] = 0;
			    } else {
				byte_delay[byte] = get_wlevel_rank_struct(&lmc_wlevel_rank, byte);
			    }
			} /* for (byte = 0; byte < 9; ++byte) */

			do {
			    // write the current set of WL delays
			    ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
                                                      lmc_wlevel_rank.u64);
			    lmc_wlevel_rank.u64 =
                                cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                            //CVMX_CSR_WRITE(node, CVMX_GTI_CWD_POKEX(cvmx_get_core_num()), 0);

			    // do the test
                            if (sw_wlevel_hw) {
                                errors = run_best_hw_patterns(cpu_id, node, ddr_interface_num, rank_addr,
                                                              DBTRAIN_TEST, bad_bits);
                                errors &= bytes_todo; // keep only the ones we are still doing
                            } else {
#if ENABLE_SW_WLEVEL_UTILIZATION
                                start_dram_dclk = cvmx_read_csr_node(node, CVMX_LMCX_DCLK_CNT(ddr_interface_num));
                                start_dram_ops  = cvmx_read_csr_node(node, CVMX_LMCX_OPS_CNT(ddr_interface_num));
#endif
                                errors = test_dram_byte64(cpu_id, node, ddr_interface_num,
                                                          rank_addr, bytemask, bad_bits);

#if ENABLE_SW_WLEVEL_UTILIZATION
                                stop_dram_dclk = cvmx_read_csr_node(node, CVMX_LMCX_DCLK_CNT(ddr_interface_num));
                                stop_dram_ops  = cvmx_read_csr_node(node, CVMX_LMCX_OPS_CNT(ddr_interface_num));
                                sum_dram_dclk += stop_dram_dclk - start_dram_dclk;
                                sum_dram_ops  += stop_dram_ops  - start_dram_ops;
#endif
                            }

			    debug_print("WL pass1: test_dram_byte returned 0x%x\n", errors);

			    // remember, errors will not be returned for byte-lanes that have maxxed out...
                            if (errors == 0) {
                                no_errors_count++; // bump
                                if (no_errors_count > 1) // bypass check/update completely
                                    continue; // to end of do-while
                            } else
                                no_errors_count = 0; // reset
			    
			    // check errors by byte
			    for (byte = 0; byte < 9; ++byte) {
				if (!(bytes_todo & (1 << byte)))
				    continue;

				delay = byte_delay[byte];
				if (errors & (1 << byte)) { // yes, an error in this byte lane
				    debug_print("        byte %d delay %2d Errors\n", byte, delay);
				    // since this byte had an error, we move to the next delay value, unless done with it
				    delay += 8; // incr by 8 to do delay high-order bits
				    if (delay < 32) {
					update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
					debug_print("        byte %d delay %2d New\n", byte, delay);
					byte_delay[byte] = delay;
				    } else { // reached max delay, maybe really done with this byte
#if SWL_TRY_HWL_ALT
					if (!measured_vref_flag && // consider an alt only for computed VREF and
                                            (hwl_alts[rankx].hwl_alt_mask & (1 << byte))) // if an alt exists...
                                        {
                                            int bad_delay = delay & 0x6; // just orig low-3 bits
					    delay = hwl_alts[rankx].hwl_alt_delay[byte]; // yes, use it
					    hwl_alts[rankx].hwl_alt_mask &= ~(1 << byte); // clear that flag
					    update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
					    byte_delay[byte] = delay;
					    debug_print("        byte %d delay %2d ALTERNATE\n", byte, delay);
					    ddr_print("N%d.LMC%d.R%d: SWL: Byte %d: %d FAIL, trying ALTERNATE %d\n",
						      node, ddr_interface_num, rankx, byte, bad_delay, delay);

					} else
#endif /* SWL_TRY_HWL_ALT */
                                        {
                                            unsigned bits_bad;
					    if (byte < 8) {
                                                bytemask &= ~(0xffULL << (8*byte)); // test no longer, remove from byte mask
                                                bits_bad = (unsigned)((bad_bits[0] >> (8 * byte)) & 0xffUL);
                                            } else {
                                                bits_bad = (unsigned)(bad_bits[1] & 0xffUL);
                                            }
                                            bytes_todo &= ~(1 << byte); // remove from bytes to do
                                            byte_test_status[byte] = WL_ESTIMATED; // make sure this is set for this case
                                            debug_print("        byte %d delay %2d Exhausted\n", byte, delay);
                                            if (!measured_vref_flag) { // this is too noisy when doing measured VREF
                                                ddr_print("N%d.LMC%d.R%d: SWL: Byte %d (0x%02x): delay %d EXHAUSTED \n",
                                                          node, ddr_interface_num, rankx, byte, bits_bad, delay);
                                            }
                                        }
				    }
				} else { // no error, stay with current delay, but keep testing it...
				    debug_print("        byte %d delay %2d Passed\n", byte, delay);
				    byte_test_status[byte] = WL_HARDWARE; // change status
				}

			    } /* for (byte = 0; byte < 9; ++byte) */

			} while (no_errors_count < WL_MIN_NO_ERRORS_COUNT);

#if ENABLE_SW_WLEVEL_UTILIZATION
                        if (! sw_wlevel_hw) {
                            uint64_t percent_x10;
                            if (sum_dram_dclk == 0)
                                sum_dram_dclk = 1;
                            percent_x10 = sum_dram_ops * 1000 / sum_dram_dclk;
                            ddr_print("N%d.LMC%d.R%d: ops %lu, cycles %lu, used %lu.%lu%%\n",
                                      node, ddr_interface_num, rankx, sum_dram_ops, sum_dram_dclk,
                                      percent_x10 / 10, percent_x10 % 10);
                        }
#endif
			if (errors) {
			    debug_print("End WLEV_64 while loop: vref_value %d(0x%x), errors 0x%02x\n",
				      vref_value, vref_value, errors);
			}
		    } // end parallel write-leveling block for delay high-order bits

                    if (sw_wlevel_hw) {
                        ddr_print("N%d.LMC%d.R%d: HW-assisted SWL - ECC estimate not needed.\n",
                                  node, ddr_interface_num, rankx);
                        goto no_ecc_estimate;
                    }

                    if ((ddr_interface_bytemask & 0xff) == 0xff) {
                        if (use_ecc) {
                            int save_byte8 = lmc_wlevel_rank.s.byte8;
                            byte_test_status[8] = WL_HARDWARE; /* H/W delay value */

                            if ((save_byte8 != lmc_wlevel_rank.s.byte3) &&
                                (save_byte8 != lmc_wlevel_rank.s.byte4)) {

                                int test_byte8 = save_byte8;
                                int test_byte8_error;
                                int byte8_error = 0x1f;
                                int adder;
				int avg_bytes = divide_nint(lmc_wlevel_rank.s.byte3+lmc_wlevel_rank.s.byte4, 2);
                                for (adder = 0; adder<= 32; adder+=8) {
                                    test_byte8_error = _abs((adder+save_byte8) - avg_bytes);
                                    if (test_byte8_error < byte8_error) {
                                        byte8_error = test_byte8_error;
                                        test_byte8 = save_byte8 + adder;
                                    }
                                }

#if SW_WL_CHECK_PATCH
				// only do the check if we are not using measured VREF 
				if (!measured_vref_flag) {
                                    test_byte8 &= ~1; /* Use only even settings, rounding down... */

                                    // do validity check on the calculated ECC delay value 
                                    // this depends on the DIMM type
                                    if (spd_rdimm) { // RDIMM
                                        if (spd_dimm_type != 5) { // but not mini-RDIMM
                                            // it can be > byte4, but should never be > byte3
                                            if (test_byte8 > lmc_wlevel_rank.s.byte3) {
                                                byte_test_status[8] = WL_ESTIMATED; /* say it is still estimated */
                                            }
                                        }
                                    } else { // UDIMM
                                        if ((test_byte8 < lmc_wlevel_rank.s.byte3) ||
                                            (test_byte8 > lmc_wlevel_rank.s.byte4))
                                            { // should never be outside the byte 3-4 range
                                                byte_test_status[8] = WL_ESTIMATED; /* say it is still estimated */
                                            }
                                    }
                                    /*
                                     * Report whenever the calculation appears bad.
                                     * This happens if some of the original values were off, or unexpected geometry
                                     * from DIMM type, or custom circuitry (NIC225E, I am looking at you!).
                                     * We will trust the calculated value, and depend on later testing to catch
                                     * any instances when that value is truly bad.
                                     */
                                    if (byte_test_status[8] == WL_ESTIMATED) { // ESTIMATED means there may be an issue
                                        ddr_print("N%d.LMC%d.R%d: SWL: (%cDIMM): calculated ECC delay unexpected (%d/%d/%d)\n",
                                                  node, ddr_interface_num, rankx, (spd_rdimm?'R':'U'),
                                                  lmc_wlevel_rank.s.byte4, test_byte8, lmc_wlevel_rank.s.byte3);
                                        byte_test_status[8] = WL_HARDWARE;
                                    }
                                }
#endif /* SW_WL_CHECK_PATCH */
                                lmc_wlevel_rank.s.byte8 = test_byte8 & ~1; /* Use only even settings */
                            }

                            if (lmc_wlevel_rank.s.byte8 != save_byte8) {
                                /* Change the status if s/w adjusted the delay */
                                byte_test_status[8] = WL_SOFTWARE; /* Estimated delay */
                            }
                        } else {
			    byte_test_status[8] = WL_HARDWARE; /* H/W delay value */
                            lmc_wlevel_rank.s.byte8 = lmc_wlevel_rank.s.byte0; /* ECC is not used */
                        }
                    } else { /* if ((ddr_interface_bytemask & 0xff) == 0xff) */
                        if (use_ecc) {
                            /* Estimate the ECC byte delay  */
                            lmc_wlevel_rank.s.byte4 |= (lmc_wlevel_rank.s.byte3 & 0x38); // add hi-order to b4
                            if ((lmc_wlevel_rank.s.byte4 & 0x06) < (lmc_wlevel_rank.s.byte3 & 0x06)) // orig b4 < orig b3
                                lmc_wlevel_rank.s.byte4 += 8; // must be next clock
                        } else {
                            lmc_wlevel_rank.s.byte4 = lmc_wlevel_rank.s.byte0; /* ECC is not used */
                        }
                        /* Change the status if s/w adjusted the delay */
                        byte_test_status[4] = WL_SOFTWARE; /* Estimated delay */
                    } /* if ((ddr_interface_bytemask & 0xff) == 0xff) */
                } /* if (wlevel_bitmask_errors == 0) */

            no_ecc_estimate:

                bytes_failed = 0;
                for (byte = 0; byte < 9; ++byte) {
                    /* Don't accumulate errors for untested bytes. */
                    if (!(ddr_interface_bytemask & (1 << byte)))
                        continue;
                    bytes_failed += (byte_test_status[byte] == WL_ESTIMATED);
                }

                 /* Vref training loop is only used for DDR4  */
                if (ddr_type != DDR4_DRAM)
			break;

                if (bytes_failed == 0) {
                    if (vref_values_count == 0) {
                        vref_values_start = vref_value;
                    }
                    ++vref_values_count;
                    if (vref_values_count > best_vref_values_count) {
                        best_vref_values_count = vref_values_count;
                        best_vref_values_start = vref_values_start;
                        debug_print("N%d.LMC%d.R%d: Vref Training                    (%2d) :    0x%02x <----- ???? -----> 0x%02x\n",
				    node, ddr_interface_num,
                                  rankx, vref_value, best_vref_values_start,
                                  best_vref_values_start+best_vref_values_count-1);
                    }
                } else {
                    vref_values_count = 0;
		    debug_print("N%d.LMC%d.R%d: Vref Training                    (%2d) :    failed\n",
				node, ddr_interface_num,
				rankx, vref_value);
                }
            } /* for (vref_value=0; vref_value<VREF_LIMIT; ++vref_value) */

            /* Determine address of DRAM to test for software write leveling. */
            rank_addr  = active_rank * (1ull << (pbank_lsb - bunk_enable + (interfaces/2)));
            //rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */
            //rank_addr = cvmx_numa_get_address(node, rank_addr);
            //rank_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;

            /* Adjust address for boot bus hole in memory map. */
            if (rank_addr > 0x10000000)
                rank_addr += 0x10000000;

            debug_print("Rank Address: 0x%llx\n", rank_addr);

            if (bytes_failed) {
#if !DISABLE_SW_WL_PASS_2

		ddr_print("N%d.LMC%d.R%d: Starting SW Write-leveling pass 2\n",
			  node, ddr_interface_num, rankx);

                sw_wl_rank_status = WL_SOFTWARE;

                /* If s/w fixups failed then retry using s/w write-leveling. */
                if (wlevel_bitmask_errors == 0) {
                    /* h/w succeeded but s/w fixups failed. So retry s/w. */
                    debug_print("N%d.LMC%d.R%d: Retrying software Write-Leveling.\n",
				node, ddr_interface_num, rankx);
                }

		{ // start parallel write-leveling block for delay low-order bits
		    int errors;
		    int byte_delay[8];
		    int byte_passed[8];
		    uint64_t bytemask;
		    uint64_t bitmask;
                    int wl_offset;
		    int bytes_todo;
                    int sw_wlevel_offset = 1;

		    for (byte = 0; byte < 8; ++byte) {
			byte_passed[byte] = 0;
		    }

		    bytes_todo = ddr_interface_bytemask;

                    for (wl_offset = sw_wlevel_offset; wl_offset >= 0; --wl_offset) {

			debug_print("Starting wl_offset for-loop: %d\n", wl_offset);

			bytemask = 0;

			for (byte = 0; byte < 8; ++byte) {
			    byte_delay[byte] = 0;
			    if (!(bytes_todo & (1 << byte))) // this does not contain fully passed bytes
				continue;

			    byte_passed[byte] = 0; // reset across passes if not fully passed
			    update_wlevel_rank_struct(&lmc_wlevel_rank, byte, 0); // all delays start at 0
			    bitmask = ((!ddr_interface_64b) && (byte == 4)) ? 0x0f: 0xff;
			    bytemask |= bitmask << (8*byte); // set the bytes bits in the bytemask
			} /* for (byte = 0; byte < 8; ++byte) */

			while (bytemask != 0) { // start a pass if there is any byte lane to test

			    debug_print("Starting bytemask while-loop: 0x%lx\n", bytemask);

			    // write this set of WL delays
			    ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
			    lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

			    //CVMX_CSR_WRITE(node, CVMX_GTI_CWD_POKEX(cvmx_get_core_num()), 0);

			    // do the test
                            if (sw_wlevel_hw)
                                errors = run_best_hw_patterns(cpu_id, node, ddr_interface_num, rank_addr, 
                                                              DBTRAIN_TEST, NULL) & 0xff;
                            else
                                errors = test_dram_byte64(cpu_id, node, ddr_interface_num,
                                                          rank_addr, bytemask, NULL);

			    debug_print("test_dram_byte returned 0x%x\n", errors);

			    // check errors by byte
			    for (byte = 0; byte < 8; ++byte) {
				if (!(bytes_todo & (1 << byte)))
				    continue;

				delay = byte_delay[byte];
				if (errors & (1 << byte)) { // yes, an error
				    debug_print("        byte %d delay %2d Errors\n", byte, delay);
				    byte_passed[byte] = 0;
				} else { // no error
				    byte_passed[byte] += 1;
				    if (byte_passed[byte] == (1 + wl_offset)) { /* Look for consecutive working settings */
					debug_print("        byte %d delay %2d FULLY Passed\n", byte, delay);
					if (wl_offset == 1) {
					    byte_test_status[byte] = WL_SOFTWARE;
					} else if (wl_offset == 0) {
					    byte_test_status[byte] = WL_SOFTWARE1;
					}
					bytemask &= ~(0xffULL << (8*byte)); // test no longer, remove from byte mask this pass
					bytes_todo &= ~(1 << byte); // remove completely from concern
					continue; // on to the next byte, bypass delay updating!!
				    } else {
					debug_print("        byte %d delay %2d Passed\n", byte, delay);
				    }
				}
				// error or no, here we move to the next delay value for this byte, unless done all delays
				// only a byte that has "fully passed" will bypass around this,
				delay += 2;
				if (delay < 32) {
				    update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
				    debug_print("        byte %d delay %2d New\n", byte, delay);
				    byte_delay[byte] = delay;
				} else {
				    // reached max delay, done with this byte
				    debug_print("        byte %d delay %2d Exhausted\n", byte, delay);
				    bytemask &= ~(0xffULL << (8*byte)); // test no longer, remove from byte mask this pass
				}
			    } /* for (byte = 0; byte < 8; ++byte) */
			    debug_print("End of for-loop: bytemask 0x%lx\n", bytemask);
			} /* while (bytemask != 0) */
		    } /* for (wl_offset = sw_wlevel_offset; wl_offset >= 0; --wl_offset) */

		    for (byte = 0; byte < 8; ++byte) {
			// any bytes left in bytes_todo did not pass
			if (bytes_todo & (1 << byte)) {
                            cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
			    /* Last resort. Use Rlevel settings to estimate
			       Wlevel if software write-leveling fails */
			    debug_print("Using RLEVEL as WLEVEL estimate for byte %d\n", byte);
			    lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
			    rlevel_to_wlevel(&lmc_rlevel_rank, &lmc_wlevel_rank, byte);
			}
		    } /* for (byte = 0; byte < 8; ++byte) */

		} // end parallel write-leveling block for delay low-order bits

                if (use_ecc) {
                    /* ECC byte has to be estimated. Take the average of the two surrounding bytes. */
                    int test_byte8 = divide_nint(lmc_wlevel_rank.s.byte3
                                                 + lmc_wlevel_rank.s.byte4
                                                 + 2 /* round-up*/ , 2);
                    lmc_wlevel_rank.s.byte8 = test_byte8 & ~1; /* Use only even settings */
                    byte_test_status[8] = WL_ESTIMATED; /* Estimated delay */
                } else {
                    byte_test_status[8] = WL_HARDWARE; /* H/W delay value */
                    lmc_wlevel_rank.s.byte8 = lmc_wlevel_rank.s.byte0; /* ECC is not used */
                }

                /* Set delays for unused bytes to match byte 0. */
                for (byte=0; byte<8; ++byte) {
                    if ((ddr_interface_bytemask & (1 << byte)))
                        continue;
                    update_wlevel_rank_struct(&lmc_wlevel_rank, byte,
					      lmc_wlevel_rank.s.byte0);
                    byte_test_status[byte] = WL_SOFTWARE;
                }
#else /* !DISABLE_SW_WL_PASS_2 */
		// FIXME? the big hammer, did not even try SW WL pass2, assume only chip reset will help
		ddr_print("N%d.LMC%d.R%d: S/W write-leveling pass 1 failed\n",
			  node, ddr_interface_num, rankx);
		sw_wl_failed = 1;
#endif /* !DISABLE_SW_WL_PASS_2 */

            } else { /* if (bytes_failed) */

		// SW WL pass 1 was OK, write the settings
                ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

#if SW_WL_CHECK_PATCH
		// do validity check on the delay values by running the test 1 more time...
		// FIXME: we really need to check the ECC byte setting here as well,
		//        so we need to enable ECC for this test!!!
		// if there are any errors, claim SW WL failure
		{
		    uint64_t datamask = (ddr_interface_64b) ? 0xffffffffffffffffULL : 0x00000000ffffffffULL;
                    int errors;

                    // do the test
                    if (sw_wlevel_hw)
                        errors = run_best_hw_patterns(cpu_id, node, ddr_interface_num, rank_addr,
                                                      DBTRAIN_TEST, NULL) & 0xff; // FIXME?
                    else
                        errors = test_dram_byte64(cpu_id, node, ddr_interface_num,
                                                  rank_addr, datamask, NULL);

		    if (errors) {
			ddr_print("N%d.LMC%d.R%d: Wlevel Rank Final Test errors 0x%x\n",
			      node, ddr_interface_num, rankx, errors);
			sw_wl_failed = 1;
		    }
		}
#endif /* SW_WL_CHECK_PATCH */

            } /* if (bytes_failed) */

	    // FIXME? dump the WL settings, so we get more of a clue as to what happened where
            ddr_print("N%d.LMC%d.R%d: Wlevel Rank %#4x, 0x%016lX  : %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %s\n",
                      node, ddr_interface_num, rankx,
                      lmc_wlevel_rank.s.status,
                      lmc_wlevel_rank.u64,
                      lmc_wlevel_rank.s.byte8, wl_status_strings[byte_test_status[8]],
                      lmc_wlevel_rank.s.byte7, wl_status_strings[byte_test_status[7]],
                      lmc_wlevel_rank.s.byte6, wl_status_strings[byte_test_status[6]],
                      lmc_wlevel_rank.s.byte5, wl_status_strings[byte_test_status[5]],
                      lmc_wlevel_rank.s.byte4, wl_status_strings[byte_test_status[4]],
                      lmc_wlevel_rank.s.byte3, wl_status_strings[byte_test_status[3]],
                      lmc_wlevel_rank.s.byte2, wl_status_strings[byte_test_status[2]],
                      lmc_wlevel_rank.s.byte1, wl_status_strings[byte_test_status[1]],
                      lmc_wlevel_rank.s.byte0, wl_status_strings[byte_test_status[0]],
                      (sw_wl_rank_status == WL_HARDWARE) ? "" : "(s)"
                      );

	    // finally, check for fatal conditions: either chip reset right here, or return error flag
            if (((ddr_type == DDR4_DRAM) && (best_vref_values_count == 0)) || sw_wl_failed) {
                if (!ddr_disable_chip_reset) { // do chip RESET
                    error_print("INFO: Short memory test indicates a retry is needed on N%d.LMC%d.R%d. Resetting node...\n",
                                node, ddr_interface_num, rankx);
#if defined(BDK) || defined(CVMX_BUILD_FOR_UBOOT)
                    cvmx_wait_usec(500000);
                    cvmx_reset_octeon_node(node);
                    while (1);
#else
                    return -1; // real error return
#endif
                } else { // return error flag so LMC init can be retried...
                    ddr_print("INFO: Short memory test indicates a retry is needed on N%d.LMC%d.R%d. Restarting LMC init...\n",
                                node, ddr_interface_num, rankx);
                    return 0; // 0 indicates restart possible...
                }
            }
            active_rank++;
        } /* for (rankx = 0; rankx < dimm_count * 4; rankx++) */

        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            for (i=0; i<9; ++i) {
                if ((s = lookup_env_parameter("ddr%d_wlevel_rank%d_byte%d", ddr_interface_num, rankx, i)) != NULL) {
                    parameter_set |= 1;
                    value = strtoul(s, NULL, 0);

                    update_wlevel_rank_struct(&lmc_wlevel_rank, i, value);
                }
            }

            if ((s = lookup_env_parameter_ull("ddr%d_wlevel_rank%d", ddr_interface_num, rankx)) != NULL) {
                parameter_set |= 1;
                value = strtoull(s, NULL, 0);
                lmc_wlevel_rank.u64 = value;
            }

            if (parameter_set) {
                ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num), lmc_wlevel_rank.u64);
                lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));
		display_WL(node, ddr_interface_num, lmc_wlevel_rank, rankx);
            }
#if WLEXTRAS_PATCH
	    if ((rank_mask & 0x0F) != 0x0F) { // if there are unused entries to be filled
		if (rankx < 3) {
		    debug_print("N%d.LMC%d.R%d: checking for WLEVEL_RANK unused entries.\n",
			      node, ddr_interface_num, rankx);
		    if (rankx == 0) { // if rank 0, write ranks 1 and 2 here if empty
			if (!(rank_mask & (1<<1))) { // check that rank 1 is empty
			    debug_print("N%d.LMC%d.R%d: writing WLEVEL_RANK unused entry R%d.\n",
				      node, ddr_interface_num, rankx, 1);
			    //DRAM_CSR_WRITE(node, BDK_LMCX_WLEVEL_RANKX(ddr_interface_num, 1), lmc_wlevel_rank.u);
                            ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(1, ddr_interface_num), lmc_wlevel_rank.u64);
			}
			if (!(rank_mask & (1<<2))) { // check that rank 2 is empty
			    debug_print("N%d.LMC%d.R%d: writing WLEVEL_RANK unused entry R%d.\n",
				      node, ddr_interface_num, rankx, 2);
			    //DRAM_CSR_WRITE(node, BDK_LMCX_WLEVEL_RANKX(ddr_interface_num, 2), lmc_wlevel_rank.u);
                            ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(2, ddr_interface_num), lmc_wlevel_rank.u64);
			}
		    }
		    // if rank 0, 1 or 2, write rank 3 here if empty
		    if (!(rank_mask & (1<<3))) { // check that rank 3 is empty
			debug_print("N%d.LMC%d.R%d: writing WLEVEL_RANK unused entry R%d.\n",
				  node, ddr_interface_num, rankx, 3);
			//DRAM_CSR_WRITE(node, BDK_LMCX_WLEVEL_RANKX(ddr_interface_num, 3), lmc_wlevel_rank.u);
                        ddr_config_write_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(3, ddr_interface_num), lmc_wlevel_rank.u64);
		    }
		}
	    }
#endif /* WLEXTRAS_PATCH */
        } /* for (rankx = 0; rankx < dimm_count * 4;rankx++) */

        /* Enable 32-bit mode if required. */
        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        lmc_config.cn78xx.mode32b         = (! ddr_interface_64b);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);

        /* Restore the ECC configuration */
        if (!sw_wlevel_hw_default) {
            lmc_config.cn78xx.ecc_ena = use_ecc;
        }
        ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);

#ifndef __U_BOOT__
        /* Restore the l2 set configuration */
        {
            const char *eptr;
            eptr = getenv("limit_l2_ways");
            if (eptr) {
                int ways = simple_strtoul(eptr, NULL, 10);
                limit_l2_ways(ways, 1);
            } else {
                limit_l2_ways(cvmx_l2c_get_num_assoc(), 0);
            }

            eptr = getenv("limit_l2_mask");
            if (eptr) {
                int mask = simple_strtoul(eptr, NULL, 10);
                limit_l2_mask(mask);
            }
        }
#endif
    } // End Software Write-Leveling block

#ifdef ENABLE_DISPLAY_MPR_PAGE
    // this sometimes causes stack overflow crashes...
    if (ddr_type == DDR4_DRAM) {
        Display_MPR_Page(cpu_id, node, rank_mask, ddr_interface_num, dimm_count, 2);
        Display_MPR_Page(cpu_id, node, rank_mask, ddr_interface_num, dimm_count, 0);
    }
#endif /* ENABLE_DISPLAY_MPR_PAGE */

    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_write_offset",
			       custom_lmc_config->dll_write_offset, "ddr%d_dll_write_offset_byte%d", 1);
    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_read_offset",
			       custom_lmc_config->dll_read_offset,  "ddr%d_dll_read_offset_byte%d",  2);

#define SLOT_CTL_INCR(csr, chip, field, incr)                                    \
                csr.chip.field = (csr.chip.field < (64 - incr)) ? (csr.chip.field + incr) : 63

    /* Workaround Errata 21063*/
    if (octeon_is_cpuid(OCTEON_CN78XX) || octeon_is_cpuid(OCTEON_CN70XX_PASS1_X)) {
        cvmx_lmcx_slot_ctl0_t slot_ctl0;
        cvmx_lmcx_slot_ctl1_t slot_ctl1;
        cvmx_lmcx_slot_ctl2_t slot_ctl2;
        cvmx_lmcx_ext_config_t ext_config;

        slot_ctl0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL0(ddr_interface_num));
        slot_ctl1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL1(ddr_interface_num));
        slot_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL2(ddr_interface_num));

        ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));

        /* When ext_config.s.read_ena_bprch is set add 1 */
        if (ext_config.s.read_ena_bprch) {
            SLOT_CTL_INCR(slot_ctl0, cn78xx, r2w_init, 1);
            SLOT_CTL_INCR(slot_ctl0, cn78xx, r2w_l_init, 1);
            SLOT_CTL_INCR(slot_ctl1, cn78xx, r2w_xrank_init, 1);
            SLOT_CTL_INCR(slot_ctl2, cn78xx, r2w_xdimm_init, 1);
        }

        /* Always add 2 */
        SLOT_CTL_INCR(slot_ctl1, cn78xx, w2r_xrank_init, 2);
        SLOT_CTL_INCR(slot_ctl2, cn78xx, w2r_xdimm_init, 2);

        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_SLOT_CTL0(ddr_interface_num),
                                  slot_ctl0.u64);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_SLOT_CTL1(ddr_interface_num),
                                  slot_ctl1.u64);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_SLOT_CTL2(ddr_interface_num),
                                  slot_ctl2.u64);
    }

    /* Workaround Errata 21216*/
    if (octeon_is_cpuid(OCTEON_CN78XX_PASS1_X) || octeon_is_cpuid(OCTEON_CN70XX_PASS1_X)) {
        cvmx_lmcx_slot_ctl1_t slot_ctl1;
        cvmx_lmcx_slot_ctl2_t slot_ctl2;

        slot_ctl1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL1(ddr_interface_num));
        slot_ctl1.cn78xx.w2w_xrank_init = max(10, (int) slot_ctl1.cn78xx.w2w_xrank_init);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_SLOT_CTL1(ddr_interface_num),
                                  slot_ctl1.u64);

        slot_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL2(ddr_interface_num));
        slot_ctl2.cn78xx.w2w_xdimm_init = max(10, (int) slot_ctl2.cn78xx.w2w_xdimm_init);
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_SLOT_CTL2(ddr_interface_num),
                                  slot_ctl2.u64);
    }

    /*
     * 4.8.11 Final LMC Initialization
     *
     * Early LMC initialization, LMC write-leveling, and LMC read-leveling
     * must be completed prior to starting this final LMC initialization.
     *
     * LMC hardware updates the LMC(0)_SLOT_CTL0, LMC(0)_SLOT_CTL1,
     * LMC(0)_SLOT_CTL2 CSRs with minimum values based on the selected
     * readleveling and write-leveling settings. Software should not write
     * the final LMC(0)_SLOT_CTL0, LMC(0)_SLOT_CTL1, and LMC(0)_SLOT_CTL2
     * values until after the final read-leveling and write-leveling settings
     * are written.
     *
     * Software must ensure the LMC(0)_SLOT_CTL0, LMC(0)_SLOT_CTL1, and
     * LMC(0)_SLOT_CTL2 CSR values are appropriate for this step. These CSRs
     * select the minimum gaps between read operations and write operations
     * of various types.
     *
     * Software must not reduce the values in these CSR fields below the
     * values previously selected by the LMC hardware (during write-leveling
     * and read-leveling steps above).
     *
     * All sections in this chapter may be used to derive proper settings for
     * these registers.
     *
     * For minimal read latency, L2C_CTL[EF_ENA,EF_CNT] should be programmed
     * properly. This should be done prior to the first read.
     */

#ifdef ENABLE_SLOT_CTL_ACCESS
    {
        cvmx_lmcx_slot_ctl0_t lmc_slot_ctl0;
        cvmx_lmcx_slot_ctl1_t lmc_slot_ctl1;
        cvmx_lmcx_slot_ctl2_t lmc_slot_ctl2;
        cvmx_lmcx_slot_ctl3_t lmc_slot_ctl3;

        lmc_slot_ctl0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL0(ddr_interface_num));
        lmc_slot_ctl1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL1(ddr_interface_num));
        lmc_slot_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL2(ddr_interface_num));
        lmc_slot_ctl3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SLOT_CTL3(ddr_interface_num));

        ddr_print("%-45s : 0x%016llx\n", "LMC_SLOT_CTL0", lmc_slot_ctl0.u64);
        ddr_print("%-45s : 0x%016llx\n", "LMC_SLOT_CTL1", lmc_slot_ctl1.u64);
        ddr_print("%-45s : 0x%016llx\n", "LMC_SLOT_CTL2", lmc_slot_ctl2.u64);
        ddr_print("%-45s : 0x%016llx\n", "LMC_SLOT_CTL3", lmc_slot_ctl3.u64);

        // for now, look only for SLOT_CTL1 envvar for override of contents
        if ((s = lookup_env_parameter("ddr%d_slot_ctl1", ddr_interface_num)) != NULL) {
            int slot_ctl1_incr = strtoul(s, NULL, 0);
            // validate the value
            if ((slot_ctl1_incr < 1) || (slot_ctl1_incr > 3)) {
                error_print("ddr%d_slot_ctl1 illegal value (%d); must be 1-3\n",
                            ddr_interface_num, slot_ctl1_incr);
            } else {

#define INCR(csr, chip, field, incr)                                    \
                csr.chip.field = (csr.chip.field < (64 - incr)) ? (csr.chip.field + incr) : 63

                // modify all the SLOT_CTL1 fields by the increment, for now...
                // but make sure the value will not overflow!!!
                INCR(lmc_slot_ctl1, s, r2r_xrank_init, slot_ctl1_incr);
                INCR(lmc_slot_ctl1, s, r2w_xrank_init, slot_ctl1_incr);
                INCR(lmc_slot_ctl1, s, w2r_xrank_init, slot_ctl1_incr);
                INCR(lmc_slot_ctl1, s, w2w_xrank_init, slot_ctl1_incr);
                ddr_config_write_csr_node(node, CVMX_LMCX_SLOT_CTL1(ddr_interface_num), lmc_slot_ctl1.u64);
            }
        }
    }
#endif /* ENABLE_SLOT_CTL_ACCESS */
    {
        /* Clear any residual ECC errors */
        int num_tads = 1;
        int tad;
        int num_mcis = 1;
        int mci;

	if (octeon_is_cpuid(OCTEON_CN78XX)) {
            num_tads = 8;
            num_mcis = 4;
        } else if (octeon_is_cpuid(OCTEON_CN70XX)) {
            num_tads = 1;
            num_mcis = 1;
        } else if (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX)) {
            num_tads = 4;
            num_mcis = 3;
        }

        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_INT(ddr_interface_num), -1ULL);
        cvmx_read_csr_node(node, CVMX_LMCX_INT(ddr_interface_num));

        for (tad = 0; tad < num_tads; tad++) {
            ddr_config_write_csr_node(node, CVMX_L2C_TADX_INT(tad),
                                      cvmx_read_csr_node(node,
                                                         CVMX_L2C_TADX_INT(tad)));
            debug_print("%-45s : (%d) 0x%08llx\n", "CVMX_L2C_TAD_INT", tad,
                      cvmx_read_csr_node(node, CVMX_L2C_TADX_INT(tad)));
        }

        for (mci = 0; mci < num_mcis; mci++) {
            ddr_config_write_csr_node(node, CVMX_L2C_MCIX_INT(mci),
                                      cvmx_read_csr_node(node,
                                                         CVMX_L2C_MCIX_INT(mci)));
            debug_print("%-45s : (%d) 0x%08llx\n", "L2C_MCI_INT", mci,
                      cvmx_read_csr_node(node, CVMX_L2C_MCIX_INT(mci)));
        }

        ddr_print("%-45s : 0x%08llx\n", "LMC_INT",
                  cvmx_read_csr_node(node, CVMX_LMCX_INT(ddr_interface_num)));
    }

    return mem_size_mbytes;
}
///////////////////////////////////////////////////////////////////////

/////    HW-assist byte DLL offset tuning   //////

#define ddr_print2   debug_print
#define ddr_print3   debug_print
#define ddr_print4   debug_print

static int
cvmx_dram_get_num_lmc(int node, uint32_t cpu_id)
{
    cvmx_lmcx_dll_ctl2_t lmcx_dll_ctl2;

    if (octeon_is_cpuid(OCTEON_CN70XX))
        return 1;

    if (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX))
    {
        // sample LMC1 
        //BDK_CSR_INIT(lmcx_dll_ctl1, node, CVMX_LMCX_DLL_CTL2(1));
        lmcx_dll_ctl2.u64 =  cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(1));
        if (lmcx_dll_ctl2.cn78xx.intf_en)
            return 2;
        else
            return 1;
    }

    // for CN78XX, LMCs are always active in pairs, and always LMC0/1
    // so, we sample LMC2 to see if 2 and 3 are active
    //BDK_CSR_INIT(lmcx_dll_ctl2, node, CVMX_LMCX_DLL_CTL2(2));
    lmcx_dll_ctl2.u64 =  cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(2));
    if (lmcx_dll_ctl2.cn78xx.intf_en)
        return 4;
    else
        return 2;
}

// got to do these here, even though already defined in BDK

/**
 * Return the number of bank bits in use
 *
 * @param node   Node to probe
 *
 */ 
// all DDR3, and DDR4 x16 today, use only 3 bank bits; DDR4 x4 and x8 always have 4 bank bits
// NOTE: this will change in the future, when DDR4 x16 devices can come with 16 banks!! FIXME!!
static int
cvmx_dram_get_num_bank_bits(int node, int lmc)
{
    // PROTECT!!!
    //BDK_CSR_INIT(lmcx_dll_ctl2, node, CVMX_LMCX_DLL_CTL2(lmc)); // can always read this
    cvmx_lmcx_dll_ctl2_t lmcx_dll_ctl2;
    cvmx_lmcx_config_t lmcx_config;
    cvmx_lmcx_ddr_pll_ctl_t lmcx_ddr_pll_ctl;
    int bank_width;

    lmcx_dll_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(lmc)); // can always read this
    
    if (lmcx_dll_ctl2.cn78xx.dreset) // check LMCn
        return 0;

    //BDK_CSR_INIT(lmcx_config, node, CVMX_LMCX_CONFIG(lmc)); // sample LMCn
    lmcx_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DLL_CTL2(lmc));
    //BDK_CSR_INIT(lmcx_ddr_pll_ctl, node, CVMX_LMCX_DDR_PLL_CTL(lmc)); // sample LMCn
    lmcx_ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(lmc));

    bank_width = ((lmcx_ddr_pll_ctl.s.ddr4_mode != 0) && (lmcx_config.s.bg2_enable)) ? 4 : 3; 
    return bank_width;
}

#define EXTRACT(v, lsb, width) (((v) >> (lsb)) & ((1ull << (width)) - 1))
#define ADDRESS_HOLE 0x10000000ULL

/**
 * Given a physical DRAM address, extract information about the node, LMC, DIMM,
 * prank, lrank, bank, row, and column that was accessed.
 *
 * @param address Physical address to decode
 * @param node    Node the address was for
 * @param lmc     LMC controller the address was for
 * @param dimm    DIMM the address was for
 * @param prank   Physical RANK on the DIMM
 * @param lrank   Logical RANK on the DIMM
 * @param bank    BANK on the DIMM
 * @param row     Row on the DIMM
 * @param col     Column on the DIMM
 */
static void
cvmx_dram_address_extract_info(uint64_t address, uint32_t cpu_id, int *node, int *lmc,
                               int *dimm, int *prank, int *lrank, int *bank, int *row, int *col)
{
    int bank_lsb, xbits;
    cvmx_l2c_ctl_t l2c_ctl;
    cvmx_lmcx_config_t lmcx_config;
    cvmx_lmcx_control_t lmcx_control;
    cvmx_lmcx_ext_config_t ext_config;
    int bitno = (octeon_is_cpuid(OCTEON_CN7XXX)) ? 20: 18;
    int bank_width;
    int dimm_lsb;
    int dimm_width;
    int prank_lsb, lrank_lsb;
    int prank_width, lrank_width;
    int row_lsb;
    int row_width;
    int col_hi_lsb;
    int col_hi_width;
    int col_hi;

    if (octeon_is_cpuid(OCTEON_CN73XX) || octeon_is_cpuid(OCTEON_CNF75XX))
        bitno = 18;

    *node = EXTRACT(address, 40, 2); /* Address bits [41:40] */

    address &= (1ULL << 40) - 1; // lop off any node bits or above
    if (address >= ADDRESS_HOLE) // adjust down if at HOLE or above
        address -= ADDRESS_HOLE;

    /* Determine the LMC controllers */
    //BDK_CSR_INIT(l2c_ctl, *node, CVMX_L2C_CTL);
    l2c_ctl.u64 = cvmx_read_csr_node(*node, CVMX_L2C_CTL);

    /* xbits depends on number of LMCs */
    xbits = cvmx_dram_get_num_lmc(*node, cpu_id) >> 1; // 4->2, 2->1, 1->0
    bank_lsb = 7 + xbits;

    /* LMC number is probably aliased */
    if (l2c_ctl.s.disidxalias)
        *lmc = EXTRACT(address, 7, xbits);
    else
        *lmc = EXTRACT(address, 7, xbits) ^ EXTRACT(address, bitno, xbits) ^ EXTRACT(address, 12, xbits);

    /* Figure out the bank field width */
    //BDK_CSR_INIT(lmcx_config, *node, CVMX_LMCX_CONFIG(*lmc));
    lmcx_config.u64 = cvmx_read_csr_node(*node, CVMX_LMCX_CONFIG(*lmc));
    ext_config.u64 = cvmx_read_csr_node(*node, CVMX_LMCX_EXT_CONFIG(*lmc));
    bank_width = cvmx_dram_get_num_bank_bits(*node, *lmc);

    /* Extract additional info from the LMC_CONFIG CSR */
    dimm_lsb    = 28 + lmcx_config.s.pbank_lsb + xbits;
    dimm_width  = 40 - dimm_lsb;
    prank_lsb   = dimm_lsb - lmcx_config.s.rank_ena;
    prank_width = dimm_lsb - prank_lsb;
    lrank_lsb   = prank_lsb - ext_config.s.dimm0_cid;
    lrank_width = prank_lsb - lrank_lsb;
    row_lsb     = 14 + lmcx_config.s.row_lsb + xbits;
    row_width   = lrank_lsb - row_lsb;
    col_hi_lsb  = bank_lsb + bank_width;
    col_hi_width= row_lsb - col_hi_lsb;

    /* Extract the parts of the address */
    *dimm = EXTRACT(address, dimm_lsb, dimm_width);
    *prank = EXTRACT(address, prank_lsb, prank_width);
    *lrank = EXTRACT(address, lrank_lsb, lrank_width);
    *row = EXTRACT(address, row_lsb, row_width);

    /* bank calculation may be aliased... */
    //BDK_CSR_INIT(lmcx_control, *node, CVMX_LMCX_CONTROL(*lmc));
    lmcx_control.u64 = cvmx_read_csr_node(*node, CVMX_LMCX_CONTROL(*lmc));
    if (lmcx_control.s.xor_bank)
        *bank = EXTRACT(address, bank_lsb, bank_width) ^ EXTRACT(address, 12 + xbits, bank_width);
    else
        *bank = EXTRACT(address, bank_lsb, bank_width);

    /* LMC number already extracted */
    col_hi = EXTRACT(address, col_hi_lsb, col_hi_width);
    *col = EXTRACT(address, 3, 4) | (col_hi << 4);
    /* Bus byte is address bits [2:0]. Unused here */
}

// end of added workarounds

// NOTE: "mode" argument:
//         DBTRAIN_TEST: for testing using GP patterns, includes ECC
//         DBTRAIN_DBI:  for DBI deskew training behavior (uses GP patterns)
//         DBTRAIN_LFSR: for testing using LFSR patterns, includes ECC
// NOTE: trust the caller to specify the correct/supported mode
//
static int test_dram_byte_hw(int node, uint32_t cpu_id, int ddr_interface_num,
                             uint64_t p, int mode, uint64_t *xor_data)
{
    uint64_t p1;
    uint64_t k;
    int errors = 0;

    uint64_t mpr_data0, mpr_data1;
    uint64_t bad_bits[2] = {0,0};

    int node_address, lmc, dimm;
    int prank, lrank;
    int bank, row, col;
    int save_or_dis;
    int byte;
    int ba_loop, ba_bits;

    cvmx_lmcx_rlevel_ctl_t rlevel_ctl;
    cvmx_lmcx_dbtrain_ctl_t dbtrain_ctl;
    cvmx_lmcx_phy_ctl_t phy_ctl;

    int biter_errs;

    // FIXME: K iterations set to 4 for now.
    // FIXME: decrement to increase interations.
    // FIXME: must be no less than 22 to stay above an LMC hash field. 
    int kshift = 27;

    const char *s;

    // allow override default setting for kshift
    if ((s = getenv("ddr_tune_set_kshift")) != NULL) {
        int temp = simple_strtoul(s, NULL, 0);
        if ((temp < 22) || (temp > 28)) {
            ddr_print("N%d.LMC%d: ILLEGAL override of kshift to %d, using default %d\n",
                      node, ddr_interface_num, temp, kshift);
        } else {
            ddr_print("N%d.LMC%d: overriding kshift (%d) to %d\n",
                      node, ddr_interface_num, kshift, temp);
            kshift = temp;
        }
    }

    /*
      1) Make sure that RLEVEL_CTL[OR_DIS] = 0.
    */
    rlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));
    save_or_dis = rlevel_ctl.s.or_dis;
    rlevel_ctl.s.or_dis = 0;    /* or_dis must be disabled for this sequence */
    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num), rlevel_ctl.u64);

    /*
      NOTE: this step done in the calling routine(s)...
      3) Setup GENERAL_PURPOSE[0-2] registers with the data pattern of choice.
      a. GENERAL_PURPOSE0[DATA<63:0>] – sets the initial lower (rising edge) 64 bits of data.
      b. GENERAL_PURPOSE1[DATA<63:0>] – sets the initial upper (falling edge) 64 bits of data.
      c. GENERAL_PURPOSE2[DATA<15:0>] – sets the initial lower (rising edge <7:0>) and upper
         (falling edge <15:8>) ECC data.
     */

    // final address must include LMC and node
    p |= (ddr_interface_num << 7); /* Map address into proper interface */
    p |= (uint64_t)node << CVMX_NODE_MEM_SHIFT; // map to node

    /* Add base offset to both test regions to not clobber u-boot stuff
     * when running from L2 for NAND boot.
     */
    p += 0x20000000; // offset to 512MB, ie above THE HOLE!!!
    p |= 1ull<<63; // needed for OCTEON

    errors = 0;

    cvmx_dram_address_extract_info(p, cpu_id, &node_address, &lmc, &dimm,
                                   &prank, &lrank, &bank, &row, &col);
    debug_print("test_dram_byte_hw: START at A:0x%012llx, N%d L%d D%d/%d R%d B%1x Row:%05x Col:%05x\n",
                p, node_address, lmc, dimm, prank, lrank, bank, row, col);

    // only check once per call, and ignore if no match...
    if ((int)node != node_address) {
        error_print("ERROR: Node address mismatch\n");
        return 0;
    }
    if (lmc != ddr_interface_num) {
        error_print("ERROR: LMC address mismatch\n");
        return 0;
    }

    /*
      7) Set PHY_CTL[PHY_RESET] = 1 (LMC automatically clears this as it’s a one-shot operation).
      This is to get into the habit of resetting PHY’s SILO to the original 0 location.
    */
    //BDK_CSR_MODIFY(phy_ctl, node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.s.phy_reset = 1);
    phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
    phy_ctl.s.phy_reset = 1;
    ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

    /* Walk through a range of addresses avoiding bits that alias
     * interfaces on the CN88XX.
     */

    // FIXME: want to try to keep the K increment from affecting the LMC via hash,
    // FIXME: so keep it above bit 21
    // NOTE:  we also want to keep k less than the base offset of bit 29 (512MB)

    for (k = 0; k < (1UL << 29); k += (1UL << kshift)) {

	// FIXME: the sequence will interate over 1/2 cacheline
	// FIXME: for each unit specified in "read_cmd_count",
	// FIXME: so, we setup each sequence to do the max cachelines it can

	p1 = p + k;

	cvmx_dram_address_extract_info(p1, cpu_id, &node_address, &lmc, &dimm,
                                       &prank, &lrank, &bank, &row, &col);

        /*
          2) Setup the fields of the CSR DBTRAIN_CTL as follows:
            a. COL, ROW, BA, BG, PRANK points to the starting point of the address.
               You can just set them to all 0.
            b. RW_TRAIN – set this to 1.
            c. TCCD_L – set this to 0.
            d. READ_CMD_COUNT – instruct the sequence to the how many writes/reads.
               It is 5 bits field, so set to 31 of maximum # of r/w.
        */
        dbtrain_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DBTRAIN_CTL(ddr_interface_num));
        dbtrain_ctl.s.column_a       = col;
        dbtrain_ctl.s.row_a          = row;
        dbtrain_ctl.s.bg             = (bank >> 2) & 3;
        dbtrain_ctl.s.prank          = (dimm * 2) + prank; // FIXME?
        dbtrain_ctl.s.lrank          = lrank; // FIXME?
        dbtrain_ctl.s.activate       = (mode == DBTRAIN_DBI);
        dbtrain_ctl.s.write_ena      = 1;
        dbtrain_ctl.s.read_cmd_count = 31; // max count pass 1.x
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X) || octeon_is_cpuid(OCTEON_CNF75XX))
            dbtrain_ctl.s.cmd_count_ext = 3; // max count on chips that support it
        else
            dbtrain_ctl.s.cmd_count_ext = 0; // max count pass 1.x
        dbtrain_ctl.s.rw_train       = 1;
        dbtrain_ctl.s.tccd_sel       = (mode == DBTRAIN_DBI);
        // LFSR should only be on when chip supports it...
        dbtrain_ctl.s.lfsr_pattern_sel = (mode == DBTRAIN_LFSR) ? 1 : 0;

        biter_errs = 0;

	// for each address, iterate over the 4 "banks" in the BA
	for (ba_loop = 0, ba_bits = bank & 3;
	     ba_loop < 4;
	     ba_loop++, ba_bits = (ba_bits + 1) & 3)
	{


            dbtrain_ctl.s.ba             = ba_bits;
            ddr_config_write_csr_node(node, CVMX_LMCX_DBTRAIN_CTL(ddr_interface_num), dbtrain_ctl.u64);

	    /*
              We will use the RW_TRAINING sequence (14) for this task.

	      4) Kick off the sequence (SEQ_CTL[SEQ_SEL] = 14, SEQ_CTL[INIT_START] = 1).
	      5) Poll on SEQ_CTL[SEQ_COMPLETE] for completion.
	    */
	    perform_octeon3_ddr3_sequence(cpu_id, node, prank, ddr_interface_num, 14);

	    /*
	      6) Read MPR_DATA0 and MPR_DATA1 for results.
	      a. MPR_DATA0[MPR_DATA<63:0>] – comparison results for DQ63:DQ0.
	      (1 means MATCH, 0 means FAIL).
	      b. MPR_DATA1[MPR_DATA<7:0>] – comparison results for ECC bit7:0.
	    */
	    mpr_data0 = cvmx_read_csr_node(node, CVMX_LMCX_MPR_DATA0(ddr_interface_num));
	    mpr_data1 = cvmx_read_csr_node(node, CVMX_LMCX_MPR_DATA1(ddr_interface_num));

	    /*
	      7) Set PHY_CTL[PHY_RESET] = 1 (LMC automatically clears this as it’s a one-shot operation).
	      This is to get into the habit of resetting PHY’s SILO to the original 0 location.
	    */
	    //BDK_CSR_MODIFY(phy_ctl, node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.s.phy_reset = 1);
            phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
            phy_ctl.s.phy_reset = 1;
            ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num), phy_ctl.u64);

            if (mode == DBTRAIN_DBI)
                continue; // bypass any error checking or updating when DBI mode

            // data bytes
            if (~mpr_data0) {
                for (byte = 0; byte < 8; byte++) {
                    if ((~mpr_data0 >> (8 * byte)) & 0xffUL)
                        biter_errs |= (1 << byte);
                }
                // accumulate bad bits
                bad_bits[0] |= ~mpr_data0;
            }

            // include ECC byte errors
            if (~mpr_data1 & 0xffUL) {
                biter_errs |= (1 << 8);
                bad_bits[1] |= ~mpr_data1 & 0xffUL;
            }

	} /* for (int ba_loop = 0; ba_loop < 4; ba_loop++) */

        errors |= biter_errs;

    } /* end for (k=...) */

    rlevel_ctl.s.or_dis = save_or_dis;
    ddr_config_write_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num), rlevel_ctl.u64);

    if ((mode != DBTRAIN_DBI) && (xor_data != NULL)) { // send the bad bits back...
        xor_data[0] = bad_bits[0];
        xor_data[1] = bad_bits[1];
    }

    return errors;
}

// setup default for byte test pattern array
// take these from the HRM section 6.9.13
static const uint64_t byte_pattern_0[] = {
    0xFFAAFFFFFF55FFFFULL, // GP0
    0x55555555AAAAAAAAULL, // GP1
    0xAA55AAAAULL,         // GP2
};
static const uint64_t byte_pattern_1[] = {
    0xFBF7EFDFBF7FFEFDULL, // GP0
    0x0F1E3C78F0E1C387ULL, // GP1
    0xF0E1BF7FULL,         // GP2
};
// this is from Andrew via LFSR with PRBS=0xFFFFAAAA
static const uint64_t byte_pattern_2[] = {
    0xEE55AADDEE55AADDULL, // GP0
    0x55AADDEE55AADDEEULL, // GP1
    0x55EEULL,             // GP2
};
// this is from Mike via LFSR with PRBS=0x4A519909
static const uint64_t byte_pattern_3[] = {
    0x0088CCEE0088CCEEULL, // GP0
    0xBB552211BB552211ULL, // GP1
    0xBB00ULL,             // GP2
};

static const uint64_t *byte_patterns[] = {
    byte_pattern_0, byte_pattern_1, byte_pattern_2, byte_pattern_3 // FIXME: use all we have
};
#define NUM_BYTE_PATTERNS ((int)(sizeof(byte_patterns)/sizeof(uint64_t *)))

#define DEFAULT_BYTE_BURSTS 32 // compromise between time and rigor

static void
setup_hw_pattern(int node, int lmc, const uint64_t *pattern_p)
{
    /*
      3) Setup GENERAL_PURPOSE[0-2] registers with the data pattern of choice.
      a. GENERAL_PURPOSE0[DATA<63:0>] â sets the initial lower (rising edge) 64 bits of data.
      b. GENERAL_PURPOSE1[DATA<63:0>] â sets the initial upper (falling edge) 64 bits of data.
      c. GENERAL_PURPOSE2[DATA<15:0>] â sets the initial lower (rising edge <7:0>) and upper
      (falling edge <15:8>) ECC data.
    */
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE0(lmc), pattern_p[0]);
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE1(lmc), pattern_p[1]);
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE2(lmc), pattern_p[2]);
}

#define DEFAULT_PRBS 0xFFFFAAAAUL /* FIXME: maybe try 0x4A519909UL */

static void
setup_lfsr_pattern(int node, int lmc, uint64_t data)
{
    cvmx_lmcx_char_ctl_t char_ctl;
    uint32_t prbs;
    const char *s;

    if ((s = getenv("ddr_lfsr_prbs"))) {
        prbs = simple_strtoul(s, NULL, 0);
    } else
        prbs = DEFAULT_PRBS; // FIXME: from data arg?

    /*
      2) DBTRAIN_CTL[LFSR_PATTERN_SEL] = 1
         here data comes from the LFSR generating a PRBS pattern
         CHAR_CTL.EN = 0
         CHAR_CTL.SEL = 0; // for PRBS
         CHAR_CTL.DR = 1;
         CHAR_CTL.PRBS = setup for whatever type of PRBS to send
         CHAR_CTL.SKEW_ON = 1;
    */
    char_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CHAR_CTL(lmc));
    char_ctl.s.en      = 0;
    char_ctl.s.sel     = 0;
    char_ctl.s.dr      = 1;
    char_ctl.s.prbs    = prbs;
    char_ctl.s.skew_on = 1;
    ddr_config_write_csr_node(node, CVMX_LMCX_CHAR_CTL(lmc), char_ctl.u64);
}

static int
choose_best_hw_patterns(int node, int lmc, int mode, uint32_t cpu_id)
{
    int new_mode = mode;
    const char *s;

    switch (mode) {
    case DBTRAIN_TEST: // always choose LFSR if chip supports it
        if (octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)) {
            int lfsr_enable = 1;
            if ((s = getenv("ddr_allow_lfsr"))) { // override?
                lfsr_enable = !!strtoul(s, NULL, 0);
            }
            if (lfsr_enable)
                new_mode = DBTRAIN_LFSR;
        }
        break;
    case DBTRAIN_DBI: // possibly can allow LFSR use?
        break;
    case DBTRAIN_LFSR: // forced already
        if (!octeon_is_cpuid(OCTEON_CN78XX_PASS2_X)) {
            ddr_print("ERROR: illegal HW assist mode %d\n", mode);
            new_mode = DBTRAIN_TEST;
        }
        break;
    default:
        ddr_print("ERROR: unknown HW assist mode %d\n", mode);
    }

    if (new_mode != mode)
        debug_print("%s: changing mode %d to %d\n", __FUNCTION__, mode, new_mode);

    return new_mode;
}

int
run_best_hw_patterns(uint32_t cpu_id, int node, int lmc, uint64_t phys_addr,
                     int mode, uint64_t *xor_data)
{
    int pattern;
    const uint64_t *pattern_p;
    int errs, errors = 0;

    // FIXME? always choose LFSR if chip supports it???
    mode = choose_best_hw_patterns(node, lmc, mode, cpu_id);

    if (mode == DBTRAIN_LFSR) {
            setup_lfsr_pattern(node, lmc, 0);
            errors = test_dram_byte_hw(node, cpu_id, lmc, phys_addr, mode, xor_data);
            debug_print("%s: LFSR at A:0x%012lx errors 0x%x\n",
                      __FUNCTION__, phys_addr, errors);
    } else {
        for (pattern = 0; pattern < NUM_BYTE_PATTERNS; pattern++) {
            pattern_p = byte_patterns[pattern];
            setup_hw_pattern(node, lmc, pattern_p);

            errs = test_dram_byte_hw(node, cpu_id, lmc, phys_addr, mode, xor_data);
            debug_print("%s: PATTERN %d at A:0x%012lx errors 0x%x\n",
                        __FUNCTION__, pattern, phys_addr, errs);
            errors |= errs;
        } /* for (pattern = 0; pattern < NUM_BYTE_PATTERNS; pattern++) */
    }

    return errors;
}

static void
hw_assist_test_dll_offset(int node, uint32_t cpu_id, int dll_offset_mode,
                          int lmc, int bytelane, int ddr_interface_64b,
                          uint64_t dram_tune_rank_offset, int dram_tune_byte_bursts)
{
    int byte_offset, new_best_offset[9];
    int rank_delay_start[4][9];
    int rank_delay_count[4][9];
    int rank_delay_best_start[4][9];
    int rank_delay_best_count[4][9];
    int errors[4], off_errors, tot_errors;
    int rank_mask, rankx, active_ranks;
    int pattern;
    const uint64_t *pattern_p;
    int byte;
    char *mode_str = (dll_offset_mode == 2) ? "Read" : "Write";
    int pat_best_offset[9];
    uint64_t phys_addr;
    int pat_beg, pat_end;
    int rank_beg, rank_end;
    int byte_lo, byte_hi;
    cvmx_lmcx_config_t lmcx_config;
    uint64_t hw_rank_offset;
    int num_lmcs = cvmx_dram_get_num_lmc(node, cpu_id);
    // FIXME? always choose LFSR if chip supports it???
    int mode = choose_best_hw_patterns(node, lmc, DBTRAIN_TEST, cpu_id);

    if (bytelane == 0x0A) { // all bytelanes
        byte_lo = 0;
        byte_hi = 8;
    } else { // just 1
        byte_lo = byte_hi = bytelane;
    }

    //BDK_CSR_INIT(lmcx_config, node, CVMX_LMCX_CONFIG(lmc));
    lmcx_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));
    rank_mask = lmcx_config.s.init_status;

    // this should be correct for 1 or 2 ranks, 1 or 2 DIMMs
    hw_rank_offset = 1ull << (28 + lmcx_config.s.pbank_lsb - lmcx_config.s.rank_ena + (num_lmcs/2));

    debug_print("N%d: %s: starting LMC%d with rank offset 0x%016llx\n",
                node, __FUNCTION__, lmc, (unsigned long long)hw_rank_offset);

    // start of pattern loop
    // we do the set of tests for each pattern supplied...

    memset(new_best_offset, 0, sizeof(new_best_offset));
    for (pattern = 0; pattern < NUM_BYTE_PATTERNS; pattern++) {

	memset(pat_best_offset, 0, sizeof(pat_best_offset));

        if (mode == DBTRAIN_TEST) {
            pattern_p = byte_patterns[pattern];
            setup_hw_pattern(node, lmc, pattern_p);
        } else {
            setup_lfsr_pattern(node, lmc, 0);
        }

	// now loop through all legal values for the DLL byte offset...

#define BYTE_OFFSET_INCR 3 // FIXME: make this tunable?

	tot_errors = 0;

	memset(rank_delay_count, 0, sizeof(rank_delay_count));
	memset(rank_delay_start, 0, sizeof(rank_delay_start));
	memset(rank_delay_best_count, 0, sizeof(rank_delay_best_count));
	memset(rank_delay_best_start, 0, sizeof(rank_delay_best_start));

	for (byte_offset = -63; byte_offset < 64; byte_offset += BYTE_OFFSET_INCR) {

	    // do the setup on the active LMC
	    // set the bytelanes DLL offsets
	    change_dll_offset_enable(cpu_id, node, lmc, 0);
	    load_dll_offset(node, lmc, cpu_id, dll_offset_mode, byte_offset, bytelane); // FIXME? bytelane?
	    change_dll_offset_enable(cpu_id, node, lmc, 1);

	    //bdk_watchdog_poke();

	    // run the test on each rank
	    // only 1 call per rank should be enough, let the bursts, loops, etc, control the load...
	
	    off_errors = 0; // errors for this byte_offset, all ranks

            active_ranks = 0;

	    for (rankx = 0; rankx < 4; rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;

		phys_addr = hw_rank_offset * active_ranks;
		// FIXME: now done by test_dram_byte_hw()
		//phys_addr |= (lmc << 7);
                //phys_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;

                active_ranks++;

                // NOTE: return is a now a bitmask of the erroring bytelanes..
		errors[rankx] = test_dram_byte_hw(node, cpu_id, lmc, phys_addr, mode, NULL);

                // process any errors in the bytelane(s) that are being tested
                for (byte = byte_lo; byte <= byte_hi; byte++) {

                    // check errors
                    if (errors[rankx] & (1 << byte)) { // yes, an error in the byte lane in this rank
                        off_errors |= (1 << byte);

                        ddr_print4("N%d.LMC%d.R%d: Bytelane %d DLL %s Offset Test %3d: Address 0x%012llx errors\n",
                                   node, lmc, rankx, byte, mode_str,
                                   byte_offset, phys_addr);

                        if (rank_delay_count[rankx][byte] > 0) { // had started run
                            ddr_print4("N%d.LMC%d.R%d: Bytelane %d DLL %s Offset Test %3d: stopping a run here\n",
                                       node, lmc, rankx, byte, mode_str, byte_offset);
                            rank_delay_count[rankx][byte] = 0;   // stop now
                        }
                        // FIXME: else had not started run - nothing else to do?
                    } else { // no error in the byte lane
                        if (rank_delay_count[rankx][byte] == 0) { // first success, set run start
                            ddr_print4("N%d.LMC%d.R%d: Bytelane %d DLL %s Offset Test %3d: starting a run here\n",
                                       node, lmc, rankx, byte, mode_str, byte_offset);
                            rank_delay_start[rankx][byte] = byte_offset;
                        }
                        rank_delay_count[rankx][byte] += BYTE_OFFSET_INCR; // bump run length

                        // is this now the biggest window?
                        if (rank_delay_count[rankx][byte] > rank_delay_best_count[rankx][byte]) {
                            rank_delay_best_count[rankx][byte] = rank_delay_count[rankx][byte];
                            rank_delay_best_start[rankx][byte] = rank_delay_start[rankx][byte];
                            debug_print("N%d.LMC%d.R%d: Bytelane %d DLL %s Offset Test %3d: updating best to %d/%d\n",
                                        node, lmc, rankx, byte, mode_str, byte_offset,
                                        rank_delay_best_start[rankx][byte], rank_delay_best_count[rankx][byte]);
                        }
                    }
                } /* for (byte = byte_lo; byte <= byte_hi; byte++) */
	    } /* for (rankx = 0; rankx < 4; rankx++) */

	    tot_errors |= off_errors;

	} /* for (byte_offset = -63; byte_offset < 64; byte_offset += BYTE_OFFSET_INCR) */

	// now choose the best byte_offsets for this pattern according to the best windows of the tested ranks
        // calculate offset by constructing an average window from the rank windows
        for (byte = byte_lo; byte <= byte_hi; byte++) {

            pat_beg = -999;
            pat_end = 999;

            for (rankx = 0; rankx < 4; rankx++) {
                if (!(rank_mask & (1 << rankx)))
                    continue;

                rank_beg = rank_delay_best_start[rankx][byte];
                pat_beg = max(pat_beg, rank_beg);
                rank_end = rank_beg + rank_delay_best_count[rankx][byte] - BYTE_OFFSET_INCR;
                pat_end = min(pat_end, rank_end);

                ddr_print3("N%d.LMC%d.R%d: Bytelane %d DLL %s Offset Test:  Rank Window %3d:%3d\n",
                           node, lmc, rankx, byte, mode_str, rank_beg, rank_end);

            } /* for (rankx = 0; rankx < 4; rankx++) */

            pat_best_offset[byte] = (pat_end + pat_beg) / 2;
            ddr_print3("N%d.LMC%d: Bytelane %d DLL %s Offset Test:  Pattern %d Average %3d\n",
                       node, lmc, byte, mode_str, pattern, pat_best_offset[byte]);

#if 0
            // FIXME: next print the window counts
            sprintf(sbuffer, "N%d.LMC%d Pattern %d: DLL %s Offset Count ",
                    node, lmc, pattern, mode_str);
            printf("%-45s : ", sbuffer);
            printf(" %3d", byte_delay_best_count);
            printf("\n");
#endif

            new_best_offset[byte] += pat_best_offset[byte]; // sum the pattern averages
        } /* for (byte = byte_lo; byte <= byte_hi; byte++) */
    } /* for (pattern = 0; pattern < NUM_BYTE_PATTERNS; pattern++) */
    // end of pattern loop

    ddr_print("N%d.LMC%d: HW DLL %s Offset Amount   : ",
              node, lmc, mode_str);

    for (byte = byte_hi; byte >= byte_lo; --byte) { // print in decending byte index order
        new_best_offset[byte] = divide_nint(new_best_offset[byte], NUM_BYTE_PATTERNS); // create the new average NINT

        // print the best offsets from all patterns

        if (bytelane == 0x0A) // print just the offset of all the bytes
            ddr_print("%5d ", new_best_offset[byte]);
        else // print the bytelanes also
            ddr_print("%5d.%d ", new_best_offset[byte], byte);
        

#if 1
        // done with testing, load up the best offsets we found...
        change_dll_offset_enable(cpu_id, node, lmc, 0); // disable offsets while we load...
        load_dll_offset(node, lmc, cpu_id, dll_offset_mode, new_best_offset[byte], byte); // FIXME
        change_dll_offset_enable(cpu_id, node, lmc, 1); // re-enable the offsets now that we are done loading
#endif
    } /* for (byte = byte_hi; byte >= byte_lo; --byte) */

    ddr_print("\n");
}

/*
 * Automatically adjust the DLL offset for the selected bytelane using hardware-assist
 */
static int
perform_HW_dll_offset_tuning(int node, uint32_t cpu_id, int dll_offset_mode, int bytelane)
{
    int ddr_interface_64b;
    int save_ecc_ena[4];
    cvmx_lmcx_config_t lmc_config;
    int lmc, num_lmcs = cvmx_dram_get_num_lmc(node, cpu_id);
    const char *s;
    //cvmx_lmcx_comp_ctl2_t comp_ctl2;
    int loops = 1, loop;
    int by;
    uint64_t dram_tune_rank_offset;
    int dram_tune_byte_bursts = DEFAULT_BYTE_BURSTS;


    // see if we want to do the tuning more than once per LMC...
    if ((s = getenv("ddr_tune_ecc_loops"))) {
	loops = strtoul(s, NULL, 0);
    }

    // allow override of the test repeats (bursts)
    if ((s = getenv("ddr_tune_byte_bursts")) != NULL) {
        dram_tune_byte_bursts = strtoul(s, NULL, 10);
    }

    // print current working values
    ddr_print2("N%d: H/W Tuning for bytelane %d will use %d loops, %d bursts, and %d patterns.\n",
	      node, bytelane, loops, dram_tune_byte_bursts,
	      NUM_BYTE_PATTERNS);

    // FIXME? get flag from LMC0 only
    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(0));
    ddr_interface_64b = !lmc_config.s.mode32b;

    // this should be correct for 1 or 2 ranks, 1 or 2 DIMMs
    dram_tune_rank_offset = 1ull << (28 + lmc_config.s.pbank_lsb - lmc_config.s.rank_ena + (num_lmcs/2));

    // do once for each active LMC

    for (lmc = 0; lmc < num_lmcs; lmc++) {

	ddr_print4("N%d: H/W Tuning: starting LMC%d bytelane %d tune.\n", node, lmc, bytelane);

	/* Enable ECC for the HW tests */
	// NOTE: we do enable ECC, but the HW tests used will not generate "visible" errors
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));
	save_ecc_ena[lmc] = lmc_config.s.ecc_ena;
	lmc_config.s.ecc_ena = 1;
	ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(lmc), lmc_config.u64);
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));

	// testing is done on a single LMC at a time
	// FIXME: for now, loop here to show what happens multiple times
	for (loop = 0; loop < loops; loop++) {
	    /* Perform DLL offset tuning */
	    //hw_assist_test_dll_offset(node, cpu_id, 1 /* 1=write */, lmc, bytelane, ddr_interface_64b);
	    hw_assist_test_dll_offset(node,  cpu_id, 2 /* 2=read */, lmc, bytelane,
                                      ddr_interface_64b, dram_tune_rank_offset,
                                      dram_tune_byte_bursts);
	}

	// perform cleanup on active LMC   
	ddr_print4("N%d: H/W Tuning: finishing LMC%d bytelane %d tune.\n", node, lmc, bytelane);

	/* Restore ECC for DRAM tests */
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));
	lmc_config.s.ecc_ena = save_ecc_ena[lmc];
	ddr_config_write_csr_node(node, CVMX_LMCX_CONFIG(lmc), lmc_config.u64);
	lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));

	// finally, see if there are any read offset overrides after tuning
	for (by = 0; by < 9; by++) {
	    if ((s = lookup_env_parameter("ddr%d_tune_byte%d", lmc, by)) != NULL) {
		int dllro = strtoul(s, NULL, 10);
		change_dll_offset_enable(cpu_id, node, lmc, 0);
		load_dll_offset(node, lmc, cpu_id, 2 /* 2=read */, dllro, by); // FIXME
		change_dll_offset_enable(cpu_id, node, lmc, 1);
	    }
	}

    } /* for (lmc = 0; lmc < num_lmcs; lmc++) */

    // finish up...

    return 0;

} /* perform_HW_dll_offset_tuning */

// this routine simply makes the calls to the tuning routine and returns any errors
static int cvmx_tune_node(int node, uint32_t cpu_id)
{
    int errs, tot_errs;
    int do_dllwo = 0; // default to NO
    const char *str;

    // Automatically tune the data and ECC byte DLL read offsets
    ddr_print("N%d: Starting DLL Read Offset Tuning for LMCs\n", node);
    errs = perform_HW_dll_offset_tuning(node, cpu_id, 2, 0x0A/* all bytelanes */); 
    ddr_print("N%d: Finished DLL Read Offset Tuning for LMCs, %d errors\n",
              node, errs);
    tot_errs = errs;

    // disabled by default for now, does not seem to be needed?
    // Automatically tune the data and ECC byte DLL write offsets
    // allow override of default setting
    str = getenv("ddr_tune_write_offsets");
    if (str)
	do_dllwo = !!strtoul(str, NULL, 0);
    if (do_dllwo) {
	ddr_print("N%d: Starting DLL Write Offset Tuning for LMCs\n", node);
        errs = perform_HW_dll_offset_tuning(node, cpu_id, 1, 0x0A/* all bytelanes */); 
	ddr_print("N%d: Finished DLL Write Offset Tuning for LMCs, %d errors\n",
                  node, errs);
	tot_errs += errs;
    }

    return tot_errs;
}

// this routine makes the calls to the tuning routines when criteria are met
// intended to be called for automated tuning, to apply filtering...

#define IS_DDR4  1
#define IS_DDR3  0
#define IS_RDIMM 1
#define IS_UDIMM 0
#define IS_1SLOT 1
#define IS_2SLOT 0

// FIXME: DDR3 is not tuned
static const uint32_t ddr_speed_filter[2][2][2] = {
    [IS_DDR4] = { 
	[IS_RDIMM] = {
	    [IS_1SLOT] =  940,
	    [IS_2SLOT] =  800
	},
	[IS_UDIMM] = {
	    [IS_1SLOT] = 1050,
	    [IS_2SLOT] =  940
	}, 
    },
    [IS_DDR3] = {
	[IS_RDIMM] = {
	    [IS_1SLOT] =    0, // disabled
	    [IS_2SLOT] =    0  // disabled
	},
	[IS_UDIMM] = {
	    [IS_1SLOT] =    0, // disabled
	    [IS_2SLOT] =    0  // disabled
	}
    }
};

static void cvmx_maybe_tune_node(int node, uint32_t cpu_id, uint32_t ddr_speed)
{
    const char *str;
    cvmx_lmcx_config_t lmc_config;
    cvmx_lmcx_control_t lmc_control;
    cvmx_lmcx_ddr_pll_ctl_t lmc_ddr_pll_ctl;
    int is_ddr4;
    int is_rdimm;
    int is_1slot;
    int do_tune = 0;
    uint32_t ddr_min_speed;

    ddr_speed = divide_nint(ddr_speed, 1000000); // scale it down from Hz to MHz

    // FIXME: allow an override here so that all configs can be tuned or none
    // If the envvar is defined, always either force it or avoid it accordingly
    if ((str = getenv("ddr_tune_all_configs")) != NULL) {
	do_tune = !!strtoul(str, NULL, 0);
	printf("N%d: DRAM auto-tuning %s.\n", node, (do_tune) ? "forced" : "disabled");
	if (do_tune)
            (void) cvmx_tune_node(node, cpu_id);
        return;
    }

    // filter the tuning calls here...
    // determine if we should/can run automatically for this configuration
    //
    // FIXME: tune only when the configuration indicates it will help:
    //    DDR type, RDIMM or UDIMM, 1-slot or 2-slot, and speed
    //
    lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(0)); // sample LMC0
    lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(0)); // sample LMC0
    lmc_ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0)); // sample LMC0

    is_ddr4  = (lmc_ddr_pll_ctl.s.ddr4_mode != 0);
    is_rdimm = (lmc_control.s.rdimm_ena != 0);
    is_1slot = (lmc_config.s.init_status < 4); // HACK, should do better

    ddr_min_speed = ddr_speed_filter[is_ddr4][is_rdimm][is_1slot];
    do_tune = ((ddr_min_speed != 0) && (ddr_speed > ddr_min_speed));

    ddr_print("N%d: DDR%d %cDIMM %d-slot at %d MHz %s eligible for auto-tuning.\n",
	      node, (is_ddr4)?4:3, (is_rdimm)?'R':'U', (is_1slot)?1:2,
	      ddr_speed, (do_tune)?"is":"is not");

    // call the tuning routine, filtering is done...
    if (do_tune)
        (void) cvmx_tune_node(node, cpu_id);
    return;
}

///////////////////////////////////////////////////////////

/* first pattern example:
   GENERAL_PURPOSE0.DATA == 64'h00ff00ff00ff00ff;
   GENERAL_PURPOSE1.DATA == 64'h00ff00ff00ff00ff;
   GENERAL_PURPOSE0.DATA == 16'h0000;
*/
const uint64_t dbi_pattern[3] = { 0x00ff00ff00ff00ffULL, 0x00ff00ff00ff00ffULL, 0x0000ULL };

// Perform switchover to DBI
static void cvmx_dbi_switchover_interface(int node, uint32_t cpu_id, int lmc)
{
    cvmx_lmcx_modereg_params0_t modereg_params0;
    cvmx_lmcx_modereg_params3_t modereg_params3;
    cvmx_lmcx_phy_ctl_t phy_ctl;
    cvmx_lmcx_config_t lmcx_config;
    cvmx_lmcx_ddr_pll_ctl_t ddr_pll_ctl;
    int rank_mask, rankx, active_ranks;
    uint64_t phys_addr, rank_offset;
    int num_lmcs, errors;
    int dbi_settings[9], byte, unlocked, retries;
    int ecc_ena;
    int rank_max = 1; // FIXME: make this 4 to try all the ranks

    ddr_pll_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_DDR_PLL_CTL(0));

    lmcx_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(lmc));
    rank_mask = lmcx_config.s.init_status;
    ecc_ena = lmcx_config.s.ecc_ena;

    // FIXME: must filter out any non-supported configs
    //        ie, no DDR3, no x4 devices 
    if ((ddr_pll_ctl.s.ddr4_mode == 0) || (lmcx_config.s.mode_x4dev == 1)) {
        ddr_print("N%d.LMC%d: DBI switchover: inappropriate device; EXITING...\n",
                  node, lmc);
        return;
    }

    // this should be correct for 1 or 2 ranks, 1 or 2 DIMMs
    num_lmcs = cvmx_dram_get_num_lmc(node, cpu_id);
    rank_offset = 1ull << (28 + lmcx_config.s.pbank_lsb - lmcx_config.s.rank_ena + (num_lmcs/2));

    ddr_print("N%d.LMC%d: DBI switchover: rank mask 0x%x, rank size 0x%016llx.\n",
	      node, lmc, rank_mask, (unsigned long long)rank_offset);

    /* 1. conduct the current init sequence as usual all the way
         after software write leveling.
     */

    read_DAC_DBI_settings(cpu_id, node, lmc, /*DBI*/0, dbi_settings);

    display_DAC_DBI_settings(node, lmc, /*DBI*/0, ecc_ena, dbi_settings, " INIT");

   /* 2. set DBI related CSRs as below and issue MR write. 
         MODEREG_PARAMS3.WR_DBI=1
         MODEREG_PARAMS3.RD_DBI=1
         PHY_CTL.DBI_MODE_ENA=1
    */
    modereg_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(lmc));

    modereg_params3.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS3(lmc));
    modereg_params3.s.wr_dbi = 1;
    modereg_params3.s.rd_dbi = 1;
    ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS3(lmc), modereg_params3.u64);

    phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(lmc));
    phy_ctl.s.dbi_mode_ena = 1;
    ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(lmc), phy_ctl.u64);

    /*
        there are two options for data to send.  Lets start with (1) and could move to (2) in the future:
        
        1) DBTRAIN_CTL[LFSR_PATTERN_SEL] = 0 (or for older chips where this does not exist)
           set data directly in these reigsters.  this will yield a clk/2 pattern:
           GENERAL_PURPOSE0.DATA == 64'h00ff00ff00ff00ff;
           GENERAL_PURPOSE1.DATA == 64'h00ff00ff00ff00ff;
           GENERAL_PURPOSE0.DATA == 16'h0000;
        2) DBTRAIN_CTL[LFSR_PATTERN_SEL] = 1
           here data comes from the LFSR generating a PRBS pattern
           CHAR_CTL.EN = 0
           CHAR_CTL.SEL = 0; // for PRBS
           CHAR_CTL.DR = 1;
           CHAR_CTL.PRBS = setup for whatever type of PRBS to send
           CHAR_CTL.SKEW_ON = 1;
    */
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE0(lmc), dbi_pattern[0]);
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE1(lmc), dbi_pattern[1]);
    ddr_config_write_csr_node(node, CVMX_LMCX_GENERAL_PURPOSE2(lmc), dbi_pattern[2]);

    /* 
      3. adjust cas_latency (only necessary if RD_DBI is set).
         here is my code for doing this:
 
         if (csr_model.MODEREG_PARAMS3.RD_DBI.value == 1) begin
           case (csr_model.MODEREG_PARAMS0.CL.value)
             0,1,2,3,4: csr_model.MODEREG_PARAMS0.CL.value += 2; // CL 9-13 -> 11-15
             5: begin
                // CL=14, CWL=10,12 gets +2, CLW=11,14 gets +3
                if((csr_model.MODEREG_PARAMS0.CWL.value==1 || csr_model.MODEREG_PARAMS0.CWL.value==3))
                  csr_model.MODEREG_PARAMS0.CL.value = 7; // 14->16
                else
                  csr_model.MODEREG_PARAMS0.CL.value = 13; // 14->17
                end
             6: csr_model.MODEREG_PARAMS0.CL.value = 8; // 15->18
             7: csr_model.MODEREG_PARAMS0.CL.value = 14; // 16->19
             8: csr_model.MODEREG_PARAMS0.CL.value = 15; // 18->21
             default:
             `cn_fatal(("Error mem_cfg (%s) CL (%d) with RD_DBI=1, I am not sure what to do.", 
                        mem_cfg, csr_model.MODEREG_PARAMS3.RD_DBI.value))
           endcase
        end
    */
    if (modereg_params3.s.rd_dbi == 1) {
        int old_cl, new_cl, old_cwl;

        old_cl  = modereg_params0.s.cl;
        old_cwl = modereg_params0.s.cwl;

        switch (old_cl) {
        case 0: case 1: case 2: case 3: case 4: new_cl = old_cl + 2; break; // 9-13->11-15
        // CL=14, CWL=10,12 gets +2, CLW=11,14 gets +3
        case 5: new_cl = ((old_cwl == 1) || (old_cwl == 3)) ? 7 : 13; break;
        case 6: new_cl =  8; break; // 15->18
        case 7: new_cl = 14; break; // 16->19
        case 8: new_cl = 15; break; // 18->21
        default:
            error_print("ERROR: Bad CL value (%d) for DBI switchover.\n", old_cl);
            // FIXME: need to error exit here...
            old_cl = -1;
            new_cl = -1;
            break;
        }
        ddr_print("N%d.LMC%d: DBI switchover: CL ADJ: old_cl 0x%x, old_cwl 0x%x, new_cl 0x%x.\n",
                  node, lmc, old_cl, old_cwl, new_cl);
        modereg_params0.s.cl = new_cl;
        ddr_config_write_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(lmc), modereg_params0.u64);
    }

    /*
      4. issue MRW to MR0 (CL) and MR5 (DBI), using LMC sequence SEQ_CTL[SEQ_SEL] = MRW.
     */
    // Use the default values, from the CSRs fields
    // also, do B-sides for RDIMMs...

    for (rankx = 0; rankx < 4; rankx++) {
        if (!(rank_mask & (1 << rankx)))
            continue;

        // for RDIMMs, B-side writes should get done automatically when the A-side is written
        ddr4_mrw(cpu_id, node, lmc, rankx, -1/* use_default*/,   0/*MRreg*/, 0 /*A-side*/); /* MR0 */
        ddr4_mrw(cpu_id, node, lmc, rankx, -1/* use_default*/,   5/*MRreg*/, 0 /*A-side*/); /* MR5 */

    } /* for (rankx = 0; rankx < 4; rankx++) */

    /*
      5. conduct DBI bit deskew training via the General Purpose R/W sequence (dbtrain).
         may need to run this over and over to get a lock (I need up to 5 in simulation):
         SEQ_CTL[SEQ_SEL] = RW_TRAINING (15)
         DBTRAIN_CTL.CMD_COUNT_EXT = all 1's
         DBTRAIN_CTL.READ_CMD_COUNT = all 1's
         DBTRAIN_CTL.TCCD_SEL = set according to MODEREG_PARAMS3[TCCD_L]
         DBTRAIN_CTL.RW_TRAIN = 1
         DBTRAIN_CTL.READ_DQ_COUNT = dont care
         DBTRAIN_CTL.WRITE_ENA = 1;
         DBTRAIN_CTL.ACTIVATE = 1;
         DBTRAIN_CTL LRANK, PRANK, ROW_A, BG, BA, COLUMN_A = set to a valid address
     */

    // NOW - do the training
    ddr_print("N%d.LMC%d: DBI switchover: TRAINING begins...\n",
                  node, lmc);

    active_ranks = 0;
    for (rankx = 0; rankx < rank_max; rankx++) {
        if (!(rank_mask & (1 << rankx)))
            continue;

        phys_addr = rank_offset * active_ranks;
        // FIXME: now done by test_dram_byte_hw()
        //phys_addr = (lmc << 7);
        //phys_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;

        active_ranks++;

        retries = 0;

#if 0
        phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(lmc));
        phy_ctl.s.phy_reset = 1; // FIXME: this may reset too much?
        // phy_ctl.s.phy_dsk_reset = 1;
        ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(lmc), phy_ctl.u64);
        //perform_octeon3_ddr3_sequence(cpu_id, node, 1<<rankx, lmc, 0x0A); /* LMC Deskew Training */
        //phy_ctl.s.phy_dsk_reset = 0;
        //ddr_config_write_csr_node(node, CVMX_LMCX_PHY_CTL(lmc), phy_ctl.u64);
#endif

restart_training:

        // NOTE: return is a bitmask of the erroring bytelanes - we only print it
        errors = test_dram_byte_hw(node, cpu_id, lmc, phys_addr, DBTRAIN_DBI, NULL);

        ddr_print("N%d.LMC%d: DBI switchover: TEST: rank %d, phys_addr 0x%llx, errors 0x%x.\n",
                  node, lmc, rankx, (unsigned long long)phys_addr, errors);

        // NEXT - check for locking
        unlocked = 0;
        read_DAC_DBI_settings(cpu_id, node, lmc, /*DBI*/0, dbi_settings);

        for (byte = 0; byte < (8+ecc_ena); byte++) {
            unlocked += (dbi_settings[byte] & 1) ^ 1;
        }

        // FIXME: print out the DBI settings array after each rank?
        if (rank_max > 1) // only when doing more than 1 rank
            display_DAC_DBI_settings(node, lmc, /*DBI*/0, ecc_ena, dbi_settings, " RANK");

        if (unlocked > 0) {
            ddr_print("N%d.LMC%d: DBI switchover: LOCK: %d still unlocked.\n",
                      node, lmc, unlocked);
            retries++;
            if (retries < 10) {
                goto restart_training;
            } else {
                ddr_print("N%d.LMC%d: DBI switchover: LOCK: %d retries exhausted.\n",
                          node, lmc, retries);
            }
        }
    } /* for (rankx = 0; rankx < 4; rankx++) */

    // print out the final DBI settings array
    display_DAC_DBI_settings(node, lmc, /*DBI*/0, ecc_ena, dbi_settings, "FINAL");
}

static void cvmx_dbi_switchover(int node, uint32_t cpu_id)
{
    int lmc;
    int num_lmcs = cvmx_dram_get_num_lmc(node, cpu_id);

    for (lmc = 0; lmc < num_lmcs; lmc++) {
        cvmx_dbi_switchover_interface(node, cpu_id, lmc);
    }
}
