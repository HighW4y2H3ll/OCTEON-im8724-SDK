static void perform_ddr_init_sequence(uint32_t cpu_id, int node, int rank_mask,
				       int ddr_interface_num);

void perform_octeon3_ddr3_sequence(uint32_t cpu_id, int node, int rank_mask,
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

#undef DEBUG_PERFORM_DDR3_SEQUENCE
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

	lmc_config.u64 = cvmx_read_csr_node(node,
					    CVMX_LMCX_CONFIG(ddr_interface_num));
	lmc_config.s.rankmask     = rank_mask;
	ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);

	seq_ctl.u64    = 0;

	seq_ctl.s.init_start  = 1;
        seq_ctl.s.seq_sel    = sequence;

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
    ddr_print("Performing LMC sequence: rank_mask=0x%02x, sequence=%d, %s\n",
              rank_mask, sequence, sequence_str[sequence]);
#endif

	if ((s = lookup_env_parameter("ddr_trigger_sequence%d", sequence)) != NULL) {
		int trigger = simple_strtoul(s, NULL, 0);
		if (trigger)
			pulse_gpio_pin(1, 2);
	}

	ddr_config_write_csr_node(node,
				  CVMX_LMCX_SEQ_CTL(ddr_interface_num),
				  seq_ctl.u64);
	cvmx_read_csr_node(node, CVMX_LMCX_SEQ_CTL(ddr_interface_num));

        do {
            cvmx_wait_usec(10); /* Wait a while */
            seq_ctl.u64 = cvmx_read_csr_node(node,
					     CVMX_LMCX_SEQ_CTL(ddr_interface_num));
        } while (seq_ctl.s.seq_complete != 1);
}

#ifdef ENABLE_DISPLAY_MPR_PAGE
void ddr4_mpr_read(uint32_t cpu_id, int node, int ddr_interface_num, int rank,
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

    mpr_data[0] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA0(ddr_interface_num));
    debug_print("mpr_data0: 0x%016llx\n", mpr_data[0]);
    mpr_data[1] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA1(ddr_interface_num));
    debug_print("mpr_data1: 0x%016llx\n", mpr_data[1]);
    mpr_data[2] = cvmx_read_csr_node(node,
				     CVMX_LMCX_MPR_DATA2(ddr_interface_num));
    debug_print("mpr_data2: 0x%016llx\n", mpr_data[2]);
}
#endif /* ENABLE_DISPLAY_MPR_PAGE */

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


typedef struct {
    unsigned char *rodt_ohms;
    unsigned char *rtt_nom_ohms;
    unsigned char *rtt_nom_table;
    unsigned char *rtt_wr_ohms;
    unsigned char *dic_ohms;
    short         *drive_strength;
} impedence_values_t;


#define RODT_OHMS_COUNT        8
#define RTT_NOM_OHMS_COUNT     8
#define RTT_NOM_TABLE_COUNT    8
#define RTT_WR_OHMS_COUNT      8
#define DIC_OHMS_COUNT         3
#define DRIVE_STRENGTH_COUNT  15

static unsigned char ddr4_rodt_ohms     [RODT_OHMS_COUNT     ] = {  0,  40,  60, 80, 120, 240, 34, 48 };
static unsigned char ddr4_rtt_nom_ohms  [RTT_NOM_OHMS_COUNT  ] = {  0,  60, 120, 40, 240,  48, 80, 34 };
static unsigned char ddr4_rtt_nom_table [RTT_NOM_TABLE_COUNT ] = {  0,   4,   2,  6,   1,   5,  3,  7 };
static unsigned char ddr4_rtt_wr_ohms   [RTT_WR_OHMS_COUNT   ] = {  0, 120,  40 };
static unsigned char ddr4_dic_ohms      [DIC_OHMS_COUNT      ] = { 34,  48 };
static short         ddr4_drive_strength[DRIVE_STRENGTH_COUNT] = {  0,   0,  26, 30,  34,  40, 48, 68, 0,0,0,0,0,0,0 };
static impedence_values_t ddr4_impedence_values = {
    .rodt_ohms             =  ddr4_rodt_ohms     ,
    .rtt_nom_ohms          =  ddr4_rtt_nom_ohms  ,
    .rtt_nom_table         =  ddr4_rtt_nom_table ,
    .rtt_wr_ohms           =  ddr4_rtt_wr_ohms   ,
    .dic_ohms              =  ddr4_dic_ohms      ,
    .drive_strength        =  ddr4_drive_strength,
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
};


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
    ulong tclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, ddr_hertz); /* Clock in psecs */
    ulong eclk_psecs = divide_nint((uint64_t) 1000*1000*1000*1000, cpu_hertz); /* Clock in psecs */

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
    int wlevel_loops = 0;
    int default_rtt_nom[4];
    int dyn_rtt_nom_mask;
    impedence_values_t *imp_values;
#pragma pack(pop)

