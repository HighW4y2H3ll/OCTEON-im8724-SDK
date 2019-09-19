/* AAPL CORE Revision: 2.3.1-beta */

/* Copyright 2014-2015 Avago Technologies. All rights reserved.
 *
 * This file is part of the AAPL CORE library.
 *
 * AAPL CORE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AAPL CORE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AAPL CORE.  If not, see http://www.gnu.org/licenses.
 */


/** Doxygen File Header
 * * @file
 ** @brief Core AAPL functions.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_asic_info.h"
#include "avago_serdes.h"
#include "avago_serdes_pmd.h"
#include "avago_spico.h"
#include "avago_logging.h"
#include "avago_sbus.h"
#include "avago_avsp.h"
#include "avago_str_conv.h"

/**
 * Initialize and construct the Avago handle.
 *
 * @param	i2c_bus		i2c bus number of Avago device
 * @param	i2c_base_addr	i2c address of Avago device
 *
 * @return	Pointer to avago handle or NULL if out of memory.
 */
struct avago_hdl *avago_construct(int i2c_bus, int i2c_base_addr)
{
	int i;
	struct avago_hdl *ah = (struct avago_hdl *)malloc(sizeof(*ah));

	debug("%s(0x%x, 0x%x)\n", __func__, i2c_bus, i2c_base_addr);

	if (!ah) {
		printf("%s: Out of memory\n", __func__);
		return NULL;
	}

	memset(ah, 0, sizeof(*ah));

	for (i = 0; i < AVAGO_MAX_CHIPS; i++)
		ah->chip_name[i] = ah->chip_rev[i] = "";

	ah->debug = 0;
	ah->chips = 0;
	ah->sbus_rings = 0;
	ah->serdes_int_timeout = AVAGO_SERDES_INT_TIMEOUT;
	ah->sbus_mdio_timeout = AVAGO_SBUS_MDIO_TIMEOUT;
	ah->i2c_base_addr = i2c_base_addr;
	ah->i2c_bus = i2c_bus;

	return ah;
}

/**
 * Returns the return code and sets it to zero.
 *
 * @param	ah	Avago handle
 *
 * @return	The current value of the Avago return code.  Negative numbers
 *		indicate an error.
 *
 * @warning	Calling this function clears the return code.
 */
int avago_get_return_code(struct avago_hdl *ah)
{
	int x = ah->return_code;
	ah->return_code = 0;
	return x;
}

/**
 * Get number of chips on the current ring
 *
 * @param[in]	ah	Avago handle
 *
 * @return Number of chips on the current ring
 */
uint32_t avago_get_chips(const struct avago_hdl *ah)
{
	return ah->chips;
}

/**
 * Return the maximum sbus address for the given chip and ring
 *
 * @param[in]	ah	Avago handle
 * @param	addr	address containing chip and ring
 *
 * @return	maximum sbus address or 0 if invalid address
 */
uint32_t avago_get_max_sbus_addr(const struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);

	if (addr_struct.chip >= AVAGO_MAX_CHIPS ||
		addr_struct.ring >= AVAGO_MAX_RINGS)
		return 0;
	return ah->max_sbus_addr[addr_struct.chip][addr_struct.ring];
}

/**
 * Get the JTAG idcode of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	JTAG idcode of the current sbus ring (specified in the
 *		SBus address)
 */
uint32_t avago_get_jtag_idcode(const struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		addr_struct.chip = 0;

	return ah->jtag_idcode[addr_struct.chip];
}

/**
 * Get the name of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Chip address
 *
 * @return	pointer to character string with the name of the chip
 */
const char *avago_get_chip_name(const struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		return "unknown";

	return ah->chip_name[addr_struct.chip];
}

/**
 * Get the revision of the specified chip
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Chip address
 *
 * @return	pointer to character string with the chip revision
 */
const char *avago_get_chip_rev_str(const struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		return "";

	return ah->chip_rev[addr_struct.chip];
}

/**
 * Gets the Logical Sub-Block (LSB) revision number of the device.
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The Logical Sub-Block (LSB) revision number of the addressed
 *		device.
 */
uint32_t avago_get_lsb_rev(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);

	if (!avago_address_range_check(&addr_struct))
		return 0;

	if (avago_check_broadcast_address(ah, addr, __func__, __LINE__, false))
		return ah->lsb_rev[0][0][addr_struct.sbus];

	if (ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] == 0) {
		uint8_t lsb_rev = avago_serdes_get_lsb_rev(ah, addr);
		ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = lsb_rev;
		if (lsb_rev > 0) {
			int baddr = 0;
			enum avago_ip_type ip_type = avago_get_ip_type(ah, addr);

			switch (ip_type) {
			case AVAGO_SERDES:
				baddr = AVAGO_SERDES_D6_BROADCAST;
				break;
			case AVAGO_M4:
				baddr = AVAGO_SERDES_M4_BROADCAST;
				break;
			case AVAGO_P1:
				baddr= AVAGO_SERDES_P1_BROADCAST;
				break;
			default:
				break;
			}
			if (baddr > 0 &&
			    (ah->lsb_rev[0][0][baddr] == 0 ||
			     ah->lsb_rev[0][0][baddr] > lsb_rev))
				ah->lsb_rev[0][0][baddr] = lsb_rev;
		}
		avago_log_printf(ah, AVAGO_DEBUG6, __func__, __LINE__,
				 "Setting aapl->lsb_rev for %s to 0x%04x\n",
				 avago_addr_to_str(0xff),
				 ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)]);
	}
	return ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)];
}

/**
 * Return the IP revision number
 */
