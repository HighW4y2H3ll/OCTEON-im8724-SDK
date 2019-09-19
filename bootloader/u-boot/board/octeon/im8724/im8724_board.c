#include <common.h>
#include <i2c.h>
#include <errno.h>
#include <environment.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-helper-cfg.h>
#include <asm/arch/cvmx-helper-sfp.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx-helper-util.h>
#include <asm/arch/cvmx-helper-bgx.h>
#include <asm/arch-octeon/cvmx-mdio.h>
#include <asm/arch/cvmx-csr-typedefs.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/octeon_sfp.h>
#include <cortina_cs4321.h>
#include <cortina.h>
#include <phy.h>

#include <asm/io.h>

#include "im8724_cpld.h"
#include "im8724_phy.h"
#include "niagara.h"
#include "leds_mgmt.h"

#include "im8724_board.h"

#define DEBUG		1

DECLARE_GLOBAL_DATA_PTR;


extern CVMX_SHARED uint64_t(*cvmx_helper_get_real_78xx_xaui_link_status)(int xiface, int index);
extern CVMX_SHARED int(*cvmx_helper_adjust_bgx_link_state_by_phy_settings)(int xiface,
						int index, cvmx_helper_link_info_t *link_info);

static int cpld_i2c_read(uint8_t sasl, uint8_t bus, uint8_t dev, uint8_t reg);
static int cpld_i2c_write(uint8_t sasl, uint8_t bus, uint8_t dev, uint8_t reg, uint8_t val);
static void reset_unit(void);
void fans_mgmt(void);

static int max_eth_devs = 3;
static int leds_management = -1;
static int fp_ports_num = 4;
static int io_cards_presents = 0;

static int fan_management = FANS_UNINITIALIZED;
static uint8_t cpu_set_temp = 0;
static uint64_t curtime = 0;
static uint32_t def_fan_pwm = DEFAULT_FAN_RPM_VALUE;
static uint8_t max_temp_critical_thresh = MAX_TEMP_CRITICAL_THRESH;

/* Read the cycles register state on cnMIPS Coprocessor 0 (COP0) */
static uint64_t hw_get_cycles(void)
{
	uint64_t result;

	asm volatile (
		"   dmfc0 %[rt], $9, 6"
		: [rt] "=d" (result)
		:
	);
	return (result);
}

static void reset_unit(void)
{
	int regval = cpld_rd(CPLD_ADDR_RST_CTL_0);
	cpld_wr(CPLD_ADDR_RST_CTL_0, regval | CPLD_BIT_CN_CPU);
}

static void reset_device(void)
{
	reset_unit();
	printf("Reset CN78XX!\n");
	cvmx_reset_octeon();
}

static int get_max_cpu_brd_temp(uint8_t *temp)
{
	int cur_bus = i2c_get_bus_num();
	int iret;

	i2c_set_bus_num(1);
	if((iret = i2c_read(MAX6699_DEV_ADDR, 0x1, 1, temp, 1)) < 0)
		iret = i2c_read(MAX6699_DEV_ADDR, 0x1, 1, temp, 1);
	i2c_set_bus_num(cur_bus);

	return iret;
}

void board_net_preinit(void)
{
	uint8_t reg = cpld_rd(CPLD_ADDR_RST_CTL_0) | CPLD_BIT_CS4318;

	cpld_wr(CPLD_ADDR_RST_CTL_0, reg);
	reg &= ~(CPLD_BIT_MGT_PHY | CPLD_BIT_CS4318);
	cpld_wr(CPLD_ADDR_RST_CTL_0, reg);
	mdelay(120);

	/* Enable MDINT interrupt */
	reg = cpld_rd(CPLD_ADDR_INT_EN_0) | CPLD_INT_MGT_PHY;
	cpld_wr(CPLD_ADDR_INT_EN_0, reg);
}

enum cortina_cs4321_host_mode
cortina_cs4321_get_host_mode(const struct phy_device *phydev)
{
	const struct octeon_eth_info *oct_eth_info;
	cvmx_bgxx_cmrx_config_t cmr_config;

	if (!phydev->dev) {
		printf("%s: Error: eth dev unknown\n", __func__);
		return CS4321_HOST_MODE_UNKNOWN;
	}

	oct_eth_info = (struct octeon_eth_info *)phydev->dev->priv;
	if (oct_eth_info->interface != 0 && oct_eth_info->interface != 1) {
		printf("Unknown cs4321 host mode!\n");
		return CS4321_HOST_MODE_UNKNOWN;
	}
	cmr_config.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMRX_CONFIG(oct_eth_info->interface, 0));
	if (cmr_config.s.lmac_type == 0) {
		debug("%s host mode SGMII\n", phydev->dev->name);
		return CS4321_HOST_MODE_SGMII;
	}
	if (cmr_config.s.lmac_type == 2) {
		debug("%s host mode RXAUI\n", phydev->dev->name);
		return CS4321_HOST_MODE_RXAUI;
	}
	printf("%s host mode is unknown!\n", phydev->dev->name);
	return CS4321_HOST_MODE_UNKNOWN;
}

extern int cortina_cs4321_phy_reset(struct phy_device *phydev);
extern int cs4223_reset(struct phy_device *phydev);

int is_sgmii_board_mode(void)
{
	cvmx_bgxx_cmr_global_config_t gconfig;
	cvmx_gserx_phy_ctl_t gserx_phy_ctl;

	gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
	gserx_phy_ctl.u64 = cvmx_read_csr_node(0, CVMX_GSERX_PHY_CTL(3));
	return (gconfig.s.pmux_sds_sel && gserx_phy_ctl.s.phy_pd == 0) ? 1 : 0;
}

int phy_reset(struct phy_device *phydev)
{
	const struct octeon_eth_info *oct_eth_info;

	if (!phydev->dev)
		return 0;
	oct_eth_info = (struct octeon_eth_info *)phydev->dev->priv;
	if (oct_eth_info->interface > 1)
		return cs4223_reset(phydev);
	if (oct_eth_info->interface == 0 || ! is_sgmii_board_mode())
		return cortina_cs4321_phy_reset(phydev);
	return 0;
}

