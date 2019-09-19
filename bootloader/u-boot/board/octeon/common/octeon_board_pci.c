/*
 * (C) Copyright 2011
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

#include <common.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/octeon-model.h>
#include <asm/arch/octeon-feature.h>

extern void init_octeon_pcie(void);
extern void init_octeon_pci(void);

void pci_init_board(void)
{
	if (octeon_has_feature(OCTEON_FEATURE_PCIE)) {
		debug("Initializing PCI Express bus\n");
		init_octeon_pcie();
	} else {
		debug("Initializing PCI bus\n");
		init_octeon_pci();
	}
}

void pci_init_board(void) __attribute__((__weak__));