static int ip_rev(struct avago_hdl *ah, uint32_t sbus_addr)
{
	enum avago_ip_type ip_type = avago_get_ip_type(ah, sbus_addr);
	enum avago_process_id proc_id;

	if (!avago_check_process(ah, sbus_addr, __func__, __LINE__, true, 3,
				 AVAGO_PROCESS_B, AVAGO_PROCESS_F, AVAGO_PROCESS_A))
		return -1;

	switch (ip_type) {
	case AVAGO_SERDES:
	case AVAGO_P1:
	case AVAGO_M4:
		proc_id = avago_get_process_id(ah, sbus_addr);
		if (proc_id == AVAGO_PROCESS_F) {
			int ret_data = avago_sbus_rd(ah, sbus_addr, 0xfe);
			if (0 == ret_data)
				ret_data = avago_serdes_mem_rd(ah, sbus_addr,
							       AVAGO_ESB,
							       0x0303);
			return (ret_data >> 8) & 0xff;
		} else if (proc_id == AVAGO_PROCESS_B) {
			int ret_data = avago_sbus_rd(ah, sbus_addr, 0xfe);
			return ret_data;
		}
		return avago_sbus_rd(ah, sbus_addr, 0xfe) & 0xff;
	case AVAGO_SPICO:
		return (avago_sbus_rd(ah, sbus_addr, 0x0f) >> 12) & 0xf;
	case AVAGO_AVSP_CONTROL_LOGIC:
	case AVAGO_PCS64B66B:
	case AVAGO_PCS64B66B_FEC:
	case AVAGO_SAPPH_GBX:
		return (avago_sbus_rd(ah, sbus_addr, 0) >> 5) & 7;
	case AVAGO_PCIE_PCS:
		return (avago_sbus_rd(ah, sbus_addr, 0) >> 1) & 0xf;
	case AVAGO_CORE_PLL:
		return (avago_sbus_rd(ah, sbus_addr, 0) >> 28) & 0xf;
	case AVAGO_MLD:
	case AVAGO_RSFEC_BRIDGE:
	case AVAGO_OPAL_CONTROL:
	case AVAGO_PAD_CONTROL:
		return (avago_sbus_rd(ah, sbus_addr, 0xfc) >> 12) & 0xf;
	case AVAGO_OPAL_RSFEC528:
	case AVAGO_OPAL_RSFEC528_544:
	case AVAGO_OPAL_HOST_ALIGNER:
	case AVAGO_OPAL_MOD_ALIGNER:
	case AVAGO_GARNET_25GE_INTERFACE:
		return avago_sbus_rd(ah, sbus_addr, 0xfc) & 0xf;
	case AVAGO_SBUS_CONTROLLER:
		return avago_sbus_rd(ah, sbus_addr, 0xfe);
	case AVAGO_AUTO_NEGOTIATION:
		return avago_sbus_rd(ah, sbus_addr, 0x23) >> 1;
	default:
		break;
	}
	return -1;
}

static bool verify_addr_and_ip_type(struct avago_hdl *ah, uint32_t addr,
				    struct avago_addr *addr_struct)
{
	avago_addr_to_struct(addr, addr_struct);

	if (!avago_address_range_check(addr_struct) ||
	    addr_struct->chip == AVAGO_BROADCAST ||
	    addr_struct->ring == AVAGO_BROADCAST)
		return false;
	if (addr_struct->sbus == AVAGO_BROADCAST)
		addr_struct->sbus = 255;
	if (ah->ip_type[AVAGO_3D_ARRAY_ADDR(*addr_struct)] == 0xff)
		avago_set_ip_type(ah, addr);
	return true;
}

/**
 * Gets the revision number of the SBus device for the specified chip and
 * SBus ring
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return	The revision number of the SBus device for the current chip and
 *		SBus ring (specified by the SBus address)
 */
uint32_t avago_get_ip_rev(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	if (!verify_addr_and_ip_type(ah, addr, &addr_struct))
		return 0;

	if (ah->ip_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] == 0xfffe)
		ah->ip_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] =
						ip_rev(ah, addr) & 0xffff;

	return ah->ip_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)];
}

/**
 * Returns the firmware rev from Avago's cache
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return		Firmware rev from the cache
 */
int avago_get_firmware_rev(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	if (!verify_addr_and_ip_type(ah, addr, &addr_struct))
		return 0;

	if (!ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] )
		ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] =
					avago_firmware_get_rev(ah, addr);

	return ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)];
}

/**
 * Returns the firmware build from Avago's cache
 *
 * @param[in,out]	ah	Avago handle
 * @param		addr	Address number
 *
 * @return	The firmware build from the cache
 */
int avago_get_firmware_build(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	if (!verify_addr_and_ip_type(ah, addr, &addr_struct))
		return 0;

	if (!ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] )
		ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] =
				avago_firmware_get_build_id(ah, addr);

	return ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)];
}

/**
 * Sets the client data pointer
 *
 * @param[in,out]	ah		Avago handle
 * @param[in]		client_data	Pointer to client data
 */
void avago_bind_set(struct avago_hdl *ah, const void *client_data)
{
	ah->client_data = (void *)client_data;
}

/**
 * Gets the client data pointer
 *
 * @param[in]	ah	Avago handle
 *
 * @return	Pointer to client data
 */
void *avago_bind_get(const struct avago_hdl *ah)
{
	return ah->client_data;
}

/**
 * Gets the IP type for the SBus address
 *
 * @param	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The IP type of the SBus device for the current chip and SBus
 *		ring as specified by the SBus address.
 */
enum avago_ip_type avago_get_ip_type(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;
	enum avago_ip_type type;
	avago_addr_to_struct(addr, &addr_struct);

	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		addr_struct.chip = 0;
	if (addr_struct.ring >= AVAGO_MAX_RINGS)
		addr_struct.ring = 0;
	if (addr_struct.sbus > 255)
		addr_struct.sbus = 255;
	if (ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)] == 0xff)
		avago_set_ip_type(ah, addr);
	type = (enum avago_ip_type)ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)];
	return type;
}

static int avago_find_chip_index(uint32_t jtag_idcode, uint32_t jtag_mask)
{
	int at;

	for (at = 0; at < ARRAY_SIZE(avago_chip_id); at++) {
		if (jtag_idcode == (avago_chip_id[at].jtag_idcode & jtag_mask))
			return at;
	}
	return -1;
}

static enum avago_process_id get_process_id(struct avago_hdl *ah, uint32_t chip)
{
	int rc = ah->return_code;
	int id;
	enum avago_process_id ret = AVAGO_UNKNOWN_PROCESS;
	uint32_t data = avago_sbus_rd(ah, avago_make_addr3(chip, 0, 0xfe), 0xfe);

