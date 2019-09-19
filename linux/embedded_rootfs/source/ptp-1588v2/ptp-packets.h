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

#ifndef __PTP_PACKETS_H__
#define __PTP_PACKETS_H__
/**
 * @file
 * This file contains structures representing all of the packet formats
 * for PTPv2. PTPv2 is documented in the IEEE Standard 1588-2008. The section
 * numbers in this document refer to this standard.
 */
#include <stdint.h>

#define PTP_VERSION     2               /* PTP header version field */

/**
 * Section 5.3.5 PortIdentity
 */
typedef struct __attribute__ ((__packed__))
{
    uint64_t    clockIdentity;
    uint16_t    portNumber;
} ptp_portIdentity_t;

/**
 * Section 5.3.3 Timestamp
 */
typedef struct __attribute__ ((__packed__))
{
    uint64_t secondsField : 48;
    uint32_t nanosecondsField : 32;
} ptp_timestamp_t;

/**
 * Section 5.3.7 ClockQuality
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t     clockClass;
    uint8_t     clockAccuracy;
    uint16_t    offsetScaledLogVariance;
} ptp_clockQuality_t;

/**
 * Section13.3.2.2 messageType
 */
typedef enum
{
    PTP_MESSAGETYPE_SYNC                    = 0x0,
    PTP_MESSAGETYPE_DELAY_REQ               = 0x1,
    PTP_MESSAGETYPE_PDELAY_REQ              = 0x2,
    PTP_MESSAGETYPE_PDELAY_RESP             = 0x3,
    PTP_MESSAGETYPE_FOLLOW_UP               = 0x8,
    PTP_MESSAGETYPE_DELAY_RESP              = 0x9,
    PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP   = 0xA,
    PTP_MESSAGETYPE_ANNOUNCE                = 0xB,
    PTP_MESSAGETYPE_SIGNALING               = 0xC,
    PTP_MESSAGETYPE_MANAGEMENT              = 0xD
} ptp_messageType_t;

/* Section 13.3.2.6 flagField. Note that the ordering of the bytes
    in the specification assumes little endian while every other field
    is big endian. These bit numbers are in big endian, so the two
    bytes are swapped compared to the spec numbering */
typedef enum
{
    PTP_FLAGFIELD_PROFILE_LEAP61                = 1<<0,
    PTP_FLAGFIELD_PROFILE_LEAP59                = 1<<1,
    PTP_FLAGFIELD_PROFILE_CURRENTUTCOFFSETVALID = 1<<2,
    PTP_FLAGFIELD_PROFILE_PTPTIMESCALE          = 1<<3,
    PTP_FLAGFIELD_PROFILE_TIMETRACEABLE         = 1<<4,
    PTP_FLAGFIELD_PROFILE_FREQUENCYTRACEABLE    = 1<<5,
    // Reserved 6-7
    PTP_FLAGFIELD_ALTERNATEMASTERFLAG           = 1<<8,
    PTP_FLAGFIELD_TWOSTEPFLAG                   = 1<<9,
    PTP_FLAGFIELD_UNICASTFLAG                   = 1<<10,
    PTP_FLAGFIELD_PROFILE_SPECIFIC1             = 1<<11,
    PTP_FLAGFIELD_PROFILE_SPECIFIC2             = 1<<12,
} ptp_flagField_t;

/**
 * Section 13.3.2.10 controlField
 */
typedef enum
{
    PTP_CONTROLFIELD_SYNC       = 0,
    PTP_CONTROLFIELD_DELAY_REQ  = 1,
    PTP_CONTROLFIELD_FOLLOW_UP  = 2,
    PTP_CONTROLFIELD_DELAY_RESP = 3,
    PTP_CONTROLFIELD_MANAGEMENT = 4,
    PTP_CONTROLFIELD_OTHERS     = 5
} ptp_controlField_t;

/**
 * Section 15.4.1.6 actionField
 */
typedef enum
{
    PTP_ACTIONFIELD_GET = 0,
    PTP_ACTIONFIELD_SET = 1,
    PTP_ACTIONFIELD_RESPONSE = 2,
    PTP_ACTIONFIELD_COMMAND = 3,
    PTP_ACTIONFIELD_ACKNOWLEDGE = 4
} ptp_actionField_t;

