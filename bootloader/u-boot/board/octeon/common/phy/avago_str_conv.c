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
 * **************************************************************** */
/* AAPL Revision: 2.2.3                                        */
/** Doxygen File Header */
/** @file */
/** @brief Functions for string conversion. */

#include <common.h>
#include <malloc.h>
#include "avago.h"
#include "avago_serdes.h"
#include "avago_serdes_pmd.h"
#include "avago_logging.h"
#include "avago_avsp.h"

static int value_to_index(const struct avago_conv_table tbl[], int value)
{
	int i;
	for (i = 0; tbl[i].name; i++)
		if (value == tbl[i].value)
			return i;
		return -1;
}

/** @defgroup Conversions Type to String and String to Type Conversion Functions */
/** @{ */


static struct avago_conv_table supervisor_mode_table[] = {
	{"TUNE_IF_SIGNAL", AVSP_SUPERVISOR_MODE_TUNE_IF_SIGNAL},
	{"TUNE_IF_LOCKED_SIGNAL", AVSP_SUPERVISOR_MODE_TUNE_IF_LOCKED_SIGNAL},
	{"NO_TUNE", AVSP_SUPERVISOR_MODE_NO_TUNE},
	{0, 0}
};

const char *avago_supervisor_mode_to_str(enum avsp_supervisor_mode value)
{
	struct avago_conv_table *table = supervisor_mode_table;
	int index = value_to_index(table, value);
	return index >= 0 ? table[index].name : "UNKNOWN_SUPERVISOR_MODE";
}

const char *avago_clk_mux_to_str(int mux)
{
	const char *tbl[4] = { "PD", "iclk", "qclk", "rclk" };
	return tbl[mux & 3];
}

/** @brief   Convert uint64_t to a string for displaying. */
/** @details Abstract platform dependencies for converting from a uint64_t */
/**          to a string. */
/** @return  The string representation of uint64_t. */
const char *avago_uint64_t_to_str(uint64_t value)
{
/**< the uint64_t value to convert */
	static char buf[4][24];
	static int which = 0;
	which = (which + 1) % 4;
#ifdef PRId64
	sprintf(buf[which], "%" PRId64, value);
#else
	if (sizeof(uint64_t) == sizeof(long int))
		sprintf(buf[which], "%ld", (long int)value);
	else
		sprintf(buf[which], "%lld", (long long int)value);
#endif
	return buf[which];
}
