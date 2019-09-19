/*
 * (C) Copyright 2012-2015 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <phy.h>
#include <net.h>
#include <fdt_support.h>
#include <libfdt.h>
#include <fdtdec.h>
#include <asm/arch/octeon_fdt.h>
#include <i2c.h>
#include <asm/gpio.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-helper-fdt.h>
#include <asm/arch/cvmx-pcsx-defs.h>
#include <compiler.h>
#ifdef CONFIG_PCA953X
# include <pca953x.h>
#endif
#ifdef CONFIG_PCF857X
# include <pcf857x.h>
#endif
#ifdef CONFIG_PHY_SFP
# include <sfp.h>
#endif

/** Maximum number of COMA GPIOs */
#define MAX_COMA_GPIOS		32

#define COMA_FLAG_ACTIVE_LOW	0x80
#define COMA_GPIO_MASK		0x7f

DECLARE_GLOBAL_DATA_PTR;

/** Media type for vsc8574 */
enum vsc8574_media_mode {
	MM_COPPER,
	MM_1000BX,
	MM_100FX
};

/** Describes each COMA pin */
struct gpio_pin {
	enum cvmx_gpio_type type;	/** Type of GPIO bus */
	uint8_t i2c_addr;	/** If an I2C GPIO device the I2C address */
	uint8_t pin:7;		/** GPIO pin on device */
	uint8_t invert:1;	/** If COMA should be inverted or not */
	uint8_t i2c_bus;	/** I2C bus number if I2C GPIO expander */
};

static int num_coma_gpios = 0;	/** Number of COMA GPIOs to activate */

/** Describes each COMA GPIO pin */
static struct gpio_pin coma_gpios[MAX_COMA_GPIOS];

extern int octeon_i2c_reg_addr_to_bus(uint64_t addr);
extern uint32_t i2c_get_stop_delay(void);
extern int i2c_set_stop_delay(uint32_t delay);

static const char *compat848x_list[] = {
	"vitesse,vsc8488",
	"vitesse,vsc8488-15",
	"vitesse,vsc8487",
	"\0"
};
static const char *compat855x_list[] = {
	"vitesse,vsc8572",
	"vitesse,vsc8574",
	"vitesse,vsc857x",
	"\0"
};
static const char *compat866x_list[] = {
	"vitesse,vsc8662",
	"vitesse,vsc8664",
	"vitesse,vsc866x",
	"\0"
};

static const uint8_t vitesse_qsgmii_vsc8574_program[] = {
	0x44, 0x83, 0x02, 0x42, 0x12, 0x02, 0x44, 0x93, 0x02, 0x44,
	0xca, 0x02, 0x44, 0x4d, 0x02, 0x43, 0xef, 0xed, 0xff, 0xe5,
	0xfc, 0x54, 0x38, 0x64, 0x20, 0x70, 0x08, 0x65, 0xff, 0x70,
	0x04, 0xed, 0x44, 0x80, 0xff, 0x22, 0x8f, 0x19, 0x7b, 0xbb,
	0x7d, 0x0e, 0x7f, 0x04, 0x12, 0x3d, 0xd7, 0xef, 0x4e, 0x60,
	0x03, 0x02, 0x41, 0xf9, 0xe4, 0xf5, 0x1a, 0x74, 0x01, 0x7e,
	0x00, 0xa8, 0x1a, 0x08, 0x80, 0x05, 0xc3, 0x33, 0xce, 0x33,
	0xce, 0xd8, 0xf9, 0xff, 0xef, 0x55, 0x19, 0x70, 0x03, 0x02,
	0x41, 0xed, 0x85, 0x1a, 0xfb, 0x7b, 0xbb, 0xe4, 0xfd, 0xff,
	0x12, 0x3d, 0xd7, 0xef, 0x4e, 0x60, 0x03, 0x02, 0x41, 0xed,
	0xe5, 0x1a, 0x54, 0x02, 0x75, 0x1d, 0x00, 0x25, 0xe0, 0x25,
	0xe0, 0xf5, 0x1c, 0xe4, 0x78, 0xc5, 0xf6, 0xd2, 0x02, 0x12,
	0x41, 0xfa, 0x7b, 0xff, 0x7d, 0x12, 0x7f, 0x07, 0x12, 0x3d,
	0xd7, 0xef, 0x4e, 0x60, 0x03, 0x02, 0x41, 0xe7, 0xc2, 0x02,
	0x74, 0xc7, 0x25, 0x1a, 0xf9, 0x74, 0xe7, 0x25, 0x1a, 0xf8,
	0xe6, 0x27, 0xf5, 0x1b, 0xe5, 0x1d, 0x24, 0x5b, 0x12, 0x44,
	0x2a, 0x12, 0x3e, 0xda, 0x7b, 0xfc, 0x7d, 0x11, 0x7f, 0x07,
	0x12, 0x3d, 0xd7, 0x78, 0xcc, 0xef, 0xf6, 0x78, 0xc1, 0xe6,
	0xfe, 0xef, 0xd3, 0x9e, 0x40, 0x06, 0x78, 0xcc, 0xe6, 0x78,
	0xc1, 0xf6, 0x12, 0x41, 0xfa, 0x7b, 0xec, 0x7d, 0x12, 0x7f,
	0x07, 0x12, 0x3d, 0xd7, 0x78, 0xcb, 0xef, 0xf6, 0xbf, 0x07,
	0x06, 0x78, 0xc3, 0x76, 0x1a, 0x80, 0x1f, 0x78, 0xc5, 0xe6,
	0xff, 0x60, 0x0f, 0xc3, 0xe5, 0x1b, 0x9f, 0xff, 0x78, 0xcb,
	0xe6, 0x85, 0x1b, 0xf0, 0xa4, 0x2f, 0x80, 0x07, 0x78, 0xcb,
	0xe6, 0x85, 0x1b, 0xf0, 0xa4, 0x78, 0xc3, 0xf6, 0xe4, 0x78,
	0xc2, 0xf6, 0x78, 0xc2, 0xe6, 0xff, 0xc3, 0x08, 0x96, 0x40,
	0x03, 0x02, 0x41, 0xd1, 0xef, 0x54, 0x03, 0x60, 0x33, 0x14,
	0x60, 0x46, 0x24, 0xfe, 0x60, 0x42, 0x04, 0x70, 0x4b, 0xef,
	0x24, 0x02, 0xff, 0xe4, 0x33, 0xfe, 0xef, 0x78, 0x02, 0xce,
	0xa2, 0xe7, 0x13, 0xce, 0x13, 0xd8, 0xf8, 0xff, 0xe5, 0x1d,
	0x24, 0x5c, 0xcd, 0xe5, 0x1c, 0x34, 0xf0, 0xcd, 0x2f, 0xff,
	0xed, 0x3e, 0xfe, 0x12, 0x44, 0x6a, 0x7d, 0x11, 0x80, 0x0b,
	0x78, 0xc2, 0xe6, 0x70, 0x04, 0x7d, 0x11, 0x80, 0x02, 0x7d,
	0x12, 0x7f, 0x07, 0x12, 0x3e, 0x9a, 0x8e, 0x1e, 0x8f, 0x1f,
	0x80, 0x03, 0xe5, 0x1e, 0xff, 0x78, 0xc5, 0xe6, 0x06, 0x24,
	0xcd, 0xf8, 0xa6, 0x07, 0x78, 0xc2, 0x06, 0xe6, 0xb4, 0x1a,
	0x0a, 0xe5, 0x1d, 0x24, 0x5c, 0x12, 0x44, 0x2a, 0x12, 0x3e,
	0xda, 0x78, 0xc5, 0xe6, 0x65, 0x1b, 0x70, 0x82, 0x75, 0xdb,
	0x20, 0x75, 0xdb, 0x28, 0x12, 0x44, 0x42, 0x12, 0x44, 0x42,
	0xe5, 0x1a, 0x12, 0x44, 0x35, 0xe5, 0x1a, 0xc3, 0x13, 0x12,
	0x44, 0x35, 0x78, 0xc5, 0x16, 0xe6, 0x24, 0xcd, 0xf8, 0xe6,
	0xff, 0x7e, 0x08, 0x1e, 0xef, 0xa8, 0x06, 0x08, 0x80, 0x02,
	0xc3, 0x13, 0xd8, 0xfc, 0xfd, 0xc4, 0x33, 0x54, 0xe0, 0xf5,
	0xdb, 0xef, 0xa8, 0x06, 0x08, 0x80, 0x02, 0xc3, 0x13, 0xd8,
	0xfc, 0xfd, 0xc4, 0x33, 0x54, 0xe0, 0x44, 0x08, 0xf5, 0xdb,
	0xee, 0x70, 0xd8, 0x78, 0xc5, 0xe6, 0x70, 0xc8, 0x75, 0xdb,
	0x10, 0x02, 0x40, 0xfd, 0x78, 0xc2, 0xe6, 0xc3, 0x94, 0x17,
	0x50, 0x0e, 0xe5, 0x1d, 0x24, 0x62, 0x12, 0x42, 0x08, 0xe5,
	0x1d, 0x24, 0x5c, 0x12, 0x42, 0x08, 0x20, 0x02, 0x03, 0x02,
	0x40, 0x76, 0x05, 0x1a, 0xe5, 0x1a, 0xc3, 0x94, 0x04, 0x50,
	0x03, 0x02, 0x40, 0x3a, 0x22, 0xe5, 0x1d, 0x24, 0x5c, 0xff,
	0xe5, 0x1c, 0x34, 0xf0, 0xfe, 0x12, 0x44, 0x6a, 0x22, 0xff,
	0xe5, 0x1c, 0x34, 0xf0, 0xfe, 0x12, 0x44, 0x6a, 0x22, 0xd2,
	0x00, 0x75, 0xfb, 0x03, 0xab, 0x7e, 0xaa, 0x7d, 0x7d, 0x19,
	0x7f, 0x03, 0x12, 0x3e, 0xda, 0xe5, 0x7e, 0x54, 0x0f, 0x24,
	0xf3, 0x60, 0x03, 0x02, 0x42, 0xb9, 0x12, 0x44, 0xa3, 0x12,
	0x44, 0xaa, 0xd8, 0xfb, 0xff, 0x20, 0xe2, 0x2a, 0x13, 0x92,
	0x04, 0xef, 0xa2, 0xe1, 0x92, 0x03, 0x30, 0x04, 0x1f, 0xe4,
	0xf5, 0x10, 0xe5, 0x10, 0x24, 0x17, 0xfd, 0x7b, 0x54, 0x7f,
	0x04, 0x12, 0x3d, 0xd7, 0x74, 0x25, 0x25, 0x10, 0xf8, 0xa6,
	0x07, 0x05, 0x10, 0xe5, 0x10, 0xc3, 0x94, 0x02, 0x40, 0xe4,
	0x12, 0x44, 0xa3, 0x12, 0x44, 0xaa, 0xd8, 0xfb, 0x54, 0x05,
	0x64, 0x04, 0x70, 0x27, 0x78, 0xc4, 0xe6, 0x78, 0xc6, 0xf6,
	0xe5, 0x7d, 0xff, 0x33, 0x95, 0xe0, 0xef, 0x54, 0x0f, 0x78,
	0xc4, 0xf6, 0x12, 0x42, 0xcf, 0x20, 0x04, 0x0c, 0x12, 0x44,
	0xa3, 0x12, 0x44, 0xaa, 0xd8, 0xfb, 0x13, 0x92, 0x05, 0x22,
	0xc2, 0x05, 0x22, 0x12, 0x44, 0xa3, 0x12, 0x44, 0xaa, 0xd8,
	0xfb, 0x54, 0x05, 0x64, 0x05, 0x70, 0x1e, 0x78, 0xc4, 0x7d,
	0xb8, 0x12, 0x42, 0xc5, 0x78, 0xc1, 0x7d, 0x74, 0x12, 0x42,
	0xc5, 0xe4, 0x78, 0xc1, 0xf6, 0x22, 0x7b, 0x01, 0x7a, 0x00,
	0x7d, 0xee, 0x7f, 0x92, 0x12, 0x38, 0xbd, 0x22, 0xe6, 0xfb,
	0x7a, 0x00, 0x7f, 0x92, 0x12, 0x38, 0xbd, 0x22, 0x78, 0xc1,
	0xe6, 0xfb, 0x7a, 0x00, 0x7d, 0x74, 0x7f, 0x92, 0x12, 0x38,
	0xbd, 0xe4, 0x78, 0xc1, 0xf6, 0xf5, 0x11, 0x74, 0x01, 0x7e,
	0x00, 0xa8, 0x11, 0x08, 0x80, 0x05, 0xc3, 0x33, 0xce, 0x33,
	0xce, 0xd8, 0xf9, 0xff, 0x78, 0xc4, 0xe6, 0xfd, 0xef, 0x5d,
	0x60, 0x44, 0x85, 0x11, 0xfb, 0xe5, 0x11, 0x54, 0x02, 0x25,
	0xe0, 0x25, 0xe0, 0xfe, 0xe4, 0x24, 0x5b, 0xfb, 0xee, 0x12,
	0x44, 0x2d, 0x12, 0x3e, 0xda, 0x7b, 0x40, 0x7d, 0x11, 0x7f,
	0x07, 0x12, 0x3d, 0xd7, 0x74, 0xc7, 0x25, 0x11, 0xf8, 0xa6,
	0x07, 0x7b, 0x11, 0x7d, 0x12, 0x7f, 0x07, 0x12, 0x3d, 0xd7,
	0xef, 0x4e, 0x60, 0x09, 0x74, 0xe7, 0x25, 0x11, 0xf8, 0x76,
	0x04, 0x80, 0x07, 0x74, 0xe7, 0x25, 0x11, 0xf8, 0x76, 0x0a,
	0x05, 0x11, 0xe5, 0x11, 0xc3, 0x94, 0x04, 0x40, 0x9a, 0x78,
	0xc6, 0xe6, 0x70, 0x15, 0x78, 0xc4, 0xe6, 0x60, 0x10, 0x75,
	0xd9, 0x38, 0x75, 0xdb, 0x10, 0x7d, 0xfe, 0x12, 0x43, 0x7d,
	0x7d, 0x76, 0x12, 0x43, 0x7d, 0x79, 0xc6, 0xe7, 0x78, 0xc4,
	0x66, 0xff, 0x60, 0x03, 0x12, 0x40, 0x25, 0x78, 0xc4, 0xe6,
	0x70, 0x09, 0xfb, 0xfa, 0x7d, 0xfe, 0x7f, 0x8e, 0x12, 0x38,
	0xbd, 0x22, 0x7b, 0x01, 0x7a, 0x00, 0x7f, 0x8e, 0x12, 0x38,
	0xbd, 0x22, 0xe4, 0xf5, 0x19, 0x74, 0x25, 0x25, 0x19, 0xf8,
	0xe6, 0x64, 0x03, 0x60, 0x51, 0xe5, 0x19, 0x24, 0x17, 0xfd,
	0x7b, 0xeb, 0x7f, 0x04, 0x12, 0x3d, 0xd7, 0x8f, 0xfb, 0x7b,
	0x22, 0x7d, 0x18, 0x7f, 0x06, 0x12, 0x3d, 0xd7, 0xef, 0x64,
	0x01, 0x4e, 0x60, 0x1c, 0x7d, 0x1c, 0xe4, 0xff, 0x12, 0x3e,
	0x9a, 0xef, 0x54, 0x1b, 0x64, 0x0a, 0x70, 0x15, 0x7b, 0xcc,
	0x7d, 0x10, 0xff, 0x12, 0x3d, 0xd7, 0xef, 0x64, 0x01, 0x4e,
	0x70, 0x07, 0x12, 0x44, 0xb1, 0x7b, 0x03, 0x80, 0x0a, 0x12,
	0x44, 0xb1, 0x74, 0x25, 0x25, 0x19, 0xf8, 0xe6, 0xfb, 0x7a,
	0x00, 0x7d, 0x54, 0x12, 0x38, 0xbd, 0x05, 0x19, 0xe5, 0x19,
	0xc3, 0x94, 0x02, 0x40, 0x9c, 0x22, 0xe5, 0x7e, 0x30, 0xe5,
	0x35, 0x30, 0xe4, 0x0b, 0x7b, 0x02, 0x7d, 0x33, 0x7f, 0x35,
	0x12, 0x36, 0x29, 0x80, 0x10, 0x7b, 0x01, 0x7d, 0x33, 0x7f,
	0x35, 0x12, 0x36, 0x29, 0x90, 0x47, 0xd2, 0xe0, 0x44, 0x04,
	0xf0, 0x90, 0x47, 0xd2, 0xe0, 0x54, 0xf7, 0xf0, 0x90, 0x47,
	0xd1, 0xe0, 0x44, 0x10, 0xf0, 0x7b, 0x05, 0x7d, 0x84, 0x7f,
	0x86, 0x12, 0x36, 0x29, 0x22, 0xfb, 0xe5, 0x1c, 0x34, 0xf0,
	0xfa, 0x7d, 0x10, 0x7f, 0x07, 0x22, 0x54, 0x01, 0xc4, 0x33,
	0x54, 0xe0, 0xf5, 0xdb, 0x44, 0x08, 0xf5, 0xdb, 0x22, 0xf5,
	0xdb, 0x75, 0xdb, 0x08, 0xf5, 0xdb, 0x75, 0xdb, 0x08, 0x22,
	0xe5, 0x7e, 0x54, 0x0f, 0x64, 0x01, 0x70, 0x0d, 0xe5, 0x7e,
	0x30, 0xe4, 0x08, 0x90, 0x47, 0xd0, 0xe0, 0x44, 0x02, 0xf0,
	0x22, 0x90, 0x47, 0xd0, 0xe0, 0x54, 0xfd, 0xf0, 0x22, 0xab,
	0x07, 0xaa, 0x06, 0x7d, 0x10, 0x7f, 0x07, 0x12, 0x3e, 0xda,
	0x7b, 0xff, 0x7d, 0x10, 0x7f, 0x07, 0x12, 0x3d, 0xd7, 0xef,
	0x4e, 0x60, 0xf3, 0x22, 0x12, 0x44, 0xc5, 0x12, 0x44, 0xbb,
	0x90, 0x47, 0xfa, 0xe0, 0x54, 0xf8, 0x44, 0x02, 0xf0, 0x22,
	0x30, 0x04, 0x03, 0x12, 0x43, 0x87, 0x78, 0xc4, 0xe6, 0xff,
	0x60, 0x03, 0x12, 0x40, 0x25, 0x22, 0xe5, 0x7e, 0xae, 0x7d,
	0x78, 0x04, 0x22, 0xce, 0xa2, 0xe7, 0x13, 0xce, 0x13, 0x22,
	0xe5, 0x19, 0x24, 0x17, 0x54, 0x1f, 0x44, 0x80, 0xff, 0x22,
	0xe4, 0x78, 0xc4, 0xf6, 0xc2, 0x05, 0x78, 0xc1, 0xf6, 0x22,
	0xc2, 0x04, 0xc2, 0x03, 0x22, 0x22
};
static const uint16_t vsc8574_program_crc = 0xb7c2;

