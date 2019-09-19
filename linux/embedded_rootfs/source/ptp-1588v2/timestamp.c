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
 * Utility functions for dealing with accurate timestamps.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/timex.h>
#include "ptp-packets.h"
#include "timestamp.h"
#include "ptp-csrs.h"

#define MASK32      0xffffffffll    /* Mask with 32bits */

/* Absolute value without truncating to an int */
#define ABS(a) (((a) < 0) ? -(a) : (a))

#define ERROR(format, ...) printf("Timestamp:" format, ##__VA_ARGS__)

#ifndef USE_PTP_CLOCK
#error USE_PTP_CLOCK must be defined as either 0 or 1
#endif

const timestamp_t TIMESTAMP_ZERO = {0,0};
const timestamp_t TIMESTAMP_BAD = {INT64_MIN, UINT32_MAX};
const timestamp_t TIMESTAMP_MAX = {INT64_MAX, UINT32_MAX};

/**
 * Return true if the supplied timestamp is BAD_TIMESTAMP.
 *
 * @param t
 *
 * @return
 */
int timestamp_is_bad(const timestamp_t t)
{
    return (t.nanoseconds == TIMESTAMP_BAD.nanoseconds) &&
        (t.fractions == TIMESTAMP_BAD.fractions);
}


/**
 * Result = a + b
 *
 * @param a
 * @param b
 *
 * @return
 */
timestamp_t timestamp_add(const timestamp_t a, const timestamp_t b)
{
    timestamp_t r;

    if (timestamp_is_bad(a))
        ERROR("timestamp_add with bad timestamp a\n");
    if (timestamp_is_bad(b))
        ERROR("timestamp_add with bad timestamp b\n");

    /* We treat nanoseconds|fractions as a 96bit twos compliment number */
    uint64_t f = (uint64_t)a.fractions + b.fractions;
    r.nanoseconds = a.nanoseconds + b.nanoseconds;
    r.nanoseconds += f >> 32;
    r.fractions = f & MASK32;
    return r;
}


/**
 * Result = a - b
 *
 * @param a
 * @param b
 *
 * @return
 */
timestamp_t timestamp_sub(const timestamp_t a, timestamp_t b)
{
    if (timestamp_is_bad(a))
        ERROR("timestamp_sub with bad timestamp a\n");
    if (timestamp_is_bad(b))
        ERROR("timestamp_sub with bad timestamp b\n");

    /* Calc -b, which for twos compliment is (~b)+1. We
        treat nanoseconds|fractions as a 96bit twos compliment number */
    b.nanoseconds = ~b.nanoseconds;
    b.fractions = ~b.fractions;
    b.fractions++;
    if (b.fractions == 0)
        b.nanoseconds++;
    return timestamp_add(a, b);
}


/**
 * Result = a / 2
 *
 * @param a
 *
 * @return
 */
timestamp_t timestamp_half(const timestamp_t a)
{
    timestamp_t r;

    if (timestamp_is_bad(a))
        ERROR("timestamp_half with bad timestamp a\n");

    r.fractions = a.fractions >> 1;
    r.fractions |= (a.nanoseconds&1)<<31;
    r.nanoseconds = a.nanoseconds>>1;
    return r;
}


/**
 * Shift a timestamp by a number of bits.
 *
 * @param a      Timestamp to shift
 * @param shift  Shift amount. Positive goes left, negative goes right
 *
 * @return Shifted timestamp
 */
static timestamp_t timestamp_shift(const timestamp_t a, int shift)
{
    timestamp_t r;
    if (shift > 0)
    {
        uint64_t f;
        r.nanoseconds = a.nanoseconds << shift;
        f = (uint64_t)a.fractions << shift;
        r.fractions = f & MASK32;
        r.nanoseconds += f >> 32;
    }
    else if (shift < 0)
    {
        shift = -shift;
        r.nanoseconds = a.nanoseconds >> shift;
        if (shift > 32)
            r.fractions = a.nanoseconds >> (shift-32);
        else if (shift == 32)
            r.fractions = a.nanoseconds;
        else
        {
            r.fractions = a.fractions >> shift;
            r.fractions |= a.nanoseconds << (32-shift);
        }
    }
    else
        r = a;
    return r;
}


