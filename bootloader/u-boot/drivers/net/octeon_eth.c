/***********************license start************************************
 * Copyright (c) 2005-2015 Cavium, Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

/**
 *
 * $Id: octeon_eth.c 167352 2017-10-23 20:47:37Z cchavva $
 *
 */

#include <common.h>
#include <command.h>
#include <exports.h>
#include <malloc.h>
#include <linux/ctype.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/octeon_eeprom_types.h>
#include <net.h>
#include <phy.h>
#include <miiphy.h>
#include <asm/arch/octeon_eth.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_eeprom_types.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_fdt.h>
#include <asm/arch/cvmx-config.h>
#include <asm/arch/cvmx-wqe.h>
#include <asm/arch/cvmx-fpa.h>
#include <asm/arch/cvmx-pow.h>
#include <asm/arch/cvmx-pko.h>
#include <asm/arch/cvmx-ipd.h>
#include <asm/arch/cvmx-pip.h>
#include <asm/arch/cvmx-spi.h>
#include <asm/arch/cvmx-mdio.h>
#include <asm/arch/cvmx-xcv-defs.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-mgmt-port.h>
#include <asm/arch/cvmx-helper-cfg.h>
#include <asm/arch/cvmx-helper-bgx.h>
#include <asm/arch/cvmx-helper-util.h>
#include <asm/arch/cvmx-bootmem.h>
#include <asm/arch/cvmx-global-resources.h>
#ifdef CONFIG_PHY_CORTINA
# include <cortina.h>
# include <cortina_cs4321.h>
#endif
#include <asm/gpio.h>
#include <i2c.h>
#include <fdtdec.h>
#ifdef CONFIG_PHY_SFP
# include <sfp.h>
# include <asm/arch/octeon_sfp.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/**
 * Enables RX packet debugging if octeon_debug_rx_packets is set in the
 * environment.
 */
#define DEBUG_RX_PACKET

/**
 * Enables TX packet debugging if octeon_debug_tx_packets is set in the
 * environment.
 */
#define DEBUG_TX_PACKET


/* Global flag indicating common hw has been set up */
int octeon_global_hw_inited = 0;

int octeon_eth_send(struct eth_device *dev, void *packet, int length);
int octeon_eth_init(struct eth_device *dev, bd_t * bis);
void octeon_eth_enable(struct eth_device *dev);
int octeon_eth_write_hwaddr(struct eth_device *dev);

/************************************************************************/
#if  defined(CONFIG_OCTEON_INTERNAL_ENET) || defined(CONFIG_OCTEON_MGMT_ENET)

/* Variable shared between ethernet routines. */
static int card_number;
static int agl_number = 0;
static int available_mac_count = -1;
static uint8_t mac_addr[6];
#if defined(DEBUG_RX_PACKET)  || defined(DEBUG_TX_PACKET)
static int packet_rx_debug = 0, packet_tx_debug = 0;
#endif

/* Make sure that we have enough buffers to keep prefetching blocks happy.
 * Absolute minimum is probably about 200.
 */
#define NUM_PACKET_BUFFERS  1000

#define PKO_SHUTDOWN_TIMEOUT_VAL     100
/****************************************************************/

/* Define the offsets from the base CSR */
#define GMX_PRT_CFG			0x10

#define GMX_RX_FRM_MAX			0x30
#define GMX_RX_JABBER			0x38

#define GMX_RX_ADR_CTL			0x100
#define GMX_RX_ADR_CAM_EN		0x108
#define GMX_RX_ADR_CAM0			0x180
#define GMX_RX_ADR_CAM1			0x188
#define GMX_RX_ADR_CAM2			0x190
#define GMX_RX_ADR_CAM3			0x198
#define GMX_RX_ADR_CAM4			0x1a0
#define GMX_RX_ADR_CAM5			0x1a8
#define GMX_TX_OVR_BP			0x4c8

/**
 * This function is called just before initializing the Ethernet interfaces
 * for any board-specific initialization.
 *
 * @param dev		Ethernet device
 *
 * @return		0 for success, otherwise error.
 */
int octeon_eth_board_pre_init(struct eth_device *dev)
{
	return 0;
}
int octeon_eth_board_pre_init(struct eth_device *dev)
__attribute__((weak));

int octeon_eth_rx_packet_hook(struct eth_device *dev) __attribute__((weak));
int octeon_eth_rx_packet_hook(struct eth_device *dev)
{
	return 0;
}

int octeon_eth_tx_packet_hook(struct eth_device *dev) __attribute__((weak));
int octeon_eth_tx_packet_hook(struct eth_device *dev)
{
	return 0;
}

/**
 * Weak function called to see if we need to check for mod_abs.  This is needed
 * when mod_abs is connected to an i2c GPIO expander with the expander's GPIO
 * pin hooked up so we don't unnecessarily probe the GPIO expander and slow
 * everything down.
 *
 * @param	dev	Ethernet device to check
 *
 * @return	0 if we should not check the device, otherwise check it
 */
int board_octeon_eth_check_mod_abs_interrupt(struct eth_device *dev)
{
	return 1;
}
int board_octeon_eth_check_mod_abs_interrupt(struct eth_device *dev)
	__attribute__((weak));

/**
 * This function is called just after initializing the Ethernet interfaces
 * for any board-specific initialization.
 *
 * @param dev		Ethernet device
 *
 * @return		0 for success, otherwise error.
 */
int octeon_eth_board_post_init(struct eth_device *dev)
{
	return 0;
}
int octeon_eth_board_post_init(struct eth_device *dev) __attribute__((weak));

#ifdef CONFIG_PHY_CORTINA
/**
 * This function is called for boards with a Cortina CS4321 PHY in
 * order to configure the LEDs.
 */
int __board_configure_cs4321_leds(struct phy_device *phydev)
{
	return 0;
}
int board_configure_cs4321_leds(struct phy_device *phydev)
__attribute__((weak, alias("__board_configure_cs4321_leds")));
#endif

/**
 * Searches for an ethernet device based on interface and index.
 *
 * @param interface - interface number to search for
 * @param index - index to search for
 *
 * @returns pointer to ethernet device or NULL if not found.
 */
struct eth_device *octeon_find_eth_by_interface_index(int interface, int index)
{
	struct eth_device *ethdev, *start;
	struct octeon_eth_info *oct_eth_info;

	start = eth_get_dev();
	if (!start)
		return NULL;
	ethdev = start;
	do {
		oct_eth_info = (struct octeon_eth_info *)ethdev->priv;
		if ((oct_eth_info->interface == interface) &&
		    (oct_eth_info->index == index))
			return oct_eth_info->ethdev;
		ethdev = ethdev->next;
	} while (ethdev && ethdev != start);
	return NULL;
}

static void cvm_oct_fill_hw_memory(uint64_t pool, uint64_t size,
				   uint64_t elements)
{
	static int alloc_count = 0;
	char tmp_name[64];

	debug("cvm_oct_fill_hw_memory: pool: 0x%llx, size: 0xx%llx, "
	      "count: 0x%llx\n", pool, size, elements);
	sprintf(tmp_name, "%s_fpa_alloc_%d",
		OCTEON_BOOTLOADER_NAMED_BLOCK_TMP_PREFIX, alloc_count++);
	cvmx_fpa_setup_pool(pool, tmp_name, NULL, size, elements);
}

/**
 * User-defined function called when the link state changes
 *
 * @param[in]	dev		Ethernet device
 * @param 	link_state	new link state
 *
 * NOTE: This is defined as a weak function.
 */
void board_net_set_link(struct eth_device *dev,
			cvmx_helper_link_info_t link_state)
{

}
void board_net_set_link(struct eth_device *dev,
			cvmx_helper_link_info_t link_state)
	__attribute__((weak));

/**
 * Converts the interface mode to the phy mode
 *
 * @param if_mode - interface mode
 *
 * @return phy mode
 */
static phy_interface_t
cvm_if_mode_to_phy_mode(cvmx_helper_interface_mode_t if_mode)
{
	switch (if_mode) {
	case CVMX_HELPER_INTERFACE_MODE_RGMII:
		return PHY_INTERFACE_MODE_RGMII;
	case CVMX_HELPER_INTERFACE_MODE_GMII:
		return PHY_INTERFACE_MODE_GMII;
	case CVMX_HELPER_INTERFACE_MODE_SGMII:
		return PHY_INTERFACE_MODE_SGMII;
	case CVMX_HELPER_INTERFACE_MODE_XAUI:
		return PHY_INTERFACE_MODE_XAUI;
	case CVMX_HELPER_INTERFACE_MODE_RXAUI:
		return PHY_INTERFACE_MODE_RXAUI;
	case CVMX_HELPER_INTERFACE_MODE_XLAUI:
		return PHY_INTERFACE_MODE_XLAUI;
	case CVMX_HELPER_INTERFACE_MODE_XFI:
		return PHY_INTERFACE_MODE_XFI;
	case CVMX_HELPER_INTERFACE_MODE_10G_KR:
		return PHY_INTERFACE_MODE_10G_KR;
	case CVMX_HELPER_INTERFACE_MODE_40G_KR4:
		return PHY_INTERFACE_MODE_40G_KR4;
	case CVMX_HELPER_INTERFACE_MODE_MIXED:
		return PHY_INTERFACE_MODE_XGMII;
	case CVMX_HELPER_INTERFACE_MODE_AGL:
		return PHY_INTERFACE_MODE_RGMII;
	case CVMX_HELPER_INTERFACE_MODE_QSGMII:
		return PHY_INTERFACE_MODE_QSGMII;
	default:
		return PHY_INTERFACE_MODE_NONE;
	}
}

/**
 * Set the hardware MAC address for a device
 *
 * @param interface    interface of port to set
 * @param index    index of port to set MAC address for
 * @param addr   Address structure to change it too.
 * @return Zero on success
 */
static int cvm_oct_set_mac_address(struct eth_device *dev)
{
	struct octeon_eth_info *priv = (struct octeon_eth_info *)dev->priv;
	cvmx_gmxx_prtx_cfg_t gmx_cfg;
	uint64_t gmx_reg;
	cvmx_helper_interface_mode_t mode;
	cvmx_gmxx_rxx_adr_ctl_t control;
	int i;
	uint8_t *ptr = (uint8_t *)dev->enetaddr;
	int interface = priv->interface;
	int index = priv->index;
	uint64_t mac = 0;
	int xipd_port;

	for (i = 0; i < 6; i++)
		mac = (mac << 8) | (uint64_t)(ptr[i]);

	debug("%s(%s (%pM))\n", __func__, dev->name, ptr);
	mode = cvmx_helper_interface_get_mode(interface);

	/* It's rather expensive to change the MAC address for BGX so we only
	 * do this if it has changed or not been set previously.
	 */
	if (octeon_has_feature(OCTEON_FEATURE_BGX)) {
		xipd_port = cvmx_helper_get_ipd_port(interface, index);
		if (priv->last_bgx_mac != mac || !priv->bgx_mac_set) {
			cvmx_helper_bgx_set_mac(xipd_port, 1, 2, mac);
			priv->last_bgx_mac = mac;
			priv->bgx_mac_set = 1;
		}
		return 0;
	}

	if (mode == CVMX_HELPER_INTERFACE_MODE_AGL) {
		gmx_reg = CVMX_AGL_GMX_RXX_INT_REG(0);
	} else {
		gmx_reg = CVMX_GMXX_RXX_INT_REG(index, interface);
		cvmx_write_csr(CVMX_GMXX_SMACX(index, interface), mac);
	}

	/* Disable interface */
	gmx_cfg.u64 = cvmx_read_csr(gmx_reg + GMX_PRT_CFG);
	cvmx_write_csr(gmx_reg + GMX_PRT_CFG, gmx_cfg.u64 & ~1ull);
	debug("%s: gmx reg: 0x%llx\n", __func__, gmx_reg);

	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM0, ptr[0]);
	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM1, ptr[1]);
	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM2, ptr[2]);
	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM3, ptr[3]);
	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM4, ptr[4]);
	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM5, ptr[5]);

	control.u64 = 0;
	control.s.bcst = 1;	/* Allow broadcast MAC addresses */
	control.s.mcst = 1;	/* Force reject multicast packets */
	control.s.cam_mode = 1;	/* Filter packets based on the CAM */

	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CTL, control.u64);

	cvmx_write_csr(gmx_reg + GMX_RX_ADR_CAM_EN, 1);

	/* Return interface to previous enable state */
	cvmx_write_csr(gmx_reg + GMX_PRT_CFG, gmx_cfg.u64);

	return 0;
}

