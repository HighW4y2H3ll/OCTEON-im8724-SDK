#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <cvmx.h>
#include <cvmx-platform.h>

#include "board_mgmt.h"
#include "mdio_octeon.h"
#include "im8724_phy.h"


#define CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL       0x1644
#define CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL       0x160A
#define CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL       0x1644
#define CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL       0x160A

#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL0 0x160C
#define CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL1 0x160B
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL1 0x1645
#define CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL0 0x1646

#define CS4318_PM_STATS_ACCESS                       0x3408
#define CS4318_PM_STATS_DATA0                        0x340B

#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA 0x108F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB 0x1090
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB        0x12AC
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB        0x12AD
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB        0x12B0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB        0x1AAC
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB        0x1AAD
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB        0x1AB0

#define LMAC_PDU_STATS                               1

typedef struct {
        cvmx_bgxx_cmrx_rx_stat0_t       rx_stat0;
        cvmx_bgxx_cmrx_tx_stat6_t       tx_stat6;
        cvmx_bgxx_cmrx_tx_stat7_t       tx_stat7;
        cvmx_bgxx_cmrx_tx_stat8_t       tx_stat8;
        cvmx_bgxx_cmrx_tx_stat9_t       tx_stat9;
        cvmx_bgxx_cmrx_tx_stat10_t      tx_stat10;
        cvmx_bgxx_cmrx_tx_stat11_t      tx_stat11;
        cvmx_bgxx_cmrx_tx_stat12_t      tx_stat12;
        cvmx_bgxx_cmrx_tx_stat13_t      tx_stat13;
} lmac_pdu_stats_t;

typedef union {
  struct {
    uint16_t access         :1;
    uint16_t wr_rd          :1;
    uint16_t rsrvd1         :4;
    uint16_t cntr_id        :10;
  } bf ;
  uint16_t   wrd;
} CS4318_PM_STATS_ACCESS_t;

int sgmii_mode = 0;
int fp_ports_num = MAX_FRONTPANEL_PORTS;
int quad_mode[MAX_SLOTS_NUM];

static int vs8221_reg_get(int addr, uint16_t* data);
static int vs8221_reg_set(int addr, uint16_t data);
static int cs4318_reg_get(int slice, int addr, uint16_t* data);
#ifndef LMAC_PDU_STATS
static int cs4318_reg_set(int slice, int addr, uint16_t data);
#endif
static int cs4343_reg_get(int bus, int slice, int addr, uint16_t* data);
static int cs4343_reg_set(int bus, int slice, int addr, uint16_t data);

int is_sgmii_board_mode(void)
{
    cvmx_bgxx_cmr_global_config_t gconfig;
    cvmx_gserx_phy_ctl_t gserx_phy_ctl;

    gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
    gserx_phy_ctl.u64 = cvmx_read_csr_node(0, CVMX_GSERX_PHY_CTL(3));
    if (gconfig.s.pmux_sds_sel && gserx_phy_ctl.s.phy_pd == 0) {
        fp_ports_num = MAX_FRONTPANEL_PORTS / 2;
        return 1;
    }
    return 0;
}

typedef struct {
    uint32_t        module;
    uint32_t        sfp;
    uint32_t        eport;
    bgx_lane_map_t  bgx_lane;
} eport_sfp_map_t;

static eport_sfp_map_t eport_sfp_map[MAX_MODULES_NUM][MAX_SFP_NUM] = {
   {{0, 0, 0, {.bgx = 0, .indx = 0}}, {0, 1, 1, {.bgx = 0,.indx = 1}}, {0, 2, 2, {.bgx = 1,.indx = 2}}, { 0, 3, 3, {.bgx = 1,.indx = 3}},
    {0, 4, -1,{.bgx = -1,.indx = -1}}, {0, 5, -1,{.bgx = -1,.indx = -1}}, {0, 6, -1,{.bgx = -1,.indx = -1}}, {0, 7, -1,{.bgx = -1,.indx = -1}}},
   {{ 1, 0, 15, {.bgx = 5,.indx = 0}}, { 1, 1, 16, {.bgx = 5,.indx = 1}}, {1, 2, 17, {.bgx = 5,.indx = 2}}, { 1, 3, 18, {.bgx = 5,.indx = 3}},
    {1, 4, 11, {.bgx = 4,.indx = 0}}, {1, 5, 12, {.bgx = 4,.indx = 1}}, {1, 6, 13, {.bgx = 4,.indx = 2}}, {1, 7, 14, {.bgx = 4,.indx = 3}}},
   {{ 2, 0, 7, {.bgx = 3,.indx = 0}}, { 2, 1, 8, {.bgx = 3,.indx = 1}}, {2, 2, 9, {.bgx = 3,.indx = 2}}, { 2, 3, 10, {.bgx = 3,.indx = 3}},
    {2, 4, 3, {.bgx = 2,.indx = 0}}, {2, 5, 4, {.bgx = 2,.indx = 1}}, {2, 6, 5, {.bgx = 2,.indx = 2}}, {2, 7, 6, {.bgx = 2,.indx = 3}}},
};

