/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights
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
 * Main Octeon executive header file (This should be the second header
 * file included by an application).
 *
 * <hr>$Revision: 96742 $<hr>
*/
#ifndef __CVMX_H__
#define __CVMX_H__

#ifdef	__cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif
#define CVMX_ENABLE_PARAMETER_CHECKING	0

#include "cvmx-abi.h"
#include "cvmx-swap.h"
#include "cvmx-warn.h"
#include "octeon-model.h"
#include "cvmx-address.h"

#include "cvmx-platform.h"
#include "cvmx-access.h"
#include "cvmx-csr-enums.h"
#include "cvmx-ciu2-defs.h"
#include "cvmx-ciu3-defs.h"
#include "cvmx-fpa-defs.h"
#include "cvmx-lmcx-defs.h"
#include "cvmx-mio-defs.h"
#include "cvmx-rst-defs.h"
#include "cvmx-rnm-defs.h"
#include "cvmx-asm.h"
#include "octeon-feature.h"
#include "cvmx-ciu-defs.h"
#include "cvmx-ipd-defs.h"
#include "cvmx-utils.h"
#include "cvmx-tim-defs.h"
#include "cvmx-clock.h"
#include "octeon-fpa.h"

#if defined(__mips__) && !defined(CVMX_BUILD_FOR_LINUX_HOST)
#include "cvmx-access-native.h"
#endif

#ifdef	__cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif /*  __CVMX_H__  */
