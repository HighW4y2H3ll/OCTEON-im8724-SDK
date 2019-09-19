
#ifndef __OCTEON_NICPRO2_SHARED_H__
#define __OCTEON_NICPRO2_SHARED_H__


#define OCTEON_NICPRO2_EEPROM_ADDR  (0x53)

/* SPD EEPROM addresses on NICPRO2 board */
#define NICPRO2_DIMM_0_SPD_TWSI_ADDR    0x51
#define NICPRO2_DIMM_1_SPD_TWSI_ADDR    0x52
#define NICPRO2_DIMM_2_SPD_TWSI_ADDR	0x55
#define NICPRO2_DIMM_3_SPD_TWSI_ADDR	0x56

/* NIC Pro2 has two pair of DIMM sockets. DIMM 0 must be populated.
** Populating DIMM 1 enables the 128/144-bit interface.
*/
#define OCTEON_NICPRO2_DRAM_SOCKET_CONFIGURATION \
    {{NICPRO2_DIMM_0_SPD_TWSI_ADDR, NICPRO2_DIMM_1_SPD_TWSI_ADDR}, {NULL, NULL}}, \
    {{NICPRO2_DIMM_2_SPD_TWSI_ADDR, NICPRO2_DIMM_3_SPD_TWSI_ADDR}, {NULL, NULL}}


/*
** CN58xx Registered configuration
**  @ DDR-667, 4 RDIMMs:  TSKW/HC/QC = 2/0/0 (4491-5240), LMC_DELAY[CLK,CMD,DQ] = 8,0,5
**  @ DDR-800, 2 RDIMMs:  TSKW/HC/QC = 2/0/1 (4375-5000), LMC_DELAY[CLK,CMD,DQ] = 5,0,5
*/
#define OCTEON_CN58XX_NICPRO2_DDR_BOARD_DELAY		4900
#define OCTEON_CN58XX_NICPRO2_LMC_DELAY_CLK		7
#define OCTEON_CN58XX_NICPRO2_LMC_DELAY_CMD		0
#define OCTEON_CN58XX_NICPRO2_LMC_DELAY_DQ		3

/*
** CN58xx Unbuffered configuration (not extensively validated)
*/
#define OCTEON_CN58XX_NICPRO2_UNB_DDR_BOARD_DELAY	4800
#define OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_CLK		12
#define OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_CMD  	0
#define OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_DQ		7




#define OCTEON_NICPRO2_DDR_CONFIGURATION   \
/* Interface 0 */   \
{   \
    .dimm_config_table = {OCTEON_NICPRO2_DRAM_SOCKET_CONFIGURATION, DIMM_CONFIG_TERMINATOR},   \
    .unbuffered = {   \
        .ddr_board_delay = OCTEON_CN58XX_NICPRO2_UNB_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_NICPRO2_UNB_LMC_DELAY_DQ,   \
        },    \
    .registered = {   \
        .ddr_board_delay = OCTEON_CN58XX_NICPRO2_DDR_BOARD_DELAY,   \
        .lmc_delay_clk = OCTEON_CN58XX_NICPRO2_LMC_DELAY_CLK,   \
        .lmc_delay_cmd = OCTEON_CN58XX_NICPRO2_LMC_DELAY_CMD,   \
        .lmc_delay_dq = OCTEON_CN58XX_NICPRO2_LMC_DELAY_DQ,   \
        },   \
    .odt_1rank_config = {   \
        CN58XX_DRAM_ODT_1RANK_CONFIGURATION,   \
        },   \
    .odt_2rank_config = {   \
        CN58XX_DRAM_ODT_2RANK_CONFIGURATION,   \
        },   \
}

#endif /* __OCTEON_NICPRO2_SHARED_H__ */
