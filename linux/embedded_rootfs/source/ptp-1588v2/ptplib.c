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
 * This file contains the external API for the PTP library, which implements
 * the IEEE 1588-2008 standard. This library is designed to be included into
 * existing applications by supplying the proper hooks to add PTP processing.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/timex.h>
#if USE_PTP_CLOCK
#include <sys/sysmips.h>
#include <asm/sysmips.h>
#endif
#include "ptp-packets.h"
#include "timestamp.h"
#include "ptp-data-set.h"
#include "packetio.h"
#include "ptplib.h"
#include "ptp-csrs.h"

#if USE_PTP_CLOCK
    #define PTP_GPIO_PPS_OUT    1   /* GPIO for output. Set to -1 to disable */
    #define PTP_GPIO_PPS_HZ     1   /* Frequency for the PPS signal, Normally 1Hz */
    #define PTP_GPIO_CLOCK_OUT  2   /* GPIO for output. Set to -1 to disable */
    #define PTP_GPIO_CLOCK_HZ   30720000    /* Frequency for the clock out, 30.72Mhz */
#else
    #define PTP_GPIO_PPS_OUT    (-1)
    #define PTP_GPIO_PPS_HZ     0
    #define PTP_GPIO_CLOCK_OUT  (-1)
    #define PTP_GPIO_CLOCK_HZ   0
#endif

#define PTPLIB_BOARD_IM8724_STR "CUST_IM8724"
#define PTPLIB_BOARD_N822_STR   "CUST_N822"

#define PTP_FOREIGN_MASTER_TIME_WINDOW  4       /* Section 9.3.2.4.4 - Four second window */
#define PTP_FOREIGN_MASTER_THRESHOLD    2       /* Section 9.3.2.4.4 - Need two announce messages */
#define PTP_DEFAULT_LOG_ANNOUNCE_INTERVAL 1     /* Section J.3.2 - Every two seconds. Spec allows 0 to 4, Telecom allows -3 to 4 */
#define PTP_DEFAULT_LOG_SYNC_INTERVAL   -2      /* Section J.3.2 - Four times per second. Spec allows -1 to 1, Telecom allows -7 to 4 */
#define PTP_DEFAULT_LOG_MIN_DELAY_REQ_INTERVAL 0 /* Section J.3.2 - Once per second. Spec allows 0 to 5, Telecom allows -7 to 4 */
#define PTP_DEFAULT_LOG_MIN_PDELAY_REQ_INTERVAL 0 /* Section J.4.2 - Once per second. Spec allows 0 to 5, Telecom allows -7 to 4 */
#define PTP_DEFAULT_ANNOUNCE_RECEIPT_TIMEOUT 3  /* Section J.4.2 - Spec allows 2 to 10 */
#define PTP_DEFAULT_PRIORITY1           128     /* Section J.3.2 */
#define PTP_DEFAULT_PRIORITY2           128     /* Section J.3.2 */
#define PTP_DEFAULT_PRIMARY_DOMAIN      0       /* Section J.3.2 */
#define PTP_DEFAULT_CLOCK_CLASS         248     /* Section 7.6.2.4 clockClass */
#define PTP_DEFAULT_CLOCK_ACCURACY      0xfe    /* Section 7.6.2.5 clockAccuracy - Unknown clock accuracy */
#define PTP_DEFAULT_OFFSET_SCALED_LOG_VARIANCE 0xffff /* Section 7.6.3.3 Variance representation - Not computed */
#define PTP_DEFAULT_CURRENT_UTC_OFFSET  0       /* Unknown */
#define PTP_DEFAULT_TIME_SOURCE         0xa0    /* Section 7.6.2.6 timeSource - Time based on an internal oscillator */
#define PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID 0x7f /* Section 13.3.2.11 logMessageInterval */
#define PTP_DEFAULT_RENEW_INTERVAL      300     /* Renew interval to use on unicast requests */
#define PTP_DEFAULT_MAX_PATH_TRACE      64      /* Maximum entries in our path trace table */

#define MAX_CLOCKS  16                          /* Max number of potential master clocks we can track */
#define MAX_PORTS   16                          /* Max number of IO ports */
#define MAX_OFFSET_FOR_SLEW (2*NSEC)            /* 2s - If the offsetFromMaster is greater than this then just set the clock instead of trying to slew it */
#define MAX_OFFSET_FOR_CALIB (NSEC/1000)        /* 1ms - Max the clock can be offset from master and we switch from uncalibrated to slave state */
#define ULL         unsigned long long          /* Used as shorthand in printf type casts */
#define AVERAGE_SHIFT_ANNOUNCE_INTERVAL 2       /* Average announce interval over 4 samples */
#define AVERAGE_SHIFT_OFFSETFROMMASTER  3       /* Average offset from master over 8 samples */
#define AVERAGE_SHIFT_DRIFTFROMMASTER   3       /* Average drift from master over 8 samples */
#define AVERAGE_SHIFT_PEERMEANPATHDELAY 3       /* Average peer delay over 8 samples */
#define AVERAGE_SHIFT_MEANPATHDELAY     3       /* Average path delay over 8 samples */

/* C doesn't allow you to do "struct a == struct b". This does the equivalent
    memcmp */
#define STRUCT_MISMATCH(a, b) memcmp(&a, &b, sizeof(a))
/* Absolute value without truncating to an int */
#define ABS(a) (((a) < 0) ? -(a) : (a))

#define PTPLIB_OUTPUT(type, pinfo, format, ...) printf("PTPLIB:[" type "] %s: " format, (pinfo) ? ((port_info_t*)pinfo)->packetio->name : "local", ##__VA_ARGS__)
#define PTPLIB_ERROR(pinfo, format, ...) PTPLIB_OUTPUT("ERROR", pinfo, format, ##__VA_ARGS__)
#define PTPLIB_DEBUG(pinfo, format, ...) PTPLIB_OUTPUT("DEBUG", pinfo, format, ##__VA_ARGS__)
#define PTPLIB_INFO(pinfo, format, ...) PTPLIB_OUTPUT("INFO", pinfo, format, ##__VA_ARGS__)

/**
 * Flags for each hardware port
 */
typedef enum
{
    PORT_FLAG_USE_UNICAST = 1<<0,   /* This port should use unicast responses when possible */
    PORT_FLAG_USE_TWOSTEP = 1<<1,   /* This port requres two step for accurate TX timestamps */
} port_flags_t;

/**
 * Information about a unicast client. These are dynamically
 * created as clients register.
 */
typedef struct unicast_node
{
    ptp_portIdentity_t  destPortIdentity;           /* Clients port asking for a unicast. Must be unique */
    char                txinfo[64];                 /* Network socket address for client */
    struct
    {
        timestamp_t     lease_expire;               /* Announce lease expire time */
        uint16_t        sequenceId;                 /* Incremented every time this port sends an Announce */
        int             interval;                   /* Interval between packets */
        timestamp_t     next;                       /* When the next announce message needs to be sent on this port */
    } announce;
    struct
    {
        timestamp_t     lease_expire;               /* Sync lease expire time */
        uint16_t        sequenceId;                 /* Incremented every time this port sends a Sync */
        int             interval;                   /* Interval between packets */
        timestamp_t     next;                       /* When the next sync message needs to be sent on this port */
    } sync;
    struct
    {
        timestamp_t     lease_expire;               /* Delay request expire time */
    } delay;
    struct unicast_node *prev;                      /* Previous unicast node in list. NULL on first */
    struct unicast_node *next;                      /* Next unicast node in list. NULL on last */
} unicast_node_t;

/**
 * Information tracked per ethernet communications port. A copy of this
 * structure exists for each Octeon hardware port.
 */
typedef struct
{
    ptp_portDS_t        portDS;                     /* Section 8.2.5 Port data set */
    timestamp_t         delayAsymmetry;             /* Section 7.4.2 Communication path asymmetry */

    struct
    {
        uint16_t        sequenceId;                 /* Incremented every time this ports sends an Announce */
        timestamp_t     next;                       /* When the next announce message needs to be sent on this port */
    } announce;

    struct
    {
        uint16_t        sequenceId;                 /* Incremented every time this ports sends a Sync */
        timestamp_t     next;                       /* When the next sync message needs to be sent on this port */
    } sync;

    struct
    {
        uint16_t        sequenceId;                 /* Incremented every time this ports sends a Delay Request */
        timestamp_t     next;                       /* When the next peer delay message needs to be sent */
        timestamp_t     t1_request_sent;            /* Time a peer delay request was sent (local) */
        timestamp_t     t2_request_received;        /* Time a peer delay request was received (remote) */
        timestamp_t     t4_reply_received;          /* Time a peer delay response was received (local) */
        timestamp_t     correctionField_resp;       /* CorrectionField from the last peer delay response */
    } pdelay;

    timestamp_t         announceReceiptTimeout;     /* Time to listen before becoming master */
    timestamp_t         qualificationTimeout;       /* Time to stay in pre master */
    port_flags_t        flags;                      /* Port info flags */

    unicast_node_t      *unicast_head;              /* List of unicast targets on this port */
    unicast_node_t      *unicast_tail;

    packetio_t *        packetio;                   /* Implementation specific info for TX and RX */
} port_info_t;

/**
 * Information about remote clocks that could be used as our master. We
 * need one of these for each clock on the network.
 */
typedef struct
{
    uint64_t                id;                     /* 64bit Clock Identifier from PTP packets */
    ptp_portIdentity_t      sourcePortIdentity;     /* PTP port the clock sent on */
    ptp_message_announce_t  announce;               /* Last announce message received for this clock */
    timestamp_t             last_announce;          /* Time last announce was received */
    timestamp_avg_t         announce_interval;      /* Interval between RX announce */
    port_info_t *           pinfo;                  /* Port this clock is connected to. Sync messages aren't sent here */
    char                    last_sync_rxinfo[64];   /* Transport layer address infor for last sync */
} clock_info_t;

/**
 * State representing our PTP daemon instance. Each clock domain serviced
 * by Octeon needs one of these. Normally you only need one.
 */
typedef struct
{
    ptplib_flags_t flags;                   /* Initialization flags */
    ptp_defaultDS_t defaultDS;
    ptp_currentDS_t currentDS;
    ptp_parentDS_t parentDS;
    ptp_timePropertiesDS_t timeDS;

    char userDescription[128 + 1];

    struct
    {
        uint16_t    last_sequenceId;        /* Sequence ID of the last sync message */
        timestamp_t last_t1_sent;           /* Time the previous master sync was sent (remote) */
        timestamp_t last_t2_received;       /* Time the previous master sync was received (local) */
        timestamp_t t1_sent;                /* Time the most recent master sync was sent (remote) */
        timestamp_t t2_received;            /* Time the most recent master sync was received (local) */
    } sync;

    struct
    {
        uint16_t    sequenceId;             /* Incremented every time this ports sends a Delay Request */
        timestamp_t next;                   /* When the next peer delay message needs to be sent */
        timestamp_t request_sent;           /* Time a peer delay request was sent (local) */
    } delay;

    struct
    {
        uint16_t    sequenceId;             /* Incremented every time this ports sends a signal */
        timestamp_t unicast_renew;          /* When the next unicast lease renewal is needed */
    } signal;

    clock_info_t *  best_clock;
    timestamp_avg_t driftFromMaster;        /* Our clock's drift from the master, expressed as parts per billion */

    port_info_t     port[MAX_PORTS];        /* Port information array */
    clock_info_t    clock[MAX_CLOCKS];      /* Array of potential master clocks */

    uint64_t        pathTrace[PTP_DEFAULT_MAX_PATH_TRACE];
    int             pathTraceSize;          /* Size of the current path trace table */
} ptplib_t;


/**
 * Increment a timestamp by a PTP log based interval.
 * Result = base + NSEC<<logInterval
 *
 * If the result is in the past, the current time is returned
 * instead.
 *
 * @param ptplib Our state info
 * @param base
 * @param logInterval
 *
 * @return
 */
static timestamp_t ptplib_add_interval(ptplib_t *ptplib, const timestamp_t base, int8_t logInterval)
{
    timestamp_t interval = {(logInterval<0) ? (NSEC >> -logInterval) : (NSEC << logInterval), 0};
    timestamp_t r = timestamp_add(base, interval);
    timestamp_t now = timestamp_get_rawtime();
    if (timestamp_before(r,  now))
        return now;
    else
        return r;
}


/**
 * Get the interface port descriptor that is associated with the
 * given packet.
 *
 * @param ptplib Our state info
 * @param packet Packet to lookup
 *
 * @return Interface port or NULL
 */
static port_info_t *ptplib_get_port(ptplib_t *ptplib, const packetio_packet_t *packet)
{
    int index;
    for (index = 0; index < ptplib->defaultDS.numberPorts; index++)
    {
        if (ptplib->port[index].packetio == packet->packetio)
            return &ptplib->port[index];
    }
    PTPLIB_ERROR(NULL, "Unable to find port info for packet\n");
    return NULL;
}

static uint64_t get_proc_info(const char *file_name, const char *search_mask, char *buf, size_t buf_len)
{
    uint64_t ui64Ret = (uint64_t)0;
    FILE *ptOctInfo = fopen(file_name, "r");

    if(ptOctInfo != NULL)
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, ptOctInfo)) != -1)
        {
            if(strncmp(line, search_mask, strlen(search_mask)) == 0)
            {
                char *pt = line + strlen(search_mask);

                pt = strchr(pt, ':');
                if(pt++)
                {
                    while(isspace(*pt))
                        pt++;

                    if(isdigit(*pt))
                    {
                        ui64Ret = atoll(pt);
                        break;
                    }
                    else if(buf != NULL)
                    {
                        strncpy(buf, pt, buf_len-1);
                        buf[buf_len-1] = '\0';
                        pt = strchr(buf, '\n');
                        if(pt)
                            *pt = '\0';

                        break;
                    }
                }
            }
        }

        fclose(ptOctInfo);

        if(line)
            free(line);
    }

    return ui64Ret;
}

static uint64_t get_io_clock_rate(void)
{
    static uint64_t ret_io_clock_rate = 0;
    
    ret_io_clock_rate = get_proc_info("/proc/octeon_info", "io_clock_hz", NULL, 0);

    return ret_io_clock_rate;
}

char *get_system_type(void)
{
    static char ptSystemType[50] = {0};

    if(*ptSystemType == '\0')
        get_proc_info("/proc/cpuinfo", "system type", ptSystemType, sizeof(ptSystemType));

    return (*ptSystemType != '\0') ? ptSystemType : NULL;
}

/**
 * Cofigure a clock output
 * 
 * @param clock_src
 * @param port_num
 */
static void ptplib_setup_ptp_input(int clock_source, int clock_num)
{
    volatile uint64_t *CLOCK_CFG    = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_CFG;
    volatile uint64_t *ptp_clock_comp = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_COMP;

    const uint64_t compensation_value = 0x3B9AC9FFE8287C00llu;

    uint64_t int_clk = 0ull;
    char *sys_type;
    uint64_t ext_clk_in_value = 0ull;
    uint64_t ext_clk_en_value = 1ull;
    ptplib_board_t board_type;
    if(clock_source == PTPLIB_USE_DEFAULT_CLOCK)
        return;

    if((clock_source == PTPLIB_USE_QLM_CLOCK) && (clock_num > 19))
        return;

    int_clk = get_io_clock_rate();
    sys_type = get_system_type();

// CN68xx:
//   0x00-0x0F = GPIOn, where n is EXT_CLK_IN<3:0>
//   0x10 = QLM0_REF_CLK
//   0x11 = QLM1_REF_CLK
//   0x12 = QLM2_REF_CLK
//   0x13 = QLM3_REF_CLK
//   0x14 = QLM4_REF_CLK
//   0x20 =GPIO<16>
//   0x21 =GPIO<17>
//   0x22 =GPIO<18>
//   0x23 =GPIO<19>
//
// CN78xx:
//   0x0 + Î± - GPIO(0..19) Normal GPIO inputs.
//   0x20 + Î± - QLM_REF(0..7) QLM0-QLM7 reference clock

    if(int_clk && sys_type)
    {
        if(strncmp(sys_type, PTPLIB_BOARD_IM8724_STR, strlen(PTPLIB_BOARD_IM8724_STR)) == 0)
            board_type = PTPLIB_BOARD_IM8724;
        else if(strncmp(sys_type, PTPLIB_BOARD_N822_STR, strlen(PTPLIB_BOARD_N822_STR)) == 0)
            board_type = PTPLIB_BOARD_N822;
        else
        {
            board_type = PTPLIB_BOARD_UNSUPPORTED;
            return;
        }

        switch(clock_source)
        {
            case PTPLIB_USE_QLM_CLOCK:
                int_clk = 156250000ull;
                if(board_type == PTPLIB_BOARD_IM8724)
                {
                    if(clock_num > 7)
                        return;

                    ext_clk_in_value = 0x20 + clock_num;
                }
                else if(board_type == PTPLIB_BOARD_N822)
                {
                    if(clock_num > 4)
                        return;

                    ext_clk_in_value = 0x10 + clock_num;
                }
                break;
            case PTPLIB_USE_GPIO_CLOCK: // use GPIO clock
                int_clk = 100000000ull;
                if(board_type == PTPLIB_BOARD_IM8724)
                {
                    if(clock_num > 19)
                        return;

                    ext_clk_in_value = 0x00 + clock_num;
                }
                else if(board_type == PTPLIB_BOARD_N822)
                {
                    if(clock_num > 19)
                        return;

                    if(clock_num < 16)
                        ext_clk_in_value = 0x00 + clock_num;
                    else
                        ext_clk_in_value = 0x20 + clock_num;
                }
                break;
            default: // use io clock
                ext_clk_en_value = 0ull;
                break;
        }

        ptplib_mio_ptp_clock_cfg_t ptp_clock_cfg;
        ptp_clock_cfg.u64 = *CLOCK_CFG;

        ptp_clock_cfg.s.ext_clk_in = ext_clk_in_value;
        ptp_clock_cfg.s.ext_clk_en = ext_clk_en_value;
        *ptp_clock_comp = compensation_value / int_clk;
        *CLOCK_CFG = ptp_clock_cfg.u64;
    }
}

