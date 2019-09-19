/*
 * (C) Copyright 2014 Cavium Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

static void kill_fdt_phy(void *fdt, int offset, void *arg)
{
	int len, phy_offset;
	const fdt32_t *php;
	uint32_t phandle;

	php = fdt_getprop(fdt, offset, "phy-handle", &len);
	if (php && len == sizeof(*php)) {
		phandle = fdt32_to_cpu(*php);
		fdt_nop_property(fdt, offset, "phy-handle");
		phy_offset = fdt_node_offset_by_phandle(fdt, phandle);
		if (phy_offset > 0)
			fdt_nop_node(fdt, phy_offset);
	}
}

int board_fixup_fdt(void)
{
	char fdt_key[16];
	int qlm;
	char env_var[16];
	const char *mode;

	/* QLMs 0 and 1 can only be configured as PCIe */
	for (qlm = 2; qlm <= 4; qlm++) {
		sprintf(env_var, "qlm%d:0_mode", qlm);
		mode = getenv(env_var);
		sprintf(fdt_key, "%d,none", qlm);
		if (!mode) {
			sprintf(fdt_key, "%d,none", qlm);
		} else if (!strncmp(mode, "sgmii", 5)) {
			sprintf(fdt_key, "%d,sgmii", qlm);
		} else if (!strncmp(mode, "xaui", 4)) {
			sprintf(fdt_key, "%d,xaui", qlm);
		} else if (!strncmp(mode, "dxaui", 5)) {
			sprintf(fdt_key, "%d,dxaui", qlm);
		} else if (!strncmp(mode, "rxaui", 5)) {
			sprintf(fdt_key, "%d,rxaui", qlm);
		} else if (!strncmp(mode, "xlaui", 5)) {
			sprintf(fdt_key, "%d,xlaui", qlm);
			if (qlm < 2)
				printf("Error: XLAUI not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,xlaui", qlm);
		} else if (!strncmp(mode, "xfi", 3)) {
			if (qlm < 2)
				printf("Error: XFI not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,xfi", qlm);
		} else if (!strncmp(mode, "10G_KR", 6)) {
			sprintf(fdt_key, "%d,10G_KR", qlm);
		} else if (!strncmp(mode, "40G_KR4", 7)) {
			sprintf(fdt_key, "%d,40G_KR4", qlm);
		}
		debug("Patching qlm %d for %s\n", qlm, fdt_key);
		octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL, true,
					strstr(mode, ",no_phy") ? kill_fdt_phy : NULL, NULL);
	}

	return 0;
}

/* Here is the description of the parameters that are passed to QLM configuration
 * 	param0 : The QLM to configure
 * 	param1 : Speed to configure the QLM at
 * 	param2 : Mode the QLM to configure
 * 	param3 : 1 = RC, 0 = EP
 * 	param4 : 0 = GEN1, 1 = GEN2, 2 = GEN3
 * 	param5 : ref clock select, 0 = 100Mhz, 1 = 125MHz, 2 = 156MHz
 * 	param6 : ref clock input to use:
 * 		 0 - external reference (QLMx_REF_CLK)
 * 		 1 = common clock 0 (QLMC_REF_CLK0)
 * 		 2 = common_clock 1 (QLMC_REF_CLK1)
 */
