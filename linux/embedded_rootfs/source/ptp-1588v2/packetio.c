/***********************license start************************************
 * OCTEON SDK
 *
 * Copyright (c) 2009-2010 Cavium Inc. All rights reserved.
 *
 * This file, which is part of the OCTEON SDK from Cavium Inc. 
 * contains proprietary and confidential information of Cavium Inc. and
 * its suppliers.
 *
 * Any licensed reproduction, distribution, modification, or other use of
 * this file or the confidential information or patented inventions
 * embodied in this file is subject to your license agreement with Cavium
 * Inc.. Unless you and Cavium Inc. have agreed otherwise in
 * writing, the applicable license terms can be found at:
 * licenses/cavium-license-type2.txt
 *
 * All other use and disclosure is prohibited.
 *
 * Contact Cavium Inc. at info@cavium.com for more information.
 **********************license end**************************************/

/**
 * @file
 * This file provides a transport mechanism for sending and receiving PTPv2
 * packets over various transports. The format of thse packets is dictated by
 * the IEEE 1588-2008 standard.
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/net_tstamp.h>
#include <errno.h>
#include "ptp-packets.h"
#include "timestamp.h"
#include "packetio.h"

/* The kernel features we are using are fairly new. Define the ioctl
    defines if they aren't present in the toolchain */
#ifndef SIOCSHWTSTAMP
#define SIOCSHWTSTAMP   0x89b0
#endif
#ifndef SO_TIMESTAMPING
#define SO_TIMESTAMPING 37
#endif

/**
 * Get the hardware MAC address of the interface
 *
 * @param packetio Packetio to update
 *
 * @return Zero on success, negative on failure
 */
static int packetio_get_hwaddr(packetio_t *packetio)
{
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct ifreq request;
    memset(&request, 0, sizeof(request));
    strcpy(request.ifr_ifrn.ifrn_name, packetio->name);

    /* Perform an ioctl to get the interface index */
    if (ioctl(sock, SIOCGIFINDEX, &request))
    {
        perror("packetio:ioctl(SIOCGIFHWADDR)");
        close(sock);
        return -1;
    }
    packetio->ifnum = request.ifr_ifru.ifru_ivalue;

    /* Perform an ioctl to get the hardware MAC address */
    if (ioctl(sock, SIOCGIFHWADDR, &request))
    {
        perror("packetio:ioctl(SIOCGIFHWADDR)");
        close(sock);
        return -1;
    }
    packetio->mac = 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[0];
    packetio->mac <<= 8;
    packetio->mac |= 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[1];
    packetio->mac <<= 8;
    packetio->mac |= 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[2];
    packetio->mac <<= 8;
    packetio->mac |= 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[3];
    packetio->mac <<= 8;
    packetio->mac |= 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[4];
    packetio->mac <<= 8;
    packetio->mac |= 0xff & request.ifr_ifru.ifru_hwaddr.sa_data[5];
    close(sock);
    return 0;
}


/**
 * Enable timestamps
 *
 * @param packetio Packetio to update
 *
 * @return Zero on success, negative on failure
 */
