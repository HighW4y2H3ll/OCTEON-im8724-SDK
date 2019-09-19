#ifndef __OCTEON_EP6300C_SHARED_H__
#define __OCTEON_EP6300C_SHARED_H__

#define WD3UE02GX818_1333L_CT_spd      \
0x92,0x10,0x0b,0x02,0x02,0x11,0x00,0x09,0x0b,0x52,0x01,0x08,0x0c,0x00,0x3c,0x00, \
0x69,0x78,0x69,0x30,0x69,0x11,0x20,0x89,0x70,0x03,0x3c,0x3c,0x00,0xf0,0x83,0x05, \
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x11,0x04,0x01, \
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
0x00,0x00,0x00,0x00,0x00,0x01,0x61,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x99,0xc0, \
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
0x00,0x00

#define WD3RE02GX818V_1333L_PEI_spd      \
0x92, 0x10, 0x0b, 0x01, 0x02, 0x19, 0x00, 0x09, 0x0b, 0x52, 0x01, 0x08, 0x0c, 0x00, 0x3c, 0x00, \
0x69, 0x78, 0x69, 0x30, 0x69, 0x11, 0x20, 0x89, 0x70, 0x03, 0x3c, 0x3c, 0x00, 0xf0, 0x83, 0x01, \
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x11, 0x0a, 0x05, \
0x00, 0x04, 0xb3, 0x04, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x5f, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00


#define USE_COMPILED_IN_SPD 0
#if USE_COMPILED_IN_SPD
#define OCTEON_EP6300C_DRAM_SOCKET_CONFIGURATION0 \
    {{0x00, 0x00}, {ep6300c_spd_values, NULL}},{{0x00, 0x0}, {NULL, NULL}}
#else
#define OCTEON_EP6300C_DRAM_SOCKET_CONFIGURATION0 \
    {{0x50, 0x0}, {NULL, NULL}},{{0x51, 0x0}, {NULL, NULL}}
#endif



#define OCTEON_EP6300C_BOARD_EEPROM_TWSI_ADDR        (0x54)


/*
** Local copy of these parameters to allow for customization for this
** board design.  The generic version resides in lib_octeon_shared.h.
** */

/* LMC0_MODEREG_PARAMS1 */
#define OCTEON_EP6300C_MODEREG_PARAMS1_1RANK_1SLOT  \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = 0,            \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = 0,            \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = 0,            \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0,            \
        }                                       \
    }

#define OCTEON_EP6300C_MODEREG_PARAMS1_1RANK_2SLOT             \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = rttwr_120ohm, \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = rttwr_120ohm, \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = rttnom_40ohm, \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0             \
        }                                       \
    }

#define OCTEON_EP6300C_MODEREG_PARAMS1_2RANK_1SLOT             \
    { .cn63xx = { .pasr_00      = 0,            \
                  .asr_00       = 0,            \
                  .srt_00       = 0,            \
                  .rtt_wr_00    = 0,            \
                  .dic_00       = 0,            \
                  .rtt_nom_00   = rttnom_40ohm, \
                  .pasr_01      = 0,            \
                  .asr_01       = 0,            \
                  .srt_01       = 0,            \
                  .rtt_wr_01    = 0,            \
                  .dic_01       = 0,            \
                  .rtt_nom_01   = 0,            \
                  .pasr_10      = 0,            \
                  .asr_10       = 0,            \
                  .srt_10       = 0,            \
                  .rtt_wr_10    = 0,            \
                  .dic_10       = 0,            \
                  .rtt_nom_10   = 0,            \
                  .pasr_11      = 0,            \
                  .asr_11       = 0,            \
                  .srt_11       = 0,            \
                  .rtt_wr_11    = 0,            \
                  .dic_11       = 0,            \
                  .rtt_nom_11   = 0,            \
        }                                       \
    }

 /* Some 2-rank dimms work better in the ep6300c board using 20 ohms
    for rtt_nom_01/11 at 1333 DDR */
