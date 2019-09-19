/*
 * (C) Copyright 2014 Cavium Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <environment.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <fdtdec.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx-helper-util.h>
#include <asm/arch/cvmx-helper-bgx.h>
#include <pca9698.h>
#include <pca953x.h>
#include <pca9555.h>
#include <i2c.h>
#include <net.h>
#include <asm/gpio.h>
#include <phy.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/octeon_sfp.h>
#include <cortina.h>

DECLARE_GLOBAL_DATA_PTR;

#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA	0x108F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB	0x1090

#define TEMP_1_MIN		48
#define	TEMP_1_MAX		70
#define TEMP_1_THERM		80
#define TEMP_LOCAL_MIN		48
#define	TEMP_LOCAL_MAX		70
#define TEMP_LOCAL_THERM	80
#define TEMP_2_MIN		48
#define TEMP_2_MAX		70
#define TEMP_2_THERM		80
#define FAN_1_MIN_DUTY_CYCLE	0x40
#define FAN_2_MIN_DUTY_CYCLE	0x40
#define FAN_3_MIN_DUTY_CYCLE	0x40

#define FAN_BUS				0	/** ADT7476A I2C bus */
#define FAN_ADDR			0x2e	/** ADT7476A I2C address */

#define ADT7476_CONFIG6				0x10
#define ADT7476_CONFIG7				0x11
#define ADT7476_REMOTE1_TEMP			0x25
#define ADT7476_LOCAL_TEMP			0x26
#define ADT7476_REMOTE2_TEMP			0x27
#define ADT7476_TACH1_LO			0x28
#define ADT7476_TACH1_HI			0x29
#define ADT7476_TACH2_LO			0x2A
#define ADT7476_TACH2_HI			0x2B
#define ADT7476_TACH3_LO			0x2C
#define ADT7476_TACH3_HI			0x2D
#define ADT7476_PWM1_CURRENT_DUTY_CYCLE		0x30
#define ADT7476_PWM2_CURRENT_DUTY_CYCLE		0x31
#define ADT7476_PWM3_CURRENT_DUTY_CYCLE		0x32
#define ADT7476_PWM1_MAX_DUTY_CYCLE		0x38
#define ADT7476_PWM2_MAX_DUTY_CYCLE		0x39
#define ADT7476_PWM3_MAX_DUTY_CYCLE		0x3A
#define ADT7476_CONFIG1				0x40
#define ADT7476_INTERRUPT_STATUS1		0x41
#define ADT7476_INTERRUPT_STATUS2		0x42
#define ADT7476_VID_GPIO			0x43
#define ADT7476_REMOTE1_TEMP_LOW_LIM		0x4E
#define ADT7476_REMOTE1_TEMP_HI_LIM		0x4F
#define ADT7476_LOCAL_TEMP_LOW_LIM		0x50
#define ADT7476_LOCAL_TEMP_HI_LIM		0x51
#define ADT7476_REMOTE2_TEMP_LOW_LIM		0x52
#define ADT7476_REMOTE2_TEMP_HI_LIM		0x53
#define ADT7476_PWM1_CONFIG			0x5C
#define ADT7476_PWM2_CONFIG			0x5D
#define ADT7476_PWM3_CONFIG			0x5E
#define ADT7476_REMOTE1_TRANGE_PWM1_FREQ	0x5F
#define ADT7476_LOCAL_TRANGE_PWM2_FREQ		0x60
#define ADT7476_REMOTE2_TRANGE_PWM3_FREQ	0x61
#define ADT7476_ENHANCE_ACOUSTICS_REG1		0x62
#define ADT7476_ENHANCE_ACOUSTICS_REG2		0x63
#define ADT7476_PWM1_MIN_DUTY_CYCLE		0x64
#define ADT7476_PWM2_MIN_DUTY_CYCLE		0x65
#define ADT7476_PWM3_MIN_DUTY_CYCLE		0x66
#define ADT7476_REMOTE1_TEMP_TMIN		0x67
#define ADT7476_LOCAL_TEMP_TMIN			0x68
#define ADT7476_REMOTE2_TEMP_TMIN		0x69
#define ADT7476_REMOTE1_THERM_LIM		0x6A
#define ADT7476_LOCAL_THERM_LIM			0x6B
#define ADT7476_REMOTE2_THERM_LIM		0x6C
#define ADT7476_REMOTE1_LOCAL_HYSTERISIS	0x6D
#define ADT7476 REMOTE2_TEMP_HYSTERISIS		0x6E
#define ADT7476_XNOR_TREE_TEST_ENABLE		0x6F
#define ADT7476_REMOTE1_TEMP_OFFSET		0x70
#define ADT7476_LOCAL_TEMP_OFFSET		0x71
#define ADT7476_REMOTE2_TEMP_OFFSET		0x72
#define ADT7476_CONFIG2				0x73
/** Extended resolution */
#define ADT7476_EXT_RES				0x77
#define ADT7476_CONFIG3				0x78
#define ADT7476_CONFIG5				0x7C
#define ADT7476_CONFIG4				0x7D