static enum cortina_slice_mode media_mode[MAX_SLOTS_NUM][MAX_SLICES_NUM];

static const char *sfp_mode_to_str[] = { "SR", "CX", "1000Base-X", "Unknown" };

void pim_set_defines(void)
{
    cvmx_gserx_cfg_t cfg;
    int slot, slice;

    for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
        cfg.u64 = cvmx_read_csr_node(0, CVMX_GSERX_CFG(slot ? 4 : 6));
        quad_mode[slot] = !!cfg.s.bgx_quad;
        debug(9, "quad_mode[%d]: %u\n", slot, quad_mode[slot]);
        for (slice = 0; slice < MAX_SLICES_NUM; ++slice)
            media_mode[slot][slice] = CORTINA_SLICE_UNKNOWN;
    }
}

static int cs4343_get_ctrl(uint16_t slot, uint16_t slice,
      enum cortina_slice_mode mode, uint16_t *ctrla, uint16_t *ctrlb)
{
    static const uint16_t tx_out_ctrla_values[CORTINA_SLICE_MAX][MAX_SLICES_NUM] = {
        {  /* SR values */
           0x3810, 0x3816, 0x3817, 0x3818, 0x381c, 0x381c, 0x3819, 0x3819
        },
        {  /* CX values */
           0x3819, 0x3819, 0x3819, 0x3819, 0x381c, 0x381b, 0x3819, 0x3819
        },
        {  /* 1000Base-X values */
           0x3810, 0x3816, 0x3817, 0x3818, 0x381c, 0x381c, 0x3819, 0x3819
        }
    };
    static const uint16_t tx_out_ctrlb_values[CORTINA_SLICE_MAX][MAX_SLICES_NUM] = {
        {
           0x107, 0x106, 0x106, 0x105, 0x108, 0x108, 0x106, 0x107
        },
        {
           0x107, 0x107, 0x105, 0x105, 0x10c, 0x108, 0x106, 0x105
        },
        {
           0x107, 0x106, 0x106, 0x105, 0x108, 0x108, 0x106, 0x107
        }
    };
    *ctrla = tx_out_ctrla_values[mode][slice];
    *ctrlb = tx_out_ctrlb_values[mode][slice];
    debug(9, "(%d:%d) ctrla: 0x%x, ctrlb: 0x%x\n", slot, slice, *ctrla, *ctrlb);
    return 0;
}

