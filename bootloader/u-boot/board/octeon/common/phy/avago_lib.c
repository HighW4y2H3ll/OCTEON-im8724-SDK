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
 ** @brief Internal library functions.
 **/

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_logging.h"
#include "avago_sbus.h"

#define AVAGO_ADDR_INVALID_LANE (0xbad)

/**
 * Initialize avago_addr structure to default values.
 *
 * @param[in]	structure to initialize
 */
void avago_addr_init(struct avago_addr *addr_struct)
{
	addr_struct->chip = 0;
	addr_struct->ring = 0;
	addr_struct->sbus = 0;
	addr_struct->lane = AVAGO_ADDR_IGNORE_LANE;
	addr_struct->next_addr = NULL;
}

/**
 * Converts an address integer into an address structure
 *
 * @param	addr		Address to convert
 * @param[out]	addr_struct	Converted address
 *
 * @return	false if address is not a recognized value
 */
bool avago_addr_to_struct(uint32_t addr, struct avago_addr *addr_struct)
{
	int lane_field = (addr >> 16) & 0x0f;

	avago_addr_init(addr_struct);
	addr_struct->chip = (addr >> 12) & 0x0f;
	addr_struct->ring = (addr >> 8) & 0x0f;
	addr_struct->sbus = (addr & 0xff);
	addr_struct->lane = (addr >> 16) & 0x0f;
	if (addr_struct->chip == 0x0f)
		addr_struct->chip = AVAGO_ADDR_BROADCAST;
	if (addr_struct->ring == 0x0f)
		addr_struct->ring = AVAGO_ADDR_BROADCAST;
	if (addr_struct->sbus == 0xff)
		addr_struct->sbus = AVAGO_ADDR_BROADCAST;
	if (lane_field > 7)
		addr_struct->lane = lane_field - 8;
	else if (lane_field == 0)
		addr_struct->lane = AVAGO_ADDR_IGNORE_LANE;
	else if (lane_field == 1)
		addr_struct->lane = AVAGO_ADDR_QUAD_LOW;
	else if (lane_field == 2)
		addr_struct->lane = AVAGO_ADDR_QUAD_HIGH;
	else if (lane_field == 3)
		addr_struct->lane = AVAGO_ADDR_QUAD_ALL;
	else if (lane_field == 4)
		addr_struct->lane = AVAGO_ADDR_BROADCAST;
	else
		addr_struct->lane = AVAGO_ADDR_INVALID_LANE;

	return (addr & 0xfff00000) == 0;
}

/**
 * Converts hardware address structure into an address integer
 *
 * @param[in]	Hardware address structure
 *
 * @return	The address value representation of the hardware address.
 */
uint32_t avago_struct_to_addr(const struct avago_addr *addr_struct)
{
	uint32_t addr, lane;
	switch (addr_struct->lane) {
	case AVAGO_ADDR_IGNORE_LANE:
		lane = 0;
		break;
	case AVAGO_ADDR_QUAD_LOW:
		lane = 1;
		break;
	case AVAGO_ADDR_QUAD_HIGH:
		lane = 2;
		break;
	case AVAGO_ADDR_QUAD_ALL:
		lane = 3;
		break;
	case AVAGO_ADDR_BROADCAST:
		lane = 4;
		break;
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		lane = 8 + addr_struct->lane;
		break;
	default:
		lane = 7;
		break;
	}
	addr =	(addr_struct->chip & 0x0f) << 12 |
		(addr_struct->ring & 0x0f) <<  8 |
		(addr_struct->sbus & 0xff) <<  0 |
		(lane)                     << 16;
	return addr;
}

/**
 * Combine the chip, ring and sbus values into an address integer
 *
 * @param	chip	chip number, range [0..15]
 * @param	ring	ring number, range [0..15]
 * @param	sbus	SerDes number, range [0..255]
 *
 * @return	Combined address
 */
