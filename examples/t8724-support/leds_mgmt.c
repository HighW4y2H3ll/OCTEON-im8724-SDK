#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include <cvmx.h>
#include <cvmx-helper.h>
#include <cvmx-platform.h>
#include <cvmx-mdio.h>

#include "imt_cpld.h"
#include "leds_mgmt.h"
#include "cortina_phys.h"

extern uint32_t get_fans_lowest(void);

/*Enable policy with forcing SFP+ rate to 10G */
static int set_max_sfp_rate = 1;

static int io_cards_presents = 0;
static int fp_ports_num = 4;
static uint8_t dbg_mode = 0;
static int fp_leds_is_init = 0;
static uint16_t io_card_sign_l[MAX_IM8724_MODULES] = { 0, 0 };

static void write_pim_led_info(int slot, int port, int flag, uint8_t value);
static uint8_t read_pim_led_info(int slot, int port, int flag);
static void write_fp_led_info(int port, int flag, uint8_t value);
static uint8_t read_fp_led_info(int port, int flag);
static int get_pim_interface_activity(int slot, int slice);
static int get_fp_interface_activity(int slice);
static uint8_t get_pim_interface_speed(int slot, int slice);
static uint8_t get_fp_interface_speed(int port);
static int sfp_mm_allow = 0; // disabled by default

#define IS_CPLD_IM13233(slot) (io_card_sign_l[slot] == IM13233_SIG)
#define IS_CPLD_IM13264(slot) (io_card_sign_l[slot] == IM13264_SIG)
#define IS_CPLD_IM13262(slot) (io_card_sign_l[slot] == IM13262_SIG)

#define GET_MAX_SLOT_PORTS(slot) (IS_CPLD_IM13233(slot) \
									? MAX_SFP_PORTS_PER_MODULE \
									: (IS_CPLD_IM13264(slot) \
										? MAX_QSFP_PORTS_PER_MODULE \
										: (IS_CPLD_IM13262(slot) \
											? MAX_SFP28_PORTS_PER_MODULE \
											: MAX_SFP_PORTS_PER_MODULE)))
