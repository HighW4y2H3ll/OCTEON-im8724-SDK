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

/* Logging and core type conversion functions
 */

/** Doxygen File Header
 * * @file
 ** @brief Logging and core type conversion functions.
 **/

#ifndef __AVAGO_LOGGING_H__
#define __AVAGO_LOGGING_H__

struct avago_hdl;

enum avago_serdes_rx_cmp_data;
enum avago_serdes_rx_cmp_mode;
enum avago_serdes_rx_term;
enum avago_serdes_dfe_tune_mode;
enum avago_serdes_tx_data_sel;
enum avago_serdes_mem_type;
enum avago_serdes_line_encoding;

enum avago_log_type {
	AVAGO_DEBUG0 = 0, AVAGO_DEBUG1, AVAGO_DEBUG2, AVAGO_DEBUG3,
	AVAGO_DEBUG4, AVAGO_DEBUG5, AVAGO_DEBUG6,
	AVAGO_DEBUG7, AVAGO_DEBUG8, AVAGO_DEBUG9,
	AVAGO_MEM_LOG, AVAGO_DATA_CHAR, AVAGO_DATA_CHAR_ADD,
	AVAGO_ERR, AVAGO_WARNING, AVAGO_INFO
};

struct avago_conv_table {
	const char *const name;
	int value;
};

/**
 * Convert an avago_process_id value into a string.
 *
 * @param	value	Process ID value
 * @return	String representation or NULL if no match.
 */
const char *avago_process_id_to_str(enum avago_process_id value);

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "TRUE" or "FALSE".
 * @see    avago_enable_to_str(), avago_onoff_to_str().
 */
const char *avago_bool_to_str(int value);

const char *avago_cmp_data_to_str(enum avago_serdes_rx_cmp_data value);
const char *avago_cmp_mode_to_str(enum avago_serdes_rx_cmp_mode value);
const char *avago_term_to_str(enum avago_serdes_rx_term value);

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "ON" or "OFF".
 * @see    avago_bool_to_str(), avago_enable_to_str().
 */
const char *avago_onoff_to_str(int value);

const char *avago_dfe_tune_mode_to_str(enum avago_serdes_dfe_tune_mode value);

/**
 * @brief  Converts a boolean value into a string.
 * @return Returns either "enabled" or "disabled".
 * @see    avago_bool_to_str(), avago_onoff_to_str().
 */
const char *avago_enable_to_str(int value);

/**
 * Convert an avago_serdes_tx_data_sel value into a string
 */
const char *avago_data_sel_to_str(enum avago_serdes_tx_data_sel value);

const char *avago_get_ip_type_str(struct avago_hdl *ah, uint32_t addr);

/**
 * convert an avago_ip_type value into a string
 */
const char *avago_addr_to_str(enum avago_ip_type value);

const char *avago_ip_type_to_str(enum avago_ip_type type);

const char *avago_mem_type_to_str(enum avago_serdes_mem_type value);

/**
 * Prints an error message
 *
 * @param	ah	Avago handle
 * @param	caller	Calling function name
 * @param	line	Calling line number
 * @param	fmt	Format string
 * @param	...	arguments for format string
 */
int avago_fail(struct avago_hdl *ah, const char *caller, int line,
	       const char *fmt, ...);

/**
 * Prints if the log output is valid.
 *
 * @param	ah	Avago handle
 * @param	log_sel	Log selector
 * @param	caller	Calling function name
 * @param	line	Calling line number
 * @param	fmt	Format string
 * @param	...	arguments for format string
 */
void avago_log_printf(struct avago_hdl *ah, enum avago_log_type log_sel,
		      const char *caller, int line, const char *fmt, ...);

/** @brief  Converts an Avago_serdes_line_encoding_t value into a string.
 * * @return Returns a string representing the type.
 ** @see    aapl_str_to_line_encoding().
 **/
const char *avago_line_encoding_to_str(enum avago_serdes_line_encoding value);


#endif /* __AVAGO_LOGGING_H__ */
