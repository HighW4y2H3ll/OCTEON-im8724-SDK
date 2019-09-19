/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-helper-jtag.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx-pcie.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <i2c.h>
#include "atheros_qca8334.h"

DECLARE_GLOBAL_DATA_PTR;

static int qca8334_17_init_flag = 0;

struct qca833x_17_port_vlan_config eap7000_ref_vlan_cfg[] = {
	{	/* Port 0 */
		.vlan_mode = MODE_8021q_DISABLE,
		.port_state = PORT_STATE_FORWARDING,
		.egress_mode = EGRESS_TX_UNTOUCHED,
		.ingress_mode = INGRESS_ALL_FRAMES,
		.configure = 1,
		.learn_enable = 1,
		.ext_loopback = 0,
		.leak_arp = 0,
		.leak_multicast = 0,
		.drop_multicast = 0,
		.force_port_vlan = 0,
		.port_cvlan_priority = 0,
		.port_svlan_priority = 0,
		.port_default_cvid = 1,
		.port_default_svid = 1,
		.port_vlan_map = 4
	},
	{ },	/* No port 1 */
	{	/* Port 2 */
		.vlan_mode = MODE_8021q_DISABLE,
		.port_state = PORT_STATE_FORWARDING,
		.egress_mode = EGRESS_TX_UNTOUCHED,
		.ingress_mode = INGRESS_ALL_FRAMES,
		.configure = 1,
		.learn_enable = 1,
		.ext_loopback = 0,
		.leak_arp = 0,
		.leak_multicast = 0,
		.drop_multicast = 0,
		.force_port_vlan = 0,
		.port_cvlan_priority = 0,
		.port_svlan_priority = 0,
		.port_default_cvid = 1,
		.port_default_svid = 1,
		.port_vlan_map = 1
	},
	{	/* Port 3 */
		.vlan_mode = MODE_8021q_DISABLE,
		.port_state = PORT_STATE_FORWARDING,
		.egress_mode = EGRESS_TX_UNTOUCHED,
		.ingress_mode = INGRESS_ALL_FRAMES,
		.configure = 1,
		.learn_enable = 1,
		.ext_loopback = 0,
		.leak_arp = 0,
		.leak_multicast = 0,
		.drop_multicast = 0,
		.force_port_vlan = 0,
		.port_cvlan_priority = 0,
		.port_svlan_priority = 0,
		.port_default_cvid = 2,
		.port_default_svid = 1,
		.port_vlan_map = 0x40
	},
	{ },
	{ },
	{ /* Port 6 */
		.vlan_mode = MODE_8021q_DISABLE,
		.port_state = PORT_STATE_FORWARDING,
		.egress_mode = EGRESS_TX_UNTOUCHED,
		.ingress_mode = INGRESS_ALL_FRAMES,
		.configure = 1,
		.learn_enable = 1,
		.ext_loopback = 0,
		.leak_arp = 0,
		.leak_multicast = 0,
		.drop_multicast = 0,
		.force_port_vlan = 0,
		.port_cvlan_priority = 0,
		.port_svlan_priority = 0,
		.port_default_cvid = 2,
		.port_default_svid = 1,
		.port_vlan_map = 8
	}
};

void qca8334_17_reg_init_wan(struct phy_device *phydev)
{
	debug("%s(%s)\n", __func__, phydev->dev->name);

	/* 26: MAC6_RGMII_EN = 1
	 * 25: MAC6_RGMII_TXCLK_DELAY_EN = 1 (see bits 22-23)
	 * 24: Reserved (RGMII interface RXCLK delay on QCA8337)
	 * 22-23: MAC6_RGMII_TXCLK_DELAY_SEL = 0 (was 1)
	 * 20-21: MAC6_RGMII_RXCLK_DELAY_SEL = 0
	 * 17: PHY4_RGMII_EN = 0
	 * 11: MAC6_PHY_MII_PIPE_RXCLK_SEL = 0 (clock edge, default 0 is invert)
	 * 10: MAC6_PHY_MII_EN = 0 1 = MAC6 connected to CPU through MII
	 *     interface, PHY mode.
	 * 9: MAC6_PHY_MII_TXCLK_SEL = 0 Select invert clock output for port6
	 *    PHY mode, MII interface TXCLK
	 * 8: MAC6_PHY_MII_RXCLK_SEL = 0 Select invert clock output for port 6
	 *    PHY mode, MII interface RXCLK
	 * 7: MAC6_SGMII_EN = 0
	 * 2: MAC6_MAC_MII_EN = 0 Set if CPU connected through MII/TMII
	 *    interface, MAC mode.
	 * 1: MAC6_MAC_MII_TXCLK_SEL = 0 Set to invert clock for port 6
	 *    MAC mode, MII interface TXCLK.
	 * 0: MAC6_MAC_MII_RXCLK_SEL = 0 Set to invert clock for port 6
	 *    MAC mode, MII interface RXCLK
	 */
	qca8334_17_reg_write(phydev, S17_P6PAD_MODE_REG, 0x06000000);

	qca8334_17_reg_write(phydev, S17_P6STATUS_REG,
			     S17_PORT_STATUS_AZ_DEFAULT);

	/* SGMII control */
	qca8334_17_reg_write(phydev, S17_SGMII_CTRL_REG , 0xc74164de);

	qca833x_17_vlan_init(phydev, 0, 7, eap7000_ref_vlan_cfg);
}

