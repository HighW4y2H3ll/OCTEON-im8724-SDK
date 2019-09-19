/* ****************************************************************
 *
 * ASIC and ASSP Programming Layer (AAPL)
 * (C) Copyright 2014-2015 Avago Technologies. All rights reserved.
 *
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
 **************************************************************** */
/* AAPL Revision: 2.2.3                                        */
/** Doxygen File Header */
/** @file */
/** @brief Functions for string conversion. */

#ifndef __AVAGO_STR_CONV_H__
#define __AVAGO_STR_CONV_H__

const char *avago_pmd_train_mode_to_str(enum avago_serdes_pmd_train_mode value);
const char *avago_pmd_clause_to_str(enum avago_serdes_pmd_clause value);
const char *avago_avsp_mode_to_str(enum avsp_mode value);
const char *avago_supervisor_mode_to_str(enum avsp_supervisor_mode value);
const char *avago_clk_mux_to_str(int mux);
const char *avago_uint64_t_to_str(uint64_t value);

#endif