/**
* Modify the device tree according to current board configuration.
*/
int board_fixup_fdt(void)
{
	cvmx_bgxx_cmrx_config_t cmr_config;
	char fdt_key[16], fdt_key2[16];
	enum cvmx_qlm_mode mode;
	uint16_t sig[MAX_IM8724_MODULES] = {0,0};
	int i, pim[MAX_IM8724_MODULES];
	uint8_t sasl;

	debug("%s ENTER\n", __func__);
	for (i = 0; i < MAX_IM8724_MODULES; i++) {
		pim[i] = CPLD_PIM_MISSING(i) ? 1 : 0;
		if (pim[i]) continue;
		sasl = i ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
		sig[i] = cpld_rd_remote(sasl, CPLD_PIM_SIG_L);
		sig[i] |= cpld_rd_remote(sasl, CPLD_PIM_SIG_H) << 8;
	}

	/* Select proper BGX mode for pim interfaces */
	for (i = 4; i < 8; i++) {
		if ((i > 5 && pim[0]) || (i < 6 && pim[1]))
			mode = CVMX_QLM_MODE_DISABLED;
		else
			mode = cvmx_qlm_get_mode_cn78xx(0, i);

		switch (mode) {
			case CVMX_QLM_MODE_SGMII:
				snprintf(fdt_key, sizeof(fdt_key), "%d,sgmii", i);
				debug("QLM %d in %s mode\n", i, "sgmii");
				break;
			case CVMX_QLM_MODE_XFI:
				if ((i > 5 && sig[0] == IM13262_SIG) ||
				    (i < 6 && sig[1] == IM13262_SIG)) {
					snprintf(fdt_key, sizeof(fdt_key), "%d,gearbox_10g", i);
					debug("QLM %d in %s mode, gearbox\n", i, "xfi");
				} else {
					snprintf(fdt_key, sizeof(fdt_key), "%d,xfi", i);
					debug("QLM %d in %s mode\n", i, "xfi");
				}
				break;
			case CVMX_QLM_MODE_XLAUI:
				if ((i > 5 && sig[0] == IM13262_SIG) ||
				    (i < 6 && sig[1] == IM13262_SIG)) {
					snprintf(fdt_key, sizeof(fdt_key), "%d,gearbox", i);
					debug("QLM %d in %s mode, gearbox\n", i, "xlaui");
				} else {
					snprintf(fdt_key, sizeof(fdt_key), "%d,xlaui", i);
					debug("QLM %d in %s mode, cs4343\n", i, "xlaui");
				}
				break;
			case CVMX_QLM_MODE_40G_KR4:
				snprintf(fdt_key, sizeof(fdt_key), "%d,xlaui", i);
				debug("QLM %d in %s mode\n", i, "XLAUI");
				break;
			case CVMX_QLM_MODE_10G_KR:
				snprintf(fdt_key, sizeof(fdt_key), "%d,gearbox_10g", i);
				debug("QLM %d in %s mode, gearbox\n", i, "10g_kr");
				break;
			default:
				debug("QLM %d is disabled\n", i);
				snprintf(fdt_key, sizeof(fdt_key), "%d,none", i);
		}
		octeon_fdt_patch(working_fdt, fdt_key, NULL);
		/* octeon_fdt_patch_rename(working_fdt, fdt_key, NULL, true, NULL, NULL); */
}

	/* Select SGMII/RXAUI mode for BGX0 interface*/
	cmr_config.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMRX_CONFIG(0, 0));
	if (cmr_config.s.lmac_type == 2) {
		snprintf(fdt_key, sizeof(fdt_key), "0,rxaui");
		debug("QLM 0 in RXAUI mode\n");
	} else if (cmr_config.s.lmac_type == 0) {
		snprintf(fdt_key, sizeof(fdt_key), "0,sgmii");
		debug("QLM 0 in SGMII mode\n");
	} else {
		snprintf(fdt_key, sizeof(fdt_key), "0,none");
		printf("QLM 0 incorrect  mode!\n");
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	/* Select SGMII/RXAUI mode for BGX1 interface*/
	 if (is_sgmii_board_mode()) {
		snprintf(fdt_key, sizeof(fdt_key), "3,sgmii");
		snprintf(fdt_key2, sizeof(fdt_key), "1,none");
		debug("QLM 3 in SGMII mode\n");
	} else {
		cmr_config.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMRX_CONFIG(0, 1));
		if (cmr_config.s.lmac_type == 2) {
			snprintf(fdt_key, sizeof(fdt_key), "1,rxaui");
			debug("QLM 1 in RXAUI mode\n");
		} else if (cmr_config.s.lmac_type == 0) {
			snprintf(fdt_key, sizeof(fdt_key), "1,sgmii");
			debug("QLM 1 in SGMII mode\n");
		} else {
			snprintf(fdt_key, sizeof(fdt_key), "1,none");
			printf("QLM 1 incorrect mode!\n");
		}
		snprintf(fdt_key2, sizeof(fdt_key), "3,none");
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);
	octeon_fdt_patch(working_fdt, fdt_key2, NULL);

	debug("%s EXIT\n", __func__);
	return 0;
}

void im8724_set_max_fans_speed(int full_speed)
{
	cpld_wr_remote(CPLD_BA_CTL_SASL_FAN, CPLD_FAN_ADDR_FC_SS, !!(full_speed));
}

int octeon_boot_bus_board_post_init(void *fdt_addr)
{
	int rc, indx;
	uint64_t cpld_base = 0;
	uint8_t pwr = 0, reg;

	debug("In %s\n", __func__);

	rc = octeon_boot_bus_get_dev_info("/soc/bootbus/cpld",
					  NULL,
					  &cpld_base,
					  NULL);
	if (rc || (cpld_base == 0)) {
		printf("%s: Error getting address of CPLD!\n", __func__);
		return rc;
	}

	printf("CPLD address: 0x%llx\n", cpld_base);
	cpld_init(cpld_base);
	printf("CPLD device %02x%02x rev %02x (PCB rev %c)\n",
		cpld_rd(CPLD_ADDR_CPLD_SIG_H),
		cpld_rd(CPLD_ADDR_CPLD_SIG_L),
		cpld_rd(CPLD_ADDR_CPLD_REV) & 0xf,
		(cpld_rd(CPLD_ADDR_CPLD_REV) >> 4) + 'A' );

	fan_management = FANS_UNINITIALIZED;
      /* Set max fans speed */
      im8724_set_max_fans_speed(1);

	return 0;
}

static void set_pim_status_led(int pim, int stage)
{
	static uint16_t sig = 0;
	uint8_t sasl, clr, lc, misc_csr, st_led_ctl, mask;

	sasl = pim ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
	if (!sig) {
		sig = cpld_rd_remote(sasl, CPLD_PIM_SIG_L);
		sig |= cpld_rd_remote(sasl, CPLD_PIM_SIG_H) << 8;
	}
	switch (sig) {
		case IM13233_SIG:
			misc_csr = CPLD_IM13233_MISC_CSR;
			st_led_ctl = CPLD_IM13233_ST_LED_CTL;
			mask = CPLD_IM13233_TM_OVERT | CPLD_IM13233_TM_ALERT;
			break;
		case IM13264_SIG:
			misc_csr = CPLD_IM13264_MISC_SR;
			st_led_ctl = CPLD_IM13264_ST_LED_CTL;
			mask = CPLD_IM13264_TM_OVERT | CPLD_IM13264_TM_ALERT;
			break;
		case IM13262_SIG:
			misc_csr = CPLD_IM13262_MISC_SR;
			st_led_ctl = CPLD_IM13262_ST_LED_CTL;
			mask = CPLD_IM13262_TM_OVERT | CPLD_IM13262_TM_ALERT;
			break;
		default:
			return;
	}
	if ((cpld_rd_remote(sasl, misc_csr) & mask)) {
		clr = LED_CLR_RED;
		lc = LED_CNTRL_BLINKING;
	} else {
		clr = LED_CLR_GREEN;
		lc = stage ? LED_CNTRL_ON : LED_CNTRL_BLINKING;
	}
	cpld_wr_remote(sasl, st_led_ctl, (clr << 2) | lc);
}

/**
 * output - magic sig from BAR0, if module has found and initialized
 *        - 0 if else
 */
static unsigned int im8724_pim_pci_init(int pim)
{
	unsigned int rev = 0x0;
	return rev;
}

