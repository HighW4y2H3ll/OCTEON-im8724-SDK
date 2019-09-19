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

#ifndef __PTP_CSRS_H__
#define __PTP_CSRS_H__

/* For compatibility with Linux definitions... */
#if __BYTE_ORDER == __BIG_ENDIAN
# ifndef __BIG_ENDIAN_BITFIELD
#  define __BIG_ENDIAN_BITFIELD
# endif
#else
# ifndef __LITTLE_ENDIAN_BITFIELD
#  define __LITTLE_ENDIAN_BITFIELD
# endif
#endif
#if defined(__BIG_ENDIAN_BITFIELD) && defined(__LITTLE_ENDIAN_BITFIELD)
# error Cannot define both __BIG_ENDIAN_BITFIELD and __LITTLE_ENDIAN_BITFIELD
#endif

#define PTPLIB_MIO_PTP_CLOCK_CFG          0x8001070000000F00ull
#define PTPLIB_MIO_PTP_CLOCK_LO           0x8001070000000F08ull
#define PTPLIB_MIO_PTP_CLOCK_HI           0x8001070000000F10ull
#define PTPLIB_MIO_PTP_CLOCK_COMP         0x8001070000000F18ull
#define PTPLIB_MIO_PTP_TIMESTAMP          0x8001070000000F20ull
#define PTPLIB_MIO_PTP_EVT_CNT            0x8001070000000F28ull
#define PTPLIB_MIO_PTP_CKOUT_THRESH_LO    0x8001070000000F30ull
#define PTPLIB_MIO_PTP_CKOUT_THRESH_HI    0x8001070000000F38ull
#define PTPLIB_MIO_PTP_CKOUT_HI_INCR      0x8001070000000F40ull
#define PTPLIB_MIO_PTP_CKOUT_LO_INCR      0x8001070000000F48ull
#define PTPLIB_MIO_PTP_PPS_THRESH_LO      0x8001070000000F50ull
#define PTPLIB_MIO_PTP_PPS_THRESH_HI      0x8001070000000F58ull
#define PTPLIB_MIO_PTP_PPS_HI_INCR        0x8001070000000F60ull
#define PTPLIB_MIO_PTP_PPS_LO_INCR        0x8001070000000F68ull
#define PTPLIB_GPIO_BIT_CFGX(offset)      (0x8001070000000800ull + ((offset) & 15) * 8)
#define PTPLIB_GPIO_XBIT_CFGX(offset)     (0x8001070000000900ull + ((offset) & 31) * 8 - 8*16)
#define PTPLIB_GPIO_BIT_CFG(offset)       ((offset<16) ? PTPLIB_GPIO_BIT_CFGX(offset) : PTPLIB_GPIO_XBIT_CFGX(offset))

/**
 * ptplib_mio_ptp_ckout_hi_incr
 *
 * MIO_PTP_CKOUT_HI_INCR = PTP Clock Out Hi Increment
 *
 */
