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
 ** @brief Functions for SerDes diagnostics.
 **/

#include "avago.h"
#include "avago_logging.h"

/** @brief   Prints a formatted dump of all SBus registers with non-zero values.
 * * @details Dumps all SBus registers for a given sbus_addr.
 **          Only registers with non-zero values are written.  All output
 **          is written using AVAGO_INFO logging via avago_log_printf().
 ** @return  void
 **/
void avago_diag_sbus_dump(struct avago_hdl *ah, uint32_t sbus_addr, bool bin_enable)
{
	int mem_addr;
	avago_log_printf(ah, AVAGO_INFO, 0, 0,
			 "SBus dump for SBus address %s\n",
			 avago_addr_to_str(sbus_addr));

	for (mem_addr = 0; mem_addr <= 0xff; mem_addr++) {
		uint32_t data;
		if (avago_check_ip_type(ah, sbus_addr, __func__, __LINE__,
					false, 1, AVAGO_PMRO)
		    && (mem_addr < 0xf8))
			continue;
		data = avago_sbus_rd(ah, sbus_addr, mem_addr);
		if (data != 0 || ah->verbose) {
			if (bin_enable)
				avago_hex_2_bin(ah->data_char, data, 1, 32);
			else
				ah->data_char[0] = 0x00;

			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "%s 0x%02x: 0x%08x  %s\n",
					 avago_addr_to_str(sbus_addr), mem_addr,
					 data, ah->data_char);
		}
	}
}

static void search_table_file(char *tablefile, uint32_t mem_addr, char *str)
{
	(void)tablefile;
	(void)mem_addr;
	str[0] = 0;
}

static void serdes_dmem_imem_dump(struct avago_hdl *ah, uint32_t sbus_addr,
				  int columns, bool dmem_dump)
{
	int stop_addr;
	int start_addr = 0;
	int buf_len;
	int mem_addr;
	int spico_run_state = 0;
	int page_width = 16;
	char *buf;
	char *tablefile = 0;

	avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");
	avago_log_printf(ah, AVAGO_INFO, __func__, 0,
			 "SerDes %cMEM dump for SBus address %s.\n",
			 dmem_dump ? 'D' : 'I', avago_addr_to_str(sbus_addr));

	if (dmem_dump) {
		stop_addr = 0x3ff;
		if ((avago_get_lsb_rev(ah, sbus_addr) < 5) &&
		    (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_F))
			start_addr = 0x200;
	} else {
		if (avago_get_lsb_rev(ah, sbus_addr) <= 2)
			stop_addr = 8 * 1024 - 1;
		else if (avago_get_lsb_rev(ah, sbus_addr) <= 5)
			stop_addr = 12 * 1024 - 1;
		else
			stop_addr = 24 * 1024 - 1;
	}

	if (!(dmem_dump && (avago_get_lsb_rev(ah, sbus_addr) > 4)))
		spico_run_state = avago_spico_halt(ah, sbus_addr);

	{
		buf = (char *)malloc(page_width * 5 * 1 + 20);
		if (page_width == 16) {
			buf_len = sprintf(buf, "        ");
			for (mem_addr = 0; mem_addr < page_width; mem_addr++)
				buf_len +=
				    sprintf(buf + buf_len, "0x%02x ", mem_addr);
			avago_log_printf(ah, AVAGO_INFO, 0, 1, "%s\n", buf);
		}
	}

	buf_len = sprintf(buf, "0x%04x: ", start_addr);
	for (mem_addr = start_addr; mem_addr <= stop_addr; mem_addr++) {
		uint32_t data;
		char str[1024];
		search_table_file(tablefile, mem_addr, str);
		if (columns == 0
		    || (((mem_addr - start_addr) % page_width) == 0
			&& (mem_addr != start_addr))) {
			avago_log_printf(ah, AVAGO_INFO, 0, 1, "%s\n", buf);
			buf_len = sprintf(buf, "0x%04x: ", mem_addr);
		}
		if (dmem_dump)
			data = avago_serdes_mem_rd(ah, sbus_addr,
						   AVAGO_DMEM_PREHALTED,
						   mem_addr);
		else
			data = avago_serdes_mem_rd(ah, sbus_addr,
						  AVAGO_IMEM_PREHALTED,
						  mem_addr);
		if (tablefile)
			buf_len +=
			    sprintf(buf + buf_len, "%04x %16.16s ", data, str);
		else
			buf_len += sprintf(buf + buf_len, "%04x ", data);
	}
	if (!dmem_dump)
		avago_sbus_wr(ah, sbus_addr, 0x00, 0x00);
	if (!(dmem_dump && (avago_get_lsb_rev(ah, sbus_addr) > 4)))
		avago_spico_resume(ah, sbus_addr, spico_run_state);
	avago_log_printf(ah, AVAGO_INFO, 0, 1, "%s\n", buf);
	avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");

	if (tablefile)
		free(tablefile);
	if (buf)
		free(buf);
}

#define MAX_LINES 0x350
/** @brief  Formats and displays the memory contents of the given SerDes.
 ** @return void
 **/