/**
 * Cofigure a clock output
 *
 * @param use_pps
 * @param gpio
 * @param frequency
 * @param hi_duty_percent
 */
static void ptplib_setup_ptp_output(int use_pps, int gpio, int frequency, int hi_duty_percent)
{
    volatile uint64_t *CLOCK_CFG    = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_CFG;
    volatile uint64_t *CLOCK_HI     = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_HI;
    volatile uint64_t *HI_INCR      = (volatile uint64_t *)((use_pps) ? PTPLIB_MIO_PTP_PPS_HI_INCR : PTPLIB_MIO_PTP_CKOUT_HI_INCR);
    volatile uint64_t *LO_INCR      = (volatile uint64_t *)((use_pps) ? PTPLIB_MIO_PTP_PPS_LO_INCR : PTPLIB_MIO_PTP_CKOUT_LO_INCR);
    volatile uint64_t *THRESH_HI    = (volatile uint64_t *)((use_pps) ? PTPLIB_MIO_PTP_PPS_THRESH_HI : PTPLIB_MIO_PTP_CKOUT_THRESH_HI);
    volatile uint64_t *THRESH_LO    = (volatile uint64_t *)((use_pps) ? PTPLIB_MIO_PTP_PPS_THRESH_LO : PTPLIB_MIO_PTP_CKOUT_THRESH_LO);

    printf("Setting up %dHz clock signal on GPIO %d, please wait...\n", frequency, gpio);
    *(volatile uint64_t *)PTPLIB_GPIO_BIT_CFG(gpio) = 1;

    /* Set the rate */
    uint64_t len_one_sec = 1000000000ull<<32;
    uint64_t clk_incr = len_one_sec / frequency;
    uint64_t clk_hi_incr = clk_incr * hi_duty_percent / 100;
    *THRESH_HI = -1;
    *THRESH_LO = -1;
    *HI_INCR = clk_hi_incr;
    *LO_INCR = clk_incr - clk_hi_incr;

    /* Determine starting phase. Clocks are assumed to be phase
        aligned starting at time zero */
    ptplib_mio_ptp_clock_cfg_t ptp_clock_cfg;
    ptp_clock_cfg.u64 = *CLOCK_CFG;
    uint64_t hi = 0;
    uint64_t lo = 0;
    int is_hi = (use_pps) ? ptp_clock_cfg.s.pps : ptp_clock_cfg.s.ckout;
    if (is_hi)
    {
        /* Clock is currently high, so first edge is the low one after
            clk_hi_incr time has passed */
        hi = clk_hi_incr >> 32;
        lo = clk_hi_incr & 0xffffffff;
    }
    /* Skip clock cycles until we catch up with real time. We actually
        start one second after the current time in case teh calculations
        take extra time */
    do
    {
        uint64_t start_time = *CLOCK_HI + NSEC;
        uint64_t factor = ((start_time - hi) << 32) / clk_incr;
        factor += 1000;
        uint64_t fractions = clk_incr & 0xffffffff;
        fractions *= factor;
        lo += fractions;
        hi += fractions >> 32;
        hi += (clk_incr>>32) * factor;
        lo &= 0xffffffff;
    } while (hi < *CLOCK_HI);

    /* Setup the output */
    if (use_pps)
    {
        ptp_clock_cfg.s.pps_out = gpio;
        ptp_clock_cfg.s.pps_inv = 0;
        ptp_clock_cfg.s.pps_en = 1;
    }
    else
    {
        ptp_clock_cfg.s.ckout_out4 = gpio>>4;
        ptp_clock_cfg.s.ckout_out = gpio;
        ptp_clock_cfg.s.ckout_inv = 0;
        ptp_clock_cfg.s.ckout_en = 1;
    }
    *CLOCK_CFG = ptp_clock_cfg.u64;
    /* Force ordering */
    *CLOCK_CFG;
    /* Program the next transition */
    *THRESH_HI = hi;
    *THRESH_LO = lo;
}

/**
 * Adjust the system time in one big jump
 *
 * @param new_time New system time
 */
static void ptplib_set_systime(timestamp_t new_time)
{
    timestamp_set_systime(new_time);
    /* Configure the PPS output */
    if (PTP_GPIO_PPS_OUT != -1)
        ptplib_setup_ptp_output(1, PTP_GPIO_PPS_OUT, PTP_GPIO_PPS_HZ, 1);
    /* Configure the clock output */
    if (PTP_GPIO_CLOCK_OUT != -1)
        ptplib_setup_ptp_output(0, PTP_GPIO_CLOCK_OUT, PTP_GPIO_CLOCK_HZ, 50);
}

/**
 * Send a PTP/1588 packet
 *
 * @param pinfo  Port to send on
 * @param ptp    Packet to send
 * @param addr   Destination address info, or NULL for multicast
 *
 * @return The software time the packet was sent at, or TIMSTAMP_BAD
 *         on failure.
 */
static timestamp_t ptplib_send_packet(port_info_t *pinfo, const ptp_packet_t *ptp, const char *addr)
{
    packetio_packet_t packet;

    memset(&packet, 0, sizeof(packet));

    packet.packetio = pinfo->packetio;
    packet.length = ntohs(ptp->header.messageLength);
    memcpy(packet.buffer, ptp, packet.length);
    packet.header = (ptp_header_t *)packet.buffer;
    packet.data = packet.header + 1;

    if (packetio_send(&packet, addr))
        return TIMESTAMP_BAD;
    else
        return packet.timestamp;
}


/**
 * Lookup a remote clock in our internal list. Create a new entry
 * if the clock isn't already in the list.
 *
 * @param ptplib Our state info
 * @param portId Clock port source identifier from the PTP header
 *
 * @return Pointer to clock info, or NULL.
 */
static clock_info_t *ptplib_clock_get(ptplib_t *ptplib, const packetio_packet_t *rx_packet, int allow_create)
{
    int index;
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const ptp_portIdentity_t *portId = &rx_packet->header->sourcePortIdentity;

    if (ptplib->clock[0].id == portId->clockIdentity)
    {
        PTPLIB_ERROR(pinfo, "Clock using same clockIdentity as local clock\n");
        return NULL;
    }

    /* Search for an existing clock entry */
    for (index=0; index<MAX_CLOCKS; index++)
    {
        if (ptplib->clock[index].id == portId->clockIdentity)
            return &ptplib->clock[index];
    }

    if (!allow_create)
        return NULL;

    /* Search for a place to create a new clock entry */
    for (index=0; index<MAX_CLOCKS; index++)
    {
        clock_info_t *clk = &ptplib->clock[index];
        if (clk->id == 0)
        {
            /* Create the new clock entry */
            memset(clk, 0, sizeof(*clk));
            clk->id = portId->clockIdentity;
            clk->sourcePortIdentity = *portId;
            clk->last_announce = TIMESTAMP_BAD;
            timestamp_t delta = TIMESTAMP_ZERO;
            delta.nanoseconds = NSEC<<pinfo->portDS.logAnnounceInterval;
            timestamp_avg_update(&clk->announce_interval, delta, AVERAGE_SHIFT_ANNOUNCE_INTERVAL);
            clk->pinfo = pinfo;
            return clk;
        }
    }

    PTPLIB_ERROR(pinfo, "New clock, but we're out of space\n");
    return NULL;
}


/**
 * Synchronize our local clock with the master clock
 *
 * @param ptplib Our state info
 * @param port   Port that initiated synchronization
 *
 * @return Zero on success
 */
static int ptplib_update_sync(ptplib_t *ptplib, port_info_t *pinfo)
{
    timestamp_t tmp;
    int64_t masterDiff;
    int64_t localDiff;
    timestamp_t delay;
    timestamp_t driftFromMaster;
    timestamp_t offsetFromMaster;

    /* We need at least two syncw before we can start synchronization */
    if (timestamp_is_bad(ptplib->sync.last_t2_received))
        return 0;
    if (timestamp_is_bad(ptplib->sync.last_t1_sent))
        return 0;

    /* Correction fields and follow up tiemstamps were already included into
        the port sync timestamps */

    /* Figure out how much time elapsed between sync pulses based on our local clock */
    tmp = timestamp_sub(ptplib->sync.t2_received, ptplib->sync.last_t2_received);
    /* Out of range values means our clock was adjusted between the syncs.
        Ignore this update */
    if ((tmp.nanoseconds < 0) || (tmp.nanoseconds > INT32_MAX))
        return 0;
    localDiff = (tmp.nanoseconds<<32) + tmp.fractions;
    if (!localDiff)
        return 0;

    /* Figure out how much time elapsed between sync pulses based on the master clock */
    tmp = timestamp_sub(ptplib->sync.t1_sent, ptplib->sync.last_t1_sent);
    /* Out of range values means the master clock was adjusted between the
        syncs. Ignore this update */
    if ((tmp.nanoseconds < 0) || (tmp.nanoseconds > INT32_MAX))
        return 0;
    masterDiff = (tmp.nanoseconds<<32) + tmp.fractions;
    if (!masterDiff)
        return 0;

    /* Calculate our clock rate difference from the master in parts per billion */
    driftFromMaster.nanoseconds = (localDiff - masterDiff) / (masterDiff >> 32);
    driftFromMaster.nanoseconds *= 1000000000;
    driftFromMaster.fractions = driftFromMaster.nanoseconds;
    driftFromMaster.nanoseconds >>= 32;
    timestamp_avg_update(&ptplib->driftFromMaster, driftFromMaster, AVERAGE_SHIFT_DRIFTFROMMASTER);

    /* Calculate our offset from the master clock. The offset is the difference
        between when we receive a sync versus when the sync was sent after
        the path delay has been taken into account. The path delay from the
        master to the slave is defined as the meanPathDelay plus the
        delayAsymmetry (Section 7.4.2) */
    tmp = timestamp_sub(ptplib->sync.t2_received, ptplib->sync.t1_sent);
    /* Don't use meanPathDelay if it is out of the range 0 - 1sec */
    if (pinfo->portDS.delayMechanism == PTP_PORT_DELAY_P2P)
        delay = timestamp_avg_get_avg(&pinfo->portDS.peerMeanPathDelay);
    else if (pinfo->portDS.delayMechanism == PTP_PORT_DELAY_E2E)
        delay = timestamp_avg_get_avg(&ptplib->currentDS.meanPathDelay);
    else
        delay = TIMESTAMP_ZERO;
    if (timestamp_in_range(delay, 0, NSEC))
        tmp = timestamp_sub(tmp, delay);

    timestamp_avg_update(&ptplib->currentDS.offsetFromMaster, timestamp_sub(tmp, pinfo->delayAsymmetry), AVERAGE_SHIFT_OFFSETFROMMASTER);

    /* Don't allow synchronization until port state is correct */
    if ((pinfo->portDS.portState != PTP_PORT_STATE_UNCALIBRATED) &&
        (pinfo->portDS.portState != PTP_PORT_STATE_SLAVE))
        return 0;

    /* We only need to get the average offsetFromMaster if we are
        synchronizing the clock phase */
    if (pinfo->portDS.delayMechanism != PTP_PORT_DELAY_DISABLED)
    {
        offsetFromMaster = timestamp_avg_get_min(&ptplib->currentDS.offsetFromMaster);
        if (!timestamp_in_range(offsetFromMaster, -MAX_OFFSET_FOR_SLEW, MAX_OFFSET_FOR_SLEW))
        {
            PTPLIB_INFO(pinfo, "Offset too high, setting the time to match master clock\n");
            ptplib_set_systime(timestamp_sub(timestamp_get_systime(1), offsetFromMaster));
            /* This forces a reset of statistics */
            timestamp_avg_update(&ptplib->currentDS.offsetFromMaster, TIMESTAMP_ZERO, AVERAGE_SHIFT_OFFSETFROMMASTER+1);
            offsetFromMaster = TIMESTAMP_ZERO;
        }
    }
    else
        offsetFromMaster = TIMESTAMP_ZERO;

    /* We only need to get the driftFromMaster if we are synchronizing the
        clock frequency */
    if (ptplib->flags & PTPLIB_FLAGS_USE_FREQUENCY_ADJUST)
        driftFromMaster = timestamp_avg_get_min(&ptplib->driftFromMaster);
    else
        driftFromMaster = TIMESTAMP_ZERO;

    /* Update the clock */
    if (driftFromMaster.nanoseconds || offsetFromMaster.nanoseconds)
        timestamp_adjust_systime(driftFromMaster.nanoseconds, offsetFromMaster);

    return 0;
}


/**
 * Choose which master we should synchronize with. This will
 * return the local clock if no sutable clock is available.
 * This code is based on Section 9.3.
 *
 * @param ptplib Our state info
 *
 * @return Zero on success
 */
static int ptplib_choose_master(ptplib_t *ptplib)
{
    clock_info_t *best = &ptplib->clock[0];
    const ptp_message_announce_t *best_announce;
    int best_stepsRemoved = 0;
    int clock_index;
    timestamp_t now = timestamp_get_rawtime();

    for (clock_index=1; clock_index<MAX_CLOCKS; clock_index++)
    {
        clock_info_t *compare = &ptplib->clock[clock_index];
        if (!compare->id)
            continue;

        /* We must have received an announce within the PTP_FOREIGN_MASTER_TIME_WINDOW */
        timestamp_t expire = timestamp_sub(now, compare->last_announce);
        if (expire.nanoseconds > NSEC * PTP_FOREIGN_MASTER_TIME_WINDOW)
        {
            compare->id = 0;
            continue;
        }

        /* The interval between announces must average to allow the minimum number
            of announce messages defined by PTP_FOREIGN_MASTER_THRESHOLD */
        timestamp_t announce_interval = timestamp_avg_get_avg(&compare->announce_interval);
        if (!timestamp_in_range(announce_interval, 0, (NSEC * PTP_FOREIGN_MASTER_TIME_WINDOW) / (PTP_FOREIGN_MASTER_THRESHOLD-1)))
            continue;

        /* The local clock is special in that it is always zero steps away
            and compares against it are really against the current master */
        if (best == &ptplib->clock[0])
        {
            best_announce = &ptplib->best_clock->announce;
            if (ptplib->best_clock != &ptplib->clock[0])
                best_stepsRemoved = htons(ntohs(best_announce->stepsRemoved)+1);
            else
                best_stepsRemoved = 0;
        }
        else
        {
            best_announce = &best->announce;
            best_stepsRemoved = best_announce->stepsRemoved;
        }

        if (best_announce->grandmasterIdentity == compare->announce.grandmasterIdentity)
        {
            /* These checks are from Figure 28, page 90 */
            if (ntohs(best_stepsRemoved) < ntohs(compare->announce.stepsRemoved))
            {
                /* Current best is a better clock as it is closer to the master */
                continue;
            }
            if (ntohs(compare->announce.stepsRemoved) < ntohs(best_stepsRemoved))
            {
                /* Compare is a better clock as it is closer to the master */
                best = compare;
                continue;
            }
            if (ntohq(best->id) < ntohq(compare->id))
            {
                continue;
            }
            if (ntohq(compare->id) < ntohq(best->id))
            {
                best = compare;
                continue;
            }
            if (ntohs(best->pinfo->portDS.portIdentity.portNumber) < ntohs(compare->pinfo->portDS.portIdentity.portNumber))
            {
                continue;
            }
            if (ntohs(compare->pinfo->portDS.portIdentity.portNumber) < ntohs(best->pinfo->portDS.portIdentity.portNumber))
            {
                best = compare;
                continue;
            }
            PTPLIB_ERROR(best->pinfo, "Somehow we got two clocks on the same port with the same ID\n");
        }
        else
        {
            /* These checks are from Figure 27, page 89 */
            if (best_announce->grandmasterPriority1 < compare->announce.grandmasterPriority1)
            {
                /* Current best is a better clock as its priority is better (lower) */
                continue;
            }
            if (best_announce->grandmasterPriority1 > compare->announce.grandmasterPriority1)
            {
                /* Compare is a better clock as its priority is better (lower) */
                best = compare;
                continue;
            }
            if (best_announce->grandmasterClockQuality.clockClass < compare->announce.grandmasterClockQuality.clockClass)
            {
                /* Current best is a better clock as its class is better (lower) */
                continue;
            }
            if (best_announce->grandmasterClockQuality.clockClass > compare->announce.grandmasterClockQuality.clockClass)
            {
                /* Compare is a better clock as its class is better (lower) */
                best = compare;
                continue;
            }
            if (best_announce->grandmasterClockQuality.clockAccuracy < compare->announce.grandmasterClockQuality.clockAccuracy)
            {
                /* Current best is a better clock as its accuracy is better (lower) */
                continue;
            }
            if (best_announce->grandmasterClockQuality.clockAccuracy > compare->announce.grandmasterClockQuality.clockAccuracy)
            {
                /* Compare is a better clock as its accuracy is better (lower) */
                best = compare;
                continue;
            }
            if (ntohs(best_announce->grandmasterClockQuality.offsetScaledLogVariance) < ntohs(compare->announce.grandmasterClockQuality.offsetScaledLogVariance))
            {
                /* Current best is a better clock as its offsetScaledLogVariance is better (lower) */
                continue;
            }
            if (ntohs(best_announce->grandmasterClockQuality.offsetScaledLogVariance) > ntohs(compare->announce.grandmasterClockQuality.offsetScaledLogVariance))
            {
                /* Compare is a better clock as its offsetScaledLogVariance is better (lower) */
                best = compare;
                continue;
            }
            if (best_announce->grandmasterPriority2 < compare->announce.grandmasterPriority2)
            {
                /* Current best is a better clock as its priority is better (lower) */
                continue;
            }
            if (best_announce->grandmasterPriority2 > compare->announce.grandmasterPriority2)
            {
                /* Compare is a better clock as its priority is better (lower) */
                best = compare;
                continue;
            }
            if (ntohq(best_announce->grandmasterIdentity) > ntohq(compare->announce.grandmasterIdentity))
            {
                /* Compare is a better clock based solely on the identity (lower) */
                best = compare;
                continue;
            }
        }
    }
    if (best != ptplib->best_clock)
    {
        ptplib->best_clock = best;
        ptplib->pathTraceSize = 0;
        /* This forces a reset of statistics */
        timestamp_avg_update(&ptplib->currentDS.offsetFromMaster, TIMESTAMP_ZERO, AVERAGE_SHIFT_OFFSETFROMMASTER+1);
        timestamp_avg_update(&ptplib->driftFromMaster, TIMESTAMP_ZERO, AVERAGE_SHIFT_DRIFTFROMMASTER+1);
        PTPLIB_INFO(best->pinfo, "Using clock ID:0x%016llx\n", (ULL)ntohq(best->id));
    }
    if (ptplib->best_clock != &ptplib->clock[0])
    {
        ptplib->currentDS.stepsRemoved = best_stepsRemoved + 1;
        ptplib->parentDS.parentPortIdentity = ptplib->best_clock->sourcePortIdentity;
        ptplib->parentDS.parentStats = 0;
        ptplib->parentDS.observedParentOffsetScaledLogVariance = ptplib->defaultDS.clockQuality.offsetScaledLogVariance;
        ptplib->parentDS.observedParentClockPhaseChangeRate = 0; // FIXME
        ptplib->parentDS.grandmasterIdentity = ptplib->best_clock->announce.grandmasterIdentity;
        ptplib->parentDS.grandmasterClockQuality = ptplib->best_clock->announce.grandmasterClockQuality;
        ptplib->parentDS.grandmasterPriority1 = ptplib->best_clock->announce.grandmasterPriority1;
        ptplib->parentDS.grandmasterPriority2 = ptplib->best_clock->announce.grandmasterPriority2;
    }
    else
    {
        ptplib->pathTraceSize = 0;
        ptplib->currentDS.stepsRemoved = 0;
        ptplib->parentDS.parentPortIdentity = ptplib->best_clock->pinfo->portDS.portIdentity;
        ptplib->parentDS.parentStats = 0;
        ptplib->parentDS.observedParentOffsetScaledLogVariance = ptplib->defaultDS.clockQuality.offsetScaledLogVariance;
        ptplib->parentDS.observedParentClockPhaseChangeRate = 0;  // FIXME
        ptplib->parentDS.grandmasterIdentity = ptplib->defaultDS.clockIdentity;
        ptplib->parentDS.grandmasterClockQuality = ptplib->defaultDS.clockQuality;
        ptplib->parentDS.grandmasterPriority1 = ptplib->defaultDS.priority1;
        ptplib->parentDS.grandmasterPriority2 = ptplib->defaultDS.priority2;
    }
    return 0;
}