static int packetio_enable_timestamp(packetio_t *packetio)
{
    struct ifreq request;
    struct hwtstamp_config hwconfig;
    int value;

    memset(&request, 0, sizeof(request));
    strcpy(request.ifr_ifrn.ifrn_name, packetio->name);

    /* Perform an ioctl to enable hardware timestamps */
    memset(&hwconfig, 0, sizeof(hwconfig));
    hwconfig.tx_type = (packetio->flags & PACKETIO_FLAG_TX_TIMESTAMP) ? HWTSTAMP_TX_ON : HWTSTAMP_TX_OFF;
    hwconfig.rx_filter = HWTSTAMP_FILTER_ALL;
    request.ifr_data = (void *)&hwconfig;

    /* Turn off TX timestamp flag until we verify it was set */
    packetio->flags &= ~PACKETIO_FLAG_TX_TIMESTAMP;

    if (ioctl(packetio->sock_event, SIOCSHWTSTAMP, &request))
    {
        printf("%s: Interface does not support hardware timestamps\n", packetio->name);
    }
    else if (hwconfig.tx_type == HWTSTAMP_TX_ON)
    {
        packetio->flags |= PACKETIO_FLAG_TX_TIMESTAMP;
        printf("%s: Using extended transmit timestamps\n", packetio->name);
    }

    /* Set the socket options to enable hardware timestamps */
    value = SOF_TIMESTAMPING_TX_HARDWARE | SOF_TIMESTAMPING_TX_SOFTWARE |
                SOF_TIMESTAMPING_RX_HARDWARE | SOF_TIMESTAMPING_RX_SOFTWARE |
                SOF_TIMESTAMPING_RAW_HARDWARE | SOF_TIMESTAMPING_SYS_HARDWARE |
                SOF_TIMESTAMPING_SOFTWARE;
    if (setsockopt(packetio->sock_event, SOL_SOCKET, SO_TIMESTAMPING, &value, sizeof(value)))
    {
        value = 1;
        if (setsockopt(packetio->sock_event, SOL_SOCKET, SO_TIMESTAMPNS, &value, sizeof(value)))
        {
            if (setsockopt(packetio->sock_event, SOL_SOCKET, SO_TIMESTAMP, &value, sizeof(value)))
                printf("%s: Kernel does not support receive timestamps\n", packetio->name);
            else
                printf("%s: Using microsecond receive timestamps\n", packetio->name);
        }
        else
            printf("%s: Using nanosecond receive timestamps\n", packetio->name);
    }
    else
        printf("%s: Using extended receive timestamps\n", packetio->name);

    return 0;
}


/**
 * Initialaize an interface for packets containing
 * PTPv2 packets.
 *
 * @param interface Linux network device to use
 * @param transport Transport to use. Can be "802.3" or "udp".
 * @param packetio  Packetio interface being updated
 *
 * @return Zero on success, negative on failure
 */
int packetio_initialize(const char *interface, const char *transport, packetio_t *packetio)
{
    extern int packetio_raw_initialize(packetio_t *packetio);
    extern int packetio_udp_initialize(packetio_t *packetio);
    extern int packetio_udp6_initialize(packetio_t *packetio);

    /* Start off with zero state */
    memset(packetio, 0, sizeof(*packetio));
    packetio->flags = PACKETIO_FLAG_NONE;

    strncpy(packetio->name, interface, sizeof(packetio->name));
    packetio->name[sizeof(packetio->name)-1] = 0;

    if (packetio_get_hwaddr(packetio))
        return -1;

    if (strcasecmp(transport, "802.3") == 0)
    {
        if (packetio_raw_initialize(packetio))
            return -1;
    }
    else if (strcasecmp(transport, "udp") == 0)
    {
        if (packetio_udp_initialize(packetio))
            return -1;
        packetio->flags |= PACKETIO_FLAG_TX_TIMESTAMP;
    }
    else if (strcasecmp(transport, "udp6") == 0)
    {
        if (packetio_udp6_initialize(packetio))
            return -1;
        //packetio->flags |= PACKETIO_FLAG_TX_TIMESTAMP; // FIXME
    }
    else
    {
        printf("%s: Unsupported transport type \"%s\"\n", packetio->name, transport);
        return -1;
    }

    if (packetio_enable_timestamp(packetio))
    {
        close(packetio->sock_event);
        close(packetio->sock_msg);
        return -1;
    }
    return 0;
}


/**
 * Shutdown a packet interface
 *
 * @param packetio Packetio to shutdown
 *
 * @return Zero on success, negative on failure.
 */
int packetio_shutdown(packetio_t *packetio)
{
    close(packetio->sock_event);
    close(packetio->sock_msg);
    return 0;
}


/**
 * Display a PTPv2 packet
 *
 * @param label  String label to display along with the packet.
 * @param packet Packet to display
 */
