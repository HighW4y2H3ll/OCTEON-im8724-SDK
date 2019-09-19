
#ifndef __OCTEON_EBT3000_SHARED_H__
#define __OCTEON_EBT3000_SHARED_H__

#define OCTEON_EBT3000_EEPROM_ADDR  (0x52)

/* EBT3000 has one pair of DIMM sockets. DIMM 0 must be populated.
** Populating DIMM 1 enables the 128/144-bit interface.
*/
#define OCTEON_EBT3000_DRAM_SOCKET_CONFIGURATION \
	{{0x50, 0x51}, {NULL, NULL}}


/*
** CN38xx Registered configuration
*/
#define OCTEON_EBT3000_DDR_BOARD_DELAY		6000
#define OCTEON_EBT3000_LMC_DELAY_CLK		2
#define OCTEON_EBT3000_LMC_DELAY_CMD		0
#define OCTEON_EBT3000_LMC_DELAY_DQ		9

/*
** CN38xx Unbuffered configuration
*/
#define OCTEON_EBT3000_UNB_DDR_BOARD_DELAY	6300
#define OCTEON_EBT3000_UNB_LMC_DELAY_CLK	4
#define OCTEON_EBT3000_UNB_LMC_DELAY_CMD	0
#define OCTEON_EBT3000_UNB_LMC_DELAY_DQ		9


#define OCTEON_EBT3000_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBT3000_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_EBT3000_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBT3000_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBT3000_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBT3000_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_EBT3000_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBT3000_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBT3000_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBT3000_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN38XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN38XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}


#endif