#define CPLD_IM132XX(slot, name) (IS_CPLD_IM13233(slot) \
									? CPLD_IM13233_ ## name \
									: (IS_CPLD_IM13264(slot) \
										? CPLD_IM13264_ ## name \
										: (IS_CPLD_IM13262(slot) \
											? CPLD_IM13262_ ## name \
											: CPLD_IM13233_ ## name)))

int pim_leds_init(void)
{
	uint8_t sasl;
	int slot;

	if (!cpld_is_init) {
		debug(1, "Error: CPLD is not init!\n");
		return -1;
	}

	debug(2, "Start pim leds init... ");
	for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
		if (CPLD_PIM_MISSING(slot))
			continue;

		sasl = slot ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;

		io_card_sign_l[slot] = cpld_rd_remote(sasl, CPLD_PIM_SIG_H) << 8;
		io_card_sign_l[slot] |= cpld_rd_remote(sasl, CPLD_PIM_SIG_L);

		/* Switch on leds management to SW mode and set leds off. */
		switch (io_card_sign_l[slot]) {
			case IM13233_SIG:
				cpld_wr_remote(sasl, CPLD_IM13233_LED_CFG,   0x03);
				cpld_wr_remote(sasl, CPLD_IM13233_LED_CTL_A, 0xcc);
				cpld_wr_remote(sasl, CPLD_IM13233_LED_CTL_B, 0xcc);
				cpld_wr_remote(sasl, CPLD_IM13233_LED_CTL_C, 0xcc);
				cpld_wr_remote(sasl, CPLD_IM13233_LED_CTL_D, 0xcc);
				break;
			case IM13264_SIG:
				cpld_wr_remote(sasl, CPLD_IM13264_LED_CFG,   0x03);
				cpld_wr_remote(sasl, CPLD_IM13264_LED_CTL_A, 0xcc);
				break;
			case IM13262_SIG:
				cpld_wr_remote(sasl, CPLD_IM13262_LED_CFG,    0x00);
				cpld_wr_remote(sasl, CPLD_IM13262_LED_INDX_0, 0x0f);
				cpld_wr_remote(sasl, CPLD_IM13262_LED_INDX_1, 0x0f);
				cpld_wr_remote(sasl, CPLD_IM13262_LED_CTL_A,  0x55);
				break;
			default:
				continue;
		}

		io_cards_presents |= 1 << slot;
	}

	debug(2, "[%s]\n", io_cards_presents ? "done" : "skipped");
	return 0;
}

cvmx_helper_interface_mode_t pim_sfp_port_get_mode(int slot, int port)
{
	return cvmx_helper_interface_get_mode(((0 - (((slot ? 8 : 0) + port) - 15)) / 4) + 2);
}

void pim_leds_mgmt(int slot)
{
	static sfp_status_t sfp[MAX_IM8724_MODULES][MAX_PORTS_PER_MODULE];
	static uint8_t first_time[MAX_IM8724_MODULES] = {1, 1};
	unsigned long sfp_pres, sfp_rxlos;
	uint8_t speed_1g, speed, activity;
	int port;

	if (!(io_cards_presents & (1 << slot)))
		return;

	debug(4, "Start pim%d leds processing... ", slot);
	debug(5, "\n");
	/* Link/Act led: off - link down, green - link up, blinking green indicates activity *
	 * Speed led: off - link down, orange - 10G, yellow - 1G */
	if (first_time[slot]) {
		for (port = 0; port < GET_MAX_SLOT_PORTS(slot); port++) {
			sfp[slot][port].sfp_rxlos = -1;
			sfp[slot][port].sfp_pres = -1;
			sfp[slot][port].activity = ACTIVITY_UNDEF;
			sfp[slot][port].speed = SPEED_UNDEF;
		}
		first_time[slot] = 0;
	}

	for (port = 0; port < GET_MAX_SLOT_PORTS(slot); port++) {
		speed_1g = activity = 0;
		sfp_pres = read_pim_led_info(slot, port, SFP_PRES_FLAG);
		if (sfp_pres == 0) {
			if (sfp[slot][port].sfp_pres != sfp_pres) {
				write_pim_led_info(slot, port, LED_ACT_FLAG, LED_CNTRL_OFF);
				write_pim_led_info(slot, port, LED_COL_FLAG, LED_CLR_OFF);
				sfp[slot][port].activity = ACTIVITY_UNDEF;
				sfp[slot][port].speed = SPEED_UNDEF;
			}
		} else { // sfp_pres == 1
			speed = get_pim_interface_speed(slot, port);
			if((sfp[slot][port].sfp_pres != sfp_pres)
				&& (io_card_sign_l[slot] == IM13233_SIG))
			{
				/* Check media type, but skip sgmii nodes */
				if(speed != SPEED_1G)
					pim_port_check_updt(slot, port);

				/* Set max SFP+ rate, if this is configured */
				set_sfp_rate(slot, port, speed, 0);
			}

			sfp_rxlos = read_pim_led_info(slot, port, SFP_RXLOS_FLAG);
			if (sfp_rxlos == 1) {
				if (sfp[slot][port].sfp_rxlos != sfp_rxlos) {
					write_pim_led_info(slot, port, LED_ACT_FLAG, LED_CNTRL_OFF);
					write_pim_led_info(slot, port, LED_COL_FLAG, LED_CLR_OFF);
					sfp[slot][port].activity = ACTIVITY_UNDEF;
					sfp[slot][port].speed = SPEED_UNDEF;
				}
			} else {
				if((speed != get_sfp_rate(slot, port)) && !sfp_mm_allow) {
					speed = SPEED_UNDEF;
					activity = ACTIVITY_UNDEF;
				} else {
					speed_1g = (speed == SPEED_1G);
					activity = get_pim_interface_activity(slot, port);
				}

				if (sfp[slot][port].speed != speed) {
					write_pim_led_info(slot, port, LED_COL_FLAG,
						(speed == SPEED_UNDEF) ? LED_CLR_OFF
							: (speed_1g ? LED_CLR_YELLOW : LED_CLR_ORANGE));
					sfp[slot][port].speed = speed;
				}

				if (sfp[slot][port].activity != activity) {
					write_pim_led_info(slot, port, LED_ACT_FLAG,
						(activity == ACTIVITY_UNDEF) ? LED_CNTRL_OFF
							: (activity == 0 ? LED_CNTRL_ON : LED_CNTRL_BLINKING));
					sfp[slot][port].activity = activity;
				}
			}
			sfp[slot][port].sfp_rxlos = sfp_rxlos;
		}
		sfp[slot][port].sfp_pres = sfp_pres;

		if(sfp_pres) {
			debug(5, "  port: %d:%d, speed: %s, activity: %s\n",
				slot, port, speed_1g ? "1G" : "10G", activity ? "on" : "off");
		} else {
			debug(5, "  port: %d:%d: no SFP\n", slot, port);
		}
	}
	debug(4, "[done]\n");
}

void pim_temp_mgmt(int slot)
{
	uint8_t sasl = slot ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;

	if(cpld_rd_remote(sasl, CPLD_IM132XX(slot, MISC_CSR)) & CPLD_IM132XX(slot, TM_OVERT)) {
		uint8_t reg = cpld_rd(CPLD_ADDR_IO_PWR_CTL);
		if ((reg & CPLD_PIM_PWR_ENABLE(slot)) == CPLD_PIM_PWR_ENABLE(slot)) {
			cpld_wr(CPLD_ADDR_IO_PWR_CTL, reg & (~CPLD_PIM_PWR_ENABLE(slot)));
		}
	}
}

int fp_leds_init(void)
{
	cvmx_bgxx_cmr_global_config_t gconfig;

	if (!cpld_is_init) {
		debug(1, "Error: CPLD is not init!\n");
		return -1;
	}

	debug(2, "Start fp leds init... ");

    /* Set number of RXAUI ports */
    gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));
	fp_ports_num = gconfig.s.pmux_sds_sel ? 2 : 4;

	/* Set SW mode for RXAUI phys leds and switch off them */
	cpld_wr(CPLD_LED_CFG, 0xe);
	cpld_wr(CPLD_S0_LED_CTL, 0xcc);
	cpld_wr(CPLD_S2_LED_CTL, 0xcc);
	fp_leds_is_init = 1;

	debug(2, "[done]\n");
	return 0;
}