struct sfp_adapter_entry {
	uint8_t oui[3];
	uint8_t vendor_pn[17];
};

static const struct sfp_adapter_entry sfp_no_autoneg[] = {
	{
		.oui = {0x00, 0x17, 0x6a},
		.vendor_pn = "ABCU-5700RZ",
	},
	{

	}
};

static void init_fan_controller(void)
{
	ulong start = get_timer(0);
	int value;

	i2c_set_bus_num(FAN_BUS);

	do {
		value = i2c_reg_read(FAN_ADDR, ADT7476_CONFIG1);
		if (value < 0) {
			puts("Error reading ADT7476 fan controller!\n");
			return;
		}
	} while ((!(value & 4)) && (get_timer(start) < 100));
	if (!(value & 4))
		puts("Error: ADT7476 is not fully powered up and ready to begin system monitoring\n");

	/* Make GPIOs GPIO pins */
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG5, 0x13);
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG4, 0x07);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_TEMP_LOW_LIM, TEMP_1_MIN);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_TEMP_HI_LIM, TEMP_1_MAX);
	i2c_reg_write(FAN_ADDR, ADT7476_LOCAL_TEMP_LOW_LIM, TEMP_LOCAL_MIN);
	i2c_reg_write(FAN_ADDR, ADT7476_LOCAL_TEMP_HI_LIM, TEMP_LOCAL_MAX);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_TEMP_LOW_LIM, TEMP_2_MIN);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_TEMP_HI_LIM, TEMP_2_MAX);
	i2c_reg_write(FAN_ADDR, ADT7476_PWM1_MIN_DUTY_CYCLE,
		      FAN_1_MIN_DUTY_CYCLE);
	i2c_reg_write(FAN_ADDR, ADT7476_PWM2_MIN_DUTY_CYCLE,
		      FAN_2_MIN_DUTY_CYCLE);
	i2c_reg_write(FAN_ADDR, ADT7476_PWM3_MIN_DUTY_CYCLE,
		      FAN_3_MIN_DUTY_CYCLE);
	/* Configure the PWM to a 250ms startup and use the fastest speed
	 * calculated by all three temperature channels
	 */
	i2c_reg_write(FAN_ADDR, ADT7476_PWM1_CONFIG, 0xC2);
	i2c_reg_write(FAN_ADDR, ADT7476_PWM2_CONFIG, 0xC2);
	i2c_reg_write(FAN_ADDR, ADT7476_PWM3_CONFIG, 0xC2);
	/* Use high frequency mode and a 32C temperature range */
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_TRANGE_PWM1_FREQ, 0xbc);
	i2c_reg_write(FAN_ADDR, ADT7476_LOCAL_TRANGE_PWM2_FREQ, 0xbc);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_TRANGE_PWM3_FREQ, 0xbc);
	/* Don't turn the fans off and turn on smoothing, 4.7s ramp time */
	i2c_reg_write(FAN_ADDR, ADT7476_ENHANCE_ACOUSTICS_REG1, 0xec);
	i2c_reg_write(FAN_ADDR, ADT7476_ENHANCE_ACOUSTICS_REG2, 0xcc);

	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_THERM_LIM, TEMP_1_THERM);
	i2c_reg_write(FAN_ADDR, ADT7476_LOCAL_THERM_LIM, TEMP_LOCAL_THERM);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_THERM_LIM, TEMP_2_THERM);
	/* Enable fan presence detect */
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG2, 1);
	/* Turn on fan LEDs */
	i2c_reg_write(FAN_ADDR, ADT7476_VID_GPIO, 0x98);
	/* Set Tmin */
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG6, 0x00);
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG7, 0x00);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_TEMP_TMIN, TEMP_1_MIN);
	i2c_reg_write(FAN_ADDR, ADT7476_LOCAL_TEMP_TMIN, TEMP_LOCAL_MIN);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_TEMP_TMIN, TEMP_2_MIN);
	/* For some reason both remote temperatures are offset 64 */
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE1_TEMP_OFFSET, 0xc0);
	i2c_reg_write(FAN_ADDR, ADT7476_REMOTE2_TEMP_OFFSET, 0xc0);
	/* Start the fan controller.  Note that we need to delay at least
	 * 146.5ms
	 */
	i2c_reg_write(FAN_ADDR, ADT7476_CONFIG1, 0x05);

	mdelay(250);
}