static int im8724_pim_init(int pim)
{
	int err, is_missing, loop = 10;
	uint16_t sig = 0;
	uint8_t reg;

	if (pim != 0 && pim != 1) {
		printf("Invalid pim %d\n", pim);
		return -EINVAL;
	}

	printf("PIM%d: checking for presence... ", pim);
	is_missing = CPLD_PIM_MISSING(pim);
	printf("%s\n", is_missing ? "missing" : "present");

	if (is_missing)
		return is_missing;;

	printf("PIM%d: powering up............. ", pim);
	reg = cpld_rd(CPLD_ADDR_IO_PWR_CTL);
	if ((reg & CPLD_PIM_PWR_ENABLE(pim)) != CPLD_PIM_PWR_ENABLE(pim)) {
		set_pim_status_led(pim, 0);
		cpld_wr(CPLD_ADDR_IO_PWR_CTL, reg | CPLD_PIM_PWR_ENABLE(pim));
	}
	printf("done\n");
	printf("PIM%d: status check............ ", pim);

	while (!CPLD_PIM_READY(pim) && loop-- > 0)
		mdelay(10);

	printf("%s\n", loop > 0 ? "ok" : "timeout");
	if (loop > 0) {
		uint16_t sig = 0;
		uint8_t sasl, rev;
		char name[12];

		mdelay(250);
		set_pim_status_led(pim, 1);
		sasl = pim ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
		sig = cpld_rd_remote(sasl, CPLD_PIM_SIG_L);
		sig |= cpld_rd_remote(sasl, CPLD_PIM_SIG_H) << 8;
		rev = cpld_rd_remote(sasl, CPLD_PIM_CPLD_REV);

		pim_signature[pim] = sig;
		io_cards_presents |= 1 << pim;

		switch ( sig )
		{
		case IM13233_SIG:
			strcpy ( name, "8x10G" ) ;
			cpld_wr_remote_force(sasl, CPLD_IM13233_LED_CFG,	0x03, 0xff);
			cpld_wr_remote      (sasl, CPLD_IM13233_LED_CTL_A,	0xcc);
			cpld_wr_remote      (sasl, CPLD_IM13233_LED_CTL_B,	0xcc);
			cpld_wr_remote      (sasl, CPLD_IM13233_LED_CTL_C,	0xcc);
			cpld_wr_remote      (sasl, CPLD_IM13233_LED_CTL_D,	0xcc);
			break;
		case IM13264_SIG:
			strcpy ( name, "2x40G" ) ;
			cpld_wr_remote_force(sasl, CPLD_IM13264_LED_CFG,	0x03, 0xff);
			cpld_wr_remote      (sasl, CPLD_IM13264_LED_CTL_A,  0xcc);
			break;
		case IM13262_SIG:
			strcpy ( name, "2x25G" ) ;
			cpld_wr_remote_force(sasl, CPLD_IM13262_LED_CFG,	0x00, 0xff);
			cpld_wr_remote      (sasl, CPLD_IM13262_LED_INDX_0,	0x0f);
			cpld_wr_remote      (sasl, CPLD_IM13262_LED_INDX_1,	0x0f);
			cpld_wr_remote      (sasl, CPLD_IM13262_LED_CTL_A,	0x55);
			break;
		default:
			printf("PIM%d: module info............. custom, rev: 0x%x\n",
				pim,
				im8724_pim_pci_init(pim));
			return 2;
		}

		printf("PIM%d: module info............. %s, rev: 0x%x (cpld rev: 0x%x)\n",
			pim, name,
			rev & PIM_BRD_REV_MASK, rev & PIM_CPLD_REV_MASK);

	}
	mdelay(20);
	return 0;
}

static int get_pim_env_speed(int pim, char *pimside)
{
	char buf[15] = {0};
	char *speed = NULL;
	int res = 0;
	const char *mgb10 = getenv("mgb10");      /* Marvell 88x5113 mode 10G ; u-Boot env */

	if(pim < 0)
		res = snprintf(buf, sizeof(buf)-1, "fp_sfp_%s", pimside);
	else {
		if(IS_CPLD_IM13233(pim))
			res = snprintf(buf, sizeof(buf)-1, "pim%d_sfp_%s", pim, pimside);
		else if(IS_CPLD_IM13264(pim))
			return IFACE_MODE_40G;
		else if(IS_CPLD_IM13262(pim))
		{	if ( NULL == mgb10 )     /* No env defined MGB mode is 25G  */
			{
				printf ( "***===> Marvell 88X5113 mode set to 25G\n" ) ;
				pim_mgbmode[pim] = 0 ;
				return IFACE_MODE_25G;
			} else
			{
				printf ( "***===> Marvell 88X5113 mode set to 10G\n" ) ;
				pim_mgbmode[pim] = 1 ;
				return IFACE_MODE_10GB;
			}
		}
		else
			return IFACE_MODE_10G;
	}

	if(res)
		speed = getenv(buf);

	return (speed && !strcmp(speed, "1g")) ? IFACE_MODE_1G : IFACE_MODE_10G;
}

static void im8724_configure_qlm(int bgx_mode, int qlm_num)
{

	switch (bgx_mode) {
		case IFACE_MODE_40G:
			debug ( "***> 40G configuration. \n") ;
			octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_XLAUI, 0, 0, 2, 2);
			octeon_qlm_tune_v3(0, qlm_num, 103125, 0xa, 0xa0, -1, -1);
			break;
		case IFACE_MODE_25G:
			debug ( "***> 25G configuration. \n") ;
			octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_XLAUI, 0, 0, 2, 2);
			/*octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_40G_KR4, 0, 0, 2, 2);    Until fully confirmed. */
			octeon_qlm_tune_v3(0, qlm_num, 103125, 0xd, 0xd0, -1, -1);
			break;
		case IFACE_MODE_10G:
			octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_XFI, 0, 0, 2, 2);
			octeon_qlm_tune_v3(0, qlm_num, 103125, 0xd, 0xd0, -1, -1);
			break;
		case IFACE_MODE_10GB:
			debug ( "***> 10GGBox configuration. \n") ;
			octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_XFI, 0, 0, 2, 2);
			/*octeon_configure_qlm_cn78xx(0, qlm_num, 103125, CVMX_QLM_MODE_10G_KR, 0, 0, 2, 2); */
			octeon_qlm_tune_v3(0, qlm_num, 103125, 0xd, 0xd0, -1, -1);
			break;
		case IFACE_MODE_1G:
			octeon_configure_qlm_cn78xx(0, qlm_num, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 2);
			break;
		case IFACE_MODE_DIS:
		default:
			octeon_configure_qlm_cn78xx(0, qlm_num, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);
	}
}

