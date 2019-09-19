/*
 * (C) Copyright 2004-2012 Cavium, Inc. <support@cavium.com>
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
#include <linux/ctype.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <pci.h>
#include <net.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * Modify the device tree to remove all unused interface types.
 */
int board_fixup_fdt(void)
{
	const char *fdt_key;
	int is_spi4000 = 0;
	int is_spi = 0;
	union cvmx_gmxx_inf_mode gmx_mode;

	gmx_mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(0));

	if (!gmx_mode.cn58xx.en) {
		fdt_key ="0,none"; /* Disabled */
	} else {
		if (gmx_mode.cn58xx.type) {
			fdt_key = "0,spi";
			is_spi4000 = cvmx_spi4000_is_present(0);
			is_spi = 1;
		} else {
			fdt_key = "0,rgmii";
		}
	}
	octeon_fdt_patch(working_fdt, fdt_key, NULL);

	if (is_spi) {
		if (is_spi4000)
			fdt_key = "9,spi4000";
		else
			fdt_key = "9,not-spi4000";
		octeon_fdt_patch(working_fdt, fdt_key, NULL);
	}
}

int checkboard(void)
{
	return 0;
}

/* Convert a character to lower case */
__inline__ static char _tolower(char c)
{
	if ((c >= 'A') && (c <= 'Z')) {
		c = (c - 'A') + 'a';
	}
	return c;
}

/* MAC address string <-> byte array from glibc. */
static uint8_t *ether_aton(char *addr_string, uint8_t * addr_bytes)
{
	size_t cnt;

	for (cnt = 0; cnt < 6; ++cnt) {
		unsigned int number;
		char ch;

		ch = _tolower(*addr_string++);
		if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
			return NULL;
		number = isdigit(ch) ? (ch - '0') : (ch - 'a' + 10);

		ch = _tolower(*addr_string);
		if ((cnt < 5 && ch != ':')
		    || (cnt == 5 && ch != '\0' && !isspace(ch))) {
			++addr_string;
			if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
				return NULL;
			number <<= 4;
			number += isdigit(ch) ? (ch - '0') : (ch - 'a' + 10);

			ch = *addr_string;
			if (cnt < 5 && ch != ':')
				return NULL;
		}

		/* Store result.  */
		addr_bytes[cnt] = (unsigned char)number;

		/* Skip ':'.  */
		++addr_string;
	}

	return addr_bytes;
}

int early_board_init(void)
{
	char *env_str;
	int cpu_ref;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

	/* Populate global data from eeprom */
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.clock_desc), ee_buf,
		       sizeof(octeon_eeprom_clock_desc_t));
	}

	/* Determine board type/rev */
	strncpy((char *)(gd->arch.board_desc.serial_str), "unknown", SERIAL_LEN);
	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_BOARD_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.board_desc), ee_buf,
		       sizeof(octeon_eeprom_board_desc_t));
	} else {
		gd->flags |= GD_FLG_BOARD_DESC_MISSING;
		gd->arch.board_desc.rev_minor = 0;
		gd->arch.board_desc.board_type = CVMX_BOARD_TYPE_THUNDER;
		/* Try to determine board rev by looking at PAL */
		gd->arch.board_desc.rev_major = 1;

	}
	gd->board_type = gd->arch.board_desc.board_type;

	if (gd->arch.clock_desc.cpu_ref_clock_mhz_x_8) {
		cpu_ref = gd->arch.clock_desc.cpu_ref_clock_mhz_x_8 / 8;
		gd->mem_clk = gd->arch.clock_desc.ddr_clock_mhz;
	} else {
		gd->flags |= GD_FLG_CLOCK_DESC_MISSING;
		/* Default values */
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;
		gd->mem_clk = OCTEON_DDR_CLOCK_MHZ;
	}

	/* Some sanity checks */
	if (cpu_ref <= 0)
		cpu_ref = DEFAULT_CPU_REF_FREQUENCY_MHZ;

	if (gd->mem_clk <= 0)
		gd->mem_clk = OCTEON_DDR_CLOCK_MHZ;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_MAC_ADDR_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.mac_desc), ee_buf,
		       sizeof(octeon_eeprom_mac_addr_t));
	} else {
		/* Read MAC address base/count from env */
		if (!(env_str = getenv("octeon_mac_base"))
		    || !ether_aton(env_str,
				   (uint8_t *) (gd->arch.mac_desc.
						mac_addr_base))) {
			gd->arch.mac_desc.count = 14;
			/* Make up some MAC addresses */
			gd->arch.mac_desc.count = 255;
			gd->arch.mac_desc.mac_addr_base[0] = 0x00;
			gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
			gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
			gd->arch.mac_desc.mac_addr_base[3] =
			    (gd->arch.board_desc.rev_major << 4) |
			    gd->arch.board_desc.rev_minor;
			gd->arch.mac_desc.mac_addr_base[4] =
			    gd->arch.board_desc.serial_str[0];
			gd->arch.mac_desc.mac_addr_base[5] = 0x00;
		}
	}

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_DBG_DATA);
	data = data >> 18;
	data &= 0x1f;

	gd->cpu_clk = data * cpu_ref * 1000000;

	/* Set the LAN bypass defaults for Thunder */
	/* Startup with bypass disabled and watchdog cleared */
	gpio_direction_output(6, 0);	/* GPIO 6 controls the bypass enable */
	gpio_direction_output(7, 1);	/* GPIO 7 controls the watchdog clear */
	gpio_direction_input(5);	/* GPIO 5 tells you the bypass status */

	return 0;

}

#ifdef CONFIG_OCTEON_LANBYPASS
void board_net_pre_init()
{
	/* Set the LAN bypass defaults for Thunder */
	gpio_direction_output(6, 0);	/* GPIO 6 controls the bypass enable */
	gpio_direction_output(7, 1);	/* GPIO 7 controls the watchdog clear */
	gpio_direction_input(5);	/* GPIO 5 tells you the bypass status */
}
#endif