/**
 * Process an incomming PTP sync message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_sync(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    const ptp_message_sync_t *sync = rx_packet->data;
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    clock_info_t *clk = ptplib_clock_get(ptplib, rx_packet, 0);

    /* Ignore syncs from clocks that aren't the master */
    if (clk != ptplib->best_clock)
        return -1;

    /* Silently drop duplicate sync packets */
    if (ptplib->sync.last_sequenceId == ntohs(rx_packet->header->sequenceId))
        return 0;
    /* Silently drop sync packets from a different master port */
    if (STRUCT_MISMATCH(clk->sourcePortIdentity, rx_packet->header->sourcePortIdentity))
        return 0;

    timestamp_t sent = timestamp_from_ptp(sync->originTimestamp);
    sent = timestamp_add(sent, timestamp_from_correction(rx_packet->header->correctionField));

    /* Record the sync sequence number and timing information */
    ptplib->sync.last_sequenceId = ntohs(rx_packet->header->sequenceId);
    ptplib->sync.last_t1_sent = ptplib->sync.t1_sent;
    ptplib->sync.last_t2_received = ptplib->sync.t2_received;
    ptplib->sync.t1_sent = sent;
    ptplib->sync.t2_received = rx_packet->timestamp;
    memcpy(clk->last_sync_rxinfo, rx_packet->rxinfo, sizeof(clk->last_sync_rxinfo));
    if (ntohs(rx_packet->header->flagField) & PTP_FLAGFIELD_TWOSTEPFLAG)
    {
        /* pinfo->sync_t1_sent will be update when we receive the follow up */
        return 0;
    }
    else
        return ptplib_update_sync(ptplib, pinfo);
}


/**
 * Process an incomming PTP delay request message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_delay_req(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    ptp_packet_t tx_packet;
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const char *unicast_destination;

    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_DELAY_RESP;
    tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_delay_resp_t));
    tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
    tx_packet.header.flagField          = htons(0);
    tx_packet.header.correctionField    = rx_packet->header->correctionField;
    tx_packet.header.sourcePortIdentity = pinfo->portDS.portIdentity;
    tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
    tx_packet.header.controlField       = PTP_CONTROLFIELD_DELAY_RESP; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = pinfo->portDS.logMinDelayReqInterval;
    if (pinfo->flags & PORT_FLAG_USE_UNICAST)
    {
        unicast_destination = rx_packet->rxinfo;
        tx_packet.header.flagField |= htons(PTP_FLAGFIELD_UNICASTFLAG);
    }
    else
        unicast_destination = NULL;

    /* Fill in the PTP delay response */
    tx_packet.ptp.dresp.receiveTimestamp = timestamp_to_ptp(rx_packet->timestamp);
    tx_packet.ptp.dresp.requestingPortIdentity = rx_packet->header->sourcePortIdentity;

    /* Subtract any fractional nanosecond portion of RX timestamp (t4) from
        the correctionField of the Delay_Resp message. We convert everything
        to timestamp_t so we get MIN/MAX checking */
    timestamp_t fractional = rx_packet->timestamp;
    fractional.nanoseconds = 0;
    timestamp_t correction = timestamp_from_correction(tx_packet.header.correctionField);
    correction = timestamp_sub(correction, fractional);
    tx_packet.header.correctionField = timestamp_to_correction(correction);

    /* Delay responses are never two step since their send time isn't important */
    if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, unicast_destination)))
    {
        PTPLIB_ERROR(pinfo, "Unable to send delay response\n");
        return -1;
    }
    else
        return 0;
}


/**
 * Process an incomming PTP peer delay request message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_pdelay_req(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    ptp_packet_t tx_packet;
    const char *unicast_destination;

    memset(&tx_packet, 0, sizeof(tx_packet));
    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_PDELAY_RESP;
    tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_pdelay_resp_t));
    tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
    tx_packet.header.correctionField    = rx_packet->header->correctionField;
    tx_packet.header.sourcePortIdentity = pinfo->portDS.portIdentity;
    tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;

    /* Fill in the PTP peer delay response */
    tx_packet.ptp.pdresp.requestingPortIdentity = rx_packet->header->sourcePortIdentity;

    if (pinfo->flags & PORT_FLAG_USE_TWOSTEP)
    {
        tx_packet.header.flagField = htons(PTP_FLAGFIELD_TWOSTEPFLAG);
        tx_packet.ptp.pdresp.requestReceiveTimestamp = timestamp_to_ptp(rx_packet->timestamp);
        /* Subtract any fractional nanosecond portion of RX timestamp (t4) from
            the correctionField of the Delay_Resp message. We convert everything
            to timestamp_t so we get MIN/MAX checking */
        timestamp_t fractional = rx_packet->timestamp;
        fractional.nanoseconds = 0;
        timestamp_t correction = timestamp_from_correction(tx_packet.header.correctionField);
        correction = timestamp_sub(correction, fractional);
        tx_packet.header.correctionField = timestamp_to_correction(correction);
    }
    else
    {
        tx_packet.header.flagField = htons(0);
        tx_packet.ptp.pdresp.requestReceiveTimestamp = timestamp_to_ptp(TIMESTAMP_ZERO);
        timestamp_t processingTime = timestamp_sub(timestamp_get_systime(1), rx_packet->timestamp);
        tx_packet.header.correctionField += timestamp_to_correction(processingTime);
    }

    if (pinfo->flags & PORT_FLAG_USE_UNICAST)
    {
        unicast_destination = rx_packet->rxinfo;
        tx_packet.header.flagField |= htons(PTP_FLAGFIELD_UNICASTFLAG);
    }
    else
        unicast_destination = NULL;

    /* Peer delay responses need a peer delay response follow up to get the
        transmit time correct */
    timestamp_t send_time = ptplib_send_packet(pinfo, &tx_packet, unicast_destination);
    if (timestamp_is_bad(send_time))
    {
        PTPLIB_ERROR(pinfo, "Unable to send peer delay response\n");
        return -1;
    }

    if ((pinfo->flags & PORT_FLAG_USE_TWOSTEP) == 0)
        return 0;

    if (pinfo->packetio->flags & PACKETIO_FLAG_TX_TIMESTAMP)
        return 0;

    /* Send follow up */
    /* Fill in the PTP header */
    tx_packet.header.messageType        = PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_pdelay_resp_follow_up_t));
    tx_packet.header.correctionField    = rx_packet->header->correctionField;
    tx_packet.header.flagField          &= ~htons(PTP_FLAGFIELD_TWOSTEPFLAG);

    /* Fill in the follow up response */
    tx_packet.ptp.pdresp2.responseOriginTimestamp = timestamp_to_ptp(send_time);
    tx_packet.ptp.pdresp2.requestingPortIdentity = rx_packet->header->sourcePortIdentity;
    timestamp_t correction = timestamp_from_correction(tx_packet.header.correctionField);
    send_time.nanoseconds = 0;
    correction = timestamp_add(correction, send_time);
    tx_packet.header.correctionField = timestamp_to_correction(correction);

    if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, unicast_destination)))
    {
        PTPLIB_ERROR(pinfo, "Unable to send peer delay response followup\n");
        return -1;
    }

    return 0;
}


/**
 * Process an incomming PTP peer delay response message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_pdelay_resp(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const ptp_message_pdelay_resp_t* resp = rx_packet->data;

    /* Drop responses when we don't have a pending request. Duplicates are
        common as peer request are multicast and not all switches understand
        not to forward them */
    if (timestamp_is_bad(pinfo->pdelay.t1_request_sent))
        return 0;

    /* Drop responses for other port identities. Responses can be multicast,
        so we can get responses that were for other people */
    if (STRUCT_MISMATCH(resp->requestingPortIdentity, pinfo->portDS.portIdentity))
        return 0;

    if (ntohs(rx_packet->header->sequenceId) != pinfo->pdelay.sequenceId)
    {
        PTPLIB_ERROR(pinfo, "Peer delay response has incorrect sequenceId\n");
        return -1;
    }

    if (ntohs(rx_packet->header->flagField) & PTP_FLAGFIELD_TWOSTEPFLAG)
    {
        pinfo->pdelay.correctionField_resp = timestamp_from_correction(rx_packet->header->correctionField);
        pinfo->pdelay.t2_request_received = timestamp_from_ptp(resp->requestReceiveTimestamp);
        pinfo->pdelay.t4_reply_received = rx_packet->timestamp;
    }
    else
    {
        pinfo->pdelay.t4_reply_received = rx_packet->timestamp;
        /* <meanPathDelay> = [(t4 - t1) - correctionField of Pdelay_Resp]/2 */
        timestamp_t tmp = timestamp_sub(pinfo->pdelay.t4_reply_received, pinfo->pdelay.t1_request_sent);
        tmp = timestamp_sub(tmp, timestamp_from_correction(rx_packet->header->correctionField));
        tmp = timestamp_half(tmp);
        if (timestamp_in_range(tmp, 0, NSEC))
            timestamp_avg_update(&pinfo->portDS.peerMeanPathDelay, tmp, AVERAGE_SHIFT_PEERMEANPATHDELAY);
        pinfo->pdelay.t1_request_sent = TIMESTAMP_BAD;
        pinfo->pdelay.t2_request_received = TIMESTAMP_BAD;
        pinfo->pdelay.t4_reply_received = TIMESTAMP_BAD;
        pinfo->pdelay.correctionField_resp = TIMESTAMP_BAD;
    }
    return 0;
}


/**
 * Process an incomming PTP sync follow up message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_follow_up(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const ptp_message_follow_up_t* follow = rx_packet->data;
    clock_info_t *clk = ptplib_clock_get(ptplib, rx_packet, 0);

    /* Ignore follow ups from anyone except the master */
    if (clk != ptplib->best_clock)
        return 0;

    /* Silently drop packets from a different master port */
    if (STRUCT_MISMATCH(clk->sourcePortIdentity, rx_packet->header->sourcePortIdentity))
        return 0;

    if (timestamp_is_bad(ptplib->sync.t2_received))
    {
        PTPLIB_ERROR(pinfo, "Follow up without sync\n");
        return -1;
    }

    if (ntohs(rx_packet->header->sequenceId) != ptplib->sync.last_sequenceId)
    {
        PTPLIB_ERROR(pinfo, "Follow up has incorrect sequenceId\n");
        return -1;
    }
    ptplib->sync.t1_sent = timestamp_add(timestamp_from_ptp(follow->preciseOriginTimestamp),
        timestamp_from_correction(rx_packet->header->correctionField));

    return ptplib_update_sync(ptplib, pinfo);
}


/**
 * Process an incomming PTP delay response message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_delay_resp(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    clock_info_t *master = ptplib->best_clock;
    const ptp_message_delay_resp_t* resp = rx_packet->data;

    /* Drop all answers that aren't from our master */
    if (rx_packet->header->sourcePortIdentity.clockIdentity != master->id)
        return 0;

    /* Drop responses for other port identities. Responses can be multicast,
        so we can get responses that were for other people */
    if (STRUCT_MISMATCH(resp->requestingPortIdentity, pinfo->portDS.portIdentity))
        return 0;

    if (ntohs(rx_packet->header->sequenceId) != ptplib->delay.sequenceId)
    {
        PTPLIB_ERROR(pinfo, "Delay response has incorrect sequenceId\n");
        return -1;
    }
    if (timestamp_is_bad(ptplib->delay.request_sent))
    {
        PTPLIB_ERROR(pinfo, "Delay response when request wasn't timestamped\n");
        return -1;
    }

    /* <meanPathDelay> = [(t2 ­ t1) + (t4 ­ t3)]/2 = [(t2 ­ t3) + (t4 ­ t1)]/2 */
    timestamp_t tmp = timestamp_sub(ptplib->sync.t2_received, ptplib->sync.t1_sent);
    timestamp_t tmp2 = timestamp_sub(timestamp_from_ptp(resp->receiveTimestamp),
        timestamp_from_correction(rx_packet->header->correctionField));
    tmp2 = timestamp_sub(tmp2, ptplib->delay.request_sent);
    tmp = timestamp_add(tmp, tmp2);
    tmp = timestamp_half(tmp);
    if (timestamp_in_range(tmp, 0, NSEC))
        timestamp_avg_update(&ptplib->currentDS.meanPathDelay, tmp, AVERAGE_SHIFT_MEANPATHDELAY);
    ptplib->delay.request_sent = TIMESTAMP_BAD;
    return 0;
}


/**
 * Process an incomming PTP peer delay response follow up message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_resp_follow_up(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const ptp_message_pdelay_resp_follow_up_t* follow = rx_packet->data;

    /* Drop follow ups when we don't have a previous response. Duplicates are
        common as peer request are multicast and not all switches understand
        not to forward them */
    if (timestamp_is_bad(pinfo->pdelay.t4_reply_received))
        return 0;

    /* Drop responses for other port identities. Responses can be multicast,
        so we can get responses that were for other people */
    if (STRUCT_MISMATCH(follow->requestingPortIdentity, pinfo->portDS.portIdentity))
        return 0;

    if (ntohs(rx_packet->header->sequenceId) != pinfo->pdelay.sequenceId)
    {
        PTPLIB_ERROR(pinfo, "Peer delay response follow up has incorrect sequenceId\n");
        return -1;
    }

    /* <meanPathDelay> = [(t4 - t1) - (responseOriginTimestamp - requestReceiptTimestamp) -
        correctionField of Pdelay_Resp - correctionField of Pdelay_Resp_Follow_Up]/2 */
    timestamp_t tmp, tmp2;
    tmp = timestamp_sub(pinfo->pdelay.t4_reply_received, pinfo->pdelay.t1_request_sent);
    tmp2 = timestamp_sub(timestamp_from_ptp(follow->responseOriginTimestamp), pinfo->pdelay.t2_request_received);
    tmp = timestamp_sub(tmp, tmp2);
    tmp = timestamp_sub(tmp, pinfo->pdelay.correctionField_resp);
    tmp = timestamp_sub(tmp, timestamp_from_correction(rx_packet->header->correctionField));
    tmp = timestamp_half(tmp);
    if (timestamp_in_range(tmp, 0, NSEC))
        timestamp_avg_update(&pinfo->portDS.peerMeanPathDelay, tmp, AVERAGE_SHIFT_PEERMEANPATHDELAY);
    pinfo->pdelay.t1_request_sent = TIMESTAMP_BAD;
    pinfo->pdelay.t2_request_received = TIMESTAMP_BAD;
    pinfo->pdelay.t4_reply_received = TIMESTAMP_BAD;
    pinfo->pdelay.correctionField_resp = TIMESTAMP_BAD;
    return 0;
}