/**
* This function is called to initialize the QLMs
*/
void board_configure_qlms(void)
{
	cvmx_bgxx_cmr_global_config_t gconfig;
	const char *qlm_mode = getenv("qlm1_mode");
	uint8_t bgx_iface_mode[BGX_MS_IFACE_MAX];
	char *speed = NULL;
	uint8_t reg;
	int i, pim;

	for (i = 0; i < BGX_MS_IFACE_MAX; ++i)
		bgx_iface_mode[i] = IFACE_MODE_DIS;

	bgx_iface_mode[BGX_MS_IFACE_0] = get_pim_env_speed(-1, "right");

	gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
	if (qlm_mode && !strcmp(qlm_mode, "rxaui")) {
		bgx_iface_mode[BGX_MS_IFACE_1] = get_pim_env_speed(-1, "left");
		gconfig.s.pmux_sds_sel = 0;
	} else {
		bgx_iface_mode[BGX_MS_IFACE_1] = IFACE_MODE_1G;
		gconfig.s.pmux_sds_sel = 1;
	}

	cvmx_write_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1), gconfig.u64);
	gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
	printf("SerDes/QLM output selected: %d (%s mode)\n", gconfig.s.pmux_sds_sel ? 3 : 1,
					gconfig.s.pmux_sds_sel ? "SGMII" : "RXAUI");
	puts("QLMs configuration ........... ");
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
	if (bgx_iface_mode[BGX_MS_IFACE_0] == IFACE_MODE_10G) {
		octeon_configure_qlm_cn78xx(0, 0, 6250, CVMX_QLM_MODE_RXAUI, 0, 0, 2, 2);
		octeon_qlm_tune_v3(0, 0, 6250, 0x12, 0xa0, -1, -1);
		octeon_qlm_dfe_disable(0, 0, -1, 6250, CVMX_QLM_MODE_RXAUI);
	} else
		octeon_configure_qlm_cn78xx(0, 0, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 2);

	octeon_configure_qlm_cn78xx(0, 2, 0, CVMX_QLM_MODE_PCIE, 1, 2, 0, 1);
	if (gconfig.s.pmux_sds_sel) {
		octeon_configure_qlm_cn78xx(0, 3, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 2); /* QLM 3 */
		octeon_configure_qlm_cn78xx(0, 1, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);
	} else if (bgx_iface_mode[BGX_MS_IFACE_1] == IFACE_MODE_10G) {
		octeon_configure_qlm_cn78xx(0, 1, 6250, CVMX_QLM_MODE_RXAUI, 0, 0, 2, 2); /* QLM 1 */
		octeon_qlm_tune_v3(0, 1, 6250, 0x12, 0xa0, -1, -1);
		octeon_qlm_dfe_disable(0, 1, -1, 6250, CVMX_QLM_MODE_RXAUI);
		octeon_configure_qlm_cn78xx(0, 3, 0, CVMX_QLM_MODE_DISABLED, 0, 0, 0, 0);
	} else
		octeon_configure_qlm_cn78xx(0, 1, 1250, CVMX_QLM_MODE_SGMII, 0, 0, 2, 2);
	puts("done\n");

	if (im8724_pim_init(0) == 0) {
		bgx_iface_mode[BGX_MS_IFACE_5] = get_pim_env_speed(0, "left");
		bgx_iface_mode[BGX_MS_IFACE_4] = get_pim_env_speed(0, "right");

		im8724_configure_qlm(bgx_iface_mode[BGX_MS_IFACE_5], 7);
		im8724_configure_qlm(bgx_iface_mode[BGX_MS_IFACE_4], 6);
	} else {
		im8724_configure_qlm(IFACE_MODE_DIS, 7);
		im8724_configure_qlm(IFACE_MODE_DIS, 6);
	}

	if (im8724_pim_init(1) == 0) {
		bgx_iface_mode[BGX_MS_IFACE_3] = get_pim_env_speed(1, "left");
		bgx_iface_mode[BGX_MS_IFACE_2] = get_pim_env_speed(1, "right");

		im8724_configure_qlm(bgx_iface_mode[BGX_MS_IFACE_3], 5);
		im8724_configure_qlm(bgx_iface_mode[BGX_MS_IFACE_2], 4);
	} else {
		im8724_configure_qlm(IFACE_MODE_DIS, 5);
		im8724_configure_qlm(IFACE_MODE_DIS, 4);
	}

	/* Reset PEX-8724 */
	 reg = cpld_rd(CPLD_ADDR_RST_CTL_0) | CPLD_BIT_PEX_PCIE;
	 cpld_wr(CPLD_ADDR_RST_CTL_0, reg);
	 reg &= ~CPLD_BIT_PEX_PCIE;
	 cpld_wr(CPLD_ADDR_RST_CTL_0, reg);
}

int octeon_add_user_boot_bus_fdt_devices(void)
{
	int rc;
	static const char *cpld_compat[] = { "imt,im8724-cpld",
					     NULL };

	debug("In %s\n", __func__);
	rc = octeon_boot_bus_add_fdt_handler("cpld",
					     (void *)cpld_compat,
					     &octeon_boot_bus_generic_init);
	return rc;
}

static void set_bgx_promiscuous_mode(int mode)
{
	int num_interfaces = cvmx_helper_get_number_of_interfaces();
	int interface, xiface, num_ports, index;

	for (interface = 0; interface < num_interfaces; interface++) {
		xiface = cvmx_helper_node_interface_to_xiface(0, interface);
		num_ports = cvmx_helper_ports_on_interface(xiface);
		for (index = 0; index < num_ports; index++) {
			if (!cvmx_helper_is_port_valid(xiface, index))
				continue;
			/* Reset MAC filtering */
			cvmx_helper_bgx_rx_adr_ctl(0, interface, index,
					mode ? 1 : 0, 0, 0);
		}
	}
}

void board_pre_exec(void)
{
	set_bgx_promiscuous_mode(0);
}

static int im8724_sfp_check_mod_abs(struct octeon_eth_info* ethinfo)
{
	static uint16_t sig = 0;
	uint8_t sasl, offset, regval, sfp_pres;

	if (ethinfo->interface < 2) {
		offset = ethinfo->interface ? 2 : 0;
		offset += ethinfo->index;
		regval = cpld_rd(CPLD_ADDR_SFP_PRES);
	} else {
		sasl = (ethinfo->interface > 3) ? CPLD_BA_CTL_SASL_PIM0 : CPLD_BA_CTL_SASL_PIM1;
		if (!sig) {
			sig = cpld_rd_remote(sasl, CPLD_PIM_SIG_L);
			sig |= cpld_rd_remote(sasl, CPLD_PIM_SIG_H) << 8;
		}
		sfp_pres = (sig == IM13233_SIG) ?
				CPLD_IM13233_SFP_PRES :
					((sig == IM13264_SIG) ?
						CPLD_IM13264_QSFP_PRES :
						((sig == IM13262_SIG) ?
							CPLD_IM13262_SFP_PRES :
							CPLD_IM13233_SFP_PRES));
		offset = (ethinfo->interface % 2) ? 4 : 0;
		offset += ethinfo->index;
		regval = cpld_rd_remote(sasl, sfp_pres);
	}
	return !(regval & (1 << offset));
}

#define IS_CS4343_PHY_ATTACHED(ethinfo)    (ethinfo->interface < 2) ? 0 : 1

static uint32_t get_adt7470_tach(int addr, int indx)
{
	uint32_t val = cpld_i2c_read(0x9e, 0x0, addr, ADT7470_REG_TACH(indx));
	val |= ((uint16_t)cpld_i2c_read(0x9e, 0x0, addr, ADT7470_REG_TACH(indx) + 1) << 8);

	if(0xffff != val)
		return val;
	else
		return 0;
}

static uint32_t get_fans_lowest(void)
{
	int indx;
	uint32_t val, ret = 0xfffe; // 0xffff at adt7470 means 100rpm or lower

	for (indx = 0; indx < ADT7470_REG_TACH_MAX_NUMBER; indx++) {
		val = get_adt7470_tach(0x2c, indx);
		if(val < ret)
			ret = val;

		val = get_adt7470_tach(0x2f, indx);
		if(val < ret)
			ret = val;

		if(!ret)
			break;
	}

	return ret;
}

void octeon_board_poll(void)
{
	struct eth_device *dev;
	static int i = 0;

	dev = eth_get_dev_by_index(i++);
	if (dev)
		octeon_phy_port_check(dev);
	if (i >= max_eth_devs) i = 0;

	if (hw_get_cycles() > 1000000000ULL + curtime) {
		if (leds_management >= 0)
			leds_mgmt();

		if (fan_management >= 0)
			fans_mgmt();

		curtime = hw_get_cycles();
	}
}

int cs4224_mmi_bus(struct mii_dev *bus)
{
	if (strstr(bus->name, "octeon2") || strstr(bus->name, "octeon3"))
		return 1;
	return 0;
}

static enum cortina_slice_mode
get_media_type(int slot, uint8_t port)
{
	cvmx_bgxx_cmr_global_config_t gconfig;
	int32_t regval, bgx;
	uint8_t sasl;

	if (IS_CPLD_IM13233(slot)) {
		bgx = slot == 0 ? 4 : 2;
		if (port < 4)
			++bgx;
		gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(bgx));
		if (gconfig.s.pmux_sds_sel)
			return CORTINA_SLICE_1000X;
	}

	if(get_pim_env_speed(slot, port > 3 ? "left" : "right") == IFACE_MODE_1G)
		return CORTINA_SLICE_1000X;

	sasl = slot ? 0x81 : 0x80;
	if ((regval = cpld_i2c_read(sasl, port, 0x50, 8)) < 0)
		return CORTINA_SLICE_UNKNOWN;
	if ((regval & 0xc) == 0x00)
			return CORTINA_SLICE_SR; // short range fiber
	return CORTINA_SLICE_CX; // 10G copper
}