void avago_serdes_mem_dump(struct avago_hdl *ah,
				    /**< [in] Pointer to struct avago_hdl structure. */
			   uint32_t sbus_addr,
			    /**< [in] SBus address to SerDes. */
			   bool bin_enable,
			    /**< [in] Also display binary value. */
			   bool columns,
			    /**< [in] Enable multi-column output. */
			   bool dma_dump,
			    /**< [in] Dump SerDes register values. */
			   bool dmem_dump,
			    /**< [in] Dump data memory. */
			   bool imem_dump)
{
/**< [in] Dump instruction memory. */
	int stop_addr = 0;
	Avago_serdes_mem_type_t dma_type = AVAGO_LSB;

	int subtype = 0;
	int *data = NULL;
	int *data_addr = NULL;
	int index[2];
	int type;
	char *tablefile = NULL;

	if (imem_dump)
		serdes_dmem_imem_dump(ah, sbus_addr, /* columns */ true, false);

	if ((avago_get_lsb_rev(ah, sbus_addr) > 4) ||
	    (avago_get_process_id(ah, sbus_addr) == AVAGO_PROCESS_B)) {
		serdes_dmem_imem_dump(ah, sbus_addr, columns, true);
		if (avago_get_lsb_rev(ah, sbus_addr) != 8)
			return;
		columns = true;
	}

	if (dmem_dump)
		serdes_dmem_imem_dump(ah, sbus_addr, /* columns */ true, true);

	if (!dma_dump)
		return;

	if (columns) {
		data = (int *)malloc(ah, 2 * MAX_LINES * sizeof(int));
		data_addr = (int *)malloc(2 * MAX_LINES * sizeof(int));
		memset(index, 0, sizeof(index));
		memset(data, 0, 2 * MAX_LINES * sizeof(int));
		memset(data_addr, 0, 2 * MAX_LINES * sizeof(int));
	}

	avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");
	avago_log_printf(ah, AVAGO_INFO, __func__, 0,
			 "SerDes LSB/ESB DMA dump for SBus address %s.\n",
			 avago_addr_to_str(sbus_addr));
	for (type = 0; type <= 3; type++) {
		int mem_addr;
		if (type == 0) {
			if (avago_get_lsb_rev(ah, sbus_addr) == 8)
				continue;
			dma_type = AVAGO_LSB_DIRECT;
			stop_addr = 0x60;
			subtype = 0;
			if (!columns)
				avago_log_printf(ah, AVAGO_INFO, 0, 1,
						 "LSB DMA dump:\n");
		} else if (type == 1) {
			dma_type = AVAGO_ESB;
			stop_addr = 0xff;
			subtype = 0x000;
			if (!columns && avago_get_lsb_rev(ah, sbus_addr) != 8)
				avago_log_printf(ah, AVAGO_INFO, 0, 1,
						 "RX ESB DMA dump:\n");
			else if (!columns)
				avago_log_printf(ah, AVAGO_INFO, 0, 1,
						 "ESB DMA dump:\n");
		} else if (type == 2) {
			if (avago_get_lsb_rev(ah, sbus_addr) == 8)
				continue;
			dma_type = AVAGO_ESB;
			stop_addr = 0xff;
			subtype = 0x200;
			if (!columns)
				avago_log_printf(ah, AVAGO_INFO, 0, 1,
						 "TX ESB DMA dump:\n");
		} else if (type == 3) {
			dma_type = AVAGO_ESB;
			stop_addr = 0x05;
			subtype = 0x300;
			if (!columns)
				avago_log_printf(ah, AVAGO_INFO, 0, 1,
						 "CLK ESB DMA dump:\n");
		}

		if (!columns) {
			for (mem_addr = 0; mem_addr <= stop_addr; mem_addr++) {
				uint32_t read =
				    avago_serdes_mem_rd(ah, sbus_addr, dma_type,
							subtype | mem_addr);
				if (read || ah->verbose) {
					char str[1024];
					str[0] = 0;
					if (dma_type != AVAGO_ESB)
						search_table_file(tablefile,
								  mem_addr,
								  str);
					if (bin_enable)
						avago_hex_2_bin(ah->data_char,
								read, 1, 16);
					else {
						ah->data_char[0] = 0x00;
					}
					avago_log_printf(ah, AVAGO_INFO, 0, 1,
							 "0x%03x 0x%04x %s %16.16s\n",
							 subtype | mem_addr,
							 read, ah->data_char,
							 str);
				}
			}
			avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");
		} else {
			for (mem_addr = 0; mem_addr <= stop_addr; mem_addr++) {
				int col_num;
				uint32_t read =
				    avago_serdes_mem_rd(ah, sbus_addr, dma_type,
							subtype | mem_addr);
				if (type == 0)
					col_num = 0;
				else
					col_num = 1;

				if (!read && !ah->verbose)
					continue;

				data[col_num * MAX_LINES + index[col_num]] =
				    read;
				data_addr[col_num * MAX_LINES +
					  index[col_num]] = subtype | mem_addr;
				index[col_num]++;
			}
		}
	}

	if (columns) {
		int x;
		if (bin_enable) {
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 "                    LSB DMA                                           ESB DMA           \n");
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 " Addr   Data                                       Addr   Data                  \n");
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 "----- ------ ----------------------------------   ----- ------ ----------------------------------\n");
		} else {
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 "   LSB DMA         ESB DMA  \n");
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 " Addr   Data     Addr   Data\n");
			avago_log_printf(ah, AVAGO_INFO, 0, 1,
					 "------------    ------------\n");
		}

		for (x = 0; x <= MAX_LINES; x++) {
			char buf[255];

			int buf_len = 0;
			int have_data = 0;
			int col;
			for (col = 0; col <= 1; col++) {
				char str[1024];
				str[0] = 0;
				if (bin_enable)
					avago_hex_2_bin(ah->data_char,
							data[col * MAX_LINES +
							     x], 1, 16);
				if (col == 0)
					search_table_file(tablefile,
							  data_addr[col *
								    MAX_LINES +
								    x], str);

				if ((ah->verbose
				     && data_addr[col * MAX_LINES + x])
				    || (data[col * MAX_LINES + x]
					&& bin_enable))
					buf_len +=
					    sprintf(buf + buf_len,
						    "0x%03x 0x%04x %16s %16.16s   ",
						    data_addr[col * MAX_LINES +
							      x],
						    data[col * MAX_LINES + x],
						    ah->data_char, str);
				else if ((ah->verbose
					  && data_addr[col * MAX_LINES + x])
					 || data[col * MAX_LINES + x])
					buf_len +=
					    sprintf(buf + buf_len,
						    "0x%03x 0x%04x %16.16s    ",
						    data_addr[col * MAX_LINES +
							      x],
						    data[col * MAX_LINES + x],
						    str);
				else if (bin_enable)
					buf_len +=
					    sprintf(buf + buf_len,
						    "  %3.3s   %4.4s %16s %16.16s    ",
						    "", "", "", "");
				else
					buf_len +=
					    sprintf(buf + buf_len,
						    "%16.16s %16.16s", "", "");

				if (x == 0 || data_addr[col * MAX_LINES + x])
					have_data = 1;
			}
			if (!have_data)
				break;
			avago_log_printf(ah, AVAGO_INFO, 0, 1, "%s\n", buf);
		}
	}
	avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");
	if (tablefile)
		free(atablefile);
	if (data)
		free(data);
	if (data_addr)
		free(data_addr);
}

