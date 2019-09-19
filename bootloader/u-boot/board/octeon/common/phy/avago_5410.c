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

/* AAPL Revision: 2.2.3                                        */
/** Doxygen File Header */
/** @file */
/** @brief Functions specific to the AVSP-5410 */

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_sbus.h"
#include "avago_avsp.h"
#include "avago_logging.h"
#include "avago_spico.h"

/**
 * @brief	Configure internal logic for the given mode.
 * @details	Assumes that the crosspoint configured externally.
 *
 * @param	ah	Avago handle
 * @param	prtad	port address
 * @param	mode	Mode to set
 *
 * @return  On success, returns 0.
 * @return  On failure, decrements ah->return_code and returns -1.
 */
int avsp_5410_set_mode(struct avago_hdl *ah,
		       uint32_t prtad,
		       enum avsp_mode mode)
{
	uint32_t addr = 0;
	switch (mode) {
	case AVSP_REPEATER_DUPLEX:
		break;

	case AVSP_GEARBOX_4_1:
		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0xf9, 0x00);
		avago_sbus_wr(ah, addr, 0x99, 0x0A05);

		addr = avago_make_addr3(prtad, 0, 0x0b);
		avago_sbus_wr(ah, addr, 0x04, 0x0f);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 151, 0x03);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0x00, 0x07);
		avago_sbus_wr(ah, addr, 0x20, 0x07);
		avago_sbus_wr(ah, addr, 0x40, 0x07);
		avago_sbus_wr(ah, addr, 0x60, 0x07);

		addr = avago_make_addr3(prtad, 0, 0x0b);
		avago_sbus_wr(ah, addr, 0x05, 0x28);
		addr = avago_make_addr3(prtad, 0, 0x0c);
		avago_sbus_wr(ah, addr, 0x05, 0x28);

		addr = avago_make_addr3(prtad, 0, 0x0b);
		avago_sbus_wr(ah, addr, 0x04, 0x0e);
		avago_sbus_wr(ah, addr, 0x04, 0x0c);

		avago_sbus_wr(ah, addr, 10, 0x8002);
		avago_sbus_wr(ah, addr, 12, 1279);
		avago_sbus_wr(ah, addr, 6, 0x02);
		avago_sbus_wr(ah, addr, 6, 0x00);

		avago_sbus_wr(ah, addr, 10, 0x7);
		avago_sbus_wr(ah, addr, 12, 0x100);
		avago_sbus_wr(ah, addr, 6, 0x10);
		avago_sbus_wr(ah, addr, 6, 0x00);

		avago_sbus_wr(ah, addr, 4, 0x00);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0x00, 0x05);
		avago_sbus_wr(ah, addr, 0x20, 0x05);
		avago_sbus_wr(ah, addr, 0x40, 0x05);
		avago_sbus_wr(ah, addr, 0x60, 0x05);

		avago_sbus_wr(ah, addr, 151, 0x02);

		addr = avago_make_addr3(prtad, 0, 0xb);
		avago_sbus_wr(ah, addr, 0x04, 0x00);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0x00, 0x01);
		avago_sbus_wr(ah, addr, 0x20, 0x01);
		avago_sbus_wr(ah, addr, 0x40, 0x01);
		avago_sbus_wr(ah, addr, 0x60, 0x01);

	case AVSP_RS_FEC:
		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0xf9, 0x0f);

		avago_sbus_wr(ah, addr, 0xf9, 0x00);
		avago_sbus_wr(ah, addr, 0x99, 0x0A05);

		addr = avago_make_addr3(prtad, 0, 0x0b);
		avago_sbus_wr(ah, addr, 0x04, 0x0f);
		addr = avago_make_addr3(prtad, 0, 0x0c);
		avago_sbus_wr(ah, addr, 0x04, 0x0f);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 151, 0x03);

		avago_sbus_wr(ah, addr, 0x00, 0x07);
		avago_sbus_wr(ah, addr, 0x20, 0x07);
		avago_sbus_wr(ah, addr, 0x40, 0x07);
		avago_sbus_wr(ah, addr, 0x60, 0x07);

		addr = avago_make_addr3(prtad, 0, 0x0b);
		avago_sbus_wr(ah, addr, 0x04, 0x0e);
		avago_sbus_wr(ah, addr, 0x04, 0x0c);

		avago_sbus_wr(ah, addr, 10, 0x8002);
		avago_sbus_wr(ah, addr, 12, 1279);
		avago_sbus_wr(ah, addr, 6, 0x02);
		avago_sbus_wr(ah, addr, 6, 0x00);

		avago_sbus_wr(ah, addr, 10, 0x7);
		avago_sbus_wr(ah, addr, 12, 0x100);
		avago_sbus_wr(ah, addr, 6, 0x10);
		avago_sbus_wr(ah, addr, 6, 0x00);
		avago_sbus_wr(ah, addr, 4, 0x00);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0x00, 0x05);
		avago_sbus_wr(ah, addr, 0x20, 0x05);
		avago_sbus_wr(ah, addr, 0x40, 0x05);
		avago_sbus_wr(ah, addr, 0x60, 0x05);

		avago_sbus_wr(ah, addr, 151, 0x02);

		addr = avago_make_addr3(prtad, 0, 0xb);
		avago_sbus_wr(ah, addr, 0x04, 0x00);

		addr = avago_make_addr3(prtad, 0, 0xec);
		avago_sbus_wr(ah, addr, 0x00, 0x01);
		avago_sbus_wr(ah, addr, 0x20, 0x01);
		avago_sbus_wr(ah, addr, 0x40, 0x01);
		avago_sbus_wr(ah, addr, 0x60, 0x01);
		break;

	default:
		break;
	}
	return 0;
}

