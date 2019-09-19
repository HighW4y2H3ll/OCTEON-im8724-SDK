
#ifndef __OCTEON_EBT5800_SHARED_H__
#define __OCTEON_EBT5800_SHARED_H__


#define OCTEON_EBT5800_EEPROM_ADDR  (0x52)

/* SPD EEPROM addresses on EBT5800 board */
#define EBT5800_DIMM_0_SPD_TWSI_ADDR    0x50
#define EBT5800_DIMM_1_SPD_TWSI_ADDR    0x51

/* EBT5800 has one pair of DIMM sockets. DIMM 0 must be populated.
** Populating DIMM 1 enables the 128/144-bit interface.
*/
#define OCTEON_EBT5800_DRAM_SOCKET_CONFIGURATION \
    {{EBT5800_DIMM_0_SPD_TWSI_ADDR, EBT5800_DIMM_1_SPD_TWSI_ADDR}, {NULL, NULL}}


/*
** CN58xx Registered configuration
*/
#define OCTEON_CN58XX_EBT5800_DDR_BOARD_DELAY		3500
#define OCTEON_CN58XX_EBT5800_LMC_DELAY_CLK		7
#define OCTEON_CN58XX_EBT5800_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_EBT5800_LMC_DELAY_DQ		4

/*
** CN58xx Unbuffered configuration
*/
#define OCTEON_CN58XX_EBT5800_UNB_DDR_BOARD_DELAY	4000
#define OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_CLK		12
#define OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_CMD  	0
#define OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_DQ		7

/*
** CN38xx Registered configuration
*/
#define OCTEON_CN38XX_EBT5800_DDR_BOARD_DELAY		4800
#define OCTEON_CN38XX_EBT5800_LMC_DELAY_CLK		0
#define OCTEON_CN38XX_EBT5800_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_EBT5800_LMC_DELAY_DQ		2

/*
** CN38xx Unbuffered configuration
*/
#define OCTEON_CN38XX_EBT5800_UNB_DDR_BOARD_DELAY	5000
#define OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_CLK		6
#define OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_DQ		5
#endif


#define OCTEON_EBT5800_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBT5800_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN58XX_EBT5800_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_EBT5800_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN58XX_EBT5800_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_EBT5800_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_EBT5800_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_EBT5800_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN58XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN58XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#define OCTEON_EBT5800_CN38XX_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBT5800_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN38XX_EBT5800_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_EBT5800_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN38XX_EBT5800_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_EBT5800_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_EBT5800_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_EBT5800_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN58XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN58XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}