/**
 * Return if timestamp "a" is before timestamp "b".
 *
 * @param a
 * @param b
 *
 * @return Non zero if A is before B
 */
int timestamp_before(const timestamp_t a, const timestamp_t b)
{
    if (timestamp_is_bad(a))
        ERROR("timestamp_before with bad timestamp a\n");
    if (timestamp_is_bad(b))
        ERROR("timestamp_before with bad timestamp b\n");

    if (a.nanoseconds < b.nanoseconds)
        return 1;
    else if (a.nanoseconds == b.nanoseconds)
        return a.fractions < b.fractions;
    else
        return 0;
}


/**
 * Return if timestamp is in the range of min_nanosec to max_nanosec.
 *
 * @param a
 * @param nanosec
 *
 * @return
 */
int timestamp_in_range(const timestamp_t a, int64_t min_nanosec, int64_t max_nanosec)
{
    return ((a.nanoseconds < max_nanosec) && (a.nanoseconds > min_nanosec));
}


/**
 * Convert the internal timestamp format into a PTP timestamp
 * suitable for use inside PTP packets. Any endian swapping necessary is
 * automatically performed.
 *
 * @param internal Internal timestamp to convert
 *
 * @return PTP formatted timestamp
 */
ptp_timestamp_t timestamp_to_ptp(const timestamp_t internal)
{
    ptp_timestamp_t r;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    r.secondsField = htonq(internal.nanoseconds / NSEC) >> 16;
#else
    r.secondsField = internal.nanoseconds / NSEC;
#endif
    r.nanosecondsField = htonl(internal.nanoseconds % NSEC);
    /* Fractional nanoseconds was lost in the conversion */
    return r;
}


/**
 * Convert a PTP packet timestamp into our internal format. Any endian swapping
 * necessary is automatically performed.
 *
 * @param ptp    Timestamp to convert
 *
 * @return Internal formatted timestamp
 */
timestamp_t timestamp_from_ptp(const ptp_timestamp_t ptp)
{
    timestamp_t r;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    r.nanoseconds = (ntohq(ptp.secondsField) >> 16) * NSEC;
#else
    r.nanoseconds = ptp.secondsField * NSEC;
#endif
    r.nanoseconds += ntohl(ptp.nanosecondsField);
    /* Fractional nanoseconds not present on PTP timestamp */
    r.fractions = 0;
    return r;
}


/**
 * Convert a PTP correctionField into an internal timestamp delta.
 * Corrections in PTP are in 1/(2**16) of a nanosecond. Any endian
 * swapping necessary is automatically performed.
 *
 * @param correction PTP correctionField value
 *
 * @return Internal timestamp delta
 */
timestamp_t timestamp_from_correction(const uint64_t correction)
{
    uint64_t hcorrection = ntohq(correction);
    timestamp_t r;
    r.nanoseconds = (int64_t)hcorrection >> 16;
    r.fractions = (hcorrection<<16) & MASK32;
    return r;
}


/**
 * Convert an internal timestamp delta into a PTP correctionField.
 * Corrections in PTP are in 1/(2**16) of a nanosecond. Overflows
 * are saturating. Any endian swapping necessary is automatically
 * performed.
 *
 * @param delta  Internal time delta
 *
 * @return PTP correction
 */
uint64_t timestamp_to_correction(const timestamp_t delta)
{
    uint64_t correction;

    /* Check for overflow */
    if (delta.nanoseconds > (INT64_MAX>>16))
        correction = INT64_MAX;
    /* Check for underflow */
    else if (delta.nanoseconds < (INT64_MIN>>16))
        correction = INT64_MIN;
    /* Valid correction */
    else
        correction = (delta.nanoseconds<<16) | (delta.fractions>>16);
    correction = htonq(correction);
    return correction;
}


