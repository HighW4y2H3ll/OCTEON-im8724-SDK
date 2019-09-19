/*
 * (C) Copyright 2004 - 2011 Cavium, Inc.
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
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/gpio.h>
#include <asm/arch/cvmx.h>
#include <miiphy.h>

typedef union {
	uint16_t u16;
	struct {
		uint16_t busy:1, rsv:2, mode:1, op:2, device:5, reg:5;
	} s;
} smi_cmd;

typedef union {
	uint16_t u16;
	struct {
		uint16_t busy:1, op:3, mode:2, rsv:4, ptr:6;
	} s;
} marvel_status_cmd;

DECLARE_GLOBAL_DATA_PTR;

static const char devname[20] = "octeon-mdio0\0";

#if defined(CONFIG_CMD_MII)

#define REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT	8
#define REG_PPM_REG16_MDIO_ENABLE		0x01

#define REG_PPM_REG17_OP_DONE		0x00	/* bit 0..1 - no operation */
#define REG_PPM_REG17_OP_WRITE		0x01	/* bit 0..1 - write operation */
#define REG_PPM_REG17_OP_READ		0x02	/* bit 0..1 - read operation */

#define PSEUDO_PHY_ADDR			0x1e	/* Pseduo PHY address */

/* Pseudo PHY MII registers */
#define REG_PSEUDO_PHY_MII_REG16	0x10	/* register 16 - Switch Register Set Access Control Register */
#define REG_PSEUDO_PHY_MII_REG17	0x11	/* register 17 - Switch Register Set Read/Write Control Register */
#define REG_PSEUDO_PHY_MII_REG24	0x18	/* register 24 - Switch Accesss Register bit 15:0 */
#define REG_PSEUDO_PHY_MII_REG25	0x19	/* register 25 - Switch Accesss Register bit 31:16 */
#define REG_PSEUDO_PHY_MII_REG26	0x20	/* register 26 - Switch Accesss Register bit 47:32 */
#define REG_PSEUDO_PHY_MII_REG27	0x21	/* register 27 - Switch Accesss Register bit 63:48 */

#define REG_PPM_REG17_REG_NUMBER_SHIFT          8
int brcm_miiphy_read(unsigned char page, unsigned char reg, unsigned int *value)
{

	int cmd;
	int max_retry = 0;
	unsigned short temp;

	cmd = (page << REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT) |
	    REG_PPM_REG16_MDIO_ENABLE;

	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG16, cmd);

	cmd = (reg << REG_PPM_REG17_REG_NUMBER_SHIFT) | REG_PPM_REG17_OP_READ;

	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17, cmd);

	do {
		miiphy_read(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17,
			    &temp);
		udelay(10);
	} while ((max_retry++ < 5) &&
		 ((temp & (REG_PPM_REG17_OP_WRITE | REG_PPM_REG17_OP_READ)) !=
		  REG_PPM_REG17_OP_DONE));
	if (max_retry == 5) {
		printf("%s: Read timed out, page=%u, reg=%u, temp=0x%x\n",
		       __func__, page, reg, temp);
	}

	miiphy_read(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG24, &temp);

	*value = temp;

	miiphy_read(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG25, &temp);

	*value |= (temp << 16);

	return 0;

}

int brcm_miiphy_write(unsigned char page, unsigned char reg, unsigned int value)
{

	unsigned short cmd, res;
	unsigned int val;
	int max_retry = 0;

	val = value;

	cmd =
	    (page << REG_PPM_REG16_SWITCH_PAGE_NUMBER_SHIFT) |
	    REG_PPM_REG16_MDIO_ENABLE;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG16, cmd);

	cmd = val >> 0 & 0xffff;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG24, cmd);
#if 1
	cmd = val >> 16 & 0xffff;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG25, cmd);

	cmd = 0;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG26, cmd);
	cmd = 0;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG27, cmd);