static int fan_get_rpm(int fan)
{
	static ulong last_read_time[3] = { 0, 0, 0 };
	ulong time;
	int value;

	if (fan > 2)
		return -1;

	time = get_timer(last_read_time[fan]);
	if (last_read_time[fan] == 0)
		mdelay(200);
	else if (time < 150)
		mdelay(150 - time);
	value = i2c_reg_read(FAN_ADDR, ADT7476_TACH1_LO + fan * 2);
	value |= (i2c_reg_read(FAN_ADDR, ADT7476_TACH1_HI + fan * 2) << 8);
	last_read_time[fan] = get_timer(0);

	if (value <= 0)
		return -1;
	return (90000 * 60 / 2) / value;
}

static int fan_get_temp(int temp)
{
	static ulong last_read_time[3] = { 0, 0, 0 };
	ulong time;
	int value;
	static const uint8_t regs[] = {
		ADT7476_REMOTE1_TEMP, ADT7476_LOCAL_TEMP, ADT7476_REMOTE2_TEMP
	};

	if (temp > 2) {
		printf("%s: Error: temp sensor %d out of range\n",
		       __func__, temp);
		return -1;
	}

	time = get_timer(last_read_time[temp]);
	if (last_read_time[temp] == 0)
		mdelay(200);
	else if (time < 150)
		mdelay(150 - time);

	value = i2c_reg_read(FAN_ADDR, regs[temp]);
	debug("%s(%d): Read 0x%x from register 0x%x\n", __func__, temp,
	      value, regs[temp]);

	last_read_time[temp] = get_timer(0);

	return value;
}

/**
 * Given the port offset in the FDT return the BGX number
 *
 * @param blob		Pointer to flat device tree
 * @param port_offset	node offset of port in device tree
 *
 * @return BGX number or error if negative.
 */
static int get_bgx_slot(const void *blob, int port_offset)
{
	int offset = fdt_parent_offset(blob, port_offset);
	uint32_t regs[4];

	if (offset < 0)
		return offset;
	if (fdt_node_check_compatible(blob, offset, "cavium,octeon-7890-bgx")) {
		printf("%s: Error in device tree, Ethernet MAC not compatible\n",
		       __func__);
		return -1;
	}
	if (fdtdec_get_int_array(blob, offset, "reg", regs, ARRAY_SIZE(regs))) {
		printf("%s: Error getting register address\n", __func__);
		return -1;
	}
	if ((regs[1] & 0xf8ffffff) != 0xe0000000) {
		printf("%s: Invalid register address 0x%x\n", __func__, regs[1]);
		return -1;
	}
	return (regs[1] >> 24) & 7;
}

/**
 * Modify the device tree to remove unused interfaces when in XLAUI mode
 */
int board_fixup_fdt(void)
{
	char fdt_key[16];
	enum cvmx_qlm_mode mode;
	int i;
	int offset;
	int rc;
	int bgx;
	int qlm;

	for (i = 6; i <= 7; i++) {
		mode = cvmx_qlm_get_mode_cn78xx(0, i);
		switch (mode) {
		case CVMX_QLM_MODE_SGMII:
			snprintf(fdt_key, sizeof(fdt_key), "%d,sgmii", i);
			break;
		case CVMX_QLM_MODE_XFI:
			snprintf(fdt_key, sizeof(fdt_key), "%d,xfi", i);
			break;
		case CVMX_QLM_MODE_XLAUI:
			snprintf(fdt_key, sizeof(fdt_key), "%d,xlaui", i);
			break;
		default:
			snprintf(fdt_key, sizeof(fdt_key), "%d,none", i);
			break;
		}
		octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL,
					true, NULL, NULL);
	}
	/* Revision 1 board has no UART 0 */
	octeon_fdt_patch_rename((void *)gd->fdt_blob,
				gd->arch.board_desc.rev_major == 1 ?
			        "0,rev1" : "0,notrev1", "cavium,board-trim",
				false, NULL, NULL);

	/* Use 1000Base-X mode for gigabit, remove this for 10 and 40G */
	offset = -1;
	do {
		offset = fdt_node_offset_by_compatible(gd->fdt_blob, offset,
						       "cavium,octeon-7890-bgx-port");
		if (offset < 0)
			break;

		bgx = get_bgx_slot(gd->fdt_blob, offset);
		/* Map BGX to QLM to get mode */
		switch (bgx) {
		case 4:
			qlm = 6;
			break;
		case 5:
			qlm = 7;
			break;
		default:
			qlm = -1;
			break;
		}
		if (qlm == -1)
			continue;

		mode = cvmx_qlm_get_mode_cn78xx(0, qlm);
		if (mode != CVMX_QLM_MODE_SGMII && mode != CVMX_QLM_MODE_XLAUI) {
			debug("%s: Clearing MAC 1000X mode for BGX: %d, QLM: %d\n",
			      __func__, bgx, qlm);
			rc = fdt_nop_property((void *)gd->fdt_blob, offset,
					      "cavium,sgmii-mac-1000x-mode");
			if (rc) {
				printf("%s: Error nooping cavium,disable-autonegotiation for BGX: %d, QLM: %d\n",
				       __func__, bgx, qlm);
				return -1;
			}
		} else {
			debug("%s: Skipping nooping 1000Base-X mode for BGX: %d, QLM: %d\n",
			      __func__, bgx, qlm);
		}
	} while (offset > 0);

	return 0;
}

