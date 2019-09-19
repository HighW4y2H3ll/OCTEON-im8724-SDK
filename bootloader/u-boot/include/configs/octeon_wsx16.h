/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004 - 2012 Cavium, Inc. <support@cavium.com>
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
 * This file contains the configuration parameters for
 * Octeon EBT3000 board.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * Define CONFIG_OCTEON_PCI_HOST = 1 to map the pci devices on the
 * bus.  Define CONFIG_OCTEON_PCI_HOST = 0 for target mode when the
 * host system performs the pci bus mapping instead.  Note that pci
 * commands are enabled to allow access to configuration space for
 * both modes.
 */
#ifndef CONFIG_OCTEON_PCI_HOST
# define CONFIG_OCTEON_PCI_HOST		0
#endif

#define OCTEON_FIRST_PCIE_BUSNO		1

#include "octeon_common.h"

#include "octeon_wsx16_shared.h"

/** Disable DDR3 support */
#define CONFIG_OCTEON_DISABLE_DDR3

/* Set this for 533 memory, as this config will work with 400 Mhz memory,
 * albeit with poor performance.  This at least prevents a dead board.
 */
#define WSX16_REV1_DEF_DRAM_FREQ	333

#define CONFIG_OCTEON_LLM_WORKAROUND
/*#define CONFIG_OCTEON_PCIX_WORKAROUND*/

/* Default DDR clock if tuple doesn't exist in EEPROM */
#define WSX16_DEF_DRAM_FREQ		200

/* Remap the Octeon TWSI Slave Address. Default is 0x77. */
#define TWSI_BUS_SLAVE_ADDRESS		0x65

/* TWSI address for the ispPAC-POWR1220AT8 Voltage Controller chip */
#define BOARD_ISP_TWSI_ADDR		0x77

#define CONFIG_SYS_I2C_EEPROM_ADDR	OCTEON_WSX16_EEPROM_ADDR
#define CONFIG_SYS_DEF_EEPROM_ADDR	CONFIG_SYS_I2C_EEPROM_ADDR
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	5


/* Set bootdelay to 0 for immediate boot */
#define CONFIG_BOOTDELAY	0	/* autoboot after X seconds	*/

#undef	CONFIG_BOOTARGS

/*
 * Define CONFIG_PCI only if the system is known to provide a PCI
 * clock to Octeon.  A bus error exception will occur if the inactive
 * Octeon PCI core is accessed.  U-boot is not currently configured to
 * recover when a exception occurs.
 */
#if CONFIG_OCTEON_PCI_HOST
#undef CONFIG_PCIAUTO_SKIP_HOST_BRIDGE
# define CONFIG_PCI_CONFIG_HOST_BRIDGE
#endif

/*
 * The WSX16 does not use the internal arbiter in Octeon.
 * Enable this for boards that do.
 */
/* #define USE_OCTEON_INTERNAL_ARBITER */

/* Change the NOR MTD partitioning here.  The JFFS2 filesystem should be kept
 * as small as reasonably possible to minimize boot time.  This environment
 * variable is suitabe for passing directly to the Linux kernel.
 * The 'mtdids' environment variable is used in conjunction with the 'mtdparts'
 * variable to define the MTD partitions for u-boot.
 */
#define MTDPARTS_DEFAULT				\
	"mtdparts=octeon_nor0:2m(bootloader)ro,"	\
	"2m(kernel),"					\
	"4088k(cramfs),"				\
	"8k(environment)ro\0"

#define MTDIDS_DEFAULT	"nor0=octeon_nor0\0"

/* Define this to enable built-in octeon ethernet support */
#define CONFIG_OCTEON_RGMII_ENET

/* Enable Octeon built-in networking if RGMII support is enabled */
#if defined(CONFIG_OCTEON_RGMII_ENET)
# define CONFIG_OCTEON_INTERNAL_ENET
#endif

#include "octeon_cmd_conf.h"

#ifdef  CONFIG_CMD_NET
/**
 * Define available PHYs
 */
# define CONFIG_PHY_GIGE
# define CONFIG_PHY_MARVELL
# include "octeon_network.h"
#endif

#define CONFIG_CMD_OCTEON_TLVEEPROM

#define CONFIG_CMD_SDRAM

#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/

#define CONFIG_CMD_DATE
#define CONFIG_CMD_OCTEON_BOOTBUS

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
        "burn_app=erase $(flash_unused_addr) +$(filesize);cp.b $(fileaddr) $(flash_unused_addr) $(filesize)\0"	\
        "bf=bootoct $(flash_unused_addr) forceboot numcores=$(numcores)\0"			\
        "autoload=n\0"					\
        ""

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_FLASH_SIZE	        	(2*1024*1024)	/* Flash size (bytes) */
#define CONFIG_SYS_MAX_FLASH_BANKS		1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT		(64)	/* max number of sectors on one chip */
#undef CONFIG_SYS_FLASH_USE_BUFFER_WRITE		/* Not supported by Toshiba flash */

#define CONFIG_SYS_FLASH_CFI_WIDTH		FLASH_CFI_8BIT
#define CONFIG_SYS_FLASH_INTERFACE_WIDTH	FLASH_CFI_X8
#define CONFIG_SYS_FLASH_CFI			1
#define CONFIG_FLASH_CFI_DRIVER			1


#if CONFIG_RAM_RESIDENT
# define	CONFIG_ENV_IS_NOWHERE		1
#else
# define	CONFIG_ENV_IS_IN_FLASH		1
#endif

/* Address and size of Primary Environment Sector	*/
#define CONFIG_ENV_SIZE		(64*1024)
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_FLASH_SIZE - CONFIG_ENV_SIZE)

/* Enable watchdog support */
#define CONFIG_HW_WATCHDOG

/* Real-time Clock */
#define CONFIG_RTC_DS1337
/* Address of RTC on TWSI bus */
#define CONFIG_SYS_I2C_RTC_ADDR                 0x68
/* TWSI bus number for the real-time clock */
#define CONFIG_SYS_RTC_BUS_NUM                  0

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		(8 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(32 * 1024)

#endif	/* __CONFIG_H__ */
