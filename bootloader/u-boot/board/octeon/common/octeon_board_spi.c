/*
 * (C) Copyright 2013 Cavium, Inc. <support@cavium.com>
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
#include <libfdt.h>
#include <fdt_support.h>
#include <fdt.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/octeon_board_mmc.h>
#include <asm/arch/octeon-feature.h>


int __octeon_spi_flash_initialize(int nodeoffset)
	__attribute__((weak, alias("octeon_spi_flash_initialize")));

int __octeon_spi_flash_initialize (int nodeoffset)
{
	int cs;
	uint32_t max_freq = 25000000;
	int cpha, cpol;
	int pagesize;
	int erasesize;
	uint64_t size;
	int address_width;

	cs = fdtdec_get_int(gd->fdt_blob, nodeoffset, "reg", -1);
	if (cs < 0) {
		puts("Invalid SPI flash reg address\n");
		return -1;
	}
	max_freq = fdtdec_get_int(gd->fdt_blob, nodeoffset, "spi-max-frequency",
				  max_freq);
	cpha = fdtdec_get_bool(gd->fdt_blob, nodeoffset, "spi-cpha");
	cpol = fdtdec_get_bool(gd->fdt_blob, nodeoffset, "spi-cpol");
	pagesize = fdtdec_get_int(gd->fdt_blob, nodeoffset, "pagesize", 256);
	erasesize = fdtdec_get_int(gd->fdt_blob, nodeoffset, "erasesize", -1);
	address_width = fdtdec_get_int(gd->fdt_blob, nodeoffset,
				       "address-width", 24);
}


int __octeon_spi_board_initialize(void)
	__attribute__((weak, alias("octeon_spi_board_initialize")));

int __octeon_spi_board_initialize(void)
{
	int i;
	int nodeoffset, next_nodeoffset;
	const void *nodep;
	u32 *pgpio_handle;
	int gpio_offset;
	int phandle;
	int max_spi = 2;
	u32 tag;
	int level = 0;

	debug("%s: Entry\n", __func__);
	if (!octeon_has_feature(OCTEON_FEATURE_SPI))
		return -1;

	nodeoffset = fdt_path_offset(gd->fdt_blob, "/soc/spi");
	if (nodeoffset < 0) {
		debug("SPI interface not found in device tree\n");
		return -1;
	}

	if (fdt_node_check_compatible(gd->fdt_blob, nodeoffset,
				      "cavium,octeon-3010-spi")) {
		puts("Incompatible SPI interface type\n");
		return -1;
	}

	while ((next_nodeoffset = fdt_next_node(gd->fdt_blob,
						nodeoffset, &level)) > 0) {
		if (level < 0)
			break;
		if (level > 1)
			continue;
		if (fdt_node_check_compatible(gd->fdt_blob,
					      next_nodeoffset,
					      "spi-flash")) {

		}
	}
}
