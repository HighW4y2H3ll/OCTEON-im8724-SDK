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

#if defined(__U_BOOT__)
# include <common.h>
# include <i2c.h>
# include <asm/arch/cvmx.h>
# include <asm/arch/cvmx-twsi.h>
# include <asm/arch/cvmx-helper.h>
# include <asm/arch/cvmx-helper-board.h>
# include <asm/arch/cvmx-helper-sfp.h>
# include <asm/arch/cvmx-helper-fdt.h>
# include <asm/arch/cvmx-helper-util.h>
# include <malloc.h>
#else
# include "cvmx.h"
# include "cvmx-helper.h"
# include "cvmx-bootmem.h"
# include "cvmx-helper-sfp.h"
# include "libfdt/cvmx-helper-fdt.h"
# include "cvmx-twsi.h"
# include "cvmx-malloc.h"
# include "cvmx-helper-util.h"
#endif

static struct cvmx_fdt_sfp_info *sfp_list;

#ifdef DEBUG
# define __DEBUG	1
#else
# define __DEBUG 0
#endif

#ifndef __U_BOOT__
# define debug_cond(cond, fmt, args...)		\
	do {						\
		if (cond)				\
			cvmx_dprintf(fmt, ##args);	\
	} while (0)

# define debug(fmt, args...)				\
	debug_cond(__DEBUG, fmt, ##args)
#endif

/**
 * Local allocator to handle both SE and U-Boot that also zeroes out memory
 *
 * @param	size	number of bytes to allocate
 *
 * @return	pointer to allocated memory or NULL if out of memory.
 *		Alignment is set to 8-bytes.
 */
static void *cvm_sfp_alloc(size_t size)
{
#ifndef __U_BOOT__
	void *ptr;
	ptr = cvmx_bootmem_alloc(size, 8);
	if (ptr)
		memset(ptr, 0, size);
	return ptr;
#else
	return calloc(size, 1);
#endif
}

/**
 * Free allocated memory.
 *
 * @param	ptr	pointer to memory to free
 *
 * NOTE: This only works in U-Boot since SE does not really have a freeing
 *	 mechanism.  In SE the memory is zeroed out and not freed so this
 *	 is a memory leak if errors occur.
 */
static inline void cvm_sfp_free(void *ptr, size_t size)
{
#ifdef __U_BOOT__
	free(ptr);
#else
	if (ptr)
		memset(ptr, 0, size);
#endif
}

/**
 * Select a QSFP device before accessing the EEPROM
 *
 * @param	sfp	handle for sfp/qsfp connector
 * @param	enable	Set true to select, false to deselect
 *
 * @return	0 on success or if SFP or no select GPIO, -1 on GPIO error
 */
static int cvmx_qsfp_select(const struct cvmx_fdt_sfp_info *sfp, bool enable)
{
	/* Select is only needed for QSFP modules */
	if (!sfp->is_qsfp) {
		debug("%s(%s, %d): not QSFP\n", __func__, sfp->name, enable);
		return 0;
	}

	if (sfp->select->of_offset >= 0) {
		/* Note that select is active low */
		return cvmx_fdt_gpio_set(sfp->select, !enable);
	} else {
		debug("%s: select GPIO unknown\n", __func__);
		return 0;
	}
}

static int cvmx_sfp_parse_sfp_buffer(struct cvmx_sfp_mod_info *sfp_info,
				     const uint8_t *buffer)
{
	uint8_t csum = 0;
	bool csum_good = false;
	int i;

	/* Validate the checksum */
	for (i = 0; i < 0x3f; i++)
		csum += buffer[i];
	csum_good = csum == buffer[0x3f];
	debug("%s: Lower checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0x3f]);
	csum = 0;
	for (i = 0x40; i < 0x5f; i++)
		csum += buffer[i];
	debug("%s: Upper checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0x5f]);
	if (csum != buffer[0x5f] || !csum_good) {
		printf("Error: SFP EEPROM checksum information is incorrect\n");
		return -1;
	}

	sfp_info->conn_type = buffer[0];
	if (buffer[1] < 1 || buffer[1] > 7) {	/* Extended ID */
		printf("Error: Unknown SFP extended identifier 0x%x\n",
		       buffer[1]);
		return -1;
	}
	if (buffer[1] != 4) {
		printf("Module is not SFP/SFP+/SFP28/QSFP+\n");
		return -1;
	}
	sfp_info->mod_type = buffer[2];
	sfp_info->eth_comp = buffer[3] & 0xf0;
	sfp_info->cable_comp = buffer[0x24];

	switch (sfp_info->mod_type) {
	case CVMX_SFP_MOD_COPPER_PIGTAIL:
	case CVMX_SFP_MOD_NO_SEP_CONN:
		debug("%s: copper pigtail or no separable cable\n", __func__);
		/* There are several ways a cable can be marked as active or
		 * passive.  8.[2-3] specify the SFP+ cable technology.  Some
		 * modules also use 3.[0-1] for Infiniband, though it's
		 * redundant.
		 */
		sfp_info->copper_cable = true;
		if ((buffer[8] & 0x0C) == 0x08) {
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
		} else if ((buffer[8] & 0xC) == 0x4) {
			sfp_info->limiting = false;
			sfp_info->active_cable = false;
		}
		if ((buffer[3] & 3) == 2) {
			sfp_info->active_cable = true;
			sfp_info->limiting = true;
		}
		break;
	case CVMX_SFP_MOD_RJ45:
		debug("%s: RJ45 adapter\n", __func__);
		sfp_info->copper_cable = true;
		sfp_info->active_cable = true;
		sfp_info->limiting = true;
		break;
	case CVMX_SFP_MOD_UNKNOWN:
		/* The Avago 1000Base-X to 1000Base-T module reports that it
		 * is an unknown module type but the Ethernet compliance code
		 * says it is 1000Base-T.  We'll change the reporting to RJ45.
		 */
		if (buffer[6] & 8) {
			debug("RJ45 gigabit module detected\n");
			sfp_info->mod_type = CVMX_SFP_MOD_RJ45;
			sfp_info->copper_cable = false;
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
			sfp_info->max_copper_cable_len = buffer[0x12];
			sfp_info->rate = CVMX_SFP_RATE_1G;
		} else {
			debug("Unknown module type 0x%x\n", sfp_info->mod_type);
		}
			/* fall through */
		default:
			sfp_info->limiting = true;
			break;
	}
	if (sfp_info->copper_cable)
		sfp_info->max_copper_cable_len = buffer[0x12];
	else
		sfp_info->max_50um_om4_cable_length = buffer[0x12] * 10;

	if (buffer[0xe])
		sfp_info->max_single_mode_cable_length = buffer[0xe] * 1000;
	else
		sfp_info->max_single_mode_cable_length = buffer[0xf] * 100000;

	sfp_info->max_50um_om2_cable_length = buffer[0x10] * 10;
	sfp_info->max_62_5um_om1_cable_length = buffer[0x11] * 10;
	sfp_info->max_50um_om3_cable_length = buffer[0x13] * 10;

	if (buffer[0xc] == 12) {
		sfp_info->rate = CVMX_SFP_RATE_1G;
	} else if (buffer[0xc] == 103) {
		sfp_info->rate = CVMX_SFP_RATE_10G;
	} else if (buffer[0xc] == 0xff) {
		if (buffer[0x42] == 103)
			sfp_info->rate = CVMX_SFP_RATE_25G;
	}

	if (buffer[0xc] < 0xff)
		sfp_info->bitrate_max = buffer[0xc] * 100;
	else
		sfp_info->bitrate_max = buffer[0x42] * 250;

	if ((buffer[8] & 0xc) == 8) {
		if (buffer[0x3c] & 0x4)
			sfp_info->limiting = true;
	}

	/* Currently we only set this for 25G.  FEC is required for CA-S cables
	 * and for cable lengths >= 5M as of this writing.
	 */
	if (((sfp_info->rate == CVMX_SFP_RATE_25G) && (sfp_info->copper_cable)) &&
		((sfp_info->cable_comp == CVMX_SFP_CABLE_25G_CR_CA_S) ||
		(sfp_info->max_copper_cable_len >= 5)))
		sfp_info->fec_required = true;

	/* copy strings and vendor info, strings will be automatically NUL
	 * terminated.
	 */
	memcpy(sfp_info->vendor_name, &buffer[0x14], 16);
	memcpy(sfp_info->vendor_oui, &buffer[0x25], 3);
	memcpy(sfp_info->vendor_pn, &buffer[0x28], 16);
	memcpy(sfp_info->vendor_rev, &buffer[0x38], 4);
	memcpy(sfp_info->vendor_sn, &buffer[0x44], 16);
	memcpy(sfp_info->date_code, &buffer[0x54], 8);

	sfp_info->linear_rx_output = !!(buffer[0x40] & 1);
	sfp_info->cooled_laser = !!(buffer[0x40] & 4);
	sfp_info->internal_cdr = !!(buffer[0x40] & 8);

	if (buffer[0x40] & 0x20)
		sfp_info->power_level = 3;
	else
		sfp_info->power_level = (buffer[0x40] & 2) ? 2 : 1;

	sfp_info->diag_paging = !!(buffer[0x40] & 0x10);
	sfp_info->los_implemented = !!(buffer[0x41] & 2);
	sfp_info->los_inverted = !!(buffer[0x41] & 4);
	sfp_info->tx_fault_implemented = !!(buffer[0x41] & 8);
	sfp_info->tx_disable_implemented = !!(buffer[0x41] & 0x10);
	sfp_info->rate_select_implemented = !!(buffer[0x41] & 0x20);
	sfp_info->tuneable_transmitter = !!(buffer[0x41] & 0x40);
	sfp_info->rx_decision_threshold_implemented = !!(buffer[0x41] & 0x80);


	sfp_info->diag_monitoring = !!(buffer[0x5c] & 0x40);
	sfp_info->diag_rx_power_averaged = !!(buffer[0x5c] & 0x8);
	sfp_info->diag_externally_calibrated = !!(buffer[0x5c] & 0x10);
	sfp_info->diag_internally_calibrated = !!(buffer[0x5c] & 0x20);
	sfp_info->diag_addr_change_required = !!(buffer[0x5c] & 0x4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0x5d] & 2);
	sfp_info->diag_app_select_control = !!(buffer[0x5d] & 4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0x5d] & 8);
	sfp_info->diag_soft_rx_los_implemented = !!(buffer[0x5d] & 0x10);
	sfp_info->diag_soft_tx_fault_implemented = !!(buffer[0x5d] & 0x20);
	sfp_info->diag_soft_tx_disable_implemented = !!(buffer[0x5d] & 0x40);
	sfp_info->diag_alarm_warning_flags_implemented = !!(buffer[0x5d] & 0x80);
	sfp_info->diag_rev = buffer[0x5e];

	return 0;
}

static int cvmx_sfp_parse_qsfp_buffer(struct cvmx_sfp_mod_info *sfp_info,
				      const uint8_t *buffer)
{
	uint8_t csum = 0;
	bool csum_good = false;
	int i;

	/* Validate the checksum */
	for (i = 0x80; i < 0xbf; i++)
		csum += buffer[i];
	csum_good = csum == buffer[0xbf];
	debug("%s: Lower checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0xbf]);
	csum = 0;
	for (i = 0xc0; i < 0xdf; i++)
		csum += buffer[i];
	debug("%s: Upper checksum: 0x%02x, expected: 0x%02x\n", __func__,
	      csum, buffer[0xdf]);
	if (csum != buffer[0xdf] || !csum_good) {
		printf("Error: SFP EEPROM checksum information is incorrect\n");
		return -1;
	}

	sfp_info->conn_type = buffer[0x80];
	sfp_info->mod_type = buffer[0x82];
	sfp_info->eth_comp = buffer[0x83] & 0xf0;
	sfp_info->cable_comp = buffer[0xa4];

	switch (sfp_info->mod_type) {
	case CVMX_SFP_MOD_COPPER_PIGTAIL:
	case CVMX_SFP_MOD_NO_SEP_CONN:
		debug("%s: copper pigtail or no separable cable\n", __func__);
		/* There are several ways a cable can be marked as active or
		 * passive.  8.[2-3] specify the SFP+ cable technology.  Some
		 * modules also use 3.[0-1] for Infiniband, though it's
		 * redundant.
		 */
		sfp_info->copper_cable = true;
		if ((buffer[0x88] & 0x0C) == 0x08) {
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
		} else if ((buffer[0x88] & 0xC) == 0x4) {
			sfp_info->limiting = false;
			sfp_info->active_cable = false;
		}
		if ((buffer[0x83] & 3) == 2) {
			sfp_info->active_cable = true;
			sfp_info->limiting = true;
		}
		break;
	case CVMX_SFP_MOD_RJ45:
		debug("%s: RJ45 adapter\n", __func__);
		sfp_info->copper_cable = true;
		sfp_info->active_cable = true;
		sfp_info->limiting = true;
		break;
	case CVMX_SFP_MOD_UNKNOWN:
		debug("Unknown module type\n");
		/* The Avago 1000Base-X to 1000Base-T module reports that it
		 * is an unknown module type but the Ethernet compliance code
		 * says it is 1000Base-T.  We'll change the reporting to RJ45.
		 */
		if (buffer[0x86] & 8) {
			sfp_info->mod_type = CVMX_SFP_MOD_RJ45;
			sfp_info->copper_cable = false;
			sfp_info->limiting = true;
			sfp_info->active_cable = true;
			sfp_info->max_copper_cable_len = buffer[0x92];
			sfp_info->rate = CVMX_SFP_RATE_1G;
		}
		/* fall through */
		default:
			sfp_info->limiting = true;
			break;
	}

	if (sfp_info->copper_cable)
		sfp_info->max_copper_cable_len = buffer[0x92];
	else
		sfp_info->max_50um_om4_cable_length = buffer[0x92] * 10;

	debug("%s: copper cable: %d, max copper cable len: %d\n", __func__,
	      sfp_info->copper_cable, sfp_info->max_copper_cable_len);
	if (buffer[0xe])
		sfp_info->max_single_mode_cable_length = buffer[0x8e] * 1000;
	else
		sfp_info->max_single_mode_cable_length = buffer[0x8f] * 100000;

	sfp_info->max_50um_om2_cable_length = buffer[0x90] * 10;
	sfp_info->max_62_5um_om1_cable_length = buffer[0x91] * 10;
	sfp_info->max_50um_om3_cable_length = buffer[0x93] * 10;

	if (buffer[0x8c] == 12) {
		sfp_info->rate = CVMX_SFP_RATE_1G;
	} else if (buffer[0x8c] == 103) {
		sfp_info->rate = CVMX_SFP_RATE_10G;
	} else if (buffer[0x8c] == 0xff) {
		if (buffer[0xc2] == 103)
			sfp_info->rate = CVMX_SFP_RATE_100G;
	}

	if (buffer[0x8c] < 0xff)
		sfp_info->bitrate_max = buffer[0x8c] * 100;
	else
		sfp_info->bitrate_max = buffer[0xc2] * 250;

	if ((buffer[0x88] & 0xc) == 8) {
		if (buffer[0xbc] & 0x4)
			sfp_info->limiting = true;
	}

	/* Currently we only set this for 25G.  FEC is required for CA-S cables
	 * and for cable lengths >= 5M as of this writing.
	 */
#if 0 /* TODO */
	if (((sfp_info->rate == CVMX_SFP_RATE_100G) && (sfp_info->copper_cable)) &&
		((sfp_info->cable_comp == SFP_CABLE_25G_CR_CA_S) ||
		(sfp_info->max_copper_cable_len >= 5)))
		sfp_info->fec_required = true;
#endif
	/* copy strings and vendor info, strings will be automatically NUL
	 * terminated.
	 */
	memcpy(sfp_info->vendor_name, &buffer[0x94], 16);
	memcpy(sfp_info->vendor_oui, &buffer[0xa5], 3);
	memcpy(sfp_info->vendor_pn, &buffer[0xa8], 16);
	memcpy(sfp_info->vendor_rev, &buffer[0xb8], 4);
	memcpy(sfp_info->vendor_sn, &buffer[0xc4], 16);
	memcpy(sfp_info->date_code, &buffer[0xd4], 8);

	sfp_info->linear_rx_output = !!(buffer[0xc0] & 1);
	sfp_info->cooled_laser = !!(buffer[0xc0] & 4);
	sfp_info->internal_cdr = !!(buffer[0xc0] & 8);

	if (buffer[0xc0] & 0x20)
		sfp_info->power_level = 3;
	else
		sfp_info->power_level = (buffer[0xc0] & 2) ? 2 : 1;

	sfp_info->diag_paging = !!(buffer[0xc0] & 0x10);
	sfp_info->los_implemented = !!(buffer[0xc1] & 2);
	sfp_info->los_inverted = !!(buffer[0xc1] & 4);
	sfp_info->tx_fault_implemented = !!(buffer[0xc1] & 8);
	sfp_info->tx_disable_implemented = !!(buffer[0xc1] & 0x10);
	sfp_info->rate_select_implemented = !!(buffer[0xc1] & 0x20);
	sfp_info->tuneable_transmitter = !!(buffer[0xc1] & 0x40);
	sfp_info->rx_decision_threshold_implemented = !!(buffer[0xc1] & 0x80);

	sfp_info->diag_monitoring = !!(buffer[0xdc] & 0x40);
	sfp_info->diag_rx_power_averaged = !!(buffer[0xdc] & 0x8);
	sfp_info->diag_externally_calibrated = !!(buffer[0xdc] & 0x10);
	sfp_info->diag_internally_calibrated = !!(buffer[0xdc] & 0x20);
	sfp_info->diag_addr_change_required = !!(buffer[0xdc] & 0x4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0xdd] & 2);
	sfp_info->diag_app_select_control = !!(buffer[0xdd] & 4);
	sfp_info->diag_soft_rate_select_control = !!(buffer[0xdd] & 8);
	sfp_info->diag_soft_rx_los_implemented = !!(buffer[0xdd] & 0x10);
	sfp_info->diag_soft_tx_fault_implemented = !!(buffer[0xdd] & 0x20);
	sfp_info->diag_soft_tx_disable_implemented = !!(buffer[0xdd] & 0x40);
	sfp_info->diag_alarm_warning_flags_implemented = !!(buffer[0xdd] & 0x80);
	sfp_info->diag_rev = buffer[0xde];

	return 0;
}

/**
 * Reads and parses SFP/QSFP EEPROM
 *
 * @param	sfp	sfp handle to read
 *
 * @return	0 for success, -1 on error.
 */
int cvmx_sfp_read_i2c_eeprom(struct cvmx_fdt_sfp_info *sfp)
{
	uint8_t buffer[256];
	const struct cvmx_fdt_i2c_bus_info *bus = sfp->i2c_bus;
	int err;
	bool is_qsfp;
	int oct_bus = cvmx_fdt_i2c_get_root_bus(bus);

	if (!bus) {
		printf("%s(%s): Error: i2c bus undefined for eeprom\n",
		       __func__, sfp->name);
		return -1;
	}
	is_qsfp = (sfp->sfp_info.conn_type == CVMX_SFP_CONN_QSFP ||
		   sfp->sfp_info.conn_type == CVMX_SFP_CONN_QSFPP ||
		   sfp->sfp_info.conn_type == CVMX_SFP_CONN_QSFP28 ||
		   sfp->sfp_info.conn_type == CVMX_SFP_CONN_MICRO_QSFP) ||
		  sfp->is_qsfp;

	err = cvmx_qsfp_select(sfp, true);
	if (err) {
		printf("%s: Error selecting SFP/QSFP slot\n", __func__);
		return err;
	}

	err = cvmx_fdt_enable_i2c_bus(bus, true);
	if (err) {
		debug("%s: Error enabling i2c bus\n", __func__);
		goto error;
	}

	debug("%s: Reading eeprom from i2c address %d:0x%x\n", __func__,
	      oct_bus, sfp->i2c_eeprom_addr);
#ifdef __U_BOOT__
	i2c_set_bus_num(oct_bus);
	err = i2c_read(sfp->i2c_eeprom_addr, 0, 1, buffer, 256);
	if (err) {
		debug("%s: Error reading eeprom at 0x%x, bus %d\n", __func__,
		      sfp->i2c_eeprom_addr, oct_bus);
		return -1;
	}
#ifdef DEBUG
	print_buffer(0, buffer, 1, 256, 0);
#endif
#else
	if (oct_bus < 0)
		return -1;
	err = cvmx_twsix_read_ia_llc(oct_bus, sfp->i2c_eeprom_addr, 0, 256,
				     1, (uint64_t *)buffer);
	if (err < 0) {
		debug("%s: Error reading eeprom\n", __func__);
		return err;
	}
#ifdef DEBUG
	cvmx_print_buffer_u8(0, buffer, 256);
#endif
#endif
	memset(&sfp->sfp_info, 0, sizeof(struct cvmx_sfp_mod_info));

	switch (buffer[0]) {
	case CVMX_SFP_CONN_SFP:
		err = cvmx_sfp_parse_sfp_buffer(&sfp->sfp_info, buffer);
		break;
	case CVMX_SFP_CONN_QSFP:
	case CVMX_SFP_CONN_QSFPP:
	case CVMX_SFP_CONN_QSFP28:
	case CVMX_SFP_CONN_MICRO_QSFP:
		err = cvmx_sfp_parse_qsfp_buffer(&sfp->sfp_info, buffer);
		break;
	default:
		printf("%s: Unknown SFP transceiver type 0x%x\n", __func__, buffer[0]);
		err = -1;
		break;
	}
error:
	err = cvmx_fdt_enable_i2c_bus(bus, false);
	if (err) {
		debug("%s: Error disabling i2c bus\n", __func__);
		goto error;
	}

	if (is_qsfp)
		err |= cvmx_qsfp_select(sfp, false);

	if (!err) {
		sfp->valid = true;
		sfp->sfp_info.valid = true;
	} else {
		sfp->valid = false;
		sfp->sfp_info.valid = false;
	}

	return err;
}

/**
 * Function called to check and return the status of the mod_abs pin or
 * mod_pres pin for QSFPs.
 *
 * @param	sfp	Handle to SFP information.
 * @param	data	User-defined data passed to the function
 *
 * @return	0 if absent, 1 if present, -1 on error
 */
int cvmx_sfp_check_mod_abs(struct cvmx_fdt_sfp_info *sfp, void *data)
{
	struct cvmx_fdt_gpio_info *mod_abs = sfp->mod_abs;
	int val;
	int err = 0;

	debug("%s(%s, %p) mod_abs: %p\n", __func__, sfp->name, data, mod_abs);
	if (!mod_abs) {
		printf("%s: Error: mod_abs not set for %s\n",
		       __func__, sfp->name);
		return -1;
	}
	val = cvmx_fdt_gpio_get(mod_abs);
	if (val >= 0 && val != sfp->last_mod_abs && sfp->mod_abs_changed)
		err = sfp->mod_abs_changed(sfp, val,
					   sfp->mod_abs_changed_data);
	debug("%s: Last mod_abs: %d, current: %d, changed: %p, rc: %d\n",
	      __func__, sfp->last_mod_abs, val, sfp->mod_abs_changed, err);
	sfp->last_mod_abs = val;

	return err;
}

/**
 * Registers a function to be called to check mod_abs/mod_pres for a SFP/QSFP
 * slot.
 *
 * @param	sfp		Handle to SFP data structure
 * @param	check_mod_abs	Function to be called or NULL to remove
 * @param	mod_abs_data	User-defined data to be passed to check_mod_abs
 *
 * @return	0 for success
 */
int cvmx_sfp_register_check_mod_abs(struct cvmx_fdt_sfp_info *sfp,
	int (*check_mod_abs)(struct cvmx_fdt_sfp_info *sfp, void *data),
	void *mod_abs_data)
{
	struct cvmx_fdt_sfp_info *nsfp = sfp;	/** For walking list */
	do {
		sfp->check_mod_abs = check_mod_abs;
		sfp->mod_abs_data = mod_abs_data;
		nsfp = nsfp->next;
	} while (nsfp);

	return 0;
}

/**
 * Registers a function to be called whenever the mod_abs/mod_pres signal
 * changes.
 *
 * @param	sfp		Handle to SFP data structure
 * @param	mod_abs_changed	Function called whenever mod_abs is changed
 *				or NULL to remove.
 * @param	mod_abs_changed_data	User-defined data passed to
 *					mod_abs_changed
 *
 * @return	0 for success
 *
 * @NOTE: If multiple SFP slots are linked together, all subsequent slots
 *	  will also be registered for the same handler.
 */
int cvmx_sfp_register_mod_abs_changed(struct cvmx_fdt_sfp_info *sfp,
	int (*mod_abs_changed)(struct cvmx_fdt_sfp_info *sfp, int val,
			       void *data),
	void *mod_abs_changed_data)
{
	sfp->mod_abs_changed = mod_abs_changed;
	sfp->mod_abs_changed_data = mod_abs_changed_data;

	sfp->last_mod_abs = -2;

	return 0;
}

/**
 * Registers a function to be called whenever rx_los changes
 *
 * @param	sfp		Handle to SFP data structure
 * @param	rx_los_changed	Function to be called when rx_los changes
 *				or NULL to remove the function
 * @param	rx_los_changed_data	User-defined data passed to
 *					rx_los_changed
 *
 * @return	0 for success
 */
int cvmx_sfp_register_rx_los_changed(struct cvmx_fdt_sfp_info *sfp,
	int (*rx_los_changed)(struct cvmx_fdt_sfp_info *sfp, int val,
			      void *data),
	void *rx_los_changed_data)
{
	sfp->rx_los_changed = rx_los_changed;
	sfp->rx_los_changed_data = rx_los_changed_data;
	sfp->last_rx_los = -2;

	return 0;
}

/**
 * Parses a SFP slot from the device tree
 *
 * @param	sfp		SFP handle to store data in
 * @param	fdt_addr	Address of flat device tree
 * @param	of_offset	Node in device tree for SFP slot
 *
 * @return	0 on success, -1 on error
 */
static int cvmx_sfp_parse_sfp(struct cvmx_fdt_sfp_info *sfp,
			      void *fdt_addr, int of_offset)
{
	int eeprom_node;
	int diag_node;
	int err = 0;

	sfp->name = fdt_get_name(fdt_addr, of_offset, NULL);
	sfp->of_offset = of_offset;
	sfp->tx_disable = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
							 of_offset,
							 "tx_disable");
	sfp->mod_abs = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
						      of_offset,
						      "mod_abs");
	sfp->tx_error = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
						       "tx_error");
	sfp->rx_los = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
						     "rx_los");
	eeprom_node = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "eeprom");
	if (eeprom_node > 0) {
		sfp->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr, eeprom_node);
		sfp->i2c_eeprom_addr = cvmx_fdt_get_int(fdt_addr, eeprom_node,
							"reg", 0x50);
	}
	diag_node = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "diag");
	if (diag_node > 0) {
		sfp->i2c_diag_addr = cvmx_fdt_get_int(fdt_addr, diag_node,
						      "reg", 0x51);
		if (!sfp->i2c_bus)
			sfp->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr,
							    diag_node);
	} else {
		sfp->i2c_diag_addr = 0x51;
	}
	sfp->last_mod_abs = -2;
	sfp->last_rx_los = -2;

	if (!sfp->i2c_bus) {
		printf("%s(%s): Error: could not get i2c bus from device tree\n",
		       __func__, sfp->name);
		err = -1;
	}

	if (err) {
		if (sfp->tx_disable)
			cvm_sfp_free(sfp->tx_disable, sizeof(*sfp->tx_disable));
		if (sfp->mod_abs)
			cvm_sfp_free(sfp->mod_abs, sizeof(*sfp->mod_abs));
		if (sfp->tx_error)
			cvm_sfp_free(sfp->tx_error, sizeof(*sfp->tx_error));
		if (sfp->rx_los)
			cvm_sfp_free(sfp->rx_los, sizeof(sfp->rx_los));
		sfp->tx_disable = NULL;
		sfp->mod_abs = NULL;
		sfp->tx_error = NULL;
		sfp->rx_los = NULL;
	} else {
		sfp->valid = true;
	}
	return err;
}

