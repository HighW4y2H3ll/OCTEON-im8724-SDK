/* (C) Copyright 2011 - 2016 Cavium, Inc. <support@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __OCTEON_CMD_CONF_H__
#define __OCTEON_CMD_CONF_H__

#define CONFIG_CMDLINE_EDITING		/** Turn on command-line editing */
#define CONFIG_SYS_LONGHELP		/** Enable long help support	*/
#define CONFIG_SYS_HELP_CMD_WIDTH	12

#if !defined(OCTEON_NO_NETWORK) && !defined(CONFIG_OCTEON_DISABLE_NETWORKING)
# define CONFIG_CMD_NET			/** Networking support commands */
# define CONFIG_NET_MULTI		/** Support for multiple network interfaces */
# define CONFIG_MII			/** MII support */
# define CONFIG_CMD_MII			/** MII command */
# define CONFIG_CMD_PING		/** PING */
# define CONFIG_CMD_NFS			/** NFS */
# define CONFIG_CMD_DHCP		/** DHCP */
# define CONFIG_CMD_TFTPPUT		/** TFTP put */
# define CONFIG_CMD_TFTPSRV		/** TFTP server */
# ifdef CONFIG_CMD_DATE
#  define CONFIG_CMD_SNTP		/** NTP */
# endif
# define CONFIG_CMD_DNS			/** DNS */
# define CONFIG_CMD_CDP			/** Cisco Discovery Protocol */
#endif /* !OCTEON_NO_NETWORK */

#define CONFIG_CMD_FDT			/** Flat device tree (fdt) */
#define CONFIG_CMD_TIME			/** Time how long a command takes */
#define CONFIG_CMD_OCTEON
#define CONFIG_CMD_OCTEON_ELF
#define CONFIG_CMD_OCTEON_BOOTLOADER_UPDATE
#define CONFIG_CMD_OCTEON_MEM
#define CONFIG_CMD_OCTEON_LINUX
#define CONFIG_CMD_MEMORY		/** md, mm, mw, nm commands */
#define CONFIG_CMD_MEMINFO		/** show memory size */
#define CONFIG_CMD_MISC
#define CONFIG_CMD_SHA1SUM		/** sha1sum command */
#define CONFIG_CMD_MD5SUM		/** md5sum command */
#define CONFIG_CRC32_VERIFY		/** Enable -v option for CRC32 */
#define CONFIG_CMD_UNLZMA
#define CONFIG_CMD_UNZIP
#define CONFIG_CMD_BUNZIP
#define CONFIG_CMD_UNLZMA
#define CONFIG_CMD_STRINGS
#define CONFIG_CMD_SETEXPR
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_EEPROM		/** read/write TWSI EEPROM	*/
#define CONFIG_CMD_I2C			/** Generic TWSI support		*/
#define CONFIG_CMD_GPIO			/** Generic GPIO support		*/

/*#define CONFIG_CMD_SOURCE*/
#define CONFIG_CMD_ECHO
/*#define CONFIG_CMD_REGINFO*/		/* Not supported yet */
#define CONFIG_CMD_OCTEON_REGINFO
#define CONFIG_CMD_ITEST
#define CONFIG_CMD_RUN
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_EDITENV
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_CONSOLE
#define CONFIG_CMD_BDI			/** display board info */
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_SAVES
#define CONFIG_CMD_SOURCE		/** source command */
#define CONFIG_CMD_XIMG			/** Extrace subimage from FIT image */
#define CONFIG_CMD_GPIO			/** GPIO commands */
#define CONFIG_CMD_CRAMFS

#ifdef CONFIG_OCTEON_FLASH
# define CONFIG_CMD_OCTEON_ERASEENV
#endif
#endif	/* __OCTEON_CMD_CONF_H__ */