/**
 * Maps GPIO lines to the global GPIO config registers.
 *
 * Please see the data sheet since the configuration for each GPIO line is
 * different.
 */
static const struct {
	uint32_t config1_status_reg;
	uint32_t config2_reg;
} vsc848x_gpio_to_reg[12] = {
	{ 0x1e0100, 0x1e0101 },	/* 0 */
	{ 0x1e0102, 0x1e0103 },	/* 1 */
	{ 0x1e0104, 0x1e0105 },	/* 2 */
	{ 0x1e0106, 0x1e0107 },	/* 3 */
	{ 0x1e0108, 0x1e0109 },	/* 4 */
	{ 0x1e010A, 0x1e010B },	/* 5 */
	{ 0x1e0124, 0x1e0125 },	/* 6 */
	{ 0x1e0126, 0x1e0127 },	/* 7 */
	{ 0x1e0128, 0x1e0129 },	/* 8 */
	{ 0x1e012a, 0x1e012b },	/* 9 */
	{ 0x1e012c, 0x1e012d },	/* 10 */
	{ 0x1e012e, 0x1e012f },	/* 11 */
};

static int octeon_vitesse_vsc8574_load_fw(const struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info = ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	u16 val;
	int i;
	ulong start;

	debug("%s(%s): Loading VSC8574 firmware\n", __func__, ethdev->name);
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0010);	/* GPIO space */
	phy_write(phydev, MDIO_DEVAD_NONE, 18, 0x800f);	/* uP command */
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0010);	/* GPIO space */
	start = get_timer(0);
	do {
		val = phy_read(phydev, MDIO_DEVAD_NONE, 18);
		if (!(val & (1 << 15)))
			break;

		mdelay(10);
	} while (get_timer(start) < 1000);		/* Wait for command completion */
	if (val & (1 << 15)) {
		printf("%s: Timeout 1\n", __func__);
		return -1;
	}

	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0);	/* Main register space */
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0010);	/* GPIO space */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 12);
	val &= 0x0800;
	phy_write(phydev, MDIO_DEVAD_NONE, 12, val);
	phy_write(phydev, MDIO_DEVAD_NONE, 9, 0x005b);
	phy_write(phydev, MDIO_DEVAD_NONE, 10, 0x005b);
	val = phy_read(phydev, MDIO_DEVAD_NONE, 12);
	val &= 0x0800;
	phy_write(phydev, MDIO_DEVAD_NONE, 12, val);
	phy_write(phydev, MDIO_DEVAD_NONE, 18, 0x800f);
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0);
	val &= 0x0800;
	phy_write(phydev, MDIO_DEVAD_NONE, 0, val);
	phy_write(phydev, MDIO_DEVAD_NONE, 18, 0x0);
	val = phy_read(phydev, MDIO_DEVAD_NONE, 12);
	val &= 0x0800;
	phy_write(phydev, MDIO_DEVAD_NONE, 12, val);
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0010);
	phy_write(phydev, MDIO_DEVAD_NONE, 0, 0x7009);
	phy_write(phydev, MDIO_DEVAD_NONE, 12, 0x5002);
	phy_write(phydev, MDIO_DEVAD_NONE, 11, 0x0000);

	for (i = 0; i < sizeof(vitesse_qsgmii_vsc8574_program); i++)
		phy_write(phydev, MDIO_DEVAD_NONE, 12,
			  0x5000 | vitesse_qsgmii_vsc8574_program[i]);

		phy_write(phydev, MDIO_DEVAD_NONE, 12,  0x0000);
	phy_write(phydev, MDIO_DEVAD_NONE, 3, 0x3eb7);
	phy_write(phydev, MDIO_DEVAD_NONE, 4, 0x4012);
	phy_write(phydev, MDIO_DEVAD_NONE, 12, 0x0100);
	phy_write(phydev, MDIO_DEVAD_NONE, 0, 0x4018);
	phy_write(phydev, MDIO_DEVAD_NONE, 0, 0xC018);

	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0001);
	phy_write(phydev, MDIO_DEVAD_NONE, 25, 0x4000);
	phy_write(phydev, MDIO_DEVAD_NONE, 26,
		  sizeof(vitesse_qsgmii_vsc8574_program) + 1);
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0010);
	phy_write(phydev, MDIO_DEVAD_NONE, 18, 0x8008);

	start = get_timer(0);
	do {
		val = phy_read(phydev, MDIO_DEVAD_NONE, 18);
		if (!(val & (1 << 15)))
			break;
		mdelay(10);
	} while (get_timer(start) < 1000);
	if (val & (1 << 15)) {
		printf("%s: Timeout 2\n", __func__);
		return -1;
	}
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0x0001);
	val = phy_read(phydev, MDIO_DEVAD_NONE, 25);
	if (val != vsc8574_program_crc) {
		printf("Calculated CRC is 0x%x, expected CRC is 0x%x\n",
		       val, vsc8574_program_crc);
		return -1;
	}
	phy_write(phydev, MDIO_DEVAD_NONE, 31, 0);
	return 0;
}

/**
 * This function is called before and after all of the PHY initialization
 * to control the Vitesse COMA mode.
 */
int __octeon_vitesse_coma(bool start)
{
	int i, gpio, val;
	int old_bus __attribute__((unused));

	debug("%s: Entry\n", __func__);
	for (i = 0; i < num_coma_gpios; i++) {
		gpio = coma_gpios[i].pin;
		val = coma_gpios[i].invert ^ !!start;
		debug("%s: Setting GPIO %d to %d\n", __func__, gpio, val);

		switch (coma_gpios[i].type) {
		case CVMX_GPIO_PIN_OCTEON:
			debug("Setting OCTEON GPIO coma pin %d to %d\n",
			      gpio, val);
			gpio_set_value(gpio, val);
			break;
#ifdef CONFIG_PCA953X
		case CVMX_GPIO_PIN_PCA953X:
			debug("Setting PCA953X at 0x%x:0x%x GPIO coma pin %d to %d\n",
			      coma_gpios[i].i2c_bus, coma_gpios[i].i2c_addr,
			      gpio, val);
			pca953x_set_val(coma_gpios[i].i2c_bus,
					coma_gpios[i].i2c_addr,
					1 << gpio,
					val << gpio);
			break;
#endif
#ifdef CONFIG_PCF857X
		case CVMX_GPIO_PIN_PCF857X:
			debug("Setting PCF857X at 0x%x:0x%x GPIO coma pin %d to %d\n",
			      coma_gpios[i].i2c_bus, coma_gpios[i].i2c_addr,
			      gpio, val);
			pcf857x_set_val(coma_gpios[i].i2c_bus,
					coma_gpios[i].i2c_addr,
					1 << gpio,
					val << gpio);
			break;
#endif
#ifdef CONFIG_PCA9698
		case CVMX_GPIO_PIN_PCA9698:
			debug("Setting PCA9698 at 0x%x:0x%x GPIO coma pin %d to %d\n",
			      coma_gpios[i].i2c_bus, coma_gpios[i].i2c_addr,
			      gpio, val);
			old_bus = i2c_get_bus_num();
			i2c_set_bus_num(coma_gpios[i].i2c_bus);
			pca9698_set_value(coma_gpios[i].i2c_addr, gpio, val);
			i2c_set_bus_num(old_bus);
			break;
#endif
		default:
			printf("%s: Unsupported GPIO type %d for GPIO pin %d\n",
			       __func__, coma_gpios[i].type, gpio);
			return -1;
		}
	}
	return 0;
}

int octeon_vitesse_coma(bool start)
__attribute__((weak, alias("__octeon_vitesse_coma")));

