/* ****************************************************************
 *
 * ASIC and ASSP Programming Layer (AAPL)
 * (C) Copyright 2014-2015 Avago Technologies. All rights reserved.
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
 *
 * **************************************************************** */
/* AAPL Revision: 2.2.3                                        */
/* AAPL (ASIC and ASSP Programming Layer) generic handling of SerDes */
/* (Serializer/Deserializer) slices on ASIC SBus rings; all functions exported */
/* from this directory. */

/** Doxygen File Header */
/** @file */
/** @brief Declarations for SerDes DFE functions. */

#ifndef __AVAGO_SERDES_DFE_H__
#define __AVAGO_SERDES_DFE_H__

bool avago_serdes_dfe_running(struct avago_hdl *ah, uint sbus_addr);

int avago_serdes_dfe_pause(struct avago_hdl *ah, uint sbus_addr,
			       uint * rr_enabled);

int avago_serdes_dfe_wait(struct avago_hdl *ah, uint sbus_addr);

int avago_serdes_dfe_resume(struct avago_hdl *ah, uint sbus_addr,
			    uint rr_enable);

#endif /* __AVAGO_SERDES_DFE_H__ */
