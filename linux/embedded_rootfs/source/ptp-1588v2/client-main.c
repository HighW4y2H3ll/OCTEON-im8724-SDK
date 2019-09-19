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
 * Main entry point of a PTP/1588v2 clock daemon for Linux.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include "ptp-packets.h"
#include "timestamp.h"
#include "packetio.h"
#include "ptplib.h"

#define PTP_DEFAULT_PRIMARY_DOMAIN      0       /* Section J.3.2 */
#define PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID 0x7f /* Section 13.3.2.11 logMessageInterval */

static void send_management(packetio_t *interface, ptp_actionField_t actionField, ptp_managementId_t managementId, int tlv_length, const ptp_management_tlv_t *tlv)
{
    static int sequenceId = 1000;
    ptp_packet_t tx_packet;
    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_MANAGEMENT;
    tx_packet.header.versionPTP         = PTP_VERSION;
    tx_packet.header.domainNumber       = PTP_DEFAULT_PRIMARY_DOMAIN;
    tx_packet.header.correctionField    = 0;
    tx_packet.header.sourcePortIdentity.clockIdentity = 0;
    tx_packet.header.sourcePortIdentity.portNumber = 1;
    tx_packet.header.sequenceId         = htons(sequenceId++);
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;

    /* Fill in the PTP management response */
    tx_packet.ptp.mgmt.targetPortIdentity.clockIdentity = -1;
    tx_packet.ptp.mgmt.targetPortIdentity.portNumber = -1;
    tx_packet.ptp.mgmt.startingBoundaryHops = 1;
    tx_packet.ptp.mgmt.boundaryHops = 0;
    tx_packet.ptp.mgmt.actionField = actionField;

    tx_packet.ptp.mgmt.managementTLV[0].tlvType = htons(PTP_TLVTYPE_MANAGEMENT);
    tx_packet.ptp.mgmt.managementTLV[0].lengthField = htons(2);
    if (tlv)
    {
        tlv_length += 2;
        memcpy(tx_packet.ptp.mgmt.managementTLV[0].valueField, tlv, sizeof(ptp_tlv_t) + tlv_length);
        tx_packet.ptp.mgmt.managementTLV[0].lengthField = htons(tlv_length);
    }

    ptp_management_tlv_t *mgmt_request = (ptp_management_tlv_t *)tx_packet.ptp.mgmt.managementTLV[0].valueField;
    mgmt_request->managementId = htons(managementId);

    tx_packet.header.messageLength = htons(sizeof(ptp_header_t) + sizeof(ptp_message_management_t) + sizeof(ptp_tlv_t) + ntohs(tx_packet.ptp.mgmt.managementTLV[0].lengthField));

    packetio_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.packetio = interface;
    packet.length = ntohs(tx_packet.header.messageLength);
    memcpy(packet.buffer, &tx_packet, packet.length);
    packet.header = (ptp_header_t *)packet.buffer;
    packet.data = packet.header + 1;

    if (packetio_send(&packet, NULL))
        printf("Failed to send packet\n");
}

static void process_management(const packetio_packet_t *packet)
{
    packetio_display("Management response", packet);
}

static void do_management(packetio_t *interface, ptp_actionField_t actionField, ptp_managementId_t managementId, int tlv_length, const ptp_management_tlv_t *tlv)
{
    send_management(interface, actionField, managementId, tlv_length, tlv);

    while (1)
    {
        /* Read the response */
        struct pollfd poll_data[1];
        poll_data[0].fd = interface->sock_msg;
        poll_data[0].revents = 0;
        poll_data[0].events = POLLIN;

        /* Wait for the sockets to need service */
        int status = poll(poll_data, 1, 1000);
        if (status <= 0)
        {
            if (status < 0)
                perror("poll");
            return;
        }

        if (poll_data[0].revents & POLLIN)
        {
            packetio_packet_t packet;
            if (packetio_receive_msg(interface, &packet) == 0)
                process_management(&packet);
        }
        else
        {
            perror("poll with no data");
            return;
        }
    }
}

/**
 * Main entry point
 *
 * @param argc   Number of arguments
 * @param argv   Our arguments
 *
 * @return Zero on success, non zero on failure
 */
