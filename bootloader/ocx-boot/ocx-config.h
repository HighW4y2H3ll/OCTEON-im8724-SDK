/***********************license start***************
 * Copyright (c) 2016  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

#ifndef __CONFIG_OCX_H__
#define __CONFIG_OCX_H__

/* UART configuration */
#define UART_NODE		0	/** Node for UART with 78XX */
/**
 * UART port to output to
 */
#define UART_PORT		0	/** Output to the first uart. This can be 0 or 1 */

/**
 * Baud rate to use
 */
#define UART_BAUD_RATE		115200	/* Baud rate for the uart */

/**
 * During TX Link Trainingthis is the TX POST Tap value that is used when
 * the INITIALIZE coefficients update is received.  It is also the TX SWING Tap
 * value used when the Base-R Link Training begins.
 *
 * The default value is 0xD.
 *
 * This is for the opposite node.
 */
#define CONFIG_OCTEON_CCPI_TX_SWING_NO		0x0D

/**
 * During TX Base-R Link Training, this is the TX PRE and Post Tap
 * values that are used when the INITIALIZE coefficients update is
 * received.  It is also the TX PRE and Post Tap value used when the Base-R Link
 * Training begins.  The upper nubble is the post tap and the lower nibble is
 * the pre-tap.
 *
 * The default value is 0xD0
 *
 * This is for the opposite node.
 */
#define CONFIG_OCTEON_CCPI_TX_PREEMP_TAP_NO	0xD0

/**
 * Loss in db for CCPI channel.  Typically this only matters if it is less
 * than 10 or greater than or equal to 10.
 *
 * The default is 0.
 */
#define CONFIG_OCTEON_CCPI_CHANNEL_LOSS		0

/**
 * During TX Link Trainingthis is the TX POST Tap value that is used when
 * the INITIALIZE coefficients update is received.  It is also the TX SWING Tap
 * value used when the Base-R Link Training begins.
 *
 * The default value is 0xD.
 *
 * This is for the local node.
 */
#define CONFIG_OCTEON_CCPI_TX_SWING		0x0D

/**
 * During TX Base-R Link Training, this is the TX PRE and Post Tap
 * values that are used when the INITIALIZE coefficients update is
 * received.  It is also the TX PRE and Post Tap value used when the Base-R Link
 * Training begins.  The upper nubble is the post tap and the lower nibble is
 * the pre-tap.
 *
 * The default value is 0xD0
 *
 * This is for the local node.
 */
#define CONFIG_OCTEON_CCPI_TX_PREEMP_TAP	0xD0

/**
 * This enables printing out verbose information.  Note that this grows the
 * size of the stage 1 bootloader.
 */
#define VERBOSE_OUTPUT

/**
 * String that is printed out at the very beginning just before executing
 * C code.  This is optional and does not need to be defined.
 */
#define STAGE1_START_STRING	"\r\nOCTEON OCX remote bootloader\r\n"

/**
 * Address where the stage 1 bootloader is loaded by the on-chip ROM
 */
#define STAGE1_DRAM_ADDR	0xFFFFFFFF81000000ull

/**
 * Maximum size of the stage 1 bootloader.  The on-chip ROM only loads
 * the first 8K.
 */
#define STAGE1_BOOTLOADER_SIZE	(8*1024)

/**
 * reference clock frequency.
 */
#define REF_CLOCK		50000000	/* 50 MHz */

#endif	/* __CONFIG_OCX_H__ */
