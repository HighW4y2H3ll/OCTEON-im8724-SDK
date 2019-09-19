/*
 * (C) Copyright 2015 Cavium Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/cvmx-sim-magic.h>
#include <asm/arch/cvmx-qlm.h>
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx-helper-util.h>
#include <asm/arch/cvmx-helper-cfg.h>
#include <asm/arch/cvmx-smi-defs.h>
#include <asm/gpio.h>

#define MAX_MIX_ENV_VARS	2

extern void print_isp_volt(const char **labels, uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts(uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts_avg(int loops, uint16_t isp_dev_addr, uint8_t adc_chan);

DECLARE_GLOBAL_DATA_PTR;

/*
 * parse_env_var:	Parse the environment variable ("bgx_for_mix%d") to
 *			extract the lmac it is set to.
 *
 *  index:		Index of environment variable to parse.
 *			environment variable.
 *  env_bgx:		Updated with the bgx of the lmac in the environment
 *			variable.
 *  env_lmac:		Updated with the index of lmac in the environment
 *			variable.
 *
 *  returns:		Zero on success, error otherwise.
 */
static int parse_env_var(int index, int *env_bgx, int *env_lmac)
{
	char	env_var[20];
	ulong	xipd_port;

	sprintf(env_var, "bgx_for_mix%d", index);
	if ((xipd_port = getenv_ulong(env_var, 0, 0xffff)) != 0xffff) {
		int xiface;
		struct cvmx_xiface xi;
		struct cvmx_xport xp;

		/*
		 * The environemt variable is set to the xipd port. Convert the
		 * xipd port to numa node, bgx, and lmac.
		 */
		xiface = cvmx_helper_get_interface_num(xipd_port);
		xi = cvmx_helper_xiface_to_node_interface(xiface);
		xp = cvmx_helper_ipd_port_to_xport(xipd_port);
		*env_bgx = xi.interface;
		*env_lmac = cvmx_helper_get_interface_index_num(xp.port);
		return 0;
	}

	return -1;
}

/*
 * get_lmac_fdt_node:	Search the device tree for the node corresponding to
 *			a given bgx lmac.
 *
 *  fdt:		Pointer to flat device tree
 *  search_node:	Numa node of the lmac to search for.
 *  search_bgx:		Bgx of the lmac to search for.
 *  search_lmac:	Lmac index to search for.

 *  returns:		The device tree node of the lmac if found,
 *			or -1 otherwise.
 */
static int get_lmac_fdt_node(const void *fdt, int search_node, int search_bgx,
			     int search_lmac)
{
	int		node;
	const fdt32_t	*reg;
	u64		addr;
	int		fdt_node = -1;
	int		fdt_bgx = -1;
	int		fdt_lmac = -1;
	int		len;
	int		parent;

	/* Iterate through all bgx ports */
	node = -1;
	while ((node = fdt_node_offset_by_compatible((void *)fdt, node,
					"cavium,octeon-7890-bgx-port")) >= 0) {
		/* Get the node and bgx from the physical address */
		if (((parent = fdt_parent_offset(fdt, node)) < 0) ||
		    !(reg = fdt_getprop(fdt, parent, "reg", &len)))
			continue;

		addr = fdt_translate_address((void *)fdt, parent, reg);
		fdt_node = (addr >> 36) & 0x7;
		fdt_bgx = (addr >> 24) & 0xf;

		/* Get the lmac index from the reg property */
		if ((reg = fdt_getprop(fdt, node, "reg", &len)))
			fdt_lmac = *reg;

		/* Check for a match */
		if (search_node == fdt_node && search_bgx == fdt_bgx &&
		    search_lmac == fdt_lmac)
			return node;
	}

	printf("WARNING: Failed to get lmac fdt node for %d%d%d\n", search_node,
	       search_bgx, search_lmac);
	return -1;
}

/*
 * get_mix_fdt_node:	Search the device tree for the node corresponding to
 *			a given mix.
 *
 *  fdt:		Pointer to flat device tree
 *  search_node:	Mix numa node to search for.
 *  search_index:	Mix index to search for.
 *
 *  returns:		The device tree node of the lmac if found,
 *			or -1 otherwise.
 */