int main(int argc, const char *argv[])
{
    ptplib_state_t ptp;
    packetio_t interface_list[1];

    /* Check that we have the proper number of arguments */
    if ((argc < 2) || (argc > 32))
    {
        printf("Usage:\n"
               "%s interface[:transport] request\n"
               "\n"
               "    interface = Linux network device name (eth0, eth1, etc).\n"
               "    transport = Which transport to use (802.3, UDP). Defaults to 802.3.\n"
               "\n", argv[0]);
        return -1;
    }

    /* Initialize ptplib */
    if (ptplib_initialize(&ptp, PTPLIB_FLAGS_USE_MULTICAST | PTPLIB_FLAGS_USE_UNICAST))
        return -1;

    /* Add packet interface */
    {
        packetio_t *packetio = interface_list;
        char interface[32];
        char *options;
        char *transport;

        strncpy(interface, argv[1], sizeof(interface));
        interface[sizeof(interface)-1] = 0;
        options = strchr(interface, ':');
        if (options)
        {
            *options = 0;
            transport = options+1;
        }
        else
            transport = "802.3";

        if (packetio_initialize(interface, transport, packetio))
            return -1;
        if (ptplib_interface_add(&ptp, packetio))
            return -1;
    }

    char buffer[1024];
    ptp_management_tlv_t *tlv = (ptp_management_tlv_t *)buffer;

    /* PTP_MANAGEMENTID_NULL_MANAGEMENT */
    do_management(interface_list, PTP_ACTIONFIELD_COMMAND, PTP_MANAGEMENTID_NULL_MANAGEMENT, 0, NULL);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_NULL_MANAGEMENT, 0, NULL);
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_NULL_MANAGEMENT, 0, NULL);

    /* PTP_MANAGEMENTID_CLOCK_DESCRIPTION */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_CLOCK_DESCRIPTION, 0, NULL);

    /* PTP_MANAGEMENTID_USER_DESCRIPTION */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_USER_DESCRIPTION, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    const char *message = "New description";
    tlv->dataField.user_description.lengthField = strlen(message);
    strcpy((char*)tlv->dataField.user_description.textField, message);
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_USER_DESCRIPTION, 1 + strlen(message), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_USER_DESCRIPTION, 0, NULL);

    /* PTP_MANAGEMENTID_DEFAULT_DATA_SET */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_DEFAULT_DATA_SET, 0, NULL);

    /* PTP_MANAGEMENTID_CURRENT_DATA_SET */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_CURRENT_DATA_SET, 0, NULL);

    /* PTP_MANAGEMENTID_PARENT_DATA_SET */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PARENT_DATA_SET, 0, NULL);

    /* PTP_MANAGEMENTID_TIME_PROPERTIES_DATA_SET */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TIME_PROPERTIES_DATA_SET, 0, NULL);

    /* PTP_MANAGEMENTID_PORT_DATA_SET */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PORT_DATA_SET, 0, NULL);

    /* PTP_MANAGEMENTID_PRIORITY1 */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PRIORITY1, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.priority1.priority1 = 3;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_PRIORITY1, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PRIORITY1, 0, NULL);

    /* PTP_MANAGEMENTID_PRIORITY2 */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PRIORITY2, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.priority2.priority2 = 4;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_PRIORITY2, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PRIORITY2, 0, NULL);

    /* PTP_MANAGEMENTID_DOMAIN */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_DOMAIN, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.domain.domain = 0;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_DOMAIN, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_DOMAIN, 0, NULL);

    /* PTP_MANAGEMENTID_SLAVE_ONLY */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_SLAVE_ONLY, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.slave_only.slave_only = 1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_SLAVE_ONLY, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_SLAVE_ONLY, 0, NULL);

    /* PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.log_announce_interval[0].logAnnounceInterval = 2;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL, 0, NULL);

    /* PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.announce_receipt_timeout[0].announceReceiptTimeout = 5;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT, 0, NULL);

    /* PTP_MANAGEMENTID_LOG_SYNC_INTERVAL */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_SYNC_INTERVAL, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.log_sync_interval[0].logSyncInterval = -1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_LOG_SYNC_INTERVAL, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_SYNC_INTERVAL, 0, NULL);

    /* PTP_MANAGEMENTID_VERSION_NUMBER */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_VERSION_NUMBER, 0, NULL);

    /* PTP_MANAGEMENTID_DISABLE_PORT */
    do_management(interface_list, PTP_ACTIONFIELD_COMMAND, PTP_MANAGEMENTID_DISABLE_PORT, 0, NULL);

    /* PTP_MANAGEMENTID_ENABLE_PORT */
    do_management(interface_list, PTP_ACTIONFIELD_COMMAND, PTP_MANAGEMENTID_ENABLE_PORT, 0, NULL);

    /* PTP_MANAGEMENTID_TIME */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TIME, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.time.currentTime.secondsField = 0;
    tlv->dataField.time.currentTime.nanosecondsField = htonl(2222);
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_TIME, sizeof(tlv->dataField.time.currentTime), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TIME, 0, NULL);

    /* PTP_MANAGEMENTID_CLOCK_ACCURACY */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_CLOCK_ACCURACY, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.clock_accuracy.clockAccuracy = 123;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_CLOCK_ACCURACY, 2, tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_CLOCK_ACCURACY, 0, NULL);

    /* PTP_MANAGEMENTID_UTC_PROPERTIES */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UTC_PROPERTIES, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.utc_properties.currentUtcOffset = htons(1000);
    tlv->dataField.utc_properties.li61 = 1;
    tlv->dataField.utc_properties.li59 = 1;
    tlv->dataField.utc_properties.utcv = 1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_UTC_PROPERTIES, sizeof(tlv->dataField.utc_properties), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UTC_PROPERTIES, 0, NULL);

    /* PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.traceability_properties.ttra = 1;
    tlv->dataField.traceability_properties.ftra = 1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES, sizeof(tlv->dataField.traceability_properties), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES, 0, NULL);

    /* PTP_MANAGEMENTID_TIMESCALE_PROPERTIES */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TIMESCALE_PROPERTIES, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.timescale_properties.ptp = 1;
    tlv->dataField.timescale_properties.timeSource = 0;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_TIMESCALE_PROPERTIES, sizeof(tlv->dataField.timescale_properties), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_TIMESCALE_PROPERTIES, 0, NULL);

    /* PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.unicast_negotiation_enable.en = 1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE, sizeof(tlv->dataField.unicast_negotiation_enable), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE, 0, NULL);

    /* PTP_MANAGEMENTID_PATH_TRACE_ENABLE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PATH_TRACE_ENABLE, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.path_trace_enable.en = 1;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_PATH_TRACE_ENABLE, sizeof(tlv->dataField.path_trace_enable), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PATH_TRACE_ENABLE, 0, NULL);

    /* PTP_MANAGEMENTID_PATH_TRACE_LIST */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_PATH_TRACE_LIST, 0, NULL);

    /* PTP_MANAGEMENTID_UNICAST_MASTER_TABLE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UNICAST_MASTER_TABLE, 0, NULL);
    //do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_UNICAST_MASTER_TABLE, 0, NULL);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UNICAST_MASTER_TABLE, 0, NULL);

    /* PTP_MANAGEMENTID_UNICAST_MASTER_MAX_TABLE_SIZE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_UNICAST_MASTER_MAX_TABLE_SIZE, 0, NULL);

    /* PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE, 0, NULL);
    //do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE, 0, NULL);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE, 0, NULL);

    /* PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED, 0, NULL);
    //do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED, 0, NULL);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED, 0, NULL);

    /* PTP_MANAGEMENTID_ACCEPTABLE_MASTER_MAX_TABLE_SIZE */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_ACCEPTABLE_MASTER_MAX_TABLE_SIZE, 0, NULL);

    /* PTP_MANAGEMENTID_DELAY_MECHANISM */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_DELAY_MECHANISM, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.delay_mechanism[0].delayMechanism = 2;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_DELAY_MECHANISM, sizeof(tlv->dataField.delay_mechanism[0]), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_DELAY_MECHANISM, 0, NULL);

    /* PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL */
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL, 0, NULL);
    memset(buffer, 0, sizeof(buffer));
    tlv->dataField.log_min_pdelay_req_interval[0].logMinPdelayReqInterval = 2;
    do_management(interface_list, PTP_ACTIONFIELD_SET, PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL, sizeof(tlv->dataField.log_min_pdelay_req_interval[0]), tlv);
    do_management(interface_list, PTP_ACTIONFIELD_GET, PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL, 0, NULL);

    return 0;
}
