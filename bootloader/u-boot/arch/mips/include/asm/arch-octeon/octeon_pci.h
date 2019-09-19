/*
 * (C) Copyright 2006-2013
 * Cavium Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/**
 * @file octeon_pci.h
 *
 * $Id: octeon_pci.h 152211 2017-01-11 21:25:03Z cchavva $
 *
 */
#ifndef __OCTEON_PCI_H__
#define __OCTEON_PCI_H__

/* Give lots of space for U-Boot since it's not as effecient at allocating
 * space as Linux is.
 */
#define OCTEON_PCI_SLOT0_BAR_ADDR	0xE0000000
#define OCTEON_PCI_SLOT0_BAR_SIZE	0x08000000
#define OCTEON_PCI_SLOT1_BAR_ADDR	0xE8000000
#define OCTEON_PCI_SLOT1_BAR_SIZE	0x08000000
#define OCTEON_PCI_SLOT2_BAR_ADDR	0xF0000000
#define OCTEON_PCI_SLOT2_BAR_SIZE	0x08000000
#define OCTEON_PCI_SLOT3_BAR_ADDR	0xF8000000
#define OCTEON_PCI_SLOT3_BAR_SIZE	0x08000000
#define OCTEON_PCI_TOTAL_BAR_SIZE	(OCTEON_PCI_SLOT0_BAR_SIZE + \
					 OCTEON_PCI_SLOT1_BAR_SIZE + \
					 OCTEON_PCI_SLOT2_BAR_SIZE + \
					 OCTEON_PCI_SLOT3_BAR_SIZE)

/**
 * EEPROM entry struct
 */
union octeon_pcie_eeprom {
 uint64_t u64;
#ifdef __BIG_ENDIAN_BITFIELD
	struct octeon_data_s {
		/**
		 * 0x9DA1 valid entry, 0x6A5D end of table, 0xffff invalid
		 * access
		 */
		uint64_t preamble:16;
		uint64_t :1;	/** Reserved */
		/** Physical function number accessed by the write operation. */
		uint64_t pf:2;
		/**
		 * Specifies bit<31> of the address written by hardware.
		 * 1 = configuration mask register, 0 = configuration register
		 */
		uint64_t cs2:1;
		/**
		 * Specifies bits<11:0> of the address written by hardware.
		 * Bits<30:12> of this address are all 0s.
		 */
		uint64_t address:12;
		uint64_t data:32;
	} s;
#else
	struct octeon_data_s {
		uint64_t data:32;
		uint64_t address:12;
		uint64_t cs2:1;
		uint64_t pf:2;
		uint64_t :1;
		uint64_t preamble:16;
	} s;
#endif
};

void pci_dev_post_init(void);


int octeon_pci_io_readb (unsigned int reg);
void octeon_pci_io_writeb (int value, unsigned int reg);
int octeon_pci_io_readw (unsigned int reg);
void octeon_pci_io_writew (int value, unsigned int reg);
int octeon_pci_io_readl (unsigned int reg);
void octeon_pci_io_writel (int value, unsigned int reg);
int octeon_pci_mem1_readb (unsigned int reg);
void octeon_pci_mem1_writeb (int value, unsigned int reg);
int octeon_pci_mem1_readw (unsigned int reg);
void octeon_pci_mem1_writew (int value, unsigned int reg);
int octeon_pci_mem1_readl (unsigned int reg);
void octeon_pci_mem1_writel (int value, unsigned int reg);



/* In the TLB mapped case, these also work with virtual addresses,
** and do the required virt<->phys translations as well. */
uint32_t octeon_pci_phys_to_bus(uint32_t phys);
uint32_t octeon_pci_bus_to_phys(uint32_t bus);

/**
 * Searches PCIe EEPROM for override data specified by address and pf.
 *
 * @param	address - PCIe config space address
 * @param	pf	- PCIe config space pf num
 * @param[out]	id	- override device and vendor ID
 *
 * @return	0 if override found, 1 if not found.
 */
int octeon_find_pcie_id_override(unsigned address, unsigned pf, uint32_t *id);

#endif /* __OCTEON_PCI_H__ */