uint32_t avago_make_addr3(int chip, int ring, int sbus)
{
	struct avago_addr addr_struct;

	avago_addr_init(&addr_struct);
	addr_struct.chip = chip;
	addr_struct.ring = ring;
	addr_struct.sbus = sbus;
	return avago_struct_to_addr(&addr_struct);
}

/**
 * Combine the chip, ring, sbus and lane values into an address integer
 *
 * @param	chip	chip number, range [0..15]
 * @param	ring	ring number, range [0..15]
 * @param	sbus	SerDes number, range [0..255]
 * @param	lane	SerDes lane number, range [0..7,
 *						   AVAGO_ADDR_QUAD_LOW,
 *						   AVAGO_ADDR_QUAD_HIGH,
 *						   AVAGO_ADDR_QUAD_ALL,
 *						   AVAGO_ADDR_BROADCAST,
 *						   AVAGO_ADDR_IGNORE_LANE ]
 *
 * @return	Combined address
 */
uint32_t avago_make_addr4(int chip, int ring, int sbus, int lane)
{
	struct avago_addr addr_struct;

	avago_addr_init(&addr_struct);
	addr_struct.chip = chip;
	addr_struct.ring = ring;
	addr_struct.sbus = sbus;
	addr_struct.lane = lane;
	return avago_struct_to_addr(&addr_struct);
}

/**
 * Sets the sbus portion of an input address to the new value and sets the lane
 * to be ignored.
 *
 * @param	addr	Chip/ring/sbus address
 * @param	sbus	new sbus value
 *
 * @return	modified addr value with new sbus value.
 */
uint32_t avago_make_sbus_addr(uint32_t addr, int sbus)
{
	struct avago_addr addr_struct;

	avago_addr_to_struct(addr, &addr_struct);
	addr_struct.sbus = sbus;
	addr_struct.lane = AVAGO_ADDR_IGNORE_LANE;
	return avago_struct_to_addr(&addr_struct);
}

/**
 * @brief	Modifies the addr_struct passed in to be all broadcast addresses
 *		(chip, ring, SBus, etc)
 *
 * @param[out]	addr_struct	The modified broadcast addr_struct
 *
 * @return The address integer, and modifies the addr_struct passed in.
 */
uint32_t avago_addr_init_broadcast(struct avago_addr *addr_struct)
{
	avago_addr_init(addr_struct);
	addr_struct->chip = AVAGO_ADDR_BROADCAST;
	addr_struct->ring = AVAGO_ADDR_BROADCAST;
	addr_struct->sbus = AVAGO_ADDR_BROADCAST;
	addr_struct->lane = AVAGO_ADDR_BROADCAST;
	return avago_struct_to_addr(addr_struct);
}

/**
 * @brief  Returns the SBus Master address for the addressed chip and ring.
 *
 * @param	addr	chip/ring address
 *
 * @return The SBus Master address corresponding to given input address.
 */
uint32_t avago_make_sbus_master_addr(uint32_t addr)
{
	return avago_make_sbus_addr(addr, AVAGO_SBUS_MASTER_ADDRESS);
}

/**
 * @brief	Returns the SBus Controller address for the addressed chip and
 *		ring.
 *
 * @param	addr	Chip/ring address
 *
 * @return The SBus Controller address corresponding to given input address.
 */
uint32_t avago_make_sbus_controller_addr(uint32_t addr)
{
	return avago_make_sbus_addr(addr, AVAGO_SBUS_CONTROLLER_ADDRESS);
}

/**
 * @brief	Returns the SerDes Broadcast address for the addressed chip and ring.
 *
 * @param	addr	Chip/ring address
 *
 * @return The SerDes Broadcast address for the addressed chip and ring.
 */
uint32_t avago_make_serdes_broadcast_addr(uint32_t addr)
{
	return avago_make_sbus_addr(addr, AVAGO_SERDES_BROADCAST_ADDRESS);
}

/**
 * @brief	Modifies the lane portion of an input address.
 *
 * @param	addr	Chip/ring/sbus address
 * @param	lane	New lane address
 *
 * @return The input address modified to reference the given lane.
 */