/**
 * Configure common hardware for all interfaces
 */
static void cvm_oct_configure_common_hw(void)
{
	int mru = getenv_ulong("octeon_mru", 0, CONFIG_OCTEON_NETWORK_MRU);
	int packet_pool_size = CVMX_FPA_PACKET_POOL_SIZE;

	debug("%s()\n", __func__);

	if (mru > packet_pool_size)
		packet_pool_size =
			(mru + CVMX_CACHE_LINE_SIZE - 1) &
				~(CVMX_CACHE_LINE_SIZE - 1);

	if (getenv("disable_spi")) {

		/* Do this here so that we can disabled this after boot but
		 * before we use networking.
		 */
		cvmx_spi_callbacks_t spi_callbacks_struct;
		cvmx_spi_get_callbacks(&spi_callbacks_struct);
		spi_callbacks_struct.reset_cb = 0;
		spi_callbacks_struct.calendar_setup_cb = 0;
		spi_callbacks_struct.clock_detect_cb = 0;
		spi_callbacks_struct.training_cb = 0;
		spi_callbacks_struct.calendar_sync_cb = 0;
		spi_callbacks_struct.interface_up_cb = 0;
		printf("SPI interface disabled with 'disable_spi' environment "
		       "variable\n");
		cvmx_spi_set_callbacks(&spi_callbacks_struct);
	}
	/* Setup the FPA */
	cvmx_fpa_enable();

	cvm_oct_fill_hw_memory(CVMX_FPA_WQE_POOL, CVMX_FPA_WQE_POOL_SIZE,
			       NUM_PACKET_BUFFERS);
#if CVMX_FPA_OUTPUT_BUFFER_POOL != CVMX_FPA_PACKET_POOL
	if (!octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		cvm_oct_fill_hw_memory(CVMX_FPA_OUTPUT_BUFFER_POOL,
				       CVMX_FPA_OUTPUT_BUFFER_POOL_SIZE, 128);
	}
#endif
	cvm_oct_fill_hw_memory(CVMX_FPA_PACKET_POOL, packet_pool_size,
			       NUM_PACKET_BUFFERS);

	cvmx_helper_initialize_packet_io_global();
	cvmx_helper_initialize_packet_io_local();

	/* The MRU defaults to 1536 bytes by the hardware.  Setting
	 * CONFIG_OCTEON_NETWORK_MRU allows this to be overridden.
	 */
	if (octeon_has_feature(OCTEON_FEATURE_PKI)) {
		struct cvmx_pki_global_config gbl_cfg;
		int i;

		cvmx_pki_read_global_config(0, &gbl_cfg);
		for (i = 0; i < CVMX_PKI_NUM_FRAME_CHECK; i++)
			gbl_cfg.frm_len[i].maxlen = mru;
		cvmx_pki_write_global_config(0, &gbl_cfg);
	}

	/* Set POW get work timeout to maximum value */
	if (octeon_has_feature(OCTEON_FEATURE_CN68XX_WQE) ||
	    octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE))
		cvmx_write_csr(CVMX_SSO_NW_TIM, 0x3ff);
	else
		cvmx_write_csr(CVMX_POW_NW_TIM, 0x3ff);
}

/**
 * Packet transmit
 *
 * @param skb    Packet to send
 * @param dev    Device info structure
 * @return Always returns zero
 */
static int cvm_oct_xmit(struct eth_device *dev, void *packet, int len)
{
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
	cvmx_pko_command_word0_t pko_command;
	cvmx_buf_ptr_t hw_buffer;
	int rv;
	int queue = cvmx_pko_get_base_queue(priv->port);

	debug("cvm_oct_xmit addr: %p, len: %d\n", packet, len);

	hw_buffer.u64 = 0;
	hw_buffer.s.addr = cvmx_ptr_to_phys(packet);
	hw_buffer.s.pool = CVMX_FPA_PACKET_POOL;
	hw_buffer.s.size = len;
	hw_buffer.s.back = 0;    //XXX- * 128; check!!

	/* Build the PKO command */
	pko_command.u64 = 0;
	pko_command.s.subone0 = 1;
	pko_command.s.dontfree = 0;
	pko_command.s.segs = 1;
	pko_command.s.total_bytes = len;
	/* Send the packet to the output queue */

	debug("cvm_oct_xmit port: %d, queue: %d\n", priv->port, queue);
	cvmx_pko_send_packet_prepare(priv->port, queue, 0);
	rv = cvmx_pko_send_packet_finish (priv->port, queue, pko_command,
					  hw_buffer, 0);
	if (rv) {
		printf("Failed to send the packet rv=%d\n", rv);
	}
	return 0;
}

/**
 * Registers a function to be called when the link goes down.  The function is
 * often used for things like reading the SFP+ EEPROM.
 *
 * @param	dev		Ethernet device
 * @param	phy_port_check	Function to call
 */
void octeon_eth_register_phy_port_check(struct eth_device *dev,
			int (*phy_port_check)(struct phy_device *dev))
{
	struct octeon_eth_info *oct_eth_info = dev->priv;

	oct_eth_info->phy_port_check = phy_port_check;
}

/**
 * Checks for state changes with the link state or module state
 *
 * @param	dev	Ethernet device to check
 *
 * NOTE: If the module state is changed then the module callback is called.
 */
void octeon_phy_port_check(struct eth_device *dev)
	__attribute__((weak, alias("__octeon_phy_port_check")));

void __octeon_phy_port_check(struct eth_device *dev)
{
	struct octeon_eth_info *oct_eth_info = dev->priv;
	struct phy_device *phydev = oct_eth_info->phydev;

#if defined(CONFIG_PHY_SFP)
	struct octeon_sfp_info *sfp = &oct_eth_info->sfp;
	int mod_abs = 1;	/* Assume module is absent */

	/* If there's been no interrupt then don't bother checking */
	if (!board_octeon_eth_check_mod_abs_interrupt(dev))
		return;

	if (sfp->check_mod_abs)
		mod_abs = sfp->check_mod_abs(dev, sfp->mod_abs_data);

	if ((!!mod_abs) != sfp->last_mod_abs && sfp->mod_abs_changed) {
		debug("%s(%s): mod_abs changed from %d to %d\n", __func__,
		      dev->name, sfp->last_mod_abs, mod_abs);
		sfp->mod_abs_changed(dev, mod_abs, sfp->mod_abs_changed_data);
	}

	if (oct_eth_info->phy_port_check && !mod_abs && sfp->last_mod_abs) {
		debug("%s: Checking port, mod_abs: %d\n", __func__, mod_abs);
		oct_eth_info->phy_port_check(phydev);
	}
#if 0
	else {
		if (oct_eth_info->phy_port_check)
			oct_eth_info->phy_port_check(phydev);
	}
#endif
	sfp->last_mod_abs = mod_abs;
#else
	if (oct_eth_info->phy_port_check)
		oct_eth_info->phy_port_check(phydev);
#endif
}

/**
 * Configure the RGMII port for the negotiated speed
 *
 * @param dev    Linux device for the RGMII port
 */
static void cvm_oct_configure_rgmii_speed(struct eth_device *dev)
{
	struct octeon_eth_info *priv = (struct octeon_eth_info *)(dev->priv);
	int port = priv->port;
	cvmx_helper_link_info_t link_state = cvmx_helper_link_get(port);

	/* If the port is down some PHYs we need to check modules, etc. */
	if (!link_state.s.link_up)
		octeon_phy_port_check(dev);

	if (link_state.u64 != priv->link_state) {
		cvmx_helper_interface_mode_t mode;

		octeon_phy_port_check(dev);

		debug("%s(%s): Link state changed\n", __func__, dev->name);
		printf("%s: ", dev->name);
		if (!link_state.s.link_up) {
			puts("Down ");
		} else {
			printf("Up %d Mbps ", link_state.s.speed);
			if (link_state.s.full_duplex)
				puts("Full duplex ");
			else
				puts("Half duplex ");
		}
		mode = cvmx_helper_interface_get_mode(priv->interface);
		printf("(port %2d) (%s)\n", port,
			cvmx_helper_interface_mode_to_string(mode));
		debug("%s: Setting link state\n", __func__);
		cvmx_helper_link_set(priv->port, link_state);
		priv->link_state = link_state.u64;

		board_net_set_link(dev, link_state);
	}
}

#if defined(CONFIG_OCTEON_MGMT_ENET)
/**
 * Really shutdown the management ports, including resetting the hardware
 * units.  This is needed for NAND boot stage2 -> stage3 handoff.  The MGMT
 * ports need to be reset before they are handed off to stage 3.
 * This is not needed with newer MGMT init code that properly handles this
 * case at init time.
 */
int octeon_mgmt_port_shutdown(void)
{
	cvmx_mgmt_port_shutdown(0);
	cvmx_mgmt_port_shutdown(1);
	cvmx_bootmem_free_named("cvmx_mgmt_port");
	return 0;
}
#endif /* CONFIG_OCTEON_MGMT_ENET */

/**
 * Shuts down the network hardware before loading new applications
 * or operating systems.
 *
 * @param mode  If set to 1, free up all buffers and named block.
 */
int octeon_network_hw_shutdown(int mode)
{
	int retval = 0;

	if (octeon_global_hw_inited) {
		debug("%s: Shutting down packet I/O and FPA pools.\n", __func__);
		retval = cvmx_helper_shutdown_packet_io_global();
		cvmx_helper_shutdown_packet_io_local();
		if (mode) 
			cvmx_helper_shutdown_fpa_pools(0);
		/* Spurious FPE errors will happen doing this cleanup. */
		if (OCTEON_IS_MODEL(OCTEON_CN68XX)) {
			cvmx_sso_err_t sso_err;
			sso_err.u64 = cvmx_read_csr(CVMX_SSO_ERR);
			sso_err.s.fpe = 1;
			cvmx_write_csr(CVMX_SSO_ERR, sso_err.u64);
		} else if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
			cvmx_sso_err0_t sso_err0;
			sso_err0.u64 = cvmx_read_csr(CVMX_SSO_ERR0);
			sso_err0.s.fpe = 1;
			cvmx_write_csr(CVMX_SSO_ERR0, sso_err0.u64);
		}
	}
	if (mode) {
		debug("%s: Freeing global resources\n", __func__);
		free_global_resources();
		/* Free command queue named block, as we have reset the hardware */
		cvmx_bootmem_phy_named_block_free("cvmx_cmd_queues", 0);
	} else {
		int i;
		int num_ints = cvmx_helper_get_number_of_interfaces();
		octeon_global_hw_inited = 0;
		for (i = 0; i < num_ints; i++) {
			struct eth_device *dev;
			dev = eth_get_dev_by_index(i);
			if (dev) {
				struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
				priv->initted_flag = 0;
			}
		}		
	}
	if (retval < 0) {
		/* Make this a fatal error since the error message is easily
		 * missed and ignoring it can lead to very strange networking
		 * behavior in the application.
		 */
		printf("FATAL ERROR: Network shutdown failed.  "
		       "Please reset the board.\n");
		while (1)
			;
	}
	return retval;
}