static int cs4343_get_ctrl(struct phy_device *phydev,
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

	debug("%s: slice: %d, offset: 0x%x\n", phydev->dev->name,
	      pinfo->sub_addr, pinfo->slice_offsets[pinfo->sub_addr]);
	if (mode >= 3 || slice >= 4) {
		printf("%s: Mode %d or slice %d out of range\n",
			phydev->dev->name, mode, slice);
		return -1;
	}
	debug("before: ctrla: 0x%x, ctrlb: 0x%x\n", *ctrla, *ctrlb);
	*ctrla = tx_out_ctrla_values[mode][addr & 1][slice];
	*ctrlb = tx_out_ctrlb_values[mode][addr & 1][slice];
	return 0;
}

static const char *sfp_mode_to_str[] = {
	"SR", "CX", "1000Base-X", "Unknown"
};

/* Checks which SFP module is plugged in and configures the PHY
* to the proper mode.  */
static int im8724_port_check(struct phy_device *phydev)
{
	struct cortina_phy_info *pinfo = phydev->priv;
	struct eth_device *dev = phydev->dev;
	struct octeon_eth_info *ethinfo;
	enum cortina_slice_mode mode;
	int mod_abs, i;
	uint8_t port;

	if (!dev)
		return -1;

	ethinfo = (struct octeon_eth_info *) dev->priv;
	if (IS_CS4343_PHY_ATTACHED(ethinfo)) {
		mod_abs = im8724_sfp_check_mod_abs(ethinfo);
		if (!mod_abs && mod_abs != ethinfo->mod_abs) {
			if (pinfo->quad_mode) {
				port = (ethinfo->interface % 2) ? 0 : 1;
			} else {
				port = (ethinfo->interface % 2) ? 0 : 4;
				port += ethinfo->index;
			}
			mode = get_media_type(ethinfo->interface < 4 ? 1 : 0, port);
			if (mode != pinfo->mode) {
				if (pinfo->quad_mode) {
					for (i = 0; i < pinfo->max_slices; i++)
						cs4343_get_ctrl(phydev, i,
							mode,
							&pinfo->tx_output_ctrla[i][mode],
							&pinfo->tx_output_ctrlb[i][mode]);
				} else {
					cs4343_get_ctrl(phydev, pinfo->sub_addr,
						mode,
						&pinfo->tx_output_ctrla[pinfo->sub_addr][mode],
						&pinfo->tx_output_ctrlb[pinfo->sub_addr][mode]);
				}
				debug("SFP mode: %s, old mode: %s\n", sfp_mode_to_str[mode], sfp_mode_to_str[pinfo->mode]);
				cs4224_set_mode(phydev, mode);
			}
		}
		ethinfo->mod_abs = mod_abs;
	}
	return 0;
}

int octeon_board_post_phy_init(struct eth_device *dev,
				const void *fdt_blob, int fdt_offset)
{
	struct octeon_eth_info *ethinfo = dev->priv;
	struct cortina_phy_info *pinfo;
	struct phy_device *phydev;

	if (!ethinfo->phydev || ethinfo->interface < 2)
		return 0;

	phydev = ethinfo->phydev;
	pinfo = phydev->priv;
	pinfo->mode = CORTINA_SLICE_UNKNOWN;
	ethinfo->mod_abs = 1;
	im8724_port_check(phydev);
	octeon_eth_register_phy_port_check(dev, im8724_port_check);
	return 0;
}

void board_net_postinit(void)
{
	cvmx_bgxx_spux_control1_t spu_control1;
	int sgmii_mode = 0, pim, lane, iface;
	char *mgmt_phy_mode = NULL;
	int regval = -1;

	/* Init RXAUI frontpanel PHY. */
	sgmii_mode = is_sgmii_board_mode();
	fp_ports_num = sgmii_mode ? 2 : 4;
	init_cs4318_phys(sgmii_mode);
	if (!sgmii_mode)
		++max_eth_devs;

	/* Reset all BGX lines for RXAUI interfaces */
	for (iface = 0; iface < 2; ++iface) {
		if (sgmii_mode && iface)
			break;
		for (lane = 0; lane < 4; ++lane) {
			spu_control1.u64 = cvmx_read_csr_node(0, CVMX_BGXX_SPUX_CONTROL1(lane, iface));
			spu_control1.s.reset = 1;
			cvmx_write_csr_node(0, CVMX_BGXX_SPUX_CONTROL1(lane, iface), spu_control1.u64);
			if (CVMX_WAIT_FOR_FIELD64_NODE(0, CVMX_BGXX_SPUX_CONTROL1(lane, iface),
				cvmx_bgxx_spux_control1_t, reset, ==, 0, 10000)) {
					debug("BGX%d: SPU stuck in reset\n", iface);
			}
		}
	}

	/* Set required HW/SW modes for main board phys leds */
	cpld_wr(CPLD_LED_CFG, 0xe);
	cpld_wr(CPLD_S0_LED_CTL, 0xcc);
	cpld_wr(CPLD_S2_LED_CTL, 0xcc);

	/* Init PHYs on pluggable modules. */
	for (pim = 0; pim < MAX_IM8724_MODULES; ++pim) {
		if (CPLD_PIM_MISSING(pim))
			continue;

		switch ( pim_signature[pim] )
		{
		case IM13233_SIG:
			max_eth_devs += MAX_SFP_PORTS_PER_MODULE;
			init_cs4343_phys(pim);
			break;
		case IM13264_SIG:
			max_eth_devs += MAX_QSFP_PORTS_PER_MODULE;
			init_cs4343_phys(pim);
			break;
		case IM13262_SIG:
			max_eth_devs += MAX_SFP28_PORTS_PER_MODULE;
			check_5113_presence(pim);
			initialize_5113(pim);
			set_5113_led(pim);
			break;
		default:
			break;
		}
	}

	/* Disable SGMII ANEG on Cavium side and VSC8221 side. */
	cvmx_mdio_write(0, 0, VSC8221_PHY_CTL_REG, VSC8221_PHY_SGMII_MODE);
	cvmx_mdio_write(0, 0, VSC8221_MODE_CTL_REG, VSC8221_CTL_SW_RESET);

	/* Enable MDINT interrupt with Link Status mask. */
	cvmx_mdio_write(0, 0, VSC8221_ISR_MASK_REG, VSC8221_ISR_MSK_PIN_ENA |
						    VSC8221_ISR_MSK_LINK_STATE);

	/* Set VS8221 led mode according to CPLD spec, Table 3 */
	cvmx_mdio_write(0, 0, VSC8221_LED_CTL_REG,
			VSC8221_LED_CFGS_MODE | VSC8221_LED_PIN2_CFG | VSC8221_LED_PIN1_CFG | VSC8221_LED_PIN0_CFG);

	mgmt_phy_mode = getenv("mgmt_phy_mode");
	if (mgmt_phy_mode && strcmp(mgmt_phy_mode, MGMT_PHY_MODE_AUTO)) {
		int xipd_port = cvmx_helper_get_ipd_port(1, 0);
		cvmx_helper_link_info_t link_info;

		link_info.s.link_up = 1;
		if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_1000M_FD)) {
			regval = VSC8221_CTL_MSB_SPEED | VSC8221_CTL_FULL_DUPLEX;
			link_info.s.full_duplex = 1;
			link_info.s.speed = 1000;
		} else if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_100M_FD)) {
			regval = VSC8221_CTL_LSB_SPEED | VSC8221_CTL_FULL_DUPLEX;
			link_info.s.full_duplex = 1;
			link_info.s.speed = 100;
		} else if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_1000M_HD)) {
			regval = VSC8221_CTL_MSB_SPEED;
			link_info.s.full_duplex = 0;
			link_info.s.speed = 1000;
		} else if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_100M_HD)) {
			regval = VSC8221_CTL_LSB_SPEED;
			link_info.s.full_duplex = 0;
			link_info.s.speed = 100;
		} else if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_10M_FD)) {
			regval = VSC8221_CTL_FULL_DUPLEX;
			link_info.s.full_duplex = 1;
			link_info.s.speed = 10;
		} else if (!strcmp(mgmt_phy_mode, MGMT_PHY_MODE_10M_HD)) {
			regval = 0;
			link_info.s.full_duplex = 0;
			link_info.s.speed = 10;
		}

		if (regval >= 0) {
			printf("Force management phy mode to %s\n", mgmt_phy_mode);
			cvmx_mdio_write(0, 0, VSC8221_MODE_CTL_REG, regval);
			cvmx_helper_link_set(xipd_port, link_info);
		}
	}

	/* Initialize networking (BGX). */
	eth_init(gd->bd);

	/* Disable promiscuous mode for all BGX interfaces */
	set_bgx_promiscuous_mode(1);
}