/**
 * This function is called to initialize the QLMs
 */
void board_configure_qlms(void)
{
	const char *speed;

	octeon_init_qlm(0);

	/* Here is the description of the parameters that are passed to QLM
	 * configuration:
	 *	param0 : The Octeon OCX node number
	 *	param1 : The QLM to configure
	 *	param2 : Speed to configure the QLM at
	 *	param3 : Mode the QLM to configure
	 *	param4 : 1 = RC, 0 = EP (for PCIe)
	 *	param5 : 0 = GEN1, 1 = GEN2, 2 = GEN3 (for PCIe)
	 *	param6 : ref clock select, 0 = 100Mhz, 1 = 125MHz, 2 = 156.25MHz
	 *	param7 : ref clock input to use:
	 *		 0 - external reference (QLMx_REF_CLK)
	 *		 1 = common clock 0 (QLMC_REF_CLK0)
	 *		 2 = common_clock 1 (QLMC_REF_CLK1)
	 */
	octeon_configure_qlm_cn78xx(0, 0, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 0);
	octeon_configure_qlm_cn78xx(0, 1, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);
	octeon_configure_qlm_cn78xx(0, 2, 0, CVMX_QLM_MODE_PCIE, 1, 2, 0, 0);
	octeon_configure_qlm_cn78xx(0, 3, 0, CVMX_QLM_MODE_PCIE, 1, 2, 0, 0);
	octeon_configure_qlm_cn78xx(0, 4, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);
	octeon_configure_qlm_cn78xx(0, 5, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);

	speed = getenv("sfp_left");
	if (speed && (!strcmp(speed, "1g") || !strcmp(speed, "1G") ||
		      !strcmp(speed, "1000") || !strcmp(speed, "sgmii"))) {
		octeon_configure_qlm_cn78xx(0, 6, 1250, CVMX_QLM_MODE_SGMII,
					    0, 0, 2, 0);
		puts("Left ports configured for 1Gbps SFP\n");
	} else if (speed && (!strcmp(speed, "40g") || !strcmp(speed, "40G") ||
			     !strcmp(speed, "40000") ||
			     !strcmp(speed, "xlaui"))) {
		octeon_configure_qlm_cn78xx(0, 6, 103125, CVMX_QLM_MODE_XLAUI,
					    0, 0, 2, 0);
		octeon_qlm_tune_v3(0, 6, 103125, 0xa, 0xa0, -1, -1);
		puts("Left ports configured for 40Gbps SFP+ XLAUI mode\n");
		puts("Make sure cables 1, 2, 3 and 4 and plugged into ports 0, 1, 2 and 3\n");
	} else {
		octeon_configure_qlm_cn78xx(0, 6, 103125, CVMX_QLM_MODE_XFI,
					    0, 0, 2, 0);
		octeon_qlm_tune_v3(0, 6, 103125, 0xa, 0xa0, -1, -1);
		puts("Left ports configured for 10Gbps SFP+\n");
	}
	speed = getenv("sfp_right");
	if (speed && (!strcmp(speed, "1g") || !strcmp(speed, "1G") ||
		      !strcmp(speed, "1000") || !strcmp(speed, "sgmii"))) {
		octeon_configure_qlm_cn78xx(0, 7, 1250, CVMX_QLM_MODE_SGMII,
					    0, 0, 2, 0);
		puts("Right ports configured for 1Gbps SFP\n");
	} else if (speed && (!strcmp(speed, "40g") || !strcmp(speed, "40G") ||
			     !strcmp(speed, "40000") ||
			     !strcmp(speed, "xlaui"))) {
		octeon_configure_qlm_cn78xx(0, 7, 103125, CVMX_QLM_MODE_XLAUI,
					    0, 0, 2, 0);
		octeon_qlm_tune_v3(0, 7, 103125, 0xa, 0xa0, -1, -1);
		puts("Right ports configured for 40Gbps SFP+ XLAUI mode\n"
		     "Make sure cables 1, 2, 3 and 4 are plugged into ports 4, 5, 6 and 7\n");
	} else {
		octeon_configure_qlm_cn78xx(0, 7, 103125, CVMX_QLM_MODE_XFI,
					    0, 0, 2, 0);
		octeon_qlm_tune_v3(0, 7, 103125, 0xa, 0xa0, -1, -1);
		puts("Right ports configured for 10Gbps SFP+\n");
	}
}

