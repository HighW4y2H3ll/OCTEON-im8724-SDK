/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
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

/*
 * This file contains functions that tend to be board specific related to IDE
 * and compact flash.  Two modes are supported, both True IDE mode and
 * non-True IDE mode.
 *
 * All of the functions are defined as weak.  Rather than changing these
 * functions they should be re-implemented in the board specific files and
 * the linker will pick up those instead.
 */

#include <common.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <fdt.h>
#include <asm/arch/octeon_cf.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-gpio.h>
#include <asm/arch/octeon_boot_bus.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * Check and return if a CF card is present.
 *
 * @param base_addr - base virtual address for compact flash
 *
 * @return 0 if not present, 1 if present
 */
int __octeon_cf_present(unsigned long base_addr)
{
#if !CONFIG_OCTEON_SIM_NO_FLASH
	uint32_t *ptr = (uint32_t *)base_addr;
	debug("%s: Checking if IDE present at 0x%lx\n", __func__, base_addr);
	debug("%s: Value at 0x%p is 0x%x\n", __func__, ptr, *ptr);
	return (*ptr != 0xffffffff);
#else
	return 0;
#endif
}
int octeon_cf_present(unsigned long base_addr)
	__attribute__((weak, alias("__octeon_cf_present")));

#ifdef CONFIG_IDE_PREINIT
/**
 * Pre-initialize the IDE bus.
 *
 * For Octeon we just reset the IDE bus.
 */
int __ide_preinit(void)
{
	/* Reset the CF card, as this seems to be necessary in some cases
	 * when using DMA
	 */
	ide_set_reset(1);
	udelay(50000);
	ide_set_reset(0);
	udelay(1000);
	return 0;
}

int ide_preinit(void) __attribute__((weak, alias("__ide_preinit")));
#endif
