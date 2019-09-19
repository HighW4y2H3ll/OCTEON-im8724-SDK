/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights
 * ither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, increserved.
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

#define PKI_SCHD_MAX_INTERFACE      10
#define PKI_MODIFY_DEFAULT_CONFIG   1
#define PKI_SEPARATE_WQE_BUFF       0
#define PKI_ENABLE_RED              1
#define PKI_ENABLE_BACK_PRESSURE    0

#define EXAMPLE_ROUTE_DMAC_PKTS       0

/* global resources should be set if any of interface/port
 is not going to use its own resources */
#define PKI_SETUP_GLOBAL_POOL           1
#define PKI_SETUP_GLOBAL_AURA           1
#define PKI_SETUP_GLOBAL_SSO_GRP        1
#define PKI_GBL_POOL_MAX_BUFF           6000
#define PKI_GBL_POOL_BUFF_SIZE          2048

/* you really need only one of this, it is here to test all the interfaces together*/
/* SGMII interface --------------------------------------------*/
#define PKI_STYLE_PER_INTF_FLAG_SGMII                   1

#define PKI_POOL_PER_INTF_FLAG_SGMII                    1
#define PKI_AURA_PER_INTF_FLAG_SGMII                    1
#define PKI_SSO_GRP_PER_INTF_FLAG_SGMII                 1

#define PKI_POOL_PER_PORT_FLAG_SGMII                    0
#define PKI_AURA_PER_PORT_FLAG_SGMII                    0
#define PKI_SSO_GRP_PER_PORT_FLAG_SGMII                 0

#define PKI_QPG_QOS_SGMII                               CVMX_PKI_QPG_QOS_NONE
#define PKI_POOL_PER_QOS_FLAG_SGMII                     0
#define PKI_AURA_PER_QOS_FLAG_SGMII                     0
#define PKI_SSO_GRP_PER_QOS_FLAG_SGMII                  0

/*if qos scheduling enabled */
#define PKI_SHARE_AURA_QOS_FLAG_SGMII                   0
#define PKI_SHARE_SSO_GRP_QOS_FLAG_SGMII                0

/* Passthrough_o3 uses same setting for all bgx smac interfaces */
/* user can modify them as needed */
/* XAUI/RXAUI/DXAUI/XFI/XLAUI interfaces -----------------------------------------------*/
#define PKI_STYLE_PER_INTF_FLAG_BGX_SMAC                   1
#define PKI_POOL_PER_INTF_FLAG_BGX_SMAC                    1
#define PKI_AURA_PER_INTF_FLAG_BGX_SMAC                    1
#define PKI_SSO_GRP_PER_INTF_FLAG_BGX_SMAC                 1

#define PKI_POOL_PER_PORT_FLAG_BGX_SMAC                    0
#define PKI_AURA_PER_PORT_FLAG_BGX_SMAC                    0
#define PKI_SSO_GRP_PER_PORT_FLAG_BGX_SMAC                 0

#define  PKI_QPG_QOS_BGX_SMAC                              CVMX_PKI_QPG_QOS_NONE
/*if qos scheduling enabled */
#define PKI_SHARE_AURA_QOS_FLAG_BGX_SMAC                   0
#define PKI_SHARE_SSO_GRP_QOS_FLAG_BGX_SMAC                0
#define PKI_POOL_PER_QOS_FLAG_BGX_SMAC                     0
#define PKI_AURA_PER_QOS_FLAG_BGX_SMAC                     0
#define PKI_SSO_GRP_PER_QOS_FLAG_BGX_SMAC                  0


/* ILK interface --------------------------------------------------------------------------*/
#define PKI_STYLE_PER_INTF_FLAG_ILK                   1

#define PKI_POOL_PER_INTF_FLAG_ILK                    1
#define PKI_AURA_PER_INTF_FLAG_ILK                    1
#define PKI_SSO_GRP_PER_INTF_FLAG_ILK                 1

