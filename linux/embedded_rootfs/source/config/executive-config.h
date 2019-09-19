/*************************************************************************
 * Copyright (c) 2003-2005, Cavium Inc. (support@cavium.com). All rights
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
*************************************************************************/

/*
 * File version info: $Id: executive-config.h 69984 2012-02-15 18:26:13Z cchavva $
 */
#ifndef __EXECUTIVE_CONFIG_H__
#define __EXECUTIVE_CONFIG_H__

/* Define to enable the use of simple executive DFA functions */
//#define CVMX_ENABLE_DFA_FUNCTIONS

/* Define to enable the use of simple executive packet output functions.
** For packet I/O setup enable the helper functions below.
*/
//#define CVMX_ENABLE_PKO_FUNCTIONS

/* Define to enable the use of simple executive timer bucket functions.
** Refer to cvmx-tim.[ch] for more information
*/
//#define CVMX_ENABLE_TIMER_FUNCTIONS

/* Define to enable the use of simple executive helper functions. These
** include many harware setup functions.  See cvmx-helper.[ch] for
** details.
*/
//#define CVMX_ENABLE_HELPER_FUNCTIONS

/* CVMX_HELPER_FIRST_MBUFF_SKIP is the number of bytes to reserve before
** the beginning of the packet. If necessary, override the default
** here.  See the IPD section of the hardware manual for MBUFF SKIP
** details.*/
//#define CVMX_HELPER_FIRST_MBUFF_SKIP 184

/* CVMX_HELPER_NOT_FIRST_MBUFF_SKIP is the number of bytes to reserve in each
** chained packet element. If necessary, override the default here */
//#define CVMX_HELPER_NOT_FIRST_MBUFF_SKIP 0

/* CVMX_HELPER_ENABLE_IPD controls if the IPD is enabled in the helper
**  function. Once it is enabled the hardware starts accepting packets. You
**  might want to skip the IPD enable if configuration changes are need
**  from the default helper setup. If necessary, override the default here */
//#define CVMX_HELPER_ENABLE_IPD 1

/* Select the number of low latency memory ports (interfaces) that
** will be configured.  Valid values are 1 and 2.
*/
#define CVMX_LLM_CONFIG_NUM_PORTS 1

#if defined(CVMX_ENABLE_HELPER_FUNCTIONS) && !defined(CVMX_ENABLE_PKO_FUNCTIONS)
#define CVMX_ENABLE_PKO_FUNCTIONS
#endif

/* Executive resource descriptions provided in cvmx-resources.config */
#include "cvmx-resources.config"

#endif
