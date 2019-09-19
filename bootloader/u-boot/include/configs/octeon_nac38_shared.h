
#ifndef __OCTEON_NAC38_SHARED_H__
#define __OCTEON_NAC38_SHARED_H__

#define OCTEON_NAC38_BOARD_EEPROM_TWSI_ADDR		(0x56)

/* NAC38 has one pair of DIMM sockets. DIMM 0 must be populated.
** Populating DIMM 1 enables the 128/144-bit interface.
*/
#define OCTEON_NAC38_DRAM_SOCKET_CONFIGURATION \
	{{0x50, 0x51}, {NULL, NULL}}, {{0x52, 0x53}, {NULL, NULL}}

/*
** CN38xx Registered configuration
*/
#define OCTEON_CN38XX_NAC38_DDR_BOARD_DELAY		5200
#define OCTEON_CN38XX_NAC38_LMC_DELAY_CLK		4
#define OCTEON_CN38XX_NAC38_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_NAC38_LMC_DELAY_DQ		4

/*
** CN38xx Unbuffered configuration
*/
#define OCTEON_CN38XX_NAC38_UNB_DDR_BOARD_DELAY		5000
#define OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_CLK		6
#define OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_CMD		0
#define OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_DQ		5

/*
** CN58xx Registered configuration
*/
#define OCTEON_CN58XX_NAC38_DDR_BOARD_DELAY		3500
#define OCTEON_CN58XX_NAC38_LMC_DELAY_CLK		9
#define OCTEON_CN58XX_NAC38_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_NAC38_LMC_DELAY_DQ		5

/*
** CN58xx Unbuffered configuration
*/
#define OCTEON_CN58XX_NAC38_UNB_DDR_BOARD_DELAY		4000
#define OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_CLK		12
#define OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_DQ		7


#define OCTEON_CN38XX_NAC38_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_NAC38_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN38XX_NAC38_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_NAC38_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN38XX_NAC38_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN38XX_NAC38_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN38XX_NAC38_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN38XX_NAC38_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN38XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN38XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#define OCTEON_CN58XX_NAC38_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_NAC38_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN58XX_NAC38_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_NAC38_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN58XX_NAC38_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_NAC38_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_NAC38_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_NAC38_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN58XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN58XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}


#endif