union ptplib_mio_ptp_ckout_hi_incr {
	uint64_t u64;
	struct ptplib_mio_ptp_ckout_hi_incr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 32; /**< Nanoseconds */
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t nanosec                      : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_ckout_hi_incr ptplib_mio_ptp_ckout_hi_incr_t;

/**
 * ptplib_mio_ptp_ckout_lo_incr
 *
 * MIO_PTP_CKOUT_LO_INCR = PTP Clock Out Lo Increment
 *
 */
union ptplib_mio_ptp_ckout_lo_incr {
	uint64_t u64;
	struct ptplib_mio_ptp_ckout_lo_incr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 32; /**< Nanoseconds */
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t nanosec                      : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_ckout_lo_incr ptplib_mio_ptp_ckout_lo_incr_t;

/**
 * ptplib_mio_ptp_ckout_thresh_hi
 *
 * MIO_PTP_CKOUT_THRESH_HI = Hi bytes of PTP Clock Out
 *
 * Writes to MIO_PTP_CKOUT_THRESH_HI also clear MIO_PTP_CKOUT_THRESH_LO. To update all 96 bits, write MIO_PTP_CKOUT_THRESH_HI followed
 * by MIO_PTP_CKOUT_THRESH_LO
 */
union ptplib_mio_ptp_ckout_thresh_hi {
	uint64_t u64;
	struct ptplib_mio_ptp_ckout_thresh_hi_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 64; /**< Nanoseconds */
#else
	uint64_t nanosec                      : 64;
#endif
	} s;
};
typedef union ptplib_mio_ptp_ckout_thresh_hi ptplib_mio_ptp_ckout_thresh_hi_t;

/**
 * ptplib_mio_ptp_ckout_thresh_lo
 *
 * MIO_PTP_CKOUT_THRESH_LO = Lo bytes of PTP Clock Out
 *
 */
union ptplib_mio_ptp_ckout_thresh_lo {
	uint64_t u64;
	struct ptplib_mio_ptp_ckout_thresh_lo_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_ckout_thresh_lo ptplib_mio_ptp_ckout_thresh_lo_t;

/**
 * ptplib_mio_ptp_clock_cfg
 *
 * MIO_PTP_CLOCK_CFG = Configuration
 *
 */
union ptplib_mio_ptp_clock_cfg {
	uint64_t u64;
	struct ptplib_mio_ptp_clock_cfg_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_42_63               : 22;
	uint64_t pps                          : 1;  /**< PTP PPS Output
                                                         reflects ptp__pps after PPS_INV inverter */
	uint64_t ckout                        : 1;  /**< PTP Clock Output
                                                         reflects ptp__ckout after CKOUT_INV inverter */
	uint64_t ext_clk_edge                 : 2;  /**< External Clock input edge
                                                         00 = rising edge
                                                         01 = falling edge
                                                         10 = both rising & falling edge
                                                         11 = reserved */
	uint64_t ckout_out4                   : 1;  /**< Destination for PTP Clock Out output
                                                         0-19 : GPIO[[CKOUT_OUT4,CKOUT_OUT[3:0]]]
                                                         This should be different from PPS_OUT */
	uint64_t pps_out                      : 5;  /**< Destination for PTP PPS output
                                                         0-19 : GPIO[PPS_OUT[4:0]]
                                                         This should be different from CKOUT_OUT */
	uint64_t pps_inv                      : 1;  /**< Invert PTP PPS
                                                         0 = don't invert
                                                         1 = invert */
	uint64_t pps_en                       : 1;  /**< Enable PTP PPS */
	uint64_t ckout_out                    : 4;  /**< Destination for PTP Clock Out output
                                                         0-19 : GPIO[[CKOUT_OUT4,CKOUT_OUT[3:0]]]
                                                         This should be different from PPS_OUT */
	uint64_t ckout_inv                    : 1;  /**< Invert PTP Clock Out
                                                         0 = don't invert
                                                         1 = invert */
	uint64_t ckout_en                     : 1;  /**< Enable PTP Clock Out */
	uint64_t evcnt_in                     : 6;  /**< Source for event counter input
                                                         0x00-0x0f : GPIO[EVCNT_IN[3:0]]
                                                         0x10      : QLM0_REF_CLK
                                                         0x11      : QLM1_REF_CLK
                                                         0x12      : QLM2_REF_CLK
                                                         0x13      : QLM3_REF_CLK
                                                         0x14      : QLM4_REF_CLK
                                                         0x15-0x3f : Reserved */
	uint64_t evcnt_edge                   : 1;  /**< Event counter input edge
                                                         0 = falling edge
                                                         1 = rising edge */
	uint64_t evcnt_en                     : 1;  /**< Enable event counter */
	uint64_t tstmp_in                     : 6;  /**< Source for timestamp input
                                                         0x00-0x0f : GPIO[TSTMP_IN[3:0]]
                                                         0x10      : QLM0_REF_CLK
                                                         0x11      : QLM1_REF_CLK
                                                         0x12      : QLM2_REF_CLK
                                                         0x13      : QLM3_REF_CLK
                                                         0x14      : QLM4_REF_CLK
                                                         0x15-0x3f : Reserved */
	uint64_t tstmp_edge                   : 1;  /**< External timestamp input edge
                                                         0 = falling edge
                                                         1 = rising edge */
	uint64_t tstmp_en                     : 1;  /**< Enable external timestamp */
	uint64_t ext_clk_in                   : 6;  /**< Source for external clock
                                                         0x00-0x0f : GPIO[EXT_CLK_IN[3:0]]
                                                         0x10      : QLM0_REF_CLK
                                                         0x11      : QLM1_REF_CLK
                                                         0x12      : QLM2_REF_CLK
                                                         0x13      : QLM3_REF_CLK
                                                         0x14      : QLM4_REF_CLK
                                                         0x15-0x3f : Reserved */
	uint64_t ext_clk_en                   : 1;  /**< Use positive edge of external clock */
	uint64_t ptp_en                       : 1;  /**< Enable PTP Module */
#else
	uint64_t ptp_en                       : 1;
	uint64_t ext_clk_en                   : 1;
	uint64_t ext_clk_in                   : 6;
	uint64_t tstmp_en                     : 1;
	uint64_t tstmp_edge                   : 1;
	uint64_t tstmp_in                     : 6;
	uint64_t evcnt_en                     : 1;
	uint64_t evcnt_edge                   : 1;
	uint64_t evcnt_in                     : 6;
	uint64_t ckout_en                     : 1;
	uint64_t ckout_inv                    : 1;
	uint64_t ckout_out                    : 4;
	uint64_t pps_en                       : 1;
	uint64_t pps_inv                      : 1;
	uint64_t pps_out                      : 5;
	uint64_t ckout_out4                   : 1;
	uint64_t ext_clk_edge                 : 2;
	uint64_t ckout                        : 1;
	uint64_t pps                          : 1;
	uint64_t reserved_42_63               : 22;
#endif
	} s;
};
typedef union ptplib_mio_ptp_clock_cfg ptplib_mio_ptp_clock_cfg_t;

/**
 * ptplib_mio_ptp_clock_comp
 *
 * MIO_PTP_CLOCK_COMP = Compensator
 *
 */
union ptplib_mio_ptp_clock_comp {
	uint64_t u64;
	struct ptplib_mio_ptp_clock_comp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 32; /**< Nanoseconds */
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t nanosec                      : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_clock_comp ptplib_mio_ptp_clock_comp_t;

/**
 * ptplib_mio_ptp_clock_hi
 *
 * MIO_PTP_CLOCK_HI = Hi bytes of CLOCK
 *
 * Writes to MIO_PTP_CLOCK_HI also clear MIO_PTP_CLOCK_LO. To update all 96 bits, write MIO_PTP_CLOCK_HI followed
 * by MIO_PTP_CLOCK_LO
 */
union ptplib_mio_ptp_clock_hi {
	uint64_t u64;
	struct ptplib_mio_ptp_clock_hi_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 64; /**< Nanoseconds */
#else
	uint64_t nanosec                      : 64;
#endif
	} s;
};
typedef union ptplib_mio_ptp_clock_hi ptplib_mio_ptp_clock_hi_t;

/**
 * ptplib_mio_ptp_clock_lo
 *
 * MIO_PTP_CLOCK_LO = Lo bytes of CLOCK
 *
 */
union ptplib_mio_ptp_clock_lo {
	uint64_t u64;
	struct ptplib_mio_ptp_clock_lo_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_clock_lo ptplib_mio_ptp_clock_lo_t;

/**
 * ptplib_mio_ptp_evt_cnt
 *
 * MIO_PTP_EVT_CNT = Event Counter
 *
 * Writes to MIO_PTP_EVT_CNT increment this register by the written data. The register counts down by
 * 1 for every MIO_PTP_CLOCK_CFG[EVCNT_EDGE] edge of MIO_PTP_CLOCK_CFG[EVCNT_IN]. When register equals
 * 0, an interrupt gets gerated
 */
union ptplib_mio_ptp_evt_cnt {
	uint64_t u64;
	struct ptplib_mio_ptp_evt_cnt_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t cntr                         : 64; /**< Nanoseconds */
#else
	uint64_t cntr                         : 64;
#endif
	} s;
};
typedef union ptplib_mio_ptp_evt_cnt ptplib_mio_ptp_evt_cnt_t;

/**
 * ptplib_mio_ptp_pps_hi_incr
 *
 * MIO_PTP_PPS_HI_INCR = PTP PPS Hi Increment
 *
 */
union ptplib_mio_ptp_pps_hi_incr {
	uint64_t u64;
	struct ptplib_mio_ptp_pps_hi_incr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 32; /**< Nanoseconds */
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t nanosec                      : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_pps_hi_incr ptplib_mio_ptp_pps_hi_incr_t;

/**
 * ptplib_mio_ptp_pps_lo_incr
 *
 * MIO_PTP_PPS_LO_INCR = PTP PPS Lo Increment
 *
 */
union ptplib_mio_ptp_pps_lo_incr {
	uint64_t u64;
	struct ptplib_mio_ptp_pps_lo_incr_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 32; /**< Nanoseconds */
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t nanosec                      : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_pps_lo_incr ptplib_mio_ptp_pps_lo_incr_t;

/**
 * ptplib_mio_ptp_pps_thresh_hi
 *
 * MIO_PTP_PPS_THRESH_HI = Hi bytes of PTP PPS
 *
 * Writes to MIO_PTP_PPS_THRESH_HI also clear MIO_PTP_PPS_THRESH_LO. To update all 96 bits, write MIO_PTP_PPS_THRESH_HI followed
 * by MIO_PTP_PPS_THRESH_LO
 */
union ptplib_mio_ptp_pps_thresh_hi {
	uint64_t u64;
	struct ptplib_mio_ptp_pps_thresh_hi_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 64; /**< Nanoseconds */
#else
	uint64_t nanosec                      : 64;
#endif
	} s;
};
typedef union ptplib_mio_ptp_pps_thresh_hi ptplib_mio_ptp_pps_thresh_hi_t;

/**
 * ptplib_mio_ptp_pps_thresh_lo
 *
 * MIO_PTP_PPS_THRESH_LO = Lo bytes of PTP PPS
 *
 */
union ptplib_mio_ptp_pps_thresh_lo {
	uint64_t u64;
	struct ptplib_mio_ptp_pps_thresh_lo_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t reserved_32_63               : 32;
	uint64_t frnanosec                    : 32; /**< Fractions of Nanoseconds */
#else
	uint64_t frnanosec                    : 32;
	uint64_t reserved_32_63               : 32;
#endif
	} s;
};
typedef union ptplib_mio_ptp_pps_thresh_lo ptplib_mio_ptp_pps_thresh_lo_t;

/**
 * ptplib_mio_ptp_timestamp
 *
 * MIO_PTP_TIMESTAMP = Timestamp latched on MIO_PTP_CLOCK_CFG[TSTMP_EDGE] edge of MIO_PTP_CLOCK_CFG[TSTMP_IN]
 *
 */
union ptplib_mio_ptp_timestamp {
	uint64_t u64;
	struct ptplib_mio_ptp_timestamp_s {
#ifdef __BIG_ENDIAN_BITFIELD
	uint64_t nanosec                      : 64; /**< Nanoseconds */
#else
	uint64_t nanosec                      : 64;
#endif
	} s;
};
typedef union ptplib_mio_ptp_timestamp ptplib_mio_ptp_timestamp_t;

#endif