void fp_leds_mgmt(void)
{
	static sfp_status_t sfp[MAX_FRONTPANEL_PORTS];
	static uint8_t first_time = 1;
	static uint8_t mgmt_st = 2; // undefined for correct init
	unsigned long sfp_pres, sfp_rxlos;
	uint8_t speed_1g, speed, activity;
	int port;

	if (!fp_leds_is_init) return;

	debug(4, "Start fp leds processing... ");
	debug(5, "\n");
	if (first_time) {
		for (port = 0; port < fp_ports_num; port++) {
			sfp[port].sfp_rxlos = -1;
			sfp[port].sfp_pres = -1;
			sfp[port].speed = SPEED_UNDEF;
			sfp[port].activity = ACTIVITY_UNDEF;
		}
		first_time = 0;
	}

	{
		cvmx_bgxx_cmr_global_config_t gconfig;

		gconfig.u64 = cvmx_read_csr_node(0, CVMX_BGXX_CMR_GLOBAL_CONFIG(1));

		if (gconfig.s.pmux_sds_sel) {
			int mgmt_link_status = !!(cvmx_mdio_read(0, 0, VSC8221_MODE_STAT_REG)
							& VSC8221_MODE_LINK_STAT);
			if (mgmt_st != mgmt_link_status) {
				uint8_t mgmt_led_ctl = cpld_rd(CPLD_ST_LED_CTL);

				if (mgmt_link_status) {
					int mgmt_aux_status = cvmx_mdio_read(0, 0, VSC8221_AUX_STAT_REG);
					mgmt_aux_status &= (VSC8221_AUX_STAT_SPEED1 | VSC8221_AUX_STAT_SPEED2);
					mgmt_aux_status >>= VSC8221_AUX_SPEED_MASK;

					mgmt_led_ctl &= ~(1 << 7);
					if (2 == mgmt_aux_status) // 1000 mbps
						mgmt_led_ctl |= (1 << 6);
					else // 100 mbps or lower
						mgmt_led_ctl &= ~(1 << 6);
				} else {
					mgmt_led_ctl |= 0xc0;
				}

				cpld_wr(CPLD_ST_LED_CTL, mgmt_led_ctl);
				mgmt_st = mgmt_link_status;
			}
		}
	}

	/* Link/Act led: off - link down, green - link up, blinking green indicates activity *
	 * Speed led: off - link down, orange - 10G, yellow - 1G */
	for (port = 0; port < fp_ports_num; port++) {
		speed_1g = activity = 0;
		sfp_pres = read_fp_led_info(port, SFP_PRES_FLAG);
		if (sfp_pres == 0) {
			if (sfp[port].sfp_pres != sfp_pres) {
				write_fp_led_info(port, LED_ACT_FLAG, LED_CNTRL_OFF);
				write_fp_led_info(port, LED_COL_FLAG, LED_CLR_OFF);
				sfp[port].speed = SPEED_UNDEF;
				sfp[port].activity = ACTIVITY_UNDEF;
			}
		} else {
			speed = get_fp_interface_speed(port);
			sfp_rxlos = read_fp_led_info(port, SFP_RXLOS_FLAG);
			/* Set max SFP+ rate, if this is configured */
			if (set_max_sfp_rate && sfp[port].sfp_rxlos != sfp_rxlos)
				set_sfp_rate(-1, port, speed, 0);
			if (sfp_rxlos == 1) {
				if (sfp[port].sfp_rxlos != sfp_rxlos) {
					write_fp_led_info(port, LED_ACT_FLAG, LED_CNTRL_OFF);
					write_fp_led_info(port, LED_COL_FLAG, LED_CLR_OFF);
					sfp[port].speed = SPEED_UNDEF;
					sfp[port].activity = ACTIVITY_UNDEF;
				}
			} else {
				if((speed != get_sfp_rate(-1, port)) && !sfp_mm_allow) {
					speed = SPEED_UNDEF;
					activity = ACTIVITY_UNDEF;
				} else {
					speed_1g = (speed == SPEED_1G);
					activity = get_fp_interface_activity(port);
				}

				if (sfp[port].speed != speed) {
					write_fp_led_info(port, LED_COL_FLAG,
                                    (speed == SPEED_UNDEF) ? LED_CLR_OFF
				                  : (speed_1g ? LED_CLR_YELLOW : LED_CLR_ORANGE));
					sfp[port].speed = speed;
				}

				if (sfp[port].activity != activity) {
					write_fp_led_info(port, LED_ACT_FLAG,
						(activity == ACTIVITY_UNDEF) ? LED_CNTRL_OFF
							: (activity == 0 ? LED_CNTRL_ON : LED_CNTRL_BLINKING));
					sfp[port].activity = activity;
				}
			}
			sfp[port].sfp_rxlos = sfp_rxlos;
		}
		sfp[port].sfp_pres = sfp_pres;
		if (dbg_mode == 5 && port == 0) printf("\n");
		debug(5, "  port: %d, speed: %s, activity: %s\n",
			 port, speed_1g ? "1G" : "10G", activity ? "on" : "off");
	}
	debug(4, "[done]\n");
}