/* Changes the mode of a CS4343 PHY between CX, SR and 1000Base-X modes */
static int cs4343_set_mode_slice(uint16_t slot, uint16_t slice, enum cortina_slice_mode mode)
{
    uint16_t ctrla, ctrlb, val;
    int bus = slot ? 3 : 2;
    int status = 0;

    cs4343_get_ctrl(slot, slice, mode, &ctrla, &ctrlb);


    switch (media_mode[slot][slice]) {
        case CORTINA_SLICE_SR: {
            switch (mode) {
                case CORTINA_SLICE_SR:
                    debug(9, "Changing mode from SR to SR\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    break;
                 case CORTINA_SLICE_CX:
                    debug(9, "Changing mode from SR to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from SR to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    break;
                default:
                    break;
            }
            break;
        }
        case CORTINA_SLICE_CX: {
            switch (mode) {
                case CORTINA_SLICE_SR:
                    debug(9, "Changing from CX to SR\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from CX to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from CX to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    break;
                default:
                    break;
            }
            break;
        }
        case CORTINA_SLICE_1000X: {
            switch (mode) {
                case CORTINA_SLICE_SR:
                    debug(9, "Changing from 1000Base-X to SR\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from 1000Base-X to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from 1000Base-X to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(10000);
                    break;
                default:
                    break;
            }
            break;
        }
        case CORTINA_SLICE_UNKNOWN: {
            switch (mode) {
                case CORTINA_SLICE_SR:
                    debug(9, "Changing from Unknown to SR\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from Unknown to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    usleep(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from Unknown to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(20000);
                    debug(9, "Changing from Unknown to 1000Base-X again\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    usleep(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    usleep(20000);
                    break;
                default:
                    break;
            }
        }
    }
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA, ctrla);
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB, ctrlb);
    return status;
}

/* Checks which SFP module is plugged in and configures the PHY
 * to the proper mode.  */
int pim_port_check_updt(uint16_t slot, uint16_t port, enum cortina_slice_mode mode)
{
    uint16_t lane;

    if (mode != media_mode[slot][port]) {
        if (quad_mode[slot]) {
            for (lane = 4*port; lane < 4*(port+1); ++lane)
                cs4343_set_mode_slice(slot, lane, mode);
        } else
            cs4343_set_mode_slice(slot, port, mode);
        debug(9, "SFP:%d:%d mode: %s, old mode: %s\n", slot, port,
                         sfp_mode_to_str[mode], sfp_mode_to_str[media_mode[slot][port]]);
        media_mode[slot][port] = mode;
    }
    return 0;
}

int get_sfp_by_eth(uint32_t eth, uint32_t *module, uint32_t *sfp, uint8_t rxaui_mode)
{
    uint32_t eport;
    int i, j;

    if ((rxaui_mode && eth > MAX_ETH_PORT_NUM) || (!rxaui_mode && eth > (MAX_ETH_PORT_NUM-2)))
        return -1;
    for (i=0; i< MAX_MODULES_NUM; ++i) {
        for (j=0; j< MAX_SFP_NUM; ++j) {
            eport = eport_sfp_map[i][j].eport;
            if (rxaui_mode && eport > 2)
                eport += 2;
            if (eport == eth) {
                *module = eport_sfp_map[i][j].module;
                *sfp = eport_sfp_map[i][j].sfp;
                break;
            }
        }
    }
    return 0;
}

int get_eth_by_sfp(uint32_t *eth, uint32_t module, uint32_t sfp, uint8_t rxaui_mode)
{
    if (module == 0) {
        if ((rxaui_mode && sfp > 4) || (!rxaui_mode && sfp > 2))
            return -1;
    } else if (module > 0  && module < 3) {
        if (sfp > 8)
            return -1;
    } else
        return -1;
    *eth = eport_sfp_map[module][sfp].eport;
    return 0;
}

int get_bgx_by_eth(bgx_lane_map_t *bgx_lane, uint32_t eth, uint8_t rxaui_mode)
{
    uint32_t eport;
    int i, j;

    if ((rxaui_mode && eth > MAX_ETH_PORT_NUM) || (!rxaui_mode && eth > (MAX_ETH_PORT_NUM-2)))
        return -1;
    for (i=0; i< MAX_MODULES_NUM; ++i) {
        for (j=0; j< MAX_SFP_NUM; ++j) {
            eport = eport_sfp_map[i][j].eport;
            if (rxaui_mode && eport > 2)
                eport += 2;
            if (eport == eth) {
                bgx_lane->bgx = eport_sfp_map[i][j].bgx_lane.bgx;
                bgx_lane->indx = eport_sfp_map[i][j].bgx_lane.indx;
                break;
            }
        }
    }
    return 0;
}

int get_eth_by_bgx(bgx_lane_map_t *bgx_lane, uint32_t *eth, uint8_t rxaui_mode)
{
    int i, j;

    if (bgx_lane->bgx == 1 && !rxaui_mode)
        return -1;
    if (bgx_lane->bgx < 2 && !rxaui_mode)
        return -1;
    if ((bgx_lane->indx > 3) || bgx_lane->bgx > 5)
        return -1;
    if ((bgx_lane->indx == 0 && bgx_lane->indx > 1) ||
        (bgx_lane->indx == 1 && bgx_lane->indx < 2))
        return -1;
    for (i=0; i< MAX_MODULES_NUM; ++i) {
        for (j=0; j< MAX_SFP_NUM; ++j) {
            if (bgx_lane->bgx == eport_sfp_map[i][j].bgx_lane.bgx &&
                bgx_lane->indx == eport_sfp_map[i][j].bgx_lane.indx) {
                *eth = eport_sfp_map[i][j].eport;
                break;
            }
        }
    }
    return 0;
}

int get_sfp_by_bgx(bgx_lane_map_t *bgx_lane, uint32_t *module, uint32_t *sfp, uint8_t rxaui_mode)
{
    int i, j;

    if (bgx_lane->bgx == 1 && !rxaui_mode)
        return -1;
    if (bgx_lane->bgx < 2 && !rxaui_mode)
        return -1;
    if ((bgx_lane->indx > 3) || bgx_lane->bgx > 5)
        return -1;
    if ((bgx_lane->indx == 0 && bgx_lane->indx > 1) ||
        (bgx_lane->indx == 1 && bgx_lane->indx < 2))
        return -1;
    for (i=0; i< MAX_MODULES_NUM; ++i) {
        for (j=0; j< MAX_SFP_NUM; ++j) {
            if (bgx_lane->bgx == eport_sfp_map[i][j].bgx_lane.bgx &&
                bgx_lane->indx == eport_sfp_map[i][j].bgx_lane.indx) {
                *module = i; *sfp = j;
                break;
            }
        }
    }
    return 0;
}

int get_bgx_by_sfp(bgx_lane_map_t *bgx_lane, uint32_t module, uint32_t sfp, uint8_t rxaui_mode)
{
    if (module == 0) {
        if ((rxaui_mode && sfp > 4) || (!rxaui_mode && sfp > 2))
            return -1;
    } else if (module > 0  && module < 3) {
        if (sfp > 8)
            return -1;
    } else
        return -1;
    bgx_lane->bgx = eport_sfp_map[module][sfp].bgx_lane.bgx;
    bgx_lane->indx = eport_sfp_map[module][sfp].bgx_lane.indx;
    return 0;
}

#ifdef LMAC_PDU_STATS
static bgx_lane_map_t bgx_lane_map[MAX_SLOTS_NUM][MAX_SLICES_NUM] = {
   {{ 5,  0}, /* 0, 0 */ { 5,  1}, /* 0, 1 */ { 5,  2}, /* 0, 2 */ { 5,  3}, /* 0, 3 */
    { 4,  0}, /* 0, 4 */ { 4,  1}, /* 0, 5 */ { 4,  2}, /* 0, 6 */ { 4,  3}, /* 0, 7 */},
   {{ 3,  0}, /* 1, 0 */ { 3,  1}, /* 1, 1 */ { 3,  2}, /* 1, 2 */ { 3,  3}, /* 1, 3 */
    { 2,  0}, /* 1, 4 */ { 2,  1}, /* 1, 5 */ { 2,  2}, /* 1, 6 */ { 2,  3}, /* 1, 7 */},
   {{ 0,  0}, /* 2, 0 */ { 0,  1}, /* 2, 1 */ { 1,  2}, /* 2, 2 */ { 1,  3}, /* 2, 3 */
    { -1,  -1}, /* 2, 4 */ { -1,  -1}, /* 2, 5 */ { -1,  -1}, /* 2, 6 */ { -1,  -1}, /* 2, 7 */},
};

static int get_lmac_interface_activity(int slot, int slice)
{
    static lmac_pdu_stats_t lmac_stats[MAX_SLOTS_NUM][MAX_SLICES_NUM];
    uint32_t bgx, ind, node = 0, activity = 0;
    lmac_pdu_stats_t lmac;

    if (slot < 0 || slot >= MAX_SLOTS_NUM) slot = MAX_SLOTS_NUM - 1;
    if (slice < 0 || slice >= MAX_SLICES_NUM) slice = MAX_SLICES_NUM - 1;
    bgx = bgx_lane_map[slot][slice].bgx;
    ind = bgx_lane_map[slot][slice].indx;
    if (bgx > 5 || ind > 3)
        return 0;

    lmac.rx_stat0.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_RX_STAT0(ind, bgx));
    if (lmac_stats[slot][slice].rx_stat0.u64 != lmac.rx_stat0.u64) {
        lmac_stats[slot][slice].rx_stat0.u64 = lmac.rx_stat0.u64;
        activity = 1;
    }
    lmac.tx_stat6.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT6(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat6.u64 != lmac.tx_stat6.u64) {
        lmac_stats[slot][slice].tx_stat6.u64 = lmac.tx_stat6.u64;
        activity = 1;
    }
    lmac.tx_stat7.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT7(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat7.u64 != lmac.tx_stat7.u64) {
        lmac_stats[slot][slice].tx_stat7.u64 = lmac.tx_stat7.u64;
        activity = 1;
    }
    lmac.tx_stat8.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT8(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat8.u64 != lmac.tx_stat8.u64) {
        lmac_stats[slot][slice].tx_stat8.u64 = lmac.tx_stat8.u64;
        activity = 1;
    }
    lmac.tx_stat9.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT9(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat9.u64 != lmac.tx_stat9.u64) {
        lmac_stats[slot][slice].tx_stat9.u64 = lmac.tx_stat9.u64;
        activity = 1;
    }
    lmac.tx_stat10.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT10(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat10.u64 != lmac.tx_stat10.u64) {
        lmac_stats[slot][slice].tx_stat10.u64 = lmac.tx_stat10.u64;
        activity = 1;
    }
    lmac.tx_stat11.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT11(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat11.u64 != lmac.tx_stat11.u64) {
        lmac_stats[slot][slice].tx_stat11.u64 = lmac.tx_stat11.u64;
        activity = 1;
    }
    lmac.tx_stat12.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT12(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat12.u64 != lmac.tx_stat12.u64) {
        lmac_stats[slot][slice].tx_stat12.u64 = lmac.tx_stat12.u64;
        activity = 1;
    }
    lmac.tx_stat13.u64 = cvmx_read_csr_node(node, CVMX_BGXX_CMRX_TX_STAT13(ind, bgx));
    if (lmac_stats[slot][slice].tx_stat13.u64 != lmac.tx_stat13.u64) {
        lmac_stats[slot][slice].tx_stat13.u64 = lmac.tx_stat13.u64;
        activity = 1;
    }
    return activity;
}
#endif

int get_pim_interface_activity(int slot, int slice)
{
#ifdef LMAC_PDU_STATS
    return get_lmac_interface_activity(slot, slice);
#else
    static uint16_t rx_blk_total[2][8][2], tx_blk_total[2][8][2];
    int status = 0, activity = 0;
    int bus = slot ? 3 : 2;
    uint16_t data;

    /* The CS4343 FEC facility is enabled in U-Boot. Before fetching the counters, *
     * trigger the capture of the FEC statistics. */
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL, 0x0001);
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL, 0x0001);
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_KR_FEC_RX_STATS_CONTROL, 0x0000);
    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_KR_FEC_TX_STATS_CONTROL, 0x0000);

    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL0, &data);
    if (tx_blk_total[slot][slice][0] != data) {
        tx_blk_total[slot][slice][0] = data; activity = 1;
    }
    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_KR_FEC_TX_STATS_TX_BLK_TOTAL1, &data);
    if (tx_blk_total[slot][slice][1] != data) {
        tx_blk_total[slot][slice][1] = data; activity = 1;
    }
    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL0, &data);
    if (rx_blk_total[slot][slice][0] != data) {
        rx_blk_total[slot][slice][0] = data; activity = 1;
    }
    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_KR_FEC_RX_STATS_RX_BLK_TOTAL1, &data);
    if (rx_blk_total[slot][slice][1] != data) {
        rx_blk_total[slot][slice][1] = data; activity = 1;
    }
    return activity;