static int octeon_fdt_vitesse_vsc8574_config(const struct eth_device *ethdev)
{
	const struct octeon_eth_info *oct_eth_info =
	(struct octeon_eth_info *)ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	int phy_node_offset;
	int rc = 0;
	const uint32_t *pgpio_handle;
	int gpio;
	enum cvmx_gpio_type gpio_type;
	int invert;
	int len;
	int phandle;
	int value;
	const char *media_type;
	ulong start;
	enum vsc8574_media_mode media_mode = MM_COPPER;

	phy_node_offset = oct_eth_info->phy_fdt_offset;
	if (phy_node_offset < 0) {
		printf("%s: Error: could not find phy info for phy device\n",
		       __func__);
		return -1;
	}

	/* For the first PHY follow the configuration steps outlined in
	 * the data sheet configuration section 2.21.
	 */
	pgpio_handle = fdt_getprop(gd->fdt_blob, phy_node_offset, "coma", &len);
	if (pgpio_handle) {

		if (len != 12) {
			printf("Invalid coma property length for %s\n",
			       ethdev->name);
			return -1;
		}
		phandle = fdt32_to_cpu(pgpio_handle[0]);
		gpio = fdt32_to_cpu(pgpio_handle[1]);
		invert = fdt32_to_cpu(pgpio_handle[2]);

		gpio_type = cvmx_fdt_get_gpio_type(gd->fdt_blob, phandle, NULL);
		if (gpio_type != CVMX_GPIO_PIN_OCTEON) {
			int bus, addr;

			if (cvmx_fdt_get_twsi_gpio_bus_addr(gd->fdt_blob,
							    phandle, &bus,
							    &addr)) {
				printf("%s: Error: could not get i2c bus and address for i2c GPIO COMA for %s\n",
				       __func__, ethdev->name);
				goto no_coma;
			}
			coma_gpios[num_coma_gpios].i2c_addr = addr;
			coma_gpios[num_coma_gpios].i2c_bus = bus;
			debug("%s: I2C coma pin for %s connected to I2C GPIO expander @ 0x%x:0x%x\n",
			      __func__, ethdev->name, bus, addr);
		}
		coma_gpios[num_coma_gpios].type = gpio_type;
		coma_gpios[num_coma_gpios].pin = gpio & COMA_GPIO_MASK;
		coma_gpios[num_coma_gpios].invert = !!invert;
		debug("%s: Added coma GPIO %d to index %d for %s\n", __func__, gpio,
		      num_coma_gpios, ethdev->name);

		num_coma_gpios++;

		if (num_coma_gpios >= MAX_COMA_GPIOS) {
			num_coma_gpios--;
			printf("ERROR: Maximum number of COMA GPIO lines (%d) exceeded\n",
			       MAX_COMA_GPIOS);
			rc = -1;
		}

		/* 1. COMA_MODE active, drive high (hopefully this is done
		 * earlier
		 */
		debug("Setting GPIO %d to %d for %s\n", gpio, !invert,
		      ethdev->name);
		gpio_set_value(gpio, !invert);
	}
 no_coma:
	/* 2. Apply Power (hopefully done elsewhere)
	 * 3. Apply RefClk
	 * 4. Release reset, drive high. Power and clock must be stable before
	 *    releasing reset.
	 * 5. Wait 120ms minimum.
	 *
	 * 6. Apply patch from PHY_API (required for production released (sic)
	 *    optional for board testing).
	 */
	if ((phydev->addr & 3) == 0) {
		debug("%s(%s): Loading firmware\n", __func__,
		      ethdev->name);
		/* 6. Apply patch from PHY_API (required for production) */
		rc = octeon_vitesse_vsc8574_load_fw(ethdev);
		if (rc) {
			printf("%s: Error loading firmware for %s\n",
			       __func__, ethdev->name);
			return -1;
		}

		/* 7. Configure register 19G for MAC mode (to access register
		 *    19G, register 31 must be 0x10).  Read register 19G.  Set
		 *    bits 15:14, MAC configuration as follows:
		 *    00: SGMII
		 *    01: QSGMII
		 *    10: Reserved
		 *    11: Reserved
		 *    Write new register 19G.
		 */
		/* Access CPU/GPIO registers */
		if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0x10)))
			goto phy_err;
		value = phy_read(phydev, MDIO_DEVAD_NONE, 19);
		if (value < 0) {
			rc = value;
			goto phy_err;
		}

		value = value & 0x3fff;
		if (phydev->interface == PHY_INTERFACE_MODE_QSGMII)
			value |= 0x4000;
		/* else SGMII */
		if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 19, value)))
			goto phy_err;

		/* 8. Configure register 18G for MAC on all 4 PHYs write:
		 *    SGMII: 0x80F0
		 *    QSGMII: 0x80e0
		 *    Read register 18G until bit 15 equals 0
		 */
		value = (phydev->interface == PHY_INTERFACE_MODE_QSGMII) ?
		0x80E0 : 0x80F0;
		if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 18, value)))
			goto phy_err;

		start = get_timer(0);
		do {
			value = phy_read(phydev, MDIO_DEVAD_NONE, 18);
			if (value < 0)
				goto phy_err;
		} while ((value & 0x8000) && get_timer(start) < 1000);
		if (value & 0x8000) {
			printf("Timed out waiting for MAC mode configuration for %s\n",
			       ethdev->name);
			goto phy_err;
		}
		/* 9. If Fiber Media on all 4 PHYs configure register 18G by
		 *    writing:
		 *    Media 1000BX: 0x8FC1
		 *    Media 100FX: 0x8FD1
		 */
		media_type = fdt_getprop(gd->fdt_blob, phy_node_offset,
					 "optical-media-type", NULL);
		if (media_type) {
			/* Optical media attached */
			if (!strcmp(media_type, "1000bx")) {
				media_mode = MM_1000BX;
				value = 0x8fc1;
			} else if (!strcmp(media_type, "100fx")) {
				media_mode = MM_100FX;
				value = 0x8fd1;
			} else {
				printf("Unknown optical media type %s for phy %s\n",
				       media_type, ethdev->name);
				goto phy_err;
			}
			if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 18, value)))
				goto phy_err;

			/* 10. If Fiber Media read register 18G till (SIC) bit 15
			 * equals 0.
			 */
			start = get_timer(0);
			do {
				value = phy_read(phydev, MDIO_DEVAD_NONE, 18);
				if (value < 0)
					goto phy_err;
			} while ((value & 0x8000) && get_timer(start) < 1000);
			if (value & 0x8000) {
				printf("Timed out waiting for PHY optical configuration for %s\n",
				       ethdev->name);
				goto phy_err;
			}
		}

		/* 11. Configure register 23 for MAC and Media mode (to access
		 *     register 23, register 31 must be 0). Read register 23.
		 *     Set bits 10:8 as follows:
		 *     000: Copper
		 *     010: 1000BX
		 *     011: 100FX
		 *     Write new register 23.
		 */
		if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 31, 0)))
			goto phy_err;

		value = phy_read(phydev, MDIO_DEVAD_NONE, 23);
		if (value < 0)
			goto phy_err;
		value &= 0xf8ff;
		if (media_mode == MM_1000BX)
			value |= 0x0200;
		else if (media_mode == MM_100FX)
			value |= 0x0300;
		if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 23, value)))
			goto phy_err;
	}

	/* 12. Software reset. Read register 0 (to access register 0,
	 *     register 31 must be 0). Set bit 15 to 1.
	 *     Write new register 0.
	 */
	if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 31, 0)))
		goto phy_err;

	value = phy_read(phydev, MDIO_DEVAD_NONE, 0);
	if (value < 0)
		goto phy_err;
	value |= 0x8000;
	if ((rc = phy_write(phydev, MDIO_DEVAD_NONE, 0, value)))
		goto phy_err;

	start = get_timer(0);
	do {
		value = phy_read(phydev, MDIO_DEVAD_NONE, 0);
		if (value < 0)
			goto phy_err;
	} while ((value & 0x8000) && get_timer(start) < 1000);

	phy_err:
	if (rc) {
		printf("%s: PHY I/O error accessing PHY for %s\n",
		       __func__, ethdev->name);
	}

	return rc;
}

/**
 * This performs initialization for the Vitesse VSC866x PHY using a number
 * of fields in the device tree.  The default values are chosen to work
 * with the NIC401NVG board.
 *
 * @param[in]	phydev	PHY device
 * @param	offset	Offset of node in device tree
 *
 * @return 0 for success, -1 for error
 */
static int octeon_vitesse_vsc866x_init(const struct eth_device *ethdev,
				       int offset)
{
	const struct octeon_eth_info *oct_eth_info =
	(struct octeon_eth_info *)ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	int cmode_mac_an;
	int cmode_mode;
	int cmode_sigdet_polarity;
	int cmode_mac_mode;
	int serdes_mac_loopback_mode;
	int serdes_media_loopback_mode;
	int led_mode = 0x403;
	int led_behavior = 0x8002;
	int tx_en_gpio;
	const void *fdt = gd->fdt_blob;
	int rc;
	int val;

	debug("%s(%s) ENTRY\n", __func__, ethdev->name);

	cmode_mode = fdtdec_get_int(fdt, offset, "cmode-mode", 2);
	debug("%s(%s) cmode mode: %d\n", __func__, ethdev->name,
	      cmode_mode);

	cmode_sigdet_polarity = fdtdec_get_int(fdt, offset,
					       "cmode-sigdet-polarity", 1);
	debug("%s(%s) cmode sigdet polarity: %d\n", __func__, ethdev->name,
	      cmode_sigdet_polarity);

	cmode_mac_mode = fdtdec_get_int(fdt, offset, "cmode-mac-mode", 1);
	debug("%s(%s) cmode mac mode: %d\n", __func__, ethdev->name,
	      cmode_mac_mode);

	cmode_mac_an = fdtdec_get_int(fdt, offset, "cmode-mac-an", 0);
	debug("%s(%s) cmode mac autonegotiation: %d\n",
	      __func__, ethdev->name, cmode_mac_an);

	serdes_mac_loopback_mode = fdtdec_get_int(fdt, offset,
						  "serdes-mac-loopback-mode", 3);
	debug("%s(%s) serdes mac loopback mode: %d\n", __func__, ethdev->name,
	      serdes_mac_loopback_mode);

	serdes_media_loopback_mode = fdtdec_get_int(fdt, offset,
						    "serdes-media-loopback-mode",
					     3);
	debug("%s(%s) serdes media loopback mode: %d\n", __func__, ethdev->name,
	      serdes_media_loopback_mode);

	led_mode = fdtdec_get_int(fdt, offset, "led-mode", 0x403);
	debug("%s(%s) LED mode: 0x%x\n", __func__, ethdev->name, led_mode);

	led_behavior = fdtdec_get_int(fdt, offset, "led-behavior", 0x8002);
	debug("%s(%s) LED behavior: 0x%x\n", __func__, ethdev->name,
	      led_behavior);

	tx_en_gpio = fdtdec_get_int(fdt, offset, "tx-enable-gpio", -1);
	debug("%s(%s) TX enable GPIO: %d\n", __func__, ethdev->name,
	      tx_en_gpio);

	/* Configure the LEDs */
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0);
	if (rc < 0)
		goto error;

	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1e, led_mode);
	if (rc < 0)
		goto error;
	debug(" 0x001e = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x1e));

	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1d, led_behavior);
	if (rc < 0)
		goto error;
	debug(" 0x001d = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x1d));

	/* Enable TX */
	if (tx_en_gpio >= 0) {
		debug("  Clearing tx enable GPIO %d\n", tx_en_gpio);
		rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0x10);
		if (rc < 0)
			goto error;

		val = phy_read(phydev, MDIO_DEVAD_NONE, 0x11);
		if (val < 0)
			goto error;
		val |= 1 << tx_en_gpio;
		debug("Setting gpio %d to output, reg: 0x%x\n", tx_en_gpio,
		      val);
		rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x11, val);
		if (rc < 0)
			goto error;
		debug(" 0x0011 = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x11));

		val = phy_read(phydev, MDIO_DEVAD_NONE, 0x10);
		if (val < 0)
			goto error;
		val &= ~(1 << tx_en_gpio);
		rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x10, val);
		if (rc < 0)
			goto error;
		debug(" 0x0010 = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x10));
		/* Reset registers back to normal */
		rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0x00);
		if (rc < 0)
			goto error;
	}

	/* Update cmode pins */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0x17);
	if (val < 0)
		goto error;
	if (cmode_mac_an)
		val |= 1 << 13;
	else
		val &= ~(1 << 13);

	val &= ~(0x700);
	val |= (cmode_mode << 8);

	if (cmode_mac_mode)
		val |= (1 << 12);
	else
		val &= ~(1 << 12);

	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x17, val);
	if (rc < 0)
		goto error;
	debug(" 0x0017 = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x17));

	/* Standard registers */
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0);
	if (rc < 0)
		goto error;

	/* Do soft reset */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0);
	if (val < 0)
		goto error;
	val |= 0x8000;
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0, val);
	if (rc < 0)
		goto error;
	mdelay(1);

	/* Extended registers */
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 1);
	if (rc < 0)
		goto error;

	/* SigDet polarity */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0x13);
	if (val < 0)
		goto error;

	/* Set MAC and Media modes */
	val &= ~0x361;

	if (cmode_sigdet_polarity)
		val |= 1;

	val |= (serdes_mac_loopback_mode & 3) << 5;
	val |= (serdes_media_loopback_mode & 3) << 8;
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x13, val);
	if (rc < 0)
		goto error;
	debug(" 0x0013 = 0x%x\n", phy_read(phydev, MDIO_DEVAD_NONE, 0x13));

	/* Standard registers */
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0);
	if (rc < 0)
		goto error;

	/* Do soft reset */
	val = phy_read(phydev, MDIO_DEVAD_NONE, 0);
	if (val < 0)
		goto error;
	val |= 0x8000;
	rc = phy_write(phydev, MDIO_DEVAD_NONE, 0, val);
	if (rc < 0)
		goto error;
	mdelay(1);

	debug("%s(%s) done\n", __func__, ethdev->name);
	return 0;

	error:
	printf("MDIO error communicating with phy %s\n", ethdev->name);
	return -1;
}

/**
 * Sets a GPIO pin on the Vitesse phy either high or low.
 *
 * @param[in]	phydev	PHY device to output
 * @param	pin	Pin number to output
 * @param	enable	true to pull GPIO high, false for low
 *
 * @return	0 on success, -1 if pin out of range
 *
 * NOTE: This will force the GPIO into general purpose output mode.
 */
int octeon_vitesse_vsc8488_gpio_set(struct phy_device *phydev, int pin,
				    bool enable)
{
	struct phy_device phy_clone;
	int dev;
	uint16_t reg, val;

	val = phy_read(phydev, 0x1e, 0);
	/* Only device 0 can be used for GPIO with the 8490 */
	if (val == 0x8490 && phydev->addr & 1) {
		memcpy(&phy_clone, phydev, sizeof(phy_clone));
		phy_clone.addr &= ~1;
		phydev = &phy_clone;
	}

	if (pin >= ARRAY_SIZE(vsc848x_gpio_to_reg)) {
		printf("%s(%s) Error: GPIO pin %d out of range\n", __func__,
		       phydev->dev->name, pin);
		return -1;
	}
	dev = vsc848x_gpio_to_reg[pin].config1_status_reg >> 16;
	reg = vsc848x_gpio_to_reg[pin].config1_status_reg & 0xffff;
	val = (!!enable << 12);
	debug("%s(%s) Setting pin %d to %d (reg: 0x%x)\n", __func__,
	      phydev->dev->name, pin, enable, reg);
	phy_write(phydev, dev, reg, val);
	return 0;
}

/**
 * Read the GPIO input from the phy.  If the pin is configured as an output,
 * read back the output value, otherwise read back the input value.
 *
 * @param[in]	phydev	PHY device to read
 * @param	pin	pin number to read
 *
 * @return	0 if pin is low, 1 if pin is high, -1 if pin out of range
 */
