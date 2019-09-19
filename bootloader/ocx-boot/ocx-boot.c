/***********************license start***************
 * Copyright (c) 2016  Cavium Inc. (support@cavium.com). All rights
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
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
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

#include <ocx-config.h>

#include <tinylibc.h>
#include <cvmx-bootloader.h>
#include <cvmx-mio-defs.h>
#include <cvmx-ciu-defs.h>
#include <cvmx-l2c-defs.h>
#include <cvmx-ocx-defs.h>
#include <cvmx-gserx-defs.h>
#include <cvmx-rst-defs.h>

#include "tiny_stdio.c"
#include "tinylibc.c"
#include "octeon3_ocx_train.c"

extern int cvmx_node;
extern int cvmx_uart;
extern unsigned long timestamp;
extern uint64_t cpu_speed;
extern unsigned long __start, __end, _edata;

int main(void)
{
	int rc;

	tinylibc_init();
	cvmx_node = cvmx_get_node_num();

	/* Configure the UART */
	octeon_set_uart(UART_NODE, UART_PORT);
	octeon_set_baudrate(UART_BAUD_RATE, 0);

	printf("CPU Speed: %llu\n", (unsigned long long)cpu_speed);
	printf("Initializing CCPI/OCX link on node %d\n", cvmx_node);
	printf("Node: %d\n", ccpi_get_node_num());

	init_ccpi_early(false);

	rc = init_ccpi_connection2(0, 10312, false);
	if (rc)
		puts("Error initializing CCPI links\n");
	else
		puts("CCPI links initialized\n");

	cvmx_write_csr_node(cvmx_node, CVMX_MIO_BOOT_LOC_CFGX(0), 0);
	cvmx_write_csr_node(cvmx_node, CVMX_MIO_BOOT_LOC_CFGX(1), 0);

	/* Put all cores on node 1 in reset */
	cvmx_write_csr_node(cvmx_node, CVMX_RST_PP_POWER, -1ull);
	cvmx_write_csr_node(cvmx_node, CVMX_CIU_PP_RST, (1ull << 48) - 1);

	puts("Reset failed\n");

	return 0;
}