int checkboard(void)
{
	int qlm;
	char env_var[16];
	int node = 0;
	int speed[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int mode[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	int pcie_rc[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int pcie_gen[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int ref_clock_sel[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int ref_clock_input[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	/* Since i2c is broken on pass 1.0 we use an environment variable */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_0)) {
		ulong board_version;
		board_version = getenv_ulong("board_version", 10, 1);

		gd->arch.board_desc.rev_major = board_version;
	}

	octeon_init_qlm(0);

	for (qlm = 0; qlm < 5; qlm++) {
		const char *mode_str;

		mode[qlm] = CVMX_QLM_MODE_DISABLED;
		sprintf(env_var, "qlm%d:0_mode", qlm);
		mode_str = getenv(env_var);
		if (!mode_str)
			continue;
		if (!strncmp(mode_str, "sgmii", 5)) {
			if (qlm < 2) {
				printf("Error: SGMII not supported on NODE 0:QLM %d\n", qlm);
				continue;
			}
			speed[qlm] = 1250;
			mode[qlm] = CVMX_QLM_MODE_SGMII;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: SGMII\n", qlm);
		} else if (!strncmp(mode_str, "xaui", 4)) {
			if (qlm < 2) {
				printf("Error: XAUI not supported on NODE 0:QLM %d\n", qlm);
				continue;
			}
			speed[qlm] = 3125;
			mode[qlm] = CVMX_QLM_MODE_XAUI;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: XAUI\n", qlm);
		} else if (!strncmp(mode_str, "dxaui", 5)) {
			if (qlm < 2) {
				printf("Error: DXAUI not supported on NODE 0:QLM %d\n", qlm);
				continue;
			}
			speed[qlm] = 6250;
			mode[qlm] = CVMX_QLM_MODE_XAUI;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: DXAUI\n", qlm);
		} else if (!strncmp(mode_str, "rxaui", 5)) {
			if (qlm < 2) {
				printf("Error: RXAUI not supported on NODE 0:QLM %d\n", qlm);
				continue;
			}
			speed[qlm] = 6250;
			mode[qlm] = CVMX_QLM_MODE_RXAUI;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: RXAUI\n", qlm);
		} else if (!strncmp(mode_str, "xlaui", 5)) {
			if (qlm < 2) {
				printf("Error: XLAUI not supported on NODE 0:QLM %d\n",
				       qlm);
				continue;
			}
			speed[qlm] = 103125;
			mode[qlm] = CVMX_QLM_MODE_XLAUI;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: XLAUI\n", qlm);
		} else if (!strncmp(mode_str, "xfi", 3)) {
			if (qlm < 2) {
				printf("Error: XFI not supported on NODE 0:QLM %d\n",
				       qlm);
				continue;
			}
			speed[qlm] = 103125;
			mode[qlm] = CVMX_QLM_MODE_XFI;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: XFI\n", qlm);
		} else if (!strncmp(mode_str, "10G_KR", 6)) {
			if (qlm < 2) {
				printf("Error: 10G_KR not supported on NODE 0:QLM %d\n",
				       qlm);
				continue;
			}
			speed[qlm] = 103125;
			mode[qlm] = CVMX_QLM_MODE_10G_KR;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: 10G_KR\n", qlm);
		} else if (!strncmp(mode_str, "40G_KR4", 7)) {
			if (qlm < 2) {
				printf("Error: 40G_KR4 not supported on NODE 0:QLM %d\n",
				       qlm);
				continue;
			}
			speed[qlm] = 103125;
			mode[qlm] = CVMX_QLM_MODE_40G_KR4;
			ref_clock_sel[qlm] = 2;
			printf("NODE 0:QLM %d: 40G_KR4\n", qlm);
		} else if (!strcmp(mode_str, "pcie")) {
			char *pmode;
			int lanes = 0;

			sprintf(env_var, "pcie%d:0_mode", qlm);
			pmode = getenv(env_var);
			if (pmode && !strcmp(pmode, "ep"))
				pcie_rc[qlm] = 0;
			else
				pcie_rc[qlm] = 1;
			sprintf(env_var, "pcie%d:0_gen", qlm);
			pcie_gen[qlm] = getenv_ulong(env_var, 0, 3);
			sprintf(env_var, "pcie%d:0_lanes", qlm);
			lanes = getenv_ulong(env_var, 0, 8);
			if (lanes == 8)
				mode[qlm] = CVMX_QLM_MODE_PCIE_1X8;
			else
				mode[qlm] = CVMX_QLM_MODE_PCIE;
			ref_clock_sel[qlm] = 0;
			printf("NODE 0:QLM %d: PCIe gen%d %s\n", qlm, pcie_gen[qlm] + 1,
			       pcie_rc[qlm] ? "root complex" : "endpoint");
		} else {
			printf("NODE 0:QLM %d: disabled\n", qlm);
		}
	}

	for (qlm = 0; qlm < 5; qlm++) {
		if (mode[qlm] == -1)
			continue;
		debug("Configuring qlm%d with speed(%d), mode(%d), RC(%d), Gen(%d), REF_CLK(%d), CLK_SOURCE(%d)\n",
			 qlm, speed[qlm], mode[qlm], pcie_rc[qlm],
			 pcie_gen[qlm] + 1, ref_clock_sel[qlm], ref_clock_input[qlm]);
		octeon_configure_qlm_cn78xx(0, qlm, speed[qlm], mode[qlm],
				     pcie_rc[qlm], pcie_gen[qlm],
				     ref_clock_sel[qlm], ref_clock_input[qlm]);
	}
	return 0;
}

int early_board_init(void)
{
	int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

	octeon_board_get_clock_info(TB7600_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_TB7600, 1, 0);

	/* CN63XX has a fixed 50 MHz reference clock */
	gd->arch.ddr_ref_hertz = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull);

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}