/*******************  Octeon networking functions ********************/
/**
 * Enables Ethernet devices to allow packets to be transmitted and received.
 * For example, this is activated when the DHCP command is issued.
 *
 * @param	dev	Ethernet device to initialize
 * @param	bis	board data structure, not used.
 *
 * @return	1 for success
 */
int octeon_eth_init(struct eth_device *dev, bd_t *bis)
{				/* Initialize the device  */
	struct octeon_eth_info *priv = (struct octeon_eth_info *)(dev->priv);

	debug("%s(), dev_ptr: %p, dev: %s, port: %d\n",
	      __func__, dev, dev->name, priv->port);

	if (priv->initted_flag) {
		debug("%s already initialized\n", dev->name);
		return 1;
	}

	if (!octeon_global_hw_inited) {
		debug("Initializing common hardware\n");
		cvm_oct_configure_common_hw();
	}

	/* Ignore backpressure on RGMII ports */
	if (!octeon_has_feature(OCTEON_FEATURE_BGX))
		cvmx_write_csr(priv->gmx_base + GMX_TX_OVR_BP, 0xf << 8 | 0xf);

	debug("%s: Setting MAC address\n", __func__);
	cvm_oct_set_mac_address(dev);

	if (!octeon_global_hw_inited) {
		debug("Enabling packet input\n");
		cvmx_helper_ipd_and_packet_input_enable();
		octeon_global_hw_inited = 1;
	}
	priv->enabled = 0;
	priv->initted_flag = 1;

	debug("%s exiting successfully\n", __func__);
	return 1;
}

#ifdef CONFIG_OCTEON_SPI4000_ENET
/**
 * Initializes the SPI4000 interface and devices
 *
 * @param dev - ethernet device to init
 *
 * @param bis - pointer to bd_t data structure
 *
 * @return 1 for success, 0 for error
 */
int octeon_spi4000_init(struct eth_device *dev, bd_t * bis)
{				/* Initialize the device  */
	struct octeon_eth_info *priv = (struct octeon_eth_info *)(dev->priv);
	static char spi4000_inited[2] = { 0 };
	debug("octeon_spi4000_init(), dev_ptr: %p, port: %d\n",
	      dev, priv->port);

	if (priv->initted_flag)
		return 1;

	if (!octeon_global_hw_inited) {
		cvm_oct_configure_common_hw();
	}

	if (!spi4000_inited[priv->interface]) {
		spi4000_inited[priv->interface] = 1;
		if (__cvmx_helper_spi_enable(priv->interface) < 0) {
			printf("ERROR initializing spi4000 on Octeon "
			       "Interface %d\n", priv->interface);
			return 0;
		}
	}

	if (!octeon_global_hw_inited) {
		cvmx_helper_ipd_and_packet_input_enable();
		octeon_global_hw_inited = 1;
	}
	priv->initted_flag = 1;
	return 1;
}
#endif

/**
 * Enables and disables the XCV RGMII interface
 *
 * @param	interface	Interface number
 * @param	index		Port index (should be 0 for RGMII)
 * @param	enable		True to enable it, false to disable it
 */
static void octeon_bgx_xcv_rgmii_enable(int interface, int index, bool enable)
{
	union cvmx_xcv_reset xcv_reset;
	debug("%s(%d, %d, %sable)\n", __func__, interface, index,
	      enable ? "en" : "dis");
	xcv_reset.u64 = cvmx_read_csr(CVMX_XCV_RESET);
	xcv_reset.s.rx_pkt_rst_n = enable ? 1 : 0;
	cvmx_write_csr(CVMX_XCV_RESET, xcv_reset.u64);
}

/**
 * Enables a SGMII interface
 *
 * @param dev - Ethernet device to initialize
 */
void octeon_eth_sgmii_enable(struct eth_device *dev)
{
	struct octeon_eth_info *oct_eth_info;
	cvmx_gmxx_prtx_cfg_t gmx_cfg;
	int index, interface;
	cvmx_helper_interface_mode_t if_mode;

	oct_eth_info = (struct octeon_eth_info *) dev->priv;

	interface = oct_eth_info->interface;
	index = oct_eth_info->index;

	debug("%s(%s) (%d.%d)\n", __func__, dev->name, interface, index);
	if (octeon_has_feature(OCTEON_FEATURE_BGX)) {
		cvmx_bgxx_cmrx_config_t cmr_config;

		cmr_config.u64 = cvmx_read_csr(CVMX_BGXX_CMRX_CONFIG(index,
								     interface));
		cmr_config.s.enable = 1;
		cmr_config.s.data_pkt_tx_en = 1;
		cmr_config.s.data_pkt_rx_en = 1;
		cvmx_write_csr(CVMX_BGXX_CMRX_CONFIG(index, interface),
			       cmr_config.u64);
		mdelay(100);
		if (cvmx_helper_bgx_is_rgmii(interface, index))
			octeon_bgx_xcv_rgmii_enable(interface, index, true);
	} else {

		if_mode = cvmx_helper_interface_get_mode(interface);
		/* Normal operating mode. */

		if (if_mode == CVMX_HELPER_INTERFACE_MODE_SGMII ||
		    if_mode == CVMX_HELPER_INTERFACE_MODE_QSGMII) {
			cvmx_pcsx_miscx_ctl_reg_t pcsx_miscx_ctl_reg;
			debug("  if mode: (Q)SGMII\n");
			pcsx_miscx_ctl_reg.u64 =
				cvmx_read_csr(CVMX_PCSX_MISCX_CTL_REG(index,
								      interface));
			pcsx_miscx_ctl_reg.s.gmxeno = 0;
			cvmx_write_csr(CVMX_PCSX_MISCX_CTL_REG(index, interface),
			       				pcsx_miscx_ctl_reg.u64);
		} else if (if_mode != CVMX_HELPER_INTERFACE_MODE_AGL) {
			cvmx_pcsxx_misc_ctl_reg_t pcsxx_misc_ctl_reg;

			debug("  if mode: AGM\n");
			pcsxx_misc_ctl_reg.u64 =
				cvmx_read_csr(CVMX_PCSXX_MISC_CTL_REG(interface));
			pcsxx_misc_ctl_reg.s.gmxeno = 0;
			cvmx_write_csr(CVMX_PCSXX_MISC_CTL_REG(interface),
			       	       pcsxx_misc_ctl_reg.u64);
		}

		gmx_cfg.u64 = cvmx_read_csr(oct_eth_info->gmx_base + GMX_PRT_CFG);
		gmx_cfg.s.en = 1;
		cvmx_write_csr(oct_eth_info->gmx_base + GMX_PRT_CFG, gmx_cfg.u64);
		gmx_cfg.u64 = cvmx_read_csr(oct_eth_info->gmx_base + GMX_PRT_CFG);
	}
}

/**
 * Enables an Ethernet interface
 *
 * @param dev - Ethernet device to enable
 */
void octeon_eth_enable(struct eth_device *dev)
{
	struct octeon_eth_info *oct_eth_info;
	uint64_t tmp;
	int interface;
	cvmx_helper_interface_mode_t if_mode;

	debug("%s(%s)\n", __func__, dev->name);
	if (octeon_eth_board_pre_init(dev)) {
		printf("Pre-initialization for interface %s failed\n", dev->name);
		return;
	}

	oct_eth_info = (struct octeon_eth_info *) dev->priv;

	interface = oct_eth_info->interface;
	if_mode = cvmx_helper_interface_get_mode(interface);

	switch (if_mode) {
	case CVMX_HELPER_INTERFACE_MODE_RGMII:
	case CVMX_HELPER_INTERFACE_MODE_GMII:
		debug("  rgmii/gmii mode\n");
		tmp = cvmx_read_csr(CVMX_ASXX_RX_PRT_EN(interface));
		tmp |= (1ull << (oct_eth_info->port & 0x3));
		cvmx_write_csr(CVMX_ASXX_RX_PRT_EN(interface), tmp);
		tmp = cvmx_read_csr(CVMX_ASXX_TX_PRT_EN(interface));
		tmp |= (1ull << (oct_eth_info->port & 0x3));
		cvmx_write_csr(CVMX_ASXX_TX_PRT_EN(interface), tmp);
		octeon_eth_write_hwaddr(dev);
		break;

	case CVMX_HELPER_INTERFACE_MODE_SGMII:
	case CVMX_HELPER_INTERFACE_MODE_XAUI:
	case CVMX_HELPER_INTERFACE_MODE_RXAUI:
	case CVMX_HELPER_INTERFACE_MODE_XLAUI:
	case CVMX_HELPER_INTERFACE_MODE_XFI:
	case CVMX_HELPER_INTERFACE_MODE_10G_KR:
	case CVMX_HELPER_INTERFACE_MODE_40G_KR4:
	case CVMX_HELPER_INTERFACE_MODE_MIXED:
	case CVMX_HELPER_INTERFACE_MODE_AGL:
		debug("  SGMII/XAUI/etc.\n");
		octeon_eth_sgmii_enable(dev);
		octeon_eth_write_hwaddr(dev);
		break;

	default:
		break;
	}
	if (octeon_eth_board_post_init(dev))
		printf("Post-initialization for interface %s failed\n",
		       dev->name);
	debug("%s: EXIT\n", __func__);
}

/**
 * Keep track of packets sent on each interface so we can
 * autonegotiate before we send the first one.  This is cleared in
 * eth_try_another.
 */
int packets_sent;

#if defined(DEBUG_TX_PACKET) || defined(DEBUG_RX_PACKET)
static void print_mac(const char *label, const uint8_t *mac_addr)
{
	printf("%s: %02x:%02x:%02x:%02x:%02x:%02x", label,
	       mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
	       mac_addr[5]);
}

static void print_ip(const void *packet)
{
	uint8_t *p = (uint8_t *)packet;
	uint16_t length;
	uint8_t hdr_len;

	puts("IP Header:\n");
	if ((p[0] & 0xF0) != 0x40) {
		printf("Invalid IP version %d\n", *p >> 4);
		return;
	}
	hdr_len = *p & 0x0F;
	if (hdr_len < 5)
		printf("Invalid IP header length %d\n", hdr_len);
	printf("  Version: 4, Header length: %d\n", hdr_len);
	length = (p[2] << 8) | p[3];
	printf("  TOS: 0x%02x, length: %d\n", p[1], length);
	printf("  ID: %d, %s%s%s fragment offset: %d\n", (p[4] << 8) | p[5],
	       p[6] & 0x80 ? "congested, " : "", p[6] & 0x40 ? "DF, " : "",
	       p[6] & 0x20 ? "MF, " : "", ((p[6] & 0x1F) << 8) | p[7]);
	printf("  TTL: %d, Protocol: %d, Header Checksum: 0x%x\n", p[8], p[9],
	       (p[10] << 8) | p[11]);
	printf("  Source IP: %d.%d.%d.%d\n  Destination IP: %d.%d.%d.%d\n",
	       p[12], p[13], p[14], p[15], p[16], p[17], p[18], p[19]);
	if ((p[9] == 17) || (p[9] == 6))
		printf("  Source port: %u, Destination port: %u\n",
		       (p[20] << 8) | p[21], (p[22] << 8) | p[23]);
	puts("\n");
}

