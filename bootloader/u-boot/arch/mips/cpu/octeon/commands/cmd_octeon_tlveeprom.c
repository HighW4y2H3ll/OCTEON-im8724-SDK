/***********************license start************************************
 * Copyright (c) 2010 - 2015 Cavium Inc. (support@cavium.com).
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

#include <command.h>
#include <common.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/octeon-model.h>
#include <asm/arch/octeon_eeprom_types.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/lib_octeon_shared.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_CMD_EEPROM) || defined(CONFIG_ENV_IS_IN_EEPROM)

extern void eeprom_init  (void);
extern int  eeprom_read  (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);
extern int  eeprom_write (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);
#endif

static const char *module_protocols[] = {
	"PCIe Host 4X",			/* 0x001 */
	"PCIe Endpoint 4x",		/* 0x002 */
	"PCIe Host 8x Lower",		/* 0x004 */
	"PCIe Host 8x Upper",		/* 0x008 */
	"PCIe Endpoint 8x Lower",	/* 0x010 */
	"PCIe Endpoint 8x Upper",	/* 0x020 */
	"SGMII",			/* 0x040 */
	"XAUI",				/* 0x080 */
	"SRIO",				/* 0x100 */
	"Interlaken",			/* 0x200 */
	"RXAUI",			/* 0x400 */
	"\0"
};

static const char *module_speeds[] = {
	"1.25G",	/* 0x01 */
	"2.5G",		/* 0x02 */
	"3.125G",	/* 0x04 */
	"5.0G",		/* 0x08 */
	"6.25G",	/* 0x10 */
	"3.75G",	/* 0x20 */
	"7.5G",		/* 0x40 */
	"\0"
};

static const char *module_flags[] = {
	"Reverse Lanes",		/* bit 0 */
	"Use Common Reference Clock",	/* bit 1 */
	"SRIO End Point Mode",		/* bit 2 */
	"Board End Point Mode",		/* bit 3 */
	"Unknown",			/* bits 4-13 */
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Invalid Configuration",	/* bit 14 */
	"Ignore TLV",			/* bit 15 */
	"\0"
};

int write_tlv_tuple(octeon_eeprom_header_t * tlv_hdr_ptr)
{

	uint16_t type = tlv_hdr_ptr->type;
	uint8_t major_version = tlv_hdr_ptr->major_version;
	uint16_t checksum = 0;
	int i;
	uint8_t *eeprom_buf = (void *)tlv_hdr_ptr;
	uint8_t tmp_buf[OCTEON_EEPROM_MAX_TUPLE_LENGTH];
	int addr;


	tlv_hdr_ptr->checksum = 0;
	for (i = 0; i < tlv_hdr_ptr->length; i++)
		checksum += eeprom_buf[i];

	tlv_hdr_ptr->checksum = checksum;

	addr = octeon_tlv_get_tuple_addr(CONFIG_SYS_DEF_EEPROM_ADDR,
					 type, major_version,
					 tmp_buf,
					 OCTEON_EEPROM_MAX_TUPLE_LENGTH);
	/* Mask addr since it might be negative if no existing tuple found */
	addr &= 0xffff;
	printf("Writing tuple type %d to addr: 0x%x\n", type, addr);

#if 0
	for (i = 0; i < tlv_hdr_ptr->length; i++) {
		printf("cd[0x%02x]:0x%02x\n", i, eeprom_buf[i]);
	}
#endif

	if (eeprom_write(CONFIG_SYS_DEF_EEPROM_ADDR, addr, (uchar *) eeprom_buf,
	     tlv_hdr_ptr->length)) {
		printf("Error writing tuple type %d to addr: 0x%x\n", type,
		       addr);
		return (-1);
	}
	return 0;
}

