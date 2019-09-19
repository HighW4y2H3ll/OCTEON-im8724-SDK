/*
 * (C) Copyright 2004,2005,2010
 * Cavium Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * File version info: $Id: executive-config.h 73836 2012-06-01 18:56:26Z cchavva $
 *
 */
#ifndef __EXECUTIVE_CONFIG_H__
#define __EXECUTIVE_CONFIG_H__

/* Define to enable the use of simple executive DFA functions */
/*#define CVMX_ENABLE_DFA_FUNCTIONS*/

/* Define to enable the use of simple executive timer bucket functions.
 * Refer to cvmx-tim.[ch] for more information
 */
/*#define CVMX_ENABLE_TIMER_FUNCTIONS*/

/* Define to enable the use of simple executive helper functions. These
 * include many harware setup functions.  See cvmx-helper.[ch] for
 * details.
 */
#define CVMX_ENABLE_HELPER_FUNCTIONS

/* CVMX_HELPER_FIRST_MBUFF_SKIP is the number of bytes to reserve before
 * the beginning of the packet. If necessary, override the default
 * here.  See the IPD section of the hardware manual for MBUFF SKIP
 * details.
 */
#define CVMX_HELPER_FIRST_MBUFF_SKIP 184

/* CVMX_HELPER_NOT_FIRST_MBUFF_SKIP is the number of bytes to reserve in each
 * chained packet element. If necessary, override the default here
 */
#define CVMX_HELPER_NOT_FIRST_MBUFF_SKIP 0

/* CVMX_HELPER_ENABLE_IPD controls if the IPD is enabled in the helper
 *  function. Once it is enabled the hardware starts accepting packets. You
 *  might want to skip the IPD enable if configuration changes are need
 *  from the default helper setup. If necessary, override the default here
 */
#define CVMX_HELPER_ENABLE_IPD 0

/* CVMX_HELPER_INPUT_TAG_TYPE selects the type of tag that the IPD assigns
 * to incoming packets.
 */
#define CVMX_HELPER_INPUT_TAG_TYPE CVMX_POW_TAG_TYPE_ORDERED

/* The following select which fields are used by the PIP to generate
 * the tag on INPUT
 * 0: don't include
 * 1: include
 */
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
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE0	1
#define CVMX_HELPER_PKO_QUEUES_PER_PORT_INTERFACE1	1

/* Force backpressure to be disabled.  This overrides all other
 * backpressure configuration
 */
#define CVMX_HELPER_DISABLE_RGMII_BACKPRESSURE 1

/* Select the number of low latency memory ports (interfaces) that
 * will be configured.  Valid values are 1 and 2.
 */
#define CVMX_LLM_CONFIG_NUM_PORTS 1

/* Executive resource descriptions provided in cvmx-resources.config */
#include "cvmx-resources.config"

#endif
