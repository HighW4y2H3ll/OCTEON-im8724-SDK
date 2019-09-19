/***********************license start************************************
 * Copyright (c) 2012 - 2014 Cavium Inc. (support@cavium.com). All rights
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
 *
 * For any questions regarding licensing please contact
 * support@cavium.com
 *
 ***********************license end**************************************/

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/octeon-model.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx-asm.h>
#include <asm/arch/cvmx-mio-defs.h>
#include <asm/arch/octeon_boot_bus.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/octeon_board_phy.h>
#include <asm/arch/octeon_pci.h>
#ifdef CONFIG_OCTEON_ENABLE_PAL
#  include <asm/arch/octeon_boot.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/* Generic bootloaders *MUST* have a TLV EEPROM */
#if !defined(CONFIG_OCTEON_GENERIC_EMMC_STAGE2)		&& !defined(CONFIG_OCTEON_GENERIC_CN70XX_EMMC_STAGE2)	&& \
    !defined(CONFIG_OCTEON_GENERIC_SPI_STAGE2)		&& !defined(CONFIG_OCTEON_GENERIC_CN70XX_SPI_STAGE2)	&& \
    !defined(CONFIG_OCTEON_GENERIC_NAND2_STAGE2)	&& !defined(CONFIG_OCTEON_GENERIC_CN70XX_NAND2_STAGE2)	&& \
    !defined(CONFIG_OCTEON_GENERIC_NAND_STAGE2)
/* We can't do this check for generic bootloaders */
# if CONFIG_SYS_I2C_EEPROM_ADDR || CONFIG_SYS_DEF_EEPROM_ADDR
#  define USE_EEPROM	1
# else
#  define USE_EEPROM	0
# endif
#else
# define USE_EEPROM	1
#endif

/**
 * Dynamically adjust the board name, used for prompt generation
 * @param name - name string to be adjusted
 * @param max_len - maximum length of name
 *
 * This function can overwrite the name of a board, for example based on
 * the processor installed.
 */
void octeon_adjust_board_name(char *name, size_t max_len) __attribute__((weak));
void octeon_adjust_board_name(char *name, size_t max_len)
{
	return;
}

int checkboard(void)
{
	return 0;
}
int checkboard(void) __attribute__((weak));

int board_usb_preinit(void) __attribute__((weak));

int board_usb_preinit(void)
{
	return 0;
}

void board_usb_postinit(void) __attribute__((weak));

void board_usb_postinit(void)
{
}

#ifdef CONFIG_OCTEON_ENABLE_PAL
void show_pal_info(void) __attribute__((weak, alias("__show_pal_info")));
void __show_pal_info(void)
{
	int pal_rev_maj = 0;
	int pal_rev_min = 0;
	int voltage_100ths = 0;
	int voltage_1s = 0;
	int mcu_rev_maj = 0;
	int mcu_rev_min = 0;
	char tmp[10];
	uint64_t pal_addr;

	if (octeon_boot_bus_get_dev_info("/soc/bootbus/pal",
					 "cavium,ebt3000-pal", &pal_addr, NULL))
		pal_addr = gd->arch.pal_addr;

	if (!pal_addr) {
		debug("%s: PAL not found\n", __func__);
		octeon_led_str_write("Boot    ");
		return;
	}

	if (octeon_read64_byte(pal_addr) == 0xa5
	    && octeon_read64_byte(pal_addr + 1) == 0x5a) {
		pal_rev_maj = octeon_read64_byte(pal_addr + 2);
		pal_rev_min = octeon_read64_byte(pal_addr + 3);
		if ((octeon_read64_byte(pal_addr + 4)) > 0xf) {
			voltage_1s = 0;
			voltage_100ths =
			    (600 + (31 - octeon_read64_byte(pal_addr + 4)) * 25);
		} else {
			voltage_1s = 1;
			voltage_100ths =
			    ((15 - octeon_read64_byte(pal_addr + 4)) * 5);
		}
	}

	if (twsii_mcu_read(0x00) == 0xa5
	    && twsii_mcu_read(0x01) == 0x5a) {
		gd->arch.mcu_rev_maj = mcu_rev_maj = twsii_mcu_read(2);
		gd->arch.mcu_rev_min = mcu_rev_min = twsii_mcu_read(3);
	}

	printf("PAL rev: %d.%02d, MCU rev: %d.%02d, CPU voltage: %d.%02d\n",
	       pal_rev_maj, pal_rev_min, mcu_rev_maj, mcu_rev_min,
	       voltage_1s, voltage_100ths);

	/* Display CPU speed on display */
	sprintf(tmp, "%d %.1d.%.2lu ", (int)gd->cpu_clk,
		voltage_1s, (long unsigned int)voltage_100ths);
	octeon_led_str_write(tmp);
}