static char *avago_serdes_get_rx_state_str(struct avago_hdl *ah,
					/**< [in] Pointer to struct avago_hdl structure. */
					   uint32_t addr)
{
/**< [in] SBus address to SerDes. */
	char *buf = (char *)malloc(1200);
	if (buf) {
		char *ptr = buf;
		int row;
		ptr += sprintf(ptr, "Rx State Dump for %s\n",
			       avago_addr_to_str(addr));
		ptr += sprintf(ptr,
			       "         0      1      2      3      4      5      6      7\n");
		ptr += sprintf(ptr,
			       "--- ------ ------ ------ ------ ------ ------ ------ ------\n");
		for (row = 0; row < 16; row++) {
			int col;
			ptr += sprintf(ptr, "%2d:", row);
			for (col = 0; col < 8; col++) {
				int int_data = row << 8 | col << 12;
				int val = avago_spico_int(ah, addr, 0x126,
							  int_data);
				ptr += sprintf(ptr, " 0x%04x", val);
			}
			ptr += sprintf(ptr, "\n");
		}
		ptr += sprintf(ptr, "\n");
	}
	return buf;
}

/** @brief   Gathers detailed data from a SerDes.
 ** @details Gathers a user defined table of TX, RX, DFE, and other information for a single SerDes.
 **          Verbose == 1: Print regardless if CRC passes, and print some extra details.
 **          Verbose == 2: Print eye data as part of RX info
 ** @return  A pointer to the string containing the data requested.
 **/
char *avago_serdes_get_state_dump(struct avago_hdl *ah,
					/**< [in] Pointer to struct avago_hdl structure. */
				  uint32_t addr,
				/**< [in] SBus address to SerDes. */
				  uint32_t disable_features,
				/**< [in] Bitmask of features to disable. */
				  bool ignore_errors)
{
/**< [in] Continue in face of errors. */
	uint32_t features = ~disable_features;
	uint32_t header = features & 0x00000001;
	uint32_t tx = features & 0x00000002;
	uint32_t rx = features & 0x00000004;
	uint32_t clk = features & 0x00000010;
#if AVAGO_ENABLE_EYE_MEASUREMENT || AVAGO_ENABLE_ESCOPE_MEASUREMENT
	uint32_t rx_data = features & 0x00000020;
#endif

	uint32_t details = features & 0x10000000;
	uint32_t column_header = features & 0x80000000;

	char *buf = 0;
	char *buf_end = 0;
	int size = 0;
	int crc = 0;
	uint32_t fw_rev, fw_build_id;
	int failed = 0;
	int spico_running;

	if (!avago_check_process
	    (ah, addr, __func__, __LINE__, true, 3, AVAGO_PROCESS_B,
	     AVAGO_PROCESS_F, AVAGO_PROCESS_A)
	    || !avago_check_ip_type(ah, addr, __func__, __LINE__, true, 3,
				    AVAGO_SERDES, AVAGO_M4, AVAGO_P1))
		return 0;

	AVAGO_SUPPRESS_ERRORS_PUSH(aapl);
	spico_running = avago_spico_running(ah, addr);
	crc = avago_spico_crc(ah, addr);
	fw_rev = avago_firmware_get_rev(ah, addr);
	fw_build_id = avago_firmware_get_build_id(ah, addr);
	AVAGO_SUPPRESS_ERRORS_POP(aapl);

	if ((!crc || !spico_running) & !ignore_errors)
		failed = 1;

	if (header) {
		int lsb_rev = avago_serdes_get_lsb_rev(ah, addr);
		char rev_id_name[30];
		sprintf(rev_id_name, "SerDes_0x%x", avago_get_ip_rev(ah, addr));
		avago_buf_add(ah, &buf, &buf_end, &size,
			      "########## %s %s %s\n",
			      "SerDes state dump for SBus address",
			      avago_addr_to_str(addr),
			      " ##############################");
		avago_buf_add(ah, &buf, &buf_end, &size,
			      "%s LSB rev %d. Firmware: 0x%04X_%04X.\n\n",
			      rev_id_name, lsb_rev, fw_rev, fw_build_id);
	}

	{
		const char *sep = "---------------";
		bool tx_en = false, rx_en = false;
		int tx_width = 0, rx_width = 0;
		enum avago_serdes_tx_data_sel tx_data_sel =
		    AVAGO_SERDES_TX_DATA_SEL_PRBS7;
		if (tx || details)
			tx_data_sel = avago_serdes_get_tx_data_sel(ah, addr);
		if (tx || rx) {
			avago_serdes_get_tx_rx_ready(ah, addr, &tx_en, &rx_en);
			avago_serdes_get_tx_rx_width(ah, addr, &tx_width,
						     &rx_width);
		}

		/*////// Display TX info: */
		if (tx && column_header) {
			const char *fmt =
			    "%8.8s %.2s %.3s %.3s %8.8s %.3s %.3s %.3s %.4s\n";
			avago_buf_add(ah, &buf, &buf_end, &size, fmt, "Addr",
				      "TX", "Wth", "Inv", "Data", "Out", "Pre",
				      "Att", "Post");
			avago_buf_add(ah, &buf, &buf_end, &size, fmt, sep, sep,
				      sep, sep, sep, sep, sep, sep, sep);
		}
		if (tx && failed)
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%6s No data collected. CRC: %d, spico_running: %d\n",
				      avago_addr_to_str(addr), crc,
				      spico_running);
		else if (tx) {
			bool tx_out =
			    avago_serdes_get_tx_output_enable(ah, addr);
			int tx_invert = avago_serdes_get_tx_invert(ah, addr);
			bool tx_encoding =
			    avago_serdes_get_tx_line_encoding(ah, addr);
			Avago_serdes_tx_eq_t tx_eq;
			avago_serdes_get_tx_eq(ah, addr, &tx_eq);
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%8s %2d %s%2d %3s ",
				      avago_addr_to_str(addr), tx_en,
				      tx_encoding ? "P" : " ", tx_width,
				      avago_onoff_to_str(tx_invert));

			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%8s %3d %3d %3d %4d\n",
				      avago_data_sel_to_str(tx_data_sel),
				      tx_out, tx_eq.pre, tx_eq.atten,
				      tx_eq.post);
		}
		if (tx && header)
			avago_buf_add(ah, &buf, &buf_end, &size, "\n");

		/*////// Display RX info: */
		if (rx && column_header) {
			const char *fmt =
			    "%8.8s %.2s %.3s %.3s %9.9s %12.12s %3.3s %.2s %.2s %.2s %5.5s %5.5s %1.1s %10.10s\n";
			avago_buf_add(ah, &buf, &buf_end, &size, fmt, "Addr",
				      "RX", "Wth", "Inv", "Data", "Cmp_Mode",
				      "EI", "OK", "LK", "LB", "Term", "Phase",
				      "E", "Errors");
			avago_buf_add(ah, &buf, &buf_end, &size, fmt, sep, sep,
				      sep, sep, sep, sep, sep, sep, sep, sep,
				      sep, sep);
		}
		if (rx && failed)
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%6s No data collected. CRC: %d, spico_running: %d\n",
				      avago_addr_to_str(addr), crc,
				      spico_running);
		else if (rx) {
			int rx_invert = avago_serdes_get_rx_invert(ah, addr);
			int rx_input_sel =
			    avago_serdes_get_rx_input_loopback(ah, addr);
			Avago_serdes_rx_term_t rx_term =
			    avago_serdes_get_rx_term(ah, addr);
			Avago_serdes_rx_cmp_mode_t rx_cmp_mode =
			    avago_serdes_get_rx_cmp_mode(ah, addr);
			Avago_serdes_rx_cmp_data_t rx_cmp_data =
			    avago_serdes_get_rx_cmp_data(ah, addr);
			uint32_t error_count =
			    avago_serdes_get_errors(ah, addr, AVAGO_LSB, false);
			bool error_flag =
			    avago_serdes_get_error_flag(ah, addr, false);
			bool signal_ok1 =
			    avago_serdes_get_signal_ok(ah, addr, true);
			bool signal_ok2 =
			    avago_serdes_get_signal_ok(ah, addr, true);
			bool freq_lock =
			    avago_serdes_get_frequency_lock(ah, addr);
			bool rx_encoding =
			    avago_serdes_get_rx_line_encoding(ah, addr);
			uint32_t phase = 0;
			char elec_idle_buf[16];
			char error_cnt_buf[16] = "-";

			if (!avago_serdes_get_signal_ok_enable(ah, addr))
				snprintf(elec_idle_buf, sizeof(elec_idle_buf),
					 "Dis");
			else if (avago_serdes_get_electrical_idle(ah, addr))
				snprintf(elec_idle_buf, sizeof(elec_idle_buf),
					 "1");
			else
				snprintf(elec_idle_buf, sizeof(elec_idle_buf),
					 "0");

			if (ah->debug
			    || (rx_cmp_mode != AVAGO_SERDES_RX_CMP_MODE_OFF
				&& rx_cmp_mode != AVAGO_SERDES_RX_CMP_MODE_XOR))
				snprintf(error_cnt_buf, sizeof(error_cnt_buf),
					 "%u", error_count);

			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%8s %2d %s%2d %3s",
				      avago_addr_to_str(addr), rx_en,
				      rx_encoding ? "P" : " ", rx_width,
				      avago_onoff_to_str(rx_invert));

			avago_buf_add(ah, &buf, &buf_end, &size, " %9s",
				      avago_cmp_data_to_str(rx_cmp_data));

			avago_buf_add(ah, &buf, &buf_end, &size,
				      " %12s %3s %d%d %2d %2d %5s %5d %1d %10s\n",
				      avago_cmp_mode_to_str(rx_cmp_mode),
				      elec_idle_buf, signal_ok1, signal_ok2,
				      freq_lock, rx_input_sel,
				      avago_term_to_str(rx_term), phase,
				      error_flag, error_cnt_buf);
		}
		if (rx && header)
			avago_buf_add(ah, &buf, &buf_end, &size, "\n");

		/*////// Display extra details */
		if (details && !failed) {
			int threshold =
			    avago_serdes_get_signal_ok_threshold(ah, addr);
			if (tx_data_sel == AVAGO_SERDES_TX_DATA_SEL_USER) {
				long tx_user[4];
				avago_serdes_get_tx_user_data(ah, addr,
							      tx_user);
				avago_log_printf(ah, AVAGO_INFO, 0, 0,
						 " (0x%05lx)", tx_user[0]);
			}
			avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");

			avago_buf_add(ah, &buf, &buf_end, &size,
				      "RX: EI threshold = %d\n", threshold);
		}
		if (details && header)
			avago_buf_add(ah, &buf, &buf_end, &size, "\n");

		/*////// Display RX info: */
