/****************************************************************
 * Copyright (c) 2007, Cavium Inc. (support@cavium.com). All rights
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
 * For any questions regarding licensing please contact marketing@cavium.com
 *
 **************************************************************************/

/**
 * @file oct-linux-identify.c
 *
 * Utility to identify the Octeon model we're running on
 *
 * $Id: oct-linux-csr.c 22261 2007-02-09 01:08:14Z kreese $
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cvmx.h"
#include "cvmx-sysinfo.h"

/**
 * Convert a boolean into "Yes" or "No"
 *
 * @param yes
 *
 * @return
 */
static const char *yes_no(int yes)
{
    if (yes)
        return "Yes";
    else
        return "No";
}

/**
 * Main
 *
 * @return
 */
int main()
{
    const char *        name;
    cvmx_l2d_fus3_t     fus3;
    cvmx_mio_fus_dat2_t fus_dat2;
    cvmx_mio_fus_dat3_t fus_dat3;
    int                 l2_size;
    int                 nokasumi = 0;

    cvmx_linux_enable_xkphys_access(0);

    cvmx_sysinfo_linux_userspace_initialize();

    name = octeon_model_get_string(cvmx_get_proc_id());

    if (OCTEON_IS_MODEL(OCTEON_CN3XXX) || OCTEON_IS_MODEL(OCTEON_CN5XXX))
        fus3.u64 = cvmx_read_csr(CVMX_L2D_FUS3);

    fus_dat2.u64 = cvmx_read_csr(CVMX_MIO_FUS_DAT2);
    fus_dat3.u64 = cvmx_read_csr(CVMX_MIO_FUS_DAT3);

    if (OCTEON_IS_MODEL(OCTEON_CN38XX))
    {
        nokasumi = 1;
        if (fus3.cn38xx.crip_256k)
            l2_size = 256;
        else if (fus3.cn38xx.crip_512k)
            l2_size = 512;
        else
            l2_size = 1024;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN31XX))
    {
        nokasumi = 1;
        if (fus3.cn31xx.crip_128k)
            l2_size = 128;
        else
            l2_size = 256;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN30XX))
    {
        fus_dat2.s.nodfa_cp2 = 1;
        fus_dat3.s.nodfa_dte = 1;
        nokasumi = 1;
        if (fus3.cn30xx.crip_64k)
            l2_size = 64;
        else
            l2_size = 128;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN58XX))
    {
        if (fus3.cn58xx.crip_512k)
            l2_size = 512;
        else if (fus3.cn58xx.crip_1024k)
            l2_size = 1024;
        else
            l2_size = 2048;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN56XX))
    {
        fus_dat2.s.nodfa_cp2 = 1;
        fus_dat3.s.nodfa_dte = 1;
        if (fus3.cn56xx.crip_512k)
            l2_size = 512;
        else if (fus3.cn56xx.crip_1024k)
            l2_size = 1024;
        else
            l2_size = 2048;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN52XX))
    {
        fus_dat2.s.nodfa_cp2 = 1;
        fus_dat3.s.nodfa_dte = 1;
        if (fus3.cn52xx.crip_128k)
            l2_size = 128;
        else if (fus3.cn52xx.crip_256k)
            l2_size = 256;
        else
            l2_size = 512;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN50XX))
    {
        fus_dat2.s.nodfa_cp2 = 1;
        fus_dat3.s.nodfa_dte = 1;
        if (fus3.cn50xx.crip_32k)
            l2_size = 32;
        else if (fus3.cn50xx.crip_64k)
            l2_size = 64;
        else
            l2_size = 128;
    }
    else if (OCTEON_IS_OCTEON2())
    {
        if (fus_dat3.cn63xx.l2c_crip == 3)
            l2_size = 512;
        else if (fus_dat3.cn63xx.l2c_crip == 2)
            l2_size = 1024;
        else if (fus_dat3.cn63xx.l2c_crip == 1)
            l2_size = 1536;
        else
            l2_size = 2048;

        /* FUS_DAT2[nokasumi] is not present in Octeon2, KASUMI 
           is enabled/disabled along with all the other crypto. */ 
        if (fus_dat2.s.nocrypto)
            nokasumi = 1;
    }
    else if (OCTEON_IS_MODEL(OCTEON_CN70XX))
    {
        if (fus_dat3.cn70xx.l2c_crip == 3)
            l2_size = 128;
        else if (fus_dat3.cn70xx.l2c_crip == 2)
            l2_size = 256;
        else if (fus_dat3.cn70xx.l2c_crip == 1)
            l2_size = 384;
        else
            l2_size = 512;

        /* FUS_DAT2[nokasumi] is not present in Octeon2, KASUMI 
           is enabled/disabled along with all the other crypto. */ 
        if (fus_dat2.s.nocrypto)
            nokasumi = 1;
    }
    else
    {
        l2_size = 0;
    }

    printf("Model:              %s\n", name);
    printf("Level 2 cache:      %d KB\n", l2_size);
    printf("Number of cores:    %d\n", cvmx_dpop(cvmx_read_csr(CVMX_CIU_FUSE)));
    printf("Crypto:             %s\n", yes_no(!fus_dat2.s.nocrypto));
    printf("Extended Multiplier:%s\n", yes_no(!fus_dat2.s.nomul));
    printf("Low Latency Memory: %s\n", yes_no(!fus_dat2.s.nodfa_cp2));
    printf("Kasumi:             %s\n", yes_no(!nokasumi));
    printf("RAID:               %s\n", yes_no(fus_dat2.s.raid_en));
    printf("DFA:                %s\n", yes_no(!fus_dat3.s.nodfa_dte));
    if (OCTEON_IS_MODEL(OCTEON_CN70XX_PASS1_X))
        printf("ZIP:                No\n");
    else
        printf("ZIP:                %s\n", yes_no(!fus_dat3.s.nozip));
    printf("PCI BAR2:           %s\n", yes_no(fus_dat3.cn38xx.bar2_en));

    return 0;
}