void packetio_display(const char *label, const packetio_packet_t *packet)
{
    const ptp_header_t *h = packet->header;
    const char *MESSAGE_TYPE[16] = {NULL,};
    char s[TIMESTAMP_STRING_LEN];

    MESSAGE_TYPE[PTP_MESSAGETYPE_SYNC] = "SYNC";
    MESSAGE_TYPE[PTP_MESSAGETYPE_DELAY_REQ] = "DELAY_REQ";
    MESSAGE_TYPE[PTP_MESSAGETYPE_PDELAY_REQ] = "PDELAY_REQ";
    MESSAGE_TYPE[PTP_MESSAGETYPE_PDELAY_RESP] = "PDELAY_RESP";
    MESSAGE_TYPE[PTP_MESSAGETYPE_FOLLOW_UP] = "FOLLOW_UP";
    MESSAGE_TYPE[PTP_MESSAGETYPE_DELAY_RESP] = "DELAY_RESP";
    MESSAGE_TYPE[PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP] = "PDELAY_RESP_FOLLOW_UP";
    MESSAGE_TYPE[PTP_MESSAGETYPE_ANNOUNCE] = "ANNOUNCE";
    MESSAGE_TYPE[PTP_MESSAGETYPE_SIGNALING] = "SIGNALING";
    MESSAGE_TYPE[PTP_MESSAGETYPE_MANAGEMENT] = "MANAGEMENT";

    timestamp_to_string(packet->timestamp, s);
    printf("%s: PTP %s timestamp: %s\n", packet->packetio->name, label, s);
    printf("  | transportSpecific:  %d\t", 0xff & h->transportSpecific);
    printf("    messageType:        %s\n", MESSAGE_TYPE[h->messageType]);
    printf("  | versionPTP:         %d\t", 0xf & h->versionPTP);
    printf("    messageLength:      %d\n", 0xffff & ntohs(h->messageLength));
    printf("  | domainNumber:       %d\t", 0xff & h->domainNumber);
    printf("    flagField:          %d\n", 0xffff & ntohs(h->flagField));
    printf("  | sequenceId:         %d\t", 0xffff & ntohs(h->sequenceId));
    printf("    correctionField:    %llu\n", (unsigned long long)ntohq(h->correctionField));
    printf("  | controlField:       %d\t", 0xff & h->controlField);
    printf("    sourcePortIdentity: %016llx.%d\n", (unsigned long long)ntohq(h->sourcePortIdentity.clockIdentity), ntohs(h->sourcePortIdentity.portNumber));
    printf("  +-logMessageInterval: %d\t", 0xff & h->logMessageInterval);
    printf("    [%d bytes of data]\n", packet->length - (int)sizeof(*h));
}


/**
 * Internal function for receiving packets. Implementation is
 * common across all transports.
 *
 * @param packetio Packetio to receive packet on
 * @param packet   Area to receive packet
 * @param is_event Non zero if we should receive a timestamped event
 * @param header_offset
 *                 Offset in bytes to the PTPv2 header in the received packet.
 * @param flags    Optional flags to pass to recvmsg()
 *
 * @return Zero on success, negative on failure
 */
