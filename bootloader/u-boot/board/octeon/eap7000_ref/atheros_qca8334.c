
/*
 * (C) Copyright 2014 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* This file deals with the broken Qualcomm/Atheros QCA8334/8337 switch.
 * Please avoid using these parts at all cost, or for that matter, any
 * Qualcomm/Atheros devices connected to SGMII.  All Atheros/Qualcomm devices
 * seem to have fundamental issues with SGMII.  This includes the QCA8033 PHY
 * chip as well.  These problems are not limited to just Octeon.  Normally
 * these issues require that a GPIO line be connected to the PHY or switch
 * reset line so that the device can be reset or power cycled whenever the
 * SGMII interface on the host processor is reset.
 *
 * The best solution is to just say no.  The data sheets for these devices are
 * also pretty bad, among the worst I have seen and support from Qualcomm
 * has been nonexistant.
 *
 * These switches have fundamental problems and are not really suitable for
 * use on Octeon.  They contain a lot of processing that is better done using
 * Octeon such as NAT, ACLs and QoS.  These features are not really usable with
 * Linux. Any inexpensive non-Qualcomm switch would be preferable to this
 * device.
 *
 * In order to make the QCA8337 work with Octeon the Octeon SGMII interface must
 * first be configured as 1Gbps full duplex with autonegotiation disabled.
 *
 * The PCS layer of Octeon needs to be power cycled before any Qualcomm/Atheros
 * devices will even work on SGMII.
 *
 * Only after the SGMII interface is up then autonegotiation needs to be enabled
 * on Octeon.  If the link should go down then PCS needs to be power cycled
 * again and autonegotiation disabled, only to be enabled again after the link
 * comes up.
 */

#include <common.h>
#include <phy.h>
#include <linux/mdio.h>
#include "atheros_qca8334.h"

static bool qca8334_init_flag = false;

u32 qca8334_17_reg_read(struct phy_device *phydev, u32 reg_addr)
{
	u32 reg_word_addr;
	u32 phy_addr, tmp_val, reg_val;
	u16  phy_val;
	u8 phy_reg;
	struct phy_device int_phydev;	/* Internal PHY device */

	memcpy(&int_phydev, phydev, sizeof(int_phydev));

	/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

	/* configure register high address */
	phy_addr = 0x18;
	phy_reg = 0;

	/* bits 16-8 of reg addr */
	phy_val = (u16)((reg_word_addr >> 8) & 0x1ff);

	/* Temporarily change phy address */
	int_phydev.addr = phy_addr;

	phy_write(&int_phydev, MDIO_DEVAD_NONE, phy_reg, phy_val);

	/* For some registers such as MIBs, since it is read/clear, we should
	 * read the lower 16-bit register then the higher one.
	 */
	/* read register in lower address */
	/* bit7-5 of reg address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7);
	/* bit4-0 of reg address */
	phy_reg = (u8)(reg_word_addr & 0x1f);

	int_phydev.addr = phy_addr;

	reg_val = (u32)phy_read(&int_phydev, MDIO_DEVAD_NONE, phy_reg);

	/* read register in higher address */
	reg_word_addr++;
	/* bit7-5 of reg address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7);
	/* bit4-0 of reg address */
	phy_reg = (u8) (reg_word_addr & 0x1f);

	int_phydev.addr = phy_addr;

	tmp_val = (u32)phy_read(&int_phydev, MDIO_DEVAD_NONE, phy_reg);
	reg_val |= (tmp_val << 16);

	debug("%s(%s, 0x%x) returned 0x%x\n", __func__, phydev->dev->name,
	      reg_addr, reg_val);
	return reg_val;
}

