/*
 * (C) Copyright 2012 Cavium, Inc.
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
#ifndef __OCTEON_BOARD_MMC_H__
#define __OCTEON_BOARD_MMC_H__

#include <common.h>
#include <asm/arch/cvmx.h>

#define OCTEON_MAX_MMC_SLOT	4

/**
 * Data structure for storing MMC slot information
 */
struct octeon_mmc_slot_info {
	int		chip_sel;	/** Octeon chip select */
	int		node;		/** Octeon OCX node */
	uint32_t	max_frequency;	/** Maximum supported frequency */
	int		bus_max_width;	/** Maximum supported bus width */
	int		wp_gpio;	/** Write-protect GPIO (-1 for none) */
	int		wp_active_low;	/** 1 if active low */
	int		write_protect;	/** 1 if write protect active */
	int		cd_gpio;	/** Card-detect GPIO (-1 for none) */
	int		cd_active_low;	/** 1 if active low */
	int		power_gpio;	/** Power GPIO (-1 for none) */
	int		power_active_low;/** 1 if active low */
	int 		cmd_clk_skew;	/** command signal clk skew in ps */
	int		dat_clk_skew;	/** data signal clk skew in ps */
	bool		non_removable;	/** Device is not removable */
};

struct octeon_mmc_info {
	struct octeon_mmc_slot_info slot[OCTEON_MAX_MMC_SLOT];
};

int octeon_boot_bus_mmc_get_info(int node, struct octeon_mmc_info *mmc_info);


#endif /* __OCTEON_BOARD_MMC_H__ */
