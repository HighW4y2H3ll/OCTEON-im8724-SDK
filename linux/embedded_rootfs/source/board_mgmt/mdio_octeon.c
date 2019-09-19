/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2009-2012 Cavium, Inc.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include <cvmx.h>
#include <cvmx-platform.h>
#include <cvmx-mdio.h>

#include "mdio_octeon.h"

#define SMI_CMD				0x0
#define SMI_WR_DAT			0x8
#define SMI_RD_DAT			0x10
#define SMI_CLK				0x18
#define SMI_EN				0x20

#define OCTEON_SMI_ADDR_BASE		0x1180000003800
#define OCTEON_SMI_BUSNUM		4

enum octeon_mdiobus_mode {
	UNINIT = 0,
	C22,
	C45
};

typedef struct mdiobus_mode_s {
	enum octeon_mdiobus_mode mode;
	uint64_t regbase;
} mdiobus_mode_t;

static void octeon_mdiobus_set_mode(int bus, uint64_t register_base,
				    enum octeon_mdiobus_mode m)
{
	static mdiobus_mode_t bm[OCTEON_SMI_BUSNUM];
	union cvmx_smix_clk smi_clk;

	if (m == bm[bus].mode && register_base == bm[bus].regbase)
		return;

	smi_clk.u64 = cvmx_read_csr(CVMX_ADD_IO_SEG(register_base + SMI_CLK));
	smi_clk.s.mode = (m == C45) ? 1 : 0;
	smi_clk.s.preamble = 1;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_CLK), smi_clk.u64);
	bm[bus].regbase = register_base;
	bm[bus].mode = m;
}

static int octeon_mdiobus_c45_addr(int bus, uint64_t register_base,
				   int phy_id, int regnum)
{
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_wr_dat smi_wr;
	int timeout = 1000;

	octeon_mdiobus_set_mode(bus, register_base, C45);

	smi_wr.u64 = 0;
	smi_wr.s.dat = regnum & 0xffff;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_WR_DAT), smi_wr.u64);

	regnum = (regnum >> 16) & 0x1f;

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 0; /* MDIO_CLAUSE_45_ADDRESS */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_CMD), smi_cmd.u64);

	do {
		usleep(1000);
		smi_wr.u64 = cvmx_read_csr(CVMX_ADD_IO_SEG(register_base + SMI_WR_DAT));
	} while (smi_wr.s.pending && --timeout);

	if (timeout <= 0)
		return -EIO;
	return 0;
}

int octeon_mdiobus_read(int bus, int phy_id, int regnum)
{
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_rd_dat smi_rd;
	uint64_t register_base;
	unsigned int op = 1; /* MDIO_CLAUSE_22_READ */
	int timeout = 1000;

	if (bus < 0 || bus > 3) return -1;
	register_base = OCTEON_SMI_ADDR_BASE + (bus << 7);

	if (regnum & MII_ADDR_C45) {
		int r = octeon_mdiobus_c45_addr(bus, register_base, phy_id, regnum);
		if (r < 0)
			return r;
		regnum = (regnum >> 16) & 0x1f;
		op = 3; /* MDIO_CLAUSE_45_READ */
	} else {
		octeon_mdiobus_set_mode(bus, register_base, C22);
	}

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = op;
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_CMD), smi_cmd.u64);

	do {
		usleep(1000);
		smi_rd.u64 = cvmx_read_csr(CVMX_ADD_IO_SEG(register_base + SMI_RD_DAT));
	} while (smi_rd.s.pending && --timeout);

	if (smi_rd.s.val)
		return smi_rd.s.dat;
	else
		return -EIO;
}

int octeon_mdiobus_write(int bus, int phy_id, int regnum, uint16_t val)
{
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_wr_dat smi_wr;
	uint64_t register_base;
	unsigned int op = 0; /* MDIO_CLAUSE_22_WRITE */
	int timeout = 1000;

	if (bus < 0 || bus > 3) return -1;
	register_base = OCTEON_SMI_ADDR_BASE + (bus << 7);

	if (regnum & MII_ADDR_C45) {
		int r = octeon_mdiobus_c45_addr(bus, register_base, phy_id, regnum);
		if (r < 0)
			return r;
		regnum = (regnum >> 16) & 0x1f;
		op = 1; /* MDIO_CLAUSE_45_WRITE */
	} else {
		octeon_mdiobus_set_mode(bus, register_base, C22);
	}

	smi_wr.u64 = 0;
	smi_wr.s.dat = val;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_WR_DAT), smi_wr.u64);

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = op;
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_ADD_IO_SEG(register_base + SMI_CMD), smi_cmd.u64);

	do {
		usleep(1000);
		smi_wr.u64 = cvmx_read_csr(CVMX_ADD_IO_SEG(register_base + SMI_WR_DAT));
	} while (smi_wr.s.pending && --timeout);

	if (timeout <= 0)
		return -EIO;
	return 0;
}