int sys_leds_init(void)
{
	uint8_t val;

    debug(2, "Start sys leds init... ");
	val = cpld_rd(CPLD_ADDR_INT_EN_0);
	cpld_wr(CPLD_ADDR_INT_EN_0, val | 0x82);
	val = cpld_rd(CPLD_ADDR_INT_EN_1);
	cpld_wr(CPLD_ADDR_INT_EN_1, val | 0x40);
        debug(2, "[done]\n");
        return 0;
}

void sys_leds_mgmt(void)
{
	static int first_time = 1;
	static uint8_t p_val[4];
	uint8_t c_val,c_val2, val;
	uint8_t wval, sasl;
	int slot;

	debug(4, "Start sys leds processing... ");
	/* status led, main board */
	c_val = ((cpld_rd(CPLD_ADDR_INT_STAT_0) & (CPLD_STAT_0_TEMP_EV | CPLD_STAT_0_OVERT_EV)))
		? SYS_LED_TEMP_EV
		: SYS_LED_OK;

	if((c_val == SYS_LED_OK)
		&& (cpld_rd(CPLD_ADDR_INT_STAT_1) & CPLD_STAT_1_FAN_EV))
		c_val = SYS_LED_FAN_EV;

	if((c_val == SYS_LED_OK) && !get_fans_lowest())
		c_val = SYS_LED_FAN_EV;

	if (first_time || p_val[0] != c_val) {
		val = cpld_rd(CPLD_ST_LED_CTL);
		val &=0xf0;
		switch(c_val) {
			case SYS_LED_TEMP_EV:
				val |= ((LED_CLR_RED << 2) | (LED_CNTRL_BLINKING));
				break;
			case SYS_LED_FAN_EV:
				val |= ((LED_CLR_RED << 2) | (LED_CNTRL_ON));
				break;
			default: // OK
				val |= ((LED_CLR_GREEN << 2) | (LED_CNTRL_ON));
				break;
		}
		cpld_wr(CPLD_ST_LED_CTL, val);
		p_val[0] = c_val;
	}

	/* status led, plugable board */
	for (slot = 0; slot < MAX_IM8724_MODULES; ++slot) {
		if (!(io_cards_presents & (1 << slot)))
			continue;
		sasl = slot ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
		val = cpld_rd_remote(sasl, CPLD_IM132XX(slot, MISC_CSR));
		c_val = (val & 0x3) ? 1 : 0;
		if (first_time || p_val[1] != c_val) {
			val = cpld_rd_remote(sasl, CPLD_IM132XX(slot, ST_LED_CTL));
			val &=0xf0;
			val |= c_val ? 0xf : 0x1;
			cpld_wr_remote(sasl, CPLD_IM132XX(slot, ST_LED_CTL), val);
			p_val[1] = c_val;
		}
	}

#define PS_PRSNT_OK  0x01
#define PS_PWR_OFF   0x00
#define PS_NOT_PRSNT 0x00

      /* power leds */
      val = cpld_rd(CPLD_ADDR_PS_CSR);

      c_val = !(val & CPLD_PS0_PRSNT) ? 0
              : (val & CPLD_PS0_OK) ? 1 : 2;
      wval = c_val == 0 ? PS_NOT_PRSNT
              : c_val == 1 ? 0x1 : PS_PWR_OFF;
      c_val2 = !(val & CPLD_PS1_PRSNT) ? 0
              : (val & CPLD_PS1_OK) ? 1 : 2;
      wval |= c_val2 == 0 ? (PS_NOT_PRSNT << 4)
              : c_val2 == 1 ? (PS_PRSNT_OK << 4) : (PS_PWR_OFF << 4);

	if (first_time || p_val[2] != c_val || p_val[3] != c_val2) {
		p_val[2] = c_val; p_val[3] = c_val2;
		cpld_wr(CPLD_PWR_LED_CTL, wval);
	}
	if (first_time)
		first_time = 0;
	debug(4, "[done]\n");
}