int octeon_vitesse_vsc8488_gpio_get(struct phy_device *phydev, int pin)
{
	struct phy_device phy_clone;
	int dev;
	uint16_t reg, val;

	val = phy_read(phydev, 0x1e, 0);
	/* Only device 0 can be used for GPIO with the 8490 */
	if (val == 0x8490 && phydev->addr & 1) {
		memcpy(&phy_clone, phydev, sizeof(phy_clone));
		phy_clone.addr &= ~1;
		phydev = &phy_clone;
	}

	if (pin >= ARRAY_SIZE(vsc848x_gpio_to_reg)) {
		printf("%s(%s) Error: GPIO pin %d out of range\n", __func__,
		       phydev->dev->name, pin);
		return -1;
	}
	dev = vsc848x_gpio_to_reg[pin].config1_status_reg >> 16;
	reg = vsc848x_gpio_to_reg[pin].config1_status_reg & 0xffff;

	val = phy_read(phydev, dev, reg);

	debug("%s(%s) pin: %d configured as %sput, read %d\n",
	      __func__, phydev->dev->name, pin, val & (1 << 15) ? "out" : "in",
	      val & (1 << 15) ? !!(val & (1 << 12)) : !!(val & (1 << 10)));
	if (val & (1 << 15))	/* Read output value back */
		return !!(val & (1 << 12));
	else	/* Read input value */
		return !!(val & (1 << 10));
}

/**
 * Perform initialization for the Vitesse VSC8488 10G PHY
 *
 * @param ethdev	Ethernet device PHY belongs to
 * @param offset	offset of node in device tree
 *
 * @return	0 for success, error otherwise
 */
static int octeon_vitesse_vsc8488_config(const struct eth_device *ethdev)
{
	const void *fdt = gd->fdt_blob;
	const struct octeon_eth_info *oct_eth_info =
	(struct octeon_eth_info *)ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	int txon_gpio;
	enum cvmx_gpio_type gpio_type;
	int rx_eq;
	int tx_preemph;
	int tx_out_driver_ctl1;
	int tx_out_driver_ctl2;
	const uint32_t *nodep;
	int value;
	int phandle;
	int offset = oct_eth_info->phy_fdt_offset;

	debug("%s(%s, %d)\n", __func__, ethdev->name, offset);

	/* The following are only supported on the VSC8488 for now */
	tx_preemph = getenv_ulong("vsc8488_preemph", 16, 0xffffffff);
	if (tx_preemph != 0xffffffff) {
		printf("Overriding PHY XS XAUI TX Pre-emphasis Control register to 0x%x\n",
		       tx_preemph);
		/* PHYXS_TYPE_CTRL: PHY XS XAUI Tx Pre-emphasis Control
		 *  14-13: LANE0_PE
		 *	00: 0 dB
		 *	01: ~2.5dB
		 *	10: ~6dB (default)
		 *	11: ~12dB
		 *
		 *  11-10: LANE1_PE
		 *  8-7: LANE2_PE
		 *  5-4: LANE3_PE
		 *  3-2: LOS_THRES
		 *	11: 80mV to 205mV differential peak-peak
		 *	10: 70mV to 195mV differential peak-peak
		 *	01: 60mV to 185mV differential peak-peak
		 *	00: 50mV to 175mV differential peak-peak (default)
		 *  1: HS_ENA Enable XAUI output high swing mode
		 *	0: disable (default), 1: enable
		 */
		phy_write(phydev, 4, 0x8011, tx_preemph);
	} else {
		tx_preemph = fdtdec_get_int(fdt, offset,
					    "vitesse,tx_preemphasis", -1);
		if (tx_preemph >= 0) {
			debug("%s: Writing 0x%x to TX pre-emphasis\n",
			      __func__, tx_preemph);
			phy_write(phydev, 4, 0x8011, tx_preemph);
		}
	}

	rx_eq = getenv_ulong("vsc8488_rxeq", 16, 0xffffffff);
	if (rx_eq != 0xffffffff) {
		printf("Overriding PHY XS XAUI Rx Equalization Control with 0x%x\n",
		       rx_eq);
		/* PHYXS_RXEQ_CTL: PHY XS Rx Equalization Control
		 *  15-12: LANE0_EQ
		 *	0000: 0 dB
		 *	0001: 1.41 dB
		 *	0010: 2.24 dB
		 *	0011: 2.83 dB
		 *	0101: 4.48 dB
		 *	0110: 5.39 dB
		 *	0111: 6.07 dB
		 *	1001: 6.18 dB
		 *	1010: 7.08 dB (default)
		 *	1011: 7.79 dB
		 *	1101: 9.96 dB
		 *	1110: 10.84 dB
		 *	1111: 11.55 dB
		 *  11-8: LANE1_EQ
		 *  7-4: LANE2_EQ
		 *  3-0: LANE3_EQ
		 */
		phy_write(phydev, 4, 0x8010, rx_eq);
	} else {
		rx_eq = fdtdec_get_int(fdt, offset,
				       "vitesse,rx_equalization", -1);
		if (rx_eq >= 0) {
			debug("%s: Writing 0x%x to RX equalization\n",
			      __func__, rx_eq);
			phy_write(phydev, 4, 0x8010, rx_eq);
		}
	}

	tx_out_driver_ctl1 = getenv_ulong("vsc8488_txdrvctrl1", 16, 0xffffffff);
	if (tx_out_driver_ctl1 != 0xffffffff) {
		printf("Overriding TX Output Driver Control 1 with 0x%x\n",
		       tx_out_driver_ctl1);
		phy_write(phydev, 1, 0x8014, tx_out_driver_ctl1);
	} else {
		tx_out_driver_ctl1 = fdtdec_get_int(fdt, offset,
						    "vitesse,txout_driver_ctrl1",
						    -1);
		if (tx_out_driver_ctl1 >= 0) {
			debug("%s: Writing 0x%x to TX Output driver control 1\n",
			      __func__, tx_out_driver_ctl1);
			phy_write(phydev, 1, 0x8013, tx_out_driver_ctl1);
		}
	}

	tx_out_driver_ctl2 = getenv_ulong("vsc8488_txdrvctrl2", 16, 0xffffffff);
	if (tx_out_driver_ctl2 != 0xffffffff) {
		printf("Overriding TX Output Driver Control 2 with 0x%x\n",
		       tx_out_driver_ctl2);
		phy_write(phydev, 1, 0x8014, tx_out_driver_ctl2);
	} else {
		tx_out_driver_ctl2 = fdtdec_get_int(fdt, offset,
						    "vitesse,txout_driver_ctrl2",
						    -1);
		if (tx_out_driver_ctl2 >= 0) {
			debug("%s: Writing 0x%x to TX Output driver control 2\n",
			      __func__, tx_out_driver_ctl2);
			phy_write(phydev, 1, 0x8014, tx_out_driver_ctl2);
		}
	}


	txon_gpio = fdtdec_get_int(fdt, offset, "txon", -1);
	if (txon_gpio >= 0) {
		debug("%s: Turning on transmit with GPIO %d for %s\n",
		      __func__, txon_gpio, ethdev->name);
		gpio_type = CVMX_GPIO_PIN_OTHER;	/* Assume PHY */
		nodep = fdt_getprop(fdt, offset, "txon-parent", NULL);
		if (nodep) {
			phandle = fdt32_to_cpu(*nodep);
			gpio_type = cvmx_fdt_get_gpio_type(fdt, phandle, NULL);
		}
		switch (gpio_type) {
		case CVMX_GPIO_PIN_OTHER:
			if (txon_gpio >= ARRAY_SIZE(vsc848x_gpio_to_reg)) {
				printf("Error: txon gpio %d is out of range on %s\n",
				       txon_gpio, ethdev->name);
				return -1;
			}
			value = vsc848x_gpio_to_reg[txon_gpio].config1_status_reg;

			debug("  Using Vitesse GPIO %d\n", txon_gpio);
			phy_write(phydev, value >> 16, value & 0xffff, 0);
			break;
		case CVMX_GPIO_PIN_OCTEON:
			debug("  Using OCTEON GPIO pin %d\n", txon_gpio);
			gpio_direction_output(txon_gpio, 0);
			break;
		default:
			printf("Unsupported GPIO type for TXON for %s\n",
			       ethdev->name);
			return -1;
		}
	} else {
		printf("Could not find txon GPIO for %s\n", ethdev->name);
	}
	return 0;
}

#ifdef CONFIG_PHY_SFP
/**
 * This function gets called whenever a module is inserted or removed for the
 * VSC8490/VSC8489 device
 *
 * @param[in]	dev	Ethernet device
 * @param	mod_abs	True if module was removed, false if inserted
 *
 * @return	0 for success, otherwise error
 */
int vsc8490_mod_abs_changed(struct eth_device *dev, bool mod_abs, void *data)
	__attribute__((weak, alias("__vsc8490_mod_abs_changed")));

int __vsc8490_mod_abs_changed(struct eth_device *dev, bool mod_abs, void *data)
{
	/* struct octeon_eth_info *oct_eth_info = dev->priv; */

	if (!mod_abs) {
		debug("%s(%s): module detected\n", __func__, dev->name);
	} else {
		debug("%s(%s): module removed\n", __func__, dev->name);
	}
	return 0;
}
#endif

/**
 * Initializes a Vitesse VSC8490 PHY device
 *
 * @param ethdev	Ethernet device pointer
 *
 * @return 0 for success, -1 for error.
 */
int octeon_fdt_vitesse_vsc8490_config(struct eth_device *ethdev)
{
	/** Mapping of GPIO pins to register addresses, device is always 0x1e. */
	static const uint16_t vsc8490_gpio_reg_addr[] = {
		0x100, 0x102, 0x104, 0x106, 0x108, 0x10a, 0x124, 0x126,
		0x128, 0x12a, 0x12c, 0x12e, 0x130, 0x132, 0x134, 0x136
	};
	const void *fdt = gd->fdt_blob;
	const struct octeon_eth_info *oct_eth_info =
				(struct octeon_eth_info *)ethdev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;
	struct phy_device phydev_copy;
	int phy_node_offset;
	uint32_t devid;
	uint32_t reg;
	int value;
	uint32_t *init_seq;
	int len, seq_len;
	int offset = oct_eth_info->phy_fdt_offset;
	int delay;
	int mphy_offset;
	int tx_enable_gpio = -1;
	int ieee1588_ts_gpio = -1;
	int wis_inta_gpio = -1, wis_intb_gpio = -1;
	const char *str;

	debug("%s(%s, %d)\n", __func__, ethdev->name, offset);

	if (!phydev) {
		printf("%s: Error: could not get phy device for %s\n",
		       __func__, ethdev->name);
		return -1;
	}

	phy_node_offset = oct_eth_info->phy_fdt_offset;
	if (phy_node_offset < 0) {
		printf("%s: Error: could not find phy info for phy device\n",
		       __func__);
		return -1;
	}
	if (fdt_node_check_compatible(fdt, phy_node_offset, "vitesse,vsc8490") &&
	    fdt_node_check_compatible(fdt, phy_node_offset, "vitesse,vsc8489")) {
		printf("Error: incompatible PHY device, expected %s\n",
		       "vitesse,vsc8490");
		return -1;
	}

	mphy_offset = fdt_parent_offset(fdt, phy_node_offset);
	if (fdt_node_check_compatible(fdt, mphy_offset,
				      "vitesse,vsc8490-nexus") &&
	    fdt_node_check_compatible(fdt, mphy_offset,
				      "vitesse,vsc8489-nexus")) {
		printf("Error: incompatible PHY device, expected %s\n",
		       "vitesse,vsc8490-nexus");
		return -1;
	}

	tx_enable_gpio = fdtdec_get_int(fdt, phy_node_offset,
					"tx-enable-gpio", -1);
	debug("%s: %s tx-enable-gpio: %d\n", __func__, ethdev->name,
	      tx_enable_gpio);

	ieee1588_ts_gpio = fdtdec_get_int(fdt, phy_node_offset,
					  "ieee1588-load-save-ts-enable", -1);

	wis_inta_gpio = fdtdec_get_int(fdt, phy_node_offset,
				       "wis-inta-gpio", -1);
	wis_intb_gpio = fdtdec_get_int(fdt, phy_node_offset,
				       "wis-intb-gpio", -1);

	memcpy(&phydev_copy, phydev, sizeof(phydev_copy));
	phydev_copy.addr &= 0x1e;	/* always device 0 in pair */

	/* Set up TX disable GPIOs */
	if (tx_enable_gpio >= 0 && tx_enable_gpio <= 15) {
		/* Set transmit disable to low to enable transmitter */
		reg = vsc8490_gpio_reg_addr[tx_enable_gpio];
		phy_write(&phydev_copy, 0x1e, reg, 0x0000);
		debug("%s: TX enable on GPIO %d\n", __func__, tx_enable_gpio);
	} else {
		debug("%s: TX enable GPIO not set for %s\n",
		      __func__, ethdev->name);
	}

	/* NOTE: With this PHY device, all writes to devid 0x1e must be written
	 * to the first device in a multi-device package.  Since the VSC8490 is
	 * a dual device package, all writes to devid 0x1e must always be
	 * written to device 0, devid 0x1e. The VSC8491 is a single device so
	 * no such changes are needed for it, but so far we only need to deal
	 * with the VSC8490.
	 */
	init_seq = (uint32_t *)fdt_getprop(fdt, phy_node_offset,
					   "vitesse,reg-init", &len);
	if (init_seq) {
		if (len % 16) {
			printf("%s: Invalid sequence length %d\n",
			       __func__, len);
			return -1;
		}
		seq_len = len / 16;
		while (seq_len--) {
			delay = fdt32_to_cpu(*init_seq++);
			devid = fdt32_to_cpu(*init_seq++);
			reg = fdt32_to_cpu(*init_seq++);
			value = fdt32_to_cpu(*init_seq++);
			if ((phydev->addr & 1) && devid == 0x1e) {
				memcpy(&phydev_copy, phydev,
				       sizeof(phydev_copy));
				phydev_copy.addr &= 0x1e;
				mdelay(delay);
				phy_write(&phydev_copy, devid, reg, value);
			} else {
				mdelay(delay);
				phy_write(phydev, devid, reg, value);
			}
			if (delay)
				debug("(void)ms_sleep(%d);\n", delay);
			debug("(void)reg_write(%d, 0x%02x, 0x%04x, 0x%04x);\n",
			      (phydev->addr & 1) && devid != 0x1e, devid, reg,
			      value);
		}
	}

	/* Set up GPIOs */
	if (ieee1588_ts_gpio == 1) {
		reg = vsc8490_gpio_reg_addr[ieee1588_ts_gpio];
		phy_write(&phydev_copy, 0x1e, reg, 0x8014);
		debug("%s: IEEE1588 load/save TS enabled for GPIO 1\n",
		      __func__);
	} else if (ieee1588_ts_gpio > 0) {
		printf("Error: ieee1588-load-save-ts-enable must be 1 when defined\n");
		return -1;
	}
	/* WIS interrupt A supported on GPIOs 1, 5, 7, 8, 9, 11, 13 and 15 */
	if (wis_inta_gpio >= 0 && wis_inta_gpio <= 15 &&
	    ((1 << wis_inta_gpio) & 0xaba2)) {
		reg = vsc8490_gpio_reg_addr[wis_inta_gpio];
		value = phy_read(&phydev_copy, 0x1e, reg);
		debug("%s: addr: 0x%x, WIS INTA GPIO reg: 0x%x\n", __func__,
		      phydev->addr, value);
		value &= 0x00ff;
		if (phydev->addr & 1) {
			if (value == 0x02)
				value = 0xa2;
			else
				value = 0x22;
		} else {
			if (value == 0x22)
				value = 0xa2;
			else
				value = 0x2;
		}
		debug("%s: Writing 0x%x to 0x%x for WIS INTA channel %d for GPIO %d\n",
		      __func__, value, reg, phydev->addr & 1, wis_inta_gpio);
		phy_write(&phydev_copy, 0x1e, reg, value);
	} else if (wis_inta_gpio >= 0) {
		printf("%s: Invalid GPIO %d for WIS INTA\n",
		       __func__, wis_inta_gpio);
		return -1;
	}

	/* WIS interrupt B supported on GPIOs 0, 3, 4, 6, 10, 12, and 14 */
	if (wis_intb_gpio >= 0 && wis_intb_gpio <= 14 &&
	    ((1 << wis_intb_gpio) & 0x5459)) {
		reg = vsc8490_gpio_reg_addr[wis_intb_gpio];
		value = phy_read(&phydev_copy, 0x1e, reg);
		debug("%s: WIS INTB GPIO reg: 0x%x\n", __func__, value);
		value &= 0x00ff;
		if (phydev->addr & 1) {
			if (value == 0x02)
				value = 0xa2;
			else
				value = 0x22;
		} else {
			if (value == 0x22)
				value = 0xa2;
			else
				value = 0x2;
		}
		debug("%s: Writing 0x%x to 0x%x for WIS INTB channel %d for GPIO %d\n",
		      __func__, value, reg, phydev->addr & 1, wis_intb_gpio);
		phy_write(&phydev_copy, 0x1e, reg, value);
	} else if (wis_intb_gpio >= 0) {
		printf("%s: Invalid GPIO %d for WIS INTB\n",
		       __func__, wis_inta_gpio);
		return -1;
	}

	/* Set RXAUI mode to either "Dune" or "Marvell" */
	if ((str = (char *)fdt_getprop(gd->fdt_blob, phy_node_offset,
				       "rxaui-mode", NULL))) {
		value = phy_read(phydev, 4, 0xf007);
		/* Bits in this register are:
		 *   3: RX_8B10B_ILV_DIS
		 *      0 - interleaved data stream is 8b10b
		 *      1 - each lane is individually 8b10b
		 *   2: TX_8B10B_ILV_DIS
		 *      0 - interleaved data stream  is 8b10b
		 *      1 - each lane is individually 8b10b
		 *   1: ILV_MODE
		 *      0 - interleave mode with /K/ comma based byte reordering
		 *          using comma replacement.
		 *      1 - interleave mode with /A/ alignment symbol based byte
		 *          reordering.
		 *   0: ILV_MODE_ENA
		 *      0 - normal XAUI mode
		 *      1 - interleave mode
		 */
		if (!strcmp(str, "dune") || !strcmp(str, "broadcom")) {
			value &= 0xfff0;
			value |= 3;
		} else if (!strcmp(str, "marvell")) {
			value &= 0xfff0;
			value |= 0xf;
		} else {
			printf("Unknown RXAUI mode %s for %s\n",
			       str, ethdev->name);
		}
		phy_write(phydev, 4, 0xf007, value);
	}
#ifdef CONFIG_PHY_SFP
	octeon_sfp_register_mod_abs_changed(ethdev,
					    &vsc8490_mod_abs_changed, NULL);
#endif
	return 0;
}