#if 1
    PRINT_CURSP("init_octeon3_ddr3_interface");
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

    if (ddr_interface_64b == 0) {
        if ( !(octeon_is_cpuid(OCTEON_CN70XX))) {
            error_print("32-bit interface width is not supported for this Octeon model\n");
            ++fatal_error;
        }
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
        imp_values = &ddr4_impedence_values;

        spd_addr = read_spd(&dimm_config_table[0], 0, DDR4_SPD_ADDRESSING_ROW_COL_BITS);
        spd_org = read_spd(&dimm_config_table[0], 0, DDR4_SPD_MODULE_ORGANIZATION);
        spd_banks = read_spd(&dimm_config_table[0], 0, DDR4_SPD_DENSITY_BANKS) & 0xff;

        bank_bits = (2 + ((spd_banks >> 4) & 0x3)) + ((spd_banks >> 6) & 0x3);
        bank_bits = min((int)bank_bits, 4); /* Controller can only address 4 bits. */
    } else {
        imp_values = &ddr3_impedence_values;

        spd_addr = read_spd(&dimm_config_table[0], 0, DDR3_SPD_ADDRESSING_ROW_COL_BITS);
        spd_org = read_spd(&dimm_config_table[0], 0, DDR3_SPD_MODULE_ORGANIZATION);
        spd_banks = read_spd(&dimm_config_table[0], 0, DDR3_SPD_DENSITY_BANKS) & 0xff;

        bank_bits = 3 + ((spd_banks >> 4) & 0x7);
        bank_bits = min((int)bank_bits, 3); /* Controller can only address 3 bits. */
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


    if (ddr_type == DDR4_DRAM) {
        spd_dimm_type   = 0xff & read_spd(&dimm_config_table[0], 0, DDR4_SPD_KEY_BYTE_MODULE_TYPE);
        spd_rdimm       = (spd_dimm_type == 1);
    } else {
        spd_dimm_type   = 0xff & read_spd(&dimm_config_table[0], 0, DDR3_SPD_KEY_BYTE_MODULE_TYPE);
        spd_rdimm       = (spd_dimm_type == 1) || (spd_dimm_type == 5) || (spd_dimm_type == 9);
    }

    if ((s = lookup_env_parameter("ddr_rdimm_ena")) != NULL) {
        spd_rdimm = !!simple_strtoul(s, NULL, 0);
    }

    wlevel_loops = 1;

    if ((s = lookup_env_parameter("ddr%d_wlevel_loops", ddr_interface_num)) != NULL) {
        wlevel_loops = simple_strtoul(s, NULL, 0);
    }

    if ((s = lookup_env_parameter("ddr_ranks")) != NULL) {
        num_ranks = simple_strtoul(s, NULL, 0);
    }

    bunk_enable = (num_ranks > 1);

    column_bits_start = 3;

    row_lsb = column_bits_start + col_bits + bank_bits - (! ddr_interface_64b);
    debug_print("row_lsb = column_bits_start + col_bits + bank_bits = %d\n", row_lsb);

    pbank_lsb = row_lsb + row_bits + bunk_enable;
    debug_print("pbank_lsb = row_lsb + row_bits + bunk_enable = %d\n", pbank_lsb);


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



    ddr_print("row bits: %d, col bits: %d, bank bits: %d, banks: %d, ranks: %d, dram width: %d, size: %d MB\n",
              row_bits, col_bits, bank_bits, num_banks, num_ranks, dram_width, mem_size_mbytes);

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

        ddr_print("%-45s : %6d ps\n", "SDRAM Minimum Cycle Time (tCKAVGmin)",          ddr4_tCKAVGmin);
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

        spd_addr_mirror  = read_spd(&dimm_config_table[0], 0,DDR4_SPD_UDIMM_ADDR_MAPPING_FROM_EDGE) & 0x1;
        debug_print("spd_addr_mirror : %#06x\n", spd_addr_mirror );
        spd_addr_mirror  = spd_addr_mirror && !spd_rdimm; /* Only address mirror unbuffered dimms.  */
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
        twtr            = spd_twtr * mtb_psec;
        trtp            = spd_trtp * mtb_psec;
        tfaw            = spd_tfaw * mtb_psec;

        ddr_print("Medium Timebase (MTB)                         : %6d ps\n", mtb_psec);
        ddr_print("Minimum Cycle Time (tCKmin)                   : %6d ps\n", tCKmin);
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

    // FIXME: no longer support 4 ranks - this saves stack space
    if ((num_ranks != 1) && (num_ranks != 2) /*&& (num_ranks != 4)*/)
    {
        error_print("Unsupported number of ranks: %d\n", num_ranks);
        ++fatal_error;
    }

    if ((dram_width != 8) && (dram_width != 16)) {
        error_print("Unsupported SDRAM Width, %d.  Must be 8 or 16.\n", dram_width);
        ++fatal_error;
    }


    /*
    ** Bail out here if things are not copasetic.
    */
    if (fatal_error)
        return(-1);

    {
        char *s;
        cvmx_lmcx_control_t lmc_control;
        cvmx_lmcx_scramble_cfg0_t lmc_scramble_cfg0;
        cvmx_lmcx_scramble_cfg1_t lmc_scramble_cfg1;

        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(0));
        lmc_scramble_cfg0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SCRAMBLE_CFG0(0));
        lmc_scramble_cfg1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_SCRAMBLE_CFG1(0));

        if ((s = lookup_env_parameter_ull("ddr_scramble_cfg0")) != NULL) {
            lmc_scramble_cfg0.u64    = simple_strtoull(s, NULL, 0);
            lmc_control.s.scramble_ena = 1;
        }
        ddr_print("%-45s : 0x%016llx\n", "LMC_SCRAMBLE_CFG0", lmc_scramble_cfg0.u64);

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_SCRAMBLE_CFG0(0),
				  lmc_scramble_cfg0.u64);

        if ((s = lookup_env_parameter_ull("ddr_scramble_cfg1")) != NULL) {
            lmc_scramble_cfg1.u64    = simple_strtoull(s, NULL, 0);
            lmc_control.s.scramble_ena = 1;
        }
        ddr_print("%-45s : 0x%016llx\n", "LMC_SCRAMBLE_CFG1", lmc_scramble_cfg1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_SCRAMBLE_CFG1(0),
				  lmc_scramble_cfg1.u64);

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(0),
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
        lmc_control.s.bprch           = 0;
        lmc_control.s.wodt_bprch      = 0;
        lmc_control.s.rodt_bprch      = 0;

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

        lmc_timing_params0.cn78xx.tzqcs    = divide_roundup(max(64*tclk_psecs, DDR3_ZQCS), (16*tclk_psecs));
        lmc_timing_params0.cn78xx.txpr     = divide_roundup(max(5*tclk_psecs, trfc+10000ull), 16*tclk_psecs);
        lmc_timing_params0.cn78xx.tzqinit  = divide_roundup(max(512*tclk_psecs, 640000ull), (256*tclk_psecs));
        lmc_timing_params0.cn78xx.trp      = trp_value & 0x1f;
        lmc_timing_params0.cn78xx.tcksre   = divide_roundup(max(5*tclk_psecs, 10000ull), tclk_psecs) - 1;

        if (ddr_type == DDR4_DRAM) {
        lmc_timing_params0.cn78xx.tcke     = divide_roundup(max(3*tclk_psecs, (ulong) DDR3_tCKE), tclk_psecs) - 1;
        lmc_timing_params0.cn78xx.tmrd     = divide_roundup((DDR4_tMRD*tclk_psecs), tclk_psecs) - 1;
        lmc_timing_params0.cn78xx.tmod     = divide_roundup(max(24*tclk_psecs, 15000ull), tclk_psecs) - 1;
        lmc_timing_params0.cn78xx.tdllk    = divide_roundup(DDR4_tDLLK, 256);
        } else {
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
        int txp;
        cvmx_lmcx_timing_params1_t lmc_timing_params1;
        lmc_timing_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));


        /* .cn70xx. */
        lmc_timing_params1.s.tmprr    = divide_roundup(DDR3_tMPRR*tclk_psecs, tclk_psecs) - 1;

        lmc_timing_params1.cn78xx.tras     = divide_roundup(tras, tclk_psecs) - 1;
        lmc_timing_params1.cn78xx.trcd     = divide_roundup(trcd, tclk_psecs);
        lmc_timing_params1.cn78xx.twtr     = divide_roundup(twtr, tclk_psecs) - 1;
        lmc_timing_params1.cn78xx.trfc     = divide_roundup(trfc, 8*tclk_psecs);
        if ((ddr_type == DDR4_DRAM) && (octeon_is_cpuid(OCTEON_CN70XX_PASS1_X) ||
                                        octeon_is_cpuid(OCTEON_CN73XX_PASS1_X))) {
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
        lmc_timing_params1.cn78xx.txp      = divide_roundup(max(3*tclk_psecs, (unsigned)txp), tclk_psecs) - 1;

        lmc_timing_params1.cn78xx.twlmrd   = divide_roundup(DDR3_tWLMRD*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.twldqsen = divide_roundup(DDR3_tWLDQSEN*tclk_psecs, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.tfaw     = divide_roundup(tfaw, 4*tclk_psecs);
        lmc_timing_params1.cn78xx.txpdll   = divide_roundup(max(10*tclk_psecs, 24000ull), tclk_psecs) - 1;

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

        lmc_timing_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS1(ddr_interface_num));
        lmc_timing_params2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_TIMING_PARAMS2(ddr_interface_num));
        ddr_print("TIMING_PARAMS2                                : 0x%016llx\n", lmc_timing_params2.u64);

        lmc_timing_params2.s.trrd_l = divide_roundup(ddr4_tRRD_Lmin, tclk_psecs) - 2;
        lmc_timing_params2.s.twtr_l = divide_roundup(max(4*tclk_psecs, 7500ull), tclk_psecs) - 1;
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
        cvmx_lmcx_modereg_params2_t lmc_modereg_params2;

        lmc_modereg_params1.u64 = odt_config[odt_idx].modereg_params1.u64;
        lmc_modereg_params2.u64 = odt_config[odt_idx].modereg_params2.u64;


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
            if ((s = lookup_env_parameter("ddr_rtt_nom_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_nom_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+9));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+9));

                ddr_rtt_nom_auto = 0;
            }
        }

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_rtt_wr_%1d%1d", !!(i&2), !!(i&1))) == NULL)
                s = lookup_env_parameter("ddr%d_rtt_wr_%1d%1d", ddr_interface_num, !!(i&2), !!(i&1));
            if (s != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params1.u64 &= ~((uint64_t)0x3  << (i*12+5));
                lmc_modereg_params1.u64 |=  ( (value & 0x3) << (i*12+5));
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
                  imp_values->rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_11],
                  imp_values->rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_10],
                  imp_values->rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_01],
                  imp_values->rtt_wr_ohms[lmc_modereg_params1.s.rtt_wr_00],
                  lmc_modereg_params1.s.rtt_wr_11,
                  lmc_modereg_params1.s.rtt_wr_10,
                  lmc_modereg_params1.s.rtt_wr_01,
                  lmc_modereg_params1.s.rtt_wr_00);

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

        for (i=0; i<4; ++i) {
            uint64_t value;
            if ((s = lookup_env_parameter("ddr_vref_value_%1d%1d", !!(i&2), !!(i&1))) != NULL) {
                value = simple_strtoul(s, NULL, 0);
                lmc_modereg_params2.u64 &= ~((uint64_t)0x3f  << (i*10+3));
                lmc_modereg_params2.u64 |=  ( (value & 0x3f) << (i*10+3));
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
        }

        ddr_print("MODEREG_PARAMS1                               : 0x%016llx\n", lmc_modereg_params1.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num),
				  lmc_modereg_params1.u64);
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

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_MODEREG_PARAMS3(ddr_interface_num),
				  lmc_modereg_params3.u64);
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
        lmc_nxm.u64 = cvmx_read_csr_node(node, CVMX_LMCX_NXM(ddr_interface_num));

        /* .cn78xx. */
        if (rank_mask & 0x1)
            lmc_nxm.cn78xx.mem_msb_d0_r0 = row_lsb + row_bits - 26;
        if (rank_mask & 0x2)
            lmc_nxm.cn78xx.mem_msb_d0_r1 = row_lsb + row_bits - 26;
        if (rank_mask & 0x4)
            lmc_nxm.cn78xx.mem_msb_d1_r0 = row_lsb + row_bits - 26;
        if (rank_mask & 0x8)
            lmc_nxm.cn78xx.mem_msb_d1_r1 = row_lsb + row_bits - 26;

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

        comp_ctl2.cn70xx.rodt_ctl = odt_config[odt_idx].qs_dic;

        if ((s = lookup_env_parameter("ddr_clk_ctl")) != NULL) {
            comp_ctl2.cn78xx.ck_ctl  = simple_strtoul(s, NULL, 0);
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

        if ((s = lookup_env_parameter("ddr_ctl_ctl")) != NULL) {
            comp_ctl2.cn78xx.control_ctl  = simple_strtoul(s, NULL, 0);
        }

        ddr_print("%-45s : %d, %d ohms\n", "DQX_CTL           ", comp_ctl2.cn78xx.dqx_ctl    , imp_values->drive_strength[comp_ctl2.cn78xx.dqx_ctl    ]);
        ddr_print("%-45s : %d, %d ohms\n", "CK_CTL            ", comp_ctl2.cn78xx.ck_ctl     , imp_values->drive_strength[comp_ctl2.cn78xx.ck_ctl     ]);
        ddr_print("%-45s : %d, %d ohms\n", "CMD_CTL           ", comp_ctl2.cn78xx.cmd_ctl    , imp_values->drive_strength[comp_ctl2.cn78xx.cmd_ctl    ]);
        ddr_print("%-45s : %d, %d ohms\n", "CONTROL_CTL       ", comp_ctl2.cn78xx.control_ctl, imp_values->drive_strength[comp_ctl2.cn78xx.control_ctl]);

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

        if ((s = lookup_env_parameter_ull("ddr_phy_ctl")) != NULL) {
            lmc_phy_ctl.u64    = simple_strtoull(s, NULL, 0);
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
                lmc_dimmx_params.s.rc15 = 4; /* 0 nCK latency adder */

                lmc_dimmx_ddr4_params0.u64 = 0;

                lmc_dimmx_ddr4_params0.s.rc8x = 0;
                lmc_dimmx_ddr4_params0.s.rc7x = 0;
                lmc_dimmx_ddr4_params0.s.rc6x = 0;
                lmc_dimmx_ddr4_params0.s.rc5x = 0;
                lmc_dimmx_ddr4_params0.s.rc4x = 0;

#define DIVIDEND_SCALE 100      /* Scale by 100 to avoid rounding error. */
#define ENCODING_BASE 1260

                lmc_dimmx_ddr4_params0.s.rc3x = divide_nint((((2 * 1000000 * DIVIDEND_SCALE) /
                                                              (tclk_psecs * DIVIDEND_SCALE)) - ENCODING_BASE), 20);

                lmc_dimmx_ddr4_params0.s.rc2x = 0;
                lmc_dimmx_ddr4_params0.s.rc1x = 0;

                lmc_dimmx_ddr4_params1.u64 = 0;

                lmc_dimmx_ddr4_params1.s.rcbx = 0;
                lmc_dimmx_ddr4_params1.s.rcax = 0;
                lmc_dimmx_ddr4_params1.s.rc9x = 0;

                lmc_ddr4_dimm_ctl.u64 = 0;
                lmc_ddr4_dimm_ctl.cn70xx.ddr4_dimm0_wmask = 0x7ff;
                lmc_ddr4_dimm_ctl.cn70xx.ddr4_dimm1_wmask = (dimm_count > 1) ? 0x7ff : 0x0000;

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

                ddr_print("DIMM%d Register Control Words       RCBx:RC1x : %x %x %x %x %x %x %x %x %x %x %x\n",
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

        lmc_modereg_params0.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num));

        lmc_modereg_params0.s.dllr = 1;
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);

        perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        lmc_modereg_params0.s.dllr = 0;
        ddr_config_write_csr_node(node,
                                  CVMX_LMCX_MODEREG_PARAMS0(ddr_interface_num),
                                  lmc_modereg_params0.u64);
    }

    /*
     * 4.8.6 LMC Offset Training
     *
     * LMC requires input-receiver offset training.
     *
     * 1. Write LMC(0)_PHY_CTL[DAC_ON] = 1
     */

    {
        cvmx_lmcx_phy_ctl_t lmc_phy_ctl;
        lmc_phy_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_PHY_CTL(ddr_interface_num));
        lmc_phy_ctl.s.dac_on = 1;
        ddr_print("PHY_CTL                                       : 0x%016llx\n", lmc_phy_ctl.u64);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_PHY_CTL(ddr_interface_num),
				  lmc_phy_ctl.u64);
    }

    /*
     * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0B and
     *    LMC(0)_SEQ_CTL[INIT_START] = 1.
     *
     * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
     */
    {
        cvmx_lmcx_comp_ctl2_t lmc_comp_ctl2;
        lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));

        ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                  lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);
	perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask,
				      ddr_interface_num, 0x0B); /* Offset training sequence */
    }

    /*
     * 4.8.7 LMC Internal Vref Training
     *
     * LMC requires input-reference-voltage training.
     *
     * 1. Write LMC(0)_EXT_CONFIG[VREFINT_SEQ_DESKEW] = 0.
     */

    {
        cvmx_lmcx_ext_config_t ext_config;
        ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
        ext_config.s.vrefint_seq_deskew = 0;

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
        ddr_print("Performing LMC sequence: vrefint_seq_deskew = %d\n",
                  ext_config.s.vrefint_seq_deskew);
#endif  /* DEBUG_PERFORM_DDR3_SEQUENCE */

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
				  ext_config.u64);
    }

    /*
     * 2. Write LMC(0)_SEQ_CTL[SEQ_SEL] = 0x0a and
     *    LMC(0)_SEQ_CTL[INIT_START] = 1.
     *
     * 3. Wait for LMC(0)_SEQ_CTL[SEQ_COMPLETE] to be set to 1.
     */

    perform_octeon3_ddr3_sequence(cpu_id, node, rank_mask, ddr_interface_num, 0x0A); /* LMC Internal Vref Training */

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