#endif /* CONFIG_OCTEON_ENABLE_PAL */

int octeon_get_num_fuses(void)
{
	return 3056;
}

#define NUMBER_OF_EFUSE_READS	((num_fuses() + 7) / 8)

static int octeon_get_fuse_addr(int node, int addr)
{
	union cvmx_mio_fus_rcmd fus_rcmd;

	fus_rcmd.u64 = 0;
	fus_rcmd.s.pend = 1;
	fus_rcmd.s.addr_hi = (addr >> 8) & 3;
	fus_rcmd.s.addr = (addr & 0xff);
	fus_rcmd.s.efuse = 1;
	cvmx_write_csr_node(node, CVMX_MIO_FUS_RCMD, fus_rcmd.u64);
	do {
		CVMX_SYNC;
		fus_rcmd.u64 = cvmx_read_csr_node(node, CVMX_MIO_FUS_RCMD);
	} while (fus_rcmd.s.pend);
	debug("%s(%d, 0x%x) : 0x%x (0x%llx)\n", __func__, node, addr,
	      fus_rcmd.s.dat, fus_rcmd.u64);
	return fus_rcmd.s.dat;
}

/**
 * Reads a range of fuses into a buffer starting at a buffer offset bit
 *
 * @param	node		OCX node number
 * @param	fuse_start	Starting fuse number [0..octeon_get_num_fuses()-1]
 * @param	fuse_end	Ending fuse number [0..octeon_get_num_fuses()-1]
 * @param[out]	buffer		Buffer to store the fuses into
 * @param	buf_start_bit	Starting bit number in the buffer to write
 *				fuse data to.
 *
 * @return	0 for success, -1 on error
 *
 * NOTE: The buffer must be cleared by the caller before calling this function
 *	 since this function is designed to insert or append fuse values into
 *	 an existing buffer.
 */
int octeon_get_fuse(int node, int fuse_start, int fuse_end, uint8_t *buffer,
		    int buf_start_bit)
{
	int shift;
	int i;
	int start, end;
	int buf_start, buf_mask, buf_bit_off, buf_idx;
	uint16_t val;

	if ((fuse_start >= octeon_get_num_fuses()) ||
	    (fuse_end > octeon_get_num_fuses()))
		return -1;

	start = fuse_start / 8;
	end = fuse_end / 8;
	shift = fuse_start % 8;

	buf_start = buf_start_bit / 8;
	buf_bit_off = buf_start_bit % 8;
	buf_mask = 0xff >> buf_bit_off;
	buf_idx = buf_start;
	debug("%s(%d, %d, %d, %p, %d): start: %d, end: %d, shift: %d, buf_start: %d, buf_bit_off: %d, buf_mask: 0x%x\n",
	      __func__, node, fuse_start, fuse_end, buffer, buf_start_bit,
	      start, end, shift, buf_start, buf_bit_off, buf_mask);
	for (i = start; i <= end; i++) {
		if (shift) {
			val = (((octeon_get_fuse_addr(node, i) << 8) |
				octeon_get_fuse_addr(node, i + 1)) >> shift);
			val &= 0xff;
		} else {
			val = octeon_get_fuse_addr(node, i);
		}
		debug("  val[%d]: 0x%x\n", i, val);
		if (buf_bit_off) {
			buffer[buf_idx] |= (val & buf_mask) << buf_bit_off;
			buffer[buf_idx + 1] |= (val & ~buf_mask) >>
							(8 - buf_bit_off);
		} else {
			buffer[buf_idx] = val;
		}
		debug("buf[%d]: 0x%02x (%02x)\n", buf_idx, buffer[buf_idx],
		      buffer[buf_idx + 1]);
		buf_idx++;
	}
	return 0;
}

