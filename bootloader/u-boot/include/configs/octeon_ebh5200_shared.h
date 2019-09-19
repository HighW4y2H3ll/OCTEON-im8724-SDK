
#define OCTEON_EBH5200_EEPROM_TWSI_ADDR        (0x56)
#if defined(CONFIG_OCTEON_EBT5200)
# define OCTEON_EBT5200_EEPROM_TWSI_ADDR        (0x56)
#endif

/* SPD EEPROM addresses on EBH5200 board */
#define EBH5200_DIMM_0_SPD_TWSI_ADDR    0x50
#define EBH5200_DIMM_1_SPD_TWSI_ADDR    0x51

/* EBH5200 has two DRAM interfaces each with two DIMM sockets. DIMM 0
** must be populated on at least one interface. Each interface used
** must have at least DIMM 0 populated.
*/
#define OCTEON_EBH5200_DRAM_SOCKET_CONFIGURATION \
    {{EBH5200_DIMM_0_SPD_TWSI_ADDR, 0}, {NULL, NULL}}, \
    {{EBH5200_DIMM_1_SPD_TWSI_ADDR, 0}, {NULL, NULL}}


#if 0

/* Use compiled in spd value */
#define EBH5200_SPD_VALUES\
    0x80,\
    0x08,\
    0x08,\
    0x0e,\
    0x0a,\
    0x61,\
    0x48,\
    0x00,\
    0x05,\
    0x30,\
    0x45,\
    0x06,\
    0x82,\
    0x08,\
    0x08,\
    0x00,\
    0x0c,\
    0x04,\
    0x38,\
    0x01,\
    0x01,\
    0x05,\
    0x07,\
    0x3d,\
    0x50,\
    0x50,\
    0x60,\
    0x3c,\
    0x1e,\
    0x3c,\
    0x27,\
    0x80,\
    0x20,\
    0x27,\
    0x10,\
    0x17,\
    0x3c,\
    0x1e,\
    0x1e,\
    0x00,\
    0x00,\
    0x36,\
    0x69,\
    0x80,\
    0x18,\
    0x22,\
    0x0f,\
    0x00,\
    0x00,\
    0x03,\
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
    0x96


#undef OCTEON_EBH5200_DRAM_SOCKET_CONFIGURATION

/* Use variable defined in lib_octeon_shared.c */
#define OCTEON_EBH5200_DRAM_SOCKET_CONFIGURATION \
    {{0, 0}, {ebh5200_spd_values, NULL}}, \
    {{0, 0}, {ebh5200_spd_values, NULL}}

#endif  /* #if 0 */


/*
** CN52xx Registered configuration
*/
#define OCTEON_CN52XX_EBH5200_DDR_BOARD_DELAY		4900
#define OCTEON_CN52XX_EBH5200_LMC_DELAY_CLK		3
#define OCTEON_CN52XX_EBH5200_LMC_DELAY_CMD		0
#define OCTEON_CN52XX_EBH5200_LMC_DELAY_DQ		2

/*
** CN52xx Unbuffered configuration
*/
#define OCTEON_CN52XX_EBH5200_UNB_DDR_BOARD_DELAY	5900
#define OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_CLK		12
#define OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_CMD  	0
#define OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_DQ		11



#define OCTEON_EBH5200_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_EBH5200_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN52XX_EBH5200_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN52XX_EBH5200_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN52XX_EBH5200_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN52XX_EBH5200_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN52XX_EBH5200_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN52XX_EBH5200_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN52XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN52XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}