/**
 * Process an incomming PTP announce message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_announce(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    const ptp_message_announce_t *announce = rx_packet->data;
    clock_info_t *clk = ptplib_clock_get(ptplib, rx_packet, 1);

    /* Fail if we didn't have space for a new clock */
    if (!clk)
        return -1;

    /* Ignoring announce messages with stepsRemoved 255 or greater */
    if (ntohs(announce->stepsRemoved) >= 255)
    {
        PTPLIB_ERROR(pinfo, "Announce is too many steps away\n");
        return -1;
    }

    /* Only record it if the announce is from the same master port */
    if (!STRUCT_MISMATCH(clk->sourcePortIdentity, rx_packet->header->sourcePortIdentity))
    {
        timestamp_t now = timestamp_get_rawtime();
        clk->announce = *announce;
        if (!timestamp_is_bad(clk->last_announce))
            timestamp_avg_update(&clk->announce_interval, timestamp_sub(now, clk->last_announce), AVERAGE_SHIFT_ANNOUNCE_INTERVAL);
        clk->last_announce = now;

        /* Check for path trace data */
        ptplib->pathTraceSize = 0;
        uint16_t length = ntohs(rx_packet->header->messageLength);
        if (length > sizeof(ptp_message_announce_t))
        {
            uint16_t tlv_length = ntohs(announce->tlv[0].lengthField);
            uint16_t min_length = sizeof(ptp_header_t) + sizeof(ptp_message_announce_t) + sizeof(ptp_tlv_t) + tlv_length;
            if ((ntohs(announce->tlv[0].tlvType) == PTP_TLVTYPE_PATH_TRACE) && (length >= min_length))
            {
                ptplib->pathTraceSize = tlv_length / sizeof(ptplib->pathTrace[0]);
                if (ptplib->pathTraceSize < PTP_DEFAULT_MAX_PATH_TRACE)
                {
                    memcpy(ptplib->pathTrace, announce->tlv[0].valueField, ptplib->pathTraceSize * sizeof(ptplib->pathTrace[0]));
                }
                else
                {
                    PTPLIB_ERROR(pinfo, "Path trace list is too long (%d)\n", ptplib->pathTraceSize);
                    ptplib->pathTraceSize = 0;
                }
            }
        }
    }
    return 0;
}


/**
 * Get a unicast node structure for the supplied port identification.
 *
 * @param p      Hardware port the node is connected to
 * @param search PTP port idenity for the unicast node
 * @param txinfo Network socket info to talk to the node using unicast
 * @param create True if a new node should be created if one can't be found
 *
 * @return Pointer to unicast node, or NULL on failure
 */
static unicast_node_t *ptplib_get_unicast(port_info_t *p, ptp_portIdentity_t search, const char *txinfo, int create)
{
    unicast_node_t *ptr;

    /* Search for an existing node */
    ptr = p->unicast_head;
    while (ptr)
    {
        if (!STRUCT_MISMATCH(ptr->destPortIdentity, search))
            return ptr;
        ptr = ptr->next;
    }

    /* Return if we can't find it and can't create a new node */
    if (!create)
        return NULL;

    /* Create a new node */
    ptr = malloc(sizeof(unicast_node_t));
    if (!ptr)
    {
        PTPLIB_ERROR(p, "Failed to allocate new unicast node\n");
        return NULL;
    }
    memset(ptr, 0, sizeof(unicast_node_t));
    ptr->destPortIdentity = search;
    if (txinfo)
        memcpy(ptr->txinfo, txinfo, sizeof(ptr->txinfo));
    ptr->announce.lease_expire = TIMESTAMP_BAD;
    ptr->announce.next = TIMESTAMP_BAD;
    ptr->sync.lease_expire = TIMESTAMP_BAD;
    ptr->sync.next = TIMESTAMP_BAD;
    ptr->delay.lease_expire = TIMESTAMP_BAD;

    /* Add the node to the list */
    ptr->prev = p->unicast_tail;
    ptr->next = NULL;
    if (p->unicast_tail)
        p->unicast_tail->next = ptr;
    else
        p->unicast_head = ptr;
    p->unicast_tail = ptr;

    return ptr;
}


/**
 * Send a signal message requesting unicast support
 *
 * @param ptplib    Our state info
 * @param p         Port the request should be sent on
 * @param ask_announce
 *                  True if we should ask for announce messages
 * @param ask_sync  True if we should ask for sync messages
 * @param ask_delay True if we should ask for delay messages
 *
 * @return Zero on success, negative on failure
 */
static int ptplib_send_unicast_request(ptplib_t *ptplib, port_info_t *p, int ask_announce, int ask_sync, int ask_delay)
{
    ptp_packet_t tx_packet;
    timestamp_t send_time;
    const char *unicast_destination;
    clock_info_t *master = ptplib->best_clock;

    memset(&tx_packet, 0, sizeof(tx_packet));

    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.versionPTP         = p->portDS.versionNumber;
    tx_packet.header.domainNumber       = ptplib->defaultDS.domainNumber;
    tx_packet.header.flagField          = htons(0);
    tx_packet.header.correctionField    = 0;
    tx_packet.header.sourcePortIdentity = p->portDS.portIdentity;
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;
    tx_packet.header.sequenceId         = htons(++ptplib->signal.sequenceId);
    tx_packet.header.messageType        = PTP_MESSAGETYPE_SIGNALING;
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */

    if (master == &ptplib->clock[0])
    {
        tx_packet.ptp.sig.targetPortIdentity.clockIdentity = -1;
        tx_packet.ptp.sig.targetPortIdentity.portNumber = -1;
        unicast_destination = NULL;
    }
    else
    {
        tx_packet.ptp.sig.targetPortIdentity = master->sourcePortIdentity;
        unicast_destination = master->last_sync_rxinfo;
        tx_packet.header.flagField |= htons(PTP_FLAGFIELD_UNICASTFLAG);
    }

    ptp_tlv_t *tlv = tx_packet.ptp.sig.tlv;
    if (ask_announce)
    {
        tlv->tlvType = htons(PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION);
        tlv->lengthField = htons(6);
        tlv->valueField[0] = PTP_MESSAGETYPE_ANNOUNCE << 4;
        tlv->valueField[1] = p->portDS.logAnnounceInterval;
        *(uint32_t*)(tlv->valueField + 2) = htonl(PTP_DEFAULT_RENEW_INTERVAL);
        tlv = (ptp_tlv_t *)(((char*)tlv) + sizeof(ptp_tlv_t) + 6);
    }

    if (ask_sync)
    {
        tlv->tlvType = htons(PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION);
        tlv->lengthField = htons(6);
        tlv->valueField[0] = PTP_MESSAGETYPE_SYNC << 4;
        tlv->valueField[1] = p->portDS.logSyncInterval;
        *(uint32_t*)(tlv->valueField + 2) = htonl(PTP_DEFAULT_RENEW_INTERVAL);
        tlv = (ptp_tlv_t *)(((char*)tlv) + sizeof(ptp_tlv_t) + 6);
    }

    if (ask_delay)
    {
        tlv->tlvType = htons(PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION);
        tlv->lengthField = htons(6);
        tlv->valueField[0] = PTP_MESSAGETYPE_DELAY_REQ << 4;
        tlv->valueField[1] = p->portDS.logMinPdelayReqInterval;
        *(uint32_t*)(tlv->valueField + 2) = htonl(PTP_DEFAULT_RENEW_INTERVAL);
        tlv = (ptp_tlv_t *)(((char*)tlv) + sizeof(ptp_tlv_t) + 6);
    }

    tx_packet.header.messageLength = htons(sizeof(ptp_header_t) + sizeof(ptp_message_signaling_t) +
        ((char*)tlv - (char*)tx_packet.ptp.sig.tlv));

    send_time = ptplib_send_packet(p, &tx_packet, unicast_destination);
    if (timestamp_is_bad(send_time))
    {
        PTPLIB_ERROR(p, "Unable to send announce unicast request\n");
        return -1;
    }
    return 0;
}


/**
 * Process an incomming TLV request related to unicast
 *
 * @param ptplib    Our state info
 * @param rx_packet Incomming request packet
 * @param tlv       The TLV being processed
 * @param tlv_response
 *                  The response TLV for this request
 *
 * @return Zero on success, negative on failure
 */
