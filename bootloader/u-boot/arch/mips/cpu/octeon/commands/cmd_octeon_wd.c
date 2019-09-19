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
 *
 * $Id: cmd_octeon_wd.c 155618 2017-03-09 06:56:43Z cchavva $
 *
 */
/*
 * Watchdog support
 */

#include <common.h>
#include <command.h>
#include <config.h>
#include <watchdog.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-access-native.h>

DECLARE_GLOBAL_DATA_PTR;

extern void hw_watchdog_start(int msecs);
extern void hw_watchdog_disable(void);

int do_octeonwd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t tout = 0;

	if (argc < 2)
		return cmd_usage(cmdtp);

	if (!strcmp(argv[1], "start")) {
		if (argc == 3) {
			tout = simple_strtoul(argv[2], NULL, 0);
		}
		hw_watchdog_start(tout);
		if (tout)
			printf("Started watchdog with a %u ms timeout\n", tout);
		else
			puts("Started watchdog with default timeout\n");
	} else if (!strcmp(argv[1], "stop")) {
		hw_watchdog_disable();
		puts("The watchdog is now disabled.\n");
	} else if (!strcmp(argv[1], "test")) {
		if (argc == 3)
			tout = simple_strtoul(argv[2], NULL, 0);
		else
			tout = 60000;
		printf("About to hang for %u ms.\n", tout);
		printf("An NMI error should be displayed if the timeout\n"
		       "exceeds the current watchdog timeout.\n");
		/* use cvmx_wait since it doesn't pet the watchdog */
		cvmx_wait_usec(tout * 1000ull);
		puts("Hang completed - watchdog NMI did not occur.\n");
	} else {
		return cmd_usage(cmdtp);
	}

	return 0;
}

U_BOOT_CMD(octwd, 3, 0, do_octeonwd,
	   "Starts the OCTEON watchdog",
	   "Usage:\n"
	   "octwd start [timeout in ms] - initializes and starts the watchdog\n"
	   "octwd stop                  - disables the watchdog timer\n"
	   "octwd test  [timeout in ms] - test (hang for 'timeout' ms)\n");