/**
 * Fills in a buffer with an 80-bit unique Octeon chip ID
 *
 * @param[out]	buffer	10-byte buffer to store the unique ID
 *
 * @return	0 for success, -1 on error
 */
int octeon_get_unique_id(int node, uint8_t buffer[10])
{
	union {

		uint64_t	u64;
		struct {
			uint64_t	f1:10;	/* 694-703 */
			uint64_t	f2:6;	/* 688-683 */
			uint64_t	f3:6;	/* 682 */
			uint64_t	f4:6;	/* 676 */
			uint64_t	f5:6;	/* 670 */
			uint64_t	f6:6;	/* 664 */
			uint64_t	f7:6;	/* 658 */
			uint64_t	f8:6;	/* 652 */
			uint64_t	f9:12;	/* 640 */
		} s;
	} fusedat1;
	union {
		uint64_t	u64;
		struct {
			uint64_t	f1:5;	/* 763-767 */
			uint64_t	f2:1;	/* 762-762 */
			uint64_t	f3:1;	/* 761-761 */
			uint64_t	f4:16;	/* 745-760 */
			uint64_t	f5:5;	/* 740-744 */
			uint64_t	f6:10;	/* 730-739 */
			uint64_t	f7:6;	/* 724-729 */
			uint64_t	f8:10;	/* 714-723 */
			uint64_t	f9:10;	/* 704-713 */
		} s;
	} fusedat2;
	uint64_t temp64;
	int rc = 0;
	uint16_t temp16;

	fusedat1.u64 = 0;
	fusedat2.u64 = 0;

	rc += octeon_get_fuse(node, 640, 703, (uint8_t *)&fusedat1, 0);
	rc += octeon_get_fuse(node, 704, 767, (uint8_t *)&fusedat2, 0);

	fusedat1.u64 = le64_to_cpu(fusedat1.u64);
	fusedat2.u64 = le64_to_cpu(fusedat2.u64);

	debug("%s: fuse data: %016llx %016llx\n", __func__,
	      fusedat1.u64, fusedat2.u64);

	temp64 = ((uint64_t)fusedat2.s.f8 << 54) |
		 ((uint64_t)fusedat1.s.f9 << 42) |
		 ((uint64_t)fusedat1.s.f8 << 36) |
		 ((uint64_t)fusedat1.s.f7 << 30) |
		 ((uint64_t)fusedat1.s.f6 << 24) |
		 ((uint64_t)fusedat1.s.f5 << 18) |
		 ((uint64_t)fusedat1.s.f4 << 12) |
		 ((uint64_t)fusedat1.s.f3 << 6) |
		 ((uint64_t)fusedat1.s.f2 << 0);
	temp16 = ((uint16_t)fusedat2.s.f9 << 6) | fusedat2.s.f7;

	memcpy(&buffer[0], &temp64, sizeof(temp64));
	memcpy(&buffer[8], &temp16, sizeof(temp16));

	/* These fuses are not blown on all Octeon devices */
	if (!temp64 && !temp16) {
		debug("%s(%d): fuses not blown\n", __func__, node);
		return -1;
	}

	return rc ? -1 : 0;
}


/**
 * Generate a random MAC address
 */
void octeon_board_create_random_mac_addr(void)
	__attribute__((weak,alias("__octeon_board_create_random_mac_addr")));