static int get_mix_fdt_node(const void *fdt, int search_node, int search_index)
{
	int		node;

	/* Iterate through all the mix fdt nodes */
	node = -1;
	while ((node = fdt_node_offset_by_compatible((void *)fdt, node,
					"cavium,octeon-7890-mix")) >= 0) {
		int		parent;
		int		len;
		const char	*name;
		int		mix_numa_node;
		const fdt32_t	*reg;
		int		mix_index = -1;
		u64		addr;

		/* Get the numa node of the mix from the parent node name */
		if (((parent = fdt_parent_offset(fdt, node)) < 0) ||
		    ((name = fdt_get_name(fdt, parent, &len)) == NULL) ||
		    ((name = strchr(name, '@')) == NULL))
			continue;

		name++;
		mix_numa_node = simple_strtol(name, NULL, 0) ? 1 : 0;

		/* Get the mix index from the reg property */
		if ((reg = fdt_getprop(fdt, node, "reg", &len))) {
			addr = fdt_translate_address((void *)fdt, parent, reg);
			mix_index = (addr >> 11) & 1;
		}

		/* Check for a match */
		if (mix_numa_node == search_node && mix_index == search_index)
			return node;
	}

	return -1;
}

/*
 * fdt_fix_mix:		Fix the mix nodes in the device tree. Only the mix nodes
 *			configured by the user will be preserved. All other mix
 *			nodes will be trimmed.
 *
 *  fdt:		Pointer to flat device tree
 *
 *  returns:		Zero on success, error otherwise.
 */
static int fdt_fix_mix(const void *fdt)
{
	int	node;
	int	next_node;
	int	len;
	int	i;

	/* Parse all the mix port environment variables */
	for (i = 0; i < MAX_MIX_ENV_VARS; i++) {
		int	env_node = 0;
		int	env_bgx = -1;
		int	env_lmac = -1;
		int	lmac_fdt_node = -1;
		int	mix_fdt_node = -1;
		int	lmac_phandle;

		/* Get the lmac for this environment variable */
		if (parse_env_var(i, &env_bgx, &env_lmac))
			continue;

		/* Get the fdt node for this lmac and add a phandle to it */
		if ((lmac_fdt_node = get_lmac_fdt_node(fdt, env_node, env_bgx,
						       env_lmac)) < 0)
			continue;
		lmac_phandle = fdt_alloc_phandle((void *)fdt);
		fdt_set_phandle((void *)fdt, lmac_fdt_node, lmac_phandle);

		/* Get the fdt mix node corresponding to this lmac */
		if ((mix_fdt_node = get_mix_fdt_node(fdt, env_node,
						     env_lmac)) < 0)
			continue;

		/* Point the mix to the lmac */
		fdt_getprop(fdt, mix_fdt_node, "cavium,mac-handle", &len);
		fdt_setprop_inplace((void *)fdt, mix_fdt_node,
				    "cavium,mac-handle", &lmac_phandle, len);
	}

	/* Trim unused mix'es from the device tree */
	for (node = fdt_next_node(fdt, -1, NULL); node >= 0;
	     node = next_node) {
		const char	*compat;
		const fdt32_t	*reg;

		next_node = fdt_next_node(fdt, node, NULL);

		if ((compat = fdt_getprop(fdt, node, "compatible", &len))) {
			if (strcmp(compat, "cavium,octeon-7890-mix"))
				continue;

			if ((reg = fdt_getprop(fdt, node,
					       "cavium,mac-handle", &len))) {
				if (*reg == 0xffff)
					fdt_nop_node((void *)fdt, node);
			}
		}
	}

	return 0;
}