/**
<<<<<<< .mine
 * Configures the Vitesse VSC8488 for use on boards like the SNIC10xxx based
=======
 * Configures the Vitesse vsc8488 for use on boards like the SNIC10xxx based
>>>>>>> .r146036
 * boards using the values in the flat device tree.
 *
 * @param ethdev - ethernet device to initialize PHY for
 *
 * @returns 0 for success, error otherwise
 */
int octeon_fdt_vitesse_config(struct eth_device *ethdev)
	__attribute__((weak, alias("__octeon_fdt_vitesse_config")));
int __octeon_fdt_vitesse_config(struct eth_device *ethdev)
{
	const void *fdt = gd->fdt_blob;
	struct phy_device *phydev;
	int phy_node_offset;
	uint32_t devid = MDIO_DEVAD_NONE;
	uint32_t reg, mask;
	int value;
	int c45 = 0;	/* True if clause-45 mode */
	int len;
	uint32_t *init_seq;
	int seq_len;
	int rc;
	const struct octeon_eth_info *oct_eth_info =
					(struct octeon_eth_info *)ethdev->priv;

	debug("%s(%s)\n", __func__, ethdev->name);

	phydev = oct_eth_info->phydev;

	if (!phydev) {
		printf("%s: Error: could not get phy device for %s\n",
		       __func__, ethdev->name);
		return -1;
	}

	phy_node_offset = oct_eth_info->phy_fdt_offset;
	if (phy_node_offset < 0) {
		printf("%s: Error: could not find phy info for phy device\n",
		       __func__);
		return -1;
	}
	if (!octeon_fdt_node_check_compatible(fdt, phy_node_offset,
					      compat848x_list)) {
		rc = octeon_vitesse_vsc8488_config(ethdev);
		if (rc) {
			printf("%s: Error configuring vsc8488 for %s\n",
			__func__, ethdev->name);
			return rc;
		}
		c45 = 1;
        } else if (!octeon_fdt_node_check_compatible(fdt, phy_node_offset,
						     compat855x_list)) {
		rc = octeon_fdt_vitesse_vsc8574_config(ethdev);
		if (rc) {
			printf("%s: Error configuring vsc857x for %s\n",
			       __func__, ethdev->name);
			return rc;
		}
	} else if (!octeon_fdt_node_check_compatible(fdt, phy_node_offset,
						     compat866x_list)) {
		rc = octeon_vitesse_vsc866x_init(ethdev, phy_node_offset);
		if (rc) {
			printf("%s: Error configuring vsc866x for %s\n",
			       __func__, ethdev->name);
			return rc;
		}
	} else if (!fdt_node_check_compatible(fdt, phy_node_offset,
					      "vitesse,vsc8490") ||
		   !fdt_node_check_compatible(fdt, phy_node_offset,
					      "vitesse,vsc8489")) {
		/* The VSC8490 uses a different format for the initialization
		 * sequence so we handle it separately.
		 */
		return octeon_fdt_vitesse_vsc8490_config(ethdev);
	}

	/* Perform any initialization sequence specified in the device tree */
	init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_node_offset,
					   "vitesse,c45-reg-init", &len);
	if (init_seq)
		c45 = 1;
	else
		init_seq = (uint32_t *)fdt_getprop(gd->fdt_blob,
						   phy_node_offset,
						   "vitesse,reg-init", &len);

	if (!init_seq)
		return 0;

	if ((c45 && (len % 16 != 0)) || ((!c45 && len % 12 != 0))) {
		printf("Invalid init sequence in DT for Vitesse PHY for %s, c45: %d, len: %d\n",
		       ethdev->name, c45, len);
		return -1;
	}

	seq_len = len / (c45 ? 16 : 12);
	while (seq_len--) {
		devid = (c45) ? fdt32_to_cpu(*init_seq++) : MDIO_DEVAD_NONE;
		reg = fdt32_to_cpu(*init_seq++);
		mask = fdt32_to_cpu(*init_seq++);

		if (mask) {
			value = phy_read(oct_eth_info->phydev, devid, reg);
			if (value < 0) {
				printf("%s: Error reading from phy for %s at %d:%d\n",
				       __func__, ethdev->name, devid, reg);
				return -1;
			}
			debug("%s: Read: 0x%x, mask: 0x%x\n", __func__,
			      value, mask);
			value &= mask;
			value |= fdt32_to_cpu(*init_seq++);
		} else {
			value = fdt32_to_cpu(*init_seq++);
		}
		debug("Vitesse init for %s addr: %d: dev: %d, reg: 0x%x, mask: 0x%x, value: 0x%x\n",
		      ethdev->name, oct_eth_info->phydev->addr, devid,
		      reg, mask, value);

		if (phy_write(oct_eth_info->phydev, devid, reg, value) < 0) {
			printf("%s: Error writing 0x%x to phy for %s at %d:%d\n",
			       __func__, value, ethdev->name, devid, reg);
			return -1;
		}

	}

	return 0;
}

#if defined(CONFIG_PHYLIB_10G) && defined(CONFIG_PHY_SFP)

#define VITESSE_FUNC_MODE_LIMITING	2	/* Optical */
#define VITESSE_FUNC_MODE_COPPER	3	/* Copper */
#define VITESSE_FUNC_MODE_LINEAR	4
#define VITESSE_FUNC_MODE_KR		5
#define VITESSE_FUNC_MODE_ZR		7
#define VITESSE_FUNC_MODE_1G		8

#ifdef DEBUG
/**
 * This reads a global variable as described in the VSC848X EDC Firmware
 * Configuration Application Note.
 *
 * @param phydev[in] - phy device to read from
 * @param channel - channel to read
 * @param addr - global variable address
 *
 * @returns 0 for success, -1 on error.
 */
static int vitesse_read_global_var(struct phy_device *phydev, uint8_t channel,
				   uint8_t addr)
{
	int timeout = 10000;
	uint16_t val;
	do {
		val = phy_read(phydev, 0x1e, 0x7fe3);
		if (val == 0)
			break;
		udelay(1);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("%s: Timeout waiting to read global\n", __func__);
		return -1;
	}
	phy_write(phydev, 0x1e, 0x7fe3, 0x4000 | ((channel & 3) << 8) | addr);
	timeout = 10000;
	do {
		val = phy_read(phydev, 0x1e, 0x7fe3);
		if (val == 0)
			break;
		udelay(1);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("%s: Timeout waiting to read global\n", __func__);
		return -1;
	}
	val = phy_read(phydev, 0x1e, 0x7fe4);
	debug("%s: Read 0x%04x from channel %d, address 0x%x\n", __func__, val,
	      channel, addr);
	return val;
}
#endif

/**
 * This writes a global variable as described in the VSC848X EDC Firmware
 * Configuration Application Note.
 *
 * @param phydev[in] - phy device to write to
 * @param channel - channel to write
 * @param addr - global variable address
 * @param value - value to write
 *
 * @returns 0 for success, -1 on error.
 */
static int vitesse_write_global_var(struct phy_device *phydev, uint8_t channel,
				    uint8_t addr, uint16_t value)
{
	int timeout = 100000;
	uint16_t val;

	/* 1. Read control register until 0 */
	do {
		val = phy_read(phydev, 0x1e, 0x7fe3);
		if (val == 0)
			break;
		udelay(100);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("%s: Timeout waiting to read global 1, val=0x%x\n",
		       __func__, val);
		return -1;
	}
	/* 2. Write data register */
	phy_write(phydev, 0x1e, 0x7fe4, value);
	/* 3. Write control register */
	phy_write(phydev, 0x1e, 0x7fe3,
		  0x8000 | ((channel & 3) << 8) | addr);
	debug("%s: Wrote 0x%04x to channel %d, address 0x%x\n", __func__, value,
	      channel, addr);
	/* 4. Wait until control reg cleared */
	timeout = 100000;
	do {
		val = phy_read(phydev, 0x1e, 0x7fe3);
		if (val == 0)
			break;
		udelay(100);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("%s: Timeout waiting to read global 2, val=0x%x\n",
		       __func__, val);
		return -1;
	}
	return 0;
}

/**
 * This function configures the Vitesse PHY based on information read from the
 * SFP+ module SFF EEPROM
 *
 * @param ethdev - ethernet device to configure
 *
 * @returns 0 for success, error otherwise
 */