void __octeon_board_create_random_mac_addr(void)
{
#ifndef CONFIG_OCTEON_SIM
	uint8_t fuse_buf[382];
	uint32_t poly = 0x04c11db7;
	uint32_t crc = 0xffffffff;
	uint64_t *ptr;
	int ser_len = strlen((char *)gd->arch.board_desc.serial_str);
	int i;

	memset(fuse_buf, 0, sizeof(fuse_buf));
	fuse_buf[0] = gd->arch.board_desc.board_type;
	fuse_buf[1] = (gd->arch.board_desc.rev_major << 4)
			| gd->arch.board_desc.rev_minor;
	fuse_buf[2] = ser_len;
	strncpy((char*)(fuse_buf+3),
		(char*)gd->arch.board_desc.serial_str,
		sizeof(fuse_buf)-3);

	/* For a random number we perform a CRC32 using the board type,
	 * revision, serial number length, serial number and for OCTEON 2 and 3
	 * the fuse settings.
	 */

	CVMX_MT_CRC_POLYNOMIAL(poly);
	CVMX_ES32(crc, crc);
	CVMX_MT_CRC_IV_REFLECT(crc);

	/* Calculate CRC of board type, revision and serial number */
	for (i = 0; i < 3; i++)
		CVMX_MT_CRC_BYTE_REFLECT(fuse_buf[i]);
	for (; i < ser_len + 3; i++)
		CVMX_MT_CRC_BYTE_REFLECT(fuse_buf[i]);

	if (!OCTEON_IS_OCTEON1PLUS()) {
		/* Add fuses to the mix if available */
		debug("%s: Fuses 0..%d\n", __func__, sizeof(fuse_buf) - 1);
		for (i = 0; i < sizeof(fuse_buf); i++)
			fuse_buf[i] = octeon_get_fuse_addr(0, i);
#ifdef DEBUG
		print_buffer(0, fuse_buf, 1, sizeof(fuse_buf), 0);
#endif
		ptr = (uint64_t *)fuse_buf;
		for (i = 0; i < sizeof(fuse_buf); i += 8)
			CVMX_MT_CRC_DWORD_REFLECT(*ptr++);
	}

	/* Get the final CRC32 */
	CVMX_MF_CRC_IV_REFLECT(crc);
	crc ^= 0xffffffff;

	gd->arch.mac_desc.count = 255;
	gd->arch.mac_desc.mac_addr_base[0] = 0x02;	/* locally administered */
	gd->arch.mac_desc.mac_addr_base[1] = crc & 0xff;
	gd->arch.mac_desc.mac_addr_base[2] = (crc >> 8) & 0xff;
	gd->arch.mac_desc.mac_addr_base[3] = (crc >> 16) & 0xff;
	gd->arch.mac_desc.mac_addr_base[4] = (crc >> 24) & 0xff;
	gd->arch.mac_desc.mac_addr_base[5] = 0;
	debug("Created random MAC address %pM", gd->arch.mac_desc.mac_addr_base);
#else
	gd->arch.mac_desc.mac_addr_base[0] = 2;
	gd->arch.mac_desc.mac_addr_base[1] = 0x00;
	gd->arch.mac_desc.mac_addr_base[2] = 0xDE;
	gd->arch.mac_desc.mac_addr_base[3] = 0xAD;
	gd->arch.mac_desc.mac_addr_base[4] = 0xBF;
	gd->arch.mac_desc.mac_addr_base[5] = 0x00;
	gd->arch.mac_desc.count = 255;
#endif
}

/**
 * Gets the MAC address for a board from the TLV EEPROM.  If not present a
 * random MAC address is generated.
 */
void octeon_board_get_mac_addr(void)
	__attribute__((weak, alias("__octeon_board_get_mac_addr")));

