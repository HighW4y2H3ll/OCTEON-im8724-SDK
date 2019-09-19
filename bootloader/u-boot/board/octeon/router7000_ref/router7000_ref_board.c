/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
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
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-pcsx-defs.h>
#include <asm/arch/cvmx-gmxx-defs.h>
#include <asm/arch/octeon_eth.h>
#include "atheros_qca8334.h"

DECLARE_GLOBAL_DATA_PTR;

extern struct phy_driver qca8334_driver;

/**
 * VLAN configuration for router7000 reference board.
 *
 * This board has all of the ports going to the same VLAN.
 */
struct qca833x_17_port_vlan_config router7000_ref_vlan_cfg[] = {
	{	/* Port 0 */
		.vlan_mode = MODE_8021q_DISABLE,
		.port_state = PORT_STATE_FORWARDING,
		.egress_mode = EGRESS_TX_UNTOUCHED,
		.ingress_mode = INGRESS_ALL_FRAMES,
		.configure = 1,		/** Configure port */
		.learn_enable = 1,	/** Enable MAC source learning */
		.ext_loopback = 0,	/** Disable external loopback */
		.leak_arp = 0,		/** Don't leak ARP across VLANS */
		.leak_multicast = 0,	/** Don't leak mcast across VLANS */
		.drop_multicast = 0,	/** Don't drop mcast packets */
		.force_port_vlan = 0,	/** Don't need to force. */
		.port_cvlan_priority = 0,/** Default client priority */
		.port_svlan_priority = 0,/** Default service priority */
		.port_default_cvid = 1,	/** Default client VLAN ID for port */
		.port_default_svid = 1,	/** Default service VLAN ID for port */
		.port_vlan_map = 0x7e	/** Ports in VLAN MAP */
	},
	{	/* Port 1 */
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
		.port_vlan_map = 0x7d

	},
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
		.port_vlan_map = 0x7b
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
		.port_default_cvid = 1,
		.port_default_svid = 1,
		.port_vlan_map = 0x77
	},
	{	/* Port 4 */
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
		.port_vlan_map = 0x6f
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
		.port_default_cvid = 1,
		.port_default_svid = 1,
		.port_vlan_map = 0x5f
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
		.port_default_cvid = 1,
		.port_default_svid = 1,
		.port_vlan_map = 0x3f
	}
};


/**
 * Attempt to initialize the broken Qualcomm QCA8337 switch.
 *
 * @param phydev	PHY device to initialize
 *
 * NOTE: The QCA8337 switch has a broken SGMII interface.  It requires that
 * the Octeon be initialized as forced full-duplex and only when the link comes
 * up autonegotiation must be enabled.  It is unknown what will happen if the
 * link goes down but it is predicted it will never recover.
 *
 * Please choose a different vendor than Qualcomm if at all possible since this
 * device will cause nothing but headaches to make it work properly.
 */
void qca8334_reg_init(struct phy_device *phydev)
{
	u32 val;
	ulong start_time;

	/* NOTE: It is HIGHLY RECOMMENDED that this
	 * device never be used in any other design.
	 * This device has significant issues with
	 * SGMII initialization which makes it very
	 * difficult to support.
	 * Choose a device from a different manufacturer
	 * if at all possible!
	 */
	/* Reset the PHY */
	val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	val |= S17_MASK_CTRL_SOFT_RESET;
	qca8334_17_reg_write(phydev, S17_MASK_CTRL_REG, val);
	start_time = get_timer(0);
	do {
		val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	} while (val & S17_MASK_CTRL_SOFT_RESET && get_timer(start_time < 5000));
	if (val & S17_MASK_CTRL_SOFT_RESET) {
		printf("Could not initialize PHY %s, reset timed out (val: 0x%x)\n",
		       phydev->dev->name, val);
		return;
	}

	/* Put port 0 in SGMII mode and set RX clock to falling edge */
	qca8334_17_reg_write(phydev, S17_P0PAD_MODE_REG,
			     S17_MAC0_SGMII_EN);

	/* Speed: 1000M, Tx MAC enable, Rx MAC enable, Tx MAC flow enable,
	 * Rx MAC flow enable, Full duplex mode
	 */
	qca8334_17_reg_write(phydev, S17_P0STATUS_REG, 0x0000007e);

	/* Enable SGMII, Configure the receive clock phase for MAC interface
	 * for falling edge.
	 */
	qca8334_17_reg_write(phydev, S17_P0PAD_MODE_REG, 0x00080080);
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

	qca833x_17_vlan_init(phydev, 0, 7, router7000_ref_vlan_cfg);
}

/**
 * User-defined function called when the link state changes
 *
 * @param[in]	dev		Ethernet device
 * @param 	link_state	new link state
 *
 * NOTE: This is defined as a weak function so this overrides it.
 *
 * NOTE: This is required because the Qualcomm QCA8337 switch is totally broken
 * in how SGMII works.  Please do not use Qualcomm devices if they can be
 * avoided.  The QCA8334 will not work if autonegotiation is enabled on Octeon
 * when this device is initialized.  Autonegotiation on Octeon must be enabled
 * once the link comes up in order for traffic to flow, however.  This took a
 * significant amount of trial and error in order to make it work and is a
 * total hack since any sane device would just work.
 */