/**
 * Prints out a packet for debugging purposes
 *
 * @param[in]	packet - pointer to packet data
 * @param	length - length of packet in bytes
 */
static void print_packet(const void *packet, int length)
{
	int i, j;
	const unsigned char *up = packet;
	uint16_t type = (up[12] << 8 | up[13]);
	int start = 14;

	print_mac("DMAC", &up[0]);
	puts("    ");
	print_mac("SMAC", &up[6]);
	printf("    TYPE: %04x\n", type);

	if (type == 0x0800)
		print_ip(&up[start]);

	for (i = start; (i + 16) < length; i += 16) {
		printf("%04x ", i);
		for (j = 0; j < 16; ++j)
			printf("%02x ", up[i + j]);

		printf("    ");
		for (j = 0; j < 16; ++j)
			printf("%c", ((up[i + j] >= ' ')
				      && (up[i + j] <= '~')) ? up[i + j] : '.');
		printf("\n");
	}
	printf("%04x ", i);
	for (j = 0; i + j < length; ++j)
		printf("%02x ", up[i + j]);

	for (; j < 16; ++j)
		printf("   ");

	printf("    ");
	for (j = 0; i + j < length; ++j)
		printf("%c", ((up[i + j] >= ' ')
			      && (up[i + j] <= '~')) ? up[i + j] : '.');

	printf("\n");
}
#endif

/**
 * Send a packet out an Ethernet device
 *
 * @param dev - Ethernet device to send on
 * @param packet - pointer to packet to send
 * @param length - length of packet to send
 *
 * @return 0 for success
 */
int octeon_eth_send(struct eth_device *dev, void *packet, int length)
{				/* Send a packet  */
	struct octeon_eth_info *oct_eth_info =
					 (struct octeon_eth_info *)dev->priv;
	void *fpa_buf = cvmx_fpa_alloc(CVMX_FPA_PACKET_POOL);

	/* If we haven't been initialized, do it now. */
	if (!oct_eth_info->initted_flag) {
		debug("Initializing %s in send\n", dev->name);
		dev->init(dev, gd->bd);
	}
	if (!oct_eth_info->enabled) {
		oct_eth_info->enabled = 1;
		octeon_eth_enable(dev);
	}
	/* We need to copy this to a FPA buffer, then give that to TX */

	if ((oct_eth_info->packets_sent == 0) &&
	    !octeon_has_feature(OCTEON_FEATURE_BGX))
		cvm_oct_configure_rgmii_speed(dev);

	if (!fpa_buf) {
		printf("ERROR allocating buffer for packet!\n");
		return -1;
	}
	memcpy(fpa_buf, (void *)packet, length);
#ifdef DEBUG_TX_PACKET
	if (packet_tx_debug) {
		printf("\nTX packet: interface: %d, index: %d\n",
		       oct_eth_info->interface, oct_eth_info->index);
		print_packet(packet, length);
	}
#endif
	cvm_oct_xmit(dev, fpa_buf, length);
	oct_eth_info->packets_sent++;
	octeon_eth_tx_packet_hook(dev);
	return 0;
}

/**
 * String representation of error codes.
 */
static const char *rx_error_codes[] = {
		"OK",
		"partial",
		"jabber",
		"overrun",
		"oversize",
		"alignment",
		"fragment",
		"fcs",
		"undersize",
		"extend",
		"length mismatch",
		"rgmii rx",
		"skip error",
		"nibble error (studder)",
		"(undefined)",
		"(undefined)",
		"SPI 4.2 FCS",
		"skip",
		"L2 malformed",
};

/**
 * Called to receive a packet
 *
 * @param dev - device to receive on
 *
 * @return - length of packet
 *
 * This function is used to poll packets.  In turn it calls NetReceive
 * to process the packets.
 */
int octeon_eth_recv(struct eth_device *dev)
{				/* Check for received packets     */
	cvmx_wqe_t *work = cvmx_pow_work_request_sync(1);
	cvmx_buf_ptr_t buf_ptr;
	struct octeon_eth_info *oct_eth_info =
					(struct octeon_eth_info *)dev->priv;
	void *packet_data;
	int length;
	int error_code;

	/* If we haven't been initialized, do it now. */
	if (!oct_eth_info->initted_flag) {
		debug("Initializing %s in recv\n", dev->name);
		dev->init(dev, gd->bd);
	}

	if (!oct_eth_info->enabled) {
		oct_eth_info->enabled = 1;
		debug("%s: Enabling interface %s\n", __func__, dev->name);
		octeon_eth_enable(dev);
	}
	cvmx_update_rx_activity_led(oct_eth_info->interface,
				    oct_eth_info->index, true);

	if (!work) {
		/* Poll for link status change, only if work times out.
		 * On some interfaces this is really slow.
		 * getwork timeout is set to maximum.
		 */
		cvm_oct_configure_rgmii_speed(dev);
		return 0;
	}
	if ((error_code = cvmx_wqe_get_rcv_err(work))) {
		/* Work has error, so drop */
		cvmx_helper_free_packet_data(work);
		cvmx_wqe_free(work);
		if (error_code < ARRAY_SIZE(rx_error_codes) &&
		    !octeon_has_feature(OCTEON_FEATURE_BGX))
			printf("Receive error (code %d: %s), dropping\n",
			       error_code, rx_error_codes[error_code]);
		else
			printf("Receive error (code %d (unknown), dropping\n",
			       error_code);
		return 0;
	}
	if (cvmx_wqe_get_bufs(work) != 1) {
		/* can only support single-buffer packets */
		printf("Abnormal packet received in %u bufs, dropping\n",
			cvmx_wqe_get_bufs(work));
		cvmx_helper_free_packet_data(work);
		cvmx_wqe_free(work);
		return 0;
	}

	buf_ptr = cvmx_wqe_get_packet_ptr(work);
	packet_data = cvmx_phys_to_ptr(buf_ptr.s.addr);
	length = cvmx_wqe_get_len(work);

	oct_eth_info->packets_received++;
	debug("############# got work: %p, len: %d, packet_ptr: %p\n",
	      work, length, packet_data);
#if defined(DEBUG_RX_PACKET)
	if (packet_rx_debug) {
		printf("\nRX packet: interface: %d, index: %d\n",
		       oct_eth_info->interface, oct_eth_info->index);
		print_packet(packet_data, length);
	}
#endif
	cvmx_update_rx_activity_led(oct_eth_info->interface,
				    oct_eth_info->index, false);
	octeon_eth_rx_packet_hook(dev);
	NetReceive(packet_data, length);
	cvmx_helper_free_packet_data(work);
	cvmx_wqe_free(work);
	/* Free WQE and packet data */
	return length;
}

static void octeon_eth_halt_bgx(struct eth_device *dev,
				cvmx_helper_interface_mode_t mode)
{
	union cvmx_bgxx_cmrx_config cmr_config;
	union cvmx_bgxx_cmr_rx_adrx_cam cmr_cam;
	struct octeon_eth_info *oct_eth_info =
					(struct octeon_eth_info *)dev->priv;
	int index = oct_eth_info->index;
	int xiface = oct_eth_info->interface;
	struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);

	debug("%s(%s(%d.%d), %d)\n", __func__, dev->name, xiface, index, mode);

	/* For RGMII we need to properly shut down the XCV interface */
	if (cvmx_helper_bgx_is_rgmii(xiface, index)) {
		debug("  Shut down XCV RGMII\n");
		octeon_bgx_xcv_rgmii_enable(xi.interface, index, false);
	} else {

		cmr_config.u64 =
			cvmx_read_csr_node(xi.node,
					   CVMX_BGXX_CMRX_CONFIG(index,
								 xi.interface));
		cmr_config.s.data_pkt_tx_en = 0;
		cmr_config.s.data_pkt_rx_en = 0;
		cvmx_write_csr_node(xi.node,
				    CVMX_BGXX_CMRX_CONFIG(index, xi.interface),
				    cmr_config.u64);

		cmr_cam.u64 = cvmx_read_csr_node(xi.node,
						 CVMX_BGXX_CMR_RX_ADRX_CAM(index * 8,
									   xi.interface));
		cmr_cam.s.en = 0;
		cvmx_write_csr_node(xi.node,
				    CVMX_BGXX_CMR_RX_ADRX_CAM(index * 8,
							      xi.interface),
				    cmr_cam.u64);
		oct_eth_info->last_bgx_mac = 0;
		oct_eth_info->bgx_mac_set = 0;
	}
}

/**
 * Halts the specified Ethernet interface preventing it from receiving any more
 * packets.
 *
 * @param dev - Ethernet device to shut down.
 */
void octeon_eth_halt(struct eth_device *dev)
{
	uint64_t tmp;
	cvmx_gmxx_prtx_cfg_t gmx_cfg;
	struct octeon_eth_info *oct_eth_info =
				(struct octeon_eth_info *)dev->priv;
	int index = oct_eth_info->index;
	int interface = oct_eth_info->interface;
	cvmx_helper_interface_mode_t mode;
	union cvmx_gmxx_rxx_adr_ctl adr_ctl;

	debug("%s(%s): Halting\n", __func__, dev->name);

	oct_eth_info->enabled = 0;

	mode = cvmx_helper_interface_get_mode(oct_eth_info->interface);
	if (octeon_has_feature(OCTEON_FEATURE_BGX)) {
		octeon_eth_halt_bgx(dev, mode);
		return;
	}

	/* stop SCC                       */
	/* Disable reception on this port at the GMX block */
	switch (mode) {
	case CVMX_HELPER_INTERFACE_MODE_RGMII:
	case CVMX_HELPER_INTERFACE_MODE_GMII:
		debug("  RGMII/GMII\n");
		tmp = cvmx_read_csr(CVMX_ASXX_RX_PRT_EN(oct_eth_info->interface));
		tmp &= ~(1ull << index);
		/* Disable the RGMII RX ports */
		cvmx_write_csr(CVMX_ASXX_RX_PRT_EN(oct_eth_info->interface), tmp);
		tmp = cvmx_read_csr(CVMX_ASXX_TX_PRT_EN(oct_eth_info->interface));
		tmp &= ~(1ull << index);
		/* Disable the RGMII TX ports */
		cvmx_write_csr(CVMX_ASXX_TX_PRT_EN(oct_eth_info->interface), tmp);
		/* No break! */
	case CVMX_HELPER_INTERFACE_MODE_SGMII:
	case CVMX_HELPER_INTERFACE_MODE_QSGMII:
	case CVMX_HELPER_INTERFACE_MODE_XAUI:
	case CVMX_HELPER_INTERFACE_MODE_RXAUI:
	case CVMX_HELPER_INTERFACE_MODE_XLAUI:
	case CVMX_HELPER_INTERFACE_MODE_XFI:
	case CVMX_HELPER_INTERFACE_MODE_10G_KR:
	case CVMX_HELPER_INTERFACE_MODE_40G_KR4:
	case CVMX_HELPER_INTERFACE_MODE_MIXED:
	case CVMX_HELPER_INTERFACE_MODE_AGL:
		/* Disable MAC filtering */
		gmx_cfg.u64 = cvmx_read_csr(oct_eth_info->gmx_base + GMX_PRT_CFG);
		cvmx_write_csr(oct_eth_info->gmx_base + GMX_PRT_CFG,
			       gmx_cfg.u64 & ~1ull);
		adr_ctl.u64 = 0;
		adr_ctl.s.bcst = 1;	/* Reject broadcast */
		cvmx_write_csr(oct_eth_info->gmx_base + GMX_RX_ADR_CTL,
			       adr_ctl.u64);
		cvmx_write_csr(oct_eth_info->gmx_base + GMX_RX_ADR_CAM_EN, 0);
		cvmx_write_csr(oct_eth_info->gmx_base + GMX_PRT_CFG,
			       gmx_cfg.u64);
		break;
	default:
		printf("%s: Unknown mode %d for interface 0x%x:%d\n", __func__,
		       mode, interface, index);
		break;
	}
	debug("%s: Exit\n", __func__);
}

