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
 * packets over IEEE802.3/Ethernet. The format of thse packets is dictated by
 * Annex F, Transport of PTP over IEEE 802.3/Ethernet, in the IEEE 1588-2008
 * standard.
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ptp-packets.h"
#include "timestamp.h"
#include "packetio.h"

#define PTP_ETHERTYPE   0x88f7          /* Layer 2 ethernet type for PTP */
#define PTP_DEST_MAC    0x011B19000000ull /* Dest MAC address for all PTP messages except peer delay */
#define PTP_PD_DEST_MAC 0x0180C200000Eull /* Dest MAC address for peer delay messages */

/**
 * Get the Linux interface index.
 *
 * @param packetio Packetio we are working with
 *
 * @return Index or negative on failure
 */
static int raw_get_index(packetio_t *packetio)
{
    struct ifreq request;
    memset(&request, 0, sizeof(request));
    strcpy(request.ifr_ifrn.ifrn_name, packetio->name);

    /* Perform an ioctl to get the hardware MAC address */
    if (ioctl(packetio->sock_msg, SIOCGIFINDEX, &request))
    {
        perror("packetio:ioctl(SIOCGIFINDEX)");
        return -1;
    }
    return request.ifr_ifru.ifru_ivalue;
}


/**
 * Send a PTP packet
 *
 * @param packet   Packet to send
 * @param addr     Optional address to send the packet to. If this is NULL, the
 *                 multicast address is determined based in the Annex F
 *                 specification. If specified, this is a socketaddr_ll structure
 *                 containing the destination MAC address.
 * @param is_event True if this packet is an event and needs timestamp on send
 *
 * @return Zero on success, negative on failure. The packet timestamp
 *         is updated or set to TIMESTAMP_BAD on failure.
 */