/**
 * Section 14.1.1 tlvType
 */
typedef enum
{
    PTP_TLVTYPE_MANAGEMENT                              = 0x0001,
    PTP_TLVTYPE_MANAGEMENT_ERROR_STATUS                 = 0x0002,
    PTP_TLVTYPE_ORGANIZATION_EXTENSION                  = 0x0003,
    PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION            = 0x0004,
    PTP_TLVTYPE_GRANT_UNICAST_TRANSMISSION              = 0x0005,
    PTP_TLVTYPE_CANCEL_UNICAST_TRANSMISSION             = 0x0006,
    PTP_TLVTYPE_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION = 0x0007,
    PTP_TLVTYPE_PATH_TRACE                              = 0x0008,
    PTP_TLVTYPE_ALTERNATE_TIME_OFFSET_INDICATOR         = 0x0009,
    PTP_TLVTYPE_AUTHENTICATION                          = 0x2000,
    PTP_TLVTYPE_AUTHENTICATION_CHALLENGE                = 0x2001,
    PTP_TLVTYPE_SECURITY_ASSOCIATION_UPDATE             = 0x2002,
    PTP_TLVTYPE_CUM_FREQ_SCALE_FACTOR_OFFSET            = 0x2003
} ptp_tlvType_t;

/**
 * Section 15.5.2.3 managementId
 */
typedef enum
{
    PTP_MANAGEMENTID_NULL_MANAGEMENT                    = 0x0000,
    PTP_MANAGEMENTID_CLOCK_DESCRIPTION                  = 0x0001,
    PTP_MANAGEMENTID_USER_DESCRIPTION                   = 0x0002,
    PTP_MANAGEMENTID_SAVE_IN_NON_VOLATILE_STORAGE       = 0x0003,
    PTP_MANAGEMENTID_RESET_NON_VOLATILE_STORAGE         = 0x0004,
    PTP_MANAGEMENTID_INITIALIZE                         = 0x0005,
    PTP_MANAGEMENTID_FAULT_LOG                          = 0x0006,
    PTP_MANAGEMENTID_FAULT_LOG_RESET                    = 0x0007,
    PTP_MANAGEMENTID_DEFAULT_DATA_SET                   = 0x2000,
    PTP_MANAGEMENTID_CURRENT_DATA_SET                   = 0x2001,
    PTP_MANAGEMENTID_PARENT_DATA_SET                    = 0x2002,
    PTP_MANAGEMENTID_TIME_PROPERTIES_DATA_SET           = 0x2003,
    PTP_MANAGEMENTID_PORT_DATA_SET                      = 0x2004,
    PTP_MANAGEMENTID_PRIORITY1                          = 0x2005,
    PTP_MANAGEMENTID_PRIORITY2                          = 0x2006,
    PTP_MANAGEMENTID_DOMAIN                             = 0x2007,
    PTP_MANAGEMENTID_SLAVE_ONLY                         = 0x2008,
    PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL              = 0x2009,
    PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT           = 0x200A,
    PTP_MANAGEMENTID_LOG_SYNC_INTERVAL                  = 0x200B,
    PTP_MANAGEMENTID_VERSION_NUMBER                     = 0x200C,
    PTP_MANAGEMENTID_ENABLE_PORT                        = 0x200D,
    PTP_MANAGEMENTID_DISABLE_PORT                       = 0x200E,
    PTP_MANAGEMENTID_TIME                               = 0x200F,
    PTP_MANAGEMENTID_CLOCK_ACCURACY                     = 0x2010,
    PTP_MANAGEMENTID_UTC_PROPERTIES                     = 0x2011,
    PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES            = 0x2012,
    PTP_MANAGEMENTID_TIMESCALE_PROPERTIES               = 0x2013,
    PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE         = 0x2014,
    PTP_MANAGEMENTID_PATH_TRACE_LIST                    = 0x2015,
    PTP_MANAGEMENTID_PATH_TRACE_ENABLE                  = 0x2016,
    PTP_MANAGEMENTID_GRANDMASTER_CLUSTER_TABLE          = 0x2017,
    PTP_MANAGEMENTID_UNICAST_MASTER_TABLE               = 0x2018,
    PTP_MANAGEMENTID_UNICAST_MASTER_MAX_TABLE_SIZE      = 0x2019,
    PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE            = 0x201A,
    PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED    = 0x201B,
    PTP_MANAGEMENTID_ACCEPTABLE_MASTER_MAX_TABLE_SIZE   = 0x201C,
    PTP_MANAGEMENTID_ALTERNATE_MASTER                   = 0x201D,
    PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_ENABLE       = 0x201E,
    PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_NAME         = 0x201F,
    PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_MAX_KEY      = 0x2020,
    PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_PROPERTIES   = 0x2021,
    PTP_MANAGEMENTID_TRANSPARENT_CLOCK_DEFAULT_DATA_SET = 0x4000,
    PTP_MANAGEMENTID_TRANSPARENT_CLOCK_PORT_DATA_SET    = 0x4001,
    PTP_MANAGEMENTID_PRIMARY_DOMAIN                     = 0x4002,
    PTP_MANAGEMENTID_DELAY_MECHANISM                    = 0x6000,
    PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL        = 0x6001,
} ptp_managementId_t;