/**
 * Sets the hardware MAC address of the Ethernet device
 *
 * @param dev - Ethernet device
 *
 * @return 0 for success
 */
int octeon_eth_write_hwaddr(struct eth_device *dev)
{
	struct octeon_eth_info *oct_eth_info;
	oct_eth_info = (struct octeon_eth_info *)dev->priv;

	/* Skip if the interface isn't yet enabled */
	if (!oct_eth_info->enabled) {
		debug("%s: Interface not enabled, not setting MAC address\n",
		      __func__);
		return 0;
	}
	debug("%s: Setting %s address to %02x:%02x:%02x:%02x:%02x:%02x\n",
	      __func__, dev->name,
	      dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],
	      dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);
	return cvm_oct_set_mac_address(dev);
}

/**
 * Get the PHY information from the FDT
 *
 * @param[in,out]	dev	- Ethernet device
 * @param		mgmt	- 1 for management port
 *
 * @return 0 for success, -1 for error.
 *
 * TODO: Clean up the FDT code to use the node info stored in cvmx_helper_cfg.
 */
static int octeon_eth_get_phy_info(struct eth_device *dev, int mgmt)
{
	struct octeon_eth_info *oct_eth_info =
					 (struct octeon_eth_info *)dev->priv;

	int aliases, eth, phy;
	const char *phy_compatible_str;
	char name_buffer[20];
	const char *path;

	debug("%s(%s, %d)\n", __func__, dev->name, mgmt);
	if (mgmt) {
		aliases = fdt_path_offset(gd->fdt_blob, "/aliases");
		if (aliases < 0) {
			printf("%s: No /aliases node in device tree\n", __func__);
			return -1;
		}

		snprintf(name_buffer, sizeof(name_buffer), "mix%d",
			 oct_eth_info->port);
		path = fdt_getprop(gd->fdt_blob, aliases, name_buffer, NULL);
		if (!path) {
			printf("%s: ERROR: mix%d path not found in device tree\n",
			       __func__, oct_eth_info->port);
			return -1;
		}

		eth = fdt_path_offset(gd->fdt_blob, path);
	} else if (octeon_has_feature(OCTEON_FEATURE_BGX)) {
		eth = cvmx_helper_get_port_fdt_node_offset(oct_eth_info->interface,
							   oct_eth_info->index);
		debug("%s(%s): fdt node for interface 0x%x, port %d (ipd: %d): %d\n",
		     __func__, dev->name,
		     oct_eth_info->interface, oct_eth_info->index,
		     oct_eth_info->port, eth);
		if (eth < 0) {
			printf("%s: Error: could not find node offset in DT for interface 0x%x, index %d\n",
			       __func__, oct_eth_info->interface,
			       oct_eth_info->index);
			return -1;
		}
	} else {
		int pip, iface;

		aliases = fdt_path_offset(gd->fdt_blob, "/aliases");
		if (aliases < 0) {
			printf("%s: No /aliases node in device tree\n", __func__);
			return -1;
		}


		path = fdt_getprop(gd->fdt_blob, aliases, "pip", NULL);
		if (!path) {
			printf("%s: ERROR: pip path not found in device tree\n",
			       __func__);
			return -1;
		}
		pip = fdt_path_offset(gd->fdt_blob, path);
		if (pip < 0) {
			printf("%s: ERROR: pip not found in device tree\n",
			       __func__);
			return -1;
		}
		snprintf(name_buffer, sizeof(name_buffer), "interface@%d",
			 oct_eth_info->interface);
		iface = fdt_subnode_offset(gd->fdt_blob, pip, name_buffer);
		if (iface < 0) {
			printf("%s: ERROR: pip intf %d not found in device tree\n",
			       __func__, oct_eth_info->interface);
			return -1;
		}
		debug("%s: Looking for ethernet@%x\n", __func__,
		      oct_eth_info->index);
		snprintf(name_buffer, sizeof(name_buffer), "ethernet@%x",
			 oct_eth_info->index);
		eth = fdt_subnode_offset(gd->fdt_blob, iface, name_buffer);
	}
	oct_eth_info->fdt_offset = eth;

	if (eth < 0) {
		printf("%s: ERROR: could not find ethernet port in device tree for interface %d, port %d\n",
		       __func__, oct_eth_info->interface, oct_eth_info->index);
		return -1;
	}

	if (!octeon_has_feature(OCTEON_FEATURE_BGX)) {
		phy = fdtdec_lookup_phandle(gd->fdt_blob, eth, "phy-handle");
		if (phy < 0) {
			debug("%s: No phy handle for node %s at %d (%s)\n",
			      __func__, dev->name, eth,
			      fdt_get_name(gd->fdt_blob, eth, NULL));
			oct_eth_info->phy_fdt_offset = -1;
			return 0;
		}
	} else {
		phy = cvmx_helper_get_phy_fdt_node_offset(oct_eth_info->interface,
							  oct_eth_info->index);
	}
	oct_eth_info->phy_fdt_offset = phy;

	debug("  phy node offset: 0x%x, name: %s\n", phy,
	      fdt_get_name(gd->fdt_blob, phy, NULL));
	if (phy < 0) {
		printf("%s: ERROR: cannot find phy for interface %d, index %d\n",
		       __func__, oct_eth_info->interface, oct_eth_info->index);
		return -1;
	}

	phy_compatible_str = (const char *)fdt_getprop(gd->fdt_blob, phy,
						       "compatible", NULL);
	if (!phy_compatible_str) {
		printf("%s: ERROR: no compatible prop in phy\n", __func__);
		return -1;
#ifdef CONFIG_PHY_VITESSE
	}
	if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "vitesse")) {
		if (oct_eth_info->is_c45) {
			oct_eth_info->phy_device_type = GENERIC_8023_C45_PHY;
			oct_eth_info->vitesse_sfp_config = 1;
		} else {
			oct_eth_info->phy_device_type = GENERIC_8023_C22_PHY;
		}
#endif
#ifdef CONFIG_PHY_MARVELL
	} else if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "marvell")) {
		oct_eth_info->phy_device_type = MARVELL_GENERIC_PHY;
#endif
#ifdef CONFIG_PHY_CORTINA
	} else if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "cortina")) {
		debug("%s is Cortina\n", dev->name);
		oct_eth_info->phy_device_type = CORTINA_PHY;
		oct_eth_info->is_c45 = 1;
#endif
#ifdef CONFIG_PHY_AQUANTIA
	} else if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "aquantia")) {
		debug("%s is Aquantia\n", dev->name);
		oct_eth_info->phy_device_type = AQUANTIA_PHY;
		oct_eth_info->is_c45 = 1;
#endif
#ifdef CONFIG_PHY_BROADCOM
	} else if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "broadcom")) {
		oct_eth_info->phy_device_type = BROADCOM_GENERIC_PHY;
#endif
	} else if (!octeon_fdt_compat_vendor(gd->fdt_blob, phy, "ti")) {
		oct_eth_info->phy_device_type = GENERIC_8023_C45_PHY;
		oct_eth_info->ti_gpio_config = 1;
	} else if (!fdt_node_check_compatible(gd->fdt_blob, phy,
					      "ethernet-phy-ieee802.3-c22")) {
		oct_eth_info->phy_device_type = GENERIC_8023_C22_PHY;
		oct_eth_info->is_c45 = 0;
	} else if (!fdt_node_check_compatible(gd->fdt_blob, phy,
					      "ethernet-phy-ieee802.3-c45")) {
		oct_eth_info->phy_device_type = GENERIC_8023_C45_PHY;
		oct_eth_info->is_c45 = 1;
#ifdef CONFIG_PHY_ATHEROS
	} else if (!fdt_node_check_compatible(gd->fdt_blob, phy,
					      "atheros,ar8334") ||
		   !fdt_node_check_compatible(gd->fdt_blob, phy,
					      "atheros,ar8337") ||
		   !fdt_node_check_compatible(gd->fdt_blob, phy,
					      "qualcomm,qca8334") ||
   		   !fdt_node_check_compatible(gd->fdt_blob, phy,
					      "qualcomm,qca8337")) {
		oct_eth_info->phy_device_type = QUALCOMM_S17;
		oct_eth_info->is_c45 = 0;
#endif
	} else {
		printf("%s: Unknown PHY compatible string %s\n", __func__,
		       phy_compatible_str);
		return -1;
	}
	oct_eth_info->phy_fdt_offset = phy;
	debug("%s(%s, %d), phy node offset: %d\n", __func__, dev->name,
	      mgmt, phy);
	return 0;
}

/**
 * This weak function is called after the phy driver is connected but before
 * it is initialized.
 *
 * @param	dev	Ethernet device for phy
 *
 * @return	0 to continue, or -1 for error to stop setting up the phy
 */
int octeon_eth_board_post_setup_phy(struct eth_device *dev)
{
	return 0;
}
int octeon_eth_board_post_setup_phy(struct eth_device *dev)
	__attribute__((weak));

/**
 * Configure PHY information for the specified port and interface mode
 *
 * @param[in,out] dev          - ethernet device
 * @param         if_mode      - mode of port
 * @param         mgmt         - set if management port
 *
 * @return PHY MII address for success, -1 on error
 */
