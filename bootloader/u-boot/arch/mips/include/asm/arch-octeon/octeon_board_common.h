/***********************license start************************************
 * Copyright (c) 2012 Cavium Inc. (support@cavium.com). All rights
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
 * support@cavium.com
 *
 ***********************license end**************************************/
#ifndef __OCTEON_BOARD_COMMON_H__
#define __OCTEON_BOARD_COMMON_H__

#include <asm/arch/cvmx-app-init.h>

#define OCTEON_PF_FLR_WAIT_MSEC		99

/* NOTE: All of these functions are weak and may be overwritten */

/**
 * Dynamically adjust the board name, used for prompt generation
 * @param name - name string to be adjusted
 * @param max_len - maximum length of name
 *
 * This function can overwrite the name of a board, for example based on
 * the processor installed.
 */
void octeon_adjust_board_name(char *name, size_t max_len);

/**
 * Generates a random MAC address using the board revision and serial string
 *
 * NOTE: Only the 5th byte is random.
 */
void octeon_board_create_random_mac_addr(void);

/**
 * Extracts and fills in the GD MAC address from the TLV EEPROM.  If not
 * found in the TLV EEPROM then it calls octeon_create_random_mac_addr().
 */
void octeon_board_get_mac_addr(void);

/**
 * Gets the board clock info from the TLV EEPROM or uses the default value
 * if not available.
 *
 * @param def_ddr_clock_mhz	Default DDR clock speed in MHz
 */
void octeon_board_get_clock_info(uint32_t def_ddr_clock_mhz);

/**
 * Reads the board descriptor from the TLV EEPROM or fills in the default
 * values.
 *
 * @param type		board type
 * @param rev_major	board major revision
 * @param rev_minor	board minor revision
 */
void octeon_board_get_descriptor(enum cvmx_board_types_enum type,
				 int rev_major, int rev_minor);

/**
 * Function to write string to LED display
 * @param str - string up to 8 characters to write.
 */
void octeon_led_str_write(const char *str);

/**
 * Initializes basic MDIO support for OCTEON.  This function
 * must be called before using any miiphy_read/write calls.
 *
 * @return 0 for success, otherwise error
 */
int octeon_mdio_initialize(void);

#ifdef CONFIG_OCTEON_ENABLE_PAL
/**
 * Display information provided by the Cavium on-board PAL device.
 */
void show_pal_info(void);
#endif

/**
 * User-defined function called just before initializing networking support
 */
void board_net_preinit(void);

/**
 * User-defined function called just after initializing networking and MDIO
 * support
 */
void board_net_postinit(void);

/**
 * User-defined function called to initialize the PHY devices for a board.
 */
void board_mdio_init(void);

/**
 * User-defined function that is called when the console is waiting for or
 * receiving serial input.
 *
 * If defined this function will be called repeatedly while the command line
 * is waiting for input.
 */
void octeon_board_poll(void);

/**
 * Called before USB is initialized
 *
 * @return 0 for success, error otherwise.
 *
 * NOTE: USB will not be initialized if this function returns an error.
 */
int board_usb_preinit(void);

/**
 * Called after USB is initialized
 */
void board_usb_postinit(void);

/**
 * Reads a range of fuses into a buffer starting at a buffer offset bit
 *
 * @param	node		OCX node number
 * @param	fuse_start	Starting fuse number [0..octeon_get_num_fuses()-1]
 * @param	fuse_end	Ending fuse number [0..octeon_get_num_fuses()-1]
 * @param[out]	buffer		Buffer to store the fuses into
 * @param	buf_start_bit	Starting bit number in the buffer to write
 *				fuse data to.
 *
 * @return	0 for success, -1 on error
 *
 * NOTE: The buffer must be cleared by the caller before calling this function
 *	 since this function is designed to insert or append fuse values into
 *	 an existing buffer.
 */
int octeon_get_fuse(int node, int fuse_start, int fuse_end, uint8_t *buffer,
		    int buf_start_bit);

/**
 * Fills in a buffer with an 80-bit unique Octeon chip ID
 *
 * @param[out]	buffer	10-byte buffer to store the unique ID
 *
 * @return	0 for success, -1 on error
 */
int octeon_get_unique_id(int node, uint8_t buffer[10]);

/**
 * Saves the PCI ID as read from the EEPROM for use by octeon_board_restore_pf()
 *
 * @return	0 on success, otherwise at least one of the PFs is progammed to
 *		the default as encoded by the Octeon chip (i.e. 0x177d9703).
 *
 * NOTE:	This function should be called from early_board_init_r();
 */
int octeon_board_save_pcie_id_override(void);

/**
 * If there is a PF FLR then the PCI EEPROM is not re-read.  In this case
 * we need to re-program the vendor and device ID immediately after hardware
 * completes FLR.
 *
 * PCI spec requires FLR to be completed within 100ms.  The user who triggered
 * FLR expects hardware to finish FLR within 100ms, otherwise the user will
 * end up reading DEVICE_ID incorrectly from the reset value.
 * CN23XX exits FLR at any point between 66 and 99ms, so U-Boot has to wait
 * 99ms to let hardware finish its part, then finish reprogramming the
 * correct device ID before the end of 100ms.
 *
 * Note: this solution only works properly when there is no other activity
 * within U-Boot for 100ms from the time FLR is triggered.
 *
 * This function gets called every 100usec.  If FLR happens during any
 * other activity like bootloader/image update then it is possible that
 * this function does not get called for more than the FLR period which will
 * cause the PF device ID restore to happen after whoever initiated the FLR to
 * read the incorrect device ID 0x9700 (reset value) instead of 0x9702
 * (restored value).
 */
void octeon_board_restore_pf(void);

#endif /* __OCTEON_BOARD_COMMON_H__ */