#endif
}

int is_cs4318_phy(void)
{
    uint16_t data;

    cs4318_reg_get(0, 0, &data);
    if (data != 0x23e5)
        return 0;

    cs4318_reg_get(0, 1, &data);
    if (data != 0x2002)
        return 0;
    return 1;
}

int get_fp_interface_activity(int slice)
{
#ifdef LMAC_PDU_STATS
    return get_lmac_interface_activity(-1, slice);
#else
    /* MAC Transmit Frames (RMON) (at offset 4f) */
    static uint16_t line_mac_tx_frames[MAX_FRONTPANEL_PORTS];
    /* MAC Receive Frames (RMON) (at offset 1d) */
    static uint16_t line_mac_rx_frames[MAX_FRONTPANEL_PORTS];
    static int attempts = 10;

    CS4318_PM_STATS_ACCESS_t pm_stats_access;
    uint16_t data, i;

    if (slice >= fp_ports_num)
        return -1;

    for (i = 0; i < 2; i++) {
        pm_stats_access.bf.wr_rd = 0;
        pm_stats_access.bf.cntr_id = i ? 0x1d : 0x4f;
        pm_stats_access.bf.access = 1;
        cs4318_reg_set(slice, CS4318_PM_STATS_ACCESS, pm_stats_access.wrd);

        cs4318_reg_get(slice, CS4318_PM_STATS_ACCESS, &(pm_stats_access.wrd));
        while (pm_stats_access.bf.access == 1) {
            if (attempts < 0)
                return -1;
            usleep(100);
            attempts -= 1;
            cs4318_reg_get(slice, CS4318_PM_STATS_ACCESS, &(pm_stats_access.wrd));
        }
        cs4318_reg_get(slice, CS4318_PM_STATS_DATA0, &data);
        if (i && line_mac_tx_frames[slice] != data) {
            line_mac_tx_frames[slice] = data;
            return 1;
        }
        else if (!i && line_mac_rx_frames[slice] != data) {
            line_mac_rx_frames[slice] = data;
            return 1;
        }
    }
    return 0;
#endif
}

