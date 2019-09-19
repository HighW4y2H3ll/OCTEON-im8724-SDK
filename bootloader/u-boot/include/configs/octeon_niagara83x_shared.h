#ifndef __OCTEON_NIAGARA83X_SHARED_H__
#define __OCTEON_NIAGARA83X_SHARED_H__

#define OCTEON_NIAGARA83X_SFP_VALUES		\
0x92,0x10,0x0b,0x0d,0x03,0x19,0x00,0x01,0x08,0x52,0x01,0x08,0x0a,0x00,0xfe,0x00,\
0x6e,0x78,0x6e,0x3c,0x6e,0x11,0x18,0x86,0xf0,0x0a,0x3c,0x3c,0x01,0x40,0x83,0x85,\
0x00,0xa2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x11,0x1f,0x01,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x30

#define OCTEON_NIAGARA83X_DRAM_SOCKET_CONFIGURATION \
	{{0x51, 0x00}, {NULL, NULL}}

#define OCTEON_NIAGARA83X_BOARD_EEPROM_TWSI_ADDR        (0x50)

/**
 * Local copy of these parameters to allow for customization for this
 * board design.  The generic version resides in lib_octeon_shared.h.
 *
 */
#define OCTEON_NIAGARA83X_MODEREG_PARAMS1_1RANK_1SLOT	\
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

#define OCTEON_NIAGARA83X_MODEREG_PARAMS1_2RANK_1SLOT	\
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




#define OCTEON_NIAGARA83X_DRAM_ODT_1RANK_CONFIGURATION \
/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1                reserved       RODT_CTL    RODT_MASK    reserved */	\
/* =====   ======== ============== ============================================== ============== ========= ============== ======== */	\
/*   1 */ 									\
{										\
	.odt_ena  = 0,								\
	.odt_mask = 0x00000001ULL,						\
	.modereg_params1 = OCTEON_NIAGARA83X_MODEREG_PARAMS1_1RANK_1SLOT, 	\
	.modereg_params2 = {.u64=0x0000},					\
	.qs_dic = 3,								\
	.rodt_ctl = 0x00000000ULL,  						\
	.dic = 0,								\
}

#define OCTEON_NIAGARA83X_DRAM_ODT_2RANK_CONFIGURATION \
/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1                reserved       RODT_CTL    RODT_MASK    reserved */	\
/* =====   ======== ============== ============================================== ============== ========= ============== ======== */	\
/*   1 */									\
{										\
	.odt_ena  = 0,								\
	.odt_mask = 0x00000101ULL,	/* WODT_MASK */					\
	.modereg_params1 = OCTEON_NIAGARA83X_MODEREG_PARAMS1_2RANK_1SLOT,	\
	.modereg_params2 = {.u64=0x0000},					\
	.qs_dic = 3,								\
	.rodt_ctl = 0x00000000ULL,	/* RODT_CTL */					\
	.dic = 0,								\
}

/* Construct a static initializer for the ddr_configuration_t variable that holds
 * (almost) all of the information required for DDR initialization.
 */

/*
 *  The parameters below make up the custom_lmc_config data structure.
 *  This structure is used to customize the way that the LMC DRAM
 *  Controller is configured for a particular board design.
 *
 *  Refer to the file lib_octeon_board_table_entry.h for a description
 *  of the custom board settings.  It is usually kept in the following
 *  location... arch/mips/include/asm/arch-octeon/
 *
 */

#define OCTEON_NIAGARA83X_DDR_CONFIGURATION {				\
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
		.ddr_rtt_nom_auto	= 0,				\
		.ddr_rodt_ctl_auto	= 0,				\
		.rlevel_comp_offset_udimm = 0,				\
		.rlevel_compute 	= 0,				\
		.ddr2t_udimm 		= 0,				\
		.ddr2t_rdimm 		= 0,				\
		.maximum_adjacent_rlevel_delay_increment = 1,		\
		.fprch2			= 1,				\
		.parity			= 0,				\
		.mode32b		= 1,				\
		.ntune_offset		= 0,				\
		.ptune_offset		= 0,				\
                .dll_write_offset	= NULL,                         \
                .dll_read_offset 	= NULL,                         \
		},							\
	.dimm_config_table = {						\
		OCTEON_NIAGARA83X_DRAM_SOCKET_CONFIGURATION,		\
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
		OCTEON_NIAGARA83X_DRAM_ODT_1RANK_CONFIGURATION,		\
	},								\
	.odt_2rank_config = {						\
		OCTEON_NIAGARA83X_DRAM_ODT_2RANK_CONFIGURATION,		\
	},								\
}

#endif   /* __OCTEON_NIAGARA83X_SHARED_H__ */