int octeon_vitesse_vsc8488_sfp_config(struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info;
	uint32_t *sfp_handle;
	uint32_t phandle;
	const uint32_t *value;
	int phy_offset;
	int sfp_offset;
	int i2c_bus, i2c_addr;
	int mode = VITESSE_FUNC_MODE_LIMITING;	/* Default for optical */
	const char *mode_str = "Unknown";
	struct phy_device *phydev;
	int timeout;
	uint16_t val;
	uint8_t addr_mask = 0;
	int rc;
	struct phy_sfp_info *sfp_info = NULL;

	oct_eth_info = (struct octeon_eth_info *)ethdev->priv;

	phydev = oct_eth_info->phydev;

	debug("%s(%s): 10g: %d, interface: %d\n", __func__, ethdev->name,
	      is_10g_interface(oct_eth_info->phydev->interface),
	      oct_eth_info->phydev->interface);
	if (!is_10g_interface(oct_eth_info->phydev->interface))
		return 0;

	/* This is a lengthy process to parse and validate the device tree.
	 * The SPD EEPROM address is linked from the PHY.  We must obtain both
	 * the TWSI bus number and the address (which should be 0x50) from the
	 * device tree before we can actually read it.
	 */
	phy_offset = oct_eth_info->phy_fdt_offset;

	sfp_handle = (uint32_t *)fdt_getprop(gd->fdt_blob, phy_offset,
					     "sfp-eeprom", NULL);
	if (!sfp_handle) {
		printf("Error: cannot find SFP for PHY on %s\n", ethdev->name);
		return -1;
	}

	phandle = fdt32_to_cpu(*sfp_handle);
	sfp_offset = fdt_node_offset_by_phandle(gd->fdt_blob, phandle);
	if (sfp_offset < 0) {
		printf("Error: cannot find SFP for PHY on %s\n", ethdev->name);
		return -1;
	}

	if (fdt_node_check_compatible(gd->fdt_blob, sfp_offset, "atmel,24c01")) {
		printf("Error: Unknown SFP EEPROM type for %s\n", ethdev->name);
		return -1;
	}

	i2c_bus = octeon_fdt_i2c_get_bus(gd->fdt_blob, sfp_offset);
	i2c_addr = fdtdec_get_int(gd->fdt_blob, sfp_offset, "reg", -1);

	if (i2c_bus < 0 || i2c_addr < 0) {
		printf("Error: Unknown register address for TWSI bus for SPD for %s\n",
		       ethdev->name);
		return -1;
	}

	if (octeon_sfp_check_mod_abs(ethdev, NULL)) {
		debug("%s(%s): Module is absent\n", __func__, ethdev->name);
		mode = VITESSE_FUNC_MODE_COPPER;
		mode_str = "Copper Pigtail (assumed)";
	} else {
		rc = octeon_sfp_read_eeprom(ethdev);
		if (rc) {
			printf("%s: Error reading SFP eeprom for %s\n",
			       __func__, ethdev->name);
			mode = VITESSE_FUNC_MODE_COPPER;
			mode_str = "Copper Pigtail (assumed)";
		} else {
			sfp_info = &oct_eth_info->sfp.sfp_info;
			mode = sfp_info->limiting ?
						VITESSE_FUNC_MODE_LIMITING :
						VITESSE_FUNC_MODE_COPPER;
			mode_str = sfp_info->limiting ?
				"Optical/Active Copper" : "Passive Copper";
		}
	}


	debug("%s: Setting PHY to mode %d (%s) on %s\n",
	      __func__, mode, mode_str, ethdev->name);

	/* Wait for firmware download to complete */
	timeout = 100000;
	do {
		/* Check the checksum result register */
		val = phy_read(phydev, 0x1e, 0x7fe0);
		if (val == 3)
			break;
		udelay(100);
	} while (timeout-- > 0);
	if (timeout <= 0) {
		printf("Error waiting for PHY firmware download to complete on %s, reg: 0x%04x\n",
		       ethdev->name, val);
		return -1;
	}

	/* Program the mode into the PHY */
	if (!fdt_node_check_compatible(gd->fdt_blob, phy_offset,
				       "vitesse,vsc8488"))
		addr_mask = 1;
	if (!fdt_node_check_compatible(gd->fdt_blob, phy_offset,
				       "vitesse,vsc8484"))
		addr_mask = 3;

	if (mode == VITESSE_FUNC_MODE_COPPER) {
		/* DC_AGC_LOS_CONFIG1:
		 *   15: Suppress_Coarse_Adj_on_LOS_Clear
		 *	0: DC offset correction performed using coarse
		 *	   resolution mode (default)
		 *	1: DC offset correction performed using fine resolution
		 *	   mode when correction resumes after LOPC/LOS alarms
		 *	   clear.  This guarantees there will be no big jumps in
		 *	   the offset at the expense of taking longer to reach
		 *	   optimal setting.
		 *   14: Force_DC2_Fine_Adj:
		 *	Forces the DC offset correction to operate in fine
		 *	resolution adjustment mode at times when the algorithm.
		 *	0: DC offset correction makes coarse adjustments when
		 *	   correction mode is first enabled (default)
		 *	1: DC offset correction performed using fine resolution
		 *	   at all times.  This is slower.
		 *   13: Force_DC1_Fine_Adj:
		 *	Forces the DC offset correction to operate in fine
		 *	resolution adjustment mode at times when the algorithm.
		 *	0: DC offset correction makes coarse adjustments when
		 *	   correction mode is first enabled (default)
		 *	1: DC offset correction performed using fine resolution
		 *	   at all times.  This is slower.
		 *   12: Force_DC0_Fine_Adj:
		 *	Forces the DC offset correction to operate in fine
		 *	resolution adjustment mode at times when the algorithm.
		 *	0: DC offset correction makes coarse adjustments when
		 *	   correction mode is first enabled (default)
		 *	1: DC offset correction performed using fine resolution
		 *	   at all times.  This is slower.
		 *   10: Skip_DC2_Adj, 1 = skip DC2 offset correction
		 *   9:  Skip_DC1_Adj, 1 = skip DC1 offset correction
		 *   8:  Skip_DC0_Adj, 1 = skip DC0 offset correction
		 *
		 *   6-4: DC_Offset_Alarm_Mode (default 1)
		 *	Selects the alarm condition that will halt the DC offset
		 *	correction logic when the alarm(s) are set.
		 *	111: reserved
		 *	110: reserved
		 *	101: LOPC and software LOS detection
		 *	100: LOPC and hardware LOS detection
		 *	011: Software LOS detection
		 *	010: Hardware LOS detection
		 *	001: LOPC
		 *	000: Never.  DC offset correction will continue to make
		 *	     fine resolution adjustments to the offsets even
		 *	     when LOPC and LOS alarms are present.
		 *
		 *  3: AGC_Enable
		 *	Selects when hardware AGC adjustment logic and LOS
		 *	detection logic is enabled (default 1)
		 *	0: disabled
		 *	1: enabled
		 *  2: AGC_Suspend
		 *	Suspends the LOS detection logic and AGC logic
		 *	from making adjustments to the gain.  Bit valid only
		 *	if AGC_Enable=1
		 *	0: AGC adjustment enabled (default)
		 *	1: AGC adjustment suspended
		 *  1: DC_Offset_Adj_Enable
		 *	Select when the hardware DC offset correction logic is
		 *	enabled.
		 *	0: disable
		 *	1: enable (default)
		 *  0: DC_Offset_Adj_Suspend
		 *	Suspends the DC offset correction logic from making
		 *	adjustments to all offset settings.  Bit valid only if
		 *	DC_Offset_Adj_Enable=1
		 *	0: DC offset correction enabled (default)
		 *	1: DC offset correction suspended
		 */
		value = fdt_getprop(gd->fdt_blob, phy_offset,
				    "vitesse,copper_dc_agc_los_config1", NULL);
		if (value) {
			debug("%s: Writing 0x%x to DC_AGC_LOS CONFIG1\n",
			      __func__, fdt32_to_cpu(*value));
			phy_write(phydev, 1, 0xB0C0, fdt32_to_cpu(*value));
		}

		value = fdt_getprop(gd->fdt_blob, phy_offset,
				    "vitesse,copper_agg_track_phase", NULL);
		if (value && !(*value)) {
			/* This setting disables the aggressive track phase for
			 * firmware convergence.  The aggressive track phase
			 * takes place in state 4 after selecting the best tap
			 * (which takes 5 seconds).  This state normally remains
			 * active for 60 seconds.
			 */
			vitesse_write_global_var(phydev,
						 phydev->addr & addr_mask,
						 0xAF, 0xFFFF);
		}

		/* This global variable is not documented */
		vitesse_write_global_var(phydev, phydev->addr & addr_mask,
					 0x83, 0x0A);
		/* AGC_Config4
		 *
		 * 13-8: Ampl_Tolerance
		 *	This defines the hysterisis built in to the AGC
		 *	adjustment circuit.  The VGA gain will not be adjusted
		 *	as long as the measured input amplitude is
		 *	Inp_Ampl_Target +/- Amnpl_Tolerance.  Default is 4.
		 * 7-0: Inp_Ampl_Target
		 *	This is the target amplitude desired to be measured at
		 *	the peak detector when measuring input amplitude.  The
		 *	VGA gain is adjusted to achieve this target setting.
		 *	Default is 0x6E.
		 */
		value = fdt_getprop(gd->fdt_blob, phy_offset,
				    "vitesse,copper_agc_config4", NULL);
		if (value) {
			debug("%s: Writing 0x%x to AGC_Config4\n",
			      __func__, fdt32_to_cpu(*value));
			phy_write(phydev, 1, 0xb0c5, fdt32_to_cpu(*value));
		}
	}
	vitesse_write_global_var(phydev, phydev->addr & addr_mask, 0x94, mode);

	/* Adjust PMA_TXOUTCTRL2 based on cable length for copper */
	if (mode == VITESSE_FUNC_MODE_COPPER) {
		if (sfp_info && sfp_info->max_copper_cable_len >= 5) {
			debug("%s(%s): Optimize for 5M or longer cables\n",
			      __func__, phydev->dev->name);
			/* Optimize for 5M and longer cables */
			/* PMA_TXOUTCTRL2
			 *	PMA Tx Output Driver Control 2
			 *  12-8: C(0) Tap Control
			 *	Tx Output Driver C(0) Coefficient Control
			 *	00000: Minimum (0mA)
			 *	11111: Maximum (+16mA)
			 *	10011: default
			 *  5-0: C(+1) Tap Control
			 *	000000: Minimum (-0.25mA) (default)
			 *	111111: Maximum (-16mA)
			 */
			phy_write(phydev, 1, 0x8014, 0x1606);
		}
		if (0) {/* Disable for now, causes link flapping */
			/* Limit the EDC taps */
			vitesse_write_global_var(phydev,
						 phydev->addr & addr_mask,
						 0x52, 0x7a0);
			vitesse_write_global_var(phydev,
						 phydev->addr & addr_mask,
						 0x54, 0x600);
		}
	}

	/* Reset the state machine */
	phy_write(phydev, 1, 0x8036, 0x11);


#ifdef DEBUG
	for (i = 0; i < 4; i++)
		vitesse_read_global_var(phydev, i, 0x94);
#endif

	return 0;
}

/**
 * This function configures the Vitesse PHY based on information read from the
 * SFP+ module SFF EEPROM
 *
 * @param ethdev - ethernet device to configure
 *
 * @returns 0 for success, error otherwise
 */
int octeon_vitesse_sfp_config(struct eth_device *ethdev)
{
	struct octeon_eth_info *oct_eth_info = ethdev->priv;
	int fdt_offset = oct_eth_info->phy_fdt_offset;

	if (!octeon_fdt_node_check_compatible(gd->fdt_blob, fdt_offset,
					      compat848x_list))
		return octeon_vitesse_vsc8488_sfp_config(ethdev);

	return 0;
}

/**
 * This function tunes the QLM for use by the Vitesse VSC8488 PHY.
 *
 * @param qlm - qlm number to tune
 */
void octeon_board_vsc8488_qlm_tune(int qlm)
{
	if (!OCTEON_IS_MODEL(OCTEON_CN66XX) &&
	    !OCTEON_IS_MODEL(OCTEON_CN61XX) &&
	    !OCTEON_IS_MODEL(OCTEON_CN68XX)) {
		printf("%s: Not supported on this OCTEON model yet\n", __func__);
		return;
	}
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_hf_ls_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_hf_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_lf_ls_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "tcoeff_lf_byp", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_eq_gen1", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_eq_gen2", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_cap_gen1", 0);
	cvmx_qlm_jtag_set(qlm, -1, "rx_cap_gen2", 0);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_hs_ls_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_hf_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_lf_ls_byp", 31);
	cvmx_qlm_jtag_set(qlm, -1, "biasdrv_lf_byp", 31);
	/* Assert serdes_tx_byp to force the new settings to override the
	 * QLM default.
	 */
	cvmx_qlm_jtag_set(qlm, -1, "serdes_tx_byp", 1);
}
#endif /* CONFIG_PHYLIB_10G */

#ifdef CONFIG_PHY_SFP
/**
 * Given an Ethernet MAC node in the device tree, read the SFP EEPROM
 *
 * @param[in]	fdt		Pointer to FDT blob
 * @param	eth_node	Offset of Ethernet MAC entry in device tree
 * @param[out]	mod_abs		Set to true if module is absent
 * @param[out]	sfp_info	SFP information parsed from EEPROM
 *
 * @return	0 for success, -1 on error
 *
 * NOTE:	This only works if the SFP EEPROM is on the Octeon I2C bus
 *		and the mod_abs pin is not directly connected to a phy.
 *		The EEPROM can sit behind an i2c mux or switch.  Also, the
 *		GPIO can be an Octeon GPIO or an i2c GPIO expander.
 */
int octeon_phy_get_sfp_info_from_fdt(const void *fdt, int eth_node,
				     bool *mod_abs,
				     struct phy_sfp_info *sfp_info)
{
	static const char *prop_name = "cavium,sfp-slot-eeprom";
	uint8_t buffer[256];
	bool mod_abs_active_low = false;
	int mod_abs_pin = -1;
	int sfp_node;
	const int *phandle;
	int sfp_i2c_bus, sfp_i2c_addr;
	uint32_t mod_abs_prop[3];
	int rc;

	phandle = fdt_getprop((void *)fdt, eth_node, prop_name, NULL);
	if (!phandle) {
		printf("Error: eth node missing %s\n", prop_name);
		return -1;
	}
	sfp_node = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*phandle));
	if (sfp_node < 0) {
		printf("%s: Invalid phandle\n", __func__);
		return -1;
	}
	rc = octeon_fdt_get_i2c_bus_addr(fdt, sfp_node,
					 &sfp_i2c_bus, &sfp_i2c_addr);
	if (rc) {
		printf("%s: Could not get SFP EEPROM i2c bus and/or address\n",
		       __func__);
		return -1;
	}
	if (fdtdec_get_int_array(fdt, eth_node, "cavium,mod_abs",
				 mod_abs_prop, 3)) {
		debug("%s: No mod-abs property\n", __func__);
	 } else {
		 mod_abs_pin = fdt32_to_cpu(mod_abs_prop[1]);
		 mod_abs_active_low = !!mod_abs_prop[2];
		 rc = octeon_fdt_read_gpio(fdt, fdt32_to_cpu(mod_abs_prop[0]),
					   mod_abs_pin);
		 debug("%s: module absent pin: %d\n", __func__, rc);
		 if (rc < 0) {
			 printf("%s: Error reading mod_abs pin %d\n", __func__,
				mod_abs_pin);
			 return -1;
		 }
		 *mod_abs = ((!!rc) ^ mod_abs_active_low);
		 if (*mod_abs) {
			 debug("%s: module absent\n", __func__);
			 return 0;
		 }
	 }

	 rc = phy_sfp_read_i2c_eeprom(buffer, sfp_i2c_bus, sfp_i2c_addr);
	 if (rc) {
		 printf("%s: Error reading SFP EEPROM at 0x%x:0x%x\n", __func__,
		        sfp_i2c_bus, sfp_i2c_addr);
		 return -1;
	 }
	 return phy_sfp_parse_eeprom(sfp_info, buffer);
}
#endif