static int octeon_setup_phy(struct eth_device *dev,
			    cvmx_helper_interface_mode_t if_mode,
			    int mgmt)
{
	cvmx_phy_info_t phy_info;
	struct octeon_eth_info *oct_eth_info =
					 (struct octeon_eth_info *)dev->priv;
	char mii_name[MDIO_NAME_LEN];
	int port_offset;
	phy_interface_t phy_mode;

#ifdef DEBUG
	debug("%s(%s, %s, %d)\n", __func__, dev->name,
	      cvmx_helper_interface_mode_to_string(if_mode), mgmt);
#endif
	port_offset = oct_eth_info->port;

	if (octeon_has_feature(OCTEON_FEATURE_BGX_MIX) && mgmt == 1) {
		char env_var[20];
		int bgx_port;
		sprintf(env_var, "bgx_for_mix%d", oct_eth_info->port);
		if (getenv(env_var)) {
			bgx_port = getenv_ulong(env_var, 0, 1);
			/* Only the first 2 ports are MIXX interfaces */
			if (((bgx_port >> 4) & 0xf) != oct_eth_info->port) {
				printf("ERROR: Invalid bgx_for_mix%d passed for MIX%d\n",
					oct_eth_info->port, oct_eth_info->port);
				return -1;
			}
			port_offset = (bgx_port & 0xffff);
		} else {
			return -1;
		}
		mgmt = 0;
		phy_mode = cvm_if_mode_to_phy_mode(if_mode);
	} else if (mgmt) {
		phy_mode = PHY_INTERFACE_MODE_MII;
		port_offset += CVMX_HELPER_BOARD_MGMT_IPD_PORT;
	}
	if (mgmt == 0) {
		phy_mode = cvm_if_mode_to_phy_mode(if_mode);
		if (phy_mode == PHY_INTERFACE_MODE_NONE) {
			debug("No phy for interface: %d, port: %d, dev: %s\n",
			      oct_eth_info->interface, oct_eth_info->port,
			      dev->name);
			oct_eth_info->mii_addr = -2;
			oct_eth_info->phydev = NULL;
		}
	}
	debug("%s: interface: %d, index: %d, phy mode: %s (%s), port offset: %d\n",
	      __func__,
	      oct_eth_info->interface, oct_eth_info->index,
	      cvmx_helper_interface_mode_to_string(if_mode),
	      phy_string_for_interface(phy_mode),
	      port_offset);
	if (cvmx_helper_board_get_phy_info(&phy_info, port_offset)) {
		debug("%s: No phy info for ipd port %d\n", __func__, port_offset);
		return 0;
	}

	oct_eth_info->mii_addr = phy_info.phy_addr;
	oct_eth_info->phy_offset = phy_info.fdt_offset;

	if (phy_info.direct_connect)
		snprintf(mii_name, sizeof(mii_name), "mdio-octeon%d",
			 oct_eth_info->mii_addr >> 8);
	else
		snprintf(mii_name, sizeof(mii_name), "mdio-mux-octeon%d",
			 phy_info.gpio_value);

	debug("%s: Setting up PHY for %s on mdio bus %s\n", __func__,
	      dev->name, mii_name);
	oct_eth_info->mii_bus = miiphy_get_dev_by_name(mii_name);
	if (!oct_eth_info->mii_bus) {
		debug("Could not find MDIO bus for %s, MII address 0x%x, interface %d, port %d\n",
		       mii_name, oct_eth_info->mii_addr,
		       oct_eth_info->interface, oct_eth_info->port);
		oct_eth_info->phydev = NULL;
#ifdef CONFIG_PHYLIB
	} else {
		debug("Connecting MII phy bus: %s (%d), mode: %s, address: %d to %s\n",
		      mii_name,
		      oct_eth_info->mii_addr >> 8,
		      phy_string_for_interface(phy_mode),
		      oct_eth_info->mii_addr & 0xff,
		      dev->name);
# ifdef CONFIG_PHY_CORTINA
		if (phy_info.phy_type == CORTINA_PHY)
			/* The Cortina PHY does not play well with how U-Boot normally
			 * configures PHYs since the PHY ID registers are at 0.0 and 0.1
			 * instead of 1.2 and 1.3 (clause 45).
			 *
			 * Also, in the case of XFI the Cortina PHY needs to
			 * know the sub address since each of the four PHY
			 * interfaces shares the same MDIO address but uses
			 * different address ranges for the registers.
			 */
			oct_eth_info->phydev =
				cortina_phy_connect(oct_eth_info->mii_bus,
						    oct_eth_info->mii_addr & 0xff,
						    phy_info.phy_sub_addr,
						    dev, phy_mode);
		else
# endif
			oct_eth_info->phydev = phy_connect(oct_eth_info->mii_bus,
							   oct_eth_info->mii_addr & 0xff,
							   dev, phy_mode);

		/* We need to call phy_startup for the Cortina PHY. For other
		 * PHY devices it just adds annoying messages and delays if
		 * they are not connected.
		 */
		if (octeon_eth_get_phy_info(dev, mgmt) == 0) {
			switch (oct_eth_info->phy_device_type) {
			case MARVELL_GENERIC_PHY:
				debug("Marvell");
				break;
			case BROADCOM_GENERIC_PHY:
				debug("Broadcom");
				break;
			case CORTINA_PHY:
				debug("Cortina");
				break;
			case AQUANTIA_PHY:
				debug("Aquantia");
				break;
			case GENERIC_8023_C22_PHY:
				debug("Generic 802.3 C22");
				break;
			case GENERIC_8023_C45_PHY:
				debug("Generic 802.3 C45");
				break;
			default:
				debug("UNKNOWN (%d)",
				      oct_eth_info->phy_device_type);
				break;
			}
			debug(" phy reported for interface %d, index %d, port %d\n",
			      oct_eth_info->interface, oct_eth_info->index,
			      oct_eth_info->port);
			if (oct_eth_info->phydev)
				phy_config(oct_eth_info->phydev);
# ifdef CONFIG_PHY_CORTINA
			if (oct_eth_info->phy_device_type == CORTINA_PHY) {
				debug("Starting PHY at %d\n",
				      oct_eth_info->phydev->addr);
				phy_startup(oct_eth_info->phydev);
				if (board_configure_cs4321_leds(oct_eth_info->phydev)) {
					puts("Error configuring Cortina LEDs\n");
				}
			}
# endif
		} else {
			debug("No PHY information available for interface %d, index %d\n",
			      oct_eth_info->interface, oct_eth_info->index);
		}
#endif
	}
	return oct_eth_info->mii_addr;
}

/**
 * Initializes the specified interface and port
 *
 * @param	interface	interface to initialize
 * @param	index		port index on interface
 * @param	port		ipd port number
 * @param	if_mode		interface mode
 *
 * @return	0 for success, -1 if out of memory, 1 if port is invalid
 */
static int octeon_eth_initialize_port(int interface, int index, int port,
				      cvmx_helper_interface_mode_t if_mode)
{
	struct eth_device *dev;
	struct octeon_eth_info *oct_eth_info;
	uint32_t *mac_inc_ptr = (uint32_t *) (&mac_addr[2]);
	int eth;
	int rc;

	eth = cvmx_helper_get_port_fdt_node_offset(interface, index);
	if (eth <= 0) {
		debug("ERROR: No fdt node for interface %d, index %d\n",
		       interface, index);
		return 1;
	}

	dev = (struct eth_device *)calloc(sizeof(*dev), 1);
	if (!dev) {
		puts("Out of memory!\n");
		return -1;
	}

	oct_eth_info = (struct octeon_eth_info *)
				calloc(sizeof(struct octeon_eth_info), 1);
	if (!oct_eth_info) {
		puts("Out of memory!\n");
		free(dev);
		return -1;
	}

	available_mac_count--;
	oct_eth_info->is_c45 =
			(if_mode == CVMX_HELPER_INTERFACE_MODE_XAUI) ||
			(if_mode == CVMX_HELPER_INTERFACE_MODE_RXAUI) ||
			(if_mode == CVMX_HELPER_INTERFACE_MODE_XFI) ||
			(if_mode == CVMX_HELPER_INTERFACE_MODE_XLAUI) ||
			(if_mode == CVMX_HELPER_INTERFACE_MODE_10G_KR) ||
			(if_mode == CVMX_HELPER_INTERFACE_MODE_10G_KR);
	oct_eth_info->port = port;
	oct_eth_info->index = index;
	oct_eth_info->interface = interface;
	oct_eth_info->initted_flag = 0;
	/* This is guaranteed to force the link state to be printed out.
	 */
	oct_eth_info->link_state = 0xffffffffffffffffULL;
	oct_eth_info->ethdev = dev;
	debug("Setting up port: %d, int: %d, index: %d, device: octeth%d\n",
	      oct_eth_info->port, oct_eth_info->interface,
	      oct_eth_info->index, card_number);
	if (if_mode == CVMX_HELPER_INTERFACE_MODE_AGL) {
		oct_eth_info->gmx_base = CVMX_AGL_GMX_RXX_INT_REG(0);
		sprintf(dev->name, "octrgmii%d", agl_number++);
	} else {
		if (!octeon_has_feature(OCTEON_FEATURE_BGX))
			oct_eth_info->gmx_base =
					CVMX_GMXX_RXX_INT_REG(index, interface);
		if (OCTEON_IS_MODEL(OCTEON_CN73XX)) {
			int bgx = getenv_ulong("bgx_for_rgmii", 0, -1);
			if (bgx == interface && index == 0)
				sprintf(dev->name, "octrgmii0");
			else
				sprintf(dev->name, "octeth%d", card_number++);
		} else {
#ifdef CONFIG_OCTEON_IM8724
			if (!strcmp(getenv("ethname"), "ifmode")) {
			    switch (if_mode) {
				case CVMX_HELPER_INTERFACE_MODE_SGMII:
					sprintf(dev->name, "octsgmii%d%d", interface, index);
					card_number++;
					break;
				case CVMX_HELPER_INTERFACE_MODE_RXAUI:
					sprintf(dev->name, "octrxaui%d%d", interface, index);
					card_number++;
					break;
				case CVMX_HELPER_INTERFACE_MODE_XFI:
					sprintf(dev->name, "octxfi%d%d", interface, index);
					card_number++;
					break;
				default:
					sprintf(dev->name, "octeth%d", card_number++);
			    }
			}
			else
#endif
			sprintf(dev->name, "octeth%d", card_number++);
		}
	}

	dev->priv = (void *)oct_eth_info;
	dev->iobase = 0;
	dev->init = octeon_eth_init;
	dev->halt = octeon_eth_halt;
	dev->send = octeon_eth_send;
	dev->recv = octeon_eth_recv;
	dev->write_hwaddr = octeon_eth_write_hwaddr;
	memcpy(dev->enetaddr, mac_addr, 6);
	(*mac_inc_ptr)++;	/* increment MAC address */

	debug("%s: Registering %s\n", __func__, dev->name);
	eth_register(dev);

	debug("%s: Setting up phy for %s\n", __func__, dev->name);
	octeon_setup_phy(dev, if_mode, 0);
	/* Link the MDIO bus and PHY to this Ethernet
	 * port
	 */

#ifdef CONFIG_PHY_SFP
	debug("%s: Getting SFP info for %s\n", __func__, dev->name);
	rc = octeon_sfp_get_info(eth, &oct_eth_info->sfp);
	debug("%s: rc: %d, valid: %d\n", __func__, rc, oct_eth_info->sfp.valid);
	if (!rc && !oct_eth_info->sfp.valid) {
		debug("%s: Not found, in port, checking interface\n",
		      __func__);
		rc = octeon_sfp_get_info(fdt_parent_offset(gd->fdt_blob, eth),
					 &oct_eth_info->sfp);
	}
	if (rc) {
		printf("%s: Error parsing SFP info for %s\n",
		       __func__, dev->name);
		return -1;
	} else if (oct_eth_info->sfp.valid) {
		debug("%s: Registering default mod_abs function\n", __func__);
		octeon_sfp_register_mod_abs_func(dev, NULL, NULL);
	}
# if defined(CONFIG_PHY_CORTINA) && defined(CONFIG_PHY_CORTINA_CS4223)
	if (oct_eth_info->phy_device_type == CORTINA_PHY)
		cs4224_register_sfp(oct_eth_info->phydev,
				    oct_eth_info->sfp.i2c_bus,
				    oct_eth_info->sfp.i2c_eeprom_addr);
# endif
#endif
	return 0;
}

