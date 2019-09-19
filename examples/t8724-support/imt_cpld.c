#include "cvmx-platform.h"
#include "cvmx.h"
#include "cvmx-access-native.h"
#include "cvmx-app-init.h"
#include "cvmx-bootmem.h"
#include "cvmx-sysinfo.h"
#include "cvmx-mio-defs.h"
#include "cvmx-csr.h"
#include "cvmx-app-init.h"
#include "cvmx-helper-util.h"

#include "imt_cpld.h"

#define LLU		long long unsigned int

#ifdef DEBUG
#define debug   printf
#else
#define debug(...)
#endif

int cpld_is_init = 0;

static uint64_t cpld_base;

static uint64_t get_real_78xx_xaui_link_status(int xiface, int index);

int cpld_init()
{
	cvmx_mio_boot_reg_cfgx_t reg_cfg;
	switch( cvmx_sysinfo_get()->board_type )
	{
                case CVMX_BOARD_TYPE_CUST_IM8724:
			debug("Board: T8724\n");
			reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(1)); break;
		default:
			printf("Unknown board!\n");
			return 0;
	}

	cpld_base = CVMX_ADD_SEG(CVMX_MIPS_SPACE_XKPHYS, reg_cfg.s.base << 16);

	if(cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_CUST_IM8724)
    	cvmx_helper_get_real_78xx_xaui_link_status = get_real_78xx_xaui_link_status;

	debug("cpld_base: [%016llx]\n", (LLU) cpld_base);
	cpld_is_init = 1;
	return 1;
}

uint8_t cpld_rd(uint8_t offset)
{
	uint8_t val;

	CVMX_SYNC;
	val = cvmx_read64_uint8(cpld_base + offset);
	debug("[%016llx] == %02x\n", (LLU) (cpld_base + offset), val);

	return val;
}

void cpld_wr(uint8_t offset, uint8_t val)
{
	debug("[%016llx] <= %02x\n", (LLU) (cpld_base + offset), val);
	cvmx_write64_uint8(cpld_base + offset, val);
	CVMX_SYNC;
}

uint8_t cpld_rd_remote(uint8_t target, uint8_t offset)
{
	uint8_t val;
	cpld_wr(CPLD_ADDR_BA_CTL, 0x80 | (target & 0x1f) );
	cpld_wr(CPLD_ADDR_SPI_CFG, 0x80);
	cpld_wr(CPLD_ADDR_SPI_ADDR_0, 0x80 | (offset & 0x7f));
	cpld_wr(CPLD_ADDR_SPI_CMD, CPLD_SPI_CMD_OP_R8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST);

	while(cpld_rd(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST);
	val = cpld_rd(CPLD_ADDR_SPI_DATA_0);
	return val;
}

void cpld_wr_remote(uint8_t target, uint8_t offset, uint8_t val)
{
	cpld_wr(CPLD_ADDR_BA_CTL, 0x80 | (target & 0x1f) );
	cpld_wr(CPLD_ADDR_SPI_CFG, 0x80);
	cpld_wr(CPLD_ADDR_SPI_ADDR_0, offset & 0x7f);
	cpld_wr(CPLD_ADDR_SPI_DATA_0, val);
	cpld_wr(CPLD_ADDR_SPI_CMD, CPLD_SPI_CMD_OP_W8 | CPLD_SPI_CMD_ADDR_W_1 | CPLD_SPI_CMD_ST);

	while(cpld_rd(CPLD_ADDR_SPI_CMD) & CPLD_SPI_CMD_ST);
}

int cpld_i2c_read(uint8_t sasl, uint8_t bus, uint8_t dev, uint8_t reg)
{
        uint8_t rc;

        cpld_wr_remote(sasl, CPLD_ADDR_I2C_BSL, bus);
        cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_EB_EN | CPLD_BA_CTL_B_MD_I2C | sasl);

        cpld_wr(CPLD_ADDR_I2C_DA, (dev & 0x7f) << 1);
        cpld_wr(CPLD_ADDR_I2C_CTRL, CPLD_I2C_CTL_R_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
        cpld_wr(CPLD_ADDR_I2C_ADDR_0, reg);
        cpld_wr(CPLD_ADDR_I2C_CSR, CPLD_I2C_CSR_ST | CPLD_I2C_CSR_READ);

        while (cpld_rd(CPLD_ADDR_I2C_CSR) & CPLD_I2C_CSR_ST)
                continue;

        rc = cpld_rd(CPLD_ADDR_I2C_ST) & CPLD_I2C_STAT_MASK;
        if (rc != 0) {
                debug("%s: i2c read error. stat: %02x\n", __func__, rc);
                return -1;
        }
        return cpld_rd(CPLD_ADDR_I2C_DATA);
}

int cpld_i2c_write(uint8_t sasl, uint8_t bus, uint8_t dev,
                          uint8_t reg, uint8_t val)
{
        uint8_t rc;

        cpld_wr_remote(sasl, CPLD_ADDR_I2C_BSL, bus);
        cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_EB_EN | CPLD_BA_CTL_B_MD_I2C | sasl);

        cpld_wr(CPLD_ADDR_I2C_DA, (dev & 0x7f) << 1);
        cpld_wr(CPLD_ADDR_I2C_CTRL, CPLD_I2C_CTL_R_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
        cpld_wr(CPLD_ADDR_I2C_ADDR_0, reg);
        cpld_wr(CPLD_ADDR_I2C_DATA, val);
        cpld_wr(CPLD_ADDR_I2C_CSR, CPLD_I2C_CSR_ST | CPLD_I2C_CSR_WRITE);

        while (cpld_rd(CPLD_ADDR_I2C_CSR) & CPLD_I2C_CSR_ST)
                continue;

        rc = cpld_rd(CPLD_ADDR_I2C_ST) & CPLD_I2C_STAT_MASK;
        if (rc != 0) {
                debug("%s: i2c write error. stat: %02x\n", __func__, rc);
                return -1;
        }
        return 0;
}

void reset_unit(void)
{
   int regval = cpld_rd(CPLD_ADDR_RST_CTL_0);
   cpld_wr(CPLD_ADDR_RST_CTL_0, regval | CPLD_BIT_CN_CPU);
}

uint8_t get_sfp_rate(int slot, uint8_t port)
{
	int32_t reg_val[2];
	uint8_t sasl;
	int ret;

	if (slot >1 || (slot < 0 && port >3) || port > 7)
		return 0;
	sasl = slot < 0 ? 0x0 : slot == 0 ? 0x80 : 0x81;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x50, 3)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x50, 6)) < 0))
		return 0;
	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf))
		return 10;
	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf))
		return 1;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x51, 110)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x51, 118)) < 0))
		return 0;
	/* if mismatch, fixing it */
	if ((reg_val[0] & 0x08) != (reg_val[1] & 0x08)) {
		if (!(reg_val[0] & 0x08))
			reg_val[1] |= 0x08;
		else
			reg_val[1] &= ~0x08;
		cpld_i2c_write(sasl, port, 0x51, 118, (uint8_t)reg_val[1]);
	}

	ret = ((reg_val[0] & 0x08)) ? 10 : 1;

	return ret;
}