	if (ah->return_code == rc) {
		switch (data) {
		case 0xbb:
			ret = AVAGO_PROCESS_D;
			break;
		case 0xbc:
			ret = AVAGO_PROCESS_E;
			break;
		default:
			printf("WARNING: (%s:%d) Unknown process id: 0x%x\n",
			       __func__, __LINE__, data);
		case 0xbd:
			id = avago_find_chip_index(ah->jtag_idcode[chip],
						   0xffffffff);
			if (id >= 0)
				ret = avago_chip_id[id].process_id;
			if (ret == AVAGO_UNKNOWN_PROCESS && data == 0xbd)
				ret = AVAGO_PROCESS_F;
			break;
		case 0xbe:
		case 0xbf:
		case 0xc1:
			ret = AVAGO_PROCESS_F;
			break;
		case 0xc2:
		case 0xc0:
			ret = AVAGO_PROCESS_B;
			break;
		}
	}
	return ret;
}

/**
 * Gets the chip process ID
 *
 * @param[in]	ah	Avago handle
 * @param	addr	Address number
 *
 * @return	The process ID of the specified device.
 */
enum avago_process_id avago_get_process_id(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);

	if (addr_struct.chip == AVAGO_BROADCAST) {
		enum avago_process_id process = AVAGO_UNKNOWN_PROCESS;
		for (addr_struct.chip = 0;
		     addr_struct.chip < avago_get_chips(ah);
		     addr_struct.chip++) {
			enum avago_process_id curr_process =

				avago_get_process_id(ah,
						     avago_struct_to_addr(&addr_struct));
			if (addr_struct.chip > 0 && curr_process != process)
				return AVAGO_UNKNOWN_PROCESS;
			process = curr_process;
		}
		return process;
	}
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		return AVAGO_UNKNOWN_PROCESS;

	if (ah->process_id[addr_struct.chip] == AVAGO_UNKNOWN_PROCESS)
		ah->process_id[addr_struct.chip] = get_process_id(ah,
								  addr_struct.chip);
	return ah->process_id[addr_struct.chip];
}

/**
 * Returns if SPICO is running or not
 *
 * @param	ah	Avago handle
 * @param	addr	device address
 *
 * @return	true if SPICO is running, false otherwise
 */
bool avago_get_spico_running_flag(struct avago_hdl *ah, uint32_t addr)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);
	if (avago_check_broadcast_address(ah, addr, __func__, __LINE__, false)) {
		bool st;
		struct avago_addr start, stop, next;
		bool running = true;

		for (st = avago_broadcast_first(ah, &addr_struct, &start, &stop,
						&next, AVAGO_BROADCAST_IGNORE_LANE);
		     st;
		     st = avago_broadcast_next(ah, &next, &start, &stop,
					       AVAGO_BROADCAST_IGNORE_LANE)) {
			uint32_t sbus_addr = avago_struct_to_addr(&next);
			if (avago_get_ip_type(ah, sbus_addr) != AVAGO_SERDES)
				continue;
			running &= ah->spico_running[next.chip][next.ring][next.sbus];
		}
		return running;
	} else {
		if (addr_struct.chip >= AVAGO_MAX_CHIPS ||
		    addr_struct.ring >= AVAGO_MAX_RINGS ||
		    addr_struct.sbus > 255)
			return false;
		return ah->spico_running[addr_struct.chip][addr_struct.ring][addr_struct.sbus];
	}
}

/**
 * Sets if SPICO is running or not
 *
 * @param	ah	Avago handle
 * @param	addr	Address of device
 * @param	running	Set true if running, false if not running
 *
 * @return	running flag or false if address out of range.
 */
bool avago_set_spico_running_flag(struct avago_hdl *ah, uint32_t addr,
				  bool running)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);

	if (!avago_address_range_check(&addr_struct)) {
		printf("%s: Error: address %s out of range\n", __func__,
		       avago_addr_to_str(addr));
		return false;
	}
	return (ah->spico_running[addr_struct.chip][addr_struct.ring][addr_struct.sbus] = running);
}

/** @brief  Get a string representation of the Process ID of the specified chip
 **         and sbus ring
 ** @return The Process ID string of the sbus device for the current chip and
 **         sbus ring (specified by the sbus address)
 **/
const char *avago_get_process_id_str(struct avago_hdl *ah, uint32_t addr)
{
	return avago_process_id_to_str(avago_get_process_id(ah, addr));
}

/**
 * Checks sbus_addr to see if it is a broadcast address (including the SerDes
 * broadcast address).
 *
 * @param[in]	ah		Avago handle
 * @param	sbus_addr	SBus address to Check
 * @param[in]	caller		Calling function name
 * @param	line		Line number of caller
 * @param	error_on_match	True to print error on match.
 *
 * @return	true if sbus_addr belongs to any broadcast address
 *		(chip/SBus/SerDes).
 */
bool avago_check_broadcast_address(struct avago_hdl *ah, uint32_t sbus_addr,
				   const char *caller, int line,
				   bool error_on_match)
{
	bool match;
	struct avago_addr addr_struct;

	avago_addr_to_struct(sbus_addr, &addr_struct);

	match = (addr_struct.chip == AVAGO_BROADCAST) ||
	(addr_struct.ring == AVAGO_BROADCAST) ||
	(addr_struct.sbus == AVAGO_BROADCAST) ||
	(addr_struct.sbus > AVAGO_MAX_RING_ADDRESS &&
	addr_struct.sbus < 0xfc);

	if (match && error_on_match)
		avago_fail(ah, caller, line, "Broadcast address (0x%02x) not supported.\n",
			   sbus_addr);

	return match;
}

/**
 * @brief	Checks the sbus_addr against the firmware revisions given in
 *		the arg list.
 *
 * Checks if the device firmware revision is >= what is in the list.  If the
 * revision has bit 19 set (0x80000), it must match exactly (not >=) after
 * ignoring bit 19.
 *
 * @param[in]	ah		Avago Handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Calling function name
 * @param	line		Caller's line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	Number of firmware revision arguments which
 *				follow.
 * @param	...		List of valid firmware revisions
 *
 * @return	true if the firmware revision matches one of those listed,
 *		false if not.
 */