int no_phy[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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

/* QLM0 - QLM6 */
void __fixup_fdt(void)
{
	int qlm;

	for (qlm = 0; qlm < 9; qlm++) {
		enum cvmx_qlm_mode mode;
		char fdt_key[16];
		const char *type_str = "none";
		mode = cvmx_qlm_get_mode(qlm);
		switch (mode) {
		case CVMX_QLM_MODE_SGMII_2X1:
		case CVMX_QLM_MODE_SGMII:
			type_str = "sgmii";
			break;
		case CVMX_QLM_MODE_XFI_1X2:
		case CVMX_QLM_MODE_XFI:
			type_str = "xfi";
			break;
		case CVMX_QLM_MODE_10G_KR_1X2:
		case CVMX_QLM_MODE_10G_KR:
			type_str = "10G_KR";
			break;
		case CVMX_QLM_MODE_SRIO_1X4:
		case CVMX_QLM_MODE_SRIO_2X2:
		case CVMX_QLM_MODE_SRIO_4X1:
			type_str = "srio";
			break;
		case CVMX_QLM_MODE_MIXED: // special for DLM5 & DLM6
		{
			cvmx_bgxx_cmrx_config_t cmr_config;
			cvmx_bgxx_spux_br_pmd_control_t pmd_control;
			int mux = cvmx_qlm_mux_interface(0);
			if (mux == 2) {   // only dlm5
				cmr_config.u64 = cvmx_read_csr(
						CVMX_BGXX_CMRX_CONFIG(2, 0));
				pmd_control.u64 = cvmx_read_csr(
						CVMX_BGXX_SPUX_BR_PMD_CONTROL(2, 0));
			} else {
				if (qlm == 4) {
					cmr_config.u64 = cvmx_read_csr(
							CVMX_BGXX_CMRX_CONFIG(0, 0));
					pmd_control.u64 = cvmx_read_csr(
							CVMX_BGXX_SPUX_BR_PMD_CONTROL(0, 0));
				} else {
					cmr_config.u64 = cvmx_read_csr(
							CVMX_BGXX_CMRX_CONFIG(2, 0));
					pmd_control.u64 = cvmx_read_csr(
							CVMX_BGXX_SPUX_BR_PMD_CONTROL(2, 0));
				}
			}
			switch (cmr_config.s.lmac_type) {
			case 0:
				type_str = "sgmii";
				break;
			case 1:
				type_str = "xaui";
				break;
			case 2:
				type_str = "rxaui";
				break;
			case 3:
				if (pmd_control.s.train_en)
					type_str = "10G_KR";
				else
					type_str = "xfi";
				break;
			case 4:
				if (pmd_control.s.train_en)
					type_str = "40G_KR4";
				else
					type_str = "xlaui";
				break;
			default:
				type_str = "none";
				break;
			}
			break;
		}
		case CVMX_QLM_MODE_CPRI:
			type_str = "cpri";
			break;
		case CVMX_QLM_MODE_SDL:
			type_str = "sdl";
			break;
		default:
			type_str = "none";
			break;
		}
		sprintf(fdt_key, "%d,%s", qlm, type_str);
		debug("Patching qlm %d for %s for mode %d%s\n",
		      qlm, fdt_key, mode, no_phy[qlm] ? ", removing PHY" : "");
		octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL,
					true, no_phy[qlm] ? kill_fdt_phy : NULL, NULL);
	}
}

/* ebb7504.rev1 phy-interrupt lines are non-functional */
void drop_broken_phy_interrupts(void)
{
	int first = 1; /* leave the first entry, SMI_0 works */
	const char *targets[] = {
		"ethernet-phy-ieee802.3-c22",
		"ethernet-phy-ieee802.3-c45",
		NULL,
	};
	const char **t;

	if (gd->arch.board_desc.rev_major > 1)
		return;

	for (t = targets; *t; t++) {
		int offset = -1;
		while ((offset = fdt_node_offset_by_compatible(
				gd->fdt_blob, offset, *t)) >= 0)
		{
			if (!first)
				fdt_nop_property((void *)gd->fdt_blob, offset,
						 "interrupts");
			first = 0;
		}
	}
}

int board_fixup_fdt(void)
{
	__fixup_fdt();

	/* Fix the mix ports */
	fdt_fix_mix(gd->fdt_blob);

	drop_broken_phy_interrupts();

	return 0;
}

/* Raise an integer to a power */
static uint64_t ipow(uint64_t base, uint64_t exp)
{
	uint64_t result = 1;
	while (exp) {
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}
	return result;
}

/*
 * This function now supports only single (node0) configuration.
 * Some of the called funcs (read_ispPAC_mvolts_avg(), octeon_led_str_write())
 * are hard-coded for node0. Also we set gd->arch.mcu_rev_maj, ...
 * It can be extended to support multinode if needed/important
 */