/**
 * Section 15.5.4.1.4 managementErrorId
 */
typedef enum
{
    PTP_MANAGEMENTERRORID_RESPONSE_TOO_BIG              = 0x0001,
    PTP_MANAGEMENTERRORID_NO_SUCH_ID                    = 0x0002,
    PTP_MANAGEMENTERRORID_WRONG_LENGTH                  = 0x0003,
    PTP_MANAGEMENTERRORID_WRONG_VALUE                   = 0x0004,
    PTP_MANAGEMENTERRORID_NOT_SETABLE                   = 0x0005,
    PTP_MANAGEMENTERRORID_NOT_SUPPORTED                 = 0x0006,
    PTP_MANAGEMENTERRORID_GENERAL_ERROR                 = 0xFFFE,
} ptp_managementErrorId_t;

/**
 * Section 5.3.8 TLV
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_tlvType_t       tlvType : 16;
    uint16_t            lengthField;
    uint8_t             valueField[0];
} ptp_tlv_t;

/**
 * Section 5.3.9 PTPText
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t             lengthField;
    uint8_t             textField[0];
} ptp_text_t;

/**
 * Section 5.3.10 FaultRecord
 */
typedef struct __attribute__ ((__packed__))
{
    uint16_t            faultRecordLength;
    ptp_timestamp_t     faultTime;
    uint8_t             severityCode;
    uint8_t             data[0];
    /* PTPText faultName; */
    /* PTPText faultValue; */
    /* PTPText faultDescription; */
} ptp_faultRecord_t;

/**
 * Section 13.3.1 General header specifications
 */
typedef struct __attribute__ ((__packed__))
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    ptp_messageType_t   messageType : 4;
    uint8_t             transportSpecific : 4;
    uint8_t             versionPTP : 4;
    uint8_t             reserved : 4;
#else
    uint8_t             transportSpecific : 4;
    ptp_messageType_t   messageType : 4;
    uint8_t             reserved : 4;
    uint8_t             versionPTP : 4;
#endif
    uint16_t            messageLength;
    uint8_t             domainNumber;
    uint8_t             reserved2;
    ptp_flagField_t     flagField : 16;
    uint64_t            correctionField;
    uint32_t            reserved3;
    ptp_portIdentity_t  sourcePortIdentity;
    uint16_t            sequenceId;
    ptp_controlField_t  controlField : 8;
    uint8_t             logMessageInterval;
} ptp_header_t;

/**
 * Section 13.5.1 General Announce message specifications
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     originTimestamp;
    uint16_t            currentUtcOffset;
    uint8_t             reserved;
    uint8_t             grandmasterPriority1;
    ptp_clockQuality_t  grandmasterClockQuality;
    uint8_t             grandmasterPriority2;
    uint64_t            grandmasterIdentity;
    uint16_t            stepsRemoved;
    uint8_t             timeSource;
    ptp_tlv_t           tlv[0]; /* Optional path trace */
} ptp_message_announce_t;

