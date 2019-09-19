#ifndef __OCTEON_EBH3100_SHARED_H__
#define __OCTEON_EBH3100_SHARED_H__

#define OCTEON_EBH3100_DRAM_SOCKET_CONFIGURATION \
	{{0x50, 0}, {NULL, NULL}},\
	{{0x51, 0}, {NULL, NULL}}


#define OCTEON_EBH3100_EEPROM_TWSI_ADDR        0x56

/* Board delay in picoseconds */
#define OCTEON_EBH3100_DDR_BOARD_DELAY_REV_1		4200
#define OCTEON_EBH3100_DDR_BOARD_DELAY_REV_2		6000


#define OCTEON_EBH3100_DDR_CONFIGURATION_REV1   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBH3100_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_EBH3100_DDR_BOARD_DELAY_REV_1,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_EBH3100_DDR_BOARD_DELAY_REV_1,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },   \
    .odt_1rank_config = {   \
        CN31XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN31XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#define OCTEON_EBH3100_DDR_CONFIGURATION_REV2   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBH3100_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_EBH3100_DDR_BOARD_DELAY_REV_2,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_EBH3100_DDR_BOARD_DELAY_REV_2,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },   \
    .odt_1rank_config = {   \
        CN31XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN31XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}


#endif   /* __OCTEON_EBH3100_SHARED_H__ */
