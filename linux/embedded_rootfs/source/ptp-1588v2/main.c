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

#define ERASE_WIN   "\033[2J"       /* Erase the window */
#define GOTO_BOTTOM "\033[100;1H"   /* Begins output at the bottom of the terminal (actually line 100) */

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
    int index;
    int num_interfaces = 0;
    packetio_t interface_list[32];

    /* Check that we have the proper number of arguments */
    if ((argc < 2) || (argc > 32))
    {
        printf("Usage:\n"
               "%s interface[:transport] [interface[:transport]]*\n"
               "\n"
               "    interface = Linux network device name (eth0, eth1, etc).\n"
               "    transport = Which transport to use (802.3, UDP, UDP6). Defaults to 802.3.\n"
               "\n", argv[0]);
        return -1;
    }

    /* Initialize ptplib */
    if (ptplib_initialize(&ptp, 0
        //| PTPLIB_FLAGS_USE_PEER_DELAY
        | PTPLIB_FLAGS_USE_MULTICAST
        | PTPLIB_FLAGS_USE_UNICAST
#if __BYTE_ORDER == __BIG_ENDIAN
        | PTPLIB_FLAGS_USE_FREQUENCY_ADJUST
        | PTPLIB_FLAGS_USE_PHASE_ADJUST
#endif
        ))
    {
        return -1;
    }

    /* Clear the screen and tell the user what we are doing */
    printf(ERASE_WIN GOTO_BOTTOM);
    printf("Adding Interfaces\n");

    /* For each argument, add it as a packet interface */
    for (index = 1; index < argc; index++)
    {
        packetio_t *packetio = interface_list + num_interfaces;
        char interface[32];
        char *options;
        char *transport;

        strncpy(interface, argv[index], sizeof(interface));
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

        /* Tell the user about the interface */
        printf("%s: Transport:%s MAC: %012llx\n", interface,
            transport, (unsigned long long)packetio->mac);
        if (ptplib_interface_add(&ptp, packetio))
            return -1;
        num_interfaces++;
    }

    printf("Done adding interfaces\n");

    timestamp_t last_display = timestamp_get_rawtime();
    /* This is our main processing loop */
    while (1)
    {
        struct pollfd poll_data[64];
        int nfds = 0;

        /* Do any periodic processing required */
        ptplib_periodic(&ptp);

        /* Build a select set for all open sockets */
        for (index=0; index<num_interfaces; index++)
        {
            poll_data[nfds].fd = interface_list[index].sock_event;
            poll_data[nfds].revents = 0;
            poll_data[nfds].events = POLLIN;
            nfds++;
            poll_data[nfds].fd = interface_list[index].sock_msg;
            poll_data[nfds].revents = 0;
            poll_data[nfds].events = POLLIN;
            nfds++;
        }

        /* Wait for the sockets to need service */
        if (poll(poll_data, nfds, 1) < 0)
        {
            perror("poll");
            continue;
        }

        /* Check which sockets need attention */
        nfds = 0;
        for (index=0; index<num_interfaces; index++)
        {
            packetio_t *io = &interface_list[index];
            if (poll_data[nfds].revents & POLLERR)
            {
                packetio_packet_t packet;
                if (packetio_receive_event(io, &packet, MSG_ERRQUEUE) == 0)
                    ptplib_process_errorqueue(&ptp, &packet);
            }
            if (poll_data[nfds].revents & POLLIN)
            {
                packetio_packet_t packet;
                if (packetio_receive_event(io, &packet, 0) == 0)
                    ptplib_process(&ptp, &packet);
            }
            nfds++;
            if (poll_data[nfds].revents & POLLIN)
            {
                packetio_packet_t packet;
                if (packetio_receive_msg(io, &packet) == 0)
                    ptplib_process(&ptp, &packet);
            }
            nfds++;
        }

        /* Display status */
        timestamp_t current = timestamp_get_rawtime();
        if (current.nanoseconds - last_display.nanoseconds >= NSEC)
        {
            ptplib_display(&ptp, 0);
            last_display = current;
        }
    }

    return 0;
}