#ifdef CONFIG_OCTEON_VITESSE_VSC7224

struct vsc7224_tapinfo {
	int length;
	int maintap;
	int pretap;
	int posttap;
};

/**
 * For each channel read the SFP module and program the transmit taps
 * appropriately based on the module type and cable length.
 *
 * @param[in]	fdt	Pointer to flat device tree
 * @param	node	node of vsc7224 in device tree (not channel nodes)
 * @param	bus	i2c bus number of vsc7224
 * @param	addr	i2c address of vsc7224
 *
 * @return	0 for success, -1 if error
 */
static int octeon_vitesse_vsc7224_channel(const void *fdt, int node,
					  int bus, int addr)
{
	struct phy_sfp_info *sfp_info;
	struct eth_device *dev;
	struct octeon_eth_info *ethinfo;
	struct vsc7224_tapinfo *tapinfo, *tap;
	int cnode = node;	/** Channel node offset */
	int channel;		/** Channel number */
	int length;
	int mac_node;
	int sfp_node;
	int cable_length;
	int i;
	int old_bus;
	int value;
	uint8_t reg_buf[2];
	bool is_tx;
	bool is_copper;
	bool pretap_dis;
	bool maintap_dis;
	bool posttap_dis;
	bool is_qsfp = false;
	int rc;
	int index, interface;
	uint32_t *bgx_addr;

	while ((cnode = fdt_node_offset_by_compatible(fdt, cnode,
						      "vitesse,vsc7224-channel")) &&
	       cnode >= 0 && fdt_parent_offset(fdt, cnode) == node) {
		debug("%s: node: 0x%x, cnode: 0x%x, parent cnode: 0x%x\n",
		      __func__, node, cnode, fdt_parent_offset(fdt, cnode));
		channel = fdtdec_get_int(fdt, cnode, "reg", -1);
		debug("%s: Processing channel %d\n", __func__, channel);
		if (channel < 0) {
			printf("%s: Error: vsc7224 channel reg in device tree is not set or invalid\n",
			       __func__);
			return -1;
		}
		is_tx = fdtdec_get_bool(fdt, cnode, "direction-tx");
		debug("%s: channel %d is%s tx\n", __func__, channel,
		      is_tx ? "" : " not");

		pretap_dis = fdtdec_get_bool(fdt, cnode, "pretap-disable");
		maintap_dis = fdtdec_get_bool(fdt, cnode, "maintap-disable");
		posttap_dis = fdtdec_get_bool(fdt, cnode, "posttap-disable");

		mac_node = fdtdec_lookup_phandle(fdt, cnode, "sfp-mac");
		if (mac_node < 0) {
			mac_node = fdtdec_lookup_phandle(fdt, cnode, "qsfp-mac");
			is_qsfp = mac_node > 0;
		}
		if (mac_node < 0) {
			printf("%s: Error: mac phandle not specified for channel %d in device tree\n",
			       __func__, channel);
			return -1;
		}
		debug("%s: mac node: 0x%x, is %ssfp\n", __func__,
		      mac_node, is_qsfp ? "q" : "");
		sfp_node = fdtdec_lookup_phandle(fdt, mac_node, "sfp-slot");
		if (sfp_node < 0) {
			debug("%s: sfp slot not found, looking for qsfp\n",
			      __func__);
			sfp_node = fdtdec_lookup_phandle(fdt, mac_node,
							 "qsfp-slot");
			if (sfp_node > 0)
				is_qsfp = true;
		}

		index = fdtdec_get_int(fdt, mac_node, "reg", -1);
		bgx_addr = fdt_getprop((void *)fdt,
				       fdt_parent_offset((void *)fdt, mac_node),
				       "reg", NULL);
		if (!bgx_addr) {
			printf("%s: Can't get parent address of node\n",
			       __func__);
			return -1;
		}
		interface = (fdt32_to_cpu(bgx_addr[1]) >> 24) & 0xf;
		debug("%s: Looking for interface 0x%x, index: %d\n",
		      __func__, interface, index);
		i = 0;
		do {
			debug("%s: Getting eth device at index %d\n",
			      __func__, i);
			dev = eth_get_dev_by_index(i++);
			if (!dev) {
				printf("%s: No Ethernet device found in device tree\n",
				       __func__);
				return -1;
			}
			ethinfo = dev->priv;
			debug("%s: Checking if %s fdt offset 0x%x == 0x%x\n",
			      __func__, dev->name, ethinfo->fdt_offset, mac_node);
		} while ((ethinfo->interface != interface) ||
			 (ethinfo->index != index));
		debug("%s: Reading SFP EEPROM for %s\n", __func__, dev->name);
		rc = octeon_sfp_read_eeprom(dev);
		if (rc) {
			debug("%s: Could not read SFP/QSFP EEPROM for %s\n",
			      __func__, dev->name);
			is_copper = false;
			cable_length = 0;
		} else {
			sfp_info = octeon_sfp_get_eeprom_info(dev);
			if (!sfp_info) {
				debug("%s: No valid SFP data for %s\n",
				      __func__, dev->name);
				is_copper = false;
				cable_length = 0;
			} else {
				is_copper = !sfp_info->limiting;
				if (is_copper)
					cable_length = sfp_info->max_copper_cable_len;
				else
					cable_length = 0;
				debug ("%s: Is%s copper cable with a length of %d meters\n",
				       __func__, is_copper ? "" : " not", cable_length);
			}
		}
		debug("%s: getting taps\n", __func__);
		tapinfo = (struct vsc7224_tapinfo *)
				fdt_getprop(fdt, cnode, "taps", &length);
		if (length % sizeof(*tapinfo)) {
			printf("%s: Invalid taps size in device tree for channel %d\n",
			       __func__, channel);
		}
		tap = NULL;
		for (i = 0; i < length / sizeof(*tapinfo); i++) {
			if (cable_length >= fdt32_to_cpu(tapinfo[i].length))
				tap = &tapinfo[i];
		}
		if (!tap) {
			printf("%s: No compatible tap found for cable length %d on channel %d\n",
			       __func__, cable_length, channel);
			return -1;
		}
		debug("%s: Found matching tap for cable length %d\n"
		      " length: %d, main tap: 0x%x, pre-tap: 0x%x, post-tap: 0x%x\n",
		      __func__, cable_length, fdt32_to_cpu(tap->length),
		      fdt32_to_cpu(tap->maintap), fdt32_to_cpu(tap->pretap),
		      fdt32_to_cpu(tap->posttap));
		old_bus = i2c_get_bus_num();
		i2c_set_bus_num(bus);
		reg_buf[0] = 0;
		reg_buf[1] = channel;
		i2c_write(addr, 0x7f, 1, reg_buf, 2);

		if (!maintap_dis) {
			value = fdt32_to_cpu(tap->maintap);
			reg_buf[0] = (value >> 8) & 0xff;
			reg_buf[1] = value & 0xff;
			i2c_write(addr, 0x99, 1, reg_buf, 2);
		}

		if (!pretap_dis) {
			value = fdt32_to_cpu(tap->pretap);
			reg_buf[0] = (value >> 8) & 0xff;
			reg_buf[1] = value & 0xff;
			i2c_write(addr, 0x9a, 1, reg_buf, 2);
		}

		if (!posttap_dis) {
			value = fdt32_to_cpu(tap->posttap);
			reg_buf[0] = (value >> 8) & 0xff;
			reg_buf[1] = value & 0xff;
			i2c_write(addr, 0x9b, 1, reg_buf, 2);
		}

		if (maintap_dis || pretap_dis || posttap_dis) {
			debug("%s: pretap disable: %d, maintap disable: %d, posttap disable: %d\n",
			      __func__, pretap_dis, maintap_dis, posttap_dis);
			i2c_read(addr, 0x97, 1, reg_buf, 2);
			if (maintap_dis)
				reg_buf[0] |= 8;	/* Set bit 11 */
			if (pretap_dis)
				reg_buf[0] |= 0x10;	/* Set bit 12 */
			if (posttap_dis)
				reg_buf[0] |= 4;	/* Set bit 10 */
			debug("%s: writing 0x%02x%02x to OUTDRVCTL (0%d_97)\n",
			      __func__, reg_buf[0], reg_buf[1], channel);
			i2c_write(addr, 0x97, 1, reg_buf, 2);
		}

		i2c_set_bus_num(old_bus);
	}
	return 0;
}

/**
 * Initializes a Vitesse VSC7224 reclocking chip
 *
 * @param[in] fdt	Pointer to flat device tree
 *
 * @return 0 for success, -1 on error
 */
int octeon_vitesse_init_vsc7224(const void *fdt)
{
	int node = -1;
	int addr;
	int bus;
	int reg;
	uint16_t value;
	int len;
	const int *init_array;
	int i;
	int reset_phandle;
	int reset_pin = -1;
	uint32_t *reset_data;
	bool reset_inverted = false;

	while ((node = fdt_node_offset_by_compatible(fdt, node, "vitesse,vsc7224")) > 0) {
                debug("%s: Found VSC7224 at node offset %d\n", __func__, node);
		bus = octeon_fdt_i2c_get_bus(fdt, node);
		if (bus < 0) {
			printf("%s: I2C bus not found in device tree\n",
			       __func__);
			return -1;
		}
		addr = fdtdec_get_int(fdt, node, "reg", -1);
		if (addr < 0) {
			printf("%s: Error: reg field missing in device tree",
			       __func__);
			return -1;
		}
		debug("%s: Found node in device tree at address 0x%x:0x%x\n",
		      __func__, bus, addr);

		reset_data = fdt_getprop((void *)fdt, node, "reset", &len);
		if (len == 12 && reset_data) {
			reset_phandle = fdt32_to_cpu(reset_data[0]);
			if (reset_phandle < 0) {
				printf("%s: Error: could not get GPIO phandle\n",
				       __func__);
				return -1;
			}
			reset_pin = fdt32_to_cpu(reset_data[1]);
			reset_inverted = !!reset_data[2];
		} else if (len == 8 && reset_data) {
			reset_phandle = fdtdec_get_int(fdt, node,
						       "reset-parent",
						       -FDT_ERR_NOTFOUND);
			if (reset_phandle < 0) {
				printf("%S: Reset GPIO parent not found in device tree\n",
				       __func__);
				return -1;
			}
			reset_pin = fdt32_to_cpu(reset_data[0]);
			reset_inverted = !!reset_data[1];
		} else if (len == 4 && reset_data) {
			reset_phandle = fdtdec_get_int(fdt, node,
						       "reset-parent",
						       -FDT_ERR_NOTFOUND);
			if (reset_phandle < 0) {
				printf("%S: Reset GPIO parent not found in device tree\n",
				       __func__);
				return -1;
			}
			reset_pin = fdt32_to_cpu(reset_data[0]);
			reset_inverted = false;
		} else if (reset_data) {
			printf("%s: Invalid length %d of reset field\n",
			       __func__, len);
			return -1;
		}
		if (reset_pin >= 0) {
			debug("%s: Putting device in reset\n", __func__);
			octeon_fdt_set_gpio(fdt, reset_phandle, reset_pin,
					    reset_inverted ? 0 : 1);
			mdelay(10);
			debug("%s: Taking device out of reset\n", __func__);
			octeon_fdt_set_gpio(fdt, reset_phandle, reset_pin,
					    reset_inverted ? 1 : 0);
			mdelay(50);
		} else {
			debug("%s: Reset not defined\n", __func__);
		}
		init_array = fdt_getprop((void *)fdt, node,
					 "vitesse,reg-init", &len);
		if (!init_array) {
			debug("%s: No initialization array\n", __func__);
			continue;
		}
		if ((len % 8) != 0) {
			printf("%s: Error: register init string should be an array of reg number followed by value\n",
			       __func__);
			return -1;
		}
		i2c_set_bus_num(bus);
		for (i = 0; i < len / sizeof(int); i += 2) {
                        uint8_t buffer[2];
			reg = fdt32_to_cpu(init_array[i]);
			value = fdt32_to_cpu(init_array[i+1]);
                        buffer[0] = value >> 8;
                        buffer[1] = value & 0xff;
                        i2c_write(addr, reg, 1, buffer, 2);
			debug("  Wrote 0x%02x <= 0x%02x%02x\n", reg,
                              buffer[0], buffer[1]);
		}
		if (octeon_vitesse_vsc7224_channel(fdt, node, bus, addr)) {
			printf("%s: Error processing vsc7224 channels for device at 0x%x:0x%x\n",
			       __func__, bus, addr);
			return -1;
		}
	}
	return 0;
}
#endif /* CONFIG_OCTEON_VITESSE_VSC7224 */

#if defined(DEBUG)
#include <command.h>

/**
 * Enable and disable various loopback options for the Vitesse VSC8490 PHY
 */
