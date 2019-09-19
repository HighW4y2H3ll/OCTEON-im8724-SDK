/***********************license start************************************
 * Copyright (c) 2008-2014 Cavium Inc. (support@cavium.com). All rights
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
 * This Software, including technical data, may be subject to U.S.  export
 * control laws, including the U.S.  Export Administration Act and its
 * associated regulations, and may be subject to export or import regulations
 * in other countries.  You warrant that You will comply strictly in all
 * respects with all such regulations and acknowledge that you have the
 * responsibility to obtain licenses to export, re-export or import the
 * Software.
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
 **********************license end**************************************/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <arpa/inet.h>

#include "../include/asm/arch/cvmx-swap.h"
#include "../include/asm/arch/cvmx-app-init.h"
#include "../include/asm/arch/cvmx-bootloader.h"

extern unsigned long crc32(unsigned long, const unsigned char *, unsigned int);
void usage(void);

#undef DEBUG
#ifdef DEBUG
#define debug_printf    printf
#else
#define debug_printf(...)
#endif

/* Do some funky stuff here to get a compile time error if the size of the
 * bootloader_header_t structure has changed.
 */
#define compile_time_assert(cond, msg) char msg[(cond) ? 1 : -1]
compile_time_assert(sizeof(bootloader_header_t) == 192,
		    bootloader_header_size_changed);

#define BUF_SIZE    (16*1024)
#define NAME_LEN    100

#ifndef WOFFSETOF
/* word offset */
#define WOFFSETOF(type, elem)  (offsetof(type, elem)/4)
#endif

static int stage2_flag = 0;
static int stage_1_5_flag = 0;
static int stage_1_flag = 0;

int lookup_board_type(char *board_name)
{
	int i;
	int board_type = 0;
	char *substr = NULL;

	/* Detect stage 2 bootloader boards */
	if (strcasestr(board_name, "_stage2")) {
		debug_printf("Stage 2 bootloader detected from substring %s in name %s\n",
			     "_stage2", board_name);
		stage2_flag = 1;
	} else {
		debug_printf("Stage 2 bootloader NOT detected from name \"%s\"\n",
			     board_name);
	}
	/* Generic is a special case since there are numerous sub-types */
	if (!strncasecmp("generic", board_name, strlen("generic")))
		return CVMX_BOARD_TYPE_GENERIC;
	if (!strcasecmp("evb7000_sff_stage2", board_name))
		return CVMX_BOARD_TYPE_EVB7000_SFF;

	/* If we're an eMMC stage 2 bootloader, cut off the _emmc_stage2
	 * part of the name.
	 */

	if (((substr = strcasestr(board_name, "_emmc_stage2")) != NULL) &&
	    (substr[strlen("_emmc_stage2")] == '\0')) {
		/*return CVMX_BOARD_TYPE_GENERIC;*/

		printf("  Converting board name %s to ", board_name);
		*substr = '\0';
		printf("%s\n", board_name);
	}

	/* If we're a NAND stage 2 bootloader, cut off the _nand_stage2
	 * part of the name.
	 */
	if (((substr = strcasestr(board_name, "_nand_stage2")) != NULL) &&
	    (substr[strlen("_nand_stage2")] == '\0')) {
		/*return CVMX_BOARD_TYPE_GENERIC;*/

		printf("  Converting board name %s to ", board_name);
		*substr = '\0';
		printf("%s\n", board_name);
	}

	/* If we're a SPI stage 2 bootloader, cut off the _spi_stage2
	 * part of the name.
	 */
	if (((substr = strcasestr(board_name, "_spi_stage2")) != NULL) &&
	    (substr[strlen("_spi_stage2")] == '\0')) {
		/*return CVMX_BOARD_TYPE_GENERIC;*/

		printf("  Converting board name %s to ", board_name);
		*substr = '\0';
		printf("%s\n", board_name);
	}

	for (i = CVMX_BOARD_TYPE_NULL; i < CVMX_BOARD_TYPE_MAX; i++)
		if (!strcasecmp(cvmx_board_type_to_string(i), board_name))
			board_type = i;
	for (i = CVMX_BOARD_TYPE_CUST_DEFINED_MIN;
	     i < CVMX_BOARD_TYPE_CUST_DEFINED_MAX; i++)
		if (!strncasecmp(cvmx_board_type_to_string(i), board_name,
				 strlen(cvmx_board_type_to_string(i))))
			board_type = i;
	for (i = CVMX_BOARD_TYPE_CUST_PRIVATE_MIN;
	     i < CVMX_BOARD_TYPE_CUST_PRIVATE_MAX; i++)
		if (!strncasecmp(cvmx_board_type_to_string(i), board_name,
				 strlen(cvmx_board_type_to_string(i))))
			board_type = i;

	return (board_type);
}