static int ptplib_process_tlv_unicast(ptplib_t *ptplib, const packetio_packet_t *rx_packet, const ptp_tlv_t *tlv, ptp_tlv_t *tlv_response)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    timestamp_t now = timestamp_get_rawtime();
    unicast_node_t *node;

    switch (ntohs(tlv->tlvType))
    {
        case PTP_TLVTYPE_GRANT_UNICAST_TRANSMISSION:
        {
            /* We should only get this message when we've asked a clock for unicast */
            //PTPLIB_DEBUG(pinfo, "Received unicast grant\n");
            tlv_response->lengthField = 0; /* No response to this TLV */
            switch (tlv->valueField[0] >> 4)
            {
                case PTP_MESSAGETYPE_ANNOUNCE:
                    ptplib_send_unicast_request(ptplib, pinfo, 0, 1, 1);
                    break;

                case PTP_MESSAGETYPE_SYNC:
                case PTP_MESSAGETYPE_DELAY_REQ:
                {
                    /* Renew when lease is half expired */
                    int lease_time = ntohl(*(uint32_t*)(tlv->valueField + 2));
                    timestamp_t renew;
                    renew.nanoseconds = NSEC * lease_time / 2;
                    renew.fractions = 0;
                    ptplib->signal.unicast_renew = timestamp_add(now, renew);
                    break;
                }

                default:
                    PTPLIB_ERROR(pinfo, "Invalid message type on unicast grant\n");
                    return -1;
            }
            break;
        }

        case PTP_TLVTYPE_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION:
        {
            /* We should only get this message when we've asked a clock to cancel unicast */
            PTPLIB_DEBUG(pinfo, "Received unicast cancel\n");
            tlv_response->lengthField = 0; /* No response to this TLV */
            break;
        }

        case PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION:
        {
            /* We should get this from peers asking us for unicast */
            //PTPLIB_DEBUG(pinfo, "Received unicast request\n");
            if (tlv->lengthField != htons(6))
            {
                PTPLIB_ERROR(pinfo, "Invalid length on unicast request\n");
                return -1;
            }
            /* Fill in a default response that will be update later */
            tlv_response->tlvType = htons(PTP_TLVTYPE_GRANT_UNICAST_TRANSMISSION);
            tlv_response->lengthField = htons(8);
            memcpy(tlv_response->valueField, tlv->valueField, 6);
            tlv_response->valueField[6] = 0;
            tlv_response->valueField[7] = 1;

            node = ptplib_get_unicast(pinfo, rx_packet->header->sourcePortIdentity, rx_packet->rxinfo, 1);
            if (node)
            {
                /* Validate the lease time */
                int lease_time = ntohl(*(uint32_t*)(tlv->valueField + 2));
                if (lease_time > 1000)
                    lease_time = 1000;
                else if (lease_time < PTP_DEFAULT_RENEW_INTERVAL)
                    lease_time = PTP_DEFAULT_RENEW_INTERVAL;
                *(uint32_t*)(tlv->valueField + 2) = htonl(lease_time);

                timestamp_t renew;
                renew.nanoseconds = NSEC * lease_time;
                renew.fractions = 0;

                /* Figure out which lease is being updated */
                switch (tlv->valueField[0] >> 4)
                {
                    case PTP_MESSAGETYPE_ANNOUNCE:
                        node->announce.interval = (int8_t)tlv->valueField[1];
                        if (node->announce.interval < -3)
                            node->announce.interval = -3;
                        else if (node->announce.interval > 4)
                            node->announce.interval = 4;
                        node->announce.lease_expire = timestamp_add(now, renew);
                        if (timestamp_is_bad(node->announce.next))
                            node->announce.next = now;
                        tlv_response->valueField[1] = node->announce.interval;
                        PTPLIB_INFO(pinfo, "Granted unicast announce for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    case PTP_MESSAGETYPE_SYNC:
                        node->sync.interval = (int8_t)tlv->valueField[1];
                        if (node->sync.interval < -7)
                            node->sync.interval = -7;
                        else if (node->sync.interval > 4)
                            node->sync.interval = 4;
                        node->sync.lease_expire = timestamp_add(now, renew);
                        if (timestamp_is_bad(node->sync.next))
                            node->sync.next = now;
                        tlv_response->valueField[1] = node->sync.interval;
                        PTPLIB_INFO(pinfo, "Granted unicast sync for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    case PTP_MESSAGETYPE_DELAY_REQ:
                        node->delay.lease_expire = timestamp_add(now, renew);
                        PTPLIB_INFO(pinfo, "Granted unicast delay for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    default:
                        PTPLIB_ERROR(pinfo, "Invalid message type on unicast request\n");
                        tlv_response->lengthField = 0;
                        return -1;
                }
            }
            else
            {
                /* Send a deny back by zeroing the lease */
                memset(tlv_response->valueField + 2, 0, 6);
                PTPLIB_ERROR(pinfo, "Unable to allocate a new unicast node\n");
            }
            break;
        }

        case PTP_TLVTYPE_CANCEL_UNICAST_TRANSMISSION:
        {
            /* We should get this from peers asking us to cancel unicast */
            PTPLIB_DEBUG(pinfo, "Received unicast cancel request\n");
            if (tlv->lengthField != htons(2))
            {
                PTPLIB_ERROR(pinfo, "Invalid length on unicast cancel\n");
                return -1;
            }
            tlv_response->tlvType = htons(PTP_TLVTYPE_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION);
            tlv_response->lengthField = htons(2);
            memcpy(tlv_response->valueField, tlv->valueField, 2);
            node = ptplib_get_unicast(pinfo, rx_packet->header->sourcePortIdentity, NULL, 0);
            if (node)
            {
                switch (tlv->valueField[0] >> 4)
                {
                    case PTP_MESSAGETYPE_ANNOUNCE:
                        node->announce.interval = 0;
                        node->announce.lease_expire = TIMESTAMP_BAD;
                        node->announce.next = TIMESTAMP_ZERO;
                        PTPLIB_INFO(pinfo, "Canceling unicast announce for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    case PTP_MESSAGETYPE_SYNC:
                        node->sync.interval = 0;
                        node->sync.lease_expire = TIMESTAMP_BAD;
                        node->sync.next = TIMESTAMP_ZERO;
                        PTPLIB_INFO(pinfo, "Canceling unicast sync for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    case PTP_MESSAGETYPE_DELAY_REQ:
                        node->delay.lease_expire = TIMESTAMP_BAD;
                        PTPLIB_INFO(pinfo, "Canceling unicast delay for %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
                        break;

                    default:
                        PTPLIB_ERROR(pinfo, "Invalid message type on unicast cancel\n");
                        return -1;
                }
            }
            else
            {
                /* Don't send a TLV response for failed cancels */
                tlv_response->lengthField = 0;
                PTPLIB_ERROR(pinfo, "Unable to find unicast node to cancel\n");
            }
            break;
        }
    }
    return 0;
}


/**
 * Process an incomming PTP signaling message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_signaling(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    port_info_t *target_port;
    const ptp_message_signaling_t *signaling = rx_packet->data;
    ptp_packet_t tx_packet;
    //PTPLIB_DEBUG(pinfo, "Received signaling\n");

    if (signaling->targetPortIdentity.clockIdentity == (uint64_t)-1)
    {
        if (signaling->targetPortIdentity.portNumber != (uint16_t)-1)
        {
            PTPLIB_ERROR(pinfo, "Signaling had port number with broadcast clock\n");
            return -1;
        }
        target_port = NULL;
    }
    else if (signaling->targetPortIdentity.clockIdentity == pinfo->portDS.portIdentity.clockIdentity)
    {
        if (signaling->targetPortIdentity.portNumber == (uint16_t)-1)
            target_port = NULL;
        else
        {
            int index;
            for (index=1; index<ptplib->defaultDS.numberPorts; index++)
            {
                target_port = &ptplib->port[index];
                if (signaling->targetPortIdentity.portNumber == target_port->portDS.portIdentity.portNumber)
                    break;
            }
            if (index >= ptplib->defaultDS.numberPorts)
            {
                PTPLIB_ERROR(pinfo, "Signaling had invalid port number\n");
                return -1;
            }
        }
    }
    else
    {
        PTPLIB_DEBUG(pinfo, "Signaling was not for me\n");
        return 0;
    }

    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_SIGNALING;
    tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
    tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
    tx_packet.header.correctionField    = rx_packet->header->correctionField;
    tx_packet.header.sourcePortIdentity = pinfo->portDS.portIdentity;
    tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;

    /* Fill in the PTP signal response */
    tx_packet.ptp.sig.targetPortIdentity = rx_packet->header->sourcePortIdentity;

    /* Loop through all TLVs supplied */
    const ptp_tlv_t *tlv = signaling->tlv;
    ptp_tlv_t *tlv_response = tx_packet.ptp.sig.tlv;
    while ((char*)tlv < rx_packet->buffer + rx_packet->length)
    {
        tlv_response->lengthField = 0;

        /* Get the length and make sure it is sane */
        int length = ntohs(tlv->lengthField);
        if ((length < 0) || (length > 256))
        {
            PTPLIB_ERROR(pinfo, "TLV length (%d) is outside of the sanity check\n", length);
            return -1;
        }

        /* Use the type to figure out what to do */
        switch (ntohs(tlv->tlvType))
        {
            case 0:
                /* A zero signals the end of the TLVs */
                break;
            case PTP_TLVTYPE_REQUEST_UNICAST_TRANSMISSION:
            case PTP_TLVTYPE_GRANT_UNICAST_TRANSMISSION:
            case PTP_TLVTYPE_CANCEL_UNICAST_TRANSMISSION:
            case PTP_TLVTYPE_ACKNOWLEDGE_CANCEL_UNICAST_TRANSMISSION:
                if (ptplib->defaultDS.unicastEnable)
                {
                    if (ptplib_process_tlv_unicast(ptplib, rx_packet, tlv, tlv_response))
                        return -1;
                }
                else
                    PTPLIB_INFO(pinfo, "Unicast disabled, so ignoring unicast TLV\n");
                break;
            case PTP_TLVTYPE_MANAGEMENT:
            case PTP_TLVTYPE_MANAGEMENT_ERROR_STATUS:
            case PTP_TLVTYPE_ORGANIZATION_EXTENSION:
            case PTP_TLVTYPE_PATH_TRACE:
            case PTP_TLVTYPE_ALTERNATE_TIME_OFFSET_INDICATOR:
            case PTP_TLVTYPE_AUTHENTICATION:
            case PTP_TLVTYPE_AUTHENTICATION_CHALLENGE:
            case PTP_TLVTYPE_SECURITY_ASSOCIATION_UPDATE:
            case PTP_TLVTYPE_CUM_FREQ_SCALE_FACTOR_OFFSET:
            default:
                PTPLIB_DEBUG(pinfo, "Unsupported TLV type %d skipped\n", ntohs(tlv->tlvType));
                break;
        }
        /* Move to the next TLV */
        tlv = (const ptp_tlv_t *)(((const char*)tlv) + sizeof(ptp_tlv_t) + length);
        if (tlv_response->lengthField)
            tlv_response = (ptp_tlv_t *)(((char*)tlv_response) + sizeof(ptp_tlv_t) + ntohs(tlv_response->lengthField));
    }

    /* Bail if we don't have a response */
    if (tlv_response == tx_packet.ptp.sig.tlv)
        return 0;

    tx_packet.header.messageLength = htons(sizeof(ptp_header_t) + sizeof(ptp_message_signaling_t) +
        ((char*)tlv_response - (char*)tx_packet.ptp.sig.tlv));

    if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, rx_packet->rxinfo)))
    {
        PTPLIB_ERROR(pinfo, "Unable to send signal response\n");
        return -1;
    }
    return 0;
}


/**
 * Build an management error status response
 *
 * @param tx_packet Packet to fill with the response
 * @param id        Management ID the response is for
 * @param error     Error code
 * @param message   Text messaging describing the failure
 */
static void ptplib_build_management_error(ptp_packet_t *tx_packet, ptp_managementId_t id, ptp_managementErrorId_t error, const char *message)
{
    ptp_tlv_t *tlv_response = tx_packet->ptp.mgmt.managementTLV;
    ptp_management_tlv_error_t *mgmt_error = (ptp_management_tlv_error_t *)tlv_response->valueField;

    tlv_response->tlvType = htons(PTP_TLVTYPE_MANAGEMENT_ERROR_STATUS);
    mgmt_error->managementErrorId = htons(error);
    mgmt_error->managementId = htons(id);
    mgmt_error->reserved = 0;
    mgmt_error->displayData.lengthField = htons(strlen(message));
    strcpy((char*)mgmt_error->displayData.textField, message);
    tlv_response->lengthField = sizeof(ptp_management_tlv_error_t) + ntohs(mgmt_error->displayData.lengthField);
}


/**
 * Internal function for packing bytes into a packet.
 *
 * @param ptr    Where to put the bytes
 * @param num    Number of bytes. Must be 1-8
 * @param bytes  Bytes to pack
 *
 * @return Pointer after the bytes
 */
static char *ptplib_pack_bytes(char *ptr, int num, uint64_t bytes)
{
    int shift = (num-1) * 8;
    while (shift >= 0)
    {
        *ptr++ = (bytes >> shift) & 0xff;
        shift -= 8;
    }
    return ptr;
}


/**
 * Internal function for packet PTPText strings into a packet
 *
 * @param ptr    Where to put the string
 * @param str    String to store
 *
 * @return Pointer after the packet string
 */
static char *ptplib_pack_string(char *ptr, const char *str)
{
    int len = strlen(str);
    *ptr++ = len;
    memcpy(ptr, str, len);
    ptr += len;
    return ptr;
}


/**
 * Process an incomming PTP management message.
 *
 * @param ptplib  Our state info
 * @param rx_info Received packet information
 *
 * @return Zero on success, negative on failure.
 */
static int ptplib_process_management(ptplib_t *ptplib, const packetio_packet_t *rx_packet)
{
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    port_info_t *target_port;
    const ptp_message_management_t *management = rx_packet->data;
    ptp_packet_t tx_packet;
    PTPLIB_DEBUG(pinfo, "Received management\n");

    if (management->targetPortIdentity.clockIdentity == (uint64_t)-1)
    {
        if (management->targetPortIdentity.portNumber != (uint16_t)-1)
        {
            PTPLIB_ERROR(pinfo, "Management had port number with broadcast clock\n");
            return -1;
        }
        target_port = NULL;
    }
    else if (management->targetPortIdentity.clockIdentity == pinfo->portDS.portIdentity.clockIdentity)
    {
        if (management->targetPortIdentity.portNumber == (uint16_t)-1)
            target_port = NULL;
        else
        {
            int index;
            for (index=1; index<ptplib->defaultDS.numberPorts; index++)
            {
                target_port = &ptplib->port[index];
                if (management->targetPortIdentity.portNumber == target_port->portDS.portIdentity.portNumber)
                    break;
            }
            if (index >= ptplib->defaultDS.numberPorts)
            {
                PTPLIB_ERROR(pinfo, "Management had invalid port number\n");
                return -1;
            }
        }
    }
    else
    {
        PTPLIB_DEBUG(pinfo, "Management was not for me\n");
        return 0;
    }

    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_MANAGEMENT;
    tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
    tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
    tx_packet.header.correctionField    = rx_packet->header->correctionField;
    tx_packet.header.sourcePortIdentity = pinfo->portDS.portIdentity;
    tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;

    /* Fill in the PTP management response */
    tx_packet.ptp.mgmt.targetPortIdentity = rx_packet->header->sourcePortIdentity;
    tx_packet.ptp.mgmt.startingBoundaryHops = management->startingBoundaryHops - management->boundaryHops;
    tx_packet.ptp.mgmt.boundaryHops = tx_packet.ptp.mgmt.startingBoundaryHops;
    tx_packet.ptp.mgmt.actionField = (management->actionField == PTP_ACTIONFIELD_COMMAND) ? PTP_ACTIONFIELD_ACKNOWLEDGE : PTP_ACTIONFIELD_RESPONSE;

    // FIXME: Process hop count, maybe forward

    const ptp_tlv_t *tlv = management->managementTLV;
    ptp_tlv_t *tlv_response = tx_packet.ptp.mgmt.managementTLV;
    tlv_response->tlvType = htons(PTP_TLVTYPE_MANAGEMENT);
    tlv_response->lengthField = 0;

    /* Per 15.3.2, Management messages can only have a single TLV in the request */
    if (ntohs(rx_packet->header->messageLength) != sizeof(ptp_header_t) +
        sizeof(ptp_message_management_t) + sizeof(ptp_tlv_t) + ntohs(tlv->lengthField))
    {
        PTPLIB_ERROR(pinfo, "Mangement length doesn't match TLV\n");
        return -1;
    }

    /* Make sure the type is correct */
    if (ntohs(tlv->tlvType) != PTP_TLVTYPE_MANAGEMENT)
    {
        PTPLIB_DEBUG(pinfo, "Unsupported management TLV type %d skipped\n", ntohs(tlv->tlvType));
        return -1;
    }

    /* Get the length and make sure it is sane */
    int length = ntohs(tlv->lengthField);
    if ((length < 0) || (length > 256))
    {
        PTPLIB_ERROR(pinfo, "TLV length (%d) is outside of the sanity check\n", length);
        return -1;
    }

    const ptp_management_tlv_t *mgmt_data = (const ptp_management_tlv_t *)tlv->valueField;
    ptp_management_tlv_t *mgmt_response = (ptp_management_tlv_t *)tlv_response->valueField;
    mgmt_response->managementId = mgmt_data->managementId;

    /* Figure out which setting is being accessed */
    ptp_managementId_t id = ntohs(mgmt_data->managementId);
    switch (id)
    {
        case PTP_MANAGEMENTID_NULL_MANAGEMENT:
            /* GET, SET, COMMAND */
            if ((management->actionField != PTP_ACTIONFIELD_GET) &&
                (management->actionField != PTP_ACTIONFIELD_SET) &&
                (management->actionField != PTP_ACTIONFIELD_COMMAND))
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            else if (ntohs(tlv->lengthField) != 2)
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_WRONG_LENGTH, "Illegal TLV lengthField");
            else
            {
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_NULL_MANAGEMENT);
            }
            break;

        case PTP_MANAGEMENTID_CLOCK_DESCRIPTION:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                char *start = (char*)mgmt_response->dataField.clock_description.data;
                char *ptr = start;
                /* clockType */
                ptr = ptplib_pack_bytes(ptr, 2, 16 | 2 | 1);
                /* physicalLayerProtocol */
                ptr = ptplib_pack_string(ptr, pinfo->packetio->physicalLayerProtocol);
                /* physicalAddressLength */
                ptr = ptplib_pack_bytes(ptr, 2, 6);
                /* physicalAddress */
                ptr = ptplib_pack_bytes(ptr, 6, pinfo->packetio->mac);
                /* protocolAddress */
                ptr = ptplib_pack_bytes(ptr, 2, pinfo->packetio->networkProtocol); /* networkProtocol */
                ptr = ptplib_pack_bytes(ptr, 2, pinfo->packetio->protocolAddressLen); /* addressLength */
                if (pinfo->packetio->protocolAddressLen > 8)
                {
                    ptr = ptplib_pack_bytes(ptr, 8, pinfo->packetio->protocolAddress[0]); /* addressField */
                    ptr = ptplib_pack_bytes(ptr, pinfo->packetio->protocolAddressLen - 8, pinfo->packetio->protocolAddress[1]); /* addressField */
                }
                else
                    ptr = ptplib_pack_bytes(ptr, pinfo->packetio->protocolAddressLen, pinfo->packetio->protocolAddress[0]); /* addressField */
                /* manufacturerIdentity */
                ptr = ptplib_pack_bytes(ptr, 3, ptplib->clock[0].id >> 40);
                /* reserved */
                ptr = ptplib_pack_bytes(ptr, 1, 0);
                /* productDescription */
                ptr = ptplib_pack_string(ptr, "Cavium Inc. 1588v2 library");
                /* revisionData */
                ptr = ptplib_pack_string(ptr, "1.0");
                /* userDescription */
                ptr = ptplib_pack_string(ptr, ptplib->userDescription);
                /* profileIdentity */
                ptr = ptplib_pack_bytes(ptr, 6, ptplib->clock[0].id);

                /* Add padding to make packet have an even length */
                if ((ptr - start) & 1)
                    ptr++;
                tlv_response->lengthField = htons(2 + (ptr - start));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_CLOCK_DESCRIPTION);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_USER_DESCRIPTION:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                char *start = (char*)mgmt_response->dataField.clock_description.data;
                char *ptr = ptplib_pack_string(start, ptplib->userDescription);
                /* Add padding to make packet have an even length */
                if ((ptr - start) & 1)
                    ptr++;
                tlv_response->lengthField = htons(2 + (ptr - start));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_USER_DESCRIPTION);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                unsigned int len = mgmt_data->dataField.user_description.lengthField;
                if (len >= sizeof(ptplib->userDescription))
                    len = sizeof(ptplib->userDescription) - 1;
                memcpy(ptplib->userDescription, mgmt_data->dataField.user_description.textField, len);
                ptplib->userDescription[len] = 0;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_USER_DESCRIPTION);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        /* State and init operations we don't need are not supported */
        case PTP_MANAGEMENTID_SAVE_IN_NON_VOLATILE_STORAGE:
        case PTP_MANAGEMENTID_RESET_NON_VOLATILE_STORAGE:
        case PTP_MANAGEMENTID_INITIALIZE:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        /* We currently don't track a fault log */
        case PTP_MANAGEMENTID_FAULT_LOG:
        case PTP_MANAGEMENTID_FAULT_LOG_RESET:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        case PTP_MANAGEMENTID_DEFAULT_DATA_SET:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.default_data_set.tsc = ptplib->defaultDS.twoStepFlag;
                mgmt_response->dataField.default_data_set.so = ptplib->defaultDS.slaveOnly;
                mgmt_response->dataField.default_data_set.numberPorts = htons(ptplib->defaultDS.numberPorts - 1); /* Don't count 0 */
                mgmt_response->dataField.default_data_set.priority1 = ptplib->defaultDS.priority1;
                mgmt_response->dataField.default_data_set.clockQuality = ptplib->defaultDS.clockQuality;
                mgmt_response->dataField.default_data_set.priority2 = ptplib->defaultDS.priority2;
                mgmt_response->dataField.default_data_set.clockIdentity = ptplib->defaultDS.clockIdentity;
                mgmt_response->dataField.default_data_set.domainNumber = ptplib->defaultDS.domainNumber;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.default_data_set));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DEFAULT_DATA_SET);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_CURRENT_DATA_SET:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                timestamp_t offsetFromMaster = timestamp_avg_get_avg(&ptplib->currentDS.offsetFromMaster);
                timestamp_t meanPathDelay = timestamp_avg_get_avg(&ptplib->currentDS.meanPathDelay);
                mgmt_response->dataField.current_data_set.stepsRemoved = htons(ptplib->currentDS.stepsRemoved);
                mgmt_response->dataField.current_data_set.offsetFromMaster = timestamp_to_correction(offsetFromMaster);
                mgmt_response->dataField.current_data_set.meanPathDelay = timestamp_to_correction(meanPathDelay);
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.current_data_set));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_CURRENT_DATA_SET);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PARENT_DATA_SET:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.parent_data_set.parentPortIdentity = ptplib->parentDS.parentPortIdentity;
                mgmt_response->dataField.parent_data_set.ps = ptplib->parentDS.parentStats;
                mgmt_response->dataField.parent_data_set.observedParentOffsetScaledLogVariance = htons(ptplib->parentDS.observedParentOffsetScaledLogVariance);
                mgmt_response->dataField.parent_data_set.observedParentClockPhaseChangeRate = htonl(ptplib->parentDS.observedParentClockPhaseChangeRate);
                mgmt_response->dataField.parent_data_set.grandmasterPriority1 = ptplib->parentDS.grandmasterPriority1;
                mgmt_response->dataField.parent_data_set.grandmasterClockQuality = ptplib->parentDS.grandmasterClockQuality;
                mgmt_response->dataField.parent_data_set.grandmasterPriority2 = ptplib->parentDS.grandmasterPriority2;;
                mgmt_response->dataField.parent_data_set.grandmasterIdentity = ptplib->parentDS.grandmasterIdentity;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.parent_data_set));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PARENT_DATA_SET);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_TIME_PROPERTIES_DATA_SET:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.time_properties_data_set.currentUtcOffset = htons(ptplib->timeDS.currentUtcOffset);
                mgmt_response->dataField.time_properties_data_set.li61 = ptplib->timeDS.leap61;
                mgmt_response->dataField.time_properties_data_set.li59 = ptplib->timeDS.leap59;
                mgmt_response->dataField.time_properties_data_set.utcv = ptplib->timeDS.currentUtcOffsetValid;
                mgmt_response->dataField.time_properties_data_set.ptp = ptplib->timeDS.ptpTimescale;
                mgmt_response->dataField.time_properties_data_set.ttra = ptplib->timeDS.timeTraceable;
                mgmt_response->dataField.time_properties_data_set.ftra = ptplib->timeDS.frequencyTraceable;
                mgmt_response->dataField.time_properties_data_set.timeSource = ptplib->timeDS.timeSource;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.time_properties_data_set));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TIME_PROPERTIES_DATA_SET);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PORT_DATA_SET:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                    {
                        mgmt_response->dataField.port_data_set[count].portIdentity = p->portDS.portIdentity;
                        mgmt_response->dataField.port_data_set[count].portState = p->portDS.portState;
                        mgmt_response->dataField.port_data_set[count].logMinDelayReqInterval = p->portDS.logMinDelayReqInterval;
                        mgmt_response->dataField.port_data_set[count].peerMeanPathDelay = timestamp_to_correction(timestamp_avg_get_avg(&p->portDS.peerMeanPathDelay));
                        mgmt_response->dataField.port_data_set[count].logAnnounceInterval = p->portDS.logAnnounceInterval;
                        mgmt_response->dataField.port_data_set[count].announceReceiptTimeout = p->portDS.announceReceiptTimeout;
                        mgmt_response->dataField.port_data_set[count].logSyncInterval = p->portDS.logSyncInterval;
                        mgmt_response->dataField.port_data_set[count].delayMechanism = p->portDS.delayMechanism;
                        mgmt_response->dataField.port_data_set[count].logMinPdelayReqInterval = p->portDS.logMinPdelayReqInterval;
                        mgmt_response->dataField.port_data_set[count].versionNumber = p->portDS.versionNumber;
                        count++;
                    }
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.port_data_set[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PORT_DATA_SET);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PRIORITY1:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.priority1.priority1 = ptplib->defaultDS.priority1;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.priority1));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PRIORITY1);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.priority1 = mgmt_data->dataField.priority1.priority1;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PRIORITY1);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PRIORITY2:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.priority2.priority2 = ptplib->defaultDS.priority2;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.priority2));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PRIORITY2);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.priority2 = mgmt_data->dataField.priority2.priority2;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PRIORITY2);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_DOMAIN:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.domain.domain = ptplib->defaultDS.domainNumber;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.domain));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DOMAIN);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.domainNumber = mgmt_data->dataField.domain.domain;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DOMAIN);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_SLAVE_ONLY:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.slave_only.slave_only = ptplib->defaultDS.slaveOnly;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.slave_only));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_SLAVE_ONLY);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.slaveOnly = mgmt_data->dataField.slave_only.slave_only;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_SLAVE_ONLY);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                    {
                        mgmt_response->dataField.log_announce_interval[count].logAnnounceInterval = p->portDS.logAnnounceInterval;
                        count++;
                    }
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.log_announce_interval[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.logAnnounceInterval = mgmt_data->dataField.log_announce_interval[0].logAnnounceInterval;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_ANNOUNCE_INTERVAL);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        mgmt_response->dataField.announce_receipt_timeout[count++].announceReceiptTimeout = p->portDS.announceReceiptTimeout;
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.announce_receipt_timeout[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.announceReceiptTimeout = mgmt_data->dataField.announce_receipt_timeout[0].announceReceiptTimeout;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_ANNOUNCE_RECEIPT_TIMEOUT);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_LOG_SYNC_INTERVAL:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        mgmt_response->dataField.log_sync_interval[count++].logSyncInterval = p->portDS.logSyncInterval;
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.log_sync_interval[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_SYNC_INTERVAL);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.logSyncInterval = mgmt_data->dataField.log_sync_interval[0].logSyncInterval;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_SYNC_INTERVAL);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_VERSION_NUMBER:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                if (target_port)
                    mgmt_response->dataField.version_number.versionNumber = target_port->portDS.versionNumber;
                else
                    mgmt_response->dataField.version_number.versionNumber = PTP_VERSION;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.version_number));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_VERSION_NUMBER);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SETABLE, "Version can't be changed");
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ENABLE_PORT:
            /* COMMAND */
            if (management->actionField == PTP_ACTIONFIELD_COMMAND)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if (((target_port == p) || (target_port == NULL)) &&
                        (p->portDS.portState == PTP_PORT_STATE_DISABLED))
                            p->portDS.portState = PTP_PORT_STATE_INITIALIZING;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DISABLE_PORT);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_DISABLE_PORT:
            /* COMMAND */
            if (management->actionField == PTP_ACTIONFIELD_COMMAND)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.portState = PTP_PORT_STATE_DISABLED;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DISABLE_PORT);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_TIME:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.time.currentTime = timestamp_to_ptp(timestamp_get_systime(1));
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.time));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TIME);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                if (ptplib->best_clock == &ptplib->clock[0])
                {
                    ptplib_set_systime(timestamp_from_ptp(mgmt_data->dataField.time.currentTime));
                    tlv_response->lengthField = htons(2);
                    mgmt_response->managementId = htons(PTP_MANAGEMENTID_TIME);
                }
                else
                    ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SETABLE, "Not allowed to set clock on slave");
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_CLOCK_ACCURACY:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.clock_accuracy.clockAccuracy = ptplib->defaultDS.clockQuality.clockAccuracy;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.clock_accuracy));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_CLOCK_ACCURACY);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.clockQuality.clockAccuracy = mgmt_data->dataField.clock_accuracy.clockAccuracy;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_CLOCK_ACCURACY);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_UTC_PROPERTIES:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.utc_properties.currentUtcOffset = htons(ptplib->timeDS.currentUtcOffset);
                mgmt_response->dataField.utc_properties.li61 = ptplib->timeDS.leap61;
                mgmt_response->dataField.utc_properties.li59 = ptplib->timeDS.leap59;
                mgmt_response->dataField.utc_properties.utcv = ptplib->timeDS.currentUtcOffsetValid;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.utc_properties));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UTC_PROPERTIES);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->timeDS.currentUtcOffset = ntohs(mgmt_data->dataField.utc_properties.currentUtcOffset);
                ptplib->timeDS.leap61 = mgmt_data->dataField.utc_properties.li61;
                ptplib->timeDS.leap59 = mgmt_data->dataField.utc_properties.li59;
                ptplib->timeDS.currentUtcOffsetValid = mgmt_data->dataField.utc_properties.utcv;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UTC_PROPERTIES);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.traceability_properties.ttra = ptplib->timeDS.timeTraceable;
                mgmt_response->dataField.traceability_properties.ftra = ptplib->timeDS.frequencyTraceable;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.traceability_properties));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->timeDS.timeTraceable = mgmt_data->dataField.traceability_properties.ttra;
                ptplib->timeDS.frequencyTraceable = mgmt_data->dataField.traceability_properties.ftra;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TRACEABILITY_PROPERTIES);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_TIMESCALE_PROPERTIES:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.timescale_properties.ptp = ptplib->timeDS.ptpTimescale;
                mgmt_response->dataField.timescale_properties.timeSource = ptplib->timeDS.timeSource;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.timescale_properties));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TIMESCALE_PROPERTIES);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->timeDS.ptpTimescale = mgmt_data->dataField.timescale_properties.ptp;
                ptplib->timeDS.timeSource = mgmt_data->dataField.timescale_properties.timeSource;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_TIMESCALE_PROPERTIES);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.unicast_negotiation_enable.en = ptplib->defaultDS.unicastEnable;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.unicast_negotiation_enable));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.unicastEnable = mgmt_data->dataField.unicast_negotiation_enable.en;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UNICAST_NEGOTIATION_ENABLE);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PATH_TRACE_LIST:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int table_size = ptplib->pathTraceSize * sizeof(mgmt_response->dataField.path_trace_list.clockIdentity[0]);
                memcpy(mgmt_response->dataField.path_trace_list.clockIdentity, ptplib->pathTrace, table_size);
                tlv_response->lengthField = htons(2 + table_size);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PATH_TRACE_LIST);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_PATH_TRACE_ENABLE:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.path_trace_enable.en = ptplib->defaultDS.pathTraceEnable;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.path_trace_enable));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PATH_TRACE_ENABLE);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                ptplib->defaultDS.pathTraceEnable = mgmt_data->dataField.path_trace_enable.en;
                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_PATH_TRACE_ENABLE);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_GRANDMASTER_CLUSTER_TABLE:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        case PTP_MANAGEMENTID_UNICAST_MASTER_TABLE:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.unicast_master_table.logQueryInterval = 0;
                mgmt_response->dataField.unicast_master_table.tableSize = htons(0);
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.unicast_master_table));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UNICAST_MASTER_TABLE);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_UNICAST_MASTER_MAX_TABLE_SIZE:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.unicast_master_max_table_size.maxTableSize = htons(0);
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.unicast_master_max_table_size));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_UNICAST_MASTER_MAX_TABLE_SIZE);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.acceptable_master_table.tableSize = htons(0);
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.acceptable_master_table));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.acceptable_master_table_enabled.en = 0;
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.acceptable_master_table_enabled));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_ACCEPTABLE_MASTER_TABLE_ENABLED);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ACCEPTABLE_MASTER_MAX_TABLE_SIZE:
            /* GET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                mgmt_response->dataField.acceptable_master_max_table_size.maxTableSize = htons(0);
                tlv_response->lengthField = htons(2 + sizeof(mgmt_response->dataField.acceptable_master_max_table_size));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_ACCEPTABLE_MASTER_MAX_TABLE_SIZE);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_ALTERNATE_MASTER:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        /* We don't support alternate time scales */
        case PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_ENABLE:
        case PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_NAME:
        case PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_MAX_KEY:
        case PTP_MANAGEMENTID_ALTERNATE_TIME_OFFSET_PROPERTIES:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        /* We don't support any of the transparent clock management messages */
        case PTP_MANAGEMENTID_TRANSPARENT_CLOCK_DEFAULT_DATA_SET:
        case PTP_MANAGEMENTID_TRANSPARENT_CLOCK_PORT_DATA_SET:
        case PTP_MANAGEMENTID_PRIMARY_DOMAIN:
            ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_NOT_SUPPORTED, "Not supported");
            break;

        case PTP_MANAGEMENTID_DELAY_MECHANISM:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        mgmt_response->dataField.delay_mechanism[count++].delayMechanism = p->portDS.delayMechanism;
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.delay_mechanism[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DELAY_MECHANISM);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.delayMechanism = mgmt_data->dataField.delay_mechanism[0].delayMechanism;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_DELAY_MECHANISM);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;

        case PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL:
            /* GET, SET */
            if (management->actionField == PTP_ACTIONFIELD_GET)
            {
                int count = 0;
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        mgmt_response->dataField.log_min_pdelay_req_interval[count++].logMinPdelayReqInterval = p->portDS.logMinPdelayReqInterval;
                }
                tlv_response->lengthField = htons(2 + count * sizeof(mgmt_response->dataField.log_min_pdelay_req_interval[0]));
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL);
            }
            else if (management->actionField == PTP_ACTIONFIELD_SET)
            {
                int port;
                for (port=1; port<ptplib->defaultDS.numberPorts; port++)
                {
                    port_info_t *p = &ptplib->port[port];
                    if ((target_port == p) || (target_port == NULL))
                        p->portDS.logMinPdelayReqInterval = mgmt_data->dataField.log_min_pdelay_req_interval[0].logMinPdelayReqInterval;
                }

                tlv_response->lengthField = htons(2);
                mgmt_response->managementId = htons(PTP_MANAGEMENTID_LOG_MIN_PDELAY_REQ_INTERVAL);
            }
            else
                ptplib_build_management_error(&tx_packet, id, PTP_MANAGEMENTERRORID_GENERAL_ERROR, "Illegal actionField");
            break;
    }

    tx_packet.header.messageLength = htons(sizeof(ptp_header_t) + sizeof(ptp_message_management_t) +
        sizeof(ptp_tlv_t) + ntohs(tlv_response->lengthField));
    if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, rx_packet->rxinfo)))
    {
        PTPLIB_ERROR(pinfo, "Unable to send management response\n");
        return -1;
    }
    return 0;
}