int do_vsc8490_loopback(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct phy_device *phydev;
	struct eth_device *ethdev;
	struct octeon_eth_info *ethinfo;
	bool enable;
	uint16_t val;

	if (argc < 4) {
		printf("Wrong number of arguments (%d)\n", argc);
		return CMD_RET_USAGE;
	}

	if (argv[1][0] == 'e')
		enable = true;
	else if (argv[1][0] == 'd')
		enable = false;
	else {
		puts("First argument must be enable or disable\n");
		return CMD_RET_USAGE;
	}

	ethdev = eth_get_dev_by_name(argv[2]);
	if (!ethdev) {
		printf("Could not find %s\n", argv[2]);
		return -1;
	}
	ethinfo = ethdev->priv;
	phydev = ethinfo->phydev;
	if (!phydev) {
		printf("No phy associated with %s\n", ethdev->name);
		return -1;
	}

	if (!strcmp(argv[3], "l1")) {
		val = phy_read(phydev, 4, 0xe62a);
		val |= 1 << 11;
		phy_write(phydev, 4, 0xe62a, val);
		printf("Enabled L1 loopback for %s\n", ethdev->name);
	} else if (!strcmp(argv[3], "l2")) {
		val = phy_read(phydev, 4, 0xea20);
		if (enable)
			val |= 1;
		else
			val &= ~1;
		phy_write(phydev, 4, 0xea20, val);
	} else if (!strcmp(argv[3], "l3")) {
		if (enable) {
			phy_write(phydev, 1, 0xa003, 1);
			phy_write(phydev, 1, 0xb700, phydev->addr & 1);
			phy_write(phydev, 1, 0xf2ab, 0x2f);
			phy_write(phydev, 1, 0xf2aa, 0);
			phy_write(phydev, 1, 0xf283, 0x121);
			phy_write(phydev, 1, 0xf282, 0x208);
			phy_write(phydev, 1, 0xf2a3, 0x121);
			phy_write(phydev, 1, 0xf2a2, 0x208);
			phy_write(phydev, 1, 0xf285, 0x7c1);
			phy_write(phydev, 1, 0xf284, 0x362a);
			phy_write(phydev, 1, 0xf2a1, 0x1fe);
			phy_write(phydev, 1, 0xf2a0, 0xd07);
			phy_write(phydev, 1, 0xf285, 0xfc1);
			phy_write(phydev, 1, 0xf284, 0x362a);
		} else {
			phy_write(phydev, 1, 0xa003, 0);
			phy_write(phydev, 1, 0xb700, phydev->addr & 1);
			phy_write(phydev, 1, 0xf2ab, 0x2f);
			phy_write(phydev, 1, 0xf2aa, 0);
			phy_write(phydev, 1, 0xf283, 0x121);
			phy_write(phydev, 1, 0xf282, 0x208);
			phy_write(phydev, 1, 0xf2a3, 0x121);
			phy_write(phydev, 1, 0xf2a2, 0x208);
			phy_write(phydev, 1, 0xf285, 0x7c1);
			phy_write(phydev, 1, 0xf284, 0x362a);
			phy_write(phydev, 1, 0xf2a1, 0x1fe);
			phy_write(phydev, 1, 0xf2a0, 0xd07);
			phy_write(phydev, 1, 0xf285, 0xfc1);
			phy_write(phydev, 1, 0xf284, 0x362a);
		}
	} else if (!strcmp(argv[3], "h2")) {
		val = phy_read(phydev, 4, 0xe62a);
		if (enable)
			val |= (1 << 10);
		else
			val &= ~(1 << 10);
		phy_write(phydev, 4, 0xe62a, val);
		val = phy_read(phydev, 4, 0xe628);
		if (enable)
			val &= ~(1 << 4);
		else
			val |= (1 << 4);
		phy_write(phydev, 4, 0xe628, val);
	} else if (!strcmp(argv[3], "h3")) {
		val = phy_read(phydev, 3, 0);
		if (enable)
			val |= (1 << 14);
		else
			val &= ~(1 << 14);
		phy_write(phydev, 3, 0, val);
	} else if (!strcmp(argv[3], "h4")) {
		val = phy_read(phydev, 2, 0);
		if (enable)
			val |= (1 << 14);
		else
			val &= ~(1 << 14);
		phy_write(phydev, 2, 0, val);
	} else {
		printf("Unknown loopback %s for %s\n", argv[3], ethdev->name);
		return -1;
	}
	printf("%sabled %s loopback for %s\n",
	       enable ? "En" : "Dis", argv[3], ethdev->name);

	return 0;
}

U_BOOT_CMD(vsc8490loop, 4, 0, do_vsc8490_loopback,
	   "Vitesse VSC8490 loopback",
	   "enable <eth name> <level>        enable loopback\n"
	   "vsc8490loop disable <eth name> <level>            disable loopback\n"
	   "<level> may be l1, l2, l3, h2, h3 or h4\n");

# ifdef CONFIG_OCTEON_VITESSE_VSC7224

static int vsc7224_read_reg(int bus, int addr, uint8_t reg)
{
	uint16_t val;
	int rc;

	i2c_set_bus_num(bus);
	rc = i2c_read(addr, reg, 1, (uint8_t *)&val, sizeof(val));
	if (rc)
		return -1;
	else
		return be16_to_cpu(val);
}

static int vsc7224_write_reg(int bus, int addr, uint8_t reg, uint16_t value)
{
	int rc;
	value = cpu_to_be16(value);

	i2c_set_bus_num(bus);
	rc = i2c_write(addr, reg, 1, (uint8_t *)&value, sizeof(value));
	return rc ? -1 : 0;
}

static int vsc7224_read_count(int bus, int addr, uint32_t *value)
{
	int val;

	i2c_set_bus_num(bus);
	val = vsc7224_read_reg(bus, addr, 0xc1);
	if (val < 0)
		return -1;
	*value = val << 16;
	val = vsc7224_read_reg(bus, addr, 0xc2);
	if (val < 0)
		return -1;
	*value |= val;

	return 0;
}

static int vsc7224_vscope(int bus, int addr, uint8_t i_chan,
			  uint8_t i_vscope_insel, uint8_t i_count_preload,
			  uint8_t i_start_phase, uint8_t i_end_phase,
			  uint8_t i_phase_hop, uint8_t i_voltage_width,
			  uint8_t i_voltage_gap)
{
	int rc;
	uint32_t errcnt;
	uint16_t ctrl;
	uint16_t time;
	uint16_t volt;
	uint16_t volt_lower, volt_upper;
	uint16_t voltage;
	uint16_t vscopepd;
	uint16_t vscope_done_mask;
	uint8_t timeout_cnt;
	uint8_t vscope_flags;

	vscope_flags = 0xc0;
	vscope_done_mask = 0x0080;

	ctrl = 0x4000 | i_count_preload | 0x0078 | i_vscope_insel;
	vscopepd = vsc7224_read_reg(bus, addr, 0x83);
	/* pd_vscope = 0, rst_n = 0 = assert reset */
	vsc7224_write_reg(bus, addr, 0x83, ((vscopepd & 0xfffc) | 0x0000));
	/* rst_n = 1 = deassert reset */
	vsc7224_write_reg(bus, addr, 0x83, ((vscopepd & 0xfffc) | 0x0002));
	/* control with ena = 0 */
	vsc7224_write_reg(bus, addr, 0x82, ctrl);
	/* upper plane */
	volt = 0x7f;

	/* scan from upper boundary to 0v (signed mag) */
	while ((volt - i_voltage_width) < 0x80) {
		volt_lower = volt - i_voltage_width;
		volt_upper = volt << 8;
		voltage = volt_upper | volt_lower;
		printf("\r\n%04X ", voltage);
		/* scan through time from start to end [0:255] */
		for (time = i_start_phase; time <= i_end_phase;
		     time += i_phase_hop) {
			/* Set 2 voltages */
			vsc7224_write_reg(bus, addr, 0x80, voltage);
			/* Set time */
			vsc7224_write_reg(bus, addr, 0x81, time);
			/* control, ena = 1 to start counting (1) */
			vsc7224_write_reg(bus, addr, 0x82, ctrl | 0x0002);
			udelay(100);
			timeout_cnt = 9;
			/* Wait for DONE bit or timeout */
			while (((vsc7224_read_reg(bus, addr, vscope_flags) &
				vscope_done_mask) == 0x0000) &&
			       (timeout_cnt > 0))
				timeout_cnt--;
			/* If timed out print out bogus value */
			if (timeout_cnt == 0) {
				printf("%08X  ", 0xFFFFFFFF);
			} else {
				/* Read error count */
				rc = vsc7224_read_count(bus, addr, &errcnt);
				if (rc)
					return -1;
				printf("%08X  ", errcnt);
			}
			/* control, ena = 0 */
			vsc7224_write_reg(bus, addr, 0x82, ctrl);
			if (ctrlc())
				return -1;
		}
		/* Decrement voltage position */
		volt = (volt - (1 + i_voltage_gap));
	}

	/* Lower plane */
	volt = 0x0080;
	/* Scan from 0V to lower boundary (signed mag) */
	while ((volt + i_voltage_width) < 0x100) {
		volt_lower = volt + i_voltage_width;
		volt_upper = volt << 8;
		voltage = volt_upper | volt_lower;
		printf("\r\n%04X ", voltage);
		for (time = i_start_phase; time <= i_end_phase;
		     time += i_phase_hop) {
			vsc7224_write_reg(bus, addr, 0x80, voltage);
			vsc7224_write_reg(bus, addr, 0x81, time);
			/* control, ena = 1 to start counting */
			vsc7224_write_reg(bus, addr, 0x82, ctrl | 0x0002);
			timeout_cnt = 9;
			while (((vsc7224_read_reg(bus, addr, vscope_flags) &
				 vscope_done_mask) == 0x0000) &&
				(timeout_cnt > 0))
				timeout_cnt--;
			/* If timed out print out bogus value */
			if (timeout_cnt == 0) {
				printf("%08X  ", 0xFFFFFFFF);
			} else {
				/* Read error count */
				rc = vsc7224_read_count(bus, addr, &errcnt);
				if (rc)
					return -1;
				printf("%08X  ", errcnt);
			}
			/* control, ena = 0 */
			vsc7224_write_reg(bus, addr, 0x82, ctrl);
			if (ctrlc())
				return -1;
		}
		volt = volt + (1 + i_voltage_gap);
	}
	puts("\n");
	return 0;
}


int vsc7224_vscope_plot(int bus, int addr, int channel)
{
	uint8_t i;
	uint8_t i_chan = channel;
	uint8_t i_vscope_insel = 0;	/* Sample VScope eye after DFE block */
	uint8_t i_count_preload = 30;
	uint8_t i_start_phase[] = {0, 4, 2, 6, 1, 3, 5, 7};
	uint8_t i_end_phase = 255;
	uint8_t i_phase_hop = 8;
	uint8_t i_voltage_width = 1;
	uint8_t i_voltage_gap = 1;
	uint16_t value;

	/* Goto proper channel of interest */
	vsc7224_write_reg(bus, addr, 0x7f, 0x20 + i_chan);

	value = vsc7224_read_reg(bus, addr, 0x83);
	value |= 0x100;
	vsc7224_write_reg(bus, addr, 0x83, value);
	for (i = 0; i < 8; i++)
		vsc7224_vscope(bus, addr, i_chan, i_vscope_insel,
			       i_count_preload, i_start_phase[i], i_end_phase,
			       i_phase_hop, i_voltage_width, i_voltage_gap);
	value = vsc7224_read_reg(bus, addr, 0x83);
	value &= 0xfffc;
	value |= 0x0001;
	vsc7224_write_reg(bus, addr, 0x83, value);
	return 0;
}
int do_vsc7224(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint8_t bus = 0;
	uint8_t addr = 0x10;
	uint8_t reg;
	uint16_t val;
	int rc;
	int old_bus = i2c_get_bus_num();
	int i, j;
	const uint8_t dump_regs[] = {
		0x82, 0x83, 0x87, 0x88, 0xae, 0xc0, 0xc1, 0xc4, 0xc8,
		0x8b, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0xc9,
		0xca, 0xcc, 0xcd, 0xdb };

	if (argc < 3)
		goto usage;
	if (argv[1][0] == 'r') {
		if (argc < 5)
			return CMD_RET_USAGE;
		bus = simple_strtoul(argv[2], NULL, 0);
		addr = simple_strtoul(argv[3], NULL, 16);
		reg = simple_strtoul(argv[4], 0, 16);
		i2c_set_bus_num(bus);
		rc = i2c_read(addr, reg, 1, (uint8_t *)&val, 2);
		if (!rc)
			printf("Reg: 0x%02x <= 0x%04x\n", reg, val);
		else
			printf("Error reading bus %d, address 0x%x, register 0x%x\n",
			       bus, addr, reg);
	} else if (argv[1][0] == 'w') {
		if (argc < 6)
			goto usage;
		bus = simple_strtoul(argv[2], NULL, 0);
		addr = simple_strtoul(argv[3], NULL, 16);
		reg = simple_strtoul(argv[4], 0, 16);
		val = simple_strtoul(argv[5], 0, 16);
		i2c_set_bus_num(bus);
		rc = i2c_write(addr, reg, 1, (uint8_t *)&val, 2);
		if (rc)
			printf("Error writing bus %d, address 0x%x, register 0x%x, value 0x%x\n",
			       bus, addr, reg, val);
	} else if (argv[1][0] == 'd') {
		bus = simple_strtoul(argv[2], NULL, 0);
		addr = simple_strtoul(argv[3], NULL, 16);
		i2c_set_bus_num(bus);
		printf("Debug dump for device at %d:%02x:\n", bus, addr);
		for (i = 0; i < 4; i++) {
			val = i;
			i2c_write(addr, 0x7f, 1, (uint8_t  *)&val, 2);
			for (j = 0; j < ARRAY_SIZE(dump_regs); j++) {
				i2c_read(addr, dump_regs[j], 1,
					 (uint8_t *)&val, 2);
				printf("    0%d_%02x = 0x%04x\n",
				       i, dump_regs[j], val);
			}
		}
		val = 0x40;
		i2c_write(addr, 0x7f, 1, (uint8_t *)&val, 2);
		i2c_read(addr, 0xc0, 1, (uint8_t *)&val, 2);
		printf("    40_c0 = 0x%04x\n", val);
	} else if (argv[1][0] == 'v') {
		int channel;
		bus = simple_strtoul(argv[2], NULL, 0);
		addr = simple_strtoul(argv[3], NULL, 16);
		channel = simple_strtoul(argv[4], NULL, 0);
		vsc7224_vscope_plot(bus, addr, channel);
	} else {
		goto usage;
	}
	i2c_set_bus_num(old_bus);
	return CMD_RET_SUCCESS;
usage:

	i2c_set_bus_num(old_bus);
	return CMD_RET_USAGE;
}

U_BOOT_CMD(vsc7224, 6, 0, do_vsc7224,
	   "Vitesse VSC7224 Register Support",
	   "read [bus] [address] [register]   Read register\n"
	   "vsc7224 write [bus] [address] [register] [value]  Write register\n"
	   "vsc7224 dump [bus] [addr] dump a number of registers for debugging\n"
	   "vsc7224 vscope [bus] [addr] [channel] Run VScope on the specified channel\n");

# endif /* CONFIG_OCTEON_VITESSE_VSC7224 */
#endif /* DEBUG */
