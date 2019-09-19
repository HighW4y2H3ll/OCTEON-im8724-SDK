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

/* AAPL (ASIC and ASSP Programming Layer) SBus-related functions defined in
 * sbus.cpp.
 */

/** Doxygen File Header
 * * @file
 ** @brief Declarations for SBus functions.
 **/

#ifndef AVAGO_SBUS_H_
#define AVAGO_SBUS_H_

struct avago_hdl;

uint32_t avago_sbus(struct avago_hdl *ah, uint32_t sbus_addr, uint8_t reg,
		    uint8_t cmd, uint32_t data, int recv_data_back);

uint32_t avago_sbus_rd(struct avago_hdl *ah, uint32_t sbus_addr, uint8_t reg);

uint32_t avago_sbus_wr(struct avago_hdl *ah, uint32_t sbus_addr, uint8_t reg,
		       uint32_t data);
uint32_t avago_sbus_wr_flush(struct avago_hdl *ah, uint32_t sbus_addr,
			     uint8_t reg, uint32_t data);

uint32_t avago_sbus_rmw(struct avago_hdl *ah, uint32_t sbus_addr, uint8_t reg,
			uint32_t data, uint32_t mask);

void avago_sbus_reset(struct avago_hdl *ah, uint32_t sbus_addr, int hard);

bool avago_diag_sbus_rw_test(struct avago_hdl *ah, uint32_t sbus_addr,
			     int cycles);

uint32_t avago_sbus_rd_array(struct avago_hdl *ah, uint32_t sbus_addr,
			     uint32_t count, const uint8_t *reg, uint32_t *data);

uint32_t avago_jtag_sbus_fn(struct avago_hdl *ah, uint32_t sbus_address,
			    uint8_t data_address, uint8_t command,
			    uint32_t *data);

#endif /* AVAGO_SBUS_H_ */
