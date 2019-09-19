/***********************license start************************************
 * Copyright (c) 2011  Cavium Inc. (support@cavium.com).
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
#include <command.h>
#include <linux/mtd/mtd.h>
#include <watchdog.h>
#include <asm/byteorder.h>
#include <asm/gpio.h>
#include <nand.h>
#include <asm/arch/octeon_nand.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/cvmx-bootloader.h>
#include <asm/arch/lib_octeon.h>

DECLARE_GLOBAL_DATA_PTR;

extern unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				char * const argv[]);

int do_oct_nand_boot_stage3(cmd_tbl_t *cmdtbp, int flag, int argc,
			    char * const argv[])
{
	uint32_t addr;
	int rc;
	int failsafe = 0;
	int dev;
	int image_no = 0;
	uint32_t image_size;
	int64_t nand_addr;
	int64_t last_image_addr = -1;
	const loff_t end = CONFIG_OCTEON_NAND_BOOT_END;

	dev = getenv_ulong("octeon_stage3_devno", 0,
			   CONFIG_OCTEON_NAND_BOOT_DEV);
	addr = getenv_ulong("octeon_stage3_load_addr", 16,
			    CONFIG_OCTEON_STAGE3_LOAD_ADDR);
#ifndef CONFIG_OCTEON_NO_STAGE3_FAILSAFE
	failsafe = !!gpio_get_value(CONFIG_OCTEON_FAILSAFE_GPIO);
	if (!failsafe && getenv("octeon_stage3_failsafe_bootloader"))
		failsafe = 1;

#endif
	rc = 0;
	do {
		debug("Searching for NAND image %d\n", image_no);
		nand_addr = octeon_nand_image_search(dev, image_no,
						     gd->board_type, 0,
						     &image_size,
						     CONFIG_OCTEON_NAND_BOOT_START,
						     end);
		if (nand_addr >= 0) {
			if (octeon_nand_validate_image(dev, nand_addr, end)) {
				printf("Skipping invalid image at NAND offset 0x%llx\n",
				       nand_addr);
				image_no++;
				continue;
			}
			debug("%s: Found image header at NAND offset 0x%llx\n",
			      __func__, nand_addr);
			image_no++;
			last_image_addr = nand_addr;
		}
	} while (nand_addr >= 0);

	if (last_image_addr < 0) {
		puts("No valid bootloader image found in NAND flash\n");
		return -1;
	}
	rc = octeon_nand_read_dev(dev, last_image_addr, image_size,
				  CONFIG_OCTEON_NAND_BOOT_END - last_image_addr,
				  (void *)addr, NULL);
	if (rc) {
		puts("Error reading bootloader image from NAND\n");
		return -1;
	}
	do_go_exec((void *)addr, argc - 1, argv + 1);

	return -1;
}

U_BOOT_CMD(bootstage3, 1, 0, do_oct_nand_boot_stage3,
	   "Load and execute the stage 3 bootloader",
	   "Load and execute the stage 3 bootloader");
