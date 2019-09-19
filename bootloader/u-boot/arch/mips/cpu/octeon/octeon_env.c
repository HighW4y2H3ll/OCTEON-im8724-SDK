/***********************license start************************************
 * Copyright (c) 2011 - 2013  Cavium, Inc. (support@cavium.com).
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 ***********************license end**************************************/
#include <common.h>
#include <environment.h>
#include <stdio_dev.h>
#include <linux/compiler.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/cvmx-sysinfo.h>
#include <asm/arch/cvmx-bootloader.h>
#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
#include <asm/arch/octeon-boot-info.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/**
 * Checks to see if a valid environment was found at the specified address
 *
 * @param[in]	env	Pointer to environment location to check
 * @param	size	Size of environment (including CRC) to check
 *
 * @return	0 if environment is valid, -1 if invalid.
 */
int octeon_check_env(const env_t *env, size_t size)
{
	uint32_t crc;

	crc = crc32(0, env->data, size - U_BOOT_RAM_ENV_CRC_SIZE);
	return (crc == env->crc) ? 0 : -1;
}

int octeon_env_init(void)
{
#if defined(CONFIG_ENV_IS_IN_FLASH)
        debug("%s: bootflags: 0x%lx\n", __func__, gd->bd->bi_bootflags);
        if ((gd->bd->bi_bootflags &
	     (OCTEON_BD_FLAG_BOOT_CACHE | OCTEON_BD_FLAG_BOOT_FLASH))
            == OCTEON_BD_FLAG_BOOT_CACHE) {
                /* Only check the cache if we're not booting from flash */
                env_t *tmp_env;
                u32 size = U_BOOT_RAM_ENV_SIZE - offsetof(env_t, data);
                u32 crc;
                if (gd->flags & GD_FLG_FAILSAFE_MODE) {
                        gd->env_addr = (ulong)&default_environment[0];
                        gd->env_valid = 0;
                        return 0;
                }

                debug("Checking env in cache\n");
                tmp_env = cvmx_phys_to_ptr(U_BOOT_RAM_ENV_ADDR);
                debug("env address: 0x%p, crc: 0x%x\n", tmp_env, tmp_env->crc);
                crc = crc32(0, tmp_env->data, size);
                if (crc != tmp_env->crc) {
                        debug("cached environment invalid, calculated crc 0x%x != 0x%x \n",
                              crc, tmp_env->crc);
                                gd->env_addr = (ulong)&default_environment[0];
                        gd->env_valid = 0;
                        return env_init();
                } else {
                        debug("Found valid environment in cache\n");
                        gd->env_addr = (uint32_t)tmp_env->data;
                        gd->env_valid = 1;
                        return 0;
                }
        } else {
                debug("%s: Not booting out of cache\n", __func__);
        }
#endif
        return env_init();
}

static int on_console_uart(const char *name, const char *value,
			   enum env_op op, int flags)
{
	int console_uart;

	debug("%s(%s, %s, %d, 0x%x)\n", __func__, name, value, op, flags);
	switch (op) {
	case env_op_create:
	case env_op_overwrite:
		console_uart = simple_strtoul(value, NULL, 10);
		if (console_uart < CONFIG_OCTEON_MIN_CONSOLE_UART &&
		console_uart > CONFIG_OCTEON_MAX_CONSOLE_UART) {
			printf("Console UART %d out of range.\n", console_uart);
			return 1;
		}
		gd->arch.console_uart = console_uart;
		printf("Console UART changed to %u\n", console_uart);
		break;
	default:
		gd->arch.console_uart = CONFIG_OCTEON_DEFAULT_CONSOLE_UART_PORT;
		break;
	}
	return 0;
}

U_BOOT_ENV_CALLBACK(console_uart, on_console_uart);

static int on_pci_console_active(const char *name, const char *value,
				 enum env_op op, int flags)
{
	char *env_str;
	char output[40];
	char *start, *end;

#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
	linux_app_boot_info_t *labi =
	    (linux_app_boot_info_t *) (0x80000000 + LABI_ADDR_IN_BOOTLOADER);
#endif