int leds_init(void)
{
	if (sys_leds_init() < 0)
		return -1;
	if (fp_leds_init() < 0)
		return -1;
	return pim_leds_init();
}

void leds_mgmt(void)
{
	 int slot;

	sys_leds_mgmt();
	fp_leds_mgmt();
	for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
		 if (!(io_cards_presents & (1 << slot)))
			continue;
		pim_leds_mgmt(slot);
		pim_temp_mgmt(slot); // main PIM led, also shutdown PIM in case of overheat
	}
}

static void write_fp_led_info(int port, int flag, uint8_t value)
{
	uint8_t offset, cvalue, uvalue;

	if (!(flag & (LED_ACT_FLAG | LED_COL_FLAG)) ||
				 port < 0 || port > 3)
		return;

	offset = (port < 2) ?
			 CPLD_S0_LED_CTL : CPLD_S2_LED_CTL ;
	cvalue = cpld_rd(offset);

	if ((flag & LED_ACT_FLAG)) {
		cvalue &= ~(0x3 << 4*(port%2));
		uvalue = (value & 0x3) << 4*(port%2);
	}
	else if ((flag & LED_COL_FLAG)) {
		cvalue &= ~(0x3 << (4*(port%2) + 2));
		uvalue = (value & 0x3) << (4*(port%2) + 2);
	}
	cpld_wr(offset, cvalue | uvalue);
}