void set_fp_interface_mode(void)
{
   /* All related CS4318 led configs according to CPLD spec, *
    * Table 3 were already done in U-Boot bootloader */
}

int get_mgmt_interface_status(void)
{
    uint16_t data;

    vs8221_reg_set(0x1f, 0x0);
    if ((vs8221_reg_get(0x1, &data) < 0) &&
        (vs8221_reg_get(0x1, &data) < 0))
       return IFACE_OFF;
    return (data & 0x4) ? IFACE_ON : IFACE_OFF;
}

uint8_t get_mgmt_interface_speed(void)
{
    uint16_t data;

    vs8221_reg_set(0x1f, 0x0);
    if (vs8221_reg_get(0x1c, &data) < 0)
    return SPEED_UNDEF;
    data >>= 3; data &= 3;
    if (data == 0)
        return SPEED_10M;
    else if (data == 1)
        return SPEED_100M;
    else if (data == 2)
        return SPEED_1G;
    else
        return SPEED_UNDEF;
}

void set_mgmt_interface_mode(void)
{
    /* Set VS8221 led mode according to CPLD spec, Table 3 */
    vs8221_reg_set(VSC8221_LED_CTL_REG,
        VSC8221_LED_CFGS_MODE | VSC8221_LED_PIN2_CFG | VSC8221_LED_PIN1_CFG | VSC8221_LED_PIN0_CFG);
}

