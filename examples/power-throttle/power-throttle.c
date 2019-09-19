/***********************license start***************
 * Copyright (c) 2003-2013  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * An example for using the power-throttle API.
 *
 * <hr>$Revision: 91015 $<hr>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cvmx.h"
#include "cvmx-asm.h"
#include "cvmx-coremask.h"
#include "cvmx-power-throttle.h"

/**
 * Display (some fields of) the power throttle register
 *
 * @param str is a NULL-terminated string for display.
 * @param ppid is the core id.
 */
static void pth_display_reg(char *str, unsigned int ppid)
{
    typedef long long unsigned int pth_llui; /* to make printf happy */
#define PTH_PRINT_FIELD(xx, xx_name)					\
	do {								\
            cvmx_power_throttle_rfield_t r = cvmx_power_throttle_show(ppid); \
	    printf("    %s: 0x%x (core %u)\n", xx_name, r.s.xx, ppid);	\
	} while (0)

    printf("%s (core %u):\n", str, ppid);

    PTH_PRINT_FIELD(maxpow, "maxpow");
    PTH_PRINT_FIELD(power, "power");
    PTH_PRINT_FIELD(powlim, "powlim");
    if (!OCTEON_IS_MODEL(OCTEON_CN63XX))
        PTH_PRINT_FIELD(hrmpowadj, "hrmpowadj");
    fflush(stdout);
}

/*
 * a wrapper around different versions of cvmx_power_throttle.
 *
 * @param ppid is the core id
 * @param pct is the desired power level
 */
static int pth_power_throttle(int ppid, int percent)
{
    cvmx_coremask_t pm;
    cvmx_coremask_clear_all(&pm);
    cvmx_coremask_set_core(&pm, ppid);
    cvmx_power_throttle(percent, &pm);

    return 0;
}

/*
 * Scan the parameter list and try to set power level for a core as
 * specified.
 *
 * @param argc
 * @param argv
 */
static int pth_throttle_ppid(int argc, char *argv[])
{
#define PARAM_PPID "tgt_ppid"
#define PARAM_PCT  "tgt_pct"

    int i, flag;
    int len_ppid, len_pct;
    int ppid, pct;

    flag = 0;
    ppid = -1;
    pct = -1;
    len_ppid = strlen(PARAM_PPID);
    len_pct = strlen(PARAM_PCT);

    for (i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], PARAM_PPID, len_ppid) == 0)
	{
	    ppid = atoi(&(argv[i][len_ppid + 1]));
	    if (ppid < 0 || ppid >= CVMX_MAX_CORES)
	        return -1;
	    flag++;
	}
	    
        if (strncmp(argv[i], PARAM_PCT, len_pct) == 0)
	{
	    pct = atoi(&(argv[i][len_pct + 1]));
	    if (pct < 0 || pct > 100)
	        return -1;
	    flag++;
        }
    }

    if (flag > 1)
    {
        if (cvmx_is_init_core()) {
            printf("Core %d power-throttled to %d percent\n", ppid, pct);
            pth_power_throttle(ppid, pct);

            return 1;
        }
    }
    else if (flag == 1 && ppid != -1)
    {
        if (cvmx_is_init_core()) {
	    /*
	     * Display the PowerThrottle register for ppid.
	     */
            char str[128];

            sprintf(&str[0], "PowerThrottle on core %d:", ppid);
            pth_display_reg(str, ppid);

	    return 1;
        }
    }

    return 0;
}

int main (int argc, char *argv[])
{
    unsigned int ppid;
    int percent;
    char buf[128];

    if (!OCTEON_IS_MODEL(OCTEON_CN6XXX) && !OCTEON_IS_MODEL(OCTEON_CN7XXX))
    {
        printf("Power throttle is not supported.\n");
        return 0;
    }

    if (pth_throttle_ppid(argc, argv))
        return 0;
    
    ppid = cvmx_get_core_num();
    pth_display_reg("Before Throttling", ppid);

    /*
     * cvmx_power_throttle_self().
     */
    percent = 20;
    cvmx_power_throttle_self(percent);
    cvmx_wait_usec((ppid + 1) * 1000);
    sprintf(buf, "Throttled to %d%%", percent);
    pth_display_reg(buf, ppid);

    /*
     * pth_power_throttle().
     */
    percent = 10;
    pth_power_throttle(ppid, percent);

    cvmx_wait_usec((ppid + 1)* 1000);
    sprintf(buf, "Throttled again to %d%%", percent);
    pth_display_reg(buf, ppid);

    return 0;
}