#define PKI_POOL_PER_PORT_FLAG_ILK                    0
#define PKI_AURA_PER_PORT_FLAG_ILK                    0
#define PKI_SSO_GRP_PER_PORT_FLAG_ILK                 0

#define PKI_QPG_QOS_ILK                               CVMX_PKI_QPG_QOS_NONE
#define PKI_POOL_PER_QOS_FLAG_ILK                     0
#define PKI_AURA_PER_QOS_FLAG_ILK                     0
#define PKI_SSO_GRP_PER_QOS_FLAG_ILK                  0

/*if qos scheduling enabled */
#define PKI_SHARE_AURA_QOS_FLAG_ILK                   0
#define PKI_SHARE_SSO_GRP_QOS_FLAG_ILK                0


/* NPI interface ----------------------------------------------------------------------------*/
#define PKI_STYLE_PER_INTF_FLAG_NPI                   1
#define PKI_POOL_PER_INTF_FLAG_NPI                    0
#define PKI_AURA_PER_INTF_FLAG_NPI                    0
#define PKI_SSO_GRP_PER_INTF_FLAG_NPI                 0

#define PKI_POOL_PER_PORT_FLAG_NPI                    0
#define PKI_AURA_PER_PORT_FLAG_NPI                    0
#define PKI_SSO_GRP_PER_PORT_FLAG_NPI                 0

#define  PKI_QPG_QOS_NPI                              CVMX_PKI_QPG_QOS_NONE
/*if qos scheduling enabled */
#define PKI_SHARE_AURA_QOS_FLAG_NPI                   0
#define PKI_SHARE_SSO_GRP_QOS_FLAG_NPI                0
#define PKI_POOL_PER_QOS_FLAG_NPI                     0
#define PKI_AURA_PER_QOS_FLAG_NPI                     0
#define PKI_SSO_GRP_PER_QOS_FLAG_NPI                  0

/* LOOP interface -----------------------------------------------------------------------------*/
#define PKI_STYLE_PER_INTF_FLAG_LOOP                   1
#define PKI_POOL_PER_INTF_FLAG_LOOP                    0
#define PKI_AURA_PER_INTF_FLAG_LOOP                    0
#define PKI_SSO_GRP_PER_INTF_FLAG_LOOP                 0

#define PKI_POOL_PER_PORT_FLAG_LOOP                    0
#define PKI_AURA_PER_PORT_FLAG_LOOP                    0
#define PKI_SSO_GRP_PER_PORT_FLAG_LOOP                 0

#define  PKI_QPG_QOS_LOOP                              CVMX_PKI_QPG_QOS_NONE
/*if qos scheduling enabled */
#define PKI_SHARE_AURA_QOS_FLAG_LOOP                   0
#define PKI_SHARE_SSO_GRP_QOS_FLAG_LOOP                0
#define PKI_POOL_PER_QOS_FLAG_LOOP                     0
#define PKI_AURA_PER_QOS_FLAG_LOOP                     0
#define PKI_SSO_GRP_PER_QOS_FLAG_LOOP                  0

/* All interface and ports will use same configuration for now */
/* Make sure to modify these counts if many auras are associated
 with 1 pool. Below combination works only for 1:1 Aura:Pool */
#define PKI_INTF_POOL_MAX_BUFFERS       6000
#define PKI_INTF_POOL_BUFFER_SIZE       2048
#define PKI_INTF_AURA_BUFFER_COUNT      6000
#define PKI_PORT_POOL_MAX_BUFFERS       6000
#define PKI_PORT_POOL_BUFFER_SIZE       2048
#define PKI_PORT_AURA_BUFFER_COUNT      6000
#define PKI_QOS_POOL_MAX_BUFFERS        6000
#define PKI_QOS_POOL_BUFFER_SIZE        2048
#define PKI_QOS_AURA_BUFFER_COUNT       6000
#define PKI_QOS_PORT_ADD_INC            0

int application_init_packet_input(int node, int num_buffers);
void application_setup_pki_ports(int node);