/**
 * Convert a timestamp into a TIMESTAMP_STRING_LEN length string
 *
 * @param a      Timestamp to convert
 * @param buffer String to hold the result
 *
 * @return Same as buffer
 */
const char *timestamp_to_string(const timestamp_t a, char *buffer)
{
    uint64_t ns = ABS(a.nanoseconds);
    if ((ns < NSEC * 60 * 60) || (a.nanoseconds < 0))
    {
        uint64_t sec = ns / NSEC;
        uint64_t nano = ns % NSEC;
        if (a.nanoseconds<0)
            sprintf(buffer, "-%llu.%09llu", (unsigned long long)sec, (unsigned long long)nano);
        else
            sprintf(buffer, "+%llu.%09llu", (unsigned long long)sec, (unsigned long long)nano);
    }
    else
    {
        struct tm lt;
        time_t utc_time;
        utc_time = ns / NSEC;
        localtime_r(&utc_time, &lt);
        sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d.%09u",
            lt.tm_year + 1900, lt.tm_mon+1, lt.tm_mday,
            lt.tm_hour, lt.tm_min, lt.tm_sec, (int)(ns % NSEC));
    }
    return buffer;
}


/**
 * Get the current raw time as a timestamp_t. Raw time is not affected by
 * set_systime nor adjust_systime. It is not considered an accurate clock and is
 * only used for rough intervals.
 *
 * @return The current raw time
 */
timestamp_t timestamp_get_rawtime(void)
{
    timestamp_t r;
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts))
    {
        ERROR("timestamp_get_rawtime: clock_gettime(CLOCK_MONOTONIC) failed\n");
        r.nanoseconds = 0;
        r.fractions = 0;
    }
    else
    {
        r.nanoseconds = (uint64_t)ts.tv_sec * NSEC;
        r.nanoseconds += ts.tv_nsec;
        r.fractions = 0;
    }
    return r;
}

/**
 * Get the current system time as a timestamp_t
 *
 * @param:
 *  1 - use HW PTP clock is possible
 *  0 - use linux clock
 * 
 * @return The current system time
 */
timestamp_t timestamp_get_systime(int which_clock)
{
#if USE_PTP_CLOCK
    int hw_enabled = 1;
#else
    int hw_enabled = 0;
#endif

    if(hw_enabled && which_clock)
    {
        timestamp_t r;
        volatile uint64_t *ptp_clock_hi = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_HI;
        r.nanoseconds = *ptp_clock_hi;
        r.nanoseconds = *ptp_clock_hi;
        r.fractions = 0;
        return r;
    }
    else
    {
        timestamp_t r;
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts))
        {
            ERROR("timestamp_get_rawtime: clock_gettime(CLOCK_REALTIME) failed\n");
            r.nanoseconds = 0;
            r.fractions = 0;
        }
        else
        {
            r.nanoseconds = (uint64_t)ts.tv_sec * NSEC;
            r.nanoseconds += ts.tv_nsec;
            r.fractions = 0;
        }
        return r;
    }
}

/**
 * Set the current system time
 *
 * @param new_time New time to set
 */
void timestamp_set_systime(timestamp_t new_time)
{
#if USE_PTP_CLOCK
    volatile uint64_t *ptp_clock_hi = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_HI;
    *ptp_clock_hi = new_time.nanoseconds;
#else
    struct timespec ts;

    memset(&ts, 0, sizeof(ts));
    ts.tv_sec = new_time.nanoseconds / NSEC;
    ts.tv_nsec = new_time.nanoseconds % NSEC;

    if (clock_settime(CLOCK_REALTIME, &ts))
        ERROR("timestamp_get_rawtime: clock_settime(CLOCK_REALTIME) failed\n");
#endif
}