static uint8_t read_fp_led_info(int port, int flag)
{
	uint8_t offset, value;

	if (port < 0 || port > 3)
		return 0xff;
	else if ((flag & SFP_PRES_FLAG))
		offset = CPLD_ADDR_SFP_PRES;
	else if ((flag & SFP_RXLOS_FLAG))
		offset = CPLD_ADDR_SFP_RX_LOS;
	else if ((flag & (LED_ACT_FLAG | LED_COL_FLAG)))
		offset = (port < 2) ?
			CPLD_S0_LED_CTL : CPLD_S2_LED_CTL;
	else
		return 0xff;

	value = cpld_rd(offset);

	if ((flag & (SFP_PRES_FLAG | SFP_RXLOS_FLAG)))
		return (value & (1 << port)) ? 1 : 0;
	else if ((flag & LED_ACT_FLAG))
		return (value & ~(0x3 << 4*(port%2)));
	return (value & ~(0x3 << (4*(port%2) + 2)));
}

static void write_pim_led_info(int slot, int port, int flag, uint8_t value)
{
	uint8_t sasl = slot ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
	uint8_t offset, cvalue, uvalue;

	if (!(flag & (LED_ACT_FLAG | LED_COL_FLAG)))
		return;

	if (port == 0 || port == 1)
		offset = CPLD_IM132XX(slot, LED_CTL_A);
	// IM13233 ports
	else if (port == 2 || port == 3)
		offset = CPLD_IM13233_LED_CTL_B;
	else if (port == 4 || port == 5)
		offset = CPLD_IM13233_LED_CTL_C;
	else if (port == 6 || port == 7)
		offset = CPLD_IM13233_LED_CTL_D;
	else
		return;

	cvalue = cpld_rd_remote(sasl, offset);

	if ((flag & LED_ACT_FLAG)) {
		cvalue &= ~(0x3 << 4*(port%2));
		uvalue = (value & 0x3) << 4*(port%2);
	}
	else if ((flag & LED_COL_FLAG)) {
		cvalue &= ~(0x3 << (4*(port%2) + 2));
		uvalue = (value & 0x3) << (4*(port%2) + 2);
	}
	cpld_wr_remote(sasl, offset, cvalue | uvalue);
}

/*
 * only for im13264
 */
static uint8_t read_pim_qsfp_rxlos(int pim, int slot)
{
	uint8_t val;

	val = cpld_i2c_read(pim ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0,
						slot,
						INFO_EEPROM_ADDR,
						QSFP_TX_RX_LOSS_OFFSET);

	return !!((val & QSFP_RX1_LOS)
			| (val & QSFP_RX2_LOS)
			| (val & QSFP_RX3_LOS)
			| (val & QSFP_RX4_LOS));
}

