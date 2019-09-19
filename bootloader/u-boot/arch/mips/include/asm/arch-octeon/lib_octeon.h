/***********************license start************************************
 * Copyright (c) 2004-2007 Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * Neither the name of Cavium Inc. nor the names of
 *       its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
 * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
 * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
 * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
 * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
 * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 *
 *
 * For any questions regarding licensing please contact
 * marketing@cavium.com
 *
 ***********************license end**************************************/

/**
 *
 * $Id: lib_octeon.h 120938 2015-07-01 10:43:28Z awilliams $
 *
 */

#ifndef __LIB_OCTEON_H___
#define __LIB_OCTEON_H___

#include <asm/arch/cvmx-bootloader.h>

#if (CONFIG_OCTEON_EBT3000 || CONFIG_OCTEON_EBT5800)
int octeon_ebt3000_get_board_major_rev(void);
int octeon_ebt3000_get_board_minor_rev(void);
#endif
int octeon_show_info(void);
void octeon_led_str_write_std(const char *str);
int octeon_env_init(void);
int octeon_get_cpu_multiplier(void);
uint64_t octeon_get_eclk_hz(void);
uint64_t octeon_get_ioclk_hz(void);

int uboot_octeon_pci_console_setup (void);
int uboot_octeon_pci_console_uninit(void);
int i2c_write(uchar chip, uint addr, int alen, uchar * buffer, int len);
uint32_t octeon_find_and_validate_normal_bootloader(int rescan_flag);
int failsafe_scan(void);
void octeon_init_cvmcount(void);
uint64_t octeon_get_io_clock_rate(void);

/**
 * Validate image header. Intended for actual header discovery.
 * Not NAND or NOR specific
 *
 * @param header     Address of expected image header.
 *
 * @return 0  on success
 *         1  on failure
 */
int validate_bootloader_header(const bootloader_header_t *header);

/**
 * Calculates the CRC of the specified bootloader header
 *
 * @param header pointer to bootloader header
 *
 * @return CRC32 of the header, using zero for the CRC field.
 */
uint32_t calculate_header_crc(const bootloader_header_t *header);

/**
 * Calculates the CRC of a bootloader image not including the header.
 *
 * @param header pointer to header in front of the image
 *
 * @return CRC32 of the payload of the U-Boot image.
 */
uint32_t calculate_image_crc(const bootloader_header_t *header);

/**
 * Returns if OCTEON originally booted from NOR flash or not.
 *
 * @return 1 if booted from NOR flash or 0 if not
 */
int octeon_is_boot_nor(void);

#endif /*  __LIB_OCTEON_H___  */