static int raw_send(packetio_packet_t *packet, const char *addr, int is_event)
{
    struct __attribute__ ((__packed__))
    {
        uint8_t dest[6];
        uint8_t src[6];
        uint16_t type;
        char buffer[1024];
    } l2_packet;

    if (addr)
    {
        const struct sockaddr_ll *lladdr = (const struct sockaddr_ll *)addr;
        memcpy(l2_packet.dest, lladdr->sll_addr, 6);
    }
    else
    {
        uint64_t dest;
        if ((packet->header->messageType == PTP_MESSAGETYPE_PDELAY_REQ) ||
            (packet->header->messageType == PTP_MESSAGETYPE_PDELAY_RESP) ||
            (packet->header->messageType == PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP))
            dest = PTP_PD_DEST_MAC;
        else
            dest = PTP_DEST_MAC;

        l2_packet.dest[0] = dest >> 40;
        l2_packet.dest[1] = dest >> 32;
        l2_packet.dest[2] = dest >> 24;
        l2_packet.dest[3] = dest >> 16;
        l2_packet.dest[4] = dest >> 8;
        l2_packet.dest[5] = dest >> 0;
    }
    l2_packet.src[0] = packet->packetio->mac >> 40;
    l2_packet.src[1] = packet->packetio->mac >> 32;
    l2_packet.src[2] = packet->packetio->mac >> 24;
    l2_packet.src[3] = packet->packetio->mac >> 16;
    l2_packet.src[4] = packet->packetio->mac >> 8;
    l2_packet.src[5] = packet->packetio->mac >> 0;
    l2_packet.type = htons(PTP_ETHERTYPE);
    memcpy(l2_packet.buffer, packet->header, packet->length);

    int send_socket = (is_event) ? packet->packetio->sock_event : packet->packetio->sock_msg;
    int bytes = send(send_socket, &l2_packet, 14 + packet->length,
        MSG_DONTWAIT);
    packet->timestamp = timestamp_get_systime(1);

    if (bytes != 14 + packet->length)
    {
        perror("packetio:sendto()");
        packet->timestamp = TIMESTAMP_BAD;
        return -1;
    }

    // FIXME: TX timestamps aren't supported by the kernel on raw sockets
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
 * @param is_event True if the packet should be read from the timed event socket
 * @param flags    Socket flags for the read
 *
 * @return Zero on success, negative on failure
 */
static int raw_receive(packetio_t *packetio, packetio_packet_t *packet, int is_event, int flags)
{
    int result = packetio_receive_internal(packetio, packet, is_event, 14, flags);
    if (flags & MSG_ERRQUEUE)
    {
        /* The socket RX info isn't useful for the error queue. Make it be the
            destination of the original message */
        if (result == 0)
        {
            memset(packet->rxinfo, 0, sizeof(packet->rxinfo));
            struct sockaddr_ll *lladdr = (struct sockaddr_ll *)packet->rxinfo;
            lladdr->sll_family = AF_PACKET;
            lladdr->sll_protocol = htons(PTP_ETHERTYPE);
            lladdr->sll_ifindex = 0;
            lladdr->sll_hatype = ARPHRD_ETHER;
            lladdr->sll_pkttype = PACKET_OUTGOING;
            lladdr->sll_halen = 6;
            memcpy(lladdr->sll_addr, packet->buffer, 6);
        }
    }
    else if (result == 0)
    {
        /* Unfortunately we need to use promiscious mode to get the PTP
            multicast messages. This gives us more than we need, so here
            we filter out messages based on the MAC address */
        uint64_t dest = ntohq(*(uint64_t*)packet->buffer) >> 16;
        if ((dest != PTP_DEST_MAC) && (dest != PTP_PD_DEST_MAC) &&
            (dest != packetio->mac))
            return -1;
    }
    return result;
}


/**
 * Initialaize an interface for raw ethernet packets containing
 * PTPv2 packets.
 *
 * @param packetio  Packetio interface being updated
 *
 * @return Zero on success, negative on failure
 */
int packetio_raw_initialize(packetio_t *packetio)
{
    packetio->physicalLayerProtocol = "IEEE802.3";
    packetio->networkProtocol = 3;
    packetio->protocolAddressLen = 6;
    packetio->protocolAddress[0] = packetio->mac;

    /* Create a socket for doing IO on */
    packetio->sock_msg = socket(PF_PACKET, SOCK_RAW, htons(0));
    if (packetio->sock_msg < 0)
    {
        perror("packetio:socket()");
        return -1;
    }
    packetio->sock_event = socket(PF_PACKET, SOCK_RAW, htons(PTP_ETHERTYPE));
    if (packetio->sock_event < 0)
    {
        perror("packetio:socket()");
        close(packetio->sock_msg);
        return -1;
    }

    /* Set promisc mode */
    struct ifreq request;
    memset(&request, 0, sizeof(request));
    strcpy(request.ifr_ifrn.ifrn_name, packetio->name);
    if (ioctl(packetio->sock_event, SIOCGIFFLAGS, &request))
    {
        perror("packetio:ioctl(SIOCGIFFLAGS)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }
    request.ifr_ifru.ifru_flags |= IFF_PROMISC;
    if (ioctl(packetio->sock_event, SIOCSIFFLAGS, &request))
    {
        perror("packetio:ioctl(SIOCSIFFLAGS)");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    /* Bind to specific interface */
    struct sockaddr_ll addr;
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(0);
    addr.sll_ifindex = raw_get_index(packetio);
    if (bind(packetio->sock_msg, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll)) < 0)
    {
        perror("packetio: bind() to interface");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }
    addr.sll_protocol = htons(PTP_ETHERTYPE);
    if (bind(packetio->sock_event, (struct sockaddr *)&addr, sizeof(struct sockaddr_ll)) < 0)
    {
        perror("packetio: bind() to interface");
        close(packetio->sock_msg);
        close(packetio->sock_event);
        return -1;
    }

    packetio->send = raw_send;
    packetio->receive = raw_receive;
    return 0;
}