/**
 * Parses a SFP slot from the device tree
 *
 * @param	sfp		SFP handle to store data in
 * @param	fdt_addr	Address of flat device tree
 * @param	of_offset	Node in device tree for SFP slot
 *
 * @return	0 on success, -1 on error
 */
static int cvmx_sfp_parse_qsfp(struct cvmx_fdt_sfp_info *sfp,
			       void *fdt_addr, int of_offset)
{
	int eeprom_node;
	int diag_node;
	int err = 0;

	sfp->name = fdt_get_name(fdt_addr, of_offset, NULL);
	sfp->of_offset = of_offset;
	sfp->lp_mode = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
						      of_offset,
						      "lp_mode");
	sfp->mod_abs = cvmx_fdt_gpio_get_info_phandle(fdt_addr,
						      of_offset,
						      "mod_prs");
	sfp->select = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
						     "select");
	sfp->reset = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
						    "reset");
	sfp->interrupt = cvmx_fdt_gpio_get_info_phandle(fdt_addr, of_offset,
							"interrupt");
	eeprom_node = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "eeprom");
	if (eeprom_node > 0) {
		sfp->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr, eeprom_node);
		sfp->i2c_eeprom_addr = cvmx_fdt_get_int(fdt_addr, eeprom_node,
							"reg", 0x50);
	}
	diag_node = cvmx_fdt_lookup_phandle(fdt_addr, of_offset, "diag");
	if (diag_node > 0) {
		sfp->i2c_diag_addr = cvmx_fdt_get_int(fdt_addr, diag_node,
						      "reg", 0x51);
		if (!sfp->i2c_bus)
			sfp->i2c_bus = cvmx_fdt_get_i2c_bus(fdt_addr,
							    diag_node);
	} else {
		sfp->i2c_diag_addr = 0x51;
	}
	sfp->last_mod_abs = -2;
	sfp->last_rx_los = -2;

	if (!sfp->i2c_bus) {
		printf("%s(%s): Error: could not get i2c bus from device tree\n",
		       __func__, sfp->name);
		err = -1;
	}

	if (err) {
		if (sfp->lp_mode)
			cvm_sfp_free(sfp->lp_mode, sizeof(*sfp->lp_mode));
		if (sfp->mod_abs)
			cvm_sfp_free(sfp->mod_abs, sizeof(*sfp->mod_abs));
		if (sfp->select)
			cvm_sfp_free(sfp->select, sizeof(*sfp->select));
		if (sfp->reset)
			cvm_sfp_free(sfp->reset, sizeof(*sfp->reset));
		if (sfp->interrupt)
			cvm_sfp_free(sfp->interrupt, sizeof(*sfp->interrupt));
		sfp->lp_mode = NULL;
		sfp->mod_abs = NULL;
		sfp->select = NULL;
		sfp->reset = NULL;
		sfp->interrupt = NULL;
	} else {
		sfp->valid = true;
	}

	return err;
}