/**
 * Section 13.6 Sync and Delay_Req messages
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     originTimestamp;
} ptp_message_sync_t;
typedef ptp_message_sync_t ptp_message_delay_req_t;

/**
 * Section 13.7 Follow_Up message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     preciseOriginTimestamp;
} ptp_message_follow_up_t;

/**
 * Section 13.8 Delay_Resp message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     receiveTimestamp;
    ptp_portIdentity_t  requestingPortIdentity;
} ptp_message_delay_resp_t;

/**
 * Section 13.9 Pdelay_Req message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     originTimestamp;
    uint8_t             reserved[10];
} ptp_message_pdelay_req_t;

/**
 * Section 13.10 Pdelay_Resp message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     requestReceiveTimestamp;
    ptp_portIdentity_t  requestingPortIdentity;
} ptp_message_pdelay_resp_t;

/**
 * Section 13.11 Pdelay_Resp_Follow_Up message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_timestamp_t     responseOriginTimestamp;
    ptp_portIdentity_t  requestingPortIdentity;
} ptp_message_pdelay_resp_follow_up_t;

/**
 * Section 13.12 Signaling message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_portIdentity_t  targetPortIdentity;
    ptp_tlv_t           tlv[0]; /* One or more */
} ptp_message_signaling_t;

/**
 * Section 13.13 Management message
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_portIdentity_t  targetPortIdentity;
    uint8_t             startingBoundaryHops;
    uint8_t             boundaryHops;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    ptp_actionField_t   actionField : 4;
    uint8_t             reserved : 4;
#else
    uint8_t             reserved : 4;
    ptp_actionField_t   actionField : 4;
#endif
    uint8_t             reserved2;
    ptp_tlv_t           managementTLV[0]; /* One or more */
} ptp_message_management_t;

