
#ifndef __OCTEON_CN3010_EVB_HS5_SHARED_H__
#define __OCTEON_CN3010_EVB_HS5_SHARED_H__
/*-----------------------------------------------------------------------
 * DRAM Module Organization
 * 
 * Octeon CN30XX can be configured to use one to four DIMM's providing
 * a 64/72-bit interface.  This structure contains the TWSI addresses
 * used to access the DIMM's Serial Presence Detect (SPD) EPROMS and
 * it also implies which DIMM socket organization is used on the
 * board.  Software uses this to detect the presence of DIMM's plugged
 * into the sockets, compute the total memory capacity, and configure
 * DRAM controller.  All DIMM's must be identical.
 */

/* CN3010_EVB_HS5 has two DIMM sockets. DIMM 0 must be populated.
*/
#define OCTEON_CN3010_EVB_HS5_DRAM_SOCKET_CONFIGURATION \
	{{0x50, 0}, {NULL, NULL}},\
	{{0x51, 0}, {NULL, NULL}}


#define CN3010_EVB_HS5_EEPROM_TWSI_ADDR        0x56
/* Board delay in picoseconds */


/*
** Clock, command, and DQ lines can be delayed independently to adjust
** timing more precisely.  Each delay unit represents about 50 ps of
** delay.
*/
#define OCTEON_CN30XX_CN3010_EVB_HS5_DDR_BOARD_DELAY		4200
#define OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CLK		4
#define OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CMD		0
#define OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_DQ		0

/*
** CN50xx Registered configuration
*/
#define OCTEON_CN50XX_CN3010_EVB_HS5_DDR_BOARD_DELAY		3600
#define OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_CLK		3
#define OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_CMD		0
#define OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_DQ		3

/*
** CN50xx Unbuffered configuration
*/
#define OCTEON_CN50XX_CN3010_EVB_HS5_UNB_DDR_BOARD_DELAY	4500
#define OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_CLK		9
#define OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_CMD  	0
#define OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_DQ		11

#define OCTEON_CN50XX_CN3010_EVB_HS5_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_CN3010_EVB_HS5_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN50XX_CN3010_EVB_HS5_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN50XX_CN3010_EVB_HS5_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN50XX_CN3010_EVB_HS5_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN50XX_CN3010_EVB_HS5_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN50XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN50XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}


#define OCTEON_CN30XX_CN3010_EVB_HS5_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_CN3010_EVB_HS5_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN30XX_CN3010_EVB_HS5_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN30XX_CN3010_EVB_HS5_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN30XX_CN3010_EVB_HS5_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN30XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN30XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#endif   /* __OCTEON_CN3010_EVB_HS5_SHARED_H__ */