/**
 * Parses the device tree for SFP and QSFP slots
 *
 * @param	fdt_addr	Address of flat device-tree
 *
 * @return	0 for success, -1 on error
 */
int cvmx_sfp_parse_device_tree(void *fdt_addr)
{
	int of_offset = -1;
	struct cvmx_fdt_sfp_info *sfp, *last_sfp = NULL;
	int err = 0;
	int reg;
	static bool parsed = false;

	if (parsed) {
		debug("%s(%p): Already parsed\n", __func__, fdt_addr);
		return 0;
	}

	do {
		of_offset = fdt_node_offset_by_compatible(fdt_addr, of_offset,
							  "ethernet,sfp-slot");
		if (of_offset < 0)
			break;

		sfp = cvm_sfp_alloc(sizeof(*sfp));
		if (!sfp)
			return -1;

		err = cvmx_sfp_parse_sfp(sfp, fdt_addr, of_offset);
		if (!err) {
			if (!sfp_list)
				sfp_list = sfp;
			if (last_sfp)
				last_sfp->next = sfp;
			sfp->prev = last_sfp;
			last_sfp = sfp;
		}
	} while (!err);

	if (err)
		return err;

	of_offset = -1;
	do {
		of_offset = fdt_node_offset_by_compatible(fdt_addr, of_offset,
							  "ethernet,qsfp-slot");
		if (of_offset < 0)
			break;
		sfp = cvm_sfp_alloc(sizeof(*sfp));
		if (!sfp)
			return -1;

		err = cvmx_sfp_parse_qsfp(sfp, fdt_addr, of_offset);
		if (!err) {
			if (!sfp_list)
				sfp_list = sfp;
			if (last_sfp)
				last_sfp->next = sfp;
			sfp->prev = last_sfp;
			last_sfp = sfp;
		}
	} while (!err);

	if (err) {
		printf("%s: Error parsing QSFP or SFP slots in device tree\n",
		       __func__);
		return err;
	}

	if (octeon_has_feature(OCTEON_FEATURE_BGX)) {
		of_offset = -1;
		err = -1;
		do {
			int sfp_node;
			int bgx_node;
			uint64_t reg_addr;
			struct cvmx_xiface xi;
			int xiface;
			cvmx_helper_interface_mode_t mode;
			int i;

			of_offset = fdt_node_offset_by_compatible(fdt_addr,
						of_offset,
						"cavium,octeon-7890-bgx-port");
			if (of_offset < 0)
				break;
			sfp_node = cvmx_fdt_lookup_phandle(fdt_addr, of_offset,
							   "sfp-slot");
			if (sfp_node < 0)
				sfp_node = cvmx_fdt_lookup_phandle(fdt_addr,
								   of_offset,
								   "qsfp-slot");
			/* If no SFP or QSFP slot found, go to next port */
			if (sfp_node < 0)
				continue;

			sfp = cvmx_sfp_find_slot_by_fdt_node(sfp_node);
			if (!sfp) {
				printf("%s: Error: could not find SFP slot for BGX port %s\n",
				       __func__,
				       fdt_get_name(fdt_addr, sfp_node, NULL));
				err = -1;
				break;
			}
			/* Get the port index */
			reg = cvmx_fdt_get_int(fdt_addr, of_offset, "reg", -1);
			if (reg < 0) {
				printf("%s: Error: could not get BGX port reg value\n",
				       __func__);
				err = -1;
				break;
			}
			/* Now get the interface number */
			bgx_node = fdt_parent_offset(fdt_addr, of_offset);
			if (bgx_node < 0) {
				err = -1;
				break;
			}
			/* Interface address */
			reg_addr = cvmx_fdt_get_addr(fdt_addr, bgx_node, "reg");
			if (reg_addr == FDT_ADDR_T_NONE) {
				cvmx_dprintf("%s: Device tree BGX node %s has invalid address\n",
					     __func__,
					     fdt_get_name(fdt_addr, bgx_node,
							  NULL));
				err = -1;
				break;
			}
			/* Translate to get node */
			reg_addr = cvmx_fdt_translate_address(fdt_addr,
							      bgx_node,
							      (uint32_t *)&reg_addr);
			/* Extrace node */
			xi.node = cvmx_csr_addr_to_node(reg_addr);
			/* Extract reg address */
			reg_addr = cvmx_csr_addr_strip_node(reg_addr);
			if ((reg_addr & 0xFFFFFFFFF0000000) != 0x00011800E0000000) {
				cvmx_dprintf("%s: Invalid BGX address 0x%llx\n",
					     __func__, (unsigned long long)reg_addr);
				xi.node = -1;
				err = -1;
				break;
			}
			/* Extract interface from address */
			xi.interface = (reg_addr >> 24) & 0x0F;
			/* Convert to xiface */
			xiface = cvmx_helper_node_interface_to_xiface(xi.node,
								      xi.interface);
			/* Convert to IPD port */
			sfp->ipd_port[0] = cvmx_helper_get_ipd_port(xiface, reg);
			mode = cvmx_helper_interface_get_mode(xiface);
			if (mode == CVMX_HELPER_INTERFACE_MODE_XLAUI ||
			    mode == CVMX_HELPER_INTERFACE_MODE_40G_KR4)
				for (i = 1; i < 4; i++)
					sfp->ipd_port[i] = -1;
			else
				for (i = 1; i < 4; i++)
					sfp->ipd_port[i] =
						cvmx_helper_get_ipd_port(xiface, i);
		} while (!err);
	}
	if (!err)
		parsed = true;
	return err;
}

/**
 * Given an IPD port number find the corresponding SFP or QSFP slot
 *
 * @param	ipd_port	IPD port number to search for
 *
 * @return	pointer to SFP data structure or NULL if not found
 */
struct cvmx_fdt_sfp_info *cvmx_sfp_find_slot_by_port(int ipd_port)
{
	struct cvmx_fdt_sfp_info *sfp = sfp_list;
	int i;

	while (sfp) {
		for (i = 0; i < 4; i++)
			if (sfp->ipd_port[i] == ipd_port)
				return sfp;
		sfp = sfp->next;
	}
	return NULL;
}

/**
 * Given a fdt node offset find the corresponding SFP or QSFP slot
 *
 * @param	of_offset	flat device tree node offset
 *
 * @return	pointer to SFP data structure or NULL if not found
 */
struct cvmx_fdt_sfp_info *cvmx_sfp_find_slot_by_fdt_node(int of_offset)
{
	struct cvmx_fdt_sfp_info *sfp = sfp_list;

	while (sfp) {
		if (sfp->of_offset == of_offset)
			return sfp;
		sfp = sfp->next;
	}
	return NULL;
}