/**
 * Section 15.5 Management TLVs
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_managementId_t managementId : 16;
    union
    {
        /* Nothing for PTP_MANAGEMENTID_NULL_MANAGEMENT */
        struct __attribute__ ((__packed__))
        {
            /* The fields in this structure change location based on length */
            uint8_t     data[0];
        } clock_description;

        ptp_text_t user_description;
        /* Nothing for PTP_MANAGEMENTID_SAVE_IN_NON_VOLATILE_STORAGE */
        /* Nothing for PTP_MANAGEMENTID_RESET_NON_VOLATILE_STORAGE */

        struct __attribute__ ((__packed__))
        {
            uint16_t initializationKey; /* Should always be zero */
        } initialize;

        struct __attribute__ ((__packed__))
        {
            uint16_t    numberOfFaultRecords;
            ptp_faultRecord_t faultRecord[0];
        } fault_log;

        /* Nothing for PTP_MANAGEMENTID_FAULT_LOG_RESET */

        struct __attribute__ ((__packed__))
        {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t     tsc : 1;
            uint8_t     so : 1;
            uint8_t     reserved : 6;
#else
            uint8_t     reserved : 6;
            uint8_t     so : 1;
            uint8_t     tsc : 1;
#endif
            uint8_t     reserved2;
            uint16_t    numberPorts;
            uint8_t     priority1;
            ptp_clockQuality_t clockQuality;
            uint8_t     priority2;
            uint64_t    clockIdentity;
            uint8_t     domainNumber;
            uint8_t     reserved3;
        } default_data_set;

        struct __attribute__ ((__packed__))
        {
            uint16_t    stepsRemoved;
            uint64_t    offsetFromMaster;
            uint64_t    meanPathDelay;
        } current_data_set;

        struct __attribute__ ((__packed__))
        {
            ptp_portIdentity_t parentPortIdentity;
            uint8_t     ps;
            uint8_t     reserved;
            uint16_t    observedParentOffsetScaledLogVariance;
            uint32_t    observedParentClockPhaseChangeRate;
            uint8_t     grandmasterPriority1;
            ptp_clockQuality_t grandmasterClockQuality;
            uint8_t     grandmasterPriority2;
            uint64_t    grandmasterIdentity;
        } parent_data_set;

        struct __attribute__ ((__packed__))
        {
            uint16_t    currentUtcOffset;
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t     li61 : 1;
            uint8_t     li59 : 1;
            uint8_t     utcv : 1;
            uint8_t     ptp : 1;
            uint8_t     ttra : 1;
            uint8_t     ftra : 1;
            uint8_t     reserved : 2;
#else
            uint8_t     reserved : 2;
            uint8_t     ftra : 1;
            uint8_t     ttra : 1;
            uint8_t     ptp : 1;
            uint8_t     utcv : 1;
            uint8_t     li59 : 1;
            uint8_t     li61 : 1;
#endif
            uint8_t     timeSource;
        } time_properties_data_set;

        struct __attribute__ ((__packed__))
        {
            ptp_portIdentity_t portIdentity;
            uint8_t     portState;
            uint8_t     logMinDelayReqInterval;
            uint64_t    peerMeanPathDelay;
            uint8_t     logAnnounceInterval;
            uint8_t     announceReceiptTimeout;
            uint8_t     logSyncInterval;
            uint8_t     delayMechanism;
            uint8_t     logMinPdelayReqInterval;
            uint8_t     versionNumber;
        } port_data_set[0];

        struct __attribute__ ((__packed__))
        {
            uint8_t     priority1;
            uint8_t     reserved;
        } priority1;

        struct __attribute__ ((__packed__))
        {
            uint8_t     priority2;
            uint8_t     reserved;
        } priority2;

        struct __attribute__ ((__packed__))
        {
            uint8_t     domain;
            uint8_t     reserved;
        } domain;

        struct __attribute__ ((__packed__))
        {
            uint8_t     slave_only;
            uint8_t     reserved;
        } slave_only;

        struct __attribute__ ((__packed__))
        {
            uint8_t     logAnnounceInterval;
            uint8_t     reserved;
        } log_announce_interval[0];

        struct __attribute__ ((__packed__))
        {
            uint8_t     announceReceiptTimeout;
            uint8_t     reserved;
        } announce_receipt_timeout[0];

        struct __attribute__ ((__packed__))
        {
            uint8_t     logSyncInterval;
            uint8_t     reserved;
        } log_sync_interval[0];

        struct __attribute__ ((__packed__))
        {
            uint8_t     versionNumber;
            uint8_t     reserved;
        } version_number;

        /* Nothing for PTP_MANAGEMENTID_ENABLE_PORT */
        /* Nothing for PTP_MANAGEMENTID_DISABLE_PORT */

        struct __attribute__ ((__packed__))
        {
            ptp_timestamp_t currentTime;
        } time;

        struct __attribute__ ((__packed__))
        {
            uint8_t clockAccuracy;
            uint8_t reserved;
        } clock_accuracy;

        struct __attribute__ ((__packed__))
        {
            uint16_t    currentUtcOffset;
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t     li61 : 1;
            uint8_t     li59 : 1;
            uint8_t     utcv : 1;
            uint8_t     reserved : 5;
#else
            uint8_t     reserved : 5;
            uint8_t     utcv : 1;
            uint8_t     li59 : 1;
            uint8_t     li61 : 1;
#endif
            uint8_t     reserved2;
        } utc_properties;

        struct __attribute__ ((__packed__))
        {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t     reserved : 4;
            uint8_t     ttra : 1;
            uint8_t     ftra : 1;
            uint8_t     reserve2 : 2;
#else
            uint8_t     reserved2 : 2;
            uint8_t     ftra : 1;
            uint8_t     ttra : 1;
            uint8_t     reserved : 4;
#endif
            uint8_t     reserved3;
        } traceability_properties;

        struct __attribute__ ((__packed__))
        {
#if __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t     reserved : 3;
            uint8_t     ptp : 1;
            uint8_t     reserved2 : 4;
#else
            uint8_t     reserved2 : 4;
            uint8_t     ptp : 1;
            uint8_t     reserved : 3;
#endif
            uint8_t     timeSource;
        } timescale_properties;

        struct __attribute__ ((__packed__))
        {
            uint8_t     en;
            uint8_t     reserved;
        } unicast_negotiation_enable;

        struct __attribute__ ((__packed__))
        {
            uint64_t    clockIdentity[0];
        } path_trace_list;

        struct __attribute__ ((__packed__))
        {
            uint8_t     en;
            uint8_t     reserved;
        } path_trace_enable;

        struct __attribute__ ((__packed__))
        {
        } grandmaster_cluster_table;
        struct __attribute__ ((__packed__))
        {
            uint8_t     logQueryInterval;
            uint16_t    tableSize;
            char        unicastMasterTable[0];
            uint8_t     reserved;
        } unicast_master_table;
        struct __attribute__ ((__packed__))
        {
            uint16_t    maxTableSize;
        } unicast_master_max_table_size;
        struct __attribute__ ((__packed__))
        {
            uint16_t    tableSize;
        } acceptable_master_table;
        struct __attribute__ ((__packed__))
        {
            uint8_t     en;
            uint8_t     reserved;
        } acceptable_master_table_enabled;
        struct __attribute__ ((__packed__))
        {
            uint16_t    maxTableSize;
        } acceptable_master_max_table_size;
        struct __attribute__ ((__packed__))
        {
        } alternate_master;
        struct __attribute__ ((__packed__))
        {
        } alternate_time_offset_enable;
        struct __attribute__ ((__packed__))
        {
        } alternate_time_offset_name;
        struct __attribute__ ((__packed__))
        {
        } alternate_time_offset_max_key;
        struct __attribute__ ((__packed__))
        {
        } alternate_time_offset_properties;

        struct __attribute__ ((__packed__))
        {
            uint64_t    clockIdentity;
            uint16_t    numberPorts;
            uint8_t     delayMechanism;
            uint8_t     primaryDomain;
        } transparent_clock_default_data_set;

        struct __attribute__ ((__packed__))
        {
             ptp_portIdentity_t portIdentity;
             uint8_t    flt;
             uint8_t    logMinPdelayReqInterval;
             uint64_t   peerMeanPathDelay;
        } transparent_clock_port_data_set;

        struct __attribute__ ((__packed__))
        {
            uint8_t     primaryDomain;
            uint8_t     reserved;
        } primary_domain;

        struct __attribute__ ((__packed__))
        {
            uint8_t     delayMechanism;
            uint8_t     reserved;
        } delay_mechanism[0];

        struct __attribute__ ((__packed__))
        {
            uint8_t     logMinPdelayReqInterval;
            uint8_t     reserved;
        } log_min_pdelay_req_interval[0];
    } dataField;
} ptp_management_tlv_t;