/* Getoptions variables must be global */
static int failsafe_flag;
static int pciboot_flag;
static int env_flag;
int main(int argc, char *argv[])
{

	int fd;

	uint8_t buf[BUF_SIZE];
	uint32_t data_crc = 0;
	int len;
	int data_len = 0;
	bootloader_header_t header;
	char filename[NAME_LEN];
	int i;
	int option_index = 0;	/* getopt_long stores the option index here. */
	char board_name[NAME_LEN] = { 0 };
	char tmp_board_name[NAME_LEN] = { 0 };
	int c;
	int board_type = 0;
	unsigned long long address = 0;
	ssize_t ret;
	const char *type_str = NULL;

	debug_printf("header size is: %d bytes\n",
		     (int)sizeof(bootloader_header_t));

	/* Parse command line options using getopt_long */
	while (1) {
		static struct option long_options[] = {
			/* These options set a flag. */
			{"failsafe", no_argument, &failsafe_flag, 1},
			{"pciboot", no_argument, &pciboot_flag, 1},
			{"nandstage2", no_argument, &stage2_flag, 1},
			{"spistage2", no_argument, &stage2_flag, 1},
			{"stage2", no_argument, &stage2_flag, 1},
			{"stage1.5", no_argument, &stage_1_5_flag, 1},
			{"stage1", no_argument, &stage_1_flag, 1},
			{"environment", no_argument, &env_flag, 1},
			/* These options don't set a flag.
			 * We distinguish them by their indices.
			 */
			{"board", required_argument, 0, 0},
			{"text_base", required_argument, 0, 0},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, (char *const *)argv, "h",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c) {
			/* All long options handled in case 0 */
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			debug_printf("option(l) %s",
				     long_options[option_index].name);
			if (optarg)
				debug_printf(" with arg %s", optarg);
			debug_printf("\n");

			if (!strcmp(long_options[option_index].name, "board")) {
				strncpy(board_name, optarg, NAME_LEN);

				debug_printf("Using user supplied board name: %s\n",
					     board_name);
			} else
			    if (!strcmp(long_options[option_index].name,
					"text_base")) {
				address = strtoull(optarg, NULL, 0);
				debug_printf("Address of image is: 0x%llx\n",
					     (unsigned long long) address);
				if (!(address & 0xFFFFFFFFULL << 32)) {
					if (address & 1 << 31) {
						address |= 0xFFFFFFFFULL << 32;
						debug_printf("Converting address to 64 bit compatibility space: 0x%llx\n",
							     address);
					}
				}
			}
			break;

		case 'h':
		case '?':
			/* getopt_long already printed an error message. */
			usage();
			return -1;
			break;

		default:
			abort();
		}
	}

	if (optind < argc) {
		/* We only support one argument - an optional bootloader
		 * file name
		 */
		if (argc - optind > 2) {
			debug_printf("non-option ARGV-elements: ");
			while (optind < argc)
				debug_printf("%s ", argv[optind++]);
			debug_printf("\n");
			usage();
			return -1;
		}
	}
	strncpy(filename, argv[optind], NAME_LEN);
	if (board_name[0] == '\0')
		strncpy(board_name, argv[optind + 1], NAME_LEN);
	strncpy(tmp_board_name, board_name, NAME_LEN);

	if ((fd = open(filename, O_RDWR)) < 0) {
		printf("Unable to open file: %s\n", filename);
		exit(-1);
	}
	if (failsafe_flag)
		debug_printf("Setting failsafe flag\n");

	if (strlen(board_name)) {
		int offset = 0;
		debug_printf("Supplied board name of: %s\n", board_name);

		if (strstr(board_name, "failsafe")) {
			failsafe_flag = 1;
			debug_printf
			    ("Setting failsafe flag based on board name\n");
		}
		/* Skip leading octeon_ if present. */
		if (!strncmp(board_name, "octeon_", 7))
			offset = 7;

		/* Check to see if 'failsafe' is in the name.  If so, set the
		 * failsafe flag.  Also, ignore extra trailing characters on
		 * passed parameter when comparing against board names.
		 * We actually use the configuration name from u-boot, so it
		 * may have some other variant names.  Variants other than
		 * failsafe _must_ be passed to this program explicitly
		 */

		board_type = lookup_board_type(board_name + offset);
		if (!board_type) {
			/* Retry with 'cust_' prefix to catch boards that are
			 * in the customer section (such as nb5)
			 */
			sprintf(tmp_board_name, "cust_%s", board_name + offset);
			board_type = lookup_board_type(tmp_board_name);
		}
		strncpy(tmp_board_name, board_name, NAME_LEN);/* reset to original value */
		if (!board_type) {
			/* Retry with 'cust_private_' prefix to catch boards
			 * that are in the customer private section
			 */
			sprintf(tmp_board_name, "cust_private_%s",
				board_name + offset);
			board_type = lookup_board_type(tmp_board_name);

		}

		if (!board_type) {
			printf("ERROR: unable to determine board type\n");
			exit(-1);
		}
		debug_printf("Board type is: %d: %s\n", board_type,
			     cvmx_board_type_to_string(board_type));
	} else {
		printf("Board name must be specified!\n");
		exit(-1);
	}

	/* Check to see if there is either an existing header, or that there
	 * are zero valued bytes where we want to put the header
	 */
	if ((len = read(fd, buf, BUF_SIZE)) > 0) {

		/* Copy the header, as the first word (jump instruction, needs
		 * to remain the same.
		 */
		memcpy(&header, buf, sizeof(bootloader_header_t));
		/* Check to see if we have zero bytes (excluding first 4, which
		 * are the jump instruction)
		 */
		for (i = 1; i < sizeof(bootloader_header_t) / 4; i++) {
			if (((uint32_t *) buf)[i]) {
				printf("ERROR: non-zero word found %x in location %d required for header, aborting\n",
				       ((uint32_t *) buf)[i], i);
				exit(-1);
			}
		}
		debug_printf("Zero bytes found in header location, adding header.\n");

	} else {
		printf("Unable to read from file %s\n", filename);
		exit(-1);
	}

	/* Read data bytes and generate CRC */
	lseek(fd, sizeof(bootloader_header_t), SEEK_SET);

	while ((len = read(fd, buf, BUF_SIZE)) > 0) {
		data_crc = crc32(data_crc, buf, len);
		data_len += len;
	}
	printf("CRC of data: 0x%x, length: %d\n", data_crc, data_len);

	/* Now create the new header */

	header.magic = htonl(BOOTLOADER_HEADER_MAGIC);
	header.maj_rev = htons(BOOTLOADER_HEADER_CURRENT_MAJOR_REV);
	header.min_rev = htons(BOOTLOADER_HEADER_CURRENT_MINOR_REV);
	header.dlen = htonl(data_len);
	header.dcrc = htonl(data_crc);
	header.board_type = htons(board_type);
	header.address = cvmx_cpu_to_be64(address);
	if (failsafe_flag)
		header.flags |= htonl(BL_HEADER_FLAG_FAILSAFE);

	debug_printf("Stage 2 flag is %sset\n", stage2_flag ? "" : "not ");
	if (pciboot_flag)
		header.image_type = htons(BL_HEADER_IMAGE_PCIBOOT);
	else if (stage2_flag)
		header.image_type = htons(BL_HEADER_IMAGE_STAGE2);
	else if (env_flag)
		header.image_type = htons(BL_HEADER_IMAGE_UBOOT_ENV);
	else if (stage_1_5_flag || stage_1_flag)
		header.image_type = htons(BL_HEADER_IMAGE_PRE_UBOOT);
	else
		header.image_type = htons(BL_HEADER_IMAGE_NOR);

	switch (ntohs(header.image_type)) {
	case BL_HEADER_IMAGE_UNKNOWN:
		type_str = "Unknown";
		break;
	case BL_HEADER_IMAGE_STAGE2:
		type_str = "Stage 2";
		break;
	case BL_HEADER_IMAGE_PRE_UBOOT:
		type_str = "Pre-U-Boot";
		break;
	case BL_HEADER_IMAGE_STAGE3:
		type_str = "Stage 3";
		break;
	case BL_HEADER_IMAGE_NOR:
		type_str = "NOR";
		break;
	case BL_HEADER_IMAGE_PCIBOOT:
		type_str = "PCI Boot";
		break;
	case BL_HEADER_IMAGE_UBOOT_ENV:
		type_str = "U-Boot Environment";
		break;
	default:
		if (ntohs(header.image_type) >= BL_HEADER_IMAGE_CUST_RESERVED_MIN &&
		    ntohs(header.image_type) <= BL_HEADER_IMAGE_CUST_RESERVED_MAX)
			type_str = "Customer Reserved";
		else
			type_str = "Unsupported";
	}
	printf("Header image type: %s\n", type_str);
	header.hlen = htons(sizeof(bootloader_header_t));

	/* Now compute header CRC over all of the header excluding the CRC */
	header.hcrc = crc32(0, (void *)&header, 12);
	header.hcrc =
	    htonl(crc32
		  (header.hcrc, ((void *)&(header)) + 16,
		   sizeof(bootloader_header_t) - 16));

	/* Seek to beginning of file */
	lseek(fd, 0, SEEK_SET);

	/* Write header to file */
	ret = write(fd, &header, sizeof(bootloader_header_t));
	if (ret < 0)
		perror("write");

	close(fd);

	printf("Header CRC: 0x%x\n", ntohl(header.hcrc));
	return 0;
}

void usage(void)
{
	printf("Usage: update_octeon_header <filename> <board_name> [--failsafe] [--text_base=0xXXXXX]\n");
}