/**
 * This function should get called whenever mod_abs changes for a SFP slot.
 *
 * @param[in]	dev	Ethernet device that changed
 * @param	mod_abs	True if module is absent, false if present
 * @param	data	User-defined data structure
 *
 * @return	0 for success, otherwise error
 */
static int rainier_mod_abs_changed(struct eth_device *dev, bool mod_abs,
				   void *data)
{
	struct octeon_eth_info *ethinfo = dev->priv;
	struct phy_device *phydev = ethinfo->phydev;
	char env_name[32];
	bool an;

	debug("%s(%s): module %splugged\n", __func__, dev->name,
	      mod_abs ? "un" : "");
	if (!mod_abs) {
		/* Delay to allow the module to settle */
		mdelay(250);
		if (cvmx_helper_interface_get_mode(ethinfo->interface) ==
		    CVMX_HELPER_INTERFACE_MODE_SGMII) {
			snprintf(env_name, sizeof(env_name), "%s_autoneg",
						  dev->name);
			if (getenv(env_name))
				an = getenv_yesno(env_name);
			else
				an = octeon_check_sfp_autonegotiation(dev);
			debug("%s: autonegotiation %ssupported\n", dev->name,
			      an ? "" : "not ");
			cvmx_helper_set_autonegotiation(ethinfo->interface,
							ethinfo->index, an);
		}

		return cs4224_port_check(phydev);
	}

	return 0;
}

/**
 * This function is called before networking is initialized
 */
void board_net_preinit(void)
{
	int i;
	/* Take phys out of reset */
	gpio_set_value(4, 1);
	gpio_set_value(6, 1);
	mdelay(10);
	/* Configure GPIO pins */
	i2c_set_bus_num(0);
	for (i = 0; i < 8; i++) {
		pca9698_direction_input(0x20, i * 4 + 0);	/* TX fault */
		pca9698_direction_output(0x20, i * 4 + 1, 0);	/* TX enable */
		pca9698_direction_input(0x20, i * 4 + 2);	/* ABS */
		pca9698_direction_input(0x20, i * 4 + 3);	/* LOS */
	}

	/* Set fault LEDs to off */
	for (i = 32; i < 40; i++)
		pca9698_direction_output(0x20, i, 0);
}

/**
 * This is called after the phy initialization code has completed.
 *
 * @param	dev		Ethernet device PHY belongs to
 * @param	fdt_blob	Pointer to FDT blob (gd->fdt_blob)
 * @param	fdt_offset	Offset of phy in the device tree
 *
 * @return	0 for success, -1 on error
 */
int octeon_board_post_phy_init(struct eth_device *dev,
			       const void *fdt_blob, int fdt_offset)
{
	struct octeon_eth_info *oct_eth_info = dev->priv;

	/* We only care about the 10G interfaces */
	if (oct_eth_info->interface == 0)
		return 0;

	debug("%s(%s): Registering port check function\n", __func__, dev->name);
	/* Register the Cortina port check function callback */
	octeon_eth_register_phy_port_check(dev, cs4224_port_check);

	octeon_sfp_register_mod_abs_changed(dev, &rainier_mod_abs_changed, NULL);

	/* If a module is present, might as well check it now */
	if (octeon_sfp_check_mod_abs(dev, NULL) < 1 /*
	    (cvmx_helper_interface_get_mode(oct_eth_info->interface) ==
					CVMX_HELPER_INTERFACE_MODE_XLAUI)*/) {
		debug("%s: Checking %s\n", __func__, dev->name);
		rainier_mod_abs_changed(dev, false, NULL);
	} else {
		debug("%s: Module for %s is absent\n", __func__, dev->name);
	}

	return 0;
}