#       if AVAGO_ENABLE_EYE_MEASUREMENT || AVAGO_ENABLE_ESCOPE_MEASUREMENT
		if (rx_data && ah->verbose > 1) {
			if (column_header) {
				avago_buf_add(ah, &buf, &buf_end, &size,
					      "%6s %s\n", "Addr", "RX Data");
				avago_buf_add(ah, &buf, &buf_end, &size,
					      "%6s %7s\n", "------", "-------");
			}
			if (failed)
				avago_buf_add(ah, &buf, &buf_end, &size,
					      "%6s No data collected. CRC: %d, spico_running: %d\n",
					      avago_addr_to_str(addr), crc,
					      spico_running);
			else {
				int i;
				int data =
				    avago_serdes_get_rx_live_data(ah, addr);
				char pattern[17];
				for (i = 0; i < 16; i++)
					pattern[i] =
					    (data & (1 << i)) ? '1' : '0';
				pattern[16] = '\0';
				avago_buf_add(ah, &buf, &buf_end, &size,
					      "%6s %s\n",
					      avago_addr_to_str(addr), pattern);
			}
			if (header)
				avago_buf_add(ah, &buf, &buf_end, &size, "\n");
		}
#       endif

		/*////// Display CLK info: */
		if (clk && column_header) {
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "    Addr         SPICO             TX PLL\n");
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "-------- ------------- ------------------\n");
		}
		if (clk && failed)
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%6s No data collected. CRC: %d, spico_running: %d\n",
				      avago_addr_to_str(addr), crc,
				      spico_running);
		else if (clk) {
			Avago_serdes_spico_clk_t spico_clk =
			    avago_serdes_get_spico_clk_src(ah, addr);
			Avago_serdes_tx_pll_clk_t tx_pll_clk =
			    avago_serdes_get_tx_pll_clk_src(ah, addr);
			avago_buf_add(ah, &buf, &buf_end, &size,
				      "%8.8s %13.13s %18.18s\n",
				      avago_addr_to_str(addr),
				      avago_spico_clk_to_str(spico_clk),
				      avago_pll_clk_to_str(tx_pll_clk));
		}
		if (clk && header)
			avago_buf_add(ah, &buf, &buf_end, &size, "\n");

		/*////// Display DFE info */
		if (features & 0x00000008) {
			char *buf2 = 0;

#    if AVAGO_DIAG
			buf2 = avago_serdes_get_rx_state_str(ah, addr);
#    endif

			if (buf2) {
				avago_buf_add(ah, &buf, &buf_end, &size, "%s",
					      buf2);
				avago_free(ah, buf2, __func__);
			}
		}
	}
	return buf;
}