/**
 * Initializes all ports on an interface
 *
 * @param	interface	interface ports are on
 * @param	if_mode		interface mode
 *
 * @return	0 for success, -1 if out of memory
 */
static int octeon_eth_initialize_ports(int interface,
				       cvmx_helper_interface_mode_t if_mode)
{
	int index;
	int port;
	int pknd;
	int num_ports;
	int incr = 1;
	int rc;

	cvmx_helper_interface_probe(interface);
	num_ports = cvmx_helper_ports_on_interface(interface);

	debug("Interface %d is %s, ports: %d\n", interface,
	      cvmx_helper_interface_mode_to_string(if_mode),
	      num_ports);
	/* RGMII or SGMII */
	pknd = cvmx_helper_get_ipd_port(interface, 0);
	debug("pknd: %d\n", pknd);
	if (octeon_has_feature(OCTEON_FEATURE_PKND))
		incr = 16;

	num_ports = pknd + (num_ports * incr);
	for (index = 0, port = pknd; (port < num_ports); port += incr, index++) {
		cvmx_helper_interface_mode_t mode;
		if (!cvmx_helper_is_port_valid(interface, index)) {
			debug("interface %d port %d invalid\n",
			      interface, index);
			continue;
		}
		if (octeon_has_feature(OCTEON_FEATURE_BGX))
			mode = cvmx_helper_bgx_get_mode(interface, index);
		else
			mode = if_mode;
		rc = octeon_eth_initialize_port(interface, index, port, mode);
		if (rc < 0) {
			printf("%s: Could not initialize interface %d, index %d\n",
			       __func__, interface, index);
			return -1;
		} else if (rc == 1) {
			cvmx_helper_set_port_valid(interface, index, false);
			debug("%s: interface: %d, index: %d is not valid\n",
			      __func__, interface, index);
		}
	}
	return 0;
}

/*********************************************************
Only handle built-in RGMII/SGMII/XAUI/GMII ports here
**********************************************************/
int octeon_eth_initialize(bd_t * bis)
{
	int interface;
	int num_ints;
	cvmx_helper_interface_mode_t if_mode;

	debug("%s called\n", __func__);
	if (available_mac_count == -1) {
		available_mac_count = gd->arch.mac_desc.count;
		memcpy(mac_addr, (void *)gd->arch.mac_desc.mac_addr_base, 6);
		debug("%s: Base MAC address: %02x:%02x:%02x:%02x:%02x:%02x, count: %d\n",
		      __func__, mac_addr[0], mac_addr[1], mac_addr[2],
		      mac_addr[3], mac_addr[4], mac_addr[5],
		      available_mac_count);
	}
	if (getenv("disable_networking")) {
		printf("Networking disabled with 'disable_networking' "
		       "environment variable, skipping RGMII interface\n");
		return 0;
	}
	if (available_mac_count <= 0) {
		printf("No available MAC addresses for RGMII interface, "
		       "skipping\n");
		return 0;
	}
	cvmx_user_static_config();
	__cvmx_helper_init_port_valid();

	/* Do board specific init in early_board_init or checkboard if
	 * possible.
	 */

	/* NOTE: on 31XX based boards, the GMXX_INF_MODE register must be set
	 * appropriately before this code is run (in checkboard for instance).
	 * The hardware is configured based on the settings of GMXX_INF_MODE.
	 */

	/* We only care about the first two (real packet interfaces)
	 * This will ignore PCI, loop, etc interfaces.
	 */
	num_ints = cvmx_helper_get_number_of_interfaces();
	debug("Num interfaces: %d\n", num_ints);

	/* Check to see what interface and ports we should use */
	for (interface = 0; interface < num_ints; interface++) {
		if_mode = cvmx_helper_interface_get_mode(interface);
		if (CVMX_HELPER_INTERFACE_MODE_RGMII == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_GMII  == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_SGMII == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_QSGMII == if_mode  ||
		    CVMX_HELPER_INTERFACE_MODE_RXAUI == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_XLAUI == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_XFI == if_mode     ||
		    CVMX_HELPER_INTERFACE_MODE_10G_KR == if_mode  ||
		    CVMX_HELPER_INTERFACE_MODE_40G_KR4 == if_mode ||
		    CVMX_HELPER_INTERFACE_MODE_MIXED == if_mode   ||
		    CVMX_HELPER_INTERFACE_MODE_AGL == if_mode     ||
		    CVMX_HELPER_INTERFACE_MODE_XAUI  == if_mode) {
			if (octeon_eth_initialize_ports(interface, if_mode)) {
				printf("%s: Error initializing ports on interface %d\n",
				       __func__, interface);
				return -1;
			}
		}
	}
	if (!packet_rx_debug)
		packet_rx_debug = (getenv("octeon_debug_rx_packets") != NULL);
	if (!packet_tx_debug)
		packet_tx_debug = (getenv("octeon_debug_tx_packets") != NULL);

	return card_number;
}

#ifdef CONFIG_OCTEON_SPI4000_ENET
/**
 * Writes MAC address to the hardware for SPI4000 (NOP)
 *
 * @param dev - SPI4000 device
 *
 * @return 0
 */
int octeon_spi4000_write_hwaddr(struct eth_device *dev)
{
	return 0;
}

/**
 * Initializes all SPI4000 interfaces
 *
 * @param bis - Board data structure pointer
 */
int octeon_spi4000_initialize(bd_t * bis)
{
	struct eth_device *dev;
	struct octeon_eth_info *oct_eth_info;
	int port;
	int interface = 0;
	uint32_t *mac_inc_ptr = (uint32_t *) (&mac_addr[2]);
	cvmx_spi_callbacks_t spi_callbacks_struct;

	debug("%s: Entry\n", __func__);

	if (available_mac_count == -1) {
		available_mac_count = gd->arch.mac_desc.count;
		memcpy(mac_addr, (void *)(gd->arch.mac_desc.mac_addr_base), 6);
	}
	if (getenv("disable_networking")) {
		printf("Networking disabled with 'disable_networking' "
		       "environment variable, skipping SPI interface\n");
		return 0;
	}

	/* Re-setup callbacks here, as the initialization done at compile time
	 * points to the link addresses in flash.
	 */
	cvmx_spi_get_callbacks(&spi_callbacks_struct);
	spi_callbacks_struct.reset_cb = cvmx_spi_reset_cb;
	spi_callbacks_struct.calendar_setup_cb = cvmx_spi_calendar_setup_cb;
	spi_callbacks_struct.clock_detect_cb = cvmx_spi_clock_detect_cb;
	spi_callbacks_struct.training_cb = cvmx_spi_training_cb;
	spi_callbacks_struct.calendar_sync_cb = cvmx_spi_calendar_sync_cb;
	spi_callbacks_struct.interface_up_cb = cvmx_spi_interface_up_cb;
	cvmx_spi_set_callbacks(&spi_callbacks_struct);

	if (!cvmx_spi4000_is_present(interface)) {
		debug("%s: interface %d not present\n", __func__, interface);
		return 0;
	}
	if (available_mac_count <= 0) {
		printf("No available MAC addresses for SPI interface, "
		       "skipping\n");
		return 0;
	}

	for (port = 0; port < 10 && available_mac_count-- > 0; port++) {
		dev = (struct eth_device *)calloc(sizeof(*dev), 1);
		oct_eth_info = (struct octeon_eth_info *)
			calloc(sizeof(struct octeon_eth_info), 1);
		oct_eth_info->port = port;
		oct_eth_info->interface = interface;
		oct_eth_info->initted_flag = 0;

		debug("Setting up SPI4000 port: %d, int: %d\n",
		      oct_eth_info->port, oct_eth_info->interface);
		sprintf(dev->name, "octspi%d", port);
		card_number++;

		dev->priv = (void *)oct_eth_info;
		dev->iobase = 0;
		dev->init = octeon_spi4000_init;
		dev->halt = octeon_eth_halt;
		dev->send = octeon_eth_send;
		dev->recv = octeon_eth_recv;
		dev->write_hwaddr = octeon_spi4000_write_hwaddr;
		memcpy(dev->enetaddr, mac_addr, 6);
		(*mac_inc_ptr)++;	/* increment MAC address */

		eth_register(dev);
		octeon_spi4000_init(dev, gd->bd);
	}
	return card_number;
}
#endif /* CONFIG_OCTEON_SPI4000_ENET */

#ifdef CONFIG_OCTEON_SPI_IXF18201_ENET
int octeon_spi_ixf18201_init(struct eth_device *dev, bd_t * bis)
{				/* Initialize the device  */
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
	static char spi_inited[2] = { 0 };
	debug("octeon_spi_ixf18201_init(), dev_ptr: %p, port: %d\n",
	      dev, priv->port);

	if (priv->initted_flag)
		return 1;

	if (!octeon_global_hw_inited) {
		cvm_oct_configure_common_hw();
	}

	if (!spi_inited[priv->interface]) {
		spi_inited[priv->interface] = 1;
		if (__cvmx_helper_spi_enable(priv->interface) < 0) {
			printf("ERROR initializing spi on Octeon Interface %d\n",
			       priv->interface);
			return 0;
		}
	}

	if (!octeon_global_hw_inited) {
		cvmx_helper_ipd_and_packet_input_enable();
		octeon_global_hw_inited = 1;
	}
	priv->initted_flag = 1;
	return (1);
}

int octeon_spi_ixf18201_write_hwaddr(struct eth_device *dev)
{
	debug("%s: Setting %s address to %02x:%02x:%02x:%02x:%02x:%02x\n",
	      __func__, dev->name,
	      dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],
	      dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);
	return 0;
}

int octeon_spi_ixf18201_initialize(bd_t * bis)
{
	struct eth_device *dev;
	struct octeon_eth_info *oct_eth_info;
	int port;
	int interface = 0;
	uint32_t *mac_inc_ptr = (uint32_t *) (&mac_addr[2]);
	cvmx_spi_callbacks_t spi_callbacks_struct;

	if (available_mac_count == -1) {
		available_mac_count = gd->arch.mac_desc.count;
		memcpy(mac_addr, (void *)(gd->arch.mac_desc.mac_addr_base), 6);
	}
	if (getenv("disable_networking")) {
		printf("Networking disabled with 'disable_networking' "
		       "environment variable, skipping SPI interface\n");
		return 0;
	}

	/* Re-setup callbacks here, as the initialization done at compile time
	 * points to the link addresses in flash.
	 */
	cvmx_spi_get_callbacks(&spi_callbacks_struct);
	spi_callbacks_struct.reset_cb = cvmx_spi_reset_cb;
	spi_callbacks_struct.calendar_setup_cb = cvmx_spi_calendar_setup_cb;
	spi_callbacks_struct.clock_detect_cb = cvmx_spi_clock_detect_cb;
	spi_callbacks_struct.training_cb = cvmx_spi_training_cb;
	spi_callbacks_struct.calendar_sync_cb = cvmx_spi_calendar_sync_cb;
	spi_callbacks_struct.interface_up_cb = cvmx_spi_interface_up_cb;
	cvmx_spi_set_callbacks(&spi_callbacks_struct);

	if (available_mac_count <= 0) {
		printf("No available MAC addresses for SPI interface, "
		       "skipping\n");
		return 0;
	}

	int eth_num = 0;
	for (interface = 0; interface < 2; interface++) {
		int ports = 1;	/* We only want to set up the real ports here */
		for (port = 0; port < ports && available_mac_count-- > 0;
		     port++) {
			dev = (struct eth_device *)calloc(sizeof(*dev), 1);
			oct_eth_info = (struct octeon_eth_info *)
			    calloc(sizeof(struct octeon_eth_info), 1);

			oct_eth_info->port = port;
			oct_eth_info->interface = interface;
			oct_eth_info->initted_flag = 0;

			debug("Setting up SPI port: %d, int: %d\n",
			      oct_eth_info->port, oct_eth_info->interface);
			sprintf(dev->name, "octspi%d", eth_num++);
			card_number++;

			dev->priv = (void *)oct_eth_info;
			dev->iobase = 0;
			dev->init = octeon_spi_ixf18201_init;
			dev->halt = octeon_eth_halt;
			dev->send = octeon_eth_send;
			dev->recv = octeon_eth_recv;
			dev->write_hwaddr = octeon_spi_ixf18201_write_hwaddr;
			memcpy(dev->enetaddr, mac_addr, 6);
			(*mac_inc_ptr)++;	/* increment MAC address */

			eth_register(dev);
			octeon_spi_ixf18201_init(dev, gd->bd);
		}
	}
	return card_number;
}
#endif /* CONFIG_OCTEON_SPI_IXF18201_ENET */