void set_sfp_rate(int slot, uint8_t port, uint8_t rate, uint8_t force)
{
	int32_t reg_val[2];
	uint8_t sasl;

	if (slot >1 || (slot < 0 && port >3) || port > 7)
		return;
	if (rate != 1 && rate != 10)
		return;
	sasl = slot < 0 ? 0x0 : slot == 0 ? 0x80 : 0x81;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x50, 3)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x50, 6)) < 0))
		return;
	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf))
		return;
	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf))
		return;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x51, 110)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x51, 118)) < 0))
		return;
	if (!force &&
	    ((rate == 1 && !(reg_val[0] & 0x08) && !(reg_val[1] & 0x08)) ||
	     (rate == 10 && (reg_val[0] & 0x08) && (reg_val[1] & 0x08))))
		return;

	if (rate == 1) {
		reg_val[0] &= ~0x08; reg_val[1] &= ~0x08;
	} else {
		reg_val[0] |= 0x08; reg_val[1] |= 0x08;
	}
	cpld_i2c_write(sasl, port, 0x51, 110, (uint8_t)reg_val[0]);
	cpld_i2c_write(sasl, port, 0x51, 118, (uint8_t)reg_val[1]);
}

/*
 * return 0-ok, 1-rx_los
 */
static uint64_t get_real_78xx_xaui_link_status(int xiface, int index)
{
	uint64_t status = 1;
	struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);

	cvmx_bgxx_cmrx_config_t cmr_config;
	cmr_config.u64 = cvmx_read_csr_node(xi.node, CVMX_BGXX_CMRX_CONFIG(index, xiface));

	// TODO: SGMII link checking on internal im8724 ports
	if(cmr_config.s.lmac_type == 2) // RXAUI
	{
		cvmx_mio_boot_reg_cfgx_t reg_cfg;
		uint64_t address;

		reg_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(1));
		address = CVMX_ADD_SEG(CVMX_MIPS_SPACE_XKPHYS, reg_cfg.s.base << 16);

		if((status = cvmx_read64_uint8(address + CPLD_ADDR_SFP_PRES) & (1 << index))) {
			status = !(cvmx_read64_uint8(address + CPLD_ADDR_SFP_RX_LOS) & (1 << index));
		}
	}

	return !status;
}
