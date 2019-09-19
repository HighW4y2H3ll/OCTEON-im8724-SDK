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
 * packets over UDP. The format of thse packets is dictated by Annex D,
 * Transport of PTP over User Datagram Protocol over Internet Protocol Version
 * 4, in the IEEE 1588-2008 standard.
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
#include "ptp-packets.h"
#include "timestamp.h"
#include "packetio.h"

#define PTP_EVENT_UDP_PORT  319
#define PTP_MSG_UDP_PORT    320

struct sockaddr_in6 ptp_ip_address;
struct sockaddr_in6 ptp_peer_ip_address;

/**
 * Send a PTP packet
 *
 * @param packet   Packet to send
 * @param addr     Optional address to send the packet to. If this is NULL, the
 *                 multicast address is determined based in the Annex D
 *                 specification. If specified, this is a socketaddr_in structure
 *                 containing the destination IP address and port.
 * @param is_event True if the packet is a timed event
 *
 * @return Zero on success, negative on failure. The packet timestamp
 *         is updated or set to TIMESTAMP_BAD on failure.
 */
static int udp6_send(packetio_packet_t *packet, const char *addr, int is_event)
{
    struct sockaddr_in6 inaddr;

    if (addr)
    {
        memcpy(&inaddr, addr, sizeof(inaddr));
        if ((packet->header->messageType != PTP_MESSAGETYPE_SIGNALING) &&
            (packet->header->messageType != PTP_MESSAGETYPE_MANAGEMENT))
        {
            /* Since 1588 uses two different ports for event and messages, we need
                to make sure we respond to the correct port regardless of which
                port asked for a message */
            inaddr.sin6_port = htons((is_event) ? PTP_EVENT_UDP_PORT : PTP_MSG_UDP_PORT);
        }
    }
    else
    {
        if ((packet->header->messageType == PTP_MESSAGETYPE_PDELAY_REQ) ||
            (packet->header->messageType == PTP_MESSAGETYPE_PDELAY_RESP) ||
            (packet->header->messageType == PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP))
            inaddr = ptp_peer_ip_address;
        else
            inaddr = ptp_ip_address;
        inaddr.sin6_port = htons((is_event) ? PTP_EVENT_UDP_PORT : PTP_MSG_UDP_PORT);
    }

    int send_socket = (is_event) ? packet->packetio->sock_event : packet->packetio->sock_msg;
    int bytes = sendto(send_socket, packet->buffer, packet->length,
        MSG_DONTWAIT, (struct sockaddr *)&inaddr, sizeof(inaddr));
    /* The hardware timestamp will show up later if it is supported. Use
        software as a guess for now */
    packet->timestamp = timestamp_get_systime(1);

    if (bytes != packet->length)
    {
        perror("packetio:sendto()");
        packet->timestamp = TIMESTAMP_BAD;
        return -1;
    }

    return 0;
}


/**
 * Receive a packet from the underlying transport. The packet's
 * header and data pointers are set appropriately. Source
 * information is stored in the rxinfo field of the packet and
 * the timestamp is updated.
 *
 * @param packetio Interface to receive on
 * @param packet   Area to store the packet
 * @param is_event True if this packet is from a timed event socket
 * @param flags    Socket flags for the receive
 *
 * @return Zero on success, negative on failure
 */
static int udp6_receive(packetio_t *packetio, packetio_packet_t *packet, int is_event, int flags)
{
    if (flags & MSG_ERRQUEUE)
    {
        int result = packetio_receive_internal(packetio, packet, is_event, 14+40+8, flags);
        /* The socket RX info isn't useful for the error queue. Make it be the
            destination of the original message */
        if (result == 0)
        {
            memset(packet->rxinfo, 0, sizeof(packet->rxinfo));
            struct sockaddr_in6 *inaddr = (struct sockaddr_in6 *)packet->rxinfo;
            inaddr->sin6_family = AF_INET6;
            inaddr->sin6_port = *(uint16_t*)(packet->buffer + 14 + 40 + 2);
            memcpy(inaddr->sin6_addr.s6_addr32, packet->buffer + 14 + 24, 16);
        }
        return result;
    }
    else
        return packetio_receive_internal(packetio, packet, is_event, 0, flags);
}