bool avago_check_firmware_rev(struct avago_hdl *ah, uint32_t sbus_addr,
			      const char *caller, int line,
			      bool error_on_no_match, int arg_count, ...)
{
	bool match = false;
	int firmware_rev;
	int i;
	struct avago_addr addr_struct;
	va_list firm_rev_type;
	avago_addr_to_struct(sbus_addr, &addr_struct);

	if (addr_struct.chip == AVAGO_BROADCAST)
		addr_struct.chip = 0;
	if (addr_struct.ring == AVAGO_BROADCAST)
		addr_struct.ring = 0;
	if (addr_struct.sbus == AVAGO_BROADCAST)
		addr_struct.sbus = 0xff;
	sbus_addr = avago_struct_to_addr(&addr_struct);
	firmware_rev = avago_get_firmware_rev(ah, sbus_addr) & 0xfff;
	if (firmware_rev == 0)
		return false;

	va_start(firm_rev_type, arg_count);
	for (i = 0; i < arg_count; i++) {
		int firm_rev_check = va_arg(firm_rev_type, int);

		if ( (!(firm_rev_check & 0x80000) &&
		      (firmware_rev >= (firm_rev_check & 0x0fff))) ||
		     (firmware_rev == (firm_rev_check & 0x0fff)) ) {
			match = true;
			break;
		}
	}

	va_end(firm_rev_type);

	if (!match && error_on_no_match) {
		printf("ERROR: (%s:%d) Unsupported firmware rev 0x%04X on SBus address 0x%x.\n",
		       caller, line, avago_get_firmware_rev(ah, sbus_addr), sbus_addr);
	}

	return match;
}

/**
 * @brief	Checks sbus_addr against firmware build given in args list.
 *
 * @details	Checks if the device firmware build exactly matches an entry in
 *		the list.
 *
 * @param[in]	ah		Avago Handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Calling function name
 * @param	line		Caller's line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	Number of firmware build arguments which
 *				follow.
 * @param	...		List of valid firmware builds
 *
 * @return	true if the firmware build matches one of those listed,
 *		false if not.
 */
bool avago_check_firmware_build(struct avago_hdl *ah, uint32_t sbus_addr,
				const char *caller, int line,
				bool error_on_no_match, int arg_count, ...)
{
	bool match = false;
	int i;
	struct avago_addr addr_struct;
	va_list firm_build_type;
	int firmware_build;

	avago_addr_to_struct(sbus_addr, &addr_struct);
	if (addr_struct.chip == AVAGO_BROADCAST)
		addr_struct.chip = 0;
	if (addr_struct.ring == AVAGO_BROADCAST)
		addr_struct.ring = 0;
	if (addr_struct.sbus == AVAGO_BROADCAST)
		addr_struct.sbus = 0xff;
	sbus_addr = avago_struct_to_addr(&addr_struct);

	firmware_build = avago_get_firmware_build(ah, sbus_addr);

	if (firmware_build == 0)
		return false;

	va_start(firm_build_type, arg_count);
	for (i = 0; i < arg_count; i++) {
		int firm_build_check = va_arg(firm_build_type, int);

		if (firmware_build == firm_build_check) {
			match = true;
			break;
		}
	}

	va_end(firm_build_type);

	if (!match && error_on_no_match) {
		printf("ERROR: SBus address 0x%x is running firmware build 0x%4X, which is not supported by %s:%d.\n",
		       sbus_addr, avago_get_firmware_build(ah, sbus_addr),
		       caller, line);
	}

	return match;
}

/**
 * @brief	Checks sbus_addr against list of supported IP types.
 *
 * @details	Args is the number of arguments passed in for ip_type or process,
 *		for example: avago_check_ip_type(ah, sbus_addr, __func__,
 *						 __LINE__, true, 2, AVAGO_SERDES,
 *						 AVAGO_QPI);
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus slice address
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid IP types
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_ip_type(struct avago_hdl *ah, uint32_t sbus_addr,
			 const char *caller, int line, bool error_on_no_match,
			 int arg_count, ...)
{
	bool match = false;
	int i;
	struct avago_addr addr_struct;
	va_list ip_types;

	va_start(ip_types, arg_count);

	avago_addr_to_struct(sbus_addr, &addr_struct);
	for (i = 0; i < arg_count; i++) {
		enum avago_ip_type ip_type =
				(enum avago_ip_type)va_arg(ip_types, int);
		if ((uint32_t)ip_type == addr_struct.sbus &&
		    ip_type > AVAGO_MAX_RING_ADDRESS) {
			match = true;
			break;
		}
		if (addr_struct.chip < AVAGO_MAX_CHIPS &&
		    addr_struct.ring < AVAGO_MAX_RINGS &&
		    addr_struct.sbus < 255 &&
		    avago_get_ip_type(ah, sbus_addr) == ip_type) {
			match = true;
			break;
		}
	}
	va_end(ip_types);

	if (!match && error_on_no_match)
		avago_fail(ah, caller, line,
			   "SBus %s, of IP type 0x%x, in process %s, is not supported by %s.\n",
			   avago_addr_to_str(sbus_addr),
			   avago_get_ip_type(ah, sbus_addr),
			   avago_get_process_id_str(ah, sbus_addr),
			   caller);
	return match;
}

/**
 * Checks sbus_addr against list of supported IP types.
 *
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus address to provide chip and ring to search
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid IP types
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_ip_type_exists(struct avago_hdl *ah, uint32_t sbus_addr,
				const char *caller, int line,
				bool error_on_no_match, int arg_count, ...)
{
	bool match = true;
	int i;
	struct avago_addr addr_struct;
	va_list ip_types;
	va_start(ip_types, arg_count);

	avago_addr_to_struct(sbus_addr, &addr_struct);
	for (i = 0; i < arg_count; i++) {
		enum avago_ip_type ip_type =
				(enum avago_ip_type)va_arg(ip_types, int);
		struct avago_addr start, stop, next;
		bool st;

		for (st = avago_broadcast_first(ah, &addr_struct, &start,
						&stop, &next, 0);
		     st;
		     st = avago_broadcast_next(ah, &next, &start, &stop, 0)) {
			if (avago_get_ip_type(ah, avago_struct_to_addr(&next))
						== ip_type)
				break;
		}

		if (!st) {
			match = false;
			break;
		}
	}
	va_end(ip_types);

	if (!match && error_on_no_match)
		printf("ERROR: (%s:%d): SBus 0x%x, of IP type 0x%x is not supported.\n",
		       caller, line, sbus_addr, avago_get_ip_type(ah, sbus_addr));

	return match;
}

/**
 * Checks sbus_addr against list of supported process types.
 *
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus address to provide chip and ring to search
 * @param[in]	caller		Caller function name
 * @param	line		Caller line number
 * @param	error_on_no_match	Set to true to print error on no match
 * @param	arg_count	The number of ip_type arguments which follow
 * @param	...		List of valid processes
 *
 * @return	true if the device is one of the listed types, false if not.
 */