void setclock(void)
{
    ptplib_set_systime(timestamp_get_systime(0));
}

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
 * @return Zero on success, negative on failure.
 */
int ptplib_initialize(ptplib_state_t *ptp, ptplib_flags_t flags, int clock_source, int clock_num)
{
    static packetio_t local_port;
    ptplib_t *ptplib = (ptplib_t *)ptp;

    if (sizeof(ptplib_state_t) < sizeof(ptplib_t))
    {
        PTPLIB_ERROR(NULL, "ptplib_state_t is smaller than ptplib_t\n");
        return -1;
    }

#if USE_PTP_CLOCK
    if (sysmips(MIPS_CAVIUM_XKPHYS_WRITE, getpid(), 3, 0))
    {
        perror("sysmips(MIPS_CAVIUM_XKPHYS_WRITE) failed");
        return -1;
    }

    if(clock_source != PTPLIB_USE_DEFAULT_CLOCK)
        ptplib_setup_ptp_input(clock_source, clock_num);

    /* Configure the PPS output */
    if (PTP_GPIO_PPS_OUT != -1)
        ptplib_setup_ptp_output(1, PTP_GPIO_PPS_OUT, PTP_GPIO_PPS_HZ, 1);

    /* Configure the clock output */
    if (PTP_GPIO_CLOCK_OUT != -1)
        ptplib_setup_ptp_output(0, PTP_GPIO_CLOCK_OUT, PTP_GPIO_CLOCK_HZ, 50);
#endif

    memset(ptplib, 0, sizeof(ptplib_t));
    ptplib->flags = flags;

    ptplib->defaultDS.twoStepFlag = 1;
    ptplib->defaultDS.clockIdentity = 0; /* This is filled in once the first port is added */
    ptplib->defaultDS.numberPorts = 0;
    ptplib->defaultDS.clockQuality.clockClass = PTP_DEFAULT_CLOCK_CLASS;
    ptplib->defaultDS.clockQuality.clockAccuracy = PTP_DEFAULT_CLOCK_ACCURACY;
    ptplib->defaultDS.clockQuality.offsetScaledLogVariance = PTP_DEFAULT_OFFSET_SCALED_LOG_VARIANCE;
    ptplib->defaultDS.priority1 = PTP_DEFAULT_PRIORITY1;
    ptplib->defaultDS.priority2 = PTP_DEFAULT_PRIORITY2;
    ptplib->defaultDS.domainNumber = PTP_DEFAULT_PRIMARY_DOMAIN;
    ptplib->defaultDS.slaveOnly = 0;
    ptplib->defaultDS.unicastEnable = 1;
    ptplib->defaultDS.pathTraceEnable = 0;

    ptplib->timeDS.currentUtcOffset = PTP_DEFAULT_CURRENT_UTC_OFFSET;
    ptplib->timeDS.currentUtcOffsetValid = (PTP_DEFAULT_CURRENT_UTC_OFFSET != 0);
    ptplib->timeDS.leap59 = 0;
    ptplib->timeDS.leap61 = 0;
    ptplib->timeDS.timeTraceable = 0;
    ptplib->timeDS.frequencyTraceable = 0;
    ptplib->timeDS.ptpTimescale = 0;
    ptplib->timeDS.timeSource = PTP_DEFAULT_TIME_SOURCE;

    strcpy(ptplib->userDescription, "Octeon II running ptplib");

    ptplib->sync.last_t1_sent = TIMESTAMP_BAD;
    ptplib->sync.last_t2_received = TIMESTAMP_BAD;
    ptplib->sync.t1_sent = TIMESTAMP_BAD;
    ptplib->sync.t2_received = TIMESTAMP_BAD;

    ptplib->delay.sequenceId = 0;
    ptplib->delay.next = TIMESTAMP_ZERO;
    ptplib->delay.request_sent = TIMESTAMP_BAD;

    ptplib->best_clock = &ptplib->clock[0]; /* Local clock */

    memset(&local_port, 0, sizeof(local_port));
    strcpy(local_port.name, "Local");
    local_port.sock_event = -1;
    local_port.sock_msg = -1;
    ptplib_interface_add(ptp, &local_port);

    /* Setup PTP data about our local clock */
    clock_info_t *local = &ptplib->clock[0];
    local->announce.originTimestamp = timestamp_to_ptp(TIMESTAMP_ZERO); /* Not used */
    local->announce.currentUtcOffset = htons(ptplib->timeDS.currentUtcOffset);
    local->announce.grandmasterPriority1 = ptplib->defaultDS.priority1;
    local->announce.grandmasterClockQuality = ptplib->defaultDS.clockQuality;
    local->announce.grandmasterPriority2 = ptplib->defaultDS.priority2;
    /* Clock idenity can't be determined until the first interface is added */
    local->announce.grandmasterIdentity = 0;
    local->announce.stepsRemoved = htons(0); /* Clock is local */
    local->announce.timeSource = ptplib->timeDS.timeSource;
    local->id = local->announce.grandmasterIdentity;
    local->pinfo = &ptplib->port[0];

    setclock(); // set PTP clock to system time

    return 0;
}


/**
 * Add a packet interface for use by PTP/1588v2
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param port   Packet interface to add.
 *
 * @return Zero on success, negative on failure.
 */
int ptplib_interface_add(ptplib_state_t *ptp, packetio_t *port)
{
    ptplib_t *ptplib = (ptplib_t *)ptp;
    clock_info_t *local = &ptplib->clock[0];

    if (ptplib->defaultDS.numberPorts >= MAX_PORTS)
        return -1;

    /* Use the MAC address from the first port to identify this clock */
    if (ptplib->defaultDS.numberPorts == 1)
    {
        const uint64_t mac = port->mac;
        /* Clock idenity is the first 3 bytes of the MAC address, the byte 0xff,
            the byte 0xfe, and the last three bytes of the MAC address */
        local->id = (mac >> 24) << 40;
        local->id |= 0xfffe000000ull;
        local->id |= mac & 0xffffff;
        local->id = htonq(local->id);
        local->announce.grandmasterIdentity = local->id;
        ptplib->defaultDS.clockIdentity = local->id;
        ptplib->port[0].portDS.portIdentity.clockIdentity = local->id;
    }

    port_info_t *p = &ptplib->port[ptplib->defaultDS.numberPorts++];

    p->portDS.portIdentity.clockIdentity   = local->id;
    p->portDS.portIdentity.portNumber      = htons(ptplib->defaultDS.numberPorts);
    p->portDS.portState                    = PTP_PORT_STATE_INITIALIZING;
    p->portDS.logMinDelayReqInterval       = PTP_DEFAULT_LOG_MIN_DELAY_REQ_INTERVAL;
    p->portDS.logAnnounceInterval          = PTP_DEFAULT_LOG_ANNOUNCE_INTERVAL;
    p->portDS.announceReceiptTimeout       = PTP_DEFAULT_ANNOUNCE_RECEIPT_TIMEOUT;
    p->portDS.logSyncInterval              = PTP_DEFAULT_LOG_SYNC_INTERVAL;
    if ((ptplib->flags & PTPLIB_FLAGS_USE_PHASE_ADJUST) == 0)
        p->portDS.delayMechanism = PTP_PORT_DELAY_DISABLED;
    else if (ptplib->flags & PTPLIB_FLAGS_USE_PEER_DELAY)
        p->portDS.delayMechanism = PTP_PORT_DELAY_P2P;
    else
        p->portDS.delayMechanism = PTP_PORT_DELAY_E2E;
    p->portDS.logMinPdelayReqInterval      = PTP_DEFAULT_LOG_MIN_PDELAY_REQ_INTERVAL;
    p->portDS.versionNumber                = PTP_VERSION;

    p->delayAsymmetry = TIMESTAMP_ZERO;

    p->announce.sequenceId = 0;
    p->announce.next = TIMESTAMP_ZERO;

    p->sync.sequenceId = 0;
    p->sync.next = TIMESTAMP_ZERO;

    p->pdelay.sequenceId = 0;
    p->pdelay.next = TIMESTAMP_ZERO;
    p->pdelay.t1_request_sent = TIMESTAMP_BAD;
    p->pdelay.t2_request_received = TIMESTAMP_BAD;
    p->pdelay.t4_reply_received = TIMESTAMP_BAD;
    p->pdelay.correctionField_resp = TIMESTAMP_BAD;

    p->announceReceiptTimeout = TIMESTAMP_BAD;
    p->qualificationTimeout = TIMESTAMP_BAD;
    if (ptplib->flags & PTPLIB_FLAGS_USE_UNICAST)
        p->flags = PORT_FLAG_USE_UNICAST;
    if (ptplib->defaultDS.twoStepFlag)
        p->flags |= PORT_FLAG_USE_TWOSTEP;
    p->packetio = port;

    return 0;
}


/**
 * This function should be called for every PTP/1588v2 packet
 * received. It performs all state updates and packet sends
 * required by the PTP/1588v2 protocol.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 * @param work   WQE containing the packet. Neither the WQE or any data
 *               connected to it are modified by this function.
 *
 * @return Zero on success, negative on failure.
 */