int checkboard(void)
{
	bool present = !!gpio_get_value(13);
	bool ok;
	int fan_hd_info;
	int ps_info = pca9555_get_val(0, 0x21);
	int rpm;

	printf("Clock coin battery is %s\n",
	       (ps_info & 1) ? "missing or dead" : "good");
	printf("Power supply %d is %sinstalled", 0, present ? "" : "not ");
	ok = gpio_get_value(7);
	if (present) {
		if (!(ps_info & 0x400))
			puts("  ALERT SET");
		if (!(ps_info & 0x1000))
			puts(", input voltage out of range");
		else
			puts(", input voltage normal");
		printf(" and is %sok", ok ? "" : "not ");
	}
	puts("\n");

	present = !!gpio_get_value(14);
	ok = gpio_get_value(9);

	printf("Power supply %d is %sinstalled", 1, present ? "" : "not ");
	if (present) {
		if (!(ps_info & 0x800))
			puts("  ALERT SET");
		if (!(ps_info & 0x2000))
			puts(", input voltage out of range");
		else
			puts(", input voltage normal");
		printf(" and is %sok", ok ? "" : "not ");
	}
	puts("\n");

	if (getenv("quietfan"))
		init_fan_controller();
	fan_hd_info = pca953x_get_val(0, 0x23);
	printf("Fan %d is %sinstalled", 0, fan_hd_info & 1 ? "not " : "");
	rpm = fan_get_rpm(0);
	if (rpm < 0)
		puts(" but not working\n");
	else
		printf(", RPM: %d\n", rpm);
	printf("Fan %d is %sinstalled", 1, fan_hd_info & 2 ? "not " : "");
	rpm = fan_get_rpm(0);
	if (rpm < 0)
		puts(" but not working\n");
	else
		printf(", RPM: %d\n", rpm);
	printf("Fan %d is %sinstalled", 2, fan_hd_info & 4 ? "not " : "");
	rpm = fan_get_rpm(0);
	if (rpm < 0)
		puts(" but not working\n");
	else
		printf(", RPM: %d\n", rpm);
	printf("Fan controller reports temperatures: remote 1: %dC, local: %dC, remote 2: %dC\n",
	       fan_get_temp(0), fan_get_temp(1), fan_get_temp(2));

	printf("Hard drive %d is %sinstalled\n",
	       0, fan_hd_info & 8 ? "not " : "");
	printf("Hard drive %d is %sinstalled\n",
	       1, fan_hd_info & 0x10 ? "not " : "");
	printf("Hard drive %d is %sinstalled\n",
	       2, fan_hd_info & 0x20 ? "not " : "");
	printf("Hard drive %d is %sinstalled\n",
	       3, fan_hd_info & 0x40 ? "not " : "");
	if (!gpio_get_value(11))
		puts("WARNING: temperature is high\n");
	if (!gpio_get_value(12))
		puts("ALERT: temperature is critical\n");
	if (!gpio_get_value(15))
		puts("ALERT: Fan controller reports high temperature\n");

	pca9555_set_val(0, 21, 0x00f0, (fan_hd_info << 1) & 0xf0);

	return 0;
}

int board_cortina_startup(struct phy_device *phydev)
{
	int reg;

	debug("%s: tuning die 1\n", __func__);
	reg = CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA;
	if (phydev->addr & 1) {
		phy_write(phydev, 0, reg + 0x0000, 0x381c);
		phy_write(phydev, 0, reg + 0x1000, 0x381c);
		phy_write(phydev, 0, reg + 0x2000, 0x3819);
		phy_write(phydev, 0, reg + 0x3000, 0x3819);
	} else {
		phy_write(phydev, 0, reg + 0x0000, 0x3819);
		phy_write(phydev, 0, reg + 0x1000, 0x3819);
		phy_write(phydev, 0, reg + 0x2000, 0x3819);
		phy_write(phydev, 0, reg + 0x3000, 0x3819);
	}

	reg = CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB;
	if (phydev->addr & 1) {
		phy_write(phydev, 0, reg + 0x0000, 0x10c);
		phy_write(phydev, 0, reg + 0x1000, 0x108);
		phy_write(phydev, 0, reg + 0x2000, 0x106);
		phy_write(phydev, 0, reg + 0x3000, 0x105);
	} else {
		phy_write(phydev, 0, reg + 0x0000, 0x107);
		phy_write(phydev, 0, reg + 0x1000, 0x107);
		phy_write(phydev, 0, reg + 0x2000, 0x105);
		phy_write(phydev, 0, reg + 0x3000, 0x105);
	}
	return 0;
}

int octeon_eth_board_post_init(struct eth_device *dev)
{
	debug("%s(%s)\n", __func__, dev->name);
	octeon_sfp_register_mod_abs_changed(dev, &rainier_mod_abs_changed, NULL);

	return 0;
}

void octeon_board_poll(void)
{
	int i;
	struct eth_device *dev;

	/* Wait for an interrupt from a module being inserted or pulled out */
	if (gpio_get_value(10))
		return;

	for (i = 2; i < 10; i++) {
		dev = eth_get_dev_by_index(i);
		if (dev)
			octeon_phy_port_check(dev);
	}
}

int board_cs4224_get_mod_abs(struct phy_device *phydev)
{
	struct eth_device *ethdev = phydev->dev;
	struct octeon_eth_info *ethinfo = ethdev->priv;
	/* In XLAUI mode assume port is present */
	if (cvmx_helper_interface_get_mode(ethinfo->interface) ==
					CVMX_HELPER_INTERFACE_MODE_XLAUI)
		return 0;

	return octeon_sfp_check_mod_abs(ethdev, NULL);
}

