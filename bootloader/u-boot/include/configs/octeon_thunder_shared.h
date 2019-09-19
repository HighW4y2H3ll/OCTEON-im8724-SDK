
#ifndef __OCTEON_THUNDER_SHARED_H__
#define __OCTEON_THUNDER_SHARED_H__

#define OCTEON_THUNDER_EEPROM_ADDR  (0x53)

#define OCTEON_THUNDER_DRAM_SOCKET_CONFIGURATION \
	{{0x51, 0x52}, {NULL, NULL}}

/* Board delay in picoseconds */
#define OCTEON_THUNDER_DDR_BOARD_DELAY		3815   /* First guess was 3815 */







#define OCTEON_THUNDER_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_THUNDER_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_THUNDER_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_THUNDER_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = 0,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 0,   \
        },   \
    .odt_1rank_config = {   \
        CN38XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN38XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#endif
