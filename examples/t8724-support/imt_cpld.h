#ifndef _IMT_CPLD_H_
#define _IMT_CPLD_H_

/* CPLD Common Addresses */

#define	CPLD_ADDR_CPLD_SIG_L		0x00
#define	CPLD_ADDR_CPLD_SIG_H		0x01
#define CPLD_ADDR_CPLD_REV		0x02
/*
 * Remote access interface
 */
#define CPLD_ADDR_BA_CTL		0x1c

#define CPLD_BA_CTL_B_MD_REG		0x00
#define CPLD_BA_CTL_B_MD_SPI		0x20
#define CPLD_BA_CTL_B_MD_I2C		0x40
#define CPLD_BA_CTL_B_MD_MI		0x60

#define CPLD_BA_CTL_EB_EN		0x80

/*
 * SPI interface
 */

#define CPLD_ADDR_MI_BSL		0x1e
#define CPLD_ADDR_I2C_BSL		0x1f

#define CPLD_ADDR_SPI_CMD		0x28
#define CPLD_ADDR_SPI_CFG		0x29
#define CPLD_ADDR_SPI_DATA_0		0x2a
#define CPLD_ADDR_SPI_DATA_1		0x2b
#define CPLD_ADDR_SPI_ADDR_0		0x2c
#define CPLD_ADDR_SPI_ADDR_1		0x2d
#define CPLD_ADDR_SPI_ADDR_2		0x2e
#define CPLD_ADDR_SPI_ADDR_3		0x2f

#define CPLD_SPI_CMD_ST			0x01
#define CPLD_SPI_CMD_ADDR_W_1		0x00
#define CPLD_SPI_CMD_ADDR_W_2		0x02
#define CPLD_SPI_CMD_ADDR_W_3		0x04
#define CPLD_SPI_CMD_ADDR_W_4		0x06
#define CPLD_SPI_CMD_OP_W8		0x00
#define CPLD_SPI_CMD_OP_W16		0x10
#define CPLD_SPI_CMD_OP_WS		0x20
#define CPLD_SPI_CMD_OP_WA		0x30
#define CPLD_SPI_CMD_OP_R8		0x40
#define CPLD_SPI_CMD_OP_R16		0x50
#define CPLD_SPI_CMD_OP_RS		0x60
#define CPLD_SPI_CMD_OP_RAM		0xf0

/*
 * T8724 I2C interface.
 */
#define CPLD_ADDR_I2C_BLOCK             0x38
#define CPLD_ADDR_I2C_CSR               (CPLD_ADDR_I2C_BLOCK + 0)
#define CPLD_ADDR_I2C_DA                (CPLD_ADDR_I2C_BLOCK + 1)
#define CPLD_ADDR_I2C_DATA              (CPLD_ADDR_I2C_BLOCK + 2)
#define CPLD_ADDR_I2C_ST                (CPLD_ADDR_I2C_BLOCK + 3)
#define CPLD_ADDR_I2C_ADDR_0            (CPLD_ADDR_I2C_BLOCK + 4)
#define CPLD_ADDR_I2C_ADDR_1            (CPLD_ADDR_I2C_BLOCK + 5)
#define CPLD_ADDR_I2C_CTRL              (CPLD_ADDR_I2C_BLOCK + 6)

#define CPLD_I2C_CTL_R_1000             0x00
#define CPLD_I2C_CTL_R_500              0x10
#define CPLD_I2C_CTL_R_100              0x20
#define CPLD_I2C_CTL_R_50               0x30
#define CPLD_I2C_CTL_SDA_SCL_HIGH	0x0C

#define CPLD_I2C_STAT_ACT               0x01

#define CPLD_I2C_CSR_WRITE              0x10
#define CPLD_I2C_CSR_READ               0x00
#define CPLD_I2C_CSR_ACK_POLL           0x20
#define CPLD_I2C_CSR_ST                 0x01
#define CPLD_I2C_STAT_MASK              0x07

/*
 * T8724 CPLD required addresses.
 */
#define CPLD_ADDR_RST_CTL_0             0x04
#define CPLD_ADDR_RST_CTL_1             0x05

#define CPLD_BIT_RE_BOOT                0x40
#define CPLD_BIT_ZL_CLK                 0x10
#define CPLD_BIT_CS4318                 0x08
#define CPLD_BIT_MGT_PHY                0x04
#define CPLD_BIT_PEX_PCIE               0x02
#define CPLD_BIT_CN_CPU                 0x01

#define CPLD_ADDR_PS_CSR		0x07
#define CPLD_PS1_PRSNT          	(1 << 7)
#define CPLD_PS1_OK             	(1 << 6)
#define CPLD_PS1_INT           	(1 << 5)
#define CPLD_PS1_OFF            	(1 << 4)
#define CPLD_PS0_PRSNT          	(1 << 3)
#define CPLD_PS0_OK             	(1 << 2)
#define CPLD_PS0_INT            	(1 << 1)
#define CPLD_PS0_OFF            	(1 << 0)
#define CPLD_ADDR_SFP_PRES		0x0B
#define CPLD_ADDR_SFP_RX_LOS		0x0D
#define CPLD_ADDR_INT_STAT_0		0x10
#define CPLD_ADDR_INT_EN_0		0x11
#define CPLD_ADDR_INT_STAT_1		0x12
#define CPLD_ADDR_INT_EN_1		0x13
#define CPLD_STAT_0_TEMP_EV		(1 << 1)
#define CPLD_STAT_0_OVERT_EV		(1 << 7)
#define CPLD_STAT_1_FAN_EV		(1 << 6)

#define VSC8221_MODE_STAT_REG		0x01
#define VSC8221_MODE_LINK_STAT		(1 << 2)
#define VSC8221_AUX_STAT_REG		0x1c
#define VSC8221_AUX_SPEED_MASK		3
#define VSC8221_AUX_STAT_SPEED1		(1 << (VSC8221_AUX_SPEED_MASK+1))
#define VSC8221_AUX_STAT_SPEED2		(1 << (VSC8221_AUX_SPEED_MASK))

extern int cpld_is_init;
extern CVMX_SHARED uint64_t(*cvmx_helper_get_real_78xx_xaui_link_status)(int xiface, int index);

int cpld_init(void);
uint8_t cpld_rd(uint8_t offset);
void cpld_wr(uint8_t offset, uint8_t val);

uint8_t cpld_rd_remote(uint8_t target, uint8_t offset);
void cpld_wr_remote(uint8_t target, uint8_t offset, uint8_t val);

int cpld_i2c_read(uint8_t sasl, uint8_t bus, uint8_t dev, uint8_t reg);
int cpld_i2c_write(uint8_t sasl, uint8_t bus, uint8_t dev,
					 uint8_t reg, uint8_t val);

void reset_unit(void);
int octeon_i2c_read(unsigned int bus, uint8_t chip, uint addr,
			int alen, uint8_t *buffer, int len);
int octeon_i2c_write(unsigned int bus, uint8_t chip, uint addr,
			int alen, uint8_t *buffer, int len);

void set_sfp_rate(int slot, uint8_t port, uint8_t rate, uint8_t force);
uint8_t get_sfp_rate(int slot, uint8_t port);

#endif /* _IMT_CPLD_H_ */