/**
 * Adjust the system time by changing the frequency of the clock.
 *
 * @param ppb    Amount clock should be adjusted in parts per billion. Positive
 *               means the clock is running fast and should slow down.
 * @param offset Current offset from real time. Positive is this clock is fast,
 *               negative if it is slow. Used to skew rate to reduce this value
 *               over time.
 */
void timestamp_adjust_systime(int64_t ppb, timestamp_t offset)
{
#if USE_PTP_CLOCK
    const uint64_t SLEW_RATE = 5;   /* Number of seconds to slew clock over to sync clocks */
    const int SCALE_SHIFT = 31;     /* 31 gives a max scale of 2.0 */
    static uint64_t clock_comp = 0;
    volatile uint64_t *ptp_clock_comp = (volatile uint64_t *)PTPLIB_MIO_PTP_CLOCK_COMP;
    uint64_t scale;
    uint64_t diff;

    /* Only read the current PTP_CLOCK_COMP once and store it */
    if (clock_comp == 0)
    {
        clock_comp = *ptp_clock_comp;
        clock_comp = *ptp_clock_comp;
    }

    if (offset.nanoseconds < 0)
    {
        offset = timestamp_sub(TIMESTAMP_ZERO, offset);
        if (offset.nanoseconds > UINT32_MAX)
            diff = UINT64_MAX;
        else
            diff = ((uint64_t)offset.nanoseconds<<32) + offset.fractions;
        /* scaleFactor = 1.0 + diff/SLEW_RATE */
        scale = (1ull<<SCALE_SHIFT) + diff / (SLEW_RATE*NSEC << (32-SCALE_SHIFT));
    }
    else
    {
        if (offset.nanoseconds > UINT32_MAX)
            diff = UINT64_MAX;
        else
            diff = ((uint64_t)offset.nanoseconds<<32) + offset.fractions;
        /* scaleFactor = 1.0 - diff/SLEW_RATE */
        scale = (1ull<<SCALE_SHIFT) - diff / (SLEW_RATE*NSEC << (32-SCALE_SHIFT));
    }

    /* Update clocks to match tick rate */
    if (ppb < 0)
        clock_comp += -ppb * clock_comp / NSEC;
    else
        clock_comp -= ppb * clock_comp / NSEC;

    /* Adjust for offsetFromMaster */
    clock_comp = clock_comp * scale >> SCALE_SHIFT;

    *ptp_clock_comp = clock_comp;
    return;
#else
#ifndef ADJ_NANO
    #define ADJ_NANO 0x2000	/* select nanosecond resolution */
#endif
    struct timex tinfo;

    /* Don't do anything if an update isn't necessary */
    if (!offset.nanoseconds)
        return;

    /* Get and then update the current clock settings */
    memset(&tinfo, 0, sizeof(tinfo));
    adjtimex(&tinfo);
    tinfo.modes = ADJ_OFFSET|ADJ_MAXERROR|ADJ_ESTERROR|ADJ_STATUS|ADJ_NANO;
    tinfo.status |= STA_PLL;
    tinfo.offset = -offset.nanoseconds*3;
    tinfo.esterror = (offset.nanoseconds < 0) ? (-offset.nanoseconds / 1000) : (offset.nanoseconds / 1000);
    tinfo.maxerror = tinfo.esterror * 2;
    if (adjtimex(&tinfo) == -1)
        perror("timestamp:adjtimex");
#endif
}


/**
 * Update a moving average with a new sample
 *
 * @param avg    Average to update
 * @param update New value
 * @param shift  Shift representing the number of samples in the average. Changing
 *               this value will force a reset of the moving average
 */
