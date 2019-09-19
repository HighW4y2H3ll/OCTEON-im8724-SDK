
#ifndef __OCTEON_WSX16_SHARED_H__
#define __OCTEON_WSX16_SHARED_H__

#define OCTEON_WSX16_EEPROM_ADDR  (0x57)

/* WSX16 has two pairs of DIMM sockets. DIMM 0 must be populated.
** Populating DIMM 1 enables the 128/144-bit interface.
*/
#define OCTEON_WSX16_DRAM_SOCKET_CONFIGURATION \
  {{0x50, 0x52}, {NULL, NULL}}, {{0x51, 0x56}, {NULL, NULL}}


/*
** CN38xx Registered configuration
*/
#define OCTEON_CN38XX_WSX16_DDR_BOARD_DELAY		4375 /*4875*/
#define OCTEON_CN38XX_WSX16_LMC_DELAY_CLK		0
#define OCTEON_CN38XX_WSX16_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_WSX16_LMC_DELAY_DQ		0

/*
** CN38xx Unbuffered configuration
*/
#define OCTEON_CN38XX_WSX16_UNB_DDR_BOARD_DELAY		4875
#define OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_CLK		6
#define OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_DQ		9

/*
** CN58xx Registered configuration
*/
#define OCTEON_CN58XX_WSX16_DDR_BOARD_DELAY		4800
#define OCTEON_CN58XX_WSX16_LMC_DELAY_CLK		0
#define OCTEON_CN58XX_WSX16_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_WSX16_LMC_DELAY_DQ		0

/*
** CN58xx Unbuffered configuration
*/
#define OCTEON_CN58XX_WSX16_UNB_DDR_BOARD_DELAY		5300
#define OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_CLK		4
#define OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_DQ		9


#define OCTEON_CN38XX_WSX16_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_WSX16_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN38XX_WSX16_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_WSX16_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN38XX_WSX16_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_WSX16_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_WSX16_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_WSX16_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN38XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN38XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#define OCTEON_CN58XX_WSX16_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_WSX16_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN58XX_WSX16_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_WSX16_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN58XX_WSX16_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_WSX16_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_WSX16_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_WSX16_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN58XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN58XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}


#endif