#ifdef DEBUG_PERFORM_DDR3_SEQUENCE
        ddr_print("Performing LMC sequence: vrefint_seq_deskew = %d\n",
                  ext_config.s.vrefint_seq_deskew);
#endif  /* DEBUG_PERFORM_DDR3_SEQUENCE */

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


    /* LMC(0)_EXT_CONFIG */
    {
        cvmx_lmcx_ext_config_t ext_config;
        ext_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_EXT_CONFIG(ddr_interface_num));
        ext_config.s.vrefint_seq_deskew = 0;
        ext_config.s.read_ena_bprch = 1;
        ext_config.s.read_ena_fprch = 1;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_EXT_CONFIG(ddr_interface_num),
				  ext_config.u64);
        ddr_print("%-45s : 0x%016llx\n", "EXT_CONFIG", ext_config.u64);
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
     * 3. For ×8 parts:
     *
     *    Without changing any other fields in LMC(0)_WLEVEL_CTL, write
     *    LMC(0)_WLEVEL_CTL[LANEMASK] to select all byte lanes with attached
     *    DRAM.
     *
     *    For ×16 parts:
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
     * 6. For ×16 parts:
     *
     *    Without changing any other fields in LMC(0)_WLEVEL_CTL, write
     *    LMC(0)_WLEVEL_CTL[LANEMASK] to select all odd byte lanes with
     *    attached DRAM.
     *
     *    Repeat substeps 4 and 5 with this new LMC(0)_WLEVEL_CTL[LANEMASK]
     *    setting. Skip to substep 7 if this has already been done.
     *
     *    For ×8 parts:
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
        cvmx_lmcx_wlevel_ctl_t wlevel_ctl;
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        int rankx = 0;
        int wlevel_bitmask[9];
        int byte_idx;
        int passx;
        int ecc_ena;
        int ddr_wlevel_roundup = 0;
        int save_mode32b;

        if (wlevel_loops)
            ddr_print("Performing Write-Leveling\n");
        else
            wlevel_bitmask_errors = 1; /* Force software write-leveling to run */

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        save_mode32b = lmc_config.cn78xx.mode32b;
        lmc_config.cn78xx.mode32b         = (! ddr_interface_64b);
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);

        while(wlevel_loops--) {
        if ((s = lookup_env_parameter("ddr_wlevel_roundup")) != NULL) {
            ddr_wlevel_roundup = simple_strtoul(s, NULL, 0);
        }
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            if (!(rank_mask & (1 << rankx)))
                continue;

            wlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_CTL(ddr_interface_num));
            lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
            ecc_ena = lmc_config.cn78xx.ecc_ena;

            if ((s = lookup_env_parameter("ddr_wlevel_rtt_nom")) != NULL) {
                wlevel_ctl.cn78xx.rtt_nom   = simple_strtoul(s, NULL, 0);
            }

            ddr_config_write_csr_node(node,
				      CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
				      0); /* Clear write-level delays */

            wlevel_bitmask_errors = 0; /* Reset error counter */

            for (byte_idx=0; byte_idx<9; ++byte_idx) {
                wlevel_bitmask[byte_idx] = 0; /* Reset bitmasks */
            }

            /* Make separate passes for each byte to reduce power. */
            for (passx=0; passx<(8+ecc_ena); ++passx) {

                if (!(ddr_interface_bytemask&(1<<passx)))
                    continue;

                wlevel_ctl.cn78xx.lanemask = (1<<passx);

                ddr_config_write_csr_node(node,
					  CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
					  wlevel_ctl.u64);

                /* Read and write values back in order to update the
                   status field. This insurs that we read the updated
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

                wlevel_bitmask[passx] = octeon_read_lmcx_ddr3_wlevel_dbg(node, ddr_interface_num, passx);
                if (wlevel_bitmask[passx] == 0)
                    ++wlevel_bitmask_errors;
            } /* for (passx=0; passx<(8+ecc_ena); ++passx) */

             /* Restore lanemask default to avoid potential side-effects during read-leveling */
            wlevel_ctl.cn78xx.lanemask = 0x1ff;
            ddr_config_write_csr_node(node,
                                      CVMX_LMCX_WLEVEL_CTL(ddr_interface_num),
                                      wlevel_ctl.u64);

            ddr_print("Rank(%d) Wlevel Debug Results                  : %05x %05x %05x %05x %05x %05x %05x %05x %05x\n",
                      rankx,
                      wlevel_bitmask[8],
                      wlevel_bitmask[7],
                      wlevel_bitmask[6],
                      wlevel_bitmask[5],
                      wlevel_bitmask[4],
                      wlevel_bitmask[3],
                      wlevel_bitmask[2],
                      wlevel_bitmask[1],
                      wlevel_bitmask[0]
                      );

            ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                      rankx,
                      lmc_wlevel_rank.cn78xx.status,
                      lmc_wlevel_rank.u64,
                      lmc_wlevel_rank.cn78xx.byte8,
                      lmc_wlevel_rank.cn78xx.byte7,
                      lmc_wlevel_rank.cn78xx.byte6,
                      lmc_wlevel_rank.cn78xx.byte5,
                      lmc_wlevel_rank.cn78xx.byte4,
                      lmc_wlevel_rank.cn78xx.byte3,
                      lmc_wlevel_rank.cn78xx.byte2,
                      lmc_wlevel_rank.cn78xx.byte1,
                      lmc_wlevel_rank.cn78xx.byte0
                      );

            if (ddr_wlevel_roundup) { /* Round up odd bitmask delays */
                for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                    if (!(ddr_interface_bytemask&(1<<byte_idx)))
                        continue;
                    update_wlevel_rank_struct(&lmc_wlevel_rank,
                                           byte_idx,
                                           roundup_ddr3_wlevel_bitmask(wlevel_bitmask[byte_idx]));
                }
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
					  lmc_wlevel_rank.u64);
                ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_wlevel_rank.cn78xx.status,
                          lmc_wlevel_rank.u64,
                          lmc_wlevel_rank.cn78xx.byte8,
                          lmc_wlevel_rank.cn78xx.byte7,
                          lmc_wlevel_rank.cn78xx.byte6,
                          lmc_wlevel_rank.cn78xx.byte5,
                          lmc_wlevel_rank.cn78xx.byte4,
                          lmc_wlevel_rank.cn78xx.byte3,
                          lmc_wlevel_rank.cn78xx.byte2,
                          lmc_wlevel_rank.cn78xx.byte1,
                          lmc_wlevel_rank.cn78xx.byte0
                          );
            }

            if (wlevel_bitmask_errors != 0) {
                ddr_print("Rank(%d) Write-Leveling Failed: %d Bitmask errors\n", rankx, wlevel_bitmask_errors);
            }
        } /* for (rankx = 0; rankx < dimm_count * 4;rankx++) */
        } /* while(wlevel_loops--) */

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        lmc_config.cn78xx.mode32b         = save_mode32b;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);
    }

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
        char default_rodt_ctl;
        unsigned char save_ddr2t;
        int rlevel_avg_loops;
        char ddr_rlevel_compute;
        char saved_ddr__ptune, saved_ddr__ntune, rlevel_comp_offset;
        char saved_int_zqcs_dis = 0;
        char disable_sequential_delay_check = 0;
        int maximum_adjacent_rlevel_delay_increment = 0;
        // FIXME: no longer support 4 ranks - this saves stack space
        struct {
            uint64_t setting;
            int      score;
        } rlevel_scoreboard[RTT_NOM_OHMS_COUNT][RODT_OHMS_COUNT][2];