bool avago_check_process(struct avago_hdl *ah, uint32_t sbus_addr,
			 const char *caller, int line,
			 bool error_on_no_match, int arg_count, ...)
{
	bool match = false;
	int i;
	va_list processes;
	struct avago_addr addr_struct;
	enum avago_process_id process_id;

	avago_addr_to_struct(sbus_addr, &addr_struct);

	if (addr_struct.chip == AVAGO_BROADCAST)
		addr_struct.chip = 0;
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		return 0;

	sbus_addr = avago_struct_to_addr(&addr_struct);
	process_id = avago_get_process_id(ah, sbus_addr);

	va_start(processes, arg_count);

	for (i = 0; i < arg_count; i++)
		if (process_id == (enum avago_process_id)va_arg(processes, int)) {
			match = true;
			break;
		}
	va_end(processes);

	if (!match && error_on_no_match)
		printf("ERROR: (%s:%d): SBus 0x%x, IP type 0x%x is not supported.\n",
		       caller, line, sbus_addr, avago_get_ip_type(ah, sbus_addr));
	return match;
}

/**
 * Reset the chip
 *
 * @details	Performs a MDIO reset (if using MDIO)
 *
 * @param	ah	Avago handle
 * @param	reset	If set, send the "reset" command to the chip
 * @param	chip	The chip number
 */
void avago_system_chip_setup(struct avago_hdl *ah, bool reset, int chip)
{
	ah->curr_chip = chip;

	debug("%s(%p, %s, %d)\n", __func__, ah, reset ? "true" : "false", chip);
	AVAGO_SUPPRESS_ERRORS_PUSH(ah);
	avago_diag_sbus_rw_test(ah, avago_make_addr3(chip, 0, 0xfe), 1);
	AVAGO_SUPPRESS_ERRORS_POP(ah);

	if (reset)
		avago_sbus_reset(ah, avago_make_addr3(chip, 0, AVAGO_BROADCAST),
				 1);
}

/**
 * Returns an avago_cmp_rev type the comparator design of the SerDes.
 *
 * @param	ah	Avago handle
 * @param	addr	Device address
 *
 * @return	avago_cmp_rev enumation type the comparitor design of the SerDes
 *		If unrecognized, logs an error message and decrements
 *		ah->return_code and returns AVAGO_SDREV_UNKNOWN.
 */
int avago_get_sdrev(struct avago_hdl *ah, uint32_t addr)
{
	/* Recursion warning:
	 * The _rev functions call avago_serdes_mem_rd, which then calls this
	 * function, resulting in recursion issues.
	 * The current solution is to detect this recursion and return
	 * an incomplete value which is "good enough" for avago_serdes_mem_rd.
	 * As new chips come on board, this solution needs to be revamped.
	 */
	static bool recursing = false;
	enum avago_ip_type ip_type = avago_get_ip_type(ah, addr);

	if (ip_type == AVAGO_P1)
		return AVAGO_SDREV_P1;
	if (ip_type == AVAGO_M4) {
		uint32_t ip_rev;

		if (recursing)
			return AVAGO_M4;

		recursing = true;
		ip_rev = avago_get_ip_rev(ah, addr);
		recursing = false;

		return (ip_rev == 0xe4) ? AVAGO_SDREV_CM4 : AVAGO_SDREV_OM4;
	}
	if (avago_get_process_id(ah, addr) == AVAGO_PROCESS_B)
		return AVAGO_SDREV_16;
	if (ip_type == AVAGO_SERDES) {
		uint32_t lsb_rev;
		if (recursing)
			return AVAGO_SDREV_D6;
		recursing = true;
		lsb_rev = avago_get_lsb_rev(ah, addr);
		recursing = false;

		return (lsb_rev == 8) ? AVAGO_SDREV_HVD6 : AVAGO_SDREV_D6;
	}
	printf("ERROR: (%s:%d): Addr: 0x%x, Unknown SerDes design.\n",
	       __func__, __LINE__, addr);
	return AVAGO_SDREV_UNKNOWN;
}

/** Return revision number of the given interrupt. */
int avago_get_interrupt_rev(struct avago_hdl *ah, uint32_t addr, int int_num)
{
	int sdrev;
	switch (int_num) {
	case 0x03:
	case 0x25:
		sdrev = avago_get_sdrev(ah, addr);
		switch (sdrev) {
		case AVAGO_SDREV_CM4:
		case AVAGO_SDREV_CM4_16:
			if (avago_check_firmware_rev(ah, addr, 0, 0, false,
						     1, 0x1056))
				return 2;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return 1;
}

static void get_chip_name(struct avago_hdl *ah, int chip)
{
	uint32_t ring;
	int at;
	uint32_t addr_80;
	uint32_t cntl_addr = avago_make_addr3(chip, 0, 0xfe);

	addr_80 = avago_sbus_rd(ah, cntl_addr, 0x80);
	ah->jtag_idcode[chip] = 0;

	AVAGO_SUPPRESS_ERRORS_PUSH(ah);
	if ((addr_80 & 0x3) == 0x00) {
		ah->jtag_idcode[chip] =
			(avago_sbus_rd(ah, cntl_addr, 0x81) & 0xff) << 12;
		if ((avago_find_chip_index(ah->jtag_idcode[chip], 0xff << 12) >= 0)
		    && ah->jtag_idcode[chip] != 0) {
			avago_sbus_rmw(ah, cntl_addr, 0x80, 0x1, 0x3);
			ah->jtag_idcode[chip] |=
			    (avago_sbus_rd(ah, cntl_addr, 0x81) & 0xff) << 20;
			avago_sbus_rmw(ah, cntl_addr, 0x80, 0x2, 0x3);
			ah->jtag_idcode[chip] |=
			    (avago_sbus_rd(ah, cntl_addr, 0x81) & 0xff)	<< 28;
			avago_sbus_rmw(ah, cntl_addr, 0x80, 0, 0x3);
			ah->jtag_idcode[chip] |= 0x57f;
		} else {
			ah->jtag_idcode[chip] = 0;
		}
		avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
				 "IDCODE read from SBus for device %d: 0x%08x\n",
				 chip, ah->jtag_idcode[chip]);
	}
	AVAGO_SUPPRESS_ERRORS_POP(ah);
	for (ring = 0; ring < AVAGO_MAX_RINGS; ring++)
		ah->max_sbus_addr[chip][ring] = 0;

	if (ah->sbus_rings) {
		debug("%s: Setting rings 0..%d max sbus address to 1\n",
		      __func__, ah->sbus_rings);
		for (ring = 0; ring < ah->sbus_rings; ring++)
			ah->max_sbus_addr[chip][ring] = 1;
	}

	at = avago_find_chip_index(ah->jtag_idcode[chip], 0xffffffff);
	if (at >= 0) {
		ah->chip_name[chip] = avago_chip_id[at].name;
		ah->chip_rev[chip] = avago_chip_id[at].rev;
		ah->process_id[chip] = avago_chip_id[at].process_id;

		if (!ah->sbus_rings) {
			ah->sbus_rings = avago_chip_id[at].sbus_rings;
			for (ring = 0; ring < avago_chip_id[at].sbus_rings;
			     ring++)
				ah->max_sbus_addr[chip][avago_chip_id[at].sbus_ring_start + ring] = 1;
		}
		avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
				 "IDCODE 0x%08x: %s %s\n", ah->jtag_idcode[chip],
				 ah->chip_name[chip], ah->chip_rev[chip]);
		return;
	}

	if (!ah->sbus_rings) {
		ah->sbus_rings = 1;
		ah->max_sbus_addr[chip][0] = 1;
		debug("%s: Set sbus_rings to 1 and max addr to 1\n", __func__);
	}
	ah->chip_name[chip] = "";
	ah->chip_rev[chip] = "";
	avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
			 "IDCODE 0x%08x: %s %s\n", ah->jtag_idcode[chip],
			 ah->chip_name[chip], ah->chip_rev[chip]);
}