int ptplib_process(ptplib_state_t *ptp, const packetio_packet_t *rx_packet)
{
    ptplib_t *ptplib = (ptplib_t *)ptp;
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    int min_size = sizeof(ptp_header_t);

    /* Ignore all messages when we are initializing or faulty */
    if ((pinfo->portDS.portState == PTP_PORT_STATE_INITIALIZING) ||
        (pinfo->portDS.portState == PTP_PORT_STATE_FAULTY))
        return 0;

    if (rx_packet->length < min_size)
    {
        PTPLIB_ERROR(pinfo, "Packet smaller than header\n");
        return -1;
    }

    /* Validate the PTP header */
    if (rx_packet->header->transportSpecific != 0)
    {
        PTPLIB_ERROR(pinfo, "Invalid header[transportSpecific]\n");
        return -1;
    }
    if (rx_packet->header->versionPTP != pinfo->portDS.versionNumber)
    {
        PTPLIB_ERROR(pinfo, "Invalid header[versionPTP]\n");
        return -1;
    }
    if (ntohs(rx_packet->header->messageLength) <= sizeof(ptp_header_t))
    {
        PTPLIB_ERROR(pinfo, "Invalid header[messageLength]\n");
        return -1;
    }
    if (rx_packet->header->domainNumber != ptplib->defaultDS.domainNumber)
        return 0;

    /* Silently drop packets that are from me that have been mistakely
        looped back */
    if (pinfo->portDS.portIdentity.clockIdentity ==
        rx_packet->header->sourcePortIdentity.clockIdentity)
        return 0;

    //packetio_display("Receive", rx_packet);

    switch (rx_packet->header->messageType)
    {
        case PTP_MESSAGETYPE_SYNC:
            min_size += sizeof(ptp_message_sync_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated sync message\n");
                return -1;
            }
            return ptplib_process_sync(ptplib, rx_packet);
        case PTP_MESSAGETYPE_DELAY_REQ:
            min_size += sizeof(ptp_message_delay_req_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated delay request message\n");
                return -1;
            }
            return ptplib_process_delay_req(ptplib, rx_packet);
        case PTP_MESSAGETYPE_PDELAY_REQ:
            min_size += sizeof(ptp_message_pdelay_req_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated peer delay request message\n");
                return -1;
            }
            return ptplib_process_pdelay_req(ptplib, rx_packet);
        case PTP_MESSAGETYPE_PDELAY_RESP:
            min_size += sizeof(ptp_message_pdelay_resp_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated peer delay response message\n");
                return -1;
            }
            return ptplib_process_pdelay_resp(ptplib, rx_packet);
        case PTP_MESSAGETYPE_FOLLOW_UP:
            min_size += sizeof(ptp_message_follow_up_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated follow up message\n");
                return -1;
            }
            return ptplib_process_follow_up(ptplib, rx_packet);
        case PTP_MESSAGETYPE_DELAY_RESP:
            min_size += sizeof(ptp_message_delay_resp_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated delay response message\n");
                return -1;
            }
            return ptplib_process_delay_resp(ptplib, rx_packet);
        case PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP:
            min_size += sizeof(ptp_message_pdelay_resp_follow_up_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated peer delay response follow up message\n");
                return -1;
            }
            return ptplib_process_resp_follow_up(ptplib, rx_packet);
        case PTP_MESSAGETYPE_ANNOUNCE:
            min_size += sizeof(ptp_message_announce_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated announce message\n");
                return -1;
            }
            return ptplib_process_announce(ptplib, rx_packet);
        case PTP_MESSAGETYPE_SIGNALING:
            min_size += sizeof(ptp_message_signaling_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated signaling message\n");
                return -1;
            }
            return ptplib_process_signaling(ptplib, rx_packet);
        case PTP_MESSAGETYPE_MANAGEMENT:
            min_size += sizeof(ptp_message_management_t);
            if (rx_packet->length < min_size)
            {
                PTPLIB_ERROR(pinfo, "Truncated management message\n");
                return -1;
            }
            return ptplib_process_management(ptplib, rx_packet);
    }
    PTPLIB_ERROR(pinfo, "Illegal message type\n");
    return -1;
}


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
int ptplib_process_errorqueue(ptplib_state_t *ptp, const packetio_packet_t *rx_packet)
{
    ptplib_t *ptplib = (ptplib_t *)ptp;
    port_info_t *pinfo = ptplib_get_port(ptplib, rx_packet);
    timestamp_t send_time = rx_packet->timestamp;
    ptp_packet_t tx_packet;

    /* If this port doesn't need follow ups then ignore all of these messages */
    if ((pinfo->flags & PORT_FLAG_USE_TWOSTEP) == 0)
        return 0;

    //packetio_display("Error Queue", rx_packet);

    switch (rx_packet->header->messageType)
    {
        case PTP_MESSAGETYPE_SYNC:
            memset(&tx_packet, 0, sizeof(tx_packet));
            /* Fill in the PTP header */
            tx_packet.header.transportSpecific  = 0;
            tx_packet.header.messageType        = PTP_MESSAGETYPE_FOLLOW_UP;
            tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
            tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_follow_up_t));
            tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
            tx_packet.header.sourcePortIdentity = rx_packet->header->sourcePortIdentity;
            tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
            tx_packet.header.controlField       = PTP_CONTROLFIELD_FOLLOW_UP; /* 13.3.2.10 controlField */
            tx_packet.header.logMessageInterval = rx_packet->header->logMessageInterval;
            tx_packet.header.flagField          = rx_packet->header->flagField & ~htons(PTP_FLAGFIELD_TWOSTEPFLAG);

            /* Fill in the follow up response, per section 9.5.10 */
            tx_packet.ptp.follow.preciseOriginTimestamp = timestamp_to_ptp(send_time);
            send_time.nanoseconds = 0;
            tx_packet.header.correctionField = timestamp_to_correction(send_time);

            if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, rx_packet->rxinfo)))
            {
                PTPLIB_ERROR(pinfo, "Unable to send sync follow up\n");
                return -1;
            }
            break;

        case PTP_MESSAGETYPE_DELAY_REQ:
            ptplib->delay.request_sent = send_time;
            break;

        case PTP_MESSAGETYPE_PDELAY_REQ:
            pinfo->pdelay.t1_request_sent = send_time;
            break;

        case PTP_MESSAGETYPE_PDELAY_RESP:
            memset(&tx_packet, 0, sizeof(tx_packet));
            /* Fill in the PTP header */
            tx_packet.header.transportSpecific  = 0;
            tx_packet.header.messageType        = PTP_MESSAGETYPE_PDELAY_RESP_FOLLOW_UP;
            tx_packet.header.versionPTP         = pinfo->portDS.versionNumber;
            tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_pdelay_resp_follow_up_t));
            tx_packet.header.domainNumber       = rx_packet->header->domainNumber;
            tx_packet.header.correctionField    = rx_packet->header->correctionField;
            tx_packet.header.sourcePortIdentity = rx_packet->header->sourcePortIdentity;
            tx_packet.header.sequenceId         = rx_packet->header->sequenceId;
            tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
            tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;
            tx_packet.header.flagField          = rx_packet->header->flagField & ~htons(PTP_FLAGFIELD_TWOSTEPFLAG);

            /* Fill in the follow up response */
            const ptp_message_delay_resp_t* resp = rx_packet->data;
            tx_packet.ptp.pdresp2.responseOriginTimestamp = timestamp_to_ptp(send_time);
            tx_packet.ptp.pdresp2.requestingPortIdentity = resp->requestingPortIdentity;
            timestamp_t correction = timestamp_from_correction(tx_packet.header.correctionField);
            send_time.nanoseconds = 0;
            correction = timestamp_add(correction, send_time);
            tx_packet.header.correctionField = timestamp_to_correction(correction);

            if (timestamp_is_bad(ptplib_send_packet(pinfo, &tx_packet, rx_packet->rxinfo)))
            {
                PTPLIB_ERROR(pinfo, "Unable to send peer delay response followup\n");
                return -1;
            }
            break;
    }
    return 0;
}


/**
 * Send an announce on a port
 *
 * @param ptplib     Our state info
 * @param p          The port to send the announce on
 * @param sequenceId Sequence ID to use for the announce
 * @param unicast_destination
 *                   Unicast to this address. NULL for multicast
 *
 * @return Zero on success, negative on failure
 */
static int ptplib_send_announce(ptplib_t *ptplib, port_info_t *p, int sequenceId, const char *unicast_destination)
{
    ptp_packet_t tx_packet;
    clock_info_t *master = ptplib->best_clock;

    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_ANNOUNCE;
    tx_packet.header.versionPTP         = p->portDS.versionNumber;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_announce_t));
    tx_packet.header.domainNumber       = ptplib->defaultDS.domainNumber;
    tx_packet.header.flagField          = htons((unicast_destination) ? PTP_FLAGFIELD_UNICASTFLAG : 0);
    tx_packet.header.correctionField    = 0;
    tx_packet.header.sourcePortIdentity = p->portDS.portIdentity;
    tx_packet.header.sequenceId         = htons(sequenceId);
    tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = p->portDS.logAnnounceInterval;

    /* Fill in the sync data */
    tx_packet.ptp.announce = master->announce;
    if (ptplib->best_clock == &ptplib->clock[0])
        tx_packet.ptp.announce.stepsRemoved = htons(0);
    else
        tx_packet.ptp.announce.stepsRemoved = htons(ntohs(master->announce.stepsRemoved)+1);
    tx_packet.ptp.announce.originTimestamp = timestamp_to_ptp(TIMESTAMP_ZERO);

    /* Fill in path trace data */
    if (ptplib->defaultDS.pathTraceEnable)
    {
        int messageLength = ntohs(tx_packet.header.messageLength);
        messageLength += sizeof(ptp_tlv_t);
        messageLength += (ptplib->pathTraceSize + 1) * sizeof(ptplib->pathTrace[0]);
        tx_packet.header.messageLength = htons(messageLength);
        tx_packet.ptp.announce.tlv[0].tlvType = htons(PTP_TLVTYPE_PATH_TRACE);
        tx_packet.ptp.announce.tlv[0].lengthField = htons((ptplib->pathTraceSize + 1) * sizeof(ptplib->pathTrace[0]));
        memcpy(tx_packet.ptp.announce.tlv[0].valueField, ptplib->pathTrace, ptplib->pathTraceSize * sizeof(ptplib->pathTrace[0]));
        ((uint64_t*)tx_packet.ptp.announce.tlv[0].valueField)[ptplib->pathTraceSize] = ptplib->defaultDS.clockIdentity;
    }

    /* Announce messages never need a follow up since their send time isn't
        important */
    if (timestamp_is_bad(ptplib_send_packet(p, &tx_packet, unicast_destination)))
    {
        PTPLIB_ERROR(p, "Unable to send announce\n");
        return -1;
    }
    return 0;
}


/**
 * Send a sync on a port
 *
 * @param ptplib     Our state info
 * @param p          The port to send the sync on
 * @param sequenceId Sequence ID to use for the sync
 * @param unicast_destination
 *                   Unicast to this address. NULL for multicast
 *
 * @return Zero on success, negative on failure
 */
static int ptplib_send_sync(ptplib_t *ptplib, port_info_t *p, int sequenceId, const char *unicast_destination)
{
    ptp_packet_t tx_packet;
    clock_info_t *master = ptplib->best_clock;

    /* Don't send sync message to the master's network. We use this check
        instead of the port state enumeration documented in section 6.6.2.2.
        Since all up ports are masters except for the one connected to the
        master clock, it seem less error prone to use one variable instead
        of a state per port */
    if (master->pinfo == p)
        return 0;

    memset(&tx_packet, 0, sizeof(tx_packet));

    /* Fill in the PTP header */
    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.messageType        = PTP_MESSAGETYPE_SYNC;
    tx_packet.header.versionPTP         = p->portDS.versionNumber;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_sync_t));
    tx_packet.header.domainNumber       = ptplib->defaultDS.domainNumber;
    tx_packet.header.sourcePortIdentity = p->portDS.portIdentity;
    tx_packet.header.sequenceId         = htons(sequenceId);
    tx_packet.header.controlField       = PTP_CONTROLFIELD_SYNC; /* 13.3.2.10 controlField */
    tx_packet.header.logMessageInterval = p->portDS.logSyncInterval;

    /* Fill in the sync data */
    if (p->flags & PORT_FLAG_USE_TWOSTEP)
    {
        /* The followup messaage will have all timing info */
        tx_packet.header.flagField = htons(PTP_FLAGFIELD_TWOSTEPFLAG);
        tx_packet.ptp.sync.originTimestamp = timestamp_to_ptp(TIMESTAMP_ZERO);
        tx_packet.header.correctionField    = 0;
    }
    else
    {
        /* Timestamp is stored in originTimestamp + correctionField */
        tx_packet.header.flagField = htons(0);
        timestamp_t send_time = timestamp_get_systime(1);
        tx_packet.ptp.sync.originTimestamp = timestamp_to_ptp(send_time);
        send_time.nanoseconds = 0;
        tx_packet.header.correctionField = timestamp_to_correction(send_time);
    }

    if (unicast_destination)
        tx_packet.header.flagField |= htons(PTP_FLAGFIELD_UNICASTFLAG);

    /* Sync message may need a follow up message to tell the exact send time */
    timestamp_t send_time = ptplib_send_packet(p, &tx_packet, unicast_destination);
    if (timestamp_is_bad(send_time))
    {
        PTPLIB_ERROR(p, "Unable to send sync\n");
        return -1;
    }

    if ((p->flags & PORT_FLAG_USE_TWOSTEP) == 0)
        return 0;

    if (p->packetio->flags & PACKETIO_FLAG_TX_TIMESTAMP)
        return 0;

    /* Send follow up */
    /* Fill in the PTP header */
    tx_packet.header.messageType        = PTP_MESSAGETYPE_FOLLOW_UP;
    tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_follow_up_t));
    tx_packet.header.controlField       = PTP_CONTROLFIELD_FOLLOW_UP; /* 13.3.2.10 controlField */
    tx_packet.header.flagField         &= ~htons(PTP_FLAGFIELD_TWOSTEPFLAG);

    /* Fill in the follow up response, per section 9.5.10 */
    tx_packet.ptp.follow.preciseOriginTimestamp = timestamp_to_ptp(send_time);
    send_time.nanoseconds = 0;
    tx_packet.header.correctionField = timestamp_to_correction(send_time);

    if (timestamp_is_bad(ptplib_send_packet(p, &tx_packet, NULL)))
    {
        PTPLIB_ERROR(p, "Unable to send sync follow up\n");
        return -1;
    }

    return 0;
}


/**
 * Send a (peer) delay on a port
 *
 * @param ptplib Our state info
 * @param p      The port to send the (peer) delay on
 *
 * @return Zero on success, negative on failure
 */
static int ptplib_send_delay(ptplib_t *ptplib, port_info_t *p)
{
    ptp_packet_t tx_packet;
    timestamp_t send_time;
    const char *unicast_destination;
    clock_info_t *master = ptplib->best_clock;

    memset(&tx_packet, 0, sizeof(tx_packet));

    tx_packet.header.transportSpecific  = 0;
    tx_packet.header.versionPTP         = p->portDS.versionNumber;
    tx_packet.header.domainNumber       = ptplib->defaultDS.domainNumber;
    tx_packet.header.flagField          = htons(0);
    tx_packet.header.correctionField    = 0;
    tx_packet.header.sourcePortIdentity = p->portDS.portIdentity;
    tx_packet.header.logMessageInterval = PTP_DEFAULT_LOG_MESSAGE_INTERVAL_INVALID;

    if (p->portDS.delayMechanism == PTP_PORT_DELAY_P2P)
    {
        tx_packet.header.sequenceId         = htons(++p->pdelay.sequenceId);
        p->pdelay.next = ptplib_add_interval(ptplib, p->pdelay.next, p->portDS.logMinPdelayReqInterval);
        tx_packet.header.messageType        = PTP_MESSAGETYPE_PDELAY_REQ;
        tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_pdelay_req_t));
        tx_packet.header.controlField       = PTP_CONTROLFIELD_OTHERS; /* 13.3.2.10 controlField */
        tx_packet.ptp.pdreq.originTimestamp = timestamp_to_ptp(TIMESTAMP_ZERO);
        unicast_destination = NULL;
    }
    else
    {
        tx_packet.header.sequenceId         = htons(++ptplib->delay.sequenceId);
        ptplib->delay.next = ptplib_add_interval(ptplib, ptplib->delay.next, p->portDS.logMinDelayReqInterval);
        tx_packet.header.messageType        = PTP_MESSAGETYPE_DELAY_REQ;
        tx_packet.header.messageLength      = htons(sizeof(ptp_header_t) + sizeof(ptp_message_delay_req_t));
        tx_packet.header.controlField       = PTP_CONTROLFIELD_DELAY_REQ; /* 13.3.2.10 controlField */
        tx_packet.ptp.dreq.originTimestamp  = timestamp_to_ptp(TIMESTAMP_ZERO);
        if (p->flags & PORT_FLAG_USE_UNICAST)
        {
            unicast_destination = master->last_sync_rxinfo;
            tx_packet.header.flagField |= htons(PTP_FLAGFIELD_UNICASTFLAG);
        }
        else
            unicast_destination = NULL;

        /* We can't do delay messages until we've received a sync */
        if (timestamp_is_bad(ptplib->sync.t1_sent) || timestamp_is_bad(ptplib->sync.t2_received))
            return 0;
    }

    /* Delay request don't need a follow up */
    send_time = ptplib_send_packet(p, &tx_packet, unicast_destination);
    if (timestamp_is_bad(send_time))
        PTPLIB_ERROR(p, "Unable to send %s request\n",
            (p->portDS.delayMechanism == PTP_PORT_DELAY_P2P) ? "peer delay" : "delay");

    /* Only record the timestamp if we won't get a better hardware one */
    if (!(p->packetio->flags & PACKETIO_FLAG_TX_TIMESTAMP))
    {
        if (p->portDS.delayMechanism == PTP_PORT_DELAY_P2P)
            p->pdelay.t1_request_sent = send_time;
        else
            ptplib->delay.request_sent = send_time;
    }

    return 0;
}


/**
 * Check the state of all unicast nodes on a port
 *
 * @param ptplib Our state info
 * @param pinfo  Port to check
 */