/** @brief  Retrieves the mode into which the core logic is configured. */
/** @return Returns the core logic configuration mode. */
enum avsp_mode avsp_5410_get_mode(struct avago_hdl * ah,
				/**< [in] Pointer to Aapl_t structure. */
			       uint32_t prtad)
{
/**< [in] Port address of the target device. */
	uint32_t addr = avago_make_addr3(prtad, 0, 0xfd);
	int val = avago_spico_int(ah, addr, 0x26, 0x3800) & 0x0f;
	enum avsp_mode mode = val == 0x001 ?
			AVSP_REPEATER_DUPLEX : val == 0x002 ?
				AVSP_GEARBOX_4_1 : val == 0x003 ?
					AVSP_RS_FEC : AVSP_ADHOC;
	return mode;
}


/** @brief   Directly resets the core logic. */
/** @return  On success, returns 0. */
/** @return  On failure, decrements aapl->return_code and returns -1. */
bool avsp_5410_control_logic_reset_direct(struct avago_hdl * ah,
					/**< [in] Pointer to Aapl_t structure. */
					  uint32_t prtad,
			   /**< [in] Port address of the targeted device. */
					  const char *direction)
{
/**< [in] direction from host to mod or mod to host */
	uint32_t addr;
	addr = avago_make_addr3(prtad, 0, 0xa);

	if (strcmp(direction, "host_to_mod") == 0 ||
	    strcmp(direction, "both") == 0) {
		printf("Resetting control logic from host to mod direction \n");
		avago_sbus_wr(ah, addr, 0, 0x0002);
		avago_sbus_wr(ah, addr, 20, 0x0002);

		avago_sbus_wr(ah, addr, 0, 0);
		avago_sbus_wr(ah, addr, 20, 0);
	}
	if (strcmp(direction, "mod_to_host") == 0 ||
	    strcmp(direction, "both") == 0) {
		printf("Resetting control logic from mod to host direction \n");
		avago_sbus_wr(ah, addr, 0, 0x0001);
		avago_sbus_wr(ah, addr, 20, 0x0001);

		avago_sbus_wr(ah, addr, 0, 0);
		avago_sbus_wr(ah, addr, 20, 0);

		avago_sbus_wr(ah, addr, 151, 0x0003);
		avago_sbus_wr(ah, addr, 151, 0x0002);
	}
	return 0;
}