void qca8334_17_reg_write(struct phy_device *phydev, u32 reg_addr, u32 reg_val)
{
	u32 reg_word_addr;
	u32 phy_addr;
	u16  phy_val;
	u8 phy_reg;
	struct phy_device int_phydev;	/* Internal PHY device */

	debug("%s(%s, 0x%x, 0x%x)\n", __func__, phydev->dev->name,
	      reg_addr, reg_val);

	memcpy(&int_phydev, phydev, sizeof(int_phydev));

	/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

	/* configure register high address */
	phy_addr = 0x18;
	phy_reg = 0;

	/* bits 16-8 of reg addr */
	phy_val = (u16)((reg_word_addr >> 8) & 0x1ff);

	/* Temporarily change phy address */
	int_phydev.addr = phy_addr;

	phy_write(&int_phydev, MDIO_DEVAD_NONE, phy_reg, phy_val);

	/* For some registers such as ARL and VLAN, since they include BUSY bit
	 * in lower address, we should write the higher 16-bit register then the
	 * lower one
	 */

	/* write register in higher address */
	reg_word_addr++;
	/* bits 7-5 of reg address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 7);
	/* bits 4-0 of reg address */
	phy_reg = (u8)(reg_word_addr & 0x1f);
	phy_val = (u16)((reg_val >> 16) & 0xffff);
	int_phydev.addr = phy_addr;
	phy_write(&int_phydev, MDIO_DEVAD_NONE, phy_reg, phy_val);

	/* write the register in the lower address */
	reg_word_addr--;

	/* bits 7-5 of reg address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 7);
	/* bits 4-0 of reg address */
	phy_reg = (u8)(reg_word_addr & 0x1f);
	phy_val = (u16)(reg_val & 0xffff);
	phy_write(&int_phydev, MDIO_DEVAD_NONE, phy_reg, phy_val);
}

void qca8334_reg_init(struct phy_device *phydev)
	__attribute__((weak, alias("__qca8334_reg_init")));