uint32_t avago_make_sbus_lane_addr(uint32_t addr, int lane)
{
	struct avago_addr addr_struct;
	avago_addr_to_struct(addr, &addr_struct);
	addr_struct.lane = lane;
	return avago_struct_to_addr(&addr_struct);
}

static const int AVAGO_BROADCAST_FIRST = 0x8000;

/**
 * @brief	Begins traversal of addresses selected by sbus_addr.
 * @details	start, stop and first are updated based on any
 *		chip, ring or sbus broadcast addresses.
 *		start and stop should be passed unchanged to avago_broadcast_next(),
 *		and first should be passed as the addr field.
 *		Flags can be the logical OR of several values:
 *		+ AVAGO_BROADCAST_IGNORE_LANE - causes all returned lane values
 *		  to be AVAGO_ADDR_IGNORE_LANE.
 *		  Use when lanes are not applicable.
 *		+ AVAGO_BROADCAST_NO_ITER_LANE - causes addr lane value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  lane range.
 *		+ AVAGO_BROADCAST_LANE - lane value AVAGO_ADDR_IGNORE_LANE is
 *		  treated as AVAGO_ADDR_BROADCAST.
 *		  Other values of lane are passed through unchanged.
 *		  Use to address all SerDes even when broadcast wasn't requested.
 *		+ AVAGO_BROADCAST_NO_ITER_SBUS - causes addr sbus value to be
 *		  passed through unchanged.
 *		  Use to send broadcast requests rather than iterate over the
 *		  sbus range.
 *
 * @param[in]	ah	Avago handle
 * @param[in]	addr	SBus slice (broadcast) address.
 * @param[out]	start	Start of hardware address range
 * @param[out]	end	End of hardware address range
 * @param[out]	first	First hardware address
 * @param	flags	Flags to control the iteration
 *
 * @return	Returns TRUE and first is initialized to the first address.
 */
bool avago_broadcast_first(struct avago_hdl *ah, struct avago_addr *addr,
			  struct avago_addr *start, struct avago_addr *stop,
			  struct avago_addr *first, uint32_t flags)
{
	bool is_broadcast_sbus = addr->sbus > AVAGO_MAX_RING_ADDRESS &&
				 addr->sbus != AVAGO_SPICO_BROADCAST &&
				 addr->sbus != AVAGO_SBUS_CONTROLLER_ADDRESS;
	bool iter_on_sbus = !(flags & AVAGO_BROADCAST_NO_ITER_SBUS) &&
			    is_broadcast_sbus;
	avago_addr_init(first);
	avago_addr_init(stop);
	avago_addr_init(start);
	start->next_addr = addr->next_addr;
	first->chip = start->chip = addr->chip != AVAGO_BROADCAST ? addr->chip : 0;
	first->ring = start->ring = addr->ring != AVAGO_BROADCAST ? addr->ring : 0;
	first->sbus = start->sbus = iter_on_sbus ? 1 : addr->sbus;
	stop->chip  = addr->chip != AVAGO_BROADCAST ?
			addr->chip : (ah->chips > 0) ? (ah->chips - 1) : 0;
	stop->ring  = addr->ring != AVAGO_BROADCAST ? addr->ring :
			(ah->sbus_rings > 0) ? ah->sbus_rings - 1 : 0;
	if (is_broadcast_sbus) {
		uint32_t c, r;
		stop->sbus = 0;
		for (c = start->chip; c <= stop->chip; c++)
			for (r = start->ring; r <= stop->ring; r++) {
				if (stop->sbus < ah->max_sbus_addr[c][r] + 2U)
					stop->sbus = ah->max_sbus_addr[c][r] + 2U;
			}
	} else {
		stop->sbus = addr->sbus;
	}
	first->lane = start->lane = stop->lane = addr->lane;
	if (flags & AVAGO_BROADCAST_IGNORE_LANE)
		first->lane = start->lane = stop->lane = AVAGO_ADDR_IGNORE_LANE;