#define OCTEON_EP6300C_MODEREG_PARAMS1_2RANK_2SLOT                     \
    { .cn63xx = { .pasr_00      = 0,                    \
                  .asr_00       = 0,                    \
                  .srt_00       = 0,                    \
                  .rtt_wr_00    = 0,                    \
                  .dic_00       = 0,                    \
                  .rtt_nom_00   = rttnom_120ohm,        \
                  .pasr_01      = 0,                    \
                  .asr_01       = 0,                    \
                  .srt_01       = 0,                    \
                  .rtt_wr_01    = 0,                    \
                  .dic_01       = 0,                    \
                  .rtt_nom_01   = rttnom_20ohm,         \
                  .pasr_10      = 0,                    \
                  .asr_10       = 0,                    \
                  .srt_10       = 0,                    \
                  .rtt_wr_10    = 0,                    \
                  .dic_10       = 0,                    \
                  .rtt_nom_10   = rttnom_120ohm,        \
                  .pasr_11      = 0,                    \
                  .asr_11       = 0,                    \
                  .srt_11       = 0,                    \
                  .rtt_wr_11    = 0,                    \
                  .dic_11       = 0,                    \
                  .rtt_nom_11   = rttnom_20ohm,         \
        }                                               \
    }

#define OCTEON_EP6300C_CN63XX_DRAM_ODT_1RANK_CONFIGURATION \
/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1            RODT_CTL    RODT_MASK    reserved */ \
/* =====   ======== ============== ========================================== ========= ============== ======== */ \
/*   1 */ {   0,    0x00000001ULL, OCTEON_EP6300C_MODEREG_PARAMS1_1RANK_1SLOT,    3,     0x00000000ULL,  0  },     \
/*   2 */ {   0,    0x00050005ULL, OCTEON_EP6300C_MODEREG_PARAMS1_1RANK_2SLOT,    3,     0x00010004ULL,  0  }

#define OCTEON_EP6300C_CN63XX_DRAM_ODT_2RANK_CONFIGURATION \
/* DIMMS   reserved  WODT_MASK                LMCX_MODEREG_PARAMS1            RODT_CTL    RODT_MASK    reserved */ \
/* =====   ======== ============== ========================================== ========= ============== ======== */ \
/*   1 */ {   0,    0x00000101ULL, OCTEON_EP6300C_MODEREG_PARAMS1_2RANK_1SLOT,    3,     0x00000000ULL,    0  },   \
/*   2 */ {   0,    0x06060909ULL, OCTEON_EP6300C_MODEREG_PARAMS1_2RANK_2SLOT,    3,     0x02020808ULL,    0  }


