
#ifndef __OCTEON_NIC10E_66_SHARED_H__
#define __OCTEON_NIC10E_66_SHARED_H__

#include "octeon_nic10e_shared.h"

#define OCTEON_NIC10E_66_REV3_DRAM_SOCKET_CONFIGURATION0 \
    {{0x52, 0x0}, {NULL, NULL}}



/*
  The parameters below make up the custom_lmc_config data structure.
  This structure is used to customize the way that the LMC DRAM
  Controller is configured for a particular board design.

  Refer to the file lib_octeon_board_table_entry.h for a description
  of the custom board settings.  It is usually kept in the following
  location... arch/mips/include/asm/arch-octeon/

*/

#define OCTEON_NIC10E_66_REV3_DDR_CONFIGURATION                 \
    /* Interface 0 */                                           \
    {                                                           \
        .custom_lmc_config = {                                  \
            .min_rtt_nom_idx	= 1,                            \
            .max_rtt_nom_idx	= 5,                            \
            .min_rodt_ctl	= 1,                            \
            .max_rodt_ctl	= 5,                            \
            .dqx_ctl		= 4,                            \
            .ck_ctl		= 4,                            \
            .cmd_ctl		= 4,                            \
            .min_cas_latency	= 0,                            \
            .offset_en 		= 0,                            \
            .offset_udimm	= 0,                            \
            .offset_rdimm	= 0,                            \
            .ddr_rtt_nom_auto	= 0,                            \
            .ddr_rodt_ctl_auto	= 0,                            \
            .rlevel_compute 	= 1,                            \
            .ddr2t_udimm	= 1,                            \
            .ddr2t_rdimm	= 1,                            \
            .fprch2		= 1,                            \
            .parity		= 0},                           \
            .dimm_config_table = {                              \
            OCTEON_NIC10E_66_REV3_DRAM_SOCKET_CONFIGURATION0,           \
            DIMM_CONFIG_TERMINATOR},                            \
                 .unbuffered = {                                \
            .ddr_board_delay = 0,                               \
            .lmc_delay_clk = 0,                                 \
            .lmc_delay_cmd = 0,                                 \
            .lmc_delay_dq = 0},                                 \
                      .registered = {                           \
            .ddr_board_delay = 0,                               \
            .lmc_delay_clk = 0,                                 \
            .lmc_delay_cmd = 0,                                 \
            .lmc_delay_dq = 0},                                 \
                           .odt_1rank_config = {                \
            OCTEON_NIC10E_CN63XX_DRAM_ODT_1RANK_CONFIGURATION}, \
                                .odt_2rank_config = {           \
            OCTEON_NIC10E_CN63XX_DRAM_ODT_2RANK_CONFIGURATION}  \
    }

#endif   /* __OCTEON_NIC10E_66_SHARED_H__ */
