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

#ifndef __PACKETIO_H__
#define __PACKETIO_H__
/**
 * @file
 * This file provides a transport mechanism for sending and receiving PTPv2
 * packets over various transports. The format of thse packets is dictated by
 * the IEEE 1588-2008 standard.
 */

#ifndef IFNAMSIZ
#define	IFNAMSIZ 16
#endif

struct packetio_s;

typedef enum
{
    PACKETIO_FLAG_NONE = 0,
    PACKETIO_FLAG_TX_TIMESTAMP = 1<<0,
} packetio_flag_t;

/**
 * For every packet, this structure contains all the info known about
 * the packet.
 */
typedef struct
{
    timestamp_t timestamp;  /* The time the packet was sent/received at */
    const ptp_header_t *header; /* Pointer to the PTP header (inside buffer) */
    const void *data;       /* Pointer to PTP data after the header (inside buffer) */
    int length;             /* Total length of the packet */
    char buffer[1024];      /* Raw packet data */
    struct packetio_s *packetio; /* Which packetio this packet is associated with */
    char rxinfo[64];        /* Received addressing info */
} packetio_packet_t;

/**
 * This structure encapsulates all knowledge about a network
 * interface. Each packet will have one of these attached to
 * it.
 */
typedef struct packetio_s
{
    char name[IFNAMSIZ];    /* Name of the interface */
    uint64_t mac;           /* Hardware MAC addrss of this interface */
    int ifnum;              /* Linux interface number */
    const char *physicalLayerProtocol; /* physicalLayerProtocol string */
    int networkProtocol;    /* PTP networkProtocol number */
    int protocolAddressLen; /* Length of the protocol Address */
    uint64_t protocolAddress[2]; /* The protocol address */
    int sock_event;         /* Socket used to transfer event messages requiring timestamps */
    int sock_msg;           /* Socket used to transfer general messages not requiring timestamps */
    packetio_flag_t flags;  /* Flag information about the interface */
    int (*send)(packetio_packet_t *packet, const char *addr, int is_event);
    int (*receive)(struct packetio_s *packetio, packetio_packet_t *packet, int is_event, int flags);
} packetio_t;

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
extern int packetio_initialize(const char *interface, const char *transport, packetio_t *packetio);

/**
 * Shutdown a packet interface
 *
 * @param packetio Packetio to shutdown
 *
 * @return Zero on success, negative on failure.
 */
extern int packetio_shutdown(packetio_t *packetio);

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
extern int packetio_send(packetio_packet_t *packet, const char *addr);

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
extern int packetio_receive_event(packetio_t *packetio, packetio_packet_t *packet, int flags);

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
extern int packetio_receive_msg(packetio_t *packetio, packetio_packet_t *packet);

/**
 * Display a PTPv2 packet
 *
 * @param label  String label to display along with the packet.
 * @param packet Packet to display
 */
extern void packetio_display(const char *label, const packetio_packet_t *packet);

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
extern int packetio_receive_internal(packetio_t *packetio, packetio_packet_t *packet, int is_event, int header_offset, int flags);

#endif
