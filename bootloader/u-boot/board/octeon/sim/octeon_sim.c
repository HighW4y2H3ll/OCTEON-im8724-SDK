/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/cvmx-sim-magic.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	if (OCTEON_IS_MODEL(OCTEON_CN31XX)
	    || OCTEON_IS_MODEL(OCTEON_CN30XX)
	    || OCTEON_IS_MODEL(OCTEON_CN50XX)) {
		/* Enable RGMII/GMII interface block */
		cvmx_gmxx_inf_mode_t mode;
		mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));
		mode.s.en = 1;
		cvmx_write_csr(CVMX_GMXX_INF_MODE(0), mode.u64);
	}
	/* See if we need to adjust DRAM width for CN3020 simulation */
	if (OCTEON_IS_MODEL(OCTEON_CN3020)) {
		/* We are a 3020, so make the DDR interface 32 bits */
		cvmx_lmc_ctl_t lmc_ctl;
		lmc_ctl.u64 = cvmx_read_csr(CVMX_LMC_CTL);
		lmc_ctl.cn31xx.mode32b = 1;
		cvmx_write_csr(CVMX_LMC_CTL, lmc_ctl.u64);
	}
	return 0;
}

int early_board_init(void)
{
	gd->arch.board_desc.board_type = CVMX_BOARD_TYPE_SIM;
	gd->board_type = gd->arch.board_desc.board_type;
	gd->arch.board_desc.rev_major = 1;
	gd->arch.board_desc.rev_minor = 0;

	gd->cpu_clk = cvmx_sim_magic_get_cpufreq();

	/* Make up some MAC addresses */
	gd->arch.mac_desc.count = 255;
	gd->arch.mac_desc.mac_addr_base[0] = 0x00;
	gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
	gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
	gd->arch.mac_desc.mac_addr_base[3] = 0xbe;
	gd->arch.mac_desc.mac_addr_base[4] = 0xef;
	gd->arch.mac_desc.mac_addr_base[5] = 0x00;

	return 0;
}
