#include <cvmx.h>
#include "cvmx-mdio.h"
#include <cvmx-platform.h>

#include "cortina_phys.h"
#include "imt_cpld.h"

#ifdef DEBUG
#define debug   printf
#else
#define debug(...)
#endif

int quad_mode[MAX_MODULES_NUM];

int cs4318_reg_get(int slice, int addr, uint16_t* data)
{
    *data = cvmx_mdio_45_read(1, slice, 0, addr);
    debug(9, "rd [%08x:%04x] == 0x%04x\n", slice, addr, *data);
    return 0;
}

int cs4318_reg_set(int slice, int addr, uint16_t data)
{
    int rc = cvmx_mdio_45_write(1, slice, 0, addr, data);
    debug(9, "wr [%08x:%04x] <= 0x%04x\n", slice, addr, data);
    return rc;
}

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

int cs4343_reg_get(int bus, int slice, int addr, uint16_t* data)
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

    *data = cvmx_mdio_45_read(bus, phy, 0, addr);
    debug(9, "rd [%02x:%02x:%04x] == 0x%04x\n", bus, phy, addr, *data);
    return 0;
}

int cs4343_reg_set(int bus, int slice, int addr, uint16_t data)
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

    rc = cvmx_mdio_45_write(bus, phy, 0, addr, data);
    debug(9, "wr [%02x:%02x:%04x] <= 0x%04x\n", bus, phy, addr, data);
    return rc;
}

#ifdef DEBUG
static const char *sfp_mode_to_str[] = { "SR", "CX", "1000Base-X", "Unknown" };
#endif
static enum cortina_slice_mode media_mode[MAX_MODULES_NUM][MAX_SLICES_NUM];

void pim_set_defines(void)
{
    cvmx_gserx_cfg_t cfg;
    int slot, slice;

    for (slot = 0; slot < MAX_MODULES_NUM; slot++) {
        cfg.u64 = cvmx_read_csr_node(0, CVMX_GSERX_CFG(slot ? 2 : 4));
        quad_mode[slot] = !!cfg.s.bgx_quad;
        for (slice = 0; slice < MAX_SLICES_NUM; ++slice)
            media_mode[slot][slice] = CORTINA_SLICE_UNKNOWN;
    }
}

static enum cortina_slice_mode get_media_type(uint16_t slot, uint16_t port)
{
    cvmx_bgxx_cmr_global_config_t gconfig;
    int32_t regval, bgx;
    uint8_t sasl;

    if (slot > 1 || (quad_mode[slot] && port > 1) ||
                    (!quad_mode[slot] && port > 7))
        return CORTINA_SLICE_UNKNOWN;

    if (quad_mode[slot] == 0) {
        bgx = slot == 0 ? 4 : 2;
        if (port < 4)
             ++bgx;
        gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(bgx));
        if (gconfig.s.pmux_sds_sel)
            return CORTINA_SLICE_1000X;
    }

    sasl = slot ? 0x81 : 0x80;
    if ((regval = cpld_i2c_read(sasl, port, 0x50, 8)) < 0)
        return CORTINA_SLICE_UNKNOWN;
    if ((regval & 0xc) == 0x00)
        return CORTINA_SLICE_SR;
    return CORTINA_SLICE_CX;
}

static int cs4343_get_ctrl(uint16_t slot, uint16_t slice,
      enum cortina_slice_mode mode, uint16_t *ctrla, uint16_t *ctrlb)
{
    static const uint16_t tx_out_ctrla_values[CORTINA_SLICE_UNKNOWN][MAX_SLICES_NUM] = {
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
    static const uint16_t tx_out_ctrlb_values[CORTINA_SLICE_UNKNOWN][MAX_SLICES_NUM] = {
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
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    break;
                 case CORTINA_SLICE_CX:
                    debug(9, "Changing mode from SR to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from SR to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
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
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from CX to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from CX to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
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
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from 1000Base-X to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from 1000Base-X to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(10000);
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
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0009);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_CX:
                    debug(9, "Changing from Unknown to CX\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0005);
                    cvmx_wait_usec(10000);
                    break;
                case CORTINA_SLICE_1000X:
                    debug(9, "Changing from Unknown to 1000Base-X\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(20000);
                    debug(9, "Changing from Unknown to 1000Base-X again\n");
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_get(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, &val);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, val);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB, 0x8000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB, 0x0009);
                    cvmx_wait_usec(20000);
                    status |= cs4343_reg_set(bus, slice, CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB, 0x0000);
                    cvmx_wait_usec(20000);
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
int pim_port_check_updt(uint16_t slot, uint16_t port)
{
    enum cortina_slice_mode mode = get_media_type(slot, port);
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