void __octeon_board_get_mac_addr(void)
{
#if USE_EEPROM
	uint16_t eeprom_buf16[OCTEON_EEPROM_MAX_TUPLE_LENGTH / 2 + 1];
	uint8_t *eeprom_buf = (void *)eeprom_buf16;
	uint8_t net_our_ether[6];
	uint16_t addr = 0, found =0;
	int len;
	octeon_eeprom_header_t *tlv_hdr_ptr = (void *)eeprom_buf;
	octeon_eeprom_mac_addr_t *ma_ptr = (void *)eeprom_buf;

#ifdef   CONFIG_EEPROM_TLV_BASE_ADDRESS
	addr = CONFIG_EEPROM_TLV_BASE_ADDRESS;
	printf("CONFIG_EEPROM_TLV_BASE_ADDRESS: 0x%x\n",
		CONFIG_EEPROM_TLV_BASE_ADDRESS);
#endif

	while ((len = octeon_tlv_eeprom_get_next_tuple(CONFIG_SYS_DEF_EEPROM_ADDR,
		addr, eeprom_buf, OCTEON_EEPROM_MAX_TUPLE_LENGTH)) > 0)
	{
		if (tlv_hdr_ptr->type == EEPROM_MAC_ADDR_TYPE) {
			memcpy(net_our_ether, ma_ptr->mac_addr_base, 6);
			printf("TLV MAC address: %02X:%02X:%02X:%02X:%02X:%02X  addr: %d\n",
				net_our_ether[0], net_our_ether[1], net_our_ether[2],
				net_our_ether[3], net_our_ether[4], net_our_ether[5], addr);
			memcpy((void *)&(gd->arch.mac_desc), eeprom_buf,
					sizeof(octeon_eeprom_mac_addr_t));
			found = 1;
			break;
		}
		addr += len;
	}
	if (found == 0)
#endif
		octeon_board_create_random_mac_addr();
}

/**
 * Gets the board clock info from the TLV EEPROM or uses the default value
 * if not available.
 *
 * @param def_ddr_clock_mhz	Default DDR clock speed in MHz
 */
void octeon_board_get_clock_info(uint32_t def_ddr_clock_mhz)
	__attribute__((weak, alias("__octeon_board_get_clock_info")));

void __octeon_board_get_clock_info(uint32_t def_ddr_clock_mhz)
{
	uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH] __attribute__((unused));
	int addr __attribute__((unused));
	/* Assume no descriptor is present */
	gd->mem_clk = def_ddr_clock_mhz;

	debug("%s(%u)\n", __func__, def_ddr_clock_mhz);
	/* Initialize DDR reference frequency if not already set. */
	if (!gd->arch.ddr_ref_hertz)
		gd->arch.ddr_ref_hertz = 50000000;

#if USE_EEPROM
	/* OCTEON I and OCTEON Plus use the old clock descriptor of which
	 * there are two versions.  OCTEON II uses a dedicated DDR clock
	 * descriptor instead.
	 */
	if (OCTEON_IS_OCTEON1PLUS()) {
		octeon_eeprom_header_t *header;
		struct octeon_eeprom_clock_desc_v1 *clock_v1;
		struct octeon_eeprom_clock_desc_v2 *clock_v2;

		addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
						 EEPROM_CLOCK_DESC_TYPE, 0,
						 ee_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
		if (addr < 0)
			return;

		header = (octeon_eeprom_header_t *)ee_buf;
		switch (header->major_version) {
		case 1:
			clock_v1 = (struct octeon_eeprom_clock_desc_v1 *)ee_buf;
			gd->mem_clk = clock_v1->ddr_clock_mhz;
			break;
		case 2:
			clock_v2 = (struct octeon_eeprom_clock_desc_v2 *)ee_buf;
			gd->mem_clk = clock_v2->ddr_clock_mhz;
			break;
		default:
			printf("Unknown TLV clock header version %d.%d\n",
			       header->major_version, header->minor_version);
		}
	} else {
		octeon_eeprom_ddr_clock_desc_t *ddr_clock_ptr;
		addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
						 EEPROM_DDR_CLOCK_DESC_TYPE, 0,
						 ee_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
		if (addr < 0)
			return;


		ddr_clock_ptr = (octeon_eeprom_ddr_clock_desc_t *)ee_buf;
		gd->mem_clk = ddr_clock_ptr->ddr_clock_hz / 1000000;
	}
	if (gd->mem_clk < 100 || gd->mem_clk > 2000)
		gd->mem_clk = def_ddr_clock_mhz;