/** @brief   Prints detailed information from a SerDes.
 * * @details Prints a predefined table of TX, RX, DFE, and other information for a single SerDes.
 **/
void avago_serdes_state_dump(struct avago_hdl *ah, uint32_t addr)
{
	char *buf = avago_serdes_get_state_dump(ah, addr, 0, true);
	if (buf) {
		avago_log_printf(ah, AVAGO_INFO, 0, 0, "%s\n", buf);
		avago_free(ah, buf, __func__);
	}
}

/** @brief   Prints detailed information for a SerDes.
 * * @details Prints a table of TX, RX, DFE, and other information for the given SerDes. This function is broadcast safe.
 **/
void avago_serdes_print_state_table_options(struct avago_hdl *ah,
					    Avago_addr_t * addr_struct,
					    Avago_state_table_options_t *
					    options)
{
	uint32_t loop = 0;
	for (loop = 0; loop <= 4; loop++) {
		uint32_t header = 0;
		Avago_addr_t start, stop, next;
		bool st;

		if (options) {
			if (options->disable_tx && loop == 0)
				continue;
			if (options->disable_rx && loop == 1)
				continue;
			if (options->disable_dfe && loop == 2)
				continue;
			if (options->disable_clk && loop == 3)
				continue;
		}

		for (st =
		     avago_broadcast_first(ah, addr_struct, &start, &stop,
					   &next, AVAGO_BROADCAST_LANE); st;
		     st =
		     avago_broadcast_next(ah, &next, &start, &stop,
					  AVAGO_BROADCAST_LANE)) {
			char *buf = 0;
			uint32_t sbus_addr = avago_struct_to_addr(&next);
			if (!avago_check_ip_type
			    (ah, sbus_addr, 0, 0, false, 3, AVAGO_SERDES,
			     AVAGO_M4, AVAGO_P1))
				continue;

			if (!header++)
				buf =
				    avago_serdes_get_state_dump(ah, sbus_addr,
								~(1 <<
								  (loop +
								   1)) &
								~0x80000000,
								false);
			else
				buf =
				    avago_serdes_get_state_dump(ah, sbus_addr,
								~(1 <<
								  (loop + 1)),
								false);

			if (buf) {
				avago_log_printf(ah, AVAGO_INFO, 0, 0, "%s",
						 buf);
				avago_free(ah, buf, __func__);
			}
		}
		avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");
	}
}

/** @brief Print information on the device(s) AAPL is connected to using the AVAGO_INFO log type (which is typically STDOUT).
 * * @details The addr field selects the device to print info on.
 **          As ah->verbose levels increase, more information is printed
 **          The type input will print info only for the IP type specified.
 **          Provides additional options as compared with avago_device_info.\n
 ** @return void
 **/
void avago_device_info_options(struct avago_hdl *ah,
			       Avago_addr_t * addr_struct,
			       Avago_ip_type_t type,
			       Avago_state_table_options_t * options)
{
	char buf[640];
	char *ptr = buf;
	bool st;
	Avago_addr_t start, stop, next;
	uint32_t prev_chip = ~0;
	uint32_t name_len = 0;
	const char **name_list = 0;
	Avago_addr_t addr2_struct = *addr_struct;

	avago_log_printf(ah, AVAGO_INFO, 0, 0, "Avago Device Info:\n");

	avago_log_printf(ah, AVAGO_INFO, 0, 0,
			 "  Communication method:      %s\n",
			 avago_comm_method_to_str(ah->communication_method));
	if (avago_is_aacs_communication_method(aapl)) {
		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  Connected to:              %s:%d\n",
				 ah->aacs_server, ah->tcp_port);
		if (ah->capabilities != 0)
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "  Remote AACS capabilities:  %x\n",
					 ah->capabilities);
	}
	if (ah->verbose >= 2) {
		uint32_t c;
		ptr +=
		    sprintf(ptr, "  Number of devices:         %d (",
			    ah->chips);
		for (c = 0; c < ah->chips; c++)
			ptr +=
			    sprintf(ptr, c == 0 ? "%s" : ", %s",
				    ah->chip_name[c]);
		avago_log_printf(ah, AVAGO_INFO, 0, 0, "%s)\n", buf);

		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  debug:                     %x\n",
				 ah->debug);
		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  enable_debug_logging:      %x\n",
				 ah->enable_debug_logging);
		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  enable_stream_logging:     %x\n",
				 ah->enable_stream_logging);
		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  enable_stream_err_logging: %x\n",
				 ah->enable_stream_err_logging);

		avago_log_printf(ah, AVAGO_INFO, 0, 0,
				 "  log size / data_char size: %d of %d / %d of %d\n",
				 (ah->data_char_end - ah->data_char),
				 ah->data_char_size, (ah->log_end - ah->log),
				 ah->log_size);
	}

	for (st =
	     avago_broadcast_first(ah, &addr2_struct, &start, &stop, &next,
				   AVAGO_BROADCAST_IGNORE_LANE); st;
	     st =
	     avago_broadcast_next(ah, &next, &start, &stop,
				  AVAGO_BROADCAST_IGNORE_LANE)) {
		if (next.chip != prev_chip) {
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "\nInformation for chip %d:\n",
					 next.chip);
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "  JTAG ID: 0x%08x",
					 ah->jtag_idcode[next.chip]);
			if (*ah->chip_name[next.chip])
				avago_log_printf(ah, AVAGO_INFO, 0, 0,
						 ";  %s rev %s",
						 ah->chip_name[next.chip],
						 ah->chip_rev[next.chip]);
			else
				avago_log_printf(ah, AVAGO_INFO, 0, 0, " (%s)",
						 avago_hex_2_bin(ah->data_char,
								 ah->
								 jtag_idcode
								 [next.chip], 0,
								 32));
			avago_log_printf(ah, AVAGO_INFO, 0, 0, ";  %s\n",
					 avago_process_id_to_str(ah->
								 process_id
								 [next.chip]));

			avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");
			if (ah->debug >= 4)
				avago_log_printf(ah, AVAGO_INFO, 0, 0,
						 "IP-ID ");
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "  Addr              IP Type  Rev  LSB Firmware    On Miscellaneous Info\n");
			if (ah->debug >= 4)
				avago_log_printf(ah, AVAGO_INFO, 0, 0,
						 "----- ");
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "------ -------------------- ---- ---- ----------- -- ----------------------\n");

			name_len = 0;
			name_list = 0;