void timestamp_avg_update(timestamp_avg_t *avg, const timestamp_t update, int shift)
{
    avg->last = update;
    if (avg->shift != shift)
    {
        avg->shift = shift;
        avg->average = timestamp_shift(avg->last, avg->shift);
        avg->deviation = TIMESTAMP_ZERO;
    }
    else
    {
        timestamp_t tmp;
        uint64_t f;
        int mult = (1<<avg->shift)-1;
        timestamp_t average = timestamp_shift(avg->average, -avg->shift);

        /* Multiply update by 2^shift-1 */
        tmp = avg->average;
        tmp.nanoseconds *= mult;
        f = (uint64_t)tmp.fractions * mult;
        tmp.nanoseconds += f >> 32;
        tmp.fractions = f & MASK32;
        tmp = timestamp_add(tmp, timestamp_shift(avg->last, avg->shift));
        avg->average = timestamp_shift(tmp, -avg->shift);

        /* Multiply update by shift-1 */
        tmp = avg->deviation;
        tmp.nanoseconds *= mult;
        f = (uint64_t)tmp.fractions * mult;
        tmp.nanoseconds += f >> 32;
        tmp.fractions = f & MASK32;

        timestamp_t last_deviation = timestamp_sub(average, avg->last);
        if (last_deviation.nanoseconds < 0)
            last_deviation = timestamp_sub(TIMESTAMP_ZERO, last_deviation);
        tmp = timestamp_add(tmp, timestamp_shift(last_deviation, avg->shift));
        avg->deviation = timestamp_shift(tmp, -avg->shift);
    }
}


/**
 * Get the current average value
 *
 * @param avg    Average to get from
 *
 * @return Average value
 */
timestamp_t timestamp_avg_get_avg(const timestamp_avg_t *avg)
{
    return timestamp_shift(avg->average, -avg->shift);
}


/**
 * Get standard deviation of the average
 *
 * @param avg    Average to get from
 *
 * @return Standard deviation
 */
timestamp_t timestamp_avg_get_deviation(const timestamp_avg_t *avg)
{
    return timestamp_shift(avg->deviation, -avg->shift);
}


/**
 * Get the last update value for an Average
 *
 * @param avg    Average to get from
 *
 * @return Last update value
 */
timestamp_t timestamp_avg_get_last(const timestamp_avg_t *avg)
{
    return avg->last;
}


/**
 * Get either the last update or the average value depening on
 * which one is closer to zero.
 *
 * @param avg    Average to get from
 *
 * @return Closest value to zero
 */
timestamp_t timestamp_avg_get_min(const timestamp_avg_t *avg)
{
    timestamp_t a = timestamp_avg_get_avg(avg);
    if (ABS(a.nanoseconds) > ABS(avg->last.nanoseconds))
        return avg->last;
    else
        return a;
}


/**
 * Convert the Average into a human readable string
 *
 * @param avg    Average to convert
 * @param buffer Buffer to fill
 *
 * @return The filled buffer
 */
const char *timestamp_avg_to_string(const timestamp_avg_t *avg, char *buffer)
{
    timestamp_t last = timestamp_avg_get_last(avg);
    timestamp_t a = timestamp_avg_get_avg(avg);
    timestamp_t a_dev = timestamp_avg_get_deviation(avg);
    uint64_t last_sec = ABS(last.nanoseconds) / NSEC;
    uint64_t last_nano = ABS(last.nanoseconds) % NSEC;
    uint64_t a_sec = ABS(a.nanoseconds) / NSEC;
    uint64_t a_nano = ABS(a.nanoseconds) % NSEC;
    int count = 0;

    if (last.nanoseconds<0)
        count += sprintf(buffer + count, "-%llu.%09llu", (unsigned long long)last_sec, (unsigned long long)last_nano);
    else
        count += sprintf(buffer + count, "+%llu.%09llu", (unsigned long long)last_sec, (unsigned long long)last_nano);
    if (a.nanoseconds<0)
        count += sprintf(buffer + count, ", avg -%llu.%09llu", (unsigned long long)a_sec, (unsigned long long)a_nano);
    else
        count += sprintf(buffer + count, ", avg +%llu.%09llu", (unsigned long long)a_sec, (unsigned long long)a_nano);
    count += sprintf(buffer + count, "+/-%lluppb", (unsigned long long)a_dev.nanoseconds);
    return buffer;
}