static int vs8221_reg_get(int addr, uint16_t* data)
{
    *data = octeon_mdiobus_read(0, 0, addr);
    debug(9, "rd [%04x] == 0x%04x\n", addr, *data);
    return 0;
}

static int vs8221_reg_set(int addr, uint16_t data)
{
    int rc = octeon_mdiobus_write(0, 0, addr, data);
    debug(9, "wr [%04x] <= 0x%04x\n", addr, data);
    return rc;
}

static int cs4318_reg_get(int slice, int addr, uint16_t* data)
{
    addr |= MII_ADDR_C45;
    *data = octeon_mdiobus_read(1, slice, addr);
    debug(9, "rd [%08x:%04x] == 0x%04x\n", slice, addr, *data);
    return 0;
}

#ifndef LMAC_PDU_STATS
static int cs4318_reg_set(int slice, int addr, uint16_t data)
{
    int rc;
    addr |= MII_ADDR_C45;
    rc = octeon_mdiobus_write(1, slice, addr, data);
    debug(9, "wr [%08x:%04x] <= 0x%04x\n", slice, addr, data);
    return rc;
}
#endif

static int cs4343_diags_register_can_read(int addr)
{
    if ((addr >= 0x1229 && addr <= 0x1233) || (addr >= 0x1A29 && addr <= 0x1A33))
        return 0;
    return 1;
}

