#ifndef __OCTEON_EBB5600_SHARED_H__
#define __OCTEON_EBB5600_SHARED_H__



/*
** CN56xx Registered configuration
*/
#define OCTEON_EBB5600_DDR_BOARD_DELAY		4200
#define OCTEON_EBB5600_LMC_DELAY_CLK		7
#define OCTEON_EBB5600_LMC_DELAY_CMD		0
#define OCTEON_EBB5600_LMC_DELAY_DQ		6

/*
** CN56xx Unbuffered configuration
*/
#define OCTEON_EBB5600_UNB_DDR_BOARD_DELAY	4600
#define OCTEON_EBB5600_UNB_LMC_DELAY_CLK	13
#define OCTEON_EBB5600_UNB_LMC_DELAY_CMD  	0
#define OCTEON_EBB5600_UNB_LMC_DELAY_DQ		8

#define OCTEON_EBB5600_DRAM_SOCKET_CONFIGURATION0 \
    {{0x50, 0x0}, {NULL, NULL}},{{0x51, 0x0}, {NULL, NULL}}

#define OCTEON_EBB5600_DRAM_SOCKET_CONFIGURATION1 \
    {{0x52, 0x0}, {NULL, NULL}},{{0x53, 0x0}, {NULL, NULL}}




#define OCTEON_EBB5600_BOARD_EEPROM_TWSI_ADDR        (0x56)
  

/* Construct a static initializer for the ddr_configuration_t variable that holds
** (almost) all of the information required for DDR initialization.
*/
#define OCTEON_EBB5600_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBB5600_DRAM_SOCKET_CONFIGURATION0, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_EBB5600_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBB5600_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBB5600_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBB5600_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_EBB5600_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBB5600_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBB5600_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBB5600_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN56XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN56XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
},   \
/* Interface 1 */   \
{   \
    .dimm_config_table = {OCTEON_EBB5600_DRAM_SOCKET_CONFIGURATION1, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_EBB5600_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBB5600_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBB5600_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBB5600_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_EBB5600_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_EBB5600_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_EBB5600_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_EBB5600_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN56XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN56XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}   \




#endif   /* __OCTEON_EBB5600_SHARED_H__ */
