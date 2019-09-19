#ifndef __OCTEON_EAP7000_REF_SHARED_H__
#define __OCTEON_EAP7000_REF_SHARED_H__

#define EAP7000_REF_MT41J256M16HA_125_32bits_1333_with_ecc_spd_CL9 \
0x92, 0x11, 0x0b, 0x02, 0x04, 0x19, 0x00, 0x02, 0x0a, 0x00, 0x01, 0x08, 0x0C, 0x00, 0x7c, 0x00 , \
0x6C, 0x78, 0x6C, 0x3C, 0x6C, 0x11, 0x20, 0x8C, 0x28, 0x08, 0x3C, 0x3C, 0x02, 0x1C, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0xc0 , \
0x44, 0x33, 0x2d, 0x35, 0x36, 0x43, 0x47, 0x31, 0x30, 0x37, 0x4a, 0x54, 0x39, 0x56, 0x2d, 0x39 , \
0x39, 0x39

#define EAP7000_REF_MT41J256M16HA_125_32bits_1333_with_ecc_spd_CL10 \
0x92, 0x11, 0x0b, 0x02, 0x04, 0x19, 0x00, 0x02, 0x0a, 0x00, 0x01, 0x08, 0x0C, 0x00, 0x7c, 0x00 , \
0x78, 0x78, 0x78, 0x5A, 0x78, 0x11, 0x2C, 0xA4, 0x28, 0x08, 0x3C, 0x3C, 0x02, 0x1C, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0xc0 , \
0x44, 0x33, 0x2d, 0x35, 0x36, 0x43, 0x47, 0x31, 0x30, 0x37, 0x4a, 0x54, 0x39, 0x56, 0x2d, 0x39 , \
0x39, 0x39

#define EAP7000_REF_DDR3_spd						\
	EAP7000_REF_MT41J256M16HA_125_32bits_1333_with_ecc_spd_CL9

#define OCTEON_EAP7000_REF_SPD_VALUES					\
	EAP7000_REF_DDR3_spd

#define OCTEON_EAP7000_REF_DRAM_SOCKET_CONFIGURATION0 \
    {{0x00, 0x00}, {octeon_eap7000_ref_spd_values, NULL}}

#define OCTEON_EAP7000_REF_BOARD_EEPROM_TWSI_ADDR        (0)

/**
 * Local copy of these parameters to allow for customization for this
 * board design.  The generic version resides in lib_octeon_shared.h.
 *
 */
#define OCTEON_EAP7000_REF_MODEREG_PARAMS1_1RANK_1SLOT	\
{							\
	.cn63xx = {					\
		.pasr_00	= 0,			\
		.asr_00		= 0,			\
		.srt_00		= 0,			\
		.rtt_wr_00	= 0,			\
		.dic_00		= 0,			\
		.rtt_nom_00	= rttnom_40ohm,		\
		.pasr_01	= 0,			\
		.asr_01		= 0,			\
		.srt_01		= 0,			\
		.rtt_wr_01	= 0,			\
		.dic_01		= 0,			\
		.rtt_nom_01	= 0,			\
		.pasr_10	= 0,			\
		.asr_10		= 0,			\
		.srt_10		= 0,			\
		.rtt_wr_10	= 0,			\
		.dic_10		= 0,			\
		.rtt_nom_10	= 0,			\
		.pasr_11	= 0,			\
		.asr_11		= 0,			\
		.srt_11		= 0,			\
		.rtt_wr_11	= 0,			\
		.dic_11		= 0,			\
		.rtt_nom_11	= 0			\
	}						\
}

#define OCTEON_EAP7000_REF_MODEREG_PARAMS1_2RANK_1SLOT	\
{							\
	.cn63xx = {					\
		.pasr_00	= 0,			\
		.asr_00		= 0,			\
		.srt_00		= 0,			\
		.rtt_wr_00	= 0,			\
		.dic_00		= 0,			\
		.rtt_nom_00	= rttnom_40ohm,		\
		.pasr_01	= 0,			\
		.asr_01		= 0,			\
		.srt_01		= 0,			\
		.rtt_wr_01	= 0,			\
		.dic_01		= 0,			\
		.rtt_nom_01	= 0,			\
		.pasr_10	= 0,			\
		.asr_10		= 0,			\
		.srt_10		= 0,			\
		.rtt_wr_10	= 0,			\
		.dic_10		= 0,			\
		.rtt_nom_10	= 0,			\
		.pasr_11	= 0,			\
		.asr_11		= 0,			\
		.srt_11		= 0,			\
		.rtt_wr_11	= 0,			\
		.dic_11		= 0,			\
		.rtt_nom_11	= 0			\
	}						\
}