int checkboard(void)
{
	return 0;
}

int early_board_init(void)
{
	int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

	octeon_board_get_clock_info(IM8724_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CUST_IM8724, 1, 0);

	/* Set for CN78XX alternative 100 MHz reference clock,
	 * instead of 50 Hz: (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) */
	gd->arch.ddr_ref_hertz = 100000000ull;

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}

static int i2c_dbg = 0;

static int cpld_i2c_read(uint8_t sasl, uint8_t bus, uint8_t dev, uint8_t reg)
{
	uint8_t rc;

	if ((sasl & 0x80)) {
		cpld_wr_remote(sasl, CPLD_ADDR_I2C_BSL, bus);
		cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_EB_EN | CPLD_BA_CTL_B_MD_I2C | sasl);
	}
	else {
		cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_B_MD_I2C);
		cpld_wr(CPLD_ADDR_I2C_BSL, bus);
	}
	cpld_wr(CPLD_ADDR_I2C_DA, (dev & 0x7f) << 1);
	cpld_wr(CPLD_ADDR_I2C_CTRL, CPLD_I2C_CTL_R_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
	cpld_wr(CPLD_ADDR_I2C_ADDR_0, reg);
	cpld_wr(CPLD_ADDR_I2C_CSR, CPLD_I2C_CSR_ST | CPLD_I2C_CSR_READ);

	while (cpld_rd(CPLD_ADDR_I2C_CSR) & CPLD_I2C_CSR_ST)
		continue;

	rc = cpld_rd(CPLD_ADDR_I2C_ST) & CPLD_I2C_STAT_MASK;
	if (rc != 0 && i2c_dbg) {
		printf("%s: i2c read error (sasl:0x%x, bus:0x%x). stat: %02x\n", __func__, sasl, bus, rc);
		return -EIO;
	}
	return cpld_rd(CPLD_ADDR_I2C_DATA);
}

static int cpld_i2c_write(uint8_t sasl, uint8_t bus, uint8_t dev,
			  uint8_t reg, uint8_t val)
{
	uint8_t rc;

	if ((sasl & 0x80)) {
		cpld_wr_remote(sasl, CPLD_ADDR_I2C_BSL, bus);
		cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_EB_EN | CPLD_BA_CTL_B_MD_I2C | sasl);
	}
	else {
		cpld_wr(CPLD_ADDR_BA_CTL, CPLD_BA_CTL_B_MD_I2C);
		cpld_wr(CPLD_ADDR_I2C_BSL, bus);
	}

	cpld_wr(CPLD_ADDR_I2C_DA, (dev & 0x7f) << 1);
	cpld_wr(CPLD_ADDR_I2C_CTRL, CPLD_I2C_CTL_R_1000 | CPLD_I2C_CTL_SDA_SCL_HIGH);
	cpld_wr(CPLD_ADDR_I2C_ADDR_0, reg);
	cpld_wr(CPLD_ADDR_I2C_DATA, val);
	cpld_wr(CPLD_ADDR_I2C_CSR, CPLD_I2C_CSR_ST | CPLD_I2C_CSR_WRITE);

	while (cpld_rd(CPLD_ADDR_I2C_CSR) & CPLD_I2C_CSR_ST)
		continue;

	rc = cpld_rd(CPLD_ADDR_I2C_STAT) & CPLD_I2C_STAT_MASK;
	if (rc != 0 && i2c_dbg) {
		printf("%s: i2c write error. stat: %02x\n", __func__, rc);
		return -EIO;
	}
	return 0;
}

int do_cpld_i2c_write(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int sasl, bus, dev, reg, val;

	if (argc != 6)
		return CMD_RET_USAGE;

	sasl = simple_strtol(argv[1], NULL, 16);
	bus = simple_strtol(argv[2], NULL, 16);
	dev = simple_strtol(argv[3], NULL, 16);
	reg = simple_strtol(argv[4], NULL, 16);
	val = simple_strtol(argv[5], NULL, 16);
	i2c_dbg = 1;

	printf("[%02x:%02x:%02x:%02x] <= %02x\n",
		sasl, bus, dev, reg, val);
	cpld_i2c_write((uint8_t)sasl, (uint8_t)bus,
			(uint8_t)dev, (uint8_t)reg, (uint8_t)val);
	i2c_dbg = 0;
	return 0;
}

U_BOOT_CMD(cpld_i2c_wr, 6, 0, do_cpld_i2c_write,
	   "i2c write via cpld extended bus",
	   "<sasl> <bus> <dev> <reg> <val>\n");

int do_cpld_i2c_read(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int sasl, bus, dev, reg, val;

	if (argc != 5)
		return CMD_RET_USAGE;

	sasl = simple_strtol(argv[1], NULL, 16);
	bus = simple_strtol(argv[2], NULL, 16);
	dev = simple_strtol(argv[3], NULL, 16);
	reg = simple_strtol(argv[4], NULL, 16);
	i2c_dbg = 1;

	printf("[%02x:%02x:%02x:%02x] == %02x\n", (uint8_t)sasl, (uint8_t)bus,
		(uint8_t)dev, (uint8_t)reg, cpld_i2c_read(sasl, bus, dev, reg));
	i2c_dbg = 0;
	return 0;
}

U_BOOT_CMD(cpld_i2c_rd, 5, 0, do_cpld_i2c_read,
	   "i2c read via cpld extended bus",
	   "cpld_i2c_rd <sasl> <bus> <dev> <reg>\n");