/* Deletes a tuple at a given address in the eeprom */
int delete_tuple(int delete_addr)
{

	/* only used for one structure at a time */
	uint16_t eeprom_buf16[OCTEON_EEPROM_MAX_TUPLE_LENGTH / 2 + 1];
	uint8_t *eeprom_buf = (void *)eeprom_buf16;
	octeon_eeprom_header_t *tlv_hdr_ptr = (void *)eeprom_buf;
	int write_addr;
	int read_addr;
	int cur_len;
	int delete_len =
	    octeon_tlv_eeprom_get_next_tuple(CONFIG_SYS_DEF_EEPROM_ADDR,
					     delete_addr,
					     eeprom_buf,
					     OCTEON_EEPROM_MAX_TUPLE_LENGTH);

	if (delete_len < 0)
		return -1;

	printf("Deleting %s (0x%x) tuple at address 0x%x, length %d\n",
	       eeprom_type_to_string(tlv_hdr_ptr->type), tlv_hdr_ptr->type,
	       delete_addr, delete_len);
	/* Erase the tuple that is to be deleted */
	memset(eeprom_buf, 0xff, delete_len);
	eeprom_write(CONFIG_SYS_DEF_EEPROM_ADDR, delete_addr, (uchar *) eeprom_buf,
		     delete_len);

	/* Now copy any tuples after the deleted one to keep the tuples packet
	 * as this is required
	 */
	write_addr = delete_addr;
	read_addr = delete_addr + delete_len;

	while ((cur_len =
		octeon_tlv_eeprom_get_next_tuple(CONFIG_SYS_DEF_EEPROM_ADDR,
						 read_addr,
						 eeprom_buf,
						 OCTEON_EEPROM_MAX_TUPLE_LENGTH))
	       > 0) {
		/* We have read the next tuple we want to keep into the
		 * eeprom_buf, so now write it over the tuple we want to delete
		 */
		printf("Copying tuple type %d (len 0x%x)to addr: 0x%x\n",
		       tlv_hdr_ptr->type, tlv_hdr_ptr->length, write_addr);

		if (eeprom_write(CONFIG_SYS_DEF_EEPROM_ADDR, write_addr,
				 (uchar *) eeprom_buf,
				 tlv_hdr_ptr->length)) {
			printf("Error writing tuple type %d to addr: 0x%x\n",
			       tlv_hdr_ptr->type, write_addr);
			return -1;
		}
		if (read_addr >= write_addr + cur_len) {
			/* Erase TLV header what we just copied if the copy did
			 * not overwrite it.
			 */
			memset(eeprom_buf, 0xff, 8);
			eeprom_write(CONFIG_SYS_DEF_EEPROM_ADDR, read_addr,
				     (uchar *) eeprom_buf, 8);
		}
		read_addr += cur_len;
		write_addr += cur_len;
	}

	return 0;
}

static void
print_module_capabilities(
	const struct octeon_eeprom_default_module_capability_v1 *mod_cap_v1)
{
	const char *protocol, *speed;
	uint16_t flags = mod_cap_v1->cfg_flags;
	int i;
	int first = 1;

	if ((__builtin_popcount(mod_cap_v1->protocol) != 1) ||
	    mod_cap_v1->protocol > 0x400)
		protocol = "Unknown";
	else
		protocol = module_protocols[__builtin_ffs(mod_cap_v1->protocol)-1];
	if ((__builtin_popcount(mod_cap_v1->speed) != 1) ||
	    mod_cap_v1->speed > 0x400)
		 speed = "Unknown";
	else
		speed = module_speeds[__builtin_ffs(mod_cap_v1->speed)-1];

	printf("QLM: %d, protocol: %s (0x%04x), speed: %s (0x%04x)\ncfg flags: ",
	       mod_cap_v1->qlm_number, protocol, mod_cap_v1->protocol,
	       speed, mod_cap_v1->speed);

	for (i = 0; i < 16; i++) {
		if ((mod_cap_v1->cfg_flags >> i) & 1) {
			if (!first)
				puts(", ");
			else
				first = 0;
			puts(module_flags[i]);
		}
	}
	printf(" (0x%04x)\n", flags);
	printf("raw config: 0x%04x, raw speed: 0x%04x, raw fsel: 0x%04x\n",
	       mod_cap_v1->raw_cfg, mod_cap_v1->raw_speed, mod_cap_v1->raw_fsel);
}


