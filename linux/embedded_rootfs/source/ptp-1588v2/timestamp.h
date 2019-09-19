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

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__
/**
 * @file
 * Utility functions for dealing with accurate timestamps.
 */

#define NSEC 1000000000ll /* Number of nanoseconds in one second */

/**
 * This structure describes the internal timestamp format we use.
 * It can represent positive or negative times in 1/(2**32) fractions
 * of a second. The format is a twos compliment 96bit integer
 * divided into two parts.
 */
typedef struct
{
    int64_t     nanoseconds;
    uint32_t    fractions;
} timestamp_t;

/**
 * This structure describes a moving average of timestamps.
 */
typedef struct
{
    timestamp_t average;
    timestamp_t deviation;
    timestamp_t last;
    int shift;
} timestamp_avg_t;

extern const timestamp_t TIMESTAMP_ZERO;
extern const timestamp_t TIMESTAMP_BAD;
extern const timestamp_t TIMESTAMP_MAX;

/**
 * Return true if the supplied timestamp is BAD_TIMESTAMP.
 *
 * @param t
 *
 * @return
 */
extern int timestamp_is_bad(const timestamp_t t);

/**
 * Result = a + b
 *
 * @param a
 * @param b
 *
 * @return
 */
extern timestamp_t timestamp_add(const timestamp_t a, const timestamp_t b);

/**
 * Result = a - b
 *
 * @param a
 * @param b
 *
 * @return
 */
extern timestamp_t timestamp_sub(const timestamp_t a, timestamp_t b);

/**
 * Result = a / 2
 *
 * @param a
 *
 * @return
 */
extern timestamp_t timestamp_half(const timestamp_t a);

/**
 * Return if timestamp "a" is before timestamp "b".
 *
 * @param a
 * @param b
 *
 * @return Non zero if A is before B
 */
extern int timestamp_before(const timestamp_t a, const timestamp_t b);

/**
 * Return if timestamp is in the range of min_nanosec to max_nanosec.
 *
 * @param a
 * @param nanosec
 *
 * @return
 */
extern int timestamp_in_range(const timestamp_t a, int64_t min_nanosec, int64_t max_nanosec);

/**
 * Convert the internal timestamp format into a PTP timestamp
 * suitable for use inside PTP packets. Any endian swapping necessary is
 * automatically performed.
 *
 * @param internal Internal timestamp to convert
 *
 * @return PTP formatted timestamp
 */
extern ptp_timestamp_t timestamp_to_ptp(const timestamp_t internal);

/**
 * Convert a PTP packet timestamp into our internal format. Any endian swapping
 * necessary is automatically performed.
 *
 * @param ptp    Timestamp to convert
 *
 * @return Internal formatted timestamp
 */
extern timestamp_t timestamp_from_ptp(const ptp_timestamp_t ptp);

/**
 * Convert a PTP correctionField into an internal timestamp delta.
 * Corrections in PTP are in 1/(2**16) of a nanosecond. Any endian
 * swapping necessary is automatically performed.
 *
 * @param correction PTP correctionField value
 *
 * @return Internal timestamp delta
 */
extern timestamp_t timestamp_from_correction(const uint64_t correction);

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
extern uint64_t timestamp_to_correction(const timestamp_t delta);

/**
 * Convert a timestamp into a TIMESTAMP_STRING_LEN length string
 *
 * @param a      Timestamp to convert
 * @param buffer String to hold the result
 *
 * @return Same as buffer
 */
extern const char *timestamp_to_string(const timestamp_t a, char *buffer);
#define TIMESTAMP_STRING_LEN 32

/**
 * Get the current raw time as a timestamp_t. Raw time is not affected by
 * set_systime nor adjust_systime. It is not considered an accurate clock and is
 * only used for rough intervals.
 *
 * @return The current raw time
 */
extern timestamp_t timestamp_get_rawtime(void);

/**
 * Get the current system time as a timestamp_t
 *
 * @param:
 *  1 - use HW PTP clock is possible
 *  0 - use linux clock
 * 
 * @return The current system time
 */
extern timestamp_t timestamp_get_systime(int);

/**
 * Set the current system time
 *
 * @param new_time New time to set
 */
extern void timestamp_set_systime(timestamp_t new_time);

/**
 * Adjust the system time by changing the frequency of the clock.
 *
 * @param ppb    Amount clock should be adjusted in parts per billion. Positive
 *               means the clock is running fast and should slow down.
 * @param offset Current offset from real time. Positive is this clock is fast,
 *               negative if it is slow. Used to skew rate to reduce this value
 *               over time.
 */
extern void timestamp_adjust_systime(int64_t ppb, timestamp_t offset);

/**
 * Update a moving average with a new sample
 *
 * @param avg    Average to update
 * @param update New value
 * @param shift  Shift representing the number of samples in the average. Changing
 *               this value will force a reset of the moving average
 */
extern void timestamp_avg_update(timestamp_avg_t *avg, const timestamp_t update, int shift);

/**
 * Get the current average value
 *
 * @param avg    Average to get from
 *
 * @return Average value
 */
extern timestamp_t timestamp_avg_get_avg(const timestamp_avg_t *avg);

/**
 * Get standard deviation of the average
 *
 * @param avg    Average to get from
 *
 * @return Standard deviation
 */
extern timestamp_t timestamp_avg_get_deviation(const timestamp_avg_t *avg);

/**
 * Get the last update value for an Average
 *
 * @param avg    Average to get from
 *
 * @return Last update value
 */
extern timestamp_t timestamp_avg_get_last(const timestamp_avg_t *avg);

/**
 * Get either the last update or the average value depening on
 * which one is closer to zero.
 *
 * @param avg    Average to get from
 *
 * @return Closest value to zero
 */
extern timestamp_t timestamp_avg_get_min(const timestamp_avg_t *avg);

/**
 * Convert the Average into a human readable string
 *
 * @param avg    Average to convert
 * @param buffer Buffer to fill
 *
 * @return The filled buffer
 */
extern const char *timestamp_avg_to_string(const timestamp_avg_t *avg, char *buffer);
#define TIMESTAMP_AVG_STRING_LEN 64

#endif
