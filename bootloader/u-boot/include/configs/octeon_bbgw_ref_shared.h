

#define BBGW_REF_BOARD_EEPROM_TWSI_ADDR        0x52


/* Static SPD values defined here since the board does not have a DIMM */
#define BBGW_REF_SPD_VALUES  \
0x80,\
0x08,\
0x08,\
0x0d,\
0x0a,\
0x00,\
0x20,\
0x00,\
0x05,\
0x3D,\
0x50,\
0x00,\
0x82,\
0x10,\
0x00,\
0x00,\
0x0c,\
0x04,\
0x38,\
0x00,\
0x02,\
0x00,\
0x01,\
0x3d,\
0x50,\
0x50,\
0x50,\
0x3c,\
0x1e,\
0x3c,\
0x28,\
0x40,\
0x25,\
0x38,\
0x10,\
0x23,\
0x3c,\
0x1e,\
0x1e,\
0x00,\
0x00,\
0x37,\
0x69,\
0x80,\
0x1E,\
0x28,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x00,\
0x12,\
0x01










#define OCTEON_BBGW_REF_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {{{0, 0}, {bbgw_ref_spd_values, NULL}}, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = 2300,   \
        .lmc_delay_clk = 6,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 8,   \
        },    \
    .registered = {   \
        .ddr_board_delay = 2300,   \
        .lmc_delay_clk = 6,   \
        .lmc_delay_cmd = 0,   \
        .lmc_delay_dq = 8,   \
        },   \
    .odt_1rank_config = {   \
        CN50XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN50XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}