/**
 * Fills out the avago handle struct with information about all devices it can
 * communicate with.
 *
 * @param	ah		Avago handle
 * @param	chip_reset	Set true to reset the chip
 */
void avago_init_ip_info(struct avago_hdl *ah, bool chip_reset)
{
	int chip, chips;
	enum avago_process_id process_id;
	chips = 1;
	for (chip = 0; chip < chips; chip++) {
		int ring;
		debug("%s: get chip name\n", __func__);
		get_chip_name(ah, chip);
		debug("%s: System chip setup\n", __func__);
		avago_system_chip_setup(ah, chip_reset, chip);
		debug("%s: Getting process ID\n", __func__);
		AVAGO_SUPPRESS_ERRORS_PUSH(ah);
		process_id = get_process_id(ah, chip);
		AVAGO_SUPPRESS_ERRORS_POP(ah);
		if (ah->process_id[chip] == AVAGO_UNKNOWN_PROCESS)
			ah->process_id[chip] = process_id;
		else if (process_id != ah->process_id[chip])
			avago_log_printf(ah, AVAGO_WARNING, __func__, __LINE__,
					 "Chip %d has inconsistent process id information: Ids as both %s and %s.\n",
					 chip,
					 avago_process_id_to_str(ah->process_id[chip]),
					 avago_process_id_to_str(process_id));
		for (ring = 0; ring < AVAGO_MAX_RINGS; ring++) {
			debug("%s: Initializing ring %d\n", __func__, ring);

			if (0 != ah->max_sbus_addr[chip][ring]) {
				uint32_t data;
				int rc = ah->return_code;
				struct avago_addr addr_struct;

				addr_struct.chip = chip;
				addr_struct.ring = ring;
				addr_struct.sbus = AVAGO_SBUS_CONTROLLER_ADDRESS;
				addr_struct.lane = AVAGO_BROADCAST;

				debug("%s: Reading SBus\n", __func__);
				AVAGO_SUPPRESS_ERRORS_PUSH(ah);
				data = avago_sbus_rd(ah,
						     avago_struct_to_addr(&addr_struct),
						     0x2);
				AVAGO_SUPPRESS_ERRORS_POP(ah);
				debug("%s: data: 0x%x\n", __func__, data);
				ah->max_sbus_addr[chip][ring] =
					(ah->return_code < rc || data == 0)
					? 0 : data - 1;
				avago_log_printf(ah, AVAGO_DEBUG2, __func__,
						 __LINE__,
						 "Chip %d (%s %s), ring %d, SBus devices: %d\n",
						 chip, ah->chip_name[chip],
						 ah->chip_rev[chip], ring,
						 ah->max_sbus_addr[chip][ring]);
				debug("%s: max_sbus_addr: 0x%x\n", __func__,
				      ah->max_sbus_addr[chip][ring]);
				if (ah->max_sbus_addr[chip][ring] >= 0xfe)
					ah->max_sbus_addr[chip][ring] = 0;

				if (ah->max_sbus_addr[chip][ring] == 0) {
					avago_log_printf(ah, AVAGO_WARNING,
							 __func__, __LINE__,
							 "Error occurred while trying to determine the number of devices on chip %d, SBus ring %d.  ah->return_code: %d, 0xfe 0x02 0x02 0x00: %d\n",
							 chip, ring, rc, data);
					break;
				}
				debug("%s: Initializing ip_type and spico_running\n", __func__);
				for (addr_struct.sbus = 1;
				     addr_struct.sbus <= ah->max_sbus_addr[chip][ring];
				     addr_struct.sbus++) {
					ah->ip_type[chip][ring][addr_struct.sbus] = 0xff;
					ah->spico_running[chip][ring][addr_struct.sbus] = 0;
				}
				debug("%s: Initializing ip_type and spico_running 2\n", __func__);
				for (addr_struct.sbus = AVAGO_MAX_RING_ADDRESS+1;
				     addr_struct.sbus <= 0xff;
				     addr_struct.sbus++) {
					ah->firm_rev[chip][ring][addr_struct.sbus] = 0;
					ah->firm_build[chip][ring][addr_struct.sbus] = 0;
					ah->ip_rev[chip][ring][addr_struct.sbus] = 0xfffe;
					ah->spico_running[chip][ring][addr_struct.sbus] = 0;
				}
				debug("%s: Initializing broadcast stuff, addr_struct.sbus: 0x%x\n",
				      __func__, addr_struct.sbus);

				ah->ip_type[chip][ring][AVAGO_SERDES_P1_BROADCAST] =
					AVAGO_SERDES_P1_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_SERDES_M4_BROADCAST] =
					AVAGO_SERDES_M4_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_THERMAL_SENSOR_BROADCAST] =
					AVAGO_THERMAL_SENSOR_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_DDR_STOP_BROADCAST        ] =
					AVAGO_DDR_STOP_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_DDR_CTC_BROADCAST         ] =
					AVAGO_DDR_CTC_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_DDR_TRAIN_BROADCAST       ] =
					AVAGO_DDR_TRAIN_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_DDR_DATA_BROADCAST        ] =
					AVAGO_DDR_DATA_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_DDR_ADDRESS_BROADCAST     ] =
					AVAGO_DDR_ADDRESS_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_PMRO_BROADCAST            ] =
					AVAGO_PMRO_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_RESERVED_BROADCAST        ] =
					AVAGO_RESERVED_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_PCIE_PCS_BROADCAST        ] =
					AVAGO_PCIE_PCS_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_PCS64B66B_BROADCAST       ] =
					AVAGO_PCS64B66B_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_AUTO_NEGOTIATION_BROADCAST] =
					AVAGO_AUTO_NEGOTIATION_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_FBD_BROADCAST             ] =
					AVAGO_FBD_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_QPI_BROADCAST             ] =
					AVAGO_QPI_BROADCAST;
				ah->ip_type[chip][ring][AVAGO_SERDES_BROADCAST          ] =
					AVAGO_SERDES_BROADCAST;
				ah->ip_type[chip][ring][0xfd] = 0xff;

				addr_struct.sbus = AVAGO_SBUS_CONTROLLER_ADDRESS;
				debug("%s: Setting IP type, addr: %s\n",
				      __func__,
				      avago_addr_to_str(avago_struct_to_addr(&addr_struct)));
				avago_set_ip_type(ah,
						  avago_struct_to_addr(&addr_struct));
				debug("%s: Waiting for twi to complete\n", __func__);
				avago_twi_wait_for_complete(ah,
							    avago_struct_to_addr(&addr_struct));
			}
		}
	}
	avago_log_printf(ah, AVAGO_DEBUG2, __func__, __LINE__,
			 "End of init_ip_info\n");
}