#if AVAGO_ENABLE_AVSP
			if (0 == strncmp(ah->chip_name[next.chip], "AVSP-", 5))
				avsp_get_name_list(ah, next.chip,
						   ah->chip_name[next.chip],
						   &name_len, &name_list);
#endif
		}
		prev_chip = next.chip;

		if (0 == ah->max_sbus_addr[next.chip][next.ring])
			continue;

		if ((int)next.sbus <=
		    ah->max_sbus_addr[next.chip][next.ring] + 2
		    || next.sbus == 0xfd || next.sbus == 0xfe) {
			Avago_addr_t addr_struct2;
			uint32_t sbus_addr;
			Avago_ip_type_t ip_type;
			int ip_rev;
			ptr = buf;

			avago_addr_init(&addr_struct2);
			addr_struct2.chip = next.chip;
			addr_struct2.ring = next.ring;
			addr_struct2.sbus = next.sbus;
			addr_struct2.lane = next.lane;
			if ((int)next.sbus ==
			    ah->max_sbus_addr[next.chip][next.ring] + 1)
				addr_struct2.sbus = 0xfd;
			if ((int)next.sbus ==
			    ah->max_sbus_addr[next.chip][next.ring] + 2)
				addr_struct2.sbus = 0xfe;
			sbus_addr = avago_struct_to_addr(&addr_struct2);

			ip_type = avago_get_ip_type(ah, sbus_addr);
			if (type && type != ip_type)
				continue;

			ip_rev = avago_get_ip_rev(ah, sbus_addr);

			if (ah->debug >= 4)
				ptr +=
				    sprintf(ptr, "0x%02x: ",
					    avago_sbus_rd(ah, sbus_addr, 0xff));
			if (next.sbus < name_len && name_list
			    && name_list[next.sbus]) {
				char ip_type_buf[100];
				sprintf(ip_type_buf, "%s %s",
					avago_ip_type_to_str(ip_type),
					name_list[next.sbus]);
				ptr +=
				    sprintf(ptr, "%6s %20s",
					    avago_addr_to_str(sbus_addr),
					    ip_type_buf);
			} else
				ptr +=
				    sprintf(ptr, "%6s %20s",
					    avago_addr_to_str(sbus_addr),
					    avago_ip_type_to_str(ip_type));

			if (ip_type == AVAGO_THERMAL_SENSOR) {
			} else {
				int firm_rev =
				    avago_get_firmware_rev(ah, sbus_addr);
				int firm_build =
				    avago_get_firmware_build(ah, sbus_addr);
				int lsb_rev = avago_get_lsb_rev(ah, sbus_addr);

				if (ip_rev != 0xffff)
					ptr += sprintf(ptr, " 0x%02x", ip_rev);
				else
					ptr += sprintf(ptr, " %4s", "    ");

				if (lsb_rev != 0xff)
					ptr += sprintf(ptr, " 0x%02x", lsb_rev);
				else
					ptr += sprintf(ptr, " %4s", "    ");

				if (firm_rev != 0xffff)
					ptr +=
					    sprintf(ptr, " 0x%04X_%04X  %d",
						    firm_rev, firm_build,
						    avago_get_spico_running_flag
						    (ah, sbus_addr));
				else
					ptr += sprintf(ptr, " %11s   ", "");

				while (ptr[-1] == ' ')
					*--ptr = '\0';
			}
			avago_log_printf(ah, AVAGO_INFO, 0, 0, "%-s\n", buf);
		}
	}

	if (ah->verbose >= 1) {
		avago_log_printf(ah, AVAGO_INFO, 0, 0, "\n");
		avago_serdes_print_state_table_options(ah, addr_struct,
						       options);
	}

	if (ah->verbose >= 3) {
		avago_log_printf(ah, AVAGO_INFO, 0, 1,
				 "  struct avago_hdl buffers:\n");
		avago_log_printf(ah, AVAGO_INFO, 0, 1,
				 "  --------- data_char:       ---------\n%s\n",
				 ah->data_char);
		avago_log_printf(ah, AVAGO_INFO, 0, 1,
				 "  --------- end of data_char ---------\n");
		avago_log_printf(ah, AVAGO_INFO, 0, 1,
				 "  --------- log:             ---------\n");
#       ifdef AVAGO_STREAM
		fwrite(ah->log, 1, ah->log_end - ah->log, AVAGO_STREAM);
#       endif
		avago_log_printf(ah, AVAGO_INFO, 0, 1,
				 "  --------- end of log       ---------\n");
		avago_log_printf(ah, AVAGO_INFO, 0, 1, "\n");
	}

}

#if AVAGO_ENABLE_DIAG

/** @brief   Allocates and initializes a default Avago_diag_config_t object.
 * * @return  On success, returns a pointer to the initialized structure, which
 **          should be freed by calling avago_diag_config_destruct() after use.
 ** @return  On failure, returns NULL.
 ** @see     avago_serdes_diag(), avago_diag_config_destruct().
 **/
Avago_diag_config_t *avago_diag_config_construct(struct avago_hdl *aapl)
{
	int dividers[] =
	    { 10, 20, 30, 40, 50, 60, 66, 70, 80, 90, 100, 110, 120, 165, -1 };
	Avago_diag_config_t *config;
	size_t bytes = sizeof(Avago_diag_config_t);

	if (!
	    (config =
	     (Avago_diag_config_t *) avago_malloc(ah, bytes, __func__)))
		return (NULL);
	memset(config, 0, sizeof(*config));

	config->binary = true;
	config->columns = false;
	config->serdes_init_only = false;
	config->state_dump = true;
	config->sbus_dump = true;
	config->dma_dump = true;
	config->dmem_dump = true;
	config->imem_dump = false;
	config->cycles = 20;

	config->dividers = (int *)avago_malloc(ah, sizeof(dividers), __func__);
	if (!config->dividers) {
		avago_free(ah, config, __func__);
		return NULL;
	}

	memcpy(config->dividers, dividers, sizeof(dividers));
	return config;
}

