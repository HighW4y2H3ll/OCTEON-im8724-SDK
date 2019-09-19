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


/*
 * File version info: $Id: executive-config.h 126701 2015-10-15 19:18:30Z cchavva $
 *
 */
#ifndef __EXECUTIVE_CONFIG_H__
#define __EXECUTIVE_CONFIG_H__

/* Define to enable the use of simple executive DFA functions */
//#define CVMX_ENABLE_DFA_FUNCTIONS

/* Define to enable the use of simple executive timer bucket functions.
** Refer to cvmx-tim.[ch] for more information
*/
//#define CVMX_ENABLE_TIMER_FUNCTIONS

/* Define to enable the use of simple executive helper functions. These
** include many harware setup functions.  See cvmx-helper.[ch] for
** details.
*/
#define CVMX_ENABLE_HELPER_FUNCTIONS

/* CVMX_HELPER_FIRST_MBUFF_SKIP is the number of bytes to reserve before
** the beginning of the packet. If necessary, override the default
** here.  See the IPD section of the hardware manual for MBUFF SKIP
** details.*/
#define CVMX_HELPER_FIRST_MBUFF_SKIP 40

/* CVMX_HELPER_NOT_FIRST_MBUFF_SKIP is the number of bytes to reserve in each
** chained packet element. If necessary, override the default here */
#define CVMX_HELPER_NOT_FIRST_MBUFF_SKIP 0

/* CVMX_HELPER_ENABLE_IPD controls if the IPD is enabled in the helper
**  function. Once it is enabled the hardware starts accepting packets. You
**  might want to skip the IPD enable if configuration changes are need
**  from the default helper setup. If necessary, override the default here */
#define CVMX_HELPER_ENABLE_IPD 1

/* CVMX_HELPER_INPUT_TAG_TYPE selects the type of tag that the IPD assigns
** to incoming packets. */
#define CVMX_HELPER_INPUT_TAG_TYPE CVMX_POW_TAG_TYPE_ORDERED

/* The following select which fields are used by the PIP to generate
** the tag on INPUT
** 0: don't include
** 1: include */
#define CVMX_HELPER_INPUT_TAG_IPV6_SRC_IP	0
#define CVMX_HELPER_INPUT_TAG_IPV6_DST_IP   	0
#define CVMX_HELPER_INPUT_TAG_IPV6_SRC_PORT 	0
#define CVMX_HELPER_INPUT_TAG_IPV6_DST_PORT 	0
#define CVMX_HELPER_INPUT_TAG_IPV6_NEXT_HEADER 	0
#define CVMX_HELPER_INPUT_TAG_IPV4_SRC_IP	0
#define CVMX_HELPER_INPUT_TAG_IPV4_DST_IP   	0
#define CVMX_HELPER_INPUT_TAG_IPV4_SRC_PORT 	0
#define CVMX_HELPER_INPUT_TAG_IPV4_DST_PORT 	0
#define CVMX_HELPER_INPUT_TAG_IPV4_PROTOCOL	0
#define CVMX_HELPER_INPUT_TAG_INPUT_PORT	1

/* Select skip mode for input ports */
#define CVMX_HELPER_INPUT_PORT_SKIP_MODE	CVMX_PIP_PORT_CFG_MODE_SKIPL2

/* Define the number of queues per output port */
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE0	8
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE1	8
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE2	8
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE3	8
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE4	8
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE5	8

#define CVMX_HELPER_PKO_MAX_PORTS_INTERFACE0            4
#define CVMX_HELPER_PKO_MAX_PORTS_INTERFACE1            4

/* Force backpressure to be disabled.  This overrides all other
** backpressure configuration */
#define CVMX_HELPER_DISABLE_RGMII_BACKPRESSURE 0

/* Disable the SPI4000's processing of backpressure packets and backpressure
** generation. When this is 1, the SPI4000 will not stop sending packets when
** receiving backpressure. It will also not generate backpressure packets when
** its internal FIFOs are full. */
#define CVMX_HELPER_DISABLE_SPI4000_BACKPRESSURE 1

/* Select the number of low latency memory ports (interfaces) that
** will be configured.  Valid values are 1 and 2.
*/
#define CVMX_LLM_CONFIG_NUM_PORTS 1

/* Enable the fix for PKI-100 errata ("Size field is 8 too large in WQE and next
** pointers"). If CVMX_ENABLE_LEN_M8_FIX is set to 0, the fix for this errata will
** not be enabled.
** 0: Fix is not enabled
** 1: Fix is enabled, if supported by hardware
*/
#define CVMX_ENABLE_LEN_M8_FIX  1

/* Configure ILK channel for each interface. Note: available only in cn68xx
 * and cn78xx Octeon model */
#define CVMX_HELPER_ILK_MAX_CHAN_INTERFACE0 8
#define CVMX_HELPER_ILK_MAX_CHAN_INTERFACE1 8

/* Configure ILK lane mask for each interface. Note: available only in cn68xx 
 * and cn78xx Octeon model */
#define CVMX_HELPER_ILK_INTF_MASK_INTERFACE0 0xf
#define CVMX_HELPER_ILK_INTF_MASK_INTERFACE1 0xf0

/* Configure ILK interface to work in look-aside mode.  Note that this is only
** available for 68XX pass 2.0 and later. */
#define CVMX_HELPER_ILK_LA_MODE_INTERFACE0 0
#define CVMX_HELPER_ILK_LA_MODE_INTERFACE1 0

/* Enable the RX calendar for ILK LA mode
** NOTE: This is only available for CN68XX pass 2.0 and in pass 2.0 there
** is a bug where if the CN68XX is connected to another CN68XX that the
** calendar can only be enabled on one end or on neither end.  If enabled
** for both sides of the connection then traffic will not flow. */
#define CVMX_HELPER_ILK_LA_MODE_CAL_ENABLE_INTERFACE0   0
#define CVMX_HELPER_ILK_LA_MODE_CAL_ENABLE_INTERFACE1   0

/* Executive resource descriptions provided in cvmx-resources.config */
#include "cvmx-resources.config"

#ifdef CAVIUM_COMPONENT_REQUIREMENT
cvmxconfig
{
    scratch TRAFFICGEN_SCR_WORK
        size        = 8
        iobdma      = true
        permanent   = true
        description = "Async get work";
}
#endif

#endif