static int checkboardinfo(void)
{
        int core_mVolts, dram_mVolts0;
	char mcu_ip_msg[64] = { 0 };
	char tmp[10];
	int characters, idx = 0, value = 0;

	debug("In %s\n", __func__);
	if (octeon_show_info()) {

		int mcu_rev_maj = 0;
		int mcu_rev_min = 0;

		if (twsii_mcu_read(0x00) == 0xa5
		    && twsii_mcu_read(0x01) == 0x5a) {
			gd->arch.mcu_rev_maj = mcu_rev_maj = twsii_mcu_read(2);
			gd->arch.mcu_rev_min = mcu_rev_min = twsii_mcu_read(3);
		} else {
                    return -1;     /* Abort if we can't access the MCU */
                }

		core_mVolts  = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 8);
		dram_mVolts0 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 7); /* DDR0 */

		if (twsii_mcu_read(0x14) == 1)
			sprintf(mcu_ip_msg, "MCU IPaddr: %d.%d.%d.%d, ",
				twsii_mcu_read(0x10),
				twsii_mcu_read(0x11),
				twsii_mcu_read(0x12),
				twsii_mcu_read(0x13));
		printf("MCU rev: %d.%02d, %sCPU voltage: %d.%03d DDR voltage: %d.%03d\n",
		       gd->arch.mcu_rev_maj, gd->arch.mcu_rev_min, mcu_ip_msg,
		       core_mVolts  / 1000, core_mVolts  % 1000,
		       dram_mVolts0 / 1000, dram_mVolts0 % 1000);

#define LED_CHARACTERS 8
		value = core_mVolts;
		idx = sprintf(tmp, "%lu ", gd->cpu_clk/(1000*1000));
		characters = LED_CHARACTERS - idx;

		if (value / 1000) {
			idx += sprintf(tmp + idx, "%d", value / 1000);
			characters = LED_CHARACTERS - idx;
		}

		characters -= 1;	/* Account for decimal point */

		value %= 1000;
		value = DIV_ROUND_UP(value, ipow(10, max(3 - characters, 0)));

		idx += sprintf(tmp + idx, ".%0*d", min(3, characters), value);

		/* Display CPU speed and voltage on display */
		octeon_led_str_write(tmp);
	} else {
		octeon_led_str_write("Boot    ");
	}

	return 0;
}

