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

#ifndef __PTP_DATA_SET_H__
#define __PTP_DATA_SET_H__
/**
 * @file
 * This file contains structures representing all of the data sets
 * for PTPv2. PTPv2 is documented in the IEEE Standard 1588-2008. The section
 * numbers in this document refer to this standard.
 */
typedef int bool;

/**
 * The default data set is defined by section 8.2.1
 */
typedef struct
{
    bool                twoStepFlag;
    uint64_t            clockIdentity;
    int                 numberPorts;
    ptp_clockQuality_t  clockQuality;
    uint8_t             priority1;
    uint8_t             priority2;
    uint8_t             domainNumber;
    bool                slaveOnly;
    bool                unicastEnable;
    bool                pathTraceEnable;
} ptp_defaultDS_t;

/**
 * The current data set is defined by section 8.2.2
 */
typedef struct
{
    uint16_t            stepsRemoved;
    timestamp_avg_t     offsetFromMaster;
    timestamp_avg_t     meanPathDelay;
} ptp_currentDS_t;

/**
 * The parent data set is defined by section 8.2.3
 */
typedef struct
{
    ptp_portIdentity_t  parentPortIdentity;
    bool                parentStats;
    uint16_t            observedParentOffsetScaledLogVariance;
    uint32_t            observedParentClockPhaseChangeRate;
    uint64_t            grandmasterIdentity;
    ptp_clockQuality_t  grandmasterClockQuality;
    uint8_t             grandmasterPriority1;
    uint8_t             grandmasterPriority2;
} ptp_parentDS_t;

/**
 * The time properties data set is defined by section 8.2.4
 */
typedef struct
{
    uint16_t            currentUtcOffset;
    bool                currentUtcOffsetValid;
    bool                leap59;
    bool                leap61;
    bool                timeTraceable;
    bool                frequencyTraceable;
    bool                ptpTimescale;
    uint8_t             timeSource;
} ptp_timePropertiesDS_t;

/**
 * The port state enumeration is defined by section 8.2.3.1
 */
typedef enum
{
    PTP_PORT_STATE_INITIALIZING = 1,
    PTP_PORT_STATE_FAULTY = 2,
    PTP_PORT_STATE_DISABLED = 3,
    PTP_PORT_STATE_LISTENING = 4,
    PTP_PORT_STATE_PRE_MASTER = 5,
    PTP_PORT_STATE_MASTER = 6,
    PTP_PORT_STATE_PASSIVE = 7,
    PTP_PORT_STATE_UNCALIBRATED = 8,
    PTP_PORT_STATE_SLAVE = 9,
} ptp_port_state_t;

/**
 * The port state enumeration is defined by section 8.2.5.4.4
 */
typedef enum
{
    PTP_PORT_DELAY_E2E = 1,
    PTP_PORT_DELAY_P2P = 2,
    PTP_PORT_DELAY_DISABLED = 0xfe,
} ptp_port_delay_t;

/**
 * The port data set is defined by section 8.2.5
 */
typedef struct
{
    ptp_portIdentity_t  portIdentity;
    ptp_port_state_t    portState;
    int8_t              logMinDelayReqInterval;
    timestamp_avg_t     peerMeanPathDelay;
    int8_t              logAnnounceInterval;
    int8_t              announceReceiptTimeout;
    int8_t              logSyncInterval;
    ptp_port_delay_t    delayMechanism;
    int8_t              logMinPdelayReqInterval;
    uint8_t             versionNumber;
} ptp_portDS_t;

#endif