static int set_temp_crit(const char *tc)
{
	uint8_t cval, nval;
	uint8_t sasl;
	int pim, i;

	if (tc) {
		uint32_t val = simple_strtoul(tc, NULL, 16);

		if (val < 0x1e || val > 0x55)
			printf("*** Set Temp Critical Threshold, 0x%x, is  out of range: 0x1e - 0x55! ***\n", val);
		else
			max_temp_critical_thresh = (uint8_t)val;
	}

	/* Check cpu/board temperature sensorce, init thresholds */
	if (get_max_cpu_brd_temp(&cpu_set_temp) < 0) {
		printf("Temp sensorce check/init failure!\n");
		return -1;
	}

	if (cpu_set_temp > CPU_SET)
		cpu_set_temp = CPU_SET;

	printf("Setting over temp threshold to %dC\n", max_temp_critical_thresh);
	/* MAX6699 */
	i2c_set_bus_num(1);
	for (i=0; i<5; ++i) {
		nval = max_temp_critical_thresh;

		i2c_read(MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &cval, 1);
		i2c_write(MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &nval, 1);
		i2c_read(MAX6699_DEV_ADDR, MAX6699_REG_TEMP_CRIT(i), 1, &nval, 1);
		debug("Changed temp crit thresh [max6699]: %uC->%uC\n", cval, nval);
	}

	/* ADM1032 */
	for (pim=0; pim<MAX_IM8724_MODULES; ++pim) {
		if (io_cards_presents & (1 << pim)) {
			sasl = pim ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;

			cval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_R_LOCAL_HIGH);
			cpld_i2c_write(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_W_LOCAL_HIGH, max_temp_critical_thresh);
			nval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_R_LOCAL_HIGH);
			debug("Changed temp crit thresh [adm1032-pim%d-0]: %uC->%uC\n", pim, cval, nval);

			cval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_R_REMOTE_HIGHH);
			cpld_i2c_write(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_W_REMOTE_HIGHH, max_temp_critical_thresh);
			nval = cpld_i2c_read(sasl, 0x9, ADM1032_DEV_ADDR, ADM1032_REG_R_REMOTE_HIGHH);
			debug("Changed temp crit thresh [adm1032-pim%d-1]: %uC->%uC\n", pim, cval, nval);
		}
	}
	return 0;
}

static void set_fan_speed(const char *fs)
{
	uint32_t val;
	int i = 0;
	int indx;

	if (fs) {
		val = simple_strtoul(fs, NULL, 16);
		if (val < 0x0c || val > 0xff) {
			printf("*** Fan PWM must be set in the range: 0x0c - 0xff! ***\n");
			return;
		}
	} else
		val = DEFAULT_FAN_RPM_VALUE;

	def_fan_pwm = val;

	/* Turn off CPLD max fans speed bit */
	im8724_set_max_fans_speed(0);

	debug("Set initial fans speed...");
	for (indx = 0; indx < ADT7470_REG_PWM_MAX_NUMBER; indx++) {
		cpld_i2c_write(0x9e, 0x0, 0x2c, ADT7470_REG_PWM(indx), (uint8_t) val);
		cpld_i2c_write(0x9e, 0x0, 0x2f, ADT7470_REG_PWM(indx), (uint8_t) val);
	}

	debug(" [done]\nFan/temp sensors board init is finished!\n");
	debug("Set Fan PWM to 0x%x\n", val);
}

#define CPU_TMP_HYST 2

void fans_mgmt(void)
{
	int fan_speed, indx;
	uint8_t cpu_tmp = MAX_TEMP_ALARM_THRESH;
	static uint8_t curr_cpu_tmp = 0xff;

	if (get_max_cpu_brd_temp(&cpu_tmp) < 0) {
		fan_speed = 0xff;
	} else {
		if (cpu_tmp <= cpu_set_temp) {
			fan_speed = def_fan_pwm;
		} else if (cpu_tmp < MAX_TEMP_ALARM_THRESH) {
			if(0xff != curr_cpu_tmp) {
				if(!((cpu_tmp >= curr_cpu_tmp+CPU_TMP_HYST)
					|| (cpu_tmp <= curr_cpu_tmp-CPU_TMP_HYST))) {
					return;
				}
			}
			curr_cpu_tmp = cpu_tmp;
			fan_speed = def_fan_pwm + (MAX_FAN_RPM_VALUE - def_fan_pwm) *
				(cpu_tmp - cpu_set_temp)/(MAX_TEMP_ALARM_THRESH - cpu_set_temp);
		} else {
			fan_speed = 0xff;
		}
	}

	if (cpu_tmp >= max_temp_critical_thresh) {
		printf("FAIL: Critical box temperature is reached: %u >= %u !\n",
					cpu_tmp, max_temp_critical_thresh);
		reset_device();
	}

	debug("cpu_tmp:%d,cpu_set_temp:%d,fan_speed:%d\n",
				cpu_tmp, cpu_set_temp, fan_speed);

	for (indx = 0; indx < ADT7470_REG_PWM_MAX_NUMBER; indx++) {
		cpld_i2c_write(0x9e, 0x0, 0x2c, ADT7470_REG_PWM(indx), (uint8_t) fan_speed);
		cpld_i2c_write(0x9e, 0x0, 0x2f, ADT7470_REG_PWM(indx), (uint8_t) fan_speed);
	}
}

static uint8_t get_sfp_rate(int slot, uint8_t port, uint8_t dbg)
{
	uint8_t sasl, bus;
	int32_t reg_val[2];
	int ret;

	if (slot >1 || (slot < 0 && port >3) || port > 7) {
		if (dbg) printf("SFP slot/port bad params!\n");
		return 0;
	}

	sasl = slot < 0 ? 0x0 : slot == 0 ? 0x80 : 0x81;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x50, 3)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x50, 6)) < 0)) {
		if (dbg) printf("SFP not present!\n");
		return 0;
	}

	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf)) {
		if (dbg) printf("Single rate 10G SFP+\n");
		return 10;
	}

	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf)) {
		if (dbg) printf("Single rate 1G SFP+\n");
		return 1;
	}

	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x51, 110)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x51, 118)) < 0)) {
		if (dbg) printf("SFP+ diag EEPROM access error!\n");
		return 0;
	}

	/* if mismatch, fixing it */
	if ((reg_val[0] & 0x08) != (reg_val[1] & 0x08)) {
		if (!(reg_val[0] & 0x08))
			reg_val[1] |= 0x08;
		else
			reg_val[1] &= ~0x08;
		cpld_i2c_write(sasl, port, 0x51, 118, (uint8_t)reg_val[1]);
	}

	ret = ((reg_val[0] & 0x08)) ? 10 : 1;
	if (dbg) printf("SFP+ rate: %dG\n", ret);

	return ret;
}

int do_get_sfp_rate(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int slot, port;

	if (argc != 3)
		return CMD_RET_USAGE;

	slot = simple_strtol(argv[1], NULL, 10);
	port = simple_strtol(argv[2], NULL, 10);

	get_sfp_rate(slot, (uint8_t)port, 1);
	return 0;
}

U_BOOT_CMD(sfp_rate_get, 3, 0, do_get_sfp_rate,
	   "get sfp slot/port rate (slot=-1 for main bord)",
	   "<slot> <port>\n");

static void set_sfp_rate(int slot, uint8_t port, uint8_t rate, uint8_t force, uint8_t dbg)
{
	int32_t reg_val[2];
	uint8_t sasl;

	if (slot >1 || (slot < 0 && port >3) || port > 7) {
		if (dbg) printf("SFP slot/port bad params!\n");
		return;
	}
	if (rate != 1 && rate != 10) {
		if (dbg) printf("Correct rate values 1 or 10!\n");
		return;
	}

	sasl = slot < 0 ? 0x0 : slot == 0 ? 0x80 : 0x81;
	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x50, 3)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x50, 6)) < 0)) {
		if (dbg) printf("SFP not present\n");
		return;
	}

	if ((reg_val[0] & 0xf0) && !(reg_val[1] & 0xf)) {
		if (dbg) printf("Single rate 10G SFP+\n");
		return;
	}

	if (!(reg_val[0] & 0xf0) && (reg_val[1] & 0xf)) {
		if (dbg) printf("Single rate 1G SFP+\n");
		return;
	}

	if (((reg_val[0] = cpld_i2c_read(sasl, port, 0x51, 110)) < 0) ||
	    ((reg_val[1] = cpld_i2c_read(sasl, port, 0x51, 118)) < 0)) {
		if (dbg) printf("SFP+ diag EEPROM access error!\n");
		return;
	}

	if (!force &&
	    ((rate == 1 && !(reg_val[0] & 0x08) && !(reg_val[1] & 0x08)) ||
	     (rate == 10 && (reg_val[0] & 0x08) && (reg_val[1] & 0x08)))) {
		if (dbg) printf("SFP+ rate %d is already set!\n", rate);
		return;
	}

	if (rate == 1) {
		reg_val[0] &= ~0x08; reg_val[1] &= ~0x08;
	} else {
		reg_val[0] |= 0x08; reg_val[1] |= 0x08;
	}

	cpld_i2c_write(sasl, port, 0x51, 110, (uint8_t)reg_val[0]);
	cpld_i2c_write(sasl, port, 0x51, 118, (uint8_t)reg_val[1]);
	if (dbg) printf("Set SFP+ rate: %dG\n", rate);
}