	switch (op) {
	case env_op_create:
	case env_op_overwrite:
		env_str = getenv(stdin);
		if (!env_str)
			snprintf(output, sizeof(output), "pci");
		else if (!strstr(env_str, "pci") && (strlen(env_str) < 36))
			snprintf(output, sizeof(output), "%s,pci", env_str);
		else
			snprintf(output, sizeof(output), "%s", env_str);
		debug("Changing stdin from %s to %s\n", env_str, output);
		setenv(stdin, env_str);

		env_str = getenv("stdout");
		if (!env_str)
			snprintf(output, sizeof(output), "pci");
		else if (!strstr(env_str, "pci") && (strlen(env_str) < 36))
			snprintf(output, sizeof(output), "%s,pci", env_str);
		else
			snprintf(output, sizeof(output), "%s", env_str);
		debug("Changing stderr from %s to %s\n", env_str, output);
		setenv("stdout", env_str);

		env_str = getenv("stderr");
		if (!env_str)
			snprintf(output, sizeof(output), "pci");
		else if (!strstr(env_str, "pci") && (strlen(env_str) < 36))
			snprintf(output, sizeof(output), "%s,pci", env_str);
		else
			snprintf(output, sizeof(output), "%s", env_str);
		debug("Changing stderr from %s to %s\n", env_str, output);
		setenv("stderr", env_str);
#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
		labi->pci_console_active = 1;
#endif
		puts("PCI console output has been enabled.\n");
		break;

	case env_op_delete:
		/* Clearing it */
		debug("Disabling pci console\n");
		/* Disable PCI console access */
		env_str = getenv("stdout");
		/* There are three cases:
		 * 1. pci could not be present (unlikely)
		 * 2. pci could be at the beginning, so we remove pci,
		 * 3. pci is not at the beginning so we remove ,pci
		 */
		if (env_str && strstr(env_str, "pci")) {
			sprintf(output, "%s", env_str);
			start = strstr(output, ",pci");
			if (!start)	/* case 3 */
				start = strstr(output, "pci,");
			if (!start)	/* case 1 */
				output[0] = '\0';
			else {		/* case 2 or 3 */
				end = &start[4];
				/* Remove 4 characters from string */
				do {
					*start++ = *end;
				} while (*end++);
			}
			setenv("stdout", output);
		}
		env_str = getenv("stderr");
		if (env_str && strstr(env_str, "pci")) {
			sprintf(output, "%s", env_str);
			start = strstr(output, ",pci");
			if (!start)
				start = strstr(output, "pci,");
			if (!start)
				output[0] = '\0';
			else {
				end = &start[4];
				do {
					*start++ = *end;
				} while (*end++);
			}
			setenv("stderr", output);
		}
#ifdef CONFIG_OCTEON_REMOTE_HOTPLUG
		labi->pci_console_active = 0;
#endif
		printf("PCI console output has been disabled.\n");
		break;
	}
	return 0;
}

U_BOOT_ENV_CALLBACK(pci_console_active, on_pci_console_active);

#ifdef CONFIG_HW_WATCHDOG
extern void hw_watchdog_disable(void);
extern void hw_watchdog_start(int msecs);

static int on_watchdog_enable(const char *name, const char *value,
			      enum env_op op, int flags)
{
	int msecs;
	switch (op) {
	case env_op_create:
	case env_op_overwrite:
		msecs = getenv_ulong("watchdog_timeout", 10,
				     CONFIG_OCTEON_WD_TIMEOUT * 1000);
		printf("Starting watchdog with %d ms timeout\n", msecs);
		hw_watchdog_start(msecs);
		break;

	case env_op_delete:
		debug("Disabling watchdog\n");
		hw_watchdog_disable();
		break;
	}
	return 0;
}
U_BOOT_ENV_CALLBACK(watchdog_enable, on_watchdog_enable);
U_BOOT_ENV_CALLBACK(watchdog_timeout, on_watchdog_enable);