/**
 * This initializes the registers needed for the SGMII and RGMII interfaces on
 * the broken Qualcomm QCA8334 switch chip used on the EAP7000 board.  Please
 * avoid using any Qualcomm/Atheros devices with SGMII since they all seem to
 * experience issues, even the QCA8033 PHY requires special work-arounds.
 *
 * @param phydev	phy device to initialize
 */
void qca8334_reg_init(struct phy_device *phydev)
{
	u32 val;
	ulong start_time;
	debug("%s(%s)\n", __func__, phydev->dev->name);

	if (qca8334_17_init_flag)
		return;

	/* Reset the PHY */
	val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	val |= S17_MASK_CTRL_SOFT_RESET;
	qca8334_17_reg_write(phydev, S17_MASK_CTRL_REG, val);

	start_time = get_timer(0);
	do {
		val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	} while (val & S17_MASK_CTRL_SOFT_RESET && get_timer(start_time) < 5000);
	if (val & S17_MASK_CTRL_SOFT_RESET) {
		printf("Could not initialize PHY %s, reset timed out (val: 0x%x)\n",
		       phydev->dev->name, val);
		return;
	}

	/* Speed: 1000M, Tx MAC enable, Rx MAC enable, Tx MAC flow enable,
	 * Rx MAC flow enable, Full duplex mode
	 */
	qca8334_17_reg_write(phydev, S17_P0STATUS_REG, 0x0000007e);

	/* Enable SGMII, Configure the receive clock phase for MAC interface
	 * for falling edge.
	 */
	qca8334_17_reg_write(phydev, S17_P0PAD_MODE_REG, 0x80080080);
	/* Bits to use for broadcast, unknown multicast and unknown unicast
	 */
	qca8334_17_reg_write(phydev, S17_GLOFW_CTRL1_REG, 0x007f7f7f);
	/* PHY mode
	 **31: Full-duplex in base-page of BASE-X for autonegotiation
	 **30: Half-duplex in base-page of BASE-X for autonegotiation
	 * 29-28: REMOTE_FAULT[1:0] in the base-page of base-x for
	 *        autonegotiation.  Generated by remote fault logic internal MAC.
	 * 27: NEXT_PAGE index in the base-page of base-x and SGMII PHY/MAC
	 *     for autonegotiation.
	 **26: PAUSE_25M Pause in the base-page of BASE-X and SGMII-PHY/MAC for
	 *     autonegotiation. This part is not included in the standard for
	 *     SGMII.
	 **25: ASYM_PAUSE_25M ASYM_PAUSE in the base-page of BASE-X and
	 *     SGMII-PHY/MAC for autonegotiation.  This part is not
	 *     included in the standard for SGMII.
	 **24: PAUSE_SG_TX_EN_25M Enable transmitting pause in the base-page of
	 *     BASE-X and SGMII-PHY/MAC for autonegotiation.
	 * 23-22: MODE_CTRL signal for mode selection among BASE-X (2'h0),
	 *        *SGMII-PHY (2'h1), and SGMII-MAC (2'h2).
	 * 21: MR_LOOPBACK, FORCE_SPEED Indicate loopback from MII register of
	 *     cooper PHY and force speed control signal.
	 * 20: MR_REG4_CH_25M Indicate register 4 has changed
	 * 19: AUTO_LPI_25M When RX_LPI_ACTIVE active for once, the register
	 *     latches this to indicate that the link-partner.
	 * 18: PRBS_EN Enable SerDes PRBS test function
	 * 17: SGMII_TH_LOS[1] Combine with bit[15], Signal detection threshold
	 *     setting control:
	 *    *00 = Default
	 *     01 = -2dB
	 *     10 = +2dB
	 *     11 = +2dB
	 **16: DIS_AUTO_LPI_25M Disable auto-detect link-partner's az ability.
	 * 15: SGMII_TH_LOS[0] Same as bit 17.
	 * 14-13: SGMII_CDR_BW CDR digital accumulator length control
	 *        00 = 0
	 *        01 = +-2
	 *        10 = +-4
	 *       *11 = +-8
	 * 12-10: SGMII_TXDR_CTRL Default value is 001
	 *        000 = Driver output Vdiff,pp = 500mV
	 *       *001 = 600mV
	 *        010 = 700mV
	 *        011 = 800mV
	 *        100 = 900mV
	 *        101 = 1V
	 *        110 = 1.1V
	 *        111 = 1.2V
	 * 9:8 SGMII_FIBER_MODE
	 *    *00 = Not in fiber mode
	 *     01 = 100BASE-FX mode
	 *     10 = reserved
	 *     11 = 1000BASE-FX mode
	 * 7: SGMII_SEL_CLK125M
	 *    0 = SGMII_CLK125M_RX_DELAY is not delayed
	 *   *1 = SGMII_CLK125M_RX_DELAY is delayed 2ns
	 * 6: SGMII_PLL_BW
	 *    0 = SGMII PLL bandwidth is low
	 *   *1 = SGMII PLL bandwidth is high (default)
	 * 5: SGMII_HALFTX
	 *   *0 = TX driver amplitude is normal (default)
	 *    1 = TX driver amplitude is half
	 * 4: SGMII_EN_SD
	 *    0 = Signal detection disabled and SGMII_FD_SDO = 0
	 *   *1 = Signal detection enabled
	 * 3: SGMII_EN_TX
	 *    0 = TX driver is in idle and kept in 900mV
	 *   *1 = TX driver enabled
	 * 2: SGMII_EN_RX
	 *    0 = RX chain disabled, CLK125M_RX and DOUT_RX could be any logic
	 *        of 0 or 1
	 *   *1 = RX chain enabled
	 * 1: SGMII_EN_PLL
	 *    0 = SGMII PLL disabled
	 *   *1 = SGMII PLL enabled
	 *
	 * 0: SGMII_EN_LCKDT
	 *   *0 = disabled (default)
	 *    1 = SGMII VCO control voltage detector and lock detector enabled
	 */
	qca8334_17_reg_write(phydev, S17_SGMII_CTRL_REG, 0xc74164de);
}

