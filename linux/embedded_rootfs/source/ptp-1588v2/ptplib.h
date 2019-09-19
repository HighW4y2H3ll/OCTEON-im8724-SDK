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

#ifndef __PTPLIB_H__
#define __PTPLIB_H__
/**
 * @file
 * This file contains the external API for the PTP library, which implements
 * the IEEE 1588-2008 standard. This library is designed to be included into
 * existing applications by supplying the proper hooks to add PTP processing.
 */
#include <stdint.h>

typedef struct
{
    char data[65536];
} ptplib_state_t;

typedef enum
{
    PTPLIB_FLAGS_USE_PEER_DELAY         = 1<<0, /* Use peer vs normal delay messages */
    PTPLIB_FLAGS_USE_MULTICAST          = 1<<1, /* Use multicast annouce and sync */
    PTPLIB_FLAGS_USE_UNICAST            = 1<<2, /* Use unicast whenever possible */
    PTPLIB_FLAGS_USE_FREQUENCY_ADJUST   = 1<<3, /* Adjust the frequency of the clock */
    PTPLIB_FLAGS_USE_PHASE_ADJUST       = 1<<4, /* Adjust the phase of the clock */
} ptplib_flags_t;

typedef enum
{
    PTPLIB_USE_DEFAULT_CLOCK = 0,
    PTPLIB_USE_INT_CLOCK,
    PTPLIB_USE_QLM_CLOCK,
    PTPLIB_USE_GPIO_CLOCK
} ptplib_clock_source_t;

typedef enum
{
    PTPLIB_BOARD_UNSUPPORTED,
    PTPLIB_BOARD_IM8724,
    PTPLIB_BOARD_N822
} ptplib_board_t;

/**
 * Initialize a state instance ofr use with ptplib. Each state
 * instance represents a synchronizable clock. Based on paramaters
 * of this function, this could be an ordinary clock, boundary
 * clock, or transparent clock.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param flags  Optional flags
 * 
 * @param clock_source  See ptplib_clock_source_t enum
 * 
 * @param clock_num
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_initialize(ptplib_state_t *ptp, ptplib_flags_t flags, int clock_source, int clock_num);

/**
 * Add a packet interface for use by PTP/1588v2
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param port   Packet interface to add.
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_interface_add(ptplib_state_t *ptp, packetio_t *port);

/**
 * This function should be called for every PTP/1588v2 packet
 * received. It performs all state updates and packet sends
 * required by the PTP/1588v2 protocol.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param packet Contains the packet. The packet is not modified by this
 *               function.
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_process(ptplib_state_t *ptp, const packetio_packet_t *packet);

/**
 * This function should be called for every PTP/1588v2 packet received on a
 * socket's message error queue. It performs all state updates and packet sends
 * required by the PTP/1588v2 protocol.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param packet Contains the packet. The packet is not modified by this
 *               function.
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_process_errorqueue(ptplib_state_t *ptp, const packetio_packet_t *packet);

/**
 * PTP/1588v2 requires processing based on an external timer. This
 * function performs all the required processing. It should be
 * called at least once per second, but may be called more often.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_periodic(ptplib_state_t *ptp);

/**
 * Display current clock into to the user
 *
 * @param ptp    Our state info
 * @param clear_screen
 *               Clear the terminal window before displaying
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_display(ptplib_state_t *ptp, int clear_screen);

/**
 * Shutdown a PTP/1588v2 library instance.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 *
 * @return Zero on success, negative on failure.
 */
extern int ptplib_shutdown(ptplib_state_t *ptp);

#endif