/** @brief   Releases an Avago_diag_config_t struct.
 * * @return  None.
 ** @see     avago_diag_config_construct(), avago_serdes_diag().
 **/
void avago_diag_config_destruct(struct avago_hdl *ah,
				Avago_diag_config_t * config)
{
	avago_free(ah, config->dividers, __func__);
	avago_free(ah, config, __func__);
}

/** @brief   Run diagnostics and memory dumps on SerDes.
 * * @details See the config struct for more info.
 **
 ** @return Return 0 on success, negative on error.
 **/
int avago_serdes_diag(struct avago_hdl *ah,	/**< [in] Pointer to struct avago_hdl structure. */
		      uint32_t sbus_addr_in,/**< [in] SBus address to SerDes. */
		      Avago_diag_config_t * config)
{
/**< [in] Options for the function. */
	bool st;
#if AVAGO_ENABLE_AVSP
	bool self_healing = false;
#endif
	Avago_addr_t addr_struct, start, stop, next;

	if (!avago_check_process
	    (ah, sbus_addr_in, __func__, __LINE__, true, 3, AVAGO_PROCESS_A,
	     AVAGO_PROCESS_B, AVAGO_PROCESS_F))
		return 0;
	if (!avago_check_ip_type
	    (ah, sbus_addr_in, __func__, __LINE__, true, 5, AVAGO_SPICO,
	     AVAGO_SERDES, AVAGO_SERDES_BROADCAST, AVAGO_M4, AVAGO_P1))
		return 0;

	avago_addr_to_struct(sbus_addr_in, &addr_struct);
	for (st =
	     avago_broadcast_first(ah, &addr_struct, &start, &stop, &next, 0);
	     st; st = avago_broadcast_next(ah, &next, &start, &stop, 0)) {
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (!avago_check_ip_type
		    (ah, sbus_addr, __func__, __LINE__, false, 3, AVAGO_SERDES,
		     AVAGO_M4, AVAGO_P1))
			continue;

		avago_diag_sbus_rw_test(ah, sbus_addr, 2);

		if (config->sbus_dump)
			avago_diag_sbus_dump(ah, sbus_addr, config->binary);

		if (!avago_check_process
		    (ah, sbus_addr_in, __func__, __LINE__, false, 2,
		     AVAGO_PROCESS_B, AVAGO_PROCESS_F))
			return 0;

#if AVAGO_ENABLE_AVSP
		if (avsp_get_self_healing(ah, next.chip)) {
			self_healing = true;
			avsp_set_self_healing(ah, next.chip, false);
		}
#endif

		if (config->dma_dump || config->imem_dump || config->dmem_dump)
			avago_serdes_mem_dump(ah, sbus_addr, config->binary,
					      config->columns, config->dma_dump,
					      config->dmem_dump,
					      config->imem_dump);
		if (config->cycles)
			avago_spico_diag(ah, sbus_addr, config->cycles);
		if (config->pmd_debug) {
			Avago_serdes_pmd_debug_t *pmd =
			    avago_serdes_pmd_debug_construct(aapl);
			avago_log_printf(ah, AVAGO_INFO, 0, 0,
					 "########## %s %s %s\n",
					 "SerDes PMD debug for SBus address",
					 avago_addr_to_str(sbus_addr),
					 " ##############################");
			avago_serdes_pmd_debug(ah, sbus_addr, pmd);
			avago_serdes_pmd_debug_print(ah, pmd);
			avago_serdes_pmd_debug_destruct(ah, pmd);
		}

		if (config->destructive) {
			int divider = 0;
			int div_loop = -1;
			while (config->dividers[++div_loop] != -1
			       && !(config->use_existing_divider
				    && div_loop != 0)) {
				int ilb, errors;
				divider = config->dividers[div_loop];

				{
					Avago_serdes_init_config_t *config =
					    avago_serdes_init_config_construct
					    (aapl);
					int log_en =
					    ah->enable_stream_err_logging;
					ah->enable_stream_err_logging = 0;

					config->init_tx = true;
					config->init_rx = true;
					config->sbus_reset = true;
					config->init_mode = AVAGO_PRBS31_ILB;
					config->tx_divider = divider;
					config->rx_divider = divider;
					errors =
					    avago_serdes_init(ah, sbus_addr,
							      config);
					avago_serdes_init_config_destruct(ah,
									  config);

					ah->enable_stream_err_logging = log_en;
				}
				avago_get_return_code(aapl);

				for (ilb = 1; ilb >= 0; ilb--) {
					int data_sel_loop;
					char test_name[256];
					avago_serdes_set_rx_input_loopback(ah,
									   sbus_addr,
									   ilb);
					if (ilb)
						snprintf(test_name, 256, "ILB");
					else
						snprintf(test_name, 256, "ELB");

					if (ilb) {
						avago_log_printf(ah, AVAGO_INFO,
								 0, 1,
								 "%10s %7s %2s %10s\n",
								 "Mode", "Data",
								 "",
								 "Non-invert");
						avago_log_printf(ah, AVAGO_INFO,
								 0, 1,
								 "%10s %7s %2s %10s\n",
								 "----------",
								 "-------", "",
								 "----------");
					} else if (!ilb) {
						avago_log_printf(ah, AVAGO_INFO,
								 0, 1,
								 "%10s %7s %2s %10s %10s\n",
								 "Mode", "Data",
								 "OK",
								 "Non-invert",
								 "Inverted");
						avago_log_printf(ah, AVAGO_INFO,
								 0, 1,
								 "%10s %7s %2s %10s %10s\n",
								 "----------",
								 "-------",
								 "--",
								 "----------",
								 "--------");
					}
					for (data_sel_loop =
					     AVAGO_SERDES_TX_DATA_SEL_PRBS7;
					     data_sel_loop <=
					     AVAGO_SERDES_TX_DATA_SEL_USER;
					     data_sel_loop++) {
						Avago_serdes_tx_data_sel_t
						    data_sel =
						    (Avago_serdes_tx_data_sel_t)
						    data_sel_loop;
						Avago_serdes_rx_cmp_data_t
						    cmp_data =
						    (Avago_serdes_rx_cmp_data_t)
						    data_sel_loop;
						if (data_sel_loop ==
						    AVAGO_SERDES_TX_DATA_SEL_PRBS31
						    + 1)
							continue;

						avago_serdes_set_tx_data_sel(ah,
									     sbus_addr,
									     data_sel);
						avago_serdes_set_rx_cmp_data(ah,
									     sbus_addr,
									     cmp_data);

						if (!ilb)
							avago_serdes_initialize_signal_ok
							    (ah, sbus_addr, 0);
						avago_serdes_get_errors(ah,
									sbus_addr,
									AVAGO_LSB,
									/*reset= */
									1);
						errors =
						    avago_serdes_get_errors(ah,
									    sbus_addr,
									    AVAGO_LSB,
									    /*reset= */
									    0);
						if (ilb)
							avago_log_printf(ah,
									 AVAGO_INFO,
									 0, 1,
									 "%10s %7s %2s %10d\n",
									 test_name,
									 avago_data_sel_to_str
									 (data_sel),
									 "",
									 errors);
						else {
							uint32_t signal_ok =
							    avago_serdes_get_signal_ok
							    (ah, sbus_addr,
							     true);
							avago_log_printf(ah,
									 AVAGO_INFO,
									 0, 1,
									 "%10s %7s %2d %10d ",
									 test_name,
									 avago_data_sel_to_str
									 (data_sel),
									 signal_ok,
									 errors);
							avago_serdes_set_rx_invert
							    (ah, sbus_addr, 1);
							avago_serdes_get_errors
							    (ah, sbus_addr,
							     AVAGO_LSB,
							     /*reset= */ 1);
							errors =
							    avago_serdes_get_errors
							    (ah, sbus_addr,
							     AVAGO_LSB,
							     /*reset= */ 0);
							avago_log_printf(ah,
									 AVAGO_INFO,
									 0, 0,
									 "%10d",
									 errors);
							avago_serdes_set_rx_invert
							    (ah, sbus_addr, 0);

							avago_serdes_set_rx_cmp_mode
							    (ah, sbus_addr,
							     AVAGO_SERDES_RX_CMP_MODE_XOR);
							avago_serdes_get_errors
							    (ah, sbus_addr,
							     AVAGO_LSB,
							     /*reset= */ 1);
							errors =
							    avago_serdes_get_errors
							    (ah, sbus_addr,
							     AVAGO_LSB,
							     /*reset= */ 0);
							avago_log_printf(ah,
									 AVAGO_INFO,
									 0, 0,
									 " %10d\n",
									 errors);
							avago_serdes_set_rx_cmp_mode
							    (ah, sbus_addr,
							     AVAGO_SERDES_RX_CMP_MODE_MAIN_PATGEN);
						}
					}
					if (ilb)
						avago_log_printf(ah, AVAGO_INFO,
								 0, 1, "\n");
				}
			}
		}
#if AVAGO_ENABLE_AVSP
		if (self_healing)
			avsp_set_self_healing(ah, next.chip, true);
#endif
	}
	return ah->return_code;
}

