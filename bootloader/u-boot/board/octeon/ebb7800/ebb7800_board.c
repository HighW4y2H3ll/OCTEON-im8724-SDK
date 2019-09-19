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
#include <asm/arch/octeon_fdt.h>
#include <fdt_support.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/octeon_qlm.h>
#include <asm/arch/cvmx-helper-util.h>

#define MAX_MIX_ENV_VARS	4

extern void print_isp_volt(const char **labels, uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts(uint16_t isp_dev_addr, uint8_t adc_chan);

extern int read_ispPAC_mvolts_avg(int loops, uint16_t isp_dev_addr, uint8_t adc_chan);

DECLARE_GLOBAL_DATA_PTR;

int no_phy[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

/*
 * parse_env_var:	Parse the environment variable ("bgx_for_mix%d") to
 *			extract the lmac it is set to.
 *
 *  index:		Index of environment variable to parse.
 *  env_numa_node:	Updated with the numa node of the lmac in the
 *			environment variable.
 *  env_bgx:		Updated with the bgx of the lmac in the environment
 *			variable.
 *  env_lmac:		Updated with the index of lmac in the environment
 *			variable.
 *
 *  returns:		Zero on success, error otherwise.
 */
static int parse_env_var(int index, int *env_numa_node, int *env_bgx,
			 int *env_lmac)
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
		*env_numa_node = xi.node;
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
		int	env_node = -1;
		int	env_bgx = -1;
		int	env_lmac = -1;
		int	lmac_fdt_node = -1;
		int	mix_fdt_node = -1;
		int	lmac_phandle;

		/* Get the lmac for this environment variable */
		if (parse_env_var(i, &env_node, &env_bgx, &env_lmac))
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

/* QLM0 - QLM7 are node#0 and QLM10 - QLM17 are node#1 */
void __fixup_fdt(int node)
{
	int i;

	for (i = 0; i < 8; i++) {
		enum cvmx_qlm_mode mode;
		char fdt_key[16];
		int qlm = (node ? i+(10*node) : i);
		mode = cvmx_qlm_get_mode_cn78xx(node, i);
		if (mode == -1) {
			sprintf(fdt_key, "%d,none", qlm);
		} else if (mode == CVMX_QLM_MODE_SGMII) {
			sprintf(fdt_key, "%d,sgmii", qlm);
		} else if (mode == CVMX_QLM_MODE_XAUI) {
			int speed = cvmx_qlm_get_gbaud_mhz(qlm) * 8 / 10;
			if (speed == 10000)
				sprintf(fdt_key, "%d,xaui", qlm);
			else
				sprintf(fdt_key, "%d,dxaui", qlm);
		} else if (mode == CVMX_QLM_MODE_RXAUI) {
			sprintf(fdt_key, "%d,rxaui", qlm);
		} else if (mode == CVMX_QLM_MODE_XLAUI) {
			if (qlm < 4)
				printf("Error: XLAUI not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,xlaui", qlm);
		} else if (mode == CVMX_QLM_MODE_XFI) {
			if (qlm < 4)
				printf("Error: XFI not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,xfi", qlm);
		} else if (mode == CVMX_QLM_MODE_10G_KR) {
			if (qlm < 4)
				printf("Error: 10G_KR not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,10G_KR", qlm);
		} else if (mode == CVMX_QLM_MODE_40G_KR4) {
			if (qlm < 4)
				printf("Error: 40G_KR4 not supported on QLM %d\n",
				       qlm);
			else
				sprintf(fdt_key, "%d,40G_KR4", qlm);
		} else
			sprintf(fdt_key, "%d,none", qlm);
		debug("Patching qlm %d for %s\n", qlm, fdt_key);
		octeon_fdt_patch_rename((void *)gd->fdt_blob, fdt_key, NULL,
					true, no_phy[qlm] ? kill_fdt_phy: NULL,
					NULL);
	}
}

int board_fixup_fdt(void)
{
	char fdt_key[16];
	int node;

	/* First patch the PHY configuration based on board revision */
	octeon_fdt_patch_rename((void *)(gd->fdt_blob),
				gd->arch.board_desc.rev_major == 1
				? "0,rev1" : "0,notrev1",
				"cavium,board-trim", false, NULL, NULL);

	/* Next patch CPU revision chages */
	for (node = 0; node < CVMX_MAX_NODES; node++) {
		if (!(gd->arch.node_mask & (1 << node)))
			continue;
		if (OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_0))
			sprintf(fdt_key, "%d,rev1.0", node);
		else
			sprintf(fdt_key, "%d,notrev1.0", node);
		octeon_fdt_patch((void *)(gd->fdt_blob), fdt_key, "cavium,cpu-trim");
	}

	__fixup_fdt(0);

	/* Test for node 1 */
	if (!(gd->arch.node_mask & (1 << 1))) {
		octeon_fdt_patch((void *)(gd->fdt_blob), "1,none",
				 "cavium,node-trim");
		goto final_cleanup;
	}

	/* This won't do anything if node 1 is missing */
	__fixup_fdt(1);

final_cleanup:
	node = fdt_node_offset_by_compatible((void *)gd->fdt_blob, -1, "cavium,octeon-7890-bgx");
	while (node != -FDT_ERR_NOTFOUND) {
		int depth = 0;
		fdt_next_node((void *)gd->fdt_blob, node, &depth);
		if (depth != 1)
			fdt_nop_node((void *)gd->fdt_blob, node);
		node = fdt_node_offset_by_compatible((void *)gd->fdt_blob, node, "cavium,octeon-7890-bgx");
	}

	/* Fix the mix ports */
	fdt_fix_mix(gd->fdt_blob);

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
        int core_mVolts, dram_mVolts0, dram_mVolts1;
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
		dram_mVolts1 = read_ispPAC_mvolts_avg(10, BOARD_ISP_TWSI_ADDR, 6); /* DDR1 */

		if (twsii_mcu_read(0x14) == 1)
			sprintf(mcu_ip_msg, "MCU IPaddr: %d.%d.%d.%d, ",
				twsii_mcu_read(0x10),
				twsii_mcu_read(0x11),
				twsii_mcu_read(0x12), twsii_mcu_read(0x13));
		printf("MCU rev: %d.%02d, %sCPU voltage: %d.%03d DDR{0,1} voltages: %d.%03d,%d.%03d\n",
		       gd->arch.mcu_rev_maj, gd->arch.mcu_rev_min, mcu_ip_msg,
		       core_mVolts  / 1000, core_mVolts  % 1000,
		       dram_mVolts0 / 1000, dram_mVolts0 % 1000,
		       dram_mVolts1 / 1000, dram_mVolts1 % 1000);

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

static void intset(int *buffer, int value, size_t size)
{
	size_t i;
	for (i = 0; i < size; i++)
		buffer[i] = value;
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
void board_configure_qlms(void)
{
	int qlm;
	char env_var[16];
	int node = 0;

	/* Since i2c is broken on pass 1.0 we use an environment variable */
	if (OCTEON_IS_MODEL(OCTEON_CN78XX_PASS1_0)) {
		ulong board_version;
		board_version = getenv_ulong("board_version", 10, 1);

		gd->arch.board_desc.rev_major = board_version;
	}

	octeon_init_qlm(0);

	for (node = 0; node < CVMX_MAX_NODES; node++) {
		int speed[8];
		int mode[8];
		int pcie_rc[8];
		int pcie_gen[8];
		int ref_clock_sel[8];
		int ref_clock_input[8];

		if (!(gd->arch.node_mask & (1 << node)))
			continue;

		intset(speed, 0, 8);
		intset(mode, -1, 8);
		intset(pcie_rc, 0, 8);
		intset(pcie_gen, 0, 8);
		intset(ref_clock_sel, 0, 8);
		intset(ref_clock_input, 0, 8);

#if defined(CONFIG_OCTEON_MCU_PROBE) && defined(BOARD_MCU_TWSI_ADDR)
		if (octeon_mcu_probe(node) != -1) {
			if (((gd->arch.node_mask) >> 1) != node)
				continue;
			return;
		}
#endif
		for (qlm = 0; qlm < 8; qlm++) {
			const char *mode_str;
			char spd_env[16];

			mode[qlm] = CVMX_QLM_MODE_DISABLED;
			sprintf(env_var, "qlm%d:%d_mode", qlm, node);
			mode_str = getenv(env_var);
			if (!mode_str)
				continue;

			if (strstr(mode_str, ",no_phy"))
				no_phy[(node ? qlm+(10*node) : qlm)] = 1;

			if (!strncmp(mode_str, "sgmii", 5) ||
			    !strncmp(mode_str, "xaui", 4)  ||
			    !strncmp(mode_str, "dxaui", 5) ||
			    !strncmp(mode_str, "rxaui", 5)) {
				/* BGX0 is QLM0 or QLM2 */
				if (qlm == 2
				    && mode[0] != -1
				    && mode[0] != CVMX_QLM_MODE_PCIE
				    && mode[0] != CVMX_QLM_MODE_PCIE_1X8) {
					printf("NODE %d: Not configuring QLM2,  as QLM0 is already set for BGX0\n", node);
					continue;
				}
				/* BGX1 is QLM1 or QLM3 */
				if (qlm == 3
				    && mode[1] != -1
				    && mode[1] != CVMX_QLM_MODE_PCIE
				    && mode[1] != CVMX_QLM_MODE_PCIE_1X8) {
					printf("NODE %d: Not configuring QLM3,  as QLM1 is already set for BGX1\n", node);
					continue;
				}
			}
			if (!strncmp(mode_str, "sgmii", 5)) {
				speed[qlm] = 1250;
				mode[qlm] = CVMX_QLM_MODE_SGMII;
				ref_clock_sel[qlm] = 2;
				printf("NODE %d:QLM %d: SGMII\n", node, qlm);
			} else if (!strncmp(mode_str, "xaui", 4)) {
				speed[qlm] = 3125;
				mode[qlm] = CVMX_QLM_MODE_XAUI;
				ref_clock_sel[qlm] = 2;
				printf("NODE %d:QLM %d: XAUI\n", node, qlm);
			} else if (!strncmp(mode_str, "dxaui", 5)) {
				speed[qlm] = 6250;
				mode[qlm] = CVMX_QLM_MODE_XAUI;
				ref_clock_sel[qlm] = 2;
				printf("NODE %d:QLM %d: DXAUI\n", node, qlm);
			} else if (!strncmp(mode_str, "rxaui", 5)) {
				speed[qlm] = 6250;
				mode[qlm] = CVMX_QLM_MODE_RXAUI;
				ref_clock_sel[qlm] = 2;
				printf("NODE %d:QLM %d: RXAUI\n", node, qlm);
			} else if (!strcmp(mode_str, "ila")) {
				int spd;
				int lanes = 0;
				char ila_env[16];
				if (qlm != 2 && qlm != 3) {
					printf("Error: ILA not supported on NODE%d:QLM %d\n",
						node, qlm);
				} else {
					mode[qlm] = CVMX_QLM_MODE_ILK;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: ILA\n", node, qlm);
				}
				sprintf(ila_env, "ila%d:%d_lanes", qlm, node);
				if (getenv(ila_env))
					lanes = getenv_ulong(ila_env, 0, 16);
				if (lanes > 4)
					ref_clock_input[qlm] = 2;
				sprintf(ila_env, "ila%d:%d_speed", qlm, node);
				if (getenv(ila_env)) {
					spd = getenv_ulong(ila_env, 0, 8);
					if (spd == 103125)
						speed[qlm] = spd;
					else
						speed[qlm] = 6250;
				} else
					speed[qlm] = 6250;
			} else if (!strcmp(mode_str, "ilk")) {
				int lanes = 0;
				int spd;
				char ilk_env[16];

				if (qlm < 4) {
					printf("Error: ILK not supported on NODE%d:QLM %d\n",
				       node, qlm);
				} else {
					mode[qlm] = CVMX_QLM_MODE_ILK;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: ILK\n", node, qlm);
				}
				sprintf(ilk_env, "ilk%d:%d_lanes", qlm, node);
				if (getenv(ilk_env))
					lanes = getenv_ulong(ilk_env, 0, 16);
				if (lanes > 4)
					ref_clock_input[qlm] = 2;
				sprintf(ilk_env, "ilk%d:%d_speed", qlm, node);
				if (getenv(ilk_env)) {
					spd = getenv_ulong(ilk_env, 0, 8);
					if (spd == 103125)
						speed[qlm] = spd;
					else
						speed[qlm] = 6250;
				} else
					speed[qlm] = 6250;
			} else if (!strncmp(mode_str, "xlaui", 5)) {
				if (qlm < 4) {
					printf("Error: XLAUI not supported on NODE%d:QLM %d\n",
				       	node, qlm);
				} else {
					mode[qlm] = CVMX_QLM_MODE_XLAUI;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: XLAUI\n", node, qlm);
				}
				sprintf(spd_env, "qlm%d:%d_speed", qlm, node);
				if (getenv(spd_env)) {
					int spd = getenv_ulong(spd_env, 0, 8);
					if (spd)
						speed[qlm] = spd;
					else
						speed[qlm] = 103125;
				} else
					speed[qlm] = 103125;
			} else if (!strncmp(mode_str, "xfi", 3)) {
				if (qlm < 4) {
					printf("Error: XFI not supported on NODE%d:QLM %d\n",
				       node, qlm);
				} else {
					speed[qlm] = 103125;
					mode[qlm] = CVMX_QLM_MODE_XFI;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: XFI\n", node, qlm);
				}
			} else if (!strncmp(mode_str, "10G_KR", 6)) {
				if (qlm < 4) {
					printf("Error: 10G_KR not supported on NODE%d:QLM %d\n",
				       node, qlm);
				} else {
					speed[qlm] = 103125;
					mode[qlm] = CVMX_QLM_MODE_10G_KR;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: 10G_KR\n", node, qlm);
				}
			} else if (!strncmp(mode_str, "40G_KR4", 7)) {
				if (qlm < 4) {
					printf("Error: 40G_KR4 not supported on NODE%d:QLM %d\n",
				       node, qlm);
				} else {
					speed[qlm] = 103125;
					mode[qlm] = CVMX_QLM_MODE_40G_KR4;
					ref_clock_sel[qlm] = 2;
					printf("NODE %d:QLM %d: 40G_KR4\n", node, qlm);
				}
			} else if (!strcmp(mode_str, "pcie")) {
				char *pmode;
				int lanes = 0;

				sprintf(env_var, "pcie%d:%d_mode", qlm, node);
				pmode = getenv(env_var);
				if (pmode && !strcmp(pmode, "ep"))
					pcie_rc[qlm] = 0;
				else
					pcie_rc[qlm] = 1;
				sprintf(env_var, "pcie%d:%d_gen", qlm, node);
				pcie_gen[qlm] = getenv_ulong(env_var, 0, 3);
				sprintf(env_var, "pcie%d:%d_lanes", qlm, node);
				lanes = getenv_ulong(env_var, 0, 8);
				if (lanes == 8)
					mode[qlm] = CVMX_QLM_MODE_PCIE_1X8;
				else
					mode[qlm] = CVMX_QLM_MODE_PCIE;
				ref_clock_sel[qlm] = 0;
				printf("NODE %d:QLM %d: PCIe gen%d %s\n", node, qlm, pcie_gen[qlm] + 1,
					pcie_rc[qlm] ? "root complex" : "endpoint");
			} else {
				printf("NODE %d:QLM %d: disabled\n", node, qlm);
			}
		}

		for (qlm = 0; qlm < 8; qlm++) {
			if (mode[qlm] == -1)
				continue;
			debug("Configuring node%d qlm%d with speed(%d), mode(%d), RC(%d), Gen(%d), REF_CLK(%d), CLK_SOURCE(%d)\n",
			 	node, qlm, speed[qlm], mode[qlm], pcie_rc[qlm],
			 	pcie_gen[qlm] + 1, ref_clock_sel[qlm], ref_clock_input[qlm]);
			octeon_configure_qlm_cn78xx(node, qlm, speed[qlm], mode[qlm], pcie_rc[qlm],
					    pcie_gen[qlm], ref_clock_sel[qlm],
					    ref_clock_input[qlm]);

			if (speed[qlm] == 6250)
				octeon_qlm_tune_v3(node, qlm, speed[qlm], 0xa, 0xa0, -1, -1);
			else if (speed[qlm] == 103125)
				octeon_qlm_tune_v3(node, qlm, speed[qlm], 0xd, 0xd0, -1, -1);

			/* Call the following function to disable DFE in the QLM
			   receiver. Rx equalization will still be completed on
			   the specified QLM. This should only be called for
			   low-loss channels. Set lane = -1 to disable DFE on all
			   lanes */
			/*octeon_qlm_dfe_disable(node, qlm, -1, speed[qlm], mode[qlm]);*/

			/* Call the following function to disable DFE in the QLM
			   receiver. This should only be called for low-loss
			   channels. The ctle_zero, agc_pre_ctle, and agc_post_ctle
			   should be selected based on board simulations. Set
			   lane = -1 to set the same ctle_zero, agc_pre_ctle and
			   agc_post_ctle values for all lanes. If calling this
			   function it must be called for all lanes in the QLM
			   because rx equalization will not happen on the other lanes */
			/*octeon_qlm_dfe_disable_ctle_agc(node, qlm, -1, speed[qlm],
				mode[qlm], ctle_zero, agc_pre_ctle, agc_post_ctle); */

			/* Example to set different values for each lane */
			/* Lane 0 */
			//octeon_qlm_dfe_disable_ctle_agc(node, qlm, 0, speed[qlm], mode[qlm], 1, 0x1, 0xf);
			/* Lane 1 */
			//octeon_qlm_dfe_disable_ctle_agc(node, qlm, 1, speed[qlm], mode[qlm], 2, 0x2, 0xa);
			/* Lane 2 */
	        	//octeon_qlm_dfe_disable_ctle_agc(node, qlm, 2, speed[qlm], mode[qlm], 1, 0, 0xf);
			/* Lane 3 */
			//octeon_qlm_dfe_disable_ctle_agc(node, qlm, 3, speed[qlm], mode[qlm], 2, 0x2, 0xf);
		}
	}
}

int checkboard(void)
{
	checkboardinfo();

	return 0;
}

int early_board_init(void)
{
	int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

#ifdef CONFIG_OCTEON_EBB7804
	octeon_board_get_clock_info(EBB7804_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBB7804, 1, 0);
#else
	octeon_board_get_clock_info(EBB7800_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_EBB7800, 1, 0);
#endif
	/* CN78XX has a fixed 50 MHz reference clock */
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