static int cs4343_adj_pp(int slice, int addr)
{
    static const uint16_t pp_cs4343_sku_offset[8] = {
        0x0000, 0x1000, 0x2000, 0x3000, 0x3000, 0x2000, 0x1000, 0x0000 };

    if ((slice & 0xff) > 7)
        return -1;
    return (addr + pp_cs4343_sku_offset[slice & 0x7]);
}

static int cs4343_adj_mseq(int slice, int addr)
{
    static const uint16_t mseq_cs4343_sku_offset[8] = {
        0x0000, 0x0000, 0x100, 0x100, 0x100, 0x100, 0x0000, 0x0000 };

    if ((slice & 0xff) > 7)
        return -1;
    return (addr + mseq_cs4343_sku_offset[slice & 0x7]);
}

static int cs4343_reg_get(int bus, int slice, int addr, uint16_t* data)
{
    int phy = (slice >> 8) & 0x1f;

    if((addr >= 0x1000) && (addr < 0x5000)) {
        addr = (addr | 0x3000) & 0x1fff;
        if (!cs4343_diags_register_can_read(addr)) {
            *data = 0xbeef;
            return 0;
        }
        addr = cs4343_adj_pp(slice, addr);
    }
    else if((addr >= 0x300) && (addr < 0x500)) {
        addr = (addr | 0x300) & 0x3ff;
        addr = cs4343_adj_mseq(slice, addr);
    }

    if((slice & 0xff) < 4)
        phy += 0x1;

    addr |= MII_ADDR_C45;
    *data = octeon_mdiobus_read(bus, phy, addr);
    debug(9, "rd [%02x:%02x:%04x] == 0x%04x\n", bus, phy, addr, *data);
    return 0;
}

static int cs4343_reg_set(int bus, int slice, int addr, uint16_t data)
{
    int phy = (slice >> 8) & 0x1f;
    int rc;

    if ((addr >= 0x1000) && (addr < 0x5000)) {
        addr = (addr | 0x3000) & 0x1fff;
        addr = cs4343_adj_pp(slice, addr);
    }
    else if((addr >= 0x300) && (addr < 0x500)) {
        addr = (addr | 0x300) & 0x3ff;
        addr = cs4343_adj_mseq(slice, addr);
    }

    if((slice & 0xff) < 4)
        phy += 0x1;

    addr |= MII_ADDR_C45;
    rc = octeon_mdiobus_write(bus, phy, addr, data);
    debug(9, "wr [%02x:%02x:%04x] <= 0x%04x\n", bus, phy, addr, data);
    return rc;
}

/* used Octeon SDK specific defines */
CVMX_SHARED uint32_t cvmx_app_init_processor_id;

/*
 * bit map for octeon features
 */
#define FEATURE_MAP_SIZE    128
CVMX_SHARED uint8_t octeon_feature_map[FEATURE_MAP_SIZE];

static uint64_t rate_eclk = 0;
static uint64_t rate_sclk = 0;
static uint64_t rate_dclk = 0;

/**
 * Get clock rate based on the clock type.
 *
 * @param node  - CPU node number
 * @param clock - Enumeration of the clock type.
 * @return      - return the clock rate.
 */
uint64_t cvmx_clock_get_rate_node(int node, cvmx_clock_t clock)
{
    const uint64_t REF_CLOCK = 50000000;

    if (cvmx_unlikely(!rate_eclk)) {
        cvmx_rst_boot_t rst_boot;
        rst_boot.u64 = cvmx_read_csr_node(node, CVMX_RST_BOOT);
        rate_eclk = REF_CLOCK * rst_boot.s.c_mul;
        rate_sclk = REF_CLOCK * rst_boot.s.pnr_mul;
    }

    switch (clock) {
    case CVMX_CLOCK_SCLK:
    case CVMX_CLOCK_IPD:
        return rate_sclk;
    case CVMX_CLOCK_TIM:
        return rate_sclk;

    case CVMX_CLOCK_RCLK:
    case CVMX_CLOCK_CORE:
        return rate_eclk;

    case CVMX_CLOCK_DDR:
        return rate_dclk;
    }
    return 0;
}

uint64_t cvmx_clock_get_rate(cvmx_clock_t clock)
{
    return cvmx_clock_get_rate_node(0, clock);
}