void __qca8334_reg_init(struct phy_device *phydev)
{
	u32 val;
	ulong start_time;
	debug("%s(%s)\n", __func__, phydev->dev->name);
	if (qca8334_init_flag)
		return;

	/* Reset the PHY */
	val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	val |= S17_MASK_CTRL_SOFT_RESET;
	qca8334_17_reg_write(phydev, S17_MASK_CTRL_REG, val);
	start_time = get_timer(0) + 5000;
	do {
		val = qca8334_17_reg_read(phydev, S17_MASK_CTRL_REG);
	} while (val & S17_MASK_CTRL_SOFT_RESET && get_timer(start_time) < 5000);
	if (val & S17_MASK_CTRL_SOFT_RESET) {
		printf("Could not initialize PHY %s, reset timed out (val: 0x%x)\n",
		       phydev->dev->name, val);
		return;
	}

	/* Put port 0 in SGMII mode and set RX clock to falling edge */
	qca8334_17_reg_write(phydev, S17_P0PAD_MODE_REG, 0x80080080);

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

bool qca8334_phy_is_link_alive(struct phy_device *phydev)
{
	u16 phy_hw_status;

	phy_hw_status = phy_read(phydev, MDIO_DEVAD_NONE, ATHR_PHY_SPEC_STATUS);
	debug("%s(%s) link is%s alive\n", __func__, phydev->dev->name,
	      phy_hw_status & ATHR_STATUS_LINK_PASS ? "" : " not");
	return !!(phy_hw_status & ATHR_STATUS_LINK_PASS);
}

bool qca8334_phy_setup(struct phy_device *phydev)
{
	int phy_unit = 0;
	u16 phy_hw_status;
	u16 timeout;
	int live_links = 0;

	debug("%s(%s)\n", __func__, phydev->dev->name);
	/* See if there's any configuration data for this enet.
	 * Start autonegotiation on each phy.
	 */
	phy_write(phydev, MDIO_DEVAD_NONE, ATHR_AUTONEG_ADVERT,
		  ATHR_ADVERTISE_ALL);

	/* Advertise 1000 full duplex */
	phy_write(phydev, MDIO_DEVAD_NONE, ATHR_1000BASET_CONTROL,
		  ATHR_ADVERTISE_1000FULL);

	/* Reset PHYs */
	phy_write(phydev, MDIO_DEVAD_NONE, ATHR_PHY_CONTROL,
		  ATHR_CTRL_AUTONEGOTIATION_ENABLE | ATHR_CTRL_SOFTWARE_RESET);

	/* After the phy is reset, it takes a little while before it can
	 * respond properly.
	 */
	mdelay(1000);

	/* Wait up to 3 seconds for ALL associated PHYs to finish autonegotiation.
	 * The only way we get out of here sooner is if ALL PHYs are connected
	 * AND finish autonegotiation.
	 */
	for (timeout = 0; timeout < 20; timeout++) {
		phy_hw_status = phy_read(phydev, MDIO_DEVAD_NONE,
					 ATHR_PHY_CONTROL);
		if (ATHR_RESET_DONE(phy_hw_status)) {
			debug("port %d, negative success\n", phy_unit);
			break;
		}
		mdelay(150);
	}
	if (!ATHR_RESET_DONE(phy_hw_status)) {
		debug("port %d, negotiation timeout\n", phy_unit);
	}

	/* All PHYs have had adequate time to autonegotiate.
	 * Now initialize software status.
	 *
	 * It's possible that some ports may take a bit longer
	 * to autonegotiate; but we can't wait forever.  They'll
	 * get noticed by mv_phyCheckStatusChange during regular
	 * polling activities.
	 */
	live_links = qca8334_phy_is_link_alive(phydev);
	debug("PHY specific status: %4.4x\n", phy_read(phydev, MDIO_DEVAD_NONE,
						       ATHR_PHY_SPEC_STATUS));
	return live_links;
}


int qca8334_config(struct phy_device *phydev)
	__attribute__((weak, alias("__qca8334_config")));

int __qca8334_config(struct phy_device *phydev)
{
	debug("%s(%s)\n", __func__, phydev->dev->name);
	qca8334_reg_init(phydev);

	return 0;
}

int qca8334_startup(struct phy_device *phydev)
{
	debug("%s(%s)\n", __func__, phydev->dev->name);
	genphy_update_link(phydev);
	return 0;
}

struct phy_driver qca8334_driver = {
	.name = "atheros,qca8334",
	.uid = 0x4dd036,
	.mask = 0xfffff0,
	.features = PHY_GBIT_FEATURES,
	.config = &qca8334_config,
	.startup = &qca8334_startup,
	.shutdown = &genphy_shutdown
};

int phy_qca8334_init(void)
{
	debug("%s()\n", __func__);
	phy_register(&qca8334_driver);
	return 0;
}

/** Used to look up register addresses based on port */
struct qca833x_17_vlan_regs {
	uint16_t lookup_ctrl;	/** Lookup control register address */
	uint16_t vlan_ctrl;	/** VLAN control register addresss */
};

/** VLAN registers for each port */
static const struct qca833x_17_vlan_regs qca833x_17_vlan_regs[] = {
	{ S17_P0LOOKUP_CTRL_REG, S17_P0VLAN_CTRL0_REG },
	{ S17_P1LOOKUP_CTRL_REG, S17_P1VLAN_CTRL0_REG },
	{ S17_P2LOOKUP_CTRL_REG, S17_P2VLAN_CTRL0_REG },
	{ S17_P3LOOKUP_CTRL_REG, S17_P3VLAN_CTRL0_REG },
	{ S17_P4LOOKUP_CTRL_REG, S17_P4VLAN_CTRL0_REG },
	{ S17_P5LOOKUP_CTRL_REG, S17_P5VLAN_CTRL0_REG },
	{ S17_P6LOOKUP_CTRL_REG, S17_P6VLAN_CTRL0_REG }
};

/**
 * Initializes port-based VLANs
 *
 * @param	phy_device	PHY device handle for switch
 * @param	start_port	starting port number to configure
 * @param	num_port	number of ports to configure
 * @param	pv_config	array of num_port port vlan configurations
 *
 * @return	0 for success, -1 on error.
 */
int qca833x_17_vlan_init(struct phy_device *phydev,
			 int start_port, int num_port,
			 const struct qca833x_17_port_vlan_config *pv_config)
{
	uint32_t reg;
	int port;

	if (start_port + num_port > ARRAY_SIZE(qca833x_17_vlan_regs)) {
		printf("%s: Error: port %d out of range\n", __func__,
		       start_port + num_port);
		return -1;
	}
	for (port = start_port; port < start_port + num_port;
	     port++, pv_config++) {
		if (!pv_config->configure)
			continue;
		reg = 0;
		reg |= pv_config->drop_multicast << 31;
		reg |= pv_config->leak_unicast << 28;
		reg |= pv_config->leak_multicast << 27;
		reg |= pv_config->leak_arp << 26;
		reg |= pv_config->ext_loopback << 21;
		reg |= pv_config->learn_enable << 20;
		reg |= (pv_config->port_state & 7) << 16;
		reg |= pv_config->force_port_vlan << 10;
		reg |= (pv_config->vlan_mode & 3) << 8;
		reg |= pv_config->port_vlan_map & ~(1 << port);
		debug("%s: P%dLOOKUP_CTRL REG: 0x%x\n", __func__, port, reg);
		qca8334_17_reg_write(phydev,
				     qca833x_17_vlan_regs[port].lookup_ctrl,
				     reg);
		reg = 0;
		reg |= pv_config->port_cvlan_priority << 29;
		reg |= pv_config->port_default_cvid << 16;
		reg |= pv_config->port_svlan_priority << 13;
		reg |= pv_config->port_default_svid;
		debug("%s: P%dVLAN_CTRL0 REG: 0x%x\n", __func__, port, reg);
		qca8334_17_reg_write(phydev,
				     qca833x_17_vlan_regs[port].vlan_ctrl, reg);
	}

	return 0;
}
#if 0
void qca8334_vlan_config(struct phy_device *phydev)
	__attribute__((weak, alias("__qca8334_vlan_config")));

void __qca8334_vlan_config(struct phy_device *phydev)
{
	qca8334_17_reg_write(phydev, S17_P0LOOKUP_CTRL_REG, 0x0014007e);
	qca8334_17_reg_write(phydev, S17_P0VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P1LOOKUP_CTRL_REG, 0x0014007d);
	qca8334_17_reg_write(phydev, S17_P1VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P2LOOKUP_CTRL_REG, 0x0014007b);
	qca8334_17_reg_write(phydev, S17_P2VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P3LOOKUP_CTRL_REG, 0x00140077);
	qca8334_17_reg_write(phydev, S17_P3VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P4LOOKUP_CTRL_REG, 0x0014006f);
	qca8334_17_reg_write(phydev, S17_P4VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P5LOOKUP_CTRL_REG, 0x0014005f);
	qca8334_17_reg_write(phydev, S17_P5VLAN_CTRL0_REG, 0x10001);

	qca8334_17_reg_write(phydev, S17_P6LOOKUP_CTRL_REG, 0x0014003f);
	qca8334_17_reg_write(phydev, S17_P6VLAN_CTRL0_REG, 0x10001);
}

void qca8334_17_reg_init_wan(struct phy_device *phydev)
	__attribute__((weak, alias("__qca8334_17_reg_init_wan")));

void __qca8334_17_reg_init_wan(struct phy_device *phydev)
{
	debug("%s(%s)\n", __func__, phydev->dev->name);
#define ATH_S17_MAC0_SGMII
#ifdef ATH_S17_MAC0_SGMII
	qca8334_17_reg_write(phydev, S17_P6PAD_MODE_REG, 0x06600000);

#else
	qca8334_17_reg_write(phydev, S17_P6PAD_MODE_REG,
			     qca8334_17_reg_read(phydev,
						 S17_P6PAD_MODE_REG) | S17_MAC6_SGMII_EN);
#endif
	qca8334_17_reg_write(phydev, S17_P6STATUS_REG,
			     S17_PORT_STATUS_AZ_DEFAULT);

	qca8334_17_reg_write(phydev, S17_SGMII_CTRL_REG , 0xc74164de); /* SGMII control */

        qca8334_vlan_config(phydev);

	printf("%s done\n",__func__);
}
#endif
#if 0
#include <miiphy.h>

static void ath_read_reg(u16 reg_addr)
{
	u32 value;
	struct mii_dev *dev;
	struct phy_device pdev;

	memset(&pdev, 0, sizeof(pdev));
	dev = mdio_get_current_dev();
	if (!dev) {
		printf("No MDIO device active\n");
		return;
	}
	pdev.bus = dev;
	pdev.drv = &qca8334_driver;
	pdev.interface = PHY_INTERFACE_MODE_SGMII;

	value = qca8334_17_reg_read(&pdev, reg_addr);
}

static void ath_write_reg(u16 reg_addr, u32 value)
{
	struct mii_dev *dev;
	struct phy_device pdev;

	memset(&pdev, 0, sizeof(pdev));
	dev = mdio_get_current_dev();
	if (!dev) {
		printf("No MDIO device active\n");
		return;
	}
	pdev.bus = dev;
	pdev.drv = &qca8334_driver;
	pdev.interface = PHY_INTERFACE_MODE_SGMII;

	qca8334_17_reg_write(&pdev, reg_addr, value);
}

int do_ath(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong reg_addr;
	u32 reg_value;

	if (argc < 3)
		return CMD_RET_USAGE;

	reg_addr = simple_strtoul(argv[2], NULL, 0);

	if (argv[1][0] == 'r') {
		ath_read_reg(reg_addr);
	} else if (argv[1][0] == 'w') {
		if (argc < 4)
			return CMD_RET_USAGE;

		reg_value = simple_strtoul(argv[3], NULL, 0);
		ath_write_reg(reg_addr, reg_value);
	}
	return 0;
}

U_BOOT_CMD(qca833x, 4, 1, do_ath,
	   "Atheros/Qualcomm QCA8334/QCA8337 utility commands",
	   "read <addr>          - read 32-bit value from address\n"
	   "qca833x write <addr> <value> - write 32-bit value to address\n");
#endif