#endif
}

/**
 * Reads the board descriptor from the TLV EEPROM or fills in the default
 * values.
 *
 * @param type		board type
 * @param rev_major	board major revision
 * @param rev_minor	board minor revision
 */
void octeon_board_get_descriptor(enum cvmx_board_types_enum type,
				 int rev_major, int rev_minor)
	__attribute__((weak, alias("__octeon_board_get_descriptor")));

void __octeon_board_get_descriptor(enum cvmx_board_types_enum type,
				   int rev_major, int rev_minor)
{
	struct bootloader_header *header;

	gd->arch.board_desc.board_type = CVMX_BOARD_TYPE_NULL;
	debug("%s(%s, %d, %d)\n", __func__, cvmx_board_type_to_string(type),
	      rev_major, rev_minor);

#if defined(OCTEON_GPIO_FAILSAFE_BIT) && !defined(CONFIG_OCTEON_NO_FAILSAFE)
	gpio_direction_input(OCTEON_GPIO_FAILSAFE_BIT);
	if (gpio_get_value(OCTEON_GPIO_FAILSAFE_BIT)) {
		debug("Using passed values for board descriptor\n");
		gd->arch.board_desc.board_type = type;
		gd->arch.board_desc.rev_major = rev_major;
		gd->arch.board_desc.rev_minor = rev_minor;
		return 0;
	}
#endif
	if (type == CVMX_BOARD_TYPE_NULL) {
		debug("NULL board type passed, extracting board type from header\n");
		header = CASTPTR(struct bootloader_header, CONFIG_SYS_TEXT_BASE);
		if (validate_bootloader_header(header)) {
			gd->flags |= GD_FLG_BOARD_DESC_MISSING;
			gd->arch.board_desc.rev_major = header->maj_rev;
			gd->arch.board_desc.rev_minor = header->min_rev;
			gd->arch.board_desc.board_type = header->board_type;
			strncpy((char *)(gd->arch.board_desc.serial_str),
				"unknown", SERIAL_LEN);
			debug("header board type: %s, revision %d.%d\n",
			      cvmx_board_type_to_string(header->board_type),
			      header->maj_rev, header->min_rev);
		}
	}
#if USE_EEPROM
	{
		uint8_t ee_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
		int addr;
		octeon_eeprom_ddr_verbose_t ddr_verbose;

		/* Determine board type/rev */
		strncpy((char *)(gd->arch.board_desc.serial_str), "unknown",
			SERIAL_LEN);
		addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
						 EEPROM_BOARD_DESC_TYPE, 0,
						 ee_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
		if (addr >= 0) {
			memcpy((void *)&(gd->arch.board_desc), ee_buf,
			       sizeof(octeon_eeprom_board_desc_t));
			gd->board_type = gd->arch.board_desc.board_type;
			gd->flags &= ~GD_FLG_BOARD_DESC_MISSING;
		}

		/* Get the DDR verbose flag while we're here. */
		addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
						 EEPROM_DDR_VERBOSE_TYPE, 0,
						 ee_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
		if (addr >= 0) {
			memcpy((void *)&ddr_verbose, ee_buf, sizeof(ddr_verbose));
			if (ddr_verbose.verbosity & 1)
				gd->flags |= GD_FLG_DDR_VERBOSE;
			if (ddr_verbose.verbosity & 2)
				gd->flags |= GD_FLG_DDR_TRACE_INIT;
			if (ddr_verbose.verbosity & 4)
				gd->flags |= GD_FLG_DDR_DEBUG;
			debug("%s: DDR Verbosity flags: 0x%x\n", __func__,
			      ddr_verbose.verbosity);
		} else {
			debug("%s: DDR Verbosity TLV not set\n", __func__);
		}
	}
#endif
	if (gd->arch.board_desc.board_type == CVMX_BOARD_TYPE_NULL ||
	    gd->arch.board_desc.board_type != type) {
		printf("Setting board type to passed-in type %s\n",
		      cvmx_board_type_to_string(type));
		gd->flags |= GD_FLG_BOARD_DESC_MISSING;
		gd->arch.board_desc.rev_major = rev_major;
		gd->arch.board_desc.rev_minor = rev_minor;
		gd->board_type = gd->arch.board_desc.board_type = type;
	}
}