int packetio_receive_internal(packetio_t *packetio, packetio_packet_t *packet, int is_event, int header_offset, int flags)
{
    struct msghdr msg;
    struct iovec iovec;
    char control[1024];
    struct cmsghdr *cmsg;

    memset(&packet->rxinfo, 0, sizeof(packet->rxinfo));
    memset(&msg, 0, sizeof(msg));
    memset(&iovec, 0, sizeof(iovec));
    memset(&control, 0, sizeof(control));

    iovec.iov_base = packet->buffer;
    iovec.iov_len = sizeof(packet->buffer);

    msg.msg_name = &packet->rxinfo;
    msg.msg_namelen = sizeof(packet->rxinfo);
    msg.msg_iov = &iovec;
    msg.msg_iovlen = 1;
    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    packet->header = NULL;
    packet->data = NULL;
    packet->packetio = packetio;
    if (is_event)
        packet->length = recvmsg(packetio->sock_event, &msg, flags);
    else
        packet->length = recvmsg(packetio->sock_msg, &msg, flags);
    if (packet->length <= 0)
    {
        perror("packetio:recvmsg()");
        return -1;
    }

    /* Use the current time if we don't get a better timestamp */
    packet->timestamp = timestamp_get_systime();

    /* Get the receive time from the control data */
    cmsg = CMSG_FIRSTHDR(&msg);
    while (cmsg)
    {
        /* Check for an extended timestamp */
        if (cmsg->cmsg_type == SO_TIMESTAMPING)
        {
            struct timespec *software = (struct timespec *)CMSG_DATA(cmsg);
            struct timespec *hardware = software+1;
#if USE_PTP_CLOCK
            struct timespec *raw = hardware+1;
            if (raw->tv_sec || raw->tv_nsec)
            {
                packet->timestamp.nanoseconds = (uint64_t)raw->tv_sec * NSEC + raw->tv_nsec;
                packet->timestamp.fractions = 0;
            }
            else
#endif
            if (hardware->tv_sec || hardware->tv_nsec)
            {
                packet->timestamp.nanoseconds = (uint64_t)hardware->tv_sec * NSEC + hardware->tv_nsec;
                packet->timestamp.fractions = 0;
            }
            else if (software->tv_sec || software->tv_nsec)
            {
                packet->timestamp.nanoseconds = (uint64_t)software->tv_sec * NSEC + software->tv_nsec;
                packet->timestamp.fractions = 0;
            }
            else
                packet->timestamp = TIMESTAMP_BAD;
            break;
        }
        /* Check for a nanosecond timestamp */
        if (cmsg->cmsg_type == SO_TIMESTAMPNS)
        {
            struct timespec *software = (struct timespec *)CMSG_DATA(cmsg);
            if (software->tv_sec || software->tv_nsec)
            {
                packet->timestamp.nanoseconds = (uint64_t)software->tv_sec * NSEC + software->tv_nsec;
                packet->timestamp.fractions = 0;
            }
            else
                packet->timestamp = TIMESTAMP_BAD;
            break;
        }
        /* Check for a microsecond timestamp */
        if (cmsg->cmsg_type == SO_TIMESTAMP)
        {
            struct timeval *software = (struct timeval *)CMSG_DATA(cmsg);
            if (software->tv_sec || software->tv_usec)
            {
                packet->timestamp.nanoseconds = (uint64_t)software->tv_sec * NSEC + (uint64_t)software->tv_usec * 1000;
                packet->timestamp.fractions = 0;
            }
            else
                packet->timestamp = TIMESTAMP_BAD;
            break;
        }
        cmsg = CMSG_NXTHDR(&msg, cmsg);
    }

    packet->header = (ptp_header_t *)(packet->buffer + header_offset);
    packet->data = packet->header+1;

    return 0;
}


/**
 * Send a PTP packet
 *
 * @param packet Packet to send
 * @param addr   Optional address to send the packet to. If this is NULL, the
 *               multicast address is determined based on the packet's contents.
 *               If specified, this is a socketaddr structure containing the
 *               destination.
 *
 * @return Zero on success, negative on failure. The packet timestamp
 *         is updated or set to TIMESTAMP_BAD on failure.
 */
int packetio_send(packetio_packet_t *packet, const char *addr)
{
    int is_event;
    switch (packet->header->messageType)
    {
        case PTP_MESSAGETYPE_SYNC:
        case PTP_MESSAGETYPE_DELAY_REQ:
        case PTP_MESSAGETYPE_PDELAY_REQ:
        case PTP_MESSAGETYPE_PDELAY_RESP:
            is_event = 1;
            break;
        default:
            is_event = 0;
            break;
    }

    int result = packet->packetio->send(packet, addr, is_event);
    if (result == 0)
    {
        //packetio_display("Send", packet);
    }
    return result;
}


/**
 * Receive an event  packet from the underlying transport. The packet's
 * header and data pointers are set appropriately. Source
 * information is stored in the rxinfo field of the packet and
 * the timestamp is updated.
 *
 * @param packetio Interface to receive on
 * @param packet   Area to store the packet
 * @param flags    Socket receive flags
 *
 * @return Zero on success, negative on failure
 */
int packetio_receive_event(packetio_t *packetio, packetio_packet_t *packet, int flags)
{
    int result = packetio->receive(packetio, packet, 1, flags);
    if (result == 0)
    {
        //packetio_display("Receive Event", packet);
    }
    return result;
}


/**
 * Receive a message packet from the underlying transport. The packet's
 * header and data pointers are set appropriately. Source
 * information is stored in the rxinfo field of the packet and
 * the timestamp is updated.
 *
 * @param packetio Interface to receive on
 * @param packet   Area to store the packet
 *
 * @return Zero on success, negative on failure
 */
int packetio_receive_msg(packetio_t *packetio, packetio_packet_t *packet)
{
    int result = packetio->receive(packetio, packet, 0, 0);
    if (result == 0)
    {
        //packetio_display("Receive Message", packet);
    }
    return result;
}

