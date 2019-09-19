/***********************license start************************************
 * Copyright (c) 2010 Cavium, Inc. <support@cavium.com>. All rights
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

#include <common.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-uctlx-defs.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/cvmx-helper.h>
#include <asm/arch/cvmx-helper-board.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

extern uint64_t octeon_get_io_clock_rate(void);

void __octeon2_usb_power_enable(int enabled)
{
	return;
}

void octeon2_usb_power_enable(int enabled)
	__attribute__((weak, alias("__octeon2_usb_power_enable")));

void octeon2_usb_clock_start(void)
{

	/* Initialize the clocks */
	/* This code is mostly taken as-is from octeon2-common.c from the
	 * Linux driver.
	 */
	cvmx_helper_board_usb_clock_types_t	clock_type;
	cvmx_uctlx_if_ena_t			if_ena;
	cvmx_uctlx_clk_rst_ctl_t		clk_rst_ctl;
	cvmx_uctlx_uphy_portx_ctl_status_t	port_ctl_status;
	cvmx_uctlx_ehci_fla_t			ehci_fla;
	u64					div;
	int					i;
	char					*hdiv_override = getenv("HDIV");

	/*
	 * Step 1: Wait for voltages stable.  That surely happened
	 * before software is running.
	 *
	 * Step 2: Enable  SCLK of UCTL by writing UCTL0_IF_ENA[EN] = 1
	 */
	if_ena.u64 = 0;
	cvmx_write_csr(CVMX_UCTLX_IF_ENA(0), if_ena.u64);
	mdelay(10);
	if_ena.s.en = 1;
	cvmx_write_csr(CVMX_UCTLX_IF_ENA(0), if_ena.u64);

	debug("%s: Setting port control status register\n", __func__);
	for (i = 0; i <= 1; i++) {
		port_ctl_status.u64 = cvmx_read_csr(CVMX_UCTLX_UPHY_PORTX_CTL_STATUS(i, 0));
		/* Set txvreftune to 15 to obtain complient 'eye' diagram. */
		port_ctl_status.s.txvreftune = 15;
		port_ctl_status.s.txrisetune = 1;
		port_ctl_status.s.txpreemphasistune = 1;
		cvmx_write_csr(CVMX_UCTLX_UPHY_PORTX_CTL_STATUS(i, 0), port_ctl_status.u64);
	}

	/* Step 3: Configure the reference clock, PHY, and HCLK */
	clk_rst_ctl.u64 = cvmx_read_csr(CVMX_UCTLX_CLK_RST_CTL(0));

	/*
	 * If the UCTL looks like it has already been started, skip
	 * the initialization, otherwise bus errors are obtained.
	 */
	if (clk_rst_ctl.s.hrst) {
		debug("UCTL clock already started!\n");
		goto end_clock;
	}
	/* 3a */
	clk_rst_ctl.s.p_por = 1;
	clk_rst_ctl.s.hrst = 0;
	clk_rst_ctl.s.p_prst = 0;
	clk_rst_ctl.s.h_clkdiv_rst = 0;
	clk_rst_ctl.s.o_clkdiv_rst = 0;
	clk_rst_ctl.s.h_clkdiv_en = 0;
	clk_rst_ctl.s.o_clkdiv_en = 0;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* 3b */
	clock_type = __cvmx_helper_board_usb_get_clock_type();

	switch (clock_type) {
	case USB_CLOCK_TYPE_REF_24:
		clk_rst_ctl.s.p_refclk_div = 1;
		break;
	case USB_CLOCK_TYPE_REF_48:
		clk_rst_ctl.s.p_refclk_div = 2;
		break;
	case USB_CLOCK_TYPE_REF_12:
	case USB_CLOCK_TYPE_CRYSTAL_12:
	default:
		clk_rst_ctl.s.p_refclk_div = 0;
		break;
	}

	if (clock_type == USB_CLOCK_TYPE_CRYSTAL_12) {
		/* USB_X0 and USB_X1 uses 12 MHz */
		debug("%s: Using 12 MHz crystal clock\n", __func__);
		clk_rst_ctl.s.p_refclk_sel = 0;
	} else {
		/* USB_X0 uses 12/24/48 MHz and USB_X1 is grounded */
		debug("%s: Using 12/24/48 MHz external clock\n", __func__);
		clk_rst_ctl.s.p_refclk_sel = 1;
	}
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* 3c */
	div = octeon_get_io_clock_rate() / 130000000ull;

	switch (div) {
		case 0:
			div = 1;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			break;
		case 5:
			div = 4;
			break;
		case 6:
		case 7:
			div = 6;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			div = 8;
			break;
		default:
			div = 12;
			break;
	}
	if (hdiv_override) {
		printf("----Env var HDIV: using H_DIV = %s----\n", hdiv_override);
		div = (u64)simple_strtoul(hdiv_override, NULL, 10);
	}
	debug("%s: Using divisor of %llu\n", __func__, div);
	clk_rst_ctl.s.h_div = div;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);
	/* Read it back, */
	clk_rst_ctl.u64 = cvmx_read_csr(CVMX_UCTLX_CLK_RST_CTL(0));
	clk_rst_ctl.s.h_clkdiv_en = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);
	/* 3d */
	clk_rst_ctl.s.h_clkdiv_rst = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* 3e */
	udelay(1);
	/*octeon_io_clk_delay(64);*/

	/*
	 * Step 4: Program the power-on reset field in the UCTL
	 * clock-reset-control register.
	 */
	clk_rst_ctl.s.p_por = 0;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* Step 5:    Wait 3 ms for the PHY clock to start. */
	mdelay(3);

	/* Steps 6..9 for ATE only, are skipped. */

	/* Step 10: Configure the OHCI_CLK48 and OHCI_CLK12 clocks. */
	/* 10a */
	clk_rst_ctl.s.o_clkdiv_rst = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* 10b */
	clk_rst_ctl.s.o_clkdiv_en = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* 10c */
	udelay(1);
	/*octeon_io_clk_delay(64);*/

	/* Step 11: Program the PHY reset field: UCTL0_CLK_RST_CTL[P_PRST] = 1
	 */
	clk_rst_ctl.s.p_prst = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* Step 11b */
	udelay(1);

	/* Step 11c */
	clk_rst_ctl.s.p_prst = 0;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* Step 11d */
	mdelay(1);

	/* Step 11e */
	clk_rst_ctl.s.p_prst = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

	/* Step 12: Wait 1 us. */
	udelay(1);

	/* Step 13: Program the HRESET_N field: UCTL0_CLK_RST_CTL[HRST] = 1 */
	clk_rst_ctl.s.hrst = 1;
	cvmx_write_csr(CVMX_UCTLX_CLK_RST_CTL(0), clk_rst_ctl.u64);

 end_clock:
	/* Now we can set some other registers.  */

	/* Fix the EHCI timing. */
	ehci_fla.u64 = cvmx_read_csr(CVMX_UCTLX_EHCI_FLA(0));
	ehci_fla.s.fla = 0x20;
	cvmx_write_csr(CVMX_UCTLX_EHCI_FLA(0), ehci_fla.u64);
}

void octeon2_usb_clock_stop(void)
{
	/* We don't disable the clock of the controller because the kernel
	 * can't reinitialize it.
	 */
}