#endif
	cmd = (reg << REG_PPM_REG17_REG_NUMBER_SHIFT) | REG_PPM_REG17_OP_WRITE;
	miiphy_write(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17, cmd);

	do {
		miiphy_read(devname, PSEUDO_PHY_ADDR, REG_PSEUDO_PHY_MII_REG17,
			    &res);
		udelay(10);
	} while ((max_retry++ < 5) &&
		 ((res & (REG_PPM_REG17_OP_WRITE | REG_PPM_REG17_OP_READ)) !=
		  REG_PPM_REG17_OP_DONE));
	if (max_retry == 5) {
		printf("%s: Write timed out, page=%u, reg=%u, value=0x%x, res=0x%x\n",
		       __func__, page, reg, value, res);
	}
	return 0;
}

void brcm_workaround(void)
{
	typedef struct {
		unsigned char addr, reg;
		unsigned short data;
	} _miidata;
	static _miidata miidata[] = {
		{0x0, 0x18, 0x0c00},
		{0x0, 0x17, 0x0f08},
		{0x0, 0x15, 0x0201},
		{0x0, 0x17, 0x0001},
		{0x0, 0x15, 0x0027},
		{0x0, 0x17, 0x001f},
		{0x0, 0x15, 0x0300},
		{0x0, 0x17, 0x601f},
		{0x0, 0x15, 0x0002},
		{0x0, 0x17, 0x0f75},
		{0x0, 0x15, 0x0028},
		{0x1, 0x18, 0x0c00},
		{0x1, 0x17, 0x0f08},
		{0x1, 0x15, 0x0201},
		{0x1, 0x17, 0x0001},
		{0x1, 0x15, 0x0027},
		{0x1, 0x17, 0x001f},
		{0x1, 0x15, 0x0300},
		{0x1, 0x17, 0x601f},
		{0x1, 0x15, 0x0002},
		{0x1, 0x17, 0x0f75},
		{0x1, 0x15, 0x0028},
		{0x2, 0x18, 0x0c00},
		{0x2, 0x17, 0x0f08},
		{0x2, 0x15, 0x0201},
		{0x2, 0x17, 0x0001},
		{0x2, 0x15, 0x0027},
		{0x2, 0x17, 0x001f},
		{0x2, 0x15, 0x0300},
		{0x2, 0x17, 0x601f},
		{0x2, 0x15, 0x0002},
		{0x2, 0x17, 0x0f75},
		{0x2, 0x15, 0x0028},
		{0x3, 0x18, 0x0c00},
		{0x3, 0x17, 0x0f08},
		{0x3, 0x15, 0x0201},
		{0x3, 0x17, 0x0001},
		{0x3, 0x15, 0x0027},
		{0x3, 0x17, 0x001f},
		{0x3, 0x15, 0x0300},
		{0x3, 0x17, 0x601f},
		{0x3, 0x15, 0x0002},
		{0x3, 0x17, 0x0f75},
		{0x3, 0x15, 0x0028},
		{0x4, 0x18, 0x0c00},
		{0x4, 0x17, 0x0f08},
		{0x4, 0x15, 0x0201},
		{0x4, 0x17, 0x0001},
		{0x4, 0x15, 0x0027},
		{0x4, 0x17, 0x001f},
		{0x4, 0x15, 0x0300},
		{0x4, 0x17, 0x601f},
		{0x4, 0x15, 0x0002},
		{0x4, 0x17, 0x0f75},
		{0x4, 0x15, 0x0028},
		{0x3, 0x1c, 0xb012},
		{0x3, 0x1c, 0xac60},
		{0x2, 0x1c, 0xac4e},
		{0x4, 0x17, 0x0f96},
		{0x4, 0x15, 0x0010},
		{0x4, 0x17, 0x0f97},
		{0x4, 0x15, 0x0c0c}
	};
	int i = 0;
	int arraysize = sizeof(miidata) / sizeof(_miidata);
	for (i = 0; i < arraysize; i++) {
		if (miiphy_write(devname, miidata[i].addr,
				 miidata[i].reg, miidata[i].data) != 0) {
			printf("Error writing to the PHY addr=%02x reg=%02x\n",
			       miidata[i].addr, miidata[i].reg);
		}
	}
}