void board_net_set_link(struct eth_device *dev,
			cvmx_helper_link_info_t link_state)
{
	union cvmx_pcsx_mrx_control_reg control_reg;
	int index, interface;
	struct octeon_eth_info *oct_eth_info =
		(struct octeon_eth_info *)dev->priv;

	/* We're only interested in interface 1 */
	if (oct_eth_info->interface != 1)
		return;

	index = oct_eth_info->index;
	interface = oct_eth_info->interface;

	/* For some reason the QCA8337 switch requires that we start out with
	 * autonegotiation disabled.  When the link goes up we can enable
	 * autonegotiation.  When the link does down we need to power down
	 * PCS to reset it again.
	 */
	debug("%s(%s, link: %s)\n", __func__, dev->name,
	      link_state.s.link_up ? "up" : "down");
	control_reg.u64 = cvmx_read_csr(CVMX_PCSX_MRX_CONTROL_REG(index,
								  interface));
	debug("%s: PCS%d_MR%d_CONTROL_REG: 0x%llx\n", __func__,
	      interface, index, control_reg.u64);
	control_reg.s.an_en = 1;	/* Turn autonegotiation back on */
	control_reg.s.spdmsb = 1;	/* Set speed to 1Gbps */
	control_reg.s.spdlsb = 0;
	control_reg.s.dup = 1;		/* Set full duplex */
	control_reg.s.pwr_dn = !link_state.s.link_up;	/* Power down on link down */

	cvmx_write_csr(CVMX_PCSX_MRX_CONTROL_REG(index, interface),
		       control_reg.u64);
	cvmx_read_csr(CVMX_PCSX_MRX_CONTROL_REG(index, interface));
	debug("%s: Done\n", __func__);
}

/**
 * This is another weak function that is called before networking is initialized
 */
void board_net_preinit(void)
{
	debug("%s: Initializing switch\n", __func__);
 	/* Please use a different vendor's switch in your product if at all
	 * possible.  Qualcomm devices seem to have a lot of SGMII issues.
	 */
	phy_qca8334_init();
	debug("%s: EXIT\n", __func__);
}

void board_configure_qlms(void)
{
	int i __attribute__((unused));

	/* Configure QLMs */
	octeon_configure_qlm(0, 2500, CVMX_QLM_MODE_SGMII_SGMII, 0, 0, 1, 1);
	octeon_configure_qlm(1, 5000, CVMX_QLM_MODE_PCIE_2X1, 1, 2, 0, 1);
	octeon_configure_qlm(2, 5000, CVMX_QLM_MODE_SATA_2X1, 1, 1, 1, 1);
	mdelay(10);

#ifdef DEBUG
	for (i = 0; i < 3; i++)
		printf("DLM%d reference clock: %d\n", i,
		       cvmx_qlm_measure_clock(i));
#endif
		mdelay(10);
	/* Take PHYs out of reset */
	gpio_direction_output(9, 1);
	mdelay(10);
}

int checkboard(void)
{
	/* Turn on USB ports */
	gpio_direction_output(1, 1);
	/* Overcurrent flag */
	gpio_direction_input(3);

	mdelay(10);

	return 0;
}

int early_board_init(void)
{
	debug("%s: ENTER\n", __func__);
	/* Enable USB power */
	gpio_direction_output(1, 1);

	/* Put the PHYs in reset */
	gpio_direction_output(9, 0);

	/* Wait at least 10ms */
	mdelay(10);

	/* Populate global data from eeprom */
	octeon_board_get_clock_info(ROUTER7000_REF_DEF_DRAM_FREQ);

	octeon_board_get_descriptor(CVMX_BOARD_TYPE_ROUTER7000_REF,
				    CONFIG_OCTEON_ROUTER7000_REF_MAJOR,
				    CONFIG_OCTEON_ROUTER7000_REF_MINOR);


	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	octeon_board_get_mac_addr();

	return 0;
}

#include <miiphy.h>
int do_qca833x(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static struct phy_device *phydev = NULL;
	uint32_t reg;
	uint32_t val;

	if (phydev == NULL)
		phydev = mdio_phydev_for_ethname("octeth1");
	if (!phydev) {
		printf("Error: could not get PHY device for octeth1\n");
		return CMD_RET_FAILURE;
	}

	if (argv[1][0] == 'd') {
		if (argc == 3) {
			phydev = mdio_phydev_for_ethname(argv[2]);
			if (!phydev) {
				printf("Could not find device %s\n", argv[2]);
				return CMD_RET_FAILURE;
			}
		} else {
			mdio_list_devices();
			printf("Current device: %s\n", phydev->dev->name);
		}
	} else if (argv[1][0] == 'r') {
		if (argc != 3)
			return CMD_RET_USAGE;
		reg = simple_strtoul(argv[2], NULL, 0);
		val = qca8334_17_reg_read(phydev, reg);
		printf("%s: r 0x%x: 0x%x\n", phydev->dev->name, reg, val);
	} else if (argv[1][0] == 'w') {
		if (argc != 4)
			return CMD_RET_USAGE;
		reg = simple_strtoul(argv[2], NULL, 0);
		val = simple_strtoul(argv[3], NULL, 0);
		qca8334_17_reg_write(phydev, reg, val);
		printf("%s: w 0x%x: 0x%x\n", phydev->dev->name, reg, val);
	} else {
		return CMD_RET_USAGE;
	}
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(qca833x, 4, 1, do_qca833x,
	   "Atheros/Qualcomm QCA8334/QCA8337 utility commands",
	   "dev <eth device>	- list eth devices if eth device is blank or set to use eth device\n"
	   "qca833x read <reg>          - read register <reg>\n"
	   "qca833x write <reg> <value> - write <val> to register <reg>\n");
