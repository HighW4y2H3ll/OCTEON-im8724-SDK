/*
 * (C) Copyright 2004-2007, 2011
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
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <linux/ctype.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	const char *board_type_str = getenv("board_type");

	if (board_type_str) {
		if (isdigit(*board_type_str)) {
			/* Numeric, take as-is */
			gd->arch.board_desc.board_type =
				simple_strtoul(board_type_str, NULL, 0);
		} else {
			/* Convert from string to number */
			int i;
			for (i = 1; i < CVMX_BOARD_TYPE_MAX; i++) {
				if (!strcmp(board_type_str,
					    cvmx_board_type_to_string(i))) {
					gd->arch.board_desc.board_type = i;
					break;
				}
			}
		}
		gd->board_type = gd->arch.board_desc.board_type;
	}

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	/* Enable MDIO */
	cvmx_write_csr(CVMX_SMI_EN, 1);
#endif
	return 0;
}

int early_board_init(void)
{
	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	gd->board_type = gd->arch.board_desc.board_type = GENERIC_BOARD_TYPE;

	/* Determine board type/rev */
	strncpy((char *)(gd->arch.board_desc.serial_str), "unknown", SERIAL_LEN);
	gd->mem_clk = 0;

	{
		/* Make up some MAC addresses */
		gd->arch.mac_desc.count = 255;
		gd->arch.mac_desc.mac_addr_base[0] = 0x00;
		gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
		gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
		gd->arch.mac_desc.mac_addr_base[3] =
		    (gd->arch.board_desc.rev_major << 4) |
		    gd->arch.board_desc.rev_minor;
		gd->arch.mac_desc.mac_addr_base[4] =
		    gd->arch.board_desc.serial_str[0];
		gd->arch.mac_desc.mac_addr_base[5] = 0x00;
	}

	return 0;
}