static uint8_t read_pim_led_info(int slot, int port, int flag)
{
	uint8_t sasl = slot ? CPLD_BA_CTL_SASL_PIM1 : CPLD_BA_CTL_SASL_PIM0;
	uint8_t offset, value;

	if (port < 0
		|| port > (GET_MAX_SLOT_PORTS(slot) - 1)
		|| slot < 0
		|| slot > (MAX_IM8724_MODULES - 1))
			return 0xff;
	else if ((flag & SFP_PRES_FLAG))
		offset = IS_CPLD_IM13264(slot) ? CPLD_IM13264_QSFP_PRES : CPLD_IM13233_SFP_PRES;
	else if ((flag & SFP_RXLOS_FLAG)) {
		if(IS_CPLD_IM13264(slot))
			return read_pim_qsfp_rxlos(slot, port);
		offset = CPLD_IM13233_SFP_RX_LOS;
	}
	else if ((flag & (LED_ACT_FLAG | LED_COL_FLAG))) {
		if (port == 0 || port == 1)
			offset = CPLD_IM132XX(slot, LED_CTL_A);
		else if (port == 2 || port == 3)
			offset = CPLD_IM13233_LED_CTL_B;
		else if (port == 4 || port == 5)
			offset = CPLD_IM13233_LED_CTL_C;
		else if (port == 6 || port == 7)
			offset = CPLD_IM13233_LED_CTL_D;
	}
	else
		return 0xff;

	value = cpld_rd_remote(sasl, offset);

	if ((flag & (SFP_PRES_FLAG | SFP_RXLOS_FLAG)))
		return (value & (1 << port)) ? 1 : 0;
	else if ((flag & LED_ACT_FLAG))
		return (value & ~(0x3 << 4*(port%2)));
	return (value & ~(0x3 << (4*(port%2) + 2)));
}

#define MAX_SLOTS_NUM			(MAX_IM8724_MODULES + 1)

typedef struct {
    uint32_t   bgx;
    uint32_t   indx;
} bgx_lane_map_t;

static bgx_lane_map_t bgx_lane_map[MAX_SLOTS_NUM][MAX_PORTS_PER_MODULE] = {
	{{ 5,  0}, /* 0, 0 */ { 5,  1}, /* 0, 1 */ { 5,  2}, /* 0, 2 */ { 5,  3}, /* 0, 3 */
	 { 4,  0}, /* 0, 4 */ { 4,  1}, /* 0, 5 */ { 4,  2}, /* 0, 6 */ { 4,  3}, /* 0, 7 */},
	{{ 3,  0}, /* 1, 0 */ { 3,  1}, /* 1, 1 */ { 3,  2}, /* 1, 2 */ { 3,  3}, /* 1, 3 */
	 { 2,  0}, /* 1, 4 */ { 2,  1}, /* 1, 5 */ { 2,  2}, /* 1, 6 */ { 2,  3}, /* 1, 7 */},
	{{ 0,  0}, /* 2, 0 */ { 0,  1}, /* 2, 1 */ { 1,  2}, /* 2, 2 */ { 1,  3}, /* 2, 3 */
	 { -1,  -1}, /* 2, 4 */ { -1,  -1}, /* 2, 5 */ { -1,  -1}, /* 2, 6 */ { -1,  -1}, /* 2, 7 */},
};

static int get_lmac_interface_activity(int slot, int slice)
{
	static lmac_pdu_stats_t lmac_stats[MAX_SLOTS_NUM][MAX_PORTS_PER_MODULE];
	uint32_t bgx, ind, node = 0, activity = 0;
	lmac_pdu_stats_t lmac;

	if (slot < 0 || slot >= MAX_SLOTS_NUM) slot = MAX_SLOTS_NUM - 1;
	if (slice < 0 || slice >= MAX_PORTS_PER_MODULE) slice = MAX_PORTS_PER_MODULE - 1;
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

static int get_pim_interface_activity(int slot, int slice)
{
	return get_lmac_interface_activity(slot, slice);
}

static int get_fp_interface_activity(int slice)
{
	return get_lmac_interface_activity(-1, slice);
}

static uint8_t get_pim_interface_speed(int slot, int slice)
{
	return IS_CPLD_IM13233(slot)
    		? (pim_sfp_port_get_mode(slot, slice) == CVMX_HELPER_INTERFACE_MODE_SGMII
			? SPEED_1G
			: SPEED_10G)
		: (IS_CPLD_IM13264(slot)
			? SPEED_40G
			: (IS_CPLD_IM13262(slot)
				? SPEED_25G
				: SPEED_10G));
}

static uint8_t get_fp_interface_speed(int port)
{
	return (cvmx_helper_interface_get_mode(port/2)
		== CVMX_HELPER_INTERFACE_MODE_SGMII)
			? SPEED_1G : SPEED_10G;
}