	else if (!(flags & AVAGO_BROADCAST_NO_ITER_LANE)) {
		if (addr->lane == AVAGO_ADDR_BROADCAST ||
		    addr->lane == AVAGO_ADDR_QUAD_ALL  ||
		    ((flags & AVAGO_BROADCAST_LANE) &&
		     addr->lane == AVAGO_ADDR_IGNORE_LANE)) {
			first->lane = start->lane = 0;
			stop->lane = 7;
		} else if (addr->lane == AVAGO_ADDR_QUAD_HIGH) {
			first->lane = start->lane = 4;
			stop->lane = 7;
		} else if (addr->lane == AVAGO_ADDR_QUAD_LOW) {
			first->lane = start->lane = 0;
			stop->lane = 3;
		}
	} else if ((flags & AVAGO_BROADCAST_LANE) &&
		   (addr->lane == AVAGO_ADDR_IGNORE_LANE)) {
		first->lane = start->lane = stop->lane = AVAGO_ADDR_QUAD_ALL;
	}

	debug("%s: start: %s, stop: %s\n", __func__,
	      avago_addr_to_str(avago_struct_to_addr(start)),
	      avago_addr_to_str(avago_struct_to_addr(stop)));
	return avago_broadcast_next(ah, first, start, stop,
				   flags | AVAGO_BROADCAST_FIRST);
}

bool avago_is_octal_p1(struct avago_hdl *ah, uint32_t addr)
{
	uint32_t ip_rev = avago_get_ip_rev(ah, addr);
	return ip_rev == 0xe8 || ip_rev == 0xc3 ||
	       ip_rev == 0xea || ip_rev == 0xf3 ||
	       ip_rev == 0xe9 || ip_rev == 0xee ||
	       ip_rev == 0xf7 || ip_rev == 0xf8 ||
	       ip_rev == 0xf9;
}

static bool is_octal_p1(struct avago_hdl *ah, struct avago_addr *addr)
{
	uint32_t addr32 = avago_make_addr3(addr->chip, addr->ring, addr->lane);
	return avago_is_octal_p1(ah, addr32);
}

/**
 * @brief	Continues traversal of addresses between start and stop.
 * @details	addr must contain the previous address on input and is updated
 *		to the next address on output.
 *
 * @param[in]		ah	Avago handle pointer
 * @param[in,out]	addr	Previous/next hardware address
 * @param[in]		start	Start of hardware address range
 * @param[in]		stop	End of hardware address range
 * @param		flags	Flags to control iteration
 *
 * @return	Updates addr and returns TRUE while traversing the selected
 *		addresses.
 *		Returns FALSE when all addresses have been traversed.
 */
bool avago_broadcast_next(struct avago_hdl *ah, struct avago_addr *addr,
			  struct avago_addr *start, struct avago_addr *stop,
			  uint32_t flags)
{
	avago_log_printf(ah, AVAGO_DEBUG9, __func__, __LINE__,
			 "Broadcast_next(addr=%s,start=%s,stop=%s,flags=0x%x,next=%x)\n",
			 avago_addr_to_str(avago_struct_to_addr(addr)),
			 avago_addr_to_str(avago_struct_to_addr(start)),
			 avago_addr_to_str(avago_struct_to_addr(stop)),
			 flags, start->next_addr);
	if (!(flags & AVAGO_BROADCAST_FIRST) &&
	    ((addr->lane == AVAGO_ADDR_IGNORE_LANE) ||
	     (++(addr->lane) > stop->lane) ||
	     (addr->lane > 3 && addr->lane < 8 && !is_octal_p1(ah, addr)))) {
		addr->lane = start->lane;
		if (++(addr->sbus) > stop->sbus) {
			addr->sbus = start->sbus;
			if (++(addr->ring) > stop->ring) {
				addr->ring = start->ring;
				if (++(addr->chip) > stop->chip) {
					if(!start->next_addr) {
						avago_log_printf(ah,
								 AVAGO_DEBUG8,
								 __func__,
								 __LINE__,
								 "Broadcast_next returning FALSE.\n");
						return false;
					}
					return avago_broadcast_first(ah,
								    start->next_addr,
								    start,
								    stop,
								    addr,
								    flags);
				}
			}
		}
	}

	if (addr->lane != AVAGO_ADDR_IGNORE_LANE) {
		uint32_t sbus_addr = avago_struct_to_addr(addr);
		enum avago_ip_type ip_type = avago_get_ip_type(ah, sbus_addr);
		if (ip_type != AVAGO_P1)
			addr->lane = AVAGO_ADDR_IGNORE_LANE;
	}
	avago_log_printf(ah, AVAGO_DEBUG9, __func__, __LINE__,
			 "Broadcast_next returning %s\n",
			 avago_addr_to_str(avago_struct_to_addr(addr)));
	return true;
}