/**
 * Function to write string to LED display
 * @param str - string up to 8 characters to write.
 */
void octeon_led_str_write(const char *str)
	__attribute__((weak, alias("__octeon_led_str_write")));

void __octeon_led_str_write(const char *str)
{
#ifdef CONFIG_OCTEON_ENABLE_LED_DISPLAY
	octeon_led_str_write_std(str);
#endif
}

void __board_mdio_init(void)
{
#ifdef CONFIG_OF_LIBFDT
	octeon_board_phy_init();
#endif
}

void board_mdio_init(void) __attribute__((weak, alias("__board_mdio_init")));

#if defined(CONFIG_OCTEON_NIC73)        || \
        defined(CONFIG_OCTEON_NIC23)    || \
        defined(CONFIG_OCTEON_NIC225E)  || \
        defined(CONFIG_OCTEON_COPPERHEAD)

static uint32_t pci_cfgspace_reg0[2] = { 0, 0 };

/**
 * Saves the PCI ID as read from the EEPROM for use by octeon_board_restore_pf()
 *
 * @return	0 on success, otherwise at least one of the PFs is progammed to
 *		the default as encoded by the Octeon chip (i.e. 0x177d9703).
 *
 * NOTE:	This function should be called from early_board_init_r();
 */
int octeon_board_save_pcie_id_override(void)
{
	int pf_num;
	int rc;
	int ret = 0;

	if (!OCTEON_IS_MODEL(OCTEON_CN73XX)) {
		printf("%s: Error: only Octeon 3 is supported\n", __func__);
		return -1;
	}

	for (pf_num = 0; pf_num < 2; pf_num++) {
		rc = octeon_find_pcie_id_override(0, pf_num,
						   &pci_cfgspace_reg0[pf_num]);
		if (rc) {
			pci_cfgspace_reg0[pf_num] =
				0x177d | ((cvmx_get_proc_id() & 0xffff) << 16);
			debug("%s: Hard coding pf %d ID to 0x%08x\n",
			      __func__, pf_num, pci_cfgspace_reg0[pf_num]);
		} else {
			debug("%s: Setting pf %d ID to 0x%08x from EEPROM\n",
			      __func__, pf_num, pci_cfgspace_reg0[pf_num]);
		}
		ret |= rc;
	}
	return ret;
}

#define DTX_SELECT_LTSSM     0x0
#define DTX_SELECT_LTSSM_ENA 0x3ff
#define LTSSM_L0             0x11

/**
 * If there is a PF FLR then the PCI EEPROM is not re-read.  In this case
 * we need to re-program the vendor and device ID immediately after hardware
 * completes FLR.
 *
 * PCI spec requires FLR to be completed within 100ms.  The user who triggered
 * FLR expects hardware to finish FLR within 100ms, otherwise the user will
 * end up reading DEVICE_ID incorrectly from the reset value.
 * CN23XX exits FLR at any point between 66 and 99ms, so U-Boot has to wait
 * 99ms to let hardware finish its part, then finish reprogramming the
 * correct device ID before the end of 100ms.
 *
 * Note: this solution only works properly when there is no other activity
 * within U-Boot for 100ms from the time FLR is triggered.
 *
 * This function gets called every 100usec.  If FLR happens during any
 * other activity like bootloader/image update then it is possible that
 * this function does not get called for more than the FLR period which will
 * cause the PF device ID restore to happen after whoever initiated the FLR to
 * read the incorrect device ID 0x9700 (reset value) instead of 0x9702
 * (restored value).
 */