void board_mdio_init(void)
{
	cvmx_smi_clk_t smi_clk;
	int val;
	/* Force us into RGMII mode */
	cvmx_write_csr(CVMX_GMXX_INF_MODE(0), 0x2);

	/* Set the SMI clock to 7 MHz, which is the fastest that
	 * all the parts on the board can handle.
	 */
	smi_clk.u64 = cvmx_read_csr(CVMX_SMI_CLK);
	smi_clk.cn50xx.phase = (gd->cpu_clk + 7) / 7;
	cvmx_write_csr(CVMX_SMI_CLK, smi_clk.u64);
	cvmx_read_csr(CVMX_SMI_CLK);

	/* Enable SMI to talk with the GMII switch */
	cvmx_write_csr(CVMX_SMI_EN, 0x1);
	cvmx_read_csr(CVMX_SMI_EN);

	/* Broadcom switch setup */

	/* Force IMP speed/mode */
	brcm_miiphy_write(0, 0xe, 0x8b);

	/* Force port 5 speed/mode */
	brcm_miiphy_write(0x0, 0x5d, 0x4b);

	/* Set up port based VLAN - effectively divides
	 * the switch into 2 - one 5 port, and one 2 port
	 * switches
	 */
	/* Octeon port 1 (IMP) is connected to 4 external switch ports
	 * for LAN connection
	 */
	brcm_miiphy_write(0x31, 0x0, 0x10f);	/* Port 0 */
	brcm_miiphy_write(0x31, 0x2, 0x10f);	/* Port 1 */
	brcm_miiphy_write(0x31, 0x4, 0x10f);	/* Port 2 */
	brcm_miiphy_write(0x31, 0x6, 0x10f);	/* Port 3 */
	brcm_miiphy_write(0x31, 0x10, 0x10f);	/* IMP */

	/* Set default VLAN ID to 0x1 */
	brcm_miiphy_write(0x34, 0x10, 0x1);	/* Port 0 */
	brcm_miiphy_write(0x34, 0x12, 0x1);	/* Port 1 */
	brcm_miiphy_write(0x34, 0x14, 0x1);	/* Port 2 */
	brcm_miiphy_write(0x34, 0x16, 0x1);	/* Port 3 */
	brcm_miiphy_write(0x34, 0x20, 0x1);	/* IMP */

	/* Octeon port 0 (port 5) is connected to 1 external switch port
	 * for WAN connection
	 */
	brcm_miiphy_write(0x31, 0x8, 0x30);	/* Port 4 */
	brcm_miiphy_write(0x31, 0xa, 0x30);	/* Port 5 */

	/* Set default VLAN ID to 0x2 */
	brcm_miiphy_write(0x34, 0x18, 0x2);	/* Port 4 */
	brcm_miiphy_write(0x34, 0x1A, 0x2);	/* Port 5 */

	/* Set up two VLANs (1 and 2) that correspone to the port based
	 * VLAN configuration.  This should allow the two 'logical' switches
	 * to have the same MAC address in each one, as would be the case in Octeon
	 * bridging between its two ports.
	 */
	brcm_miiphy_write(0x05, 0x83, (0x10f << 9) | 0x10f);	/* Vlan table entry contents */
	brcm_miiphy_write(0x05, 0x81, 0x1);	/* Vlan entry index */
	brcm_miiphy_write(0x05, 0x80, 0x80);	/* Write VLAN entryx */

	brcm_miiphy_write(0x05, 0x83, (0x30 << 9) | 0x30);	/* Vlan table entry contents */
	brcm_miiphy_write(0x05, 0x81, 0x2);	/* Vlan entry index */
	brcm_miiphy_write(0x05, 0x80, 0x80);	/* Write VLAN entryx */

	/* Enable 802.1Q */
	brcm_miiphy_read(0x34, 0x0, &val);
	val = val | 0x80;
	brcm_miiphy_write(0x34, 0x0, val);

	/* Must disable STP for all ports, as
	 * disabling forwarding via HW_FWDG_EN pin changes
	 * the defaults
	 */
	brcm_miiphy_write(0x0, 0x0, 0);
	brcm_miiphy_write(0x0, 0x1, 0);
	brcm_miiphy_write(0x0, 0x2, 0);
	brcm_miiphy_write(0x0, 0x3, 0);
	brcm_miiphy_write(0x0, 0x4, 0);
	brcm_miiphy_write(0x0, 0x5, 0);
	brcm_miiphy_write(0x0, 0x6, 0);
	brcm_miiphy_write(0x0, 0x7, 0);

	/* Now enable forwarding */
	brcm_miiphy_read(0x0, 0xb, &val);
	val = (val & ~0x3) | 0x2;
	brcm_miiphy_write(0x0, 0xb, val);

	brcm_workaround();

}
#endif /* MII */