#endif
#ifdef CONFIG_OCTEON_SECURE_MODE_BUS

/** Save the addresses for when we disable the port */
static uint64_t bar1_addresses[16];

/**
 * Disables BAR1 PCIe access from a host
 *
 * @param	pcie_port	PCIe port number
 */
static void octeon_disable_bar1(int pcie_port)
{
	union cvmx_sli_s2m_portx_ctl sli_s2m_portx_ctl;
	union cvmx_spemx_bar1_indexx spemx_bar1_indexx;
	int i;

	debug("%s(%d)\n", __func__, pcie_port);
	sli_s2m_portx_ctl.u64 =
	cvmx_read_csr(CVMX_PEXP_SLI_S2M_PORTX_CTL(pcie_port));
	sli_s2m_portx_ctl.cn73xx.bar0_d = 1;
	sli_s2m_portx_ctl.cn73xx.wind_d = 1;
	cvmx_write_csr(CVMX_PEXP_SLI_S2M_PORTX_CTL(pcie_port),
		       sli_s2m_portx_ctl.u64);
	for (i = 0; i < 16; i++) {
		spemx_bar1_indexx.u64 =
			cvmx_read_csr(CVMX_SPEMX_BAR1_INDEXX(i, pcie_port));
		/* Record the address for re-enabling later */
		if (spemx_bar1_indexx.s.addr_v)
			bar1_addresses[i] = spemx_bar1_indexx.s.addr_v;
		spemx_bar1_indexx.s.addr_v = 0;
		cvmx_write_csr(CVMX_SPEMX_BAR1_INDEXX(i, pcie_port),
			       spemx_bar1_indexx.u64);
	}
}

/**
 * Enables BAR1 PCIe access from a host using values saved when disabled
 *
 * @param	pcie_port	PCIe port number
 */
static void octeon_enable_bar1(int pcie_port)
{
	union cvmx_sli_s2m_portx_ctl sli_s2m_portx_ctl;
	union cvmx_spemx_bar1_indexx spemx_bar1_indexx;
	int i;

	debug("%s(%d)\n", __func__, pcie_port);
	sli_s2m_portx_ctl.u64 =
	cvmx_read_csr(CVMX_PEXP_SLI_S2M_PORTX_CTL(pcie_port));
	sli_s2m_portx_ctl.cn73xx.bar0_d = 0;
	sli_s2m_portx_ctl.cn73xx.wind_d = 0;
	cvmx_write_csr(CVMX_PEXP_SLI_S2M_PORTX_CTL(pcie_port),
		       sli_s2m_portx_ctl.u64);
	for (i = 0; i < 16; i++) {
		spemx_bar1_indexx.u64 =
		cvmx_read_csr(CVMX_SPEMX_BAR1_INDEXX(i, pcie_port));
		/* Restore the addresses */
		if (bar1_addresses[i])
			spemx_bar1_indexx.s.addr_v = bar1_addresses[i];
		cvmx_write_csr(CVMX_SPEMX_BAR1_INDEXX(i, pcie_port),
			       spemx_bar1_indexx.u64);

	}
}


static int on_pci_secure_mode(const char *name, const char *value,
			      enum env_op op, int flags)
{
	switch (op) {
		case env_op_create:
		case env_op_overwrite:
			if (value && (*value == '1' || *value == 'y' ||
				      *value == 'Y' || *value == 't' ||
				      *value == 'T'))
				octeon_disable_bar1(CONFIG_OCTEON_SECURE_MODE_BUS);
			else
				octeon_enable_bar1(CONFIG_OCTEON_SECURE_MODE_BUS);
			break;
		case env_op_delete:
			octeon_enable_bar1(CONFIG_OCTEON_SECURE_MODE_BUS);
			break;
	}
	return 0;
}

U_BOOT_ENV_CALLBACK(pci_secure_mode, on_pci_secure_mode);
#endif /* CONFIG_OCTEON_SECURE_MODE_BUS */
