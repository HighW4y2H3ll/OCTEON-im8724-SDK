/***********************license start************************************
 * Copyright (c) 2004-2011 Cavium, Inc. (support@cavium.com). All rights
 * reserved.
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
 *     * Neither the name of Cavium, Inc. nor the names of
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
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

/*
 * This file contains the configuration parameters for the octeon simulator
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Must define for common */
#define CONFIG_SYS_NO_FLASH		1
#define	CONFIG_ENV_IS_NOWHERE		1

/* default load address	*/
#define	CONFIG_SYS_LOAD_ADDR		0x10000000

#define DEFAULT_ECLK_FREQ_MHZ		600

#include "octeon_common.h"

#undef CONFIG_OCTEON_FLASH
/* Don't use board serial EEPROM */
#undef CONFIG_OCTEON_EEPROM_TUPLES

/* CONFIG_OCTEON_SIM set by Makefile in include/config.h */

/* Below are several defines that are used to control whether certain
 * changes for the simulator are made, based on the reason for the change.
 */
/* Changes that are purely speed optimizations. */
#define CONFIG_OCTEON_SIM_SPEED		1
/* Changes due to boot bus not having real flash */
#define CONFIG_OCTEON_SIM_NO_FLASH	1
/* Changes due to setup differences (ie DRAM size simulated, etc.) */
#define CONFIG_OCTEON_SIM_SETUP		1
/* Changes due to hw/sim differences (no workarounds, etc.) */
#define CONFIG_OCTEON_SIM_HW_DIFF	1
/* Changes due to no DDR init being done for the simulator */
#define CONFIG_OCTEON_SIM_NO_DDR	1
/* Software differences, such as default coremask to run apps on */
#define CONFIG_OCTEON_SIM_SW_DIFF	1
/* Memory layout differences (reserverved blocks), due application always being
 * on boot bus
 */
#define CONFIG_OCTEON_SIM_MEM_LAYOUT	1

/** Enable octbootbus command */
#define CONFIG_CMD_OCTEON_BOOTBUS

#define DCLK_FREQ_MHZ			667	/* 1/2 Data Rate */

#if 0
/** Support the native U-Boot API */
#define CONFIG_API
#endif

/** Enable Octeon's serial port */
#define CONFIG_OCTEON_SERIAL

/* Set this to disable all accesses to the boot bus in the bootloader */
/* You also need to change the TEXT_BASE defined board/octeon_sim/config.mk
 * and rebuild all files to load the bootloader at a different address.
 * usually set during config by top level makefile
 */
/* #define CONFIG_OCTEON_NO_BOOT_BUS  1 */

#define CONFIG_ETHADDR		00:DE:AD:BE:EF:00	/* Ethernet address */

/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#define CONFIG_OCTEON_QLM
#define CONFIG_OCTEON_OCX		/** Enable NUMA OCI support */

#undef CONFIG_DOWNLOAD_BAUDRATE
#define CONFIG_DOWNLOAD_BAUDRATE	460800

#undef	CONFIG_BOOTARGS

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"addmisc=setenv bootargs $(bootargs) "				\
		"console=ttyS0,$(baudrate) "				\
		"panic=1\0"						\
	"bootfile=/tftpboot/vmlinux.srec\0"				\
	"loadaddr=0x10000000\0"						\
	"load=tftp 0x80500000 $(u-boot)\0"				\
	"autoload=n\0"							\
	""

/* Set default boot command */
#if !defined(CONFIG_OCTEON_NO_BOOT_BUS)
# define CONFIG_BOOTCOMMAND		"bootoct 0x10000000"
#else
/* Expect ELF image at 0x4000000 in DRAM if not using boot bus */
# define CONFIG_BOOTCOMMAND		"bootoct 0x4000000"
#endif

#if 0
#define CONFIG_SYS_PCI_CONSOLE
#ifdef CONFIG_SYS_PCI_CONSOLE
#define CONFIG_CONSOLE_MUX
#define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif
#endif

#include "octeon_cmd_conf.h"

/*#define ENABLE_NETWORKING*/
#define CONFIG_OCTEON_DISABLE_NETWORKING

#ifdef ENABLE_NETWORKING
# define CONFIG_OCTEON_INTERNAL_ENET
# define CONFIG_NET_MULTI
# define CONFIG_OCTEON_RGMII_ENET
# define CONFIG_OCTEON_MGMT_ENET
# define CONFIG_CMD_NET
# define CONFIG_CMD_DHCP
# define CONFIG_CMD_PING
# define CONFIG_CMD_NFS
# define CONFIG_CMD_BOOTD
# define CONFIG_CMD_MII
#endif

#include <config_cmd_default.h>

#define CONFIG_OCTEON_SE_NAPI
/*
 * Miscellaneous configurable options
 */
#define CONFIG_CMD_ENV
#define CONFIG_CMD_OCTEON

#define CONFIG_SYS_PROMPT		"Octeon simulator"

#define CONFIG_SYS_MHZ			600

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#undef CONFIG_CMD_IMLS

/* Address and size of Primary Environment Sector */
#define CONFIG_ENV_ADDR			0xB0030000
#define CONFIG_ENV_SIZE			(4*1024)

#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		0
#endif

/*#define CONFIG_SYS_PCI_CONSOLE*/
#ifdef CONFIG_SYS_PCI_CONSOLE
# define CONFIG_CONSOLE_MUX
# define CONFIG_OCTEON_BOOTCMD /* Deprecated */
#endif


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		( 8 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(32 * 1024)

#endif	/* __CONFIG_H__ */