static void ptplib_check_unicast(ptplib_t *ptplib, port_info_t *p)
{
    timestamp_t now = timestamp_get_rawtime();
    unicast_node_t *node;

    /* Loop through all unicast nodes */
    node = p->unicast_head;
    while (node)
    {
        /* Mark the annouce lease expired if needed */
        if (!timestamp_is_bad(node->announce.lease_expire) &&
            timestamp_before(node->announce.lease_expire, now))
        {
            node->announce.lease_expire = TIMESTAMP_BAD;
            PTPLIB_INFO(p, "Unicast node %016llx:%d annouce lease has expired\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
        }
        /* Mark the sync lease expired if needed */
        if (!timestamp_is_bad(node->sync.lease_expire) &&
            timestamp_before(node->sync.lease_expire, now))
        {
            node->sync.lease_expire = TIMESTAMP_BAD;
            PTPLIB_INFO(p, "Unicast node %016llx:%d sync lease has expired\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
        }
        /* Mark the delay lease expired if needed */
        if (!timestamp_is_bad(node->delay.lease_expire) &&
            timestamp_before(node->delay.lease_expire, now))
        {
            node->delay.lease_expire = TIMESTAMP_BAD;
            PTPLIB_INFO(p, "Unicast node %016llx:%d delay lease has expired\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
        }

        /* Delete nodes where all leases are expired */
        if (!ptplib->defaultDS.unicastEnable ||
            (timestamp_is_bad(node->announce.lease_expire) &&
             timestamp_is_bad(node->sync.lease_expire) &&
             timestamp_is_bad(node->delay.lease_expire)))
        {
            unicast_node_t *next = node->next;
            if (node->prev)
                node->prev->next = next;
            else
                p->unicast_head = next;
            if (node->next)
                node->next->prev = node->prev;
            else
                p->unicast_tail = node->prev;
            PTPLIB_INFO(p, "Expiring unicast node %016llx:%d\n", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
            free(node);
            node = next;
            continue;
        }

        if (!timestamp_is_bad(node->announce.lease_expire) &&
            timestamp_before(node->announce.next, now))
        {
            if (ptplib_send_announce(ptplib, p, ++node->announce.sequenceId, node->txinfo) == 0)
                node->announce.next = ptplib_add_interval(ptplib, node->announce.next, node->announce.interval);
        }

        if (!timestamp_is_bad(node->sync.lease_expire) &&
            timestamp_before(node->sync.next, now))
        {
            if (ptplib_send_sync(ptplib, p, ++node->sync.sequenceId, node->txinfo) == 0)
                node->sync.next = ptplib_add_interval(ptplib, node->sync.next, node->sync.interval);
        }

        node = node->next;
    }
}


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
int ptplib_periodic(ptplib_state_t *ptp)
{
    ptplib_t *ptplib = (ptplib_t *)ptp;
    timestamp_t now = timestamp_get_rawtime();
    int port;

    /* Figure out which clock is currently best */
    ptplib_choose_master(ptplib);

    /* Port zero is local, so skip it */
    for (port=1; port<ptplib->defaultDS.numberPorts; port++)
    {
        port_info_t *p = &ptplib->port[port];

        /* Peer to peer delay needs to be done on each port when it is in use */
        if ((p->portDS.delayMechanism == PTP_PORT_DELAY_P2P) &&
            timestamp_before(p->pdelay.next, now))
        {
            ptplib_send_delay(ptplib, p);
            p->pdelay.next = ptplib_add_interval(ptplib, p->pdelay.next, p->portDS.logMinPdelayReqInterval);
        }

        if ((ptplib->flags & PTPLIB_FLAGS_USE_MULTICAST) == 0)
        {
            // FIXME: What should this be?
            if (timestamp_before(ptplib->signal.unicast_renew, now) && ptplib->defaultDS.unicastEnable)
            {
                ptplib_send_unicast_request(ptplib, p, 1, 0, 0);
                ptplib->signal.unicast_renew = ptplib_add_interval(ptplib, now, 0);
            }
        }

        switch (p->portDS.portState)
        {
            case PTP_PORT_STATE_INITIALIZING:
                /* Init is complete, start listening */
                p->portDS.portState = PTP_PORT_STATE_LISTENING;
                p->announceReceiptTimeout = ptplib_add_interval(ptplib, now, p->portDS.announceReceiptTimeout);
                break;

            case PTP_PORT_STATE_FAULTY:
                /* We're faulty, do nothing */
                break;

            case PTP_PORT_STATE_DISABLED:
                /* We're disabled, do nothing. Only a management message can get
                    us going */
                break;

            case PTP_PORT_STATE_LISTENING:
                /* Wait for the announce receipt timeout to find all clocks
                    that are broadcasting */
                if (timestamp_before(p->announceReceiptTimeout, now))
                {
                    if (ptplib->best_clock == &ptplib->clock[0])
                    {
                        if (!ptplib->defaultDS.slaveOnly)
                        {
                            p->qualificationTimeout = ptplib_add_interval(ptplib, now, p->portDS.logAnnounceInterval);
                            p->portDS.portState = PTP_PORT_STATE_PRE_MASTER;
                        }
                    }
                    else
                        p->portDS.portState = PTP_PORT_STATE_UNCALIBRATED;
                }
                break;

            case PTP_PORT_STATE_PRE_MASTER:
                /* Announce and Sync are not allowed */
                if (ptplib->best_clock->pinfo == p)
                    p->portDS.portState = PTP_PORT_STATE_UNCALIBRATED;
                else if (timestamp_before(p->qualificationTimeout, now))
                    p->portDS.portState = PTP_PORT_STATE_MASTER;
                break;

            case PTP_PORT_STATE_MASTER:
                /* If the master clock is on this port then this port shouldn't
                    be in the master state. Change it to uncalibrated */
                if (ptplib->best_clock->pinfo == p)
                {
                    p->portDS.portState = PTP_PORT_STATE_UNCALIBRATED;
                    break;
                }
                else if (ptplib->defaultDS.slaveOnly)
                {
                    p->portDS.portState = PTP_PORT_STATE_LISTENING;
                    break;
                }

                /* Send the multicast messages if needed */
                if (ptplib->flags & PTPLIB_FLAGS_USE_MULTICAST)
                {
                    /* Check if we need to send an announce on this port */
                    if (timestamp_before(p->announce.next, now))
                    {
                        if (ptplib_send_announce(ptplib, p, ++p->announce.sequenceId, NULL) == 0)
                            p->announce.next = ptplib_add_interval(ptplib, p->announce.next, p->portDS.logAnnounceInterval);
                    }

                    /* Check if we need to send a sync on this port */
                    if (timestamp_before(p->sync.next, now))
                    {
                        if (ptplib_send_sync(ptplib, p, ++p->sync.sequenceId, NULL) == 0)
                            p->sync.next = ptplib_add_interval(ptplib, p->sync.next, p->portDS.logSyncInterval);
                    }
                }

                /* Check all the unicast nodes */
                ptplib_check_unicast(ptplib, p);
                break;

            case PTP_PORT_STATE_PASSIVE:
                /* Nothing to do */
                break;

            case PTP_PORT_STATE_UNCALIBRATED:
            case PTP_PORT_STATE_SLAVE:
                /* Ports that aren't connected to the master clock should
                    become masters */
                if (ptplib->best_clock->pinfo != p)
                {
                    if (ptplib->defaultDS.slaveOnly)
                        p->portDS.portState = PTP_PORT_STATE_LISTENING;
                    else
                        p->portDS.portState = PTP_PORT_STATE_PRE_MASTER;
                    break;
                }

                /* Only change to slave when offsetFromMaster is less than a
                    threshold */
                timestamp_t offsetFromMaster = timestamp_avg_get_avg(&ptplib->currentDS.offsetFromMaster);
                if (timestamp_in_range(offsetFromMaster, -MAX_OFFSET_FOR_CALIB, MAX_OFFSET_FOR_CALIB))
                    p->portDS.portState = PTP_PORT_STATE_SLAVE;
                else
                    p->portDS.portState = PTP_PORT_STATE_UNCALIBRATED;

                /* Send the delay request if needed */
                if ((p == ptplib->best_clock->pinfo) &&
                    (p->portDS.delayMechanism == PTP_PORT_DELAY_E2E) &&
                    timestamp_before(ptplib->delay.next, now))
                {
                    ptplib_send_delay(ptplib, p);
                    ptplib->delay.next = ptplib_add_interval(ptplib, ptplib->delay.next, p->portDS.logMinDelayReqInterval);
                }

                /* Check all the unicast nodes */
                ptplib_check_unicast(ptplib, p);
                break;
        }
    }
    return 0;
}


/**
 * Display current clock into to the user
 *
 * @param ptp    Our state info
 * @param clear_screen
 *               Clear the terminal window before displaying
 *
 * @return Zero on success, negative on failure.
 */
int ptplib_display(ptplib_state_t *ptp, int clear_screen)
{
    #define GOTO_TOP  "\033[1;1H"   /* ESC[1;1H begins output at the top of the terminal (line 1) */
    #define GOTO_BOTTOM "\033[100;1H"   /* ESC[1;1H begins output at the bottom of the terminal (actually line 100) */
    #define ERASE_EOL "\033[0K"     /* Erase to end of line */
    #define REVERSE   "\033[7m"     /* Reverse the display */
    #define NORMAL    "\033[0m"     /* Normal display */
    const char *EOL = ERASE_EOL "\n";
    ptplib_t *ptplib = (ptplib_t *)ptp;
    int index;
    char localtime[TIMESTAMP_STRING_LEN];
    timestamp_t now = timestamp_get_rawtime();

    printf(GOTO_TOP);
    printf(REVERSE);
    printf("---- 1588v2 Daemon ----%s", EOL);
    printf(NORMAL);

    printf("Domain Number:          %u%s", ptplib->defaultDS.domainNumber, EOL);
    printf("Clock synchronization:  %s%s%s",
        (ptplib->flags & PTPLIB_FLAGS_USE_FREQUENCY_ADJUST) ? "Frequency" : "None",
        (ptplib->flags & PTPLIB_FLAGS_USE_PHASE_ADJUST) ? " and Phase" : "",
        EOL);
    printf("Local time:             %s%s", timestamp_to_string(timestamp_get_systime(1), localtime), EOL);

    if (ptplib->best_clock != &ptplib->clock[0])
    {
        char ts[TIMESTAMP_AVG_STRING_LEN];
        printf("Frequency drift:        %s%s", timestamp_avg_to_string(&ptplib->driftFromMaster, ts), EOL);
        printf("Phase offset:           %s%s", timestamp_avg_to_string(&ptplib->currentDS.offsetFromMaster, ts), EOL);
        if (ptplib->best_clock->pinfo->portDS.delayMechanism == PTP_PORT_DELAY_E2E)
            printf("Mean path delay:        %s%s", timestamp_avg_to_string(&ptplib->currentDS.meanPathDelay, ts), EOL);
    }

#if USE_PTP_CLOCK
    {
        volatile uint64_t *PTP_TIMESTAMP = (volatile uint64_t *)PTPLIB_MIO_PTP_TIMESTAMP;
        char ts[TIMESTAMP_STRING_LEN];
        timestamp_t t;
        t.nanoseconds = *PTP_TIMESTAMP;
        t.nanoseconds = *PTP_TIMESTAMP;
        *PTP_TIMESTAMP = t.nanoseconds;
        t.fractions = 0;
        if (t.nanoseconds)
            printf("Event Timestamp:        %s%s", timestamp_to_string(t, ts), EOL);
    }
#endif

#if USE_PTP_CLOCK == 0
    struct timex tinfo;
    memset(&tinfo, 0, sizeof(tinfo));
    if (adjtimex(&tinfo) >= 0)
    {
        printf("%s", EOL);
        printf("---- Adjtimex ----%s", EOL);
        printf("offset:       %+ld ns%s", tinfo.offset, EOL);
        printf("freq:         %+lld ppb%s", ((long long)tinfo.freq * 1000)>>16, EOL);
        printf("maxerror:      %ld us%s", tinfo.maxerror, EOL);
        printf("esterror:      %ld us%s", tinfo.esterror, EOL);
    }
#endif

    printf("%s", EOL);
    printf("---- Known Clocks ----%s", EOL);

    for (index=0; index<MAX_CLOCKS; index++)
    {
        clock_info_t *clk = &ptplib->clock[index];
        if (clk->id == 0)
            continue;
        printf("ID:%016llx ", (ULL)ntohq(clk->id));
        if ((index == 0) && (ptplib->best_clock != &ptplib->clock[0]))
            printf("Master:%016llx ", (ULL)ntohq(ptplib->best_clock->announce.grandmasterIdentity));
        else
            printf("Master:%016llx ", (ULL)ntohq(clk->announce.grandmasterIdentity));
        printf("Announce interval:%4dms ", (int)(timestamp_avg_get_avg(&clk->announce_interval).nanoseconds / (NSEC / 1000)));
        printf("Steps:%u ", ntohs(clk->announce.stepsRemoved));
        if (clk->pinfo == NULL)
            printf("Port:Local%s%s",
                (clk == ptplib->best_clock) ? " (Master)" : "", EOL);
        else
            printf("Port:%s%s%s", clk->pinfo->packetio->name,
                (clk == ptplib->best_clock) ? " (Master)" : "", EOL);
    }

    if (ptplib->pathTraceSize)
    {
        printf("%s", EOL);
        printf("Path Trace:");
        for (index=0; index<ptplib->pathTraceSize; index++)
            printf(" %016llx", (ULL)ptplib->pathTrace[index]);
        printf("%s", EOL);
    }

    printf("%s", EOL);
    printf("---- Ports ----%s", EOL);

    for (index=1; index<ptplib->defaultDS.numberPorts; index++)
    {
        port_info_t *p = &ptplib->port[index];
        const char *state_str = "UNKNOWN";

        switch (p->portDS.portState)
        {
            case PTP_PORT_STATE_INITIALIZING:
                state_str = "INITIALIZING";
                break;
            case PTP_PORT_STATE_FAULTY:
                state_str = "FAULTY";
                break;
            case PTP_PORT_STATE_DISABLED:
                state_str = "DISABLED";
                break;
            case PTP_PORT_STATE_LISTENING:
                state_str = "LISTENING";
                break;
            case PTP_PORT_STATE_PRE_MASTER:
                state_str = "PRE_MASTER";
                break;
            case PTP_PORT_STATE_MASTER:
                state_str = "MASTER";
                break;
            case PTP_PORT_STATE_PASSIVE:
                state_str = "PASSIVE";
                break;
            case PTP_PORT_STATE_UNCALIBRATED:
                state_str = "UNCALIBRATED";
                break;
            case PTP_PORT_STATE_SLAVE:
                state_str = "SLAVE";
                break;
        }
        printf("%-5s State:%-12s", p->packetio->name, state_str);
        if (p->portDS.logAnnounceInterval < 0)
            printf(" Announce interval:%d%s", 1<<-p->portDS.logAnnounceInterval, "/s");
        else
            printf(" Announce interval:%d%s", 1<<p->portDS.logAnnounceInterval, "s");
        char delayAsymmetry[TIMESTAMP_STRING_LEN];
        printf(" Asymmetry:%s Mode:%s step%s",
            timestamp_to_string(p->delayAsymmetry, delayAsymmetry),
            (ptplib->defaultDS.twoStepFlag) ? "Two" : "One", EOL);
        if (p->portDS.delayMechanism == PTP_PORT_DELAY_P2P)
        {
            char buffer[TIMESTAMP_AVG_STRING_LEN];
            printf("    Peer mean path delay:%s", timestamp_avg_to_string(&p->portDS.peerMeanPathDelay, buffer));
            if (p->portDS.logMinPdelayReqInterval < 0)
                printf(" Peer delay interval:%d%s%s", 1<<-p->portDS.logMinPdelayReqInterval, "/s", EOL);
            else
                printf(" Peer delay interval:%d%s%s", 1<<p->portDS.logMinPdelayReqInterval, "s", EOL);
        }

        /* Print out the unicast nodes for this port */
        unicast_node_t *node;
        node = p->unicast_head;
        while (node)
        {
            timestamp_t tmp;
            printf("    Unicast: %016llx:%d", (ULL)node->destPortIdentity.clockIdentity, node->destPortIdentity.portNumber);
            if (!timestamp_is_bad(node->announce.lease_expire))
            {
                tmp = timestamp_sub(node->announce.lease_expire, now);
                printf(" Announce: %s", timestamp_to_string(tmp, localtime));
                if (node->announce.interval < 0)
                    printf(", interval:%d%s", 1<<-node->announce.interval, "/s");
                else
                    printf(", interval:%d%s", 1<<node->announce.interval, "s");
            }
            if (!timestamp_is_bad(node->sync.lease_expire))
            {
                tmp = timestamp_sub(node->sync.lease_expire, now);
                printf(" Sync: %s", timestamp_to_string(tmp, localtime));
                if (node->sync.interval < 0)
                    printf(", interval:%d%s", 1<<-node->sync.interval, "/s");
                else
                    printf(", interval:%d%s", 1<<node->sync.interval, "s");
            }
            if (!timestamp_is_bad(node->delay.lease_expire))
            {
                tmp = timestamp_sub(node->delay.lease_expire, now);
                printf(" Delay: %s", timestamp_to_string(tmp, localtime));
            }
            printf("%s", EOL);
            node = node->next;
        }
    }
    printf("-----------------------%s%s", EOL, EOL);
    printf(GOTO_BOTTOM);
    fflush(stdout);
    return 0;
}


/**
 * Shutdown a PTP/1588v2 library instance.
 *
 * @param ptp    State for ptplib representing a single clock processing
 *               instance.
 *
 * @return Zero on success, negative on failure.
 */
int ptplib_shutdown(ptplib_state_t *ptp)
{
    ptplib_t *ptplib = (ptplib_t *)ptp;
    memset(ptplib, 0, sizeof(ptplib_t));
    return 0;
}

