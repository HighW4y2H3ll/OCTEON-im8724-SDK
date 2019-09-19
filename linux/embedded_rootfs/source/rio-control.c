/****************************************************************
 * Copyright (c) 2009-2010, Cavium Inc. (support@cavium.com). All rights
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
 * @file rio-control.c
 *
 * "rio-control" is a utility for manipulation Serial Rapid IO
 * packet interface parameters. This utility provides comamnd
 * line access to the IOCTLs exposed by the Cavium ethernet
 * driver for Serial Rapid IO mailboxes. See the Octeon SDK
 * documentation for more information.
 *
 * $Id: oct-linux-csr.c 30001 2007-11-08 23:26:43Z kreese $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if.h>
#include <arpa/inet.h>

#include "asm/octeon/octeon-ethernet-user.h"

int do_ioctl(const char *netdev, const char *name, int cmd, int is_write, int value)
{
    struct ifreq request;

    memset(&request, 0, sizeof(request));
    strncpy(request.ifr_ifrn.ifrn_name, netdev,
        sizeof(request.ifr_ifrn.ifrn_name));
    request.ifr_ifru.ifru_ivalue = value;

    int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }

    int status = ioctl(sock, cmd, &request);
    if (status < 0)
    {
        perror("ioctl");
        close(sock);
        return -1;
    }
    close(sock);

    if (is_write)
        return 0;

    printf("%s: %s = %d\n", netdev, name, request.ifr_ifru.ifru_ivalue);
    return 0;
}

int main(int argc, const char *argv[])
{
    int cmd;
    const char *netdev  = (argc>1) ? argv[1] : NULL;
    const char *name    = (argc>2) ? argv[2] : NULL;
    int value = (argc>3) ? atoi(argv[3]) : 0;

    if (!netdev || !name)
        goto usage;

    if (strcmp(name, "priority") == 0)
    {
        cmd = (argc>3) ? CAVIUM_NET_IOCTL_SETPRIO : CAVIUM_NET_IOCTL_GETPRIO;
    }
    else if (strcmp(name, "idsize") == 0)
    {
        cmd = (argc>3) ? CAVIUM_NET_IOCTL_SETIDSIZE : CAVIUM_NET_IOCTL_GETIDSIZE;
    }
    else if (strcmp(name, "source") == 0)
    {
        cmd = (argc>3) ? CAVIUM_NET_IOCTL_SETSRCID : CAVIUM_NET_IOCTL_GETSRCID;
    }
    else if (strcmp(name, "letter") == 0)
    {
        cmd = (argc>3) ? CAVIUM_NET_IOCTL_SETLETTER : CAVIUM_NET_IOCTL_GETLETTER;
    }
    else
        goto usage;

    return do_ioctl(netdev, name, cmd, (argc > 3), value);

usage:
    printf("\n"
           "Usage: rio-control rio# <name> [value]\n"
           "    Read or write Rapid IO network parameters.\n"
           "\n"
           "    rio#    Network interface to access.\n"
           "    name    Parameter to read/write. Support names are:\n"
           "            priority    Packet priority (0 - 3)\n"
           "            idsize      Device ID size (0 = 8bit, 1 = 16bit)\n"
           "            source      Which source ID to use (0 = primary, 1 = secondary)\n"
           "            letter      Message letter code (0 - 3, or -1 for automatic)\n"
           "    value   If present, write this value. Otherwise do a read.\n"
           "\n");
    return 1;
}