int board_octeon_eth_check_mod_abs_interrupt(struct eth_device *dev)
{
	struct octeon_eth_info *ethinfo = dev->priv;

	if (ethinfo->interface < 4)
		return 0;

	return !gpio_get_value(10);
}

int board_cs4224_get_ctrl(struct phy_device *phydev,
			  int slice,
			  enum cortina_slice_mode mode,
			  uint16_t *ctrla, uint16_t *ctrlb)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	int addr = phydev->addr;
	static const uint16_t tx_out_ctrla_values[3][2][4] = {
		{	/* SR values */
			{0x3810, 0x3816, 0x3817, 0x3818},	/* addr 0 */
			{0x381c, 0x381c, 0x3819, 0x3819}	/* addr 1 */
		},
		{	/* CX values */
			{0x3819, 0x3819, 0x3819, 0x3819},
			{0x381c, 0x381b, 0x3819, 0x3819}
		},
		{	/* 1000Base-X values */
			{0x3810, 0x3816, 0x3817, 0x3818},
			{0x381c, 0x381c, 0x3819, 0x3819}
		}
	};
	static const uint16_t tx_out_ctrlb_values[3][2][4] = {
		{
			{0x107, 0x106, 0x106, 0x105},
			{0x108, 0x108, 0x106, 0x107}
		},
		{
			{0x107, 0x107, 0x105, 0x105},
			{0x10c, 0x108, 0x106, 0x105}
		},
		{
			{0x107, 0x106, 0x106, 0x105},
			{0x108, 0x108, 0x106, 0x107}
		}
	};

	debug("%s(%s): slice: %d, offset: 0x%x\n", __func__, phydev->dev->name,
	      pinfo->sub_addr, pinfo->slice_offsets[pinfo->sub_addr]);
	if (mode >= 3 || slice >= 4) {
		printf("%s(%s): Mode %d or slice %d out of range\n", __func__,
		       phydev->dev->name, mode, slice);
		return -1;
	}
	debug("%s: before: ctrla: 0x%x, ctrlb: 0x%x\n", __func__,
	      *ctrla, *ctrlb);
	*ctrla = tx_out_ctrla_values[mode][addr & 1][slice];
	*ctrlb = tx_out_ctrlb_values[mode][addr & 1][slice];
	return 0;
}

/**
 * This is called whenever the mode is changed for a SFP+ port
 *
 * @param phydev	phy device of port changed
 * @param old_mode	old mode port was in
 * @param new_mode	new mode port is in
 *
 * @return 0 for success
 */
int board_cs4224_set_mode(struct phy_device *phydev,
			  enum cortina_slice_mode old_mode,
			  enum cortina_slice_mode new_mode)
{
#if 0
	struct cortina_phy_info *pinfo = phydev->priv;
	uint16_t reg;
	uint16_t val;

	static const uint16_t tx_out_ctrla_values[3][2][4] = {
		{	/* SR values */
			{0x3810, 0x3816, 0x3817, 0x3818},	/* addr 0 */
			{0x381c, 0x381c, 0x3819, 0x3819}	/* addr 1 */
		},
		{	/* CX values */
			{0x3819, 0x3819, 0x3819, 0x3819},
			{0x381c, 0x381b, 0x3819, 0x3819}
		},
		{	/* 1000Base-X values */
			{0x3810, 0x3816, 0x3817, 0x3818},
			{0x381c, 0x381c, 0x3819, 0x3819}
		}
	};
	static const uint16_t tx_out_ctrlb_values[3][2][4] = {
		{
			{0x107, 0x106, 0x106, 0x105},
			{0x108, 0x108, 0x106, 0x107}
		},
		{
			{0x107, 0x107, 0x105, 0x105},
			{0x10c, 0x108, 0x106, 0x105}
		},
		{
			{0x107, 0x106, 0x106, 0x105},
			{0x108, 0x108, 0x106, 0x107}
		}
	};

	debug("%s(%s): slice: %d, offset: 0x%x\n", __func__, phydev->dev->name,
	      pinfo->sub_addr, pinfo->slice_offsets[pinfo->sub_addr]);
	if (old_mode == new_mode) {
		debug("%s(%s): mode: %d, no change\n", __func__,
		      phydev->dev->name, new_mode);
		return 0;
	} else {
		debug("%s(%s): Setting mode to %d\n", __func__,
		      phydev->dev->name, new_mode);
	}