/**
 * Initialaize an interface for UDP packets containing
 * PTPv2 packets.
 *
 * @param packetio Packetio interface being updated
 *
 * @return Zero on success, negative on failure
 */
int packetio_udp6_initialize(packetio_t *packetio)
{
    packetio->physicalLayerProtocol = "UDP/IPv6";
    packetio->networkProtocol = 2;
    packetio->protocolAddressLen = 16;
    // FIXME: packetio->protocolAddress

    /* Convert address strings to structures */
    memset(&ptp_ip_address, 0, sizeof(ptp_ip_address));
    ptp_ip_address.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, "FF0E:0:0:0:0:0:0:181", &ptp_ip_address.sin6_addr) != 1)
    {
        perror("packetio:inet_pton(PTP_IP_ADDRESS)");
        return -1;
    }
    memset(&ptp_peer_ip_address, 0, sizeof(ptp_peer_ip_address));
    ptp_peer_ip_address.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, "FF02:0:0:0:0:0:0:6B", &ptp_peer_ip_address.sin6_addr) != 1)
    {
        perror("packetio:inet_pton(PTP_PEER_IP_ADDRESS)");
        return -1;
    }

    /* Create a socket for doing IO on */
    packetio->sock_msg = socket(PF_INET6, SOCK_DGRAM, 0);
    if (packetio->sock_msg < 0)
    {
        perror("packetio:socket()");
        return -1;
    }
    packetio->sock_event = socket(PF_INET6, SOCK_DGRAM, 0);
    if (packetio->sock_event < 0)
    {
        perror("packetio:socket()");
        close(packetio->sock_msg);
        return -1;
    }

    /* Bind to PTP port */
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_addr = in6addr_any;
    addr.sin6_port = htons(PTP_MSG_UDP_PORT);
    if (bind(packetio->sock_msg, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        // FIXME: Bind fails if we are using multiple ports. We should
        // handle this better...
    }
    addr.sin6_port = htons(PTP_EVENT_UDP_PORT);
    if (bind(packetio->sock_event, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        // FIXME: Bind fails if we are using multiple ports. We should
        // handle this better...
    }

    /* join multicast group for normal messages */
    struct ipv6_mreq imr;
    memset(&imr, 0, sizeof(imr));
    imr.ipv6mr_interface = packetio->ifnum;
    imr.ipv6mr_multiaddr = ptp_ip_address.sin6_addr;
    if (setsockopt(packetio->sock_msg, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                   &imr, sizeof(imr)) < 0)
    {
        perror("packetio:setsockopt(IPV6_ADD_MEMBERSHIP)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }
    if (setsockopt(packetio->sock_event, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                   &imr, sizeof(imr)) < 0)
    {
        perror("packetio:setsockopt(IPV6_ADD_MEMBERSHIP)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    /* join multicast group for peer messages */
    memset(&imr, 0, sizeof(imr));
    imr.ipv6mr_interface = packetio->ifnum;
    imr.ipv6mr_multiaddr = ptp_peer_ip_address.sin6_addr;
    if (setsockopt(packetio->sock_event, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                   &imr, sizeof(imr)) < 0)
    {
        perror("packetio:setsockopt(IPV6_ADD_MEMBERSHIP)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    /* set multicast group for outgoing packets */
    if (setsockopt(packetio->sock_msg, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                   &packetio->ifnum, sizeof(int)) < 0)
    {
        perror("packetio:setsockopt(IPV6_MULTICAST_IF)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }
    if (setsockopt(packetio->sock_event, IPPROTO_IPV6, IPV6_MULTICAST_IF,
        &packetio->ifnum, sizeof(int)) < 0)
    {
        perror("packetio:setsockopt(IPV6_MULTICAST_IF)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    /* Disable multicast loopback */
    int value = 0;
    if (setsockopt(packetio->sock_msg, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                   &value, sizeof(value)) < 0)
    {
        perror("packetio:setsockopt(IPV6_MULTICAST_LOOP)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }
    if (setsockopt(packetio->sock_event, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                   &value, sizeof(value)) < 0)
    {
        perror("packetio:setsockopt(IPV6_MULTICAST_LOOP)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    packetio->send = udp6_send;
    packetio->receive = udp6_receive;
    return 0;
}