#pragma pack(pop)

        default_rodt_ctl = odt_config[odt_idx].qs_dic;

        lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
        save_ddr2t                    = lmc_control.cn78xx.ddr2t;

        lmc_config.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        ecc_ena = lmc_config.cn78xx.ecc_ena;

        {
            int save_ref_zqcs_int;
            uint64_t temp_delay_usecs;

            /* Temporarily select the minimum ZQCS interval and wait
               long enough for a few ZQCS calibrations to occur.  This
               should ensure that the calibration circuitry is
               stabilized before read-leveling occurs. */
            save_ref_zqcs_int         = lmc_config.cn78xx.ref_zqcs_int;
            lmc_config.cn78xx.ref_zqcs_int = 1 | (32<<7); /* set smallest interval */
            cvmx_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num),
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

            lmc_config.cn78xx.ref_zqcs_int = save_ref_zqcs_int; /* Restore computed interval */
            cvmx_write_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num), lmc_config.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_CONFIG(ddr_interface_num));
        }

        if ((s = lookup_env_parameter("ddr_rlevel_2t")) != NULL) {
            lmc_control.cn78xx.ddr2t = simple_strtoul(s, NULL, 0);
        }

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONTROL(ddr_interface_num),
				  lmc_control.u64);

        ddr_print("Performing Read-Leveling\n");

        rlevel_ctl.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_CTL(ddr_interface_num));

        rlevel_avg_loops = custom_lmc_config->rlevel_average_loops;
        if (rlevel_avg_loops == 0) rlevel_avg_loops = 1;

        ddr_rlevel_compute = custom_lmc_config->rlevel_compute;
        rlevel_ctl.cn78xx.offset_en = custom_lmc_config->offset_en;
        rlevel_ctl.cn78xx.offset    = spd_rdimm
            ? custom_lmc_config->offset_rdimm
            : custom_lmc_config->offset_udimm;

        rlevel_ctl.cn78xx.delay_unload_0 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_1 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_2 = 1; /* should normally be set */
        rlevel_ctl.cn78xx.delay_unload_3 = 1; /* should normally be set */

        {
            int byte_bitmask = 0xff;

            /* If we will be switching to 32bit mode level based on only
               four bits because there are only 4 ECC bits. */
            if (! ddr_interface_64b)
                byte_bitmask = 0x0f;

            rlevel_ctl.cn78xx.or_dis   = (! ddr_interface_64b);
            rlevel_ctl.cn78xx.bitmask  = byte_bitmask;
        }

        ddr_rodt_ctl_auto = custom_lmc_config->ddr_rodt_ctl_auto;

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

        if ((s = lookup_env_parameter("ddr_rodt_ctl")) == NULL)
            s = lookup_env_parameter("ddr%d_rodt_ctl", ddr_interface_num);
        if (s != NULL) {
            default_rodt_ctl    = simple_strtoul(s, NULL, 0);
            ddr_rodt_ctl_auto = 0;
        }

        if ((s = lookup_env_parameter("ddr_rodt_ctl_auto")) != NULL) {
            ddr_rodt_ctl_auto = !!simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_average")) != NULL) {
            rlevel_avg_loops = simple_strtoul(s, NULL, 0);
        }

        if ((s = lookup_env_parameter("ddr_rlevel_compute")) != NULL) {
            ddr_rlevel_compute = simple_strtoul(s, NULL, 0);
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

        while(rlevel_debug_loops--) {
        /* Initialize the error scoreboard */
        memset(rlevel_scoreboard, 0, sizeof(rlevel_scoreboard));

        if ((s = lookup_env_parameter("ddr_rlevel_comp_offset")) != NULL) {
            rlevel_comp_offset = simple_strtoul(s, NULL, 0);
        }

        disable_sequential_delay_check = custom_lmc_config->disable_sequential_delay_check;

        if ((s = lookup_env_parameter("ddr_disable_sequential_delay_check")) != NULL) {
            disable_sequential_delay_check = simple_strtoul(s, NULL, 0);
        }

        maximum_adjacent_rlevel_delay_increment = custom_lmc_config->maximum_adjacent_rlevel_delay_increment;

        if ((s = lookup_env_parameter("ddr_maximum_adjacent_rlevel_delay_increment")) != NULL) {
            maximum_adjacent_rlevel_delay_increment = simple_strtoul(s, NULL, 0);
        }

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

            lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
            saved_int_zqcs_dis = lmc_control.cn78xx.int_zqcs_dis;
            lmc_control.cn78xx.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_CONTROL(ddr_interface_num),
				      lmc_control.u64);

            lmc_comp_ctl2.cn78xx.byp = 1; /* Enable bypass mode */
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				      lmc_comp_ctl2.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
            ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                      lmc_comp_ctl2.cn78xx.ddr__ptune, lmc_comp_ctl2.cn78xx.ddr__ntune);
        }

        lmc_modereg_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

        for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) {
            rtt_nom = imp_values->rtt_nom_table[rtt_idx];

            /* When the read ODT mask is zero the dyn_rtt_nom_mask is
               zero than RTT_NOM will not be changing during
               read-leveling.  Since the value is fixed we only need
               to test it once. */
            if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
                continue;

            if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.cn78xx.rtt_nom_00 = rtt_nom;
            if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.cn78xx.rtt_nom_01 = rtt_nom;
            if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.cn78xx.rtt_nom_10 = rtt_nom;
            if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.cn78xx.rtt_nom_11 = rtt_nom;

            ddr_config_write_csr_node(node,
				      CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num),
				      lmc_modereg_params1.u64);
            ddr_print("\n");
            ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_11],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_10],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_01],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_00],
                      lmc_modereg_params1.cn78xx.rtt_nom_11,
                      lmc_modereg_params1.cn78xx.rtt_nom_10,
                      lmc_modereg_params1.cn78xx.rtt_nom_01,
                      lmc_modereg_params1.cn78xx.rtt_nom_00);

            perform_ddr_init_sequence(cpu_id, node, rank_mask, ddr_interface_num);

        for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) {
            rlevel_rodt_errors = 0;
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            lmc_comp_ctl2.cn78xx.rodt_ctl = rodt_ctl;
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				      lmc_comp_ctl2.u64);
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                      lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);

            for (rankx = 0; rankx < dimm_count*4; rankx++) {
                int byte_idx;
                rlevel_byte_data_t rlevel_byte[9];
                int average_loops;
                int rlevel_rank_errors;

                if (!(rank_mask & (1 << rankx)))
                    continue;

                memset(rlevel_byte, 0, sizeof(rlevel_byte));

                average_loops = rlevel_avg_loops;
                while (average_loops--) {
                    rlevel_rank_errors = 0;

                    /* Clear read-level delays */
                    ddr_config_write_csr_node(node,
					      CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num),
					      0);

                    perform_octeon3_ddr3_sequence(cpu_id, node, 1 << rankx,
						  ddr_interface_num, 1); /* read-leveling */

                    do {
                        lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                    } while (lmc_rlevel_rank.cn78xx.status != 3);

                    {
                        rlevel_bitmask_t rlevel_bitmask[9];
                        memset(rlevel_bitmask, 0, sizeof(rlevel_bitmask));

                        /* Evaluate the quality of the read-leveling
                           delays. Also save off a software computed
                           read-leveling mask that may be used later
                           to qualify the delay results from Octeon. */
                        for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                            if (!(ddr_interface_bytemask&(1<<byte_idx)))
                                continue;
                            rlevel_bitmask[byte_idx].bm = octeon_read_lmcx_ddr3_rlevel_dbg(node, ddr_interface_num, byte_idx);
                            rlevel_rank_errors +=
                                validate_ddr3_rlevel_bitmask(&rlevel_bitmask[byte_idx], ddr_type);
                        }

                        /* Set delays for unused bytes to match byte 0. */
                        for (byte_idx=0; byte_idx<9; ++byte_idx) {
                            if (ddr_interface_bytemask&(1<<byte_idx))
                                continue;
                            update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx, lmc_rlevel_rank.cn78xx.byte0);
                        }

                        /* Save a copy of the byte delays in physical
                           order for sequential evaluation. */
                        if ((ddr_interface_bytemask&0xff) == 0xff) {
                        if (ecc_ena) {
                            rlevel_byte[8].delay = lmc_rlevel_rank.cn78xx.byte7;
                            rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte6;
                            rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte5;
                            rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte4;
                            rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte8; /* ECC */
                        } else {
                            rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte7;
                            rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte6;
                            rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte5;
                            rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte4;
                        }
                        } else {
                            rlevel_byte[8].delay = lmc_rlevel_rank.cn78xx.byte8; /* unused */
                            rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte7; /* unused */
                            rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte6; /* unused */
                            rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte5; /* unused */
                            rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte4; /* ECC */
                        }
                        rlevel_byte[3].delay = lmc_rlevel_rank.cn78xx.byte3;
                        rlevel_byte[2].delay = lmc_rlevel_rank.cn78xx.byte2;
                        rlevel_byte[1].delay = lmc_rlevel_rank.cn78xx.byte1;
                        rlevel_byte[0].delay = lmc_rlevel_rank.cn78xx.byte0;

                        if (! disable_sequential_delay_check) {
                            if ((ddr_interface_bytemask&0xff) == 0xff) {
                                /* Evaluate delay sequence across the whole range of bytes for stantard dimms. */
                                if ((spd_dimm_type == 1) || (spd_dimm_type == 5)) { /* 1=RDIMM, 5=Mini-RDIMM */
                                    /* Registerd dimm topology routes from the center. */
                                    rlevel_rank_errors += nonsequential_delays(rlevel_byte, 0, 3+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                    rlevel_rank_errors += nonsequential_delays(rlevel_byte, 4, 7+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                }
                                if ((spd_dimm_type == 2) || (spd_dimm_type == 6)) { /* 1=UDIMM, 5=Mini-UDIMM */
                                    /* Unbuffered dimm topology routes from end to end. */
                                    rlevel_rank_errors += nonsequential_delays(rlevel_byte, 0, 7+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                                }
                            } else {
                                rlevel_rank_errors += nonsequential_delays(rlevel_byte, 0, 3+ecc_ena, maximum_adjacent_rlevel_delay_increment);
                            }
                        }

                        ddr_print("Rank(%d) Rlevel Debug Test Results  8:0        : %05llx %05llx %05llx %05llx %05llx %05llx %05llx %05llx %05llx\n",
                                  rankx,
                                  rlevel_bitmask[8].bm,
                                  rlevel_bitmask[7].bm,
                                  rlevel_bitmask[6].bm,
                                  rlevel_bitmask[5].bm,
                                  rlevel_bitmask[4].bm,
                                  rlevel_bitmask[3].bm,
                                  rlevel_bitmask[2].bm,
                                  rlevel_bitmask[1].bm,
                                  rlevel_bitmask[0].bm
                                  );

                        ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d)\n",
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
                                  lmc_rlevel_rank.cn78xx.byte0,
                                  rlevel_rank_errors
                                  );

                        if (ddr_rlevel_compute) {
                            /* Recompute the delays based on the bitmask */
                            for (byte_idx=0; byte_idx<(8+ecc_ena); ++byte_idx) {
                                if (!(ddr_interface_bytemask&(1<<byte_idx)))
                                    continue;
                                update_rlevel_rank_struct(&lmc_rlevel_rank, byte_idx,
                                                          compute_ddr3_rlevel_delay(rlevel_bitmask[byte_idx].mstart,
                                                                                    rlevel_bitmask[byte_idx].width,
                                                                                    rlevel_ctl));
                            }

                            /* Override the copy of byte delays with the computed results. */
                            if ((ddr_interface_bytemask&0xff) == 0xff) {
                            if (ecc_ena) {
                                rlevel_byte[8].delay = lmc_rlevel_rank.cn78xx.byte7;
                                rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte6;
                                rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte5;
                                rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte4;
                                rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte8; /* ECC */
                            } else {
                                rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte7;
                                rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte6;
                                rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte5;
                                rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte4;
                            }
                            } else {
                                rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte8; /* unused */
                                rlevel_byte[7].delay = lmc_rlevel_rank.cn78xx.byte7; /* unused */
                                rlevel_byte[6].delay = lmc_rlevel_rank.cn78xx.byte6; /* unused */
                                rlevel_byte[5].delay = lmc_rlevel_rank.cn78xx.byte5; /* unused */
                                rlevel_byte[4].delay = lmc_rlevel_rank.cn78xx.byte4; /* ECC */
                            }
                            rlevel_byte[3].delay = lmc_rlevel_rank.cn78xx.byte3;
                            rlevel_byte[2].delay = lmc_rlevel_rank.cn78xx.byte2;
                            rlevel_byte[1].delay = lmc_rlevel_rank.cn78xx.byte1;
                            rlevel_byte[0].delay = lmc_rlevel_rank.cn78xx.byte0;

                            ddr_print("Rank(%d) Computed delays                       : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                                      rankx,
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
                    }

                    /* Accumulate the total score across averaging loops for this setting */
                    rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score += rlevel_rank_errors;
                    debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d\n",
                                rtt_nom, rodt_ctl, rankx,
                                rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score);

                    /* Accumulate the delay totals and loop counts
                       necessary to compute average delay results */
                    for (byte_idx=0; byte_idx<9; ++byte_idx) {
                        rlevel_byte[byte_idx].loop_total += rlevel_byte[byte_idx].delay;
                        if (rlevel_byte[byte_idx].delay != 0) /* Don't include delay=0 in the average */
                            ++rlevel_byte[byte_idx].loop_count;
                    }

                    rlevel_rodt_errors += rlevel_rank_errors;
                } /* while (rlevel_avg_loops--) */

                /* Compute the average score across averaging loops */
                rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score =
                    divide_nint(rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score, rlevel_avg_loops);

                /* Compute the average delay results */
                for (byte_idx=0; byte_idx<9; ++byte_idx) {
                    if (rlevel_byte[byte_idx].loop_count == 0)
                        rlevel_byte[byte_idx].loop_count = 1;
                    rlevel_byte[byte_idx].delay = divide_nint(rlevel_byte[byte_idx].loop_total,
                                                              rlevel_byte[byte_idx].loop_count);
                }

                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));

                if ((ddr_interface_bytemask&0xff) == 0xff) {
                if (ecc_ena) {
                    lmc_rlevel_rank.cn78xx.byte7 = rlevel_byte[8].delay;
                    lmc_rlevel_rank.cn78xx.byte6 = rlevel_byte[7].delay;
                    lmc_rlevel_rank.cn78xx.byte5 = rlevel_byte[6].delay;
                    lmc_rlevel_rank.cn78xx.byte4 = rlevel_byte[5].delay;
                    lmc_rlevel_rank.cn78xx.byte8 = rlevel_byte[4].delay; /* ECC */
                } else {
                    lmc_rlevel_rank.cn78xx.byte7 = rlevel_byte[7].delay;
                    lmc_rlevel_rank.cn78xx.byte6 = rlevel_byte[6].delay;
                    lmc_rlevel_rank.cn78xx.byte5 = rlevel_byte[5].delay;
                    lmc_rlevel_rank.cn78xx.byte4 = rlevel_byte[4].delay;
                }
                } else {
                    lmc_rlevel_rank.cn78xx.byte8 = rlevel_byte[8].delay;
                    lmc_rlevel_rank.cn78xx.byte7 = rlevel_byte[7].delay;
                    lmc_rlevel_rank.cn78xx.byte6 = rlevel_byte[6].delay;
                    lmc_rlevel_rank.cn78xx.byte5 = rlevel_byte[5].delay;
                    lmc_rlevel_rank.cn78xx.byte4 = rlevel_byte[4].delay;
                }
                lmc_rlevel_rank.cn78xx.byte3 = rlevel_byte[3].delay;
                lmc_rlevel_rank.cn78xx.byte2 = rlevel_byte[2].delay;
                lmc_rlevel_rank.cn78xx.byte1 = rlevel_byte[1].delay;
                lmc_rlevel_rank.cn78xx.byte0 = rlevel_byte[0].delay;

                if (rlevel_avg_loops > 1) {
                    ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d) Average\n\n",
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
                              lmc_rlevel_rank.cn78xx.byte0,
                              rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score
                              );
                }

                rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting = lmc_rlevel_rank.u64;
            } /* for (rankx = 0; rankx < dimm_count*4; rankx++) */
        } /* for (rodt_ctl=odt_config[odt_idx].qs_dic; rodt_ctl>0; --rodt_ctl) */
        } /*  for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) */


        /* Re-enable dynamic compensation settings. */
        if (rlevel_comp_offset != 0) {
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));

            lmc_comp_ctl2.cn78xx.ptune = 0;
            lmc_comp_ctl2.cn78xx.ntune = 0;
            lmc_comp_ctl2.cn78xx.byp = 0; /* Disable bypass mode */
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				      lmc_comp_ctl2.u64);
            cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read once */

            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */
            ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                      lmc_comp_ctl2.cn78xx.ddr__ptune, lmc_comp_ctl2.cn78xx.ddr__ntune);

            lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
            lmc_control.cn78xx.int_zqcs_dis    = saved_int_zqcs_dis; /* Restore original setting */
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_CONTROL(ddr_interface_num),
				      lmc_control.u64);

        }


        {
            int override_compensation = 0;
            if ((s = lookup_env_parameter("ddr__ptune")) != NULL) {
                saved_ddr__ptune = simple_strtoul(s, NULL, 0);
                override_compensation = 1;
            }
            if ((s = lookup_env_parameter("ddr__ntune")) != NULL) {
                saved_ddr__ntune = simple_strtoul(s, NULL, 0);
                override_compensation = 1;
            }
            if (override_compensation) {
                lmc_comp_ctl2.cn78xx.ptune = saved_ddr__ptune;
                lmc_comp_ctl2.cn78xx.ntune = saved_ddr__ntune;

                lmc_control.u64 = cvmx_read_csr_node(node, CVMX_LMCX_CONTROL(ddr_interface_num));
                saved_int_zqcs_dis = lmc_control.cn78xx.int_zqcs_dis;
                lmc_control.cn78xx.int_zqcs_dis    = 1; /* Disable ZQCS while in bypass. */
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_CONTROL(ddr_interface_num),
					  lmc_control.u64);

                lmc_comp_ctl2.cn78xx.byp = 1; /* Enable bypass mode */
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_COMP_CTL2(ddr_interface_num),
					  lmc_comp_ctl2.u64);
                lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num)); /* Read again */

                ddr_print("DDR__PTUNE/DDR__NTUNE                         : %d/%d\n",
                          lmc_comp_ctl2.cn78xx.ptune, lmc_comp_ctl2.cn78xx.ntune);
            }
        }

        {
            int      best_rodt_score = 9999999; /* Start with an arbitrarily high score */
            int      auto_rodt_ctl = 0;
            int      auto_rtt_nom  = 0;
            int rodt_score;

            ddr_print("Evaluating Read-Leveling Scoreboard.\n");
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
                    debug_print("rodt_score:%d, best_rodt_score:%d\n", rodt_score, best_rodt_score);

                    /* When using automatic ODT settings use the ODT
                       settings associated with the best score for
                       all of the tested ODT combinations. */
                    if (rodt_score < best_rodt_score) {
                        best_rodt_score = rodt_score;
                        auto_rodt_ctl   = rodt_ctl;
                        auto_rtt_nom    = rtt_nom;
                    }
                }
            }

            lmc_modereg_params1.u64 = cvmx_read_csr_node(node, CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num));

            if (ddr_rtt_nom_auto) {
                /* Store the automatically set RTT_NOM value */
                if (dyn_rtt_nom_mask & 1) lmc_modereg_params1.cn78xx.rtt_nom_00 = auto_rtt_nom;
                if (dyn_rtt_nom_mask & 2) lmc_modereg_params1.cn78xx.rtt_nom_01 = auto_rtt_nom;
                if (dyn_rtt_nom_mask & 4) lmc_modereg_params1.cn78xx.rtt_nom_10 = auto_rtt_nom;
                if (dyn_rtt_nom_mask & 8) lmc_modereg_params1.cn78xx.rtt_nom_11 = auto_rtt_nom;
            } else {
                /* restore the manual settings to the register */
                lmc_modereg_params1.cn78xx.rtt_nom_00 = default_rtt_nom[0];
                lmc_modereg_params1.cn78xx.rtt_nom_01 = default_rtt_nom[1];
                lmc_modereg_params1.cn78xx.rtt_nom_10 = default_rtt_nom[2];
                lmc_modereg_params1.cn78xx.rtt_nom_11 = default_rtt_nom[3];
            }

            ddr_config_write_csr_node(node,
				      CVMX_LMCX_MODEREG_PARAMS1(ddr_interface_num),
				      lmc_modereg_params1.u64);
            ddr_print("RTT_NOM     %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_11],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_10],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_01],
                      imp_values->rtt_nom_ohms[lmc_modereg_params1.cn78xx.rtt_nom_00],
                      lmc_modereg_params1.cn78xx.rtt_nom_11,
                      lmc_modereg_params1.cn78xx.rtt_nom_10,
                      lmc_modereg_params1.cn78xx.rtt_nom_01,
                      lmc_modereg_params1.cn78xx.rtt_nom_00);

            ddr_print("RTT_WR      %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                      imp_values->rtt_wr_ohms[lmc_modereg_params1.cn78xx.rtt_wr_11],
                      imp_values->rtt_wr_ohms[lmc_modereg_params1.cn78xx.rtt_wr_10],
                      imp_values->rtt_wr_ohms[lmc_modereg_params1.cn78xx.rtt_wr_01],
                      imp_values->rtt_wr_ohms[lmc_modereg_params1.cn78xx.rtt_wr_00],
                      lmc_modereg_params1.cn78xx.rtt_wr_11,
                      lmc_modereg_params1.cn78xx.rtt_wr_10,
                      lmc_modereg_params1.cn78xx.rtt_wr_01,
                      lmc_modereg_params1.cn78xx.rtt_wr_00);

            ddr_print("DIC         %3d, %3d, %3d, %3d ohms           :  %x,%x,%x,%x\n",
                      imp_values->dic_ohms[lmc_modereg_params1.cn78xx.dic_11],
                      imp_values->dic_ohms[lmc_modereg_params1.cn78xx.dic_10],
                      imp_values->dic_ohms[lmc_modereg_params1.cn78xx.dic_01],
                      imp_values->dic_ohms[lmc_modereg_params1.cn78xx.dic_00],
                      lmc_modereg_params1.cn78xx.dic_11,
                      lmc_modereg_params1.cn78xx.dic_10,
                      lmc_modereg_params1.cn78xx.dic_01,
                      lmc_modereg_params1.cn78xx.dic_00);

            if (ddr_type == DDR4_DRAM) {
            cvmx_lmcx_modereg_params2_t lmc_modereg_params2;
            lmc_modereg_params2.u64 = odt_config[odt_idx].modereg_params2.u64;
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
                lmc_comp_ctl2.cn78xx.rodt_ctl = default_rodt_ctl;
            ddr_config_write_csr_node(node,
				      CVMX_LMCX_COMP_CTL2(ddr_interface_num),
				      lmc_comp_ctl2.u64);
            lmc_comp_ctl2.u64 = cvmx_read_csr_node(node, CVMX_LMCX_COMP_CTL2(ddr_interface_num));
            ddr_print("Read ODT_CTL                                  : 0x%x (%d ohms)\n",
                      lmc_comp_ctl2.cn78xx.rodt_ctl, imp_values->rodt_ohms[lmc_comp_ctl2.cn78xx.rodt_ctl]);

            /* Use the delays associated with the best score for each individual rank */
            for (rankx = 0; rankx < dimm_count * 4;rankx++) {
                int best_rank_score = 9999999; /* Start with an arbitrarily high score */
                if (!(rank_mask & (1 << rankx)))
                    continue;

                for (rtt_idx=min_rtt_nom_idx; rtt_idx<=max_rtt_nom_idx; ++rtt_idx) {
                    rtt_nom = imp_values->rtt_nom_table[rtt_idx];

                    /* When the read ODT mask is zero the dyn_rtt_nom_mask is
                       zero than RTT_NOM will not be changing during
                       read-leveling.  Since the value is fixed we only need
                       to test it once. */
                    if ((dyn_rtt_nom_mask == 0) && (rtt_idx != min_rtt_nom_idx))
                        continue;

                    for (rodt_ctl=max_rodt_ctl; rodt_ctl>=min_rodt_ctl; --rodt_ctl) {
                        if (rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score <= best_rank_score) {
                            best_rank_score	= rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score;
                            lmc_rlevel_rank.u64	= rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].setting;

                            debug_print("rlevel_scoreboard[rtt_nom=%d][rodt_ctl=%d][rankx=%d].score:%d, best_rank_score:%d\n",
                                        rtt_nom, rodt_ctl, rankx, rlevel_scoreboard[rtt_nom][rodt_ctl][rankx].score,
                                        best_rank_score);
                        }
                    }
                }

                if (!ecc_ena){
                    lmc_rlevel_rank.cn78xx.byte8 = lmc_rlevel_rank.cn78xx.byte0; /* ECC is not used */
                }
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num),
					  lmc_rlevel_rank.u64);
                lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                ddr_print("Rank(%d) Rlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d (%d)\n",
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
                          lmc_rlevel_rank.cn78xx.byte0,
                          best_rank_score
                          );
            }
        }
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
        }
    }

    {
        /* Try to determine/optimize write-level delays experimentally. */
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank;
        cvmx_lmcx_wlevel_rankx_t lmc_wlevel_rank_hw_results;
        cvmx_lmcx_rlevel_rankx_t lmc_rlevel_rank;
        cvmx_lmcx_config_t lmc_config;
        int byte;
        int delay;
        int rankx = 0;
        int save_ecc_ena;
        int active_rank;
        int sw_wlevel_offset = 1;
        int sw_wlevel_enable = 1; /* FIX... Should be customizable. */
        int interfaces;
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

        if ((s = lookup_env_parameter("ddr_software_wlevel")) != NULL) {
            sw_wlevel_enable = simple_strtoul(s, NULL, 0);
        }

        if (sw_wlevel_enable)
            ddr_print("Performing software Write-Leveling\n");

        /* Disable ECC for DRAM tests */
        lmc_config.u64 = cvmx_read_csr_node(node,
					    CVMX_LMCX_CONFIG(ddr_interface_num));
        save_ecc_ena = lmc_config.cn78xx.ecc_ena;
        lmc_config.cn78xx.ecc_ena = 0;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
#ifndef __U_BOOT__
        limit_l2_ways(0, 1);       /* Disable l2 sets for DRAM testing */
#endif

        /* We need to track absolute rank number, as well as how many
        ** active ranks we have.  Two single rank DIMMs show up as
        ** ranks 0 and 2, but only 2 ranks are active. */
        active_rank = 0;

        interfaces = __builtin_popcount(ddr_interface_mask);

        for (rankx = 0; rankx < dimm_count * 4; rankx++) {
            uint64_t rank_addr;
            int vref_value, final_vref_value;
            char best_vref_values_count, vref_values_count;
            char best_vref_values_start, vref_values_start;

            int bytes_failed;
            sw_wl_status_t byte_test_status[9] = { WL_ESTIMATED };
            sw_wl_status_t sw_wl_rank_status = WL_HARDWARE;

            if (!sw_wlevel_enable)
                break;

            if (!(rank_mask & (1 << rankx)))
                continue;


            /* Save off the h/w wl results */
            lmc_wlevel_rank_hw_results.u64 = cvmx_read_csr_node(node,
								CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            vref_values_count = 0;
            vref_values_start = 0;
            best_vref_values_count = 0;
            best_vref_values_start = 0;

            debug_print("Rank(%d)", rankx);
            for (vref_value=0; vref_value<(0x33); ++vref_value) {
                debug_print(" %02x", vref_value);
            }
            debug_print("\nRank(%d)", rankx);
            /* Loop one extra time using the Final Vref value. */
            for (vref_value=0; vref_value<(0x33+1); ++vref_value) {
                if (ddr_type == DDR4_DRAM) {
                    if (vref_value < 0x33) {
                        set_vref(cpu_id, node, ddr_interface_num, rankx, 0, vref_value);
                    } else {
                        /* Set the final Vref value. */
                        if (best_vref_values_count > 0) {
                            best_vref_values_count = max(best_vref_values_count, 2);
                            final_vref_value = best_vref_values_start + divide_roundup((best_vref_values_count-1)*5,10);
                        } else {
                            /* If nothing passed use the default Vref value for this rank */
                            cvmx_lmcx_modereg_params2_t lmc_modereg_params2;

                            lmc_modereg_params2.u64 = cvmx_read_csr_node(node,
									 CVMX_LMCX_MODEREG_PARAMS2(ddr_interface_num));

                            switch (rankx) {
                            case 0:
                                final_vref_value = lmc_modereg_params2.s.vref_value_00;
                                break;
                            case 1:
                                final_vref_value = lmc_modereg_params2.s.vref_value_01;
                                break;
                            case 2:
                                final_vref_value = lmc_modereg_params2.s.vref_value_10;
                                break;
                            case 3:
                                final_vref_value = lmc_modereg_params2.s.vref_value_11;
                                break;
                            }
                        }
                        debug_print(" (0x%02x)\n", final_vref_value);
                        ddr_print("Rank(%d) Vref Training Summary                 :    0x%02x <----- 0x%02x -----> 0x%02x\n",
                                  rankx, best_vref_values_start, final_vref_value,
                                  best_vref_values_start+best_vref_values_count-1);
                        set_vref(cpu_id, node, ddr_interface_num, rankx, 0,
			         final_vref_value);
                    }
                } /* if (ddr_type == DDR4_DRAM) */
                lmc_wlevel_rank.u64 = lmc_wlevel_rank_hw_results.u64; /* Restore the saved value */
                if (wlevel_bitmask_errors == 0) {
                    /* Determine address of DRAM to test for software write leveling. */
                    rank_addr  = active_rank * ((1ull << (pbank_lsb+interfaces/2))/(1+bunk_enable));
                    rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */

                    /* Adjust address for boot bus hole in memory map. */
                    if (rank_addr > 0x10000000)
                        rank_addr += 0x10000000;
                    rank_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;
                    debug_print("Rank Address: 0x%llx\n", rank_addr);

                    for (byte = 0; byte < 8; ++byte) {
                        uint64_t byte_bitmask = 0xff;
                        if (!(ddr_interface_bytemask&(1<<byte)))
                            continue;

                        /* If we will be switching to 32bit mode only test 4 ECC bits.  */
                        if ((! ddr_interface_64b) && (byte == 4))
                            byte_bitmask = 0x0f;

                        /* If h/w write-leveling had no errors then use
                        ** s/w write-leveling to fixup only the upper bits
                        ** of the delays. */
                        for (delay = get_wlevel_rank_struct(&lmc_wlevel_rank, byte);
			     delay < 32; delay += 8) {
                            update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
                            debug_print("Testing byte %d delay %2d\n", byte, delay);
                            ddr_config_write_csr_node(node,
						      CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
						      lmc_wlevel_rank.u64);
                            lmc_wlevel_rank.u64 = cvmx_read_csr_node(node,
								     CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                            if (!test_dram_byte(rank_addr, 2048, byte, byte_bitmask)) {
                                debug_print("        byte %d(0x%llx) delay %2d Passed\n", byte, rank_addr, delay);
                                byte_test_status[byte] = WL_HARDWARE;
                                break;
                            } else {
                                debug_print("        byte %d delay %2d Errors\n", byte, delay);
                            }
                        }
                    }

                    if ((ddr_interface_bytemask & 0xff) == 0xff) {
                        if (save_ecc_ena) {
                            int save_byte8 = lmc_wlevel_rank.cn78xx.byte8;
                            int test_byte8;
                            int test_byte8_error;
                            int byte8_error;
                            int adder;
                            byte_test_status[8] = WL_HARDWARE; /* H/W delay value */

                            if ((save_byte8 != lmc_wlevel_rank.cn78xx.byte3) &&
                                (save_byte8 != lmc_wlevel_rank.cn78xx.byte4)) {

                                test_byte8 = save_byte8;
                                byte8_error = 0x1f;
                                for (adder = 0; adder<= 32; adder+=8) {
                                    test_byte8_error = _abs((adder+save_byte8)
                                                            - divide_nint(lmc_wlevel_rank.s.byte3+lmc_wlevel_rank.s.byte4, 2));
                                    if (test_byte8_error < byte8_error) {
                                        byte8_error = test_byte8_error;
                                        test_byte8 = save_byte8 + adder;
                                    }
                                }

                                lmc_wlevel_rank.cn78xx.byte8 = test_byte8 & ~1; /* Use only even settings */
                            }

                            if (lmc_wlevel_rank.cn78xx.byte8 != save_byte8) {
                                /* Change the status if s/w adjusted the delay */
                                byte_test_status[8] = WL_SOFTWARE; /* Estimated delay */
                            }
                        } else {
                            byte_test_status[8] = WL_HARDWARE; /* H/W delay value */
                            lmc_wlevel_rank.cn78xx.byte8 = lmc_wlevel_rank.cn78xx.byte0; /* ECC is not used */
                        }
                    } else {
                        if (save_ecc_ena) {
                            /* Estimate the ECC byte delay  */
                            if (lmc_wlevel_rank.cn78xx.byte4 < lmc_wlevel_rank.cn78xx.byte3)
                               lmc_wlevel_rank.cn78xx.byte4 = lmc_wlevel_rank.cn78xx.byte3;
                        } else {
                            lmc_wlevel_rank.cn78xx.byte4 = lmc_wlevel_rank.cn78xx.byte0; /* ECC is not used */
                        }
                    }
                }

                bytes_failed = 0;
                for (byte = 0; byte < 8; ++byte) {
                    /* Don't accumulate errors for untested bytes. */
                    if (!(ddr_interface_bytemask & (1 << byte)))
                        continue;
                    bytes_failed += (byte_test_status[byte] == WL_ESTIMATED);
                }

                 /* Vref training loop is only used for DDR4  */
                if (ddr_type != 4)
			break;

                if (vref_value < 0x33) {
                    debug_print(" %2d", bytes_failed);
                }
                if (bytes_failed == 0) {
                    if (vref_values_count == 0) {
                        vref_values_start = vref_value;
                    }
                    ++vref_values_count;
                    if (vref_values_count > best_vref_values_count) {
                        best_vref_values_count = vref_values_count;
                        best_vref_values_start = vref_values_start;
                    }
                } else {
                    vref_values_count = 0;
                }
            } /* for (vref_value=0; vref_value<0x33; ++vref_value) */

            if (bytes_failed) {
                uint64_t rank_addr;
                sw_wl_rank_status = WL_SOFTWARE;
                /* If s/w fixups failed then retry using s/w write-leveling. */
                if (wlevel_bitmask_errors == 0) {
                    /* h/w succeeded but s/w fixups failed. So retry s/w. */
                    debug_print("Rank(%d) Retrying software Write-Leveling.\n", rankx);
                }
                for (byte = 0; byte < 8; ++byte) {
                    int passed = 0;
                    int wl_offset;
                    uint64_t byte_bitmask = 0xff;

                    if (!(ddr_interface_bytemask & (1 << byte)))
                        continue;

                    /* If we will be switching to 32bit mode only test 4 ECC bits.  */
                    if ((! ddr_interface_64b) && (byte == 4))
                        byte_bitmask = 0x0f;

                    /* Determine address of DRAM to test for software write leveling. */
                    rank_addr  = active_rank * ((1ull << (pbank_lsb+interfaces/2))/(1+bunk_enable));
                    rank_addr |= (ddr_interface_num<<7); /* Map address into proper interface */

                    /* Adjust address for boot bus hole in memory map. */
                    if (rank_addr > 0x10000000)
                        rank_addr += 0x10000000;
                    rank_addr |= (uint64_t)node << CVMX_NODE_MEM_SHIFT;
                    debug_print("Rank Address: 0x%llx\n", rank_addr);

                    for (wl_offset = sw_wlevel_offset; wl_offset >= 0; --wl_offset) {
                    //for (delay=30; delay>=0; delay-=2) { /* Top-Down */
                        for (delay = 0; delay < 32; delay += 2) {  /* Bottom-UP */
                            update_wlevel_rank_struct(&lmc_wlevel_rank, byte, delay);
                            debug_print("Testing byte %d delay %2d\n", byte, delay);
                            ddr_config_write_csr_node(node,
						      CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
						      lmc_wlevel_rank.u64);
                            lmc_wlevel_rank.u64 = cvmx_read_csr_node(node,
								     CVMX_LMCX_WLEVEL_RANKX(rankx,
											    ddr_interface_num));

                            if (!test_dram_byte(rank_addr, 2048, byte, byte_bitmask)) {
                                ++passed;
                                if (passed == (1 + wl_offset)) { /* Look for consecutive working settings */
                                    debug_print("        byte %d(0x%llx) delay %2d Passed\n", byte, rank_addr, delay);
                                    if (wl_offset == 1) {
                                        byte_test_status[byte] = WL_SOFTWARE;
                                    } else if (wl_offset == 0) {
                                        byte_test_status[byte] = WL_SOFTWARE1;
                                    }
                                    break;
                                }
                            } else {
                                debug_print("        byte %d delay %2d Errors\n", byte, delay);
                                passed = 0;
                            }
                        }
                        if (passed == (1 + wl_offset)) { /* Look for consecutive working settings */
                            break;
                        }
                    }
                    if (passed == 0) {
                        /* Last resort. Use Rlevel settings to estimate
                           Wlevel if software write-leveling fails */
                        lmc_rlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_RLEVEL_RANKX(rankx, ddr_interface_num));
                        rlevel_to_wlevel(&lmc_rlevel_rank, &lmc_wlevel_rank, byte);
                    }
                }

                if (save_ecc_ena) {
                    /* ECC byte has to be estimated. Take the average of the two surrounding bytes. */
                    int test_byte8 = divide_nint(lmc_wlevel_rank.cn78xx.byte3
                                                 + lmc_wlevel_rank.cn78xx.byte4
                                                 + 2 /* round-up*/ , 2);
                    lmc_wlevel_rank.cn78xx.byte8 = test_byte8 & ~1; /* Use only even settings */
                    byte_test_status[8] = WL_ESTIMATED; /* Estimated delay */
                } else {
                    byte_test_status[8] = WL_HARDWARE; /* H/W delay value */
                    lmc_wlevel_rank.cn78xx.byte8 = lmc_wlevel_rank.cn78xx.byte0; /* ECC is not used */
                }

                /* Set delays for unused bytes to match byte 0. */
                for (byte=0; byte<8; ++byte) {
                    if ((ddr_interface_bytemask & (1 << byte)))
                        continue;
                    update_wlevel_rank_struct(&lmc_wlevel_rank, byte,
					      lmc_wlevel_rank.cn78xx.byte0);
                    byte_test_status[byte] = WL_SOFTWARE;
                }

            }

            ddr_config_write_csr_node(node,
				      CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
				      lmc_wlevel_rank.u64);
            lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %2d%3s %s\n",
                      rankx,
                      lmc_wlevel_rank.cn78xx.status,
                      lmc_wlevel_rank.u64,
                      lmc_wlevel_rank.cn78xx.byte8, wl_status_strings[byte_test_status[8]],
                      lmc_wlevel_rank.cn78xx.byte7, wl_status_strings[byte_test_status[7]],
                      lmc_wlevel_rank.cn78xx.byte6, wl_status_strings[byte_test_status[6]],
                      lmc_wlevel_rank.cn78xx.byte5, wl_status_strings[byte_test_status[5]],
                      lmc_wlevel_rank.cn78xx.byte4, wl_status_strings[byte_test_status[4]],
                      lmc_wlevel_rank.cn78xx.byte3, wl_status_strings[byte_test_status[3]],
                      lmc_wlevel_rank.cn78xx.byte2, wl_status_strings[byte_test_status[2]],
                      lmc_wlevel_rank.cn78xx.byte1, wl_status_strings[byte_test_status[1]],
                      lmc_wlevel_rank.cn78xx.byte0, wl_status_strings[byte_test_status[0]],
                      (sw_wl_rank_status == WL_HARDWARE) ? "" : "(s)"
                      );

            active_rank++;
        }


        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t value;
            int parameter_set = 0;
            if (!(rank_mask & (1 << rankx)))
                continue;

            lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

            for (i=0; i<9; ++i) {
                if ((s = lookup_env_parameter("ddr%d_wlevel_rank%d_byte%d", ddr_interface_num, rankx, i)) != NULL) {
                    parameter_set |= 1;
                    value = simple_strtoul(s, NULL, 0);

                    update_wlevel_rank_struct(&lmc_wlevel_rank, i, value);
                }
            }

            if ((s = lookup_env_parameter_ull("ddr%d_wlevel_rank%d", ddr_interface_num, rankx)) != NULL) {
                parameter_set |= 1;
                value = simple_strtoull(s, NULL, 0);
                lmc_wlevel_rank.u64 = value;
            }

            if (parameter_set) {
                ddr_config_write_csr_node(node,
					  CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num),
					  lmc_wlevel_rank.u64);
                lmc_wlevel_rank.u64 = cvmx_read_csr_node(node, CVMX_LMCX_WLEVEL_RANKX(rankx, ddr_interface_num));

                ddr_print("Rank(%d) Wlevel Rank %#5x, 0x%016llX : %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
                          rankx,
                          lmc_wlevel_rank.cn78xx.status,
                          lmc_wlevel_rank.u64,
                          lmc_wlevel_rank.cn78xx.byte8,
                          lmc_wlevel_rank.cn78xx.byte7,
                          lmc_wlevel_rank.cn78xx.byte6,
                          lmc_wlevel_rank.cn78xx.byte5,
                          lmc_wlevel_rank.cn78xx.byte4,
                          lmc_wlevel_rank.cn78xx.byte3,
                          lmc_wlevel_rank.cn78xx.byte2,
                          lmc_wlevel_rank.cn78xx.byte1,
                          lmc_wlevel_rank.cn78xx.byte0
                          );
            }
        }