/**
 * Sets the ip_type, ip_rev, spico_running, firm_rev and lsb_rev fields in the
 * avago handle.
 *
 * @param	ah		Avago handle
 * @param	sbus_addr	SBus slice address
 */
void avago_set_ip_type(struct avago_hdl *ah, uint32_t sbus_addr)
{
	uint32_t data;
	uint32_t jtag_idcode;
	struct avago_addr addr_struct;

	if (avago_check_broadcast_address(ah, sbus_addr, __func__,
					  __LINE__, false))
		return;

	avago_addr_to_struct(sbus_addr, &addr_struct);
	if (addr_struct.chip >= AVAGO_MAX_CHIPS)
		addr_struct.chip = 0;
	if (addr_struct.ring >= AVAGO_MAX_RINGS)
		addr_struct.ring = 0;
	if (addr_struct.sbus > 255)
		addr_struct.sbus = 255;

	ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)]       = AVAGO_UNKNOWN_IP;
	ah->ip_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)]        = 0xfffe;
	ah->spico_running[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0;
	ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)]      = 0;
	ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)]    = 0;
	ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)]       = 0;

	data = avago_sbus_rd(ah, sbus_addr, 0xff);
	jtag_idcode = ah->jtag_idcode[addr_struct.chip];

	debug("%s(%p, %s): data: 0x%x, jtag_idcode: 0x%x\n", __func__, ah,
	      avago_addr_to_str(sbus_addr), data, jtag_idcode);

	switch (data) {
	case 0x00:
		break;

	case 0x01:
		if (jtag_idcode == 0x0964257f &&   /* AVSP-5410 */
		    addr_struct.sbus == 0x04)      /* SerDes not pinned out */
			data = AVAGO_UNKNOWN_IP;
		break;
	case 0x08:
		if (jtag_idcode == 0x14211001 &&
		    (addr_struct.sbus == 0x05 || addr_struct.sbus == 0x06))
			data = AVAGO_MLD;
		break;

	case 0x0b:
		if (jtag_idcode == 0x14211001 && addr_struct.sbus == 0x1c)
			data = AVAGO_RAM_PMRO;
		break;

	case 0x11:
		if ((jtag_idcode == 0x1911357f && addr_struct.sbus < 0x17) ||
		    (jtag_idcode == 0x090b957f && addr_struct.sbus < 0x16) ||
		    (jtag_idcode == 0x0911457f && addr_struct.sbus < 0x16))
			data = AVAGO_AVSP_CONTROL_LOGIC;
		break;

	case 0x83:
		if (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_B)
			data = AVAGO_SLE;
		if (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_F)
			data = AVAGO_RAM_PMRO;
		break;

	case 0xc1:
		if (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_F)
			data = AVAGO_PANDORA_LSB;
		break;
	}

	if (data <= AVAGO_IP_TYPE_MAX ||
	    (data >= AVAGO_IP_TYPE_ALT_RANGE_LO &&
	     data <= AVAGO_IP_TYPE_ALT_RANGE_HI)) {
		ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)] =
						(enum avago_ip_type) data;
	} else {
		ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)] = AVAGO_UNKNOWN_IP;
		printf("WARNING: (%s:%d): IP type 0x%02x unrecognized for SBus address 0x%x.\n",
		       __func__, __LINE__, data, sbus_addr);
	}

	switch (ah->ip_type[AVAGO_3D_ARRAY_ADDR(addr_struct)]) {
	case AVAGO_P1:
	case AVAGO_M4:
	case AVAGO_SERDES:
		switch (avago_get_process_id(ah, sbus_addr)) {
		case AVAGO_PROCESS_B:
		case AVAGO_PROCESS_F:
			avago_spico_wait_for_upload(ah, sbus_addr);
			avago_sbus_wr(ah, sbus_addr, 0x01, 0x20000000);
			break;
		case AVAGO_PROCESS_A:
		default:
			ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
			ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
			break;
		}
		break;
	case AVAGO_SPICO:
		switch (avago_get_process_id(ah, sbus_addr)) {
		case AVAGO_PROCESS_B:
		case AVAGO_PROCESS_F:
			avago_spico_wait_for_upload(ah, sbus_addr);
			ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xff;
			break;
		case AVAGO_PROCESS_A:
			ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xff;
			break;
		default:
			ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
			ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
			ah->ip_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
			ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xff;
			break;
		}
		break;
	case AVAGO_SBUS_CONTROLLER:
		ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
		ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
		ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xff;
		break;
	default:
		ah->spico_running[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0;
		ah->firm_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
		ah->firm_build[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xffff;
		ah->lsb_rev[AVAGO_3D_ARRAY_ADDR(addr_struct)] = 0xff;
	}
}

/**
 * @brief Print information on the device(s) AAPL is connected to using the
 *        AVAGO_INFO log type (which is typically STDOUT).
 * @details The addr field selects the device to print info on.
 *          As ah->verbose levels increase, more information is printed
 *          The type input will print info only for the IP type specified.
 *          Provides additional options as compared with avago_device_info.
 * @return void
 */
void avago_device_info_options(struct avago_hdl *ah,
			       struct avago_addr *addr_struct,
			       enum avago_ip_type type,
			       struct avago_state_table_options *options)
{
	char buf[640];
	char *ptr = buf;
	const char __attribute__((unused)) const *end = &buf[sizeof(buf) - 1];
	bool st;
	struct avago_addr start, stop, next;
	uint prev_chip = ~0;
	uint name_len = 0;
	const char **name_list = 0;
	struct avago_addr addr2_struct = *addr_struct;
	int firm_rev;
	int firm_build;
	int lsb_rev;

	avago_log_printf(ah, AVAGO_INFO, 0, 0, "Avago Device Info:\n");

	for (st = avago_broadcast_first(ah, &addr2_struct, &start, &stop, &next,
					AVAGO_BROADCAST_IGNORE_LANE);
	     st;
	     st = avago_broadcast_next(ah, &next, &start, &stop,
				       AVAGO_BROADCAST_IGNORE_LANE)) {
		if( next.chip != prev_chip) {
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "\nInformation for chip %d:\n",
					 next.chip);

			ptr = buf;
			ptr += snprintf(buf, end - buf, "  JTAG ID: 0x%08x",
					ah->jtag_idcode[next.chip]);
			if (*ah->chip_name[next.chip])
				ptr += snprintf(ptr, end - ptr,
						";  %s rev %s",
						ah->chip_name[next.chip],
						ah->chip_rev[next.chip]);
			else
				ptr += snprintf(ptr, end - ptr, " (%s)",
					       avago_hex_2_bin(ah->data_char,
							       ah->jtag_idcode[next.chip],
							        0, 32));
			avago_log_printf(ah, AVAGO_INFO, 0, 0, "%s;  %s\n",
					 buf,
					 avago_process_id_to_str(ah->process_id[next.chip]));

			avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");
			buf[0] = '\0';
			ptr = buf;
			if (ah->debug >= 4)
				ptr += snprintf(buf, end - buf, "IP-ID ");

			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "%s  Addr              IP Type  Rev  LSB Firmware    On Miscellaneous Info\n",
					 buf);
			buf[0] = '\0';
			ptr = buf;
			if (ah->debug >= 4)
				ptr += snprintf(buf, end - buf, "----- ");
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "%s------ -------------------- ---- ---- ----------- -- ----------------------\n",
					 buf);

			name_len = 0;
			name_list = 0;

			if (0 == strncmp(ah->chip_name[next.chip], "AVSP-", 5))
				avsp_get_name_list(ah, next.chip,
						   ah->chip_name[next.chip],
						   &name_len, &name_list);
		}
		prev_chip = next.chip;

		if (0 == ah->max_sbus_addr[next.chip][next.ring])
			continue;

		if ((int)next.sbus <= ah->max_sbus_addr[next.chip][next.ring] + 2 ||
		    next.sbus == 0xfd || next.sbus == 0xfe) {
			struct avago_addr addr_struct2;
			uint32_t sbus_addr;
			enum avago_ip_type ip_type;
			int ip_rev;
			ptr = buf;

			avago_addr_init(&addr_struct2);
			addr_struct2.chip = next.chip;
			addr_struct2.ring = next.ring;
			addr_struct2.sbus = next.sbus;
			addr_struct2.lane = next.lane;
			if ((int)next.sbus == ah->max_sbus_addr[next.chip][next.ring] + 1)
				addr_struct2.sbus = 0xfd;
			if ((int)next.sbus == ah->max_sbus_addr[next.chip][next.ring] + 2)
				addr_struct2.sbus = 0xfe;
			sbus_addr = avago_struct_to_addr(&addr_struct2);

			ip_type = avago_get_ip_type(ah, sbus_addr);
			if (type && type != ip_type)
				continue;

			ip_rev = avago_get_ip_rev(ah, sbus_addr);

			if (ah->debug >= 4)
				ptr += sprintf(ptr,"0x%02x: ",
					       avago_sbus_rd(ah, sbus_addr, 0xff));
			if (next.sbus < name_len && name_list &&
			    name_list[next.sbus]) {
				char ip_type_buf[100];
				sprintf(ip_type_buf, "%s %s", avago_ip_type_to_str(ip_type), name_list[next.sbus]);
				ptr += sprintf(ptr, "%6s %20s", avago_addr_to_str(sbus_addr), ip_type_buf);
			} else {
				ptr += sprintf(ptr, "%6s %20s", avago_addr_to_str(sbus_addr), avago_ip_type_to_str(ip_type));
			}

			firm_rev   = avago_get_firmware_rev(ah, sbus_addr);
			firm_build = avago_get_firmware_build(ah, sbus_addr);
			lsb_rev    = avago_get_lsb_rev(ah, sbus_addr);

			if (ip_rev != 0xffff)
				ptr += sprintf(ptr, " 0x%02x", ip_rev);
			else
				ptr += sprintf(ptr, " %4s","    ");

			if (lsb_rev != 0xff)
				ptr += sprintf(ptr, " 0x%02x", lsb_rev);
			else
				ptr += sprintf(ptr, " %4s","    ");

			if (firm_rev != 0xffff)
				ptr += sprintf(ptr, " 0x%04X_%04X  %d", firm_rev, firm_build, avago_get_spico_running_flag(ah, sbus_addr));
			else
				ptr += sprintf(ptr, " %11s   ", "");

			while (ptr[-1] == ' ' )
				*--ptr = '\0';  /* Strip trailing spaces */

			avago_log_printf(ah, AVAGO_INFO, 0, 0, "%-s\n", buf);
		}
	}

	avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");
#if 0
	avago_serdes_print_state_table_options(ah, addr_struct, options);
#endif

}