void octeon_board_restore_pf(void)
{
	union cvmx_spemx_flr_pf_stopreq stopreq;
	int pf_num;
	static bool start_initialized[2] = {false, false};
	bool pf0_flag, pf1_flag;
	uint64_t ltssm_bits;
	const uint64_t pf_flr_wait_usecs = 99700;
	uint64_t cur_cycles, cycles_per_usec, elapsed_usecs;
	union cvmx_spemx_cfg_wr cfg_wr;
	union cvmx_spemx_cfg_rd cfg_rd;
	static uint64_t start_cycles[2];

	cvmx_write_csr(CVMX_DTX_SPEM_SELX(0), DTX_SELECT_LTSSM);
	cvmx_read_csr(CVMX_DTX_SPEM_SELX(0));
	cvmx_write_csr(CVMX_DTX_SPEM_ENAX(0), DTX_SELECT_LTSSM_ENA);
	cvmx_read_csr(CVMX_DTX_SPEM_ENAX(0));
	ltssm_bits = cvmx_read_csr(CVMX_DTX_SPEM_DATX(0));
	if (((ltssm_bits >> 3) & 0x3f) != LTSSM_L0)
		return;

	stopreq.u64 = cvmx_read_csr(CVMX_SPEMX_FLR_PF_STOPREQ(0));
	pf0_flag = stopreq.s.pf0_stopreq;
	pf1_flag = stopreq.s.pf1_stopreq;
	/* See if PF interrupt happened */
	if (!(pf0_flag || pf1_flag))
		return;

	if (pf0_flag && !start_initialized[0]) {
		start_initialized[0] = true;
		start_cycles[0] = cvmx_clock_get_count(CVMX_CLOCK_CORE);
	}

	if (pf1_flag && !start_initialized[1]) {
		start_initialized[1] = true;
		start_cycles[1] = cvmx_clock_get_count(CVMX_CLOCK_CORE);
	}

	/* For PF, rewrite pci config space reg 0 */
	for (pf_num = 0; pf_num < 2; pf_num++) {

		if (!start_initialized[pf_num])
			continue;

		cur_cycles = cvmx_clock_get_count(CVMX_CLOCK_CORE);
		cycles_per_usec = cvmx_clock_get_rate(CVMX_CLOCK_CORE) / 1000000;
		elapsed_usecs = (cur_cycles - start_cycles[pf_num]) / cycles_per_usec;

		if (elapsed_usecs > pf_flr_wait_usecs) {
			/* Here, our measured FLR duration has passed;
			 * check if device ID has been reset,
			 * which indicates FLR completion (per MA team).
			 */
			cfg_rd.s.addr = (pf_num << 24) | 0x0;                    
			cvmx_write_csr(CVMX_SPEMX_CFG_RD(0), cfg_rd.u64);
			cfg_rd.u64 = cvmx_read_csr(CVMX_SPEMX_CFG_RD(0));                                              
			/* if DevID has NOT been reset, FLR is not yet complete */
			if (cfg_rd.s.data != pci_cfgspace_reg0[pf_num]) {

				stopreq.s.pf0_stopreq = (pf_num == 0) ? 1 : 0;
				stopreq.s.pf1_stopreq = (pf_num == 1) ? 1 : 0;
				cvmx_write_csr(CVMX_SPEMX_FLR_PF_STOPREQ(0), stopreq.u64);

				cfg_wr.u64 = 0;
				cfg_wr.s.addr = (pf_num << 24) | 0;
				cfg_wr.s.data = pci_cfgspace_reg0[pf_num];
				cvmx_write_csr(CVMX_SPEMX_CFG_WR(0), cfg_wr.u64);
				start_initialized[pf_num] = false;
			}
		}
	}
}
#endif