#ifdef ENABLE_DISPLAY_MPR_PAGE
        /* Display MPR values for Page 2 */
        if (ddr_type == DDR4_DRAM) {
        for (rankx = 0; rankx < dimm_count * 4;rankx++) {
            uint64_t mpr_data[3];
            if (!(rank_mask & (1 << rankx)))
                continue;

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
        }
        }
#endif /* ENABLE_DISPLAY_MPR_PAGE */

        /* Enable 32-bit mode if required. */
        lmc_config.cn78xx.mode32b         = (! ddr_interface_64b);
        ddr_print("%-45s : %d\n", "MODE32B", lmc_config.cn78xx.mode32b);

        /* Restore the ECC configuration */
        lmc_config.cn78xx.ecc_ena = save_ecc_ena;
        ddr_config_write_csr_node(node,
				  CVMX_LMCX_CONFIG(ddr_interface_num),
				  lmc_config.u64);
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
    }


    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_write_offset",
			       custom_lmc_config->dll_write_offset, "ddr%d_dll_write_offset_byte%d", 1);
    process_custom_dll_offsets(node, ddr_interface_num, cpu_id, "ddr_dll_read_offset",
			       custom_lmc_config->dll_read_offset,  "ddr%d_dll_read_offset_byte%d",  2);

    /* Workaround Errata 21216 */
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

    {
        /* Clear any residual ECC errors */
        int num_tads = 1;
        int tad;

        ddr_config_write_csr_node(node,
				  CVMX_LMCX_INT(ddr_interface_num), -1ULL);
        cvmx_read_csr_node(node, CVMX_LMCX_INT(ddr_interface_num));

        for (tad = 0; tad < num_tads; tad++)
            cvmx_write_csr_node(node, CVMX_L2C_TADX_INT(tad),
				cvmx_read_csr_node(node,
						   CVMX_L2C_TADX_INT(tad)));
        ddr_print("%-45s : 0x%08x\n", "LMC_INT",
                  cvmx_read_csr_node(node, CVMX_LMCX_INT(ddr_interface_num)));
    }

    return mem_size_mbytes;
}

// dummy entry to satisfy the linker; 70xx does not support this feature
static void cvmx_dbi_switchover(int node, uint32_t cpuid)
{
}

// dummy entry to satisfy the linker; 70xx does not support this feature
static void cvmx_maybe_tune_node(int node, uint32_t cpuid, uint32_t ddr_speed)
{
}