/**=============================================================================
 * * Diag
 ** Run diagnostics on various IP. See the Avago_diag_config_t struct for more info.
 **
 ** @brief  Display diagnostics for the given address, which may be a
 ** @return On success, returns 0.  Otherwise decrements ah->return_code and returns -1;
 ** @return On failure, decrements ah->return_code and returns -1;
 **/
int avago_diag(struct avago_hdl *ah, uint32_t sbus_addr_in,
	       Avago_diag_config_t * config)
{
	int return_code = ah->return_code;
	bool st;
	Avago_addr_t addr_struct, start, stop, next;

	avago_log_printf(ah, AVAGO_INFO, 0, 1,
			 "AAPL diag started. AAPL version: " AVAGO_VERSION ";  "
			 AVAGO_COPYRIGHT "\n");
	avago_log_printf(ah, AVAGO_INFO, 0, 1, "ah->return_code: %d\n",
			 ah->return_code);

	if (ah->capabilities & AACS_SERVER_DIAG) {
		uint32_t index;
		char cmd_buffer[320];
		char *cmd_buffer_end = cmd_buffer;

		cmd_buffer_end +=
		    snprintf(cmd_buffer_end, 319, "diag %s ",
			     avago_addr_to_str(sbus_addr_in));

		for (index = 0; index < sizeof(Avago_diag_config_t) - 8;
		     index++)
			cmd_buffer_end +=
			    snprintf(cmd_buffer_end,
				     cmd_buffer_end - cmd_buffer - 319, "0x%x ",
				     *((char *)config + index));

		avago_log_printf(ah, AVAGO_DEBUG6, __func__, __LINE__,
				 "Executing AACS direct diag cmd: %s\n",
				 cmd_buffer);
		AVAGO_SUPPRESS_ERRORS_PUSH(aapl);
		avago_aacs_send_command_options(ah, cmd_buffer,
						/* recv_data_back */ 1,
						/* strtol */ 0);
		AVAGO_SUPPRESS_ERRORS_POP(aapl);
		avago_log_add(ah, AVAGO_INFO, ah->data_char, __func__,
			      __LINE__);

		return return_code == ah->return_code ? 0 : -1;
	}

	{
		int prev_verbose = ah->verbose;
		Avago_addr_t addr_struct;
		avago_addr_to_struct(sbus_addr_in, &addr_struct);
		ah->verbose = 0;
		avago_device_info_options(ah, &addr_struct, (Avago_ip_type_t) 0,
					  0);
		ah->verbose = prev_verbose;
	}

	avago_addr_to_struct(sbus_addr_in, &addr_struct);
	for (st =
	     avago_broadcast_first(ah, &addr_struct, &start, &stop, &next, 0);
	     st; st = avago_broadcast_next(ah, &next, &start, &stop, 0)) {
		uint32_t sbus_addr = avago_struct_to_addr(&next);

		if (avago_check_ip_type
		    (ah, sbus_addr, __func__, __LINE__, false, 1,
		     AVAGO_UNKNOWN_IP))
			continue;
		if (!avago_check_process
		    (ah, sbus_addr, __func__, __LINE__, true, 3,
		     AVAGO_PROCESS_A, AVAGO_PROCESS_B, AVAGO_PROCESS_F))
			return 0;
		if (avago_check_ip_type
		    (ah, sbus_addr, __func__, __LINE__, false, 3, AVAGO_SERDES,
		     AVAGO_M4, AVAGO_P1)) {
			avago_serdes_diag(ah, sbus_addr, config);
			continue;
		}

		if (config->sbus_dump)
			avago_diag_sbus_dump(ah, sbus_addr, config->binary);

	}
	return return_code == ah->return_code ? 0 : -1;
}

#endif
