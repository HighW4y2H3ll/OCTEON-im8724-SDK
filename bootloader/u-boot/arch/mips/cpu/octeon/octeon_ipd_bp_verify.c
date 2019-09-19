/***********************license start************************************
 * Copyright (c) 2005-2007 Cavium Inc. (support@cavium.com). All rights
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
 ***********************license end**************************************/

#include <common.h>
#include "octeon_boot.h"
#include "octeon_hal.h"


/**
 * Verify the IPD backpressure workaround setup in
 * octeon_ipd_bp_enable() is working properly.
 *
 * @return Non zero if it is working
 */
int octeon_ipd_bp_verify(void)
{
    uint64_t bp_status0;
    uint64_t bp_status1;

    /* Enable backpressure for port with a zero threshold */
    cvmx_write_csr(CVMX_IPD_PORTX_BP_PAGE_CNT(0), 1<<17);
    cvmx_write_csr(CVMX_IPD_PORTX_BP_PAGE_CNT(16), 1<<17);

    /* Add 1000 to the page count to simulate packets comming in */
    cvmx_write_csr(CVMX_IPD_SUB_PORT_BP_PAGE_CNT, 1000);
    cvmx_write_csr(CVMX_IPD_SUB_PORT_BP_PAGE_CNT, (16<<25) | 1000);

    /* Wait 500 cycles for the BP to update */
    cvmx_wait(500);

    /* Read the BP state from the debug select register */
    cvmx_write_csr(CVMX_NPI_DBG_SELECT, 0x9004);
    bp_status0 = cvmx_read_csr(CVMX_DBG_DATA);
    cvmx_write_csr(CVMX_NPI_DBG_SELECT, 0x9804);
    bp_status1 = cvmx_read_csr(CVMX_DBG_DATA);

    /* Shutdown BP */
    cvmx_write_csr(CVMX_IPD_SUB_PORT_BP_PAGE_CNT, 0x1ffffff & -1000);
    cvmx_write_csr(CVMX_IPD_SUB_PORT_BP_PAGE_CNT, (16<<25) | (0x1ffffff & -1000));
    cvmx_write_csr(CVMX_IPD_PORTX_BP_PAGE_CNT(0), 0);
    cvmx_write_csr(CVMX_IPD_PORTX_BP_PAGE_CNT(16), 0);

    /* Clear any error interrupts that might have been set */
    cvmx_write_csr(CVMX_IPD_INT_SUM, 0x1f);

    /* Ignore results from interfaces that aren't enabled or aren't SPI */
    if (cvmx_read_csr(CVMX_GMXX_INF_MODE(0)) != 0x3)
        bp_status0 = 0;
    if (cvmx_read_csr(CVMX_GMXX_INF_MODE(1)) != 0x3)
        bp_status1 = 0;

    /* Return failure if BP isn't on */
    return (((bp_status0&1) == 0) && ((bp_status1&1) == 0));
}