/* Construct a static initializer for the ddr_configuration_t variable that holds
** (almost) all of the information required for DDR initialization.
*/
/*
  The parameters below make up the custom_lmc_config data structure.
  This structure is used to customize the way that the LMC DRAM
  Controller is configured for a particular board design.

  The HRM describes LMC Read Leveling which supports automatic
  selection of per byte-lane delays.  When measuring the read delays
  the LMC configuration software sweeps through a range of settings
  for LMC0_COMP_CTL2[RODT_CTL], the Octeon II on-die-termination
  resistance and LMC0_MODEREG_PARAMS1[RTT_NOM_XX], the DRAM
  on-die-termination resistance.  The minimum and maximum parameters
  for rtt_nom_idx and rodt_ctl listed below determine the ranges of
  ODT settings used for the measurements.  Note that for rtt_nom an
  index is used into a sorted table rather than the direct csr setting
  in order to optimize the sweep.

   .min_rtt_nom_idx: 1=120ohms, 2=60ohms, 3=40ohms, 4=30ohms, 5=20ohms
   .max_rtt_nom_idx: 1=120ohms, 2=60ohms, 3=40ohms, 4=30ohms, 5=20ohms
   .min_rodt_ctl: 1=20ohms, 2=30ohms, 3=40ohms, 4=60ohms, 5=120ohms
   .max_rodt_ctl: 1=20ohms, 2=30ohms, 3=40ohms, 4=60ohms, 5=120ohms

   The settings below control the Octeon II drive strength for the CK,
   ADD/CMD, and DQ/DQS signals.  1=24ohms, 2=26.67ohms, 3=30ohms,
   4=34.3ohms, 5=40ohms, 6=48ohms, 6=60ohms.

   .dqx_ctl: Drive strength control for DDR_DQX/DDR_DQS_X_P/N drivers.
   .ck_ctl: Drive strength control for DDR_CK_X_P/DDR_DIMMX_CSX_L/DDR_DIMMX_ODT_X drivers.
   .cmd_ctl: Drive strength control for CMD/A/RESET_L/CKEX drivers.

   The LMC controller software selects the most optimal CAS Latency
   that complies with the appropriate SPD values and the frequency
   that the DRAMS are being operated.  When operating the DRAMs at
   frequencies substantially lower than their rated frequencies it
   might be necessary to limit the minimum CAS Latency the LMC
   controller software is allowed to select in order to make the DRAM
   work reliably.

   .min_cas_latency: Minimum allowed CAS Latency


   The value used for LMC0_RLEVEL_CTL[OFFSET_EN] determine how the
   read-leveling information that the Octeon II gathers is interpreted
   to determine the per-byte read delays.

   .offset_en: Value used for LMC0_RLEVEL_CTL[OFFSET_EN].
   .offset_udimm: Value used for LMC0_RLEVEL_CTL[OFFSET] for UDIMMS.
   .offset_rdimm: Value used for LMC0_RLEVEL_CTL[OFFSET] for RDIMMS.


   The LMC configuration software sweeps through a range of ODT
   settings while measuring the per-byte read delays.  During those
   measurements the software makes an assessment of the quality of the
   measurements in order to determine which measurements provide the
   most accurate delays.  The automatic settings provide the option to
   allow that same assessment to determine the most optimal RODT_CTL
   and/or RTT_NOM settings.

   The automatic approach might provide the best means to determine
   the settings used for initial poweron of a new design.  However,
   the final settings should be determined by board analysis, testing,
   and experience.

   .ddr_rtt_nom_auto: 1 means automatically set RTT_NOM value.
   .ddr_rodt_ctl_auto: 1 means automatically set RODT_CTL value.

   .rlevel_compute: Enables software interpretation of per-byte read
                    delays using the measurements collected by the
                    Octeon II rather than completely relying on the
                    Octeon II to determine the delays.  1=software
                    computation is recomended since a more complete
                    analysis is implemented in software.

   .rlevel_comp_offset: Defaults to 2 at run time for Octeon II. Don't
			change unless instructed differently by
			Cavium.

   .ddr2t_udimm:
   .ddr2t_rdimm: Turn on the DDR 2T mode. 2-cycle window for CMD and
                 address. This mode helps relieve setup time pressure
                 on the address and command bus. Please refer to
                 Micron's tech note tn_47_01 titled DDR2-533 Memory
                 Design Guide for Two Dimm Unbuffered Systems for
                 physical details.
*/

#define OCTEON_EP6300C_DDR_CONFIGURATION                                \
    /* Interface 0 */                                                   \
    {                                                                   \
	.custom_lmc_config = {                                          \
	.min_rtt_nom_idx	= 2,                                    \
	.max_rtt_nom_idx	= 5,                                    \
	.min_rodt_ctl		= 1,                                    \
	.max_rodt_ctl		= 4,                                    \
	.dqx_ctl		= 4,                                    \
	.ck_ctl			= 4,                                    \
	.cmd_ctl		= 4,                                    \
	.min_cas_latency	= 7,                                    \
	.offset_en 		= 1,                                    \
	.offset_udimm 		= 2,                                    \
	.offset_rdimm 		= 1,                                    \
	.ddr_rtt_nom_auto	= 0,                                    \
	.ddr_rodt_ctl_auto	= 0,                                    \
	.rlevel_compute 	= 1,                                    \
        .ddr2t_udimm 	 	= 1,                                    \
        .ddr2t_rdimm 	 	= 1,                                    \
        .fprch2 	 	= 1,                                    \
        .parity 	 	= 0},                                   \
        .dimm_config_table = {OCTEON_EP6300C_DRAM_SOCKET_CONFIGURATION0, DIMM_CONFIG_TERMINATOR}, \
        .unbuffered = {                                                 \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
        .registered = {                                                 \
            .ddr_board_delay = 0,                                       \
            .lmc_delay_clk = 0,                                         \
            .lmc_delay_cmd = 0,                                         \
            .lmc_delay_dq = 0},                                         \
        .odt_1rank_config = {                                           \
            CN63XX_DRAM_ODT_1RANK_CONFIGURATION},                       \
        .odt_2rank_config = {                                           \
            CN63XX_DRAM_ODT_2RANK_CONFIGURATION},                       \
     }

#define OCTEON_CONFIG_DFM (1)


#endif   /* __OCTEON_EP6300C_SHARED_H__ */