/**
 * Checks if the selected address matches a SerDes device.
 * @details Checks if addr_struct is a D6, M4 or P1 SerDes broadcast address
 *              and addr references an instance of the broadcast type
 *              and the SerDes is not ignoring broadcasts;
 *          OR if addr_struct is not a SerDes broadcast
 *              and addr references any SerDes type.
 *
 * @param[in]	ah		Avago handle
 * @param[in]	addr_struct	Original address struct
 * @param	sbus_addr	Device address
 * @param[out]	ignore		If address selected is broadcast, returns SerDes
 *				broadcast ignore bit.
 *
 * @return  Returns TRUE if addr references a matching SerDes device.
 *          The ignore flag is set if broadcast type and ignore bit is set.
 */
bool avago_addr_selects_fw_device(struct avago_hdl *ah,
				  struct avago_addr *addr_struct,
				  uint32_t sbus_addr,
				  bool *ignore)
{
	if (!avago_check_ip_type(ah, sbus_addr, __func__, __LINE__, false, 4,
				AVAGO_SERDES, AVAGO_SPICO, AVAGO_M4, AVAGO_P1))
		return false;

	if ((addr_struct->sbus == AVAGO_SERDES_D6_BROADCAST &&
	     avago_get_ip_type(ah, sbus_addr) != AVAGO_SERDES) ||
	    (addr_struct->sbus == AVAGO_SPICO_BROADCAST &&
	     avago_get_ip_type(ah, sbus_addr) != AVAGO_SPICO)  ||
	    (addr_struct->sbus == AVAGO_SERDES_M4_BROADCAST &&
	     avago_get_ip_type(ah, sbus_addr) != AVAGO_M4)     ||
	    (addr_struct->sbus == AVAGO_SERDES_P1_BROADCAST &&
	     avago_get_ip_type(ah, sbus_addr) != AVAGO_P1))
		return false;

	if (addr_struct->sbus > AVAGO_MAX_RING_ADDRESS &&
	    addr_struct->sbus != AVAGO_SBUS_MASTER_ADDRESS) {
		bool ig = avago_sbus_rd(ah, sbus_addr, 0xfd) & 0x1;
		if (ignore)
			*ignore = ig;
		return !ig;
	}
	return true;
}

/**
 * @brief Converts data to a 32 character ASCII encoded binary string with
 * optional underscores every 8 bits.
 *
 * @param[out] 	str	Output string, also returned on success
 * @param	data	Data to convert.
 * @param	underscore_en	true to add underscores every 8 bits.
 * @param	bits	Number of bits to convert
 *
 * @return	str is returned; if bits requested is greater than 32, then
 *		NULL is returned.
 */

char *avago_hex_2_bin(char *str, uint32_t data, bool underscore_en, int bits)               /**< Number of bits to convert. */
{
#define kBitsPerByte 8
	if ( bits <= (int)(sizeof(data) * kBitsPerByte) ) {
		char *pDest = str;
		int offset = (kBitsPerByte*(bits%kBitsPerByte) - bits);
		int i;
		for (i = 1; i <= bits; ++i) {
			*pDest++ = ((data >> (bits - i)) & 0x1) + 0x30;

			if (underscore_en && (i != bits) &&
			    ((i + offset) % kBitsPerByte == 0))
				*pDest++ = '_';
		}

		*pDest = '\0';

		return str;
	}
	return NULL;
}