/**
 * Here is the description of the parameters that are passed to QLM configuration
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
void board_configure_qlms(void)
{
	int qlm;
	char env_var[16];
	int speed[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int mode[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	int pcie_rc[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int pcie_gen[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int ref_clock_sel[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int ref_clock_input[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	octeon_init_qlm(0);

#if defined(CONFIG_OCTEON_MCU_PROBE) && defined(BOARD_MCU_TWSI_ADDR)
	if (octeon_mcu_probe(0) != -1) {
		return 0;
	}
#endif
	for (qlm = 0; qlm < 9; qlm++) {
		const char *mode_str;
		//char spd_env[16];

		mode[qlm] = CVMX_QLM_MODE_DISABLED;
		sprintf(env_var, "qlm%d_mode", qlm);
		mode_str = getenv(env_var);
		if (!mode_str)
			continue;

		if (strstr(mode_str, ",no_phy"))
			no_phy[qlm] = 1;

		if (!strncmp(mode_str, "sgmii", 5)) {
			if ((qlm == 4) || (qlm == 5)) {
				speed[qlm] = 1250;
				mode[qlm] = CVMX_QLM_MODE_SGMII_2X1;
				ref_clock_sel[qlm] = 2;
				ref_clock_input[qlm] = 2; // use QLMC_REF_CLK1
				if (no_phy[qlm]) {
					int i;
					int start = 0, stop = 2;
					if (qlm == 5) {
						start = 2;
						stop = 4;
					}
					for (i = start; i < stop; i++) {
						printf("Ignoring PHY for interface0: port: %d\n", i);
						cvmx_helper_set_port_force_link_up(0, i, true);
					}
				}
				printf("QLM %d: SGMII\n", qlm);
			} else 
				mode[qlm] = CVMX_QLM_MODE_DISABLED;
		} else if (!strncmp(mode_str, "xfi", 3)) {
			if (qlm == 5) {
				speed[qlm] = 103125;
				mode[qlm] = CVMX_QLM_MODE_XFI_1X2;
				ref_clock_sel[qlm] = 2;
				ref_clock_input[qlm] = 2; // use QLMC_REF_CLK1
				printf("QLM %d: XFI\n", qlm);
			} else
				mode[qlm] = CVMX_QLM_MODE_DISABLED;
		} else if (!strncmp(mode_str, "10G_KR", 6)) {
			if (qlm == 5) {
				speed[qlm] = 103125;
				mode[qlm] = CVMX_QLM_MODE_10G_KR_1X2;
				ref_clock_sel[qlm] = 2;
				ref_clock_input[qlm] = 2; // use QLMC_REF_CLK1
				printf("QLM %d: 10G_KR\n", qlm);
			} else
				mode[qlm] = CVMX_QLM_MODE_DISABLED;
		} else if (!strcmp(mode_str, "pcie")) {
			char *pmode;
			int lanes = 0;
			sprintf(env_var, "pcie%d_mode", qlm);
			pmode = getenv(env_var);
			if (pmode && !strcmp(pmode, "ep"))
				pcie_rc[qlm] = 0;
			else
				pcie_rc[qlm] = 1;
			sprintf(env_var, "pcie%d_gen", qlm);
			pcie_gen[qlm] = getenv_ulong(env_var, 0, 3);
			sprintf(env_var, "pcie%d_lanes", qlm);
			lanes = getenv_ulong(env_var, 0, 8);
			if (lanes == 4)
				mode[qlm] = CVMX_QLM_MODE_PCIE;
			else if (lanes == 2)
				mode[qlm] = CVMX_QLM_MODE_PCIE_1X2;
			else
				mode[qlm] = CVMX_QLM_MODE_DISABLED;
			ref_clock_sel[qlm] = 0;
			printf("QLM %d: PCIe gen%d %s, x%d lanes\n", qlm, pcie_gen[qlm] + 1,
				pcie_rc[qlm] ? "root complex" : "endpoint", lanes);
		} else if (!strcmp(mode_str, "srio")) {
			char *pmode;
			sprintf(env_var, "srio%d_mode", qlm);
			pmode = getenv(env_var);
			if (pmode && !strcmp(pmode, "ep"))
				pcie_rc[qlm] = 0;
			else
				pcie_rc[qlm] = 1;
			sprintf(env_var, "srio%d_speed", qlm);
			speed[qlm] = getenv_ulong(env_var, 0, 8);
			mode[qlm] = CVMX_QLM_MODE_SRIO_1X4;
			ref_clock_sel[qlm] = 2;
			printf("QLM %d: SRIO is in %s mode\n", qlm, 
				pcie_rc[qlm] ? "host" : "endpoint");
		} else if (!strcmp(mode_str, "cpri")) {
			sprintf(env_var, "cpri%d_speed", qlm);
			speed[qlm] = getenv_ulong(env_var, 0, 8);
			mode[qlm] = CVMX_QLM_MODE_CPRI;
			ref_clock_sel[qlm] = 3;
			printf("QLM %d: CPRI\n", qlm);
		} else if (!strcmp(mode_str, "sdl")) {
			speed[qlm] = 49152;
			mode[qlm] = CVMX_QLM_MODE_SDL;
			ref_clock_sel[qlm] = 3;
			printf("QLM %d: SDL\n", qlm);
		} else {
			printf("QLM %d: disabled\n", qlm);
		}
	}

	for (qlm = 0; qlm < 9; qlm++) {
		if (mode[qlm] == -1)
			continue;
		debug("Configuring qlm%d with speed(%d), mode(%d), RC(%d), Gen(%d), REF_CLK(%d), CLK_SOURCE(%d)\n",
		      qlm, speed[qlm], mode[qlm], pcie_rc[qlm],
		      pcie_gen[qlm] + 1, ref_clock_sel[qlm], ref_clock_input[qlm]);
		octeon_configure_qlm(qlm, speed[qlm], mode[qlm],
					  pcie_rc[qlm], pcie_gen[qlm],
					  ref_clock_sel[qlm],
					  ref_clock_input[qlm]);

		if (speed[qlm] == 103125)
			octeon_qlm_tune_v3(0, qlm, speed[qlm], 0xd, 0xd0, -1, -1);
	}
}

int checkboard(void)
{
	checkboardinfo();

	return 0;
}

int early_board_init(void)
{
	octeon_board_get_clock_info(EBB7500_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBB7500, 1, 0);

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	/* Configure GPIO inputs */
	gpio_direction_input(0);	/* Failsafe */
	gpio_direction_input(1);	/* Display more info */
	gpio_direction_input(4);
	gpio_direction_input(5);

	return 0;
}