/**
 * Section 15.5.4 MANAGEMENT_ERROR_STATUS TLV
 */
typedef struct __attribute__ ((__packed__))
{
    uint16_t    managementErrorId;
    uint16_t    managementId;
    uint32_t    reserved;
    ptp_text_t  displayData;
} ptp_management_tlv_error_t;

/**
 * This is the complete PTP message including Layer 2 headers
 * as defined by Annex F, "Transport of PTP over IEEE 802.3 /Ethernet".
 * Since Octeon only supports accurate timestamps over SGMII and XAUI,
 * we should only need to support the 802.3 transport.
 */
typedef struct __attribute__ ((__packed__))
{
    ptp_header_t        header;
    union
    {
        ptp_message_sync_t          sync;               /* PTP_MESSAGETYPE_SYNC */
        ptp_message_delay_req_t     dreq;               /* PTP_MESSAGETYPE_DELAY_REQ */
        ptp_message_pdelay_req_t    pdreq;              /* PTP_MESSAGETYPE_PDELAY_REQ */
        ptp_message_pdelay_resp_t   pdresp;             /* PTP_MESSAGETYPE_PDELAY_RESP */
        ptp_message_follow_up_t     follow;             /* PTP_MESSAGETYPE_FOLLOW_UP */
        ptp_message_delay_resp_t    dresp;              /* PTP_MESSAGETYPE_DELAY_RESP */
        ptp_message_pdelay_resp_follow_up_t pdresp2;    /* PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP */
        ptp_message_announce_t      announce;           /* PTP_MESSAGETYPE_ANNOUNCE */
        ptp_message_signaling_t     sig;                /* PTP_MESSAGETYPE_SIGNALING */
        ptp_message_management_t    mgmt;               /* PTP_MESSAGETYPE_MANAGEMENT */
        char raw[2048];
    } ptp;
} ptp_packet_t;

static inline uint64_t htonq(uint64_t x)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return ((x >> 56) |
            (((x >> 48) & 0xfful) << 8) |
            (((x >> 40) & 0xfful) << 16) |
            (((x >> 32) & 0xfful) << 24) |
            (((x >> 24) & 0xfful) << 32) |
            (((x >> 16) & 0xfful) << 40) |
            (((x >>  8) & 0xfful) << 48) |
            (((x >>  0) & 0xfful) << 56));
#else
    return x;
#endif
}

static inline uint64_t ntohq(uint64_t x)
{
    return htonq(x);
}

#endif