	reg = CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA +
				pinfo->slice_offsets[pinfo->sub_addr];
	val = tx_out_ctrla_values[new_mode][phydev->addr & 1][pinfo->sub_addr];
	debug("%s: Writing 0x%x to CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA + 0x%04x\n",
	      __func__, val, pinfo->slice_offsets[pinfo->sub_addr]);
	phy_write(phydev, 0, reg, val);
	reg = CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB +
				pinfo->slice_offsets[pinfo->sub_addr];
	val = tx_out_ctrlb_values[new_mode][phydev->addr & 1][pinfo->sub_addr];
	phy_write(phydev, 0, reg, val);
#endif
	return 0;
}


static void board_init_gpio(void)
{
	/* Configure GPIO pins */
	gpio_direction_input(0);
	gpio_direction_input(1);
	gpio_direction_input(2);
	gpio_direction_input(3);
	gpio_direction_output(4, 0);	/* Reset 10G phy */
	mdelay(10);
	gpio_set_value(4, 1);
	gpio_direction_input(5);
	gpio_direction_output(6, 0);	/* Reset 1G phy */
	gpio_direction_input(7);
	gpio_direction_input(8);
	gpio_direction_input(9);
	gpio_direction_input(10);	/* SFP_IO_INT_N */
	gpio_direction_input(11);
	gpio_direction_input(12);
	gpio_direction_input(13);
	gpio_direction_input(14);
	gpio_direction_input(15);
	gpio_direction_output(16, 0);	/* Reset i2c GPIO */
	mdelay(1);
	gpio_set_value(16, 1);		/* Take out of reset */
	gpio_direction_input(17);
	gpio_direction_input(18);
	gpio_direction_output(19, 1);

	pca9555_set_dir(0, 0x21, 0xffff, 0xff01);
	pca953x_set_dir(0, 0x22, 0xff, 0xf0);
	pca953x_set_dir(0, 0x23, 0xff, 0xff);

	/* Clear HDD fault LED */
	pca9555_set_val(0, 0x21, 0x03f0, 0x00f0);
	/* Enable HDD power pins */
	pca953x_set_val(0, 0x22, 0x0f, 0xf0);
}

static int get_rainier_uart(void)
{
	return (gd->arch.board_desc.rev_major == 1) ? 1 : 0;
}

int board_early_init_f(void)
{
	/* We do this a bit earlier in order to set the console UART properly */
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_RAINIER, 2, 0);
	gd->arch.console_uart = get_rainier_uart();
	return 0;
}

int early_board_init(void)
{
	octeon_board_get_clock_info(RAINIER_DEF_DRAM_FREQ);

	octeon_board_get_mac_addr();

	board_init_gpio();

	return 0;
}

int late_board_init(void)
{
	i2c_set_bus_num(FAN_BUS);
	if (getenv_yesno("quietfan"))
		init_fan_controller();
	else
		i2c_reg_write(FAN_ADDR, ADT7476_CONFIG1, 0x4);

	/* Enable interrupts from the SFP external GPIO controller whenever
	 * mod_abs changes.
	 */
	i2c_set_bus_num(0);
	i2c_reg_write(0x20, 0x20, 0xbb);
	i2c_reg_write(0x20, 0x21, 0xbb);
	i2c_reg_write(0x20, 0x22, 0xbb);
	i2c_reg_write(0x20, 0x23, 0xbb);
	return 0;
}

/**
 * Override the Octeon UART serial init function to not change the console UART
 */
int octeon_serial_init(void)
{
	int uart = getenv_ulong("console_uart", 0, -1);
	int i;

	if (uart >= 0)
		gd->arch.console_uart = uart;

	/* Don't change the console UART */
	for (i = CONFIG_OCTEON_MIN_CONSOLE_UART;
	     i <= CONFIG_OCTEON_MAX_CONSOLE_UART; i++)
	     octeon_uart_setup(i);

	return 0;
}

int on_quietfan(const char *name, const char *value, enum env_op op,
		       int flags)
{
	debug("%s(%s, %s, %d, 0x%x)\n", __func__, name, value, op, flags);
	switch (op) {
	case env_op_create:
	case env_op_overwrite:
		switch (*value) {
		case '1':
		case 'y':
		case 'Y':
		case 't':
		case 'T':
			i2c_set_bus_num(FAN_BUS);
			puts("Quieting down fan\n");
			init_fan_controller();
			break;
		default:
			puts("Restoring fan to default state\n");
			i2c_set_bus_num(FAN_BUS);
			i2c_reg_write(FAN_ADDR, ADT7476_CONFIG1, 0x04);
			break;
		}
		break;
	case env_op_delete:
	default:
		i2c_set_bus_num(FAN_BUS);
		i2c_reg_write(FAN_ADDR, ADT7476_CONFIG1, 0x04);
		break;
	}
	return 0;
}

U_BOOT_ENV_CALLBACK(quietfan, on_quietfan);
