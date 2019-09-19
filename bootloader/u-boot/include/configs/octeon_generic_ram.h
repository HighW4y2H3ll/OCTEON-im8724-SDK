/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004,2005,2008,2011 Cavium, Inc. <support@cavium.com>
 * All Rights Reserved
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

/*
 * This file contains the configuration parameters for a generic
 * pciboot based board.  Since the host that is doing the PCI boot is
 * responsible for DDR configuration, u-boot needs to do very little to
 * get basic functionality working.
 *
 * This should work with any board that can boot over the PCI or PCIe
 * bus with either OCTEON I or OCTEON II.
 *
 * The generic RAM version of U-Boot has the following features:
 * - No flash support
 * - No TLV EEPROM support
 * - No networking support
 * - No boot bus device support
 * - No USB support
 * - PCI console support is enabled
 * - Optional PCI host mode support (but no E1000 driver support)
 * - QLM support is enabled (only works on devices with QLMs)
 * - Very small size
 */
#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_SYS_NO_FLASH			1	/* No flash */
#define OCTEON_NO_NETWORK				/* No network support */
#define CONFIG_OCTEON_NO_BOOT_BUS		1	/* No boot bus support */

/* Common settings */
#include <configs/octeon_common.h>

#undef CONFIG_OCTEON_FLASH				/* No flash */

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST			0
#endif

/*
 * Define CONFIG_PCI only if the system is known to provide a PCI
 * clock to Octeon.  A bus error exception will occur if the inactive
 * Octeon PCI core is accessed.  U-boot is not currently configured to
 * recover when a exception occurs.
 */
#if CONFIG_OCTEON_PCI_HOST
# define CONFIG_PCI
# define CONFIG_PCI_PNP
# define CONFIG_SYS_PCI_64BIT
# define CONFIG_PCI_SCAN_SHOW
#endif

/* Since we're booting remotely, assume that the PCI console is supported */
#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif
/*-----------------------------------------------------------------------
 * U-boot Commands Configuration
 */
#include <config_cmd_default.h>
/* Disable networking commands that are enabled by default */
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS

#define CONFIG_CONSOLE_IS_IN_ENV

#ifdef CONFIG_PCI
/* PCI and bootcmd support */
# define CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif

/* Enable the qlm command, though it won't work on all OCTEON devices */
#define CONFIG_CMD_QLM

/* Enable the I2C command */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_MULTI_BUS
#define CONFIG_I2C_MULTI_BUS
#undef CONFIG_SYS_MAX_I2C_BUS
#define CONFIG_SYS_MAX_I2C_BUS		1

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS			\
        "autoload=n\0"					\
        "dram_size_mbytes=128\0"			\
        ""

/* The environment is stored in RAM only since there's no flash support. */
#define CONFIG_ENV_IS_IN_RAM		1

/* NOTE: these are defined in cvmx-bootloader.h and must match. */
#define U_BOOT_RAM_ENV_ADDR		(0x1000)
#define U_BOOT_RAM_ENV_SIZE		(0x1000)
#define U_BOOT_RAM_ENV_ADDR_2		(U_BOOT_RAM_ENV_ADDR + U_BOOT_RAM_ENV_SIZE)
#define U_BOOT_RAM_ENV_CRC_SIZE		(0x4)
#define CONFIG_ENV_SIZE			U_BOOT_RAM_ENV_SIZE
#define CONFIG_ENV_ADDR			U_BOOT_RAM_ENV_ADDR

#include "octeon_cmd_conf.h"

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(16 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(37 * 1024)

/* Set the board type that you want the generic bootloader to report */
#define GENERIC_BOARD_TYPE  CVMX_BOARD_TYPE_GENERIC

#endif	/* __CONFIG_H */