int qca8334_config(struct phy_device *phydev)
{
	int rc;

	if (!phydev->dev) {
		debug("%s: phy device not attached to Ethernet.\n", __func__);
		return 0;
	}
	debug("%s(%s)\n", __func__, phydev->dev->name);
	rc = __qca8334_config(phydev);
	if (rc)
		printf("%s: %s returned error %d\n", __func__,
		       phydev->dev->name, rc);
	else
		qca8334_17_reg_init_wan(phydev);

	return rc;
}

void board_net_preinit(void)
{
	debug("%s: Initializing switch\n", __func__);
	phy_qca8334_init();
	mdelay(100);
	debug("%s: EXIT\n", __func__);
}

void board_configure_qlms(void)
{
	/* Take PHYs out of reset */
	gpio_set_value(9, 1);

	mdelay(100);

	/* Configure QLMs */
	/* qlm, speed, mode, rc, pcie_mode, ref_clk_sel, ref_clk_input */
	octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_SGMII_DISABLED, 0, 0, 1, 1);
	octeon_configure_qlm(1, 2500, CVMX_QLM_MODE_PCIE_1X2, 1, 1, 0, 1);
	octeon_configure_qlm(2, 2500, CVMX_QLM_MODE_PCIE_1X2, 1, 1, 0, 1);

#ifdef DEBUG
	debug("%s: DLM 0 rate: %d, baud: %d\n", __func__,
	      cvmx_qlm_measure_clock(0), cvmx_qlm_get_gbaud_mhz(0));
#endif
}

int early_board_init(void)
{
	debug("%s: ENTER\n", __func__);
	/* Enable USB power */
	gpio_direction_output(1, 1);

	/* Put the PHYs in reset */
	gpio_direction_output(9, 0);

	/* Wait at least 10ms */
	mdelay(11);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(EAP7000_REF_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EAP7000_REF,
				    CONFIG_OCTEON_EAP7000_REF_MAJOR,
				    CONFIG_OCTEON_EAP7000_REF_MINOR);


	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	return 0;
}
