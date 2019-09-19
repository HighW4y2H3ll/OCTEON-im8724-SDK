/***********************license start***************
 * Copyright (c) 2003-2017  Cavium Inc. (support@cavium.com). All rights
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
 * Interface to the Octeon cnf75xx extended error status.
 *
 * This file is auto generated. Do not edit.
 */

#ifndef __CVMX_ERROR_ARRAYS_CN78XX_H__
#define __CVMX_ERROR_ARRAYS_CN78XX_H__

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

struct cvmx_error_78xx {
	uint64_t block_csr;		/* CSR's address */
	char *err_mesg;			/* Error message */
	uint32_t intsn;			/* Interrupt source number */
	uint16_t flags;			/* Flags */
	uint8_t error_group;		/* Error group */
	uint8_t block_csr_bitpos;	/* Bit position in the CSR */
};

extern unsigned int cvmx_error_78xx_array_sizes [4];

extern struct cvmx_error_78xx error_array_cn78xx[2678];
extern struct cvmx_error_78xx error_array_cn78xxp1[2682];
extern struct cvmx_error_78xx error_array_cn73xx[1530];
extern struct cvmx_error_78xx error_array_cnf75xx[949];

struct cvmx_error_array {
	struct cvmx_error_78xx *array;
	uint32_t prid_mask;
	uint32_t prid_val;
};

extern struct cvmx_error_array octeon_error_arrays[5];

#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /* __CVMX_ERROR_ARRAYS_CN78XX_H__*/