int octeon_get_board_major_rev(void)
{
	return (gd->arch.board_desc.rev_major);
}

int octeon_get_board_minor_rev(void)
{
	return (gd->arch.board_desc.rev_minor);
}

int checkboard(void)
{
	return 0;
}

/* Convert a character to lower case */
static inline char _tolower(char c)
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
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	char *env_str;
	int cpu_ref = 50;
	int addr;

	/* NOTE: this is early in the init process, so the serial port is not
	 * yet configured
	 */

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
		gd->arch.board_desc.board_type = CVMX_BOARD_TYPE_BBGW_REF;
		gd->arch.board_desc.rev_major = 1;
		gd->arch.board_desc.rev_minor = 0;
	}

	/* Populate global data from eeprom */
	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_CLOCK_DESC_TYPE, 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.clock_desc), ee_buf,
		       sizeof(octeon_eeprom_clock_desc_t));
	}


	cpu_ref = CN5020_FORCED_DDR_AND_CPU_REF_HZ / 1000000;
	gd->mem_clk = 266;
	gd->arch.ddr_ref_hertz = CN5020_FORCED_DDR_AND_CPU_REF_HZ;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 EEPROM_MAC_ADDR_TYPE,
					 0, ee_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	if (addr >= 0) {
		memcpy((void *)&(gd->arch.mac_desc), ee_buf,
		       sizeof(octeon_eeprom_mac_addr_t));
	} else {
		/* Make up some MAC addresses */
		gd->arch.mac_desc.count = 255;
		gd->arch.mac_desc.mac_addr_base[0] = 0x00;
		gd->arch.mac_desc.mac_addr_base[1] = 0xDE;
		gd->arch.mac_desc.mac_addr_base[2] = 0xAD;
		gd->arch.mac_desc.mac_addr_base[3] =
		    (gd->arch.board_desc.rev_major << 4) | gd->arch.
		    board_desc.rev_minor;
		gd->arch.mac_desc.mac_addr_base[4] =
		    gd->arch.board_desc.serial_str[0];
		gd->arch.mac_desc.mac_addr_base[5] = 0x00;

		/* Read MAC address base/count from env */
		if (!(env_str = getenv("octeon_mac_base")) ||
		    !ether_aton(env_str,
				(uint8_t *) (gd->arch.mac_desc.mac_addr_base))) {
			/* Special value allows silent overrides */
			gd->arch.mac_desc.mac_addr_base[0] = 0x00;
			gd->arch.mac_desc.mac_addr_base[1] = 0xba;
			gd->arch.mac_desc.mac_addr_base[2] = 0xdd;
		}
	}

	/* Read CPU clock multiplier */
	uint64_t data = cvmx_read_csr(CVMX_DBG_DATA);
	data = data >> 18;
	data &= 0x1f;

	gd->cpu_clk = data * cpu_ref * 1000000;

	gpio_direction_output(4, 0);	/* GPIO 4 resets the switch */
	gpio_direction_output(2, 0);	/* GPIO 2 resets the SLIC */
	mdelay(1);
	gpio_set_value(4, 1);
	gpio_set_value(2, 0);

	return 0;

}

void octeon_led_str_write(const char *str)
{

}