int do_set_sfp_rate(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int slot, port, rate;

	if (argc != 4)
		return CMD_RET_USAGE;

	slot = simple_strtol(argv[1], NULL, 10);
	port = simple_strtol(argv[2], NULL, 10);
	rate = simple_strtol(argv[3], NULL, 10);
	set_sfp_rate(slot, (uint8_t)port, (uint8_t)rate, 1, 1);
	return 0;
}

U_BOOT_CMD(sfp_rate_set, 4, 0, do_set_sfp_rate,
	   "set sfp slot/port rate (slot=-1 for main bord)",
	   "<slot> <port> <10|1>\n");

int do_mgmt_port_bgx_speed(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	cvmx_helper_link_info_t link_info;
	int speed, xipd_port;

	if (argc != 2)
		return CMD_RET_USAGE;

	xipd_port = cvmx_helper_get_ipd_port(1, 0);
	speed = simple_strtol(argv[1], NULL, 10);
	if (speed == 10 || speed == 100 || speed == 1000) {
		link_info.s.link_up = 1;
		link_info.s.full_duplex = 1;
		link_info.s.speed = speed;
		printf("Setting speed to %d, ipd_port: %d\n", speed, xipd_port);
		cvmx_helper_link_set(xipd_port, link_info);
	}
	return 0;
}

U_BOOT_CMD(mgmt_p_speed, 2, 0, do_mgmt_port_bgx_speed,
	    "set mgmt port speed",
	    "<10|100|1000>\n");

int do_mgmt_port_status(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int xipd_port = cvmx_helper_get_ipd_port(1, 0);
	cvmx_helper_link_info_t link_info = cvmx_helper_link_get(xipd_port);

	if (link_info.s.link_up)
		printf("Mgmt port (%d), link: UP, speed %d, duplex: %s\n", xipd_port,
				link_info.s.speed, link_info.s.full_duplex ? "full" : "half");
	else
		printf("Mgmt port (%d), link: DOWN\n", xipd_port);
	return 0;
}

U_BOOT_CMD(mgmt_p_status, 1, 0, do_mgmt_port_status,
	    "get mgmt port status",
	    "\n");

cvmx_helper_interface_mode_t pim_sfp_port_get_mode(int slot, int port)
{
	return cvmx_helper_interface_get_mode(((0 - (((slot ? 8 : 0) + port) - 15)) / 4) + 2);
}

static void set_max_sfp_rates(void)
{
	int slot, port;

	for (port = 0; port < (is_sgmii_board_mode() ? 2 : 4); ++port)
		set_sfp_rate(-1, port,
			(cvmx_helper_interface_get_mode(port/2)
				== CVMX_HELPER_INTERFACE_MODE_SGMII) ? 1 : 10, 1, 0);

	for (slot = 0; slot < MAX_IM8724_MODULES; ++slot)
		if (CPLD_PIM_MISSING(slot) || (!IS_CPLD_IM13233(slot)))
			continue;
		for (port = 0; port < MAX_SFP_PORTS_PER_MODULE; ++port)
			set_sfp_rate(slot, port, pim_sfp_port_get_mode(slot, port)
					== CVMX_HELPER_INTERFACE_MODE_SGMII ? 1 : 10, 1, 0);
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

static int adjust_bgx_link_state_by_phy_settings(int xiface, int index, cvmx_helper_link_info_t *link_info)
{
	static cvmx_helper_link_info_t li = {0};
	static int initial_call = 1;
	struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);
	cvmx_bgxx_gmp_pcs_mrx_control_t gmp_ctl;
	int val;

	/* Adjustment for mgmt port only, VSC8221 */
	if (xi.interface != 1 || index)
		return 0;

	if ((cpld_rd(CPLD_ADDR_INT_STAT_0) & CPLD_INT_MGT_PHY) || /* if VSC8221 link status is changed */
	     initial_call)  {					  /* or initial call */
		/* Clear Link status Interrupt */
		cvmx_mdio_read(0, 0, VSC8221_ISR_STATUS_REG);

		val = cvmx_mdio_read(0, 0, VSC8221_STATUS_REG);
		memset(&li, 0, sizeof(li));
		if ((val & VSC8221_LINK_STATUS)) {
			li.s.link_up = 1;
			val = cvmx_mdio_read(0, 0, VSC8221_AUXIL_CS_REG);
			li.s.full_duplex = (val >> 5) & 0x1;
			val = (val >> 3) & 0x3;
			li.s.speed = val == 2 ? 1000 : val == 1 ? 100: 10;
		}

		cvmx_helper_link_set(cvmx_helper_get_ipd_port(1, 0), li);
		if (initial_call)
			initial_call = 0;
	}
	*link_info = li;
	return 1;
}

int late_board_init(void)
{
	cvmx_pciercx_cfg032_t pciercx_cfg032;
	union cvmx_rst_boot rst_boot;
	uint8_t reg, clr, lc, cpld_reg;

	/* Enable USB Power */
	reg = cpld_rd(CPLD_ADDR_IO_PWR_CTL);
	if (!(reg & CPLD_USB_PE))
		cpld_wr(CPLD_ADDR_IO_PWR_CTL, reg | CPLD_USB_PE);

	/* PCIe init fixup for board cold boot init */
	rst_boot.u64 = cvmx_read_csr(CVMX_RST_BOOT);
	if (rst_boot.s.lboot < 0x3) {
		pciercx_cfg032.u32 = cvmx_pcie_cfgx_read_node(0, 2, CVMX_PCIERCX_CFG032(2));
		debug("%s: lboot: %d, pcie: gen%d\n", __func__,
			rst_boot.s.lboot, pciercx_cfg032.s.ls);
		if (pciercx_cfg032.s.ls == 1)
			cvmx_pcie_rc_initialize(2);
	}

	cvmx_helper_adjust_bgx_link_state_by_phy_settings = adjust_bgx_link_state_by_phy_settings;
	cvmx_helper_get_real_78xx_xaui_link_status = get_real_78xx_xaui_link_status;

	/* Enable Over-Temperature Shut Down Enable. */
	cpld_reg = cpld_rd(CPLD_ADDR_INT_EN_0) | 0x80;
	cpld_wr(CPLD_ADDR_INT_EN_0, cpld_reg);

	/* Lower temp sensors critical thresholds */
	fan_management = set_temp_crit(getenv("temp_crit"));

	/* Check fan board presence */
	if (cpld_rd_remote(CPLD_BA_CTL_SASL_FAN, CPLD_FAN_ADDR_SIGNATURE) == 0x8d) {
		/* Lower fan speed */
		if(fan_management >= 0)
			set_fan_speed(getenv("fan_pwm"));
	} else {
		printf("*** Fan Board CPLD not detected, no fan speed control! ***\n");
		fan_management = FANS_UNINITIALIZED;
	}

	curtime = hw_get_cycles();

	/* Set all available dual speed SFP+ to max rate */
	set_max_sfp_rates();

	leds_management = sys_leds_init();
	return 0;
}

#include "leds_mgmt.c"