#endif /* OCTEON_INTERNAL_ENET */

#if defined(CONFIG_OCTEON_MGMT_ENET) && defined(CONFIG_CMD_NET)

int octeon_mgmt_eth_init(struct eth_device *dev, bd_t * bis)
{				/* Initialize the device  */
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
	debug("%s(), dev_ptr: %p\n", __func__, dev);

	if (priv->initted_flag)
		return 1;

	priv->enabled = 0;

	priv->initted_flag = 1;
	return 1;
}

int octeon_mgmt_eth_send(struct eth_device *dev, void *packet, int length)
{				/* Send a packet  */
	int retval;
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;

	if (!priv->enabled) {
		priv->enabled = 1;
		cvmx_mgmt_port_enable(priv->port);
	}
	debug("%s: called for %s, sending %d bytes\n",
	      __func__, dev->name, length);
	retval = cvmx_mgmt_port_send(priv->port, length, (void *)packet);

	if (packets_sent++ == 0)
		cvmx_mgmt_port_link_set(priv->port,
					cvmx_mgmt_port_link_get(priv->port));
	octeon_eth_tx_packet_hook(dev);

	return retval;

}

#define MGMT_BUF_SIZE   1700
int octeon_mgmt_eth_recv(struct eth_device *dev)
{				/* Check for received packets     */
	uchar buffer[MGMT_BUF_SIZE];
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
	int length;

	if (!priv->enabled) {
		priv->enabled = 1;
		cvmx_mgmt_port_enable(priv->port);
	}
	length = cvmx_mgmt_port_receive(priv->port, MGMT_BUF_SIZE, buffer);

	if (length > 0) {
#if defined(DEBUG) && 0
		/* Dump out packet contents */
		{
			int i, j;
			unsigned char *up = buffer;

			for (i = 0; (i + 16) < length; i += 16) {
				printf("%04x ", i);
				for (j = 0; j < 16; ++j) {
					printf("%02x ", up[i + j]);
				}
				printf("    ");
				for (j = 0; j < 16; ++j) {
					printf("%c", ((up[i + j] >= ' ')
						      && (up[i + j] <=
							  '~')) ? up[i +
								     j] : '.');
				}
				printf("\n");
			}
			printf("%04x ", i);
			for (j = 0; i + j < length; ++j) {
				printf("%02x ", up[i + j]);
			}
			for (; j < 16; ++j) {
				printf("   ");
			}
			printf("    ");
			for (j = 0; i + j < length; ++j) {
				printf("%c", ((up[i + j] >= ' ')
					      && (up[i + j] <=
						  '~')) ? up[i + j] : '.');
			}
			printf("\n");
		}
#endif
		debug("MGMT port %s got %d byte packet\n", dev->name, length);
		octeon_eth_rx_packet_hook(dev);
		NetReceive(buffer, length);
	} else if (length < 0) {
		printf("MGMT port %s rx error: %d\n", dev->name, length);
	}

	return (length);
}

/**
 * Shuts down receiving packets on a management interface
 *
 * @param dev - Management Ethernet device
 */
void octeon_mgmt_eth_halt(struct eth_device *dev)
{				/* stop SCC                       */
	struct octeon_eth_info *priv = (struct octeon_eth_info *) dev->priv;
	priv->enabled = 0;
	cvmx_mgmt_port_disable(priv->port);
}

/**
 * Writes the MAC address to the hardware for management devices
 *
 * @param dev - Management Ethernet device
 *
 * @return 0 for success
 */
int octeon_mgmt_eth_write_hwaddr(struct eth_device *dev)
{
	struct octeon_eth_info *oct_eth_info;
	u64 mac = 0;

	oct_eth_info = (struct octeon_eth_info *)dev->priv;

	memcpy(((char *)&mac) + 2, dev->enetaddr, 6);
	debug("%s: Setting %s address to %02x:%02x:%02x:%02x:%02x:%02x\n",
	      __func__, dev->name,
	      dev->enetaddr[0], dev->enetaddr[1], dev->enetaddr[2],
	      dev->enetaddr[3], dev->enetaddr[4], dev->enetaddr[5]);
	return cvmx_mgmt_port_set_mac(oct_eth_info->port, mac);
}

/**
 * Initializes management Ethernet devices
 *
 * @param bis - Board specific data structure pointer
 *
 * @return current card number
 */
int octeon_mgmt_eth_initialize(bd_t * bis)
{
	struct eth_device *dev;
	struct octeon_eth_info *oct_eth_info;
	uint32_t *mac_inc_ptr = (uint32_t *) (&mac_addr[2]);
	int mgmt_port_count;
	int cur_port;
	int retval;
	int aliases;

	debug("%s called\n", __func__);
	/* Figure out how many management ports there are from
	 * the device tree, otherwise assume number available to chip.
	 */
	aliases = fdt_path_offset(gd->fdt_blob, "/aliases");
	if (aliases >= 0) {
		if (fdt_getprop(gd->fdt_blob, aliases, "mix1", NULL))
			mgmt_port_count = 2;
		else if (fdt_getprop(gd->fdt_blob, aliases, "mix0", NULL))
			mgmt_port_count = 1;
		else
			mgmt_port_count = 0;
	} else {
		if (OCTEON_IS_MODEL(OCTEON_CN52XX)
		    || OCTEON_IS_MODEL(OCTEON_CN63XX)
		    || OCTEON_IS_MODEL(OCTEON_CN66XX)
		    || OCTEON_IS_MODEL(OCTEON_CN61XX)
		    || OCTEON_IS_MODEL(OCTEON_CN78XX)
		    || OCTEON_IS_MODEL(OCTEON_CN73XX)
		    || OCTEON_IS_MODEL(OCTEON_CNF75XX))
			mgmt_port_count = 2;
		else
			mgmt_port_count = 1;
	}

	if (available_mac_count == -1) {
		available_mac_count = gd->arch.mac_desc.count;
		memcpy(mac_addr,
		       (uint8_t *) (gd->arch.mac_desc.mac_addr_base), 6);
	}
	if (getenv("disable_networking")) {
		printf("Networking disabled with 'disable_networking' "
		       "environment variable, skipping RGMII interface\n");
		return 0;
	}

	if (available_mac_count <= 0) {
		printf("No available MAC addresses for Management "
		       "interface(s), skipping\n");
		return 0;
	}

	for (cur_port = 0;
	     (cur_port < mgmt_port_count) && available_mac_count-- > 0;
	     cur_port++) {
		uint64_t mac = 0x0;
		cvmx_helper_link_info_t link;
		cvmx_helper_interface_mode_t if_mode;
		int ipd_port;

		/* Initialize port state array to invalid values */
		dev = (struct eth_device *)malloc(sizeof(*dev));

		oct_eth_info = (struct octeon_eth_info *)
				calloc(sizeof(struct octeon_eth_info), 1);

		oct_eth_info->port = cur_port;
		/* These don't apply to the management ports */
		oct_eth_info->interface = ~0;
		oct_eth_info->initted_flag = 0;
#ifdef CONFIG_PHY_SFP
		octeon_sfp_register_mod_abs_func(dev, NULL, NULL);
#endif
		if (octeon_has_feature(OCTEON_FEATURE_BGX_MIX)) {
			int bgx_port;
			char env_var[16];

			sprintf(env_var, "bgx_for_mix%d", oct_eth_info->port);
			if (getenv(env_var)) {
				bgx_port = getenv_ulong(env_var, 0, 1);
				if (((bgx_port >> 4) & 0xf) != oct_eth_info->port) {
					printf("Error: Invalid bgx_for_mix%d passed for octmgmt%d\n",
						oct_eth_info->port, oct_eth_info->port);
					continue;
				}
				ipd_port = (bgx_port & 0xffff);
				oct_eth_info->interface = ((ipd_port >> 8) & 0xf) - 8;
				oct_eth_info->index = (ipd_port >> 4) & 0xf;
			} else {
				debug("Not configuring MIX%d, %s is not set\n", oct_eth_info->port, env_var);
				continue;
			}
			if_mode = cvmx_helper_interface_get_mode(
				cvmx_helper_node_interface_to_xiface(
					(bgx_port >> 12) & 0xf,
					oct_eth_info->interface));
		} else {
			ipd_port = oct_eth_info->port;
			if_mode = CVMX_HELPER_INTERFACE_MODE_GMII;
		}

		oct_eth_info->link_state = 0xffffffffffffffffULL;

		debug("Setting up mgmt eth port %d\n", cur_port);
		sprintf(dev->name, "octmgmt%d", cur_port);

		dev->priv = (void *)oct_eth_info;
		dev->iobase = 0;
		dev->init = octeon_mgmt_eth_init;
		dev->halt = octeon_mgmt_eth_halt;
		dev->send = octeon_mgmt_eth_send;
		dev->recv = octeon_mgmt_eth_recv;
		dev->write_hwaddr = octeon_mgmt_eth_write_hwaddr;

		/* Use "ethaddr" MAC if present, or fall back to computed MAC */
		if (!eth_getenv_enetaddr("ethaddr", dev->enetaddr)) {
			memcpy(dev->enetaddr, mac_addr, 6);
			(*mac_inc_ptr)++;	/* increment MAC address */
		}

		eth_register(dev);

		/* Call platform-specific intialization */
		retval = cvmx_mgmt_port_initialize(ipd_port);
		if (CVMX_MGMT_PORT_SUCCESS != retval) {
			debug("ERROR: cvmx_mgmt_port_initialize() failed: %d\n",
			       retval);
		}
		memcpy(((char *)&mac) + 2, dev->enetaddr, 6);
		cvmx_mgmt_port_set_mac(oct_eth_info->port, mac);
		link = cvmx_mgmt_port_link_get(oct_eth_info->port);
		if (octeon_setup_phy(dev, if_mode, 1) < 0)
			printf("Error configuring PHY for management port %d\n",
			       cur_port);

		debug("Port: %d, speed: %d, up: %d, duplex: %d\n", cur_port,
		      link.s.speed, link.s.link_up, link.s.full_duplex);
		cvmx_mgmt_port_link_set(oct_eth_info->port, link);
	}

	return card_number;

}
#endif /* defined(CONFIG_OCTEON_MGMT_ENET) && defined(CONFIG_CMD_NET) */