#define MIN_TLV_COMMAND_CHARS   3
int do_tlv_eeprom(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{

	/* only used for one structure at a time */
	uint16_t eeprom_buf16[OCTEON_EEPROM_MAX_TUPLE_LENGTH / 2 + 1];
	uint8_t *eeprom_buf = (void *)eeprom_buf16;

	/* Provide a number of different pointer types to access the eeprom
	 * buffer. There is only one user of the buffer at a time.
	 */
	octeon_eeprom_header_t *tlv_hdr_ptr = (void *)eeprom_buf;
	octeon_eeprom_clock_desc_t *cd_ptr = (void *)eeprom_buf;
	octeon_eeprom_board_desc_t *bd_ptr = (void *)eeprom_buf;
	octeon_eeprom_chip_capability_t *cc_ptr = (void *)eeprom_buf;
	octeon_eeprom_mac_addr_t *ma_ptr = (void *)eeprom_buf;
	octeon_eeprom_voltage_multiplier_t *vm_ptr = (void *)eeprom_buf;
	octeon_eeprom_multiplier_desc_t *mult_ptr = (void *)eeprom_buf;
	octeon_eeprom_voltage_desc_t *volt_ptr = (void *)eeprom_buf;
	octeon_eeprom_ddr_clock_desc_t *ddr_clock_ptr = (void *)eeprom_buf;
	octeon_eeprom_ddr_verbose_t *ddr_verbose_ptr = (void *)eeprom_buf;
	uint16_t addr = 0;
	int len;

#ifdef   CONFIG_EEPROM_TLV_BASE_ADDRESS
	addr = CONFIG_EEPROM_TLV_BASE_ADDRESS;
	printf("CONFIG_EEPROM_TLV_BASE_ADDRESS: 0x%x\n",
	       CONFIG_EEPROM_TLV_BASE_ADDRESS);
#endif

	eeprom_init();

	if (argc < 2 || (argc == 2 && !strcmp(argv[1], "help")) ||
	    strlen(argv[1]) < MIN_TLV_COMMAND_CHARS) {
		int i;

		cmd_usage(cmdtp);
		/* Print out board type table here */
		printf("Board types: \n");
		for (i = 1; i < CVMX_BOARD_TYPE_MAX; i++) {
			printf("%20s\t", cvmx_board_type_to_string(i));
			if ((i - 1) % 3 == 2)
				printf("\n");
		}
		printf("\n");
		return 1;
	}
	if (strncmp(argv[1], "delete", strlen(argv[1])) == 0) {
		int delete_addr = simple_strtoul(argv[2], NULL, 0);

		if (delete_tuple(delete_addr) < 0) {
			printf("Error deleting tuple (address %d (0x%x).\n",
			       delete_addr, delete_addr);
			return -1;
		}
	} else if ((strncmp(argv[1], "display", strlen(argv[1])) == 0) ||
	           (strncmp(argv[1], "print", strlen(argv[1])) == 0)) {

		while ((len =
			octeon_tlv_eeprom_get_next_tuple(CONFIG_SYS_DEF_EEPROM_ADDR,
							 addr, eeprom_buf,
							 OCTEON_EEPROM_MAX_TUPLE_LENGTH))
		       > 0) {

			printf("==========================================\n");
			printf("%s (0x%x) tuple found: at addr 0x%x\n",
			       eeprom_type_to_string(tlv_hdr_ptr->type),
			       tlv_hdr_ptr->type, addr);
			printf("type: 0x%x, len: 0x%x, csum: 0x%x, maj_ver: %d, min_ver: %d\n",
			       tlv_hdr_ptr->type, tlv_hdr_ptr->length,
			       tlv_hdr_ptr->checksum, tlv_hdr_ptr->major_version,
			       tlv_hdr_ptr->minor_version);
			switch (tlv_hdr_ptr->type) {

			case EEPROM_CLOCK_DESC_TYPE:
				if (tlv_hdr_ptr->major_version >
				    OCTEON_EEPROM_CLOCK_DESC_MAJOR_VER) {
					printf
					    ("WARNING: tuple version mismatch, "
					     "display may be wrong. (compiled "
					     "for version: %d)\n",
					     OCTEON_EEPROM_CLOCK_DESC_MAJOR_VER);
				}

				if (tlv_hdr_ptr->major_version == 1) {
					struct octeon_eeprom_clock_desc_v1
					    clock_v1;
					memcpy(&clock_v1, tlv_hdr_ptr,
					       sizeof(clock_v1));
					printf
					    ("DDR clock: %d Mhz (raw: 0x%x)\n",
					     clock_v1.ddr_clock_mhz,
					     clock_v1.ddr_clock_mhz);
					printf
					    ("CPU ref clock: %d Mhz (raw: 0x%x)\n",
					     clock_v1.
					     cpu_ref_clock_mhz_x_8 >> 3,
					     clock_v1.cpu_ref_clock_mhz_x_8);
					break;
				}

				printf("DDR clock: %d Mhz (raw: 0x%x)\n",
				       cd_ptr->ddr_clock_mhz,
				       cd_ptr->ddr_clock_mhz);
				printf("CPU ref clock: %d Mhz (raw: 0x%x)\n",
				       cd_ptr->cpu_ref_clock_mhz_x_8 >> 3,
				       cd_ptr->cpu_ref_clock_mhz_x_8);
				printf("DFA ref clock: %d Mhz (raw: 0x%x)\n",
				       cd_ptr->dfa_ref_clock_mhz_x_8 >> 3,
				       cd_ptr->dfa_ref_clock_mhz_x_8);
				break;

			case EEPROM_BOARD_DESC_TYPE:
				if (tlv_hdr_ptr->major_version !=
				    OCTEON_EEPROM_BOARD_DESC_MAJOR_VER) {
					printf
					    ("WARNING: tuple version mismatch, "
					     "display may be wrong. (compiled "
					     "for version: %d)\n",
					     OCTEON_EEPROM_BOARD_DESC_MAJOR_VER);
				}

				printf("Board type: %s (0x%x) \n",
				       cvmx_board_type_to_string(bd_ptr->
								 board_type),
				       bd_ptr->board_type);
				printf("Board revision major:%d, minor:%d\n",
				       bd_ptr->rev_major, bd_ptr->rev_minor);
				bd_ptr->serial_str[OCTEON_SERIAL_LEN - 1] = 0;
				printf("Board ser #: %s\n", bd_ptr->serial_str);

				break;

			case EEPROM_CHIP_CAPABILITY_TYPE:
				printf
				    ("Coremask: 0x%04x, voltage_x100: %d, "
				     "cpu_freq_mhz: %d\n",
				     cc_ptr->coremask, cc_ptr->voltage_x100,
				     cc_ptr->cpu_freq_mhz);

				break;
			case EEPROM_MAC_ADDR_TYPE:
				printf
				    ("MAC base: %02x:%02x:%02x:%02x:%02x:%02x, "
				     "count: %d\n",
				     ma_ptr->mac_addr_base[0],
				     ma_ptr->mac_addr_base[1],
				     ma_ptr->mac_addr_base[2],
				     ma_ptr->mac_addr_base[3],
				     ma_ptr->mac_addr_base[4],
				     ma_ptr->mac_addr_base[5], ma_ptr->count);
				break;
			case EEPROM_VOLT_MULT_TYPE:
				printf("Voltage: %d millivolts\n",
				       vm_ptr->voltage_millivolts);
				printf("CPU multiplier: %d\n",
				       vm_ptr->cpu_multiplier);
				break;
			case EEPROM_DDR_CLOCK_DESC_TYPE:
				printf("DDR ref Hz: %d, clock Hz: %d\n",
				       ddr_clock_ptr->ddr_ref_hz,
				       ddr_clock_ptr->ddr_clock_hz);
				break;
			case EEPROM_VOLTAGE_TYPE:
				printf("CPU voltage: %d Millivolts\n",
				       volt_ptr->octeon_core_voltage_mv);
				break;

			case EEPROM_MULTIPLIER_TYPE:
				printf("CPU Multiplier: %d (0x%x), IO "
				     "Multiplier: %d (0x%x)\n",
				     mult_ptr->octeon_core_mult,
				     mult_ptr->octeon_core_mult,
				     mult_ptr->octeon_io_mult,
				     mult_ptr->octeon_io_mult);
				break;
			case EEPROM_DEFAULT_MODULE_CAPABILITY_TYPE:
				if (tlv_hdr_ptr->major_version == 1) {
					struct octeon_eeprom_default_module_capability_v1 mod_cap_v1;
					memcpy(&mod_cap_v1, tlv_hdr_ptr,
					       sizeof(mod_cap_v1));
					print_module_capabilities(&mod_cap_v1);
				}
				break;
			case EEPROM_DELETE_ME:

				break;

			case EEPROM_DDR_VERBOSE_TYPE:
				if (tlv_hdr_ptr->major_version == 1) {
					printf("DDR Verbosity bitmap: 0x%x\n",
					       ddr_verbose_ptr->verbosity);
					if (ddr_verbose_ptr->verbosity & 1)
						puts("  DDR_VERBOSE\n");
					if (ddr_verbose_ptr->verbosity & 2)
						puts("  DDR_TRACE_INIT\n");
					if (ddr_verbose_ptr->verbosity & 4)
						puts("  DDR_DEBUG\n");
				}
				break;
			default:
				/* Default case handled outside of switch statement */
				break;

			}

			addr += len;
		}
		if (addr)
			printf("==========================================\n");

	} else if ((strncmp(argv[1], "set", strlen(argv[1])) == 0)
		   && strlen(argv[2]) >= MIN_TLV_COMMAND_CHARS && argc > 3) {
		memset(eeprom_buf, 0, OCTEON_EEPROM_MAX_TUPLE_LENGTH);

		if (strncmp(argv[2], "clock", strlen(argv[2])) == 0
		    && argc == (3 + 4)) {
			if (!(OCTEON_IS_OCTEON1() || OCTEON_IS_OCTEON1PLUS())) {
				printf
				    ("ERROR: this tuple type is only supported on CN3XXX/CN5XXX\n");
				printf
				    ("       use the ddrclock tuple to set ddr frequency.\n");
				return (1);
			}

			printf("Setting clock descriptor...\n");
			tlv_hdr_ptr->type = EEPROM_CLOCK_DESC_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_clock_desc_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_CLOCK_DESC_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_CLOCK_DESC_MINOR_VER;

			cd_ptr->ddr_clock_mhz =
			    simple_strtoul(argv[3], NULL, 0);
			cd_ptr->cpu_ref_clock_mhz_x_8 =
			    simple_strtoul(argv[4], NULL, 0);
			cd_ptr->dfa_ref_clock_mhz_x_8 =
			    simple_strtoul(argv[5], NULL, 0);
			cd_ptr->spi_clock_mhz =
			    simple_strtoul(argv[6], NULL, 0);

			printf("ddr: %d, cpu ref: %d, dfa ref: %d spi: %d\n",
			       cd_ptr->ddr_clock_mhz,
			       cd_ptr->cpu_ref_clock_mhz_x_8,
			       cd_ptr->dfa_ref_clock_mhz_x_8,
			       cd_ptr->spi_clock_mhz);

			if ((gd->arch.board_desc.board_type ==
			     CVMX_BOARD_TYPE_EBT3000 && gd->arch.mcu_rev_maj <= 2
			     && gd->arch.mcu_rev_min <= 11)
			    || (gd->arch.board_desc.board_type ==
				CVMX_BOARD_TYPE_EBH3000 && gd->arch.mcu_rev_maj < 1
				&& gd->arch.mcu_rev_min < 15)) {

				struct octeon_eeprom_clock_desc_v1 clock_v1;

				/* MCU rev 2.11 and lower only understand rev 1 clock descriptors. */
				puts("Using version 1 clock descriptor for ebt3000 board\n");
				clock_v1.header.type = EEPROM_CLOCK_DESC_TYPE;
				clock_v1.header.major_version = 1;
				clock_v1.header.minor_version = 0;
				clock_v1.header.length = sizeof(clock_v1);
				clock_v1.ddr_clock_mhz = cd_ptr->ddr_clock_mhz;
				clock_v1.cpu_ref_clock_mhz_x_8 =
				    cd_ptr->cpu_ref_clock_mhz_x_8;
				clock_v1.spi_clock_mhz = cd_ptr->spi_clock_mhz;
				write_tlv_tuple((octeon_eeprom_header_t *) &
						clock_v1);
			} else {
				write_tlv_tuple(tlv_hdr_ptr);
			}

		} else if (strncmp(argv[2], "board", strlen(argv[2])) == 0
			   && argc == (3 + 4)) {
			int i;
			/* We don't have strnicmp so make argument upper case */
			for (i=0; argv[3][i] != '\0'; i++)
				argv[3][i] = toupper(argv[3][i]);

			/* bd_type bd_rv_mj bd_rv_mn chip_type ch_rv_mj ch_rv_mn serial_str */

			printf("Setting board descriptor...\n");
			tlv_hdr_ptr->type = EEPROM_BOARD_DESC_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_board_desc_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_BOARD_DESC_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_BOARD_DESC_MINOR_VER;

			bd_ptr->board_type = 0;
			for (i = 1; i < CVMX_BOARD_TYPE_MAX; i++) {
				if (!strncmp(argv[3],
					     cvmx_board_type_to_string(i),
					     100)) {
					bd_ptr->board_type = i;
					break;
				}
			}
			if (!bd_ptr->board_type) {
				printf("ERROR: unrecognized board name: %s\n",
				       argv[3]);
				return (0);
			}

			bd_ptr->rev_major = simple_strtoul(argv[4], NULL, 0);
			bd_ptr->rev_minor = simple_strtoul(argv[5], NULL, 0);
			memset(bd_ptr->serial_str, 0, OCTEON_SERIAL_LEN);
			strncpy((char *)bd_ptr->serial_str, argv[6],
				OCTEON_SERIAL_LEN);

			printf("Board type: %d, rev: %d.%d, serial: %s\n",
			       bd_ptr->board_type,
			       bd_ptr->rev_major,
			       bd_ptr->rev_minor, bd_ptr->serial_str);

			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "capability", strlen(argv[2])) == 0
			   && argc <= (3 + 3)) {
			printf("Setting chip capability descriptor...\n");
			tlv_hdr_ptr->type = EEPROM_CHIP_CAPABILITY_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_chip_capability_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_CHIP_CAPABILITY_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_CHIP_CAPABILITY_MINOR_VER;

			cc_ptr->coremask = simple_strtoul(argv[3], NULL, 16);

			write_tlv_tuple(tlv_hdr_ptr);

		} else if (strncmp(argv[2], "mac", strlen(argv[2])) == 0
			   && argc == (3 + 2)) {
			int i;
			char *tmp = argv[3];
			char *end;

			printf("Setting mac address descriptor...\n");
			tlv_hdr_ptr->type = EEPROM_MAC_ADDR_TYPE;
			tlv_hdr_ptr->length = sizeof(octeon_eeprom_mac_addr_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_MAC_ADDR_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_MAC_ADDR_MINOR_VER;

			for (i = 0; i < 6; i++) {
				ma_ptr->mac_addr_base[i] =
				    tmp ? simple_strtoul(tmp, &end, 16) : 0;
				if (tmp)
					tmp = (*end) ? end + 1 : end;
			}

			ma_ptr->count = simple_strtoul(argv[4], NULL, 0);

			printf("Setting mac count to: %d\n", ma_ptr->count);
			printf("Setting mac base to: %02x:%02x:%02x:%02x:%02x:%02x\n",
			       ma_ptr->mac_addr_base[0],
			       ma_ptr->mac_addr_base[1],
			       ma_ptr->mac_addr_base[2],
			       ma_ptr->mac_addr_base[3],
			       ma_ptr->mac_addr_base[4],
			       ma_ptr->mac_addr_base[5]);
			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "voltmult", 8) == 0
			   && argc == (3 + 2)) {
			if (!(OCTEON_IS_OCTEON1() || OCTEON_IS_OCTEON1PLUS())) {
				printf
				    ("ERROR: this tuple type is only supported on CN3XXX/CN5XXX\n");
				return (1);
			}
			tlv_hdr_ptr->type = EEPROM_VOLT_MULT_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_voltage_multiplier_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_VOLT_MULT_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_VOLT_MULT_MINOR_VER;

			vm_ptr->voltage_millivolts =
			    simple_strtoul(argv[3], NULL, 0);
			vm_ptr->cpu_multiplier =
			    simple_strtoul(argv[4], NULL, 0);
			printf
			    ("Setting voltage to %d millivolts and cpu multiplier to %d\n",
			     vm_ptr->voltage_millivolts,
			     vm_ptr->cpu_multiplier);
			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "volt", 4) == 0 && argc == (3 + 1)) {
			if (OCTEON_IS_OCTEON1() || OCTEON_IS_OCTEON1PLUS()) {
				printf
				    ("ERROR: this tuple type is not supported on CN3XXX/CN5XXX\n");
				return (1);
			}
			tlv_hdr_ptr->type = EEPROM_VOLTAGE_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_voltage_desc_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_VOLTAGE_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_VOLTAGE_MINOR_VER;

			volt_ptr->octeon_core_voltage_mv =
			    simple_strtoul(argv[3], NULL, 0);
			printf("Setting core voltage to %d millivolts\n",
			       vm_ptr->voltage_millivolts);
			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "mult", 4) == 0 && argc == (3 + 2)) {
			if (OCTEON_IS_OCTEON1() || OCTEON_IS_OCTEON1PLUS()) {
				printf
				    ("ERROR: this tuple type is not supported on CN3XXX/CN5XXX\n");
				return (1);
			}
			tlv_hdr_ptr->type = EEPROM_MULTIPLIER_TYPE;
			tlv_hdr_ptr->length =
			    sizeof(octeon_eeprom_multiplier_desc_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
			    OCTEON_EEPROM_MULTIPLIER_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
			    OCTEON_EEPROM_MULTIPLIER_MINOR_VER;

			mult_ptr->octeon_core_mult =
			    simple_strtoul(argv[3], NULL, 0);
			mult_ptr->octeon_io_mult =
			    simple_strtoul(argv[4], NULL, 0);
			printf("Setting core multiplier to: %d, and IO multiplier to: %d\n",
			       mult_ptr->octeon_core_mult,
			       mult_ptr->octeon_io_mult);

			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "ddrclock", 8) == 0
			   && argc == (3 + 1)) {
			if (OCTEON_IS_OCTEON1() || OCTEON_IS_OCTEON1PLUS()) {
				printf
				    ("ERROR: this tuple type is not supported on CN3XXX/CN5XXX\n");
				return (1);
			}
			tlv_hdr_ptr->type = EEPROM_DDR_CLOCK_DESC_TYPE;
			tlv_hdr_ptr->length =
				sizeof(octeon_eeprom_ddr_clock_desc_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
				OCTEON_EEPROM_DDR_CLOCK_DESC_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
				OCTEON_EEPROM_DDR_CLOCK_DESC_MINOR_VER;

			ddr_clock_ptr->ddr_ref_hz = 0;	/* Use default */
			ddr_clock_ptr->ddr_clock_hz =
					simple_strtoul(argv[3], NULL, 0);
			printf("Setting ddr clock to: %d Hz\n",
			       ddr_clock_ptr->ddr_clock_hz);

			write_tlv_tuple(tlv_hdr_ptr);
		} else if (strncmp(argv[2], "ddrverbose", 10) == 0 &&
			   argc == (3 + 1)) {
			tlv_hdr_ptr->type = EEPROM_DDR_VERBOSE_TYPE;
			tlv_hdr_ptr->length =
					sizeof(octeon_eeprom_ddr_verbose_t);
			tlv_hdr_ptr->checksum = 0;
			tlv_hdr_ptr->major_version =
				OCTEON_EEPROM_DDR_VERBOSE_MAJOR_VER;
			tlv_hdr_ptr->minor_version =
				OCTEON_EEPROM_DDR_VERBOSE_MINOR_VER;
			ddr_verbose_ptr->verbosity =
					simple_strtoul(argv[3], NULL, 0);
			write_tlv_tuple(tlv_hdr_ptr);
			printf("Setting DDR verbosity to %u\n",
			       ddr_verbose_ptr->verbosity);
		} else {
			return CMD_RET_USAGE;
		}
	} else {
		return CMD_RET_USAGE;
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(tlv_eeprom, 12, 1, do_tlv_eeprom,
	   "EEPROM data parsing",
	   "[display|set|delete|help]\n"
	   "tlv_eeprom print\n"
	   "tlv_eeprom display\n"
	   "       - display contents of eeprom data structures\n"
	   "tlv_eeprom set\n"
	   "       - set contents of eeprom data structures, values in hex or decimal\n"
	   "       set clock <ddr_clock in Mhz> <cpu_ref_clock in Mhz * 8> <dfa_ref_clock in Mhz * 8> <spi_clock in Mhz> (CN3XXX/CN5XXX)\n"
	   "       set board <board type> <major rev> <minor rev>  <serial string>\n"
	   "       set mac    <mac base x:x:x:x:x:x> <mac addr count>\n"
	   "       set voltmult <voltage in millivolts> <cpu multiplier> (CN3XXX/CN5XXX)\n"
	   "       set volt <core voltage in millivolts> (CN6XXX/CNF7XXX/CN7XXX)\n"
	   "       set mult <cpu multiplier> <IO multiplier> (CN6XXX/CNF7XXX/CN7XXX)\n"
	   "       set ddrclock <ddr clock Hz> (CN6XXX/CNF7XXX/CN7XXX)\n"
	   "       set ddrverbose <verbosity> Sets the DDR verbosity during DDR \n"
	   "           initialization, 0 for none.\n"
	   "             bit 0: DDR verbose output\n"
	   "             bit 1: DDR trace output\n"
	   "             bit 2: DDR debug output\n"
	   "       set capability [coremask_override]\n"
	   "tlv_eeprom delete <tuple addr>\n"
	   "       - deletes tuple at address and compacts any following tuples.\n"
	   "\n"
	   "Chip types have been deprecated, and can no longer be set.\n"
	   "\n" "(use tlv_eeprom help for list of board types)\n" "");
