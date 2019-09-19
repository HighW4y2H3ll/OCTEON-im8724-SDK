/***********************license start***************
 * Copyright (c) 2003-2012  Cavium Inc. (support@cavium.com). All rights
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

#include "cvmx.h"
#include "cvmx-fau.h"
#include "cvmx-global-resources.h"
#include "cvmx-bootmem.h"

CVMX_TLS uint8_t *cvmx_fau_regs_ptr;

void cvmx_fau_bootmem_init(void *bootmem)
{
	memset(bootmem, 0, CVMX_FAU_MAX_REGISTERS_8);
}

/**
 * Initializes FAU region for devices without FAU unit.
 * @return 0 on success -1 on failure
 */
int cvmx_fau_init()
{
	cvmx_fau_regs_ptr = (uint8_t*)cvmx_bootmem_alloc_named_range_once(CVMX_FAU_MAX_REGISTERS_8, 0, 1ull<<31, 128, "cvmx_fau_registers", cvmx_fau_bootmem_init);

	if(cvmx_fau_regs_ptr == 0ull) {
		cvmx_dprintf("ERROR: Failed to alloc named block for software FAU.\n");
		return -1;
	}

	return 0;
}