#define OCTEON_EAP7000_REF_DRAM_ODT_1RANK_CONFIGURATION \
	/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1             reserved       RODT_CTL    RODT_MASK    reserved */	\
	/* =====   ======== ============== ==========================================  ============== ========= ============== ======== */	\
    /*   1 */ {   0,    0x00000001ULL, OCTEON_EAP7000_REF_MODEREG_PARAMS1_1RANK_1SLOT, {.u64=0x0000},     3,     0x00000000ULL,  0  }      \

#define OCTEON_EAP7000_REF_DRAM_ODT_2RANK_CONFIGURATION \
	/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1             reserved       RODT_CTL    RODT_MASK    reserved */	\
	/* =====   ======== ============== ==========================================  ============== ========= ============== ======== */	\
    /*   1 */ {   0,    0x00000101ULL, OCTEON_EAP7000_REF_MODEREG_PARAMS1_2RANK_1SLOT, {.u64=0x0000},     3,     0x00000000ULL,    0  }    \

/* Construct a static initializer for the ddr_configuration_t variable that holds
 * (almost) all of the information required for DDR initialization.
 */

/*
  The parameters below make up the custom_lmc_config data structure.
  This structure is used to customize the way that the LMC DRAM
  Controller is configured for a particular board design.

  Refer to the file lib_octeon_board_table_entry.h for a description
  of the custom board settings.  It is usually kept in the following
  location... arch/mips/include/asm/arch-octeon/

*/

#define OCTEON_EAP7000_REF_DDR_CONFIGURATION {				\
	/* Interface 0 */						\
	.custom_lmc_config = {						\
		.min_rtt_nom_idx	= 1,				\
		.max_rtt_nom_idx	= 5,				\
		.min_rodt_ctl		= 1,				\
		.max_rodt_ctl		= 5,				\
		.dqx_ctl		= 4,				\
		.ck_ctl			= 4,				\
		.cmd_ctl		= 4,				\
		.min_cas_latency	= 0,				\
		.offset_en 		= 1,				\
		.offset_udimm		= 2,				\
		.offset_rdimm		= 2,				\
                .ntune_offset		= -2,				\
                .ptune_offset		= 1,				\
		.ddr_rtt_nom_auto	= 0,				\
		.ddr_rodt_ctl_auto	= 0,				\
		.rlevel_compute 	= 0,				\
		.dll_write_offset	= NULL,				\
		.dll_read_offset	= NULL,				\
		.ddr2t_udimm 		= 0,				\
		.ddr2t_rdimm 		= 0,				\
		.fprch2			= 1,				\
		.parity			= 0,				\
		.mode32b		= 1,				\
	},								\
        .dimm_config_table = {						\
            OCTEON_EAP7000_REF_DRAM_SOCKET_CONFIGURATION0,		\
		DIMM_CONFIG_TERMINATOR,					\
	},								\
	.unbuffered = {							\
		.ddr_board_delay = 0,					\
		.lmc_delay_clk = 0,					\
		.lmc_delay_cmd = 0,					\
		.lmc_delay_dq = 0,					\
	},								\
	.registered = {							\
		.ddr_board_delay = 0,					\
		.lmc_delay_clk = 0,					\
		.lmc_delay_cmd = 0,					\
		.lmc_delay_dq = 0,					\
	},								\
	.odt_1rank_config = {                        			\
            OCTEON_EAP7000_REF_DRAM_ODT_1RANK_CONFIGURATION,		\
	},								\
	.odt_2rank_config = {						\
            OCTEON_EAP7000_REF_DRAM_ODT_2RANK_CONFIGURATION,		\
	},								\
}

#endif   /* __OCTEON_EAP7000_REF_SHARED_H__ */
