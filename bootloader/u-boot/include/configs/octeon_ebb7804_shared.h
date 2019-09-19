#ifndef __OCTEON_EBB7804_SHARED_H__
#define __OCTEON_EBB7804_SHARED_H__


/*
 * Define this to use SPD data defined in header files instead of
 * using TWSI to access the SPDs in the DIMMs.  This is a workaround
 * for boards that lack working TWSI interfaces.
 */

/* 1-DIMM, 4-LMC */
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_4LMC0(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},   \
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_4LMC1(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},   \
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_4LMC2(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},   \
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_4LMC3(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},   \
	{{0x0, 0x0}, {NULL, NULL}}

/* 2-DIMM, 4-LMC */
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_4LMC0(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_4LMC1(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_4LMC2(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_4LMC3(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}

/* 1-DIMM, 2-LMC */
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_2LMC0(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_2LMC1(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_2LMC2(spd_pointer) \
	{{0x0, 0x0}, {NULL, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_1DIMM_2LMC3(spd_pointer) \
	{{0x0, 0x0}, {NULL, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}

/* 2-DIMM, 2-LMC */
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_2LMC0(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_2LMC1(spd_pointer) \
	{{0x0, 0x0}, {spd_pointer, NULL}},\
	{{0x0, 0x0}, {spd_pointer, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_2LMC2(spd_pointer) \
	{{0x0, 0x0}, {NULL, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}
#define OCTEON_DRAM_SOCKET_CONFIG_2DIMM_2LMC3(spd_pointer) \
	{{0x0, 0x0}, {NULL, NULL}},\
	{{0x0, 0x0}, {NULL, NULL}}

#define EBB7804_MT18ASF1G72AZ_2G1AYESZG_VALUES \
0x23,0x06,0x0c,0x02,0x84,0x19,0x00,0x00,0x00,0x00,0x00,0x03,0x09,0x0b,0x80,0x00,\
0x00,0x00,0x08,0x0c,0xf4,0x1b,0x00,0x00,0x6c,0x6c,0x6c,0x11,0x08,0x74,0x20,0x08,\
0x00,0x05,0x70,0x03,0x00,0xa8,0x1e,0x2b,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0xec,0xb5,0xce,0x00,0x00,0x00,0x00,0x00,0xc2,0x98,0xf4,\
0x11,0x11,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x2e

#define EBB7804_MT18ASF1G72AZ_2G1AXESZG_VALUES \
0x23,0x10,0x0c,0x02,0x84,0x19,0x00,0x00,0x00,0x00,0x00,0x03,0x09,0x0b,0x80,0x00,\
0x00,0x00,0x08,0x0c,0xf4,0x1b,0x00,0x00,0x6c,0x6c,0x6c,0x11,0x08,0x74,0x20,0x08,\
0x00,0x05,0x70,0x03,0x00,0xa8,0x1e,0x2b,0x2b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x2c,0x15,0x35,\
0x15,0x35,0x0b,0x2c,0x15,0x35,0x0b,0x2c,0x0b,0x2c,0x0b,0x35,0x15,0x36,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0xec,0xb5,0xce,0x00,0x00,0x00,0x00,0x00,0xc2,0x38,0x93,\
0x11,0x11,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x2e

/* EBB7804_MT18ASF1G72AZ_2G1AYESZG_VALUES       ID: 101   DDR4 */
/* EBB7804_MT18ASF1G72AZ_2G1AXESZG_VALUES       ID: 102   DDR4 */

/*-------------------------------------------------------------------*/
/* Configuration 0: gpio 1 = OFF */
/*-------------------------------------------------------------------*/
/* Select SPD from the list. */
#define OCTEON_EBB7804_CFG0_SPD_VALUES	EBB7804_MT18ASF1G72AZ_2G1AXESZG_VALUES

/* Select socket configuration.
 * _1DIMM_4LMC _2DIMM_4LMC
 * _1DIMM_2LMC _2DIMM_2LMC
 */
#define OCTEON_EBB7804_CFG0_DIMM_LMC_CONFIGURATION _1DIMM_4LMC
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/* Configuration 1: gpio 1 = ON */
/*-------------------------------------------------------------------*/
/* Select SPD from the list. */
#define OCTEON_EBB7804_CFG1_SPD_VALUES	EBB7804_MT18ASF1G72AZ_2G1AYESZG_VALUES

/* Select socket configuration.
 * _1DIMM_4LMC _2DIMM_4LMC
 * _1DIMM_2LMC _2DIMM_2LMC
 */
#define OCTEON_EBB7804_CFG1_DIMM_LMC_CONFIGURATION _1DIMM_4LMC
/*-------------------------------------------------------------------*/


#define MERGE_AGAIN(X, Y) X ## Y
#define MERGE(X, Y) MERGE_AGAIN(X, Y)
#define OCTEON_EBB7804_CFG0_DRAM_SOCKET_CONFIGURATION0	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG0_DIMM_LMC_CONFIGURATION), 0)(octeon_ebb7804_cfg0_spd_values)
#define OCTEON_EBB7804_CFG0_DRAM_SOCKET_CONFIGURATION1	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG0_DIMM_LMC_CONFIGURATION), 1)(octeon_ebb7804_cfg0_spd_values)
#define OCTEON_EBB7804_CFG0_DRAM_SOCKET_CONFIGURATION2	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG0_DIMM_LMC_CONFIGURATION), 2)(octeon_ebb7804_cfg0_spd_values)
#define OCTEON_EBB7804_CFG0_DRAM_SOCKET_CONFIGURATION3	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG0_DIMM_LMC_CONFIGURATION), 3)(octeon_ebb7804_cfg0_spd_values)

#define OCTEON_EBB7804_CFG1_DRAM_SOCKET_CONFIGURATION0	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG1_DIMM_LMC_CONFIGURATION), 0)(octeon_ebb7804_cfg1_spd_values)
#define OCTEON_EBB7804_CFG1_DRAM_SOCKET_CONFIGURATION1	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG1_DIMM_LMC_CONFIGURATION), 1)(octeon_ebb7804_cfg1_spd_values)
#define OCTEON_EBB7804_CFG1_DRAM_SOCKET_CONFIGURATION2	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG1_DIMM_LMC_CONFIGURATION), 2)(octeon_ebb7804_cfg1_spd_values)
#define OCTEON_EBB7804_CFG1_DRAM_SOCKET_CONFIGURATION3	MERGE( MERGE( OCTEON_DRAM_SOCKET_CONFIG, OCTEON_EBB7804_CFG1_DIMM_LMC_CONFIGURATION), 3)(octeon_ebb7804_cfg1_spd_values)


#define OCTEON_EBB7804_DRAM_SOCKET_CONFIGURATION0     {{0x1050, 0x0}, {NULL, NULL}},{{0x1051, 0x0}, {NULL, NULL}}
#define OCTEON_EBB7804_DRAM_SOCKET_CONFIGURATION1     {{0x1052, 0x0}, {NULL, NULL}},{{0x1053, 0x0}, {NULL, NULL}}
#define OCTEON_EBB7804_DRAM_SOCKET_CONFIGURATION2     {{0x1054, 0x0}, {NULL, NULL}},{{0x1055, 0x0}, {NULL, NULL}}
#define OCTEON_EBB7804_DRAM_SOCKET_CONFIGURATION3     {{0x1056, 0x0}, {NULL, NULL}},{{0x1057, 0x0}, {NULL, NULL}}

#define OCTEON_EBB7804_BOARD_EEPROM_TWSI_ADDR        (0x56)
  


/*
 * Local copy of these parameters to allow for customization for this
 * board design.  The generic version resides in lib_octeon_shared.h.
 */

/* LMC0_MODEREG_PARAMS1 */
#define OCTEON_EBB7804_MODEREG_PARAMS1_1RANK_1SLOT      \
    { .cn78xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00    = ddr4_rttwr_80ohm&3,   \
                  .rtt_wr_00_ext= (ddr4_rttwr_80ohm>>2)&1,        \
                  .dic_00	= ddr4_dic_34ohm,	\
                  .rtt_nom_00	= 0,                    \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01    = 0,                    \
                  .dic_01	= ddr4_dic_34ohm,	\
                  .rtt_nom_01   = 0,                    \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10    = 0,                    \
                  .dic_10	= ddr4_dic_34ohm,	\
                  .rtt_nom_10   = 0,                    \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = 0,                    \
                  .dic_11	= ddr4_dic_34ohm,	\
                  .rtt_nom_11   = 0,                    \
        }                                               \
    }

#define OCTEON_EBB7804_MODEREG_PARAMS1_1RANK_2SLOT      \
    { .cn78xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00    = ddr4_rttwr_80ohm&3,   \
                  .rtt_wr_00_ext= (ddr4_rttwr_80ohm>>2)&1,        \
                  .dic_00	= ddr4_dic_34ohm,	\
                  .rtt_nom_00   = 0,                    \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01    = 0,                    \
                  .dic_01	= ddr4_dic_34ohm,	\
                  .rtt_nom_01   = 0,                    \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10    = ddr4_rttwr_80ohm&3,   \
                  .rtt_wr_10_ext= (ddr4_rttwr_80ohm>>2)&1,      \
                  .dic_10	= ddr4_dic_34ohm,	\
                  .rtt_nom_10   = 0,                    \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = 0,                    \
                  .dic_11	= ddr4_dic_34ohm,	\
                  .rtt_nom_11   = 0                     \
        }                                               \
    }

#define OCTEON_EBB7804_MODEREG_PARAMS1_2RANK_1SLOT      \
    { .cn78xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00	= ddr4_rttwr_240ohm,    \
                  .dic_00	= ddr4_dic_34ohm,       \
                  .rtt_nom_00	= 0,                    \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01	= ddr4_rttwr_240ohm,    \
                  .dic_01	= ddr4_dic_34ohm,       \
                  .rtt_nom_01	= 0,                    \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .dic_10	= ddr4_dic_34ohm,       \
                  .rtt_nom_10	= 0,                    \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = 0,                    \
                  .dic_11	= ddr4_dic_34ohm,	\
                  .rtt_nom_11   = 0,                    \
        }                                               \
    }

#define OCTEON_EBB7804_MODEREG_PARAMS1_2RANK_2SLOT      \
    { .cn78xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00	= ddr4_rttwr_240ohm,    \
                  .dic_00	= ddr4_dic_34ohm,       \
                  .rtt_nom_00	= ddr4_rttnom_120ohm,   \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01	= ddr4_rttwr_240ohm,    \
                  .dic_01	= ddr4_dic_34ohm,       \
                  .rtt_nom_01	= ddr4_rttnom_120ohm,   \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10	= ddr4_rttwr_240ohm,    \
                  .dic_10	= ddr4_dic_34ohm,       \
                  .rtt_nom_10	= ddr4_rttnom_120ohm,   \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11	= ddr4_rttwr_240ohm,    \
                  .dic_11	= ddr4_dic_34ohm,       \
                  .rtt_nom_11	= ddr4_rttnom_120ohm,   \
        }                                               \
    }

#define OCTEON_EBB7804_MODEREG_PARAMS1_4RANK_1SLOT      \
    { .cn78xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00    = rttwr_60ohm,          \
                  .dic_00       = dic_34ohm,            \
                  .rtt_nom_00   = rttnom_20ohm,         \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01    = rttwr_60ohm,          \
                  .dic_01       = dic_34ohm,            \
                  .rtt_nom_01   = rttnom_none,          \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10    = rttwr_60ohm,          \
                  .dic_10       = dic_34ohm,            \
                  .rtt_nom_10   = rttnom_20ohm,         \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = rttwr_60ohm,          \
                  .dic_11       = dic_34ohm,            \
                  .rtt_nom_11   = rttnom_none,          \
        }                                               \
    }


#define OCTEON_EBB7804_MODEREG_PARAMS2_1RANK_1SLOT	\
{							\
	.cn78xx = {					\
		.rtt_park_00    = ddr4_rttpark_60ohm,	\
		.vref_value_00  = 0x22,			\
		.vref_range_00  = 0,			\
		.rtt_park_01    = 0,			\
		.vref_value_01  = 0,			\
		.vref_range_01  = 0,			\
		.rtt_park_10    = 0,			\
		.vref_value_10  = 0,			\
		.vref_range_10  = 0,			\
		.rtt_park_11    = 0,			\
		.vref_value_11  = 0,			\
		.vref_range_11  = 0			\
	}						\
}

/* FIX */
#define OCTEON_EBB7804_MODEREG_PARAMS2_1RANK_2SLOT	\
{							\
	.cn78xx = {					\
		.rtt_park_00    = ddr4_rttpark_48ohm,	\
		.vref_value_00  = 0x1f,			\
		.vref_range_00  = 0,			\
		.rtt_park_01    = 0,			\
		.vref_value_01  = 0,			\
		.vref_range_01  = 0,			\
		.rtt_park_10    = ddr4_rttpark_48ohm,	\
		.vref_value_10  = 0x1f,			\
		.vref_range_10  = 0,			\
		.rtt_park_11    = 0,			\
		.vref_value_11  = 0,			\
		.vref_range_11  = 0			\
	}						\
}

#define OCTEON_EBB7804_MODEREG_PARAMS2_2RANK_1SLOT	\
{							\
	.cn78xx = {					\
		.rtt_park_00    = ddr4_rttpark_120ohm,	\
		.vref_value_00  = 0x19,			\
		.vref_range_00  = 0,			\
		.rtt_park_01    = ddr4_rttpark_120ohm,	\
		.vref_value_01  = 0x19,			\
		.vref_range_01  = 0,			\
		.rtt_park_10    = 0,			\
		.vref_value_10  = 0,			\
		.vref_range_10  = 0,			\
		.rtt_park_11    = 0,			\
		.vref_value_11  = 0,			\
		.vref_range_11  = 0			\
	}						\
}

#define OCTEON_EBB7804_MODEREG_PARAMS2_2RANK_2SLOT	\
{							\
	.cn78xx = {					\
		.rtt_park_00    = ddr4_rttpark_60ohm,	\
		.vref_value_00  = 0x19,			\
		.vref_range_00  = 0,			\
		.rtt_park_01    = ddr4_rttpark_60ohm,	\
		.vref_value_01  = 0x19,			\
		.vref_range_01  = 0,			\
		.rtt_park_10    = ddr4_rttpark_60ohm,	\
		.vref_value_10  = 0x19,			\
		.vref_range_10  = 0,			\
		.rtt_park_11    = ddr4_rttpark_60ohm,	\
		.vref_value_11  = 0x19,			\
		.vref_range_11  = 0			\
	}						\
}

#define OCTEON_EBB7804_MODEREG_PARAMS2_4RANK_1SLOT	\
{							\
	.cn78xx = {					\
		.rtt_park_00    = ddr4_rttpark_80ohm,	\
		.vref_value_00  = 0x1f,			\
		.vref_range_00  = 0,			\
		.rtt_park_01    = ddr4_rttpark_80ohm,	\
		.vref_value_01  = 0x1f,			\
		.vref_range_01  = 0,			\
		.rtt_park_10    = 0,			\
		.vref_value_10  = 0,			\
		.vref_range_10  = 0,			\
		.rtt_park_11    = 0,			\
		.vref_value_11  = 0,			\
		.vref_range_11  = 0			\
	}						\
}

#define OCTEON_EBB7804_CN78XX_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS    DQX_CTL  WODT_MASK                LMCX_MODEREG_PARAMS1                        LMCX_MODEREG_PARAMS2            RODT_CTL   RODT_MASK    rsv'ed */ \
    /* =====   ======== ============== ==========================================  ========================================== ======== ============== ====== */ \
    /*   1 */ {   ddr4_dqx_driver_34_ohm,    0x00000000ULL, OCTEON_EBB7804_MODEREG_PARAMS1_1RANK_1SLOT, OCTEON_EBB7804_MODEREG_PARAMS2_1RANK_1SLOT,   ddr4_rodt_ctl_48_ohm,    0x00000000ULL,    0  }, \
    /*   2 */ {   ddr4_dqx_driver_34_ohm,    0x00000000ULL, OCTEON_EBB7804_MODEREG_PARAMS1_1RANK_2SLOT, OCTEON_EBB7804_MODEREG_PARAMS2_1RANK_2SLOT,   ddr4_rodt_ctl_80_ohm,    0x00000000ULL,    0  }

#define OCTEON_EBB7804_CN78XX_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS    DQX_CTL  WODT_MASK                LMCX_MODEREG_PARAMS1                        LMCX_MODEREG_PARAMS2            RODT_CTL   RODT_MASK    rsv'ed */ \
    /* =====   ======== ============== ==========================================  ========================================== ======== ============== ====== */ \
    /*   1 */ {   ddr4_dqx_driver_34_ohm,    0x00000000ULL, OCTEON_EBB7804_MODEREG_PARAMS1_2RANK_1SLOT, OCTEON_EBB7804_MODEREG_PARAMS2_2RANK_1SLOT,   ddr4_rodt_ctl_80_ohm,    0x00000000ULL,    0  }, \
    /*   2 */ {   ddr4_dqx_driver_34_ohm,    0x0c0c0303ULL, OCTEON_EBB7804_MODEREG_PARAMS1_2RANK_2SLOT, OCTEON_EBB7804_MODEREG_PARAMS2_2RANK_2SLOT,   ddr4_rodt_ctl_48_ohm,    0x04080102ULL,    0  }

#define OCTEON_EBB7804_CN78XX_DRAM_ODT_4RANK_CONFIGURATION \
    /* DIMMS    DQX_CTL  WODT_MASK                LMCX_MODEREG_PARAMS1                        LMCX_MODEREG_PARAMS2            RODT_CTL   RODT_MASK    rsv'ed */ \
    /* =====   ======== ============== ==========================================  ========================================== ======== ============== ====== */ \
    /*   1 */ {   ddr4_dqx_driver_34_ohm,    0x01030203ULL, OCTEON_EBB7804_MODEREG_PARAMS1_4RANK_1SLOT, OCTEON_EBB7804_MODEREG_PARAMS2_4RANK_1SLOT,   ddr4_rodt_ctl_48_ohm,    0x01010202ULL,    0  }

/* Construct a static initializer for the ddr_configuration_t variable that holds
** (almost) all of the information required for DDR initialization.
*/

/*
  The parameters below make up the custom_lmc_config data structure.
  This structure is used to customize the way that the LMC DRAM
  Controller is configured for a particular board design.

  Refer to the file lib_octeon_board_table_entry.h for a description
  of the custom board settings.  It is usually kept in the following
  location... arch/mips/include/asm/arch-octeon/

*/

#define OCTEON_EBB7804_DDR_CONFIGURATION(SPD_CONFIGURATION)             \
    /* Interface 0 */                                                   \
    {                                                                   \
        .custom_lmc_config = {                                          \
            .min_rtt_nom_idx	= 1,                                    \
            .max_rtt_nom_idx	= 7,                                    \
            .min_rodt_ctl	= 1,                                    \
            .max_rodt_ctl	= 7,                                    \
            .ck_ctl		= ddr4_driver_34_ohm,                   \
            .cmd_ctl		= ddr4_driver_34_ohm,                   \
            .ctl_ctl		= ddr4_driver_34_ohm,                   \
            .min_cas_latency	= 0,                                    \
            .offset_en 		= 1,                                    \
            .offset_udimm	= 2,                                    \
            .offset_rdimm	= 2,                                    \
            .ddr_rtt_nom_auto	= 0,                                    \
            .ddr_rodt_ctl_auto	= 0,                                    \
            .rlevel_comp_offset_udimm	= 0,                            \
            .rlevel_comp_offset_rdimm	= 0,                            \
            .rlevel_compute 	= 0,                                    \
            .ddr2t_udimm 	= 1,                                    \
            .ddr2t_rdimm 	= 1,                                    \
            .maximum_adjacent_rlevel_delay_increment = 2,               \
            .fprch2		= 2,                                    \
            .dll_write_offset   = NULL,                                 \
            .dll_read_offset    = NULL,                                 \
            .parity		= 0},                                   \
            .dimm_config_table = {                                      \
             SPD_CONFIGURATION##0,                                      \
             DIMM_CONFIG_TERMINATOR},                                   \
                 .unbuffered = {                                        \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                      .registered = {                                   \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                           .odt_1rank_config = {                        \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_1RANK_CONFIGURATION},        \
                                .odt_2rank_config = {                   \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_2RANK_CONFIGURATION},        \
                                     .odt_4rank_config = {              \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_4RANK_CONFIGURATION}         \
    },                                                                  \
    /* Interface 1 */                                                   \
    {                                                                   \
        .custom_lmc_config = {                                          \
            .min_rtt_nom_idx	= 1,                                    \
            .max_rtt_nom_idx	= 7,                                    \
            .min_rodt_ctl	= 1,                                    \
            .max_rodt_ctl	= 7,                                    \
            .ck_ctl		= ddr4_driver_34_ohm,                   \
            .cmd_ctl		= ddr4_driver_34_ohm,                   \
            .ctl_ctl		= ddr4_driver_34_ohm,                   \
            .min_cas_latency	= 0,                                    \
            .offset_en 		= 1,                                    \
            .offset_udimm	= 2,                                    \
            .offset_rdimm	= 2,                                    \
            .ddr_rtt_nom_auto	= 0,                                    \
            .ddr_rodt_ctl_auto	= 0,                                    \
            .rlevel_comp_offset_udimm	= 0,                            \
            .rlevel_comp_offset_rdimm	= 0,                            \
            .rlevel_compute 	= 0,                                    \
            .ddr2t_udimm 	= 1,                                    \
            .ddr2t_rdimm 	= 1,                                    \
            .maximum_adjacent_rlevel_delay_increment = 2,               \
            .fprch2		= 2,                                    \
            .dll_write_offset   = NULL,                                 \
            .dll_read_offset    = NULL,                                 \
            .parity		= 0},                                   \
            .dimm_config_table = {                                      \
             SPD_CONFIGURATION##1,                                      \
             DIMM_CONFIG_TERMINATOR},                                   \
                 .unbuffered = {                                        \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                      .registered = {                                   \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                           .odt_1rank_config = {                        \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_1RANK_CONFIGURATION},        \
                                .odt_2rank_config = {                   \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_2RANK_CONFIGURATION},        \
                                     .odt_4rank_config = {              \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_4RANK_CONFIGURATION}         \
    },                                                                  \
    /* Interface 2 */                                                   \
    {                                                                   \
        .custom_lmc_config = {                                          \
            .min_rtt_nom_idx	= 1,                                    \
            .max_rtt_nom_idx	= 7,                                    \
            .min_rodt_ctl	= 1,                                    \
            .max_rodt_ctl	= 7,                                    \
            .ck_ctl		= ddr4_driver_34_ohm,                   \
            .cmd_ctl		= ddr4_driver_34_ohm,                   \
            .ctl_ctl		= ddr4_driver_34_ohm,                   \
            .min_cas_latency	= 0,                                    \
            .offset_en 		= 1,                                    \
            .offset_udimm	= 2,                                    \
            .offset_rdimm	= 2,                                    \
            .ddr_rtt_nom_auto	= 0,                                    \
            .ddr_rodt_ctl_auto	= 0,                                    \
            .rlevel_comp_offset_udimm	= 0,                            \
            .rlevel_comp_offset_rdimm	= 0,                            \
            .rlevel_compute 	= 0,                                    \
            .ddr2t_udimm 	= 1,                                    \
            .ddr2t_rdimm 	= 1,                                    \
            .maximum_adjacent_rlevel_delay_increment = 2,               \
            .fprch2		= 2,                                    \
            .dll_write_offset   = NULL,                                 \
            .dll_read_offset    = NULL,                                 \
            .parity		= 0},                                   \
            .dimm_config_table = {                                      \
             SPD_CONFIGURATION##2,                                      \
             DIMM_CONFIG_TERMINATOR},                                   \
                 .unbuffered = {                                        \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                      .registered = {                                   \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                           .odt_1rank_config = {                        \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_1RANK_CONFIGURATION},        \
                                .odt_2rank_config = {                   \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_2RANK_CONFIGURATION},        \
                                     .odt_4rank_config = {              \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_4RANK_CONFIGURATION}         \
    },                                                                  \
    /* Interface 3 */                                                   \
    {                                                                   \
        .custom_lmc_config = {                                          \
            .min_rtt_nom_idx	= 1,                                    \
            .max_rtt_nom_idx	= 7,                                    \
            .min_rodt_ctl	= 1,                                    \
            .max_rodt_ctl	= 7,                                    \
            .ck_ctl		= ddr4_driver_34_ohm,                   \
            .cmd_ctl		= ddr4_driver_34_ohm,                   \
            .ctl_ctl		= ddr4_driver_34_ohm,                   \
            .min_cas_latency	= 0,                                    \
            .offset_en 		= 1,                                    \
            .offset_udimm	= 2,                                    \
            .offset_rdimm	= 2,                                    \
            .ddr_rtt_nom_auto	= 0,                                    \
            .ddr_rodt_ctl_auto	= 0,                                    \
            .rlevel_comp_offset_udimm	= 0,                            \
            .rlevel_comp_offset_rdimm	= 0,                            \
            .rlevel_compute 	= 0,                                    \
            .ddr2t_udimm 	= 1,                                    \
            .ddr2t_rdimm 	= 1,                                    \
            .maximum_adjacent_rlevel_delay_increment = 2,               \
            .fprch2		= 2,                                    \
            .dll_write_offset   = NULL,                                 \
            .dll_read_offset    = NULL,                                 \
            .parity		= 0},                                   \
            .dimm_config_table = {                                      \
             SPD_CONFIGURATION##3,                                      \
             DIMM_CONFIG_TERMINATOR},                                   \
                 .unbuffered = {                                        \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                      .registered = {                                   \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
                           .odt_1rank_config = {                        \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_1RANK_CONFIGURATION},        \
                                .odt_2rank_config = {                   \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_2RANK_CONFIGURATION},        \
                                     .odt_4rank_config = {              \
            OCTEON_EBB7804_CN78XX_DRAM_ODT_4RANK_CONFIGURATION}         \
    }

#endif   /* __OCTEON_EBB7804_SHARED_H__ */
