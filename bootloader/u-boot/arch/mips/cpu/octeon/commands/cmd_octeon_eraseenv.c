/***********************license start************************************
 * Copyright (c) 2011 Cavium Inc. (support@cavium.com). All rights
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


/**
 * @file
 *
 * $Id: cmd_octeon_eraseenv.c 69988 2012-02-15 18:55:21Z cchavva $
 *
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <environment.h>
#include <malloc.h>

#if CONFIG_ENV_IS_IN_FLASH

static env_t *flash_addr = (env_t *)CONFIG_ENV_ADDR;

/* CONFIG_ENV_ADDR is supposed to be on sector boundary */
static ulong end_addr = CONFIG_ENV_ADDR + CONFIG_ENV_SECT_SIZE - 1;

int do_eraseenv(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int rc = 1;
#if CONFIG_ENV_SECT_SIZE > CONFIG_ENV_SIZE
	char	*saved_data = NULL;
	ulong	up_data = 0;
	
	up_data = (end_addr + 1) - ((long)flash_addr + CONFIG_ENV_SIZE);
	debug("Data to save 0x%lx\n", up_data);
	if (up_data) {
		if ((saved_data = malloc(up_data)) == NULL) {
			printf("Unable to save the rest of the sector (%ld)\n",
			       up_data);
			goto done;
		}
		memcpy(saved_data,
		       (void *)((long)flash_addr + CONFIG_ENV_SIZE),
		       up_data);
		debug("Data (start 0x%lx, len 0x%lx) saved at 0x%lx\n",
		      (ulong)flash_addr + CONFIG_ENV_SIZE, up_data,
		      (ulong)saved_data);
	}
#endif /* CONFIG_ENV_SECT_SIZE */
	debug("Protect off %08lX .. %08lX\n", (ulong)flash_addr, end_addr);
	if (flash_sect_protect(0, (long)flash_addr, end_addr))
		goto done;
	puts("Erasing Flash...");
	if (flash_sect_erase((long)flash_addr, end_addr))
		goto done;
#if CONFIG_ENV_SECT_SIZE > CONFIG_ENV_SIZE
	if (up_data) {	/* restore the rest of sector */
		debug("Restoring the rest of data to 0x%lx len 0x%lx\n",
			(ulong)flash_addr + CONFIG_ENV_SIZE, up_data);
		if (flash_write(saved_data,
				(long)flash_addr + CONFIG_ENV_SIZE,
				up_data)) {
			flash_perror(rc);
			goto done;
		}
	}
#endif
	puts("done\n");
	rc = 0;
done:
#if CONFIG_ENV_SECT_SIZE > CONFIG_ENV_SIZE
	if (saved_data)
		free(saved_data);
#endif
	/* Try to re-protect */
	(void)flash_sect_protect(1, (long)flash_addr, end_addr);
	return rc;	
}

U_BOOT_CMD(
	eraseenv,	1,	0,	do_eraseenv,
	"erase the environment in flash",
	"erase the environment in flash"
);

#endif /* CONFIG_ENV_IS_IN_FLASH */