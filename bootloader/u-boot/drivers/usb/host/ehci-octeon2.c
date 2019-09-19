/***********************license start************************************
 * Copyright (c) 2010 Cavium Inc. (support@cavium.com). All rights
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
#include <usb.h>
#include "ehci.h"
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-uahcx-defs.h>
#include <libfdt.h>

extern void octeon2_usb_clock_start(void);
extern void octeon2_usb_clock_stop(void);
extern int octeon_bist_6XXX_usb(void);

DECLARE_GLOBAL_DATA_PTR;

#if 0
static const char ehci_reg_to_name(uint32_t *addr32)
{
	uint32_t addr = (uint32_t)addr32;
	switch (addr) {
	/* HCCR */
	case 0x0:	return "CAPBASE";
	case 0x4:	return "HCSPARAMS";
	case 0x8:	return "HCCPARAMS";

	/* HCOR */
	case 0x0:	return "USBCMD";
	case 0x4:	return "USBSTS";
	case 0x8:	return "USBINTR";
	case 0xc:	return "FRINDEX";
	case 0x10:	return "CTRLDSSEGMENT";
	case 0x14:	return "PERIODICLISTBASE";
	case 0x18:	return "ASYNCLISTADDR";
	case 0x20:	return "BURSTSIZE";
	case 0x24:	return "TXFILLTUNING";
	case 0x40:	return "CONFIGFLAG";
	case 0x44:	return "PORTSC0";
	case 0x48:	return "PORTSC1";
	default:	return "UNKNOWN";
	}
}
#endif

u32 ehci_readl(volatile u32 *addr32)
{
	u64 addr64 = 0x80016F0000000000ull + (ulong)addr32;
	u32 val;

	val = cvmx_read64_uint32(addr64);
	debug("%s: read 0x%08x from address %p (0x%llx)\n",
	      __func__, val, addr32, addr64);
	return val;
}

void ehci_writel(volatile u32 *addr32, u32 val)
{
	u64 addr64 = 0x80016F0000000000ull + (ulong)addr32;
	u32 val2 = 0;
	cvmx_write64_uint32(addr64, val);
	val2 = cvmx_read64_uint32(addr64);
	debug("%s: wrote 0x%08x to address %p (0x%llx), read back 0x%08x\n",
	      __func__, val, addr32, addr64, val2);
}

int ehci_hcd_init(int index, enum usb_init_type init,
		  struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	union cvmx_uctlx_ehci_ctl ehci_ctl;

	/* Make sure USB is present in the device tree */
	if (fdt_node_offset_by_compatible(gd->fdt_blob, 0,
					  "cavium,octeon-6335-uctl") < 0) {
		puts("USB not present.\n");
		return -1;
	}

	octeon2_usb_clock_start();

	debug("%s(%d, %p (%p), %p (%p))\n", __func__, index, hccr, *hccr,
	      hcor, *hcor);

	/* ehci byte swap */
	ehci_ctl.u64 = cvmx_read_csr(CVMX_UCTLX_EHCI_CTL(0));
	/* For 64-bit addressing, we do it here rather than EHCI */
	ehci_ctl.s.ehci_64b_addr_en = 1;    /* Add 64-bit addressing in EHCI */
	ehci_ctl.s.l2c_addr_msb = 0;
	ehci_ctl.s.l2c_buff_emod = 1; /* Byte swapped. */
	ehci_ctl.s.l2c_desc_emod = 1; /* Byte swapped. */
	cvmx_write_csr(CVMX_UCTLX_EHCI_CTL(0), ehci_ctl.u64);

	*hccr = (struct ehci_hccr *)NULL;	/* We use the correct address in the access funcs */
	*hcor = CASTPTR(struct ehci_hcor,
			(u32)(*hccr) + HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));
	debug("%s: hccr: 0x%p, hcor: 0x%p\n", __func__, *hccr, *hcor);
	if (octeon_bist_6XXX_usb()) {
		puts("Error: USB failed BIST test!\n");
		return -1;
	}

	return 0;
}

int ehci_hcd_stop(int index)
{
	octeon2_usb_clock_stop();

	return 0;
}
