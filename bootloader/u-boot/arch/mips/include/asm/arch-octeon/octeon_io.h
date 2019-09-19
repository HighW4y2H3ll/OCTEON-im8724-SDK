/***********************license start************************************
 * Copyright (c) 2011 Cavium Inc. (support@cavium.com). All rights
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
#ifndef __OCTEON_IO_H__
#define __OCTEON_IO_H__

#define CVMX_OCT_SUBDID_PCI_CFG     1
#define CVMX_OCT_SUBDID_PCI_IO      2
#define CVMX_OCT_SUBDID_PCI_MEM0    3
#define CVMX_OCT_SUBDID_PCI_MEM1    4
#define CVMX_OCT_SUBDID_PCI_MEM2    5
#define CVMX_OCT_SUBDID_PCI_MEM3    6

#define OCTEON_USB_EHCI_OHCI_PHYS_BASE_ADDR	(0x80016F0000000000ull)

#define OCTEON_PCIE_CFG_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 1))

#define OCTEON_PCIE_IO_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 2))

#define OCTEON_PCIE_MEM0_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 3))

#define OCTEON_PCIE_MEM1_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 4))

#define OCTEON_PCIE_MEM2_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 5))

#define OCTEON_PCIE_MEM3_PHYS_BASE_ADDR	\
		    CVMX_ADDR_DID(CVMX_FULL_DID(CVMX_OCT_DID_PCI, 6))
#define OCTEON_BOOT_BUS_VIRT_BASE_ADDR		0x10000000
#define OCTEON_BOOT_BUS_SIZE			0x0fffffff
#define OCTEON_USB_EHCI_OHCI_VIRT_BASE_ADDR	0xefff0000
#define OCTEON_USB_EHCI_OHCI_SIZE		0x00001000
#define OCTEON_PCIE_IO0_VIRT_BASE_ADDR		0x00001000
#define OCTEON_PCIE_IO0_SIZE			0x000ff000
#define OCTEON_PCIE_IO1_VIRT_BASE_ADDR		0x00100000
#define OCTEON_PCIE_IO1_VIRT_SIZE		0xffffffff
#define OCTEON_PCIE_MEM0_VIRT_BASE_ADDR		0xf0000000
#define OCTEON_PCIE_MEM0_SIZE			0x08000000
#define OCTEON_PCIE_MEM1_VIRT_BASE_ADDR		0xf8000000
#define OCTEON_PCIE_MEM1_SIZE			0x08000000


#endif /* __OCTEON_IO_H__ */