#ifndef __OCTEON_NIC_XLE_4G_SHARED_H__
#define __OCTEON_NIC_XLE_4G_SHARED_H__

#define OCTEON_NIC_XLE_4G_BOARD_DELAY   5500
#define OCTEON_NIC_XLE_4G_LMC_DELAY_CLK		7
#define OCTEON_NIC_XLE_4G_LMC_DELAY_CMD		0
#define OCTEON_NIC_XLE_4G_LMC_DELAY_DQ		7

#define OCTEON_NIC_XLE_4G_DRAM_SOCKET_CONFIGURATION0 \
    {{0x50, 0x0}, {NULL, NULL}}

#define OCTEON_NIC_XLE_4G_DRAM_SOCKET_CONFIGURATION1 \
    {{0x52, 0x0}, {NULL, NULL}}




#define OCTEON_NIC_XLE_4G_EEPROM_ADDR  (0x53)
  
#define NIC_XLE_DRAM_ODT_1RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ==============  ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000001,  {.u64=0x0000},  {.u64=0x0000},    2,   0x00000001,  0  },  \
    /*   2 */ {   2,    0x00010001,  {.u64=0x0000},  {.u64=0x0000},    2,   0x00010001,  0  },  \
    /*   3 */ {   2,    0x01040104,  {.u64=0x0000},  {.u64=0x0000},    2,   0x01040104,  0  },  \
    /*   4 */ {   2,    0x01040104,  {.u64=0x0000},  {.u64=0x0000},    2,   0x01040104,  0  }

#define NIC_XLE_DRAM_ODT_2RANK_CONFIGURATION \
    /* DIMMS   RODT_ENA LMC_ODT_CTL  reserved        reserved       QS_DIC RODT_CTL DIC */ \
    /* =====   ======== ============ ==============  ============== ====== ======== === */ \
    /*   1 */ {   2,    0x00000011,  {.u64=0x0000},  {.u64=0x0000},    2,   0x00000011,  0  },  \
    /*   2 */ {   2,    0x00110011,  {.u64=0x0000},  {.u64=0x0000},    2,   0x00110011,  0  },  \
    /*   3 */ {   2,    0x11441144,  {.u64=0x0000},  {.u64=0x0000},    2,   0x11441144,  0  },  \
    /*   4 */ {   2,    0x11441144,  {.u64=0x0000},  {.u64=0x0000},    2,   0x11441144,  0  }

/* Construct a static initializer for the ddr_configuration_t variable that holds
** (almost) all of the information required for DDR initialization.
*/
#define OCTEON_NIC_XLE_4G_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_NIC_XLE_4G_DRAM_SOCKET_CONFIGURATION0, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_NIC_XLE_4G_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_NIC_XLE_4G_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_NIC_XLE_4G_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_NIC_XLE_4G_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_NIC_XLE_4G_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_NIC_XLE_4G_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_NIC_XLE_4G_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_NIC_XLE_4G_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        NIC_XLE_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        NIC_XLE_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
},   \
/* Interface 1 */   \
{   \
    .dimm_config_table = {OCTEON_NIC_XLE_4G_DRAM_SOCKET_CONFIGURATION1, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_NIC_XLE_4G_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_NIC_XLE_4G_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_NIC_XLE_4G_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_NIC_XLE_4G_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_NIC_XLE_4G_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_NIC_XLE_4G_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_NIC_XLE_4G_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_NIC_XLE_4G_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        NIC_XLE_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        NIC_XLE_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}   \


#endif   /* __OCTEON_NIC_XLE_4G_SHARED_H__ */
